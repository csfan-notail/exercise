/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : nvram_config.h
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/5/2
  Last Modified :
  Description   : nvram_config.h header file
                : nvram配置数据的宏定义和网页功能的宏定义
  Function List :
  History       :
  1.Date        : 2013/5/2
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#ifndef __NVRAM_CONFIG_H__
#define __NVRAM_CONFIG_H__

/*----------- 返回值 ------------*/
#define NvramResultSuccess  0
#define NvramResultFailed   1

/*-------------- nvram 字段定义 --------------*/

/* nvram 长度定义 */
#define NVRAM_LEN           32
#define NVRAM_NAME_MAX_LEN		128
#define NVRAM_VALUE_MAX_LEN		256

#define VAP_ADVANCED_MAC_NUM_MAX    64       /* 允许上网主机MAC最大数目*/

#define NVRAM_NAME_TIME_SNTP_SWITCH     "time_sntp_switch"
#define NVRAM_NAME_TIME_SNTP_URL        "time_sntp_url"
#define NVRAM_NAME_TIME_SNTP_CYCLE      "time_sntp_cycle"


#define INIT_SUCCESS        "init_success"

/* 设备基本配置 */
#ifdef OPENWRT
#define GATEWAY "gateway.@gateway[0]."
#define NVRAM_DEVICE_TYPE                   "device_type"
#define NVRAM_DEVICE_SN                     "device_sn"
#define NVRAM_DEVICE_NAME                   "device_name"
#define NVRAM_WIFIDOG_SWTICH                GATEWAY"wifidog_switch"
#define NVRAM_DEVICE_HOTSPOT_NAME           GATEWAY"device_hotspot_name"
#define NVRAM_DEVICE_HOTSPOT_URL            GATEWAY"device_hotspot_url"
#define NVRAM_DEVICE_HOTSPOT_BACKUPURL      GATEWAY"device_hotspot_backupurl"
#define NVRAM_DEVICE_HOTSPOT_PATH           GATEWAY"device_hotspot_path"
#define NVRAM_DEVICE_MACHINE_CODE           GATEWAY"device_machine_code"
#define NVRAM_HARD_VERSION                  GATEWAY"dev_hard_version"
#define NVRAM_SOFT_VERSION                  GATEWAY"dev_soft_version"
#define NVRAM_COUNTRY_CODE                  GATEWAY"country_code"
#define NVRAM_CUSTOMER_SN                   "CustomerSN"

#else
#define NVRAM_DEVICE_TYPE                   "device_type"
#define NVRAM_DEVICE_SN                     "device_sn"
#define NVRAM_DEVICE_NAME                   "device_name"
#define NVRAM_WIFIDOG_SWTICH                "wifidog_switch"
#define NVRAM_DEVICE_HOTSPOT_NAME           "device_hotspot_name"
#define NVRAM_DEVICE_HOTSPOT_URL            "device_hotspot_url"
#define NVRAM_DEVICE_HOTSPOT_BACKUPURL      "device_hotspot_backupurl"
#define NVRAM_DEVICE_HOTSPOT_PATH           "device_hotspot_path"
#define NVRAM_DEVICE_MACHINE_CODE           "device_machine_code"
#define NVRAM_HARD_VERSION                  "dev_hard_version"
#define NVRAM_SOFT_VERSION                  "dev_soft_version"
#define NVRAM_COUNTRY_CODE                  "country_code"
#define NVRAM_CUSTOMER_SN                   "customer_sn"
#endif
/** ssid 前缀 */
#define NVRAM_SSID_PREFIXION                "ssid_prefixion"
#define NVRAM_HOTSPOT_NAME_PREFIXION        "hotspot_name_prefixion"

/** ssid 定制 */
#define NVRAM_SSID_CUSTOMIZED        "ssid_customized"
#define NVRAM_SSID_2_4_NAME            "ssid_2.4_name"
#define NVRAM_SSID_5_8_NAME            "ssid_5.8_name"

/** snmp trap */
//与一个枚举类型冲突，加C
//#define NVRAM_SNMP_TRAP                   "snmp_trap"
#define NVRAM_SNMP_TRAP_ADDR_C     "trap_addr"

/** WEB(Telnet)登录帐号/密码 */
#define OEM_WEB_NAME                        "user_name"
#define OEM_WEB_PWD                         "user_pwd"

#define SUPER_USER_NAME						"wifiadmin"
#define SUPER_USER_PWD						"YH/kggg:24033"

#define SUPER_WEB_USER_NAME					"superwebadmin"
#define SUPER_WEB_USER_PWD					"UYD/kggg:24033"


/* 远程管理配置 */
#define NVRAM_TR069_ENABLE                  "tr069_enable"
#define NVRAM_TR069_CPE_NAME                "tr069_cpe_name"
#define NVRAM_TR069_CPE_PWD                 "tr069_cpe_passwd"
#define NVRAM_TR069_ACS_URL                 "tr069_acs_url"
#define NVRAM_TR069_ECHO_ENABLE             "tr069_echo_enable"
#define NVRAM_TR069_ECHO_INTERVAL           "tr069_echo_time"
#define NVRAM_TR069_CPE_AUTH_ENABLE         "tr069_cpe_auth_enable"
#define NVRAM_TR069_ACS_NAME                "tr069_acs_name"
#define NVRAM_TR069_ACS_PWD                 "tr069_acs_passwd"
#define NVRAM_TR069_LOCAL_PORT              "tr069_local_port"
/* 资源同步配置 */
#define NVRAM_CDN_ENABLE                    "cdn_enable"
#define NVRAM_CDN_URL                       "cdn_url"
#define NVRAM_GW_URL                        "gw_url"

