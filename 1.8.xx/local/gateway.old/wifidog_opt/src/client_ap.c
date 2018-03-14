/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : client_rdl.c

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <syslog.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <stdarg.h>
#include <pthread.h>
#include "cJSON.h"
#include "httpd.h"

#include "common.h"
#include "conf.h"
#include "auth_server.h"
#include "http.h"
#include "log.h"
#include "util.h"
#include "client_list.h"
#include "firewall.h"
#include "redirect_portal.h"

#include <signal.h>
#include <sys/time.h>

#include "list.h"
#include "client_ap.h"

struct hlist_head g_sta_HList[STA_HASH_SIZE];
pthread_mutex_t g_sta_list_mutex;

struct hlist_head g_ap_HList[AP_HASH_SIZE];
pthread_mutex_t g_ap_list_mutex;

extern int g_ext_dbg_level;

inline int mac_hashfn(unsigned char *mac, int hash_size)
{
	int	x;
	int	index;
	
	x = mac[0];
	x = (x << 2) ^ mac[1];
	x = (x << 2) ^ mac[2];
	x = (x << 2) ^ mac[3];
	x = (x << 2) ^ mac[4];
	x = (x << 2) ^ mac[5];

	x ^= x >> 8;

	index = x & (hash_size - 1);

	return index;
}

int get_model_by_model_id(unsigned int model_id, char *model)
{
	switch(model_id)
	{
		case 9001001:
			sprintf(model, "CA1020C");
			break;
		case 9001005:
			sprintf(model, "EA2020C");
			break;
		case 9001004:
			sprintf(model, "CA2020C");
			break;
		case 9001002:
			sprintf(model, "TA2020C");
			break;
		case 9002001:
			sprintf(model, "CA2025C");
			break;
		case 7004001:
			sprintf(model, "CGW500");
			break;
		case 7004002:
			sprintf(model, "CGW500");
			break;

            case 7005001:
			sprintf(model, "CGW600");
			break;

            case 7006001:
			sprintf(model, "CGW800");
			break;

		case 7003001:
			sprintf(model, "CGW1200");
			break;
		case 7003002:
			sprintf(model, "CGW1200H");
			break;
		case 1:
			sprintf(model, "CGW2000");
			break;
		case 2:
			sprintf(model, "CGW3000");
			break;
		default:
			sprintf(model, "OTHER");
			break;
	}
	return 0;
}


int sta_add_by_mac(unsigned char *mac, unsigned char *ap_mac, char *ssid)
{
	WD_STA_T *pststa;
	struct hlist_node *p;
	struct hlist_head *head;
	unsigned int index;
	unsigned char empty_ap_mac[6];
	unsigned char *tmp_ap_mac;
	char empty_ssid[32] = "";
	char *tmp_ssid;

	if(NULL == mac)
		return -1;
	
	index = mac_hashfn(mac, STA_HASH_SIZE);
	head = &(g_sta_HList[index]);

	STA_LIST_LOCK();
	hlist_for_each( p, head )
	{
		pststa = hlist_entry(p, WD_STA_T, mac_Hlist);
		if (!memcmp(pststa->mac, mac, 6)){ /*终端存在*/
			if(ap_mac != NULL)
				tmp_ap_mac = ap_mac;
			else{
				memset(empty_ap_mac, 0, 6);
				tmp_ap_mac = &empty_ap_mac[0];
			}
			if (memcmp(pststa->ap_mac, tmp_ap_mac, 6)){ /*需要更新ap mac*/
				log_info(LOG_INFO, "ap-sta:  ap mac change from "MAC_FMT"to "MAC_FMT" of sta("MAC_FMT")",
					MAC_ARG(pststa->ap_mac), MAC_ARG(tmp_ap_mac), MAC_ARG(mac));
				memcpy(pststa->ap_mac, tmp_ap_mac, 6);
				STA_LIST_UNLOCK();
				return 0;
			}

			if(ssid != NULL)
				tmp_ssid = ssid;
			else
				tmp_ssid = &empty_ssid[0];
			if(ssid != NULL){
				if (strncmp(pststa->ssid, tmp_ssid, 32)){ /*需要更新ssid*/
					log_info(LOG_INFO, "ap-sta:  ssid change from %s to %s of sta("MAC_FMT")",
						pststa->ssid, tmp_ssid, MAC_ARG(mac));
					strncpy(pststa->ssid, tmp_ssid, 32);
					STA_LIST_UNLOCK();
					return 0;
				}
			}

			STA_LIST_UNLOCK();
			return 0;				
		}
	}

	/*终端未找到，加入终端*/
	pststa = (WD_STA_T*)malloc(sizeof(WD_STA_T));
	memset(pststa, 0, sizeof(WD_STA_T));
	memcpy(pststa->mac, mac, 6);
	if(ap_mac != NULL)
		memcpy(pststa->ap_mac, ap_mac, 6);
	if(ssid != NULL)
		strncpy(pststa->ssid, ssid, 32);
	hlist_add_head(&(pststa->mac_Hlist), head);
	STA_LIST_UNLOCK();

	log_info(LOG_DEBUG, "ap-sta:  sta ("MAC_FMT") of ap ("MAC_FMT") - ssid(%s) is added", 
		MAC_ARG(pststa->mac), MAC_ARG(pststa->ap_mac), pststa->ssid);

	if(g_ext_dbg_level > 2){
		int i;
		log_info(LOG_DEBUG, "ALL STAs ->>");
		STA_LIST_LOCK();
		for(i=0; i<STA_HASH_SIZE; ++i){
			hlist_for_each(p, &g_sta_HList[i])
			{
				pststa = hlist_entry(p, WD_STA_T, mac_Hlist);
				log_info(LOG_DEBUG, "  sta("MAC_FMT") of ap("MAC_FMT") - ssid(%s)", 
					MAC_ARG(pststa->mac), MAC_ARG(pststa->ap_mac), pststa->ssid);
			}
		}
		STA_LIST_UNLOCK(); 
	}

	return 0;	
}


