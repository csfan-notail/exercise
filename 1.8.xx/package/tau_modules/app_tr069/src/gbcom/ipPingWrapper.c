/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : ipPingWrapper.c
  版 本 号   : 初稿
  作    者   : zhangjiasi
  生成日期   : 2015年6月9日
  最近修改   :
  功能描述   : GBCOM封装的关于ip诊断的接口实现
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
//#include "util/util.h"
#include "informer.h"
#include "ap_types.h"
#include "event.h"
#include "om_tr69c.h"
#include "ipPingDiagnosticsParams.h"
/*Add by zhanghao 获取配置文件内容 2016/6/15 begin*/
#include "uci_config.h"
#include "tr69_lua.h"
/*Add by zhanghao 获取配置文件内容 end*/
#define TAUARRY_LEN 128

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
extern int g_log_level;
extern int TRxObjNode_Node3_Num;
extern TRxObjNode  IPPingDiagnosticsDesc[];
extern TRxObjNode  x_PPPoE2L2TPDesc[];
extern TRxObjNode  x_PPPoE2L2TP_i[];
extern int	sendDiagComplete;
extern int	g_pingWaiteTime;
extern Diag_CMD_INFO g_diag_cmd_info;
OM_TR69C_PING_DIAG_T g_stPingParam;
int IPPing_read_flag;
/* 指示当前ping诊断数据的可用性：
“None”
“Requested”
“Complete”
“Error_CannotResolveHostName”
“Error_Internal”
“Error_Other”
如果ACS将该值设置为“Requested”，则CPE必须初始化相应的ping诊断测试。
请参照TR-098Amendment-2 */

