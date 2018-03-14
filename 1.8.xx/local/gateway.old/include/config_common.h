/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : config_common.h
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/5/30
  Last Modified :
  Description   : 功能配置接口
  Function List :
  History       :
  1.Date        : 2013/5/30
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#ifndef __CONFIG_COMMON__
#define __CONFIG_COMMON__

/* nvram 操作函数的定义(opera_no) */
typedef enum _en_opera_no
{
    /* vlan 相关操作 */
    VLAN_GET        = 30,
    VLAN_ADD        = 31,
    VLAN_DELETE     = 32,
    VLAN_MODI       = 33,
    VLAN_MAPPING    = 34,

    /* 账户管理相关操作 */
    USERS_ADD       = 40,
    USERS_MODI      = 41,
    USERS_DELETE    = 42,
    USERS_GET       = 43,

    /* 恢复出厂配置相关操作 */
    RESTORE_SET     = 50,
    RESTORE_GET     = 51,

    /* 时间配置 */
    TIME_SET        = 55,
    TIME_GET        = 56,

    /*设备配置*/
    DEV_ADVANCED_GET = 60,
    DEV_ADVANCED_SET = 61,

    /* snmp */
    SNMP_GET         = 70,
    SNMP_SET         = 71,

    /* legap */
    LEGAP_GET        = 75,
    LEGAP_SET        = 76,

    /* ap flow */
    AP_FLOW_GET      = 80,
    AP_FLOW_SET      = 81,
    
}opera_no;

/*------------------ 功能宏定义 ------------------*/
/* 配置的变量长度定义 */
#define CONFIG_VALUE_LEN  32
#define CONFIG_IP_NUM     2

/* 登录配置 */
typedef struct _user_login
{
    int language;
    char username[CONFIG_VALUE_LEN];
    char password[CONFIG_VALUE_LEN];
}user_login;

/* home_page.html */
typedef struct _home_page
{
    int language;
}home_page;

/* dev_basic.html */
typedef struct _dev_basic
{
    int language;
    int countrycode;
    char devname[CONFIG_VALUE_LEN];
    char software[CONFIG_VALUE_LEN];
    char hardware[CONFIG_VALUE_LEN];
}dev_basic;

/* sntp.html */
typedef struct _sntp
{
    int language;
    int iswitch;
    int period;
    char url[CONFIG_VALUE_LEN];
}sntp;

/* ipv4.html */
#define BR_NAME         "br"
#define IP_MODEL_STATIC         0
#define IP_MODEL_DHCP_CLIENT    1
#define IP_MODEL_PPPOE_CLIENT   2
#define IP_MODEL_3G_4G_LTE      3

typedef struct _ipv4
{
    int ipmodule;
    unsigned char gateway[4];
    unsigned char dns[CONFIG_IP_NUM][4];
    unsigned char ipaddr[CONFIG_IP_NUM][4];
    unsigned char mask[CONFIG_IP_NUM][4];
    int vlan[CONFIG_IP_NUM];
    char pppoe_name[32];
    char pppoe_pwd[32];
}ipv4;

/* lan_config.html */

typedef struct _lan_cf
{
    char lan_ip[32];
    char lan_mask[32];
	int  lan_freeauth;/*add@2014.9.12*/
}lan_cf;


typedef enum _eth_port_mode
{
	PORT_MODE_WAN1 = 0,
	PORT_MODE_WAN2 = 1,
	PORT_MODE_WAN3 = 2,
	PORT_MODE_WAN_MAX = 3,
	PORT_MODE_LAN1 = 4,
	PORT_MODE_LAN2 = 5,
	PORT_MODE_LAN3 = 6,
	PORT_MODE_MGMT = 7,
	PORT_MODE_UNDEF = 8
}eth_port_mode;

typedef struct _portconf_cf
{
	eth_port_mode eth2_mode;
	eth_port_mode eth3_mode;
}portconf_cf;


/* wan_rate.html */
#define WAN_SPEED_AUTO        0
#define WAN_SPEED_HALF_10M    1
#define WAN_SPEED_10M         2
#define WAN_SPEED_HALF_100M   3
#define WAN_SPEED_100M        4
#define WAN_SPEED_1000M       5

#define WAN_DUPLEX_FULL     1
#define WAN_DUPLEX_HALF     2
#define WAN_DUPLEX_AUTO     0
typedef struct _wan_rate
{
    int speed;
    int duplex;
}wan_rate;

/* dhcp_server.html */
#define DHCP_SERVER_OFF     0
#define DHCP_SERVER_ON      1

