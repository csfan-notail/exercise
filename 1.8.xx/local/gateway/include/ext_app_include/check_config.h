/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : check_config.h
  Version       : Initial Draft
  Author        : 
  Created       : 
  Last Modified :
  Description   : ��������ļ��Ϸ��ԣ�����ȡֵ�Ϸ��Ժ�����������
                    ���÷��ദ��ͬ��ʼ������
  Function List :
  History       :
  1.Date        : 
    Author      : 
    Modification: Created file

******************************************************************************/
#ifndef __CHECK_CONFIG_H__
#define __CHECK_CONFIG_H__

#if 0
/* WIDS �¶���ṹ��, ����ԭ���Ľṹ��*/
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
   ccs����ģ�������
*/
typedef struct
{
    wifidog         stWifidog;      /* ������Ϣ--���ȵ�����*/
    locator_param   stLocator;      /* ������Ϣ--����λ��������*/
    lan_cf          stLan;          /* ��������--��LAN����*/
    dhcp_server     stDhcpServer;   /* ��������--��DHCP����������*/
    unsigned long   ulWifiNum;                          /* Wifi Num   */
    wifi_config     astWifi[WIFI_NUM];                  /* Wi-Fi */
    //ccs_cfg_wids_t  stWids;                             /* Wi-Fi  WIDS */
    unsigned long   ulVapNum;                           /* SSID Num */
    char            acStrVapIdList[NVRAM_VALUE_MAX_LEN];    /* SSID����б�, �ַ�����ʽ*/
    char            acVapIdList[NVRAM_VALUE_MAX_LEN];   /* SSID����б�, ʹ�ö��ŷָ�*/            
    vap_config      astVap[VAP_MAX];                    /* SSID */
    CCS_MODULE_CONFIG_CHECK_E eCheckVapStatus;
    ccs_cfg_maclist_t   astVapMacList[VAP_MAX];         /* SSID ������������MAC�б�*/
    vlan_map        astVlanMaps[VAP_MAX+LAN_NUM];       /* VLAN--���˿�VLAN���� */
    sntp            stSntp;                             /* ϵͳ����--��ʱ������*/
}ccs_cfg_t;

#endif

