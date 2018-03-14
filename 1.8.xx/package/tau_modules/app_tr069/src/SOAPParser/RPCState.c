/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : RPCState.c
  版 本 号   : 初稿
  作    者   : zhanghaozhong
  生成日期   : 2013年5月16日
  最近修改   :
  功能描述   : RPC routines
  函数列表   :
              buildInform
              checkActiveNotifications
              closeBodyEnvelope
              deleteTree
              doAddObject
              doDeleteObject
              doDownload
              doFactoryResetRPC
              doGetParameterAttributes
              doGetParameterNames
              doGetParameterValues
              doGetRPCMethods
              doRebootRPC
              doSetParameterAttributes
              doSetParameterValues
              doUpload
              dumpAcsState
              dumpRpcAction
              factoryResetCompletion
              freeAttributeItems
              freeDownloadReq
              freeParamItems
              freeRPCAction
              getAllNotifications
              getFaultCode
              getFaultStr
              getInformEventStr
              getNotifyParams
              getRPCMethodName
              initNotification
              mprintf
              newRPCAction
              openBody
              openEnvWithHeader
              rebootCompletion
              resetNotificationBuf
              restoreNodeNotifyAttr
              restoreNotificationAttr
              runRPC
              saveConfigurations
              saveNotificationAttributes
              saveNotifyAttr
              sendGetRPCMethods
              sendTransferComplete
              setAttributes
              traverseTree
              updateKeys
              walkTree
              writeGetAttribute
              writeGetPName
              writeGetPValue
              writeParamValueStruct
              writeSoapFault
              XMLmIndent
              xml_mprintf
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h> 
#include <syslog.h>

#include "tr69cdefs.h" /* defines for ACS state */
#include "appdefs.h"
#include "utils.h"
#include "event.h"
#include "nanoxml/nanoxml.h"
#include "xmlParserSM.h"
#include "CPEframework.h"
#include "RPCState.h"
#include "xmlTables.h"
#include "informer.h"
#include "httpDownload.h"
#include "sysWrapper.h" /*add by duanguiyuan 20160322*/
#include "om_tr69c.h"

#define empty(x) ((x) == NULL || strcmp((x), "") == 0)
#define PNAME_LTH    257   /* parameter name length+1 */
#define MAX_PADDINGS 20

/* forwards */

static const char    *getRPCMethodName(eRPCMethods );
extern eInformState  informState;
extern unsigned int uiInformState;
extern unsigned int uiLastCfgState;
extern int iAlarmPending;
extern int g_log_level;
extern eACSContactedState acsContactedState;
extern int protect_connect_state_flag;
extern OM_TR69C_ALARM_INFO_T g_alarm_info;
//extern PAL_TR69C_UDP_MSG g_stUdpMSG;

RPCAction   *rpcAction;

/* State of ACS maintained here */
ACSState    acsState;

int         transferCompletePending;
int         sendGETRPC;

static eACSContactedState  rebootFlag;       /* a TRX_REBOOT has been returned from a setxxx */
static int                 saveConfigFlag;   /* save config on disconnect */
int                        saveNotificationsFlag;

/* Active Notification paramValueStruct buffer pointer and size */
static int  activeNotifications;
static int  notifyChanges;
/* if deviceConfig.configFile is set, flash content is not to be altered afterward */
#ifdef MDM_DEVICE_CONFIG    
static int  configFileSet = 0;
#endif   // MDM_DEVICE_CONFIG            

RPCAction* newRPCAction( void )
{
    RPCAction* rpc;
    if ( (rpc = (RPCAction *)malloc(sizeof(RPCAction)))) {
        memset(rpc,0,sizeof(RPCAction));
    }
    return rpc;
}
char *informParameters_N2[] = {
"InternetGatewayDevice.X_CMCC_SINR",
"InternetGatewayDevice.X_CMCC_UpstreamTotalByte",
"InternetGatewayDevice.X_CMCC_DownstreamTotalByte",
"InternetGatewayDevice.X_CMCC_StartTime",
"InternetGatewayDevice.X_CMCC_LoadAverage",
"InternetGatewayDevice.X_CMCC_DeviceMemory",
"InternetGatewayDevice.X_CMCC_MTU",
"InternetGatewayDevice.DeviceInfo.X_CMCC_IMEI",
"InternetGatewayDevice.DeviceInfo.X_CMCC_ConfigVersion",
"InternetGatewayDevice.DeviceInfo.X_CMCC_ModuleVersion",
"InternetGatewayDevice.DeviceInfo.X_CMCC_ModuleType", 
"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.2.WANPPPConnection.1.Enable",
"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.2.WANPPPConnection.1.Name",
"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.2.WANPPPConnection.1.Uptime",
"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.2.WANPPPConnection.1.Username",
};
char *informParameters_N1[] = {
"InternetGatewayDevice.X_CMCC_CellLockType",
"InternetGatewayDevice.X_CMCC_CellLocking",
"InternetGatewayDevice.X_CMCC_CellLockInfo",
"InternetGatewayDevice.X_CMCC_LockPin",
"InternetGatewayDevice.X_CMCC_LockPinType",
"InternetGatewayDevice.X_CMCC_FirstPin",
"InternetGatewayDevice.X_CMCC_FixedPin",
"InternetGatewayDevice.X_CMCC_EncryptCard",
"InternetGatewayDevice.X_CMCC_EncryptCardKey",
"InternetGatewayDevice.X_CMCC_RSRP",
"InternetGatewayDevice.X_CMCC_RSRQ",

};
char *informParameters[] = {
	/*
"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.ExternalIPAddress",
"InternetGatewayDevice.ManagementServer.ConnectionRequestPassword",

"InternetGatewayDevice.ManagementServer.Password",

"InternetGatewayDevice.DeviceSummary",
"InternetGatewayDevice.DeviceInfo.HardwareVersion",
*/

"InternetGatewayDevice.ManagementServer.Username",
"InternetGatewayDevice.ManagementServer.ConnectionRequestUsername",

"InternetGatewayDevice.DeviceInfo.Manufacturer",
"InternetGatewayDevice.DeviceInfo.ManufacturerOUI",
"InternetGatewayDevice.DeviceInfo.ModelName",
"InternetGatewayDevice.DeviceInfo.ProductClass",
"InternetGatewayDevice.DeviceInfo.ProvisioningCode",
"InternetGatewayDevice.DeviceInfo.SerialNumber",
"InternetGatewayDevice.DeviceInfo.SoftwareVersion",
"InternetGatewayDevice.DeviceInfo.SpecVersion",
"InternetGatewayDevice.ManagementServer.ConnectionRequestURL",
"InternetGatewayDevice.ManagementServer.ParameterKey",
"InternetGatewayDevice.WANDevice.1.X_ATP_WANNetConfigInfo.CellID",
"InternetGatewayDevice.WANDevice.1.X_ATP_WANNetConfigInfo.RSRP",
"InternetGatewayDevice.WANDevice.1.X_ATP_WANNetConfigInfo.RSRQ",
"InternetGatewayDevice.WANDevice.1.X_ATP_WANNetConfigInfo.RSSI",
"InternetGatewayDevice.WANDevice.1.X_ATP_WANNetConfigInfo.SINR",
//"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.ExternalIPAddress",

//"InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.MACAddress",
//"InternetGatewayDevice.ManagementServer.PeriodicInformTime",
"InternetGatewayDevice.ManagementServer.PeriodicInformInterval",
"InternetGatewayDevice.ManagementServer.PeriodicInformEnable",
//"InternetGatewayDevice.ManagementServer.UpgradesManaged",
 
 	NULL, /* for alarm */
	NULL, /* for alarm */
	NULL, /* for alarm */
	NULL, /* for alarm */
	NULL, /* for alarm */
	NULL, /* for alarm */
 	NULL,
    NULL                        /* this is filled in by buildExternalConnParamName() */
};

#define NUMREQINFORMPARMS (sizeof(informParameters)/sizeof(char *))
int informParameters_num = NUMREQINFORMPARMS;
/* 
	add by duanguiyuan
	添加告警上报节点名称，列表
*/
char *alarmParametersname[] = {
"InternetGatewayDevice.WANDevice.1.X_ATP_AlarmInstance.LAN1",
"InternetGatewayDevice.WANDevice.1.X_ATP_AlarmInstance.LAN2",
"InternetGatewayDevice.WANDevice.1.X_ATP_AlarmInstance.LAN3",
"InternetGatewayDevice.WANDevice.1.X_ATP_AlarmInstance.LAN4",
"InternetGatewayDevice.WANDevice.1.X_ATP_AlarmInstance.LanCheck",
NULL
};
char *informEventStr[]={
    "0 BOOTSTRAP",
    "1 BOOT",
    "2 PERIODIC",
    "3 SCHEDULED",
    "4 VALUE CHANGE",
    "5 KICKED",
    "6 CONNECTION REQUEST",
    "7 TRANSFER COMPLETE",
    "8 DIAGNOSTICS COMPLETE",
    /*
    "M Reboot",			//重启完成，与1 BOOT 同时使用
    "M Download",		//先前RMS使用Download方法请求的内容下载已经完成
    "M Upload",			//先前RMS使用Upload方法请求的内容上传已经完成
   
    "CMCC MONITOR",		//上报监控参数
    
    */
    "X CMCC BIND",		//用于家庭网关基于设备认证Password认证首次连接RMS
    /* BEGIN: Added by zhanghaozhong, 2013/5/24   PN:增加告警上报功能 */
    "X OUI AlARM",
    /* END:   Added by zhanghaozhong, 2013/5/24   PN:增加告警上报功能 */
	/* add by duanguiyuan 20161126 添加华为定制告警上报*/
    "X ACE215 AlarmEvent",
    " "
    
};

/* 根据事件枚举号，获取事件字符串 */
static char *getInformEventStr(eInformEvent iev, eRPCMethods rpcMethod)
{

   static char eventstrbuf[30];
   if (iev>=eIEBootStrap &&  iev<eIEMethodResult) 
   {  // 0<iev<10
   	
      return informEventStr[iev];
   }
   else if (iev==eIEMethodResult && rpcMethod!=rpcUnknown ) 
   {
      snprintf(eventstrbuf,sizeof(eventstrbuf),"M %s",getRPCMethodName(rpcMethod));
	 
      return eventstrbuf;
   }
   eventstrbuf[0]='\0';

   return eventstrbuf;
   
}


void dumpAcsState(void)
{
    if (g_debug)
    {
       fprintf(stderr, "ACS State dump\n");
       fprintf(stderr, "HoldRequests       %d\n", acsState.holdRequests);
       fprintf(stderr, "NoMoreRequest      %d\n", acsState.noMoreRequests);
       fprintf(stderr, "CommandKey(DL)     %s\n", acsState.downloadCommandKey);
       fprintf(stderr, "CommandKey(Reboot  %s\n", acsState.rebootCommandKey);
       fprintf(stderr, "ParameterKey       %s\n", acsState.parameterKey);
       fprintf(stderr, "MaxEnvelopes       %d\n", acsState.maxEnvelopes);
       fprintf(stderr, "RPC Methods supported by ACS:\n");
       fprintf(stderr, "   GetRpcMethods           %s\n", acsState.acsRpcMethods.rpcGetRPCMethods?
               "Yes": "No");
       fprintf(stderr, "   SetParameterValues      %s\n", acsState.acsRpcMethods.rpcSetParameterValues?
               "Yes": "No");
       fprintf(stderr, "   GetParameterValues      %s\n", acsState.acsRpcMethods.rpcGetParameterValues?
               "Yes": "No");
       fprintf(stderr, "   GetParameterNames       %s\n", acsState.acsRpcMethods.rpcGetParameterNames?
               "Yes": "No");
       fprintf(stderr, "   GetParameterAttributes  %s\n", acsState.acsRpcMethods.rpcGetParameterAttributes?
               "Yes": "No");
       fprintf(stderr, "   SetParameterAttributes  %s\n", acsState.acsRpcMethods.rpcSetParameterAttributes?
               "Yes": "No");
       fprintf(stderr, "   Reboot                  %s\n", acsState.acsRpcMethods.rpcReboot?
               "Yes": "No");
       fprintf(stderr, "   FactoryReset            %s\n", acsState.acsRpcMethods.rpcFactoryReset?
               "Yes": "No");
       fprintf(stderr, "   rpcDownload             %s\n", acsState.acsRpcMethods.rpcDownload?
               "Yes": "No");
    /* BEGIN: Modified by zhanghaozhong, 2013/1/23   PN:transplant tr-069 by zhz */
//#ifdef RPC_UPLOAD    
       fprintf(stderr, "   rpcUpload             %s\n", acsState.acsRpcMethods.rpcUpload?
                "Yes": "No");
//#endif
    /* END:   Modified by zhanghaozhong, 2013/1/23   PN:transplant tr-069 by zhz */
    }
}

void dumpRpcAction(RPCAction *a)
{
    if (g_debug)
   fprintf(stderr, "RPC description: RPC Method = %s ID=%s\n",
           getRPCMethodName(a->rpcMethod), a->ID);
}

/* 在此处重启设备 */
/* rebootCompletion routine */
/* This is envoked following the ACS response to the rebootresponse msg */
void rebootCompletion(void)
{
   if (rebootFlag>=eACS_DOWNLOADREBOOT && rebootFlag<=eACS_RPCREBOOT) {
      printf("rebootCompletion\n");
      //sleep(100);
     // closeAllFds(); /*注释 by duanguiyuan 20160323 */
      saveTR69StatusItems( &acsState );
      /* if configFile is set, reboot doesn't saveConfig; otherwise, saveConfig */
#ifdef MDM_DEVICE_CONFIG    
      if (configFileSet) {
         wrapperReboot(rebootFlag);
      }
      else {
         wrapperReboot(rebootFlag);
      }
#else
    wrapperReboot(rebootFlag);
#endif   // MDM_DEVICE_CONFIG            
   }
}

/* factoryResetCompletion routine */
/* This is envoked following the ACS response to the FactoryResetResponse msg */
static int factoryResetFlag;
void factoryResetCompletion(void)
{
   if (factoryResetFlag) 
   {
      wrapperFactoryReset();
      factoryResetFlag = 0;
   }
}

/* save Configuration routine */
/* This is envoked following the ACS response to the a set/add/delete RPC msg */
void saveConfigurations(void)
{
#ifdef MDM_DEVICE_CONFIG    
   if ( saveConfigFlag && !configFileSet )
#else
   if ( saveConfigFlag )
#endif   // MDM_DEVICE_CONFIG            
   {
      wrapperSaveConfigurations();
      saveConfigFlag = 0;
   }
}

