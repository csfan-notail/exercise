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
//#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <linux/if.h>
#include <syslog.h>
#include <fcntl.h>

#include "tr69cdefs.h"
#include "appdefs.h"
#include "utils.h"
#include "SOAPParser/RPCState.h"
#include "SOAPParser/xmlParserSM.h"
#include "SOAPParser/xmlTables.h"
#include "event.h"

#include "webproto/protocol.h"
#include "webproto/www.h"
#include "webproto/wget.h"
#include "informer.h"
#include "httpProto.h"
#include "httpDownload.h"
#include "acsListener.h"
#include "sysWrapper.h"
#include "manageServerParams.h"
#include "ipPingDiagnosticsParams.h"
#include "traceRouteDiagnosticsParams.h"
#include "tr69cdefs.h" /* defines for ACS state */

#define UPDATE_VALUE 	"tr069_update"
/** external functions **/
extern TRX_STATUS setMSrvrInformEnable(const char *value);
extern WanState wanState; //extern OM_TR69C_NET_FORWARD_IP_T wanState;
extern unsigned int uiAcsIP;       /* ACS服务器IP */
extern int g_log_level;

/** external data **/
extern ACSState            acsState;
extern eACSContactedState  acsContactedState;
extern TRxObjNode          rootDevice[];
extern int transferCompletePending;
extern int saveNotificationsFlag;
extern int sendGETRPC;			/* send a GetRPCMetods */
extern int tr69cTerm;			/* TR69C termination flag */
extern int notifyChanges;     /* notify parameter change flag */
extern int uiCFMfd;
extern int protect_connect_state_flag; //add by duanguiyuan
//extern OM_TR69C_ALARM_INFO_T g_alarm_info; /* add by duanguiyuan 20161126 */
extern LTEInfoState lteinfostate;
extern int g_device_first_up;
/** public data **/
eInformState   informState;
/** private data **/
HttpTask      httpTask;
//static HttpTask      httpTask;       /* http io desc */
static InformEvList  informEvList;
static SessionAuth   sessionAuth;
static int	sentACSNull;   /* set if last msg to ACS was NULL, cleared if non-null */
                           /* response received from ACS */
static int	notificationPending;
static int	connectionReqPending;
static int	periodicInformPending;
int	sendDiagComplete;	/* send Diagnostic inform */
int	g_pingWaiteTime = 10;	/* send Diagnostic inform */
Diag_CMD_INFO g_diag_cmd_info;
Diag_TYPE g_diag_type;
/** public functions **/

/** private functions **/
static void clearInformEventList(void);
static int  getInformEvCnt(void);
static int  putInformEvent(eInformEvent event);
static void putInformEventMmethod(eRPCMethods mMethod);
static int  isConnectionReqPending(void);
static void closeACSConnection(HttpTask *ht);
static void acsDisconnect(HttpTask *ht, AcsStatus acsStatus);
static void updateAuthorizationHdr(HttpTask *ht);
static void nullHttpTimeout(void *handle);
static void postComplete(void *handle);
static void Connected(void *handle);
static void sendInform(void* handle);
static void sendInformData(void);
static void periodicInformTimeOut(void *handle);
//static void cfmDelayTimeOut(void *handle);
static void checkModifiedParameters( void *handle );
static void startACSComm(void *handle);
void cfmKickListener(void *handle);


int isAcsConnected(void)
{
	return ((httpTask.wio != NULL) && (httpTask.wio->pc != NULL));
}

tProtoCtx *getAcsConnDesc(void)
{
   if (!isAcsConnected())
       return NULL;
       
#ifdef USE_SSL
   if (httpTask.wio->pc->type == iSsl && httpTask.wio->pc->ssl != NULL) {
      if (httpTask.wio->pc->sslConn > 0)
         return (httpTask.wio->pc);
      else
         return NULL;
   } else
      return (httpTask.wio->pc);   
#else
   return (httpTask.wio->pc);
#endif
}

static void clearInformEventList(void)
{
    informEvList.informEvCnt = 0;
   informEvList.mMethod = rpcUnknown;
}
static int getInformEvCnt(void)
{
   return informEvList.informEvCnt;
}
static int putInformEvent(eInformEvent event)
{
    if (informEvList.informEvCnt<MAXINFORMEVENTS) {
        informEvList.informEvList[informEvList.informEvCnt++] = event;
        return informEvList.informEvCnt;
    }
    return 0;
}
static void putInformEventMmethod(eRPCMethods mMethod)
{
   informEvList.mMethod = mMethod;
}

static int isConnectionReqPending(void)
{
   int ret = connectionReqPending;
   connectionReqPending = 0;
   return ret;
}

static void closeACSConnection( HttpTask *ht)
{
   if (ht->wio){
      wget_Disconnect(ht->wio);
      ht->wio = NULL;
   }
}

static void acsDisconnect(HttpTask *ht, AcsStatus acsStatus)
{
   static int  acsConnectFails;
   int         active = 0;
   char *value = NULL;
	time_t   time_date;
   stopTimer(nullHttpTimeout, (void *)ht);
   if (g_log_level)
         slog(TR69C_LOG_INFO, "acsDisconnect\n" );

//   saveNotificationAttributes();
   if(NULL != ht->authHdr)
   {
		free(ht->authHdr);
   		ht->authHdr =NULL;
   }
   
   closeACSConnection(ht);
   ht->xfrStatus  = acsStatus;
   ht->eHttpState = eClosed;
   ht->eAuthState = sIdle;
   //slog(TR69C_LOG_INFO, "xiao li  zi  jing  dian  zuo  pin  \n" );
   switch (acsStatus) {
      case eAuthError:
         ++acsConnectFails;
         slog(TR69C_LOG_INFO, "Failed authentication with ACS");
         break;
      case eConnectError:
      case eGetError:
      case ePostError:
         ++acsConnectFails;
         slog(TR69C_LOG_ERR, "ACS Disconnect with error %d", acsStatus);
         #ifdef ALLOW_DISCONNECT_ERROR
         saveACSContactedState();
         saveConfigurations();
         rebootCompletion();
         factoryResetCompletion();
         #endif
         break;
      case eAcsDone:
      default:
         acsConnectFails = 0;
         if (g_debug)
         slog(TR69C_LOG_DEBUG, "ACS Disconnect: ok");
         /* if no error then run thru pending disconnect actions */
         saveACSContactedState();
         saveConfigurations();
         rebootCompletion();
         factoryResetCompletion();
         break;
   }
   
   if (notificationPending) {
      active = checkActiveNotifications();
      notificationPending = 0;
   }
   if (tr69cTerm) {
      if (g_debug)
      slog(TR69C_LOG_DEBUG, "TR69C terminated");
      tr69cTerm = 0;         /* Clear tr69c termination flag */
      unlink(TR69_PID_FILE);   /* Remove tr69c pid file */
      exit(0);
   }
   /*add by duanguiyuan 20160325 upload 上传完了之后 发送信息报文*/
	if(acsContactedState == eACS_UPLOAD){
			printf(":::::::::::::http : upload connect  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> !!!\n");
		//	sleep(10);
			sendInform((void*)eACS_UPLOAD);
			return;
	}
   if(sendDiagComplete == 1)
            {
            	periodicInformPending = 0;
				sendDiagComplete =1;
                printf("http :sendDiagComplete connect =%d !!!\n",sendDiagComplete);
				printf("http :g_pingWaiteTime =%d !!!\n",g_pingWaiteTime);
				slog(TR69C_LOG_ERR, "TR69C daig_type = %d ",g_diag_cmd_info.daig_type);
			
				if(g_diag_cmd_info.daig_type ==1)  //Traceroute诊断 
				{
					g_diag_cmd_info.daig_time= time(NULL);
				 	TraceRoute_start();
					time_date= time(NULL);
					printf("http : CDM str  =%ld  --- %ld  !!!\n",g_diag_cmd_info.daig_time,time_date);
					g_diag_cmd_info.daig_time =time_date - g_diag_cmd_info.daig_time;

				}else if(g_diag_cmd_info.daig_type ==2 ){//IPPing诊断 
					IPping_start();
				}else{								//PPPoE诊断 
					om_PPPOE_Emulator_Start();
#if 0
					if(g_pingWaiteTime < 10)
					{
						g_pingWaiteTime = 10;
					}
                	sleep(g_pingWaiteTime); //延时5秒钟等待ping操作完成
#endif 
				}
				slog(TR69C_LOG_ERR, "TR69C g_pingWaiteTime = %d ",g_pingWaiteTime);
                sendInform((void*)eIEDiagnostics);
               // sendDiagComplete =0;
				printf("http :sendDiagComplete connect  send ok!!!\n");
			   return;
    }
   if (    active >0
       || connectionReqPending      /* check for pending events */
       || periodicInformPending
       || sendDiagComplete
       || sendGETRPC
       || (acsStatus==eAuthError)
       || (acsStatus==eConnectError)   /* add by zhanghaozhong */
       ) {
      if (acsConnectFails==0) {
	  	  slog(TR69C_LOG_ERR, "TR69C sendInform fo ASCDisconnect 0");
         sendInform(NULL);
      } else {
         /* retry ACS connect errors with decreasing retry time interval */
         int backOffTime = acsConnectFails>CONN_DECAYMAX?
             CONN_DECAYTIME*CONN_DECAYMAX*1000: CONN_DECAYTIME*acsConnectFails*1000;
		 slog(TR69C_LOG_ERR, "TR69C sendInform fo ASCDisconnect 1");
		 setTimer(sendInform, NULL, backOffTime);
         getURL(&value);
         if (value)
		 	{ 
		 		free(value);
				value=NULL;
			}
         acsState.retryCount++;  /* increment retry count -- reset to zero by inform response */
      }
   }


}  /* End of acsDisconnect() */

