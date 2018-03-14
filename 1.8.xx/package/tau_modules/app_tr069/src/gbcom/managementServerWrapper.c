/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : managementServer.c
  版 本 号   : 初稿
  作    者   : zhangjiasi
  生成日期   : 2015年6月9日
  最近修改   :
  功能描述   : GBCOM 封装的管理服务器相关的接口实现
  函数列表   :
  修改历史   :
  1.日    期   : 2015年6月9日
    作    者   : zhangjiasi
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include "utils.h"
#include "ap_types.h"
#include "om_tr69c.h"
#include "om_log.h"
//#include "pal/app/pal_app.h"

#include "SOAPParser/RPCState.h"
#include "informer.h"
#include "deviceInfoParams.h"
#include "sysWrapper.h"
#include "wanDeviceParams.h"
#include "manageServerParams.h"
#include "ipPingDiagnosticsParams.h"
#include "traceRouteDiagnosticsParams.h"
/*Add by zhanghao 获取配置文件内容 2016/6/15 begin*/
#include "uci_config.h"
#include "tr69_lua.h"
/*Add by zhanghao 获取配置文件内容 end*/
/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
#define TAUARRY_LEN 128
extern int g_log_level;

extern TRxObjNode  mgmtServerDesc[];
/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
extern ACSState acsState;
extern int lastInstanceId;
extern int g_debug;
extern eInformState informState;
extern LTEInfoState lteinfostate;
extern int g_device_first_up;
/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
int getPasswordAuthFlag()
{
    /* OM_TR69C_APP_PARAM_T *pstTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(pstTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    pstTr69cAcsInfo->eSetFlag = TR069_PASS_AUTH_FLAG;
    if(OM_E_NONE != om_tr69c_get_acsinfo(pstTr69cAcsInfo))
    {
        free(pstTr69cAcsInfo);
        pstTr69cAcsInfo = NULL;
        slog(TR69C_LOG_ERR,"ERROR");
		acsState.passAuthflag=0;
        return TRX_OK;
    }
    acsState.passAuthflag= pstTr69cAcsInfo->passAuthFlag; 
    free(pstTr69cAcsInfo);
	printf("getPasswordAuthFlag = %d \n",acsState.passAuthflag);
    pstTr69cAcsInfo = NULL; */
    return TRX_OK;  
}
TRX_STATUS getCWMPEnable(char **ppcValue)
{
   /*  OM_TR69C_APP_PARAM_T *pstTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(pstTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    pstTr69cAcsInfo->eSetFlag = TR069_ENABLE;
    if(OM_E_NONE != om_tr69c_get_acsinfo(pstTr69cAcsInfo))
    {
        free(pstTr69cAcsInfo);
        pstTr69cAcsInfo = NULL;
        slog(TR69C_LOG_ERR,"ERROR");
		*value = strdup("1");
        return TRX_OK;
    }
    acsState.informEnable = pstTr69cAcsInfo->EnableCWMP; 
    *value = strdup(pstTr69cAcsInfo->EnableCWMP? "1": "0");
    free(pstTr69cAcsInfo);
    pstTr69cAcsInfo = NULL; */
    
    char acCWMPEnable[TAUARRY_LEN]={0};
	if (acsState.informEnable != NULL)
	{
		free(acsState.informEnable);
		acsState.informEnable=NULL;
	}	
	if(UCI_OK != uci_config_get(UCI_RETRIEVETR69STATUSITEMS_INFORMCWMPENABLE,acCWMPEnable))
	{
		*ppcValue = strdup("");
		slog(TR69C_LOG_ERR,"ERROR");
		return TRX_ERR;
	}
	else
	{
		 *ppcValue = strdup(acCWMPEnable);
					  
	}	
	acsState.informEnable=atoi(strdup(*ppcValue));
	//acsState.informEnable=1;
    return TRX_OK;     
}

TRX_STATUS setURL(const char *pcValue)
{
   /*  OM_TR69C_APP_PARAM_T *stTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(stTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    stTr69cAcsInfo->eSetFlag = TR069_ACS_URL;
    strcpy(stTr69cAcsInfo->acsURL, value);
    if(OM_E_NONE != om_tr69c_set_acsinfo(stTr69cAcsInfo))
    {
        free(stTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
    }

    if (acsState.acsURL == NULL)
		acsState.acsURL = strdup(value);
	else if (strcmp(acsState.acsURL, value)) {
		printf("write connection  state 444 \n");
		setACSContactedState(eACS_NEVERCONTACTED);
		saveACSContactedState();
		free(acsState.acsURL);
		acsState.acsURL = strdup(value);
	}
    
    free(stTr69cAcsInfo); */
    if(UCI_OK !=uci_config_set(UCI_MANAGEMENTSERVER_URL,pcValue))
    {
    	 slog(TR69C_LOG_ERR,"ERROR");
		 return TRX_ERR;
	}
    return TRX_OK;
}

TRX_STATUS getURL(char **ppcValue)
{
    /* OM_TR69C_APP_PARAM_T *stTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(stTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    stTr69cAcsInfo->eSetFlag = TR069_ACS_URL;
	//change by duanguiyuan 20160323 BEGIN
#if 0
  	if(0 == acsState.retryCount%7)
    {
        stTr69cAcsInfo->uiReGetUrlCnt = 1;  //首次切换 
    }
    else if (0 == acsState.retryCount%9)
    {
        stTr69cAcsInfo->uiReGetUrlCnt = 0;  //再次返回 
    }else if (0 == acsState.retryCount)
        stTr69cAcsInfo->uiReGetUrlCnt = 0;
#endif
	if(9 < acsState.retryCount%10)
    {
        stTr69cAcsInfo->uiReGetUrlCnt = 1;  //首次切换 
    }else{
		stTr69cAcsInfo->uiReGetUrlCnt = 0;
	}
	//change by duanguiyuan 20160323 BEGIN
    if(OM_E_NONE != om_tr69c_get_acsinfo(stTr69cAcsInfo))
    {
        free(stTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
		*value = strdup("http://ACS.chinamobile.com:5481/ACS-server/ACS");
        return TRX_OK;
    }
    if (acsState.acsURL == NULL)
		acsState.acsURL = strdup("");
	if (strcmp(acsState.acsURL, stTr69cAcsInfo->acsURL))
    {
		free(acsState.acsURL);
		acsState.acsURL = strdup(stTr69cAcsInfo->acsURL);
	}
	*value = acsState.acsURL?strdup(acsState.acsURL):strdup("");
    free(stTr69cAcsInfo); */
	char acURL[TAUARRY_LEN]={0};
	if (acsState.acsURL != NULL)
	{
		free(acsState.acsURL);
		acsState.acsURL=NULL;
	}	
	if(UCI_OK != uci_config_get(UCI_MANAGEMENTSERVER_URL,acURL))
	{
		acsState.acsURL = strdup("0.0.0.0");
		*ppcValue = strdup(acsState.acsURL);
		slog(TR69C_LOG_ERR,"ERROR");
		return TRX_ERR;
	}
	else
	{
		 acsState.acsURL = strdup(acURL);
		 *ppcValue = strdup(acsState.acsURL);				  
	}		 

   // acsState.acsURL=strdup("http://172.16.1.2:8080/");
    //acsState.acsURL=strdup("http://192.168.128.1:80/");
	//*value = strdup(acsState.acsURL);
    return TRX_OK;
}