/* Utility routines for data structures */
static const char    *getRPCMethodName(eRPCMethods m)
{
   const char *t;



   

   switch (m) {
      case rpcGetRPCMethods:
         t="GetRPCMethods";
         break;
      case rpcSetParameterValues:
         t = "SetParameterValues";
         break;
      case rpcGetParameterValues:
         t="GetParameterValues";
         break;
      case rpcGetParameterNames:
         t="GetParameterNames";
         break;
      case rpcGetParameterAttributes:
         t="GetParameterAttributes";
         break;
      case rpcSetParameterAttributes:
         t="SetParameterAttributes";
         break;
      case rpcAddObject:
         t="AddObject";
         break;
      case rpcDeleteObject:
         t="DeleteObject";
         break;
      case rpcReboot:
         t="Reboot";
         break;
      case rpcDownload:
         t="Download";
         break;
//#ifdef RPC_UPLOAD    
    case rpcUpload:
        t="Upload";
        break;
//#endif   // RPC_UPLOAD        
      case rpcFactoryReset:
         t="FactoryReset";
         break;
      case rpcInformResponse:
         t="InformResponse";
         break;
      default:
         t="no RPC methods";
         break;
   }
   return t;
}

static void freeParamItems( ParamItem *item)
{
   ParamItem *next;

   while (item) {
      next = item->next;
      free(item->pname);     /* free data */
	  item->pname=NULL;
      free(item->pvalue);
	  item->pvalue=NULL;
      free(item->pOrigValue);
	  item->pOrigValue=NULL;
      free(item);           /* free ParamItem */
	  item=NULL;
      item = next;
   }
}

static void freeAttributeItems( AttributeItem *item)
{
   AttributeItem *next;

   while (item) {
      next = item->next;
      free(item->pname);
	  item->pname=NULL;
      free(item);
	  item=NULL;
      item = next;
   }
}

static void freeDownloadReq( DownloadReq *r)
{
   free(r->url);
   r->url=NULL;
   free(r->user);
   r->user=NULL;
   free(r->pwd);
   r->user=NULL;
   free(r->fileName);
   r->fileName=NULL;
}

/*
 * item is undefined on return
 */
void freeRPCAction(RPCAction *item)
{
   free(item->ID);
   item->ID=NULL;
   free(item->parameterKey);
   item->parameterKey=NULL;
   free(item->commandKey);
   item->commandKey=NULL;
   free(item->informID);
   item->informID=NULL;
   switch(item->rpcMethod) {
      case rpcGetParameterNames:
         free(item->ud.paramNamesReq.parameterPath);
		 item->ud.paramNamesReq.parameterPath=NULL;
         break;
      case rpcSetParameterValues:
      case rpcGetParameterValues:
         freeParamItems(item->ud.pItem);
		 item->ud.pItem=NULL;
         break;
      case rpcSetParameterAttributes:
      case rpcGetParameterAttributes:
         freeAttributeItems(item->ud.aItem);
		 item->ud.aItem=NULL;
         break;
      case rpcAddObject:
      case rpcDeleteObject:
         free(item->ud.addDelObjectReq.objectName);
		 item->ud.addDelObjectReq.objectName=NULL;
         break;
      case rpcDownload:
         freeDownloadReq( &item->ud.downloadReq );
         break;
//#ifdef RPC_UPLOAD    
    case rpcUpload:
        freeDownloadReq( &item->ud.downloadReq );
        break;
//#endif   // RPC_UPLOAD        
      default:
         break;
   }
   free(item);
   item=NULL;
}  /* End of freeRPCAction() */

/*----------------------------------------------------------------------*
 * memory printf
 */
static void mprintf(tProtoCtx *pc, int *len, const char *fmt, ...)
{
   int      n;
   va_list  ap;
   char     buf[512];

   va_start(ap, fmt);
   if (ap == NULL)
   {
      n = strlen(fmt);
      if (n > 0)
      {
         *len += n;
         if (pc != NULL)
         {
            proto_SendRaw(pc, fmt, n);
         }
      }
   }
   else
   {
      n = vsnprintf(buf, 512, fmt, ap);
      if (n < 0 || n >= 512)
      {
         /* out of memory */ 
         slog(TR69C_LOG_ERR, "xml: mprintf: out of memory");
         acsState.fault = 9004;
      }
      else if (n > 0)
      {
         *len += n;
         if (pc != NULL)
         {
            proto_SendRaw(pc, buf, n);
         }
      }
   }
   va_end(ap);
}

/*----------------------------------------------------------------------*/
static void xml_mprintf(tProtoCtx *pc, int *len, char *s)
{
   if (s)
   {
      for (; *s; s++)
      {
         switch (*s)
         {
            case '&':
               mprintf(pc, len, "&amp;");
               break;
            case '<':
               mprintf(pc, len, "&lt;");
               break;
            case '>':
               mprintf(pc, len, "&gt;");
               break;
            case '"':
               mprintf(pc, len, "&quot;");
               break;
            case '\'':
               mprintf(pc, len, "&apos;");
            case 9:
            case 10:
            case 13:
               mprintf(pc, len, "&#%d;", *s);
               break;
            default:
               if (isprint(*s))
               {
                  mprintf(pc, len, "%c", *s);
               }
               else
               {
                  mprintf(pc, len, " ");
               }
               break;
         }
      }
   }
}

#ifdef OMIT_INDENT
#define xml_mIndent(A,B,C) ;
#else
#define xml_mIndent(A,B,C) XMLmIndent( A, B, (C * 2));
#endif
/*----------------------------------------------------------------------*/
static void XMLmIndent(tProtoCtx *pc, int *len, int indent)
{
   int i;

   if (indent <= 0)
   {
      return;
   }

   for (i = 0; i < indent; i++)
   {
      mprintf(pc, len, " ");
   }
}

static void closeBodyEnvelope(tProtoCtx *pc, int *lth)
{
   xml_mIndent(pc, lth, 2);
   mprintf(pc, lth, "</%sBody>\n", nsSOAP);
   mprintf(pc, lth, "</%sEnvelope>\n", nsSOAP);
}

/* Add <SOAP:Body>
*/
static void openBody(tProtoCtx *pc, int *lth)
{
   xml_mIndent(pc, lth, 2);
   mprintf(pc, lth, "<%sBody>\n", nsSOAP);
}

/*
* Add <SOAP:Envelope
 *      xmlns:....
 *      <..:Header>
 *      ....
 *      </..:Header>
 * to the buffer
 */
static void openEnvWithHeader(char *idstr, tProtoCtx *pc, int *lth)
{
   NameSpace   *ns;
   mprintf(pc, lth, "<%sEnvelope", nsSOAP);
   /* generate Namespace declarations */
   ns = nameSpaces;
   while (ns->sndPrefix)
   {
      char    pbuf[40];
      char    *e;
      mprintf(pc, lth, "\n");
      strncpy(pbuf, ns->sndPrefix, sizeof(pbuf));
      e=strchr(pbuf,':');
      if (e) *e='\0'; /* find : in prefix */
      xml_mIndent(pc, lth, 2);
      mprintf(pc, lth, "xmlns:%s=\"%s\"", pbuf, ns->nsURL);
      ++ns;
   }
   mprintf(pc, lth, ">\n");
   if (idstr)
   {
      xml_mIndent(pc, lth, 2);
      mprintf(pc, lth, "<%sHeader>\n", nsSOAP);
      xml_mIndent(pc, lth, 3);
      mprintf(pc, lth, "<%sID %smustUnderstand=\"1\">%s</%sID>\n",
               nsCWMP, nsSOAP,idstr , nsCWMP);
      xml_mIndent(pc, lth, 2);
      mprintf(pc, lth, "</%sHeader>\n", nsSOAP);
   }
}

static const char *getFaultCode( int fault )
{
   const char *r;
    switch(fault){
    case 9000:
    case 9001:
    case 9002:
    case 9004:
    case 9009:
    case 9010:
    case 9011:
    case 9012:
    case 9013:
        r = "Server";
        break;
    case 9003:
    case 9005:
    case 9006:
    case 9007:
    case 9008:
        r = "Client";
        break;
    default:
        r = "Vendor";
        break;
    }
   return r;
}

static const char *getFaultStr( int fault)
{
   const char *detailFaultStr;

    switch(fault){
    case 9000:
        detailFaultStr = "Method not supported";
        break;
    case 9001:
        detailFaultStr = "Request denied";
        break;
    case 9002:
        detailFaultStr = "Internal Error";
        break;
    case 9003:
        detailFaultStr = "Invalid arguments";
        break;
    case 9004:
        detailFaultStr = "Resources Exceeded";
        break;
    case 9005:
        detailFaultStr = "Invalid Parameter Name";
        break;
    case 9006:
        detailFaultStr = "Invalid parameter type";
        break;
    case 9007:
        detailFaultStr = "Invalid parameter value";
        break;
    case 9008:
        detailFaultStr = "Attempt to set a non-writeable parameter";
        break;
    case 9009:
        detailFaultStr = "Notification request rejected";
        break;
    case 9010:
        detailFaultStr = "Download failure";
        break;
//#ifdef RPC_UPLOAD    
    case 9011:
        detailFaultStr = "Upload failure";
        break;
//#endif   // RPC_UPLOAD        
    case 9012:
        detailFaultStr = "File transfer server authentication failure";
        break;
    case 9013:
        detailFaultStr = "Unsupported protocol for file transfer";
      break;
   case 9014:
      detailFaultStr = "MaxEnvelopes exceeded";
        break;
    default:
        detailFaultStr = "Vendor defined fault";
        break;
    }
   return detailFaultStr;
}

static void writeSoapFault(RPCAction *a, int fault)
{
   int         loop;
   tProtoCtx   *pc;

   /* Loop through the code twice.  The first loop is to calculate
    * the size of the entire soap envelope so that the HTTP header
    * can be written to the TCP socket first.  The second loop is
    * to write the content of the envelope to the TCP socket as it
    * is generated.
    */
   for (loop = 0, pc = NULL; loop < 2; loop++, pc = getAcsConnDesc()) {

      int   bufsz = 0;

      openEnvWithHeader(a->ID, pc, &bufsz);
      openBody(pc, &bufsz);
      xml_mIndent(pc, &bufsz, 3);
      mprintf(pc, &bufsz, "<%sFault>\n", nsSOAP);
      xml_mIndent(pc, &bufsz, 4);
      mprintf(pc, &bufsz, "<faultcode>%s</faultcode>\n", getFaultCode(fault));
      xml_mIndent(pc, &bufsz, 4);
      mprintf(pc, &bufsz, "<faultstring>CWMP fault</faultstring>\n");
      xml_mIndent(pc, &bufsz, 5);
      mprintf(pc, &bufsz, "<detail>\n");
      xml_mIndent(pc, &bufsz, 6);
      mprintf(pc, &bufsz, "<%sFault>\n", nsCWMP);
      xml_mIndent(pc, &bufsz, 7);
      mprintf(pc, &bufsz, "<FaultCode>%d</FaultCode>\n", fault);
      xml_mIndent(pc, &bufsz, 7);
      mprintf(pc, &bufsz, "<FaultString>%s</FaultString>\n", getFaultStr(fault));
      if (a->rpcMethod == rpcSetParameterValues) {

         ParamItem   *pi = a->ud.pItem;
         /* walk thru parameters to generate errors */
         while (pi != NULL ) {

            if (pi->fault) {
               xml_mIndent(pc, &bufsz, 7);
               mprintf(pc, &bufsz, "<SetParameterValuesFault>\n");
               xml_mIndent(pc, &bufsz, 8);
               mprintf(pc, &bufsz, "<ParameterName>%s</ParameterName>\n",
                           pi->pname);
               xml_mIndent(pc, &bufsz, 8);
               mprintf(pc, &bufsz, "<FaultCode>%d</FaultCode>\n", pi->fault);
               xml_mIndent(pc, &bufsz, 8);
               mprintf(pc, &bufsz, "<FaultString>%s</FaultString>\n",
                           getFaultStr(pi->fault));
               xml_mIndent(pc, &bufsz, 7);
               mprintf(pc, &bufsz, "</SetParameterValuesFault>\n");
            }
            pi = pi->next;
         }
      }
      xml_mIndent(pc, &bufsz, 6);
      mprintf(pc, &bufsz, "</%sFault>\n", nsCWMP);
      xml_mIndent(pc, &bufsz, 5);
      mprintf(pc, &bufsz, "</detail>\n");
      xml_mIndent(pc, &bufsz, 3);
      mprintf(pc, &bufsz, "</%sFault>\n", nsSOAP);
      closeBodyEnvelope(pc, &bufsz);

      if (loop == 0) {
         /* send the HTTP message header */
         sendToAcs(bufsz, NULL);
      }
   }  /* for (loop.....) */
   uiLastCfgState = 1;  /* 供WEB使用，1：业务配置失败 */
}  /* End of writeSoapFault() */

static void doGetRPCMethods(RPCAction *a)
{
   eRPCMethods m;
   int         loop;
   tProtoCtx   *pc;

   /* Loop through the code twice.  The first loop is to calculate
    * the size of the entire soap envelope so that the HTTP header
    * can be written to the TCP socket first.  The second loop is
    * to write the content of the envelope to the TCP socket as it
    * is generated.
    */
   for (loop = 0, pc = NULL; loop < 2; loop++, pc = getAcsConnDesc()) {

      int   bufsz = 0;
   
      openEnvWithHeader(a->ID, pc, &bufsz);
      openBody(pc, &bufsz);
      xml_mIndent(pc, &bufsz, 3);
      mprintf(pc, &bufsz, "<%sGetRPCMethodsResponse>\n", nsCWMP);
      xml_mIndent(pc, &bufsz, 4);
      #ifdef SUPPRESS_SOAP_ARRAYTYPE
      mprintf(pc, &bufsz, "<MethodList>\n");
slog(TR69C_LOG_DEBUG, "<MethodList>\n");
      #else
      mprintf(pc, &bufsz, "<MethodList %sarrayType=\"%sstring[%d]\">\n",
              nsSOAP_ENC, nsXSD, LAST_RPC_METHOD );
      #endif
   
      for (m = rpcGetRPCMethods; m <= LAST_RPC_METHOD; ++m) {
         xml_mIndent(pc, &bufsz, 5);
         mprintf(pc, &bufsz, "<string>%s</string>\n", getRPCMethodName(m));
slog(TR69C_LOG_DEBUG,  "<string>%s</string>\n", getRPCMethodName(m));
      }
      xml_mIndent(pc, &bufsz, 4);
      mprintf(pc, &bufsz, "</MethodList>\n");
slog(TR69C_LOG_DEBUG, "</MethodList>\n");
      xml_mIndent(pc, &bufsz, 3);
      mprintf(pc, &bufsz, "</%sGetRPCMethodsResponse>\n", nsCWMP);
      closeBodyEnvelope(pc, &bufsz);

      if (loop == 0) {
         /* send the HTTP message header */
         sendToAcs(bufsz, NULL);
      }
   }  /* for (loop.....) */

}  /* End of doGetRPCMethods() */

