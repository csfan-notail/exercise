/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : wget.c
  版 本 号   : 初稿
  作    者   : zhanghaozhong
  生成日期   : 2013年5月16日
  最近修改   :
  功能描述   : wget
  函数列表   :
              addCookieHdr
              addPostHdr
              do_connect
              do_connect_ssl
              do_resolve
              do_response
              do_send_request
              freeCookies
              freeData
              freeHdrs
              getticks
              report_status
              send_get_request
              send_post_request
              send_put_request
              send_request
              timer_connect
              timer_response
              wget_AddPostHdr
              wget_ClearPostHdrs
              wget_Connect
              wget_Disconnect
              wget_GetData
              wget_LastErrorMsg
              wget_PostData
              wget_PostDataClose
              wget_PutData
  修改历史   :
  1.日    期   : 2013年5月16日
    作    者   : zhanghaozhong
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
extern int g_log_level;
/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <sys/time.h>
#include <signal.h>
#include <ctype.h>
#include <syslog.h>

#include "appdefs.h"
#include "utils.h"
#ifdef USE_SSL
#include <openssl/ssl.h>
#endif
#include "event.h"
#include "informer.h"

#include "protocol.h"
#include "www.h"
#include "wget.h"
#include "httpProto.h"
/* 外部变量 */
extern unsigned int uiAcsIP;


#define BUF_SIZE 1024
//#define DEBUG

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
#ifdef  DEBUG
    #define mkstr(S) # S
    #define setListener(A,B,C) {fprintf(stderr,mkstr(%s setListener B fd=%d\n), getticks(), A);\
setListener( A,B,C);}

    #define setListenerType(A,B,C,E) {fprintf(stderr,mkstr(%s setListenerType B-E fd=%d\n), getticks(), A);\
setListenerType( A,B,C,E);}

    #define stopListener(A) {fprintf(stderr,"%s stopListener fd=%d\n", getticks(), A);\
stopListener( A );}

static char timestr[40];
static char *getticks()
{
   struct timeval now;
   gettimeofday( &now,NULL);
   sprintf(timestr,"%04ld.%06ld", now.tv_sec%1000, now.tv_usec);
   return timestr;
}
#endif
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/*----------------------------------------------------------------------*
 * forwards
 *----------------------------------------------------------------------*/
static void timer_connect(void *p);
static void do_connect(void *p);
static void do_connect_ssl(void *p, int errorcode);
static void timer_response(void *p);
static void do_response(void *p);
static void do_send_request(void *p, int errorcode);
static char noHostConnectMsg[] = "Could not establish connection to host %s(%s):%d";
static char noHostResolve[] = "Could not resolve host %s";
static char lastErrorMsg[255];

static void freeHdrs( XtraPostHdr **p )
{
   XtraPostHdr *next = *p;
   while( next ) {
      XtraPostHdr *temp;
      temp = next->next;
      free(next->hdr);
	  next->hdr=NULL;
      free(next->value);
	  next->hdr=NULL;
      free(next);
	  next=NULL;
      next = temp;
   }
}

static void freeCookies( CookieHdr **p )
{
   CookieHdr *next = *p;
   while( next ) {
      CookieHdr *temp;
      temp = next->next;
      free(next->name);
	  next->name=NULL;
      free(next->value);
	  next->value=NULL;
      free(next);
	  next=NULL;
      next = temp;
   }
}
/*----------------------------------------------------------------------*/
static void freeData(tWgetInternal *wg)
{
   if (wg != NULL) {
      if (wg->pc != NULL) {
         proto_FreeCtx(wg->pc);
         wg->pc = NULL;
      }
      free(wg->proto);
	  wg->proto=NULL;
      free(wg->host);
	  wg->host=NULL;
      free(wg->uri);
	  wg->uri=NULL;
      if (wg->hdrs != NULL) 
         proto_FreeHttpHdrs(wg->hdrs);
      freeCookies( &wg->cookieHdrs );
      freeHdrs( &wg->xtraPostHdrs );
      // don't free content_type and postdata since they're only pointers, there are no strdup for them.
      free(wg);
	  wg=NULL;
   }
}

typedef enum {
   REPLACE,
   ADDTOLIST
} eHdrOp;

static int addCookieHdr( CookieHdr **hdrQp, char *cookieName, char *value, eHdrOp replaceDups)
{
   CookieHdr *xh;
   CookieHdr **p = hdrQp;
   xh= (CookieHdr *)malloc( sizeof(struct CookieHdr));
   memset(xh, 0, sizeof(struct CookieHdr));
   if (xh) {
      xh->name = strdup(cookieName);
      xh->value = strdup(value);
      if (replaceDups == REPLACE) {
         while (*p) {
            CookieHdr *xp = *p;
            if ( strcasecmp(xp->name, xh->name)==0) {
               /* replace header */
               xh->next = xp->next;
               free(xp->name);
			   xp->name=NULL;
               free(xp->value);
			   xp->value=NULL;
               free(xp);
			   xp=NULL;
               *p = xh;
               return 1;
            }
            p = &xp->next;
         }
      }
      /* just stick it at beginning of list */
      xh->next = *hdrQp;
      *hdrQp = xh;
      return 1;
   }
   return 0;
}

