/******************************************************************************

  Copyright (C), 2010-2011, GBCOM Co., Ltd.

 ******************************************************************************
  File Name     : ag_module.h
  Version       : Initial Draft
  Author        : lvjianlin
  Created       : 2014/11/17
  Last Modified :
  Description   : ag_module层头文件
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

/*板内组播消息*/
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
	WIFIDOG_MSG_SYN_ALL_STA, 	/*83886085 定期同步终端信息10245*/
	WIFIDOG_MSG_STADCT_INFO,
	WIFIDOG_MSG_SNMPAGENT_SET_STA_AUTH,		/*终端认证消息*/
	WIFIDOG_MSG_SNMPAGENT_SET_AUTH_CONFIG,
	WIFIDOG_MSG_SNMPAGENT_GET_AUTH_CONFIG,
   	WIFIDOG_MSG_SNMPAGENT_SET_MAC_BAW_UPDATE, /*覆盖方式更新MAC黑白名单*/
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
	u8 acname[MODULE_NAME_LENGTH];			//模块名称
	AG_MODULE_INIT aginit;	 			//模块配置相关初始化
	AG_MODULE_CONTROL_INIT agcontralinit; 		//模块配置消息ID及处理函数初始化
	AG_MODULE_BUSINESS_INIT agbussinessinit;	//模块业务消息ID及处理函数初始化
	AG_MODULE_EVENT_INIT ageventinit;		//模块event事件/定时器注册
	const u8 *pmoduleinfo;	//该指针指向对应模块配置信息
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
	u32 uiModuleId;								//消息ID集合
	struct sockaddr_in stipcskaddr;			/*Module sock addr*/
	struct sockaddr_in stSerskaddr; 				/*Ser sock addr*/
	struct list_head stModuleList;  	 			/*module list*/
	struct hlist_node stHeadHlist;  				/*终端mac hash链*/
}AG_MODULE_IPC_MODULE_LIST_T;


#define CONNECT_IPC_SERVER_INTERVAL 7
typedef struct ag_module_register
{
	u8 acname[MODULE_NAME_LENGTH];		//注册模块名
	//u32 uiIdSet;						//消息ID集合
	u32 uiModuleId;						//模块ID编号
	
	u32 uipcSocket;						/* connect to ipc server */
	struct sockaddr_in stipcskaddr;		/*Module sock addr*/
	u32 uipcipaddr;						/*Module IP addr*/
	u16 usipcport;						/*Module UDP port*/

	struct sockaddr_in stSerskaddr; 		/*Ser sock addr*/
	u32 uiSeripaddr; 						/*Ser IP addr*/
	u16 usSerport;						/*Ser UDP port*/

	u16 uiFlag;							/* connect flag */
	struct event_base *main_base;			
	struct event connect_ipc_server_ev;	/*配置检查 Event*/
	struct timeval connect_ipc_server_tv;	/*配置检查 时间间隔*/
	struct event recv_ev;					/* 接收消息事件 */
	
}AG_MODULE_IPC_REGISTER;

typedef struct ag_link_msg_head
{
	u32 usMsgId;
	u16 usMsgType;
    u16 uiUnicast;
	u8 acname[MODULE_NAME_LENGTH];		//发送者模块名
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


/* 打印级别 */
#define LOG_OFF                 0xff    /* 不打印 */

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

/**********************************************************模块间通信消息定义、消息体定义******************************************/
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
/*snmpagent aglink-msg 消息定义*/
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
    int iAuthState;       /*认证状态，STAM_STA_UNAUTHED, STAM_STA_AUTHED*/
	long iLoginTimeSet;   /*设置已认证时的uptime*/
	long iOnlineTime;     /*已认证时长*/
    long iAssoTimeSet;    /*创建表项的uptime*/
    long iAssoTime;       /*已关联时长*/
}STAM_SNMPAGENTSTA_INFO_T,WIFIDOG_SNMPAGENT_INFO_T;
typedef struct{
	int iStaNum;
    int iFlag;    /*1表示最后一条消息*/
    STAM_SNMPAGENTSTA_INFO_T astStaInfo[0];
}STAM_SNMPAGENT_ALL_STA_RSP_T,WIFIDOG_SNMPAGENT_ALL_STA_RSP_T;


