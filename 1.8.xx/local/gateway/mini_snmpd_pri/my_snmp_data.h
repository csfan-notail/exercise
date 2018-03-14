/******************************************************************************
* COPYRIGHT GBCOM
*******************************************************************************
* 功    能: 
* 修改历史: 
* 2012-9-10     冯静              新建
******************************************************************************/

/******************************* 宏定义 ********************************/
#ifndef __MINI_SNMPD_DATA__
#define __MINI_SNMPD_DATA__

#define SNMPAGENT_CONTROL_NAME "snmpagentd"
#define AP_AGENT_TRAP_PORT       162
#define AP_AGENT_TRAP_TIMEOUT    5

// 增加缺省读写共同体定义
#define AGENT_RO_COMMUNITY    "public"
#define AGENT_RW_COMMUNITY    "private"

/*认证类型定义*/
#define authenticationType_OPENSYSTEM         1
#define authenticationType_SHAREDKEY          2
#define authenticationType_WPAPSK             3
#define authenticationType_WPA                4
#define authenticationType_WAPI               5

/*加密类型定义*/
#define securityMode_DISABLE    0
#define securityMode_WPAPSK     1
#define securityMode_WEP        2
#define securityMode_WPA        3

/*加密机制定义*/
#define encyptType_TKIP                 1
#define encyptType_AES                  2

/*IPv4地址类型*/
#define ENUM_IP_MANUAL      0
#define ENUM_IP_DHCP        1

/*动态信道开关*/
#define AUTOCHANNEL_OFF     0
#define AUTOCHANNEL_ON      1

/*信道定义*/
/*2.4G*/
#define ENUM_APWIFICHANNEL_CH1 ((UINT32)1)
#define ENUM_APWIFICHANNEL_CH2 ((UINT32)2)
#define  ENUM_APWIFICHANNEL_CH3 ((UINT32)3)
#define  ENUM_APWIFICHANNEL_CH4 ((UINT32)4)
#define  ENUM_APWIFICHANNEL_CH5 ((UINT32)5)
#define  ENUM_APWIFICHANNEL_CH6 ((UINT32)6)
#define  ENUM_APWIFICHANNEL_CH7 ((UINT32)7)
#define  ENUM_APWIFICHANNEL_CH8 ((UINT32)8)
#define  ENUM_APWIFICHANNEL_CH9 ((UINT32)9)
#define  ENUM_APWIFICHANNEL_CH10 ((UINT32)10)
#define  ENUM_APWIFICHANNEL_CH11 ((UINT32)11)
#define  ENUM_APWIFICHANNEL_CH12 ((UINT32)12)
#define  ENUM_APWIFICHANNEL_CH13 ((UINT32)13)

/*5G*/	
#define  ENUM_APWIFICHANNEL_CH52 ((UINT32)52)
#define  ENUM_APWIFICHANNEL_CH56 ((UINT32)56)
#define  ENUM_APWIFICHANNEL_CH60 ((UINT32)60)
#define  ENUM_APWIFICHANNEL_CH64 ((UINT32)64)
#define  ENUM_APWIFICHANNEL_CH100 ((UINT32)100)
#define  ENUM_APWIFICHANNEL_CH104 ((UINT32)104)
#define  ENUM_APWIFICHANNEL_CH108 ((UINT32)108)
#define  ENUM_APWIFICHANNEL_CH112 ((UINT32)112)
#define  ENUM_APWIFICHANNEL_CH116 ((UINT32)116)
#define  ENUM_APWIFICHANNEL_CH132 ((UINT32)132)
#define  ENUM_APWIFICHANNEL_CH136 ((UINT32)136)
#define  ENUM_APWIFICHANNEL_CH140 ((UINT32)140)
#define  ENUM_APWIFICHANNEL_CH149 ((UINT32)149)
#define  ENUM_APWIFICHANNEL_CH153 ((UINT32)153)
#define  ENUM_APWIFICHANNEL_CH157 ((UINT32)157)
#define  ENUM_APWIFICHANNEL_CH161 ((UINT32)161)
#define  ENUM_APWIFICHANNEL_CH165 ((UINT32)165)

/* BEGIN: Added by zoucaihong, 2014/5/15 */
/* AP设备硬件类型定义 */
#define AP_HWTYPE_AR9341        ((UINT32)5)
#define AP_HWTYPE_AR9558        ((UINT32)8)
#define AP_HWTYPE_AR7240        ((UINT32)1)
#define AP_HWTYPE_AR9344        ((UINT32)4)
#define AP_HWTYPE_AR9531       ((UINT32)9)
#define AP_HWTYPE_AR9563        ((UINT32)10)
/* END: Added by zoucaihong, 2014/5/15 */

/* BEGIN: Added by zoucaihong, 2014/6/17 */
/* 配置文件导入导出定义 */
typedef enum
{
    AP_CFGFILE_NORMAL = 0,
    AP_CFGFILE_IMPORT,
    AP_CFGFILE_EXPORT
}AP_CFGFILE_E;
/* END: Added by zoucaihong, 2014/6/17 */

/* BEGIN: Added by zoucaihong, 2014/7/15 */
typedef enum
{
    AP_SYSOPER_NORMAL = 0,
    AP_SYSOPER_SET
}AP_SYSOPER_E;
/* END: Added by zoucaihong, 2014/7/15 */

/* BEGIN: Added by Zhou Shouya, 2013/8/9 */
/* 设备软件升级状态
 * 1: success(1) 2: get-file-fail(2) 3: file-wrong(3) 4: flash-error(4) 
 */
typedef enum
{
    AP_SWUPDATE_SUCCESS=1,
    AP_SWUPDATE_DOWNLOADFAIL,
    AP_SWUPDATE_PARAMINVALID,
    AP_SWUPDATE_FLASHFAIL
}AP_SWUPDATE_STATUS_E;

/* BEGIN: Added by zoucaihong, 2014/7/16 */
typedef enum
{
    FILE_SOFTWARE=1,
    FILE_CONFIG,
    FILE_DIAGNOSE,
    FILE_OTHER
}AP_FILE_TYPE_E;
/* END: Added by zoucaihong, 2014/7/16 */

typedef enum{
    TRANS_NONE=0,
    TRANS_FTP=1,
    TRANS_TFTP=2,
    TRANS_HTTP=3,
    TRANS_HTTPS=4,
    TRANS_MAX,
}AP_TRANS_PROTO_E;
#define FTP_PORT        21
/* END:   Added by Zhou Shouya, 2013/8/9 */
/* BEGIN: Added by Zhou Shouya, 2013/9/2 */
#ifndef LITTLE_ENDIAN
#define  LITTLE_ENDIAN  ((UINT32)1)
#define  BIG_ENDIAN     ((UINT32)2)
#endif
/* END:   Added by Zhou Shouya, 2013/9/2 */

/* BEGIN: Added by zoucaihong, 2014/5/13 */
/* 设备状态枚举 */
typedef enum{
    DEV_STATE_NONE=0,
    DEV_STATE_DISCOVER=1,
    DEV_STATE_RUNNING=2,
    DEV_STATE_MAX
}DEV_STATE_E;

/* 设备管理状态 */
typedef enum{
    MANAGE_STATE_OFFLINE=0,
    MANAGE_STATE_ONLINE=1,
    MANAGE_STATE_MAX
}MANAGE_STATE_E;

#define DEV_DISCOVER_PERIOD      60//second
#define DEV_CMS_TIMEOUT          60*2//second
#define DEV_HEART_PERIOD         30//second

#define TICKS_UP()do{                           \
    g_ulTicks++;                                \
    if( DEV_STATE_RUNNING == g_enDevState )           \
    {                                           \
        g_ulHeartTicks++;/* 心跳计时 */   \
    }                                           \
    else if( DEV_STATE_DISCOVER == g_enDevState )           \
    {                                           \
        g_ulDisTicks++;/* discover超时计时 */   \
    }                                           \
	agent_printf(LOG_DEBUG,"g_ulTicks:%d,g_ulHeartTicks:%d,g_ulDisTicks:%d,state:%s",\
	g_ulTicks,g_ulHeartTicks,g_ulDisTicks,g_aucStateName[g_enDevState]); \
}while(0);









/* END: Added by zoucaihong, 2014/5/13 */
/****************************snmp读写共同体配置文件***************************/
#define SNMPAGENT_FILE_CONF                "/tmp/snmp_agent.conf"
#define FILE_TRANS_DIR          "/tmp/app.tar.gz"
#define UNCOMPRESS_DIR          "/tmp/cms-version"
#define UPDATE_IMAGE_LOG        "/tmp/auto_up_ver_info"
#define CURR_VERSION            "currVersion"
#define TARG_VERSION            "TargVersion"
#define CONFIG_FILE_DIR         "/tmp/tmp.bin"
#define CCS_CONIG_FILE          "/tmp/ccscfg"

/* BEGIN: Added by zoucaihong, 2014/5/14 */
/* DHCP Option43配置文件 */
#define DHCP_OPTION43_CONF                 "/var/db/ipcfg.conf "
/* END: Added by zoucaihong, 2014/5/14 */
#define BACKUP_NAME         "/tmp/backup.dat"
/* BEGIN: Added by zoucaihong, 2014/7/16 */
#define PROBLEM_COLLECT_FILE    "/tmp/dev_problem_file.tar.gz"
#define PROBLEM_COLLECT_SCRIPT  "/etc/dev_problem_script.sh"
/* END: Added by zoucaihong, 2014/7/16 */





/* 配置模板更新结果宏定义*/
/*
0：成功
1：文件传输配置异常
2：配置文件FTP下载失败
3：配置文件数据不合法
4：其它原因
*/
#define CONFIGFILE_UPDATE_RESULT_SUCCESS                0
#define CONFIGFILE_UPDATE_RESULT_FILETRANSCFGINVALID    1
#define CONFIGFILE_UPDATE_RESULT_FILETRANSFAIL          2
#define CONFIGFILE_UPDATE_RESULT_CONFIGFILEINVALID      3
#define CONFIGFILE_UPDATE_RESULT_OTHER                  4







/******************************NVRAM中定义************************************/
#ifdef OPENWRT

