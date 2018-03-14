/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : app_public.h
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/5/2
  Last Modified :
  Description   : app_public.h header file
                : 
  Function List :
  History       :
  1.Date        : 2013/5/2
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/

#ifndef __APP_PUBLIC_H__
#define __APP_PUBLIC_H__

/*------------ format ------------*/
#define __file__ __FILE__
#define __func__ __FUNCTION__
#define __line__ __LINE__


/* 接口返回值 */
#define OK      0
#define ERROR   1

/* 初始化和配置定义 */
#define DO_FLAG_INIT    0
#define DO_FLAG_OMC     1

/* 模块的操作 */
#define FLAG_VAP_ADD        1
#define FLAG_VAP_DELETE     2
#define FLAG_VAP_ENTER      3
#define FLAG_VAP_MODIFY      4

#define FLAG_VLAN_ADD       0
#define FLAG_VLAN_DELETE    1
#define FLAG_VLAN_MODI      2

#define FLAG_FLOW_ADD       1
#define FLAG_FLOW_DELETE    0
#define FLAG_FLOW_AUTO    2


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
#define ETH2    "eth2"
#define ETH3    "eth3"
#endif

#define PPP      "ppp"
#define PPP0     "ppp0"

/* BEGIN: Added by WeiHonggang, 2014/3/6   PN:TASK#0 LTE-CPE上联口名称定义 */
#if defined(LTE_OAM)
#define USB		"usb0"
#endif

/* BEGIN: Added by wanggang, 2014/10/27   PN:gwifi */
#ifndef USB
#define USB "usb0"
#endif 
/* END:   Added by wanggang, 2014/10/27   PN:gwifi */
/* END:   Added by WeiHonggang, 2014/3/6   PN:TASK#0 LTE-CPE上联口名称定义 */

/* 国家码定义 */
#define COUNTRY_CODE_CHINA      156
#define COUNTRY_CODE_TW         158
#define COUNTRY_CODE_EUROPE     276

/** 不能被删除的nvram 字段(恢复出厂设置) */
#define NOT_TO_DELETE       "mtdblock4:mtdblock5:boot_success:bootnum:targetver:wifi_numbers"\
                            ":device_type:device_sn:hard_version:soft_version:country_code:customer_sn"\
                            ":config_flash_8_devcie_type:config_flash_8_oem:config_flash_8_sn:config_flash_8_prosn"\
                            ":BOOT_HWP:BOOT_PCB:BOOT_PCBVER:BOOT_BOMCODE:BOOT_VERSION:platform"\

/** 不能被删除的nvram 字段(恢复备份时不应当更换用户名与oem等信息,
而只是网络配置,如果有需要应当修改oem更合理) */
#define NOT_TO_DELETE_FOR_RECOVER      "mtdblock4:mtdblock5:boot_success:bootnum:targetver:wifi_numbers"\
                            ":device_type:device_sn:hard_version:soft_version:country_code:customer_sn"\
                            ":config_flash_8_devcie_type:config_flash_8_oem:config_flash_8_sn:config_flash_8_prosn"\
                            ":BOOT_HWP:BOOT_PCB:BOOT_PCBVER:BOOT_BOMCODE:BOOT_VERSION:platform"\
                            ":restore_main_log:last_login_user:last_login_addr:device_name"\


#define NOT_TO_DELETE_FOR_COPY       "mtdblock4:mtdblock5:boot_success:bootnum:targetver:wifi_numbers"\
                            ":device_type:device_sn:hard_version:soft_version:country_code:customer_sn"\
                            ":config_flash_8_devcie_type:config_flash_8_oem:config_flash_8_sn:config_flash_8_prosn"\
                            ":BOOT_HWP:BOOT_PCB:BOOT_PCBVER:BOOT_BOMCODE:BOOT_VERSION:platform"\
                            ":device_hotspot_name:last_login_user:last_login_addr"\
                            ":net_forward_mode:ipv4_ip_method:ipv4_ip_gw:ipv4_0_ip_dns:ipv4_0_ip_address"\
                            ":ipv4_0_ip_mask:ipv4_1_ip_dns:ipv4_pppoe_name:ipv4_pppoe_pwd:ipv4_lte_access"\
                            
                          
/* wifi 设备类型定义 */
#define WIFI_NUM        2
#define WIFI_24GHZ      1
#define WIFI_5GHZ       2

#define PONIT_LENGTH    32

#define AP_NAME_FMT     "ap"
#define WIFI_NAME_FMT   "wifi"

/* 产品形态定义 */
typedef struct _wifi_info
{
    int freq;           /*2.4:1, 5:2*/
    int limit_txpower;  /* 最大功率 */
    int rfnum;          /* 空间流数目 */
    char wifi_name[32]; /* wifi名称 */
    int wifi_id;
    int flag;           /* 0: 无效, 1: 有效*/
}wifi_info;

typedef struct _oem_info
{
    char software[32];
    char hardware[32];
    char company[32];
    char type[32];
}oem_info;

typedef struct _product_info
{
    int device_type_id;
    wifi_info st_wifi_info[WIFI_NUM];
    oem_info st_oem;
}product_info;


typedef struct _sys_wifi_info
{
    char ssid[32];
    char bssid[32];
    char channel[32];
    char safe[32];
    char state[32];
    char tag[32];
    char vlan[32];
	char auto_switch[32];
}sys_wifi_info;

typedef struct _sys_wifi_stat
{
    char ssid[32];
    unsigned long  txpackt;
    unsigned long  rxpackt;
    unsigned long  txbytes;
    unsigned long  rxbytes;
    unsigned long  txerrs;
    unsigned long  rxerrs;
}sys_wifi_stat;


#endif /* __APP_PUBLIC_H__ */


