/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : deviceInfo.c
  版 本 号   : 初稿
  作    者   : duanguiyuan
  生成日期   : 2015年9月17日
  最近修改   :
  功能描述   : deviceInfo节点的函数实现
  函数列表   :
  修改历史   :
  1.日    期   : 2015年9月7日
    作    者   : duanguiyuan
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "rootParams.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include "utils.h"
#include "ap_types.h"
#include "om_tr69c.h" 

//#include "pal/app/pal_app.h"
//#include "om/om_lte.h" 

#include "informer.h"
#include "lteDeviceParams.h"
/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
extern int g_log_level;
extern LTEInfoState lteinfostate;
/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

/*

*/

TRX_STATUS getX_CMCC_WANDeviceNumberOfEntries(char **value)
{
    *value = strdup("1");
    return TRX_OK;
}
/*
CS网络注册状态
0:无服务
1:已注册，本地网络
2:未注册,但 ME 正在搜索要注册业务的新运营商
3:注册被拒绝
5:已注册,漫游
其他 :
*/
TRX_STATUS getX_CMCC_NetRegStatus(char **value)
{
    *value = strdup("1");
    return TRX_OK;
}
/*GPRS注册状态
0:无服务
1:已注册，本地网络
2:未注册,但 ME 正在搜索要注册业务的新运营商
3:注册被拒绝
5:已注册,漫游
其他 :

*/
TRX_STATUS getX_CMCC_GprsRegStatus(char **value)
{
    *value = strdup("1");
    return TRX_OK;
}
/*EPS注册状态
0:无服务
1:已注册，本地网络
2:未注册,但 ME 正在搜索要注册业务的新运营商
3:注册被拒绝
5:已注册,漫游
其他 :

*/
TRX_STATUS getX_CMCC_EpsRegStatus(char **value)
{
    *value = strdup("1");
    return TRX_OK;
}
/*
当前的网络模式:
*中国移动:46000
*中国移动:46002
*中国移动:46007
*/
TRX_STATUS getX_CMCC_CurrentNetwork(char **value)
{/*
	OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};

    stLteState.eFlag = TR69C_LTE_IN_NETWORK_STATE;
    if(0 != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        *value = strdup("0");
        return TRX_OK;
    }

    *value = strdup(stLteState.stLteState.InNetWorkState);
  	return TRX_OK;
  */
  *value = strdup("46002");
  return TRX_OK;

}
/*
当前注册的网络:
0 :GSM
1 :GSM Compact
2 :TD
7 :LTE
*/
TRX_STATUS getX_CMCC_CurrentNetmode(char **value)
{
/*
    OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};
	CHAR acTemp[CHAR_LEN_32] = {0};
    stLteState.eFlag = TR69C_LTE_IN_NETWORK_STATE;
    if(0 != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        *value = strdup("NONE");
        return TRX_OK;
    }
    sprintf(acTemp,"%d",stLteState.stLteState.InNetWorkState);

    *value = strdup(acTemp);
*/
 	*value = strdup("7");	
  	return TRX_OK;
}

/*
网络优先模式:
    LTE_NONE = 0x0000,
    LTE_2G_PRI = 0x0001,
    LTE_3G_PRI = 0x0002,
    LTE_4G_PRI = 0x0004,
    LTE_AUTO = 0x0008,
*/

