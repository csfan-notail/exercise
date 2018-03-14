/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : redirect_portal.c
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/12/4
  Last Modified :
  Description   : 给予未认证终端做重定向portal页面
  Function List :
  History       :
  1.Date        : 2013/12/4
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <syslog.h>
#include <errno.h>

#include <sys/socket.h>
#ifdef GW_X86
#include <sys/epoll.h>
#endif
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <stdarg.h>
#include <pthread.h>

#include "httpd.h"
#include "lib_pa.h"

#include "common.h"
#include "conf.h"
#include "auth_server.h"
#include "client_list.h"
#include "http.h"
#include "log.h"
#include "util.h"
#include "firewall.h"
#include "redirect_portal.h"
#include "client_ap.h"

#include <signal.h>
#include <sys/time.h>


#ifdef GW_X86
int g_8061_epollfd[FIRST_PAGE_TASK_NUM];
int g_8061_fdmax[FIRST_PAGE_TASK_NUM] = {0};   //8061线程正在监听的fd总数
struct hlist_head g_stCSHashHead_8061[FIRST_PAGE_TASK_NUM][MAXSOCKFD];
extern long g_tcp_timeout;
#endif

int g_status_firstpage = 0;
int g_status_redirect[FIRST_PAGE_TASK_NUM] = {0};
int g_status_80 = 0;

extern int g_client_access_num;
extern pthread_mutex_t client_list_mutex;

extern char *g_pstAppleSuccess;

extern pthread_t first_page_handle_pid;
extern long g_tcp_timeout;

extern pthread_t wxl_handle_pid; //task#416 wechat connect wifi @2015.01.27

extern httpd *webserver;
httpd *g_redirectServer[FIRST_PAGE_TASK_NUM] = {0};
httpd *g_firstpagehttpdServer = NULL;
httpd *g_80httpdServer = NULL;

extern int wifidog_set_thread_name(EN_WIFIDOG_THREAD_INDEX enThreadIndex);

/**@ 客户端与网关交互socket, 以及数据区域 */
/*8061线程*/
request g_thread_8061_client_request[FIRST_PAGE_TASK_NUM][CLIENT_TCP_8061_NUM];
ssize_t g_thread_8061_client_bytes[FIRST_PAGE_TASK_NUM][CLIENT_TCP_8061_NUM];
size_t g_thread_8061_client_totalbytes[FIRST_PAGE_TASK_NUM][CLIENT_TCP_8061_NUM];
/*8062线程*/
request g_thread_8062_client_request[CLIENT_TCP_8062_NUM];
ssize_t g_thread_8062_client_bytes[CLIENT_TCP_8062_NUM];
size_t g_thread_8062_client_totalbytes[CLIENT_TCP_8062_NUM];
/*wxl 线程add@2015.01.27*/
request g_thread_80_client_request[CLIENT_TCP_80_NUM];
ssize_t g_thread_80_client_bytes[CLIENT_TCP_80_NUM];
size_t g_thread_80_client_totalbytes[CLIENT_TCP_80_NUM];

extern int user_access_init(void);

#ifdef GW_X86
#include <execinfo.h>

/*begin: add@2014.8.19 for print_trace when program exit()*/
void print_trace(void)
{
	void *array[128];
	size_t size;
	char **strings;
	size_t i;
	size = backtrace (array, 128);
	strings = (char **) backtrace_symbols (array, size);
	char buf[2048] = {0};
	int len = 0;

	for (i = 0; i < size; i++)
	{
		if(strings[i])
		{
			len += sprintf(buf + len, "%s\n", strings[i]);
		}
	}	
	sys_log_set("/var/wifidog_exit_stack.log", buf);
	free(strings);
}
/*end: add@2014.8.19 for print_trace when program exit()*/
#endif

pthread_mutex_t g_wdclear_mutex;

