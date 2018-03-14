/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : commonString.h
  版 本 号   : 初稿
  作    者   : zhangjiasi
  生成日期   : 2015年6月7日
  最近修改   :
  功能描述   : 对于一些名称一样的节点，统一定义在该处
  函数列表   :
  修改历史   :
  1.日    期   : 2015年6月7日
    作    者   : zhangjiasi
    修改内容   : 创建文件

******************************************************************************/
#ifndef __COMMONSTRING_H__
#define __COMMONSTRING_H__
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


/****************************************************************************************************/
/****************************************** root related     *********************************/
/****************************************************************************************************/
 /* InternetGatewayDevice. */
SVAR(InternetGatewayDevice);
/* InternetGatewayDevice.LANDeviceNumberOfEntries */
SVAR(LANDeviceNumberOfEntries);
SVAR(WANDeviceNumberOfEntries);
/* InternetGatewayDevice.DeviceSummary */
SVAR(DeviceSummary);
/*add by duanguiyuan 20150915 
	InternetGatewayDevice.
*/
SVAR(X_CMCC_WANDeviceNumberOfEntries);
SVAR(X_CMCC_NetRegStatus);
SVAR(X_CMCC_GprsRegStatus);
SVAR(X_CMCC_EpsRegStatus);
SVAR(X_CMCC_CurrentNetwork);
SVAR(X_CMCC_CurrentNetmode);
SVAR(X_CMCC_NetworkPriority);
SVAR(X_CMCC_SingalLevel);
SVAR(X_CMCC_Txpower);
SVAR(X_CMCC_FrequencyPoint);
SVAR(X_CMCC_Band);
SVAR(X_CMCC_BandWidth);
SVAR(X_CMCC_EnodeBId);
SVAR(X_CMCC_CellId);
SVAR(X_CMCC_GlobeCellId);
SVAR(X_CMCC_PhysicsCellId);
SVAR(X_CMCC_ICCID);
SVAR(X_CMCC_APN);
SVAR(X_CMCC_IsFrequencyLock);
SVAR(X_CMCC_FrequencyLocking);
SVAR(X_CMCC_FrequencyLockInfo);
SVAR(X_CMCC_IsCellLock);
SVAR(X_CMCC_CellLockType);
SVAR(X_CMCC_CellLocking);
SVAR(X_CMCC_CellLockInfo);
SVAR(X_CMCC_LockPin);
SVAR(X_CMCC_LockPinType);
SVAR(X_CMCC_FirstPin);
SVAR(X_CMCC_FixedPin);
SVAR(X_CMCC_EncryptCard);
SVAR(X_CMCC_EncryptCardKey);
SVAR(X_CMCC_RSRP);
SVAR(X_CMCC_RSRQ);
SVAR(X_CMCC_SINR);
SVAR(X_CMCC_UpstreamTotalByte);
SVAR(X_CMCC_DownstreamTotalByte);
SVAR(X_CMCC_StartTime);
SVAR(X_CMCC_LoadAverage);
SVAR(X_CMCC_DeviceMemory);
SVAR(X_CMCC_MTU);

/*add by duanguiyuan   */
/* InternetGatewayDevice.DeviceInfo. */
SVAR(DeviceInfo);
/*---------------add by wangminglin------------------*/
SVAR(DeviceConfig);
SVAR(X_ATP_EXT);


/* InternetGatewayDevice.UserInfo. */
SVAR(X_CMCC_UserInfo);

/* InternetGatewayDevice.ManagementServer. */
SVAR(ManagementServer);
/* InternetGatewayDevice.Time. */
SVAR(Time);
/* InternetGatewayDevice.LANConfigSecurity. */
SVAR(LANConfigSecurity);

