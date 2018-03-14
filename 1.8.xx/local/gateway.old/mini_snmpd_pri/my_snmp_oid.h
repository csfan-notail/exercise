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
#if 0
/*ruijie ap rootoid:1.3.6.1.4.1.4881.1.3.1.1.110*/
/* BEGIN: Modified by zoucaihong, 2012/12/4 */
/*MIB-2*/
#define OID_MGMT                            2
#define OID_MIB2                            OID_MGMT, 1

/**
 *	RFC1213和RFC2863
 */
/* BEGIN: Added by zoucaihong, 2012/12/4 
add system group and interfaces group of RFC1213*/
#define OID_SYSTEM                          OID_MIB2, 1
#define OID_INTERFACES                      OID_MIB2, 2
#define OID_INTERFACESTABLE                 OID_INTERFACES, 2
/* END: Added by zoucaihong, 2012/12/4 */

/*private mib*/
#define OID_PRIVATE                         4
#define OID_ENTERPRISES                     OID_PRIVATE, 1
#define OID_DLINK                           OID_ENTERPRISES, 4881
/* END: Modified by zoucaihong, 2012/12/4 */
#define OID_DLINK_PRODUCTS                  OID_DLINK, 1
#define OID_DLINK_WLAN                      OID_DLINK_PRODUCTS, 3
#define OID_DLINK_AP_MIB                    OID_DLINK_WLAN, 1
#define OID_DLINK_AP_OBJECT_GROUP           OID_DLINK_AP_MIB, 1
#define OID_DLINK_AP_OBJECT                 OID_DLINK_AP_OBJECT_GROUP, 110

//#define OID_AP_ALARM_TRAP_OBJECT            OID_DLINK_AP_OBJECT, 1
#define OID_AP_CONFIG                       OID_DLINK_AP_OBJECT, 11
//#define OID_AP_ALARM                        OID_DLINK_AP_OBJECT, 12
#define OID_AP_STAT                         OID_DLINK_AP_OBJECT, 13
#define OID_AP_OPER                         OID_DLINK_AP_OBJECT, 14
#else
#define OID_AP_ALARM_TRAP_OBJECT            1
#define OID_AP_CONFIG                       11
#define OID_AP_ALARM                        12
#define OID_AP_STAT                         13
#define OID_AP_OPER                         14
#endif
/**
 *	config
 */
#define OID_AP_CONFIG_SYS                   OID_AP_CONFIG, 1
#define OID_AP_CONFIG_SYS_DEVINFO           OID_AP_CONFIG_SYS, 1
#define OID_AP_CONFIG_SYS_SWINFO            OID_AP_CONFIG_SYS, 2
#define OID_AP_CONFIG_SYS_HWINFO            OID_AP_CONFIG_SYS, 3
#define OID_AP_CONFIG_SYS_IP                OID_AP_CONFIG_SYS, 4
#define OID_AP_CONFIG_SYS_IP_TABLE          OID_AP_CONFIG_SYS_IP, 10
#if 0
#define OID_AP_CONFIG_SYS_LEGALAP           OID_AP_CONFIG_SYS, 5
#define OID_AP_CONFIG_SYS_LEGALAP_TABLE     OID_AP_CONFIG_SYS_LEGALAP, 10
#define OID_AP_CONFIG_SYS_FLOWCTL           OID_AP_CONFIG_SYS, 6

/* BEGIN: Added by zoucaihong, 2013/3/11 */
#define OID_AP_CONFIG_ADVANCE               OID_AP_CONFIG, 2
#define OID_AP_CONFIG_ADVANCE_5GPRIOR       OID_AP_CONFIG_ADVANCE, 1
#define OID_AP_CONFIG_ADVANCE_11NPRIOR      OID_AP_CONFIG_ADVANCE, 2
/* END: Added by zoucaihong, 2013/3/11 */
#endif

