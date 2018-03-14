/******************************************************************************
* COPYRIGHT GBCOM
*******************************************************************************
* ��    ��: 
* �޸���ʷ: 
* 2012-9-10     �뾲              �½�
******************************************************************************/

/******************************* �궨�� ********************************/
#ifndef __MINI_SNMPD_DATA__
#define __MINI_SNMPD_DATA__

#define SNMPAGENT_CONTROL_NAME "snmpagentd"
#define AP_AGENT_TRAP_PORT       162
#define AP_AGENT_TRAP_TIMEOUT    5

// ����ȱʡ��д��ͬ�嶨��
#define AGENT_RO_COMMUNITY    "public"
#define AGENT_RW_COMMUNITY    "private"

/*��֤���Ͷ���*/
#define authenticationType_OPENSYSTEM         1
#define authenticationType_SHAREDKEY          2
#define authenticationType_WPAPSK             3
#define authenticationType_WPA                4
#define authenticationType_WAPI               5

/*�������Ͷ���*/
#define securityMode_DISABLE    0
#define securityMode_WPAPSK     1
#define securityMode_WEP        2
#define securityMode_WPA        3

/*���ܻ��ƶ���*/
#define encyptType_TKIP                 1
#define encyptType_AES                  2

/*IPv4��ַ����*/
#define ENUM_IP_MANUAL      0
#define ENUM_IP_DHCP        1

/*��̬�ŵ�����*/
#define AUTOCHANNEL_OFF     0
#define AUTOCHANNEL_ON      1

/*�ŵ�����*/
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
/* AP�豸Ӳ�����Ͷ��� */
#define AP_HWTYPE_AR9341        ((UINT32)5)
#define AP_HWTYPE_AR9558        ((UINT32)8)
#define AP_HWTYPE_AR7240        ((UINT32)1)
#define AP_HWTYPE_AR9344        ((UINT32)4)
#define AP_HWTYPE_AR9531       ((UINT32)9)
#define AP_HWTYPE_AR9563        ((UINT32)10)
/* END: Added by zoucaihong, 2014/5/15 */

/* BEGIN: Added by zoucaihong, 2014/6/17 */
/* �����ļ����뵼������ */
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
/* �豸�������״̬
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
/* �豸״̬ö�� */
typedef enum{
    DEV_STATE_NONE=0,
    DEV_STATE_DISCOVER=1,
    DEV_STATE_RUNNING=2,
    DEV_STATE_MAX
}DEV_STATE_E;

/* �豸����״̬ */
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
        g_ulHeartTicks++;/* ������ʱ */   \
    }                                           \
    else if( DEV_STATE_DISCOVER == g_enDevState )           \
    {                                           \
        g_ulDisTicks++;/* discover��ʱ��ʱ */   \
    }                                           \
	agent_printf(LOG_DEBUG,"g_ulTicks:%d,g_ulHeartTicks:%d,g_ulDisTicks:%d,state:%s",\
	g_ulTicks,g_ulHeartTicks,g_ulDisTicks,g_aucStateName[g_enDevState]); \
}while(0);









/* END: Added by zoucaihong, 2014/5/13 */
/****************************snmp��д��ͬ�������ļ�***************************/
#define SNMPAGENT_FILE_CONF                "/tmp/snmp_agent.conf"
#define FILE_TRANS_DIR          "/tmp/app.tar.gz"
#define UNCOMPRESS_DIR          "/tmp/cms-version"
#define UPDATE_IMAGE_LOG        "/tmp/auto_up_ver_info"
#define CURR_VERSION            "currVersion"
#define TARG_VERSION            "TargVersion"
#define CONFIG_FILE_DIR         "/tmp/tmp.bin"
#define CCS_CONIG_FILE          "/tmp/ccscfg"

/* BEGIN: Added by zoucaihong, 2014/5/14 */
/* DHCP Option43�����ļ� */
#define DHCP_OPTION43_CONF                 "/var/db/ipcfg.conf "
/* END: Added by zoucaihong, 2014/5/14 */
#define BACKUP_NAME         "/tmp/backup.dat"
/* BEGIN: Added by zoucaihong, 2014/7/16 */
#define PROBLEM_COLLECT_FILE    "/tmp/dev_problem_file.tar.gz"
#define PROBLEM_COLLECT_SCRIPT  "/etc/dev_problem_script.sh"
/* END: Added by zoucaihong, 2014/7/16 */





