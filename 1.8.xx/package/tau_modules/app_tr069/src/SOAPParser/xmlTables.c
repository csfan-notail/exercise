/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : xmlTables.c
  版 本 号   : 初稿
  作    者   : zhanghaozhong
  生成日期   : 2013年5月16日
  最近修改   :
  功能描述   : SOAP xmlTables and data structures
  函数列表   :
              fAccessList
              fAcclistChange
              fAddObject
              fBody
              fCommandKey
              fDeleteObject
              fDLCommandKey
              fDownload
              fDownloadDelaySec
              fDownloadFileSize
              fDownloadFileType
              fDownloadPwd
              fDownloadURL
              fDownloadUser
              fEnvelope
              fFactoryReset
              fFault
              fFaultCode
              fFaultString
              fGetParameterAttributes
              fGetParameterNames
              fGetParameterValues
              fGetRPCMethods
              fGetRPCMethodsResponse
              fHoldRequest
              fIDValue
              fInformResponse
              fMaxEnvelopes
              fNextLevel
              fNoMoreRequests
              fNotification
              fNotificationChange
              fObjectName
              fParameterNameArraySize
              fParameterNames
              fParameterPath
              fParameterValue
              fReboot
              fSetAttParameterName
              fSetParameterAttributes
              fSetParameterAttributesList
              fSetParameterKey
              fSetParameterValues
              fTargetFileName
              fTransferCompleteResponse
              fUpload
              fUploadFileType
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



#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/utsname.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/errno.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <time.h>
#include <syslog.h>

#include "tr69cdefs.h"   /* defines for RPCAction and ACSState */
#include "utils.h"
#include "nanoxml/nanoxml.h"
#include "RPCState.h"
#include "xmlTables.h"
#include "informer.h"

extern RPCAction   *rpcAction;
extern ACSState acsState;

/* List of namespaces recognized by this parser */
/* Prefixes are set by each message */
/* !!!!! Notice change MACROS if this table is changed */
NameSpace nameSpaces[] = {
    {NULL, "SOAP-ENV:", "http://schemas.xmlsoap.org/soap/envelope/"},
    {NULL, "SOAP-ENC:", "http://schemas.xmlsoap.org/soap/encoding/"},
    {NULL, "xsd:",      "http://www.w3.org/2001/XMLSchema"}, 
    {NULL, "xsi:",      "http://www.w3.org/2001/XMLSchema-instance"},
    {NULL, "cwmp:",     "urn:dslforum-org:cwmp-1-0"},
    {NULL, NULL,NULL}
};

/* MACROS for referencing the above namespace */
/* strings from xml node description tables   */
/* must match initializations in xmlTables */
#define iSOAP       &nameSpaces[0]
#define iSOAP_ENC   &nameSpaces[1]
#define iXSD        &nameSpaces[2]
#define iXSI        &nameSpaces[3]
#define iCWMP       &nameSpaces[4]
/* */
/* Node descriptors for SOAP Header */
static XmlNodeDesc mustDesc[] = {
    {iSOAP, "mustUnderstand",NULL,NULL},
    {iSOAP, "actor", NULL,NULL}, /* not sure about this-- ignore attr value */
    {iXSI,	"type", NULL,NULL},
	{NULL, NULL}
};

XMLFUNC(fIDValue);
XMLFUNC(fHoldRequest);
XMLFUNC(fNoMoreRequests);
static XmlNodeDesc headerDesc[] = {
    {iCWMP, "ID", fIDValue, mustDesc},               /* data value */
    {iCWMP, "HoldRequests",fHoldRequest, mustDesc}, /* data value */
    {iCWMP, "NoMoreRequests", fNoMoreRequests,NULL}, /* data value flag */
    {NULL, NULL}
};

/* Node descriptors for body */

