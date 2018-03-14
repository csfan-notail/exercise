/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : om_tr69c.h
  版 本 号   : 初稿
  作    者   : zhangjiasi
  生成日期   : 2015年5月20日
  最近修改   :
  功能描述   : 用于声明所有与tr069通信相关的参数
  函数列表   :
  修改历史   :
  1.日    期   : 2015年5月20日
    作    者   : zhangjiasi
    修改内容   : 创建文件

******************************************************************************/
#ifndef __OM_TR69C_H__
#define __OM_TR69C_H__
/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "ap_types.h"

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#ifndef TR69C_DEFAULT_CONFIG_FILE
#define TR69C_DEFAULT_CONFIG_FILE     "/tmp/default_config"   //ACS下发的缺省配置文件,tr69c将该值设置到nvram中，然后重启。
#endif

#ifndef RECOVER_DEFAULT_CONFIG
#define RECOVER_DEFAULT_CONFIG  1       //从default_config文件恢复配置
#endif

#ifndef TR69C_SOFTWARE_FILE
#define TR69C_SOFTWARE_FILE     "/tmp/app.bin"   //ACS下发的版本,tr69c将版本写到flash中，然后重启。
#endif

/* 
	add by duanguiyuan 20161125 
	添加华为定制的 告警上报 信息 的 结构体定义
*/
/* 告警上报的 参数节点名称 */
#define TR69C_ALARMCODE_LAN "240010"
/* 告警上报的 状态  1 上报新的告警   2 清除之前的告警 */
#define TR69C_ALARM_REPORT 	1
#define TR69C_ALARM_RCLEAR 	2

typedef enum {
    TR69C_ALARM_TYPE_LAN1=0,
    TR69C_ALARM_TYPE_LAN2,
    TR69C_ALARM_TYPE_LAN3,
    TR69C_ALARM_TYPE_LAN4,
    TR69C_ALARM_TYPE_LanCheck,
    
	TR69C_ALARM_TYPE_END
}OM_TR69C_ALARM_TYPE_E;
typedef enum {
	 TR69C_ALARM_NO=0,
	 TR69C_ALARM_YES
}OM_TR69C_ALARM_STATUS_E;
typedef struct {
	int s_cnt;
    OM_TR69C_ALARM_STATUS_E alarm_flag; /* 是否 需要 上报 告警信息*/
	OM_TR69C_ALARM_STATUS_E alarm_status[TR69C_ALARM_TYPE_END];
}OM_TR69C_ALARM_INFO_T;

/* 告警msg结构体 */
typedef struct {
    UINT32 uiAlarmCode;           /* 告警码 用来标识告警的种类 从1001开始 */
    UINT32 uiAlarmReasonCode;     /* 告警原因码 每一个数字对应一个产生的原因。 从0开始，不同的告警码对应不同的告警原因码 */
    UINT32 uiAlarmState;          /* 告警状态 告警上报0或告警清除1 */
    CHAR  acAlarmObject[32];  /* 告警对象，由设备的SN构成，可以按SN号查看告警 */
    CHAR  acAlarmInfo[128];   /* 告警附加信息 如:cpu利用率90%*/
}OM_TR69C_ALARM_MSG_T;

/* CGI发送给TR69C的事件序号 和tr69c中eCFMMsg的定义一定要对应起来*/
typedef enum {
    TR69C_PARAM_CHANGE=0,
    TR69C_SEND_GETRPC,
    TR69C_SEND_DIAG_COMPLETE,
    TR69C_SEND_ENABLE_INFORM,
    TR69C_SEND_DISABLE_INFORM,
    TR69C_SEND_ACS_URL_CHANGED, /* ACS的URL被改变 */
    TR69C_SEND_WAN_CHANGED,     /* WAN口的IP地址被改变 */
    TR69C_SEND_NONCONNREQAUTH_CHANGED,  /* do nothing */
    TR69C_SEND_ENABLE_NONECONNREQAUTH,
    TR69C_SEND_DISABLE_NONECONNREQAUTH,
    TR69C_ACS_USRNAME_CHANGED,
    TR69C_ACS_PASSWD_CHANGED,
    TR69C_PERIODIC_INFORM_ENABLE,
    TR69C_PERIODIC_INFORM_DISABLE,
    TR69C_PERIODIC_TIME,
    TR69C_CPE_USRNAME_CHANGED,
    TR69C_CPE_PASSWD_CHANGED,
    TR69C_CPE_GET_TR069_STA,
    /* BEGIN: Added by zhanghaozhong, 2013/5/29   PN:增加告警上报功能 */
    TR69C_ALARM,
    /* END:   Added by zhanghaozhong, 2013/5/29   PN:增加告警上报功能 */
    TR69C_SIMPLE_LOG,
    TR69C_DETAIL_LOG,
}OM_TR69C_SEND_MSG_E;

