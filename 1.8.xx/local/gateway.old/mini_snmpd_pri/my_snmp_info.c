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

#include "my_snmp_def.h"
#include "my_snmp_data.h"
#include "my_snmp_handle.h"
#include "my_snmp_util.h"
#include "my_snmp_config.h"
#include "my_snmp_info.h"
#include "snmp_interface.h"

//#include "app_public.h"
#include "tool.h"
#include "config_common.h"
#include "nvram_config.h"

#ifndef GW_X86
#ifdef OPENWRT
#include "pal_network.h"
#include "pal_wifi.h"
#else
#include "gbcom_wifi_pal.h"
#endif
#endif
/*********************************全局变量*********************************/
extern SNMP_AGENT_DATA_T *g_pstSnmpAgentData;
extern AGENT_INIT_STATE_T g_stInitState;
extern int g_stati_per;
//extern pthread_mutex_t g_ap_list_mutex;
//extern  struct hlist_head g_ap_HList[AP_HASH_SIZE];
//extern pthread_mutex_t g_sta_list_mutex;
//extern  struct hlist_head g_sta_HList[STA_HASH_SIZE];

#if 0
/* BEGIN: Added by zoucaihong, 2012/12/4 */
BOOL agent_get_system()
{
    BOOL bRet = FALSE;

    if ( NULL == g_pstSnmpAgentData )
    {
        agent_printf(LOG_ERR, "File:%s Line:%d g_pstSnmpAgentData is null.\n",
            __FILE__, __LINE__);
        return FALSE;
    }

    if ( NULL == g_pstSnmpAgentData->pstSystem )
    {
        g_pstSnmpAgentData->pstSystem = SNMP_AGENT_MALLOC(sizeof(SYSTEM_T));
        if ( NULL == g_pstSnmpAgentData->pstSystem )
        {
            agent_printf(LOG_ERR, "File:%s, Line:%d g_pstSnmpAgentData->pstSystem is null.\n",
                __FILE__, __LINE__);
            return FALSE;
        }
    }
    memset(g_pstSnmpAgentData->pstSystem, 0, sizeof(SYSTEM_T));

    bRet = app_get_system(g_pstSnmpAgentData->pstSystem);
    if ( !bRet )
    {
        agent_printf(LOG_ERR, "File:%s, Line:%d, app_get_system failed.\n",
            __FILE__, __LINE__);
        return FALSE;
    }

    return TRUE;
}

