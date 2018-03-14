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

/* Ӳ���豸���Ͷ��� */
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
/** @ �������8M Flash ���
 *  @ ��nvram�д洢sn, oem, �豸����
 */
#define FLASH_8_DEVTYPE     "config_flash_8_devcie_type"
#define FLASH_8_OEM         "config_flash_8_oem"
#define FLASH_8_SN          "config_flash_8_sn"
#define FLASH_8_PRODUCT_SN  "config_flash_8_prosn"

#endif

/* aptool ��� */
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

/* �������ṹ */
#ifdef AP93_OAM
typedef struct deviceSign_s
{
	long	lDeviceType;				/* �豸���� */	
	unsigned char	aucDeviceId[6];		/* �豸ID��6��16������ */
	int		setFlag;					/* �Ƿ��������豸���ͱ�ʶ */
	unsigned long	ulDeviceModuleType;	/* �豸��������*/
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
	long	        lDeviceType;		    /* �豸�ͺ� */
	unsigned char	aucDeviceId[6];		    /* �豸ID��6��16������ */
	unsigned long	ulDeviceModuleType; 	/* �豸��������*/
    T_WIFI_AMP_CFG  stModuleWifiAmpCfg1;
    T_WIFI_AMP_CFG  stModuleWifiAmpCfg2;
    
	unsigned char   aucCustomer[64];        /* ���ÿͻ���� , ���ַ�����ʽ�Ŀͻ����� Ĭ��ֵȫ0 */
	unsigned char   aucProductSn[64];       /* Dlink ��Ʒ���к� */
	unsigned char   acinternalmodel[64];    /* add by zhz ��ӡmodel name */
	unsigned char   aucRsv3[64];            /* Ԥ���ֶ�3��Ĭ��ֵΪȫ0 */
	unsigned char   aucRsv4[64];            /* Ԥ���ֶ�4��Ĭ��ֵΪȫ0 */

	int	            setFlag;				/* �Ƿ��������豸���ͱ�ʶ */
}T_deviceSign;
#endif

#endif /* __APTOOL_H__ */
