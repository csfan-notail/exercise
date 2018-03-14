/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : gateway.c
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/11/25
  Last Modified :
  Description   : 
  Function List :
  History       :
  1.Date        : 2013/11/25
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <stdarg.h>
#include <pthread.h>

#include "includes.h"
#include "httpd.h"
#include "conf.h"
#include "commandline.h"
#include "client_list.h"
#include "auth_server.h"
#include "firewall.h"
#include "util.h"
#include "log.h"
#include "config.h"
#include "common.h"
#include "http.h"
#include "redirect_portal.h"

#include "cloud_platform.h"
#include "gctl_thread.h"

#include "client_aglink.h"

/* The internal web server */
httpd *webserver = NULL;

/* Time when wifidog started */
//time_t started_time = 0;
long started_time = 0;

/**@ 客户端与网关交互socket, 以及数据区域 */
request client_request[CLIENT_TCP_8060_NUM];
pthread_mutex_t g_client_request_8060;

ssize_t client_bytes[CLIENT_TCP_8060_NUM];
size_t client_totalbytes[CLIENT_TCP_8060_NUM];

int g_status_main = 0;

/* tcp time out */
long g_tcp_timeout;

/* redirect thread */
pthread_t redirect_pid[FIRST_PAGE_TASK_NUM];
pthread_t platform_pid;
pthread_t gctl_pid;
pthread_t first_page_handle_pid;
pthread_t aglink_pid;     //task#416 wechat connect wifi @2015.02.03
pthread_t gMacroHot_pid;



#ifdef PCAP_SUPPORT
pthread_t pcap_pid; 
#endif

/*begin: add@2015.01.28*/
struct __thread_name g_threadname[WIFIDOG_THREAD_MAX] = {
	{ "WD_8060",             	    WIFIDOG_THREAD_MASTER },
	{ "WD_GCTL",         	        WIFIDOG_THREAD_GCTL },
	{ "WD_PLAT",  	                WIFIDOG_THREAD_PLATFORM },
	{ "WD_8061",          	        WIFIDOG_THREAD_8061 },
	{ "WD_8062",   	                WIFIDOG_THREAD_8062 },
	{ "WD_80",     	            	WIFIDOG_THREAD_80 },
	{ "WD_AGLINK",     	            WIFIDOG_THREAD_AGLINK },
	{ "WD_OTHER",     	        	WIFIDOG_THREAD_OTHER },
	{"WD_MACRO_HOT",           WIFIDOG_THREAD_MACRO_HOT},
	#ifdef PCAP_SUPPORT
      { "WD_PCAP",     	        	WIFIDOG_THREAD_PCAP },
      #endif
};

unsigned int g_stThreadId[WIFIDOG_THREAD_MAX];

int wifidog_set_thread_name(EN_WIFIDOG_THREAD_INDEX enThreadIndex)
{
	if (enThreadIndex > WIFIDOG_THREAD_MAX){
		return -1;
	}
	g_stThreadId[enThreadIndex] = (unsigned int)(pthread_self());
	return 0;
}
int wifidog_get_thread_index(unsigned int pid)
{
	int iLoop = 0;
	for (iLoop = 0; iLoop < WIFIDOG_THREAD_MAX; iLoop++){
		if (g_stThreadId[iLoop] == pid){
			return iLoop;
		}
	}
	return WIFIDOG_THREAD_OTHER;
}
/*end: add@2015.01.28*/

int g_debug_level = DEFAULT_DEBUGLEVEL;
int g_daemon = 1;

pid_t g_pid;

/*add@2014.9.10 */
#ifdef GW_X86
#define GW_INFO_HTML "/appfs/web/webpages/portal/gw_info.html"
#else
#define GW_INFO_HTML "/tmp/app/home/cloud/webpages/portal/gw_info.html"
#endif

extern pthread_mutex_t g_wdclear_mutex;
extern int do_wifidog_clear();
extern void print_trace();

/**@internal
 * @brief Handles SIGCHLD signals to avoid zombie processes
 *
 * When a child process exits, it causes a SIGCHLD to be sent to the
 * process. This handler catches it and reaps the child process so it
 * can exit. Otherwise we'd get zombie processes.
 */
void sigchld_handler(int s)
{
    log_info(__LOG_FAILED__, "wifidog into sigchld_handler s %d", s);

	do_wifidog_clear();

	sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog(%s) exit at sigchld_handler() %d",
		g_threadname[wifidog_get_thread_index((unsigned int)(pthread_self()))].name, s);

#ifdef GW_X86
	print_trace();
#endif

	exit(1);
}

/** Exits cleanly after cleaning up the firewall.  
 *  Use this function anytime you need to exit after firewall initialization */
void termination_handler(int s)
{
    log_info(__LOG_FAILED__, "wifidog into termination_handler s %d", s);
	
	do_wifidog_clear();
	
	sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog(%s) exit at termination_handler() %d",
		g_threadname[wifidog_get_thread_index((unsigned int)(pthread_self()))].name, s);

#ifdef GW_X86
	print_trace();
#endif

	exit(1);
}
extern pthread_mutex_t client_list_mutex;

