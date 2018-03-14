/******************************************************************************

                  ��Ȩ���� (C), 2010-2012, �Ϻ�徴�ͨ�ſƼ����޹�˾

 ******************************************************************************
  �� �� ��   : wanDeviceHandlers.c
  �� �� ��   : ����
  ��    ��   : zhangjiasi
  ��������   : 2015��6��4��
  ����޸�   :
  ��������   : wan��Ϣ�����ֱ�׼�ӿ�ʵ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��6��4��
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

#include "wanDeviceParams.h"
#include "SOAPParser/CPEframework.h"
/*Add by zhanghao ��ȡ�����ļ����� 2016/6/15 begin*/
#include "uci_config.h"
#include "tr69_lua.h"
#define TAUARRY_LEN 128

/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

extern int g_log_level;
/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/

/* wanʵ���� */
TRX_STATUS getWANDevice_WANConnectionNumberOfEntries(char **ppcValue)
{
	char acWANConnection[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_WANCONNECTIONNUBEROFENTRIES,acWANConnection))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWANConnection);
		
	}	
   // *value = strdup("1");   
    return TRX_OK;
}
TRX_STATUS getLANDeviceWANEthernetInterfaceConfig_MACAddress(char **ppcValue)
{
	
     
    return getWANDeviceWANConnectionDeviceWANIPConnection_MACAddress(ppcValue);
}


/* ��ǰ״̬
��Up��
��Down��
��Initializing��
��Unavailable�� */
TRX_STATUS getWANDeviceWANCommonInterfaceConfig_PhysicalLinkStatus(char **value)
{
    *value = strdup("up");
    return TRX_OK;
}



/** InternetGatewayDevice.WANDevice.WANConnectionDevice.WANIPConnection. */
TRX_STATUS addWANDeviceWANConnectionDeviceWANIPConnectionI(char **value)
{
	InstanceDesc *idp;
	if( (idp = getCurrentInstanceDesc()) != NULL){
		return TRX_OK;
	}
	return TRX_ERR;
}
TRX_STATUS delWANDeviceWANConnectionDeviceWANIPConnectionI(const char *value)
{
	InstanceDesc *idp;
	if ((idp= getNewInstanceDesc( getCurrentNode(), NULL, 0)) != NULL){
		return TRX_OK;
	}
	return TRX_ERR;
}

TRX_STATUS addPPPConnection(char **value)
{
    InstanceDesc *idp;
    if ( (idp= getNewInstanceDesc( getCurrentNode(),getCurrentInstanceDesc(),  0)) != NULL ) {
        *value = strdup(itoa(idp->instanceID));
        return TRX_REBOOT; /* or TRX_BOOT if necessary */
    }
    return TRX_ERR;
}

TRX_STATUS deletePPPConnection(const char *value)
{
	TRxObjNode *n = getCurrentNode();
//	InstanceDesc *idp;
	int     id = atoi(value);
	if ( !deleteInstanceDesc(n,id )){
		fprintf(stderr, "Delete PPPConnection instance = %d\n", id);
		return TRX_REBOOT; /* or TRX_BOOT if necessary */
	}
	return TRX_ERR;
}
TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_Enable(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_WANDEVICE_PORTMAPPING_ENABLE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_Enable(char **ppcValue)
{
	char acWANIPConnection_Enable[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_PORTMAPPING_ENABLE,acWANIPConnection_Enable))
	{
		*ppcValue = strdup("false");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWANIPConnection_Enable);
		
	}	
	//*value = strdup("true");
	return TRX_OK;
}

TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_ConnectionStatus(char **ppcValue)
{
	char acConnectionStatus[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_PORTMAPPING_CONNECTIONSTATUS,acConnectionStatus))
	{
		*ppcValue = strdup("DisConnected");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acConnectionStatus);
		
	}	
   // *value = strdup("Connected");
    return TRX_OK;
}

TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_PossibleConnectionTypes(char **value)
{
	*value = strdup("Unconfigured,IP_Routed,IP_Bridged");
	return TRX_OK;
}

/* ��ǰָ�����������ͣ�
��Unconfigured��
��IP_Routed��
��IP_Bridged�� */
TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_ConnectionType(const char *value)
{
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_ConnectionType(char **value)
{
	*value = strdup("IP_Routed");
	return TRX_OK;
}

/* �������� */
TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_Name(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_WANDEVICE_WANIPCONNECTION_NAME,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_Name(char **ppcValue)
{
	char acName[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_WANIPCONNECTION_NAME,acName))
	{
		*ppcValue = strdup("USBXX");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acName);
		
	}	
	//*value = strdup("usb0");
	return TRX_OK;
}

/* Indicates if Realm-specific IP (RSIP) is available as
   a feature on the CPE. */
TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_RSIPAvailable(char **value)
{
	*value = strdup("false");
	return TRX_OK;
}

/* ����IP ���ӵĴ�����ʽ��
��OnDemand��
��AlwaysOn��
��Manual�� */
TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_ConnectionTrigger(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_WANDEVICE_WANIPCONNECTION_CONNECTIONTIiGGER,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_ConnectionTrigger(char **ppcValue)
{   
	char acConnectionTrigger[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_WANIPCONNECTION_CONNECTIONTIiGGER,acConnectionTrigger))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acConnectionTrigger);
		
	}	
	//*value = strdup("AlwaysOn");
	return TRX_OK;
}

/* Defines the Rx protocol to be used. Enumeration of:
��Off��
��RIPv1�� (OPTIONAL)
��RIPv2�� (OPTIONAL)
��OSPF�� (OPTIONAL) */

TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_RouteProtocolRx(const char *value)
{
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_RouteProtocolRx(char **value)
{
	*value = strdup("Off");
	return TRX_OK;
}
/* ���һ�ε������Ӳ��ܳɹ�������ԭ��
��ERROR_NONE��
��ERROR_COMMAND_ABORTED��
��ERROR_NOT_ENABLED_FOR_INTERNET��
��ERROR_USER_DISCONNECT��
��ERROR_ISP_DISCONNECT��
��ERROR_IDLE_DISCONNECT��
��ERROR_FORCED_DISCONNECT��
��ERROR_NO_CARRIER��
��ERROR_IP_CONFIGURATION��
��ERROR_UNKNOWN� */

TRX_STATUS setWANDeviceWANConnectionDeviceWANIPConnection_LastConnectionError(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_WANDEVICE_WANIPCONNECTION_LASTCONNECTIONERROR,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
	return TRX_OK;
}
TRX_STATUS getWANDeviceWANConnectionDeviceWANIPConnection_LastConnectionError(char **ppcValue)
{
	char acLastConnectionError[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_WANDEVICE_WANIPCONNECTION_LASTCONNECTIONERROR,acLastConnectionError))
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