#define UCI_NETWORK "network."
#define UCI_GATEWAY "gateway.@gateway[0]."
#define UCI_SYSTEM "system.@system[0]."



/*系统相关*/
#define NVRAM_SYSGWADDRESS          "netWanDefaultGw"
#define NVRAM_SYSIPMODE             "netSysIpMethod"
#define NVRAM_SYSCONTACT            "sysContact"
#define NVRAM_SYSNAME               "sysName"
#define NVRAM_SYSLOCATION           "sysLocation"
/* BEGIN: Added by zoucaihong, 2012/12/25 */
#define NVRAM_REGETPERIOD           UCI_GATEWAY"snmpStatCycle"
/* END: Added by zoucaihong, 2012/12/25 */
#define NVRAM_TIMEOUTINTERVAL       UCI_GATEWAY"timeoutInterval"
#define NVRAM_HEARTBEATPERIOD       UCI_GATEWAY"heartBeatPeriod"

/*IP v4*/
#define NVRAM_SYSIPADDRESS          "netWanIpAddr"
#define NVRAM_SYSIPNETMASK          "netWanNetMask"
#define NVRAM_SYSIPPORT             "netWanPort"
#define NVRAM_SYSIPADDRESS2         "netWanIpAddr_second"
#define NVRAM_SYSIPNETMASK2         "netWanNetMask_second"
#define NVRAM_SYSIPPORT2            "netWanPort_second"


/*无线信道配置*/
#define NVRAM_CHANNELMODE           "AutoChanSwitch"
#define NVRAM_AUTOCHANTIME          "AutoChanTime"
#define NVRAM_AUTOCHANLIST          "AutoChanList"
#define NVRAM_STATICCHANNEL         "ModuleChannelSet"
#define NVRAM_RFNUM                 "ChainMask"
#define NVRAM_ABGMODE               "AbgMode"
/*无线高级配置*/
#define NVRAM_SWITCH                "RssiSwitch"
#define NVRAM_MODE                  "RssiMode"
#define NVRAM_RSSIHIGH              "RssiHigh"
#define NVRAM_RSSILOW               "RssiLow"
#define NVRAM_RSSIINTERVAL          "RssiInterval"
/*无线功率配置*/
#define NVRAM_MAXTXPOWER            "CurTxPower"

/*VAP相关*/
#define NVRAM_SSIDNAME              "Ssid"
#define NVRAM_VAPWIFIID             "RadioId"
#define NVRAM_VAPVAPID              "WlanId"
#define NVRAM_AUTHENTICATIONMODE    "AuthenticationType"
#define NVRAM_SECURITYMODE          "SecurityMode"
#define NVRAM_WPAALGORITHM          "WpaAlgorithm"
#define NVRAM_VAPBROADCAST          "BroadcastSsid"
#define NVRAM_VAPENABLE             "VapStatus"
#define NVRAM_WPAKEY                "wpaPsk"
#define NVRAM_WPAKEYRENEWAL         "wpaRekeyPeriod"
#define NVRAM_WPAKEYFORMAT          ""
#define NVRAM_WEPKEYLEN             "wepKeyLen"
#define NVRAM_WEPKEYFORMAT          "wepInputFormat"
#define NVRAM_WEPKEY                "wepKey"
#define NVRAM_WEPKEYINDEX           "wepKeyIndex"
#define NVRAM_WEPBROADCASTKEYLEN    "wepBroadcastKeyLen"
#define NVRAM_WEPUNICASTKEYLEN      "wepUnicastKeyLen"
#define NVRAM_WEPREKEYPERIOD        "wepRekeyPeriod"
#define NVRAM_8021XMODE             "8021xMode"
#define NVRAM_EAPOLVER              ""
#define NVRAM_EAPAUTHPERIOD         "EapAurhPeriod"
#define NVRAM_ENDUSERTIME           "EndUserTime"
#define NVRAM_ENDUSERMAXNUM         "endUserMaxNum"
/*VAP流控*/
#define NVRAM_FLOWSWITCH            "Flowswtich"
#define NVRAM_UPLOADMAXRATE         "Uploadflow"
#define NVRAM_DOWNLOADMAXRATE       "Downloadflow"

/*VLAN相关*/
#define NVRAM_VLANID                "VlanId"
#define NVRAM_VLANNAME              "VlanName"
#define NVRAM_VLANSSID              "VlanSsid"
#define NVRAM_VLANPORT              "VlanPort"
#define NVRAM_VLANPRRIO             "VlanPrio"

/*告警相关*/
#define NVRAM_ALARMCPULOADHIGH      UCI_GATEWAY"alarmCpuLoadHigh"
#define NVRAM_ALARMCPULOADLOW       UCI_GATEWAY"alarmCpuLoadLow"
#define NVRAM_ALARMMEMLOADHIGH      UCI_GATEWAY"alarmMemLoadHigh"
#define NVRAM_ALARMMEMLOADLOW       UCI_GATEWAY"alarmMemLoadLow"

/*trap相关*/
#define NVRAM_TRAPDSWTICH           UCI_GATEWAY"trap_switch"
#define NVRAM_TRAPDESIP             UCI_GATEWAY"trap_ip"
#define NVRAM_TRAPDESPORT           UCI_GATEWAY"trap_port"
#define NVRAM_TRAPCOMMUITY          UCI_GATEWAY"trap_community"


#define NVRAM_CONFIGVERSION     UCI_GATEWAY"snmp_config_version"

#else
/*系统相关*/
#define NVRAM_SYSGWADDRESS          "netWanDefaultGw"
#define NVRAM_SYSIPMODE             "netSysIpMethod"
#define NVRAM_SYSCONTACT            "sysContact"
#define NVRAM_SYSNAME               "sysName"
#define NVRAM_SYSLOCATION           "sysLocation"
/* BEGIN: Added by zoucaihong, 2012/12/25 */
#define NVRAM_REGETPERIOD           "snmpStatCycle"
/* END: Added by zoucaihong, 2012/12/25 */
#define NVRAM_TIMEOUTINTERVAL       "timeoutInterval"
#define NVRAM_HEARTBEATPERIOD       "heartBeatPeriod"

/*IP v4*/
#define NVRAM_SYSIPADDRESS          "netWanIpAddr"
#define NVRAM_SYSIPNETMASK          "netWanNetMask"
#define NVRAM_SYSIPPORT             "netWanPort"
#define NVRAM_SYSIPADDRESS2         "netWanIpAddr_second"
#define NVRAM_SYSIPNETMASK2         "netWanNetMask_second"
#define NVRAM_SYSIPPORT2            "netWanPort_second"


/*无线信道配置*/
#define NVRAM_CHANNELMODE           "AutoChanSwitch"
#define NVRAM_AUTOCHANTIME          "AutoChanTime"
#define NVRAM_AUTOCHANLIST          "AutoChanList"
#define NVRAM_STATICCHANNEL         "ModuleChannelSet"
#define NVRAM_RFNUM                 "ChainMask"
#define NVRAM_ABGMODE               "AbgMode"
/*无线高级配置*/
#define NVRAM_SWITCH                "RssiSwitch"
#define NVRAM_MODE                  "RssiMode"
#define NVRAM_RSSIHIGH              "RssiHigh"
#define NVRAM_RSSILOW               "RssiLow"
#define NVRAM_RSSIINTERVAL          "RssiInterval"
/*无线功率配置*/
#define NVRAM_MAXTXPOWER            "CurTxPower"

/*VAP相关*/
#define NVRAM_SSIDNAME              "Ssid"
#define NVRAM_VAPWIFIID             "RadioId"
#define NVRAM_VAPVAPID              "WlanId"
#define NVRAM_AUTHENTICATIONMODE    "AuthenticationType"
#define NVRAM_SECURITYMODE          "SecurityMode"
#define NVRAM_WPAALGORITHM          "WpaAlgorithm"
#define NVRAM_VAPBROADCAST          "BroadcastSsid"
#define NVRAM_VAPENABLE             "VapStatus"
#define NVRAM_WPAKEY                "wpaPsk"
#define NVRAM_WPAKEYRENEWAL         "wpaRekeyPeriod"
#define NVRAM_WPAKEYFORMAT          ""
#define NVRAM_WEPKEYLEN             "wepKeyLen"
#define NVRAM_WEPKEYFORMAT          "wepInputFormat"
#define NVRAM_WEPKEY                "wepKey"
#define NVRAM_WEPKEYINDEX           "wepKeyIndex"
#define NVRAM_WEPBROADCASTKEYLEN    "wepBroadcastKeyLen"
#define NVRAM_WEPUNICASTKEYLEN      "wepUnicastKeyLen"
#define NVRAM_WEPREKEYPERIOD        "wepRekeyPeriod"
#define NVRAM_8021XMODE             "8021xMode"
#define NVRAM_EAPOLVER              ""
#define NVRAM_EAPAUTHPERIOD         "EapAurhPeriod"
#define NVRAM_ENDUSERTIME           "EndUserTime"
#define NVRAM_ENDUSERMAXNUM         "endUserMaxNum"
/*VAP流控*/
#define NVRAM_FLOWSWITCH            "Flowswtich"
#define NVRAM_UPLOADMAXRATE         "Uploadflow"
#define NVRAM_DOWNLOADMAXRATE       "Downloadflow"

/*VLAN相关*/
#define NVRAM_VLANID                "VlanId"
#define NVRAM_VLANNAME              "VlanName"
#define NVRAM_VLANSSID              "VlanSsid"
#define NVRAM_VLANPORT              "VlanPort"
#define NVRAM_VLANPRRIO             "VlanPrio"

/*告警相关*/
#define NVRAM_ALARMCPULOADHIGH      "alarmCpuLoadHigh"
#define NVRAM_ALARMCPULOADLOW       "alarmCpuLoadLow"
#define NVRAM_ALARMMEMLOADHIGH      "alarmMemLoadHigh"
#define NVRAM_ALARMMEMLOADLOW       "alarmMemLoadLow"

/*trap相关*/
#define NVRAM_TRAPDSWTICH           "trap_switch"
#define NVRAM_TRAPDESIP             "trap_ip"
#define NVRAM_TRAPDESPORT           "trap_port"
#define NVRAM_TRAPCOMMUITY          "trap_community"

