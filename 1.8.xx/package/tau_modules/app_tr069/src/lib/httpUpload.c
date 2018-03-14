/*
 * Copyright (C) GBCOM ltd.,co 2015
 *
 * This is a part of project tr69cd, which implements TR069 protocal(aka
 * CWMP).
 * 
 * This is a PRIVATE software.
 * All rights reserved.
 */
 
//#ifdef RPC_UPLOAD    
 
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
/*
#include "syscall.h"
#include "ifcdefs.h"
#include "bcmcfm.h"
#include "board_api.h"
#include "systemlog.h"
*/
#include "appdefs.h"
#include "utils.h"
#include "SOAPParser/CPEframework.h"
#include "SOAPParser/RPCState.h"
#include "event.h"

#include "webproto/protocol.h"
#include "webproto/www.h"
#include "webproto/wget.h"
#include "httpProto.h"
#include "sysWrapper.h"
//#include "main/informer.h"
//#include "main/httpProto.h"
#include "informer.h"
#include "httpDownload.h"
#include "SOAPParser/xmlParserSM.h"
#include "SOAPParser/xmlTables.h"
#include "SOAPParser/RPCState.h"
#include "tr69cdefs.h" /* defines for ACS state */
//#include "standard/tr69cwrapper.h"

#ifdef DMALLOC
#include "dmalloc.h"
#endif
/*
#ifdef GBCOM
#include "pal_data.h"

#endif
*/
#include "om_types.h"

#define XML_PSI_END_TAG       "</psitree>"
#define UPLOAD_BUFFER_SIZE    512

/*add by duanguiyuan 20150901  */
#define TR69C_PERFORMANCE_FILE   "/tmp/pal/sys/performance"
#define LOG_PACK 	"/tmp/log/syslog.log.tmp"
//#define LOG_PACK 	"/tmp/log/log.tar.bz2"
#define CONFIG_PACK  "/etc/config/tr069"          // "/etc/default_config"

//#define LOG_PACK 	"/var/msg.txt"

//#define DEBUG
/*
if (g_debug)
#define DBGPRINT(X) fprintf X
#else
#define DBGPRINT(X)
#endif
*/
extern ACSState     acsState;
extern int g_log_level;
extern int protect_connect_state_flag;

static SessionAuth putSessionAuth;
static HttpTask httpUpload;

static void uploadConnected(void *handle);
static void uploadPutData(RPCAction *action);

static void uploadStop(char *msg, int status)
{ 
	if (g_debug)            
            fprintf(stderr, "upload stop msg=%s status = %d \n",msg,status);
    if (httpUpload.wio){
        wget_Disconnect(httpUpload.wio);
        httpUpload.wio = NULL;
    }
    if ( httpUpload.authHdr != NULL ) {       
        free(httpUpload.authHdr);
        httpUpload.authHdr =NULL;
    }   
    if ( httpUpload.postMsg != NULL ) {
        free(httpUpload.postMsg);
        httpUpload.postMsg = NULL;
        httpUpload.postLth = 0;
    }
    httpUpload.xfrStatus  = eUploadDone;
    httpUpload.eHttpState = eClosed;
    httpUpload.eAuthState = sIdle;   
    httpUpload.callback = NULL;   

    setACSContactedState( eACS_UPLOAD );
    acsState.fault = 0;
    acsState.dlFaultStatus = status; 
    if ( msg == NULL )
        acsState.dlFaultMsg = "Upload successful";                  
    else
        acsState.dlFaultMsg = msg;                  
    acsState.endDLTime = time(NULL);
    saveACSContactedState();
    sendDownloadFault();
}

static void uploadCompletePut(void *handle)
{
    int status = 9011;
    char *msg = "Upload failed";
    tWget *wg = (tWget *)handle;

    if (wg->status == iWgetStatus_Ok) {
		printf("upload response code = %d \n",wg->hdrs->status_code);
        switch (wg->hdrs->status_code) {
        case 401:
            status = 9012;
            msg = "File transfer server authentication failure";
            break;
        case 405:
			status = 9012;
            msg = "File transfer server authentication failure 405";
            break;
        case 501:
            status = 9011;
            msg = "Upload server does not support PUT method";
            break;
        case 100:   // Continue status might be returned by Microsoft-IIS/5.1
        case 201:   // Created status is returned by Microsoft-IIS/5.1
        case 204:   // No content status is returned by Apache/2.2.2
        case 200:
            status = 0;
            msg = "Upload successful";
            break;
        default:
            status = 9011;
          	printf("upload response code = %d \n",wg->hdrs->status_code);
            msg = "Upload failed";
			
            break;
        }
    }
    
    uploadStop(msg, status);
}

