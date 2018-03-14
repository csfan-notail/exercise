/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : wanWrapper.c
  版 本 号   : 初稿
  作    者   : zhangjiasi
  生成日期   : 2015年6月9日
  最近修改   :
  功能描述   : GBCOM 封装的wan相关的接口实现
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
//#include "util/util.h"
#include "ap_types.h"
#include "om_tr69c.h"
#include "utils.h"
#include "wanDeviceParams.h"
#include "SOAPParser/CPEframework.h"

/*Add by zhanghao 获取配置文件内容 2016/6/15 begin*/
#include "uci_config.h"
#include "tr69_lua.h"
#include "om_include/om_net.h"
#define TAUARRY_LEN 128
//#include "../soapparser/CPEframework.h"
/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
OM_TR69C_ALARM_INFO_T g_alarm_info;
extern int g_log_level;
extern time_t startTime;
extern char *alarmParametersname[];

extern TRxObjNode WANDeviceInstanceDesc[];
extern TRxObjNode WANDeviceWANCommonInterfaceConfigDesc[];
extern TRxObjNode WANX_ATP_APNDescObj[];
extern TRxObjNode WANDeviceWANConnectionDeviceInstanceDesc[];
extern TRxObjNode WANDeviceWANConnectionDeviceWANIPConnectionInstanceDesc[];
extern TRxObjNode WANDeviceWANConnectionDeviceWANIPConnectionPortMappingInstanceDesc[];
extern TRxObjNode WANDeviceWANConnectionDeviceWANIPConnectionStatsDesc[];
extern TRxObjNode WANDeviceWANConnectionDeviceWANPPPConnectionInstanceDesc[];
extern TRxObjNode WANDeviceX_ATP_WANNetConfigInfoInstanceDesc[];
extern TRxObjNode WANDeviceWANConnectionDeviceWANPPPConnectionX_ATP_DMZDesc[];
extern TRxObjNode WANDeviceWANConnectionDeviceWANPPPConnectionPortMappingDesc[];
extern TRxObjNode WANDeviceWANConnectionDeviceWANPPPConnectionStatsDesc[];
	
/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
extern TRxObjNode  WANDeviceWANConnectionDeviceWANPPPConnectionInstanceDesc[];
extern TRxObjNode  WANDeviceWANConnectionDeviceWANIPConnectionInstanceDesc[];
/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

/* */
TRX_STATUS  popen_cmd_get( char *pcCmd,char *pcOut,int uiOutLenMax )
{
    FILE *pstFp = NULL;
    
    if (NULL == pcCmd || NULL == pcOut)
    {
        //OM_LOG(OM_LOG_ERR, "pointer is NULL");
        return -1;
    }

    pstFp = popen(pcCmd, "r");
    if (NULL == pstFp)
    {
        return -1;
    }

    if (fgets(pcOut, uiOutLenMax, pstFp))
    {
        pcOut[strlen(pcOut) - 1] = '\0';
        pclose(pstFp);
    }
    else
    {
        pclose(pstFp);
    }
    printf("cmd (%s):%s\n",pcCmd,pcOut);
    return 0;
}
int get_value_TAU(const char *name,char **value)
{
	char acTmp[128];
    memset(acTmp,0,sizeof(acTmp));
    popen_cmd_get(name,acTmp,sizeof(acTmp));
    *value = strdup(acTmp);
	
	return 0;
}
/*add new interface */
TRX_STATUS setWANAPN(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_WANDEVICE_X_ATP_APN_APN,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
 
    return TRX_OK;
}

TRX_STATUS getWANAPN(char **ppcValue)
{
    char acWANAPN[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_X_ATP_APN_APN,acWANAPN))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWANAPN);
		
	}	
    return TRX_OK;

}


TRX_STATUS setWANAuthenticationProtocol(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_WANDEVICE_X_ATP_APN_AUTHPROTOCOL,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
 
    
    return TRX_OK;
}

TRX_STATUS getWANAuthenticationProtocol(char **ppcValue)
{
    char acProtocol[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_X_ATP_APN_AUTHPROTOCOL,acProtocol))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acProtocol);
		
	}	
    return TRX_OK;

}

TRX_STATUS setWANUsername(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_WANDEVICE_X_ATP_APN_USERNAME,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
    return TRX_OK;
}

TRX_STATUS getWANUsername(char **ppcValue)
{
    char acWANUsername[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_X_ATP_APN_USERNAME,acWANUsername))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWANUsername);
		
	}	
    return TRX_OK;

}

TRX_STATUS setWANPassword(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_WANDEVICE_X_ATP_APN_PASSWORD,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
    return TRX_OK;
}

TRX_STATUS getWANPassword(char **ppcValue)
{
    char acWANPassword[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_X_ATP_APN_PASSWORD,acWANPassword))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWANPassword);
		
	}	
    return TRX_OK;

}

TRX_STATUS setWANDialNumber(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_WANDEVICE_X_ATP_APN_DIALNUM,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
    return TRX_OK;
}

TRX_STATUS getWANDialNumber(char **ppcValue)
{
    char acWANDialNumber[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_X_ATP_APN_DIALNUM,acWANDialNumber))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWANDialNumber);
		
	}	
    return TRX_OK;

}



TRX_STATUS getWANAutoAPN(char **ppcValue)
{
    char acWANAutoAPN[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_X_ATP_APN_AUTOAPN,acWANAutoAPN))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWANAutoAPN);
		
	}	
    return TRX_OK;

}




TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_MaxMTUSize(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_WANDEVICE_PORTMAPPING_MAXMTUSIZE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
    return TRX_OK;
}

TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_MaxMTUSize(char **ppcValue)
{
    char acMaxMTUSize[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_PORTMAPPING_MAXMTUSIZE,acMaxMTUSize))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acMaxMTUSize);
		
	}	
    return TRX_OK;

}





TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_APN(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_WANDEVICE_PORTMAPPING_X_ATP_APN,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
    return TRX_OK;
}

TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_APN(char **ppcValue)
{
    char acX_ATP_APN[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_PORTMAPPING_X_ATP_APN,acX_ATP_APN))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_APN);
		
	}	
    return TRX_OK;

}




TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_AuthenticationProtocol(const char *pcValue)
{
	if(UCI_OK != uci_config_set(UCI_WANDEVICE_PORTMAPPING_X_ATP_AUTHPROTOCOL,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
   
    return TRX_OK;
}

TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_AuthenticationProtocol(char **ppcValue)
{
    char acProtocol[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_PORTMAPPING_X_ATP_AUTHPROTOCOL,acProtocol))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acProtocol);
		
	}	
    return TRX_OK;

}



TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_Username(const char *pcValue)
{
	if(UCI_OK != uci_config_set(UCI_WANDEVICE_PORTMAPPING_X_ATP_USERNAME,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
    return TRX_OK;
}

TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_Username(char **ppcValue)
{
    char acX_ATP_Username[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_PORTMAPPING_X_ATP_USERNAME,acX_ATP_Username))
	{
		*ppcValue = strdup("00");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_Username);
		
	}	
    return TRX_OK;

}



TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_DialNumber(const char *pcValue)
{
	if(UCI_OK != uci_config_set(UCI_WANDEVICE_PORTMAPPING_X_ATP_DIALNUM,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
    return TRX_OK;
}

TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_DialNumber(char **ppcValue)
{
    char acX_ATP_DialNumber[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_PORTMAPPING_X_ATP_DIALNUM,acX_ATP_DialNumber))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_DialNumber);
		
	}	
    return TRX_OK;

}





TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_AutoAPN(const char *pcValue)
{
	if(UCI_OK != uci_config_set(UCI_WANDEVICE_PORTMAPPING_X_ATP_AUTOAPN,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
    return TRX_OK;
}

TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_AutoAPN(char **ppcValue)
{
    char acX_ATP_AutoAPN[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_PORTMAPPING_X_ATP_AUTOAPN,acX_ATP_AutoAPN))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_AutoAPN);
		
	}	
    return TRX_OK;

}



TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnectionPortMapping_RemoteHost(const char *pcValue)
{
	if(UCI_OK != uci_config_set(UCI_WANDEVICE_PORTMAPPING_REMOTEHOST,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
    return TRX_OK;
}

TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnectionPortMapping_RemoteHost(char **ppcValue)
{
    char acRemoteHost[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_PORTMAPPING_REMOTEHOST,acRemoteHost))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acRemoteHost);
		
	}	
    return TRX_OK;

}


TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_ExternalPort(const char *pcValue)
{
	if(UCI_OK != uci_config_set(UCI_WANDEVICE_PORTMAPPING_EXTERNALPORT,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
    return TRX_OK;
}

TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_ExternalPort(char **ppcValue)
{
    char acExternalPort[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_PORTMAPPING_EXTERNALPORT,acExternalPort))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acExternalPort);
		
	}	
    return TRX_OK;

}





TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_ExternalPortEndRange(const char *pcValue)
{
	if(UCI_OK != uci_config_set(UCI_WANDEVICE_PORTMAPPING_EPER,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
    return TRX_OK;
}

TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_ExternalPortEndRange(char **ppcValue)
{
    char acRange[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_PORTMAPPING_EPER,acRange))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acRange);
		
	}	
    return TRX_OK;

}




TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_InternalPort(const char *pcValue)
{
	if(UCI_OK != uci_config_set(UCI_WANDEVICE_PORTMAPPING_INTERNALPORT,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
    return TRX_OK;
}

TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_InternalPort(char **ppcValue)
{
    char acInternalPort[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_PORTMAPPING_INTERNALPORT,acInternalPort))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acInternalPort);
		
	}	
    return TRX_OK;

}




TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_PortMappingProtocol(const char *pcValue)
{
	if(UCI_OK != uci_config_set(UCI_WANDEVICE_PORTMAPPING_PORTMAPPINGPROTOCOL,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
    return TRX_OK;
}

TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_PortMappingProtocol(char **ppcValue)
{
    char acProtocol[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_PORTMAPPING_PORTMAPPINGPROTOCOL,acProtocol))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acProtocol);
		
	}	
    return TRX_OK;

}




TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_CAREnable(const char *pcValue)
{
	if(UCI_OK != uci_config_set(UCI_WANDEVICE_X_ATP_CAR_ENABLE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
    return TRX_OK;
}

TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_CAREnable(char **ppcValue)
{
    char acX_ATP_CAREnable[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_X_ATP_CAR_ENABLE,acX_ATP_CAREnable))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_CAREnable);
		
	}	
    return TRX_OK;

}




TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_CARUplinkRate(const char *pcValue)
{
	if(UCI_OK != uci_config_set(UCI_WANDEVICE_X_ATP_CAR_UPLINKRATE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
    return TRX_OK;
}

TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_CARUplinkRate(char **ppcValue)
{
    char acX_ATP_CARUplinkRate[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_X_ATP_CAR_UPLINKRATE,acX_ATP_CARUplinkRate))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_CARUplinkRate);
		
	}	
    return TRX_OK;

}



TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_DMZEnable(const char *pcValue)
{
	if(UCI_OK != uci_config_set(UCI_WANDEVICE_X_ATP_DMZ_DMZENABLE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
    return TRX_OK;
}

TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_DMZEnable(char **ppcValue)
{
    char acDMZEnable[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_X_ATP_DMZ_DMZENABLE,acDMZEnable))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acDMZEnable);
		
	}	
    return TRX_OK;

}


TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_DMZHostIPAddress(const char *pcValue)
{
	if(UCI_OK != uci_config_set(UCI_WANDEVICE_X_ATP_DMZ_DMZHOSTIPADDR,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
    return TRX_OK;
}

TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_DMZHostIPAddress(char **ppcValue)
{
    char acIPAddress[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_X_ATP_DMZ_DMZHOSTIPADDR,acIPAddress))
	{
		*ppcValue = strdup("0.0.0.0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acIPAddress);
		
	}	
    return TRX_OK;

}





TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_PLMN(char **ppcValue)
{
    char acPLMN[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_X_ATP_WIRELESSSTATUS_PLMN,acPLMN))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acPLMN);
		
	}	
    return TRX_OK;

}


TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_ServiceStatus(char **ppcValue)
{
    char acServiceStatus[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_X_ATP_WIRELESSSTATUS_SERSTATUS,acServiceStatus))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acServiceStatus);
		
	}	
    return TRX_OK;

}


TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_Bandwidth(char **ppcValue)
{
    char acBandwidth[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_X_ATP_WIRELESSSTATUS_BANDWIDTH,acBandwidth))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acBandwidth);
		
	}	
    return TRX_OK;

}


TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_CellID(char **ppcValue)
{
    char acCellID[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LTEINFO_CELLID,acCellID))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
	}
	else{
		*ppcValue = strdup(acCellID);
		
	}	
    return TRX_OK;

}


TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_SignalStrength(char **ppcValue)
{
    char acSignalStrength[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_X_ATP_WIRELESSSTATUS_SIGNALSTRENGTH,acSignalStrength))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acSignalStrength);
		
	}	
    return TRX_OK;

}


TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_RSSI(char **ppcValue)
{
    char acRSSI[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LTEINFO_RSSI,acRSSI))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acRSSI);
		
	}	
    return TRX_OK;

}


TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_RSRP(char **ppcValue)
{
    char acRSRP[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LTEINFO_RSRP,acRSRP))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acRSRP);
		
	}	
    return TRX_OK;

}


TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_RSRQ(char **ppcValue)
{
    char acRSRQ[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LTEINFO_RSRQ,acRSRQ))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acRSRQ);
		
	}	
    return TRX_OK;

}
TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_SINR(char **ppcValue)
{
    char tmp[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LTEINFO_SINR,tmp))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
	}
	else{
		*ppcValue = strdup(tmp);
		
	}	
    return TRX_OK;
}

	

TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_Roam(char **ppcValue)
{
    char acRoam[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_X_ATP_WIRELESSSTATUS_ROAM,acRoam))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acRoam);
		
	}	
    return TRX_OK;

}


TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_Frequency(char **ppcValue)
{
    char acFrequency[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_X_ATP_WIRELESSSTATUS_FREQUENCY,acFrequency))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acFrequency);
		
	}	
    return TRX_OK;

}





TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnectionStats_EthernetErrorsSent(char **ppcValue)
{
    char acEthernetErrorsSent[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_IPCONNECTION_EESENT,acEthernetErrorsSent))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acEthernetErrorsSent);
		
	}	
    return TRX_OK;

}


TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnectionStats_EthernetErrorsReceived(char **ppcValue)
{
    char acEthernetErrorsReceived[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_IPCONNECTION_EERECEIVED,acEthernetErrorsReceived))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acEthernetErrorsReceived);
		
	}	
    return TRX_OK;

}


TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnectionStats_EthernetDiscardPacketsSent(char **ppcValue)
{
    char acPacketsSent[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_IPCONNECTION_EDPSENT,acPacketsSent))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acPacketsSent);
		
	}	
    return TRX_OK;

}


TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnectionStats_EthernetDiscardPacketsReceived(char **ppcValue)
{
    char acPacketsReceived[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_IPCONNECTION_EDPRECEVIVED,acPacketsReceived))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acPacketsReceived);
		
	}	
    return TRX_OK;

}


TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnectionStats_X_ATP_EthernetUplinkRate(char **ppcValue)
{
    char acUplinkRate[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_IPCONNECTION_X_ATP_EURATE,acUplinkRate))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acUplinkRate);
		
	}	
    return TRX_OK;

}


TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnectionStats_X_ATP_EthernetDownlinkRate(char **ppcValue)
{
    char acDownlinkRate[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_IPCONNECTION_X_ATP_EDRATE,acDownlinkRate))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acDownlinkRate);
		
	}	
    return TRX_OK;

}





/* InternetGatewayDevice.WANDevice.1.WANCommonInterfaceConfig. */
/* 上下行理论最大速率 */
TRX_STATUS getWANDeviceWANCommonInterfaceConfig_Layer1UpstreamMaxBitRate(char **value)
{
    *value = strdup("100");

    return TRX_OK;
}
TRX_STATUS getWANDeviceWANCommonInterfaceConfig_Layer1DownstreamMaxBitRate(char **value)
{
    *value = strdup("100");
    return TRX_OK;

}


/* wan口收发包统计 */
TRX_STATUS getWANDeviceWANCommonInterfaceConfig_TotalBytesSent(char **value)
{
   /*  OM_TR69C_WAN_STATS_T stTsStats;
    CHAR szTemp[CHAR_LEN_32] = {0};
	memset(szTemp, 0, sizeof(szTemp));

    if(OM_E_NONE != om_net_tr069_wan_stats(&stTsStats))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    sprintf(szTemp,"%d",stTsStats.stTsList.stTsIpv4.uiUpBytes);

    *value = strdup(szTemp); */
    return TRX_OK;
}
TRX_STATUS getWANDeviceWANCommonInterfaceConfig_TotalBytesReceived(char **value)
{
    /* OM_TR69C_WAN_STATS_T stTsStats;
    CHAR szTemp[CHAR_LEN_32] = {0};
	memset(szTemp, 0, sizeof(szTemp));

    if(OM_E_NONE != om_net_tr069_wan_stats(&stTsStats))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    sprintf(szTemp,"%d",stTsStats.stTsList.stTsIpv4.uiDownBytes);

    *value = strdup(szTemp); */
    return TRX_OK;
}
TRX_STATUS getWANDeviceWANCommonInterfaceConfig_TotalPacketsSent(char **value)
{
    /* OM_TR69C_WAN_STATS_T stTsStats;
    CHAR szTemp[CHAR_LEN_32] = {0};
	memset(szTemp, 0, sizeof(szTemp));

    if(OM_E_NONE != om_net_tr069_wan_stats(&stTsStats))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    sprintf(szTemp,"%d",stTsStats.stTsList.stTsIpv4.uiUpGiga);

    *value = strdup(szTemp); */
    return TRX_OK;
}
TRX_STATUS getWANDeviceWANCommonInterfaceConfig_TotalPacketsReceived(char **value)
{
    /* OM_TR69C_WAN_STATS_T stTsStats;
    CHAR szTemp[CHAR_LEN_32] = {0};
	memset(szTemp, 0, sizeof(szTemp));

    if(OM_E_NONE != om_net_tr069_wan_stats(&stTsStats))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    sprintf(szTemp,"%d",stTsStats.stTsList.stTsIpv4.uiDownGiga);

    *value = strdup(szTemp); */
    return TRX_OK;

}

/* InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}. */
/* 增加或删除wan设备实例，此处默认只有一个wan实例，故实际上并不进行添加和删
   除 */
TRX_STATUS addWANConnectionDevice(char **value)
{
	InstanceDesc *idp;
	if ( (idp = getNewInstanceDesc( getCurrentNode(),getCurrentInstanceDesc(),0)) !=NULL ) {
        *value = strdup(itoa(idp->instanceID));
        return TRX_REBOOT; /* or TRX_BOOT if necessary */
    }
    return TRX_ERR;
}

TRX_STATUS deleteWANConnectionDevice(const char *value)
{
	TRxObjNode *n = getCurrentNode();
//	InstanceDesc *idp;
	int     id = atoi(value);
//		 && getInstanceCount(WANDeviceWANConnectionDeviceWANIPConnectionInstanceDesc)==0	
	if ( getInstanceCount(WANDeviceWANConnectionDeviceWANPPPConnectionInstanceDesc)== 0) {
		fprintf(stderr, "Child object NOT DELETED\n");
	}
	if (!deleteInstanceDesc(n,id)){
		fprintf(stderr, "Delete WANConnectionDevice Instance = %d\n", id);
		return TRX_REBOOT; /* or TRX_BOOT if necessary */
	}

	return TRX_OK;
}


TRX_STATUS getWANDeviceWANConnectionDevice_WANIPConnectionNumberOfEntries(char **ppcValue)
{
	//*value = strdup( itoa(getInstanceCount(WANDeviceWANConnectionDeviceWANIPConnectionInstanceDesc)));

	*ppcValue = strdup( itoa(getInstanceCount(WANDeviceWANConnectionDeviceWANPPPConnectionInstanceDesc)));


	return TRX_OK;
}

TRX_STATUS getPPPConNumberEntries(char **value)
{
	*value = strdup( itoa(getInstanceCount(WANDeviceWANConnectionDeviceWANPPPConnectionInstanceDesc)));
	return TRX_OK;
}


/* InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.*/
/* 当置为真时，cpe必须使用IP方式，重新进行连接。并将reset置为false */
TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_Reset(const char *value)
{
   /*  OM_TR69C_WAN_IP_CONNECT_T stWanIpConnect = {0};

    stWanIpConnect.eFlag = OM_TR69C_WAN_IP_CONNECT_RESET;

    if(OM_E_NONE != om_net_wan_IP_connect_set(&stWanIpConnect))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    } */
    return TRX_OK;
}

TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_Reset(char **value)
{
    *value = strdup("0");

	return TRX_OK;
}

/* The time in seconds that this connection has been,与acs的连接时长，从建立连接成功后开始计算，单位:s */
TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_Uptime(char **ppcValue)
{
	time_t  upt;
	char buf[CHAR_LEN_64];
	memset(buf, 0, sizeof(buf));
//	INT32 uiUpTime = 0;
            
	upt = time(NULL) - startTime;
	sprintf(buf, "%lu", upt); 
   	*ppcValue = strdup(buf); 
	return TRX_OK;
}

/* 是否启用nat */
TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_NATEnabled(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_WANDEVICE_WANIPCONNECTION_NATENABLED,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_NATEnabled(char **ppcValue)
{
	char acNatEnabled[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_WANIPCONNECTION_NATENABLED,acNatEnabled))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acNatEnabled);
		
	}	
   // *value = strdup("1");
    return TRX_OK;
}

