/******************************************************************************

                  ��Ȩ���� (C), 2010-2012, �Ϻ�徴�ͨ�ſƼ����޹�˾

 ******************************************************************************
  �� �� ��   : traceRouteWrapper.c
  �� �� ��   : ����
  ��    ��   : zhangjiasi
  ��������   : 2015��6��9��
  ����޸�   :
  ��������   : GBCOM��װ�Ĺ���traceroute��صĽӿ�ʵ��
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
#include "ap_types.h"
#include "om_tr69c.h"
#include "om_log.h"
#include "utils.h"
#include "event.h"
#include "informer.h"
#include "SOAPParser/CPEframework.h"
#include "traceRouteDiagnosticsParams.h"
#include <unistd.h>
/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/
extern TRxObjNode TraceRouteDiagnosticsDesc[];
extern TRxObjNode TraceRouteDiagnosticsRouteHopsDesc[];
extern TRxObjNode TraceRouteDiagnosticsRouteHopsInstance[];
//altered by caibing 11-15

//extern TRxObjNode x_CMCC_PPPOE_EmulatorDesc[];
extern int g_log_level;
extern int	g_pingWaiteTime;
extern int	sendDiagComplete;
extern Diag_CMD_INFO g_diag_cmd_info;
int TraceRouteHops_num;
/* OM_TR69C_TRACEROUTE_CMD_CFG_T traceRouteCMD;
OM_TR69C_TRACEROUTE_DATE_INFO_T traceRouteDate; */

