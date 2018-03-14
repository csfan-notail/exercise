/******************************************************************************

  Copyright (C), 2008-2012, GBCOM Co., Ltd.

 ******************************************************************************
  File Name     : my_snmp_aglink_client.h
  Version       : Initial Draft
  Author        : xiabing
  Created       : 2014/12/03
  Last Modified :
  Description   : my_snmp_aglink_client.c header file
  Function List :
  History       :
  1.Date        : 2014/12/03
    Author      : xiabing
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

#ifndef __MY_SNMP_AGLINK_CLIENT_H__
#define __MY_SNMP_AGLINK_CLIENT_H__
#include <pthread.h>

/*******************************宏定义************************************/
#define SNMPAGENT_STA_MAX_NUM 128
#define SNMP_ALL_STA_MUTEX_INIT(x) do \
        {\
		     pthread_mutex_init(&x, NULL);\
        }while(0);
#define SNMP_ALL_STA_MUTEX_LOCK(x) pthread_mutex_lock(&x); 
#define SNMP_ALL_STA_MUTEX_UNLOCK(x) pthread_mutex_unlock(&x);
/******************************* 消息号定义 ********************************/


/*****************************结构体定义**********************************/
typedef struct {
	unsigned char aucMac[6];
    unsigned char aucIp[16];
    int iAuthState;       /*认证状态，STAM_STA_UNAUTHED, STAM_STA_AUTHED*/
	long iLoginTimeSet;   /*设置已认证时的uptime*/
	long iOnlineTime;     /*已认证时长*/
    long iAssoTimeSet;    /*创建表项的uptime*/
    long iAssoTime;       /*已关联时长*/
}SNMPAGENT_STA_T;
typedef struct __SNMPAGENT_STA_INFO{
    pthread_mutex_t mutex;
	int iStaNum;
    SNMPAGENT_STA_T astSta[SNMPAGENT_STA_MAX_NUM];
}SNMPAGENT_ALL_STA_INFO_T;



/**********************************函数声明**************************************/
extern SNMPAGENT_ALL_STA_INFO_T g_stSnmpAllStaInfo;

extern int snmp_notify_wifidog_get_gw_id();
extern int snmp_notify_stam_get_all_sta();
extern int snmp_notify_wifidog_get_all_sta();
extern int snmp_notify_wifidog_set_ap_ssid(unsigned char *aucApMac, 
                          						unsigned int uiSsidNumber,
					                            unsigned int  uiRadioType,
					                            char *acVapName,
					                            char *acssid,
					                            unsigned int uiDefault,
					                            unsigned int uiUpRate,
					                            unsigned int uiDownRate,
					                            char *acPortalUrl,
					                            unsigned int uiOnlineControlTime,
					                            unsigned int uiFlowOffTime);
extern int snmp_notify_wifidog_del_ap_ssid(unsigned char *aucApMac, unsigned int uiSsidNumber);
extern int snmp_notify_wifidog_set_self_ssid(unsigned char *aucApMac, 
                          						unsigned int uiSsidNumber,
					                            unsigned int  uiRadioType,
					                            char *acVapName,
					                            char *acssid,
					                            unsigned int uiDefault,
					                            unsigned int uiUpRate,
					                            unsigned int uiDownRate,
					                            char *acPortalUrl,
					                            unsigned int uiOnlineControlTime,
					                            unsigned int uiFlowOffTime);
extern int snmp_notify_wifidog_del_self_ssid(unsigned char *aucApMac, unsigned int uiSsidNumber);
extern int snmp_notify_wifidog_add_mac_baw(unsigned int uiSsidNumber, int iNum, SNMPAGENT_MAC_BAW_T *pstMacBaw);
extern int snmp_notify_wifidog_del_mac_baw(unsigned int uiSsidNumber, int iNum, SNMPAGENT_MAC_BAW_T *pstMacBaw);
extern int snmp_notify_wifidog_add_url_baw(unsigned int uiSsidNumber, int iNum, SNMPAGENT_URL_BAW_T *pstUrlBaw);
extern int snmp_notify_wifidog_del_url_baw(unsigned int uiSsidNumber, int iNum, SNMPAGENT_URL_BAW_T *pstUrlBaw);
extern int snmp_notify_wifidog_user_logout(AP_CONFIG_USER_LOGOUT_T *pstUser);
extern int snmp_notify_wifidog_user_speed(AP_CONFIG_USER_SPEED_T *pstUser);
extern int snmp_notify_wifidog_white_user_speed(AP_CONFIG_USER_SPEED_T *pstUser);
extern int snmp_notify_wifidog_get_secret_key(AP_CONFIG_GET_SECRET_KEY_T *pstIntval);

#endif 