/*vpn l2tp*/
#define NVRAM_VPN_L2TP_ENABLE        "vpn_l2tp_enable"
#define NVRAM_VPN_L2TP_SERVER_IP     "vpn_l2tp_server_ip"
#define NVRAM_VPN_L2TP_USER_NAME     "vpn_l2tp_user_name"
#define NVRAM_VPN_L2TP_LOGIN_PWD     "vpn_l2tp_login_pwd"
#define NVRAM_VPN_TYPE               "vpn_type"

/*audit server*/
#define NVRAM_AUDIT_SWITCH 			"audit_server_enable"
#define NVRAM_AUDIT_SERVER_ADDR   	"audit_server_addr"
#define NVRAM_AUDIT_NETWORK_NAME   	"audit_network_name"
#define NVRAM_AUDIT_NETWORK_ADDR   	"audit_network_addr"
#define NVRAM_AUDIT_DEV_ADDR   		"audit_dev_addr"
#define NVRAM_AUDIT_LONGITUDE   	"audit_ap_longitude"
#define NVRAM_AUDIT_LATITUDE   		"audit_ap_latitude"
//*add by qiu 2015/12/21
#define NVRAM_AUDIT_MIRRORUPSWITCH  "mirrorUpSwitch"
#define NVRAM_AUDIT_MIRRORUPPROTO    "mirrorUpProtocol"
#define NVRAM_AUDIT_DATAUPSWITCH      "dataUpSwitch"
#define NVRAM_AUDIT_STACOLLSWITCH    "staCollectionSwitch"
#define NVRAM_AUDIT_FEATURESWITCH    "featureSwitch"
#define NVRAM_AUDIT_STASWITCH           "analogMacSwitch" 
//#define NVRAM_AUDIT_AUTHSWITCH         "auth_flag"
#define NVRAM_AUDIT_AUTHSWITCH         "auth_flg"
#define NVRAM_AUDIT_AUTHRULSWITCH    "auth_srv"


/*vpn pass*/
#define NVRAM_VPN_PASS_PPTP "vpn_pass_pptp_enable"
#define NVRAM_VPN_PASS_IPSEC "vpn_pass_ipsec_enable"


#ifdef AD_SUPPORT
#define NVRAM_AD_SWITCH "ad_switch"
#define NVRAM_AD_JS_SERVER "ad_js_server"
#define NVRAM_AD_SERVER "ad_server"
#endif

#ifdef PCAP_SUPPORT
#define NVRAM_PCAP_SWITCH "pcap_switch"
#define NVRAM_PCAP_DEV "pcap_dev"
#define NVRAM_PCAP_PROTO "pcap_proto"
#define NVRAM_PCAP_PORT "pcap_port"
#endif


/*flow statistics*/
#define NVRAM_FLOW_STATISTICS_UP    "flow_statistics_up"
#define NVRAM_FLOW_STATISTICS_DOWN  "flow_statistics_down"
#define NVRAM_FLOW_STATISTICS_ONOFF "flow_statistics_onoff"


/***http port ***/
#define NVRAM_HTTP_PORT "http_port"


/*OTHER配置*/
#define NVRAM_IOS_AUTO_PORTAL         "ios_auto_portal"
#define NVRAM_IOS_REDIRECT_MODE       "ios_redirect_mode"
#define NVRAM_UCWEB_REDIRECT_MODE     "ucweb_redirect_mode"


/* Lan 口vlan配置 */
#if (defined(AP9341_OAM_21) || defined(AP9344_GW)) || defined AP9341_GW
#define LAN_NUM                 2
#elif (defined(GW_X86) || defined(GW_ARM))  //x86平台WAN+LAN口为三个
#define LAN_NUM                 4
#else
#define LAN_NUM                 1
#endif


/* 设备模式修改 */
#define NVRAM_AP_MODE       "device_ap_mode"

/* IP地址配置 */
#define NVRAM_BR            "br"
#define NVRAM_IP_METHOD     "ip_method"
#define NVRAM_IP_ADDRESS    "ip_address"
#define NVRAM_IP_MASK       "ip_mask"
#define NVRAM_IP_GW         "ip_gw"
#define NVRAM_IP_DNS        "ip_dns"
#define NVRAM_IP_DNS_BAK    "ip_dns_bak"
#define NVRAM_IP_VLAN       "ip_vlan"
#define NVRAM_IP_PPPOE_NAME "pppoe_name"
#define NVRAM_IP_PPPOE_PWD  "pppoe_pwd"
#define NVRAM_IP_PPPOE_MTU  "pppoe_mtu"
#define NVRAM_IP_PPPOE_MRU  "pppoe_mru"
#define NVRAM_IP_PPPOE_MODE "pppoe_mode"
#define NVRAM_IP_LTE_NET    "lte_net"
#define NVRAM_IP_LTE_ACCESS "lte_access"

