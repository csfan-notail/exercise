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

BOOL snmp_check_valid_ap_config_wifidevicechannel(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_wifidevicepower(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_wifideviceadvance(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_wifidevicerate(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_sys_devinfo(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_vapbasic_table(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_vapsecurity_table(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_hotspot_table(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_alarm(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_trapdes(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_systeminfo(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_ip(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_vlan(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_advance_5gprior(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_advance_11nprior(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_flowctl(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_legalap(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_quick_multiple(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_quick_multiple_wifi24(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_quick_multiple_wifi5(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_quick_single(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_quick_single_wifi24(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_quick_single_wifi5(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_log(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_user(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_sntp(VOID *pData, VOID *pOriData);
BOOL snmp_check_valid_ap_config_active(VOID *pData, VOID *pOriData);


BOOL snmp_check_valid_cpe_basic_info(VOID *pData, VOID *pOriData);
BOOL snmp_info_set(VOID *pData, VOID *pOriData);
BOOL sys_debug_info_set(VOID *pData, VOID *pOriData);
BOOL device_heart_info_set(VOID *pData, VOID *pOriData);
BOOL snmp_log_cfg_set(VOID *pData, VOID *pOriData);
BOOL snmp_device_oper_set(VOID *pData, VOID *pOriData);
BOOL snmp_file_trans_set(VOID *pData, VOID *pOriData);

BOOL snmp_lte_cfg_info_set(VOID * pData, VOID * pOriData);
BOOL snmp_lte_station_lock_set(VOID * pData, VOID * pOriData);
BOOL snmp_lte_station_lock_table_set(VOID * pData, VOID * pOriData);

BOOL snmp_keepalive_cfg_set(VOID * pData, VOID * pOriData);
BOOL snmp_keepalive_info_set(VOID * pData, VOID * pOriData);




#endif