void IPPing_resualt()
{
	/* if(IPPing_read_flag == 1 )
	{
 		om_net_ping_diagnostics_result(&g_stPingParam);
		IPPing_read_flag = 0;
	} */
}
void IPping_start(void)
{
	/* OM_TR69C_PING_DIAG_PARAM_T stPingParam = {0}; 
	if(OM_E_NONE !=om_net_ping_diagnostics(&stPingParam.stPingState))
    {
        slog(TR69C_LOG_ERR,"set om_net_ping_diagnostics failed");
        return ;
    }
	IPPing_read_flag = 1; */

	//g_diag_cmd_info.daig_type = 1;
	//sprintf(g_diag_cmd_info.cmd_str,"%s",stPingParam.stPingState.Interface);
	//slog(TR69C_LOG_ERR,"cmd_str  =  %s",g_diag_cmd_info.cmd_str);
	//stopTimer(IPping_start, NULL);
}
TRX_STATUS setIPPingDiagnostics_DiagnosticsState(const char *pcValue)
{
    
 //  stPingParam.eFlag = TR69C_PING_DIAGNOSTICS_STATE;
 //   memcpy(stPingParam.stPingState.DiagnosticsState,value,strlen(value));
 if(UCI_OK !=uci_config_set(UCI_IPPINGDIAGNOSTICS_STATE,pcValue))
 {
    slog(TR69C_LOG_ERR,"ERROR");
	return TRX_ERR;
 } 
 setTimer(IPping_start,NULL,1000);
 sendDiagComplete=1;
 return TRX_OK;
}
TRX_STATUS getIPPingDiagnostics_DiagnosticsState(char **ppcValue)
{
/*
    OM_TR69C_PING_DIAG_PARAM_T stPingParam = {0};
    stPingParam.eFlag = TR69C_PING_DIAGNOSTICS_STATE;
    if(OM_E_NONE != om_net_tr069_ping_diag_get(&stPingParam))
    {
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
    }      

    *value = strdup(stPingParam.stPingState.DiagnosticsState);
*/
	char acDiagnosticsState[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_IPPINGDIAGNOSTICS_STATE,acDiagnosticsState))
	{
		*ppcValue = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	else
	{
		*ppcValue = strdup(acDiagnosticsState);
	}	
	//*value = strdup("Complete");
    return TRX_OK;                
}
/* 指定诊断测试接口 */
TRX_STATUS setIPPingDiagnostics_Interface(const char *pcValue)
{
    /* OM_TR69C_PING_DIAG_PARAM_T stPingParam = {0};
    
    stPingParam.eFlag = TR69C_PING_INTERFACE;
    memcpy(stPingParam.stPingState.Interface,value,strlen(value));
    if(OM_E_NONE != om_net_tr069_ping_diag_set(&stPingParam))
    {
        slog(TR69C_LOG_ERR,"error");
        return TRX_OK;
    } */
 if(UCI_OK !=uci_config_set(UCI_IPPINGDIAGNOSTICS_INTERFACE,pcValue))
 {
    slog(TR69C_LOG_ERR,"ERROR");
	return TRX_ERR;
 } 
    return TRX_OK;
}
TRX_STATUS getIPPingDiagnostics_Interface(char **ppcValue)
{
   /*  OM_TR69C_PING_DIAG_PARAM_T stPingParam = {0};

    stPingParam.eFlag = TR69C_PING_INTERFACE;
    if(OM_E_NONE != om_net_tr069_ping_diag_get(&stPingParam))
    {
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
    }      

    *value = strdup(stPingParam.stPingState.Interface);     */                 
	char acInterface[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_IPPINGDIAGNOSTICS_INTERFACE,acInterface))
	{
		*ppcValue = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	else
	{
		*ppcValue = strdup(acInterface);
	}	
    return TRX_OK;                
}
TRX_STATUS getIPPingDiagnostics_Pinglog(char **value)
{
	char pcOut[4096]={0};
    FILE *pstFp = NULL;
	char acBuf[128] = {0};

	memset(pcOut, 0, sizeof(pcOut));
   	pstFp= fopen("/tmp/ping_test.file", "r");
    if (!pstFp){
        slog(TR69C_LOG_ERR, "Open /tmp/ping_test.file fail.");
		*value = strdup(""); 
		return TRX_OK;     
    }
	fseek(pstFp, 0, SEEK_SET);
	while(NULL != fgets(acBuf, sizeof(acBuf),pstFp))
	{
	 	if(0 != strlen(acBuf))
	 	{
			acBuf[strlen(acBuf)-1] = ' ';
			acBuf[strlen(acBuf)-2] = ' ';
		//	acBuf[strlen(acBuf)] = ' ';
			strcat(pcOut, acBuf);
			memset(acBuf, 0, sizeof(acBuf));
	 	}
	}
	fclose(pstFp);
	*value = strdup(pcOut);   
	//free(pcOut);
	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>%s\n", pcOut);
    return TRX_OK;   
}
/* 将要诊断的主机名或者地址 */
TRX_STATUS setIPPingDiagnostics_Host(const char *pcValue)
{
    /* OM_TR69C_PING_DIAG_PARAM_T stPingParam = {0};
    
    stPingParam.eFlag = TR69C_PING_HOST;
    memcpy(stPingParam.stPingState.Host,value,strlen(value));
    if(OM_E_NONE != om_net_tr069_ping_diag_set(&stPingParam))
    {
        slog(TR69C_LOG_ERR,"error");
    }
	//设置 完  Host 之后 开始 诊断 
	IPPing_read_flag = 1;
	//setTimer(IPping_start,NULL,500);
	sendDiagComplete=1;
	g_diag_cmd_info.daig_type = 2; */
 if(UCI_OK !=uci_config_set(UCI_IPPINGDIAGNOSTICS_HOST,pcValue))
 {
    slog(TR69C_LOG_ERR,"ERROR");
	return TRX_ERR;
 } 
    return TRX_OK;
}
TRX_STATUS getIPPingDiagnostics_Host(char **ppcValue)
{
    /* OM_TR69C_PING_DIAG_PARAM_T stPingParam = {0};

    stPingParam.eFlag = TR69C_PING_HOST;
    if(OM_E_NONE != om_net_tr069_ping_diag_get(&stPingParam))
    {
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
    }      

    *value = strdup(stPingParam.stPingState.Host);    */       
    char acHost[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_IPPINGDIAGNOSTICS_HOST,acHost))
	{
		*ppcValue = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	else
	{
		*ppcValue = strdup(acHost);
	}	
    return TRX_OK;                
}
/* 获得诊断结果前，诊断的重复次数 */
TRX_STATUS setIPPingDiagnostics_NumberOfRepetitions(const char *pcValue)
{
    /* OM_TR69C_PING_DIAG_PARAM_T stPingParam = {0};
    
    stPingParam.eFlag = TR69C_PING_NUM_REPETITIONS;
    stPingParam.stPingState.NumberOfRepetitions = atoi(value);
    if(OM_E_NONE != om_net_tr069_ping_diag_set(&stPingParam))
    {
        slog(TR69C_LOG_ERR,"error");
        return TRX_OK;
    }
	g_pingWaiteTime = atoi(value); */
	if(UCI_OK !=uci_config_set(UCI_IPPINGDIAGNOSTICS_NUMOFREPETITIONS,pcValue))
	{
	    slog(TR69C_LOG_ERR,"ERROR");
		return TRX_ERR;
	} 
    return TRX_OK;
}
TRX_STATUS getIPPingDiagnostics_NumberOfRepetitions(char **ppcValue)
{
   /*  OM_TR69C_PING_DIAG_PARAM_T stPingParam = {0};
    CHAR acTemp[CHAR_LEN_32] = {0};

    stPingParam.eFlag = TR69C_PING_NUM_REPETITIONS;
    if(OM_E_NONE != om_net_tr069_ping_diag_get(&stPingParam))
    {
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
    }      
    sprintf(acTemp,"%d",stPingParam.stPingState.NumberOfRepetitions);
    *value =strdup(acTemp);   */
    char acNumberOfRepetitions[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_IPPINGDIAGNOSTICS_NUMOFREPETITIONS,acNumberOfRepetitions))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	else
	{
		*ppcValue = strdup(acNumberOfRepetitions);
	}	                        
    return TRX_OK;                
}
TRX_STATUS setIPPingDiagnostics_Timeout(const char *pcValue)
{
    /* OM_TR69C_PING_DIAG_PARAM_T stPingParam = {0};
    
    stPingParam.eFlag = TR69C_PING_TIMEOUT;
    stPingParam.stPingState.Timeout = atoi(value);
    if(OM_E_NONE != om_net_tr069_ping_diag_set(&stPingParam))
    {
        slog(TR69C_LOG_ERR,"error");
        return TRX_OK;
    } */
    if(UCI_OK !=uci_config_set(UCI_IPPINGDIAGNOSTICS_TIMEOUT,pcValue))
	{
	    slog(TR69C_LOG_ERR,"ERROR");
		return TRX_ERR;
	} 
	return TRX_OK;
}
TRX_STATUS getIPPingDiagnostics_Timeout(char **ppcValue)
{
	/* OM_TR69C_PING_DIAG_PARAM_T stPingParam = {0};
    CHAR acTemp[CHAR_LEN_32] = {0};
	memset(acTemp, 0, sizeof(acTemp));
    stPingParam.eFlag = TR69C_PING_TIMEOUT;
    if(OM_E_NONE != om_net_tr069_ping_diag_get(&stPingParam))
    {
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
    }      
    sprintf(acTemp,"%d",stPingParam.stPingState.Timeout);
    *value =strdup(acTemp); */
    char acTimeout[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_IPPINGDIAGNOSTICS_TIMEOUT,acTimeout))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	else
	{
		*ppcValue = strdup(acTimeout);
	}	        
	return TRX_OK;
}

