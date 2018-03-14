/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : om_net.h
  版 本 号   : 初稿
  作    者   : zhangjiasi
  生成日期   : 2014年5月4日
  最近修改   :
  功能描述   : om_net.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2014年5月4日
    作    者   : zhangjiasi
    修改内容   : 创建文件

******************************************************************************/
#ifndef __OM_NET_H__
#define __OM_NET_H__
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

#ifndef IP_OR_MAC_LEN
#define IP_OR_MAC_LEN         32       /*IP或者MAC的长度*/
#endif

#include "om_types.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#define IP_DOT_LEN 16  /*点分式IP长度*/


#define LAN_BR_NAME             "br0"
#define LAN_IF_NAME0            "eth0"
#define LAN_IF_NAME1            "eth1"
#define LAN_IF_ALL              "all"
#define LAN_VAP_NAME0           "vap0_0"
#define WAN_IF_NAME_WIRED       "eth0"
#define WAN_IF_NAME_LTE         "usb0"
#define WAN_IF_NAME_NONE        "none"

#define WAN_IF_NAME_PPP0        "ppp0"


/*PPPOE*/
#define PPPOE_MAX_REMOTE_IP_NUM 2000
#define PPPOE_DEF_REMOTE_IP_NUM  100
#define PPPOE_DEF_SERVER_NAME   "GBCOM-PPPOE"
#define PPPOE_MAX_NAME_NUM      16

#define PPPOE_SERVER_DEFAULT_CONFIG_FILE1   "/etc/ppp/pap-secrets"
#define PPPOE_SERVER_DEFAULT_CONFIG_FILE2   "/etc/ppp/chap-secrets"

/* Dev */
#define WEB_DEV_LIST_WAN_NUM        0
#define WEB_DEV_LIST_LAN_NUM        1
#define WEB_DEV_LIST_LTE_NUM        2

#define LTECPE_PLATFORM_OUTSIDE    "ltecpe-ta401-00"
#define LTECPE_PLATFORM_INSIDE     "ltecpe-ta401-21"


/*NAT*/
#define OM_NAT_RULE_MAX 32
/*ROUTE*/
#define OM_ROUTE_STATIC_MAX 32
/*MAC ACL*/
#define OM_MAC_ACL_MAX 32

/* ARP */
#define OM_ARP_NUM_MAX      32

#define WHITE_MAC_LIST_CHAIN "whitelist"

#define BLACK_MAC_LIST_CHAIN "blacklist"

#define WHITE_MAC_LIST_TYPE 1

#define BLACK_MAC_LIST_TYPE 0

/*IP ACL*/
#define OM_IP_ACL_MAX 32

#define ACL_RULE_BUILD 0
#define ACL_RULE_DESTORY 1

#define IP_ACL_INPUT_CHAIN "ip_acl_input"

#define IP_ACL_FORWARD_CHAIN "ip_acl_forward"
#define DEFAULT_AUTH_FILE "/etc/xl2tpd/l2tp-secrets"
/* BW LIST */
#define ADD_DEL_IP_ADDR             1
#define ADD_DEL_URL_ADDR            2
#define BWLIST_NUM_MAX              32
#define BLACK_LIST_NVRAM_PRE_FORMATE    "black_list"
#define WHITE_LIST_NVRAM_PRE_FORMATE    "white_list"

/* check Alive模块内部模式使用地址 */
#define LTE_CHECKALIVE_DEFAULT_ADDR1            "202.108.22.5"
#define LTE_CHECKALIVE_DEFAULT_ADDR2            "223.5.5.5"


#define DHCPV6_SERVER_CONFIG_FILE_BAK   "/etc/dhcpdv6.conf.tmp"
#define DHCPV6_SERVER_CONFIG_FILE       "/tmp/dhcpdv6.conf"
#define DHCPV6_SERVER_LEASE_FILE        "/tmp/dhcpd6.leases"

#define LTE_CHECKALIVE_IP_FILE          "/tmp/checkaliveIP"
/************************************* 结构体定义 ******************************/
typedef enum 
{
    WAN_ACCESS_MODE_WIRED =1,          
    WAN_ACCESS_MODE_LTE=2              
}WAN_ACCESS_MODE_E;

typedef enum 
{
    WAN_FORWARD_MODE_BRIDGE=1,
    WAN_FORWARD_MODE_ROUTE=2          
}WAN_FORWARD_MOD_E;

typedef enum 
{
    WAN_IP_MODE_DHCP        =1,
    WAN_IP_MODE_STATIC      =2,
    WAN_IP_MODE_PPPOE       =3,
    WAN_IP_MODE_PPPOE_DHCP  =4       
}WAN_IP_MODE_E;


typedef enum 
{
    PPPOE_L2TP_MODE_PPPOE   =1,
    PPPOE_L2TP_MODE_L2TP    =2
}PPPOE_L2TP_MODE_E;


typedef enum 
{
    PPPOE_L2TP_SWICTH_OFF        =0,
    PPPOE_L2TP_SWICTH_ON      =1,   
}PPPOE_L2TP_SWITCH_E;

