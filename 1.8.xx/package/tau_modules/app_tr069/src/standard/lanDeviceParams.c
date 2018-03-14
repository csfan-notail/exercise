/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : lanDeviceParams.c
  版 本 号   : 初稿
  作    者   : zhangjiasi
  生成日期   : 2015年6月4日
  最近修改   :
  功能描述   : lan相关信息。接口定义及树形结构拓扑定义
  函数列表   :
  修改历史   :
  1.日    期   : 2015年6月4日
    作    者   : zhangjiasi
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "lanDeviceParams.h"
/*Add by zhanghao 获取配置文件内容 2016/6/15 begin*/
#include "uci_config.h"
#include "tr69_lua.h"
/*Add by zhanghao 获取配置文件内容 end*/

extern int g_log_level;




#define CHADDR_LEN 64
#define YIADDR_LEN 64
#define X_ATP_RIPOPERATION_LEN 64
#define X_ATP_ROUTEPROTOCOLRX_LEN 64
#define X_ATP_PRE_ROUTEPROTOCOL_LEN 64
#define WLANCONFIGURATIONSTATUS_LEN 64
#define WLANCONFIGURATIONBSSID_LEN 64
#define MAXBITRATE_LEN 64
#define REGULATORYDOMAIN_LEN 64
#define SSID_LEN 64
#define BEACONTYPE_LEN 64 
#define STANDARD_LEN 64
#define WEPENCRYPTIONLEVEL_LEN 64
#define BASICENCRYPTIONMODES_LEN 64
#define BASICAUTHENTICATIONMODE_LEN 64
#define WPAENCRYPTIONMODES_LEN 64
#define WPAAUTHENTICATIONMODE_LEN 64
#define IEEE11IENCRYPTIONMODES_LEN 64
#define IEEE11IAUTHENTICATIONMODE_LEN 64
#define X_ATP_MIXEDENCRYPTIONMODES_LEN 64
#define X_ATP_MIXEDAUTHENTICATIONMODE_LEN 64
#define X_ATP_WLANVERSION_LEN 64
#define X_ATP_WLAN11NBWCONTROL_LEN 64
#define X_ATP_WLAN11NGICONTROL_LEN 64
#define WPSX_ATP_WPSMODE_LEN 64
#define WPSDEVICEPASSWORD_LEN 64
#define X_ATP_WLANSRCMACADDRESS_LEN 64
#define ASSOCIATEDDEVICEMACADDRESS_LEN 64
#define WEPKEY_LEN 64
#define PRESHAREDKEY_LEN 64
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

/* InternetGatewayDevice.LANConfigSecurity. */
TRxObjNode  LANConfigSecurityDesc[] = {
    {ConfigPassword,{{tString,64,0}}, setConfigPassword,getConfigPassword, NULL,NULL},
    {NULL}
};

/* InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1 */
TRxObjNode LANDeviceLANHostConfigManagementIPInterfaceDesc[] = {

	{IPInterfaceIPAddress,{{tipv4,16}},
	  setLANDeviceLANHostConfigManagementIPInterface_IPInterfaceIPAddress,
	  getLANDeviceLANHostConfigManagementIPInterface_IPInterfaceIPAddress,
	  NULL,NULL},
	  
	{IPInterfaceSubnetMask,{{tString,16}},
	  setLANDeviceLANHostConfigManagementIPInterface_IPInterfaceSubnetMask,
	  getLANDeviceLANHostConfigManagementIPInterface_IPInterfaceSubnetMask,
	  NULL,NULL},
#if 0
	{Enable,{{tBool,0}},
	  setLANDeviceLANHostConfigManagementIPInterface_Enable, 
	  getLANDeviceLANHostConfigManagementIPInterface_Enable,
	  NULL,NULL},

	{IPInterfaceAddressingType,{{tString,16}},
	  setLANDeviceLANHostConfigManagementIPInterface_IPInterfaceAddressingType,   
	  getLANDeviceLANHostConfigManagementIPInterface_IPInterfaceAddressingType,
	  NULL,NULL},

	{X_ATP_RouteProtocolRx,{{tString,X_ATP_ROUTEPROTOCOLRX_LEN}},
	  setLANDeviceLANHostConfigManagementIPInterface_X_ATP_RouteProtocolRx,
	  getLANDeviceLANHostConfigManagementIPInterface_X_ATP_RouteProtocolRx,
	  NULL,NULL},

	{X_ATP_RIPOperation,{{tString,X_ATP_RIPOPERATION_LEN}},
	  setLANDeviceLANHostConfigManagementIPInterface_X_ATP_RIPOperation,
	  getLANDeviceLANHostConfigManagementIPInterface_X_ATP_RIPOperation,
	  NULL,NULL},

	{X_ATP_Pre_RouteProtocol,{{tString,X_ATP_PRE_ROUTEPROTOCOL_LEN}},
	  setLANDeviceLANHostConfigManagementIPInterface_X_ATP_Pre_RouteProtocol,
	  getLANDeviceLANHostConfigManagementIPInterface_X_ATP_Pre_RouteProtocol,
	  NULL,NULL},
#endif
	{NULL}
};

