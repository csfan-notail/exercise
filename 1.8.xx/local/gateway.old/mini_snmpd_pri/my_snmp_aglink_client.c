/******************************************************************************

  Copyright (C), 2010-2011, GBCOM Co., Ltd.

 ******************************************************************************
  File Name     : my_snmp_aglink_client.c
  Version       : Initial Draft
  Author        : xiabing
  Created       : 2014/12/03
  Last Modified :
  Description   : register aglink client
  Function List :
  History       :
  1.Date        : 2014/12/03
    Author      : xiabing
    Modification: Created file

******************************************************************************/
#include <syslog.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "my_snmp_def.h"
#include "my_snmp_data.h"
#include "mini_snmpd.h"
#include "my_snmp_trap.h"
#include "aglink.h"
#include "ag_module.h"       //aglink 头文件
#include "ag_msg.h"
#include "my_snmp_aglink_client.h"
#include "snmp_interface.h"
/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/
extern SNMP_AGENT_DATA_T *g_pstSnmpAgentData;
extern OSTOKEN gtSemConfig;
extern OSTOKEN gtSemStat;
extern CHAR g_acAuthCode[64];
extern UINT8 g_aucDevMac[6];
/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/
int g_iSnmpAglinkLogLevel = LOG_ERR;
unsigned int g_ulNoGwIdFlag = 1;
static AG_MODULE_IPC_REGISTER g_stSnmpAglink;
SNMPAGENT_ALL_STA_INFO_T g_stSnmpAllStaInfo;
UINT32 g_ulUserFlag = 1;

#define MODULE_NAME_STAM "stam"
#define MODULE_NAME_SNMPAGENT "snmpagent"
#define MODULE_NAME_EVENT "event"
#define MODULE_NAME_WIFIDOG "wifidog"
/*----------------------------------------------*
 * module-wide global variables                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/
#define SNMP_AGLINK_LOG(level, format, arg...) do{ \
			app_log((char *)__func__, __line__, level, format, ##arg);\
		}while(0)
/*----------------------------------------------*
 * macros                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * routines' implementations                    *
 *----------------------------------------------*/

int snmp_ap_register_handle(WIFIOG_AP_REGISTER_T *pstMsg)
{
	if (NULL == pstMsg){
		SNMP_AGLINK_LOG(LOG_ERR, "NULL == pstMsg fail");
        return -1;
    }
    snmp_add_ap_by_mac(pstMsg->aucMac);
    return 0;
}

int snmp_ap_logout_handle(WIFIOG_AP_LOGOUT_T *pstMsg)
{
	if (NULL == pstMsg){
		SNMP_AGLINK_LOG(LOG_ERR, "NULL == pstMsg fail");
        return -1;
    }
    snmp_del_ap_by_mac(pstMsg->aucMac);
    return 0;
}

int snmp_wifidog_get_authcfg_handle(AUTH_CONF_T *pstMsg)
{
	if (NULL == pstMsg){
		SNMP_AGLINK_LOG(LOG_ERR, "NULL == pstMsg fail");
        return -1;
    }
	if (!OsSemaphoreP(gtSemConfig, WAIT_FOREVER, 1))
	{
		SNMP_AGLINK_LOG(LOG_ERR, "gtSemConfig OsSemaphoreP fail");
	}
    else
    {
        g_pstSnmpAgentData->stAuthConfig.ulSwitch = pstMsg->iSwitch;
        g_pstSnmpAgentData->stAuthConfig.ulWebAuth = pstMsg->iWeb_auth;
        g_pstSnmpAgentData->stAuthConfig.ulOffline = pstMsg->iOffline;
        g_pstSnmpAgentData->stAuthConfig.ulReverify = pstMsg->iReverify/60;
		OsSemaphoreV(gtSemConfig, 1);
    }
    return 0;
}
int snmp_aglink_get_apinfo_handle(AGLINK_AP_DETAIL_INFO_T *pstMsg)
{
    AGLINK_AP_DETAIL_INFO_T stApInfo;
    int iLoop = 0;
	if (NULL == pstMsg){
		SNMP_AGLINK_LOG(LOG_ERR, "NULL == pstMsg fail");
        return -1;
    }
    memset(&stApInfo, 0, sizeof(stApInfo));

    memcpy(&stApInfo, pstMsg, sizeof(AGLINK_AP_DETAIL_INFO_T));
    NTOHL_IT(stApInfo.cur_link);
    NTOHL_IT(stApInfo.total_link);
    NTOHL_IT(stApInfo.iCha2);
    NTOHL_IT(stApInfo.total_up);
    NTOHL_IT(stApInfo.total_down);
    NTOHL_IT(stApInfo.iRunStatus);
    NTOHL_IT(stApInfo.runtimelong);
    NTOHL_IT(stApInfo.iSsidNum);
    NTOHL_IT(stApInfo.iOnline);
    NTOHL_IT(stApInfo.iCha5);
    NTOHL_IT(stApInfo.iRoam);
    NTOHL_IT(stApInfo.hardversion);
    NTOHL_IT(stApInfo.ap_work_mode);
    
    SNMP_AGLINK_LOG(LOG_INFO, "AP MAC:"MAC_FMT" "
        "ip:"IP_FMT" sn:%s dType:%s location:%s vendor:%s online:%d \n"
        "cur_link:%d total_link:%d iCha2:%d iCha5:%d iRoam:%d total_up:%d "
        "total_down:%d iRunStatus:%d runtimelong:%d iSsidNum:%d \n"
        "memo:%s cpuStatus:%s softver:%s", 
        MAC_ARG(stApInfo.acMac),
        IP_ARG(stApInfo.acIp),
        stApInfo.acfap_sn,
        stApInfo.acDeviceType,
        stApInfo.acLocation,
        stApInfo.acVendor,
        stApInfo.iOnline,
        stApInfo.cur_link,
        stApInfo.total_link,
        stApInfo.iCha2,
        stApInfo.iCha5,
        stApInfo.iRoam,
        stApInfo.total_up,
        stApInfo.total_down,
        stApInfo.iRunStatus,
        stApInfo.runtimelong,
        stApInfo.iSsidNum,
        stApInfo.acMemo,
        stApInfo.acCpu_status,
        stApInfo.softversion
        );
    
    for ( iLoop = 0 ; iLoop < stApInfo.iSsidNum ; iLoop++ )
    {
        NTOHL_IT(stApInfo.ssid[iLoop].iUser_num);
    }

    SNMP_AGLINK_LOG(LOG_INFO, "AP MAC:"MAC_FMT, MAC_ARG(stApInfo.acMac));
    update_ap_info_by_mac(stApInfo.acMac, &stApInfo);
    return 0;
}