typedef enum 
{
    PPPOE_AUTH_PAP        =0,
    PPPOE_AUTH_CHAP      =1,   
}PPPOE_AUTH_MODE_E;


typedef enum 
{
    PPPOE_L2TP_AUTH_SWITCH_OFF        =0,
    PPPOE_L2TP_AUTH_SWITCH_ON      =1,   
}PPPOE_L2TP_AUTH_SWITCH_E;


typedef enum 
{
    USER_ISOLATE_ON        =0,
    USER_ISOLATE_OFF      =1,   
}USER_ISOLATE_MODE_E;


/*NAT*/
typedef enum 
{
    NAT_SWITCH_OFF        =0,
    NAT_SWITCH_ON      =1,   
}NAT_SWITCH_E;

typedef enum 
{
    NAT_RULE_TYPE        =0,
    NAT_RULE_SRC_IP      =1,   
    NAT_RULE_SRC_NETMASK      =2,   
    NAT_RULE_DST_STARTIP      =3,   
    NAT_RULE_DST_ENDIP      =4,   
}NAT_RULE_INFO_E;



typedef enum 
{
    ROUTE_STATIC_DST_IP        =0,
    ROUTE_STATIC_DST_NETMASK      =1,   
    ROUTE_STATIC_NEXT_STOP      =2,   
    ROUTE_STATIC_PRI      =3,   
    ROUTE_STATIC_IF_NAME      =4,   
}ROUTE_STATIC_INFO_E;


typedef enum 
{
    MAC_ACL_RULE_PROTO            =0,
    MAC_ACL_RULE_PRIORITY            =1,
    MAC_ACL_RULE_SRC_ADDR      =2,   
    MAC_ACL_RULE_SRC_MASK      =3,   
    MAC_ACL_RULE_DST_ADDR      =4,   
    MAC_ACL_RULE_DST_MASK      =5,   
    MAC_ACL_RULE_INPORT           =6,   
    MAC_ACL_RULE_OUTPORT           =7,   
    MAC_ACL_RULE_TYPE         =8,
}MAC_ACL_RULE_INFO_E;



typedef enum 
{
    IP_ACL_RULE_CHAIN=0,
    IP_ACL_RULE_IPTYPE,
    IP_ACL_RULE_PROTO,
    IP_ACL_RULE_ACTION,
    IP_ACL_RULE_PRIORITY,          
    IP_ACL_RULE_SRC_ADDR,     
    IP_ACL_RULE_SRC_MASK,  
    IP_ACL_RULE_SRC_PORT,
    IP_ACL_RULE_DST_ADDR,      
    IP_ACL_RULE_DST_MASK,  
    IP_ACL_RULE_DST_PORT,
    IP_ACL_RULE_INPORT,           
    IP_ACL_RULE_OUTPORT,           
}IP_ACL_RULE_INFO_E;


/*IP ACL PROTO*/
typedef enum 
{
    IP_ACL_PROTO_ALL        =0,
    IP_ACL_PROTO_UDP      =17,   
    IP_ACL_PROTO_TCP      =6,   
    IP_ACL_PROTO_ICMP      =1,   
}IP_ACL_PROTO_E;

/*IP ACL SWITCH*/
typedef enum 
{
    IP_ACL_SWITCH_OFF        =0,
    IP_ACL_SWITCH_ON      =1,   
}IP_ACL_SWITCH_E;

/*IP ACL ACTION*/
typedef enum 
{
    IP_ACL_ACTION_DENY        =0,
    IP_ACL_SWITCH_ACCEPT      =1,   
}IP_ACL_ACTION_E;

/*IP ACL IPTYPE*/
typedef enum 
{
    IP_ACL_IPTYPE_V4       =0,
    IP_ACL_IPTYPE_V6      =1,   
}IP_ACL_IPTYPE_E;

/*IP ACL CHAIN*/
typedef enum 
{
    IP_ACL_CHAIN_INPUT       =0,
    IP_ACL_CHAIN_FORWARD      =1,   
}IP_ACL_CHAIN_E;

typedef enum{
    BW_LIST_ADDR = 0,
    BW_LIST_ADDR_TYPE,
    BW_LIST_MAX
}BW_LIST_INFO_E;


/* wired wan */
typedef struct 
{
    WAN_ACCESS_MODE_E eWanAccessMode;
    INT32 uiForwardMode;    

    INT32 uiIpMode;
    UINT8 aucStaticIp[4];
    UINT8 aucNetMask[4];
    UINT8 aucDftGw[4];
    UINT8 aucDnsMaster[4];
    UINT8 aucDnsSlave[4];

    CHAR acPppoeName[CHAR_LEN_64];
    CHAR acPppoePasswd[CHAR_LEN_64];
    
}OM_NET_WIRED_WAN_CFG_T;