BOOL agent_get_interfaces()
{
    BOOL bRet = FALSE;
    UINT32 ulTableMaxRecNum = TABLE_MAXRECNUM_INTERFACES;
    UINT32 ulOldIfAdminStatus = 0;

    if ( NULL == g_pstSnmpAgentData )
    {
        agent_printf(LOG_ERR, "File:%s Line:%d g_pstSnmpAgentData is null.\n",
            __FILE__, __LINE__);
        return FALSE;
    }

    if ( NULL == g_pstSnmpAgentData->pstInterfaces )
    {
        g_pstSnmpAgentData->pstInterfaces = SNMP_AGENT_MALLOC(sizeof(INTERFACES_ENTRY_T)*ulTableMaxRecNum);
        if ( NULL == g_pstSnmpAgentData->pstInterfaces )
        {
            agent_printf(LOG_ERR, "File:%s Line:%d g_pstSnmpAgentData->pstInterfaces is null.\n",
                __FILE__, __LINE__);
            return FALSE;
        }
        memset(g_pstSnmpAgentData->pstInterfaces, 0, sizeof(INTERFACES_ENTRY_T)*ulTableMaxRecNum);
    }
    ulOldIfAdminStatus = g_pstSnmpAgentData->pstInterfaces[0].ulIfAdminStatus;

    if ( FALSE == g_stInitState.bInitApConfigData )
    {
        g_pstSnmpAgentData->pstInterfaces[0].ulIfLastChange = 0;
        g_pstSnmpAgentData->pstInterfaces[0].ulIfLastTime = 0;
    }
    
    bRet = route_get_interfaces(&g_pstSnmpAgentData->pstInterfaces[0]);
    if ( !bRet )
    {
        agent_printf(LOG_ERR, "File:%s Line:%d route_get_interfaces failed.\n",
            __FILE__, __LINE__);
        return FALSE;
    }
    if ( ulOldIfAdminStatus != g_pstSnmpAgentData->pstInterfaces[0].ulIfAdminStatus)
    {
        g_pstSnmpAgentData->pstInterfaces[0].ulIfLastTime = snmp_get_systime();
    }
    
    g_pstSnmpAgentData->stInterfaces.ulIfNumber = 1;

    return TRUE;
}
/* END: Added by zoucaihong, 2012/12/4 */
#endif
BOOL ac_agent_get_apbasicsetup_config()
{
    BOOL bRet = FALSE;

	if (g_pstSnmpAgentData == NULL)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}

	if (g_pstSnmpAgentData->pstSysDevInfo == NULL)
	{
		g_pstSnmpAgentData->pstSysDevInfo = SNMP_AGENT_MALLOC(sizeof(AP_CONFIG_SYS_DEVINFO_T));
		if (g_pstSnmpAgentData->pstSysDevInfo == NULL)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstSysDevInfo failed\n",
				SNMPAGENT_CONTROL_NAME);
				return FALSE;
		}
    	memset(g_pstSnmpAgentData->pstSysDevInfo, 0, sizeof(AP_CONFIG_SYS_DEVINFO_T));
	}
    g_pstSnmpAgentData->pstSysDevInfo->ulRegetPeriod = g_stati_per;
    /* BEGIN: Added by zoucaihong, 2013/4/24 */
    g_pstSnmpAgentData->pstSysDevInfo->ulTrapSwitch = g_traps_switch;
    /* END: Added by zoucaihong, 2013/4/24 */
    bRet = app_get_sys_devinfo(g_pstSnmpAgentData->pstSysDevInfo);
    if ( FALSE == bRet )
    {
        agent_printf(LOG_ERR,"%s: app_get_sys_devinfo failed.\n",SNMPAGENT_CONTROL_NAME);
        return FALSE;
    }

    return TRUE;
}
#ifdef OPENWRT
BOOL ac_agent_get_apipsetup_config()
{
    INT32 ret;
    UINT32 ulNum = 0;
    UINT32 ulTableMaxRecNum = TABLE_MAXRECNUM_IP;
    SYS_IP_ENTRY_T stSysIpInfo;
    memset(&stSysIpInfo,0,sizeof(SYS_IP_ENTRY_T));
    memcpy(&stSysIpInfo.aucInterfaceName,"wan",strlen("wan"));

    if (NULL == g_pstSnmpAgentData)
    {
    	agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
    	return FALSE;
    }
    
    if (NULL == g_pstSnmpAgentData->pstSysIpConfig)
    {
    	g_pstSnmpAgentData->pstSysIpConfig = SNMP_AGENT_MALLOC(
            sizeof(AP_CONFIG_IP_ENTRY_T)*ulTableMaxRecNum);
    	if (NULL == g_pstSnmpAgentData->pstSysIpConfig)
    	{
    		agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstSysIpConfig failed\n",
    			SNMPAGENT_CONTROL_NAME);
    		return FALSE;
    	}
    }
    memset(g_pstSnmpAgentData->pstSysIpConfig, 0, 
		sizeof(AP_CONFIG_IP_ENTRY_T)*ulTableMaxRecNum);

    g_pstSnmpAgentData->stSysIpConfig.ulNum = 0;

    ret = pal_network_ip_info_get(&stSysIpInfo);
    if(ret != OK){
        ulNum = 0;
        return FALSE;
    }
    g_pstSnmpAgentData->pstSysIpConfig[0].ulIpMode = stSysIpInfo.ulIpMode;
    *(UINT32 *)g_pstSnmpAgentData->pstSysIpConfig[0].aucIpAddress = *(UINT32 *)stSysIpInfo.aucIpAddress;
    *(UINT32 *)g_pstSnmpAgentData->pstSysIpConfig[0].aucIpNetMask = *(UINT32 *)stSysIpInfo.aucIpNetMask;
    *(UINT32 *)g_pstSnmpAgentData->pstSysIpConfig[0].aucDnsMaster = *(UINT32 *)stSysIpInfo.aucDnsMaster;
    *(UINT32 *)g_pstSnmpAgentData->pstSysIpConfig[0].aucGwAddress = *(UINT32 *)stSysIpInfo.aucGwAddress;
    ulNum = 1;
   
    g_pstSnmpAgentData->stSysIpConfig.ulNum =ulNum;
    return TRUE;
}
#else
/*****************************************************************************
 Prototype    : ac_agent_get_apipsetup_config
 Description  : 获取IP配置信息
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/2/25
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL ac_agent_get_apipsetup_config()
{
    UINT32 ulTableMaxRecNum = TABLE_MAXRECNUM_IP;
    UINT32 ulNum = 0;
	char acWanIpAddr[32] = "";
	char acWanNetMask[32] = "";
    int iSysIpAddr[4] = {0};
    int iSysIpNetMask[4] = {0};
    int iLoop = 0;
    CHAR    acNvramName[32] = {0};
    CHAR    acNvramValue[32] = {0};
    char acSysGwddr[NVRAM_SIZE]  = {0};
    char acIpMode[NVRAM_SIZE] = {0};
    UINT8   aucGwAddr[4] = {0};
    UINT32  ulIpMode = 0;

	if (NULL == g_pstSnmpAgentData)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}
    
	if (NULL == g_pstSnmpAgentData->pstSysIpConfig)
	{
		g_pstSnmpAgentData->pstSysIpConfig = SNMP_AGENT_MALLOC(
            sizeof(AP_CONFIG_IP_ENTRY_T)*ulTableMaxRecNum);
		if (NULL == g_pstSnmpAgentData->pstSysIpConfig)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstSysIpConfig failed\n",
				SNMPAGENT_CONTROL_NAME);
			return FALSE;
		}
	}
	memset(g_pstSnmpAgentData->pstSysIpConfig, 0, 
		sizeof(AP_CONFIG_IP_ENTRY_T)*ulTableMaxRecNum);

    g_pstSnmpAgentData->stSysIpConfig.ulNum = 0;


    /* 获取网关地址 */
    if ( NvramResultSuccess == nvram_get(acSysGwddr, NVRAM_IPv4"_"NVRAM_IP_GW, NVRAM_SIZE) )
    {
        sscanf(acSysGwddr, "%d.%d.%d.%d", 
            &iSysIpAddr[0], &iSysIpAddr[1], &iSysIpAddr[2], &iSysIpAddr[3]);
        for (iLoop = 0; iLoop < 4; iLoop++)
        {
            aucGwAddr[iLoop] = iSysIpAddr[iLoop];
        }
    }
    /* 获取IP模式 */
    if ( NvramResultSuccess == nvram_get(acIpMode, NVRAM_IPv4"_"NVRAM_IP_METHOD, NVRAM_SIZE) )
    {
        ulIpMode = atoi(acIpMode);            
    }
    
    if ( 0 == nvram_get(acWanIpAddr, NVRAM_IPv4"_0_"NVRAM_IP_ADDRESS, NVRAM_SIZE) )
    {
        if ( '\0' != acWanIpAddr[0] )
        {
            sscanf(acWanIpAddr, "%d.%d.%d.%d", 
                &iSysIpAddr[0], &iSysIpAddr[1], &iSysIpAddr[2], &iSysIpAddr[3]);
            for (iLoop = 0; iLoop < 4; iLoop++)
            {
                g_pstSnmpAgentData->pstSysIpConfig[0].aucIpAddress[iLoop] = iSysIpAddr[iLoop];
            }
            g_pstSnmpAgentData->pstSysIpConfig[0].ulIndex = 1;
            if ( 0 == nvram_get(acWanNetMask, NVRAM_IPv4"_0_"NVRAM_IP_MASK, NVRAM_SIZE) )
            {
                if ( '\0' != acWanNetMask[0] )
                {
                    sscanf(acWanNetMask, "%d.%d.%d.%d", 
                        &iSysIpNetMask[0], &iSysIpNetMask[1], &iSysIpNetMask[2], &iSysIpNetMask[3]);
                    for (iLoop = 0; iLoop < 4; iLoop++)
                    {
                        g_pstSnmpAgentData->pstSysIpConfig[0].aucIpNetMask[iLoop] = iSysIpNetMask[iLoop];
                    }
                }
            }
            /* dns */
            sprintf(acNvramName, NVRAM_IPv4"_0_"NVRAM_IP_DNS);
            if(NvramResultSuccess != nvram_get(acNvramValue, acNvramName, NVRAM_LEN))
            {
                agent_printf(LOG_ERR, "get dns failed.\n");
            }
            else
            {
                if(OK != string_ip_tools(acNvramValue, (char *)g_pstSnmpAgentData->pstSysIpConfig[0].aucDnsMaster))
                {
                    agent_printf(LOG_ERR, "string dns[0] failed.\n");
                }
            }
            sprintf(acNvramName, NVRAM_IPv4"_1_"NVRAM_IP_DNS);
            if(NvramResultSuccess != nvram_get(acNvramValue, acNvramName, NVRAM_LEN))
            {
                agent_printf(LOG_ERR, "get dns failed.\n");
            }
            else
            {
                if(OK != string_ip_tools(acNvramValue, (char *)g_pstSnmpAgentData->pstSysIpConfig[0].aucDnsSlave))
                {
                    agent_printf(LOG_ERR, "string dns[1] failed.\n");
                }
            }

            memcpy(g_pstSnmpAgentData->pstSysIpConfig[0].aucGwAddress, aucGwAddr, 4);
            g_pstSnmpAgentData->pstSysIpConfig[0].ulIpMode = ulIpMode;
            ulNum++;
        }
    }    
    g_pstSnmpAgentData->stSysIpConfig.ulNum = ulNum;
    
    return TRUE;
}
#endif
BOOL ac_agent_get_apswinfo()
{
    BOOL bRet = FALSE;
    
	if (NULL == g_pstSnmpAgentData)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}

	if (g_pstSnmpAgentData->pstSysSwInfo == NULL)
	{
		g_pstSnmpAgentData->pstSysSwInfo = SNMP_AGENT_MALLOC(sizeof(AP_CONFIG_SYS_SWINFO_T));
		if (g_pstSnmpAgentData->pstSysSwInfo == NULL)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstSysSwInfo failed\n",
	    			SNMPAGENT_CONTROL_NAME);
			return FALSE;
		}
	}
	memset(g_pstSnmpAgentData->pstSysSwInfo, 0, sizeof(AP_CONFIG_SYS_SWINFO_T));

    bRet = app_get_swinfo(g_pstSnmpAgentData->pstSysSwInfo);
    if ( FALSE == bRet )
    {
        agent_printf(LOG_ERR,"%s: app_get_swinfo failed.\n",SNMPAGENT_CONTROL_NAME);
        return FALSE;
    }

    return TRUE;
}
BOOL ac_agent_get_aphwinfo()
{
    BOOL bRet = FALSE;
    
	if (NULL == g_pstSnmpAgentData)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}

	if (g_pstSnmpAgentData->pstSysHwInfo == NULL)
	{
		g_pstSnmpAgentData->pstSysHwInfo = SNMP_AGENT_MALLOC(sizeof(AP_CONFIG_SYS_HWINFO_T));
		if (g_pstSnmpAgentData->pstSysHwInfo == NULL)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstSysHwInfo failed\n",
	    			SNMPAGENT_CONTROL_NAME);
			return FALSE;
		}
	}
	memset(g_pstSnmpAgentData->pstSysHwInfo, 0, sizeof(AP_CONFIG_SYS_HWINFO_T));

    bRet = app_get_hdinfo(g_pstSnmpAgentData->pstSysHwInfo);
    if ( FALSE == bRet )
    {
        agent_printf(LOG_ERR,"app_get_hdinfo failed.\n");
        return FALSE;
    }

    return TRUE;
}
#if 0
/* BEGIN: Added by zoucaihong, 2013/5/13 */
/*****************************************************************************
 Prototype    : ac_agent_get_aplegalap_config
 Description  : 获取AP合法AP配置
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/5/13
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL ac_agent_get_aplegalap_config()
{
    UINT32  ulTableMaxRecNum = TABLE_MAXRECNUM_LEGALAP;
    UINT32  ulRecNum = 0;
    
	if (NULL == g_pstSnmpAgentData)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}
    
	if (NULL == g_pstSnmpAgentData->pstSysLegalApConfig)
	{
		g_pstSnmpAgentData->pstSysLegalApConfig = SNMP_AGENT_MALLOC(
            sizeof(AP_CONFIG_LEGALAP_ENTRY_T)*ulTableMaxRecNum);
		if (NULL == g_pstSnmpAgentData->pstSysLegalApConfig)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstSysLegalApConfig failed\n",
				SNMPAGENT_CONTROL_NAME);
			return FALSE;
		}
	}
	memset(g_pstSnmpAgentData->pstSysLegalApConfig, 0, 
		sizeof(AP_CONFIG_LEGALAP_ENTRY_T)*ulTableMaxRecNum);

    g_pstSnmpAgentData->stSysLegalApConfig.ulNum = 0;

    if ( FALSE == app_get_ap_legalap(g_pstSnmpAgentData->pstSysLegalApConfig, &ulRecNum) )
    {
        agent_printf(LOG_ERR, "app_get_ap_legalap fail.");
        return FALSE;
    }
    g_pstSnmpAgentData->stSysLegalApConfig.ulNum = ulRecNum;

    return TRUE;
}
/*****************************************************************************
 Prototype    : ac_agent_get_apflowctl_config
 Description  : 获取AP级流控配置
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/5/13
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL ac_agent_get_apflowctl_config()
{    
	if (NULL == g_pstSnmpAgentData)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}
    
	if (NULL == g_pstSnmpAgentData->pstSysFlowCtl)
	{
		g_pstSnmpAgentData->pstSysFlowCtl = SNMP_AGENT_MALLOC(sizeof(AP_CONFIG_FLOWCTL_T));
		if (NULL == g_pstSnmpAgentData->pstSysFlowCtl)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstSysFlowCtl failed\n",
				SNMPAGENT_CONTROL_NAME);
			return FALSE;
		}
	}
	memset(g_pstSnmpAgentData->pstSysFlowCtl, 0, sizeof(AP_CONFIG_FLOWCTL_T));

    if ( FALSE == app_get_ap_flowctl(g_pstSnmpAgentData->pstSysFlowCtl) )
    {
        agent_printf(LOG_ERR, "app_get_ap_flowctl fail.");
        return FALSE;
    }

    return TRUE;
}
/* END: Added by zoucaihong, 2013/5/13 */
/*****************************************************************************
 Prototype    : ac_agent_get_ap5gpriorsetup_config
 Description  : 获取AP 5G优先配置函数
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/3/11
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL ac_agent_get_ap5gpriorsetup_config()
{
    BOOL bRet = FALSE;

	if (g_pstSnmpAgentData == NULL)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}

	if (g_pstSnmpAgentData->pstAdvance5GPrior == NULL)
	{
		g_pstSnmpAgentData->pstAdvance5GPrior = SNMP_AGENT_MALLOC(sizeof(AP_CONFIG_ADVANCE_5GPRIOR_T));
		if (g_pstSnmpAgentData->pstAdvance5GPrior == NULL)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstAdvance5GPrior failed\n",
				SNMPAGENT_CONTROL_NAME);
				return FALSE;
		}
	}
	memset(g_pstSnmpAgentData->pstAdvance5GPrior, 0, sizeof(AP_CONFIG_ADVANCE_5GPRIOR_T));

    bRet = app_get_advance_5gprior(g_pstSnmpAgentData->pstAdvance5GPrior);
    if ( FALSE == bRet )
    {
        agent_printf(LOG_ERR,"%s: app_get_advance_5gprior failed.\n",SNMPAGENT_CONTROL_NAME);
        return FALSE;
    }

    return TRUE;
}
/*****************************************************************************
 Prototype    : ac_agent_get_ap11npriorsetup_config
 Description  : 获取AP 11n优先配置函数
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/3/11
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL ac_agent_get_ap11npriorsetup_config()
{
    BOOL bRet = FALSE;

	if (g_pstSnmpAgentData == NULL)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}

	if (g_pstSnmpAgentData->pstAdvance11nPrior == NULL)
	{
		g_pstSnmpAgentData->pstAdvance11nPrior = SNMP_AGENT_MALLOC(sizeof(AP_CONFIG_ADVANCE_11NPRIOR_T));
		if (g_pstSnmpAgentData->pstAdvance11nPrior == NULL)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstAdvance11nPrior failed\n",
				SNMPAGENT_CONTROL_NAME);
				return FALSE;
		}
	}
	memset(g_pstSnmpAgentData->pstAdvance11nPrior, 0, sizeof(AP_CONFIG_ADVANCE_11NPRIOR_T));

    bRet = app_get_advance_11nprior(g_pstSnmpAgentData->pstAdvance11nPrior);
    if ( FALSE == bRet )
    {
        agent_printf(LOG_ERR,"%s: app_get_advance_11nprior failed.\n",SNMPAGENT_CONTROL_NAME);
        return FALSE;
    }

    return TRUE;
}
#endif

#ifndef GW_X86


/*****************************************************************************
 Prototype    : ac_agent_get_apwifidevicebasic_config
 Description  : 获取2.4G卡基本配置信息
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/3/21
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL ac_agent_get_apwifidevicebasic_config()
{
    BOOL bRet = FALSE;

	if (NULL == g_pstSnmpAgentData)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}
    
	if (NULL == g_pstSnmpAgentData->pstApWifiBasicConfig)
	{
		g_pstSnmpAgentData->pstApWifiBasicConfig = SNMP_AGENT_MALLOC(sizeof(AP_CONFIG_WIFIDEVICEBASIC_T));
		if (NULL == g_pstSnmpAgentData->pstApWifiBasicConfig)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstApWifiBasicConfig failed\n",
				SNMPAGENT_CONTROL_NAME);
			return FALSE;
		}
	}
	memset(g_pstSnmpAgentData->pstApWifiBasicConfig, 0, sizeof(AP_CONFIG_WIFIDEVICEBASIC_T));

    g_pstSnmpAgentData->pstApWifiBasicConfig->ulWifiId = 0;
    bRet = wlan_get_wifibasicinfo(g_pstSnmpAgentData->pstApWifiBasicConfig);
    if (FALSE == bRet)
    {
        agent_printf(LOG_ERR, "get pstApWifiBasicConfig fail");
        return FALSE;
    }

    return TRUE;
}
/*****************************************************************************
 Prototype    : ac_agent_get_apwifidevicebasic5_config
 Description  : 获取5G卡基本配置信息
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/3/21
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL ac_agent_get_apwifidevicebasic5_config()
{
    BOOL bRet = FALSE;

	if (NULL == g_pstSnmpAgentData)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}
    
	if (NULL == g_pstSnmpAgentData->pstApWifiBasic5Config)
	{
		g_pstSnmpAgentData->pstApWifiBasic5Config = SNMP_AGENT_MALLOC(sizeof(AP_CONFIG_WIFIDEVICEBASIC_T));
		if (NULL == g_pstSnmpAgentData->pstApWifiBasic5Config)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstApWifiBasic5Config failed\n",
				SNMPAGENT_CONTROL_NAME);
			return FALSE;
		}
	}
	memset(g_pstSnmpAgentData->pstApWifiBasic5Config, 0, sizeof(AP_CONFIG_WIFIDEVICEBASIC_T));

    g_pstSnmpAgentData->pstApWifiBasic5Config->ulWifiId = 1;
    bRet = wlan_get_wifibasicinfo(g_pstSnmpAgentData->pstApWifiBasic5Config);
    if (FALSE == bRet)
    {
        agent_printf(LOG_ERR, "get pstApWifiBasic5Config fail");
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************
 Prototype    : ac_agent_get_apwifidevicechannel_config
 Description  : 获取2.4G卡信道配置信息
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/3/21
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL ac_agent_get_apwifidevicechannel_config()
{
    BOOL bRet = FALSE;

	if (NULL == g_pstSnmpAgentData)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}

	if (NULL == g_pstSnmpAgentData->pstApWifiChannelConfig)
	{
		g_pstSnmpAgentData->pstApWifiChannelConfig = SNMP_AGENT_MALLOC(sizeof(AP_CONFIG_WIFIDEVICECHANNEL_T));
		if (NULL == g_pstSnmpAgentData->pstApWifiChannelConfig)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstApWifiChannelConfig failed\n",
				SNMPAGENT_CONTROL_NAME);
			return FALSE;
		}
	}
	memset(g_pstSnmpAgentData->pstApWifiChannelConfig, 0, sizeof(AP_CONFIG_WIFIDEVICECHANNEL_T));
    
    g_pstSnmpAgentData->pstApWifiChannelConfig->ulWifiId = 0;
    bRet = wlan_get_wifichannelinfo(g_pstSnmpAgentData->pstApWifiChannelConfig);
    if (FALSE == bRet)
    {
        agent_printf(LOG_ERR, "get pstApWifiChannelConfig fail");
        return FALSE;
    }

    return TRUE;
}
/*****************************************************************************
 Prototype    : ac_agent_get_apwifidevicechannel5_config
 Description  : 获取5G卡信道配置信息
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/3/21
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL ac_agent_get_apwifidevicechannel5_config()
{
    BOOL bRet = FALSE;

	if (NULL == g_pstSnmpAgentData)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}

	if (NULL == g_pstSnmpAgentData->pstApWifiChannel5Config)
	{
		g_pstSnmpAgentData->pstApWifiChannel5Config = SNMP_AGENT_MALLOC(sizeof(AP_CONFIG_WIFIDEVICECHANNEL_T));
		if (NULL == g_pstSnmpAgentData->pstApWifiChannel5Config)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstApWifiChannel5Config failed\n",
				SNMPAGENT_CONTROL_NAME);
			return FALSE;
		}
	}
	memset(g_pstSnmpAgentData->pstApWifiChannel5Config, 0, sizeof(AP_CONFIG_WIFIDEVICECHANNEL_T));
    
    g_pstSnmpAgentData->pstApWifiChannel5Config->ulWifiId = 1;
    bRet = wlan_get_wifichannelinfo(g_pstSnmpAgentData->pstApWifiChannel5Config);
    if (FALSE == bRet)
    {
        agent_printf(LOG_ERR, "get pstApWifiChannel5Config fail");
        return FALSE;
    }

    return TRUE;
}
/*****************************************************************************
 Prototype    : ac_agent_get_apwifidevicepower_config
 Description  : 获取2.4G卡无线功率配置函数
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/2/26
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL ac_agent_get_apwifidevicepower_config()
{
    BOOL bRet = FALSE;

	if (NULL == g_pstSnmpAgentData)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}
    
	if (NULL == g_pstSnmpAgentData->pstApWifiPowerConfig)
	{
		g_pstSnmpAgentData->pstApWifiPowerConfig = SNMP_AGENT_MALLOC(sizeof(AP_CONFIG_WIFIDEVICEPOWER_T));
		if (NULL == g_pstSnmpAgentData->pstApWifiPowerConfig)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstApWifiPowerConfig failed\n",
				SNMPAGENT_CONTROL_NAME);
			return FALSE;
		}
	}
	memset(g_pstSnmpAgentData->pstApWifiPowerConfig, 0, sizeof(AP_CONFIG_WIFIDEVICEPOWER_T));

    g_pstSnmpAgentData->pstApWifiPowerConfig->ulWifiId = 0;
    bRet = wlan_get_wifitxpowerinfo(g_pstSnmpAgentData->pstApWifiPowerConfig);
    if (FALSE == bRet)
    {
        agent_printf(LOG_ERR, "get pstApWifiPowerConfig fail");
        return FALSE;
    }	

    return TRUE;
}
/*****************************************************************************
 Prototype    : ac_agent_get_apwifidevicepower5_config
 Description  : 获取5G卡无线功率配置函数
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/2/26
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL ac_agent_get_apwifidevicepower5_config()
{
    BOOL bRet = FALSE;

	if (NULL == g_pstSnmpAgentData)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}
    
	if (NULL == g_pstSnmpAgentData->pstApWifiPower5Config)
	{
		g_pstSnmpAgentData->pstApWifiPower5Config = SNMP_AGENT_MALLOC(sizeof(AP_CONFIG_WIFIDEVICEPOWER_T));
		if (NULL == g_pstSnmpAgentData->pstApWifiPower5Config)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstApWifiPower5Config failed\n",
				SNMPAGENT_CONTROL_NAME);
			return FALSE;
		}
	}
	memset(g_pstSnmpAgentData->pstApWifiPower5Config, 0, sizeof(AP_CONFIG_WIFIDEVICEPOWER_T));

    g_pstSnmpAgentData->pstApWifiPower5Config->ulWifiId = 1;
    bRet = wlan_get_wifitxpowerinfo(g_pstSnmpAgentData->pstApWifiPower5Config);
    if (FALSE == bRet)
    {
        agent_printf(LOG_ERR, "get pstApWifiPower5Config fail");
        return FALSE;
    }	

    return TRUE;
}
/*****************************************************************************
 Prototype    : ac_agent_get_apwifideviceadvance_config
 Description  : 获取2.4G卡无线高级配置
 Input        : None
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/21
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL ac_agent_get_apwifideviceadvance_config()
{
    BOOL bRet = FALSE;

	if (NULL == g_pstSnmpAgentData)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}

	if (NULL == g_pstSnmpAgentData->pstApWifiAdvanceConfig)
	{
		g_pstSnmpAgentData->pstApWifiAdvanceConfig = SNMP_AGENT_MALLOC(sizeof(AP_CONFIG_WIFIDEVICEADVANCE_T));
		if (NULL == g_pstSnmpAgentData->pstApWifiAdvanceConfig)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstApWifiAdvanceConfig failed\n",
				SNMPAGENT_CONTROL_NAME);
			return FALSE;
		}
	}
	memset(g_pstSnmpAgentData->pstApWifiAdvanceConfig, 0, sizeof(AP_CONFIG_WIFIDEVICEADVANCE_T));

    g_pstSnmpAgentData->pstApWifiAdvanceConfig->ulWifiId = 0;
    bRet = wlan_get_wifiadvanceinfo(g_pstSnmpAgentData->pstApWifiAdvanceConfig);
    if (FALSE == bRet)
    {
        agent_printf(LOG_ERR, "get pstApWifiAdvanceConfig fail");
        return FALSE;
    }	

    return TRUE;
}
/*****************************************************************************
 Prototype    : ac_agent_get_apwifideviceadvance5_config
 Description  : 获取5G卡无线高级配置
 Input        : None
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/21
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL ac_agent_get_apwifideviceadvance5_config()
{
    BOOL bRet = FALSE;

	if (NULL == g_pstSnmpAgentData)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}

	if (NULL == g_pstSnmpAgentData->pstApWifiAdvance5Config)
	{
		g_pstSnmpAgentData->pstApWifiAdvance5Config = SNMP_AGENT_MALLOC(sizeof(AP_CONFIG_WIFIDEVICEADVANCE_T));
		if (NULL == g_pstSnmpAgentData->pstApWifiAdvance5Config)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstApWifiAdvance5Config failed\n",
				SNMPAGENT_CONTROL_NAME);
			return FALSE;
		}
	}
	memset(g_pstSnmpAgentData->pstApWifiAdvance5Config, 0, sizeof(AP_CONFIG_WIFIDEVICEADVANCE_T));

    g_pstSnmpAgentData->pstApWifiAdvance5Config->ulWifiId = 1;
    bRet = wlan_get_wifiadvanceinfo(g_pstSnmpAgentData->pstApWifiAdvance5Config);
    if (FALSE == bRet)
    {
        agent_printf(LOG_ERR, "get pstApWifiAdvance5Config fail");
        return FALSE;
    }	

    return TRUE;
}
/*****************************************************************************
 Prototype    : ac_agent_get_apwifidevicerate_config
 Description  : 获取2.4G卡速率配置
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/8/29
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
BOOL ac_agent_get_apwifidevicerate_config()
{
    BOOL bRet = FALSE;

	if (NULL == g_pstSnmpAgentData)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}

	if (NULL == g_pstSnmpAgentData->pstApWifiRateConfig)
	{
		g_pstSnmpAgentData->pstApWifiRateConfig = SNMP_AGENT_MALLOC(sizeof(AP_CONFIG_WIFIDEVICERATE_T));
		if (NULL == g_pstSnmpAgentData->pstApWifiRateConfig)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstApWifiRateConfig failed\n",
				SNMPAGENT_CONTROL_NAME);
			return FALSE;
		}
	}
	memset(g_pstSnmpAgentData->pstApWifiRateConfig, 0, sizeof(AP_CONFIG_WIFIDEVICERATE_T));

    g_pstSnmpAgentData->pstApWifiRateConfig->ulWifiId = 0;
    bRet = wlan_get_wifirateinfo(g_pstSnmpAgentData->pstApWifiRateConfig);
    if (FALSE == bRet)
    {
        agent_printf(LOG_ERR, "get pstApWifiRateConfig fail");
        return FALSE;
    }	

    return TRUE;
}
/*****************************************************************************
 Prototype    : ac_agent_get_apwifidevicerate5_config
 Description  : 获取5G卡速率配置
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/8/29
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
BOOL ac_agent_get_apwifidevicerate5_config()
{
    BOOL bRet = FALSE;

	if (NULL == g_pstSnmpAgentData)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}

	if (NULL == g_pstSnmpAgentData->pstApWifiRate5Config)
	{
		g_pstSnmpAgentData->pstApWifiRate5Config = SNMP_AGENT_MALLOC(sizeof(AP_CONFIG_WIFIDEVICERATE_T));
		if (NULL == g_pstSnmpAgentData->pstApWifiRate5Config)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstApWifiRate5Config failed\n",
				SNMPAGENT_CONTROL_NAME);
			return FALSE;
		}
	}
	memset(g_pstSnmpAgentData->pstApWifiRate5Config, 0, sizeof(AP_CONFIG_WIFIDEVICERATE_T));

    g_pstSnmpAgentData->pstApWifiRate5Config->ulWifiId = 1;
    bRet = wlan_get_wifirateinfo(g_pstSnmpAgentData->pstApWifiRate5Config);
    if (FALSE == bRet)
    {
        agent_printf(LOG_ERR, "get pstApWifiRate5Config fail");
        return FALSE;
    }	

    return TRUE;
}
BOOL ac_agent_get_apvapbasicsetup_config()
{
    BOOL bRet = FALSE;
    int iVapLoop  = 0;
    SSID_T stSsidNum;

	if (NULL == g_pstSnmpAgentData)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}
    
	if (NULL == g_pstSnmpAgentData->pstApVapBasicConfig)
	{
		g_pstSnmpAgentData->pstApVapBasicConfig = SNMP_AGENT_MALLOC(
            sizeof(AP_CONFIG_VAPBASIC_ENTRY_T)*VAP_MAX_NUM);
		if (NULL == g_pstSnmpAgentData->pstApVapBasicConfig)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstApVapBasicConfig failed\n",
				SNMPAGENT_CONTROL_NAME);
			return FALSE;
		}
	}
	memset(g_pstSnmpAgentData->pstApVapBasicConfig, 0, 
		sizeof(AP_CONFIG_VAPBASIC_ENTRY_T)*VAP_MAX_NUM);
	g_pstSnmpAgentData->stApVapConfig.ulVapNum = 0;

	#ifdef OPENWRT
	unsigned int  uiRadioId = 0, uiWlanId = 0,uiIndex=0;
	AP_WIFI_T stWifiInfo,*pstWifiInfo;
	AP_CONFIG_VAPBASIC_ENTRY_T *pstApVapBasicConfig;

	pstWifiInfo = &stWifiInfo;
	memset(pstWifiInfo,0,sizeof(AP_WIFI_T));
	/*获取当前vap列表*/
	pal_wifi_get_vap_info(pstWifiInfo);
	g_pstSnmpAgentData->stApVapConfig.ulVapNum=pstWifiInfo->astRadio[0].uiVapNum + pstWifiInfo->astRadio[1].uiVapNum;

	for(uiRadioId=0;uiRadioId < pstWifiInfo->uiRadioNum;uiRadioId++)
	{
		for(uiWlanId = 0; uiWlanId < pstWifiInfo->astRadio[uiRadioId].uiVapNum; uiWlanId++)
		{
			uiIndex = pstWifiInfo->astRadio[uiRadioId].astVap[uiWlanId].uiIndex;
			pstApVapBasicConfig = &g_pstSnmpAgentData->pstApVapBasicConfig[uiIndex];

			pstApVapBasicConfig->ulRowStatus = ENUM_ROWSTATUS_ACTIVE;
			pstApVapBasicConfig->ulIndex = uiIndex+1;
			pstApVapBasicConfig->ulVapId = uiWlanId;
			pstApVapBasicConfig->ulWifiId = uiRadioId;
			pstApVapBasicConfig->ulSsidEnabled = !pstWifiInfo->astRadio[uiRadioId].astVap[uiWlanId].uiSsidEnable;
			pstApVapBasicConfig->ulSsidBroadcast = pstWifiInfo->astRadio[uiRadioId].astVap[uiWlanId].uiSsidHide;

			memcpy(pstApVapBasicConfig->acSsid, pstWifiInfo->astRadio[uiRadioId].astVap[uiWlanId].acSsid, strlen(pstWifiInfo->astRadio[uiRadioId].astVap[uiWlanId].acSsid));
			memcpy(pstApVapBasicConfig->aucBssid, pstWifiInfo->astRadio[uiRadioId].astVap[uiWlanId].aucBssid,MAC_LEN);

			agent_printf(LOG_INFO, "uiIndex: %d pstApVapBasicConfig->uiIndex:%d uiWlanId:%d uiRadioId:%d acSsid:%s  pstApVapBasicConfig->aucBssid "MAC_FMT"  ",\
                        uiIndex,pstApVapBasicConfig->ulIndex,uiWlanId,uiRadioId,\
                        pstApVapBasicConfig->acSsid,MAC_ARG(pstApVapBasicConfig->aucBssid));
        }
	}

    
    #else
   
	memset(&stSsidNum, 0, sizeof(SSID_T));
	bRet = get_wifi_vap_number(&stSsidNum);
	if (FALSE == bRet)
	{
        agent_printf(LOG_ERROR, "get_wifi_vap_number failed");
        return FALSE;
	}

    g_pstSnmpAgentData->stApVapConfig.ulVapNum = stSsidNum.ulSsidNum;
	for (iVapLoop = 0; iVapLoop < stSsidNum.ulSsidNum; iVapLoop++)
	{
        g_pstSnmpAgentData->pstApVapBasicConfig[iVapLoop].ulIndex = stSsidNum.astSsidInfo[iVapLoop].ulNvramId+1;
        g_pstSnmpAgentData->pstApVapBasicConfig[iVapLoop].ulWifiId = stSsidNum.astSsidInfo[iVapLoop].ulWifiId;
        g_pstSnmpAgentData->pstApVapBasicConfig[iVapLoop].ulVapId = stSsidNum.astSsidInfo[iVapLoop].ulVapId;
        g_pstSnmpAgentData->pstApVapBasicConfig[iVapLoop].ulNvramId = stSsidNum.astSsidInfo[iVapLoop].ulNvramId;
        g_pstSnmpAgentData->pstApVapBasicConfig[iVapLoop].ulRowStatus = ENUM_ROWSTATUS_ACTIVE;

        bRet = wlan_get_vapinfo(&(g_pstSnmpAgentData->pstApVapBasicConfig[iVapLoop]));
        if (FALSE == bRet)
        {
            agent_printf(LOG_ERROR, "get ap%d_%d pstApVapBasicConfig fail",
                g_pstSnmpAgentData->pstApVapBasicConfig[iVapLoop].ulWifiId,
                g_pstSnmpAgentData->pstApVapBasicConfig[iVapLoop].ulVapId);
            return FALSE;
        }
	}
    #endif

    return TRUE;
}
BOOL ac_agent_get_apvapsecuritysetup_config()
{
    BOOL bRet = FALSE;
    int iVapLoop  = 0;
    SSID_T stSsidNum;

	if (NULL == g_pstSnmpAgentData)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}
    
	if (NULL == g_pstSnmpAgentData->pstApVapSecurityConfig)
	{
		g_pstSnmpAgentData->pstApVapSecurityConfig = SNMP_AGENT_MALLOC(
            sizeof(AP_CONFIG_VAPSECURITY_ENTRY_T)*VAP_MAX_NUM);
		if (NULL == g_pstSnmpAgentData->pstApVapSecurityConfig)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstApVapSecurityConfig failed\n",
				SNMPAGENT_CONTROL_NAME);
			return FALSE;
		}
	}
	memset(g_pstSnmpAgentData->pstApVapSecurityConfig, 0, 
		sizeof(AP_CONFIG_VAPSECURITY_ENTRY_T)*VAP_MAX_NUM);
	#ifdef OPENWRT

	AP_CONFIG_VAPSECURITY_ENTRY_T *pstApVapSecurityConfig;

	AP_VAP_ENCRYPT_T *pstEncrypt;

	unsigned int  uiRadioId = 0, uiWlanId = 0,uiIndex=0;
	AP_WIFI_T stWifiInfo,*pstWifiInfo;
	AP_CONFIG_VAPBASIC_ENTRY_T *pstApVapBasicConfig;

	pstWifiInfo = &stWifiInfo;
	memset(pstWifiInfo,0,sizeof(AP_WIFI_T));
	/*获取当前vap信息*/
	pal_wifi_get_vap_info(pstWifiInfo);
	g_pstSnmpAgentData->stApVapConfig.ulVapNum=pstWifiInfo->astRadio[0].uiVapNum + pstWifiInfo->astRadio[1].uiVapNum;

	for(uiRadioId=0;uiRadioId < pstWifiInfo->uiRadioNum;uiRadioId++)
	{
		for(uiWlanId = 0; uiWlanId < pstWifiInfo->astRadio[uiRadioId].uiVapNum; uiWlanId++)
		{
			uiIndex = pstWifiInfo->astRadio[uiRadioId].astVap[uiWlanId].uiIndex;
			pstApVapSecurityConfig = &g_pstSnmpAgentData->pstApVapSecurityConfig[uiIndex];

			pstEncrypt = &pstWifiInfo->astRadio[uiRadioId].astVap[uiWlanId].stVapEncrypt;

			pstApVapSecurityConfig->ulRowStatus = ENUM_ROWSTATUS_ACTIVE;
			pstApVapSecurityConfig->ulIndex = uiIndex+1;
			pstApVapSecurityConfig->ulVapId = uiWlanId;
			pstApVapSecurityConfig->ulWifiId = uiRadioId;

			memcpy(pstApVapSecurityConfig->acSsid, pstWifiInfo->astRadio[uiRadioId].astVap[uiWlanId].acSsid,strlen(pstWifiInfo->astRadio[uiRadioId].astVap[uiWlanId].acSsid));
			
			pstApVapSecurityConfig->ulAuthenticationType = pstEncrypt->ulAuthenticationType;
			pstApVapSecurityConfig->ulSecurityMode = pstEncrypt->ulSecurityMode;
			pstApVapSecurityConfig->ulEncryptType = pstEncrypt->ulEncryptType;
			pstApVapSecurityConfig->ulWepKeyFormat = pstEncrypt->ulWepKeyFormat;
			pstApVapSecurityConfig->ulWpaKeyRenewal = pstEncrypt->ulWpaKeyRenewal;
			
			memcpy(pstApVapSecurityConfig->acWepKey, pstEncrypt->acWepKey,strlen(pstEncrypt->acWepKey));
			memcpy(pstApVapSecurityConfig->acWpaKey, pstEncrypt->acWpaKey,strlen(pstEncrypt->acWpaKey));
			
		}

	}

	
	#else
	memset(&stSsidNum, 0, sizeof(SSID_T));
	bRet = get_wifi_vap_number(&stSsidNum);
	if (FALSE == bRet)
	{
        agent_printf(LOG_ERROR, "get_wifi_vap_number failed");
        return FALSE;
	}

	for (iVapLoop = 0; iVapLoop < stSsidNum.ulSsidNum; iVapLoop++)
	{
        g_pstSnmpAgentData->pstApVapSecurityConfig[iVapLoop].ulIndex = stSsidNum.astSsidInfo[iVapLoop].ulNvramId+1;
        g_pstSnmpAgentData->pstApVapSecurityConfig[iVapLoop].ulWifiId = stSsidNum.astSsidInfo[iVapLoop].ulWifiId;
        g_pstSnmpAgentData->pstApVapSecurityConfig[iVapLoop].ulVapId = stSsidNum.astSsidInfo[iVapLoop].ulVapId;
        g_pstSnmpAgentData->pstApVapSecurityConfig[iVapLoop].ulRowStatus = ENUM_ROWSTATUS_ACTIVE;
        bRet = wlan_get_vapsecurityinfo(&(g_pstSnmpAgentData->pstApVapSecurityConfig[iVapLoop]));
        if (FALSE == bRet)
        {
            agent_printf(LOG_ERROR, "get ap%d_%d pstApVapSecurityConfig fail",
                g_pstSnmpAgentData->pstApVapSecurityConfig[iVapLoop].ulWifiId,
                g_pstSnmpAgentData->pstApVapSecurityConfig[iVapLoop].ulVapId);
            return FALSE;
        }
	}
	#endif

    return TRUE;
}
#endif