#define OID_AP_CONFIG_WIFIDEVICE            OID_AP_CONFIG, 10
#define OID_AP_CONFIG_WIFIBASIC             OID_AP_CONFIG_WIFIDEVICE, 1
#define OID_AP_CONFIG_WIFICHANNEL           OID_AP_CONFIG_WIFIDEVICE, 2
#define OID_AP_CONFIG_WIFIPOWER             OID_AP_CONFIG_WIFIDEVICE, 3
#define OID_AP_CONFIG_WIFIADVANCED          OID_AP_CONFIG_WIFIDEVICE, 4
/* BEGIN: Added by Zhou Shouya, 2013/8/29   PN:VLAN */
#define OID_AP_CONFIG_WIFIRATE              OID_AP_CONFIG_WIFIDEVICE, 5
/* END:   Added by Zhou Shouya, 2013/8/29   PN:VLAN */

/* BEGIN: Added by zoucaihong, 2013/3/21 */
/* 2.4G卡配置和5G卡配置分开 */
#define OID_AP_CONFIG_WIFIDEVICE5           OID_AP_CONFIG, 15
#define OID_AP_CONFIG_WIFIBASIC5            OID_AP_CONFIG_WIFIDEVICE5, 1
#define OID_AP_CONFIG_WIFICHANNEL5          OID_AP_CONFIG_WIFIDEVICE5, 2
#define OID_AP_CONFIG_WIFIPOWER5            OID_AP_CONFIG_WIFIDEVICE5, 3
#define OID_AP_CONFIG_WIFIADVANCED5         OID_AP_CONFIG_WIFIDEVICE5, 4
/* END: Added by zoucaihong, 2013/3/21 */
/* BEGIN: Added by Zhou Shouya, 2013/8/29   PN:VLAN */
#define OID_AP_CONFIG_WIFIRATE5             OID_AP_CONFIG_WIFIDEVICE5, 5
/* END:   Added by Zhou Shouya, 2013/8/29   PN:VLAN */

#define OID_AP_CONFIG_VAP                   OID_AP_CONFIG, 20
#define OID_AP_CONFIG_VAPBASIC_TABLE        OID_AP_CONFIG_VAP, 10
#define OID_AP_CONFIG_VAPSECURITY_TABLE     OID_AP_CONFIG_VAP, 11

#define OID_AP_CONFIG_ALARM                 OID_AP_CONFIG, 30

#define OID_AP_CONFIG_TRAPDES               OID_AP_CONFIG, 40
//#define OID_AP_CONFIG_TRAPDES_TABLE         OID_AP_CONFIG_TRAPDES, 10

#define OID_AP_CONFIG_VLAN                  OID_AP_CONFIG, 50
#define OID_AP_CONFIG_VLAN_TABLE            OID_AP_CONFIG_VLAN, 10

#define OID_AP_CONFIG_SSID                  OID_AP_CONFIG, 60
#define OID_AP_CONFIG_SSID_TABLE            OID_AP_CONFIG_SSID, 10

#define OID_AP_CONFIG_DOMAINFILTER          OID_AP_CONFIG, 62
/* BEGIN: Added by zoucaihong, 2015/3/24 */
#define OID_AP_CONFIG_USER                  OID_AP_CONFIG, 70
#define OID_AP_CONFIG_GWSSID                OID_AP_CONFIG, 71
#define OID_AP_CONFIG_AUTH                  OID_AP_CONFIG, 72
#define OID_AP_CONFIG_BATCH_BWLIST          OID_AP_CONFIG, 73
/* END: Added by zoucaihong, 2015/3/24 */
#define OID_AP_CONFIG_USERSPEED             OID_AP_CONFIG, 74
#define OID_AP_CONFIG_WHITE_USERSPEED       OID_AP_CONFIG, 75
#define OID_AP_CONFIG_USERLOGOUT            OID_AP_CONFIG, 76
#define OID_AP_CONFIG_GET_SECRET_KEY        OID_AP_CONFIG, 77
/**
 * stat
 */
#define OID_AP_STAT_SYS                     OID_AP_STAT, 101

#define OID_AP_STAT_WIFI                    OID_AP_STAT, 103
#define OID_AP_STAT_WIFI_TABLE              OID_AP_STAT_WIFI, 10

#define OID_AP_STAT_WIFIVAP                 OID_AP_STAT, 104
#define OID_AP_STAT_WIFIVAP_TABLE           OID_AP_STAT_WIFIVAP, 10

#define OID_AP_STAT_STATION                 OID_AP_STAT, 105
#define OID_AP_STAT_STATION_TABLE           OID_AP_STAT_STATION, 10