typedef struct  
{ 
    UINT8 aucLocalIp[4]; 
    UINT8 aucRemoteStartIp[4]; 
    UINT8 aucRemoteEndIp[4]; 
    UINT8 aucPrimaryDnsIp[4]; 
    UINT8 aucSlaveDnsIp[4]; 
    UINT32 uiPppMtu; 
    UINT32 uiPppMru; 
    PPPOE_AUTH_MODE_E enAuthMode ; 
    UINT8 aucPortName[CHAR_LEN_32]; 
    CHAR acAccountType[CHAR_LEN_32];
    CHAR acPppoeName[CHAR_LEN_64];
    CHAR acPppoePasswd[CHAR_LEN_64];
}PPPOE_SERVER_CFG_T; 

/* l2tp隧道选择 */
typedef enum 
{
    L2TP_TUNNEL_SINGLE = 0,
    L2TP_TUNNEL_MULTI
}PPPOE_L2TP_TUNNEL_MODE_E;

typedef struct  
{ 
    PPPOE_L2TP_TUNNEL_MODE_E enTunnelMode;
    UINT8 aucLnsIp[4]; 
	CHAR aucLnsDomain[CHAR_LEN_32];
    CHAR aucHostName[CHAR_LEN_32]; 
    CHAR aucPortName[CHAR_LEN_32]; 
    PPPOE_L2TP_AUTH_SWITCH_E enTaSwitch;
    CHAR aucTaPassWd[CHAR_LEN_32];
}PPPOE_L2TP_CFG_T; 

typedef struct 
{ 
	UINT32 acNoAuthSw;
    CHAR acNoAuthIp[CHAR_LEN_32]; 
    CHAR acNoAuthUser[CHAR_LEN_32]; 
    CHAR acNoAuthPass[CHAR_LEN_32];
    CHAR acNoAuthVpnHost[CHAR_LEN_32];
    CHAR acNoAuthVpnPass[CHAR_LEN_32];
}PPPOE_NO_AUTH_CFG_T; 

typedef struct 
{ 
    PPPOE_L2TP_SWITCH_E enPppoeSvrSw; 
    PPPOE_L2TP_MODE_E enPppoeSvrMode; 
    UINT32 ulPppoeL2tpStaMtu;
    union{ 
        PPPOE_SERVER_CFG_T stPppoeServer; 
        PPPOE_L2TP_CFG_T stPppoeL2tp; 
    }u; 
    PPPOE_NO_AUTH_CFG_T stPppoeNoAuth;
}OM_NET_PPPOE_L2TP_CFG_T;

typedef struct 
{ 
    UINT32  ulID;
    PPPOE_L2TP_CFG_T stPppoeL2tp; 
}OM_NET_L2TP_CFG_T;

#define MAX_PPPOE_L2TP_NUM  32
typedef struct
{
    UINT32 ulCount;
    OM_NET_L2TP_CFG_T astL2tpC[0];
}OM_NET_GET_L2TP_CFG_T;

/* wired lan */

/*USER ISOLATE*/
typedef struct 
{
    USER_ISOLATE_MODE_E enIsolateMode;
}OM_NET_USER_ISOLATE_CFG_T;

//lan ipv6地址配置方式
typedef enum 
{
    LAN_IPV6_CFG_AUTO = 0,
    LAN_IPV6_CFG_MANUAL
}OM_WIRED_LAN_IPV6_CFG_TYPE_E;

//用户 ipv6地址配置方式
typedef enum 
{
    IP_PROTO_IPV4 = 0,
    IP_PROTO_IPV6,
    IP_PROTO_IPV4_IPV6,
}OM_WIRED_IP_PROTO_MODE_E;

typedef enum 
{
    USER_IPV6_CFG_MANUAL = 0,
    USER_IPV6_CFG_STATELESS,
    USER_IPV6_CFG_STATEFUL
}OM_WIRED_LAN_USER_IPV6_CFG_TYPE_E;

typedef struct 
{
    OM_WIRED_LAN_IPV6_CFG_TYPE_E enLanIpv6Type;
    CHAR szLanIpv6[CHAR_LEN_128];       //ipv6 address
    CHAR szLanIpv6Netmask[CHAR_LEN_128]; //ipv6 address prefix
    OM_WIRED_LAN_USER_IPV6_CFG_TYPE_E enUserIpv6Type;
}OM_NET_WRIED_LAN_IPV6_CFG_T;

typedef struct 
{
    OM_NET_USER_ISOLATE_CFG_T stUserIso;       //用户隔离
    UINT8 aucLanIp[4];
    UINT8 aucLanNetMask[4];
    OM_WIRED_IP_PROTO_MODE_E enIpProtoMode;     //协议栈模式
    OM_NET_WRIED_LAN_IPV6_CFG_T stIpv6Cfg;
}OM_NET_WIRED_LAN_CFG_T;



typedef struct 
{
    OM_WIRED_IP_PROTO_MODE_E enIpProtoMode;     //协议栈模式
}OM_WIRED_IP_PROTO_CFG_T;

