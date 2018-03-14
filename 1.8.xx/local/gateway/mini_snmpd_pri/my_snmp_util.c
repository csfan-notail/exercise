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
#include <netdb.h>     /* gethostbyname */

#include <semaphore.h>
#include <time.h>
#ifndef OPENWRT
#include <linux/sys.h>
#endif
#include <sys/sysinfo.h> 

#include "mini_snmpd.h"

#include "my_snmp_def.h"
#include "my_snmp_data.h"
#include "my_snmp_handle.h"
#include "my_snmp_info.h"
#include "my_snmp_util.h"
#include "snmp_interface.h"

#include "config_common.h"
#include "nvram_config.h"
/* BEGIN: Added by zoucaihong, 2014/6/9 */
#include "interface.h"
#include "log.h"
#include "tool.h"
/* END: Added by zoucaihong, 2014/6/9 */

/************************************************************************/
extern SNMP_AGENT_DATA_T *g_pstSnmpAgentData;
extern OPEN_MIB_COMMUNITY_CONFIG_T g_stSnmpV2Config;

AC_IP_LIST_T g_ac_ip_list = {0};


UINT32  g_aulTableRegetCheck[MAX_TABLE_NUM] = {0};
extern UINT32      g_ulEndian;
extern int g_stati_per;
/* BEGIN: Added by zoucaihong, 2015/6/9 */
UINT8   g_aucCmsIp[4] = {0};
/* END: Added by zoucaihong, 2015/6/9 */
RATE_DEF_T astRates[] = 
{
	/*rate    index*/
	{"1",       0},
	{"2",       1},
	{"5.5",     2},
	{"11",      3},
	{"6",       4},
	{"9",       5},
	{"12",      6},
	{"18",      7},
	{"24",      8},
	{"36",      9},
	{"48",      10},
	{"54",      11},
	{"MCS0",    12+0},
	{"MCS1",    12+1},
	{"MCS2",    12+2},
	{"MCS3",    12+3},
	{"MCS4",    12+4},
	{"MCS5",    12+5},
	{"MCS6",    12+6},
	{"MCS7",    12+7},
	{"MCS8",    12+8},
	{"MCS9",    12+9},
	{"MCS10",   12+10},
	{"MCS11",   12+11},
	{"MCS12",   12+12},
	{"MCS13",   12+13},
	{"MCS14",   12+14},
	{"MCS15",   12+15},

	{"",        0},
};

/* BEGIN: Added by zoucaihong, 2014/5/13 */
extern UINT32   g_ulRecvTimeout;    /* 设备与CMS保活超时时间*/
extern UINT32   g_ulCmsRecvTime;    /* 记录设备正确接收到CMS发送的SNMP报文的时间 */
/* END: Added by zoucaihong, 2014/5/13 */

extern UINT8 g_aucDevMac[6];
extern int g_trap_port;
/************************************************************************/
extern int sem_timedwait(sem_t *, const struct timespec *);
extern BOOL agent_send_apswupdatetrap(VOID);

/************************************************************************/
/*                                                                      */
/************************************************************************/
UINT8 snmp_get_asn1_type(UINT8 ucColumnType)
{
	switch(ucColumnType)
	{
		case TYPE_CHAR:
			return BER_TYPE_OCTET_STRING;    /* 字符串，多字节 */
			
		case TYPE_BYTE:
			return BER_TYPE_INTEGER;    
			
		case TYPE_SHORT:
			return BER_TYPE_INTEGER;
			
		case TYPE_LONG:
			return BER_TYPE_INTEGER;
			
		case TYPE_BOOL:
			return BER_TYPE_BOOLEAN;    /* 对应TruthValue */
			
		case TYPE_INT:
			return BER_TYPE_INTEGER;
			
		case TYPE_BIN:
			return BER_TYPE_OCTET_STRING;
			
		case TYPE_IP:
			return BER_TYPE_IP;
			
		case TYPE_MAC:
			return BER_TYPE_OCTET_STRING;
			
		case TYPE_TIME:
			return BER_TYPE_TIME_TICKS;
			
		case TYPE_IPV6:
			return BER_TYPE_OCTET_STRING;
			
		case TYPE_OID:
			return BER_TYPE_OID;
			
		case TYPE_ROWSTATUS:
			return BER_TYPE_INTEGER;
			
		case TYPE_TADDR:
			return BER_TYPE_OCTET_STRING;
			
		case TYPE_LONGLONG:
			return BER_TYPE_COUNTER64;
			
		case TYPE_COUNTER32:
			return BER_TYPE_COUNTER;
			
		case TYPE_GAUGE32:
			return BER_TYPE_GAUGE;
			
		default:
	        return BER_TYPE_OCTET_STRING;
	}
		
}

#ifdef __LINUX__
sem_t * sem_alloc(void)
{
	return malloc(sizeof(sem_t));
}

void sem_free(sem_t *ptSem)
{
	free((void*)ptSem);
}
#endif


/*====================================================================
函数名          : OsCreateSemaphore
功能            : 创建信号量
算法实现        :
引用全局变量    : 无
输入参数说明    :
    LPBYTE lpbyName  -- 信号量名称
    WORD32 dwInitVal -- 信号量初始值
    WORD32 dwMaxVal  -- 信号量最大值
    WORD32 dwFlag    -- 信号量标志
    OSTOKEN *ptSmID  -- 信号量ID
返回值说明      : BOOL
====================================================================*/
BOOL OsCreateSemaphore (LPBYTE lpbyName,
                        WORD32 dwInitVal,
                        WORD32 dwMaxVal,
                        WORD32 dwFlag,
                        OSTOKEN *ptSmID)
{
#ifdef _OSWIN32_
     switch (dwFlag)
    {
    case SEM_COUNT: /* 计数信号量 */
    case SEM_SYNC:  /* 同步信号量 */
        *ptSmID = (OSTOKEN)CreateSemaphore (NULL, dwInitVal, dwMaxVal, (CHAR*)lpbyName);
        break;
    case SEM_MUTEX: /* 互斥信号量 */
        *ptSmID = (OSTOKEN)CreateMutex (NULL, FALSE, (CHAR*)lpbyName);
        break;
    default:
        *ptSmID = 0;
        break;
    }

    if (*ptSmID == 0)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
#endif

#ifdef __LINUX__
	int ret;

	*ptSmID = (OSTOKEN)sem_alloc();
	if(*ptSmID == (OSTOKEN)NULL)
	{
	}

	switch (dwFlag)
    {
    case SEM_COUNT: /* 计数信号量 */
        ret = sem_init ((sem_t*)(*ptSmID), 0, dwInitVal);
        break;
    case SEM_SYNC: /* 同步信号量 */
        ret = sem_init ((sem_t*)(*ptSmID), 0, 0);
        break;
    case SEM_MUTEX: /* 互斥信号量 */
        ret = sem_init((sem_t*)(*ptSmID), 0, 1);
        break;
    default:
        return FALSE;
    }

    if (ret != 0)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
#endif /* _LINUX_ */
}


/*====================================================================
函数名          : OsCloseSemaphore
功能            : 关闭信号量
算法实现        :
引用全局变量    : 无
输入参数说明    : OSTOKEN tSmID -- 信号量ID
返回值说明      : BOOL
====================================================================*/
BOOL OsCloseSemaphore (OSTOKEN tSmID)
{
#ifdef _OSWIN32_
    return CloseHandle ((HANDLE)tSmID);
#endif

#ifdef __LINUX__
    if (sem_destroy ((sem_t*)tSmID) == -1)
    {
		sem_free((sem_t*)tSmID);
        return FALSE;
    }
    else
    {
		sem_free((sem_t*)tSmID);
        return TRUE;
    }
#endif
}

/*====================================================================
函数名          : OsSemaphoreP
功能            : 信号量P操作
算法实现        :
引用全局变量    : 无
输入参数说明    :
    OSTOKEN tSmID      -- 信号量ID
    WORD32 dwTimeout   -- 超时(ms)
    WORD32 dwCount     -- P操作数目
返回值说明      : BOOL
====================================================================*/
BOOL OsSemaphoreP (OSTOKEN tSmID, WORD32 dwTimeout, WORD32 dwCount)
{
#ifdef _OSWIN32_
    WORD32    dwRet;

	if(dwCount != 1)
	{
	    return FALSE;
	}
    dwRet = WaitForSingleObject ((HANDLE)tSmID, dwTimeout);
    if (dwRet == WAIT_OBJECT_0)
    {
        return TRUE;
    }

    if (dwRet == WAIT_TIMEOUT)
    {
        return FALSE;
    }

    return FALSE;
#endif

#ifdef __LINUX__
	struct timespec timeout;

	if(dwCount != 1)
	{
	    return FALSE;
	}
	if (dwTimeout == WAIT_FOREVER)
	{
		if(0 == sem_wait((sem_t*)tSmID))
			return TRUE;
	}
	else
	{
		if (dwTimeout == NO_WAIT)
	    {
			timeout.tv_sec = 0;
			timeout.tv_nsec = 0;
	    }
		else
		{
			clock_gettime(CLOCK_REALTIME, &timeout);
			timeout.tv_sec += dwTimeout/1000;
			timeout.tv_nsec += (dwTimeout%1000)*1000000;
		}

		if (0 == sem_timedwait((sem_t*)tSmID, &timeout))
		{
			return TRUE;
		}
		else
		{
			if (errno == ETIMEDOUT)
			{
				return FALSE;
			}
		}
	}

    return FALSE;
#endif
}