TRX_STATUS setIPPingDiagnostics_DataBlockSize(const char *pcValue)
{
   /*  OM_TR69C_PING_DIAG_PARAM_T stPingParam = {0};
    
    stPingParam.eFlag = TR69C_PING_DATA_BLOCK_SIZE;
    stPingParam.stPingState.DataBlockSize= atoi(value);
    if(OM_E_NONE != om_net_tr069_ping_diag_set(&stPingParam))
    {
        slog(TR69C_LOG_ERR,"error");
        return TRX_OK;
    } */
    if(UCI_OK !=uci_config_set(UCI_IPPINGDIAGNOSTICS_DATABLOCKSIZE,pcValue))
	{
	    slog(TR69C_LOG_ERR,"ERROR");
		return TRX_ERR;
	} 
    return TRX_OK;
}
TRX_STATUS getIPPingDiagnostics_DataBlockSize(char **ppcValue)
{
    /* OM_TR69C_PING_DIAG_PARAM_T stPingParam = {0};
    CHAR acTemp[CHAR_LEN_32] = {0};
	memset(acTemp, 0, sizeof(acTemp));
    stPingParam.eFlag = TR69C_PING_DATA_BLOCK_SIZE;
    if(OM_E_NONE != om_net_tr069_ping_diag_get(&stPingParam))
    {
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
    }      
    sprintf(acTemp,"%d",stPingParam.stPingState.DataBlockSize);
    *value =strdup(acTemp);             */    
    char acDataBlockSize[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_IPPINGDIAGNOSTICS_DATABLOCKSIZE,acDataBlockSize))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	else
	{
		*ppcValue = strdup(acDataBlockSize);
	}	        
    return TRX_OK;
}
TRX_STATUS setIPPingDiagnostics_DSCP(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_IPPINGDIAGNOSTICS_DSCP,pcValue))
	{
	    slog(TR69C_LOG_ERR,"ERROR");
		return TRX_ERR;
	} 
	return TRX_OK;
}
TRX_STATUS getIPPingDiagnostics_DSCP(char **ppcValue)
{
	char acDscp[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_IPPINGDIAGNOSTICS_DSCP,acDscp))
	{
		*ppcValue = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	else
	{
		*ppcValue = strdup(acDscp);
	}	        
	//*value = strdup("");
	return TRX_OK;
}