static int addPostHdr( XtraPostHdr **hdrQp, char *xhdrname, char *value, eHdrOp replaceDups)
{
   XtraPostHdr *xh;
   XtraPostHdr **p = hdrQp;
   xh= (XtraPostHdr *)malloc( sizeof(struct XtraPostHdr));
   memset(xh, 0, sizeof(struct XtraPostHdr));
   if (xh) {
      xh->hdr = strdup(xhdrname);
      xh->value = strdup(value);
      if (replaceDups == REPLACE) {
         while (*p) {
            XtraPostHdr *xp = *p;
            if ( strcmp(xp->hdr, xh->hdr)==0) {
               /* replace header */
               xh->next = xp->next;
               free(xp->hdr);
			   xp->hdr=NULL;
               free(xp->value);
			   xp->value=NULL;
               free(xp);
			   xp=NULL;
               *p = xh;
               return 1;
            }
            p = &xp->next;
         }
      }
      /* just stick it at beginning of list */
      xh->next = *hdrQp;
      *hdrQp = xh;
      return 1;
   }
   return 0;
}
/*----------------------------------------------------------------------*/
static void report_status(tWgetInternal *data, tWgetStatus status, 
                          const char *msg)
{
   tWget wg;
   /* internal error, call callback */
   wg.status = status;
   wg.pc = data->pc;
   wg.hdrs = data->hdrs;
   wg.msg = msg;
   wg.handle = data->handle;
   data->cbActive = 1;
   (*data->cb)(&wg);
   data->cbActive = 0;
   if (data->keepConnection==eCloseConnection)
        freeData(data);
}

/*----------------------------------------------------------------------*
 * returns
 *   0 if ok
 *  -1 if WAN interface is not active
 */
static int send_get_request(tWgetInternal *p, const char *host, int port, const char *uri)
{
	printf("send_get_request  0\n");
   tProtoCtx   *pc = p->pc;
   XtraPostHdr *next;

/*修改历史 : 20150817 duanguiyuan 注释掉

   if (!getWanInfo(&wanState)) {
   	printf("send_get_request  return -1\n");

      return -1;
   }
*/
printf("send_get_request  1\n");
   proto_SendRequest(pc, "GET", uri);
   proto_SendHeader(pc,  "Host", host);
   proto_SendHeader(pc,  "User-Agent", USER_AGENT_NAME);
   if (p->keepConnection==eCloseConnection)
      proto_SendHeader(pc,  "Connection", "close");
   else
      proto_SendHeader(pc,  "Connection", "keep-alive");
printf("send_get_request  2\n");
   next = p->xtraPostHdrs;
   while (next) {
      proto_SendHeader(pc,next->hdr, next->value);
      next = next->next;
   }
   proto_SendEndHeaders(pc);
printf("send_get_request  3\n");
   return 0;
}

static int send_request(tWgetInternal *p, const char *host, int port, const char *data,
                        int datalen, const char *content_type, int content_len)
{
   tProtoCtx   *pc = p->pc;
   char        buf[BUF_SIZE];
   XtraPostHdr *next;
   CookieHdr   *cookie;

   sprintf(buf, "%s:%d", host, port);
   proto_SendHeader(pc,  "Host", buf);
   proto_SendHeader(pc,  "User-Agent", USER_AGENT_NAME);
   if (p->keepConnection==eCloseConnection)
      proto_SendHeader(pc,  "Connection", "close");
   else
      proto_SendHeader(pc,  "Connection", "keep-alive");
   next = p->xtraPostHdrs;
   while (next) {
      proto_SendHeader(pc,next->hdr, next->value);
      next = next->next;
   }
   cookie = p->cookieHdrs;
   while(cookie) {
      proto_SendCookie(pc, cookie);
      cookie = cookie->next;
   }
   proto_SendHeader(pc,  "Content-Type", content_type);

   sprintf(buf, "%d", content_len);
   proto_SendHeader(pc,  "Content-Length", buf);

   proto_SendEndHeaders(pc);
   printf("send_request ok !\n");
   if (data && datalen)
      proto_SendRaw(pc, data, datalen);

 printf("send_request ok !\n");
   return 0;
}
/* add by duanguiyuan 20150925 
upload 上传日志文件

*/