BOOL ac_agent_get_apalarmsetup_config()
{
    BOOL bRet = FALSE;

	if (NULL == g_pstSnmpAgentData)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}
    
	if (NULL == g_pstSnmpAgentData->pstApAlarmConfig)
	{
		g_pstSnmpAgentData->pstApAlarmConfig = SNMP_AGENT_MALLOC(sizeof(AP_CONFIG_ALARM_T));
		if (NULL == g_pstSnmpAgentData->pstApAlarmConfig)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstApAlarmConfig failed\n",
				SNMPAGENT_CONTROL_NAME);
			return FALSE;
		}
	}
	memset(g_pstSnmpAgentData->pstApAlarmConfig, 0, sizeof(AP_CONFIG_ALARM_T));

    bRet = app_get_alarmconfig(g_pstSnmpAgentData->pstApAlarmConfig);
    if ( FALSE == bRet )
    {
        lprintf(LOG_ERR, "app_get_alarmconfig failed.");
        return FALSE;
    }

    return TRUE;
}
BOOL ac_agent_get_aptrapdessetup_config()
{
    BOOL bRet = FALSE;

	if (NULL == g_pstSnmpAgentData)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}
    
	if (NULL == g_pstSnmpAgentData->pstApTrapDesConfig)
	{
		g_pstSnmpAgentData->pstApTrapDesConfig = SNMP_AGENT_MALLOC(sizeof(AP_CONFIG_TRAPDES_ENTRY_T));
		if (NULL == g_pstSnmpAgentData->pstApTrapDesConfig)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstApTrapDesConfig failed\n",
				SNMPAGENT_CONTROL_NAME);
			return FALSE;
		}
	}
	memset(g_pstSnmpAgentData->pstApTrapDesConfig, 0, sizeof(AP_CONFIG_TRAPDES_ENTRY_T));
    bRet = app_get_trapdesinfo(g_pstSnmpAgentData->pstApTrapDesConfig);
    if (FALSE == bRet)
    {
        agent_printf(LOG_ERROR, "get pstApTrapDesConfig fail");
        return FALSE;
    }
    return TRUE;
}
BOOL ac_agent_get_apsysstat()
{
    BOOL bRet = FALSE;
    AP_STAT_SYS_T stSysStat = {0};
    UINT32  ulOldUp = 0;
    UINT32  ulOldDw = 0;
    UINT32 ulCpuUsage = 0;
    UINT32 ulMemUsage = 0;

	if (NULL == g_pstSnmpAgentData)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}
    
    #if 0
    bRet = app_get_sysinfo((AP_STAT_SYS_T *)(&stSysStat));
    if ( FALSE == bRet )
    {
        agent_printf(LOG_ERR,"%s: app_get_sysinfo failed.\n",SNMPAGENT_CONTROL_NAME);
        //return FALSE;
    }
    #endif
    device_cpu_get(&ulCpuUsage);
    if(0 == ulCpuUsage)
        ulCpuUsage = 1;
    
    device_mem_get(&ulMemUsage);
    
    g_pstSnmpAgentData->stSysStat.ulCPURTUsage = ulCpuUsage;
    g_pstSnmpAgentData->stSysStat.ulMemRTUsage = ulMemUsage;
    g_pstSnmpAgentData->stSysStat.ulCPUPeakUsage = ulCpuUsage;
    g_pstSnmpAgentData->stSysStat.ulCPUAvgUsage = ulCpuUsage;
    g_pstSnmpAgentData->stSysStat.ulMemPeakUsage = ulMemUsage;
    g_pstSnmpAgentData->stSysStat.ulMemAvgUsage = ulMemUsage;

    ulOldUp = g_pstSnmpAgentData->stSysStat.ulTotalUp;
    ulOldDw = g_pstSnmpAgentData->stSysStat.ulTotalDw;
    get_interface_stats(&g_pstSnmpAgentData->stSysStat.ulTotalDw, 
        &g_pstSnmpAgentData->stSysStat.ulTotalUp);
    if ( g_pstSnmpAgentData->stSysStat.ulTotalUp >= ulOldUp )
    {
        g_pstSnmpAgentData->stSysStat.ulUpSpeed = 
            (g_pstSnmpAgentData->stSysStat.ulTotalUp - ulOldUp)*8/(PERIOD_DYNAMICINFO*1024);
    }
    if ( g_pstSnmpAgentData->stSysStat.ulTotalDw >= ulOldDw)
    {
        g_pstSnmpAgentData->stSysStat.ulDwSpeed = 
            (g_pstSnmpAgentData->stSysStat.ulTotalDw - ulOldDw)*8/(PERIOD_DYNAMICINFO*1024);
    }
    g_pstSnmpAgentData->stSysStat.ulTotalWideBand = 100;//Mbps
    g_pstSnmpAgentData->stSysStat.ulWidebandPerused =  
        (g_pstSnmpAgentData->stSysStat.ulUpSpeed+g_pstSnmpAgentData->stSysStat.ulDwSpeed)/(100*1024);
    g_pstSnmpAgentData->stSysStat.ulApNum = g_pstSnmpAgentData->ulApNum;
    g_pstSnmpAgentData->stSysStat.ulApOnlineNum = g_pstSnmpAgentData->ulApNum;
    g_pstSnmpAgentData->stSysStat.ulApOfflineNum = 0;
    g_pstSnmpAgentData->stSysStat.ulOnlineUserNum = g_pstSnmpAgentData->ulUserNum;
    g_pstSnmpAgentData->stSysStat.ulFeelUserNum = g_pstSnmpAgentData->ulUserNum;
    
    return TRUE;
}
#ifndef GW_X86