/* ����ģ����½���궨��*/
/*
0���ɹ�
1���ļ����������쳣
2�������ļ�FTP����ʧ��
3�������ļ����ݲ��Ϸ�
4������ԭ��
*/
#define CONFIGFILE_UPDATE_RESULT_SUCCESS                0
#define CONFIGFILE_UPDATE_RESULT_FILETRANSCFGINVALID    1
#define CONFIGFILE_UPDATE_RESULT_FILETRANSFAIL          2
#define CONFIGFILE_UPDATE_RESULT_CONFIGFILEINVALID      3
#define CONFIGFILE_UPDATE_RESULT_OTHER                  4







/******************************NVRAM�ж���************************************/
#ifdef OPENWRT

#define UCI_NETWORK "network."
#define UCI_GATEWAY "gateway.@gateway[0]."
#define UCI_SYSTEM "system.@system[0]."



/*ϵͳ���*/
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


/*�����ŵ�����*/
#define NVRAM_CHANNELMODE           "AutoChanSwitch"
#define NVRAM_AUTOCHANTIME          "AutoChanTime"
#define NVRAM_AUTOCHANLIST          "AutoChanList"
#define NVRAM_STATICCHANNEL         "ModuleChannelSet"
#define NVRAM_RFNUM                 "ChainMask"
#define NVRAM_ABGMODE               "AbgMode"
/*���߸߼�����*/
#define NVRAM_SWITCH                "RssiSwitch"
#define NVRAM_MODE                  "RssiMode"
#define NVRAM_RSSIHIGH              "RssiHigh"
#define NVRAM_RSSILOW               "RssiLow"
#define NVRAM_RSSIINTERVAL          "RssiInterval"
/*���߹�������*/
#define NVRAM_MAXTXPOWER            "CurTxPower"

/*VAP���*/
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
/*VAP����*/
#define NVRAM_FLOWSWITCH            "Flowswtich"
#define NVRAM_UPLOADMAXRATE         "Uploadflow"
#define NVRAM_DOWNLOADMAXRATE       "Downloadflow"

/*VLAN���*/
#define NVRAM_VLANID                "VlanId"
#define NVRAM_VLANNAME              "VlanName"
#define NVRAM_VLANSSID              "VlanSsid"
#define NVRAM_VLANPORT              "VlanPort"
#define NVRAM_VLANPRRIO             "VlanPrio"

/*�澯���*/
#define NVRAM_ALARMCPULOADHIGH      UCI_GATEWAY"alarmCpuLoadHigh"
#define NVRAM_ALARMCPULOADLOW       UCI_GATEWAY"alarmCpuLoadLow"
#define NVRAM_ALARMMEMLOADHIGH      UCI_GATEWAY"alarmMemLoadHigh"
#define NVRAM_ALARMMEMLOADLOW       UCI_GATEWAY"alarmMemLoadLow"

/*trap���*/
#define NVRAM_TRAPDSWTICH           UCI_GATEWAY"trap_switch"
#define NVRAM_TRAPDESIP             UCI_GATEWAY"trap_ip"
#define NVRAM_TRAPDESPORT           UCI_GATEWAY"trap_port"
#define NVRAM_TRAPCOMMUITY          UCI_GATEWAY"trap_community"


#define NVRAM_CONFIGVERSION     UCI_GATEWAY"snmp_config_version"

#else
/*ϵͳ���*/
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


/*�����ŵ�����*/
#define NVRAM_CHANNELMODE           "AutoChanSwitch"
#define NVRAM_AUTOCHANTIME          "AutoChanTime"
#define NVRAM_AUTOCHANLIST          "AutoChanList"
#define NVRAM_STATICCHANNEL         "ModuleChannelSet"
#define NVRAM_RFNUM                 "ChainMask"
#define NVRAM_ABGMODE               "AbgMode"
/*���߸߼�����*/
#define NVRAM_SWITCH                "RssiSwitch"
#define NVRAM_MODE                  "RssiMode"
#define NVRAM_RSSIHIGH              "RssiHigh"
#define NVRAM_RSSILOW               "RssiLow"
#define NVRAM_RSSIINTERVAL          "RssiInterval"
/*���߹�������*/
#define NVRAM_MAXTXPOWER            "CurTxPower"