TRX_STATUS setUsername(const char *pcValue)
{
	/* OM_TR69C_APP_PARAM_T *stTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(stTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    stTr69cAcsInfo->eSetFlag = TR069_ACS_USRNAME;
    strcpy(stTr69cAcsInfo->acsUser, value);
    if(OM_E_NONE != om_tr69c_set_acsinfo(stTr69cAcsInfo))
    {
        free(stTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
    }
    if (NULL != acsState.acsUser)
	    free(acsState.acsUser);
	acsState.acsUser = strdup(value);	
    free(stTr69cAcsInfo); */
    if(UCI_OK !=uci_config_set(UCI_MANAGEMENTSERVER_USERNAME,pcValue))
    {
    	 slog(TR69C_LOG_ERR,"ERROR");
		 return TRX_ERR;
	}
	return TRX_OK;
}

TRX_STATUS getUsername(char **ppcValue)
{
    /* OM_TR69C_APP_PARAM_T *stTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(stTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    stTr69cAcsInfo->eSetFlag = TR069_ACS_USRNAME;
    if(OM_E_NONE != om_tr69c_get_acsinfo(stTr69cAcsInfo))
    {
        free(stTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
		*value = strdup("acs");
        return TRX_OK;
    }
    if (NULL != acsState.acsUser)
	    free(acsState.acsUser);
	acsState.acsUser = strdup(stTr69cAcsInfo->acsUser);
	*value = acsState.acsUser?strdup(acsState.acsUser):strdup("");
    free(stTr69cAcsInfo); */
    char acUserName[TAUARRY_LEN]={0};
	if (NULL != acsState.acsUser)
	{
		free(acsState.acsUser);
		acsState.acsUser=NULL;
	}
	    
	if(UCI_OK != uci_config_get(UCI_MANAGEMENTSERVER_USERNAME,acUserName))
	{
		acsState.acsUser = strdup("");
		*ppcValue = strdup(acsState.acsUser);
		slog(TR69C_LOG_ERR,"ERROR");
		return TRX_ERR;
	}
	else
	{
		 acsState.acsUser = strdup(acUserName);
		 *ppcValue = strdup(acsState.acsUser);				  
	}		 
	
    return TRX_OK;
}

TRX_STATUS setPassword(const char *pcValue)
{
   /*  OM_TR69C_APP_PARAM_T *stTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(stTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    stTr69cAcsInfo->eSetFlag = TR069_ACS_PASSWD;
    strcpy(stTr69cAcsInfo->acsPwd, value);
    if(OM_E_NONE != om_tr69c_set_acsinfo(stTr69cAcsInfo))
    {
        free(stTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
    }

    if (NULL != acsState.acsPwd)
        free(acsState.acsPwd);
    acsState.acsPwd = strdup(value);  
    free(stTr69cAcsInfo); */
    if(UCI_OK !=uci_config_set(UCI_MANAGEMENTSERVER_PASSWORD,pcValue))
    {
    	 slog(TR69C_LOG_ERR,"ERROR");
		 return TRX_ERR;
	}
    return TRX_OK;
}

TRX_STATUS getPassword(char **ppcValue)
{
   /*  OM_TR69C_APP_PARAM_T *stTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(stTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    stTr69cAcsInfo->eSetFlag = TR069_ACS_PASSWD;
    if(OM_E_NONE != om_tr69c_get_acsinfo(stTr69cAcsInfo))
    {
        free(stTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
		*value = strdup("cwmp");		
        return TRX_OK;
    }
    if (NULL != acsState.acsPwd)
        free(acsState.acsPwd);
    acsState.acsPwd = strdup(stTr69cAcsInfo->acsPwd);
    *value = acsState.acsPwd?strdup(acsState.acsPwd):strdup("");
    free(stTr69cAcsInfo); */
	char aPassWord[TAUARRY_LEN]={0};
	if (NULL != acsState.acsPwd)
	{
		free(acsState.acsPwd);
		acsState.acsPwd=NULL;
	}
      
	if(UCI_OK != uci_config_get(UCI_MANAGEMENTSERVER_PASSWORD,aPassWord))
	{
		acsState.acsPwd = strdup("");
		*ppcValue = strdup(acsState.acsPwd);
		slog(TR69C_LOG_ERR,"ERROR");
		return TRX_ERR;
	}
	else
	{
		 acsState.acsPwd = strdup(aPassWord);
		 *ppcValue = strdup(acsState.acsPwd);				  
	}		   
	//acsState.acsPwd = strdup("123456789");
	//*value = strdup(acsState.acsPwd);
	
    return TRX_OK;
}

/* TODO */
TRX_STATUS setCWMPEnable(const char *value)
{
   /*  OM_TR69C_APP_PARAM_T *stTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(stTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    stTr69cAcsInfo->eSetFlag = TR069_ENABLE;
    stTr69cAcsInfo->EnableCWMP= atoi(value);
    if(OM_E_NONE != om_tr69c_set_acsinfo(stTr69cAcsInfo))
    {
        free(stTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    acsState.informEnable = testBoolean(value);
    free(stTr69cAcsInfo); */
    return TRX_OK;
}

TRX_STATUS setPeriodicInformEnable(const char *pcValue)
{
	/* OM_TR69C_APP_PARAM_T *stTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(stTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    stTr69cAcsInfo->eSetFlag = TR069_ECHO_ENABLE;
    stTr69cAcsInfo->PeriodicInformEnable= atoi(value);
    if(OM_E_NONE != om_tr69c_set_acsinfo(stTr69cAcsInfo))
    {
        free(stTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
    }
    acsState.PeriodicInformEnable= atoi(value);
	//resetPeriodicInform(acsState.informInterval);
    free(stTr69cAcsInfo); */
    if(UCI_OK !=uci_config_set(UCI_MANAGEMENTSERVER_PERIODICINFOENABLE,pcValue))
    {
    	 slog(TR69C_LOG_ERR,"ERROR");
		 return TRX_ERR;
	}
    return TRX_OK;

}
TRX_STATUS getPeriodicInformEnable(char **ppcValue)
{
   /*  OM_TR69C_APP_PARAM_T *stTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(stTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));  // memset 
	char buf[CHAR_LEN_32];
	memset(buf, 0, sizeof(buf));
    if(OM_E_NONE != om_tr69c_get_acsinfo(stTr69cAcsInfo))
    {
        free(stTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
		*value = strdup("0");
        return TRX_OK;
    }
	
    acsState.PeriodicInformEnable = stTr69cAcsInfo->PeriodicInformEnable;
    sprintf(buf, "%d", stTr69cAcsInfo->PeriodicInformEnable);
    *value = strdup(buf);
    free(stTr69cAcsInfo);   //free  */
    char aPeriodicInform[TAUARRY_LEN]={0};
    if(UCI_OK != uci_config_get(UCI_MANAGEMENTSERVER_PERIODICINFOENABLE,aPeriodicInform))
	{
		*ppcValue = strdup("");
		slog(TR69C_LOG_ERR,"ERROR");
		return TRX_ERR;
	}
	else
	{
		  *ppcValue = strdup(aPeriodicInform);
		  acsState.PeriodicInformEnable = atoi(aPeriodicInform);		  
	}		   
    //printf("PeriodicInformEnable is $$$$$$$$$$$$ %d\n",acsState.PeriodicInformEnable);
    return TRX_OK;
}
TRX_STATUS setPeriodicInformInterval(const char *pcValue)
{
   /*  OM_TR69C_APP_PARAM_T *stTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(stTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    stTr69cAcsInfo->eSetFlag = TR069_ECHO_TIME;
    stTr69cAcsInfo->informInterval = atoi(value);
    if(OM_E_NONE != om_tr69c_set_acsinfo(stTr69cAcsInfo))
    {
        free(stTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    acsState.informInterval = atoi(value);
	resetPeriodicInform(acsState.informInterval);
    free(stTr69cAcsInfo); */
    if(UCI_OK !=uci_config_set(UCI_MANAGEMENTSERVER_PERIDDICINFOINTERVAL,pcValue))
    {
    	 slog(TR69C_LOG_ERR,"ERROR");
		 return TRX_ERR;
	}
    return TRX_OK;
}