/*====================================================================
函数名          : OsSemaphoreV
功能            : 信号量V操作
算法实现        :
引用全局变量    : 无
输入参数说明    :
    OSTOKEN tSmID   -- 信号量ID
    WORD32 dwCount  -- V操作数目
返回值说明      : BOOL
====================================================================*/
BOOL OsSemaphoreV (OSTOKEN tSmID, WORD32 dwCount)
{
#ifdef _OSWIN32_
    if (ReleaseSemaphore ((HANDLE)tSmID, dwCount, NULL) == 0) /* 对非互斥信号量的处理 */
    {
        if (ERROR_INVALID_HANDLE == GetLastError ()) /* 如果出错，则是互斥信号量 */
        {
            if (0 == ReleaseMutex ((HANDLE)tSmID))
            {
                return FALSE;
            }
            else
            {
                return TRUE;
            }
        }
        return FALSE;
    }

    return TRUE;
#endif

#ifdef __LINUX__
	int i;

	for(i=0; i<dwCount; i++)
	{
		if(sem_post((sem_t*)tSmID) != 0)
		{
			return FALSE;
		}
	}

	return TRUE;
#endif
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
UINT32 snmp_get_uptime(VOID)
{
#if defined(_OSWIN32_)
    static WORD32  ulStartTicks = 0;
	
    if (0 == ulStartTicks)
    {
        ulStartTicks = GetTickCount();
        return 0;
    }
	
    return (WORD32) ((GetTickCount() - ulStartTicks) / 1000);
	
#elif defined(__LINUX__)
	static unsigned long ulStartTime = 0;
	struct sysinfo info;

	if (0 == ulStartTime)
	{
		if( 0 == sysinfo( &info ) )
		{
			ulStartTime = (unsigned long)info.uptime;
		}

		return 0;
	}

	if( 0 == sysinfo( &info ) )
	{
		return (DWORD)(info.uptime - ulStartTime);
	}
	else
	{
		return 0;
	}   
#endif
}


/*************************************************************************
* 函数名称: snmp_get_systime
* 功    能: 获取系统时间
* 参    数: 
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明: 
*************************************************************************/
UINT32 snmp_get_systime(VOID)
{
    UINT32  ulTime = time(NULL);

    return ulTime;
}


/*************************************************************************
* 函数名称: snmp_get_table_reget_status
* 功    能: 获取表记录数据
* 参    数: 
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明: 
*************************************************************************/
BOOL snmp_get_table_reget_status(SNMP_TABLE_T *pstTable)
{
	UINT32 ulCurrent = 0;
	UINT32 ulLastChange = 0;
    /* BEGIN: Added by zoucaihong, 2012/12/25 */
    UINT32 ulPeriod = 0;//数据刷新周期
    /* END: Added by zoucaihong, 2012/12/25 */

	if (pstTable->ulTableID>=MAX_TABLE_NUM)
	{
		return FALSE;
	}
	
	if (NULL == pstTable)
	{
		agent_printf(LOG_ERR, "snmp_get_table_reget_status() pstTable is NULL.\n");
		return FALSE;
	}

	ulLastChange = g_aulTableRegetCheck[pstTable->ulTableID];
	ulCurrent = snmp_get_systime();
    /* BEGIN: Modified by zoucaihong, 2012/12/25 */
    if ( 0 == g_stati_per )
    {
        ulPeriod = AGENT_REFRESH_PERIOD_DFTVAL;
    }
    else
    {
        ulPeriod = g_stati_per;
    }
	if(0 == ulLastChange ||
		(ulCurrent>ulLastChange && ulCurrent-ulLastChange>ulPeriod))
	{
		return TRUE;
	}
    /* END: Modified by zoucaihong, 2012/12/25 */

	return FALSE;
}

/*************************************************************************
* 函数名称: snmp_set_table_reget_status
* 功    能: 获取表记录数据
* 参    数: 
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明: 
*************************************************************************/
BOOL snmp_set_table_reget_status(SNMP_TABLE_T *pstTable)
{
	UINT32 ulCurrent = 0;

	if (pstTable->ulTableID>=MAX_TABLE_NUM)
	{
		return FALSE;
	}
	
	ulCurrent = snmp_get_systime();
	g_aulTableRegetCheck[pstTable->ulTableID] = ulCurrent;
	
	return TRUE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL snmp_reset_table_reget_status(UINT32 ulTableID)
{
	if (ulTableID>=MAX_TABLE_NUM)
	{
		return FALSE;
	}

	g_aulTableRegetCheck[ulTableID] = 0;
	return TRUE;
}

/*************************************************************************
* 函数名称: snmp_get_table_record
* 功    能: set/add操作相关动态数据置位
* 参    数: 
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明: 
*************************************************************************/
UINT8* snmp_get_table_record (SNMP_TABLE_T *pstTable)
{	
 	UINT8 *pucData = NULL;
 	UINT32 ulTableID = 0;	
	BOOL   bRet = FALSE;

	ulTableID = pstTable->ulTableID;
    agent_printf(LOG_ERR,"snmp_get_table_record ulTableID:%u.",ulTableID);
	switch(ulTableID)
 	{
 	    #if 0
        /* BEGIN: Added by zoucaihong, 2012/12/4 
        add systerm group and interfaces group of RFC1213*/
        case TABLE_ID_SYSTEM:
            bRet = agent_get_system();
            if ( FALSE == bRet )
            {
                agent_printf(LOG_ERR, "agent_get_system failed!\n");
            }
            else
            {
                agent_printf(LOG_INFO, "agent_get_system succeed.\n");
            }
            
            pucData = (UINT8 *)(g_pstSnmpAgentData->pstSystem);
            break;
        case TABLE_ID_INTERFACES:
        case TABLE_ID_INTERFACESTABLE:
            bRet = agent_get_interfaces();
            if ( FALSE == bRet )
            {
                agent_printf(LOG_ERR, "agent_get_interfaces failed!\n");
            }
            else
            {
                agent_printf(LOG_INFO, "agent_get_interfaces succeed.\n");
            }
            UINT32 ulCurrentTime = 0;
            ulCurrentTime = snmp_get_systime();
			if (ulCurrentTime>g_pstSnmpAgentData->pstInterfaces[0].ulIfLastTime)
			{
				g_pstSnmpAgentData->pstInterfaces[0].ulIfLastChange = 
					ulCurrentTime-g_pstSnmpAgentData->pstInterfaces[0].ulIfLastTime;
				g_pstSnmpAgentData->pstInterfaces[0].ulIfLastChange *= 100; //10ms
			}

            if ( pstTable->ulTableID == TABLE_ID_INTERFACES )
            {
                pucData = (UINT8 *)&(g_pstSnmpAgentData->stInterfaces);
            }
            else if ( pstTable->ulTableID == TABLE_ID_INTERFACESTABLE )
            {
                pucData = (UINT8 *)(g_pstSnmpAgentData->pstInterfaces);
            }
            break;
        /* END: Added by zoucaihong, 2012/12/4 */
        #endif
        case TABLE_ID_AP_CONFIG_SYS_DEVINFO:
			bRet = ac_agent_get_apbasicsetup_config();
			if (FALSE == bRet)
			{
				agent_printf(LOG_ERR, "%s:snmp_get_table_record ac_agent_get_apbasicsetup_config failed\n",
					SNMPAGENT_CONTROL_NAME);
				//return NULL;
			}
			pucData = (UINT8 *)g_pstSnmpAgentData->pstSysDevInfo;
			break;
	
		case TABLE_ID_AP_CONFIG_SYS_SWINFO:
			bRet = ac_agent_get_apswinfo();
			if (FALSE == bRet)
			{
				agent_printf(LOG_ERR, "%s:snmp_get_table_record ac_agent_get_apswinfo failed\n",
					SNMPAGENT_CONTROL_NAME);
				//return NULL;
			}
			pucData = (UINT8 *)g_pstSnmpAgentData->pstSysSwInfo;
			break;
		case TABLE_ID_AP_CONFIG_SYS_HWINFO:
			bRet = ac_agent_get_aphwinfo();
			if (FALSE == bRet)
			{
				agent_printf(LOG_ERR, "%s:snmp_get_table_record ac_agent_get_aphwinfo failed\n",
					SNMPAGENT_CONTROL_NAME);
				//return NULL;
			}
			pucData = (UINT8 *)g_pstSnmpAgentData->pstSysHwInfo;
			break;
        case TABLE_ID_AP_CONFIG_SYS_IP:
        case TABLE_ID_AP_CONFIG_SYS_IPTABLE:
            bRet = ac_agent_get_apipsetup_config();
            if ( FALSE == bRet )
            {
                lprintf(LOG_ERR, "ac_agent_get_apipsetup_config failed.");
            }
			if(pstTable->ulTableID == TABLE_ID_AP_CONFIG_SYS_IP)
			{
				pucData = (UINT8 *)&(g_pstSnmpAgentData->stSysIpConfig);
			}
			else if(pstTable->ulTableID == TABLE_ID_AP_CONFIG_SYS_IPTABLE)
			{
				pucData = (UINT8 *)(g_pstSnmpAgentData->pstSysIpConfig);
			}
            break;
        #if 0    
        /* BEGIN: Added by zoucaihong, 2013/5/13 */
        case TABLE_ID_AP_CONFIG_SYS_LEGALAP:
        case TABLE_ID_AP_CONFIG_SYS_LEGALAPTABLE:
            bRet = ac_agent_get_aplegalap_config();
            if ( FALSE == bRet )
            {
                lprintf(LOG_ERR, "ac_agent_get_aplegalap_config failed.");
            }
			if(pstTable->ulTableID == TABLE_ID_AP_CONFIG_SYS_LEGALAP)
			{
				pucData = (UINT8 *)&(g_pstSnmpAgentData->stSysLegalApConfig);
			}
			else if(pstTable->ulTableID == TABLE_ID_AP_CONFIG_SYS_LEGALAPTABLE)
			{
				pucData = (UINT8 *)(g_pstSnmpAgentData->pstSysLegalApConfig);
			}
            break;
		case TABLE_ID_AP_CONFIG_SYS_FLOWCTL:
			bRet = ac_agent_get_apflowctl_config();
			if (FALSE == bRet)
			{
				agent_printf(LOG_ERR, "ac_agent_get_apflowctl_config failed\n");
				//return NULL;
			}
			pucData = (UINT8 *)g_pstSnmpAgentData->pstSysFlowCtl;
			break;

        /* END: Added by zoucaihong, 2013/5/13 */
        /* BEGIN: Added by zoucaihong, 2013/3/11 */
		case TABLE_ID_AP_CONFIG_ADVANCE_5GPRIOR:
			bRet = ac_agent_get_ap5gpriorsetup_config();
			if (FALSE == bRet)
			{
				agent_printf(LOG_ERR, "%s:snmp_get_table_record ac_agent_get_ap5gpriorsetup_config failed\n",
					SNMPAGENT_CONTROL_NAME);
				//return NULL;
			}
			pucData = (UINT8 *)g_pstSnmpAgentData->pstAdvance5GPrior;
			break;
            
		case TABLE_ID_AP_CONFIG_ADVANCE_11NPRIOR:
			bRet = ac_agent_get_ap11npriorsetup_config();
			if (FALSE == bRet)
			{
				agent_printf(LOG_ERR, "%s:snmp_get_table_record ac_agent_get_ap11npriorsetup_config failed\n",
					SNMPAGENT_CONTROL_NAME);
				//return NULL;
			}
			pucData = (UINT8 *)g_pstSnmpAgentData->pstAdvance11nPrior;
			break;
        /* END: Added by zoucaihong, 2013/3/11 */
	    #endif
#ifndef GW_X86
		case TABLE_ID_AP_CONFIG_WIFIBASIC:
            bRet = ac_agent_get_apwifidevicebasic_config();
            if ( FALSE == bRet )
            {
                lprintf(LOG_ERR, "ac_agent_get_apwifidevicebasic_config failed.");
            }
			pucData = (UINT8 *)(g_pstSnmpAgentData->pstApWifiBasicConfig);
            break;
		case TABLE_ID_AP_CONFIG_WIFICHANNEL:
            bRet = ac_agent_get_apwifidevicechannel_config();
            if ( FALSE == bRet )
            {
                lprintf(LOG_ERR, "ac_agent_get_apwifidevicechannel_config failed.");
            }
            pucData = (UINT8 *)(g_pstSnmpAgentData->pstApWifiChannelConfig);
            break;
		case TABLE_ID_AP_CONFIG_WIFIPOWER:
			bRet = ac_agent_get_apwifidevicepower_config();
			if (FALSE == bRet)
			{
				lprintf(LOG_ERR, "ac_agent_get_apwifidevicepower_config failed.");
			}
			pucData = (UINT8 *)(g_pstSnmpAgentData->pstApWifiPowerConfig);
			break;
        /* BEGIN: Added by zoucaihong, 2013/1/21 */
		case TABLE_ID_AP_CONFIG_WIFIADVANCE:
			bRet = ac_agent_get_apwifideviceadvance_config();
			if (FALSE == bRet)
			{
				lprintf(LOG_ERR, "ac_agent_get_apwifideviceadvance_config failed.");
			}
			pucData = (UINT8 *)(g_pstSnmpAgentData->pstApWifiAdvanceConfig);
			break;
        /* END: Added by zoucaihong, 2013/1/21 */
        /* BEGIN: Added by Zhou Shouya, 2013/8/29 */
		case TABLE_ID_AP_CONFIG_WIFIRATE:
			bRet = ac_agent_get_apwifidevicerate_config();
			if (FALSE == bRet)
			{
				lprintf(LOG_ERR, "ac_agent_get_apwifidevicerate_config failed.");
			}
			pucData = (UINT8 *)(g_pstSnmpAgentData->pstApWifiRateConfig);
			break;
        /* END:   Added by Zhou Shouya, 2013/8/29 */
        /* BEGIN: Added by zoucaihong, 2013/3/21 */
        /* 2.4G卡配置和5G卡配置分开 */
		case TABLE_ID_AP_CONFIG_WIFIBASIC5:
            bRet = ac_agent_get_apwifidevicebasic5_config();
            if ( FALSE == bRet )
            {
                lprintf(LOG_ERR, "ac_agent_get_apwifidevicebasic5_config failed.");
            }
			pucData = (UINT8 *)(g_pstSnmpAgentData->pstApWifiBasic5Config);
            break;
		case TABLE_ID_AP_CONFIG_WIFICHANNEL5:
            bRet = ac_agent_get_apwifidevicechannel5_config();
            if ( FALSE == bRet )
            {
                lprintf(LOG_ERR, "ac_agent_get_apwifidevicechannel5_config failed.");
            }
            pucData = (UINT8 *)(g_pstSnmpAgentData->pstApWifiChannel5Config);
            break;
		case TABLE_ID_AP_CONFIG_WIFIPOWER5:
			bRet = ac_agent_get_apwifidevicepower5_config();
			if (FALSE == bRet)
			{
				lprintf(LOG_ERR, "ac_agent_get_apwifidevicepower5_config failed.");
			}
			pucData = (UINT8 *)(g_pstSnmpAgentData->pstApWifiPower5Config);
			break;
		case TABLE_ID_AP_CONFIG_WIFIADVANCE5:
			bRet = ac_agent_get_apwifideviceadvance5_config();
			if (FALSE == bRet)
			{
				lprintf(LOG_ERR, "ac_agent_get_apwifideviceadvance5_config failed.");
			}
			pucData = (UINT8 *)(g_pstSnmpAgentData->pstApWifiAdvance5Config);
			break;
        /* END: Added by zoucaihong, 2013/3/21 */
        /* BEGIN: Added by Zhou Shouya, 2013/8/29 */
		case TABLE_ID_AP_CONFIG_WIFIRATE5:
			bRet = ac_agent_get_apwifidevicerate5_config();
			if (FALSE == bRet)
			{
				lprintf(LOG_ERR, "ac_agent_get_apwifidevicerate5_config failed.");
			}
			pucData = (UINT8 *)(g_pstSnmpAgentData->pstApWifiRate5Config);
			break;
        /* END:   Added by Zhou Shouya, 2013/8/29 */
		case TABLE_ID_AP_CONFIG_VAP:
		case TABLE_ID_AP_CONFIG_VAPBASICTABLE:
			bRet = ac_agent_get_apvapbasicsetup_config();
			if (FALSE == bRet)
			{
				agent_printf(LOG_ERR, "%s:snmp_get_table_record ac_agent_get_apvapbasicsetup_config failed\n",
					SNMPAGENT_CONTROL_NAME);
				//return NULL;
			}
			if(pstTable->ulTableID == TABLE_ID_AP_CONFIG_VAP)
			{
				pucData = (UINT8 *)&(g_pstSnmpAgentData->stApVapConfig);
			}
			else if(pstTable->ulTableID == TABLE_ID_AP_CONFIG_VAPBASICTABLE)
			{
				pucData = (UINT8 *)(g_pstSnmpAgentData->pstApVapBasicConfig);
			}
			break;
		case TABLE_ID_AP_CONFIG_VAPSECURITYTABLE:
			bRet = ac_agent_get_apvapsecuritysetup_config();
			if (FALSE == bRet)
			{
				agent_printf(LOG_ERR, "%s:snmp_get_table_record ac_agent_get_apvapsecuritysetup_config failed\n",
					SNMPAGENT_CONTROL_NAME);
				//return NULL;
			}
            pucData = (UINT8 *)(g_pstSnmpAgentData->pstApVapSecurityConfig);
            break;
		case TABLE_ID_AP_CONFIG_ALARM:
			bRet = ac_agent_get_apalarmsetup_config();
			if (FALSE == bRet)
			{
				agent_printf(LOG_ERR, "%s:snmp_get_table_record ac_agent_get_apalarmsetup_config failed\n",
					SNMPAGENT_CONTROL_NAME);
				//return NULL;
			}
			pucData = (UINT8 *)g_pstSnmpAgentData->pstApAlarmConfig;
			break;
		case TABLE_ID_AP_CONFIG_TRAPDES:
		//case TABLE_ID_AP_CONFIG_TRAPDES_TABLE:
			bRet = ac_agent_get_aptrapdessetup_config();
			if (FALSE == bRet)
			{
				agent_printf(LOG_ERR, "%s:snmp_get_table_record ac_agent_get_aptrapdessetup_config failed\n",
					SNMPAGENT_CONTROL_NAME);
				//return NULL;
			}
			//pucData = (UINT8 *)&(g_pstSnmpAgentData->pstApTrapDesConfig);
			pucData = (UINT8 *)(g_pstSnmpAgentData->pstApTrapDesConfig);
			#if 0
			else if(pstTable->ulTableID == TABLE_ID_AP_CONFIG_TRAPDES_TABLE)
			{
				pucData = (UINT8 *)(g_pstSnmpAgentData->pstApTrapDesConfig);
			}
			#endif
			break;
        /* BEGIN: Added by zoucaihong, 2012/11/21 */
		case TABLE_ID_AP_CONFIG_VLAN:
		case TABLE_ID_AP_CONFIG_VLAN_TABLE:
			bRet = ac_agent_get_apvlansetup_config();
			if (FALSE == bRet)
			{
				agent_printf(LOG_ERR, "%s:snmp_get_table_record ac_agent_get_apvlansetup_config failed\n",
					SNMPAGENT_CONTROL_NAME);
			}
			if(pstTable->ulTableID == TABLE_ID_AP_CONFIG_VLAN)
			{
				pucData = (UINT8 *)&(g_pstSnmpAgentData->stApVlanConfig);
			}
			else if(pstTable->ulTableID == TABLE_ID_AP_CONFIG_VLAN_TABLE)
			{
				pucData = (UINT8 *)(g_pstSnmpAgentData->pstApVlanConfig);
			}
			break;
        /* END: Added by zoucaihong, 2012/11/21 */
        /* BEGIN: Added by zoucaihong, 2013/3/1 */
		case TABLE_ID_AP_CONFIG_SSID:
		case TABLE_ID_AP_CONFIG_SSID_TABLE:
			bRet = ac_agent_get_apssid_setup();
			if (FALSE == bRet)
			{
				agent_printf(LOG_ERR, "%s:snmp_get_table_record ac_agent_get_apssid_setup failed\n",
					SNMPAGENT_CONTROL_NAME);
			}
			if(pstTable->ulTableID == TABLE_ID_AP_CONFIG_SSID)
			{
				pucData = (UINT8 *)&(g_pstSnmpAgentData->stApSsidConfig);
			}
			else if(pstTable->ulTableID == TABLE_ID_AP_CONFIG_SSID_TABLE)
			{
				pucData = (UINT8 *)(g_pstSnmpAgentData->pstApSsidConfig);
			}
			break;
        /* END: Added by zoucaihong, 2013/3/1 */
		case TABLE_ID_AP_STAT_SYS:
			bRet = ac_agent_get_apsysstat();
			if (FALSE == bRet)
			{
				agent_printf(LOG_ERR, "%s:snmp_get_table_record ac_agent_get_apsysstat failed\n",
					SNMPAGENT_CONTROL_NAME);
				//return NULL;
			}
			pucData = (UINT8 *)g_pstSnmpAgentData->pstSysStat;
			break;
		case TABLE_ID_AP_STAT_WIFI:
		case TABLE_ID_AP_STAT_WIFITABLE:
		case TABLE_ID_AP_STAT_WIFIVAP:
		case TABLE_ID_AP_STAT_WIFIVAPTABLE:
			bRet = ac_agent_get_apwifistat();
			bRet = FALSE;
			if (FALSE == bRet)
			{
				agent_printf(LOG_ERR, "%s:snmp_get_table_record ac_agent_get_apwifistat failed\n",
    				SNMPAGENT_CONTROL_NAME);
				//return NULL;
			}
            
			if(pstTable->ulTableID == TABLE_ID_AP_STAT_WIFI)
			{
				pucData = (UINT8 *)&(g_pstSnmpAgentData->stApWifiStat);
			}
			else if(pstTable->ulTableID == TABLE_ID_AP_STAT_WIFITABLE)
			{
				pucData = (UINT8 *)(g_pstSnmpAgentData->pstApWifiStat);
			}
			else if(pstTable->ulTableID == TABLE_ID_AP_STAT_WIFIVAP)
			{
				pucData = (UINT8 *)&(g_pstSnmpAgentData->stApWifiVapStat);
			}
			else if(pstTable->ulTableID == TABLE_ID_AP_STAT_WIFIVAPTABLE)
			{
				pucData = (UINT8 *)(g_pstSnmpAgentData->pstApWifiVapStat);
			}
			break;
		case TABLE_ID_AP_STAT_STATION:
		case TABLE_ID_AP_STAT_STATIONTABLE:
			bRet = ac_agent_get_apstationstat();
			if (FALSE == bRet)
			{
				agent_printf(LOG_ERR, "%s:snmp_get_table_record ac_agent_get_apstationstat failed\n",
    				SNMPAGENT_CONTROL_NAME);
			}
            
			if(pstTable->ulTableID == TABLE_ID_AP_STAT_STATION)
			{
				pucData = (UINT8 *)&(g_pstSnmpAgentData->stApStationStat);
			}
			else if(pstTable->ulTableID == TABLE_ID_AP_STAT_STATIONTABLE)
			{
				pucData = (UINT8 *)(g_pstSnmpAgentData->pstApStationStat);
			}
			break;
#endif
#if 0
        /* BEGIN: Added by zoucaihong, 2013/1/22 */
        /* AP 2.4G卡同频干扰 */
		case TABLE_ID_AP_STAT_24COCHAINTERFINFO:
		case TABLE_ID_AP_STAT_24COCHAINTERFINFOTABLE:
			bRet = ac_agent_get_ap24cochainterfinfo();
			if (FALSE == bRet)
			{
				agent_printf(LOG_ERR, "%s:snmp_get_table_record ac_agent_get_ap24cochainterfinfo failed\n",
    				SNMPAGENT_CONTROL_NAME);
			}
            
			if(pstTable->ulTableID == TABLE_ID_AP_STAT_24COCHAINTERFINFO)
			{
				pucData = (UINT8 *)&(g_pstSnmpAgentData->stAp24CoChaInterfInfo);
			}
			else if(pstTable->ulTableID == TABLE_ID_AP_STAT_24COCHAINTERFINFOTABLE)
			{
				pucData = (UINT8 *)(g_pstSnmpAgentData->pstAp24CoChaInterfInfo);
			}
			break;

        /* AP 5G卡同频干扰 */
		case TABLE_ID_AP_STAT_5COCHAINTERFINFO:
		case TABLE_ID_AP_STAT_5COCHAINTERFINFOTABLE:
			bRet = ac_agent_get_ap5cochainterfinfo();
			if (FALSE == bRet)
			{
				agent_printf(LOG_ERR, "%s:snmp_get_table_record ac_agent_get_ap5cochainterfinfo failed\n",
    				SNMPAGENT_CONTROL_NAME);
			}
            
			if(pstTable->ulTableID == TABLE_ID_AP_STAT_5COCHAINTERFINFO)
			{
				pucData = (UINT8 *)&(g_pstSnmpAgentData->stAp5CoChaInterfInfo);
			}
			else if(pstTable->ulTableID == TABLE_ID_AP_STAT_5COCHAINTERFINFOTABLE)
			{
				pucData = (UINT8 *)(g_pstSnmpAgentData->pstAp5CoChaInterfInfo);
			}
			break;

        /* AP 2.4G卡邻频干扰 */    
		case TABLE_ID_AP_STAT_24ADJCHAINTERFINFO:
		case TABLE_ID_AP_STAT_24ADJCHAINTERFINFOTABLE:
			bRet = ac_agent_get_ap24adjchainterfinfo();
			if (FALSE == bRet)
			{
				agent_printf(LOG_ERR, "%s:snmp_get_table_record ac_agent_get_ap24adjchainterfinfo failed\n",
    				SNMPAGENT_CONTROL_NAME);
			}
            
			if(pstTable->ulTableID == TABLE_ID_AP_STAT_24ADJCHAINTERFINFO)
			{
				pucData = (UINT8 *)&(g_pstSnmpAgentData->stAp24AdjChaInterfInfo);
			}
			else if(pstTable->ulTableID == TABLE_ID_AP_STAT_24ADJCHAINTERFINFOTABLE)
			{
				pucData = (UINT8 *)(g_pstSnmpAgentData->pstAp24AdjChaInterfInfo);
			}
			break;

        /* AP 5G卡邻频干扰 */    
		case TABLE_ID_AP_STAT_5ADJCHAINTERFINFO:
		case TABLE_ID_AP_STAT_5ADJCHAINTERFINFOTABLE:
			bRet = ac_agent_get_ap5adjchainterfinfo();
			if (FALSE == bRet)
			{
				agent_printf(LOG_ERR, "%s:snmp_get_table_record ac_agent_get_ap5adjchainterfinfo failed\n",
    				SNMPAGENT_CONTROL_NAME);
			}
            
			if(pstTable->ulTableID == TABLE_ID_AP_STAT_5ADJCHAINTERFINFO)
			{
				pucData = (UINT8 *)&(g_pstSnmpAgentData->stAp5AdjChaInterfInfo);
			}
			else if(pstTable->ulTableID == TABLE_ID_AP_STAT_5ADJCHAINTERFINFOTABLE)
			{
				pucData = (UINT8 *)(g_pstSnmpAgentData->pstAp5AdjChaInterfInfo);
			}
			break;

        /* AP 2.4G卡WLAN设备干扰 */    
		case TABLE_ID_AP_STAT_24WLANDEVINTERFINFO:
		case TABLE_ID_AP_STAT_24WLANDEVINTERFINFOTABLE:
			bRet = ac_agent_get_ap24wlandevinterfinfo();
			if (FALSE == bRet)
			{
				agent_printf(LOG_ERR, "%s:snmp_get_table_record ac_agent_get_ap24wlandevinterfinfo failed\n",
    				SNMPAGENT_CONTROL_NAME);
			}
            
			if(pstTable->ulTableID == TABLE_ID_AP_STAT_24WLANDEVINTERFINFO)
			{
				pucData = (UINT8 *)&(g_pstSnmpAgentData->stAp24WlanDevInterfInfo);
			}
			else if(pstTable->ulTableID == TABLE_ID_AP_STAT_24WLANDEVINTERFINFOTABLE)
			{
				pucData = (UINT8 *)(g_pstSnmpAgentData->pstAp24WlanDevInterfInfo);
			}
			break;

        /* AP 5G卡WLAN设备干扰 */    
		case TABLE_ID_AP_STAT_5WLANDEVINTERFINFO:
		case TABLE_ID_AP_STAT_5WLANDEVINTERFINFOTABLE:
			bRet = ac_agent_get_ap5wlandevinterfinfo();
			if (FALSE == bRet)
			{
				agent_printf(LOG_ERR, "%s:snmp_get_table_record ac_agent_get_ap5wlandevinterfinfo failed\n",
    				SNMPAGENT_CONTROL_NAME);
			}
            
			if(pstTable->ulTableID == TABLE_ID_AP_STAT_5WLANDEVINTERFINFO)
			{
				pucData = (UINT8 *)&(g_pstSnmpAgentData->stAp5WlanDevInterfInfo);
			}
			else if(pstTable->ulTableID == TABLE_ID_AP_STAT_5WLANDEVINTERFINFOTABLE)
			{
				pucData = (UINT8 *)(g_pstSnmpAgentData->pstAp5WlanDevInterfInfo);
			}
			break;

        /* BEGIN: Added by zoucaihong, 2013/5/10 */
        /* AP 2.4G卡非法AP检测列表 */    
		case TABLE_ID_AP_STAT_24ROGUEAPINFO:
		case TABLE_ID_AP_STAT_24ROGUEAPINFOTABLE:
			bRet = ac_agent_get_ap24rogueapinfo();
			if (FALSE == bRet)
			{
				agent_printf(LOG_ERR, "%s:snmp_get_table_record ac_agent_get_ap24rogueapinfo failed\n",
    				SNMPAGENT_CONTROL_NAME);
			}
            
			if(pstTable->ulTableID == TABLE_ID_AP_STAT_24ROGUEAPINFO)
			{
				pucData = (UINT8 *)&(g_pstSnmpAgentData->stAp24RogueApInfo);
			}
			else if(pstTable->ulTableID == TABLE_ID_AP_STAT_24ROGUEAPINFOTABLE)
			{
				pucData = (UINT8 *)(g_pstSnmpAgentData->pstAp24RogueApInfo);
			}
			break;

        /* AP 5G卡非法AP检测列表 */    
		case TABLE_ID_AP_STAT_5ROGUEAPINFO:
		case TABLE_ID_AP_STAT_5ROGUEAPINFOTABLE:
			bRet = ac_agent_get_ap5rogueapinfo();
			if (FALSE == bRet)
			{
				agent_printf(LOG_ERR, "%s:snmp_get_table_record ac_agent_get_ap5rogueapinfo failed\n",
    				SNMPAGENT_CONTROL_NAME);
			}
            
			if(pstTable->ulTableID == TABLE_ID_AP_STAT_5ROGUEAPINFO)
			{
				pucData = (UINT8 *)&(g_pstSnmpAgentData->stAp5RogueApInfo);
			}
			else if(pstTable->ulTableID == TABLE_ID_AP_STAT_5ROGUEAPINFOTABLE)
			{
				pucData = (UINT8 *)(g_pstSnmpAgentData->pstAp5RogueApInfo);
			}
			break;        
        /* END: Added by zoucaihong, 2013/5/10 */
            
		case TABLE_ID_AP_STAT_NONWLANDEVINTERFINFO:
		case TABLE_ID_AP_STAT_NONWLANDEVINTERFINFOTABLE:
			bRet = ac_agent_get_apnonwlandevinterfinfo();
			if (FALSE == bRet)
			{
				agent_printf(LOG_ERR, "%s:snmp_get_table_record ac_agent_get_apnonwlandevinterfinfo failed\n",
    				SNMPAGENT_CONTROL_NAME);
			}
			if(pstTable->ulTableID == TABLE_ID_AP_STAT_NONWLANDEVINTERFINFO)
			{
				pucData = (UINT8 *)&(g_pstSnmpAgentData->stApNonWlanDevInterfInfo);
			}
			else if(pstTable->ulTableID == TABLE_ID_AP_STAT_NONWLANDEVINTERFINFOTABLE)
			{
				pucData = (UINT8 *)(g_pstSnmpAgentData->pstApNonWlanDevInterfInfo);
			}
			break;
		case TABLE_ID_AP_STAT_BRMODESTA:
		case TABLE_ID_AP_STAT_BRMODESTATABLE:
			bRet = ac_agent_get_apbrmodestainfo();
			if (FALSE == bRet)
			{
				agent_printf(LOG_ERR, "%s:snmp_get_table_record ac_agent_get_apbrmodestainfo failed\n",
    				SNMPAGENT_CONTROL_NAME);
			}
            
			if(pstTable->ulTableID == TABLE_ID_AP_STAT_BRMODESTA)
			{
				pucData = (UINT8 *)&(g_pstSnmpAgentData->stApBrModeSta);
			}
			else if(pstTable->ulTableID == TABLE_ID_AP_STAT_BRMODESTATABLE)
			{
				pucData = (UINT8 *)(g_pstSnmpAgentData->pstApBrModeSta);
			}
			break;
        /* END: Added by zoucaihong, 2013/1/22 */
#endif
        /* BEGIN: Added by zoucaihong, 2014/5/15 */
		case TABLE_ID_AP_OPER_SYSOPER:
			pucData = (UINT8 *)(g_pstSnmpAgentData->pstApSysOper);
			break;
		case TABLE_ID_AP_OPER_FILETRANS:
			pucData = (UINT8 *)(g_pstSnmpAgentData->pstApFileTransConfig);
			break;
        /* END: Added by zoucaihong, 2014/5/15 */

		default:
			break;
	}

	return pucData;
}

 VOID ap_agent_convert_u32_to_u64(UINT32 ulHigh, UINT32 ulLow, UINT8 *pucU64)
 {
 	if (NULL == pucU64)
 	{
 		return;
 	}
    
    #ifdef BIGENDIAN 
	memcpy(pucU64, (UINT8 *)(&ulHigh), 4);
	memcpy(pucU64+4, (UINT8 *)(&ulLow), 4);
    #else
	memcpy(pucU64, (UINT8 *)(&ulLow), 4);
	memcpy(pucU64+4, (UINT8 *)(&ulHigh), 4);
 	#endif
 	
 	return;
 }

UINT32 ap_agent_generate_trapsn(VOID)
{
	static UINT32 ulTrapSn = 1;
	return (ulTrapSn++);
}
CHAR* ap_agent_convert_time_str(UINT32 ulTime)
{
	struct tm * time = NULL;

	if (ulTime == 0)
	{
		agent_printf(LOG_INFO, "ap_agent_convert_time_str:ulTime is 0\n");
		return "";
	}

	time = localtime((const time_t *)&(ulTime));
	if (time == NULL)
	{
		agent_printf(LOG_ERR, "ap_agent_convert_time_str:ulTime:%u localtime() return NULL\n",
			ulTime);
		return "";
	}

	return asctime(time);
}
 BOOL ac_agent_check_num(char *sz)
 {
 	char *p = NULL;
 	
 	if (sz == NULL)
 	{
 		agent_printf(LOG_ERR, "ac_agent_check_number() sz is null.\n");
 		return FALSE;
 	}
 
 	if (strlen(sz) == 0)
 	{
 		agent_printf(LOG_ERR, "ac_agent_check_number() sz length is 0.\n");
 		return FALSE;
 	}
 
 	if (*sz == '0' && strlen(sz) != 1)
 	{
 		agent_printf(LOG_ERR, "ac_agent_check_number() sz:%s is invalid.\n",
 			sz);
 		return FALSE;
 	}
 	
 	p = sz;
 	while(*p != '\0')
 	{
 		if (*p < '0' || *p > '9')
 		{
 			agent_printf(LOG_ERR, "ac_agent_check_number() sz:%s is invalid.\n", sz);
 			return FALSE;
 		}
 
 		p++;
 	}
 
 	return TRUE;
 }

BOOL ap_agent_parse_conf_file(VOID)
#if 0
{
	FILE    *pfData = NULL;	
	UINT32  ulIndex = 0;
	UINT32	ulLoop = 0;
    UINT32  ulCommunityId = 0;
    UINT32  i = 0;
    CHAR    *szFileName= SNMPAGENT_FILE_CONF;
	CHAR	*pucPos = NULL;
	CHAR	*pucStart = NULL;
    CHAR    *szTemp = NULL;
    CHAR    acBuf[128] = {0};
    
	for(ulLoop = 0; ulLoop < 5; ulLoop++)
	{
		pfData = fopen (szFileName, "rb");
		if (NULL != pfData)
		{
			break;
		}
	}

	if (NULL == pfData)
	{
		agent_printf(LOG_ERR, "%s:read snmp agent conf file:%s failed, use default community.\n",
			SNMPAGENT_CONTROL_NAME,
			szFileName);
        memcpy(g_astSnmpV2Config[0].acReadCommunity, AGENT_RO_COMMUNITY,
            sizeof(g_astSnmpV2Config[0].acReadCommunity));
        memcpy(g_astSnmpV2Config[0].acWriteCommunity, AGENT_RW_COMMUNITY,
            sizeof(g_astSnmpV2Config[0].acWriteCommunity));
		return FALSE;
	}

    while ( fgets(acBuf, sizeof(acBuf)-1, pfData) )
    {
        acBuf[sizeof(acBuf)-1] = '\0';

        /* 去除行尾空格 */
        for (i=strlen(acBuf)-1; i >= 0 && IS_EMPTY_CHAR(acBuf[i]); i--)
        {
            acBuf[i] = '\0';
        }

        /* 跳过空格 */
        for(szTemp = acBuf; IS_EMPTY_CHAR(szTemp[0]); szTemp ++);

        /* 判断是否是空行 */
        if (szTemp[0] == '\0')
        {
            continue;
        }

    	// 查询rwcommunity
    	pucPos = strstr(acBuf, "rwcommunity");
    	if (NULL != pucPos)
    	{
        	sscanf(acBuf, "rwcommunity%u", &ulCommunityId);
            if ( 0 != ulCommunityId )
            {
                ulCommunityId--;
            }
    		ulIndex = 0;
    		pucStart = pucPos+strlen("rwcommunity1");
    		while(*pucStart == ' ')
    		{
    			pucStart++;
    		}
    		
    		if (*pucStart != '\n')
    		{
    			while((*pucStart != ' ') && (*pucStart != '\n') && (*pucStart != '\0')&& (*pucStart != '\r'))
    			{
    				g_astSnmpV2Config[ulCommunityId].acWriteCommunity[ulIndex++] = *pucStart;
    				pucStart++;
    			}
                
    			g_astSnmpV2Config[ulCommunityId].acWriteCommunity[ulIndex] = '\0';
                lprintf(LOG_INFO, "g_astSnmpV2Config[%u].acWriteCommunity:%s",
                    ulCommunityId, g_astSnmpV2Config[ulCommunityId].acWriteCommunity);
    		}    		
    	}

    	// 查询rocommunity
    	pucPos = strstr(acBuf, "rocommunity");
    	if (NULL != pucPos)
    	{
    	    sscanf(acBuf, "rocommunity%u", &ulCommunityId);
            if ( 0 != ulCommunityId )
            {
                ulCommunityId--;
            }
    		ulIndex = 0;
    		pucStart = pucPos+strlen("rocommunity1");
    		while(*pucStart == ' ')
    		{
    			pucStart++;
    		}
    				
    		if (*pucStart != '\n')
    		{
    			while((*pucStart != ' ') && (*pucStart != '\n') && (*pucStart != '\0')&& (*pucStart != '\r'))
    			{
    				g_astSnmpV2Config[ulCommunityId].acReadCommunity[ulIndex++] = *pucStart;
    				pucStart++;
    			}

    			g_astSnmpV2Config[ulCommunityId].acReadCommunity[ulIndex] = '\0';
                lprintf(LOG_INFO, "g_astSnmpV2Config[%u].acReadCommunity:%s",
                    ulCommunityId, g_astSnmpV2Config[ulCommunityId].acReadCommunity);
    		}
        }   
    }
    
    fclose(pfData);
			
	return TRUE;
}
#else
{
    memcpy(g_stSnmpV2Config.acReadCommunity, AGENT_RO_COMMUNITY,
        sizeof(g_stSnmpV2Config.acReadCommunity));
    if ( NULL != g_pstSnmpAgentData->pstSysDevInfo )
    {
        sprintf(g_stSnmpV2Config.acWriteCommunity, "%02X:%02X:%02X:%02X:%02X:%02X", 
            MAC_ARG(g_pstSnmpAgentData->pstSysDevInfo->aucSysMacAddr));
    }
    else
    {
        memcpy(g_stSnmpV2Config.acWriteCommunity, AGENT_RW_COMMUNITY,
            sizeof(g_stSnmpV2Config.acWriteCommunity));
    }
    return TRUE;
}
#endif

#ifndef GW_X86
/*****************************************************************************
 Prototype    : agent_modify_vap_flow
 Description  : VAP流控配置生效函数
 Input        : CHAR acDevName            
                UINT32 ulFlowSwitch       
                UINT32 ulUplinkMaxRate    
                UINT32 ulDownlinkMaxRate  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/2/26
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL agent_modify_vap_flow(CHAR *pacDevName, UINT32 ulFlowSwitch, UINT32 ulUplinkMaxRate, UINT32 ulDownlinkMaxRate)
{
    CHAR acStrCmd[128] = {0};
    char acNvramValue[32];
    /* 配置流控命令 */
    /* 开关为关闭状态， 则不配置流控配置（保证清空之前的配置） */
    nvram_get(acNvramValue, "apFlowLevel", 32);
    if(0 == atoi(acNvramValue))
    {
        system("flowctrl delap");
        system("flowctrl rule vap multi");
        nvram_set("apFlowSwitch", "0");
        nvram_set("apFlowLevel","1");
    }
    
    if (0 == ulFlowSwitch)
    {
        /* 清空之前的配置 */
        sprintf(acStrCmd, "flowctrl delvap %s", pacDevName);
        system(acStrCmd);
    }
    else
    {
        sprintf(acStrCmd, "flowctrl addvap %s %d %d %d %d",
            pacDevName,
            ulUplinkMaxRate,
            ulUplinkMaxRate < 5000 ? 5000 : ulUplinkMaxRate,
            ulDownlinkMaxRate,
            ulDownlinkMaxRate < 5000 ? 5000 : ulDownlinkMaxRate);
        system(acStrCmd);
    }

    return TRUE;
}
#endif
BOOL agent_parse_wep_key_format(UINT32 ulWepKeyFormat, UINT32 *pulFormat, UINT32 *pulLen)
{
    /* 检查参数合法性 */
    if ( NULL == pulFormat )
    {
        agent_printf(LOG_ERR, "pulFormat is null.\n");
        return FALSE;
    }

    if ( NULL == pulLen )
    {
        agent_printf(LOG_ERR, "pulLen is null.\n");
        return FALSE;
    }

    /*
    wep-64bit-16hex(0),wep-64bit-ascii(1),wep-128bit-16hex(2),wep-128bit-ascii(3)
    */
    switch ( ulWepKeyFormat )
    {
        case 0 :
            *pulFormat = HEX;
            *pulLen = BIT64;
            break;
        case 1 :
            *pulFormat = ASSIC;
            *pulLen = BIT64;
            break;
        case 2 :
            *pulFormat = HEX;
            *pulLen = BIT128;
            break;
        case 3 :
            *pulFormat = ASSIC;
            *pulLen = BIT128;
            break;
        default:
            agent_printf(LOG_ERR, "ulWepKeyFormat:%u is invalid.\n", ulWepKeyFormat);
            return FALSE;
    }

    return TRUE;
}
BOOL agent_check_key_validty(CHAR *szKey)
{
    CHAR *pcPos = NULL;
    
    /* 参数合法性检查 */
    if ( NULL == szKey )
    {
        agent_printf(LOG_ERR, "szKey is null.\n");
        return FALSE;
    }

    if ( 0 != strlen(szKey) )
    {
        if ( NULL != (pcPos = strstr(szKey, " ")) )
        {
            agent_printf(LOG_ERR, "szKey:%s invalid.\n", szKey);
            return FALSE;
        }
    }

    return TRUE;
}
BOOL agent_check_wep_key_len(UINT32 ulWepKeyFormat, UINT32 ulWepBit, CHAR *szWepKey)
{
    /* 参数合法性校验 */
    if ( NULL == szWepKey )
    {
        agent_printf(LOG_ERR, "szWepKey is null.\n");
        return FALSE;
    }

    /* wepkey取值范围:
       64位ASCII密钥5位，64位16进制密钥10位，
       128位ASCII密钥13位，128位16进制密钥26位 */
    if ( 0 != strlen(szWepKey) )
    {
        switch ( ulWepKeyFormat )
        {
            case ASSIC :
                if ( BIT64 == ulWepBit )
                {
                    if ( strlen(szWepKey)!=5 )
                    {
                        agent_printf(LOG_ERR, "assic-64bit szWepKey len:%u isn't 5.\n", strlen(szWepKey));
                        return FALSE;
                    }
                }
                else
                {
                    if ( strlen(szWepKey)!=13 )
                    {
                        agent_printf(LOG_ERR, "assic-128bit szWepKey len:%u isn't 13.\n", strlen(szWepKey));
                        return FALSE;
                    }
                }
                break;
            case HEX :
                if ( BIT64 == ulWepBit )
                {
                    if ( strlen(szWepKey)!=10 )
                    {
                        agent_printf(LOG_ERR, "hex-64bit szWepKey len:%u isn't 10.\n", strlen(szWepKey));
                        return FALSE;
                    }
                }
                else
                {
                    if ( strlen(szWepKey)!=26 )
                    {
                        agent_printf(LOG_ERR, "hex-128bit szWepKey len:%u isn't 26.\n", strlen(szWepKey));
                        return FALSE;
                    }
                }
                break;
                break;
            default:
                agent_printf(LOG_ERR, "ulWepKeyFormat:%u is invalid.\n", ulWepBit);
                return FALSE;
        }
    }
    return TRUE;
}
#ifndef GW_X86

#ifdef OPENWRT
BOOL agent_modify_vap_security( AP_CONFIG_VAPSECURITY_ENTRY_T *pstVapSecurity,
                                AP_CONFIG_VAPSECURITY_ENTRY_T *pstConfig, CHAR *pacDevName)
{
    char ssid_name[32];
    UINT32 ulWepKeyFormat = 0;
    UINT32 ulWepBit = 0;
    char nvram_name[128];
    char nvram_value[128];


    /* 参数合法性检查 */
    if ( NULL == pstVapSecurity )
    {
        agent_printf(LOG_ERR, "pstVapSecurity is null.\n");
        return FALSE;
    }

    if ( NULL == pstConfig )
    {
        agent_printf(LOG_ERR, "pstConfig is null.\n");
        return FALSE;
    }

    /* 根据加密模式来做相应配置 */
    switch(pstVapSecurity->ulSecurityMode)
    {
        case OPEN:
            /* 认证类型:disabled(1),wep(2) */
            
            if(pstVapSecurity->ulAuthenticationType == 1)
            {
                sprintf(nvram_name,"wireless.@wifi-iface[%d].encryption",pstVapSecurity->ulIndex);
                uci_config_set(nvram_name,"none");
            }
            else if(pstVapSecurity->ulAuthenticationType == 2)
            {
                sprintf(nvram_name,"wireless.@wifi-iface[%d].encryption",pstVapSecurity->ulIndex);
                uci_config_set(nvram_name,"wep-open");

                sprintf(nvram_name,"wireless.@wifi-iface[%d].key",pstVapSecurity->ulIndex);
                sprintf(nvram_value,"%d",pstVapSecurity->ulWepKeyFormat);
                uci_config_set(nvram_name,nvram_value);
                sprintf(nvram_name,"wireless.@wifi-iface[%d].key%d",pstVapSecurity->ulIndex,pstVapSecurity->ulWepKeyFormat);
                uci_config_set(nvram_name,pstVapSecurity->acWepKey);
                
            }
            break;
            
        case SHARED:
           
                sprintf(nvram_name,"wireless.@wifi-iface[%d].encryption",pstVapSecurity->ulIndex);
                uci_config_set(nvram_name,"wep-shared");

                sprintf(nvram_name,"wireless.@wifi-iface[%d].key",pstVapSecurity->ulIndex);
                sprintf(nvram_value,"%d",pstVapSecurity->ulWepKeyFormat);
                uci_config_set(nvram_name,nvram_value);
                sprintf(nvram_name,"wireless.@wifi-iface[%d].key%d",pstVapSecurity->ulIndex,pstVapSecurity->ulWepKeyFormat);
                uci_config_set(nvram_name,pstVapSecurity->acWepKey);
                
            break;
        case WPA_PSK:
                
                sprintf(nvram_name,"wireless.@wifi-iface[%d].encryption",pstVapSecurity->ulIndex);
                /*AES*/
                if(pstVapSecurity->ulEncryptType==0){
                    
                    uci_config_set(nvram_name,"psk+ccmp");
                }
                else if(pstVapSecurity->ulEncryptType==1)
                {
                    uci_config_set(nvram_name,"psk+tkip");
                }
                sprintf(nvram_name,"wireless.@wifi-iface[%d].key",pstVapSecurity->ulIndex);
                uci_config_set(nvram_name,pstVapSecurity->acWpaKey);
            
        case WPA2_PSK:
                sprintf(nvram_name,"wireless.@wifi-iface[%d].encryption",pstVapSecurity->ulIndex);
                /*AES*/
                if(pstVapSecurity->ulEncryptType==0){
                    
                    uci_config_set(nvram_name,"psk2+ccmp");
                }
                else if(pstVapSecurity->ulEncryptType==1)
                {
                    uci_config_set(nvram_name,"psk2+tkip");
                }
                sprintf(nvram_name,"wireless.@wifi-iface[%d].key",pstVapSecurity->ulIndex);
                uci_config_set(nvram_name,pstVapSecurity->acWpaKey);
            
        case WPA_PSK_WPA2_PSK:
                sprintf(nvram_name,"wireless.@wifi-iface[%d].encryption",pstVapSecurity->ulIndex);
                /*AES*/
                if(pstVapSecurity->ulEncryptType==0){
                    
                    uci_config_set(nvram_name,"psk-mixed+ccmp");
                }
                else if(pstVapSecurity->ulEncryptType==1)
                {
                    uci_config_set(nvram_name,"psk-mixed+tkip");
                }
                sprintf(nvram_name,"wireless.@wifi-iface[%d].key",pstVapSecurity->ulIndex);
                uci_config_set(nvram_name,pstVapSecurity->acWpaKey);

            break;
            /*openwrt not support now*/
            #if 0
        case WPA:
        case WPA2:
        case WPA_WPA2:
            /* 加密机制:none(0),tkip(1),aes(2),auto(3) */
            if ( pstVapSecurity->ulEncryptType > 3 )
            {
                agent_printf(LOG_ERR, "ulEncryptType:%u invalid.\n", pstVapSecurity->ulEncryptType);
                return FALSE;
            }
            st_en.u.st_wpa_ent.type = pstVapSecurity->ulEncryptType;
            memcpy(st_en.u.st_wpa_ent.auth_ip, pstVapSecurity->aucRadiusAddr, 4);
            st_en.u.st_wpa_ent.auth_port = pstVapSecurity->ulRadiusPort;
            /* BEGIN: Added by Zhou Shouya, 2013/8/6   PN:VLAN */
            if ( 0 != memcmp(pstVapSecurity->acRadiusSecu, pstConfig->acRadiusSecu, sizeof(pstVapSecurity->acRadiusSecu)) )
            {
                /* 增加密钥校验 */
                if ( FALSE == agent_check_key_validty(pstVapSecurity->acRadiusSecu) )
                {
                    agent_printf(LOG_ERR, "agent_check_key_validty fail.\n");
                    return FALSE;
                }
            }
            /* END:   Added by Zhou Shouya, 2013/8/6   PN:VLAN */
            memcpy(st_en.u.st_wpa_ent.auth_key, pstVapSecurity->acRadiusSecu, 64);
            
            memcpy(st_en.u.st_wpa_ent.acct_ip, pstVapSecurity->aucAccountAddr, 4);
            st_en.u.st_wpa_ent.acct_port = pstVapSecurity->ulAccountPort;
            /* BEGIN: Added by Zhou Shouya, 2013/8/6   PN:VLAN */
            if ( 0 != memcmp(pstVapSecurity->acAccountSecu, pstConfig->acAccountSecu, sizeof(pstVapSecurity->acAccountSecu)) )
            {
                /* 增加密钥校验 */
                if ( FALSE == agent_check_key_validty(pstVapSecurity->acAccountSecu) )
                {
                    agent_printf(LOG_ERR, "agent_check_key_validty fail.\n");
                    return FALSE;
                }
            }
            /* END:   Added by Zhou Shouya, 2013/8/6   PN:VLAN */
            memcpy(st_en.u.st_wpa_ent.acct_key, pstVapSecurity->acAccountSecu, 64);
            /* BEGIN: Added by Zhou Shouya, 2013/8/7   PN:VLAN */
            /* radius服务器超时时间:1～10s，默认3s */
            if ( 0 == pstVapSecurity->ulRadiusAgingTime )
            {
                pstVapSecurity->ulRadiusAgingTime = 3;
            }
            else if ( pstVapSecurity->ulRadiusAgingTime > 10 )
            {
                agent_printf(LOG_ERR, "ulRadiusAgingTime:%u invalid.\n", pstVapSecurity->ulRadiusAgingTime);
                return FALSE;
            }
            /* END:   Added by Zhou Shouya, 2013/8/7   PN:VLAN */
            st_en.u.st_wpa_ent.send_interval = pstVapSecurity->ulRadiusAgingTime;
            /* BEGIN: Added by Zhou Shouya, 2013/8/8   PN:VLAN */
            /* 计费服务器间隔,>=10s */
            if ( 0 != pstVapSecurity->ulAccountInterval
                && pstVapSecurity->ulAccountInterval < 10)
            {
                agent_printf(LOG_ERR, "ulAccountInterval:%u invalid.\n", pstVapSecurity->ulAccountInterval);
                return FALSE;
            }
            st_en.u.st_wpa_ent.acct_interim = pstVapSecurity->ulAccountInterval;
            /* END:   Added by Zhou Shouya, 2013/8/8   PN:VLAN */
            break;
            #endif
        default:
            agent_printf(LOG_ERR, "ulSecurityMode:%u invalid.\n", pstVapSecurity->ulSecurityMode);
            return FALSE;
    }

    uci_config_commit();

    /*apply right now?*/
}

BOOL agent_parse_auto_channel( UINT32 ulOutDoor, UINT32 ulAbgMode, CHAR *szAutoChanList )
{

}

BOOL agent_checkvalid_auto_channel( UINT32 ulAbgMode, CHAR *szAutoChanList )
{
}
#else
/*****************************************************************************
 Prototype    : agent_modify_vap_security
 Description  : 修改VAP加密配置函数
 Input        : AP_CONFIG_VAPSECURITY_ENTRY_T *pstVapSecurity  
                CHAR *pacDevName                               
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/2/26
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL agent_modify_vap_security( AP_CONFIG_VAPSECURITY_ENTRY_T *pstVapSecurity,
                                AP_CONFIG_VAPSECURITY_ENTRY_T *pstConfig, CHAR *pacDevName)
{
    char ssid_name[32];
    UINT32 ulWepKeyFormat = 0;
    UINT32 ulWepBit = 0;

    encrypt st_en;
    un_vaps st_vap;

    /* 参数合法性检查 */
    if ( NULL == pstVapSecurity )
    {
        agent_printf(LOG_ERR, "pstVapSecurity is null.\n");
        return FALSE;
    }

    if ( NULL == pstConfig )
    {
        agent_printf(LOG_ERR, "pstConfig is null.\n");
        return FALSE;
    }

    memset(&st_en, 0, sizeof(encrypt));
    st_en.auth_type = pstVapSecurity->ulSecurityMode;
    /* 根据加密模式来做相应配置 */
    switch(pstVapSecurity->ulSecurityMode)
    {
        case OPEN:
            /* 认证类型:disabled(1),wep(2) */
            if ( pstVapSecurity->ulAuthenticationType < 1
                || pstVapSecurity->ulAuthenticationType > 2)
            {
                agent_printf(LOG_ERR, "ulAuthenticationType:%u invalid.\n", pstVapSecurity->ulAuthenticationType);
                return FALSE;
            }
            st_en.u.st_open.type = pstVapSecurity->ulAuthenticationType;
            /* BEGIN: Added by Zhou Shouya, 2013/8/6   PN:VLAN */
            /* WEP密钥类型转换 */
            if ( FALSE == agent_parse_wep_key_format(pstVapSecurity->ulWepKeyFormat, &ulWepKeyFormat, &ulWepBit) )
            {
                agent_printf(LOG_ERR, "agent_parse_wep_key_format ulWepKeyFormat:%u fail.\n",
                    pstVapSecurity->ulWepKeyFormat);
                return FALSE;
            }
            /* END:   Added by Zhou Shouya, 2013/8/6   PN:VLAN */
            st_en.u.st_open.st_wep.format = ulWepKeyFormat;
            st_en.u.st_open.st_wep.len = ulWepBit;
            if ( 0 != memcmp(pstVapSecurity->acWepKey, pstConfig->acWepKey, sizeof(pstVapSecurity->acWepKey)) )
            {
                /* wep密钥长度检查 */
                if ( FALSE == agent_check_wep_key_len(ulWepKeyFormat, ulWepBit, pstVapSecurity->acWepKey))
                {
                    agent_printf(LOG_ERR, "agent_check_wep_key_len fail.\n");
                    return FALSE;
                }
            }
            memcpy(st_en.u.st_open.st_wep.key, pstVapSecurity->acWepKey, 32);
            break;
        case SHARED:
            /* BEGIN: Added by Zhou Shouya, 2013/8/6   PN:VLAN */
            /* WEP密钥类型转换 */
            if ( FALSE == agent_parse_wep_key_format(pstVapSecurity->ulWepKeyFormat, &ulWepKeyFormat, &ulWepBit) )
            {
                agent_printf(LOG_ERR, "agent_parse_wep_key_format ulWepKeyFormat:%u fail.\n",
                    pstVapSecurity->ulWepKeyFormat);
                return FALSE;
            }
            /* END:   Added by Zhou Shouya, 2013/8/6   PN:VLAN */
            st_en.u.st_shared.st_wep.format = ulWepKeyFormat;
            st_en.u.st_shared.st_wep.len = ulWepBit;
            if ( 0 != memcmp(pstVapSecurity->acWepKey, pstConfig->acWepKey, sizeof(pstVapSecurity->acWepKey)) )
            {
                /* wep密钥长度检查 */
                if ( FALSE == agent_check_wep_key_len(ulWepKeyFormat, ulWepBit, pstVapSecurity->acWepKey))
                {
                    agent_printf(LOG_ERR, "agent_check_wep_key_len fail.\n");
                    return FALSE;
                }
            }
            memcpy(st_en.u.st_shared.st_wep.key, pstVapSecurity->acWepKey, 32);
            break;
        case WPA_PSK:
        case WPA2_PSK:
        case WPA_PSK_WPA2_PSK:
            /* 加密机制:none(0),tkip(1),aes(2),auto(3) */
            if ( pstVapSecurity->ulEncryptType > 3 )
            {
                agent_printf(LOG_ERR, "ulEncryptType:%u invalid.\n", pstVapSecurity->ulEncryptType);
                return FALSE;
            }
            st_en.u.st_wpa_per.type = pstVapSecurity->ulEncryptType;
            /* BEGIN: Added by Zhou Shouya, 2013/8/7   PN:VLAN */
            /* 增加WPA KEY更新周期检查 */
            if ( 0 == pstVapSecurity->ulWpaKeyRenewal )
            {
                pstVapSecurity->ulWpaKeyRenewal = 3600;//默认值为1小时(3600s)
            }
            else if( pstVapSecurity->ulWpaKeyRenewal < 10*60 || pstVapSecurity->ulWpaKeyRenewal > 24*60*60 )
            {
                agent_printf(LOG_ERR, "ulWpaKeyRenewal:%u out range of 10min-24h.\n", pstVapSecurity->ulWpaKeyRenewal);
                return FALSE;
            }
            /* END:   Added by Zhou Shouya, 2013/8/7   PN:VLAN */
            st_en.u.st_wpa_per.renew = pstVapSecurity->ulWpaKeyRenewal;
            /* BEGIN: Added by Zhou Shouya, 2013/8/6   PN:VLAN */
            /* 增加密钥长度检查,密钥长度为8-63 */
            if ( 0 != strlen(pstVapSecurity->acWpaKey)
            && (strlen(pstVapSecurity->acWpaKey)<8 || strlen(pstVapSecurity->acWpaKey)>63) )
            {
                agent_printf(LOG_ERR, "acWpaKey len:%u is out range of 8-63.\n",
                    strlen(pstVapSecurity->acWpaKey));
                return FALSE;
            }
            /* END:   Added by Zhou Shouya, 2013/8/6   PN:VLAN */
            memcpy(st_en.u.st_wpa_per.key, pstVapSecurity->acWpaKey, 64);
            break;
        case WPA:
        case WPA2:
        case WPA_WPA2:
            /* 加密机制:none(0),tkip(1),aes(2),auto(3) */
            if ( pstVapSecurity->ulEncryptType > 3 )
            {
                agent_printf(LOG_ERR, "ulEncryptType:%u invalid.\n", pstVapSecurity->ulEncryptType);
                return FALSE;
            }
            st_en.u.st_wpa_ent.type = pstVapSecurity->ulEncryptType;
            memcpy(st_en.u.st_wpa_ent.auth_ip, pstVapSecurity->aucRadiusAddr, 4);
            st_en.u.st_wpa_ent.auth_port = pstVapSecurity->ulRadiusPort;
            /* BEGIN: Added by Zhou Shouya, 2013/8/6   PN:VLAN */
            if ( 0 != memcmp(pstVapSecurity->acRadiusSecu, pstConfig->acRadiusSecu, sizeof(pstVapSecurity->acRadiusSecu)) )
            {
                /* 增加密钥校验 */
                if ( FALSE == agent_check_key_validty(pstVapSecurity->acRadiusSecu) )
                {
                    agent_printf(LOG_ERR, "agent_check_key_validty fail.\n");
                    return FALSE;
                }
            }
            /* END:   Added by Zhou Shouya, 2013/8/6   PN:VLAN */
            memcpy(st_en.u.st_wpa_ent.auth_key, pstVapSecurity->acRadiusSecu, 64);
            
            memcpy(st_en.u.st_wpa_ent.acct_ip, pstVapSecurity->aucAccountAddr, 4);
            st_en.u.st_wpa_ent.acct_port = pstVapSecurity->ulAccountPort;
            /* BEGIN: Added by Zhou Shouya, 2013/8/6   PN:VLAN */
            if ( 0 != memcmp(pstVapSecurity->acAccountSecu, pstConfig->acAccountSecu, sizeof(pstVapSecurity->acAccountSecu)) )
            {
                /* 增加密钥校验 */
                if ( FALSE == agent_check_key_validty(pstVapSecurity->acAccountSecu) )
                {
                    agent_printf(LOG_ERR, "agent_check_key_validty fail.\n");
                    return FALSE;
                }
            }
            /* END:   Added by Zhou Shouya, 2013/8/6   PN:VLAN */
            memcpy(st_en.u.st_wpa_ent.acct_key, pstVapSecurity->acAccountSecu, 64);
            /* BEGIN: Added by Zhou Shouya, 2013/8/7   PN:VLAN */
            /* radius服务器超时时间:1～10s，默认3s */
            if ( 0 == pstVapSecurity->ulRadiusAgingTime )
            {
                pstVapSecurity->ulRadiusAgingTime = 3;
            }
            else if ( pstVapSecurity->ulRadiusAgingTime > 10 )
            {
                agent_printf(LOG_ERR, "ulRadiusAgingTime:%u invalid.\n", pstVapSecurity->ulRadiusAgingTime);
                return FALSE;
            }
            /* END:   Added by Zhou Shouya, 2013/8/7   PN:VLAN */
            st_en.u.st_wpa_ent.send_interval = pstVapSecurity->ulRadiusAgingTime;
            /* BEGIN: Added by Zhou Shouya, 2013/8/8   PN:VLAN */
            /* 计费服务器间隔,>=10s */
            if ( 0 != pstVapSecurity->ulAccountInterval
                && pstVapSecurity->ulAccountInterval < 10)
            {
                agent_printf(LOG_ERR, "ulAccountInterval:%u invalid.\n", pstVapSecurity->ulAccountInterval);
                return FALSE;
            }
            st_en.u.st_wpa_ent.acct_interim = pstVapSecurity->ulAccountInterval;
            /* END:   Added by Zhou Shouya, 2013/8/8   PN:VLAN */
            break;
        default:
            agent_printf(LOG_ERR, "ulSecurityMode:%u invalid.\n", pstVapSecurity->ulSecurityMode);
            return FALSE;
    }
    memcpy(ssid_name, pstVapSecurity->acSsid, 32);
    
    vap_encrypt_config(&st_en, &st_en, pacDevName, ssid_name, 1);

    memset(&st_vap, 0, sizeof(un_vaps));
    st_vap.nvram_id=pstVapSecurity->ulIndex-1;
    st_vap.len = sizeof(encrypt);
    memcpy(&st_vap.u.st_en, &st_en, st_vap.len);
    vap_encrypt_nvram_set(&st_vap);
    
    return TRUE;
}
/*****************************************************************************
 Prototype    : agent_parse_auto_channel
 Description  : 检查动态信道合法值函数
 Input        : UINT32 ulOutDoor      
                UINT32 ulAbgMode      
                CHAR *szAutoChanList  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/2/26
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL agent_parse_auto_channel( UINT32 ulOutDoor, UINT32 ulAbgMode, CHAR *szAutoChanList )
{
 	CHAR *pcPos = NULL;
 	CHAR *pcOriPos = NULL;
 	CHAR    acTemp[5] = {0};
 	BOOL    bRet = FALSE;
 	UINT32 ulValue = 0;
 
 	if (NULL == szAutoChanList)
 	{
 		agent_printf(LOG_ERR, "ac_agent_parse_wifichannel() szAutoChanList is null.\n");
 		return FALSE;
 	}
 
 	
 	pcOriPos = szAutoChanList;
 	pcPos = strstr(pcOriPos, ",");
 	while(pcPos != NULL)
 	{
 		if ((UINT32)(pcPos-pcOriPos)>=5)
 		{
 			return FALSE;
 		}
 
 		memset(acTemp, 0, sizeof(acTemp));
 		memcpy(acTemp, pcOriPos, (UINT32)(pcPos-pcOriPos));
 
 		bRet = ac_agent_check_num(acTemp);
 		if (FALSE == bRet)
 		{
 			agent_printf(LOG_ERR, "agent_parse_auto_channel:acTemp :%s is not a number.\n", acTemp);
 			return FALSE;
 		}
 
 		ulValue = atoi(acTemp);
 
 		if (ulAbgMode == MODE_11A
 			|| ulAbgMode == MODE_11AN
 			|| ulAbgMode == MODE_11A_11AN
 			)
 		{
 		    if ( 0 == ulOutDoor )
 		    {
 		        if ( ulValue != ENUM_APWIFICHANNEL_CH52
                    && ulValue != ENUM_APWIFICHANNEL_CH56
                    && ulValue != ENUM_APWIFICHANNEL_CH60
                    && ulValue != ENUM_APWIFICHANNEL_CH64
                    && ulValue != ENUM_APWIFICHANNEL_CH100
                    && ulValue != ENUM_APWIFICHANNEL_CH104
                    && ulValue != ENUM_APWIFICHANNEL_CH108
                    && ulValue != ENUM_APWIFICHANNEL_CH112
                    && ulValue != ENUM_APWIFICHANNEL_CH116
                    && ulValue != ENUM_APWIFICHANNEL_CH132
                    && ulValue != ENUM_APWIFICHANNEL_CH136
                    && ulValue != ENUM_APWIFICHANNEL_CH140
                    && ulValue != ENUM_APWIFICHANNEL_CH149
                    && ulValue != ENUM_APWIFICHANNEL_CH153
                    && ulValue != ENUM_APWIFICHANNEL_CH157
                    && ulValue != ENUM_APWIFICHANNEL_CH161
                    && ulValue != ENUM_APWIFICHANNEL_CH165
                    )
 		        {
     				agent_printf(LOG_ERR, "agent_parse_auto_channel() inDoor 5G ulValue:%u is invalid.\n",
     					ulValue);
     				return FALSE;
 		        }
 		    }
            else if ( 1 == ulOutDoor )
            {
 		        if ( ulValue != ENUM_APWIFICHANNEL_CH100
                    && ulValue != ENUM_APWIFICHANNEL_CH104
                    && ulValue != ENUM_APWIFICHANNEL_CH108
                    && ulValue != ENUM_APWIFICHANNEL_CH112
                    && ulValue != ENUM_APWIFICHANNEL_CH116
                    && ulValue != ENUM_APWIFICHANNEL_CH132
                    && ulValue != ENUM_APWIFICHANNEL_CH136
                    && ulValue != ENUM_APWIFICHANNEL_CH140
                    && ulValue != ENUM_APWIFICHANNEL_CH149
                    && ulValue != ENUM_APWIFICHANNEL_CH153
                    && ulValue != ENUM_APWIFICHANNEL_CH157
                    && ulValue != ENUM_APWIFICHANNEL_CH161
                    && ulValue != ENUM_APWIFICHANNEL_CH165
                    )
 		        {
     				agent_printf(LOG_ERR, "agent_parse_auto_channel() outDoor 5G ulValue:%u is invalid.\n",
     					ulValue);
     				return FALSE;
 		        }
            }
        }
 		else if (ulAbgMode == MODE_11B 
 			|| ulAbgMode == MODE_11G 
 			|| ulAbgMode == MODE_11B_11G
 			|| ulAbgMode == MODE_11GN 
 			|| ulAbgMode == MODE_11G_11GN
 			|| ulAbgMode == MODE_11B_11G_11GN
 			)
 		{
 			if (ulValue < ENUM_APWIFICHANNEL_CH1 ||
 			    ulValue > ENUM_APWIFICHANNEL_CH11)
 			{
 				agent_printf(LOG_ERR, "agent_parse_auto_channel() 2.4G ulValue:%u is invalid.\n",
 					ulValue);
 				return FALSE;
 			}
 		}
 
 		pcOriPos= pcPos+1;
 		if(*pcOriPos == '\0')
 		{
 			break;
 		}
 		
 		pcPos = strstr(pcOriPos, ",");
 	}
 
 	memset(acTemp, 0, sizeof(acTemp));
 	strcpy(acTemp, pcOriPos);
 
 	bRet = ac_agent_check_num(acTemp);
 	if (FALSE == bRet)
 	{
 		agent_printf(LOG_ERR, "agent_parse_auto_channel:acTemp :%s is not a number.\n", acTemp);
 		return FALSE;
 	}
 
 	ulValue = atoi(acTemp);
 
 		if (ulAbgMode == MODE_11A
 			|| ulAbgMode == MODE_11AN
 			|| ulAbgMode == MODE_11A_11AN
 			)
 		{
 		    if ( 0 == ulOutDoor )
 		    {
 		        if ( ulValue != ENUM_APWIFICHANNEL_CH52
                    && ulValue != ENUM_APWIFICHANNEL_CH56
                    && ulValue != ENUM_APWIFICHANNEL_CH60
                    && ulValue != ENUM_APWIFICHANNEL_CH64
                    && ulValue != ENUM_APWIFICHANNEL_CH100
                    && ulValue != ENUM_APWIFICHANNEL_CH104
                    && ulValue != ENUM_APWIFICHANNEL_CH108
                    && ulValue != ENUM_APWIFICHANNEL_CH112
                    && ulValue != ENUM_APWIFICHANNEL_CH116
                    && ulValue != ENUM_APWIFICHANNEL_CH132
                    && ulValue != ENUM_APWIFICHANNEL_CH136
                    && ulValue != ENUM_APWIFICHANNEL_CH140
                    && ulValue != ENUM_APWIFICHANNEL_CH149
                    && ulValue != ENUM_APWIFICHANNEL_CH153
                    && ulValue != ENUM_APWIFICHANNEL_CH157
                    && ulValue != ENUM_APWIFICHANNEL_CH161
                    && ulValue != ENUM_APWIFICHANNEL_CH165
                    )
 		        {
     				agent_printf(LOG_ERR, "agent_parse_auto_channel() inDoor 5G ulValue:%u is invalid.\n",
     					ulValue);
     				return FALSE;
 		        }
 		    }
            else if ( 1 == ulOutDoor )
            {
 		        if ( ulValue != ENUM_APWIFICHANNEL_CH100
                    && ulValue != ENUM_APWIFICHANNEL_CH104
                    && ulValue != ENUM_APWIFICHANNEL_CH108
                    && ulValue != ENUM_APWIFICHANNEL_CH112
                    && ulValue != ENUM_APWIFICHANNEL_CH116
                    && ulValue != ENUM_APWIFICHANNEL_CH132
                    && ulValue != ENUM_APWIFICHANNEL_CH136
                    && ulValue != ENUM_APWIFICHANNEL_CH140
                    && ulValue != ENUM_APWIFICHANNEL_CH149
                    && ulValue != ENUM_APWIFICHANNEL_CH153
                    && ulValue != ENUM_APWIFICHANNEL_CH157
                    && ulValue != ENUM_APWIFICHANNEL_CH161
                    && ulValue != ENUM_APWIFICHANNEL_CH165
                    )
 		        {
     				agent_printf(LOG_ERR, "agent_parse_auto_channel() outDoor 5G ulValue:%u is invalid.\n",
     					ulValue);
     				return FALSE;
 		        }
            }
        }
 		else if (ulAbgMode == MODE_11B 
 			|| ulAbgMode == MODE_11G 
 			|| ulAbgMode == MODE_11B_11G
 			|| ulAbgMode == MODE_11GN 
 			|| ulAbgMode == MODE_11G_11GN
 			|| ulAbgMode == MODE_11B_11G_11GN
 			)
 		{
 			if (ulValue < ENUM_APWIFICHANNEL_CH1 ||
 			    ulValue > ENUM_APWIFICHANNEL_CH11)
 			{
 				agent_printf(LOG_ERR, "agent_parse_auto_channel() 2.4G ulValue:%u is invalid.\n",
 					ulValue);
 				return FALSE;
 			}
 		}
 	
 	return TRUE;
}
BOOL agent_checkvalid_auto_channel( UINT32 ulAbgMode, CHAR *szAutoChanList )
{
    CHAR acStrCmd[128] = {0};

    FILE *pfData = NULL;
    CHAR acBuff[128] = {0};
    UINT32 ulDevType = 0;
    UINT32 ulOutDoor = 0;
    BOOL bRet = FALSE;
    int iRet = -1;

    if ( NULL == szAutoChanList )
    {
        agent_printf(LOG_ERR, "szAutoChanList is null.");
        return FALSE;
    }

    sprintf(acStrCmd, "aptool getdevtype |grep \"device type\" > /tmp/apdevtype.tmp");
    iRet = system(acStrCmd);
    if ( 0 != iRet )
    {
        agent_printf(LOG_ERR, "get dev type failed.");
        return FALSE;
    }

    pfData = fopen("/tmp/apdevtype.tmp", "r");
    if (NULL == pfData)
    {
        agent_printf(LOG_ERROR, "open apdevtype.tmp fail");
        return FALSE;
    }
    if (NULL == fgets(acBuff, 128, pfData))
    {
        fclose(pfData);
        system("rm -f /tmp/apdevtype.tmp");
        agent_printf(LOG_ERROR, "read apdevtype.tmp fail");
        return FALSE;
    }
    fclose(pfData);
    sscanf(acBuff, "device type:%d", &ulDevType);
    agent_printf(LOG_INFO, "the devtype is:%u.", ulDevType);
    
    system("rm -f /tmp/apdevtype.tmp");

    if ( ulDevType == 4001008
        || ulDevType == 4001004
        || ulDevType == 9001001
        || ulDevType == 9001002
        || ulDevType == 9002001)
    {
        ulOutDoor = 0;
    }
    else if ( ulDevType == 5001006
        || ulDevType == 1002019
        || ulDevType == 5002010
        || ulDevType == 10001001
        || ulDevType == 10001002)
    {
        ulOutDoor = 1;
    }
    bRet = agent_parse_auto_channel(ulOutDoor, ulAbgMode, szAutoChanList);
    if ( FALSE == bRet )
    {
        agent_printf(LOG_ERR, "agent_parse_auto_channel fail.");
        return FALSE;
    }

    return TRUE;
}
#endif
#endif
/*****************************************************************************
 Prototype    : agent_modify_ap_ip
 Description  : 修改AP IP，生效配置并保存配置
 Input        : AP_CONFIG_IP_ENTRY_T *pstAPIpConfig  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/5/13
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL agent_modify_ap_ip(AP_CONFIG_IP_ENTRY_T *pstAPIpConfig, AP_CONFIG_IP_ENTRY_T *pstAPIpConfigOld)
{
    ipv4 ip_old, ip;

    if(NULL == pstAPIpConfig)
    {
        agent_printf(LOG_ERR, "pstAPIpConfig is null");
        return FALSE;
    }

    memset(&ip_old, 0, sizeof(ipv4));
    memset(&ip, 0, sizeof(ipv4));

    ip_old.ipmodule = pstAPIpConfigOld->ulIpMode;
    memcpy(ip_old.gateway, pstAPIpConfigOld->aucGwAddress, 4);
    memcpy(ip_old.dns[0], pstAPIpConfigOld->aucDnsMaster, 4);
    memcpy(ip_old.dns[1], pstAPIpConfigOld->aucDnsSlave, 4);
    memcpy(ip_old.ipaddr[0], pstAPIpConfigOld->aucIpAddress, 4);
    memcpy(ip_old.mask[0], pstAPIpConfigOld->aucIpNetMask, 4);

    ip.ipmodule = pstAPIpConfig->ulIpMode;
    if ( IP_MODEL_STATIC == ip.ipmodule )
    {
        memcpy(ip.gateway, pstAPIpConfig->aucGwAddress, 4);
        memcpy(ip.dns[0], pstAPIpConfig->aucDnsMaster, 4);
        memcpy(ip.dns[1], pstAPIpConfig->aucDnsSlave, 4);
        memcpy(ip.ipaddr[0], pstAPIpConfig->aucIpAddress, 4);
        memcpy(ip.mask[0], pstAPIpConfig->aucIpNetMask, 4);
    }

    #ifdef OPENWRT
    
    #else
    if(NvramResultSuccess != ipv4_nvram_set(&ip))
    {
        agent_printf(LOG_ERR, "ipv4 set failed for nvram.\n");
        return FALSE;
    }

    
    if ( OK != network_ipv4_config(&ip, &ip_old, DO_FLAG_OMC) )
    {
        agent_printf(LOG_ERR, "network_ipv4_config fail.\n");
        return FALSE;
    }
    #endif    

	return TRUE;
}
#if 0
BOOL agent_modify_ap_legalap(AP_CONFIG_LEGALAP_ENTRY_T *pstApLegalAp, AP_CONFIG_LEGALAP_ENTRY_T *OldpstApLegalAp)
{
    char nvrambssid[32];
    char nvrambssidvalue[32];
    char nvramssid[32];
    char nvramValue[32];
    int loop;

    for(loop = 0; loop<16; loop++)
    {
        sprintf(nvramssid, "legap_%d_ssid", loop);
        if(0==nvram_get(nvramValue, nvramssid, 32))
        {
            if(0==strcmp(OldpstApLegalAp->acLegalApSsid, nvramValue))
            {
                sprintf(nvrambssid, "legap_%d_oui", loop);
                sprintf(nvramValue, "%02x:%02x:%02x:%02x:%02x:%02x", 
                    OldpstApLegalAp->aucLegalApBssid[0],
                    OldpstApLegalAp->aucLegalApBssid[1],
                    OldpstApLegalAp->aucLegalApBssid[2],
                    OldpstApLegalAp->aucLegalApBssid[3],
                    OldpstApLegalAp->aucLegalApBssid[4],
                    OldpstApLegalAp->aucLegalApBssid[5]);
                nvram_get(nvrambssidvalue, nvrambssid, 32);
                if(0==strcmp(nvrambssidvalue, nvramValue))
                {
                    nvram_set(nvramssid, pstApLegalAp->acLegalApSsid);
                    sprintf(nvramValue, "%02x:%02x:%02x:%02x:%02x:%02x", 
                        pstApLegalAp->aucLegalApBssid[0],
                        pstApLegalAp->aucLegalApBssid[1],
                        pstApLegalAp->aucLegalApBssid[2],
                        pstApLegalAp->aucLegalApBssid[3],
                        pstApLegalAp->aucLegalApBssid[4],
                        pstApLegalAp->aucLegalApBssid[5]);
                    nvram_set(nvrambssid, nvramValue);
                    break;
                }
            }
        }
    }
    nvram_commit();
    return TRUE;
}
BOOL agent_delete_ap_legalap(AP_CONFIG_LEGALAP_ENTRY_T *pstApLegalAp)
{
    char nvrambssid[32];
    char nvrambssidvalue[32];
    char nvramssid[32];
    char nvramName[32];
    char nvramValue[32];
    int loop;

    for(loop = 0; loop<16; loop++)
    {
        sprintf(nvramssid, "legap_%d_ssid", loop);
        if(0==nvram_get(nvramValue, nvramssid, 32))
        {
            if(0==strcmp(pstApLegalAp->acLegalApSsid, nvramValue))
            {
                sprintf(nvrambssid, "legap_%d_oui", loop);
                sprintf(nvramValue, "%02x:%02x:%02x:%02x:%02x:%02x", 
                    pstApLegalAp->aucLegalApBssid[0],
                    pstApLegalAp->aucLegalApBssid[1],
                    pstApLegalAp->aucLegalApBssid[2],
                    pstApLegalAp->aucLegalApBssid[3],
                    pstApLegalAp->aucLegalApBssid[4],
                    pstApLegalAp->aucLegalApBssid[5]);
                nvram_get(nvrambssidvalue, nvrambssid, 32);
                if(0==strcmp(nvrambssidvalue, nvramValue))
                {
                    sprintf(nvramName, "legap_%d_nvram", loop);
                    nvram_unset(nvramName);
                    nvram_unset(nvrambssid);
                    nvram_unset(nvramssid);
                    break;
                }
            }
        }
    }
    nvram_commit();
    return TRUE;
}
BOOL agent_add_ap_legalap(AP_CONFIG_LEGALAP_ENTRY_T *pstApLegalAp)
{
    char nvrambssid[32];
    char nvramssid[32];
    char nvramName[32];
    char nvramValue[32];
    int loop;

    for(loop = 0; loop<16; loop++)
    {
        sprintf(nvrambssid, "legap_%d_oui", loop);
        if(0!=nvram_get(nvramValue, nvrambssid, 32))
        {
            sprintf(nvramssid, "legap_%d_ssid", loop);
            sprintf(nvramValue, "%02x:%02x:%02x:%02x:%02x:%02x", 
                pstApLegalAp->aucLegalApBssid[0],
                pstApLegalAp->aucLegalApBssid[1],
                pstApLegalAp->aucLegalApBssid[2],
                pstApLegalAp->aucLegalApBssid[3],
                pstApLegalAp->aucLegalApBssid[4],
                pstApLegalAp->aucLegalApBssid[5]);
            nvram_set(nvrambssid, nvramValue);
            nvram_set(nvramssid, pstApLegalAp->acLegalApSsid);
            
            sprintf(nvramValue, "%d", loop);
            sprintf(nvramName, "legap_%d_nvram", loop);
            nvram_set(nvramName, nvramValue);
            break;
        }
    }
    nvram_commit();
    return TRUE;
}
#endif
/* BEGIN: Added by Zhou Shouya, 2013/8/29 */
BOOL agent_check_transmit_speed_validity(UINT32 ulflag, CHAR *szRate, CHAR *szIndex)
{
 	CHAR    *pcPos = NULL;
 	CHAR    *pcOriPos = NULL;
 	CHAR    acTemp[6] = {0};
 	BOOL    bRet = FALSE;
 	UINT32  ulValue = 0;
 	UINT32  aulRate[] = {1,2,11,6,9,12,18,24,36,48,54};
 	UINT32  ulLoop = 0;
 	UINT32  ulSize = sizeof(aulRate)/sizeof(UINT32);
 	UINT32  ulId = 0;
 	RATE_DEF_T *pstRate = NULL;
 
    /* 参数合法性检查 */
    if ( NULL == szRate )
    {
        agent_printf(LOG_ERR, "szRate is null.\n");
        return FALSE;
    } 
    if ( NULL == szIndex )
    {
        agent_printf(LOG_ERR, "szIndex is null.\n");
        return FALSE;
    } 
 	
 	pcOriPos = szRate;
 	pcPos = strstr(pcOriPos, ",");
 	while(pcPos != NULL)
 	{
 		if ((UINT32)(pcPos-pcOriPos)>=6)
 		{
 		    agent_printf(LOG_ERR, "(pcPos-pcOriPos)>=6.\n");
 			return FALSE;
 		}
 
 		memset(acTemp, 0, sizeof(acTemp));
 		memcpy(acTemp, pcOriPos, (UINT32)(pcPos-pcOriPos));
 		acTemp[(UINT32)(pcPos-pcOriPos)] = '\0';

        /* 基本速率集 */
		/*1,2,5.5,11,6,9,12,18,24,36,48,54*/
 		if ( 0 == ulflag )
 		{
        	if (strcmp(acTemp, "5.5") == 0)
        	{
        	    for ( pstRate = astRates; '\0' != pstRate->acRateName[0]; pstRate++ )
        	    {
        	        if ( 0 == strcmp(acTemp, pstRate->acRateName) )
        	        {
        	            sprintf(szIndex+strlen(szIndex), "%d,", pstRate->ulIndex);
        	            break;
        	        }
        	    }
        	}
        	else
        	{
        		bRet = ac_agent_check_num(acTemp);
        		if (FALSE == bRet)
        		{
        			agent_printf(LOG_ERR, "invalid speed:%s in basic.\n", acTemp);
        			return FALSE;
        		}
    			ulValue = atoi(acTemp);
    			
    			for ( ulLoop = 0 ; ulLoop < ulSize; ulLoop++ )
    			{
    			    if ( ulValue == aulRate[ulLoop])
    			    {
    			        break;
    			    }
    			}
    			if ( ulLoop == ulSize)
    			{
    			    agent_printf(LOG_ERR, "invalid speed:%u in basic.\n", ulValue);
    			    return FALSE;
    			}
        	    for ( pstRate = astRates; '\0' != pstRate->acRateName[0]; pstRate++ )
        	    {
        	        if ( 0 == strcmp(acTemp, pstRate->acRateName) )
        	        {
        	            sprintf(szIndex+strlen(szIndex), "%d,", pstRate->ulIndex);
        	            break;
        	        }
        	    }
        	}
 		}
        /* 11n速率集 */
        else if ( 1 == ulflag )
        {
        	if (strcmp(acTemp, "5.5") == 0)
        	{
        	    agent_printf(LOG_ERR, "invalid speed:%s in 11n rates.\n", acTemp);
        	    return FALSE;
        	}
        	else
        	{
        		bRet = ac_agent_check_num(acTemp);
        		if (FALSE == bRet)
        		{
        			if(sscanf(acTemp, "MCS%d", &ulId) == 1
        			   && ulId >=0 && ulId <=15)
        			{
        			}
        			else
        			{
        				agent_printf(LOG_ERR, "invalid speed:%s in 11n rates.\n", acTemp);	
        				return FALSE;
        			}
            	    for ( pstRate = astRates; '\0' != pstRate->acRateName[0]; pstRate++ )
            	    {
            	        if ( 0 == strcmp(acTemp, pstRate->acRateName) )
            	        {
            	            sprintf(szIndex+strlen(szIndex), "%d,", pstRate->ulIndex-12);
            	            break;
            	        }
            	    }
        		}
        		else
        		{
        		    agent_printf(LOG_ERR, "invalid speed:%s in 11n rates.\n", acTemp);
        		    return FALSE;
        		}
        	}
        }
 
 		pcOriPos= pcPos+1;
 		if(*pcOriPos == '\0')
 		{
 			break;
 		}
 		
 		pcPos = strstr(pcOriPos, ",");
 	}
 
 	memset(acTemp, 0, sizeof(acTemp));
 	strcpy(acTemp, pcOriPos);
 
    /* 基本速率集 */
	/*1,2,5.5,11,6,9,12,18,24,36,48,54*/
	if ( 0 == ulflag )
	{
    	if (strcmp(acTemp, "5.5") == 0)
    	{
    	    for ( pstRate = astRates; '\0' != pstRate->acRateName[0]; pstRate++ )
    	    {
    	        if ( 0 == strcmp(acTemp, pstRate->acRateName) )
    	        {
    	            sprintf(szIndex+strlen(szIndex), "%d,", pstRate->ulIndex);
    	            break;
    	        }
    	    }
    	}
    	else
    	{
    		bRet = ac_agent_check_num(acTemp);
    		if (FALSE == bRet)
    		{
    			agent_printf(LOG_ERR, "invalid speed:%s in basic.\n", acTemp);
    			return FALSE;
    		}
    		ulValue = atoi(acTemp);
    		
    		for ( ulLoop = 0 ; ulLoop < ulSize; ulLoop++ )
    		{
    		    if ( ulValue == aulRate[ulLoop])
    		    {
    		        break;
    		    }
    		}
    		if ( ulLoop == ulSize)
    		{
    		    agent_printf(LOG_ERR, "invalid speed:%u in basic.\n", ulValue);
    		    return FALSE;
    		}
    	    for ( pstRate = astRates; '\0' != pstRate->acRateName[0]; pstRate++ )
    	    {
    	        if ( 0 == strcmp(acTemp, pstRate->acRateName) )
    	        {
    	            sprintf(szIndex+strlen(szIndex), "%d,", pstRate->ulIndex);
    	            break;
    	        }
    	    }
    	}
	}
    /* 11n速率集 */
    else if ( 1 == ulflag )
    {
    	if (strcmp(acTemp, "5.5") == 0)
    	{
    	    agent_printf(LOG_ERR, "invalid speed:%s in 11n rates.\n", acTemp);
    	    return FALSE;
    	}
    	else
    	{
    		bRet = ac_agent_check_num(acTemp);
    		if (FALSE == bRet)
    		{
    			if(sscanf(acTemp, "MCS%d", &ulId) == 1
    			   && ulId >=0 && ulId <=15)
    			{
    			}
    			else
    			{
    				agent_printf(LOG_ERR, "invalid speed:%s in 11n rates.\n", acTemp);	
    				return FALSE;
    			}
        	    for ( pstRate = astRates; '\0' != pstRate->acRateName[0]; pstRate++ )
        	    {
        	        if ( 0 == strcmp(acTemp, pstRate->acRateName) )
        	        {
        	            sprintf(szIndex+strlen(szIndex), "%d,", pstRate->ulIndex-12);
        	            break;
        	        }
        	    }
    		}
    		else
    		{
    		    agent_printf(LOG_ERR, "invalid speed:%s in 11n rates.\n", acTemp);
    		    return FALSE;
    		}
    	}
    }
    if ( ',' == szIndex[strlen(szIndex)-1] )
    {
        szIndex[strlen(szIndex)-1] = '\0';
    }
 	
 	return TRUE;
}
BOOL agent_convert_index_to_rate(UINT32 ulFlag, CHAR *szIndex, CHAR *szRate)
{
    CHAR    acIndex[32] = {0};
    UINT32  ulTotal = 0;
    UINT32  ulLoop = 0;
    RATE_DEF_T  *pstRate = astRates;
    
    /* 参数合法性检查 */
    if ( NULL == szIndex )
    {
        agent_printf(LOG_ERR, "szIndex is null.\n");
        return FALSE;
    } 
    if ( NULL == szRate )
    {
        agent_printf(LOG_ERR, "szRate is null.\n");
        return FALSE;
    }

    array_string_to_int(szIndex, acIndex, (int *)&ulTotal);
    for ( ulLoop = 0 ; ulLoop < ulTotal ; ulLoop++ )
    {
        if ( 0 == ulFlag )
        {
            sprintf(szRate+strlen(szRate), "%s,", pstRate[acIndex[ulLoop]].acRateName);
        }
        else if ( 1 == ulFlag )
        {
            sprintf(szRate+strlen(szRate), "%s,", pstRate[acIndex[ulLoop]+12].acRateName);
        }
    }
    if ( ',' == szRate[strlen(szRate)-1] )
    {
        szRate[strlen(szRate)-1] = '\0';
    }
 	
 	return TRUE;
}
/* END:   Added by Zhou Shouya, 2013/8/29 */
VOID get_cpu_endian()
{
	int x = 1; 
	
	if(*(char *)&x == 1)
	{
		//取x指针强制转换为char*类型再取值，此时取到的值是int最低字节值 
	    lprintf(LOG_INFO, "little-endian"); 
		g_ulEndian = LITTLE_ENDIAN;
	}
	else
	{
	    lprintf(LOG_INFO, "big-endian"); 
		g_ulEndian = BIG_ENDIAN;
	}
}
/*****************************************************************************
 Prototype    : dev_get_cms_timeout
 Description  : 获取当前设备与CMS保活超时时间
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
BOOL dev_get_cms_timeout()
{
    UINT32  ulCurTime = 0;
    
    ulCurTime = app_get_uptime();
    g_ulRecvTimeout = ulCurTime - g_ulCmsRecvTime;

    return TRUE;
}
/*****************************************************************************
 Prototype    : dev_get_cms_recvtime
 Description  : 记录AP收到CMS报文的时间
 Input        : UINT8 *pucFromIp  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/5/13
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL dev_get_cms_recvtime(UINT8 *pucFromIp)
{
    UINT8   aucCmsIp[4] = {0};
    //app_get_cms_ip(aucCmsIp);
    //if ( 0 == memcmp(pucFromIp, aucCmsIp, 4) )
    {
        g_ulCmsRecvTime = app_get_uptime();
    }
    return TRUE;
}
/*****************************************************************************
 Prototype    : app_get_cms_ip
 Description  : 获得CMS IP
 Input        : UINT8 * pucSessionIp  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/5/13
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL app_get_cms_ip(UINT8 * pucSessionIp)
{
    struct hostent *hptr = NULL;
    char **pptr;
    char acTrapDesPort[32] = {0};

    g_ac_ip_list.uiResAcNum = 0;
    memset(g_ac_ip_list.aucResAcIp, 0, BIND_AC_NUM_MAX*sizeof(UINT32));

    if ( NULL == pucSessionIp )
    {
        agent_printf(LOG_ERR, "Func:%s pucSessionIp is null.\n", __func__);
        return FALSE;
    }

    if ( (0 == nvram_get(acTrapDesPort, NVRAM_TRAPDESPORT, NVRAM_SIZE))
         && (0 != atoi(acTrapDesPort)))
    {
        g_trap_port = atoi(acTrapDesPort);
    }

    /* 获取静态配置的CMS IP */
    if ( 0 == nvram_get(g_ac_ip_list.acAcURL, NVRAM_TRAPDESIP, 128)
        && '\0' != g_ac_ip_list.acAcURL[0] )
    {
        lprintf(LOG_INFO, "acAcURL:%s.", g_ac_ip_list.acAcURL);
        hptr = gethostbyname(g_ac_ip_list.acAcURL);
        if(NULL != hptr)
        {
            lprintf(LOG_INFO, "h_addrtype:%d.", hptr->h_addrtype);
            switch(hptr->h_addrtype)
            {
                case AF_INET:
                case AF_INET6:
                    pptr=hptr->h_addr_list;
                    for(;*pptr!=NULL;pptr++)
                    {
                        lprintf(LOG_INFO, "*pptr:["IP_FMT"].", IP_ARG(*pptr));
                        memcpy(&g_ac_ip_list.aucResAcIp[g_ac_ip_list.uiResAcNum], *pptr, 4);
                        lprintf(LOG_INFO, "aucResAcIp%lu:"IP_FMT"\n",
                            g_ac_ip_list.uiResAcNum, 
                            IP_ARG(g_ac_ip_list.aucResAcIp[g_ac_ip_list.uiResAcNum]));
                        g_ac_ip_list.uiResAcNum++;

                        if (g_ac_ip_list.uiResAcNum >= BIND_AC_NUM_MAX)
                        {
                            break;
                        }
                    }
                    break;
                default:
                lprintf(LOG_ERR,"%s:invalid addr type:%d\n",__FUNCTION__,hptr->h_addrtype);
                return FALSE;
            }
        }
    }
    memcpy(pucSessionIp, g_ac_ip_list.aucResAcIp[0], 4);
    memcpy(g_aucCmsIp, pucSessionIp, 4);

    return TRUE;
}

