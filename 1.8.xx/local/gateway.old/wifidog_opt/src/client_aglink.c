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
#include "ag_msg.h"
#include "conf.h"

#include <config_common.h>
#include <nvram_config.h>
#include "cJSON.h"
#include "httpd.h"
#include "http.h"
#include "util.h"

#include "client_ap.h"
#include "lib_pa.h"
#include "client_list.h"
#include "tool.h"
#include "client_aglink.h"

#include "pal_sys.h"

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



int wifidog_ap_register_handle(WIFIOG_AP_REGISTER_T *pstMsg)
{
	unsigned int uiRadioNum = 0;
	WD_AP_RADIO_T astRadio[WD_AP_MAX_RADIO_NUM];
	int i,j;
	//char model[16];	
	
	if (NULL == pstMsg){
		log_info(__LOG_ERROR__, "NULL == pstMsg, fail");
        return -1;
    }
	memset(astRadio, 0, WD_AP_MAX_RADIO_NUM * sizeof(WD_AP_RADIO_T));
	uiRadioNum = ntohl(pstMsg->stApInfo.uiRadioNum);
	if (uiRadioNum >= WD_AP_MAX_RADIO_NUM){
		uiRadioNum = WD_AP_MAX_RADIO_NUM;
	}
	log_info(__LOG_DEBUG__, "ap mac  :　"MAC_FMT, MAC_ARG(pstMsg->aucMac));
	log_info(__LOG_DEBUG__, "ap ip   :　"IP_FMT, IP_ARG(pstMsg->aucIp));
	log_info(__LOG_DEBUG__, "modelId :　%u", ntohl(pstMsg->uiModelId));
	log_info(__LOG_DEBUG__, "name    :　%s", pstMsg->acName);
	log_info(__LOG_DEBUG__, "RadioNum:　%u", uiRadioNum);
	for (i = 0; i < uiRadioNum; i++){
		astRadio[i].uiRadioId = i+1;
		astRadio[i].uiRadioType = ntohl(pstMsg->stApInfo.astRadio[i].uiRadioType);
		astRadio[i].uiVapNum = ntohl(pstMsg->stApInfo.astRadio[i].uiVapNum);
		if (astRadio[i].uiVapNum >= WD_RADIO_MAX_SSID_NUM){
			astRadio[i].uiVapNum = WD_RADIO_MAX_SSID_NUM;
		}
		log_info(__LOG_DEBUG__, "Radio%d.id          :　%u", i, astRadio[i].uiRadioId);
		log_info(__LOG_DEBUG__, "Radio%d.type  		 :　%u", i, astRadio[i].uiRadioType);
		log_info(__LOG_DEBUG__, "Radio%d.vapnum		 :　%u", i, astRadio[i].uiVapNum);
		for (j = 0; j < astRadio[i].uiVapNum; j++){
			memcpy(astRadio[i].astVap[j].aucBssid, pstMsg->stApInfo.astRadio[i].astVap[j].aucBssid, MAC_LEN);
			strcpy(astRadio[i].astVap[j].acSsid, pstMsg->stApInfo.astRadio[i].astVap[j].acSsid);
			log_info(__LOG_DEBUG__, "Radio%d.vap%d.ssid :　%s", i, j, astRadio[i].astVap[j].acSsid);
			log_info(__LOG_DEBUG__, "Radio%d.vap%d.bssid:　"MAC_FMT, i, j, MAC_ARG(astRadio[i].astVap[j].aucBssid));
		}
	}

	//get_model_by_model_id(ntohl(pstMsg->uiModelId), model);
	
    return ap_add_by_mac(pstMsg->aucMac, pstMsg->aucIp, ntohl(pstMsg->uiModelId), pstMsg->acName, uiRadioNum, astRadio);
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

int event_wifidog_add_sta_handle(WIFIOG_EVENT_ADD_STA_T *pstMsg)
{
	if (NULL == pstMsg){
		log_info(__LOG_ERROR__, "NULL == pstMsg, fail");
        return -1;
    }
    log_info(__LOG_DEBUG__, "pstMsg->aucApMac :"MAC_FMT, MAC_ARG(pstMsg->aucApMac));
    log_info(__LOG_DEBUG__, "pstMsg->aucStaMac:"MAC_FMT, MAC_ARG(pstMsg->aucStaMac));
    log_info(__LOG_DEBUG__, "pstMsg->aucIp    :"IP_FMT, IP_ARG(pstMsg->aucIp));
    log_info(__LOG_DEBUG__, "pstMsg->acSsid   : %s", pstMsg->acSsid);
    log_info(__LOG_DEBUG__, "pstMsg->acVapName: %s", pstMsg->acVapName);
    return sta_add_by_mac(pstMsg->aucStaMac, pstMsg->aucApMac, pstMsg->acSsid);
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

int snmp_set_sta_auth_handle(STAM_WIFIDOG_STA_T *pstMsg)
{
    char acMac[18];
	
	if (NULL == pstMsg){
		log_info(__LOG_ERROR__, "NULL == pstMsg, fail");
        return -1;
    }
    
    log_info(__LOG_DEBUG__, "pstMsg->aucMac:"MAC_FMT, MAC_ARG(pstMsg->aucMac));
    log_info(__LOG_DEBUG__, "pstMsg->aucIp :"IP_FMT, IP_ARG(pstMsg->aucIp));
    log_info(__LOG_DEBUG__, "pstMsg->iAuthState : %d", pstMsg->iAuthState);
    log_info(__LOG_DEBUG__, "pstMsg->iAuthType : %d", pstMsg->iAuthType);
    if ( NULL != pstMsg->acBblink_UserId )
    {
        log_info(__LOG_DEBUG__, "pstMsg->acBblink_UserId : %s", pstMsg->acBblink_UserId);
    }
    else
    {
        strcpy(pstMsg->acBblink_UserId,"0");
    }
    
    sprintf(acMac, MAC_FMT,pstMsg->aucMac[0], 
                        pstMsg->aucMac[1], 
                        pstMsg->aucMac[2], 
                        pstMsg->aucMac[3], 
                        pstMsg->aucMac[4], 
                        pstMsg->aucMac[5]
            );
            
    if ( STAM_STA_AUTHED == pstMsg->iAuthState )
    {
        http_auth_sta(acMac,pstMsg->iAuthType,pstMsg->acBblink_UserId);
    }
    else if ( STAM_STA_UNAUTHED == pstMsg->iAuthState )
    {
        http_close_sta(acMac);
    }
    return 0;
}

int snmp_set_auth_conf_handle(AUTH_CONF_T *pstMsg)
{
    char acBuf[64] = {0};
    int iflag = 0;
	if (NULL == pstMsg){
		log_info(__LOG_ERROR__, "NULL == pstMsg, fail");
        return -1;
    }
    
    log_info(__LOG_DEBUG__, "pstMsg->iSwitch : %d", pstMsg->iSwitch);
    log_info(__LOG_DEBUG__, "pstMsg->iWeb_auth : %d", pstMsg->iWeb_auth);
    log_info(__LOG_DEBUG__, "pstMsg->iOffline : %d", pstMsg->iOffline);
    log_info(__LOG_DEBUG__, "pstMsg->iReverify : %d", pstMsg->iReverify);

    memset(acBuf,0,sizeof(acBuf));
    sys_config_get(acBuf,NVRAM_WIFIDOG_SWTICH);
    if ( atoi(acBuf) == pstMsg->iSwitch )
    {
        log_info(__LOG_DEBUG__, "pstMsg->iSwitch change none %d : %d", atoi(acBuf),pstMsg->iSwitch);
    }
    else
    {
        if ( 0 == pstMsg->iSwitch )
        {
            sys_config_set(NVRAM_WIFIDOG_SWTICH, "0");
            log_info(__LOG_ERROR__, "SNMP KILL WIFIDOG! ");
            system("killall -9 wifidog");
        }
        else
        {
            sys_config_set(NVRAM_WIFIDOG_SWTICH, "1");
        }
    }

    memset(acBuf,0,sizeof(acBuf));
    sys_config_get(acBuf,NVRAM_BBLINK_WEBAUTH_SWTICH);
    if ( atoi(acBuf) == pstMsg->iWeb_auth )
    {
        log_info(__LOG_DEBUG__, "pstMsg->iWeb_auth change none %d : %d", atoi(acBuf),pstMsg->iWeb_auth);
    }
    else
    {
        if ( 0 == pstMsg->iWeb_auth )
        {
            sys_config_set(NVRAM_BBLINK_WEBAUTH_SWTICH, "0");
        }
        else
        {
            sys_config_set(NVRAM_BBLINK_WEBAUTH_SWTICH, "1");
        }
    }
    
    memset(acBuf,0,sizeof(acBuf));
    sys_config_get(acBuf,NVRAM_AUTH_IDLE_TIMEOUT);
    if ( atoi(acBuf) == pstMsg->iOffline )
    {
        log_info(__LOG_DEBUG__, "pstMsg->iOffline change none %d : %d", atoi(acBuf),pstMsg->iOffline);
    }
    else
    {
        sprintf(acBuf,"%d",pstMsg->iOffline);
        sys_config_set(NVRAM_AUTH_IDLE_TIMEOUT, acBuf);
        iflag = 1;
    }


    memset(acBuf,0,sizeof(acBuf));
    sys_config_get(acBuf,NVRAM_AUTH_FORCE_TIMEOUT);
    if ( atoi(acBuf) == pstMsg->iReverify )
    {
        log_info(__LOG_DEBUG__, "pstMsg->iReverify change none %d : %d", atoi(acBuf),pstMsg->iReverify);
    }
    else
    {
        sprintf(acBuf,"%d",pstMsg->iReverify);
        sys_config_set(NVRAM_AUTH_FORCE_TIMEOUT, acBuf);
        iflag = 1;
    }

    if ( 1 == iflag )
    {
        ap_sta_auth_write((int)pstMsg->iOffline,(int)pstMsg->iReverify);
    }
    return 0;
}


int snmp_get_auth_conf_handle()
{
    char acBuf[64] = {0};
	int iRet = -1;
	int iMsgLen;
	AUTH_CONF_T stAuthConf = {0};
	AUTH_CONF_T* pstAuthConf = &stAuthConf;

    memset(acBuf,0,sizeof(acBuf));
    sys_config_get(acBuf,NVRAM_WIFIDOG_SWTICH);
    pstAuthConf->iSwitch = atoi(acBuf);
    log_info(__LOG_DEBUG__, "pstMsg->iSwitch : %d", pstAuthConf->iSwitch);
    
    memset(acBuf,0,sizeof(acBuf));
    sys_config_get(acBuf,NVRAM_BBLINK_WEBAUTH_SWTICH);
    pstAuthConf->iWeb_auth = atoi(acBuf);
    log_info(__LOG_DEBUG__, "pstMsg->iWeb_auth : %d", pstAuthConf->iWeb_auth);
    
    memset(acBuf,0,sizeof(acBuf));
    sys_config_get(acBuf,NVRAM_AUTH_IDLE_TIMEOUT);
    pstAuthConf->iOffline = atoi(acBuf);
    log_info(__LOG_DEBUG__, "pstMsg->iOffline : %d", pstAuthConf->iOffline);
    
    memset(acBuf,0,sizeof(acBuf));
    sys_config_get(acBuf,NVRAM_AUTH_FORCE_TIMEOUT);
    pstAuthConf->iReverify = atoi(acBuf);
    log_info(__LOG_DEBUG__, "pstMsg->iReverify : %d", pstAuthConf->iReverify);
    
	iMsgLen = sizeof(AUTH_CONF_T);
	
	iRet = ag_module_ipc_client_send_msg(SNMP_MSG_GET_AUTH_CFG_ACK,
		    							  AG_IPC_MSG_TYPE_SELF_CPU,
		    							  &g_stWifidogAglink,
		    							  (unsigned char *)pstAuthConf,
		    							  iMsgLen);
	if (iRet < 0){
		log_info(__LOG_ERROR__, "SNMP_MSG_GET_AUTH_CFG_ACK send fail");
    }else{
		log_info(__LOG_DEBUG__, "SNMP_MSG_GET_AUTH_CFG_ACK send success");
	}

    return 0;
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

int bblink_mac_baw_update(WIFIDOG_SNMPAGENT_MAC_BAW_CONFIG_T *pstList)
{
	int loop = 0;
	int white_flush = 0, black_flush = 0;
	int white_num = 0, black_num = 0;
	char name[64], value[64];
	char white_conf_value[BAW_NUM_PER_CONFIG*18 + 1] = {0};
	char black_conf_value[BAW_NUM_PER_CONFIG*18 + 1] = {0};
	WIFIOG_SNMPAGENT_MAC_BAW_T *pt;

	pt = pstList->astMacBaw;
	
	for(loop = 0; loop < pstList->iNum; ++loop)
	{
		pt += loop;

		switch(pt->iType)
		{
		case BAW_TYPE_WHITE: /*添加白名单*/
			if(!white_flush){
				portal_flush_mac_white_list();
				white_flush = 1;
			}
			portal_add_mac_white_list(pt->aucMac);
			++white_num;

			snprintf(value, 64, MAC_FMT",", MAC_ARG(pt->aucMac));
			if((strlen(white_conf_value) + strlen(value) + 1) <= sizeof(white_conf_value))
				strcat(white_conf_value, value);
			else{
				white_conf_value[strlen(white_conf_value)] = '\0';

				snprintf(name, 64, BBLINK_WHITE_LIST"%d", white_num/BAW_NUM_PER_CONFIG);
				sys_config_set(name, white_conf_value);

				sprintf(white_conf_value, value);
			}
			break;
		case BAW_TYPE_BLACK: /*添加黑名单*/
			if(!black_flush){
				portal_flush_mac_black_list();
				black_flush = 1;
			}
			portal_add_mac_black_list(pt->aucMac);
			++black_num;
			
			snprintf(value, 64, MAC_FMT",", MAC_ARG(pt->aucMac));
			if((strlen(black_conf_value) + strlen(value) + 1) <= sizeof(black_conf_value))
				strcat(black_conf_value, value);
			else{
				black_conf_value[strlen(black_conf_value)] = '\0';

				snprintf(name, 64, BBLINK_BLACK_LIST"%d", black_num/BAW_NUM_PER_CONFIG);
				sys_config_set(name, black_conf_value);

				sprintf(black_conf_value, value);
			}
			break;
		default:
			break;
		}
	}

	if(strlen(white_conf_value) > 0){
		snprintf(name, 64, BBLINK_WHITE_LIST"%d", white_num/BAW_NUM_PER_CONFIG);
		sys_config_set(name, white_conf_value);
	}
	if(strlen(black_conf_value) > 0){
		snprintf(name, 64, BBLINK_BLACK_LIST"%d", black_num/BAW_NUM_PER_CONFIG);
		sys_config_set(name, black_conf_value);
	}

	if(white_flush && (white_num > 0)){
		snprintf(value, 64, "%d", white_num);
		sys_config_set(BBLINK_WHITE_LIST_NUM, value);
	}
	if(black_flush && (black_num > 0)){
		snprintf(value, 64, "%d", black_num);
		sys_config_set(BBLINK_BLACK_LIST_NUM, value);
	}
	
	return 0;
}


int snmp_set_add_baw_url(WIFIDOG_SNMPAGENT_URL_BAW_CONFIG_T *pstList)
{
	int loop = 0;
    unsigned int ulUrlNo= 0;
    unsigned int ulTotal= 0;
   	WIFIOG_SNMPAGENT_URL_BAW_T *pt;

	pt = pstList->astUrlBaw;
            log_info(__LOG_DEBUG__, "%p list num %d ",pstList,pstList->iNum);
	for(loop = 0; loop < pstList->iNum; ++loop)
	{
		switch(pt[loop].iType)
		{
    		case BAW_TYPE_WHITE: /*添加白名单*/
    			portal_add_url_free_auth_list((unsigned char *)pt[loop].aucUrl);
                tools_nvram_list_add(NVRAM_WD_WHITE_URL_LIST,NVRAM_WD_WHITE_URL_NUM,NVRAM_WD_WHITE_URL_HEAD,&ulTotal,&ulUrlNo,WD_URL_NUM_MAX,pt[loop].aucUrl);
    			break;
    		case BAW_TYPE_BLACK: /*添加黑名单*/
    			portal_add_url_black_list((unsigned char *)pt[loop].aucUrl);
                tools_nvram_list_add(NVRAM_WD_BLACK_URL_LIST,NVRAM_WD_BLACK_URL_NUM,NVRAM_WD_BLACK_URL_HEAD,&ulTotal,&ulUrlNo,WD_URL_NUM_MAX,pt[loop].aucUrl);
    			break;
    		default:
    			break;
		}
	}
    nvram_commit();
	return 0;
}


int snmp_set_del_baw_url(WIFIDOG_SNMPAGENT_URL_BAW_CONFIG_T *pstList)
{
	int loop = 0;
    unsigned int ulTotal= 0;
   	WIFIOG_SNMPAGENT_URL_BAW_T *pt;

	pt = pstList->astUrlBaw;
	
	for(loop = 0; loop < pstList->iNum; ++loop)
	{
		switch(pt[loop].iType)
		{
    		case BAW_TYPE_WHITE: /*添加白名单*/
    			portal_del_url_free_auth_list((unsigned char *)pt[loop].aucUrl);
                tools_nvram_list_del(NVRAM_WD_WHITE_URL_LIST,NVRAM_WD_WHITE_URL_NUM,NVRAM_WD_WHITE_URL_HEAD,&ulTotal,pt[loop].aucUrl);
    			break;
    		case BAW_TYPE_BLACK: /*添加黑名单*/
    			portal_del_url_black_list((unsigned char *)pt[loop].aucUrl);
                tools_nvram_list_del(NVRAM_WD_BLACK_URL_LIST,NVRAM_WD_BLACK_URL_NUM,NVRAM_WD_BLACK_URL_HEAD,&ulTotal,pt[loop].aucUrl);
    			break;
    		default:
    			break;
		}
	}
	return 0;
}


int snmp_set_add_baw_mac(WIFIDOG_SNMPAGENT_MAC_BAW_CONFIG_T *pstList)
{
	int loop = 0;
    unsigned int ulMacNo= 0;
    unsigned int ulTotal= 0;
	char value[64]={0};
   	WIFIOG_SNMPAGENT_MAC_BAW_T *pt;

	pt = pstList->astMacBaw;
	
	for(loop = 0; loop < pstList->iNum; ++loop)
	{
        sprintf(value,MAC_FMT,MAC_ARG(pt[loop].aucMac));
		switch(pt[loop].iType)
		{
    		case BAW_TYPE_WHITE: /*添加白名单*/
    			portal_add_mac_white_list(pt[loop].aucMac);
                tools_nvram_list_add(NVRAM_WD_WHITE_MAC_LIST,NVRAM_WD_WHITE_MAC_NUM,NVRAM_WD_WHITE_MAC_HEAD,&ulTotal,&ulMacNo,WD_MAC_NUM_MAX,value);
    			break;
    		case BAW_TYPE_BLACK: /*添加黑名单*/
    			portal_add_mac_black_list(pt[loop].aucMac);
                tools_nvram_list_add(NVRAM_WD_BLACK_MAC_LIST,NVRAM_WD_BLACK_MAC_NUM,NVRAM_WD_BLACK_MAC_HEAD,&ulTotal,&ulMacNo,WD_MAC_NUM_MAX,value);
    			break;
    		default:
    			break;
		}
	}
    nvram_commit();
	return 0;
}


int snmp_set_del_baw_mac(WIFIDOG_SNMPAGENT_MAC_BAW_CONFIG_T *pstList)
{
	int loop = 0;
    unsigned int ulTotal= 0;
	char value[64]={0};
   	WIFIOG_SNMPAGENT_MAC_BAW_T *pt;

	pt = pstList->astMacBaw;
	
	for(loop = 0; loop < pstList->iNum; ++loop)
	{
        sprintf(value,MAC_FMT,MAC_ARG(pt[loop].aucMac));
		switch(pt[loop].iType)
		{
    		case BAW_TYPE_WHITE: /*添加白名单*/
    			portal_del_mac_white_list(pt[loop].aucMac);
                tools_nvram_list_del(NVRAM_WD_WHITE_MAC_LIST,NVRAM_WD_WHITE_MAC_NUM,NVRAM_WD_WHITE_MAC_HEAD,&ulTotal,value);
    			break;
    		case BAW_TYPE_BLACK: /*添加黑名单*/
    			portal_del_mac_black_list(pt[loop].aucMac);
                tools_nvram_list_del(NVRAM_WD_BLACK_MAC_LIST,NVRAM_WD_BLACK_MAC_NUM,NVRAM_WD_BLACK_MAC_HEAD,&ulTotal,value);
    			break;
    		default:
    			break;
		}
	}
    nvram_commit();
	return 0;
}


int snmp_set_user_timeout(WIFIDOG_SNMPAGENT_USER_TIMEOUT *pstTimeout)
{
    s_config *config = NULL;
    char     acValue[32]={0};
    config = config_get_config();
    config->iAgeingSwitch=pstTimeout->iSwitch;
    config->clienttimeout=pstTimeout->iTimeOut*60;
    sprintf(acValue,"%d",config->iAgeingSwitch);
    sys_config_set(CLIENT_AGE_SWITCH,acValue);
    sprintf(acValue,"%d",config->clienttimeout);
    sys_config_set(CLIENT_TIME_OUT,acValue);
	return 0;
}
int snmp_set_normal_flowctl(WIFIDOG_SNMPAGENT_FLOW_CTL *pstFlowCtl)
{
    s_config *config = NULL;
    char     acValue[32]={0};
    
    config = config_get_config();
    config->bw_up=pstFlowCtl->iUpFlowMax;
    config->bw_down=pstFlowCtl->iDownFlowMax;

    if(config->bw_up !=0 || config->bw_down !=0)
    {
        config->qos_enable = 1;
        sprintf(acValue,"%u",config->qos_enable);
        sys_config_set(QOS_ENABLE,acValue);
    }
    sprintf(acValue,"%u",config->bw_up);
    sys_config_set(FLOW_CTL_UP,acValue);
    sprintf(acValue,"%u",config->bw_down);
    sys_config_set(FLOW_CTL_DOWN,acValue);
	return 0;
}
int snmp_set_white_flowctl(WIFIDOG_SNMPAGENT_FLOW_CTL *pstFlowCtl)
{
    s_config *config = NULL;
    char     acValue[32]={0};
    
    config = config_get_config();
    config->bw_white_up=pstFlowCtl->iUpFlowMax;
    config->bw_white_down=pstFlowCtl->iDownFlowMax;
    if(config->bw_white_up !=0 || config->bw_white_down !=0)
    {
        config->qos_enable = 1;
        sprintf(acValue,"%u",config->qos_enable);
        sys_config_set(QOS_ENABLE,acValue);
    }
    sprintf(acValue,"%u",config->bw_white_up);
    sys_config_set(FLOW_CTL_WHITE_UP,acValue);
    sprintf(acValue,"%u",config->bw_white_down);
    sys_config_set(FLOW_CTL_WHITE_DOWN,acValue);
	return 0;
}
int snmp_set_sta_key_intval(WIFIDOG_SNMPAGENT_STAKEY_INTVAL *pstIntval)
{
    s_config *config = NULL;
    char     acValue[32]={0};
    config = config_get_config();
    config->iStaKeyIntval=pstIntval->iIntval;
    sprintf(acValue,"%d",config->iStaKeyIntval);
    sys_config_set(WD_STAKEY_INTVAL,acValue);
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
    log_info(__LOG_DEBUG__, "RECV msg, msg id: %u, msg type: %u", ntohs(pstMsgHdr->usMsgId), ntohs(pstMsgHdr->usMsgType));

    switch(ntohl(pstMsgHdr->usMsgId)){
        case AG_MSG_IPC_REGISTER_ACK:
			g_stWifidogAglink.uiFlag = 1;
			/*wifidog 启动，向AP广播消息，让AP重新注册到wifidog*/
		    wifidog_aglink_send_nak_to_ap();
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
		#if 0
		case WIFIDOG_IDSET_AP_WIFI_INFO_UPDATE:
			log_info(__LOG_DEBUG__, "RECV WIFIDOG_IDSET_AP_WIFI_INFO_UPDATE msg");
			pstMsgBody = (WIFIOG_AP_REGISTER_T *)(pstMsgHdr + 1);
			wifidog_ap_register_handle(pstMsgBody);					
            break;
		#endif
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
            locator_wifidog_add_sta_handle(pstMsgBody);
            break;
case WIFIDOG_MSG_SNMPAGENT_SET_MAC_BAW_UPDATE:
			log_info(__LOG_DEBUG__, "RECV WIFIDOG_MSG_SNMPAGENT_SET_MAC_BAW_UPDATE msg");
			pstMsgBody = (WIFIDOG_SNMPAGENT_MAC_BAW_CONFIG_T *)(pstMsgHdr + 1);
			bblink_mac_baw_update(pstMsgBody);
			break;
        case WIFIDOG_MSG_SYN_ALL_STA:
            break;
        case WIFIDOG_MSG_STADCT_INFO:
            break;
            
        case WIFIDOG_MSG_SNMPAGENT_SET_STA_AUTH:
			log_info(__LOG_DEBUG__, "RECV WIFIDOG_MSG_SNMPAGENT_SET_STA_AUTH msg");
            pstMsgBody = (STAM_WIFIDOG_STA_T *)(pstMsgHdr + 1);
            snmp_set_sta_auth_handle(pstMsgBody);
            break;
        case WIFIDOG_MSG_SNMPAGENT_SET_AUTH_CONFIG:
            log_info(__LOG_DEBUG__, "RECV WIFIDOG_MSG_SNMPAGENT_SET_AUTH_CONFIG msg");
            pstMsgBody = (AUTH_CONF_T *)(pstMsgHdr + 1);
            snmp_set_auth_conf_handle(pstMsgBody);
            break;
        case WIFIDOG_MSG_SNMPAGENT_GET_AUTH_CONFIG:
            log_info(__LOG_DEBUG__, "RECV WIFIDOG_MSG_SNMPAGENT_GET_AUTH_CONFIG msg");
            snmp_get_auth_conf_handle();
            break;
        case WIFIDOG_MSG_SNMPAGENT_ADD_BAW_URL:
            log_info(__LOG_DEBUG__, "RECV WIFIDOG_MSG_SNMPAGENT_ADD_BAW_URL msg");
            pstMsgBody = pstMsgHdr + 1;
            snmp_set_add_baw_url(pstMsgBody);
            break;
        case WIFIDOG_MSG_SNMPAGENT_DEL_BAW_URL:
            log_info(__LOG_DEBUG__, "RECV WIFIDOG_MSG_SNMPAGENT_DEL_BAW_URL msg");
            pstMsgBody = pstMsgHdr + 1;
            snmp_set_del_baw_url(pstMsgBody);
            break;
        case WIFIDOG_MSG_SNMPAGENT_ADD_BAW_MAC:
            log_info(__LOG_DEBUG__, "RECV WIFIDOG_MSG_SNMPAGENT_ADD_BAW_MAC msg");
            pstMsgBody = pstMsgHdr + 1;
            snmp_set_add_baw_mac(pstMsgBody);
            break;
        case WIFIDOG_MSG_SNMPAGENT_DEL_BAW_MAC:
            log_info(__LOG_DEBUG__, "RECV WIFIDOG_MSG_SNMPAGENT_DEL_BAW_MAC msg");
            pstMsgBody = pstMsgHdr + 1;
            snmp_set_del_baw_mac(pstMsgBody);
            break;
        case WIFIDOG_MSG_SNMPAGENT_SET_USER_TIMEOUT:
            log_info(__LOG_DEBUG__, "RECV WIFIDOG_MSG_SNMPAGENT_SET_USER_TIMEOUT msg");
            pstMsgBody = pstMsgHdr + 1;
            snmp_set_user_timeout(pstMsgBody);
            break;
        case WIFIDOG_MSG_SNMPAGENT_SET_NORMAL_FlOW_CTL:
            log_info(__LOG_DEBUG__, "RECV WIFIDOG_MSG_SNMPAGENT_SET_NORMAL_FlOW_CTL msg");
            pstMsgBody = pstMsgHdr + 1;
            snmp_set_normal_flowctl(pstMsgBody);
            break;
        case WIFIDOG_MSG_SNMPAGENT_SET_WHITE_FlOW_CTL:
            log_info(__LOG_DEBUG__, "RECV WIFIDOG_MSG_SNMPAGENT_SET_WHITE_FlOW_CTL msg");
            pstMsgBody = pstMsgHdr + 1;
            snmp_set_white_flowctl(pstMsgBody);
            break;
        case WIFIDOG_MSG_SNMPAGENT_SET_STA_KEY_INTVAL:
            log_info(__LOG_DEBUG__, "RECV WIFIDOG_MSG_SNMPAGENT_SET_STA_KEY_INTVAL msg");
            pstMsgBody = pstMsgHdr + 1;
            snmp_set_sta_key_intval(pstMsgBody);
            break;
        default:
            log_info(__LOG_ERROR__, "UNKOWN MSG, msg id: %u, msg type: %u", ntohs(pstMsgHdr->usMsgId), ntohs(pstMsgHdr->usMsgType));
            break;
    }

	free(acBuf);
    return;
}

/*begin: add@2015.01.27 for wechat connect wifi*/
//need redesign later
int wifidog_get_gw_wan_ip(char *pcIp)
{
	FILE *pfData = NULL;
	char acIp[32] = {0};
	pfData = popen("ifconfig eth0 |grep Bcast |awk '{print $2}'", "r");
	if(NULL == pfData){
		log_info(__LOG_ERROR__,"popen failed. ");
	}
	else{
		fread(acIp, sizeof(acIp)-1, 1, pfData);
		strcpy(pcIp, acIp + sizeof("addr:") - 1);
		log_info(__LOG_DEBUG__, "pcIp: %s", pcIp);
		pclose(pfData);
	}
	return 0;
}
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

int wifidog_get_radio_vap_info(unsigned int *puiRadioNum, WD_AP_RADIO_T *pstRadio)
{
#ifdef GW_X86
	*puiRadioNum = 0;
#elif defined OPENWRT
	int i;
	int vap_num = 0;
	char nvram_name[128];
	char nvram_value[128];
	char vap_name[32];
	char ssid_name[32];
	int  radio_flag_24=0,radio_flag_5=0;
	
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
					memcpy(pstRadio[0].astVap[pstRadio[0].uiVapNum].acSsid,ssid_name,32);
					wifidog_bssid_get_by_name(vap_name,pstRadio[0].astVap[pstRadio[0].uiVapNum].aucBssid);
					sys_log_set(WIFIDOG_DEBUG_LOG_FILE," pstRadio[1].uiVapNum %d  ssid %s  bssid "MAC_FMT"  ",pstRadio[0].uiVapNum,pstRadio[0].astVap[pstRadio[0].uiVapNum].acSsid,MAC_ARG(pstRadio[0].astVap[pstRadio[0].uiVapNum].aucBssid));
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
	int i;
	char nvram_value[128];
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

		if(ssid[radio] < WD_RADIO_MAX_SSID_NUM){
			nvram_get(pstRadio[radio].astVap[ssid[radio]].acSsid, nvram_type, 64);
		
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
	return 0;
}


int wifidog_get_gw_info()
{
	WD_AP_T stAp;
	s_config *config = config_get_config();
	char acWanIp[16] = {0};
	char acName[16] = {0};
	unsigned int uiRadioNum = 0;
	WD_AP_RADIO_T astRadio[WD_AP_MAX_RADIO_NUM];
	unsigned int model_id;

	memset(&stAp, 0, sizeof(stAp));
	memset(astRadio, 0, sizeof(WD_AP_RADIO_T) * WD_AP_MAX_RADIO_NUM);
	
	/*wan ip*/
	wifidog_get_gw_wan_ip(acWanIp);
	/*dev_type*/
	model_id = pal_sysinfo_device_model_id();
	/*device name*/
	wifidog_get_gw_name(acName);
	/*radio & vap*/
	wifidog_get_radio_vap_info(&uiRadioNum, astRadio);

	stAp.uiRadioNum = uiRadioNum;
	strmac_to_mac(config->wan_mac, stAp.mac);
	*((unsigned int *)(stAp.ip)) = (unsigned int)inet_addr(acWanIp);
	stAp.uiRadioNum = uiRadioNum;
	if (uiRadioNum){
		stAp.uiRadioNum = uiRadioNum;
		memcpy(stAp.astRadio, astRadio, sizeof(WD_AP_RADIO_T) * WD_AP_MAX_RADIO_NUM);
	}
	
#ifdef GW_X86
	strcpy(stAp.model, acModel);
#else
	get_model_by_model_id(model_id, stAp.model);
#endif
	strcpy(stAp.name, acName);
#ifdef GW_X86
	ap_add_by_mac(stAp.mac, stAp.ip, 1, stAp.name, uiRadioNum, stAp.astRadio);
#else
	ap_add_by_mac(stAp.mac, stAp.ip, model_id, stAp.name, uiRadioNum, stAp.astRadio);
#endif

	ap_info_print(&stAp);
	return 0;
}


int wifidog_aglink_send_sync_sta(WIFIDOG_SNMPAGENT_ALL_STA_INFO_T *sta_info)
{
	int iRet = -1;
	int iMsgLen;
	if(NULL == sta_info)
		return -1;

	iMsgLen = sizeof(WIFIDOG_SNMPAGENT_ALL_STA_INFO_T) + 
		sizeof(STAM_SNMPAGENTSTA_INFO_T) * sta_info->iStaNum;
	
	iRet = ag_module_ipc_client_send_msg(SNMP_MSG_USERINFO,
		    							  AG_IPC_MSG_TYPE_SELF_CPU,
		    							  &g_stWifidogAglink,
		    							  (unsigned char *)sta_info,
		    							  iMsgLen);
	if (iRet < 0){
		log_info(__LOG_ERROR__, "SNMP_MSG_USERINFO send fail");
    }else{
		log_info(__LOG_DEBUG__, "SNMP_MSG_USERINFO send success");
	}
	return 0;
}



int wifidog_aglink_send_add_sta(t_client *sta_node)
{
	int iRet = -1;
	int iMsgLen;
    WIFIDOG_SNMPAGENT_STA_T *sta_info=NULL;

	if(NULL == sta_node)
    {
		return -1;
    }   

    if(NULL == (sta_info=malloc(sizeof(WIFIDOG_SNMPAGENT_STA_T))))
    {
		log_info(__LOG_ERROR__, "malloc fail");
        return -1;
    }
    memset(sta_info,0,sizeof(WIFIDOG_SNMPAGENT_STA_T));
	iMsgLen = sizeof(WIFIDOG_SNMPAGENT_STA_T);
	get_mac(sta_node->mac,sta_info->aucMac);
	get_ip(sta_node->ip,sta_info->aucIp);
    get_local_time(sta_info->acLogInTime);

	iRet = ag_module_ipc_client_send_msg(SNMP_MSG_USER_ADD,
		    							  AG_IPC_MSG_TYPE_SELF_CPU,
		    							  &g_stWifidogAglink,
		    							  (unsigned char *)sta_info,
		    							  iMsgLen);
	if (iRet < 0){
		log_info(__LOG_ERROR__, "SNMP_MSG_ADD_USER send fail");
    }else{
		log_info(__LOG_DEBUG__, "SNMP_MSG_ADD_USER send success");
	}
	return 0;
}



int wifidog_aglink_send_del_sta(t_client *sta_node)
{
	int iRet = -1;
	int iMsgLen;
    WIFIDOG_SNMPAGENT_STA_T *sta_info=NULL;

	if(NULL == sta_node)
    {
		return -1;
    }   

    if(NULL == (sta_info=malloc(sizeof(WIFIDOG_SNMPAGENT_STA_T))))
    {
		log_info(__LOG_ERROR__, "malloc fail");
        return -1;
    }
    memset(sta_info,0,sizeof(WIFIDOG_SNMPAGENT_STA_T));
	iMsgLen = sizeof(WIFIDOG_SNMPAGENT_STA_T);
	get_mac(sta_node->mac,sta_info->aucMac);
	get_ip(sta_node->ip,sta_info->aucIp);
    get_local_time(sta_info->aucLogOutTime);
	iRet = ag_module_ipc_client_send_msg(SNMP_MSG_USER_DEL,
		    							  AG_IPC_MSG_TYPE_SELF_CPU,
		    							  &g_stWifidogAglink,
		    							  (unsigned char *)sta_info,
		    							  iMsgLen);
	if (iRet < 0){
		log_info(__LOG_ERROR__, "SNMP_MSG_DEL_USER send fail");
    }else{
		log_info(__LOG_DEBUG__, "SNMP_MSG_DEL_USER send success");
	}
	return 0;
}

extern void lock_client_list();
extern void unlock_client_list();

/**在线终端同步*/
void authsta_sync_report(int *sockfd)
{
    int loop;
    t_client *p1, *p2;
	WIFIDOG_SNMPAGENT_ALL_STA_INFO_T *sta_info;
	WIFIDOG_SNMPAGENTSTA_INFO_T *sta_head, *sta;
	unsigned char *buf;;
	int buf_size = sizeof(WIFIDOG_SNMPAGENT_ALL_STA_INFO_T) 
		+ sizeof(WIFIDOG_SNMPAGENTSTA_INFO_T)*16;

	if((buf = (unsigned char *)malloc(buf_size)) == NULL){
		log_info(__LOG_ERROR__, "authsta_sync_report alloc mem failed.");
		return;
	}
	
	memset(buf, 0, buf_size);
	sta_info = (WIFIDOG_SNMPAGENT_ALL_STA_INFO_T *)buf;
	sta_head = sta_info->astStaInfo;

    for(loop = 0; loop < CLIENT_HASH_SIZE; loop++)
    {
        lock_client_list();
        p1 = g_client_head[loop].next;
        while(p1)
        {
            p2 = p1->next;
			
			sta = sta_head + sta_info->iStaNum;
            string_ip_tools(p1->ip,(char *)sta->aucIp);
            string_mac_tools(p1->mac,(char *)sta->aucMac);

			sta->iAssoTime = p1->online_time_last_update_time - p1->online_time;
			sta->iAuthType = p1->iAuthType;
			if ( NULL != p1->pBblink_userid)
			{
                strncpy(sta->acBblink_UserId,p1->pBblink_userid,strlen(p1->pBblink_userid));
			}
			else
			{
                strcpy(sta->acBblink_UserId,"0");
			}
			sta->iUpSpeed = 0;
			sta->iDownSpeed = 0;
			sta->iCnntCnt = 0;
			sta->incoming = p1->counters.incoming/1024/1024;
			sta->outgoing = p1->counters.outgoing/1024/1024;

			++sta_info->iStaNum;
            log_info(__LOG_WARNING__, "authsta_sync_report ip:"IP_FMT" mac:"MAC_FMT" AssoTime:%ld"
                                            "AuthType:%d Bblink_UserId :%s UpSpeed:%ld CnntCnt:%ld incoming:%ld outgoing:%ld StaNum:%d ",
                                            IP_ARG(sta->aucIp),MAC_ARG(sta->aucMac),sta->iAssoTime,
                                            sta->iAuthType,sta->acBblink_UserId,sta->iUpSpeed,sta->iCnntCnt,sta->incoming,sta->outgoing,sta_info->iStaNum
                                            );
			if(sta_info->iStaNum >= 16){
				sta_info->iFlag = (((NULL == p2)&&(loop >= CLIENT_HASH_SIZE - 1)) ? 1 : 0);
				wifidog_aglink_send_sync_sta((WIFIDOG_SNMPAGENT_ALL_STA_INFO_T *)sta_info);
				memset(buf, 0, buf_size);
				sta_info->iStaNum = 0;
			}

            p1 = p2;
        }
        unlock_client_list();

		if((loop >= CLIENT_HASH_SIZE - 1)&&(0 == sta_info->iFlag)){
			sta_info->iFlag = 1;
			wifidog_aglink_send_sync_sta((WIFIDOG_SNMPAGENT_ALL_STA_INFO_T *)sta_info);
            memset(buf, 0, buf_size);
			sta_info->iStaNum = 0;
		}
		
    }
	log_info(__LOG_DEBUG__, "authsta_sync_report");

	free(buf);
	return;
}


void thread_aglink_client(void){
	unsigned int uiModuleId = 0;
	int iRet = 0;
	
	uiModuleId = (u32)AG_GET_MODULE_ID(WIFIDOG_MSG_BEGIN);
	
	wifidog_set_thread_name(WIFIDOG_THREAD_AGLINK);
	/*wifidog get gw info*/
	wifidog_get_gw_info();
	
    memset(&g_stWifidogAglink, 0, sizeof(g_stWifidogAglink));
    iRet = ag_module_ipc_module_init((unsigned char *)MODULE_NAME_WIFIDOG, uiModuleId, &g_stWifidogAglink, 
		(IPC_CLIENT_RECV_EVENT *)wifidog_aglink_msg_handle);  
	
    ag_module_ipc_power_on(&g_stWifidogAglink);
		
	return;
}

