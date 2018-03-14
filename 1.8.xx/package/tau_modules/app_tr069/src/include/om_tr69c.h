/******************************************************************************

                  ��Ȩ���� (C), 2010-2012, �Ϻ�徴�ͨ�ſƼ����޹�˾

 ******************************************************************************
  �� �� ��   : om_tr69c.h
  �� �� ��   : ����
  ��    ��   : zhangjiasi
  ��������   : 2015��5��20��
  ����޸�   :
  ��������   : ��������������tr069ͨ����صĲ���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��5��20��
    ��    ��   : zhangjiasi
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __OM_TR69C_H__
#define __OM_TR69C_H__
/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "ap_types.h"

/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
#ifndef TR69C_DEFAULT_CONFIG_FILE
#define TR69C_DEFAULT_CONFIG_FILE     "/tmp/default_config"   //ACS�·���ȱʡ�����ļ�,tr69c����ֵ���õ�nvram�У�Ȼ��������
#endif

#ifndef RECOVER_DEFAULT_CONFIG
#define RECOVER_DEFAULT_CONFIG  1       //��default_config�ļ��ָ�����
#endif

#ifndef TR69C_SOFTWARE_FILE
#define TR69C_SOFTWARE_FILE     "/tmp/app.bin"   //ACS�·��İ汾,tr69c���汾д��flash�У�Ȼ��������
#endif

/* 
	add by duanguiyuan 20161125 
	��ӻ�Ϊ���Ƶ� �澯�ϱ� ��Ϣ �� �ṹ�嶨��
*/
/* �澯�ϱ��� �����ڵ����� */
#define TR69C_ALARMCODE_LAN "240010"
/* �澯�ϱ��� ״̬  1 �ϱ��µĸ澯   2 ���֮ǰ�ĸ澯 */
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
    OM_TR69C_ALARM_STATUS_E alarm_flag; /* �Ƿ� ��Ҫ �ϱ� �澯��Ϣ*/
	OM_TR69C_ALARM_STATUS_E alarm_status[TR69C_ALARM_TYPE_END];
}OM_TR69C_ALARM_INFO_T;

/* �澯msg�ṹ�� */
typedef struct {
    UINT32 uiAlarmCode;           /* �澯�� ������ʶ�澯������ ��1001��ʼ */
    UINT32 uiAlarmReasonCode;     /* �澯ԭ���� ÿһ�����ֶ�Ӧһ��������ԭ�� ��0��ʼ����ͬ�ĸ澯���Ӧ��ͬ�ĸ澯ԭ���� */
    UINT32 uiAlarmState;          /* �澯״̬ �澯�ϱ�0��澯���1 */
    CHAR  acAlarmObject[32];  /* �澯�������豸��SN���ɣ����԰�SN�Ų鿴�澯 */
    CHAR  acAlarmInfo[128];   /* �澯������Ϣ ��:cpu������90%*/
}OM_TR69C_ALARM_MSG_T;

/* CGI���͸�TR69C���¼���� ��tr69c��eCFMMsg�Ķ���һ��Ҫ��Ӧ����*/
typedef enum {
    TR69C_PARAM_CHANGE=0,
    TR69C_SEND_GETRPC,
    TR69C_SEND_DIAG_COMPLETE,
    TR69C_SEND_ENABLE_INFORM,
    TR69C_SEND_DISABLE_INFORM,
    TR69C_SEND_ACS_URL_CHANGED, /* ACS��URL���ı� */
    TR69C_SEND_WAN_CHANGED,     /* WAN�ڵ�IP��ַ���ı� */
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
    /* BEGIN: Added by zhanghaozhong, 2013/5/29   PN:���Ӹ澯�ϱ����� */
    TR69C_ALARM,
    /* END:   Added by zhanghaozhong, 2013/5/29   PN:���Ӹ澯�ϱ����� */
    TR69C_SIMPLE_LOG,
    TR69C_DETAIL_LOG,
}OM_TR69C_SEND_MSG_E;