/* BEGIN: Added by zoucaihong, 2012/11/21 
获取VLAN配置*/
BOOL ac_agent_get_apvlansetup_config()
{
    UINT32 ulTableMaxRecNum = TABLE_MAXRECNUM_VLAN;

	if (NULL == g_pstSnmpAgentData)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}
    
	if (NULL == g_pstSnmpAgentData->pstApVlanConfig)
	{
		g_pstSnmpAgentData->pstApVlanConfig = SNMP_AGENT_MALLOC(
            sizeof(AP_CONFIG_VLAN_ENTRY_T)*ulTableMaxRecNum);
		if (NULL == g_pstSnmpAgentData->pstApVlanConfig)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstApVlanConfig failed\n",
				SNMPAGENT_CONTROL_NAME);
			return FALSE;
		}
		agent_printf(LOG_ERR, "malloc g_pstSnmpAgentData->pstApVlanConfig success.\n");
	}
	memset(g_pstSnmpAgentData->pstApVlanConfig, 0, 
		sizeof(AP_CONFIG_VLAN_ENTRY_T)*ulTableMaxRecNum);
    g_pstSnmpAgentData->stApVlanConfig.ulVlanNum = 0;

    return app_get_vlaninfo(&g_pstSnmpAgentData->stApVlanConfig.ulVlanNum, g_pstSnmpAgentData->pstApVlanConfig);
}
/* END: Added by zoucaihong, 2012/11/21 */