/* network dhcpserver */
typedef enum 
{
    OM_NET_DHCP_SVR_MODE_OFF=0,
    OM_NET_DHCPV4_SVR_MODE,
    OM_NET_DHCPV6_SVR_MODE,
    OM_NET_DHCPV4V6_SVR_MODE,
    OM_NET_DHCP_RELAY
}OM_NET_DHCP_SVR_MODE_E;

typedef struct 
{
    UINT8 aucStartIp[IP_DOT_LEN];
    UINT8 aucEndIp[IP_DOT_LEN];
    CHAR  acReservedIp[CHAR_LEN_256];
    UINT8 aucNetMask[IP_DOT_LEN];
    UINT8 aucMasterDns[IP_DOT_LEN];
    UINT8 aucSlaveDns[IP_DOT_LEN];
    UINT8 aucGw[IP_DOT_LEN];
    UINT32 ulLease;
}DHCP_IP_POOL_CFG_T;


typedef struct 
{
    CHAR szStartIp[CHAR_LEN_128];
    CHAR szEndIp[CHAR_LEN_128];
    UINT32 uiPrefix;
    CHAR szMasterDns[CHAR_LEN_128];
    CHAR szSlaveDns[CHAR_LEN_128];
    CHAR szGw[CHAR_LEN_128];
    UINT32 uiLease;
}DHCPV6_IP_POOL_CFG_T;

typedef struct 
{
    UINT8 aucMac[6];   
    INT32 iDhcpConfigurable;        /* dhcp server 是否可配置标志位 */
    UINT32 ulDhcpMode;
    UINT8 aucLanIp[4];
    CHAR szLanIpv6Addr[CHAR_LEN_128];
    INT32 iIpv6Prefix;
    CHAR szAllowedMac[CHAR_LEN_256];
    DHCP_IP_POOL_CFG_T stDhcpv4Cfg;
    DHCPV6_IP_POOL_CFG_T stDhcpv6Cfg;
}OM_NET_DHCP_CFG_T;

/* nat rule  */
typedef enum 
{
    OM_NET_NAT_OPERATE_NAT_SW = 1,
    OM_NET_NAT_OPERATE_RULE_ADD,
    OM_NET_NAT_OPERATE_RULE_DEL,
}OM_NET_NAT_OPERATE_FLAG_E;

typedef enum 
{
    OM_NET_NAT_SNAT = 0,
    OM_NET_NAT_PAT,
    OM_NET_NAT_DNAT
}OM_NET_NAT_RULE_TYPE_E;

typedef struct 
{
    UINT32 ulNatRuleIndex;
    UINT32 ulNatRuleType;
    UINT8 aucSrcIp[4];
    UINT8 aucSrcNetMask[4];
    UINT8 aucDstStartIp[4];
    UINT8 aucDstEndIp[4];
}OM_NET_NAT_RULE_CFG_T;


typedef struct 
{
    UINT32 ulNatSwitch;
    UINT32 ulNatDefaultruleSwitch;
}OM_NET_NAT_CFG_T;

/* 从页面获取的配置信息 */
typedef struct 
{
    OM_NET_NAT_OPERATE_FLAG_E eOperateFlag;
    OM_NET_NAT_CFG_T stNatCfg;
    OM_NET_NAT_RULE_CFG_T stNatRule;
}OM_NET_NAT_INFO_T;

/*ROUTE INFO*/
typedef struct 
{
    UINT32 ulRouteIndex;
    UINT8 aucDstIp[4];
    UINT8 aucDstNetMask[4];
    UINT8 aucNextStop[4];
    UINT32 ulRoutePri;
    UINT8 aucIfName[32];
}OM_NET_ROUTE_STATIC_CFG_T;

typedef struct{
    UINT32 uiUpBytes;
    UINT32 uiUpGiga;
    UINT32 uiDownBytes;
    UINT32 uiDownGiga;
}OM_TS_INFO_T;

#ifndef TS_PORT_NAME_LEN
#define TS_PORT_NAME_LEN    8
#endif

/* STA info */
typedef struct{
    CHAR acDevName[TS_PORT_NAME_LEN];
    UINT8 aucMac[6];
    UINT32 uiIp;
    UINT8 aucIpv6[16];
    UINT32 uiType;
    OM_TS_INFO_T stTsIpv4;
    OM_TS_INFO_T stTsIpv6;
    char    acUsername[128];
}OM_NET_TS_LIST_T; 



typedef struct{
    INT32 iOperateType;
    INT32 iIsolationSwitch;
    INT32 iUpdateFlag;
}OM_NET_USER_LIST_WEB_VALUE_T;


/*ARP INFO*/
typedef struct 
{
    UINT8 aucIpAddr[4];
    UINT8 aucMacAddr[6];
}OM_NET_ARP_CFG_T;