/** SIGUSR1 print global */
void sigusr1_handler(int s)
{
	int loop;
    char temp[512];
    char client_list[4096];

    int total = 0;
    int client_num;
    t_client *p1;
	int len = 0;
	int len_t = 0;
	
	log_info(__LOG_FAILED__, "into sigusr1_handler s %d", s);
	for(loop = 0; loop < CLIENT_HASH_SIZE; loop++)
    { 
        pthread_mutex_lock(&client_list_mutex);
        p1 = g_client_head[loop].next;
        while(p1)
        {
			len = 0;
			len += sprintf(temp, "Client %d: ip: %s, mac: %s, auth_period: %d, incoming: %lld, outgoing: %lld \n"
                          "           idletimeout: %ld, forcetimeout: %ld, bw_up: %u, bw_down: %u, online_time_last_update_time: %ld\n"
                          "			  online_time: %ld, login_time: %ld, last_updated: %ld, fw_connection_state: %d\n"
                          "           token=%s\n", 
                    total+1, p1->ip, p1->mac, p1->auth_period, p1->counters.incoming, p1->counters.outgoing,
                    p1->idletimeout, p1->forcetimeout, p1->bw_up, p1->bw_down, p1->online_time_last_update_time, p1->online_time, p1->login_time,
                    p1->counters.last_updated, p1->fw_connection_state, p1->token);
			len += sprintf(temp + len, "-------------------------------------------------------------------\n");

			if (len >= 512)
			{
				log_info(__LOG_FAILED__, "len >= 512, client_list error");
				pthread_mutex_unlock(&client_list_mutex);
				return;
			}

			len_t += len;
			
			strcat(client_list, temp);
            client_num++;
            total++;
            p1 = p1->next;
            if(len_t >= 3800)
            {
                log_info(__LOG_ERROR__, "%s", client_list);
                client_num = 0;
				len_t = 0;
                memset(client_list, 0, sizeof(client_list));
            }
        }
        pthread_mutex_unlock(&client_list_mutex);
    }
	log_info(__LOG_ERROR__, "%s", client_list);
	log_info(__LOG_ERROR__, "total_num: %d", total);
}
/** SIGUSR2 print global */
extern int g_client_access_num;
void sigusr2_handler(int s)
{
    log_info(__LOG_FAILED__, "into sigusr2_handler s %d", s);
	log_info(__LOG_ERROR__, "config: ");
	log_info(__LOG_ERROR__, "config.wdctl_sock         		 : %s", config_get_config()->wdctl_sock);
	log_info(__LOG_ERROR__, "config.internal_sock      		 : %s", config_get_config()->internal_sock);	
	log_info(__LOG_ERROR__, "config.debuglevel         		 : %d", config_get_config()->debuglevel);
	log_info(__LOG_ERROR__, "config.external_interface 		 : %s", config_get_config()->external_interface);	
	log_info(__LOG_ERROR__, "config.gw_id              		 : %s", config_get_config()->gw_id);
	log_info(__LOG_ERROR__, "config.gw_interface       		 : %s", config_get_config()->gw_interface);	
	log_info(__LOG_ERROR__, "config.gw_address         		 : %s", config_get_config()->gw_address);
	log_info(__LOG_ERROR__, "config.ccs_server         		 : %s", config_get_config()->ccs_server);
	log_info(__LOG_ERROR__, "config.snmp_server         	 : %s", config_get_config()->snmp_server);
	log_info(__LOG_ERROR__, "config.max_client         		 : %d", config_get_config()->max_client);	
	log_info(__LOG_ERROR__, "config.dev_max_client           : %d", config_get_config()->dev_max_client);	
	log_info(__LOG_ERROR__, "g_client_access_num       		 : %d", g_client_access_num);
	log_info(__LOG_ERROR__, "config.conf_ver           		 : %lld", config_get_config()->conf_ver);	
	log_info(__LOG_ERROR__, "config.config_ver_tmp     		 : %lld", config_get_config()->config_ver_tmp);
	log_info(__LOG_ERROR__, "config.wan_flow_report_switch   : %d", config_get_config()->wan_flow_report_switch);	
	log_info(__LOG_ERROR__, "config.wan_flow_report_interval : %d", config_get_config()->wan_flow_report_interval);
}

/** @internal 
 * Registers all the signal handlers
 */