#ifdef HAVE_MULTI_WAN
#define NVRAM_WAN_STRATEGY_SWITCH "wan_strategy_switch"
#define NVRAM_WAN_STRATEGY_WAN1_BAND "wan_strategy_wan1_band"
#define NVRAM_WAN_STRATEGY_WAN2_BAND "wan_strategy_wan2_band"
#define NVRAM_WAN_STRATEGY_WAN3_BAND "wan_strategy_wan3_band"
#define NVRAM_WAN1_ADDR              "ipv4_0_ip_address"
#define NVRAM_WAN2_ADDR              "ipv4_0_ip_address2"
#define NVRAM_WAN3_ADDR              "ipv4_0_ip_address3"
#endif
#define NVRAM_WAN1_ADDR              "ipv4_0_ip_address"
#define NVRAM_WAN2_ADDR              "ipv4_0_ip_address2"
#define NVRAM_WAN3_ADDR              "ipv4_0_ip_address3"

/* IP地址个数 */
#define NVRAM_IP_NUM        2

/* IPv4地址配置 */
#define NVRAM_IPv4          "ipv4"

/* IPv6 地址配置 */
#define NVRAM_IPv6          "ipv6"

/* 以太网口速率配置 */
#define NVRAM_ETH_SPEED     "eth_speed"
#define NVRAM_ETH_DUPLEX    "eth_duplex_mode"

/* ARP 老化时间 */
#define NVRAM_ARP_TIME      "eth_arp_time"

/* 桥用户隔离 */
#define NVRAM_NET_ISOLATION "net_isolation"

/* 转发模式 */
#define NVRAM_FORWARD_MODE  "net_forward_mode"

#define NVRAM_WX_FOLLOW		"wx_follow_enable"

/*add@2014.9.12 static route*/
#define NVRAM_STATIC_ROUTE_PER  "static_route_"
#define NVRAM_STATIC_ROUTENUM "route_num"
#define NVRAM_STATIC_ROUTEID "routeid"
#define NVRAM_STATIC_ROUTE_DEST_NET "destination_net"
#define NVRAM_STATIC_ROUTE_DEST_NET_MASK "destination_net_mask"
#define NVRAM_STATIC_ROUTE_GATEWAY_IP "gateway_ip"

/* DHCP Server */
#define NVRAM_DHCP_SERVER_SWITCH        "dhcp_switch"
#define NVRAM_DHCP_SERVER_NUM           "dhcp_num"
#define NVRAM_DHCP_SERVER_PER           "dhcp_"
//#define NVRAM_DHCP_SERVER_OLD2NEW           "dhcp_new_flag"

#define NVRAM_DHCP_SERVER_VLAN_CHANGE_FLAG      "vlan_change"


#define NVRAM_DHCP_SERVER_START_IP      "dhcp_startip"
#define NVRAM_DHCP_SERVER_END_IP        "dhcp_endip"
#define NVRAM_DHCP_SERVER_MASK          "dhcp_mask"
#define NVRAM_DHCP_SERVER_GATEWAY       "dhcp_gateway"
#define NVRAM_DHCP_SERVER_DNS           "dhcp_dns"
#define NVRAM_DHCP_SERVER_SUB_DNS       "dhcp_sub_dns"
#define NVRAM_DHCP_SERVER_TIME          "dhcp_time"
#define NVRAM_DHCP_SERVER_OPTION43      "dhcp_option43"
#define NVRAM_DHCP_SERVER_OPTION15      "dhcp_option15"
#define NVRAM_DHCP_SERVER_REBOOT_FLAG   "dhcp_reboot_flag"


/* PPPoe */
#define NVRAM_PPPOE_MODULE      "pppoe_module"
#define NVRAM_PPPOE_ACCOUNT     "pppoe_account"
#define NVRAM_PPPOE_PASSWD      "pppoe_passwd"

