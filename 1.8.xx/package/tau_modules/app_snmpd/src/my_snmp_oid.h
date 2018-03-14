/******************************************************************************
* COPYRIGHT GBCOM
*******************************************************************************
* 功    能: 
* 修改历史: 
* 2012-9-10     冯静              新建
******************************************************************************/

/******************************* 宏定义 ********************************/
#ifndef __MINI_SNMPD_OID__
#define __MINI_SNMPD_OID__


/******************************* OID ********************************/
/*dlink ap rootoid:1.3.6.1.4.1.171.10.132*/
/* BEGIN: Modified by zoucaihong, 2012/12/4 */
/*MIB-2*/
#define OID_MGMT                            2
#define OID_MIB2                            OID_MGMT, 1

/* DevInfo */
static SNMP_OID oid_lte_config_sys_devinfo[]         = {14, 2, 1, 1};
static SNMP_OID oid_lte_devinfo_sn[]                 = {14, 2, 1, 1, 1};
static SNMP_OID oid_lte_devinfo_devtype[]            = {14, 2, 1, 1, 2};
static SNMP_OID oid_lte_devinfo_time[]               = {14, 2, 1, 1, 3};
static SNMP_OID oid_lte_devinfo_uptime[]             = {14, 2, 1, 1, 4};

/* SwInfo */
static SNMP_OID oid_lte_sw_info[]                    = {14, 2, 1, 2};
static SNMP_OID oid_lte_swinfo_targetVer[]           = {14, 2, 1, 2, 1};
static SNMP_OID oid_lte_swinfo_curVer[]              = {14, 2, 1, 2, 2};

/* HwInfo */
static SNMP_OID oid_lte_hw_info[]                    = {14, 2, 1, 3};
static SNMP_OID oid_lte_hwinfo_cpuType[]             = {14, 2, 1, 3, 1};
static SNMP_OID oid_lte_hwinfo_memType[]             = {14, 2, 1, 3, 2};
static SNMP_OID oid_lte_hwinfo_memorySize[]          = {14, 2, 1, 3, 3};
static SNMP_OID oid_lte_hwinfo_flashSize[]           = {14, 2, 1, 3, 4};
static SNMP_OID oid_lte_hwinfo_hardwareVer[]         = {14, 2, 1, 3, 5};

/* ManagerInfo */
static SNMP_OID oid_lte_config_snmp[]                = {14, 2, 1, 4};
static SNMP_OID oid_lte_manager_ip[]                 = {14, 2, 1, 4, 1};
static SNMP_OID oid_lte_manager_mac[]                = {14, 2, 1, 4, 2};
static SNMP_OID oid_lte_manager_state[]              = {14, 2, 1, 4, 3};
static SNMP_OID oid_lte_manager_ip6[]                = {14, 2, 1, 4, 4};

/* protoInfo */
static SNMP_OID oid_lte_proto_snmp[]                 = {14, 2, 1, 5};
static SNMP_OID oid_lte_proto_ipv4Support[]          = {14, 2, 1, 5, 1};
static SNMP_OID oid_lte_proto_ipv6Support[]          = {14, 2, 1, 5, 2};

/*device heart  configure info*/
static SNMP_OID oid_lte_deviceHeart_snmp[]           = {14, 2, 1, 6};
static SNMP_OID oid_lte_deviceHerat_Interval[]       = {14, 2, 1, 6, 1};
static SNMP_OID oid_lte_sendTrap_Interval[]          = {14, 2, 1, 6, 2};

/* l2tp Rmt telnet info */
static SNMP_OID oid_lte_debug_info[]                 = {14, 2, 1, 7};
static SNMP_OID oid_lte_debug_SeverIp[]              = {14, 2, 1, 7, 1};
static SNMP_OID oid_lte_debug_ClientIp[]             = {14, 2, 1, 7, 2};
static SNMP_OID oid_lte_debug_SeverUserName[]        = {14, 2, 1, 7, 3};
static SNMP_OID oid_lte_debug_SeverPasswd[]          = {14, 2, 1, 7, 4};

/* lteHwInfo */
static SNMP_OID oid_lte_modulehw_info[]              = {14, 2, 2, 1};
static SNMP_OID oid_lte_module_type[]                = {14, 2, 2, 1, 1};
static SNMP_OID oid_lte_module_ver[]                 = {14, 2, 2, 1, 2};
static SNMP_OID oid_lte_module_imei[]                = {14, 2, 2, 1, 3};
static SNMP_OID oid_lte_sim_state[]                  = {14, 2, 2, 1, 4};
static SNMP_OID oid_lte_sim_iccid[]                  = {14, 2, 2, 1, 5};
static SNMP_OID oid_lte_sim_imsi[]                   = {14, 2, 2, 1, 6};
static SNMP_OID oid_lte_sim_lock[]                   = {14, 2, 2, 1, 7};