TRX_STATUS getX_CMCC_NetworkPriority(char **value)
{
	/* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};
	CHAR acTemp[CHAR_LEN_32] = {0};

    stLteState.eFlag = TR69C_LTE_X_CMCC_NetworkPriority;
    if(0 != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        *value = strdup("0");
        return TRX_OK;
    }
    sprintf(acTemp,"%d",stLteState.stLteState.X_CMCC_NetworkPriority);
    *value = strdup(acTemp); */
  	return TRX_OK;
}
TRX_STATUS getX_CMCC_SingalLevel(char **value)
{
	CHAR acTemp[CHAR_LEN_32] = {0};
	if(lteinfostate.singleLevel == 0)
	{
		*value = strdup("N/A");
	}else{
		
		sprintf(acTemp,"%d",lteinfostate.singleLevel);
    	*value =strdup(acTemp);   
	} 
	return TRX_OK; 
}
TRX_STATUS getX_CMCC_Txpower(char **value)
{
    *value = strdup("23");
    return TRX_OK;
}
TRX_STATUS getX_CMCC_FrequencyPoint(char **value)
{
	/* CHAR acTemp[CHAR_LEN_32] = {0};
	if(lteinfostate.FrequencyPoint != 0)
	{
		sprintf(acTemp,"%d",lteinfostate.FrequencyPoint);
    	*value =strdup(acTemp);  
		return TRX_OK; 
	} 
	

    OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};
    stLteState.eFlag = TR69C_LTE_RADIO_FRENQUENCY;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        *value = strdup("0");
        return TRX_OK;
    }
	lteinfostate.FrequencyPoint = atoi(stLteState.stLteState.RadioFrenquency);
    *value = strdup(stLteState.stLteState.RadioFrenquency); */
  	return TRX_OK;

  
}