/* msg结构体 */
typedef struct {
    OM_TR69C_SEND_MSG_E eFlag;           
    OM_TR69C_ALARM_MSG_T stAlarm;
}OM_TR69C_UDP_MSG_T;

/**************************************  tr069管理相关   **************************************************/
typedef struct {
    /* tr69c状态信息 */
    CHAR acAcsIP[CHAR_LEN_32];
    UINT32 uiInformState;
    UINT32 uiConnectState;
    UINT32 uiLastCfgState;
} OM_TR69C_STATUS_T;

/* tr69c操作码 */
typedef enum 
{
    TR069_ENABLE,
    
    TR069_ACS_URL,
    TR069_ACS_USRNAME,
    TR069_ACS_PASSWD,

    TR069_ECHO_ENABLE,
    TR069_ECHO_TIME,
    TR069_PERIOD_INFORM_TIME,
    /* acs发起连接时的相关参数 */
    TR069_CPE_AUTH_ENABLE,
    TR069_CPE_URL,
    TR069_CPE_USRNAME,
    TR069_CPE_PASSWD,

    TR069_UDP_REQADDR,
    TR069_UDP_REQADDR_NOTIFY_LIMIT,

    /* stun */
    TR069_STUN_ENABLE,
    TR069_STUN_SERVER_ADDR,
    TR069_STUN_SERVER_PORT,
    TR069_STUN_USERNAME,
    TR069_STUN_PASSWD,
    TR069_STUN_KA_TIME_MAX,
    TR069_STUN_KA_TIME_MIN,
    TR069_NAT_DETECTED,

    TR069_LOCAL_PORT,
    TR069_NEW_PARAMETERKEY, 
    TR069_REBOOT_CMD_KEY,
    TR069_DOWNLOAD_CMD_KEY,
    TR069_UPGRADE_ENABLE,
    TR069_NAT_KEEPALIVE_TIME,
    TR069_DOWNLOAD_FAULT_CODE,
    TR069_DOWNLOAD_FAULT_MSG,
    TR069_DOWNLOAD_START_TIME,
    TR069_DOWNLOAD_END_TIME,
    TR069_ACS_CONNECT_STATE,
    TR069_OPT43_PRO_CODE,
    TR069_OPT43_URL,
    TR069_ROUTE_LIST,
    TR069_ROUTE_NUM,      
    
}OM_TR69C_APP_OPERATE_CODE_E;


