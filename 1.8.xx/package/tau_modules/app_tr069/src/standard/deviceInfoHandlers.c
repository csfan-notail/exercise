/*
 * Copyright (C) GBCOM ltd.,co 2015
 *
 * This is a part of project tr69cd, which implements TR069 protocal(aka
 * CWMP).
   设备相关的，通用接口实现
 * 
 * This is a PRIVATE software.
 * All rights reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include "utils.h"
#include "ap_types.h"

#include "om_tr69c.h"
#include "../SOAPParser/RPCState.h"
#include "informer.h"
#include "sysWrapper.h"
#include "deviceInfoParams.h".

/*Add by zhanghao 获取配置文件内容 2016/6/15 begin*/
#include "uci_config.h"
#include "tr69_lua.h"
/*Add by zhanghao 获取配置文件内容 end*/
#define TAUARRY_LEN 128
extern int g_log_level;
extern ACSState acsState;
extern char *informParameters[];
extern int informParameters_num;
extern LTEInfoState lteinfostate;
	
//TRxObjNode	DeviceInfoX_CMCC_TeleComAccountDesc[];


/* 功能描述  : 获取当前的CPU利用率*/
TRX_STATUS getLoadAverage(char **value)
{	
	char acCpu[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_SYS_CPU_USES,acCpu))
	{
		*value = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
	}
	else{
		*value = strdup(acCpu);
		free(acCpu);
	}
    return TRX_OK;
}

/* 获取当前内存使用率 */
/*
sysinfo()
*/
TRX_STATUS getMemory(char **value)
{
	char *acMemFree,acMemTotal;
	if(UCI_OK != uci_config_get(UCI_SYS_MEM_FREE,acMemFree))
	{
		*value = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
	}
	
	if(UCI_OK != uci_config_get(UCI_SYS_CPU_USES,acMemTotal))
	{
		*value = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
	}

	sprintf(value,"%d",*acMemFree/acMemTotal);
	free(acMemFree);
	free(acMemTotal);

    return TRX_OK;
}


/* 终端入网时间，即终端首次使用ntp获得的时间 */
TRX_STATUS getFirstUseDate(char **value)
{
	/* OM_APP_SYS_BASIC_INFO_T *pstSysBasicInfo=(OM_APP_SYS_BASIC_INFO_T *)malloc(sizeof(OM_APP_SYS_BASIC_INFO_T)); 
	 memset(pstSysBasicInfo, 0, sizeof(OM_APP_SYS_BASIC_INFO_T));
 	if(AP_E_NONE!=om_app_sys_basic_info_get(pstSysBasicInfo))
	{
		//slog(TR69C_LOG_DEBUG, "get system time failed");
		free(pstSysBasicInfo);
   		return TRX_ERR;
	}
	*value = strdup(pstSysBasicInfo->acSysTime);
	 free(pstSysBasicInfo); */
    return TRX_OK;
}

/* 规范版本 */
TRX_STATUS getSpecVersion(char **ppcValue)
{
//	*value = strdup("TR-069 Amendment 5");
	char acSpecVersion[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_DEVICEINFO_SPACVER,acSpecVersion))
	{
		*ppcValue = strdup("");
		slog(TR69C_LOG_ERR,"ERROR");
		return TRX_ERR;
	}
	else
	{
		 *ppcValue = strdup(acSpecVersion);				  
	}	
	
//	*value = strdup("version 1.0");
	return TRX_OK;
}

/* add by duanguiyuan 20150916 */
TRX_STATUS getX_CMCC_IMEI(char **value)
{

	/* if(strlen(lteinfostate.aucModuleIMEI) > 2)
	{
		*value = strdup(lteinfostate.aucModuleIMEI);
		 return TRX_OK;
	}
	OM_TR69C_LTE_MODULE_INFO_T *trLteModule = (OM_TR69C_LTE_MODULE_INFO_T *)malloc(sizeof(OM_TR69C_LTE_MODULE_INFO_T));
    memset(trLteModule, 0, sizeof(OM_TR69C_LTE_MODULE_INFO_T));

    if(OM_E_NONE != om_lte_tr69_module_info_get(trLteModule))    
    {
        free(trLteModule); 
        *value = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
    }
    *value = strdup((char*)trLteModule->aucModuleIMEI);  
	memcpy(lteinfostate.aucModuleIMEI,(char*)trLteModule->aucModuleIMEI,sizeof((char*)trLteModule->aucModuleIMEI));
printf("get aucModuleIMEI =%s 11111111 !!!!!\n",lteinfostate.aucModuleIMEI);
	free(trLteModule); */
    return TRX_OK;
}



/*add by wangminglin*/

TRX_STATUS getX_ATP_IMEI(char** ppcValue)
{	
	char acImei[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LTEINFO_SIM_INFO_IMSI,acImei))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acImei);
		
	}
	return TRX_OK;
}
TRX_STATUS setX_ATP_AntennaConfig(const char*pcValue)
{
	
	if(UCI_OK !=uci_config_set(UCI_DEVICEINFO_X_ATP_ANTENNACONFIG,pcValue))
	{
		 slog(TR69C_LOG_ERR,"ERROR");
		 return TRX_ERR;
	}
	return TRX_OK;
}