static void updateAuthorizationHdr( HttpTask *ht )
{
   if (ht->authHdr){
      if ( sessionAuth.qopType==eAuth ) {
         free(ht->authHdr);
		 ht->authHdr=NULL;
         ht->authHdr = generateNextAuthorizationHdrValue(&sessionAuth,
                                          acsState.acsUser, acsState.acsPwd );
         wget_AddPostHdr(ht->wio,"Authorization", ht->authHdr);    /* replace header */
      } else {
         /* must be basic or not "Auth" */
         wget_AddPostHdr(ht->wio,"Authorization", ht->authHdr);    /* replace header */
      }
   }
}  /* End of updateAuthorizationHdr() */

static void nullHttpTimeout(void *handle)
{
   HttpTask    *ht = (HttpTask*)handle;

   if (g_debug)
   fprintf(stderr,"nullHttpTimeout - disconnect\n");
   acsDisconnect(ht,ePostError);
}

void sendNullHttp(void)
{
   HttpTask *ht = &httpTask;
    
   if (ht->wio)
    	if (g_debug)
            fprintf(stderr,"sendNullHttp(%s) to ACS\n",ht->eHttpState==eClose? "close": "keepOpen");
        
   if (ht->postMsg) {
      free(ht->postMsg);
      ht->postMsg = NULL;
   }
   sendToAcs(0, NULL);
   sentACSNull =1;
   setTimer(nullHttpTimeout, (void *)ht, ACSRESPONSETIME/2); /* half of max */
}

#ifdef SUPPORT_ACS_CISCO
void informRspTimeout(void *handle)
{
   informState = eACSInformed;
   printf("write connection  state  1 \n");
   setACSContactedState(eACS_CONTACTED);
   if (transferCompletePending) {
      sendTransferComplete();
      transferCompletePending = 0;
   } else {
      sendNullHttp();
   }

   /* need this to get CISCO notifications to work.Since we never get a */
   /* InformResponse for the Cisco tool */
   initNotification();  /* update all copies of param data */
}
#endif

static void freeMsg(char *buf, int *bufSz) {
   if (buf != NULL) {
      free(buf);
      buf = NULL;
   }
   *bufSz = 0;
}

static char *readLengthMsg(tWget *wg, int readLth, int *mlth) {
   int bufCnt = 0, readCnt = 0;
   int bufLth = readLth;
   char *soapBuf = NULL;
   *mlth = 0;
   if(bufLth > 0 )
   	{
		   printf("len=%d\n",readLth);
			//char a_buf[12]={"adfgadf"};
		   // printf(">>>>>=%s\n",a_buf);
		  
			
		   printf("[%s:%d] soapBuf\n",__func__,__LINE__);
			soapBuf = (char *) malloc(readLth + 1);
			printf("[%s:%d] soapBuf\n",__func__,__LINE__);
		   if (soapBuf != NULL) 
		   	{   /* add one for null */
			      while (bufCnt < readLth) 
				  {
				         if ((readCnt = proto_ReadWait(wg->pc, soapBuf+bufCnt, bufLth)) > 0) 
						  {
					            bufCnt += readCnt;
					            bufLth -= readCnt;
								
				          } 
						 else 
				          {
					            /* read error */
					            *mlth = 0;
					            free(soapBuf);
					            soapBuf = NULL;
								
				          }
			      }
			      if (g_debug)
			      		fprintf(stderr, "soapBuf bufCnt=%d readLth=%d\n", bufCnt, readLth);
			      *mlth = bufCnt;
			      soapBuf[bufCnt] = '\0';
			  
		   }
		   printf(">>>>>>>>>> read soap >> 1\n");
		   
   }
   
   return soapBuf;
}

static char *readChunkedMsg(tWget *wg, int *mlth, int maxSize) {
   char *soapBuf = NULL;
   char chunkedBuf[128];   
   
   *mlth = 0;
   // read chunked size of first chunk*/
   if (proto_Readline(wg->pc, chunkedBuf, sizeof(chunkedBuf)) > 0) {
      int  chunkedSz = 0, readSz = 0;
      char *newBuf = NULL, *readBuf = NULL;
      
      sscanf(chunkedBuf, "%x", &chunkedSz);
#ifdef SUPPORT_ACS_SUPPORT_SOFT_TOT
printf("===> readChunkedMsg, first chunked data, chunked size = %x, chunked buffer = %s\n", chunkedSz, chunkedBuf);
#endif
      while (chunkedSz > 0) {
         // read chunked data
         readBuf = readLengthMsg(wg, chunkedSz, &readSz);  
#ifdef SUPPORT_ACS_SUPPORT_SOFT_TOT
         if (chunkedSz != readSz)
            printf("===> readChunkedMsg, chunked size = %d, read size = %d\n", chunkedSz, readSz);
#endif
         if (readBuf == NULL) {
            freeMsg(soapBuf, mlth);
            break;
         }
         if ((*mlth + readSz) > maxSize) {
            freeMsg(soapBuf, mlth);
            freeMsg(readBuf, &readSz);
            break;
         }
         // reallocate soap message size
         if ((newBuf = realloc(soapBuf, *mlth + readSz)) == NULL) {
            freeMsg(soapBuf, mlth);
            freeMsg(readBuf, &readSz);
            break;
         }
         // point soap message to new allocated memory
         soapBuf = newBuf;
         // append chunked data to soap message 
         strncpy(soapBuf + *mlth, readBuf, readSz);
         // increase soap message size
         *mlth += readSz;
         // free chunked data
         freeMsg(readBuf, &readSz);
         chunkedSz = 0;
         // flush off trailing crlf
         do {
            chunkedBuf[0] = '\0';
            readSz = proto_Readline(wg->pc, chunkedBuf, sizeof(chunkedBuf));
         } while (readSz > 0 && isxdigit(chunkedBuf[0]) == 0);
         // read chunked size of next chunk
         if (isxdigit(chunkedBuf[0]) != 0) {
            sscanf(chunkedBuf, "%x", &chunkedSz);
         } else {
            freeMsg(soapBuf, mlth);
         }
      }      
      // skip(flush) anything else
      proto_Skip(wg->pc);         
   }

#ifdef SUPPORT_ACS_SUPPORT_SOFT_TOT
   if (soapBuf == NULL)
      printf("===> readChunkedMsg return NULL soap message\n");
#endif
   return soapBuf;
}

/*
* tWget *wg is an connected web descriptor,
 *      *mlth is pointer to location of result read length,
 *      maxBufferSize is maximum size to read if non-zero. No limit if maxSize is 0.
 * Returns:
 *     pointer to response buffer or NULL.
 *      *mlth contain size of buffer. Undefined if return is NULL.
 */     
char *readResponse( tWget *wg, int *mlth, int maxBufferSize)
{
   char *soapBuf = NULL;
  
   if (wg->hdrs->content_length > 0) 
   {	
      soapBuf = readLengthMsg(wg, wg->hdrs->content_length, mlth); 
   } 
   else if (wg->hdrs->TransferEncoding && !strcasecmp(wg->hdrs->TransferEncoding,"chunked")) 
   {
      int maxSize = (maxBufferSize > 0) ? maxBufferSize : MAXWEBBUFSZ;
      soapBuf = readChunkedMsg(wg, mlth, maxSize);
   }
   return soapBuf;
}  /* End of readResponse() */

