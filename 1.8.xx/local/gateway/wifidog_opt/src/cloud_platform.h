/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : cloud_platform.h
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/12/5
  Last Modified :
  Description   : cloud_platform.c header file
  Function List :
  History       :
  1.Date        : 2013/12/5
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#ifndef __CLOUD_PLATFORM_H__
#define __CLOUD_PLATFORM_H__

/** ping */
#define CLIENT_NUM  "client_num="
#define CLIENT_LIST "client_list="
#define CONF_VER    "conf_ver="
#define CMD_VER     "cmd_ver="
#define TPL_VER     "res_tpl_version="
#define PROD_VER    "res_product_version="
#define AD_VER      "res_adver_version="
#define SOFT_VER  "soft_ver="
#define REBOOT_DELAY_TIME "reboot_delay_time="
/*add@2014.10.11*/
#define USER_VER	"userversion="
#define TOTAL_USER_NUM "totalusernum="

/** conf */
#define QOSENABLE   "QosEnable "                         /** ���ؿ��� */
#define QOSUP		"QosUp"								 /** up max band*/
#define QOSDOWN		"QosDown"							 /** down max band*/
#define TRUSTEDMACLIST "TrustedMACList "                 /** MAC ������ */
#define TRUSTEDIPLIST "TrustedIPList "                   /** IP ������ */
#define TRUSTEDWANHOSTLIST "TrustedWANHOSTList "         /** ��֤ǰ URL ������ */
#define POSTTRUSTEDWANHOSTLIST "PostTrustedWANHOSTList " /** ��֤�� URL ������ */
#define BLACKIPLIST "BlackIPList "                       /** IP ������ */
#define BLACKNETLIST "BlackNETList "                     /** ���� ������ */
#define BLACKWANHOSTLIST "BlackWANHOSTList "             /** URL ������ */
#define BLACKMACLIST "BlackMACList "                     /** MAC ������ */
#define FREEAUTHMACLIST "FreeAuthMACList "               /** MAC ����֤ */
#define FREEAUTHIPLIST  "FreeAuthIPList "                /** IP ����֤ */
#define MAXSTATIONNUM "MaxStationNum "                   /** �������û��� :������*/
#define MAXDEVSTATIONNUM "MaxDevStationNum "                   /** �������û��� :��Ʒ��λ���*/
#define FORCETIMEOUT  "ForceCutTime "					 /** Ӳ�ԶϿ�ʱ�� */
#define IDLETIMEOUT  "IdleCutTime "					     /** ���жϿ�ʱ�� */
#define WANFLOWSTATSWITCH "WanFlowStatSwitch"            /**add@2014.8.26 wan�������ϱ����أ�Ĭ�Ϲر�*/
/* BEGIN: Added by WeiHonggang, 2014/2/18   PN:TASK#0ƽ̨֧����֤ģʽͬ�������أ���������֤ģʽ�������� */
#define AUTHMODE "auth_mode" /*��֤��ʽ*/
#define STADCTENABLE "StadctEnable " /*�ն�ɨ��ʹ��*/
#define STADCTRSSI "StadctRSSI " /*�ն˸�֪��ֵ*/
#define STADCTRPTTIME "StadctRptTime " /*ɨ���ϱ�����*/
#define STADCTQUITTIMEOUT "StadctQuitTimeout"    /*add 2015-10-13 for�ն�ɨ������ʱ��*/
#define MUSTREDIRECTURLLIST "MustRedirectUrlList"  /**add@2014.11.02 ����Ҫ�ض���url����*/
/* add@2015.01.28 for wechat connect wifi*/
#define WXLSERVERIP "WXLServerIP"
#define IBEACONSETUP "iBeaconSetup"    /*add 2015-11-3 for ibeacon����*/


#define MACRO_HOTSPOT "Macro_hotspot"    /*add 2015-8-26 for ��������*/

#define CLOUD_SPAN_SSID_AUTH "span_ssid_auth"

typedef enum {
	CONFIG_CHANGE_BW_UP = 0,
	CONFIG_CHANGE_BW_DOWN,
	CONFIG_CHANGE_FORCETIMEOUT,
	CONFIG_CHANGE_IDLETIMEOUT,
	MAX_GLOBAL_CONFIG_TO_CLIENT_NUM
}en_global_config_to_client_type;


#define URL_LEN_MAX 64
typedef enum
{
    AUTH_MODE_FREE = 0,
    AUTH_MODE_USER,
    AUTH_MODE_MES,
    AUTH_MODE_QQ,
    AUTH_MODE_WEIBO,
    AUTH_MODE_WEIXIN,
    AUTH_MODE_ALIPAY, /*add@2014.10.11 add auth mode: alipay*/
    AUTH_MODE_WXL,    /*add@2015.01.28 for wechat connect wifi*/
    AUTH_MODE_MAX,
}en_auth_mode;
typedef enum
{
    AUTH_MODE_NOT_SUPPORT = 0,
    AUTH_MODE_SUPPORT,   
}en_auth_mode_support;
typedef struct
{
    unsigned char url[URL_LEN_MAX];
}st_url;
/* END:   Added by WeiHonggang, 2014/2/18   PN:TASK#0ƽ̨֧����֤ģʽͬ�������أ���������֤ģʽ�������� */

