/******************************************************************************

  Copyright (C), 2010-2011, GBCOM Co., Ltd.

 ******************************************************************************
  File Name     : aglink_temp.h
  Version       : Initial Draft
  Author        : lvjianlin
  Created       : 2014/11/18
  Last Modified :
  Description   : 
  Function List :
  History       :
  1.Date        : 2014/11/18
    Author      : lvjianlin
    Modification: Created file

******************************************************************************/

/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/

/*----------------------------------------------*
 * module-wide global variables                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macros                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * routines' implementations                    *
 *----------------------------------------------*/
#ifndef _AGLINK_H_
#define _AGLINK_H_

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
#include <nvram_config.h>

#include <fcntl.h>
#include <sys/mman.h>

#include <event.h>

#include "list.h"
#include "ag_common.h"
#include "ag_msg.h"
#include "ag_module.h"

/*申请内存并初始化为0*/
#define MEM_CALLOC(__size__)  calloc(1, __size__)

/*释放内存并赋值为NULL*/
#define MEM_FREE(__addr__) \
	do{\
		if(__addr__ != NULL){ \
			free(__addr__); \
			__addr__ = NULL; \
		}\
	}while(0)



#define KEEPALIVE_INTERVAL		10 /*5*/ 
#define KEEPALIVE_MAX_TIMES 	6  /*3*/ 

#define ACK_CHECK_INTERVAL		12
#define ACK_RESEND_MAX_TIMES 	25

#define CONFIG_CHECK_INTERVAL	8

#define AP_AGE_INTERVAL		32
#define AP_AGE_TIME			60


#define AP_HASH_SIZE 			256
#define STA_HASH_SIZE 			512

#define AG_BUFFER_SIZE			((u32)6144)
#define MAXLEN 					2048
#define WIFIDOG_CNNT_INTERVAL		4
#define AGLINK_REPORT_AP_INFO		240

#define AGLINK_FIT_AP 	0
#define AGLINK_FAT_AP 	1

//#define PHY_IF "eth0"
#ifdef OPENWRT

#define UCI_NETWORK "network."
#define UCI_GATEWAY "gateway.@gateway[0]."
#define UCI_SYSTEM "system.@system[0]."




#define DEFAULT_SERVER_PORT		"15513"


#define AG_SERVER_IP_CONFIG		"ag_gw_ip_dynamic"	//"gbverdor_nv"
#define AG_SERVER_PORT_CONFIG		UCI_GATEWAY"ag_server_port"
#define AG_IP_GET_METHOD			"ag_gw_ip_method"//"ipv4_ip_method"
#define AG_IP_GATEWAY				"ag_gw_ip_static"//"ipv4_ip_gw"

#define AG_IP_INTERNEL				"ag_internel_ip"
#define AG_PORT_INTERNEL			"ag_internel_port"

#define DEFAULT_IP_INTERNEL		"127.0.0.1"
#define DEFAULT_PORT_INTERNEL		"300"

#define AG_LAN_IP_CONFIG			UCI_NETWORK"lan.ipaddr"

#define AG_WAN_PROTO				UCI_NETWORK"wan.proto"

#define AG_WAN_IFNAME				UCI_NETWORK"wan.ifname"

#define AG_WAN_ADDR				"ipv4_0_ip_address"
#define AG_HOTSPOT_NAME			UCI_GATEWAY"device_hotspot_name"

#define AG_GW_WAN_MAC			"server_wan_mac"

#define AG_STA_DETECT_ENABLE		"ag_sta_detect"

#define AG_DEV_NAME				UCI_SYSTEM"device_name"

#define AG_DEBUG_CONFIG			"ag_debug"

/*config nvram*/
#define AG_CONFIG_LOCATION				UCI_GATEWAY"ag_config_location"
#define AG_CONFIG_LOCATION_DEFAULT	UCI_GATEWAY"first floor"

#define AG_CONFIG_CHANNEL_24		"wifi_0_channel"
#define AG_CONFIG_CHANNEL_5		"wifi_1_channel"
#define AG_CONFIG_MEMO			UCI_GATEWAY"ag_config_memo"
#define AG_CONFIG_TOTAL_LINK		UCI_GATEWAY"ag_config_total_link"

#define AP_WORK_MODE "ap_work_mode"
#else

#define NET_FORWARD_MODE			"net_forward_mode"
#define DEFAULT_SERVER_PORT		"15513"
#define AG_SERVER_IP_CONFIG		"ag_gw_ip_dynamic"	//"gbverdor_nv"
#define AG_SERVER_PORT_CONFIG		"ag_server_port"
#define AG_IP_GET_METHOD			"ag_gw_ip_method"//"ipv4_ip_method"
#define AG_IP_GATEWAY				"ag_gw_ip_static"//"ipv4_ip_gw"

