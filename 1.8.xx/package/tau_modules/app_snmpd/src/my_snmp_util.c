/******************************************************************************
* COPYRIGHT GBCOM
*******************************************************************************
* ��    ��: 
* �޸���ʷ: 
* 2012-9-10     �뾲              �½�
******************************************************************************/

#include <syslog.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <semaphore.h>
#include <time.h>
#include <linux/sys.h>
#include <sys/sysinfo.h> 

#include "mini_snmpd.h"

#include "my_snmp_def.h"
#include "my_snmp_data.h"
#include "my_snmp_handle.h"
#include "my_snmp_info.h"
#include "my_snmp_util.h"

/************************************************************************/
extern SNMP_AGENT_DATA_T *g_pstSnmpAgentData;
//extern OPEN_MIB_COMMUNITY_CONFIG_T g_astSnmpV2Config[TABLE_MAXRECNUM_SNMPV2];

UINT32  g_aulTableRegetCheck[MAX_TABLE_NUM] = {0};
extern UINT32      g_ulEndian;
extern int g_stati_per;

/************************************************************************/
extern int sem_timedwait(sem_t *, const struct timespec *);
/************************************************************************/
/*                                                                      */
/************************************************************************/
UINT8 snmp_get_asn1_type(UINT8 ucColumnType)
{
	switch(ucColumnType)
	{
		case TYPE_CHAR:
			return BER_TYPE_OCTET_STRING;    /* �ַ��������ֽ� */
			
		case TYPE_BYTE:
			return BER_TYPE_INTEGER;    
			
		case TYPE_SHORT:
			return BER_TYPE_INTEGER;
			
		case TYPE_LONG:
			return BER_TYPE_INTEGER;
			
		case TYPE_BOOL:
			return BER_TYPE_BOOLEAN;    /* ��ӦTruthValue */
			
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

VOID ModuleLogMsg (DWORD dwModuleId, BYTE byLevel, CHAR* szFormat, ...)
{
	return;
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
������          : OsCreateSemaphore
����            : �����ź���
�㷨ʵ��        :
����ȫ�ֱ���    : ��
�������˵��    :
    LPBYTE lpbyName  -- �ź�������
    WORD32 dwInitVal -- �ź�����ʼֵ
    WORD32 dwMaxVal  -- �ź������ֵ
    WORD32 dwFlag    -- �ź�����־
    OSTOKEN *ptSmID  -- �ź���ID
����ֵ˵��      : BOOL
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
    case SEM_COUNT: /* �����ź��� */
    case SEM_SYNC:  /* ͬ���ź��� */
        *ptSmID = (OSTOKEN)CreateSemaphore (NULL, dwInitVal, dwMaxVal, (CHAR*)lpbyName);
        break;
    case SEM_MUTEX: /* �����ź��� */
        *ptSmID = (OSTOKEN)CreateMutex (NULL, FALSE, (CHAR*)lpbyName);
        break;
    default:
        *ptSmID = 0;
        ModuleLogMsg (OSS_MODULE_VOS, APP_ERROR, "OsCreateSemaphore: Invalid semaphore flag !");
        break;
    }

    if (*ptSmID == 0)
    {
        ModuleLogMsg (OSS_MODULE_VOS, APP_ERROR, "OsCreateSemaphore: Create semaphore failed!");
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
        ModuleLogMsg (OSS_MODULE_VOS, APP_FATAL, "OsCreateSemaphore: sem_alloc failed!");
	}

	switch (dwFlag)
    {
    case SEM_COUNT: /* �����ź��� */
        ret = sem_init ((sem_t*)(*ptSmID), 0, dwInitVal);
        break;
    case SEM_SYNC: /* ͬ���ź��� */
        ret = sem_init ((sem_t*)(*ptSmID), 0, 0);
        break;
    case SEM_MUTEX: /* �����ź��� */
        ret = sem_init((sem_t*)(*ptSmID), 0, 1);
        break;
    default:
        ModuleLogMsg (OSS_MODULE_VOS, APP_FATAL, "OsCreateSemaphore: Invalid semaphore type !");
        return FALSE;
    }

    if (ret != 0)
    {
        ModuleLogMsg (OSS_MODULE_VOS, APP_FATAL, "OsCreateSemaphore: sem_init failed!");
        return FALSE;
    }
    else
    {
        return TRUE;
    }
#endif /* _LINUX_ */
}


/*====================================================================
������          : OsCloseSemaphore
����            : �ر��ź���
�㷨ʵ��        :
����ȫ�ֱ���    : ��
�������˵��    : OSTOKEN tSmID -- �ź���ID
����ֵ˵��      : BOOL
====================================================================*/
BOOL OsCloseSemaphore (OSTOKEN tSmID)
{
#ifdef _OSWIN32_
    return CloseHandle ((HANDLE)tSmID);
#endif

#ifdef __LINUX__
    if (sem_destroy ((sem_t*)tSmID) == -1)
    {
        ModuleLogMsg (OSS_MODULE_VOS, APP_ERROR, "OsCloseSemaphore: sem_destroy failed!");
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
������          : OsSemaphoreP
����            : �ź���P����
�㷨ʵ��        :
����ȫ�ֱ���    : ��
�������˵��    :
    OSTOKEN tSmID      -- �ź���ID
    WORD32 dwTimeout   -- ��ʱ(ms)
    WORD32 dwCount     -- P������Ŀ
����ֵ˵��      : BOOL
====================================================================*/
BOOL OsSemaphoreP (OSTOKEN tSmID, WORD32 dwTimeout, WORD32 dwCount)
{
#ifdef _OSWIN32_
    WORD32    dwRet;

	if(dwCount != 1)
	{
	    ModuleLogMsg (OSS_MODULE_VOS, APP_ERROR, "OsSemaphoreP: p count must be 1 !", tSmID);
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

    ModuleLogMsg (OSS_MODULE_VOS, APP_ERROR, "OsSemaphoreP: P on semaphore failed, semID = %d !", tSmID);
    return FALSE;
#endif

#ifdef __LINUX__
	struct timespec timeout;

	if(dwCount != 1)
	{
	    ModuleLogMsg (OSS_MODULE_VOS, APP_ERROR, "OsSemaphoreP: p count must be 1 !");
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

    ModuleLogMsg (OSS_MODULE_VOS, APP_ERROR, "OsSemaphoreP: P on semaphore failed, semID = %lu !", tSmID);
    return FALSE;
#endif
}


/*====================================================================
������          : OsSemaphoreV
����            : �ź���V����
�㷨ʵ��        :
����ȫ�ֱ���    : ��
�������˵��    :
    OSTOKEN tSmID   -- �ź���ID
    WORD32 dwCount  -- V������Ŀ
����ֵ˵��      : BOOL
====================================================================*/
BOOL OsSemaphoreV (OSTOKEN tSmID, WORD32 dwCount)
{
#ifdef _OSWIN32_
    if (ReleaseSemaphore ((HANDLE)tSmID, dwCount, NULL) == 0) /* �Էǻ����ź����Ĵ��� */
    {
        if (ERROR_INVALID_HANDLE == GetLastError ()) /* ����������ǻ����ź��� */
        {
            if (0 == ReleaseMutex ((HANDLE)tSmID))
            {
                ModuleLogMsg (OSS_MODULE_VOS, APP_ERROR, "OsSemaphoreV: V on semaphore failed, semID = %d !", tSmID);
                return FALSE;
            }
            else
            {
                return TRUE;
            }
        }
        ModuleLogMsg (OSS_MODULE_VOS, APP_ERROR, "OsSemaphoreV: V on semaphore failed, semID = %lu !", tSmID);
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
			ModuleLogMsg (OSS_MODULE_VOS, APP_ERROR, "OsSemaphoreV: sem_post failed, semID = %lu !", tSmID);
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
* ��������: snmp_get_systime
* ��    ��: ��ȡϵͳʱ��
* ��    ��: 
* ��������			����/���				����
* ��������: 
* ˵    ��: 
*************************************************************************/
UINT32 snmp_get_systime(VOID)
{
    UINT32  ulTime = time(NULL);

    return ulTime;
}


/*************************************************************************
* ��������: snmp_get_table_reget_status
* ��    ��: ��ȡ���¼����
* ��    ��: 
* ��������			����/���				����
* ��������: 
* ˵    ��: 
*************************************************************************/
BOOL snmp_get_table_reget_status(SNMP_TABLE_T *pstTable)
{
	UINT32 ulCurrent = 0;
	UINT32 ulLastChange = 0;

	if (pstTable->ulTableID>=MAX_TABLE_NUM)
	{
		return FALSE;
	}

	if (NULL == pstTable)
	{
		agent_printf(LOG_ERR, "pstTable is NULL.\n");
		return FALSE;
	}

	ulLastChange = g_aulTableRegetCheck[pstTable->ulTableID];
	ulCurrent = snmp_get_uptime();

	if(0 == ulLastChange ||
		(ulCurrent>ulLastChange && ulCurrent-ulLastChange>1))
	{
		return TRUE;
	}

	return FALSE;

}

/*************************************************************************
* ��������: snmp_set_table_reget_status
* ��    ��: ��ȡ���¼����
* ��    ��: 
* ��������			����/���				����
* ��������: 
* ˵    ��: 
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
* ��������: snmp_get_table_record
* ��    ��: set/add������ض�̬������λ
* ��    ��: 
* ��������			����/���				����
* ��������: 
* ˵    ��: 
*************************************************************************/
UINT8* snmp_get_table_record (SNMP_TABLE_T *pstTable)
{	
 	UINT8 *pucData = NULL;
 	UINT32 ulTableID = 0;	
	BOOL   bRet = FALSE;
    UINT32 ulCurrentTime = 0;
    ulCurrentTime = snmp_get_systime();

	ulTableID = pstTable->ulTableID;
    //printf("most important:snmp_get_table_record,ulTableID:%u\n",ulTableID);

	switch(ulTableID)
 	{
        /* Sys Info */
        case TABLE_ID_SYS_DEV_INFO:
            bRet = snmp_sys_dev_info();
            if (FALSE == bRet)
            {
                snmp_log(LOG_ERR, "get dev info fail\n");
            }          
        	pucData = (UINT8 *)&g_pstSnmpAgentData->stSysDevInfo;
			break;
			
		case TABLE_ID_SYS_SW_INFO:
		    bRet = snmp_sys_sw_info();
            if (FALSE == bRet)
            {
                snmp_log(LOG_ERR, "get sw info fail\n");
            }       
			pucData = (UINT8 *)&g_pstSnmpAgentData->stSwInfo;
			break;
			
	    case TABLE_ID_SYS_HW_INFO:
            bRet = snmp_sys_hardware_info();
			pucData = (UINT8 *)&g_pstSnmpAgentData->stHwInfo;
			break;	

	    case TABLE_ID_SYS_MANAGE_INFO:
	        bRet = snmp_sys_manage_info();
            if (FALSE == bRet)
            {
                snmp_log(LOG_ERR, "get manage info fail\n");
            }       	    
            pucData = (UINT8 *)&g_pstSnmpAgentData->stManageInfo;
            break;
	    case TABLE_ID_SYS_PROTO_INFO:
	        bRet = snmp_sys_proto_info();
            if (FALSE == bRet)
            {
                snmp_log(LOG_ERR, "get proto info fail\n");
            }       	    
            pucData = (UINT8 *)&g_pstSnmpAgentData->stProtoInfo;
            break;
        /* Lte Info */
	    case TABLE_ID_LTE_HW_INFO:
	        bRet = snmp_lte_hw_info();
            if (FALSE == bRet)
            {
                snmp_log(LOG_ERR, "get lte hardware info fail\n");
            }       	    
            pucData = (UINT8 *)&g_pstSnmpAgentData->stLteHwInfo;
            break;

	    case TABLE_ID_LTE_NET_INFO:
	        bRet = snmp_lte_net_info();
            if (FALSE == bRet)
            {
                snmp_log(LOG_ERR, "get lte net info fail\n");
            }       	    
            pucData = (UINT8 *)&g_pstSnmpAgentData->stLteNetInfo;
            break;


 	    case TABLE_ID_SYS_LTE_CFG_INFO:
	        bRet = snmp_lte_cfg_info();
            if (FALSE == bRet)
            {
                snmp_log(LOG_ERR, "get lte sim info fail\n");
            }       	    
            pucData = (UINT8 *)&g_pstSnmpAgentData->stLteCfgInfo;
            break;
           
        case TABLE_ID_SYS_DEBUG_INFO:
            pucData = (UINT8 *)&g_pstSnmpAgentData->stDebugInfo;
            break;

        case TABLE_ID_DEVICE_HEART_INFO:
            pucData = (UINT8 *)&g_pstSnmpAgentData->stDeviceHeartInfo;
            break;

        case TABLE_ID_LOG_CFG:
            pucData = (UINT8 *)&g_pstSnmpAgentData->stLogCfg;
            break;
           
        case TABLE_ID_WAN_STAT:
            bRet = snmp_lte_wan_state_info();
            if (FALSE == bRet)
            {
                snmp_log(LOG_ERR, "get lte wan info fail\n");
            } 
    		pucData = (UINT8 *)&g_pstSnmpAgentData->stWanStateInfo;
			break;    
		case TABLE_ID_USER_INFO:
		    bRet = snmp_lte_user_info();
		    if (FALSE == bRet)
            {
                snmp_log(LOG_ERR, "get lte wan user info fail\n");
            } 
		    pucData = (UINT8 *)&g_pstSnmpAgentData->stWanUserInfo;
			break;
		case TABLE_ID_LTEKP_CFG:
            bRet = snmp_lte_keepalive_cfg();
		    if (FALSE == bRet)
            {
                snmp_log(LOG_ERR, "get lte keepalive info fail\n");
            } 
            pucData = (UINT8 *)&g_pstSnmpAgentData->stLteKPCfg;
		    break;
        case TABLE_ID_LTEKP_INFO:
		    bRet = snmp_lte_keepalive_info();
		    if (FALSE == bRet)
            {
                snmp_log(LOG_ERR, "get lte keepalive info fail\n");
            } 
		    pucData = (UINT8 *)&g_pstSnmpAgentData->stLteKPInfo;
			break;  
		case TABLE_ID_LTE_SYS_OPER:
			pucData = (UINT8 *)&g_pstSnmpAgentData->stOperInfo;
			break;
		case TABLE_ID_LTE_FILE_TRANS:
			pucData = (UINT8 *)&g_pstSnmpAgentData->stFileTrans;
			break;
		case TABLE_ID_PPPOE_USER_NUM:
    	    bRet = snmp_lte_pppoe_user_num();
    	    if (FALSE == bRet)
            {
                snmp_log(LOG_ERR, "get lte wan user info fail\n");
            } 
    	    pucData = (UINT8 *)&g_pstSnmpAgentData->stPppoeUserNum;
    		break;
        case TABLE_ID_PPPOE_USER_INFO:
    	    bRet = snmp_lte_pppoe_user_info();
    	    if (FALSE == bRet)
            {
                snmp_log(LOG_ERR, "get lte wan user info fail\n");
            } 
    	    pucData = (UINT8 *)&g_pstSnmpAgentData->stPppoeUserInfo;
    		break;
        case TABLE_ID_PPPOE_ALL_USER_NAMES:
            bRet = snmp_lte_pppoe_all_user_names();
            if (FALSE == bRet)
            {
                snmp_log(LOG_ERR, "get lte wan user info fail\n");
            } 
            pucData = (UINT8 *)&g_pstSnmpAgentData->stPppoeAllUserNames;
    		break;
	  	case TABLE_ID_LTE_ENODB_INFO:
		    bRet = snmp_lte_enodb_info();
		    if (FALSE == bRet)
	        {
	            snmp_log(LOG_ERR, "get lte wan user info fail\n");
	        } 
		    pucData = (UINT8 *)&g_pstSnmpAgentData->stLteEnodb;
			break;
	    case TABLE_ID_LTE_STATION_LOCK:
		    bRet = snmp_lte_station_lock();
		    if (FALSE == bRet)
	        {
	            snmp_log(LOG_ERR, "get lte wan user info fail\n");
	        } 
		    pucData = (UINT8 *)&g_pstSnmpAgentData->stLteStationLock;
		    /*printf("!!!!!!!!pucData:%d,%d,%d,%d\n", ((SNMP_LTE_STATION_LOCK_T *)pucData)->ulLteLockType,
		        ((SNMP_LTE_STATION_LOCK_T *)pucData)->ulLteAutoLock, ((SNMP_LTE_STATION_LOCK_T *)pucData)->ulLteAutoMax,
		        ((SNMP_LTE_STATION_LOCK_T *)pucData)->ulLteLockNum);*/
			break;
	    case TABLE_ID_LTE_STATION_TABLE_LOCK:
		    bRet = snmp_lte_station_lock_table();
		    if (FALSE == bRet)
		    {
		        snmp_log(LOG_ERR, "get lte wan user info fail\n");
		    } 
		    pucData = (UINT8 *)&g_pstSnmpAgentData->stLteStationLockTable;
			break;
        case TABLE_ID_LTE_STATION_ALL_LOCK_ID:
            bRet = snmp_lte_station_all_lock_id();
		    if (FALSE == bRet)
		    {
		        snmp_log(LOG_ERR, "get lte wan user info fail\n");
		    } 
		    pucData = (UINT8 *)&g_pstSnmpAgentData->stLteStationAllLockID;
			break;
		default:
			break;
	}

	return pucData;
}



UINT32 ap_agent_generate_trapsn(VOID)
{
	static UINT32 ulTrapSn = 1;
	return (ulTrapSn++);
}


VOID get_cpu_endian()
{
	int x = 1; 
	
	if(*(char *)&x == 1)
	{
		//ȡxָ��ǿ��ת��Ϊchar*������ȡֵ����ʱȡ����ֵ��int����ֽ�ֵ 
	    lprintf(LOG_INFO, "little-endian"); 
		g_ulEndian = LITTLE_ENDIAN;
	}
	else
	{
	    lprintf(LOG_INFO, "big-endian"); 
		g_ulEndian = BIG_ENDIAN;
	}
}
/* BEGIN: Added by zoucaihong, 2013/10/22 */
BOOL agent_convert_time(UINT32 ulTime, CHAR *szTime)
{
 	struct tm * time = NULL;
 
 	if (NULL == szTime)
 	{
 		agent_printf(LOG_ERR, "szTime is null\n");
 		return FALSE;
 	}
 
 	time = localtime((const time_t *)&(ulTime));
 	if (time == NULL)
 	{
 		agent_printf(LOG_ERR, "ulTime:%u localtime() return NULL\n",
 			ulTime);
 		return FALSE;
 	}
 
 	sprintf(szTime, "%04d-%02d-%02d %02d:%02d:%02d",
             time->tm_year+1900,
             time->tm_mon+1,
             time->tm_mday,
             time->tm_hour,
             time->tm_min,
             time->tm_sec);
 	
 	return TRUE;
 }
 /* END: Added by zoucaihong, 2013/10/22 */