typedef struct{
    OM_TR69C_APP_OPERATE_CODE_E eSetFlag;
    INT32       iUnSaveFlag;
    /* ACS与CPE之间关联的参数 */
    INT32       EnableCWMP;                         /* 是否使能tr69c功能 */
    CHAR        acsURL[CHAR_LEN_128];        /* URL of ACS */
    UINT32      uiReGetUrlCnt;                      /* 尝试拿ACS的URL的次数，首次优先使用option43
                                                       带下来的URL，如果没有，则使用默认的URL，首
                                                       次（option43的URL）尝试10次失败后,切换到默
                                                       认。如果首次是默认的URL，则调用GETACSURL函数重新拿URL
                                                       在续约过程中可能会更新option43*/
    CHAR        acsNewURL[CHAR_LEN_128];     /* New URL if URL has been changed*/
    CHAR        acsUser[CHAR_LEN_64];        /* ACS的用户名和密码 */
    CHAR        acsPwd[CHAR_LEN_64];   
    
    INT32       PeriodicInformEnable;               /* 是否定期发起inform报文给ACS */
    time_t      informInterval;                     /* inform interval ，inform的时间间隔*/

    INT32       iConnReqEnable;                             /* 是否使能让ACS连接CPE */
    CHAR        connReqURL[CHAR_LEN_128];            /* CPE的URL、用户名、密码 */
    CHAR        connReqUser[CHAR_LEN_64];
    CHAR        connReqPwd[CHAR_LEN_64];    

    CHAR        newParameterKey[CHAR_LEN_32];  /* the pending key */                       /* save for GBCOM */
    CHAR        rebootCommandKey[CHAR_LEN_32]; /* key for reboot command key */           /* save for GBCOM */
    CHAR        downloadCommandKey[CHAR_LEN_32];     /* key for download cmd*/               /* save for GBCOM */
    

    INT32       upgradesManaged;    /* 是否支持ACS给CPE升级版本，若为真，仅仅使用ACS升级，
                                        若为假，则CPE可以可以使用其他方式升级，如WEB */  

    CHAR        UDPConnReqAddr[CHAR_LEN_128];/* acs用于连接cpe的地址和端口 */
    UINT32      UDPConnReqAddrNotifynLimit; /* ActiveNotification更改到新的
                                               地址时，中间等待的最小时间，
                                               单位：秒 */

    /*****    STUN 相关    *******/
    INT32       iSTUNEnable;            /* Enables or disables the use of
                                           STUN by the CPE. */
    CHAR        acSTUNSvr[CHAR_LEN_128];/* Host name or IP address of the STUN server for the CPE to send Binding Requests if STUN is
                                            enabled via STUNEnable. */
    UINT32      uiSTUNPort;
    CHAR        acSTUNUsername[CHAR_LEN_64];
    CHAR        acSTUNPasswd[CHAR_LEN_64];
    INT32       iSTUNMaxKaPeriod;
    UINT32      uiSTUNMinKaPeriod;
    INT32       iNATDetected;           /* When STUN is enabled, this parameter indicates
                                           whether or not the CPE has detected address
                                           and/or port mapping in use. */
                                            
    INT32       dlFaultStatus;      /* download fault status */
    CHAR        dlFaultMsg[CHAR_LEN_128];    /* download fault message */              /* save for GBCOM */
    time_t      startDLTime;    /* 下载开始时间 */
    time_t      endDLTime;      /* 下载结束时间 */
    INT32       contactedState; /* add by zhanghaozhong 2013-1-20 */
    UINT32      iTr69cPort;
    UINT32      uiKeepAliveTime;
    //INT32       lastInstaceID;  /* 最后实例ID号 */
    //time_t      informTime;     /* next ACS inform Time */
    //int         maxEnvelopes;   /* Number of max env returned in inform response*/
    //int         holdRequests;   /* hold request to ACS if true */
    //int         noMoreRequests; /* don't send any more Req to ACS */
    //RpcMethods  acsRpcMethods;  /* methods from GetRPCMethods response*/
    //char        *parameterKey;  /* update key for ACS - may be NULL */
    //int         noneConnReqAuth;  /* no connection request authentication*/
    //char        connReqPath[TR69C_ARRAY_LEN_128];   /* path part of connReqURL -- used by listener */
    //int         retryCount;     /* reset on each ACS response*/
    //int         fault;          /* last operation fault code */
    //char        kickURL[TR69C_ARRAY_LEN_128];
} OM_TR69C_APP_PARAM_T;


/* 设备相关参数 */
typedef enum{
    TR69C_PARAM_DEV_NAME,
    TR69C_PARAM_MAN_FACT,
    TR69C_PARAM_MAN_FACT_OUI,
    TR69C_PARAM_MODULE,
    TR69C_PARAM_DESCRIPT,
    TR69C_PARAM_PRODUCT_CLASS,
    TR69C_PARAM_SERIAL_NUM,
    TR69C_PARAM_CPE_MAC,
    TR69C_PARAM_HARD_VER,
    TR69C_PARAM_SOFT_WARE,
    TR69C_PARAM_PRIVATE_SOFT_WARE,
    TR69C_PARAM_MODEM_FIRMWARE_VER,
    TR69C_PARAM_PROV_CODE,
    TR69C_PARAM_UPTIME,
    TR69C_PARAM_MAX,
    TR69C_PARAM_USERNAME,
    TR69C_PARAM_PASSWORD,
}OM_TR69C_DEVICE_OPERATE_CODE_E;