/*
 * Data has been posted to the server or an
 * error has ocurred.
 */
 /* 处理POST到ACS回来的响应 */
static void postComplete(void *handle)
{


   tWget       *wg = (tWget *)handle;
   HttpTask    *ht = (HttpTask*)wg->handle;
   SessionAuth *sa = &sessionAuth;
   int         skipResult=1;

   if (g_log_level)
       slog(TR69C_LOG_INFO, "postComplete\n");
   
   stopTimer(nullHttpTimeout, (void *)ht);
  
   if (wg->status == iWgetStatus_Ok) 
   	{
   	  
      if (ht->wio->hdrs->Connection && !strcasecmp(ht->wio->hdrs->Connection,"close"))
        ht->eHttpState = eClose;  
      if (g_debug)
      	fprintf(stderr, "Connection = %s\n", ht->eHttpState==eClose?"close": "keep-alive");
	  
      if (wg->hdrs->status_code == 401) 
	  	{
         /* need to send authenticate */
         char *hdrvalue=NULL; 
         if (wg->hdrs->content_length > 0 ||
             (wg->hdrs->TransferEncoding && streq(wg->hdrs->TransferEncoding,"chunked"))) 
           {	
					
					int  mlth;
		            char *tmpBuf=NULL;
					
		            if ((tmpBuf = readResponse(wg,&mlth,0)))
		            {
		            	
						if(tmpBuf)
							{
								 free(tmpBuf);
								 tmpBuf=NULL;
								
							}
						
					}  
					
		            if (g_debug)
		                fprintf(stderr,"proto_Skip ---\n");
		            skipResult = proto_Skip(wg->pc);
					
           }
		
         sentACSNull = 0;
         /* free in case of reauth requested during connection */
		 if(NULL!=ht->authHdr)
		 {
			free (ht->authHdr); 
			ht->authHdr = NULL;

		 }
         
		
         if (ht->eAuthState == sAuthenticating) 
		 	{
		            ht->eAuthState = sAuthFailed;
		           if(ht->postMsg != NULL)
					{
					 free(ht->postMsg); 
					 ht->postMsg = NULL;
					}
				   ht->postLth = 0;
		            /* disconnect and delay */
		            acsDisconnect(ht, eAuthError);
					
		            return;
            } 
		 else 
		 	{
            		ht->eAuthState = sAuthenticating;
			}

         if (g_debug)
            		fprintf(stderr, "WWW-Authenticate= %s\n", wg->hdrs->wwwAuthenticate);
         if (!(hdrvalue = generateAuthorizationHdrValue(sa, wg->hdrs->wwwAuthenticate,
                                                        "POST", ht->wio->uri,
                                                        acsState.acsUser,
                                                        acsState.acsPwd))) 
            {
		            slog(TR69C_LOG_ERR, "WWWAuthenticate header parsing error: %s", wg->hdrs->wwwAuthenticate);
		            if(ht->postMsg != NULL)
						 	 {
								 free(ht->postMsg); 
								 ht->postMsg = NULL;
						  	 }
		            acsDisconnect(ht, ePostError);
					
		            return;
            }
         ht->authHdr = hdrvalue;
         if (skipResult == 0 || ht->eHttpState == eClose) 
		 	{ 
				 	/* end of data on 401 skip_Proto() */
		            /* close connection and reconnect with Authorization header*/
		            closeACSConnection(ht);
		            ht->wio=wget_Connect(acsState.acsURL, Connected, ht);
		            if ( ht->wio == NULL ) 
					{
				             slog(TR69C_LOG_ERR, "ACS Connect failed: %s", wget_LastErrorMsg());
				             if(ht->postMsg != NULL)
						 	 {
								 free(ht->postMsg); 
								 ht->postMsg = NULL;
						  	 }
							 if(ht->authHdr != NULL)
							 {
								free (ht->authHdr); 
								ht->authHdr = NULL;

							 }
		               
		            }
            		return;
            } 
		 else 
		 	{
		            /* now just resend the last data with the Authorization header */
		            sendInformData();
		    }
         /* now we just return to wait on the response from the server */
         #ifdef FORCE_NULL_AFTER_INFORM
         	if (g_debug)
         fprintf(stderr, "set Timer to Force Null send to ACS (Cisco tool)\n");
         /******** ?????????????CISCO TOOL ???????? send null if server doesn't respond */
         setTimer(informRspTimeout, ht,1*1000); 
         #endif
		 
      } 
	  else 
	  	{
	    
         // If status != 401 after inform, and eAuthState == sIdle,
         // we're in the wrong authentication state.  The correct state should be from 
         // sAuthenticating to sAutenticated so disconnect unless
         // ACS does not have any authentications and returns status_code == 200
         // right after CPE sends the first Inform message 
         if ( ht->eAuthState == sIdle && wg->hdrs->status_code != 200 ) 
		 	{
		            ht->eAuthState = sAuthFailed;
		            /* disconnect and delay */
		            acsDisconnect(ht, eAuthError);   
		            if (g_debug)
		                fprintf(stderr, "Error: From sIdle -> sAuthenticated w/t sAuthenticating\n");
		            if (g_log_level)
		                slog(TR69C_LOG_ERR, "Error: From sIdle -> sAuthenticated w/t sAuthenticating\n");
		            return;
         	}

         ht->eAuthState = sAuthenticated;
         if ((wg->hdrs->status_code == 200) &&
             ((wg->hdrs->content_length > 0) ||
              (wg->hdrs->TransferEncoding && streq(wg->hdrs->TransferEncoding,"chunked")))) 
         {
		            /* allocate buffer and parse the response */
		            int     mlth;
		            char    *soapmsg;
		             
		            /* msg posted - free buffer */
		             if(ht->postMsg != NULL)
				 	 {
						 free(ht->postMsg); 
						 ht->postMsg = NULL;
				  	 }
		            resetNotificationBuf();
					/*change by duanguiyuan 2060323 BEGIN*/
					/*修改说明:不加判断条件的时候，重启和升级的时候，connectstate会被覆盖掉*/
				 	//setACSContactedState(eACS_CONTACTED);
		          	if (g_debug)
						printf("write connection  state  444    acsContactedState = %d \n",acsContactedState);
					slog(TR69C_LOG_ERR, "write connection  state  444 !!!! acsContactedState = %d \n",acsContactedState);
				   	if(protect_connect_state_flag != 1)
				   	{
						setACSContactedState(eACS_CONTACTED);
					}
					/*change by duanguiyuan 2060323 BEGIN*/
		            soapmsg = readResponse(wg, &mlth,0);

		            if ( soapmsg ) 
					{
				               eParseStatus   status;
				               ParseHow       parseReq;
				               #ifdef DUMPSOAPIN
				               fprintf(stderr, "SoapInMsg=>>>>>\n%s\n<\n", soapmsg);
				               #endif
				               parseReq.topLevel = envelopeDesc;
				               parseReq.nameSpace = nameSpaces;
				               status = parseGeneric(NULL,soapmsg, mlth,&parseReq);
				               free(soapmsg);
							   soapmsg=NULL;
				               if ( status == NO_ERROR )
							   {
				                  if ( runRPC() == eRPCRunFail ) /* couldn't run the RPC: so just disconnect */ 
				                     acsDisconnect(ht, eAcsDone);
				               } 
							   else 
							   {
				                  slog(TR69C_LOG_DEBUG, "informer: ACS Msg. Parse Error %80s", soapmsg);
				                  acsDisconnect(ht, eAcsDone);  /* force disconnect on parse error*/
				               }
		            } 
					else 
					{                
				               /* no response */
				               	if (g_debug)
				               fprintf(stderr, "informer: status = 200, no Soapmsg. sentACSNull=%d\n", sentACSNull);
				               if ( !sentACSNull ) 
							   {
				                  sendNullHttp();
				               }
				               acsDisconnect(ht, eAcsDone);
							  
		            }
         } 
		 else if ( wg->hdrs->status_code == 200 && !wg->hdrs->TransferEncoding ) 
		 {    
            
		            /* empty ACS message -- ACS is done */
		            if (g_debug)
		                fprintf(stderr, "informer: empty ACS msg - sentACSNull=%d\n", sentACSNull );
		            if (g_log_level)
		                slog(TR69C_LOG_INFO, "informer: empty ACS msg - sentACSNull=%d\n", sentACSNull );
		            /* msg posted - free buffer */
		          if(ht->postMsg != NULL)
				  {
					 free(ht->postMsg); 
					 ht->postMsg = NULL;
				  }
		            resetNotificationBuf();
		            if ( !sentACSNull ) 
					{
		               sendNullHttp();
           			}
		            if ( ht->eHttpState == eClose ) 
					{
		               closeACSConnection(ht);
		            } 
					else 
					{
		               acsDisconnect(ht, eAcsDone);
					  
		            }
         } 
		 else if ( wg->hdrs->status_code == 204 ) 
		 {
		            /* msg posted - free buffer */
					printf("http : 204  !!!\n");
		          if(ht->postMsg != NULL)
				  {
					 free(ht->postMsg); 
					 ht->postMsg = NULL;
				  }
		            resetNotificationBuf();
		            if ( !sentACSNull ) 
					{
		               sendNullHttp();
		            } 	
			
					 /*add by duanguiyuan 20160325 upload 上传完了之后 发送信息报文*/
					/*add by duanguiyuan
					 说明:  诊断上报的时候，ascDisconnect也会发送报文，原因未知，
					 因此在此处把 ascDisconnect规避掉,后期有空待查
					 201060331*/
           
		           //ht->wio->keepConnection = eCloseConnection;
		            acsDisconnect(ht,eAcsDone);
				
				 
		            if (g_log_level)
		                slog(TR69C_LOG_INFO, "204 not content disconnect with acs\n");
         } 
		 else if ( wg->hdrs->status_code == 100 ) 
		 {
		            /* 100 Continue: Just ignore this status */
		            /* msg posted - free buffer */
		          if(ht->postMsg != NULL)
				  {
					 free(ht->postMsg); 
					 ht->postMsg = NULL;
				  }
		            resetNotificationBuf();
         } 
		 else if ( wg->hdrs->status_code >= 300 && 
                     wg->hdrs->status_code <= 307 &&
                     wg->hdrs->locationHdr ) 
         {
		            /* Redirect status with new location */
		            /* repost msg to new URL */
		            char redirectURL[256];
		            strcpy(redirectURL, wg->hdrs->locationHdr);
		            closeACSConnection(ht);
		            ht->eHttpState = eStart;
		            slog(TR69C_LOG_DEBUG, "Redirect to %s", redirectURL);
		            ht->wio=wget_Connect(redirectURL, Connected, ht);
		            if ( ht->wio == NULL ) 
					{
		               slog(TR69C_LOG_ERR, "Redirect failed: %s", wget_LastErrorMsg());
		               acsDisconnect(ht, ePostError);
					   
		            }
         } 
		 else 
		 {
		            /* msg posted - free buffer */
		          if(ht->postMsg != NULL)
				  {
					 free(ht->postMsg); 
					 ht->postMsg = NULL;
				  }
		            resetNotificationBuf();
		            slog(TR69C_LOG_ERR, "informer: Unknown status_code=%d received from ACS or encoding", wg->hdrs->status_code);
		            acsDisconnect(ht, ePostError);
					
         }
      }
	 
   } 
   else 
   {
			      slog(TR69C_LOG_ERR, "informer: Post to ACS failed, Status = %d %s\n", wg->status, wg->msg);
				  if(ht->postMsg != NULL)
				  {
					 free(ht->postMsg); 
					 ht->postMsg = NULL;
				  }
			      acsDisconnect(ht, ePostError);
				
   }

}  /* End of postComplete() */