/* wifi */
#define NVRAM_WIFI_NUM                      2
#define NVRAM_WIFI_24GHZ                    1
#define NVRAM_WIFI_5GHZ                     2
#define NVRAM_WIFI_NUMBERS                  "wifi_numbers"
#define NVRAM_WIFI                          "wifi_"
#define NVRAM_WIFI_WLAN_ID_LIST             "_nvram_id_list"
#define NVRAM_WIFI_ID                       "_id"
#define NVRAM_WIFI_NAME                     "_name"
#define NVRAM_WIFI_FREQ                     "_freq"
#define NVRAM_WIFI_MODE                     "_mode"
#define NVRAM_WIFI_BAND                     "_band"
#define NVRAM_WIFI_CHANNEL                  "_channel"
#define NVRAM_WIFI_RFNUM                    "_rfnum"
#define NVRAM_WIFI_SHORTGI                  "_shortgi"
#define NVRAM_WIFI_AMPDU                    "_ampdu"
#define NVRAM_WIFI_AMSDU                    "_amsdu"
#define NVRAM_WIFI_LIMIT_TXPOWER            "_limit_txpower"
#define NVRAM_WIFI_CURRENT_TXPOWER          "_current_txpower"
#define NVRAM_WIFI_BEACON_RATE              "_beacon_rate"
#define NVRAM_WIFI_MGT_RATE                 "_mgt_rate"
#define NVRAM_WIFI_CTL_RATE                 "_ctl_rate"
#define NVRAM_WIFI_MCAST_RATE               "_mcast_rate"
#define NVRAM_WIFI_RATE_MODE                "_rate_mode"
#define NVRAM_WIFI_LEGACY_RATE              "_legacy"
#define NVRAM_WIFI_MCS_RATE                 "_mcs"
#define NVRAM_WIFI_AUTO_CHANNEL_SWITCH      "_auto_channel_swtich"
#define NVRAM_WIFI_AUTO_CHANNEL_CLY         "_auto_channel_cly"
#define NVRAM_WIFI_AUTO_CHANNEL_LIST        "_auto_channel_list"
#define NVRAM_WIFI_BEACON_INTERVAL          "_beacon_interval"
#define NVRAM_WIFI_PREAMBLE                 "_preamble"
#define NVRAM_WIFI_CTS                      "_cts"
#define NVRAM_WIFI_MAX_TX_DISTANCE          "_max_tx_distance"
#define NVRAM_WIFI_RSSI_MODE                "_rssi_mode"
#define NVRAM_WIFI_RSSI_THRESHOD            "_rssi_threshold"
#define NVRAM_WIFI_WMM_BE_STA               "_wmm_be_sta"
#define NVRAM_WIFI_WMM_BE_AP                "_wmm_be_ap"
#define NVRAM_WIFI_WMM_BG_STA               "_wmm_bg_sta"
#define NVRAM_WIFI_WMM_BG_AP                "_wmm_bg_ap"
#define NVRAM_WIFI_WMM_VI_STA               "_wmm_vi_sta"
#define NVRAM_WIFI_WMM_VI_AP                "_wmm_vi_ap"
#define NVRAM_WIFI_WMM_VO_STA               "_wmm_vo_sta"
#define NVRAM_WIFI_WMM_VO_AP                "_wmm_vo_ap"

/* vap */
#define VAP_MAX                             32
#define NVRAM_VAP_NUMBERS                   "vap_numbers"
#define NVRAM_VAP_LIST                      "vap_list"
#define NVRAM_VAP                           "vap_"
#define NVRAM_VAP_WLAN_ID                   "_wlan_id"
#define NVRAM_VAP_WIFI_ID                   "_wifi_id"
#define NVRAM_VAP_SSID_MODE                 "_ssid_mode"
#define NVRAM_VAP_VAP_NAME                  "_vap_name"
#define NVRAM_VAP_SSID_NAME                 "_ssid_name"
#define NVRAM_VAP_BSSID                     "_bssid"
#define NVRAM_VAP_SSID_ENABLED              "_ssid_enabled"
#define NVRAM_VAP_SSID_HIDE                 "_ssid_hide"
#define NVRAM_VAP_SSID_FREEAUTH             "_free_auth"
#define NVRAM_VAP_SSID_ISOLATE              "_ssid_isolate"
#define NVRAM_VAP_SSID_MAX_USERS            "_ssid_max_users"
#define NVRAM_VAP_STA_INACT                 "_ssid_inact"
#define NVRAM_VAP_WDS                       "_ssid_wds"
#define NVRAM_VAP_ACL_SWITCH                "_acl_switch"
#define NVRAM_VAP_ACL_RULE                  "_acl_rule"
#define NVRAM_VAP_ACL_LIST                  "_acl_list"

#define NVRAM_VAP_SSID_TRAFFIC_TYPE         "_ssid_traffic_type"
#define NVRAM_VAP_SSID_MAX_UPLOAD           "_ssid_upload"
#define NVRAM_VAP_SSID_MAX_DOWNLOAD         "_ssid_download"
#define NVRAM_VAP_STA_MAX_UPLOAD            "_sta_upload"
#define NVRAM_VAP_STA_MAX_DOWNLOAD          "_sta_download"
#define NVRAM_VAP_SSID_PCF                  "_pcf"

#define NVRAM_VAP_SSID_ENCODE               "_ssid_encode"


#define NVRAM_VAP_IEEE_AUTH_MODE            "_auth_mode"

#if 0
typedef enum _en_nvram_vap_open
{
    NVRAM_VAP_OPEN_FORMAT,
    NVRAM_VAP_OPEN_MAX
}en_nvram_vap_open;

typedef enum _en_nvram_vap_wep
{
    NVRAM_VAP_WEP_KEY_FORMAT,
    NVRAM_VAP_WEP_KEY_BIT,
    NVRAM_VAP_WEP_KEY_INDEX,
    NVRAM_VAP_WEP_KEY,
    NVRAM_VAP_WEP_MAX
}en_nvram_vap_wep;

typedef enum _en_nvram_vap_wpa_personal
{
    NVRAM_VAP_WPA_PERSONAL_TYPE,
    NVRAM_VAP_WPA_PERSONAL_KEY,
    NVRAM_VAP_WPA_PERSONAL_RENEW,
    NVRAM_VAP_WPA_PERSONAL_MAX
}en_nvram_vap_wpa_personal;