static void uploadCompleteAuth(void *handle)
{
    int status = 9011;
    char *msg = "Unable to connect to upload URL";
    tWget *wg = (tWget *)handle;
    HttpTask *ht = &httpUpload;
    RPCAction *action = (RPCAction *)ht->callback;
    DownloadReq *ulreq = &(action->ud.downloadReq);
    SessionAuth *sa = &putSessionAuth;
    
    if (wg->status == iWgetStatus_Ok) {
        if (wg->hdrs->status_code == 401) {
            int closeConn = 0;
            if (wg->hdrs->content_length > 0) {
                int mlth = 0;
                char *tmpBuf = readResponse(wg,&mlth,0);
                if (tmpBuf != NULL)
                {
						free(tmpBuf);
						tmpBuf=NULL;
             	}
                    
                proto_Skip(wg->pc);
            }
			/* change by duanguiyuan  修改上传配置文件的 方法 为  post */
#if 0
            ht->authHdr = generateAuthorizationHdrValue(sa, wg->hdrs->wwwAuthenticate,
                                                       "PUT", ht->wio->uri, ulreq->user, ulreq->pwd);
	#else
			 ht->authHdr = generateAuthorizationHdrValue(sa, wg->hdrs->wwwAuthenticate,
                                                       "POST", ht->wio->uri, ulreq->user, ulreq->pwd);	
#endif 
            if (ht->authHdr == NULL) {
                uploadStop("Upload server has unknow authentication header format", status);
                return;
            }

            closeConn = ht->wio->hdrs->Connection && !strcasecmp(ht->wio->hdrs->Connection,"close");
            if (closeConn) { /* end of data on 401 skip_Proto() */
                /* close connection and reconnect with Authorization header*/
                wget_Disconnect(ht->wio);
                ht->wio = wget_Connect(ulreq->url, uploadConnected, action);
                if (ht->wio == NULL) {
                    uploadStop("Failed to connect upload server", status);
                } else {
                    wget_AddPostHdr(ht->wio,"Authorization", ht->authHdr);
                }
            } else {
				#if 0
                wget_AddPostHdr(ht->wio,"Authorization", ht->authHdr);
                /* now just resend the last data with the Authorization header */
				#else
				/* Begin : add by duanguiyuan  20161125 修复摘要认证失败问题 */
				if (ht->authHdr){
      				if ( sa->qopType==eAuth ) {
         					free(ht->authHdr);
		 					ht->authHdr=NULL;
         					ht->authHdr = generateNextAuthorizationHdrValue(sa,ulreq->user, ulreq->pwd);
         					wget_AddPostHdr(ht->wio,"Authorization", ht->authHdr);    /* replace header */
      				} else {
         					/* must be basic or not "Auth" */
         					wget_AddPostHdr(ht->wio,"Authorization", ht->authHdr);    /* replace header */
      				}
   				}
   				/* Begin : add by duanguiyuan  20161125 修复摘要认证失败问题 */
				#endif
				//printf(" ..............   nc  = %d \n      33",sa->nonceCnt);
                uploadPutData(action);
            }
        } else {
        	//printf("upload connect state = %d  msg = (%s) \n",wg->hdrs->status_code,wg->hdrs->message);
            uploadStop("Upload server authentication failure", 9012);
        }
    } else {
        uploadStop(msg, status);
    }
}

static void uploadPutPerformance(void)
{
    FILE *pFile;
    char data[1024];
    char *filename = TR69C_PERFORMANCE_FILE;

    HttpTask *ht = &httpUpload;
    tProtoCtx *pc = ht->wio->pc;
    if (NULL != (pFile = fopen(filename, "rb")))
    {
        while (fgets(data, sizeof(data), pFile) != NULL )
        {     
            proto_SendRaw(pc, data, strlen(data));
        }
        fclose(pFile);
        slog(TR69C_LOG_INFO, "open flie %s OK\n",filename);
    }
    else
        slog(TR69C_LOG_ERR, "open flie %s error\n",filename);
}