/* fault codes from ACS */
XMLFUNC(fFaultCode);
XMLFUNC(fFaultString);
static XmlNodeDesc faultDesc[] = {
    {NULL,  "faultCode", fFaultCode, NULL},               /* data value */
    {NULL,  "faultString",fFaultString, NULL}, /* data value */
    {NULL, NULL}
};

/* <cmsp:Reboot>
*/
XMLFUNC(fMaxEnvelopes);
static XmlNodeDesc informResponseDesc[] = {
    {NULL,  "MaxEnvelopes", fMaxEnvelopes, NULL},               /*  */
    {NULL, NULL}
};
/* <cmsp:Reboot>
*/

XMLFUNC(fCommandKey);
static XmlNodeDesc rebootDesc[] = {
    {NULL,  "CommandKey", fCommandKey, NULL},               /*  */
    {NULL, NULL}
};
/* <cmwp:Download >
*/

XMLFUNC(fDownloadFileType);
XMLFUNC(fDLCommandKey);
XMLFUNC(fDownloadURL);
XMLFUNC(fDownloadUser);
XMLFUNC(fDownloadPwd);
XMLFUNC(fDownloadFileSize);
XMLFUNC(fTargetFileName);
XMLFUNC(fDownloadDelaySec);
static XmlNodeDesc downLoadDesc[] = {
    {NULL,  "CommandKey", fDLCommandKey,NULL},
    {NULL,  "FileType", fDownloadFileType,NULL},
    {NULL,  "URL", fDownloadURL,NULL},
    {NULL,  "Username", fDownloadUser,NULL},
    {NULL,  "Password", fDownloadPwd,NULL},
    {NULL,  "FileSize", fDownloadFileSize,NULL},
    {NULL,  "TargetFileName", fTargetFileName,NULL},
    {NULL,  "DelaySeconds", fDownloadDelaySec,NULL},
    {NULL,  "SuccessURL", NULL,NULL},
    {NULL,  "FailureURL", NULL,NULL},
    {NULL,NULL}
};
/* <cmwp:Upload >
*/

//#ifdef RPC_UPLOAD    
XMLFUNC(fUploadFileType);
static XmlNodeDesc upLoadDesc[] = {
    {NULL,  "CommandKey", fDLCommandKey,NULL},
    {NULL,  "FileType", fUploadFileType,NULL},
    {NULL,  "URL", fDownloadURL,NULL},
    {NULL,  "Username", fDownloadUser,NULL},
    {NULL,  "Password", fDownloadPwd,NULL},
    {NULL,  "FileSize", fDownloadFileSize,NULL},
    {NULL,  "TargetFileName", fTargetFileName,NULL},
    {NULL,  "DelaySeconds", fDownloadDelaySec,NULL},
    {NULL,  "SuccessURL", NULL,NULL},
    {NULL,  "FailureURL", NULL,NULL},
    {NULL,NULL}
};
//#endif   // RPC_UPLOAD
/* <cwmp:DeleteObject> 
*  <cwmp:AddObject>
*/

XMLFUNC(fObjectName);
XMLFUNC(fSetParameterKey);
static XmlNodeDesc addDelObjectDesc[] = {
    {NULL,  "ObjectName", fObjectName,NULL},
    {NULL,  "ParameterKey", fSetParameterKey, NULL},
    {NULL,NULL}
};

/* <cwmp:GetParameterValues ..../> */
/*
 * NOTE: fParameterNames is shared by multiple nodes.
 *       arrayType is used generically for determining array size.
* <GetParameterValues>
 *  <ParameterNames arrayType="xsd:string[x]>
 *      <xsd:string>name1</xsd:string>
 *      .....
 *  </ParameterNames>
 * </GetParameterValues>
 */