typedef void (*SoapOutCB)(TRxObjNode *node, void **pc, int *bufsz, int isFragment);
static char paramName[PNAME_LTH];
static int  paramCnt;

static void writeGetAttribute(TRxObjNode *node, tProtoCtx **pc, int *bufsz, int isFragment)
{
   InstanceDope   *pDope      = findDopeInstance(node);
   int            accessIndex = pDope? pDope->accessListIndex: 0;

   xml_mIndent(*pc,bufsz,5);
   mprintf(*pc,bufsz,"<ParameterAttributeStruct>\n");

//slog(TR69C_LOG_DEBUG, "<ParameterAttributeStruct>\n");
   xml_mIndent(*pc,bufsz,6);
   mprintf(*pc,bufsz,"<Name>%s</Name>\n", paramName);
slog(TR69C_LOG_DEBUG,"<Name>%s</Name>\n", paramName);

   xml_mIndent(*pc,bufsz,6);
   mprintf(*pc,bufsz,"<Notification>%d</Notification>\n", pDope? pDope->notification: 0);
slog(TR69C_LOG_DEBUG, "<Notification>%d</Notification>\n", pDope? pDope->notification: 0);


   xml_mIndent(*pc,bufsz,6);   /*** ?????? lot more here is more than just subscriber access */
   #ifdef SUPPRESS_SOAP_ARRAYTYPE
   mprintf(*pc,bufsz,"<AccessList>\n");
//slog(TR69C_LOG_DEBUG,"<AccessList>\n");

   #else
   mprintf(*pc,bufsz,"<AccessList %sarrayType=\"%sstring[%d]\">\n", nsSOAP_ENC, nsXSD,accessIndex);
//slog(TR69C_LOG_DEBUG,"<AccessList %sarrayType=\"%sstring[%d]\">\n", nsSOAP_ENC, nsXSD,accessIndex);
                                               
   #endif
   if (accessIndex) {
      xml_mIndent(*pc,bufsz,7);
      mprintf(*pc,bufsz,"<%sstring>%s</%sstring>\n",nsXSD, "Subscriber", nsXSD);
//slog(TR69C_LOG_DEBUG, "<%sstring>%s</%sstring>\n",nsXSD, "Subscriber", nsXSD);
   }
   xml_mIndent(*pc,bufsz,6);
   mprintf(*pc,bufsz,"</AccessList>\n");
//slog(TR69C_LOG_DEBUG,"</AccessList>\n");
   xml_mIndent(*pc,bufsz,5);
   mprintf(*pc,bufsz,"</ParameterAttributeStruct>\n");
//slog(TR69C_LOG_DEBUG,"</ParameterAttributeStruct>\n");
   ++paramCnt;

}  /* End of writeGetAttribute() */
/* 是否可读可写根据setTRxParam函数是否存在 */
static void writeGetPName(TRxObjNode *node, tProtoCtx **pc, int *bufsz, int isFragment)
{
   int   writeable    = 0;
   int   makeFragName = 0;

   xml_mIndent(*pc,bufsz,5);
   mprintf(*pc,bufsz,"<ParameterInfoStruct>\n");
   slog(TR69C_LOG_DEBUG, "<ParameterInfoStruct>\n");
   if (node->name != instanceIDMASK && node->paramAttrib.attrib.etype==tInstance) {
      /* current node is tInstance. It points to the instance Node */
      if (node->objDetail && ((TRxObjNode *)(node->objDetail))->setTRxParam) {
         writeable = 1;
      }
      makeFragName = 1;  /* if instance object then make it a name fragment */
   } else {
      writeable = node->setTRxParam!=NULL;
      makeFragName = ((node->name == instanceIDMASK) || 
                      (node->paramAttrib.attrib.etype == tInstance) ||
                      (node->paramAttrib.attrib.etype == tObject));
   }
   xml_mIndent(*pc,bufsz,6);
   mprintf(*pc,bufsz,"<Name>%s%s</Name>\n", paramName, (isFragment&&makeFragName)?".":"");

slog(TR69C_LOG_DEBUG, "<Name>%s%s</Name>\n", paramName, (isFragment&&makeFragName)?".":"");
   
   xml_mIndent(*pc,bufsz,6);
   mprintf(*pc,bufsz,"<Writable>%s</Writable>\n", writeable?"1":"0");
   
slog(TR69C_LOG_DEBUG, "<Writable>%s</Writable>\n", writeable?"1":"0");  

   xml_mIndent(*pc,bufsz,5);
   mprintf(*pc,bufsz,"</ParameterInfoStruct>\n");
slog(TR69C_LOG_DEBUG, "</ParameterInfoStruct>\n");
slog(TR69C_LOG_DEBUG, "paramCnt = %d \n",paramCnt);

   ++paramCnt;

}  /* End of writeGetPName() */
/* Begin: add by duanguiyuan 华为定制 getallname 非协议标准 */
char *ParametersName_string[] = {
"InternetGatewayDevice.DeviceInfo.",
"InternetGatewayDevice.IPPingDiagnostics.",
"InternetGatewayDevice.LANDevice.",
"InternetGatewayDevice.ManagementServer.",
"InternetGatewayDevice.Time.",
"InternetGatewayDevice.WANDevice.",
"InternetGatewayDevice.X_ATP_AlarmInfo.",
"InternetGatewayDevice.X_ATP_SystemMonitor.",
NULL,
NULL 
};
void writeGetPName_huawei(tProtoCtx **pc, int *bufsz)
{
	int i = 0;
	while(1)
	{
		if(ParametersName_string[i]==NULL)
			break;
		xml_mIndent(*pc,bufsz,5);
   		mprintf(*pc,bufsz,"<ParameterInfoStruct>\n");
		xml_mIndent(*pc,bufsz,6);
   		mprintf(*pc,bufsz,"<Name>%s</Name>\n",ParametersName_string[i]);
		xml_mIndent(*pc,bufsz,6);
   		mprintf(*pc,bufsz,"<Writable>0</Writable>\n");
    	xml_mIndent(*pc,bufsz,5);
   		mprintf(*pc,bufsz,"</ParameterInfoStruct>\n");
	}
	return ;
}
/* End: add by duanguiyuan 华为定制 getallname 非协议标准 */

/*
* Input:
 * node: Current node to link from
 * cback; Output routine to display node information
 * buf:   *pointer to next available buffer space.
 * bufsz: pointner to size remaining in buffer(buf).
 * noDescend: Just walk next level of tree- don't desend if true
 * oparam: Parameter passed to output (cback) function as parameter 4
 * 
 * If the node->name[0] is '#' then the node is ignored by the tree traversal.
 * This name allows pseudo parameter names to be added to the paraemter tree 
 * to be used to allow calling of getter/setter functions that are not directly related
 * to  TR069 parameters. These routines are accessed by the CPEFramework searchTree
 * functions.
*/
static void traverseTree(TRxObjNode *node, SoapOutCB cback, void **pc,
                         int *bufsz, int noDesend, int oparam)
{
   InstanceDesc   *idp;
   TRxObjNode     *n = node->objDetail;
   int            pNameEnd, plth;
    
   pNameEnd = plth = strlen(paramName);     
   if (paramName[pNameEnd-1]!='.' && plth<(PNAME_LTH-1))
      strcpy( &paramName[pNameEnd++], ".");
  
   if (n!=NULL && n->name==instanceIDMASK) {
      /* this is an object instance -- step thru the instances if present */
      idp = n->paramAttrib.instance; 
      while (idp!=NULL && !acsState.fault) {
         if (checkInstancePath(idp)) {
            pushInstance(idp);
            snprintf(&paramName[pNameEnd],PNAME_LTH-pNameEnd,"%d", idp->instanceID);
			
			// if (n->objDetail==NULL) { 
			 	
	  
	  			
          		  (cback)(n,pc,bufsz, oparam);
			//  }
            if (!noDesend)
               traverseTree(n, cback, pc, bufsz, 0, oparam);
            	popInstance();
         }

         /* truncate paramName */
          paramName[pNameEnd] = '\0';
         idp = idp->next;
      }
      return;
   }
   
   while ( n!=NULL && n->name!=NULL && !acsState.fault ) {

	  if (n->name[0] != '#') { /* check for pseudo parameter */
         strncpy( &paramName[pNameEnd], n->name, PNAME_LTH-pNameEnd);	
         if (n->objDetail==NULL) {
            (cback)(n, pc, bufsz, oparam);   /* Call output function*/
			
         } else if (!noDesend) {
            //(cback)(n,pc,bufsz, oparam);
            /* add current object name to paramName */
           traverseTree(n, cback, pc, bufsz, 0, oparam);
            /* truncate paramName */
            paramName[pNameEnd] = '\0';
         } else if (n->paramAttrib.attrib.etype == tInstance ||
                    n->paramAttrib.attrib.etype == tObject) {
  		//	traverseTree(n, cback, pc, bufsz, 0, oparam);
            (cback)(n, pc, bufsz, oparam);   /* special test to get instance write indicator*/
         }                                   /* to indicate AddObject/DeleteObject capability*/
      }
      ++n;
   }
   return;

}  /* End of traverseTree() */
 static void traverseTree_m(TRxObjNode *node, SoapOutCB cback, void **pc,
						  int *bufsz, int noDesend, int oparam)
 {
	InstanceDesc   *idp;
	TRxObjNode	   *n = node->objDetail;
	int 		   pNameEnd, plth;
   // printf("*************************************************\n");
   // printf("*************paramName =%s\n",paramName);
   // printf("*************************************************\n");
	pNameEnd = plth = strlen(paramName);	 
	if (paramName[pNameEnd-1]!='.' && plth<(PNAME_LTH-1))
	   strcpy( &paramName[pNameEnd++], ".");
   
	if (n!=NULL && n->name==instanceIDMASK) {
	   /* this is an object instance -- step thru the instances if present */
	   idp = n->paramAttrib.instance; 
	   
	   while (idp!=NULL && !acsState.fault) {
		  if (checkInstancePath(idp)) {
			 pushInstance(idp);
			 snprintf(&paramName[pNameEnd],PNAME_LTH-pNameEnd,"%d", idp->instanceID);
			 
			 // if (n->objDetail==NULL) { 
				 
	   
				 
				   (cback)(n,pc,bufsz, oparam);
			 //  }
			 if (!noDesend)
				traverseTree_m(n, cback, pc, bufsz, 0, oparam);
				 popInstance();
		  }
 
		  /* truncate paramName */
		   paramName[pNameEnd] = '\0';
		  idp = idp->next;
	   }
	   return;
	}
	
	while ( n!=NULL && n->name!=NULL && !acsState.fault ) {
 
	   if (n->name[0] != '#') { /* check for pseudo parameter */
		  strncpy( &paramName[pNameEnd], n->name, PNAME_LTH-pNameEnd);	 
		 // printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
		 // printf("$$$$$$$$$$$$$$$$$$paramName =%s\n",paramName);
		 // printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
		  if (n->objDetail==NULL) {
			 (cback)(n, pc, bufsz, oparam);   /* Call output function*/
			 
		  } else if (!noDesend) {
			 //(cback)(n,pc,bufsz, oparam);
			 /* add current object name to paramName */
			traverseTree_m(n, cback, pc, bufsz, 0, oparam);
			 /* truncate paramName */
			 paramName[pNameEnd] = '\0';
		  } else if (n->paramAttrib.attrib.etype == tInstance ||
					 n->paramAttrib.attrib.etype == tObject) {
		   traverseTree_m(n, cback, pc, bufsz, 0, oparam);
			// (cback)(n, pc, bufsz, oparam);   /* special test to get instance write indicator*/
		  } 								  /* to indicate AddObject/DeleteObject capability*/
	   }
	   ++n;
	}
	return;
 
 }


/* 
* GetParameterNames requests a single parameter path or single parameter path fragment 
*/
static void doGetParameterNames(RPCAction *a)
{
   int         isFragment;
   int         loop;
   tProtoCtx   *pc = NULL;

   acsState.fault = 0; /* init to no fault */
   paramCnt       = 0;
   /* Loop through the code twice.  The first loop is to calculate
    * the size of the entire soap envelope so that the HTTP header
    * can be written to the TCP socket first.  The second loop is
    * to write the content of the envelope to the TCP socket as it
    * is generated.
    */
   for (loop = 0, pc = NULL; loop < 2; loop++, pc = getAcsConnDesc()) {

      int         bufsz = 0;
      const char  *pp   = a->ud.paramNamesReq.parameterPath;
      TRxObjNode  *n;

      if (pp==NULL || strlen(pp)==0) { 
         n = rootDevice;
         pp = n->name;
         isFragment = 1;
      } else {
         n = findGWParameter(pp);
         isFragment = pp[strlen(pp)-1]=='.';
      }

      if (n) {
         /* path found: generate response */
         if (n->objDetail==NULL && (a->ud.paramNamesReq.nextLevel || isFragment)) {
            /* invalid paramter */
            acsState.fault = 9003;  
         } else {
            openEnvWithHeader(a->ID, pc, &bufsz);
            openBody(pc, &bufsz);
            xml_mIndent(pc,&bufsz, 3);
            mprintf(pc,&bufsz,"<%sGetParameterNamesResponse>\n", nsCWMP);
            xml_mIndent(pc,&bufsz,4);
            #ifdef SUPPRESS_SOAP_ARRAYTYPE
            mprintf(pc,&bufsz,"<ParameterList>\n");
            #else
            /* In the first loop paramCnt=0.  In the second loop, paramCnt will have
             * the actual parameter count.
             */
            mprintf(pc,&bufsz,
                    "<ParameterList %sarrayType=\"%sParameterInfoStruct[%04d]\">\n",
                    nsSOAP_ENC, nsCWMP, paramCnt);
            #endif
            paramCnt = 0;  /* reset paramCnt */
            strncpy(paramName, pp, PNAME_LTH);
            if (isFragment) {
               if (a->ud.paramNamesReq.nextLevel == 0) {
                  writeGetPName(n, &pc, &bufsz, 0);
               }
               traverseTree(n, (SoapOutCB)writeGetPName, (void **)&pc, &bufsz,
                            a->ud.paramNamesReq.nextLevel, isFragment);
            } else {
               writeGetPName(n, &pc, &bufsz, 0);
            }
            xml_mIndent(pc, &bufsz, 4);
            mprintf(pc,&bufsz,"</ParameterList>\n");
            xml_mIndent(pc,&bufsz,3);
            mprintf(pc,&bufsz,"</%sGetParameterNamesResponse>\n", nsCWMP);
            closeBodyEnvelope(pc, &bufsz);
         }
      } else {
         acsState.fault = 9005; /* Invalid Parameter Name */
      }
      if (acsState.fault != 0) {
         break;
      }

      if (loop == 0) {
         /* send HTTP message header */
         sendToAcs(bufsz, NULL);
      }
   } /* for (loop.... */
   if (acsState.fault != 0) {
      /* build fault here */
      writeSoapFault(a, acsState.fault);
      if (g_debug)
      fprintf(stderr, "Fault in GetParameterNames %d\n", acsState.fault);
   }
}  /* End of doGetParameterNames() */