/* msg�ṹ�� */
typedef struct {
    OM_TR69C_SEND_MSG_E eFlag;           
    OM_TR69C_ALARM_MSG_T stAlarm;
}OM_TR69C_UDP_MSG_T;

/**************************************  tr069�������   **************************************************/
typedef struct {
    /* tr69c״̬��Ϣ */
    CHAR acAcsIP[CHAR_LEN_32];
    UINT32 uiInformState;
    UINT32 uiConnectState;
    UINT32 uiLastCfgState;
} OM_TR69C_STATUS_T;

/* tr69c������ */
typedef enum 
{
    TR069_ENABLE,
    
    TR069_ACS_URL,
    TR069_ACS_USRNAME,
    TR069_ACS_PASSWD,

    TR069_ECHO_ENABLE,
    TR069_ECHO_TIME,
    TR069_PERIOD_INFORM_TIME,
    /* acs��������ʱ����ز��� */
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
    /* ACS��CPE֮������Ĳ��� */
    INT32       EnableCWMP;                         /* �Ƿ�ʹ��tr69c���� */
    CHAR        acsURL[CHAR_LEN_128];        /* URL of ACS */
    UINT32      uiReGetUrlCnt;                      /* ������ACS��URL�Ĵ������״�����ʹ��option43
                                                       ��������URL�����û�У���ʹ��Ĭ�ϵ�URL����
                                                       �Σ�option43��URL������10��ʧ�ܺ�,�л���Ĭ
                                                       �ϡ�����״���Ĭ�ϵ�URL�������GETACSURL����������URL
                                                       ����Լ�����п��ܻ����option43*/
    CHAR        acsNewURL[CHAR_LEN_128];     /* New URL if URL has been changed*/
    CHAR        acsUser[CHAR_LEN_64];        /* ACS���û��������� */
    CHAR        acsPwd[CHAR_LEN_64];   
    
    INT32       PeriodicInformEnable;               /* �Ƿ��ڷ���inform���ĸ�ACS */
    time_t      informInterval;                     /* inform interval ��inform��ʱ����*/

    INT32       iConnReqEnable;                             /* �Ƿ�ʹ����ACS����CPE */
    CHAR        connReqURL[CHAR_LEN_128];            /* CPE��URL���û��������� */
    CHAR        connReqUser[CHAR_LEN_64];
    CHAR        connReqPwd[CHAR_LEN_64];    

    CHAR        newParameterKey[CHAR_LEN_32];  /* the pending key */                       /* save for GBCOM */
    CHAR        rebootCommandKey[CHAR_LEN_32]; /* key for reboot command key */           /* save for GBCOM */
    CHAR        downloadCommandKey[CHAR_LEN_32];     /* key for download cmd*/               /* save for GBCOM */
    

    INT32       upgradesManaged;    /* �Ƿ�֧��ACS��CPE�����汾����Ϊ�棬����ʹ��ACS������
                                        ��Ϊ�٣���CPE���Կ���ʹ��������ʽ��������WEB */  

    CHAR        UDPConnReqAddr[CHAR_LEN_128];/* acs��������cpe�ĵ�ַ�Ͷ˿� */
    UINT32      UDPConnReqAddrNotifynLimit; /* ActiveNotification���ĵ��µ�
                                               ��ַʱ���м�ȴ�����Сʱ�䣬
                                               ��λ���� */

    /*****    STUN ���    *******/
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
    time_t      startDLTime;    /* ���ؿ�ʼʱ�� */
    time_t      endDLTime;      /* ���ؽ���ʱ�� */
    INT32       contactedState; /* add by zhanghaozhong 2013-1-20 */
    UINT32      iTr69cPort;
    UINT32      uiKeepAliveTime;
    //INT32       lastInstaceID;  /* ���ʵ��ID�� */
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


/* �豸��ز��� */
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
    OM_TR69C_DEVICE_OPERATE_CODE_E eFlag;                        /* ��ȡ��Ϣ�ı�־ */
    CHAR        acDevName[CHAR_LEN_64];   /* ������ */
    CHAR        Manufacturer[CHAR_LEN_64];   /* ������ */
    CHAR        ManufacturerOUI[6]; /* ����OUI */
    CHAR        ModelName[CHAR_LEN_64];      /* CPE�ͺ����� */
    CHAR        Description[CHAR_LEN_64];    /* CPE�豸���������� */
    CHAR        ProductClass[CHAR_LEN_64];   /* ָ����Ʒ���к��������Ĳ�Ʒ��� */
    CHAR        SerialNumber[CHAR_LEN_64];   /* CPE�����к� */
    CHAR        acCpeMac[CHAR_LEN_64];       /* CPE��MAC */
    CHAR        acHardVer[CHAR_LEN_64];		//Ӳ���汾
    CHAR        acSoftVer[CHAR_LEN_64];		//����汾	
    CHAR        acPriSoftVer[CHAR_LEN_64];		//˽������汾
    CHAR        acModemFirmwareVer[CHAR_LEN_64];    //boot�汾
    CHAR        ProvisioningCode[CHAR_LEN_64];   /* �������룬����optio43�ṩ */
    UINT32      UpTime;                                 /* CPE�ϴ������������������� */
	CHAR        Username[CHAR_LEN_32];   /* ά���˺ŵ��û��� */
	CHAR        Password[CHAR_LEN_64];   /* ά���˺ŵ����� */
}OM_TR69C_DEVICE_PARAM_T;

