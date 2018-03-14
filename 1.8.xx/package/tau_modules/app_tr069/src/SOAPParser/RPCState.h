/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : RPCState.h
  版 本 号   : 初稿
  作    者   : zhanghaozhong
  生成日期   : 2013年5月16日
  最近修改   :
  功能描述   : RPC states and data structures
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

#ifndef RPCSTATE_H
#define RPCSTATE_H

#include "webproto/protocol.h"
 
/* fault codes */
#define NO_FAULT    0
#define RPC_PENDING 1
#define RPC_FAULTS  9000

/* Has the ACS been contacted,
* This is used to determine the type of Inform Event on 
* startup.
*/
typedef enum {
   eACS_NEVERCONTACTED = 0,
   eACS_CONTACTED,           /* NOTICE: These enum are order dependent*/
   eACS_UPLOAD,
   eACS_DOWNLOADREBOOT,
   eACS_SETVALUEREBOOT,
   eACS_ADDOBJECTREBOOT,
   eACS_DELOBJECTREBOOT,
   eACS_RPCREBOOT
} eACSContactedState;       /* NOTICE: These enum are order dependent*/
#define NOREBOOT eACS_NEVERCONTACTED

/* status enum for acs connection and msg xfer */
typedef enum {
   eOK,
   eConnectError,
   eGetError,
   ePostError,
   eAuthError,
   eDownloadDone,
//#ifdef RPC_UPLOAD    
   eUploadDone,
//#endif   
   eAcsDone
} AcsStatus;

/* rpcRun return status */
typedef enum {
   eRPCRunOK,     /* sent RPC response to ACS */
   eRPCRunEnd,    /* sent NULL http msg to ACS */
   eRPCRunFail    /* RPC run failed no reponse to send */
                  /*  this should probably send a fault */
} RunRPCStatus;

/* inform event enumb -- kind of inform msg */
typedef enum {
   eIEBootStrap,
   eIEBoot,
   eIEPeriodix,
   eIEScheduled,
   eIEValueChanged,
   eIEKicked,
   eIEConnectionRequest,
   eIETransferComplete,
   eIEDiagnostics,
   /*Add by duanguiyuan 20160331 添加Password认证*/
   ePasswordAuth,
   /* BEGIN: Added by zhanghaozhong, 2013/5/24   PN:增加告警上报功能 */
   eIEAlarm,
   /* END:   Added by zhanghaozhong, 2013/5/24   PN:增加告警上报功能 */
   eAlarmByTAU,
   eIEMethodResult,
   eIEXVendor
} eInformEvent;


typedef enum {
   rpcUnknown=0,
   rpcGetRPCMethods,
   rpcSetParameterValues,
   rpcGetParameterValues,
   rpcGetParameterNames,
   rpcGetParameterAttributes,
   rpcSetParameterAttributes,
   rpcAddObject,
   rpcDeleteObject,
   rpcReboot,
   rpcDownload,
   rpcUpload,  
   rpcFactoryReset,            /******** last rpc method ******/
   rpcInformResponse,          /* responses start here */
   rpcTransferCompleteResponse,
   rpcGetRPCMethodsResponse,
   rpcFault                   /* soapenv:Fault response from ACS */
} eRPCMethods;
#define LAST_RPC_METHOD    rpcFactoryReset     /* see above enumeration */

#define MAXINFORMEVENTS eIEXVendor    // should be match with number of eInformEvent 
typedef struct InformEvList {
   eInformEvent   informEvList[MAXINFORMEVENTS];
   int            informEvCnt;   /* number of events in list */
   eRPCMethods    mMethod;       /* set if M <method> event required */
} InformEvList;

typedef enum {
   eFirmwareUpgrade=1,
   eVendorLog     =2,
   eVendorConfig   =3,
   eVendorLog_web  =4,
   eVendorPerformance = 5
} eFileType;

typedef struct DownloadReq {
   eFileType   efileType;
   char        *commandKey;
   char        *url;
   char        *user;
   char        *pwd;
   int         fileSize;
   char        *fileName;  /* ignore in this implementation- everything is in memory */
   int         delaySec;
} DownloadReq;

typedef struct ParamItem {
   struct ParamItem  *next;
   char              *pname;
   char              *pvalue;
   char              *pOrigValue;
   int               fault;   /* 0 or set fault code */
} ParamItem;

typedef struct AttributeItem {
   struct AttributeItem *next;
   char                 *pname;
   int                  notification;
   int                  chgNotify;
   int                  chgAccess;
   int                  subAccess;  /* need to add list here if spec changes or vendor reqmts */
} AttributeItem;

typedef struct ParamNamesReq {
   char  *parameterPath;
   int   nextLevel;
} ParamNamesReq;

typedef struct AddDelObject {
   char  *objectName;   /* For Add the object name is xxx.yyy. */
                        /* for Delete the object anme is xxx.yyy.i. */
                        /* where i is the instance number */
} AddDelObject;

typedef struct   {
   char        *ID;           /* pointer to ID string */
   char        *informID;     /* ID string sent with last inform */
   eRPCMethods rpcMethod;
   int         arrayItemCnt;  /* cnt of items in parameter list-not used */
   char        *commandKey;   /* */
   char        *parameterKey; /* for setParameterValue key */
   union {
      ParamItem       *pItem;
      AttributeItem   *aItem;
      ParamNamesReq   paramNamesReq;
      AddDelObject    addDelObjectReq;
      DownloadReq     downloadReq;
      /* more items here later for each rpc method*/
   } ud;
} RPCAction;

/* structures to save notification entries */
typedef struct AttEntry {
   short int   nodeIndex;      /* index in CPE param table */
   short int   attrValue;      /* attribute value (1..2)  0 is not saved*/
   int         instanceId;      /* Id of instance or zero */
} AttEntry;

typedef struct AttSaveBuf {
   short int   sigValue;
   short int   numAttSaved;      /* number of notification attributes saved*/
   AttEntry      attEntry[];
} AttSaveBuf;


RPCAction*   newRPCAction(void);
RunRPCStatus runRPC(void);
RunRPCStatus runRPC_test(void);
void freeRPCAction(RPCAction *item);
void dumpAcsState(void);
void dumpRpcAction(RPCAction *);
void buildInform(RPCAction *a, InformEvList *infEvent, char* myExtIP);
void updateKeys( RPCAction *a);
int  checkActiveNotifications(void);
void initNotification(void);
int  getAllNotifications(tProtoCtx **pc, int *bufsz);
void resetNotificationBuf(void);
void saveConfigurations(void);
void rebootCompletion(void);
void factoryResetCompletion(void);
void sendTransferComplete(void);
//int  restoreNotificationAttr(void);
void saveNotificationAttributes(void);
#endif
