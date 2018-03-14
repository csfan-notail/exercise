/******************************************************************************
* COPYRIGHT GBCOM
*******************************************************************************
* 功    能: 
* 修改历史: 
* 2012-9-10     冯静              新建
******************************************************************************/
#include <syslog.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "mini_snmpd.h"

#include "snmp_nms.h"

#include "my_snmp_def.h"
#include "my_snmp_data.h"
#include "my_snmp_oid.h"
#include "my_snmp_handle.h"
#include "my_snmp_util.h"
//#include "my_snmp_config.h"
#include "my_snmp_init.h"
#include "my_snmp_trap.h"
#include "my_snmp_alarm.h"

/*************************************************************************/
extern UINT32      g_ulRootOIDSize;
extern OID         g_rootOID[MAX_SNMP_OID_SIZE];
extern SNMP_AGENT_DATA_T *g_pstSnmpAgentData;
extern OSTOKEN gtSemConfig;	/* 配置互斥信号量 */
extern UINT8 g_aucDevMac[6];
extern CHAR gacConfigVersion[256];/*ccs 配置模板-版本号*/

/* BEGIN: Added by zoucaihong, 2015/6/9 */
extern UINT8 g_aucCmsIp[4];
/* END: Added by zoucaihong, 2015/6/9 */
extern int g_trap_port;

