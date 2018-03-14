/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : client_ap.h
  Version       : Initial Draft
  Author        : xiabing
  Created       : 2015/01/28
  Last Modified :
  Description   : client_ap.c header file
  Function List :
  History       :
  1.Date        : 2015/01/28
    Author      : xiabing
    Modification: Created file

******************************************************************************/

#ifndef __CLIENT_AP_H__
#define __CLIENT_AP_H__
#include "list.h"
/******************AP PARA***************/
#define AP_HASH_SIZE 256
#define AP_LIST_LOCK()		pthread_mutex_lock(&g_ap_list_mutex)
#define AP_LIST_UNLOCK()	pthread_mutex_unlock(&g_ap_list_mutex)
#define WD_AP_MAX_RADIO_NUM 2
#define WD_RADIO_MAX_SSID_NUM 16
typedef struct
{
    unsigned char aucBssid[6];          /*bssid*/
	char acSsid[32];					/*ssid*/
} WD_AP_VAP_T;

typedef struct
{
	unsigned int uiRadioId;			    /*radio id*/
	unsigned int uiRadioType;           /*radio type: 0(2g), 1(5g)*/
	unsigned int uiVapNum;
	WD_AP_VAP_T astVap[WD_RADIO_MAX_SSID_NUM];
} WD_AP_RADIO_T;

typedef struct
{
	unsigned char mac[6];          		/* AP MAC */
	unsigned char ip[16];          		/* AP IP */
	char model[16];						/* model*/
	char name[65];						/* name*/
	unsigned int uiRadioNum;            /*radio num*/
	WD_AP_RADIO_T astRadio[WD_AP_MAX_RADIO_NUM];
	
	struct hlist_node mac_Hlist;       	/* AP MAC hash chain */
} WD_AP_T;

extern struct hlist_head g_ap_HList[AP_HASH_SIZE];
extern pthread_mutex_t g_ap_list_mutex;

/******************STA PARA***************/
#define STA_HASH_SIZE 512
#define STA_LIST_LOCK()		pthread_mutex_lock(&g_sta_list_mutex)
#define STA_LIST_UNLOCK()	pthread_mutex_unlock(&g_sta_list_mutex)

typedef struct
{
	unsigned char mac[6];          		/* STA MAC */
	unsigned char ap_mac[6];          	/* 接入的AP MAC */
	char ssid[32];						/* 接入的SSID*/
	struct hlist_node mac_Hlist;       	/* STA MAC hash chain */
} WD_STA_T;

extern struct hlist_head g_sta_HList[STA_HASH_SIZE];
extern pthread_mutex_t g_sta_list_mutex;

/******************interface*****************/
extern int sta_add_by_mac(unsigned char *mac, unsigned char *ap_mac, char *ssid);
extern int sta_del_by_mac(unsigned char *mac, unsigned char *apmac, char *ssid);
extern int ap_add_by_mac(unsigned char *mac, unsigned char *ip, unsigned int model_id, 
	       				  char *name, unsigned int uiRadioNum, WD_AP_RADIO_T *pstRadio);
WD_AP_T * ap_find_by_mac(unsigned char *mac);
extern int ap_del_by_mac(unsigned char *mac);
extern int sta_dct_add_by_mac(unsigned char *ap_mac, unsigned char *sta_mac, unsigned char rssi);
extern int sta_dct_del_by_mac(unsigned char *ap_mac, unsigned char *sta_mac);
extern int sta_dct_enable(int enable);
extern int show_sta_cnnt_list(int fd);
extern int show_reg_ap_list(int fd);
extern int ap_info_print(WD_AP_T *pstAp);

int get_model_by_model_id(unsigned int model_id, char *model);

#endif /* __CLIENT_AP_H__ */