typedef struct {
    OM_TR69C_DEVICE_OPERATE_CODE_E eFlag;                        /* 获取信息的标志 */
    CHAR        acDevName[CHAR_LEN_64];   /* 制造商 */
    CHAR        Manufacturer[CHAR_LEN_64];   /* 制造商 */
    CHAR        ManufacturerOUI[6]; /* 厂商OUI */
    CHAR        ModelName[CHAR_LEN_64];      /* CPE型号名称 */
    CHAR        Description[CHAR_LEN_64];    /* CPE设备的完整描述 */
    CHAR        ProductClass[CHAR_LEN_64];   /* 指定产品序列号所归属的产品类别 */
    CHAR        SerialNumber[CHAR_LEN_64];   /* CPE的序列号 */
    CHAR        acCpeMac[CHAR_LEN_64];       /* CPE的MAC */
    CHAR        acHardVer[CHAR_LEN_64];		//硬件版本
    CHAR        acSoftVer[CHAR_LEN_64];		//软件版本	
    CHAR        acPriSoftVer[CHAR_LEN_64];		//私有软件版本
    CHAR        acModemFirmwareVer[CHAR_LEN_64];    //boot版本
    CHAR        ProvisioningCode[CHAR_LEN_64];   /* 供给编码，可由optio43提供 */
    UINT32      UpTime;                                 /* CPE上次重新启动以来的秒数 */
	CHAR        Username[CHAR_LEN_32];   /* 维护账号的用户名 */
	CHAR        Password[CHAR_LEN_64];   /* 维护账号的密码 */
}OM_TR69C_DEVICE_PARAM_T;

/* ntp信息 */
typedef enum 
{
    TR69C_NTP_SWITCH = 0,
    TR69C_NTP_SYN_PERIOD,
    TR69C_NTP_PARAM_NTP_SVR,
    TR69C_NTP_PARAM_NTP_SVR2,
    TR69C_NTP_PARAM_LOCAL_TIME,
    TR69C_NTP_PARAM_LOCAL_TIME_ZONE,
    TR69C_NTP_PARAM_LOCAL_TIME_ZONE_NAME,
}OM_TR69C_NTP_CODE_E;



/**************************************  wan相关   **************************************************/
typedef struct
{
    CHAR   acDevName[CHAR_LEN_32];        //设备名
    CHAR   acForwardIp[CHAR_LEN_32];      //可以连接INTERNET的ip地址        
}OM_TR69C_NET_FORWARD_IP_T;
typedef enum 
{
    OM_TR69C_WAN_IP_CONNECT_ENABLE = 0,
    OM_TR69C_WAN_IP_CONNECT_RESET,
    OM_TR69C_WAN_IP_CONNECT_LAST_ERR,
    OM_TR69C_WAN_IP_CONNECT_NAT_ENABLE,
    OM_TR69C_WAN_IP_CONNECT_IP_ADDR,
    OM_TR69C_WAN_IP_CONNECT_MASK,
    OM_TR69C_WAN_IP_CONNECT_GW,
    OM_TR69C_WAN_IP_CONNECT_DNS1,        //DNS LIST
    OM_TR69C_WAN_IP_CONNECT_DNS2,        //DNS LIST
    OM_TR69C_WAN_IP_CONNECT_MAC,
}OM_TR69C_WAN_IP_CONNECT_CODE_E;


/**************************************  lan相关   **************************************************/

typedef enum 
{
    TR69C_LAN_ENABLE = 0,
    TR69C_LAN_IP_ADDR_SET,
    TR69C_LAN_IP_ADDR_GET,
    TR69C_LAN_IP_NETMASK_SET,
    TR69C_LAN_IP_NETMASK_GET,
    TR69C_LAN_IP_ADDR_TYPE_SET,
    TR69C_LAN_IP_ADDR_TYPE_GET,     /* 默认为static，不可配置 */
    
}OM_TR69C_LAN_CODE_E;

typedef enum 
{
    TR69C_LAN_DHCP_ENABLE = 0,      /* 设置dhcp模式 */
    TR69C_LAN_DHCP_MIN_ADDR,
    TR69C_LAN_DHCP_MAX_ADDR,
    TR69C_LAN_DHCP_RESERVED_ADDR,   /* 保留地址 */
    TR69C_LAN_DHCP_NETMASK,
    TR69C_LAN_DHCP_DNS_SRV_MASTER,     /* 多个dns地址用","隔开 */
    TR69C_LAN_DHCP_DNS_SRV_SLAVE,
    TR69C_LAN_DHCP_DOMAIN,
    TR69C_LAN_DHCP_IPROUTERS,   /* 默认网关 */
    TR69C_LAN_DHCP_LEASE_TIME,
    TR69C_LAN_MAC_ADDR,         /* lan mac address */
    TR69C_LAN_DHCP_CONFIGURABLE,    /* dhcp server配置使能。True为有效，如果
                                       设置为false，则cpe将恢复到默认配置 */
    TR69C_LAN_DHCP_RELAY,
    TR69C_LAN_ALLOWED_MAC_ADDR,     /* 允许接入的mac列表 */
}OM_TR69C_LAN_DHCP_CODE_E;