/*
 * set the Attributes in the DopeDescriptor for the Node,instance.
 * Returns: 0 or fault value.
 * 
 */
static void setAttributes( TRxObjNode *n, void **p, int *ret, int fragment)
{
   InstanceDope   *pDope;
   AttributeItem  *pi = (AttributeItem *)p; 

   *ret = 0;
   if ( (pDope=findDopeInstance(n))) {
      /* found instance dope */
      if (pi->chgNotify) {
         if (!(pi->notification==ACTIVE_NOTIFICATION
              && n->paramAttrib.attrib.inhibitActiveNotify)) {
            pDope->notification = pi->notification;
            switch (pDope->notification) {
            case NOTIFICATION_OFF:
               // if notification is off then
               // free parameter value if it's not NULL
               if (pDope->pdata != NULL) {
                  free(pDope->pdata);
                  pDope->pdata = NULL;
               }
               break;
            case PASSIVE_NOTIFICATION:
            case ACTIVE_NOTIFICATION:
               // if it's active or passive notification
               // assign parameter value if it's NULL
               if (pDope->pdata == NULL) {
                  switch (n->paramAttrib.attrib.etype) {
                     case tInt:
                     case tUnsigned:
                     case tBool:
					 case tselect:
					 case tipv4:
                     case tString:
                     case tStringSOnly:
                     case tDateTime:
                     case tBase64:
                        if (n->getTRxParam) {
                           n->getTRxParam(&pDope->pdata);      
                        } else {
                           pDope->pdata = strdup("");
                        }                                                                
                        break;
                     default:
                        pDope->pdata = strdup("");
                        break;
                  }
               }
               break;
            default:
               break;
            }
         } else if (!fragment) {
            *ret = 9009;
         }
      }
      if (pi->chgAccess) {
         pDope->accessListIndex = pi->subAccess;
      }
   } else {
      if (n->name!=instanceIDMASK && n->paramAttrib.attrib.etype!=tInstance)
         *ret = 9002;
   }
}  /* End of setAttributes () */

static void doSetParameterAttributes(RPCAction *a)
{
   AttributeItem   *pi = a->ud.aItem;

   acsState.fault = 0;  /* init to no fault */

   // if attribute name is empty then return fault message
   if (pi == NULL) acsState.fault = 9009;

   /* first set attributes */
   while (pi!=NULL && !acsState.fault ) {

      int         isFragment = 0;
      TRxObjNode  *n;
      const char  *pp = pi->pname;

      if (pp==NULL || strlen(pp)==0) { 
         n = rootDevice;
         pp = n->name;
         isFragment = 1;
      }
      else {
         n = findGWParameter(pp);
         isFragment = pp[strlen(pp)-1]=='.';
      }
      if (n) {
         /* path found: apply attribute change */
         int status=0;
         if (n->name!=instanceIDMASK && n->paramAttrib.attrib.etype!=tInstance)
            setAttributes(n, (void **)pi, &status, isFragment);
         if (isFragment)
            traverseTree(n, setAttributes, (void **)pi, &status, 0, isFragment);
         acsState.fault = status;
      } else {
         acsState.fault = 9005; /* invalid parameter name */
      }
      pi = pi->next;
   }

   if (acsState.fault == 0) {

      int         loop;
      tProtoCtx   *pc;

      saveNotificationsFlag = 1;

      /* Loop through the code twice.  The first loop is to calculate
       * the size of the entire soap envelope so that the HTTP header
       * can be written to the TCP socket first.  The second loop is
       * to write the content of the envelope to the TCP socket as it
       * is generated.
       */
      for (loop = 0, pc = NULL; loop < 2; loop++, pc = getAcsConnDesc()) {

         int   bufsz = 0;

         /* build good response */
         openEnvWithHeader(a->ID, pc, &bufsz);
         openBody(pc, &bufsz);
         xml_mIndent(pc,&bufsz, 3);
         mprintf(pc,&bufsz,"<%sSetParameterAttributesResponse/>\n", nsCWMP);
         closeBodyEnvelope(pc, &bufsz);

         if (loop == 0) {
            /* send the HTTP message header */
            sendToAcs(bufsz, NULL);
         }
      }  /* for (loop.... */
   } else {
      writeSoapFault(a, acsState.fault);
      if (g_debug)
      fprintf(stderr, "Fault in SetParameterAttribute %d\n", acsState.fault);
   }
}  /* End of doSetParameterAttributes() */

/* 
* GetParameterAttributes requests a single parameter path or single parameter path fragment
* This RPC uses the paramItem union member. 
*/
static void doGetParameterAttributes(RPCAction *a)
{

   int         loop;
   int         cnt = 0; /* parameter counter -- need to count fragment name expansion */
   tProtoCtx   *pc = NULL;

   acsState.fault = 0; /* init to no fault */
   paramCnt       = 0;
             
   /* Loop through the code twice.  The first loop is to calculate
    * the size of the entire soap envelope so that the HTTP header
    * can be written to the TCP socket first.  The second loop is
    * to write the content of the envelope to the TCP socket as it
    * is generated.
    */
   for (loop = 0, pc = NULL; loop < 2; loop++, pc = getAcsConnDesc()) {

      int         bufsz  = 0;
      ParamItem   *pi    = a->ud.pItem;
      const char  *pname = NULL; /* pointer to bad name */

      if (pi != NULL) {
         /* create response msg start */
         openEnvWithHeader(a->ID, pc, &bufsz);
         openBody(pc, &bufsz);
         xml_mIndent(pc,&bufsz, 3);
         mprintf(pc,&bufsz,"<%sGetParameterAttributesResponse>\n", nsCWMP);
         xml_mIndent(pc,&bufsz,4);
      #ifdef SUPPRESS_SOAP_ARRAYTYPE
         mprintf(pc,&bufsz,"<ParameterList>\n");
      #else
         /* In the first loop cnt=0.  In the second loop, cnt will have
          * the actual parameter count.
          */
         mprintf(pc,&bufsz,
                 "<ParameterList %sarrayType=\"%sParameterAttributeStruct[%04d]\">\n",
                 nsSOAP_ENC, nsCWMP, cnt);
      #endif
         cnt      = 0; /* reset cnt */
         paramCnt = 0;

         /* step thru the requested parameter list some may be parameter fragments. */
         while (pi!=NULL && !acsState.fault ) {
             
            int        isFragment = 0;
            TRxObjNode *n;
            const char *pp = pi->pname;

            if (pp==NULL || strlen(pp)==0) { 
               n = rootDevice;
               pp = n->name;
               isFragment = 1;
            } else {
               n = findGWParameter(pp);
               isFragment = pp[strlen(pp)-1]=='.';
            }
            if (n) {
               /* path found: generate response */
			//   slog(TR69C_LOG_DEBUG, "<--******************************************************************-->\n"); 

               strncpy(paramName, pp, PNAME_LTH);
               if (isFragment ) {
                  traverseTree(n, (SoapOutCB)writeGetAttribute, (void **)&pc, &bufsz, 0, isFragment);
                  cnt += paramCnt;
               } else {
                  writeGetAttribute(n, &pc, &bufsz, 0);
                  ++cnt;
               }
			 //  slog(TR69C_LOG_DEBUG, "<--******************************************************************-->\n"); 

            } else {
               pname = pp;
               acsState.fault = 9005; /* invalid parameter name */
            }
            pi = pi->next;
         }
      } else {
         acsState.fault = 9003; /* no parameter specified - Invalid arguments */
      }

      if (acsState.fault != 0) {
         break;
      }

      /* good response */
      xml_mIndent(pc, &bufsz, 4);
      mprintf(pc,&bufsz,"</ParameterList>\n");
      xml_mIndent(pc,&bufsz,3);
      mprintf(pc,&bufsz,"</%sGetParameterAttributesResponse>\n", nsCWMP);
      closeBodyEnvelope(pc, &bufsz);

      if (loop == 0) {
         /* send HTTP message header */
         sendToAcs(bufsz, NULL);
      }
   }  /* for (loop..... */
   
   if (acsState.fault != 0) {
      /* build fault here */
      writeSoapFault(a, acsState.fault);
      if (g_debug)
      fprintf(stderr, "Fault in GetParameterAttribute %d\n", acsState.fault);
   }
}  /* End of doGetParameterAttributes */

static void doSetParameterValues(RPCAction *a)
{
   // keep the rebootFlag state of the previous RPC method
   int            setParamReboot = rebootFlag;
   char           *pname = NULL;
   TRxObjNode     *n;
   InstanceDope   *pDope;
   ParamItem      *pi = a->ud.pItem;

   acsState.fault = 0;      /* Init to no fault */

   /* first set parameters */
   if (pi != NULL) {
      while (pi != NULL) {

         TRX_STATUS  stat;

         pname = pi->pname; /* save name for fault msg */

         if ((n=findGWParameter(pname))) {
            /* found parameter node */
            if (n->setTRxParam!= NULL) {
               if (empty(pi->pvalue) && n->paramAttrib.attrib.etype!=tString) {
                  pi->fault = 9007;
                  acsState.fault = 9003;
               } else {
                  /* first save value for restore */
                  if (n->getTRxParam) {
                     if ( pi->pOrigValue != NULL ) {
                        free(pi->pOrigValue);
                        pi->pOrigValue = NULL;
                     }
                     /* ignore errors here */
                     n->getTRxParam( &pi->pOrigValue );
                  }
                  if (pi->pvalue==NULL) /* only valid for tSting params */
                     /* fake up a null string to avoid NULL ptr problem*/
                     pi->pvalue = strdup("");

                  if ((n->setTRxParam != NULL) &&
                      ((stat = n->setTRxParam(pi->pvalue))!=TRX_ERR)) {
                     if (stat==TRX_REBOOT)
                        setParamReboot = 1;

                     pi->fault = 0;
                     // tr69 spec. requires that if value is changed by ACS, the parameter that 
                     // has positive notification attribute should not be reported in Inform
                     // with "4 VALUE CHANGE" event. To support this requirement,
                     // if parameter keeps notification value since it has positive notification
                     // attribute then update old value to current one so that tr69c will not report
                     // "4 value change" event later. In getNotifyParams() pdata is compared
                     // with current parameter value, since pdata is updated at here, this parameter
                     // will be not in the notification change parameter list.
                     pDope = n->instanceDope;
                     if (pDope != NULL && pDope->pdata != NULL) {
                        free(pDope->pdata);
						pDope->pdata= NULL;
                        pDope->pdata = strdup(pi->pvalue);      /* 此处将新值设进去 add by zhanghaozhong*/
                     }
                     if (g_log_level)
                        slog(TR69C_LOG_INFO, "doGetParameterAttributes setting %s to value %s\n", pname, pi->pvalue);
                  }
                  else {
                     slog(TR69C_LOG_ERR, "Error setting %s to value %s\n", pname, pi->pvalue);
                     pi->fault = acsState.fault = 9002; /* Use Internal error for now */
                  }
               }
            } else {
               pi->fault = 9008;  /* non-writeable param */
               acsState.fault = 9003;
            }
         } else {
            pi->fault = 9005; /* invalid parameter name */
            acsState.fault = 9003; 
         }
         pi = pi->next;

      }  /* while (pi != NULL) */

      /* if fault above then restore orig parameter values */
      if (acsState.fault) {
         pi = a->ud.pItem;
         while (pi != NULL) {
            pname = pi->pname; /* save name for fault msg */
            if ((n=findGWParameter(pname))) {
               /* found parameter node */
               if (n->setTRxParam!= NULL) {
                  if (pi->pOrigValue==NULL)
                     pi->pOrigValue = strdup("");

                  n->setTRxParam(pi->pOrigValue);
                  pDope = n->instanceDope;
                  if (pDope != NULL && pDope->pdata != NULL) {
                     free(pDope->pdata);
					 pDope->pdata=NULL;
                     pDope->pdata = strdup(pi->pOrigValue);     /* 此处将旧值设进去 add by zhanghaozhong*/
                  }
               }
            }
            pi = pi->next;
         }
      }
   } else {
      acsState.fault = 9003; /* no parameter specified - Invalid arguments */
   }

   if (acsState.fault == 0) {

      int         loop;
      tProtoCtx   *pc;

    /* ACS下发配置文件 add by zhanghaozhong*/
   #ifdef MDM_DEVICE_CONFIG    
      /* if the parameter is InternetGatewayDevice.DeviceConfig.ConfigFile,
         then we do not want to save any configuration anymore because ACS
         sends a config file over, it wouldn't want the file to be overwritten.
         saveConfigFlag is not used because it could be flip back to 1 if
         there is another set after the config file set. This flag is cleared
         after reboot. */
      if (strcmp(pname,"InternetGatewayDevice.DeviceConfig.ConfigFile") == 0) {
         configFileSet = 1;
      } else {
         saveConfigFlag = 1;
      }
   #else            
      saveConfigFlag = 1;
   #endif   /* MDM_DEVICE_CONFIG */

      if (setParamReboot)
         rebootFlag = eACS_SETVALUEREBOOT;

      /* Loop through the code twice.  The first loop is to calculate
       * the size of the entire soap envelope so that the HTTP header
       * can be written to the TCP socket first.  The second loop is
       * to write the content of the envelope to the TCP socket as it
       * is generated.
       */
      for (loop = 0, pc = NULL; loop < 2; loop++, pc = getAcsConnDesc()) {

         int   bufsz = 0;

         /* build good response */
         openEnvWithHeader(a->ID, pc, &bufsz);
         openBody(pc, &bufsz);
         xml_mIndent(pc,&bufsz, 3);
         mprintf(pc,&bufsz,"<%sSetParameterValuesResponse>\n", nsCWMP);
         xml_mIndent(pc,&bufsz, 4);
         mprintf(pc,&bufsz,"<Status>%s</Status>\n", rebootFlag? "1":"0");
         xml_mIndent(pc,&bufsz, 3);
         mprintf(pc,&bufsz,"</%sSetParameterValuesResponse>\n", nsCWMP);
         closeBodyEnvelope(pc, &bufsz);

         if (loop == 0) {
            /* send the HTTP message header */
            sendToAcs(bufsz, NULL);
         }
      }  /* for (loop.... */
   } else {
      writeSoapFault(a, acsState.fault);
      if (g_debug)
      fprintf(stderr, "Fault in SetParameterValue %d for%s\n", acsState.fault, pname);
      if (g_log_level)
         slog(TR69C_LOG_INFO, "Fault in SetParameterValue %d for%s\n", acsState.fault, pname);
   }
}  /* End of doSetParameterValues() */

