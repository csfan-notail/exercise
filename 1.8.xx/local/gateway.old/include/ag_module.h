/******************************************************************************

  Copyright (C), 2010-2011, GBCOM Co., Ltd.

 ******************************************************************************
  File Name     : ag_module.h
  Version       : Initial Draft
  Author        : lvjianlin
  Created       : 2014/11/17
  Last Modified :
  Description   : ag_module��ͷ�ļ�
  Function List :
  History       :
  1.Date        : 2014/11/17
    Author      : lvjianlin
    Modification: Created file

******************************************************************************/
#ifndef _AG_MODULE_H_
#define _AG_MODULE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/netlink.h>
#include <linux/if.h>
#include <linux/sockios.h>

#include <sys/un.h>
#include <syslog.h>
#include <linux/filter.h>

#include <config_common.h>


#include <fcntl.h>
#include <sys/mman.h>

#include "event.h"
#include "event_struct.h"
#include "event_compat.h"

#include "ag_common.h"
#include "list.h"

#include "log.h"
#include "nvram_config.h"


/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/
/* about module  */

/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/

#define AG_MODULE_IPC_SERVER_IP		"ag_module_ipc_ip"
#define AG_MODULE_IPC_SERVER_PORT	"ag_module_ipc_port"

#define AG_MODULE_IPC_DEFAULT_IP		"127.0.0.1"
#define AG_MODULE_IPC_DEFAULT_PORT	"300"

#define VAP_MAX_USER 128
#define STA_HASH_SIZE 			512


#define PID_FILE "/var/run/aglink.pid"
#define AG_MODULE_PRINTF	"/tmp/printf"
#ifdef GW_X86
#define IPC_CONFIG_FILE 	"/appfs/etc/ag_ipc.conf"
#elif defined OPENWRT
#define IPC_CONFIG_FILE 	"/etc/aglink/ag_ipc.conf"
#else
#define IPC_CONFIG_FILE 	"/etc/ag_ipc.conf"
#endif

#ifndef IP_FMT
#define IP_FMT	"%u.%u.%u.%u"
#endif
#ifndef IP_ARG
#define IP_ARG(x) ((u8*)(x))[0], ((u8*)(x))[1], ((u8*)(x))[2], ((u8*)(x))[3]
#endif

#define MODULE_NAME_LENGTH 32


#define AG_MODULE_ID_SHIFT	24
#define AG_GET_MODULE_ID(x)	((x)>>AG_MODULE_ID_SHIFT)

#define AG_IPC_MSG_TYPE_SELF_CPU		((u16) 1)
#define AG_IPC_MSG_TYPE_OTHER_CPU	((u16)2)

#if 0
#define AG_IPC_MSG_BEGIN			((u16)100)
#define AG_IPC_MSG_REGISTER 		((u16)AG_IPC_MSG_BEGIN + 0)
#define AG_IPC_MSG_REGISTER_ACK	((u16)AG_IPC_MSG_BEGIN + 1)
#endif

#define SOCKET_SEND_TIMEOUT_VALUE	10
#define AG_MODULE_BUFFER_SIZE			((u32)6144)

/*�����鲥��Ϣ*/
enum BC_MSG_ID
{
	BC_MSG_BEGIN=0x00000000,	//0
	BC_MSG_AP_REGISTER_TO_GW_SUCCESS,
	BC_MSG_MAX
};

enum AG_IPC_MSG_ID
{
	AG_MSG_IPC_BEGIN=0x01000000,	//16777216
	AG_MSG_IPC_REGISTER,
	AG_MSG_IPC_REGISTER_ACK,
	AG_MSG_IPC_UNREGISTER,
	AG_MSG_IPC_MAX
};

enum AGLINK_MSG_ID
{
	AGLINK_MSG_BEGIN=0x02000000,	//33554432
	AGLINK_MSG_DEV_REGISTER_MSG,
	AGLINK_MSG_DEV_REGISTER_ACK,
	AGLINK_MSG_DEV_NAK_MSG,		//33554435
	AGLINK_MSG_DEV_KEEPLIVE_MSG,
	AGLINK_MSG_DEV_KEEPLIVE_ACK_MSG,
	AGLINK_MSG_AP_INFO,
	AGLINK_MSG_DEV_REBOOT_MSG,	
	AGLINK_MSG_DEV_UPDATE_MSG,	//33554440
	AGLINK_MSG_DEV_CONFIG_MSG,	
	AGLINK_MSG_DEV_SSIDCFG_MSG,
	AGLINK_MSG_SYN_AP_LIST,
	AGLINK_CLI_BEGIN,
	AGLINK_CLI_GET_MODULE_INFO,	//33554445
	AGLINK_CLI_GET_ONLINE_INFO,	
	AGLINK_CLI_GET_AP_LIST,
	AGLINK_CLI_SET_KEEPLIVE_TM,
	AGLINK_CLI_MAX,
	AGLINK_MSG_MAX
};