typedef struct 
{
    CHAR acLanCfgPasswd[CHAR_LEN_64]; //允许lan口接入ACS服务器的密码
}OM_TR69C_LAN_CONFIG_SECURITY_T;

/**************************************  wlan相关   **************************************************/
typedef enum
{

    /* BEGIN: Added by zhanghaozhong, 2013/5/22   PN:NULL */
    TR69C_WIFI_SET_ENABLE_CMD ,
    TR69C_VAP_SET_ENABLE_CMD ,
    TR69C_VAP_SET_SSID_CMD ,
    TR69C_VAP_SET_SSID_HIDDEN_CMD,
    TR69C_VAP_SET_USR_ISOLATION_CMD,
    TR69C_VAP_SET_BAND_CMD ,
    TR69C_VAP_SET_RFNUM_CMD ,
    TR69C_VAP_SET_SHORTGI_CMD ,
    TR69C_VAP_SET_AMPDU_CMD ,
    TR69C_VAP_SET_AMSDU_CMD ,
    TR69C_VAP_SET_CHANNEL_CMD ,
    TR69C_VAP_SET_AUTH_MODE_CMD ,        /*设置加密后的认证模式，即加密模式*/
    TR69C_VAP_SET_DATA_ENCRYPT_CMD ,     /*设置数据加密标志位*/
    TR69C_VAP_SET_WEPKEY_LEN_CMD ,       /*设置WEP密钥长度*/
    TR69C_VAP_SET_WEPKEY_FORMAT_CMD ,    /*设置WEP密钥格式:ASCII or 十六进制*/
    TR69C_VAP_SET_WEPKEY_INDEX_CMD ,     /*设置WEP密钥编号*/
    TR69C_VAP_SET_WEPKEY_CMD ,           /*WEP密钥*/
    TR69C_VAP_SET_WPATYPE_CMD ,          /*设置加密算法(tkip or aes)*/
    TR69C_VAP_SET_WPA_RENEW_CMD ,        /*设置WPA密钥更新周期*/
    TR69C_VAP_SET_WPA_KEY_CMD ,          /*WPA密钥*/
    TR69C_CREATE_VAP_CMD ,
    TR69C_DELETE_VAP_CMD ,


    TR69C_WIFI_GET_ENABLE_CMD ,
    TR69C_VAP_GET_ENABLE_CMD ,
    TR69C_VAP_GET_STATUS_CMD ,
    TR69C_VAP_GET_BSSID_CMD ,
    TR69C_VAP_GET_SSID_CMD ,
    TR69C_VAP_GET_SSID_HIDDEN_CMD ,
    TR69C_VAP_GET_USR_ISOLATION_CMD,
    TR69C_VAP_GET_MAXRATE_CMD ,
    TR69C_VAP_GET_MODE_CMD ,
    TR69C_VAP_GET_BAND_CMD ,
    TR69C_VAP_GET_WPS_ENABLE_CMD,
    TR69C_VAP_GET_WPS_MODE_CMD,
    TR69C_VAP_GET_RFNUM_CMD ,
    TR69C_VAP_GET_SHORTGI_CMD ,
    TR69C_VAP_GET_AMPDU_CMD ,
    TR69C_VAP_GET_AMSDU_CMD ,
    TR69C_VAP_GET_CHANNEL_CMD ,
    TR69C_GET_CHANNEL_LIST_CMD ,
    TR69C_GET_CURCHANNEL_CMD ,
    TR69C_GET_BYTES_SENT_CMD ,
    TR69C_GET_BYTES_RECV_CMD ,
    TR69C_GET_PACKET_SENT_CMD ,
    TR69C_GET_PACKET_RECV_CMD ,
    TR69C_GET_STA_NUM_CMD ,
    TR69C_GET_STA_MAC_CMD ,
    TR69C_VAP_GET_AUTH_MODE_CMD ,        /*获取加密后的认证模式，即加密模式*/
    TR69C_VAP_GET_DATA_ENCRYPT_CMD ,     /*获取数据加密标志位*/
    TR69C_VAP_GET_WEPKEY_LEN_CMD ,       /*获取WEP密钥长度*/
    TR69C_VAP_GET_WEPKEY_FORMAT_CMD ,    /*获取WEP密钥格式:ASCII or 十六进制*/
    TR69C_VAP_GET_WEPKEY_INDEX_CMD ,     /*获取WEP密钥编号*/
    TR69C_VAP_GET_WPATYPE_CMD ,          /*获取加密算法(tkip or aes)*/
    TR69C_VAP_GET_WPA_RENEW_CMD ,        /*获取WPA密钥更新周期*/
    TR69C_WIFI_GET_VAP_NUM ,             /* VAP的个数 */
    /* END:   Added by zhanghaozhong, 2013/5/22   PN:NULL */
}OM_TR69C_WLAN_CODE_E;




