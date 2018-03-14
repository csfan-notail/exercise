/*
 * Copyright (C) GBCOM ltd.,co 2015
 *
 * This is a part of project tr69cd, which implements TR069 protocal(aka
 * CWMP).
 * 
 * This is a PRIVATE software.
 * All rights reserved.
 */

#include "deviceInfoParams.h"


#define IMEI_LEN 64
#define SOFTWAREVERSION_LEN 64
#define MODEMPRODUCTNAME_LEN 64 
#define BOMCODE_LEN 64
#define DESTIPADDRESS_LEN 64
#define DESTSUBNETMASK_LEN 64
#define GATEWAYIPADDRESS_LEN 64
#define INTERFACE_LEN 256
#define LANCHECKHOST_LEN 64
#define WANCHECKHOST_LEN 64
#define PARAMETERNAME_LEN 256
#define X_ATP_WEBUSERNAME_LEN 256
#define X_ATP_WEBUSERPASSWORD_LEN 256
#define CURRENTLEVEL_LEN 64
#define FILTERMODELEVEL_LEN 64
#define URL_LEN 64
#define SOURCEMACADDRESS_LEN 64
#define STARTSOURCEIP_LEN 64
#define ENDSOURCEIP_LEN 64
#define STARTDESTIP_LEN 64
#define ENDDESTIP_LEN 64
#define PROTOCOL_LEN 64
#define SERVICE_LEN 64
#define DIRECTION_LEN 64
#define STARTIPADDR_LEN 64
#define ENDIPADDR_LEN 64

TRxObjNode DeviceInfoX_CMCC_MonitorMonitorConfigDesc[]={
    {ParaList,{{tString,256,0}}, setDeviceInfoX_CMCC_MonitorMonitorConfigParaList,getDeviceInfoX_CMCC_MonitorMonitorConfigParaList,NULL,NULL},
	{TimeList,{{tString,16,0}}, setDeviceInfoX_CMCC_MonitorMonitorConfigTimeList,getDeviceInfoX_CMCC_MonitorMonitorConfigTimeList,NULL,NULL},
	{NULL}
};
TRxObjNode  DeviceInfoX_CMCC_MonitorMonitorConfig[] = {
    {instanceIDMASK,{{0}},
		addMonitorConfig,
	   deleteMonitorConfig,
	   DeviceInfoX_CMCC_MonitorMonitorConfigDesc}  
};

TRxObjNode	DeviceInfoX_CMCC_TeleComAccountDesc[]={
    {Enable,{{tBool,0,0}}, setDeviceInfoX_CMCC_TeleComAccountEnable,getDeviceInfoX_CMCC_TeleComAccountEnable,NULL,NULL},
    {Username,{{tString,16,0}}, setDeviceInfoX_CMCC_TeleComAccountUsername,getDeviceInfoX_CMCC_TeleComAccountUsername,NULL,NULL},
    {Password,{{tString,16,0}}, setDeviceInfoX_CMCC_TeleComAccountPassword,getDeviceInfoX_CMCC_TeleComAccountPassword,NULL,NULL},

	{NULL}
};

TRxObjNode	DeviceInfoX_CMCC_MonitorDesc[]={
    {Enable,{{tBool,0,0}}, setDeviceInfoX_CMCC_MonitorDescEnable,getDeviceInfoX_CMCC_MonitorDescEnable,NULL,NULL},
    {MonitorNumberOfEntries,{{tUnsigned,0,0}}, NULL,getDeviceInfoX_CMCC_MonitorDescMonitorNumberOfEntries,NULL,NULL},
 	{MonitorConfig,{{tInstance,0}},
	  NULL,
	  NULL,
	  DeviceInfoX_CMCC_MonitorMonitorConfig},
//MonitorConfig
	{NULL}
};


TRxObjNode	DeviceInfoX_CMCC_AlarmDesc[]={
    {AlarmNumber,{{tString,256,0}}, setDeviceInfoX_CMCC_AlarmAlarmNumber,getDeviceInfoX_CMCC_AlarmAlarmNumber,NULL,NULL},
   {NULL}
};

TRxObjNode	DeviceInfoX_ATP_InformationsInstanceDesc[]={
    {imsi,{{tString,64,0}}, NULL,getDeviceInfoX_ATP_Informationsimsi,NULL,NULL},
    {sn,{{tString,64,0}}, NULL,getDeviceInfoX_ATP_Informationssn,NULL,NULL},
   {NULL}
};