#ifdef OPENWRT
BOOL app_get_dev_ip()
{
    unsigned char  acWanIpAddr[32] = {0};

    if ( OK == pal_network_wan_ip_get(acWanIpAddr) )
    {
        if ( '0' != acWanIpAddr[0] )
        {
            if(OK != string_ip_tools(acWanIpAddr, (char *)g_pstSnmpAgentData->pstSysDevInfo->aucIpAddress))
            {
                agent_printf(LOG_ERR, "app_get_dev_ip string ip[0]  ip: %s  failed.\n",acWanIpAddr);
                return FALSE;
            }
            return TRUE;
        }
    }
    return FALSE;
}

BOOL app_get_device_id(UINT32 *pulSysModel)
{
    *pulSysModel = pal_sysinfo_device_model_id();
    return TRUE;
}
BOOL app_get_boardinfo(UINT32 *pulBoardVer)
{


}
#else
BOOL app_get_dev_ip()
{
	char acWanIpAddr[32] = "";
    
    if ( 0 == nvram_get(acWanIpAddr, NVRAM_IPv4"_0_"NVRAM_IP_ADDRESS, NVRAM_SIZE) )
    {
        if ( '0' != acWanIpAddr[0] )
        {
            if(OK != string_ip_tools(acWanIpAddr, (char *)g_pstSnmpAgentData->pstSysDevInfo->aucIpAddress))
            {
                agent_printf(LOG_ERR, "app_get_dev_ip string ip[0] failed.\n");
                return FALSE;
            }
            return TRUE;
        }
    }
    return FALSE;
}