typedef struct _dhcp_server_param
{
    char startip[32];   /* 起始IP */
    char endip[32];     /* 终止IP */
    char mask[32];      /* 子网掩码 */
    char gateway[32];   /* 网关 */
    char dns[32];       /* 主DNS */
    char sub_dns[32];   /* 备DNS */
    char lease[32];     /* 该地址池所分配的租约时间 */
    char vlan_band[128]; /* 该地址池所绑定的VLAN */
    int  vlan_arr[32];   /* vlan数组表现形式 */
    int  vlan_num;       /* vlan数目 */
    int  br_x;           /* 该地址池桥序号 */
    char lan_ip[32];    /* 该地址池LAN口的IP */
    char lan_mask[32];  /* 该地址池LAN口的掩码 */
}dhcp_server_param;

typedef struct _dhcp_server
{
    int dhcp_switch;                /* DHCP 开关 */
    int ope_id;                     /* 操作ID */
    int nvram_id;                     /* WEB修改地址池的编号 */
    int num;                        /* DHCP地址池个数 */
    char dhcp_option43[32];         /* 全局option 43(AC发现和网关AP通信使用)变量 */
    char dhcp_option15[32];         /* 全局option15(domain name使用)变量 */
    dhcp_server_param dhcp_para[30];    /* DHCP 参数,最多30个地址池 */
}dhcp_server;

typedef struct _dhcpd_server
{
    int module;
    int lease;
    unsigned char startip[4];
    unsigned char endip[4];
    unsigned char mask[4];
    unsigned char gateway[4];
    unsigned char dns[4];
    unsigned char sub_dns[4];
}dhcpd_server;


/* pppoe_client.html */
typedef struct _pppoe 
{
    int module;
    char account[CONFIG_VALUE_LEN];
    char password[CONFIG_VALUE_LEN];
}pppoe;

/* net_advanced.html */
typedef struct _net_advanced
{
    int arp_time;
    int isolation_switch;
}net_advanced;

#define WIFI_NAME_LEN 16
typedef struct _wifi_basic
{
    int wifi_id;
    char wifi_name[WIFI_NAME_LEN];
    int wifi_switch;
    int country_code;
    int freq; /* wifi 频段 */
}wifi_basic;

#define MODE_11A            1
#define MODE_11B            2
#define MODE_11G            3
#define MODE_11B_11G        6
#define MODE_11AN           8
#define MODE_11A_11AN       9
#define MODE_11GN           16
#define MODE_11G_11GN       20
#define MODE_11B_11G_11GN   22

#define BAND_20MHZ          0
#define BAND_20MHZ_40MHZ    1
#define BAND_40MHZ          2

typedef struct _wifi_80211n
{
    int flag;
    int mode;
    int band;
    int channel;
    int rfnum;
    int shortgi;
    int ampdu;
    int amsdu;
    int limit_txpower;
    int decay_txpower;
    
/* 以下配置页面暂不提供接口 */
    int power_save;
    int htdelyblackack;
    int max_amsdu;
    int max_rxfactor;
    int minstaspacing;
    int hisuppdaterate;
    int ampdu_buff_size;
    int htc_supp;
}wifi_80211n;

typedef struct _wifi_txpower
{
    int limit_txpower;
    int current_txpower;
}wifi_txpower;

typedef struct _wifi_rate
{
    int beacon;
    int mgt;
    int ctl;
    int mcast;
#define RATE_MODE_AUTO      0
#define RATE_MODE_MANUAL    1
    int rates_mode;
    char legacy[256]; /* 非 11n速率集 */
    char mcs[256];    /* 11n速率集 */
}wifi_rate;

typedef struct _wifi_envmoni
{
    int i_switch;
    int i_cyc;
    char channel_list[256];
}wifi_envmoni;

enum wme_info_e
{
    BE_STA_INFO,
    BE_AP_INFO,
    BG_STA_INFO,
    BG_AP_INFO,
    VI_STA_INFO,
    VI_AP_INFO,
    VO_STA_INFO,
    VO_AP_INFO,
    WME_INFO_MAX,
};


typedef struct _wifi_wmm
{
    int acm;
    int aifs;
    int cw_max;
    int cm_min;
    int txop_limit;
}wifi_wmm;

typedef struct _wifi_wmm_web
{
    char be_sta[32];
    char be_ap[32];
    char bg_sta[32];
    char bg_ap[32];
    char vi_sta[32];
    char vi_ap[32];
    char vo_sta[32];
    char vo_ap[32];
}wifi_wmm_web;


#define RSSI_MODE_STATIC    0
#define RSSI_MODE_DYNAMIC   1
typedef struct _wifi_advanced
{
    int beacon_interval; /* 单位ms */
    int preamble;
    int cts; /* cts 保护 */
    int max_tx_distance;
    int rssi_mode;
    int rssi_static_threshod;
}wifi_advanced;