TRX_STATUS getPeriodicInformInterval(char **ppcValue)
{
    /* OM_TR69C_APP_PARAM_T *stTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(stTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));  // memset 
    #if 0//根据定期inform开关决定是否获取定期时间，此处暂时忽略
    stTr69cAcsInfo->eSetFlag = TR069_ECHO_ENABLE;
    if(OM_E_NONE != om_tr69c_get_acsinfo(stTr69cAcsInfo))
    {
        free(stTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    
    if (stTr69cAcsInfo->PeriodicInformEnable)
    {
        stTr69cAcsInfo->eSetFlag = TR069_ECHO_TIME;
        if(OM_E_NONE != om_tr69c_get_acsinfo(stTr69cAcsInfo))
        {
            free(stTr69cAcsInfo);
            slog(TR69C_LOG_ERR,"ERROR");
            return TRX_ERR;
        }
        acsState.informInterval = stTr69cAcsInfo->informInterval;
    }
    else if (0 == stTr69cAcsInfo->PeriodicInformEnable) // 如果周期inform关闭掉，则周期Inform时间为0 
        acsState.informInterval = 0;
    else
        slog(TR69C_LOG_ERR,"[tr69c] get PeriodicInformEnable from CPE ERROR\n");
    #else
    stTr69cAcsInfo->eSetFlag = TR069_ECHO_TIME;
    if(OM_E_NONE != om_tr69c_get_acsinfo(stTr69cAcsInfo))
    {
        free(stTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    acsState.informInterval = stTr69cAcsInfo->informInterval;
    #endif
    
    *value = strdup(itoa(acsState.informInterval));
    free(stTr69cAcsInfo);   //free */
	char aPeriodicInform[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_MANAGEMENTSERVER_PERIDDICINFOINTERVAL,aPeriodicInform))
	{
		*ppcValue = strdup("");
		slog(TR69C_LOG_ERR,"ERROR");
		return TRX_ERR;
	}
	else
	{
		  *ppcValue = strdup(aPeriodicInform);
		  acsState.informInterval = atoi(aPeriodicInform);		  
	}		   
   // printf("informInterval is $$$$$$$$$$$$ %d\n",acsState.informInterval);
    return TRX_OK;
}

TRX_STATUS setPeriodicInformTime(const char *value)
{     
	extern char *strptime(const char *s, const char *format, struct tm *tm);
	struct tm bt;

	if ( strcmp(value, "0000-00-00T00:00:00") != 0 ) {                 
		strptime(value,"%Y-%m-%dT%H:%M:%S", &bt );
		acsState.informTime = mktime(&bt);
		time_t interval = acsState.informTime - time(NULL);
		if ( interval > 0 && interval < acsState.informInterval )
			resetPeriodicInform(interval);
	} else
		acsState.informTime = -1;
                        
	return TRX_OK;
}
TRX_STATUS getPeriodicInformTime(char **value)
{

	
	char    buf[30];
    memset(buf, 0, sizeof(buf));    
	if ( acsState.informTime != -1 ) {
		struct tm *bt=localtime(&acsState.informTime);
		strftime(buf,sizeof(buf),"%Y-%m-%dT%H:%M:%S",bt );
		*value = strdup(buf);
	}
	else
		
		*value = strdup("0000-00-00T00:00:00");
        
    return TRX_OK;
}

TRX_STATUS getParameterKey(char **ppcValue)
{
    /* 初始化时已经从NVRAM里读取 by zhanghaozhong*/
	 char acParameterKey[TAUARRY_LEN]={0};
	if (NULL != acsState.parameterKey)
	{
		free(acsState.parameterKey);
		acsState.parameterKey=NULL;
	}
	    
	if(UCI_OK != uci_config_get(UCI_MANAGEMENTSERVER_PARAMKEYL,acParameterKey))
	{
		
		*ppcValue = strdup("");
		slog(TR69C_LOG_ERR,"ERROR");
		return TRX_ERR;
	}
	else
	{
		
		 *ppcValue = strdup(acParameterKey);				  
	}		 
	 acsState.parameterKey = strdup(*ppcValue);
   // *value = acsState.parameterKey? strdup(acsState.parameterKey) : strdup("");
    return TRX_OK;
}


TRX_STATUS getConnectionRequestURL(char **ppcValue)
{
    //*value = acsState.connReqURL?strdup(acsState.connReqURL):strdup("");
    char acConnectionRequestURL[TAUARRY_LEN]={0};
	if (NULL != acsState.connReqURL)
	{
	 	free(acsState.connReqURL);
		acsState.connReqURL=NULL;
	}
	if(UCI_OK != uci_config_get(UCI_MANAGEMENTSERVER_CONNECTIONREQURL,acConnectionRequestURL))
	{
		acsState.connReqURL = strdup("");
		*ppcValue = strdup(acsState.connReqURL);
		slog(TR69C_LOG_ERR,"ERROR");
		return TRX_ERR;
	}
	else
	{
		 acsState.connReqURL = strdup(acConnectionRequestURL);
		 *ppcValue = strdup(acsState.connReqURL);				  
	}		 
	
   // acsState.connReqURL=strdup("http://172.16.1.2:8080/");
   //acsState.connReqURL=strdup("http://192.168.128.1:80/");
   // *value =strdup(acsState.connReqURL);

    return TRX_OK;
}

TRX_STATUS setConnectionRequestUsername(const char *pcValue)
{
   /*  OM_TR69C_APP_PARAM_T *stTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(stTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    stTr69cAcsInfo->eSetFlag = TR069_CPE_USRNAME;
    strcpy(stTr69cAcsInfo->connReqUser, value);
    
    if(OM_E_NONE != om_tr69c_set_acsinfo(stTr69cAcsInfo))
    {
        free(stTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
    }
    if (NULL != acsState.connReqUser)
        free(acsState.connReqUser);
    acsState.connReqUser= strdup(value);
    free(stTr69cAcsInfo); */
     if(UCI_OK !=uci_config_set(UCI_MANAGEMENTSERVER_CONNECTIONREQUSERNAME,pcValue))
    {
    	 slog(TR69C_LOG_ERR,"ERROR");
		 return TRX_ERR;
	}
    return TRX_OK;
}