TRxObjNode LANDeviceLANHostConfigManagementIPInterfaceInstanceDesc[] = {
	{instanceIDMASK,{{0}},
	  NULL, /* delLANDeviceLANHostConfigManagementIPInterfaceI,*/
	  NULL, /* addLANDeviceLANHostConfigManagementIPInterfaceI,*/
	  LANDeviceLANHostConfigManagementIPInterfaceDesc},
};

TRxObjNode DHCPStaticAddressDesc[]={
	/*{DHCPStaticAddressEnable,{{tBool,0}},
	  setDHCPStaticAddressEnable,
	  getDHCPStaticAddressEnable,
	  NULL,NULL},

	{DHCPStaticAddressChaddr,{{tString,CHADDR_LEN}},
	  setDHCPStaticAddressChaddr,
	  getDHCPStaticAddressChaddr,
	  NULL,NULL},

	{DHCPStaticAddressYiaddr,{{tString,YIADDR_LEN}},
	  setDHCPStaticAddressYiaddr,
	  getDHCPStaticAddressYiaddr,
	  NULL,NULL},

	{X_ATP_valid,{{tBool,0}},
	  setDHCPStaticAddressX_ATP_valid,
	  getDHCPStaticAddressX_ATP_valid,
	  NULL,NULL},*/
	{NULL}
};
TRxObjNode DHCPRelaySeverConfigDesc[]={
	{DHCPRelayEnable,{{tBool,0}},
	  setLANDeviceLANHostConfigManagement_DHCPRelayEnable,
	  getLANDeviceLANHostConfigManagement_DHCPRelayEnable,
	  NULL,NULL},

	{ServerIPAddr,{{tString,32}},
		setLANDeviceLANHostConfigManagement_ServerIPAddr,
		getLANDeviceLANHostConfigManagement_ServerIPAddr,
		NULL,NULL},
	{NULL}
};