/* 地址类型：
“DHCP”
“Static” */
TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_AddressingType(const char *value)
{
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_AddressingType(char **value)
{
	*value = strdup("DHCP");
	return TRX_OK;
}

/* 
管理IP。当AddressingType为'static'时，该节点可以进行配置  */
TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_ExternalIPAddress(const char *value)
{
   /*  OM_TR69C_WAN_IP_CONNECT_T stWanIpConnect = {0};

    stWanIpConnect.eFlag = OM_TR69C_WAN_IP_CONNECT_IP_ADDR;
    sscanf(value, "%hhu.%hhu.%hhu.%hhu",
                &stWanIpConnect.stWanIpConnectCfg.aucStaticIp[0],
                &stWanIpConnect.stWanIpConnectCfg.aucStaticIp[1],
                &stWanIpConnect.stWanIpConnectCfg.aucStaticIp[2],
                &stWanIpConnect.stWanIpConnectCfg.aucStaticIp[3]);
    
    if(OM_E_NONE != om_net_wan_IP_connect_set(&stWanIpConnect))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }
      
 */
    return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_ExternalIPAddress(char **ppcValue)
{
    /* OM_TR69C_WAN_IP_CONNECT_T stWanIpConnect = {0};
    CHAR szTemp[CHAR_LEN_32] = {0};
	memset(szTemp, 0, sizeof(szTemp));
    stWanIpConnect.eFlag = OM_TR69C_WAN_IP_CONNECT_IP_ADDR;

    if(OM_E_NONE != om_net_wan_IP_connect_get(&stWanIpConnect))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);
		*value = strdup("0.0.0.0");
        return TRX_OK;
    }

    sprintf(szTemp,IP_FMT,IP_ARG(stWanIpConnect.stWanIpConnectCfg.aucStaticIp));

    *value = strdup(szTemp); */
	char acExternalIPAddress[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_WANIPCONNECTION_EXTERNALIPADDRESS,acExternalIPAddress))
	{
		*ppcValue = strdup("0.0.0.0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acExternalIPAddress);
		
	}	
    return TRX_OK;
}

/* WAN接口的子网掩码 */
TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_SubnetMask(const char *pcValue)
{
    /* OM_TR69C_WAN_IP_CONNECT_T stWanIpConnect = {0};

    stWanIpConnect.eFlag = OM_TR69C_WAN_IP_CONNECT_MASK;
    sscanf(value, "%hhu.%hhu.%hhu.%hhu",
                &stWanIpConnect.stWanIpConnectCfg.aucMask[0],
                &stWanIpConnect.stWanIpConnectCfg.aucMask[1],
                &stWanIpConnect.stWanIpConnectCfg.aucMask[2],
                &stWanIpConnect.stWanIpConnectCfg.aucMask[3]);
    
    if(OM_E_NONE != om_net_wan_IP_connect_set(&stWanIpConnect))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_OK;
    } */
    if(UCI_OK !=uci_config_set(UCI_WANDEVICE_WANIPCONNECTION_SUBNETMASK,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_SubnetMask(char **ppcValue)
{
    /* OM_TR69C_WAN_IP_CONNECT_T stWanIpConnect = {0};
    CHAR szTemp[CHAR_LEN_32] = {0};
	memset(szTemp, 0, sizeof(szTemp));

    stWanIpConnect.eFlag = OM_TR69C_WAN_IP_CONNECT_MASK;

    if(OM_E_NONE != om_net_wan_IP_connect_get(&stWanIpConnect))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);  
		 *value = strdup("255.255.255.0");
        return TRX_OK;
    }

    sprintf(szTemp,IP_FMT,IP_ARG(stWanIpConnect.stWanIpConnectCfg.aucMask));

    *value = strdup(szTemp); */
	char acSubnetMask[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_WANIPCONNECTION_SUBNETMASK,acSubnetMask))
	{
		*ppcValue = strdup("0.0.0.0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acSubnetMask);
		
	}	
    return TRX_OK;
}

/* WAN接口的子网掩码 */
TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_DefaultGateway(const char *pcValue)
{
    /* OM_TR69C_WAN_IP_CONNECT_T stWanIpConnect = {0};

    stWanIpConnect.eFlag = OM_TR69C_WAN_IP_CONNECT_GW;
    sscanf(value, "%hhu.%hhu.%hhu.%hhu",
                &stWanIpConnect.stWanIpConnectCfg.aucGw[0],
                &stWanIpConnect.stWanIpConnectCfg.aucGw[1],
                &stWanIpConnect.stWanIpConnectCfg.aucGw[2],
                &stWanIpConnect.stWanIpConnectCfg.aucGw[3]);
    
    if(OM_E_NONE != om_net_wan_IP_connect_set(&stWanIpConnect))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_OK;
    } */
    if(UCI_OK !=uci_config_set(UCI_WANDEVICE_WANIPCONNECTION_DEFAULTGATEWAY,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_DefaultGateway(char **ppcValue)
{
    /* OM_TR69C_WAN_IP_CONNECT_T stWanIpConnect = {0};
    CHAR szTemp[CHAR_LEN_32] = {0};
	memset(szTemp, 0, sizeof(szTemp));

    stWanIpConnect.eFlag = OM_TR69C_WAN_IP_CONNECT_GW;

    if(OM_E_NONE != om_net_wan_IP_connect_get(&stWanIpConnect))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);  
		*value = strdup("0.0.0.0");
        return TRX_OK;
    }

    sprintf(szTemp,IP_FMT,IP_ARG(stWanIpConnect.stWanIpConnectCfg.aucGw));

    *value = strdup(szTemp); */
    char acDefaultGateway[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_WANIPCONNECTION_DEFAULTGATEWAY,acDefaultGateway))
	{
		*ppcValue = strdup("0.0.0.0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acDefaultGateway);
		
	}	
    return TRX_OK;
}

/* 设备是否进行DNS query,默认true */
TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_DNSEnabled(const char *value)
{
    return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_DNSEnabled(char **value)
{
    *value = strdup("1");
    return TRX_OK;
}

/*Whether or not a manually set, non-empty DNS
address can be overridden by a DNS entry
received from the WAN.*/
TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_DNSOverrideAllowed(const char *value)
{
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_DNSOverrideAllowed(char **value)
{
	*value = strdup("");
	return TRX_OK;
}

/* DNS列表。以逗号隔开 */
TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_DNSServers(const char *value)
{
   /*  OM_TR69C_WAN_IP_CONNECT_T stWanIpConnect = {0};
    CHAR szMaster[CHAR_LEN_64] = {0}; 
    CHAR szSlave[CHAR_LEN_64] = {0}; 
    CHAR *pcTmp = NULL;
    CHAR szTemp[CHAR_LEN_128] = {0};

    // 区分当时一共有几个dns服务器 
    memcpy(szTemp,value,strlen(value));
    if ( NULL == (pcTmp = strstr(value,",")) )
    {
        sscanf(value, "%hhu.%hhu.%hhu.%hhu",
                &stWanIpConnect.stWanIpConnectCfg.aucDnsSvr1[0],
                &stWanIpConnect.stWanIpConnectCfg.aucDnsSvr1[1],
                &stWanIpConnect.stWanIpConnectCfg.aucDnsSvr1[2],
                &stWanIpConnect.stWanIpConnectCfg.aucDnsSvr1[3]);
        stWanIpConnect.eFlag = OM_TR69C_WAN_IP_CONNECT_DNS1;
    
        if(OM_E_NONE != om_net_wan_IP_connect_get(&stWanIpConnect))
        {
            slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);
            return TRX_ERR;
        }
    }
    else
    {
        strncpy(szMaster,value,strlen(value)-strlen(pcTmp));
        strcpy(szSlave,++pcTmp);

        sscanf(value, "%hhu.%hhu.%hhu.%hhu",
                &stWanIpConnect.stWanIpConnectCfg.aucDnsSvr1[0],
                &stWanIpConnect.stWanIpConnectCfg.aucDnsSvr1[1],
                &stWanIpConnect.stWanIpConnectCfg.aucDnsSvr1[2],
                &stWanIpConnect.stWanIpConnectCfg.aucDnsSvr1[3]);
        stWanIpConnect.eFlag = OM_TR69C_WAN_IP_CONNECT_DNS1;
    
        if(OM_E_NONE != om_net_wan_IP_connect_get(&stWanIpConnect))
        {
            slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);
            return TRX_ERR;
        }

        sscanf(value, "%hhu.%hhu.%hhu.%hhu",
                &stWanIpConnect.stWanIpConnectCfg.aucDnsSvr2[0],
                &stWanIpConnect.stWanIpConnectCfg.aucDnsSvr2[1],
                &stWanIpConnect.stWanIpConnectCfg.aucDnsSvr2[2],
                &stWanIpConnect.stWanIpConnectCfg.aucDnsSvr2[3]);
        stWanIpConnect.eFlag = OM_TR69C_WAN_IP_CONNECT_DNS2;
    
        if(OM_E_NONE != om_net_wan_IP_connect_get(&stWanIpConnect))
        {
            slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);
            return TRX_ERR;
        }
    } */

    return TRX_OK;

}
TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_DNSServers(char **value)
{
  /*   OM_TR69C_WAN_IP_CONNECT_T stWanIpConnect = {0};
    CHAR szTemp[CHAR_LEN_64] = {0};
	memset(szTemp, 0, sizeof(szTemp));

    stWanIpConnect.eFlag = OM_TR69C_WAN_IP_CONNECT_DNS1;  

    if(OM_E_NONE != om_net_wan_IP_connect_get(&stWanIpConnect))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__); 
		 *value = strdup("0.0.0.0");
        return TRX_OK;
    }

    stWanIpConnect.eFlag = OM_TR69C_WAN_IP_CONNECT_DNS2;  

    if(OM_E_NONE != om_net_wan_IP_connect_get(&stWanIpConnect))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);   
		*value = strdup("0.0.0.0");
        return TRX_OK;
    }

    sprintf(szTemp,IP_FMT","IP_FMT,
                IP_ARG(stWanIpConnect.stWanIpConnectCfg.aucDnsSvr1),
                IP_ARG(stWanIpConnect.stWanIpConnectCfg.aucDnsSvr1));
    *value = strdup(szTemp); */
    return TRX_OK;
}


/* MAC地址 */
TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_MACAddress(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_WANDEVICE_WANIPCONNECTION_MACADDR,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_MACAddress(char **ppcValue)
{
    /* OM_TR69C_WAN_IP_CONNECT_T stWanIpConnect = {0};
    CHAR szTemp[CHAR_LEN_32] = {0};
	memset(szTemp, 0, sizeof(szTemp));

    stWanIpConnect.eFlag = OM_TR69C_WAN_IP_CONNECT_MAC;

    if(OM_E_NONE != om_net_wan_IP_connect_get(&stWanIpConnect))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);  
		*value = strdup("00:00:00:00:00:00");
        return TRX_OK;
    }
    sprintf(szTemp,MAC_FMT,MAC_ARG(stWanIpConnect.stWanIpConnectCfg.aucGw));

    *value = strdup(szTemp); */
    char acMACAddress[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_WANIPCONNECTION_MACADDR,acMACAddress))
	{
		*ppcValue = strdup("00:00:00:00:00:00");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acMACAddress);
		
	}	
    return TRX_OK;
}

/* 端口映射实体数 */
TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_PortMappingNumberOfEntries(char **ppcValue)
{
	char acNumberOfEntries[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_WANIPCONNECTION_PORTMAPPINGNUMBEROFENTRIES,acNumberOfEntries))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acNumberOfEntries);
		
	}	
	//*value = strdup("");
	return TRX_OK;
}
/* InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.{i} */
TRX_STATUS delWANDeviceWANConnectionDeviceWANIPConnectionPortMappingI(const char *value)
{
	return TRX_OK;
}
TRX_STATUS addWANDeviceWANConnectionDeviceWANIPConnectionPortMappingI(char **value)
{
    *value = strdup("");
    return TRX_OK;
}

/* PortMappingEnabled */
TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnectionPortMapping_PortMappingEnabled(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_WANDEVICE_PORTMAPPING_PORTMAPPINGENABLED,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnectionPortMapping_PortMappingEnabled(char **ppcValue)
{
	char acPortMappingEnabled[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_PORTMAPPING_PORTMAPPINGENABLED,acPortMappingEnabled))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acPortMappingEnabled);
		
	}	
	//*value = strdup("");
	return TRX_OK;
}

TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnectionPortMapping_InternalClient(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_WANDEVICE_PORTMAPPING_INTERNALCLIENT,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnectionPortMapping_InternalClient(char **ppcValue)
{
	char acInternalClient[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_PORTMAPPING_PORTMAPPINGENABLED,acInternalClient))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acInternalClient);
		
	}	
	//*value = strdup("");
	return TRX_OK;
}

/* InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.Stats.*/
TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnectionStats_EthernetBytesSent(char **ppcValue)
{
	char acEthernetBytesSent[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_IPCONNECTION_ETHERNETBYTESSENT,acEthernetBytesSent))
	{
		*ppcValue = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acEthernetBytesSent);
		
	}	
	//*value = strdup("");
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnectionStats_EthernetBytesReceived(char **ppcValue)
{
	char acEthernetBytesReceived[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_IPCONNECTION_ETHERNETBTTESRECEIVED,acEthernetBytesReceived))
	{
		*ppcValue = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acEthernetBytesReceived);
		
	}	
	//*value = strdup("");
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnectionStats_EthernetPacketsSent(char **ppcValue)
{
	char acEthernetBytesReceived[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_IPCONNECTION_ETHERNETPACKETSSENT,acEthernetBytesReceived))
	{
		*ppcValue = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acEthernetBytesReceived);
		
	}	
	//*value = strdup("");
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnectionStats_EthernetPacketsReceived(char **ppcValue)
{
	char acEthernetPacketsReceived[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_IPCONNECTION_ETHERNETPACKETSRECEIVED,acEthernetPacketsReceived))
	{
		*ppcValue = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acEthernetPacketsReceived);
		
	}	
	//*value = strdup("");
	return TRX_OK;
}

/* add by duanguiyuan 20150917 */



TRX_STATUS setWANDeviceWANConnectionDeviceWANPPPConnection_Enable(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_WANDEVICE_WANPPPCONNECTION_ENABLE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
	return TRX_OK;
}


TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnection_Enable(char **ppcValue)
{
	char acEnable[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_WANPPPCONNECTION_ENABLE,acEnable))
	{
		*ppcValue = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acEnable);
		
	}	
	//*value = strdup("1");
	return TRX_OK;
}