typedef struct _un_wifis
{
   wifi_basic st_basic;
   union
   {
       wifi_80211n st_mode;
       wifi_rate st_rate;
       wifi_txpower st_txpower;
       wifi_advanced st_advanced;
       wifi_envmoni st_moni;
       wifi_wmm_web st_wmm;
   }u;
}un_wifis;

typedef struct _wifi_config
{
    int wifi_id;
    char wifi_name[32];
    wifi_80211n st_mode;
    wifi_rate st_rate;
    wifi_txpower st_txpower;
    wifi_advanced st_advanced;
    wifi_envmoni st_moni;
}wifi_config;

/*----- vap -----*/
#define SSID_NAME_LEN   32
#define SSID_AP         0
#define SSID_STA        1
#define VAP_AP_NAME    "ap"
#define VAP_STA_NAME   "sta"
typedef struct _vap_basic
{
    int wlan_id;
    int wifi_id;
    int ssid_mode;                  /* ssid 工作模式 */
    char vap_name[SSID_NAME_LEN];   /* ssid 设备名称 */
    char ssid_name[SSID_NAME_LEN];
    char bssid[6];
    char rsv[2];
    int ssid_enabled;
    int ssid_hide;
    int free_auth;
    int wds;
    int ssid_isolate;
    int ssid_max_users;
    int sta_inact;
    int ssid_encode;
}vap_basic;

typedef struct _vap_qos
{
    int traffic_type;
    char name[32];
    int ssid_max_upload;
    int ssid_max_download;
    int sta_max_upload;
    int sta_max_download;
    int pcf;
}vap_qos;

/* 加密配置 */
typedef enum _ieee80211_auth_mode
{
    OPEN                = 1,
    SHARED              = 2,
    WPA_PSK             = 3,
    WPA2_PSK            = 4,
    WPA_PSK_WPA2_PSK    = 5,
    WPA                 = 6,
    WPA2                = 7,
    WPA_WPA2            = 8,
}ieee80211_auth_mode;

typedef enum _open_mode
{
    DISABLED   = 1,
    WEP        = 2,
}open_mode;

typedef enum _wep_mode
{
    ASSIC   = 1,
    HEX     = 2,
    BIT64   = 3,
    BIT128  = 4,
}wep_mode;

typedef enum _wpa_mode
{
    TKIP    = 1,
    AES     = 2,
    AUTO    = 3,
}wpa_mode;

typedef struct _wep
{
    int len;
    int format;
    int index;
    char key[32];
}wep;

typedef struct _open
{
    int type;
    wep st_wep;
}open_t;

typedef struct _shared
{
    wep st_wep;
}shared;

typedef struct _wpa_personal
{
    int type;
    char key[64];
    int renew;
}wpa_personal;

typedef struct wpa_enterprise
{
    int type;
    char auth_ip[4];
    int auth_port;
    char auth_key[64];
    char acct_ip[4];
    int acct_port;
    char acct_key[64];
    int acct_interim;
    int send_interval; /* 消息重发间隔 */
}wpa_enterprise;

typedef struct _encrypt
{
    int auth_type;
    union
    {
        open_t st_open;
        shared st_shared;
        wpa_personal st_wpa_per;
        wpa_enterprise st_wpa_ent;
    }u;
}encrypt;

typedef struct _vap_acl
{
    int acl_switch;
    int rule;
    char acl[512];
}vap_acl;

typedef struct _vap_advanced
{
    int dtim;
    int frag;
    int rts;
    //add by zhz
    int mac_acc_sw; //mac开关
    int num;        //MAC个数
    int nvram_id;   //vap 编号
    int del_id;     //删除的ID
    int oper_id;    //操作ID
    char acMac[32]; //WEB 增加的MAC
}vap_advanced;

typedef struct _un_vaps
{
    int wlan_id;
    int wifi_id;
    int nvram_id;
    int len; /* 合法性检测 */
    union
    {
        vap_basic st_basic;
        vap_qos st_qos;
        encrypt st_en;
        vap_advanced st_advanced;
        vap_acl st_acl;
    }u;
}un_vaps;

typedef struct _vap
{
    int nvram_id;
    int wlan_id;
    int wifi_id;
    int ssid_mode;
    int wifi_freq;
    char ssid_status[32];
    char ssid_name[32];
    char bssid[6];
    char rsv[2];
}vap;

typedef struct _vap_config
{
    int nvram_id;
    vap_basic st_basic;
    vap_qos st_qos;
    encrypt st_en;
    vap_advanced st_advanced;
}vap_config;