/**************************************  lte相关   **************************************************/
typedef enum 
{
    TR69C_LTE_RADIO_SIGNAL_STRENGTH = 0,
    TR69C_LTE_TXPOWER,
    TR69C_LTE_IMEI,
    TR69C_LTE_IMSI,
    TR69C_LTE_RSRP,
    TR69C_LTE_RSRQ,
    TR69C_LTE_SINR,
    TR69C_LTE_RSSI,
    TR69C_LTE_BAND,
    TR69C_LTE_BANDWIDTH,
    TR69C_LTE_SIM_STATE,
    TR69C_LTE_RADIO_MODE,
    TR69C_LTE_CELL_LOCKING,
    TR69C_LTE_IS_CELL_LOCK,
    TR69C_LTE_FREQUENCY_ISLOCK,
    TR69C_LTE_FREQUENCY_LOCKING,
    TR69C_LTE_FREQUENCY_LOCKING_INFO,
    TR69C_LTE_RADIO_FRENQUENCY,
    TR69C_LTE_REGISTRATION_STATUS,
    TR69C_LTE_NET_REG_STATUS,
    TR69C_LTE_GPRS_REG_STATUS,
    TR69C_LTE_EPS_REG_STATUS,
    TR69C_LTE_RADIO_INTERFERENCE,
    TR69C_LTE_NETWORK_NAME,
    TR69C_LTE_ENODEBID,
    TR69C_LTE_GLOBE_CELLID,
    TR69C_LTE_PHYSICS_CELLID,
    TR69C_LTE_ICCID,
    TR69C_LTE_CELLID,
    TR69C_LTE_APN_STATUS,
    TR69C_LTE_IN_NETWORK_STATE,
	TR69C_LTE_X_CMCC_CellLockType,
	TR69C_LTE_X_CMCC_CellLockInfo,
	TR69C_LTE_X_CMCC_LockPin,
	TR69C_LTE_X_CMCC_LockPinType,
	TR69C_LTE_X_CMCC_FirstPin,
	TR69C_LTE_X_CMCC_FixedPin,
	TR69C_LTE_X_CMCC_NetworkPriority,
	TR69C_LTE_X_CMCC_EncryptCard,
	TR69C_LTE_X_CMCC_EncryptCardKey,
	   
}OM_TR69C_LTE_CODE_E;


typedef struct 
{
    INT32 RadioSignalStrength;     //信号服务质量
    INT32 Txpower;
    CHAR  IMEI[32];
    CHAR  IMSI[32]; 
    INT32 RSRP;
    INT32 RSRQ;
    INT32 SINR;
    INT32 RSSI;
    CHAR  Band[32];               //频段
    INT32 BandWidth;          //带宽，MHz
    CHAR  SIMState[16];
    CHAR  RadioMode[16];          //广播模式
    CHAR  IsCellLock[32];        //锁小区开关
    CHAR  CellLocking[32];        //锁扇区信息
    INT32 IsFrequencyLock;				//是否锁频
    CHAR  FrequencyLocking[64];   		//配置的锁频信息
    CHAR  FrequencyLockingInfo[64];   	//已经锁定的频率
    CHAR  RadioFrenquency[32];
    CHAR  RegistrationStatus[32];
    CHAR  NetRegStatus[32];     //CS网络注册状态
    CHAR  GprsRegStatus[32];    //GPRS网络注册状态
    CHAR  EpsRegStatus[32];     //EPS网络注册状态
    CHAR  RadioInterference[16];    
    CHAR  NetworkName[32];
    CHAR  EnodeBId[32];
    CHAR  GlobeCellId[32];      //全局cell id
    CHAR  PhysicsCellId[32];    //物理cell id
    CHAR  Iccid[32];            //ICCID
    CHAR  CellID[32];           //CELL ID
    CHAR  APNStatus[16];        //APN
   	INT32 InNetWorkState;   //在网状态 
    CHAR  X_CMCC_CellLockType[32];
	CHAR  X_CMCC_LockPin[32];
	CHAR  X_CMCC_LockPinType[32];
	CHAR  X_CMCC_CellLockInfo[64];
	CHAR  X_CMCC_FirstPin[32];
	CHAR  X_CMCC_FixedPin[32];
	UINT32	X_CMCC_NetworkPriority;
	INT32 X_CMCC_EncryptCard;
	CHAR  X_CMCC_EncryptCardKey[32];
}OM_TR69C_LTE_STATE_T;