/*************************************************************************
* 函数名称: snmp_add_trap_var
* 功    能: 给Trap消息增加绑定变量
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
int snmp_add_trap_var(SNMP_VAR_T **ppstVarList, SNMP_OID *pstOID, UINT32 ulOIDSize, 
                        UINT16 usVarType, VOID* pVar, UINT32 ulVarLen)
{
	SNMP_OID astOID[32] = {0};
    UINT8 ucType = snmp_get_asn1_type(usVarType);

	if (memcmp(pstOID, oid_snmp_trap, sizeof(oid_snmp_trap))== 0)
	{
		memcpy(astOID, &(g_rootOID[0]), g_ulRootOIDSize*sizeof(OID));
		memcpy((UINT8 *)&(astOID[0])+(g_ulRootOIDSize*sizeof(OID)), pVar, 
			ulVarLen);
	    return snmp_varlist_add_var(ppstVarList, pstOID, ulOIDSize,
	                                ucType, 
	                                (unsigned char*)astOID, 
	                                g_ulRootOIDSize*sizeof(OID)+ulVarLen);
	}
	else
	{
		memcpy(astOID, &(g_rootOID[0]), g_ulRootOIDSize*sizeof(OID));
		memcpy((UINT8 *)&(astOID[0])+(g_ulRootOIDSize*sizeof(OID)), pstOID, 
			ulOIDSize*sizeof(OID));
		
	    return snmp_varlist_add_var(ppstVarList, astOID, g_ulRootOIDSize+ulOIDSize,
	                                ucType, 
	                                (unsigned char*)pVar, 
	                                ulVarLen);
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL test_snmp_trap(VOID)
{	
    SNMP_VAR_T              *pstVarList = NULL;
    OPEN_MIB_ALARM_TRAP_T   stAlarmTrap;// = {0};
	SNMP_SESSION_T          stSnmpSession;// = {0};
    int                     ret = -1; 

	stAlarmTrap.ulAlarmSn = 123456;
    sprintf(stAlarmTrap.acAlarmNeName, "22:11:33:33:33:33");
    stAlarmTrap.ulAlarmLevel = 3;
    stAlarmTrap.ulAlarmType = 1;
    stAlarmTrap.ulAlarmReasonID = 0;
    sprintf(stAlarmTrap.acAlarmReason, "no alarm reason");
    stAlarmTrap.ulAlarmRaiseTime = 100*1234567;
    stAlarmTrap.ulAlarmStatus = 1;
    sprintf(stAlarmTrap.acAlarmTitle, "alarm title - test");
    sprintf(stAlarmTrap.acAlarmInfo, "alarm info - test");

	/*binding variable*/
	SNMP_INIT_TRAP_VAR(pstVarList, oid_ap_alarm_trap, sizeof(oid_ap_alarm_trap));
	
	SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmSn,        TYPE_LONG, stAlarmTrap.ulAlarmSn);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmNeName,    TYPE_CHAR, stAlarmTrap.acAlarmNeName);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmLevel,     TYPE_LONG, stAlarmTrap.ulAlarmLevel);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmType,      TYPE_LONG, stAlarmTrap.ulAlarmType);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmReasonID,  TYPE_LONG, stAlarmTrap.ulAlarmReasonID);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmReason,    TYPE_CHAR, stAlarmTrap.acAlarmReason);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmRaiseTime, TYPE_TIME, stAlarmTrap.ulAlarmRaiseTime);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmStatus,    TYPE_LONG, stAlarmTrap.ulAlarmStatus);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmTitle,     TYPE_CHAR, stAlarmTrap.acAlarmTitle);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmInfo,      TYPE_CHAR, stAlarmTrap.acAlarmInfo);

	/* snmp session */
	stSnmpSession.aucAgentIP[0] = 10;
	stSnmpSession.aucAgentIP[1] = 1;
	stSnmpSession.aucAgentIP[2] = 200;
	stSnmpSession.aucAgentIP[3] = 45;
	stSnmpSession.usAgentPort = 162;
	strcpy(stSnmpSession.acCommunity, g_rocommunity);
	stSnmpSession.ulTimeout = 5;

    ret = snmp_send_trap2(&stSnmpSession, pstVarList);
	if(-1 == ret) {	return FALSE; }

	return TRUE;
}
BOOL snmp_send_mib_alarm_trap(OPEN_MIB_ALARM_TRAP_T *pstAlarmTrap)
{
	SNMP_VAR_T       *pstVarList = NULL;
	SNMP_SESSION_T stSnmpSession;
    int iRet = 0;
	
	if (NULL == pstAlarmTrap)
	{
		agent_printf(LOG_ERR, "%s:snmp_send_alarm_trap2 pstAlarmTrap is NULL\n", 
			SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}
	
	SNMP_INIT_TRAP_VAR(pstVarList, pstAlarmTrap->pstOID, pstAlarmTrap->ulOIDLen);
		
    SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmSn,       TYPE_LONG, pstAlarmTrap->ulAlarmSn);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmNeName,   TYPE_CHAR, pstAlarmTrap->acAlarmNeName);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmLevel,    TYPE_LONG, pstAlarmTrap->ulAlarmLevel);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmType,     TYPE_LONG, pstAlarmTrap->ulAlarmType);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmReasonID, TYPE_LONG, pstAlarmTrap->ulAlarmReasonID);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmReason,   TYPE_CHAR, pstAlarmTrap->acAlarmReason);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmRaiseTime,TYPE_LONG, pstAlarmTrap->ulAlarmRaiseTime);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmStatus,   TYPE_LONG, pstAlarmTrap->ulAlarmStatus);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmTitle,    TYPE_CHAR, pstAlarmTrap->acAlarmTitle);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmInfo,     TYPE_CHAR, pstAlarmTrap->acAlarmInfo);
    
    memset(&stSnmpSession, 0, sizeof(SNMP_SESSION_T));
    if ( FALSE == app_get_cms_ip(stSnmpSession.aucAgentIP) )
    {
        agent_printf(LOG_ERR, "Func:%s get cms ip fail.\n", __func__);
		snmp_free_varlist(pstVarList);
        return FALSE;
    }
    stSnmpSession.usAgentPort = g_trap_port;
    strcpy(stSnmpSession.acCommunity, 
        ('\0' == g_pstSnmpAgentData->pstApTrapDesConfig->acTrapCommunity[0]?
        g_rocommunity:
        g_pstSnmpAgentData->pstApTrapDesConfig->acTrapCommunity));
    stSnmpSession.ulTimeout = AP_AGENT_TRAP_TIMEOUT;
    iRet = snmp_send_trap2(&stSnmpSession, pstVarList);
	if(-1 == iRet) {	return FALSE; }
    
	return TRUE;
}
BOOL ap_agent_convert_alarm_trap(AP_ALARM_INFO_T *pstAlarm, 
									    OPEN_MIB_ALARM_TRAP_T *pstAlarmTrap)
{
	UINT8 *pucMac = NULL;
    UINT8 aucInvalidMac[6]={0};
    UINT32 ulCurrTime=0;

	if (NULL == pstAlarm)
	{
		agent_printf(LOG_ERR, "ap_agent_convert_alarm_trap() pstAlarm is null.\n");
		return FALSE;
	}

	if (NULL == pstAlarmTrap)
	{
		agent_printf(LOG_ERR, "ap_agent_convert_alarm_trap() pstAlarmTrap is null.\n");
		return FALSE;
	}
	
	pstAlarmTrap->ulAlarmSn = ap_agent_generate_trapsn();
    pucMac = g_pstSnmpAgentData->pstSysDevInfo->aucSysMacAddr;
	sprintf(pstAlarmTrap->acAlarmNeName, "%02x:%02x:%02x:%02x:%02x:%02x", 
				MAC_ARG(pucMac));

	pstAlarmTrap->ulAlarmLevel = pstAlarm->ulAlarmLevel;
    pstAlarmTrap->ulAlarmReasonID = pstAlarm->ulAlarmReasonID;
	pstAlarmTrap->ulAlarmStatus = pstAlarm->stAlarmKey.ulAlarmStatus;
		
	switch(pstAlarm->stAlarmKey.ulAlarmCode)
	{
		case ALARM_ID_APCOLDREBOOT://             1001               /* AP冷启动告警*/
			pstAlarmTrap->ulAlarmType = OPEN_MIB_ALARM_TYPE_DEVICE;
			if (pstAlarm->stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_RAISE)
			{
				sprintf(pstAlarmTrap->acAlarmTitle, "apColdStartTrap");
				pstAlarmTrap->pstOID = (OID *)oid_ap_alarm_sys_apColdStart_trap;
				pstAlarmTrap->ulOIDLen = sizeof(oid_ap_alarm_sys_apColdStart_trap);
			}
			break;
		case ALARM_ID_APWARMREBOOT://             1002               /* AP热启动告警*/
			pstAlarmTrap->ulAlarmType = OPEN_MIB_ALARM_TYPE_DEVICE;
			if (pstAlarm->stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_RAISE)
			{
				sprintf(pstAlarmTrap->acAlarmTitle, "apWarmStartTrap");
				pstAlarmTrap->pstOID = (OID *)oid_ap_alarm_sys_apWarmStart_trap;
				pstAlarmTrap->ulOIDLen = sizeof(oid_ap_alarm_sys_apWarmStart_trap);
			}
			break;
		case ALARM_ID_CPU_OVERLOAD://				1003    /* AP设备CPU负荷过高 */
			pstAlarmTrap->ulAlarmType = OPEN_MIB_ALARM_TYPE_DEVICE;
			if (pstAlarm->stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_RAISE)
			{	
				sprintf(pstAlarmTrap->acAlarmTitle, "apCpuUsageTooHighTrap");
				pstAlarmTrap->pstOID = (OID *)oid_ap_alarm_sys_apCpuUsageTooHigh_trap;
				pstAlarmTrap->ulOIDLen = sizeof(oid_ap_alarm_sys_apCpuUsageTooHigh_trap);
			}
			else if (pstAlarm->stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_CLEAR)
			{
				sprintf(pstAlarmTrap->acAlarmTitle, "apCpuUsageTooHighClearTrap");
				pstAlarmTrap->pstOID = (OID *)oid_ap_alarm_sys_apCpuUsageTooHighClear_trap;
				pstAlarmTrap->ulOIDLen = sizeof(oid_ap_alarm_sys_apCpuUsageTooHighClear_trap);
			}
			break;
		case ALARM_ID_MEM_OVERLOAD://	        	1004    /* AP设备内存负荷过高 */
			pstAlarmTrap->ulAlarmType = OPEN_MIB_ALARM_TYPE_DEVICE;
			if (pstAlarm->stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_RAISE)
			{
				sprintf(pstAlarmTrap->acAlarmTitle, "apMemUsageTooHighTrap");
				pstAlarmTrap->pstOID = (OID *)oid_ap_alarm_sys_apMemUsageTooHigh_trap;
				pstAlarmTrap->ulOIDLen = sizeof(oid_ap_alarm_sys_apMemUsageTooHigh_trap);
			}
			else if (pstAlarm->stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_CLEAR)
			{
				sprintf(pstAlarmTrap->acAlarmTitle, "apMemUsageTooHighClearTrap");
				pstAlarmTrap->pstOID = (OID *)oid_ap_alarm_sys_apMemUsageTooHighClear_trap;
				pstAlarmTrap->ulOIDLen = sizeof(oid_ap_alarm_sys_apMemUsageTooHighClear_trap);
			}
			break;
        /* BEGIN: Added by zoucaihong, 2014/6/27 */
		case ALARM_ID_DEV_OFFLINE://        1005    /* AP离线告警 */
			pstAlarmTrap->ulAlarmType = OPEN_MIB_ALARM_TYPE_DEVICE;
			if (pstAlarm->stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_RAISE)
			{
				sprintf(pstAlarmTrap->acAlarmTitle, "apFaultTrap");
				pstAlarmTrap->pstOID = (OID *)oid_ap_alarm_fault_trap;
				pstAlarmTrap->ulOIDLen = sizeof(oid_ap_alarm_fault_trap);
			}
			break;
        /* END: Added by zoucaihong, 2014/6/27 */
		case ALARM_ID_COINTERFRE://	        	2001    /* 同频干扰告警 */
			pstAlarmTrap->ulAlarmType = OPEN_MIB_ALARM_TYPE_QOS;
			if (pstAlarm->stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_RAISE)
			{
				sprintf(pstAlarmTrap->acAlarmTitle, "apCoInterfDetectedTrap");
				pstAlarmTrap->pstOID = (OID *)oid_ap_alarm_wids_cointerfre_trap;
				pstAlarmTrap->ulOIDLen = sizeof(oid_ap_alarm_wids_cointerfre_trap);
			}
			else if (pstAlarm->stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_CLEAR)
			{
				sprintf(pstAlarmTrap->acAlarmTitle, "apCoInterfClearTrap");
				pstAlarmTrap->pstOID = (OID *)oid_ap_alarm_wids_cointerfreClear_trap;
				pstAlarmTrap->ulOIDLen = sizeof(oid_ap_alarm_wids_cointerfreClear_trap);
			}
			break;
		case ALARM_ID_NEINTERFRE://	        	2002    /* 邻频干扰告警 */
			pstAlarmTrap->ulAlarmType = OPEN_MIB_ALARM_TYPE_QOS;
			if (pstAlarm->stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_RAISE)
			{
				sprintf(pstAlarmTrap->acAlarmTitle, "apNerborInterfDetectedTrap");
				pstAlarmTrap->pstOID = (OID *)oid_ap_alarm_wids_neinterfre_trap;
				pstAlarmTrap->ulOIDLen = sizeof(oid_ap_alarm_wids_neinterfre_trap);
			}
			else if (pstAlarm->stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_CLEAR)
			{
				sprintf(pstAlarmTrap->acAlarmTitle, "apNeiborInterfClearTrap");
				pstAlarmTrap->pstOID = (OID *)oid_ap_alarm_wids_neinterfreClear_trap;
				pstAlarmTrap->ulOIDLen = sizeof(oid_ap_alarm_wids_neinterfreClear_trap);
			}
			break;
		case ALARM_ID_WLANDEVINTERFRE://	        	2003    /* WLAN设备干扰告警 */
			pstAlarmTrap->ulAlarmType = OPEN_MIB_ALARM_TYPE_QOS;
			if (pstAlarm->stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_RAISE)
			{
				sprintf(pstAlarmTrap->acAlarmTitle, "wlanDevInterfDetectedTrap");
				pstAlarmTrap->pstOID = (OID *)oid_ap_alarm_wids_wlanDevInterfre_trap;
				pstAlarmTrap->ulOIDLen = sizeof(oid_ap_alarm_wids_wlanDevInterfre_trap);
			}
			else if (pstAlarm->stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_CLEAR)
			{
				sprintf(pstAlarmTrap->acAlarmTitle, "wlanDevInterfClearTrap");
				pstAlarmTrap->pstOID = (OID *)oid_ap_alarm_wids_wlanDevInterfreClear_trap;
				pstAlarmTrap->ulOIDLen = sizeof(oid_ap_alarm_wids_wlanDevInterfreClear_trap);
			}
			break;
		case ALARM_ID_OTHERDEVINTERFRE://	        	2004    /* 非WLAN设备干扰告警 */
			pstAlarmTrap->ulAlarmType = OPEN_MIB_ALARM_TYPE_QOS;
			if (pstAlarm->stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_RAISE)
			{
				sprintf(pstAlarmTrap->acAlarmTitle, "nonWlanDevInterfDetectedTrap");
				pstAlarmTrap->pstOID = (OID *)oid_ap_alarm_wids_otherDevInterfre_trap;
				pstAlarmTrap->ulOIDLen = sizeof(oid_ap_alarm_wids_otherDevInterfre_trap);
			}
			else if (pstAlarm->stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_CLEAR)
			{
				sprintf(pstAlarmTrap->acAlarmTitle, "nonWlanDevInterfClearTrap");
				pstAlarmTrap->pstOID = (OID *)oid_ap_alarm_wids_otherDevInterfreClear_trap;
				pstAlarmTrap->ulOIDLen = sizeof(oid_ap_alarm_wids_otherDevInterfreClear_trap);
			}
			break;
		default:			
			agent_printf(LOG_ERR, "%s:ap_agent_convert_alarm_trap invalid info:", SNMPAGENT_CONTROL_NAME);
			agent_printf(LOG_ERR, "ulAlarmCode       = %lu\n"
				"ulAulAlarmStatus  = %lu\n"
				"ulAlarmChangeTime = %lu\n"
				"ulAlarmSn         = %lu",
				pstAlarm->stAlarmKey.ulAlarmCode,
				pstAlarm->stAlarmKey.ulAlarmStatus,
				pstAlarm->stAlarmKey.ulAlarmChangeTime,
				pstAlarm->stAlarmKey.ulAlarmSn);			
			return FALSE;
	}
		
	if (pstAlarmTrap->ulAlarmStatus == AP_ALARM_STATUS_RAISE)
	{
		if (pstAlarm->ulAlarmRaiseTime != 0)
		{
			pstAlarmTrap->ulAlarmRaiseTime = pstAlarm->ulAlarmRaiseTime;
		}
		else
		{
			pstAlarmTrap->ulAlarmRaiseTime = pstAlarm->stAlarmKey.ulAlarmChangeTime;
		}
        pstAlarmTrap->ulAlarmStatus = ALARM_STATUS_ACTIVE;
	}
	else if(pstAlarmTrap->ulAlarmStatus == AP_ALARM_STATUS_CLEAR)
	{
		pstAlarmTrap->ulAlarmRaiseTime = pstAlarm->ulAlarmClearTime;
        pstAlarmTrap->ulAlarmStatus = ALARM_STATUS_CLEAR;
	}
	else
	{
		pstAlarmTrap->ulAlarmRaiseTime = pstAlarm->stAlarmKey.ulAlarmChangeTime;
	}

	if ('\0' == pstAlarm->acAlarmExtInfo[0])
	{
		if (NULL != pucMac && memcmp(pucMac, aucInvalidMac, 6) != 0)
		{
			ulCurrTime = (UINT32)time(NULL);		
			sprintf(pstAlarmTrap->acAlarmInfo, "mac=%02x:%02x:%02x:%02x:%02x:%02x&time=%s",
    			MAC_ARG(pucMac),
    			ap_agent_convert_time_str(ulCurrTime));
		}
		else
		{
			sprintf(pstAlarmTrap->acAlarmInfo, "%s", pstAlarm->acAlarmExtInfo);
		}
	}
	else
	{
		sprintf(pstAlarmTrap->acAlarmInfo, "%s", pstAlarm->acAlarmExtInfo);
	}
		
	return TRUE;
}
            