/*2015-10-22  ccs配置模板-版本号*/
#define NVRAM_CONFIGVERSION     "snmp_config_version"

#endif

/******************************* 常量定义 ********************************/
#define SNMP_TIMEOUT  5*60 /* 5分钟 */
#define AGENT_REFRESH_PERIOD_DFTVAL  ((UINT32)10)
#define WIFI_MAX_NUM 2
#define VAP_MAX_NUM  32
#define NVRAM_SIZE  32

/**
RFC1212中建议：如果允许增加或删除MIB表结构的行，列对象中应该包含一个
名为“xType”或“xStatus”对象，它的语法类型应该是枚举型的整数，
这个列对象称为状态列对象。
管理工作站可以通过设置状态列对象的值来控制表中行的创建与删除操作。

状态栏列对象有6个定义值：
-active（1），表明状态行是可用的
-notInService（2），表明行存在但不可用
-notReady（3），表明存在，但因为缺少必要的信息而不能用
-createAndGo（4），有管理者设置，表明希望创建一个概念行并设置该行的状态列对象为active
-createAndWait（5），有管理者设置，表明希望创建一个概念行，但不可用
-destroy（6），删除行

An existing conceptual row can be in any one of the three states, 'notReady', 'notInService', or 'active'. 
If the manager wishes to add a row in a single shot with values for all the columns, the status column should be given as 'createAndGo(4)'. 
After the creation of a row, its value is set to active(1). 
If a row has to be created with values for only some columns, the status column should be 'createAndWait(5)'. 
Also, this row with partially filled columns has the status 'notReady(3)'. 
The entry remains in this state until the values for all the columns are set. 
After all the values are set, the agent changes this value to active(1).

For SMIv2 tables, a new row can be added in any of the following ways:
CreateAndWait
CreateAndGo using Multiple-Variable Set

CreateAndWait
In this method, you should SET the RowStatus with value "CreateAndWait" and SET each column one by one. 
Finally, SET the RowStatus to "Active". 
Each SET method can be performed either using MibBrowser or through command line application. 

CreateAndGo Using Multiple-Variable Set
Instead of using multiple SET methods to create a new row, you can use Multiple-variable SET method to create a new row in a single SET method. 
For this, all the columns with their value should be given and rowStatus should be CreateAndGo.
Multiple-variable SET can be done using the command line application. 
Set all the column OIDs with the instance value (index for the row) using the method setObjectIDList() in SnmpTarget. 
Set the values for all the columns using snmpSetList() method. Rows can be deleted from a table by setting the value for the RowStatus column.

 */
#define ENUM_ROWSTATUS_ACTIVE          1
#define ENUM_ROWSTATUS_NOTINSERVICE    2
#define ENUM_ROWSTATUS_NOTREADY        3
#define ENUM_ROWSTATUS_CREATEANDGO     4
#define ENUM_ROWSTATUS_CREATEANDWAIT   5
#define ENUM_ROWSTATUS_DESTROY         6

/******************************* 表容量定义 ***********************************/
#define TABLE_MAXRECNUM_AP_WIFI         ((UINT32)2)
#define TABLE_MAXRECNUM_AP_VAP          ((UINT32)32)
#define TABLE_MAXRECNUM_TRAP_DES        ((UINT32)1)
#define TABLE_MAXRECNUM_SNMPV2          ((UINT32)4)
#define TABLE_MAXRECNUM_VLAN            ((UINT32)32)
#define TABLE_MAXRECNUM_STATION         ((UINT32)128)
#define TABLE_MAXRECNUM_INTERFACES      ((UINT32)1)
#define TABLE_MAXRECNUM_INTERF          ((UINT32)64)
#define TABLE_MAXRECNUM_WLANINTERF      ((UINT32)64)
#define TABLE_MAXRECNUM_ROGUEAP         ((UINT32)64)
#define TABLE_MAXRECNUM_IP              ((UINT32)2)
#define TABLE_MAXRECNUM_LEGALAP         ((UINT32)16)

/******************************* 表名称定义************************************/
#if 0
/* BEGIN: Added by zoucaihong, 2012/12/4 
add system group and interfaces group of RFC1213*/
/*the system group*/
#define TABLE_NAME_SYSTEM                       "system"
#define TABLE_NAME_INTERFACES                   "interfaces"
#define TABLE_NAME_INTERFACESTABLE              "interfacesTable"
/* END: Added by zoucaihong, 2012/12/4 */
#endif
/*配置表*/
#define TABLE_NAME_AP_CONFIG_SYS_DEVINFO        "sysDevInfo"
#define TABLE_NAME_AP_CONFIG_SYS_SWINFO         "sysSwInfo"
#define TABLE_NAME_AP_CONFIG_SYS_HWINFO         "sysHwInfo"
#define TABLE_NAME_AP_CONFIG_SYS_IP             "sysIpConfig"
#define TABLE_NAME_AP_CONFIG_SYS_IPTABLE        "sysIpConfigTable"
#if 0
#define TABLE_NAME_AP_CONFIG_SYS_LEGALAP        "sysLegalApConfig"
#define TABLE_NAME_AP_CONFIG_SYS_LEGALAP_TABLE  "sysLegalApConfigTable"
#define TABLE_NAME_AP_CONFIG_SYS_FLOWCTL        "sysFlowCtl"
/* BEGIN: Added by zoucaihong, 2013/3/11 */
#define TABLE_NAME_AP_CONFIG_ADVANCE_5GPRIOR    "advance5gPrior"
#define TABLE_NAME_AP_CONFIG_ADVANCE_11nPRIOR   "advance11nPrior"
/* END: Added by zoucaihong, 2013/3/11 */
#endif
#define TABLE_NAME_AP_CONFIG_WIFI               "wifiConfig"
#define TABLE_NAME_AP_CONFIG_WIFIBASIC          "wifiBasic"
#define TABLE_NAME_AP_CONFIG_WIFICHANNEL        "wifiChannel"
#define TABLE_NAME_AP_CONFIG_WIFIPOWER          "wifiPower"
#define TABLE_NAME_AP_CONFIG_WIFIADVANCE        "wifiAdvance"
/* BEGIN: Added by Zhou Shouya, 2013/8/29 */
#define TABLE_NAME_AP_CONFIG_WIFIRATE           "wifiRate"
/* END:   Added by Zhou Shouya, 2013/8/29 */
/* BEGIN: Added by zoucaihong, 2013/3/21 */
/* 2.4G卡配置和5G卡配置分开 */
#define TABLE_NAME_AP_CONFIG_WIFI5              "wifi5Config"
#define TABLE_NAME_AP_CONFIG_WIFIBASIC5         "wifiBasic5"
#define TABLE_NAME_AP_CONFIG_WIFICHANNEL5       "wifiChannel5"
#define TABLE_NAME_AP_CONFIG_WIFIPOWER5         "wifiPower5"
#define TABLE_NAME_AP_CONFIG_WIFIADVANCE5       "wifiAdvance5"
/* END: Added by zoucaihong, 2013/3/21 */
/* BEGIN: Added by Zhou Shouya, 2013/8/29 */
#define TABLE_NAME_AP_CONFIG_WIFIRATE5          "wifiRate5"
/* END:   Added by Zhou Shouya, 2013/8/29 */
#define TABLE_NAME_AP_CONFIG_VAP                "vapConfig"
#define TABLE_NAME_AP_CONFIG_VAPBASICTABLE      "vapBasicTable"
#define TABLE_NAME_AP_CONFIG_VAPSECURITYTABLE   "vapSecurityTable"
#define TABLE_NAME_AP_CONFIG_ALARM              "alarmConfig"
#define TABLE_NAME_AP_CONFIG_TRAPDES            "trapDestinationConfig"
#define TABLE_NAME_AP_CONFIG_TRAPDESTABLE       "trapDestinationTable"
#define TABLE_NAME_AP_CONFIG_VLAN               "vlanConfig"
#define TABLE_NAME_AP_CONFIG_VLANTABLE          "vlanConfigTable"
#define TABLE_NAME_AP_CONFIG_SSID               "ssidSetup"
#define TABLE_NAME_AP_CONFIG_SSIDTABLE          "ssidSetupTable"

/*统计表*/
#define TABLE_NAME_AP_STAT_SYS                  "sysStat"
#define TABLE_NAME_AP_STAT_WIFI                 "wifiStat"
#define TABLE_NAME_AP_STAT_WIFITABLE            "wifiStatTable"
#define TABLE_NAME_AP_STAT_WIFIVAP              "wifiVapStat"
#define TABLE_NAME_AP_STAT_WIFIVAPTABLE         "wifiVapStatTable"
#define TABLE_NAME_AP_STAT_STATION              "stationStat"
#define TABLE_NAME_AP_STAT_STATIONTABLE         "stationStatTable"
#if 0
#define TABLE_NAME_AP_STAT_24COCHAINTERFINFO            "24coChaInterfInfo"
#define TABLE_NAME_AP_STAT_24COCHAINTERFINFOTABLE       "24cochainterfinfoTable"
#define TABLE_NAME_AP_STAT_5COCHAINTERFINFO            "5coChaInterfInfo"
#define TABLE_NAME_AP_STAT_5COCHAINTERFINFOTABLE       "5cochainterfinfoTable"
#define TABLE_NAME_AP_STAT_24ADJCHAINTERFINFO           "24adjChaInterfInfo"
#define TABLE_NAME_AP_STAT_24ADJCHAINTERFINFOTABLE      "24adjChaInterfInfoTable"
#define TABLE_NAME_AP_STAT_5ADJCHAINTERFINFO           "5adjChaInterfInfo"
#define TABLE_NAME_AP_STAT_5ADJCHAINTERFINFOTABLE      "5adjChaInterfInfoTable"
#define TABLE_NAME_AP_STAT_24WLANDEVINTERFINFO          "24wlanDevInterfInfo"
#define TABLE_NAME_AP_STAT_24WLANDEVINTERFINFOTABLE     "24wlanDevInterfInfoTable"
#define TABLE_NAME_AP_STAT_5WLANDEVINTERFINFO          "5wlanDevInterfInfo"
#define TABLE_NAME_AP_STAT_5WLANDEVINTERFINFOTABLE     "5wlanDevInterfInfoTable"
#define TABLE_NAME_AP_STAT_NONWLANDEVINTERFINFO         "nonWlanDevInterfInfo"
#define TABLE_NAME_AP_STAT_NONWLANDEVINTERFINFOTABLE    "nonWlanDevInterfInfoTable"
#define TABLE_NAME_AP_STAT_BRMODESTA            "brModeSta"
#define TABLE_NAME_AP_STAT_BRMODESTATABLE       "brModeStaTable"
#define TABLE_NAME_AP_STAT_24ROGUEAPINFO        "24rogueApINfo"
#define TABLE_NAME_AP_STAT_24ROGUEAPINFOTABLE   "24rogueApInfoTable"
#define TABLE_NAME_AP_STAT_5ROGUEAPINFO         "5rogueApINfo"
#define TABLE_NAME_AP_STAT_5ROGUEAPINFOTABLE    "5rogueApInfoTable"
#endif
/* BEGIN: Added by zoucaihong, 2014/5/14 */
#define TABLE_NAME_AP_OPER_SYSOPER              "apSysOper"
#define TABLE_NAME_AP_OPER_FILETRANS            "apFileTransConfig"
/* END: Added by zoucaihong, 2014/5/14 */

