/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : wanDeviceParams.h
  版 本 号   : 初稿
  作    者   : zhangjiasi
  生成日期   : 2015年6月4日
  最近修改   :
  功能描述   : wanDeviceParams.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2015年6月4日
    作    者   : zhangjiasi
    修改内容   : 创建文件

******************************************************************************/
#ifndef __WANDEVICEPARAMS_H__
#define __WANDEVICEPARAMS_H__

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "tr69cdefs.h"
/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

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

#ifdef CPEVARNAMEINSTANCE
#undef CPEVARNAMEINSTANCE
#endif

/* InternetGatewayDevice.WANDevice.i.WANConnectionDevice.i.WANPPPnection.i. */
//SVAR(Enable);
SVAR(Name);
SVAR(Uptime);
SVAR(LastConnectionError);
SVAR(Username);
SVAR(Password);  
SVAR(ExternalIPAddress);
SVAR(MaxMRUSize);
SVAR(DNSEnabled);
SVAR(DNSServers);
SVAR(MACAddress);
SVAR(X_CMCC_TunnelDial);
SVAR(X_CMCC_LNS);
SVAR(X_CMCC_LnsIp);
SVAR(X_CMCC_LnsName);
SVAR(X_CMCC_LnsPassword);
SVAR(X_CMCC_L2TPMode);
SVAR(X_CMCC_UpstreamTotalByte);
SVAR(X_CMCC_DownstreamTotalByte);
#if 1
/* InternetGatewayDevice.WANDevice.{i}. */
SVAR(WANConnectionNumberOfEntries);
SVAR(WANCommonInterfaceConfig);
SVAR(WANEthernetInterfaceConfig);
SVAR(MACAddress);

SVAR(WANConnectionDevice);
SVAR(X_ATP_WANNetConfigInfo);
SVAR(X_ATP_AlarmInstance);

/* InternetGatewayDevice.WANDevice.1.WANCommonInterfaceConfig. */
SVAR(Layer1UpstreamMaxBitRate);
SVAR(Layer1DownstreamMaxBitRate);
SVAR(PhysicalLinkStatus);
SVAR(TotalBytesSent);
SVAR(TotalBytesReceived);
SVAR(TotalPacketsSent);
SVAR(TotalPacketsReceived);

/* InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}. */
SVAR(WANIPConnectionNumberOfEntries);
SVAR(WANPPPConnectionNumberOfEntries);
SVAR(WANIPConnection);
SVAR(WANPPPConnection);

/* InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.X_ATP_APN.{i}. */
SVAR(X_ATP_APN);
SVAR(APN);
SVAR(AuthenticationProtocol);
SVAR(Username);
SVAR(Password);
SVAR(DialNumber);
SVAR(AutoAPN);


/* InternetGatewayDevice.WANDevice.{i}.X_ATP_AlarmInstance */
SVAR(LAN1);
SVAR(LAN2);
SVAR(LAN3);
SVAR(LAN4);
SVAR(LanCheck);
SVAR(RSRPWeek);
SVAR(TempAmplifierHigh);
SVAR(TempAmplifierLow);
SVAR(TempModemHigh);
SVAR(TempModemLow);
SVAR(WanCheck);
SVAR(WebLogin);




/* InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1. */
SVAR(Enable);
SVAR(Reset);
SVAR(ConnectionStatus);
SVAR(PossibleConnectionTypes);
SVAR(ConnectionType);
//SVAR(Name);
//SVAR(Uptime);
//SVAR(LastConnectionError);
SVAR(RSIPAvailable );
SVAR(NATEnabled);
SVAR(AddressingType);
//SVAR(ExternalIPAddress);
SVAR(SubnetMask);
SVAR(DefaultGateway);
//SVAR(DNSEnabled);
SVAR(DNSOverrideAllowed);
//SVAR(DNSServers);
//SVAR(MACAddress);
SVAR(ConnectionTrigger);
SVAR(X_ATP_ServiceList);
SVAR(X_ATP_NATType);
SVAR(RouteProtocolRx);
SVAR(PortMappingNumberOfEntries);
SVAR(PortMapping);
SVAR(MaxMTUSize);
SVAR(MAXMTUSize);
//SVAR(X_ATP_APN);
SVAR(X_ATP_AuthenticationProtocol);
SVAR(X_ATP_Username);
SVAR(X_ATP_DialNumber);
SVAR(X_ATP_AutoAPN);

/* InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection.{i}.X_ATP_CAR. */
SVAR(X_ATP_CAR);
//SVAR(Enable);
SVAR(UplinkRate);

/* InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection.{i}.X_ATP_DMZ. */
SVAR(X_ATP_DMZ);
SVAR(DMZEnable);
SVAR(DMZHostIPAddress);


/* InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection.{i}.X_ATP_WirelessStatus. */
SVAR(X_ATP_WirelessStatus);

SVAR(PLMN);
SVAR(ServiceStatus);
SVAR(Bandwidth);
SVAR(CellID);
SVAR(SignalStrength);
SVAR(RSSI);
SVAR(RSRP);
SVAR(SINR);
SVAR(RSRQ);
SVAR(Roam);
SVAR(Frequency);
SVAR(SignalQuality);
SVAR(RoamStatus);
SVAR(PhysicalCellID);









/* InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.{i} */
/* InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.*/
SVAR(PortMappingEnabled);
SVAR(InternalClient);
SVAR(RemoteHost);

SVAR(ExternalPort);
SVAR(ExternalPortEndRange);
SVAR(InternalPort);
SVAR(PortMappingProtocol);




/* InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.Stats. */
SVAR(Stats);
SVAR(EthernetBytesSent);
SVAR(EthernetBytesReceived);
SVAR(EthernetPacketsSent);
SVAR(EthernetPacketsReceived);

SVAR(EthernetErrorsSent);
SVAR(EthernetErrorsReceived);
SVAR(EthernetDiscardPacketsSent);
SVAR(EthernetDiscardPacketsReceived);
SVAR(X_ATP_EthernetUplinkRate);
SVAR(X_ATP_EthernetDownlinkRate);







#endif

/* 函数原型定义 */
/* InternetGatewayDevice.WANDevice.{i}. */
TRXGFUNC(getWANDevice_WANConnectionNumberOfEntries);
TRXGFUNC(getLANDeviceWANEthernetInterfaceConfig_MACAddress);


/* InternetGatewayDevice.WANDevice.1.WANCommonInterfaceConfig. */
TRXGFUNC(getWANDeviceWANCommonInterfaceConfig_Layer1UpstreamMaxBitRate);
TRXGFUNC(getWANDeviceWANCommonInterfaceConfig_Layer1DownstreamMaxBitRate);
TRXGFUNC(getWANDeviceWANCommonInterfaceConfig_PhysicalLinkStatus);
TRXGFUNC(getWANDeviceWANCommonInterfaceConfig_TotalBytesSent);
TRXGFUNC(getWANDeviceWANCommonInterfaceConfig_TotalBytesReceived);
TRXGFUNC(getWANDeviceWANCommonInterfaceConfig_TotalPacketsSent);
TRXGFUNC(getWANDeviceWANCommonInterfaceConfig_TotalPacketsReceived);


/* InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}. */
TRXSFUNC(deleteWANConnectionDevice);
TRXGFUNC(addWANConnectionDevice);

TRXGFUNC(getWANDeviceWANConnectionDevice_WANIPConnectionNumberOfEntries);
TRXGFUNC(getPPPConNumberEntries);

/* InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.X_ATP_APN.{i}. */
TRXSFUNC(setWANAPN);
TRXGFUNC(getWANAPN);


TRXSFUNC(setWANAuthenticationProtocol);
TRXGFUNC(getWANAuthenticationProtocol);

TRXSFUNC(setWANUsername);
TRXGFUNC(getWANUsername);

TRXSFUNC(setWANPassword);
TRXGFUNC(getWANPassword);

TRXSFUNC(setWANDialNumber);
TRXGFUNC(getWANDialNumber);


TRXGFUNC(getWANAutoAPN);