#define AG_IP_INTERNEL				"ag_internel_ip"
#define AG_PORT_INTERNEL			"ag_internel_port"

#define DEFAULT_IP_INTERNEL		"127.0.0.1"
#define DEFAULT_PORT_INTERNEL		"300"

#define AG_LAN_IP_CONFIG			"lan_ip"

#define AG_WAN_ADDR				"ipv4_0_ip_address"
#define AG_HOTSPOT_NAME			"device_hotspot_name"

#define AG_GW_WAN_MAC			"server_wan_mac"

#define AG_STA_DETECT_ENABLE		"ag_sta_detect"

#define AG_DEV_NAME				"device_name"

#define AG_DEBUG_CONFIG			"ag_debug"

/*config nvram*/
#define AG_CONFIG_LOCATION		"ag_config_location"
#define AG_CONFIG_LOCATION_DEFAULT	"first floor"

#define AG_CONFIG_CHANNEL_24		"wifi_0_channel"
#define AG_CONFIG_CHANNEL_5		"wifi_1_channel"
#define AG_CONFIG_MEMO			"ag_config_memo"
#define AG_CONFIG_TOTAL_LINK		"ag_config_total_link"

#define AP_WORK_MODE "ap_work_mode"
#endif

#define FILE_PROC_STAT 	"/proc/stat"
#define FILE_PROC_NET_DEV 	"/proc/net/dev"
#define AC_LINK_STATE_FILE_PATH		"/tmp/aclinkstate"
#define PRODUCT_INFO_PATH   	"/etc/.product_info"


#define AG_APHWTYPE_IXP			0			/* 硬件类型为IXP */
#define AG_APHWTYPE_AP93			1			/* 硬件类型为AP93 */
#define AG_APHWTYPE_PB44			2			/* 硬件类型为PB44 */
#define AG_APHWTYPE_PB44_2		3			/* 硬件类型为自研PB44硬件平台*/
#define AG_APhWTYPE_9344			4			/* 硬件类型为9344平台 */
#define AG_APHWTYPE_TA40			5			/* 硬件类型为WallAP平台*/
#define AG_APHWTYPE_LTEFI        		6           /* 硬件类型为LTEFI平台 */
/* BEGIN: Added by lvjianlin, 2013/7/24   PN:AP9331_OAM */
#define AG_APHWTYPE_TA33         		7           /* 硬件类型为TA33平台 */
/* END:   Added by lvjianlin, 2013/7/24   PN:AP9331_OAM */
/* BEGIN: Added by wangdusun, 2013/8/22   PN:TA55 */
#define AG_APHWTYPE_TA55         		8           /* 硬件类型为TA55平台 */
#define AG_APHWTYPE_TA37         		9           /* 硬件类型为TA37平台 */
#define AG_APHWTYPE_TA62         		10           /* 硬件类型为TA62平台 */



#define SEND_TIMEOUT_VALUE	10

#define DEFAULT_WAN_IP  	"192.168.1.1"


/*
*   AP-GW通信相关
*/ 

/*
*  连接维护消息
*/
#define	AGLINK_REGISTER_MSG			1
#define DEV_REGISTER_ACK_MSG		2
#define DEV_NAK_MSG				3
#define DEV_KEEPALIVE_MSG			4
#define DEV_KEEPALIVE_ACK_MSG		5
//#define DEV_LOGOUT_MSG				6
#define DEV_LOGOUT_ACK_MSG		7



enum DEV_TYPE
{
	DEV_TYPE_GW = 0,
	DEV_TYPE_AP
};

enum ag_debug_level
{
	DEBUG_LEVER_NONE = 0,
	DEBUG_LEVER_COMMON,
	DEBUG_LEVER_VERBOSE
};

#if 0
struct ap_node
{
	struct sockaddr_in sk_addr;			/*AP sock addr*/
	u8 ip[IP_LEN];						/*AP IP addr*/
	u8 mac[MAC_LEN]; 					/*AP WAN口MAC地址*/
	s8 dev_name[64];					/*AP device name*/
	u32 model_id;						/*AP设备model id*/
	struct timeval time; 	  			/*AP最近在线时间点*/
	struct hlist_node id_hlist;
};
#endif

typedef struct{
	u64 user;
	u64 nice;
	u64 system;
	u64 idle;
	u64 iowait;
	u64 irq;
	u64 softirq;
	u64 stealstolen;
	u64 guest;
}AGLINK_CPU_INFO;

typedef struct
{
	u32 total;
	u32 used;
}AGLINK_DISK_INFO;