typedef struct 
{
    OM_TR69C_LTE_CODE_E eFlag;
    OM_TR69C_LTE_STATE_T stLteState;
}OM_TR69C_LTE_STATE_PARAM_T;


/**************************************  ping Diagnostic相关   ***************************************/
#define PING_REQUEST  "Requested"
#define PING_COMPLETE   "Complete"
#define PING_ERROR_DNS  "Error_CannotResolveHostName"
#define PING_ERROR_INTERNAL "Error_Internal"
#define PING_ERROR_OTHER    "Error_Other"

typedef enum 
{
    TR69C_PING_DIAGNOSTICS_STATE = 0,
    TR69C_PING_INTERFACE,
    TR69C_PING_HOST,
    TR69C_PING_NUM_REPETITIONS,
    TR69C_PING_TIMEOUT,
    TR69C_PING_DATA_BLOCK_SIZE,
    TR69C_PING_DSCP,
    TR69C_PING_SUCCESS_COUNT,
    TR69C_PING_FAIL_COUNT,
    TR69C_PING_AVER_RESP_TIME,
    TR69C_PING_MIN_RESP_TIME,
    TR69C_PING_MAX_RESP_TIME,
}OM_TR69C_PING_DIAG_CODE_E;

typedef enum 
{
    TR69C_L2TP_ENABLE = 0,
	TR69C_L2TP_MTU,	
	TR69C_L2TP_SERVERNUMBER,
	TR69C_L2TP_DEVICE,
	TR69C_L2TP_TUNNELMODE,
	TR69C_L2TP_LNSSERVERIPADDR,
	TR69C_L2TP_LNSSERVERHOSTNAME,
	TR69C_L2TP_LNSSERVERPORTNAME,
	TR69C_L2TP_TUNNELAUTHENTICATEENABLE,
	TR69C_L2TP_PASSWORDOFTUNNEL,
}OM_TR69C_L2TP_CODE_E;

typedef struct 
{
    CHAR DiagnosticsState[64];
    CHAR Interface[256];
    CHAR Host[256];
    UINT32 NumberOfRepetitions;
    UINT32 Timeout;
    UINT32 DataBlockSize;
    UINT32 DSCP;
    UINT32 SuccessCount;
    UINT32 FailureCount;
    UINT32 AverageResponseTime;
    UINT32 MinimumResponseTime;
    UINT32 MaximumResponseTime;
}OM_TR69C_PING_DIAG_T;

typedef struct 
{
    OM_TR69C_PING_DIAG_CODE_E eFlag;
    OM_TR69C_PING_DIAG_T stPingState;
}OM_TR69C_PING_DIAG_PARAM_T;

typedef struct 
{
    UINT8   aucModuleType[32];
    UINT8   aucModuleVer[128];
    UINT8   aucModuleIMEI[32];
}OM_TR69C_LTE_MODULE_INFO_T;



#if 0    
typedef struct 
{
    OM_TR69C_NTP_CODE_E  eFlag;    
    OM_APP_SYSMNT_NTP_INFO_T stNtpInfo;
}OM_TR69C_NTP_PARAM_T;
/* lan流量统计 */
typedef struct 
{
    OM_NET_TS_LIST_T stTsList;
}OM_TR69C_LAN_STATS_T;

typedef struct 
{
    OM_TR69C_LAN_DHCP_CODE_E eFlag;
    OM_NET_DHCP_CFG_T stDhcpCfg;
}OM_TR69C_LAN_DHCP_PATAM_T;

