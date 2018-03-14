/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : nf_portal_ioctl.h
  版 本 号   : 初稿
  作    者   : WeiHonggang
  生成日期   : 2013年11月26日 星期五
  最近修改   :
  功能描述   : nf_portal_ioctl.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2013年11月26日 星期五
    作    者   : WeiHonggang
    修改内容   : 创建文件

******************************************************************************/

#ifndef __NF_PORTAL_IOCTL_H__
#define __NF_PORTAL_IOCTL_H__


/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "ap_types.h"
#include "nf_portal_public.h"

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
 * 基础类型定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define NF_PORTAL_PROC_FILE        "nf_portal"

#define NF_PORTAL_IOCTL_CMD_BEGIN 0


#define NF_PORTAL_MOD_VERSION  "v1.0.1"
#define NF_PORTAL_APP_VERSION  "v1.0.1"

#define NF_PORTAL_MOD_DEP_VERSION "v1.0.1"   /*支持内核模块的APP最小版本号*/
#define NF_PORTAL_APP_DEP_VERSION "v1.0.1"   /*支持APP的内核模块最小版本号*/


 /*----------------------------------------------*
 * 宏操作                                       *
 *----------------------------------------------*/


 
/*----------------------------------------------*
 * 枚举定义                                       *
 *----------------------------------------------*/
typedef enum
{
    NF_PORTAL_IOCTL_CMD_GET_VERSION = NF_PORTAL_IOCTL_CMD_BEGIN,
    
    NF_PORTAL_IOCTL_CMD_SET_DEBUG_LEVEL,
    NF_PORTAL_IOCTL_CMD_GET_DEBUG_LEVEL,

    NF_PORTAL_IOCTL_CMD_SET_AUTH_STATE,
    NF_PORTAL_IOCTL_CMD_GET_AUTH_STATE,
    
    NF_PORTAL_IOCTL_CMD_SET_HOST_IP,
    NF_PORTAL_IOCTL_CMD_GET_HOST_IP,

    NF_PORTAL_IOCTL_CMD_SET_AUTH_SERVER_IP,
    NF_PORTAL_IOCTL_CMD_GET_AUTH_SERVER_IP,
    
    NF_PORTAL_IOCTL_CMD_SET_CCS_SERVER_IP,
    NF_PORTAL_IOCTL_CMD_GET_CCS_SERVER_IP,

	NF_PORTAL_IOCTL_CMD_SET_SNMP_SERVER_IP,
    NF_PORTAL_IOCTL_CMD_GET_SNMP_SERVER_IP,
    
    NF_PORTAL_IOCTL_CMD_SET_FREE_AUTH_SW,
    NF_PORTAL_IOCTL_CMD_GET_FREE_AUTH_SW,

    NF_PORTAL_IOCTL_CMD_GET_STA_LIST,
    NF_PORTAL_IOCTL_CMD_STA_LIST_FLUSH,

    NF_PORTAL_IOCTL_CMD_SET_LAN_IF_NAME,
    NF_PORTAL_IOCTL_CMD_GET_LAN_IF_NAME,

    NF_PORTAL_IOCTL_CMD_ADD_DEST_IP_BAW,
    NF_PORTAL_IOCTL_CMD_DEL_DEST_IP_BAW,
    NF_PORTAL_IOCTL_CMD_GET_DEST_IP_BAW_LIST,
    NF_PORTAL_IOCTL_CMD_DEST_IP_BAW_FLUSH,

    NF_PORTAL_IOCTL_CMD_ADD_DEST_NET_BAW,
    NF_PORTAL_IOCTL_CMD_DEL_DEST_NET_BAW,
    NF_PORTAL_IOCTL_CMD_GET_DEST_NET_BAW_LIST,
    NF_PORTAL_IOCTL_CMD_DEST_NET_BAW_FLUSH,

    NF_PORTAL_IOCTL_CMD_ADD_FREE_AUTH_PORT,
    NF_PORTAL_IOCTL_CMD_DEL_FREE_AUTH_PORT,
    NF_PORTAL_IOCTL_CMD_GET_FREE_AUTH_PORT_LIST,
    NF_PORTAL_IOCTL_CMD_FREE_AUTH_PORT_FLUSH,

    NF_PORTAL_IOCTL_CMD_ADD_MAC_BAW,
    NF_PORTAL_IOCTL_CMD_DEL_MAC_BAW,
    NF_PORTAL_IOCTL_CMD_GET_MAC_BAW_LIST,
    NF_PORTAL_IOCTL_CMD_MAC_BAW_FLUSH,

    NF_PORTAL_IOCTL_CMD_ADD_USER_IP_BAW,
    NF_PORTAL_IOCTL_CMD_DEL_USER_IP_BAW,
    NF_PORTAL_IOCTL_CMD_GET_USER_IP_BAW_LIST,
    NF_PORTAL_IOCTL_CMD_USER_IP_BAW_FLUSH,

    NF_PORTAL_IOCTL_CMD_ADD_URL_BAW,
    NF_PORTAL_IOCTL_CMD_DEL_URL_BAW,
    NF_PORTAL_IOCTL_CMD_GET_URL_BAW_LIST,
    NF_PORTAL_IOCTL_CMD_URL_BAW_FLUSH,
    NF_PORTAL_IOCTL_CMD_GET_URL_IP_BAW_LIST,
    NF_PORTAL_IOCTL_CMD_ADD_URL_IP_BAW,

    NF_PORTAL_IOCTL_CMD_ADD_PASS_PORT_NUM,
    NF_PORTAL_IOCTL_CMD_DEL_PASS_PORT_NUM,
    NF_PORTAL_IOCTL_CMD_GET_PASS_PORT_NUM_LIST,
    NF_PORTAL_IOCTL_CMD_PASS_PORT_NUM_FLUSH,

	/*begin: add@2015.01.31 for wechat connect wifi*/
	NF_PORTAL_IOCTL_CMD_ADD_WXL_SERVERIP,
    NF_PORTAL_IOCTL_CMD_DEL_WXL_SERVERIP,
    NF_PORTAL_IOCTL_CMD_GET_WXL_SERVERIP_LIST,
    NF_PORTAL_IOCTL_CMD_WXL_SERVERIP_FLUSH,
	/*end: add@2015.01.31 for wechat connect wifi*/
#ifdef HAVE_MULTI_WAN
	NF_PORTAL_IOCTL_CMD_WAN_CONFIG_SET,
	NF_PORTAL_IOCTL_CMD_WAN_LIST_GET,
	NF_PORTAL_IOCTL_CMD_WAN_MAP_GET,
#endif

    /*begin:lan_free_auth 2015-9-6*/
    NF_PORTAL_IOCTL_CMD_SET_LAN_FREE_AUTH_SW,
    NF_PORTAL_IOCTL_CMD_GET_LAN_FREE_AUTH_SW,
    /*end:lan_free_auth 2015-9-6*/

    /*begin:lan_free_auth 2015-9-18*/
    NF_PORTAL_IOCTL_CMD_SET_NET_CONNECT,
    NF_PORTAL_IOCTL_CMD_GET_NET_CONNECT,
    /*end:lan_free_auth 2015-9-18*/

    NF_PORTAL_IOCTL_CMD_SET_STA_TYPE,
    NF_PORTAL_IOCTL_CMD_GET_STA_TYPE,

    NF_PORTAL_IOCTL_CMD_SET_NET_MODEL,
    NF_PORTAL_IOCTL_CMD_GET_NET_MODEL,

	NF_PORTAL_IOCTL_CMD_GET_VLAN_FLOOD,
	NF_PORTAL_IOCTL_CMD_SET_VLAN_FLOOD
    /*可以对模板中的命令进行修改或增减*/
    /*.......*/
}NF_PORTAL_IOCTL_CMD_E;

