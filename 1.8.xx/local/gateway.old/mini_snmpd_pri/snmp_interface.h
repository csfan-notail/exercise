/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : interface.h
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2012/11/7
  Last Modified :
  Description   : 接口函数定义和声明
  Function List :
  History       :
  1.Date        : 2012/11/7
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#ifndef __SNMP_INTERFACE_H__
#define __SNMP_INTERFACE_H__

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
#ifndef MAC_FMT
#define MAC_FMT    "%02x:%02x:%02x:%02x:%02x"
#endif
#ifndef MAC_ARG
#define MAC_ARG(a) (char *)(a)[0], (char *)(a)[1], (char *)(a)[2], (char *)(a)[3], (char *)(a)[4], (char *)(a)[5]
#endif

#ifndef IP_FMT
#define IP_FMT     "%u.%u.%u.%u"
#endif
#ifndef IP_ARG
#define IP_ARG(a)  (char *)(a)[0], (char *)(a)[1], (char *)(a)[2], (char *)(a)[3]
#endif

/*  有线网口定义 */
/* 2014/4/22 added by zhangping */
#if defined(AP9341_OAM)
#define ETH0    "wan0"
#define ETH1    "lan0"
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
#endif

/* 软件版本路径 */
#define VERSION_PATH "/etc/version.conf"

/* 查询CPU */
#define SNMP_CPU     "/tmp/snmp_cpu.tmp"

/* 查询内存 */
#define SNMP_MEM     "/tmp/snmp_memory.tmp"
#define MEM_TOTAL    0
#define MEM_USED     1
#define MEM_FREED    2
#define MEM_MAX      3 

/* vap数目 */
#define VAP_SZIE     32

/* wifi卡数目 */
#define RADIOID_LIST 2

/* wifi名字 */
#define WIFINAME "wifi"

/* vap名字 */
#define VAPNAME  "vap"

/* BEGIN: Added by zoucaihong, 2012/12/6 */
#define R_BYTE          0
#define R_PACKET        1
#define R_ERR           2
#define R_DROP          3
#define R_FIFO          4
#define R_FRAME         5
#define R_COMPRESSED    6
#define R_MULTICAST     7
#define T_BYTE          8
#define T_PACKET        9
#define T_ERR           10
#define T_DROP          11
#define T_FIFO          12
#define T_FRAME         13
#define T_COMPRESSED    14
#define T_MULTICAST     15
#define DEV_STAT_MAX    16
/* END: Added by zoucaihong, 2012/12/6 */
/*----------------------------------------------*
 * routines' implementations                    *
 *----------------------------------------------*/

/****************** 上层接口函数声明 ******************/
extern UINT32 app_get_uptime(void);
extern UINT32 app_get_devmac(char * pcMac, char * devName);
extern BOOL   app_get_sys_devinfo(AP_CONFIG_SYS_DEVINFO_T * pstDevInfo);
extern UINT32 app_get_swinfo(AP_CONFIG_SYS_SWINFO_T * pstSwInfo);
extern BOOL app_get_hdinfo(AP_CONFIG_SYS_HWINFO_T *pstHdInfo);
extern BOOL   mem_str2info(char * pcBuff, unsigned int * pucBuff, int iMax);
extern BOOL   app_get_sysinfo(AP_STAT_SYS_T * pstStatSys);
/* BEGIN: Added by zoucaihong, 2012/11/15 */
extern BOOL   app_get_trapdesinfo(AP_CONFIG_TRAPDES_ENTRY_T * pstTrapDesInfo);
/* END: Added by zoucaihong, 2012/11/15 */
/* BEGIN: Added by zoucaihong, 2012/11/21 */
extern BOOL   app_get_vlaninfo(UINT32 *total, AP_CONFIG_VLAN_ENTRY_T * pstVlanInfo);
/* END: Added by zoucaihong, 2012/11/21 */
extern BOOL app_get_alarmconfig( AP_CONFIG_ALARM_T *pstAlarmConfig );
extern BOOL get_trapdes_number( TRAPDES_T *pstTrapDes );
//extern BOOL app_get_system( SYSTEM_T *pstSystem);
/* BEGIN: Added by zoucaihong, 2013/3/25 */
//extern BOOL app_get_advance_5gprior(AP_CONFIG_ADVANCE_5GPRIOR_T *pstAdvance5gPrior);
//extern BOOL app_get_advance_11nprior(AP_CONFIG_ADVANCE_11NPRIOR_T *pstAdvance11nPrior);
/* END: Added by zoucaihong, 2013/3/25 */
//extern BOOL app_set_advanced_5gprior(AP_CONFIG_ADVANCE_5GPRIOR_T *pstAdvance5gPrior);
//extern BOOL app_set_advanced_11nprior(AP_CONFIG_ADVANCE_11NPRIOR_T * pstAdvance11nPrior);
/* BEGIN: Added by zoucaihong, 2013/5/13 */
//extern BOOL app_get_ap_legalap( AP_CONFIG_LEGALAP_ENTRY_T *pstSysLegalApConfig, UINT32 *pulRecNum);
//extern BOOL app_get_ap_flowctl(AP_CONFIG_FLOWCTL_T *pstSysFlowCtl);
/* END: Added by zoucaihong, 2013/5/13 */
extern int app_vlan_eabled(int br_id, int state, char *vap_name);
extern int app_set_vlaninfo(AP_CONFIG_VLAN_ENTRY_T *pstVlanInfo, int flag);
extern int app_vlan_modify(int br_id, int br_id_old, char *vap_name);

