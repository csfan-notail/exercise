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
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <fcntl.h>

#include "appdefs.h"
#include "utils.h"
#include "SOAPParser/CPEframework.h"
#include "event.h"

#include "webproto/protocol.h"
#include "webproto/www.h"
#include "webproto/wget.h"
#include "informer.h"
#include "httpProto.h"
#include "SOAPParser/xmlParserSM.h"
#include "SOAPParser/xmlTables.h"
#include "SOAPParser/RPCState.h"

#include "sysWrapper.h"

#ifdef DMALLOC
#include "dmalloc.h"
#endif

//#define DEBUG
#ifdef DEBUG
#define DBGPRINT(X) fprintf X
#else
#define DBGPRINT(X)
#endif


#include "tr69cdefs.h" /* defines for ACS state */

extern ACSState    acsState;
extern int g_log_level;
extern int protect_connect_state_flag;

static SessionAuth getSessionAuth;
static HttpTask    httpDownload;

static void downloadConnected(void *handle);

void downloadStop(char *msg, int status)
{
    if (httpDownload.wio) {
        wget_Disconnect(httpDownload.wio);
        httpDownload.wio = NULL;
    }
    if ( httpDownload.authHdr != NULL ) {       
        free(httpDownload.authHdr);
        httpDownload.authHdr =NULL;
    }   
    if ( httpDownload.postMsg != NULL ) {
        free(httpDownload.postMsg);
        httpDownload.postMsg = NULL;
        httpDownload.postLth = 0;
    }
    httpDownload.xfrStatus  = eDownloadDone;
    httpDownload.eHttpState = eClosed;
    httpDownload.eAuthState = sIdle;   
    httpDownload.callback = NULL;   

    setACSContactedState( eACS_DOWNLOADREBOOT );
    acsState.fault = 0;
    acsState.dlFaultStatus = status; 
    if ( msg == NULL )
        acsState.dlFaultMsg = "Download successful";                  
    else
        acsState.dlFaultMsg = msg;                  
    acsState.endDLTime = time(NULL);
    saveACSContactedState();
    sendDownloadFault();
}