static void init_signals(void)
{
	struct sigaction sa;

	log_info(__LOG_DEBUG__, "Initializing signal handlers");

	#if 0
	sa.sa_handler = sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		log_info(__LOG_ERROR__, "sigaction(): %s", strerror(errno));
		sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at init_signals()");
		exit(1);
	}
	#endif

	/* Trap SIGPIPE */
	/* This is done so that when libhttpd does a socket operation on
	 * a disconnected socket (i.e.: Broken Pipes) we catch the signal
	 * and do nothing. The alternative is to exit. SIGPIPE are harmless
	 * if not desirable.
	 */
	sa.sa_handler = SIG_IGN;
	if (sigaction(SIGPIPE, &sa, NULL) == -1) {
		log_info(__LOG_ERROR__, "sigaction(): %s", strerror(errno));
		sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at init_signals() SIGPIPE");
		exit(1);
	}

	sa.sa_handler = termination_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	/* Trap SIGTERM */
	if (sigaction(SIGTERM, &sa, NULL) == -1) {
		log_info(__LOG_ERROR__, "sigaction(): %s", strerror(errno));
		sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at init_signals() SIGTERM");
		exit(1);
	}

	/* Trap SIGQUIT */
	if (sigaction(SIGQUIT, &sa, NULL) == -1) {
		log_info(__LOG_ERROR__, "sigaction(): %s", strerror(errno));
		sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at init_signals() SIGQUIT");
		exit(1);
	}

	/* Trap SIGINT */
	if (sigaction(SIGINT, &sa, NULL) == -1) {
		log_info(__LOG_ERROR__, "sigaction(): %s", strerror(errno));
		sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at init_signals() SIGINT");
		exit(1);
	}

    /* Trap SIGSEGV */
    if (sigaction(SIGSEGV, &sa, NULL) == -1) {
		log_info(__LOG_ERROR__, "sigaction(): %s", strerror(errno));
		sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at init_signals() SIGSEGV");
		exit(1);
	}

	/* Trap SIGTSTP */
	if (sigaction(SIGTSTP, &sa, NULL) == -1) {
		log_info(__LOG_ERROR__, "sigaction(): %s", strerror(errno));
		sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at init_signals() SIGTSTP");
		exit(1);
	}

	/*begin: add@2014.8.11 add SIGUSR1 & SIGUSR2*/
	/* SIGUSR1*/
	sa.sa_handler = sigusr1_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGUSR1, &sa, NULL);
	/* SIGUSR2*/
	sa.sa_handler = sigusr2_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGUSR2, &sa, NULL);
	/*end: add@2014.8.11 add SIGUSR1 & SIGUSR2*/
	
	pthread_mutex_init(&g_wdclear_mutex, NULL);	
	
	return;
}

static void client_request_init(int max)
{
    int loop;
    
    memset(client_request, 0, sizeof(request) * max);
    for(loop = 0; loop < max; loop++)
    {
        client_request[loop].authSock = SOCKET_INVALID;
        client_request[loop].clientSock = SOCKET_INVALID;
        client_bytes[loop] = client_totalbytes[loop] = 0;
        //client_auth_bytes[loop] = client_auth_totalbytes = 0;
    }
    return;
}

void set_lan_info()
{
	int num, iLoop;
	char value[512];
	char name[64];

	char startip[32] = {0};   /* 起始IP */
    char endip[32] = {0};     /* 终止IP */
    char mask[32] = {0};      /* 子网掩码 */
    char gateway[32] = {0};   /* 网关 */
    char dns[32] = {0};       /* 主DNS */
    char sub_dns[32] = {0};   /* 备DNS */
    char lease[32] = {0};     /* 该地址池所分配的租约时间 */
    char vlan_band[128] = {0}; /* 该地址池所绑定的VLAN */
    char lan_ip[32] = {0};    /* 该地址池LAN口的IP */
    char lan_mask[32] = {0};  /* 该地址池LAN口的掩码 */
	int br_x;

	s_config *config = config_get_config(); 
	lan_info_t *pstLanInfo = NULL, *pstLanInfoTmp;

	LANINFO_LIST_LOCK();

	list_for_each_entry_safe(pstLanInfo, pstLanInfoTmp, &(config->stLanInfo), stLanInfo)
	{
		list_del_init(&pstLanInfo->stLanInfo);
		free(pstLanInfo);
	}

	sys_config_get(value, "dhcp_num", 512);
	num = atoi(value);

    for (iLoop = 0; iLoop < num; ++iLoop)
    {
        sprintf(name, "dhcp_%d", iLoop);
		sys_config_get(value, name, 512);
        sscanf(value, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^:]:%[^:]:%[^,],%[^,],%d!",  //存在nvram中的格式
	        startip,
	        endip,
	        mask,
	        gateway,
	        dns,
	        sub_dns,
	        lease,
	        vlan_band,
	        lan_ip,
	        lan_mask,
	        &br_x
	        );
		log_info(__LOG_DEBUG__, "get dhcp gateway %s", gateway);

		/*添加一组gateway_ip/mask信息*/
		pstLanInfo = (lan_info_t *)malloc(sizeof(lan_info_t));
		if(pstLanInfo != NULL){
			snprintf(pstLanInfo->name, sizeof(pstLanInfo->name), "br%d", iLoop);
			pstLanInfo->uiIp = (unsigned int)inet_addr(gateway);
			pstLanInfo->uiMask = (unsigned int)inet_addr(mask);
			pstLanInfo->uiDns = (unsigned int)inet_addr(dns);
			pstLanInfo->uiSubDns = (unsigned int)inet_addr(sub_dns);
			list_add_tail(&pstLanInfo->stLanInfo, &config->stLanInfo);
		}
    }

	LANINFO_LIST_UNLOCK();

	sys_config_get(value, "dhcp_option43", 512);
	config->uiOption43 = (unsigned int)inet_addr(value);

	return;
}


/** @从client_request, authserver 找一个暂未使用的 */
request *get_invalid_sockfd(request *r, int max)
{
    int loop;
    for(loop = 0; loop < max; loop++)
    {
        if(SOCKET_INVALID == r[loop].authSock
            && SOCKET_INVALID == r[loop].clientSock)
        {
            memset(r[loop].readBuf, 0, sizeof(r[loop].readBuf));
            log_info(__LOG_DEBUG__, "return loop [%d]", loop);
            return &r[loop];
        }
    }
    return NULL;
}