static void setupEventList(eInformEvent iEvent)
{
   tProtoCtx      *pc   = NULL;
   int            bufsz = 0; 
   slog(TR69C_LOG_DEBUG, "===> setupEventList( %d )\n", iEvent);
   slog(TR69C_LOG_ERR, "===> SSSSSSSSSSSSSSSSSs  state = %d \n", informState);
   printf("inform state = %d \n",informState);  
   printf("acsContactedState = %d \n",acsContactedState); 
 
   if (g_log_level)
        slog(TR69C_LOG_INFO, "===> setupEventList( %d )\n", iEvent);
   clearInformEventList();

   if (getAllNotifications(&pc, &bufsz) > 0)
      	putInformEvent(eIEValueChanged);
   
  
   
   if (informState == eJustBooted) //设备启动要发送的 EvenCode
   {
	      eACSContactedState cState;
		 
		  
	      if ((cState=getACSContactedState()) == eACS_NEVERCONTACTED)  // cms_tr69c_connect_state = 0 
		  {	
				
				printf("PasswordAuthFlag = %d \n",acsState.passAuthflag);
		  		if(acsState.passAuthflag == 1) //添加password 认证
		  		{
					
						printf("password auth !!!!!!!!!!!!!\n");
						putInformEvent(ePasswordAuth);
				}
				else
				{		
						
	        		    putInformEvent(eIEBootStrap);
	         		    setACSContactedState(eACS_CONTACTED);
	        	 	    saveACSContactedState();
				} 
	      }
	      else 
		  {
		  		 
		         switch (cState) 
				 	{
						 	case eACS_CONTACTED:  /*add by duanguiyuan 20160328 非首次连接 进行认证*/
								putInformEvent(eIEBoot);
								break;
				            case eACS_DOWNLOADREBOOT:  //升级下载完 重启  升级  cms_tr69c_connect_state = 0 
				               putInformEvent(eIEBoot);
				               putInformEvent(eIETransferComplete);
				               putInformEvent(eIEMethodResult);
				               putInformEventMmethod (rpcDownload);
				               break;
				            case eACS_SETVALUEREBOOT:
							   putInformEvent(eIEBoot);
				               putInformEvent(eIEValueChanged);
				               putInformEvent(eIEMethodResult);
				               putInformEventMmethod (rpcSetParameterValues); 
				               break;
				            case eACS_ADDOBJECTREBOOT:
							   putInformEvent(eIEBoot);
				               putInformEvent(eIEValueChanged);
				               putInformEvent(eIEMethodResult);
				               putInformEventMmethod (rpcAddObject);
				               break;
				            case eACS_DELOBJECTREBOOT:
							   putInformEvent(eIEBoot);
				               putInformEvent(eIEValueChanged);
				               putInformEvent(eIEMethodResult);
				               putInformEventMmethod (rpcDeleteObject);
				               break;
				            case eACS_RPCREBOOT:   				//重启  cms_tr69c_connect_state = 0 
						//	   putInformEvent(eIEMethodResult);   //添加M Reboot
							   putInformEvent(eIEBoot);
				               putInformEvent(eIEMethodResult);
				               putInformEventMmethod (rpcReboot);
				               break;
				            default:
				               break;
		           }
	      }
   } 
   else 
   {  
	   	 //设备运行中的要发送的 Evencode
	   	 
	      switch (acsContactedState) 
		  {
	         case eACS_NEVERCONTACTED:
	            putInformEvent(eIEBootStrap);
	            setACSContactedState(eACS_CONTACTED);
	            saveACSContactedState();
	            break;
	         case eACS_DOWNLOADREBOOT:
	            putInformEvent(eIETransferComplete);
	            putInformEvent(eIEMethodResult);
	            putInformEventMmethod(rpcDownload);
	            break;
			//#ifdef RPC_UPLOAD    
	         case eACS_UPLOAD:
	            putInformEvent(eIETransferComplete);
	            putInformEvent(eIEMethodResult);
	            putInformEventMmethod(rpcUpload);
	            break;
			//#endif   // RPC_UPLOAD
	         default:
	            break;
	      }
		  if(acsContactedState != eACS_UPLOAD)
		  { 
		  		/*add by duanguiyuan 上传文件的时候 过滤掉 心跳*/
	      		// Periodic Inform event  心跳
	     		 if (periodicInformPending || iEvent == eIEPeriodix) 
				 {
	         		putInformEvent(eIEPeriodix);
	         		periodicInformPending = 0;
	      		 }
		  }
	      // Connection Request event
	      if (isConnectionReqPending() || iEvent == eIEConnectionRequest)
	         	putInformEvent(eIEConnectionRequest);
	      		// Diagnostic Complete event
	      if (sendDiagComplete) 
		  {
	         	putInformEvent(eIEDiagnostics);
	         	sendDiagComplete = 0;
	      }
	      // Other events are eIEScheduled, eIEValueChanged, eIEKicked, eIEXVendor
	      // up to now only support eIEValueChanged
	      switch (iEvent) 
		  {
	         case eIEScheduled:
	         case eIEValueChanged:
	         case eIEKicked:
	         case eIEXVendor:
	         /* BEGIN: Added by zhanghaozhong, 2013/5/28   PN:增加告警上报功能 */
	         case eIEAlarm:
	         /* END:   Added by zhanghaozhong, 2013/5/28   PN:增加告警上报功能 */
			case eAlarmByTAU:
			 	putInformEvent(iEvent);
	            break;
	         default:
	            break;
	      }
	      // force a Periodic event if none others
	      if (getInformEvCnt() == 0)      
	         putInformEvent(eIEPeriodix); 
   }

  /* add by duanguiyuan 20151027 */
#if 0
	int actmp = 0;
  	if(0 !=om_tr69c_reboot_get(&actmp))
	{
    	 slog(TR69C_LOG_ERR, "om_tr69c_reboot_get error .................\n");
		 
	}else{
		slog(TR69C_LOG_ERR, "om_tr69c_reboot_get  actmp = %d....................\n",actmp);
		if(actmp==1)
		{
  	 		putInformEvent(eIEMethodResult);
         	putInformEventMmethod (rpcReboot);
			actmp = 0;
			om_tr69c_reboot_set(actmp);
		}
	}
	actmp = 0;
	if(0 !=om_tr69c_Upgrade_Complete_get(&actmp))
	{
    	 slog(TR69C_LOG_ERR, "om_tr69c_Upgrade_Complete_get error .................\n");
	}else{
	slog(TR69C_LOG_ERR, "om_tr69c_Upgrade_Complete_get  actmp = %d....................\n",actmp);
		if(actmp==1)
		{
  	 		putInformEvent(eIETransferComplete);
   			putInformEvent(eIEMethodResult);
   			putInformEventMmethod (rpcDownload);
			actmp = 0;
			om_tr69c_Upgrade_Complete_set(actmp);
		}
	}
#endif
  /*   */

               
}  /* End of setupEventList() */

