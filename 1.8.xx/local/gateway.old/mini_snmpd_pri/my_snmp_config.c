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
#include "my_snmp_info.h"
#include "my_snmp_handle.h"
#include "my_snmp_config.h"
#include "my_snmp_util.h"

#include "config_common.h"
#include "nvram_config.h"
//#include "interface.h"
#include "snmp_interface.h"
#include "tool.h"
#include "pal_network.h"

extern SNMP_AGENT_DATA_T *g_pstSnmpAgentData;
extern int g_stati_per;
/* BEGIN: Added by zoucaihong, 2014/5/14 */
extern DEV_STATE_E     g_enDevState;
/* END: Added by zoucaihong, 2014/5/14 */
extern UINT32 g_ulHeartTicks;       /* 心跳计时 */
extern UINT32 g_ulDisTicks;       /* discover超时计时 */
UINT32 g_ulSoftwareUpState = AP_SYSOPER_NORMAL;
#ifndef GW_X86
extern void set_5g_switch(char *acWifi5gSwitch);
extern BOOL set_11n_switch( char *acSwitch );
extern BOOL snmp_vlan_ssid_mapping(int iWlanId, int iVapId, int iVlanId);
extern BOOL snmp_vlan_status( int iVlanId, int iStatus, int iVapId, int iWlanId );
extern int wlan_del_vap(AP_CONFIG_SSID_ENTRY_T *pstSsidSetup);
extern BOOL wlan_add_vap(AP_CONFIG_SSID_ENTRY_T *pstSsidSetup);
extern int wifi_set_chainmask( char *pcDevName, int iChainMask );
#endif
/* BEGIN: Added by zoucaihong, 2014/6/17 */
BOOL app_restart_ap()
{
    char acStrCmd[256];
    memset(acStrCmd, 0, sizeof(acStrCmd));

    sprintf(acStrCmd, "reboot 5 snmp restart ap &");
    agent_printf(LOG_INFO, "acStrCmd:%s.", acStrCmd);
    system(acStrCmd);
    return TRUE;
}

#ifdef OPENWRT

BOOL app_reset_ap(UINT32 ulRestoreFlag)
{
	system("resore -a");
	return TRUE;
}
BOOL agent_recover_config(char *name)
{
	return TRUE;
}
#else
BOOL app_reset_ap(UINT32 ulRestoreFlag)
{
    char acStrCmd[256];    
    char nvram_name[NVRAM_LEN];
    char nvram_vale[NVRAM_LEN];

    memset(nvram_name, 0, NVRAM_LEN);
    memset(nvram_vale, 0, NVRAM_LEN);
    sprintf(nvram_name, NVRAM_RESTORE"all");
    sprintf(nvram_vale, "%d", ulRestoreFlag);
    if(NvramResultFailed == nvram_set(nvram_name, nvram_vale))
    {
        agent_printf(LOG_ERR, "set %s nvram name failed.", nvram_name);
        return FALSE;
    }
    if(NvramResultFailed == nvram_commit())
    {
        agent_printf(LOG_ERR, "app_reset_ap nvram commit failed.");
        return FALSE;
    }
    
    memset(acStrCmd, 0, sizeof(acStrCmd));
    sprintf(acStrCmd, "reboot 5 by snmp restore ap! &");
    system(acStrCmd);
    return TRUE;
}

/* END: Added by zoucaihong, 2014/6/17 */
BOOL agent_recover_config(char *name)
{
    FILE *pf;
    char *start, *index, *end;
    char buff[1024];
    const char opt[] = NOT_TO_DELETE;
    
    if(NULL == name)
    {
        agent_printf(LOG_ERR, "Func:%s name is null.\n", __func__);
        return FALSE;
    }

    pf = fopen(name, "rb");
    if(NULL == pf)
    {
        agent_printf(LOG_ERR, "Func:%s open %s fail.\n", __func__, name);
        return FALSE;
    }
    
    system("restore -c");
    while(NULL != fgets(buff, sizeof(buff), pf))
    {
        if(NULL == (index = strstr(buff, "=")))
        {
            continue;
        }
        start = buff;
        *index = '\0';
        index++;
        if(NULL == strstr(opt, start))
        {
            end = index;
            while(*end != '\n')
            {
                end++;
                continue;
            }
            *end = '\0';
            nvram_set(start, index);
        }
        start = index = NULL;
    }
    fclose(pf);
    nvram_commit();
    return TRUE;
}
#endif
/*****************************************************************************
 Prototype    : app_cfgfile_import
 Description  : 配置文件导入
 Input        : AP_OPER_FILETRANS_T *pstFileTransConfig  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/6/19
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL app_cfgfile_import( AP_OPER_FILETRANS_T *pstFileTrans )
{
    CHAR    acCmd[256];
    UINT8   aucInvalidIp[4] = {0};
    CHAR    *szFileName;

    /* 检查服务器IP配置 */
    if ( 0 == memcmp( aucInvalidIp, pstFileTrans->aucServerIp, 4 ) )
    {
        agent_printf(LOG_ERR, "Func:%s server ip is invalid(0.0.0.0).\n", __func__);
        return FALSE;
    }

    /* 检查文件路径名配置 */
    if ( 0 == strlen(pstFileTrans->acFileName) )
    {
        agent_printf(LOG_ERR, "Func:%s file path is empty.\n", __func__);
        return FALSE;
    }

    /* 检查文件名 */
    szFileName = app_parse_filename( pstFileTrans->acFileName );
    if ( NULL == szFileName )
    {
        agent_printf(LOG_ERR, "Func:%s szFileName is NULL.\n", __func__);
        return FALSE;
    }

    /* 从FTP服务器获取配置文件 */
    /* 
Usage: ftpget [options] remote-host local-file remote-file

Retrieve a remote file via FTP.

Options:
        -c, --continue         Continue a previous transfer
        -v, --verbose          Verbose
        -u, --username         Username to be used
        -p, --password         Password to be used
        -P, --port             Port number to be used    */
    sprintf(acCmd,"ftpget -u %s -p %s -P %u %u.%u.%u.%u %s %s",
        pstFileTrans->aucTransUser,
        pstFileTrans->aucTransPasswd,
        pstFileTrans->ulTransPort,
        IP_ARG(pstFileTrans->aucServerIp),
        CONFIG_FILE_DIR,
        pstFileTrans->acFileName);
    agent_printf(LOG_INFO,"acCmd:%s",acCmd);
    if(0 != system(acCmd))
    {
        agent_printf(LOG_ERR,"command excute error");
        return FALSE;
    }

    /* 生效配置文件 */
    if ( FALSE == agent_recover_config(CONFIG_FILE_DIR) )
    {
        agent_printf(LOG_ERR, "agent_recover_config fail.\n");
        return FALSE;
    }
#if (!defined(GW_X86) && !defined(GW_ARM) && !defined(GW_AC))
    system("reboot 5 by snmp import backup.dat &");
#endif 

    return TRUE;
}
/************************************************************************/
/* 备份配置文件                                                         */
/************************************************************************/
BOOL agent_backup_data()
{
    if(0 != system("nvramcli show >"BACKUP_NAME))
    {
        agent_printf(LOG_ERR, "shell exec nvramcli show failed");
        return FALSE;
    }
    return TRUE;
}
/************************************************************************/
/* 导出配置文件                                                         */
/************************************************************************/
BOOL app_cfgfile_export( AP_OPER_FILETRANS_T *pstFileTrans )
{
    CHAR    acCmd[256];
    UINT8   aucInvalidIp[4] = {0};
    CHAR    *szFileName;

    /* 检查服务器IP配置 */
    if ( 0 == memcmp( aucInvalidIp, pstFileTrans->aucServerIp, 4 ) )
    {
        agent_printf(LOG_ERR, "Func:%s server ip is invalid(0.0.0.0).\n", __func__);
        return FALSE;
    }

    /* 检查文件路径名配置 */
    if ( 0 == strlen(pstFileTrans->acFileName) )
    {
        agent_printf(LOG_ERR, "Func:%s file path is empty.\n", __func__);
        return FALSE;
    }

    /* 检查文件名 */
    szFileName = app_parse_filename( pstFileTrans->acFileName );
    if ( NULL == szFileName )
    {
        agent_printf(LOG_ERR, "Func:%s szFileName is NULL.\n", __func__);
        return FALSE;
    }

    /* 备份配置文件 */
    if ( FALSE == agent_backup_data() )
    {
        agent_printf(LOG_ERR, "agent_backup_data fail.\n");
        return FALSE;
    }

    /* 将本地配置文件上传到FTP服务器 */
    /* 
Usage: ftpput [options] remote-host remote-file local-file

Store a local file on a remote machine via FTP.

Options:
        -v, --verbose          Verbose
        -u, --username         Username to be used
        -p, --password         Password to be used
        -P, --port             Port number to be used    */
    sprintf(acCmd,"ftpput -u %s -p %s -P %u %u.%u.%u.%u %s %s",
        pstFileTrans->aucTransUser,
        pstFileTrans->aucTransPasswd,
        pstFileTrans->ulTransPort,
        IP_ARG(pstFileTrans->aucServerIp),
        pstFileTrans->acFileName,
        BACKUP_NAME);
    if(-1 == system(acCmd))
    {
        agent_printf(LOG_ERR,"command excute error");
        sprintf(acCmd, "rm -rf %s", BACKUP_NAME);
        system(acCmd);
        return FALSE;
    }
    sprintf(acCmd, "rm -rf %s", BACKUP_NAME);
    system(acCmd);
    return TRUE;
}
/************************************************************************/
/* 配置文件上传/下发                                                    */
/************************************************************************/
BOOL app_cfgfile_impexp( UINT32 ulFlag )
{
    if ( NULL == g_pstSnmpAgentData )
    {
        agent_printf(LOG_ERR, "Func:%s g_pstSnmpAgentData is null.\n", __func__);
        return FALSE;
    }

    if ( NULL == g_pstSnmpAgentData->pstApFileTransConfig )
    {
        agent_printf(LOG_ERR, "Func:%s g_pstSnmpAgentData->pstApFileTransConfig is null.\n", __func__);
        return FALSE;
    }

    switch ( ulFlag )
    {
        case AP_CFGFILE_IMPORT :
            return app_cfgfile_import(g_pstSnmpAgentData->pstApFileTransConfig);
        case AP_CFGFILE_EXPORT :
            return app_cfgfile_export(g_pstSnmpAgentData->pstApFileTransConfig);
    }

    return FALSE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL ac_agent_modify_ap_config_sys_devinfo(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_SYS_DEVINFO_T *pstData = (AP_CONFIG_SYS_DEVINFO_T *)pData;
    AP_CONFIG_SYS_DEVINFO_T *pstOriData = (AP_CONFIG_SYS_DEVINFO_T *)pOriData;
    CHAR acValue[32] = {0};
    
    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
    }

    if ( NULL == pOriData )
    {
        lprintf(LOG_ERR, "pOriData is null.");
    }

    /* SNMP数据刷新周期，在此周期内，SNMP收到GET报文返回的是本地缓存的数据，不会取最新数据 */
    if ( pstData->ulRegetPeriod != pstOriData->ulRegetPeriod )
    {
        /* BEGIN: Added by Zhou Shouya, 2013/8/7   PN:VLAN */
        /* 周期范围2-15s */
        if ( pstData->ulRegetPeriod < 2 || pstData->ulRegetPeriod > 15 )
        {
            pstData->ulRegetPeriod = AGENT_REFRESH_PERIOD_DFTVAL;
        }
        /* END:   Added by Zhou Shouya, 2013/8/7   PN:VLAN */
        sprintf( acValue, "%d", pstData->ulRegetPeriod );
        nvram_set(NVRAM_REGETPERIOD, acValue); //Milk: 这个是干嘛的? 不清楚了...
        g_stati_per=pstData->ulRegetPeriod;
    }

    /* BEGIN: Added by zoucaihong, 2013/4/24 */
    /*trap开关配置*/
    if ( pstData->ulTrapSwitch != pstOriData->ulTrapSwitch )
    {
        /* BEGIN: Added by Zhou Shouya, 2013/8/7   PN:VLAN */
        /* trap开关:off(0),on(1) */
        if ( pstData->ulTrapSwitch > 1 )
        {
            agent_printf(LOG_ERR, "ulTrapSwitch:%u invalid.\n", pstData->ulTrapSwitch);
            return FALSE;
        }
        /* END:   Added by Zhou Shouya, 2013/8/7   PN:VLAN */
        sprintf(acValue, "%u", pstData->ulTrapSwitch );
        nvram_set("trap_switch", acValue);
        g_traps_switch = pstData->ulTrapSwitch;
    }
    /* END: Added by zoucaihong, 2013/4/24 */
    /* BEGIN: Added by zoucaihong, 2014/5/14 */
    if ( pstData->ulSysManageState != pstOriData->ulSysManageState )
    {
        if ( MANAGE_STATE_ONLINE == pstData->ulSysManageState )
        {
            agent_printf(LOG_ERR, "MANAGE_STATE_ONLINE.\n");
            g_enDevState = DEV_STATE_RUNNING;
            g_ulHeartTicks = 0;/* 心跳计时清零 */
            g_ulDisTicks = 0;/* discover计时清零 */
            //agent_send_gw_basicinfo_trap();
            //agent_send_gw_dyninfo_trap();
            agent_send_gw_zcbasicinfo_trap();


            agent_printf(LOG_ERR, "MANAGE_STATE_ONLINE.\n");
        }
        else if ( MANAGE_STATE_OFFLINE == pstData->ulSysManageState )
        {
            g_enDevState = DEV_STATE_NONE;
        }
    }
    /* END: Added by zoucaihong, 2014/5/14 */
    if ( pstData->ulTimeOutInterval != pstOriData->ulTimeOutInterval )
    {
        sprintf(acValue, "%u", pstData->ulTimeOutInterval );
        nvram_set(NVRAM_TIMEOUTINTERVAL, acValue);
    }
    if ( pstData->ulHeartBeatPeriod != pstOriData->ulHeartBeatPeriod )
    {
        sprintf(acValue, "%u", pstData->ulHeartBeatPeriod );
        nvram_set(NVRAM_HEARTBEATPERIOD, acValue);
    }
    
    nvram_commit();

#if 0
    /* 重新获取配置信息 */
    if ( FALSE == ac_agent_get_apbasicsetup_config() )
    {
        agent_printf(LOG_ERR, "ac_agent_get_apbasicsetup_config fail.");
        return FALSE;
    }
#endif
        
    return TRUE;
}
#if 0
/*****************************************************************************
 Prototype    : ac_agent_modify_ap_config_advance_5gprior
 Description  : 修改AP 5G优先配置函数
 Input        : VOID *pData     
                VOID *pOriData  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/3/11
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL ac_agent_modify_ap_config_advance_5gprior(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_ADVANCE_5GPRIOR_T *pstData    = (AP_CONFIG_ADVANCE_5GPRIOR_T *)pData;
    AP_CONFIG_ADVANCE_5GPRIOR_T *pstOriData = (AP_CONFIG_ADVANCE_5GPRIOR_T *)pOriData;
    CHAR ac5GPrior[32]={0};
    
    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
    }

    if ( NULL == pOriData )
    {
        lprintf(LOG_ERR, "pOriData is null.");
    }

    if ( 0!= memcmp(&pstData->ul5GPriSwitch, &pstOriData->ul5GPriSwitch, 
        sizeof(AP_CONFIG_ADVANCE_5GPRIOR_T)) )
    {
        if ( pstData->ul5GPriSwitch != pstOriData->ul5GPriSwitch )
        {
            /* BEGIN: Added by Zhou Shouya, 2013/8/7   PN:VLAN */
            /* 5G优先开关:off(0),on(1) */
            if ( pstData->ul5GPriSwitch > 1 )
            {
                agent_printf(LOG_ERR, "ul5GPriSwitch:%u invalid.\n", pstData->ul5GPriSwitch);
                return FALSE;
            }
            /* END:   Added by Zhou Shouya, 2013/8/7   PN:VLAN */
            sprintf(ac5GPrior, "%d", pstData->ul5GPriSwitch);
            nvram_set(NVRAM_DEV"prior_5g", ac5GPrior);
        }
        if ( pstData->ul2dot4GDelayTime != pstOriData->ul2dot4GDelayTime )
        {
            sprintf(ac5GPrior, "%d", pstData->ul2dot4GDelayTime);
            nvram_set(NVRAM_DEV"prior_5g_delayTime", ac5GPrior);
        }
        if ( pstData->ulStaTimesOut != pstOriData->ulStaTimesOut )
        {
            sprintf(ac5GPrior, "%d", pstData->ulStaTimesOut);
            nvram_set(NVRAM_DEV"prior_5g_StaTimeOut", ac5GPrior);
        }
        app_set_advanced_5gprior(pstData);
    }
    
    nvram_commit();
    
    if ( FALSE == ac_agent_get_ap5gpriorsetup_config() )
    {
        lprintf(LOG_ERR,"ac_agent_get_ap5gpriorsetup_config failed.");
        return FALSE;
    }
    
    return TRUE;
}
/*****************************************************************************
 Prototype    : ac_agent_modify_ap_config_advance_11nprior
 Description  : 修改AP 11n配置函数
 Input        : VOID *pData     
                VOID *pOriData  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/3/11
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL ac_agent_modify_ap_config_advance_11nprior(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_ADVANCE_11NPRIOR_T *pstData    = (AP_CONFIG_ADVANCE_11NPRIOR_T *)pData;
    AP_CONFIG_ADVANCE_11NPRIOR_T *pstOriData = (AP_CONFIG_ADVANCE_11NPRIOR_T *)pOriData;
    CHAR ac11nPrior[32]={0};
    
    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
    }

    if ( NULL == pOriData )
    {
        lprintf(LOG_ERR, "pOriData is null.");
    }

    if ( 0!= memcmp(&pstData->ul11nPriSwitch, &pstOriData->ul11nPriSwitch, 
        sizeof(AP_CONFIG_ADVANCE_11NPRIOR_T)) )
    {
        if ( pstData->ul11nPriSwitch != pstOriData->ul11nPriSwitch )
        {
            /* BEGIN: Added by Zhou Shouya, 2013/8/7   PN:VLAN */
            /* 11n优先开关:off(0),on(1) */
            if ( pstData->ul11nPriSwitch > 1 )
            {
                agent_printf(LOG_ERR, "ul11nPriSwitch:%u invalid.\n", pstData->ul11nPriSwitch);
                return FALSE;
            }
            /* END:   Added by Zhou Shouya, 2013/8/7   PN:VLAN */
            sprintf(ac11nPrior, "%d", pstData->ul11nPriSwitch);
            nvram_set(NVRAM_DEV"prior_11n", ac11nPrior);
        }
        if(pstData->ul11bgDelayTime != pstOriData->ul11bgDelayTime)
        {
            sprintf(ac11nPrior, "%d", pstData->ul11bgDelayTime);
            nvram_set(NVRAM_DEV"prior_11n_delayTime", ac11nPrior);
        }

        if(pstData->ulStaTimesOut != pstOriData->ulStaTimesOut)
        {
            sprintf(ac11nPrior, "%d", pstData->ulStaTimesOut);
            nvram_set(NVRAM_DEV"prior_11n_StaTimeOut", ac11nPrior);
        }
        app_set_advanced_11nprior(pstData);
    }
    
    nvram_commit();
    
    if ( FALSE == ac_agent_get_ap11npriorsetup_config() )
    {
        lprintf(LOG_ERR,"ac_agent_get_ap11npriorsetup_config failed.");
        return FALSE;
    }
    
    return TRUE;
}
#endif
/*****************************************************************************
 Prototype    : ac_agent_modify_ap_config_ip
 Description  : 修改IP地址配置函数
 Input        : VOID *pData     
                VOID *pOriData  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/2/26
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL ac_agent_modify_ap_config_ip(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_IP_ENTRY_T    *pstData      = (AP_CONFIG_IP_ENTRY_T *)pData;
    AP_CONFIG_IP_ENTRY_T    *pstConfig    = g_pstSnmpAgentData->pstSysIpConfig;

	if (g_pstSnmpAgentData == NULL)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}
	
    if ( NULL == pOriData )
    {
        if ( FALSE == agent_modify_ap_ip(pstData, pstConfig) )
        {
            agent_printf(LOG_ERR, "agent_modify_ap_ip fail.\n");
            return FALSE;
        }
    }

    if ( FALSE == ac_agent_get_apipsetup_config() )
    {
        lprintf(LOG_ERR, "ac_agent_get_apipsetup_config failed.");
        return FALSE;
    }

    /* BEGIN: Added by zoucaihong, 2014/6/26 */
    /* IP配置修改后需要重新发现进行上线 */
    g_enDevState = DEV_STATE_NONE;
    /* END: Added by zoucaihong, 2014/6/26 */
    
	return TRUE;
}
#if 0
/*****************************************************************************
 Prototype    : ac_agent_modify_ap_config_legalap
 Description  : 修改AP合法AP配置
 Input        : VOID *pData     
                VOID *pOriData  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/5/13
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL ac_agent_modify_ap_config_legalap(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_LEGALAP_ENTRY_T    *pstData      = (AP_CONFIG_LEGALAP_ENTRY_T *)pData;
    AP_CONFIG_LEGALAP_ENTRY_T    *pstConfig    = g_pstSnmpAgentData->pstSysLegalApConfig;
    UINT32 ulLoopNo = 0;
    UINT32 ulLoopNo2 = 0;
    UINT32 ulTableMaxRecNum = TABLE_MAXRECNUM_LEGALAP;
    UINT32 ulRecNum = g_pstSnmpAgentData->stSysLegalApConfig.ulNum;

	if (g_pstSnmpAgentData == NULL)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}
	
    if ( NULL == pOriData )
    {
        /*修改或删除记录*/
        for ( ulLoopNo2 = 0 ; ulLoopNo2 <ulRecNum; ulLoopNo2++ )
        {
            for ( ulLoopNo = 0 ; ulLoopNo <ulTableMaxRecNum; ulLoopNo++ )
            {
				if (pstData[ulLoopNo].ulIndex== 0)
				{
					continue;
				}			

                if ( pstData[ulLoopNo].ulIndex == pstConfig[ulLoopNo2].ulIndex)
                {
                    if ( 0 != memcmp(pstData[ulLoopNo].aucLegalApBssid,
                        pstConfig[ulLoopNo2].aucLegalApBssid, 6) )
                    {
                        if ( FALSE == agent_modify_ap_legalap(&pstData[ulLoopNo], &pstConfig[ulLoopNo2]) )
                        {
                            agent_printf(LOG_ERR, "agent_modify_ap_legalap fail.");
                            return FALSE;
                        }
                    }
                    break;
                }
            }
            /*删除合法AP配置*/
            if ( ulLoopNo == ulTableMaxRecNum )
            {
                if ( FALSE == agent_delete_ap_legalap(&pstConfig[ulLoopNo2]) )
                {
                    agent_printf(LOG_ERR, "agent_delete_ap_legalap fail.");
                    return FALSE;
                }
            }
        }
    }
    else
    {
        /*增加合法AP配置*/
        for ( ulLoopNo2 = 0 ; ulLoopNo2 < ulRecNum; ulLoopNo2++ )
        {
            if ( pstData->ulIndex == pstConfig[ulLoopNo2].ulIndex)
            {
                lprintf(LOG_ERR, "adding record index is duplicate");
                return FALSE;
            }
        }
        if ( FALSE == agent_add_ap_legalap(pstData) )
        {
            agent_printf(LOG_ERR, "agent_add_ap_legalap fail.");
            return FALSE;
        }
    }

    if ( FALSE == ac_agent_get_aplegalap_config() )
    {
        lprintf(LOG_ERR, "ac_agent_get_aplegalap_config failed.");
        return FALSE;
    }
    
	return TRUE;
}
#endif