/* InternetGatewayDevice.LANDevice.1.LANHostConfigManagement. */
TRxObjNode LANDeviceLANHostConfigManagementDesc[] = {

	{IPInterface,{{tInstance,0}},
		NULL,
		NULL,
	LANDeviceLANHostConfigManagementIPInterfaceInstanceDesc},
	
	{MACAddress,{{tString,64}},
		NULL,
		getLANDeviceLANHostConfigManagement_MACAddress,
		NULL,NULL},

	{DHCPServerEnable,{{tselect,0}},
	  setLANDeviceLANHostConfigManagement_DHCPServerEnable,
	  getLANDeviceLANHostConfigManagement_DHCPServerEnable,
	  NULL,NULL},
	{MinAddress,{{tString,16}},
	  setLANDeviceLANHostConfigManagement_MinAddress,
	  getLANDeviceLANHostConfigManagement_MinAddress,
	  NULL,NULL},
	  
	{MaxAddress,{{tString,16}},
	  setLANDeviceLANHostConfigManagement_MaxAddress,
	  getLANDeviceLANHostConfigManagement_MaxAddress,
	  NULL,NULL},
	{DHCPLeaseTime,{{tInt,0}},
	  setLANDeviceLANHostConfigManagement_DHCPLeaseTime,
	  getLANDeviceLANHostConfigManagement_DHCPLeaseTime,
	  NULL,NULL},
	{PassthroughLease,{{tUnsigned,0}},
	  setLANDeviceLANHostConfigManagement_PassthroughLease,
	  getLANDeviceLANHostConfigManagement_PassthroughLease,
	  NULL,NULL},
	{IPInterfaceNumberOfEntries,{{tUnsigned,0}},
	  NULL,
	  getLANDeviceLANHostConfigManagement_IPInterfaceNumberOfEntries,
	  NULL,NULL},
	{DHCPStaticAddressNumberOfEntries,{{tUnsigned,0}},
	  NULL,
	  getLANDeviceLANHostConfigManagement_DHCPStaticAddressNumberOfEntries,
	  NULL,NULL}, 
	  
	{DHCPRelaySeverConfig,{{tObject,0}},
	  NULL,
	  NULL,
	  DHCPRelaySeverConfigDesc},
	{DHCPStaticAddress,{{tObject,0}},
	  NULL,
	  NULL,
	  DHCPStaticAddressDesc},
#if 0	
	{SubnetMask,{{tString,16}},
	  setLANDeviceLANHostConfigManagement_SubnetMask,
	  getLANDeviceLANHostConfigManagement_SubnetMask,
	  NULL,NULL},
	  
    
   	
	{DHCPServerConfigurable,{{tBool,0}},
	  setLANDeviceLANHostConfigManagement_DHCPServerConfigurable,
	  getLANDeviceLANHostConfigManagement_DHCPServerConfigurable,
	  NULL,NULL},
	  
	{ReservedAddresses,{{tString,16}},
	  setLANDeviceLANHostConfigManagement_ReservedAddresses,
	  getLANDeviceLANHostConfigManagement_ReservedAddresses,
	  NULL,NULL},
	  
	{DNSServers,{{tString,64}},
	  setLANDeviceLANHostConfigManagement_DNSServers,
	  getLANDeviceLANHostConfigManagement_DNSServers,
	  NULL,NULL},
	  
	{DomainName,{{tString,64}},
	  setLANDeviceLANHostConfigManagement_DomainName,
	  getLANDeviceLANHostConfigManagement_DomainName,
	  NULL,NULL},
	  
	{IPRouters,{{tString,64}},
	  setLANDeviceLANHostConfigManagement_IPRouters,
	  getLANDeviceLANHostConfigManagement_IPRouters,
	  NULL,NULL},
	  
	
	  
	{AllowedMACAddresses,{{tString,16}},
	  setLANDeviceLANHostConfigManagement_AllowedMACAddresses,
	  getLANDeviceLANHostConfigManagement_AllowedMACAddresses,
	  NULL,NULL},
	  
      
	
	 
	{X_ATP_VlanID,{{tUnsigned,0}},
	  setLANDeviceLANHostConfigManagement_X_ATP_VlanID,
	  getLANDeviceLANHostConfigManagement_X_ATP_VlanID,
	  NULL,NULL},   

	
	  
#endif  

	{NULL}

};

/* InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Stats. */
TRxObjNode LANDeviceLANEthernetInterfaceConfigStatsDesc[] = {

	{BytesSent,{{tUnsigned,0,1}},
	  NULL,
	  getLANDeviceLANEthernetInterfaceConfigStats_BytesSent,
	  NULL,NULL},

	{BytesReceived,{{tUnsigned,0,1}},
	  NULL,
	  getLANDeviceLANEthernetInterfaceConfigStats_BytesReceived,
	  NULL,NULL},

	{DiscardPacketsSent,{{tUnsigned,0,1}},
	  NULL,
	  getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_PacketsReceivedDrops,
	  NULL,NULL},

	{DiscardPacketsReceived,{{tUnsigned,0,1}},
	  NULL,
	  getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_PacketsSentDrops,
	  NULL,NULL},
	  
	{PacketsSent,{{tUnsigned,0,1}},
	  NULL,
	  getLANDeviceLANEthernetInterfaceConfigStats_PacketsSent,
	  NULL,NULL},
	  
	{PacketsReceived,{{tUnsigned,0,1}},
	  NULL,
	  getLANDeviceLANEthernetInterfaceConfigStats_PacketsReceived,
	  NULL,NULL},

	{X_ATP_SentRate,{{tUnsigned,0,1}},
	  NULL,
	  getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_SentRate,
	  NULL,NULL},


	{X_ATP_ReceivedRate,{{tUnsigned,0,1}},
	  NULL,
	  getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_ReceivedRate,
	  NULL,NULL},

	{ErrorsSent,{{tUnsigned,0,1}},
	  NULL,
	  getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_PacketsSentError,
	  NULL,NULL},

	{ErrorsReceived,{{tUnsigned,0,1}},
	  NULL,
	  getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_PacketsReceivedError,
	  NULL,NULL},

	{X_ATP_PacketsSentError,{{tUnsigned,0,1}},
	  NULL,
	  getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_PacketsSentError,
	  NULL,NULL},

	{X_ATP_PacketsSentDrops,{{tUnsigned,0,1}},
	  NULL,
	  getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_PacketsSentDrops,
	  NULL,NULL},
	{X_ATP_PacketsReceivedError,{{tUnsigned,0,1}},
	  NULL,
	  getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_PacketsReceivedError,
	  NULL,NULL},

	{X_ATP_PacketsReceivedDrops,{{tUnsigned,0,1}},
	  NULL,
	  getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_PacketsReceivedDrops,
	  NULL,NULL},
	{X_ATP_PacketsError,{{tUnsigned,0,1}},
	  NULL,
	  getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_PacketsError,
	  NULL,NULL},
	 {X_ATP_PacketsDrops,{{tUnsigned,0,1}},
	  NULL,
	  getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_PacketsDrops,
	  NULL,NULL},
#if 0
	
	 {X_ATP_Timestamp,{{tUnsigned,0,1}},
	  NULL,
	  getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_Timestamp,
	  NULL,NULL},
	  
	

#endif

	{NULL}
};


/* InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1. */
TRxObjNode LANDeviceLANEthernetInterfaceConfigDesc[] = {

	{Status,{{tString,16}},
	  NULL,
	  getLANDeviceLANEthernetInterfaceConfig_Status,
	  NULL,NULL},

	{Stats,{{tObject,0}},
	  NULL,
	  NULL,
	  LANDeviceLANEthernetInterfaceConfigStatsDesc},
	{Enable,{{tBool,0}},
	  NULL,
	  NULL,
	  NULL,NULL},
#if 0

	{Enable,{{tBool,0}},
	  setLANDeviceLANEthernetInterfaceConfig_Enable,
	  getLANDeviceLANEthernetInterfaceConfig_Enable,
	  NULL,NULL},
	{MACAddress,{{tString,16}},
	  NULL,
	  getLANDeviceLANEthernetInterfaceConfig_MACAddress,
	  NULL,NULL},

#endif
	{NULL}
};

TRxObjNode LANDeviceLANEthernetInterfaceConfigInstanceDesc[] = {    
    {instanceIDMASK,{{0}},
	  NULL,
	  NULL,
	  LANDeviceLANEthernetInterfaceConfigDesc},
};

/* InternetGatewayDevice.LANDevice.1.Hosts.Host.{i} */
TRxObjNode LANDeviceHostsHostDesc[] = {
	{IPAddress,{{tString,16}},
	  NULL,
	  getLANDeviceHostsHost_IPAddress,
	  NULL,NULL},
	{MACAddress,{{tString,16}},
	  NULL,
	  getLANDeviceHostsHost_MACAddress,
	  NULL,NULL},
	{HostName,{{tString,64}},
	  NULL,
	  getLANDeviceHostsHost_HostName,
	  NULL,NULL},
	{InterfaceType,{{tString,16}},
	  NULL,
	  getLANDeviceHostsHost_InterfaceType,
	  NULL,NULL},
	{NULL}
};

