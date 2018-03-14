/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : lanDeviceParams.h
  版 本 号   : 初稿
  作    者   : zhangjiasi
  生成日期   : 2015年6月4日
  最近修改   :
  功能描述   : lanDeviceParams.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2015年6月4日
    作    者   : zhangjiasi
    修改内容   : 创建文件

******************************************************************************/
#ifndef __LANDEVICEPARAMS_H__
#define __LANDEVICEPARAMS_H__
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

/* InternetGatewayDevice.LANConfigSecurity. */
SVAR(ConfigPassword);

/* InternetGatewayDevice.LANDevice. */
/* InternetGatewayDevice.LANDevice.1. */
SVAR(LANEthernetInterfaceNumberOfEntries);
SVAR(LANWLANConfigurationNumberOfEntries);
SVAR(LANHostConfigManagement);
SVAR(LANEthernetInterfaceConfig);
SVAR(Hosts);
SVAR(X_ATP_AutoConfig);

/*InternetGatewayDevice.LANDevice.{i}.WLANConfiguration.{i}.*/
SVAR(WLANConfiguration);
SVAR(SSIDAdvertisementEnabled);
//SVAR(Enable);
//SVAR(Status);
SVAR(BSSID);
SVAR(MaxBitRate);
SVAR(Channel);
SVAR(AutoChannelEnable);
SVAR(RegulatoryDomain);
SVAR(SSID);
SVAR(BeaconType);
SVAR(MACAddressControlEnabled);
SVAR(Standard);
SVAR(WEPKeyIndex);
SVAR(WEPEncryptionLevel);
SVAR(BasicEncryptionModes);
SVAR(BasicAuthenticationMode);
SVAR(WPAEncryptionModes);
SVAR(WPAAuthenticationMode);
SVAR(IEEE11iEncryptionModes);
SVAR(IEEE11iAuthenticationMode);
SVAR(TotalAssociations);
SVAR(TotalBytesSent);
SVAR(TotalBytesReceived);
SVAR(TotalPacketsSent);
SVAR(TotalPacketsReceived);
SVAR(WMMEnable);
SVAR(X_ATP_MixedEncryptionModes);
SVAR(X_ATP_MixedAuthenticationMode);
SVAR(X_ATP_ChannelUsed);
SVAR(X_ATP_WLANVersion);
SVAR(X_ATP_PowerValue);
SVAR(X_ATP_TotalBytesSentError);
SVAR(X_ATP_TotalBytesReceivedError);
SVAR(X_ATP_TotalPacketsSentError);
SVAR(X_ATP_TotalPacketsReceivedError);
SVAR(X_ATP_AssociateDeviceNum);
SVAR(X_ATP_WlanMacFilterpolicy);
SVAR(X_ATP_WlanMacFilternum);
SVAR(X_ATP_WlanStaWakeEnable);
SVAR(X_ATP_WlanIsolateControl);
SVAR(X_ATP_Wlan11NBWControl);
SVAR(X_ATP_Wlan11NGIControl);
SVAR(X_ATP_Wlan11NHtMcs);
SVAR(X_ATP_Wlan11NTxRxStream);

/*InternetGatewayDevice.LANDevice.{i}.WLANConfiguration.{i}.WPS.*/
SVAR(WPS);
//SVAR(Enable);
SVAR(X_ATP_WPSMode);
SVAR(DevicePassword);

/*InternetGatewayDevice.LANDevice.{i}.WLANConfiguration.{i}.X_ATP_WlanMacFilter.{i}.*/
SVAR(X_ATP_WlanMacFilter);
SVAR(X_ATP_WlanSrcMacAddress);

SVAR(X_ATP_SSID1);
SVAR(X_ATP_SSID2);
SVAR(X_ATP_SSID3);
SVAR(X_ATP_SSID4);


/*InternetGatewayDevice.LANDevice.{i}.WLANConfiguration.{i}.AssociatedDevice.{i}.*/
SVAR(AssociatedDevice);

SVAR(AssociatedDeviceMACAddress);

/*InternetGatewayDevice.LANDevice.{i}.WLANConfiguration.{i}.WEPKey.{i}.*/
SVAR(WEPKey);
//SVAR(WLANConfigurationWEPKey);


/*InternetGatewayDevice.LANDevice.{i}.WLANConfiguration.{i}.PreSharedKey.{i}.*/
SVAR(PreSharedKey);
//SVAR(PreSharedKey);