TRX_STATUS getIPPingDiagnostics_SuccessCount(char **value)
{
 	CHAR acTemp[CHAR_LEN_32] = {0};
	memset(acTemp, 0, sizeof(acTemp));
	IPPing_resualt();
    sprintf(acTemp,"%d",g_stPingParam.SuccessCount);
    *value =strdup(acTemp);                            
    return TRX_OK;                
}
TRX_STATUS getIPPingDiagnostics_FailureCount(char **value)
{
	CHAR acTemp[CHAR_LEN_32] = {0};
	memset(acTemp, 0, sizeof(acTemp));
	IPPing_resualt();
    sprintf(acTemp,"%d",g_stPingParam.FailureCount);
    *value =strdup(acTemp);                            
    return TRX_OK;   
}


TRX_STATUS getIPPingDiagnostics_AverageResponseTime(char **value)
{
	CHAR acTemp[CHAR_LEN_32] = {0};
	memset(acTemp, 0, sizeof(acTemp));
	IPPing_resualt();
    sprintf(acTemp,"0.%d",g_stPingParam.AverageResponseTime);
    *value =strdup(acTemp);                              
    return TRX_OK;                
}

TRX_STATUS getIPPingDiagnostics_MinimumResponseTime(char **value)
{	
	CHAR acTemp[CHAR_LEN_32] = {0};
	memset(acTemp, 0, sizeof(acTemp));
	IPPing_resualt();
 	sprintf(acTemp,"0.%d",g_stPingParam.MinimumResponseTime);
    *value =strdup(acTemp);                               
    return TRX_OK;                
}

TRX_STATUS getIPPingDiagnostics_MaximumResponseTime(char **value)
{
	CHAR acTemp[CHAR_LEN_32] = {0};
	memset(acTemp, 0, sizeof(acTemp));
   	IPPing_resualt();
 	sprintf(acTemp,"0.%d",g_stPingParam.MaximumResponseTime);
    *value =strdup(acTemp);                               
    return TRX_OK;                
}


void initIPPingDiagnosticsInstances(void)
{
    TRxObjNode *n;
    int id;
    InstanceDesc  *IPPingDiagnosticsIdp;
    
    n = IPPingDiagnosticsDesc;
    id = 1;
    IPPingDiagnosticsIdp = getNewInstanceDesc(n, NULL, id);
}


TRX_STATUS setX_PPPoE2L2TP_Enable(const char *value)
{
	/* int tmp = atoi(value);

	if(OM_E_NONE != om_net_l2tp_info_set(TR69C_L2TP_ENABLE,tmp))
	{
			slog(TR69C_LOG_ERR,"ERROR");
			return TRX_ERR;
	}	 */
    return TRX_OK;
}

TRX_STATUS getX_PPPoE2L2TP_Enable(char **value)
{
   /*  int tmp;
	CHAR acTemp[CHAR_LEN_32] = {0};

	if(OM_E_NONE != om_net_l2tp_info_get(TR69C_L2TP_ENABLE,&tmp))
	{
			slog(TR69C_LOG_ERR,"ERROR");
			return TRX_ERR;
	}
	sprintf(acTemp,"%d",tmp);
    *value =strdup(acTemp);  */
    return TRX_OK;                                           
}

TRX_STATUS setX_PPPoE2L2TP_MTUOfUser(const char *value)
{	
	/* int tmp = atoi(value);

	if(OM_E_NONE != om_net_l2tp_info_set(TR69C_L2TP_MTU,tmp))
	{
			slog(TR69C_LOG_ERR,"ERROR");
			return TRX_ERR;
	}	 */
    return TRX_OK;
}