BOOL app_get_device_id(UINT32 *pulSysModel)
{
	FILE    *pfData = NULL;	
	CHAR	*pucPos = NULL;
    CHAR    *szTemp = NULL;
    CHAR    acBuf[128] = {0};
    UINT32  i = 0;
    CHAR    acCmd[128] = {0};

    if ( NULL == pulSysModel )
    {
        agent_printf(LOG_ERR, "app_get_device_id pulSysModel is null.\n");
        *pulSysModel = 9001001;
        return FALSE;
    }

    sprintf(acCmd, "aptool getdevtype > /tmp/getdevtype.tmp");
    system(acCmd);
    
    if ( NULL == (pfData = fopen("/tmp/getdevtype.tmp", "r")) )
    {
        agent_printf(LOG_ERR, "open /tmp/getdevtype.tmp failed.\n");
        *pulSysModel = 9001001;
        return FALSE;
    }

	memset(acBuf, 0, sizeof(acBuf));     
    while ( fgets(acBuf, sizeof(acBuf)-1, pfData) )
    {
        acBuf[sizeof(acBuf)-1] = '\0';

        /* 去除行尾空格 */
        for (i=strlen(acBuf)-1; i >= 0 && IS_EMPTY_CHAR(acBuf[i]); i--)
        {
            acBuf[i] = '\0';
        }

        /* 跳过空格 */
        for(szTemp = acBuf; IS_EMPTY_CHAR(szTemp[0]); szTemp ++);

        /* 判断是否是空行 */
        if (szTemp[0] == '\0')
        {
            continue;
        }

    	// 查询device type
    	pucPos = strstr(acBuf, "device type:");
    	if (NULL != pucPos)
    	{
        	sscanf(acBuf, "device type:%d", pulSysModel);
    	}
    }
    fclose(pfData);
    system("rm -f /tmp/getdevtype.tmp");

    return TRUE;
}

