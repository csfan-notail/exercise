/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : lteWrapper.c
  版 本 号   : 初稿
  作    者   : zhangjiasi
  生成日期   : 2015年6月9日
  最近修改   :
  功能描述   : 所有与lte相关并为GBCOM封装的接口
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
#include "lteDeviceParams.h"

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
extern int g_log_level;
extern TRxObjNode  x_lteDeviceDesc[];
extern TRxObjNode  lteDeviceX_LteDeviceDesc[];



TRX_STATUS getX_LteDevice_RadioSignalStrength(char **value)
{
    /* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};
    CHAR szTemp[CHAR_LEN_32] = {0};

    stLteState.eFlag = TR69C_LTE_RADIO_SIGNAL_STRENGTH;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    sprintf(szTemp,"%d",stLteState.stLteState.RadioSignalStrength);

    *value = strdup(szTemp); */
    return TRX_OK;
}

TRX_STATUS getX_LteDevice_Txpower(char **value)
{
    /* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};
    CHAR szTemp[CHAR_LEN_32] = {0};

    stLteState.eFlag = TR69C_LTE_TXPOWER;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    sprintf(szTemp,"%d",stLteState.stLteState.Txpower);

    *value = strdup(szTemp); */
    return TRX_OK;
}

TRX_STATUS getX_LteDevice_IMEI(char **value)
{
    /* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};

    stLteState.eFlag = TR69C_LTE_IMEI;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    *value = strdup(stLteState.stLteState.IMEI); */
    return TRX_OK;
}

TRX_STATUS getX_LteDevice_IMSI(char **value)
{
   /*  OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};

    stLteState.eFlag = TR69C_LTE_IMSI;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    *value = strdup(stLteState.stLteState.IMSI); */
    return TRX_OK;
}

TRX_STATUS getX_LteDevice_RSRP(char **value)
{
    /* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};
    CHAR szTemp[CHAR_LEN_32] = {0};

    stLteState.eFlag = TR69C_LTE_RSRP;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    sprintf(szTemp,"%d",stLteState.stLteState.RSRP);

    *value = strdup(szTemp); */
    return TRX_OK;
}

TRX_STATUS getX_LteDevice_RSRQ(char **value)
{
    /* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};
    CHAR szTemp[CHAR_LEN_32] = {0};

    stLteState.eFlag = TR69C_LTE_RSRQ;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    sprintf(szTemp,"%d",stLteState.stLteState.RSRQ);

    *value = strdup(szTemp); */
    return TRX_OK;
}

TRX_STATUS getX_LteDevice_SINR(char **value)
{
    /* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};
    CHAR szTemp[CHAR_LEN_32] = {0};

    stLteState.eFlag = TR69C_LTE_SINR;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    sprintf(szTemp,"%d",stLteState.stLteState.SINR);

    *value = strdup(szTemp); */
    return TRX_OK;
}

TRX_STATUS getX_LteDevice_Band(char **value)
{
    /* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};

    stLteState.eFlag = TR69C_LTE_BAND;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    *value = strdup(stLteState.stLteState.Band); */
    return TRX_OK;
}

TRX_STATUS getX_LteDevice_BandWidth(char **value)
{
    /* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};
    CHAR szTemp[CHAR_LEN_32] = {0};

    stLteState.eFlag = TR69C_LTE_BANDWIDTH;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    sprintf(szTemp,"%d",stLteState.stLteState.BandWidth);

    *value = strdup(szTemp); */
    return TRX_OK;
}

TRX_STATUS getX_LteDevice_SIMSTATUS(char **value)
{
    /* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};

    stLteState.eFlag = TR69C_LTE_SIM_STATE;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    *value = strdup(stLteState.stLteState.SIMState); */
    return TRX_OK;
}

TRX_STATUS getX_LteDevice_RadioMode(char **value)
{
    /* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};

    stLteState.eFlag = TR69C_LTE_RADIO_MODE;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    *value = strdup(stLteState.stLteState.RadioMode); */
    return TRX_OK;
}

TRX_STATUS getX_LteDevice_CellLocking(char **value)
{
   /*  OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};

    stLteState.eFlag = TR69C_LTE_CELL_LOCKING;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    *value = strdup(stLteState.stLteState.CellLocking); */
    return TRX_OK;
}


TRX_STATUS getX_LteDevice_FrequencyLocking(char **value)
{
   /*  OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};

    stLteState.eFlag = TR69C_LTE_FREQUENCY_LOCKING;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    *value = strdup(stLteState.stLteState.FrequencyLocking); */
    return TRX_OK;
}

TRX_STATUS getX_LteDevice_RadioFrenquency(char **value)
{
  /*   OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};

    stLteState.eFlag = TR69C_LTE_RADIO_FRENQUENCY;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    *value = strdup(stLteState.stLteState.RadioFrenquency); */
    return TRX_OK;
}