/*
* write SOAP value with type at buffer location **bp. Limit to size *bufsz.
 * increments paramCnt for each value written to buffer.
 *
*/
static void writeParamValueStruct(TRxObjNode *n, char *val, tProtoCtx *pc, int *bufsz)
{
   /* now fill in ParameterValueStruct in response */
   xml_mIndent(pc,bufsz, 6);
   mprintf(pc,bufsz,"<ParameterValueStruct>\n");
   xml_mIndent(pc,bufsz, 7);
   mprintf(pc,bufsz,"<Name>%s</Name>\n", paramName);
   xml_mIndent(pc,bufsz, 7);

   mprintf(pc,bufsz,"<Value %stype=\"%s%s\">",nsXSI, nsXSD,
         getValTypeStr(n->paramAttrib.attrib.etype));
   #ifdef SUPPRESS_EMPTY_PARAM
   if (empty(val)) 
      xml_mprintf(pc,bufsz,"empty");
   else
      xml_mprintf(pc,bufsz,val);
   #else
   xml_mprintf(pc,bufsz,val);
   #endif
   mprintf(pc,bufsz,"</Value>\n");
   xml_mIndent(pc,bufsz, 6);
   mprintf(pc,bufsz,"</ParameterValueStruct>\n");
   ++paramCnt;

}  /* End of writeParamValueStruct() */

static void writeGetPValue(TRxObjNode *n, tProtoCtx **pc, int *bufsz, int isFragment)
{
   if (n->name==instanceIDMASK)
      return;       /* instance node : no value to get */

   if (n->getTRxParam!= NULL) {
      char *val = NULL; /* getTRxParam allocated value buffer -- need to free it*/
                        /* init to NULL in case getter returns OK but no data*/
      if (n->paramAttrib.attrib.etype == tStringSOnly) {
         /* used to protect passwords, etc.Return null string */
         writeParamValueStruct(n,"",*pc,bufsz);
      } else if (n->getTRxParam(&val)!=TRX_OK) {
         acsState.fault = 9002; /* Use Internal error for now */
         slog(TR69C_LOG_ERR, "Error getting param value for %s\n", paramName);
      } else {
         /* now fill in ParameterValueStruct in response */
         slog(TR69C_LOG_DEBUG, "paramName:%s,value:%s", paramName,val);

         writeParamValueStruct(n,val,*pc,bufsz);
         free(val); /* clean up after getTRxParam */
		 val=NULL;
      }
   } else if (!isFragment )   /* if not walking tree then   */
      acsState.fault = 9005; /* non-existant read function */

}  /* End of writeGetPValue() */

static void doGetParameterValues(RPCAction *a)
{
   int         bufsz0 = 0;
   int         loop;
   tProtoCtx   *pc;

   acsState.fault = 0;  /* init to no fault */
   paramCnt       = 0;

   /* Loop through the code twice.  The first loop is to calculate
    * the size of the entire soap envelope so that the HTTP header
    * can be written to the TCP socket first.  The second loop is
    * to write the content of the envelope to the TCP socket as it
    * is generated.
    */
   for (loop = 0, pc = NULL; loop < 2; loop++, pc = getAcsConnDesc()) {

      int         bufsz  = 0;
      ParamItem   *pi    = a->ud.pItem;
      const char  *pname = NULL; /* pointer to bad name */

      if (pi != NULL) {

         /* create response msg start */
         openEnvWithHeader(a->ID, pc, &bufsz);
         openBody(pc, &bufsz);
         xml_mIndent(pc,&bufsz, 3);
         mprintf(pc,&bufsz,"<%sGetParameterValuesResponse>\n", nsCWMP);
         xml_mIndent(pc,&bufsz,4);
      #ifdef SUPPRESS_SOAP_ARRAYTYPE
         mprintf(pc,&bufsz,"<ParameterList>\n");
      #else
         /* In the first loop paramCnt=0.  In the second loop, paramCnt will have
          * the actual parameter count.
          */
         mprintf(pc,&bufsz,
                 "<ParameterList %sarrayType=\"%sParameterValueStruct[%04d]\">\n",
                 nsSOAP_ENC, nsCWMP, paramCnt);
      #endif
         paramCnt = 0; /* reset paramCnt */
         /* step thru the requested parameter list some may be parameter fragments. */
         while (pi!=NULL && !acsState.fault ) {

            int         isFragment = 0;
            const char  *pp        = pi->pname;
            TRxObjNode  *n;

            if (pp==NULL || strlen(pp)==0) { 
               n = rootDevice;
               pp = n->name;
               isFragment =1;
            } else {
             // slog(TR69C_LOG_DEBUG, "dgy_test   pp= %s\n", pp);
               n = findGWParameter(pp); /* 找到将要GET的实例或对象 add by zhanghaozhong*/
               isFragment = pp[strlen(pp)-1]=='.';
            }
            if (n) {
               /* path found: generate response */
               strncpy(paramName, pp, PNAME_LTH);
               if (isFragment)
                  traverseTree_m(n, (SoapOutCB)writeGetPValue, (void **)&pc, &bufsz, 0, isFragment);
               else if (n->name!=instanceIDMASK)
                  writeGetPValue(n, &pc, &bufsz, 0);
               else{
                  acsState.fault = 9005; /* can't get from an instance */
                  if (g_debug)
                    slog(TR69C_LOG_ERR, "invalid parameter name for %s\n", pp);
                }
            } else {
               pname = pp;
               acsState.fault = 9005; /* invalid parameter name */
               if (g_debug)
                    slog(TR69C_LOG_ERR, "invalid parameter name for %s\n", pp);
            }
            pi = pi->next;
         }
      } else {
         acsState.fault = 9003; /* no parameter specified - Invalid arguments */
      }

      if (acsState.fault != 0) {
         break;
      }

      /* good response */
      xml_mIndent(pc,&bufsz, 4);
      mprintf(pc,&bufsz,"</ParameterList>\n");
      xml_mIndent(pc,&bufsz, 3);
      mprintf(pc,&bufsz,"</%sGetParameterValuesResponse>\n", nsCWMP);
      closeBodyEnvelope(pc, &bufsz);
      
      if (loop == 0) {
         xml_mIndent(pc, &bufsz, MAX_PADDINGS);
         bufsz0 = bufsz;   /* save the envelope size */

         /* send HTTP message header */
         sendToAcs(bufsz, NULL);
      } else {
      	 slog(TR69C_LOG_ERR, " >>>>>  bufsz= %d bufsz0 = %d \n",bufsz,bufsz0);
         if (bufsz < bufsz0) {
            XMLmIndent(pc, &bufsz, bufsz0-bufsz);
         } 
      }
   } /* for (loop..... */

   if (acsState.fault != 0) {
      writeSoapFault(a, acsState.fault);
      if (g_debug)
      fprintf(stderr, "Fault in GetParameterValue %d\n", acsState.fault);
   }
}  /* End of doGetParameterValues()  */

/* AddObject
*
*/
/* 找到要添加对象的位置，并且判断此对象是否支持getTRxParam(AddObject) */
static void doAddObject(RPCAction *a)
{
   TRxObjNode *n;
   char       *pp    = a->ud.addDelObjectReq.objectName;
   char       *value = NULL;

   acsState.fault = 0;  /* init to no fault */

   if ((pp[strlen(pp)-1]=='.') && (n = findGWParameter(pp))) {
      if (n->paramAttrib.attrib.etype==tInstance) {
         TRxObjNode  *inode;
         TRX_STATUS  trStatus;

         if ((inode=n->objDetail) 
             && (inode->name==instanceIDMASK)
             && (inode->getTRxParam!=NULL)) {

            if ((trStatus = (inode->getTRxParam(&value))) != TRX_ERR) {

               int         loop;
               tProtoCtx   *pc;

               if (trStatus==TRX_REBOOT)
                  rebootFlag = eACS_ADDOBJECTREBOOT;

               saveConfigFlag = 1;

               /* Loop through the code twice.  The first loop is to calculate
                * the size of the entire soap envelope so that the HTTP header
                * can be written to the TCP socket first.  The second loop is
                * to write the content of the envelope to the TCP socket as it
                * is generated.
                */
               for (loop = 0, pc = NULL; loop < 2; loop++, pc = getAcsConnDesc()) {

                  int bufsz  = 0;

                  /* build AddObject response */
                  openEnvWithHeader(a->ID, pc, &bufsz);
                  openBody(pc, &bufsz);
                  xml_mIndent(pc, &bufsz, 3);
                  mprintf(pc, &bufsz, "<%sAddObjectResponse>\n", nsCWMP);
                  xml_mIndent(pc, &bufsz, 4);
                  mprintf(pc, &bufsz, "<InstanceNumber>%s</InstanceNumber>\n", value);
                  xml_mIndent(pc, &bufsz, 4);
                  mprintf(pc, &bufsz, "<Status>%s</Status>\n", rebootFlag? "1":"0" );
                  xml_mIndent(pc, &bufsz, 3);
                  mprintf(pc, &bufsz, "</%sAddObjectResponse>\n", nsCWMP);
                  closeBodyEnvelope(pc,&bufsz);
                  initNotification();

                  if (loop == 0) {
                     /* send HTTP message header */
                     sendToAcs(bufsz, NULL);
                  }
               }  /* for (loop..... */
               free (value);
			   value=NULL;
            } else {
               /* fault codes */
               acsState.fault = 9002;
            } /* TrStatus == TRX_ERR */
         } else {
            acsState.fault = 9003;
         }
      } /* (n->paramAttrib.attrib.etype==tInstance) */
      else {
         /* not an instance */
         acsState.fault = 9005;
      }
   } else {  /* if (pp[strlen(pp)-1]=='.') */
      acsState.fault = 9005;
   }

   if (acsState.fault) {
      writeSoapFault(a, acsState.fault);
   }
}  /* End of doAddObject() */ 

static TRX_STATUS deleteTree(TRxObjNode *node )
{
   InstanceDesc *idp;
   TRxObjNode  *n = node->objDetail;
   char   *instanceIDstr;
   TRX_STATUS trStatus = TRX_OK;
    
   if (n!=NULL && n->name==instanceIDMASK) {
      /* this is an object instance -- step thru the instances if present */
      idp = n->paramAttrib.instance;

      while (idp!=NULL && !acsState.fault && trStatus!= TRX_ERR) {
         pushInstance(idp);
         if (checkInstancePath(idp)) {
            trStatus = deleteTree(n);
            if (trStatus==TRX_REBOOT)
               rebootFlag = eACS_DELOBJECTREBOOT;
            saveConfigFlag = 1;
            if (n->setTRxParam != NULL) {
               instanceIDstr = strdup(itoa(idp->instanceID));
               trStatus      = n->setTRxParam( instanceIDstr);
               if (trStatus==TRX_REBOOT)
                  rebootFlag = eACS_DELOBJECTREBOOT;
               saveConfigFlag = 1;
               free(instanceIDstr);
			   instanceIDstr=NULL;
            }
         }
         popInstance();
         /* truncate paramName */
         idp = idp->next;
      }
      /* must return here, instance descriptor arrays are not null terminated */
      return trStatus;
   }
   while (n!=NULL && n->name!=NULL && !acsState.fault && trStatus!=TRX_ERR) {
      if (n->objDetail!=NULL) {
         trStatus = deleteTree(n);
         /* truncate paramName */
      }
      ++n;
   }
   return trStatus;

}  /* End of deleteTree() */

/* DeleteObject
*
*/

static void doDeleteObject(RPCAction *a)
{
   TRxObjNode  *n;
   char        *pp = a->ud.addDelObjectReq.objectName;
   int         instanceID;
   char        *instanceIDstr;
   TRX_STATUS  trStatus;

   if ((pp[strlen(pp)-1]=='.') && (n = findGWParameter(pp))) {
      if (n->name == instanceIDMASK 
          && n->setTRxParam != NULL
          && (instanceID = getCurrentInstanceID())!= -1) {

         if ((trStatus = deleteTree(n))!= TRX_ERR) {
            
            if (trStatus==TRX_REBOOT )
               rebootFlag = eACS_DELOBJECTREBOOT;

            instanceIDstr = strdup(itoa(instanceID));
            // Needed so checkInstancePath() succeeds.
            popInstance();

            if ((trStatus = n->setTRxParam( instanceIDstr)) != TRX_ERR) {

               int         loop;
               tProtoCtx   *pc;

               if (trStatus==TRX_REBOOT)
                  rebootFlag = eACS_DELOBJECTREBOOT;

               saveConfigFlag = 1;

               /* Loop through the code twice.  The first loop is to calculate
                * the size of the entire soap envelope so that the HTTP header
                * can be written to the TCP socket first.  The second loop is
                * to write the content of the envelope to the TCP socket as it
                * is generated.
                */
               for (loop = 0, pc = NULL; loop < 2; loop++, pc = getAcsConnDesc()) {

                  int bufsz  = 0;

                  /* build DeleteObject response */
                  openEnvWithHeader(a->ID, pc, &bufsz);
                  openBody(pc, &bufsz);
                  xml_mIndent(pc, &bufsz, 3);
                  mprintf(pc, &bufsz, "<%sDeleteObjectResponse>\n", nsCWMP);
                  xml_mIndent(pc, &bufsz, 4);
                  mprintf(pc, &bufsz, "<Status>%s</Status>\n", rebootFlag? "1":"0" );
                  xml_mIndent(pc, &bufsz, 3);
                  mprintf(pc, &bufsz, "</%sDeleteObjectResponse>\n", nsCWMP);
                  closeBodyEnvelope(pc,&bufsz);

                  if (loop == 0) {
                     /* send HTTP message header */
                     sendToAcs(bufsz, NULL);
                  }
               }  /* for (loop..... */
            } else { 
               /* fault codes */
               acsState.fault = 9002;
            }
            free (instanceIDstr);
			instanceIDstr=NULL;
         } else {
            acsState.fault = 9003;
         }
      } else {
          acsState.fault = 9003;
      }
   } else {
      acsState.fault = 9005;
   }

   if (acsState.fault) {
      writeSoapFault(a, acsState.fault);
   }
}  /* End of doDeleteObject() */

static void doRebootRPC(RPCAction *a)
{
   int         loop;
   tProtoCtx   *pc;
   rebootFlag = eACS_RPCREBOOT;

   /* Loop through the code twice.  The first loop is to calculate
    * the size of the entire soap envelope so that the HTTP header
    * can be written to the TCP socket first.  The second loop is
    * to write the content of the envelope to the TCP socket as it
    * is generated.
    */
   //收到重启命令后需要先发送响应报文，在执行重启这一动作
   for (loop = 0, pc = NULL; loop < 2; loop++, pc = getAcsConnDesc()) {

      int   bufsz = 0;
      openEnvWithHeader(a->ID, pc, &bufsz);
      openBody(pc, &bufsz);
      xml_mIndent(pc, &bufsz, 3);
      mprintf(pc, &bufsz, "<%sRebootResponse/>\n", nsCWMP);
      closeBodyEnvelope(pc, &bufsz);
      if (loop == 0) {
         /* send the HTTP message header */
         sendToAcs(bufsz, NULL);
		
      }
   }  /* for (loop.....) */
   	protect_connect_state_flag=1;
   	rebootFlag = eACS_RPCREBOOT;
	acsContactedState = eACS_RPCREBOOT;
	rebootCompletion();

}  /* End of doRebootRPC() */