TRX_STATUS getX_ATP_AntennaConfig(char** ppcValue)
{	
	char aAntennaConfig[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_DEVICEINFO_X_ATP_ANTENNACONFIG,aAntennaConfig))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(aAntennaConfig);
		
	}
	return TRX_OK;
}
TRX_STATUS getX_ATP_PCBVersion(char** ppcValue)
{
	char aPcbVersion[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_DEVICEINFO_X_ATP_PCBVERSION,aPcbVersion))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(aPcbVersion);
		
	}
	return TRX_OK;
}
TRX_STATUS getX_ATP_LastSoftwareVersion(char** ppcValue)
{
	char aLastSoftwareVersion[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_DEVICEINFO_X_ATP_LASTSOFTVER,aLastSoftwareVersion))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(aLastSoftwareVersion);
		
	}
	return TRX_OK;
}
TRX_STATUS getX_ATP_ModemProductName(char** ppcValue)
{
	char aModemProductName[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_DEVICEINFO_X_ATP_MODEMPRODUCTNAME,aModemProductName))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(aModemProductName);
		
	}
	return TRX_OK;
}
TRX_STATUS getX_ATP_SupportVpnNum(char** ppcValue)
{
	char aSupportVpnNum[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_DEVICEINFO_X_ATP_SUPPORTVPNNUM,aSupportVpnNum))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(aSupportVpnNum);
		
	}
	return TRX_OK;
}
TRX_STATUS getX_ATP_BOMCode(char** ppcValue)
{
	char aBomCode[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_DEVICEINFO_X_ATP_BOMCLODE,aBomCode))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(aBomCode);
		
	}
	return TRX_OK;
}
TRX_STATUS getX_ATP_Temperature(char** ppcValue)
{
	char aTemperature[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_DEVICEINFO_X_ATP_TEMPERATRUE,aTemperature))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(aTemperature);
		
	}
	return TRX_OK;
}

TRX_STATUS getX_OUI_PLATFORM_VER(char** ppcValue)
{
	
		*ppcValue = strdup("5");
		return TRX_OK;
	
}

TRX_STATUS getX_OUI_PRIVATE_SOFTVER(char** ppcValue)
{
	
		*ppcValue = strdup("ap-ta40.21-1.1.2.1-16M-T-8386&#10;");
		return TRX_OK;
	
}


TRX_STATUS setX_ATP_PreserveEnable(const char*pcValue)
{
	//char *acPreseveEnable;
	//tau_config_PreserveEnable(value);
	
	if(UCI_OK !=uci_config_set(UCI_SYS_PRESERVE_ENABLE,pcValue))
	{
		 slog(TR69C_LOG_ERR,"ERROR");
		 return TRX_ERR;
		 
	}
	return TRX_OK;
}
TRX_STATUS getX_ATP_PreserveEnable(char** ppcValue)
{
	char acPreseveEnable[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_SYS_PRESERVE_ENABLE,acPreseveEnable))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	else{
		*ppcValue = strdup(acPreseveEnable);
		
	}	
	return TRX_OK;
}
TRX_STATUS getModemLogSessionStatus(char** ppcValue)
{
	char acModemLogSessionStatus[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_EXT_MODEMLOGSESSDIONSTATUS,acModemLogSessionStatus))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	else
	{
		*ppcValue = strdup(acModemLogSessionStatus);
	}	
	return TRX_OK;
}
TRX_STATUS getX_ATP_EXTCellID(char** ppcValue)
{
	char acCellID[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_EXT_CELLID,acCellID))
	{
		*ppcValue = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else
	{
		*ppcValue = strdup(acCellID);
	}	
	return TRX_OK;
}



TRX_STATUS getDownloadProgress(char** ppcValue)
{
	char acDownloadProgress[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_EXT_CELLID,acDownloadProgress))
	{
		*ppcValue = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acDownloadProgress);
	
	}	
	return TRX_OK;
}
TRX_STATUS getForwardNumberOfEntries(char** ppcValue)
{
	char acForwardNumberOfEntries[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_EXT_CELLID,acForwardNumberOfEntries))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acForwardNumberOfEntries);
	}	
	return TRX_OK;
}





TRX_STATUS setForwardingDestIPAddress(const char*pcValue)
{
	
	if(UCI_OK !=uci_config_set(UCI_LAYER3FORWARDING_DETIPADDR,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	return TRX_OK;
}
TRX_STATUS getForwardingDestIPAddress(char** ppcValue)
{
	char acForwardingDestIPAddress[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LAYER3FORWARDING_DETIPADDR,acForwardingDestIPAddress))
	{
		*ppcValue = strdup("0.0.0.0");
        slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	else{
		*ppcValue = strdup(acForwardingDestIPAddress);

	}	
	return TRX_OK;
}



TRX_STATUS setForwardingDestSubnetMask(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LAYER3FORWARDING_DESTSUBNETMASK,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	return TRX_OK;
}
TRX_STATUS getForwardingDestSubnetMask(char** ppcValue)
{
	char acForwardingDestSubnetMask[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LAYER3FORWARDING_DESTSUBNETMASK,acForwardingDestSubnetMask))
	{
		*ppcValue = strdup("255.255.255.255");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acForwardingDestSubnetMask);
	}	
	return TRX_OK;
}



TRX_STATUS setForwardingGatewayIPAddress(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LAYER3FORWARDING_GATEWAYIPADDRESS,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getForwardingGatewayIPAddress(char** ppcValue)
{
	char acForwardingGatewayIPAddress[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LAYER3FORWARDING_GATEWAYIPADDRESS,acForwardingGatewayIPAddress))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else
	{
		*ppcValue = strdup(acForwardingGatewayIPAddress);

	}	
	return TRX_OK;
}



TRX_STATUS setForwardingInterface(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LAYER3FORWARDING_INTERFACE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	return TRX_OK;
}
TRX_STATUS getForwardingInterface(char** ppcValue)
{
	char acForwardingInterface[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LAYER3FORWARDING_INTERFACE,acForwardingInterface))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acForwardingInterface);
	}	
	return TRX_OK;
}