int sta_del_by_mac(unsigned char *mac, unsigned char *apmac, char *ssid)
{
	WD_STA_T *pststa;
	struct hlist_node *p, *n;
	struct hlist_head *head;
	unsigned int index;
	
	index = mac_hashfn(mac, STA_HASH_SIZE);
	head = &(g_sta_HList[index]);

	STA_LIST_LOCK();
	hlist_for_each_safe(p, n, head)
	{
		pststa = hlist_entry(p, WD_STA_T, mac_Hlist);
		if (!memcmp(pststa->mac, mac, 6)){
			if(!memcmp(pststa->ap_mac, apmac, 6)){
				if(!strcmp(pststa->ssid, ssid)){
					hlist_del(&(pststa->mac_Hlist));
					log_info(LOG_DEBUG, "del STA "MAC_FMT, MAC_ARG(mac));
					free(pststa);
					break;
				}
			}
		}
	}
	STA_LIST_UNLOCK();

	if(g_ext_dbg_level > 2){
		int i;
		log_info(LOG_DEBUG, "ALL STAs ->>");
		STA_LIST_LOCK();
		for(i=0; i<STA_HASH_SIZE; ++i){
			hlist_for_each(p, &g_sta_HList[i])
			{
				pststa = hlist_entry(p, WD_STA_T, mac_Hlist);
				log_info(LOG_DEBUG, "  sta("MAC_FMT") of ap("MAC_FMT")", 
					MAC_ARG(pststa->mac), MAC_ARG(pststa->ap_mac));
			}
		}
		STA_LIST_UNLOCK(); 
	}

	return 0;	
}

int sta_del_by_ap_mac(unsigned char *ap_mac)
{
	int i;
	WD_STA_T *pststa;
	struct hlist_node *p, *n;
	
	STA_LIST_LOCK();
	for(i=0; i<STA_HASH_SIZE; ++i){
		hlist_for_each_safe(p, n, &g_sta_HList[i])
		{
			pststa = hlist_entry(p, WD_STA_T, mac_Hlist);
			if (!memcmp(pststa->ap_mac, ap_mac, 6)){
				log_info(LOG_DEBUG, "del STA "MAC_FMT, MAC_ARG(pststa->mac));
				hlist_del(&(pststa->mac_Hlist));
				free(pststa);
				break;
			}
		}
	}
	STA_LIST_UNLOCK();

	if(g_ext_dbg_level > 2){
		log_info(LOG_DEBUG, "ALL STAs ->>");
		STA_LIST_LOCK();
		for(i=0; i<STA_HASH_SIZE; ++i){
			hlist_for_each(p, &g_sta_HList[i])
			{
				pststa = hlist_entry(p, WD_STA_T, mac_Hlist);
				log_info(LOG_DEBUG, "  sta("MAC_FMT") of ap("MAC_FMT")", 
					MAC_ARG(pststa->mac), MAC_ARG(pststa->ap_mac));
			}
		}
		STA_LIST_UNLOCK(); 
	}

	return 0;	
}



