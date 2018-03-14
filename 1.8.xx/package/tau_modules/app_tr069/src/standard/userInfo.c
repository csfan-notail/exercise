/*
 * Copyright (C) GBCOM ltd.,co 2015
 *
 * This is a part of project tr69cd, which implements TR069 protocal(aka
 * CWMP).
 * 
 * This is a PRIVATE software.
 * All rights reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include "utils.h"

#include "userInfo.h"
#include "deviceInfoParams.h"
#include "ap_types.h"
//#include "pal/app/pal_app.h"
#include "om_tr69c.h"

/*
#include "ap_types.h"
#include "om/om_tr69c.h"
#include "om/om_log.h"
#include "manageServerParams.h"
#include "SOAPParser/RPCState.h"
#include "informer.h"
#include "sysWrapper.h"
*/
extern int g_log_level;
extern ACSState    acsState;

TRX_STATUS setUserInfoPassword(const char *value)
{
	/* OM_TR69C_APP_PARAM_T *pstTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(pstTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    pstTr69cAcsInfo->eSetFlag = TR069_PASS_AUTH_DATE;
    strncpy(pstTr69cAcsInfo->passAuthdate,value,sizeof(pstTr69cAcsInfo->passAuthdate));
    if(OM_E_NONE != om_tr69c_set_acsinfo(pstTr69cAcsInfo))
    {
        free(pstTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
    }
    free(pstTr69cAcsInfo);    */
    return TRX_OK;
}
TRX_STATUS getUserInfoPassword(char **value)
{
	/* OM_TR69C_APP_PARAM_T *pstTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(pstTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    pstTr69cAcsInfo->eSetFlag = TR069_PASS_AUTH_DATE;
    if(OM_E_NONE != om_tr69c_get_acsinfo(pstTr69cAcsInfo))
    {
        free(pstTr69cAcsInfo);
        pstTr69cAcsInfo = NULL;
        slog(TR69C_LOG_ERR,"ERROR");
		*value =strdup("");
        return TRX_OK;
    }
   // acsState.passAuthflag= pstTr69cAcsInfo->passAuthFlag; 
    *value =strdup(pstTr69cAcsInfo->passAuthdate);
    free(pstTr69cAcsInfo);
    pstTr69cAcsInfo = NULL; */
    return TRX_OK;  
}

TRX_STATUS setUserInfoStatus(const char *value)
{	
/* printf("setUserInfoStatus = %s\n",value);
	int state = atoi(value);
	if(state == 0)
	{ //passoword认证成功 标记为 0
		acsState.passAuthflag = 0;
		OM_TR69C_APP_PARAM_T *pstTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
   	 	memset(pstTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    	pstTr69cAcsInfo->eSetFlag = TR069_PASS_AUTH_FLAG;
    	pstTr69cAcsInfo->passAuthFlag= 0;
    	if(OM_E_NONE != om_tr69c_set_acsinfo(pstTr69cAcsInfo))
    	{
        	free(pstTr69cAcsInfo);
        	slog(TR69C_LOG_ERR,"ERROR");
        	return TRX_ERR;
    	}
    	free(pstTr69cAcsInfo);  
	} */
    return TRX_OK;
}
TRX_STATUS getUserInfoStatus(char **value)
{
	*value = strdup("99");
	return TRX_OK;
}

TRX_STATUS setUserInfoLimit(const char *value)
{
	return TRX_OK;
}
TRX_STATUS getUserInfoLimit(char **value)
{
	*value = strdup("10");
	return TRX_OK;
}

TRX_STATUS setUserInfoTimes(const char *value)
{
	return TRX_OK;
}
TRX_STATUS getUserInfoTimes(char **value)
{
	*value = strdup("0");
	return TRX_OK;
}

TRX_STATUS setUserInfoResult(const char *value)
{
	return TRX_OK;
}
TRX_STATUS getUserInfoResult(char **value)
{
	*value = strdup("99");
	return TRX_OK;
}

TRX_STATUS setUserInfoServiceNum(const char *value)
{
	return TRX_OK;
}
TRX_STATUS getUserInfoServiceNum(char **value)
{
	*value = strdup("0");
	return TRX_OK;
}

TRX_STATUS setUserInfoServiceName(const char *value)
{
	return TRX_OK;
}
TRX_STATUS getUserInfoServiceName(char **value)
{
	*value = strdup("");
	return TRX_OK;
}




//altered by caibing
#if 0
TRxObjNode  userInfoDesc[] = {
	{Password,{{tString,10,0}}, setUserInfoPassword,getUserInfoPassword,NULL,NULL},
	{Status,{{tUnsigned,0,0}}, setUserInfoStatus,getUserInfoStatus,NULL,NULL},
	{Limit,{{tUnsigned,0,0}}, setUserInfoLimit,getUserInfoLimit,NULL,NULL},
	{Times,{{tUnsigned,0,0}}, setUserInfoTimes,getUserInfoTimes,NULL,NULL},
	{Result,{{tUnsigned,0,0}}, setUserInfoResult,getUserInfoResult,NULL,NULL},
	{ServiceNum,{{tUnsigned,0,0}}, setUserInfoServiceNum,getUserInfoServiceNum,NULL,NULL},
	{ServiceName,{{tString,64,0}}, setUserInfoServiceName,getUserInfoServiceName,NULL,NULL},
	
    {NULL}
};
#endif