XMLFUNC(fParameterNameArraySize);
XMLFUNC(fParameterNames);
XMLFUNC(fGetParameterValues);
static XmlNodeDesc pNamesDesc[] = {
    {iSOAP, "arrayType", fParameterNameArraySize,NULL},
    {iXSD,  "string", fParameterNames,NULL},    /* namevalue is TAGDATA */
    {iCWMP, "item", fParameterNames,NULL},    /* namevalue is TAGDATA -- added for dimark */
    {iCWMP, "ParameterNames", fParameterNames,NULL},    /* namevalue is TAGDATA -- added for dimark */
    {NULL,NULL}
};
static XmlNodeDesc getParameterValuesDesc[] = {
    {NULL,  "ParameterNames", NULL,pNamesDesc},
    {NULL,NULL}
};

/* <SetParameterValue>
*   <ParameterList <ParameterValueStruct>
*                       <Name>xxxx</Name> <Value>yyyy</value>
*   </ParamterList>
*   <ParameterKey>kkkkk</ParameterKey>
*/
XMLFUNC(fParameterValue );
XMLFUNC(fSetParameterValues );

static XmlNodeDesc valueDesc[] = {
    {iXSD, "type", NULL, NULL},         
    {NULL,NULL}
};
static XmlNodeDesc parameterValueStructDesc[] = {
    {NULL, "Name", fParameterNames, NULL},
    {NULL, "Value", fParameterValue, valueDesc},
    {NULL,NULL}
};


/* <SetParameterAttributes>
*  <ParameterList <SetParameterAttributesStruct>
*               <Name>xxx</Name>  ....
*/

XMLFUNC(fSetAttParameterName );
XMLFUNC(fNotification);
XMLFUNC(fNotificationChange );
XMLFUNC(fAcclistChange);
XMLFUNC(fAccessList);
static XmlNodeDesc accessListDesc[] = {
	{NULL, "string", fAccessList, NULL},	/* only subscriber defined */
	{NULL, "arrayType", NULL,NULL},
	{NULL,NULL}
};
static XmlNodeDesc setparameterAttriStructDesc[] = {
    {NULL, "Name", fSetAttParameterName, NULL},
    {NULL, "Notification", fNotification, NULL},
    {NULL, "NotificationChange", fNotificationChange, NULL},
    {NULL, "AccessListChange", fAcclistChange, NULL},
    {NULL, "AccessList", NULL, accessListDesc},
    {NULL,NULL}
};

XMLFUNC(fSetParameterAttributesList);
/* this is being used by multiple complex types -- will not detect mixed element errors*/
static XmlNodeDesc parameterListDesc[] = {
    {iSOAP, "arrayType", fParameterNameArraySize, NULL},
    {NULL, "ParameterValueStruct",NULL,parameterValueStructDesc},
    {NULL, "SetParameterAttributesStruct",fSetParameterAttributesList,setparameterAttriStructDesc},
    {NULL,NULL}
};
static XmlNodeDesc setParameterValuesDesc[] = {
    {NULL, "ParameterList", NULL, parameterListDesc},
    {NULL, "ParameterKey", fSetParameterKey, NULL},
    {NULL,NULL}
};
/*
*/
static XmlNodeDesc setParameterAttributesDesc[] = {
    {NULL, "ParameterList", NULL, parameterListDesc},
    {NULL, "ParameterKey", fSetParameterKey, NULL},
    {NULL,NULL}
};
/*
*/
static XmlNodeDesc getParameterAttributesDesc[] = {
    {NULL, "ParameterNames", NULL, pNamesDesc},
    {NULL,NULL}
};

/*
*/
XMLFUNC(fParameterPath);
XMLFUNC( fNextLevel);
static XmlNodeDesc getParameterNamesDesc[] = {
    {NULL,  "ParameterPath", fParameterPath, NULL},               /* data value */
    {NULL,  "NextLevel",fNextLevel, NULL}, /* data value */
    {NULL, NULL}
};