typedef struct
{
	u32 cpu_rate;
	u32 system;
	u32 user;
	u32 nice;
	u32 idle;
}AGLINK_CPU_RATE;

typedef struct
{
	u32 RecBytes;	//接收字节数
	u32 SendBytes;	//发送字节数
}AGLINK_WAN_STAT;

typedef struct
{
    	s32 freq;           /*2.4:1, 5:2*/
    	s32 limit_txpower;  	/* 最大功率 */
	s32 iflag;			/*0:无效, 1:有效*/
}AGLINK_WIFI_INFO;

#define AGLINK_WIFI_NUM        2

typedef struct aglink_dev_info
{
#define DEV_TYPE_LEN		2
#define HOTSPOT_NAME_LEN	64
	u8 phy_mac[MAC_LEN];					/*设备物理口MAC地址*/
	u8 ip[IP_LEN];							/*设备WAN口IP地址*/
	u8 lan_mac[MAC_LEN]; 				/*设备LAN口MAC地址*/
	u8 lan_ip[IP_LEN];						/*设备LAN口IP地址*/
	u8 dev_type;							/*设备类型AP or Gateway*/
	s8 dev_name[64];						/*设备名称*/
	s8 hotspot_name[HOTSPOT_NAME_LEN];		/*热点名称*/
	s32 model_id;							/*设备model id*/
	s32 ap_work_mode;						/*胖瘦模式*/
	AGLINK_WIFI_INFO	stAgwifiInfo[AGLINK_WIFI_NUM];		/*wifi 信息*/
}AGLINK_DEV_INFO;

struct wifidog_mgt
{
	s32 sockfd;
	struct event rcv_ev;				/*接收数据event*/
	struct event cnnt_timer_ev;			/*连接Server定时器*/

	struct timeval cnnt_tv;
	
	u8 buf[AG_BUFFER_SIZE];  		/*unix socket 数据接收、发送buf*/
};
struct boxll_mgt
{
	s32 sockfd;
	struct event rcv_ev;				/*接收数据event*/
	struct event cnnt_timer_ev; 		/*连接Server定时器*/

	struct timeval cnnt_tv;

	struct event report_ap_info_ev;
	struct timeval ap_info_tv;
	u8 buf[MAXLEN]; 		/*unix socket 数据接收、发送buf*/
};

#define AG_SSID_UPDATE		1
#define AG_SSID_ADD		2
#define AG_SSID_DEL			3

typedef struct aglink_managment
{
	s32 sockfd;								/*UDP socket fd*/
	/* connect  to gw */
	struct sockaddr_in sk_addr;				/*server sock addr*/
	u32 ipaddr;								/*server IP addr*/
	u16 port;								/*server UDP port*/
	u8 mac[MAC_LEN];						/*server WAN mac addr*/

	/* communication  with other module */
	s32 iSockfdInternal;						/* internal socket */
	struct sockaddr_in sk_addr_internal;		/* internal socket address */
	u32 iaddrinternal;						/* internal ip address */
	u16 usportinternal;						/* internal port */

	struct aglink_dev_info *pstdev_info;			/*设备信息*/
	struct wifidog_mgt *w_mgt;				/*aglink-wifidog连接管理结构*/
	#ifdef _BOXLL_
	struct boxll_mgt *b_mgr;              /* aglink-boxll连接管理结构 */
	#endif
	AGLINK_AP_CONFIG_INFO_T *pAgconfig;
	
	struct event_base *main_base;				/*libevent main base*/
	
	struct event rcv_ev;						/*接收数据event*/
	struct event keepalive_timer_ev;			/*设备keepalive 报文发送定时器*/
	struct event ack_check_timer_ev;			/*ACK消息接收情况检查定时器*/
	struct event config_check_timer_ev;		/*配置检查 Event*/
	struct timeval config_check_tv;			/*配置检查 时间间隔*/
	struct event ap_age_timer_ev;			/*AP老化Event*/
	struct timeval ap_age_tv;					/*AP老化时间间隔*/
	
	struct timeval cnnt_tv;					/*Server定时器间隔*/
	struct timeval keepalive_tv;				/*设备keepalive 报文发送间隔*/
	struct timeval ack_check_tv;				/*ACK消息接收情况检查时间间隔*/
	
	u32 keepalive_times;						/*在收到keepalive ACK之前keepalive报文已发送的次数*/
	s32 dev_reg_ack_ok;						/*表示是否收到注册ACK消息*/

	pthread_mutex_t ag_msg_mutex;			/*发送消息锁*/

	struct hlist_head ap_list[AP_HASH_SIZE]; 	/*AP信息链*/
	pthread_mutex_t ap_hlist_mutex;			/*AP信息访问锁*/

	struct hlist_head sta_list[STA_HASH_SIZE]; 	/*终端关联信息链*/
	pthread_mutex_t sta_hlist_mutex;			/*终端关联信息缓存访问锁*/

	enum ag_debug_level debug_level; 			/*调试等级*/
	
}AGLINK_MGT; 


 AGLINK_MGT *gpstaglinkmgt = NULL;