/** client */
#define CLIENT_COUNT "count="
#define CLIENT_BEGIN "begin="

/* portal �³����� */
#define CLOUD_SRC_VER_STORE_FILE      "/tmp/cloud_src_ver"      /* �洢portal�³�������Դ�İ汾�� */
/* ע���ļ�·�����Ҫ��б�� */
#if (defined(GW_X86) || defined(GW_ARM) || defined(GW_AC))   //by cj
#define LIGHTTPD_ROOT_DOC   "/appfs/web/webpages/"     /* lighttpd�����ҳ�ĸ�Ŀ¼ */
#else
#define LIGHTTPD_ROOT_DOC   "/tmp/app/home/cloud/webpages/"     /* lighttpd�����ҳ�ĸ�Ŀ¼ */
#endif

#define CLOUD_RESOURCE_PATH	"cmps/static/"
#define TPL_SRC_FILE        "themes"
#define PRODUCT_SRC_FILE    "attach"
#define AD_SRC_FILE         "uploads"

typedef struct _cloud_src_ver_t{
    int iTpl_ver;       //ģ��汾��
    int iProduct_ver;   //��Ʒ�汾
    int iAd_ver;        //��˾�汾
}cloud_src_ver_t;

typedef enum _en_cloud_soft_ver
{
    GW_CLOUD_UPDATE_IDLE=0,
    GW_CLOUD_UPDATE_GET_RIGHT_DATA,
    GW_CLOUD_UPDATE_DELAY_RUN,
}CLOUD_SOFT_VER_EN;

typedef struct _cloud_soft_ver_t
{
    char aucSoftUrl[512];
    char aucSoftVer[8];
    char aucSoftName[64];
    int iDelayMin;/*0:�������� >0:�ӳٺ�����*/
    long iRunTime;
    CLOUD_SOFT_VER_EN ucFlag;/*0:���п��Խ����µĲ���1:��ȷ���յ���������2:�ϴ��������Եȴ�ִ��*/
}cloud_soft_ver_t;

/**@ �¼���� */
typedef enum _en_event_code{
    EVENT_PING, /** ping */
    EVENT_DENY_CLIENT,  /** �����̻��߳�ʱ���� */
    EVENT_REQUEST_CLIENT_LIST, /** ��ȡ�ն��б� */
    EVENT_REPORT_CLIENT, /** �ն���Ϣ�ϱ� */
    EVENT_REQUEST_CONFIG, /** �������� */
    EVENT_REPORT_AP, /*AP��Ϣ�ϱ�*/
    EVENT_REPORT_STADCT, /*�ն�̽����Ϣ�ϱ�*/
    EVENT_REPORT_WAN_FLOWING, /*add@2014.8.25 ����wan������ͳ���ϱ�*/
    EVENT_DOWNLOAD_WXL_WHITELIST, /*΢������������������*/
    EVENT_CLIENT_SYN_HANDLE,
    EVENT_STALIST_SYN,
    EVENT_MAX,
}en_event_code;

typedef struct _event_map {
    int time_interval; /**@ �¼��������ʱ�� */
    int timeout_count; /**@ ��ʱ���� */
    int count;         /**@ ִ�д���, 0��ʾһֱִ�� */
    en_event_code event_code;    /**@ �¼���� */
    //time_t last_time;  /**@ �ϴδ����¼� */
    long last_time;  /**@ �ϴδ����¼�ʱ�� */
    void (*request_func)(); /**@ �����¼��ĺ���ָ�� */
    void (*response_func)();/**@ ����ظ��¼�����ָ�� */
	
    struct _event_map *next;
}event_map;
/*add@2014.8.26 */
typedef struct _interface_flow_stats{
	unsigned long long wan_total_incoming;
	unsigned long long wan_total_outgoing;
	unsigned long long wan_last_report_incoming;
	unsigned long long wan_last_report_outging;
}interface_flow_stats_t;

extern void thread_platform(void);

extern int passive_user_logout(t_client *client);

extern int first_ping(void);
extern int first_client_request(void);
extern int download_src();
extern void read_src_loacl_ver();
#define OM_PORTMAP_MAX 32
/*add 2015-11-3 for ibeacon*/
typedef struct _ibeacon_parameter
{
    char ibeacon_switch[32];
    char equipment_id[32];
    char ibeacon_power[32];    
    char uuid[256];
    char major[32];
    char minor[32];
}ibeacon_parameter_t;

typedef enum {
	IBEACON_SWITCH = 0,
	IBEACON_NAME,        
	IBEACON_POWR,        
	IBEACON_PUID,
	IBEACON_MAJR,
	IBEACON_MINR,
	IBEACON_RESET//������Ҫʼ�ձ�����ö�ٵ����
}en_ibeacon_set;

#define AT_BCON     "AT+BCON"
#define AT_POWR     "AT+POWR"
#define AT_PUID     "AT+PUID"
#define AT_MAJR     "AT+MAJR"
#define AT_MINR     "AT+MINR"
#define AT_RESET    "AT+RESET"
#define AT_NAM      "AT+NAME"
#define COM_PATH    "/dev/ttyACM0"
#define LEVEL_0     "4dBm"
#define LEVEL_1     "0dBm"
#define LEVEL_2     "-6dBm"
#define LEVEL_3     "-23dBm"

#endif /* __CLOUD_PLATFORM_H__ */