/* InternetGatewayDevice.LANDevice. */
SVAR(LANDevice);
/* InternetGatewayDevice.WANDevice. */
SVAR(WANDevice);
/* InternetGatewayDevice.IPPingDiagnostics. */
SVAR(IPPingDiagnostics);
/* InternetGatewayDevice.TraceRouteDiagnostics. */
SVAR(TraceRouteDiagnostics);
/* InternetGatewayDevice.X_CMCC_PPPOE_EMULATOR. */
SVAR(X_CMCC_PPPOE_EMULATOR);
/* InternetGatewayDevice.UserInterface */
//SVAR(UserInterface);
/* InternetGatewayDevice.Layer3Forwarding. */
SVAR(Layer3Forwarding);
/* InternetGatewayDevice.Layer2Bridging */
SVAR(Layer2Bridging);
/* InternetGatewayDevice.QueueManagement */
SVAR(QueueManagement);
/* InternetGatewayDevice.Services. */
SVAR(Services);
SVAR(VoiceService);
SVAR(X_PPPoE2L2TP);


/****************************************************************************************************/
/****************************************** system related     *********************************/
/****************************************************************************************************/
/*InternetGatewayDevice.X_DeviceInfo.*/
SVAR(X_DeviceInfo);
/* InternetGatewayDevice.X_LteDevice. */
SVAR(X_LteDevice);

/* InternetGatewayDevice.DeviceInfo. */
SVAR(Manufacturer);
SVAR(ManufacturerOUI);
SVAR(ModelName);
SVAR(Description);
SVAR(ProductClass);
SVAR(SerialNumber);
SVAR(DeviceLog);
SVAR(ModemFirmwareVersion);
SVAR(ProvisioningCode);
SVAR(UpTime);
SVAR(SpecVersion);

SVAR(ProductType);
SVAR(AdditionalSoftwareVersion);
/*SVAR(FirstUseDate);*/
/*add by duanguiyuan 20150916 
	InternetGatewayDevice.info
*/
SVAR(HardwareVersion);
SVAR(SoftwareVersion);
SVAR(X_CMCC_IMEI);
SVAR(X_CMCC_ConfigVersion);
SVAR(X_CMCC_ModuleVersion);
SVAR(X_CMCC_ModuleType);
SVAR(X_ATP_Informations);
SVAR(imsi);
SVAR(sn);
/* InternetGatewayDevice.DeviceInfo.X_CMCC_TeleComAccount  */
SVAR(X_CMCC_TeleComAccount);
SVAR(Enable);
//SVAR(Username);
//SVAR(Password);
/* InternetGatewayDevice.DeviceInfo.X_CMCC_Monitor  */
SVAR(X_CMCC_Monitor);
//SVAR(Enable);
SVAR(MonitorNumberOfEntries);

/* InternetGatewayDevice.DeviceInfo.X_CMCC_Monitor.MonitorConfig.i. */
SVAR(MonitorConfig);
SVAR(ParaList);
SVAR(TimeList);
/* InternetGatewayDevice.DeviceInfo.X_CMCC_Alarm  */
SVAR(X_CMCC_Alarm);
SVAR(AlarmNumber);
	
/* InternetGatewayDevice.UserInfo  */
//SVAR(Password);
SVAR(Status);
SVAR(Limit);
SVAR(Times);
SVAR(Result);
SVAR(ServiceNum);
SVAR(ServiceName);

/* InternetGatewayDevice.X_DeviceInfo. */
SVAR(DeviceStatus);
SVAR(UserName);
SVAR(LoadAverage);
SVAR(Memory);
/****************************************************************************************************/
/****************************************** time related     *********************************/
/****************************************************************************************************/

/* InternetGatewayDevice.Time. */

//SVAR(Enable);
//SVAR(Status);

SVAR(NTPServer1);
SVAR(NTPServer2);
SVAR(CurrentLocalTime);
SVAR(LocalTimeZone);
SVAR(LocalTimeZoneName);
SVAR(ConfigType);
SVAR(X_ATP_MannualTime);



/****************************************************************************************************/
/****************************************** lan device related     *********************************/
/****************************************************************************************************/
/* InternetGatewayDevice.LANConfigSecurity. */
SVAR(ConfigPassword);

/* InternetGatewayDevice.LANDevice. */
/* InternetGatewayDevice.LANDevice.1. */
SVAR(LANEthernetInterfaceNumberOfEntries);
SVAR(LANWLANConfigurationNumberOfEntries);
SVAR(LANHostConfigManagement);
SVAR(LANEthernetInterfaceConfig);
SVAR(Hosts);