int snmp_aglink_get_stainfo_handle(STAM_ALL_STA_LIST *pstMsg)
{
    int iLoop = 0;
	if (NULL == pstMsg){
		SNMP_AGLINK_LOG(LOG_ERR, "NULL == pstMsg fail");
        return -1;
    }

    NTOHL_IT(pstMsg->iStaNum);
    NTOHL_IT(pstMsg->iFlag);
    SNMP_AGLINK_LOG(LOG_INFO, "==========iStaNum:%d iFlag:%d.",
        pstMsg->iStaNum, pstMsg->iFlag);
    for ( iLoop = 0 ; iLoop < pstMsg->iStaNum ; iLoop++ )
    {
        NTOHL_IT(pstMsg->astStaInfo[iLoop].rssi);
        NTOHL_IT(pstMsg->astStaInfo[iLoop].connect_time);
        NTOHL_IT(pstMsg->astStaInfo[iLoop].total_up);
        NTOHL_IT(pstMsg->astStaInfo[iLoop].total_down);
        SNMP_AGLINK_LOG(LOG_INFO, "loop:%d apMac("MAC_FMT") staMac:"MAC_FMT" staIp("IP_FMT") SSID:%s.",
            iLoop, 
            MAC_ARG(pstMsg->astStaInfo[iLoop].aucApMac),
            MAC_ARG(pstMsg->astStaInfo[iLoop].aucStaMac),
            IP_ARG(pstMsg->astStaInfo[iLoop].aucIp),
            pstMsg->astStaInfo[iLoop].acSsid);
    }
    SNMP_AGLINK_LOG(LOG_INFO, "AP MAC:"MAC_FMT, MAC_ARG(pstMsg->aucApMac));
    update_sta_info_by_ap_mac(pstMsg->aucApMac, pstMsg->iStaNum, pstMsg->astStaInfo);
    return 0;
}
int snmp_aglink_get_userinfo_handle(WIFIDOG_SNMPAGENT_ALL_STA_INFO_T *pstMsg)
{
    int iLoop = 0;
    UINT32 ulTableMaxRecNum = TABLE_MAXRECNUM_USER;
    UINT32 ulIndex = 0;
	if (NULL == pstMsg){
		SNMP_AGLINK_LOG(LOG_ERR, "NULL == pstMsg fail");
        return -1;
    }

	if (!OsSemaphoreP(gtSemStat, WAIT_FOREVER, 1))
	{
		SNMP_AGLINK_LOG(LOG_ERR, "gtSemStat OsSemaphoreP fail");
        return -1;
	}

	if (NULL == g_pstSnmpAgentData)
	{
		SNMP_AGLINK_LOG(LOG_ERR, "g_pstSnmpAgentData is null");
		goto end;
	}
    
	if (NULL == g_pstSnmpAgentData->pstOnlineUserInfo)
	{
		g_pstSnmpAgentData->pstOnlineUserInfo = SNMP_AGENT_MALLOC(
            sizeof(AP_STAT_ONLINEUSER_ENTRY_T)*ulTableMaxRecNum);
		if (NULL == g_pstSnmpAgentData->pstOnlineUserInfo)
		{
			SNMP_AGLINK_LOG(LOG_ERR, "malloc g_pstSnmpAgentData->pstOnlineUserInfo failed");
			goto end;
		}
    	memset(g_pstSnmpAgentData->pstOnlineUserInfo, 0, sizeof(AP_STAT_ONLINEUSER_ENTRY_T)*ulTableMaxRecNum);
	}

    SNMP_AGLINK_LOG(LOG_INFO, "==========iStaNum:%d iFlag:%d.", 
        pstMsg->iStaNum, pstMsg->iFlag);
    for ( iLoop = 0 ; iLoop < pstMsg->iStaNum ; iLoop++ )
    {
        SNMP_AGLINK_LOG(LOG_INFO, "loop:%d aucMac:"MAC_FMT" "
            "aucIp:"IP_FMT" "
            "iAuthType:%d "
            "iUpSpeed:%d "
            "iDownSpeed:%d "
            "iCnntCnt:%d "
            "incoming:%d "
            "outgoing:%d "
            "iAssoTime:%d "
            "acBblink_UserId:%s."
            ,
            iLoop,
            MAC_ARG(pstMsg->astStaInfo[iLoop].aucMac),
            IP_ARG(pstMsg->astStaInfo[iLoop].aucIp),
            pstMsg->astStaInfo[iLoop].iAuthType,
            pstMsg->astStaInfo[iLoop].iUpSpeed,
            pstMsg->astStaInfo[iLoop].iDownSpeed,
            pstMsg->astStaInfo[iLoop].iCnntCnt,
            pstMsg->astStaInfo[iLoop].incoming,
            pstMsg->astStaInfo[iLoop].outgoing,
            pstMsg->astStaInfo[iLoop].iAssoTime,
            pstMsg->astStaInfo[iLoop].acBblink_UserId
            );
    }

    if ( 1 == g_ulUserFlag )/* 第一条上报消息 */
    {
        g_ulUserFlag = 0;
        g_pstSnmpAgentData->ulUserNum = 0;
        SNMP_AGLINK_LOG(LOG_INFO, "first userinfo msg, set userflag/usernum 0.");
        memset(g_pstSnmpAgentData->pstOnlineUserInfo, 0, sizeof(AP_STAT_ONLINEUSER_ENTRY_T)*ulTableMaxRecNum);
    }
    if ( (0 == g_ulUserFlag) && ( 1 == pstMsg->iFlag ))
    {
        g_ulUserFlag = 1;/* 最后一条上报消息 */
        SNMP_AGLINK_LOG(LOG_INFO, "last userinfo msg, set userflag 1.");
    }
    
    ulIndex = g_pstSnmpAgentData->ulUserNum;
    for ( iLoop = 0 ; iLoop < pstMsg->iStaNum ; iLoop++ )
    {
        if ( ulIndex > ulTableMaxRecNum )
        {
    		SNMP_AGLINK_LOG(LOG_ERR, "ulIndex:%d has been max.", ulIndex);
            goto end;
        }
        memcpy(g_pstSnmpAgentData->pstOnlineUserInfo[ulIndex].aucUserMac, pstMsg->astStaInfo[iLoop].aucMac, 6);
        g_pstSnmpAgentData->pstOnlineUserInfo[ulIndex].ulAuthType = pstMsg->astStaInfo[iLoop].iAuthType;
        g_pstSnmpAgentData->pstOnlineUserInfo[ulIndex].ulUpLoad = pstMsg->astStaInfo[iLoop].iUpSpeed;
        g_pstSnmpAgentData->pstOnlineUserInfo[ulIndex].ulDwLoad = pstMsg->astStaInfo[iLoop].iDownSpeed;
        g_pstSnmpAgentData->pstOnlineUserInfo[ulIndex].ulConnect = pstMsg->astStaInfo[iLoop].iCnntCnt;
        g_pstSnmpAgentData->pstOnlineUserInfo[ulIndex].ulTotalUp = pstMsg->astStaInfo[iLoop].incoming;
        g_pstSnmpAgentData->pstOnlineUserInfo[ulIndex].ulTotalDw = pstMsg->astStaInfo[iLoop].outgoing;
        g_pstSnmpAgentData->pstOnlineUserInfo[ulIndex].ulConnectTime = pstMsg->astStaInfo[iLoop].iAssoTime;
        memcpy(g_pstSnmpAgentData->pstOnlineUserInfo[ulIndex].acUserId, pstMsg->astStaInfo[iLoop].acBblink_UserId, 64);
        memcpy(g_pstSnmpAgentData->pstOnlineUserInfo[ulIndex].aucUserIp, pstMsg->astStaInfo[iLoop].aucIp, 4);
        ulIndex++;
    }
    g_pstSnmpAgentData->ulUserNum = ulIndex;
	SNMP_AGLINK_LOG(LOG_INFO, "ulUserNum:%d.", ulIndex);
end:    
    OsSemaphoreV(gtSemStat, 1);
    return 0;
}
int snmp_aglink_user_loginout_handle(WIFIDOG_SNMPAGENT_STA_T *pstMsg)
{
    AP_STAT_ONLINEUSER_ENTRY_T stUserInfo;
    
	if (NULL == pstMsg){
		SNMP_AGLINK_LOG(LOG_ERR, "NULL == pstMsg fail");
        return -1;
    }

    memset(&stUserInfo, 0, sizeof(AP_STAT_ONLINEUSER_ENTRY_T));
    SNMP_AGLINK_LOG(LOG_INFO, "Mac:"MAC_FMT" "
        "Ip:"IP_FMT" "
        "Ssid:%s "
        "LogInTime:%s "
        "LogOutTime:%s "
        "OnlineTime:%d "
        "LogOutType:%s."
        ,
        MAC_ARG(pstMsg->aucMac),
        IP_ARG(pstMsg->aucIp),
        pstMsg->acSsid,
        pstMsg->acLogInTime,
        pstMsg->aucLogOutTime,
        pstMsg->ulOnlineTime,
        pstMsg->acLogOutType
        );
    memcpy(stUserInfo.aucUserMac, pstMsg->aucMac, 6);
    memcpy(stUserInfo.aucUserIp, pstMsg->aucIp, 4);
    strncpy(stUserInfo.acUserSsid, pstMsg->acSsid, 32);
    strncpy(stUserInfo.acUserLoginTime, pstMsg->acLogInTime, 32);
    strncpy(stUserInfo.acUserLogoutTime, pstMsg->aucLogOutTime, 32);
    strncpy(stUserInfo.acLogoutType, pstMsg->acLogOutType, 32);
    agent_send_gw_userinfo_trap(&stUserInfo);

    return 0;
}

int snmp_aglink_sta_add_handle(SNMP_EVENT_ADD_STA_INFO *pstMsg)
{
    AP_STAT_AP_STA_ENTRY_T stStaInfo;
    UINT32 ulCurrTime = 0;
    
	if (NULL == pstMsg){
		SNMP_AGLINK_LOG(LOG_ERR, "NULL == pstMsg fail");
        return -1;
    }

    memset(&stStaInfo, 0, sizeof(AP_STAT_AP_STA_ENTRY_T));

    SNMP_AGLINK_LOG(LOG_INFO, "aucMac:"MAC_FMT" "
        "aucIp:"IP_FMT" "
        "aucSsid:%s."
        ,
        MAC_ARG(pstMsg->aucStaMac),
        IP_ARG(pstMsg->aucIp),
        pstMsg->acSsid
        );
    memcpy(stStaInfo.aucStaMac, pstMsg->aucStaMac, 6);
    memcpy(stStaInfo.aucStaIp, pstMsg->aucIp, 4);
    strncpy(stStaInfo.acClientSsid, pstMsg->acSsid, 32);
	ulCurrTime = (UINT32)time(NULL);		
	agent_convert_time(ulCurrTime, stStaInfo.acLoginTime);
    agent_send_gw_clientinfo_trap(&stStaInfo);
    return 0;
}

