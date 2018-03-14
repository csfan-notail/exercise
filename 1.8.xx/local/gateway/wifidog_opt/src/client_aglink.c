/******************************************************************************

  Copyright (C), 2010-2015, GBCOM Co., Ltd.

 ******************************************************************************
  File Name     : client_aglink.c
  Version       : Initial Draft
  Author        : xiabing
  Created       : 2014/11/30
  Last Modified :
  Description   : client_aglink
  Function List :
  History       :
  1.Date        : 2014/11/30
    Author      : xiabing
    Modification: Created file

******************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <time.h>
#include "log.h"
#include "ag_module.h"       //aglink 头文件
#include "conf.h"

#include <config_common.h>
#include <nvram_config.h>


#include "client_aglink.h"
#include "httpd.h"
#include "client_list.h"
#include "http.h"

#include "client_ap.h"
#include "client_rdl.h"
#include "util.h"


#include "cloud_platform.h"

#include "gateway.h"
#include "cloud_platform.h"
#include "firewall.h"
#include "auth_server.h"

#ifdef OPENWRT
#include "pal_sys.h"
#include "tool.h"
#endif

/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/
extern int g_ext_dbg_level;
/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/
extern int wifidog_set_thread_name(EN_WIFIDOG_THREAD_INDEX enThreadIndex);
extern int nvram_get(char *value,char *name, int len);
extern int nvram_set(char *name ,char *value);
extern int nvram_commit(void);
extern int nvram_unset(char * name);

extern int set_sta_auth_status(char *ip, char *mac, int auth, char *method,char *token, int status);

/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/
AG_MODULE_IPC_REGISTER g_stWifidogAglink;

#define MODULE_NAME_STAM "stam"
#define MODULE_NAME_SNMPAGENT "snmpagent"
#define MODULE_NAME_EVENT "event"
#define MODULE_NAME_WIFIDOG "wifidog"
#define MODULE_NAME_LOCATOR_RUN "locator_run"
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


int wifidog_ap_report_handle(WIFIOG_AP_REGISTER_T *pstMsg)
{
	unsigned int uiRadioNum = 0;
	WD_AP_RADIO_T astRadio[WD_AP_MAX_RADIO_NUM];
	int i,j;
	char model[16];	
	
	if (NULL == pstMsg){
		log_info(__LOG_ERROR__, "NULL == pstMsg, fail");
        return -1;
    }
	memset(astRadio, 0, WD_AP_MAX_RADIO_NUM * sizeof(WD_AP_RADIO_T));
	uiRadioNum = ntohl(pstMsg->stApInfo.uiRadioNum);
	if (uiRadioNum >= WD_AP_MAX_RADIO_NUM){
		uiRadioNum = WD_AP_MAX_RADIO_NUM;
	}
	log_info(__LOG_DEBUG__, "ap mac : "MAC_FMT, MAC_ARG(pstMsg->aucMac));
	log_info(__LOG_DEBUG__, "ap ip : "IP_FMT, IP_ARG(pstMsg->aucIp));
	log_info(__LOG_DEBUG__, "modelId : %u", ntohl(pstMsg->uiModelId));
	log_info(__LOG_DEBUG__, "name : %s", pstMsg->acName);
	log_info(__LOG_DEBUG__, "RadioNum: %u", uiRadioNum);
	for (i = 0; i < uiRadioNum; i++){
		astRadio[i].uiRadioId = i+1;
		astRadio[i].uiRadioType = ntohl(pstMsg->stApInfo.astRadio[i].uiRadioType);
		astRadio[i].uiVapNum = ntohl(pstMsg->stApInfo.astRadio[i].uiVapNum);
		if (astRadio[i].uiVapNum >= WD_RADIO_MAX_SSID_NUM){
			astRadio[i].uiVapNum = WD_RADIO_MAX_SSID_NUM;
		}
		log_info(__LOG_DEBUG__, "Radio%d.id : %u", i, astRadio[i].uiRadioId);
		log_info(__LOG_DEBUG__, "Radio%d.type : %u", i, astRadio[i].uiRadioType);
		log_info(__LOG_DEBUG__, "Radio%d.vapnum : %u", i, astRadio[i].uiVapNum);
		for (j = 0; j < astRadio[i].uiVapNum; j++){
			memcpy(astRadio[i].astVap[j].aucBssid, pstMsg->stApInfo.astRadio[i].astVap[j].aucBssid, MAC_LEN);
			strcpy(astRadio[i].astVap[j].acSsid, pstMsg->stApInfo.astRadio[i].astVap[j].acSsid);
			log_info(__LOG_DEBUG__, "Radio%d.vap%d.ssid : %s", i, j, astRadio[i].astVap[j].acSsid);
			log_info(__LOG_DEBUG__, "Radio%d.vap%d.bssid: "MAC_FMT, i, j, MAC_ARG(astRadio[i].astVap[j].aucBssid));
		}
	}

	get_model_by_model_id(ntohl(pstMsg->uiModelId), model);
	
    return ap_add_by_mac(pstMsg->aucMac, pstMsg->aucIp, model, pstMsg->acName, uiRadioNum, astRadio);
}


int wifidog_ap_register_handle(WIFIOG_AP_REGISTER_T *pstMsg)
{
	unsigned int uiRadioNum = 0;
	WD_AP_RADIO_T astRadio[WD_AP_MAX_RADIO_NUM];
	int i,j;
	char model[16];	
	
	if (NULL == pstMsg){
		log_info(__LOG_ERROR__, "NULL == pstMsg, fail");
        return -1;
    }
	memset(astRadio, 0, WD_AP_MAX_RADIO_NUM * sizeof(WD_AP_RADIO_T));
	uiRadioNum = ntohl(pstMsg->stApInfo.uiRadioNum);
	if (uiRadioNum >= WD_AP_MAX_RADIO_NUM){
		uiRadioNum = WD_AP_MAX_RADIO_NUM;
	}
	log_info(__LOG_DEBUG__, "ap mac : "MAC_FMT, MAC_ARG(pstMsg->aucMac));
	log_info(__LOG_DEBUG__, "ap ip : "IP_FMT, IP_ARG(pstMsg->aucIp));
	log_info(__LOG_DEBUG__, "modelId : %u", ntohl(pstMsg->uiModelId));
	log_info(__LOG_DEBUG__, "name : %s", pstMsg->acName);
	log_info(__LOG_DEBUG__, "RadioNum : %u", uiRadioNum);
	for (i = 0; i < uiRadioNum; i++){
		astRadio[i].uiRadioId = i+1;
		astRadio[i].uiRadioType = ntohl(pstMsg->stApInfo.astRadio[i].uiRadioType);
		astRadio[i].uiVapNum = ntohl(pstMsg->stApInfo.astRadio[i].uiVapNum);
		if (astRadio[i].uiVapNum >= WD_RADIO_MAX_SSID_NUM){
			astRadio[i].uiVapNum = WD_RADIO_MAX_SSID_NUM;
		}
		log_info(__LOG_DEBUG__, "Radio%d.id : %u", i, astRadio[i].uiRadioId);
		log_info(__LOG_DEBUG__, "Radio%d.type : %u", i, astRadio[i].uiRadioType);
		log_info(__LOG_DEBUG__, "Radio%d.vapnum:%u", i, astRadio[i].uiVapNum);
		for (j = 0; j < astRadio[i].uiVapNum; j++){
			memcpy(astRadio[i].astVap[j].aucBssid, pstMsg->stApInfo.astRadio[i].astVap[j].aucBssid, MAC_LEN);
			strcpy(astRadio[i].astVap[j].acSsid, pstMsg->stApInfo.astRadio[i].astVap[j].acSsid);
			log_info(__LOG_DEBUG__, "Radio%d.vap%d.ssid : %s", i, j, astRadio[i].astVap[j].acSsid);
			log_info(__LOG_DEBUG__, "Radio%d.vap%d.bssid : "MAC_FMT, i, j, MAC_ARG(astRadio[i].astVap[j].aucBssid));
		}
	}

	get_model_by_model_id(ntohl(pstMsg->uiModelId), model);
	
    return ap_add_by_mac(pstMsg->aucMac, pstMsg->aucIp, model, pstMsg->acName, uiRadioNum, astRadio);
}







int wifidog_ap_unregister_handle(WIFIOG_AP_LOGOUT_T *pstMsg)
{
	if (NULL == pstMsg){
		log_info(__LOG_ERROR__, "NULL == pstMsg, fail");
        return -1;
    }
    log_info(__LOG_DEBUG__, "pstMsg->aucMac   :"MAC_FMT, MAC_ARG(pstMsg->aucMac));
    log_info(__LOG_DEBUG__, "pstMsg->aucIp    :"IP_FMT, IP_ARG(pstMsg->aucIp));
    return ap_del_by_mac(pstMsg->aucMac);
}



int wifidog_sta_ssid_auth_handle(char *pStrMac,char *pSsid)
{
    if(NULL == pStrMac || NULL == pSsid)
    {
        log_info(__LOG_ERROR__, "NULL == pstMsg, return");
        return -1;
    }
    log_info(__LOG_DEBUG__,"sta_mac:%s  ssid:%s\n",pStrMac,pSsid);
    WD_STA_T *pstSta = NULL;
    t_client *client = NULL;
    pstSta = get_sta_by_mac(pStrMac);
    if(NULL != pstSta)
    {
        if(1 ==  pstSta->stStaAuthInfo.uiLastAuthStatus)
        {
            LOCK_CLIENT_LIST();
            client = client_list_find_by_mac(pStrMac);
            if(NULL != client)
            {
                log_info(__LOG_DEBUG__,"client fw_state:%d LastAussid:%s  NowSsid:%s\n",client->fw_connection_state,\
                    pstSta->stStaAuthInfo.aucLastAuthSsid,pSsid);
                if((FW_MARK_SPAN_SSID == client->fw_connection_state) &&\
                        (0 == strcmp(pstSta->stStaAuthInfo.aucLastAuthSsid,pSsid)))
                {/**/
                    client_change_fw_connection_state(client,FW_MARK_KNOWN,NULL);
                    sys_log_set(WIFIDOG_USR_LOG_FILE, "nf-ALLOW: SpanSsid MAC:%s BackToAuthSsid:%s",pStrMac,pSsid);
                    nf_fw_access(FW_ACCESS_ALLOW, pStrMac,client);
                }
                else if((FW_MARK_KNOWN== client->fw_connection_state) && \
                        (strcmp(pstSta->stStaAuthInfo.aucLastAuthSsid,pSsid)))
                {
                    nf_fw_access(FW_ACCESS_DENY,client->mac,client);
                    client_change_fw_connection_state(client,FW_MARK_SPAN_SSID,NULL);
                    sys_log_set(WIFIDOG_USR_LOG_FILE, "nf-DENY SpanSsid MAC:%s  Change Ssid from:%s  to:%s",pStrMac,\
                        pstSta->stStaAuthInfo.aucLastAuthSsid,pSsid);
                }
            }
            UNLOCK_CLIENT_LIST();       
        }
        free(pstSta);
        pstSta = NULL;
    }
    
    return 0;
}