/*----------------------------------------------*
 * 扩展类型定义                                     *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 结构定义                                       *
 *----------------------------------------------*/
typedef struct
{
    #define NF_PORTAL_VERSION_SIZE  128
    CHAR acModVersion[NF_PORTAL_VERSION_SIZE];    /*内核模块版本号*/
    CHAR acModDepVersion[NF_PORTAL_VERSION_SIZE]; /*支持内核模块的APP模块最小版本号*/
}NF_PORTAL_IOCTL_VERSION_T;


typedef struct
{
    UINT32 uiDebugLevel;
}NF_PORTAL_IOCTL_DEBUG_T;

typedef struct
{
    UINT8 aucMac[ETH_ALEN];
    UINT32 uiIp;
    NF_PORTAL_AUTH_STATE_E enState;
}NF_PORTAL_IOCTL_AUTH_STATE_T;
typedef struct
{
    UINT8 aucMac[ETH_ALEN];
    UINT32 uiIp;
    NF_PORTAL_STA_TYPE_E enStaType;
}NF_PORTAL_IOCTL_STA_TYPE_T;

typedef struct
{
    UINT32 uiIp;
}NF_PORTAL_IOCTL_IP_T;

typedef struct
{
    NF_PORTAL_SW_STATE_E enSW;
}NF_PORTAL_IOCTL_SW_T;
typedef struct
{
    NF_PORTAL_NET_MODEL_E enNetModel;
}NF_PORTAL_IOCTL_NET_MODEL_T;