TRX_STATUS getConnectionRequestUsername(char **ppcValue)
{
    /* OM_TR69C_APP_PARAM_T *stTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(stTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));  //memset 
    stTr69cAcsInfo->eSetFlag = TR069_CPE_USRNAME;
    if(OM_E_NONE != om_tr69c_get_acsinfo(stTr69cAcsInfo))
    {
        free(stTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
		*value = strdup("cpe");
        return TRX_OK;
    }

	if (NULL != acsState.connReqUser)
	    free(acsState.connReqUser);
	acsState.connReqUser = strdup(stTr69cAcsInfo->connReqUser);

	*value = acsState.connReqUser?strdup(acsState.connReqUser):strdup("");
    free(stTr69cAcsInfo); */
	char acConnectionRequestUsername[TAUARRY_LEN]={0};
	if (NULL != acsState.connReqUser)
	{
		free(acsState.connReqUser);
		acsState.connReqUser=NULL;
	}
	if(UCI_OK != uci_config_get(UCI_MANAGEMENTSERVER_CONNECTIONREQUSERNAME,acConnectionRequestUsername))
	{
		acsState.connReqUser = strdup("");
		*ppcValue = strdup(acsState.connReqUser);
		slog(TR69C_LOG_ERR,"ERROR");
		return TRX_ERR;
	}
	else
	{
		  acsState.connReqUser = strdup(acConnectionRequestUsername);
		 *ppcValue = strdup(acsState.connReqUser);				  
	}		 
	
  
	//acsState.connReqUser=NULL;
	//acsState.connReqUser=strdup("cpe");
	//*value = strdup(acsState.connReqUser);
	
	return TRX_OK;
}
TRX_STATUS setConnectionRequestPassword(const char *pcValue)
{
   /*  OM_TR69C_APP_PARAM_T *stTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(stTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    stTr69cAcsInfo->eSetFlag = TR069_CPE_PASSWD;
    strcpy(stTr69cAcsInfo->connReqPwd, value);
    if(OM_E_NONE != om_tr69c_set_acsinfo(stTr69cAcsInfo))
    {
        free(stTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
    }
    if (NULL != acsState.connReqPwd)
        free(acsState.connReqPwd);
    acsState.connReqPwd= strdup(value);
    free(stTr69cAcsInfo); */
    if(UCI_OK !=uci_config_set(UCI_MANAGEMENTSERVER_CONNECTIONREQPASSWORD,pcValue))
    {
    	 slog(TR69C_LOG_ERR,"ERROR");
		 return TRX_ERR;
	}
    return TRX_OK;
}

TRX_STATUS getConnectionRequestPassword(char **ppcValue)
{    
    /* OM_TR69C_APP_PARAM_T *stTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(stTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));  // memset 
    stTr69cAcsInfo->eSetFlag = TR069_CPE_PASSWD;
    if(OM_E_NONE != om_tr69c_get_acsinfo(stTr69cAcsInfo))
    {
        free(stTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
		*value = strdup("cwmp");
        return TRX_OK;
    }
        free(acsState.connReqPwd);
    acsState.connReqPwd = strdup(stTr69cAcsInfo->connReqPwd);

    *value = acsState.connReqPwd?strdup(acsState.connReqPwd):strdup("");
    free(stTr69cAcsInfo); */
    char aConnectionRequestPassword[TAUARRY_LEN]={0};
    if (NULL != acsState.connReqPwd)
	{
		free(acsState.connReqPwd);
		acsState.connReqPwd=NULL;
	}
    if(UCI_OK != uci_config_get(UCI_MANAGEMENTSERVER_CONNECTIONREQPASSWORD,aConnectionRequestPassword))
	{
		acsState.connReqPwd = strdup("");
		*ppcValue = strdup(acsState.connReqPwd);
		slog(TR69C_LOG_ERR,"ERROR");
		return TRX_ERR;
	}
	else
	{
		 acsState.connReqPwd = strdup(aConnectionRequestPassword);
		// acsState.connReqPwd = strdup("acs");
		 *ppcValue = strdup(acsState.connReqPwd);				  
	}		 
  
	//acsState.connReqPwd =strdup("cwmp");
	//*value = strdup(acsState.connReqPwd);
    return TRX_OK;
}
TRX_STATUS setmgmtServerEnableCWMP(const char *pcValue)
{
   
    return TRX_OK;
}
TRX_STATUS getmgmtServerEnableCWMP(char **ppcValue)
{    
   
    return TRX_OK;
}
TRX_STATUS setX_ATP_ConnReqPort(const char *pcValue)
{
   
    return TRX_OK;
}
TRX_STATUS getX_ATP_ConnReqPort(char **ppcValue)
{    
   
    return TRX_OK;
}

void updateConnectionInfo(void)
{
	char	*value=NULL;
	slog(TR69C_LOG_ERR,"ucp >>>>>>>>> 0 ");
	/* get items that may have been change by WEB-UI */
	getPeriodicInformEnable(&value);
	slog(TR69C_LOG_ERR,"ucp >>>>>>>>> 1 ");
	// need to free value since it's allocated memory by strdup        
	if ( value != NULL ) 
	{	
		free(value); 
		value=NULL;
	}
	getPeriodicInformInterval(&value);
	slog(TR69C_LOG_ERR,"ucp >>>>>>>>> 2 ");
	if ( value != NULL ) 
	{	
		free(value); 
		value=NULL;
	}
	getPeriodicInformTime(&value);
	slog(TR69C_LOG_ERR,"ucp >>>>>>>>> 3 ");
	if ( value != NULL ) 
	{	
		free(value); 
		value=NULL;
	}
	getURL(&value);
	slog(TR69C_LOG_ERR,"ucp >>>>>>>>> 4 ");
	if ( value != NULL ) 
	{	
		free(value); 
		value=NULL;
	}
	getUsername(&value);
	slog(TR69C_LOG_ERR,"ucp >>>>>>>>> 5 ");
	if ( value != NULL ) 
	{	
		free(value); 
		value=NULL;
	}
	getPassword(&value);
	slog(TR69C_LOG_ERR,"ucp >>>>>>>>> 6 ");
	if ( value != NULL ) 
	{	
		free(value); 
		value=NULL;
	}
    getConnectionRequestURL(&value);
	slog(TR69C_LOG_ERR,"ucp >>>>>>>>> 7 ");
	if ( value != NULL ) 
	{	
		free(value); 
		value=NULL;
	}
	getConnectionRequestUsername(&value);
	slog(TR69C_LOG_ERR,"ucp >>>>>>>>> 8 ");
	if ( value != NULL ) 
	{	
		free(value); 
		value=NULL;
	}
	getConnectionRequestPassword(&value);
	slog(TR69C_LOG_ERR,"ucp >>>>>>>>>9 ");
	if ( value != NULL ) 
	{	
		free(value); 
		value=NULL;
	}
	slog(TR69C_LOG_ERR,"ucp >>>>>>>>>10 ");

   
}

TRX_STATUS getUpgradesManaged(char **value)
{
	/* CHAR acTemp[CHAR_LEN_32] = {0}; 
    OM_TR69C_APP_PARAM_T *stTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(stTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
	memset(acTemp, 0, sizeof(acTemp));
    stTr69cAcsInfo->eSetFlag = TR069_UPGRADE_ENABLE;
    if(OM_E_NONE != om_tr69c_get_acsinfo(stTr69cAcsInfo))
    {
        free(stTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    //acsState.upgradesManaged = (char)(stTr69cAcsInfo->upgradesManaged & 0x0F);
    acsState.upgradesManaged = stTr69cAcsInfo->upgradesManaged;
	sprintf(acTemp,"%d",stTr69cAcsInfo->upgradesManaged);
	
    *value = strdup(acTemp);
    free(stTr69cAcsInfo); */
    acsState.upgradesManaged=0;
	*value = strdup("0");
    return TRX_OK;
}