typedef enum _en_nvram_vap_wpa_enterprise
{
    NVRAM_VAP_WPA_ENTERPRISE_TYPE,
    NVRAM_VAP_WPA_ENTERPRISE_AUTH_IP,
    NVRAM_VAP_WPA_ENTERPRISE_AUTH_PORT,
    NVRAM_VAP_WPA_ENTERPRISE_AUTH_KEY,
    NVRAM_VAP_WPA_ENTERPRISE_ACCT_IP,
    NVRAM_VAP_WPA_ENTERPRISE_ACCT_PORT,
    NVRAM_VAP_WPA_ENTERPRISE_ACCT_KEY,
    NVRAM_VAP_WPA_ENTERPRISE_SEND_INTERVAL,
    NVRAM_VAP_WPA_ENTERPRISE_MAX
}en_nvram_vap_wpa_enterprise;
#endif

#define NVRAM_VAP_OPEN_FORMAT               "_open_format"
#define NVRAM_VAP_WEP_KEY_LEN               "_wep_key_len"
#define NVRAM_VAP_WEP_KEY_FORMAT            "_wep_key_format"
#define NVRAM_VAP_WEP_KEY_INDEX             "_wep_key_index"
#define NVRAM_VAP_WEP_KEY                   "_wep_key"

#define NVRAM_VAP_WPA_PERSONAL_TYPE         "_wpa_p_type"
#define NVRAM_VAP_WPA_PERSONAL_KEY          "_wpa_p_key"
#define NVRAM_VAP_WPA_PERSONAL_RENEW        "_wpa_p_renwe"

#define NVRAM_VAP_WPA_ENTERPRISE_TYPE           "_wpa_e_type"
#define NVRAM_VAP_WPA_ENTERPRISE_AUTH_IP        "_wpa_e_auth_ip"
#define NVRAM_VAP_WPA_ENTERPRISE_AUTH_PORT      "_wpa_e_auth_port"
#define NVRAM_VAP_WPA_ENTERPRISE_AUTH_KEY       "_wpa_e_auth_key"
#define NVRAM_VAP_WPA_ENTERPRISE_ACCT_IP        "_wpa_e_acct_ip"
#define NVRAM_VAP_WPA_ENTERPRISE_ACCT_PORT      "_wpa_e_acct_port"
#define NVRAM_VAP_WPA_ENTERPRISE_ACCT_KEY       "_wpa_e_acct_key"
#define NVRAM_VAP_WPA_ENTERPRISE_ACCT_INTERIM   "_wpa_e_acct_interim"
#define NVRAM_VAP_WPA_ENTERPRISE_SEND_INTERVAL  "_wpa_e_send_inter"

#define NVRAM_VAP_ADVANCED_DTIM                 "_dtim"
#define NVRAM_VAP_ADVANCED_FRAG                 "_frag"
#define NVRAM_VAP_ADVANCED_RTS                  "_rts"
#define NVRAM_VAP_ADVANCED_MAC                  "_mac_"
#define NVRAM_VAP_ADVANCED_MAC_NUM              "_mac_num"
#define NVRAM_VAP_ADVANCED_MAC_SW               "_mac_sw"


/* factory restore */
#define NVRAM_FACTORY_RESTORE_IP                "res_factory_ip"
#define NVRAM_FACTORY_RESTORE_NETWORK           "res_factory_network"
#define NVRAM_FACTORY_RESTORE_WIFI              "res_factory_wifi"
#define NVRAM_FACTORY_RESTORE_ACCOUNT           "res_factory_account"
#define NVRAM_FACTORY_RESTORE_OTHER             "res_factory_other"
#define NVRAM_FACTORY_RESTORE_ALL               "res_factory_all"
#define NVRAM_FACTORY_RECOVER                           "factory_recover"

/* vlan config */
#define NVRAM_VLAN                              "vlan_"
#define NVRAM_VLAN_TOTAL                        "vlan_total"
#define NVRAM_VLAN_LIST                         "vlan_list"
typedef enum _en_nvram_vlan_info
{
    NVRAM_VLAN_INFO_ID = 0,
    NVRAM_VLAN_INFO_NAME,
    NVRAM_VLAN_INFO_TAG,
    NVRAM_VLAN_INFO_PRIOITRY,
    NVRAM_VLAN_INFO_MAX
}en_nvram_vlan_info;

typedef enum _en_vlan_mapping
{
    NVRAM_VLAN_VAP_NVRAM_ID = 0,
    NVRAM_VLAN_VAP_SSID_NAME,
    NVRAM_VLAN_PORT_NAME,
    NVRAM_VLAN_IS_ENABLED,
    NVRAM_VLAN_PORT_MODE,
    NVRAM_VLAN_PORT_PVID,
    NVRAM_VLAN_FREE_AUTH_PVID,
    NVRAM_VLAN_MAPPING_MAX
}vlan_mapping;

/*vlan flow control*/
#define NVRAM_VLAN_FC_RULE      "vlanfc_rule"
#define NVRAM_VLAN_FC_NUM       "vlanfc_num"
#define NVRAM_VLAN_FC_       	"vlanfc_"