/* ������ݿ�����
��None��
��Requested��
��Complete��
��Error_CannotResolveHostName��
��Error_MaxHopCountExceeded��
�����TR-098Amendment-2 */
void getTraceRouteResult(void)
{
	/* char CMD_str[256];
	char acInfo[128] = {0}; 
	int enRet;
	int i = 0;
	if(traceRouteDate.getdate_flag == TR69C_DATE_YES)
	{
		//���ж�  �Ƿ�  ��  Unknown Host
		memset(acInfo, 0, sizeof(acInfo));
    	memset(CMD_str, 0, sizeof(CMD_str));
		sprintf(CMD_str,"cat "TR69C_TRACEROUTE_FILE" |grep Unknown");
    	enRet = popen_get_info(CMD_str, acInfo, sizeof(CMD_str));
    	if (0 != enRet)
    	{
    	}else{
    		if(strlen(acInfo)>7)  //Unknown host
    		{
    			sprintf(traceRouteDate.Resualtstate,"Error_CannotResolveHostName");
				traceRouteDate.HopsNumberOfEntries = 0;  //��������
				return;
			}
    	}
		//��ȷ  ���
		for(i = 1; i <= traceRouteCMD.MaxHopCount;i++)
		{
			//��ȡ host 
			memset(acInfo, 0, sizeof(acInfo));
    		memset(CMD_str, 0, sizeof(CMD_str));
			sprintf(CMD_str,"awk 'NR==%d' "TR69C_TRACEROUTE_FILE" |awk  '{print $2}'",i+1);
    		enRet = popen_get_info(CMD_str, acInfo, sizeof(CMD_str));
    		if (0 != enRet)
    		{
    			slog(TR69C_LOG_ERR,"Read file : "TR69C_TRACEROUTE_FILE" ERROR ");
        		return;
    		}else{
    			if(strlen(acInfo)<1)  //��ǰ��û�л�ȡ�����ݣ��Ѿ���ȡ���ļ�ĩβ��
    			{
					traceRouteDate.HopsNumberOfEntries = i-1;
					slog(TR69C_LOG_DEBUG,"traceRouteDate.HopsNumberOfEntries : %d ",traceRouteDate.HopsNumberOfEntries);
					return;
				}else if(strlen(acInfo)<8){  //��ȡ������̫��(һ���� '*') ˵����һ��û����ϵ�����
					traceRouteDate.hopsDate[i-1].HopErrorCode = 303;    //ErrorCode ������ 303
					sprintf(traceRouteDate.hopsDate[i-1].HopHost,"*");
					sprintf(traceRouteDate.hopsDate[i-1].HopHostAddress,"0.0.0.0");
					sprintf(traceRouteDate.hopsDate[i-1].HopRTTimes,"0");
					slog(TR69C_LOG_ERR,"ID:%d host: %s,ip:%s ERR: %d ,time:%s",i,traceRouteDate.hopsDate[i-1].HopHost,traceRouteDate.hopsDate[i-1].HopHostAddress,traceRouteDate.hopsDate[i-1].HopErrorCode,traceRouteDate.hopsDate[i-1].HopRTTimes);
					continue;
				}else{
					sprintf(traceRouteDate.hopsDate[i-1].HopHost,"%s",acInfo); //��ȡ����ȷ�� host
				}
			}
			// ��ȡ host addr 
			memset(acInfo, 0, sizeof(acInfo));
    		memset(CMD_str, 0, sizeof(CMD_str));
			sprintf(CMD_str,"awk 'NR==%d' "TR69C_TRACEROUTE_FILE" |awk  '{print $3}'",i+1);
    		enRet = popen_get_info(CMD_str, acInfo, sizeof(CMD_str));
    		if (0 != enRet)
    		{
    			sprintf(traceRouteDate.hopsDate[i-1].HopHostAddress,"0.0.0.0");
    		}else{
				if(strlen(acInfo)<4)  //��ȡ��IP̫��(һ���� '*')  û��  ��ȡ����ȷ�� IP ��ַ
    			{
					sprintf(traceRouteDate.hopsDate[i-1].HopHostAddress,"0.0.0.0");
				}else{ //��ȡ��ȷ�� IP  ȥ��'('')'
					acInfo[0] = ' ';
					acInfo[strlen(acInfo)-1] = acInfo[strlen(acInfo)];
					sprintf(traceRouteDate.hopsDate[i-1].HopHostAddress,"%s",acInfo);
				}
			}
			// ��ȡ ��� ʱ�� 
			memset(acInfo, 0, sizeof(acInfo));
    		memset(CMD_str, 0, sizeof(CMD_str));
			sprintf(CMD_str,"awk 'NR==%d' "TR69C_TRACEROUTE_FILE" |awk  '{print $4}'",i+1);
    		enRet = popen_get_info(CMD_str, acInfo, sizeof(CMD_str));
    		if (0 != enRet)
    		{
    			sprintf(traceRouteDate.hopsDate[i-1].HopRTTimes,"0");
    		}else{
				sprintf(traceRouteDate.hopsDate[i-1].HopRTTimes,"%s",acInfo);			
			}
			traceRouteDate.hopsDate[i-1].HopErrorCode = 0;
			slog(TR69C_LOG_ERR,"ID:%d host: %s,ip:%s ERR: %d ,time:%s",i,traceRouteDate.hopsDate[i-1].HopHost,traceRouteDate.hopsDate[i-1].HopHostAddress,traceRouteDate.hopsDate[i-1].HopErrorCode,traceRouteDate.hopsDate[i-1].HopRTTimes);
		}
		traceRouteDate.HopsNumberOfEntries = i-1;  //��������
		//printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> HopsNumberOfEntries = %d \n",i-1);
		slog(TR69C_LOG_ERR,">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> HopsNumberOfEntries = %d \n",i-1);

		traceRouteDate.getdate_flag = TR69C_DATE_No;//��ȡ������ flag  ����
	} */
	return;
}
void TraceRoute_start(void)
{
	/* char traceRoute_CMD_str[256]="traceroute ";
	char tmp_str[128];
	int waittime_n =1;
	//ִ�� traceroute ����
	slog(TR69C_LOG_ERR,"traceRoute_CMD Host: %s  NumberOfTries:%d  Timeout:%d MaxHopCount:%d",traceRouteCMD.host,traceRouteCMD.NumberOfTries,traceRouteCMD.Timeout,traceRouteCMD.MaxHopCount);

	//���� ��ϵ� ÿһ��������ϴ���
	if(traceRouteCMD.NumberOfTries > 0)
	{	
		memset(tmp_str, 0, sizeof(tmp_str));
		sprintf(tmp_str," -q %d",traceRouteCMD.NumberOfTries);
		strcat (traceRoute_CMD_str,tmp_str);
		
		waittime_n = waittime_n * traceRouteCMD.NumberOfTries;
	}else{
		waittime_n = waittime_n * 3;
	}

	//���� ��ϵ� ��� �ȴ� ʱ��
	if(traceRouteCMD.Timeout> 1)
	{	
		memset(tmp_str, 0, sizeof(tmp_str));
		sprintf(tmp_str," -w %d",traceRouteCMD.Timeout/1000);
		strcat (traceRoute_CMD_str,tmp_str);
		waittime_n = waittime_n * traceRouteCMD.Timeout;
	}else{
		memset(tmp_str, 0, sizeof(tmp_str));
		sprintf(tmp_str," -w 3");
		strcat (traceRoute_CMD_str,tmp_str);
		waittime_n = waittime_n * 3000;
	}
	//���� ��ϵ� ����
	if((traceRouteCMD.MaxHopCount > 1)&&(traceRouteCMD.MaxHopCount < 31))
	{
		memset(tmp_str, 0, sizeof(tmp_str));
		sprintf(tmp_str," -m %d",traceRouteCMD.MaxHopCount);
		strcat (traceRoute_CMD_str,tmp_str);
		waittime_n = waittime_n * traceRouteCMD.MaxHopCount;
	}else{
		traceRouteCMD.MaxHopCount = 30;
		waittime_n = waittime_n * 30;
	}
	//������ϵ� host
	if(strlen(traceRouteCMD.host)<8)
	{
		strcat (traceRoute_CMD_str,"www.baidu.com > "TR69C_TRACEROUTE_FILE" 2>&1 ");
	}else{
		memset(tmp_str, 0, sizeof(tmp_str));
		sprintf(tmp_str," %s > "TR69C_TRACEROUTE_FILE" 2>&1 ",traceRouteCMD.host);
		strcat (traceRoute_CMD_str,tmp_str);
	}
	slog(TR69C_LOG_ERR,"traceRoute_CMD : %s ",traceRoute_CMD_str);
	//���� TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
	
	//sprintf(g_diag_cmd_info.cmd_str,"%s",traceRoute_CMD_str);
	
	system(traceRoute_CMD_str);
	//���� tracerouteִ�е�ʱ��
	//g_pingWaiteTime = waittime_n/1000;
	traceRouteDate.getdate_flag = TR69C_DATE_YES;
	//stopTimer(TraceRoute_start, NULL); */
	
}
	
