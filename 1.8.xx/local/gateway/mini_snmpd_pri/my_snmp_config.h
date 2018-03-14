/******************************************************************************
* COPYRIGHT GBCOM
*******************************************************************************
* 功    能: 
* 修改历史: 
* 2012-9-10     冯静              新建
******************************************************************************/

/******************************* 宏定义 ********************************/
#ifndef __MINI_SNMPD_CONFIG__
#define __MINI_SNMPD_CONFIG__


/* BEGIN: Added by zoucaihong, 2013/3/25 */
BOOL snmp_check_valid_ap_config_wifidevicechannel(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_wifidevicepower(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_wifideviceadvance(VOID *pData, VOID *pOriData);
/* END: Added by zoucaihong, 2013/3/25 */
/* BEGIN: Added by Zhou Shouya, 2013/8/29 */
BOOL snmp_check_valid_ap_config_wifidevicerate(VOID *pData, VOID *pOriData);
/* END:   Added by Zhou Shouya, 2013/8/29 */
BOOL snmp_check_valid_ap_config_sys_devinfo(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_vapbasic_table(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_vapsecurity_table(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_alarm(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_trapdes(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_systeminfo(VOID *pData, VOID *pOriData);
/* BEGIN: Added by zoucaihong, 2013/2/25 */
BOOL snmp_check_valid_ap_config_ip(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_vlan(VOID *pData, VOID *pOriData);
/* END: Added by zoucaihong, 2013/2/25 */
/* BEGIN: Added by zoucaihong, 2013/3/1 */
BOOL snmp_check_valid_ap_config_ssid(VOID *pData, VOID *pOriData);
/* END: Added by zoucaihong, 2013/3/1 */
/* BEGIN: Added by zoucaihong, 2013/3/11 */
//BOOL snmp_check_valid_ap_config_advance_5gprior(VOID *pData, VOID *pOriData);
//BOOL snmp_check_valid_ap_config_advance_11nprior(VOID *pData, VOID *pOriData);
/* END: Added by zoucaihong, 2013/3/11 */
//BOOL snmp_check_valid_ap_config_flowctl(VOID *pData, VOID *pOriData);
//BOOL snmp_check_valid_ap_config_legalap(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_oper_sysoper(VOID *pData, VOID *pOriData);
#endif