static void doFactoryResetRPC(RPCAction *a)
{
   int         loop;
   tProtoCtx   *pc;

   /* Loop through the code twice.  The first loop is to calculate
    * the size of the entire soap envelope so that the HTTP header
    * can be written to the TCP socket first.  The second loop is
    * to write the content of the envelope to the TCP socket as it
    * is generated.
    */
   for (loop = 0, pc = NULL; loop < 2; loop++, pc = getAcsConnDesc())
   {
      int   bufsz = 0;

      openEnvWithHeader(a->ID, pc, &bufsz);
      openBody(pc, &bufsz);
      xml_mIndent(pc, &bufsz, 3);
      mprintf(pc, &bufsz, "<%sFactoryResetResponse/>\n", nsCWMP);
      closeBodyEnvelope(pc,&bufsz);

      if (loop == 0) {
         /* send the HTTP message header */
         sendToAcs(bufsz, NULL);
      }
   }  /* for (loop.....) */
   
   factoryResetFlag = 1;
   wrapperFactoryReset();
   slog(TR69C_LOG_ERR,"tr69cd factoryreset  ok ! ");

}  /* End of doFactoryResetRPC() */

/* Build an Inform msg        ???????????????? much more complicated 
* Probably will add a lot more args to the call 
*/
/*
TRXGFUNC(getManufacturer);
TRXGFUNC(getManufacturerOUI);
TRXGFUNC(getProductClass);
TRXGFUNC(getSerialNumber);
*/
/* myExtIP从int型改为char *型 */
void buildInform(RPCAction *a, InformEvList *infEvent, char* myExtIP)
{


   int         loop;
   int         bufsz0   = 0;
   tProtoCtx   *pc      = NULL;
   time_t      curtime  = time(NULL);

   acsState.fault = 0;  /* init to no fault */
   a->informID    = strdup(itoa(rand()));
   paramCnt       = 0;

   /* Loop through the code twice.  The first loop is to calculate
    * the size of the entire soap envelope so that the HTTP header
    * can be written to the TCP socket first.  The second loop is
    * to write the content of the envelope to the TCP socket as it
    * is generated.
    */
	
   for (loop = 0, pc = NULL; loop < 2; loop++, pc = getAcsConnDesc()) 
   	{

      int  i;
      int  bufsz = 0;
      char *val;

      openEnvWithHeader(a->informID, pc, &bufsz);
      openBody(pc, &bufsz);
      xml_mIndent(pc,&bufsz, 3);
      mprintf(pc,&bufsz,"<%sInform>\n", nsCWMP);
      xml_mIndent(pc,&bufsz, 4);
	  printf("buildInform   informID = %s\n",a->informID);
      /* create DeviceId structure */
      mprintf(pc, &bufsz,"<DeviceId>\n");
      getManufacturer(&val);     /* go direct to parameter since we know it here*/
      xml_mIndent(pc,&bufsz, 5);
      mprintf(pc, &bufsz,"<%s>%s</%s>\n","Manufacturer", val, "Manufacturer");
      free(val);
	  val=NULL;
      getManufacturerOUI(&val);  /* go direct to parameter since we know it here*/
      xml_mIndent(pc,&bufsz, 5);
      mprintf(pc, &bufsz,"<%s>%s</%s>\n", "OUI", val, "OUI");
      free(val);
	  val=NULL;
/*add by duanguiyuan 20160426: ProvisioningCode*/
	  xml_mIndent(pc,&bufsz, 5);
      mprintf(pc, &bufsz,"<%s></%s>\n","ProvisioningCode", "ProvisioningCode");    /* ProvisioningCode */
/*add by duanguiyuan : ProvisioningCode*/	  
      getProductClass(&val);     /* go direct to parameter since we know it here*/
      xml_mIndent(pc,&bufsz, 5);
      mprintf(pc, &bufsz,"<%s>%s</%s>\n","ProductClass", val, "ProductClass");
      free(val);
	  val=NULL;
      getSerialNumber(&val);     /* go direct to parameter since we know it here*/
      xml_mIndent(pc,&bufsz, 5);
      mprintf(pc, &bufsz,"<%s>%s</%s>\n","SerialNumber", val, "SerialNumber");
      free(val);
	  val=NULL;
      getCpeMac(&val);     /* go direct to parameter since we know it here*/
      xml_mIndent(pc,&bufsz, 5);
      mprintf(pc, &bufsz,"<%s>%s</%s>\n","CUI", val, "CUI");    /* CPE 唯一标示,MAC地址为标示 */
      free(val);
	  val=NULL;
      xml_mIndent(pc,&bufsz, 4);
      mprintf(pc, &bufsz,"</DeviceId>\n");
      xml_mIndent(pc,&bufsz, 4);

      #ifdef SUPPRESS_SOAP_ARRAYTYPE
      mprintf(pc, &bufsz,"<Event>\n");
      #else
      mprintf(pc, &bufsz,"<Event %sarrayType=\"%sEventStruct[%d]\">\n",
             nsSOAP_ENC, nsCWMP, infEvent->informEvCnt);
      #endif
	

	/*printf("[%s:%d] port_s 7\n",__func__,__LINE__);
	port_s=(char*)malloc(1065);
	printf("[%s:%d] port_s 8\n",__func__,__LINE__);
	free(port_s);
	port_s=NULL;
	printf("[%s:%d] port_s 9\n",__func__,__LINE__);*/
      for (i=0; i < infEvent->informEvCnt; ++i) { //创建 报文的 EventCode 部分

         char *ck = NULL;

         xml_mIndent(pc,&bufsz, 5);
         mprintf(pc, &bufsz,"<EventStruct>\n");
         xml_mIndent(pc,&bufsz, 6);
         mprintf(pc, &bufsz,"<EventCode>%s</EventCode>\n",
                  getInformEventStr(infEvent->informEvList[i], infEvent->mMethod) );
         xml_mIndent(pc,&bufsz, 6);
         if (infEvent->informEvList[i]==eIEMethodResult && infEvent->mMethod==rpcReboot ) 
            ck=acsState.rebootCommandKey;
         else if (infEvent->informEvList[i]==eIEMethodResult && infEvent->mMethod==rpcDownload ) 
            ck=acsState.downloadCommandKey;
//#ifdef RPC_UPLOAD    
         else if (infEvent->informEvList[i]==eIEMethodResult && infEvent->mMethod==rpcUpload ) 
            ck=acsState.downloadCommandKey;
//#endif   // RPC_UPLOAD            
         #ifdef SUPPRESS_EMPTY_PARAM
         if (empty(ck))
            mprintf(pc, &bufsz,"<CommandKey>empty</CommandKey>\n");
         else
            mprintf(pc, &bufsz,"<CommandKey>%s</CommandKey>\n", ck);
         #else
         	mprintf(pc, &bufsz,"<CommandKey>%s</CommandKey>\n", ck? ck: "");
         #endif
         xml_mIndent(pc,&bufsz, 5);
         mprintf(pc, &bufsz,"</EventStruct>\n");
      }
//printf("buildInform   2\n");
	  
      xml_mIndent(pc,&bufsz, 4);
      mprintf(pc, &bufsz,"</Event>\n");
      xml_mIndent(pc,&bufsz, 4);
      mprintf(pc, &bufsz,"<MaxEnvelopes>1</MaxEnvelopes>\n");
      xml_mIndent(pc,&bufsz, 4);
      mprintf(pc, &bufsz,"<CurrentTime>%s</CurrentTime>\n", getXSIdateTime(&curtime));
      xml_mIndent(pc,&bufsz, 4);
      mprintf(pc, &bufsz,"<RetryCount>%d</RetryCount>\n", acsState.retryCount);
      xml_mIndent(pc,&bufsz, 4);
      #ifdef SUPPRESS_SOAP_ARRAYTYPE
      mprintf(pc, &bufsz,"<ParameterList>\n");
      #else
      /* In the first loop paramCnt=0.  In the second loop, paramCnt will have
       * the actual parameter count.
       */
      mprintf(pc, &bufsz,
              "<ParameterList %sarrayType=\"%sParameterValueStruct[%04d]\">\n",
              nsSOAP_ENC, nsCWMP, paramCnt);
      #endif
      paramCnt = 0;  /* reset paramCnt */

		//informParameters[NUMREQINFORMPARMS-1] ="InternetGatewayDevice.WANDevice.1.WANConnectionDevice.2.WANPPPConnection.1.ExternalIPAddress";
	  	informParameters[NUMREQINFORMPARMS-1] ="InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.ExternalIPAddress";

   
			/* BEGIN: add by duanguiyuan 20161126 华为定制 告警上报功能 */
			int alar_num=6;
			if(g_alarm_info.alarm_flag == TR69C_ALARM_YES)
			{
				int i=0;
				for(i=0;i<TR69C_ALARM_TYPE_END ;i++)
				{
					//if (g_debug)
						printf("alarm param name (%s)\n",alarmParametersname[i]);
					if(g_alarm_info.alarm_status[i] == TR69C_ALARM_YES)
					{
						informParameters[NUMREQINFORMPARMS-2-alar_num+i]=alarmParametersname[i];
					}else{
						//informParameters[NUMREQINFORMPARMS-2-alar_num+i]=NULL;
					}
				}
				g_alarm_info.alarm_flag = TR69C_ALARM_NO;
			}
		
		/* END: add by duanguiyuan 20161126 华为定制 告警上报功能 */
	if(acsState.passAuthflag == 1)
	{
		informParameters[NUMREQINFORMPARMS-2] ="InternetGatewayDevice.UserInfo.Password";
	}
									/*ExternalIPAddress需要处理  by zhanghaozhong 2013-4-3*/

	  for (i=0;i<NUMREQINFORMPARMS; ++i) {
			if(informParameters[i]==NULL)
					continue;

         TRxObjNode *n; 
         if ((n=findGWParameter(informParameters[i]))) { 
        //    slog(TR69C_LOG_DEBUG, "informParameters:%s",informParameters[i]);
            /* found parameter node */
            /* need to verify that its an item check TRxType ?????????*/
            if (n->getTRxParam!= NULL) {
               char    *val=NULL; /* getTRxParam allocated value buffer -- need to free it*/
               if (n->getTRxParam(&val)!=TRX_OK) {
                  acsState.fault = 9002; /* Use Internal error for now */
               } else {
        //       slog(TR69C_LOG_DEBUG, "informParameters:%s,value:%s",informParameters[i],val);
                  strcpy(paramName,informParameters[i]); 
                  writeParamValueStruct(n,val,pc,&bufsz);
                  free(val);
				  val=NULL;
               }
            } else {
       //        slog(TR69C_LOG_DEBUG, "Unable to read %s",informParameters[i]);
               if (g_debug)
               fprintf(stderr, "Unable to read %s\n", informParameters[i]);
            }
         } else {
            if (g_debug)
            fprintf(stderr, "Unable to locate %s\n", informParameters[i]);
         }
      }
		
	  if (notifyChanges) {
         getAllNotifications(&pc, &bufsz);                
         /* notify changes are released after ACS response */
      }

      if (acsState.fault != 0) {
         break;
      }

      xml_mIndent(pc,&bufsz, 4);
      mprintf(pc, &bufsz,"</ParameterList>\n");
      /* BEGIN: Added by zhanghaozhong, 2013/5/24   PN:增加告警上报功能 */
      if (iAlarmPending){
          xml_mIndent(pc,&bufsz, 4);    /* soap报文里首行空6(3*2)个空格 */
          mprintf(pc,&bufsz,"<DeviceAlarm>\n");
          xml_mIndent(pc,&bufsz, 5);
          //mprintf(pc,&bufsz,"<AlarmCode>%d</AlarmCode>\n", g_stUdpMSG.stAlarm.uiAlarmCode);      /* 告警码 */
          //xml_mIndent(pc,&bufsz, 5);
          //mprintf(pc,&bufsz,"<AlarmReasonCode>%d</AlarmReasonCode>\n", g_stUdpMSG.stAlarm.uiAlarmReasonCode);/* 告警原因码 */
          //xml_mIndent(pc,&bufsz, 5);
          //mprintf(pc,&bufsz,"<AlarmObject>%s</AlarmObject>\n", g_stUdpMSG.stAlarm.acAlarmObject);      /* 告警对象标识 */
          //xml_mIndent(pc,&bufsz, 5);
          //mprintf(pc,&bufsz,"<AlarmInfo>%s</AlarmInfo>\n", g_stUdpMSG.stAlarm.acAlarmInfo);         /* 告警附加的具体信息 */
          //xml_mIndent(pc,&bufsz, 5);
          //mprintf(pc,&bufsz,"<AlarmState>%d</AlarmState>\n", g_stUdpMSG.stAlarm.uiAlarmState);       /* 告警状态 */
          //xml_mIndent(pc,&bufsz, 4);    /* soap报文里首行空6(3*2)个空格 */
          mprintf(pc,&bufsz,"</DeviceAlarm>\n");
      }
      /* END:   Added by zhanghaozhong, 2013/5/24   PN:增加告警上报功能 */
	  
      xml_mIndent(pc,&bufsz, 3);
      mprintf(pc,&bufsz,"</%sInform>\n", nsCWMP);
      closeBodyEnvelope(pc,&bufsz);

      if (loop == 0) {
         xml_mIndent(pc, &bufsz, MAX_PADDINGS);
         bufsz0 = bufsz;   /* save the envelope size */

         /* send HTTP message header */
         sendToAcs(bufsz, NULL);
      } else {
         if (bufsz < bufsz0) {
            XMLmIndent(pc, &bufsz, bufsz0-bufsz);
         } 
      }
   }  /* for (loop.....) */
   
   if (acsState.fault == 0) {
      if (transferCompletePending == 0) {
         eACSContactedState cState = getACSContactedState();
         if (cState == eACS_DOWNLOADREBOOT || cState == eACS_UPLOAD)
            transferCompletePending = 1;
      }
      acsState.retryCount++;  /* increment retry count -- reset to zero by inform response */
   }
}  /* End of buildInform() */

void sendGetRPCMethods(void)
{
   int         loop;
   tProtoCtx   *pc;

   if (g_debug)
   slog(TR69C_LOG_DEBUG, "sendGetRPCMethods\n");

   /* Loop through the code twice.  The first loop is to calculate
    * the size of the entire soap envelope so that the HTTP header
    * can be written to the TCP socket first.  The second loop is
    * to write the content of the envelope to the TCP socket as it
    * is generated.
    */
   for (loop = 0, pc = NULL; loop < 2; loop++, pc = getAcsConnDesc()) {

      int   bufsz = 0;

      openEnvWithHeader(NULL, pc, &bufsz);
      openBody(pc, &bufsz);
      xml_mIndent(pc,&bufsz, 3);
      mprintf(pc,&bufsz,"<%sGetRPCMethods/>\n", nsCWMP);
      closeBodyEnvelope(pc,&bufsz);

      if (loop == 0) {
         /* send the HTTP message header */
         sendToAcs(bufsz, NULL);
      }
   }  /* for (loop.....) */

}  /* End of sendGetRPCMethods() */