/*lteNetInfo*/
static SNMP_OID oid_lte_network_info[]               = {14, 2, 2, 2};
static SNMP_OID oid_lte_network_operCompany[]        = {14, 2, 2, 2, 1};
static SNMP_OID oid_lte_network_dailway[]            = {14, 2, 2, 2, 2};
static SNMP_OID oid_lte_network_dailstate[]          = {14, 2, 2, 2, 3};
static SNMP_OID oid_lte_signal_rssi[]                = {14, 2, 2, 2, 4};
static SNMP_OID oid_lte_signal_rsrp[]                = {14, 2, 2, 2, 5};
static SNMP_OID oid_lte_signal_rsrq[]                = {14, 2, 2, 2, 6};
static SNMP_OID oid_lte_signal_sinr[]                = {14, 2, 2, 2, 7};
static SNMP_OID oid_lte_signal_level[]               = {14, 2, 2, 2, 8};
static SNMP_OID oid_lte_network_accessNet[]          = {14, 2, 2, 2, 9};

/*oid_lte_cfg_info*/
static SNMP_OID oid_lte_cfg_info[]                   = {14, 2, 2, 3};
static SNMP_OID oid_lte_network_apnselect[]          = {14, 2, 2, 3, 1};
static SNMP_OID oid_lte_network_accessmode[]         = {14, 2, 2, 3, 2};
static SNMP_OID oid_lte_network_priaccessmode[]      = {14, 2, 2, 3, 3};
static SNMP_OID oid_lte_network_bandlist[]           = {14, 2, 2, 3, 4};
static SNMP_OID oid_lte_network_band[]               = {14, 2, 2, 3, 5};
static SNMP_OID oid_lte_network_channel[]            = {14, 2, 2, 3, 6};
static SNMP_OID oid_lte_network_bandlocked[]         = {14, 2, 2, 3, 7};
static SNMP_OID oid_lte_network_cid[]                = {14, 2, 2, 3, 8};
static SNMP_OID oid_lte_network_pcid[]               = {14, 2, 2, 3, 9};
static SNMP_OID oid_lte_network_pcidlocked[]         = {14, 2, 2, 3, 10};
static SNMP_OID oid_lte_network_channellocked[]      = {14, 2, 2, 3, 11};

/*lte_Station_Info*/
static SNMP_OID oid_lte_station_info[]               = {14, 2, 2, 4};
static SNMP_OID oid_lte_enodeb_info[]                = {14, 2, 2, 4, 1};
static SNMP_OID oid_lte_enodeb_id[]                  = {14, 2, 2, 4, 1, 1};
static SNMP_OID oid_lte_lc_id[]                      = {14, 2, 2, 4, 1, 2};
static SNMP_OID oid_lte_tac[]                        = {14, 2, 2, 4, 1, 3};
static SNMP_OID oid_lte_station_lock[]               = {14, 2, 2, 4, 2};
static SNMP_OID oid_lte_lock_type[]                  = {14, 2, 2, 4, 2, 1};
static SNMP_OID oid_lte_auto_lock[]                  = {14, 2, 2, 4, 2, 2};
static SNMP_OID oid_lte_auto_max[]                   = {14, 2, 2, 4, 2, 3};
static SNMP_OID oid_lte_lock_num[]                   = {14, 2, 2, 4, 2, 4};
static SNMP_OID oid_lte_station_lock_table[]         = {14, 2, 2, 4, 3};
static SNMP_OID oid_lte_station_lock_entry[]         = {14, 2, 2, 4, 3, 1};
static SNMP_OID oid_lte_lock_num_id[]                = {14, 2, 2, 4, 3, 1, 1};
static SNMP_OID oid_lte_lock_id[]                    = {14, 2, 2, 4, 3, 1, 2};
static SNMP_OID oid_lte_lock_cfg_state[]             = {14, 2, 2, 4, 3, 1, 3};
static SNMP_OID oid_lte_station_all_lock_id[]        = {14, 2, 2, 4, 4};