TRX_STATUS setUpgradesManaged(const char *value)
{
    /* OM_TR69C_APP_PARAM_T *stTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(stTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    stTr69cAcsInfo->eSetFlag = TR069_UPGRADE_ENABLE;
    stTr69cAcsInfo->upgradesManaged = atoi(value);
    if (g_debug)
        fprintf(stderr, "stTr69cAcsInfo->upgradesManaged=%d\n", stTr69cAcsInfo->upgradesManaged);
    if(OM_E_NONE != om_tr69c_set_acsinfo(stTr69cAcsInfo))
    {
        free(stTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    acsState.upgradesManaged = stTr69cAcsInfo->upgradesManaged;
    free(stTr69cAcsInfo); */
    return TRX_OK;
}

/* acs用于连接cpe的地址和端口 */
TRX_STATUS getUDPConnectionRequestAddress(char **value)
{    
    /* OM_TR69C_APP_PARAM_T *pstTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(pstTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    pstTr69cAcsInfo->eSetFlag = TR069_UDP_REQADDR;
    if(OM_E_NONE != om_tr69c_get_acsinfo(pstTr69cAcsInfo))
    {
        free(pstTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    *value =strdup(pstTr69cAcsInfo->UDPConnReqAddr);
	 slog(TR69C_LOG_DEBUG,"udp_req_addr = %s",pstTr69cAcsInfo->UDPConnReqAddr);
    free(pstTr69cAcsInfo); */
    return TRX_OK;
}


/* ActiveNotification更改到新的地址时，中间等待的最小时间，单位：秒 */
TRX_STATUS setUDPConnectionRequestAddressNotificationLimit(const char *value)
{    
    /* OM_TR69C_APP_PARAM_T *pstTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(pstTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    pstTr69cAcsInfo->eSetFlag = TR069_UDP_REQADDR_NOTIFY_LIMIT;
    pstTr69cAcsInfo->UDPConnReqAddrNotifynLimit = atoi(value);
    if (g_debug)
        fprintf(stderr, "stTr69cAcsInfo->UDPConnReqAddrNotifynLimit=%d\n", pstTr69cAcsInfo->UDPConnReqAddrNotifynLimit);
    if(OM_E_NONE != om_tr69c_set_acsinfo(pstTr69cAcsInfo))
    {
        free(pstTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
	slog(TR69C_LOG_ERR,"setUDPConnectionRequestAddressNotificationLimit success ");
    free(pstTr69cAcsInfo); */
    return TRX_OK;
}

TRX_STATUS getUDPConnectionRequestAddressNotificationLimit(char **value)
{   
	/* CHAR acTemp[CHAR_LEN_32] = {0}; 
    OM_TR69C_APP_PARAM_T *pstTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(pstTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
	memset(acTemp, 0, sizeof(acTemp));
    pstTr69cAcsInfo->eSetFlag = TR069_UDP_REQADDR_NOTIFY_LIMIT;
    if(OM_E_NONE != om_tr69c_get_acsinfo(pstTr69cAcsInfo))
    {
        free(pstTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
	sprintf(acTemp,"%d",pstTr69cAcsInfo->UDPConnReqAddrNotifynLimit);
    *value =strdup(acTemp);
    free(pstTr69cAcsInfo); */
    return TRX_OK;
}

/* Enables or disables the use of STUN by the CPE.
This applies only to the use of STUN in association
with the ACS to allow UDP Connection Requests. */
TRX_STATUS setSTUNEnable(const char *value)
{    
    /* OM_TR69C_APP_PARAM_T *pstTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(pstTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    pstTr69cAcsInfo->eSetFlag = TR069_STUN_ENABLE;
    pstTr69cAcsInfo->iSTUNEnable = atoi(value);
    if (g_debug)
        fprintf(stderr, "stTr69cAcsInfo->iSTUNEnable=%d\n", pstTr69cAcsInfo->iSTUNEnable);
    if(OM_E_NONE != om_tr69c_set_acsinfo(pstTr69cAcsInfo))
    {
        free(pstTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    free(pstTr69cAcsInfo);   */ 

    return TRX_OK;
}

TRX_STATUS getSTUNEnable(char **value)
{    
    /* CHAR acTemp[CHAR_LEN_32] = {0}; 
    
    OM_TR69C_APP_PARAM_T *pstTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(pstTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
	memset(acTemp, 0, sizeof(acTemp));
    pstTr69cAcsInfo->eSetFlag = TR069_STUN_ENABLE;
    if(OM_E_NONE != om_tr69c_get_acsinfo(pstTr69cAcsInfo))
    {
        free(pstTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    sprintf(acTemp,"%d",pstTr69cAcsInfo->iSTUNEnable);
    *value =strdup(acTemp);
    free(pstTr69cAcsInfo); */
    return TRX_OK;
}

/* Enables or disables the use of STUN by the CPE.
This applies only to the use of STUN in association
with the ACS to allow UDP Connection Requests. */
TRX_STATUS setSTUNServerAddress(const char *value)
{    
    /* OM_TR69C_APP_PARAM_T *pstTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(pstTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    pstTr69cAcsInfo->eSetFlag = TR069_STUN_SERVER_ADDR;
    strncpy(pstTr69cAcsInfo->acSTUNSvr,value,sizeof(pstTr69cAcsInfo->acSTUNSvr));
    if (g_debug)
        fprintf(stderr, "stTr69cAcsInfo->acSTUNSvr=%s\n", pstTr69cAcsInfo->acSTUNSvr);
    if(OM_E_NONE != om_tr69c_set_acsinfo(pstTr69cAcsInfo))
    {
        free(pstTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    free(pstTr69cAcsInfo);   */ 

    return TRX_OK;
}

TRX_STATUS getSTUNServerAddress(char **value)
{        
    /* OM_TR69C_APP_PARAM_T *pstTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(pstTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    pstTr69cAcsInfo->eSetFlag = TR069_STUN_SERVER_ADDR;
    if(OM_E_NONE != om_tr69c_get_acsinfo(pstTr69cAcsInfo))
    {
        free(pstTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    *value =strdup(pstTr69cAcsInfo->acSTUNSvr);
    free(pstTr69cAcsInfo); */
    return TRX_OK;
}


/* Enables or disables the use of STUN by the CPE.
This applies only to the use of STUN in association
with the ACS to allow UDP Connection Requests. */
TRX_STATUS setSTUNServerPort(const char *value)
{    
    /* OM_TR69C_APP_PARAM_T *pstTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(pstTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    pstTr69cAcsInfo->eSetFlag = TR069_STUN_SERVER_PORT;
    pstTr69cAcsInfo->uiSTUNPort = atoi(value);
    if (g_debug)
        fprintf(stderr, "stTr69cAcsInfo->uiSTUNPort=%d\n", pstTr69cAcsInfo->uiSTUNPort);
    if(OM_E_NONE != om_tr69c_set_acsinfo(pstTr69cAcsInfo))
    {
        free(pstTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    free(pstTr69cAcsInfo);  */  

    return TRX_OK;
}