/*add by wangminglin*/
SVAR(X_ATP_AutoConfig);


/* InternetGatewayDevice.LANDevice.1.LANHostConfigManagement. */
//SVAR(MACAddress);//define in commonString.h
SVAR(DHCPServerConfigurable);
SVAR(DHCPServerEnable);
SVAR(DHCPRelay);
SVAR(MinAddress);
SVAR(MaxAddress);
SVAR(ReservedAddresses);
SVAR(SubnetMask);
//SVAR(DNSServers);
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
//SVAR(Enable);//define in commonString.h
SVAR(IPInterfaceIPAddress);
SVAR(IPInterfaceSubnetMask);
SVAR(IPInterfaceAddressingType);
SVAR(X_ATP_RouteProtocolRx);
SVAR(X_ATP_RIPOperation);
SVAR(X_ATP_Pre_RouteProtocol);


/* InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1. */
//SVAR(Enable);define in commonString.h
//SVAR(Status);
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
//SVAR(Host);define in commonString.h
SVAR(IPAddress);
//SVAR(MACAddress);
SVAR(HostName);
SVAR(InterfaceType);

/****************************************************************************************************/
/******************************************wan device  related     *********************************/
/******************************  changed by duanguiyuan 20150917   *********************************/
/****************************************************************************************************/
/* InternetGatewayDevice.WANDevice.i.WANConnectionDevice.i.WANPPPnection.i. */
//SVAR(Enable);
SVAR(Name);
SVAR(Uptime);
SVAR(LastConnectionError);
//SVAR(Username);
//SVAR(Password);  
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
//SVAR(X_CMCC_UpstreamTotalByte);
//SVAR(X_CMCC_DownstreamTotalByte);

#if 1
/* InternetGatewayDevice.WANDevice.{i}. */
SVAR(WANConnectionNumberOfEntries);
SVAR(WANCommonInterfaceConfig);
SVAR(WANEthernetInterfaceConfig);

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
//SVAR(Username);
//SVAR(Password);
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
//SVAR(Enable);define in commonString.h
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
//SVAR(SubnetMask);
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
SVAR(RSRQ);
//SVAR(SINR);
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
//SVAR(Stats);
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
/* InternetGatewayDevice.IPPingDiagnostics. */
SVAR(DiagnosticsState);
SVAR(Interface);
SVAR(Host);
SVAR(NumberOfRepetitions);
SVAR(Timeout);
SVAR(DataBlockSize);
SVAR(DSCP);
SVAR(SuccessCount);
SVAR(FailureCount);
SVAR(AverageResponseTime);
SVAR(MinimumResponseTime);
SVAR(MaximumResponseTime);
SVAR(PingLog);

/****************************************************************************************************/
/****************************************** trace route related     *********************************/
/****************************************************************************************************/
/* InternetGatewayDevice.TraceRouteDiagnostics. */
//SVAR(DiagnosticsState);
//SVAR(Interface);
//SVAR(Host);
SVAR(NumberOfTries);
//SVAR(Timeout);
//SVAR(DataBlockSize);
//SVAR(DSCP);
SVAR(MaxHopCount);
SVAR(ResponseTime);
SVAR(HopsNumberOfEntries);
SVAR(TraceLog);
SVAR(RouteHops);

SVAR(HopHost);
SVAR(HopHostAddress);
SVAR(HopErrorCode);
SVAR(HopRTTimes);