BOOL app_get_boardinfo(UINT32 *pulBoardVer)
{
    FILE *pFile;
    char acBuffer[128] = "";
    int iRet = -1;
    int iBoardVer = -1;
    
    #if defined(AP9344_OAM)
    pFile = popen("boardioctl v ", "r");
    if(NULL == pFile)
    {
        agent_printf(LOG_ERR, 
            "FILE:%s FUN:%s LINE:%d popen error!",
            __FILE__, __func__, __LINE__);

        return FALSE;
    }

    if(NULL == fgets(acBuffer, sizeof(acBuffer), pFile))
    {
        agent_printf(LOG_ERR, 
            "FILE:%s FUN:%s LINE:%d fgets message error!",
            __FILE__, __func__, __LINE__);
        pclose(pFile);
        return FALSE;
    }

    iRet = sscanf(acBuffer, "user_read versiondata is %d", &iBoardVer);
    if(-1 == iRet)
    {
        agent_printf(LOG_ERR, 
            "FILE:%s FUN:%s LINE:%d sscanf error!",
            __FILE__, __func__, __LINE__);
        pclose(pFile);
        return FALSE;
    }
    
    *pulBoardVer = iBoardVer;

    pclose(pFile);
    
    #elif defined(AP9341_OAM) || defined(AP9331_OAM) ||defined(AP9550_OAM)

    pFile = popen("boardinfo show | grep BoardVer ", "r");
    if(NULL == pFile)
    {
        agent_printf(LOG_ERR, 
            "FILE:%s FUN:%s LINE:%d popen error!",
            __FILE__, __func__, __LINE__);

        return FALSE;
    }

    if(NULL == fgets(acBuffer, sizeof(acBuffer), pFile))
    {
        agent_printf(LOG_ERR, 
            "FILE:%s FUN:%s LINE:%d fgets message error!",
            __FILE__, __func__, __LINE__);
        pclose(pFile);
        return FALSE;
    }

    iRet = sscanf(acBuffer, "BoardVer  : %d ", &iBoardVer);
    if(-1 == iRet)
    {
        agent_printf(LOG_ERR, 
            "FILE:%s FUN:%s LINE:%d sscanf error!",
            __FILE__, __func__, __LINE__);
        pclose(pFile);
        return FALSE;
    }

    *pulBoardVer = iBoardVer;
    pclose(pFile);    
    #endif 
    
    return TRUE;
}
#endif
BOOL app_get_cpuinfo(CHAR *pcCpuInfo, UINT32 *pulHwType)
{
    FILE    *pfData = NULL;
    CHAR    acFileName[64] = {"/tmp/cpuinfo"};
    CHAR    *pucBuf = NULL;
    CHAR    *pucPos = NULL;
    UINT32  ulLen;

    system("cat /proc/cpuinfo > /tmp/cpuinfo");
   
    if(NULL == (pfData = fopen (acFileName, "rb")))
    {
        agent_printf(LOG_ERR, "app_get_cpuinfo open %s fail.\n", acFileName);
        return FALSE;
    }
    
    fseek(pfData, 0, SEEK_END);
    ulLen = ftell(pfData);                           /* 获取文件字节数*/
    fseek(pfData, 0, SEEK_SET);

    pucBuf = malloc(ulLen);
    if (pucBuf == NULL) {
        agent_printf(LOG_ERR, "app_get_cpuinfo malloc pucBuf fail.\n");
        fclose(pfData);
        return FALSE;
    }

    fread(pucBuf, 1, ulLen, pfData);
    fclose(pfData);

    /* 查找Atheros AR9341*/
    pucPos = strstr(pucBuf, "Atheros AR9341");
    if (pucPos != NULL)
    {
        strcpy(pcCpuInfo, "Atheros AR9341");
        *pulHwType = AP_HWTYPE_AR9341;
        goto end;
    }

    /* 查找Atheros AR9558*/
    pucPos = strstr(pucBuf, "Atheros AR9558");
    if (pucPos != NULL)
    {
        strcpy(pcCpuInfo, "Atheros AR9558");
        *pulHwType = AP_HWTYPE_AR9558;
        goto end;
    }

    /* 查找Atheros AR7240*/
    pucPos = strstr(pucBuf, "Atheros AR7240");
    if (pucPos != NULL)
    {
        strcpy(pcCpuInfo, "Atheros AR7240");
        *pulHwType = AP_HWTYPE_AR7240;
        goto end;
    }

    /* 查找Atheros AR9344*/
    pucPos = strstr(pucBuf, "Atheros AR9344");
    if (pucPos != NULL)
    {
        strcpy(pcCpuInfo, "Atheros AR9344");
        *pulHwType = AP_HWTYPE_AR9344;
        goto end;
    }

    /* 查找Atheros AR934x*/
    pucPos = strstr(pucBuf, "Atheros AR934x");
    if (pucPos != NULL)
    {
        strcpy(pcCpuInfo, "Atheros AR934x");
        *pulHwType = AP_HWTYPE_AR9344;
        goto end;
    }
    /* 查找Atheros AR9532*/
    pucPos = strstr(pucBuf, "Atheros AR934x");
    if (pucPos != NULL)
    {
        strcpy(pcCpuInfo, "Atheros AR934x");
        *pulHwType = AP_HWTYPE_AR9344;
        goto end;
    }
    pucPos = strstr(pucBuf, "Atheros AR9531");
    if (pucPos != NULL)
    {
        strcpy(pcCpuInfo, "Atheros AR9531");
        *pulHwType = AP_HWTYPE_AR9531;
        goto end;
    }

   pucPos = strstr(pucBuf, "Atheros AR9563");
    if (pucPos != NULL)
    {
        strcpy(pcCpuInfo, "Atheros AR9563");
        *pulHwType = AP_HWTYPE_AR9563;
        goto end;
    }

end:
    free(pucBuf);
    return TRUE;
}