/* 上传LOG日志到ACS */
static void uploadPutLog(void)
{
slog(TR69C_LOG_DEBUG, "uploadPutLog   0 !!!"); 

#ifdef GBCOM
    //int readptr = BCM_SYSLOG_FIRST_READ;
    //char data[BCM_SYSLOG_MAX_LINE_SIZE];
    FILE *pFile;
    char data[1024];
	char buf[1024];
//	char boundary[256] ={0};
    int datalen = 0;
    int datalenTotal = 0;

    HttpTask *ht = &httpUpload;
    tProtoCtx *pc = ht->wio->pc;

slog(TR69C_LOG_DEBUG, "uploadPutLog   1 !!!"); 
	
    /* BEGIN: Added by zhanghaozhong, 2013/10/21   PN:BUG号：BUG #4178支持通过ACS下载指定某台IAD的日志，包括两个message文件、bb、通话记录 */
    if (NULL != (pFile = fopen(LOG_PACK, "rb")))
    /* END:   Added by zhanghaozhong, 2013/10/21   PN:BUG号：BUG #4178支持通过ACS下载指定某台IAD的日志，包括两个message文件、bb、通话记录 */
    {
        //while (fgets(data, sizeof(data), pFile) != NULL )
        while ((datalen = fread(data, 1, sizeof(data), pFile)) > 0)
        {  
 slog(TR69C_LOG_DEBUG, "proto_SendRaw %d  !!!",datalenTotal); 

            proto_SendRaw(pc, data, datalen);
            datalenTotal += datalen;
        }
        fclose(pFile);
/* add bu duanguiyuan 20150915 */
 	memset(buf, 0, sizeof(buf));  /* memset */
 	sprintf(buf, "\r\n--%s--\r\n", HTTP_PUT_BOUNDARY_KEY);
 	proto_SendRaw(pc,buf,strlen(buf));

/* add bu duanguiyuan */
        if (g_debug)
       slog(TR69C_LOG_INFO, "uploadPutLog() size=%d\n", datalenTotal);
    }
    else
        slog(TR69C_LOG_ERR, "open flie %s error\n",LOG_PACK);
#else
slog(TR69C_LOG_DEBUG, "uploadPutLog   2 !!!"); 

    int readptr = BCM_SYSLOG_FIRST_READ;
    char data[BCM_SYSLOG_MAX_LINE_SIZE];

    HttpTask *ht = &httpUpload;
    tProtoCtx *pc = ht->wio->pc;

    // try reading circular buffer
    while ((readptr != BCM_SYSLOG_READ_BUFFER_ERROR) &&
           (readptr != BCM_SYSLOG_READ_BUFFER_END)) {
        readptr = bcmLogreadPartial(readptr,data);
        proto_SendRaw(pc, data, strlen(data));
    }
#endif
}

static void uploadPutConfig(void)
{
#ifdef GBCOM
    FILE *pFile;
    char data[1024];
	char buf[1024];

 //   char *filename = "/tmp/pal/sys/iad_config";

    HttpTask *ht = &httpUpload;
    tProtoCtx *pc = ht->wio->pc;
    if (NULL != (pFile = fopen(CONFIG_PACK, "rb")))
    {
        while (fgets(data, sizeof(data), pFile) != NULL )
        {     
            proto_SendRaw(pc, data, strlen(data));
        }
        fclose(pFile);

/* add bu duanguiyuan 20150915 */
 	memset(buf, 0, sizeof(buf));  /* memset */
 	sprintf(buf, "\r\n--%s--\r\n", HTTP_PUT_BOUNDARY_KEY);
 	proto_SendRaw(pc,buf,strlen(buf));

/* add bu duanguiyuan */
		
        slog(TR69C_LOG_INFO, "open flie %s OK\n",CONFIG_PACK);
    }
    else
        slog(TR69C_LOG_ERR, "open flie %s error\n",CONFIG_PACK);
#else

    int cnt = 0, len = 0;
    char data[UPLOAD_BUFFER_SIZE];
    HttpTask *ht = &httpUpload;
    tProtoCtx *pc = ht->wio->pc;

    while (cnt < ht->content_len) {
        memset(data, 0, UPLOAD_BUFFER_SIZE);    
        if (cnt + UPLOAD_BUFFER_SIZE < ht->content_len)
            len = UPLOAD_BUFFER_SIZE;
        else
            len = ht->content_len - cnt;
        //sysPersistentGet(data, len, cnt); /* del by zhanghaozhong BCM库函数 */
        proto_SendRaw(pc, data, len);
        cnt += len;
    }
#endif
}