#if 0
int snmp_stam_get_all_sta_handle(STAM_SNMPAGENT_ALL_STA_RSP_T *pstMsg)
{
	int iLoop = 0;
    int iStaNum = 0;
	if (NULL == pstMsg){
		SNMP_AGLINK_LOG(LOG_ERR, "NULL == pstMsg fail");
        return -1;
    }
    iStaNum = ntohl(pstMsg->iStaNum);
    if (iStaNum > SNMPAGENT_STA_MAX_NUM){
		SNMP_AGLINK_LOG(LOG_ERR, "NULL == pstMsg fail");
        return -1;
    }
    SNMP_ALL_STA_MUTEX_LOCK(g_stSnmpAllStaInfo.mutex);
	g_stSnmpAllStaInfo.iStaNum = iStaNum;
    memset(g_stSnmpAllStaInfo.astSta, 0, sizeof(SNMPAGENT_STA_T)*SNMPAGENT_STA_MAX_NUM);
    for (iLoop = 0; iLoop < iStaNum; iLoop++){
		memcpy(g_stSnmpAllStaInfo.astSta[iLoop].aucMac, pstMsg->astStaInfo[iLoop].aucMac, 6);
        memcpy(g_stSnmpAllStaInfo.astSta[iLoop].aucIp, pstMsg->astStaInfo[iLoop].aucIp, 16);
        g_stSnmpAllStaInfo.astSta[iLoop].iAuthState = ntohl(pstMsg->astStaInfo[iLoop].iAuthState);
        g_stSnmpAllStaInfo.astSta[iLoop].iLoginTimeSet = ntohl(pstMsg->astStaInfo[iLoop].iLoginTimeSet);
        g_stSnmpAllStaInfo.astSta[iLoop].iOnlineTime = ntohl(pstMsg->astStaInfo[iLoop].iOnlineTime);
        g_stSnmpAllStaInfo.astSta[iLoop].iAssoTimeSet = ntohl(pstMsg->astStaInfo[iLoop].iAssoTimeSet);
        g_stSnmpAllStaInfo.astSta[iLoop].iAssoTime = ntohl(pstMsg->astStaInfo[iLoop].iAssoTime);

        SNMP_AGLINK_LOG(LOG_DEBUG, "get-sta-from-stam sta-mac     : "MAC_FMT, MAC_ARG(g_stSnmpAllStaInfo.astSta[iLoop].aucMac));
        SNMP_AGLINK_LOG(LOG_DEBUG, "get-sta-from-stam sta-ip      : "IP_FMT, IP_ARG(g_stSnmpAllStaInfo.astSta[iLoop].aucIp));
        SNMP_AGLINK_LOG(LOG_DEBUG, "get-sta-from-stam authstate   : %d", g_stSnmpAllStaInfo.astSta[iLoop].iAuthState);
        SNMP_AGLINK_LOG(LOG_DEBUG, "get-sta-from-stam logintimeset: %d", g_stSnmpAllStaInfo.astSta[iLoop].iLoginTimeSet);
        SNMP_AGLINK_LOG(LOG_DEBUG, "get-sta-from-stam onlinetime  : %d", g_stSnmpAllStaInfo.astSta[iLoop].iOnlineTime);
        SNMP_AGLINK_LOG(LOG_DEBUG, "get-sta-from-stam assotimeset : %d", g_stSnmpAllStaInfo.astSta[iLoop].iAssoTimeSet);
        SNMP_AGLINK_LOG(LOG_DEBUG, "get-sta-from-stam assotime    : %d", g_stSnmpAllStaInfo.astSta[iLoop].iAssoTime);
    }
    SNMP_ALL_STA_MUTEX_UNLOCK(g_stSnmpAllStaInfo.mutex);
    return 0;
}

int snmp_wifidog_get_all_sta_handle(WIFIDOG_SNMPAGENT_ALL_STA_RSP_T *pstMsg)
{
	int iLoop = 0;
    int iStaNum = 0;
	if (NULL == pstMsg){
		SNMP_AGLINK_LOG(LOG_ERR, "NULL == pstMsg fail");
        return -1;
    }
    SNMP_ALL_STA_MUTEX_LOCK(g_stSnmpAllStaInfo.mutex);
    iStaNum = ntohl(pstMsg->iStaNum);
    if (iStaNum > SNMPAGENT_STA_MAX_NUM){
		SNMP_AGLINK_LOG(LOG_ERR, "NULL == pstMsg fail");
        return -1;
    }
	g_stSnmpAllStaInfo.iStaNum = iStaNum;
    memset(g_stSnmpAllStaInfo.astSta, 0, sizeof(SNMPAGENT_STA_T)*SNMPAGENT_STA_MAX_NUM);
    for (iLoop = 0; iLoop < iStaNum; iLoop++){
		memcpy(g_stSnmpAllStaInfo.astSta[iLoop].aucMac, pstMsg->astStaInfo[iLoop].aucMac, 6);
        memcpy(g_stSnmpAllStaInfo.astSta[iLoop].aucIp, pstMsg->astStaInfo[iLoop].aucIp, 16);
        g_stSnmpAllStaInfo.astSta[iLoop].iAuthState = ntohl(pstMsg->astStaInfo[iLoop].iAuthState);
        g_stSnmpAllStaInfo.astSta[iLoop].iLoginTimeSet = ntohl(pstMsg->astStaInfo[iLoop].iLoginTimeSet);
        g_stSnmpAllStaInfo.astSta[iLoop].iOnlineTime = ntohl(pstMsg->astStaInfo[iLoop].iOnlineTime);
        g_stSnmpAllStaInfo.astSta[iLoop].iAssoTimeSet = ntohl(pstMsg->astStaInfo[iLoop].iAssoTimeSet);
        g_stSnmpAllStaInfo.astSta[iLoop].iAssoTime = ntohl(pstMsg->astStaInfo[iLoop].iAssoTime);
        SNMP_AGLINK_LOG(LOG_DEBUG, "get-sta-from-wifidog sta-mac     : "MAC_FMT, MAC_ARG(g_stSnmpAllStaInfo.astSta[iLoop].aucMac));
        SNMP_AGLINK_LOG(LOG_DEBUG, "get-sta-from-wifidog sta-ip      : "IP_FMT, IP_ARG(g_stSnmpAllStaInfo.astSta[iLoop].aucIp));
        SNMP_AGLINK_LOG(LOG_DEBUG, "get-sta-from-wifidog authstate   : %d", g_stSnmpAllStaInfo.astSta[iLoop].iAuthState);
        SNMP_AGLINK_LOG(LOG_DEBUG, "get-sta-from-wifidog logintimeset: %d", g_stSnmpAllStaInfo.astSta[iLoop].iLoginTimeSet);
        SNMP_AGLINK_LOG(LOG_DEBUG, "get-sta-from-wifidog onlinetime  : %d", g_stSnmpAllStaInfo.astSta[iLoop].iOnlineTime);
        SNMP_AGLINK_LOG(LOG_DEBUG, "get-sta-from-wifidog assotimeset : %d", g_stSnmpAllStaInfo.astSta[iLoop].iAssoTimeSet);
        SNMP_AGLINK_LOG(LOG_DEBUG, "get-sta-from-wifidog assotime    : %d", g_stSnmpAllStaInfo.astSta[iLoop].iAssoTime);
    }
    SNMP_ALL_STA_MUTEX_UNLOCK(g_stSnmpAllStaInfo.mutex);
    return 0;
}

int snmp_wifidog_get_gw_id_handle(WIFIDOG_SNMPAGENT_GWID_T *pstMsg)
{
	if (NULL == pstMsg){
		SNMP_AGLINK_LOG(LOG_ERR, "NULL == pstMsg fail");
        return -1;
    }
    SNMP_AGLINK_LOG(LOG_DEBUG, "recv SNMPAGENT_IDSET_WIFIDOG_SNMPAGENT_GET_GWID_RSP");
    SNMP_AGLINK_LOG(LOG_DEBUG, "gw-id: %s", pstMsg->acGwId);
    if (!OsSemaphoreP(gtSemConfig, WAIT_FOREVER, 1))
    {
        SNMP_AGLINK_LOG(LOG_DEBUG, "gtSemConfig OsSemaphoreP fail");
    }
    else
    {
    	strncpy(g_pstSnmpAgentData->pstSysDevInfo->acHotId, pstMsg->acGwId, 32);
        //strcpy(g_pstSnmpAgentData->pstSysDevInfo->acHotId,"drpeng-cgw500");
        /* 发送discover trap */
        if ( 0 != strlen(g_pstSnmpAgentData->pstSysDevInfo->acHotId) )
        {
            SNMP_AGLINK_LOG(LOG_ERR, "Func:%s Line:%d get acHotId:%s, begin send discover.\n",
                    __func__, __line__, g_pstSnmpAgentData->pstSysDevInfo->acHotId);
        }
        OsSemaphoreV(gtSemConfig, 1);
    }

    return 0;
}

int snmp_notified_wifidog_add_all_apssid_conf()
{
	/*ssid 数目最多6个*/	
	int iRet = -1;
	WIFIDOG_SNMPAGENT_APSSID_CONFIG_RSP_T *pstRspMsg = NULL;
    int iMsgLen = 8 + sizeof(int) + 6 * sizeof(WIFIOG_SNMPAGENT_SSIDCONFIG_T);
	pstRspMsg = (WIFIDOG_SNMPAGENT_APSSID_CONFIG_RSP_T *)malloc(iMsgLen);
	if (NULL == pstRspMsg){
		SNMP_AGLINK_LOG(LOG_ERR, "(WIFIDOG_SNMPAGENT_APSSID_CONFIG_RSP_T *)malloc(iMsgLen) fail");
		return -1;
	}
	//todo
	/* BEGIN: Added by zoucaihong, 2014/12/16 */
    memset(pstRspMsg, 0, sizeof(WIFIDOG_SNMPAGENT_APSSID_CONFIG_RSP_T));
    if ( FALSE == wlan_wifidog_get_apssid_info(pstRspMsg) )
    {
		SNMP_AGLINK_LOG(LOG_ERR, "wlan_wifidog_get_apssid_info fail");
		return -1;
    }
	/* END: Added by zoucaihong, 2014/12/16 */

    iRet = ag_module_ipc_client_send_msg(WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_NOTIFIED_APSSID_CONF,
		    							  AG_IPC_MSG_TYPE_OTHER_CPU,
		    							  &g_stSnmpAglink,
		    							  (unsigned char *)(pstRspMsg),
		    							  iMsgLen);
    if (FALSE == iRet){
        SNMP_AGLINK_LOG(LOG_ERR, "WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_GET_GWID send fail");
    }
    return iRet;
}