int event_wifidog_add_sta_handle(WIFIOG_EVENT_ADD_STA_T *pstMsg)
{
    int iLoop=0,iRet;
    WD_USER_IOS_ACCESS_T *pstuser=NULL;
	if (NULL == pstMsg){
		log_info(__LOG_ERROR__, "NULL == pstMsg, fail");
        return -1;
    }
    s_config *pConf = config_get_config();
    
    log_info(__LOG_DEBUG__, "pstMsg->aucApMac :"MAC_FMT, MAC_ARG(pstMsg->aucApMac));
    log_info(__LOG_DEBUG__, "pstMsg->aucStaMac:"MAC_FMT, MAC_ARG(pstMsg->aucStaMac));
    log_info(__LOG_DEBUG__, "pstMsg->aucIp    :"IP_FMT, IP_ARG(pstMsg->aucIp));
    log_info(__LOG_DEBUG__, "pstMsg->acSsid   : %s", pstMsg->acSsid);
    log_info(__LOG_DEBUG__, "pstMsg->acVapName: %s", pstMsg->acVapName);
    log_info(__LOG_DEBUG__, "pstMsg->rssi: %d", ntohl(pstMsg->rssi));
    for(iLoop = 0; iLoop < FIRST_PAGE_TASK_NUM; iLoop++)
    {
        pstuser = user_ios_access_get_by_mac(iLoop,pstMsg->aucStaMac,CHECK_USER_GET);
        if(NULL != pstuser)
        {
            pstuser->check = CHECK_INIT;
        }
    }
    #if 1
    /*begin:2015-11-30  不同ssid间切换是否需要认证*/
    char aucStrMac[32]={0};
    if(1 == pConf->uiSpanSsidAuthSwith)
    {/*需要认证*/
        sprintf(aucStrMac,MAC_FMT,MAC_ARG(pstMsg->aucStaMac));
        wifidog_sta_ssid_auth_handle(aucStrMac,pstMsg->acSsid);
    }
    /*end:2015-11-30  不同ssid间切换是否需要认证*/
    #endif
    
    iRet = sta_add_by_mac(pstMsg->aucStaMac, pstMsg->aucApMac, pstMsg->acSsid, ntohl(pstMsg->rssi));

    /*begin:2015-8-26 for 网间漫游*/
   // s_config *pConf = config_get_config();
    if(MACRO_SWITCH_UNPERCEPT == pConf->macro_hotspot)
    {
        http_send_check_to_server(pstMsg->aucIp,pstMsg->aucStaMac);
    }
    /*end:2015-8-26 for 网间漫游*/
 
    
    return iRet;
}