int do_wifidog_clear()
{
	int loop;
	static int do_clear = 0;
#if defined(BOARD_9341_21) || defined(GW_X86) || defined(AP9344_GW)
	char prog[64] = "wget";
#else
	char prog[64] = "httpdownload";
#endif
	char cmd[128];

	if(0 == config_get_config()->child_process)
		return 0;

	pthread_mutex_lock(&g_wdclear_mutex);

	if(0 == do_clear)
		do_clear = 1;
	else{
		pthread_mutex_unlock(&g_wdclear_mutex);
		return 0;
	}
	
	sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog(%s) do clearing",
		g_threadname[wifidog_get_thread_index((unsigned int)(pthread_self()))].name);

	fw_destroy();

	snprintf(cmd, 128, "killall -9 %s", prog);
	system(cmd);
	system("killall -9 downloadcloudsrc.sh");
	system("killall -9 httpgetinfo");

	snprintf(cmd, 128, "pidof %s", prog);
	if(0 == system(cmd)){ /*进程未杀死，根据pid来kill*/
		FILE *pfData = NULL;
		char cBuff[128] = {0};
		pfData = popen(cmd, "r");
		if(pfData != NULL){
			fread(cBuff, sizeof(cBuff)-1, 1, pfData);
			pclose(pfData);

			snprintf(cmd, 128, "kill -9 %s", cBuff);
			system(cmd);
		}
	}
	
	for(loop=0; loop<FIRST_PAGE_TASK_NUM; ++loop){
		if(g_redirectServer[loop] != NULL){
			close(g_redirectServer[loop]->serverSock);
		}
	}
	if(webserver != NULL){
		close(webserver->serverSock);
	}
	if(g_firstpagehttpdServer != NULL){
		close(g_firstpagehttpdServer->serverSock);
	}
	if(g_80httpdServer != NULL){
		close(g_80httpdServer->serverSock);
	}

	pthread_mutex_unlock(&g_wdclear_mutex);
	return 0;
}


#ifdef GW_X86

static int clientsock_hash_fn(int iClientSockFd)
{
	return ((iClientSockFd % MAXSOCKFD) < MAXSOCKFD ? (iClientSockFd % MAXSOCKFD) : MAXSOCKFD);
}

static void clientsock_hlist_init(int idx)
{
	int iLoop = 0;
	for(iLoop = 0; iLoop < MAXSOCKFD; iLoop++){
		INIT_HLIST_HEAD(&g_stCSHashHead_8061[idx][iLoop]);
	}
	return;
}

static int clientsock_hlist_add(int loop, int iClientSockFd, char *ipaddr)
{
	int iKey = -1;
	ClientSockList *tpos;
	struct hlist_node *pos,*n;
	iKey = clientsock_hash_fn(iClientSockFd);
	hlist_for_each_entry_safe(tpos, pos, n, &g_stCSHashHead_8061[loop][iKey], stNode){
		log_info(__LOG_DEBUG__, "already exist clientsock fd: %d, then don't add it", iClientSockFd);
		return 0;
	}
    tpos = (ClientSockList *)malloc(sizeof(ClientSockList));
    if (NULL == tpos)
    {
        log_info(__LOG_ERROR__,"malloc memory for client socket fd(%d) failed!", iClientSockFd);
        return -1;
    }
    memset(tpos, 0, sizeof(ClientSockList));
    tpos->stReq.clientSock = iClientSockFd;
    time(&tpos->stConnetTime);
	if(ipaddr)
    {
        strncpy(tpos->stReq.clientAddr, ipaddr, HTTP_IP_ADDR_LEN);
        tpos->stReq.clientAddr[HTTP_IP_ADDR_LEN-1]=0;
    }
    else
    {
        *(tpos->stReq.clientAddr) = 0;
    }
    tpos->stReq.readBufRemain = 0;
    tpos->stReq.readFlag = READ_FD_SET;
    tpos->stReq.clientSock_timeout = get_uptime();
    tpos->stReq.readBufPtr = NULL;
    hlist_add_head(&(tpos->stNode), &g_stCSHashHead_8061[loop][iKey]);
	
	log_info(__LOG_DEBUG__,"get new client(fd: %d, ipaddr: %s)", iClientSockFd, ipaddr); 				
	
    return 0;
}