/****************************************************************************************************/
/****************************************** X_CMCC_PPPOE_EMULATOR related     *********************************/
/****************************************************************************************************/
//SVAR(Username);
//SVAR(Password);
SVAR(WANInterface);
SVAR(PPPAuthenticationProtocol);
SVAR(RetryTimes);
//SVAR(Result);
SVAR(PPPSessionID);
//SVAR(ExternalIPAddress);
//SVAR(DefaultGateway);
SVAR(PPPOEEmulatorlog);
/****************************************************************************************************/
/****************************************** lte device related     **********************************/
/****************************************************************************************************/
/* InternetGatewayDevice.X_LteDevice. */
/* InternetGatewayDevice.X_LteDevice.Status. */
//SVAR(X_LteDevice);
//SVAR(Status);
SVAR(RadioSignalStrength);
SVAR(Txpower);
SVAR(IMEI);
SVAR(IMSI);
//SVAR(RSRP);
//SVAR(RSRQ);
SVAR(SINR);
SVAR(Band);
SVAR(BandWidth);
SVAR(SIMSTATUS);
SVAR(RadioMode);
SVAR(CellLocking);
SVAR(FrequencyLocking);
SVAR(RadioFrenquency);
//SVAR(RSSI);
SVAR(RegistrationStatus);
SVAR(NetRegStatus);
SVAR(GprsRegStatus);
SVAR(EpsRegStatus);
SVAR(RadioInterference);
SVAR(NetworkName);
SVAR(EnodeBId);
SVAR(GlobeCellId);
SVAR(PhysicsCellId);
SVAR(Iccid);
//SVAR(CellID);
SVAR(APNStatus);
SVAR(InNetWorkState);

/* InternetGatewayDevice.X_PPPoE2L2TP. */
//SVAR(Enable);
SVAR(MTUOfUser);
SVAR(L2TPServerNumberOfEntries);
SVAR(L2TPServer);
/* InternetGatewayDevice.X_PPPoE2L2TP_i. */

SVAR(Device);

SVAR(TunnelMode);
SVAR(LNSServerIPAddress);
SVAR(LNSServerHostName);
SVAR(LNSServerPortName);
SVAR(TunnelAuthenticateEnable);
SVAR(PasswordOfTunnel);




/*------------------------add by wangminglin---------------------------------------------------*/
/* InternetGatewayDevice.Layer3Forwarding. */

SVAR(ForwardNumberOfEntries);
SVAR(Forwarding);


/* InternetGatewayDevice.Layer3Forwarding.Forwarding.{i}. */

SVAR(DestIPAddress);
SVAR(DestSubnetMask);
SVAR(GatewayIPAddress);
//SVAR(Interface);



/* InternetGatewayDevice.X_ATP_SystemMonitor. */

SVAR(X_ATP_SystemMonitor);
SVAR(RsrpWeakBoundary);
SVAR(FreqLock);
SVAR(FreqLock_Mode);
SVAR(FreqBand_Info);
SVAR(FreqLock_FreqValue);
SVAR(FreqLock_Cell);

/* InternetGatewayDevice.X_ATP_Vxlan. */
SVAR(X_ATP_Vxlan);

/*InternetGatewayDevice.X_ATP_SystemMonitor.LanCheck.*/
//SVAR(LanCheck);
//SVAR(LanCheckHost);
//SVAR(LanCheckStatus);

/*InternetGatewayDevice.X_ATP_SystemMonitor.WanCheck.*/
//SVAR(WanCheck);
//SVAR(WanCheckHost);
//SVAR(WanCheckStatus);

/*InternetGatewayDevice.X_ATP_SystemMonitor.SysMonitor.*/
SVAR(SysMonitor);
//SVAR(SysMonitorStatus);

/*InternetGatewayDevice.FreqLock.FreqLData.*/
SVAR(FreqLData);
/* InternetGatewayDevice.X_ATP_Vxlan.VlanMapVniList. */
SVAR(VlanMapVniList);

/* InternetGatewayDevice.X_ATP_Vxlan.VlanMapDscpList. */
SVAR(VlanMapDscpList);

/* InternetGatewayDevice.X_ATP_Vxlan.VXLAN_LAC. */
SVAR(VXLAN_LAC);
SVAR(VxlanRemoteIpAddress);

/* InternetGatewayDevice.X_ATP_L2Gre. */
SVAR(X_ATP_L2Gre);
/* InternetGatewayDevice.X_ATP_L2Gre.IpMapDscpList. */
SVAR(IpMapDscpList);

/* InternetGatewayDevice.X_ATP_L2Gre.L2GRE_LAC. */
SVAR(L2GRE_LAC);
SVAR(GreRemoteIpAddress);
SVAR(VlanEnable);
SVAR(DscpMarkMode);
SVAR(ARPDscpMark);
SVAR(IPMapVlanPrioEnable);
/* InternetGatewayDevice.X_ATP_L2Gre.IpMapVlanPrioList. */
SVAR(IpMapVlanPrioList);