TRxObjNode  deviceInfoDesc[] = {

	{DeviceLog,{{tString,1024*32,1}}, NULL,getDeviceLog,NULL,NULL}, 
   	{Manufacturer,{{tString,64,0}}, NULL,getManufacturer,NULL,NULL},
    {ManufacturerOUI,{{tString,6,0}}, NULL,getManufacturerOUI,NULL,NULL},
  	{ModelName,{{tString,64,0}}, NULL,getModelName,NULL,NULL},
  	{Description,{{tString,64,0}}, NULL,getDescription,NULL,NULL},
  	{ProductClass,{{tString,256,0}},NULL,getProductClass,NULL,NULL},
  	{ProductType,{{tString,64,0}}, NULL,getProductType,NULL,NULL},
  	{SerialNumber,{{tString,64,0}}, NULL,getSerialNumber,NULL,NULL},
  	{HardwareVersion,{{tString,64,0}}, NULL,getHardwareVersion,NULL,NULL},
  	{SoftwareVersion,{{tString,64,0}}, NULL,getSoftwareVersion,NULL,NULL},
  	{ModemFirmwareVersion,{{tString,64,0}}, NULL,getModemFirmwareVersion,NULL,NULL},
  	{AdditionalSoftwareVersion,{{tString,64,0}}, NULL,getAdditionalSoftwareVersion,NULL,NULL},
	{SpecVersion,{{tString,16,0}}, NULL,getSpecVersion,NULL,NULL},
	{ProvisioningCode,{{tString,64,0}}, setProvisioningCode,getProvisioningCode,NULL,NULL},  
 	{UpTime,{{tUnsigned,0,1}}, NULL,getUpTime,NULL,NULL},
	{X_ATP_Informations,{{tObject,0}},
	  NULL,
	  NULL,
	  DeviceInfoX_ATP_InformationsInstanceDesc},
	
#if 0	
    {Description,{{tString,256,0}}, NULL,getDescription,NULL,NULL},
    {ModemFirmwareVersion,{{tString,64,0}}, NULL,getModemFirmwareVersion,NULL,NULL},
  	/*	add by wangminglin 	*/
	{X_ATP_IMEI,{{tString,IMEI_LEN,0}}, NULL,getX_ATP_IMEI,NULL,NULL},
	{X_ATP_AntennaConfig,{{tUnsigned,0,0}}, setX_ATP_AntennaConfig,getX_ATP_AntennaConfig,NULL,NULL},
	{X_ATP_PCBVersion,{{tUnsigned,0,0}}, NULL,getX_ATP_PCBVersion,NULL,NULL},
	{X_ATP_LastSoftwareVersion,{{tString,SOFTWAREVERSION_LEN,0}}, NULL,getX_ATP_LastSoftwareVersion,NULL,NULL},
	{X_ATP_ModemProductName,{{tString,MODEMPRODUCTNAME_LEN,0}}, NULL,getX_ATP_ModemProductName,NULL,NULL},
	{X_ATP_SupportVpnNum,{{tUnsigned,0,0}}, NULL,getX_ATP_SupportVpnNum,NULL,NULL},
	{X_ATP_BOMCode,{{tString,BOMCODE_LEN,0}}, NULL,getX_ATP_BOMCode,NULL,NULL},
	{X_ATP_Temperature,{{ tInt,0,0}}, NULL,getX_ATP_Temperature,NULL,NULL},

	/*ÔÝÊ±ÆÁ±Î*/
   	{FirstUseDate,{{tDateTime,0,0}}, NULL,getFirstUseDate,NULL,NULL},
	{X_CMCC_IMEI,{{tString,64,0}}, NULL,getX_CMCC_IMEI,NULL,NULL},
    {X_CMCC_ConfigVersion,{{tString,64,0}}, NULL,getX_CMCC_ConfigVersion,NULL,NULL},
    {X_CMCC_ModuleVersion,{{tString,64,0}}, NULL,getX_CMCC_ModuleVersion,NULL,NULL},
    {X_CMCC_ModuleType,{{tString,64,0}}, NULL,getX_CMCC_ModuleType,NULL,NULL},
	{X_CMCC_TeleComAccount,{{tObject,0}},NULL,NULL,DeviceInfoX_CMCC_TeleComAccountDesc},
	{X_CMCC_Monitor,{{tObject,0}},NULL,NULL,DeviceInfoX_CMCC_MonitorDesc},
	{X_CMCC_Alarm,{{tObject,0}},NULL,NULL, DeviceInfoX_CMCC_AlarmDesc},
 	{X_OUI_PLATFORM_VER,{{tString,64,0}}, NULL,getX_OUI_PLATFORM_VER,NULL,NULL},
	{X_OUI_PRIVATE_SOFTVER,{{tString,64,0}}, NULL,getX_OUI_PRIVATE_SOFTVER,NULL,NULL},
#endif
	{NULL}
};

TRxObjNode  x_deviceInfoDesc[] = {
    {DeviceStatus,{{tString,0,0}}, NULL,getDeviceStatus, NULL,NULL},
    {UserName,{{tString,0,0}}, NULL,getUserName, NULL,NULL},
    {LoadAverage,{{tUnsigned,0,0}}, NULL,getLoadAverage, NULL,NULL},
    {Memory,{{tUnsigned,0,0}}, NULL,getMemory, NULL,NULL},
    {NULL}
};


TRxObjNode  DeviceConfigDesc[] ={
	{X_ATP_PreserveEnable,{{tBool,0,0}}, setX_ATP_PreserveEnable,getX_ATP_PreserveEnable,NULL,NULL},
	{NULL}
};