void updateDownLoadKey(DownloadReq *dlr)
{
	if (acsState.downloadCommandKey)
	{
		free(acsState.downloadCommandKey);
		acsState.downloadCommandKey=NULL;
	}
	//strncpy(acsState.downloadCommandKey, dlr->commandKey, sizeof(acsState.downloadCommandKey));
	acsState.downloadCommandKey= strdup(dlr->commandKey);
	slog(TR69C_LOG_DEBUG,"downloadCommandKey = %s\n",dlr->commandKey);
	dlr->commandKey = NULL;
}
/* 从ACS侧下载文件，支持版本下载和配置文件下载 */
static void downloadGetData(void *handle)
{
    printf("*******************\n");
    printf("downloadGetData\n");
    printf("*******************\n");
    //sleep(100);// TCP Window Full
	int status = 9010;
    char *msg = "Download failed";
    tWget *wg = (tWget *)handle;
    HttpTask *ht = &httpDownload;
    RPCAction *action = (RPCAction *)ht->callback;
    DownloadReq *dlreq = &(action->ud.downloadReq);
/*
	int  a =0 ;
   a = syslog(TR69C_LOG_DEBUG, "downloadGetData start!!!\n");
   printf("aaa =  %d\n",a);
*/
slog(TR69C_LOG_DEBUG, "downloadGetData start!!!\n");
slog(TR69C_LOG_DEBUG, "Status =  %d %s\n",wg->status,wg->msg);
	

//mBUG("downloadGetData start!!!\n");
//mBUG("downloadGetData start Status =  %d %s \n",wg->status,wg->msg);

   if (wg->status == iWgetStatus_Ok) 
   {
		
		slog(TR69C_LOG_DEBUG, "downloadGetData start    ee!!!\n");
	 	printf("downloadGetData  0\n");	
        if (wg->hdrs->status_code == 200 && 
            ((wg->hdrs->content_length > 0) ||
             (wg->hdrs->TransferEncoding && streq(wg->hdrs->TransferEncoding,"chunked")))) 
        {
		            //oma_tr69c_kill_progress();  /* 下载之前将占用内存的进程杀掉zhanghaozhong */
					slog(TR69C_LOG_DEBUG, "downloadGetData start    0\n");
					 printf("downloadGetData  1\n");
		            /* readin download image */
		            int  mlth = 0;
					
		            char *rambuf = readResponse(wg, &mlth, getRAMSize()); //将CMS发来的数据读取到 rambuf 中
		            if (rambuf && mlth) 
					{
			                dlreq->fileSize = mlth;
			                //sleep(100);
			           		/* add by duanguiyuan 20160324 计算下载 结束的时间  */
							acsState.endDLTime = time(NULL);
							printf("acsState.endDLTime = %ld \n",acsState.endDLTime);
							/**/
			       			printf("downloadGetData  2\n");          
			                downloadComplete(dlreq, rambuf);
							slog(TR69C_LOG_DEBUG, "downloadGetData start    end\n");

		            }
		            /* only hit the following codes when error occurs during download
		              so free(rambuf) and reboot to make system recover since
		              bcmTr69KillAllApps is already called */
		            if (rambuf != NULL) 
					{
			                free(rambuf);
							rambuf=NULL;
		            }
		            slog(TR69C_LOG_ERR, "Download from %s failed", dlreq->url);

        } 
		else if (wg->hdrs->status_code == 401)
		{
		            slog(TR69C_LOG_ERR, "File transfer server authentication failure from %s failed. Status = %d\n", dlreq->url, wg->hdrs->status_code);
		            downloadStop("File transfer server authentication failure", 9012);
        } 
		else if (wg->hdrs->status_code == 404) 
		{
		            slog(TR69C_LOG_ERR, "Download file cannot be found from %s failed. Status = %d\n", dlreq->url, wg->hdrs->status_code);
		            downloadStop("Download file cannot be found", 9010);
        } 
		else if (wg->hdrs->status_code >= 100 && wg->hdrs->status_code < 200) 
		{
            		return; /* ignore these status codes */
        } 
		else 
		{
		            slog(TR69C_LOG_ERR, "Download from %s failed. Status = %d\n", dlreq->url, wg->hdrs->status_code);
		            downloadStop(msg, status);
        }

    } 
  else 
  {
        /* if control falls thru to here send a fault status to ACS */


		//(TR69C_LOG_DEBUG, "Download from %s failed. Status = %d %s\n", dlreq->url, wg->status, wg->msg);
	 printf("downloadGetData  00  Status = %d  \n",wg->status);	
		
	 printf("Download from %s failed. Status = %d %s\n", dlreq->url, wg->status, wg->msg);		
		 
        //slog(TR69C_LOG_ERR, "Download from %s failed. Status =   %s\n", dlreq->url,wg->msg);
		
        downloadStop("Unable to connect to download server", 9010);
  }

}