TRX_STATUS getX_CMCC_Band(char **value)
{
	/* CHAR acTemp[CHAR_LEN_32] = {0};
	if(lteinfostate.band != 0)
	{	
		sprintf(acTemp,"%d",lteinfostate.band);
    	*value =strdup(acTemp);  
		return TRX_OK; 
	}	  
    OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};
    stLteState.eFlag = TR69C_LTE_BAND;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        *value = strdup("0");
        return TRX_OK;
    }
	lteinfostate.band=atoi(stLteState.stLteState.Band);
    *value = strdup(stLteState.stLteState.Band); */
  	return TRX_OK;
}
TRX_STATUS getX_CMCC_BandWidth(char **value)
{
/*
	OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};

    stLteState.eFlag = TR69C_LTE_BANDWIDTH;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    *value = strdup(stLteState.stLteState.InNetWorkState);
   */
   	*value = strdup("40");
  	return TRX_OK;
}
TRX_STATUS getX_CMCC_EnodeBId(char **value)
{
	/* if(strlen(lteinfostate.aucEnodebId)>3)
	{
		 *value = strdup(lteinfostate.aucEnodebId);
		 return TRX_OK;
	}
    OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};

    stLteState.eFlag = TR69C_LTE_ENODEBID;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        *value = strdup("0");
        return TRX_OK;
    }
	
	memcpy(lteinfostate.aucEnodebId,stLteState.stLteState.EnodeBId,strlen(stLteState.stLteState.EnodeBId));
    *value = strdup(stLteState.stLteState.EnodeBId); */
  	return TRX_OK;
}
TRX_STATUS getX_CMCC_CellId(char **value)
{

	if(strlen(lteinfostate.aucLcid) !=3)
	{
		 *value = strdup(lteinfostate.aucLcid);
	}else{
		 *value = strdup("N/A");		 
	}
	 return TRX_OK;
/*   	OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};
	CHAR acTemp[CHAR_LEN_32] = {0};
	CHAR acTemp_str[CHAR_LEN_32] = {0};
	memset(acTemp, 0, sizeof(acTemp));
	memset(acTemp_str, 0, sizeof(acTemp_str));
	int i;
	int strlength;
	int add_flag = 0;
    stLteState.eFlag = TR69C_LTE_CELLID;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        *value = strdup("0");
        return TRX_OK;
    }
	memcpy(lteinfostate.aucCellID,stLteState.stLteState.CellID,strlen(stLteState.stLteState.CellID));
	memcpy(acTemp,stLteState.stLteState.CellID,strlen(stLteState.stLteState.CellID));
	 
	strlength = strlen(acTemp);
	slog(TR69C_LOG_ERR,"CellID = %s  length = %d ",acTemp,strlength);  
	
	acTemp_str[strlength+1]='\0';
	slog(TR69C_LOG_ERR,"acTemp_str = %s",acTemp_str);  
	*value = strdup(acTemp_str);
  	return TRX_OK;
 */
}
TRX_STATUS getX_CMCC_GlobeCellId(char **value)
{
	/* if(strlen(lteinfostate.aucTac)!=3)
	{
		 *value = strdup(lteinfostate.aucTac);
		 return TRX_OK;
	}
	OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};

    stLteState.eFlag = TR69C_LTE_GLOBE_CELLID;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        *value = strdup("0");
        return TRX_OK;
    }
	memcpy(lteinfostate.aucTac,stLteState.stLteState.GlobeCellId,strlen(stLteState.stLteState.GlobeCellId));
    *value = strdup(stLteState.stLteState.GlobeCellId); */
  	return TRX_OK;
}
TRX_STATUS getX_CMCC_PhysicsCellId(char **value)
{
	/* CHAR acTemp[CHAR_LEN_32] = {0};
	memset(acTemp, 0, sizeof(acTemp));
	if(lteinfostate.PhysicsCellId != 0)
	{
		sprintf(acTemp,"%d",lteinfostate.PhysicsCellId);
    	*value =strdup(acTemp);  
		return TRX_OK; 
	} 
	

     OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};
    stLteState.eFlag = TR69C_LTE_PHYSICS_CELLID;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        *value = strdup("0");
        return TRX_OK;
    }
	lteinfostate.PhysicsCellId = atoi(stLteState.stLteState.PhysicsCellId);
    *value = strdup(stLteState.stLteState.PhysicsCellId); */

  	return TRX_OK;
  	 
	
}
TRX_STATUS getX_CMCC_ICCID(char **value)
{
	/* CHAR acTemp[CHAR_LEN_32] = {0};
	memset(acTemp, 0, sizeof(acTemp));
	if( strlen(lteinfostate.X_CMCC_ICCID) > 3)
	{
		sprintf(acTemp,"%s",lteinfostate.X_CMCC_ICCID);
    	*value =strdup(acTemp);   
		return TRX_OK;
	}

   	OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};
    stLteState.eFlag = TR69C_LTE_ICCID;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        *value = strdup("N/A");
        return TRX_OK;
    }
	memcpy(lteinfostate.X_CMCC_ICCID,stLteState.stLteState.Iccid,strlen(stLteState.stLteState.Iccid));
    *value = strdup(stLteState.stLteState.Iccid); */
  	return TRX_OK; 
   
	
}
TRX_STATUS getX_CMCC_APN(char **value)
{
   /* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};

    stLteState.eFlag = TR69C_LTE_APN_STATUS;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        *value = strdup("N/A");
        return TRX_OK;
    }

    *value = strdup(stLteState.stLteState.APNStatus); */
  	return TRX_OK;
}
TRX_STATUS getX_CMCC_IsFrequencyLock(char **value)
{
	/* CHAR actmp[32];
    OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};
	memset(actmp, 0, sizeof(actmp));
    stLteState.eFlag = TR69C_LTE_FREQUENCY_ISLOCK;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        *value = strdup("0");
        return TRX_OK;
    }
	
	sprintf(actmp, "%d", stLteState.stLteState.IsFrequencyLock);
    *value = strdup(actmp); */
  	return TRX_OK;

}
TRX_STATUS setX_CMCC_IsFrequencyLock(const char *value)
{
  	/* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};
    stLteState.eFlag = TR69C_LTE_FREQUENCY_ISLOCK;
	//memcpy(stLteState.stLteState.FrequencyLocking,value,strlen(value));
	stLteState.stLteState.IsFrequencyLock = atoi(value);
    if(OM_E_NONE != om_lte_tr069_lte_state_set(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_OK;
    } */
  	return TRX_OK;

}
TRX_STATUS setX_CMCC_FrequencyLocking(const char *value)
{
  
/* 	OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};
    stLteState.eFlag = TR69C_LTE_FREQUENCY_LOCKING;
	memcpy(stLteState.stLteState.FrequencyLocking,value,strlen(value));
    if(OM_E_NONE != om_lte_tr069_lte_state_set(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_OK;
    } */
  	return TRX_OK;
}