int snmp_ap_register_success_handle()
{
	SNMP_AGLINK_LOG(LOG_DEBUG,"recv BC_MSG_AP_REGISTER_TO_GW_SUCCESS");
	snmp_notified_wifidog_add_all_apssid_conf();
	snmp_notify_wifidog_get_gw_id();
	return 0;
}
#endif

int snmp_aglink_msg_handle(evutil_socket_t fd, short event, void *arg)
{
	char acBuf[AG_MODULE_BUFFER_SIZE] = {0};
	AG_LINK_MSG_HEAD_T *pstMsgHdr = NULL;
    AG_LINK_MSG_HEAD_T stHdr = {0};
	void *pstMsgBody = NULL;
    int iRet = -1;

    u8 *p = acBuf;
	u8 **pnt = &p;

    iRet = recvfrom(g_stSnmpAglink.uipcSocket, acBuf, AG_MODULE_BUFFER_SIZE, 0, NULL, NULL);
    if(iRet < 0){
		SNMP_AGLINK_LOG(LOG_ERR, "recvfrom fail. fd: %d", g_stSnmpAglink.uipcSocket);
        return -1;
    }

    DECODE_GETL(pnt, stHdr.usMsgId);
	DECODE_GETW(pnt, stHdr.usMsgType);
	DECODE_GETW(pnt, stHdr.uiUnicast);
	DECODE_GET(stHdr.acname, pnt, MODULE_NAME_LENGTH);
	DECODE_GETL(pnt, stHdr.uiMsgLen);
    
	AGLINK_LOG(LOG_INFO, "Module name:%s MsgId:%d MsgType:%d, MsgLen:%d", stHdr.acname, stHdr.usMsgId, stHdr.usMsgType, stHdr.uiMsgLen);

    pstMsgHdr = acBuf;
    
    switch(stHdr.usMsgId){
        case AG_MSG_IPC_REGISTER_ACK:
			g_stSnmpAglink.uiFlag = 1;
			SNMP_AGLINK_LOG(LOG_NOTICE, "RECV AG_IPC_MSG_REGISTER_ACK msg");
			break;
        case AG_MSG_IPC_UNREGISTER:
			AG_MODULE_LOG(LOG_INFO, "RECV AG_IPC_MSG_UNREGISTER msg");
			g_stSnmpAglink.uiFlag = 0;
			ag_module_ipc_reconnect_server(&g_stSnmpAglink);
			break;
        /* BEGIN: Added by zoucaihong, 2015/3/31 */
        case SNMP_MSG_AP_ADD:
			SNMP_AGLINK_LOG(LOG_NOTICE, "RECV SNMP_MSG_AP_ADD msg");
            pstMsgBody = (WIFIOG_AP_REGISTER_T *)(pstMsgHdr + 1);
            snmp_ap_register_handle(pstMsgBody);					
			break;
        case SNMP_MSG_AP_DEL:
			SNMP_AGLINK_LOG(LOG_NOTICE, "RECV SNMP_MSG_AP_DEL msg");
            pstMsgBody = (WIFIOG_AP_LOGOUT_T *)(pstMsgHdr + 1);
            snmp_ap_logout_handle(pstMsgBody);
			break;
        case SNMP_MSG_GET_AUTH_CFG_ACK:
			SNMP_AGLINK_LOG(LOG_INFO, "RECV SNMP_MSG_GET_AUTH_CFG_ACK msg");
            pstMsgBody = (AUTH_CONF_T *)(pstMsgHdr + 1);
            snmp_wifidog_get_authcfg_handle(pstMsgBody);
			break;
        case SNMP_MSG_AP_INFO:
			SNMP_AGLINK_LOG(LOG_INFO, "RECV SNMP_MSG_AP_INFO msg");
            pstMsgBody = (AGLINK_AP_DETAIL_INFO_T *)(pstMsgHdr + 1);
            snmp_aglink_get_apinfo_handle(pstMsgBody);
			break;
        case SNMP_MSG_STA_INFO:
			SNMP_AGLINK_LOG(LOG_INFO, "RECV SNMP_MSG_STA_INFO msg");
            pstMsgBody = (STAM_ALL_STA_LIST *)(pstMsgHdr + 1);
            snmp_aglink_get_stainfo_handle(pstMsgBody);
			break;
        case SNMP_MSG_USERINFO:
			SNMP_AGLINK_LOG(LOG_INFO, "RECV SNMP_MSG_USERINFO msg");
            pstMsgBody = (WIFIDOG_SNMPAGENT_ALL_STA_INFO_T *)(pstMsgHdr + 1);
            snmp_aglink_get_userinfo_handle(pstMsgBody);
			break;
        case SNMP_MSG_USER_ADD:
			SNMP_AGLINK_LOG(LOG_INFO, "RECV SNMP_MSG_USER_ADD msg");
            pstMsgBody = (WIFIDOG_SNMPAGENT_STA_T *)(pstMsgHdr + 1);
            snmp_aglink_user_loginout_handle(pstMsgBody);
			break;
        case SNMP_MSG_USER_DEL:
			SNMP_AGLINK_LOG(LOG_INFO, "RECV SNMP_MSG_USER_DEL msg");
            pstMsgBody = (WIFIDOG_SNMPAGENT_STA_T *)(pstMsgHdr + 1);
            snmp_aglink_user_loginout_handle(pstMsgBody);
			break;
        case SNMP_MSG_STA_ADD:
			SNMP_AGLINK_LOG(LOG_INFO, "RECV SNMP_MSG_STA_ADD msg");
            pstMsgBody = (SNMP_EVENT_ADD_STA_INFO *)(pstMsgHdr + 1);
            snmp_aglink_sta_add_handle(pstMsgBody);
			break;
        /* END: Added by zoucaihong, 2015/3/31 */
        #if 0    
        case SNMPAGENT_IDSET_STAM_SNMPAGENT_ALL_STA_RSP:
            pstMsgBody = (STAM_SNMPAGENT_ALL_STA_RSP_T *)(pstMsgHdr + 1);
            snmp_stam_get_all_sta_handle(pstMsgBody);
            break;
		case SNMPAGENT_IDSET_WIFIDOG_SNMPAGENT_ALL_STA_RSP:
            pstMsgBody = (WIFIDOG_SNMPAGENT_ALL_STA_RSP_T *)(pstMsgHdr + 1);
            snmp_wifidog_get_all_sta_handle(pstMsgBody);
            break;
        case SNMPAGENT_IDSET_WIFIDOG_SNMPAGENT_GET_GWID_RSP:
            pstMsgBody = (WIFIDOG_SNMPAGENT_GWID_T *)(pstMsgHdr + 1);
            snmp_wifidog_get_gw_id_handle(pstMsgBody);
			break;
		/*AP注册成功消息*/
		case BC_MSG_AP_REGISTER_TO_GW_SUCCESS:
			snmp_ap_register_success_handle();
			break;
        #endif    
        default:
            SNMP_AGLINK_LOG(LOG_ERR, "UNKOWN MSG, msg id: %u, msg type: %u", ntohs(pstMsgHdr->usMsgId), ntohs(pstMsgHdr->usMsgType));
            break;
    }
    return 0;
}