/*VAP���*/
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
/*VAP����*/
#define NVRAM_FLOWSWITCH            "Flowswtich"
#define NVRAM_UPLOADMAXRATE         "Uploadflow"
#define NVRAM_DOWNLOADMAXRATE       "Downloadflow"

/*VLAN���*/
#define NVRAM_VLANID                "VlanId"
#define NVRAM_VLANNAME              "VlanName"
#define NVRAM_VLANSSID              "VlanSsid"
#define NVRAM_VLANPORT              "VlanPort"
#define NVRAM_VLANPRRIO             "VlanPrio"

/*�澯���*/
#define NVRAM_ALARMCPULOADHIGH      "alarmCpuLoadHigh"
#define NVRAM_ALARMCPULOADLOW       "alarmCpuLoadLow"
#define NVRAM_ALARMMEMLOADHIGH      "alarmMemLoadHigh"
#define NVRAM_ALARMMEMLOADLOW       "alarmMemLoadLow"

/*trap���*/
#define NVRAM_TRAPDSWTICH           "trap_switch"
#define NVRAM_TRAPDESIP             "trap_ip"
#define NVRAM_TRAPDESPORT           "trap_port"
#define NVRAM_TRAPCOMMUITY          "trap_community"

/*2015-10-22  ccs����ģ��-�汾��*/
#define NVRAM_CONFIGVERSION     "snmp_config_version"

#endif

/******************************* �������� ********************************/
#define SNMP_TIMEOUT  5*60 /* 5���� */
#define AGENT_REFRESH_PERIOD_DFTVAL  ((UINT32)10)
#define WIFI_MAX_NUM 2
#define VAP_MAX_NUM  32
#define NVRAM_SIZE  32

/**
RFC1212�н��飺����������ӻ�ɾ��MIB��ṹ���У��ж�����Ӧ�ð���һ��
��Ϊ��xType����xStatus�����������﷨����Ӧ����ö���͵�������
����ж����Ϊ״̬�ж���
������վ����ͨ������״̬�ж����ֵ�����Ʊ����еĴ�����ɾ��������

״̬���ж�����6������ֵ��
-active��1��������״̬���ǿ��õ�
-notInService��2���������д��ڵ�������
-notReady��3�����������ڣ�����Ϊȱ�ٱ�Ҫ����Ϣ��������
-createAndGo��4�����й��������ã�����ϣ������һ�������в����ø��е�״̬�ж���Ϊactive
-createAndWait��5�����й��������ã�����ϣ������һ�������У���������
-destroy��6����ɾ����

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

/******************************* ���������� ***********************************/
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

/******************************* �����ƶ���************************************/
#if 0
/* BEGIN: Added by zoucaihong, 2012/12/4 
add system group and interfaces group of RFC1213*/
/*the system group*/
#define TABLE_NAME_SYSTEM                       "system"
#define TABLE_NAME_INTERFACES                   "interfaces"
#define TABLE_NAME_INTERFACESTABLE              "interfacesTable"
/* END: Added by zoucaihong, 2012/12/4 */
#endif
/*���ñ�*/
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
/* 2.4G�����ú�5G�����÷ֿ� */
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

/*ͳ�Ʊ�*/
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

/********************************** �ṹ���� **********************************/
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
/************************ RFC1213��RFC2863�ṹ�嶨�� **************************/
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
/***************************** ˽��mib�ṹ�嶨�� ******************************/
/******************************* ���ýṹ�嶨�� *******************************/
/*
 * apDevInfo
 */