XMLFUNC(fFault);
XMLFUNC(fGetRPCMethods);
XMLFUNC(fGetParameterNames);
XMLFUNC(fSetParameterAttributes);
XMLFUNC(fGetParameterAttributes);
XMLFUNC(fAddObject);
XMLFUNC(fDeleteObject);
XMLFUNC(fDownload);
//#ifdef RPC_UPLOAD    
XMLFUNC(fUpload);
//#endif
XMLFUNC(fReboot);
XMLFUNC(fInformResponse);
XMLFUNC(fFactoryReset);
XMLFUNC(fTransferCompleteResponse);
XMLFUNC(fGetRPCMethodsResponse);
static XmlNodeDesc bodyDesc[] = {
    {iSOAP, "Fault", fFault, faultDesc}, 
    {iCWMP, "GetRPCMethods", fGetRPCMethods, NULL},
    {iCWMP, "GetParameterNames", fGetParameterNames, getParameterNamesDesc},
    {iCWMP, "GetParameterValues", fGetParameterValues, getParameterValuesDesc},
    {iCWMP, "SetParameterValues", fSetParameterValues, setParameterValuesDesc},
    {iCWMP, "SetParameterAttributes", fSetParameterAttributes, setParameterAttributesDesc},
    {iCWMP, "GetParameterAttributes", fGetParameterAttributes, getParameterAttributesDesc},
    {iCWMP, "AddObject", fAddObject, addDelObjectDesc},
    {iCWMP, "DeleteObject", fDeleteObject, addDelObjectDesc},
    {iCWMP, "Download", fDownload, downLoadDesc},
//#ifdef RPC_UPLOAD    
    {iCWMP, "Upload", fUpload, upLoadDesc},
//#endif    
    {iCWMP, "Reboot", fReboot, rebootDesc},
    {iCWMP, "InformResponse", fInformResponse, informResponseDesc},
    {iCWMP, "FactoryReset", fFactoryReset, NULL},
    {iCWMP, "TransferCompleteResponse", fTransferCompleteResponse, faultDesc},
    {iCWMP, "GetRPCMethodsResponse", fGetRPCMethodsResponse, NULL},	/* ignore content*/
    {NULL, NULL}
};

//XMLFUNC(fHeader);
XMLFUNC(fBody);
static XmlNodeDesc envDesc[] = {
    {iSOAP, "Header", NULL, headerDesc},
    {iSOAP, "Body", fBody, bodyDesc},
    {NULL, NULL}
};
/* TopLevel node for a SOAP envelope */
XMLFUNC(fEnvelope);
XmlNodeDesc envelopeDesc[] = {
    {iNULL,"Envelope",fEnvelope, envDesc}, /* -1 is namespace exception flag*/
    {NULL, NULL}
};

static XML_STATUS fFaultString(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGBEGIN) {
        }
    else {
    }
    return XML_STS_OK;
}
static XML_STATUS fFaultCode(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGBEGIN) {
        }
    else {
    }
    return XML_STS_OK;
}
static XML_STATUS fFault(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGBEGIN) {
        rpcAction->rpcMethod = rpcFault;
        }
    else {
    }
    return XML_STS_OK;
}
/* <Download>
*/

static XML_STATUS fDownload(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGBEGIN)
        rpcAction->rpcMethod = rpcDownload;
    return XML_STS_OK;
}
static XML_STATUS fDLCommandKey(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGDATA){
        rpcAction->ud.downloadReq.commandKey = strdup(value);
    }
    return XML_STS_OK;
}
static XML_STATUS fDownloadFileType(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGDATA)
        rpcAction->ud.downloadReq.efileType = atoi(value);
    return XML_STS_OK;
}
static XML_STATUS fDownloadURL(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGDATA)
        rpcAction->ud.downloadReq.url = strdup(value);
    return XML_STS_OK;
}
static XML_STATUS fDownloadUser(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGDATA)
        rpcAction->ud.downloadReq.user = strdup(value);
    return XML_STS_OK;
}
static XML_STATUS fDownloadPwd(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGDATA)
        rpcAction->ud.downloadReq.pwd = strdup(value);
    return XML_STS_OK;
}
static XML_STATUS fDownloadFileSize(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGDATA)
        rpcAction->ud.downloadReq.fileSize = atoi(value);
    return XML_STS_OK;
}
static XML_STATUS fTargetFileName(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGDATA)
        rpcAction->ud.downloadReq.fileName = strdup(value);
    return XML_STS_OK;
}
static XML_STATUS fDownloadDelaySec(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGDATA)
        rpcAction->ud.downloadReq.delaySec = atoi(value);
    return XML_STS_OK;
}