static ClientSockList * clientsock_hlist_find(int loop, int iClientSockFd)
{
	int iKey = -1;
	ClientSockList *tpos;
	struct hlist_node *pos,*n;
	iKey = clientsock_hash_fn(iClientSockFd);
	hlist_for_each_entry_safe(tpos, pos, n, &g_stCSHashHead_8061[loop][iKey], stNode){
		log_info(__LOG_DEBUG__, "find it clientsock fd: %d", iClientSockFd);
		return tpos;
	}
	log_info(__LOG_ERROR__, "can't find it ,return NULL fd(%d)", iClientSockFd);
	
	return NULL;
}

static int clientsock_hlist_del(ClientSockList *pstNode)
{
	hlist_del(&pstNode->stNode);
	if (-1 != pstNode->stReq.clientSock){
		close(pstNode->stReq.clientSock);
	}
	free(pstNode);
	pstNode = NULL;
	
	return 0;
}

static void clientsock_timeout_check(int loop)
{
	int iLoop = 0; 
	ClientSockList *tpos = NULL;
	struct hlist_node *pos,*n;
	struct epoll_event stEvent;
	time_t stNowTime;
	time(&stNowTime);
	for (iLoop = 0; iLoop < MAXSOCKFD; iLoop++){
		hlist_for_each_entry_safe(tpos, pos, n, &g_stCSHashHead_8061[loop][iLoop], stNode){
			if (stNowTime - tpos->stConnetTime > g_tcp_timeout * 60){
				if (-1 == epoll_ctl(g_8061_epollfd[loop], EPOLL_CTL_DEL, tpos->stReq.clientSock, &stEvent))
				{
					log_info(__LOG_ERROR__,"del client fd from g_epollfd failed when timeout - %s", strerror(errno));
				}else{
					log_info(__LOG_DEBUG__,"del client fd [%d] in g_epollfd !",tpos->stReq.clientSock);
				}
				clientsock_hlist_del(tpos);				
				g_8061_fdmax[loop]--;
			}
		}
	}
}