TRX_STATUS getSTUNServerPort(char **value)
{    
   /*  CHAR acTemp[CHAR_LEN_32] = {0}; 
    
    OM_TR69C_APP_PARAM_T *pstTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(pstTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    pstTr69cAcsInfo->eSetFlag = TR069_STUN_SERVER_PORT;
    if(OM_E_NONE != om_tr69c_get_acsinfo(pstTr69cAcsInfo))
    {
        free(pstTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    sprintf(acTemp,"%d",pstTr69cAcsInfo->uiSTUNPort);
    *value =strdup(acTemp);
    free(pstTr69cAcsInfo); */
    return TRX_OK;
}

/* Enables or disables the use of STUN by the CPE.
This applies only to the use of STUN in association
with the ACS to allow UDP Connection Requests. */
TRX_STATUS setSTUNUsername(const char *value)
{    
    /* OM_TR69C_APP_PARAM_T *pstTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(pstTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    pstTr69cAcsInfo->eSetFlag = TR069_STUN_USERNAME;
    strncpy(pstTr69cAcsInfo->acSTUNUsername,value,sizeof(pstTr69cAcsInfo->acSTUNUsername));
    if (g_debug)
        fprintf(stderr, "stTr69cAcsInfo->acSTUNUsername=%s\n", pstTr69cAcsInfo->acSTUNUsername);
    if(OM_E_NONE != om_tr69c_set_acsinfo(pstTr69cAcsInfo))
    {
        free(pstTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    free(pstTr69cAcsInfo);    */

    return TRX_OK;
}

TRX_STATUS getSTUNUsername(char **value)
{    
    /* OM_TR69C_APP_PARAM_T *pstTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(pstTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    pstTr69cAcsInfo->eSetFlag = TR069_STUN_USERNAME;
    if(OM_E_NONE != om_tr69c_get_acsinfo(pstTr69cAcsInfo))
    {
        free(pstTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    *value =strdup(pstTr69cAcsInfo->acSTUNUsername);
    free(pstTr69cAcsInfo);    */ 
    
    return TRX_OK;
}

/* Enables or disables the use of STUN by the CPE.
This applies only to the use of STUN in association
with the ACS to allow UDP Connection Requests. */
TRX_STATUS setSTUNPassword(const char *value)
{    
   /*  OM_TR69C_APP_PARAM_T *pstTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(pstTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    pstTr69cAcsInfo->eSetFlag = TR069_STUN_PASSWD;
    strncpy(pstTr69cAcsInfo->acSTUNPasswd,value,sizeof(pstTr69cAcsInfo->acSTUNPasswd));
    if (g_debug)
        fprintf(stderr, "stTr69cAcsInfo->acSTUNPasswd=%s\n", pstTr69cAcsInfo->acSTUNPasswd);
    if(OM_E_NONE != om_tr69c_set_acsinfo(pstTr69cAcsInfo))
    {
        free(pstTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    free(pstTr69cAcsInfo);    */
    return TRX_OK;
}

/* Enables or disables the use of STUN by the CPE.
This applies only to the use of STUN in association
with the ACS to allow UDP Connection Requests. */
TRX_STATUS getSTUNPassword(char **value)
{    
   /*  OM_TR69C_APP_PARAM_T *pstTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(pstTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    pstTr69cAcsInfo->eSetFlag = TR069_STUN_PASSWD;
    if(OM_E_NONE != om_tr69c_get_acsinfo(pstTr69cAcsInfo))
    {
        free(pstTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    *value =strdup(pstTr69cAcsInfo->acSTUNPasswd);
    free(pstTr69cAcsInfo);   */
    return TRX_OK;
}

/* If STUN Is enabled, the maximum period, in
   seconds, that STUN Binding Requests MUST be
   sent by the CPE for the purpose of maintaining the
   binding in the Gateway. This applies specifically to
   Binding Requests sent from the UDP Connection
   Request address and port.
   A value of -1 indicates that no maximum period is
   specified. */
TRX_STATUS setSTUNMaximumKeepAlivePeriod(const char *value)
{    
   /*  OM_TR69C_APP_PARAM_T *pstTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(pstTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    pstTr69cAcsInfo->eSetFlag = TR069_STUN_KA_TIME_MAX;
    pstTr69cAcsInfo->iSTUNMaxKaPeriod = atoi(value);
    if (g_debug)
        fprintf(stderr, "stTr69cAcsInfo->uiSTUNPort=%d\n", pstTr69cAcsInfo->iSTUNMaxKaPeriod);
    if(OM_E_NONE != om_tr69c_set_acsinfo(pstTr69cAcsInfo))
    {
        free(pstTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    free(pstTr69cAcsInfo);   */
    
    return TRX_OK;
}

TRX_STATUS getSTUNMaximumKeepAlivePeriod(char **value)
{  /*   
    CHAR acTemp[CHAR_LEN_32] = {0}; 
    
    OM_TR69C_APP_PARAM_T *pstTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(pstTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
	memset(acTemp, 0, sizeof(acTemp));
    pstTr69cAcsInfo->eSetFlag = TR069_STUN_KA_TIME_MAX;
    if(OM_E_NONE != om_tr69c_get_acsinfo(pstTr69cAcsInfo))
    {
        free(pstTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    sprintf(acTemp,"%d",pstTr69cAcsInfo->iSTUNMaxKaPeriod);
    *value =strdup(acTemp);
    free(pstTr69cAcsInfo); */
    return TRX_OK;
}

TRX_STATUS setSTUNMinimumKeepAlivePeriod(const char *value)
{    
   /*  OM_TR69C_APP_PARAM_T *pstTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(pstTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    pstTr69cAcsInfo->eSetFlag = TR069_STUN_KA_TIME_MIN;
    pstTr69cAcsInfo->uiSTUNMinKaPeriod = atoi(value);
    if (g_debug)
        fprintf(stderr, "stTr69cAcsInfo->uiSTUNMinKaPeriod=%d\n", pstTr69cAcsInfo->uiSTUNMinKaPeriod);
    if(OM_E_NONE != om_tr69c_set_acsinfo(pstTr69cAcsInfo))
    {
        free(pstTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    free(pstTr69cAcsInfo);  */
    return TRX_OK;
}

TRX_STATUS getSTUNMinimumKeepAlivePeriod(char **value)
{   /*  
    CHAR acTemp[CHAR_LEN_32] = {0}; 
    
    OM_TR69C_APP_PARAM_T *pstTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(pstTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
	memset(acTemp, 0, sizeof(acTemp));
    pstTr69cAcsInfo->eSetFlag = TR069_STUN_KA_TIME_MIN;
    if(OM_E_NONE != om_tr69c_get_acsinfo(pstTr69cAcsInfo))
    {
        free(pstTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    sprintf(acTemp,"%d",pstTr69cAcsInfo->uiSTUNMinKaPeriod);
    *value =strdup(acTemp);
    free(pstTr69cAcsInfo); */
    return TRX_OK;
}

/* A True value indicates that the received MAPPED
   ADDRESS in the most recent Binding Response
   differs from the CPE’s source address and port.
   When STUNEnable is False, this value MUST be
   False. */