/*FlowCtrl INFO*/
typedef enum 
{
    FLOWCTRL_NO       =0,
    FLOWCTRL_CTRL     ,  
    FLOWCTRL_SELF      
}FLOWCTRL_MODE_FLAG;
typedef struct 
{
	FLOWCTRL_MODE_FLAG mode;
    UINT32 speed;
    UINT32 up_byteTick;
	UINT32 up_byteburst;
	UINT32 down_byteTick;
	UINT32 down_byteburst;
}OM_NET_FLOWCTRL_CFG_T;
/*MAC ACL INFO*/
typedef struct 
{
    UINT32 ulMacAclIndex;
    UINT32 ulMacAclRuleNum;
    UINT32 ulProto;
    UINT32 ulPriority;
    UINT32 ulType;
    UINT8 aucSrcMacAddr[6];
    UINT8 aucSrcMacMask[6];
    UINT8 aucDstMacAddr[6];
    UINT8 aucDstMacMask[6];
    UINT8 aucInPort[CHAR_LEN_32];
    UINT8 aucOutPort[CHAR_LEN_32];
}OM_NET_MAC_ACL_CFG_T;


/*MAC ACL POLICY*/
typedef struct 
{
    UINT32 ulSwitch;
    UINT32 ulType;
}OM_NET_MAC_ACL_POLICY_CFG_T;


/*IP ACL POLICY*/
typedef struct 
{
    IP_ACL_SWITCH_E enSwitch;
}OM_NET_IP_ACL_SWITCH_CFG_T;

/*IP ACL INFO*/
typedef struct 
{
    UINT32 ulIpAclIndex;
    UINT32 ulIpAclRuleNum;
    IP_ACL_CHAIN_E enChainId;
    IP_ACL_IPTYPE_E enIpType;/*v4(default) or v6 */
    IP_ACL_PROTO_E enProto;
    IP_ACL_ACTION_E enAction;
    UINT32 ulPriority;
    UINT8 aucSrcIpAddr[4];
    UINT8 aucSrcIpMask[4];
    UINT32 ulSrcPort;
    UINT8 aucDstIpAddr[4];
    UINT8 aucDstIpMask[4];
    UINT32 ulDstPort;
    UINT8 aucInPort[CHAR_LEN_32];
    UINT8 aucOutPort[CHAR_LEN_32];
    
}OM_NET_IP_ACL_CFG_T;

typedef struct{
    UINT32 uiListType;
    CHAR acIpOrUrl[128];
}OM_NET_BW_LIST_T;


/* info collect  */
typedef struct
{
    char    sw[CHAR_LEN_32];
    char    ServerIp[CHAR_LEN_32];
    char    ServerPort[CHAR_LEN_32];
    char    Interval[CHAR_LEN_32];
    char    ScanType[CHAR_LEN_32];
    char    filterSwitch[CHAR_LEN_32];
    int     filterMacLen[5];
    unsigned char filterMac[5][6];
    unsigned char resv[2];
    char    locatorEngine[CHAR_LEN_32];
    char    channelSwitch[CHAR_LEN_32];
    char    channel[64];
}OM_NET_LOCATOR_T;


typedef struct{
    CHAR acDestIp[16];      /* Destination ip */    
    CHAR acGwMac[32];       /* Gateway */ 
}OM_NET_ARP_INFO_T;;


typedef struct {
    UINT32 uiIterm;
    OM_NET_ARP_INFO_T stArpInfo[32];
}OM_NET_ARP_ITERM_T;



typedef struct {
    INT32 iOpType;
    UINT32 uiBwEnabe;
    UINT32 uiBwType;
    INT32 iAddDelIpUrlFlag;
    INT32 iIndex;
    CHAR acIpUrl[128];
}OM_NET_BWLIST_WEB_VALUE_T;

typedef struct {
    CHAR acDestAddr[128];
    INT32 iStatus;
    CHAR acResult[256];
}OM_NET_PING_TEST_T;

/*	VPN setting	*/
#define VPN_WEB_SUBMIT_CONFIG				"config"
#define VPN_WEB_SUBMIT_CONNECT				"connect"
#define VPN_WEB_SUBMIT_DISCONNECT			"disconnect"
#define VPN_CURR_CONNECT_WAY_L2TP			"L2TP"
#define VPN_CURR_CONNECT_WAY_PPTP			"PPTP"
#define VPN_CURR_CONNECT_STATE_CONNECT		"connect"
#define VPN_CURR_CONNECT_STATE_DISCONNECT	"disconnect"
typedef struct {
	char	lnsIp[CHAR_LEN_32];   	// server ip address
	char	lnsHost[CHAR_LEN_32];	// server username 
	char	lnsPass[CHAR_LEN_32];	// server password 
	int		lnsCurrWay;				//curr vpn link way		1:l2tp 2:pptp
	int		lnsAction;				//submit action 		1:set  2:link  3:break
	int		lnsCurrState;			//curr vpn link state	1:link 2:break
	int		lnsL2tpSw;
	char	lnsL2tpHost[CHAR_LEN_32];			
	char	lnsL2tpPass[CHAR_LEN_32];			
}OM_VPN_LNS_CFG_T;

typedef enum {
    VPN_LNS_SUBMIT,
    VPN_LNS_SET,
    VPN_LNS_LINK,
    VPN_LNS_BREAK
}OM_VPN_LNS_SUBMIT;