static void redirect_portal_epoll(httpd *webServer)
{
	int iloop = 0;
	int ires = 0;
	int ifdnum = 0; /* 已有事件发生的fd  数  */
	unsigned int ulTicks = 0;
	char *ipaddr;

	struct sockaddr_in stclientaddr;
	int iClientSock = -1;
	int iSize = 0;
	ClientSockList *pstClientSockNode;
	int loop = webServer->idx;

	struct epoll_event stEvent;
	struct epoll_event stEpollEvents[MAXSOCKFD];

	clientsock_hlist_init(loop);
   
	g_8061_epollfd[loop] = epoll_create(MAXSOCKFD);
	if (-1 == g_8061_epollfd[loop])
	{
		log_info(__LOG_ERROR__, "epoll_create failed - %s",strerror(errno));		
		sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at redirect_portal_epoll1() ");
		exit(1);
	}
	
	stEvent.events = EPOLLIN;
	stEvent.data.fd = webServer->serverSock;

	if (-1 == epoll_ctl(g_8061_epollfd[loop], EPOLL_CTL_ADD, webServer->serverSock, &stEvent))
	{
		log_info(__LOG_ERROR__,"add server fd in g_8061_epollfd failed [fd = %d] - %s",
						webServer->serverSock, strerror(errno));
		sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at redirect_portal_epoll2() ");
		exit(1);
	}
	g_8061_fdmax[loop]++;
	
	/* 使用死循环监听客户端socket  */
	while(1)
	{
		ulTicks++;
		if (0 == ulTicks%30){			
			/*	调用超时处理函数  */
			clientsock_timeout_check(loop);
		}
		
		/*	epoll 监听的fd	数目超过门限(1000) */
		if (HTTP_CONNECT_MAX_NUM < g_8061_fdmax[loop])
		{
			log_info(__LOG_ERROR__, "g_8061_fdmax(%d) > HTTP_CONNECT_MAX_NUM", g_8061_fdmax[loop]);
		}
		
		/*	开始监听epoll  */
		ifdnum = epoll_wait(g_8061_epollfd[loop], stEpollEvents, g_8061_fdmax[loop], 1000);
		if (-1 == ifdnum)
		{
			if(errno != EINTR)
			{
				log_info(__LOG_ERROR__,"epoll_wait failed - %s",strerror(errno));
				sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at redirect_portal_epoll3() ");
				exit(1);
				return;
			}
		}

		g_status_redirect[loop] = (g_status_redirect[loop]+1)%262144;

		for (iloop = 0; iloop < ifdnum; iloop++)
		{
			if (webServer->serverSock == stEpollEvents[iloop].data.fd)
			{
				iSize = sizeof(stclientaddr);
				iClientSock = accept(webServer->serverSock, (struct sockaddr  *)&stclientaddr, (socklen_t *)&iSize);   
				
				if (0 > iClientSock)
				{
					log_info(__LOG_ERROR__,"server accept failed - %s",strerror(errno));
					continue ;
				}

				ipaddr = inet_ntoa(stclientaddr.sin_addr);

				stEvent.events = EPOLLIN;
				stEvent.data.fd = iClientSock;
				if (-1 == epoll_ctl(g_8061_epollfd[loop], EPOLL_CTL_ADD, iClientSock, &stEvent))
				{
					log_info(__LOG_ERROR__,"add client fd to g_epollfd failed, continue [fd = %d] - %s",
												   iClientSock, strerror(errno));
					close(iClientSock);
					continue ;
				}else{
					log_info(__LOG_DEBUG__,"add client fd [%d] to g_epollfd ",iClientSock);
				}
				if (-1 == clientsock_hlist_add(loop, iClientSock, ipaddr))
				{
					log_info(__LOG_ERROR__,"add client fd [%d] in ClientSock List failed !",iClientSock);
					if (-1 == epoll_ctl(g_8061_epollfd[loop], EPOLL_CTL_DEL, iClientSock, &stEvent))
					{
						log_info(__LOG_ERROR__,"add client fd in g_epollfd failed [fd = %d] - %s",
														iClientSock, strerror(errno));
					}
					close(iClientSock);
					continue;
				}
				g_8061_fdmax[loop]++;
			}
			else
		    {
				pstClientSockNode = clientsock_hlist_find(loop, stEpollEvents[iloop].data.fd);
				if (NULL == pstClientSockNode){
					log_info(__LOG_ERROR__, "can't find clientNode, fd is %d, then continue", stEpollEvents[iloop].data.fd);
					if (-1 == epoll_ctl(g_8061_epollfd[loop], EPOLL_CTL_DEL, iClientSock, &stEvent))
					{
						log_info(__LOG_ERROR__,"add client fd in g_epollfd failed [fd = %d] - %s",
														iClientSock, strerror(errno));
					}
					close(stEpollEvents[iloop].data.fd);
					g_8061_fdmax[loop]--;
					continue;
				}
				/* 接收socket  消息  */
				ires = recv(stEpollEvents[iloop].data.fd, 
							pstClientSockNode->stReq.readBuf, 
							sizeof(pstClientSockNode->stReq.readBuf), 0);
				if (0 < ires)
				{
					if(pstClientSockNode->stReq.readBuf[ires-1] == '\n'
						|| pstClientSockNode->stReq.readBuf[ires-1] == '}'  /*带有json数据*/
	                	|| !isascii(pstClientSockNode->stReq.readBuf[ires-1]))
	                {
	                    pstClientSockNode->stReq.readBuf[ires] = '\0';
	                    log_info(__LOG_DEBUG__, "Client Request Data [%s]", pstClientSockNode->stReq.readBuf);
						if (-1 == epoll_ctl(g_8061_epollfd[loop], EPOLL_CTL_DEL, stEpollEvents[iloop].data.fd, &stEvent))
						{
							log_info(__LOG_ERROR__,"del client fd in g_epollfd failed [fd = %d] - %s",
															iClientSock, strerror(errno));
						}
	                    process_http(webServer, &(pstClientSockNode->stReq));
						pstClientSockNode->stReq.readFlag = NOT_READ_FD_SET;
						clientsock_hlist_del(pstClientSockNode);
						g_8061_fdmax[loop]--;
						continue;
	                }
					else 
					{
						pstClientSockNode->stReq.readBuf[ires] = '\0';
						log_info(__LOG_DEBUG__, "ires: %d, buf: %s\n", ires, pstClientSockNode->stReq.readBuf);						
					}
				}
				else if (0 > ires){
					log_info(__LOG_ERROR__,"0 > ires, del node - %s",strerror(errno));
					/*表示需要继续recv. TODO*/
					if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN){
						log_info(__LOG_DEBUG__,"errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN");
					}
					goto UNSETFD;
				}else{
					/*	返回0则表示另一端已关闭 */
					log_info(__LOG_ERROR__,"0 = ires  failed, del node ");
					goto UNSETFD;
				}//end of if(0>res)				
UNSETFD:				
				if (-1 == epoll_ctl(g_8061_epollfd[loop], EPOLL_CTL_DEL, stEpollEvents[iloop].data.fd, &stEvent))
				{
					log_info(__LOG_ERROR__,"del client fd in g_epollfd failed [fd = %d] - %s",
													iClientSock, strerror(errno));
				}
				clientsock_hlist_del(pstClientSockNode);
				g_8061_fdmax[loop]--;
			}/*  end of if-else */
		}/*end of for*/
	}/* end of while */
}