enum STAM_MSG_ID
{
	STAM_MSG_BEGIN=0x03000000,	//50331648
	STAM_MSG_SW,
	STAM_MSG_MAX
};

enum WIFIDOG_MSG_ID
{
	WIFIDOG_MSG_BEGIN=0x05000000,	//83886080
	WIFIDOG_MSG_AP_REGISTER,
	WIFIDOG_MSG_AP_LOGOUT,
	WIFIDOG_MSG_ADD_STA,
	WIFIDOG_MSG_DEL_STA,
	WIFIDOG_MSG_SYN_ALL_STA, 	/*83886085 ����ͬ���ն���Ϣ10245*/
	WIFIDOG_MSG_STADCT_INFO,
	WIFIDOG_MSG_SNMPAGENT_SET_STA_AUTH,		/*�ն���֤��Ϣ*/
	WIFIDOG_MSG_SNMPAGENT_SET_AUTH_CONFIG,
	WIFIDOG_MSG_SNMPAGENT_GET_AUTH_CONFIG,
   	WIFIDOG_MSG_SNMPAGENT_SET_MAC_BAW_UPDATE, /*���Ƿ�ʽ����MAC�ڰ�����*/
	WIFIDOG_MSG_LOCATOR_WIFIDOG_ADD_STA,
	WIFIDOG_MSG_LOCATOR_WIFIDOG_DEL_STA,
	WIFIDOG_MSG_SNMPAGENT_ADD_BAW_URL,
    WIFIDOG_MSG_SNMPAGENT_DEL_BAW_URL,
	WIFIDOG_MSG_SNMPAGENT_ADD_BAW_MAC,
    WIFIDOG_MSG_SNMPAGENT_DEL_BAW_MAC,
	WIFIDOG_MSG_SNMPAGENT_SET_USER_TIMEOUT,
	WIFIDOG_MSG_SNMPAGENT_SET_NORMAL_FlOW_CTL,
	WIFIDOG_MSG_SNMPAGENT_SET_WHITE_FlOW_CTL,
	WIFIDOG_MSG_SNMPAGENT_SET_STA_KEY_INTVAL,
	WIFIDOG_MSG_MAX
};

enum AGLINKCLI_MSG_ID
{
	AGLINKCLI_MSG_BEGIN=0x06000000,	//100663296
	AGLINK_CLI_GET_MODULE_INFO_ACK,
	AGLINK_CLI_GET_ONLINE_INFO_ACK,
	AGLINK_CLI_GET_AP_LIST_ACK,
	AGLINK_CLI_SET_KEEPLIVE_TM_ACK,	//100663300
	AGLINKCLI_MSG_MAX
};
enum BOXLL_MSG_ID
{
	BOXLL_MSG_BEGIN=0x07000000,	//117440512
	BOXLL_MSG_AP_INFO,
	BOXLL_MSG_STA_INFO,
	BOXLL_MSG_MAX
};

#if 0
enum STADCT_MSG_ID
{
	STADCT_MSG_BEGIN=0x08000000,	//134217728
	STADCT_MSG_SW,
	STADCT_MSG_SCAN_INFO,
	STADCT_MSG_MAX
};
#endif
/* for locator module */
enum locator_idset{
	LOCATOR_IDSET_BEGIN=0x08000000,	//134217728
    	LOCATOR_IDSET_SW,
    	LOCATOR_IDSET_RSSI,
    	LOCATOR_IDSET_MAX
};

enum SNMP_MSG_ID
{
    SNMP_MSG_BEGIN=0x04000000,	//67108864
    SNMP_MSG_AP_ADD,	//67108865
    SNMP_MSG_AP_DEL,
    SNMP_MSG_USER_ADD,
    SNMP_MSG_USER_DEL,
    SNMP_MSG_GET_AUTH_CFG_ACK,
    SNMP_MSG_AP_INFO,	//67108870
    SNMP_MSG_STA_INFO,
    SNMP_MSG_USERINFO,
	SNMP_MSG_STA_ADD,
	SNMP_MSG_STA_DEL,
	SNMP_MSG_MAX
};