static void downloadGetAuth(void *handle)
{
    int status = 9010;
    char *msg = "Unable to connect to download URL";
    tWget *wg = (tWget *)handle;
    HttpTask *ht = &httpDownload;
    RPCAction *action = (RPCAction *)ht->callback;
    DownloadReq *dlreq = &(action->ud.downloadReq);
    SessionAuth *sa = &getSessionAuth;
    printf("downloadGetAuth\n");
    if (g_debug)
    fprintf(stderr, "downloadGetAuth(): starting authentication\n");
	
 printf("downloadGetAuth...    0\n");    
  printf("downloadGetAuth  wg->status = %d\n",wg->status);  
    if (wg->status == iWgetStatus_Ok) {
	printf("downloadGetAuth...    1\n");
        if (wg->hdrs->status_code == 401) {
            int closeConn = 0;
            if (wg->hdrs->content_length > 0) {
                int mlth = 0;
                char *tmpBuf = readResponse(wg, &mlth, 0);
                if (tmpBuf != NULL)
                {
                 	free(tmpBuf);
					tmpBuf=NULL;
				}
                    
                proto_Skip(wg->pc);
            }
            ht->authHdr = generateAuthorizationHdrValue(sa, wg->hdrs->wwwAuthenticate,
                                                       "GET", ht->wio->uri, dlreq->user, dlreq->pwd);
            if (ht->authHdr == NULL) {
                downloadStop("Download server has unknow authentication header format", status);
                return;
            }

            closeConn = ht->wio->hdrs->Connection && !strcasecmp(ht->wio->hdrs->Connection,"close");
            if (closeConn) { /* end of data on 401 skip_Proto() */
                /* close connection and reconnect with Authorization header*/
                wget_Disconnect(ht->wio);
                ht->wio = wget_Connect(dlreq->url, downloadConnected, action);
                if (ht->wio == NULL) {
                    downloadStop("Failed to connect download server", status);
                } else {
                    wget_AddPostHdr(ht->wio,"Authorization", ht->authHdr);
                }
            } else {
                #if 0
                 wget_AddPostHdr(ht->wio,"Authorization", ht->authHdr);
                // now just resend the last data with the Authorization header
                // HttpTask has to be the last argument in wget_GetData
                // since it is used later in do_send_request
				#else
				/* Begin : add by duanguiyuan  20161125 修复摘要认证失败问题 */
				if (ht->authHdr){
      				if ( sa->qopType==eAuth ) {
         					free(ht->authHdr);
		 					ht->authHdr=NULL;
         					ht->authHdr = generateNextAuthorizationHdrValue(sa,dlreq->user, dlreq->pwd);
         					wget_AddPostHdr(ht->wio,"Authorization", ht->authHdr);    /* replace header */
      				} else {
         					/* must be basic or not "Auth" */
         					wget_AddPostHdr(ht->wio,"Authorization", ht->authHdr);    /* replace header */
      				}
   				}
   				/* Begin : add by duanguiyuan  20161125 修复摘要认证失败问题 */
				#endif
  printf("downloadGetAuth...    1 wg->status= %d  \n",wg->status);

				printf("downloadGetAuth...    1 wg->status= %d  \n",wg->status);
                wget_GetData(ht->wio, downloadGetData, (void *)ht);
				printf("downloadGetAuth...    1 wg->status= %d  \n",wg->status);
            }
        } else if (wg->hdrs->status_code == 404) {
            downloadStop("Download file cannot be found", 9010);
        } else if (wg->hdrs->status_code >= 100 && wg->hdrs->status_code < 200) {
            return; /* ignore these status codes */
        } else {
            downloadStop("Download server authentication failure", 9012);
        }
    } else {
printf("downloadGetAuth...    2\n");
        downloadStop(msg, status);
    }
}

static int downloadIsAuthNeeded(RPCAction *action)
{
    int ret = TRUE;
    HttpTask *ht = &httpDownload;
    
    if (action->ud.downloadReq.user == NULL)
        ret = FALSE;
    else if (action->ud.downloadReq.user[0] == '\0')
        ret = FALSE;
    else if (action->ud.downloadReq.pwd == NULL)
        ret = FALSE;
    else if (action->ud.downloadReq.pwd[0] == '\0')
        ret = FALSE;

    // authHdr already has value by downloadGetAuth
    // so don't need to perform authentication again        
    if (ht->authHdr != NULL)
        ret = FALSE;

    return ret;
}

static void downloadConnected(void *handle)   
	