TRX_STATUS getNATDetected(char **value)
{  /*   
    CHAR acTemp[CHAR_LEN_32] = {0}; 
    
    OM_TR69C_APP_PARAM_T *pstTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(pstTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
	memset(acTemp, 0, sizeof(acTemp));
    pstTr69cAcsInfo->eSetFlag = TR069_NAT_DETECTED;
    if(OM_E_NONE != om_tr69c_get_acsinfo(pstTr69cAcsInfo))
    {
        free(pstTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    sprintf(acTemp,"%d",pstTr69cAcsInfo->iNATDetected);
    *value =strdup(acTemp);
    free(pstTr69cAcsInfo); */
    return TRX_OK;
}
void getLetstateinfo(void)
{
printf("get lte state info 0!\n");
	/* OM_LTEInfoState ltestate={0};
    if(0 != om_lte_tr69_get_ltestate(&ltestate))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d]   ERROR",__func__,__LINE__);        

    }
	printf("get lte state info 1!\n");
	lteinfostate.singleLevel = ltestate.singleLevel;
	lteinfostate.FrequencyPoint=ltestate.FrequencyPoint;
	lteinfostate.band=ltestate.band;
	lteinfostate.PhysicsCellId=ltestate.PhysicsCellId;
	lteinfostate.CMCC_RSRP=ltestate.CMCC_RSRP;
	lteinfostate.CMCC_RSRQ=ltestate.CMCC_RSRQ;
	lteinfostate.CMCC_SINR=ltestate.CMCC_SINR;
	printf("get lte state info 2!\n");
	memcpy(lteinfostate.X_CMCC_ICCID,ltestate.X_CMCC_ICCID,sizeof(ltestate.X_CMCC_ICCID));
	memcpy(lteinfostate.aucModuleType,ltestate.aucModuleType,sizeof(ltestate.aucModuleType));
	memcpy(lteinfostate.aucModuleVer,ltestate.aucModuleVer,sizeof(ltestate.aucModuleVer));
	memcpy(lteinfostate.aucModuleIMEI,ltestate.aucModuleIMEI,sizeof(ltestate.aucModuleIMEI));
	memcpy(lteinfostate.aucDeviceMAC,ltestate.aucDeviceMAC,sizeof(ltestate.aucDeviceMAC));

	memcpy(lteinfostate.aucEnodebId,ltestate.aucEnodebId,sizeof(ltestate.aucEnodebId));
	memcpy(lteinfostate.aucCellID,ltestate.aucCellID,sizeof(ltestate.aucCellID));

	memcpy(lteinfostate.aucLcid,ltestate.aucLcid,sizeof(ltestate.aucLcid));
	memcpy(lteinfostate.aucTac,ltestate.aucTac,sizeof(ltestate.aucTac));
	
	printf("get lte state info !!!!!\n");
	printf("get aucModuleIMEI =%s !!!!!\n",lteinfostate.aucModuleIMEI);
	printf("get aucEnodebId =%s !!!!!\n",lteinfostate.aucEnodebId);
	printf("get aucCellID =%s !!!!!\n",lteinfostate.aucCellID);
	printf("get aucLcid =%s !!!!!\n",lteinfostate.aucLcid);
	printf("get aucTac =%s !!!!!\n",lteinfostate.aucTac); */
	return ;
}
void retrieveTR69StatusItems(void) {
	
     /*OM_TR69C_APP_PARAM_T *pstTr69cAcsInfo = calloc(1, sizeof(OM_TR69C_APP_PARAM_T));
    UINT32 iLoop;

    memset(pstTr69cAcsInfo,0,sizeof(OM_TR69C_APP_PARAM_T));

    // 循环读取需要从NVRAM中获取的数据 
    for (iLoop = TR069_NEW_PARAMETERKEY; iLoop <= TR069_DOWNLOAD_END_TIME; iLoop++)
    {
        pstTr69cAcsInfo->eSetFlag = iLoop;
        if(OM_E_NONE != om_tr69c_get_acsinfo(pstTr69cAcsInfo))
        {
            slog(TR69C_LOG_ERR,"[tr69c] get enum from OM_NVRAM_LIST_E %d ERROR\n", iLoop);
            //return TRX_ERR;
        }
    }

	if ( acsState.downloadCommandKey ) {
		free(acsState.downloadCommandKey);
		acsState.downloadCommandKey = NULL;
	}
	if ( pstTr69cAcsInfo->downloadCommandKey )
		acsState.downloadCommandKey = strdup(pstTr69cAcsInfo->downloadCommandKey);

	if ( acsState.rebootCommandKey ) {
		free(acsState.rebootCommandKey);
		acsState.rebootCommandKey = NULL;
	}
	if ( pstTr69cAcsInfo->rebootCommandKey )
		acsState.rebootCommandKey = strdup(pstTr69cAcsInfo->rebootCommandKey);

	if ( acsState.newParameterKey ) {
		free(acsState.newParameterKey);
		acsState.newParameterKey = NULL;
	}
	if ( pstTr69cAcsInfo->newParameterKey )
		acsState.parameterKey = strdup(pstTr69cAcsInfo->newParameterKey);
	acsState.dlFaultStatus = pstTr69cAcsInfo->dlFaultStatus;
	acsState.startDLTime = pstTr69cAcsInfo->startDLTime;
	acsState.endDLTime = pstTr69cAcsInfo->endDLTime;
	if ( acsState.dlFaultMsg ) {
		free(acsState.dlFaultMsg);
		acsState.dlFaultMsg = NULL;
	}
	if ( pstTr69cAcsInfo->dlFaultMsg )
		acsState.dlFaultMsg = strdup(pstTr69cAcsInfo->dlFaultMsg);

//add by duanguiyuan 20160318 begin
		acsState.retryCount = 0;
		pstTr69cAcsInfo->eSetFlag = TR069_ACS_CONNECT_STATE;
        if(OM_E_NONE != om_tr69c_get_acsinfo(pstTr69cAcsInfo))
        {
            slog(TR69C_LOG_ERR,"[tr69c] get enum from OM_NVRAM_LIST_E %d ERROR\n", iLoop);
            //return TRX_ERR;
        }
		//acsContactedState = pstTr69cAcsInfo->contactedState;
		if(pstTr69cAcsInfo->contactedState == 1)
		{
			informState = eACSInformed;
		}
		else
		{
			informState = eJustBooted;
		}
		
	//add by duanguiyuan 20160318 end
	// add by duanguiyuan 20160328 获取一下lte的状态信息 用于组织报文
	printf("send getLetstateinfo!!!!\n");
	getLetstateinfo();
	//add bu duanguiyuan 20160328 判断设备是不是 第一次启动 
	INT32 uiUpTime = 0;
   	// 获取设备上电时间 
    if(AP_E_NONE != app_get_up_time(&uiUpTime))
    {
        slog(TR69C_LOG_ERR, "get up time failed");
    }
	
	printf("app_get_up_time = %d !!!!!!!!!!!!\n",uiUpTime);
	if(uiUpTime >= 2000){
		g_device_first_up = 0;
	}else{
		g_device_first_up = 1;
	}
	//add bu duanguiyuan 20160328
	printf("send getLetstateinfo !!!!\n");
		free(pstTr69cAcsInfo); */
	/*char* value=NULL;
	char *acTauConfig=NULL;
	if(UCI_OK != uci_config_get(UCI_RETRIEVETR69STATUSITEMS_DCKEY,acTauConfig))
	{
		*value = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
	}
	else{
		*value = strdup(acTauConfig);
		
	}
	if(acsState.downloadCommandKey)
	{
		free(acsState.downloadCommandKey);
		acsState.downloadCommandKey = NULL;
	}
	if(value)
	{
		acsState.downloadCommandKey=value;
		value=NULL;
	}*/
	char acBuf[TAUARRY_LEN]={0};
	char *value=NULL;
	if(UCI_OK != uci_config_get(UCI_RETRIEVETR69STATUSITEMS_DCKEY,acBuf))
	{
		value = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
       // return ;
	}
	else{
		value = strdup(acBuf);
		
	}	
	if(acsState.downloadCommandKey)
	{
		free(acsState.downloadCommandKey);
		acsState.downloadCommandKey = NULL;
	}
	if(value)
	{
		acsState.downloadCommandKey=value;
		value=NULL;
	}
	
	memset(acBuf,0,sizeof(acBuf));
	if(UCI_OK != uci_config_get(UCI_RETRIEVETR69STATUSITEMS_REBOOTCONMANDKEY,acBuf))
	{
		value = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        //return ;
	}
	else{
		value = strdup(acBuf);
		
	}	
	if(acsState.rebootCommandKey)
	{
		free(acsState.rebootCommandKey);
		acsState.rebootCommandKey = NULL;
	}
	if(value)
	{
		acsState.rebootCommandKey=value;
		value=NULL;
	}
	

	memset(acBuf,0,sizeof(acBuf));
	if(UCI_OK != uci_config_get(UCI_RETRIEVETR69STATUSITEMS_NEWPARAMETERKEY,acBuf))
	{
		value = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        //return ;
	}
	else{
		value = strdup(acBuf);
		
	}	
	if(acsState.newParameterKey)
	{
		free(acsState.newParameterKey);
		acsState.newParameterKey = NULL;
	}
	if(value)
	{
		acsState.newParameterKey=value;
		value=NULL;
	}

	

	memset(acBuf,0,sizeof(acBuf));
	if(UCI_OK != uci_config_get(UCI_RETRIEVETR69STATUSITEMS_DLFAULTSTATUS,acBuf))
	{
		value = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        //return ;
	}
	else
    {
		value = strdup(acBuf);
	}
	if(value)
	{
		acsState.dlFaultStatus=atoi(value);
		free(value);
		value=NULL;
	}



	memset(acBuf,0,sizeof(acBuf));
	if(UCI_OK != uci_config_get(UCI_RETRIEVETR69STATUSITEMS_STARTDLTIME,acBuf))
	{
		value = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
       // return ;
	}
	else
    {
		value = strdup(acBuf);
	}
	if(value)
	{
		acsState.startDLTime=atol(value);
		free(value);
		value=NULL;
	}
	


	memset(acBuf,0,sizeof(acBuf));
	if(UCI_OK != uci_config_get(UCI_RETRIEVETR69STATUSITEMS_ENDDLTIME,acBuf))
	{
		value = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        //return ;
	}
	else
    {
		value = strdup(acBuf);
	}
	if(value)
	{
		acsState.endDLTime=atol(value);
		free(value);
		value=NULL;
	}
	

	memset(acBuf,0,sizeof(acBuf));
	if(UCI_OK != uci_config_get(UCI_RETRIEVETR69STATUSITEMS_DLFAULTMSG,acBuf))
	{
		value = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
       // return ;
	}
	else{
		value = strdup(acBuf);
		
	}	
	if(acsState.dlFaultMsg)
	{
		free(acsState.dlFaultMsg);
		acsState.dlFaultMsg = NULL;
	}
	if(value)
	{
		acsState.dlFaultMsg=value;
		value=NULL;
	}
	
	
	memset(acBuf,0,sizeof(acBuf));
	if(UCI_OK != uci_config_get(UCI_RETRIEVETR69STATUSITEMS_INFORMSTATE,acBuf))
	{
		value = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
       // return ;
	}
	else
    {
		value = strdup(acBuf);
	}
	if(value)
	{
		if(1==atoi(value))
		{
			informState = eACSInformed;
		}
		else
		{
			informState = eJustBooted;
		}
		free(value);
		value=NULL;
		
	}	
	 //acsState.downloadCommandKey=strdup("downkey");
	 //acsState.rebootCommandKey=strdup("rebootkey");
	 //acsState.newParameterKey=strdup("paramkey");
	// acsState.dlFaultStatus     = 1;
	 //acsState.startDLTime       =1;
	// acsState.endDLTime			=1;
	 //acsState.dlFaultMsg =strdup("msg");
	 //informState = eJustBooted;

	 g_device_first_up = 1;
}