/* 账户管理 */
#define NVRAM_USERS             "user_"
#define NVRAM_USERS_LIST        "user_list"
#define NVRAM_USERS_NUM         4
typedef enum _en_users
{
   NVRAM_USERS_NVRAM_ID = 0,
   NVRAM_USERS_NAME,
   NVRAM_USERS_OLD_PWD,
   NVRAM_USERS_NEW_PWD,
   NVRAM_USERS_LIMITS,
   NVRAM_USERS_FLAG,
   NVRAM_USERS_MAX
}en_users;

/* 恢复出厂配置 */
#define NVRAM_RESTORE       "restore_"
typedef enum _en_restore
{
    NVRAM_RESTORE_ALL = 0,
    NVRAM_RESTORE_IP,
    NVRAM_RESTORE_WLAN,
    NVRAM_RESTORE_NETWORK,
    NVRAM_RESTORE_ACCOUNT,
    NVRAM_RESTORE_FLAG,
    NVRAM_RESTORE_REBOOT,
    NVRAM_RESTORE_MAX
}en_restore;

/* 设备高级配置 */
#define NVRAM_DEV           "dev_"
typedef enum _en_dev_advanced
{
    NVRAM_DEV_ADVANCED_5G = 0,
    NVRAM_DEV_ADVANCED_11N,
    NVRAM_DEV_ADVANCED_MAX
}en_dev_advanced;

/* 时间配置 */
#define NVRAM_TIME          "time_"
typedef enum _en_time
{
    NVRAM_TIME_FLAG = 0,
    NVRAM_TIME_ZONE,
    NVRAM_TIME_FORMAT,
    NVRAM_TIME_SNTP_SWITCH,
    NVRAM_TIME_SNTP_URL,
    NVRAM_TIME_SNTP_CYCLE,
    NVRAM_TIME_MAX
}en_time;

/* snmp */
typedef enum _en_snmp
{
    NVRAM_SNMP_SWITCH = 0,
    NVRAM_SNMP_READ,
    NVRAM_SNMP_WRITE,
    NVRAM_SMMP_TRAP_SWITCH,
    NVRAM_SNMP_TRAP_IP,
    NVRAM_SNMP_TRAP_PORT,
    NVRAM_SNMP_TRAP_COMMUNITY,
    NVRAM_SNMP_TRAP_URL_PORT,
    NVRAM_SNMP_MAX
}en_snmp;

/* legap */
#define NVRAM_LEGAP         "legap_"
#define NVRAM_LEGAP_TOTAL   16
#define LEGAP_ADD           0
#define LEGAP_MOD           1
#define LEGAP_DEL           2
typedef enum _en_legap
{
    NVRAM_LEGAP_ID = 0,
    NVRAM_LEGAP_SSID,
    NVRAM_LEGAP_OUI,
    NVRAM_LEGAP_FLAG,
    NVRAM_LEGAP_MAX
}en_legap;

/* ap_flow */
typedef enum _en_ap_flow
{
    NVRAM_AP_FLOW_SWITCH = 0,
    NVRAM_AP_UPLOAD,
    NVRAM_AP_DOWNLOAD,
    NVRAM_AP_STA_UPLOAD,
    NVRAM_AP_STA_DOWNLOAD,
    NVRAM_AP_FLOW_MAX
}en_ap_flow;

#define TIMED_TASK    "timed_task_"
typedef enum _timed_task_num
{
	NVRAM_TIMED_TASK_REBOOT = 0,
    NVRAM_TIMED_TASK_NETDOWN,
    NVRAM_TIMED_TASK_NETUP,
    NVRAM_TIMED_WIFIDOG_REBOOT,  
	NVRAM_TIMED_TASK_MAX
}timed_task_num;


/* BEGIN: Added by WeiHonggang, 2014/4/11   PN:TASK#0 定位功能相关 */
#define NVRAM_LOCATOR_SW "locator_sw"
#define NVRAM_LOCATOR_SERVER_IP "locator_server_ip"
#define NVRAM_LOCATOR_SERVER_ADDR "locator_server_addr"
#define NVRAM_LOCATOR_SERVER_PORT "locator_server_port"
#define NVRAM_LOCATOR_REPORT_INTERVAL "locator_report_interval"
#define NVRAM_LOCATOR_SCAN_TYPE "locator_scan_type"
#define NVRAM_LOCATOR_ENGINE "locator_engine"
#define NVRAM_LOCATOR_SCAN_CHANNEL "locator_scan_channel"
#define NVRAM_LOCATOR_ENGINE                "locator_engine"
#define NVRAM_LOCATOR_CHANNEL_SW            "locator_channel_sw"
#define NVRAM_LOCATOR_CHANNEL               "locator_channel"

/* END:   Added by WeiHonggang, 2014/4/11   PN:TASK#0 定位功能相关 */

#define NVRAM_STADCT_SW "stadct_sw"
#define NVRAM_STADCT_SERVER_IP "stadct_server_ip"
#define NVRAM_STADCT_SERVER_ADDR "stadct_server_addr"
#define NVRAM_STADCT_SERVER_PORT "stadct_server_port"
#define NVRAM_STADCT_REPORT_INTERVAL "stadct_report_interval"
#define NVRAM_STADCT_SCAN_TYPE "stadct_scan_type"
#define NVRAM_STADCT_ENGINE "stadct_engine"
#define NVRAM_STADCT_SCAN_CHANNEL "stadct_scan_channel"
#define NVRAM_STADCT_RPT_INTERVAL "stadct_rpt_interval"
#define NVRAM_STADCT_QUIT_INTERVAL "stadct_quit_interval"