BOOL ac_agent_get_apssid_setup()
{
    BOOL bRet = FALSE;
    int iVapLoop  = 0;
    SSID_T stSsidNum;
    CHAR acNvramName[NVRAM_SIZE] = {0};

	if (NULL == g_pstSnmpAgentData)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}
    
	if (NULL == g_pstSnmpAgentData->pstApSsidConfig)
	{
		g_pstSnmpAgentData->pstApSsidConfig = SNMP_AGENT_MALLOC(
            sizeof(AP_CONFIG_SSID_ENTRY_T)*VAP_MAX_NUM);
		if (NULL == g_pstSnmpAgentData->pstApSsidConfig)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstApSsidConfig failed\n",
				SNMPAGENT_CONTROL_NAME);
			return FALSE;
		}
	}
	memset(g_pstSnmpAgentData->pstApSsidConfig, 0, 
		sizeof(AP_CONFIG_SSID_ENTRY_T)*VAP_MAX_NUM);
    g_pstSnmpAgentData->stApSsidConfig.ulNum = 0;

	memset(&stSsidNum, 0, sizeof(SSID_T));
	bRet = get_wifi_vap_number(&stSsidNum);
	if (FALSE == bRet)
	{
        agent_printf(LOG_ERROR, "get_wifi_vap_number failed");
        return FALSE;
	}

	for (iVapLoop = 0; iVapLoop < stSsidNum.ulSsidNum; iVapLoop++)
	{
        g_pstSnmpAgentData->pstApSsidConfig[iVapLoop].ulIndex = stSsidNum.astSsidInfo[iVapLoop].ulNvramId+1;
        g_pstSnmpAgentData->pstApSsidConfig[iVapLoop].ulWifiId = stSsidNum.astSsidInfo[iVapLoop].ulWifiId;
        g_pstSnmpAgentData->pstApSsidConfig[iVapLoop].ulVapId = stSsidNum.astSsidInfo[iVapLoop].ulVapId;
        g_pstSnmpAgentData->pstApSsidConfig[iVapLoop].ulRowStatus = ENUM_ROWSTATUS_ACTIVE;
        sprintf(acNvramName, NVRAM_VAP"%d_ssid_name", stSsidNum.astSsidInfo[iVapLoop].ulNvramId);
        nvram_get(g_pstSnmpAgentData->pstApSsidConfig[iVapLoop].acSsidName,
            acNvramName, NVRAM_SIZE);
	}
    g_pstSnmpAgentData->stApSsidConfig.ulNum = stSsidNum.ulSsidNum;

    return TRUE;
}
#ifdef OPENWRT

BOOL ac_agent_get_apwifistat()
{
}
#else
BOOL ac_agent_copy_airstats(WIFI_API_BSS_STATS_T stAirStats, UINT32 ulLoopNo, UINT32 ulWifiId, UINT32 ulVapId)
{
    /*VAP级统计*/
    g_pstSnmpAgentData->pstApWifiVapStat[ulLoopNo].ulIndex = ulLoopNo+1;
    g_pstSnmpAgentData->pstApWifiVapStat[ulLoopNo].ulVapId = ulVapId;
    g_pstSnmpAgentData->pstApWifiVapStat[ulLoopNo].ulWifiId = ulWifiId;
    strcpy(g_pstSnmpAgentData->pstApWifiVapStat[ulLoopNo].acVapSsid,(char *)stAirStats.unBssStats.stBssAirStats.aucSsid);
    /*
    ap_agent_convert_u32_to_u64(stAirStats.ulRecvBytesHigh,
        stAirStats.ulRecvBytesLow,
        (UINT8 *) (&g_pstSnmpAgentData->pstApWifiVapStat[ulLoopNo].ullRecvBytes));
    ap_agent_convert_u32_to_u64(stAirStats.ulSendBytesHigh,
        stAirStats.ulSendBytesLow,
        (UINT8 *) (&g_pstSnmpAgentData->pstApWifiVapStat[ulLoopNo].ullSendBytes));
    */
    g_pstSnmpAgentData->pstApWifiVapStat[ulLoopNo].ullRecvBytes = stAirStats.unBssStats.stBssAirStats.ullRxDataBytes;
    g_pstSnmpAgentData->pstApWifiVapStat[ulLoopNo].ullSendBytes = stAirStats.unBssStats.stBssAirStats.ullTxDataBytes;
    g_pstSnmpAgentData->pstApWifiVapStat[ulLoopNo].ulRecvFrameNum = stAirStats.unBssStats.stBssAirStats.ulRxDataFrame;
    g_pstSnmpAgentData->pstApWifiVapStat[ulLoopNo].ulSendFrameNum = stAirStats.unBssStats.stBssAirStats.ulTxDataFrame;
    g_pstSnmpAgentData->pstApWifiVapStat[ulLoopNo].ulRecvErrFrame = stAirStats.unBssStats.stBssAirStats.ulRxErrorTotal;
    g_pstSnmpAgentData->pstApWifiVapStat[ulLoopNo].ulSendErrFrame = stAirStats.unBssStats.stBssAirStats.ulTxErrorTotal;
    if ( 0 != (stAirStats.unBssStats.stBssAirStats.ulRxDataFrame + stAirStats.unBssStats.stBssAirStats.ulTxDataFrame) )
    {
        g_pstSnmpAgentData->pstApWifiVapStat[ulLoopNo].ulErrFrameRate = 
            (stAirStats.unBssStats.stBssAirStats.ulRxErrorTotal + stAirStats.unBssStats.stBssAirStats.ulTxErrorTotal)*100/
            (stAirStats.unBssStats.stBssAirStats.ulRxDataFrame + stAirStats.unBssStats.stBssAirStats.ulTxDataFrame);
    }
    else
    {
        g_pstSnmpAgentData->pstApWifiVapStat[ulLoopNo].ulErrFrameRate = 0;
    }
    g_pstSnmpAgentData->pstApWifiVapStat[ulLoopNo].ulAssoStaNum = stAirStats.unBssStats.stBssAirStats.ucStationNum;

    /*WIFI级统计*/
    g_pstSnmpAgentData->pstApWifiStat[ulWifiId].ulIndex = ulWifiId+1;
    g_pstSnmpAgentData->pstApWifiStat[ulWifiId].ulWifiId = ulWifiId;
    g_pstSnmpAgentData->pstApWifiStat[ulWifiId].ullRecvBytes += g_pstSnmpAgentData->pstApWifiVapStat[ulLoopNo].ullRecvBytes;
    g_pstSnmpAgentData->pstApWifiStat[ulWifiId].ullSendBytes += g_pstSnmpAgentData->pstApWifiVapStat[ulLoopNo].ullSendBytes;
    g_pstSnmpAgentData->pstApWifiStat[ulWifiId].ulRecvFrameNum += g_pstSnmpAgentData->pstApWifiVapStat[ulLoopNo].ulRecvFrameNum;
    g_pstSnmpAgentData->pstApWifiStat[ulWifiId].ulSendFrameNum += g_pstSnmpAgentData->pstApWifiVapStat[ulLoopNo].ulSendFrameNum;
    g_pstSnmpAgentData->pstApWifiStat[ulWifiId].ulRecvErrFrame += g_pstSnmpAgentData->pstApWifiVapStat[ulLoopNo].ulRecvErrFrame;
    g_pstSnmpAgentData->pstApWifiStat[ulWifiId].ulSendErrFrame += g_pstSnmpAgentData->pstApWifiVapStat[ulLoopNo].ulSendErrFrame;
    if ( 0 != (g_pstSnmpAgentData->pstApWifiStat[ulWifiId].ulRecvFrameNum + 
            g_pstSnmpAgentData->pstApWifiStat[ulWifiId].ulSendFrameNum) )
    {
        g_pstSnmpAgentData->pstApWifiStat[ulWifiId].ulErrFrameRate =
            (g_pstSnmpAgentData->pstApWifiStat[ulWifiId].ulRecvErrFrame +
            g_pstSnmpAgentData->pstApWifiStat[ulWifiId].ulSendErrFrame)*100/
            (g_pstSnmpAgentData->pstApWifiStat[ulWifiId].ulRecvFrameNum + 
            g_pstSnmpAgentData->pstApWifiStat[ulWifiId].ulSendFrameNum);
    }
    else
    {
        g_pstSnmpAgentData->pstApWifiStat[ulWifiId].ulErrFrameRate = 0;
    }
    g_pstSnmpAgentData->pstApWifiStat[ulWifiId].ulAssoStaNum += stAirStats.unBssStats.stBssAirStats.ucStationNum;

    return TRUE;
}