#ifndef GW_X86
#ifdef OPENWRT
BOOL ac_agent_modify_ap_config_vapbasic(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_VAPBASIC_ENTRY_T *pstData      = (AP_CONFIG_VAPBASIC_ENTRY_T *)pData;
    AP_CONFIG_VAPBASIC_ENTRY_T *pstConfig    = g_pstSnmpAgentData->pstApVapBasicConfig;
    UINT32 ulLoopNo = 0;
    UINT32 ulLoopNo2 = 0;
    UINT32 ulTableMaxRecNum = VAP_MAX_NUM;
    UINT32 ulRecNum = g_pstSnmpAgentData->stApVapConfig.ulVapNum;
    CHAR acNvramName[32] = {0};
    CHAR acNvramValue[32] = {0};
	CHAR acVapName[32] = {0};
	UINT32 acVlanId = 0;
	UINT32 acVlanIdOld = 0;
	vap_qos st_qos;

    if ( NULL == pOriData )
    {
        /*修改或删除记录*/
        for ( ulLoopNo2 = 0 ; ulLoopNo2 <ulRecNum; ulLoopNo2++ )
        {
            for ( ulLoopNo = 0 ; ulLoopNo <ulTableMaxRecNum; ulLoopNo++ )
            {                    
				if (pstData[ulLoopNo].ulIndex== 0)
				{
					continue;
				}			

    			if (pstData[ulLoopNo].ulRowStatus == ENUM_ROWSTATUS_DESTROY)
    			{
    				continue;
    			}

    			if (pstData[ulLoopNo].ulRowStatus == ENUM_ROWSTATUS_CREATEANDWAIT)
    			{
    				continue;
    			}

				    lprintf(LOG_INFO, "pstConfig[%u].ulIndex:%u",
                        ulLoopNo2,pstConfig[ulLoopNo2].ulIndex);
                    
                if ( pstData[ulLoopNo].ulIndex == pstConfig[ulLoopNo2].ulIndex)
                {
                
                    pal_wifi_get_vap_name(pstData[ulLoopNo].ulIndex,acVapName);
                    agent_printf(LOG_ERR, "agent_modify_vap_ssid name:%s .",acVapName);
					
                    if ( 0 != strcmp(pstData[ulLoopNo].acSsid, pstConfig[ulLoopNo2].acSsid ) )
                    {
                        sprintf(acNvramName,"wireless.@wifi-iface[%d].ssid",pstData[ulLoopNo].ulIndex);
                        uci_config_set(acNvramName, pstData[ulLoopNo].acSsid);
                        if ( 0 != pal_wifi_set_ssid(acVapName, pstData[ulLoopNo].acSsid))
                        {
                            agent_printf(LOG_ERR, "agent_modify_vap_ssid failed.");
                            return FALSE;
                        }
                    }
                    if ( pstData[ulLoopNo].ulSsidEnabled != pstConfig[ulLoopNo2].ulSsidEnabled )
                    {
                        /* BEGIN: Added by Zhou Shouya, 2013/8/7   PN:VLAN */
                        /* ssid是否启用:enable(1),disable(0) */
                        if ( pstData[ulLoopNo].ulSsidEnabled > 1 )
                        {
                            agent_printf(LOG_ERR, "ulSsidEnabled:%u invalid.\n", pstData[ulLoopNo].ulSsidEnabled);
                            return FALSE;
                        }
                        /* END:   Added by Zhou Shouya, 2013/8/7   PN:VLAN */
                        if(!pstData[ulLoopNo].ulSsidEnabled){
                            sprintf(acNvramName,"wireless.@wifi-iface[%d].disabled",pstData[ulLoopNo].ulIndex);
                            uci_config_set(acNvramName, pstData[ulLoopNo].ulSsidEnabled);
                        }
                        else{
                            sprintf(acNvramName,"wireless.@wifi-iface[%d].disabled",pstData[ulLoopNo].ulIndex);
                            uci_config_delete(acNvramName);
                        }
  
                        if ( 0 != pal_wifi_set_ssid_enabled(acVapName, pstData[ulLoopNo].ulSsidEnabled) )
                        {
                            agent_printf(LOG_ERR, "wifi_vap_runmode failed.");
                            return FALSE;
                        }
                    }
                    if ( pstData[ulLoopNo].ulSsidBroadcast != pstConfig[ulLoopNo2].ulSsidBroadcast)
                    {
                        /* BEGIN: Added by Zhou Shouya, 2013/8/7   PN:VLAN */
                        /* ssid是否广播:off(0),on(1) */
                        if ( pstData[ulLoopNo].ulSsidBroadcast > 1 )
                        {
                            agent_printf(LOG_ERR, "ulSsidBroadcast:%u invalid.\n", pstData[ulLoopNo].ulSsidBroadcast);
                            return FALSE;
                        }
                        /* END:   Added by Zhou Shouya, 2013/8/7   PN:VLAN */
                        sprintf(acNvramName,"wireless.@wifi-iface[%d].hidden",pstData[ulLoopNo].ulIndex);
                        uci_config_set(acNvramName, pstData[ulLoopNo].ulSsidBroadcast);
                            
                        if ( 0 != pal_wifi_set_ssid_hide(acVapName, pstData[ulLoopNo].ulSsidBroadcast) )
                        {
                            agent_printf(LOG_ERR, "agent_modify_vap_ssidbroadcast failed.");
                            return FALSE;
                        }
                    }


                    #if 0
                    if ( pstData[ulLoopNo].ulEndUserTime!= pstConfig[ulLoopNo2].ulEndUserTime)
                    {
                        /* 终端保活时间:大于20s */
                        if ( pstData[ulLoopNo].ulEndUserTime < 20 )
                        {
                            agent_printf(LOG_ERR, "ulEndUserTime:%u is less than 20s.\n",
                                pstData[ulLoopNo].ulEndUserTime);
                            return FALSE;
                        }
                        sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_STA_INACT, pstData[ulLoopNo].ulNvramId);
                        sprintf(acNvramValue, "%u", pstData[ulLoopNo].ulEndUserTime);
                        nvram_set(acNvramName, acNvramValue);
                        if ( 0 != wlan_set_inact(acVapName, pstData[ulLoopNo].ulEndUserTime) )
                        {
                            agent_printf(LOG_ERR, "agent_modify_vap_endusertime failed.");
                            return FALSE;
                        }
                    }
                    
                    /* BEGIN: Added by zoucaihong, 2013/4/22 */
                    if ( pstData[ulLoopNo].ulMaxSimultUsers != pstConfig[ulLoopNo2].ulMaxSimultUsers )
                    {
                        /* BEGIN: Added by Zhou Shouya, 2013/8/7   PN:VLAN */
                        /* 最大允许同时接入用户数:0-256 */
                        if ( pstData[ulLoopNo].ulMaxSimultUsers > 256 )
                        {
                            agent_printf(LOG_ERR, "ulMaxSimultUsers:%u invalid.\n", pstData[ulLoopNo].ulMaxSimultUsers);
                            return FALSE;
                        }
                        /* END:   Added by Zhou Shouya, 2013/8/7   PN:VLAN */
                        sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_SSID_MAX_USERS, pstData[ulLoopNo].ulNvramId);
                        sprintf(acNvramValue, "%u", pstData[ulLoopNo].ulMaxSimultUsers);
                        nvram_set(acNvramName, acNvramValue);
                        if ( 0 != wlan_set_ssid_max_users(acVapName, pstData[ulLoopNo].ulMaxSimultUsers) )
                        {
                            agent_printf(LOG_ERR, "agent_modify_vap_endusermaxnum failed.");
                            return FALSE;
                        }
                    }
                    /* END: Added by zoucaihong, 2013/4/22 */
                    /* BEGIN: Added by zoucaihong, 2013/1/22 */
                    /*修改VAP级流控配置*/
                    if ( pstData[ulLoopNo].ulFlowSwitch != pstConfig[ulLoopNo2].ulFlowSwitch
                        || pstData[ulLoopNo].ulUplinkMaxRate != pstConfig[ulLoopNo2].ulUplinkMaxRate
                        || pstData[ulLoopNo].ulDownlinkMaxRate != pstConfig[ulLoopNo2].ulDownlinkMaxRate)
                    {
                        if ( pstData[ulLoopNo].ulFlowSwitch != pstConfig[ulLoopNo2].ulFlowSwitch )
                        {
                            /* BEGIN: Added by Zhou Shouya, 2013/8/7   PN:VLAN */
                            /* vap流控开关:off(0),on(1) */
                            if ( pstData[ulLoopNo].ulFlowSwitch > 1 )
                            {
                                agent_printf(LOG_ERR, "ulFlowSwitch:%u invalid.\n", pstData[ulLoopNo].ulFlowSwitch);
                                return FALSE;
                            }
                            /* END:   Added by Zhou Shouya, 2013/8/7   PN:VLAN */
                            sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_SSID_TRAFFIC_TYPE, pstData[ulLoopNo].ulNvramId);
                            sprintf(acNvramValue, "%u", pstData[ulLoopNo].ulFlowSwitch);
                            nvram_set(acNvramName, acNvramValue);
                        }
                        if ( pstData[ulLoopNo].ulUplinkMaxRate != pstConfig[ulLoopNo2].ulUplinkMaxRate)
                        {
                            /* vap最大上行带宽:0-300000Kbps */
                            if ( pstData[ulLoopNo].ulUplinkMaxRate > 300000 )
                            {
                                agent_printf(LOG_ERR, "ulUplinkMaxRate:%u invalid.\n",  pstData[ulLoopNo].ulUplinkMaxRate);
                                return FALSE;
                            }
                            sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_SSID_MAX_UPLOAD, pstData[ulLoopNo].ulNvramId);
                            sprintf(acNvramValue, "%u", pstData[ulLoopNo].ulUplinkMaxRate);
                            nvram_set(acNvramName, acNvramValue);
                        }
                        if ( pstData[ulLoopNo].ulDownlinkMaxRate != pstConfig[ulLoopNo2].ulDownlinkMaxRate)
                        {
                            /* vap最大下行带宽:0-300000Kbps */
                            if ( pstData[ulLoopNo].ulDownlinkMaxRate > 300000 )
                            {
                                agent_printf(LOG_ERR, "ulUplinkMaxRate:%u invalid.\n",  pstData[ulLoopNo].ulDownlinkMaxRate);
                                return FALSE;
                            }
                            sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_SSID_MAX_DOWNLOAD, pstData[ulLoopNo].ulNvramId);
                            sprintf(acNvramValue, "%u", pstData[ulLoopNo].ulDownlinkMaxRate);
                            nvram_set(acNvramName, acNvramValue);
                        }
                        if ( FALSE == agent_modify_vap_flow(acVapName, 
                                                            pstData[ulLoopNo].ulFlowSwitch,
                                                            pstData[ulLoopNo].ulUplinkMaxRate,
                                                            pstData[ulLoopNo].ulDownlinkMaxRate) )
                        {
                            agent_printf(LOG_ERR, "agent_modify_vap_flow failed.");
                            return FALSE;
                        }
                    }
                    /* END: Added by zoucaihong, 2013/1/22 */
                    /* BEGIN: Added by zoucaihong, 2013/4/23 */
                    /*修改用户级流控*/
                    if ( pstData[ulLoopNo].ulFlowSwitch != pstConfig[ulLoopNo2].ulFlowSwitch
                        || pstData[ulLoopNo].ulUserUpload != pstConfig[ulLoopNo2].ulUserUpload
                        || pstData[ulLoopNo].ulUserDownload != pstConfig[ulLoopNo2].ulUserDownload)
                    {
                        if ( pstData[ulLoopNo].ulUserUpload != pstConfig[ulLoopNo2].ulUserUpload )
                        {
                            /* 用户最大上行带宽:0-300000Kbps */
                            if ( pstData[ulLoopNo].ulUserUpload > 300000 )
                            {
                                agent_printf(LOG_ERR, "ulUserUpload:%u invalid.\n",pstData[ulLoopNo].ulUserUpload);
                                return FALSE;
                            }
                            sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_STA_MAX_UPLOAD, pstData[ulLoopNo].ulNvramId);
                            sprintf(acNvramValue, "%u", pstData[ulLoopNo].ulUserUpload);
                            nvram_set(acNvramName, acNvramValue);
                        }
                        if ( pstData[ulLoopNo].ulUserDownload != pstConfig[ulLoopNo2].ulUserDownload )
                        {
                            /* 用户最大下行带宽:0-300000Kbps */
                            if ( pstData[ulLoopNo].ulUserDownload > 300000 )
                            {
                                agent_printf(LOG_ERR, "ulUserDownload:%u invalid.\n",pstData[ulLoopNo].ulUserDownload);
                                return FALSE;
                            }
                            sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_STA_MAX_DOWNLOAD, pstData[ulLoopNo].ulNvramId);
                            sprintf(acNvramValue, "%u", pstData[ulLoopNo].ulUserDownload);
                            nvram_set(acNvramName, acNvramValue);
                        }
                        memset(&st_qos, 0, sizeof(st_qos));
                        st_qos.traffic_type = pstData[ulLoopNo].ulFlowSwitch;
                        memcpy(st_qos.name, acVapName, sizeof(st_qos.name));
                        st_qos.ssid_max_upload = pstData[ulLoopNo].ulUplinkMaxRate;
                        st_qos.ssid_max_download = pstData[ulLoopNo].ulDownlinkMaxRate;
                        st_qos.sta_max_upload = pstData[ulLoopNo].ulUserUpload;
                        st_qos.sta_max_download = pstData[ulLoopNo].ulUserDownload;
                        st_qos.pcf = 0;
                        if ( 0 != sta_flow_write(&st_qos) )
                        {
                            agent_printf(LOG_ERR, "sta_flow_write fail.\n");
                            return FALSE;
                        }
                    }
                    /* END: Added by zoucaihong, 2013/4/23 */
                    /* BEGIN: Added by Zhou Shouya, 2013/7/24   PN:VLAN */
                    if(pstData[ulLoopNo].ulVlanId != pstConfig[ulLoopNo2].ulVlanId)
                    {
                        /* BEGIN: Added by Zhou Shouya, 2013/8/7   PN:VLAN */
                        /* vlan id:1-4094 */
                        if ( pstData[ulLoopNo].ulVlanId < 1 || pstData[ulLoopNo].ulVlanId > 4094 )
                        {
                            agent_printf(LOG_ERR, "ulVlanId:%u invalid.\n", pstData[ulLoopNo].ulVlanId);
                            return FALSE;
                        }
                        /* END:   Added by Zhou Shouya, 2013/8/7   PN:VLAN */
                        sprintf(acNvramName, NVRAM_VAP"%d_port_pvid", pstData[ulLoopNo].ulNvramId);
                        sprintf(acNvramValue, "%d", pstData[ulLoopNo].ulVlanId);
                        nvram_set(acNvramName, acNvramValue);
                        sprintf(acNvramName, NVRAM_VAP"%d_port_vlanEabled", pstData[ulLoopNo].ulNvramId);
                        nvram_get(acNvramValue, acNvramName, 32);
                        //nvram_set(acNvramName, "1");
                        if(1 == atoi(acNvramValue))
                        {
                            acVlanId = bridge_id_get(pstData[ulLoopNo].ulVlanId);
                            acVlanIdOld = bridge_id_get(pstConfig[ulLoopNo2].ulVlanId);
                            app_vlan_modify(acVlanId, acVlanIdOld, acVapName);
                        }
                    }

                    if(pstData[ulLoopNo].ulVlanEnabled != pstConfig[ulLoopNo2].ulVlanEnabled)
                    {
                        /* BEGIN: Added by Zhou Shouya, 2013/8/7   PN:VLAN */
                        /* ssid所在vlan是否启用:off(0), on(1) */
                        if ( pstData[ulLoopNo].ulVlanEnabled > 1 )
                        {
                            agent_printf(LOG_ERR, "ulVlanEnabled:%u invalid.\n", pstData[ulLoopNo].ulVlanEnabled);
                            return FALSE;
                        }
                        /* END:   Added by Zhou Shouya, 2013/8/7   PN:VLAN */
                        sprintf(acNvramName, NVRAM_VAP"%d_port_vlanEabled", pstData[ulLoopNo].ulNvramId);
                        sprintf(acNvramValue, "%d", pstData[ulLoopNo].ulVlanEnabled);
                        nvram_set(acNvramName, acNvramValue);
                        acVlanId = bridge_id_get(pstData[ulLoopNo].ulVlanId);
                        app_vlan_eabled(acVlanId, pstData[ulLoopNo].ulVlanEnabled, acVapName);
                    }
                    /* END:   Added by Zhou Shouya, 2013/7/24 */
                    #endif
                    
                    break;
                }
            }
            if ( ulLoopNo == ulTableMaxRecNum )
            {
                /*不允许删除ssid*/
                lprintf(LOG_ERR, "no deleting record.");
                return FALSE;
            }
        }
    }
    else
    {
        /*不允许新建ssid*/
        lprintf(LOG_ERR, "no adding record.");
        return FALSE;
    }
    
    //nvram_commit();
    uci_config_commit();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    

    if ( FALSE == ac_agent_get_apvapbasicsetup_config() )
    {
        lprintf(LOG_ERR, "ac_agent_get_apvapbasicsetup_config failed.");
        return FALSE;
    }

    return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL ac_agent_modify_ap_config_vapsecurity(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_VAPSECURITY_ENTRY_T *pstData      = (AP_CONFIG_VAPSECURITY_ENTRY_T *)pData;
    AP_CONFIG_VAPSECURITY_ENTRY_T *pstConfig    = g_pstSnmpAgentData->pstApVapSecurityConfig;
    UINT32  ulLoopNo = 0;
    UINT32  ulLoopNo2 = 0;
    UINT32  ulTableMaxRecNum = VAP_MAX_NUM;
    UINT32  ulRecNum = g_pstSnmpAgentData->stApVapConfig.ulVapNum;
	CHAR    acDevName[32] = "";

    if ( NULL == pOriData )
    {
        /*修改或删除记录*/
        for ( ulLoopNo2 = 0 ; ulLoopNo2 <ulRecNum; ulLoopNo2++ )
        {
            for ( ulLoopNo = 0 ; ulLoopNo <ulTableMaxRecNum; ulLoopNo++ )
            {
				if (pstData[ulLoopNo].ulIndex== 0)
				{
					continue;
				}			

    			if (pstData[ulLoopNo].ulRowStatus == ENUM_ROWSTATUS_DESTROY)
    			{
    				continue;
    			}

    			if (pstData[ulLoopNo].ulRowStatus == ENUM_ROWSTATUS_CREATEANDWAIT)
    			{
    				continue;
    			}

                if ( pstData[ulLoopNo].ulIndex == pstConfig[ulLoopNo2].ulIndex)
                {
                    if ( 0 != memcmp(&pstData[ulLoopNo].ulAuthenticationType,
                        &pstConfig[ulLoopNo2].ulAuthenticationType,
                        sizeof(AP_CONFIG_VAPSECURITY_ENTRY_T)-32-3*sizeof(UINT32)))
                    {
                    	sprintf(acDevName,"ap%d_%d", pstData[ulLoopNo].ulWifiId, 
                            pstData[ulLoopNo].ulVapId);
                        if ( FALSE == agent_modify_vap_security(&pstData[ulLoopNo], &pstConfig[ulLoopNo2], acDevName) )
                        {
                            agent_printf(LOG_ERR, "agent_modify_vap_security failed.");
                            return FALSE;
                        }
                    }
                    break;
                }
            }
            if ( ulLoopNo == ulTableMaxRecNum )
            {
                /*不允许删除ssid*/
                lprintf(LOG_ERR, "no deleting record.");
                return FALSE;
            }
        }
    }
    else
    {
        /*不允许新建ssid*/
        lprintf(LOG_ERR, "no adding record.");
        return FALSE;
    }

    if ( FALSE == ac_agent_get_apvapsecuritysetup_config() )
    {
        lprintf(LOG_ERR, "ac_agent_get_apvapsecuritysetup_config failed.");
        return FALSE;
    }

    return TRUE;
}
#else

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL ac_agent_modify_ap_config_vapbasic(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_VAPBASIC_ENTRY_T *pstData      = (AP_CONFIG_VAPBASIC_ENTRY_T *)pData;
    AP_CONFIG_VAPBASIC_ENTRY_T *pstConfig    = g_pstSnmpAgentData->pstApVapBasicConfig;
    UINT32 ulLoopNo = 0;
    UINT32 ulLoopNo2 = 0;
    UINT32 ulTableMaxRecNum = VAP_MAX_NUM;
    UINT32 ulRecNum = g_pstSnmpAgentData->stApVapConfig.ulVapNum;
    CHAR acNvramName[32] = {0};
    CHAR acNvramValue[32] = {0};
	CHAR acVapName[32] = {0};
	UINT32 acVlanId = 0;
	UINT32 acVlanIdOld = 0;
	vap_qos st_qos;

    if ( NULL == pOriData )
    {
        /*修改或删除记录*/
        for ( ulLoopNo2 = 0 ; ulLoopNo2 <ulRecNum; ulLoopNo2++ )
        {
            for ( ulLoopNo = 0 ; ulLoopNo <ulTableMaxRecNum; ulLoopNo++ )
            {                    
				if (pstData[ulLoopNo].ulIndex== 0)
				{
					continue;
				}			

    			if (pstData[ulLoopNo].ulRowStatus == ENUM_ROWSTATUS_DESTROY)
    			{
    				continue;
    			}

    			if (pstData[ulLoopNo].ulRowStatus == ENUM_ROWSTATUS_CREATEANDWAIT)
    			{
    				continue;
    			}

				    lprintf(LOG_INFO, "pstConfig[%u].ulIndex:%u",
                        ulLoopNo2,pstConfig[ulLoopNo2].ulIndex);
                    
                if ( pstData[ulLoopNo].ulIndex == pstConfig[ulLoopNo2].ulIndex)
                {
                	sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_VAP_NAME, pstData[ulLoopNo].ulNvramId);
                	nvram_get(acVapName, acNvramName, NVRAM_LEN);
                    if ( 0 != strcmp(pstData[ulLoopNo].acSsid, pstConfig[ulLoopNo2].acSsid ) )
                    {
                        sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_SSID_NAME, pstData[ulLoopNo].ulNvramId);
                        nvram_set(acNvramName, pstData[ulLoopNo].acSsid);
                        if ( 0 != wlan_set_ssid_name(acVapName, pstData[ulLoopNo].acSsid))
                        {
                            agent_printf(LOG_ERR, "agent_modify_vap_ssid failed.");
                            return FALSE;
                        }
                    }
                    if ( pstData[ulLoopNo].ulSsidEnabled != pstConfig[ulLoopNo2].ulSsidEnabled )
                    {
                        /* BEGIN: Added by Zhou Shouya, 2013/8/7   PN:VLAN */
                        /* ssid是否启用:enable(1),disable(0) */
                        if ( pstData[ulLoopNo].ulSsidEnabled > 1 )
                        {
                            agent_printf(LOG_ERR, "ulSsidEnabled:%u invalid.\n", pstData[ulLoopNo].ulSsidEnabled);
                            return FALSE;
                        }
                        /* END:   Added by Zhou Shouya, 2013/8/7   PN:VLAN */
                        sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_SSID_ENABLED, pstData[ulLoopNo].ulNvramId);
                        sprintf(acNvramValue, "%u", pstData[ulLoopNo].ulSsidEnabled);
                        nvram_set(acNvramName, acNvramValue);
                        if ( 0 != wlan_set_ssid_enabled(acVapName, pstData[ulLoopNo].ulSsidEnabled) )
                        {
                            agent_printf(LOG_ERR, "wifi_vap_runmode failed.");
                            return FALSE;
                        }
                    }
                    if ( pstData[ulLoopNo].ulSsidBroadcast != pstConfig[ulLoopNo2].ulSsidBroadcast)
                    {
                        /* BEGIN: Added by Zhou Shouya, 2013/8/7   PN:VLAN */
                        /* ssid是否广播:off(0),on(1) */
                        if ( pstData[ulLoopNo].ulSsidBroadcast > 1 )
                        {
                            agent_printf(LOG_ERR, "ulSsidBroadcast:%u invalid.\n", pstData[ulLoopNo].ulSsidBroadcast);
                            return FALSE;
                        }
                        /* END:   Added by Zhou Shouya, 2013/8/7   PN:VLAN */
                        sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_SSID_HIDE, pstData[ulLoopNo].ulNvramId);
                        sprintf(acNvramValue, "%u", pstData[ulLoopNo].ulSsidBroadcast);
                        nvram_set(acNvramName, acNvramValue);
                        if ( 0 != wlan_set_ssid_hide(acVapName, pstData[ulLoopNo].ulSsidBroadcast) )
                        {
                            agent_printf(LOG_ERR, "agent_modify_vap_ssidbroadcast failed.");
                            return FALSE;
                        }
                    }
                    if ( pstData[ulLoopNo].ulEndUserTime!= pstConfig[ulLoopNo2].ulEndUserTime)
                    {
                        /* 终端保活时间:大于20s */
                        if ( pstData[ulLoopNo].ulEndUserTime < 20 )
                        {
                            agent_printf(LOG_ERR, "ulEndUserTime:%u is less than 20s.\n",
                                pstData[ulLoopNo].ulEndUserTime);
                            return FALSE;
                        }
                        sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_STA_INACT, pstData[ulLoopNo].ulNvramId);
                        sprintf(acNvramValue, "%u", pstData[ulLoopNo].ulEndUserTime);
                        nvram_set(acNvramName, acNvramValue);
                        if ( 0 != wlan_set_inact(acVapName, pstData[ulLoopNo].ulEndUserTime) )
                        {
                            agent_printf(LOG_ERR, "agent_modify_vap_endusertime failed.");
                            return FALSE;
                        }
                    }
                    
                    /* BEGIN: Added by zoucaihong, 2013/4/22 */
                    if ( pstData[ulLoopNo].ulMaxSimultUsers != pstConfig[ulLoopNo2].ulMaxSimultUsers )
                    {
                        /* BEGIN: Added by Zhou Shouya, 2013/8/7   PN:VLAN */
                        /* 最大允许同时接入用户数:0-256 */
                        if ( pstData[ulLoopNo].ulMaxSimultUsers > 256 )
                        {
                            agent_printf(LOG_ERR, "ulMaxSimultUsers:%u invalid.\n", pstData[ulLoopNo].ulMaxSimultUsers);
                            return FALSE;
                        }
                        /* END:   Added by Zhou Shouya, 2013/8/7   PN:VLAN */
                        sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_SSID_MAX_USERS, pstData[ulLoopNo].ulNvramId);
                        sprintf(acNvramValue, "%u", pstData[ulLoopNo].ulMaxSimultUsers);
                        nvram_set(acNvramName, acNvramValue);
                        if ( 0 != wlan_set_ssid_max_users(acVapName, pstData[ulLoopNo].ulMaxSimultUsers) )
                        {
                            agent_printf(LOG_ERR, "agent_modify_vap_endusermaxnum failed.");
                            return FALSE;
                        }
                    }
                    /* END: Added by zoucaihong, 2013/4/22 */
                    /* BEGIN: Added by zoucaihong, 2013/1/22 */
                    /*修改VAP级流控配置*/
                    if ( pstData[ulLoopNo].ulFlowSwitch != pstConfig[ulLoopNo2].ulFlowSwitch
                        || pstData[ulLoopNo].ulUplinkMaxRate != pstConfig[ulLoopNo2].ulUplinkMaxRate
                        || pstData[ulLoopNo].ulDownlinkMaxRate != pstConfig[ulLoopNo2].ulDownlinkMaxRate)
                    {
                        if ( pstData[ulLoopNo].ulFlowSwitch != pstConfig[ulLoopNo2].ulFlowSwitch )
                        {
                            /* BEGIN: Added by Zhou Shouya, 2013/8/7   PN:VLAN */
                            /* vap流控开关:off(0),on(1) */
                            if ( pstData[ulLoopNo].ulFlowSwitch > 1 )
                            {
                                agent_printf(LOG_ERR, "ulFlowSwitch:%u invalid.\n", pstData[ulLoopNo].ulFlowSwitch);
                                return FALSE;
                            }
                            /* END:   Added by Zhou Shouya, 2013/8/7   PN:VLAN */
                            sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_SSID_TRAFFIC_TYPE, pstData[ulLoopNo].ulNvramId);
                            sprintf(acNvramValue, "%u", pstData[ulLoopNo].ulFlowSwitch);
                            nvram_set(acNvramName, acNvramValue);
                        }
                        if ( pstData[ulLoopNo].ulUplinkMaxRate != pstConfig[ulLoopNo2].ulUplinkMaxRate)
                        {
                            /* vap最大上行带宽:0-300000Kbps */
                            if ( pstData[ulLoopNo].ulUplinkMaxRate > 300000 )
                            {
                                agent_printf(LOG_ERR, "ulUplinkMaxRate:%u invalid.\n",  pstData[ulLoopNo].ulUplinkMaxRate);
                                return FALSE;
                            }
                            sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_SSID_MAX_UPLOAD, pstData[ulLoopNo].ulNvramId);
                            sprintf(acNvramValue, "%u", pstData[ulLoopNo].ulUplinkMaxRate);
                            nvram_set(acNvramName, acNvramValue);
                        }
                        if ( pstData[ulLoopNo].ulDownlinkMaxRate != pstConfig[ulLoopNo2].ulDownlinkMaxRate)
                        {
                            /* vap最大下行带宽:0-300000Kbps */
                            if ( pstData[ulLoopNo].ulDownlinkMaxRate > 300000 )
                            {
                                agent_printf(LOG_ERR, "ulUplinkMaxRate:%u invalid.\n",  pstData[ulLoopNo].ulDownlinkMaxRate);
                                return FALSE;
                            }
                            sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_SSID_MAX_DOWNLOAD, pstData[ulLoopNo].ulNvramId);
                            sprintf(acNvramValue, "%u", pstData[ulLoopNo].ulDownlinkMaxRate);
                            nvram_set(acNvramName, acNvramValue);
                        }
                        if ( FALSE == agent_modify_vap_flow(acVapName, 
                                                            pstData[ulLoopNo].ulFlowSwitch,
                                                            pstData[ulLoopNo].ulUplinkMaxRate,
                                                            pstData[ulLoopNo].ulDownlinkMaxRate) )
                        {
                            agent_printf(LOG_ERR, "agent_modify_vap_flow failed.");
                            return FALSE;
                        }
                    }
                    /* END: Added by zoucaihong, 2013/1/22 */
                    /* BEGIN: Added by zoucaihong, 2013/4/23 */
                    /*修改用户级流控*/
                    if ( pstData[ulLoopNo].ulFlowSwitch != pstConfig[ulLoopNo2].ulFlowSwitch
                        || pstData[ulLoopNo].ulUserUpload != pstConfig[ulLoopNo2].ulUserUpload
                        || pstData[ulLoopNo].ulUserDownload != pstConfig[ulLoopNo2].ulUserDownload)
                    {
                        if ( pstData[ulLoopNo].ulUserUpload != pstConfig[ulLoopNo2].ulUserUpload )
                        {
                            /* 用户最大上行带宽:0-300000Kbps */
                            if ( pstData[ulLoopNo].ulUserUpload > 300000 )
                            {
                                agent_printf(LOG_ERR, "ulUserUpload:%u invalid.\n",pstData[ulLoopNo].ulUserUpload);
                                return FALSE;
                            }
                            sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_STA_MAX_UPLOAD, pstData[ulLoopNo].ulNvramId);
                            sprintf(acNvramValue, "%u", pstData[ulLoopNo].ulUserUpload);
                            nvram_set(acNvramName, acNvramValue);
                        }
                        if ( pstData[ulLoopNo].ulUserDownload != pstConfig[ulLoopNo2].ulUserDownload )
                        {
                            /* 用户最大下行带宽:0-300000Kbps */
                            if ( pstData[ulLoopNo].ulUserDownload > 300000 )
                            {
                                agent_printf(LOG_ERR, "ulUserDownload:%u invalid.\n",pstData[ulLoopNo].ulUserDownload);
                                return FALSE;
                            }
                            sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_STA_MAX_DOWNLOAD, pstData[ulLoopNo].ulNvramId);
                            sprintf(acNvramValue, "%u", pstData[ulLoopNo].ulUserDownload);
                            nvram_set(acNvramName, acNvramValue);
                        }
                        memset(&st_qos, 0, sizeof(st_qos));
                        st_qos.traffic_type = pstData[ulLoopNo].ulFlowSwitch;
                        memcpy(st_qos.name, acVapName, sizeof(st_qos.name));
                        st_qos.ssid_max_upload = pstData[ulLoopNo].ulUplinkMaxRate;
                        st_qos.ssid_max_download = pstData[ulLoopNo].ulDownlinkMaxRate;
                        st_qos.sta_max_upload = pstData[ulLoopNo].ulUserUpload;
                        st_qos.sta_max_download = pstData[ulLoopNo].ulUserDownload;
                        st_qos.pcf = 0;
                        if ( 0 != sta_flow_write(&st_qos) )
                        {
                            agent_printf(LOG_ERR, "sta_flow_write fail.\n");
                            return FALSE;
                        }
                    }
                    /* END: Added by zoucaihong, 2013/4/23 */
                    /* BEGIN: Added by Zhou Shouya, 2013/7/24   PN:VLAN */
                    if(pstData[ulLoopNo].ulVlanId != pstConfig[ulLoopNo2].ulVlanId)
                    {
                        /* BEGIN: Added by Zhou Shouya, 2013/8/7   PN:VLAN */
                        /* vlan id:1-4094 */
                        if ( pstData[ulLoopNo].ulVlanId < 1 || pstData[ulLoopNo].ulVlanId > 4094 )
                        {
                            agent_printf(LOG_ERR, "ulVlanId:%u invalid.\n", pstData[ulLoopNo].ulVlanId);
                            return FALSE;
                        }
                        /* END:   Added by Zhou Shouya, 2013/8/7   PN:VLAN */
                        sprintf(acNvramName, NVRAM_VAP"%d_port_pvid", pstData[ulLoopNo].ulNvramId);
                        sprintf(acNvramValue, "%d", pstData[ulLoopNo].ulVlanId);
                        nvram_set(acNvramName, acNvramValue);
                        sprintf(acNvramName, NVRAM_VAP"%d_port_vlanEabled", pstData[ulLoopNo].ulNvramId);
                        nvram_get(acNvramValue, acNvramName, 32);
                        //nvram_set(acNvramName, "1");
                        if(1 == atoi(acNvramValue))
                        {
                            acVlanId = bridge_id_get(pstData[ulLoopNo].ulVlanId);
                            acVlanIdOld = bridge_id_get(pstConfig[ulLoopNo2].ulVlanId);
                            app_vlan_modify(acVlanId, acVlanIdOld, acVapName);
                        }
                    }

                    if(pstData[ulLoopNo].ulVlanEnabled != pstConfig[ulLoopNo2].ulVlanEnabled)
                    {
                        /* BEGIN: Added by Zhou Shouya, 2013/8/7   PN:VLAN */
                        /* ssid所在vlan是否启用:off(0), on(1) */
                        if ( pstData[ulLoopNo].ulVlanEnabled > 1 )
                        {
                            agent_printf(LOG_ERR, "ulVlanEnabled:%u invalid.\n", pstData[ulLoopNo].ulVlanEnabled);
                            return FALSE;
                        }
                        /* END:   Added by Zhou Shouya, 2013/8/7   PN:VLAN */
                        sprintf(acNvramName, NVRAM_VAP"%d_port_vlanEabled", pstData[ulLoopNo].ulNvramId);
                        sprintf(acNvramValue, "%d", pstData[ulLoopNo].ulVlanEnabled);
                        nvram_set(acNvramName, acNvramValue);
                        acVlanId = bridge_id_get(pstData[ulLoopNo].ulVlanId);
                        app_vlan_eabled(acVlanId, pstData[ulLoopNo].ulVlanEnabled, acVapName);
                    }
                    /* END:   Added by Zhou Shouya, 2013/7/24 */
                    
                    break;
                }
            }
            if ( ulLoopNo == ulTableMaxRecNum )
            {
                /*不允许删除ssid*/
                lprintf(LOG_ERR, "no deleting record.");
                return FALSE;
            }
        }
    }
    else
    {
        /*不允许新建ssid*/
        lprintf(LOG_ERR, "no adding record.");
        return FALSE;
    }
    
    nvram_commit();

    if ( FALSE == ac_agent_get_apvapbasicsetup_config() )
    {
        lprintf(LOG_ERR, "ac_agent_get_apvapbasicsetup_config failed.");
        return FALSE;
    }

    return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL ac_agent_modify_ap_config_vapsecurity(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_VAPSECURITY_ENTRY_T *pstData      = (AP_CONFIG_VAPSECURITY_ENTRY_T *)pData;
    AP_CONFIG_VAPSECURITY_ENTRY_T *pstConfig    = g_pstSnmpAgentData->pstApVapSecurityConfig;
    UINT32  ulLoopNo = 0;
    UINT32  ulLoopNo2 = 0;
    UINT32  ulTableMaxRecNum = VAP_MAX_NUM;
    UINT32  ulRecNum = g_pstSnmpAgentData->stApVapConfig.ulVapNum;
	CHAR    acDevName[32] = "";

    if ( NULL == pOriData )
    {
        /*修改或删除记录*/
        for ( ulLoopNo2 = 0 ; ulLoopNo2 <ulRecNum; ulLoopNo2++ )
        {
            for ( ulLoopNo = 0 ; ulLoopNo <ulTableMaxRecNum; ulLoopNo++ )
            {
				if (pstData[ulLoopNo].ulIndex== 0)
				{
					continue;
				}			

    			if (pstData[ulLoopNo].ulRowStatus == ENUM_ROWSTATUS_DESTROY)
    			{
    				continue;
    			}

    			if (pstData[ulLoopNo].ulRowStatus == ENUM_ROWSTATUS_CREATEANDWAIT)
    			{
    				continue;
    			}

                if ( pstData[ulLoopNo].ulIndex == pstConfig[ulLoopNo2].ulIndex)
                {
                    if ( 0 != memcmp(&pstData[ulLoopNo].ulAuthenticationType,
                        &pstConfig[ulLoopNo2].ulAuthenticationType,
                        sizeof(AP_CONFIG_VAPSECURITY_ENTRY_T)-32-3*sizeof(UINT32)))
                    {
                    	sprintf(acDevName,"ap%d_%d", pstData[ulLoopNo].ulWifiId, 
                            pstData[ulLoopNo].ulVapId);
                        if ( FALSE == agent_modify_vap_security(&pstData[ulLoopNo], &pstConfig[ulLoopNo2], acDevName) )
                        {
                            agent_printf(LOG_ERR, "agent_modify_vap_security failed.");
                            return FALSE;
                        }
                    }
                    break;
                }
            }
            if ( ulLoopNo == ulTableMaxRecNum )
            {
                /*不允许删除ssid*/
                lprintf(LOG_ERR, "no deleting record.");
                return FALSE;
            }
        }
    }
    else
    {
        /*不允许新建ssid*/
        lprintf(LOG_ERR, "no adding record.");
        return FALSE;
    }

    if ( FALSE == ac_agent_get_apvapsecuritysetup_config() )
    {
        lprintf(LOG_ERR, "ac_agent_get_apvapsecuritysetup_config failed.");
        return FALSE;
    }

    return TRUE;
}
#endif
#endif

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL ac_agent_modify_ap_config_alarm(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_ALARM_T *pstData      = (AP_CONFIG_ALARM_T *)pData;
    AP_CONFIG_ALARM_T *pstOriData   = (AP_CONFIG_ALARM_T *)pOriData;
    CHAR acNvramValue[NVRAM_SIZE]={0};
    
    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
    }

    if ( NULL == pOriData )
    {
        lprintf(LOG_ERR, "pOriData is null.");
    }

    /* 合法性检查 */
    if ( pstData->ulCpuLoadHigh < pstData->ulCpuLoadLow )
    {
        agent_printf(LOG_ERR, "ulCpuLoadHigh:%u is less than ulCpuLoadLow:%u.\n",
            pstData->ulCpuLoadHigh, pstData->ulCpuLoadLow);
        return FALSE;
    }
    if ( pstData->ulMemLoadHigh < pstData->ulMemLoadLow )
    {
        agent_printf(LOG_ERR, "ulMemLoadHigh:%u is less than ulMemLoadLow:%u.\n",
            pstData->ulMemLoadHigh, pstData->ulMemLoadLow);
        return FALSE;
    }

    if ( pstData->ulCpuLoadHigh != pstOriData->ulCpuLoadHigh )
    {
        if ( pstData->ulCpuLoadHigh > 100 )
        {
            agent_printf(LOG_ERR, "ulCpuLoadHigh:%u invalid.\n", pstData->ulCpuLoadHigh);
            return FALSE;
        }
        sprintf(acNvramValue, "%u", pstData->ulCpuLoadHigh);
        nvram_set(NVRAM_ALARMCPULOADHIGH, acNvramValue);
    }
    if ( pstData->ulCpuLoadLow != pstOriData->ulCpuLoadLow )
    {
        if ( pstData->ulCpuLoadLow > 100 )
        {
            agent_printf(LOG_ERR, "ulCpuLoadLow:%u invalid.\n", pstData->ulCpuLoadLow);
            return FALSE;
        }
        sprintf(acNvramValue, "%u", pstData->ulCpuLoadLow);
        nvram_set(NVRAM_ALARMCPULOADLOW, acNvramValue);
    }
    if ( pstData->ulMemLoadHigh != pstOriData->ulMemLoadHigh )
    {
        if ( pstData->ulMemLoadHigh > 100 )
        {
            agent_printf(LOG_ERR, "ulMemLoadHigh:%u invalid.\n", pstData->ulMemLoadHigh);
            return FALSE;
        }
        sprintf(acNvramValue, "%u", pstData->ulMemLoadHigh);
        nvram_set(NVRAM_ALARMMEMLOADHIGH, acNvramValue);
    }
    if ( pstData->ulMemLoadLow != pstOriData->ulMemLoadLow )
    {
        if ( pstData->ulMemLoadLow > 100 )
        {
            agent_printf(LOG_ERR, "ulMemLoadLow:%u invalid.\n", pstData->ulMemLoadLow);
            return FALSE;
        }
        sprintf(acNvramValue, "%u", pstData->ulMemLoadLow);
        nvram_set(NVRAM_ALARMMEMLOADLOW, acNvramValue);
    }
    nvram_commit();

    if ( FALSE == ac_agent_get_apalarmsetup_config() )
    {
        lprintf(LOG_ERR, "ac_agent_get_apalarmsetup_config failed.");
        return FALSE;
    }

    return TRUE;
}