/* ntp��Ϣ */
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



/**************************************  wan���   **************************************************/
typedef struct
{
    CHAR   acDevName[CHAR_LEN_32];        //�豸��
    CHAR   acForwardIp[CHAR_LEN_32];      //��������INTERNET��ip��ַ        
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


/**************************************  lan���   **************************************************/

typedef enum 
{
    TR69C_LAN_ENABLE = 0,
    TR69C_LAN_IP_ADDR_SET,
    TR69C_LAN_IP_ADDR_GET,
    TR69C_LAN_IP_NETMASK_SET,
    TR69C_LAN_IP_NETMASK_GET,
    TR69C_LAN_IP_ADDR_TYPE_SET,
    TR69C_LAN_IP_ADDR_TYPE_GET,     /* Ĭ��Ϊstatic���������� */
    
}OM_TR69C_LAN_CODE_E;

typedef enum 
{
    TR69C_LAN_DHCP_ENABLE = 0,      /* ����dhcpģʽ */
    TR69C_LAN_DHCP_MIN_ADDR,
    TR69C_LAN_DHCP_MAX_ADDR,
    TR69C_LAN_DHCP_RESERVED_ADDR,   /* ������ַ */
    TR69C_LAN_DHCP_NETMASK,
    TR69C_LAN_DHCP_DNS_SRV_MASTER,     /* ���dns��ַ��","���� */
    TR69C_LAN_DHCP_DNS_SRV_SLAVE,
    TR69C_LAN_DHCP_DOMAIN,
    TR69C_LAN_DHCP_IPROUTERS,   /* Ĭ������ */
    TR69C_LAN_DHCP_LEASE_TIME,
    TR69C_LAN_MAC_ADDR,         /* lan mac address */
    TR69C_LAN_DHCP_CONFIGURABLE,    /* dhcp server����ʹ�ܡ�TrueΪ��Ч�����
                                       ����Ϊfalse����cpe���ָ���Ĭ������ */
    TR69C_LAN_DHCP_RELAY,
    TR69C_LAN_ALLOWED_MAC_ADDR,     /* ��������mac�б� */
}OM_TR69C_LAN_DHCP_CODE_E;

typedef struct 
{
    CHAR acLanCfgPasswd[CHAR_LEN_64]; //����lan�ڽ���ACS������������
}OM_TR69C_LAN_CONFIG_SECURITY_T;

/**************************************  wlan���   **************************************************/
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
    TR69C_VAP_SET_AUTH_MODE_CMD ,        /*���ü��ܺ����֤ģʽ��������ģʽ*/
    TR69C_VAP_SET_DATA_ENCRYPT_CMD ,     /*�������ݼ��ܱ�־λ*/
    TR69C_VAP_SET_WEPKEY_LEN_CMD ,       /*����WEP��Կ����*/
    TR69C_VAP_SET_WEPKEY_FORMAT_CMD ,    /*����WEP��Կ��ʽ:ASCII or ʮ������*/
    TR69C_VAP_SET_WEPKEY_INDEX_CMD ,     /*����WEP��Կ���*/
    TR69C_VAP_SET_WEPKEY_CMD ,           /*WEP��Կ*/
    TR69C_VAP_SET_WPATYPE_CMD ,          /*���ü����㷨(tkip or aes)*/
    TR69C_VAP_SET_WPA_RENEW_CMD ,        /*����WPA��Կ��������*/
    TR69C_VAP_SET_WPA_KEY_CMD ,          /*WPA��Կ*/
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
    TR69C_VAP_GET_AUTH_MODE_CMD ,        /*��ȡ���ܺ����֤ģʽ��������ģʽ*/
    TR69C_VAP_GET_DATA_ENCRYPT_CMD ,     /*��ȡ���ݼ��ܱ�־λ*/
    TR69C_VAP_GET_WEPKEY_LEN_CMD ,       /*��ȡWEP��Կ����*/
    TR69C_VAP_GET_WEPKEY_FORMAT_CMD ,    /*��ȡWEP��Կ��ʽ:ASCII or ʮ������*/
    TR69C_VAP_GET_WEPKEY_INDEX_CMD ,     /*��ȡWEP��Կ���*/
    TR69C_VAP_GET_WPATYPE_CMD ,          /*��ȡ�����㷨(tkip or aes)*/
    TR69C_VAP_GET_WPA_RENEW_CMD ,        /*��ȡWPA��Կ��������*/
    TR69C_WIFI_GET_VAP_NUM ,             /* VAP�ĸ��� */
    /* END:   Added by zhanghaozhong, 2013/5/22   PN:NULL */
}OM_TR69C_WLAN_CODE_E;