#endif



/**begin: add@2014.12.26 ***/
static void redirect_portal_client_request_init(request *r, ssize_t *bytes, size_t *totalbytes, int max)
{
    int loop;
    
    memset(r, 0, sizeof(request) * max);
    for(loop = 0; loop < max; loop++)
    {
        r[loop].clientSock = SOCKET_INVALID;
        bytes[loop] = totalbytes[loop] = 0;
    }
    return;
}

/** @从client_request找一个暂未使用的 */
static request *redirect_portal_get_invalid_sockfd(request *r, int max)
{
    int loop;
    for(loop = 0; loop < max; loop++)
    {
        if(SOCKET_INVALID == r[loop].clientSock)
        {
            memset(r[loop].readBuf, 0, sizeof(r[loop].readBuf));
            log_info(__LOG_DEBUG__, "return loop [%d]", loop);
            return &r[loop];
        }
    }
    return NULL;
}

static void redirect_portal_socket_fd_set(int serverSock, request *pstReq, fd_set *fds, int *maxfd, int max)
{
    int loop;

    /** webserver FD_SET */
    FD_SET(serverSock, fds);
    *maxfd = update_max_fd(*maxfd, serverSock);

    for(loop = 0; loop < max; loop++)
    {
        if(SOCKET_INVALID != pstReq[loop].clientSock && READ_FD_SET == pstReq[loop].readFlag)
        {
            FD_SET(pstReq[loop].clientSock, fds);
            *maxfd = update_max_fd(*maxfd, pstReq[loop].clientSock);
        }
    }
    
    return;
}

static int redirect_portal_webserver_fd_isset(int serverSock, request *pstReq, fd_set *fds, int max)
{
    request *r; 
    struct sockaddr_in addr;
    char *ipaddr;
    size_t addrLen;

    if(FD_ISSET(serverSock, fds))
    {
        if(NULL == (r = redirect_portal_get_invalid_sockfd(pstReq, max)))
        {
            log_info(__LOG_WARNING__, "Could not find a invalid request data");
            return -1;
        }
        addrLen = sizeof(addr);
		
        r->clientSock = accept(serverSock, (struct sockaddr *)&addr, &addrLen);
        log_info(__LOG_DEBUG__, "accept a new client and socket %d", r->clientSock);
        if(-1 != r->clientSock)
        {
            ipaddr = inet_ntoa(addr.sin_addr);
            if(ipaddr)
            {
                strncpy(r->clientAddr, ipaddr, HTTP_IP_ADDR_LEN);
                r->clientAddr[HTTP_IP_ADDR_LEN-1]=0;
            }
            else
            {
                *(r->clientAddr) = 0;
            }
            r->readBufRemain = 0;
            r->readFlag = READ_FD_SET;
            r->clientSock_timeout = get_uptime();
            r->readBufPtr = NULL;
        }
    }
    return 0;
}