TRX_STATUS getX_CMCC_FrequencyLocking(char **value)
{
    /* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};
    stLteState.eFlag = TR69C_LTE_FREQUENCY_LOCKING;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        *value = strdup("N/A");
        return TRX_OK;
    }
	if(strlen(stLteState.stLteState.FrequencyLocking)<1)
	{
		 *value = strdup("N/A");
	}else{
    	*value = strdup(stLteState.stLteState.FrequencyLocking);
	} */
  	return TRX_OK;
}
TRX_STATUS getX_CMCC_FrequencyLockInfo(char **value){
    /* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};
    stLteState.eFlag = TR69C_LTE_FREQUENCY_LOCKING_INFO;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        *value = strdup("N/A");
        return TRX_OK;
    }
	slog(TR69C_LOG_ERR,"tr69c >>>>>> FrequencyLockingInfo  %s ",stLteState.stLteState.FrequencyLockingInfo); 
	if(strlen(stLteState.stLteState.FrequencyLockingInfo)<1)
	{
		 *value = strdup("N/A");
	}else{
		*value = strdup(stLteState.stLteState.FrequencyLockingInfo);
	} */
  	return TRX_OK;
}
TRX_STATUS setX_CMCC_IsCellLock(const char *value)
{
	/* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};

    stLteState.eFlag = TR69C_LTE_IS_CELL_LOCK;

	memcpy(stLteState.stLteState.IsCellLock,value,strlen(value));

    if(OM_E_NONE != om_lte_tr069_lte_state_set(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_OK;
    } */
  	return TRX_OK;
}
TRX_STATUS getX_CMCC_IsCellLock(char **value)
{

   	/* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};

    stLteState.eFlag = TR69C_LTE_IS_CELL_LOCK;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);
        *value = strdup("0");
        return TRX_OK;
    }
    *value = strdup(stLteState.stLteState.IsCellLock); */

  	return TRX_OK;
}
/*

*/
TRX_STATUS setX_CMCC_CellLockType(const char *value)
{
	/* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};

    stLteState.eFlag = TR69C_LTE_X_CMCC_CellLockType;
	memcpy(stLteState.stLteState.X_CMCC_CellLockType,value,strlen(value));

    if(OM_E_NONE != om_lte_tr069_lte_state_set(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_OK;
    } */
  	return TRX_OK;
}
TRX_STATUS getX_CMCC_CellLockType(char **value)
{
	/* OM_TR69C_LTE_STATE_PARAM_T pstLteState={0};
	pstLteState.eFlag = TR69C_LTE_X_CMCC_CellLockType;

    if(OM_E_NONE != om_lte_tr069_lte_state_get(&pstLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        *value = strdup("0");
        return TRX_OK;
    }
//sprintf(buf, "%d", pstLteCellCfg.uiCellAutoLockSw);
    *value = strdup(pstLteState.stLteState.X_CMCC_CellLockType); */
 
  	return TRX_OK;
}
TRX_STATUS getX_CMCC_CellLocking(char **value)
{
	/* char actmp[32]={0};
	OM_TR69C_LTE_STATE_PARAM_T pstLteState={0};
	memset(actmp, 0, sizeof(actmp));
	pstLteState.eFlag = TR69C_LTE_CELL_LOCKING;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&pstLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);  
		*value = strdup("0");
        return TRX_OK;
    }
	sprintf(actmp,"%d",atoi(pstLteState.stLteState.CellLocking));
	*value = strdup(actmp); */
  	return TRX_OK;
}
TRX_STATUS getX_CMCC_CellLockInfo(char **value)
{
	/* CHAR acTemp[256] = {0};
	memset(acTemp, 0, sizeof(acTemp));
	if(strlen(lteinfostate.aucLcid) !=3)
	{
		
		sprintf(acTemp, "%s,%s-%s,4G;",lteinfostate.aucTac,lteinfostate.aucEnodebId,lteinfostate.aucLcid);
		*value = strdup(acTemp);
		return TRX_OK;
	}

	OM_TR69C_LTE_STATE_PARAM_T pstLteState={0};
	pstLteState.eFlag = TR69C_LTE_X_CMCC_CellLockInfo;

    if(OM_E_NONE != om_lte_tr069_lte_state_get(&pstLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        *value = strdup("N/A");
        return TRX_OK;
    }
  	slog(TR69C_LOG_ERR,">>>>>>.. : %s",pstLteState.stLteState.X_CMCC_CellLockInfo);     
	sprintf(lteinfostate.aucLcid,"%s",pstLteState.stLteState.X_CMCC_CellLockInfo);
  	sprintf(acTemp, "%s,%s-%s,4G;",lteinfostate.aucTac,lteinfostate.aucEnodebId,lteinfostate.aucLcid);
    *value = strdup(pstLteState.stLteState.X_CMCC_CellLockInfo); */
	return TRX_OK;

}
/*
	LockPin 
		缺省配置:FALSE = 0
		  	OM_LTE_PIN_OFF_AND_READY= 1,
   	 		OM_LTE_PIN_ON_AND_READY,
    		OM_LTE_PIN_ON_AND_SIMPIN,
    		OM_LTE_PIN_ON_AND_SIMPUK,
    		OM_LTE_PIN_FAILED 
*/
TRX_STATUS setX_CMCC_LockPin(const char *value)
{
	/* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};

    stLteState.eFlag = TR69C_LTE_X_CMCC_LockPin;
	memcpy(stLteState.stLteState.X_CMCC_LockPin,value,strlen(value));

    if(OM_E_NONE != om_lte_tr069_lte_state_set(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_OK;
    } */
  	return TRX_OK;
}
TRX_STATUS getX_CMCC_LockPin(char **value)
{
  /*  	OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};

    stLteState.eFlag = TR69C_LTE_X_CMCC_LockPin;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        *value = strdup("0");
        return TRX_OK;
    }

    *value = strdup(stLteState.stLteState.X_CMCC_LockPin); */
  	return TRX_OK;
}