#if 0
#define OID_AP_STAT_24COCHAINTERFINFO         OID_AP_STAT, 106
#define OID_AP_STAT_24COCHAINTERFINFO_TABLE   OID_AP_STAT_24COCHAINTERFINFO, 10

#define OID_AP_STAT_5COCHAINTERFINFO          OID_AP_STAT, 107
#define OID_AP_STAT_5COCHAINTERFINFO_TABLE    OID_AP_STAT_5COCHAINTERFINFO, 10

#define OID_AP_STAT_24ADJCHAINTERFINFO         OID_AP_STAT, 108
#define OID_AP_STAT_24ADJCHAINTERFINFO_TABLE   OID_AP_STAT_24ADJCHAINTERFINFO, 10

#define OID_AP_STAT_5ADJCHAINTERFINFO          OID_AP_STAT, 109
#define OID_AP_STAT_5ADJCHAINTERFINFO_TABLE    OID_AP_STAT_5ADJCHAINTERFINFO, 10

#define OID_AP_STAT_24WLANDEVINTERFINFO         OID_AP_STAT, 110
#define OID_AP_STAT_24WLANDEVINTERFINFO_TABLE   OID_AP_STAT_24WLANDEVINTERFINFO, 10

#define OID_AP_STAT_5WLANDEVINTERFINFO          OID_AP_STAT, 111
#define OID_AP_STAT_5WLANDEVINTERFINFO_TABLE    OID_AP_STAT_5WLANDEVINTERFINFO, 10

#define OID_AP_STAT_NONWLANDEVINTERFINFO         OID_AP_STAT, 112
#define OID_AP_STAT_NONWLANDEVINTERFINFO_TABLE   OID_AP_STAT_NONWLANDEVINTERFINFO, 10

#define OID_AP_STAT_BRMODESTA               OID_AP_STAT, 113
#define OID_AP_STAT_BRMODESTA_TABLE         OID_AP_STAT_BRMODESTA, 10

#define OID_AP_STAT_24ROGUEAPINFO           OID_AP_STAT, 114
#define OID_AP_STAT_24ROGUEAPINFO_TABLE     OID_AP_STAT_24ROGUEAPINFO, 10

#define OID_AP_STAT_5ROGUEAPINFO            OID_AP_STAT, 115
#define OID_AP_STAT_5ROGUEAPINFO_TABLE      OID_AP_STAT_5ROGUEAPINFO, 10
#endif

/* BEGIN: Added by zoucaihong, 2015/3/24 */
#define OID_AP_STAT_ONLINEUSER              OID_AP_STAT, 120
#define OID_AP_STAT_APINFO                  OID_AP_STAT, 121
#define OID_AP_STAT_VERSION                 OID_AP_STAT, 122
#define OID_AP_STAT_AP_STA                  OID_AP_STAT, 123
/* END: Added by zoucaihong, 2015/3/24 */

/* BEGIN: Added by zoucaihong, 2014/5/14 */
/**
 * oper
 */
#define OID_AP_OPER_SYSOPER                 OID_AP_OPER, 1
#define OID_AP_OPER_FILETRANSCONFIG         OID_AP_OPER, 2
/* END: Added by zoucaihong, 2014/5/14 */
/* BEGIN: Added by zoucaihong, 2015/3/24 */
#define OID_AP_OPER_GWAP                    OID_AP_OPER, 3
/* END: Added by zoucaihong, 2015/3/24 */

/**
 * alarm
 */
#define OID_AP_ALARM_TRAP                   OID_AP_ALARM, 10

#define OID_AP_ALARM_SYS_TRAP               OID_AP_ALARM_TRAP, 1
#define OID_AP_ALARM_WIDS_TRAP              OID_AP_ALARM_TRAP, 2

