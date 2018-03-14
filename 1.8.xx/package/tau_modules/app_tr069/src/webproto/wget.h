/******************************************************************************

                  ��Ȩ���� (C), 2010-2012, �Ϻ�徴�ͨ�ſƼ����޹�˾

 ******************************************************************************
  �� �� ��   : wget.h
  �� �� ��   : ����
  ��    ��   : zhanghaozhong
  ��������   : 2013��5��16��
  ����޸�   :
  ��������   : wget
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��5��16��
    ��    ��   : zhanghaozhong
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/


#ifndef _WGET_H_
#define _WGET_H_

#include "../webproto/protocol.h"
#include "event.h"

/*----------------------------------------------------------------------*/
typedef enum {
   iWgetStatus_Ok = 0,
   iWgetStatus_InternalError,
   iWgetStatus_ConnectionError,
   iWgetStatus_Error,
   iWgetStatus_HttpError
} tWgetStatus;

typedef struct {
   tWgetStatus status;
   tProtoCtx   *pc;
   tHttpHdrs   *hdrs;
   const char  *msg;  /* Msg associated with status */
   void        *handle;
} tWget;

typedef enum {
   eCloseConnection=0,
   eKeepConnectionOpen  /* used by wConnect and wClose */
} tConnState;

typedef enum {
   eUndefined,
   eConnect,
   ePostData,
   eGetData,
//#ifdef RPC_UPLOAD    
   ePutData,
//#endif   
   eDisconnect
} tRequest;

typedef struct XtraPostHdr{
   struct XtraPostHdr   *next;
   char                 *hdr;   /* header string */
   char                 *value; /* value string*/
} XtraPostHdr;

typedef struct {
   tConnState  keepConnection;
   int         status;
   tRequest    request;
   int         cbActive; /* set to 1 if callback from report status */
   tProtoCtx   *pc;
   EventHandler cb;
   void        *handle;
   char        *proto;
   char        *host;
   tIpAddr     host_addr;
   int         port;
   char        *uri;
   tHttpHdrs   *hdrs;
   CookieHdr	*cookieHdrs;
   XtraPostHdr *xtraPostHdrs;
   char        *content_type;
   char        *postdata;
   int         datalen;
} tWgetInternal;

/*----------------------------------------------------------------------*
 * returns
 *   0 if sending request succeded
 *  -1 on URL syntax error
 *
 * The argument to the callback is of type (tWget *)
 */
int wget_GetData(tWgetInternal *wg,EventHandler callback, void *handle);
//not used int wget_Get(const char *url, EventHandler callback, void *handle);
//not used int wget_Post(const char *url, char *arg_keys[], char *arg_values[], EventHandler callback, void *handle);
//not used int wget_PostRaw(const char *url, char *content_type, char *data, int len, EventHandler callback, void *handle);
//not used int wget_GetFile(const char *url, const char *filename, EventHandler callback, void *handle);
//not used int wget_PostXmlFile(const char *url, char *data, int len, const char *filename, EventHandler callback, void *handle);
tWgetInternal *wget_Connect(const char *url, EventHandler callback, void *handle);
int wget_PostData(tWgetInternal *,char *data, int datalen, char *contenttype, EventHandler callback, void *handle);
int wget_PostDataClose(tWgetInternal *,char *data, int datalen, char *contenttype, EventHandler callback, void *handle);
//#ifdef RPC_UPLOAD    
int wget_PutData(tWgetInternal *,char *data, int datalen, char *contenttype, EventHandler callback, void *handle);
//#endif   // RPC_UPLOAD
int wget_Disconnect(tWgetInternal *);
const char *wget_LastErrorMsg(void);

int wget_AddPostHdr( tWgetInternal *wio, char *xhdrname, char *value);
void wget_ClearPostHdrs( tWgetInternal *wio);
#endif