TRxObjNode X_ATP_EXTDesc[]={

	//{ModemLogSessionStatus,{{tString,256,0}}, NULL,getModemLogSessionStatus, NULL,NULL},	
	{CellID,{{tUnsigned,0,0}}, NULL,getX_ATP_EXTCellID, NULL,NULL},
	{DownloadProgress,{{tUnsigned,0,0}}, NULL,getDownloadProgress, NULL,NULL},
	
	{NULL}
};

TRxObjNode ForwardingDesc[]={
	{DestIPAddress,{{tString,DESTIPADDRESS_LEN,0}}, setForwardingDestIPAddress,getForwardingDestIPAddress, NULL,NULL},
	{DestSubnetMask,{{tString,DESTSUBNETMASK_LEN,0}}, setForwardingDestSubnetMask,getForwardingDestSubnetMask, NULL,NULL},
	{GatewayIPAddress,{{tString,GATEWAYIPADDRESS_LEN,0}}, setForwardingGatewayIPAddress,getForwardingGatewayIPAddress, NULL,NULL},
	{Interface,{{tString,INTERFACE_LEN,0}}, setForwardingInterface,getForwardingInterface, NULL,NULL},

	{NULL}
};
TRxObjNode ForwardingDescObj[] = {    
    {instanceIDMASK,{{0}},
	  NULL,
	  NULL,
	  ForwardingDesc},
};
TRxObjNode Layer3ForwardingDesc[]={
	
	{ForwardNumberOfEntries,{{tUnsigned,0,0}}, NULL,getForwardNumberOfEntries, NULL,NULL},
	{Forwarding,{{tObject,0,0}}, NULL,NULL, ForwardingDescObj,NULL},
	{NULL}
};


TRxObjNode LanCheckDesc[]={

	{Host,{{tString,32,0}}, setLanCheckHost,getLanCheckHost, NULL,NULL},
	{Status,{{tUnsigned,0,0}}, setLanCheckStatus,getLanCheckStatus, NULL,NULL},	
	{NULL}
};

TRxObjNode WanCheckDesc[]={

	{Host,{{tString,32,0}}, setWanCheckHost,getWanCheckHost, NULL,NULL},
	{Status,{{tString,0,0}}, setWanCheckStatus,getWanCheckStatus, NULL,NULL},	
	{NULL}
};


TRxObjNode SysMonitorDesc[]={

	{Status,{{tUnsigned,0,0}}, setSysMonitorStatus,getSysMonitorStatus, NULL,NULL},	
	{NULL}
};


TRxObjNode X_ATP_SystemMonitorDesc[]={
	
	{RsrpWeakBoundary,{{ tInt,0,0}}, setRsrpWeakBoundary,getRsrpWeakBoundary,NULL,NULL},
	{LanCheck,{{tObject,0,0}}, NULL,NULL, LanCheckDesc,NULL},
	{WanCheck,{{tObject,0,0}}, NULL,NULL, WanCheckDesc,NULL},
	{SysMonitor,{{tObject,0,0}}, NULL,NULL, SysMonitorDesc,NULL},
	{NULL}
};
TRxObjNode FreqLDataDesc[]={
	
	{FreqLock_Mode,{{tUnsigned,0}},
	  setFreqLock_Mode,
	  getFreqLock_Mode,
	  NULL,NULL},
	{FreqBand_Info,{{tUnsigned,0}},
	  setFreqBand_Info,
	  getFreqBand_Info,
	  NULL,NULL},
	{FreqLock_FreqValue,{{tUnsigned,0}},
	  setFreqLock_FreqValue,
	  getFreqLock_FreqValue,
	  NULL,NULL},
	{FreqLock_Cell,{{tUnsigned,0}},
	  setFreqLock_Cell,
	  getFreqLock_Cell,
	  NULL,NULL},
	  
	{NULL}
};

TRxObjNode FreqLockDesc[]={
	{FreqLData,{{tObject,0,0}}, 
		NULL,
		NULL, FreqLDataDesc,NULL},
		
	{NULL}
};

TRxObjNode VlanMapVniListDesc[]={		
	{NULL}
};

TRxObjNode VlanMapDscpListDesc[]={		
	{NULL}
};
TRxObjNode VXLAN_LACDesc[]={	
	{Enable,{{tBool,0,0}}, 
		 setVXLAN_LACEnable,
		 getVXLAN_LACEnable,
			NULL,NULL},
	{VxlanRemoteIpAddress,{{tString,32}},
	  setVXLAN_LACVxlanRemoteIpAddress,
	  getVXLAN_LACVxlanRemoteIpAddress,
	  NULL,NULL},
	{NULL}
};

TRxObjNode X_ATP_VxlanDesc[]={
	{VlanMapVniList,{{tObject,0,0}}, 
		NULL,
		NULL, VlanMapVniListDesc,NULL},	
	{VlanMapDscpList,{{tObject,0,0}}, 
		NULL,
		NULL, VlanMapDscpListDesc,NULL},	
	{VXLAN_LAC,{{tObject,0,0}}, 
		NULL,
		NULL, VXLAN_LACDesc,NULL},
	{NULL}
};