static int send_request_put(tWgetInternal *p, const char *host, int port, const char *data,
                        int datalen, const char *content_type, int content_len,const char *url)
{
   tProtoCtx   *pc = p->pc;
   char        buf[BUF_SIZE];
   XtraPostHdr *next;
   CookieHdr   *cookie;
	char url_buf[256];
	
   sprintf(buf, "%s:%d", host, port);
   proto_SendHeader(pc,  "Host", buf);
   
   /* add bu duanguiyuan 20150812 
			Accept-Encoding: gzip, deflate
			Accept-Language: zh-CN,zh;q=0.8
   */
   #if 0 
  // HTTP_PUT_BOUNDARY_KEY
   sprintf(buf, "multipart/form-data; boundary=%s", HTTP_PUT_BOUNDARY_KEY);

   proto_SendHeader(pc,  "Content-Type", buf);
#endif
	
   //proto_SendHeader(pc,  "Accept-Encoding", "gzip, deflate");

   //proto_SendHeader(pc,  "Accept-Language", "zh-CN,zh;q=0.8");

#if 0
   if (p->keepConnection==eCloseConnection)
      proto_SendHeader(pc,  "Connection", "close");
   else
      proto_SendHeader(pc,  "Connection", "keep-alive");
#endif
   next = p->xtraPostHdrs;
   while (next) {
      proto_SendHeader(pc,next->hdr, next->value);
      next = next->next;
   }
   cookie = p->cookieHdrs;
   while(cookie) {
      proto_SendCookie(pc, cookie);
      cookie = cookie->next;
   }
   proto_SendHeader(pc,  "User-Agent", USER_AGENT_NAME);
   proto_SendHeader(pc,  "Accept", "*/*"); //Accept: */*
  
   	memset(url_buf, 0, sizeof(url_buf));
	//sprintf(url_buf, "%s",url);

	int lenn=strlen(url);
	do
	{
		if(url[lenn - 1]=='/')
		{
			sprintf(url_buf, "%s",url+lenn);
			break;
		}
		lenn--;
	}while(lenn > 1);
	if(lenn == 1)
	{
		sprintf(url_buf, "%s",url);
	}
	if(strlen(url_buf) <1 )
	{
		sprintf(url_buf, "log.txt" );
	}
	printf("url:%s\n  lenn:%d \nurl_buf:%s\n ",url,lenn,url_buf);
	/* 上传文件 第一次 探测 认证  获取认证 信息 */
	if(content_len <= 2)
	{
	   	proto_SendHeader(pc,  "Content-Length", "0");
   		proto_SendEndHeaders(pc);
		return 0;
	}
	//Expect: 100-continue
   proto_SendHeader(pc,  "Expect", "100-continue");
   proto_SendHeader(pc,  "Content-Type", "multipart/form-data; boundary="HTTP_PUT_BOUNDARY_KEY"");
 
   memset(buf, 0, sizeof(buf));  /* memset */
   sprintf(buf, "--%s\r\nContent-Disposition: form-data; name=\"data\"; filename=\"%s\"\r\nContent-Type: "\
   	CONTET_TYPE_OTHER"\r\n\r\n\r\n--%s--\r\n", HTTP_PUT_BOUNDARY_KEY,url_buf,HTTP_PUT_BOUNDARY_KEY);
	
  // printf("buf = %d  !\n%s\n",strlen(buf),buf);
   sprintf(buf, "%d", content_len+strlen(buf));
   proto_SendHeader(pc,  "Content-Length", buf);
   
  proto_SendEndHeaders(pc);

   
   printf("send_request ok !\n");
   if (data && datalen)
      proto_SendRaw(pc, data, datalen);

 memset(buf, 0, sizeof(buf));  /* memset */
 sprintf(buf, "--%s\r\nContent-Disposition: form-data; name=\"data\"; filename=\"%s\"\r\nContent-Type: "\
 	CONTET_TYPE_OTHER"\r\n\r\n", HTTP_PUT_BOUNDARY_KEY,url_buf);
 proto_SendRaw(pc,buf,strlen(buf));
 
 printf("send_request ok !\n");
   return 0;
}
/*----------------------------------------------------------------------*
 * returns
 *   0 if ok
 *  -1 if WAN interface is not active
 *  arg_keys is a NULL terminated array of (char *)
 *  arg_values is a NULL terminated array of (char *) of same length as arg_keys
 */
 
 /* http 发送 post请求 add by zhanghaozhong */
static int send_post_request(tWgetInternal *p, const char *host, int port, const char *uri,
                              const char *data, int datalen, const char *content_type,
                              int content_len)
{
   tProtoCtx   *pc = p->pc;
/*
   if (!getWanInfo(&wanState)) {
      return -1;
   }
*/
   proto_SendRequest(pc, "POST", uri);
   
   return send_request(p, host, port, data, datalen, content_type, content_len);
}

/*----------------------------------------------------------------------*
 * returns
 *   0 if ok
 *  -1 if WAN interface is not active
 *  arg_keys is a NULL terminated array of (char *)
 *  arg_values is a NULL terminated array of (char *) of same length as arg_keys
 */