typedef struct
{
    #define NF_PORTAL_STA_LIST_MAX 8
    UINT32 uiStartNum;
    UINT32 uiNum;
    NF_PORTAL_AUTH_STATE_E enState;
    NF_PORTAL_STA_ATTRIB_T astSta[NF_PORTAL_STA_LIST_MAX];
}NF_PORTAL_IOCTL_STA_LIST_T;

typedef struct
{
    UINT8 aucLanIfName[IFNAMSIZ];
}NF_PORTAL_IOCTL_IF_NAME_T;

typedef struct
{
    NF_PORTAL_IP_BAW_ATTRIB_T stIpBaw;
}NF_PORTAL_IOCTL_IP_BAW_T;
typedef struct
{
    #define NF_PORTAL_IP_BAW_LIST_MAX 8
    UINT32 uiStartNum;
    UINT32 uiNum;
    NF_PORTAL_BAW_FLAG_E enFlag;
    NF_PORTAL_IP_BAW_ATTRIB_T astIpBaw[NF_PORTAL_IP_BAW_LIST_MAX];
}NF_PORTAL_IOCTL_IP_BAW_LIST_T;
typedef struct
{
    NF_PORTAL_NET_BAW_ATTRIB_T stNetBaw;
}NF_PORTAL_IOCTL_NET_BAW_T;
typedef struct
{
    #define NF_PORTAL_NET_BAW_LIST_MAX 8
    UINT32 uiStartNum;
    UINT32 uiNum;
    NF_PORTAL_BAW_FLAG_E enFlag;
    NF_PORTAL_NET_BAW_ATTRIB_T astNetBaw[NF_PORTAL_NET_BAW_LIST_MAX];
}NF_PORTAL_IOCTL_NET_BAW_LIST_T;

typedef struct
{
    #define NF_PORTAL_PORT_LIST_MAX 8
    UINT32 uiStartNum;
    UINT32 uiNum;
    NF_PORTAL_IOCTL_IF_NAME_T astPort[NF_PORTAL_PORT_LIST_MAX];
}NF_PORTAL_IOCTL_PORT_LIST_T;

typedef struct
{
    NF_PORTAL_MAC_BAW_ATTRIB_T stMacBaw;
}NF_PORTAL_IOCTL_MAC_BAW_T;
typedef struct
{
    #define NF_PORTAL_MAC_BAW_LIST_MAX 8
    UINT32 uiStartNum;
    UINT32 uiNum;
    NF_PORTAL_BAW_FLAG_E enFlag;
    NF_PORTAL_MAC_BAW_ATTRIB_T astMacBaw[NF_PORTAL_MAC_BAW_LIST_MAX];
}NF_PORTAL_IOCTL_MAC_BAW_LIST_T;

typedef struct
{
    NF_PORTAL_URL_BAW_ATTRIB_T stUrlBaw;
}NF_PORTAL_IOCTL_URL_BAW_T;

typedef struct
{
    #define NF_PORTAL_URL_BAW_LIST_MAX 4
    UINT32 uiStartNum;
    UINT32 uiNum;
    NF_PORTAL_BAW_FLAG_E enFlag;
    NF_PORTAL_URL_BAW_ATTRIB_T astUrlBaw[NF_PORTAL_URL_BAW_LIST_MAX];
}NF_PORTAL_IOCTL_URL_BAW_LIST_T;

typedef struct
{
    UINT32 uiIp;
    UINT8 aucUrl[URL_MAX_SIZE];
}NF_PROTAL_IOCTL_URL_IP_BAW_T;
typedef struct
{
     #define NF_PORTAL_URL_IP_BAW_LIST_MAX 4
    UINT32 uiStartNum;
    UINT32 uiNum;
    NF_PORTAL_BAW_FLAG_E enFlag;
    NF_PROTAL_IOCTL_URL_IP_BAW_T astUrlIpBaw[NF_PORTAL_URL_IP_BAW_LIST_MAX];   
}NF_PORTAL_IOCTL_URL_IP_BAW_LIST_T;

typedef struct
{
    NF_PORTAL_PORT_NUM_ATTRIB_T stPortNum;
}NF_PORTAL_IOCTL_PORT_NUM_T;

typedef struct
{
    #define NF_PORTAL_PORT_NUM_LIST_MAX 8
    UINT32 uiStartNum;
    UINT32 uiNum;
    UINT8 ucIpProto;
    NF_PORTAL_IOCTL_PORT_NUM_T astPortNum[NF_PORTAL_PORT_NUM_LIST_MAX];
}NF_PORTAL_IOCTL_PORT_NUM_LIST_T;
/*begin: add@2015.01.31 for wechat connect wifi*/
typedef struct
{
	NF_PORTAL_WXL_SERVERIP_ATTRIB_T stWxlServerIp;
}NF_PORTAL_IOCTL_WXL_SERVERIP_T;
typedef struct
{
	#define NF_PORTAL_WXL_SERVERIP_NUM_LIST_MAX 8
	UINT32 uiStartNum;
	UINT32 uiNum;
	NF_PORTAL_WXL_SERVERIP_ATTRIB_T astWxlServerip[NF_PORTAL_WXL_SERVERIP_NUM_LIST_MAX];
}NF_PORTAL_IOCTL_WXL_SERVERIP_LIST_T;
/*end: add@2015.01.31 for wechat connect wifi*/