int get_sta_ap_mac(char *str_mac, char *ap_mac)
{
	WD_STA_T *pststa;
	struct hlist_node *p;
	struct hlist_head *head;
	unsigned int index;
	unsigned char mac[6];
	s_config *config = config_get_config();
	ap_mac[0] = '\0';
	int find_ap = 0;

	if(strmac_to_mac(str_mac, mac) >= 0){
		index = mac_hashfn(mac, STA_HASH_SIZE);
		head = &(g_sta_HList[index]);

		STA_LIST_LOCK();
		hlist_for_each(p, head)
		{
			pststa = hlist_entry(p, WD_STA_T, mac_Hlist);
			if (!memcmp(pststa->mac, mac, 6)){
				sprintf(ap_mac, "&apmac="MAC_FMT, MAC_ARG(pststa->ap_mac));
				find_ap = 1;
				break;
			}
		}
		STA_LIST_UNLOCK();
	}
	else{
		return -1;
	}
	if (0 == find_ap){
		/*add@2015.02.06 for wechat connect wifi*/
		log_info(__LOG_DEBUG__, "find_ap = 0");
		sprintf(ap_mac, "&apmac=%s", config->wan_mac);
	}
	log_info(__LOG_DEBUG__, "%s", ap_mac);
	return 0;	
}

int get_sta_ap_mac2(char *str_mac, char *ap_mac)
{
	WD_STA_T *pststa;
	struct hlist_node *p;
	struct hlist_head *head;
	unsigned int index;
	unsigned char mac[6];
	s_config *config = config_get_config();
	ap_mac[0] = '\0';
	int find_ap = 0;

	if(strmac_to_mac(str_mac, mac) >= 0){
		index = mac_hashfn(mac, STA_HASH_SIZE);
		head = &(g_sta_HList[index]);

		STA_LIST_LOCK();
		hlist_for_each(p, head)
		{
			pststa = hlist_entry(p, WD_STA_T, mac_Hlist);
			if (!memcmp(pststa->mac, mac, 6)){
				sprintf(ap_mac, MAC_FMT, MAC_ARG(pststa->ap_mac));
				find_ap = 1;
				break;
			}
		}
		STA_LIST_UNLOCK();
	}
	else{
		return -1;
	}
	if (0 == find_ap){
		/*add@2015.02.06 for wechat connect wifi*/
		log_info(__LOG_DEBUG__, "find_ap = 0");
		sprintf(ap_mac, "&apmac=%s", config->wan_mac);
	}
	log_info(__LOG_DEBUG__, "%s", ap_mac);
	return 0;	
}