static int redirect_portal_client_fd_isset(httpd *webServer, request *r, int r_len, 
											      ssize_t *bytes, size_t *total_bytes, fd_set *fds)
{
    int loop;

    for(loop = 0; loop < r_len; loop++)
    {
        if(SOCKET_INVALID == r[loop].clientSock)
        {
            continue;
        }
        if(FD_ISSET(r[loop].clientSock, fds))
        {
            bytes[loop] = read(r[loop].clientSock, r[loop].readBuf + total_bytes[loop], HTTP_READ_BUF_LEN - total_bytes[loop]);
            if(bytes[loop] <= 0)
            {
            	log_info(__LOG_DEBUG__, "read error, r[%d].clientSock:%d, client_bytes[%d]: %d, %s", 
										loop, r[loop].clientSock, loop, bytes[loop], strerror(errno));
                close(r[loop].clientSock);
                r[loop].clientSock = SOCKET_INVALID;
                bytes[loop] = total_bytes[loop] = 0;
                continue;
            }
            else
            {
                total_bytes[loop] += bytes[loop];
                log_info(__LOG_DEBUG__, "r[%d].clientSock, Read %d bytes, total now %d",loop, bytes[loop], total_bytes[loop]);

				/*if(!strncmp(r[loop].readBuf, "check-status", strlen("check-status"))){
					log_info(__LOG_MSG__, "send status ok, r[%d].clientSock:%d, client_bytes[%d]: %d", 
										loop, r[loop].clientSock, loop, bytes[loop]);
					send(r[loop].clientSock, "status-ok", strlen ("status-ok"), 0);
					close(r[loop].clientSock);
					r[loop].clientSock = SOCKET_INVALID;
					bytes[loop] = total_bytes[loop] = 0;
					continue;
				}*/

                if(r[loop].readBuf[total_bytes[loop]-1] == '\n'
					|| r[loop].readBuf[total_bytes[loop]-1] == '}' /*带有json数据*/
                	|| !isascii(r[loop].readBuf[total_bytes[loop]-1]))
                {
                    r[loop].readBuf[total_bytes[loop]] = '\0';
                    log_info(__LOG_DEBUG__, "Client Request Data [%s], loop [%d]", r[loop].readBuf, loop);
                    bytes[loop] = total_bytes[loop] = 0;
                    r[loop].readFlag = NOT_READ_FD_SET;
                    process_http(webServer, &r[loop]);
                }
            }
        }
        else if(r[loop].readFlag != NOT_READ_FD_SET
            && ((get_uptime() - r[loop].clientSock_timeout) > g_tcp_timeout) )
        {
            close(r[loop].clientSock);
            r[loop].clientSock = SOCKET_INVALID;
            bytes[loop] = total_bytes[loop] = 0;
            log_info(__LOG_DEBUG__, "the client socket is time out [loop %d]", loop);
        }
    }

    return 0;
}

static void redirect_portal_select_loop(httpd *webServer,request *r,ssize_t *bytes,size_t *total_bytes, int max, int *status)
{
    int result;
    int maxfd = 0;
    fd_set fds;
    struct timeval timeout;

    redirect_portal_client_request_init(r, bytes, total_bytes, max);

    while(1)
    {        
        FD_ZERO(&fds);
        redirect_portal_socket_fd_set(webServer->serverSock, r, &fds, &maxfd, max);

        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        
        result = select(maxfd + 1, &fds, NULL, NULL, &timeout);
        if(result > 0)
        {
            /**
             *@检测是否有新的连接 webserver
             */
            redirect_portal_webserver_fd_isset(webServer->serverSock, r, &fds, max);

            /*
            **@读取终端的socket
            */
            redirect_portal_client_fd_isset(webServer, r, max, bytes, total_bytes, &fds);
            
        }
        else if(result == 0)
        {
            //
        }
        else
        {
            log_info(__LOG_WARNING__, "select error");
        }
		*status = (*status+1)%262144;
    }
    return;
}
/**end: add@2014.12.26 ***/

