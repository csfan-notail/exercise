#ifndef __MINI_SNMPD_INFO__
#define __MINI_SNMPD_INFO__

BOOL snmp_sys_lte_info_init(void);
BOOL snmp_sys_dev_info(void);
BOOL snmp_sys_sw_info(void);
BOOL snmp_sys_hardware_info(void);
BOOL snmp_sys_manage_info(void);
BOOL snmp_lte_wifi_info(void);
BOOL snmp_lte_cfg_info(void);
BOOL snmp_lte_hw_info(void);
BOOL snmp_lte_net_info(void);
BOOL snmp_sys_proto_info(void);
BOOL snmp_lte_wan_state_info(void);
BOOL snmp_lte_wan_bytes_info(void);
BOOL snmp_lte_user_info(void);
BOOL snmp_lte_keepalive_cfg(void);
BOOL snmp_lte_keepalive_info(void);
BOOL snmp_lte_pppoe_user_num(void);
BOOL snmp_lte_pppoe_user_info(void);
BOOL snmp_lte_pppoe_all_user_names(void);
BOOL snmp_lte_enodb_info(void);
BOOL snmp_lte_station_lock(void);
BOOL snmp_lte_station_lock_table(void);
BOOL snmp_lte_station_all_lock_id(void);

#endif