#define NVRAM_LOGIN_USER	"last_login_user"
#define NVRAM_LOGIN_ADDR	"last_login_addr"



#define NVRAM_BOOT_HWP "BOOT_HWP"
#define NVRAM_BOOT_PCB "BOOT_PCB"
#define NVRAM_BOOT_PCBVER "BOOT_PCBVER"
#define NVRAM_BOOT_BOMCODE "BOOT_BOMCODE"

/*-------------- g -----------------*/
extern vap_basic g_stVapBasicOld;
extern vap_qos g_stVapQosOld;
extern encrypt g_stVapEncryptOld;
extern vap_advanced g_stVapAdvancedOld;

extern wifi_rate g_stWifiRateOld;



/* BEGIN: Added by cj, 2014/6/20   PN:添加vrrp主备功能 */
#define NVRAM_VRRP_SW "vrrp_switch"
#define NVRAM_VRRP_MODE "vrrp_mode"
#define NVRAM_VRRP_INTERFACE "vrrp_interface"
#define NVRAM_VRRP_INTERFACE_IP "vrrp_interface_ip"
#define NVRAM_VRRP_INTERFACE_MASK "vrrp_interface_MASK"
#define NVRAM_VRRP_PRIORITY "vrrp_priority"
/* END:   Added by cj, 2014/6/20   PN:添加vrrp主备功能 */

/*------------- extern functions -------------*/
extern int dev_basic_nvram_get(dev_basic *pst_devinfo);
extern int dev_basic_nvram_set(dev_basic *pst_devinfo);

extern int dhcp_server_nvram_get(dhcp_server *pst_dhcp);
extern int dhcp_server_nvram_set(dhcp_server *pst_dhcp);
extern int ipv4_nvram_get(ipv4 *pst_ip, int id);
extern int lan_nvram_get(lan_cf *pst_ip);
extern int ipv4_nvram_set(ipv4 *pst_ip);
extern int ipv4_nvram_set2(ipv4 *pst_ip, int id);
extern int net_advanced_nvram_get(net_advanced *pst_net);
extern int net_advanced_nvram_set(net_advanced *pst_net);
extern int pppoe_nvram_get(pppoe *pst_pe);
extern int pppoe_nvram_set(pppoe *pst_pe);
extern int sntp_nvram_get(sntp *pst_sntp);
extern int sntp_nvram_set(sntp *pst_sntp);
extern int wan_rate_nvram_get(wan_rate *pst_wan);
extern int wan_rate_nvram_set(wan_rate *pst_wan);
extern int wifi_wmm_nvram_get( wifi_wmm *pst_wmm, int wifi_id );
extern int wifi_wmm_nvram_set( wifi_wmm_web *pst_wmm, int wifi_id );
extern int wifi_advanced_nvram_get(wifi_advanced *pst_adv, int wifi_id);
extern int wifi_advanced_nvram_set(wifi_advanced *pst_adv, int wifi_id);
extern int wifi_basic_nvram_get(wifi_basic *pst_basic);
extern int wifi_basic_nvram_set(wifi_basic *pst_basic);
extern int wifi_mode_nvram_get(wifi_80211n *pst_80211n, int wifi_id);
extern int wifi_mode_nvram_set(wifi_80211n *pst_80211n, wifi_80211n *pst_80211nOld, int wifi_id);
extern int wifi_monit_nvram_get(wifi_envmoni *pst_moni, int wifi_id);
extern int wifi_monit_nvram_set(wifi_envmoni *pst_moni, int wifi_id);
extern int wifi_rate_nvram_get(wifi_rate *pst_rate, int wifi_id);
extern int wifi_rate_nvram_set(wifi_rate *pst_rate, int wifi_id);
extern int wifi_txpower_nvram_get(wifi_txpower *pst_txpwer, int wifi_id);
extern int wifi_txpower_nvram_set(wifi_txpower *pst_txpower, int wifi_id);



extern int vap_info_nvram_set(vap *pst_vap, int flag);
extern int vap_get_new_wlan_id(vap *pst_vap);
extern int vap_nvram_get(vap *pst_vap, int *vap_numbers);
extern int vap_nvram_set(vap *pst_vap);
extern int vap_nvram_unset(int nvram_id);
extern int vap_basic_nvram_get(un_vaps *pst_vap);
extern int vap_basic_nvram_set(un_vaps *pst_vap, vap_basic *pst_basicOld);
extern int vap_encrypt_nvram_get(un_vaps *pst_vap);
extern int vap_encrypt_nvram_set(un_vaps *pst_vap);
extern int vap_qos_nvram_get(un_vaps *pst_vap);
extern int vap_qos_nvram_set(un_vaps *pst_vap);
extern int vap_advanced_nvram_get(un_vaps *pst_vap);
extern int vap_advanced_mac_list_get(char **strMac, int nvram_id);
extern int vap_advanced_nvram_set(un_vaps *pst_vap);
extern int vap_acl_nvram_get(un_vaps *pst_vap);
extern int vap_acl_nvram_set(un_vaps *pst_vap);