/****************** end 上层接口函数声明 ******************/


/****************** 无线驱动接口函数声明 ******************/
extern BOOL wlan_get_vapinfo( AP_CONFIG_VAPBASIC_ENTRY_T *pstVapInfo );
/* BEGIN: Added by zoucaihong, 2012/11/15 */
extern BOOL wlan_get_vapsecurityinfo(AP_CONFIG_VAPSECURITY_ENTRY_T * pstVapSecurityInfo);
/* END: Added by zoucaihong, 2012/11/15 */
//extern BOOL wlan_get_vapstatinfo( BSS_AIR_STATS_T *pstBssAirStats );
extern BOOL wlan_get_wifichannelinfo( AP_CONFIG_WIFIDEVICECHANNEL_T *pstWifiChanInfo );
extern BOOL wlan_get_wifimac( char *pcDevName, char *pcDevMac );
extern BOOL wlan_get_wifitxpowerinfo( AP_CONFIG_WIFIDEVICEPOWER_T *pstWifiTxPower );
extern BOOL wlan_get_wifiadvanceinfo( AP_CONFIG_WIFIDEVICEADVANCE_T *pstApWifiAdvance );
extern BOOL get_wifi_vap_number( SSID_T *pstSsid );
extern BOOL wlan_get_wifibasicinfo(AP_CONFIG_WIFIDEVICEBASIC_T *pstWifiBasicInfo);
/* BEGIN: Added by zoucaihong, 2013/1/11 */
//extern int ieee80211_state_get(char *dev, T_wifiState_i *ptState);
extern BOOL wlan_get_userinfo(AP_STAT_STATION_ENTRY_T *pstUserInfo, UINT32 *pulUserNum);
/* END: Added by zoucaihong, 2013/1/11 */
extern int wifi_device_channel_config(AP_CONFIG_WIFIDEVICECHANNEL_T *pstData, AP_CONFIG_WIFIDEVICECHANNEL_T *pstDataOld);
extern BOOL wlan_get_wifirateinfo( AP_CONFIG_WIFIDEVICERATE_T *pstApWifiRate );

#if 0
/* BEGIN: Added by zoucaihong, 2013/1/21 */
extern BOOL wlan_get_stadevtype( UINT8 *pucStaMac, UINT8 *pucStaDevType );
extern BOOL wlan_get_brmodestainfo( AP_STAT_BRMODESTA_ENTRY_T *pstApBrModeStaInfo, UINT32 *pulStaNum );
extern BOOL wlan_get_ap24cochainterfinfo( AP_STAT_24COCHAINTERFINFO_ENTRY_T *pstAp24CoChaInterfInfo, UINT32 *pulNum );
extern BOOL wlan_get_ap5cochainterfinfo( AP_STAT_5COCHAINTERFINFO_ENTRY_T *pstAp5CoChaInterfInfo, UINT32 *pulNum );
extern BOOL wlan_get_ap24adjchainterfinfo( AP_STAT_24ADJCHAINTERFINFO_ENTRY_T *pstAp24AdjChaInterfInfo, UINT32 *pulNum );
extern BOOL wlan_get_ap5adjchainterfinfo( AP_STAT_5ADJCHAINTERFINFO_ENTRY_T *pstAp5AdjChaInterfInfo, UINT32 *pulNum );
extern BOOL wlan_get_ap24wlandevinterfinfo( AP_STAT_24WLANDEVINTERFINFO_ENTRY_T *pstAp24WlanDevInterfInfo, UINT32 *pulNum );
extern BOOL wlan_get_ap5wlandevinterfinfo( AP_STAT_5WLANDEVINTERFINFO_ENTRY_T *pstAp5WlanDevInterfInfo, UINT32 *pulNum );
extern BOOL wlan_get_apnonwlandevinterfinfo( AP_STAT_NONWLANDEVINTERFINFO_ENTRY_T *pstApNonWlanDevInterfInfo, UINT32 *pulNum );
/* END: Added by zoucaihong, 2013/1/21 */
/* BEGIN: Added by zoucaihong, 2013/5/10 */
extern BOOL wlan_get_ap24rogueapinfo( AP_STAT_24ROGUEAPINFO_ENTRY_T *pstAp24RogueApInfo, UINT32 *pulNum );
extern BOOL wlan_get_ap5rogueapinfo( AP_STAT_5ROGUEAPINFO_ENTRY_T *pstAp5RogueApInfo, UINT32 *pulNum );
/* END: Added by zoucaihong, 2013/5/10 */
#endif
/****************** end 无线驱动接口函数声明 ******************/


/****************** 桥和有线驱动接口函数声明 ******************/
//extern BOOL route_get_interfaces( INTERFACES_ENTRY_T *pstInterfaces );
/****************** end 桥和有线驱动接口函数声明 ******************/

#endif

