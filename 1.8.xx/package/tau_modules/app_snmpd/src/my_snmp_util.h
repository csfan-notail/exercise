/******************************************************************************
* COPYRIGHT GBCOM
*******************************************************************************
* 功    能: 
* 修改历史: 
* 2012-9-10     冯静              新建
******************************************************************************/
#ifndef __MINI_SNMPD_UTIL__
#define __MINI_SNMPD_UTIL__

typedef struct  
{
	CHAR  acReadCommunity[32];
	CHAR  acWriteCommunity[32];
	//UINT8  aucSnmpManagerIp[4];
	
	//CHAR   acSysObjectID[128];
}OPEN_MIB_COMMUNITY_CONFIG_T;

/* feng jing add */
#define agent_printf snmp_log


UINT8 snmp_get_asn1_type(UINT8 ucColumnType);

UINT32 snmp_get_uptime(VOID);
UINT32 snmp_get_systime(VOID);
/*metux*/
BOOL OsCreateSemaphore (LPBYTE lpbyName,
                        WORD32 dwInitVal,
                        WORD32 dwMaxVal,
                        WORD32 dwFlag,
                        OSTOKEN *ptSmID);
BOOL OsCloseSemaphore (OSTOKEN tSmID);
BOOL OsSemaphoreP (OSTOKEN tSmID, WORD32 dwTimeout, WORD32 dwCount);
BOOL OsSemaphoreV (OSTOKEN tSmID, WORD32 dwCount);

BOOL snmp_get_table_reget_status(SNMP_TABLE_T *pstTable);
BOOL snmp_set_table_reget_status(SNMP_TABLE_T *pstTable);
BOOL snmp_reset_table_reget_status(UINT32 ulTableID);
UINT8* snmp_get_table_record (SNMP_TABLE_T *pstTable);
UINT32 ap_agent_generate_trapsn(VOID);
/* 2013-05-16  fengjing add BEGIN: 判断是大端还是小端字节序 */
VOID get_cpu_endian();
/* BEGIN: Added by zoucaihong, 2013/10/22 */
BOOL agent_convert_time(UINT32 ulTime, CHAR *szTime);
/* END: Added by zoucaihong, 2013/10/22 */
#endif