/*********************************************发送消息接口*********************************************/
#if 0
int snmp_notify_stam_get_all_sta()
{
	int iRet = -1;
    iRet = ag_module_ipc_client_send_msg(STAM_IDSET_SNMPAGENT_STAM_GET_ALL_STA,
		    							  AG_IPC_MSG_TYPE_SELF_CPU,
		    							  &g_stSnmpAglink,
		    							  NULL,
		    							  0);
    if (FALSE == iRet){
        SNMP_AGLINK_LOG(LOG_ERR, "STAM_IDSET_SNMPAGENT_STAM_GET_ALL_STA send fail");
    }
    return iRet;
}
int snmp_notify_wifidog_get_gw_id()
{
	int iRet = -1;
	g_ulNoGwIdFlag = 1;
	int iMsgLen = sizeof(WIFIDOG_SNMPAGENT_GET_GWID_T);
	WIFIDOG_SNMPAGENT_GET_GWID_T stMsg;
    memset(&stMsg, 0, sizeof(stMsg));
	#if defined(AP9344_OAM)
	if (iRet == app_get_devmac(stMsg.aucApMac, "eth0"))
	#elif defined(AP9341_OAM) 
	if (iRet == app_get_devmac(stMsg.aucApMac, "wan0"))
	#endif
    {
        SNMP_AGLINK_LOG(LOG_ERR, "Func:%s get dev MAC fail.\n", __func__);
        return iRet;
    }
    strcpy(stMsg.acAuthCode, g_acAuthCode);
    iRet = ag_module_ipc_client_send_msg(WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_GET_GWID,
		    							  AG_IPC_MSG_TYPE_OTHER_CPU,
		    							  &g_stSnmpAglink,
		    							  (unsigned char *)(&stMsg),
		    							  iMsgLen);
    if (FALSE == iRet) {
        SNMP_AGLINK_LOG(LOG_ERR, "WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_GET_GWID send fail");
    }
    return iRet;
}
int snmp_notify_wifidog_get_all_sta()
{
	int iRet = -1;
    iRet = ag_module_ipc_client_send_msg(WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_GET_ALL_STA,
		    							  AG_IPC_MSG_TYPE_SELF_CPU,
		    							  &g_stSnmpAglink,
		    							  NULL,
		    							  0);
    if (FALSE == iRet){
        SNMP_AGLINK_LOG(LOG_ERR, "WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_GET_ALL_STA send fail");
    }
    return iRet;
}
int snmp_notify_wifidog_set_ap_ssid(unsigned char *aucApMac, 
                          						unsigned int uiSsidNumber,
					                            unsigned int  uiRadioType,
					                            char *acVapName,
					                            char *acssid,
					                            unsigned int uiDefault,
					                            unsigned int uiUpRate,
					                            unsigned int uiDownRate,
					                            char *acPortalUrl,
					                            unsigned int uiOnlineControlTime,
					                            unsigned int uiFlowOffTime)
{
	WIFIOG_SNMPAGENT_SSIDCONFIG_T stMsg;
    int iMsgLen = sizeof(WIFIOG_SNMPAGENT_SSIDCONFIG_T);
    int iRet = -1;
    
    if(NULL == aucApMac || NULL == acVapName || NULL == acPortalUrl){
		SNMP_AGLINK_LOG(LOG_ERR, "NULL == aucApMac || NULL == acVapName || NULL == acPortalUrl fail");
        return -1;
    }

    memcpy(stMsg.aucApMac, aucApMac, 6);
    stMsg.uiSsidNumber = htonl(uiSsidNumber);
    stMsg.uiRadioType = htonl(uiRadioType);
    strncpy(stMsg.acVapName, acVapName, 32);
    strncpy(stMsg.acSsid, acssid, 32);
    stMsg.uiDefault = htonl(uiDefault);
    stMsg.uiUpRate = htonl(uiUpRate);
    stMsg.uiDownRate = htonl(uiDownRate);
    strncpy(stMsg.acPortalUrl, acPortalUrl, 32);
    stMsg.uiOnlineControlTime = htonl(uiOnlineControlTime);
    stMsg.uiFlowOffTime = htonl(uiFlowOffTime);

    iRet = ag_module_ipc_client_send_msg(WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_AP_SSIDCONFIG,
		    							  AG_IPC_MSG_TYPE_SELF_CPU,
		    							  &g_stSnmpAglink,
		    							  (unsigned char *)(&stMsg),
		    							  iMsgLen);
    if (FALSE == iRet){
        SNMP_AGLINK_LOG(LOG_ERR, "WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_AP_SSIDCONFIG send fail");
    }
    return iRet;
}
int snmp_notify_wifidog_del_ap_ssid(unsigned char *aucApMac, unsigned int uiSsidNumber)
{
	WIFIOG_SNMPAGENT_SSID_DEL_T stMsg;
    int iMsgLen = sizeof(WIFIOG_SNMPAGENT_SSIDCONFIG_T);
    int iRet = -1;
    
    if(NULL == aucApMac){
		SNMP_AGLINK_LOG(LOG_ERR, "NULL == aucApMac  fail");
        return -1;
    }

    memcpy(stMsg.aucApMac, aucApMac, 6);
    stMsg.uiSsidNumber = htonl(uiSsidNumber);

    iRet = ag_module_ipc_client_send_msg(WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_AP_SSID_DEL,
		    							  AG_IPC_MSG_TYPE_SELF_CPU,
		    							  &g_stSnmpAglink,
		    							  (unsigned char *)(&stMsg),
		    							  iMsgLen);
    if (FALSE == iRet){
        SNMP_AGLINK_LOG(LOG_ERR, "WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_AP_SSID_DEL send fail");
    }
    return iRet;
}
int snmp_notify_wifidog_set_self_ssid(unsigned char *aucApMac, 
                          						unsigned int uiSsidNumber,
					                            unsigned int  uiRadioType,
					                            char *acVapName,
					                            char *acssid,
					                            unsigned int uiDefault,
					                            unsigned int uiUpRate,
					                            unsigned int uiDownRate,
					                            char *acPortalUrl,
					                            unsigned int uiOnlineControlTime,
					                            unsigned int uiFlowOffTime)
{
	WIFIOG_SNMPAGENT_SSIDCONFIG_T stMsg;
    int iMsgLen = sizeof(WIFIOG_SNMPAGENT_SSIDCONFIG_T);
    int iRet = -1;
    
    if(NULL == aucApMac || NULL == acVapName || NULL == acPortalUrl){
		SNMP_AGLINK_LOG(LOG_ERR, "NULL == aucApMac || NULL == acVapName || NULL == acPortalUrl fail");
        return -1;
    }

    memcpy(stMsg.aucApMac, aucApMac, 6);
    stMsg.uiSsidNumber = htonl(uiSsidNumber);
    stMsg.uiRadioType = htonl(uiRadioType);
    strncpy(stMsg.acVapName, acVapName, 32);
    strncpy(stMsg.acSsid, acssid, 32);
    stMsg.uiDefault = htonl(uiDefault);
    stMsg.uiUpRate = htonl(uiUpRate);
    stMsg.uiDownRate = htonl(uiDownRate);
    strncpy(stMsg.acPortalUrl, acPortalUrl, 32);
    stMsg.uiOnlineControlTime = htonl(uiOnlineControlTime);
    stMsg.uiFlowOffTime = htonl(uiFlowOffTime);

    iRet = ag_module_ipc_client_send_msg(WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_SELF_SSIDCONFIG,
		    							  AG_IPC_MSG_TYPE_SELF_CPU,
		    							  &g_stSnmpAglink,
		    							  (unsigned char *)(&stMsg),
		    							  iMsgLen);
    if (FALSE == iRet){
        SNMP_AGLINK_LOG(LOG_ERR, "WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_SELF_SSIDCONFIG send fail");
    }
    return iRet;
}
int snmp_notify_wifidog_del_self_ssid(unsigned char *aucApMac, unsigned int uiSsidNumber)
{
	WIFIOG_SNMPAGENT_SSID_DEL_T stMsg;
    int iMsgLen = sizeof(WIFIOG_SNMPAGENT_SSIDCONFIG_T);
    int iRet = -1;
    
    if(NULL == aucApMac){
		SNMP_AGLINK_LOG(LOG_ERR, "NULL == aucApMac fail");
        return -1;
    }

    memcpy(stMsg.aucApMac, aucApMac, 6);
    stMsg.uiSsidNumber = htonl(uiSsidNumber);

    iRet = ag_module_ipc_client_send_msg(WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_AP_SSID_DEL,
		    							  AG_IPC_MSG_TYPE_SELF_CPU,
		    							  &g_stSnmpAglink,
		    							  (unsigned char *)(&stMsg),
		    							  iMsgLen);
    if (FALSE == iRet){
        SNMP_AGLINK_LOG(LOG_ERR, "WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_AP_SSID_DEL send fail");
    }
    return iRet;
}
#endif
int snmp_notify_ap_ssidcfg(AP_SSID_LIST_T *pstSsidList)
{
    CHAR acBuff[1024]={0};
    CHAR *pcMsg = NULL;
    AGLINK_AP_SSID_CFG_T stMsg;
    UINT8 aucMac[6]={0};
    int iMsgLen = sizeof(AP_SSID_LIST_T)+6;
    int iRet = -1;
    int iLoop = 0;
    
    if(NULL == pstSsidList){
		SNMP_AGLINK_LOG(LOG_ERR, "pstSsidList is null.\n");
        return -1;
    }

    if ( 0 == memcmp(aucMac, g_aucDevMac, 6) )
    {
        SNMP_AGLINK_LOG(LOG_ERR, "gw mac is null.");
        return -1;
    }
    memcpy(aucMac, g_aucDevMac, 6);
    SNMP_AGLINK_LOG(LOG_INFO, "gw mac("MAC_FMT").", MAC_ARG(aucMac));
    memset(&stMsg, 0, sizeof(AGLINK_AP_SSID_CFG_T));
    stMsg.iSsidNum = htonl(pstSsidList->ulNum);
    for ( iLoop = 0 ; iLoop < pstSsidList->ulNum ; iLoop++ )
    {
        strcpy(stMsg.ssid[iLoop].acSsid, pstSsidList->stSsidList[iLoop].acSsid);
    }
    pcMsg = acBuff;
    memcpy(acBuff, aucMac, 6);
    pcMsg += 6;
    memcpy(pcMsg, &stMsg, sizeof(AGLINK_AP_SSID_CFG_T));

#ifdef GW_X86
    iRet = ag_module_ipc_client_send_msg( AGLINK_MSG_DEV_SSIDCFG_MSG,
                                            AG_IPC_MSG_TYPE_OTHER_CPU,
                                            &g_stSnmpAglink,
                                            acBuff,
                                            iMsgLen);
#else
    iRet = ag_module_ipc_client_send_msg( AGLINK_MSG_DEV_SSIDCFG_MSG,
                                            AG_IPC_MSG_TYPE_SELF_CPU,
                                            &g_stSnmpAglink,
                                            acBuff,
                                            iMsgLen);
#endif
    if (FALSE == iRet){
        SNMP_AGLINK_LOG(LOG_ERR, "AGLINK_MSG_DEV_SSIDCFG_MSG send fail");
    }
    return iRet;
}
int snmp_notify_ap_reboot(UINT8 *pcMac)
{
    int iMsgLen = 6;
    int iRet = -1;
    UINT8 aucMac[6] = {0};
    
    if(NULL == pcMac){
		SNMP_AGLINK_LOG(LOG_ERR, "pcMac is null.");
        return -1;
    }
    SNMP_AGLINK_LOG(LOG_INFO, "pcMac:"MAC_FMT".", MAC_ARG(pcMac));
    if ( 0 == memcmp(aucMac, g_aucDevMac, 6) )
    {
        SNMP_AGLINK_LOG(LOG_ERR, "gw mac is null.\n");
        return -1;
    }
    memcpy(aucMac, g_aucDevMac, 6);

    iRet = ag_module_ipc_client_send_msg_to_ap( AGLINK_MSG_DEV_REBOOT_MSG,
		    							  AG_IPC_MSG_TYPE_OTHER_CPU,
		    							  &g_stSnmpAglink,
		    							  aucMac,
		    							  iMsgLen,
		    							  pcMac);
    if (FALSE == iRet){
        SNMP_AGLINK_LOG(LOG_ERR, "AGLINK_MSG_DEV_REBOOT_MSG send fail");
    }
    return iRet;
}
int snmp_notify_ap_upgrade(UINT8 *pcMac)
{
    VERSION_UPDATE_T stMsg;
    int iMsgLen = sizeof(VERSION_UPDATE_T);
    int iRet = -1;
    
    if(NULL == pcMac){
		SNMP_AGLINK_LOG(LOG_ERR, "pcMac is null.\n");
        return -1;
    }

    if ( NULL != g_pstSnmpAgentData->pstApFileTransConfig )
    {
        strcpy(stMsg.aucUrl, g_pstSnmpAgentData->pstApFileTransConfig->acFileName);
        stMsg.iType = htonl(g_pstSnmpAgentData->pstApFileTransConfig->ulTransProto);
        strcpy(stMsg.aucUser, g_pstSnmpAgentData->pstApFileTransConfig->aucTransUser);
        strcpy(stMsg.aucPasswd , g_pstSnmpAgentData->pstApFileTransConfig->aucTransPasswd);
    }

    iRet = ag_module_ipc_client_send_msg_to_ap( AGLINK_MSG_DEV_UPDATE_MSG,
		    							  AG_IPC_MSG_TYPE_OTHER_CPU,
		    							  &g_stSnmpAglink,
		    							  (unsigned char *)(&stMsg),
		    							  iMsgLen,
		    							  pcMac);
    if (FALSE == iRet){
        SNMP_AGLINK_LOG(LOG_ERR, "AGLINK_MSG_DEV_UPDATE_MSG send fail");
    }
    return iRet;
}
int snmp_notify_ap_cfgupdate(UINT8 *pcMac, SNMPAGENT_UPDATE_APCFG_T *pstApCfg)
{
    CHAR acBuff[2048]={0};
    CHAR *pcMsg = NULL;
    UINT8 aucMac[6]={0};
    AGLINK_AP_CONFIG_INFO_T stMsg;
    int iMsgLen = sizeof(AGLINK_AP_CONFIG_INFO_T)+6;
    int iRet = -1;
    
    if((NULL == pcMac) || (NULL == pstApCfg)){
		SNMP_AGLINK_LOG(LOG_ERR, "pcMac/pstApCfg is null.\n");
        return -1;
    }

    if ( 0 == memcmp(aucMac, g_aucDevMac, 6) )
    {
        SNMP_AGLINK_LOG(LOG_ERR, "gw mac is null.\n");
        return -1;
    }
    memcpy(aucMac, g_aucDevMac, 6);
    
    memset(&stMsg, 0, sizeof(stMsg));
    memcpy(stMsg.acMac, pcMac, 6);
    stMsg.iCha2 = htonl(pstApCfg->channel);
    stMsg.total_link = htonl(pstApCfg->totalLink);
    strcpy(stMsg.acMemo, pstApCfg->acMemo);
    stMsg.iSsidNum = htonl(2);
    strcpy(stMsg.ssid[0].acSsid, pstApCfg->acFirstSsid);
    strcpy(stMsg.ssid[1].acSsid, pstApCfg->acSecondSsid);
    SNMP_AGLINK_LOG(LOG_INFO, "ap-cfg: apMac("MAC_FMT") "
        "iCha2:%d total_link:%d acMemo:%s ssid[0]:%s ssid[1]:%s.",
        MAC_ARG(stMsg.acMac),
        pstApCfg->channel,
        pstApCfg->totalLink,
        pstApCfg->acMemo,
        stMsg.ssid[0].acSsid,
        stMsg.ssid[1].acSsid
        );
    pcMsg = acBuff;
    memcpy(acBuff, aucMac, 6);
    pcMsg += 6;
    memcpy(pcMsg, &stMsg, sizeof(AGLINK_AP_CONFIG_INFO_T));
    
    iRet = ag_module_ipc_client_send_msg_to_ap( AGLINK_MSG_DEV_CONFIG_MSG,
		    							  AG_IPC_MSG_TYPE_OTHER_CPU,
		    							  &g_stSnmpAglink,
		    							  acBuff,
		    							  iMsgLen,
		    							  pcMac);
    if (FALSE == iRet){
        SNMP_AGLINK_LOG(LOG_ERR, "AGLINK_MSG_DEV_CONFIG_MSG send fail");
    }
    return iRet;
}