/*
 * The connection to the ACS has been completed or
 * an error has ocurred.
 */
static void Connected(void *handle)
{
	/*printf("[%s:%d] port_s 0\n",__func__,__LINE__);
	char *port_s =NULL; 
	printf("[%s:%d] port_s 1\n",__func__,__LINE__);
	port_s=(char*)malloc(1065);
	printf("[%s:%d] port_s 2\n",__func__,__LINE__);
	free(port_s);
	port_s=NULL;
	printf("[%s:%d] port_s 3\n",__func__,__LINE__);
	*/
slog(TR69C_LOG_DEBUG, "ACS Connect  0 \n");

   HttpTask *ht = &httpTask;
   tWget    *wg = (tWget *)handle;
   eInformEvent iEvent = (eInformEvent)wg->handle;   

   if (wg->status != 0) {
      free(ht->postMsg); ht->postMsg = NULL;
      acsDisconnect(ht, eConnectError);
      slog(TR69C_LOG_ERR, "ACS Connect Status = %d %s", wg->status, wg->msg);
      return;
   }
slog(TR69C_LOG_DEBUG, "ACS Connect  1 \n");
   
   if (ht->wio==NULL) {
      ht->eHttpState = eClosed;
      slog(LOG_CRIT,"Error -- pointer to IO desc is NULL");
      return;
   }
slog(TR69C_LOG_DEBUG, "ACS Connect  2 \n");
slog(TR69C_LOG_DEBUG, "ACS Connect  iEvent = %d \n",iEvent);

   ht->eHttpState = eConnected;
   setupEventList(iEvent);
   sendInformData(); /* TODO */
slog(TR69C_LOG_DEBUG, "ACS Connect  3 \n");

}  /* End of Connected() */

/*
 * Send the current buffer to the ACS. This is an async call. The 
 * return status only indicates that the connection has started.
 * The httpTask structure represents the only connection to an
 * ACS. If the connection is up then it is reused; otherwise,
 * a new connection is attempted.
 * 
 * Returs 0: oK
 *       -1: Error
 */
void sendToAcs(int contentLen, char *buf)
{
	/*printf("[%s:%d] port_s 0\n",__func__,__LINE__);
	char *port_s =NULL; 
	printf("[%s:%d] port_s 1\n",__func__,__LINE__);
	port_s=(char*)malloc(1065);
	printf("[%s:%d] port_s 2\n",__func__,__LINE__);
	free(port_s);
	port_s=NULL;
	printf("[%s:%d] port_s 3\n",__func__,__LINE__);
	*/
	
   HttpTask *ht = &httpTask;

   if (ht->postMsg) 
   {
      slog(TR69C_LOG_ERR,"sendToAcs internal error: postMsg buffer not null");
      free(ht->postMsg);
	  ht->postMsg=NULL;
   }
   if (getAcsConnDesc() == NULL) {
      slog(TR69C_LOG_ERR, "Warning: Try to send message to ACS while connection is NULL!");
      return;
   }
   ht->content_len = contentLen;
   ht->postMsg     = buf;
   ht->postLth     = buf? strlen(buf): 0;
   wget_ClearPostHdrs(ht->wio);
   #ifdef GENERATE_SOAPACTION_HDR
   wget_AddPostHdr(ht->wio,"SOAPAction", "");
   #endif
   updateAuthorizationHdr(ht);
   if (ht->eHttpState == eClose) {
      wget_PostDataClose(ht->wio, ht->postMsg, ht->postLth, "text/xml", 
                         postComplete,( void*) ht);
   } else {
      wget_PostData(ht->wio, ht->postMsg, ht->postLth, "text/xml",
                    postComplete, (void *)ht);
   }
}  /* End of sendToAcs() */

extern char *informEventStr[];

/* 
 * Send an Inform message. The possible handle values are
 * eIEConnectionRequest
 * eIETransferComplete
 * eIEJustBooted.
 * The other possible events are set by xxxPending flags.
 * eIEPeriodix
 * eIEDiagnostics
 * 
 */
static void sendInform(void* handle)
{
	/*printf("[%s:%d] port_s 0\n",__func__,__LINE__);
	char *port_s =NULL; 
	printf("[%s:%d] port_s 1\n",__func__,__LINE__);
	port_s=(char*)malloc(1065);
	printf("[%s:%d] port_s 2\n",__func__,__LINE__);
	free(port_s);
	port_s=NULL;
	printf("[%s:%d] port_s 3\n",__func__,__LINE__);
	*/
   eInformEvent   iEvent = (eInformEvent)handle;
   static unsigned int iCount;
  // char acs_url_buf[512];
   printf("sendInform Event = %s \n",informEventStr[iEvent]);
slog(TR69C_LOG_DEBUG, "sendInform Event = %s   0", informEventStr[iEvent]);
#if 0
   if ( isAcsConnected() ){
      connectionReqPending = connectionReqPending ||(iEvent==eIEConnectionRequest);
      if (acsContactedState == eACS_NEVERCONTACTED)
         // need to close connection to avoid system crash problem
         // when system is booted up without URL, then its value is changed                
         // from WEB UI, then connection request is performed, then system is crashed                
         // ==> call closeACSConnection to fix CR #16299
         closeACSConnection(&httpTask); 
      else                                       
         return;
   }
#endif
   sentACSNull = 0;

   updateConnectionInfo(); /* TODO missing! */
   /* add by duanguiyuan 20161130 执行脚本更新 tr069配置文件中的 信息 */
   system(UPDATE_VALUE);
  // slog(TR69C_LOG_DEBUG, "sendInform tau<><>><><><><><>><><><><>");

   initACSConnectionURL(); /* TODO seems useless */
slog(TR69C_LOG_ERR, "sendInform Event = %s   1", informEventStr[iEvent]);
   if (acsState.acsURL != NULL && acsState.acsURL[0] != '\0') {
      HttpTask *ht = &httpTask;
slog(TR69C_LOG_ERR, "Connect to ACS at %s\n", acsState.acsURL);
      ht->eHttpState = eStart;
//sprintf(acs_url_buf,"http://%s:8080/",acsState.acsURL);
      ht->wio = wget_Connect(acsState.acsURL, Connected, handle);
slog(TR69C_LOG_ERR, "ht->wio->status = %d\n",ht->wio->status);

      if (ht->wio == NULL) {
         setTimer(sendInform, handle, 60*1000); /* 60 sec 尝试重新发起inform */
            slog(TR69C_LOG_DEBUG, "ACS Connect Failed: %s, try send inform again after 60s", wget_LastErrorMsg());
         if (iCount < 5)
            slog(TR69C_LOG_ERR, "ACS Connect Failed: %s, try send inform again after 60s", wget_LastErrorMsg());
         iCount++;
         if (iCount > 0xFFFFFFF0)
            iCount = 0;
      }
   }
}  /* End of sendInform() */

