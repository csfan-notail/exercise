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

#include "httpd.h"

#include "common.h"
#include "conf.h"
#include "auth_server.h"
#include "client_list.h"
#include "http.h"
#include "log.h"
#include "util.h"
#include "firewall.h"
#include "redirect_portal.h"

#include <signal.h>
#include <sys/time.h>

#include "list.h"
#include "client_ap.h"
#include "client_rdl.h"

struct hlist_head g_sta_HList[STA_HASH_SIZE];
pthread_mutex_t g_sta_list_mutex;

struct hlist_head g_ap_HList[AP_HASH_SIZE];
struct hlist_head g_ap_send_Hlist[AP_HASH_SIZE];

pthread_mutex_t g_ap_list_mutex;

extern int g_ext_dbg_level;

static char *utobitmap(unsigned int value, char *str)
{
	int i, j;
    for(i = 0; value > 0; ++i, value /= 2)
        str[i] = value % 2 + '0';

	str[i] = '\0';

    for(j = i-1, i = 0; j - i >= 1; j--, i++){ /*将数字字符反序存放*/
        str[i] = str[i]^str[j];
        str[j] = str[i]^str[j];
        str[i] = str[i]^str[j];
    }
	return str;
}

int sta_add_by_mac(unsigned char *mac, unsigned char *ap_mac, char *ssid,int rssi)
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
		if (!memcmp(pststa->mac, mac, 6))
		{ /*终端存在*/
			if(ap_mac != NULL)
				tmp_ap_mac = ap_mac;
			else{
				memset(empty_ap_mac, 0, 6);
				tmp_ap_mac = &empty_ap_mac[0];
			}
			pststa->iRssi =rssi;
			if (memcmp(pststa->ap_mac, tmp_ap_mac, 6))
			{ /*需要更新ap mac*/
				log_info(LOG_INFO, "ap-sta:  ap mac change from "MAC_FMT"to "MAC_FMT" of sta("MAC_FMT")",
					MAC_ARG(pststa->ap_mac), MAC_ARG(tmp_ap_mac), MAC_ARG(mac));
				memcpy(pststa->ap_mac, tmp_ap_mac, 6);
				//STA_LIST_UNLOCK();
				//return 0;
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
					pststa->uiDisAssociaFlag = 0;
					pststa->ulDisAssociaTime = 0;
					STA_LIST_UNLOCK();
					return 0;
				}
			}

                    pststa->uiDisAssociaFlag = 0;
                    pststa->ulDisAssociaTime = 0;
                    
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
	pststa->iRssi =rssi;
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
    if(NULL == mac || NULL == apmac || NULL == ssid)
    {
        log_info(LOG_ERR,"param is NULL,return\n");
        return -1;
    }
	WD_STA_T *pststa;
	struct hlist_node *p, *n;
	struct hlist_head *head;
	unsigned int index;
	s_config *pConf = config_get_config();
	
	index = mac_hashfn(mac, STA_HASH_SIZE);
	head = &(g_sta_HList[index]);

	STA_LIST_LOCK();
	hlist_for_each_safe(p, n, head)
	{
		pststa = hlist_entry(p, WD_STA_T, mac_Hlist);
		if (!memcmp(pststa->mac, mac, 6)){
			if(!memcmp(pststa->ap_mac, apmac, 6)){
				if(!strcmp(pststa->ssid, ssid))
				{
				       if(1 == pConf->uiSpanSsidAuthSwith)
				       {
                                       pststa->uiDisAssociaFlag = 1;
					    pststa->ulDisAssociaTime = get_uptime();
					    log_info(LOG_DEBUG, "set disconnect STA "MAC_FMT"DisAssociaFlag=%d  ulDisAssociaTime=%ld\n", MAC_ARG(mac),\
					    pststa->uiDisAssociaFlag,pststa->ulDisAssociaTime);
				       }
				       else
				       {
					    hlist_del(&(pststa->mac_Hlist));
					    log_info(LOG_DEBUG, "del STA "MAC_FMT, MAC_ARG(mac));
					    free(pststa);
					}
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






/*根据终端mac获取终端信息*/
WD_STA_T *get_sta_by_mac(char *mac)
{
    WD_STA_T *pststa = NULL;
	WD_STA_T *r_pststa = NULL;
    struct hlist_node *p, *n;
    struct hlist_head *head;
    unsigned int index;
    unsigned char aucMac[6];
    if(strmac_to_mac(mac,aucMac) < 0)
    {
        log_info(LOG_DEBUG, "mac change failed ");
        return NULL;
    }
    
    index = mac_hashfn(aucMac, STA_HASH_SIZE);
    head = &(g_sta_HList[index]);

    STA_LIST_LOCK();
    hlist_for_each_safe(p, n, head)
    {
        pststa = hlist_entry(p, WD_STA_T, mac_Hlist);
        if (!memcmp(pststa->mac, aucMac, 6)){		
            log_info(LOG_DEBUG, "find STA "MAC_FMT, MAC_ARG(aucMac));
            r_pststa = (WD_STA_T *)malloc(sizeof(WD_STA_T));
			if(r_pststa != NULL)
				memcpy(r_pststa, pststa, sizeof(WD_STA_T));
			STA_LIST_UNLOCK();
			return r_pststa;
		}
    }
    STA_LIST_UNLOCK();
    return NULL;
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


/**
返回值:
   -1:error
   0:authinfo 更新成功，且认证的ssid与关联ssid一致
   1:authinfo 更新成功，但是认证ssid与关联ssid不一致
**/
WD_UPDATE_AUTH_INFO_E sta_update_auth_info(char *pStaStrMac,WD_STA_AUTH_INFO_T *pstNewAuthInfo, WD_STA_AUTH_INFO_T *pstOldAuthInfo)
{
    if(NULL == pStaStrMac ||NULL == pstNewAuthInfo || NULL ==pstOldAuthInfo)
    {
        log_info(LOG_ERR,"param  is NULL return\n");
        return WD_UPDATE_AUTH_INFO_ERR;
    }
    
    WD_STA_T *pststa = NULL;
    struct hlist_node *p;
    struct hlist_head *head;
    unsigned int index;
    unsigned char aucIntStaMac[6]={0};
    WD_UPDATE_AUTH_INFO_E  iRet = WD_UPDATE_AUTH_INFO_ERR;
    
    if(strmac_to_mac(pStaStrMac,aucIntStaMac) < 0)
    {
        log_info(LOG_ERR, "mac change failed ");
        return WD_UPDATE_AUTH_INFO_ERR;
    }

    index = mac_hashfn(aucIntStaMac, STA_HASH_SIZE);
    head = &(g_sta_HList[index]);

    STA_LIST_LOCK();
    hlist_for_each( p, head )
    {
        pststa = hlist_entry(p, WD_STA_T, mac_Hlist);
        if (!memcmp(pststa->mac, aucIntStaMac, 6))
        { /*终端存在*/

            /*获取之前的认证信息*/
            strncpy(pstOldAuthInfo->aucLastAuthSsid,pststa->stStaAuthInfo.aucLastAuthSsid,32);
            memcpy(pstOldAuthInfo->aucLastAuthApMac,pststa->stStaAuthInfo.aucLastAuthApMac,6);
            pstOldAuthInfo->uiLastAuthStatus = pststa->stStaAuthInfo.uiLastAuthStatus;
            strncpy(pstOldAuthInfo->aucToken,pststa->stStaAuthInfo.aucToken,64);
           
            log_info(LOG_DEBUG, "Old Authinfo:  sta_mac:%s  authStatus:%d  ap_mac:"MAC_FMT" ssid: %s token:%s\n", \
                pStaStrMac,pstOldAuthInfo->uiLastAuthStatus,MAC_ARG(pstOldAuthInfo->aucLastAuthApMac),\
                pstOldAuthInfo->aucLastAuthSsid,pstOldAuthInfo->aucToken);

            
           /*更新数据*/
            memset(&pststa->stStaAuthInfo,0,sizeof(WD_STA_AUTH_INFO_T));
            pststa->stStaAuthInfo.uiLastAuthStatus = pstNewAuthInfo->uiLastAuthStatus;
            strncpy(pststa->stStaAuthInfo.aucLastAuthSsid,pstNewAuthInfo->aucLastAuthSsid,32);
            memcpy(pststa->stStaAuthInfo.aucLastAuthApMac,pstNewAuthInfo->aucLastAuthApMac,6);
            strncpy(pststa->stStaAuthInfo.aucToken,pstNewAuthInfo->aucToken,64);
           
            log_info(LOG_DEBUG, "New Authinfo: sta_mac:%s  authStatus:%d   ap_mac: "MAC_FMT" ssid: %s token:%s\n", \
                pStaStrMac,pststa->stStaAuthInfo.uiLastAuthStatus,MAC_ARG(pststa->stStaAuthInfo.aucLastAuthApMac),\
                pststa->stStaAuthInfo.aucLastAuthSsid,pststa->stStaAuthInfo.aucToken);

           /*比较认证时ssid 与关联ssid是否一致*/
            if(0 == strcmp(pststa->stStaAuthInfo.aucLastAuthSsid,pststa->ssid))
            {
                iRet = WD_UPDATE_AUTH_INFO_SAME_SSID;
            }
            else
            {
                iRet = WD_UPDATE_AUTH_INFO_DIFF_SSID;
            }
            STA_LIST_UNLOCK();
	    
            return iRet;
        }
    }

    memset(pstOldAuthInfo,0,sizeof(WD_STA_AUTH_INFO_T));
    if(NULL == pststa)
    {/*sta 不存在*/
        if(1 == pstNewAuthInfo->uiLastAuthStatus)
        {/*添加一个关联终端*/
            pststa = (WD_STA_T*)malloc(sizeof(WD_STA_T));
            memset(pststa, 0, sizeof(WD_STA_T));
	     memcpy(pststa->mac, aucIntStaMac, 6);
	     strncpy(pststa->stStaAuthInfo.aucLastAuthSsid,pstNewAuthInfo->aucLastAuthSsid,32);
	     memcpy(pststa->stStaAuthInfo.aucLastAuthApMac,pstNewAuthInfo->aucLastAuthApMac,6);
	     pststa->stStaAuthInfo.uiLastAuthStatus = 1;
	     strncpy(pststa->stStaAuthInfo.aucToken,pstNewAuthInfo->aucToken,64);

           log_info(LOG_DEBUG, "Add Authinfo:  sta_mac:%s   of ap ("MAC_FMT") - ssid(%s) token:%s\n", \
                    pStaStrMac,MAC_ARG(pststa->stStaAuthInfo.aucLastAuthApMac),pststa->stStaAuthInfo.aucLastAuthSsid,\
                    pststa->stStaAuthInfo.aucToken);
	    
	    hlist_add_head(&(pststa->mac_Hlist), head);
	    STA_LIST_UNLOCK();
	    return WD_UPDATE_AUTH_INFO_SAME_SSID;
	}

    }

    log_info(LOG_ERR,"run wrong\n");
    STA_LIST_UNLOCK();
        
    return WD_UPDATE_AUTH_INFO_ERR;	
}
    

void sta_update_auth_info_clean_all()
{
        
    WD_STA_T *pststa = NULL;
    struct hlist_node *p;
    struct hlist_head *head;
    unsigned int index;

    for(index = 0; index < STA_HASH_SIZE;index++)
    {
        head = &(g_sta_HList[index]);
        STA_LIST_LOCK();
        hlist_for_each( p, head )
        {
            pststa = hlist_entry(p, WD_STA_T, mac_Hlist);
            if(pststa && (1 == pststa->stStaAuthInfo.uiLastAuthStatus))
            {
                memset(&pststa->stStaAuthInfo,0,sizeof(WD_STA_AUTH_INFO_T));
            }
        }
        STA_LIST_UNLOCK();
    }
}






int ap_add_by_mac(unsigned char *mac, unsigned char *ip, char *model, 
	       				  char *name, unsigned int uiRadioNum, WD_AP_RADIO_T *pstRadio)
{
	WD_AP_T *pstap;
	struct hlist_node *p;
	struct hlist_head *head;
	unsigned int index;
	int ap_update = 0;
	//unsigned int uiRadioType = 0;
	int i,j;
	
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
			if(strncmp(model, pstap->model, 16)){
				log_info(LOG_INFO, "ap-sta:  ap("MAC_FMT")'s type changes from %s to %s",
					MAC_ARG(pstap->mac), pstap->model, model);
				strncpy(pstap->model, model, 16);
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
	if(NULL == pstap)
		return -1;
	memset(pstap, 0, sizeof(WD_AP_T));
	memcpy(pstap->mac, mac, 6);
	memcpy(pstap->ip, ip, 16);
	strncpy(pstap->model, model, 16);
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

        /*2015-12-11 更新ssid信息时不删除关联用户*/
	//sta_del_by_ap_mac(mac);

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


int ap_radion_comp(WD_AP_RADIO_T *pstRegisterRadion,WD_AP_RADIO_T *pstSendRadio)
{
    if(NULL == pstRegisterRadion || NULL == pstSendRadio)
    {
        log_info(__LOG_ERROR__,"param is NULL,return\n");
        return -1;
    }
    //log_info(__LOG_DEBUG__,"#########begin ap_radion_comp####\n");
    int i,j,uiSendVapNum,uiAddVap=0;
    int iFindFlag = 0;
    WD_AP_VAP_T *pstRegisterVap = NULL,*pstSendVap = NULL;
    uiSendVapNum = pstSendRadio->uiVapNum;
    for(i = 0; i < pstRegisterRadion ->uiVapNum;i++)
    {
        pstRegisterVap = &pstRegisterRadion->astVap[i];
        iFindFlag = 0;
        log_info(__LOG_DEBUG__,"Register Vap_%d: ssid:%s Bssid:"MAC_FMT"  beforeSendNUm=%d ",i,pstRegisterVap->acSsid,\
        MAC_ARG(pstRegisterVap->aucBssid),uiSendVapNum);
        for(j =0;j < uiSendVapNum;j++)
        {
            pstSendVap = &pstSendRadio->astVap[j];
            //log_info(__LOG_DEBUG__,"    SendVapNum=%d j=%d,sendSsid:%s\n",uiSendVapNum,j,pstSendVap->acSsid);
            if(0 == memcmp(pstSendVap->aucBssid,pstRegisterVap->aucBssid,6))
            {/*bssid 相同*/
                iFindFlag = 1;
                if(0 == strcmp(pstSendVap->acSsid,pstRegisterVap->acSsid))
                {/*ssid相同*/
                    log_info(__LOG_DEBUG__,"  result=Find");
                    pstSendVap->eFlag = WD_AP_VAP_FLAG_FIND;
                }
                else
                {/*ssid 不相同*/
                    log_info(__LOG_DEBUG__," result=Diffrent");
                    pstSendVap->eFlag = WD_AP_VAP_FLAG_DEL;
                    if((uiSendVapNum + uiAddVap) < WD_RADIO_MAX_SSID_NUM*2 -1)
                    {
                        strcpy(pstSendRadio->astVap[uiSendVapNum+uiAddVap].acSsid,pstRegisterVap->acSsid);
                        //pstSendRadio ->uiVapNum ++;
                        memcpy(pstSendRadio->astVap[uiSendVapNum+uiAddVap].aucBssid,pstRegisterVap->aucBssid,6);
                        pstSendRadio->astVap[uiSendVapNum + uiAddVap].eFlag = WD_AP_VAP_FLAG_ADD;
                        uiAddVap ++;
                    }
                }
                break;
            }
        }

        if(0 == iFindFlag)
        {
            log_info(__LOG_DEBUG__,"   result=Add");
            /*not find add*/
            if((uiSendVapNum + uiAddVap + 1)< WD_RADIO_MAX_SSID_NUM*2 -1)
            {
                memcpy(pstSendRadio->astVap[uiSendVapNum + uiAddVap].aucBssid,pstRegisterVap->aucBssid,6);
                strcpy(pstSendRadio->astVap[uiSendVapNum + uiAddVap].acSsid,pstRegisterVap->acSsid);
                //pstSendRadio ->uiVapNum ++;
                pstSendRadio->astVap[uiSendVapNum + uiAddVap].eFlag = WD_AP_VAP_FLAG_ADD;
                uiAddVap ++;
            }
           // pstSendRadio ->uiVapNum += uiAddVap;
        }
        
    }

    pstSendRadio->uiVapNum += uiAddVap;
    log_info(__LOG_DEBUG__,"   afterSendNum=%d\n",pstSendRadio->uiVapNum);
    //log_info(__LOG_DEBUG__,"send vapNum=%d\n",pstSendRadio->uiVapNum);
    
    //log_info(__LOG_DEBUG__,"#########end ap_radion_comp####\n");
    return 0;
}


/*
2015-12-28 aplist 上报优化
flag:1==增量上报 0==完整上报

*/
int ap_send_list_cmp(WD_AP_T *pstRegisterApInfo,WD_AP_REPORT_FLAG_E eflag)
{
    if(NULL == pstRegisterApInfo)
    {
        log_info(__LOG_ERROR__,"param is NULL,return\n");
        return -1;
    }

    //log_info(__LOG_DEBUG__,"^^^^ begin ap_send_list_cmp^^^^^");
    
    WD_AP_T *pstSendApInfo = NULL;
    struct hlist_node *p, *n;
    struct hlist_head *head;
    unsigned int index,i,j;

    index = mac_hashfn(pstRegisterApInfo->mac, AP_HASH_SIZE);
    head = &(g_ap_send_Hlist[index]);
    hlist_for_each_safe(p, n, head)
    {
        pstSendApInfo = hlist_entry(p, WD_AP_T, mac_Hlist);
        if (0 == memcmp(pstSendApInfo->mac, pstRegisterApInfo->mac, 6))
        {
            if(WD_AP_REPORT_FLAG_ADD == eflag)
            {
                pstSendApInfo->uiFlag = 1;
                for(i =0;i < pstRegisterApInfo->uiRadioNum;i++)
                {
                    ap_radion_comp(&pstRegisterApInfo->astRadio[i],&pstSendApInfo->astRadio[i]);
                }
                //log_info(__LOG_DEBUG__,"^^^^ end ap_send_list_cmp^^^^^");
                return 0;
            }
            else
            {/*ALL*/
                memcpy(pstSendApInfo,pstRegisterApInfo,sizeof(WD_AP_T));
                pstSendApInfo->uiFlag = 1;
                for(i =0; i < pstSendApInfo->uiRadioNum;i++)
                {
                    for(j = 0; j <pstSendApInfo->astRadio[i].uiVapNum;j++)
                    {
                        pstSendApInfo->astRadio[i].astVap[j].eFlag = WD_AP_VAP_FLAG_ADD;
                    }
                }
                //log_info(__LOG_DEBUG__,"^^^^ end ap_send_list_cmp^^^^^");
                return 0;
            }
        }
    }
    
    /*add new*/
    pstSendApInfo = (WD_AP_T*)malloc(sizeof(WD_AP_T));
    if(NULL == pstSendApInfo)
    {
        log_info(__LOG_ERROR__,"malloc error,return\n");
        return -1;
    }
    memset(pstSendApInfo, 0, sizeof(WD_AP_T));
    memcpy(pstSendApInfo->mac, pstRegisterApInfo->mac, 6);
    memcpy(pstSendApInfo->ip, pstRegisterApInfo->ip, 16);
    strncpy(pstSendApInfo->model, pstRegisterApInfo->model, 16);
    strncpy(pstSendApInfo->name, pstRegisterApInfo->name, 64);
    pstSendApInfo->uiFlag = 1;
    pstSendApInfo->uiRadioNum = pstRegisterApInfo->uiRadioNum;
    for (i = 0; i < pstRegisterApInfo->uiRadioNum; i++)
    {
        pstSendApInfo->astRadio[i].uiRadioId = pstRegisterApInfo->astRadio[i].uiRadioId;
        pstSendApInfo->astRadio[i].uiRadioType = pstRegisterApInfo->astRadio[i].uiRadioType;
        pstSendApInfo->astRadio[i].uiVapNum = pstRegisterApInfo->astRadio[i].uiVapNum;
        for (j = 0; j < pstRegisterApInfo->astRadio[i].uiVapNum; j++)
        {
            memcpy(pstSendApInfo->astRadio[i].astVap[j].aucBssid,pstRegisterApInfo->astRadio[i].astVap[j].aucBssid, 6);
            strcpy(pstSendApInfo->astRadio[i].astVap[j].acSsid,pstRegisterApInfo->astRadio[i].astVap[j].acSsid);
            pstSendApInfo->astRadio[i].astVap[j].eFlag = WD_AP_VAP_FLAG_ADD;
        }
    }
    hlist_add_head(&(pstSendApInfo->mac_Hlist), head);



    log_info(__LOG_DEBUG__,"add apinfo:ip:"IP_FMT" mac:"MAC_FMT" model:%s name:%s uiFlag=%d\n",\
        IP_ARG(pstSendApInfo->ip),MAC_ARG(pstSendApInfo->mac),pstSendApInfo->model,pstSendApInfo->name,\
        pstSendApInfo->uiFlag);
    #if 0
    for(i= 0;i < pstSendApInfo->uiRadioNum;i++)
    {
        log_info(__LOG_DEBUG__,"\t\tRadionId=%d   RadioType=%d  VapNum=%d\n",\
        pstSendApInfo->astRadio[i].uiRadioId,pstSendApInfo->astRadio[i].uiRadioType,\
        pstSendApInfo->astRadio[i].uiVapNum);
        for(j=0;j<pstSendApInfo->astRadio[i].uiVapNum;j++)
        {
            log_info(__LOG_DEBUG__,"\t\t\tssid:%s  Bssid:"MAC_FMT" flag=%d",\
            pstSendApInfo->astRadio[i].astVap[j].acSsid,MAC_ARG(pstSendApInfo->astRadio[i].astVap[j].aucBssid),\
            pstSendApInfo->astRadio[i].astVap[j].eFlag);
        }
    }
    #endif
    
    //log_info(__LOG_DEBUG__,"^^^^ end ap_send_list_cmp^^^^^");
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
			if(r_pstap != NULL)
				memcpy(r_pstap, pstap, sizeof(WD_AP_T));
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

#if 0
char g_ap_list[7168];
int ap_send_list_report(void *func, void *config, int *sockfd)
{
	int i;
	struct hlist_node *p;
	WD_AP_T *pstap;
	int ap_num;
	char temp[4096];
	//int len;
	char ssid_type[16];
	char *pUrlDevNameEncode =NULL,*pUrlSsidEncode = NULL;

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
            //len = strlen(pstap->name);
            /*begin:设置名称支持空格2015-8-20*/
            #if 0
            for(j = 0; j < len; ++j)
            { /*名称为空格则改为-*/
                if(' ' == pstap->name[j])
                {
                    pstap->name[j] = '-';
                }
            }
	     #endif
	     /*end:设置名称支持空格2015-8-20*/
            if (1 == pstap->uiRadioNum)
            {
            	int s;
				strcpy(ssid_type, "wxwifi");

				/*1个radio，只考虑2.4G*/
				for(s=0; s<pstap->astRadio[0].uiVapNum; ++s){
					if(strlen(pstap->astRadio[0].astVap[s].acSsid) <= 0)
						continue;
					
					/*首个SSID用于微信连 ,其余SSID仅上报*/
					/*url code*/
					pUrlSsidEncode = http_url_standard_encode(pstap->astRadio[0].astVap[s].acSsid, strlen(pstap->astRadio[0].astVap[s].acSsid));
					pUrlDevNameEncode = http_url_standard_encode(pstap->name,strlen(pstap->name));
					
					sprintf(temp + strlen(temp), IP_FMT"|"MAC_FMT"|%s|%s|%s|"MAC_FMT"|%s;", 
						IP_ARG(pstap->ip), MAC_ARG(pstap->mac), pstap->model, pUrlDevNameEncode, 
						pUrlSsidEncode, 
						MAC_ARG(pstap->astRadio[0].astVap[s].aucBssid), ssid_type);
					strcpy(ssid_type, "other");
					++ap_num;
					URL_STANDAR_CODE_FREE(pUrlSsidEncode);
					URL_STANDAR_CODE_FREE(pUrlDevNameEncode);
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
							if(strlen(pstap->astRadio[0].astVap[s0].acSsid) > 0)
							{
							       pUrlDevNameEncode = http_url_standard_encode(pstap->name, strlen(pstap->name));
							       pUrlSsidEncode = http_url_standard_encode(pstap->astRadio[0].astVap[s0].acSsid,strlen(pstap->astRadio[0].astVap[s0].acSsid));
								sprintf(temp + strlen(temp), IP_FMT"|"MAC_FMT"|%s|%s|%s|"MAC_FMT","MAC_FMT"|%s;", 
									IP_ARG(pstap->ip), MAC_ARG(pstap->mac), pstap->model, pUrlDevNameEncode, 
									pUrlSsidEncode, 
									MAC_ARG(pstap->astRadio[0].astVap[s0].aucBssid), 
									MAC_ARG(pstap->astRadio[1].astVap[s1].aucBssid), ssid_type);
							        URL_STANDAR_CODE_FREE(pUrlDevNameEncode);
							        URL_STANDAR_CODE_FREE(pUrlSsidEncode);
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
							if(strlen(pstap->astRadio[0].astVap[s0].acSsid) > 0)
							{
							       pUrlDevNameEncode = http_url_standard_encode(pstap->name, strlen(pstap->name));
							       pUrlSsidEncode = http_url_standard_encode(pstap->astRadio[0].astVap[s0].acSsid, \
							       strlen(pstap->astRadio[0].astVap[s0].acSsid));
								sprintf(temp + strlen(temp), IP_FMT"|"MAC_FMT"|%s|%s|%s|"MAC_FMT","MAC_FMT"|%s;", 
									IP_ARG(pstap->ip), MAC_ARG(pstap->mac), pstap->model, pUrlDevNameEncode, 
									pUrlSsidEncode, 
									MAC_ARG(pstap->astRadio[0].astVap[s0].aucBssid), 
									MAC_ARG(pstap->astRadio[1].astVap[s1].aucBssid), ssid_type);
								URL_STANDAR_CODE_FREE(pUrlDevNameEncode);
							        URL_STANDAR_CODE_FREE(pUrlSsidEncode);
								strcpy(ssid_type, "other");
								++ap_num;
								break;
							}		
						}
					}

					if(s1 >= pstap->astRadio[1].uiVapNum){ /*单个2.4G SSID*/
					      pUrlDevNameEncode = http_url_standard_encode(pstap->name, strlen(pstap->name));
					      pUrlSsidEncode = http_url_standard_encode(pstap->astRadio[0].astVap[s0].acSsid, strlen(pstap->astRadio[0].astVap[s0].acSsid));
						sprintf(temp + strlen(temp), IP_FMT"|"MAC_FMT"|%s|%s|%s|"MAC_FMT"|%s;", 
							IP_ARG(pstap->ip), MAC_ARG(pstap->mac), pstap->model, pUrlDevNameEncode, 
							pUrlSsidEncode, 
							MAC_ARG(pstap->astRadio[0].astVap[s0].aucBssid), ssid_type);
							URL_STANDAR_CODE_FREE(pUrlDevNameEncode);
							URL_STANDAR_CODE_FREE(pUrlSsidEncode);
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
					         pUrlDevNameEncode = http_url_standard_encode(pstap->name, strlen(pstap->name));
					      pUrlSsidEncode = http_url_standard_encode(pstap->astRadio[1].astVap[s1].acSsid, strlen(pstap->astRadio[1].astVap[s1].acSsid));
						sprintf(temp + strlen(temp), IP_FMT"|"MAC_FMT"|%s|%s|%s|"MAC_FMT"|%s;", 
							IP_ARG(pstap->ip), MAC_ARG(pstap->mac), pstap->model, pUrlDevNameEncode, 
							pUrlSsidEncode, 
							MAC_ARG(pstap->astRadio[1].astVap[s1].aucBssid), ssid_type);
							URL_STANDAR_CODE_FREE(pUrlDevNameEncode);
							URL_STANDAR_CODE_FREE(pUrlSsidEncode);
						strcpy(ssid_type, "other");
						++ap_num;
					}
				}
            }
            else
            {
                            
                               pUrlDevNameEncode = http_url_standard_encode(pstap->name, strlen(pstap->name));
				sprintf(temp + strlen(temp), IP_FMT"|"MAC_FMT"|%s|%s;", 
					IP_ARG(pstap->ip), MAC_ARG(pstap->mac), pstap->model, pUrlDevNameEncode);
                            URL_STANDAR_CODE_FREE(pUrlDevNameEncode);
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
#endif


#if 1 /*new*/
char g_ap_list[7168];
int ap_send_list_report(void *func, void *config, int *sockfd)
{
    //log_info(__LOG_DEBUG__,"*************begin ap_send_list_report*****\n");
    //static int uiTimes =0;
    //uiTimes ++;
    //sys_log_set(WIFIOG_SYN_CLOUD_LOG,"****aplist_syn uitimes=%d",uiTimes);
    int i,j,iLoopi;
    WD_AP_VAP_T *pstVapInfo = NULL;
    struct hlist_node *p,*n;
    struct hlist_head *head;
    WD_AP_T *pstap;
    int ap_num = 0;
    char temp[4096]={0};
    
    //char ssid_type[16];
    char *pUrlDevNameEncode =NULL,*pUrlSsidEncode = NULL;
    static int uisTimers = 0;
    WD_AP_REPORT_FLAG_E eFlag;
    if(0 == uisTimers % 10 || 1 == uisTimers)
    {
        eFlag = WD_AP_REPORT_FLAG_ALL;
    }
    else
    {
        eFlag = WD_AP_REPORT_FLAG_ADD;
    }
    
    log_info(__LOG_DEBUG__,"ap_send_list_report Times=%d eFlag=%d\n",uisTimers,eFlag);
    uisTimers++;
    
    void (*send_ap_list)(int , char *, int *, void *, char *) = func;
	
    memset(g_ap_list, 0, sizeof(g_ap_list));
    ap_num = 0;
    AP_LIST_LOCK();
    for(i = 0; i < AP_HASH_SIZE; ++i)
    {
        hlist_for_each(p, &g_ap_HList[i])
        {
            pstap = hlist_entry(p, WD_AP_T, mac_Hlist);
            ap_send_list_cmp(pstap,eFlag);
        }
    }
    AP_LIST_UNLOCK();

    //log_info(__LOG_DEBUG__,"finish ap_send_list_cmp\n");

    for(i = 0;i < AP_HASH_SIZE;i++)
    {
        //hlist_for_each(p, &g_ap_send_Hlist[i]);
        head = &(g_ap_send_Hlist[i]);
        hlist_for_each_safe(p, n, head)
        {
            pstap = hlist_entry(p, WD_AP_T, mac_Hlist);
            //log_info(__LOG_DEBUG__,"ap_name:%s\n",pstap->name);
            pUrlDevNameEncode = http_url_standard_encode(pstap->name,strlen(pstap->name));
            if(1 == pstap->uiFlag)
            {/*ap 存在*/
                pstap->uiFlag = 0;
                for(j =0;j < pstap->uiRadioNum;j++)
                {
                    //log_info(__LOG_DEBUG__,"uiRadioIndex=%d \n",j);
                    for(iLoopi=0;iLoopi < pstap->astRadio[j].uiVapNum;iLoopi++)
                    {
                        log_info(__LOG_DEBUG__,"iLoopi=%d vapNUm=%d\n",iLoopi,pstap->astRadio[j].uiVapNum);
                        pstVapInfo= &pstap->astRadio[j].astVap[iLoopi];
                        pUrlSsidEncode = http_url_standard_encode(pstVapInfo->acSsid, strlen(pstVapInfo->acSsid));
                        if(WD_AP_VAP_FLAG_ADD == pstVapInfo->eFlag)
                        {/*ADD*/
                            if(WD_AP_REPORT_FLAG_ALL == eFlag)
                            {
                                sprintf(temp + strlen(temp), IP_FMT"|"MAC_FMT"|%s|%s|%s|"MAC_FMT"|%s;",
                                        IP_ARG(pstap->ip), MAC_ARG(pstap->mac), pstap->model, \
                                        (NULL != pUrlDevNameEncode ? pUrlDevNameEncode : ""), \
                                        (NULL != pUrlSsidEncode ? pUrlSsidEncode : ""),\
						MAC_ARG(pstVapInfo->aucBssid), "wxwifi");
                                log_info(__LOG_DEBUG__,"WD_AP_REPORT_FLAG_ALL:%s\n",temp);
                                //sys_log_set(WIFIDOG_AP_LIST_LOG,"ap_flag=1 ReportALL ap_num=%d ap_list:%s",ap_num,temp);
                                pstVapInfo->eFlag = WD_AP_VAP_FLAG_INIT;
                                ap_num ++;
                            }
                            else
                            {
                                sprintf(temp + strlen(temp), IP_FMT"|"MAC_FMT"|%s|%s|%s|"MAC_FMT"|%s|1;",
                                        IP_ARG(pstap->ip), MAC_ARG(pstap->mac), pstap->model, \
                                        (NULL != pUrlDevNameEncode ? pUrlDevNameEncode : ""), \
                                        (NULL != pUrlSsidEncode ? pUrlSsidEncode : ""),\
						MAC_ARG(pstVapInfo->aucBssid), "wxwifi");
                                log_info(__LOG_DEBUG__,"WD_AP_VAP_FLAG_ADD:%s\n",temp);
                                //sys_log_set(WIFIDOG_AP_LIST_LOG,"ap_flag=1 ReportADD ap_num=%d ap_list:%s",ap_num,temp);
                                pstVapInfo->eFlag = WD_AP_VAP_FLAG_INIT;
                                ap_num++;
                            }
                        }
                        else if(WD_AP_VAP_FLAG_DEL == pstVapInfo->eFlag)
                        {/*DEL*/
                            sprintf(temp + strlen(temp), IP_FMT"|"MAC_FMT"|%s|%s|%s|"MAC_FMT"|%s|0;",
                                        IP_ARG(pstap->ip), MAC_ARG(pstap->mac), pstap->model, \
                                        (NULL != pUrlDevNameEncode ? pUrlDevNameEncode : ""), \
                                        (NULL != pUrlSsidEncode ? pUrlSsidEncode : ""),\
						MAC_ARG(pstVapInfo->aucBssid), "wxwifi");
                            log_info(__LOG_DEBUG__,"WD_AP_VAP_FLAG_DEL:%s\n",temp);
                            //sys_log_set(WIFIDOG_AP_LIST_LOG,"ap_flag=1 ReportDEL ap_num=%d ap_list:%s",ap_num,temp);
                            ap_num++;
                            /*删除*/
                            if(iLoopi < pstap->astRadio[j].uiVapNum -1)
                            {
                                memcpy((char *)pstVapInfo,(char *)(pstVapInfo + 1),(pstap->astRadio[j].uiVapNum - iLoopi-1)*
sizeof(WD_AP_VAP_T));
                                pstap->astRadio[j].uiVapNum --;
                                //continue;
                                iLoopi--;
                            }
                            else if(iLoopi == pstap->astRadio[j].uiVapNum -1)
                            {
                                pstap->astRadio[j].uiVapNum--;
                            }
                        }
                        else if(WD_AP_VAP_FLAG_FIND == pstVapInfo->eFlag)
                        {
                            log_info(__LOG_DEBUG__,"WD_AP_VAP_FLAG_INIT");
                            pstVapInfo->eFlag = WD_AP_VAP_FLAG_INIT;
                        }
                        else
                        {/*INIT*/
                            sprintf(temp + strlen(temp), IP_FMT"|"MAC_FMT"|%s|%s|%s|"MAC_FMT"|%s|0;",
                                        IP_ARG(pstap->ip), MAC_ARG(pstap->mac), pstap->model, \
                                        (NULL != pUrlDevNameEncode ? pUrlDevNameEncode : ""), \
                                        (NULL != pUrlSsidEncode ? pUrlSsidEncode : ""),\
						MAC_ARG(pstVapInfo->aucBssid), "wxwifi");
				log_info(__LOG_DEBUG__,"WD_AP_VAP_FLAG_DEL:%s\n",temp);
                            //sys_log_set(WIFIDOG_AP_LIST_LOG,"ap_flag=1 VAP=INIT ReportDEL ap_num=%d ap_list:%s",ap_num,temp);
                            ap_num++;
				/*删除*/
                            if(iLoopi < pstap->astRadio[j].uiVapNum -1)
                            {
                                memcpy((char *)pstVapInfo,(char *)(pstVapInfo + 1),(pstap->astRadio[j].uiVapNum - iLoopi-1)*
sizeof(WD_AP_VAP_T));
                                pstap->astRadio[j].uiVapNum --;
                                iLoopi--;
                            }
                            else if(iLoopi == pstap->astRadio[j].uiVapNum -1)
                            {
                                pstap->astRadio[j].uiVapNum --;
                            }
				
                        }
                        
                        URL_STANDAR_CODE_FREE(pUrlSsidEncode);
                        if(strlen(g_ap_list)+strlen(temp) > (sizeof(g_ap_list)-1))
                        {
                            //sys_log_set(WIFIDOG_AP_LIST_LOG,"before send  ap_num=%d",ap_num);
                            send_ap_list(ap_num, g_ap_list, sockfd, config, "aplist");
                            ap_num = 0;
                            memset(g_ap_list, 0, sizeof(g_ap_list));
                        }
                        strcat(g_ap_list, temp);
                        memset(temp,0,4096);
                        //iLoopi ++;
                    }
                }
            }
            else
            {/*ap 不存在*/
                if(WD_AP_REPORT_FLAG_ADD == eFlag)
                {
                    for(j=0;j <pstap->uiRadioNum;j++)
                    {
                        for(iLoopi=0;iLoopi < pstap->astRadio[j].uiVapNum;iLoopi++)
                        {
                            pstVapInfo= &pstap->astRadio[j].astVap[iLoopi];
                            pUrlSsidEncode = http_url_standard_encode(pstVapInfo->acSsid, strlen(pstVapInfo->acSsid));
                            sprintf(temp + strlen(temp), IP_FMT"|"MAC_FMT"|%s|%s|%s|"MAC_FMT"|%s|0;",
                                        IP_ARG(pstap->ip), MAC_ARG(pstap->mac), pstap->model, \
                                        (NULL != pUrlDevNameEncode ? pUrlDevNameEncode : ""), \
                                        (NULL != pUrlSsidEncode ? pUrlSsidEncode : ""),\
						MAC_ARG(pstVapInfo->aucBssid), "wxwifi");
				log_info(__LOG_DEBUG__,"WD_AP_VAP_FLAG_DEL:%s\n",temp);
                            URL_STANDAR_CODE_FREE(pUrlSsidEncode);
                            ap_num ++;
                            if(strlen(g_ap_list)+strlen(temp) > (sizeof(g_ap_list)-1))
                            {
                                //sys_log_set(WIFIDOG_AP_LIST_LOG,"ap_flag=0 send  ap_num=%d ap_list:%s",ap_num,);
                                send_ap_list(ap_num, g_ap_list, sockfd, config, "aplist");
                                ap_num = 0;
                                memset(g_ap_list, 0, sizeof(g_ap_list));
                            }
                            strcat(g_ap_list, temp);
                            memset(temp,0,4096);
                        }
                    }
                }
                hlist_del(&(pstap->mac_Hlist));
                free(pstap);
                pstap = NULL;
            }
            URL_STANDAR_CODE_FREE(pUrlDevNameEncode);
        }
    }

    if(ap_num > 0)
    {
        //sys_log_set(WIFIDOG_AP_LIST_LOG,"before send  ap_num=%d",ap_num);
        send_ap_list(ap_num, g_ap_list, sockfd, config, "aplist");
        ap_num = 0;
        memset(g_ap_list, 0, sizeof(g_ap_list));
    }

    //log_info(__LOG_DEBUG__,"*************end ap_send_list_report*****\n");
    return 0;
}
#endif

int get_sta_default_apmac_ssid(const char *pStrStaMac,char *pStrApMac,char *pSsid)
{
    if(NULL ==pStrStaMac || NULL == pStrApMac || NULL == pSsid)
    {
        log_info(__LOG_ERROR__,"param is NULL, return\n");
        return -1;
    }
    WD_AP_T *pstap= NULL;
    s_config *config = config_get_config();
    unsigned char aucIntMac[6]={0};
    if(0 == strmac_to_mac(config->wan_mac,aucIntMac))
    {
        strncpy(pStrApMac,config->wan_mac,17);
        pstap = ap_get_by_mac(aucIntMac);
        if(pstap != NULL)
        {
        #ifdef GW_X86
            if(config->wxl_bssid != NULL)
            {
                snprintf(pSsid,32,"%s",config->wxl_ssid);
            }
            log_info(__LOG_DEBUG__,"get default wxl_ssid:%s\n",pSsid);
        #else
            int r, s;
            for(r=0; r<WD_AP_MAX_RADIO_NUM; ++r)
            {
                for(s=0; s<WD_RADIO_MAX_SSID_NUM; ++s)
                {
                    if(strlen(pstap->astRadio[r].astVap[s].acSsid) > 0)
                    {
                        snprintf(pSsid,32, "%s", pstap->astRadio[r].astVap[s].acSsid);
                        log_info(__LOG_DEBUG__,"get default ssid:%s\n",pSsid);
                        free(pstap);
                        pstap = NULL;
                        return 0;
                    }
                }
            }
        #endif
            if(pstap)
            {
                free(pstap);
                pstap = NULL;
            }
        }
    }
    return 0;
}


int get_sta_wifi_cnnt(char *str_mac, char *ap_mac, char *ssid,int uiSsidLen)
{
	WD_STA_T *pststa;
	struct hlist_node *p;
	struct hlist_head *head;
	unsigned int index;
	unsigned char mac[6];
	s_config *config = config_get_config();
	int find_sta = 0;
	char *safe_ssid = NULL;

	if(NULL == str_mac || NULL == ap_mac || NULL == ssid || 0 == uiSsidLen)
	{
	    return -1;
	}

        ap_mac[0] = '\0';
	ssid[0] = '\0';
	
	if(strmac_to_mac(str_mac, mac) >= 0){
		index = mac_hashfn(mac, STA_HASH_SIZE);
		head = &(g_sta_HList[index]);

		STA_LIST_LOCK();
		hlist_for_each(p, head)
		{
			pststa = hlist_entry(p, WD_STA_T, mac_Hlist);
			if (!memcmp(pststa->mac, mac, 6))
			{
				snprintf(ap_mac, 32, "&apmac="MAC_FMT, MAC_ARG(pststa->ap_mac));
				log_info(__LOG_DEBUG__, "before encode src ssid:%s\n", pststa->ssid);
				safe_ssid = http_url_standard_encode(pststa->ssid, strlen(pststa->ssid));
				if(NULL != safe_ssid)
				{
				    snprintf(ssid, uiSsidLen, "&ssid=%s", safe_ssid);
				    log_info(__LOG_DEBUG__, "after encode src ssid:%s", safe_ssid);
                                URL_STANDAR_CODE_FREE(safe_ssid);
				}
				else
				{
				    snprintf(ssid, uiSsidLen, "&ssid=%s", pststa->ssid);
				}
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
#ifdef GW_X86
				if(config->wxl_bssid != NULL)
					snprintf(ssid, uiSsidLen, "&ssid=%s", config->wxl_ssid);
				else
					snprintf(ssid, uiSsidLen, "&ssid=%s", "");
#else
				int r, s;
				for(r=0; r<WD_AP_MAX_RADIO_NUM; ++r){
					for(s=0; s<WD_RADIO_MAX_SSID_NUM; ++s){
						if(strlen(pstap->astRadio[r].astVap[s].acSsid) > 0){
							snprintf(ssid, uiSsidLen, "&ssid=%s", pstap->astRadio[r].astVap[s].acSsid);
							free(pstap);
							return 0;
						}
					}
				}
				snprintf(ssid, uiSsidLen, "&ssid=%s", "");
#endif				
				free(pstap);	
			}
			else
				snprintf(ssid, uiSsidLen, "&ssid=%s", "");
		}
		else
			snprintf(ssid, uiSsidLen, "&ssid=%s", "");
	}
	log_info(__LOG_DEBUG__, "ap_mac:%s", ap_mac);
	log_info(__LOG_DEBUG__, "ssid:%s", ssid);
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
	int find_sta = 0;
	
	if(strmac_to_mac(str_mac, mac) >= 0){
		index = mac_hashfn(mac, STA_HASH_SIZE);
		head = &(g_sta_HList[index]);

		STA_LIST_LOCK();
		hlist_for_each(p, head)
		{
			pststa = hlist_entry(p, WD_STA_T, mac_Hlist);
			if (!memcmp(pststa->mac, mac, 6)){
				snprintf(ap_mac, 32, MAC_FMT, MAC_ARG(pststa->ap_mac));
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
		snprintf(ap_mac, 32, "%s", config->wan_mac);
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
	char temp[2048];
    char ap_list[4096];

    int total = 0;
    //int ap_num = 0;
	int iRadio,iVap;
    WD_AP_RADIO_T *pstApradio;
    WD_AP_VAP_T *pstApvap;

    memset(ap_list, 0, sizeof(ap_list));

	AP_LIST_LOCK();
	for(loop = 0; loop < AP_HASH_SIZE; loop++)
	{ 
		head = &(g_ap_HList[loop]);
		hlist_for_each( p, head )
		{
		       memset(temp,0, sizeof(temp));
			pstap = hlist_entry(p, WD_AP_T, mac_Hlist);
			snprintf(temp, sizeof(temp), "AP %04d: ip="IP_FMT" mac="MAC_FMT" model %s name %s RadioNum %d\n", total+1, 
				IP_ARG(pstap->ip), MAC_ARG(pstap->mac), pstap->model, pstap->name, pstap->uiRadioNum);
           // strcat(ap_list, temp);

                    for ( iRadio = 0 ; iRadio < pstap->uiRadioNum; iRadio++ )
                    {
                        pstApradio = &pstap->astRadio[iRadio];
                        snprintf(temp + strlen(temp),sizeof(temp)-strlen(temp), "\t=>wifi%d:",iRadio);
                        // strcat(ap_list, temp);
                        snprintf(temp + strlen(temp),sizeof(temp)-strlen(temp) ,"RadioId=%d RadioType=%d VapNum=%d\n",pstApradio->uiRadioId,pstApradio->uiRadioType,pstApradio->uiVapNum);
                        //strcat(ap_list, temp);
                        for ( iVap = 0 ; iVap < pstApradio->uiVapNum; iVap++ )
                        {
                            pstApvap = &pstApradio->astVap[iVap];
                            snprintf(temp + strlen(temp),sizeof(temp)-strlen(temp), "\t\t==>vap%d:",iVap);
                            //strcat(ap_list, temp);
                            snprintf(temp +strlen(temp),sizeof(temp)-strlen(temp), "Bssid="MAC_FMT" Ssid=%s\n",MAC_ARG(pstApvap->aucBssid),pstApvap->acSsid);
                            //strcat(ap_list, temp);
                        }
                
                    }

                    //ap_num++;
                    total++;
                    if((sizeof(ap_list) - strlen(ap_list)) > strlen(temp))
                    {
                        strcat(ap_list, temp);
                    }
                    else
                    {
                        write(fd, ap_list, strlen(ap_list));
                        memset(ap_list,0,sizeof(ap_list));
                        strcat(ap_list, temp);
                    }
                    #if 0
			if((sizeof(ap_list)/sizeof(temp)) <= ap_num){
                write(fd, ap_list, strlen(ap_list));
                ap_num = 0;
                memset(ap_list, 0, sizeof(ap_list));
               
            } 
            #endif
		}
    }
	AP_LIST_UNLOCK();

	if(strlen(ap_list) > 0)
	{
		write(fd, ap_list, strlen(ap_list));
        }

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
	char temp[512];
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
			snprintf(temp, sizeof(temp), "STA %04d: mac="MAC_FMT" apmac="MAC_FMT\
			" ssid=%s Rssi=%d  LastAuthStatus=%d LastAuthSsid:%s  disConnect=%d  disconnectTime=%ld\n",\
			total+1,MAC_ARG(pststa->mac), MAC_ARG(pststa->ap_mac), pststa->ssid,pststa->iRssi,\
			pststa->stStaAuthInfo.uiLastAuthStatus,pststa->stStaAuthInfo.aucLastAuthSsid,pststa->uiDisAssociaFlag,pststa->ulDisAssociaTime);
            strcat(sta_list, temp);
            sta_num++;
            total++;
            
            if((sizeof(sta_list)/sizeof(temp)) <= sta_num){
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

int show_ios_sta_cnnt_list(int fd)
{
	WD_USER_IOS_ACCESS_T *pstuser = NULL;
	struct hlist_node *p;
	struct hlist_head *head;
	int index, loop;

	char temp[256];
    char sta_list[4096];
	char tbuf[64] = {0};
	char binbuf[64] = {0};

    int total = 0;
    int sta_num = 0;

	time_t   now;
	struct tm *timenow;

    memset(sta_list, 0, sizeof(sta_list));

	for(index = 0; index < FIRST_PAGE_TASK_NUM; ++index)
	{
		USER_IOS_ACCESS_LIST_LOCK(index);
		for(loop = 0; loop < USER_IOS_ACCESS_HASH_SIZE; loop++)
		{
			head = &(g_user_ios_access_mac_HList[index][loop]);

			hlist_for_each( p, head )
			{
				pstuser = hlist_entry(p, WD_USER_IOS_ACCESS_T, mac_Hlist);
				now = pstuser->time.tv_sec;
				timenow = localtime(&now);
				strftime(tbuf, sizeof(tbuf), "%Y-%m-%d %H:%M:%S", timenow);
				snprintf(temp, sizeof(temp), "IOS-STA %04d: mac="MAC_FMT" m_times=%d n_times=%d bitmap = 0x%x(%s) check=%s time=%s\n", total+1, 
					MAC_ARG(pstuser->mac), pstuser->m_times, pstuser->n_times, pstuser->bitmap, 
					utobitmap(pstuser->bitmap, binbuf),
					((pstuser->check == CHECK_INIT) ? "init" : 
					((pstuser->check == CHECK_MOZILLA) ? "mz" : 
					((pstuser->check == CHECK_PORTAL) ? "portal" : "other"))), tbuf);
	            strcat(sta_list, temp);
	            sta_num++;
	            total++;
	            
	            if((sizeof(sta_list)/sizeof(temp)) <= sta_num){
	                write(fd, sta_list, strlen(sta_list));
	                sta_num = 0;
	                memset(sta_list, 0, sizeof(sta_list));
	            }
			}
			
		}
		USER_IOS_ACCESS_LIST_UNLOCK(index);
	}

	if(sta_num > 0)
		write(fd, sta_list, strlen(sta_list));

	memset(sta_list, 0, sizeof(sta_list));
    sprintf(sta_list, "Sta_nums = %d\n", total);
    write(fd, sta_list, strlen(sta_list));
	
	return 0;
}

int show_sta_acl_list(int fd)
{
	WD_USER_ACL_T *pstuser = NULL;
	struct hlist_node *p;
	struct hlist_head *head;
	int loop;

	char temp[256];
    char sta_list[4096];
	char tbuf[64] = {0};

    int total = 0;
    int sta_num = 0;

	time_t   now;
	struct tm *timenow;

    memset(sta_list, 0, sizeof(sta_list));

	USER_ACL_LIST_LOCK();
	for(loop = 0; loop < USER_ACL_LIST_HASH_SIZE; loop++)
	{
		head = &(g_user_acl_HList[loop]);

		hlist_for_each( p, head )
		{
			pstuser = hlist_entry(p, WD_USER_ACL_T, mac_Hlist);
			now = pstuser->time.tv_sec;
			timenow = localtime(&now);
			strftime(tbuf, sizeof(tbuf), "%m-%d-%H:%M:%S", timenow);
			snprintf(temp, sizeof(temp), "USER-ACL %04d: type=%s mac="MAC_FMT" time=%s dura = %ld\n", total+1, 
				((USER_ACL_WHITE == pstuser->type) ? "white" : "black"), 
				MAC_ARG(pstuser->mac), tbuf, pstuser->duration);
            strcat(sta_list, temp);
            sta_num++;
            total++;
            
            if((sizeof(sta_list)/sizeof(temp)) <= sta_num){
                write(fd, sta_list, strlen(sta_list));
                sta_num = 0;
                memset(sta_list, 0, sizeof(sta_list));
            }
		}
		
	}
	USER_ACL_LIST_UNLOCK();

	if(sta_num > 0)
		write(fd, sta_list, strlen(sta_list));

	memset(sta_list, 0, sizeof(sta_list));
    sprintf(sta_list, "Sta_nums = %d\n", total);
    write(fd, sta_list, strlen(sta_list));
	
	return 0;
}

int show_sta_http_record(int fd)
{
	WD_USER_HTTP_RECORD_T *pstuser = NULL;
	struct hlist_node *p;
	struct hlist_head *head;
	int loop, id;

	char temp[256];
    char sta_list[4096];

    int total = 0;
    int sta_num = 0;

	char stage[32];

    memset(sta_list, 0, sizeof(sta_list));

	for(id = 0; id < FIRST_PAGE_TASK_NUM; ++id)
	{
		USER_HTTP_RECORD_LOCK(id);
		for(loop = 0; loop < USER_HTTP_RECORD_HASH_SIZE; loop++)
		{
			head = &(g_user_http_record_HList[id][loop]);

			hlist_for_each( p, head )
			{
				pstuser = hlist_entry(p, WD_USER_HTTP_RECORD_T, mac_Hlist);
				switch(pstuser->stage)
				{
				case USER_HTTP_RECORD_INIT:
					strncpy(stage, "init", sizeof(stage));
					break;
				case USER_HTTP_RECORD_IN_WHITE:
					strncpy(stage, "in-white", sizeof(stage));
					break;
				case USER_HTTP_RECORD_OUT_WHITE:
					strncpy(stage, "out-white", sizeof(stage));
					break;
				case USER_HTTP_RECORD_IN_BLACK:
					strncpy(stage, "in-black", sizeof(stage));
					break;
				case USER_HTTP_RECORD_OUT_BLACK:
					strncpy(stage, "out-black", sizeof(stage));
					break;
				default:
					strncpy(stage, "undefined", sizeof(stage));
					break;
				}
				snprintf(temp, sizeof(temp), "USER-HTTP-RECORD %04d(%d): mac="MAC_FMT" cnt=%d hit=%d stage=%s\n", total+1, 
					9061+id, MAC_ARG(pstuser->mac), pstuser->cnt, pstuser->hit, stage);
	            strcat(sta_list, temp);
	            sta_num++;
	            total++;
	            
	            if((sizeof(sta_list)/sizeof(temp)) <= sta_num){
	                write(fd, sta_list, strlen(sta_list));
	                sta_num = 0;
	                memset(sta_list, 0, sizeof(sta_list));
	            }
			}
			
		}
		USER_HTTP_RECORD_UNLOCK(id);
	}

	if(sta_num > 0)
		write(fd, sta_list, strlen(sta_list));

	memset(sta_list, 0, sizeof(sta_list));
    sprintf(sta_list, "Sta_nums = %d\n", total);
    write(fd, sta_list, strlen(sta_list));
	
	return 0;
}


int show_lan_info(int fd)
{
	s_config *config = config_get_config(); 
	char temp[256];
    char lan_info[4096];

    int total = 0;
    int info_num = 0;

	lan_info_t *pstLanInfo = NULL;
	memset(lan_info, 0, sizeof(lan_info));
	
	LANINFO_LIST_LOCK();

	list_for_each_entry(pstLanInfo, &(config->stLanInfo), stLanInfo)
	{
		snprintf(temp, sizeof(temp), "LAN-INFO %04d: name=%s ip="IP_FMT" mask="IP_FMT" dns1="IP_FMT" dns2="IP_FMT"\n", total+1, 
			pstLanInfo->name, IP_ARG(&pstLanInfo->uiIp), IP_ARG(&pstLanInfo->uiMask),
			IP_ARG(&pstLanInfo->uiDns), IP_ARG(&pstLanInfo->uiSubDns));
		strcat(lan_info, temp);
		info_num++;
		total++;
		
		if((sizeof(lan_info)/sizeof(temp)) <= info_num){
			write(fd, lan_info, strlen(lan_info));
			info_num = 0;
			memset(lan_info, 0, sizeof(lan_info));
		}		
	}

	LANINFO_LIST_UNLOCK();

	if(info_num > 0)
		write(fd, lan_info, strlen(lan_info));

	memset(lan_info, 0, sizeof(lan_info));
    sprintf(lan_info, "Nums = %d\n", total);
    write(fd, lan_info, strlen(lan_info));
	
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