static void socket_fd_set(fd_set *fds, int *maxfd, int max)
{
    int loop;

    /** webserver FD_SET */
    FD_SET(webserver->serverSock, fds);
    *maxfd = update_max_fd(*maxfd, webserver->serverSock);

    for(loop = 0; loop < max; loop++)
    {
        if(SOCKET_INVALID != client_request[loop].authSock)
        {
            FD_SET(client_request[loop].authSock, fds);
            *maxfd = update_max_fd(*maxfd, client_request[loop].authSock);
        }

        if(SOCKET_INVALID != client_request[loop].clientSock
            && READ_FD_SET == client_request[loop].readFlag)
        {
            FD_SET(client_request[loop].clientSock, fds);
            *maxfd = update_max_fd(*maxfd, client_request[loop].clientSock);
        }
    }
    
    return;
}

static int webserver_fd_isset(fd_set *fds, int max)
{
    request *r; 
    struct sockaddr_in addr;
    char *ipaddr;
    size_t addrLen;

    if(FD_ISSET(webserver->serverSock, fds))
    {
        if(NULL == (r = get_invalid_sockfd(client_request, max)))
        {
            log_info(__LOG_WARNING__, "Could not find a invalid request data");
            return -1;
        }
        addrLen = sizeof(addr);
        r->clientSock = accept(webserver->serverSock, (struct sockaddr *)&addr, &addrLen);
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

static int client_fd_isset(request *r, int r_len, fd_set *fds)
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
            client_bytes[loop] = read(r[loop].clientSock, r[loop].readBuf + client_totalbytes[loop], HTTP_READ_BUF_LEN - client_totalbytes[loop]);
            if(client_bytes[loop] <= 0)
            {
                log_info(__LOG_DEBUG__, "read error, r[%d].clientSock:%d, client_bytes[%d]: %d, %s", 
										loop, r[loop].clientSock, loop, client_bytes[loop], strerror(errno));
                close(r[loop].clientSock);
                r[loop].clientSock = SOCKET_INVALID;
                client_bytes[loop] = client_totalbytes[loop] = 0;
                continue;
            }
            else
            {
                client_totalbytes[loop] += client_bytes[loop];
                log_info(__LOG_DEBUG__, "r[%d].clientSock, Read %d bytes, total now %d",loop, client_bytes[loop], client_totalbytes[loop]);

                if(r[loop].readBuf[client_totalbytes[loop]-1] == '\n'
					|| r[loop].readBuf[client_totalbytes[loop]-1] == '}' /*带有json数据*/
                	|| !isascii(r[loop].readBuf[client_totalbytes[loop]-1]))
                {
                    r[loop].readBuf[client_totalbytes[loop]] = '\0';
                    log_info(__LOG_DEBUG__, "Client Request Data [%s], loop [%d]", r[loop].readBuf, loop);
                    client_bytes[loop] = client_totalbytes[loop] = 0;
                    r[loop].readFlag = NOT_READ_FD_SET;
                    process_http(webserver, &r[loop]);
                }
            }
        }
        else if(r[loop].readFlag != NOT_READ_FD_SET
            && ((get_uptime() - r[loop].clientSock_timeout) > g_tcp_timeout) )
        {
            close(r[loop].clientSock);
            r[loop].clientSock = SOCKET_INVALID;
            client_bytes[loop] = client_totalbytes[loop] = 0;
            log_info(__LOG_DEBUG__, "the client socket is time out [loop %d]", loop);
        }
    }

    for(loop = 0; loop < r_len; loop++)
    {
        if(SOCKET_INVALID == r[loop].authSock)
        {
            continue;
        }
        if(FD_ISSET(r[loop].authSock, fds))
        {
            client_bytes[loop] = read(r[loop].authSock, r[loop].readBuf + client_totalbytes[loop], HTTP_READ_BUF_LEN - client_totalbytes[loop]);
            if(client_bytes[loop] < 0)
            {
                log_info(__LOG_ERROR__, "read error, r[%d].authSock:%d, client_bytes[%d]: %d, %s", 
										loop, r[loop].authSock, loop, client_bytes[loop], strerror(errno));
                close(r[loop].authSock);
                r[loop].authSock = SOCKET_INVALID;
                client_bytes[loop] = client_totalbytes[loop] = 0;

                /* @同时关闭终端tcp连接 */
                if(SOCKET_INVALID != r[loop].clientSock)
                {
                    close(r[loop].clientSock);
                    r[loop].clientSock = SOCKET_INVALID;   
                }
            }
            else if(client_bytes[loop] == 0)
            {
                client_totalbytes[loop] += client_bytes[loop];
                log_info(__LOG_DEBUG__, "r[%d].authSock, Read %d bytes, total now %d",loop, client_bytes[loop], client_totalbytes[loop]);
                r[loop].readBuf[client_totalbytes[loop]] = '\0';
                client_bytes[loop] = client_totalbytes[loop] = 0;
				/*auth result from auth server to wifidog:
				  *[HTTP/1.1 200 OK#015#012Date: Thu, 31 Jul 2014 11:10:40 GMT#015#012Server: Apache/2.4.7 (Unix) PHP/5.4.16#015#
				  *012X-Powered-By: ThinkPHP#015#012Set-Cookie: PHPSESSID=8di39ighbi2fdskbn4gle52ur3; path=/#015#012Expires: Thu, 
				  *19 Nov 1981 08:52:00 GMT#015#012Cache-Control: private#015#012Pragma: no-cache#015#012Content-Length: 33#015
				  *#012Connection: close#015#012Content-Type: text/html; charset=utf-8#015#012#015#012#015#012#015#012#015#012?
				  *Auth: 1#0120 0 0 0 200 8640]
				  */
                authenticate_client(&r[loop]);
                close(r[loop].authSock);
                r[loop].authSock = SOCKET_INVALID;
            }
            else
            {
                client_totalbytes[loop] += client_bytes[loop];
                log_info(__LOG_DEBUG__, "r[%d].authSock, Read %d bytes, total now %d, loop %d",loop, client_bytes[loop], client_totalbytes[loop], loop);
            }
        }
        else if((get_uptime() - r[loop].authSock_timeout) > g_tcp_timeout)
        {
            close(r[loop].authSock);
            r[loop].authSock = SOCKET_INVALID;
            client_bytes[loop] = client_totalbytes[loop] = 0;

            /* @同时关闭终端tcp连接 */
            if(SOCKET_INVALID != r[loop].clientSock)
            {
                close(r[loop].clientSock);
                r[loop].clientSock = SOCKET_INVALID;
            }
            log_info(__LOG_DEBUG__, "the auth socket is time out [loop %d]", loop);
        }
    }
    return 0;
}