//#ifdef RPC_UPLOAD    
static int send_put_request(tWgetInternal *p, const char *host, int port, const char *uri,
                              const char *data, int datalen, const char *content_type,
                              int content_len)
{
   tProtoCtx   *pc = p->pc;
/*注释 by duanguiyuan 20150901   error
   if (getWanState()== eWAN_INACTIVE) {
      return -1;
   }
*/	
  
   proto_SendRequest(pc, "POST", uri);

   
   return send_request_put(p, host, port, data, datalen, content_type, content_len,uri);
}
//#endif   // RPC_UPLOAD

/*----------------------------------------------------------------------
 * connect timeout
 */
static void timer_connect(void *p)
{
   tWgetInternal *data = (tWgetInternal *) p;
   char          buf[256];

   stopListener(data->pc->fd);

   sprintf(buf, "Connection timed out to host %s:%d", data->host, data->port);
   report_status(data, iWgetStatus_ConnectionError, buf);
}

/*----------------------------------------------------------------------*/
static void timer_response(void *p)
{
   tWgetInternal *data = (tWgetInternal *) p;
   char          buf[512];
   stopListener(data->pc->fd);
   sprintf(buf, "Host (%s:%d) is not responding, timeout", data->host, data->port);
   report_status(data, iWgetStatus_ConnectionError, buf);
}

/*----------------------------------------------------------------------*/
/* http连接 by zhanghaozhong */
static void do_connect(void *p)
{
   tWgetInternal *data = (tWgetInternal *) p;
   int           err;
   u_int         n;
slog(TR69C_LOG_DEBUG,"do connect   0\n");
   stopTimer(timer_connect, data);
   stopListener(data->pc->fd);

   /* check fd status */
   n = sizeof(int);
 //  int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
   if (getsockopt(data->pc->fd, SOL_SOCKET, SO_ERROR, &err, &n) < 0) {
      report_status(data, iWgetStatus_InternalError,
                    "internal error: do_connect(): getsockopt failed");

      return;
   }

   if (err != 0) {
      /* connection not established */
      char buf[256];
slog(TR69C_LOG_DEBUG,"do connect   1\n");
      snprintf(buf,sizeof(buf), "Connection to host %s(%s):%d failed %d (%s)", 
               data->host, writeIp(data->host_addr), data->port, 
               err, strerror(err));
      report_status(data, iWgetStatus_ConnectionError, buf);
      return;
   }
slog(TR69C_LOG_DEBUG,"do connect  2\n");
printf("[%s:%d] data->proto=%s \n",__FUNCTION__,__LINE__,data->proto);	
#ifdef USE_SSL
   /* init ssl if proto is https */
   if ((strcmp(data->proto, "https") == 0) && (data->pc->sslConn <= 0)) {
      proto_SetSslCtx(data->pc, do_connect_ssl, data);
      return;
   }
#endif
slog(TR69C_LOG_DEBUG,"do connect   3\n");
   /* return at this point if function is connect only */
   if (data->request==eConnect)   {

	slog(TR69C_LOG_DEBUG,"do connect   4\n");
      report_status(data, iWgetStatus_Ok, NULL);
      return;
   }
}

/*----------------------------------------------------------------------*/
static void do_connect_ssl(void *p, int errorcode)
{
   tWgetInternal *data = (tWgetInternal *) p;
   printf("[%s:%d] \n",__FUNCTION__,__LINE__);	

   if (errorcode < 0)
      report_status(data, iWgetStatus_ConnectionError, 
                      "Failed to establish SSL connection");
   else
      do_connect(p);
    printf("[%s:%d] \n",__FUNCTION__,__LINE__);	
}

/*----------------------------------------------------------------------*/
static void do_send_request(void *p, int errorcode)
{
   tWgetInternal *data = (tWgetInternal *) p;
   HttpTask       *ht   = (HttpTask *)(data->handle);
   int res;
printf("de send request  0\n");
if (g_debug)
   fprintf(stderr, "do_send_request keepConn=%d status=%d\n", data->keepConnection, data->status);

   if (errorcode < 0) {
      report_status(data, iWgetStatus_ConnectionError, 
                    "Failed to establish SSL connection");
      return;
   }
printf("de send request  1\n");
printf("de send request  data->request = %d\n",data->request);
   /* send request */
   switch (data->request) {
      case eGetData:
          res = send_get_request(p, data->host, data->port, data->uri);
          break;
      case ePostData:
          res = send_post_request(p, data->host, data->port, data->uri, data->postdata, 
                                  data->datalen, data->content_type, ht->content_len);
          break;
//#ifdef RPC_UPLOAD    
      case ePutData:

		  res = send_put_request(p, data->host, data->port, data->uri, data->postdata, 
                                 data->datalen, data->content_type, ht->content_len);
          break;
//#endif   // RPC_UPLOAD          
      default:
          res = -1;
          break;
   }
printf("de send request  2\n");
   if (res < 0) {
      report_status(data, iWgetStatus_ConnectionError, 
                    "Failed to send request on connection");
      return;
   }

   /* wait for response */
   setListener(data->pc->fd, do_response, data);
   setTimer(timer_response, data, ACSRESPONSETIME); /* response timeout is 60 sec */
printf("de send request  2\n");
}

