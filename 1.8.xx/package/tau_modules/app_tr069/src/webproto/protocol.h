/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : protocol.h
  版 本 号   : 初稿
  作    者   : zhanghaozhong
  生成日期   : 2013年5月16日
  最近修改   :
  功能描述   : protocol
  函数列表   :
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


#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

/* BEGIN: Added by zhanghaozhong, 2013/1/21   PN:transplant tr-069 by zhz */
#include "types.h"
/* END:   Added by zhanghaozhong, 2013/1/21   PN:transplant tr-069 by zhz */

#ifdef USE_SSL
#include <openssl/ssl.h>
#endif
/*----------------------------------------------------------------------*
 * typedefs
 */

typedef enum {
  iZone_Unknown,
  iZone_Lan,
  iZone_Ihz
} tZone;



typedef struct CookieHdr {
	struct CookieHdr *next;
	char	*name;
	char	*value;
} CookieHdr;

/*--------------------*/
typedef struct {
  /* common */
  char *content_type;
  char *protocol;
  char *wwwAuthenticate;
  char *Authorization;
  char *TransferEncoding;
  char *Connection;
  /* request */
  char *method;
  char *path;
  char *host;
  int  port;
  int  content_length;

  /* result */
  int  status_code;
  CookieHdr	*setCookies;
  char *message;
  char *locationHdr;		/* from 3xx status response */

  /* request derived */
  tIpAddr addr;  /* IP-address of communicating entity */
  tZone zone;    /* zone in which communicating entity is */
  char *filename;
  char *arg;
} tHttpHdrs;

typedef void (*tProtoHandler)(void *, int lth);

typedef enum {
    sslRead,
    sslWrite
} tSSLIO;
/*--------------------*/
typedef enum {
  iUnknown,
  iNormal,
  /*altered by caibing xxx*/
//#ifdef USE_SSL
  iSsl,
//#endif
  i__Last
} tPostCtxType;

/*--------------------*/
typedef struct {
   tPostCtxType   type;
   int            fd;      /* filedescriptor */

   /* internal use */
#ifdef USE_SSL
   SSL            *ssl;
   int            sslConn;
#endif
   tProtoHandler  cb;
   void           *data;
} tProtoCtx;


/* convenient naming */
#define fdgets   proto_Readline
#define fdprintf proto_Printline

#define PROTO_OK                0
#define PROTO_ERROR            -1
#define PROTO_ERROR_SSL        -2

/*----------------------------------------------------------------------*/
extern void proto_Init(void);

extern tHttpHdrs *proto_NewHttpHdrs(void);
extern void proto_FreeHttpHdrs(tHttpHdrs *p);

extern tProtoCtx *proto_NewCtx(int fd);
#ifdef USE_SSL
extern void proto_SetSslCtx(tProtoCtx *pc, tProtoHandler cb, void *data);
#endif
extern void proto_FreeCtx(tProtoCtx *pc);

extern int  proto_ReadWait(tProtoCtx *pc, char *ptr, int nbytes);
extern int  proto_Readn(tProtoCtx *pc, char *ptr, int nbytes);
extern int  proto_Writen(tProtoCtx *pc, const char *ptr, int nbytes);
extern int  proto_Readline(tProtoCtx *pc, char *ptr, int maxlen);
extern void proto_Printline(tProtoCtx *pc, const char *fmt, ...);
extern int	proto_Skip(tProtoCtx *pc);
extern int  proto_SSL_IO(tSSLIO iofunc, tProtoCtx *pc, char *ptr, int nbytes, tProtoHandler cb, void *data);
extern void proto_SendRequest(tProtoCtx *pc, const char *method, const char *url);
void proto_SendCookie(tProtoCtx *pc, CookieHdr *c);
extern void proto_SendHeader(tProtoCtx *pc,  const char *header, const char *value);
extern void proto_SendEndHeaders(tProtoCtx *pc);
extern void proto_SendRaw(tProtoCtx *pc, const char *arg, int len);
extern void proto_SendHeaders(tProtoCtx *pc, int status, const char* title, const char* extra_header, const char* content_type);

extern void proto_SendRedirect(tProtoCtx *pc, const char *host, const char* location);
extern void proto_SendRedirectProtoHost(tProtoCtx *pc, const char *protohost, const char* location);
extern void proto_SendRedirectViaRefresh(tProtoCtx *pc, const char *host, const char* location);
extern void proto_SendError(tProtoCtx *pc, int status, const char* title, const char* extra_header, const char* text);

extern int  proto_ParseResponse(tProtoCtx *pc, tHttpHdrs *hdrs);
extern int  proto_ParseRequest(tProtoCtx *pc, tHttpHdrs *hdrs);
extern void proto_ParseHdrs(tProtoCtx *pc, tHttpHdrs *hdrs);
extern void proto_ParsePost(tProtoCtx *pc, tHttpHdrs *hdrs);

#endif