BOOL ap_agent_send_alarm_trap(AP_ALARM_INFO_T *pstAlarm)
{
	OPEN_MIB_ALARM_TRAP_T stAlarmTrap = {0};
	BOOL bRet = FALSE;
    int iRet = -1;
        
	if (NULL == pstAlarm)
	{
		agent_printf(LOG_ERR, "%s:ap_agent_send_alarm_trap pstAlarm is null\n",
			SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}

	/* 加上互斥锁 */
	if (!OsSemaphoreP(gtSemConfig, WAIT_FOREVER, 1))
	{
		agent_printf(LOG_ERR,
			"gtSemConfig OsSemaphoreP fail\n");
    	return FALSE;
	}
			
	agent_printf(LOG_INFO, "pstAlarm->stAlarmKey.ulAlarmCode:%lu\n"
		"pstAlarm->stAlarmKey.ulAlarmStatus:%lu\n"
		"pstAlarm->ulAlarmRaiseTime:%lu\n"
		"pstAlarm->stAlarmKey.ulAlarmChangeTime:%lu\n"
		"pstAlarm->ulAlarmClearTime:%lu\n"
		"pstAlarm->stAlarmKey.ulAlarmSn:%lu\n"
		"pstAlarm->ulAlarmReasonID:%lu\n"
		"pstAlarm->ulAlarmLevel:%lu\n"
		,
		pstAlarm->stAlarmKey.ulAlarmCode,
		pstAlarm->stAlarmKey.ulAlarmStatus,
		pstAlarm->ulAlarmRaiseTime,
		pstAlarm->stAlarmKey.ulAlarmChangeTime,
		pstAlarm->ulAlarmClearTime,
		pstAlarm->stAlarmKey.ulAlarmSn,
		pstAlarm->ulAlarmReasonID,
		pstAlarm->ulAlarmLevel
		);
	
	bRet = ap_agent_convert_alarm_trap(pstAlarm, &stAlarmTrap);
	if ( !bRet)
	{
		agent_printf(LOG_ERR, "ap_agent_convert_alarm_trap failed.\n");
	}
	
	if (stAlarmTrap.acAlarmTitle[0] == '\0')
	{
		agent_printf(LOG_INFO, "%s:no need to send trap:%lu\n", SNMPAGENT_CONTROL_NAME, 
			pstAlarm->stAlarmKey.ulAlarmCode);
        goto error;
	}
    
	iRet = snmp_send_mib_alarm_trap(&stAlarmTrap);
	if (-1 == iRet)
	{
		agent_printf(LOG_ERR, "%s:snmp_send_alarm_trap failed\n", 
			SNMPAGENT_CONTROL_NAME);
		goto error;
	}

	OsSemaphoreV(gtSemConfig, 1);
    return TRUE;
	
error:
	OsSemaphoreV(gtSemConfig, 1);
	return FALSE;
}

/*****************************************************************************
 Prototype    : agent_start_discover
 Description  : 设备向CMS发送discover trap
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/5/13
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL agent_start_discover()
{
    SNMP_VAR_T              *pstVarList = NULL;
	SNMP_SESSION_T          stSnmpSession;
    int                     ret = -1; 

    /* BEGIN: Added by zoucaihong, 2014/7/3 */
    /* 获取设备IP */
    if ( FALSE == app_get_dev_ip() )
    {
        agent_printf(LOG_ERR, "Func:%s get dev ip fail.\n", __func__);
        return FALSE;
    }
    /* END: Added by zoucaihong, 2014/7/3 */
    /* snmp session */
    if ( FALSE == app_get_cms_ip(stSnmpSession.aucAgentIP) )
    {
        agent_printf(LOG_ERR, "Func:%s get cms ip fail.\n", __func__);
        return FALSE;
    }
    stSnmpSession.usAgentPort = g_trap_port;
	strcpy(stSnmpSession.acCommunity, g_rocommunity);
	stSnmpSession.ulTimeout = 5;

	/* binding variable */
	SNMP_INIT_TRAP_VAR(pstVarList, oid_dev_discover_trap, sizeof(oid_dev_discover_trap));
    
	SNMP_ADD_TRAP_VAR(pstVarList, oid_dev_mac,          TYPE_MAC,  g_aucDevMac);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_dev_ip,           TYPE_IP,    g_pstSnmpAgentData->pstSysDevInfo->aucIpAddress);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_dev_managestate,  TYPE_LONG,   g_pstSnmpAgentData->pstSysDevInfo->ulSysManageState);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_dev_model,        TYPE_LONG,   g_pstSnmpAgentData->pstSysDevInfo->ulSysModel);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_dev_software_ver, TYPE_CHAR,  g_pstSnmpAgentData->pstSysSwInfo->acSoftwareVersion);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_dev_software_oemver, TYPE_CHAR,  g_pstSnmpAgentData->pstSysSwInfo->acOemVersion);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_dev_hardware_ver, TYPE_CHAR,  g_pstSnmpAgentData->pstSysHwInfo->acHardwareVersion);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_dev_modelname,    TYPE_CHAR,  g_pstSnmpAgentData->pstSysDevInfo->acInterModel);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_dev_hardware_type,TYPE_LONG,  g_pstSnmpAgentData->pstSysHwInfo->ulHardwareType);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_dev_board_ver,    TYPE_LONG,  g_pstSnmpAgentData->pstSysHwInfo->ulBoardVersion);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_dev_sysModel,     TYPE_CHAR,  g_pstSnmpAgentData->pstSysDevInfo->acSysModelName);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_dev_type,         TYPE_LONG,  g_pstSnmpAgentData->pstSysDevInfo->ulDevType);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_dev_sysName,      TYPE_CHAR,  g_pstSnmpAgentData->pstSysDevInfo->acSysName);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_dev_hotId,        TYPE_CHAR,  g_pstSnmpAgentData->pstSysDevInfo->acHotId);

        /*2015-10-22 ccs 配置模板版本号*/
        SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_oper_sysoper_sendofflineconfigfile,         TYPE_CHAR,  gacConfigVersion);

    ret = snmp_send_trap2(&stSnmpSession, pstVarList);
	if(-1 == ret) 
    {
    	agent_printf(LOG_ERR,"snmp_send_trap2 fail");
        return FALSE; 
    }
    else
    {
    	agent_printf(LOG_DEBUG,"snmp_send_discover success!!");
    }

	return TRUE;
}
/*****************************************************************************
 Prototype    : agent_send_heart_beat_trap
 Description  : 发送心跳trap
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/8/25
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL agent_send_heart_beat_trap()
{
    SNMP_VAR_T              *pstVarList = NULL;
	SNMP_SESSION_T          stSnmpSession;
    int                     ret = -1; 
    
	/* binding variable */
	SNMP_INIT_TRAP_VAR(pstVarList, oid_ap_alarm_heart_trap, sizeof(oid_ap_alarm_heart_trap));
    
	SNMP_ADD_TRAP_VAR(pstVarList, oid_dev_mac,          TYPE_MAC,   g_aucDevMac);