/*----------------------------------------------------------------------*/
static void do_response(void *p)
{
   CookieHdr      *cp;
   tWgetInternal  *data = (tWgetInternal *) p;
   if (g_debug)
   slog(TR69C_LOG_DEBUG,"do_response data->pc=%p",data->pc);

   stopTimer(timer_response, data);

   if (data->pc == NULL) {
      slog(LOG_ERROR, "wget %s", "Internal Error");
      report_status(data, iWgetStatus_InternalError, 
                    "internal error: no filedescriptor");
      return;
   }
slog(TR69C_LOG_DEBUG,"do_response 1");

   if (data->pc->fd <= 0) {
      report_status(data, iWgetStatus_InternalError, 
                    "internal error: no filedescriptor");
      return;
   }
slog(TR69C_LOG_DEBUG,"do_response 2");

   if (data->hdrs)
      proto_FreeHttpHdrs(data->hdrs);
   data->hdrs = proto_NewHttpHdrs();

slog(TR69C_LOG_DEBUG,"do_response 3");

   if (data->hdrs == NULL) {
      /* memory exhausted?!? */
      slog(LOG_ERROR, "wget %s", "Memory exhausted");
   }
slog(TR69C_LOG_DEBUG,"do_response 4");

   if (proto_ParseResponse(data->pc, data->hdrs) < 0) {
      report_status(data, iWgetStatus_Error, 
                    "error: illegal http response or read failure");
      return;
   }
slog(TR69C_LOG_DEBUG,"do_response 5");

   proto_ParseHdrs(data->pc, data->hdrs);
   cp = data->hdrs->setCookies;
   while (cp) { /* save new cookies if present*/
      addCookieHdr( &data->cookieHdrs,cp->name, cp->value, REPLACE );
      cp = cp->next;
   }
slog(TR69C_LOG_DEBUG,"do_response 6");

   if (data->hdrs->status_code >= 100 && data->hdrs->status_code < 600 ) {
      report_status(data, iWgetStatus_Ok, NULL);
	   slog(TR69C_LOG_DEBUG,"do_response ok");
  
   } else {
      char buf[1024];
      sprintf(buf, "Host %s returned error \"%s\"(%d)", 
              data->host, data->hdrs->message, data->hdrs->status_code);
      report_status(data, iWgetStatus_HttpError, buf);
   }
slog(TR69C_LOG_DEBUG,"do_response end");

}

/*----------------------------------------------------------------------*/
static void do_resolve(void *handle)
{
   tWgetInternal  *data = handle;
   int            res, fd;

   stopTimer(timer_connect, data);
   /* if callback does not exist, this is a non-op */
   stopCallback(&(data->host_addr), do_resolve, data);

   if (data->host_addr == 0) {
      char buf[1024];
      sprintf(buf, noHostResolve, data->host);
      report_status(data, iWgetStatus_ConnectionError, buf);
   } else if ((res = www_EstablishConnection(data->host_addr, data->port, &fd)) < 0) {
      if (res == -1) {
         report_status(data, iWgetStatus_InternalError, "Socket creation error");
     } else {
         char buf[1024];
         sprintf(buf, noHostConnectMsg, 
                 data->host, writeIp(data->host_addr), data->port);
         report_status(data, iWgetStatus_ConnectionError, buf);
     }
   } else {
      data->pc = proto_NewCtx(fd);
      setListenerType(fd, do_connect, data, iListener_Write);    
   }
}

/*----------------------------------------------------------------------*/
#if 0 //not used
int do_wget(const char *url, EventHandler callback, void *handle, char *postdata, int datalen, char *content_type)
{
    char proto[10];
    char host[1024];
    char uri[1024];
    int port;

if (g_debug)
    slog(TR69C_LOG_DEBUG, "do_wget(\"%s\", ...)", url);

    if (www_ParseUrl(url, proto, host, &port, uri) < 0) {
        return -5;
    }

    if (port == 0) {
        if (strcmp(proto, "http") == 0) {
            port = 80;
        } else if (strcmp(proto, "https") == 0) {
            port = 443;
        } else {
            slog(LOG_ERROR, "unsupported protocol in url \"%s\"", proto);
            port = 80; /* guess http and port 80 */
        }
    }

    {
        /* connect() started, lets wait until we have a connection */
        tWgetInternal *data;
        data = (tWgetInternal *) malloc(sizeof(tWgetInternal));
        memset(data, 0, sizeof(tWgetInternal));
        data->keepConnection = eCloseConnection;
        data->pc = NULL;
        data->cb = callback;
        data->handle = handle;
        data->proto = strdup(proto);
        data->host = strdup(host);
        data->host_addr = 0;
        data->port = port;
        data->uri = strdup(uri);
        data->content_type = content_type;
        data->postdata = postdata;
        data->request = postdata? ePostData: eGetData;
        data->datalen = datalen;

        if (dns_lookup(data->host, &(data->host_addr))) {
            do_resolve(data);
        } else {
            setCallback(&(data->host_addr), do_resolve, data);
        }
    }

    return 0;
}
#endif

