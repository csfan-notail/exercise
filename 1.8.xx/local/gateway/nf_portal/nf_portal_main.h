/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : nf_portal_main.h
  版 本 号   : 初稿
  作    者   : WeiHonggang
  生成日期   : 2013年11月22日 星期五
  最近修改   :
  功能描述   : nf_portal_main.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2013年11月22日 星期五
    作    者   : WeiHonggang
    修改内容   : 创建文件

******************************************************************************/

#ifndef __NF_PORTAL_MAIN_H__
#define __NF_PORTAL_MAIN_H__


/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/jiffies.h>
#include <linux/errno.h>
#include <linux/version.h>

#include <linux/skbuff.h>
#include <linux/jhash.h>

#include <linux/netdevice.h>
#include <linux/if_ether.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_bridge.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#if (defined(GW_X86) || defined(GW_ARM))   //兼容x86网关的内核版本     by cj
#include <linux/rwlock_types.h>
#include <../net/bridge/br_private.h>
#if defined(GW_ARM)
#ifndef BIG_ENDIAN
#define BIG_ENDIAN 
#endif
#endif
#else
#ifdef OPENWRT
#include "br_private.h"
#else
#include "../bridge/br_private.h"
#endif
#if (!defined(GW_AC))   //by cj
#ifndef BIG_ENDIAN
#define BIG_ENDIAN 
#endif
#endif
#endif
#include "ap_types.h"
#include "nf_portal_public.h"

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
 * 基础类型定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
//#define NF_IP_PRI_PORTAL (NF_IP_PRI_MANGLE-1) /*钩子函数优先级*/
#define NF_IP_PRI_PORTAL (NF_BR_PRI_FILTER_BRIDGED-1) /*钩子函数优先级*/

/* BEGIN: Added by WeiHonggang, 2014/3/5   PN:BUG #5088不同VLAN接口加到同一个桥下会导致的广播环路问题  */
//#define NF_PORTAL_HOOK_NUM 2 /*钩子函数数目*/
#define NF_PORTAL_HOOK_NUM 3 /*钩子函数数目*/
/* END:   Added by WeiHonggang, 2014/3/5   PN:BUG #5088不同VLAN接口加到同一个桥下会导致的广播环路问题  */
#define IP_HLEN 20 /*IP 头长度*/
#define UDP_HLEN 8 /*UDP头长度*/
#define TCP_HLEN 20 /*TCP头长度*/
#define DNS_HLEN 12 /*DNS头长度*/
#define BOOTPC 0x44
#define BOOTPS 0x43
#define DNSS 0x35
#define HTTPS 0x50
/* add@2014.9.24 PN:中核高通需求，需要拦截8008端口的tcp报文，做重定向*/
#define HTTPS_FORCUSCOM 0x1F48

#define VLAN_HLEN 4

#define PORTAL_NORMAL_MARK  1
#define PORTAL_SPECIAL_MARK 33
#define PORTAL_PROXY_MARK 35

#ifdef GW_X86
#define PORTAL_MARK_NUM		4
#else
#define PORTAL_MARK_NUM		1
#endif

#define DEF_LAN_IF_NAME "br"
#define DEF_HOST_IP 0xAC100001			/*172.16.0.1*/
#define DEF_AHTH_SERVER_IP 0x731DACF8  /*115.29.172.248 www.login-wifi.com*/
#define DEF_CCS_SERVER_IP 0x731DACF8	/*115.29.172.248 www.login-wifi.com*/
#define DEF_SNMP_SERVER_IP 0x731DACF8	/*115.29.172.248 www.login-wifi.com*/
#define DEF_SPEC_IP 0x0A010006  		/*10.1.0.6，用于微信连、支付宝认证*/

#if defined (BIG_ENDIAN)
#define DNS_FLAG_QR_R 1
#define DNS_FLAG_OPCODE_S 0
#define DNS_FLAG_TC_NO 0
#define DNS_FLAG_RCODE_OK 0

#else
#define ROOT_DNS_SERVER_FLAGS 0x8180
#define AUTHORITY_DNS_SERVER_FLAGS 0x8580
#endif
#define DNS_ANSWER_TYPE_A 0x0001
/*add@2015.01.31 for wechat connect wifi*/
#define NF_PORTAL_IS_WXL_SERVER_IP 1
#define NF_PORTAL_ISNOT_WXL_SERVER_IP 0


 /*----------------------------------------------*
 * 宏操作                                       *
 *----------------------------------------------*/