/* InternetGatewayDevice.X_ATP_L2Gre.VlanSettings. */
SVAR(VlanSettings);
SVAR(VlanType);
SVAR(AllowedPvidForwardEnable);
SVAR(Pvid);

/* InternetGatewayDevice.X_ATP_L2Gre.VlanPrioMapDscpList. */
SVAR(VlanPrioMapDscpList);

/* InternetGatewayDevice.X_ATP_L2Gre.VlanList. */
SVAR(VlanList);

/* InternetGatewayDevice.X_ATP_ModemLog. */
SVAR(X_ATP_ModemLog);
//SVAR(Status);

/* InternetGatewayDevice.X_ATP_PreserveData.{i}. */
SVAR(X_ATP_PreserveData);
SVAR(ParameterName);


/* InternetGatewayDevice.UserInterface.. */
SVAR(UserInterface);

/* InternetGatewayDevice.X_ATP_FireWall. */
SVAR(X_ATP_FireWall);
SVAR(CurrentLevel);

/* InternetGatewayDevice.X_ATP_VPN. */
SVAR(X_ATP_VPN);

/* InternetGatewayDevice.X_ATP_AlarmInfo. */
SVAR(X_ATP_AlarmInfo);
SVAR(SequenceNo);

/* InternetGatewayDevice.X_ATP_AlarmInfo.AlarmInstance. */
SVAR(AlarmInstance);
SVAR(LiftDescription);
/* InternetGatewayDevice.X_ATP_VPN.L2TP_LAC. */
SVAR(L2TP_LAC);
//SVAR(Password);
SVAR(BCPEnable);
SVAR(WanInterface);
SVAR(LNSAddress);
SVAR(PCAuthMode);
SVAR(PppUser);
SVAR(PppPass);
SVAR(KeepAliveTime);
/* InternetGatewayDevice.X_ATP_Config. */
SVAR(X_ATP_Config);

/* InternetGatewayDevice.X_ATP_Config.dialup. */
SVAR(dialup);
SVAR(roam_connect);
SVAR(current_profile);
SVAR(profile);
/* InternetGatewayDevice.X_ATP_Config.dialup.profile. */
//SVAR(index);
SVAR(profile_name);
SVAR(apn);
SVAR(username);
SVAR(password);
SVAR(auth_mode);
/* InternetGatewayDevice.Services.VoiceService.X_ATP_CommonConfig. */
SVAR(X_ATP_CommonConfig);
SVAR(X_ATP_Voipcsselect);
SVAR(X_ATP_CIDSendType);
/* InternetGatewayDevice.Services.VoiceService.X_ATP_CommonConfig.Button. */
SVAR(Button);
/* InternetGatewayDevice.Services.VoiceService. */
SVAR(X_ATP_UmtsDTMFMethod);

/* InternetGatewayDevice.Services.VoiceService.VoiceProfile. */
SVAR(VoiceProfile);
SVAR(DTMFMethod);
/* InternetGatewayDevice.Services.VoiceService.VoiceProfile.Line. */
SVAR(Line);
SVAR(X_ATP_ClipEnable);
SVAR(DirectoryNumber);
/* InternetGatewayDevice.Services.VoiceService.VoiceProfile.Line.SIP. */
SVAR(SIP);
SVAR(AuthUserName);
SVAR(AuthPassword);
/* InternetGatewayDevice.Services.VoiceService.VoiceProfile.SIP. */
SVAR(ProxyServer);
SVAR(ProxyServerPort);
SVAR(RegistrarServer);
SVAR(RegistrarServerPort);
SVAR(X_ATP_SIPDomain);
SVAR(X_SecondProxyServer);
SVAR(X_SecondProxyServerPort);
SVAR(X_SecondSIPDomain);
SVAR(X_SecondRegistrarServer);
SVAR(X_SecondRegistrarServerPort);
/* InternetGatewayDevice.Services.VoiceService.VoiceProfile.Line.Codec. */
SVAR(Codec);
/* InternetGatewayDevice.Services.VoiceService.VoiceProfile.Line.Codec.List. */
SVAR(List);
/*InternetGatewayDevice.X_ATP_FireWall.FilterMode.{i}.*/
SVAR(FilterMode);
SVAR(Level);
SVAR(MACMode);
SVAR(IPMode);
SVAR(URLMode);