TRxObjNode IpMapDscpListDesc[]={		
	{NULL}
};

TRxObjNode L2GRE_LACDesc[]={	
	{Enable,{{tBool,0,0}}, 
		 setL2GRE_LACEnable,
		 getL2GRE_LACEnable,
			NULL,NULL},
	{GreRemoteIpAddress,{{tString,32}},
	  setL2GRE_LACGreRemoteIpAddress,
	  getL2GRE_LACGreRemoteIpAddress,
	  NULL,NULL},
	{VlanEnable,{{tBool,0,0}},
	  setL2GRE_LACVlanEnable,
	  getL2GRE_LACVlanEnable,
	  NULL,NULL},
	{DscpMarkMode,{{tString,32}},
	  setL2GRE_LACDscpMarkMode,
	  getL2GRE_LACDscpMarkMode,
	  NULL,NULL},
	{ARPDscpMark,{{tUnsigned,0}},
	  setL2GRE_LACARPDscpMark,
	  getL2GRE_LACARPDscpMark,
	  NULL,NULL},
	{IPMapVlanPrioEnable,{{tBool,0,0}},
	  setL2GRE_LACIPMapVlanPrioEnable,
	  getL2GRE_LACIPMapVlanPrioEnable,
	  NULL,NULL},
	{NULL}
};

TRxObjNode IpMapVlanPrioListDesc[]={		
	{NULL}
};

TRxObjNode VlanSettingsDesc[]={		
	{Interface,{{tString,32}},
	  setVlanSettingsInterface,
	  getVlanSettingsInterface,
	  NULL,NULL},
	{VlanType,{{tString,32}},
	  setVlanSettingsVlanType,
	  getVlanSettingsVlanType,
	  NULL,NULL},
	{AllowedPvidForwardEnable,{{tString,32}},
	  setVlanSettingsAdPvidFdEnable,
	  getVlanSettingsAdPvidFdEnable,
	  NULL,NULL},
	{Pvid,{{tUnsigned,0}},
	  setVlanSettingsPvid,
	  getVlanSettingsPvid,
	  NULL,NULL},
	{NULL}
};

TRxObjNode VlanPrioMapDscpListDesc[]={		
	{NULL}
};

TRxObjNode VlanListDesc[]={		
	{NULL}
};

TRxObjNode X_ATP_L2GreDesc[]={
	{IpMapDscpList,{{tObject,0,0}}, 
		NULL,
		NULL, IpMapDscpListDesc,NULL}, 
	{L2GRE_LAC,{{tObject,0,0}}, 
		NULL,
		NULL, L2GRE_LACDesc,NULL}, 
	{IpMapVlanPrioList,{{tObject,0,0}}, 
		NULL,
		NULL, IpMapVlanPrioListDesc,NULL}, 
	{VlanSettings,{{tObject,0,0}}, 
		NULL,
		NULL, VlanSettingsDesc,NULL}, 
	{VlanPrioMapDscpList,{{tObject,0,0}}, 
		NULL,
		NULL, VlanPrioMapDscpListDesc,NULL}, 
	{VlanList,{{tObject,0,0}}, 
		NULL,
		NULL, VlanListDesc,NULL}, 
	{NULL}
};

TRxObjNode X_ATP_ModemLogDesc[]={

	{Status,{{tUnsigned,0,0}}, setX_ATP_ModemLogStatus,getX_ATP_ModemLogStatus, NULL,NULL},
	{NULL}
};


TRxObjNode X_ATP_PreserveDataDesc[]={
	{ParameterName,{{tString,PARAMETERNAME_LEN,0}}, setParameterName,getParameterName, NULL,NULL},
	{NULL}
};


TRxObjNode X_ATP_WebUserInfoDesc[]={
	
	{Username,{{tString,X_ATP_WEBUSERNAME_LEN,0}}, setX_ATP_WebUsername,getX_ATP_WebUsername, NULL,NULL},
	{Userpassword,{{tString,X_ATP_WEBUSERPASSWORD_LEN,0}}, setX_ATP_WebUserpassword,getX_ATP_WebUserpassword, NULL,NULL},
	{NULL}
};
TRxObjNode X_ATP_WebUserInfoDescObj[] = {    
    {instanceIDMASK,{{0}},
	  NULL,
	  NULL,
	  X_ATP_WebUserInfoDesc},
};
TRxObjNode X_ATP_WebDesc[]={
	{UserInfo,{{tObject,0,0}}, NULL,NULL, X_ATP_WebUserInfoDescObj,NULL},
	{NULL}
};

TRxObjNode GatewayUserInterfaceDesc[]={
	{X_ATP_Web,{{tObject,0,0}}, NULL,NULL, X_ATP_WebDesc,NULL},
	
	{NULL}
};