static void select_loop(int max)
{
    int result;
    int maxfd = 0;
    fd_set fds;
    struct timeval timeout;

    client_request_init(max);
	sys_log_set(WIFIDOG_MAINLOOP_FILE, "Begin to select.");
    while(1)
    {        
        FD_ZERO(&fds);
        socket_fd_set(&fds, &maxfd, max);

        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        
        result = select(maxfd + 1, &fds, NULL, NULL, &timeout);
        if(result > 0)
        {
            /**
             *@检测是否有新的连接 webserver
             */
             
            pthread_mutex_lock(&g_client_request_8060);
            webserver_fd_isset(&fds, max);

            /*
            **@读取终端的socket
            */
            client_fd_isset(client_request, max, &fds);
            pthread_mutex_unlock(&g_client_request_8060);
            
        }
        else if(result == 0)
        {
            //
        }
        else
        {
            log_info(__LOG_WARNING__, "select error");
            // 如何操作此select的错误
        }
		g_status_main = (g_status_main+1)%262144;
    }
    return;
}

extern int user_access_init(void);
extern int init_httpd_decode();
extern pthread_t http_port80_handle_pid;
extern void http_port80_handle(void);
extern void thread_pcap(void);

static void main_loop(void)
{
	s_config *config;
	int ping_idx = 0;
	int loop;
	int *idx;
	int count;

	sys_log_set(WIFIDOG_MAINLOOP_FILE, "Thread main_loop() Start..");

	wifidog_set_thread_name(WIFIDOG_THREAD_MASTER);

    config = config_get_config();
	config->child_process = 1;
    g_pid = getpid();

    /* Set the time when wifidog started */
    if(!started_time)
    {
        log_info(__LOG_DEBUG__, "Setting started_time");
        started_time = get_uptime();
    }

    /* Set tcp time out */
    g_tcp_timeout = config->tcp_timeout;

	sys_log_set(WIFIDOG_MAINLOOP_FILE, "Get gateway ip addr");

    /** @获取网关IP地址,未检测到地址配置则退出程序 */
    if(!config->gw_address)
    {
		char acIp[64] = {0};
		if(get_iface_ip(config->gw_interface, acIp, sizeof(acIp)) >= 0)
			config->gw_address = safe_strdup(acIp);

        if(!config->gw_address)
        {
            log_info(__LOG_ERROR__, "Could not get IP address information of %s, exiting...", config->gw_interface);
			sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at main_loop() gw_address");
			exit(1);
        }
    }

	/*先清除iptables规则配置*/
	fw_destroy();

	sys_log_set(WIFIDOG_MAINLOOP_FILE, "Get gateway mac addr step 1.");
    /** @如果热点标志没有配置, 则获取该网卡的信息作为热点标识 */
    if(!config->gw_id)
    {
		char acMac[64] = {0};
		if(get_iface_mac(config->gw_interface, acMac, sizeof(acMac)) >= 0)
			config->gw_id = safe_strdup(acMac);

		if(!config->gw_id)
        {
            log_info(__LOG_ERROR__, "Could not get MAC address information of %s, exiting...", config->gw_interface); 
			sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at main_loop() gw_id");
			exit(1);
        }
    }

	sys_log_set(WIFIDOG_MAINLOOP_FILE, "Get gateway mac addr step 2.");
    /** @ 获取wan口 mac地址 */
    if(config->wan_interface)
    {
		char acMac[64] = {0};
		if(get_iface_mac(config->wan_interface, acMac, sizeof(acMac)) >= 0)
			config->wan_mac = safe_strdup(acMac);

		if(!config->wan_mac)
        {
            log_info(__LOG_ERROR__, "Could not get MAC address information of %s, exiting...", config->wan_interface); 
			sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at main_loop() wan_interface");
			exit(1);
        }
    }
    
    log_info(__LOG_DEBUG__, "gw_interface %s: gw_address = %s, gw_id = %s, wan_interface = %s, wan_mac = %s", 
        config->gw_interface, config->gw_address, config->gw_id, config->wan_interface, config->wan_mac);

    /** 
     * @初始化webserver : 8060
     */
    pthread_mutex_init(&g_client_request_8060,NULL);
     if(0 == access(WIFIDOG_MARCO_HOT_LOG,F_OK))
     {
        remove(WIFIDOG_MARCO_HOT_LOG);
     }
     
	log_info(__LOG_DEBUG__, "Creating web server on %s:%d", config->gw_address, config->gw_port);
	if ((webserver = httpdCreate(config->gw_address, config->gw_port)) == NULL) 
	{
		log_info(__LOG_ERROR__, "Could not create web server: %s", strerror(errno));
		sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at main_loop() webserver");
		exit(1);
	}

	/**
	 * @在ping台注册测设备, 注册成功后走后续流程
	 */
	sys_log_set(WIFIDOG_SERVER_LOG_FILE, "Set off-line by main_loop.");
	sys_log_set(WIFIDOG_MAINLOOP_FILE, "Set off-line by main_loop.");
	set_is_online(OFF_LINE);

	sys_log_set(WIFIDOG_MAINLOOP_FILE, "Create aglink task.");
	/*add@2015.02.03 for add thread aglink client*/	
	if(pthread_create(&aglink_pid, NULL, (void *)thread_aglink_client, NULL) != 0)
	{
		log_info(__LOG_ERROR__, "Failed to create a new thread (thread_aglink_client) - exiting");
		sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at main_loop() thread_aglink_client");
		exit(1);
	}
	log_info(__LOG_DEBUG__, "Create a new thread (thread_aglink_client) success. pid %u", (unsigned int)aglink_pid);
	pthread_detach(aglink_pid);

	set_lan_info();

	user_access_init();
	init_httpd_decode();

	sys_log_set(WIFIDOG_MAINLOOP_FILE, "Create redirect task.");
	/**@ 启动重定向进程(9061 ... ):  处理未认证的*/
	for(loop=0; loop<FIRST_PAGE_TASK_NUM; ++loop){
		idx = (int *)malloc(sizeof(int));
		if(idx != NULL){
			*idx = loop;
		    if(pthread_create(&redirect_pid[loop], NULL, (void *)thread_redirect, idx) != 0)
		    {
		        log_info(__LOG_ERROR__, "Failed to create a new thread (redirect) - exiting");
				sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at main_loop() thread_redirect. loop(%d)", loop);
		        exit(1);
		    }
	    	log_info(__LOG_DEBUG__, "Create a new thread (redirect) success. pid %u", (unsigned int)redirect_pid[loop]);
	    	pthread_detach(redirect_pid[loop]);
		}
	}

	/*add@2014.8.15*/
	/**@ first_page 处理线程(8062): 处理first page 的http报文*/
	if(pthread_create(&first_page_handle_pid, NULL, (void *)thread_first_page_handle, NULL) != 0)
	{
		log_info(__LOG_ERROR__, "Failed to create a new thread (first page handle) - exiting");
		sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at thread_redirect() thread_first_page_handle");
		exit(1);
	}
	pthread_detach(first_page_handle_pid);
	log_info(__LOG_DEBUG__, "Create a new thread (first page handle) success. pid %u", (unsigned int)first_page_handle_pid);

      #ifdef PCAP_SUPPORT
      sys_log_set(WIFIDOG_MAINLOOP_FILE, "Create pcap task.");
      if(pthread_create(&pcap_pid, NULL, (void*)thread_pcap, NULL) != 0)
      {
           log_info(__LOG_ERROR__, "Failed to create a new thread (gctl) - exiting");
           sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at main_loop() thread_pcap");
           exit(1);
      }
      log_info(__LOG_DEBUG__, "Create a new thread (pcap) success. pid %u", (unsigned int)pcap_pid);
      pthread_detach(pcap_pid);
      #endif


	sys_log_set(WIFIDOG_MAINLOOP_FILE, "Create gctl task.");
    /**@ 启动命令行线程 */
    if(pthread_create(&gctl_pid, NULL, (void*)thread_gctl, NULL) != 0)
    {
        log_info(__LOG_ERROR__, "Failed to create a new thread (gctl) - exiting");
		sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at main_loop() thread_gctl");
        exit(1);
    }
    log_info(__LOG_DEBUG__, "Create a new thread (gctl) success. pid %u", (unsigned int)gctl_pid);
    pthread_detach(gctl_pid);

	if(SERVER_MODE_WIFIDOG == config->server_mode) /*wifidog 接口云平台*/
	{
		log_info(__LOG_MSG__, "Server mode is wifidog.");
		sys_log_set(WIFIDOG_MAINLOOP_FILE, "Start first ping.");
		while(first_ping())
		{
			if(0 == ping_idx){
	        	sleep(3);
				ping_idx = 1;
			}
			else if(1 == ping_idx){
				sleep(10);
				ping_idx = 2;
			}
			else if(2 == ping_idx){
				sleep(30);
				ping_idx = 3;
			}
			else
				sleep(60);
		}
		log_info(__LOG_DEBUG__, "Device registration is successful");
	    /* 进程启动后,下载资源  add by zhz */
		sys_log_set(WIFIDOG_MAINLOOP_FILE, "Download source.");
	    /*first ping后可能外存还未挂载起来，第一次下载放到常规ping response里执行*/
	    /*download_src();*/ 
		
		/* @获取平台终端列表 */
		sys_log_set(WIFIDOG_MAINLOOP_FILE, "Update sta list...");
		count = first_client_request();
	    if(count < 0){
			sys_log_set(WIFIDOG_MAINLOOP_FILE, "Update sta list failed.");
	    }
		else
			sys_log_set(WIFIDOG_MAINLOOP_FILE, "Update sta list Succeed. count(%d)", count);

	    /* @获取NF模块终端列表 */
	    //TODO...
	
		sys_log_set(WIFIDOG_MAINLOOP_FILE, "Registor callbacks.");
	    log_info(__LOG_DEBUG__, "Assigning callbacks to web server");
	    httpdAddCContent(webserver, "/", "wifidog", 0, NULL, http_callback_wifidog);
		httpdAddCContent(webserver, "/wifidog", "", 0, NULL, http_callback_wifidog);
		//httpdAddCContent(webserver, "/wifidog", "about", 0, NULL, http_callback_about);
		//httpdAddCContent(webserver, "/wifidog", "status", 0, NULL, http_callback_status);
		/*终端认证请求页面发送给auth server*/
		httpdAddCContent(webserver, "/wifidog", "auth", 0, NULL, http_callback_auth); 
		httpdAddCContent(webserver, "/wifidog", "roaming", 0, NULL, http_callback_roaming); 
		//httpdAddC404Content(webserver, http_callback_404);
		/*add@2014.11.14 终端获取自己mac方法*/
	    httpdAddCContent(webserver, "/wifidog", "get_mac", 0, NULL, http_callback_sta_get_itsmac);
		/*获取终端认证状态*/
	    httpdAddCContent(webserver, "/wifidog", "get_auth", 0, NULL, http_callback_sta_get_auth);
		/*add@2014.12.25 用户下线处理*/
	    httpdAddCContent(webserver, "/wifidog", "userlogout", 0, NULL, http_callback_sta_logout);
		/*任务检测*/
	    httpdAddCContent(webserver, "/wifidog", "useraccess", 0, NULL, http_callback_sta_access);
        /*接第三方ap iPhone自动弹portal机制*/
	    httpdAddCContent(webserver, "/wifidog", "thirdpartyap", 0, NULL, http_callback_thirdpartyap);
	}
	else if(SERVER_MODE_THIRD_PARTY == config->server_mode) /*第三方接口云平台*/
	{
		char ip[64];
		log_info(__LOG_MSG__, "Server mode is third party.");

		while(addr_to_ip_string(config->auth_servers->authserv_hostname, ip, 64) != 0)
			sleep(5);

		config->auth_servers->last_ip = safe_strdup(ip);
		log_info(__LOG_MSG__, "Get Third Party Server IP %s", config->auth_servers->last_ip);

		if(pthread_create(&http_port80_handle_pid, NULL, (void *)http_port80_handle, NULL) != 0)
		{
			log_info(__LOG_ERROR__, "Failed to create a new thread (http_port80_handle) - exiting");
			sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at conf_set_task()");
			exit(1);
		}
		pthread_detach(http_port80_handle_pid);
		log_info(__LOG_MSG__, "Create a new thread (http_port80_handle) success. pid %u", 
			(unsigned int)http_port80_handle_pid);

		set_is_online(ON_LINE);
	}
	else{
		log_info(__LOG_MSG__, "Server mode is %d.", config->server_mode);
		sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at main_loop() server_mode is %d.",
			config->server_mode);
		exit(0);
	}

	sys_log_set(WIFIDOG_MAINLOOP_FILE, "Init fw.");
    /** @初始化firewall配置 */
    if(fw_init(config))
    {
        log_info(__LOG_ERROR__, "Failed to initialize firewall");
		sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at main_loop() firewall");
        exit(1);
    }

	sys_log_set(WIFIDOG_MAINLOOP_FILE, "Init qos.");
    /** @初始化流控配置 */
    fw_qos_init();

	if(SERVER_MODE_WIFIDOG == config->server_mode) /*wifidog 接口云平台*/
	{
		sys_log_set(WIFIDOG_MAINLOOP_FILE, "create platform task.");
	    /**@ 启动与平台交互线程 */
	    if(pthread_create(&platform_pid, NULL, (void *)thread_platform, NULL) != 0)
	    {
	        log_info(__LOG_ERROR__, "Failed to create a new thread (platform) - exiting");
			sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at main_loop() thread_platform");
	        exit(1);
	    }
	    log_info(__LOG_DEBUG__, "Create a new thread (platform) success. pid %u", (unsigned int)platform_pid);
	    pthread_detach(platform_pid);
	}

	sys_log_set(WIFIDOG_MAINLOOP_FILE, "select_loop start.");
    select_loop(CLIENT_TCP_8060_NUM);

    return;
}