/* WifiInfo */
static SNMP_OID oid_lte_wifi_info[]                  = {14, 2, 3};
static SNMP_OID oid_lte_wifi_radiomode[]             = {14, 2, 3, 1};
static SNMP_OID oid_lte_wifi_channel[]               = {14, 2, 3, 2};
static SNMP_OID oid_lte_wifi_channelwidth[]          = {14, 2, 3, 3};
static SNMP_OID oid_lte_wifi_txPower[]               = {14, 2, 3, 4};
static SNMP_OID oid_lte_wifi_signLevel[]             = {14, 2, 3, 5};
static SNMP_OID oid_lte_wifi_noiseLevel[]            = {14, 2, 3, 6};
static SNMP_OID oid_lte_wifi_support[]               = {14, 2, 3, 7};

/* gpsInfo */
static SNMP_OID oid_lte_gps_info[]                   = {14, 2, 4, 1};
static SNMP_OID oid_lte_gps_longitude[]              = {14, 2, 4, 1, 1};
static SNMP_OID oid_lte_gps_latitude[]               = {14, 2, 4, 1, 2};
static SNMP_OID oid_lte_gps_elevation[]              = {14, 2, 4, 1, 3};
static SNMP_OID oid_lte_gps_velocity[]               = {14, 2, 4, 1, 4};
static SNMP_OID oid_lte_gps_orientation[]            = {14, 2, 4, 1, 5};
static SNMP_OID oid_lte_gps_uptime[]                 = {14, 2, 4, 1, 6};

/*trap*/
static SNMP_OID oid_lte_discover_trap[]              = {14, 3, 1, 1};
static SNMP_OID oid_lte_swupdate_trap[]              = {14, 3, 1, 2};
static SNMP_OID oid_lte_heart_trap[]                 = {14, 3, 1, 3};
static SNMP_OID oid_lte_debug_trap[]                 = {14, 3, 1, 4};
static SNMP_OID oid_lte_script_trap[]                = {14, 3, 1, 5};
//static SNMP_OID oid_lte_northbound_trap[]            = {14, 3, 1, 5};
static SNMP_OID oid_lte_status_trap[]                = {14, 3, 1, 6};

/* WanStat */
static SNMP_OID oid_lte_wan_state[]                  = {14, 4, 1};
static SNMP_OID oid_lte_wan_mode[]                   = {14, 4, 1, 1};
static SNMP_OID oid_lte_wan_RxBytes[]                = {14, 4, 1, 2};
static SNMP_OID oid_lte_wan_RxGBytes[]               = {14, 4, 1, 3};
static SNMP_OID oid_lte_wan_TxBytes[]                = {14, 4, 1, 4};
static SNMP_OID oid_lte_wan_TxGBytes[]               = {14, 4, 1, 5};
static SNMP_OID oid_lte_wan_wanName[]                = {14, 4, 1, 6};
static SNMP_OID oid_lte_wan_IpAddr[]                 = {14, 4, 1, 7};
static SNMP_OID oid_lte_wan_IpMask[]                 = {14, 4, 1, 8};
static SNMP_OID oid_lte_wan_Ip6Addr[]                = {14, 4, 1, 9};
static SNMP_OID oid_lte_wan_Ip6Mask[]                = {14, 4, 1, 10};
static SNMP_OID oid_lte_wan_Mac[]                    = {14, 4, 1, 11};
static SNMP_OID oid_lte_wan_AllRxBytes[]             = {14, 4, 1, 12};
static SNMP_OID oid_lte_wan_AllTxBytes[]             = {14, 4, 1, 13};


/* UserStat */
static SNMP_OID oid_lte_user_num[]                   = {14, 4, 2, 1};
static SNMP_OID oid_lte_user_table[]                 = {14, 4, 2, 10};
static SNMP_OID oid_lte_user_entry[]                 = {14, 4, 2, 10, 1};
static SNMP_OID oid_lte_user_id[]                    = {14, 4, 2, 10, 1, 1};
static SNMP_OID oid_lte_user_mac[]                   = {14, 4, 2, 10, 1, 2};
static SNMP_OID oid_lte_user_type[]                  = {14, 4, 2, 10, 1, 3};
static SNMP_OID oid_lte_user_RxBytes[]               = {14, 4, 2, 10, 1, 4};
static SNMP_OID oid_lte_user_RxGBytes[]              = {14, 4, 2, 10, 1, 5};
static SNMP_OID oid_lte_user_TxBytes[]               = {14, 4, 2, 10, 1, 6};
static SNMP_OID oid_lte_user_TxGBytes[]              = {14, 4, 2, 10, 1, 7};
static SNMP_OID oid_lte_user_IpAddr[]                = {14, 4, 2, 10, 1, 8};
static SNMP_OID oid_lte_user_Ip6Addr[]               = {14, 4, 2, 10, 1, 9};