int snmp_notify_wifidog_user(SNMPAGENT_USER_T *pstUser)
{
	STAM_WIFIDOG_STA_T stMsg;
    int iMsgLen = sizeof(stMsg);
    int iRet = -1;
    
    if(NULL == pstUser){
		SNMP_AGLINK_LOG(LOG_ERR, "pstUser is null.\n");
        return -1;
    }

    memset(&stMsg, 0, sizeof(stMsg));
    memcpy(stMsg.aucMac, pstUser->aucMac, 6);
    stMsg.iAuthType = pstUser->iAuthType;
    stMsg.iAuthState = pstUser->iAuthState;
    memcpy(stMsg.acBblink_UserId, pstUser->acUserId, 64);
    SNMP_AGLINK_LOG(LOG_ERR, "aucMac("MAC_FMT") iAuthType:%d iAuthState:%d acBblink_UserId:%s",
        MAC_ARG(stMsg.aucMac), pstUser->iAuthType, pstUser->iAuthState, stMsg.acBblink_UserId);

    iRet = ag_module_ipc_client_send_msg( WIFIDOG_MSG_SNMPAGENT_SET_STA_AUTH,
		    							  AG_IPC_MSG_TYPE_SELF_CPU,
		    							  &g_stSnmpAglink,
		    							  (unsigned char *)(&stMsg),
		    							  iMsgLen );
    if (FALSE == iRet){
        SNMP_AGLINK_LOG(LOG_ERR, "WIFIDOG_MSG_STA_AUTH send fail");
    }
    return iRet;
}

int snmp_notify_wifidog_user_logout(AP_CONFIG_USER_LOGOUT_T *pstUser)
{
	WIFIDOG_SNMPAGENT_USER_TIMEOUT stMsg;
    int iMsgLen = sizeof(stMsg);
    int iRet = -1;
    
    if(NULL == pstUser){
		SNMP_AGLINK_LOG(LOG_ERR, "pstUser is null.\n");
        return -1;
    }

    memset(&stMsg, 0, sizeof(stMsg));
    stMsg.iSwitch = pstUser->ulLogoutStatus;
    stMsg.iTimeOut = pstUser->ulTimeCondition;
    stMsg.iFlowThres = pstUser->ulFlowCondition;
    SNMP_AGLINK_LOG(LOG_ERR, "iSwitch:%d iTimeOut:%d iFlowThres:%d",
        stMsg.iSwitch, stMsg.iTimeOut, stMsg.iFlowThres);

    iRet = ag_module_ipc_client_send_msg( WIFIDOG_MSG_SNMPAGENT_SET_USER_TIMEOUT,
		    							  AG_IPC_MSG_TYPE_SELF_CPU,
		    							  &g_stSnmpAglink,
		    							  (unsigned char *)(&stMsg),
		    							  iMsgLen );
    if (FALSE == iRet){
        SNMP_AGLINK_LOG(LOG_ERR, "WIFIDOG_MSG_SNMPAGENT_SET_USER_TIMEOUT send fail");
    }
    return iRet;
}