int event_wifidog_del_sta_handle(WIFIOG_EVENT_DEL_STA_T *pstMsg)
{
	if (NULL == pstMsg){
		log_info(__LOG_ERROR__, "NULL == pstMsg, fail");
        return -1;
    }
    log_info(__LOG_DEBUG__, "pstMsg->aucMac:"MAC_FMT, MAC_ARG(pstMsg->aucMac));
    log_info(__LOG_DEBUG__, "pstMsg->aucIp :"IP_FMT, IP_ARG(pstMsg->aucIp));
	log_info(__LOG_DEBUG__, "pstMsg->aucApMac:"MAC_FMT, MAC_ARG(pstMsg->aucApMac));
	log_info(__LOG_DEBUG__, "pstMsg->acSsid: %s", pstMsg->acSsid);

    return sta_del_by_mac(pstMsg->aucMac, pstMsg->aucApMac, pstMsg->acSsid);
}

int locator_wifidog_add_sta_handle(WIFIOG_LOCATOR_ADD_STA_T *pstMsg)
{
	if (NULL == pstMsg){
		if(g_ext_dbg_level > 0)
			log_info(__LOG_ERROR__, "NULL == pstMsg, fail");
        return -1;
    }
	if(g_ext_dbg_level > 0){
	    log_info(__LOG_DEBUG__, "pstMsg->aucApMac :"MAC_FMT, MAC_ARG(pstMsg->aucApMac));
	    log_info(__LOG_DEBUG__, "pstMsg->aucStaMac:"MAC_FMT, MAC_ARG(pstMsg->aucStaMac));
	    log_info(__LOG_DEBUG__, "pstMsg->acSsid   : %d", pstMsg->cRssi);
	}

    return sta_dct_add_by_mac(pstMsg->aucApMac, pstMsg->aucStaMac, pstMsg->cRssi);
}

int locator_wifidog_del_sta_handle(WIFIOG_LOCATOR_DEL_STA_T *pstMsg)
{
	if (NULL == pstMsg){
		if(g_ext_dbg_level > 0)
			log_info(__LOG_ERROR__, "NULL == pstMsg, fail");
        return -1;
    }
	if(g_ext_dbg_level > 0){
	    log_info(__LOG_DEBUG__, "pstMsg->aucApMac :"MAC_FMT, MAC_ARG(pstMsg->aucApMac));
	    log_info(__LOG_DEBUG__, "pstMsg->aucStaMac:"MAC_FMT, MAC_ARG(pstMsg->aucStaMac));
	}
	
    return sta_dct_del_by_mac(pstMsg->aucApMac, pstMsg->aucStaMac);
}


int wifidog_aglink_send_nak_to_ap()
{
	int iRet = -1;
	iRet = ag_module_ipc_client_send_msg(AGLINK_MSG_DEV_NAK_MSG,
		    							  AG_IPC_MSG_TYPE_OTHER_CPU,
		    							  &g_stWifidogAglink,
		    							  (unsigned char *)(NULL),
		    							  0);
	if (iRet < 0){
		log_info(__LOG_ERROR__, "AGLINK_MSG_DEV_NAK_MSG send fail");
    }else{
		log_info(__LOG_DEBUG__, "AGLINK_MSG_DEV_NAK_MSG send success");
	}
	return 0;
}