TRX_STATUS setTraceRouteDiagnostics_DiagnosticsState(const char *value)
{
/* #if 0
	setTimer(TraceRoute_start,NULL,500);

	// TraceRoute_start(NULL); 
	//���� tracerouteִ�е�ʱ��
	g_pingWaiteTime = 60;
	slog(TR69C_LOG_ERR,"g_pingWaiteTime traceroute  : %d ",g_pingWaiteTime);
	//���� �����ϱ��ı�־
	sendDiagComplete = 1;
	
	//�������»�ȡ���ݱ�ʾ  ����Ͻ�� ���ļ��ж�ȡ�� �ṹ�� traceRouteDate��
	traceRouteDate.getdate_flag = TR69C_DATE_YES;

	sprintf(traceRouteDate.Resualtstate,"Complete");
#endif */
	return TRX_OK;
}
TRX_STATUS getTraceRouteDiagnostics_DiagnosticsState(char **value)
{/* 
	if(strlen(traceRouteDate.Resualtstate) > 5)
	{
		*value = strdup(traceRouteDate.Resualtstate);
	}else{
	
		*value = strdup("None");
	} */
	return TRX_OK;
}

/* ָ��������ϵĽӿڣ�WAN����LAN�ڣ��磺
InternetGatewayDevice.WANDevice.1.WANConnectionDevice.2.WANPPPConnection.1����
InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1 
*/
TRX_STATUS setTraceRouteDiagnostics_Interface(const char *value)
{
	/* sprintf(traceRouteCMD.interface,"%s",value); */
	return TRX_OK;
}
TRX_STATUS getTraceRouteDiagnostics_Interface(char **value)
{
	char actmp[256];
	memset(actmp, 0, sizeof(actmp));
	/* sprintf(actmp,"%s",traceRouteCMD.interface); */
	*value = strdup(actmp);
	return TRX_OK;
}

/* Host name or address of the host to find a route to */
TRX_STATUS setTraceRouteDiagnostics_Host(const char *value)
{
	/* sprintf(traceRouteCMD.host,"%s",value);

	//setTimer(TraceRoute_start,NULL,500);
	//���� tracerouteִ�е�ʱ��
	//g_pingWaiteTime = 60;
	slog(TR69C_LOG_ERR,"g_pingWaiteTime traceroute  : %d ",g_pingWaiteTime);
	//���� �����ϱ��ı�־
	sendDiagComplete = 1;

	g_diag_cmd_info.daig_type = 1;
	//�������»�ȡ���ݱ�ʾ  ����Ͻ�� ���ļ��ж�ȡ�� �ṹ�� traceRouteDate��
	traceRouteDate.getdate_flag = TR69C_DATE_YES;
	sprintf(traceRouteDate.Resualtstate,"Complete");
	 */
	return TRX_OK;
}
TRX_STATUS getTraceRouteDiagnostics_Host(char **value)
{
	/* char actmp[256];
	memset(actmp, 0, sizeof(actmp));
	sprintf(actmp,"%s",traceRouteCMD.host);
	*value = strdup(actmp); */
	return TRX_OK;
}

/* Number of tries per hop. Set prior to running
Diagnostic. By default, the CPE SHOULD set this
value to 3. */
TRX_STATUS setTraceRouteDiagnostics_NumberOfTries(const char *value)
{
	//sprintf(traceRouteCMD.NumberOfTries,"%s",(char *)value);
	/* traceRouteCMD.NumberOfTries = atoi(value); */
	return TRX_OK;
}
TRX_STATUS getTraceRouteDiagnostics_NumberOfTries(char **value)
{
	/* char actmp[16];
	memset(actmp, 0, sizeof(actmp));
	sprintf(actmp,"%d",traceRouteCMD.NumberOfTries);
	*value = strdup(actmp); */
	return TRX_OK;
}

/* Timeout in milliseconds for the trace route test. By
default the CPE SHOULD set this value to 5000. */
TRX_STATUS setTraceRouteDiagnostics_Timeout(const char *value)
{
	//sprintf(traceRouteCMD.Timeout,"%s",value);
	/* traceRouteCMD.Timeout= atoi(value); */
	return TRX_OK;
}
TRX_STATUS getTraceRouteDiagnostics_Timeout(char **value)
{
	/* char actmp[16];
	memset(actmp, 0, sizeof(actmp));
	sprintf(actmp,"%d",traceRouteCMD.Timeout);
	*value = strdup(actmp); */
	return TRX_OK;
}