/*begin: add@2014.8.15 for add thread to handle first page */
void thread_first_page_handle(void)
{
    httpd *firstpagehttpdServer;
    s_config *config = config_get_config();

	sys_log_set(WIFIDOG_FIRSTPAGE_FILE, "Thread thread_first_page_handle() Start..");

	/*add@2015.01.28 SET THREAD NAME*/
	wifidog_set_thread_name(WIFIDOG_THREAD_8062);
	
	firstpagehttpdServer = httpdCreate(config->gw_address, config->gw_first_page_handle_port);
    if ( NULL == firstpagehttpdServer) 
	{
		log_info(__LOG_ERROR__, "Could not create first page handle httpd server: %s", strerror(errno));
		goto end;
	}
	g_firstpagehttpdServer = firstpagehttpdServer;
	log_info(__LOG_DEBUG__, "create first page server success.");
	//httpdAddC404Content(redirectServer, http_callback_404_8062);

	sys_log_set(WIFIDOG_FIRSTPAGE_FILE, "Create firstpagehttpdServer socket ok.");

	//终端first page请求页面发送到auth server
    httpdAddCContent(firstpagehttpdServer, "/rdaccess", "", 0, NULL, http_callback_404_8062);
    httpdAddCContent(firstpagehttpdServer, "/", "rdaccess", 0, NULL, http_callback_404_8062);

	/*微信连处理*/
	httpdAddCContent(firstpagehttpdServer, "/redirect", "", 0, NULL, http_wxl_redirect_callback);
	httpdAddCContent(firstpagehttpdServer, "/", "redirect", 0, NULL, http_wxl_redirect_callback);

    /*收到 alipay的离线token*/
	httpdAddCContent(firstpagehttpdServer, "/login", "", 0, NULL, http_alipay_redirect_callback);
	httpdAddCContent(firstpagehttpdServer, "/", "login", 0, NULL, http_alipay_redirect_callback);

	sys_log_set(WIFIDOG_FIRSTPAGE_FILE, "8062 task start.");

    redirect_portal_select_loop(firstpagehttpdServer, 
								g_thread_8062_client_request, 
								g_thread_8062_client_bytes, 
								g_thread_8062_client_totalbytes,
								CLIENT_TCP_8062_NUM,
								&g_status_firstpage);

end:
	sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at thread_first_page_handle()");
    exit(1);
    return;
}
/*end: add@2014.8.15 for add thread to handle first page */

int wifidog_check_http_port()
{
	char value[128];
	int port;
	
	if(sys_config_get(value, "http_port", 128) >= 0){
		port = atoi(value);
		if(port != 80)
			return 0;
	}

	log_info(__LOG_MSG__, "reset boa port to 8090");
	/*BOA端口号为80，应该重新设置其端口号*/
	system("killall boa");
	system("sed -i 's/^Port.*/Port 8090/' /tmp/boa/boa.conf");
	sleep(2);
	
	return -1;
}