TRxObjNode FilterModeDesc[]={
	{Level,{{tString,FILTERMODELEVEL_LEN,0}}, 
		NULL,
		getFilterModeLevel, 
		NULL,NULL},

	{MACMode,{{tUnsigned,0,0}}, 
	setMACMode,
	getMACMode, 
	NULL,NULL},

	{IPMode,{{tUnsigned,0,0}}, 
	setIPMode,
	getIPMode, 
	NULL,NULL},

	{URLMode,{{tUnsigned,0,0}}, 
	setURLMode,
	getURLMode, 
	NULL,NULL},
	
	{NULL}
};

TRxObjNode FilterModeDescObj[] = {    
    {instanceIDMASK,{{0}},
	  NULL,
	  NULL,
	  FilterModeDesc},
};
TRxObjNode UrlBlackFilterDesc[]={
	{Url,{{tString,URL_LEN,0}}, 
		setBlackFilterUrl,
		getBlackFilterUrl, 
		NULL,NULL},
	{NULL}
};
TRxObjNode UrlBlackFilterDescObj[] = {    
    {instanceIDMASK,{{0}},
	  NULL,
	  NULL,
	  UrlBlackFilterDesc},
};

TRxObjNode MacBlackFilterDesc[]={
	{SourceMACAddress,{{tString,SOURCEMACADDRESS_LEN,0}}, 
		setBlackFilterSourceMACAddress,
		getBlackFilterSourceMACAddress, 
		NULL,NULL},
	{NULL}
};

TRxObjNode MacBlackFilterDescObj[] = {    
    {instanceIDMASK,{{0}},
	  NULL,
	  NULL,
	  MacBlackFilterDesc},
};

TRxObjNode IpBlackFilterDesc[]={
	
	{SourceIPStart,{{tString,STARTSOURCEIP_LEN,0}}, 
		setBlackFilterSourceIPStart,
		getBlackFilterSourceIPStart, 
		NULL,NULL},

	{SourceIPEnd,{{tString,ENDSOURCEIP_LEN,0}}, 
		setBlackFilterSourceIPEnd,
		getBlackFilterSourceIPEnd, 
		NULL,NULL},

	{DestIPStart,{{tString,STARTDESTIP_LEN,0}}, 
		setBlackFilterDestIPStart,
		getBlackFilterDestIPStart, 
		NULL,NULL},

	{DestIPEnd,{{tString,ENDDESTIP_LEN,0}}, 
		setBlackFilterDestIPEnd,
		getBlackFilterDestIPEnd, 
		NULL,NULL},

	{Protocol,{{tString,PROTOCOL_LEN,0}}, 
		setBlackFilterProtocol,
		getBlackFilterProtocol, 
		NULL,NULL},

	{SourcePortStart,{{tUnsigned,0,0}}, 
	setBlackFilterSourcePortStart,
	getBlackFilterSourcePortStart, 
	NULL,NULL},

	{SourcePortEnd,{{tUnsigned,0,0}}, 
	setBlackFilterSourcePortEnd,
	getBlackFilterSourcePortEnd, 
	NULL,NULL},

	{DestPortStart,{{tUnsigned,0,0}}, 
	setBlackFilterDestPortStart,
	getBlackFilterDestPortStart, 
	NULL,NULL},

	{DestPortEnd,{{tUnsigned,0,0}}, 
	setBlackFilterDestPortEnd,
	getBlackFilterDestPortEnd, 
	NULL,NULL},
		
	{NULL}
};

TRxObjNode IpBlackFilterDescObj[] = {    
    {instanceIDMASK,{{0}},
	  NULL,
	  NULL,
	  IpBlackFilterDesc},
};



TRxObjNode UrlWhiteFilterDesc[]={
	{Url,{{tString,URL_LEN,0}}, 
		setWhiteFilterUrl,
		getWhiteFilterUrl, 
		NULL,NULL},
	{NULL}
};
TRxObjNode UrlWhiteFilterDescObj[] = {    
    {instanceIDMASK,{{0}},
	  NULL,
	  NULL,
	  UrlWhiteFilterDesc},
};

TRxObjNode MacWhiteFilterDesc[]={
	{SourceMACAddress,{{tString,SOURCEMACADDRESS_LEN,0}}, 
		setWhiteFilterSourceMACAddress,
		getWhiteFilterSourceMACAddress, 
		NULL,NULL},
	{NULL}
};
TRxObjNode MacWhiteFilterDescObj[] = {    
    {instanceIDMASK,{{0}},
	  NULL,
	  NULL,
	  MacWhiteFilterDesc},
};