typedef struct 
{
	UINT8   aucSysMacAddr[6];   /*���߿�MAC ��ַ*/
	UINT8   aucResv[2];	
    /* BEGIN: Added by zoucaihong, 2012/11/5 */
	UINT32  ulSysUpTime;
    /* END: Added by zoucaihong, 2012/11/5 */
    /* BEGIN: Added by zoucaihong, 2012/12/25 */
    UINT32  ulRegetPeriod;       /*����ˢ������*/
    /* END: Added by zoucaihong, 2012/12/25 */
    /* BEGIN: Added by zoucaihong, 2013/1/22 */
    UINT32  ulIfSpeed;           /*���·/���·���Ӎ������*/
    /* END: Added by zoucaihong, 2013/1/22 */
    /* BEGIN: Added by zoucaihong, 2013/4/24 */
    UINT32  ulTrapSwitch;       /*trap����*/
    /* END: Added by zoucaihong, 2013/4/24 */
    /* BEGIN: Added by Zhou Shouya, 2013/8/9   PN:VLAN */
    UINT32  ulSysRestart;       /* ����AP */
    /* END:   Added by Zhou Shouya, 2013/8/9   PN:VLAN */
    /* BEGIN: Added by zoucaihong, 2014/5/13 */
    UINT32  ulTimeZone;         /* ʱ�� */
    CHAR    acTime[32];         /* ʱ�� */
    UINT8   aucIpAddress[4];    /* �豸��IP */
    UINT32  ulSysManageState;   /* �豸����״̬��offline(0), online(1) */
    UINT32  ulSysModel;         /* �豸�ͺ�ID */
    CHAR    acInterModel[32];   /* �ڲ��豸�ͺ����� */
    CHAR    acSysModelName[32]; /* �豸�ͺ����� */
    /* END: Added by zoucaihong, 2014/5/13 */
    UINT32  ulTimeOutInterval;  /* ���ĳ�ʱʱ�䣬��λ:second */
    UINT32  ulHeartBeatPeriod;  /* �������ڣ���λ:second */
    /* BEGIN: Added by zoucaihong, 2014/9/23 */
    UINT32  ulDevType;          /* �豸��̬��1(����),2(AP) */
    CHAR    acSysName[32];      /* �豸���� */
    CHAR    acHotId[32];        /* �ȵ�ID */
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
    UINT32  ulIpMode;           /* IPģʽ:static(0), dhcp(1) */
    /* END: Added by zoucaihong, 2014/6/3 */
    UINT8   aucIpAddress[4];    /* IP ��ַ */
    UINT8   aucIpNetMask[4];    /* �������� */
    /* BEGIN: Modified by zoucaihong, 2014/6/3 */
    UINT8   aucGwAddress[4];    /* ���ص�ַ */
    UINT8   aucDnsMaster[4];    /* ����DNS������ */
    UINT8   aucDnsSlave[4];     /* ����DNS������ */
    /* END: Modified by zoucaihong, 2014/6/3 */
}AP_CONFIG_IP_ENTRY_T;
/* END: Added by zoucaihong, 2013/2/25 */

#if 0
/* BEGIN: Added by zoucaihong, 2013/5/13 */
/*�Ϸ�AP��������*/
typedef struct
{
    UINT32  ulNum;
}AP_CONFIG_LEGALAP_NUM_T;

typedef struct
{
    UINT32  ulIndex;
    CHAR    acLegalApSsid[32];      /*�Ϸ�AP SSID*/
    UINT8   aucLegalApBssid[6];     /*�Ϸ�AP BSSID*/
    UINT8   aucResv[2];
}AP_CONFIG_LEGALAP_ENTRY_T;

/*AP����������*/
typedef struct
{
    UINT32  ulFlowSwitch;           /*AP���ؿ���*/
    UINT32  ulApUplinkMaxRate;      /*AP�����������*/
    UINT32  ulApDwlinkMaxRate;      /*AP�����������*/
    /* BEGIN: Added by Zhou Shouya, 2013/8/13   PN:VLAN */
    UINT32  ulStaUplinkMaxRate;     /*STA�����������*/
    UINT32  ulStaDwlinkMaxRate;     /*STA�����������*/
    /* END:   Added by Zhou Shouya, 2013/8/13   PN:VLAN */
}AP_CONFIG_FLOWCTL_T;
/* END: Added by zoucaihong, 2013/5/13 */

/* BEGIN: Added by zoucaihong, 2013/3/11 */
/*
 * ap5gPriorConfig
 */