int snmp_set_sta_auth_handle(STAM_WIFIDOG_STA_T *pstMsg)
{
    char acStrMac[18] = {0};
    char *pStrIp = NULL;
    t_client *pstClient = NULL;
    if (NULL == pstMsg)
    {
        log_info(__LOG_ERROR__, "NULL == pstMsg, fail");
        return -1;
    }
    
    log_info(__LOG_DEBUG__, "pstMsg->aucMac:"MAC_FMT, MAC_ARG(pstMsg->aucMac));
    //log_info(__LOG_DEBUG__, "pstMsg->aucIp :"IP_FMT, IP_ARG(pstMsg->aucIp));
    //log_info(__LOG_DEBUG__, "pstMsg->iAuthState : %d", pstMsg->iAuthState);
   // log_info(__LOG_DEBUG__, "pstMsg->iAuthType : %d", pstMsg->iAuthType);
    
    
    sprintf(acStrMac, MAC_FMT,MAC_ARG(pstMsg->aucMac));
   // sys_log_set("/tmp/wd_ccs_auth.log","snmp_set_sta_auth_handle,sta_mac:%s AuthSta=%d",acStrMac,pstMsg->iAuthState);  
    if ( STAM_STA_AUTHED == pstMsg->iAuthState)
    {
        //http_auth_sta(acMac,pstMsg->iAuthType,pstMsg->acBblink_UserId);
        pStrIp = arp_get_ip(acStrMac);
        if(NULL == pStrIp)
        {
            log_info(__LOG_ERROR__, "sta:%s arp_get_ip is NULL return\n",acStrMac);
            return -1;
        }
        //sys_log_set("/tmp/wd_ccs_auth.log","arp_ip:%s  mac:%s",pStrIp,acStrMac);
        log_info(__LOG_DEBUG__,"arp_ip:%s  mac:%s\n",pStrIp,acStrMac);
        
        set_sta_auth_status(pStrIp,acStrMac,2,"snmp",NULL,FW_MARK_KNOWN);
        if(pStrIp)
        {
            free(pStrIp);
        }
        LOCK_CLIENT_LIST();
        pstClient = client_list_find_by_mac(acStrMac);
        UNLOCK_CLIENT_LIST();
        if(pstClient)
        {
            pthread_mutex_lock(&g_client_request_8060);
             http_add_sta_to_server(pstClient);
             pthread_mutex_unlock(&g_client_request_8060);
        }
    }
    else if ( STAM_STA_UNAUTHED == pstMsg->iAuthState )
    {
        LOCK_CLIENT_LIST();
        pstClient = client_list_find_by_mac(acStrMac);
        UNLOCK_CLIENT_LIST();
        if(pstClient)
        {
            //sys_log_set("/tmp/wd_ccs_auth.log","token=%s",pstClient->token);
            http_sta_onoff_to_server(REQUEST_TYPE_LOGOUT,acStrMac,NULL,NULL,pstClient->token,CLIENT_LOGOUT_CCS,NULL);
        }
        set_sta_auth_status(NULL,acStrMac,0,"snmp",NULL,0);
    }
    return 0;
}






void wifidog_aglink_msg_handle(int fd, short event, void *arg)
{
	char *acBuf;
	AG_LINK_MSG_HEAD_T *pstMsgHdr = NULL;
	void *pstMsgBody = NULL;
    int iRet = -1;

	acBuf = (char *)malloc(AG_MODULE_BUFFER_SIZE);
	if(NULL == acBuf)
		return;
	memset(acBuf, 0, AG_MODULE_BUFFER_SIZE);

    iRet = recvfrom(g_stWifidogAglink.uipcSocket, acBuf, AG_MODULE_BUFFER_SIZE, 0, NULL, NULL);
    if(iRet < 0){
		log_info(__LOG_ERROR__, "recvfrom fail. fd: %d", g_stWifidogAglink.uipcSocket);
		free(acBuf);
        return;
    }
    
    pstMsgHdr = (AG_LINK_MSG_HEAD_T *)acBuf;
    log_info(__LOG_DEBUG__, "RECV msg, msg id: %u, msg type: %u", ntohl(pstMsgHdr->usMsgId), ntohs(pstMsgHdr->usMsgType));

    switch(ntohl(pstMsgHdr->usMsgId)){
        case AG_MSG_IPC_REGISTER_ACK:
			g_stWifidogAglink.uiFlag = 1;
			/*wifidog 启动，向AP广播消息，让AP重新注册到wifidog*/
		    wifidog_aglink_send_nak_to_ap();
		    ag_module_ipc_client_send_msg(BC_MSG_AP_REGISTER_TO_GW_SUCCESS,AG_IPC_MSG_TYPE_SELF_CPU,&g_stWifidogAglink,(unsigned char *)(NULL),0);
			log_info(__LOG_DEBUG__, "RECV AG_MSG_IPC_REGISTER_ACK msg");
			break;
		case AG_MSG_IPC_UNREGISTER:
			log_info(__LOG_DEBUG__, "RECV AG_MSG_IPC_UNREGISTER msg");
			g_stWifidogAglink.uiFlag = 0;
			ag_module_ipc_reconnect_server(&g_stWifidogAglink);
			break;
        case WIFIDOG_MSG_AP_REGISTER:
			log_info(__LOG_DEBUG__, "RECV WIFIDOG_MSG_AP_REGISTER msg");
            pstMsgBody = (WIFIOG_AP_REGISTER_T *)(pstMsgHdr + 1);
            wifidog_ap_register_handle(pstMsgBody);					
            break;
            /*begin:2015-9-8 ap report info*/
		case WIFIDOG_MSG_AP_REPORT_APINFO:
			log_info(__LOG_DEBUG__, "WIFIDOG_MSG_AP_REPORT_APINFO msg");
			pstMsgBody = (WIFIOG_AP_REGISTER_T *)(pstMsgHdr + 1);
			wifidog_ap_report_handle(pstMsgBody);					
            break;
            /*end:2015-9-8 ap report info*/
		case WIFIDOG_MSG_AP_LOGOUT:
			log_info(__LOG_DEBUG__, "RECV WIFIDOG_MSG_AP_LOGOUT msg");
			pstMsgBody = (WIFIOG_AP_LOGOUT_T *)(pstMsgHdr + 1);
            wifidog_ap_unregister_handle(pstMsgBody);	
			break;
		case WIFIDOG_MSG_ADD_STA:
			log_info(__LOG_DEBUG__, "RECV WIFIDOG_MSG_ADD_STA msg");
            pstMsgBody = (WIFIOG_EVENT_ADD_STA_T *)(pstMsgHdr + 1);
            event_wifidog_add_sta_handle(pstMsgBody);
            break;
        case WIFIDOG_MSG_DEL_STA:
			log_info(__LOG_DEBUG__, "RECV WIFIDOG_MSG_DEL_STA msg");
            pstMsgBody = (WIFIOG_EVENT_DEL_STA_T *)(pstMsgHdr + 1);
            event_wifidog_del_sta_handle(pstMsgBody);
            break; 
		case WIFIDOG_MSG_LOCATOR_WIFIDOG_ADD_STA:
			log_info(__LOG_DEBUG__, "RECV WIFIDOG_MSG_LOCATOR_WIFIDOG_ADD_STA msg");
            pstMsgBody = (WIFIOG_LOCATOR_ADD_STA_T *)(pstMsgHdr + 1);
            locator_wifidog_add_sta_handle(pstMsgBody);
            break;
		case WIFIDOG_MSG_LOCATOR_WIFIDOG_DEL_STA:
			log_info(__LOG_DEBUG__, "RECV WIFIDOG_MSG_LOCATOR_WIFIDOG_DEL_STA msg");
            pstMsgBody = (WIFIOG_LOCATOR_DEL_STA_T *)(pstMsgHdr + 1);
            locator_wifidog_del_sta_handle(pstMsgBody);
            break;
            case WIFIDOG_MSG_SNMPAGENT_SET_STA_AUTH:
            {
                log_info(__LOG_DEBUG__, "RECV WIFIDOG_MSG_SNMPAGENT_SET_STA_AUTH msg");
                pstMsgBody = (STAM_WIFIDOG_STA_T *)(pstMsgHdr + 1);
                snmp_set_sta_auth_handle(pstMsgBody);
                break;
            }
        default:
            log_info(__LOG_ERROR__, "UNKOWN MSG, msg id: %u, msg type: %u", ntohs(pstMsgHdr->usMsgId), ntohs(pstMsgHdr->usMsgType));
            break;
    }

	free(acBuf);
    return;
}

