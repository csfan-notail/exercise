/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : wanDeviceParams.c
  版 本 号   : 初稿
  作    者   : zhangjiasi
  生成日期   : 2015年6月4日
  最近修改   :
  功能描述   : wan相关信息。接口定义及树形结构拓扑定义
  函数列表   :
  修改历史   :
  1.日    期   : 2015年6月4日
    作    者   : zhangjiasi
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "wanDeviceParams.h"
#define WANAPN_LEN 64
#define X_ATP_APN_LEN 64
#define X_ATP_AUTHENTICATIONPROTOCOL_LEN 64
#define X_ATP_USERNAME_LEN 64
#define X_ATP_DIALNUMER_LEN 64
#define REMOTEHOST_LEN 64
#define PORTMAPPINGPROTOCOL_LEN 64
#define DMZHOSTIPADDRESS_LEN 64

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

/* InternetGatewayDevice.WANDevice.1.WANCommonInterfaceConfig. */
/*
TRxObjNode WANDeviceWANCommonInterfaceConfigDesc[] = {
	{Layer1UpstreamMaxBitRate,{{tUnsigned,0}},
	  NULL,
	  getWANDeviceWANCommonInterfaceConfig_Layer1UpstreamMaxBitRate,
	  NULL,NULL},
	{Layer1DownstreamMaxBitRate,{{tUnsigned,0}},
	  NULL,
	  getWANDeviceWANCommonInterfaceConfig_Layer1DownstreamMaxBitRate,
	  NULL,NULL},
	{PhysicalLinkStatus,{{tString,16}},
	  NULL,
	  getWANDeviceWANCommonInterfaceConfig_PhysicalLinkStatus,
	  NULL,NULL},       
	{TotalBytesSent,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANCommonInterfaceConfig_TotalBytesSent,
	  NULL,NULL},
	{TotalBytesReceived,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANCommonInterfaceConfig_TotalBytesReceived,
	  NULL,NULL},
	{TotalPacketsSent,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANCommonInterfaceConfig_TotalPacketsSent,
	  NULL,NULL},
	{TotalPacketsReceived,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANCommonInterfaceConfig_TotalPacketsReceived,
	  NULL,NULL},
	{NULL}
};
*/
/* InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.Stats. */

/**/
TRxObjNode WANDeviceWANConnectionDeviceWANIPConnectionStatsDesc[] = {

	{EthernetBytesSent,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANIPConnectionStats_EthernetBytesSent,
	  NULL,NULL},
	{EthernetBytesReceived,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANIPConnectionStats_EthernetBytesReceived,
	  NULL,NULL},
	  
	{EthernetDiscardPacketsSent,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANIPConnectionStats_EthernetDiscardPacketsSent,
	  NULL,NULL},

	{EthernetDiscardPacketsReceived,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANIPConnectionStats_EthernetDiscardPacketsReceived,
	  NULL,NULL},

	{EthernetErrorsSent,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANIPConnectionStats_EthernetErrorsSent,
	  NULL,NULL},
	{EthernetErrorsReceived,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANIPConnectionStats_EthernetErrorsReceived,
	  NULL,NULL},

	{EthernetPacketsSent,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANIPConnectionStats_EthernetPacketsSent,
	  NULL,NULL},
	{EthernetPacketsReceived,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANIPConnectionStats_EthernetPacketsReceived,
	  NULL,NULL},

	{X_ATP_EthernetUplinkRate,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANIPConnectionStats_X_ATP_EthernetUplinkRate,
	  NULL,NULL},

	{X_ATP_EthernetDownlinkRate,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANIPConnectionStats_X_ATP_EthernetDownlinkRate,
	  NULL,NULL},
	
	{NULL}
};

/* InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.{i} */
/* InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1. */
/**/
TRxObjNode WANDeviceWANConnectionDeviceWANIPConnectionPortMappingDesc[] = {
	{PortMappingEnabled,{{tBool,0}},
	  setWANDeviceWANConnectionDeviceWANIPConnectionPortMapping_PortMappingEnabled,
	  getWANDeviceWANConnectionDeviceWANIPConnectionPortMapping_PortMappingEnabled,
	  NULL,NULL},
	  
	{RemoteHost,{{tString,REMOTEHOST_LEN}},
	  setWANDeviceWANConnectionDeviceWANIPConnectionPortMapping_RemoteHost,
	  getWANDeviceWANConnectionDeviceWANIPConnectionPortMapping_RemoteHost,
	  NULL,NULL},
	
	{ExternalPort,{{tUnsigned,0,1}},
		  setWANDeviceWANConnectionDeviceWANIPConnection_ExternalPort,
		  getWANDeviceWANConnectionDeviceWANIPConnection_ExternalPort,
		  NULL,NULL},

	{ExternalPortEndRange,{{tUnsigned,0,1}},
		  setWANDeviceWANConnectionDeviceWANIPConnection_ExternalPortEndRange,
		  getWANDeviceWANConnectionDeviceWANIPConnection_ExternalPortEndRange,
		  NULL,NULL},

	{InternalPort,{{tUnsigned,0,1}},
		  setWANDeviceWANConnectionDeviceWANIPConnection_InternalPort,
		  getWANDeviceWANConnectionDeviceWANIPConnection_InternalPort,
		  NULL,NULL},

	{PortMappingProtocol,{{tString,PORTMAPPINGPROTOCOL_LEN}},
	  setWANDeviceWANConnectionDeviceWANIPConnection_PortMappingProtocol,
	  getWANDeviceWANConnectionDeviceWANIPConnection_PortMappingProtocol,
	  NULL,NULL},

	{InternalClient,{{tString,16}},
	  setWANDeviceWANConnectionDeviceWANIPConnectionPortMapping_InternalClient,
	  getWANDeviceWANConnectionDeviceWANIPConnectionPortMapping_InternalClient,
	  NULL,NULL},
	  
	{NULL}
};


TRxObjNode WANDeviceWANConnectionDeviceWANIPConnectionPortMappingInstanceDesc[] = {
	{instanceIDMASK,{{0}},
	  delWANDeviceWANConnectionDeviceWANIPConnectionPortMappingI,
	  addWANDeviceWANConnectionDeviceWANIPConnectionPortMappingI,
	  WANDeviceWANConnectionDeviceWANIPConnectionPortMappingDesc},
};



TRxObjNode WANDeviceWANConnectionDeviceWANIPConnectionX_ATP_CARDesc[]={
	{Enable,{{tBool,0}},
	  setWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_CAREnable,
	  getWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_CAREnable,
	  NULL,NULL},

	{UplinkRate,{{tUnsigned,0,1}},
	  setWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_CARUplinkRate,
	  getWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_CARUplinkRate,
	  NULL,NULL},
	{NULL}
};

TRxObjNode WANDeviceWANConnectionDeviceWANIPConnectionX_ATP_DMZDesc[]={
	{DMZEnable,{{tBool,0}},
	  setWANDeviceWANConnectionDeviceWANIPConnection_DMZEnable,
	  getWANDeviceWANConnectionDeviceWANIPConnection_DMZEnable,
	  NULL,NULL},

	{DMZHostIPAddress,{{tString,DMZHOSTIPADDRESS_LEN}},
	  setWANDeviceWANConnectionDeviceWANIPConnection_DMZHostIPAddress,
	  getWANDeviceWANConnectionDeviceWANIPConnection_DMZHostIPAddress,
	  NULL,NULL},
	{NULL}
};