typedef enum {
    VPN_LNS_SET_WAY,
    VPN_LNS_SET_L2TP,
    VPN_LNS_SET_PPTP
}OM_VPN_LNS_WAY;


#define MANGLE_RULES_NUM_MAX    32
#define IPTABLES_MANGLE_QOS_CHAIN  "qos_chain"

#define ADD_QOS_MANGLE      1
#define DEL_QOS_MANGLE      2

#define QOS_DISABLE         0
#define QOS_ENABLE          1


typedef struct{
    UINT32 uiQosEnable;

    UINT32 uiIndex;
    UINT32 uiSrcIp;
    UINT32 uiSrcMask;
    UINT32 uiDstIp;
    UINT32 uiDstMask;
    UINT8 aucSrcMac[6];
    UINT8 aucDstMac[6];
    UINT32 uiSrcPort;
    UINT32 uiDstPort;
    CHAR acProto[CHAR_LEN_32];
    
    UINT32 uiPrio;
    UINT32 uiAction;
}OM_NET_MANGLE_T;

typedef struct{
    UINT32 uiQosEnable;
    UINT32 uiNum;
    OM_NET_MANGLE_T pstMangle[0];
}OM_NET_MANGLE_LIST_T;


#define QOS_ADD         1
#define QOS_DEL         2
#define QOS_MODIFY      3

#define TC_DEFAULT_ROOT_ID              10

#define TC_DEFAULT_CLASSID_PARENT       11

#define TC_DEFAULT_CLASSID_CHILD        21

#define TC_CLASS_DEFAULT_NUM            4

typedef struct{
    CHAR acDevName[8];
    UINT32 uiAction;
    
    UINT32 uiParentId1;
    UINT32 uiParentId2;
    UINT32 uiClassId1;
    UINT32 uiClassId2;
    /* for class */
    UINT32 uiRateLimit; /* kb */
    UINT32 uiRateCeil;  /* kb */
    /* for filter */
    UINT32 uiPrio;
    UINT32 uiHandle;
    /* for sfq */
    UINT32 uiPerturb;
}OM_NET_QOS_T;

/**************************************** 函数声明 *******************************/
extern  OM_ERROR_CODE_E  om_net_wired_lan_config_get( OM_NET_WIRED_LAN_CFG_T *pstWiredLanInfo);
extern  OM_ERROR_CODE_E  om_net_wired_wan_config_get( OM_NET_WIRED_WAN_CFG_T *pstWanWiredInfo );
extern  OM_ERROR_CODE_E  om_net_wired_wan_config_nvram( OM_NET_WIRED_WAN_CFG_T *pstWiredWanCfg);
extern  OM_ERROR_CODE_E  om_net_wired_wan_forward_bridge( OM_NET_WIRED_WAN_CFG_T *pstWiredWanCfg ,CHAR *pcDevName);
extern  OM_ERROR_CODE_E  om_net_wired_wan_ipmode_static( OM_NET_WIRED_WAN_CFG_T *pstWiredWanCfg ,CHAR *pcDevName );
extern  OM_ERROR_CODE_E  om_net_wired_wan_forward_route( OM_NET_WIRED_WAN_CFG_T *pstWiredWanCfg ,CHAR *pcDevName);
extern  OM_ERROR_CODE_E  om_net_wired_wan_config_module( OM_NET_WIRED_WAN_CFG_T *pstWiredWanCfg );
extern  OM_ERROR_CODE_E  om_net_wired_wan_set( void *pvData);

extern  OM_ERROR_CODE_E  om_net_wired_lan_config_nvram( OM_NET_WIRED_LAN_CFG_T *pstWiredWanCfg);
extern  OM_ERROR_CODE_E  om_net_wired_lan_ip( OM_NET_WIRED_LAN_CFG_T *pstWiredLanCfg ,CHAR *pcDevName);
extern  OM_ERROR_CODE_E  om_net_wired_lan_config_module( OM_NET_WIRED_LAN_CFG_T *pstWiredLanCfg);
extern  OM_ERROR_CODE_E  om_net_wired_lan_set( void *pvData);

extern OM_ERROR_CODE_E  om_net_wired_proto_ipv4_mode(OM_NET_WIRED_LAN_CFG_T *pstWiredLanInfo);
extern OM_ERROR_CODE_E  om_net_wired_proto_ipv6_mode(OM_NET_WIRED_LAN_CFG_T *pstWiredLanInfo);
extern OM_ERROR_CODE_E  om_net_wired_proto_ipv4_ipv6_mode(OM_NET_WIRED_LAN_CFG_T *pstWiredLanInfo);
extern OM_ERROR_CODE_E  om_net_wired_proto_init( void );
extern OM_ERROR_CODE_E  om_net_wired_ip_proto_config_get( OM_WIRED_IP_PROTO_CFG_T *pstIpProtoMode);
extern OM_ERROR_CODE_E  om_net_wired_ip_proto_config_nvram(OM_WIRED_IP_PROTO_CFG_T *pstIpProtoMode);
extern OM_ERROR_CODE_E  om_net_wired_ip_proto_config_module(OM_WIRED_IP_PROTO_CFG_T *pstIpProtoMode);
extern OM_ERROR_CODE_E  om_net_wired_ip_proto_config_set(OM_WIRED_IP_PROTO_CFG_T *pstIpProtoMode);