TRxObjNode LANDeviceHostsHostInstanceDesc[] = {
	{instanceIDMASK,{{0}},
	  NULL,
	  NULL,
	  LANDeviceHostsHostDesc},
};

/* InternetGatewayDevice.LANDevice.1.Hosts. */
TRxObjNode LANDeviceHostsDesc[] = {
	{HostNumberOfEntries,{{tUnsigned,0}},
	  NULL,
	  getLANDeviceHosts_HostNumberOfEntries,
	  NULL,NULL},
	{Host,{{tInstance,0}},
	  NULL,
	  NULL,
	  LANDeviceHostsHostInstanceDesc},
	{NULL}
};



TRxObjNode WPSDesc[]={
	{Enable,{{tBool,0,0}}, 
	 NULL,
	 getWPSEnable,
	NULL,NULL},
	
	{X_ATP_WPSMode,{{tString,WPSX_ATP_WPSMODE_LEN}},
	  NULL,
	  getWPSX_ATP_WPSMode,
	  NULL,NULL},

	{DevicePassword,{{tString,WPSDEVICEPASSWORD_LEN}},
	  NULL,
	  getWPSDevicePassword,
	  NULL,NULL},
	
	{NULL}
};

TRxObjNode X_ATP_WlanMacFilterDesc[]={
	{X_ATP_WlanSrcMacAddress,{{tString,X_ATP_WLANSRCMACADDRESS_LEN}},
	  setX_ATP_WlanSrcMacAddress,
	  getX_ATP_WlanSrcMacAddress,
	  NULL,NULL},
	{X_ATP_SSID1,{{tBool,0,0}}, 
	 setX_ATP_SSID1,
	 getX_ATP_SSID1,
	NULL,NULL},

	{X_ATP_SSID2,{{tBool,0,0}}, 
	 setX_ATP_SSID2,
	 getX_ATP_SSID2,
	NULL,NULL},

	{X_ATP_SSID3,{{tBool,0,0}}, 
	 setX_ATP_SSID3,
	 getX_ATP_SSID3,
	NULL,NULL},

	{X_ATP_SSID4,{{tBool,0,0}}, 
	 setX_ATP_SSID4,
	 getX_ATP_SSID4,
	NULL,NULL},
	{NULL}
};

TRxObjNode X_ATP_WlanMacFilterDescObj[] = {    
    {instanceIDMASK,{{0}},
	  NULL,
	  NULL,
	  X_ATP_WlanMacFilterDesc},
};
TRxObjNode AssociatedDeviceDesc[]={
	{AssociatedDeviceMACAddress,{{tString,ASSOCIATEDDEVICEMACADDRESS_LEN}},
	  NULL,
	  getAssociatedDeviceMACAddress,
	  NULL,NULL},
	{NULL}
};
TRxObjNode AssociatedDeviceDescObj[] = {    
    {instanceIDMASK,{{0}},
	  NULL,
	  NULL,
	  AssociatedDeviceDesc},
};


TRxObjNode WEPKeyDesc[]={
	{WEPKey,{{tString,WEPKEY_LEN}},
	  setWLANConfigurationWEPKey,
	  getWLANConfigurationWEPKey,
	  NULL,NULL},
	{NULL}
};
TRxObjNode WEPKeyDescObj[] = {    
    {instanceIDMASK,{{0}},
	  NULL,
	  NULL,
	  WEPKeyDesc},
};


TRxObjNode PreSharedKeyDesc[]={
	{PreSharedKey,{{tString,PRESHAREDKEY_LEN}},
	  setWLANConfigurationPreSharedKey,
	  getWLANConfigurationPreSharedKey,
	  NULL,NULL},
	{NULL}
};
TRxObjNode PreSharedKeyDescObj[] = {    
    {instanceIDMASK,{{0}},
	  NULL,
	  NULL,
	  PreSharedKeyDesc},
};