BOOL ac_agent_modify_ap_config_trapdes(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_TRAPDES_ENTRY_T *pstData      = (AP_CONFIG_TRAPDES_ENTRY_T *)pData;
    AP_CONFIG_TRAPDES_ENTRY_T *pstOriData   = (AP_CONFIG_TRAPDES_ENTRY_T *)pOriData;
    CHAR   acNvramValue[NVRAM_SIZE] = {0};
    
	if (g_pstSnmpAgentData == NULL)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}
	
    if(pstData->ulSwtich != pstOriData->ulSwtich)
    {
        if ( pstData->ulSwtich > 2 )
        {
            agent_printf(LOG_ERR, "ulSwtich:%u invalid.\n", pstData->ulSwtich);
            return FALSE;
        }
        sprintf(acNvramValue, "%d", pstData->ulSwtich);
        nvram_set(NVRAM_TRAPDSWTICH, acNvramValue);
    }

    if(pstData->ulTrapDesPort != pstOriData->ulTrapDesPort)
    {
        sprintf(acNvramValue, "%d", pstData->ulTrapDesPort);
        nvram_set(NVRAM_TRAPDESPORT, acNvramValue);
    }

    if(0 != memcmp(pstData->aucTrapDesIPAddress, pstOriData->aucTrapDesIPAddress, 4))
    {
        sprintf(acNvramValue, IP_FMT, IP_ARG(pstData->aucTrapDesIPAddress));
        nvram_set(NVRAM_TRAPDESIP, acNvramValue);
    }

    if(0 != memcmp(pstData->acTrapCommunity, pstOriData->acTrapCommunity, strlen(pstData->acTrapCommunity)))
    {
        nvram_set(NVRAM_TRAPCOMMUITY, pstData->acTrapCommunity);
    }
    nvram_commit();

    if ( FALSE == ac_agent_get_aptrapdessetup_config() )
    {
        lprintf(LOG_ERR, "ac_agent_get_aptrapdessetup_config failed.");
        return FALSE;
    }
    
	return TRUE;
}