TRX_STATUS getX_PPPoE2L2TP_MTUOfUser(char **value)
{	
	/* int tmp;
	CHAR acTemp[CHAR_LEN_32] = {0};

	if(OM_E_NONE != om_net_l2tp_info_get(TR69C_L2TP_MTU,&tmp))
	{
			slog(TR69C_LOG_ERR,"ERROR");
			return TRX_ERR;
	}
	sprintf(acTemp,"%d",tmp);
    *value =strdup(acTemp);  */
    return TRX_OK;  
                                              
}

TRX_STATUS getX_PPPoE2L2TP_L2TPServerNumberOfEntries(char **value)
{
   /*  int tmp;
	CHAR acTemp[CHAR_LEN_32] = {0};

	if(OM_E_NONE != om_net_l2tp_info_get(TR69C_L2TP_SERVERNUMBER,&tmp))
	{
			slog(TR69C_LOG_ERR,"ERROR");
			return TRX_ERR;
	}
	sprintf(acTemp,"%d",tmp);
    *value =strdup(acTemp);  */
    return TRX_OK;                
}

void initX_PPPoE2L2TPInstances(void)
{
   /*  TRxObjNode *n;
    int id;
    InstanceDesc  *X_PPPoE2L2TPInstancesIdp;
	InstanceDesc  *x_PPPoE2L2TP_iIdp;
    
    n = x_PPPoE2L2TPDesc;
    id = 1;
    X_PPPoE2L2TPInstancesIdp = getNewInstanceDesc(n, NULL, id);

	int tmp;
	if(OM_E_NONE != om_net_l2tp_info_get(TR69C_L2TP_SERVERNUMBER,&tmp))
	{
			slog(TR69C_LOG_ERR,"ERROR");
			return ;
	}
	slog(TR69C_LOG_DEBUG,"tmp = %d ",tmp);
	int i=1;
	for(i=1;i<=tmp;i++)
	{	
		id=i;
		n=x_PPPoE2L2TP_i;
		x_PPPoE2L2TP_iIdp  = getNewInstanceDesc(n, X_PPPoE2L2TPInstancesIdp, id);
	} */
/*	
	//OM_NET_L2TP_CFG_T *pstl2tpCfg = (OM_NET_L2TP_CFG_T *)malloc(sizeof(OM_NET_L2TP_CFG_T));
	PPPOE_L2TP_CFG_T *pstL2tpCfg = (PPPOE_L2TP_CFG_T *)malloc(sizeof(PPPOE_L2TP_CFG_T));
	memset(pstL2tpCfg, 0, sizeof(PPPOE_L2TP_CFG_T));
	CHAR acTemp[CHAR_LEN_32] = {"192.168.12.2"};
	
	pstL2tpCfg.enTunnelMode = 1;
	pstL2tpCfg.enTaSwitch = 1;
	
	sprintf(pstL2tpCfg.aucHostName,"hostname0");
	sprintf(pstL2tpCfg.aucPortName,"portname0");
	sprintf(pstL2tpCfg.aucTaPassWd,"12345ad");
	util_str_to_ip(acTemp,pstL2tpCfg.aucLnsIp);

	if(OM_E_NONE != om_net_l2tp_modify(1,pstL2tpCfg))
	{
				slog(TR69C_LOG_ERR,"ERROR");
	}
	free(pstL2tpCfg);
*/
	
	
//	x_PPPoE2L2TP_i
}
//*********************************************
/*OM_ERROR_CODE_E  om_net_tr69_l2tp_get(OM_NET_L2TP_CFG_T *pstl2tpCfg)
{
	return OM_E_NONE;
}
*/


//*********************************************
TRX_STATUS setX_PPPoE2L2TP_Device(const char *value)
{
    return TRX_OK;
}