int ap_add_by_mac(unsigned char *mac, unsigned char *ip, unsigned int model_id, 
	       				  char *name, unsigned int uiRadioNum, WD_AP_RADIO_T *pstRadio)
{
	WD_AP_T *pstap;
	struct hlist_node *p;
	struct hlist_head *head;
	unsigned int index;
	int ap_update = 0;
	char ap_type[16];
	//unsigned int uiRadioType = 0;
	int i,j;

	get_model_by_model_id(model_id, ap_type);
	
	index = mac_hashfn(mac, AP_HASH_SIZE);
	head = &(g_ap_HList[index]);

	AP_LIST_LOCK();
	hlist_for_each( p, head )
	{
		pstap = hlist_entry(p, WD_AP_T, mac_Hlist);
		if (!memcmp(pstap->mac, mac, 6)){
			if (memcmp(pstap->ip, ip, 16)){
				log_info(LOG_INFO, "ap-sta:  ap("MAC_FMT")'s ip changes from "IP_FMT"to "IP_FMT,
					MAC_ARG(pstap->mac), IP_ARG(pstap->ip), IP_ARG(ip));
				memcpy(pstap->ip, ip, 16);
				ap_update = 1;
			}
			if(strncmp(ap_type, pstap->model, 16)){
				log_info(LOG_INFO, "ap-sta:  ap("MAC_FMT")'s type changes from %s to %s",
					MAC_ARG(pstap->mac), pstap->model, ap_type);
				strncpy(pstap->model, ap_type, 16);
				ap_update = 1;
			}
			if(strncmp(name, pstap->name, 64)){
				log_info(LOG_INFO, "ap-sta:  ap("MAC_FMT")'s name changes from %s to %s",
					MAC_ARG(pstap->mac), pstap->name, name);
				strncpy(pstap->name, name, 64);
				ap_update = 1;
			}
			
			if (uiRadioNum > WD_AP_MAX_RADIO_NUM)
				pstap->uiRadioNum = WD_AP_MAX_RADIO_NUM;
			else
				pstap->uiRadioNum = uiRadioNum;
			
			for (i = 0; i < uiRadioNum; i++){
				pstap->astRadio[i].uiRadioId = pstRadio[i].uiRadioId;
				pstap->astRadio[i].uiRadioType = pstRadio[i].uiRadioType;
				if (pstRadio[i].uiVapNum > WD_RADIO_MAX_SSID_NUM)
					pstap->astRadio[i].uiVapNum = WD_RADIO_MAX_SSID_NUM;
				else
					pstap->astRadio[i].uiVapNum = pstRadio[i].uiVapNum;
				for (j = 0; j < pstRadio[i].uiVapNum; j++){
					memcpy(pstap->astRadio[i].astVap[j].aucBssid,
						   pstRadio[i].astVap[j].aucBssid, 6);
					strcpy(pstap->astRadio[i].astVap[j].acSsid,
						   pstRadio[i].astVap[j].acSsid);
				}
			}
			
			if(ap_update){
				AP_LIST_UNLOCK();
				return 0;
			}
			else{
				AP_LIST_UNLOCK();
				return -1;
			}
		}
	}

	/*AP未找到，加入AP*/
	pstap = (WD_AP_T*)malloc(sizeof(WD_AP_T));
	memcpy(pstap->mac, mac, 6);
	memcpy(pstap->ip, ip, 16);
	strncpy(pstap->model, ap_type, 16);
	strncpy(pstap->name, name, 64);
	if (uiRadioNum > WD_AP_MAX_RADIO_NUM)
		pstap->uiRadioNum = WD_AP_MAX_RADIO_NUM;
	else
		pstap->uiRadioNum = uiRadioNum;
	
	for (i = 0; i < uiRadioNum; i++){
		pstap->astRadio[i].uiRadioId = pstRadio[i].uiRadioId;
		pstap->astRadio[i].uiRadioType = pstRadio[i].uiRadioType;
		if (pstRadio[i].uiVapNum > WD_RADIO_MAX_SSID_NUM)
			pstap->astRadio[i].uiVapNum = WD_RADIO_MAX_SSID_NUM;
		else
			pstap->astRadio[i].uiVapNum = pstRadio[i].uiVapNum;

		for (j = 0; j < pstRadio[i].uiVapNum; j++){
			memcpy(pstap->astRadio[i].astVap[j].aucBssid,
				   pstRadio[i].astVap[j].aucBssid, 6);
			strcpy(pstap->astRadio[i].astVap[j].acSsid,
				   pstRadio[i].astVap[j].acSsid);
		}
	}
	
	hlist_add_head(&(pstap->mac_Hlist), head);
	AP_LIST_UNLOCK();

	log_info(LOG_INFO, "ap-sta:  ap("MAC_FMT") ("IP_FMT") (%s) (%s)is added", 
		MAC_ARG(pstap->mac), IP_ARG(pstap->ip), pstap->model, pstap->name);

	if(g_ext_dbg_level > 2){
		int i;
		log_info(LOG_DEBUG, "ALL APs ->>");
		AP_LIST_LOCK();
		for(i=0; i<AP_HASH_SIZE; ++i){
			hlist_for_each(p, &g_ap_HList[i])
			{
				pstap = hlist_entry(p, WD_AP_T, mac_Hlist);
				log_info(LOG_DEBUG, "  ap("MAC_FMT") ("IP_FMT") %s %s", 
					MAC_ARG(pstap->mac), IP_ARG(pstap->ip), pstap->model, pstap->name);
			}
		}
		AP_LIST_UNLOCK();
	}

	return 0;	
}


