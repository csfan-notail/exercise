/*
 * Copyright (C) GBCOM ltd.,co 2015
 *
 * This is a part of project tr69cd, which implements TR069 protocal(aka
 * CWMP).
 * 
 * This is a PRIVATE software.
 * All rights reserved.
 */
 
#include <stdio.h>
#include <stdlib.h>
#define _GNU_SOURCE
#include <string.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <syslog.h>
#include <unistd.h>
#include <fcntl.h>

#include "appdefs.h"
#include "utils.h"
#include "SOAPParser/CPEframework.h"
#include "SOAPParser/RPCState.h"
#include "event.h"
#include "webproto/wget.h"
#include "httpProto.h"

#include "webproto/protocol.h"
#include "webproto/www.h"
#include "SOAPParser/xmlParserSM.h"
#include "SOAPParser/xmlTables.h"
#include "SOAPParser/RPCState.h"

#include "informer.h"
//#include "pal_data.h"
//#define DEBUG

#include "uci_config.h"
#include "tr69cdefs.h" /* defines for ACS state */
 #include "tr69_lua.h"
extern ACSState    acsState;

extern eInformState informState;
extern unsigned int uiConnectState;

static void startACScallback(void *handle);

typedef struct ACSConnection {
    AuthState cState; /* authentication state */
    tProtoCtx   *cpc;   /* so we can use wget proto functions */
    int         lfd;    /* listener socket */
    tHttpHdrs   hdrs;
} ACSConnection;

static ACSConnection  connection;
static SessionAuth acsConnSess;

WanState wanState;

unsigned int uiTr69cPort = TR69C_PORT;   /* add by zhanghaozhong in order to get val from WEB */
extern int g_log_level;

static void free_http_headers(tHttpHdrs *hdrs) {
    CookieHdr	*cp=NULL, *last=NULL;    
    free(hdrs->content_type);
	hdrs->content_type=NULL;
    free(hdrs->protocol);
	hdrs->protocol=NULL;
    free(hdrs->wwwAuthenticate);
	hdrs->wwwAuthenticate=NULL;
    free(hdrs->Authorization);
	hdrs->Authorization=NULL;
    free(hdrs->TransferEncoding);
	hdrs->TransferEncoding=NULL;
    free(hdrs->Connection);
	hdrs->Connection=NULL;
    free(hdrs->method);
	hdrs->method=NULL;
    free(hdrs->path);
	hdrs->path=NULL;
    free(hdrs->host);
	hdrs->host=NULL;
    cp = hdrs->setCookies;
    while (cp) {
        last = cp->next;
        free(cp->name);
		cp->name=NULL;
        free(cp->value);
		cp->value=NULL;
        free(cp);
		cp=NULL;
        cp = last;
    }
    free(hdrs->message);
	hdrs->message=NULL;
    free(hdrs->locationHdr);
	hdrs->locationHdr=NULL;
    free(hdrs->filename);
	hdrs->filename=NULL;
    free(hdrs->arg);
	hdrs->arg=NULL;
    // do not free(hdrs) since it's needed for Connection Request
    memset(hdrs, 0, sizeof(tHttpHdrs));
}

int initACSConnectionURL(void)
{
    char buf[50];
   
    if (!getWanInfo(&wanState)) {
    //	return -1;
    }
	/*altered by caibing*/
#if 0
    char acURL[64] ={0};
   	char *pcValue =NULL;
	if(0 != uci_config_get(UCI_NETWORK_BR_LAN_IPADDR,acURL))		
 	{			
			pcValue = strdup("");			
			syslog(3,"ERROR");			
			return -1;		
	}		
	else		
	{			
			pcValue = strdup(acURL);

	}
	
	wanState.ip=atoi(pcValue);
	
	if(pcValue)
	{
		free(pcValue);
		pcValue=NULL;
	}
#endif
    snprintf(buf, sizeof(buf), "http://%s:%d%s", writeIp(wanState.ip),
    	uiTr69cPort, ACSCONNPATH);
    slog(TR69C_LOG_INFO, "cpe's URL %s\n", buf);
    slog(TR69C_LOG_DEBUG, "cpe's URL %s\n", buf);

    if (acsState.connReqURL)
    {
		free(acsState.connReqURL);
		acsState.connReqURL=NULL;
	}
    	
    acsState.connReqURL =  strdup(buf);
    if (acsState.connReqPath)
    {
		free(acsState.connReqPath);
		acsState.connReqPath=NULL;
	}
    	
    acsState.connReqPath = strdup(ACSCONNPATH);
    return 0;
}

static int testChallenge( ACSConnection *cd)
{
    return ( parseAuthorizationHdr(cd->hdrs.Authorization, &acsConnSess,
                                    acsState.connReqUser,acsState.connReqPwd));
}

void statusWriteComplete(void *handle)
{
    ACSConnection *cd = (ACSConnection*)handle;
    free_http_headers(&cd->hdrs);
    proto_FreeCtx(cd->cpc);
    memset(cd, 0, sizeof(struct ACSConnection));
    setTimer(startACScallback, NULL, 2000);   /* may want smarter time value here ????*/
}