TRX_STATUS setWANDeviceWANConnectionDeviceWANPPPConnection_Name(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_WANDEVICE_WANPPPCONNECTION_NAME,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnection_Name(char **ppcValue)
{
	/* OM_TR69C_WAN_STATS_T pstTsStats;
  //  CHAR szTemp[CHAR_LEN_32] = {0};

    if(OM_E_NONE != om_net_tr069_wan_stats(&pstTsStats))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
    	*value = strdup("NONE"); 
		return TRX_OK;
    }

//slog(TR69C_LOG_ERR,"[tr69c:%s,%d]acUsername = %s .............",__func__,__LINE__,pstTsStats.stTsList.acDevName);   
	//sprintf(szTemp,MAC_FMT,MAC_ARG(pstTsStats.stTsList.acDevName));
	
    *value = strdup(pstTsStats.stTsList.acDevName); */
    char acName[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_WANPPPCONNECTION_NAME,acName))
	{
		*ppcValue = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acName);
		
	}	
    return TRX_OK;	
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnection_Uptime(char **value)
{
	time_t  upt;
	char buf[CHAR_LEN_64];
	memset(buf, 0, sizeof(buf));

	//INT32 uiUpTime = 0;
            
	upt = time(NULL) - startTime;
	sprintf(buf, "%lu", upt); 
   	*value = strdup(buf); 
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnection_LastConnectionError(char **ppcValue)
{
	char acLastConnectionError[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_WANPPPCONNECTION_LASTCONNECTIONERROR,acLastConnectionError))
	{
		*ppcValue = strdup("ERROR_NONE");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acLastConnectionError);
		
	}	
	//*value = strdup("ERROR_NONE");
	return TRX_OK;
}