#ifndef GW_X86
	SNMP_ADD_TRAP_VAR(pstVarList, oid_wifi0_channel,    TYPE_LONG,  g_pstSnmpAgentData->pstApWifiChannelConfig->ulChannelUsing);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_wifi0_txpower,    TYPE_LONG,  g_pstSnmpAgentData->pstApWifiPowerConfig->ulTransmitPower);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_wifi1_channel,    TYPE_LONG,  g_pstSnmpAgentData->pstApWifiChannel5Config->ulChannelUsing);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_wifi1_txpower,    TYPE_LONG,  g_pstSnmpAgentData->pstApWifiPower5Config->ulTransmitPower);
#endif
        
	SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_oper_sysoper_sendofflineconfigfile,    TYPE_CHAR,  gacConfigVersion);
    /* snmp session */
    memcpy(stSnmpSession.aucAgentIP, g_aucCmsIp, 4);
    stSnmpSession.usAgentPort = g_trap_port;
	strcpy(stSnmpSession.acCommunity, g_rocommunity);
	stSnmpSession.ulTimeout = 5;

    ret = snmp_send_inform(&stSnmpSession, pstVarList);
	if(-1 == ret) 
    {
    	agent_printf(LOG_ERR,"Func:%s fail", __func__);
        return FALSE; 
    }
    else
    {
    	agent_printf(LOG_DEBUG,"Func:%s success!!", __func__);
    }

	return TRUE;
}
BOOL agent_send_apswupdatetrap()
{
    SNMP_VAR_T              *pstVarList = NULL;
	SNMP_SESSION_T          stSnmpSession;
    int                     ret = -1; 
	/*binding variable
    1: sysMacAddress
	2: softwareVersion
	3: targetVersion
	4: transStatus
	*/
	/*构建初始化trap报文头*/
	SNMP_INIT_TRAP_VAR(pstVarList, oid_ap_alarm_sys_apSoftwareUpdate_trap, sizeof(oid_ap_alarm_sys_apSoftwareUpdate_trap));
    
	SNMP_ADD_TRAP_VAR(pstVarList, oid_dev_mac,          TYPE_MAC,       g_aucDevMac);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_dev_software_ver, TYPE_CHAR,      g_pstSnmpAgentData->pstSysSwInfo->acSoftwareVersion);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_dev_software_targetver,   TYPE_CHAR,   g_pstSnmpAgentData->pstSysSwInfo->acTargetVersion);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_filetrans_status,         TYPE_LONG,   g_pstSnmpAgentData->pstApFileTransConfig->ulTransStatus);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_file_type,        TYPE_LONG,      g_pstSnmpAgentData->pstApFileTransConfig->ulFileType);

    /* snmp session */
    memcpy(stSnmpSession.aucAgentIP, g_aucCmsIp, 4);
    stSnmpSession.usAgentPort = g_trap_port;
	strcpy(stSnmpSession.acCommunity, g_rocommunity);
	stSnmpSession.ulTimeout = 5;

    ret = snmp_send_trap2(&stSnmpSession, pstVarList);
	if(-1 == ret) 
    {
    	agent_printf(LOG_ERR,"snmp_send_trap2 fail");
        return FALSE; 
    }
    else
    {
    	agent_printf(LOG_DEBUG,"agent_send_apswupdatetrap success!!");
    }

	return TRUE;
}