/***********************************************************************/
/*Brocast msg id*/
#if 0
enum BC_idset
{
	BC_MSG_BEGIN=((u16)0),
	BC_MSG_AP_REGISTER_TO_GW_SUCCESS,
	BC_MSG_MAX
};

enum IPC_ipset
{
	AG_IPC_MSG_BEGIN=((u16)128),
	AG_IPC_MSG_REGISTER,
	AG_IPC_MSG_REGISTER_ACK,
	AG_IPC_MSG_UNREGISTER,
	AG_IPC_MSG_MAX
};

enum aglink_idset
{
	AGLINK_IDSET_BEGIN=256,
	AGLINK_IDSET_DEV_REGISTER_MSG,
	AGLINK_IDSET_DEV_REGISTER_ACK,
	AGLINK_IDSET_DEV_NAK_MSG,
	AGLINK_IDSET_DEV_KEEPLIVE_MSG,
	AGLINK_IDSET_DEV_KEEPLIVE_ACK_MSG,
	AGLINK_IDSET_DEV_LOGOUT_MSG,
	AGLINK_IDSET_DEV_LOGOUT_ACK,
	AGLINK_IDSET_FIT_AP_INFO,
	AGLINK_IDSET_STA_ADD,
	AGLINK_IDSET_STA_DEL,
	AGLINK_IDSET_MAX
};
#endif

typedef	u32 (*AG_MODULE_INIT)(u8 *pagmodule);
typedef	u32 (*AG_MODULE_CONTROL_INIT)(u8 *pagmodule);
typedef u32 (*AG_MODULE_BUSINESS_INIT)(u8 *pagmodule);
typedef  u32 (*AG_MODULE_EVENT_INIT)(u8 *pagmodule);
typedef void(*IPC_CLIENT_RECV_EVENT)(evutil_socket_t stfd, s16 event, void *parg);


typedef struct  ag_module
{
	u8 acname[MODULE_NAME_LENGTH];			//ģ������
	AG_MODULE_INIT aginit;	 			//ģ��������س�ʼ��
	AG_MODULE_CONTROL_INIT agcontralinit; 		//ģ��������ϢID����������ʼ��
	AG_MODULE_BUSINESS_INIT agbussinessinit;	//ģ��ҵ����ϢID����������ʼ��
	AG_MODULE_EVENT_INIT ageventinit;		//ģ��event�¼�/��ʱ��ע��
	const u8 *pmoduleinfo;	//��ָ��ָ���Ӧģ��������Ϣ
}AG_MODULE_SETUP;

typedef struct ag_module_mgt
{
	s32 ipcsocket;					//ipc socket
	struct sockaddr_in stipcskaddr;	/*server sock addr*/
	u32 uipcipaddr;					/*server IP addr*/
	u16 usipcport;						/*server UDP port*/
	
}AG_MODULE_IPC_MGT;


typedef struct ag_module_ipc_list
{
	u8 acname[MODULE_NAME_LENGTH];			/* module name */
	u32 uiModuleId;								//��ϢID����
	struct sockaddr_in stipcskaddr;			/*Module sock addr*/
	struct sockaddr_in stSerskaddr; 				/*Ser sock addr*/
	struct list_head stModuleList;  	 			/*module list*/
	struct hlist_node stHeadHlist;  				/*�ն�mac hash��*/
}AG_MODULE_IPC_MODULE_LIST_T;


#define CONNECT_IPC_SERVER_INTERVAL 7
typedef struct ag_module_register
{
	u8 acname[MODULE_NAME_LENGTH];		//ע��ģ����
	//u32 uiIdSet;						//��ϢID����
	u32 uiModuleId;						//ģ��ID���
	
	u32 uipcSocket;						/* connect to ipc server */
	struct sockaddr_in stipcskaddr;		/*Module sock addr*/
	u32 uipcipaddr;						/*Module IP addr*/
	u16 usipcport;						/*Module UDP port*/

	struct sockaddr_in stSerskaddr; 		/*Ser sock addr*/
	u32 uiSeripaddr; 						/*Ser IP addr*/
	u16 usSerport;						/*Ser UDP port*/

	u16 uiFlag;							/* connect flag */
	struct event_base *main_base;			
	struct event connect_ipc_server_ev;	/*���ü�� Event*/
	struct timeval connect_ipc_server_tv;	/*���ü�� ʱ����*/
	struct event recv_ev;					/* ������Ϣ�¼� */
	
}AG_MODULE_IPC_REGISTER;