WD_AP_T * ap_find_by_mac(unsigned char *mac)
{
	WD_AP_T *pstap = NULL;
	struct hlist_node *p;
	struct hlist_head *head;
	unsigned int index;
	
	index = mac_hashfn(mac, AP_HASH_SIZE);
	head = &(g_ap_HList[index]);

	AP_LIST_LOCK();
	hlist_for_each( p, head )
	{
		pstap = hlist_entry(p, WD_AP_T, mac_Hlist);
		if (!memcmp(pstap->mac, mac, 6)){
			AP_LIST_UNLOCK();
			return pstap;
		}
	}
	AP_LIST_UNLOCK();

	return NULL;	
}
int ap_del_by_mac(unsigned char *mac)
{
	WD_AP_T *pstap;
	struct hlist_node *p, *n;
	struct hlist_head *head;
	unsigned int index;
	
	index = mac_hashfn(mac, AP_HASH_SIZE);
	head = &(g_ap_HList[index]);

	AP_LIST_LOCK();
	hlist_for_each_safe(p, n, head)
	{
		pstap = hlist_entry(p, WD_AP_T, mac_Hlist);
		if (!memcmp(pstap->mac, mac, 6)){
			hlist_del(&(pstap->mac_Hlist));
			log_info(LOG_INFO, "del AP ("MAC_FMT") ("IP_FMT")", 
				MAC_ARG(pstap->mac), IP_ARG(pstap->ip));
			free(pstap);
			break;
		}
	}
	AP_LIST_UNLOCK();

	sta_del_by_ap_mac(mac);

	if(g_ext_dbg_level > 2){
		int i;
		log_info(LOG_DEBUG, "ALL APs ->>");
		AP_LIST_LOCK();
		for(i=0; i<AP_HASH_SIZE; ++i){
			hlist_for_each(p, &g_ap_HList[i])
			{
				pstap = hlist_entry(p, WD_AP_T, mac_Hlist);
				log_info(LOG_DEBUG, "  ap("MAC_FMT") ("IP_FMT")", 
					MAC_ARG(pstap->mac), IP_ARG(pstap->ip));
			}
		}
		AP_LIST_UNLOCK(); 
	}

	return 0;	
}


WD_AP_T * ap_get_by_mac(unsigned char *mac)
{
	WD_AP_T *pstap;
	WD_AP_T *r_pstap;
	struct hlist_node *p, *n;
	struct hlist_head *head;
	unsigned int index;
	
	index = mac_hashfn(mac, AP_HASH_SIZE);
	head = &(g_ap_HList[index]);

	AP_LIST_LOCK();
	hlist_for_each_safe(p, n, head)
	{
		pstap = hlist_entry(p, WD_AP_T, mac_Hlist);
		if (!memcmp(pstap->mac, mac, 6)){
			r_pstap = (WD_AP_T *)malloc(sizeof(WD_AP_T));
			AP_LIST_UNLOCK();
			return r_pstap;
		}
	}
	AP_LIST_UNLOCK();
	
	return NULL;	
}

int ap_info_print(WD_AP_T *pstAp)
{
	int i,j;
	if (NULL == pstAp){
		log_info(__LOG_ERROR__, "NULL == pstAp, fail");
		return -1;
	}
	log_info(__LOG_DEBUG__, "mac   :   "MAC_FMT, MAC_ARG(pstAp->mac));
	log_info(__LOG_DEBUG__, "ip    :   "IP_FMT, IP_ARG(pstAp->ip));
	log_info(__LOG_DEBUG__, "model :   %s", pstAp->model);
	log_info(__LOG_DEBUG__, "name  :   %s", pstAp->name);
	log_info(__LOG_DEBUG__, "radios:   %u", pstAp->uiRadioNum);
	for(i = 0; i < pstAp->uiRadioNum; i++){
		log_info(__LOG_DEBUG__, "radio%d.id  :   %u", i, pstAp->astRadio[i].uiRadioId);
		log_info(__LOG_DEBUG__, "radio%d.type:   %u", i, pstAp->astRadio[i].uiRadioType);
		log_info(__LOG_DEBUG__, "radio%d.vaps:   %u", i, pstAp->astRadio[i].uiVapNum);
		for(j = 0; j < pstAp->astRadio[i].uiVapNum; j++){
			log_info(__LOG_DEBUG__, "radio%d.vap%d.bssid:   "MAC_FMT, i, j, MAC_ARG(pstAp->astRadio[i].astVap[j].aucBssid));
			log_info(__LOG_DEBUG__, "radio%d.vap%d.ssid :   %s", i, j, pstAp->astRadio[i].astVap[j].acSsid);
		}
	}
	return 0;
}