void sendTransferComplete(void)
{
   char        *ck;
   int         loop;
   tProtoCtx   *pc;

   if (g_debug)
   slog(TR69C_LOG_DEBUG, "sendTransferComplete\n");

   /* Loop through the code twice.  The first loop is to calculate
    * the size of the entire soap envelope so that the HTTP header
    * can be written to the TCP socket first.  The second loop is
    * to write the content of the envelope to the TCP socket as it
    * is generated.
    */
   for (loop = 0, pc = NULL; loop < 2; loop++, pc = getAcsConnDesc()) {

      int   bufsz = 0;

      openEnvWithHeader(NULL, pc, &bufsz);
      openBody(pc, &bufsz);
      xml_mIndent(pc,&bufsz, 3);
      mprintf(pc,&bufsz,"<%sTransferComplete>\n", nsCWMP);
      xml_mIndent(pc,&bufsz, 4);
      ck=acsState.downloadCommandKey;
      #ifdef SUPPRESS_EMPTY_PARAM
      if (ck && strlen(ck)>0)
         mprintf(pc, &bufsz,"<CommandKey>%s</CommandKey>\n", ck);
      else
         mprintf(pc, &bufsz,"<CommandKey>empty</CommandKey>\n");
      #else
      mprintf(pc, &bufsz,"<CommandKey>%s</CommandKey>\n", ck? ck: "");
      #endif
      xml_mIndent(pc, &bufsz, 4);
      mprintf(pc, &bufsz,"<FaultStruct>\n");
      xml_mIndent(pc, &bufsz, 5);
      mprintf(pc, &bufsz,"<FaultCode>%d</FaultCode>\n",acsState.dlFaultStatus);
      xml_mIndent(pc, &bufsz, 5);
	  if(acsState.dlFaultStatus == 0){
 			mprintf(pc, &bufsz,"<FaultString></FaultString>\n");
	  }else{
      mprintf(pc, &bufsz,"<FaultString>%s</FaultString>\n", acsState.dlFaultMsg?
      #ifdef SUPPRESS_EMPTY_PARAM
            acsState.dlFaultMsg: "empty");
      #else
            acsState.dlFaultMsg: "");
      #endif
	  }
      xml_mIndent(pc, &bufsz, 4);
      mprintf(pc, &bufsz,"</FaultStruct>\n");
      xml_mIndent(pc, &bufsz, 4);
      mprintf(pc, &bufsz,"<StartTime>%s</StartTime>\n", getXSIdateTime(&acsState.startDLTime));
      xml_mIndent(pc, &bufsz, 4);
      mprintf(pc, &bufsz,"<CompleteTime>%s</CompleteTime>\n", getXSIdateTime(&acsState.endDLTime));
      xml_mIndent(pc,&bufsz,3);
      mprintf(pc,&bufsz,"</%sTransferComplete>\n", nsCWMP);
      closeBodyEnvelope(pc,&bufsz);

      if (loop == 0) {
         /* send the HTTP message header */
         sendToAcs(bufsz, NULL);
      }
   }  /* for (loop.....) */

}  /* End of sendTransferComplete() */

static void  doDownload(RPCAction *a)
{
   int         loop;
   tProtoCtx   *pc;
   DownloadReq *r = &a->ud.downloadReq;
   
   if (a == rpcAction)
      rpcAction = NULL;  /* if *a is copy of rpcAction. set to NULL */

    if (g_debug)
    {
       fprintf(stderr, "doDownload: URL=%s\n", r->url);
       fprintf(stderr, "User/pw: %s:%s\n", r->user, r->pwd);
       fprintf(stderr, "Required memory buffer size will be %d\n", r->fileSize);
    }
    if (g_log_level)
    {
        slog(TR69C_LOG_INFO, "doDownload: URL=%s\n", r->url);
        slog(TR69C_LOG_INFO, "User/pw: %s:%s\n", r->user, r->pwd);
        slog(TR69C_LOG_INFO, "Required memory buffer size will be %d\n", r->fileSize);
    }
   /* build good response */
   /* Loop through the code twice.  The first loop is to calculate
    * the size of the entire soap envelope so that the HTTP header
    * can be written to the TCP socket first.  The second loop is
    * to write the content of the envelope to the TCP socket as it
    * is generated.
    */
   for (loop = 0, pc = NULL; loop < 2; loop++, pc = getAcsConnDesc()) {
      int   bufsz = 0;

      /* build good response */
      openEnvWithHeader(a->ID, pc, &bufsz);
      openBody(pc, &bufsz);
      xml_mIndent(pc,&bufsz, 3);
      mprintf(pc,&bufsz,"<%sDownloadResponse>\n", nsCWMP);

	  slog(TR69C_LOG_DEBUG, "<%sDownloadResponse>\n", nsCWMP);
	  
      xml_mIndent(pc, &bufsz, 4);
      mprintf(pc,&bufsz,"<Status>0</Status>\n");

      slog(TR69C_LOG_DEBUG,"<Status>1</Status>\n");

      xml_mIndent(pc, &bufsz, 4);
      mprintf(pc,&bufsz,"<StartTime>0001-01-01T00:00:00Z</StartTime>\n");

      slog(TR69C_LOG_DEBUG, "<StartTime>0001-01-01T00:00:00Z</StartTime>\n");

      xml_mIndent(pc, &bufsz, 4);
      mprintf(pc,&bufsz,"<CompleteTime>0001-01-01T00:00:00Z</CompleteTime>\n");

      slog(TR69C_LOG_DEBUG, "<CompleteTime>0001-01-01T00:00:00Z</CompleteTime>\n");

      xml_mIndent(pc,&bufsz, 3);
      mprintf(pc,&bufsz,"</%sDownloadResponse>\n", nsCWMP);
	  
	  slog(TR69C_LOG_DEBUG, "</%sDownloadResponse>\n", nsCWMP);

	  closeBodyEnvelope(pc,&bufsz);
      if (loop == 0) {
         /* send the HTTP message header */
         sendToAcs(bufsz, NULL);

      }
   }  /* for (loop.....) */
	  slog(TR69C_LOG_DEBUG, "start download... delay= %d \n",r->delaySec);  
   setTimer(downloadStart, (void *)a, (1+r->delaySec)*1000);
//  setTimer(downloadStart, (void *)a, 0);

slog(TR69C_LOG_DEBUG, "start download... end");  

}  /* doDownload() */

//#ifdef RPC_UPLOAD   

static void doUpload(RPCAction *a)
{
   int         loop;
   tProtoCtx   *pc;
   DownloadReq *r = &a->ud.downloadReq;
   
   if (a == rpcAction)
      rpcAction = NULL;  /* if *a is copy of rpcAction. set to NULL */

    if (g_debug)
    {
       fprintf(stderr, "UploadSetup: URL=%s\n", r->url);
       fprintf(stderr, "User/pw: %s:%s\n", r->user, r->pwd);
       fprintf(stderr, "Required memory buffer size will be %d\n", r->fileSize);
    }
    if (g_log_level)
    {
        slog(TR69C_LOG_INFO, "UploadSetup: URL=%s\n", r->url);
        slog(TR69C_LOG_INFO, "User/pw: %s:%s\n", r->user, r->pwd);
        slog(TR69C_LOG_INFO, "Required memory buffer size will be %d\n", r->fileSize);
    }
      
   /* build good response */
   /* Loop through the code twice.  The first loop is to calculate
    * the size of the entire soap envelope so that the HTTP header
    * can be written to the TCP socket first.  The second loop is
    * to write the content of the envelope to the TCP socket as it
    * is generated.
       */
   for (loop = 0, pc = NULL; loop < 2; loop++, pc = getAcsConnDesc()) {

      int   bufsz = 0;

      /* build good response */
      openEnvWithHeader(a->ID, pc, &bufsz);
      openBody(pc, &bufsz);
      xml_mIndent(pc,&bufsz, 3);
      mprintf(pc,&bufsz,"<%sUploadResponse>\n", nsCWMP);
      xml_mIndent(pc, &bufsz, 4);
      mprintf(pc,&bufsz,"<Status>1</Status>\n");
      xml_mIndent(pc, &bufsz, 4);
      mprintf(pc,&bufsz,"<StartTime>0001-01-01T00:00:00Z</StartTime>\n");
      xml_mIndent(pc, &bufsz, 4);
      mprintf(pc,&bufsz,"<CompleteTime>0001-01-01T00:00:00Z</CompleteTime>\n");
      xml_mIndent(pc,&bufsz, 3);
      mprintf(pc,&bufsz,"</%sUploadResponse>\n", nsCWMP);
      closeBodyEnvelope(pc,&bufsz);

      if (loop == 0) {
         /* send the HTTP message header */
         sendToAcs(bufsz, NULL);
	  slog(TR69C_LOG_INFO, "HTTP message header length: %d\n", bufsz);
 
      }
   }  /* for (loop.....) */

   slog(TR69C_LOG_INFO, "Upload beginning !!!");

   setTimer(uploadStart, (void *)a, (1+r->delaySec)*1000);
}  /* doUpload() */
//#endif   // RPC_UPLOAD
/*
 * Update the ACSState parameterKey or commandKeys if they are in
 * the RPC.
 */
void updateKeys(RPCAction *rpcAction)
{
   if (rpcAction->parameterKey) {
      if (rebootFlag == NOREBOOT) {
         /* not rebooting also copy to current parameter key */
         free(acsState.parameterKey);
		 acsState.parameterKey=NULL;
         acsState.parameterKey = strdup(rpcAction->parameterKey);
      }
      if (acsState.newParameterKey)
      	{
			free(acsState.newParameterKey);
			acsState.newParameterKey=NULL;
	
	  	}
         
      acsState.newParameterKey = rpcAction->parameterKey;
      rpcAction->parameterKey = NULL;
   }
   if (rpcAction->rpcMethod==rpcReboot && rpcAction->commandKey) {
      if (acsState.rebootCommandKey)
      	{
			free(acsState.rebootCommandKey);
			acsState.rebootCommandKey=NULL;
	  	}
         
      acsState.rebootCommandKey = rpcAction->commandKey;
      rpcAction->commandKey = NULL;
   }
}  /* End of updateKeys() */

RunRPCStatus runRPC_test(void)
{
   RunRPCStatus   ret = eRPCRunOK;

   informState    = eACSInformed;
   acsState.fault = 0;

   if (rpcAction != NULL) {
        if (g_log_level)
            slog(TR69C_LOG_INFO, "rpcAction->rpcMethod=%s\n", getRPCMethodName(rpcAction->rpcMethod));
      switch (rpcAction->rpcMethod) {
         case rpcGetRPCMethods:
            doGetRPCMethods(rpcAction);
            break;
         case rpcSetParameterValues:
            doSetParameterValues(rpcAction);
            break;
         case rpcGetParameterValues:
            doGetParameterValues(rpcAction);
            break;
         case rpcGetParameterNames:
            doGetParameterNames(rpcAction);
            break;
         case rpcGetParameterAttributes:
            doGetParameterAttributes(rpcAction);
            break;
         case rpcSetParameterAttributes:
            doSetParameterAttributes(rpcAction);
            break;
         case rpcAddObject:
            doAddObject(rpcAction);
            break;
         case rpcDeleteObject:
            doDeleteObject(rpcAction);
            break;
         case rpcReboot:
            doRebootRPC(rpcAction);
            break;
         case rpcFactoryReset:
            doFactoryResetRPC(rpcAction);
            break;
         case rpcDownload:
            doDownload(rpcAction);
            break;
//#ifdef RPC_UPLOAD    
         case rpcUpload:
            doUpload(rpcAction);
            break;
//#endif   // RPC_UPLOAD            
         case rpcInformResponse:
            uiInformState = 1;          /* 供web使用 1：已上报（已建立远程管理连接） */
            iAlarmPending = 0;          /* 清除告警标志 */
            if (g_debug)
                fprintf(stderr, "after rpcInformResponse iAlarmPending = %d\n", iAlarmPending);
            acsState.retryCount = 0;            /* must have been successful */
            if (transferCompletePending == 1) {
               /* make any callbacks that were setup when RPC started */
               sendTransferComplete();
               transferCompletePending = 0;
               // setACSContactedState to eACS_CONTACTED for clearing 
               // previous state which is eACS_DOWNLOADREBOOT or eACS_UPLOAD
               setACSContactedState(eACS_CONTACTED);
               saveACSContactedState();
            } else if (sendGETRPC) {
               sendGetRPCMethods();
               sendGETRPC = 0;
            } else { /* send empty message to indcate no more requests */
               sendNullHttp();   /* following a InformResponse send a NULL msg */
               ret = eRPCRunEnd;
            }
            initNotification();  /* update notifications following informResponse*/
            break;
         case rpcTransferCompleteResponse:
            sendNullHttp();   /* following a TransferCompleteResponse send a NULL msg */
            ret = eRPCRunEnd;
            break;
         case rpcGetRPCMethodsResponse:
            sendNullHttp();   /* following a GetRPCMethodsResponse send a NULL msg */
            ret = eRPCRunEnd;
            break;
         case rpcFault:
         default:
            ret = eRPCRunFail;
            break;
      }

      /* if no faults then update ACS state with parameter key or command key.*/
      if (ret!=eRPCRunFail && acsState.fault==NO_FAULT && rpcAction!=NULL ) {
         /* in the case of download this must wait until the download completes*/
         updateKeys(rpcAction);
      }
   }
   return ret;

}  /* End of runRPC() */