TRxObjNode IpWhiteFilterDesc[]={
	{SourceIPStart,{{tString,STARTSOURCEIP_LEN,0}}, 
		setWhiteFilterSourceIPStart,
		getWhiteFilterSourceIPStart, 
		NULL,NULL},

	{SourceIPEnd,{{tString,ENDSOURCEIP_LEN,0}}, 
		setWhiteFilterSourceIPEnd,
		getWhiteFilterSourceIPEnd, 
		NULL,NULL},

	{DestIPStart,{{tString,STARTDESTIP_LEN,0}}, 
		setWhiteFilterDestIPStart,
		getWhiteFilterDestIPStart, 
		NULL,NULL},

	{DestIPEnd,{{tString,ENDDESTIP_LEN,0}}, 
		setWhiteFilterDestIPEnd,
		getWhiteFilterDestIPEnd, 
		NULL,NULL},

	{Protocol,{{tString,PROTOCOL_LEN,0}}, 
		setWhiteFilterProtocol,
		getWhiteFilterProtocol, 
		NULL,NULL},

	{SourcePortStart,{{tUnsigned,0,0}}, 
	setWhiteFilterSourcePortStart,
	getWhiteFilterSourcePortStart, 
	NULL,NULL},

	{SourcePortEnd,{{tUnsigned,0,0}}, 
	setWhiteFilterSourcePortEnd,
	getWhiteFilterSourcePortEnd, 
	NULL,NULL},

	{DestPortStart,{{tUnsigned,0,0}}, 
	setWhiteFilterDestPortStart,
	getWhiteFilterDestPortStart, 
	NULL,NULL},

	{DestPortEnd,{{tUnsigned,0,0}}, 
	setWhiteFilterDestPortEnd,
	getWhiteFilterDestPortEnd, 
	NULL,NULL},
	{NULL}
};

TRxObjNode IpWhiteFilterDescObj[] = {    
    {instanceIDMASK,{{0}},
	  NULL,
	  NULL,
	  IpWhiteFilterDesc},
};

TRxObjNode AccessControlDesc[]={
	{Service,{{tString,SERVICE_LEN,0}}, 
		NULL,
		getService, 
		NULL,NULL},

	{Direction,{{tString,DIRECTION_LEN,0}}, 
		NULL,
		getDirection, 
		NULL,NULL},

	{StartIpAddr,{{tString,STARTIPADDR_LEN,0}}, 
		setStartIpAddr,
		getStartIpAddr, 
		NULL,NULL},

	{EndIpAddr,{{tString,ENDIPADDR_LEN,0}}, 
		setEndIpAddr,
		getEndIpAddr, 
		NULL,NULL},

	{Enable,{{tBool,0,0}}, 
	 setAccessControlEnable,
	 getAccessControlEnable,
	 NULL,NULL},
	{NULL}
};

TRxObjNode AccessControlDescObj[] = {    
    {instanceIDMASK,{{0}},
	  NULL,
	  NULL,
	  AccessControlDesc},
};

/*TRxObjNode X_ATP_FireWallDesc[]={
	{CurrentLevel,{{tString,CURRENTLEVEL_LEN,0}}, 
	setX_ATP_FireWallCurrentLevel,
	getX_ATP_FireWallCurrentLevel,
	NULL,NULL},

	{FilterMode,{{tObject,0,0}}, NULL,NULL, FilterModeDescObj,NULL},
	{UrlBlackFilter,{{tObject,0,0}}, NULL,NULL, UrlBlackFilterDescObj,NULL},
	{MacBlackFilter,{{tObject,0,0}}, NULL,NULL, MacBlackFilterDescObj,NULL},
	{IpBlackFilter,{{tObject,0,0}}, NULL,NULL,  IpBlackFilterDescObj,NULL},

	{UrlWhiteFilter,{{tObject,0,0}}, NULL,NULL, UrlWhiteFilterDescObj,NULL},
	{MacWhiteFilter,{{tObject,0,0}}, NULL,NULL,MacWhiteFilterDescObj,NULL},
	{IpWhiteFilter,{{tObject,0,0}}, NULL,NULL,IpWhiteFilterDescObj,NULL},
	{AccessControl,{{tObject,0,0}}, NULL,NULL, AccessControlDescObj,NULL},
	{NULL}	
};
*/
TRxObjNode IpFilterDesc[]={
	
	{NULL}
};

TRxObjNode MacFilterDesc[]={
	
	{NULL}
};

TRxObjNode UrlFilterDesc[]={
	
	{NULL}
};

TRxObjNode SwitchDesc[]={
	{FirewallMainSwitch,{{tUnsigned,0,0}}, 
	 	setFirewallMainSwitch,
		 getFirewallMainSwitch,
		 NULL,NULL},
	 {FirewallIPFilterSwitch,{{tUnsigned,0,0}}, 
		 setFirewallIPFilterSwitch,
		 getFirewallIPFilterSwitch,
		 NULL,NULL},
     {FirewallWanPortPingSwitch,{{tUnsigned,0,0}}, 
		 setFirewallWanPortPingSwitch,
		 getFirewallWanPortPingSwitch,
		 NULL,NULL},
     {FirewallWanWebSwitch,{{tUnsigned,0,0}}, 
		 setFirewallWanWebSwitch,
		 getFirewallWanWebSwitch,
		 NULL,NULL},
	{firewallmacfilterswitch,{{tUnsigned,0,0}}, 
		 setfirewallmacfilterswitch,
		 getfirewallmacfilterswitch,
		 NULL,NULL},
	{firewallurlfilterswitch,{{tUnsigned,0,0}}, 
		 setfirewallurlfilterswitch,
		 getfirewallurlfilterswitch,
		 NULL,NULL},
	{firewallfwstateswitch,{{tUnsigned,0,0}}, 
		 setfirewallfwstateswitch,
		 getfirewallfwstateswitch,
		 NULL,NULL},
	
	{NULL}
};