typedef struct ag_link_msg_head
{
	u32 usMsgId;
	u16 usMsgType;
    u16 uiUnicast;
	u8 acname[MODULE_NAME_LENGTH];		//������ģ����
	u32 uiMsgLen;
}AG_LINK_MSG_HEAD_T;

typedef struct ag_link_msg_unicast_head
{
	u8 ucMac[MAC_LEN];
}AG_LINK_MSG_UNICAST_HEAD_T;




/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/

/*----------------------------------------------*
 * module-wide global variables                 *
 *----------------------------------------------*/

#define AG_MODULE_HASH_LIST_SIZE 128


/* ��ӡ���� */
#define LOG_OFF                 0xff    /* ����ӡ */

#define LOG_EMERG               0       /* system is unusable */
#define LOG_ALERT               1       /* action must be taken immediately */
#define LOG_CRIT                2       /* critical conditions */
#define LOG_ERR                 3       /* error conditions */
#define LOG_WARNING             4       /* warning conditions */
#define LOG_NOTICE              5       /* normal but significant condition */
#define LOG_INFO                6       /* informational */
#define LOG_DEBUG               7       /* debug-level messages */

#if 1
#define AG_MODULE_LOG(__level__, __format__, __arg__...) \
	do{ \
			syslog(LOG_DEBUG, "[%s][%d] "__format__, __FUNCTION__, __LINE__, ##__arg__);\
	}while(0)
#else
#define AG_MODULE_LOG(level, format, arg...) \
	do{ \
			app_log((char *)__FUNCTION__, __LINE__, level, format, ##arg);\
	}while(0)	
#endif

/**********************************************************ģ���ͨ����Ϣ���塢��Ϣ�嶨��******************************************/
#define AG_MODULE_AP_MAX_RADIO_NUM 2
#define AG_MODULE_RADIO_MAX_VAP_NUM 16

typedef struct{
	char acVapName[SSID_LEN];
         char acSsid[SSID_LEN];                                             /*ssid*/
         unsigned char aucBssid[MAC_LEN];          			/*bssid*/
}AP_REGISTER_VAP_T;

typedef struct{
         unsigned int uiRadioType;           /*radio type: 0(2g), 1(5g)*/
         unsigned int uiVapNum;
         AP_REGISTER_VAP_T astVap[AG_MODULE_RADIO_MAX_VAP_NUM];
}AP_RADIO_INFO_T;

typedef struct {
	u32 uiRadioNum;
	AP_RADIO_INFO_T astRadio[AG_MODULE_AP_MAX_RADIO_NUM];
}AP_WIFI_INFO;

#if 0
/*snmpagent aglink-msg ��Ϣ����*/
enum snmpagent_set{
	SNMPAGENT_IDSET_BEGIN=4096,
    SNMPAGENT_IDSET_WIFIDOG_SNMPAGENT_GET_GWID_RSP,
    SNMPAGENT_IDSET_STAM_SNMPAGENT_ALL_STA_RSP,
    SNMPAGENT_IDSET_WIFIDOG_SNMPAGENT_ALL_STA_RSP,
    SNMPAGENT_IDSET_MAX
};
#endif
typedef struct {
    char acGwId[32];
}WIFIDOG_SNMPAGENT_GWID_T;
typedef struct {
	unsigned char aucMac[6];
    unsigned char aucIp[16];
    int iAuthState;       /*��֤״̬��STAM_STA_UNAUTHED, STAM_STA_AUTHED*/
	long iLoginTimeSet;   /*��������֤ʱ��uptime*/
	long iOnlineTime;     /*����֤ʱ��*/
    long iAssoTimeSet;    /*���������uptime*/
    long iAssoTime;       /*�ѹ���ʱ��*/
}STAM_SNMPAGENTSTA_INFO_T,WIFIDOG_SNMPAGENT_INFO_T;
typedef struct{
	int iStaNum;
    int iFlag;    /*1��ʾ���һ����Ϣ*/
    STAM_SNMPAGENTSTA_INFO_T astStaInfo[0];
}STAM_SNMPAGENT_ALL_STA_RSP_T,WIFIDOG_SNMPAGENT_ALL_STA_RSP_T;


typedef struct {
	unsigned char aucMac[6];
    unsigned char aucIp[16];
    int iAuthType; /*��֤���Ϳ���ֵΪ1~9 �û�����Ϊ1,�Ż�ȯΪ2,�̶�����Ϊ3,�� ��Ϊ4,��ѶQQΪ5,����΢��Ϊ6,������ Ϊ7,΢��Ϊ8,֧����Ϊ9*/ 
    char acBblink_UserId[64];      //�������µ��û�id
	long iUpSpeed;		/*��������(kbps)*/
	long iDownSpeed;	/*��������(kbps)*/
	long iCnntCnt;		/*������*/
	long incoming;		/*��������(MB)*/
    long outgoing;		/*��������(MB)*/
    long iAssoTime;     /*����ʱ��(��)*/
}WIFIDOG_SNMPAGENTSTA_INFO_T;
typedef struct{
	int iStaNum;
    int iFlag;    /*1��ʾ���һ����Ϣ*/
    WIFIDOG_SNMPAGENTSTA_INFO_T astStaInfo[0];
}WIFIDOG_SNMPAGENT_ALL_STA_INFO_T;


typedef struct{
	unsigned char aucMac[6];
    unsigned char aucIp[4];
    char acSsid[33];
    char acLogInTime[32];
    char aucLogOutTime[32];
    unsigned int  ulOnlineTime;
    char acLogOutType[32];
}WIFIDOG_SNMPAGENT_STA_T;

#if 0
/*stam aglink-msg ��Ϣ����*/
enum stam_set{
	STAM_IDSET_BEGIN=2048,
    STAM_IDSET_EVENT_STAM_ADD_STA,
    STAM_IDSET_EVENT_STAM_DEL_STA,
    STAM_IDSET_SNMPAGENT_STAM_GET_ALL_STA,
    STAM_IDSET_WIFIDOG_STAM_UPDATE_AUTH_STATE,
    STAM_IDSET_MAX
};
#endif
/*STAM_IDSET_EVENT_STAM_ADD_STA ����Ϣ��*/
/*ʹ������wifidog�Ľṹ��
typedef struct{
	unsigned char aucMac[6];
    unsigned char aucIp[16];
}STAM_EVENT_ADD_STA_T;
*/
/*STAM_IDSET_EVENT_STAM_DEL_STA ����Ϣ��*/
/*ʹ������wifidog�Ľṹ��
typedef struct{
	unsigned char aucMac[6];
    unsigned char aucIp[16];
}STAM_EVENT_DEL_STA_T;
*/
/*STAM_IDSET_WIFIDOG_STAM_UPDATE_AUTH_STATE ����Ϣ��*/
typedef struct{
    unsigned char aucMac[6];
    unsigned char aucIp[16];
    int iAuthState;       /*��֤״̬��STAM_STA_UNAUTHED, STAM_STA_AUTHED*/
    int iAuthType; /*��֤���Ϳ���ֵΪ1~9 �û�����Ϊ1,�Ż�ȯΪ2,�̶�����Ϊ3,�� ��Ϊ4,��ѶQQΪ5,����΢��Ϊ6,������ Ϊ7,΢��Ϊ8,֧����Ϊ9*/ 
    char acBblink_UserId[64];      //�������µ��û�id
}STAM_WIFIDOG_STA_T;

#define STAM_STA_AUTHED 1        //�û�����
#define STAM_STA_UNAUTHED 0        //�û�����


typedef struct {
	s32 iSwitch;			//������֤���ÿ���,1Ϊ����,0Ϊ�ر�
	s32 iWeb_auth;			// Web��֤���ÿ���,1Ϊ����,0Ϊ�ر�
	s32 iOffline; 			 //�Զ�����,����ʱ����������ǿ��������,0 Ϊ������,��λ��
	s32 iReverify;			 //������֤,��������ָ��ʱ��������֤,0 Ϊ������,��λ����
 } AUTH_CONF_T;

typedef struct{
	int iStaNum;
    STAM_WIFIDOG_STA_T astStaInfo[0];
}STAM_WIFIDOG_UPDATE_STATE_T;

#if 0
/*wifidog aglink-msg ��Ϣ����*/
enum wifidog_set{
	WIFIDOG_IDSET_BEGIN=512,
    WIFIDOG_IDSET_AP_REGISTER,
    WIFIDOG_IDSET_AP_LOGOUT,
    WIFIDOG_IDSET_EVENT_WIFIDOG_ADD_STA,
    WIFIDOG_IDSET_EVENT_WIFIDOG_DEL_STA,
    WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_AP_SSIDCONFIG,
    WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_AP_SSID_DEL,
    WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_SELF_SSIDCONFIG,
    WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_SELF_SSID_DEL,		//520
    WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_SSID_MAC_BAW_ADD,
    WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_SSID_MAC_BAW_DEL,
    WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_MAC_BAW_UPDATE, /*���Ƿ�ʽ����MAC�ڰ�����*/
    WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_SSID_URL_BAW_ADD,
    WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_SSID_URL_BAW_DEL,
    WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_GET_GWID,
    WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_GET_ALL_STA,
	WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_NOTIFIED_APSSID_CONF, /*WIFIDOG �����յ���AP��ssid��Ӧ*/
	WIFIDOG_IDSET_STAM_WIFIDOG_NOTIFIED_ALL_STA, /*WIFIDOG�����յ�stam����������ն���Ϣ*/
	WIFIDOG_IDSET_AP_WIFI_INFO_UPDATE,
    WIFIDOG_IDSET_MAX	//530
};
#endif 
/*WIFIDOG_IDSET_AP_REGISTER ����Ϣ��*/
typedef struct{
	unsigned char aucMac[6];
    	unsigned char aucIp[16];
    	unsigned int uiModelId;
    	char acName[32];
	AP_WIFI_INFO stApInfo;
}WIFIOG_AP_REGISTER_T;
typedef  WIFIOG_AP_REGISTER_T WIFIOG_AP_LOGOUT_T;
/*WIFIDOG_IDSET_EVENT_WIFIDOG_ADD_STA ����Ϣ��*/
typedef struct{
	u8 aucApMac[MAC_LEN];
	u8 aucStaMac[MAC_LEN];
    	u8 aucIp[4];
    	s8 acSsid[SSID_LEN];          /*�����ssid����*/
    	u8 acVapName[32];       /*�����vap�ӿ�����*/
	s32 rssi;
	u32 connect_time;
    	u64 total_up;
    	u64 total_down;
}WIFIOG_EVENT_ADD_STA_T,STAM_EVENT_ADD_STA_T, BOXLL_EVENT_STA, SNMP_EVENT_ADD_STA_INFO,SNMP_EVENT_STA_INFO;

typedef struct
{
	u8 aucApMac[MAC_LEN];	//AP mac
	s32 iStaNum;
    	s32 iFlag;    				/*1��ʾ���һ����Ϣ*/
    	SNMP_EVENT_STA_INFO astStaInfo[0];
}STAM_ALL_STA_LIST;

/*WIFIDOG_IDSET_EVENT_WIFIDOG_DEL_STA ����Ϣ��*/
typedef struct{
	unsigned char aucMac[MAC_LEN];
    unsigned char aucIp[16];
	char acVapName[32];
	u8 aucApMac[MAC_LEN];
	s8 acSsid[SSID_LEN];          /*�����ssid����*/
}WIFIOG_EVENT_DEL_STA_T,STAM_EVENT_DEL_STA_T;
/*WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_AP_SSIDCONFIG/WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_SELF_SSIDCONFIG ����Ϣ��*/
typedef struct{
	unsigned char aucApMac[6];
	unsigned char aucRsvd[2];
    char          acVapName[32];       /*SSID��Ч��vap�ӿ���*/
	unsigned int  uiSsidNumber;        /*SSID��ţ�ȫ�ַ�Χ1-6*/
	unsigned int  uiRadioType;         /*��Ƶ�����ͣ�0: 2.4G, 1: 5G*/
    char          acSsid[32];        
    unsigned int  uiDefault;           /*�Ƿ���Ĭ��SSID*/
    unsigned int  uiUpRate;            /*��������*/
	unsigned int  uiDownRate;          /*��������*/
	char          acPortalUrl[32];     /*SSID��portal url*/
    unsigned int  uiOnlineControlTime; /*����ʱ�����ƣ�Ӳ�ԶϿ�ʱ�䣬����*/
	unsigned int  uiFlowOffTime;       /*����������ʱ�䣬����*/
}WIFIOG_SNMPAGENT_SSIDCONFIG_T;
typedef struct{
	unsigned char aucApMac[6];
	unsigned char aucRsvd[2];
	unsigned int  uiSsidNumber;        /*SSID��ţ�ȫ�ַ�Χ1-6*/
}WIFIOG_SNMPAGENT_SSID_DEL_T;
/*WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_SSID_MAC_BAW_CONFIG ����Ϣ��*/
#define BAW_TYPE_BLACK 1
#define BAW_TYPE_WHITE 2
typedef struct{
	int iType;               /*�ڰ���������*/
    unsigned char aucMac[6]; /*�û�MAC��ַ*/
}WIFIOG_SNMPAGENT_MAC_BAW_T;
typedef struct{
    unsigned int uiSsidNumber; /*ssid ���*/
	int iNum;
    WIFIOG_SNMPAGENT_MAC_BAW_T astMacBaw[0];
}WIFIDOG_SNMPAGENT_MAC_BAW_CONFIG_T;
/*WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_SSID_URL_BAW_CONFIG ����Ϣ��*/
typedef struct{
	int iType;               /*�ڰ���������*/
    char aucUrl[32];         /*url*/
}WIFIOG_SNMPAGENT_URL_BAW_T;
typedef struct{
    unsigned int uiSsidNumber; /*ssid ���*/
	int iNum;
    WIFIOG_SNMPAGENT_URL_BAW_T astUrlBaw[0];
}WIFIDOG_SNMPAGENT_URL_BAW_CONFIG_T;
/*WIFIDOG_SNMPAGENT_USER_TIMEOUT ����Ϣ��*/
typedef struct{
    int iSwitch;
	int iTimeOut;
	int iFlowThres;
}WIFIDOG_SNMPAGENT_USER_TIMEOUT;
/*WIFIDOG_SNMPAGENT_FLOW_CTL ����Ϣ��*/
typedef struct{
    int iUpFlowMax;    //������������
	int iDownFlowMax;  //������������
}WIFIDOG_SNMPAGENT_FLOW_CTL;
/*WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_GET_GWID ����Ϣ��*/
typedef struct{
    unsigned char aucApMac[6];
    char acAuthCode[64];
}WIFIDOG_SNMPAGENT_GET_GWID_T;

/*WIFIDOG_SNMPAGENT_STAKEY_INTVAL ����Ϣ��*/
typedef struct{
	int iIntval;
}WIFIDOG_SNMPAGENT_STAKEY_INTVAL;
/*WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_GET_ALL_STA ����Ϣ��*/
/*WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_NTIFIED_APSSID_CONF ����Ϣ��*/
typedef struct{
    unsigned char aucApMac[6]; /*AP��mac��ַ*/
	unsigned char aucRsvd[2];
	int iNum;                  /*���6��SSID*/
    WIFIOG_SNMPAGENT_SSIDCONFIG_T astApSsid[0];
}WIFIDOG_SNMPAGENT_APSSID_CONFIG_RSP_T;
/*WIFIDOG_IDSET_STAM_WIFIDOG_NOTIFIED_ALL_STA*/
typedef struct{
	int iNum;                  
    STAM_EVENT_ADD_STA_T astSta[0];
}WIFIDOG_STAM_ALL_STA_INFO_T;
/*WIFIDOG_IDSET_LOCATOR_WIFIDOG_ADD_STA ����Ϣ��*/
typedef struct{
	unsigned char aucApMac[MAC_LEN];
	unsigned char aucStaMac[MAC_LEN];
    char cRssi;
}WIFIOG_LOCATOR_ADD_STA_T;
/*WIFIDOG_IDSET_LOCATOR_WIFIDOG_DEL_STA ����Ϣ��*/
typedef struct{
	unsigned char aucApMac[MAC_LEN];
	unsigned char aucStaMac[MAC_LEN];
}WIFIOG_LOCATOR_DEL_STA_T;


/**********************************************************ģ���ͨ����Ϣ���塢��Ϣ�嶨��******************************************/



/* for event module */
/*enum event_idset
{
	EVENT_IDSET_BEGIN=1024,
	EVENT_IDSET_STA_REGISTERED,
	EVENT_IDSET_STA_EXPIRED,
	EVENT_IDSET_MAX
};
*/
#ifndef IFNAMSIZ
#define IFNAMSIZ 16
#endif

typedef struct event_sta_info
{
	u8 ifname[IFNAMSIZ];
	u8 staMac[MAC_LEN];
}EVENT_STA_INFO_T;

struct sta_node
{	
	u8 ap_mac[MAC_LEN]; 				/*�ն˹���AP��MAC��ַ*/
	u8 mac[MAC_LEN]; 					/*�ն�MAC��ַ*/
	u8 aucIp[4];
#ifdef _BOXLL_
	/* BEGIN:fengjing add 2014-09-16 */
	s8 ap_ssid[SSID_LEN]; 				/*�ն˹���AP��SSID*/
	/* END:fengjing add 2014-09-16 */
#endif
	s8 vap_name[SSID_LEN];
	s32 rssi;
	u32 connect_time;
	u64 total_up;
	u64 total_down;
	u32 associate_time;
	struct hlist_node id_hlist;
	struct list_head stStaList;   /*�ն�����*/
};

/*for aglinkcli*/
typedef struct
{
	//u32 iFlag;	/*0:get time; 1: set time*/
	u32 uiKeeptm;
}AGLINKCLI_KEEPLIVE_T;

typedef struct
{
	u32	uiOnlinestate;
	u32 uiOnlinetm;
	u32 uiSerIp;
	u8 ucSerMAC[MAC_LEN];
}AGLINKCLI_ONLINE_STATE_T;

#define AGLINK_MODULE_NUMBER_MAX 32
typedef struct
{
	u32 iNum;
	AG_MODULE_IPC_MODULE_LIST_T stModule[AGLINK_MODULE_NUMBER_MAX];
}AGLINKCLI_MODULE_INFO_T;

#define AGLINK_GW_AP_NUM_MAX		2

typedef struct 
{
	struct sockaddr_in sk_addr;			/*AP sock addr*/
	u8 ip[IP_LEN];						/*AP IP addr*/
	u8 mac[MAC_LEN]; 					/*AP WAN��MAC��ַ*/
	s8 dev_name[64];					/*AP device name*/
	u32 model_id;						/*AP�豸model id*/
	struct timeval time; 	  			/*AP�������ʱ���*/
	//AP_WIFI_INFO stWifiInfo;
	struct hlist_node id_hlist;
}ap_node;

typedef struct
{
	u32 ibegin;	//��ȡAP�б����ʼλ�ã��б��п��ܴ���>64���ڵ�����
	u32 iNum;	//��ǰ�Ѿ���ȡ���ն���
	ap_node	stApInfo[AGLINK_GW_AP_NUM_MAX];
}AGLINKCLI_AP_LIST_T;

#define AGLINKCLI_ERROR_MSG_LEN	128
typedef struct
{
	u32 iFlag;	/*0: except  1:normal*/
	union{
		u8 ucErrMsg[AGLINKCLI_ERROR_MSG_LEN];
		AGLINKCLI_ONLINE_STATE_T stOnline;
		AGLINKCLI_MODULE_INFO_T stModuleInfo;
		AGLINKCLI_AP_LIST_T stAplistInfo;
		AGLINKCLI_KEEPLIVE_T stKeeplive;
	}u;
}AGLINKCLI_MSG_T;


/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macros                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * routines' implementations                    *
 *----------------------------------------------*/


extern s32 ag_module_ipc_module_init(u8 *pacName, u32 uiModuleId, AG_MODULE_IPC_REGISTER *pstIpcRegister,  IPC_CLIENT_RECV_EVENT *pRecv_fun);
extern s32 ag_module_ipc_client_send_msg(u32 uiMsgID, u32 uiMsgType, AG_MODULE_IPC_REGISTER *pstIpcRegister, u8 *pbuff, int iMsgLen);
extern s32 ag_module_ipc_client_send_msg_to_ap(u32 uiMsgID, u32 uiMsgType, AG_MODULE_IPC_REGISTER * pstIpcRegister, u8 * pbuff, int iMsgLen, u8 * ucMac);
extern s32 ag_module_ipc_power_on(AG_MODULE_IPC_REGISTER *pstIpcRegister);
extern s32 ag_module_ipc_reconnect_server(AG_MODULE_IPC_REGISTER * pstIpcRegister);
extern s32 aglink_parse_agcli_msg(u8 *buf, u32 len, struct sockaddr_in *sk_addr);
extern s32 ag_module_ipc_get_server_ip_port(u32 * uipcipaddr, u16 * usipcport, u32 * uiSeripaddr, u16 * usSerport, u8 * pacName);
extern s32 aglink_parse_ag_msg(u8 * buf, u32 len, struct sockaddr_in * sk_addr);

#endif