TRxObjNode WANDeviceWANConnectionDeviceWANIPConnectionX_ATP_WirelessStatusDesc[]={
	{PLMN,{{tString,DMZHOSTIPADDRESS_LEN}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANIPConnection_PLMN,
	  NULL,NULL},

	{ServiceStatus,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANIPConnection_ServiceStatus,
	  NULL,NULL},

	{Bandwidth,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANIPConnection_Bandwidth,
	  NULL,NULL},

	{CellID,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANIPConnection_CellID,
	  NULL,NULL},

	{SignalStrength,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANIPConnection_SignalStrength,
	  NULL,NULL},

	{RSSI,{{tInt,0,0}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANIPConnection_RSSI,
	  NULL,NULL},

	{RSRP,{{tInt,0,0}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANIPConnection_RSRP,
	  NULL,NULL},

	{RSRQ,{{tInt,0,0}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANIPConnection_RSRQ,
	  NULL,NULL},

	{Roam,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANIPConnection_Roam,
	  NULL,NULL},

	{Frequency,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANIPConnection_Frequency,
	  NULL,NULL},
	{NULL}
};

/* InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.*/

TRxObjNode WANDeviceWANConnectionDeviceWANIPConnectionDesc[] = {

	{ConnectionStatus,{{tString,16}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANIPConnection_ConnectionStatus,
	  NULL,NULL},

	{DefaultGateway,{{tString,16}},
	  setWANDeviceWANConnectionDeviceWANIPConnection_DefaultGateway,
	  getWANDeviceWANConnectionDeviceWANIPConnection_DefaultGateway,
	  NULL,NULL},
	  
	{Name,{{tString,16}},
	  setWANDeviceWANConnectionDeviceWANIPConnection_Name,
	  getWANDeviceWANConnectionDeviceWANIPConnection_Name,
	  NULL,NULL},

	{ExternalIPAddress,{{tString,16}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANIPConnection_ExternalIPAddress,
	  NULL,NULL},
	
	{MACAddress,{{tString,64}},
	  setWANDeviceWANConnectionDeviceWANIPConnection_MACAddress, 
	  getWANDeviceWANConnectionDeviceWANIPConnection_MACAddress,
	  NULL,NULL},
	  
	{SubnetMask,{{tString,45}},
	  setWANDeviceWANConnectionDeviceWANIPConnection_SubnetMask,
	  getWANDeviceWANConnectionDeviceWANIPConnection_SubnetMask,
	  NULL,NULL},

	{Stats,{{tObject,0}},
	  NULL,
	  NULL,
	  WANDeviceWANConnectionDeviceWANIPConnectionStatsDesc},
	
#if 0
	
	{Enable,{{tBool,0}},
	  setWANDeviceWANConnectionDeviceWANIPConnection_Enable,
	  getWANDeviceWANConnectionDeviceWANIPConnection_Enable,
	  NULL,NULL},

	{Uptime,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANIPConnection_Uptime,
	  NULL,NULL},

	{LastConnectionError,{{tString,16}},
	  setWANDeviceWANConnectionDeviceWANIPConnection_LastConnectionError,
	  getWANDeviceWANConnectionDeviceWANIPConnection_LastConnectionError,
	  NULL,NULL},

	{NATEnabled,{{tBool,0}},
	  setWANDeviceWANConnectionDeviceWANIPConnection_NATEnabled,
	  getWANDeviceWANConnectionDeviceWANIPConnection_NATEnabled,
	  NULL,NULL},
	
	{ConnectionTrigger,{{tString,16}},
	  setWANDeviceWANConnectionDeviceWANIPConnection_ConnectionTrigger,
	  getWANDeviceWANConnectionDeviceWANIPConnection_ConnectionTrigger,
	  NULL,NULL},
	{PortMappingNumberOfEntries,{{tUnsigned,0,0}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANIPConnection_PortMappingNumberOfEntries,
	  NULL,NULL},

	{MaxMTUSize,{{tUnsigned,0,1}},
	  setWANDeviceWANConnectionDeviceWANIPConnection_MaxMTUSize,
	  getWANDeviceWANConnectionDeviceWANIPConnection_MaxMTUSize,
	  NULL,NULL},

	{X_ATP_APN,{{tString,X_ATP_APN_LEN}},
	  setWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_APN,
	  getWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_APN,
	  NULL,NULL},

	{X_ATP_AuthenticationProtocol,{{tString,X_ATP_AUTHENTICATIONPROTOCOL_LEN}},
	  setWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_AuthenticationProtocol,
	  getWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_AuthenticationProtocol,
	  NULL,NULL},

	{X_ATP_Username,{{tString,X_ATP_USERNAME_LEN}},
	  setWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_Username,
	  getWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_Username,
	  NULL,NULL},

	{X_ATP_DialNumber,{{tString,X_ATP_DIALNUMER_LEN}},
	  setWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_DialNumber,
	  getWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_DialNumber,
	  NULL,NULL},
	  
	{X_ATP_AutoAPN,{{tBool,0}},
	  setWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_AutoAPN,
	  getWANDeviceWANConnectionDeviceWANIPConnection_X_ATP_AutoAPN,
	  NULL,NULL},
	  
	{PortMapping,{{tInstance,0}},
	  NULL,
	  NULL,
	  WANDeviceWANConnectionDeviceWANIPConnectionPortMappingInstanceDesc},

	{X_ATP_CAR,{{tObject,0}},
	  NULL,
	  NULL,
	  WANDeviceWANConnectionDeviceWANIPConnectionX_ATP_CARDesc},

	{X_ATP_DMZ,{{tObject,0}},
	  NULL,
	  NULL,
	  WANDeviceWANConnectionDeviceWANIPConnectionX_ATP_DMZDesc},

	{X_ATP_WirelessStatus,{{tObject,0}},
	  NULL,
	  NULL,
	  WANDeviceWANConnectionDeviceWANIPConnectionX_ATP_WirelessStatusDesc},

	{AddressingType,{{tString,16}},
	  setWANDeviceWANConnectionDeviceWANIPConnection_AddressingType,
	  getWANDeviceWANConnectionDeviceWANIPConnection_AddressingType,
	  NULL,NULL},
	{DNSEnabled,{{tBool,0}},
	  setWANDeviceWANConnectionDeviceWANIPConnection_DNSEnabled,
	  getWANDeviceWANConnectionDeviceWANIPConnection_DNSEnabled,
	  NULL,NULL},

	{DNSServers,{{tString,64}},
	  setWANDeviceWANConnectionDeviceWANIPConnection_DNSServers,
	  getWANDeviceWANConnectionDeviceWANIPConnection_DNSServers,
	  NULL,NULL},

	{Reset,{{tBool,0}},
	  setWANDeviceWANConnectionDeviceWANIPConnection_Reset,
	  getWANDeviceWANConnectionDeviceWANIPConnection_Reset,
	  NULL,NULL},
	
	{PossibleConnectionTypes,{{tString,16}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANIPConnection_PossibleConnectionTypes,
	  NULL,NULL},
	{ConnectionType,{{tString,16}},
	  setWANDeviceWANConnectionDeviceWANIPConnection_ConnectionType,  
	  getWANDeviceWANConnectionDeviceWANIPConnection_ConnectionType,
	  NULL,NULL},
	
	{RSIPAvailable,{{tBool,0}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANIPConnection_RSIPAvailable,
	  NULL,NULL},	
	
	{DNSOverrideAllowed,{{tBool,0}},
	  setWANDeviceWANConnectionDeviceWANIPConnection_DNSOverrideAllowed,
	  getWANDeviceWANConnectionDeviceWANIPConnection_DNSOverrideAllowed,
	  NULL,NULL},
	
	{RouteProtocolRx,{{tString,16}},
	  setWANDeviceWANCnnectionDeviceWANIPConnection_RouteProtocolRx,
	  getWANDeviceWANConnectionDeviceWANIPConnection_RouteProtocolRx,
	  NULL,NULL},
#endif  
	
	{NULL}
};

TRxObjNode WANDeviceWANConnectionDeviceWANPPPConnectionX_ATP_DMZDesc[]={
	{DMZEnable,{{tselect,0}},
	  setWANDeviceWANConnectionDeviceWANPPPConnection_DMZEnable,
	  getWANDeviceWANConnectionDeviceWANPPPConnection_DMZEnable,
	  NULL,NULL},
	{DMZHostIPAddress,{{tString,DMZHOSTIPADDRESS_LEN}},
	  setWANDeviceWANConnectionDeviceWANPPPConnection_DMZHostIPAddress,
	  getWANDeviceWANConnectionDeviceWANPPPConnection_DMZHostIPAddress,
	  NULL,NULL},
	{NULL}
};
TRxObjNode WANDeviceWANConnectionDeviceWANPPPConnectionPortMappingDesc[]={
  {NULL}
};

TRxObjNode WANDeviceWANConnectionDeviceWANPPPConnectionStatsDesc[] = {

	{EthernetBytesSent,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANPPPConnectionStats_EthernetBytesSent,
	  NULL,NULL},
	{EthernetBytesReceived,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANPPPConnectionStats_EthernetBytesReceived,
	  NULL,NULL},
	  
	{EthernetDiscardPacketsSent,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANPPPConnectionStats_EthernetDiscardPacketsSent,
	  NULL,NULL},

	{EthernetDiscardPacketsReceived,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANPPPConnectionStats_EthernetDiscardPacketsRec,
	  NULL,NULL},

	{EthernetErrorsSent,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANPPPConnectionStats_EthernetErrorsSent,
	  NULL,NULL},
	{EthernetErrorsReceived,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANPPPConnectionStats_EthernetErrorsReceived,
	  NULL,NULL},

	{EthernetPacketsSent,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANPPPConnectionStats_EthernetPacketsSent,
	  NULL,NULL},
	{EthernetPacketsReceived,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANPPPConnectionStats_EthernetPacketsReceived,
	  NULL,NULL},

	{X_ATP_EthernetUplinkRate,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANPPPConnectionStats_X_ATP_EthernetUplinkRate,
	  NULL,NULL},

	{X_ATP_EthernetDownlinkRate,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANPPPConnectionStats_X_ATP_EthernetDownlinkRate,
	  NULL,NULL},
	
	{NULL}
};
TRxObjNode WANDeviceWANConnectionDeviceWANIPConnectionInstanceDesc[] = {
	{instanceIDMASK,{{0}},
	  delWANDeviceWANConnectionDeviceWANIPConnectionI,
	  addWANDeviceWANConnectionDeviceWANIPConnectionI,
	  WANDeviceWANConnectionDeviceWANIPConnectionDesc},
};

/* InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.WANPPPConnection.{i}. */
TRxObjNode  WANDeviceWANConnectionDeviceWANPPPConnectionDesc[] = {
	{Enable,{{tselect,0}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANPPPConnection_Enable,
	  NULL,NULL},
	{Name,{{tString,128}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANPPPConnection_Name,
	  NULL,NULL},
	{Uptime,{{tUnsigned,0}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANPPPConnection_Uptime,
	  NULL,NULL},
	{LastConnectionError,{{tString,1024}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANPPPConnection_LastConnectionError,
	  NULL,NULL},
	{NATEnabled,{{tselect,0}},
	  setWANDeviceWANConnectionDeviceWANPPPConnection_NATEnabled,
	  getWANDeviceWANConnectionDeviceWANPPPConnection_NATEnabled,
	  NULL,NULL},
	{ExternalIPAddress,{{tString,32}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANPPPConnection_ExternalIPAddress,
	  NULL,NULL},
	{MaxMRUSize,{{tUnsigned,0}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANPPPConnection_MaxMRUSize,
	  NULL,NULL},
	{MAXMTUSize,{{tUnsigned,0}},
	  setWANDeviceWANConnectionDeviceWANPPPConnection_MAXMTUSize,
	  getWANDeviceWANConnectionDeviceWANPPPConnection_MAXMTUSize,
	  NULL,NULL},
	{DNSServers,{{tString,64}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANPPPConnection_DNSServers,
	  NULL,NULL},
	{ConnectionTrigger,{{tString,64}},
	  NULL,
	  getWANDevWANConnectDevWANPPPConnect_ConnectTrigger,
	  NULL,NULL},
	{X_ATP_ServiceList,{{tString,64}},
	  NULL,
	  getWANDevWANConnectDevWANPPPConnect_X_ATP_ServiceList,
	  NULL,NULL},
	{X_ATP_NATType,{{tString,64}},
	  NULL,
	  getWANDevWANConnectDevWANPPPConnect_X_ATP_NATType,
	  NULL,NULL},
	{ConnectionStatus,{{tString,64}},
	  NULL,
	  getWANDevWANConnectDevWANPPPConnect_ConnectionStatus,
	  NULL,NULL},
	{DefaultGateway,{{tString,64}},
	  NULL,
	  getWANDevWANConnectDevWANPPPConnect_DefaultGateway,
	  NULL,NULL},
	{X_ATP_DMZ,{{tObject,0}},
	  NULL,
	  NULL,
	  WANDeviceWANConnectionDeviceWANPPPConnectionX_ATP_DMZDesc},
	{PortMapping,{{tObject,0}},
	  NULL,
	  NULL,
	  WANDeviceWANConnectionDeviceWANPPPConnectionPortMappingDesc},
	 {Stats,{{tObject,0}},
	  NULL,
	  NULL,
	  WANDeviceWANConnectionDeviceWANPPPConnectionStatsDesc},
#if 0
	{Enable,{{tBool,0}},
	  setWANDeviceWANConnectionDeviceWANPPPConnection_Enable,
	  getWANDeviceWANConnectionDeviceWANPPPConnection_Enable,
	  NULL,NULL},
	{Name,{{tString,128}},
	  setWANDeviceWANConnectionDeviceWANPPPConnection_Name,
	  getWANDeviceWANConnectionDeviceWANPPPConnection_Name,
	  NULL,NULL},
	  
	{Uptime,{{tUnsigned,0}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANPPPConnection_Uptime,
	  NULL,NULL},

	{LastConnectionError,{{tString,1024}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANPPPConnection_LastConnectionError,
	  NULL,NULL},
	  
	{Username,{{tString,64}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANPPPConnection_Username,
	  NULL,NULL},
	  
	{Password,{{tString,64}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANPPPConnection_Password,
	  NULL,NULL},
	  
	{ExternalIPAddress,{{tString,0}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANPPPConnection_ExternalIPAddress,
	  NULL,NULL},
	  
	{DNSEnabled,{{tBool,0}},
	  setWANDeviceWANConnectionDeviceWANPPPConnection_DNSEnabled,
	  getWANDeviceWANConnectionDeviceWANPPPConnection_DNSEnabled,
	  NULL,NULL},

	{DNSServers,{{tString,64}},
	  setWANDeviceWANConnectionDeviceWANPPPConnection_DNSServers,
	  getWANDeviceWANConnectionDeviceWANPPPConnection_DNSServers,
	  NULL,NULL},
	  
	{MACAddress,{{tString,64}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANPPPConnection_MACAddress,
	  NULL,NULL},
  
	{X_CMCC_TunnelDial,{{tBool,0}},
	  setWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_TunnelDial,
	  getWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_TunnelDial,
	  NULL,NULL},

	{X_CMCC_LNS,{{tBool,0}},
	  setWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_LNS,
	  getWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_LNS,
	  NULL,NULL},
	  
	{X_CMCC_LnsIp,{{tString,64}},
	  setWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_LnsIp,
	  getWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_LnsIp,
	  NULL,NULL},

	{X_CMCC_LnsName,{{tString,64}},
	  setWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_LnsName,
	  getWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_LnsName,
	  NULL,NULL},

	{X_CMCC_LnsPassword,{{tString,64}},
	  setWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_LnsPassword,
	  getWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_LnsPassword,
	  NULL,NULL},

	{X_CMCC_L2TPMode,{{tUnsigned,0}},
	  setWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_L2TPMode,
	  getWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_L2TPMode,
	  NULL,NULL},
	{X_CMCC_UpstreamTotalByte,{{tUnsigned,0}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANPPPConnection_X_CMCC_UpstreamTotalByte,
	  NULL,NULL},
	{X_CMCC_DownstreamTotalByte,{{tUnsigned,0}},
	  NULL,
	  getWANDeviceWANConnectionDeviceWANPPPConnection_X_X_CMCC_DownstreamTotalByte,
	  NULL,NULL},
  #endif

    {NULL}
};

/* InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.WANPPPConnection. */
TRxObjNode  WANDeviceWANConnectionDeviceWANPPPConnectionInstanceDesc[] = {
    {instanceIDMASK,{{0}},
	   deletePPPConnection,
	   addPPPConnection,
	   WANDeviceWANConnectionDeviceWANPPPConnectionDesc},
	       {NULL}
};



TRxObjNode WANX_ATP_APNDesc[]={
	{APN,{{tString,WANAPN_LEN}},
	  setWANAPN,
	  getWANAPN,
	  NULL,NULL},

	{AuthenticationProtocol,{{tString,WANAPN_LEN}},
	  setWANAuthenticationProtocol,
	  getWANAuthenticationProtocol,
	  NULL,NULL},

	{Username,{{tString,WANAPN_LEN}},
	  setWANUsername,
	  getWANUsername,
	  NULL,NULL},

	{Password,{{tString,WANAPN_LEN}},
	  setWANPassword,
	  getWANPassword,
	  NULL,NULL},

	{DialNumber,{{tString,WANAPN_LEN}},
	  setWANDialNumber,
	  getWANDialNumber,
	  NULL,NULL},

	{AutoAPN,{{tBool,0,0}}, 
	 NULL,
	 getWANAutoAPN,
	NULL,NULL},
	
	{NULL}
};

TRxObjNode  WANX_ATP_APNDescObj[] = {
    {instanceIDMASK,{{0}},
	   NULL,
	   NULL,
	   WANX_ATP_APNDesc},
	       {NULL}
};

/* InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}. */
TRxObjNode  wanConnectionDeviceDesc[] = {

  	{WANIPConnection,{{tInstance,0}},
	  NULL,
	  NULL,
	  WANDeviceWANConnectionDeviceWANIPConnectionInstanceDesc},
	  
	{WANPPPConnectionNumberOfEntries,{{tUnsigned}}, 
        NULL,
        getPPPConNumberEntries,
        NULL,NULL}, 
     {WANPPPConnection,{{tInstance}},
	  NULL,
	  NULL,
	  WANDeviceWANConnectionDeviceWANPPPConnectionInstanceDesc},
#if 0	
	{X_ATP_APN,{{tObject,0,0}}, 
		NULL,
		NULL,
		WANX_ATP_APNDescObj,NULL},

	{WANIPConnectionNumberOfEntries,{{tUnsigned}}, 
        NULL,
        getWANDeviceWANConnectionDevice_WANIPConnectionNumberOfEntries,
        NULL,NULL},
   	
   
#endif
	 {NULL}
};

TRxObjNode  WANDeviceWANConnectionDeviceInstanceDesc[] = {
    {instanceIDMASK,{{0}},
	   deleteWANConnectionDevice,
	  addWANConnectionDevice,
	   wanConnectionDeviceDesc},
	       {NULL}
};



TRxObjNode  WANEthernetInterfaceConfigDesc[] ={
{MACAddress,{{tString,16}},
	NULL,
	getLANDeviceWANEthernetInterfaceConfig_MACAddress,
	NULL,NULL},

{NULL}
};


TRxObjNode  WANDeviceX_ATP_WANNetConfigInfoInstanceDesc[] ={

	{RSSI,{{tInt,0,0}},
	  NULL,
	  getWANDeviceX_ATP_WANNetConfigInfo_RSSI,
	  NULL,NULL},

	{RSRP,{{tInt,0,0}},
	  NULL,
	  getWANDeviceX_ATP_WANNetConfigInfo_RSRP,
	  NULL,NULL},

	{RSRQ,{{tInt,0,0}},
	  NULL,
	  getWANDeviceX_ATP_WANNetConfigInfo_RSRQ,
	  NULL,NULL},
	
	{SINR,{{tInt,0,0}},
	  NULL,
	  getWANDeviceX_ATP_WANNetConfigInfo_SINR,
	  NULL,NULL},
	{CellID,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceX_ATP_WANNetConfigInfo_CellID,
	  NULL,NULL},
	{PLMN,{{tString,64}},
	  NULL,
	  getWANDeviceX_ATP_WANNetConfigInfo_PLMN,
	  NULL,NULL},
	{ServiceStatus,{{tString,64}},
	  NULL,
	  getWANDevX_ATP_WANNetConfigInfo_SerStatus,
	  NULL,NULL},
	{SignalQuality,{{tInt,0,0}},
	  NULL,
	  getWANDeviceX_ATP_WANNetConfigInfo_SignalQuality,
	  NULL,NULL},
	{RoamStatus,{{tString,64}},
	  NULL,
	  getWANDevX_ATP_WANNetConfigInfo_RoamStatus,
	  NULL,NULL},
	{PhysicalCellID,{{tUnsigned,0,1}},
	  NULL,
	  getWANDeviceX_ATP_WANNetConfigInfo_PhysicalCellID,
	  NULL,NULL},
	{NULL}
};
/*
	add by duanguiyuan 20161125 
	添加lan口状态 告警上报参数 节点

 .LanCheck
 .RSRPWeek
 .TempAmplifierHigh
 .TempAmplifierLow
 .
 .TempModemLow
 .
 .
*/
/* InternetGatewayDevice.WANDevice.{i}.X_ATP_AlarmInstance */
TRxObjNode  WANDeviceX_ATP_AlarmInstanceInstanceDesc[] ={

	{LAN1,{{tString,512}},
	  NULL,
	  getWANDeviceX_ATP_Alarm_LAN1,
	  NULL,NULL},

	{LAN2,{{tString,512}},
	  NULL,
	  getWANDeviceX_ATP_Alarm_LAN2,
	  NULL,NULL},

	{LAN3,{{tString,512}},
	  NULL,
	  getWANDeviceX_ATP_Alarm_LAN3,
	  NULL,NULL},
	
	{LAN4,{{tString,512}},
	  NULL,
	  getWANDeviceX_ATP_Alarm_LAN4,
	  NULL,NULL},
	  
	{LanCheck,{{tString,512}},
	  NULL,
	  getWANDeviceX_ATP_Alarm_LanCheck,
	  NULL,NULL},

	{RSRPWeek,{{tString,512}},
	  NULL,
	  getWANDeviceX_ATP_Alarm_RSRPWeek,
	  NULL,NULL},
	 
	{TempAmplifierHigh,{{tString,512}},
	  NULL,
	  getWANDeviceX_ATP_Alarm_TempAmplifierHigh,
	  NULL,NULL},
	  
	{TempAmplifierLow,{{tString,512}},
	  NULL,
	  getWANDeviceX_ATP_Alarm_TempAmplifierLow,
	  NULL,NULL},
	  
	{TempModemHigh,{{tString,512}},
	  NULL,
	  getWANDeviceX_ATP_Alarm_TempModemHigh,
	  NULL,NULL},
	  
	{TempModemLow,{{tString,512}},
	  NULL,
	  getWANDeviceX_ATP_Alarm_TempModemLow,
	  NULL,NULL},
	  
	{TempModemHigh,{{tString,512}},
	  NULL,
	  getWANDeviceX_ATP_Alarm_TempModemHigh,
	  NULL,NULL},

	{WanCheck,{{tString,512}},
	  NULL,
	  getWANDeviceX_ATP_Alarm_WanCheck,
	  NULL,NULL},

	{WebLogin,{{tString,512}},
	  NULL,
	  getWANDeviceX_ATP_Alarm_WebLogin,
	  NULL,NULL},
	  
	{NULL}
};

/* InternetGatewayDevice.WANDevice.{i}. */
TRxObjNode WANDeviceDesc[] = {

    {WANConnectionDevice,{{tInstance,0}},
	  NULL,
	  NULL,
	  WANDeviceWANConnectionDeviceInstanceDesc},
	  
	{X_ATP_WANNetConfigInfo,{{tInstance,0}},
	  NULL,
	  NULL,
	  WANDeviceX_ATP_WANNetConfigInfoInstanceDesc},
	  //X_ATP_AlarmInstance
	  //WANDeviceX_ATP_AlarmInstanceInstanceDesc
	{X_ATP_AlarmInstance,{{tInstance,0}},
	  NULL,
	  NULL,
	  WANDeviceX_ATP_AlarmInstanceInstanceDesc},

	{WANConnectionNumberOfEntries,{{tUnsigned,0}},
	  NULL,
	  getWANDevice_WANConnectionNumberOfEntries,
	  NULL,NULL},
#if 0	
	

	{WANCommonInterfaceConfig,{{tObject,0}},
	  NULL,
	  NULL,
	  WANDeviceWANCommonInterfaceConfigDesc},
  
	{WANEthernetInterfaceConfig,{{tObject,0}},
	  NULL,
	  NULL,
	 WANEthernetInterfaceConfigDesc},

#endif
	      {NULL}
};


/** InternetGatewayDevice.WANDevice. */
TRxObjNode WANDeviceInstanceDesc[] = {
	{instanceIDMASK,{{0}},
	  NULL,
	  NULL,
	  WANDeviceDesc},
};





/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