char g_ap_list[7168];
int ap_send_list_report(void *func, void *config, int *sockfd)
{
	int i,j;
	struct hlist_node *p;
	WD_AP_T *pstap;
	int ap_num;
	char temp[4096];
	int len;
	char ssid_type[16];

	void (*send_ap_list)(int , char *, int *, void *, char *) = func;
	
    memset(g_ap_list, 0, sizeof(g_ap_list));
    ap_num = 0;
    AP_LIST_LOCK();
    for(i = 0; i < AP_HASH_SIZE; ++i)
    {
        hlist_for_each(p, &g_ap_HList[i])
        {
            memset(temp, 0, sizeof(temp));
            pstap = hlist_entry(p, WD_AP_T, mac_Hlist);
            len = strlen(pstap->name);
            for(j = 0; j < len; ++j)
            { /*名称为空格则改为-*/
                if(' ' == pstap->name[j])
                {
                    pstap->name[j] = '-';
                }
            }
			
            if (1 == pstap->uiRadioNum)
            {
            	int s;
				strcpy(ssid_type, "wxwifi");

				/*1个radio，只考虑2.4G*/
				for(s=0; s<pstap->astRadio[0].uiVapNum; ++s){
					if(strlen(pstap->astRadio[0].astVap[s].acSsid) <= 0)
						continue;
					
					/*首个SSID用于微信连 ,其余SSID仅上报*/
					sprintf(temp + strlen(temp), IP_FMT"|"MAC_FMT"|%s|%s|%s|"MAC_FMT"|%s;", 
						IP_ARG(pstap->ip), MAC_ARG(pstap->mac), pstap->model, pstap->name, 
						pstap->astRadio[0].astVap[s].acSsid, 
						MAC_ARG(pstap->astRadio[0].astVap[s].aucBssid), ssid_type);
					strcpy(ssid_type, "other");
					++ap_num;
				}
            }
            else if (2 == pstap->uiRadioNum)
            {
            	int s0, s1;
				strcpy(ssid_type, "wxwifi");
				int for_wxfifi_s0 = -1;
				int for_wxfifi_s1 = -1;
				
				for(s0=0; s0<pstap->astRadio[0].uiVapNum; ++s0){
					for(s1=0; s1<pstap->astRadio[1].uiVapNum; ++s1){
						if(!strcmp(pstap->astRadio[0].astVap[s0].acSsid, 
							pstap->astRadio[1].astVap[s1].acSsid))/*首个2.4G/5G SSID对，用于微信连*/
						{
							if(strlen(pstap->astRadio[0].astVap[s0].acSsid) > 0){
								sprintf(temp + strlen(temp), IP_FMT"|"MAC_FMT"|%s|%s|%s|"MAC_FMT","MAC_FMT"|%s;", 
									IP_ARG(pstap->ip), MAC_ARG(pstap->mac), pstap->model, pstap->name, 
									pstap->astRadio[0].astVap[s0].acSsid, 
									MAC_ARG(pstap->astRadio[0].astVap[s0].aucBssid), 
									MAC_ARG(pstap->astRadio[1].astVap[s1].aucBssid), ssid_type);								
								for_wxfifi_s0 = s0;
								for_wxfifi_s1 = s1;
								strcpy(ssid_type, "other");
								++ap_num;
								goto for_other_ssid;
							}
						}
					}
				}

for_other_ssid: /*其余SSID，仅上报*/	
				for(s0=0; s0<pstap->astRadio[0].uiVapNum; ++s0){
					if(s0 == for_wxfifi_s0)
						continue;
					
					for(s1=0; s1<pstap->astRadio[1].uiVapNum; ++s1){
						if(!strcmp(pstap->astRadio[0].astVap[s0].acSsid, 
							pstap->astRadio[1].astVap[s1].acSsid))/*SSID对，一起上报*/
						{
							if(strlen(pstap->astRadio[0].astVap[s0].acSsid) > 0){
								sprintf(temp + strlen(temp), IP_FMT"|"MAC_FMT"|%s|%s|%s|"MAC_FMT","MAC_FMT"|%s;", 
									IP_ARG(pstap->ip), MAC_ARG(pstap->mac), pstap->model, pstap->name, 
									pstap->astRadio[0].astVap[s0].acSsid, 
									MAC_ARG(pstap->astRadio[0].astVap[s0].aucBssid), 
									MAC_ARG(pstap->astRadio[1].astVap[s1].aucBssid), ssid_type);
								strcpy(ssid_type, "other");
								++ap_num;
								break;
							}		
						}
					}

					if(s1 >= pstap->astRadio[1].uiVapNum){ /*单个2.4G SSID*/
						sprintf(temp + strlen(temp), IP_FMT"|"MAC_FMT"|%s|%s|%s|"MAC_FMT"|%s;", 
							IP_ARG(pstap->ip), MAC_ARG(pstap->mac), pstap->model, pstap->name, 
							pstap->astRadio[0].astVap[s0].acSsid, 
							MAC_ARG(pstap->astRadio[0].astVap[s0].aucBssid), ssid_type);
						strcpy(ssid_type, "other");
						++ap_num;
					}
				}

				for(s1=0; s1<pstap->astRadio[1].uiVapNum; ++s1){
					if(s1 == for_wxfifi_s1)
						continue;

					for(s0=0; s0<pstap->astRadio[0].uiVapNum; ++s0){
						if(!strcmp(pstap->astRadio[0].astVap[s0].acSsid, 
							pstap->astRadio[1].astVap[s1].acSsid))
						{
							break;
						}
					}

					/*单个5G SSID*/
					if(s0 >= pstap->astRadio[0].uiVapNum){
						sprintf(temp + strlen(temp), IP_FMT"|"MAC_FMT"|%s|%s|%s|"MAC_FMT"|%s;", 
							IP_ARG(pstap->ip), MAC_ARG(pstap->mac), pstap->model, pstap->name, 
							pstap->astRadio[1].astVap[s1].acSsid, 
							MAC_ARG(pstap->astRadio[1].astVap[s1].aucBssid), ssid_type);
						strcpy(ssid_type, "other");
						++ap_num;
					}
				}
            }
            else
            {
				sprintf(temp + strlen(temp), IP_FMT"|"MAC_FMT"|%s|%s;", 
					IP_ARG(pstap->ip), MAC_ARG(pstap->mac), pstap->model, pstap->name);
				++ap_num;
            }
			
            if(strlen(g_ap_list)+strlen(temp) > (sizeof(g_ap_list)-1))
            {
                send_ap_list(ap_num, g_ap_list, sockfd, config, "aplist");
                ap_num = 0;
                memset(g_ap_list, 0, sizeof(g_ap_list));
            }
            strcat(g_ap_list, temp);
        }
    }
	AP_LIST_UNLOCK(); 
	if(ap_num > 0)
	{
		send_ap_list(ap_num, g_ap_list, sockfd, config, "aplist");
	}
	log_info(__LOG_DEBUG__, "");
	return 0;
}