/* LanStat */
static SNMP_OID oid_lte_lan_state[]                  = {14, 4, 3};
static SNMP_OID oid_lte_lan_name[]                   = {14, 4, 3, 1};
static SNMP_OID oid_lte_lan_IpAddr[]                 = {14, 4, 3, 2};
static SNMP_OID oid_lte_lan_IpMask[]                 = {14, 4, 3, 3};
static SNMP_OID oid_lte_lan_Ip6Addr[]                = {14, 4, 3, 4};
static SNMP_OID oid_lte_lan_Ip6Mask[]                = {14, 4, 3, 5};
static SNMP_OID oid_lte_lan_RxBytes[]                = {14, 4, 3, 6};
static SNMP_OID oid_lte_lan_RxGBytes[]               = {14, 4, 3, 7};
static SNMP_OID oid_lte_lan_TxBytes[]                = {14, 4, 3, 8};
static SNMP_OID oid_lte_lan_TxGBytes[]               = {14, 4, 3, 9};
static SNMP_OID oid_lte_lan_Isolate[]                = {14, 4, 3, 10};

/* lan port status */
static SNMP_OID oid_lte_lan_port[]                	 = {14, 4, 3, 11};

/* account Info */
static SNMP_OID oid_lte_account_info[]               = {14, 4, 4};
static SNMP_OID oid_lte_account_num[]                = {14, 4, 4, 1};
static SNMP_OID oid_lte_account_table[]              = {14, 4, 4, 2};
static SNMP_OID oid_lte_account_entry[]              = {14, 4, 4, 2, 1};
static SNMP_OID oid_lte_account_id[]                 = {14, 4, 4, 2, 1, 1};
static SNMP_OID oid_lte_account_name[]               = {14, 4, 4, 2, 1, 2};
static SNMP_OID oid_lte_account_oldpwd[]             = {14, 4, 4, 2, 1, 3};
static SNMP_OID oid_lte_account_newpwd[]             = {14, 4, 4, 2, 1, 4};
static SNMP_OID oid_lte_account_type[]               = {14, 4, 4, 2, 1, 5};
static SNMP_OID oid_lte_account_cfgstate[]           = {14, 4, 4, 2, 1, 6};

/* ntp Info */
static SNMP_OID oid_lte_ntp_info[]                   = {14, 4, 5};
static SNMP_OID oid_lte_ntp_synmode[]                = {14, 4, 5, 1};
static SNMP_OID oid_lte_ntp_server[]                 = {14, 4, 5, 2};
static SNMP_OID oid_lte_ntp_interval[]               = {14, 4, 5, 3};
static SNMP_OID oid_lte_ntp_date[]                   = {14, 4, 5, 4};
static SNMP_OID oid_lte_ntp_time[]                   = {14, 4, 5, 5};

/* log Info */
static SNMP_OID oid_lte_log_info[]                   = {14, 4, 6};
static SNMP_OID oid_lte_locallog_info[]              = {14, 4, 6, 1};
static SNMP_OID oid_lte_locallog_sw[]                = {14, 4, 6, 1, 1};
static SNMP_OID oid_lte_locallog_interval[]          = {14, 4, 6, 1, 2};
static SNMP_OID oid_lte_remotelog_info[]             = {14, 4, 6, 2};
static SNMP_OID oid_lte_remotelog_sw[]               = {14, 4, 6, 2, 1};
static SNMP_OID oid_lte_remotelog_serverip[]         = {14, 4, 6, 2, 2};
static SNMP_OID oid_lte_remotelog_serverport[]       = {14, 4, 6, 2, 3};
static SNMP_OID oid_lte_logcfg_info[]                = {14, 4, 6, 3};
static SNMP_OID oid_lte_logcfg_state[]               = {14, 4, 6, 3, 1};

/* l2tp lac Info */
static SNMP_OID oid_lte_lac_info[]                   = {14, 4, 7, 1};
static SNMP_OID oid_lte_lac_num[]                    = {14, 4, 7, 1, 1};
static SNMP_OID oid_lte_lac_table[]                  = {14, 4, 7, 1, 2};
static SNMP_OID oid_lte_lac_entry[]                  = {14, 4, 7, 1, 2, 1};
static SNMP_OID oid_lte_lac_id[]                     = {14, 4, 7, 1, 2, 1, 1};
static SNMP_OID oid_lte_lac_mode[]                   = {14, 4, 7, 1, 2, 1, 2};
static SNMP_OID oid_lte_lac_ip[]                     = {14, 4, 7, 1, 2, 1, 3};
static SNMP_OID oid_lte_lac_ipv6[]                   = {14, 4, 7, 1, 2, 1, 4};
static SNMP_OID oid_lte_lac_name[]                   = {14, 4, 7, 1, 2, 1, 5};
static SNMP_OID oid_lte_lac_pwdsw[]                  = {14, 4, 7, 1, 2, 1, 6};
static SNMP_OID oid_lte_lac_pwd[]                    = {14, 4, 7, 1, 2, 1, 7};
static SNMP_OID oid_lte_lac_cfgstate[]               = {14, 4, 7, 1, 2, 1, 8};