static void sendInformData(void)
{
	/*printf("[%s:%d] port_s 0\n",__func__,__LINE__);
	char *port_s =NULL; 
	printf("[%s:%d] port_s 1\n",__func__,__LINE__);
	port_s=(char*)malloc(1065);
	printf("[%s:%d] port_s 2\n",__func__,__LINE__);
	free(port_s);
	port_s=NULL;
	printf("[%s:%d] port_s 3\n",__func__,__LINE__);
	*/
   RPCAction *a = newRPCAction();

   /* Reinitialize instances that have been added by the
    * Web user interface or by some other non-TR69 method.
    */
   if (g_log_level)
     slog(TR69C_LOG_INFO, "sendInformData-->reInitInstances\n");
   //reInitInstances(); /* TODO missing! */

   /* writeIp is NOT SAFE! */
   buildInform(a, &informEvList, writeIp(wanState.ip));
   freeRPCAction(a);
}
/*
 * Periodic Timer callback.
 * if inform is enable and informInterval > 0 then always reset.
 */
void periodicInformTimeOut(void *handle)
{
    periodicInformPending = 1;
	getLetstateinfo();
	printf("send inform  periodic !!!!\n");
   sendInform((void*)eIEPeriodix);
   if (acsState.informEnable && acsState.informInterval && acsState.PeriodicInformEnable)
      setTimer(periodicInformTimeOut,NULL, acsState.informInterval*1000);
   if (g_log_level)
         slog(TR69C_LOG_INFO, "periodicInformTimeOut sendInform event eIEPeriodix\n" );
    slog(TR69C_LOG_ERR, "periodicInformTimeOut sendInform event eIEPeriodix\n" );

#ifdef USE_DMALLOC
    dmalloc_log_unfreed();
    //printf("***  after dmalloc_log_unfreed\n");      
#endif // USE_DMALLOC
}  /* End of periodicInformTimeOut() */
void alarmTimeOut(void *handle)
{
	//&&(g_alarm_info.alarm_flag==TR69C_ALARM_YES)
	if(alarm_check_procc() > 0)
	{
		sendInform((void*)eAlarmByTAU);
	}
	setTimer(alarmTimeOut,NULL, 10*1000);
}
/*
 * Called from setter function to update next inform time
 */
void resetPeriodicInform(unsigned long interval)
{
   stopTimer(periodicInformTimeOut, NULL);
   setTimer(periodicInformTimeOut,NULL, interval*1000);
}

/*
 * 
 * This can only be called after doDownload() has already sent the Download response.
 * It is called by the httpDownload functions if an error occurs.
 * Thus we need to send a TransferComplete message here.
 * If the ACS is not connected then an Inform needs to be started.
 */
void sendDownloadFault()
{

   if (isAcsConnected()) {
      transferCompletePending = 1;
   }
   else {
      transferCompletePending = 1;
      sendInform((void*)eIETransferComplete);
   }
}  /* End of sendDownloadFault() */
void startACSComm(void *handle)
{
    char *acTmp = NULL;
    int i = 0;
 //	if (getWanInfo(&wanState)) {
   	if (i == 0) {

      startACSListener();
      // comment out the following line to send Inform at start up even when Periodic Inform is disable
      /*change by duanguiyuan 20160323 BEGIN*/
	 /*
     if (acsState.informEnable){
 			sendInform((void*) eJustBooted);
	 }*/
	 printf(" g_device_first_up = %d !!!!\n",g_device_first_up);
 	if (acsState.informEnable){
		
		if(g_device_first_up == 1)    //connect_state != 1 正常进行
 		{	
 			informState = 0;
 			sendInform((void*) eJustBooted);
		}else{							  //connect_state == 1 设备开机启动，或者，程序挂掉重新运行
			sendInform((void*) eACSInformed);  //开启tr69cd进程的时候，走到这里，发送的Eventcode是 心跳  因为未知
			acsContactedState = 1;
		} 
	 /*change by duanguiyuan 20160323 END*/
      /* WAN check timer is stopped -- */
		printf("periodc is all read!!!!\n");
      if (acsState.informEnable) {
	  		if(acsState.informInterval && acsState.PeriodicInformEnable)
	  		{
	  			printf("periodc is begin !!!!\n");
         		setTimer(periodicInformTimeOut, NULL, acsState.informInterval*1000);
			}
			/* add by 段归源 20161126 设置 告警上报 功能 */
		 	setTimer(alarmTimeOut, NULL, 10*1000);
 		}
	}
   } else {
        getURL(&acTmp);
        if (NULL != acTmp)
        {
			 free(acTmp);
			 acTmp=NULL;
		}
           
        /* keep checking until wanup */
        setTimer(startACSComm,NULL,CHECKWANINTERVAL);  
        acsState.retryCount++;  /* increment retry count -- reset to zero by inform response */
		printf(" startACSComm connection again !!!\n");

   }
}  /* End of startACSComm() */

/* called once when tr69c starts - must be called AFTER all instances are initialized */
void initInformer(void)
{
    slog(TR69C_LOG_DEBUG,"start initInformer");
/*add by duanguiyuan 20160331 password认证开关*/
		getPasswordAuthFlag();
/*		while(acsState.passAuthflag){
			printf("password auth !!!!!");
			sendInform((void*) eJustBooted);
			sleep(5*60);
			getPasswordAuthFlag();
		}*/
   /* start trying ACSComm in a moment */
   setTimer(startACSComm,NULL, 500);   //定时器
slog(TR69C_LOG_DEBUG,"initInformer   0");

   initNotification();  /* this creates all the InstanceDope descriptors for current */
                       /* instancesf or all parameters */
slog(TR69C_LOG_DEBUG,"initInformer   1");

						//restoreNotificationAttr();
   initNotification();  /* this applies all the notification attributes. Retrieve a */
                        /* current copy of the parameter values for any notify params*/
slog(TR69C_LOG_DEBUG,"initInformer   2");
	setCallback(&informState, sendInform, (void *)eIEConnectionRequest);
                        /* flag to indcate signal from conn Req*/ 
   /* setCallback函数设置了一个动作，如果ACS连接CPE会调用acsReadData()函数,再调用
   *        notifyCallbacks(&informState);  this will initiate the inform code
   *   函数。notifyCallbacks()函数会找到设置回调函数的&informState标志，从而实现CPE发送
   *   inform报文给ACS，EventCode 为ConnectionRequest。
   */
slog(TR69C_LOG_DEBUG,"initInformer   3");
   setTimer(checkModifiedParameters, NULL, 120*1000);
slog(TR69C_LOG_DEBUG,"initInformer   end");

}  

static void checkModifiedParameters( void *handle )
{
   int      notifies;
   if (g_debug)
     fprintf(stderr, "checkModifiedParameters\n");

   /* Attempt to lock CFM configuration data before checking for notification */
   stopTimer(checkModifiedParameters,handle);
   //reInitInstances();
   saveNotificationsFlag = 1;
   if ((notifies = checkActiveNotifications()) > 0) {
      if (g_debug)
        fprintf(stderr, "Active notifications =%d\n",notifies); 
      if ( isAcsConnected())
         notificationPending++;
      else
         sendInform(NULL);
   } else {
        ;
      /* no active Notifications- do nothing */
   }
   setTimer(checkModifiedParameters, handle, 60*1000);  /* 60秒遍历所有节点一次 */
}  /* End of cfmDelayTimeOut() */

//extern TRX_STATUS setMSrvrInformEnable(const char *value);
int iAlarmPending = 0;   /* 告警标志 */