/* <Upload>
*/

//#ifdef RPC_UPLOAD    
static XML_STATUS fUpload(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGBEGIN)
        rpcAction->rpcMethod = rpcUpload;
    return XML_STS_OK;
}
/* 处理来自ACS请求上传的文件类型 */
/* X ACE215 Diagnosis File */
/*
	add by duanguiyuan 20161121
	添加华为定制需求
	
*/
#define LOG_WORLD "ACE215 Diagnosis"	//X ACE215 Diagnosis File
static XML_STATUS fUploadFileType(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGDATA) {
		/*	华为定制的 文件类型 log */
		if(strstr(value,LOG_WORLD) != NULL)
		{
			rpcAction->ud.downloadReq.efileType = eVendorLog;
			return XML_STS_OK;
		}
        int type = atoi(value);
        /*if ( type == eWebContent )
            rpcAction->ud.downloadReq.efileType = eVendorLog;
        else
            rpcAction->ud.downloadReq.efileType = eVendorConfig;
        }del by zhanghaozhong*/
        /* BEGIN: Modified by zhanghaozhong, 2013/4/24   PN:dev tr69c */
        if ( type == eVendorLog )
            rpcAction->ud.downloadReq.efileType = eVendorLog;
        else if (type == eVendorPerformance)
            rpcAction->ud.downloadReq.efileType = eVendorPerformance;
        else
            rpcAction->ud.downloadReq.efileType = eVendorConfig;
        /* END:   Modified by zhanghaozhong, 2013/4/24   PN:dev tr69c */
    }
    return XML_STS_OK;
}
//#endif    // RPC_UPLOAD
/* <xxx:InformResponse>
*       <MaxEnvelopes>xx</MaxEnvelopes>
* Assumes that we only get this in an InformResponse msg.
*/

static XML_STATUS fMaxEnvelopes(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGDATA)
        acsState.maxEnvelopes= atoi(value);
    return XML_STS_OK;
}
/* <Reboot>
*/

static XML_STATUS fCommandKey(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGDATA){
        rpcAction->commandKey = strdup(value);
    }
    return XML_STS_OK;
}
static XML_STATUS fReboot(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGBEGIN)
        rpcAction->rpcMethod = rpcReboot;
    return XML_STS_OK;
}
/* 
* <FactoryReset/> 
*/
static XML_STATUS fFactoryReset(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGBEGIN)
        rpcAction->rpcMethod = rpcFactoryReset;
    return XML_STS_OK;
}

static XML_STATUS fInformResponse(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGBEGIN){
        rpcAction->rpcMethod = rpcInformResponse;
        informState = eACSInformed;
    }
    return XML_STS_OK;
}
static XML_STATUS fTransferCompleteResponse(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGBEGIN){
        rpcAction->rpcMethod = rpcTransferCompleteResponse;
    }
    return XML_STS_OK;
}
/* <AddObject>
*  <DeleteObject>
*
*/

static XML_STATUS fObjectName(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGDATA && rpcAction){
        rpcAction->ud.addDelObjectReq.objectName = strdup(value);
    }
    return XML_STS_OK;
}