int snmp_notify_wifidog_get_secret_key(AP_CONFIG_GET_SECRET_KEY_T *pstIntval)
{
	WIFIDOG_SNMPAGENT_STAKEY_INTVAL stMsg;
    int iMsgLen = sizeof(stMsg);
    int iRet = -1;
    
    if(NULL == pstIntval){
		SNMP_AGLINK_LOG(LOG_ERR, "pstIntval is null.\n");
        return -1;
    }

    memset(&stMsg, 0, sizeof(stMsg));
    stMsg.iIntval = pstIntval->ulInterval;
    SNMP_AGLINK_LOG(LOG_ERR, "iIntval:%d.", stMsg.iIntval);

    iRet = ag_module_ipc_client_send_msg( WIFIDOG_MSG_SNMPAGENT_SET_STA_KEY_INTVAL,
		    							  AG_IPC_MSG_TYPE_SELF_CPU,
		    							  &g_stSnmpAglink,
		    							  (unsigned char *)(&stMsg),
		    							  iMsgLen );
    if (FALSE == iRet){
        SNMP_AGLINK_LOG(LOG_ERR, "WIFIDOG_MSG_SNMPAGENT_SET_STA_KEY_INTVAL send fail");
    }
    return iRet;
}
int snmp_notify_wifidog_user_speed(AP_CONFIG_USER_SPEED_T *pstUser)
{
	WIFIDOG_SNMPAGENT_FLOW_CTL stMsg;
    int iMsgLen = sizeof(stMsg);
    int iRet = -1;
    
    if(NULL == pstUser){
		SNMP_AGLINK_LOG(LOG_ERR, "pstUser is null.\n");
        return -1;
    }

    memset(&stMsg, 0, sizeof(stMsg));
    stMsg.iUpFlowMax = pstUser->ulUploadMax;
    stMsg.iDownFlowMax = pstUser->ulDwloadMax;
    SNMP_AGLINK_LOG(LOG_ERR, "iUpFlowMax:%d iDownFlowMax:%d.",
        stMsg.iUpFlowMax, stMsg.iDownFlowMax);

    iRet = ag_module_ipc_client_send_msg( WIFIDOG_MSG_SNMPAGENT_SET_NORMAL_FlOW_CTL,
		    							  AG_IPC_MSG_TYPE_SELF_CPU,
		    							  &g_stSnmpAglink,
		    							  (unsigned char *)(&stMsg),
		    							  iMsgLen );
    if (FALSE == iRet){
        SNMP_AGLINK_LOG(LOG_ERR, "WIFIDOG_MSG_SNMPAGENT_SET_NORMAL_FlOW_CTL send fail");
    }
    return iRet;
}

int snmp_notify_wifidog_white_user_speed(AP_CONFIG_USER_SPEED_T *pstUser)
{
	WIFIDOG_SNMPAGENT_FLOW_CTL stMsg;
    int iMsgLen = sizeof(stMsg);
    int iRet = -1;
    
    if(NULL == pstUser){
		SNMP_AGLINK_LOG(LOG_ERR, "pstUser is null.\n");
        return -1;
    }

    memset(&stMsg, 0, sizeof(stMsg));
    stMsg.iUpFlowMax = pstUser->ulUploadMax;
    stMsg.iDownFlowMax = pstUser->ulDwloadMax;
    SNMP_AGLINK_LOG(LOG_ERR, "iUpFlowMax:%d iDownFlowMax:%d.",
        stMsg.iUpFlowMax, stMsg.iDownFlowMax);

    iRet = ag_module_ipc_client_send_msg( WIFIDOG_MSG_SNMPAGENT_SET_WHITE_FlOW_CTL,
		    							  AG_IPC_MSG_TYPE_SELF_CPU,
		    							  &g_stSnmpAglink,
		    							  (unsigned char *)(&stMsg),
		    							  iMsgLen );
    if (FALSE == iRet){
        SNMP_AGLINK_LOG(LOG_ERR, "WIFIDOG_MSG_SNMPAGENT_SET_WHITE_FlOW_CTL send fail");
    }
    return iRet;
}

int snmp_notify_wifidog_set_authcfg(SNMPAGENT_AUTH_T *pstAuth)
{
	AUTH_CONF_T stMsg;
    int iMsgLen = sizeof(stMsg);
    int iRet = -1;
    
    if(NULL == pstAuth){
		SNMP_AGLINK_LOG(LOG_ERR, "pstAuth is null.\n");
        return -1;
    }

    memset(&stMsg, 0, sizeof(stMsg));
    stMsg.iSwitch = pstAuth->iSwitch;
    stMsg.iWeb_auth = pstAuth->iWeb_auth;
    stMsg.iOffline = pstAuth->iOffline;
    stMsg.iReverify= pstAuth->iReverify;

    iRet = ag_module_ipc_client_send_msg( WIFIDOG_MSG_SNMPAGENT_SET_AUTH_CONFIG,
		    							  AG_IPC_MSG_TYPE_SELF_CPU,
		    							  &g_stSnmpAglink,
		    							  (unsigned char *)(&stMsg),
		    							  iMsgLen );
    if (FALSE == iRet){
        SNMP_AGLINK_LOG(LOG_ERR, "SNMP_MSG_AUTH_CONF send fail");
    }
    return iRet;
}
int snmp_notify_wifidog_get_authcfg()
{
	int iRet = -1;
    iRet = ag_module_ipc_client_send_msg( WIFIDOG_MSG_SNMPAGENT_GET_AUTH_CONFIG,
		    							  AG_IPC_MSG_TYPE_SELF_CPU,
		    							  &g_stSnmpAglink,
		    							  NULL,
		    							  0);
    if (FALSE == iRet){
        SNMP_AGLINK_LOG(LOG_ERR, "WIFIDOG_MSG_SNMPAGENT_GET_AUTH_CONFIG send fail");
    }
    return iRet;
}
int snmp_notify_wifidog_update_mac_baw(unsigned int uiSsidNumber, int iNum, SNMPAGENT_MAC_BAW_T *pstMacBaw)
{
	WIFIDOG_SNMPAGENT_MAC_BAW_CONFIG_T *pstMsg = NULL;
    int iMsgLen = 0;
    int iRet = -1;
    int iLoop = 0;

    iMsgLen = 8 + iNum*sizeof(WIFIOG_SNMPAGENT_MAC_BAW_T);
    pstMsg = (WIFIDOG_SNMPAGENT_MAC_BAW_CONFIG_T *)malloc(iMsgLen);
    if (NULL == pstMsg){
		SNMP_AGLINK_LOG(LOG_ERR, "NULL == pstMsg fail");
        return -1;
    }
    pstMsg->uiSsidNumber = uiSsidNumber;
    pstMsg->iNum = iNum;
    SNMP_AGLINK_LOG(LOG_INFO, "iNum:%d", iNum);
    for(iLoop = 0; iLoop < iNum; iLoop++){
		if (NULL == (pstMacBaw + iLoop)){
			SNMP_AGLINK_LOG(LOG_ERR, "NULL == (pstMacBaw + %d) fail", iLoop);
            free(pstMsg);
            return -1;
        }
        pstMsg->astMacBaw[iLoop].iType = pstMacBaw[iLoop].iType;
        memcpy(pstMsg->astMacBaw[iLoop].aucMac, pstMacBaw[iLoop].aucMac, 6);
        SNMP_AGLINK_LOG(LOG_INFO, "iLoop:%d iType:%d aucMac"MAC_FMT, 
            iLoop, pstMsg->astMacBaw[iLoop].iType, MAC_ARG(pstMsg->astMacBaw[iLoop].aucMac));
    }

    iRet = ag_module_ipc_client_send_msg( WIFIDOG_MSG_SNMPAGENT_SET_MAC_BAW_UPDATE,
		    							  AG_IPC_MSG_TYPE_SELF_CPU,
		    							  &g_stSnmpAglink,
		    							  (unsigned char *)(pstMsg),
		    							  iMsgLen);
    free(pstMsg);
    if (FALSE == iRet){
        SNMP_AGLINK_LOG(LOG_ERR, "WIFIDOG_MSG_SNMPAGENT_SET_MAC_BAW_UPDATE send fail");
    }
    return iRet;
}
int snmp_notify_wifidog_add_url_baw(unsigned int uiSsidNumber, int iNum, SNMPAGENT_URL_BAW_T *pstUrlBaw)
{
	WIFIDOG_SNMPAGENT_URL_BAW_CONFIG_T *pstMsg = NULL;
    int iMsgLen = 0;
    int iRet = -1;
    int iLoop = 0;

    iMsgLen = 8 + iNum*sizeof(WIFIOG_SNMPAGENT_URL_BAW_T);
    pstMsg = (WIFIDOG_SNMPAGENT_URL_BAW_CONFIG_T *)malloc(iMsgLen);
    if (NULL == pstMsg){
		SNMP_AGLINK_LOG(LOG_ERR, "NULL == pstMsg fail");
        return -1;
    }
    pstMsg->uiSsidNumber = uiSsidNumber;
    pstMsg->iNum = iNum;
    SNMP_AGLINK_LOG(LOG_INFO, "uiSsidNumber:%d iNum:%d.",
        pstMsg->uiSsidNumber, pstMsg->iNum = iNum);
    for(iLoop = 0; iLoop < iNum; iLoop++){
		if (NULL == (pstUrlBaw + iLoop)){
			SNMP_AGLINK_LOG(LOG_ERR, "NULL == (pstUrlBaw + %d) fail", iLoop);
            free(pstMsg);
            return -1;
        }
        pstMsg->astUrlBaw[iLoop].iType = pstUrlBaw[iLoop].iType;
        strncpy(pstMsg->astUrlBaw[iLoop].aucUrl, pstUrlBaw[iLoop].acUrl, 32);
        SNMP_AGLINK_LOG(LOG_INFO, "iLoop:%d iType:%d aucUrl:%s.",
            iLoop, pstMsg->astUrlBaw[iLoop].iType, 
            pstMsg->astUrlBaw[iLoop].aucUrl);
    }

    iRet = ag_module_ipc_client_send_msg( WIFIDOG_MSG_SNMPAGENT_ADD_BAW_URL,
		    							  AG_IPC_MSG_TYPE_SELF_CPU,
		    							  &g_stSnmpAglink,
		    							  (unsigned char *)(pstMsg),
		    							  iMsgLen);
    free(pstMsg);
    if (FALSE == iRet){
        SNMP_AGLINK_LOG(LOG_ERR, "WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_SSID_URL_BAW_ADD send fail");
    }
    return iRet;
}
int snmp_notify_wifidog_del_url_baw(unsigned int uiSsidNumber, int iNum, SNMPAGENT_URL_BAW_T *pstUrlBaw)
{
	WIFIDOG_SNMPAGENT_URL_BAW_CONFIG_T *pstMsg = NULL;
    int iMsgLen = 0;
    int iRet = -1;
    int iLoop = 0;

    iMsgLen = 8 + iNum*sizeof(WIFIOG_SNMPAGENT_URL_BAW_T);
    pstMsg = (WIFIDOG_SNMPAGENT_URL_BAW_CONFIG_T *)malloc(iMsgLen);
    if (NULL == pstMsg){
		SNMP_AGLINK_LOG(LOG_ERR, "NULL == pstMsg fail");
        return -1;
    }
    pstMsg->uiSsidNumber = uiSsidNumber;
    pstMsg->iNum = iNum;
    SNMP_AGLINK_LOG(LOG_INFO, "uiSsidNumber:%d iNum:%d.",
        pstMsg->uiSsidNumber, pstMsg->iNum = iNum);
    for(iLoop = 0; iLoop < iNum; iLoop++){
		if (NULL == (pstUrlBaw + iLoop)){
			SNMP_AGLINK_LOG(LOG_ERR, "NULL == (pstUrlBaw + %d) fail", iLoop);
            free(pstMsg);
            return -1;
        }
        pstMsg->astUrlBaw[iLoop].iType = pstUrlBaw[iLoop].iType;
        strncpy(pstMsg->astUrlBaw[iLoop].aucUrl, pstUrlBaw[iLoop].acUrl, 32);
        SNMP_AGLINK_LOG(LOG_INFO, "iLoop:%d iType:%d aucUrl:%s.",
            iLoop, pstMsg->astUrlBaw[iLoop].iType, 
            pstMsg->astUrlBaw[iLoop].aucUrl);
    }

    iRet = ag_module_ipc_client_send_msg( WIFIDOG_MSG_SNMPAGENT_DEL_BAW_URL,
		    							  AG_IPC_MSG_TYPE_SELF_CPU,
		    							  &g_stSnmpAglink,
		    							  (unsigned char *)(pstMsg),
		    							  iMsgLen);
    free(pstMsg);
    if (FALSE == iRet){
        SNMP_AGLINK_LOG(LOG_ERR, "WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_SSID_URL_BAW_ADD send fail");
    }
    return iRet;
}