/* l2tp pppoe Info */
static SNMP_OID oid_lte_pppoe_info[]                 = {14, 4, 7, 2};
static SNMP_OID oid_lte_pppoe_user_um[]              = {14, 4, 7, 2, 1};
static SNMP_OID oid_lte_pppoe_table[]                = {14, 4, 7, 2, 2};
static SNMP_OID oid_lte_pppoe_entry[]                = {14, 4, 7, 2, 2, 1};
static SNMP_OID oid_lte_pppoe_user_id[]              = {14, 4, 7, 2, 2, 1, 1};
static SNMP_OID oid_lte_pppoe_ip_addr[]              = {14, 4, 7, 2, 2, 1, 2};
static SNMP_OID oid_lte_pppoe_ipv6_addr[]            = {14, 4, 7, 2, 2, 1, 3};
static SNMP_OID oid_lte_pppoe_server_name[]          = {14, 4, 7, 2, 2, 1, 4};
static SNMP_OID oid_lte_pppoe_user_name[]            = {14, 4, 7, 2, 2, 1, 5};
static SNMP_OID oid_lte_pppoe_password[]             = {14, 4, 7, 2, 2, 1, 6};
static SNMP_OID oid_lte_pppoe_all_user_names[]       = {14, 4, 7, 2, 3};


/* lte keepalive Info */
static SNMP_OID oid_lte_keepalive_info[]             = {14, 4, 8, 1};
static SNMP_OID oid_lte_keepalive_num[]              = {14, 4, 8, 1, 1};
static SNMP_OID oid_lte_keepalive_sw[]               = {14, 4, 8, 1, 2};
static SNMP_OID oid_lte_keepalive_interval[]         = {14, 4, 8, 1, 3};
static SNMP_OID oid_lte_keepalive_table[]            = {14, 4, 8, 1, 4};
static SNMP_OID oid_lte_keepalive_entry[]            = {14, 4, 8, 1, 4, 1};
static SNMP_OID oid_lte_keepalive_id[]               = {14, 4, 8, 1, 4, 1, 1};
static SNMP_OID oid_lte_keepalive_ip[]               = {14, 4, 8, 1, 4, 1, 2};
static SNMP_OID oid_lte_keepalive_ipv6[]             = {14, 4, 8, 1, 4, 1, 3};
static SNMP_OID oid_lte_keepalive_cfgstate[]         = {14, 4, 8, 1, 4, 1, 4};

/* system operation */
static SNMP_OID oid_lte_sys_oper[]                   = {14, 5, 1};
static SNMP_OID oid_lte_software_update[]            = {14, 5, 1, 1};
static SNMP_OID oid_lte_system_reboot[]              = {14, 5, 1, 2};
static SNMP_OID oid_lte_system_reset[]               = {14, 5, 1, 3};
static SNMP_OID oid_lte_system_debug[]               = {14, 5, 1, 4};
static SNMP_OID oid_lte_system_script[]              = {14, 5, 1, 5};

/* file operation */
static SNMP_OID oid_lte_file_trans[]                 = {14, 5, 2};
static SNMP_OID oid_lte_load_flag[]                  = {14, 5, 2, 1};
static SNMP_OID oid_lte_file_name[]                  = {14, 5, 2, 2};
static SNMP_OID oid_lte_file_type[]                  = {14, 5, 2, 3};
static SNMP_OID oid_lte_trans_protocal[]             = {14, 5, 2, 4};
static SNMP_OID oid_lte_server_addr[]                = {14, 5, 2, 5};
static SNMP_OID oid_lte_server_port[]                = {14, 5, 2, 6};
static SNMP_OID oid_lte_server_username[]            = {14, 5, 2, 7};
static SNMP_OID oid_lte_server_password[]            = {14, 5, 2, 8};
static SNMP_OID oid_lte_trans_status[]               = {14, 5, 2, 9};
static SNMP_OID oid_lte_fail_reason[]                = {14, 5, 2, 10};
static SNMP_OID oid_lte_file_version[]               = {14, 5, 2, 11};


#endif