TRX_STATUS setRsrpWeakBoundary(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_SYSTEMMONITOR_RSRPWEAKBOUNDARY,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	return TRX_OK;
}
TRX_STATUS getRsrpWeakBoundary(char** ppcValue)
{
	char acRsrpWeakBoundary[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_SYSTEMMONITOR_RSRPWEAKBOUNDARY,acRsrpWeakBoundary))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acRsrpWeakBoundary);
	}	
	return TRX_OK;
}

TRX_STATUS setFreqLock_Mode(const char*pcValue)
{

	return TRX_OK;
}
TRX_STATUS getFreqLock_Mode(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setFreqBand_Info(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getFreqBand_Info(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setFreqLock_FreqValue(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getFreqLock_FreqValue(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setFreqLock_Cell(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getFreqLock_Cell(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setVXLAN_LACEnable(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getVXLAN_LACEnable(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setVXLAN_LACVxlanRemoteIpAddress(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getVXLAN_LACVxlanRemoteIpAddress(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setL2GRE_LACEnable(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getL2GRE_LACEnable(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setL2GRE_LACGreRemoteIpAddress(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getL2GRE_LACGreRemoteIpAddress(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setL2GRE_LACVlanEnable(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getL2GRE_LACVlanEnable(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setL2GRE_LACDscpMarkMode(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getL2GRE_LACDscpMarkMode(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setL2GRE_LACARPDscpMark(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getL2GRE_LACARPDscpMark(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setL2GRE_LACIPMapVlanPrioEnable(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getL2GRE_LACIPMapVlanPrioEnable(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setVlanSettingsInterface(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getVlanSettingsInterface(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setVlanSettingsVlanType(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getVlanSettingsVlanType(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setVlanSettingsAdPvidFdEnable(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getVlanSettingsAdPvidFdEnable(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setVlanSettingsPvid(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getVlanSettingsPvid(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setFirewallMainSwitch(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getFirewallMainSwitch(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setFirewallIPFilterSwitch(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getFirewallIPFilterSwitch(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setFirewallWanPortPingSwitch(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getFirewallWanPortPingSwitch(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setFirewallWanWebSwitch(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getFirewallWanWebSwitch(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setfirewallmacfilterswitch(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getfirewallmacfilterswitch(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setfirewallurlfilterswitch(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getfirewallurlfilterswitch(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setfirewallfwstateswitch(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getfirewallfwstateswitch(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setL2TP_LACHostName(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getL2TP_LACHostName(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setL2TP_LACPassword(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getL2TP_LACPassword(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setL2TP_LACEnable(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getL2TP_LACEnable(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setL2TP_LACBCPEnable(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getL2TP_LACBCPEnable(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setL2TP_LACWanInterface(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getL2TP_LACWanInterface(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setL2TP_LACLNSAddress(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getL2TP_LACLNSAddress(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setL2TP_LACPCAuthMode(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getL2TP_LACPCAuthMode(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setL2TP_LACPppUser(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getL2TP_LACPppUser(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setL2TP_LACPppPass(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getL2TP_LACPppPass(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setL2TP_LACKeepAliveTime(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getL2TP_LACKeepAliveTime(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getX_ATP_AlarmInfoSequenceNo(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getAlarmInstanceDescription(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getAlarmInstanceLiftDescription(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setdialuproam_connect(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getdialuproam_connect(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setdialupcurrent_profile(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getdialupcurrent_profile(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getdialupprofileindex(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setdialupprofile_name(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getdialupprofile_name(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setdialupapn(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getdialupapn(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setdialupusername(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getdialupusername(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setdialuppassword(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getdialuppassword(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setdialupauth_mode(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getdialupauth_mode(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setVoiceServiceX_ATP_UmtsDTMFMethod(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getVoiceServiceX_ATP_UmtsDTMFMethod(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getVoiceProfileDTMFMethod(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setLineEnable(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getLineEnable(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setLineDirectoryNumber(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getLineDirectoryNumber(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getLineStatus(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setLineX_ATP_ClipEnable(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getLineX_ATP_ClipEnable(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getListCodec(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setSipAuthUserName(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getSipAuthUserName(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setSipAuthPassword(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getSipAuthPassword(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setSipProxyServer(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getSipProxyServer(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setSipProxyServerPort(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getSipProxyServerPort(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setSipRegistrarServer(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getSipRegistrarServer(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setSipRegistrarServerPort(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getSipRegistrarServerPort(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setSipX_ATP_SIPDomain(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getSipX_ATP_SIPDomain(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setSipX_SecondProxyServer(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getSipX_SecondProxyServer(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setSipX_SecondProxyServerPort(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getSipX_SecondProxyServerPort(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setSipX_SecondSIPDomain(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getSipX_SecondSIPDomain(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setSipX_SecondRegistrarServer(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getSipX_SecondRegistrarServer(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setSipX_SecondRegistrarServerPort(const char*pcValue)
{
	return TRX_OK;
}
TRX_STATUS getSipX_SecondRegistrarServerPort(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getVoiceX_ATP_Voipcsselect(char** ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getVoiceX_ATP_CIDSendType(char** ppcValue)
{
	
	return TRX_OK;
}

TRX_STATUS setLanCheckHost(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_SYSTEMMONITORLANCHECK_HOST,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	return TRX_OK;
}
TRX_STATUS getLanCheckHost(char** ppcValue)
{
	char acLanCheckHost[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_SYSTEMMONITORLANCHECK_HOST,acLanCheckHost))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acLanCheckHost);
	}	
	return TRX_OK;
}



TRX_STATUS setLanCheckStatus(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_SYSTEMMONITORLANCHECK_STATUS,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getLanCheckStatus(char** ppcValue)
{
	char acLanCheckStatus[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_SYSTEMMONITORLANCHECK_STATUS,acLanCheckStatus))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else
	{
		*ppcValue = strdup(acLanCheckStatus);
	
	}	
	return TRX_OK;
}



TRX_STATUS setWanCheckHost(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_SYSTEMMONITORWANCHECK_HOST,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	return TRX_OK;
}
TRX_STATUS getWanCheckHost(char** ppcValue)
{
	char acWanCheckHost[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_SYSTEMMONITORWANCHECK_HOST,acWanCheckHost))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWanCheckHost);
	}	
	return TRX_OK;
}



TRX_STATUS setWanCheckStatus(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_SYSTEMMONITORWANCHECK_STATUS,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	return TRX_OK;
}
TRX_STATUS getWanCheckStatus(char** ppcValue)
{
	char acWanCheckStatus[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_SYSTEMMONITORWANCHECK_STATUS,acWanCheckStatus))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWanCheckStatus);
	}	
	return TRX_OK;
}



TRX_STATUS setSysMonitorStatus(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_SYSTEMMONITORSYSTEMMONITOR_STATUS,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getSysMonitorStatus(char** ppcValue)
{
	char acSysMonitorStatus[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_SYSTEMMONITORSYSTEMMONITOR_STATUS,acSysMonitorStatus))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acSysMonitorStatus);
	}	
	return TRX_OK;
}



TRX_STATUS setX_ATP_ModemLogStatus(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_MODEMLOG_STATUS,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getX_ATP_ModemLogStatus(char** ppcValue)
{
	char acX_ATP_ModemLogStatus[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_MODEMLOG_STATUS,acX_ATP_ModemLogStatus))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_ModemLogStatus);
		
	}	
	return TRX_OK;
}



TRX_STATUS setParameterName(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_PRESERVEDATA_PARAMNAME,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getParameterName(char** ppcValue)
{
	char acParameterName[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_PRESERVEDATA_PARAMNAME,acParameterName))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acParameterName);
	}	
	return TRX_OK;
}


TRX_STATUS setX_ATP_WebUsername(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_GATEWAYUSERINTERFACE_X_ATP_WEB_USERINFO_USERNAME,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getX_ATP_WebUsername(char** ppcValue)
{
	char acX_ATP_WebUsername[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_GATEWAYUSERINTERFACE_X_ATP_WEB_USERINFO_USERNAME,acX_ATP_WebUsername))
	{
		*ppcValue = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_WebUsername);
		
	}	
	return TRX_OK;
}



TRX_STATUS setX_ATP_WebUserpassword(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_GATEWAYUSERINTERFACE_X_ATP_WEB_USERINFO_USERPASSWORD,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}

	return TRX_OK;
}
TRX_STATUS getX_ATP_WebUserpassword(char** ppcValue)
{
	char acX_ATP_WebUserpassword[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_GATEWAYUSERINTERFACE_X_ATP_WEB_USERINFO_USERPASSWORD,acX_ATP_WebUserpassword))
	{
		*ppcValue = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_WebUserpassword);

	}	
	return TRX_OK;
}

TRX_STATUS setX_ATP_FireWallCurrentLevel(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_FIRTEWALL_CURRENTLEVEL,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	return TRX_OK;
}
TRX_STATUS getX_ATP_FireWallCurrentLevel(char** ppcValue)
{
	char acX_ATP_FireWallCurrentLevel[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_CURRENTLEVEL,acX_ATP_FireWallCurrentLevel))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_FireWallCurrentLevel);
	}	
	return TRX_OK;
}


/*InternetGatewayDevice.X_ATP_FireWall.FilterMode.{i}.*/
TRX_STATUS getFilterModeLevel(char** ppcValue)
{
	char acFilterModeLevel[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_FILTERMODE_LEVEL,acFilterModeLevel))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
	}
	else{
		*ppcValue = strdup(acFilterModeLevel);
		
	}	
	return TRX_OK;
}



TRX_STATUS setMACMode(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_FIRTEWALL_FILTERMODE_MACMODE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getMACMode(char** ppcValue)
{
	char acMACMode[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_FILTERMODE_MACMODE,acMACMode))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acMACMode);

	}	
	return TRX_OK;
}


TRX_STATUS setIPMode(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_FIRTEWALL_FILTERMODE_IPMODE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getIPMode(char** ppcValue)
{
	char acIPMode[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_FILTERMODE_IPMODE,acIPMode))
	{
		*ppcValue = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acIPMode);
	
	}	
	return TRX_OK;
}


TRX_STATUS setURLMode(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_FIRTEWALL_FILTERMODE_URLMODE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getURLMode(char** ppcValue)
{
	char acURLMode[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_FILTERMODE_URLMODE,acURLMode))
	{
		*ppcValue = strdup("0.0.0.0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acURLMode);
	
	}	
	return TRX_OK;
}

/*InternetGatewayDevice.X_ATP_FireWall.UrlBlackFilter.{i}.*/
TRX_STATUS setBlackFilterUrl(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_FIRTEWALL_URLBLACKFILTER_URL,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getBlackFilterUrl(char** ppcValue)
{
	char acBlackFilterUrl[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_URLBLACKFILTER_URL,acBlackFilterUrl))
	{
		*ppcValue = strdup("0.0.0.0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acBlackFilterUrl);
	
	}	
	return TRX_OK;
}



/*InternetGatewayDevice.X_ATP_FireWall.MacBlackFilter.{i}.*/
TRX_STATUS setBlackFilterSourceMACAddress(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_FIRTEWALL_MACBLACKFILTER_SOURCEMACADDR,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getBlackFilterSourceMACAddress(char** ppcValue)
{
	char acBlackFilterSourceMACAddress[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_MACBLACKFILTER_SOURCEMACADDR,acBlackFilterSourceMACAddress))
	{
		*ppcValue = strdup("00-00-00-00-00-00");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acBlackFilterSourceMACAddress);
		
	}	
	return TRX_OK;
}



/*InternetGatewayDevice.X_ATP_FireWall.IpBlackFilter.{i}.*/

TRX_STATUS setBlackFilterSourceIPStart(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_FIRTEWALL_IPBLACKFILTER_SOURCEIPSTART,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getBlackFilterSourceIPStart(char** ppcValue)
{
	char acBlackFilterSourceIPStart[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_IPBLACKFILTER_SOURCEIPSTART,acBlackFilterSourceIPStart))
	{
		*ppcValue = strdup("0.0.0.0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acBlackFilterSourceIPStart);
		
	}	
	return TRX_OK;
}



TRX_STATUS setBlackFilterSourceIPEnd(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_FIRTEWALL_IPBLACKFILTER_SOURCEIPEnd,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getBlackFilterSourceIPEnd(char** ppcValue)
{
	char acBlackFilterSourceIPEnd[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_IPBLACKFILTER_SOURCEIPEnd,acBlackFilterSourceIPEnd))
	{
		*ppcValue = strdup("0.0.0.0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acBlackFilterSourceIPEnd);
		
	}	
	return TRX_OK;
}

TRX_STATUS setBlackFilterDestIPStart(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_FIRTEWALL_IPBLACKFILTER_DESTIPSTART,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getBlackFilterDestIPStart(char** ppcValue)
{
	char acBlackFilterDestIPStart[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_IPBLACKFILTER_DESTIPSTART,acBlackFilterDestIPStart))
	{
		*ppcValue = strdup("0.0.0.0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acBlackFilterDestIPStart);
		
	}	
	return TRX_OK;
}

TRX_STATUS setBlackFilterDestIPEnd(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_FIRTEWALL_IPBLACKFILTER_DESTIPEND,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getBlackFilterDestIPEnd(char** ppcValue)
{
	char acBlackFilterDestIPEnd[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_IPBLACKFILTER_DESTIPEND,acBlackFilterDestIPEnd))
	{
		*ppcValue = strdup("0.0.0.0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acBlackFilterDestIPEnd);
		
	}	
	return TRX_OK;
}

TRX_STATUS setBlackFilterProtocol(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_FIRTEWALL_IPBLACKFILTER_PROTOCOL,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getBlackFilterProtocol(char** ppcValue)
{
	char acBlackFilterProtocol[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_IPBLACKFILTER_PROTOCOL,acBlackFilterProtocol))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acBlackFilterProtocol);
		
	}	
	return TRX_OK;
}


TRX_STATUS setBlackFilterSourcePortStart(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_FIRTEWALL_IPBLACKFILTER_SOURCEPORTSTART,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getBlackFilterSourcePortStart(char** ppcValue)
{
	char acBlackFilterSourcePortStart[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_IPBLACKFILTER_SOURCEPORTSTART,acBlackFilterSourcePortStart))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acBlackFilterSourcePortStart);
		
	}	
	return TRX_OK;
}

TRX_STATUS setBlackFilterSourcePortEnd(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_FIRTEWALL_IPBLACKFILTER_SOURCEPORTEND,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	

	return TRX_OK;
}
TRX_STATUS getBlackFilterSourcePortEnd(char** ppcValue)
{
	char acBlackFilterSourcePortEnd[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_IPBLACKFILTER_SOURCEPORTEND,acBlackFilterSourcePortEnd))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acBlackFilterSourcePortEnd);
		
	}	
	return TRX_OK;
}


TRX_STATUS setBlackFilterDestPortStart(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_FIRTEWALL_IPBLACKFILTER_DESTPORTSTART,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getBlackFilterDestPortStart(char** ppcValue)
{
	char acBlackFilterDestPortStart[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_IPBLACKFILTER_DESTPORTSTART,acBlackFilterDestPortStart))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acBlackFilterDestPortStart);
		
	}	
	return TRX_OK;
}


TRX_STATUS setBlackFilterDestPortEnd(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_FIRTEWALL_IPBLACKFILTER_DESTPORTEND,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	
	return TRX_OK;
}
TRX_STATUS getBlackFilterDestPortEnd(char** ppcValue)
{
	char acBlackFilterDestPortEnd[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_IPBLACKFILTER_DESTPORTEND,acBlackFilterDestPortEnd))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acBlackFilterDestPortEnd);
	
	}	
	return TRX_OK;
}



/*InternetGatewayDevice.X_ATP_FireWall.UrlWhiteFilter.{i}.*/


TRX_STATUS setWhiteFilterUrl(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_FIRTEWALL_URLWHITEFILTER_URL,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getWhiteFilterUrl(char** ppcValue)
{
	char acWhiteFilterUrl[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_URLWHITEFILTER_URL,acWhiteFilterUrl))
	{
		*ppcValue = strdup("0.0.0.0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWhiteFilterUrl);
		
	}	
	return TRX_OK;
}



/*InternetGatewayDevice.X_ATP_FireWall.MacWhiteFilter.{i}.*/


TRX_STATUS setWhiteFilterSourceMACAddress(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_FIRTEWALL_MACWHITEFILTER_SOURCEMACADDR,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getWhiteFilterSourceMACAddress(char** ppcValue)
{
	char acWhiteFilterSourceMACAddress[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_MACWHITEFILTER_SOURCEMACADDR,acWhiteFilterSourceMACAddress))
	{
		*ppcValue = strdup("00-00-00-00-00-00");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWhiteFilterSourceMACAddress);
	
	}	
	return TRX_OK;
}





/*InternetGatewayDevice.X_ATP_FireWall.IpWhiteFilter.{i}.*/

TRX_STATUS setWhiteFilterSourceIPStart(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_FIRTEWALL_IPWHITEFILTER_SOURCEIPSTART,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getWhiteFilterSourceIPStart(char** ppcValue)
{
	char acWhiteFilterSourceIPStart[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_IPWHITEFILTER_SOURCEIPSTART,acWhiteFilterSourceIPStart))
	{
		*ppcValue = strdup("0.0.0.0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWhiteFilterSourceIPStart);
	
	}	
	return TRX_OK;
}



TRX_STATUS setWhiteFilterSourceIPEnd(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_FIRTEWALL_IPWHITEFILTER_SOURCEIPEnd,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getWhiteFilterSourceIPEnd(char** ppcValue)
{
	char acWhiteFilterSourceIPEnd[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_IPWHITEFILTER_SOURCEIPEnd,acWhiteFilterSourceIPEnd))
	{
		*ppcValue = strdup("0.0.0.0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWhiteFilterSourceIPEnd);
		
	}	
	return TRX_OK;
}

TRX_STATUS setWhiteFilterDestIPStart(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_FIRTEWALL_IPWHITEFILTER_DESTIPSTART,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getWhiteFilterDestIPStart(char** ppcValue)
{
	char acWhiteFilterDestIPStart[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_IPWHITEFILTER_DESTIPSTART,acWhiteFilterDestIPStart))
	{
		*ppcValue = strdup("0.0.0.0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWhiteFilterDestIPStart);
		
	}	
	return TRX_OK;
}

TRX_STATUS setWhiteFilterDestIPEnd(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_FIRTEWALL_IPWHITEFILTER_DESTIPEND,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getWhiteFilterDestIPEnd(char** ppcValue)
{
	char acWhiteFilterDestIPEnd[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_IPWHITEFILTER_DESTIPEND,acWhiteFilterDestIPEnd))
	{
		*ppcValue = strdup("0.0.0.0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWhiteFilterDestIPEnd);
		
	}	
	return TRX_OK;
}

TRX_STATUS setWhiteFilterProtocol(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_FIRTEWALL_IPWHITEFILTER_PROTOCOL,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getWhiteFilterProtocol(char** ppcValue)
{
	char acWhiteFilterProtocol[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_IPWHITEFILTER_PROTOCOL,acWhiteFilterProtocol))
	{
		*ppcValue = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWhiteFilterProtocol);
	}	
	return TRX_OK;
}


TRX_STATUS setWhiteFilterSourcePortStart(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_FIRTEWALL_IPWHITEFILTER_SOURCEPORTSTART,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getWhiteFilterSourcePortStart(char** ppcValue)
{
	char acWhiteFilterSourcePortStart[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_IPWHITEFILTER_SOURCEPORTSTART,acWhiteFilterSourcePortStart))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWhiteFilterSourcePortStart);
		
	}	
	return TRX_OK;
}

TRX_STATUS setWhiteFilterSourcePortEnd(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_FIRTEWALL_IPWHITEFILTER_SOURCEPORTEND,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getWhiteFilterSourcePortEnd(char** ppcValue)
{
	char acWhiteFilterSourcePortEnd[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_IPWHITEFILTER_SOURCEPORTEND,acWhiteFilterSourcePortEnd))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWhiteFilterSourcePortEnd);
		
	}	
	return TRX_OK;
}


TRX_STATUS setWhiteFilterDestPortStart(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_FIRTEWALL_IPWHITEFILTER_DESTPORTSTART,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getWhiteFilterDestPortStart(char** ppcValue)
{
	char acWhiteFilterDestPortStart[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_IPWHITEFILTER_DESTPORTSTART,acWhiteFilterDestPortStart))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWhiteFilterDestPortStart);
	
	}	
	return TRX_OK;
}


TRX_STATUS setWhiteFilterDestPortEnd(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_FIRTEWALL_IPWHITEFILTER_DESTPORTEND,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getWhiteFilterDestPortEnd(char** ppcValue)
{
	char acWhiteFilterDestPortEnd[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_IPWHITEFILTER_DESTPORTEND,acWhiteFilterDestPortEnd))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWhiteFilterDestPortEnd);
	
	}	
	return TRX_OK;
}




/*InternetGatewayDevice.X_ATP_FireWall.AccessControl.{i}.*/

TRX_STATUS getService(char** ppcValue)
{
	char acService[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_ACCESSCONTROL_SERVICE,acService))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acService);
		
	}	
	return TRX_OK;
}



TRX_STATUS getDirection(char** ppcValue)
{
	char acDirection[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_ACCESSCONTROL_DIRECTION,acDirection))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acDirection);
		
	}	
	return TRX_OK;
}


TRX_STATUS setStartIpAddr(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_FIRTEWALL_ACCESSCONTROL_STARTIPADDR,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getStartIpAddr(char** ppcValue)
{
	char acStartIpAddr[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_ACCESSCONTROL_STARTIPADDR,acStartIpAddr))
	{
		*ppcValue = strdup("0.0.0.0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acStartIpAddr);
		
	}	
	return TRX_OK;
}

TRX_STATUS setEndIpAddr(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_FIRTEWALL_ACCESSCONTROL_ENDIPADDR,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getEndIpAddr(char** ppcValue)
{
	char acEndIpAddr[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_ACCESSCONTROL_ENDIPADDR,acEndIpAddr))
	{
		*ppcValue = strdup("0.0.0.0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acEndIpAddr);
		
	}	
	return TRX_OK;
}


TRX_STATUS setAccessControlEnable(const char*pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_X_ATP_FIRTEWALL_ACCESSCONTROL_ENABLE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getAccessControlEnable(char** ppcValue)
{

	char acAccessControlEnable[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_X_ATP_FIRTEWALL_ACCESSCONTROL_ENABLE,acAccessControlEnable))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acAccessControlEnable);
		
	}	
	return TRX_OK;
}







TRX_STATUS getX_CMCC_ConfigVersion(char **value)
{

/* 	OM_TR69C_DEVICE_PARAM_T *stTr69cParam = (OM_TR69C_DEVICE_PARAM_T *)malloc(sizeof(OM_TR69C_DEVICE_PARAM_T));  
    memset(stTr69cParam, 0, sizeof(OM_TR69C_DEVICE_PARAM_T));
    stTr69cParam->eFlag = TR69C_PARAM_SOFT_WARE;  
    if(OM_E_NONE != om_tr69c_get_devinfo(stTr69cParam))    
    {
        slog(TR69C_LOG_ERR,"ERROR");
        *value = strdup("NONE");
    }
    else
    {
        *value = strdup(stTr69cParam->acSoftVer);
    }
    free(stTr69cParam);
 */
    return TRX_OK;
}
TRX_STATUS getX_CMCC_ModuleVersion(char **value)
{
/*
	OM_TR69C_LTE_MODULE_INFO_T *trLteModule = (OM_TR69C_LTE_MODULE_INFO_T *)malloc(sizeof(OM_TR69C_LTE_MODULE_INFO_T));
    memset(trLteModule, 0, sizeof(OM_TR69C_LTE_MODULE_INFO_T));

    if(OM_E_NONE != om_lte_tr69_module_info_get(trLteModule))    
    {
        free(trLteModule); 
        *value = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
    }
    *value = strdup((char*)trLteModule->aucModuleVer);  
    free(trLteModule);
*/
	*value = strdup(lteinfostate.aucModuleVer);
    return TRX_OK;
}
TRX_STATUS getX_CMCC_ModuleType(char **value)
{
/*
	OM_TR69C_LTE_MODULE_INFO_T *trLteModule = (OM_TR69C_LTE_MODULE_INFO_T *)malloc(sizeof(OM_TR69C_LTE_MODULE_INFO_T));
    memset(trLteModule, 0, sizeof(OM_TR69C_LTE_MODULE_INFO_T));

    if(OM_E_NONE != om_lte_tr69_module_info_get(trLteModule))    
    {
        free(trLteModule); 
        *value = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
    }
    *value = strdup((char*)trLteModule->aucModuleType);  
    free(trLteModule);
*/
	*value = strdup(lteinfostate.aucModuleType);
    return TRX_OK;
}



TRX_STATUS setDeviceInfoX_CMCC_TeleComAccountEnable(const char *value)
{
	return TRX_OK;

}

TRX_STATUS setDeviceInfoX_CMCC_TeleComAccountUsername(const char *value)
{
	/* OM_TR69C_DEVICE_PARAM_T *stTr69cParam = (OM_TR69C_DEVICE_PARAM_T *)malloc(sizeof(OM_TR69C_DEVICE_PARAM_T));
    memset(stTr69cParam, 0, sizeof(OM_TR69C_DEVICE_PARAM_T));
    stTr69cParam->eFlag = TR69C_PARAM_USERNAME;  
	memcpy(stTr69cParam->Username,value,strlen(value));
	
//	sprintf(stTr69cParam.Username, "%s",value);
    if(OM_E_NONE != om_tr69c_set_devinfo(stTr69cParam))    
    {
        free(stTr69cParam); 
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
    } 
    free(stTr69cParam); */
    return TRX_OK;
}


TRX_STATUS setDeviceInfoX_CMCC_TeleComAccountPassword(const char *value)
{
	/* OM_TR69C_DEVICE_PARAM_T *stTr69cParam = (OM_TR69C_DEVICE_PARAM_T *)malloc(sizeof(OM_TR69C_DEVICE_PARAM_T));
    memset(stTr69cParam, 0, sizeof(OM_TR69C_DEVICE_PARAM_T));
    stTr69cParam->eFlag = TR69C_PARAM_PASSWORD;  
	memcpy(stTr69cParam->Password,value,strlen(value));

//	sprintf(stTr69cParam.Password, "%s",value);

    if(OM_E_NONE != om_tr69c_set_devinfo(stTr69cParam))    
    {
        free(stTr69cParam); 
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
    } 
    free(stTr69cParam); */
    return TRX_OK;

}

TRX_STATUS getDeviceInfoX_CMCC_TeleComAccountEnable(char **value)
{
	*value = strdup("1");
	return TRX_OK;
}
TRX_STATUS getDeviceInfoX_CMCC_TeleComAccountUsername(char **value)
{
    /* OM_TR69C_DEVICE_PARAM_T *stTr69cParam = (OM_TR69C_DEVICE_PARAM_T *)malloc(sizeof(OM_TR69C_DEVICE_PARAM_T));
    memset(stTr69cParam, 0, sizeof(OM_TR69C_DEVICE_PARAM_T));
    stTr69cParam->eFlag = TR69C_PARAM_USERNAME;  
    if(OM_E_NONE != om_tr69c_get_devinfo(stTr69cParam))    
    {
        free(stTr69cParam); 
        *value = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    *value = strdup(stTr69cParam->Username);  
    free(stTr69cParam); */

    return TRX_OK;

}
TRX_STATUS getDeviceInfoX_CMCC_TeleComAccountPassword(char **value)
{
    /* OM_TR69C_DEVICE_PARAM_T *stTr69cParam = (OM_TR69C_DEVICE_PARAM_T *)malloc(sizeof(OM_TR69C_DEVICE_PARAM_T));
    memset(stTr69cParam, 0, sizeof(OM_TR69C_DEVICE_PARAM_T));
    stTr69cParam->eFlag = TR69C_PARAM_PASSWORD;  
    if(OM_E_NONE != om_tr69c_get_devinfo(stTr69cParam))    
    {
        free(stTr69cParam); 
        *value = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    *value = strdup(stTr69cParam->Password);  
    free(stTr69cParam);
 */
    return TRX_OK;
}




TRX_STATUS getDeviceInfoX_CMCC_MonitorDescEnable(char **value)
{
 	/* OM_TR69C_APP_PARAM_T *stTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(stTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));  // memset 
	char buf[CHAR_LEN_32];
	memset(buf, 0, sizeof(buf));
    stTr69cAcsInfo->eSetFlag = TR069_ECHO_ENABLE;
    if(OM_E_NONE != om_tr69c_get_acsinfo(stTr69cAcsInfo))
    {
        free(stTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
	
    acsState.PeriodicInformEnable = stTr69cAcsInfo->PeriodicInformEnable;
    sprintf(buf, "%d", stTr69cAcsInfo->PeriodicInformEnable);
    *value = strdup(buf);
    free(stTr69cAcsInfo);   //free  */
    return TRX_OK;
}


TRX_STATUS setDeviceInfoX_CMCC_MonitorDescEnable(const char *value)
{ //TR069_ECHO_ENABLE
	/* OM_TR69C_APP_PARAM_T *stTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(stTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));
    stTr69cAcsInfo->eSetFlag = TR069_ECHO_ENABLE;
    stTr69cAcsInfo->PeriodicInformEnable= atoi(value);
    if(OM_E_NONE != om_tr69c_set_acsinfo(stTr69cAcsInfo))
    {
        free(stTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    acsState.PeriodicInformEnable= atoi(value);
	//resetPeriodicInform(acsState.informInterval);
    free(stTr69cAcsInfo); */
    return TRX_OK;
}

TRX_STATUS getDeviceInfoX_CMCC_MonitorDescMonitorNumberOfEntries(char **value)
{//NUMREQINFORMPARMS
/*
	char buf[CHAR_LEN_32];
	int i = 0;
	int number = 0 ;
	for(i = 0 ; i<informParameters_num;i++)
	{
		if(informParameters[i]==NULL)
					continue;
		number++;
	}

	sprintf(buf, "%d",number);
	*/
	*value = strdup("N");
	return TRX_OK;
}




TRX_STATUS deleteMonitorConfig(char **value)
{
/*nstanceDesc *idp;
	if ( idp= getNewInstanceDesc( getCurrentNode(),getCurrentInstanceDesc(),  0) ) {
        *value = strdup(itoa(idp->instanceID));
        return TRX_REBOOT;
*/
        /* or TRX_BOOT if necessary */
  // }
    return TRX_OK;
}

TRX_STATUS addMonitorConfig(const char *value)
{
 return TRX_OK;
}
TRX_STATUS  setDeviceInfoX_CMCC_MonitorMonitorConfigParaList(const char *value)
{
/*
	char buf[CHAR_LEN_256];
	char actmp[CHAR_LEN_256];
	char *ptr;
	int length;
	int i =0;
	int j=0;
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%s", value);

	for(i = 0 ; i<informParameters_num;i++)
	{
		if(strlen(buf)>1)
		{
			ptr = strchr(buf,',');

			length = ptr?(strlen(buf)-strlen(ptr)):strlen(buf);
			strncpy(actmp, buf,length); 
			actmp[length]='\0';

			informParameters[i] = strdup(actmp);
			printf("informParameters[%d] =%s\n",i,informParameters[i]);
			

			if(ptr == NULL)
				break;

			sprintf(buf, "%s",ptr);
			j =0 ;
			do
			{
				buf[j]=buf[j+1];
				j++;
			}while(buf[j]!='\0');

		}else{
				informParameters[i] = strdup(NULL);
		}
		 
	}
    free(buf)
*/;
	return TRX_OK;
}
TRX_STATUS 	getDeviceInfoX_CMCC_MonitorMonitorConfigParaList(char **value)
{
/*
	char buf[512];
	int i = 0 ;

	for(i = 0 ; i<informParameters_num;i++)
	{
		if(informParameters[i]==NULL)
					continue;
		sprintf(buf, "%s%s,",buf, informParameters[i]);
	}
	buf[strlen(buf)-1]='\0';
	*value = strdup(buf);
*/
	*value = strdup("");
	return TRX_OK;
}
TRX_STATUS 	setDeviceInfoX_CMCC_MonitorMonitorConfigTimeList(const char *value)
{
/*	OM_TR69C_APP_PARAM_T *stTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
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
    free(stTr69cAcsInfo);
    */
    return TRX_OK;
}
TRX_STATUS 	getDeviceInfoX_CMCC_MonitorMonitorConfigTimeList(char **value)
{
	/* OM_TR69C_APP_PARAM_T *stTr69cAcsInfo = (OM_TR69C_APP_PARAM_T *)malloc(sizeof(OM_TR69C_APP_PARAM_T));
    memset(stTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));  // memset 

    stTr69cAcsInfo->eSetFlag = TR069_ECHO_TIME;
    if(OM_E_NONE != om_tr69c_get_acsinfo(stTr69cAcsInfo))
    {
        free(stTr69cAcsInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    acsState.informInterval = stTr69cAcsInfo->informInterval;
    
    *value = strdup(itoa(acsState.informInterval));
    free(stTr69cAcsInfo);   // free */
    return TRX_OK;
}



TRX_STATUS setDeviceInfoX_CMCC_AlarmAlarmNumber(const char *value)
{
	return TRX_OK;
}
TRX_STATUS getDeviceInfoX_CMCC_AlarmAlarmNumber(char **value)
{
	*value = strdup("");
	return TRX_OK;
}

TRX_STATUS getDeviceInfoX_ATP_Informationsimsi(char **value)
{
	
	return TRX_OK;
}
TRX_STATUS getDeviceInfoX_ATP_Informationssn(char **value)
{
	
	return TRX_OK;
}

/* add by duanguiyuan  */