/********************************** 结构定义 **********************************/
/* BEGIN: Added by zoucaihong, 2013/3/21 */
typedef BOOL  (*AGENTMETHOD)(VOID);
#define TABLEID_MAX_NUM 16

typedef struct
{
	CHAR acTblName[32];
	AGENTMETHOD md;
	UINT32 ulInitFlag;
}AGENT_TBLMD_MAP_T;
/* END: Added by zoucaihong, 2013/3/21 */
/* BEGIN: Added by zoucaihong, 2012/12/10 */
typedef struct
{
	BOOL   bInitApConfigData;
	BOOL   bInitAll;
}AGENT_INIT_STATE_T;
/* END: Added by zoucaihong, 2012/12/10 */

/* BEGIN: Added by zoucaihong, 2013/1/21 */
typedef struct
{
    CHAR acCoChaInterf[64];
    CHAR acAdjChaInterf[64];
    CHAR acWlanDevInterf[64];
    CHAR acNonWlanDevInterf[64];
}AGENT_ALARMINFO_T;
/* END: Added by zoucaihong, 2013/1/21 */
#if 0
/************************ RFC1213和RFC2863结构体定义 **************************/
/* BEGIN: Added by zoucaihong, 2012/12/4 
add system group and interfaces group of RFC1213*/
/*the system group*/
typedef struct
{
    CHAR acSysDescr[128];
    //UINT32 aulSysObjectID[128];
    CHAR acSysObjectID[128];
    UINT32 ulSysUpTime;
    CHAR acSysContact[64];
    CHAR acSysName[64];
    CHAR acSysLocation[64];
    UINT32 ulSysServices;
}SYSTEM_T;

/*the interfaces group*/
typedef struct
{
    UINT32 ulIfNumber;
}INTERFACES_T;

typedef struct
{
    UINT32 ulIfIndex;
    CHAR acIfDescr[32];
    UINT32 ulIfType;
    UINT32 ulIfMtu;
    UINT32 ulIfSpeed;
    UINT8 aucIfPhysAddress[6];
    UINT8 aucRsv[2];
    UINT32 ulIfAdminStatus;
    UINT32 ulIfOperStatus;
    UINT32 ulIfLastChange;
    UINT32 ulIfInOctets;
    UINT32 ulIfInUcastPkts;
    UINT32 ulIfInNUcastPkts;
    UINT32 ulIfInDiscards;
    UINT32 ulIfInErrors;
    UINT32 ulIfInUnknownProtos;
    UINT32 ulIfOutOctets;
    UINT32 ulIfOutUcastPkts;
    UINT32 ulIfOutNUcastPkts;
    UINT32 ulIfOutDiscards;
    UINT32 ulIfOutErrors;
    UINT32 ulIfOutQLen;
    //UINT32 aulIfSpecific[128];
    CHAR acIfSpecific[32];
    UINT32 ulIfLastTime;
}INTERFACES_ENTRY_T;
/* END: Added by zoucaihong, 2012/12/4 */
#endif
/***************************** 私有mib结构体定义 ******************************/
/******************************* 配置结构体定义 *******************************/
/*
 * apDevInfo
 */
typedef struct 
{
	UINT8   aucSysMacAddr[6];   /*有线口MAC 地址*/
	UINT8   aucResv[2];	
    /* BEGIN: Added by zoucaihong, 2012/11/5 */
	UINT32  ulSysUpTime;
    /* END: Added by zoucaihong, 2012/11/5 */
    /* BEGIN: Added by zoucaihong, 2012/12/25 */
    UINT32  ulRegetPeriod;       /*数据刷新周期*/
    /* END: Added by zoucaihong, 2012/12/25 */
    /* BEGIN: Added by zoucaihong, 2013/1/22 */
    UINT32  ulIfSpeed;           /*上鏈路/下鏈路最大訊務流量*/
    /* END: Added by zoucaihong, 2013/1/22 */
    /* BEGIN: Added by zoucaihong, 2013/4/24 */
    UINT32  ulTrapSwitch;       /*trap开关*/
    /* END: Added by zoucaihong, 2013/4/24 */
    /* BEGIN: Added by Zhou Shouya, 2013/8/9   PN:VLAN */
    UINT32  ulSysRestart;       /* 重启AP */
    /* END:   Added by Zhou Shouya, 2013/8/9   PN:VLAN */
    /* BEGIN: Added by zoucaihong, 2014/5/13 */
    UINT32  ulTimeZone;         /* 时区 */
    CHAR    acTime[32];         /* 时间 */
    UINT8   aucIpAddress[4];    /* 设备主IP */
    UINT32  ulSysManageState;   /* 设备管理状态，offline(0), online(1) */
    UINT32  ulSysModel;         /* 设备型号ID */
    CHAR    acInterModel[32];   /* 内部设备型号名称 */
    CHAR    acSysModelName[32]; /* 设备型号名称 */
    /* END: Added by zoucaihong, 2014/5/13 */
    UINT32  ulTimeOutInterval;  /* 报文超时时间，单位:second */
    UINT32  ulHeartBeatPeriod;  /* 心跳周期，单位:second */
    /* BEGIN: Added by zoucaihong, 2014/9/23 */
    UINT32  ulDevType;          /* 设备形态，1(网关),2(AP) */
    CHAR    acSysName[32];      /* 设备名称 */
    CHAR    acHotId[32];        /* 热点ID */
    /* END: Added by zoucaihong, 2014/9/23 */
}AP_CONFIG_SYS_DEVINFO_T;

/* BEGIN: Added by zoucaihong, 2013/2/25 */
/*
 * apIpConfig
 */
typedef struct
{
    UINT32  ulNum;
}AP_CONFIG_IP_NUM_T;

typedef struct
{
    UINT32  ulIndex;
    /* BEGIN: Added by zoucaihong, 2014/6/3 */
    UINT32  ulIpMode;           /* IP模式:static(0), dhcp(1) */
    /* END: Added by zoucaihong, 2014/6/3 */
    UINT8   aucIpAddress[4];    /* IP 地址 */
    UINT8   aucIpNetMask[4];    /* 子网掩码 */
    /* BEGIN: Modified by zoucaihong, 2014/6/3 */
    UINT8   aucGwAddress[4];    /* 网关地址 */
    UINT8   aucDnsMaster[4];    /* 主用DNS服务器 */
    UINT8   aucDnsSlave[4];     /* 备用DNS服务器 */
    /* END: Modified by zoucaihong, 2014/6/3 */
}AP_CONFIG_IP_ENTRY_T;
/* END: Added by zoucaihong, 2013/2/25 */

#if 0
/* BEGIN: Added by zoucaihong, 2013/5/13 */
/*合法AP名单配置*/
typedef struct
{
    UINT32  ulNum;
}AP_CONFIG_LEGALAP_NUM_T;

typedef struct
{
    UINT32  ulIndex;
    CHAR    acLegalApSsid[32];      /*合法AP SSID*/
    UINT8   aucLegalApBssid[6];     /*合法AP BSSID*/
    UINT8   aucResv[2];
}AP_CONFIG_LEGALAP_ENTRY_T;

/*AP级流控配置*/
typedef struct
{
    UINT32  ulFlowSwitch;           /*AP流控开关*/
    UINT32  ulApUplinkMaxRate;      /*AP上行最大流量*/
    UINT32  ulApDwlinkMaxRate;      /*AP下线最大流量*/
    /* BEGIN: Added by Zhou Shouya, 2013/8/13   PN:VLAN */
    UINT32  ulStaUplinkMaxRate;     /*STA上行最大流量*/
    UINT32  ulStaDwlinkMaxRate;     /*STA下线最大流量*/
    /* END:   Added by Zhou Shouya, 2013/8/13   PN:VLAN */
}AP_CONFIG_FLOWCTL_T;
/* END: Added by zoucaihong, 2013/5/13 */

/* BEGIN: Added by zoucaihong, 2013/3/11 */
/*
 * ap5gPriorConfig
 */
typedef struct 
{
    UINT32  ul5GPriSwitch;  /* 5G优先开关 */
    UINT32  ul2dot4GDelayTime;  /* 2.4G延迟时间,单位:秒 */
    UINT32  ulStaTimesOut;    /* 终端超时时间,单位:秒 */
}AP_CONFIG_ADVANCE_5GPRIOR_T;

/*
 * ap11nPriorConfig
 */
typedef struct 
{
    UINT32  ul11nPriSwitch;  /* 11n优先开关 */
    UINT32  ul11bgDelayTime;  /* 11bg延迟时间,单位:秒 */
    UINT32  ulStaTimesOut;    /* 终端超时时间,单位:秒 */
}AP_CONFIG_ADVANCE_11NPRIOR_T;
/* END: Added by zoucaihong, 2013/3/11 */
#endif

/*
 * IfConfig
 */
typedef struct 
{
    UINT32      ulIfNum;
}CPE_CONFIG_IF_T;