/**************************************  lte���   **************************************************/
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
    INT32 RadioSignalStrength;     //�źŷ�������
    INT32 Txpower;
    CHAR  IMEI[32];
    CHAR  IMSI[32]; 
    INT32 RSRP;
    INT32 RSRQ;
    INT32 SINR;
    INT32 RSSI;
    CHAR  Band[32];               //Ƶ��
    INT32 BandWidth;          //����MHz
    CHAR  SIMState[16];
    CHAR  RadioMode[16];          //�㲥ģʽ
    CHAR  IsCellLock[32];        //��С������
    CHAR  CellLocking[32];        //��������Ϣ
    INT32 IsFrequencyLock;				//�Ƿ���Ƶ
    CHAR  FrequencyLocking[64];   		//���õ���Ƶ��Ϣ
    CHAR  FrequencyLockingInfo[64];   	//�Ѿ�������Ƶ��
    CHAR  RadioFrenquency[32];
    CHAR  RegistrationStatus[32];
    CHAR  NetRegStatus[32];     //CS����ע��״̬
    CHAR  GprsRegStatus[32];    //GPRS����ע��״̬
    CHAR  EpsRegStatus[32];     //EPS����ע��״̬
    CHAR  RadioInterference[16];    
    CHAR  NetworkName[32];
    CHAR  EnodeBId[32];
    CHAR  GlobeCellId[32];      //ȫ��cell id
    CHAR  PhysicsCellId[32];    //����cell id
    CHAR  Iccid[32];            //ICCID
    CHAR  CellID[32];           //CELL ID
    CHAR  APNStatus[16];        //APN
   	INT32 InNetWorkState;   //����״̬ 
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


/**************************************  ping Diagnostic���   ***************************************/
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
/* lan����ͳ�� */
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
    CHAR acLastErr[CHAR_LEN_64]; //���һ�ε��²������ӽ�����ԭ��
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
/**************************************  ��ȫ���   **************************************************/

/*****************************************************************************************************/
/**************************************  ��������   **************************************************/
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