#define IS_DEV_AP() 	(DEV_TYPE_AP == gpstaglinkmgt->pstdev_info->dev_type)
#define IS_DEV_GW() 	(DEV_TYPE_GW == gpstaglinkmgt->pstdev_info->dev_type)


#define AP_HLIST_LOCK()			pthread_mutex_lock(&gpstaglinkmgt->ap_hlist_mutex)
#define AP_HLIST_UNLOCK()		pthread_mutex_unlock(&gpstaglinkmgt->ap_hlist_mutex)

#define STA_HLIST_LOCK()		pthread_mutex_lock(&gpstaglinkmgt->sta_hlist_mutex)
#define STA_HLIST_UNLOCK()		pthread_mutex_unlock(&gpstaglinkmgt->sta_hlist_mutex)

#define AG_MSG_LOCK()			pthread_mutex_lock(&gpstaglinkmgt->ag_msg_mutex)
#define AG_MSG_UNLOCK()		pthread_mutex_unlock(&gpstaglinkmgt->ag_msg_mutex)

#define IPC_LIST_LOCK()			pthread_mutex_lock(&gpstaglinkmgt->ipc_hlist_mutex)
#define IPC_LIST_UNLOCK()		pthread_mutex_unlock(&gpstaglinkmgt->ipc_hlist_mutex)

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

extern int gAglinkDebugLevel;


#define AGLINK_LOG(__level__, __format__, __arg__...) \
	do{ \
	        if(__level__ <= gAglinkDebugLevel)\
	        {\
                    if(LOG_DEBUG == __level__)\
                    {\
			    syslog(LOG_INFO, "[%s][%d] "__format__, __FUNCTION__, __LINE__, ##__arg__);\
                    }\
	            else\
	            {\
			    syslog(__level__, "[%s][%d] "__format__, __FUNCTION__, __LINE__, ##__arg__);\
                   }\
            }\
	}while(0)



		
/*
*  配置相关
*/

int inline AGLINK_CONFIG_SET(char *name, char *value)
{	
	AGLINK_LOG(LOG_INFO, "Set config [%s] -> %s", name, value);
	return nvram_set(name, value);
}

int inline AGLINK_CONFIG_UNSET(char *name)
{
	AGLINK_LOG(LOG_INFO, "Unset config [%s]", name);
	return nvram_unset(name);
}

int inline AGLINK_CONFIG_COMMIT()
{
	AGLINK_LOG(LOG_INFO, "commit config");
	return nvram_commit();
}

int inline AGLINK_CONFIG_WRITE(char *name, char *value)
{	
	int ret;
	AGLINK_LOG(LOG_INFO, "Write config [%s] -> %s", name, value);
	ret = nvram_set(name, value);
	if(ret != 0)
		return ret;
	return nvram_commit();
}

int inline AGLINK_CONFIG_ERASE(char *name)
{
	int ret;
	AGLINK_LOG(LOG_INFO, "Erase config [%s]", name);
	ret = nvram_unset(name);
	if(ret != 0)
		return ret;
	return nvram_commit();
}
		
int inline AGLINK_CONFIG_GET(char *value, char *name, int len)
{
	int ret = nvram_get(value, name, len);
	/*if(0 == ret)
		AGLINK_LOG(LOG_INFO, "Get config [%s] -> %s", name, value);*/
	
	return ret;
}

u32 aglink_ap_send_dev_register_msg();
void aglink_recv_ag_msg(evutil_socket_t fd, short event, void * arg);
s32 aglink_gw_recv_sta_connect_msg(u8 **Msgbuf, u32 usMsgId);
u32 aglink_send_ag_msg(u32 usMsgID, u16 uiMsgType, u8 *pbuff, u32 iMsgLen, u8 *dmac);


/*  有线网口定义 */
#if defined(AP9341_OAM)
#define ETH0    "eth0"
#define ETH1    "eth1"
#elif defined(AP93_OAM)
	#ifdef AP93_SDK95
		#define ETH0    "eth0"
		#define ETH1    "eth1"
	#else
		#define ETH0    "ixp0"
		#define ETH1    "ixp1"
	#endif
#else
#define ETH0    "eth0"
#define ETH1    "eth1"
#if (defined(GW_X86) || defined(GW_ARM) || defined(GW_AC))
#define PPP     "ppp"
#endif
#endif


#endif