typedef struct 
{
    UINT32  ulIfId;
    CHAR    acIfDesc[64];
    UINT32  ulIfType;
    UINT32  ulIfMtu;
    UINT32  ulIfSpeed;
    UINT8   aucIfPhysAddress[6];
    UINT8   aucResv[2];
    UINT32  ulIfAdminStatus;
    UINT32  ulIfOperStatus;
    UINT32  ulIfLastChange;

	UINT32  ulRowStatus;
}CPE_CONFIG_IF_ENTRY_T;

/* BEGIN: Added by zoucaihong, 2012/11/5 */
typedef struct 
{
    CHAR    acSoftwareVersion[32];
    CHAR    acTargetVersion[32];
    CHAR    acOemVersion[32];
}AP_CONFIG_SYS_SWINFO_T;

typedef struct 
{
    CHAR    acCpuType[128];
    CHAR    acMemType[128];
    UINT32  ulMemSize;
    UINT32  ulDiskSize;
	CHAR    acHardwareVersion[32];
    /* BEGIN: Added by zoucaihong, 2014/5/15 */
    UINT32  ulHardwareType;         /* 硬件类型 */
    UINT32  ulBoardVersion;         
    /* END: Added by zoucaihong, 2014/5/15 */
}AP_CONFIG_SYS_HWINFO_T;

typedef struct
{
	UINT32  ulWifiId;
    /* BEGIN: Added by zoucaihong, 2012/11/1 */
    UINT8   aucMacAddress[6];
    UINT8   aucResv[2];
    /* END: Added by zoucaihong, 2012/11/1 */
	UINT32  ulWifiDeviceType;				/* 设备类型 :
    ap-single(0),ap-ms(1),adhoc-single(2),adhoc-ms(3),ap-adhoc(4),sta-adhoc(5),sta-single(6),sta-ms(7)*/
}AP_CONFIG_WIFIDEVICEBASIC_T;

typedef struct
{
	UINT32  ulWifiId;
	UINT32  ulAbgMode;					/* A/B/G模式 */
	UINT32  ulChannelUsing;				/* 当前使用信道 */
	UINT32  ulChannelState;				/* 工作信道模式:0:static; 1:dynamic */
    UINT32  ulAutoChanTime;             /* 动态信道扫描时间 */
    UINT32  ulStaicChannel;             /* 静态信道 */
	CHAR    acAutoChanList[256];        /* 动态信道列表 */
    /* BEGIN: Added by zoucaihong, 2012/11/1 */
	UINT32  ulRFNum;    /* 空间流数量 */
	UINT32  ulBwMode;    /* 0:off; 1:on */
    /* END: Added by zoucaihong, 2012/11/1 */
}AP_CONFIG_WIFIDEVICECHANNEL_T;

typedef struct
{
	UINT32  ulWifiId;
	UINT32	ulLimitTxPower;	/* 最大发射功率 */
	UINT32	ulMaxTxPower;	/* 衰减值  */

    /* BEGIN: Added by zoucaihong, 2012/11/1 */
    UINT32  ulTransmitPower;/* 当前发射功率 */
    /* END: Added by zoucaihong, 2012/11/1 */
}AP_CONFIG_WIFIDEVICEPOWER_T;

/* BEGIN: Added by zoucaihong, 2013/1/21 */
typedef struct
{
	UINT32  ulWifiId;
	UINT32  ulRssiMode;
	INT32   iRssi;
	UINT32  ulBeaconInterval;
 }AP_CONFIG_WIFIDEVICEADVANCE_T;
/* END: Added by zoucaihong, 2013/1/21 */

/* BEGIN: Added by Zhou Shouya, 2013/8/29   PN:VLAN */
typedef struct
{
    UINT32  ulWifiId;
    /* BEGIN: Added by Zhou Shouya, 2013/9/17 */
    UINT32  ulRateMode;     /* 速率模式 */
    /* END:   Added by Zhou Shouya, 2013/9/17 */
    CHAR    acTransmitSpeedBasic[256];
    CHAR    acTransmitSpeed11n[256];
}AP_CONFIG_WIFIDEVICERATE_T;
/* END:   Added by Zhou Shouya, 2013/8/29   PN:VLAN */

typedef struct
{
    UINT32   ulVapNum;
}AP_CONFIG_VAP_T;

typedef struct 
{
    UINT32  ulIndex;
    UINT32  ulNvramId;
    UINT32  ulVapId;
    UINT32  ulWifiId;
    CHAR    acSsid[32];/*SSID名称*/
    UINT32  ulSsidEnabled;/*SSID 是否启用*/
    UINT32  ulSsidBroadcast;/*SSID是否广播*/
    UINT32  ulVlanId;/* VLAN ID*/
    /* BEGIN: Added by zoucaihong, 2012/11/1 */
    UINT32  ulVlanEnabled;/* VLAN 是否启用*/
    /* END: Added by zoucaihong, 2012/11/1 */
    UINT8   aucBssid[6];
    UINT8   aucResv[2];
    UINT32  ulMaxSimultUsers;
    UINT32  ulFlowSwitch;
    UINT32  ulUplinkMaxRate;    /*VAP级上行最大带宽(Kbps)*/
    UINT32  ulDownlinkMaxRate;  /*VAP级下行最大带宽(Kbps)*/
    /* BEGIN: Added by zoucaihong, 2013/4/23 */
    UINT32  ulUserUpload;       /*用户上行最大带宽(Kbps)*/
    UINT32  ulUserDownload;     /*用户下行最大带宽(Kbps)*/
    /* END: Added by zoucaihong, 2013/4/23 */
    UINT32  ulEndUserTime;

    UINT32  ulRowStatus;
}AP_CONFIG_VAPBASIC_ENTRY_T;

typedef struct
{		
	UINT32  ulIndex;
	UINT32  ulVapId;
    UINT32  ulWifiId;
    CHAR    acSsid[32];/*SSID名称*/
    
	UINT32  ulAuthenticationType; /* 0: disabled, 1:wep */
	UINT32  ulSecurityMode;     /*加密类型:1:open;
	                                    2: shared
                                        3:wpa-psk
                                        4:wpa2-psk
                                        5:psk-mixed
                                        6:wpa
                                        7:wpa2
                                        8:wpa-wpa2-mixed
                                */
	UINT32  ulEncryptType; /*加密机制:0:TKIP; 1:AES*/      
	CHAR    acWpaKey[64];   
	UINT32  ulWpaKeyRenewal;
		      
	UINT32  ulWepKeyFormat; 
	UINT32  ulWepBit;
	CHAR    acWepKey[32];

    UINT32  ulRadiusPort;
    UINT8   aucRadiusAddr[4];
    CHAR    acRadiusSecu[64];
    UINT32  ulAccountPort;
    UINT8   aucAccountAddr[4];
    CHAR    acAccountSecu[64];
    UINT32  ulAccountInterval;
    /* BEGIN: Added by zoucaihong, 2013/3/11 */
    UINT32  ulRadiusAgingTime;
    /* END: Added by zoucaihong, 2013/3/11 */
    UINT32  ulRowStatus;
}AP_CONFIG_VAPSECURITY_ENTRY_T;

typedef struct 
{
    UINT32   ulCpuLoadHigh	;	/* CPU负荷告警产生门限	90      */
    UINT32   ulCpuLoadLow	;	/* CPU负荷告警消除门限	80      */

    UINT32   ulMemLoadHigh	;	/* 内存利用率告警产生门限	85  */
    UINT32   ulMemLoadLow	;	/* 内存利用率告警消除门限	80  */

}AP_CONFIG_ALARM_T;
/* END: Added by zoucaihong, 2012/11/5 */

/* BEGIN: Added by zoucaihong, 2012/11/15 */
/*
 * apTrapDesConfig
 */
typedef struct  
{
	UINT32  ulTrapDesNum;
}AP_CONFIG_TRAPDES_T;

typedef struct  
{
	UINT32  ulSwtich;
	UINT8   aucTrapDesIPAddress[4];
	UINT32  ulTrapDesPort;
	CHAR    acTrapCommunity[32];
}AP_CONFIG_TRAPDES_ENTRY_T;
/* END: Added by zoucaihong, 2012/11/15 */

/* BEGIN: Added by zoucaihong, 2012/11/21 */
/*
* apVlanSetup
*/
typedef struct
{
    UINT32 ulVlanNum;
}AP_CONFIG_VLAN_T;

typedef struct
{
    UINT32 ulIndex;
    UINT32 ulVlanId;
    CHAR   acVlanName[32];
    UINT32 ulNvramId;
    UINT32 ulVlanPrio;/*VLAN优先级*/
    UINT32 ulRowStatus;
}AP_CONFIG_VLAN_ENTRY_T;
/* END: Added by zoucaihong, 2012/11/21 */
/* BEGIN: Added by zoucaihong, 2013/3/1 */
typedef struct
{
    UINT32  ulNum;
}AP_CONFIG_SSID_T;
typedef struct
{
    UINT32  ulIndex;
    UINT32  ulWifiId;
    UINT32  ulVapId;
    CHAR    acSsidName[32];
    UINT32  ulRowStatus;
}AP_CONFIG_SSID_ENTRY_T;
/* END: Added by zoucaihong, 2013/3/1 */
/******************************* 统计结构体定义********************************/
typedef struct 
{
    UINT32       ulCPURTUsage;
    UINT32       ulCPUPeakUsage;
    UINT32       ulCPUAvgUsage;
    UINT32       ulMemRTUsage;
    UINT32       ulMemPeakUsage;
    UINT32       ulMemAvgUsage;
}AP_STAT_SYS_T;

typedef struct 
{
    UINT32      ulIfNum;
}CPE_STAT_IF_T;


typedef struct 
{
	UINT32      ulIfID;
	
	UINT64      ulIfInUnicastPkts;
	UINT32      ulIfInBroadcastPkts;
	UINT32      ulIfInMulticastPkts;	
	UINT64      ulIfInNUcastPkts;   /* 端口接收非单播包数 */
	UINT64      ulIfInBytes;
	UINT32      ulIfInDiscardPkts;
	UINT32      ulIfInErrPkts;
	
	UINT64      ulIfOutUnicastPkts;
	UINT32      ulIfOutBroadcastPkts;
	UINT32      ulIfOutMulticastPkts;
	UINT64      ulIfOutNUcastPkts;  /* 端口发送非单播包数 */
	UINT64      ulIfOutBytes;
	UINT32      ulIfOutDiscardPkts;
	UINT32      ulIfOutErrPkts;
	
	UINT32      ulIfUpDwnTimes;
}CPE_STAT_IF_ENTRY_T;