/******************************* 静态变量 ********************************/
/******************************* RFC1213和RFC2863 ********************************/
#if 0
/* BEGIN: Added by zoucaihong, 2012/12/4 */
static OID oid_system[] = {OID_SYSTEM};
static OID oid_interfaces[] = {OID_INTERFACES};
static OID oid_interfaces_table[] = {OID_INTERFACESTABLE};
/* END: Added by zoucaihong, 2012/12/4 */
#endif
/******************************* 配置 ********************************/
static OID oid_ap_config_sys_devinfo[]          = {OID_AP_CONFIG_SYS_DEVINFO};
static OID oid_ap_config_sys_swinfo[]           = {OID_AP_CONFIG_SYS_SWINFO};
static OID oid_ap_config_sys_hwinfo[]           = {OID_AP_CONFIG_SYS_HWINFO};
/* BEGIN: Added by zoucaihong, 2013/2/25 */
static OID oid_ap_config_sys_ip[]               = {OID_AP_CONFIG_SYS_IP};
static OID oid_ap_config_sys_ip_table[]         = {OID_AP_CONFIG_SYS_IP_TABLE};
#if 0
/* END: Added by zoucaihong, 2013/2/25 */
static OID oid_ap_config_sys_legalap[]          = {OID_AP_CONFIG_SYS_LEGALAP};
static OID oid_ap_config_sys_legalap_table[]    = {OID_AP_CONFIG_SYS_LEGALAP_TABLE};

static OID oid_ap_config_sys_flowctl[]          = {OID_AP_CONFIG_SYS_FLOWCTL};

/* BEGIN: Added by zoucaihong, 2013/3/11 */
static OID oid_ap_config_advance_5gprior[]      = {OID_AP_CONFIG_ADVANCE_5GPRIOR};
static OID oid_ap_config_advance_11nprior[]     = {OID_AP_CONFIG_ADVANCE_11NPRIOR};
/* END: Added by zoucaihong, 2013/3/11 */
#endif
static OID oid_ap_config_wifibasic[]            = {OID_AP_CONFIG_WIFIBASIC};
static OID oid_ap_config_wifichannel[]          = {OID_AP_CONFIG_WIFICHANNEL};
static OID oid_ap_config_wifirate[]             = {OID_AP_CONFIG_WIFIRATE};
static OID oid_ap_config_wifipower[]            = {OID_AP_CONFIG_WIFIPOWER};
static OID oid_ap_config_wifiadvanced[]         = {OID_AP_CONFIG_WIFIADVANCED};
//static OID oid_ap_config_wifiwids_table[]       = {OID_AP_CONFIG_WIFIWIDS_TABLE};
/* BEGIN: Added by zoucaihong, 2013/3/21 */
/* 2.4G卡配置和5G卡配置分开 */
static OID oid_ap_config_wifibasic5[]           = {OID_AP_CONFIG_WIFIBASIC5};
static OID oid_ap_config_wifichannel5[]         = {OID_AP_CONFIG_WIFICHANNEL5};
static OID oid_ap_config_wifipower5[]           = {OID_AP_CONFIG_WIFIPOWER5};
static OID oid_ap_config_wifiadvanced5[]        = {OID_AP_CONFIG_WIFIADVANCED5};
/* END: Added by zoucaihong, 2013/3/21 */
static OID oid_ap_config_wifirate5[]            = {OID_AP_CONFIG_WIFIRATE5};

static OID oid_ap_config_vap[]                  = {OID_AP_CONFIG_VAP};
static OID oid_ap_config_vapbasic_table[]      = {OID_AP_CONFIG_VAPBASIC_TABLE};
static OID oid_ap_config_vapsecurity_table[]    = {OID_AP_CONFIG_VAPSECURITY_TABLE};

static OID oid_ap_config_alarm[]                = {OID_AP_CONFIG_ALARM};

static OID oid_ap_config_trapdes[] = {OID_AP_CONFIG_TRAPDES};
//static OID oid_ap_config_trapdes_table[] = {OID_AP_CONFIG_TRAPDES_TABLE};

static OID oid_ap_config_vlan[] = {OID_AP_CONFIG_VLAN};
static OID oid_ap_config_vlan_table[] = {OID_AP_CONFIG_VLAN_TABLE};

static OID oid_ap_config_ssid[] = {OID_AP_CONFIG_SSID};
static OID oid_ap_config_ssid_table[] = {OID_AP_CONFIG_SSID_TABLE};

static OID oid_ap_config_domainfilter[]         = {OID_AP_CONFIG_DOMAINFILTER};