BOOL ac_agent_get_apwifistat()
{
    BOOL bRet = FALSE;
    BOOL bWifiFlag1 = FALSE;
    BOOL bWifiFlag2 = FALSE;
    SSID_T stSsidInfo = {0};
    UINT32 ulWifiId = 0;
    UINT32 ulVapId = 0;
    UINT32 ulLoopNo = 0;
    CHAR vap_name[32];
    WIFI_API_BSS_STATS_T stAirStats = {0};

	if (NULL == g_pstSnmpAgentData)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}
    
	if (NULL == g_pstSnmpAgentData->pstApWifiStat)
	{
		g_pstSnmpAgentData->pstApWifiStat = SNMP_AGENT_MALLOC(
            sizeof(AP_STAT_WIFI_ENTRY_T)*WIFI_MAX_NUM);
		if (NULL == g_pstSnmpAgentData->pstApWifiStat)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstApWifiStat failed\n",
				SNMPAGENT_CONTROL_NAME);
			return FALSE;
		}
	}
	memset(g_pstSnmpAgentData->pstApWifiStat, 0, sizeof(AP_STAT_WIFI_ENTRY_T)*WIFI_MAX_NUM);
	memset(&g_pstSnmpAgentData->stApWifiStat.ulWifiNum, 0, sizeof(AP_STAT_WIFI_T));

	if (NULL == g_pstSnmpAgentData->pstApWifiVapStat)
	{
		g_pstSnmpAgentData->pstApWifiVapStat = SNMP_AGENT_MALLOC(
            sizeof(AP_STAT_WIFIVAP_ENTRY_T)*VAP_MAX_NUM);
		if (NULL == g_pstSnmpAgentData->pstApWifiVapStat)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstApWifiVapStat failed\n",
				SNMPAGENT_CONTROL_NAME);
			return FALSE;
		}
	}
	memset(g_pstSnmpAgentData->pstApWifiVapStat, 0, sizeof(AP_STAT_WIFIVAP_ENTRY_T)*VAP_MAX_NUM);
    g_pstSnmpAgentData->stApWifiVapStat.ulVapNum = 0;
    
    bRet = get_wifi_vap_number(&stSsidInfo);
    if (FALSE == bRet)
    {
        agent_printf(LOG_ERR, "stSsidInfo get vap fail");
        return FALSE;
    }
    g_pstSnmpAgentData->stApWifiVapStat.ulVapNum = stSsidInfo.ulSsidNum;
    
    for ( ulLoopNo = 0 ; ulLoopNo < stSsidInfo.ulSsidNum ; ulLoopNo++ )
    {
        ulWifiId = stSsidInfo.astSsidInfo[ulLoopNo].ulWifiId;
        ulVapId = stSsidInfo.astSsidInfo[ulLoopNo].ulVapId;
        if ( 0 == ulWifiId )
        {
            bWifiFlag1 = TRUE;
        }
        else if ( 1 == ulWifiId )
        {
            bWifiFlag2 = TRUE;
        }
        sprintf(vap_name,  "ap%d_%d", ulWifiId, ulVapId);

        stAirStats.enStatsType = BSS_AIR_STATS;
        if(0 != wifi_api_vap_get_bss_stats(vap_name, &stAirStats))
        {
            continue;
        }
        ac_agent_copy_airstats(stAirStats, ulLoopNo, ulWifiId, ulVapId);
    }
    if ( ( bWifiFlag1 && !bWifiFlag2 ) || ( !bWifiFlag1 && bWifiFlag2 ) )
    {
        g_pstSnmpAgentData->stApWifiStat.ulWifiNum = 1;
    }
    else if ( bWifiFlag1 && bWifiFlag2 )
    {
        g_pstSnmpAgentData->stApWifiStat.ulWifiNum = 2;
    }
    /* BEGIN: Added by zoucaihong, 2013/5/10 */
    /*AP整机上/下行讯务流量统计*/
    g_pstSnmpAgentData->stApWifiStat.ullApRecvBytes = g_pstSnmpAgentData->pstApWifiStat[0].ullRecvBytes
        + g_pstSnmpAgentData->pstApWifiStat[1].ullRecvBytes;
    g_pstSnmpAgentData->stApWifiStat.ullApSendBytes = g_pstSnmpAgentData->pstApWifiStat[0].ullSendBytes
        + g_pstSnmpAgentData->pstApWifiStat[1].ullSendBytes;
    /*AP 2.4G上/下行讯务流量统计*/
    g_pstSnmpAgentData->stApWifiStat.ullWifi24RecvBytes = g_pstSnmpAgentData->pstApWifiStat[0].ullRecvBytes;
    g_pstSnmpAgentData->stApWifiStat.ullWifi24SendBytes = g_pstSnmpAgentData->pstApWifiStat[0].ullSendBytes;
    /*AP 5G上/下行讯务流量统计*/
    g_pstSnmpAgentData->stApWifiStat.ullWifi5RecvBytes = g_pstSnmpAgentData->pstApWifiStat[1].ullRecvBytes;
    g_pstSnmpAgentData->stApWifiStat.ullWifi5SendBytes = g_pstSnmpAgentData->pstApWifiStat[1].ullSendBytes;
    /* END: Added by zoucaihong, 2013/5/10 */

    return TRUE;
}
#endif
/* BEGIN: Added by zoucaihong, 2012/11/22 */
BOOL ac_agent_get_apstationstat()
{
    BOOL bRet = FALSE;
    UINT32 ulUserNum = 0;
    UINT32 ulTableMaxRecNum = TABLE_MAXRECNUM_STATION;

	if (NULL == g_pstSnmpAgentData)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}
    
	if (NULL == g_pstSnmpAgentData->pstApStationStat)
	{
		g_pstSnmpAgentData->pstApStationStat = SNMP_AGENT_MALLOC(
            sizeof(AP_STAT_STATION_ENTRY_T)*ulTableMaxRecNum);
		if (NULL == g_pstSnmpAgentData->pstApStationStat)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstApStationStat failed\n",
				SNMPAGENT_CONTROL_NAME);
			return FALSE;
		}
	}
	memset(g_pstSnmpAgentData->pstApStationStat, 0, sizeof(AP_STAT_STATION_ENTRY_T)*ulTableMaxRecNum);
    g_pstSnmpAgentData->stApStationStat.ulStationNum = 0;

    bRet = wlan_get_userinfo(g_pstSnmpAgentData->pstApStationStat, &ulUserNum);
    if ( FALSE == bRet )
    {
        agent_printf(LOG_ERR, "wlan_get_userinfo failed.\n");
        return FALSE;
    }

    g_pstSnmpAgentData->stApStationStat.ulStationNum = ulUserNum;

    return TRUE;
}

#endif
/* END: Added by zoucaihong, 2012/11/22 */
/* BEGIN: Added by zoucaihong, 2015/3/25 */
BOOL ac_agent_get_onlineuser()
{
    BOOL bRet = FALSE;
    UINT32 ulTableMaxRecNum = TABLE_MAXRECNUM_USER;

	if (NULL == g_pstSnmpAgentData)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}
    
	if (NULL == g_pstSnmpAgentData->pstOnlineUserInfo)
	{
		g_pstSnmpAgentData->pstOnlineUserInfo = SNMP_AGENT_MALLOC(
            sizeof(AP_STAT_ONLINEUSER_ENTRY_T)*ulTableMaxRecNum);
		if (NULL == g_pstSnmpAgentData->pstOnlineUserInfo)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstOnlineUserInfo failed\n",
				SNMPAGENT_CONTROL_NAME);
			return FALSE;
		}
	}
	memset(g_pstSnmpAgentData->pstOnlineUserInfo, 0, sizeof(AP_STAT_ONLINEUSER_ENTRY_T)*ulTableMaxRecNum);

    //todo
    //test code
    UINT8 aucMac1[6] = {0xff,0x22,0x33,0x44,0x55,0x66};
    UINT8 aucMac2[6] = {0xff,0x33,0x33,0x33,0x33,0x33};
    memcpy(g_pstSnmpAgentData->pstOnlineUserInfo[0].aucUserMac, aucMac1, 6);
    g_pstSnmpAgentData->pstOnlineUserInfo[0].ulAuthType = 1;
    g_pstSnmpAgentData->pstOnlineUserInfo[0].ulUpLoad= 1000;
    g_pstSnmpAgentData->pstOnlineUserInfo[0].ulDwLoad= 2000;
    g_pstSnmpAgentData->pstOnlineUserInfo[0].ulConnect= 321;
    g_pstSnmpAgentData->pstOnlineUserInfo[0].ulTotalUp= 5000;
    g_pstSnmpAgentData->pstOnlineUserInfo[0].ulTotalDw= 6000;
    g_pstSnmpAgentData->pstOnlineUserInfo[0].ulConnectTime= 600;
    strcpy(g_pstSnmpAgentData->pstOnlineUserInfo[0].acUserId, "qefsdcdsdsdsd");
    memcpy(g_pstSnmpAgentData->pstOnlineUserInfo[1].aucUserMac, aucMac2, 6);
    g_pstSnmpAgentData->pstOnlineUserInfo[1].ulAuthType = 2;
    g_pstSnmpAgentData->pstOnlineUserInfo[1].ulUpLoad= 1050;
    g_pstSnmpAgentData->pstOnlineUserInfo[1].ulDwLoad= 2050;
    g_pstSnmpAgentData->pstOnlineUserInfo[1].ulConnect= 444;
    g_pstSnmpAgentData->pstOnlineUserInfo[1].ulTotalUp= 5050;
    g_pstSnmpAgentData->pstOnlineUserInfo[1].ulTotalDw= 6050;
    g_pstSnmpAgentData->pstOnlineUserInfo[1].ulConnectTime= 800;
    
    return TRUE;
}
BOOL ac_agent_get_apinfo()
{
    BOOL bRet = FALSE;
    UINT32 ulTableMaxRecNum = TABLE_MAXRECNUM_AP;

	if (NULL == g_pstSnmpAgentData)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}
    
	if (NULL == g_pstSnmpAgentData->pstApInfo)
	{
		g_pstSnmpAgentData->pstApInfo = SNMP_AGENT_MALLOC(
            sizeof(AP_STAT_APINFO_ENTRY_T)*ulTableMaxRecNum);
		if (NULL == g_pstSnmpAgentData->pstApInfo)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstApInfo failed\n",
				SNMPAGENT_CONTROL_NAME);
			return FALSE;
		}
	}
	memset(g_pstSnmpAgentData->pstApInfo, 0, sizeof(AP_STAT_APINFO_ENTRY_T)*ulTableMaxRecNum);
    g_pstSnmpAgentData->ulApNum = 0;

    if( FALSE == agent_update_apinfo(g_pstSnmpAgentData->pstApInfo, &g_pstSnmpAgentData->ulApNum))
    {
        agent_printf(LOG_ERR, "agent_update_apinfo fail.\n");
        return FALSE;
    }
    agent_printf(LOG_INFO, "ac_agent_get_apinfo success.\n");
    return TRUE;
}
BOOL ac_agent_get_apversioninfo()
{
    BOOL bRet = FALSE;
    UINT32 ulTableMaxRecNum = TABLE_MAXRECNUM_AP;

	if (NULL == g_pstSnmpAgentData)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}
    
	if (NULL == g_pstSnmpAgentData->pstApVersionInfo)
	{
		g_pstSnmpAgentData->pstApVersionInfo = SNMP_AGENT_MALLOC(
            sizeof(AP_STAT_VERSION_ENTRY_T)*ulTableMaxRecNum);
		if (NULL == g_pstSnmpAgentData->pstApVersionInfo)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstApVersionInfo failed\n",
				SNMPAGENT_CONTROL_NAME);
			return FALSE;
		}
	}
	memset(g_pstSnmpAgentData->pstApVersionInfo, 0, sizeof(AP_STAT_VERSION_ENTRY_T)*ulTableMaxRecNum);
    if( FALSE == agent_update_apversioninfo(g_pstSnmpAgentData->pstApVersionInfo))
    {
        agent_printf(LOG_ERR, "agent_update_apversioninfo fail.");
        return FALSE;
    }
    agent_printf(LOG_INFO, "ac_agent_get_apversioninfo success.");
#if 0
    AP_HASH_T *pstAp;
    struct hlist_node *p;
    int i=0;

    AP_LIST_LOCK();
    for(i=0; i<AP_HASH_SIZE; ++i){
        hlist_for_each(p, &g_ap_HList[i])
        {
            pstAp = hlist_entry(p, AP_HASH_T, mac_Hlist);
            if ( NULL != pstAp )
            {
                g_pstSnmpAgentData->pstApVersionInfo[i].ulApNo = i+1;
                memcpy(g_pstSnmpAgentData->pstApVersionInfo[i].aucApMac, pstAp->stApDetailInfo.acMac, 6);
                strcpy(g_pstSnmpAgentData->pstApVersionInfo[i].acCurVer, pstAp->stApDetailInfo.softversion);
                strcpy(g_pstSnmpAgentData->pstApVersionInfo[i].acNewVer, pstAp->stApDetailInfo.softversion);
                g_pstSnmpAgentData->pstApVersionInfo[i].ulApStatus = pstAp->stApDetailInfo.iRunStatus;
            }
            else
            {
                agent_printf(LOG_ERR, "pstAp is null.\n");
                break;
            }
        }
    }
    AP_LIST_UNLOCK();
