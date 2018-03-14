/******************************************************************************
* COPYRIGHT GBCOM
*******************************************************************************
* 功    能: 
* 修改历史: 
* 2012-9-10     冯静              新建
******************************************************************************/

/******************************* 宏定义 ********************************/
#ifndef __MINI_SNMPD_INIT__
#define __MINI_SNMPD_INIT__

/******************************* 函数声明********************************/
#if 0
/* BEGIN: Added by zoucaihong, 2012/12/4 
add system group and interfaces group of RFC1213*/
BOOL init_snmp_system(SNMP_TABLE_T *pstTable);
BOOL init_snmp_interfaces(SNMP_TABLE_T *pstTable);
BOOL init_snmp_interfaces_table(SNMP_TABLE_T * pstTable, UINT32 ulMaxTableRecNum);
/* END: Added by zoucaihong, 2012/12/4 */
#endif
BOOL init_snmp_ap_config_sys_devinfo(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_config_swinfo(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_config_hwinfo(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_config_ip(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_config_ip_table(SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum);
//BOOL init_snmp_ap_config_legalap(SNMP_TABLE_T *pstTable);
//BOOL init_snmp_ap_config_legalap_table(SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum);
//BOOL init_snmp_ap_config_flowctl(SNMP_TABLE_T *pstTable);

/* BEGIN: Added by zoucaihong, 2013/3/11 */
//BOOL init_snmp_ap_config_advance_5gprior(SNMP_TABLE_T *pstTable);
//BOOL init_snmp_ap_config_advance_11nprior(SNMP_TABLE_T *pstTable);
/* END: Added by zoucaihong, 2013/3/11 */
BOOL init_snmp_ap_config_wifidevicebasic(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_config_wifidevicechannel(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_config_wifidevicepower(SNMP_TABLE_T *pstTable);
/* BEGIN: Added by zoucaihong, 2013/1/22 */
BOOL init_snmp_ap_config_wifideviceadvance(SNMP_TABLE_T *pstTable);
/* END: Added by zoucaihong, 2013/1/22 */
/* BEGIN: Added by Zhou Shouya, 2013/8/29   PN:VLAN */
BOOL init_snmp_ap_config_wifidevicerate(SNMP_TABLE_T *pstTable);
/* END:   Added by Zhou Shouya, 2013/8/29   PN:VLAN */
/* BEGIN: Added by zoucaihong, 2013/3/21 */
/* 2.4G卡配置和5G卡配置分开 */
BOOL init_snmp_ap_config_wifidevicebasic5(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_config_wifidevicechannel5(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_config_wifidevicepower5(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_config_wifideviceadvance5(SNMP_TABLE_T *pstTable);
/* END: Added by zoucaihong, 2013/3/21 */
/* BEGIN: Added by Zhou Shouya, 2013/8/29   PN:VLAN */
BOOL init_snmp_ap_config_wifidevicerate5(SNMP_TABLE_T *pstTable);
/* END:   Added by Zhou Shouya, 2013/8/29   PN:VLAN */
BOOL init_snmp_ap_config_vap(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_config_vapbasictable(SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum);
BOOL init_snmp_ap_config_vapsecuritytable(SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum);
BOOL init_snmp_ap_config_alarm(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_config_trapdes(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_config_trapdes_table(SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum);
BOOL init_snmp_ap_config_vlan(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_config_vlan_table(SNMP_TABLE_T * pstTable, UINT32 ulMaxTableRecNum);
BOOL init_snmp_ap_config_ssid(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_config_ssid_table(SNMP_TABLE_T * pstTable, UINT32 ulMaxTableRecNum);
/* BEGIN: Added by zoucaihong, 2015/3/25 */
BOOL init_snmp_ap_config_user(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_config_gwssid(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_config_auth(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_config_batch_bwlist(SNMP_TABLE_T *pstTable);
/* END: Added by zoucaihong, 2015/3/25 */
BOOL init_snmp_ap_config_domain_filter(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_config_user_speed(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_config_white_user_speed(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_config_user_logout(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_config_get_secret_key(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_stat_sys(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_stat_wifi(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_stat_wifi_table(SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum);
BOOL init_snmp_ap_stat_wifivap(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_stat_wifivap_table(SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum);
BOOL init_snmp_ap_stat_station(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_stat_station_table(SNMP_TABLE_T * pstTable, UINT32 ulMaxTableRecNum);
#if 0
/* BEGIN: Added by zoucaihong, 2013/1/22 */
BOOL init_snmp_ap_stat_24cochainterfinfo(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_stat_24cochainterfinfo_table(SNMP_TABLE_T * pstTable, UINT32 ulMaxTableRecNum);
BOOL init_snmp_ap_stat_5cochainterfinfo(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_stat_5cochainterfinfo_table(SNMP_TABLE_T * pstTable, UINT32 ulMaxTableRecNum);
BOOL init_snmp_ap_stat_24adjchainterfinfo(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_stat_24adjchainterfinfo_table(SNMP_TABLE_T * pstTable, UINT32 ulMaxTableRecNum);
BOOL init_snmp_ap_stat_5adjchainterfinfo(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_stat_5adjchainterfinfo_table(SNMP_TABLE_T * pstTable, UINT32 ulMaxTableRecNum);
BOOL init_snmp_ap_stat_24wlandevinterfinfo(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_stat_24wlandevinterfinfo_table(SNMP_TABLE_T * pstTable, UINT32 ulMaxTableRecNum);
BOOL init_snmp_ap_stat_5wlandevinterfinfo(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_stat_5wlandevinterfinfo_table(SNMP_TABLE_T * pstTable, UINT32 ulMaxTableRecNum);
BOOL init_snmp_ap_stat_nonwlandevinterfinfo(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_stat_nonwlandevinterfinto_table(SNMP_TABLE_T * pstTable, UINT32 ulMaxTableRecNum);
BOOL init_snmp_ap_stat_brmodestation(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_stat_brmodestation_table(SNMP_TABLE_T * pstTable, UINT32 ulMaxTableRecNum);
/* END: Added by zoucaihong, 2013/1/22 */
/* BEGIN: Added by zoucaihong, 2013/5/10 */
BOOL init_snmp_ap_stat_24rogueapinfo(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_stat_24rogueapinfo_table(SNMP_TABLE_T * pstTable, UINT32 ulMaxTableRecNum);
BOOL init_snmp_ap_stat_5rogueapinfo(SNMP_TABLE_T *pstTable);
BOOL init_snmp_ap_stat_5rogueapinfo_table(SNMP_TABLE_T * pstTable, UINT32 ulMaxTableRecNum);
/* END: Added by zoucaihong, 2013/5/10 */
#endif
/* BEGIN: Added by zoucaihong, 2015/3/25 */
BOOL init_snmp_ap_stat_onlineuser_table(SNMP_TABLE_T * pstTable, UINT32 ulMaxTableRecNum);
BOOL init_snmp_ap_stat_ap_table(SNMP_TABLE_T * pstTable, UINT32 ulMaxTableRecNum);
BOOL init_snmp_ap_stat_version_table(SNMP_TABLE_T * pstTable, UINT32 ulMaxTableRecNum);
BOOL init_snmp_ap_stat_ap_sta_table(SNMP_TABLE_T * pstTable, UINT32 ulMaxTableRecNum);
/* END: Added by zoucaihong, 2015/3/25 */
BOOL init_snmp_ap_oper_sysoper( SNMP_TABLE_T *pstTable );
BOOL init_snmp_ap_oper_filetransconfig( SNMP_TABLE_T *pstTable );
/* BEGIN: Added by zoucaihong, 2015/3/25 */
BOOL init_snmp_ap_oper_gwap( SNMP_TABLE_T *pstTable );
/* END: Added by zoucaihong, 2015/3/25 */
#endif