/* BEGIN: Added by zoucaihong, 2012/11/5 */
/*
 * apWlanWifiVapStat
 */
typedef struct  
{
	UINT32  ulVapNum;
}AP_STAT_WIFIVAP_T;

typedef struct  
{
	UINT32  ulIndex;
	UINT32  ulVapId;
    UINT32  ulWifiId;
	CHAR   acVapSsid[32];
	UINT64  ullRecvBytes;
	UINT64  ullSendBytes;
    UINT32  ulRecvFrameNum;
    UINT32  ulSendFrameNum;
    UINT32  ulRecvErrFrame;
    UINT32  ulSendErrFrame;
    UINT32  ulErrFrameRate;
    /* BEGIN: Added by zoucaihong, 2012/11/1 */
    UINT32  ulAssoStaNum;/*实际联机用户数*/
    /* END: Added by zoucaihong, 2012/11/1 */
}AP_STAT_WIFIVAP_ENTRY_T;

/*
 * apWlanWifiStat
 */
typedef struct  
{
	UINT32  ulWifiNum;
    /* BEGIN: Added by zoucaihong, 2013/5/10 */
	UINT64  ullApRecvBytes;
	UINT64  ullApSendBytes;
	UINT64  ullWifi24RecvBytes;
	UINT64  ullWifi24SendBytes;
	UINT64  ullWifi5RecvBytes;
	UINT64  ullWifi5SendBytes;
    /* END: Added by zoucaihong, 2013/5/10 */
}AP_STAT_WIFI_T;

typedef struct  
{
	UINT32  ulIndex;
	UINT32  ulWifiId;
	UINT64  ullRecvBytes;
	UINT64  ullSendBytes;
    UINT32  ulRecvFrameNum;
    UINT32  ulSendFrameNum;
    UINT32  ulRecvErrFrame;
    UINT32  ulSendErrFrame;
    UINT32  ulErrFrameRate;
    /* BEGIN: Added by zoucaihong, 2012/11/1 */
    UINT32  ulAssoStaNum;/*实际联机用户数*/
    /* END: Added by zoucaihong, 2012/11/1 */
}AP_STAT_WIFI_ENTRY_T;
/* END: Added by zoucaihong, 2012/11/5 */

/* BEGIN: Added by zoucaihong, 2012/11/22 */
typedef struct
{
    UINT32 ulStationNum;
}AP_STAT_STATION_T;

typedef struct 
{
    UINT32 ulStaID;
    UINT8  aucBssid[6];         /*AP设备SSID MAC address*/
    UINT8  aucResv[2];
    UINT8  aucSsid[32];         /*SSID名称*/
    UINT32 ulVlanId;            /*VLAN ID*/
    /* BEGIN: Added by zoucaihong, 2013/4/18 */
    UINT32 ulVlanEnable;        /*VLAN是否启用*/
    /* END: Added by zoucaihong, 2013/4/18 */
	UINT8  aucStaMac[6];        /*连线用戶终端MAC address*/
    UINT8  aucResv2[2];
    /* BEGIN: Added by zoucaihong, 2014/6/18 */
    UINT8  aucStaIp[4];        /* 终端IP */
    /* END: Added by zoucaihong, 2014/6/18 */
    UINT8  aucStaDevType[64];   /*连线用戶终端设备类型*/
    UINT32 ulConnectTime;       /*连线用戶终端Connection Time*/
    UINT64 ulStaUplinkOctets;   /*连线用戶终端上行讯务流量*/
    UINT64 ulStaDownlinkOctets; /*连线用戶终端下行讯务流量*/
	INT32  iRssi;               /*AP端接收之上链路用戶终端RSSI(Received Signal Strength Indication)，单位须为dBm*/
	UINT8  aucFrequency[32];    /*使用的頻段(2.4GHz或5GHz)*/
    UINT32 ulSnr;               /*AP端接收之上鏈路用戶終端SNR(Signal-to-Noise Ratio)，單位須為dB*/
    UINT32 ulTxRate;            /*连线用戶终端实体层传输速率(Transmit Rate)*/
    CHAR   acTxRate[32];
    UINT32 ulErrRate;           /*连线用戶终端封包错误率(Packet Error Ratio)*/
    /* BEGIN: Added by Zhou Shouya, 2013/9/6 */
    UINT32 uAssoRate;           /* 终端协商速率 */
    /* END:   Added by Zhou Shouya, 2013/9/6 */
}AP_STAT_STATION_ENTRY_T;
/* END: Added by zoucaihong, 2012/11/22 */
typedef struct
{
    UINT32 ulWifiId;
    UINT32 ulVapId;
    UINT32 ulNvramId;
}SSID_INFO_T;
typedef struct
{
    UINT32 ulWifiNum;
    UINT32 ulSsidNum;
    SSID_INFO_T astSsidInfo[VAP_MAX_NUM];
}SSID_T;

typedef struct
{
    UINT32 ulTrapDesId;
}TRAPDES_INFO_T;
typedef struct
{
    UINT32 ulTrapDesNum;
    TRAPDES_INFO_T astTrapDesInfo[TABLE_MAXRECNUM_TRAP_DES];
}TRAPDES_T;

/* BEGIN: Added by zoucaihong, 2012/11/23 */
typedef struct
{
    UINT32 ulStaNum;
    AP_STAT_STATION_ENTRY_T astStaInfo[64];
}STATION_T;
/* END: Added by zoucaihong, 2012/11/23 */

#if 0
/* BEGIN: Added by zoucaihong, 2013/1/21 */
/*
* 2.4G同频干扰(Co-Channel Interference，CCI)
*/
typedef struct
{
    UINT32  ulNum;
}AP_STAT_24COCHAINTERFINFO_T;
typedef struct
{
    UINT32  ulIndex;
    UINT8   aucBssid[6];
    CHAR    acSsid[32];
    INT32   iRssi;
    UINT32  ulChannel;
}AP_STAT_24COCHAINTERFINFO_ENTRY_T;

/*
* 5G同频干扰(Co-Channel Interference，CCI)
*/
typedef struct
{
    UINT32  ulNum;
}AP_STAT_5COCHAINTERFINFO_T;
typedef struct
{
    UINT32  ulIndex;
    UINT8   aucBssid[6];
    CHAR    acSsid[32];
    INT32   iRssi;
    UINT32  ulChannel;
}AP_STAT_5COCHAINTERFINFO_ENTRY_T;

/*
* 2.4G邻频干扰(Adjacent Channel Interference，ACI)
*/
typedef struct
{
    UINT32  ulNum;
}AP_STAT_24ADJCHAINTERFINFO_T;
typedef struct
{
    UINT32  ulIndex;
    UINT8   aucBssid[6];
    CHAR    acSsid[32];
    INT32   iRssi;
    UINT32  ulChannel;
}AP_STAT_24ADJCHAINTERFINFO_ENTRY_T;

/*
* 5G邻频干扰(Adjacent Channel Interference，ACI)
*/
typedef struct
{
    UINT32  ulNum;
}AP_STAT_5ADJCHAINTERFINFO_T;
typedef struct
{
    UINT32  ulIndex;
    UINT8   aucBssid[6];
    CHAR    acSsid[32];
    INT32   iRssi;
    UINT32  ulChannel;
}AP_STAT_5ADJCHAINTERFINFO_ENTRY_T;

/*
* 2.4G WLAN 设备干扰(WLAN Devices Interference)
*/
typedef struct
{
    UINT32  ulNum;
}AP_STAT_24WLANDEVINTERFINFO_T;
typedef struct
{
    UINT32  ulIndex;
    UINT8   aucBssid[6];
    CHAR    acSsid[32];
    INT32   iRssi;
    UINT32  ulChannel;
}AP_STAT_24WLANDEVINTERFINFO_ENTRY_T;

/*
* 5G WLAN 设备干扰(WLAN Devices Interference)
*/
typedef struct
{
    UINT32  ulNum;
}AP_STAT_5WLANDEVINTERFINFO_T;
typedef struct
{
    UINT32  ulIndex;
    UINT8   aucBssid[6];
    CHAR    acSsid[32];
    INT32   iRssi;
    UINT32  ulChannel;
}AP_STAT_5WLANDEVINTERFINFO_ENTRY_T;
#endif

/* BEGIN: Added by zoucaihong, 2013/5/10 */
#define LEG_AP_NUMS 16
struct leg_ap{
    char bssid[32];
    char ssid[32];
    char id[32];
};

#if 0
/*
* 2.4G 非法AP列表
*/
typedef struct
{
    UINT32  ulNum;
}AP_STAT_24ROGUEAPINFO_T;
typedef struct
{
    UINT32  ulIndex;
    UINT8   aucBssid[6];
    CHAR    acSsid[32];
    INT32   iRssi;
    UINT32  ulChannel;
}AP_STAT_24ROGUEAPINFO_ENTRY_T;

/*
* 5G 非法AP列表
*/
typedef struct
{
    UINT32  ulNum;
}AP_STAT_5ROGUEAPINFO_T;
typedef struct
{
    UINT32  ulIndex;
    UINT8   aucBssid[6];
    CHAR    acSsid[32];
    INT32   iRssi;
    UINT32  ulChannel;
}AP_STAT_5ROGUEAPINFO_ENTRY_T;
/* END: Added by zoucaihong, 2013/5/10 */

/*
* 非WLAN 设备干扰(Non WLAN Devices Interference)
*/
typedef struct
{
    UINT32  ulNum;
}AP_STAT_NONWLANDEVINTERFINFO_T;
typedef struct
{
    UINT32 ulIndex;
    UINT32 ulWifiId;
    INT32  iNoise;/*当前信道底噪*/
    UINT32 ulChannel;/*信道*/
    CHAR   acTime[32];
}AP_STAT_NONWLANDEVINTERFINFO_ENTRY_T;
/*
* Bridge Mode时终端信息统计
*/
typedef struct
{
    UINT32 ulStationNum;
}AP_STAT_BRMODESTA_T;