/* BEGIN: Added by zoucaihong, 2015/3/24 */
static OID oid_ap_config_user[]     = {OID_AP_CONFIG_USER};
static OID oid_ap_config_gwssid[]   = {OID_AP_CONFIG_GWSSID};
static OID oid_ap_config_auth[]     = {OID_AP_CONFIG_AUTH};
static OID oid_ap_config_batch_bwlist[] = {OID_AP_CONFIG_BATCH_BWLIST};
/* END: Added by zoucaihong, 2015/3/24 */
static OID oid_ap_config_user_speed[] = {OID_AP_CONFIG_USERSPEED};
static OID oid_ap_config_white_user_speed[] = {OID_AP_CONFIG_WHITE_USERSPEED};
static OID oid_ap_config_user_logout[] = {OID_AP_CONFIG_USERLOGOUT};
static OID oid_ap_config_get_secret_key[] = {OID_AP_CONFIG_GET_SECRET_KEY};
/******************************* 统计 ********************************/
static OID oid_ap_stat_sys[]                    = {OID_AP_STAT_SYS};

static OID oid_ap_stat_wifi[]                   = {OID_AP_STAT_WIFI};
static OID oid_ap_stat_wifi_table[]             = {OID_AP_STAT_WIFI_TABLE};

static OID oid_ap_stat_wifivap[]               = {OID_AP_STAT_WIFIVAP};
static OID oid_ap_stat_wifivap_table[]         = {OID_AP_STAT_WIFIVAP_TABLE};

static OID oid_ap_stat_station[] = {OID_AP_STAT_STATION};
static OID oid_ap_stat_station_table[] = {OID_AP_STAT_STATION_TABLE};

#if 0
static OID oid_ap_stat_24cochainterfinfo[]        = {OID_AP_STAT_24COCHAINTERFINFO};
static OID oid_ap_stat_24cochainterfinfo_table[]  = {OID_AP_STAT_24COCHAINTERFINFO_TABLE};
static OID oid_ap_stat_5cochainterfinfo[]        = {OID_AP_STAT_5COCHAINTERFINFO};
static OID oid_ap_stat_5cochainterfinfo_table[]  = {OID_AP_STAT_5COCHAINTERFINFO_TABLE};
static OID oid_ap_stat_24adjchainterfinfo[]       = {OID_AP_STAT_24ADJCHAINTERFINFO};
static OID oid_ap_stat_24adjchainterfinfo_table[] = {OID_AP_STAT_24ADJCHAINTERFINFO_TABLE};
static OID oid_ap_stat_5adjchainterfinfo[]       = {OID_AP_STAT_5ADJCHAINTERFINFO};
static OID oid_ap_stat_5adjchainterfinfo_table[] = {OID_AP_STAT_5ADJCHAINTERFINFO_TABLE};
static OID oid_ap_stat_24wlandevinterfinfo[]      = {OID_AP_STAT_24WLANDEVINTERFINFO};
static OID oid_ap_stat_24wlandevinterfinfo_table[]= {OID_AP_STAT_24WLANDEVINTERFINFO_TABLE};
static OID oid_ap_stat_5wlandevinterfinfo[]      = {OID_AP_STAT_5WLANDEVINTERFINFO};
static OID oid_ap_stat_5wlandevinterfinfo_table[]= {OID_AP_STAT_5WLANDEVINTERFINFO_TABLE};

static OID oid_ap_stat_nonwlandevinterfinfo[]      = {OID_AP_STAT_NONWLANDEVINTERFINFO};
static OID oid_ap_stat_nonwlandevinterfinfo_table[]= {OID_AP_STAT_NONWLANDEVINTERFINFO_TABLE};

static OID oid_ap_stat_brmodesta[]              = {OID_AP_STAT_BRMODESTA};
static OID oid_ap_stat_brmodesta_table[]        = {OID_AP_STAT_BRMODESTA_TABLE};

static OID oid_ap_stat_24rogueapinfo[]          = {OID_AP_STAT_24ROGUEAPINFO};
static OID oid_ap_stat_24rogueapinfo_table[]    = {OID_AP_STAT_24ROGUEAPINFO_TABLE};

static OID oid_ap_stat_5rogueapinfo[]           = {OID_AP_STAT_5ROGUEAPINFO};
static OID oid_ap_stat_5rogueapinfo_table[]     = {OID_AP_STAT_5ROGUEAPINFO_TABLE};
#endif