/* Size of the data block in bytes to be sent for each
trace route. By default, the CPE SHOULD set this
value to 38. */
TRX_STATUS setTraceRouteDiagnostics_DataBlockSize(const char *value)
{
	//sprintf(traceRouteCMD.DataBlockSize,"%s",value);
	/* traceRouteCMD.DataBlockSize= atoi(value); */
	return TRX_OK;
}
TRX_STATUS getTraceRouteDiagnostics_DataBlockSize(char **value)
{
	/* char actmp[16];
	memset(actmp, 0, sizeof(actmp));
	sprintf(actmp,"%d",traceRouteCMD.DataBlockSize);
	*value = strdup(actmp); */
	return TRX_OK;
}

/* DiffServ codepoint to be used for the test packets.
By default the CPE SHOULD set this value to 0. */
TRX_STATUS setTraceRouteDiagnostics_DSCP(const char *value)
{
	//sprintf(traceRouteCMD.DSCP,"%s",value);
	/* traceRouteCMD.DSCP= atoi(value); */
	return TRX_OK;
}
TRX_STATUS getTraceRouteDiagnostics_DSCP(char **value)
{
	/* char actmp[16];
	memset(actmp, 0, sizeof(actmp));
	sprintf(actmp,"%d",traceRouteCMD.DSCP);
	*value = strdup(actmp); */
	return TRX_OK;
}

/* The maximum number of hop used in outgoing
probe packets (max TTL). By default the CPE
SHOULD set this value to 30. */
TRX_STATUS setTraceRouteDiagnostics_MaxHopCount(const char *value)
{
	//sprintf(traceRouteCMD.MaxHopCount,"%s",value);
	/* traceRouteCMD.MaxHopCount= atoi(value); */
	return TRX_OK;
}
TRX_STATUS getTraceRouteDiagnostics_MaxHopCount(char **value)
{/* 
	char actmp[16];
	memset(actmp, 0, sizeof(actmp));
	sprintf(actmp,"%d",traceRouteCMD.MaxHopCount);
	*value = strdup(actmp); */
	return TRX_OK;
}

/* Result parameter indicating the response time in
milliseconds the most recent trace route test. If a
route could not be determined, this value MUST be
zero. */
TRX_STATUS getTraceRouteDiagnostics_ResponseTime(char **value)
{
	char actmp[16];
	memset(actmp, 0, sizeof(actmp));
	sprintf(actmp,"%ld",g_diag_cmd_info.daig_time);
	if(atoi(actmp) == 0 )
	{
		*value = strdup("1");
	}else{
		*value = strdup(actmp);
	}
	return TRX_OK;
}

/* Number of entries in the RouteHops table. */
TRX_STATUS getTraceRouteDiagnostics_RouteHopsNumberOfEntries(char **value)
{	
	/* getTraceRouteResult();
	char actmp[16];
	memset(actmp, 0, sizeof(actmp));
	sprintf(actmp,"%d",traceRouteDate.HopsNumberOfEntries);
	*value = strdup(actmp); */
	return TRX_OK;
}
/* TraceRoute log  */
TRX_STATUS getTraceRouteDiagnostics_TraceRoutelog(char **value)//TR69C_TRACEROUTE_FILE
{
/*
	char pcOut[4096];
    FILE *pstFp = NULL;
	char acBuf[128] = {0};
	
   	pstFp= fopen(TR69C_TRACEROUTE_FILE, "r");
    if (!pstFp){
        slog(TR69C_LOG_ERR, "Open "" fail.");
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
			strcat(pcOut, acBuf);
			memset(acBuf, 0, sizeof(acBuf));
	 	}
	}
	
	fclose(pstFp);
	//printf("log ping : %d %s \n",i,pcOut);
	*value = strdup(pcOut);   
	return TRX_OK;
	*/
	/* char pcOut[4096]={0};
    FILE *pstFp = NULL;
	char acBuf[128] = {0};
	
	memset(pcOut, 0, sizeof(pcOut));
   	pstFp= fopen(TR69C_TRACEROUTE_FILE, "r");
    if (!pstFp){
        slog(TR69C_LOG_ERR, "Open "TR69C_TRACEROUTE_FILE" fail.");
		*value = strdup(""); 
		return TRX_OK;     
    }
	fseek(pstFp, 0, SEEK_SET);
	while(NULL != fgets(acBuf, sizeof(acBuf),pstFp))
	{
	 	if(0 != strlen(acBuf))
	 	{//printf("log ping buf = %s  \n",acBuf);
			acBuf[strlen(acBuf)-1] = ' ';
			acBuf[strlen(acBuf)-2] = ' ';
		//	acBuf[strlen(acBuf)] = ' ';
			strcat(pcOut, acBuf);
			memset(acBuf, 0, sizeof(acBuf));
	 	}
		
	}
	fclose(pstFp);
	*value = strdup(pcOut);   
	printf("log ping read \n");
	//free(pcOut);
	//printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>%s\n", pcOut); */
	return TRX_OK;
}
TRX_STATUS deleteTraceRouteDiagnosticsRouteHops(const char *value)
{
/*
	TRxObjNode *n = getCurrentNode();
	InstanceDesc *idp;
	int     id = atoi(value);
//		 && getInstanceCount(WANDeviceWANConnectionDeviceWANIPConnectionInstanceDesc)==0	
	if ( getInstanceCount(WANDeviceWANConnectionDeviceWANPPPConnectionInstanceDesc)== 0) {
		fprintf(stderr, "Child object NOT DELETED\n");
	}
	if (!deleteInstanceDesc(n,id)){
		fprintf(stderr, "Delete WANConnectionDevice Instance = %d\n", id);
		return TRX_REBOOT; *//* or TRX_BOOT if necessary */
//	}
//
	return TRX_OK;
}
TRX_STATUS addTraceRouteDiagnosticsRouteHops(char **value)
{
	InstanceDesc *idp;
	
	if ( (idp= getNewInstanceDesc( getCurrentNode(),getCurrentInstanceDesc(),TraceRouteHops_num+1)) != NULL ) 
	{
        *value = strdup(itoa(idp->instanceID));
		TraceRouteHops_num = TraceRouteHops_num+1;
        return TRX_REBOOT; /* or TRX_BOOT if necessary */
   	}
   return TRX_ERR;
}