/*begin: add@2015.01.27 for wechat connect wifi*/
//need redesign later
int wifidog_get_gw_dev_type(char *pcModel)
{
#ifdef GW_X86
	strcpy(pcModel, "cgw2000");
#else	

#ifdef OPENWRT
	*pcModel = pal_sysinfo_device_model_id();
#else
	FILE *pfData = NULL;
	char acModelId[32] = {0};
	pfData = popen("aptool getdevtype", "r");
	if(NULL == pfData){
		log_info(__LOG_ERROR__,"popen failed. ");
	}
	else{
		fread(acModelId, sizeof(acModelId)-1, 1, pfData);
		strcpy(pcModel, acModelId + sizeof("device type:") - 1);
		log_info(__LOG_DEBUG__, "pcModel: %s", pcModel);
		pclose(pfData);
	}
#endif
#endif
	return 0;
}

int wifidog_get_gw_name(char *pcName)
{
#ifdef OPENWRT
	FILE *pf = NULL;
	char *vl,*vl1;
	char buf[32] = {0};
	int len=32;
	pf = popen("uci show  system.@system[0].hostname", "r");
	if(NULL == pf){
		log_info(__LOG_ERROR__,"popen failed. ");
	}
	else{
		if(fgets(buf, sizeof(buf), pf) != NULL){
			vl1 = strstr(buf, "Entry not found");
			if(vl1){
				pclose(pf);
				return -1;
			}
			vl = strstr(buf, "=");
			if(vl != NULL){
					strncpy(pcName, vl+1, len);
					pclose(pf);
					return strlen(pcName);
				}
			}
			pclose(pf);
	}
#else
	char nvram_value[32];
	memset(nvram_value, 0, sizeof(nvram_value));
	nvram_get(nvram_value, "device_name", 32);
	strcpy(pcName, nvram_value);
#endif
	return 0;
}