/*处理http 80端口业务*/
void http_port80_handle(void)
{
	httpd *httpdServer;
	
	wifidog_set_thread_name(WIFIDOG_THREAD_80);

	/*检查是否和BOA端口冲突*/
	wifidog_check_http_port();
	
	httpdServer = httpdCreate(HTTP_ANY_ADDR, 80); /*监听所有80端口访问*/
	if ( NULL == httpdServer){
		log_info(__LOG_ERROR__, "Could not create http_port80_handle server: %s", strerror(errno));
		goto end;
	}
	g_80httpdServer = httpdServer;
	log_info(__LOG_DEBUG__, "create http_port80_handle server success.");

    /*锦江国际接口*/
	httpdAddCContent(httpdServer, "/bittel", "", 0, NULL, http_jinjiang_auth_callback);
	httpdAddCContent(httpdServer, "/", "bittel", 0, NULL, http_jinjiang_auth_callback);
	
	httpdAddCContent(httpdServer, "/openAccess", "", 0, NULL, http_jinjiang_auth_callback);
	httpdAddCContent(httpdServer, "/", "openAccess", 0, NULL, http_jinjiang_auth_callback);

	/*直接访问则重定向到BOA的8090端口*/
	httpdAddCContent(httpdServer, "/", "", 1, NULL, http_80_redirect2boa_callback);
	
	redirect_portal_select_loop(httpdServer, 
								g_thread_80_client_request, 
								g_thread_80_client_bytes, 
								g_thread_80_client_totalbytes,
								CLIENT_TCP_80_NUM,
								&g_status_80);

end:
	sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at wxl_handle()");
	exit(1);
	return;
}

/*end: add@2015.01.27 for wechat connect wifi*/
void *thread_redirect(void *args)
{
    httpd *redirectServer;
    s_config *config = config_get_config();
	int loop = (int)(*((int*)args));
	free(args);
	#if 0
    /*设置ip(2.2.2.2)为必须重定向*/
    if (0 != portal_add_dest_ip_must_redirect_list((unsigned)inet_addr("2.2.2.2"), 80))
    {
        sys_log_set(WIFIDOG_REDIRECT_FILE, "portal_add_dest_ip_must_redirect_list (2.2.2.2) failed");
    }
    else
    {
        sys_log_set(WIFIDOG_REDIRECT_FILE, "portal_add_dest_ip_must_redirect_list (2.2.2.2) ok");
    }
    #endif

	sys_log_set(WIFIDOG_REDIRECT_FILE, "Thread thread_redirect() Start.. loop(%d)", loop);

	/*add@2015.01.28 SET THREAD NAME*/
	if(FIRST_PAGE_TASK_NUM > 1){
		if(0 == loop)
			wifidog_set_thread_name(WIFIDOG_THREAD_8061);
		else
			wifidog_set_thread_name(WIFIDOG_THREAD_OTHER);
	}
	else
		wifidog_set_thread_name(WIFIDOG_THREAD_8061);

	sys_log_set(WIFIDOG_REDIRECT_FILE, "ios access init ok.. loop(%d)", loop);

    if ((redirectServer = httpdCreate(config->gw_address, config->gw_redirect_port+loop)) == NULL) 
	{
		log_info(__LOG_ERROR__, "Could not create redirect server: %s", strerror(errno));
		sys_log_set(WIFIDOG_REDIRECT_FILE, "Could not create redirect server: %s loop(%d)", strerror(errno), loop);
		goto end;
	}
	redirectServer->idx = loop;
	g_redirectServer[loop] = redirectServer;
	
	log_info(__LOG_DEBUG__, "create redirect server success.");

	sys_log_set(WIFIDOG_REDIRECT_FILE, "Create redirectServer socket ok. loop(%d)", loop);

	//httpdAddC404Content(redirectServer, http_callback_404);

	/*midified@2014.8.15*/
	//终端first page请求页面发送到auth server
    //httpdAddCContent(redirectServer, "/redirect", "", 0, NULL, http_callback_404);
    //httpdAddCContent(redirectServer, "/", "redirect", 0, NULL, http_callback_404);
	//终端未认证http报文网关wifidog给重定向到first page页面

	httpdAddC404Content(redirectServer, http_callback_redirect2local);

	sys_log_set(WIFIDOG_REDIRECT_FILE, "8061 task start. loop(%d)", loop);

#ifdef GW_X86
	redirect_portal_epoll(redirectServer);
#else
	redirect_portal_select_loop(redirectServer, 
								g_thread_8061_client_request[loop], 
								g_thread_8061_client_bytes[loop], 
								g_thread_8061_client_totalbytes[loop],
								CLIENT_TCP_8061_NUM,
								&g_status_redirect[loop]);
#endif
end:
	sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at thread_redirect() loop(%d)", loop);
    exit(1);
    return NULL;
}