/* InternetGatewayDevice.LANDevice.1.LANHostConfigManagement. */
SVAR(MACAddress);
SVAR(DHCPServerConfigurable);
SVAR(DHCPServerEnable);
SVAR(DHCPRelay);
SVAR(MinAddress);
SVAR(MaxAddress);
SVAR(ReservedAddresses);
SVAR(SubnetMask);
SVAR(DNSServers);
SVAR(DomainName);
SVAR(IPRouters);
SVAR(DHCPLeaseTime);
SVAR(AllowedMACAddresses);
SVAR(IPInterfaceNumberOfEntries);
SVAR(DHCPStaticAddressNumberOfEntries);
SVAR(X_ATP_VlanID);
SVAR(DHCPStaticAddress);
SVAR(IPInterface);

SVAR(PassthroughLease);
/*InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.DHCPRelaySeverConfig.*/
SVAR(DHCPRelaySeverConfig);
SVAR(DHCPRelayEnable);
SVAR(ServerIPAddr);

/* InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPStaticAddress.{i}. */
SVAR(DHCPStaticAddressEnable);
SVAR(DHCPStaticAddressChaddr);
SVAR(DHCPStaticAddressYiaddr);
SVAR(X_ATP_valid);



/* InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1 */
SVAR(Enable);
SVAR(IPInterfaceIPAddress);
SVAR(IPInterfaceSubnetMask);
SVAR(IPInterfaceAddressingType);
SVAR(X_ATP_RouteProtocolRx);
SVAR(X_ATP_RIPOperation);
SVAR(X_ATP_Pre_RouteProtocol);


/* InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1. */
//SVAR(Enable);define in commonString.h
SVAR(Status);
//SVAR(MACAddress);define in commonString.h
SVAR(Stats);


/* InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Stats. */
SVAR(BytesSent);
SVAR(BytesReceived);
SVAR(DiscardPacketsSent);
SVAR(DiscardPacketsReceived);
SVAR(PacketsSent);
SVAR(PacketsReceived);
SVAR(X_ATP_PacketsSentError);
SVAR(X_ATP_PacketsSentDrops);
SVAR(X_ATP_Timestamp);

SVAR(X_ATP_PacketsReceivedError);
SVAR(X_ATP_PacketsReceivedDrops);
SVAR(X_ATP_PacketsError);
SVAR(X_ATP_PacketsDrops);
SVAR(X_ATP_SentRate);
SVAR(X_ATP_ReceivedRate);
SVAR(ErrorsSent);
SVAR(ErrorsReceived);



/* InternetGatewayDevice.LANDevice.1.Hosts. */
SVAR(HostNumberOfEntries);
SVAR(Host);
SVAR(IPAddress);
SVAR(MACAddress);
SVAR(HostName);
SVAR(InterfaceType);


/* 函数原型定义 */
/* InternetGatewayDevice.LANConfigSecurity. */
TRXSFUNC(setConfigPassword);
TRXGFUNC(getConfigPassword);

/* InternetGatewayDevice.LANDevice.1.*/
TRXGFUNC(getLANDevice_LANEthernetInterfaceNumberOfEntries);
TRXGFUNC(getLANDevice_LANWLANConfigurationNumberOfEntries);
TRXSFUNC(setX_ATP_AutoConfig);
TRXGFUNC(getX_ATP_AutoConfig);




/* InternetGatewayDevice.LANDevice.1.LANHostConfigManagement. */
TRXGFUNC(getLANDeviceLANHostConfigManagement_MACAddress);
TRXSFUNC(setLANDeviceLANHostConfigManagement_DHCPServerConfigurable);
TRXGFUNC(getLANDeviceLANHostConfigManagement_DHCPServerConfigurable);
TRXSFUNC(setLANDeviceLANHostConfigManagement_DHCPServerEnable);
TRXGFUNC(getLANDeviceLANHostConfigManagement_DHCPServerEnable);
TRXGFUNC(getLANDeviceLANHostConfigManagement_DHCPRelay);
TRXSFUNC(setLANDeviceLANHostConfigManagement_MinAddress);
TRXGFUNC(getLANDeviceLANHostConfigManagement_MinAddress);
TRXSFUNC(setLANDeviceLANHostConfigManagement_MaxAddress);
TRXGFUNC(getLANDeviceLANHostConfigManagement_MaxAddress);
TRXSFUNC(setLANDeviceLANHostConfigManagement_ReservedAddresses);
TRXGFUNC(getLANDeviceLANHostConfigManagement_ReservedAddresses);
TRXSFUNC(setLANDeviceLANHostConfigManagement_SubnetMask);
TRXGFUNC(getLANDeviceLANHostConfigManagement_SubnetMask);
TRXSFUNC(setLANDeviceLANHostConfigManagement_DNSServers);
TRXGFUNC(getLANDeviceLANHostConfigManagement_DNSServers);
TRXSFUNC(setLANDeviceLANHostConfigManagement_DomainName);
TRXGFUNC(getLANDeviceLANHostConfigManagement_DomainName);
TRXSFUNC(setLANDeviceLANHostConfigManagement_IPRouters);
TRXGFUNC(getLANDeviceLANHostConfigManagement_IPRouters);
TRXSFUNC(setLANDeviceLANHostConfigManagement_DHCPLeaseTime);
TRXGFUNC(getLANDeviceLANHostConfigManagement_DHCPLeaseTime);
TRXSFUNC(setLANDeviceLANHostConfigManagement_PassthroughLease);
TRXGFUNC(getLANDeviceLANHostConfigManagement_PassthroughLease);
TRXSFUNC(setLANDeviceLANHostConfigManagement_AllowedMACAddresses);
TRXGFUNC(getLANDeviceLANHostConfigManagement_AllowedMACAddresses);
TRXSFUNC(setLANDeviceLANHostConfigManagement_IPInterfaceNumberOfEntries);
TRXGFUNC(getLANDeviceLANHostConfigManagement_IPInterfaceNumberOfEntries);