typedef struct {
	unsigned char aucMac[6];
    unsigned char aucIp[16];
    int iAuthType; /*认证类型可能值为1~9 用户密码为1,优惠券为2,固定密码为3,手 机为4,腾讯QQ为5,新浪微博为6,无密码 为7,微信为8,支付宝为9*/ 
    char acBblink_UserId[64];      //贝联传下的用户id
	long iUpSpeed;		/*上行速率(kbps)*/
	long iDownSpeed;	/*下行速率(kbps)*/
	long iCnntCnt;		/*连接数*/
	long incoming;		/*下行流量(MB)*/
    long outgoing;		/*上行流量(MB)*/
    long iAssoTime;     /*连接时长(秒)*/
}WIFIDOG_SNMPAGENTSTA_INFO_T;
typedef struct{
	int iStaNum;
    int iFlag;    /*1表示最后一条消息*/
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
/*stam aglink-msg 消息定义*/
enum stam_set{
	STAM_IDSET_BEGIN=2048,
    STAM_IDSET_EVENT_STAM_ADD_STA,
    STAM_IDSET_EVENT_STAM_DEL_STA,
    STAM_IDSET_SNMPAGENT_STAM_GET_ALL_STA,
    STAM_IDSET_WIFIDOG_STAM_UPDATE_AUTH_STATE,
    STAM_IDSET_MAX
};
#endif
/*STAM_IDSET_EVENT_STAM_ADD_STA 的消息体*/
/*使用下面wifidog的结构体
typedef struct{
	unsigned char aucMac[6];
    unsigned char aucIp[16];
}STAM_EVENT_ADD_STA_T;
*/
/*STAM_IDSET_EVENT_STAM_DEL_STA 的消息体*/
/*使用下面wifidog的结构体
typedef struct{
	unsigned char aucMac[6];
    unsigned char aucIp[16];
}STAM_EVENT_DEL_STA_T;
*/
/*STAM_IDSET_WIFIDOG_STAM_UPDATE_AUTH_STATE 的消息体*/
typedef struct{
    unsigned char aucMac[6];
    unsigned char aucIp[16];
    int iAuthState;       /*认证状态，STAM_STA_UNAUTHED, STAM_STA_AUTHED*/
    int iAuthType; /*认证类型可能值为1~9 用户密码为1,优惠券为2,固定密码为3,手 机为4,腾讯QQ为5,新浪微博为6,无密码 为7,微信为8,支付宝为9*/ 
    char acBblink_UserId[64];      //贝联传下的用户id
}STAM_WIFIDOG_STA_T;

#define STAM_STA_AUTHED 1        //用户上线
#define STAM_STA_UNAUTHED 0        //用户下线


typedef struct {
	s32 iSwitch;			//无线认证启用开关,1为开启,0为关闭
	s32 iWeb_auth;			// Web认证启用开关,1为开启,0为关闭
	s32 iOffline; 			 //自动下线,超过时间无流量则强制其下线,0 为无限制,单位秒
	s32 iReverify;			 //重新认证,上网超过指定时须重新认证,0 为无限制,单位分钟
 } AUTH_CONF_T;

typedef struct{
	int iStaNum;
    STAM_WIFIDOG_STA_T astStaInfo[0];
}STAM_WIFIDOG_UPDATE_STATE_T;

#if 0
/*wifidog aglink-msg 消息定义*/
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
    WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_MAC_BAW_UPDATE, /*覆盖方式更新MAC黑白名单*/
    WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_SSID_URL_BAW_ADD,
    WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_SSID_URL_BAW_DEL,
    WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_GET_GWID,
    WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_GET_ALL_STA,
	WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_NOTIFIED_APSSID_CONF, /*WIFIDOG 启动收到的AP的ssid响应*/
	WIFIDOG_IDSET_STAM_WIFIDOG_NOTIFIED_ALL_STA, /*WIFIDOG启动收到stam的添加所有终端消息*/
	WIFIDOG_IDSET_AP_WIFI_INFO_UPDATE,
    WIFIDOG_IDSET_MAX	//530
};
#endif 
/*WIFIDOG_IDSET_AP_REGISTER 的消息体*/
typedef struct{
	unsigned char aucMac[6];
    	unsigned char aucIp[16];
    	unsigned int uiModelId;
    	char acName[32];
	AP_WIFI_INFO stApInfo;
}WIFIOG_AP_REGISTER_T;
typedef  WIFIOG_AP_REGISTER_T WIFIOG_AP_LOGOUT_T;
/*WIFIDOG_IDSET_EVENT_WIFIDOG_ADD_STA 的消息体*/
typedef struct{
	u8 aucApMac[MAC_LEN];
	u8 aucStaMac[MAC_LEN];
    	u8 aucIp[4];
    	s8 acSsid[SSID_LEN];          /*接入的ssid名称*/
    	u8 acVapName[32];       /*接入的vap接口名称*/
	s32 rssi;
	u32 connect_time;
    	u64 total_up;
    	u64 total_down;
}WIFIOG_EVENT_ADD_STA_T,STAM_EVENT_ADD_STA_T, BOXLL_EVENT_STA, SNMP_EVENT_ADD_STA_INFO,SNMP_EVENT_STA_INFO;