typedef struct 
{
    UINT32 ulStaID;
    UINT32 ulMode;/*点对点或点对多点模式*/
	UINT8  aucStaMac[6];/*对接设备MAC*/
    UINT8  aucResv2[2];
	INT32  iRssi;/*对接设备RSSI(Received Signal Strength Indication)，单位须为dBm*/
    UINT32 ulSnr;/*对接设备SNR(Signal-to-Noise Ratio)，單位須為dB*/
    UINT32 ulTxRate;/*对接设备实体层传输速率(Transmit Rate)*/
    CHAR   acTxRate[32];
}AP_STAT_BRMODESTA_ENTRY_T;
#endif

/* END: Added by zoucaihong, 2013/1/21 */
/* BEGIN: Added by Zhou Shouya, 2013/8/29 */
typedef struct
{
    CHAR   acRateName[32];
    UINT32 ulIndex;
}RATE_DEF_T;
/* END:   Added by Zhou Shouya, 2013/8/29 */

/* BEGIN: Added by zoucaihong, 2014/5/14 */
/*
* 设备系统操作
*/
typedef struct
{
    UINT32  ulSoftwareUpdate;   /* 设备软件升级 */
    UINT32  ulSysRestart;       /* 设备重启 */
    UINT32  ulSysReset;         /* 设备恢复出厂配置 */
    UINT32  ulCfgFileImpExp;    /* 配置文件导入导出,normal(0),import(1),export(2) */
    /* BEGIN: Added by zoucaihong, 2014/7/15 */
    UINT32  ulDiagnose;         /* 一键收集，normal(0),diagnose(1) */
    /* END: Added by zoucaihong, 2014/7/15 */
    
    UINT32  ulStopRecover;          /* */
    CHAR    acOfflineConfigFile[256];   /*CCSV3下发配置模板==  版本号*/
}AP_OPER_SYSOPER_T;

/*
* 文件传输配置
*/
typedef struct{
    UINT32      ulLoadFlag;                 /*1-download,2-upload*/
    CHAR        acFileName[128];            /*版本文件路径*/
    UINT32      ulFileType;                 /*文件类型software(1),config(2),log(3),cert(4),perf(5)*/
    UINT32      ulTransProto;               /*传输协议类型*/
    UINT8       aucServerIp[4];             /*远程文件所在地址*/
    UINT32      ulTransPort;                /*传输的端口号*/
    CHAR        aucTransUser[32];           /*用户名*/
    CHAR        aucTransPasswd[32];         /*密码*/
    UINT32      ulTransStatus;              /*传输状态 0:ok, 1:param invalid, 2:download fail, 3:updatefail */
    CHAR        acFailReason[128]; 
	CHAR        acFileVersion[32];//软件版本号
}AP_OPER_FILETRANS_T;
/* END: Added by zoucaihong, 2014/5/14 */
#define BIND_AC_NUM_MAX     8
#define AC_URL_LEN_MAX      128
typedef struct
{
    /* DHCP OPTION43发现方式时, 绑定的AC IP列表*/
	UINT32	uiBindAcNum;
	UINT8	aucBindAcIp[BIND_AC_NUM_MAX][4];

	/* DNS发现方式时, 绑定的AC URL */
	CHAR	acAcURL[AC_URL_LEN_MAX];		/* AC URL */
	UINT32	uiResAcNum;		/* DNS解析获取到的AC IP数量*/
	UINT8	aucResAcIp[BIND_AC_NUM_MAX][4];	/* DNS解析后获取到的AC IP */
}AC_IP_LIST_T;

/********************************* 数据存储 **********************************/
typedef struct
{
    #if 0
    /* BEGIN: Added by zoucaihong, 2012/12/4 
    add system group and interfaces group of RFC1213*/
    SYSTEM_T                                    *pstSystem;
    INTERFACES_T                                stInterfaces;
    INTERFACES_ENTRY_T                          *pstInterfaces;
    /* END: Added by zoucaihong, 2012/12/4 */
    #endif

	AP_CONFIG_SYS_DEVINFO_T                     *pstSysDevInfo;
    AP_CONFIG_SYS_SWINFO_T                      *pstSysSwInfo;
    AP_CONFIG_SYS_HWINFO_T                      *pstSysHwInfo;
    AP_CONFIG_IP_NUM_T                          stSysIpConfig;
    AP_CONFIG_IP_ENTRY_T                        *pstSysIpConfig;
    #if 0
    AP_CONFIG_LEGALAP_NUM_T                     stSysLegalApConfig;
    AP_CONFIG_LEGALAP_ENTRY_T                   *pstSysLegalApConfig;
    AP_CONFIG_FLOWCTL_T                         *pstSysFlowCtl;
    
    /* BEGIN: Added by zoucaihong, 2013/3/11 */
    AP_CONFIG_ADVANCE_5GPRIOR_T                 *pstAdvance5GPrior;
    AP_CONFIG_ADVANCE_11NPRIOR_T                *pstAdvance11nPrior;
    /* END: Added by zoucaihong, 2013/3/11 */
    #endif
    AP_CONFIG_WIFIDEVICEBASIC_T                 *pstApWifiBasicConfig;
    AP_CONFIG_WIFIDEVICECHANNEL_T               *pstApWifiChannelConfig;
    AP_CONFIG_WIFIDEVICEPOWER_T                 *pstApWifiPowerConfig;
    AP_CONFIG_WIFIDEVICEADVANCE_T               *pstApWifiAdvanceConfig;
    /* BEGIN: Added by Zhou Shouya, 2013/8/29 */
    AP_CONFIG_WIFIDEVICERATE_T                  *pstApWifiRateConfig;
    /* END:   Added by Zhou Shouya, 2013/8/29 */
    /* BEGIN: Added by zoucaihong, 2013/3/21 */
    /* 2.4G卡配置和5G卡配置分开 */
    AP_CONFIG_WIFIDEVICEBASIC_T                 *pstApWifiBasic5Config;
    AP_CONFIG_WIFIDEVICECHANNEL_T               *pstApWifiChannel5Config;
    AP_CONFIG_WIFIDEVICEPOWER_T                 *pstApWifiPower5Config;
    AP_CONFIG_WIFIDEVICEADVANCE_T               *pstApWifiAdvance5Config;
    /* END: Added by zoucaihong, 2013/3/21 */
    /* BEGIN: Added by Zhou Shouya, 2013/8/29 */
    AP_CONFIG_WIFIDEVICERATE_T                  *pstApWifiRate5Config;
    /* END:   Added by Zhou Shouya, 2013/8/29 */
    
    AP_CONFIG_VAP_T                             stApVapConfig;
    AP_CONFIG_VAPBASIC_ENTRY_T                  *pstApVapBasicConfig;
    AP_CONFIG_VAPSECURITY_ENTRY_T               *pstApVapSecurityConfig;
    AP_CONFIG_ALARM_T                           *pstApAlarmConfig;
	AP_CONFIG_TRAPDES_T                         stApTrapDesConfig;
	AP_CONFIG_TRAPDES_ENTRY_T                   *pstApTrapDesConfig;
    AP_CONFIG_VLAN_T                            stApVlanConfig;
    AP_CONFIG_VLAN_ENTRY_T                      *pstApVlanConfig;
    AP_CONFIG_SSID_T                            stApSsidConfig;
    AP_CONFIG_SSID_ENTRY_T                      *pstApSsidConfig;

	AP_STAT_SYS_T                               *pstSysStat;
	AP_STAT_WIFI_T                              stApWifiStat;
	AP_STAT_WIFI_ENTRY_T                        *pstApWifiStat;
    AP_STAT_WIFIVAP_T                           stApWifiVapStat;
    AP_STAT_WIFIVAP_ENTRY_T                     *pstApWifiVapStat;
    AP_STAT_STATION_T                           stApStationStat;
    AP_STAT_STATION_ENTRY_T                     *pstApStationStat;
#if 0
    AP_STAT_BRMODESTA_T                         stApBrModeSta;
    AP_STAT_BRMODESTA_ENTRY_T                   *pstApBrModeSta;

    AP_STAT_24COCHAINTERFINFO_T                 stAp24CoChaInterfInfo;
    AP_STAT_24COCHAINTERFINFO_ENTRY_T           *pstAp24CoChaInterfInfo;
    AP_STAT_5COCHAINTERFINFO_T                  stAp5CoChaInterfInfo;
    AP_STAT_5COCHAINTERFINFO_ENTRY_T            *pstAp5CoChaInterfInfo;
    AP_STAT_24ADJCHAINTERFINFO_T                stAp24AdjChaInterfInfo;
    AP_STAT_24ADJCHAINTERFINFO_ENTRY_T          *pstAp24AdjChaInterfInfo;
    AP_STAT_5ADJCHAINTERFINFO_T                 stAp5AdjChaInterfInfo;
    AP_STAT_5ADJCHAINTERFINFO_ENTRY_T           *pstAp5AdjChaInterfInfo;
    AP_STAT_24WLANDEVINTERFINFO_T               stAp24WlanDevInterfInfo;
    AP_STAT_24WLANDEVINTERFINFO_ENTRY_T         *pstAp24WlanDevInterfInfo;
    AP_STAT_5WLANDEVINTERFINFO_T                stAp5WlanDevInterfInfo;
    AP_STAT_5WLANDEVINTERFINFO_ENTRY_T          *pstAp5WlanDevInterfInfo;
    AP_STAT_24ROGUEAPINFO_T                     stAp24RogueApInfo;
    AP_STAT_24ROGUEAPINFO_ENTRY_T               *pstAp24RogueApInfo;
    AP_STAT_5ROGUEAPINFO_T                      stAp5RogueApInfo;
    AP_STAT_5ROGUEAPINFO_ENTRY_T                *pstAp5RogueApInfo;
    AP_STAT_NONWLANDEVINTERFINFO_T              stApNonWlanDevInterfInfo;
    AP_STAT_NONWLANDEVINTERFINFO_ENTRY_T        *pstApNonWlanDevInterfInfo;
#endif

    AP_OPER_SYSOPER_T                           *pstApSysOper;
    AP_OPER_FILETRANS_T                         *pstApFileTransConfig;

}SNMP_AGENT_DATA_T;

/******************************* 表ID定义 *************************************/