static int sendOK( ACSConnection *cd)
{
    char    response[300];
    int     i;

    uiConnectState = 1; /* 1£ºÒÑ½¨Á¢Ô¶³ÌÁ¬½ÓÇëÇó */
    i = snprintf(response, sizeof(response), "HTTP/1.1 200 OK\r\n");
    i+= snprintf(response+i, sizeof(response)-i, "Content-Length: 0\r\n\r\n");
    if ( proto_Writen(cd->cpc, response, i) < i)
        return 0;
    return 1;
}

static int sendAuthFailed( ACSConnection *cd )
{
    char    response[300];
    int     i;

    i = snprintf(response, sizeof(response), "HTTP/1.1 401 Unauthorized\r\n");
    i+= snprintf(response+i, sizeof(response)-i, "Content-Length: 0\r\n\r\n");
    if ( proto_Writen(cd->cpc, response, i) < i)
        return 0;
    return 1;
}

static int sendChallenge( ACSConnection *cd )
{
    char    response[300];
    char    *h;
    int     i;

    i = snprintf(response, sizeof(response), "HTTP/1.1 401 Unauthorized\r\n");
    i+= snprintf(response+i, sizeof(response)-i, "Content-Length: 0\r\n");
    h = generateWWWAuthenticateHdr( &acsConnSess, ACSREALM, ACSDOMAIN, cd->hdrs.method);
    i+= snprintf(response+i, sizeof(response)-i,"%s\r\n\r\n", h);
    free(h);
	h=NULL;
    if ( proto_Writen(cd->cpc, response, i) < i)
        return 0;
    return 1;
}
/**
 * A connected ACS is sending us data,
 * Our action is to generate a digest authentication challange
 * with a 401 Unauthorized status code and
 * wait for the response to the challange. Then  send a 
 * 200 OK or a 401 Unauthorized. */
static void acsReadData(void *handle) 
{
    ACSConnection *cd = (ACSConnection *)handle;
   /* Free resources allocated earlier */
    free_http_headers(&cd->hdrs);
    if ( proto_ParseRequest(cd->cpc, &cd->hdrs) == 0 ){
		proto_ParseHdrs(cd->cpc, &cd->hdrs);
        if (!strcasecmp("http/1.1", cd->hdrs.protocol))
        {
			/* protocol is correct */
			if(acsState.connReqPath==NULL)
			{
				acsState.connReqPath = strdup("/");
			}
			
			
           if (!strcmp(acsState.connReqPath,cd->hdrs.path)) {
                /* path is correct proceed with authorization */
                if (acsState.connReqUser==NULL || acsState.connReqUser[0]=='\0')
                    cd->cState = sAuthenticated;
			fprintf(stderr, " acsReadData cd->cState = %d \n ",cd->cState);
				if (cd->cState == sIdle) {
                 	if(acsState.noneConnReqAuth) {
						fprintf(stderr, " acsState.noneConnReqAuth ok!!!  \n");
                        cd->cState = sAuthenticated;
                        sendOK(cd);
						fprintf(stderr, " acsState.noneConnReqAuth ok   0  \n");
                        // avoid race condition between periodic inform and connection request inform
			//			resetPeriodicInform(acsState.informInterval); 		//ÖØÐÂÉèÖÃÐÄÌø ÉÏ±¨Ê±¼ä
						fprintf(stderr, " acsState.noneConnReqAuth ok   1  \n");
                        notifyCallbacks(&informState);  /* this will initiate the inform code ·¢ËÍ 6 ConnectReq*/
						fprintf(stderr, " acsState.noneConnReqAuth ok   2  \n");
                        cd->cState = sShutdown;
                        setListenerType(cd->cpc->fd, statusWriteComplete,cd, iListener_Write);
						fprintf(stderr, " acsState.noneConnReqAuth ok   3  \n");
                        return;
                    } else {
                    	fprintf(stderr, " acsState.noneConnReqAuth No!!!  \n");
                        /* send 401 with digest challange */
                        sendChallenge( cd );
						fprintf(stderr, " acsState.noneConnReqAuth No   0 \n");
                        cd->cState = sAuthenticating;
                        setListener(cd->cpc->fd, acsReadData, (void *)cd);
						fprintf(stderr, " acsState.noneConnReqAuth No   1  \n");
                        return;
                    }
                } else if (cd->cState == sAuthenticating) {
                    if ( testChallenge( cd ) ) {
						fprintf(stderr, " acsState.noneConnReqAuth 1 Y 0\n");
                        sendOK(cd);
						/*add by duanguiyuan 2060320 BEGIN*/
						
						/*add by duanguiyuan 2060320 END*/
						fprintf(stderr, " acsState.noneConnReqAuth 1 Y 1\n");
                        notifyCallbacks(&informState);  /* this will initiate the inform code ·¢ËÍ 6 ConnectReq*/
						fprintf(stderr, " acsState.noneConnReqAuth 1 Y 2\n");
                        cd->cState = sShutdown;
                    }
                    else {
                        slog(TR69C_LOG_DEBUG, "ConnectRequest authentication error");
                        sendAuthFailed(cd);
                        cd->cState = sShutdown;
                    }
                    setListenerType(cd->cpc->fd, statusWriteComplete,cd, iListener_Write);
					fprintf(stderr, " acsState.noneConnReqAuth 1 Y end\n");
                    return;
                } else if (cd->cState == sAuthenticated) {
                    sendOK(cd);
					fprintf(stderr, " acsState.noneConnReqAuth 2 Y 0\n");
                    // avoid race condition between periodic inform and connection request inform
			//		resetPeriodicInform(acsState.informInterval); 		//ÖØÐÂÉèÖÃÐÄÌø ÉÏ±¨Ê±¼ä
					fprintf(stderr, " acsState.noneConnReqAuth 2 Y 1\n");
                    notifyCallbacks(&informState);  /* this will initiate the inform code ·¢ËÍ 6 ConnectReq*/
					fprintf(stderr, " acsState.noneConnReqAuth 2 Y 2\n");
                    cd->cState = sShutdown;
					fprintf(stderr, " acsState.noneConnReqAuth 2 Y 3\n");
                    setListenerType(cd->cpc->fd, statusWriteComplete,cd, iListener_Write);
					fprintf(stderr, " acsState.noneConnReqAuth 2 Y end\n");
                    return;
                }
            }
        }
			
    } else {
        if (g_debug)
        fprintf(stderr, "acsListener Error reading response\n");
    }
    cd->cState = sShutdown;
    setListenerType(cd->cpc->fd, statusWriteComplete, cd, iListener_Write);
}