/*begin:网间漫游2015-8-28*/
extern int g_macro_hot_thread_close;
void thread_macro_hot_deal(int iStatus)
{
    s_config *config;
    config = config_get_config();
    if(2 == iStatus && 0 == config->macro_hotspot_thread_status)
    {/*开启线程*/
        g_macro_hot_thread_close=0;
        if(pthread_create(&gMacroHot_pid, NULL, (void *)thread_macro_hot, NULL) != 0)
        {
               log_info(__LOG_ERROR__, "Failed to create a new thread (macro_hot) - exiting");
               sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at main_loop() thread_macro_hot");
               exit(1);
        }
        log_info(__LOG_DEBUG__, "Create a new thread (platform) success. pid %u", (unsigned int)gMacroHot_pid);
        pthread_detach(gMacroHot_pid);
    }
    else if(0 == iStatus)
    {
        g_macro_hot_thread_close=1;
    }

    
}
/*end:网间漫游2015-8-28*/

void create_station_page()
{
	FILE* fh;
	char fp_file_path[128];
	int result = 0;
	s_config	*config = config_get_config();
	
	char province_code[64] = {0};
	char city_code[64] = {0};
	char area_code[64] = {0};
	char biz_code[64] = {0};
	char station_id[64] = {0};
	char station_name[64] = {0};
	char station_identity[64] = {0};
	
	char station_page[1024];

	char dev_sn[64] = {0};
	sys_config_get(dev_sn, "device_sn", 64);

	snprintf(station_identity, 64, "%s", config->gw_id);
	
	snprintf(station_page, 1024, 
		 	"{\"resultCode\":%d,"
		 	"\"data\":\"{\\\"province_code\\\":\\\"%s"
		 	"\\\",\\\"city_code\\\":\\\"%s"
		 	"\\\",\\\"area_code\\\":\\\"%s"
		 	"\\\",\\\"biz_code\\\":\\\"%s"
		 	"\\\",\\\"station_id\\\":\\\"%s"
		 	"\\\",\\\"station_name\\\":\\\"%s"
		 	"\\\",\\\"station_identity\\\":\\\"%s"
		 	"\\\",\\\"station_sn\\\":\\\"%s"
		 	"\\\",\\\"station_cloud\\\":\\\"http://%s\\\"}\"}",
		 	result,
		 	province_code,
		 	city_code,
		 	area_code,
		 	biz_code,
		 	station_id,
		 	station_name,
		 	station_identity,
		 	dev_sn,
		 	(config->auth_servers ? (config->auth_servers->authserv_hostname 
			? config->auth_servers->authserv_hostname : "") : ""));

	sprintf(fp_file_path, "%s%s", LIGHTTPD_ROOT_DOC, STATION_PAGE);
	if ((fh = fopen(fp_file_path, "w+")) != NULL) {
		fwrite(station_page, strlen(station_page), 1, fh);
		fclose(fh);
	}
	else
	{
		log_info(__LOG_ERROR__, "fopen file: %s fail. ", fp_file_path);
	}
}