static XML_STATUS fAddObject(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGBEGIN && rpcAction){
        rpcAction->rpcMethod = rpcAddObject;
    }
    return XML_STS_OK;
}
static XML_STATUS fDeleteObject(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGBEGIN && rpcAction){
        rpcAction->rpcMethod = rpcDeleteObject;
    }
    return XML_STS_OK;
}
/* <SetParameterValue>
*   <ParameterList <ParameterValueStruct>
*                       <Name>xxxx</Name> <Value>yyyy</value>
*   </ParamterList>
*   <ParameterKey>kkkkk</ParameterKey>
*/
static XML_STATUS fParameterValue(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGDATA && rpcAction){
        if(rpcAction->ud.pItem)
            rpcAction->ud.pItem->pvalue = strdup(value);
    }
    return XML_STS_OK;
}
static XML_STATUS fSetParameterKey(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGDATA && rpcAction){
        rpcAction->parameterKey = strdup(value);
    }
    return XML_STS_OK;
}
static XML_STATUS fSetParameterValues(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGBEGIN && rpcAction){
        rpcAction->rpcMethod = rpcSetParameterValues;
    }
    return XML_STS_OK;
}
/* <ns:GetParameterValues ....../>
*
*/
static XML_STATUS fParameterNameArraySize(const char *name, TOKEN_TYPE ttype, const char *value)
{
	
    #if !defined(SUPPRESS_SOAP_ARRAYTYPE)  /* use element cnt */
    if (ttype == ATTRIBUTEVALUE && rpcAction){
        char *s;
        s = strchr(value,'[');
        if (s)
            rpcAction->arrayItemCnt = atoi(s+1);
    }
	#endif
    return XML_STS_OK;
}

static XML_STATUS fParameterNames(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGBEGIN && rpcAction){
        ParamItem *p = (ParamItem *)malloc(sizeof (ParamItem));
        if (p) {
			#ifdef SUPPRESS_SOAP_ARRAYTYPE
            ++rpcAction->arrayItemCnt;
			#endif
            p->pname = p->pvalue = p->pOrigValue = NULL;
            p->next = rpcAction->ud.pItem;
            rpcAction->ud.pItem = p;
            return XML_STS_OK;
        }
        return XML_STS_ERR;
    } else if (ttype==TAGDATA) {
        ParamItem *p = rpcAction->ud.pItem;
        p->pname = strdup(value);

    }
    return XML_STS_OK;
}

static XML_STATUS fGetParameterValues(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGBEGIN && rpcAction)
        rpcAction->rpcMethod = rpcGetParameterValues;
    return XML_STS_OK;
}

/* <SetParameterAttributes>
*  <ParameterList <SetParameterAttributesStruct>
*               <Name>xxx</Name>  ....
*/
static XML_STATUS fAccessList(const char *name, TOKEN_TYPE ttype, const char *value)
{   /* there's only one access list item today */
    if (ttype == TAGDATA && rpcAction){
        if (rpcAction->ud.aItem) {
            rpcAction->ud.aItem->subAccess= !strcasecmp(value,"subscriber");
            return XML_STS_OK;
        }
        return XML_STS_ERR;
    }
    return XML_STS_OK;
}
static XML_STATUS fAcclistChange(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGDATA && rpcAction){
        if (rpcAction->ud.aItem) {
            rpcAction->ud.aItem->chgAccess= testBoolean(value);
            return XML_STS_OK;
        }
        return XML_STS_ERR;
    }
    return XML_STS_OK;
}
static XML_STATUS fNotificationChange(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGDATA && rpcAction){
        if (rpcAction->ud.aItem) {
            rpcAction->ud.aItem->chgNotify= testBoolean(value);
            return XML_STS_OK;
        }
        return XML_STS_ERR;
    }
    return XML_STS_OK;
}
static XML_STATUS fNotification(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGDATA && rpcAction){
        if (rpcAction->ud.aItem) {
            rpcAction->ud.aItem->notification = atoi(value);
            return XML_STS_OK;
        }
        return XML_STS_ERR;
    }
    return XML_STS_OK;
}
static XML_STATUS fSetAttParameterName(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGDATA && rpcAction){
        if (rpcAction->ud.aItem) {
            rpcAction->ud.aItem->pname = strdup(value);
            return XML_STS_OK;
        }
        return XML_STS_ERR;
    }
    return XML_STS_OK;
}