#ifdef HAVE_MULTI_WAN
typedef struct
{
	UINT32 uiWanIndex; //wan1 1 wan2 2 wan3 3
	UINT32 uiEnable;   // enable 1 disable 0
	UINT32 uiBand;     
	UINT32 uiFlagNum;  
}NF_PORTAL_IOCTL_WAN_CONF_T;

typedef struct
{
	#define NF_PORTAL_WAN_MAP_LIST_MAX 100
	UINT32 uiStartNum;
    UINT32 uiNum;
	UINT32 auiWanMap[NF_PORTAL_WAN_MAP_LIST_MAX];
}NF_PORTAL_IOCTL_WAN_MAP_T;

typedef struct
{
	#define NF_PORTAL_WAN_CONF_NUM 3
	UINT32 uiNum;
	NF_PORTAL_IOCTL_WAN_CONF_T astWanConf[NF_PORTAL_WAN_CONF_NUM];
}NF_PORTAL_IOCTL_WAN_LIST_T;
#endif

typedef struct
{
	BOOL bEnable;
}NF_PORTAL_IOCTL_VLAN_FLOOD_T;


/*可以对模板中的命令进行修改或增减*/
/*.......*/

typedef struct
{
	NF_PORTAL_IOCTL_CMD_E enCmd;

	union
	{
		NF_PORTAL_IOCTL_VERSION_T stVersion;
		NF_PORTAL_IOCTL_DEBUG_T stDebug;
		NF_PORTAL_IOCTL_AUTH_STATE_T stIoAuthState;
		NF_PORTAL_IOCTL_IP_T stIoIp;
		NF_PORTAL_IOCTL_SW_T stIoSW;
		NF_PORTAL_IOCTL_STA_LIST_T stIoStaList;
		NF_PORTAL_IOCTL_IF_NAME_T stIoIfName;
		NF_PORTAL_IOCTL_IP_BAW_T stIoIpBaw;
		NF_PORTAL_IOCTL_IP_BAW_LIST_T stIoIpBawList;
		NF_PORTAL_IOCTL_NET_BAW_T stIoNetBaw;
		NF_PORTAL_IOCTL_NET_BAW_LIST_T stIoNetBawList;
		NF_PORTAL_IOCTL_PORT_LIST_T stIoPortList;
		NF_PORTAL_IOCTL_MAC_BAW_T stIoMacBaw;
		NF_PORTAL_IOCTL_MAC_BAW_LIST_T stIoMacBawList;
		NF_PORTAL_IOCTL_URL_BAW_T stIoUrlBaw;
		NF_PORTAL_IOCTL_URL_BAW_LIST_T stIoUrlBawList;
		NF_PROTAL_IOCTL_URL_IP_BAW_T stIoUrlIpBaw;
		NF_PORTAL_IOCTL_URL_IP_BAW_LIST_T stIoUrlIpBawList;
		NF_PORTAL_IOCTL_PORT_NUM_T stIoPortNum;
		NF_PORTAL_IOCTL_PORT_NUM_LIST_T stIoPortNumList;
		/*begin: add@2015.01.31 for wechat connect wifi*/
		NF_PORTAL_IOCTL_WXL_SERVERIP_T stIoWxlServerIp;
		NF_PORTAL_IOCTL_WXL_SERVERIP_LIST_T stIoWxlServerIpList;
		/*end: add@2015.01.31 for wechat connect wifi*/
		NF_PORTAL_IOCTL_STA_TYPE_T stIoStaType;
        NF_PORTAL_IOCTL_NET_MODEL_T stIoNetModel;
       
#ifdef HAVE_MULTI_WAN
		NF_PORTAL_IOCTL_WAN_CONF_T stIoWanCfg;
		NF_PORTAL_IOCTL_WAN_LIST_T stIoWanList;
		NF_PORTAL_IOCTL_WAN_MAP_T stIoWanMap;
#endif
		NF_PORTAL_IOCTL_VLAN_FLOOD_T stIoVlanFlood;
	    /*可以对模板中的命令进行修改或增减*/
        /*.......*/
	}unData;
}NF_PORTAL_IOCTL_T;




/*----------------------------------------------*
 * 导出函数原型说明                             *
 *----------------------------------------------*/



#endif /* __NF_PORTAL_IOCTL_H__ */