/* InternetGatewayDevice.WANDevice.{i}.X_ATP_AlarmInstance */
TRXGFUNC(getWANDeviceX_ATP_Alarm_LAN1);
TRXGFUNC(getWANDeviceX_ATP_Alarm_LAN2);
TRXGFUNC(getWANDeviceX_ATP_Alarm_LAN3);
TRXGFUNC(getWANDeviceX_ATP_Alarm_LAN4);
TRXGFUNC(getWANDeviceX_ATP_Alarm_LanCheck);
TRXGFUNC(getWANDeviceX_ATP_Alarm_RSRPWeek);
TRXGFUNC(getWANDeviceX_ATP_Alarm_TempAmplifierHigh);
TRXGFUNC(getWANDeviceX_ATP_Alarm_TempAmplifierLow);
TRXGFUNC(getWANDeviceX_ATP_Alarm_TempModemHigh);
TRXGFUNC(getWANDeviceX_ATP_Alarm_TempModemLow);
TRXGFUNC(getWANDeviceX_ATP_Alarm_WanCheck);
TRXGFUNC(getWANDeviceX_ATP_Alarm_WebLogin);
TRXGFUNC(getWANDeviceX_ATP_Alarm_AutoAPN);




/* InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1. */
TRXSFUNC(delWANDeviceWANConnectionDeviceWANIPConnectionI);
TRXGFUNC(addWANDeviceWANConnectionDeviceWANIPConnectionI);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_Enable);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_Enable);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_Reset);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_Reset);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_ConnectionStatus);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_PossibleConnectionTypes);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_ConnectionType);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_ConnectionType);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_Name);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_Name);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_Uptime);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_RSIPAvailable);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_NATEnabled);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_NATEnabled);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_AddressingType);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_AddressingType);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_ExternalIPAddress);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_ExternalIPAddress);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_SubnetMask);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_SubnetMask);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_DefaultGateway);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_DefaultGateway);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_DNSEnabled);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_DNSEnabled);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_DNSOverrideAllowed);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_DNSOverrideAllowed);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_DNSServers);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_DNSServers);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_MACAddress);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_MACAddress);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_ConnectionTrigger);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_ConnectionTrigger);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_RouteProtocolRx);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_RouteProtocolRx);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_LastConnectionError);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_LastConnectionError);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_PortMappingNumberOfEntries);

TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_MaxMTUSize);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_MaxMTUSize);


TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_APN);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_APN);


TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_AuthenticationProtocol);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_AuthenticationProtocol);

TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_Username);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_Username);

TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_DialNumber);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_DialNumber);

TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_AutoAPN);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_AutoAPN);


/*InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection.{i}.X_ATP_CAR.*/

TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_CAREnable);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_CAREnable);

TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_CARUplinkRate);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_CARUplinkRate);


/*InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection.{i}.X_ATP_DMZ.*/

TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_DMZEnable);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_DMZEnable);

TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_DMZHostIPAddress);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_DMZHostIPAddress);


/*InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection.{i}.X_ATP_WirelessStatus.*/
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_PLMN);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_ServiceStatus);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_Bandwidth);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_CellID);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_SignalStrength);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_RSSI);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_RSRP);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_RSRQ);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_SINR);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_Roam);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_Frequency);

/*InternetGatewayDevice.WANDevice.{i}.X_ATP_WANNetConfigInfo.*/

TRXGFUNC(getWANDeviceX_ATP_WANNetConfigInfo_RSSI);
TRXGFUNC(getWANDeviceX_ATP_WANNetConfigInfo_RSRQ);
TRXGFUNC(getWANDeviceX_ATP_WANNetConfigInfo_SINR);
TRXGFUNC(getWANDeviceX_ATP_WANNetConfigInfo_CellID);
TRXGFUNC(getWANDeviceX_ATP_WANNetConfigInfo_PLMN);
TRXGFUNC(getWANDeviceX_ATP_WANNetConfigInfo_RSRP);
TRXGFUNC(getWANDevX_ATP_WANNetConfigInfo_SerStatus);
TRXGFUNC(getWANDeviceX_ATP_WANNetConfigInfo_SignalQuality);
TRXGFUNC(getWANDevX_ATP_WANNetConfigInfo_RoamStatus);
TRXGFUNC(getWANDeviceX_ATP_WANNetConfigInfo_PhysicalCellID);




/* InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.{i} */
TRXSFUNC(delWANDeviceWANConnectionDeviceWANIPConnectionPortMappingI);
TRXGFUNC(addWANDeviceWANConnectionDeviceWANIPConnectionPortMappingI);
/* InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.*/
TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnectionPortMapping_PortMappingEnabled);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnectionPortMapping_PortMappingEnabled);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnectionPortMapping_InternalClient);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnectionPortMapping_InternalClient);

TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnectionPortMapping_RemoteHost);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnectionPortMapping_RemoteHost);

TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_ExternalPort);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_ExternalPort);

TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_ExternalPortEndRange);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_ExternalPortEndRange);

TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_InternalPort);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_InternalPort);

TRXSFUNC(setWANDeviceWANConnectionDeviceWANIPConnection_PortMappingProtocol);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnection_PortMappingProtocol);




/* InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.Stats. */
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnectionStats_EthernetBytesSent);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnectionStats_EthernetBytesReceived);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnectionStats_EthernetPacketsSent);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnectionStats_EthernetPacketsReceived);

TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnectionStats_EthernetErrorsSent);

TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnectionStats_EthernetErrorsReceived);

TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnectionStats_EthernetDiscardPacketsSent);

TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnectionStats_EthernetDiscardPacketsReceived);

TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnectionStats_X_ATP_EthernetUplinkRate);

TRXGFUNC(getWANDeviceWANConnectionDeviceWANIPConnectionStats_X_ATP_EthernetDownlinkRate);


/* InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.WANPPPConnection. */
TRXSFUNC(deletePPPConnection);
TRXGFUNC(addPPPConnection);

TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnection_Enable);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANPPPConnection_Enable);

TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnection_Name);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnection_Uptime);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnection_LastConnectionError);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANPPPConnection_NATEnabled);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnection_NATEnabled);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANPPPConnection_MAXMTUSize);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnection_MAXMTUSize);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANPPPConnection_DMZEnable);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnection_DMZEnable);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANPPPConnection_DMZHostIPAddress);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnection_DMZHostIPAddress);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnection_Username);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnection_Password);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnection_ExternalIPAddress);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnection_MaxMRUSize);
TRXGFUNC(getWANDevWANConnectDevWANPPPConnect_ConnectTrigger);
TRXGFUNC(getWANDevWANConnectDevWANPPPConnect_X_ATP_ServiceList);
TRXGFUNC(getWANDevWANConnectDevWANPPPConnect_X_ATP_NATType);
TRXGFUNC(getWANDevWANConnectDevWANPPPConnect_ConnectionStatus);
TRXGFUNC(getWANDevWANConnectDevWANPPPConnect_DefaultGateway);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnectionStats_EthernetBytesSent);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnectionStats_EthernetBytesReceived);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnectionStats_EthernetDiscardPacketsSent);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnectionStats_EthernetDiscardPacketsRec);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnectionStats_EthernetErrorsSent);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnectionStats_EthernetErrorsReceived);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnectionStats_EthernetPacketsSent);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnectionStats_EthernetPacketsReceived);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnectionStats_X_ATP_EthernetUplinkRate);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnectionStats_X_ATP_EthernetDownlinkRate);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnection_DNSEnabled);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnection_DNSServers);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnection_MACAddress);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_TunnelDial);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_LNS);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_LnsIp);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_LnsName);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_LnsPassword);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_L2TPMode);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_UpstreamTotalByte);
TRXGFUNC(getWANDeviceWANConnectionDeviceWANPPPConnection_X_X_CMCC_DownstreamTotalByte);
	
TRXSFUNC(setWANDeviceWANConnectionDeviceWANPPPConnection_Name);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_TunnelDial);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_LNS);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_LnsIp);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_LnsName);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_LnsPassword);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_L2TPMode);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANPPPConnection_DNSEnabled);
TRXSFUNC(setWANDeviceWANConnectionDeviceWANPPPConnection_DNSServers);
/* 函数原型定义 */
void initWANDeviceInstances(void);
int alarm_check_procc(void);
void getalarmvlueInit(void);
#endif /* __WANDEVICEPARAMS_H__ */