#ifndef GW_X86
BOOL ac_agent_modify_ap_config_vlan(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_VLAN_ENTRY_T *pstData      = (AP_CONFIG_VLAN_ENTRY_T *)pData;
    AP_CONFIG_VLAN_ENTRY_T *pstConfig    = g_pstSnmpAgentData->pstApVlanConfig;
    UINT32 ulLoopNo = 0;
    UINT32 ulLoopNo2 = 0;
    UINT32 ulTableMaxRecNum = TABLE_MAXRECNUM_VLAN;
    UINT32 ulRecNum = g_pstSnmpAgentData->stApVlanConfig.ulVlanNum;
    
	if (g_pstSnmpAgentData == NULL)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}
    
    if ( NULL == pOriData )
    {
        /*修改或删除记录*/
        for ( ulLoopNo2 = 0 ; ulLoopNo2 <ulRecNum; ulLoopNo2++ )
        {
            for ( ulLoopNo = 0 ; ulLoopNo <ulTableMaxRecNum; ulLoopNo++ )
            {
				if (pstData[ulLoopNo].ulIndex== 0)
				{
					continue;
				}			

    			if (pstData[ulLoopNo].ulRowStatus == ENUM_ROWSTATUS_DESTROY)
    			{
    				continue;
    			}

    			if (pstData[ulLoopNo].ulRowStatus == ENUM_ROWSTATUS_CREATEANDWAIT)
    			{
    				continue;
    			}
                if ( pstData[ulLoopNo].ulIndex == pstConfig[ulLoopNo2].ulIndex)
                {
                    if ( 0 != strcmp(pstData[ulLoopNo].acVlanName, pstConfig[ulLoopNo2].acVlanName)
                        || pstData[ulLoopNo].ulVlanPrio != pstConfig[ulLoopNo2].ulVlanPrio )
                    {
                        if ( pstData[ulLoopNo].ulVlanPrio > 8 )
                        {
                            agent_printf(LOG_ERR, "ulVlanPrio:%u invalid.\n", pstData[ulLoopNo].ulVlanPrio);
                            return FALSE;
                        }
                        app_set_vlaninfo(&pstData[ulLoopNo], 1);
                    }
                    break;
                }                 
            }
            if ( ulLoopNo == ulTableMaxRecNum )            
            {
                app_set_vlaninfo(&pstConfig[ulLoopNo2], 0);
                break;
            }
        }
    }
    else
    {
        for ( ulLoopNo2 = 0 ; ulLoopNo2 < ulRecNum; ulLoopNo2++ )
        {
            if ( pstData->ulIndex == pstConfig[ulLoopNo2].ulIndex)
            {
                lprintf(LOG_ERR, "adding record index is duplicate");
                return FALSE;
            }
            if ( pstData->ulVlanId > 4094 || pstData->ulVlanPrio > 8 )
            {
                agent_printf(LOG_ERR, "ulVlanId:%u or ulVlanPrio:%u invalid.\n",
                    pstData->ulVlanId, pstData->ulVlanPrio);
                return FALSE;
            }
            app_set_vlaninfo(pstData, 2);
            break;
        }
    }

    if ( FALSE == ac_agent_get_apvlansetup_config() )
    {
        lprintf(LOG_ERR, "ac_agent_get_apvlansetup_config failed.");
        return FALSE;
    }
    
	return TRUE;
}