//TRXSFUNC(setLANDeviceLANHostConfigManagement_DHCPStaticAddressNumberOfEntries);
TRXGFUNC(getLANDeviceLANHostConfigManagement_DHCPStaticAddressNumberOfEntries);
TRXSFUNC(setLANDeviceLANHostConfigManagement_DHCPRelayEnable);
TRXGFUNC(getLANDeviceLANHostConfigManagement_DHCPRelayEnable);
TRXSFUNC(setLANDeviceLANHostConfigManagement_ServerIPAddr);
TRXGFUNC(getLANDeviceLANHostConfigManagement_ServerIPAddr);

TRXSFUNC(setLANDeviceLANHostConfigManagement_X_ATP_VlanID);
TRXGFUNC(getLANDeviceLANHostConfigManagement_X_ATP_VlanID);

/*InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPStaticAddress.{i}.*/
TRXSFUNC(setDHCPStaticAddressEnable);
TRXGFUNC(getDHCPStaticAddressEnable);

TRXSFUNC(setDHCPStaticAddressChaddr);
TRXGFUNC(getDHCPStaticAddressChaddr);

TRXSFUNC(setDHCPStaticAddressYiaddr);
TRXGFUNC(getDHCPStaticAddressYiaddr);

TRXSFUNC(setDHCPStaticAddressX_ATP_valid);
TRXGFUNC(getDHCPStaticAddressX_ATP_valid);



/* InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1 */
TRXSFUNC(setLANDeviceLANHostConfigManagementIPInterface_Enable);
TRXGFUNC(getLANDeviceLANHostConfigManagementIPInterface_Enable);
TRXSFUNC(setLANDeviceLANHostConfigManagementIPInterface_IPInterfaceIPAddress);
TRXGFUNC(getLANDeviceLANHostConfigManagementIPInterface_IPInterfaceIPAddress);
TRXSFUNC(setLANDeviceLANHostConfigManagementIPInterface_IPInterfaceSubnetMask);
TRXGFUNC(getLANDeviceLANHostConfigManagementIPInterface_IPInterfaceSubnetMask);
TRXSFUNC(setLANDeviceLANHostConfigManagementIPInterface_IPInterfaceAddressingType);
TRXGFUNC(getLANDeviceLANHostConfigManagementIPInterface_IPInterfaceAddressingType);

TRXSFUNC(setLANDeviceLANHostConfigManagementIPInterface_X_ATP_RouteProtocolRx);
TRXGFUNC(getLANDeviceLANHostConfigManagementIPInterface_X_ATP_RouteProtocolRx);

TRXSFUNC(setLANDeviceLANHostConfigManagementIPInterface_X_ATP_RIPOperation);
TRXGFUNC(getLANDeviceLANHostConfigManagementIPInterface_X_ATP_RIPOperation);

TRXSFUNC(setLANDeviceLANHostConfigManagementIPInterface_X_ATP_Pre_RouteProtocol);
TRXGFUNC(getLANDeviceLANHostConfigManagementIPInterface_X_ATP_Pre_RouteProtocol);



/* InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1. */
TRXSFUNC(setLANDeviceLANEthernetInterfaceConfig_Enable);
TRXGFUNC(getLANDeviceLANEthernetInterfaceConfig_Enable);
TRXGFUNC(getLANDeviceLANEthernetInterfaceConfig_Status);
TRXGFUNC(getLANDeviceLANEthernetInterfaceConfig_MACAddress);
/* InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Stats. */
TRXGFUNC(getLANDeviceLANEthernetInterfaceConfigStats_BytesSent);
TRXGFUNC(getLANDeviceLANEthernetInterfaceConfigStats_BytesReceived);
TRXGFUNC(getLANDeviceLANEthernetInterfaceConfigStats_PacketsSent);
TRXGFUNC(getLANDeviceLANEthernetInterfaceConfigStats_PacketsReceived);
TRXGFUNC(getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_PacketsSentError);
TRXGFUNC(getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_PacketsSentDrops);
TRXGFUNC(getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_Timestamp);

TRXGFUNC(getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_PacketsReceivedError);

TRXGFUNC(getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_PacketsReceivedDrops);
TRXGFUNC(getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_PacketsError);
TRXGFUNC(getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_PacketsDrops);

TRXGFUNC(getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_SentRate);

TRXGFUNC(getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_ReceivedRate);


/*add by wangminglin*/
/*InternetGatewayDevice.LANDevice.{i}.WLANConfiguration.{i}.*/
TRXSFUNC(setSSIDAdvertisementEnabled);
TRXGFUNC(getSSIDAdvertisementEnabled);

TRXSFUNC(setWLANConfigurationEnable);
TRXGFUNC(getWLANConfigurationEnable);

TRXGFUNC(getWLANConfigurationStatus);

TRXGFUNC(getWLANConfigurationBSSID);

TRXSFUNC(setWLANConfigurationMaxBitRate);
TRXGFUNC(getWLANConfigurationMaxBitRate);


TRXSFUNC(setWLANConfigurationChannel);
TRXGFUNC(getWLANConfigurationChannel);

TRXSFUNC(setWLANConfigurationAutoChannelEnable);
TRXGFUNC(getWLANConfigurationAutoChannelEnable);

TRXSFUNC(setWLANConfigurationRegulatoryDomain);
TRXGFUNC(getWLANConfigurationRegulatoryDomain);


TRXSFUNC(setWLANConfigurationSSID);
TRXGFUNC(getWLANConfigurationSSID);

TRXSFUNC(setWLANConfigurationBeaconType);
TRXGFUNC(getWLANConfigurationBeaconType);

TRXSFUNC(setWLANConfigurationMACAddressControlEnabled);
TRXGFUNC(getWLANConfigurationMACAddressControlEnabled);

TRXSFUNC(setWLANConfigurationStandard);
TRXGFUNC(getWLANConfigurationStandard);

TRXSFUNC(setWLANConfigurationWEPKeyIndex);
TRXGFUNC(getWLANConfigurationWEPKeyIndex);

TRXSFUNC(setWLANConfigurationWEPEncryptionLevel);
TRXGFUNC(getWLANConfigurationWEPEncryptionLevel);

TRXSFUNC(setWLANConfigurationBasicEncryptionModes);
TRXGFUNC(getWLANConfigurationBasicEncryptionModes);

TRXSFUNC(setWLANConfigurationBasicAuthenticationMode);
TRXGFUNC(getWLANConfigurationBasicAuthenticationMode);


TRXSFUNC(setWLANConfigurationWPAEncryptionModes);
TRXGFUNC(getWLANConfigurationWPAEncryptionModes);


TRXSFUNC(setWLANConfigurationWPAAuthenticationMode);
TRXGFUNC(getWLANConfigurationWPAAuthenticationMode);

TRXSFUNC(setWLANConfigurationIEEE11iEncryptionModes);
TRXGFUNC(getWLANConfigurationIEEE11iEncryptionModes);

TRXSFUNC(setWLANConfigurationIEEE11iAuthenticationMode);
TRXGFUNC(getWLANConfigurationIEEE11iAuthenticationMode);

TRXGFUNC(getWLANConfigurationTotalAssociations);

TRXGFUNC(getWLANConfigurationTotalBytesSent);

TRXGFUNC(getWLANConfigurationTotalBytesReceived);


TRXGFUNC(getWLANConfigurationTotalPacketsSent);

TRXGFUNC(getWLANConfigurationTotalPacketsReceived);


TRXSFUNC(setWLANConfigurationWMMEnable);
TRXGFUNC(getWLANConfigurationWMMEnable);

TRXSFUNC(setWLANConfigurationX_ATP_MixedEncryptionModes);
TRXGFUNC(getWLANConfigurationX_ATP_MixedEncryptionModes);