/*InternetGatewayDevice.X_ATP_FireWall.UrlBlackFilter.{i}.*/
SVAR(UrlBlackFilter);
SVAR(Url);

/*InternetGatewayDevice.X_ATP_FireWall.MacBlackFilter.{i}.*/
SVAR(MacBlackFilter);
SVAR(SourceMACAddress);

/*InternetGatewayDevice.X_ATP_FireWall.IpBlackFilter.{i}.*/
SVAR(IpBlackFilter);
SVAR(SourceIPStart);
SVAR(SourceIPEnd);
SVAR(DestIPStart);
SVAR(DestIPEnd);
SVAR(Protocol);
SVAR(SourcePortStart);
SVAR(SourcePortEnd);
SVAR(DestPortStart);
SVAR(DestPortEnd);



/*InternetGatewayDevice.X_ATP_FireWall.UrlWhiteFilter.{i}.*/
SVAR(UrlWhiteFilter);
//SVAR(Url);

/*InternetGatewayDevice.X_ATP_FireWall.MacWhiteFilter.{i}.*/
SVAR(MacWhiteFilter);
//SVAR(SourceMACAddress);


/*InternetGatewayDevice.X_ATP_FireWall.IpWhiteFilter.{i}.*/
SVAR(IpWhiteFilter);

//SVAR(SourceIPStart);
//SVAR(WhiteFilterSourceIPEnd);
//SVAR(WhiteFilterDestIPStart);
//SVAR(WhiteFilterDestIPEnd);
//SVAR(WhiteFilterProtocol);
//SVAR(WhiteFilterSourcePortStart);
//SVAR(WhiteFilterSourcePortEnd);
//SVAR(WhiteFilterDestPortStart);
//SVAR(WhiteFilterDestPortEnd);


/*InternetGatewayDevice.X_ATP_FireWall.AccessControl.{i}.*/
SVAR(AccessControl);

SVAR(Service);
SVAR(Direction);
SVAR(StartIpAddr);
SVAR(EndIpAddr);
//SVAR(Enable);


SVAR(IpFilter);
SVAR(MacFilter);
SVAR(UrlFilter);
SVAR(Switch);

SVAR(FirewallMainSwitch);
SVAR(FirewallIPFilterSwitch);
SVAR(FirewallWanPortPingSwitch);
SVAR(FirewallWanWebSwitch);
SVAR(firewallmacfilterswitch);
SVAR(firewallurlfilterswitch);
SVAR(firewallfwstateswitch);
/*InternetGatewayDevice.UserInterface.X_ATP_Web.*/
SVAR(X_ATP_Web);

/*InternetGatewayDevice.UserInterface.X_ATP_Web.UserInfo.{i}.*/
SVAR(UserInfo);
//SVAR(Username);
SVAR(Userpassword);

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
//SVAR(TotalBytesSent);
//SVAR(TotalBytesReceived);
//SVAR(TotalPacketsSent);
//SVAR(TotalPacketsReceived);
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
//SVAR(WEPKey);

/*InternetGatewayDevice.LANDevice.{i}.WLANConfiguration.{i}.PreSharedKey.{i}.*/
SVAR(PreSharedKey);
//SVAR(PreSharedKey);




SVAR(X_ATP_IMEI);
SVAR(X_ATP_AntennaConfig);
SVAR(X_ATP_PCBVersion);
SVAR(X_ATP_LastSoftwareVersion);
SVAR(X_ATP_ModemProductName);
SVAR(X_ATP_SupportVpnNum);
SVAR(X_ATP_BOMCode);
SVAR(X_ATP_Temperature);
SVAR(X_OUI_PLATFORM_VER);
SVAR(X_OUI_PRIVATE_SOFTVER);

SVAR(X_ATP_PreserveEnable);
SVAR(ModemLogSessionStatus);
//SVAR(CellID);
SVAR(DownloadProgress);









#endif /* __COMMONSTRING_H__ */