static XML_STATUS fSetParameterAttributesList(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGBEGIN && rpcAction){
        AttributeItem *p = (AttributeItem *)malloc(sizeof (AttributeItem));
        if (p) {
            #ifdef SUPPRESS_SOAP_ARRAYTYPE
			++rpcAction->arrayItemCnt;
			#endif
            p->next = rpcAction->ud.aItem;
            rpcAction->ud.aItem = p;
            return XML_STS_OK;
        }
        return XML_STS_ERR;
    }
    return XML_STS_OK;
}

static XML_STATUS fSetParameterAttributes(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGBEGIN && rpcAction){
        rpcAction->rpcMethod = rpcSetParameterAttributes;
    }
    return XML_STS_OK;
}
/* <GetParameterAttributes>
*  <ParameterNames SOAP-ENC:arrayType="xsd:string[xx]"
*               <xsd:string>xxx</xsd:string>  ....
*/
static XML_STATUS fGetParameterAttributes(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGBEGIN && rpcAction){
        rpcAction->rpcMethod = rpcGetParameterAttributes;
    }
    return XML_STS_OK;
}


/* <ns:GetParameterNames>
*     <ParameterPath>parameterpath</ParameterPath>
*/
static XML_STATUS fGetParameterNames(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGBEGIN && rpcAction)
        rpcAction->rpcMethod = rpcGetParameterNames;
    return XML_STS_OK;
}
static XML_STATUS fParameterPath(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGDATA && rpcAction)
        rpcAction->ud.paramNamesReq.parameterPath = strdup(value);

    return XML_STS_OK;
}
static XML_STATUS fNextLevel(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGDATA && rpcAction)
        rpcAction->ud.paramNamesReq.nextLevel = testBoolean(value);
    return XML_STS_OK;
}
/* <SOAP_ENV:Body ..../> */


static XML_STATUS fGetRPCMethods(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGBEGIN && rpcAction)
        rpcAction->rpcMethod = rpcGetRPCMethods;
    return XML_STS_OK;
}

static XML_STATUS fBody(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGBEGIN) {
        }
    else {
    }
    return XML_STS_OK;
}


static XML_STATUS fGetRPCMethodsResponse(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGBEGIN && rpcAction)
        rpcAction->rpcMethod = rpcGetRPCMethodsResponse;
    return XML_STS_OK;
}
/* <SOAP-ENV:Header ..../> */
static XML_STATUS fNoMoreRequests(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGDATA)
        acsState.noMoreRequests = !strcmp(value,"1");
    return XML_STS_OK;
}
static XML_STATUS fHoldRequest(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGDATA)
        acsState.holdRequests = !strcmp(value,"1");
        
    return XML_STS_OK;
}
static XML_STATUS fIDValue(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGDATA) {
        if (rpcAction)
            rpcAction->ID = strdup(value);
        else
            return XML_STS_ERR;
    }
    return XML_STS_OK;
}
/* <envelope .../>   */
static XML_STATUS fEnvelope(const char *name, TOKEN_TYPE ttype, const char *value)
{
    if (ttype == TAGBEGIN) {
            if (g_debug)
        DEBUGP(("fEnvelope() TAGBEGIN ------\n"));
        if (rpcAction)
            freeRPCAction(rpcAction);
        rpcAction = newRPCAction();
        }
    else {
            if (g_debug)
        DEBUGP(("fEnvelope() TAGEND -----\n"));
        /* end of envelope -- now run RPC */
        /* */
        if (g_debug)
        {
            dumpAcsState();
            dumpRpcAction(rpcAction);
        }

    }
    return XML_STS_OK;
}