/**
 * A client is trying to connect to us.
 */
static void acsConnect(void *handle) 
{
    int res;
    struct sockaddr_in addr;
    ACSConnection *cd = (ACSConnection *) handle;
    socklen_t sz = sizeof(struct sockaddr_in);
    int     flags=1;
    int     fd;
    stopListener(cd->lfd);
    memset(&addr, 0, sz);
    if (( fd = accept(cd->lfd, (struct sockaddr *)&addr, &sz)) < 0)
    {
        slog(TR69C_LOG_ERR, "acsListen accept failed errno=%d.%s",errno, strerror(errno));
        close(cd->lfd);
        setTimer(startACScallback, NULL, 5000 ); /* reenable listen in 5 sec */
        return; /* return errno */
    }
    close(cd->lfd); /* close the listener socket - only one connection at a time */
    cd->lfd = 0;
    cd->cpc = proto_NewCtx(fd);
    if ( (res = setsockopt(cd->cpc->fd, SOL_SOCKET, SO_REUSEADDR, &flags,sizeof(flags)))<0)
        slog(TR69C_LOG_ERR, "proxy_connection() setsockopt error %d %d %s", cd->cpc->fd, errno, strerror(errno));

    setListener(cd->cpc->fd, acsReadData, cd);
}
/* 
* return -1: for error
*       != -1 is socket
*/                              
static int initSocket(unsigned int ip, int port) 
{
    struct sockaddr_in addr;
    int port_sock = 0;
    int res, i = 1;
    port_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (port_sock < 0)
    {
        slog(TR69C_LOG_ERR, "acsListener web: init_listen_socket(port=%d), socket failed", port);
        return -1;
    }

    res = setsockopt(port_sock, SOL_SOCKET, SO_REUSEADDR, (char*) &i, sizeof(i));
    if (res < 0)
    {
        slog(TR69C_LOG_ERR,"acsListener web: %s", "Socket error listening to ACS");
        close(port_sock);
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family       = AF_INET;
  //  addr.sin_addr.s_addr  = htonl(ip);
	addr.sin_addr.s_addr  = htonl(inet_addr("0.0.0.0"));
    addr.sin_port         = htons(port);

    res = bind(port_sock, (struct sockaddr *)&addr, sizeof(addr));
    if (res < 0)
    {
        slog(TR69C_LOG_ERR, "acsListener bind failed errno=%d.%s",errno, strerror(errno));
        close(port_sock);
        return -1;
    }

    res = listen(port_sock,10);
    if (res < 0)
    {
        slog(TR69C_LOG_ERR, "acsListener listent failed errno=%d.%s",errno, strerror(errno));
        close(port_sock);
        return -1;
    }
    return port_sock;
}  
                        
static void startACScallback(void *handle) {
    ACSConnection *cd = &connection;

    /* setup path for acs connection */
   //	if ((cd->lfd =initSocket(INADDR_ANY, uiTr69cPort)) < 0)
   if ((cd->lfd =initSocket(INADDR_ANY, uiTr69cPort)) < 0)
   {   
   		setTimer(startACScallback, NULL, 5000 ); /* retry init_socket in 5 sec */
   		printf("acsListener socket error!!\n");
   }else
     {   setListener(cd->lfd, acsConnect, cd );
		 slog(TR69C_LOG_ERR, "acsListener socket ok!!");
		 printf("acsListener socket ok!!\n");
	}
}


/* 
* Listen for connections from the ACS
*/
void startACSListener(void)
{
    memset(&connection,0, sizeof(struct ACSConnection));
    startACScallback(NULL);

}