CHAR * app_parse_filename(CHAR *szPath)
{
	char    *szFile;

	if(NULL == szPath)
	{
		agent_printf(LOG_ERR, "ac_agent_parse_filename() szPath is NULL.\n");
		return NULL;
	}
	
    szFile = szPath + strlen(szPath)-1;
    if (*szFile == '/')
    {
        return "";
    }

    for (; szFile >= szPath; szFile--)
    {
        if (*szFile == '/')
        {
            return szFile+1;
        }
    }

    return szPath;
}
BOOL  app_parse_swversion(CHAR *szZipFileName, CHAR *szUnzipDir)
{	
	/* 局部变量定义 */
	CHAR *c = NULL;
	//CHAR *compare = ".tar.gz";
	CHAR *compare = ".bin";
	UINT32 ulLen = 0;
	
	if (NULL == szZipFileName)
	{
		agent_printf(LOG_ERR, "app_parse_swversion:szZipFile is NULL.\n");
		return FALSE;
	}

	if (NULL == szUnzipDir)
	{
		agent_printf(LOG_ERR, "app_parse_swversion:szSwVersion is NULL.\n");
		return FALSE;
	}

	c = szZipFileName;
	while (*c != '\0' &&
		(strlen(c)>=strlen(compare)) &&
		memcmp(c, compare, strlen(compare)) != 0)
	{
		c++;
		ulLen++;
	}

	if (c == '\0' ||
		strlen(c)<strlen(compare))
	{
		agent_printf(LOG_ERR, "app_parse_swversion:input szZipFileName:%s is not a .bin file.", szZipFileName);
		return FALSE;
	}

	memcpy(szUnzipDir, szZipFileName, ulLen);
	szUnzipDir[ulLen] = '\0';
	
	return TRUE;
}
BOOL agent_file_transmit(AP_OPER_FILETRANS_T *pstFileTrans)
{
	agent_printf(LOG_DEBUG,"entering this function.\n");
    CHAR acCmd[256]={0};
    // 压缩包文件名称
    CHAR    *szSwFileName = NULL;
    CHAR    acFileName[32] = {0};
    
    /*检查参数合法性*/
    if(NULL == pstFileTrans)
    {
        agent_printf(LOG_ERR,"pstFileTrans is null.\n");
		return FALSE;
    }
    if(TRANS_FTP != pstFileTrans->ulTransProto)
    {
        agent_printf(LOG_ERR,"trans proto is not ftp.\n");
		return FALSE;
    }
    if(FTP_PORT != pstFileTrans->ulTransPort)
    {
        agent_printf(LOG_ERR,"ulTransPort is not ftp port.\n");
		return FALSE;
    }

    /* 使用ftpget下载软件版本
Usage: ftpget [options] remote-host local-file remote-file

Retrieve a remote file via FTP.

Options:
        -c, --continue         Continue a previous transfer
        -v, --verbose          Verbose
        -u, --username         Username to be used
        -p, --password         Password to be used
        -P, --port             Port number to be used 
    */
#ifndef GW_X86
    sprintf(acCmd,"ftpget -u %s -p %s -P %u %u.%u.%u.%u %s %s",
        pstFileTrans->aucTransUser,
        pstFileTrans->aucTransPasswd,
        pstFileTrans->ulTransPort,
        IP_ARG(pstFileTrans->aucServerIp),
        CONFIG_FILE_DIR,
        pstFileTrans->acFileName);
#else
    sprintf(acCmd,"busybox ftpget -u %s -p %s -P %u %u.%u.%u.%u %s %s",
        pstFileTrans->aucTransUser,
        pstFileTrans->aucTransPasswd,
        pstFileTrans->ulTransPort,
        IP_ARG(pstFileTrans->aucServerIp),
        CONFIG_FILE_DIR,
        pstFileTrans->acFileName);	
#endif
    agent_printf(LOG_INFO,"Func:%s acCmd:%s begin.\n",__func__, acCmd);
    if(0 != system(acCmd))
    {
        agent_printf(LOG_ERR,"Func:%s acCmd:%s excute error", __func__, acCmd);
        goto fail;
    }

    /* 获得版本文件名，包含文件后缀.bin */
    szSwFileName = app_parse_filename(g_pstSnmpAgentData->pstApFileTransConfig->acFileName);
    if ( NULL == szSwFileName )
    {
        agent_printf(LOG_ERR, "app_parse_filename szSwFileName is null.\n");
        return FALSE;
    }
    
    /* 获取软件版本名称，去除文件后缀.bin */
    if ( FALSE == app_parse_swversion(szSwFileName, acFileName) )
    {
        agent_printf(LOG_ERR, "app_parse_swversion fail.\n");
        goto fail;
    }
    if ( NULL != g_pstSnmpAgentData->pstSysSwInfo )
    {
        strcpy(g_pstSnmpAgentData->pstSysSwInfo->acTargetVersion, acFileName);
    }
    
    return TRUE;
fail:
    sprintf(acCmd, "rm -rf "CONFIG_FILE_DIR);
    if(0 != system(acCmd))
    {
        agent_printf(LOG_ERR,"command:%s excute error.\n", acCmd);
		return FALSE;
    }
    
    return FALSE;
}
BOOL agent_flash_image(VOID)
{
    #ifdef OPENWRT
    if(0 != system("mv /tmp/tmp.bin /tmp/app.bin"))
    {
        agent_printf(LOG_ERR, "Func:%s mv /tmp/tmp.bin /tmp/app.tar.gz failed.\n",
            __func__);
        return FALSE;
    }
    if (0 != system("vflag localupdate")) 
    {
        agent_printf(LOG_ERR, "Func:%s vflag localupdate ERROR\n", __func__);
    }

    system("/usr/sbin/reboot 3 rebooting by snmp software update &");
    
    #endif
    #if (defined(GW_X86) || defined(GW_ARM))
        if(0 != system("mv /tmp/tmp.bin /repo/app.bin"))
        {
            agent_printf(LOG_ERR, "Func:%s mv /tmp/tmp.bin /repo/app.bin failed.\n",
                __func__);
            return FALSE;
        }
        if (0 != system("/appfs/sbin/vflag localupdate")) 
        {
            agent_printf(LOG_ERR, "Func:%s vflag localupdate ERROR\n", __func__);
        }
		system("reboot");
    #endif
    #ifdef AP9341_OAM_21    //8M flash

        #ifdef FLASH_16  //16M flash
        if(0 != system("mv /tmp/tmp.bin /tmp/app.bin"))
        {
            agent_printf(LOG_ERR, "Func:%s mv /tmp/tmp.bin /tmp/app.bin failed.\n",
                __func__);
            return FALSE;
        }
		//sleep(30);
        if (0 != system("/usr/sbin/vflag localupdate")) 
        {
            agent_printf(LOG_ERR, "Func:%s vflag localupdate ERROR\n", __func__);
        }
        system("/usr/sbin/reboot 3 rebooting by snmp software update &");
        #else
        if(0 != system("mv /tmp/tmp.bin /tmp/app.bin"))
        {
            agent_printf(LOG_ERR, "Func:%s mv /tmp/tmp.bin /tmp/app.bin failed.\n",
                __func__);
            return FALSE;
        }
		//sleep(30);
        if (0 != system("/usr/sbin/vflag localupdate &")) 
        {
            agent_printf(LOG_ERR, "Func:%s vflag localupdate ERROR\n", __func__);
        }
        #endif
    #endif

    #ifdef AP9341_OAM        
       #ifdef LTE_SUPPORT
        if(0 != system("mv /tmp/tmp.bin /tmp/app.bin 1>/dev/null 2>&1"))
        {
            agent_printf(LOG_ERR, "shell exec mv /tmp/tmp.bin /tmp/app.bin 1>/dev/null 2>&1 failed");
            return FALSE;
        }
        if (0 != system("/usr/sbin/vflag localupdate")) 
        {
            agent_printf(LOG_ERR, "Func:%s vflag localupdate ERROR\n", __func__);
        }
        system("/usr/sbin/reboot 3 rebooting by snmp software update &");
        #else
            
        if(0 != system("mv /tmp/tmp.bin /tmp/app.tar.gz 1>/dev/null 2>&1"))
        {
            agent_printf(LOG_ERR, "shell exec mv /tmp/tmp.bin /tmp/app.tar.gz 1>/dev/null 2>&1 failed");
            return FALSE;
        }
        if (0 != system("/usr/sbin/vflag localupdate &")) 
        {
            agent_printf(LOG_ERR, "Func:%s vflag localupdate ERROR\n", __func__);
        }
        #endif
       
    #endif
    
    #ifdef AP9344_OAM
        if(0 != system("mv /tmp/tmp.bin /tmp/app.tar.gz"))
        {
            agent_printf(LOG_ERR, "Func:%s mv /tmp/tmp.bin /tmp/app.tar.gz failed.\n",
                __func__);
            return FALSE;
        }
        if (0 != system("/usr/sbin/vflag localupdate"))
        {
            agent_printf(LOG_ERR, "Func:%s vflag localupdate ERROR\n", __func__);
        }
        system("/usr/sbin/reboot 3 rebooting by snmp software update &");
    #endif

	#ifdef AP93_OAM
        if(0 != system("mv /tmp/tmp.bin /tmp/app.tar.gz"))
        {
            agent_printf(LOG_ERR, "Func:%s mv /tmp/tmp.bin /tmp/app.tar.gz failed.\n",
                __func__);
            return FALSE;
        }
	    if (0 != system("updatever /tmp/app.tar.gz turn 1>/dev/null 2>&1"))
        {
            lcgic_log(CGI_LOG_FAIL, "updatever fail snmp exit\n");
        }
	    system("/usr/sbin/reboot 3 rebooting by snmp update 1>/dev/null 2>&1 &");
	#endif
    return TRUE;
}