extern  OM_ERROR_CODE_E  om_net_wired_wan_get_ifname(CHAR *pucWanName, UINT32 ulNameLen);
extern  OM_ERROR_CODE_E  om_net_wired_lan_get_ifname(CHAR *pucLanName, UINT32 ulNameLen);
extern  OM_ERROR_CODE_E  get_wired_mac_lte( unsigned char *pucOutMac );
extern  OM_ERROR_CODE_E set_wired_mac_lte(void);
extern  OM_ERROR_CODE_E  om_net_hwaddr_set(CHAR *pcDevName, UINT8 *pucHwAddr, UINT32 ulHwLen);
extern  OM_ERROR_CODE_E  om_net_dhcp_server_start(CHAR *pucPortName);
extern  OM_ERROR_CODE_E  om_net_dhcp_server_stop(void);
extern  OM_ERROR_CODE_E  om_net_dhcp_client_start(CHAR *pucPortName);
extern OM_ERROR_CODE_E  om_net_pppoe_dhcp_client_start(CHAR *pucWanName);
extern  OM_ERROR_CODE_E  om_net_pppoe_client_start(CHAR *pucPortName);
extern  OM_ERROR_CODE_E  om_net_pppoe_server_start(void);
extern  OM_ERROR_CODE_E  om_net_pppoe_server_stop(void);
extern  OM_ERROR_CODE_E  om_net_pppoe_server_setup(int iFlag,OM_NET_PPPOE_L2TP_CFG_T *pstPppoel2tpCfg);
extern OM_ERROR_CODE_E  om_net_l2tp_add(OM_NET_L2TP_CFG_T *pstl2tpCfg);
extern OM_ERROR_CODE_E  om_net_l2tp_del(OM_NET_L2TP_CFG_T *pstl2tpCfg);
extern OM_ERROR_CODE_E  om_net_l2tp_get(OM_NET_L2TP_CFG_T *pstl2tpCfg);
extern  OM_ERROR_CODE_E  om_net_wired_wan_connect(CHAR *pucWanName);
extern  OM_ERROR_CODE_E  om_net_lan_br_create(void);
extern  OM_ERROR_CODE_E  om_net_lan_br_delete(void);
extern  OM_ERROR_CODE_E  om_net_lan_br_add_if(CHAR *pucLanName);
extern  OM_ERROR_CODE_E  om_net_lan_br_del_if(CHAR *pucLanName);
extern  OM_ERROR_CODE_E  om_net_wan_name_get(CHAR *pucWanName, UINT32 ulNameLen);
extern  OM_ERROR_CODE_E  om_net_wan_ip_get( void *pvData);
extern  OM_ERROR_CODE_E  om_net_nat_rule_default(void);
extern OM_ERROR_CODE_E  om_net_nat_rule_add(OM_NET_NAT_RULE_CFG_T *pstNatRuleCfg);
extern OM_ERROR_CODE_E  om_net_nat_rule_del(OM_NET_NAT_RULE_CFG_T *pstNatRuleCfg);
extern OM_ERROR_CODE_E  om_net_default_nat_rebuild(void);
extern OM_ERROR_CODE_E  om_net_default_nat_rebuild_asyn(void);
extern OM_ERROR_CODE_E om_net_nat_cfg_get( OM_NET_NAT_CFG_T *pstNatCfg);
extern OM_ERROR_CODE_E  om_net_nat_rule_list_get(CHAR *pcNatRuleList,UINT32 *puiNum);
extern OM_ERROR_CODE_E  om_net_default_gw_set(CHAR *pucPortName,CHAR *aucIP);
extern OM_ERROR_CODE_E  om_net_dns_setup(OM_NET_WIRED_WAN_CFG_T *pstWiredWanCfg);
extern OM_ERROR_CODE_E  om_net_dhcp_server_setup(OM_NET_DHCP_CFG_T *pstDhcpCfg);
extern OM_ERROR_CODE_E  om_net_user_list_get(OM_NET_TS_LIST_T * pstUserList, int * iNum, int iMax);
extern OM_ERROR_CODE_E  om_net_set_user_ts_clear(void);
extern OM_ERROR_CODE_E om_net_get_dev_ts(OM_NET_TS_LIST_T * pstDevList);
extern OM_ERROR_CODE_E  om_net_dev_list_get(OM_NET_TS_LIST_T * pstDevList, int * iNum, int iMax);
extern OM_ERROR_CODE_E  om_net_get_bwlist(OM_NET_BW_LIST_T * pstBwList, int * iNum, int * piEnable, int * piType);
extern OM_ERROR_CODE_E om_net_add_bwlist_ip_url(OM_NET_BWLIST_WEB_VALUE_T * pstWebValue, int iFlag);
extern OM_ERROR_CODE_E om_net_del_bwlist_ip_url(OM_NET_BWLIST_WEB_VALUE_T * pstWebValue);
extern OM_ERROR_CODE_E om_net_bwlist_init(void);
extern OM_ERROR_CODE_E  om_net_whitelist_ip_add( CHAR *szIpAddr);
extern OM_ERROR_CODE_E  om_net_whitelist_ip_del( CHAR *szIpAddr);
extern OM_ERROR_CODE_E  om_net_whitelist_checkalive_set( void );