TRxObjNode X_ATP_FireWallDesc[]={
	
	{IpFilter,{{tObject,0,0}}, NULL,NULL, IpFilterDesc,NULL},
	{MacFilter,{{tObject,0,0}}, NULL,NULL, MacFilterDesc,NULL},
	{UrlFilter,{{tObject,0,0}}, NULL,NULL, UrlFilterDesc,NULL},
	{Switch,{{tObject,0,0}}, NULL,NULL, SwitchDesc,NULL},
	{NULL}
};
TRxObjNode L2TP_LACDesc[]={
	{HostName,{{tString,STARTIPADDR_LEN,0}}, 
		setL2TP_LACHostName,
		getL2TP_LACHostName, 
		NULL,NULL},
	{Password,{{tString,STARTIPADDR_LEN,0}}, 
		setL2TP_LACPassword,
		getL2TP_LACPassword, 
		NULL,NULL},
	{Enable,{{tBool,0,0}}, 
		setL2TP_LACEnable,
		getL2TP_LACEnable, 
		NULL,NULL},
	{BCPEnable,{{tBool,0,0}}, 
		setL2TP_LACBCPEnable,
		getL2TP_LACBCPEnable, 
		NULL,NULL},
	{WanInterface,{{tString,STARTIPADDR_LEN,0}}, 
		setL2TP_LACWanInterface,
		getL2TP_LACWanInterface, 
		NULL,NULL},
	{LNSAddress,{{tString,STARTIPADDR_LEN,0}}, 
		setL2TP_LACLNSAddress,
		getL2TP_LACLNSAddress, 
		NULL,NULL},
	{PCAuthMode,{{tUnsigned,0,0}}, 
		setL2TP_LACPCAuthMode,
		getL2TP_LACPCAuthMode, 
		NULL,NULL},
	{PppUser,{{tString,STARTIPADDR_LEN,0}}, 
		setL2TP_LACPppUser,
		getL2TP_LACPppUser, 
		NULL,NULL},
	{PppPass,{{tString,STARTIPADDR_LEN,0}}, 
		setL2TP_LACPppPass,
		getL2TP_LACPppPass, 
		NULL,NULL},
	{KeepAliveTime,{{tUnsigned,0,0}}, 
		setL2TP_LACKeepAliveTime,
		getL2TP_LACKeepAliveTime, 
		NULL,NULL},
	{NULL}
};
TRxObjNode X_ATP_VPNDesc[]={
	
	{L2TP_LAC,{{tObject,0,0}}, NULL,NULL, L2TP_LACDesc,NULL},
	{NULL}
};

TRxObjNode AlarmInstanceDesc[]={
	{Description,{{tString,STARTIPADDR_LEN,0}}, 
		NULL,
		getAlarmInstanceDescription, 
		NULL,NULL},
	{LiftDescription,{{tString,STARTIPADDR_LEN,0}}, 
		NULL,
		getAlarmInstanceLiftDescription, 
		NULL,NULL},
	{NULL}
};
TRxObjNode X_ATP_AlarmInfoDesc[]={
	{SequenceNo,{{tUnsigned,0,0}}, 
		NULL,
		getX_ATP_AlarmInfoSequenceNo, 
		NULL,NULL},
	{AlarmInstance,{{tObject,0,0}}, NULL,NULL, AlarmInstanceDesc,NULL},
	{NULL}
};
TRxObjNode profileDesc[]={
	/*{index,{{tUnsigned,0,0}}, 
		NULL,
		getdialupprofileindex, 
		NULL,NULL},*/
	{profile_name,{{tString,64,0}}, 
		setdialupprofile_name,
		getdialupprofile_name, 
		NULL,NULL},
	{apn,{{tString,64,0}}, 
		setdialupapn,
		getdialupapn, 
		NULL,NULL},
	{username,{{tString,64,0}}, 
		setdialupusername,
		getdialupusername, 
		NULL,NULL},
	{password,{{tString,64,0}}, 
		setdialuppassword,
		getdialuppassword, 
		NULL,NULL},
	{auth_mode,{{tString,64,0}}, 
		setdialupauth_mode,
		getdialupauth_mode, 
		NULL,NULL},
	{NULL}
};

TRxObjNode dialupDesc[]={
	{roam_connect,{{tUnsigned,0,0}}, 
		setdialuproam_connect,
		getdialuproam_connect, 
		NULL,NULL},
	{current_profile,{{tUnsigned,0,0}}, 
		setdialupcurrent_profile,
		getdialupcurrent_profile, 
		NULL,NULL},
	{profile,{{tObject,0,0}}, NULL,NULL, profileDesc,NULL},
	{NULL}
};

TRxObjNode X_ATP_ConfigDesc[]={
	{dialup,{{tObject,0,0}}, NULL,NULL, dialupDesc,NULL},
	{NULL}
};