int snmp_notify_wifidog_add_mac_baw(unsigned int uiSsidNumber, int iNum, SNMPAGENT_MAC_BAW_T *pstMacBaw)
{
	WIFIDOG_SNMPAGENT_MAC_BAW_CONFIG_T *pstMsg = NULL;
    int iMsgLen = 0;
    int iRet = -1;
    int iLoop = 0;

    iMsgLen = 8 + iNum*sizeof(WIFIOG_SNMPAGENT_MAC_BAW_T);
    pstMsg = (WIFIDOG_SNMPAGENT_MAC_BAW_CONFIG_T *)malloc(iMsgLen);
    if (NULL == pstMsg){
		SNMP_AGLINK_LOG(LOG_ERR, "NULL == pstMsg fail");
        return -1;
    }
    pstMsg->uiSsidNumber = uiSsidNumber;
    pstMsg->iNum = iNum;
    SNMP_AGLINK_LOG(LOG_INFO, "uiSsidNumber:%d iNum:%d.",
        pstMsg->uiSsidNumber, pstMsg->iNum = iNum);
    for(iLoop = 0; iLoop < iNum; iLoop++){
		if (NULL == (pstMacBaw + iLoop)){
			SNMP_AGLINK_LOG(LOG_ERR, "NULL == (pstMacBaw + %d) fail", iLoop);
            free(pstMsg);
            return -1;
        }
        pstMsg->astMacBaw[iLoop].iType = pstMacBaw[iLoop].iType;
        memcpy(pstMsg->astMacBaw[iLoop].aucMac, pstMacBaw[iLoop].aucMac, 6);
        SNMP_AGLINK_LOG(LOG_INFO, "iLoop:%d iType:%d aucMac["MAC_FMT"].",
            iLoop, pstMsg->astMacBaw[iLoop].iType, 
            MAC_ARG(pstMsg->astMacBaw[iLoop].aucMac));
    }

    iRet = ag_module_ipc_client_send_msg( WIFIDOG_MSG_SNMPAGENT_ADD_BAW_MAC,
		    							  AG_IPC_MSG_TYPE_SELF_CPU,
		    							  &g_stSnmpAglink,
		    							  (unsigned char *)(pstMsg),
		    							  iMsgLen);
    free(pstMsg);
    if (FALSE == iRet){
        SNMP_AGLINK_LOG(LOG_ERR, "WIFIDOG_MSG_SNMPAGENT_ADD_BAW_MAC send fail");
    }
    return iRet;
}
int snmp_notify_wifidog_del_mac_baw(unsigned int uiSsidNumber, int iNum, SNMPAGENT_MAC_BAW_T *pstMacBaw)
{
	WIFIDOG_SNMPAGENT_MAC_BAW_CONFIG_T *pstMsg = NULL;
    int iMsgLen = 0;
    int iRet = -1;
    int iLoop = 0;

    iMsgLen = 8 + iNum*sizeof(WIFIOG_SNMPAGENT_MAC_BAW_T);
    pstMsg = (WIFIDOG_SNMPAGENT_MAC_BAW_CONFIG_T *)malloc(iMsgLen);
    if (NULL == pstMsg){
		SNMP_AGLINK_LOG(LOG_ERR, "NULL == pstMsg fail");
        return -1;
    }
    pstMsg->uiSsidNumber = uiSsidNumber;
    pstMsg->iNum = iNum;
    SNMP_AGLINK_LOG(LOG_INFO, "uiSsidNumber:%d iNum:%d.",
        pstMsg->uiSsidNumber, pstMsg->iNum = iNum);
    for(iLoop = 0; iLoop < iNum; iLoop++){
		if (NULL == (pstMacBaw + iLoop)){
			SNMP_AGLINK_LOG(LOG_ERR, "NULL == (pstMacBaw + %d) fail", iLoop);
            free(pstMsg);
            return -1;
        }
        pstMsg->astMacBaw[iLoop].iType = pstMacBaw[iLoop].iType;
        memcpy(pstMsg->astMacBaw[iLoop].aucMac, pstMacBaw[iLoop].aucMac, 6);
        SNMP_AGLINK_LOG(LOG_INFO, "iLoop:%d iType:%d aucMac["MAC_FMT"].",
            iLoop, pstMsg->astMacBaw[iLoop].iType, 
            MAC_ARG(pstMsg->astMacBaw[iLoop].aucMac));
    }

    iRet = ag_module_ipc_client_send_msg( WIFIDOG_MSG_SNMPAGENT_DEL_BAW_MAC,
		    							  AG_IPC_MSG_TYPE_SELF_CPU,
		    							  &g_stSnmpAglink,
		    							  (unsigned char *)(pstMsg),
		    							  iMsgLen);
    free(pstMsg);
    if (FALSE == iRet){
        SNMP_AGLINK_LOG(LOG_ERR, "WIFIDOG_MSG_SNMPAGENT_DEL_BAW_MAC send fail");
    }
    return iRet;
}

void *snmp_aglink_client_handle(void *arg)
{
	unsigned int idset = 0;
    int iLoop = 0;
    unsigned int uiModuleId = 0;

	/*初始化*/
    memset(&g_stSnmpAllStaInfo, 0, sizeof(g_stSnmpAllStaInfo));
    SNMP_ALL_STA_MUTEX_INIT(g_stSnmpAllStaInfo.mutex);
    memset(&g_stSnmpAglink, 0, sizeof(g_stSnmpAglink));
#if 0    
    for (iLoop = SNMPAGENT_IDSET_BEGIN; iLoop < SNMPAGENT_IDSET_MAX; iLoop++){
		idset |= iLoop;
    }
#endif
    uiModuleId = (unsigned int)AG_GET_MODULE_ID(SNMP_MSG_BEGIN);
    ag_module_ipc_module_init(MODULE_NAME_SNMPAGENT, uiModuleId, &g_stSnmpAglink, snmp_aglink_msg_handle);    
    ag_module_ipc_power_on(&g_stSnmpAglink);
    return NULL;
}

int snmp_aglink_client_init()
{	
	int iRet = -1;
	pthread_t stThreadId;

	iRet = pthread_create(&stThreadId, NULL, snmp_aglink_client_handle, NULL);
	if (0 != iRet) 
	{
		SNMP_AGLINK_LOG(LOG_ERR, "Thread creation faied!");
		return FALSE;
	}
	SNMP_AGLINK_LOG(LOG_INFO, "Thread creation succeed!");

	return TRUE;
}

