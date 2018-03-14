#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <syslog.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <stdarg.h>
#include <pthread.h>

#include "httpd.h"
#include "common.h"
#include "conf.h"
#include "auth_server.h"
#include "http.h"
#include "log.h"
#include "cJSON.h"
#include "util.h"
#include "client_list.h"
#include "firewall.h"
#include "redirect_portal.h"
#include "client_ap.h"
#include "cloud_platform.h"

#include <signal.h>
#include <sys/time.h>

extern long g_tcp_timeout;
extern int wifidog_set_thread_name(EN_WIFIDOG_THREAD_INDEX enThreadIndex);
extern int event_map_register(int event_code, 
                               int time_interval, 
                               int timeout_count, 
                               void (*request_func)(),
                               void (*response_func)());

/*sta_key线程*/
request g_thread_stakey_client_request[CLIENT_TCP_NUM];
ssize_t g_thread_stakey_client_bytes[CLIENT_TCP_NUM];
size_t g_thread_stakey_client_totalbytes[CLIENT_TCP_NUM];
char   g_acStaKey[STRING_LEN_EX]={0};
int sta_update_keyinfo(cJSON *data, void *out)
{
    s_config *config = NULL;
    config = config_get_config();
    
    JSON_GET_OBJ_STRING(data,"result",config->acStaKey,"");
    
    strncpy(g_acStaKey,config->acStaKey,sizeof(config->acStaKey));
    return 0;
}
void sta_get_keyinfo(int *sockfd)
{
    char *requestpath="/emria/device/getSecretKey";
    char *host="testmh.cwifi.cn";
    http_get_url_info(requestpath,host,8086,"","",HTTP_REQ_TYPE_SYNC,sta_update_keyinfo,NULL);
    return;
}
void http_callback_get_keyinfo(httpd *webserver, request *r)
{
    char  acDataBuf[STRING_LEN_EX*10]={0};
    char *client_mac = NULL;
    s_config *config = NULL;
    config = config_get_config();
	client_mac = arp_get(r->clientAddr);
    sprintf(acDataBuf,"jsonString={\"secretKey\"=\"%s\",\"userMac\"=\"%s\"\"deviceMac\"=\"%s\"\"deviceSn\"=\"%s\"}",
        g_acStaKey,client_mac,config->wan_mac,"121212");
	free(client_mac);
	strcpy(r->response.headers,
	"Server: Embedded Server\n"); 
	strcpy(r->response.contentType, "text/html");
	strcpy(r->response.response,"200 Output Follows\n");
	r->response.headersSent = 0;
    httpdOutput(r, acDataBuf);
}

/**begin: add@2014.12.26 ***/
static void sta_key_client_request_init(request *r, ssize_t *bytes, size_t *totalbytes, int max)
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

static void sta_key_socket_fd_set(int serverSock, request *pstReq, fd_set *fds, int *maxfd)
{
    int loop;

    /** webserver FD_SET */
    FD_SET(serverSock, fds);
    *maxfd = update_max_fd(*maxfd, serverSock);

    for(loop = 0; loop < CLIENT_TCP_NUM; loop++)
    {
        if(SOCKET_INVALID != pstReq[loop].clientSock && READ_FD_SET == pstReq[loop].readFlag)
        {
            FD_SET(pstReq[loop].clientSock, fds);
            *maxfd = update_max_fd(*maxfd, pstReq[loop].clientSock);
        }
    }
    
    return;
}

/** @从client_request找一个暂未使用的 */
static request *sta_key_get_invalid_sockfd(request *r, int max)
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

static int sta_key_webserver_fd_isset(int serverSock, request *pstReq, fd_set *fds)
{
    request *r; 
    struct sockaddr_in addr;
    char *ipaddr;
    size_t addrLen;

    if(FD_ISSET(serverSock, fds))
    {
        if(NULL == (r = sta_key_get_invalid_sockfd(pstReq, CLIENT_TCP_NUM)))
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

static int sta_key_client_fd_isset(httpd *webServer, request *r, int r_len, 
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
                log_info(__LOG_ERROR__, "read error, r[%d].clientSock:%d, client_bytes[%d]: %d, %s", 
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

                if(r[loop].readBuf[total_bytes[loop]-1] == '\n'
                || !isascii(r[loop].readBuf[total_bytes[loop]-1]))
                {
                    r[loop].readBuf[total_bytes[loop]] = '\0';
                    log_info(__LOG_DEBUG__, "Client Request Data [%s], loop [%d]", r[loop].readBuf, loop);
                    bytes[loop] = total_bytes[loop] = 0;
                    r[loop].readFlag = NOT_READ_FD_SET;
		log_info(__LOG_ERROR__, "===============process_http");
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

static void sta_key_select_loop(httpd *webServer,request *r,ssize_t *bytes,size_t *total_bytes)
{
    int result;
    int maxfd = 0;
    fd_set fds;
    struct timeval timeout;

    sta_key_client_request_init(r, bytes, total_bytes, CLIENT_TCP_NUM);
    while(1)
    {        
        FD_ZERO(&fds);
        sta_key_socket_fd_set(webServer->serverSock, r, &fds, &maxfd);

        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        result = select(maxfd + 1, &fds, NULL, NULL, &timeout);
        if(result > 0)
        {
		log_info(__LOG_ERROR__, "===============select success");
            /**
             *@检测是否有新的连接 webserver
             */
            sta_key_webserver_fd_isset(webServer->serverSock, r, &fds);

            /*
            **@读取终端的socket
            */
            sta_key_client_fd_isset(webServer, r, CLIENT_TCP_NUM, bytes, total_bytes, &fds);
            
        }
        else if(result == 0)
        {
		log_info(__LOG_ERROR__, "===============select timeout");
            //
        }
        else
        {
		log_info(__LOG_ERROR__, "===============select fail");
            log_info(__LOG_WARNING__, "select error");
        }
    }
    return;
}

void thread_sta_key_handle(void)
{
    httpd *staKeyServer;
    s_config *config = config_get_config();

	/*add@2015.01.28 SET THREAD NAME*/
	wifidog_set_thread_name(WIFIDOG_THREAD_STAKEY);
    //if ((staKeyServer = httpdCreate(config->gw_address, config->gw_sta_key_port)) == NULL) 
    if ((staKeyServer = httpdCreate(NULL,config->gw_sta_key_port)) == NULL) 
	{
	    
		log_info(__LOG_ERROR__, "Could not create sta key server: %s", strerror(errno));
        sleep(15);
		goto end;
	}
		log_info(__LOG_ERROR__, "create sta key server success");
        sleep(15);
	log_info(__LOG_DEBUG__, "create sta key success.");
    httpdAddCContent(staKeyServer, "", "getSecretKeyInfo", 0, NULL, http_callback_get_keyinfo);
    sta_key_select_loop(staKeyServer, 
								g_thread_stakey_client_request, 
								g_thread_stakey_client_bytes, 
								g_thread_stakey_client_totalbytes);

end:
	sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at thread_sta_key_handle()");
    exit(1);
    return;
}