int get_sta_wifi_cnnt(char *str_mac, char *ap_mac, char *ssid)
{
	WD_STA_T *pststa;
	struct hlist_node *p;
	struct hlist_head *head;
	unsigned int index;
	unsigned char mac[6];
	s_config *config = config_get_config();
	ap_mac[0] = '\0';
	ssid[0] = '\0';
	int find_sta = 0;
	
	if(strmac_to_mac(str_mac, mac) >= 0){
		index = mac_hashfn(mac, STA_HASH_SIZE);
		head = &(g_sta_HList[index]);

		STA_LIST_LOCK();
		hlist_for_each(p, head)
		{
			pststa = hlist_entry(p, WD_STA_T, mac_Hlist);
			if (!memcmp(pststa->mac, mac, 6)){
				snprintf(ap_mac, 32, "&apmac="MAC_FMT, MAC_ARG(pststa->ap_mac));
				snprintf(ssid, 64, "&ssid=%s", pststa->ssid);
				find_sta = 1;
				break;
			}
		}
		STA_LIST_UNLOCK();
	}
	else{
		return -1;
	}
	if (0 == find_sta){
		log_info(__LOG_DEBUG__, "find_sta = 0");
		snprintf(ap_mac, 32, "&apmac=%s", config->wan_mac);

		if(strmac_to_mac(config->wan_mac, mac) >= 0){
			WD_AP_T *pstap = ap_get_by_mac(mac);
			if(pstap != NULL){
				int r, s;
				for(r=0; r<WD_AP_MAX_RADIO_NUM; ++r){
					for(s=0; s<WD_RADIO_MAX_SSID_NUM; ++s){
						if(strlen(pstap->astRadio[r].astVap[s].acSsid) > 0){
							snprintf(ssid, 64, "&ssid=%s", pstap->astRadio[r].astVap[s].acSsid);
							free(pstap);
							return 0;
						}
					}
				}
				snprintf(ssid, 64, "&ssid=%s", "");
				free(pstap);
			}
			else
				snprintf(ssid, 64, "&ssid=%s", "");
		}
		else
			snprintf(ssid, 64, "&ssid=%s", "");
	}
	log_info(__LOG_DEBUG__, "%s", ap_mac);
	return 0;	
}