TRX_STATUS getX_PPPoE2L2TP_Device(char **value)
{	
	CHAR acTemp[CHAR_LEN_32] = {0};
	sprintf(acTemp,"lte_cpe_l2tpServer_Num_%d",TRxObjNode_Node3_Num-1);

     *value =strdup(acTemp);                             
    return TRX_OK;                
}
/*
l2tp隧道模式:
	0:单隧道模式
	1:多隧道模式
*/
TRX_STATUS setX_PPPoE2L2TP_TunnelMode(const char *value)
{	
	/* OM_NET_L2TP_CFG_T *pstl2tpCfg = (OM_NET_L2TP_CFG_T *)malloc(sizeof(OM_NET_L2TP_CFG_T));
	memset(pstl2tpCfg, 0, sizeof(OM_NET_L2TP_CFG_T));
	//CHAR acTemp[CHAR_LEN_32] = {0};
	pstl2tpCfg->ulID=TRxObjNode_Node3_Num-1;

	pstl2tpCfg->stPppoeL2tp.enTunnelMode= atoi(value);
	
	if( OM_E_NONE != om_net_tr69_l2tp_set(TR69C_L2TP_TUNNELMODE,pstl2tpCfg))
	{ 
		slog(TR69C_LOG_ERR,"ERROR");
		free(pstl2tpCfg);
 		return TRX_ERR;
	}
	free(pstl2tpCfg); */
    return TRX_OK;
}

TRX_STATUS getX_PPPoE2L2TP_TunnelMode(char **value)
{
	/* OM_NET_L2TP_CFG_T *pstl2tpCfg = (OM_NET_L2TP_CFG_T *)malloc(sizeof(OM_NET_L2TP_CFG_T));
	memset(pstl2tpCfg, 0, sizeof(OM_NET_L2TP_CFG_T));

//	OM_TR69C_DEVICE_PARAM_T *stTr69cParam = (OM_TR69C_DEVICE_PARAM_T *)malloc(sizeof(OM_TR69C_DEVICE_PARAM_T));
 //   memset(stTr69cParam, 0, sizeof(OM_TR69C_DEVICE_PARAM_T));
	CHAR acTemp[CHAR_LEN_32] = {0};

	pstl2tpCfg->ulID=TRxObjNode_Node3_Num-1;
	if( OM_E_NONE != om_net_tr69_l2tp_get(pstl2tpCfg))
	{ 
		slog(TR69C_LOG_ERR,"ERROR");
		free(pstl2tpCfg);
 		return TRX_ERR;
	}
	
    sprintf(acTemp,"%d",pstl2tpCfg->stPppoeL2tp.enTunnelMode);
    *value =strdup(acTemp);    
	free(pstl2tpCfg); */
    return TRX_OK;  
}

TRX_STATUS setX_PPPoE2L2TP_LNSServerIPAddress(const char *value)
{
   /*  OM_NET_L2TP_CFG_T *pstl2tpCfg = (OM_NET_L2TP_CFG_T *)malloc(sizeof(OM_NET_L2TP_CFG_T));
	memset(pstl2tpCfg, 0, sizeof(OM_NET_L2TP_CFG_T));
	//CHAR acTemp[CHAR_LEN_32] = {0};
	pstl2tpCfg->ulID=TRxObjNode_Node3_Num-1;

	util_str_to_ip((char *)value,pstl2tpCfg->stPppoeL2tp.aucLnsIp);	
	if( OM_E_NONE != om_net_tr69_l2tp_set(TR69C_L2TP_LNSSERVERIPADDR,pstl2tpCfg))
	{ 
		slog(TR69C_LOG_ERR,"ERROR");
		free(pstl2tpCfg);
 		return TRX_ERR;
	} */
    return TRX_OK;
}

TRX_STATUS getX_PPPoE2L2TP_LNSServerIPAddress(char **value)
{
                                
    /* OM_NET_L2TP_CFG_T *pstl2tpCfg = (OM_NET_L2TP_CFG_T *)malloc(sizeof(OM_NET_L2TP_CFG_T));
	memset(pstl2tpCfg, 0, sizeof(OM_NET_L2TP_CFG_T));
	CHAR acTemp[CHAR_LEN_32] = {0};

	pstl2tpCfg->ulID=TRxObjNode_Node3_Num-1;
	if( OM_E_NONE != om_net_tr69_l2tp_get(pstl2tpCfg))
	{ 
		slog(TR69C_LOG_ERR,"ERROR");
		free(pstl2tpCfg);
 		return TRX_ERR;
	}

	sprintf(acTemp,IP_FMT,IP_ARG(pstl2tpCfg->stPppoeL2tp.aucLnsIp));
   // sprintf(acTemp,"%d",pstl2tpCfg->stPppoeL2tp.enTunnelMode);
    *value =strdup(acTemp);  
   free(pstl2tpCfg); */
    return TRX_OK;                 
}