TRX_STATUS setWANDeviceWANConnectionDeviceWANPPPConnection_NATEnabled(const char *pcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnection_NATEnabled(char **ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnection_MaxMRUSize(char **ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setWANDeviceWANConnectionDeviceWANPPPConnection_MAXMTUSize(const char *pcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnection_MAXMTUSize(char **ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getWANDevWANConnectDevWANPPPConnect_ConnectTrigger(char **ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getWANDevWANConnectDevWANPPPConnect_X_ATP_ServiceList(char **ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getWANDevWANConnectDevWANPPPConnect_X_ATP_NATType(char **ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getWANDevWANConnectDevWANPPPConnect_ConnectionStatus(char **ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getWANDevWANConnectDevWANPPPConnect_DefaultGateway(char **ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setWANDeviceWANConnectionDeviceWANPPPConnection_DMZEnable(const char *pcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnection_DMZEnable(char **ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setWANDeviceWANConnectionDeviceWANPPPConnection_DMZHostIPAddress(const char *pcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnection_DMZHostIPAddress(char **ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnectionStats_EthernetBytesSent(char **ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnectionStats_EthernetBytesReceived(char **ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnectionStats_EthernetDiscardPacketsSent(char **ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnectionStats_EthernetDiscardPacketsRec(char **ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnectionStats_EthernetErrorsSent(char **ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnectionStats_EthernetPacketsSent(char **ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnectionStats_EthernetPacketsReceived(char **ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnectionStats_EthernetErrorsReceived(char **ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnectionStats_X_ATP_EthernetUplinkRate(char **ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnectionStats_X_ATP_EthernetDownlinkRate(char **ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getWANDeviceX_ATP_WANNetConfigInfo_RSSI(char **ppcValue)
{
	get_value_TAU("cat /tmp/uci/iRssi",ppcValue);
	return TRX_OK;
}
TRX_STATUS getWANDeviceX_ATP_WANNetConfigInfo_RSRP(char **ppcValue)
{
	get_value_TAU("cat /tmp/uci/iRsrp",ppcValue);
	return TRX_OK;
}
TRX_STATUS getWANDeviceX_ATP_WANNetConfigInfo_RSRQ(char **ppcValue)
{
	get_value_TAU("cat /tmp/uci/iRsrq",ppcValue);
	return TRX_OK;
}
TRX_STATUS getWANDeviceX_ATP_WANNetConfigInfo_SINR(char **ppcValue)
{
	get_value_TAU("cat /tmp/uci/iSinr",ppcValue);
	return TRX_OK;
}
TRX_STATUS getWANDeviceX_ATP_WANNetConfigInfo_CellID(char **ppcValue)
{
	get_value_TAU("cat /tmp/uci/acPhyCellid",ppcValue);
	return TRX_OK;
}
TRX_STATUS getWANDeviceX_ATP_WANNetConfigInfo_PLMN(char **ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getWANDeviceX_ATP_WANNetConfigInfo_SignalQuality(char **ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getWANDevX_ATP_WANNetConfigInfo_RoamStatus(char **ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getWANDeviceX_ATP_WANNetConfigInfo_PhysicalCellID(char **ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getWANDevX_ATP_WANNetConfigInfo_SerStatus(char **ppcValue)
{
	
	return TRX_OK;
}

TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnection_Username(char **ppcValue)
{
	char acUsername[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_WANPPPCONNECTION_USERNAME,acUsername))
	{
		*ppcValue = strdup("user");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acUsername);
		
	}	
	//*value = strdup("user");
	return TRX_OK;
}

TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnection_Password(char **ppcValue)
{
	char acPassword[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_WANPPPCONNECTION_PASSWORD,acPassword))
	{
		*ppcValue = strdup(" ");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acPassword);
		
	}	
	//*value = strdup(" ");
	return TRX_OK;
}  
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnection_ExternalIPAddress(char **ppcValue)
{

	/* UINT8 ip_tmp[4];
    CHAR szTemp[CHAR_LEN_64] = {0};
	memset(szTemp, 0, sizeof(szTemp));
 	if ( AP_E_NONE != om_net_wan_ip_get(ip_tmp) )
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        *value = strdup("0.0.0.0");
		return TRX_OK;
    }
   	sprintf(szTemp,IP_FMT,IP_ARG(ip_tmp));
	//sprintf(szTemp,"%d",pstTsStats.stTsList.uiIp);
    *value = strdup(szTemp); */
    char acExternalIPAddress[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_WANPPPCONNECTION_EXTERNALIPADDRESS,acExternalIPAddress))
	{
		*ppcValue = strdup("0.0.0.0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acExternalIPAddress);
		
	}	
    printf("acExternalIPAddress is ============%s \n",acExternalIPAddress);
	
	//*value = strdup("172.16.1.2");
    return TRX_OK;	

/*
	OM_TR69C_WAN_STATS_T pstTsStats = {0};
	
		//slog(TR69C_LOG_DEBUG,"[tr69c:%s,%d] ExternalIPAddress  0 !!!",__func__,__LINE__);

    if(OM_E_NONE != om_net_tr069_wan_stats(&pstTsStats))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        *value = strdup("0.0.0.0");
		return TRX_OK;
    }
//      util_str_to_ip(acTemp,pstWanIpConnect->stWanIpConnectCfg.aucStaticIp);
//		slog(TR69C_LOG_DEBUG,"[tr69c:%s,%d] ExternalIPAddress  0 !!!",__func__,__LINE__);
	ip_tmp = (UINT32 *)pstTsStats.stTsList.uiIp;
	*/
 
}  
TRX_STATUS setWANDeviceWANConnectionDeviceWANPPPConnection_DNSEnabled(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_WANDEVICE_WANPPPCONNECTION_DNSENABLED,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnection_DNSEnabled(char **ppcValue)
{
	char acDNSEnabled[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_WANPPPCONNECTION_DNSENABLED,acDNSEnabled))
	{
		*ppcValue = strdup("1");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acDNSEnabled);
		
	}	
	//*value = strdup("1");
	return TRX_OK;
}
TRX_STATUS setWANDeviceWANConnectionDeviceWANPPPConnection_DNSServers(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_WANDEVICE_WANPPPCONNECTION_DNSSERVERS,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnection_DNSServers(char **ppcValue)
{
    /* OM_TR69C_WAN_IP_CONNECT_T stWanIpConnect = {0};
    CHAR szTemp[CHAR_LEN_64] = {0};
	memset(szTemp, 0, sizeof(szTemp));
    stWanIpConnect.eFlag = OM_TR69C_WAN_IP_CONNECT_DNS1;  

    if(OM_E_NONE != om_net_wan_IP_connect_get(&stWanIpConnect))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        *value = strdup("NONE");
		return TRX_OK;
    }

    stWanIpConnect.eFlag = OM_TR69C_WAN_IP_CONNECT_DNS2;  

    if(OM_E_NONE != om_net_wan_IP_connect_get(&stWanIpConnect))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        *value = strdup("NONE");
		return TRX_OK;
    }

    sprintf(szTemp,IP_FMT","IP_FMT,
                IP_ARG(stWanIpConnect.stWanIpConnectCfg.aucDnsSvr1),
                IP_ARG(stWanIpConnect.stWanIpConnectCfg.aucDnsSvr1));
    *value = strdup(szTemp); */
    char acDNSServers[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_WANPPPCONNECTION_DNSSERVERS,acDNSServers))
	{
		*ppcValue = strdup("0.0.0.0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acDNSServers);
		
	}	
    return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnection_MACAddress(char **ppcValue)
{
	//om_net_tr069_wan_stats( OM_TR69C_WAN_STATS_T *pstTsStats)
	/* OM_TR69C_WAN_STATS_T pstTsStats;
    CHAR szTemp[CHAR_LEN_32] = {0};
	memset(szTemp, 0, sizeof(szTemp));
    if(OM_E_NONE != om_net_tr069_wan_stats(&pstTsStats))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        *value = strdup("NONE");
		return TRX_OK;
    }

    sprintf(szTemp,MAC_FMT,MAC_ARG(pstTsStats.stTsList.aucMac));
    *value = strdup(szTemp); */
    char acMacAddress[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_WANPPPCONNECTION_MACADDRESS,acMacAddress))
	{
		*ppcValue = strdup("XX-XX-XX-XX-XX-XX");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acMacAddress);
		
	}	
    return TRX_OK;	
}


TRX_STATUS setWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_TunnelDial(const char *value)
{
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_TunnelDial(char **value)
{
	*value = strdup("0");
	return TRX_OK;
}

TRX_STATUS setWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_LNS(const char *value)
{/* 
	OM_NET_L2TP_CFG_T *pstl2tpCfg = (OM_NET_L2TP_CFG_T *)malloc(sizeof(OM_NET_L2TP_CFG_T));
	memset(pstl2tpCfg, 0, sizeof(OM_NET_L2TP_CFG_T));
	//CHAR acTemp[CHAR_LEN_32] = {0};
	//pstl2tpCfg->ulID=TRxObjNode_Node3_Num-1;
	pstl2tpCfg->ulID=0;

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
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_LNS(char **value)
{
	/* OM_NET_L2TP_CFG_T *pstl2tpCfg = (OM_NET_L2TP_CFG_T *)malloc(sizeof(OM_NET_L2TP_CFG_T));
	memset(pstl2tpCfg, 0, sizeof(OM_NET_L2TP_CFG_T));

	CHAR acTemp[CHAR_LEN_32] = {0};
	memset(acTemp, 0, sizeof(acTemp));
	//pstl2tpCfg->ulID=TRxObjNode_Node3_Num-1;
	pstl2tpCfg->ulID=0;

	if( OM_E_NONE != om_net_tr69_l2tp_get(pstl2tpCfg))
	{ 
		slog(TR69C_LOG_ERR,"ERROR");
		free(pstl2tpCfg);
 		*value = strdup("0");
		return TRX_OK;
	}
	
    sprintf(acTemp,"%d",pstl2tpCfg->stPppoeL2tp.enTunnelMode);
    *value =strdup(acTemp);    
	free(pstl2tpCfg); */
	*value = strdup("NONE");
    return TRX_OK;  
}



TRX_STATUS setWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_LnsIp(const char *value)
{
	/*
	OM_NET_L2TP_CFG_T *pstl2tpCfg = (OM_NET_L2TP_CFG_T *)malloc(sizeof(OM_NET_L2TP_CFG_T));
	memset(pstl2tpCfg, 0, sizeof(OM_NET_L2TP_CFG_T));
	//CHAR acTemp[CHAR_LEN_32] = {0};
	pstl2tpCfg->ulID=0;

	 util_str_to_ip((char *)value,pstl2tpCfg->stPppoeL2tp.aucLnsIp);	
	if( OM_E_NONE != om_net_tr69_l2tp_set(TR69C_L2TP_LNSSERVERIPADDR,pstl2tpCfg))
	{ 
		slog(TR69C_LOG_ERR,"ERROR");
		free(pstl2tpCfg);
 		return TRX_ERR;
	}
	*/
    return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_LnsIp(char **value)
{
	/* 
	OM_NET_L2TP_CFG_T *pstl2tpCfg = (OM_NET_L2TP_CFG_T *)malloc(sizeof(OM_NET_L2TP_CFG_T));
	memset(pstl2tpCfg, 0, sizeof(OM_NET_L2TP_CFG_T));

	CHAR acTemp[CHAR_LEN_32] = {0};
	memset(acTemp, 0, sizeof(acTemp));
	//pstl2tpCfg->ulID=TRxObjNode_Node3_Num-1;
	pstl2tpCfg->ulID=0;

	if( OM_E_NONE != om_net_tr69_l2tp_get(pstl2tpCfg))
	{ 
		slog(TR69C_LOG_ERR,"ERROR");
		free(pstl2tpCfg);
 		*value = strdup("NONE");
		return TRX_OK;
	} */
	
 //   sprintf(acTemp,"%d",pstl2tpCfg->stPppoeL2tp.aucLnsIp);
	/* util_ip_to_str(pstl2tpCfg->stPppoeL2tp.aucLnsIp,acTemp);
    *value =strdup(acTemp);    
	free(pstl2tpCfg); */
	*value = strdup("NONE");
    return TRX_OK;  
}


TRX_STATUS setWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_LnsName(const char *value)
{
	/* 
	OM_NET_L2TP_CFG_T *pstl2tpCfg = (OM_NET_L2TP_CFG_T *)malloc(sizeof(OM_NET_L2TP_CFG_T));
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
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_LnsName(char **value)
{
	/*
	OM_NET_L2TP_CFG_T *pstl2tpCfg = (OM_NET_L2TP_CFG_T *)malloc(sizeof(OM_NET_L2TP_CFG_T));
	memset(pstl2tpCfg, 0, sizeof(OM_NET_L2TP_CFG_T));

	//CHAR acTemp[CHAR_LEN_32] = {0};

	//pstl2tpCfg->ulID=TRxObjNode_Node3_Num-1;
	pstl2tpCfg->ulID=0;

 if( OM_E_NONE != om_net_tr69_l2tp_get(pstl2tpCfg))
	{ 
		slog(TR69C_LOG_ERR,"ERROR");
		free(pstl2tpCfg);
 		*value = strdup("NONE");
		return TRX_OK;
	}
	
 //   sprintf(acTemp,"%d",pstl2tpCfg->stPppoeL2tp.aucHostName);
    *value =strdup(pstl2tpCfg->stPppoeL2tp.aucHostName);    
	free(pstl2tpCfg);
	pstl2tpCfg=NULL
	*/
	*value = strdup("NONE");
    return TRX_OK;  
}



TRX_STATUS setWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_LnsPassword(const char *value)
{
/* 
OM_NET_L2TP_CFG_T *pstl2tpCfg = (OM_NET_L2TP_CFG_T *)malloc(sizeof(OM_NET_L2TP_CFG_T));
	memset(pstl2tpCfg, 0, sizeof(OM_NET_L2TP_CFG_T));
	//CHAR acTemp[CHAR_LEN_32] = {0};
	pstl2tpCfg->ulID=0;

	memcpy(pstl2tpCfg->stPppoeL2tp.aucTaPassWd,value,strlen(value));
	if( OM_E_NONE != om_net_tr69_l2tp_set(TR69C_L2TP_PASSWORDOFTUNNEL,pstl2tpCfg))
	{ 
		slog(TR69C_LOG_ERR,"ERROR");
		free(pstl2tpCfg);
 		return TRX_ERR;
	} */
    return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_LnsPassword(char **value)
{
/* 
	OM_NET_L2TP_CFG_T *pstl2tpCfg = (OM_NET_L2TP_CFG_T *)malloc(sizeof(OM_NET_L2TP_CFG_T));
	memset(pstl2tpCfg, 0, sizeof(OM_NET_L2TP_CFG_T));

//	CHAR acTemp[CHAR_LEN_32] = {0};

	//pstl2tpCfg->ulID=TRxObjNode_Node3_Num-1;
	pstl2tpCfg->ulID=0;

	if( OM_E_NONE != om_net_tr69_l2tp_get(pstl2tpCfg))
	{ 
		slog(TR69C_LOG_ERR,"ERROR");
		free(pstl2tpCfg);
 		*value = strdup("NONE");
		return TRX_OK;
	} 
	
  //  sprintf(acTemp,"%d",pstl2tpCfg->stPppoeL2tp.aucTaPassWd);
    *value =strdup(pstl2tpCfg->stPppoeL2tp.aucTaPassWd);    
	free(pstl2tpCfg);
	pstl2tpCfg=NULL;*/

	*value = strdup("NONE");
    return TRX_OK;  
}



TRX_STATUS setWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_L2TPMode(const char *value)
{
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_L2TPMode(char **value)
{
	*value = strdup("");
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_UpstreamTotalByte(char **value)
	{
	*value = strdup("");
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANPPPConnection_X_X_CMCC_DownstreamTotalByte(char **value)
{
	*value = strdup("");
	return TRX_OK;
}
/*BEGIN : add byduanguiyuan 20161125*/
/* InternetGatewayDevice.WANDevice.{i}.X_ATP_AlarmInstance */
TRX_STATUS getWANDeviceX_ATP_Alarm_LAN1(char **value)
{	
	char acNatEnabled[512]={0};

	if(UCI_OK != popen_cmd_get("uci get "UCI_WANDevice_X_ATP_Alarm_LAN1,acNatEnabled,sizeof(acNatEnabled)))
	{
		*value = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
	}
	else{
		if(strstr(acNatEnabled,"uci")==NULL)
		{
			*value = strdup(acNatEnabled);
		}else{
			slog(TR69C_LOG_ERR,"error by cmd(uci get %s)",UCI_WANDevice_X_ATP_Alarm_LAN1);
			*value = strdup("");
		}
	}	
	return TRX_OK;
}

TRX_STATUS getWANDeviceX_ATP_Alarm_LAN2(char **value)
{	
	char acNatEnabled[512]={0};
	if(UCI_OK != popen_cmd_get("uci get "UCI_WANDevice_X_ATP_Alarm_LAN2,acNatEnabled,sizeof(acNatEnabled)))
	{
		*value = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
	}
	else{
		if(strstr(acNatEnabled,"uci")==NULL)
		{
			*value = strdup(acNatEnabled);
		}else{
			slog(TR69C_LOG_ERR,"error by cmd(uci get %s)",UCI_WANDevice_X_ATP_Alarm_LAN2);
			*value = strdup("");
		}
	}	
	return TRX_OK;
}

TRX_STATUS getWANDeviceX_ATP_Alarm_LAN3(char **value)
{	
	char acNatEnabled[512]={0};
	if(UCI_OK != popen_cmd_get("uci get "UCI_WANDevice_X_ATP_Alarm_LAN3,acNatEnabled,sizeof(acNatEnabled)))
	{
		*value = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
	}
	else{
		if(strstr(acNatEnabled,"uci")==NULL)
		{
			*value = strdup(acNatEnabled);
		}else{
			slog(TR69C_LOG_ERR,"error by cmd(uci get %s)",UCI_WANDevice_X_ATP_Alarm_LAN3);
			*value = strdup("");
		}
	}	
	return TRX_OK;
}
TRX_STATUS getWANDeviceX_ATP_Alarm_LAN4(char **value)
{
		char acNatEnabled[512]={0};
	if(UCI_OK != popen_cmd_get("uci get "UCI_WANDevice_X_ATP_Alarm_LAN4,acNatEnabled,sizeof(acNatEnabled)))
	{
		*value = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
	}
	else{
		if(strstr(acNatEnabled,"uci")==NULL)
		{
			*value = strdup(acNatEnabled);
		}else{
			slog(TR69C_LOG_ERR,"error by cmd(uci get %s)",UCI_WANDevice_X_ATP_Alarm_LAN4);
			*value = strdup("");
		}
	}	
	return TRX_OK;
}
TRX_STATUS getWANDeviceX_ATP_Alarm_LanCheck(char **value)
{	*value = strdup("");	return TRX_OK;}
TRX_STATUS getWANDeviceX_ATP_Alarm_RSRPWeek(char **value)
{	*value = strdup("");	return TRX_OK;}
TRX_STATUS getWANDeviceX_ATP_Alarm_TempAmplifierHigh(char **value)
{	*value = strdup("");	return TRX_OK;}
TRX_STATUS getWANDeviceX_ATP_Alarm_TempAmplifierLow(char **value)
{	*value = strdup("");	return TRX_OK;}
TRX_STATUS getWANDeviceX_ATP_Alarm_TempModemHigh(char **value)
{	*value = strdup("");	return TRX_OK;}
TRX_STATUS getWANDeviceX_ATP_Alarm_TempModemLow(char **value)
{	*value = strdup("");	return TRX_OK;}
TRX_STATUS getWANDeviceX_ATP_Alarm_WanCheck(char **value)
{	*value = strdup("");	return TRX_OK;}
TRX_STATUS getWANDeviceX_ATP_Alarm_WebLogin(char **value)
{	*value = strdup("");	return TRX_OK;}
TRX_STATUS getWANDeviceX_ATP_Alarm_AutoAPN(char **value)
{	*value = strdup("");	return TRX_OK;}

/*END : add byduanguiyuan 20161125*/
/* 
	返回值 
	0  : 新的告警 cnt需要++
	1  : 恢复旧的告警
*/
/* add by duanguiyuan 20161128*/
int get_curret_time(char * timestr)
{
	/* 20000103 22:24:01 获取告警的 时间 */
 	time_t timep;
    struct tm *p;
    time(&timep);
    p = localtime(&timep); //取得当地时间
    sprintf(timestr,"%04d%02d%02d %02d:%02d:%02d",(1900+p->tm_year),(1+p->tm_mon),p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
	return 0;
}
int get_alarm_status_config(int num,char *timestr)
{
	char actmp[32]={0};
	char acName[32]={0};
	sprintf(acName,UCI_WANDevice_X_ATP_Alarm_STATUS_N"%d",num);
	if(UCI_OK != uci_config_get(acName,actmp))
	{	
		return 0;
	}
	if(atoi(actmp) == 0)
	{
		return 0;
	}
	/* set alarm time   */	
	memset(acName,0,sizeof(acName));
	sprintf(acName,UCI_WANDevice_X_ATP_Alarm_TIME_N"%d",num);
	if(UCI_OK != uci_config_get(acName,timestr))
	{	
	//	return 0;
	}
	return atoi(actmp);	
}
/* 	设置告警的状态写入 配置文件中 */
int set_alarm_status_config(int num,int cnt_stat,char *timestr)
{
	char actmp[32]={0};
	char acName[32]={0};
	sprintf(acName,UCI_WANDevice_X_ATP_Alarm_STATUS_N"%d",num);
	sprintf(actmp,"%d",cnt_stat);
	if(UCI_OK !=uci_config_set(acName,(const char *)actmp))
	{
		slog(TR69C_LOG_ERR,"ERROR");	
	}
	/* set alarm time   */	
	memset(acName,0,sizeof(acName));
	sprintf(acName,UCI_WANDevice_X_ATP_Alarm_TIME_N"%d",num);
printf("acName = %d \n",acName);
	//memset(actmp,0,sizeof(actmp));
	//sprintf(actmp,"%s",timestr);
	if(UCI_OK !=uci_config_set(acName,timestr))
	{
		slog(TR69C_LOG_ERR,"ERROR");	
	}
	return 0;	
}
/* add by duanguiyuan 20161128*/
TRX_STATUS get_alarmCnt(void)
{
	char actmp[32]={0};
	
	if(UCI_OK != uci_config_get(UCI_WANDevice_X_ATP_Alarm_CNT,actmp))
	{	
        slog(TR69C_LOG_ERR,"ERROR");
	}
	g_alarm_info.s_cnt=atoi(actmp);
	
	return TRX_OK;	
}

TRX_STATUS set_alarmCnt(int cnt)
{
	char actmp[32]={0};
	sprintf(actmp,"%d",cnt);
	if(UCI_OK !=uci_config_set(UCI_WANDevice_X_ATP_Alarm_CNT,(const char *)actmp))
	{
		slog(TR69C_LOG_ERR,"ERROR");	
	}
    return TRX_OK;
}
void set_alarm_value(int id,int status,char *scha,char*buf_time)
{
	char acTemp[256]={0};
/* set alarm flag */
	g_alarm_info.alarm_flag=TR69C_ALARM_YES;
	g_alarm_info.alarm_status[id]=TR69C_ALARM_YES;
	id++;
/* add cnt */
	if(g_alarm_info.s_cnt < 65536)
	{
		g_alarm_info.s_cnt++;
	}else{
		g_alarm_info.s_cnt = 1;
	}
	set_alarmCnt(g_alarm_info.s_cnt);
	#if 1
/* set uci value info */
	memset(acTemp,0,sizeof(acTemp));
	sprintf(acTemp,"uci set %s='SEQUENCENO=%d,ALARMID=%d,ALARMCODE="TR69C_ALARMCODE_LAN\
		",ALARMSEVERITY=4,EVENTTIME=%s,STATUS=%d,ADDITIONALINFO=LAN%d connection alarm!'",scha,g_alarm_info.s_cnt,id,buf_time,status,id);
	system(acTemp);
	printf("0 uci cmd=%s\n",acTemp);


#endif
	return;
}
/* add by duanguiyuan */
void set_alarm_value2(int id,int status,char *scha,int count_n,char *buf_time)
{
	char acTemp[256]={0};
/* set alarm flag */
	g_alarm_info.alarm_flag=TR69C_ALARM_YES;
	g_alarm_info.alarm_status[id]=TR69C_ALARM_YES;
	id++;
	memset(acTemp,0,sizeof(acTemp));
	sprintf(acTemp,"uci set %s='SEQUENCENO=%d,ALARMID=%d,ALARMCODE="TR69C_ALARMCODE_LAN\
		",ALARMSEVERITY=4,EVENTTIME=%s,STATUS=%d,ADDITIONALINFO=LAN%d connection alarm!'",scha,count_n,id,buf_time,status,id);
	system(acTemp);
	printf("1 uci cmd=%s\n",acTemp);

}

OM_NET_LINK_STA g_stPalLinkSta={0};
void getLinkstate(void)
{
	int stat;
	OM_NET_LINK_STA pstPalLinkSta={0};
	char buf_time[32]={0};
	
	om_net_lan_link_status(&pstPalLinkSta);
	{
#if 1
		g_stPalLinkSta.uiLinkPort1 = pstPalLinkSta.uiLinkPort1;
		g_stPalLinkSta.uiLinkPort2 = pstPalLinkSta.uiLinkPort2;
		g_stPalLinkSta.uiLinkPort3 = pstPalLinkSta.uiLinkPort3;	
		g_stPalLinkSta.uiLinkPort4 = pstPalLinkSta.uiLinkPort4;

#else
		if((stat = get_alarm_status_config(1,buf_time)) ==0)
		{
			g_stPalLinkSta.uiLinkPort1 = pstPalLinkSta.uiLinkPort1;
		}else{
			g_stPalLinkSta.uiLinkPort1 = stat;
		}
		if((stat = get_alarm_status_config(2,buf_time)) ==0)
		{
			g_stPalLinkSta.uiLinkPort2 = pstPalLinkSta.uiLinkPort2;
		}else{
			g_stPalLinkSta.uiLinkPort2= stat;
		}
		if((stat = get_alarm_status_config(3,buf_time)) ==0)
		{
			g_stPalLinkSta.uiLinkPort3 = pstPalLinkSta.uiLinkPort3;
		}else{
			g_stPalLinkSta.uiLinkPort3= stat;
		}
		if((stat = get_alarm_status_config(4,buf_time)) ==0)
		{
			g_stPalLinkSta.uiLinkPort4 = pstPalLinkSta.uiLinkPort4;
		}else{
			g_stPalLinkSta.uiLinkPort4= stat;
		}
#endif
	}
}
/*

 stat = 2 解除告警
 stat = 1 上报告警
 流水号 的 数值范围:0-65535
 Alarm ID : 

 */
void start_alarm_info(int id,int stat,char *scha)
{
	char buf_time[32]={0};

	int count_num=get_alarm_status_config(id,buf_time);
	if(0 == count_num)
	{	
		stat=TR69C_ALARM_REPORT;
		memset(buf_time,0,sizeof(buf_time));
		get_curret_time(buf_time);
		set_alarm_value(id,stat,scha,buf_time);
		set_alarm_status_config(id,g_alarm_info.s_cnt,buf_time);
	
	}else{
		stat=TR69C_ALARM_RCLEAR;
		set_alarm_value2(id,stat,scha,count_num,buf_time);
		set_alarm_status_config(id,0,"0:0:0");
	}
}
int alarm_check_procc(void)
{
	OM_NET_LINK_STA pstPalLinkSta={0};
	int ret_code=0;
printf("test ........................................................\n");
	om_net_lan_link_status(&pstPalLinkSta);
	{
		if(pstPalLinkSta.uiLinkPort1 != g_stPalLinkSta.uiLinkPort1)
		{
			g_stPalLinkSta.uiLinkPort1 = pstPalLinkSta.uiLinkPort1;
			start_alarm_info(TR69C_ALARM_TYPE_LAN1,pstPalLinkSta.uiLinkPort1,UCI_WANDevice_X_ATP_Alarm_LAN1);
			printf("lan statue chenge    1   \n");
			ret_code=1;
		}
		if(pstPalLinkSta.uiLinkPort2 != g_stPalLinkSta.uiLinkPort2)
		{
			g_stPalLinkSta.uiLinkPort2 = pstPalLinkSta.uiLinkPort2 ;
			start_alarm_info(TR69C_ALARM_TYPE_LAN2,pstPalLinkSta.uiLinkPort2,UCI_WANDevice_X_ATP_Alarm_LAN2);
			printf("lan statue chenge    2   \n");
			ret_code=2;
		}
		if(pstPalLinkSta.uiLinkPort3 != g_stPalLinkSta.uiLinkPort3)
		{
			g_stPalLinkSta.uiLinkPort3 = pstPalLinkSta.uiLinkPort3;
			start_alarm_info(TR69C_ALARM_TYPE_LAN3,pstPalLinkSta.uiLinkPort3,UCI_WANDevice_X_ATP_Alarm_LAN3);
			printf("lan statue chenge    3   \n");
			ret_code=3;
		}
		if(pstPalLinkSta.uiLinkPort4 != g_stPalLinkSta.uiLinkPort4)
		{
			g_stPalLinkSta.uiLinkPort4 = pstPalLinkSta.uiLinkPort4;
			start_alarm_info(TR69C_ALARM_TYPE_LAN4,pstPalLinkSta.uiLinkPort4,UCI_WANDevice_X_ATP_Alarm_LAN4);
			printf("lan statue chenge    4   \n");
			ret_code=4;
		}
		
	}
	printf("test lan1=%d lan2=%d lan3=%d lan4=%d \n",pstPalLinkSta.uiLinkPort1,pstPalLinkSta.uiLinkPort2,pstPalLinkSta.uiLinkPort3,pstPalLinkSta.uiLinkPort4);
	return ret_code;
}
void getalarmvlueInit(void)
{
	 
	getLinkstate();
	get_alarmCnt();
	
}
void initWANDeviceInstances(void)
{
    TRxObjNode *n;
    int id;
    InstanceDesc  *wanDeviceIdp;
 // InstanceDesc  *wanCommonInterfaceCfgIdp;
 	InstanceDesc  *X_ATP_APNIdp;
    InstanceDesc  *wanConnectDeviceIdp;
    InstanceDesc  *wanConnectDeviceWanIpConnectIdp;
    InstanceDesc  *wanConnectDeviceWanPppConnectIdp;
    InstanceDesc  *wanConnectDeviceWanIpConnectPortMapIdp;
    InstanceDesc  *wanConnectDeviceWanIpConnectStatsIdp;
	
	InstanceDesc  *wanConnectDevWanPPPConnectIdp;
	InstanceDesc  *wanConnectDevWanPPPConnectX_ATP_DMZIdp;
	InstanceDesc  *wanConnectDevWanPPPConnectPortMappIdp;
	InstanceDesc  *wanConnectDevWanPPPConnectPortStatsIdp;

	InstanceDesc  *X_ATP_WANNetConfigInfoIdp;
    /** InternetGatewayDevice.WANDevice. */
    n = WANDeviceInstanceDesc;
    id = 1;
    wanDeviceIdp = getNewInstanceDesc(n, NULL, id);

	n = WANDeviceWANConnectionDeviceInstanceDesc;
    wanConnectDeviceIdp = getNewInstanceDesc(n, wanDeviceIdp, id);

	n = WANDeviceX_ATP_WANNetConfigInfoInstanceDesc;
    X_ATP_WANNetConfigInfoIdp = getNewInstanceDesc(n, wanDeviceIdp, id);
	/* InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.*/
   	n = WANDeviceWANConnectionDeviceWANIPConnectionInstanceDesc;
   	wanConnectDeviceWanIpConnectIdp = getNewInstanceDesc(n, wanConnectDeviceIdp, id);

	n = WANDeviceWANConnectionDeviceWANIPConnectionStatsDesc;
   	wanConnectDeviceWanIpConnectStatsIdp = getNewInstanceDesc(n, wanConnectDeviceWanIpConnectIdp, id);

  
	/* InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.*/
	n = WANDeviceWANConnectionDeviceWANPPPConnectionInstanceDesc;
   	wanConnectDevWanPPPConnectIdp = getNewInstanceDesc(n, wanConnectDeviceIdp, id);

	n = WANDeviceWANConnectionDeviceWANPPPConnectionX_ATP_DMZDesc;
   	wanConnectDevWanPPPConnectX_ATP_DMZIdp = getNewInstanceDesc(n, wanConnectDevWanPPPConnectIdp, id);

	n = WANDeviceWANConnectionDeviceWANPPPConnectionPortMappingDesc;
   	wanConnectDevWanPPPConnectPortMappIdp = getNewInstanceDesc(n, wanConnectDevWanPPPConnectIdp, id);

	n = WANDeviceWANConnectionDeviceWANPPPConnectionStatsDesc;
   	wanConnectDevWanPPPConnectPortStatsIdp = getNewInstanceDesc(n, wanConnectDevWanPPPConnectIdp, id);
 	/* InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.Stats. */
  #if 0
 	/* InternetGatewayDevice.WANDevice.1.WANCommonInterfaceConfig. */
  	n = WANDeviceWANCommonInterfaceConfigDesc;
	wanCommonInterfaceCfgIdp = getNewInstanceDesc(n, wanDeviceIdp, id);

    /* InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}. */
	n = WANX_ATP_APNDescObj;
	X_ATP_APNIdp =  getNewInstanceDesc(n, wanDeviceIdp, id);
	
	id = 2;
	n = WANDeviceWANConnectionDeviceInstanceDesc;
    wanConnectDeviceIdp = getNewInstanceDesc(n, wanDeviceIdp, id);
	id = 1;

    /* port mapping instance init  */
    /* 暂未实现 */
	/* InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1. */
   	n = WANDeviceWANConnectionDeviceWANIPConnectionPortMappingInstanceDesc;
  	wanConnectDeviceWanIpConnectPortMapIdp = getNewInstanceDesc(n, wanConnectDeviceWanIpConnectIdp, id);

    /* InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection. */
    n = WANDeviceWANConnectionDeviceWANPPPConnectionInstanceDesc;
   	wanConnectDeviceWanPppConnectIdp = getNewInstanceDesc(n, wanConnectDeviceIdp, id);
#endif
}