/*--------------- system ---------------*/
typedef enum _en_restore_flag
{
    FLAG_RESTORE = 0,
    FLAG_IMPORT  = 1,
    FLAG_EXPORT  = 2,
}en_restore_flag;

typedef enum _en_factory_restore
{
    ALL_RESTORE         = 0,
    IP_RESTORE          = 1,
    NETWORK_RESTORE     = 2,
    WIFI_RESTORE        = 3,
    ACCOUNT_RESTORE     = 4,
    OTHER_RESTORE       = 5,
}en_factory_restore;

typedef struct _factory_restore
{
    int all_restore;
    int ip_restore;
    int network_restore;
    int wifi_restore;
    int account_restore;
    int other_restore;
}factory_restore;

/*--------------- vlan config ---------------*/
#define VLAN_MAX        31
typedef struct _vlan_info
{
    int id;         /* nvram id */
    int tag;        /* vlan tag */
    int priority;   /* 优先级 0~7 */
    char name[32];  /* vlan 名称,自定义 */
}vlan_info;

#define TRUNK_NUM       5

typedef enum _en_port_mode
{
    PORT_ACCESS = 0,
    PORT_TRUNK  = 1,
}en_port_mode;

typedef enum _en_tag_enabled
{
    TAG_DISABLE = 0,
    TAG_ENABLE  = 1,
}en_tag_enabled;

typedef struct _vlan_map
{
    char port_id[32]; /* vap nvram id */
    char name[32];
    char port_name[32];  /* 端口名称 */
    char port_mode[32]; /* 端口模式, 0:access 1:trunk */
    int vlan_num;       /* 该端口的vlan id数目 by zhz */
    int vlan_free_auth_num; /* 免认证的 vlan id 数目 */
    char vlan_id[30][32];   /* VLAN ID 数组 */
    char vlan_id_free_auth[30][32]; /* 免认证VLAN id数组 */
    char isEnabled[32];     /* 是否使能VLAN 页面中的tag是否勾选 */
}vlan_map;

/* 账户管理 */
typedef enum _en_users_flag
{
    FLAG_USERS_ADD  = 0,
    FLAG_USERS_DELETE,
    FLAG_USERS_MODIFICATION
}users_flag;

typedef struct _users
{
    int flag;
    int limits;
    char name[32];
    char old_pw[32];
    char new_pw[32];
}users;

#define STA_MAX 128
typedef struct _sta_info {
    int iWifiId;
    char aucUserMac[6];
    char ucRecv[2];
    char acSsidName[32];
    int  uiUserLiveTime;
    char acSignalStrength;
    char company[128];
    char systeminfo[32];
}sta_info;

typedef struct _stas_info {
    int iUserMaxNums;
    sta_info stUserInfo[STA_MAX];
}stas_info;

typedef struct _arp_tables
{
    char ip[32];
    int type;
    int flags;
    char hwa[32];
    char mask[32];
    char dev[32];
}arp_tables;

typedef struct _vaps_infos
{
    char ssid[32];
    char mac[32];
    char auth_type[32];
    int ssid_status;
    int vlanStatus;
    int channel;
}vaps_infos;


/* multiple ssid and single ssid */
typedef struct _single_ssid
{
    char nvram_id[32];
    char ssid_enabled[32];
    char ssid_name[32];
    char vlan_enabled[32];
    char pvid[32];
    char qos[32];
    char security[32];
    char radius_ip[32];
}single_ssid;


#define WEB_LEG_AP_NUMS 16

struct web_leg_ap{
    char bssid[32];
    char ssid[32];
    char id[32];
};

#define MAXRECNUM_ROGUEAP         ((unsigned int)64)

typedef struct
{
    unsigned int  ulIndex;
    unsigned char   aucBssid[6];
    char    acSsid[32];
    int   iRssi;
    unsigned int  ulChannel;
}rogue_ap;

typedef struct
{
    unsigned int  ulIndex;
    unsigned char   aucBssid[6];
    char    acSsid[32];
    int   iRssi;
    unsigned int  ulChannel;
}ssid_scan;

typedef struct _wifidog
{
    char acSwitch[32];
    char name[32];
    char url[128];
    char path[64];
}wifidog;

typedef struct _tr69c 
{
    char tr69cSwitch[32];
    char acsUrl[128];
    char acsName[32];
    char acsPwd[32];
    char handSwitch[32];
    char handInterval[32];
    char conUserSwitch[32];
    char conUserName[32];
    char conUserPwd[32];
    char localPort[32];
}tr69c;