/*������*/
TRX_STATUS getTraceRouteDiagnostics_Hops_HopHost(char **value)
{
	/* InstanceDesc *idp = getCurrentInstanceDesc();
    UINT32 index;
	char buf[128];
    index = idp->instanceID - 1;
	memset(buf, 0, sizeof(buf));
	getTraceRouteResult();
	sprintf(buf,"%s",traceRouteDate.hopsDate[idp->instanceID -1].HopHost);
	*value = strdup(buf); */
	return TRX_OK;
}
/*IP��ַ*/
TRX_STATUS getTraceRouteDiagnostics_Hops_HopHostAddress(char **value)
{
	/* InstanceDesc *idp = getCurrentInstanceDesc();
    UINT32 index;
	char buf[16];
    index = idp->instanceID - 1;
	memset(buf, 0, sizeof(buf));
	getTraceRouteResult();
	sprintf(buf,"%s",traceRouteDate.hopsDate[idp->instanceID -1].HopHostAddress);
	*value = strdup(buf); */
	return TRX_OK;
}

/* ������ error Code*/

TRX_STATUS getTraceRouteDiagnostics_Hops_HopErrorCode(char **value)
{
	/* InstanceDesc *idp = getCurrentInstanceDesc();
    UINT32 index;
	char buf[16];
    index = idp->instanceID - 1;
	memset(buf, 0, sizeof(buf));
	getTraceRouteResult();
	sprintf(buf,"%d",traceRouteDate.hopsDate[idp->instanceID -1].HopErrorCode);
	*value = strdup(buf); */
	return TRX_OK;
}
/*����ʱ��*/
TRX_STATUS getTraceRouteDiagnostics_Hops_HopRTTimes(char **value)
{
	/* InstanceDesc *idp = getCurrentInstanceDesc();
    UINT32 index;
	char buf[16];
    index = idp->instanceID - 1;
	memset(buf, 0, sizeof(buf));
	getTraceRouteResult();
	sprintf(buf,"%s",traceRouteDate.hopsDate[idp->instanceID -1].HopRTTimes);
	*value = strdup(buf); */
	return TRX_OK;
}


void initTraceRouteDiagnosticsInstances(void)
{
	TRxObjNode *n;
    int id;
    InstanceDesc  *traceRouteDiagnosticsIdp;
    InstanceDesc  *idp;
  //  InstanceDesc  *hopsInstance;
	
    n = TraceRouteDiagnosticsDesc; 
    id = 1;
    traceRouteDiagnosticsIdp = getNewInstanceDesc(n, NULL, id);
slog(TR69C_LOG_DEBUG,"start instances 8 inittraceRoute 0");

   	n = TraceRouteDiagnosticsRouteHopsDesc;
   	idp = getNewInstanceDesc(n,traceRouteDiagnosticsIdp,id);
/*
	id = 2;
	n = TraceRouteDiagnosticsRouteHopsDesc;
   	idp = getNewInstanceDesc(n,traceRouteDiagnosticsIdp,id);

	TraceRouteHops_num = id; 
*/
//TraceRouteDiagnosticsRouteHopsInstance
 /*	n = TraceRouteDiagnosticsRouteHopsInstance;
	hopsInstance  = getNewInstanceDesc(n,idp,id);
	id = 2;
	hopsInstance  = getNewInstanceDesc(n,idp,id);
	*/
/* slog(TR69C_LOG_DEBUG,"start instances 8 inittraceRoute 1"); */
}

