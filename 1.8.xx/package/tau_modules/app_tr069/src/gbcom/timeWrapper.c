/******************************************************************************

                  ��Ȩ���� (C), 2010-2012, �Ϻ�徴�ͨ�ſƼ����޹�˾

 ******************************************************************************
  �� �� ��   : timeWrapper.c
  �� �� ��   : ����
  ��    ��   : zhangjiasi
  ��������   : 2015��6��9��
  ����޸�   :
  ��������   : GBCOM ��װ�Ĺ���time��صĽӿ�ʵ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��6��9��
    ��    ��   : zhangjiasi
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include "utils.h"
#include "ap_types.h"
#include "om_tr69c.h"

#include "timeParams.h"
/*Add by zhanghao ��ȡ�����ļ����� 2016/6/15 begin*/
#include "uci_config.h"
#include "tr69_lua.h"
/*Add by zhanghao ��ȡ�����ļ����� end*/
#define TAUARRY_LEN 128

/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/
extern int g_log_level;

extern TRxObjNode  TimeDesc[];

/*-------add by wangminglin-----------*/
TRX_STATUS setTimeEnable(const char *pcValue)
{

	if(UCI_OK !=uci_config_set(UCI_TIME_ENABLE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}

TRX_STATUS getTimeEnable(char **ppcValue)
{
	char acTimeEnable[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_TIME_ENABLE,acTimeEnable))
	{
		*ppcValue = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acTimeEnable);
		
	}	
	return TRX_OK;
}

TRX_STATUS getTimeStatus(char **ppcValue)
{
	char  acTimeStatus[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_TIME_STATUS,acTimeStatus))
	{
		*ppcValue = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acTimeStatus);
		
	}	
	return TRX_OK;
}


TRX_STATUS setConfigType(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_TIME_CONFIGTYPE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	return TRX_OK;
}

TRX_STATUS getConfigType(char **ppcValue)
{
	char acConfigType[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_TIME_CONFIGTYPE,acConfigType))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acConfigType);
		
	}	
	return TRX_OK;
}

TRX_STATUS setX_ATP_MannualTime(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_TIME_X_ATP_MANNUALTIME,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getX_ATP_MannualTime(char **ppcValue)
{
	char  acX_ATP_MannualTime[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_TIME_X_ATP_MANNUALTIME,acX_ATP_MannualTime))
	{
		*ppcValue = strdup("00");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_MannualTime);
		
	}	
	return TRX_OK;
}







TRX_STATUS setNTPServer1(const char *pcValue)
{
    /* OM_TR69C_NTP_PARAM_T stTr69cNtpCfg = {0};

    stTr69cNtpCfg.eFlag = TR69C_NTP_PARAM_NTP_SVR;
    memcpy(stTr69cNtpCfg.stNtpInfo.szNtpSvr,value,strlen(value));
    if(OM_E_NONE != om_tr69c_set_ntp_info(&stTr69cNtpCfg))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);
        return TRX_ERR;
    } */
    if(UCI_OK !=uci_config_set(UCI_TIME_NTPSERVER1,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
    return TRX_OK;
}

TRX_STATUS getNTPServer1(char **ppcValue)
{
    /* OM_TR69C_NTP_PARAM_T stTr69cNtpCfg = {0};

    stTr69cNtpCfg.eFlag = TR69C_NTP_PARAM_NTP_SVR;
    if(OM_E_NONE != om_tr69c_get_ntp_info(&stTr69cNtpCfg))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);
        return TRX_ERR;
    }
    *value = stTr69cNtpCfg.stNtpInfo.szNtpSvr?strdup(stTr69cNtpCfg.stNtpInfo.szNtpSvr):strdup("");
         */
    char acNTPServer1[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_TIME_NTPSERVER1,acNTPServer1))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acNTPServer1);
		
	}	
    return TRX_OK;
}

TRX_STATUS setNTPServer2(const char *pcValue)
{
    /* OM_TR69C_NTP_PARAM_T stTr69cNtpCfg = {0};

    stTr69cNtpCfg.eFlag = TR69C_NTP_PARAM_NTP_SVR2;
    memcpy(stTr69cNtpCfg.stNtpInfo.szNtpSvr2,value,strlen(value));
    if(OM_E_NONE != om_tr69c_set_ntp_info(&stTr69cNtpCfg))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);
        return TRX_ERR;
    } */
    if(UCI_OK !=uci_config_set(UCI_TIME_NTPSERVER2,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
    return TRX_OK;
}

TRX_STATUS getNTPServer2(char **ppcValue)
{
    /* OM_TR69C_NTP_PARAM_T stTr69cNtpCfg = {0};

    stTr69cNtpCfg.eFlag = TR69C_NTP_PARAM_NTP_SVR2;
    if(OM_E_NONE != om_tr69c_get_ntp_info(&stTr69cNtpCfg))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);
        return TRX_ERR;
    }
    *value = stTr69cNtpCfg.stNtpInfo.szNtpSvr2?strdup(stTr69cNtpCfg.stNtpInfo.szNtpSvr2):strdup(""); */
    char acNTPServer2[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_TIME_NTPSERVER1,acNTPServer2))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acNTPServer2);
		
	}	   
    return TRX_OK;
}


void initTimeInstances()
{
    TRxObjNode *n;
    int id;
    InstanceDesc  *timeIdp;
    
    n = TimeDesc;
    id = 1;
    timeIdp = getNewInstanceDesc(n, NULL, id);
}

