/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : check_config.h
  Version       : Initial Draft
  Author        : 
  Created       : 
  Last Modified :
  Description   : 检查配置文件合法性，包括取值合法性和配置完整性
                    配置分类处理同初始化流程
  Function List :
  History       :
  1.Date        : 
    Author      : 
    Modification: Created file

******************************************************************************/
#ifndef __CHECK_CONFIG_H__
#define __CHECK_CONFIG_H__

#if 0
/* WIDS 新定义结构体, 不动原来的结构体*/
typedef struct
{
    char aacSsid[WIFI_WIDS_SSID_NUM_MAX][32];
    char aacBssid[WIFI_WIDS_BSSID_NUM_MAX][32];
    char aacOui[WIFI_WIDS_OUI_NUM_MAX][32];
    char aacActionBssid[WIFI_WIDS_ACTION_BSSID_NUM_MAX][32];
}ccs_cfg_wids_t;
#endif

typedef struct
{
    char aacMac[VAP_ADVANCED_MAC_NUM_MAX][32];
} ccs_cfg_maclist_t;


/*
   ccs配置模板的数据
*/
typedef struct
{
    wifidog         stWifidog;      /* 基本信息--》热点配置*/
    locator_param   stLocator;      /* 基本信息--》定位服务配置*/
    lan_cf          stLan;          /* 网络配置--》LAN管理*/
    dhcp_server     stDhcpServer;   /* 网络配置--》DHCP服务器配置*/
    unsigned long   ulWifiNum;                          /* Wifi Num   */
    wifi_config     astWifi[WIFI_NUM];                  /* Wi-Fi */
    //ccs_cfg_wids_t  stWids;                             /* Wi-Fi  WIDS */
    unsigned long   ulVapNum;                           /* SSID Num */
    char            acStrVapIdList[NVRAM_VALUE_MAX_LEN];    /* SSID编号列表, 字符串格式*/
    char            acVapIdList[NVRAM_VALUE_MAX_LEN];   /* SSID编号列表, 使用逗号分隔*/            
    vap_config      astVap[VAP_MAX];                    /* SSID */
    CCS_MODULE_CONFIG_CHECK_E eCheckVapStatus;
    ccs_cfg_maclist_t   astVapMacList[VAP_MAX];         /* SSID 允许上网主机MAC列表*/
    vlan_map        astVlanMaps[VAP_MAX+LAN_NUM];       /* VLAN--》端口VLAN配置 */
    sntp            stSntp;                             /* 系统操作--》时间配置*/
}ccs_cfg_t;

#endif