/*****************   X_CMCC_PPPOE_EMULATOR   *******************/
//OM_ERROR_CODE_E  om_vpn_config_set_l2tp( OM_VPN_LNS_CFG_T *pstVpnLnsCfg )
/* 
���״̬������ֵ֮һ��
"None"  ��
"Start"  ��ʼ
"Stop"  ֹͣ
"Complete" ���
"Running"  ������
����Start��StopΪ��д������״ֻ̬����
*/
/* OM_VPN_LNS_CFG_T pstVpnLnsCfg;
OM_TR69C_PPPOE_EMULATOR_CFG_T 	g_pppoe_emulator_cfg; */
/* OM_TR69C_PPPOE_EMULATOR_INFO_T  g_pppoe_emulator_date; */
int pppoe_switch_flag;
//TR69C_PPPoE_EMULATOR_FILE
/*��ȡ ��� ��� */
void getPPPoEEmulator_result(void)
{
	/* char CMD_str[256];
	char acInfo[128] = {0}; 
	int enRet;

	if(g_pppoe_emulator_date.pppoe_getdate_flag == TR69C_DATE_YES)
	{
		//��ȡ  PPP0 �� IP
		memset(acInfo, 0, sizeof(acInfo));
   	 	memset(CMD_str, 0, sizeof(CMD_str));
		sprintf(CMD_str,"ifconfig  ppp0|grep addr|awk '{print $2}'|awk -F ':' '{print $2}'");
    	enRet = popen_get_info(CMD_str, acInfo, sizeof(CMD_str));
    	if (0 != enRet)
    	{
    		slog(TR69C_LOG_ERR," ifconfig  ppp0|grep addr|awk '{print $3}'|awk -F ':' '{print $2}' ERROR ");
			goto pppoe_err;
			return;
    	}else{
    		if(strlen(acInfo)<8)  //IP addr ̫�� ˵�� ��ȡ�� ��Ϣ ���� 
    		{
				goto pppoe_err;
			}
    		sprintf(g_pppoe_emulator_date.ExternalIPAddress,"%s",acInfo);
		}

		//��ȡ  PPP0 �� ����  
		memset(acInfo, 0, sizeof(acInfo));
   	 	memset(CMD_str, 0, sizeof(CMD_str));
		//sprintf(CMD_str,"cat "TR69C_TRACEROUTE_FILE"|grep Local |awk  '{print $2}'");
		sprintf(CMD_str,"ifconfig  ppp0|grep addr|awk '{print $3}'|awk -F ':' '{print $2}'");
    	enRet = popen_get_info(CMD_str, acInfo, sizeof(CMD_str));
    	if (0 != enRet)
    	{
    		slog(TR69C_LOG_ERR," ifconfig  ppp0|grep addr|awk '{print $3}'|awk -F ':' '{print $2}'  ERROR ");
			
    	}else{
    		sprintf(g_pppoe_emulator_date.DefaultGateway,"%s",acInfo);
		}
		//�ر� PPPoE ���  ��ԭ PPPoE������
		PPPOE_Emulator_Stop(pppoe_switch_flag);
		
		//���� �ϱ���־
		memset(acInfo, 0, sizeof(acInfo));
   	 	memset(CMD_str, 0, sizeof(CMD_str));
		sprintf(CMD_str,"awk 'NR<=33' "TR69C_PPPoE_EMULATOR_FILE" > "TR69C_PPPoE_EMULATOR_LOG_FILE);	
    	enRet = popen_get_info(CMD_str, acInfo, sizeof(CMD_str));
    	if (0 != enRet)
    	{
    		slog(TR69C_LOG_ERR," awk 'NR<=33' "TR69C_PPPoE_EMULATOR_FILE" > "TR69C_PPPoE_EMULATOR_LOG_FILE);
    	}
		system("sed -i 's/xl2tpd/log/g' "TR69C_PPPoE_EMULATOR_LOG_FILE);

		//ɾ�� ��־ 
	//	system(" rm -rf "TR69C_PPPoE_EMULATOR_LOG_FILE);
		//��ȡ �ػ�ID   Session ID
		memset(acInfo, 0, sizeof(acInfo));
   	 	memset(CMD_str, 0, sizeof(CMD_str));
		sprintf(CMD_str,"cat "TR69C_PPPoE_EMULATOR_LOG_FILE"|grep  Local|awk 'NR==2'|awk -F 'Local:' '{print $2}'|awk -F ',' '{print $1}'");
    	enRet = popen_get_info(CMD_str, acInfo, sizeof(CMD_str));
    	if (0 != enRet)
    	{
    		slog(TR69C_LOG_ERR," cat "TR69C_PPPoE_EMULATOR_LOG_FILE"|grep  Local|awk 'NR==2'|awk -F 'Local:' '{print $2}'|awk -F ',' '{print $1}' ");
			goto pppoe_err;
    	}else{
    		sprintf(g_pppoe_emulator_date.PPPSessionID,"%d",atoi(acInfo));
		}
		
		//���� ��� ���
		sprintf(g_pppoe_emulator_date.Result,"Success");
		sprintf(g_pppoe_emulator_date.state,"Complete");
		//����  �ϱ� ��Ϣ 
		g_pppoe_emulator_date.pppoe_getdate_flag = TR69C_DATE_No;//��ȡ������ flag  ����
	}
	return;

pppoe_err:
	sprintf(g_pppoe_emulator_date.Result,"Unknown");
	sprintf(g_pppoe_emulator_date.state,"Complete");
	g_pppoe_emulator_date.pppoe_getdate_flag = TR69C_DATE_No;//��ȡ������ flag  ���� */
	return;
	
}
/*
*	PPPoE�������:
*	1.nvram �ر� pppoe�ܿ���
*	2.����pppoe��ϵĲ���
*	3.�������
*	......
*	4.����� �ر�
*	5.��ԭ pppoe����
*/
void om_PPPOE_Emulator_Start(void)
{
	/* char CMD_str[256];
	char acInfo[128] = {0}; 
	int enRet;
	int count = 20;
	
	slog(TR69C_LOG_ERR,"AAAAAAA lnsL2tpHost = %s lnsL2tpPass = %s ",pstVpnLnsCfg.lnsHost,pstVpnLnsCfg.lnsPass);
	if(0 != PPPOE_Emulator_Start(&pstVpnLnsCfg, &pppoe_switch_flag))
	{
		slog(TR69C_LOG_ERR,"PPPOE_Emulator start fail !!! ");	
		sendDiagComplete = 1;
		g_pppoe_emulator_date.pppoe_getdate_flag = TR69C_DATE_No;
	}
slog(TR69C_LOG_ERR,"***************  0 ");	
	while(count >1)
	{
		memset(acInfo, 0, sizeof(acInfo));
   		memset(CMD_str, 0, sizeof(CMD_str));
		sprintf(CMD_str,"ifconfig ppp0");	
    	enRet = popen_get_info(CMD_str, acInfo, sizeof(CMD_str));
		if (0 != enRet)
    	{
    		slog(TR69C_LOG_ERR," PPPOE_Emulator NO ");	
    		count--;	
    	}else{
    		if(strlen(acInfo) > 10){

				slog(TR69C_LOG_ERR," PPPOE_Emulator ok = %s  !!! ",acInfo);	
				sprintf(g_pppoe_emulator_date.Result,"Success");
				g_pppoe_emulator_date.pppoe_getdate_flag = TR69C_DATE_YES;
				break;

			}else{
				count--;	
			}
		}
		sleep(1);
	}
	slog(TR69C_LOG_ERR,"***************  1 ");	
	if(count == 1)
	{
		slog(TR69C_LOG_ERR," PPPOE_Emulator NO time ");
		sprintf(g_pppoe_emulator_date.Result,"Unknown");
		g_pppoe_emulator_date.pppoe_getdate_flag = TR69C_DATE_No;
	}
	slog(TR69C_LOG_ERR,"***************  2 ");	 */
	return;
}
TRX_STATUS setPPPOE_Emulator_DiagnosticsState(const char *value)
{
	/* //��� һ��  �û���   ��  ���� 
	slog(TR69C_LOG_ERR,"lnsL2tpHost = %s lnsL2tpPass = %s ",pstVpnLnsCfg.lnsHost,pstVpnLnsCfg.lnsPass);
	//���� ʱ�� 500ms  ��  ���� ���
//	setTimer(om_PPPOE_Emulator_Start,NULL,500);
	//om_PPPOE_Emulator_Start(NULL);
	// ���� �����ϱ���ʱ�� 
	g_pingWaiteTime = 20;//1����
	slog(TR69C_LOG_ERR," PPPOE_Emulator waitetime  : %d ",g_pingWaiteTime);
	//���� �����ϱ��ı�־
	sendDiagComplete = 1;
	g_diag_cmd_info.daig_type =3;
	// ���� ��ȡ��Ϣ�� ��־ 
	g_pppoe_emulator_date.pppoe_getdate_flag = TR69C_DATE_YES; */
	return TRX_OK;
}
TRX_STATUS getPPPOE_Emulator_DiagnosticsState(char **value)
{
	/* if(strlen(g_pppoe_emulator_date.state)<3)
	{
		*value = strdup("None");
	}else{
		*value = strdup(g_pppoe_emulator_date.state);
	} */

	return TRX_OK;
}
/* pppoe��ϵ� */
TRX_STATUS setPPPOE_Emulator_Username(const char *value)
{
	/* sprintf(pstVpnLnsCfg.lnsHost,"%s",value);
	//traceRouteCMD.MaxHopCount= atoi(value);
	sendDiagComplete = 1;
	g_diag_cmd_info.daig_type =3; */
	return TRX_OK;
}
TRX_STATUS getPPPOE_Emulator_Username(char **value)
{
	/* char actmp[64];
	memset(actmp, 0, sizeof(actmp));
	sprintf(actmp,"%s",pstVpnLnsCfg.lnsHost);
	*value = strdup(actmp); */
	return TRX_OK;
}