#ifdef OPENWRT
int wifidog_bssid_get_by_name(char *name,unsigned char *bssid)
{
	char acCmd[256] = {0};
	FILE *pfData = NULL;
	char acTempBssid[64] = {0};
	
	snprintf(acCmd, 256, "ifconfig %s |grep HWaddr |awk '{print $5}'", name);

	sys_log_set(WIFIDOG_DEBUG_LOG_FILE,"%s %d acCmd %s   ",__FUNCTION__,__LINE__,acCmd);
	
	pfData = popen(acCmd, "r");
	if(NULL == pfData){
		log_info(__LOG_ERROR__,"popen failed. ");
	}
	else{
		fread(acTempBssid, sizeof(acTempBssid)-1, 1, pfData);				
		pclose(pfData);
	}

	strmac_to_mac(acTempBssid,bssid);
	return 1;
}
#endif
int wifidog_get_radio_vap_info(unsigned int *puiRadioNum, WD_AP_RADIO_T *pstRadio)
{
#ifdef OPENWRT
	int i;
	int vap_num = 0;
	char nvram_name[128];
	char nvram_value[128];
	char vap_name[32];
	char ssid_name[32];
	//char ssid_encode[32];
	int  radio_flag_24=0,radio_flag_5=0;
	//char *temstrp;
	//char acCmd[128];
	
	for(i=0;i<32;i++){
		memset(ssid_name,0,sizeof(ssid_name));
		sprintf(nvram_name,"wireless.@wifi-iface[%d].ssid",i);
		if(!nvram_get(ssid_name,nvram_name,128)){
			vap_num++;
			sprintf(nvram_name,"wireless.@wifi-iface[%d].device",i);
			memset(nvram_value,0,sizeof(nvram_value));
			if(!nvram_get(nvram_value,nvram_name,128)){
				if(!memcmp(nvram_value,"wifi0",strlen("wifi0"))){
					pstRadio[0].uiRadioId = 0;
					pstRadio[0].uiRadioType= ((0 == pstRadio[0].uiRadioId) ? 0 : 1);
					if(pstRadio[0].uiVapNum == 0)
						sprintf(vap_name,"ath0");
					else
						sprintf(vap_name,"ath0%d",pstRadio[0].uiVapNum);
					#if 0
					
					sprintf(nvram_name,"wireless.@wifi-iface[%d].encode",i);
					memset(ssid_encode,0,sizeof(ssid_encode));
					
					if(!nvram_get(ssid_encode,nvram_name,128) && !strncmp(ssid_encode,"GB2312",strlen("GB2312"))){
							sprintf(acCmd, "echo %s > /tmp/ssid_encode_gb2312", ssid_name);  
        						system(acCmd);
							sprintf(acCmd,"iconv -c -s -t GB2312 -f UTF-8 /tmp/ssid_encode_gb2312");
							temstrp = cmd_return_str(acCmd);		
							memcpy(pstRadio[0].astVap[pstRadio[0].uiVapNum].acSsid,temstrp,strlen(temstrp));
					}
					else
						#endif
						memcpy(pstRadio[0].astVap[pstRadio[0].uiVapNum].acSsid,ssid_name,32);
					
					wifidog_bssid_get_by_name(vap_name,pstRadio[0].astVap[pstRadio[0].uiVapNum].aucBssid);
					sys_log_set(WIFIDOG_DEBUG_LOG_FILE," pstRadio[0].uiVapNum %d  ssid %s  bssid "MAC_FMT"  ",pstRadio[0].uiVapNum,pstRadio[0].astVap[pstRadio[0].uiVapNum].acSsid,MAC_ARG(pstRadio[0].astVap[pstRadio[0].uiVapNum].aucBssid));
					pstRadio[0].uiVapNum++;
					radio_flag_24 =1;

				}
				else if(!memcmp(nvram_value,"wifi1",strlen("wifi1"))){
					pstRadio[1].uiRadioId = 1;
					pstRadio[1].uiRadioType= ((0 == pstRadio[0].uiRadioId) ? 0 : 1);
					if(pstRadio[1].uiVapNum == 0)
						sprintf(vap_name,"ath1");
					else
						sprintf(vap_name,"ath1%d",pstRadio[1].uiVapNum);
					#if 0

					sprintf(nvram_name,"wireless.@wifi-iface[%d].encode",i);
					memset(ssid_encode,0,sizeof(ssid_encode));
					
					if(!nvram_get(ssid_encode,nvram_name,128) && !strncmp(ssid_encode,"GB2312",strlen("GB2312"))){
							sprintf(acCmd, "echo %s > /tmp/ssid_encode_gb2312", ssid_name);  
        						system(acCmd);
							sprintf(acCmd,"iconv -c -s -t GB2312 -f UTF-8 /tmp/ssid_encode_gb2312");
							temstrp = cmd_return_str(acCmd);		
							memcpy(pstRadio[1].astVap[pstRadio[1].uiVapNum].acSsid,temstrp,strlen(temstrp));
					}
					else
						#endif
						
						memcpy(pstRadio[1].astVap[pstRadio[1].uiVapNum].acSsid,ssid_name,32);

					
					
					wifidog_bssid_get_by_name(vap_name,pstRadio[1].astVap[pstRadio[1].uiVapNum].aucBssid);
					
					sys_log_set(WIFIDOG_DEBUG_LOG_FILE," pstRadio[1].uiVapNum %d  ssid %s  bssid "MAC_FMT"  ",pstRadio[1].uiVapNum,pstRadio[1].astVap[pstRadio[1].uiVapNum].acSsid,MAC_ARG(pstRadio[1].astVap[pstRadio[1].uiVapNum].aucBssid));
					pstRadio[1].uiVapNum++;
					radio_flag_5 =1;
				}
				}
		}

		
	}
	if(radio_flag_24 && radio_flag_5 )
		*puiRadioNum = 2;
	else if(radio_flag_24 ^ radio_flag_5)
		*puiRadioNum = 1;
	else
		*puiRadioNum = 0;
#else

	char nvram_value[128];
#ifdef GW_X86
	s_config *config = config_get_config();
	if((config->wxl_ssid != NULL) && (config->wxl_bssid != NULL)){
		pstRadio[0].uiVapNum = 1;
		pstRadio[0].uiRadioType = 0;
		pstRadio[0].uiRadioId = 0;

		if(nvram_get(nvram_value, "wxl_ssid", 128) == 0){
			free(config->wxl_ssid);
			config->wxl_ssid = strdup(nvram_value);
		}
		strncpy(pstRadio[0].astVap[0].acSsid, config->wxl_ssid, 32);
		strmac_to_mac(config->wxl_bssid, pstRadio[0].astVap[0].aucBssid);
		*puiRadioNum = 1;
	}
	else
		*puiRadioNum = 0;
	sys_log_set(WIFIDOG_DEBUG_LOG_FILE, "get_radio_vap_info: wxl_ssid = %s wxl_bssid = %s", 
		config->wxl_ssid, config->wxl_bssid);
#else
	int i;
	char *pt1, *pt2;
	char nvram_type[64];
	char acTempBssid[64] = {0};
	char acCmd[256] = {0};
	int vap_num = 0;
	int radio = 0;
	int ssid[WD_AP_MAX_RADIO_NUM] = {0};

	FILE *pfData = NULL;
	int wlan = 0;
	int vap_id[32];

	char aucEncodeName[32]={0},aucEncodeValue[32]={0},*pEncode=NULL;

	memset(nvram_value, 0, sizeof(nvram_value));
	nvram_get(nvram_value, "wifi_numbers", 128);
	if (*puiRadioNum > WD_AP_MAX_RADIO_NUM)
		*puiRadioNum = WD_AP_MAX_RADIO_NUM;
	else 
		*puiRadioNum = atoi(nvram_value);

	nvram_get(nvram_value, "vap_list", 128);
	pt1 = pt2 = nvram_value;

/*获取vap列表*/
	while(*pt2 != '\0'){
		pt2 = strstr(pt1, ",");
		if(NULL == pt2)
			break;
		
		*pt2 = '\0';
		vap_id[vap_num++] = atoi(pt1);
		sys_log_set(WIFIDOG_DEBUG_LOG_FILE, "vap[%d]: %d", vap_num-1, vap_id[vap_num-1]);
		++pt2;
		pt1 = pt2;
	}
	if(NULL == pt2){
		vap_id[vap_num++] = atoi(pt1);
		sys_log_set(WIFIDOG_DEBUG_LOG_FILE, "vap[%d]: %d", vap_num-1, vap_id[vap_num-1]);
	}
	
/*获取vap信息*/	
	for(i=0; i<vap_num; ++i)
	{
		sprintf(nvram_type, "vap_%d_wifi_id", vap_id[i]);
		
		memset(nvram_value, 0, sizeof(nvram_value));
		nvram_get(nvram_value, nvram_type, 128);
		radio = atoi(nvram_value);
		if(radio >= WD_AP_MAX_RADIO_NUM)
			continue;

		memset(nvram_value, 0, sizeof(nvram_value));
		sprintf(nvram_type, "vap_%d_ssid_name", vap_id[i]);
		sprintf(aucEncodeName,"vap_%d_ssid_encode",vap_id[i]);

		if(ssid[radio] < WD_RADIO_MAX_SSID_NUM)
		{
			nvram_get(pstRadio[radio].astVap[ssid[radio]].acSsid, nvram_type, 64);
		       nvram_get(aucEncodeValue,aucEncodeName,32);
		       if(1 == atoi(aucEncodeValue))
		       {/*GB2312编码转换为 UTF8*/
		            sprintf(acCmd,"echo %s >/tmp/wd_ssid_encode",pstRadio[radio].astVap[ssid[radio]].acSsid);
                          system(acCmd);
                          sprintf(acCmd,"iconv -c -s -f GB2312 -t UTF-8 /tmp/wd_ssid_encode");
                          pEncode = util_cmd_return_str(acCmd);
                          memset(pstRadio[radio].astVap[ssid[radio]].acSsid,0,sizeof(pstRadio[radio].astVap[ssid[radio]].acSsid));
                          strcpy(pstRadio[radio].astVap[ssid[radio]].acSsid,pEncode);
		       }
			++pstRadio[radio].uiVapNum;
			pstRadio[radio].uiRadioId = radio;
			pstRadio[radio].uiRadioType = ((0 == radio) ? 0 : 1);

			sprintf(nvram_type, "vap_%d_wlan_id", vap_id[i]);
			memset(nvram_value, 0, sizeof(nvram_value));
			nvram_get(nvram_value, nvram_type, 128);
			
			wlan = atoi(nvram_value);

			snprintf(acCmd, 256, "ifconfig ap%d_%d |grep HWaddr |awk '{print $5}'", radio, wlan);
			pfData = popen(acCmd, "r");
			if(NULL == pfData){
				log_info(__LOG_ERROR__,"popen failed. ");
			}
			else{
				fread(acTempBssid, sizeof(acTempBssid)-1, 1, pfData);				
				pclose(pfData);
			}

			strmac_to_mac(acTempBssid, pstRadio[radio].astVap[ssid[radio]].aucBssid);

			sys_log_set(WIFIDOG_DEBUG_LOG_FILE, "get_radio_vap_info: Radio[%d].Vap[%d].acSsid = %s; Radio[%d].Vap[%d].Bssid = "MAC_FMT"; wlan = %d", 
				radio, ssid[radio], pstRadio[radio].astVap[ssid[radio]].acSsid,
				radio, ssid[radio], MAC_ARG(pstRadio[radio].astVap[ssid[radio]].aucBssid),
				wlan);		
			++ssid[radio];
		}
	}
#endif	
#endif
	return 0;
}


