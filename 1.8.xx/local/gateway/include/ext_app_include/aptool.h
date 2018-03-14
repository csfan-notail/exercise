/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : aptool.h
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/7/7
  Last Modified :
  Description   : aptool.c header file
  Function List :
  History       :
  1.Date        : 2013/7/7
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#ifndef __APTOOL_H__
#define __APTOOL_H__

/* 硬件设备类型定义 */
typedef enum _en_hardware_type
{
    AP_IPX_TYPE    = 0,
    AP_AP93_TYPE   = 1,
    AP_PB44_TYPE   = 2,
    AP_TA98_TYPE   = 3,
    AP_TA40_TYPE   = 4,
    AP_TA55_TYPE   = 5,
}en_hardware_type;

#define APTOOL_SUCCESS      0
#define APTOOL_FAILED       1

#define DEVICE_SIGN_FILE        "/tmp/device-sign.dat"
#define DEVICE_TYPE_FILE        "/etc/device_type.dat"


#ifdef FLASH_8
/** @ 定义针对8M Flash 设计
 *  @ 在nvram中存储sn, oem, 设备类型
 */
#define FLASH_8_DEVTYPE     "config_flash_8_devcie_type"
#define FLASH_8_OEM         "config_flash_8_oem"
#define FLASH_8_SN          "config_flash_8_sn"
#define FLASH_8_PRODUCT_SN  "config_flash_8_prosn"

#endif

/* aptool 命令集 */
typedef struct _aptool_command
{
    const char *command;
    int (*func)(int argc, char **argv);
    const char *help;
}aptool_command;

typedef struct wifi_amp_cfg
{
       unsigned char      ucWifiDevAmpSwitch;
       unsigned char      ucWifiDevAmpGain;
       unsigned char      ucWifiDevAmpMaxInput;
       unsigned char      ucRsv;
}T_WIFI_AMP_CFG;

/* 定义产测结构 */
#ifdef AP93_OAM
typedef struct deviceSign_s
{
	long	lDeviceType;				/* 设备类型 */	
	unsigned char	aucDeviceId[6];		/* 设备ID，6个16进制数 */
	int		setFlag;					/* 是否配置了设备类型标识 */
	unsigned long	ulDeviceModuleType;	/* 设备主从类型*/
#define sysDeviceModuleType_ALONE	0
#define sysDeviceModuleType_MAIN	1
#define sysDeviceModuleType_SLAVE	2
#define WIFI_DEV_AMP_SET_MASK  	    0x01
#define WIFI_DEV_AMP_VALUE_MASK     0xf8
    T_WIFI_AMP_CFG    stModuleWifiAmpCfg1;
    T_WIFI_AMP_CFG    stModuleWifiAmpCfg2;
} T_deviceSign;
#else
typedef struct deviceSign_s
{
	long	        lDeviceType;		    /* 设备型号 */
	unsigned char	aucDeviceId[6];		    /* 设备ID，6个16进制数 */
	unsigned long	ulDeviceModuleType; 	/* 设备主从类型*/
    T_WIFI_AMP_CFG  stModuleWifiAmpCfg1;
    T_WIFI_AMP_CFG  stModuleWifiAmpCfg2;
    
	unsigned char   aucCustomer[64];        /* 配置客户标记 , 以字符串形式的客户代号 默认值全0 */
	unsigned char   aucProductSn[64];       /* Dlink 产品序列号 */
	unsigned char   acinternalmodel[64];    /* add by zhz 打印model name */
	unsigned char   aucRsv3[64];            /* 预留字段3，默认值为全0 */
	unsigned char   aucRsv4[64];            /* 预留字段4，默认值为全0 */

	int	            setFlag;				/* 是否配置了设备类型标识 */
}T_deviceSign;
#endif

#endif /* __APTOOL_H__ */