BOOL ac_agent_modify_ap_config_ssid(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_SSID_ENTRY_T *pstData      = (AP_CONFIG_SSID_ENTRY_T *)pData;
    AP_CONFIG_SSID_ENTRY_T *pstConfig    = g_pstSnmpAgentData->pstApSsidConfig;
    UINT32 ulLoopNo = 0;
    UINT32 ulLoopNo2 = 0;
    UINT32 ulTableMaxRecNum = TABLE_MAXRECNUM_AP_VAP;
    UINT32 ulRecNum = g_pstSnmpAgentData->stApSsidConfig.ulNum;

	if (g_pstSnmpAgentData == NULL)
	{
		agent_printf(LOG_ERR, "%s: g_pstSnmpAgentData is null\n", SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}

	agent_printf(LOG_ERR, "into this %s function", __FUNCTION__);
    
    if ( NULL == pOriData )
    {
        /*修改或删除记录*/
        for ( ulLoopNo2 = 0 ; ulLoopNo2 <ulRecNum; ulLoopNo2++ )
        {
            for ( ulLoopNo = 0 ; ulLoopNo <ulTableMaxRecNum; ulLoopNo++ )
            {
				if (pstData[ulLoopNo].ulIndex== 0)
				{
					continue;
				}			

    			if (pstData[ulLoopNo].ulRowStatus == ENUM_ROWSTATUS_DESTROY)
    			{
    				continue;
    			}

    			if (pstData[ulLoopNo].ulRowStatus == ENUM_ROWSTATUS_CREATEANDWAIT)
    			{
    				continue;
    			}
                if ( pstData[ulLoopNo].ulIndex == pstConfig[ulLoopNo2].ulIndex)
                {
                    break;
                }                 
            }
            if ( ulLoopNo == ulTableMaxRecNum )            
            {
                wlan_del_vap(&pstConfig[ulLoopNo2]);
                break;
            }
        }
    }
    else
    {
        for ( ulLoopNo2 = 0 ; ulLoopNo2 < ulRecNum; ulLoopNo2++ )
        {
            if ( pstData->ulIndex == pstConfig[ulLoopNo2].ulIndex)
            {
                lprintf(LOG_ERR, "adding record index is duplicate");
                return FALSE;
            }
        }

        if ( pstData->ulRowStatus == ENUM_ROWSTATUS_ACTIVE ||
		     pstData->ulRowStatus == ENUM_ROWSTATUS_CREATEANDGO ||
		     pstData->ulRowStatus == ENUM_ROWSTATUS_CREATEANDWAIT )
        {
            //create ssid;
            /* BEGIN: Modified by Zhou Shouya, 2013/9/6 */
            if ( FALSE == wlan_add_vap(pstData) )
            {
                agent_printf(LOG_ERR, "add vap fail.\n");
                return FALSE;
            }
            /* END:   Modified by Zhou Shouya, 2013/9/6 */
        }
        else
        {
            agent_printf(LOG_ERR, "Func:%s Line:%d ulRowStatus:%d invalid.\n",
                __func__, __line__, pstData->ulRowStatus);
            return FALSE;
        }
    }

    if ( FALSE == ac_agent_get_apssid_setup() )
    {
        lprintf(LOG_ERR, "ac_agent_get_apssid_setup failed.");
        return FALSE;
    }
    
	return TRUE;
}
#endif
#if 0
BOOL agent_modify_ap_flowctl(AP_CONFIG_FLOWCTL_T * pstApFlowCtl, AP_CONFIG_FLOWCTL_T *pstApFlowCtlOri)
{
    char acCtorl[32];
    char acUpload[32];
    char acDownload[32];
    char acStaUpload[32];
    char acStaDownload[32];

    if ( pstApFlowCtl->ulFlowSwitch != pstApFlowCtlOri->ulFlowSwitch )
    {
        /* BEGIN: Added by Zhou Shouya, 2013/8/7   PN:VLAN */
        /* 流控开关:off(0),on(1) */
        if ( pstApFlowCtl->ulFlowSwitch > 1 )
        {
            agent_printf(LOG_ERR, "ulFlowSwitch:%u invalid.\n", pstApFlowCtl->ulFlowSwitch);
            return FALSE;
        }
        /* END:   Added by Zhou Shouya, 2013/8/7   PN:VLAN */
        sprintf(acCtorl, "%d", pstApFlowCtl->ulFlowSwitch);
        nvram_set("ap_flow_switch", acCtorl);
    }
    if ( pstApFlowCtl->ulApUplinkMaxRate != pstApFlowCtlOri->ulApUplinkMaxRate )
    {
        if ( pstApFlowCtl->ulApUplinkMaxRate < 0 || pstApFlowCtl->ulApUplinkMaxRate > 1000000 )
        {
            agent_printf(LOG_ERR, "ulApUplinkMaxRate:%u invalid.\n", pstApFlowCtl->ulApUplinkMaxRate);
            return FALSE;
        }
        sprintf(acUpload, "%d", pstApFlowCtl->ulApUplinkMaxRate);
        nvram_set("ap_upload", acUpload);
    }
    if ( pstApFlowCtl->ulApDwlinkMaxRate != pstApFlowCtlOri->ulApDwlinkMaxRate )
    {
        if ( pstApFlowCtl->ulApDwlinkMaxRate < 0 || pstApFlowCtl->ulApDwlinkMaxRate > 1000000 )
        {
            agent_printf(LOG_ERR, "ulApDwlinkMaxRate:%u invalid.\n", pstApFlowCtl->ulApDwlinkMaxRate);
            return FALSE;
        }
        sprintf(acDownload, "%d", pstApFlowCtl->ulApDwlinkMaxRate);
        nvram_set("ap_download", acDownload);
    }
    if ( pstApFlowCtl->ulStaUplinkMaxRate != pstApFlowCtlOri->ulStaUplinkMaxRate )
    {
        if ( pstApFlowCtl->ulStaUplinkMaxRate < 0 || pstApFlowCtl->ulStaUplinkMaxRate > 1000000 )
        {
            agent_printf(LOG_ERR, "ulStaUplinkMaxRate:%u invalid.\n", pstApFlowCtl->ulStaUplinkMaxRate);
            return FALSE;
        }
        sprintf(acStaUpload, "%d", pstApFlowCtl->ulStaUplinkMaxRate);
        nvram_set("ap_sta_upload", acStaUpload);
            agent_printf(LOG_ERR, "test fail.\n");
    }
    if ( pstApFlowCtl->ulStaDwlinkMaxRate != pstApFlowCtlOri->ulStaDwlinkMaxRate )
    {
        if ( pstApFlowCtl->ulStaDwlinkMaxRate < 0 || pstApFlowCtl->ulStaDwlinkMaxRate > 1000000 )
        {
            agent_printf(LOG_ERR, "ulStaDwlinkMaxRate:%u invalid.\n", pstApFlowCtl->ulStaDwlinkMaxRate);
            return FALSE;
        }
        sprintf(acStaDownload, "%d", pstApFlowCtl->ulStaDwlinkMaxRate);
        nvram_set("ap_sta_download", acStaDownload);
            agent_printf(LOG_ERR, "test2 fail.\n");
    }
    if( 1 == pstApFlowCtl->ulFlowSwitch
        &&(pstApFlowCtl->ulFlowSwitch != pstApFlowCtlOri->ulFlowSwitch
        || pstApFlowCtl->ulStaUplinkMaxRate != pstApFlowCtlOri->ulStaUplinkMaxRate
        || pstApFlowCtl->ulStaDwlinkMaxRate != pstApFlowCtlOri->ulStaDwlinkMaxRate)
     )
    {
        if( 0 != flow_synchro_vap() )
        {
            agent_printf(LOG_ERR, "flow_synchro_vap fail.\n");
            return FALSE;
        }
        if ( 0 != ap_sta_flow_write(pstApFlowCtl->ulFlowSwitch,
            pstApFlowCtl->ulStaUplinkMaxRate, pstApFlowCtl->ulStaDwlinkMaxRate) )
        {
            agent_printf(LOG_ERR, "ap_sta_flow_write fail.\n");
            return FALSE;
        }
    }
    
    if ( pstApFlowCtl->ulFlowSwitch != pstApFlowCtlOri->ulFlowSwitch
        || pstApFlowCtl->ulApUplinkMaxRate != pstApFlowCtlOri->ulApUplinkMaxRate
        || pstApFlowCtl->ulApDwlinkMaxRate != pstApFlowCtlOri->ulApDwlinkMaxRate )
    {
        if ( 0 != config_ap_flow(pstApFlowCtl->ulFlowSwitch,
            pstApFlowCtl->ulApUplinkMaxRate, pstApFlowCtl->ulApDwlinkMaxRate) )
        {
            agent_printf(LOG_ERR, "config_ap_flow fail.\n");
            return FALSE;
        }
    }

    nvram_commit();
#if 0
    set_ap_flowcontrol(pstApFlowCtl->ulFlowSwitch,
        pstApFlowCtl->ulApUplinkMaxRate,
        pstApFlowCtl->ulApDwlinkMaxRate);
#endif
    return TRUE;
}
#endif
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL snmp_check_valid_ap_config_sys_devinfo(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_SYS_DEVINFO_T *pstData    = (AP_CONFIG_SYS_DEVINFO_T *)pData;
    AP_CONFIG_SYS_DEVINFO_T *pstOriData = (AP_CONFIG_SYS_DEVINFO_T *)pOriData;
    BOOL bRet = FALSE;

    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
    }

    if ( NULL == pOriData )
    {
        lprintf(LOG_ERR, "pOriData is null.");
    }

    if ( 
        /* BEGIN: Added by zoucaihong, 2012/12/25 */
        pstData->ulRegetPeriod != pstOriData->ulRegetPeriod
        /* END: Added by zoucaihong, 2012/12/25 */
        /* BEGIN: Added by zoucaihong, 2013/4/24 */
        || pstData->ulTrapSwitch != pstOriData->ulTrapSwitch
        /* END: Added by zoucaihong, 2013/4/24 */
        /* BEGIN: Added by zoucaihong, 2014/5/14 */
        || pstData->ulSysManageState != pstOriData->ulSysManageState
        /* END: Added by zoucaihong, 2014/5/14 */
        || pstData->ulTimeOutInterval != pstOriData->ulTimeOutInterval
        || pstData->ulHeartBeatPeriod != pstOriData->ulHeartBeatPeriod
        )
    {
        bRet = ac_agent_modify_ap_config_sys_devinfo(pData, pOriData);
        if ( FALSE == bRet )
        {
            lprintf(LOG_ERR, "ac_agent_modify_ap_config_sys_devinfo failed.");
            return FALSE;
        }
        lprintf(LOG_INFO, "ac_agent_modify_ap_config_sys_devinfo succeed.");
    }
    
    /* 重启AP */
    if ( AP_SYSOPER_SET == pstData->ulSysRestart )
    {
        char acStrCmd[256];
        memset(acStrCmd, 0, sizeof(acStrCmd));

        sprintf(acStrCmd, "reboot 5 snmp restart ap");
        system(acStrCmd);
    }
    
	return TRUE;
}
/*****************************************************************************
 Prototype    : snmp_check_valid_ap_config_ip
 Description  : 修改IP地址配置合法性检查函数
 Input        : VOID *pData     
                VOID *pOriData  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/2/26
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL snmp_check_valid_ap_config_ip(VOID *pData, VOID *pOriData)
{
    BOOL bRet = FALSE;

    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
        return FALSE;
    }

    bRet = ac_agent_modify_ap_config_ip(pData, pOriData);
    if ( FALSE == bRet )
    {
        lprintf(LOG_ERR, "ac_agent_modify_ap_config_ip failed.");
        return FALSE;
    }
    lprintf(LOG_INFO, "ac_agent_modify_ap_config_ip succeed.");

	return TRUE;
}
#if 0
/* BEGIN: Added by zoucaihong, 2013/5/13 */
BOOL snmp_check_valid_ap_config_legalap(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_LEGALAP_ENTRY_T *pstData      = (AP_CONFIG_LEGALAP_ENTRY_T *)pData;
    AP_CONFIG_LEGALAP_ENTRY_T *pstOriData   = (AP_CONFIG_LEGALAP_ENTRY_T *)pOriData;
    BOOL bRet = FALSE;
    UINT32 ulLoopNo = 0;
    UINT32 ulLoopNo2 = 0;
    UINT32 ulRecSize = 0;
    UINT32 ulTableMaxRecNum = TABLE_MAXRECNUM_LEGALAP;

    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
        return FALSE;
    }

    if ( NULL == pOriData )    
    {
        /*修改或删除记录*/
        for ( ulLoopNo = 0 ; ulLoopNo < ulTableMaxRecNum ; ulLoopNo++ )
        {
            if ( 0 == pstData[ulLoopNo].ulIndex )
            {
                continue;
            }
            for ( ulLoopNo2 = ulLoopNo+1 ; ulLoopNo2 < ulTableMaxRecNum ; ulLoopNo2++ )
            {
                if ( 0 == pstData[ulLoopNo2].ulIndex )
                {
                    continue;
                }
                if ( pstData[ulLoopNo].ulIndex == pstData[ulLoopNo2].ulIndex )
                {
                    lprintf(LOG_ERR, "modify or delete index is duplicated.");
                    return FALSE;
                }
            }
        }
    }
    else
    {
        /*增加记录*/
        if ( 0 == pstData->ulIndex )
        {
            lprintf(LOG_ERR, "add record index is 0.");
            return FALSE;
        }

        for ( ulLoopNo = 0 ; ulLoopNo < ulTableMaxRecNum ; ulLoopNo++ )
        {
            if ( 0 == pstOriData[ulLoopNo].ulIndex )
            {
                break;
            }
            if ( pstData->ulIndex == pstOriData[ulLoopNo].ulIndex )
            {
                lprintf( LOG_ERR, "add record index is duplicated.");
                return FALSE;
            }
            ulRecSize ++;
        }
        if ( ulRecSize >= ulTableMaxRecNum )
        {
            lprintf(LOG_ERR, "add record has been max table record num.");
            return FALSE;
        }
    }

    bRet = ac_agent_modify_ap_config_legalap(pData, pOriData);
    if ( FALSE == bRet )
    {
        lprintf(LOG_ERR, "ac_agent_modify_ap_config_legalap failed.");
        return FALSE;
    }
    lprintf(LOG_INFO, "ac_agent_modify_ap_config_legalap succeed.");

	return TRUE;
}

/*****************************************************************************
 Prototype    : snmp_check_valid_ap_config_flowctl
 Description  : 修改AP级流控配置
 Input        : VOID *pData     
                VOID *pOriData  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/5/13
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL snmp_check_valid_ap_config_flowctl(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_FLOWCTL_T *pstData    = (AP_CONFIG_FLOWCTL_T *)pData;
    AP_CONFIG_FLOWCTL_T *pstOriData = (AP_CONFIG_FLOWCTL_T *)pOriData;

    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
    }

    if ( NULL == pOriData )
    {
        lprintf(LOG_ERR, "pOriData is null.");
    }

    if ( 0 != memcmp(&pstData->ulFlowSwitch, &pstOriData->ulFlowSwitch, 5*sizeof(UINT32)) )
    {
        if ( FALSE == agent_modify_ap_flowctl(pstData, pstOriData) )
        {
            agent_printf(LOG_ERR, "agent_modify_ap_flowctl fail.");
            return FALSE;
        }
    }
    
	return TRUE;
}
/* END: Added by zoucaihong, 2013/5/13 */