typedef enum
{
	TABLE_ID_AP_CONFIG_BEGIN, 
        
    //TABLE_ID_SYSTEM,
    
	TABLE_ID_AP_CONFIG_SYS_DEVINFO,
	TABLE_ID_AP_CONFIG_SYS_SWINFO,
	TABLE_ID_AP_CONFIG_SYS_HWINFO,
	TABLE_ID_AP_CONFIG_SYS_IP,
	TABLE_ID_AP_CONFIG_SYS_IPTABLE,
	#if 0
	TABLE_ID_AP_CONFIG_SYS_LEGALAP,
	TABLE_ID_AP_CONFIG_SYS_LEGALAPTABLE,
	TABLE_ID_AP_CONFIG_SYS_FLOWCTL,
	/* BEGIN: Added by zoucaihong, 2013/3/11 */
	TABLE_ID_AP_CONFIG_ADVANCE_5GPRIOR,
	TABLE_ID_AP_CONFIG_ADVANCE_11NPRIOR,
	/* END: Added by zoucaihong, 2013/3/11 */
	#endif
	TABLE_ID_AP_CONFIG_WIFIBASIC,
	TABLE_ID_AP_CONFIG_WIFICHANNEL,
	TABLE_ID_AP_CONFIG_WIFIPOWER,
	TABLE_ID_AP_CONFIG_WIFIADVANCE,
	/* BEGIN: Added by Zhou Shouya, 2013/8/29   PN:VLAN */
	TABLE_ID_AP_CONFIG_WIFIRATE,
	/* END:   Added by Zhou Shouya, 2013/8/29   PN:VLAN */
	/* BEGIN: Added by zoucaihong, 2013/3/21 */
	/* 2.4G卡配置和5G卡配置分开 */
	TABLE_ID_AP_CONFIG_WIFIBASIC5,
	TABLE_ID_AP_CONFIG_WIFICHANNEL5,
	TABLE_ID_AP_CONFIG_WIFIPOWER5,
	TABLE_ID_AP_CONFIG_WIFIADVANCE5,
	/* END: Added by zoucaihong, 2013/3/21 */
	/* BEGIN: Added by Zhou Shouya, 2013/8/29   PN:VLAN */
	TABLE_ID_AP_CONFIG_WIFIRATE5,
	/* END:   Added by Zhou Shouya, 2013/8/29   PN:VLAN */	

    TABLE_ID_AP_CONFIG_VAP,
    TABLE_ID_AP_CONFIG_VAPBASICTABLE,
    TABLE_ID_AP_CONFIG_VAPSECURITYTABLE,

    TABLE_ID_AP_CONFIG_ALARM,
	TABLE_ID_AP_CONFIG_TRAPDES,
	//TABLE_ID_AP_CONFIG_TRAPDES_TABLE,

    TABLE_ID_AP_CONFIG_VLAN,
    TABLE_ID_AP_CONFIG_VLAN_TABLE,

    TABLE_ID_AP_CONFIG_SSID,
    TABLE_ID_AP_CONFIG_SSID_TABLE,

	TABLE_ID_AP_CONFIG_END,

	TABLE_ID_AP_STAT_BEGIN,
	
    //TABLE_ID_INTERFACES,
    //TABLE_ID_INTERFACESTABLE,
        
	TABLE_ID_AP_STAT_SYS,
	TABLE_ID_AP_STAT_WIFI,
	TABLE_ID_AP_STAT_WIFITABLE,
	TABLE_ID_AP_STAT_WIFIVAP,
	TABLE_ID_AP_STAT_WIFIVAPTABLE,
	TABLE_ID_AP_STAT_STATION,
	TABLE_ID_AP_STAT_STATIONTABLE,
#if 0
    TABLE_ID_AP_STAT_24COCHAINTERFINFO,
    TABLE_ID_AP_STAT_24COCHAINTERFINFOTABLE,
    TABLE_ID_AP_STAT_5COCHAINTERFINFO,
    TABLE_ID_AP_STAT_5COCHAINTERFINFOTABLE,
    TABLE_ID_AP_STAT_24ADJCHAINTERFINFO,
    TABLE_ID_AP_STAT_24ADJCHAINTERFINFOTABLE,
    TABLE_ID_AP_STAT_5ADJCHAINTERFINFO,
    TABLE_ID_AP_STAT_5ADJCHAINTERFINFOTABLE,
    TABLE_ID_AP_STAT_24WLANDEVINTERFINFO,
    TABLE_ID_AP_STAT_24WLANDEVINTERFINFOTABLE,
    TABLE_ID_AP_STAT_5WLANDEVINTERFINFO,
    TABLE_ID_AP_STAT_5WLANDEVINTERFINFOTABLE,
    TABLE_ID_AP_STAT_NONWLANDEVINTERFINFO,
    TABLE_ID_AP_STAT_NONWLANDEVINTERFINFOTABLE,

    TABLE_ID_AP_STAT_BRMODESTA,
    TABLE_ID_AP_STAT_BRMODESTATABLE,
    TABLE_ID_AP_STAT_24ROGUEAPINFO,
    TABLE_ID_AP_STAT_24ROGUEAPINFOTABLE,
    TABLE_ID_AP_STAT_5ROGUEAPINFO,
    TABLE_ID_AP_STAT_5ROGUEAPINFOTABLE,
#endif    
	TABLE_ID_AP_STAT_END,
    TABLE_ID_AP_OPER_BEGIN,
    TABLE_ID_AP_OPER_SYSOPER,
    TABLE_ID_AP_OPER_FILETRANS,
    TABLE_ID_AP_OPER_END,
	
	MAX_TABLE_NUM 
}SNMP_TABLE_E;

/* BEGIN: Added by zoucaihong, 2013/9/24 */
#define UDP_QUEUE_NUM   256
/* 批量处理最大数目 */
#define UDP_QUEUE_BATCHNUM_MAX     32

typedef struct queueNode
{
    struct queueNode 	*pstNext;
    struct queueNode 	*pstPrev;

    UINT8               *pucData;       /* point to the node data */	
} QUEUE_NODE_T;

typedef struct
{
    QUEUE_NODE_T    stFreeHead;         /* head of free node list */
    QUEUE_NODE_T    stFreeTail;         /* tail of free node list */

    QUEUE_NODE_T    stUsedHead;         /* tail of used node list */
    QUEUE_NODE_T    stUsedTail;         /* tail of used node list */

    UINT32          ulNodeNum;          /* node num */
    UINT32          ulNodeDataSize;     /* node data size */
	
    QUEUE_NODE_T    *pstNodeList;  	    /* node list for queue */	
} QUEUE_T;
/* END: Added by zoucaihong, 2013/9/24 */

/******************************* 注册表宏定义 *********************************/
#define SNMP_INIT_TABLE(table_oid, ulTableID, szTableName, ulMaxTableRecNum, pfTableValidFunc) \
{																	\
    if (!snmp_init_table_info(pstTable,                             \
                         TRUE,                                      \
                         ulTableID,                                 \
                         szTableName,                               \
                         pfTableValidFunc,                          \
                         sizeof(table_oid)/sizeof(OID),             \
                         table_oid,                                 \
                         ulMaxTableRecNum,                          \
                         sizeof(*type)))							\
	{																\
		return FALSE;												\
	}																\
}

#define SNMP_INIT_SCALAR(table_oid, ulTableID, szTableName, pfTableValidFunc) \
{																	\
    if (!snmp_init_table_info(pstTable,                             \
                         FALSE,                                     \
                         ulTableID,                                 \
                         szTableName,                               \
                         pfTableValidFunc,                          \
                         sizeof(table_oid)/sizeof(OID),             \
                         table_oid,                                 \
                         0,											\
                         sizeof(*type)))							\
	{																\
		return FALSE;												\
	}																\
}

#define SNMP_INIT_SCALAR_FIELD(var, ucColumnType, mib_rw)				\
{                                                                       \
    if (!snmp_init_column_info(pstTable, "", ucColumnType, 				\
                           sizeof(var), (UINT16)(UINT32)(&var), mib_rw, FALSE)) \
    {                                                                   \
        return FALSE;                                                   \
    }                                                                   \
}

#define SNMP_INIT_TABLE_FIELD(var, ucColumnType, mib_rw, bIsKey)		\
{                                                                       \
    if (!snmp_init_column_info(pstTable, "", ucColumnType, 				\
                           sizeof(var), (UINT16)(UINT32)(&var), mib_rw, bIsKey))\
    {                                                                   \
        return FALSE;                                                   \
    }                                                                   \
}

#define SNMP_INIT_SCALAR_COLUMN(var, columnName, ucColumnType, mib_rw)	\
{                                                                       \
    if (!snmp_init_column_info(pstTable, columnName, ucColumnType,   	\
                           sizeof(var), (UINT16)(UINT32)(&var), mib_rw, FALSE)) \
    {                                                                   \
        return FALSE;                                                   \
    }                                                                   \
}

#define SNMP_INIT_TABLE_COLUMN(var, columnName, ucColumnType, mib_rw, bIsKey) \
{                                                                       \
    if (!snmp_init_column_info(pstTable, columnName, ucColumnType,  	\
                           sizeof(var), (UINT16)(UINT32)(&var), mib_rw, bIsKey))\
    {                                                                   \
        return FALSE;                                                   \
    }                                                                   \
}

#if 0
#define SNMP_INIT_COLUMN_ENUM(enumEntry)                \
{                                                        \
    if (!snmp_init_column_enum_info(pstTable, enumEntry))	\
	{                                                                 \
		return FALSE;                                             \
	}                                                                 \
}
#endif

#define SNMP_INIT_COLUMN_RANGE(min, max) \
{\
	if (!snmp_init_column_range(pstTable, min, max))\
	{\
		return FALSE;\
	}\
}

#if 0
#define SNMP_INIT_COLUMN_UNITS(units)\
{\
    if (!snmp_init_column_units(pstTable, units))\
    {\
    	return FALSE;\
    }\
}
#endif

#define SNMP_INIT_COLUMN_DEFVAL(pdefval, defval)\
{\
    if (!snmp_init_column_defval(pstTable, pdefval, defval))\
    {\
    	return FALSE;\
    }\
}


#endif