/*----------------------------------------------------------------------*
 * return
 *   0 Ok
 *  -1 Could not create socket
 *  -2 Could not connect to host
 *  -3 Could not resolve host or host syntax error
 *  -4 WAN interface is not active
 *  -5 URL syntax error
 *  -10 unknown error
 */
#if 0 //not used
int wget_Get(const char *url, EventHandler callback, void *handle)
{
    return do_wget(url, callback, handle, NULL, 0, NULL);
}
#endif

/*----------------------------------------------------------------------*
 * return
 *   0  Ok
 *  -1  Could not create socket
 *  -2  Could not connect to host
 *  -3  Could not resolve host or host syntax error
 *  -4  WAN interface is not active
 *  -5  URL syntax error
 *  -10 unknown error
 */
#if 0 //not used
int wget_Post(const char *url, char *arg_keys[], char *arg_values[], EventHandler callback, void *handle)
{
    char *argbuf;
    int arglen;
    int i;
    char buf[BUF_SIZE];

    argbuf = (char *) malloc(BUF_SIZE * 4);
    if (argbuf == NULL) {
        return -10;
    }
    memset(argbuf, 0, BUF_SIZE * 4);

    /* build arguments */

#if 0
    fprintf(stderr, "DEBUG: wget_Post()\n");
    cvprint(arg_keys);
    cvprint(arg_values);
#endif
    strcpy(argbuf, "");
    for (i = 0; arg_keys[i] != NULL; i++) {
        if (i > 0) {
            strcat(argbuf, "&");
        }
        www_UrlEncode(arg_values[i], buf);
        sprintf(&argbuf[strlen(argbuf)], "%s=%s", arg_keys[i], buf);
    }
    arglen = strlen(argbuf);

    return do_wget(url, callback, handle, argbuf, arglen, "application/x-www-form-urlencoded");
}
#endif

/*----------------------------------------------------------------------*
 * return
 *   0  Ok
 *  -1  Could not create socket
 *  -2  Could not connect to host
 *  -3  Could not resolve host or host syntax error
 *  -4  WAN interface is not active
 *  -5  URL syntax error
 *  -10 unknown error
 */
#if 0 //not used
int wget_PostRaw(const char *url, char *content_type, char *data, int len, EventHandler callback, void *handle)
{
    return do_wget(url, callback, handle, data, len, content_type);
}
#endif

/*----------------------------------------------------------------------*
 * wget_GetFile
 *----------------------------------------------------------------------*/
typedef struct {
   EventHandler   cb;
   void           *handle;
   char           *filename;
} tWgetFile;

/*----------------------------------------------------------------------*
 * returns
 *    0 Ok
 *   -1 File could not be open for writing
 *   -2 Error when reading from fd
 *   -3 Error when writing to file
 */
#if 0 //not used
static int copy_to_file(tProtoCtx *pc, const char *filename)
{
    int ofd;
    char buf[BUF_SIZE];
    int rlen, wlen;
    mode_t mode;
    int flags, bflags;

    /* 664 */
    mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
    if ((ofd = creat(filename, mode)) < 0) {
        return -1;
    }

    /* TBD: Temporary fix part 1, turn on synchroneous I/O, this call will block. */
    {
        flags = (long) fcntl(pc->fd, F_GETFL);
        bflags = flags & ~O_NONBLOCK; /* clear non-block flag, i.e. block */
        fcntl(pc->fd, F_SETFL, bflags);
    }

    while ((rlen = proto_Readn(pc, buf, BUF_SIZE)) != 0) {
        if (rlen < 0) {
            if (errno == EAGAIN) {
if (g_debug)
                fprintf(stderr, "wget, data not ready, let's busy wait\n");

                continue;
            }
            /* error when reading from fd */
            return -2;
        } else if (rlen > 0) {
            /* Bytes read */
            wlen = write(ofd, buf, rlen);
            if (wlen != rlen) {
                /* error on writing */
                return -3;
            }
        }
    }

    /* TBD: Temp fix part 2. remove blocking flags */
    {
        fcntl(pc->fd, F_SETFL, flags);
    }
    /* EOF */
    close(ofd);
    return 0;
}
#endif

/*----------------------------------------------------------------------
 * argument of callback is of type (tWget *)
 */