/*****************************************************************************
 Prototype    : snmp_check_valid_ap_config_advance_5gprior
 Description  : AP 5G优先配置合法性检查函数
 Input        : VOID *pData     
                VOID *pOriData  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/3/11
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL snmp_check_valid_ap_config_advance_5gprior(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_ADVANCE_5GPRIOR_T *pstData    = (AP_CONFIG_ADVANCE_5GPRIOR_T *)pData;
    AP_CONFIG_ADVANCE_5GPRIOR_T *pstOriData = (AP_CONFIG_ADVANCE_5GPRIOR_T *)pOriData;
    BOOL bRet = FALSE;

    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
    }

    if ( NULL == pOriData )
    {
        lprintf(LOG_ERR, "pOriData is null.");
    }

    if ( 0!= memcmp(&pstData->ul5GPriSwitch, &pstOriData->ul5GPriSwitch, 
        sizeof(AP_CONFIG_ADVANCE_5GPRIOR_T) ))
    {
        bRet = ac_agent_modify_ap_config_advance_5gprior(pData, pOriData);
        if ( FALSE == bRet )
        {
            lprintf(LOG_ERR, "ac_agent_modify_ap_config_advance_5gprior failed.");
        }
        lprintf(LOG_INFO, "ac_agent_modify_ap_config_advance_5gprior succeed.");
    }
    
	return TRUE;
}

/*****************************************************************************
 Prototype    : snmp_check_valid_ap_config_advance_11nprior
 Description  : AP 11n优先配置合法性检查函数
 Input        : VOID *pData     
                VOID *pOriData  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/3/11
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL snmp_check_valid_ap_config_advance_11nprior(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_ADVANCE_11NPRIOR_T *pstData    = (AP_CONFIG_ADVANCE_11NPRIOR_T *)pData;
    AP_CONFIG_ADVANCE_11NPRIOR_T *pstOriData = (AP_CONFIG_ADVANCE_11NPRIOR_T *)pOriData;
    BOOL bRet = FALSE;

    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
    }

    if ( NULL == pOriData )
    {
        lprintf(LOG_ERR, "pOriData is null.");
    }

    if ( 0!= memcmp(&pstData->ul11nPriSwitch, &pstOriData->ul11nPriSwitch, 
        sizeof(AP_CONFIG_ADVANCE_11NPRIOR_T) ))
    {
        bRet = ac_agent_modify_ap_config_advance_11nprior(pData, pOriData);
        if ( FALSE == bRet )
        {
            lprintf(LOG_ERR, "ac_agent_modify_ap_config_advance_11nprior failed.");
        }
        lprintf(LOG_INFO, "ac_agent_modify_ap_config_advance_11nprior succeed.");
    }
    
	return TRUE;
}
#endif
#ifndef GW_X86

/*****************************************************************************
 Prototype    : snmp_check_valid_ap_config_wifidevicechannel
 Description  : 修改卡的信道配置
 Input        : VOID *pData     
                VOID *pOriData  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/3/25
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL snmp_check_valid_ap_config_wifidevicechannel(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_WIFIDEVICECHANNEL_T *pstData    = (AP_CONFIG_WIFIDEVICECHANNEL_T *)pData;
    AP_CONFIG_WIFIDEVICECHANNEL_T *pstOriData = (AP_CONFIG_WIFIDEVICECHANNEL_T *)pOriData;

    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
    }

    if ( NULL == pOriData )
    {
        lprintf(LOG_ERR, "pOriData is null.");
    }

    if(FALSE == wifi_device_channel_config(pstData, pstOriData))
    {
        return FALSE;
    }
    lprintf(LOG_INFO, "wifi_device_channel_config succeed.");
	
    if ( 0 == pstData->ulWifiId
        && FALSE == ac_agent_get_apwifidevicechannel_config() )
    {
        return FALSE;
    }
    else if ( 1 == pstData->ulWifiId
        && FALSE == ac_agent_get_apwifidevicechannel5_config() )
    {
        return FALSE;
    }
    
	return TRUE;
}
/*****************************************************************************
 Prototype    : snmp_check_valid_ap_config_wifidevicepower
 Description  : 修改卡的功率配置
 Input        : VOID *pData     
                VOID *pOriData  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/3/21
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL snmp_check_valid_ap_config_wifidevicepower(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_WIFIDEVICEPOWER_T *pstData    = (AP_CONFIG_WIFIDEVICEPOWER_T *)pData;
    AP_CONFIG_WIFIDEVICEPOWER_T *pstOriData = (AP_CONFIG_WIFIDEVICEPOWER_T *)pOriData;
    char nvram_name[32];
    char nvram_value[32];
    char vap_name[32];

    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
    }

    if ( NULL == pOriData )
    {
        lprintf(LOG_ERR, "pOriData is null.");
    }

	if (pstData->ulMaxTxPower != pstOriData->ulMaxTxPower)
	{
	    /* BEGIN: Added by Zhou Shouya, 2013/8/1   PN:VLAN */
	    /* 功率衰减最大值为10dBm */
	    if ( 10 < pstData->ulMaxTxPower )
	    {
	        agent_printf(LOG_ERR, "File:%s Line:%d ulMaxTxPower:%u is more than 10dBm.\n",
	            __file__, __line__, pstData->ulMaxTxPower);
	        return FALSE;
	    }
	    /* END:   Added by Zhou Shouya, 2013/8/1   PN:VLAN */
        sprintf(nvram_name, NVRAM_WIFI"%d_current_txpower", pstData->ulWifiId);
        sprintf(nvram_value, "%d", pstData->ulMaxTxPower);
        nvram_set(nvram_name, nvram_value);

        get_vap_name_for_wifi_id(vap_name, pstData->ulWifiId);
        wlan_set_txpower(vap_name, pstData->ulLimitTxPower-pstData->ulMaxTxPower);
        nvram_commit();
	}

    if ( 0 == pstData->ulWifiId
        && FALSE == ac_agent_get_apwifidevicepower_config() )
    {
        return FALSE;
    }
    else if ( 1 == pstData->ulWifiId
        && FALSE == ac_agent_get_apwifidevicepower5_config() )
    {
        return FALSE;
    }

	return TRUE;
}
/*****************************************************************************
 Prototype    : snmp_check_valid_ap_config_wifideviceadvance
 Description  : 修改卡高级配置
 Input        : VOID *pData     
                VOID *pOriData  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/3/25
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL snmp_check_valid_ap_config_wifideviceadvance(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_WIFIDEVICEADVANCE_T *pstData    = (AP_CONFIG_WIFIDEVICEADVANCE_T *)pData;
    AP_CONFIG_WIFIDEVICEADVANCE_T *pstOriData = (AP_CONFIG_WIFIDEVICEADVANCE_T *)pOriData;
    CHAR   acNvramName[32] = {0};
    CHAR   acNvramValue[32] = {0};

    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
    }

    if ( NULL == pOriData )
    {
        lprintf(LOG_ERR, "pOriData is null.");
    }

    if ( pstData->ulRssiMode != pstOriData->ulRssiMode )
    {
        sprintf(acNvramName, "wifi_%u_rssi_mode", pstData->ulWifiId);
        sprintf(acNvramValue, "%u", pstData->ulRssiMode);
        nvram_set(acNvramName, acNvramValue);
    }

    if ( pstData->iRssi != pstOriData->iRssi )
    {
        /* 动态模式不允许配置rssi门限 */
        if ( 1 == pstData->ulRssiMode )
        {
            agent_printf(LOG_ERR, "rssi mode is dynamic, can't modify rssi threshold.\n");
            return FALSE;
        }
        /* rssi取值范围:0~-95dBm */
        if ( (-95>pstData->iRssi) || (0 <pstData->iRssi)  )
        {
            agent_printf(LOG_ERR, "iRssi:%d is invalid.\n", pstData->iRssi);
            return FALSE;
        }
        sprintf(acNvramName, "wifi_%u_rssi_threshold", pstData->ulWifiId);
        sprintf(acNvramValue, "%d", pstData->iRssi);
        nvram_set(acNvramName, acNvramValue);
        if ( 0 != wlan_set_rssi_static_threshold_all(pstData->ulWifiId, 95+pstData->iRssi) )
        {
            agent_printf(LOG_ERR, "wifi_rssi_set failed.");
            return FALSE;
        }
    }

    nvram_commit();

    if ( 0 == pstData->ulWifiId
        && FALSE == ac_agent_get_apwifideviceadvance_config() )
    {
		agent_printf(LOG_ERR, "ac_agent_get_apwifideviceadvance_config fail.");
        return FALSE;
    }
    else if ( 1 == pstData->ulWifiId
        && FALSE == ac_agent_get_apwifideviceadvance5_config() )
    {
		agent_printf(LOG_ERR, "ac_agent_get_apwifideviceadvance5_config fail.");
        return FALSE;
    }
    
	return TRUE;
}
/*****************************************************************************
 Prototype    : snmp_check_valid_ap_config_wifidevicerate
 Description  : 修改卡级速率配置
 Input        : VOID *pData     
                VOID *pOriData  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/8/29
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
BOOL snmp_check_valid_ap_config_wifidevicerate(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_WIFIDEVICERATE_T *pstData    = (AP_CONFIG_WIFIDEVICERATE_T *)pData;
    AP_CONFIG_WIFIDEVICERATE_T *pstOriData = (AP_CONFIG_WIFIDEVICERATE_T *)pOriData;
    char nvram_name[32];
    char nvram_value[32];
    char vap_name[32];
    char list[128];
    char vap_list[32];
    char rate_basic[256];
    char rate_11n[256];
    char rate_list[256];
    int total;
    int loop;
    int wifi_id;
    BOOL bFlagBasic = FALSE;
    BOOL bFlag11n = FALSE;
    BOOL flag = 0;
    
    if ( NULL == pstData )
    {
        lprintf(LOG_ERR, "pstData is null.");
        return FALSE;
    }

    if ( NULL == pstOriData )
    {
        lprintf(LOG_ERR, "pstOriData is null.");
        return FALSE;
    }

    wifi_id = pstData->ulWifiId;

    /* BEGIN: Added by Zhou Shouya, 2013/9/17 */
    /* 速率模式变更 */
    if ( pstData->ulRateMode != pstOriData->ulRateMode )
    {
        /* 生效配置并保存 */
        sprintf(nvram_name, NVRAM_WIFI"%d"NVRAM_WIFI_RATE_MODE, wifi_id);
        sprintf(nvram_value, "%d", pstData->ulRateMode);
        if(NvramResultSuccess != nvram_set(nvram_name, nvram_value))
        {
            agent_printf(LOG_ERR, "set rates mode failed");
            return FALSE;
        }
        if(0 == pstData->ulRateMode)
        {
            sprintf(rate_basic, "3,6,7,8,9,10,11");
            sprintf(rate_11n, "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15");
        }
        else
        {
            sprintf(nvram_name, NVRAM_WIFI"%d"NVRAM_WIFI_LEGACY_RATE, wifi_id);
            nvram_get(rate_basic, nvram_name, sizeof(rate_basic));
            sprintf(nvram_name, NVRAM_WIFI"%d"NVRAM_WIFI_MCS_RATE, wifi_id);
            nvram_get(rate_11n, nvram_name, sizeof(rate_11n));
        }
        bFlagBasic = TRUE;
        bFlag11n = TRUE;
        flag = 1;
    }
    /* END:   Added by Zhou Shouya, 2013/9/17 */

    /* 基本速率集变更 */
    if ( 0 != memcmp(pstData->acTransmitSpeedBasic, pstOriData->acTransmitSpeedBasic, 256) )
    {
        /* 0: auto, 1:manual */
        if(0 == pstData->ulRateMode)
        {
            agent_printf(LOG_ERR, "the rates mode is auto\n");
            return FALSE;
        }
        /* 合法性检查 */
        if ( FALSE == agent_check_transmit_speed_validity(0, pstData->acTransmitSpeedBasic, rate_basic) )
        {
            agent_printf(LOG_ERR, "acTransmitSpeedBasic:%s invalid.\n",
                pstData->acTransmitSpeedBasic);
            return FALSE;
        }
        bFlagBasic = TRUE;
    }

    /* 11n速率集变更 */
    if ( 0 != memcmp(pstData->acTransmitSpeed11n, pstOriData->acTransmitSpeed11n, 256) )
    {
        /* 0: auto, 1:manual */
        if(0 == pstData->ulRateMode)
        {
            agent_printf(LOG_ERR, "the rates mode is auto\n");
            return FALSE;
        }
        
        /* 合法性检查 */
        if ( FALSE == agent_check_transmit_speed_validity(1, pstData->acTransmitSpeed11n, rate_11n) )
        {
            agent_printf(LOG_ERR, "acTransmitSpeed11n:%s invalid.\n",
                pstData->acTransmitSpeed11n);
            return FALSE;
        }
        bFlag11n = TRUE;
    }

    /* 取VAP ID */
    if ( bFlagBasic || bFlag11n )
    {
        sprintf(nvram_name, NVRAM_WIFI"%d"NVRAM_WIFI_WLAN_ID_LIST, wifi_id);
        nvram_get(list, nvram_name, sizeof(list));
        memset(vap_list, 0, sizeof(vap_list));
        array_string_to_int(list, vap_list, &total);
    }
    /* 配置没有变更，则返回TRUE */
    else
    {
        return TRUE;
    }
    
    /* 基本速率集配置变更，生效并保存基本速率集配置 */
    if ( bFlagBasic )
    {
        for(loop = 0; loop < total; loop++)
        {
            sprintf(rate_list, "%s", rate_basic);
            sprintf(vap_name, "ap%d_%d", wifi_id, vap_list[loop]);
            if ( 0 != wlan_set_date_basic_rates(vap_name, rate_list) )
            {
                agent_printf(LOG_ERR, "%s set basic rates fail.\n",vap_name);
                return FALSE;
            }
        }
        if(0 == flag)
        {
            sprintf(nvram_name, NVRAM_WIFI"%d"NVRAM_WIFI_LEGACY_RATE, wifi_id);
            if(NvramResultSuccess != nvram_set(nvram_name, rate_basic))
            {
                agent_printf(LOG_ERR, "set legacy rate failed");
                return FALSE;
            }
        }
    }

    /* 11n速率集配置变更，生效并保存11n速率集配置 */
    if ( bFlag11n )
    {
        for(loop = 0; loop < total; loop++)
        {
            sprintf(rate_list, "%s", rate_11n);
            sprintf(vap_name, "ap%d_%d", wifi_id, vap_list[loop]);
            if ( 0 != wlan_set_date_ht_rates(vap_name, rate_list) )
            {
                agent_printf(LOG_ERR, "%s set 11n rates fail.\n",vap_name);
                return FALSE;
            }
        }
        if(0 == flag)
        {
            sprintf(nvram_name, NVRAM_WIFI"%d"NVRAM_WIFI_MCS_RATE, wifi_id);
            if(NvramResultSuccess != nvram_set(nvram_name, rate_11n))
            {
                agent_printf(LOG_ERR, "set mcs rate failed");
                return FALSE;
            }
        }
    }

    if(NvramResultSuccess != nvram_commit())
    {
        agent_printf(LOG_ERR, "nvram commit failed");
        return FALSE;
    }

    agent_printf(LOG_INFO, "modify rates succeed.\n");

	return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL snmp_check_valid_ap_config_vapbasic_table(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_VAPBASIC_ENTRY_T *pstData      = (AP_CONFIG_VAPBASIC_ENTRY_T *)pData;
    AP_CONFIG_VAPBASIC_ENTRY_T *pstOriData   = (AP_CONFIG_VAPBASIC_ENTRY_T *)pOriData;
    BOOL bRet = FALSE;
    UINT32 ulLoopNo = 0;
    UINT32 ulLoopNo2 = 0;
    UINT32 ulRecSize = 0;
    UINT32 ulTableMaxRecNum = WIFI_MAX_NUM;

    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
        return FALSE;
    }

    if ( NULL == pOriData )    
    {
        /*修改或删除记录*/
        for ( ulLoopNo = 0 ; ulLoopNo < ulTableMaxRecNum ; ulLoopNo++ )
        {
            if ( 0 == pstData[ulLoopNo].ulIndex )
            {
                continue;
            }
            for ( ulLoopNo2 = ulLoopNo+1 ; ulLoopNo2 < ulTableMaxRecNum ; ulLoopNo2++ )
            {
                if ( 0 == pstData[ulLoopNo2].ulIndex )
                {
                    continue;
                }
                if ( pstData[ulLoopNo].ulIndex == pstData[ulLoopNo2].ulIndex )
                {
                    lprintf(LOG_ERR, "modify or delete index is duplicated.");
                    return FALSE;
                }
            }
        }
    }
    else
    {
        /*增加记录*/
        if ( 0 == pstData->ulIndex )
        {
            lprintf(LOG_ERR, "add record index is 0.");
            return FALSE;
        }

        for ( ulLoopNo = 0 ; ulLoopNo < ulTableMaxRecNum ; ulLoopNo++ )
        {
            if ( 0 == pstOriData[ulLoopNo].ulIndex )
            {
                break;
            }
            if ( pstData->ulIndex == pstOriData[ulLoopNo].ulIndex )
            {
                lprintf( LOG_ERR, "add record index is duplicated.");
                return FALSE;
            }
            ulRecSize ++;
        }
        if ( ulRecSize >= ulTableMaxRecNum )
        {
            lprintf(LOG_ERR, "add record has been max table record num.");
            return FALSE;
        }
    }

    bRet = ac_agent_modify_ap_config_vapbasic(pData, pOriData);
    if ( FALSE == bRet )
    {
        lprintf(LOG_ERR, "ac_agent_modify_ap_config_vapbasic failed.");
        return FALSE;
    }
    lprintf(LOG_INFO, "ac_agent_modify_ap_config_vapbasic succeed.");

    return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL snmp_check_valid_ap_config_vapsecurity_table(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_VAPSECURITY_ENTRY_T *pstData      = (AP_CONFIG_VAPSECURITY_ENTRY_T *)pData;
    AP_CONFIG_VAPSECURITY_ENTRY_T *pstOriData   = (AP_CONFIG_VAPSECURITY_ENTRY_T *)pOriData;
    BOOL bRet = FALSE;
    UINT32 ulLoopNo = 0;
    UINT32 ulLoopNo2 = 0;
    UINT32 ulRecSize = 0;
    UINT32 ulTableMaxRecNum = WIFI_MAX_NUM;

    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
        return FALSE;
    }

    if ( NULL == pOriData )    
    {
        /*修改或删除记录*/
        for ( ulLoopNo = 0 ; ulLoopNo < ulTableMaxRecNum ; ulLoopNo++ )
        {
            if ( 0 == pstData[ulLoopNo].ulIndex )
            {
                continue;
            }
            for ( ulLoopNo2 = ulLoopNo+1 ; ulLoopNo2 < ulTableMaxRecNum ; ulLoopNo2++ )
            {
                if ( 0 == pstData[ulLoopNo2].ulIndex )
                {
                    continue;
                }
                if ( pstData[ulLoopNo].ulIndex == pstData[ulLoopNo2].ulIndex )
                {
                    lprintf(LOG_ERR, "modify or delete index is duplicated.");
                    return FALSE;
                }
            }
        }
    }
    else
    {
        /*增加记录*/
        if ( 0 == pstData->ulIndex )
        {
            lprintf(LOG_ERR, "add record index is 0.");
            return FALSE;
        }

        for ( ulLoopNo = 0 ; ulLoopNo < ulTableMaxRecNum ; ulLoopNo++ )
        {
            if ( 0 == pstOriData[ulLoopNo].ulIndex )
            {
                break;
            }
            if ( pstData->ulIndex == pstOriData[ulLoopNo].ulIndex )
            {
                lprintf( LOG_ERR, "add record index is duplicated.");
                return FALSE;
            }
            ulRecSize ++;
        }
        if ( ulRecSize >= ulTableMaxRecNum )
        {
            lprintf(LOG_ERR, "add record has been max table record num.");
            return FALSE;
        }
    }

    bRet = ac_agent_modify_ap_config_vapsecurity(pData, pOriData);
    if ( FALSE == bRet )
    {
        lprintf(LOG_ERR, "ac_agent_modify_ap_config_vapsecurity failed.");
        return FALSE;
    }
    lprintf(LOG_INFO, "ac_agent_modify_ap_config_vapsecurity succeed.");

    return TRUE;
}