/*****************************************************************************
 函 数 名  : uploadGetperformanceSize
 功能描述  : 获取性能文件的大小
 输入参数  : void  
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年6月6日
    作    者   : zhanghaozhong
    修改内容   : 新生成函数

*****************************************************************************/
static UINT32 uploadGetperformanceSize(void) 
{
    FILE *ipF;
    int fLen;
    char *filename = TR69C_PERFORMANCE_FILE;

    if (NULL == (ipF = fopen(filename , "r+")))
    {
        slog(TR69C_LOG_ERR, "open file %s error\n",filename);
        return -1;
    }

    fseek(ipF, 0, SEEK_END);
    fLen = ftell(ipF);
    fclose(ipF);
    if (g_debug)
        fprintf(stderr, "/tmp/pal/sys/performance lenth=%d\n", fLen);
    if(fLen < 2) //如果文件为空
        return 0;
    return fLen;
}

/* modify by zhanghaozhong */
static UINT32 uploadGetLogSize(void) 
{
#ifdef GBCOM
    FILE *ipF;  
    int fLen;

    /* BEGIN: Added by zhanghaozhong, 2013/10/21   PN:BUG号：BUG #4178支持通过ACS下载指定某台IAD的日志，包括两个message文件、bb、通话记录 */
    //if (NULL == (ipF = fopen(TR69C_UPLOAD_LOG_FILE , "r+")))
    system("cp -rf /tmp/log/syslog.log "LOG_PACK);
	/* 华为 定制的 日志文件 log.tar.bz2 */
	//system("cd /tmp/log/ && tar -cf "LOG_PACK" syslog.log.tmp && cd ../../");
	
    if (NULL == (ipF = fopen(LOG_PACK , "rb+")))
    {
        slog(TR69C_LOG_ERR, "open file %s error\n",LOG_PACK);
        return -1;
    }
    /* END:   Added by zhanghaozhong, 2013/10/21   PN:BUG号：BUG #4178支持通过ACS下载指定某台IAD的日志，包括两个message文件、bb、通话记录 */

    fseek(ipF, 0, SEEK_END);
    fLen = ftell(ipF);
    fclose(ipF);
    if (g_debug)
        fprintf(stderr, ""LOG_PACK" lenth=%d\n", fLen);
    if(fLen < 2) //如果文件为空
        return 0;
    return fLen;
#else

    UINT32 cnt = 0;
    int readptr = BCM_SYSLOG_FIRST_READ;
    char buffer[BCM_SYSLOG_MAX_LINE_SIZE];

    // try reading circular buffer
    while ((readptr != BCM_SYSLOG_READ_BUFFER_ERROR) &&
           (readptr != BCM_SYSLOG_READ_BUFFER_END)) {
        readptr = bcmLogreadPartial(readptr,buffer);
        cnt += strlen(buffer);
    }

    return cnt;
#endif
}

static UINT32 uploadGetConfigSize(void) 
{
#ifdef GBCOM
    FILE *ipF;  
    int fLen;
/*
	char *filename = "/tmp/pal/sys/iad_config";

    if (0 != system("nvramcli show > /tmp/pal/sys/iad_config"))
        slog(TR69C_LOG_ERR, "tr69c: create iad_config ERROR\n");
*/
   // if (0 != system("nvram show|grep = > /etc/default_config"))
    if (0 != system("cat /etc/config/lte |grep option > /tmp/config"))
    	slog(TR69C_LOG_ERR, "tr69c: create default_config ERROR\n");

    if (NULL == (ipF = fopen(CONFIG_PACK , "r+")))
    {
        slog(TR69C_LOG_ERR, "open file %s error\n",CONFIG_PACK);
        return -1;
    }

    fseek(ipF, 0, SEEK_END);
    fLen = ftell(ipF);
    fclose(ipF);
if (g_debug)
    fprintf(stderr, "/tmp/default_config lenth=%d\n", fLen);
    if(fLen < 2) //如果文件为空
        return 0;
    return fLen;

#else
    UINT32 cnt = 0;
    UINT32 size = sysGetPsiSize();
    char data[UPLOAD_BUFFER_SIZE+1];
    char *pc = NULL;
    
    do {
        memset(data, '\0', UPLOAD_BUFFER_SIZE+1);    
        sysPersistentGet(data, UPLOAD_BUFFER_SIZE, cnt);
        pc = strstr(data, XML_PSI_END_TAG);
        if ( pc == NULL )
            cnt += UPLOAD_BUFFER_SIZE;
        else 
            cnt += strlen(data);
    } while ( pc == NULL && cnt < size );

    return cnt;
#endif
}