typedef struct 
{
    OM_TR69C_LAN_CODE_E  eFlag;    
    OM_NET_WIRED_LAN_CFG_T stLanCfg;
}OM_TR69C_LAN_PARAM_T;
typedef struct 
{
    CHAR acLastErr[CHAR_LEN_64]; //最后一次导致不能连接建立的原因
    OM_NET_NAT_CFG_T stNatCfg;
    UINT8 aucStaticIp[4];
    UINT8 aucMask[4];
    UINT8 aucGw[4];
    UINT8 aucDnsSvr1[4];
    UINT8 aucDnsSvr2[4];
    UINT8 aucMac[6];
}OM_TR69C_WAN_IP_CONNECT_CFG_T;
typedef struct 
{
    OM_TR69C_WAN_IP_CONNECT_CODE_E eFlag;
    OM_TR69C_WAN_IP_CONNECT_CFG_T stWanIpConnectCfg;
}OM_TR69C_WAN_IP_CONNECT_T;
#endif
/**************************************  安全相关   **************************************************/

/*****************************************************************************************************/
/**************************************  函数声明   **************************************************/
/*****************************************************************************************************/
#if 0
REMOTE OM_ERROR_CODE_E  om_tr69c_get_devinfo( OM_TR69C_DEVICE_PARAM_T *pstDevParam );
REMOTE OM_ERROR_CODE_E  om_tr69c_set_devinfo( OM_TR69C_DEVICE_PARAM_T *pstDevParam );
REMOTE OM_ERROR_CODE_E  om_tr69c_set_acsinfo( OM_TR69C_APP_PARAM_T *pstConnParam);
REMOTE OM_ERROR_CODE_E  om_tr69c_get_acsinfo( OM_TR69C_APP_PARAM_T *pstConnParam );
REMOTE OM_ERROR_CODE_E  om_tr69c_config_modify( OM_APP_RMTMGR_CFG_T *pstRmtMgrCfgOld,OM_APP_RMTMGR_CFG_T *pstRmtMgrCfgNew);
REMOTE OM_ERROR_CODE_E  om_tr69c_get_ntp_info( OM_TR69C_NTP_PARAM_T *pstNtpCfg  );
REMOTE OM_ERROR_CODE_E  om_tr69c_set_ntp_info( OM_TR69C_NTP_PARAM_T *pstNtpCfg );

REMOTE OM_ERROR_CODE_E  om_net_tr069_lan_stats( OM_TR69C_LAN_STATS_T *pstTsStats);
REMOTE OM_ERROR_CODE_E  om_net_tr069_wan_stats( OM_TR69C_WAN_STATS_T *pstTsStats);

REMOTE OM_ERROR_CODE_E  om_net_tr069_dhcp_set( OM_TR69C_LAN_DHCP_PATAM_T *pstDHCPInfo );
REMOTE OM_ERROR_CODE_E  om_net_tr069_dhcp_get( OM_TR69C_LAN_DHCP_PATAM_T *pstDHCPInfo );


REMOTE OM_ERROR_CODE_E  om_net_tr069_lan_config_serurity_get( OM_TR69C_LAN_CONFIG_SECURITY_T *pstLanCfgSec);
REMOTE OM_ERROR_CODE_E  om_net_tr069_lan_config_serurity_set( OM_TR69C_LAN_CONFIG_SECURITY_T *pstLanCfgSec);

REMOTE OM_ERROR_CODE_E  om_net_wan_IP_connect_get( OM_TR69C_WAN_IP_CONNECT_T *pstWanIpConnect );
REMOTE OM_ERROR_CODE_E  om_net_wan_IP_connect_set( OM_TR69C_WAN_IP_CONNECT_T *pstWanIpConnect );

REMOTE OM_ERROR_CODE_E  om_net_tr069_ping_diag_set( OM_TR69C_PING_DIAG_PARAM_T *pstPingParam );
REMOTE OM_ERROR_CODE_E  om_net_tr069_ping_diag_get( OM_TR69C_PING_DIAG_PARAM_T *pstPingParam );
REMOTE OM_ERROR_CODE_E  om_lte_tr69_module_info_get(OM_TR69C_LTE_MODULE_INFO_T *trLteModule);
REMOTE OM_ERROR_CODE_E  om_lte_tr69_wanconnectIP_ip_get(UINT8 acIpv4_tmp[4]);
#endif
#endif /* __OM_TR69C_H__ */