int show_reg_ap_list(int fd)
{
	WD_AP_T *pstap;
	struct hlist_node *p;
	struct hlist_head *head;
	int loop;
	char temp[128];
    char ap_list[4096];

    int total = 0;
    int ap_num = 0;
	int iRadio,iVap;
    WD_AP_RADIO_T *pstApradio;
    WD_AP_VAP_T *pstApvap;

    memset(ap_list, 0, sizeof(ap_list));

	AP_LIST_LOCK();
	for(loop = 0; loop < AP_HASH_SIZE; loop++)
	{ 
		head = &(g_ap_HList[loop]);
		hlist_for_each( p, head ){
			pstap = hlist_entry(p, WD_AP_T, mac_Hlist);
			sprintf(temp, "AP %04d: ip="IP_FMT" mac="MAC_FMT" model %s name %s RadioNum %d\n", total+1, 
				IP_ARG(pstap->ip), MAC_ARG(pstap->mac), pstap->model, pstap->name, pstap->uiRadioNum);
            strcat(ap_list, temp);

			for ( iRadio = 0 ; iRadio < pstap->uiRadioNum; iRadio++ )
            {
                pstApradio = &pstap->astRadio[iRadio];
                sprintf(temp, "\t=>wifi%d:",iRadio);
                strcat(ap_list, temp);
                sprintf(temp, "RadioId=%d RadioType=%d VapNum=%d\n",pstApradio->uiRadioId,pstApradio->uiRadioType,pstApradio->uiVapNum);
                strcat(ap_list, temp);
                for ( iVap = 0 ; iVap < pstApradio->uiVapNum; iVap++ )
                {
                    pstApvap = &pstApradio->astVap[iVap];
                    sprintf(temp, "\t\t==>vap%d:",iVap);
                    strcat(ap_list, temp);
                    sprintf(temp, "Bssid="MAC_FMT" Ssid=%s\n",MAC_ARG(pstApvap->aucBssid),pstApvap->acSsid);
                    strcat(ap_list, temp);
                }
                
            }

            ap_num++;
            total++;
            
            if(16 == ap_num){
                write(fd, ap_list, strlen(ap_list));
                ap_num = 0;
                memset(ap_list, 0, sizeof(ap_list));
            }
		}
    }
	AP_LIST_UNLOCK();

	if(ap_num > 0)
		write(fd, ap_list, strlen(ap_list));

	memset(ap_list, 0, sizeof(ap_list));
    sprintf(ap_list, "AP_nums = %d\n", total);
    write(fd, ap_list, strlen(ap_list));
	
	return 0;
}


int show_sta_cnnt_list(int fd)
{
	WD_STA_T *pststa;
	struct hlist_node *p;
	struct hlist_head *head;
	int loop;
	char temp[128];
    char sta_list[4096];

    int total = 0;
    int sta_num = 0;

    memset(sta_list, 0, sizeof(sta_list));

	STA_LIST_LOCK();
	for(loop = 0; loop < STA_HASH_SIZE; loop++)
	{ 
		head = &(g_sta_HList[loop]);
		hlist_for_each( p, head ){
			pststa = hlist_entry(p, WD_STA_T, mac_Hlist);
			sprintf(temp, "STA %04d: mac="MAC_FMT" apmac="MAC_FMT" ssid=%s\n", total+1, 
				MAC_ARG(pststa->mac), MAC_ARG(pststa->ap_mac), pststa->ssid);
            strcat(sta_list, temp);
            sta_num++;
            total++;
            
            if(64 == sta_num){
                write(fd, sta_list, strlen(sta_list));
                sta_num = 0;
                memset(sta_list, 0, sizeof(sta_list));
            }
		}
    }
	STA_LIST_UNLOCK();

	if(sta_num > 0)
		write(fd, sta_list, strlen(sta_list));

	memset(sta_list, 0, sizeof(sta_list));
    sprintf(sta_list, "Sta_nums = %d\n", total);
    write(fd, sta_list, strlen(sta_list));
	
	return 0;
}

int client_ap_status_init(void)
{
	int i;
	pthread_mutex_init(&g_sta_list_mutex, NULL);
	for(i = 0; i < STA_HASH_SIZE; ++i){	
		INIT_HLIST_HEAD(&g_sta_HList[i]);
	}

	pthread_mutex_init(&g_ap_list_mutex, NULL);
	for(i = 0; i < AP_HASH_SIZE; ++i){	
		INIT_HLIST_HEAD(&g_ap_HList[i]);
	}

	return 0;
}