TRX_STATUS setX_CMCC_LockPinType(const char *value)
{
	return TRX_OK;
}
TRX_STATUS getX_CMCC_LockPinType(char **value)
{
    *value = strdup("1");
    return TRX_OK;
}
TRX_STATUS getX_CMCC_FirstPin(char **value)
{

	/* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};

    stLteState.eFlag = TR69C_LTE_X_CMCC_FirstPin;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
		*value = strdup("N/A");
        return TRX_OK;
    }

    *value = strdup(stLteState.stLteState.X_CMCC_FirstPin);
   
    *value = strdup("1234"); */
  	return TRX_OK;
}
TRX_STATUS getX_CMCC_FixedPin(char **value)
{
	/* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};

    stLteState.eFlag = TR69C_LTE_X_CMCC_FixedPin;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);    
		*value = strdup("N/A");
        return TRX_OK;
    }

    *value = strdup(stLteState.stLteState.X_CMCC_FixedPin); */
  	return TRX_OK;
}

/*
	开启卡密验证 
	EncryptCard
		缺省配置:FALSE

*/
TRX_STATUS setX_CMCC_EncryptCard(const char *value)
{
	/* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};
    stLteState.eFlag = TR69C_LTE_X_CMCC_EncryptCard;
//	memcpy(stLteState.stLteState.X_CMCC_EncryptCard,value,strlen(value));
	stLteState.stLteState.X_CMCC_EncryptCard = atoi(value);

    if(OM_E_NONE != om_lte_tr069_lte_state_set(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);
        return TRX_OK;
    } */
  	return TRX_OK;
}
TRX_STATUS getX_CMCC_EncryptCard(char **value)
{
	/* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};
	char buf[CHAR_LEN_32];
	memset(buf, 0, sizeof(buf));
    stLteState.eFlag = TR69C_LTE_X_CMCC_EncryptCard;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        *value = strdup("0");
        return TRX_OK;
    }
	sprintf(buf, "%d", stLteState.stLteState.X_CMCC_EncryptCard);
    *value = strdup(buf); */
  	return TRX_OK;
}