int wifidog_get_gw_info()
{
	WD_AP_T stAp;
	s_config *config = config_get_config();
	char acWanIp[64] = {0};
	char acName[16] = {0};
	unsigned int uiRadioNum = 0;
	WD_AP_RADIO_T astRadio[WD_AP_MAX_RADIO_NUM];

	memset(&stAp, 0, sizeof(stAp));
	memset(astRadio, 0, sizeof(WD_AP_RADIO_T) * WD_AP_MAX_RADIO_NUM);
	
	/*wan ip*/
	get_iface_ip(config->wan_interface, acWanIp, sizeof(acWanIp));
	/*device name*/
	wifidog_get_gw_name(acName);
	sys_log_set(WIFIDOG_DEBUG_LOG_FILE," %s %d  ",__FUNCTION__,__LINE__);
	/*radio & vap*/
	wifidog_get_radio_vap_info(&uiRadioNum, astRadio);
	sys_log_set(WIFIDOG_DEBUG_LOG_FILE," %s %d  ",__FUNCTION__,__LINE__);

	stAp.uiRadioNum = uiRadioNum;
	strmac_to_mac(config->wan_mac, stAp.mac);
	*((unsigned int *)(stAp.ip)) = (unsigned int)inet_addr(acWanIp);
	stAp.uiRadioNum = uiRadioNum;
	if (uiRadioNum){
		stAp.uiRadioNum = uiRadioNum;
		memcpy(stAp.astRadio, astRadio, sizeof(WD_AP_RADIO_T) * WD_AP_MAX_RADIO_NUM);
	}

	sys_log_set(WIFIDOG_DEBUG_LOG_FILE," %s %d  ",__FUNCTION__,__LINE__);
	
	strncpy(stAp.model, config->model, 16);
	strncpy(stAp.name, acName, 64);
	ap_add_by_mac(stAp.mac, stAp.ip, stAp.model, stAp.name, uiRadioNum, stAp.astRadio);

	sys_log_set(WIFIDOG_DEBUG_LOG_FILE," %s %d  ",__FUNCTION__,__LINE__);

	/*ap_info_print(&stAp); 存在段错误，暂注掉*/
	return 0;
}

int wifidog_update_gw_info()
{
	unsigned char mac[6];
	s_config *config = config_get_config();

	strmac_to_mac(config->wan_mac, mac);
	ap_del_by_mac(mac);	

	wifidog_get_gw_info();

	return 0;
}


void thread_aglink_client(void){
	unsigned int uiModuleId = 0;
	int iRet = 0;
	uiModuleId = (u32)AG_GET_MODULE_ID(WIFIDOG_MSG_BEGIN);
	
	wifidog_set_thread_name(WIFIDOG_THREAD_AGLINK);
	/*wifidog get gw info*/
	wifidog_get_gw_info();

    int uiLoopi = 0;
    for(uiLoopi = 0; uiLoopi <36;uiLoopi++)
    {
        if(check_online_interval(5))
        {
            log_info(__LOG_ERROR__,"Delay %d 5s\n",uiLoopi);
            break;
        }
        
    }
    sys_log_set(WIFIDOG_MAINLOOP_FILE,"wifidog_aglink thread start (Max-3*60s Now Delay-%d*5s)",uiLoopi);
    memset(&g_stWifidogAglink, 0, sizeof(g_stWifidogAglink));
    iRet = ag_module_ipc_module_init((unsigned char *)MODULE_NAME_WIFIDOG, uiModuleId, &g_stWifidogAglink, 
		(IPC_CLIENT_RECV_EVENT *)wifidog_aglink_msg_handle);  
	
    ag_module_ipc_power_on(&g_stWifidogAglink);
		
	return;
}