TRX_STATUS setX_PPPoE2L2TP_LNSServerHostName(const char *value)
{
    /* OM_NET_L2TP_CFG_T *pstl2tpCfg = (OM_NET_L2TP_CFG_T *)malloc(sizeof(OM_NET_L2TP_CFG_T));
	memset(pstl2tpCfg, 0, sizeof(OM_NET_L2TP_CFG_T));
	//CHAR acTemp[CHAR_LEN_32] = {0};
	pstl2tpCfg->ulID=0;

	memcpy(pstl2tpCfg->stPppoeL2tp.aucHostName,value,strlen(value));
	if( OM_E_NONE != om_net_tr69_l2tp_set(TR69C_L2TP_LNSSERVERHOSTNAME,pstl2tpCfg))
	{ 
		slog(TR69C_LOG_ERR,"ERROR");
		free(pstl2tpCfg);
 		return TRX_ERR;
	} */
    return TRX_OK;
}

TRX_STATUS getX_PPPoE2L2TP_LNSServerHostName(char **value)
{
                                
    /* OM_NET_L2TP_CFG_T *pstl2tpCfg = (OM_NET_L2TP_CFG_T *)malloc(sizeof(OM_NET_L2TP_CFG_T));
	memset(pstl2tpCfg, 0, sizeof(OM_NET_L2TP_CFG_T));
	//CHAR acTemp[CHAR_LEN_32] = {0};

	pstl2tpCfg->ulID=TRxObjNode_Node3_Num-1;
	if( OM_E_NONE != om_net_tr69_l2tp_get(pstl2tpCfg))
	{ 
		slog(TR69C_LOG_ERR,"ERROR");
		free(pstl2tpCfg);
 		return TRX_ERR;
	}
	
    *value =strdup(pstl2tpCfg->stPppoeL2tp.aucHostName);                             
  	free(pstl2tpCfg); */
  	return TRX_OK;    
	
}

TRX_STATUS setX_PPPoE2L2TP_LNSServerPortName(const char *value)
{
    return TRX_OK;
}

TRX_STATUS getX_PPPoE2L2TP_LNSServerPortName(char **value)
{
                                
    /* OM_NET_L2TP_CFG_T *pstl2tpCfg = (OM_NET_L2TP_CFG_T *)malloc(sizeof(OM_NET_L2TP_CFG_T));
	memset(pstl2tpCfg, 0, sizeof(OM_NET_L2TP_CFG_T));
	pstl2tpCfg->ulID=TRxObjNode_Node3_Num-1;
	
	if( OM_E_NONE != om_net_tr69_l2tp_get(pstl2tpCfg))
	{ 
		slog(TR69C_LOG_ERR,"ERROR");
		free(pstl2tpCfg);
 		return TRX_ERR;
	}
	
    *value =strdup(pstl2tpCfg->stPppoeL2tp.aucPortName);                             
    free(pstl2tpCfg); */
	return TRX_OK;      
	
}
/*
LNS隧道验证
PPPOE_L2TP_AUTH_SWITCH_E:

	PPPOE_L2TP_AUTH_SWITCH_OFF     =0,
    PPPOE_L2TP_AUTH_SWITCH_ON      =1,   

*/
TRX_STATUS setX_PPPoE2L2TP_TunnelAuthenticateEnable(const char *value)
{
    /* OM_NET_L2TP_CFG_T *pstl2tpCfg = (OM_NET_L2TP_CFG_T *)malloc(sizeof(OM_NET_L2TP_CFG_T));
	memset(pstl2tpCfg, 0, sizeof(OM_NET_L2TP_CFG_T));
	//CHAR acTemp[CHAR_LEN_32] = {0};
	pstl2tpCfg->ulID=TRxObjNode_Node3_Num-1;

	pstl2tpCfg->stPppoeL2tp.enTaSwitch= atoi(value);
	
	if( OM_E_NONE != om_net_tr69_l2tp_set(TR69C_L2TP_TUNNELAUTHENTICATEENABLE,pstl2tpCfg))
	{ 
		slog(TR69C_LOG_ERR,"ERROR");
		free(pstl2tpCfg);
 		return TRX_ERR;
	}
	free(pstl2tpCfg); */
    return TRX_OK;
}