static void uploadPutData(RPCAction *action)
{
    UINT32 size = 0;
    HttpTask *ht = &httpUpload;


    if (ht->wio == NULL) {
        uploadStop("Unable to connect to upload server", 9011);       
        return;
    }

    
    if (ht->wio->pc == NULL) {
        uploadStop("Unable to connect to upload server", 9011);       
        return;
    }
            
    // Get the body data length
    switch (action->ud.downloadReq.efileType) {
        case eVendorLog:            
            /* BEGIN: Added by zhanghaozhong, 2013/10/21   PN:BUG号：BUG #4178支持通过ACS下载指定某台IAD的日志，包括两个message文件、bb、通话记录 */
          /*  if (OM_E_NONE != om_generate_uploadlog())*/

		/*注释 by duanguiyuan 20150901 
			if (OM_E_NONE != oma_tr69c_gen_logs2ACS())                
                slog(TR69C_LOG_ERR, "om_generate_uploadlog error\n");   
		*/	
            size = uploadGetLogSize();  /* 获取上传log的大小 */
            if (g_debug)
                fprintf(stderr, "uploadGetLogSize() size=%d\n", size);
            break;
            /* END:   Added by zhanghaozhong, 2013/10/21   PN:BUG号：BUG #4178支持通过ACS下载指定某台IAD的日志，包括两个message文件、bb、通话记录 */
        case eVendorPerformance:
            /* 调用OMA接口生成性能文件/tmp/pal/sys/performance */
		/*注释 by duanguiyuan 20150901 

			if (OM_E_NONE != oma_tr69c_performance_file_create())
                slog(TR69C_LOG_ERR, "oma_tr69c_performance_file_create error\n");  
		*/	
            size = uploadGetperformanceSize();  /* 获取上传log的大小 */
            if (g_debug)
                fprintf(stderr, "uploadGetLogSize() is exed\n");
            break;
        default:
            size = uploadGetConfigSize();  /* 获取上传配置文件的大小 */
            if (g_debug)            
            fprintf(stderr, "uploadGetConfigSize() is exed\n");
            break;
    }
	
slog(TR69C_LOG_DEBUG, "uploadPutData   size = %d!!!",size);
    if (size > 0) {
        // Send the PUT header with content length
        ht->content_len = (int)size;
        ht->postMsg     = NULL;
        ht->postLth     = (int)size;
        ht->callback    = (CallBack)action; // stick RPCAction to callback
slog(TR69C_LOG_DEBUG, "uploadPutData   ht->content_len = %d!!!",ht->content_len);

        // HttpTask has to be the last argument in wget_PutData
        // since it is used later in do_send_request
        wget_PutData(ht->wio, ht->postMsg, ht->postLth, "text/xml",
                     uploadCompletePut, (void *)ht);
		
slog(TR69C_LOG_DEBUG, "uploadPutData   4!!!"); 

        if (g_debug)            
            fprintf(stderr, "efileType=%d\n",action->ud.downloadReq.efileType);

		// Send the body data
        switch (action->ud.downloadReq.efileType) {
        case eVendorLog:
         uploadPutLog();  /* 上传log到ACS */

		/*注释 by duanguiyuan 20150901 */

		//	 oma_tr69c_rm_logs2ACS();    /* 删除log相关文件 */
		
            if (g_debug)
                fprintf(stderr, "uploadPutLog() is exed\n");
            break;
        case eVendorPerformance:
            uploadPutPerformance();  /* 上传性能文件到ACS */
            if (g_debug)
            fprintf(stderr, "uploadPutPerformance() is exed\n");
            break;
        default:
            uploadPutConfig();  /* 上传配置文件到ACS */  
            if (g_debug)            
            fprintf(stderr, "uploadPutConfig() is exed\n");
            break;
        }
		/*add by duanguiyuan 20160325 计算上传文件的时间*/
		 acsState.endDLTime = time(NULL);
   	 	saveACSContactedState();
		/*add by duanguiyuan 20160325 计算上传文件的时间*/
    } else {
        uploadStop("No upload data", 9011);
    }
slog(TR69C_LOG_DEBUG, "uploadPutData   end!!!"); 
}