TRX_STATUS setX_CMCC_EncryptCardKey(const char *value)
{
	/* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};

    stLteState.eFlag = TR69C_LTE_X_CMCC_EncryptCardKey;
	memcpy(stLteState.stLteState.X_CMCC_EncryptCardKey,value,strlen(value));

    if(OM_E_NONE != om_lte_tr069_lte_state_set(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_OK;
    } */
  	return TRX_OK;
}
TRX_STATUS getX_CMCC_EncryptCardKey(char **value)
{
	/* OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};
	//char buf[CHAR_LEN_32];

    stLteState.eFlag = TR69C_LTE_X_CMCC_EncryptCardKey;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        *value = strdup("N/A");
        return TRX_OK;
    }
//	sprintf(buf, "%d", stLteState.stLteState.X_CMCC_EncryptCard);
    *value = strdup(stLteState.stLteState.X_CMCC_EncryptCardKey); */
  	return TRX_OK;
}
TRX_STATUS getX_CMCC_RSRP(char **value)
{
	/* CHAR acTemp[CHAR_LEN_32] = {0};
	memset(acTemp, 0, sizeof(acTemp));
	if(lteinfostate.CMCC_RSRP != 0)
	{
		sprintf(acTemp,"%d",lteinfostate.CMCC_RSRP);
    	*value =strdup(acTemp);  
		return TRX_OK;
	} 
	
    OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};
	char buf[CHAR_LEN_32];
    stLteState.eFlag = TR69C_LTE_RSRP;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        *value = strdup("0");
        return TRX_OK;
    }
	lteinfostate.CMCC_RSRP = stLteState.stLteState.RSRP;
	sprintf(buf, "%d", stLteState.stLteState.RSRP);
    *value = strdup(buf); */
  	return TRX_OK;
  
  
}
TRX_STATUS getX_CMCC_RSRQ(char **value)
{
	/* CHAR acTemp[CHAR_LEN_32] = {0};
	memset(acTemp, 0, sizeof(acTemp));
	if(lteinfostate.CMCC_RSRP != 0)
	{
		sprintf(acTemp,"%d",lteinfostate.CMCC_RSRP);
    	*value =strdup(acTemp);   
		return TRX_OK;
	}

   	OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};
	char buf[CHAR_LEN_32];
	memset(buf, 0, sizeof(buf));
    stLteState.eFlag = TR69C_LTE_RSRQ;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        *value = strdup("0");
        return TRX_OK;
    }
	lteinfostate.CMCC_RSRP = stLteState.stLteState.RSRQ;
	sprintf(buf, "%d", stLteState.stLteState.RSRQ);
    *value = strdup(buf); */
  	return TRX_OK;
}
TRX_STATUS getX_CMCC_SINR(char **value)
{
	/* CHAR acTemp[CHAR_LEN_32] = {0};
	memset(acTemp, 0, sizeof(acTemp));
	if(lteinfostate.CMCC_SINR != 0)
	{
		sprintf(acTemp,"%d",lteinfostate.CMCC_SINR);
    	*value =strdup(acTemp);  
		return TRX_OK;
	}
	
	OM_TR69C_LTE_STATE_PARAM_T stLteState = {0};
	char buf[CHAR_LEN_32];
	memset(buf, 0, sizeof(buf));
    stLteState.eFlag = TR69C_LTE_SINR;
    if(OM_E_NONE != om_lte_tr069_lte_state_get(&stLteState))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);
        *value = strdup("0");
        return TRX_OK;
    }
	lteinfostate.CMCC_SINR = stLteState.stLteState.SINR;
	sprintf(buf, "%d", stLteState.stLteState.SINR);
    *value = strdup(buf); */
  	return TRX_OK;
  
  	
}
TRX_STATUS getX_CMCC_UpstreamTotalByte(char **value)
{

/* 	OM_NET_TS_LIST_T  pstUserList[128];
	int iNum = 0;
	char buf[CHAR_LEN_32];
	memset(buf, 0, sizeof(buf));	
	if (OM_E_NONE != om_net_user_list_get(pstUserList, &iNum, 128)){
		slog(TR69C_LOG_ERR,"[tr69c:%s,%d] Get User List Fail.",__func__,__LINE__);

        *value = strdup("0");
        return TRX_OK;
    }

	sprintf(buf, "%d", pstUserList[1].stTsIpv4.uiUpBytes);
    *value = strdup(buf);
    return TRX_OK;

 	CHAR actmp[CHAR_LEN_128] = {0};
	memset(actmp, 0, sizeof(actmp));
	if (0 !=om_tr69c_Upstream_get(actmp))
	{
		slog(TR69C_LOG_ERR,"get om_tr69c_Upstream_get error");
		return TRX_ERR;
	}
	slog(TR69C_LOG_DEBUG,"actmp = %s",actmp);

    *value = strdup(actmp); */
    return TRX_OK;
}
TRX_STATUS getX_CMCC_DownstreamTotalByte(char **value)
{
//OM_ERROR_CODE_E om_net_user_list_get(OM_NET_TS_LIST_T * pstUserList, int *iNum, int iMax)
	/* OM_NET_TS_LIST_T  pstUserList[128];

	int iNum = 0;
	char buf[CHAR_LEN_32];
	memset(buf, 0, sizeof(buf));	
	if (OM_E_NONE != om_net_user_list_get(pstUserList, &iNum, 128)){
		slog(TR69C_LOG_ERR,"[tr69c:%s,%d] Get User List Fail.",__func__,__LINE__);
        *value = strdup("0");
        return TRX_OK;
    }

	sprintf(buf, "%d", pstUserList[1].stTsIpv4.uiDownBytes);
    *value = strdup(buf);
    return TRX_OK;

	CHAR actmp[CHAR_LEN_128] = {0};
	memset(actmp, 0, sizeof(actmp));
	if (0 !=om_tr69c_downstream_get(actmp))
	{
		slog(TR69C_LOG_ERR,"get om_tr69c_downstream_get error");
		return TRX_ERR;
	}
	slog(TR69C_LOG_DEBUG,"actmp = %s",actmp);
    *value = strdup(actmp); */
    return TRX_OK;

}
TRX_STATUS getX_CMCC_StartTime(char **value)
{

	/* char buf[CHAR_LEN_64];
	memset(buf, 0, sizeof(buf));
	INT32 uiUpTime = 0;
            

	// 获取设备上电时间 
    if(AP_E_NONE != app_get_up_time(&uiUpTime))
    {
        slog(TR69C_LOG_ERR, "get up time failed");
    }
     sprintf(buf, "%d", uiUpTime);
     *value = strdup(buf);    */
	return TRX_OK;
}
TRX_STATUS getX_CMCC_LoadAverage(char **value)
{
	/* CHAR actmp[CHAR_LEN_128] = {0};
	memset(actmp, 0, sizeof(actmp));
	if (0 !=om_tr69c_LoadAverage_get(actmp))
	{
		slog(TR69C_LOG_ERR,"get memory info error");
		return TRX_ERR;
	}
	slog(TR69C_LOG_DEBUG,"actmp = %s",actmp);

    *value = strdup(actmp);*/
    return TRX_OK;
}
TRX_STATUS getX_CMCC_DeviceMemory(char **value)
{
	/*CHAR actmp[CHAR_LEN_128] = {0};
	memset(actmp, 0, sizeof(actmp));
	if (0 !=om_tr69c_memory_info_get(actmp))
	{
		slog(TR69C_LOG_ERR,"get memory info error");
		return TRX_ERR;
	}
    *value = strdup(actmp); */
    return TRX_OK;
}

TRX_STATUS setX_CMCC_MTU(const char *value)
{
	/* int tmp = atoi(value);

	if(OM_E_NONE != om_net_l2tp_info_set(TR69C_L2TP_MTU,tmp))
	{
			slog(TR69C_LOG_ERR,"ERROR");
			return TRX_ERR;
	}	
    return TRX_OK;
}
TRX_STATUS getX_CMCC_MTU(char **value)
{
    int tmp;
	CHAR acTemp[CHAR_LEN_32] = {0};
	memset(acTemp, 0, sizeof(acTemp));
	if(OM_E_NONE != om_net_l2tp_info_get(TR69C_L2TP_MTU,&tmp))
	{
			slog(TR69C_LOG_ERR,"ERROR");
			return TRX_ERR;
	}
	sprintf(acTemp,"%d",tmp);
    *value =strdup(acTemp);  */
    return TRX_OK;  
}