/*begin:2015-9-28 for audit
   如果pstClient为NULL 时,则根据pStaIp 和 pStaMac去查找client信息
*/
extern CLIENT_AUTH_TYPE_T gstClientAuthType[CLIENT_AUTH_MAX];
int wifidog_send_onoff_client_to_audit(fw_access_t eType,const char *pStaMac,t_client *pstClient)
{
    t_client *pstClientInfo =NULL;
    s_config *config;
    config = config_get_config();

    if(NULL == pstClient)
    {
        pstClientInfo = client_list_find_by_mac(pStaMac);
        if(NULL == pstClientInfo)
        {
            log_info(__LOG_DEBUG__, "can not fint Mac:%s in client list\n",pStaMac);
            sys_log_set(WIFIDOG_STAINFO_AUDIT, "can not fint  Mac:%s in client list\n",pStaMac);
            return -1;
        }
    }
    else
    {
        pstClientInfo = pstClient;
    }


    if(NULL == pstClientInfo)
    {
        log_info(__LOG_ERROR__, "param id NULL\n");
        return -1;
    }
    sys_log_set(WIFIDOG_STAINFO_AUDIT, "wifidog_send_onoff_client_to_audit StaIP:%s  StaMac:%s fw_type:(%d)%s",pstClientInfo->ip,pstClientInfo->mac,eType,(0 == eType)?"Allowd":((1==eType)?"Deny":"Other"));
    log_info(__LOG_DEBUG__," IP:%s  Mac:%s fw_type:(%d)%s",pstClientInfo->ip,pstClientInfo->mac,eType,(0 == eType)?"Allowd":((1==eType)?"Deny":"Other"));

    int now;
    WIFIDOG_AUDIT_STA_INFO_T pstStaInfo;
    WD_STA_T *pstApStaInfo = NULL;
    memset(&pstStaInfo,0,sizeof(WIFIDOG_AUDIT_STA_INFO_T));

    strmac_to_mac(pstClientInfo->mac,(unsigned char*)pstStaInfo.aucStaMac);/*终端mac*/
    strip_to_intip(pstClientInfo->ip, (char *)pstStaInfo.aucStaIp);/*staIp*/
    strcpy(pstStaInfo.aucStaId,pstClientInfo->stAuthInfo.aucUserCount);
    now = time(NULL);
    if(FW_ACCESS_ALLOW == eType)
    {
        pstStaInfo.iLogOnTime = htonl(now);
        pstClientInfo->login_time_t = now;

        /*获取接入的ap信息*/
        pstApStaInfo = get_sta_by_mac(pstClientInfo->mac);
        if(NULL == pstApStaInfo)
        {
            log_info(__LOG_ERROR__, "sta not find ap info.");
            if((NULL != config) && (NULL != config->wan_mac))
            {
                strmac_to_mac(config->wan_mac,pstClientInfo->stClientConnectInfo.aucApMac);
            }
            
        }
        else
        {
            memcpy(pstClientInfo->stClientConnectInfo.aucApMac, pstApStaInfo->ap_mac, 6);
            pstClientInfo->stClientConnectInfo.uiRssi = pstApStaInfo->iRssi;
        }
        //memcpy(pstStaInfo.aucApMac, pstApStaInfo->ap_mac, 6);/*apmac*/
       // pstStaInfo.iStaRssi = htonl(pstApStaInfo->iRssi);
    }
    else if(FW_ACCESS_DENY ==eType)
    {
        pstStaInfo.iLogOnTime = htonl(pstClientInfo->login_time_t);
        pstStaInfo.iLogOffTime = htonl(now);
    }
    else
    {
        return -1;
    }

    memcpy(pstStaInfo.aucApMac,pstClientInfo->stClientConnectInfo.aucApMac, 6);/*apmac*/
    pstStaInfo.iStaRssi = htonl(pstClientInfo->stClientConnectInfo.uiRssi);
     
    /*认证类型处理*/
    if(NULL != gstClientAuthType[pstClientInfo->stAuthInfo.uiAuthType].pAuthTypeCode)
    {
        strcpy(pstStaInfo.aucAuthType,gstClientAuthType[pstClientInfo->stAuthInfo.uiAuthType].pAuthTypeCode);
    }
    else
    {
        strcpy(pstStaInfo.aucAuthType,"1029999");
    }
    
    

    sys_log_set(WIFIDOG_STAINFO_AUDIT, "To Audit: Client_IP("IP_FMT") Client_Mac("MAC_FMT") Login_time(%d) Logout_time(%d) Rssi(%d) "
	    "Ap_Mac("MAC_FMT") ClientId(%s) authType:%s\n",
	    IP_ARG(pstStaInfo.aucStaIp), MAC_ARG(pstStaInfo.aucStaMac),
	    ntohl(pstStaInfo.iLogOnTime), ntohl(pstStaInfo.iLogOffTime), ntohl(pstStaInfo.iStaRssi),
	    MAC_ARG(pstStaInfo.aucApMac), pstStaInfo.aucStaId,pstStaInfo.aucAuthType);

    log_info(__LOG_DEBUG__, "To Snmp: Client_IP("IP_FMT") Client_Mac("MAC_FMT") Login_time(%d) Logout_time(%d) Rssi(%d) Ap_Mac("MAC_FMT")",
    	IP_ARG(pstStaInfo.aucStaId), MAC_ARG(pstStaInfo.aucStaMac),
    	ntohl(pstStaInfo.iLogOnTime), ntohl(pstStaInfo.iLogOffTime), ntohl(pstStaInfo.iStaRssi),
    	MAC_ARG(pstStaInfo.aucApMac));
    
    ag_module_ipc_client_send_msg(AUDIT_MSG_STA_ON_OFF, AG_IPC_MSG_TYPE_SELF_CPU,
    	&g_stWifidogAglink, (unsigned char *)&pstStaInfo, sizeof(WIFIDOG_AUDIT_STA_INFO_T));

	if(pstApStaInfo != NULL)
		free(pstApStaInfo);

    return 0;
}


/*end:2015-9-28 for audit*/