static void uploadPutAuth(RPCAction *action)  
{
    int size = 2;
    char *msg = (char *)malloc(size);
    HttpTask *ht = &httpUpload;
    
    if ( ht->wio == NULL ) {
        uploadStop("Unable to connect to upload server", 9011);       
        return;
    }
    
    memset(msg, '\0', size);
    msg[0] = '\n';    
    ht->content_len = size;
    ht->postMsg     = msg;
    ht->postLth     = size;
    ht->callback    = (CallBack)action; // stick RPCAction to callback

    // Send the newline file to test for authentication method
    // HttpTask has to be the last argument in wget_PutData
    // since it is used later in do_send_request
    /*
    	changed by duanguiyuan 20161122 
		上传日志 采用 post 方式 上传 
	*/
#if 0
    wget_PostData(ht->wio, ht->postMsg, ht->postLth, "text/xml",
                 uploadCompleteAuth, (void *)ht);
#else
    wget_PutData(ht->wio, ht->postMsg, ht->postLth, "text/xml",
                 uploadCompleteAuth, (void *)ht);
#endif

}

static int uploadIsAuthNeeded(RPCAction *action)
{
    int ret = TRUE;
    HttpTask *ht = &httpUpload;
    
    if (action->ud.downloadReq.user == NULL)
        ret = FALSE;
    else if (action->ud.downloadReq.user[0] == '\0')
        ret = FALSE;
    else if (action->ud.downloadReq.pwd == NULL)
        ret = FALSE;
    else if (action->ud.downloadReq.pwd[0] == '\0')
        ret = FALSE;

    // authHdr already has value by uploadPutAuth
    // so don't need to perform authentication again        
    if (ht->authHdr != NULL)
        ret = FALSE;

    return ret;
}

static void uploadConnected(void *handle) 
{
    tWget *wg = (tWget *)handle;
    RPCAction *action = (RPCAction *)wg->handle;
    if (g_debug)            
            fprintf(stderr, "uploadConnected efileType=%d\n",action->ud.downloadReq.efileType);

slog(TR69C_LOG_DEBUG, "Upload Connected 0!!!");
slog(TR69C_LOG_DEBUG, "Upload Connected wg->status = %d !!!",wg->status);
	if (g_debug) 
		printf("upload info >>>>>>>>>>> user =%s pass = %s url = %s \n",action->ud.downloadReq.user,action->ud.downloadReq.pwd,action->ud.downloadReq.url);
    if (wg->status != 0) {
        uploadStop("Unable to connect to upload server", 9011);       
        return;      
    }
    
    if (uploadIsAuthNeeded(action) == FALSE) {   
slog(TR69C_LOG_DEBUG, "Upload Connected no auth!!!");
        uploadPutData(action);
    } else {
slog(TR69C_LOG_DEBUG, "Upload Connected need auth!!!");

        uploadPutAuth(action);
    }
}

/* this is called by the callback from the startTimer in doUpload. */
/* we have to use wget_Connect in case of authentication in which case */
/* we need to control the connection */
void uploadStart( void *handle) 
{
    RPCAction   *action = (RPCAction *)handle;
    HttpTask    *ht = &httpUpload;
    DownloadReq *ulreq = &action->ud.downloadReq;
	
	
    if (strstr(ulreq->url, "http:") == NULL &&
        strstr(ulreq->url, "https:") == NULL) {
        uploadStop("Protocol not supported", 9013);
        return;
    }
    slog(TR69C_LOG_DEBUG, "Upload type = %d ",ulreq->efileType);
    if (ulreq->efileType != eVendorConfig &&
        ulreq->efileType != eVendorLog &&
        ulreq->efileType != eVendorPerformance) {
        uploadStop("Invalid upload file type", 9011);       
        return;
    }
    
    memset(ht, 0, sizeof(struct HttpTask));
    memset(&putSessionAuth,0, sizeof(struct SessionAuth));
    updateDownLoadKey(ulreq);

	slog(TR69C_LOG_DEBUG, "Upload Start  1!!!");
  	acsState.startDLTime = time(NULL);
	printf("acsState.startDLTime = %ld \n",acsState.startDLTime);

	ht->wio = wget_Connect(ulreq->url, uploadConnected, action); 
    if (ht->wio == NULL){
        slog(TR69C_LOG_ERR, "wget_Connect to upload server failed: %s", wget_LastErrorMsg());
        uploadStop("Connect to upload server failed", 9011);   
    } else {

	  	protect_connect_state_flag=0;
		setACSContactedState( eACS_UPLOAD );
		protect_connect_state_flag=1;
        wget_ClearPostHdrs(ht->wio);
        if (ht->authHdr){
            free(ht->authHdr);
            ht->authHdr = NULL;
        }
    }
	
slog(TR69C_LOG_DEBUG, "Upload Start  1!!!");
}
//#endif   // RPC_UPLOAD