#if 0 //not used
static void save_to_file(void *p)
{
    tWgetFile *wf;
    tWget *w;
    tWget cbw;
    int res;

if (g_debug)
    fprintf(stderr, "DEBUG: wget.save_to_file()...\n");

    w = (tWget *) p;
    wf = (tWgetFile *) w->handle;

    memset(&cbw, 0, sizeof(tWget));

    if (w->status != iWgetStatus_Ok) {
if (g_debug)
        fprintf(stderr, "DEBUG: wget.save_to_file, status NOT ok (%d)\n", w->status);

        cbw.status = iWgetStatus_InternalError;
    } else if ((res = copy_to_file(w->pc, wf->filename)) < 0) {
if (g_debug)
        fprintf(stderr, "DEBUG: wget.save_to_file, copy_to_file failed (%d), status NOT ok (%d)\n",
                res, w->status);

        cbw.status = iWgetStatus_InternalError;
    } else {
if (g_debug)
        fprintf(stderr, "DEBUG: wget.save_to_file, status OK (%d)\n", w->status);

        cbw.status = iWgetStatus_Ok;
    }

    /* call callback */
    cbw.msg = w->msg;
    cbw.pc = w->pc;
    cbw.hdrs = w->hdrs;
    cbw.handle = wf->handle;
if (g_debug)
    fprintf(stderr, "DEBUG: wget.save_to_file() calling callback = %p...\n", (void *) wf->cb);

    (*wf->cb)(&cbw);

    free(wf->filename);
    free(wf);
if (g_debug)
    fprintf(stderr, "DEBUG: wget.save_to_file() done.\n");

}
#endif

/*----------------------------------------------------------------------*
 * returns
 *    0  Ok
 *   -1 to -5 same error codes as wget_Get
 * set status in callback to
 *    0 Ok
 *   -1 Could not open file for writing
 */
#if 0 //not used
int wget_GetFile(const char *url, const char *filename, EventHandler callback, void *handle)
{
    int res;
    tWgetFile *wf;

    wf = (tWgetFile *) malloc(sizeof(tWgetFile));
    memset(wf, 0, sizeof(tWgetFile));
    wf->cb = callback;
    wf->handle = handle;
    wf->filename = strdup(filename);

    res = wget_Get(url, save_to_file, wf);
    if (res < 0) {
        free(wf->filename);
        free(wf);
        return res;
    }
    return 0;
}
#endif

/*----------------------------------------------------------------------*
 * returns
 *    0  Ok
 *   -1 to -5 same error codes as wget_Get
 * set status in callback to
 *    0 Ok
 *   -1 Could not open file for writing
 */
#if 0 //not used
int wget_PostXmlFile(const char *url, char *data, int len, const char *filename, EventHandler callback, void *handle)
{
    int res;
    tWgetFile *wf;

    wf = (tWgetFile *) malloc(sizeof(tWgetFile));
    memset(wf, 0, sizeof(tWgetFile));
    wf->cb = callback;
    wf->handle = handle;
    wf->filename = strdup(filename);

    res = wget_PostRaw(url, "text/xml", data, len, save_to_file, wf);
    if (res < 0) {
        free(wf->filename);
        free(wf);
        return res;
    }
    return 0;
}
#endif
/*
* Connect to the specified url
* Returns: NULL  failed allocate memory or immediate connection error.
 *               Call wget_LastErrorMsg() to retrieve last error msg. 
 *         pointer to Connection descriptor tWgetInternal. 
*/
tWgetInternal *wget_Connect(const char *url, EventHandler callback, void *handle)
{
   tWgetInternal *wg;
   char proto[10];
   char host[1024];
   char uri[1024];
   int port;

   slog(TR69C_LOG_DEBUG, "wget_Connect(\"%s\", ...)", url);

   if ( (wg = (tWgetInternal*) malloc(sizeof(tWgetInternal)))==NULL)
      return NULL;
   memset(wg, 0, sizeof(tWgetInternal));
   lastErrorMsg[0] = '\0';
   wg->request = eConnect;
   wg->keepConnection = eKeepConnectionOpen;
   if (www_ParseUrl(url, proto, host, &port, uri) < 0) {
      wg->status = -5;
      return wg;
   }
 slog(TR69C_LOG_DEBUG, "www_ParseUrl:\n proto=%s\n host=%s\n port=%d\n uri=%s\n",proto,host,port,uri );
 
   if (port == 0) {
      if (strcmp(proto, "http") == 0) {
         port = 80;
      } else if (strcmp(proto, "https") == 0) {
         port = 443;
      } else {
         slog(LOG_ERROR, "unsupported protocol in url \"%s\"", proto);
         port = 80; /* guess http and port 80 */
      }
   }

   wg->pc = NULL;
   wg->cb = callback;
   wg->handle = handle;
   wg->proto = strdup(proto);
   wg->host = strdup(host);
   wg->host_addr = 0;
   wg->port = port;
   if (strlen(uri))
      wg->uri = strdup(uri);
   else
      wg->uri = strdup("");

#ifdef USE_SSL
	/* 初始化 https的 openssl库中的 函数*/
	if (strcmp(proto, "https") == 0) {
		proto_Init();
	}

#endif
   if (dns_lookup(wg->host, &(wg->host_addr))) {
      /* immediate return requires special handling. */
      int res; int fd;
      uiAcsIP = wg->host_addr;  /* 供WEB使用 */
      if (wg->host_addr == 0) {
         snprintf(lastErrorMsg,sizeof(lastErrorMsg), noHostResolve, wg->host);
         freeData(wg);
         wg=NULL;
      } else if ((res = www_EstablishConnection(wg->host_addr, wg->port, &fd)) < 0) {
         if (res == -1)
            strncpy(lastErrorMsg, "Socket creation error", sizeof(lastErrorMsg));
         else 
            snprintf(lastErrorMsg,sizeof(lastErrorMsg), noHostConnectMsg, 
                  wg->host, writeIp(wg->host_addr), wg->port);
         freeData(wg);
         wg=NULL;
      } else { /* connection complete- start it */      /* HTTP链接已经建立 */
slog(TR69C_LOG_DEBUG, "HTTP connection complete- start it! ");
         wg->pc = proto_NewCtx(fd);
#ifdef USE_SSL
			/* add by duanguiyuan 20161110*/
			if (strcmp(proto, "https") == 0) {
				wg->pc->type = iSsl;
			}
#endif
         setListenerType(fd, do_connect, wg, iListener_Write);
      }
   } else {
      setTimer(timer_connect, wg, ACSRESPONSETIME); /* response timeout is 60 sec */
      setCallback(&(wg->host_addr), do_resolve, wg);
   }

   return wg;
}