typedef struct 
{
    UINT32  ul5GPriSwitch;  /* 5G���ȿ��� */
    UINT32  ul2dot4GDelayTime;  /* 2.4G�ӳ�ʱ��,��λ:�� */
    UINT32  ulStaTimesOut;    /* �ն˳�ʱʱ��,��λ:�� */
}AP_CONFIG_ADVANCE_5GPRIOR_T;

/*
 * ap11nPriorConfig
 */
typedef struct 
{
    UINT32  ul11nPriSwitch;  /* 11n���ȿ��� */
    UINT32  ul11bgDelayTime;  /* 11bg�ӳ�ʱ��,��λ:�� */
    UINT32  ulStaTimesOut;    /* �ն˳�ʱʱ��,��λ:�� */
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
    UINT32  ulHardwareType;         /* Ӳ������ */
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
	UINT32  ulWifiDeviceType;				/* �豸���� :
    ap-single(0),ap-ms(1),adhoc-single(2),adhoc-ms(3),ap-adhoc(4),sta-adhoc(5),sta-single(6),sta-ms(7)*/
}AP_CONFIG_WIFIDEVICEBASIC_T;

typedef struct
{
	UINT32  ulWifiId;
	UINT32  ulAbgMode;					/* A/B/Gģʽ */
	UINT32  ulChannelUsing;				/* ��ǰʹ���ŵ� */
	UINT32  ulChannelState;				/* �����ŵ�ģʽ:0:static; 1:dynamic */
    UINT32  ulAutoChanTime;             /* ��̬�ŵ�ɨ��ʱ�� */
    UINT32  ulStaicChannel;             /* ��̬�ŵ� */
	CHAR    acAutoChanList[256];        /* ��̬�ŵ��б� */
    /* BEGIN: Added by zoucaihong, 2012/11/1 */
	UINT32  ulRFNum;    /* �ռ������� */
	UINT32  ulBwMode;    /* 0:off; 1:on */
    /* END: Added by zoucaihong, 2012/11/1 */
}AP_CONFIG_WIFIDEVICECHANNEL_T;

typedef struct
{
	UINT32  ulWifiId;
	UINT32	ulLimitTxPower;	/* ����书�� */
	UINT32	ulMaxTxPower;	/* ˥��ֵ  */

    /* BEGIN: Added by zoucaihong, 2012/11/1 */
    UINT32  ulTransmitPower;/* ��ǰ���书�� */
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
    UINT32  ulRateMode;     /* ����ģʽ */
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
    CHAR    acSsid[32];/*SSID����*/
    UINT32  ulSsidEnabled;/*SSID �Ƿ�����*/
    UINT32  ulSsidBroadcast;/*SSID�Ƿ�㲥*/
    UINT32  ulVlanId;/* VLAN ID*/
    /* BEGIN: Added by zoucaihong, 2012/11/1 */
    UINT32  ulVlanEnabled;/* VLAN �Ƿ�����*/
    /* END: Added by zoucaihong, 2012/11/1 */
    UINT8   aucBssid[6];
    UINT8   aucResv[2];
    UINT32  ulMaxSimultUsers;
    UINT32  ulFlowSwitch;
    UINT32  ulUplinkMaxRate;    /*VAP������������(Kbps)*/
    UINT32  ulDownlinkMaxRate;  /*VAP������������(Kbps)*/
    /* BEGIN: Added by zoucaihong, 2013/4/23 */
    UINT32  ulUserUpload;       /*�û�����������(Kbps)*/
    UINT32  ulUserDownload;     /*�û�����������(Kbps)*/
    /* END: Added by zoucaihong, 2013/4/23 */
    UINT32  ulEndUserTime;

    UINT32  ulRowStatus;
}AP_CONFIG_VAPBASIC_ENTRY_T;

