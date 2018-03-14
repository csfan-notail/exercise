/*
 * Copyright (C) GBCOM ltd.,co 2015
 *
 * This is a part of project tr69cd, which implements TR069 protocal(aka
 * CWMP).
 * 
 * This is a PRIVATE software.
 * All rights reserved.
 */


#include "tr69cdefs.h"
#include "commonString.h"
#include "rootParams.h"

#define IDMASK_LEN 32
extern TRxObjNode  deviceInfoDesc[]; 
/*altered by caibing*/
//extern TRxObjNode  userInfoDesc[]; 


extern TRxObjNode DeviceConfigDesc[];
extern TRxObjNode X_ATP_EXTDesc[];
extern TRxObjNode Layer3ForwardingDesc[];
extern TRxObjNode X_ATP_SystemMonitorDesc[];
extern TRxObjNode X_ATP_ModemLogDesc[];
extern TRxObjNode X_ATP_PreserveDataDesc[];
extern TRxObjNode GatewayUserInterfaceDesc[];
extern TRxObjNode X_ATP_FireWallDesc[];



extern TRxObjNode  x_deviceInfoDesc[]; 
extern TRxObjNode  mgmtServerDesc[];
extern TRxObjNode  TimeDesc[];
extern TRxObjNode  LANConfigSecurityDesc[];
extern TRxObjNode  LANDeviceInstanceDesc[];
extern TRxObjNode  WANDeviceInstanceDesc[];
extern TRxObjNode  IPPingDiagnosticsDesc[];
extern TRxObjNode  TraceRouteDiagnosticsDesc[];
extern TRxObjNode  x_lteDeviceDesc[];
extern TRxObjNode  x_PPPoE2L2TPDesc[];
//altered by caibing 11-15
//extern TRxObjNode  x_CMCC_PPPOE_EmulatorDesc[];
extern TRxObjNode  FreqLockDesc[];
extern TRxObjNode  X_ATP_VxlanDesc[];
extern TRxObjNode  X_ATP_VPNDesc[];
extern TRxObjNode  X_ATP_L2GreDesc[];
extern TRxObjNode  X_ATP_VPNDesc[];
extern TRxObjNode  X_ATP_AlarmInfoDesc[];
extern TRxObjNode  X_ATP_ConfigDesc[];
extern TRxObjNode  ServicesDesc[];


const char instanceIDMASK[]="##";