#endif
    return TRUE;
}
BOOL ac_agent_get_apstainfo()
{
    BOOL bRet = FALSE;
    UINT32 ulTableMaxRecNum = TABLE_MAXRECNUM_USER;

	if (NULL == g_pstSnmpAgentData)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}
    
	if (NULL == g_pstSnmpAgentData->pstGwApStaInfo)
	{
		g_pstSnmpAgentData->pstGwApStaInfo = SNMP_AGENT_MALLOC(
            sizeof(AP_STAT_AP_STA_ENTRY_T)*ulTableMaxRecNum);
		if (NULL == g_pstSnmpAgentData->pstGwApStaInfo)
		{
			agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstGwApStaInfo failed\n",
				SNMPAGENT_CONTROL_NAME);
			return FALSE;
		}
	}
	memset(g_pstSnmpAgentData->pstGwApStaInfo, 0, sizeof(AP_STAT_AP_STA_ENTRY_T)*ulTableMaxRecNum);

    if ( FALSE == agent_update_apstainfo( g_pstSnmpAgentData->pstGwApStaInfo ) )
    {
        agent_printf(LOG_ERR, "agent_update_apstainfo fail.");
        return FALSE;
    }
    agent_printf(LOG_INFO, "ac_agent_get_apstainfo success.");

    #if 0
    //todo
    //test code
    UINT8 aucMac1[6] = {0x11,0x22,0x33,0x44,0x55,0x66};
    UINT8 aucMac2[6] = {0x33,0x33,0x33,0x33,0x33,0x33};
    UINT8 aucIp1[4] = {1,1,1,1};
    UINT8 aucIp2[4] = {2,2,2,2};
    g_pstSnmpAgentData->pstGwApStaInfo[0].ulStaNo =1;
    strcpy(g_pstSnmpAgentData->pstGwApStaInfo[0].acStaName, "用户一");
    memcpy(g_pstSnmpAgentData->pstGwApStaInfo[0].aucStaMac, aucMac1,6);
    memcpy(g_pstSnmpAgentData->pstGwApStaInfo[0].aucStaIp, aucIp1,4);
    memcpy(g_pstSnmpAgentData->pstGwApStaInfo[0].aucApMac, aucMac1,6);
    g_pstSnmpAgentData->pstGwApStaInfo[0].ulRssi=66;
    g_pstSnmpAgentData->pstGwApStaInfo[0].ulConnectTime=3623;
    g_pstSnmpAgentData->pstGwApStaInfo[0].ulStaTotalUp=1478;
    g_pstSnmpAgentData->pstGwApStaInfo[0].ulStaTotalDw=74123;
    g_pstSnmpAgentData->pstGwApStaInfo[1].ulStaNo =2;
    strcpy(g_pstSnmpAgentData->pstGwApStaInfo[1].acStaName, "用户二");
    memcpy(g_pstSnmpAgentData->pstGwApStaInfo[1].aucStaMac, aucMac2,6);
    memcpy(g_pstSnmpAgentData->pstGwApStaInfo[1].aucStaIp, aucIp2,4);
    memcpy(g_pstSnmpAgentData->pstGwApStaInfo[1].aucApMac, aucMac2,6);
    g_pstSnmpAgentData->pstGwApStaInfo[1].ulRssi=45;
    g_pstSnmpAgentData->pstGwApStaInfo[1].ulConnectTime=3456;
    g_pstSnmpAgentData->pstGwApStaInfo[1].ulStaTotalUp=6789;
    g_pstSnmpAgentData->pstGwApStaInfo[1].ulStaTotalDw=3456;
    g_pstSnmpAgentData->pstGwApStaInfo[2].ulStaNo =3;
    strcpy(g_pstSnmpAgentData->pstGwApStaInfo[2].acStaName, "用户三");
    memcpy(g_pstSnmpAgentData->pstGwApStaInfo[2].aucStaMac, aucMac2,6);
    memcpy(g_pstSnmpAgentData->pstGwApStaInfo[2].aucStaIp, aucIp2,4);
    memcpy(g_pstSnmpAgentData->pstGwApStaInfo[2].aucApMac, aucMac1,6);
    g_pstSnmpAgentData->pstGwApStaInfo[2].ulRssi=45;
    g_pstSnmpAgentData->pstGwApStaInfo[2].ulConnectTime=3456;
    g_pstSnmpAgentData->pstGwApStaInfo[2].ulStaTotalUp=6789;
    g_pstSnmpAgentData->pstGwApStaInfo[2].ulStaTotalDw=3456;
    #endif
    
    return TRUE;
}
/* END: Added by zoucaihong, 2015/3/25 */
#if 0
/* BEGIN: Added by zoucaihong, 2013/1/21 */
/*****************************************************************************
 Prototype    : ac_agent_get_ap24cochainterfinfo
 Description  : 获取AP 2.4G卡同频干扰信息
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/22
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL ac_agent_get_ap24cochainterfinfo()
{
    UINT32 ulTableMaxRecNum = TABLE_MAXRECNUM_INTERF;
    UINT32 ulNum = 0;
    BOOL bRet = FALSE;
    
    if ( NULL == g_pstSnmpAgentData )
    {
        agent_printf(LOG_ERR, "%s g_pstSnmpAgentData is null.\n", SNMPAGENT_CONTROL_NAME);
    }

    if ( NULL == g_pstSnmpAgentData->pstAp24CoChaInterfInfo )
    {
        g_pstSnmpAgentData->pstAp24CoChaInterfInfo = SNMP_AGENT_MALLOC(
            sizeof(AP_STAT_24COCHAINTERFINFO_ENTRY_T)*ulTableMaxRecNum);
        if ( NULL == g_pstSnmpAgentData->pstAp24CoChaInterfInfo )
        {
            agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstAp24CoChaInterfInfo failed.\n",
                SNMPAGENT_CONTROL_NAME);
            return FALSE;
        }
    }

    memset(g_pstSnmpAgentData->pstAp24CoChaInterfInfo, 0, 
        sizeof(AP_STAT_24COCHAINTERFINFO_ENTRY_T)*ulTableMaxRecNum);
    g_pstSnmpAgentData->stAp24CoChaInterfInfo.ulNum = 0;

    bRet = wlan_get_ap24cochainterfinfo(g_pstSnmpAgentData->pstAp24CoChaInterfInfo, &ulNum);
    if ( FALSE == bRet )
    {
        agent_printf(LOG_ERR, "wlan_get_ap24cochainterfinfo failed.\n");
        return FALSE;
    }
    g_pstSnmpAgentData->stAp24CoChaInterfInfo.ulNum = ulNum;

    return TRUE;
}
/*****************************************************************************
 Prototype    : ac_agent_get_ap5cochainterfinfo
 Description  : 获取AP 5G卡同频干扰信息
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/22
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL ac_agent_get_ap5cochainterfinfo()
{
    UINT32 ulTableMaxRecNum = TABLE_MAXRECNUM_INTERF;
    UINT32 ulNum = 0;
    BOOL bRet = FALSE;
    
    if ( NULL == g_pstSnmpAgentData )
    {
        agent_printf(LOG_ERR, "%s g_pstSnmpAgentData is null.\n", SNMPAGENT_CONTROL_NAME);
    }

    if ( NULL == g_pstSnmpAgentData->pstAp5CoChaInterfInfo )
    {
        g_pstSnmpAgentData->pstAp5CoChaInterfInfo = SNMP_AGENT_MALLOC(
            sizeof(AP_STAT_5COCHAINTERFINFO_ENTRY_T)*ulTableMaxRecNum);
        if ( NULL == g_pstSnmpAgentData->pstAp5CoChaInterfInfo )
        {
            agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstAp5CoChaInterfInfo failed.\n",
                SNMPAGENT_CONTROL_NAME);
            return FALSE;    
        }
    }

    memset(g_pstSnmpAgentData->pstAp5CoChaInterfInfo, 0, 
        sizeof(AP_STAT_5COCHAINTERFINFO_ENTRY_T)*ulTableMaxRecNum);
    g_pstSnmpAgentData->stAp5CoChaInterfInfo.ulNum = 0;

    bRet = wlan_get_ap5cochainterfinfo(g_pstSnmpAgentData->pstAp5CoChaInterfInfo, &ulNum);
    if ( FALSE == bRet )
    {
        agent_printf(LOG_ERR, "wlan_get_ap5cochainterfinfo failed.\n");
        return FALSE;
    }
    g_pstSnmpAgentData->stAp5CoChaInterfInfo.ulNum = ulNum;

    return TRUE;
}
/*****************************************************************************
 Prototype    : ac_agent_get_ap24adjchainterfinfo
 Description  : 获取AP 2.4G卡邻频干扰信息
 Input        : None
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/22
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL ac_agent_get_ap24adjchainterfinfo()
{
    UINT32 ulTableMaxRecNum = TABLE_MAXRECNUM_INTERF;
    UINT32 ulNum = 0;
    BOOL bRet = FALSE;
    
    if ( NULL == g_pstSnmpAgentData )
    {
        agent_printf(LOG_ERR, "%s g_pstSnmpAgentData is null.\n", SNMPAGENT_CONTROL_NAME);
    }

    if ( NULL == g_pstSnmpAgentData->pstAp24AdjChaInterfInfo )
    {
        g_pstSnmpAgentData->pstAp24AdjChaInterfInfo = SNMP_AGENT_MALLOC(
            sizeof(AP_STAT_24ADJCHAINTERFINFO_ENTRY_T)*ulTableMaxRecNum);
        if ( NULL == g_pstSnmpAgentData->pstAp24AdjChaInterfInfo )
        {
            agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstAp24AdjChaInterfInfo failed.\n",
                SNMPAGENT_CONTROL_NAME);
            return FALSE;
        }
    }

    memset(g_pstSnmpAgentData->pstAp24AdjChaInterfInfo, 0, 
        sizeof(AP_STAT_24ADJCHAINTERFINFO_ENTRY_T)*ulTableMaxRecNum);
    g_pstSnmpAgentData->stAp24AdjChaInterfInfo.ulNum = 0;

    bRet = wlan_get_ap24adjchainterfinfo(g_pstSnmpAgentData->pstAp24AdjChaInterfInfo, &ulNum);
    if ( FALSE == bRet )
    {
        agent_printf(LOG_ERR, "wlan_get_ap24adjchainterfinfo failed.\n");
        return FALSE;
    }
    g_pstSnmpAgentData->stAp24AdjChaInterfInfo.ulNum = ulNum;

    return TRUE;
}
/*****************************************************************************
 Prototype    : ac_agent_get_ap5adjchainterfinfo
 Description  : 获取AP 5G卡邻频干扰信息
 Input        : None
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/22
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL ac_agent_get_ap5adjchainterfinfo()
{
    UINT32 ulTableMaxRecNum = TABLE_MAXRECNUM_INTERF;
    UINT32 ulNum = 0;
    BOOL bRet = FALSE;
    
    if ( NULL == g_pstSnmpAgentData )
    {
        agent_printf(LOG_ERR, "%s g_pstSnmpAgentData is null.\n", SNMPAGENT_CONTROL_NAME);
    }

    if ( NULL == g_pstSnmpAgentData->pstAp5AdjChaInterfInfo )
    {
        g_pstSnmpAgentData->pstAp5AdjChaInterfInfo = SNMP_AGENT_MALLOC(
            sizeof(AP_STAT_5ADJCHAINTERFINFO_ENTRY_T)*ulTableMaxRecNum);
        if ( NULL == g_pstSnmpAgentData->pstAp5AdjChaInterfInfo )
        {
            agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstAp5AdjChaInterfInfo failed.\n",
                SNMPAGENT_CONTROL_NAME);
            return FALSE;
        }
    }

    memset(g_pstSnmpAgentData->pstAp5AdjChaInterfInfo, 0, 
        sizeof(AP_STAT_5ADJCHAINTERFINFO_ENTRY_T)*ulTableMaxRecNum);
    g_pstSnmpAgentData->stAp5AdjChaInterfInfo.ulNum = 0;

    bRet = wlan_get_ap5adjchainterfinfo(g_pstSnmpAgentData->pstAp5AdjChaInterfInfo, &ulNum);
    if ( FALSE == bRet )
    {
        agent_printf(LOG_ERR, "wlan_get_ap5adjchainterfinfo failed.\n");
        return FALSE;
    }
    g_pstSnmpAgentData->stAp5AdjChaInterfInfo.ulNum = ulNum;

    return TRUE;
}
/*****************************************************************************
 Prototype    : ac_agent_get_ap24wlandevinterfinfo
 Description  : 获取AP 2.4G卡WLAN设备干扰信息
 Input        : None
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/22
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL ac_agent_get_ap24wlandevinterfinfo()
{
    UINT32 ulTableMaxRecNum = TABLE_MAXRECNUM_INTERF;
    UINT32 ulNum = 0;
    BOOL bRet = FALSE;
    
    if ( NULL == g_pstSnmpAgentData )
    {
        agent_printf(LOG_ERR, "%s g_pstSnmpAgentData is null.\n", SNMPAGENT_CONTROL_NAME);
    }

    if ( NULL == g_pstSnmpAgentData->pstAp24WlanDevInterfInfo )
    {
        g_pstSnmpAgentData->pstAp24WlanDevInterfInfo = SNMP_AGENT_MALLOC(
            sizeof(AP_STAT_24WLANDEVINTERFINFO_ENTRY_T)*ulTableMaxRecNum);
        if ( NULL == g_pstSnmpAgentData->pstAp24WlanDevInterfInfo )
        {
            agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstAp24WlanDevInterfInfo failed.\n",
                SNMPAGENT_CONTROL_NAME);
            return FALSE;
        }
    }

    memset(g_pstSnmpAgentData->pstAp24WlanDevInterfInfo, 0, 
        sizeof(AP_STAT_24WLANDEVINTERFINFO_ENTRY_T)*ulTableMaxRecNum);
    g_pstSnmpAgentData->stAp24WlanDevInterfInfo.ulNum = 0;

    bRet = wlan_get_ap24wlandevinterfinfo(g_pstSnmpAgentData->pstAp24WlanDevInterfInfo, &ulNum);
    if ( FALSE == bRet )
    {
        agent_printf(LOG_ERR, "wlan_get_ap24wlandevinterfinfo failed.\n");
        return FALSE;
    }
    g_pstSnmpAgentData->stAp24WlanDevInterfInfo.ulNum = ulNum;

    return TRUE;
}
/*****************************************************************************
 Prototype    : ac_agent_get_ap5wlandevinterfinfo
 Description  : 获取AP 5G卡WLAN设备干扰信息
 Input        : None
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/22
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL ac_agent_get_ap5wlandevinterfinfo()
{
    UINT32 ulTableMaxRecNum = TABLE_MAXRECNUM_INTERF;
    UINT32 ulNum = 0;
    BOOL bRet = FALSE;
    
    if ( NULL == g_pstSnmpAgentData )
    {
        agent_printf(LOG_ERR, "%s g_pstSnmpAgentData is null.\n", SNMPAGENT_CONTROL_NAME);
    }

    if ( NULL == g_pstSnmpAgentData->pstAp5WlanDevInterfInfo )
    {
        g_pstSnmpAgentData->pstAp5WlanDevInterfInfo = SNMP_AGENT_MALLOC(
            sizeof(AP_STAT_5WLANDEVINTERFINFO_ENTRY_T)*ulTableMaxRecNum);
        if ( NULL == g_pstSnmpAgentData->pstAp5WlanDevInterfInfo )
        {
            agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstAp5WlanDevInterfInfo failed.\n",
                SNMPAGENT_CONTROL_NAME);
            return FALSE;
        }
    }

    memset(g_pstSnmpAgentData->pstAp5WlanDevInterfInfo, 0, 
        sizeof(AP_STAT_5WLANDEVINTERFINFO_ENTRY_T)*ulTableMaxRecNum);
    g_pstSnmpAgentData->stAp5WlanDevInterfInfo.ulNum = 0;

    bRet = wlan_get_ap5wlandevinterfinfo(g_pstSnmpAgentData->pstAp5WlanDevInterfInfo, &ulNum);
    if ( FALSE == bRet )
    {
        agent_printf(LOG_ERR, "wlan_get_ap5wlandevinterfinfo failed.\n");
        return FALSE;
    }
    g_pstSnmpAgentData->stAp5WlanDevInterfInfo.ulNum = ulNum;

    return TRUE;
}
/* BEGIN: Added by zoucaihong, 2013/5/10 */
/*****************************************************************************
 Prototype    : ac_agent_get_ap24rogueapinfo
 Description  : AP 2.4G非法AP检测列表
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/5/10
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL ac_agent_get_ap24rogueapinfo()
{
    UINT32 ulTableMaxRecNum = TABLE_MAXRECNUM_ROGUEAP;
    UINT32 ulNum = 0;
    BOOL bRet = FALSE;
    
    if ( NULL == g_pstSnmpAgentData )
    {
        agent_printf(LOG_ERR, "%s g_pstSnmpAgentData is null.\n", SNMPAGENT_CONTROL_NAME);
    }

    if ( NULL == g_pstSnmpAgentData->pstAp24RogueApInfo )
    {
        g_pstSnmpAgentData->pstAp24RogueApInfo = SNMP_AGENT_MALLOC(
            sizeof(AP_STAT_24ROGUEAPINFO_ENTRY_T)*ulTableMaxRecNum);
        if ( NULL == g_pstSnmpAgentData->pstAp24RogueApInfo )
        {
            agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstAp24RogueApInfo failed.\n",
                SNMPAGENT_CONTROL_NAME);
            return FALSE;
        }
    }

    memset(g_pstSnmpAgentData->pstAp24RogueApInfo, 0, 
        sizeof(AP_STAT_24ROGUEAPINFO_ENTRY_T)*ulTableMaxRecNum);
    g_pstSnmpAgentData->stAp24RogueApInfo.ulNum = 0;

    bRet = wlan_get_ap24rogueapinfo(g_pstSnmpAgentData->pstAp24RogueApInfo, &ulNum);
    if ( FALSE == bRet )
    {
        agent_printf(LOG_ERR, "ac_agent_get_ap24rogueapinfo failed.\n");
        return FALSE;
    }
    g_pstSnmpAgentData->stAp24RogueApInfo.ulNum = ulNum;

    return TRUE;
}
/*****************************************************************************
 Prototype    : ac_agent_get_ap5rogueapinfo
 Description  : AP 5G非法AP检测列表
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/5/10
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL ac_agent_get_ap5rogueapinfo()
{
    UINT32 ulTableMaxRecNum = TABLE_MAXRECNUM_ROGUEAP;
    UINT32 ulNum = 0;
    BOOL bRet = FALSE;
    
    if ( NULL == g_pstSnmpAgentData )
    {
        agent_printf(LOG_ERR, "%s g_pstSnmpAgentData is null.\n", SNMPAGENT_CONTROL_NAME);
    }

    if ( NULL == g_pstSnmpAgentData->pstAp5RogueApInfo )
    {
        g_pstSnmpAgentData->pstAp5RogueApInfo = SNMP_AGENT_MALLOC(
            sizeof(AP_STAT_5ROGUEAPINFO_ENTRY_T)*ulTableMaxRecNum);
        if ( NULL == g_pstSnmpAgentData->pstAp5RogueApInfo )
        {
            agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstAp5RogueApInfo failed.\n",
                SNMPAGENT_CONTROL_NAME);
            return FALSE;
        }
    }

    memset(g_pstSnmpAgentData->pstAp5RogueApInfo, 0, 
        sizeof(AP_STAT_5ROGUEAPINFO_ENTRY_T)*ulTableMaxRecNum);
    g_pstSnmpAgentData->stAp5RogueApInfo.ulNum = 0;

    bRet = wlan_get_ap5rogueapinfo(g_pstSnmpAgentData->pstAp5RogueApInfo, &ulNum);
    agent_printf(LOG_ERR, "ac_agent_get_ap5rogueapinfo ulNum %d.\n", ulNum);
    if ( FALSE == bRet )
    {
        agent_printf(LOG_ERR, "ac_agent_get_ap5rogueapinfo failed.\n");
        return FALSE;
    }
    g_pstSnmpAgentData->stAp5RogueApInfo.ulNum = ulNum;

    return TRUE;
}
/* END: Added by zoucaihong, 2013/5/10 */