TRxObjNode LANDeviceWLANConfigurationDesc[] = { 
	{SSIDAdvertisementEnabled,{{tBool,0,0}}, 
	 setSSIDAdvertisementEnabled,
	 getSSIDAdvertisementEnabled,
	NULL,NULL},

	{Enable,{{tBool,0,0}}, 
	 setWLANConfigurationEnable,
	 getWLANConfigurationEnable,
	NULL,NULL},

	{Status,{{tString,WLANCONFIGURATIONSTATUS_LEN}},
	  NULL,
	  getWLANConfigurationStatus,
	  NULL,NULL},

	{BSSID,{{tString,WLANCONFIGURATIONBSSID_LEN}},
	  NULL,
	  getWLANConfigurationBSSID,
	  NULL,NULL},

	{MaxBitRate,{{tString,MAXBITRATE_LEN}},
	  setWLANConfigurationMaxBitRate,
	  getWLANConfigurationMaxBitRate,
	  NULL,NULL},
	{Channel,{{tInt,0,0}},
	  setWLANConfigurationChannel,
	  getWLANConfigurationChannel,
	  NULL,NULL},

	{AutoChannelEnable,{{tBool,0,0}}, 
	 setWLANConfigurationAutoChannelEnable,
	 getWLANConfigurationAutoChannelEnable,
	NULL,NULL},	
	{RegulatoryDomain,{{tString,REGULATORYDOMAIN_LEN}},
	  setWLANConfigurationRegulatoryDomain,
	  getWLANConfigurationRegulatoryDomain,
	  NULL,NULL},
	
	{SSID,{{tString,SSID_LEN}},
	  setWLANConfigurationSSID,
	  getWLANConfigurationSSID,
	  NULL,NULL},

	{BeaconType,{{tString,BEACONTYPE_LEN}},
	  setWLANConfigurationBeaconType,
	  getWLANConfigurationBeaconType,
	  NULL,NULL},

	{MACAddressControlEnabled,{{tBool,0,0}}, 
	 setWLANConfigurationMACAddressControlEnabled,
	 getWLANConfigurationMACAddressControlEnabled,
	NULL,NULL},
	
	{Standard,{{tString,STANDARD_LEN}},
	  setWLANConfigurationStandard,
	  getWLANConfigurationStandard,
	  NULL,NULL},

	{WEPKeyIndex,{{tInt,0,0}},
	  setWLANConfigurationWEPKeyIndex,
	  getWLANConfigurationWEPKeyIndex,
	  NULL,NULL},

	
	{WEPEncryptionLevel,{{tString,WEPENCRYPTIONLEVEL_LEN}},
	  setWLANConfigurationWEPEncryptionLevel,
	  getWLANConfigurationWEPEncryptionLevel,
	  NULL,NULL},

	{BasicEncryptionModes,{{tString,BASICENCRYPTIONMODES_LEN}},
	  setWLANConfigurationBasicEncryptionModes,
	  getWLANConfigurationBasicEncryptionModes,
	  NULL,NULL},

	{BasicAuthenticationMode,{{tString,BASICAUTHENTICATIONMODE_LEN}},
	  setWLANConfigurationBasicAuthenticationMode,
	  getWLANConfigurationBasicAuthenticationMode,
	  NULL,NULL},


	{WPAEncryptionModes,{{tString,WPAENCRYPTIONMODES_LEN}},
	  setWLANConfigurationWPAEncryptionModes,
	  getWLANConfigurationWPAEncryptionModes,
	  NULL,NULL},

	{WPAAuthenticationMode,{{tString,WPAAUTHENTICATIONMODE_LEN}},
	  setWLANConfigurationWPAAuthenticationMode,
	  getWLANConfigurationWPAAuthenticationMode,
	  NULL,NULL},


	{IEEE11iEncryptionModes,{{tString,IEEE11IENCRYPTIONMODES_LEN}},
	  setWLANConfigurationIEEE11iEncryptionModes,
	  getWLANConfigurationIEEE11iEncryptionModes,
	  NULL,NULL},

	{IEEE11iAuthenticationMode,{{tString,IEEE11IAUTHENTICATIONMODE_LEN}},
	  setWLANConfigurationIEEE11iAuthenticationMode,
	  getWLANConfigurationIEEE11iAuthenticationMode,
	  NULL,NULL},


	{TotalAssociations,{{tUnsigned,0}},
	  NULL,
	  getWLANConfigurationTotalAssociations,
	  NULL,NULL},

	{TotalBytesSent,{{tUnsigned,0}},
	  NULL,
	  getWLANConfigurationTotalBytesSent,
	  NULL,NULL},

	{TotalBytesReceived,{{tUnsigned,0}},
	  NULL,
	  getWLANConfigurationTotalBytesReceived,
	  NULL,NULL},
		

	{TotalPacketsSent,{{tUnsigned,0}},
	  NULL,
	  getWLANConfigurationTotalPacketsSent,
	  NULL,NULL},

	{TotalPacketsReceived,{{tUnsigned,0}},
	  NULL,
	  getWLANConfigurationTotalPacketsReceived,
	  NULL,NULL},

	{WMMEnable,{{tBool,0,0}}, 
	 setWLANConfigurationWMMEnable,
	 getWLANConfigurationWMMEnable,
	NULL,NULL},

	{X_ATP_MixedEncryptionModes,{{tString,X_ATP_MIXEDENCRYPTIONMODES_LEN}},
	  setWLANConfigurationX_ATP_MixedEncryptionModes,
	  getWLANConfigurationX_ATP_MixedEncryptionModes,
	  NULL,NULL},

	{X_ATP_MixedAuthenticationMode,{{tString,X_ATP_MIXEDAUTHENTICATIONMODE_LEN}},
	  setWLANConfigurationX_ATP_MixedAuthenticationMode,
	  getWLANConfigurationX_ATP_MixedAuthenticationMode,
	  NULL,NULL},
	{X_ATP_ChannelUsed,{{tInt,0,0}},
	  NULL,
	  getWLANConfigurationX_ATP_ChannelUsed,
	  NULL,NULL},

	{X_ATP_WLANVersion,{{tString,X_ATP_WLANVERSION_LEN}},
	  NULL,
	  getWLANConfigurationX_ATP_WLANVersion,
	  NULL,NULL},
	{X_ATP_PowerValue,{{tUnsigned,0}},
	  setWLANConfigurationX_ATP_PowerValue,
	  getWLANConfigurationX_ATP_PowerValue,
	  NULL,NULL}, 

	{X_ATP_TotalBytesSentError,{{tUnsigned,0}},
	  NULL,
	  getWLANConfigurationX_ATP_TotalBytesSentError,
	  NULL,NULL}, 

	{X_ATP_TotalBytesReceivedError,{{tUnsigned,0}},
	  NULL,
	  getWLANConfigurationX_ATP_TotalBytesReceivedError,
	  NULL,NULL},

	{X_ATP_TotalPacketsSentError,{{tUnsigned,0}},
	  NULL,
	  getWLANConfigurationX_ATP_TotalPacketsSentError,
	  NULL,NULL},

	{X_ATP_TotalPacketsReceivedError,{{tUnsigned,0}},
	  NULL,
	  getWLANConfigurationX_ATP_TotalPacketsReceivedError,
	  NULL,NULL},
		
	{X_ATP_AssociateDeviceNum,{{tUnsigned,0}},
	  setWLANConfigurationX_ATP_AssociateDeviceNum,
	  getWLANConfigurationX_ATP_AssociateDeviceNum,
	  NULL,NULL},

	{X_ATP_WlanMacFilterpolicy,{{tBool,0,0}}, 
	 setWLANConfigurationX_ATP_WlanMacFilterpolicy,
	 getWLANConfigurationX_ATP_WlanMacFilterpolicy,
	NULL,NULL},

    {X_ATP_WlanMacFilternum,{{tUnsigned,0}},
	  NULL,
	  getWLANConfigurationX_ATP_WlanMacFilternum,
	  NULL,NULL},

 	{X_ATP_WlanStaWakeEnable,{{tBool,0,0}}, 
 	 setWLANConfigurationX_ATP_WlanStaWakeEnable,
 	 getWLANConfigurationX_ATP_WlanStaWakeEnable,
 	NULL,NULL},

	
 	{X_ATP_WlanIsolateControl,{{tBool,0,0}}, 
 	 setWLANConfigurationX_ATP_WlanIsolateControl,
 	 getWLANConfigurationX_ATP_WlanIsolateControl,
 	NULL,NULL},

	{X_ATP_Wlan11NBWControl,{{tString,X_ATP_WLAN11NBWCONTROL_LEN}},
	  setWLANConfigurationX_ATP_Wlan11NBWControl,
	  getWLANConfigurationX_ATP_Wlan11NBWControl,
	  NULL,NULL},

	{X_ATP_Wlan11NGIControl,{{tString,X_ATP_WLAN11NGICONTROL_LEN}},
	  setWLANConfigurationX_ATP_Wlan11NGIControl,
	  getWLANConfigurationX_ATP_Wlan11NGIControl,
	  NULL,NULL},

	{X_ATP_Wlan11NHtMcs,{{tInt,0,0}},
	  setWLANConfigurationX_ATP_Wlan11NHtMcs,
	  getWLANConfigurationX_ATP_Wlan11NHtMcs,
	  NULL,NULL},

	{X_ATP_Wlan11NTxRxStream,{{tInt,0,0}},
	  NULL,
	  getWLANConfigurationX_ATP_Wlan11NTxRxStream,
	  NULL,NULL},

	{WPS,{{tObject,0}},
	  NULL,
	  NULL,
	  WPSDesc},

	
	{X_ATP_WlanMacFilter,{{tObject,0}},
	  NULL,
	  NULL,
	  X_ATP_WlanMacFilterDescObj},

	{AssociatedDevice,{{tObject,0}},
	  NULL,
	  NULL,
	 AssociatedDeviceDescObj},

	
	{WEPKey,{{tObject,0}},
	  NULL,
	  NULL,
	WEPKeyDescObj},

	{PreSharedKey,{{tObject,0}},
	  NULL,
	  NULL,
	PreSharedKeyDescObj},

    {NULL}
};
TRxObjNode LANDeviceWLANConfigurationInstanceDesc[] = {    
    {instanceIDMASK,{{0}},
	  NULL,
	  NULL,
	  LANDeviceWLANConfigurationDesc},
};