/* 如果在主循环里select到了内容，就会调用此函数去处理监听CFM(CGI、CLI)发送过来的内容。 by zhanghaozhong 2013-1-25*/
void cfmKickListener(void *handle)
{
   char *acTmp = NULL;
   int      fd = (int)handle;

   eCFMMsg  buf;
   if ( read(fd, (void *)&buf, sizeof(buf))>0 ) {
      if (g_debug)
      fprintf(stderr, "cfmKickListener called msg=%d\n", buf);
      switch ( buf ) {
      case SEND_GETRPC:
         sendGETRPC = 1;
         sendInform(NULL);
         break;
      case SEND_ENABLE_INFORM:      /* CGI ok */
         slog(TR69C_LOG_DEBUG, "recv ENABLE_INFORM form CGI");
         setMSrvrInformEnable("1");
         resetPeriodicInform(acsState.informInterval);
         if (g_debug)
         fprintf(stderr,"rec SEND_ENABLE_INFORM form CGI\n");
         break;
      case SEND_DISABLE_INFORM:      /* CGI ok */
         slog(TR69C_LOG_DEBUG, "recv DISABLE_INFORM form CGI");
         setMSrvrInformEnable("0");
         stopTimer(periodicInformTimeOut, NULL);  
         if (g_debug)
         fprintf(stderr,"rec SEND_DISABLE_INFORM form CGI\n");
         /* BEGIN: Added by zhanghaozhong, 2013/8/30   PN:使网页上的开关生效 */
         exit(0);
         /* END:   Added by zhanghaozhong, 2013/8/30   PN:使网页上的开关生效 */
      case SEND_DIAG_COMPLETE:
         sendDiagComplete = 1;
         sendInform((void*)eIEDiagnostics);
         break;
      case SEND_ACS_URL_CHANGED:      /* CGI ok 如果是value change，则在发送inform报文时，
                                        会遍历树，将节点的值get出来(前提是已经设置了该实例的属性)，
                                        此时会将ACS的url赋值到acsState全局变量里*/
         setACSContactedState(eACS_NEVERCONTACTED);
         saveACSContactedState();
         sendInform((void*)eIEValueChanged);
         if (g_debug)
         fprintf(stderr, "rec SEND_ACS_URL_CHANGED form CGI\n\n");
         
         break;
      case SEND_WAN_CHANGED:        /* 暂时没有实现 */
         /* BEGIN: Deleted by zhanghaozhong, 2013/6/3   PN:ACS使用request里的IP和端口，此处不需要上报 */
         /*sendInform((void*)eIEValueChanged);
         if (g_debug)
         fprintf(stderr,"rec SEND_WAN_CHANGED form CGI\n");*/
         /* END:   Deleted by zhanghaozhong, 2013/6/3   PN:ACS使用request里的IP和端口，此处不需要上报 */
         
         break;
      case SEND_ENABLE_NONECONNREQAUTH:      /* CGI */
         acsState.noneConnReqAuth = 1;
         if (g_debug)
         fprintf(stderr,"rec SEND_ENABLE_NONECONNREQAUTH form CGI\n");
         
         break;
      case SEND_DISABLE_NONECONNREQAUTH:      /* CGI */
         acsState.noneConnReqAuth = 0;
         if (g_debug)
         fprintf(stderr,"rec SEND_DISABLE_NONECONNREQAUTH form CGI\n");
         
         break;

      case SEND_ACS_USRNAME_CHANGED:      /* CGI  */
         if (g_debug)
         fprintf(stderr,"rec SEND_ACS_USRNAME_CHANGED form CGI\n");
         
         //getUsername(&acTmp);/* 这些get函数不能执行，如果执行，在发起inform的时候不能比较出是否value change了（旧值被此处覆盖了） */
         sendInform((void*)eIEValueChanged);
         break;
      case SEND_ACS_PASSWD_CHANGED:      /* CGI */
         if (g_debug)
         fprintf(stderr,"rec SEND_ACS_PASSWD_CHANGED form CGI\n");
         
         //getPassword(&acTmp);
         sendInform((void*)eIEValueChanged);
         break;
      case SEND_PERIODIC_INFORM_ENABLE:      /* CGI*/
         if (g_debug)
         fprintf(stderr,"rec SEND_PERIODIC_INFORM_ENABLE form CGI\n");
         break;
         
      case SEND_PERIODIC_INFORM_DISABLE:      /* CGI*/
         if (g_debug)
         fprintf(stderr,"rec SEND_PERIODIC_INFORM_DISABLE form CGI\n");
         break;
         
      case SEND_PERIODIC_TIME:      /* CGI*/
         if (g_debug)
         fprintf(stderr,"rec SEND_PERIODIC_TIME form CGI\n");
         
          getPeriodicInformInterval(&acTmp);
          if (NULL != acTmp)
          {
				free(acTmp);
				acTmp=NULL;
		  }
            
          resetPeriodicInform(acsState.informInterval); 
         break;
      case SEND_CPE_USRNAME_CHANGED:
         if (g_debug)
         fprintf(stderr,"rec SEND_CPE_USRNAME_CHANGED form CGI\n");
         
         /*getConnectionUsername(&acTmp);
         if (NULL != acTmp)
            free(acTmp);*/
      case SEND_CPE_PASSWD_CHANGED:      /* CGI  */
         if (g_debug)
         fprintf(stderr,"rec SEND_CPE_PASSWD_CHANGED form CGI\n");
         
         //getConnectionPassword(acTmp);
         sendInform((void*)eIEValueChanged);   /* 告知ACS，ACS访问CPE的用户名或密码改变了，ACS需要更新数据库 */
         break;
      case SEND_CPE_GET_TR069_STA:      /* CGI  */
         if (g_debug)
         fprintf(stderr,"rec SEND_CPE_GET_TR069_STA form CGI\n");
         
         //getTr069Status();
         break;
#if 0
      case ALARM:      /* 告警上报 */
         if (g_debug)
            fprintf(stderr,"rec ALARM form oma or sysmonitor\n");
         memcpy(&g_stUdpMSG, &buf, sizeof(PAL_UDP_MSG));
         if ((g_stUdpMSGTmp.stAlarm.uiAlarmCode != g_stUdpMSG.stAlarm.uiAlarmCode) || 
            (g_stUdpMSGTmp.stAlarm.uiAlarmState != g_stUdpMSG.stAlarm.uiAlarmState))
        {
            iAlarmPending = 1; /* 收到了告警上报 */
            memcpy(&g_stUdpMSGTmp, &g_stUdpMSG, sizeof(PAL_UDP_MSG));
            sendInform((void*)eIEAlarm);
        }
         
         break;
      case SIMPLE_LOG:      /* 简单日志打印到syslog */
         if (g_debug)
            fprintf(stderr,"tr69c log is simple\n");
         slog(TR69C_LOG_INFO, "tr69c log is simple");
         g_log_level = 0;
         break;
      case DETAIL_LOG:      /* 复杂日志打印到syslog */
         if (g_debug)
            fprintf(stderr,"tr69c log is detail\n");
         slog(TR69C_LOG_INFO, "tr69c log is detail");
         g_log_level = 1;
         break;
#endif

      default:
         if (g_debug)
         fprintf(stderr,"rec default form CGI it's not normal!!!!!\n");
         
         /* BEGIN: Added by zhanghaozhong, 2013/5/31   PN:增加告警上报功能 */
         //stopTimer(cfmDelayTimeOut, NULL);   /* restart timer on each cfm msg */
         //setTimer(cfmDelayTimeOut,NULL, CFMKICKDELAY );
         /* END:   Added by zhanghaozhong, 2013/5/31   PN:增加告警上报功能 */
         break;
      }
   }
}  /* End of cfmKickListener() */

/* */
static int udp_listen(unsigned int ip, int port, char *inf, int broadcast)
{
   struct ifreq         interface;
   struct sockaddr_in   addr;
   int                  fd;
   int                  n = 1;

if (g_debug)
   slog(TR69C_LOG_DEBUG,"Opening listen socket on 0x%08x:%d %s\n", ip, port, 
        inf == NULL ? "all ifcs" : inf);

   if ((fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {   /* PF_INET:protocol family */
      return -1;
   }
  
   memset(&addr, 0, sizeof(addr));
   addr.sin_family = AF_INET;
   addr.sin_port = htons(port);
   addr.sin_addr.s_addr = htonl(ip);

   if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *) &n, sizeof(n)) == -1) {
      close(fd);
      return -1;
   }
   if (broadcast &&
      setsockopt(fd, SOL_SOCKET, SO_BROADCAST, (char *) &n, sizeof(n)) == -1) {
      close(fd);
      return -1;
   }
  
   if (inf != NULL) {
      strncpy(interface.ifr_ifrn.ifrn_name, inf, IFNAMSIZ);
      if (setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE,
          (char *)&interface, sizeof(interface)) < 0) {
         close(fd);
         return -1;
      }
   }
  
   if (bind(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr)) == -1) {
      close(fd);
      return -1;
   }
   return fd;
}  /* End of udp_listen() */  


void initCFMListener(void)
{
   int fd;
    /* 此socet用于侦听CGI或其他进程发来的UDP报文 */
   if ((fd = udp_listen(INADDR_ANY, CFMLISTENPORT, NULL, FALSE)) < 0) { /* INADDR_ANY 是IP为0.0.0.0任意IP的意思 */
      slog(TR69C_LOG_ERR, "could not initiate UDP server socket (port=%d)", CFMLISTENPORT);
      uiCFMfd = 0;
      return;
   }
   uiCFMfd = fd;
   setListener( fd, cfmKickListener, (void *)fd );
}  /* End of initCFMListener() */