#endif
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL snmp_check_valid_ap_config_alarm(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_ALARM_T *pstData      = (AP_CONFIG_ALARM_T *)pData;
    AP_CONFIG_ALARM_T *pstOriData   = (AP_CONFIG_ALARM_T *)pOriData;
    BOOL bRet = FALSE;

    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
    }

    if ( NULL == pOriData )
    {
        lprintf(LOG_ERR, "pOriData is null.");
    }

    if ( 0!= memcmp(&pstData->ulCpuLoadHigh, &pstOriData->ulCpuLoadHigh, sizeof(UINT32)*8 ) )
    {
        bRet = ac_agent_modify_ap_config_alarm(pData, pOriData);
        if ( FALSE == bRet )
        {
            lprintf(LOG_ERR, "app_alarmconfig_set failed.");
            return FALSE;
        }
        lprintf(LOG_INFO, "app_alarmconfig_set succeed.");
    }

	return TRUE;
}
BOOL snmp_check_valid_ap_config_trapdes(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_TRAPDES_ENTRY_T *pstData      = (AP_CONFIG_TRAPDES_ENTRY_T *)pData;
    AP_CONFIG_TRAPDES_ENTRY_T *pstOriData   = (AP_CONFIG_TRAPDES_ENTRY_T *)pOriData;
    BOOL bRet = FALSE;
    
    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
        return FALSE;
    }

    if(pstData->ulSwtich != pstOriData->ulSwtich
        || pstData->ulTrapDesPort != pstOriData->ulTrapDesPort
        || 0 != memcmp(pstData->acTrapCommunity, pstOriData->acTrapCommunity, strlen(pstOriData->acTrapCommunity))
        || 0 != memcmp(pstData->aucTrapDesIPAddress, pstOriData->aucTrapDesIPAddress, 4))
    {
        bRet = ac_agent_modify_ap_config_trapdes(pData, pOriData);
    }
    if ( FALSE == bRet )
    {
        lprintf(LOG_ERR, "ac_agent_modify_ap_config_trapdes failed.");
        return FALSE;
    }
    lprintf(LOG_INFO, "ac_agent_modify_ap_config_trapdes succeed.");

	return TRUE;
}
/* BEGIN: Added by zoucaihong, 2012/12/5 */
#ifndef GW_X86
BOOL snmp_check_valid_ap_config_vlan(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_VLAN_ENTRY_T *pstData      = (AP_CONFIG_VLAN_ENTRY_T *)pData;
    AP_CONFIG_VLAN_ENTRY_T *pstOriData   = (AP_CONFIG_VLAN_ENTRY_T *)pOriData;
    BOOL bRet = FALSE;
    UINT32 ulLoopNo = 0;
    UINT32 ulLoopNo2 = 0;
    UINT32 ulRecSize = 0;
    UINT32 ulTableMaxRecNum = TABLE_MAXRECNUM_VLAN;

    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
        return FALSE;
    }

    if ( NULL == pOriData )    
    {
        /*修改或删除记录*/
        for ( ulLoopNo = 0 ; ulLoopNo < ulTableMaxRecNum ; ulLoopNo++ )
        {
            if ( 0 == pstData[ulLoopNo].ulIndex )
            {
                continue;
            }
            for ( ulLoopNo2 = ulLoopNo+1 ; ulLoopNo2 < ulTableMaxRecNum ; ulLoopNo2++ )
            {
                if ( 0 == pstData[ulLoopNo2].ulIndex )
                {
                    continue;
                }
                if ( pstData[ulLoopNo].ulIndex == pstData[ulLoopNo2].ulIndex )
                {
                    lprintf(LOG_ERR, "modify or delete index is duplicated.");
                    return FALSE;
                }
            }
        }
    }
    else
    {
        /*增加记录*/
        if ( 0 == pstData->ulIndex )
        {
            lprintf(LOG_ERR, "add record index is 0.");
            return FALSE;
        }

        for ( ulLoopNo = 0 ; ulLoopNo < ulTableMaxRecNum ; ulLoopNo++ )
        {
            if ( 0 == pstOriData[ulLoopNo].ulIndex )
            {
                break;
            }
            if ( pstData->ulIndex == pstOriData[ulLoopNo].ulIndex )
            {
                lprintf( LOG_ERR, "add record index is duplicated.");
                return FALSE;
            }
            ulRecSize ++;
        }
        if ( ulRecSize >= ulTableMaxRecNum )
        {
            lprintf(LOG_ERR, "add record has been max table record num.");
            return FALSE;
        }
    }

    bRet = ac_agent_modify_ap_config_vlan(pData, pOriData);
    if ( FALSE == bRet )
    {
        lprintf(LOG_ERR, "ac_agent_modify_ap_config_vlan failed.");
        return FALSE;
    }
    lprintf(LOG_INFO, "ac_agent_modify_ap_config_vlan succeed.");

	return TRUE;
}
BOOL snmp_check_valid_ap_config_ssid(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_SSID_ENTRY_T *pstData      = (AP_CONFIG_SSID_ENTRY_T *)pData;
    AP_CONFIG_SSID_ENTRY_T *pstOriData   = (AP_CONFIG_SSID_ENTRY_T *)pOriData;
    BOOL bRet = FALSE;
    UINT32 ulLoopNo = 0;
    UINT32 ulLoopNo2 = 0;
    UINT32 ulRecSize = 0;
    UINT32 ulTableMaxRecNum = TABLE_MAXRECNUM_AP_VAP;

    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
        return FALSE;
    }

    if ( NULL == pOriData )    
    {
        /*修改或删除记录*/
        for ( ulLoopNo = 0 ; ulLoopNo < ulTableMaxRecNum ; ulLoopNo++ )
        {
            if ( 0 == pstData[ulLoopNo].ulIndex )
            {
                continue;
            }
            for ( ulLoopNo2 = ulLoopNo+1 ; ulLoopNo2 < ulTableMaxRecNum ; ulLoopNo2++ )
            {
                if ( 0 == pstData[ulLoopNo2].ulIndex )
                {
                    continue;
                }
                if ( pstData[ulLoopNo].ulIndex == pstData[ulLoopNo2].ulIndex )
                {
                    lprintf(LOG_ERR, "modify or delete index is duplicated.");
                    return FALSE;
                }
            }
        }
    }
    else
    {
        /*增加记录*/
        if ( 0 == pstData->ulIndex )
        {
            lprintf(LOG_ERR, "add record index is 0.");
            return FALSE;
        }

        for ( ulLoopNo = 0 ; ulLoopNo < ulTableMaxRecNum ; ulLoopNo++ )
        {
            if ( 0 == pstOriData[ulLoopNo].ulIndex )
            {
                break;
            }
            if ( pstData->ulIndex == pstOriData[ulLoopNo].ulIndex )
            {
                lprintf( LOG_ERR, "add record index is duplicated.");
                return FALSE;
            }
            ulRecSize ++;
        }
        if ( ulRecSize >= ulTableMaxRecNum )
        {
            lprintf(LOG_ERR, "add record has been max table record num.");
            return FALSE;
        }
    }

    bRet = ac_agent_modify_ap_config_ssid(pData, pOriData);
    if ( FALSE == bRet )
    {
        lprintf(LOG_ERR, "ac_agent_modify_ap_config_ssid failed.");
        return FALSE;
    }
    lprintf(LOG_INFO, "ac_agent_modify_ap_config_ssid succeed.");

	return TRUE;
}
#endif
#if 0
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL snmp_check_valid_systeminfo(VOID *pData, VOID *pOriData)
{
    SYSTEM_T *pstData    = (SYSTEM_T *)pData;
    SYSTEM_T *pstOriData = (SYSTEM_T *)pOriData;
    BOOL bRet = FALSE;
    CHAR acNvramName[NVRAM_SIZE] = {0};

    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
    }

    if ( NULL == pOriData )
    {
        lprintf(LOG_ERR, "pOriData is null.");
    }

    if ( 0!= strcmp(pstData->acSysContact, pstOriData->acSysContact) )
    {
        sprintf(acNvramName, "%s", NVRAM_SYSCONTACT);
        nvram_set(acNvramName, pstData->acSysContact);
    }

    if ( 0!= strcmp(pstData->acSysName, pstOriData->acSysName) )
    {
        sprintf(acNvramName, "%s", NVRAM_SYSNAME);
        nvram_set(acNvramName, pstData->acSysName);
    }

    if ( 0!= strcmp(pstData->acSysLocation, pstOriData->acSysLocation) )
    {
        sprintf(acNvramName, "%s", NVRAM_SYSLOCATION);
        nvram_set(acNvramName, pstData->acSysLocation);
    }
    
    nvram_commit();
    
    bRet = agent_get_system();
    if ( !bRet )
    {
        agent_printf(LOG_ERR, "File:%s Line:%d agent_get_system failed.\n",
            __FILE__, __LINE__);
        return FALSE;
    }
    agent_printf(LOG_INFO, "snmp_check_valid_systeminfo succeed.\n");
    
	return TRUE;
}
/* END: Added by zoucaihong, 2012/12/5 */
#endif
/* BEGIN: Added by zoucaihong, 2015/3/25 */
BOOL snmp_check_valid_ap_config_user(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_USER_T *pstData      = (AP_CONFIG_USER_T *)pData;
    AP_CONFIG_USER_T *pstOriData   = (AP_CONFIG_USER_T *)pOriData;
    BOOL bRet = FALSE;
    SNMPAGENT_USER_T stUser;
    UINT8 aucInvalidMac[6] = {0};
    
    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
        return FALSE;
    }
    
    lprintf(LOG_INFO, "ulCommit:%d.\n", pstData->ulCommit);
    if ( AP_CFG_COMMIT == pstData->ulCommit )
    {
        memset(&stUser, 0, sizeof(SNMPAGENT_USER_T));
        if ( 0 == memcmp(pstData->aucMac, aucInvalidMac, 6) )
        {
            lprintf(LOG_INFO, "MAC:"MAC_FMT".\n", 
                MAC_ARG(stUser.aucMac));
            goto end;
        }
        if ( pstData->ulAction > SNMPAGENT_USER_LOGOUT )
        {
            lprintf(LOG_INFO, "ulAction:%d.\n", 
                pstData->ulAction);
            goto end;
        }
        if ( SNMPAGENT_USER_LOGIN == pstData->ulAction )
        {
            if ( pstData->ulAuthType > 9 )
            {
                lprintf(LOG_INFO, "ulAuthType:%d.\n", 
                    pstData->ulAuthType);
                goto end;
            }
        }

        memcpy(stUser.aucMac, pstData->aucMac, 6);
        if ( SNMPAGENT_USER_LOGIN == pstData->ulAction )
        {
            stUser.iAuthState = STAM_STA_AUTHED;
        }
        else if ( SNMPAGENT_USER_LOGOUT == pstData->ulAction )
        {
            stUser.iAuthState = STAM_STA_UNAUTHED;
        }
        stUser.iAuthType = pstData->ulAuthType;
        memcpy(stUser.acUserId, pstData->acUserId, 64);
        lprintf(LOG_INFO, "MAC:"MAC_FMT" iAuthType:%d iAuthState:%d acUserId:%s ulTimeLimit:%u.\n", 
            MAC_ARG(stUser.aucMac), stUser.iAuthType, stUser.iAuthState, stUser.acUserId,
            pstData->ulTimeLimit);

        if( FALSE == snmp_notify_wifidog_user(&stUser))
        {
            lprintf(LOG_ERR, "snmp_notify_wifidog_user fail.");
            pstData->ulCommit = AP_CFG_NORMAL;
            return FALSE;
        }

        SNMPAGENT_AUTH_T stAuth = {0};
        
        stAuth.iSwitch = g_pstSnmpAgentData->stAuthConfig.ulSwitch;
        stAuth.iWeb_auth = g_pstSnmpAgentData->stAuthConfig.ulWebAuth;
        stAuth.iOffline = g_pstSnmpAgentData->stAuthConfig.ulOffline;/* unit:s */
        stAuth.iReverify = pstData->ulTimeLimit*60;/* unit:min */
        
        if( FALSE == snmp_notify_wifidog_set_authcfg(&stAuth))
        {
            lprintf(LOG_ERR, "snmp_notify_wifidog_auth fail.");
            return FALSE;
        }

        lprintf(LOG_INFO, "snmp_check_valid_ap_config_user succeed.");
    }

end:    
    pstData->ulCommit = AP_CFG_NORMAL;

	return TRUE;
}
BOOL snmp_check_valid_ap_config_gwssid(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_GWSSID_T *pstData      = (AP_CONFIG_GWSSID_T *)pData;
    AP_CONFIG_GWSSID_T *pstOriData   = (AP_CONFIG_GWSSID_T *)pOriData;
    BOOL bRet = FALSE;
    AP_SSID_LIST_T stSsidList;
    memset(&stSsidList, 0, sizeof(stSsidList));
    
    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
        return FALSE;
    }
    if ( 0 == strlen(pstData->acGwSsid) )
    {
        lprintf(LOG_ERR, "acGwSsid length is 0.");
        return TRUE;
    }
    lprintf(LOG_INFO, "acGwSsid:%s.", pstData->acGwSsid);

    if( FALSE == multi_ssid_convert(pstData->acGwSsid, &stSsidList))
    {
        lprintf(LOG_INFO, "multi_ssid_convert fail.");
        return FALSE;
    }

    if( FALSE == snmp_notify_ap_ssidcfg(&stSsidList))
    {
        lprintf(LOG_INFO, "snmp_notify_ap_ssidcfg fail.");
        return FALSE;
    }
    lprintf(LOG_INFO, "snmp_check_valid_ap_config_gwssid succeed.");
    memset(pstData->acGwSsid, 0, 512);
	return TRUE;
}
BOOL snmp_check_valid_ap_config_auth(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_AUTH_T *pstData      = (AP_CONFIG_AUTH_T *)pData;
    AP_CONFIG_AUTH_T *pstOriData   = (AP_CONFIG_AUTH_T *)pOriData;
    BOOL bRet = FALSE;
    SNMPAGENT_AUTH_T stAuth = {0};
    
    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
        return FALSE;
    }

    lprintf(LOG_INFO, "iSwitch:%u iWeb_auth:%u iOffline:%u iReverify:%u.\n",
        pstData->ulSwitch, pstData->ulWebAuth, pstData->ulOffline, pstData->ulReverify);

    stAuth.iSwitch = pstData->ulSwitch;
    stAuth.iWeb_auth = pstData->ulWebAuth;
    stAuth.iOffline = pstData->ulOffline;/* unit:s */
    stAuth.iReverify = pstData->ulReverify*60;/* unit:min */
    
    if( FALSE == snmp_notify_wifidog_set_authcfg(&stAuth))
    {
        lprintf(LOG_ERR, "snmp_notify_wifidog_auth fail.");
        return FALSE;
    }

    lprintf(LOG_INFO, "snmp_check_valid_ap_config_auth succeed.");

	return TRUE;
}
BOOL snmp_check_valid_ap_config_batch_bwlist(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_BATCH_BWLIST_T *pstData      = (AP_CONFIG_BATCH_BWLIST_T *)pData;
    AP_CONFIG_BATCH_BWLIST_T *pstOriData   = (AP_CONFIG_BATCH_BWLIST_T *)pOriData;
    BOOL bRet = FALSE;
    UINT32 ulLoop = 0;
    UINT32 ulListType = 0;
    SNMPAGENT_MAC_BAW_T *pstMacList = NULL;
    AP_MAC_LIST_T stApMacList;
    memset(&stApMacList, 0, sizeof(stApMacList));

    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
        return FALSE;
    }
    if ( STATUS_NORMAL == pstData->ulStatus )
    {
        lprintf(LOG_INFO, "STATUS_NORMAL.");
        return TRUE;
    }
    lprintf(LOG_INFO, "maclist:%s listType:%d status:%u.\n", 
        pstData->acMacList, pstData->ulListType, pstData->ulStatus);
    if ( (0 == pstData->ulListType) || (0 == strlen(pstData->acMacList)) )
    {
        goto fail;
    }
    if ( 1 == pstData->ulListType )
    {
        ulListType = 2;
    }
    else if ( 2 == pstData->ulListType )
    {
        ulListType = 1;
    }

    string_macs_to_array(pstData->acMacList, &stApMacList);
    pstMacList = (SNMPAGENT_MAC_BAW_T *)malloc((stApMacList.ulNum)*sizeof(SNMPAGENT_MAC_BAW_T));
    if ( NULL == pstMacList )
    {
        lprintf(LOG_ERR, "pstMacList is null.");
        goto fail;
    }
    memset(pstMacList, 0, (stApMacList.ulNum)*sizeof(SNMPAGENT_MAC_BAW_T));
    for ( ulLoop = 0 ; ulLoop < stApMacList.ulNum ; ulLoop++ )
    {
        memcpy(pstMacList[ulLoop].aucMac, stApMacList.stMacList[ulLoop].aucMac, 6);
        pstMacList[ulLoop].iType = ulListType;
        lprintf(LOG_ERR, "ulLoop:%u aucMac:"MAC_FMT" iType:%d.",
            ulLoop, MAC_ARG(pstMacList[ulLoop].aucMac), pstMacList[ulLoop].iType);
    }
    /* BEGIN: Added by zoucaihong, 2015/4/16 */
    switch ( pstData->ulStatus )
    {
        case STATUS_MODIFY :
            if( FALSE == snmp_notify_wifidog_update_mac_baw(0, stApMacList.ulNum, pstMacList))
            {
                lprintf(LOG_ERR, "snmp_notify_wifidog_update_mac_baw fail.");
                goto fail;
            }
            break;
        case STATUS_ADD :
            if( FALSE == snmp_notify_wifidog_add_mac_baw(0, stApMacList.ulNum, pstMacList))
            {
                lprintf(LOG_ERR, "snmp_notify_wifidog_add_mac_baw fail.");
                goto fail;
            }
            break;
        case STATUS_DELETE :
            if( FALSE == snmp_notify_wifidog_del_mac_baw(0, stApMacList.ulNum, pstMacList))
            {
                lprintf(LOG_ERR, "snmp_notify_wifidog_add_mac_baw fail.");
                goto fail;
            }
            break;
    }
    /* END: Added by zoucaihong, 2015/4/16 */
    free(pstMacList);
    pstData->ulStatus = STATUS_NORMAL;
    
    lprintf(LOG_INFO, "snmp_check_valid_ap_config_batch_bwlist succeed.");

	return TRUE;