/* BEGIN: Added by zoucaihong, 2014/6/27 */
BOOL agent_send_dev_offline_trap()
{
    AP_ALARM_INFO_T stAlarmInfo;

    memset(&stAlarmInfo, 0, sizeof(AP_ALARM_INFO_T));
	stAlarmInfo.stAlarmKey.ulAlarmCode = ALARM_ID_DEV_OFFLINE;
	stAlarmInfo.stAlarmKey.ulAlarmStatus = AP_ALARM_STATUS_RAISE;

    if ( FALSE == ap_agent_send_alarm_trap(&stAlarmInfo))	
    {
        agent_printf(LOG_ERR, "%s ulAlarmCode:%u fail.", __func__, ALARM_ID_DEV_OFFLINE);
        return FALSE;
    }
    agent_printf(LOG_INFO, "%s ulAlarmCode:%u success.", __func__, ALARM_ID_DEV_OFFLINE);

	return TRUE;
}
/* END: Added by zoucaihong, 2014/6/27 */



/* 向CCS发送配置模板更新结果TRAP */
BOOL agent_send_configfileupdate_trap(UINT32 ulResult)
{
    SNMP_VAR_T              *pstVarList = NULL;
	SNMP_SESSION_T          stSnmpSession;
    int                     ret = -1; 
	/*binding variable
    1: trap result
	*/
	/*构建初始化trap报文头*/
	SNMP_INIT_TRAP_VAR(pstVarList, oid_ap_alarm_configfileupdate_trap, sizeof(oid_ap_alarm_configfileupdate_trap));
    
	SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarm_configfileupdate_trap_result,          TYPE_LONG,       ulResult);
	
    /* snmp session */
    memcpy(stSnmpSession.aucAgentIP, g_pstSnmpAgentData->pstApTrapDesConfig->aucTrapDesIPAddress, 4);
    stSnmpSession.usAgentPort = 162;
	strcpy(stSnmpSession.acCommunity, g_rocommunity);
	stSnmpSession.ulTimeout = 5;

    ret = snmp_send_trap2(&stSnmpSession, pstVarList);
	if(-1 == ret) 
    {
    	agent_printf(LOG_ERR,"snmp_send_trap2 fail");
        return FALSE; 
    }
    else
    {
    	agent_printf(LOG_DEBUG,"agent_send_configfileupdate_trap success!!");
    }

	return TRUE;
}