TRX_STATUS getX_PPPoE2L2TP_TunnelAuthenticateEnable(char **value)
{
                                
   	/* OM_NET_L2TP_CFG_T *pstl2tpCfg = (OM_NET_L2TP_CFG_T *)malloc(sizeof(OM_NET_L2TP_CFG_T));
	memset(pstl2tpCfg, 0, sizeof(OM_NET_L2TP_CFG_T));
	CHAR acTemp[CHAR_LEN_32] = {0};

	pstl2tpCfg->ulID=TRxObjNode_Node3_Num-1;
	if( OM_E_NONE != om_net_tr69_l2tp_get(pstl2tpCfg))
	{ 
		slog(TR69C_LOG_ERR,"ERROR");
		free(pstl2tpCfg);
 		return TRX_ERR;
	}
	
    sprintf(acTemp,"%d",pstl2tpCfg->stPppoeL2tp.enTaSwitch);
    *value =strdup(acTemp);                             
    free(pstl2tpCfg); */
	return TRX_OK;        
	
}

TRX_STATUS setX_PPPoE2L2TP_PasswordOfTunnel(const char *value)
{
    /* OM_NET_L2TP_CFG_T *pstl2tpCfg = (OM_NET_L2TP_CFG_T *)malloc(sizeof(OM_NET_L2TP_CFG_T));
	memset(pstl2tpCfg, 0, sizeof(OM_NET_L2TP_CFG_T));
	//CHAR acTemp[CHAR_LEN_32] = {0};
	pstl2tpCfg->ulID=TRxObjNode_Node3_Num-1;

	memcpy(pstl2tpCfg->stPppoeL2tp.aucTaPassWd,value,strlen(value));
	if( OM_E_NONE != om_net_tr69_l2tp_set(TR69C_L2TP_PASSWORDOFTUNNEL,pstl2tpCfg))
	{ 
		slog(TR69C_LOG_ERR,"ERROR");
		free(pstl2tpCfg);
 		return TRX_ERR;
	} */
    return TRX_OK;
}

TRX_STATUS getX_PPPoE2L2TP_PasswordOfTunnel(char **value)
{
                                
   /*  OM_NET_L2TP_CFG_T *pstl2tpCfg = (OM_NET_L2TP_CFG_T *)malloc(sizeof(OM_NET_L2TP_CFG_T));
	memset(pstl2tpCfg, 0, sizeof(OM_NET_L2TP_CFG_T));
	pstl2tpCfg->ulID=TRxObjNode_Node3_Num-1;
	
	if( OM_E_NONE != om_net_tr69_l2tp_get(pstl2tpCfg))
	{ 
		slog(TR69C_LOG_ERR,"ERROR");
		free(pstl2tpCfg);
 		return TRX_ERR;
	}
	
    *value =strdup(pstl2tpCfg->stPppoeL2tp.aucTaPassWd);                             
    free(pstl2tpCfg); */
	return TRX_OK;      
	             
}


TRX_STATUS deletex_PPPoE2L2TP_i(char **value)
{
/*nstanceDesc *idp;
	if ( idp= getNewInstanceDesc( getCurrentNode(),getCurrentInstanceDesc(),  0) ) {
        *value = strdup(itoa(idp->instanceID));
        return TRX_REBOOT;
*/
        /* or TRX_BOOT if necessary */
  // }
slog(TR69C_LOG_ERR,"deletex_PPPoE2L2TP_i");
    return TRX_ERR;
}

TRX_STATUS addx_PPPoE2L2TP_i(const char *value)
{
/*	TRxObjNode *n = getCurrentNode();
	InstanceDesc *idp;
	int     id = atoi(value);
	
	if ( getInstanceCount(WANDeviceWANConnectionDeviceWANPPPConnectionInstanceDesc)== 0
		 && getInstanceCount(WANDeviceWANConnectionDeviceWANIPConnectionInstanceDesc)==0 ) {
		fprintf(stderr, "Child object NOT DELETED\n");
	}
	if (!deleteInstanceDesc(n,id)){
		fprintf(stderr, "Delete WANConnectionDevice Instance = %d\n", id);
		return TRX_REBOOT;*/	
		/* or TRX_BOOT if necessary */
	//}
slog(TR69C_LOG_ERR,"addx_PPPoE2L2TP_i");
return TRX_ERR;
}