/*****************************************************************************
 Prototype    : ac_agent_get_nonwlandevinterfinfo
 Description  : 获取AP 非WLAN设备干扰信息
 Input        : None
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/22
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL ac_agent_get_apnonwlandevinterfinfo()
{
    BOOL bRet = FALSE;
    UINT32 ulTableMaxRecNum = 0;
    UINT32 ulNum = 0;
    
    if ( NULL == g_pstSnmpAgentData )
    {
        agent_printf(LOG_ERR, "%s g_pstSnmpAgentData is null.\n", SNMPAGENT_CONTROL_NAME);
    }

    if ( NULL == g_pstSnmpAgentData->pstApNonWlanDevInterfInfo )
    {
        g_pstSnmpAgentData->pstApNonWlanDevInterfInfo = SNMP_AGENT_MALLOC(
            sizeof(AP_STAT_NONWLANDEVINTERFINFO_ENTRY_T)*ulTableMaxRecNum);
        if ( NULL == g_pstSnmpAgentData->pstApNonWlanDevInterfInfo )
        {
            agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstApNonWlanDevInterfInfo failed.\n",
                SNMPAGENT_CONTROL_NAME);
            return FALSE;
        }
    }

    memset(g_pstSnmpAgentData->pstApNonWlanDevInterfInfo, 0, 
        sizeof(AP_STAT_NONWLANDEVINTERFINFO_ENTRY_T)*ulTableMaxRecNum);
    g_pstSnmpAgentData->stApNonWlanDevInterfInfo.ulNum = 0;

    bRet = wlan_get_apnonwlandevinterfinfo(g_pstSnmpAgentData->pstApNonWlanDevInterfInfo, &ulNum);
    if ( FALSE == bRet )
    {
        agent_printf(LOG_ERR, "wlan_get_apnonwlandevinterfinfo failed.\n");
        return FALSE;
    }
    g_pstSnmpAgentData->stApNonWlanDevInterfInfo.ulNum = ulNum;

    return TRUE;
}
/*****************************************************************************
 Prototype    : ac_agent_get_apbrmodestainfo
 Description  : 获取AP桥接模式时对接设备信息
 Input        : None
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/22
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL ac_agent_get_apbrmodestainfo()
{
    UINT32 ulTableMaxRecNum = TABLE_MAXRECNUM_STATION;
    UINT32 ulUserNum = 0;
    BOOL bRet = FALSE;
    
    if ( NULL == g_pstSnmpAgentData )
    {
        agent_printf(LOG_ERR, "%s g_pstSnmpAgentData is null.\n", SNMPAGENT_CONTROL_NAME);
    }

    if ( NULL == g_pstSnmpAgentData->pstApBrModeSta)
    {
        g_pstSnmpAgentData->pstApBrModeSta = SNMP_AGENT_MALLOC(
            sizeof(AP_STAT_BRMODESTA_ENTRY_T)*ulTableMaxRecNum);
        if ( NULL == g_pstSnmpAgentData->pstApBrModeSta )
        {
            agent_printf(LOG_ERR, "%s: malloc g_pstSnmpAgentData->pstApBrModeSta failed.\n",
                SNMPAGENT_CONTROL_NAME);
            return FALSE;
        }
    }

    memset(g_pstSnmpAgentData->pstApBrModeSta, 0, 
        sizeof(AP_STAT_BRMODESTA_ENTRY_T)*ulTableMaxRecNum);
    g_pstSnmpAgentData->stApBrModeSta.ulStationNum = 0;

    bRet = FALSE;//wlan_get_brmodestainfo(g_pstSnmpAgentData->pstApBrModeSta, &ulUserNum);
    if ( FALSE == bRet )
    {
        agent_printf(LOG_ERR, "wlan_get_apbrmodestainfo failed.\n");
        return FALSE;
    }
    g_pstSnmpAgentData->stApBrModeSta.ulStationNum = ulUserNum;

    return TRUE;
}
/* END:   Added by zoucaihong, 2013/1/21 */
#endif