/* BEGIN: Added by zoucaihong, 2015/3/24 */
static OID oid_ap_stat_onlineuser[] = {OID_AP_STAT_ONLINEUSER};
static OID oid_ap_stat_apinfo[]     = {OID_AP_STAT_APINFO};
static OID oid_ap_stat_version[]    = {OID_AP_STAT_VERSION};
static OID oid_ap_stat_ap_sta[]     = {OID_AP_STAT_AP_STA};
/* END: Added by zoucaihong, 2015/3/24 */

/* BEGIN: Added by zoucaihong, 2014/5/14 */
/******************************* 设备操作 ********************************/
static OID oid_ap_oper_sysoper[]                = {OID_AP_OPER_SYSOPER};
static OID oid_ap_oper_filetrans[]              = {OID_AP_OPER_FILETRANSCONFIG};
/* END: Added by zoucaihong, 2014/5/14 */
/* BEGIN: Added by zoucaihong, 2015/3/24 */
static OID oid_ap_oper_gwap[]                   = {OID_AP_OPER_GWAP};
/* END: Added by zoucaihong, 2015/3/24 */

/******************************* 告警通告 ********************************/
static SNMP_OID oid_snmp_trap[]                 = {1,3,6,1,6,3,1,1,4,1,0};

static SNMP_OID oid_ap_alarmSn[]                = {OID_AP_ALARM_TRAP_OBJECT, 1};
static SNMP_OID oid_ap_alarmNeName[]            = {OID_AP_ALARM_TRAP_OBJECT, 2};
static SNMP_OID oid_ap_alarmLevel[]             = {OID_AP_ALARM_TRAP_OBJECT, 3};
static SNMP_OID oid_ap_alarmType[]              = {OID_AP_ALARM_TRAP_OBJECT, 4};
static SNMP_OID oid_ap_alarmReasonID[]          = {OID_AP_ALARM_TRAP_OBJECT, 5};
static SNMP_OID oid_ap_alarmReason[]            = {OID_AP_ALARM_TRAP_OBJECT, 6};
static SNMP_OID oid_ap_alarmRaiseTime[]         = {OID_AP_ALARM_TRAP_OBJECT, 7};
static SNMP_OID oid_ap_alarmStatus[]            = {OID_AP_ALARM_TRAP_OBJECT, 8};
static SNMP_OID oid_ap_alarmTitle[]             = {OID_AP_ALARM_TRAP_OBJECT, 9};
static SNMP_OID oid_ap_alarmInfo[]              = {OID_AP_ALARM_TRAP_OBJECT, 10};
/************************************************************************/
/* apAlarm.apAlarmTrap                                               */
/************************************************************************/
static SNMP_OID oid_ap_alarm_trap[]             = {OID_AP_ALARM_TRAP};

static SNMP_OID oid_ap_alarm_sys_apWarmStart_trap[]         = {OID_AP_ALARM_SYS_TRAP, 1};
static SNMP_OID oid_ap_alarm_sys_apColdStart_trap[]         = {OID_AP_ALARM_SYS_TRAP, 2};
static SNMP_OID oid_ap_alarm_sys_apCpuUsageTooHigh_trap[]         = {OID_AP_ALARM_SYS_TRAP, 3};
static SNMP_OID oid_ap_alarm_sys_apCpuUsageTooHighClear_trap[]         = {OID_AP_ALARM_SYS_TRAP, 4};
static SNMP_OID oid_ap_alarm_sys_apMemUsageTooHigh_trap[]         = {OID_AP_ALARM_SYS_TRAP, 5};
static SNMP_OID oid_ap_alarm_sys_apMemUsageTooHighClear_trap[]         = {OID_AP_ALARM_SYS_TRAP, 6};
/* BEGIN: Added by zoucaihong, 2014/5/13 */
static SNMP_OID oid_dev_discover_trap[]         = {OID_AP_ALARM_SYS_TRAP, 7};
static SNMP_OID oid_ap_alarm_sys_apSoftwareUpdate_trap[]         = {OID_AP_ALARM_SYS_TRAP, 8};
/* END: Added by zoucaihong, 2014/5/13 */
static SNMP_OID oid_ap_alarm_fault_trap[]         = {OID_AP_ALARM_SYS_TRAP, 10};
static SNMP_OID oid_ap_alarm_heart_trap[]       = {OID_AP_ALARM_SYS_TRAP, 11};
/* BEGIN: Added by zoucaihong, 2015/3/25 */
static SNMP_OID oid_ap_alarm_gw_basicinfo_trap[] = {OID_AP_ALARM_SYS_TRAP, 13};
static SNMP_OID oid_ap_alarm_gw_dyninfo_trap[] = {OID_AP_ALARM_SYS_TRAP, 14};
static SNMP_OID oid_ap_alarm_gw_zcbasicinfo_trap[] = {OID_AP_ALARM_SYS_TRAP, 17};
static SNMP_OID oid_ap_alarm_userinfo_trap[] = {OID_AP_ALARM_SYS_TRAP, 15};
static SNMP_OID oid_ap_alarm_clientinfo_trap[] = {OID_AP_ALARM_SYS_TRAP, 16};
/* END: Added by zoucaihong, 2015/3/25 */