#if 0
int g_keepalive_flag = 0;

#define UINT32 unsigned int
#define CHAR   char
typedef struct {
	UINT32 uiID;
	UINT32 uiIP;           /* PAL_TR69C_SEND_MSG_E */
	UINT32 uiPort;
	UINT32 uiFlag;
	CHAR   acCpeMac[17];
	CHAR   acMsg[64];          /* msg body */
}PAL_TR69C_NAT_MSG;

/*****************************************************************************
 函 数 名  : natUdpListener
 功能描述  : 侦听ACS发送的UDP消息，如果有UDP CONNECTION 请求，则发送inform
 输入参数  : void *handle  
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年7月8日
    作    者   : zhanghaozhong
    修改内容   : 新生成函数

*****************************************************************************/
static void natUdpListener(void *handle)
{
   int      fd = (int)handle;
   PAL_TR69C_NAT_MSG  stNatMSG;
   
   if (g_debug)
       fprintf(stderr, "receive NAT UDP response from ACS\n");  

   g_keepalive_flag = 0; /* 收到UDP响应则清零标志 */
    
   if ( read(fd, (PAL_TR69C_NAT_MSG *)&stNatMSG, sizeof(stNatMSG))>0 ) {
      if (g_debug)
        fprintf(stderr, "natUdpListener called flag=%d, ID=%d, ip=%s port=%d\n", \
        stNatMSG.uiFlag, stNatMSG.uiID, writeIp(stNatMSG.uiIP), stNatMSG.uiPort);
      
        /* BEGIN: Added by zhangjiasi, 2013/12/3   PN:私网telnet功能添加 */        
        switch ( stNatMSG.uiID )
        {
            case TR69C_NAT_MSG:
                printf("I dont !\n");
                if (NAT_UDP_CONNECTION == stNatMSG.uiFlag)
                {
                    sendInform((void*)eIEConnectionRequest);
                    if (g_debug)
                        fprintf(stderr, "send inform to ACS by UDP CONNECTION REQUEST\n");
                }
                break;
            //case TR69C_TELNET_SERVER_CONNECT:
                //tr69c_active_ptelnet_server();
                //break;
            //case TR69C_TELNET_SERVER_DISCONNECT:
                //tr69c_deactive_ptelnet_server();
                //break;
            default:
                slog(TR69C_LOG_ERR, "unknown MSG ID:(%d)",stNatMSG.uiID);
        }

        /* END:   Added by zhangjiasi, 2013/12/3   PN:私网telnet功能添加 */
   }
}  /* End of cfmKickListener() */
extern unsigned int uiInformState;
#if 0
/*****************************************************************************
 函 数 名  : natSendUdp
 功能描述  : 发送UDP报文给ACS做保活，发送IAD的WAN口的IP和该进程的端口
 输入参数  : void *handle  
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年7月8日
    作    者   : zhanghaozhong
    修改内容   : 新生成函数

*****************************************************************************/
static void natSendUdp(void *handle)
{  
    char *acTmp = NULL;
    PAL_TR69C_NAT_MSG  stNatMSG = {0};
    OM_TR69C_NET_FORWARD_IP_T stWanIP = {0};
    int fd = (int)handle;
    static int iAcsOfflineFlag = 0; 
    int iDelayTime = 0;

    if (g_debug)
         fprintf(stderr, "Begin send UDP MSG TO ACS keepalive\n");

    /* BEGIN: Added by zhanghaozhong, 2013/10/16   PN:ACS离线又上线，发生inform，periodic事件 */
    g_keepalive_flag ++;    /* 若超过3次收不到来自UDP的响应，则iAcsOfflineFlag置位 */
    if (g_keepalive_flag > 3)
    {
        if (1 == uiInformState)/* 如果曾经连接ACS成功过,则ACS离线标志置位 */
            iAcsOfflineFlag = 1;
        if (g_debug)
            fprintf(stderr, "iad has losed keepalive msg more than 3 times, ACS IP = %s", writeIp(uiAcsIP));
        if (5 == g_keepalive_flag)
            slog(TR69C_LOG_INFO, "iad has losed keepalive msg more than 3 times, ACS IP = %s", writeIp(uiAcsIP));
         if (g_log_level)
            slog(TR69C_LOG_INFO, "iad has losed keepalive msg more than 3 times, ACS IP = %s", writeIp(uiAcsIP));
    }
    if ((1 == iAcsOfflineFlag) && (1 == g_keepalive_flag))
    {
        slog(TR69C_LOG_INFO, "thanks goodness the keepalive response has been made by ACS again, iad will send inform with PERIODIC");
        iAcsOfflineFlag = 0;
        /* 生成一个随机值(10秒到10分钟之间) */
        iDelayTime = iadRandomTime(10, 600);
        //iDelayTime=5; //测试使用
        setTimer(sendInform, (void *)eIEPeriodix, iDelayTime*1000);
    }
    /* END:   Added by zhanghaozhong, 2013/10/16   PN:ACS离线又上线，发生inform，periodic事件 */
    
    getCpeMac(&acTmp);
    if (NULL != acTmp){
        strncpy(stNatMSG.acCpeMac, acTmp, 17);
        free(acTmp);
    }

    if(OM_E_NONE != getCPEForwardIP(&stWanIP))
        slog(TR69C_LOG_ERR, "oma_net_get_forward_ip ERROR");
    stNatMSG.uiID = TR69C_NAT_MSG;
    stNatMSG.uiIP = readIp(stWanIP.acForwardIp);
    stNatMSG.uiPort = NATUDPCPEPORT;
    stNatMSG.uiFlag = NAT_KEEP_ALIVE;
    
    if (g_debug)
             fprintf(stderr, "stNatMSG.uiID =%x\n stNatMSG.uiFlag=%x\n", stNatMSG.uiID, stNatMSG.uiFlag);    
    if (g_log_level)
         slog(TR69C_LOG_INFO, "stNatMSG.uiID =%x\n stNatMSG.uiFlag=%x\n", stNatMSG.uiID, stNatMSG.uiFlag);

    if (0 != uiAcsIP){
        //printf("************stNatMSG=%d\n", sizeof(stNatMSG));
        if (util_udp_send_remote(fd, NATUDACSPPORT, &stNatMSG, sizeof(stNatMSG), uiAcsIP) < 0)
            slog(TR69C_LOG_ERR, "util_udp_send_remote ERROR!!");
        if (g_debug)
            slog(TR69C_LOG_INFO, "util_udp_send_remote send OK size = %d", sizeof(stNatMSG));
    }
    else{   /* DNS解析在发送inform时做了保护 */
        g_keepalive_flag = 0;
        /* BEGIN: Deleted by zhanghaozhong, 2013/9/3   PN:删除打印 */
        if (g_log_level)
            slog(TR69C_LOG_INFO, "DNS has not resolve yet, can't send UDP msg to ACS");
        /* END:   Deleted by zhanghaozhong, 2013/9/3   PN:删除打印 */
    }
    getX_OUI_NAT_KEEPALIVE_TIME(&acTmp);

    if (NULL != acTmp){
        if (atoi(acTmp) > 2)    //如果从NVRAM中读取的时间间隔大于2秒，则使用
            setTimer(natSendUdp, (void *)handle, atoi(acTmp)*1000); //smart time
        else
            setTimer(natSendUdp, (void *)handle, 10*1000); //nvram中读取不到就默认10s
        free(acTmp);
    }
}  /* End of cfmKickListener() */
#endif
/*****************************************************************************
 函 数 名  : initNatUdpListener
 功能描述  : 为NAT保活和UDP CONNECTION REQ初始化UDP socket
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年7月8日
    作    者   : zhanghaozhong
    修改内容   : 新生成函数

*****************************************************************************/
void initNatUdpListener(void)
{
   int fd;
   
    /* 此socet用于侦听CGI或其他进程发来的UDP报文 */
   if ((fd = udp_listen(INADDR_ANY, NATUDPCPEPORT, NULL, FALSE)) < 0) { /* INADDR_ANY 是IP为0.0.0.0任意IP的意思 */
      slog(TR69C_LOG_ERR, "could not initiate UDP server socket (port=%d)", NATUDPCPEPORT);
      return;
   }
   if (g_debug)
       fprintf(stderr, "initNatUdpListener \n");   
   setListener( fd, natUdpListener, (void *)fd );

   //setTimer(natSendUdp, (void *)fd, 0); //smart time
}  
#endif