typedef struct
{		
	UINT32  ulIndex;
	UINT32  ulVapId;
    UINT32  ulWifiId;
    CHAR    acSsid[32];/*SSID����*/
    
	UINT32  ulAuthenticationType; /* 0: disabled, 1:wep */
	UINT32  ulSecurityMode;     /*��������:1:open;
	                                    2: shared
                                        3:wpa-psk
                                        4:wpa2-psk
                                        5:psk-mixed
                                        6:wpa
                                        7:wpa2
                                        8:wpa-wpa2-mixed
                                */
	UINT32  ulEncryptType; /*���ܻ���:0:TKIP; 1:AES*/      
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
    UINT32   ulCpuLoadHigh	;	/* CPU���ɸ澯��������	90      */
    UINT32   ulCpuLoadLow	;	/* CPU���ɸ澯��������	80      */

    UINT32   ulMemLoadHigh	;	/* �ڴ������ʸ澯��������	85  */
    UINT32   ulMemLoadLow	;	/* �ڴ������ʸ澯��������	80  */

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
    UINT32 ulVlanPrio;/*VLAN���ȼ�*/
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
/******************************* ͳ�ƽṹ�嶨��********************************/
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
	UINT64      ulIfInNUcastPkts;   /* �˿ڽ��շǵ������� */
	UINT64      ulIfInBytes;
	UINT32      ulIfInDiscardPkts;
	UINT32      ulIfInErrPkts;
	
	UINT64      ulIfOutUnicastPkts;
	UINT32      ulIfOutBroadcastPkts;
	UINT32      ulIfOutMulticastPkts;
	UINT64      ulIfOutNUcastPkts;  /* �˿ڷ��ͷǵ������� */
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
    UINT32  ulAssoStaNum;/*ʵ�������û���*/
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
    UINT32  ulAssoStaNum;/*ʵ�������û���*/
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
    UINT8  aucBssid[6];         /*AP�豸SSID MAC address*/
    UINT8  aucResv[2];
    UINT8  aucSsid[32];         /*SSID����*/
    UINT32 ulVlanId;            /*VLAN ID*/
    /* BEGIN: Added by zoucaihong, 2013/4/18 */
    UINT32 ulVlanEnable;        /*VLAN�Ƿ�����*/
    /* END: Added by zoucaihong, 2013/4/18 */
	UINT8  aucStaMac[6];        /*�����Ñ��ն�MAC address*/
    UINT8  aucResv2[2];
    /* BEGIN: Added by zoucaihong, 2014/6/18 */
    UINT8  aucStaIp[4];        /* �ն�IP */
    /* END: Added by zoucaihong, 2014/6/18 */
    UINT8  aucStaDevType[64];   /*�����Ñ��ն��豸����*/
    UINT32 ulConnectTime;       /*�����Ñ��ն�Connection Time*/
    UINT64 ulStaUplinkOctets;   /*�����Ñ��ն�����Ѷ������*/
    UINT64 ulStaDownlinkOctets; /*�����Ñ��ն�����Ѷ������*/
	INT32  iRssi;               /*AP�˽���֮����·�Ñ��ն�RSSI(Received Signal Strength Indication)����λ��ΪdBm*/
	UINT8  aucFrequency[32];    /*ʹ�õ��l��(2.4GHz��5GHz)*/
    UINT32 ulSnr;               /*AP�˽���֮���·�Ñ��K��SNR(Signal-to-Noise Ratio)����λ횞�dB*/
    UINT32 ulTxRate;            /*�����Ñ��ն�ʵ��㴫������(Transmit Rate)*/
    CHAR   acTxRate[32];
    UINT32 ulErrRate;           /*�����Ñ��ն˷��������(Packet Error Ratio)*/
    /* BEGIN: Added by Zhou Shouya, 2013/9/6 */
    UINT32 uAssoRate;           /* �ն�Э������ */
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
* 2.4GͬƵ����(Co-Channel Interference��CCI)
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
* 5GͬƵ����(Co-Channel Interference��CCI)
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
* 2.4G��Ƶ����(Adjacent Channel Interference��ACI)
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
* 5G��Ƶ����(Adjacent Channel Interference��ACI)
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
* 2.4G WLAN �豸����(WLAN Devices Interference)
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
* 5G WLAN �豸����(WLAN Devices Interference)
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
* 2.4G �Ƿ�AP�б�
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
* 5G �Ƿ�AP�б�
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
* ��WLAN �豸����(Non WLAN Devices Interference)
*/
typedef struct
{
    UINT32  ulNum;
}AP_STAT_NONWLANDEVINTERFINFO_T;
typedef struct
{
    UINT32 ulIndex;
    UINT32 ulWifiId;
    INT32  iNoise;/*��ǰ�ŵ�����*/
    UINT32 ulChannel;/*�ŵ�*/
    CHAR   acTime[32];
}AP_STAT_NONWLANDEVINTERFINFO_ENTRY_T;
/*
* Bridge Modeʱ�ն���Ϣͳ��
*/
typedef struct
{
    UINT32 ulStationNum;
}AP_STAT_BRMODESTA_T;

typedef struct 
{
    UINT32 ulStaID;
    UINT32 ulMode;/*��Ե���Զ��ģʽ*/
	UINT8  aucStaMac[6];/*�Խ��豸MAC*/
    UINT8  aucResv2[2];
	INT32  iRssi;/*�Խ��豸RSSI(Received Signal Strength Indication)����λ��ΪdBm*/
    UINT32 ulSnr;/*�Խ��豸SNR(Signal-to-Noise Ratio)����λ횞�dB*/
    UINT32 ulTxRate;/*�Խ��豸ʵ��㴫������(Transmit Rate)*/
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
* �豸ϵͳ����
*/
typedef struct
{
    UINT32  ulSoftwareUpdate;   /* �豸������� */
    UINT32  ulSysRestart;       /* �豸���� */
    UINT32  ulSysReset;         /* �豸�ָ��������� */
    UINT32  ulCfgFileImpExp;    /* �����ļ����뵼��,normal(0),import(1),export(2) */
    /* BEGIN: Added by zoucaihong, 2014/7/15 */
    UINT32  ulDiagnose;         /* һ���ռ���normal(0),diagnose(1) */
    /* END: Added by zoucaihong, 2014/7/15 */
    
    UINT32  ulStopRecover;          /* */
    CHAR    acOfflineConfigFile[256];   /*CCSV3�·�����ģ��==  �汾��*/
}AP_OPER_SYSOPER_T;

/*
* �ļ���������
*/
typedef struct{
    UINT32      ulLoadFlag;                 /*1-download,2-upload*/
    CHAR        acFileName[128];            /*�汾�ļ�·��*/
    UINT32      ulFileType;                 /*�ļ�����software(1),config(2),log(3),cert(4),perf(5)*/
    UINT32      ulTransProto;               /*����Э������*/
    UINT8       aucServerIp[4];             /*Զ���ļ����ڵ�ַ*/
    UINT32      ulTransPort;                /*����Ķ˿ں�*/
    CHAR        aucTransUser[32];           /*�û���*/
    CHAR        aucTransPasswd[32];         /*����*/
    UINT32      ulTransStatus;              /*����״̬ 0:ok, 1:param invalid, 2:download fail, 3:updatefail */
    CHAR        acFailReason[128]; 
	CHAR        acFileVersion[32];//����汾��
}AP_OPER_FILETRANS_T;
/* END: Added by zoucaihong, 2014/5/14 */
#define BIND_AC_NUM_MAX     8
#define AC_URL_LEN_MAX      128
typedef struct
{
    /* DHCP OPTION43���ַ�ʽʱ, �󶨵�AC IP�б�*/
	UINT32	uiBindAcNum;
	UINT8	aucBindAcIp[BIND_AC_NUM_MAX][4];

	/* DNS���ַ�ʽʱ, �󶨵�AC URL */
	CHAR	acAcURL[AC_URL_LEN_MAX];		/* AC URL */
	UINT32	uiResAcNum;		/* DNS������ȡ����AC IP����*/
	UINT8	aucResAcIp[BIND_AC_NUM_MAX][4];	/* DNS�������ȡ����AC IP */
}AC_IP_LIST_T;

/********************************* ���ݴ洢 **********************************/
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
    /* 2.4G�����ú�5G�����÷ֿ� */
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

/******************************* ��ID���� *************************************/

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
	/* 2.4G�����ú�5G�����÷ֿ� */
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
/* �������������Ŀ */
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

/******************************* ע���궨�� *********************************/
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