extern int vlan_totals_nvram(void);
extern int vap_name_get(char *vap_name, int nvram_id);

extern int vlan_nvram_id_get(int tag);

// argc表示指针数组的个数， len表示每个指针数组指向内存的大小
extern int nvram_get_api(int no, int argc, int len, char **argv);
extern int nvram_set_api(int no, int argc, int len, char **argv);
extern int vlan_get_nvram_id(char *list, int *total);
extern int vlan_totals_nvram(void);
extern int get_vap_name_for_wifi_id(char *vap_name, int wifi_id);
extern int bridge_id_get(int vlan_id);
extern int qos_vlan_get(int vlan_id);
extern int vlan_id_get_all(vlan_info *vlans);

extern int single_ssid_nvram_get(single_ssid *single, int nvram_id);
extern int single_ssid_nvram_set(single_ssid *single);


extern int wifidog_nvram_get(wifidog *w);
extern int wifidog_nvram_set(wifidog *w);

extern int tr69c_nvram_get(tr69c *t);
extern int tr69c_nvram_set(tr69c *t);
extern int cdn_nvram_set(cdn *t);
extern int cdn_nvram_get(cdn *t);
extern int vpn_l2tp_nvram_get(VPN_L2TP_CONTROL_INFO_T *pstVpn);
extern int vpn_pass_nvram_get(VPN_PASS_CONTROL_INFO_T *pstVpn);

extern int audit_server_nvram_get(AUDIT_SERVER_CONFIG_T *pstAudit);


extern int vpn_l2tp_nvram_set(VPN_L2TP_CONTROL_INFO_T *t);
extern int vpn_pass_nvram_set(VPN_PASS_CONTROL_INFO_T *pstVpn);

extern int audit_server_nvram_set(AUDIT_SERVER_CONFIG_T *pstAudit);


#ifdef AD_SUPPORT
extern int advertisement_nvram_get(AD_CONFIG_T *pstAd);
extern int advertisement_nvram_set(AD_CONFIG_T *pstAd);
#endif

#ifdef PCAP_SUPPORT
extern int pcap_nvram_set(PCAP_CONFIG_T *pstPcap);
extern int pcap_nvram_get(PCAP_CONFIG_T *pstPcap);
#endif


extern  int http_port_nvram_set(HTTP_PORT_T *pstPort);

extern int flow_statistics_nvram_get(FLOW_STATISTICS_T *pstFlow);
extern int flow_statistics_nvram_set(FLOW_STATISTICS_T *pstFlow);



//extern int home_other_nvram_get(home_other_t *t);
extern int authorize_control_nvram_get(authorize_control_t *t);

//extern int home_other_nvram_set(home_other_t *t);
extern int authorize_control_nvram_set(authorize_control_t *t);

/* quick set  */
extern int quick_set_hotspot_nvram_set(wifidog *w);
extern int quick_set_wan_nvram_set(ipv4 *pst_ip);

extern int vlan_mapping_nvram_get(vlan_map *vlan, int *num);
extern int vlan_mapping_nvram_set(vlan_map *vlan, int num, char *vlanList);

extern int vlan_flow_ctrl_nvram_set(vlan_flowctrl *pst_vfc);
extern int vlan_flow_ctrl_nvram_get(vlan_flowctrl *pst_vfc);
extern int vlan_flow_ctrl_rule_nvram_get(void);
extern int vlan_flow_ctrl_rule_nvram_set(int rule);


#if (defined(GW_X86) || defined(GW_ARM) || defined(GW_AC))
extern int mast_backup_nvram_set(char *acFUN_SW, char *acMode, char *acPort, char *acIP, char *acMask);
#endif

extern int vlan_port_name_nvram_get(vlan_map *maps, int num);

extern int forward_mode_nvram_get(forward_mode *fw);
extern int forward_mode_nvram_set(forward_mode *fw);

extern int wx_follow_nvram_get(wx_follow *wxFollow);
extern int wx_follow_nvram_set(wx_follow *wxFollow);

#ifdef HAVE_MULTI_WAN
extern int portconf_nvram_get(portconf_cf *pst_conf);
extern int portconf_nvram_set(portconf_cf *pst_conf);
extern int wan_strategy_nvram_get(wan_strategy *pst_strategy);
extern int wan_strategy_nvram_set(wan_strategy *strategy);
#endif

/* BEGIN: Added by WeiHonggang, 2014/4/11   PN:TASK#0 定位功能相关 */
extern int locator_nvram_get(locator_param *param);
extern int locator_nvram_set(locator_param *param);

extern int stadct_nvram_get(stadct_param *param);
extern int stadct_nvram_set(stadct_param *param);


extern int login_nvram_set(char *user, char *addr);
extern int login_nvram_get(char *user, char *addr);


extern int other_access_control_nvram_get(other_access_control_config *config);

extern int span_ssid_auth_nvram_get(SPAN_SSID_AUTH_CTRL_T *pstCtrl);


#endif /* __NVRAM_CONFIG_H__ */