TRX_STATUS getX_LteDevice_RSSI(char **value)
{
   /*  OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};
    CHAR szTemp[CHAR_LEN_32] = {0};

    stLteState.eFlag = TR69C_LTE_RSSI;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    sprintf(szTemp,"%d",stLteState.stLteState.RSSI);

    *value = strdup(szTemp); */
    return TRX_OK;
}

TRX_STATUS getX_LteDevice_RegistrationStatus(char **value)
{
   /*  OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};

    stLteState.eFlag = TR69C_LTE_REGISTRATION_STATUS;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    *value = strdup(stLteState.stLteState.RegistrationStatus); */
    return TRX_OK;
}

TRX_STATUS getX_LteDevice_NetRegStatus(char **value)
{
    *value = strdup("");
    return TRX_OK;
}

TRX_STATUS getX_LteDevice_GprsRegStatus(char **value)
{
    *value = strdup("");
    return TRX_OK;
}

TRX_STATUS getX_LteDevice_EpsRegStatus(char **value)
{
    *value = strdup("");
    return TRX_OK;
}

TRX_STATUS getX_LteDevice_RadioInterference(char **value)
{
    *value = strdup("");
    return TRX_OK;
}

TRX_STATUS getX_LteDevice_NetworkName(char **value)
{
    /* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};

    stLteState.eFlag = TR69C_LTE_NETWORK_NAME;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    *value = strdup(stLteState.stLteState.NetworkName); */
    return TRX_OK;
}

TRX_STATUS getX_LteDevice_EnodeBId(char **value)
{
    /* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};

    stLteState.eFlag = TR69C_LTE_ENODEBID;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    *value = strdup(stLteState.stLteState.EnodeBId); */
    return TRX_OK;
}

TRX_STATUS getX_LteDevice_GlobeCellId(char **value)
{
    /* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};

    stLteState.eFlag = TR69C_LTE_GLOBE_CELLID;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    *value = strdup(stLteState.stLteState.GlobeCellId); */
    return TRX_OK;
}

TRX_STATUS getX_LteDevice_PhysicsCellId(char **value)
{
   /*  OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};

    stLteState.eFlag = TR69C_LTE_PHYSICS_CELLID;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    *value = strdup(stLteState.stLteState.PhysicsCellId); */
    return TRX_OK;
}

TRX_STATUS getX_LteDevice_Iccid(char **value)
{
    /* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};

    stLteState.eFlag = TR69C_LTE_ICCID;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    *value = strdup(stLteState.stLteState.Iccid); */
    return TRX_OK;
}

TRX_STATUS getX_LteDevice_CellID(char **value)
{
   /*  OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};

    stLteState.eFlag = TR69C_LTE_CELLID;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    *value = strdup(stLteState.stLteState.CellID); */
    return TRX_OK;
}

TRX_STATUS getX_LteDevice_APNStatus(char **value)
{
    /* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};

    stLteState.eFlag = TR69C_LTE_APN_STATUS;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    *value = strdup(stLteState.stLteState.APNStatus); */
    return TRX_OK;
}

TRX_STATUS getX_LteDevice_InNetWorkState(char **value)
{
    /* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};
	 CHAR szTemp[CHAR_LEN_32] = {0};
    stLteState.eFlag = TR69C_LTE_IN_NETWORK_STATE;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }
	 sprintf(szTemp,"%d",stLteState.stLteState.InNetWorkState);
    *value = strdup(szTemp); */
    return TRX_OK;
}

/*
	TRxObjNode *n;
    int id;
    InstanceDesc  *IPPingDiagnosticsIdp;
    
    n = IPPingDiagnosticsDesc;
    id = 1;
    IPPingDiagnosticsIdp = getNewInstanceDesc(n, NULL, id);
*/
void initX_LteDeviceInstances()
{
    TRxObjNode *n;
    int id;
    InstanceDesc  *x_lteDeviceIdp;
	
    InstanceDesc  *lteDeviceX_lteDeviceIdp;
	
slog(TR69C_LOG_DEBUG,"start X_LteDeviceInstances 0");

    n = x_lteDeviceDesc;
    id = 1;
    x_lteDeviceIdp = getNewInstanceDesc(n, NULL, id);
	
slog(TR69C_LOG_DEBUG,"start X_LteDeviceInstances 1");  

    n = lteDeviceX_LteDeviceDesc;
    lteDeviceX_lteDeviceIdp = getNewInstanceDesc(n, x_lteDeviceIdp, id);
	
slog(TR69C_LOG_DEBUG,"start X_LteDeviceInstances 2");
}