#define NF_PORTAL_LOG(level, fmt, args...)                  \
do{											    \
	if (level <= g_enDebugLevel)					        \
	{											\
		printk("NF_PORTAL %s(%d): ",__FUNCTION__, __LINE__); \
		printk(fmt, ##args);				    \
	}										    \
}while(0)

/*----------------------------------------------*
 * 枚举定义                                       *
 *----------------------------------------------*/
typedef enum
{
    NF_PORTAL_LOG_EMERG = 0,	/* system is unusable */
    NF_PORTAL_LOG_ALERT,	/* action must be taken immediately */
    NF_PORTAL_LOG_CRIT,	/* critical conditions */
    NF_PORTAL_LOG_ERR,	/* error conditions */
    NF_PORTAL_LOG_WARNING,	/* warning conditions */
    NF_PORTAL_LOG_NOTICE,	/* normal but significant condition */
    NF_PORTAL_LOG_INFO,	/* informational */
    NF_PORTAL_LOG_DEBUG,	/* debug-level messages */
}NF_PORTAL_LOG_LEVEL_E;

/*----------------------------------------------*
 * 扩展类型定义                                     *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 结构定义                                       *
 *----------------------------------------------*/

typedef struct
{
    NF_PORTAL_STA_ATTRIB_T stSta;/*STA 属性信息 */
    ULONG ulAccessTime; /*最新访问时间*/
    struct hlist_node stStaHList;/*用于组织STA Hash表*/
}NF_PORTAL_STA_ENTRY_T;

typedef struct
{
    struct ethhdr stEthHdr;
    UINT16 usVlanId;
    UINT16 usProto;
}__attribute__((packed)) NF_PORTAL_8021Q_HDR_T;
typedef struct
{
    NF_PORTAL_8021Q_HDR_T st8021qHdr;
    UINT16 usVlanId;
    UINT16 usProto;
}__attribute__((packed)) NF_PORTAL_QINQ_HDR_T;

typedef struct
{
    UINT16 usTransactionId;/*会话标识*/
    #if defined (BIG_ENDIAN) /*标志位*/
    UINT16 QR:1,
               opcode:4,
               AA:1,
               TC:1,
               RD:1,
               RA:1,
               zero:3,
               rcode:4;
    #else
    /*
    UINT16 RA:1,
               zero:3,
               rcode:4,
               QR:1,
               opcode:4,
               AA:1,
               TC:1,
               RD:1;
   */
   UINT16 usFlags;
    #endif
    UINT16 usQuestions;/*问题数*/
    UINT16 usAnswerRRS;/*应答数*/
    UINT16 usAuthorityRRS;/*授权数*/
    UINT16 usAdditionalRRS;/*额外信息数*/
}__attribute__((packed)) NF_PORTAL_DNS_HDR_T;

typedef struct
{
    NF_PORTAL_IP_BAW_ATTRIB_T stIpBAW;
    struct hlist_node stIpBAWHList;
}NF_PORTAL_IP_BAW_ENTRY_T;
typedef struct
{
    NF_PORTAL_NET_BAW_ATTRIB_T stNetBAW;
    struct hlist_node stNetBAWHList;
}NF_PORTAL_NET_BAW_ENTRY_T;

typedef struct
{
    UINT8 aucPortName[IFNAMSIZ];
    struct hlist_node stPortHList;
}NF_PORTAL_PORT_ENTRY_T;

typedef struct
{
    NF_PORTAL_MAC_BAW_ATTRIB_T stMacBAW;
    struct hlist_node stMacBAWHList;
}NF_PORTAL_MAC_BAW_ENTRY_T;

typedef struct
{
    NF_PORTAL_URL_BAW_ATTRIB_T stUrlBAW;
    struct list_head stUrlBAWList;
    struct list_head stIpList;
}NF_PORTAL_URL_BAW_ENTRY_T;

typedef struct
{
    NF_PORTAL_IP_BAW_ATTRIB_T stIpBAW;
    struct hlist_node stIpBAWHList;
    struct list_head stIpList;
    NF_PORTAL_URL_BAW_ENTRY_T *pstUrlBaw;
    UINT8 aucRealUrl[URL_MAX_SIZE];
}NF_PORTAL_URL_IP_BAW_ENTRY_T;

typedef struct
{
    NF_PORTAL_PORT_NUM_ATTRIB_T stPortNum;
    struct hlist_node stPortNumHList;
}NF_PORTAL_PORT_NUM_ENTRY_T;

/*begin: add@2015.01.31 for wechat connect wifi*/
typedef struct
{
	UINT32 uiMask;
	UINT32 uiIp;
	struct hlist_node stWxlServerIpHlist;
}NF_PORTAL_WXL_SERVERIP_ENTRY_T;
/*end: add@2015.01.31 for wechat connect wifi*/


#ifdef HAVE_MULTI_WAN
/*wan 口标记*/
typedef struct
{
	UINT32 uiWanIndex;  //wan口标记
}NF_PORTAL_WAN_FLAG_T;

/*wan 口配置*/
#define NF_PORTAL_WAN_MAX_NUM 3
#define NF_PORTAL_WAN_MAP_SIZE 100
enum {
	NF_PORTAL_WAN1 = 1,
	NF_PORTAL_WAN2 = 2,
	NF_PORTAL_WAN3 = 3
};
enum {
	NF_PORTAL_WAN_STRATEGY_STATIC_IP = 0,
	NF_PORTAL_WAN_STRATEGY_SOURCE_IP = 1,
	NF_PORTAL_WAN_STRATEGY_SESSION = 2
};
typedef struct
{
	UINT32 uiWanIndex;     //wan 口索引
	UINT32 uiEnable;       //wan 口是否使能 1 使能 0 禁用
	UINT32 uiWanMaxBand;   //wan 口最大带宽值 单位Mbps

	UINT32 uiFlagNum;      //wan 标记数组中占的数组元素数量
}NF_PORTAL_WAN_CFG_T;
#endif

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 导出函数原型说明                             *
 *----------------------------------------------*/
 
AP_ERROR_CODE_E nf_portal_set_debugLevel(UINT32 uiDebugLevel);
UINT32 nf_portal_get_debugLevel(void);

AP_ERROR_CODE_E nf_portal_init(void);
AP_ERROR_CODE_E nf_portal_destroy(void);
UINT32 nf_portal_get_host_ip(VOID);
AP_ERROR_CODE_E nf_portal_set_host_ip(UINT32 uiIp);
AP_ERROR_CODE_E nf_portal_set_auth_server_ip(UINT32 uiIp);
UINT32 nf_portal_get_auth_server_ip(VOID);
AP_ERROR_CODE_E nf_portal_set_free_auth_sw(NF_PORTAL_SW_STATE_E enGlobalFreeAuth);
NF_PORTAL_SW_STATE_E nf_portal_get_free_auth_sw(VOID);
AP_ERROR_CODE_E nf_portal_set_lan_name(UINT8 *pucLanIfName);
UINT8 *nf_portal_get_lan_name(VOID);
AP_ERROR_CODE_E nf_portal_set_ccs_server_ip(UINT32 uiIp);
UINT32 nf_portal_get_ccs_server_ip(VOID);
AP_ERROR_CODE_E nf_portal_set_snmp_server_ip(UINT32 uiIp);
UINT32 nf_portal_get_snmp_server_ip(VOID);
/*----------------------------------------------*
 * 导出变量说明                             *
 *----------------------------------------------*/
REMOTE NF_PORTAL_LOG_LEVEL_E g_enDebugLevel;

#ifdef HAVE_MULTI_WAN
//wan 标记以WAN口顺序1,2,3错开，如果有两个按1,2错开，默认全为1. 
REMOTE NF_PORTAL_WAN_FLAG_T g_astWanFlag[NF_PORTAL_WAN_MAP_SIZE];
//LOCAL DEFINE_RWLOCK(g_stWanFlagLock);
REMOTE NF_PORTAL_WAN_CFG_T  g_astWanCfg[NF_PORTAL_WAN_MAX_NUM];
AP_ERROR_CODE_E nf_portal_wanflag_init(VOID);
AP_ERROR_CODE_E nf_portal_wanflag_build(VOID);
#endif

/*begin:add @2015-9-6 for lan_free_auth*/
AP_ERROR_CODE_E nf_portal_set_lan_free_auth_sw(NF_PORTAL_SW_STATE_E enGlobalFreeAuth);
NF_PORTAL_SW_STATE_E nf_portal_get_lan_free_auth_sw(VOID);
/*end:add @2015-9-6 for lan_free_auth*/

/*begin:add @2015-9-6 for lan_free_auth*/
AP_ERROR_CODE_E nf_portal_set_net_connect(NF_PORTAL_SW_STATE_E enGlobalNetConnect);
NF_PORTAL_SW_STATE_E nf_portal_get_net_connect(VOID);
/*end:add @2015-9-6 for lan_free_auth*/

AP_ERROR_CODE_E nf_portal_set_net_model(NF_PORTAL_NET_MODEL_E enGlobalNetModel);
NF_PORTAL_NET_MODEL_E nf_portal_get_net_model(VOID);

AP_ERROR_CODE_E nf_portal_set_vlan_flood(BOOL bEnable);
BOOL nf_portal_get_vlan_flood(VOID);


#endif /* __NF_PORTAL_MAIN_H__ */