/* pppoe��ϵ� */
TRX_STATUS setPPPOE_Emulator_Password(const char *value)
{
	/* sendDiagComplete = 1;
	g_diag_cmd_info.daig_type =3;
	sprintf(pstVpnLnsCfg.lnsPass,"%s",value);
	//traceRouteCMD.MaxHopCount= atoi(value); */
	return TRX_OK;
}
TRX_STATUS getPPPOE_Emulator_Password(char **value)
{
	/* char actmp[64];
	memset(actmp, 0, sizeof(actmp));
	sprintf(actmp,"%s",pstVpnLnsCfg.lnsPass);
	*value = strdup(actmp); */
	return TRX_OK;
}	

/* pppoe��ϵ� */
TRX_STATUS setPPPOE_Emulator_WANInterface(const char *value)
{
	/* sprintf(g_pppoe_emulator_cfg.WANinterface,"%s",value); */
	//traceRouteCMD.MaxHopCount= atoi(value);
	return TRX_OK;
}
TRX_STATUS getPPPOE_Emulator_WANInterface(char **value)
{
	/* char actmp[256];
	memset(actmp, 0, sizeof(actmp));
	sprintf(actmp,"%s",g_pppoe_emulator_cfg.WANinterface);
	*value = strdup(actmp); */
	return TRX_OK;
}