fail:
    if ( NULL != pstMacList )
    {
        free(pstMacList);
    }
    pstData->ulStatus = STATUS_NORMAL;
    
    lprintf(LOG_INFO, "snmp_check_valid_ap_config_batch_bwlist fail.");

	return FALSE;
}
BOOL snmp_check_valid_ap_config_domain_filter(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_DOMAIN_FILTER_T *pstData      = (AP_CONFIG_DOMAIN_FILTER_T *)pData;
    SNMPAGENT_URL_BAW_T astUrlBaw[10];
    CHAR *pcPos = NULL;
    CHAR *pcTmp = NULL;
    UINT32 ulNum = 0;
    UINT32 ulLoop = 0;

    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
        return FALSE;
    }

    if ( NULL == pOriData )    
    {
        lprintf(LOG_ERR, "pOriData is null.");
        return FALSE;
    }
    if ( STATUS_NORMAL == pstData->ulStatus )
    {
        lprintf(LOG_INFO, "STATUS_NORMAL.");
        return TRUE;
    }

    memset(&astUrlBaw, 0, sizeof(astUrlBaw));
    lprintf(LOG_INFO, "domain:%s type:%u status:%u.",
        pstData->acDomain,
        pstData->ulType,
        pstData->ulStatus);

    pcTmp = pstData->acDomain;
    while ( NULL != (pcPos = strstr(pstData->acDomain, ",")) )
    {
        *pcPos = '\0';
        strcpy(astUrlBaw[ulNum].acUrl, pcTmp);
        astUrlBaw[ulNum].iType = pstData->ulType;
        pcPos++;
        pcTmp=pcPos;
        ulNum++;
    }
    /* 只有一条记录或多条记录的最后一条 */
    strcpy(astUrlBaw[ulNum].acUrl, pcTmp);
    astUrlBaw[ulNum].iType = pstData->ulType;
    ulNum++;

    for ( ulLoop = 0 ; ulLoop < ulNum ; ulLoop++ )
    {
        lprintf(LOG_INFO, "snmp_check_valid_ap_config_domain_filter "
            "acUrl:%s "
            "iType:%d "
            "ulNum:%u ",
            astUrlBaw[ulLoop].acUrl,
            astUrlBaw[ulLoop].iType,
            ulNum
            );
    }
        
    /*增加记录*/
    if ( pstData->ulStatus == STATUS_ADD)
    {
        snmp_notify_wifidog_add_url_baw(0, 1, astUrlBaw);
        lprintf(LOG_INFO, "snmp_notify_wifidog_add_url_baw index:%u.", pstData->ulIndex);
    }
    /* 删除记录 */
    else if ( pstData->ulStatus == STATUS_DELETE)
    {
        snmp_notify_wifidog_del_url_baw(0, 1, astUrlBaw);
        lprintf(LOG_INFO, "snmp_notify_wifidog_del_url_baw index:%u.", pstData->ulIndex);
    }
    pstData->ulStatus = STATUS_NORMAL;

	return TRUE;
}
BOOL snmp_check_valid_ap_config_user_speed(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_USER_SPEED_T  *pstData = (AP_CONFIG_USER_SPEED_T *)pData;
    AP_CONFIG_USER_SPEED_T  *pstOriData = (AP_CONFIG_USER_SPEED_T *)pOriData;

    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
        return FALSE;
    }

    if ( NULL == pOriData )    
    {
        lprintf(LOG_ERR, "pOriData is null.");
        return FALSE;
    }

    lprintf(LOG_INFO, "commit:%u upMax:%u dwMax:%u.",
        pstData->ulCommit, pstData->ulUploadMax, pstData->ulDwloadMax);

    if ( AP_CFG_COMMIT == pstData->ulCommit )
    {
        snmp_notify_wifidog_user_speed(pstData);
    }
    pstData->ulCommit = AP_CFG_NORMAL;

	return TRUE;
}

BOOL snmp_check_valid_ap_config_white_user_speed(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_USER_SPEED_T  *pstData = (AP_CONFIG_USER_SPEED_T *)pData;
    AP_CONFIG_USER_SPEED_T  *pstOriData = (AP_CONFIG_USER_SPEED_T *)pOriData;

    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
        return FALSE;
    }

    if ( NULL == pOriData )    
    {
        lprintf(LOG_ERR, "pOriData is null.");
        return FALSE;
    }

    lprintf(LOG_INFO, "commit:%u upMax:%u dwMax:%u.",
        pstData->ulCommit, pstData->ulUploadMax, pstData->ulDwloadMax);

    if ( AP_CFG_COMMIT == pstData->ulCommit )
    {
        snmp_notify_wifidog_white_user_speed(pstData);
    }
    pstData->ulCommit = AP_CFG_NORMAL;

	return TRUE;
}

BOOL snmp_check_valid_ap_config_user_logout(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_USER_LOGOUT_T *pstData = (AP_CONFIG_USER_LOGOUT_T *)pData;
    AP_CONFIG_USER_LOGOUT_T *pstOriData = (AP_CONFIG_USER_LOGOUT_T *)pOriData;

    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
        return FALSE;
    }

    if ( NULL == pOriData )    
    {
        lprintf(LOG_ERR, "pOriData is null.");
        return FALSE;
    }

    lprintf(LOG_INFO, "ulLogoutStatus:%u ulTimeCondition:%u ulFlowCondition:%u ulCommit:%u.",
        pstData->ulLogoutStatus, 
        pstData->ulTimeCondition, 
        pstData->ulFlowCondition,
        pstData->ulCommit);
    if ( AP_CFG_COMMIT == pstData->ulCommit )
    {
        if( FALSE == snmp_notify_wifidog_user_logout(pstData))
        {
            lprintf(LOG_ERR, "snmp_notify_wifidog_user_logout fail.");
            return FALSE;
        }
    }
    pstData->ulCommit = AP_CFG_NORMAL;
    lprintf(LOG_INFO, "snmp_check_valid_ap_config_user_logout success.");

	return TRUE;
}

BOOL snmp_check_valid_ap_config_get_secret_key(VOID *pData, VOID *pOriData)
{
    AP_CONFIG_GET_SECRET_KEY_T *pstData = (AP_CONFIG_GET_SECRET_KEY_T *)pData;
    AP_CONFIG_GET_SECRET_KEY_T *pstOriData = (AP_CONFIG_GET_SECRET_KEY_T *)pOriData;

    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
        return FALSE;
    }

    if ( NULL == pOriData )    
    {
        lprintf(LOG_ERR, "pOriData is null.");
        return FALSE;
    }

    lprintf(LOG_INFO, "ulInterval:%u.", pstData->ulInterval);
    if( FALSE == snmp_notify_wifidog_get_secret_key(pstData))
    {
        lprintf(LOG_ERR, "snmp_notify_wifidog_get_secret_key fail.");
        return FALSE;
    }
    lprintf(LOG_INFO, "snmp_check_valid_ap_config_get_secret_key success.");

	return TRUE;
}

BOOL snmp_check_valid_ap_oper_gwap(VOID *pData, VOID *pOriData)
{
    AP_OPER_GWAP_T *pstData      = (AP_OPER_GWAP_T *)pData;
    AP_OPER_GWAP_T *pstOriData   = (AP_OPER_GWAP_T *)pOriData;
    BOOL bRet = FALSE;
    UINT8 aucMac[6] = {0};
    UINT8 aucInvalidMac[6] = {0};
    UINT32 ulLoop = 0;
    AP_MAC_LIST_T stApMacList;
    SNMPAGENT_UPDATE_APCFG_T stApCfg;
    memset(&stApMacList, 0, sizeof(stApMacList));
    memset(&stApCfg, 0, sizeof(stApCfg));
    
    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
        return FALSE;
    }

    switch ( pstData->ulAction )
    {
        /* 重启AP */
        case GW_AP_OPSTATE_REBOOT:
            if ( 0 == strlen(pstData->acApMac) )
            {
                lprintf(LOG_ERR, "ap macs length is 0.\n");
                return TRUE;
            }
            string_mac_tools(pstData->acApMac, aucMac);
            if ( 0 == memcmp(aucMac, aucInvalidMac, 6) )
            {
                lprintf(LOG_ERR, "ap mac is invalid.\n");
                return FALSE;
            }
            lprintf(LOG_INFO, "ap mac:"MAC_FMT".\n", MAC_ARG(aucMac));
            if( FALSE == snmp_notify_ap_reboot(aucMac))
            {
                lprintf(LOG_ERR, "snmp_notify_ap_reboot fail.\n");
                pstData->ulAction=GW_AP_OPSTATE_NORMAL;
                memset(pstData->acApMac, 0, sizeof(pstData->acApMac));
                return FALSE;
            }
            else
            {
                lprintf(LOG_ERR, "snmp_notify_ap_reboot succeed.\n");
                pstData->ulAction=GW_AP_OPSTATE_NORMAL;
                memset(pstData->acApMac, 0, sizeof(pstData->acApMac));
                return TRUE;
            }
            break;
        /* AP升级软件版本 */
        case GW_AP_OPSTATE_UPGRADE :
            if ( 0 == strlen(pstData->acApMac) )
            {
                lprintf(LOG_ERR, "ap macs length is 0.\n");
                return TRUE;
            }
            string_macs_to_array(pstData->acApMac, &stApMacList);
            lprintf(LOG_INFO, "macNum:%d.\n", stApMacList.ulNum);
            memset(pstData->acApMac, 0, sizeof(pstData->acApMac));
            break;
        case GW_AP_OPSTATE_UPDATE :
            if ( (0 == strlen(pstData->acApMac))
                || (0 == pstData->ulRoad)
                || (0 == pstData->ulTotalLink)
                || (0 == strlen(pstData->acFirstSsid))
                || (0 == strlen(pstData->acSecondSsid))
                || (0 == strlen(pstData->acApMemo)))
            {
                lprintf(LOG_ERR, "ap config params invalid.\n");
                return TRUE;
            }
            string_macs_to_array(pstData->acApMac, &stApMacList);
            stApCfg.channel = pstData->ulRoad;
            stApCfg.totalLink = pstData->ulTotalLink;
            strcpy(stApCfg.acFirstSsid, pstData->acFirstSsid);
            strcpy(stApCfg.acSecondSsid, pstData->acSecondSsid);
            strcpy(stApCfg.acMemo, pstData->acApMemo);
            for ( ulLoop = 0 ; ulLoop < stApMacList.ulNum ; ulLoop++ )
            {
                snmp_notify_ap_cfgupdate(&stApMacList.stMacList[ulLoop], &stApCfg);
            }
            memset(pstData, 0, sizeof(AP_OPER_GWAP_T));
            break;
        default:
            lprintf(LOG_ERR, "ulAction:%d invalid.\n", pstData->ulAction);
            return FALSE;
    }

	return TRUE;
}
/* END: Added by zoucaihong, 2015/3/25 */
/* BEGIN: Added by zoucaihong, 2014/5/14 */
BOOL snmp_check_valid_ap_oper_sysoper(VOID *pData, VOID *pOriData)
{
    AP_OPER_SYSOPER_T *pstData    = (AP_OPER_SYSOPER_T *)pData;
    CHAR acNvramValue[32] = {0};
    CHAR acCmd[64] = {0};
    UINT32 ulDhcpNum = 0;
    UINT32 ulLoop = 0;

    DHCP_SERVER_T *pstDhcpServer,stDhcpServer;

    if ( NULL == pData )
    {
        lprintf(LOG_ERR, "pData is null.");
    }

    if ( NULL == pOriData )
    {
        lprintf(LOG_ERR, "pOriData is null.");
    }

    /* ==================== 设备软件升级 ==================== */
    if ( AP_SYSOPER_SET == pstData->ulSoftwareUpdate)
    {
        if ( AP_SYSOPER_NORMAL == g_ulSoftwareUpState )
        {
            g_ulSoftwareUpState = AP_SYSOPER_SET;
            if( FALSE == app_software_update())
            {
                agent_printf(LOG_ERR, "app_software_update fail.\n");
                pstData->ulSoftwareUpdate = AP_SYSOPER_NORMAL;
                g_ulSoftwareUpState = AP_SYSOPER_NORMAL;
                return FALSE;
            }
        }
    }
    pstData->ulSoftwareUpdate = AP_SYSOPER_NORMAL;

    /* BEGIN: Added by zoucaihong, 2014/6/17 */
    /* ==================== 设备重启 ==================== */
    if ( AP_SYSOPER_SET == pstData->ulSysRestart)
    {
        if( FALSE == app_restart_ap())
        {
            agent_printf(LOG_ERR, "app_restart_ap fail.\n");
            pstData->ulSysRestart = AP_SYSOPER_NORMAL;
            return FALSE;
        }
    }
    pstData->ulSysRestart = AP_SYSOPER_NORMAL;
    
    /* ==================== 设备恢复出厂配置 ==================== */
    /* 保留IP配置 */
    if ( AP_SYSOPER_SET == pstData->ulSysReset )
    {
        if( FALSE == app_reset_ap(pstData->ulSysReset))
        {
            agent_printf(LOG_ERR, "app_reset_ap fail.\n");
            pstData->ulSysReset = AP_SYSOPER_NORMAL;
            return FALSE;
        }
    }
    pstData->ulSysReset = AP_SYSOPER_NORMAL;
    
    /* ==================== 配置文件导入导出 ==================== */
    if ( AP_CFGFILE_IMPORT == pstData->ulCfgFileImpExp
        || AP_CFGFILE_EXPORT == pstData->ulCfgFileImpExp )
    {
        if ( FALSE == app_cfgfile_impexp(pstData->ulCfgFileImpExp) )
        {
            agent_printf(LOG_ERR, "app_cfgfile_impexp fail.\n");
            pstData->ulCfgFileImpExp = AP_CFGFILE_NORMAL;
            return FALSE;
        }
    }
    pstData->ulCfgFileImpExp = AP_CFGFILE_NORMAL;
    /* END: Added by zoucaihong, 2014/6/17 */

    /* ==================== 设备停用/恢复 ==================== */
    #ifdef OPENWRT
    if ( AP_STOPRECOVER_NORMAL != pstData->ulSysStopRecover )
    {
        pstDhcpServer = &stDhcpServer;
        pal_dhcp_server_info_get(pstDhcpServer);

        agent_printf(LOG_INFO, "pstDhcpServer->dhcp_server_num:%d.", pstDhcpServer->dhcp_server_num);
    }
    if ( AP_STOPRECOVER_STOP == pstData->ulSysStopRecover )
    {
        for(ulDhcpNum = 0;ulDhcpNum< pstDhcpServer->dhcp_server_num;ulDhcpNum++)
        {
            if(pstDhcpServer->dhcp_server_info[ulDhcpNum].dhcp_enable == 1)
            {
                memset(acCmd,0,sizeof(acCmd));
                sprintf(acCmd, "ifconfig br-%s down", pstDhcpServer->dhcp_server_info[ulDhcpNum].dhcp_interface);
                agent_printf(LOG_INFO, "acCmd:%s.", acCmd);
                system(acCmd);
            }
        }
            
    }
    else if ( AP_STOPRECOVER_RECOVER == pstData->ulSysStopRecover )
    {
          for(ulDhcpNum = 0;ulDhcpNum<pstDhcpServer->dhcp_server_num;ulDhcpNum++)
        {
            if(pstDhcpServer->dhcp_server_info[ulDhcpNum].dhcp_enable == 1)
            {
                sprintf(acCmd, "ifconfig br-%s up", pstDhcpServer->dhcp_server_info[ulDhcpNum].dhcp_interface);
                agent_printf(LOG_INFO, "acCmd:%s.", acCmd);
                system(acCmd);
            }
        }
    }


    #else 
    if ( AP_STOPRECOVER_NORMAL != pstData->ulSysStopRecover )
    {
        nvram_get(acNvramValue, NVRAM_DHCP_SERVER_NUM, 32);
        ulDhcpNum = atoi(acNvramValue);
        agent_printf(LOG_INFO, "ulDhcpNum:%u.", ulDhcpNum);
    }
    if ( AP_STOPRECOVER_STOP == pstData->ulSysStopRecover )
    {
        if ( 0 != ulDhcpNum )
        {
            for ( ulLoop = 0 ; ulLoop < ulDhcpNum ; ulLoop++ )
            {
                sprintf(acCmd, "ifconfig br%u down", ulLoop);
                agent_printf(LOG_INFO, "acCmd:%s.", acCmd);
                system(acCmd);
            }
        }
    }
    else if ( AP_STOPRECOVER_RECOVER == pstData->ulSysStopRecover )
    {
        if ( 0 != ulDhcpNum )
        {
            for ( ulLoop = 0 ; ulLoop < ulDhcpNum ; ulLoop++ )
            {
                sprintf(acCmd, "ifconfig br%u up", ulLoop);
                agent_printf(LOG_INFO, "acCmd:%s.", acCmd);
                system(acCmd);
            }
        }
    }
    #endif
    
    pstData->ulSysStopRecover = AP_STOPRECOVER_NORMAL;
    return TRUE;
}
/* END: Added by zoucaihong, 2014/5/14 */
