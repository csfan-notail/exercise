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
#define agent_printf lprintf

extern int nvram_get(char *value,char *name, int len);
extern int nvram_set(char *name ,char *value);
extern int nvram_commit(void);
extern int nvram_unset(char * name);

UINT8 snmp_get_asn1_type(UINT8 ucColumnType);

UINT32 snmp_get_uptime(VOID);
UINT32 snmp_get_systime(VOID);
BOOL agent_convert_time(UINT32 ulTime, CHAR *szTime);
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
VOID ap_agent_convert_u32_to_u64(UINT32 ulHigh, UINT32 ulLow, UINT8 *pucU64);
BOOL ap_agent_parse_conf_file(VOID);
UINT32 ap_agent_generate_trapsn(VOID);
CHAR* ap_agent_convert_time_str(UINT32 ulTime);
BOOL agent_modify_ssidvlanmapping(UINT32 ulVlanId, UINT32 ulVapId);
BOOL agent_modify_vap_security(AP_CONFIG_VAPSECURITY_ENTRY_T *pstVapSecurity, AP_CONFIG_VAPSECURITY_ENTRY_T *pstConfig, CHAR *pacDevName);
BOOL agent_checkvalid_auto_channel( UINT32 ulAbgMode, CHAR *szAutoChanList );
BOOL agent_modify_vap_flow(CHAR *pacDevName, UINT32 ulFlowSwitch, UINT32 ulUplinkMaxRate, UINT32 ulDownlinkMaxRate);
BOOL agent_modify_ssidvlanmapping(UINT32 ulVlanId, UINT32 ulVapId);
/* BEGIN: Added by zoucaihong, 2013/5/13 */
BOOL agent_modify_ap_ip(AP_CONFIG_IP_ENTRY_T *pstAPIpConfig, AP_CONFIG_IP_ENTRY_T *pstAPIpConfigOld);
/* END: Added by zoucaihong, 2013/5/13 */
//BOOL agent_modify_ap_legalap(AP_CONFIG_LEGALAP_ENTRY_T * pstApLegalAp, AP_CONFIG_LEGALAP_ENTRY_T * OldpstApLegalAp);
//BOOL agent_delete_ap_legalap(AP_CONFIG_LEGALAP_ENTRY_T *pstApLegalAp);
//BOOL agent_add_ap_legalap(AP_CONFIG_LEGALAP_ENTRY_T *pstApLegalAp);
//BOOL agent_modify_ap_flowctl(AP_CONFIG_FLOWCTL_T *pstApFlowCtl, AP_CONFIG_FLOWCTL_T *pstApFlowCtlOri);
/* BEGIN: Added by Zhou Shouya, 2013/8/29 */
BOOL agent_check_transmit_speed_validity(UINT32 ulflag, CHAR *szRate, CHAR *szIndex);
/* END:   Added by Zhou Shouya, 2013/8/29 */
/* 2013-05-16  fengjing add BEGIN: 判断是大端还是小端字节序 */
VOID get_cpu_endian(VOID);
BOOL app_get_cms_ip(UINT8 * pucSessionIp);
BOOL app_get_dev_ip(VOID);
BOOL app_get_device_id(UINT32 *pulSysModel);
BOOL app_get_boardinfo(UINT32 *pulBoardVer);
BOOL app_get_cpuinfo(CHAR *pcCpuInfo, UINT32 *pulHwType);
BOOL app_problem_collect( AP_OPER_FILETRANS_T *pstFileTrans);
BOOL agent_convert_index_to_rate(UINT32 ulFlag, CHAR *szIndex, CHAR *szRate);
CHAR * app_parse_filename(CHAR *szPath);
BOOL app_software_update(VOID);
BOOL dev_get_cms_recvtime(UINT8 *pucFromIp);
BOOL dev_get_cms_timeout(VOID);
char * get_convert_start(char *pcBuf);
int str2info(char *pcBuf, unsigned int *pulDevInfo, int iMax);
BOOL get_interface_stats( UINT64 *pulIn, UINT64 *pulOut);
BOOL string_macs_to_array(CHAR * pcMacList, AP_MAC_LIST_T * pstMac);
BOOL multi_ssid_convert(CHAR * pcSsidList, AP_SSID_LIST_T * pstSsid);
int ap_hash_init(VOID);

#endif