#if 0
/* InternetGatewayDevice. */
TRxObjNode  internetGatewayDeviceDesc[] = {
    {DeviceSummary,{{tString,1024,0}},NULL,getDeviceSummary,NULL,NULL}, 
    {LANDeviceNumberOfEntries,{{tUnsigned,0,0}}, NULL,getWANLANDeviceNumEntries,NULL,NULL},
    {WANDeviceNumberOfEntries,{{tUnsigned,0,0}}, NULL,getWANLANDeviceNumEntries,NULL,NULL},

/*add by duanguiyuan 20150915  暂时屏蔽*/
   /* {X_CMCC_WANDeviceNumberOfEntries,{{tUnsigned,0,0}}, NULL,getX_CMCC_WANDeviceNumberOfEntries,NULL,NULL},
    {X_CMCC_NetRegStatus,{{tUnsigned,0,0}}, NULL,getX_CMCC_NetRegStatus,NULL,NULL},
    {X_CMCC_GprsRegStatus,{{tUnsigned,0,0}}, NULL,getX_CMCC_GprsRegStatus,NULL,NULL},
    {X_CMCC_EpsRegStatus,{{tUnsigned,0,0}}, NULL,getX_CMCC_EpsRegStatus,NULL,NULL},
    {X_CMCC_CurrentNetwork,{{tUnsigned,0,0}}, NULL,getX_CMCC_CurrentNetwork,NULL,NULL},
    {X_CMCC_CurrentNetmode,{{tUnsigned,0,0}}, NULL,getX_CMCC_CurrentNetmode,NULL,NULL},
    {X_CMCC_NetworkPriority,{{tString,32,0}}, NULL,getX_CMCC_NetworkPriority,NULL,NULL},
	{X_CMCC_SingalLevel,{{tUnsigned,0,0}}, NULL,getX_CMCC_SingalLevel,NULL,NULL},
    {X_CMCC_Txpower,{{tUnsigned,0,0}}, NULL,getX_CMCC_Txpower,NULL,NULL},
    {X_CMCC_FrequencyPoint,{{tUnsigned,0,0}}, NULL,getX_CMCC_FrequencyPoint,NULL,NULL},
    {X_CMCC_Band,{{tUnsigned,0,0}}, NULL,getX_CMCC_Band,NULL,NULL},
    {X_CMCC_BandWidth,{{tUnsigned,0,0}}, NULL,getX_CMCC_BandWidth,NULL,NULL},
    {X_CMCC_EnodeBId,{{tUnsigned,0,0}}, NULL,getX_CMCC_EnodeBId,NULL,NULL},
    {X_CMCC_CellId,{{tUnsigned,0,0}}, NULL,getX_CMCC_CellId,NULL,NULL},
    {X_CMCC_GlobeCellId,{{tUnsigned,0,0}}, NULL,getX_CMCC_GlobeCellId,NULL,NULL},
    {X_CMCC_PhysicsCellId,{{tUnsigned,0,0}}, NULL,getX_CMCC_PhysicsCellId,NULL,NULL},
    {X_CMCC_ICCID,{{tString,64,0}}, NULL,getX_CMCC_ICCID,NULL,NULL},
    {X_CMCC_APN,{{tString,64,0}}, NULL,getX_CMCC_APN,NULL,NULL},
	{X_CMCC_IsFrequencyLock,{{tBool,0,0}}, setX_CMCC_IsFrequencyLock,getX_CMCC_IsFrequencyLock,NULL,NULL},
 	{X_CMCC_FrequencyLocking,{{tString,512,0}}, setX_CMCC_FrequencyLocking,getX_CMCC_FrequencyLocking,NULL,NULL},
	{X_CMCC_FrequencyLockInfo,{{tString,512,0}}, NULL,getX_CMCC_FrequencyLockInfo,NULL,NULL},
	{X_CMCC_IsCellLock,{{tBool,0,0}}, setX_CMCC_IsCellLock,getX_CMCC_IsCellLock,NULL,NULL},
	{X_CMCC_CellLockType,{{tUnsigned,0,0}}, setX_CMCC_CellLockType,getX_CMCC_CellLockType,NULL,NULL},
	{X_CMCC_CellLocking,{{tString,512,0}}, NULL,getX_CMCC_CellLocking,NULL,NULL},
	{X_CMCC_CellLockInfo,{{tString,512,0}}, NULL,getX_CMCC_CellLockInfo,NULL,NULL},
	{X_CMCC_LockPin,{{tBool,0,0}}, setX_CMCC_LockPin,getX_CMCC_LockPin,NULL,NULL},
    {X_CMCC_LockPinType,{{tUnsigned,0,0}}, setX_CMCC_LockPinType,getX_CMCC_LockPinType,NULL,NULL},
	{X_CMCC_FirstPin,{{tString,64,0}}, NULL,getX_CMCC_FirstPin,NULL,NULL},
	{X_CMCC_FixedPin,{{tString,64,0}}, NULL,getX_CMCC_FixedPin,NULL,NULL},
    {X_CMCC_EncryptCard,{{tBool,0,0}}, setX_CMCC_EncryptCard,getX_CMCC_EncryptCard,NULL,NULL},	
	{X_CMCC_EncryptCardKey,{{tString,64,0}}, setX_CMCC_EncryptCardKey,getX_CMCC_EncryptCardKey,NULL,NULL},
    {X_CMCC_RSRP,{{tUnsigned,0,0}}, NULL,getX_CMCC_RSRP,NULL,NULL},
    {X_CMCC_RSRQ,{{tUnsigned,0,0}}, NULL,getX_CMCC_RSRQ,NULL,NULL},
    {X_CMCC_SINR,{{tUnsigned,0,0}}, NULL,getX_CMCC_SINR,NULL,NULL},
    {X_CMCC_UpstreamTotalByte,{{tUnsigned,0,0}}, NULL,getX_CMCC_UpstreamTotalByte,NULL,NULL},
    {X_CMCC_DownstreamTotalByte,{{tUnsigned,0,0}}, NULL,getX_CMCC_DownstreamTotalByte,NULL,NULL},
    {X_CMCC_StartTime,{{tDateTime,0,0}}, NULL,getX_CMCC_StartTime,NULL,NULL},
	{X_CMCC_LoadAverage,{{tString,64,0}}, NULL,getX_CMCC_LoadAverage,NULL,NULL},*/
	//{X_CMCC_DeviceMemory,{{tString,64,0}}, NULL,getX_CMCC_DeviceMemory,NULL,NULL},
	//{X_CMCC_MTU,{{tString,64,0}}, setX_CMCC_MTU,getX_CMCC_MTU,NULL,NULL},

/*add by duanguiyuan  */	
  //  {X_DeviceInfo,{{tObject,0,0}}, NULL,NULL,x_deviceInfoDesc,NULL},
  	{DeviceInfo,{{tObject,0,0}}, NULL,NULL, deviceInfoDesc,NULL},
  	{Time,{{tObject,0,0}}, NULL,NULL, TimeDesc,NULL},
    {LANDevice,{{tInstance,0,0}}, NULL,NULL, LANDeviceInstanceDesc,NULL},
    {WANDevice,{{tInstance,0,0}}, NULL,NULL, WANDeviceInstanceDesc,NULL}, 
 	{ManagementServer,{{tObject,0,0}}, NULL,NULL, mgmtServerDesc,NULL},  
  	{IPPingDiagnostics,{{tObject,0,0}}, NULL,NULL, IPPingDiagnosticsDesc,NULL},  
  	{X_ATP_SystemMonitor,{{tObject,0,0}}, NULL,NULL, X_ATP_SystemMonitorDesc,NULL},

	{FreqLock,{{tObject,0,0}}, NULL,NULL, FreqLockDesc,NULL},
	{X_ATP_Vxlan,{{tObject,0,0}}, NULL,NULL, X_ATP_VxlanDesc,NULL},
	{X_ATP_L2Gre,{{tObject,0,0}}, NULL,NULL, X_ATP_L2GreDesc,NULL},
	{X_ATP_EXT,{{tObject,0,0}}, NULL,NULL, X_ATP_EXTDesc,NULL},
	{Layer3Forwarding,{{tObject,0,0}}, NULL,NULL, Layer3ForwardingDesc,NULL},
	{X_ATP_FireWall,{{tObject,0,0}}, NULL,NULL, X_ATP_FireWallDesc,NULL},
	{X_ATP_VPN,{{tObject,0,0}}, NULL,NULL, X_ATP_VPNDesc,NULL},
	{X_ATP_AlarmInfo,{{tObject,0,0}}, NULL,NULL,X_ATP_AlarmInfoDesc,NULL},
	{X_ATP_Config,{{tObject,0,0}}, NULL,NULL,X_ATP_ConfigDesc,NULL},
	{Services,{{tObject,0,0}}, NULL,NULL,ServicesDesc,NULL},
	
#if 0
  	{LANConfigSecurity,{{tObject,0,0}}, NULL,NULL, LANConfigSecurityDesc,NULL},
    {X_LteDevice,{{tObject,0,0}}, NULL,NULL, x_lteDeviceDesc,NULL},
    {X_PPPoE2L2TP,{{tObject,0,0}}, NULL,NULL, x_PPPoE2L2TPDesc,NULL},
	/*	altered by caibing 11-15	*/
   	{X_CMCC_UserInfo,{{tObject,0,0}}, NULL,NULL, userInfoDesc,NULL},
    {WANDevice,{{tInstance,0,0}}, NULL,NULL, WANDeviceInstanceDesc,NULL},    
    {TraceRouteDiagnostics,{{tObject,0,0}}, NULL,NULL, TraceRouteDiagnosticsDesc,NULL},
	/*	altered by caibing 11-15	*/
	{X_CMCC_PPPOE_EMULATOR,{{tObject,0,0}}, NULL,NULL, x_CMCC_PPPOE_EmulatorDesc,NULL},
 	{DeviceInfo,{{tObject,0,0}}, NULL,NULL, deviceInfoDesc,NULL},
	/*----------------add by wangminglin-----------------------*/
 	{DeviceConfig,{{tObject,0,0}}, NULL,NULL, DeviceConfigDesc,NULL},
 	
 	
 	{X_ATP_ModemLog,{{tObject,0,0}}, NULL,NULL, X_ATP_ModemLogDesc,NULL},
	{X_ATP_PreserveData,{{tObject,0,0}}, NULL,NULL, X_ATP_PreserveDataDesc,NULL},
	{UserInterface,{{tObject,0,0}}, NULL,NULL, GatewayUserInterfaceDesc,NULL},
	
 #endif
    {NULL}
};
#endif
TRxObjNode  internetGatewayDeviceDesc[] = {
    {DeviceSummary,{{tString,1024,0}},NULL,getDeviceSummary,NULL,NULL}, 
    {LANDeviceNumberOfEntries,{{tUnsigned,0,0}}, NULL,getWANLANDeviceNumEntries,NULL,NULL},
    {WANDeviceNumberOfEntries,{{tUnsigned,0,0}}, NULL,getWANLANDeviceNumEntries,NULL,NULL},
  	{DeviceInfo,{{tObject,0,0}}, NULL,NULL, deviceInfoDesc,NULL},
  	{Time,{{tObject,0,0}}, NULL,NULL, TimeDesc,NULL},
    {LANDevice,{{tInstance,0,0}}, NULL,NULL, LANDeviceInstanceDesc,NULL},
    {WANDevice,{{tInstance,0,0}}, NULL,NULL, WANDeviceInstanceDesc,NULL}, 
 	{ManagementServer,{{tObject,0,0}}, NULL,NULL, mgmtServerDesc,NULL},  
  	{IPPingDiagnostics,{{tObject,0,0}}, NULL,NULL, IPPingDiagnosticsDesc,NULL},  
  	{X_ATP_SystemMonitor,{{tObject,0,0}}, NULL,NULL, X_ATP_SystemMonitorDesc,NULL},
	{FreqLock,{{tObject,0,0}}, NULL,NULL, FreqLockDesc,NULL},
	{X_ATP_Vxlan,{{tObject,0,0}}, NULL,NULL, X_ATP_VxlanDesc,NULL},
	{X_ATP_L2Gre,{{tObject,0,0}}, NULL,NULL, X_ATP_L2GreDesc,NULL},
	{X_ATP_EXT,{{tObject,0,0}}, NULL,NULL, X_ATP_EXTDesc,NULL},
	{Layer3Forwarding,{{tObject,0,0}}, NULL,NULL, Layer3ForwardingDesc,NULL},
	{X_ATP_FireWall,{{tObject,0,0}}, NULL,NULL, X_ATP_FireWallDesc,NULL},
	{X_ATP_VPN,{{tObject,0,0}}, NULL,NULL, X_ATP_VPNDesc,NULL},
	{X_ATP_AlarmInfo,{{tObject,0,0}}, NULL,NULL,X_ATP_AlarmInfoDesc,NULL},
	{X_ATP_Config,{{tObject,0,0}}, NULL,NULL,X_ATP_ConfigDesc,NULL},
	{Services,{{tObject,0,0}}, NULL,NULL,ServicesDesc,NULL},
    {NULL}
};

/* . */
TRxObjNode  rootDevice[] = {
    {InternetGatewayDevice,{{tObject,0,0}}, NULL,NULL, internetGatewayDeviceDesc,NULL},
    {NULL}
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