/* pppoe��ϵ� */
TRX_STATUS setPPPOE_Emulator_PPPAuthenticationProtocol(const char *value)
{
	/* sprintf(g_pppoe_emulator_cfg.PPPAuthenticationProtocol,"%s",value); */
	//traceRouteCMD.MaxHopCount= atoi(value);
	return TRX_OK;
}
TRX_STATUS getPPPOE_Emulator_PPPAuthenticationProtocol(char **value)
{/* 
	char actmp[16];
	memset(actmp, 0, sizeof(actmp));
	sprintf(actmp,"%s",g_pppoe_emulator_cfg.PPPAuthenticationProtocol);
	*value = strdup(actmp); */
	return TRX_OK;
}
 
/* pppoe��ϵ� */
TRX_STATUS setPPPOE_Emulator_RetryTimes(const char *value)
{
	//sprintf(g_pppoe_emulator_cfg.RetryTimes,"%s",value);
	/* g_pppoe_emulator_cfg.RetryTimes = atoi(value); */
	return TRX_OK;
}
TRX_STATUS getPPPOE_Emulator_RetryTimes(char **value)
{
	/* char actmp[16];
	memset(actmp, 0, sizeof(actmp));
	sprintf(actmp,"%d",g_pppoe_emulator_cfg.RetryTimes);
	*value = strdup(actmp); */
	return TRX_OK;
}

/*PPPoE��ϵĽ��
��Ͻ��������ֵ֮һ��
"Success" �ɹ�
"ParamNegoFail" Э�̲���ʧ��
"UserAuthenticationFail" ��֤ʧ��
"Timeout" ��ʱ
"UserStop" �û�ֹͣ
"Unknown" δ֪����

*/
TRX_STATUS getPPPOE_Emulator_Result(char **value)
{
	/* char actmp[16];
	memset(actmp, 0, sizeof(actmp));
	getPPPoEEmulator_result();
	sprintf(actmp,"%s",g_pppoe_emulator_date.Result);
	*value = strdup(actmp); */
	return TRX_OK;
}

TRX_STATUS getPPPOE_Emulator_PPPSessionID(char **value)
{
	/* char actmp[16];
	memset(actmp, 0, sizeof(actmp));
	getPPPoEEmulator_result();
	sprintf(actmp,"%s",g_pppoe_emulator_date.PPPSessionID);
	*value = strdup(actmp); */
	return TRX_OK;
}

TRX_STATUS getPPPOE_Emulator_ExternalIPAddress(char **value)
{
/* 	char actmp[16];
	memset(actmp, 0, sizeof(actmp));
	getPPPoEEmulator_result();
	sprintf(actmp,"%s",g_pppoe_emulator_date.ExternalIPAddress);
	*value = strdup(actmp); */
	return TRX_OK;
}	


TRX_STATUS getPPPOE_Emulator_DefaultGateway(char **value)
{
	/* char actmp[16];
	memset(actmp, 0, sizeof(actmp));
	getPPPoEEmulator_result();
	sprintf(actmp,"%s",g_pppoe_emulator_date.DefaultGateway);
	*value = strdup(actmp); */
	return TRX_OK;
}

TRX_STATUS getPPPOEEmulator_log(char **value)
{
	/* getPPPoEEmulator_result();
	char pcOut[4096];
    FILE *pstFp = NULL;
	char acBuf[128] = {0};
	memset(pcOut, 0, sizeof(pcOut));
   	pstFp= fopen(TR69C_PPPoE_EMULATOR_LOG_FILE, "r");
    if (!pstFp){
        slog(TR69C_LOG_ERR, "Open "TR69C_PPPoE_EMULATOR_LOG_FILE" fail.");
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
			strcat(pcOut, acBuf);
			memset(acBuf, 0, sizeof(acBuf));
	 	}
	}
	fclose(pstFp);
	slog(TR69C_LOG_ERR, "  file length = %d ",strlen(pcOut));
	*value = strdup(pcOut);   
	//free(pcOut); */
	return TRX_OK;
}
//altered by caibing 11-15
#if 0
void initX_PPPOE_EmulatorInstances(void)
{/* 
	TRxObjNode *n;
    int id;
    InstanceDesc  *PPPOE_EmulatorIdp;
	
    n = x_CMCC_PPPOE_EmulatorDesc; 
    id = 1;
    PPPOE_EmulatorIdp = getNewInstanceDesc(n, NULL, id);
slog(TR69C_LOG_DEBUG,"start instances 8 inittraceRoute 0"); */
}
#endif