int wget_GetData(tWgetInternal *wg,EventHandler callback, void *handle)
{
printf("wget_GetData 0\n");
   wg->request = eGetData;
   wg->handle = handle;
   wg->cb = callback;
   if (wg->hdrs) {
      wg->hdrs->status_code = 0; /* reset status_code */
      wg->hdrs->content_length = 0;
   }

   do_send_request(wg, PROTO_OK);
   printf("wget_GetData 1\n");
   return 0;
}

int wget_PostData(tWgetInternal *wg,char *postdata, int datalen, char *content_type,
                  EventHandler callback, void *handle)
{
   wg->request = ePostData;
   wg->content_type = content_type;
   wg->postdata = postdata;
   wg->datalen = datalen;
   wg->handle = handle;
   wg->cb = callback;
   if (wg->hdrs) {
      wg->hdrs->status_code = 0; /* reset status_code */
      wg->hdrs->content_length = 0;
   }

   do_send_request(wg, PROTO_OK);
   return 0;
}

int wget_PostDataClose(tWgetInternal *wg,char *postdata, int datalen, char *content_type,
                       EventHandler callback, void *handle)
{
   wg->request = ePostData;
   wg->content_type = content_type;
   wg->postdata = postdata;
   wg->datalen = datalen;
   wg->handle = handle;
   wg->cb = callback;
   if (wg->hdrs) {
      wg->hdrs->status_code = 0; /* reset status_code */
      wg->hdrs->content_length = 0;
   }
   wg->keepConnection = eCloseConnection;


   do_send_request(wg, PROTO_OK);
   return 0;
}

//#ifdef RPC_UPLOAD    
int wget_PutData(tWgetInternal *wg,char *putdata, int datalen, char *content_type,
                  EventHandler callback, void *handle)
{
   wg->request = ePutData;
   wg->content_type = content_type;
   wg->postdata = putdata;
   wg->datalen = datalen;
   wg->handle = handle;
   wg->cb = callback;
   if (wg->hdrs) {
      wg->hdrs->status_code = 0; /* reset status_code */
      wg->hdrs->content_length = 0;
   }

   do_send_request(wg, PROTO_OK);
   
   return 0;
}
//#endif   // RPC_UPLOAD

/*
* Disconnect maybe called from within a callback called
 * by report_status. Don't freeData() if cbActive is set.
 * Setting cCloseConnection will cause report_status
 * to free up the data on return by the callback.
* 
*/

int wget_Disconnect(tWgetInternal *wio)
{
   if (wio!=NULL) { 
      stopTimer(timer_response, wio); /* may be running */
      wio->request = eDisconnect;
      wio->keepConnection = eCloseConnection;
      if (!wio->cbActive)
      	{
      		freeData(wio);
			wio=NULL;
	  	}
         
   }
   return 0;
}

int wget_AddPostHdr( tWgetInternal *wio, char *xhdrname, char *value)
{
   XtraPostHdr   **p = &wio->xtraPostHdrs;
   return addPostHdr(p, xhdrname,value, REPLACE );
}

void wget_ClearPostHdrs( tWgetInternal *wio)
{
   XtraPostHdr *xh = wio->xtraPostHdrs;

   while (xh) {
      XtraPostHdr *nxt;
      free( xh->hdr);
	  xh->hdr=NULL;
      free(xh->value);
	  xh->value=NULL;
      nxt = xh->next;
      free(xh);
	  xh=NULL;
      xh= nxt;
   }
   wio->xtraPostHdrs = NULL;
}

const char *wget_LastErrorMsg(void)
{
   return lastErrorMsg;
}