extern int client_ap_status_init(void);
extern int sta_dct_init(void);

int main(int argc, char **argv)
{
    s_config *config = NULL;
	FILE* fh;
	char gw_info_page[512] = {0};

	sys_log_set(WIFIDOG_LOG_FILE, ">>>>>>>>>>> wifidog main start.");

    config = config_get_config();
    /** 
     * 初始化时需要对config进行一次清空, 
     * 防止数据存储不合法值 
     */
    memset(config, 0, sizeof(s_config));

    config_init();
    parse_commandline(argc, argv);

    #ifdef OPENWRT
    get_wifidog_conf();
    set_wifidog_conf();
    #endif

	/* Initialize the config */
	config_read(config->configfile);

    /*获取wan口ip*/
    char acWanIp[64] = {0};
    if(get_iface_ip(config->wan_interface, acWanIp, sizeof(acWanIp)) >= 0){
		config->wan_address = (char *)malloc(sizeof(char)*64);
		if(config->wan_address != NULL){
		    strncpy(config->wan_address, acWanIp, 64);
		    sys_log_set(WIFIDOG_DEBUG_LOG_FILE,"config get wan_ip:%s",config->wan_address);
		}
    }

	/* add@2014.9.10 */
	if (config->gw_id){
		#ifndef OPENWRT
		system("chmod 777 "GW_INFO_HTML);
		#endif
		sprintf(gw_info_page, "<HTML><HEAD><TITLE>%s</TITLE></HEAD><BODY><center><h1>%s<h1></center></BODY></HTML>", config->gw_id, config->gw_id);
		if ((fh = fopen(GW_INFO_HTML, "w+")) != NULL) {
			fwrite(gw_info_page, strlen(gw_info_page), 1, fh);
			fclose(fh);
    	}
		else
		{
			log_info(__LOG_ERROR__, "fopen file: %s fail. can't create html page", GW_INFO_HTML);
		}
		log_info(__LOG_ERROR__, "create gw_info.html page");
	}

    /**
     * 检测gw,auth_server 是否有效, 无效则推出程序 
     */
	config_validate();

	/* Initializes the linked list of connected clients */
	client_list_init();

	/* Initializes ap-sta list*/
	client_ap_status_init();

	/* Initializes sta-dct*/
	sta_dct_init();

	/* Init the signals to catch chld/quit/etc */
	init_signals();

	create_station_page();

    if(config->daemon)
    {
        log_info(__LOG_DEBUG__, "Forking into background");
        switch(safe_fork()) {
			case 0: /* child */
				setsid();
				main_loop();
				break;

			default: /* parent */
				exit(0);
				break;
		}
    }
    else
    {
        main_loop();
    }

    return SUCCESS;
}