extern OM_ERROR_CODE_E  om_net_info_collect_config_get( OM_NET_LOCATOR_T *pstLocatorCfg);
extern OM_ERROR_CODE_E  om_net_info_collect_config_nvram( OM_NET_LOCATOR_T *pstLocatorCfg,OM_NET_LOCATOR_T *pstLocatorCfgOld);
extern OM_ERROR_CODE_E  om_net_info_collect_config_module(OM_NET_LOCATOR_T *pstLocatorCfg,OM_NET_LOCATOR_T *pstLocatorCfgOld);
extern OM_ERROR_CODE_E  om_net_info_collect_config_set( OM_NET_LOCATOR_T *pstLocatorCfg);   

extern OM_ERROR_CODE_E om_net_restart_pppoe_server(CHAR *pcName, CHAR *pcPasswd);
extern CHAR * om_net_get_pppoe_names_list(CHAR *pcList);
extern OM_ERROR_CODE_E om_net_add_pppoe_account(PPPOE_SERVER_CFG_T *pstPppoeCfg);
extern OM_ERROR_CODE_E om_net_del_pppoe_account(PPPOE_SERVER_CFG_T *pstPppoeCfg);
extern OM_ERROR_CODE_E om_net_modify_pppoe_server(CHAR *pcFileName, CHAR *pcName, CHAR *pcPasswd);
extern OM_ERROR_CODE_E om_net_init_pppoe_account(void);
extern OM_ERROR_CODE_E om_net_get_ts_switch(int * piTsSwitch);
extern OM_ERROR_CODE_E om_net_set_ts_switch(int iTsSwitch);

/* qos */
extern OM_ERROR_CODE_E om_net_tc_class_setup(OM_NET_QOS_T *pstQos);
extern OM_ERROR_CODE_E om_net_tc_filter_setup(OM_NET_QOS_T *pstQos);
extern OM_ERROR_CODE_E om_net_tc_init(CHAR *pcevName);
extern OM_ERROR_CODE_E om_net_tc_qdisc_add(OM_NET_QOS_T *pstQos);
extern OM_ERROR_CODE_E om_net_tc_qdisc_sfq(OM_NET_QOS_T *pstQos);
extern OM_ERROR_CODE_E om_net_mangle_add(OM_NET_MANGLE_T *pstMangle);
extern OM_ERROR_CODE_E om_net_mangle_build_iptables(OM_NET_MANGLE_T *pstMangle);
extern OM_ERROR_CODE_E om_net_mangle_del(OM_NET_MANGLE_T *pstMangle);
extern OM_ERROR_CODE_E om_net_mangle_disable(void);
extern OM_ERROR_CODE_E om_net_mangle_enable(void);
extern OM_ERROR_CODE_E om_net_mangle_init(void);
extern OM_ERROR_CODE_E om_net_qos_config(OM_NET_MANGLE_T *pstMangle);
extern OM_ERROR_CODE_E om_net_get_qos_list(OM_NET_MANGLE_LIST_T *pstMangleList, UINT32 uiNum);
extern OM_ERROR_CODE_E om_net_get_qos_nvram(OM_NET_MANGLE_T *pstMangle);
extern OM_ERROR_CODE_E om_net_qos_init();
/*l2tp*/
extern OM_ERROR_CODE_E  om_net_tr69_l2tp_set(int flag,OM_NET_L2TP_CFG_T *stl2tpCfg);
extern OM_ERROR_CODE_E  om_net_tr69_l2tp_get(OM_NET_L2TP_CFG_T *pstl2tpCfg);
extern OM_ERROR_CODE_E  om_net_l2tp_info_get(int flag,int *tmp);
extern OM_ERROR_CODE_E  om_network_pppoeserver_config_get( OM_NET_PPPOE_L2TP_CFG_T *pstNetworkPppoeCfg, OM_NET_GET_L2TP_CFG_T *pstPppoeL2tp);
/*flowctrl*/
extern OM_ERROR_CODE_E om_net_set_flowctrl(OM_NET_FLOWCTRL_CFG_T *pstflowctrlCfg);
extern OM_ERROR_CODE_E om_net_set_flowctrl_nvram(OM_NET_FLOWCTRL_CFG_T *pstflowctrlCfg);
extern OM_ERROR_CODE_E om_net_get_flowctrl(OM_NET_FLOWCTRL_CFG_T *pstflowctrlCfg);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __OM_NET_H__ */
