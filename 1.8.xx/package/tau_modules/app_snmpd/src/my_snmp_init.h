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

BOOL init_sys_dev_info_table(SNMP_TABLE_T *pstTable);
BOOL init_sys_sw_info_table(SNMP_TABLE_T *pstTable);
BOOL init_sys_hw_info_table(SNMP_TABLE_T *pstTable);
BOOL init_sys_manage_info_table(SNMP_TABLE_T *pstTable);
BOOL init_sys_proto_info_table(SNMP_TABLE_T *pstTable);
BOOL init_device_heart_info_table(SNMP_TABLE_T *pstTable);
BOOL init_sys_debug_info_table(SNMP_TABLE_T *pstTable);

BOOL init_lte_hw_table(SNMP_TABLE_T *pstTable);
BOOL init_lte_net_table(SNMP_TABLE_T *pstTable);
BOOL init_lte_cfg_table(SNMP_TABLE_T *pstTable);

BOOL init_wifi_info_table(SNMP_TABLE_T *pstTable);


BOOL init_wan_state_info_table(SNMP_TABLE_T *pstTable);
BOOL init_wan_user_num(SNMP_TABLE_T *pstTable);
BOOL init_wan_user_info_table(SNMP_TABLE_T *pstTable, UINT32 ulMaxTableNum);

BOOL init_log_cfg_table(SNMP_TABLE_T *pstTable);

BOOL init_keepalive_cfg_table(SNMP_TABLE_T *pstTable);
BOOL init_keepalive_info_table(SNMP_TABLE_T *pstTable, UINT32 ulMaxTableNum);


BOOL init_device_oper_table(SNMP_TABLE_T *pstTable);
BOOL init_file_trans_table(SNMP_TABLE_T *pstTable);

BOOL init_pppoe_user_num(SNMP_TABLE_T *pstTable);
BOOL init_pppoe_user_info_table(SNMP_TABLE_T *pstTable, UINT32 ulMaxTableNum);
BOOL init_pppoe_all_user_names(SNMP_TABLE_T *pstTable);

BOOL init_lte_enodeb_info(SNMP_TABLE_T *pstTable);
BOOL init_lte_station_lock(SNMP_TABLE_T *pstTable);
BOOL init_lte_station_lock_table(SNMP_TABLE_T *pstTable, UINT32 ulMaxTableNum);
BOOL init_lte_station_all_lock_id(SNMP_TABLE_T *pstTable);

#endif