static SNMP_OID oid_ap_alarm_wids_cointerfre_trap[]         = {OID_AP_ALARM_WIDS_TRAP, 1};
static SNMP_OID oid_ap_alarm_wids_cointerfreClear_trap[]         = {OID_AP_ALARM_WIDS_TRAP, 2};
static SNMP_OID oid_ap_alarm_wids_neinterfre_trap[]         = {OID_AP_ALARM_WIDS_TRAP, 3};
static SNMP_OID oid_ap_alarm_wids_neinterfreClear_trap[]         = {OID_AP_ALARM_WIDS_TRAP, 4};
static SNMP_OID oid_ap_alarm_wids_wlanDevInterfre_trap[]         = {OID_AP_ALARM_WIDS_TRAP, 5};
static SNMP_OID oid_ap_alarm_wids_wlanDevInterfreClear_trap[]         = {OID_AP_ALARM_WIDS_TRAP, 6};
static SNMP_OID oid_ap_alarm_wids_otherDevInterfre_trap[]         = {OID_AP_ALARM_WIDS_TRAP, 7};
static SNMP_OID oid_ap_alarm_wids_otherDevInterfreClear_trap[]         = {OID_AP_ALARM_WIDS_TRAP, 8};
/* END: Added by zoucaihong, 2013/1/23 */

/* BEGIN: Added by zoucaihong, 2014/5/13 */
static SNMP_OID oid_dev_mac[] = {OID_AP_CONFIG_SYS_DEVINFO, 1};
static SNMP_OID oid_dev_uptime[] = {OID_AP_CONFIG_SYS_DEVINFO, 2};
static SNMP_OID oid_dev_ip[] = {OID_AP_CONFIG_SYS_DEVINFO, 9};
static SNMP_OID oid_dev_managestate[] = {OID_AP_CONFIG_SYS_DEVINFO, 10};
static SNMP_OID oid_dev_model[] = {OID_AP_CONFIG_SYS_DEVINFO, 11};
static SNMP_OID oid_dev_modelname[] = {OID_AP_CONFIG_SYS_DEVINFO, 12};
static SNMP_OID oid_dev_sysModel[] = {OID_AP_CONFIG_SYS_DEVINFO, 13};
static SNMP_OID oid_dev_heart_period[] = {OID_AP_CONFIG_SYS_DEVINFO, 15};
static SNMP_OID oid_dev_type[] = {OID_AP_CONFIG_SYS_DEVINFO, 16};
static SNMP_OID oid_dev_sysName[] = {OID_AP_CONFIG_SYS_DEVINFO, 17};
static SNMP_OID oid_dev_hotId[] = {OID_AP_CONFIG_SYS_DEVINFO, 18};