typedef struct _cdn 
{
    char cdnSwitch[32];
    char cdnUrl[128];
    char gwUrl[128];
}cdn;


typedef struct
{
	char aVpnSwitch[32]; //switch
	char acVpnServerIP[32]; //serverIP
	char acVpnProtocolType[32];
	char acVpnUserName[32]; //username
	char acVpnPasswd[32]; //password
}VPN_L2TP_CONTROL_INFO_T;


typedef struct
{
    char acFlowStaticSwitch[32];
    char acUpBandWidth[32];
    char acDownBandWidth[32];
}FLOW_STATISTICS_T;



typedef struct
{
    char acPort[32];
}HTTP_PORT_T;


typedef struct _authorize_control_t
{
    int ios_auto_portal;
}authorize_control_t;


#define FORWARD_MODE_NAT    0
#define FORWARD_MODE_ROUTE  1
#define FORWARD_MODE_AP     2
typedef struct _forward_mode
{
    char mode[32];
}forward_mode;


#define WX_FOLLOW_OFF		0
#define WX_FOLLOW_ON		1
typedef struct _wx_follow
{
	char sw[32];
}wx_follow;

/*begin: add@2014.9.11*/
/*ROUTE INFO*/
typedef struct 
{
    int ulRouteIndex;
    unsigned char aucDstIp[4];
    unsigned char aucDstNetMask[4];
    unsigned char aucNextStop[4];
    int ulRoutePri;
    unsigned char aucIfName[32];
}OM_NET_ROUTE_STATIC_CFG_T;
/*end: add@2014.9.11*/

typedef struct 
{
#define PROT_TCP 0
#define PROT_UDP 1
    int ulPortmapIndex;
    unsigned short usOutPort;
    unsigned short usInPort;
	unsigned char aucIp[4];
	unsigned char ucProt;
}OM_NET_PORTMAP_STATIC_CFG_T;


#define MAC_FORMAT_SEMICOLON  0  /* mac地址分号分隔，如00:34:cb:11:22:33 */
#define MAC_FORMAT_DASH       1  /* mac地址横杠分隔，如00-34-cb-11-22-33 */

/* BEGIN: Added by WeiHonggang, 2014/4/11   PN:TASK#0 定位功能相关 */
#define LOCATOR_DEF_SW "0"
#define LOCATOR_DEF_SERVER_IP "192.168.1.249"
#define LOCATOR_DEF_SERVER_PORT "10608"
#define LOCATOR_DEF_INTERVAL "1"
#define LOCATOR_DEF_RPT_INTERVAL "600"
#define LOCATOR_DEF_QUIT_INTERVAL "30"
#define LOCATOR_DEF_SCAN_TYPE "STA"
#define LOCATOR_DEF_ENGINE "uradio"
#define LOCATOR_DEF_SCAN_CHAN "0x0"

typedef struct
{
    char  sw[CONFIG_VALUE_LEN];
    char ServerIp[CONFIG_VALUE_LEN];
    char ServerPort[CONFIG_VALUE_LEN];
    char Interval[CONFIG_VALUE_LEN];
    char ScanType[CONFIG_VALUE_LEN];
	char Engine[CONFIG_VALUE_LEN];
	char ScanChannel[CONFIG_VALUE_LEN];
}locator_param;

typedef struct
{
    char sw[CONFIG_VALUE_LEN];
    char ServerIp[CONFIG_VALUE_LEN];
    char ServerPort[CONFIG_VALUE_LEN];
    char Interval[CONFIG_VALUE_LEN];
    char ScanType[CONFIG_VALUE_LEN];
	char Engine[CONFIG_VALUE_LEN];
	char ScanChannel[CONFIG_VALUE_LEN];
	char RptInterval[CONFIG_VALUE_LEN];
	char QuitInterval[CONFIG_VALUE_LEN];
}stadct_param;

/* END:   Added by WeiHonggang, 2014/4/11   PN:TASK#0 定位功能相关 */


typedef struct _vlan_fc_item
{
	int vlan_id;
	int up_rate;
	int down_rate;
}vlan_fc_item;


typedef struct _vlan_flowctrl
{
	int num;					/*vlan 限速配置条数*/
	vlan_fc_item vfc[30];		/*最多配置30条vlan限速*/
}vlan_flowctrl;

/* network_diagnose.html */
typedef struct _net_diagnose
{
    int diagnose_type;
    char domain[64];
    int ping_num;
    int ping_size;
    int traceroute_num;
}net_diagnose;
#define DIAGNOSE_PING           0
#define DIAGNOSE_TRACEROUTE     1
#define DIAGNOSE_FILE           "/tmp/diagnose.txt"


#endif