BOOL app_software_update()
{
	UINT8  aucInvalidServerIp[4] = {0};

    if ( NULL == g_pstSnmpAgentData )
    {
        agent_printf(LOG_ERR, "app_software_update g_pstSnmpAgentData is null.\n");
        return FALSE;
    }

    if ( NULL == g_pstSnmpAgentData->pstApFileTransConfig )
    {
        agent_printf(LOG_ERR, "app_software_update g_pstSnmpAgentData->pstApFileTransConfig is null.\n");
        return FALSE;
    }

    /* BEGIN: Added by zoucaihong, 2014/7/16 */
    g_pstSnmpAgentData->pstApFileTransConfig->ulFileType = FILE_SOFTWARE;
    /* END: Added by zoucaihong, 2014/7/16 */

	if (memcmp(g_pstSnmpAgentData->pstApFileTransConfig->aucServerIp, aucInvalidServerIp, 4) == 0)
	{
		agent_printf(LOG_ERR, "app_software_update aucServerIp:0.0.0.0 invalid.\n");
    	g_pstSnmpAgentData->pstApFileTransConfig->ulTransStatus = AP_SWUPDATE_PARAMINVALID;
        goto end;
	}

	if (strcmp(g_pstSnmpAgentData->pstApFileTransConfig->acFileName, "") == 0)
	{
		agent_printf(LOG_ERR, "app_software_update acFileName is empty.\n");
    	g_pstSnmpAgentData->pstApFileTransConfig->ulTransStatus = AP_SWUPDATE_PARAMINVALID;
        goto end;
	}

    /*文件传输*/
	if(FALSE == agent_file_transmit(g_pstSnmpAgentData->pstApFileTransConfig))
	{
        agent_printf(LOG_ERR,"agent_file_transmit fail.\n");
    	g_pstSnmpAgentData->pstApFileTransConfig->ulTransStatus = AP_SWUPDATE_DOWNLOADFAIL;
        goto end;
	}
    g_pstSnmpAgentData->pstApFileTransConfig->ulTransStatus = AP_SWUPDATE_SUCCESS;
    
    /*烧录FLASH*/
	if(FALSE == agent_flash_image())
	{
        agent_printf(LOG_ERR,"agent_flash_image fail");
    	g_pstSnmpAgentData->pstApFileTransConfig->ulTransStatus = AP_SWUPDATE_FLASHFAIL;
        goto end;
	}
    g_pstSnmpAgentData->pstApFileTransConfig->ulTransStatus = AP_SWUPDATE_SUCCESS;
    
    if(FALSE == agent_send_apswupdatetrap())
    {
        agent_printf(LOG_ERR,"agent_send_apswupdatetrap fail");
        return FALSE;
    }
    return TRUE;
	
end:
    /*发送apSoftwareUpdateTrap*/
    if(FALSE == agent_send_apswupdatetrap())
    {
        agent_printf(LOG_ERR,"agent_send_apswupdatetrap fail");
		return FALSE;
    }
    return FALSE;
}