static SNMP_OID oid_dev_software_ver[] = {OID_AP_CONFIG_SYS_SWINFO, 1};
static SNMP_OID oid_dev_software_targetver[] = {OID_AP_CONFIG_SYS_SWINFO, 2};
static SNMP_OID oid_dev_software_oemver[] = {OID_AP_CONFIG_SYS_SWINFO, 3};
static SNMP_OID oid_dev_hardware_ver[] = {OID_AP_CONFIG_SYS_HWINFO, 5};
static SNMP_OID oid_dev_hardware_type[] = {OID_AP_CONFIG_SYS_HWINFO, 6};
static SNMP_OID oid_dev_board_ver[] = {OID_AP_CONFIG_SYS_HWINFO, 7};
static SNMP_OID oid_wifi0_channel[] = {OID_AP_CONFIG_WIFICHANNEL, 3};
static SNMP_OID oid_wifi0_txpower[] = {OID_AP_CONFIG_WIFIPOWER, 4};
static SNMP_OID oid_wifi1_channel[] = {OID_AP_CONFIG_WIFICHANNEL5, 3};
static SNMP_OID oid_wifi1_txpower[] = {OID_AP_CONFIG_WIFIPOWER5, 4};
static SNMP_OID oid_filetrans_status[] = {OID_AP_OPER_FILETRANSCONFIG, 9};
//2014-7-16 zch add
static SNMP_OID oid_file_type[] = {OID_AP_OPER_FILETRANSCONFIG, 3};
/* 贝联项目需求 */
static SNMP_OID oid_gw_partner[] = {OID_AP_CONFIG_SYS_DEVINFO, 22};
static SNMP_OID oid_gw_line[] = {OID_AP_CONFIG_SYS_DEVINFO, 23};
static SNMP_OID oid_gw_ipmode[] = {OID_AP_CONFIG_SYS_IP_TABLE, 1, 2 };
static SNMP_OID oid_gw_runstate[] = {OID_AP_CONFIG_SYS_DEVINFO, 19};
static SNMP_OID oid_gw_apnum[] = {OID_AP_STAT_SYS, 9};
static SNMP_OID oid_gw_aponlinenum[] = {OID_AP_STAT_SYS, 10};
static SNMP_OID oid_gw_apofflinenum[] = {OID_AP_STAT_SYS, 11};
static SNMP_OID oid_gw_onlineusernum[] = {OID_AP_STAT_SYS, 12};
static SNMP_OID oid_gw_feelusernum[] = {OID_AP_STAT_SYS, 13};
static SNMP_OID oid_gw_cpuusage[] = {OID_AP_STAT_SYS, 1};
static SNMP_OID oid_gw_memusage[] = {OID_AP_STAT_SYS, 4};
static SNMP_OID oid_gw_totalup[] = {OID_AP_STAT_SYS, 14};
static SNMP_OID oid_gw_totaldw[] = {OID_AP_STAT_SYS, 15};
static SNMP_OID oid_gw_upspeed[] = {OID_AP_STAT_SYS, 16};
static SNMP_OID oid_gw_dwspeed[] = {OID_AP_STAT_SYS, 17};
static SNMP_OID oid_gw_totalband[] = {OID_AP_STAT_SYS, 18};
static SNMP_OID oid_gw_bandused[] = {OID_AP_STAT_SYS, 19};
/* END: Added by zoucaihong, 2014/5/13 */
/* 中传项目需求 */
static SNMP_OID oid_dev_manufacture[] = {OID_AP_CONFIG_SYS_DEVINFO, 24};
static SNMP_OID oid_dev_sn[] = {OID_AP_CONFIG_SYS_DEVINFO, 25};
static SNMP_OID oid_dev_ssid[] = {OID_AP_CONFIG_GWSSID, 1};
static SNMP_OID oid_user_mac[] = {OID_AP_STAT_ONLINEUSER, 1, 1 };
static SNMP_OID oid_user_ip[] = {OID_AP_STAT_ONLINEUSER, 1, 10};
static SNMP_OID oid_user_ssid[] = {OID_AP_STAT_ONLINEUSER, 1, 11};
static SNMP_OID oid_user_logintime[] = {OID_AP_STAT_ONLINEUSER, 1, 12};
static SNMP_OID oid_user_logouttime[] = {OID_AP_STAT_ONLINEUSER, 1, 13};
static SNMP_OID oid_user_onlinetime[] = {OID_AP_STAT_ONLINEUSER, 1, 14};
static SNMP_OID oid_user_logouttype[] = {OID_AP_STAT_ONLINEUSER, 1, 15};
static SNMP_OID oid_client_mac[] = {OID_AP_STAT_AP_STA, 1, 2};
static SNMP_OID oid_client_ip[] = {OID_AP_STAT_AP_STA, 1, 5};
static SNMP_OID oid_client_ssid[] = {OID_AP_STAT_AP_STA, 1, 10};
static SNMP_OID oid_client_logintime[] = {OID_AP_STAT_AP_STA, 1, 11};

#endif