/* InternetGatewayDevice.LANDevice.1. */
TRxObjNode LANDeviceDesc[] = {

	{LANEthernetInterfaceConfig,{{tInstance,0}},
	  NULL,
	  NULL,
	  LANDeviceLANEthernetInterfaceConfigInstanceDesc},

	{LANHostConfigManagement,{{tObject,0}},
	  NULL,
	  NULL,
	  LANDeviceLANHostConfigManagementDesc},

	 {LANEthernetInterfaceNumberOfEntries,{{tUnsigned,0}},
	  NULL,
	  getLANDevice_LANEthernetInterfaceNumberOfEntries,
	  NULL,NULL},
	 {LANWLANConfigurationNumberOfEntries,{{tUnsigned,0}},
	  NULL,
	  getLANDevice_LANWLANConfigurationNumberOfEntries,
	  NULL,NULL},
#if 0
	{X_ATP_AutoConfig,{{tBool,0,0}}, setX_ATP_AutoConfig,getX_ATP_AutoConfig,NULL,NULL},
	

	{LANWLANConfigurationNumberOfEntries,{{tUnsigned,0}},
	  NULL,
	  getLANDevice_LANWLANConfigurationNumberOfEntries,
	  NULL,NULL},
	  
	{Hosts,{{tObject,0}},
	  NULL,
	  NULL,
	  LANDeviceHostsDesc},

  	{WLANConfiguration,{{tInstance,0}},
	  NULL,
	  NULL,
	  LANDeviceWLANConfigurationInstanceDesc},
#endif
	{NULL}
};

TRxObjNode  LANDeviceInstanceDesc[] = {
	{instanceIDMASK,{{0}},
	  NULL,
	  NULL,
	  LANDeviceDesc},
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