void initManagementServerInstances(void)
{
    TRxObjNode *n;
    int id;
    InstanceDesc  *mgmtSrvIdp;
    
    n = mgmtServerDesc;
    id = 1;
    mgmtSrvIdp = getNewInstanceDesc(n, NULL, id);
}
/* change by duanguiyuan 20161117 修改参数节点 */
void initInstances()
{
    lastInstanceId = 0;  
	/* start at 10 for testing */
	initDeviceInfoInstances();
	initLANDeviceInstances();
    initWANDeviceInstances();
	initTimeInstances();
	initManagementServerInstances();
    initIPPingDiagnosticsInstances();
	initX_ATP_SystemMonitorInstances();	
	initFreqLockInstances();	
	initX_ATP_VxlanInstances();
	initX_ATP_L2GreInstances();
	initX_ATP_FireWallInstances();
	initX_ATP_VPNInstances();
	initX_ATP_AlarmInfoInstances();
	initX_ATP_ConfigInstances();
	initServicesInstances();
#if 0
	initDeviceConfigInstances();
	
	initGatewayUserInterfaceInstances();
	initX_ATP_PreserveDataInstances();
	initX_ATP_EXTInstances();
	initLayer3ForwardingInstances();
	initX_ATP_ModemLogInstances();
	initX_DeviceInfoInstances();
	initLANConfigSecurityInstances();
   	initTraceRouteDiagnosticsInstances();
	/* PPPoE诊断*/
	/*	altered by caibing 11-15 	*/
	initX_PPPOE_EmulatorInstances();
	initX_LteDeviceInstances();
	initX_PPPoE2L2TPInstances();
#endif
slog(TR69C_LOG_DEBUG,"start instances end");
}

void reInitInstances()
{
}

TRX_STATUS setMSrvrInformEnable(const char *value)
{
	return setCWMPEnable(value);
}

extern time_t startTime;
void initBCMLibIF(void)
{
    char *value=NULL;
	
    slog(TR69C_LOG_DEBUG,"start initBCMLibIF");
    
    /* init start time */
    startTime = time(NULL);
	slog(TR69C_LOG_DEBUG,"start initBCMLibIF 0");
	getUpgradesManaged(&value);
	if ( value != NULL )
	{
		 free(value);
		 value=NULL;
	}
	slog(TR69C_LOG_DEBUG,"start initBCMLibIF 1");
    getConnectionRequestURL(&value);
	if ( value != NULL ) 
	{
		 free(value);
		 value=NULL;
	}
	slog(TR69C_LOG_DEBUG,"start initBCMLibIF 2");
	getProvisioningCode(&value);
	if ( value != NULL )
	{
		 free(value);
		 value=NULL;
	}
	/* add by duanguiyuan 20161126 添加获取告警上报的信息 */
	getalarmvlueInit();

	slog(TR69C_LOG_DEBUG,"start retrieveTR69StatusItems");
	retrieveTR69StatusItems();
	slog(TR69C_LOG_DEBUG,"start instances");
    initInstances();    /* 初始化实例 */
	slog(TR69C_LOG_DEBUG,"start initBCMLibIF   end ");
}

unsigned int uiConnectState = 0;/* 接受ACS连接请求情况 */