{
    printf("downloadConnected\n");
    tWget *wg = (tWget *)handle;
    RPCAction *action = (RPCAction *)wg->handle;
    HttpTask *ht = &httpDownload;
/*
	printf("downloadConnected 0: wg->status = %d\n",wg->status);
	printf("downloadConnected 0: wg->msg = %s\n",wg->msg);
	printf("downloadConnected 0: wg->hdrs->content_type = %s\n",wg->hdrs->content_type);
//	printf("downloadConnected 0: wg->status = %d\n",wg->status);
*/	
    if (wg->status != 0) {
        downloadStop("Unable to connect to download server", 9010);       
        return;      
    }
 
    // stick RPCAction to callback
    ht->callback = (CallBack)action;

	printf("downloadConnected 1: wg->status = %d\n",wg->status);
    
    // HttpTask has to be the last argument in wget_GetData
    // since it is used later in do_send_request
    
slog(TR69C_LOG_DEBUG, "downloadConnected.\n");
	
    if (downloadIsAuthNeeded(action) == FALSE) {
        printf("FALSE!\n");
        //sleep(100);
        wget_GetData(ht->wio, downloadGetData,(void *)ht);
    } else {
        wget_GetData(ht->wio, downloadGetAuth,(void *)ht);
    }
	printf("downloadConnected end: wg->status = %d\n",wg->status);	
}

/* this is called by the callback from the startTimer in doDownload. */
/* we have to use wget_Connect in case of authentication in which case */
/* we need to control the connection */
void downloadStart( void *handle) {
    
    printf("downloadStart\n");
slog(TR69C_LOG_DEBUG, "start download...    0\n"); 
    RPCAction   *action = (RPCAction *)handle;
    HttpTask    *ht = &httpDownload;
    DownloadReq *dlreq = &action->ud.downloadReq;
	
slog(TR69C_LOG_DEBUG, "start download...    0\n"); 

    if (strstr(dlreq->url, "http:") == NULL &&
        strstr(dlreq->url, "https:") == NULL) {
        downloadStop("Protocol not supported", 9013);
        return;
    }
	
slog(TR69C_LOG_DEBUG, "start download...    1\n");  

    
    if (dlreq->efileType != eFirmwareUpgrade &&
        dlreq->efileType != eVendorConfig) {
        downloadStop("Invalid download file type", 9010);       
        return;
    }
	
 slog(TR69C_LOG_DEBUG, "start download...    2\n");   
 
    memset(ht, 0, sizeof(struct HttpTask));
    memset(&getSessionAuth,0, sizeof(struct SessionAuth));
    // update download command key to fix CSP# 41725
    printf("commandKey = %s !!!!\n",dlreq->commandKey);
    updateDownLoadKey(dlreq);
	
slog(TR69C_LOG_DEBUG, "start download...    3\n");    
	/* add by duanguiyuan 20160324 开始计算下载时间*/
	acsState.startDLTime = time(NULL);
	printf("acsState.startDLTime = %ld \n",acsState.startDLTime);
	/*add by duanguiyuan 20160324 */
    ht->wio = wget_Connect(dlreq->url, downloadConnected, action);
/*
	printf("ht->wio->status= %d\n ",ht->wio->status);
	printf("ht->wio->keepConnection= %d\n ",ht->wio->keepConnection);
	printf("ht->wio->request= %d\n ",ht->wio->request);	
	printf("ht->wio->content_type= %s\n ",ht->wio->content_type);	

	printf("ht->wio->proto= %s\n ",ht->wio->proto);
	printf("ht->wio->host= %s\n ",ht->wio->host);
//	printf("ht->wio->content_type= %s\n ",ht->wio->content_type);
//	printf("ht->wio->content_type= %s\n ",ht->wio->content_type);
*/	
    if (ht->wio == NULL){
slog(TR69C_LOG_ERR, "wget_Connect to download server failed: %s\n", wget_LastErrorMsg());
        downloadStop("Connect to download server failed", 9010);   
    } else {
		slog(TR69C_LOG_DEBUG, "wget_Connect to download  NO  failed \n");  
        wget_ClearPostHdrs(ht->wio);
        if (ht->authHdr){
            free(ht->authHdr);
            ht->authHdr = NULL;
        }
    }
slog(TR69C_LOG_DEBUG, "start download...    4\n");  
    //sleep(100);
}