RunRPCStatus runRPC(void)
{
   RunRPCStatus   ret = eRPCRunOK;

   informState    = eACSInformed;
   acsState.fault = 0;

   if (!isAcsConnected())
      return eRPCRunFail;

   if (rpcAction != NULL) {
        if (g_log_level)
            slog(TR69C_LOG_INFO, "rpcAction->rpcMethod=%s\n", getRPCMethodName(rpcAction->rpcMethod));
      switch (rpcAction->rpcMethod) {
         case rpcGetRPCMethods:
            doGetRPCMethods(rpcAction);
            break;
         case rpcSetParameterValues:
            doSetParameterValues(rpcAction);
            break;
         case rpcGetParameterValues:
            doGetParameterValues(rpcAction);
            break;
         case rpcGetParameterNames:
            doGetParameterNames(rpcAction);
            break;
         case rpcGetParameterAttributes:
            doGetParameterAttributes(rpcAction);
            break;
         case rpcSetParameterAttributes:
            doSetParameterAttributes(rpcAction);
            break;
         case rpcAddObject:
            doAddObject(rpcAction);
            break;
         case rpcDeleteObject:
            doDeleteObject(rpcAction);
            break;
         case rpcReboot:
            doRebootRPC(rpcAction);
            break;
         case rpcFactoryReset:
            doFactoryResetRPC(rpcAction);
            break;
         case rpcDownload:
            doDownload(rpcAction);
            break;
//#ifdef RPC_UPLOAD    
         case rpcUpload:
            doUpload(rpcAction);
            break;
//#endif   // RPC_UPLOAD            
         case rpcInformResponse:
            uiInformState = 1;          /* 供web使用 1：已上报（已建立远程管理连接） */
            iAlarmPending = 0;          /* 清除告警标志 */
            if (g_debug)
                fprintf(stderr, "after rpcInformResponse iAlarmPending = %d\n", iAlarmPending);
            acsState.retryCount = 0;            /* must have been successful */
            if (transferCompletePending == 1) {
               /* make any callbacks that were setup when RPC started */
               sendTransferComplete();
               transferCompletePending = 0;
               // setACSContactedState to eACS_CONTACTED for clearing 
               // previous state which is eACS_DOWNLOADREBOOT or eACS_UPLOAD
	printf("write connection  state  3333 \n");
			   setACSContactedState(eACS_CONTACTED);
               saveACSContactedState();
            } else if (sendGETRPC) {
               sendGetRPCMethods();
               sendGETRPC = 0;
            } else { /* send empty message to indcate no more requests */
               sendNullHttp();   /* following a InformResponse send a NULL msg */
               ret = eRPCRunEnd;
            }
            initNotification();  /* update notifications following informResponse*/
            break;
         case rpcTransferCompleteResponse:
            sendNullHttp();   /* following a TransferCompleteResponse send a NULL msg */
            ret = eRPCRunEnd;
            break;
         case rpcGetRPCMethodsResponse:
            sendNullHttp();   /* following a GetRPCMethodsResponse send a NULL msg */
            ret = eRPCRunEnd;
            break;
         case rpcFault:
         default:
            ret = eRPCRunFail;
            break;
      }

      /* if no faults then update ACS state with parameter key or command key.*/
      if (ret!=eRPCRunFail && acsState.fault==NO_FAULT && rpcAction!=NULL ) {
         /* in the case of download this must wait until the download completes*/
         updateKeys(rpcAction);
      }
   }
   return ret;

}  /* End of runRPC() */

typedef int (*WalkTreeCB)(TRxObjNode *node, InstanceDesc *idp, void **pc, int *bufsz, int update);

static int   walkIndex; /* number of node visits- used to id node for saved attributes */
/*
 * getNotifyParams
 * idp is NULL if not instance parameter, such as, .DeviceInfo.ProvisioningCode.
 * idp points to instanceDesc for parameter. Framework stack must be valid.
 * If update flag is set then update the copy of the data with the new getTRxParam() data.
 * If buf is not NULL then call writeParamValueStruct to fill buf.if notification change found 
 * return 0: no notification parameter change found
 * return 1: notification found change found.
 *           If the parameter has an active notification flags set then activeNotifications is
 *           incremented.
 */
 /* 如果之前实例的值已经读出来，会存在内存里，这个函数会重新读取这个值，并且
    和之前的值做比较，如果不同则添加到inform报文里，发送给ACS，并且event
    code是4 VALUE CHANGE */
int getNotifyParams(TRxObjNode *n, InstanceDesc *idp, void **buf, int *bufsz, int update)
{
   char           *pValue = NULL;
   InstanceDope   *pDope,     /* pointer to Dope data chained off of parameter node*/
                  **pLast;    /* pointer to lask link variable. */
   tProtoCtx      **pc = (tProtoCtx **)buf;                  

   pLast = &n->instanceDope;
   if ( (pDope=n->instanceDope) ) {
      /* find dope that matches instance path */
      while ( pDope ) {
         if ( (idp==NULL && pDope->instance==NULL)
             || (pDope->instance==idp && checkInstanceStackPath(pDope->instance))) {
            /* found instance dope */
            if (pDope->notification>NOTIFICATION_OFF) {
               //fprintf(stderr, "getParam %s\n", paramName);
               if ( n->getTRxParam && n->getTRxParam(&pValue)==TRX_OK) {
                  if (pDope->pdata!=NULL) {
                     if (pValue!=NULL) {
                        if (strcmp(pValue, pDope->pdata)==0) {
                           /* parameter value has no change */
                           free(pValue);
						   pValue=NULL;
                           return 0;
                        } else {
                           /* parameter value is changed */
                    if (g_debug)
                        fprintf(stderr, "getNotifyParams - %s has value change from %s to %s\n", paramName, pDope->pdata, pValue);

                           if (pc && bufsz)
                              writeParamValueStruct(n, pValue, *pc, bufsz);
                           if (update) { /*replace Dope data if update is set*/
                              free (pDope->pdata);
							  pDope->pdata=NULL;
                              pDope->pdata = pValue;
                           } else
                              free(pValue);
						   pValue=NULL;
                           if (pDope->notification==ACTIVE_NOTIFICATION)
                              ++activeNotifications;
                           return 1;
                        }                                                                
                     }
                  } else {
                     /* only parameter notification is changed */
                    if (g_debug)
                        fprintf(stderr, "getNotifyParams - %s has notification change to %d\n", paramName, pDope->notification);

                     /* assign pDope->pdata to keep the current value */
                     if (update) /*replace Dope data if update is set*/
                        pDope->pdata = pValue;
                     return 0;
                  }
               } else
                  return 0;
            }else
               return 0;
         }
         pLast = &pDope->next;
         pDope = pDope->next;
      }
   }
   /* This is necessary following an AddObject */
   /* didn't find idp -- add it to either *n->instancedope or to end of dope list */
   if ( (pDope = (InstanceDope *) malloc(sizeof(struct InstanceDope))) ) {
      memset(pDope,0,sizeof(struct InstanceDope));
      *pLast = pDope;
      pDope->instance = idp;
      if (g_debug)  /* 每次进程启动时，或在此申请实例的内存 */
      fprintf(stderr, "New InstanceDope(%d) %s\n",walkIndex, paramName);
   }
   return 0;

}  /* End of getNotifyParams() */

/*
 * Inputs   *n - node pointer to start at.
 *         *idp - Current Instance descriptor or NULL.
 *         update - Flag to pass to getNotifyParams to indicate it should update is copy of 
 *               the save parameter data.
 *         buf - If not NULL the place to put the parameter data for notification (SOAP format).
 *         bufsz - size of buffer for parameter message struct entries.
 * static global:
 *         walkIndex - Should be set to zero before first call. Reflects the order in which
 *                nodes are visited. Used to remember a specific node for attribute storate.
 *         
* return 0: not found
*        >0: number of notify changes found
*/

static int walkTree(TRxObjNode *n, InstanceDesc *idp, WalkTreeCB callb, int update,
                    void **pc, int *bufsz)
{
   int            pathLth = strlen(paramName);
   int            changes = 0;
   InstanceDesc   *idpParent = idp;

   //fprintf(stderr, "searchTree %s paramPath=%s\n", n->name, paramPath);

   while (n && n->name) {
      pushNode(n);
      walkIndex++;
      if ( streq(n->name,instanceIDMASK)) {
         InstanceDesc    *idp;
         if ( (idp = n->paramAttrib.instance) ) {
            while (idp) {
               if (idp->parent == idpParent) {
                  pushInstance(idp);
                  sprintf(paramName, "%s%d.", paramName, idp->instanceID);
                  if (n->objDetail)
                     changes += walkTree(n->objDetail, idp, callb, update, pc, bufsz);
                  popInstance();
                  paramName[pathLth]='\0';
               }
               idp = idp->next;
            }
         } else if (n->objDetail) {
            /* walk to node past instance descriptor- primary purpose is to keep walkIndex the same */
            /* with or without instances */
            changes = walkTree(n->objDetail, NULL, NULL, 0, NULL, 0);
         }
         popNode();
         return changes;   /* return here instance nodes donot have a null terminator*/
      } else if (n->paramAttrib.attrib.etype==tObject
                || n->paramAttrib.attrib.etype==tInstance) {
         sprintf(paramName, "%s%s.", paramName, n->name);
         changes += walkTree(n->objDetail, idp, callb, update, pc, bufsz );
      } else if ( callb!=NULL) {
         sprintf(paramName, "%s%s", paramName, n->name);
         changes += callb(n,idp,pc,bufsz,update);
      }
      paramName[pathLth]='\0';
      ++n;  /* try next item in node list */
      popNode();
   }
        
   return changes;

}  /* End of walkTree() */

void resetNotificationBuf(void)
{
   notifyChanges = 0;
}
int checkActiveNotifications(void)
{
   int active;
        
   walkIndex = activeNotifications = 0;
   paramName[0] = '\0';
   walkTree(rootDevice, NULL, (WalkTreeCB)getNotifyParams, 0, NULL, NULL);
   active = activeNotifications;
   activeNotifications = 0;
   if (g_debug)
   fprintf(stderr, "checkActiveNotificaton active = %d\n", active);
        
   return active;
}
/*
* Get all notifications before inform.
* do not update dope data until inform response has been received.
*/
int getAllNotifications(tProtoCtx **pc, int *bufsz)
{
   walkIndex = notifyChanges = activeNotifications = 0;
   paramName[0] = '\0';
   notifyChanges = walkTree(rootDevice, NULL, (WalkTreeCB)getNotifyParams, 0/* no updatecopies*/,
                            (void **)pc, bufsz);
   if (g_debug)
   fprintf(stderr, "getAllNotificaton notifyChanges = %d\n", notifyChanges);
        
   return notifyChanges;

}  /* End of getAllNotifications() */

/*
* called on start up, following InformResponse and whenever the ACS sets an Attribute
 * , adds or delete objects or sets parameter values.
*/
void initNotification(void)
{
   walkIndex = activeNotifications = notifyChanges = 0;
   paramName[0] = '\0';
   clearStacks();
   walkTree(rootDevice, NULL, (WalkTreeCB)getNotifyParams, 1/*update*/, NULL, NULL);

}  /* End of initNotification() */

/* callback to save node attributes in save buffer */
int saveNotifyAttr(TRxObjNode *n, InstanceDesc *idp, void **buf,
                          int *bufsz, int update)
{
   /* some recasting needed for pointers */
   AttEntry   *ap = *(AttEntry **)buf;
   InstanceDope *pDope = findDopeInstance(n);
   if (pDope) {
      if ( (pDope->notification > NOTIFICATION_OFF 
           && *bufsz>sizeof(struct AttEntry)) ) {
         /* we only care about parameter with notification values */
         ap->nodeIndex = walkIndex;;
         ap->instanceId = idp==NULL? 0: idp->instanceID;
         ap->attrValue = pDope->notification;
         *buf += sizeof(struct AttEntry);
         *bufsz -= sizeof(struct AttEntry);
         if (g_debug)
         fprintf(stderr, "svAttr %s at %d\n",paramName, ap->nodeIndex);
         return 1;
      }
   } 
   return 0;

}  /* End of saveNotifyAttr() */
#if 0

/*
 * callback to restore Notify attributes:
 * The walkTree call this function for each node. This function scans the 
 * retrieved notify table to attempt to match the walkIndex to a AttEntry. If
 * an entry matches the attributes are applied to the node and a 1 is returned
 * as the function value. If no indexs match a 0 is returned.
 */
int restoreNodeNotifyAttr(TRxObjNode *n, InstanceDesc *idp, void **buf,
                                 int *bufsz, int update)
{
   int i;
   /* some recasting needed for pointers */
   AttSaveBuf   *ap = *(AttSaveBuf **)buf;
   InstanceDope *pDope = findDopeInstance(n);
   for (i=0; i<ap->numAttSaved; ++i) {
      AttEntry *e = &ap->attEntry[i];
      if ( walkIndex == e->nodeIndex) {
         if (!pDope ) {
            fprintf(stderr, "Notification initialization error- no InstanceDope for %s.%d\n",
                   n->name, idp?idp->instanceID:0);
            return 0;
         }
         if (n->paramAttrib.attrib.etype == tString
            || n->paramAttrib.attrib.etype == tInt
            || n->paramAttrib.attrib.etype == tUnsigned
            || n->paramAttrib.attrib.etype == tBool
            || n->paramAttrib.attrib.etype == tDateTime
            || n->paramAttrib.attrib.etype == tBase64 ) {
            /* passed simple validation */
            pDope->notification = e->attrValue;
            if (g_debug)
            fprintf(stderr, "rdAttr %s %d=%d\n",paramName, e->nodeIndex, e->attrValue);
            return 1;
         } else
            slog(LOG_ERROR, "Unable to set notification attribute for %s\n", paramName);
         return 0;
      }
   }
   return 0;

}  /* End of restoreNodeNotifyAttr() */

int restoreNotificationAttr(void)
{
   AttSaveBuf  *ap;
   int         bufSz;
   int         attRestored;

   if (tr69RetrieveFromStore(&ap, &bufSz)) {
      if (g_debug)
      {
         int i;
         for (i=0; i<ap->numAttSaved; ++i) {
            fprintf(stderr, "nodeIndex[%d] = %d\n", i, ap->attEntry[i].nodeIndex);
         }
      }

      paramName[0] = '\0';
      walkIndex = 0;
      attRestored = walkTree(rootDevice,NULL, (WalkTreeCB)restoreNodeNotifyAttr, 0,
                             (void **)&ap, &bufSz);
      free(ap);
      if (g_debug)
      fprintf(stderr, "Parameter Attributes restored from scratch pad = %d\n", attRestored);

      return 1;
   }
   slog(LOG_ERROR, "Stored Parameter Attribute data is corrupt or missing");
   return 0;

}  /* End of restoreNotificationAttr() */

/* 
 */
void saveNotificationAttributes(void)
{
   void        *buf;
   AttSaveBuf  *ap;
   char        *ps;
   int         bufSz;

   if ( saveNotificationsFlag) {
      saveNotificationsFlag = 0;
      if ( (buf = (char *)malloc(ATTRIBUTESAVESZ))) {
         memset(buf,0,(ATTRIBUTESAVESZ));
         ap = (AttSaveBuf *)buf;
         ps = (char *)&ap->attEntry;
         bufSz = ATTRIBUTESAVESZ-sizeof(struct AttSaveBuf);
         paramName[0] = '\0';
         walkIndex = 0;
         clearStacks();
         ap->numAttSaved = walkTree(rootDevice,NULL, (WalkTreeCB)saveNotifyAttr, 1,
                                    (void **)&ps, &bufSz);
        if (g_debug)
        slog(TR69C_LOG_INFO, "tr69SaveToStore will exe!\n");
         tr69SaveToStore(ap);
         free(buf);
      }
   }
   return;

}  /* End of saveNotificationAttributes() */
#endif