BOOL app_problem_collect( AP_OPER_FILETRANS_T *pstFileTrans)
{
    CHAR    acCmd[256];
    UINT8   aucInvalidIp[4] = {0};
    CHAR    *szFileName = NULL;

    pstFileTrans->ulFileType = FILE_DIAGNOSE;
    /* 检查服务器IP配置 */
    if ( 0 == memcmp( aucInvalidIp, pstFileTrans->aucServerIp, 4 ) )
    {
        agent_printf(LOG_ERR, "Func:%s server ip is invalid(0.0.0.0).\n", __func__);
    	pstFileTrans->ulTransStatus = AP_SWUPDATE_PARAMINVALID;
        goto end;
    }

    /* 检查文件路径名配置 */
    if ( 0 == strlen(pstFileTrans->acFileName) )
    {
        agent_printf(LOG_ERR, "Func:%s file path is empty.\n", __func__);
    	pstFileTrans->ulTransStatus = AP_SWUPDATE_PARAMINVALID;
        goto end;
    }

    /* 检查文件名 */
    szFileName = app_parse_filename( pstFileTrans->acFileName );
    if ( NULL == szFileName )
    {
        agent_printf(LOG_ERR, "Func:%s szFileName is NULL.\n", __func__);
    	pstFileTrans->ulTransStatus = AP_SWUPDATE_PARAMINVALID;
        goto end;
    }

    /* 执行一键收集脚本生成一键收集文件 */
    sprintf(acCmd,PROBLEM_COLLECT_SCRIPT" fat > /dev/null 2>&1");
    if ( -1 == system(acCmd) )
    {
        agent_printf(LOG_ERR, "Func:%s Line:%d acCmd:%s excute fail.\n",
            __func__, __line__, acCmd);
    	pstFileTrans->ulTransStatus = AP_SWUPDATE_PARAMINVALID;
        goto end;
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
        PROBLEM_COLLECT_FILE);
    if(-1 == system(acCmd))
    {
        agent_printf(LOG_ERR,"command excute error");
        sprintf(acCmd, "rm -rf %s", PROBLEM_COLLECT_FILE);
        system(acCmd);
    	pstFileTrans->ulTransStatus = AP_SWUPDATE_DOWNLOADFAIL;
        goto end;
    }
    sprintf(acCmd, "rm -rf %s", PROBLEM_COLLECT_FILE);
    system(acCmd);
	pstFileTrans->ulTransStatus = AP_SWUPDATE_SUCCESS;
    
end:
    agent_send_apswupdatetrap();
    return TRUE;
}