TRxObjNode ListDesc[]={
	{Codec,{{tString,64,0}}, 
		NULL,
		getListCodec, 
		NULL,NULL},
	{NULL}
};

TRxObjNode CodecDesc[]={
	{List,{{tObject,0,0}}, NULL,NULL, ListDesc,NULL},
	{NULL}
};

TRxObjNode SIPDesc[]={
	{AuthUserName,{{tString,64,0}}, 
		setSipAuthUserName,
		getSipAuthUserName, 
		NULL,NULL},
	{AuthPassword,{{tString,64,0}}, 
		setSipAuthPassword,
		getSipAuthPassword, 
		NULL,NULL},
	{NULL}
};

	
TRxObjNode LineDesc[]={
	{Enable,{{tString,64,0}}, 
		setLineEnable,
		getLineEnable, 
		NULL,NULL},
	{DirectoryNumber,{{tString,64,0}}, 
		setLineDirectoryNumber,
		getLineDirectoryNumber, 
		NULL,NULL},
	{Status,{{tString,64,0}}, 
		NULL,
		getLineStatus, 
		NULL,NULL},
	{X_ATP_ClipEnable,{{tBool,0,0}}, 
		setLineX_ATP_ClipEnable,
		getLineX_ATP_ClipEnable, 
		NULL,NULL},
	{Codec,{{tObject,0,0}}, NULL,NULL, CodecDesc,NULL},
	{SIP,{{tObject,0,0}}, NULL,NULL, SIPDesc,NULL},
	{NULL}
};

TRxObjNode ProfileSIPDesc[]={
	{ProxyServer,{{tString,64,0}}, 
		setSipProxyServer,
		getSipProxyServer, 
		NULL,NULL},
	{ProxyServerPort,{{tString,64,0}}, 
		setSipProxyServerPort,
		getSipProxyServerPort, 
		NULL,NULL},
	{RegistrarServer,{{tString,64,0}}, 
		setSipRegistrarServer,
		getSipRegistrarServer, 
		NULL,NULL},
	{RegistrarServerPort,{{tString,64,0}}, 
		setSipRegistrarServerPort,
		getSipRegistrarServerPort, 
		NULL,NULL},
	{X_ATP_SIPDomain,{{tString,64,0}}, 
		setSipX_ATP_SIPDomain,
		getSipX_ATP_SIPDomain, 
		NULL,NULL},
	{X_SecondProxyServer,{{tString,64,0}}, 
		setSipX_SecondProxyServer,
		getSipX_SecondProxyServer, 
		NULL,NULL},
	{X_SecondProxyServerPort,{{tString,64,0}}, 
		setSipX_SecondProxyServerPort,
		getSipX_SecondProxyServerPort, 
		NULL,NULL},
	{X_SecondSIPDomain,{{tString,64,0}}, 
		setSipX_SecondSIPDomain,
		getSipX_SecondSIPDomain, 
		NULL,NULL},
	{X_SecondRegistrarServer,{{tString,64,0}}, 
		setSipX_SecondRegistrarServer,
		getSipX_SecondRegistrarServer, 
		NULL,NULL},
	{X_SecondRegistrarServerPort,{{tString,64,0}}, 
		setSipX_SecondRegistrarServerPort,
		getSipX_SecondRegistrarServerPort, 
		NULL,NULL},
	{NULL}
};

TRxObjNode VoiceProfileDesc[]={
	{DTMFMethod,{{tString,64,0}}, 
		NULL,
		getVoiceProfileDTMFMethod, 
		NULL,NULL},
	{Line,{{tObject,0,0}}, NULL,NULL, LineDesc,NULL},
	{SIP,{{tObject,0,0}}, NULL,NULL, ProfileSIPDesc,NULL},
	{NULL}
};

TRxObjNode ButtonDesc[]={
	
	{NULL}
};


TRxObjNode X_ATP_CommonConfigDesc[]={
	{X_ATP_Voipcsselect,{{tInt,0,0}}, 
		NULL,
		getVoiceX_ATP_Voipcsselect, 
		NULL,NULL},
	{X_ATP_CIDSendType,{{tInt,0,0}}, 
		NULL,
		getVoiceX_ATP_CIDSendType, 
		NULL,NULL},
	{Button,{{tObject,0,0}}, NULL,NULL, ButtonDesc,NULL},
	{NULL}
};

TRxObjNode VoiceServiceDesc[]={
	{X_ATP_UmtsDTMFMethod,{{tString,64,0}}, 
		setVoiceServiceX_ATP_UmtsDTMFMethod,
		getVoiceServiceX_ATP_UmtsDTMFMethod, 
		NULL,NULL},
	{VoiceProfile,{{tObject,0,0}}, NULL,NULL, VoiceProfileDesc,NULL},
	{X_ATP_CommonConfig,{{tObject,0,0}}, NULL,NULL, X_ATP_CommonConfigDesc,NULL},
	{NULL}
};	
TRxObjNode ServicesDesc[]={
	{VoiceService,{{tObject,0,0}}, NULL,NULL, VoiceServiceDesc,NULL},
	{NULL}
};
	
 