typedef struct
{
	u8 aucApMac[MAC_LEN];	//AP mac
	s32 iStaNum;
    	s32 iFlag;    				/*1表示最后一条消息*/
    	SNMP_EVENT_STA_INFO astStaInfo[0];
}STAM_ALL_STA_LIST;

/*WIFIDOG_IDSET_EVENT_WIFIDOG_DEL_STA 的消息体*/
typedef struct{
	unsigned char aucMac[MAC_LEN];
    unsigned char aucIp[16];
	char acVapName[32];
	u8 aucApMac[MAC_LEN];
	s8 acSsid[SSID_LEN];          /*接入的ssid名称*/
}WIFIOG_EVENT_DEL_STA_T,STAM_EVENT_DEL_STA_T;
/*WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_AP_SSIDCONFIG/WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_SELF_SSIDCONFIG 的消息体*/
typedef struct{
	unsigned char aucApMac[6];
	unsigned char aucRsvd[2];
    char          acVapName[32];       /*SSID生效的vap接口名*/
	unsigned int  uiSsidNumber;        /*SSID编号，全局范围1-6*/
	unsigned int  uiRadioType;         /*射频卡类型，0: 2.4G, 1: 5G*/
    char          acSsid[32];        
    unsigned int  uiDefault;           /*是否是默认SSID*/
    unsigned int  uiUpRate;            /*上行限速*/
	unsigned int  uiDownRate;          /*下行限速*/
	char          acPortalUrl[32];     /*SSID的portal url*/
    unsigned int  uiOnlineControlTime; /*上网时长控制，硬性断开时间，分钟*/
	unsigned int  uiFlowOffTime;       /*无流量下线时间，分钟*/
}WIFIOG_SNMPAGENT_SSIDCONFIG_T;
typedef struct{
	unsigned char aucApMac[6];
	unsigned char aucRsvd[2];
	unsigned int  uiSsidNumber;        /*SSID编号，全局范围1-6*/
}WIFIOG_SNMPAGENT_SSID_DEL_T;
/*WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_SSID_MAC_BAW_CONFIG 的消息体*/
#define BAW_TYPE_BLACK 1
#define BAW_TYPE_WHITE 2
typedef struct{
	int iType;               /*黑白名单类型*/
    unsigned char aucMac[6]; /*用户MAC地址*/
}WIFIOG_SNMPAGENT_MAC_BAW_T;
typedef struct{
    unsigned int uiSsidNumber; /*ssid 编号*/
	int iNum;
    WIFIOG_SNMPAGENT_MAC_BAW_T astMacBaw[0];
}WIFIDOG_SNMPAGENT_MAC_BAW_CONFIG_T;
/*WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_SSID_URL_BAW_CONFIG 的消息体*/
typedef struct{
	int iType;               /*黑白名单类型*/
    char aucUrl[32];         /*url*/
}WIFIOG_SNMPAGENT_URL_BAW_T;
typedef struct{
    unsigned int uiSsidNumber; /*ssid 编号*/
	int iNum;
    WIFIOG_SNMPAGENT_URL_BAW_T astUrlBaw[0];
}WIFIDOG_SNMPAGENT_URL_BAW_CONFIG_T;
/*WIFIDOG_SNMPAGENT_USER_TIMEOUT 的消息体*/
typedef struct{
    int iSwitch;
	int iTimeOut;
	int iFlowThres;
}WIFIDOG_SNMPAGENT_USER_TIMEOUT;
/*WIFIDOG_SNMPAGENT_FLOW_CTL 的消息体*/
typedef struct{
    int iUpFlowMax;    //上行流量限制
	int iDownFlowMax;  //上行流量限制
}WIFIDOG_SNMPAGENT_FLOW_CTL;
/*WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_GET_GWID 的消息体*/
typedef struct{
    unsigned char aucApMac[6];
    char acAuthCode[64];
}WIFIDOG_SNMPAGENT_GET_GWID_T;

