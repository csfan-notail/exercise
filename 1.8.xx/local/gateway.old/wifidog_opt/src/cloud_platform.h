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
#define MUSTREDIRECTURLLIST "MustRedirectUrlList"  /**add@2014.11.02 ����Ҫ�ض���url����*/
/* add@2015.01.28 for wechat connect wifi*/
#define WXLSERVERIP "WXLServerIP"

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

#define TPL_SRC_FILE        "cmps/static/themes/"             /* ģ���ŵ�λ�� */
#define PRODUCT_SRC_FILE    "cmps/static/attach/"
#define AD_SRC_FILE         "cmps/static/uploads/"

typedef struct _cloud_src_ver_t{
    int iTpl_ver;       //ģ��汾��
    int iProduct_ver;   //��Ʒ�汾
    int iAd_ver;        //��˾�汾
}cloud_src_ver_t;

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
    EVENT_SYNC_AUTHSTA_LIST,
    EVENT_SYNC_STA_KEY,
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
extern void first_client_request(void);
extern int download_src();
extern void read_src_loacl_ver();
extern int auth_mode_support_wxl(void);
extern int auth_mode_support_weixin(void);

extern void get_local_config();

#endif /* __CLOUD_PLATFORM_H__ */