TRXSFUNC(setWLANConfigurationX_ATP_MixedAuthenticationMode);
TRXGFUNC(getWLANConfigurationX_ATP_MixedAuthenticationMode);

TRXGFUNC(getWLANConfigurationX_ATP_ChannelUsed);

TRXGFUNC(getWLANConfigurationX_ATP_WLANVersion);

TRXSFUNC(setWLANConfigurationX_ATP_PowerValue);
TRXGFUNC(getWLANConfigurationX_ATP_PowerValue);

TRXGFUNC(getWLANConfigurationX_ATP_TotalBytesSentError);

TRXGFUNC(getWLANConfigurationX_ATP_TotalBytesReceivedError);

TRXGFUNC(getWLANConfigurationX_ATP_TotalPacketsSentError);

TRXGFUNC(getWLANConfigurationX_ATP_TotalPacketsReceivedError);


TRXSFUNC(setWLANConfigurationX_ATP_AssociateDeviceNum);
TRXGFUNC(getWLANConfigurationX_ATP_AssociateDeviceNum);

TRXSFUNC(setWLANConfigurationX_ATP_WlanMacFilterpolicy);
TRXGFUNC(getWLANConfigurationX_ATP_WlanMacFilterpolicy);

TRXGFUNC(getWLANConfigurationX_ATP_WlanMacFilternum);

TRXSFUNC(setWLANConfigurationX_ATP_WlanStaWakeEnable);
TRXGFUNC(getWLANConfigurationX_ATP_WlanStaWakeEnable);

TRXSFUNC(setWLANConfigurationX_ATP_WlanIsolateControl);
TRXGFUNC(getWLANConfigurationX_ATP_WlanIsolateControl);

TRXSFUNC(setWLANConfigurationX_ATP_Wlan11NBWControl);
TRXGFUNC(getWLANConfigurationX_ATP_Wlan11NBWControl);

TRXSFUNC(setWLANConfigurationX_ATP_Wlan11NGIControl);
TRXGFUNC(getWLANConfigurationX_ATP_Wlan11NGIControl);

TRXSFUNC(setWLANConfigurationX_ATP_Wlan11NHtMcs);
TRXGFUNC(getWLANConfigurationX_ATP_Wlan11NHtMcs);

TRXGFUNC(getWLANConfigurationX_ATP_Wlan11NTxRxStream);


/*InternetGatewayDevice.LANDevice.{i}.WLANConfiguration.{i}.{i}.WPS.*/

TRXGFUNC(getWPSEnable);
TRXGFUNC(getWPSX_ATP_WPSMode);
TRXGFUNC(getWPSDevicePassword);


/*InternetGatewayDevice.LANDevice.{i}.WLANConfiguration.{i}.X_ATP_WlanMacFilter.{i}.*/
TRXSFUNC(setX_ATP_WlanSrcMacAddress);
TRXGFUNC(getX_ATP_WlanSrcMacAddress);

TRXSFUNC(setX_ATP_SSID1);
TRXGFUNC(getX_ATP_SSID1);

TRXSFUNC(setX_ATP_SSID2);
TRXGFUNC(getX_ATP_SSID2);

TRXSFUNC(setX_ATP_SSID3);
TRXGFUNC(getX_ATP_SSID3);

TRXSFUNC(setX_ATP_SSID4);
TRXGFUNC(getX_ATP_SSID4);

/*InternetGatewayDevice.LANDevice.{i}.WLANConfiguration.{i}.AssociatedDevice.{i}.*/

TRXGFUNC(getAssociatedDeviceMACAddress);


/*InternetGatewayDevice.LANDevice.{i}.WLANConfiguration.{i}.WEPKey.{i}.*/
TRXSFUNC(setWLANConfigurationWEPKey);
TRXGFUNC(getWLANConfigurationWEPKey);


/*InternetGatewayDevice.LANDevice.{i}.WLANConfiguration.{i}.PreSharedKey.{i}.*/
TRXSFUNC(setWLANConfigurationPreSharedKey);
TRXGFUNC(getWLANConfigurationPreSharedKey);




/* InternetGatewayDevice.LANDevice.1.Hosts. */
TRXGFUNC(getLANDeviceHosts_HostNumberOfEntries);
/* InternetGatewayDevice.LANDevice.1.Hosts.Host.{i} */
TRXGFUNC(getLANDeviceHostsHost_IPAddress);
TRXGFUNC(getLANDeviceHostsHost_MACAddress);
TRXGFUNC(getLANDeviceHostsHost_HostName);
TRXGFUNC(getLANDeviceHostsHost_InterfaceType);




#endif /* __LANDEVICEPARAMS_H__ */