/*WIFIDOG_SNMPAGENT_STAKEY_INTVAL 的消息体*/
typedef struct{
	int iIntval;
}WIFIDOG_SNMPAGENT_STAKEY_INTVAL;
/*WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_GET_ALL_STA 的消息体*/
/*WIFIDOG_IDSET_SNMPAGENT_WIFIDOG_NTIFIED_APSSID_CONF 的消息体*/
typedef struct{
    unsigned char aucApMac[6]; /*AP的mac地址*/
	unsigned char aucRsvd[2];
	int iNum;                  /*最多6个SSID*/
    WIFIOG_SNMPAGENT_SSIDCONFIG_T astApSsid[0];
}WIFIDOG_SNMPAGENT_APSSID_CONFIG_RSP_T;
/*WIFIDOG_IDSET_STAM_WIFIDOG_NOTIFIED_ALL_STA*/
typedef struct{
	int iNum;                  
    STAM_EVENT_ADD_STA_T astSta[0];
}WIFIDOG_STAM_ALL_STA_INFO_T;
/*WIFIDOG_IDSET_LOCATOR_WIFIDOG_ADD_STA 的消息体*/
typedef struct{
	unsigned char aucApMac[MAC_LEN];
	unsigned char aucStaMac[MAC_LEN];
    char cRssi;
}WIFIOG_LOCATOR_ADD_STA_T;
/*WIFIDOG_IDSET_LOCATOR_WIFIDOG_DEL_STA 的消息体*/
typedef struct{
	unsigned char aucApMac[MAC_LEN];
	unsigned char aucStaMac[MAC_LEN];
}WIFIOG_LOCATOR_DEL_STA_T;


/**********************************************************模块间通信消息定义、消息体定义******************************************/



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
	u8 ap_mac[MAC_LEN]; 				/*终端关联AP的MAC地址*/
	u8 mac[MAC_LEN]; 					/*终端MAC地址*/
	u8 aucIp[4];
#ifdef _BOXLL_
	/* BEGIN:fengjing add 2014-09-16 */
	s8 ap_ssid[SSID_LEN]; 				/*终端关联AP的SSID*/
	/* END:fengjing add 2014-09-16 */
#endif
	s8 vap_name[SSID_LEN];
	s32 rssi;
	u32 connect_time;
	u64 total_up;
	u64 total_down;
	u32 associate_time;
	struct hlist_node id_hlist;
	struct list_head stStaList;   /*终端链表*/
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
	u8 mac[MAC_LEN]; 					/*AP WAN口MAC地址*/
	s8 dev_name[64];					/*AP device name*/
	u32 model_id;						/*AP设备model id*/
	struct timeval time; 	  			/*AP最近在线时间点*/
	//AP_WIFI_INFO stWifiInfo;
	struct hlist_node id_hlist;
}ap_node;

typedef struct
{
	u32 ibegin;	//获取AP列表的起始位置，列表中可能存在>64个节点的情况
	u32 iNum;	//当前已经获取的终端数
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
