/*
 * Copyright (C) GBCOM ltd.,co 2015
 *
 * This is a part of project tr69cd, which implements TR069 protocal(aka
 * CWMP).
 * 
 * This is a PRIVATE software.
 * All rights reserved.
 */
 

#ifndef __DEVICEINFOPARAMS_H__
#define __DEVICEINFOPARAMS_H__

#include "tr69cdefs.h"

#ifdef CPEVARNAMEINSTANCE
#undef CPEVARNAMEINSTANCE
#endif
/* InternetGatewayDevice.DeviceInfo. */

SVAR(Manufacturer);
SVAR(ManufacturerOUI);
SVAR(ModelName);
SVAR(Description);
SVAR(ProductClass);
SVAR(SerialNumber);


SVAR(ModemFirmwareVersion);
SVAR(ProvisioningCode);
SVAR(UpTime);

SVAR(ProductType);
SVAR(AdditionalSoftwareVersion);
SVAR(DeviceLog);
SVAR(SpecVersion);

/*SVAR(FirstUseDate);*/
SVAR(HardwareVersion);
SVAR(SoftwareVersion);
SVAR(X_CMCC_IMEI);


/*add by wangminglin */
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
SVAR(CellID);
SVAR(DownloadProgress);
/* InternetGatewayDevice.DeviceInfo.X_ATP_Informations. */
SVAR(X_ATP_Informations);
SVAR(imsi);
SVAR(sn);

/* InternetGatewayDevice.Layer3Forwarding. */

SVAR(ForwardNumberOfEntries);
SVAR(Forwarding);

/* InternetGatewayDevice.Layer3Forwarding.Forwarding.{i}. */

SVAR(DestIPAddress);
SVAR(DestSubnetMask);
SVAR(GatewayIPAddress);
SVAR(Interface);


/* InternetGatewayDevice.X_ATP_SystemMonitor. */
SVAR(RsrpWeakBoundary);

/*InternetGatewayDevice.X_ATP_SystemMonitor.LanCheck.*/
SVAR(LanCheck);
SVAR(Host);
SVAR(Status);

/*InternetGatewayDevice.X_ATP_SystemMonitor.WanCheck.*/
SVAR(WanCheck);
//SVAR(Host);
//SVAR(Status);


/*InternetGatewayDevice.X_ATP_SystemMonitor.SysMonitor.*/
SVAR(SysMonitor);
//SVAR(Status);

/*InternetGatewayDevice.FreqLock.FreqLData.*/
SVAR(FreqLData);
SVAR(FreqLock_Mode);
SVAR(FreqBand_Info);
SVAR(FreqLock_FreqValue);
SVAR(FreqLock_Cell);
/* InternetGatewayDevice.X_ATP_Vxlan.VlanMapVniList. */
SVAR(VlanMapVniList);
/* InternetGatewayDevice.X_ATP_Vxlan.VlanMapDscpList. */
SVAR(VlanMapDscpList);
/* InternetGatewayDevice.X_ATP_Vxlan.VXLAN_LAC. */
SVAR(VXLAN_LAC);
SVAR(VxlanRemoteIpAddress);

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
/*InternetGatewayDevice.X_ATP_ModemLog.*/
//SVAR(Status);

/*InternetGatewayDevice.X_ATP_PreserveData.{i}.*/
SVAR(ParameterName);

/*InternetGatewayDevice.UserInterface.X_ATP_Web.*/
SVAR(X_ATP_Web);

/*InternetGatewayDevice.UserInterface.X_ATP_Web.UserInfo.{i}.*/
SVAR(UserInfo);
//SVAR(Username);
SVAR(Userpassword);

/*InternetGatewayDevice.X_ATP_FireWall.*/
SVAR(CurrentLevel);


/*InternetGatewayDevice.X_ATP_FireWall.FilterMode.{i}.*/
SVAR(FilterMode);
SVAR(Level);
SVAR(MACMode);
SVAR(IPMode);
SVAR(URLMode);

/* InternetGatewayDevice.X_ATP_VPN.L2TP_LAC. */
SVAR(L2TP_LAC);
SVAR(HostName);
SVAR(BCPEnable);
SVAR(WanInterface);
SVAR(LNSAddress);
SVAR(PCAuthMode);
SVAR(PppUser);
SVAR(PppPass);
SVAR(KeepAliveTime);

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

/* InternetGatewayDevice.Services.VoiceService. */
SVAR(VoiceService);
SVAR(X_ATP_UmtsDTMFMethod);

/* InternetGatewayDevice.Services.VoiceService.VoiceProfile. */
SVAR(VoiceProfile);
SVAR(DTMFMethod);
/* InternetGatewayDevice.Services.VoiceService.X_ATP_CommonConfig. */
SVAR(X_ATP_CommonConfig);
SVAR(X_ATP_Voipcsselect);
SVAR(X_ATP_CIDSendType);
/* InternetGatewayDevice.Services.VoiceService.X_ATP_CommonConfig.Button. */
SVAR(Button);
/* InternetGatewayDevice.Services.VoiceService.VoiceProfile.Line. */
SVAR(Line);
SVAR(X_ATP_ClipEnable);
SVAR(DirectoryNumber);

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
/* InternetGatewayDevice.Services.VoiceService.VoiceProfile.Line.SIP. */
SVAR(SIP);
SVAR(AuthUserName);
SVAR(AuthPassword);
/* InternetGatewayDevice.Services.VoiceService.VoiceProfile.Line.Codec.*/
SVAR(Codec);
/* InternetGatewayDevice.Services.VoiceService.VoiceProfile.Line.Codec.List. */
SVAR(List);
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

/*InternetGatewayDevice.X_ATP_FireWall.{i}.*/
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


SVAR(X_CMCC_ConfigVersion);
SVAR(X_CMCC_ModuleVersion);
SVAR(X_CMCC_ModuleType);
SVAR(X_CMCC_TeleComAccount);
/* InternetGatewayDevice.X_ATP_AlarmInfo. */
SVAR(SequenceNo);
/* InternetGatewayDevice.X_ATP_AlarmInfo.AlarmInstance. */
SVAR(AlarmInstance);
SVAR(LiftDescription);
/* InternetGatewayDevice.DeviceInfo.X_CMCC_TeleComAccount  */
SVAR(Enable);
SVAR(Username);
SVAR(Password);
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

/* InternetGatewayDevice.X_DeviceInfo. */
SVAR(DeviceStatus);
SVAR(UserName);
SVAR(LoadAverage);
SVAR(Memory);


/* 函数原型定义 */
/* InternetGatewayDevice.DeviceInfo. */
TRXGFUNC(getManufacturer);
TRXGFUNC(getManufacturerOUI);
TRXGFUNC(getModelName);
TRXGFUNC(getDescription);
TRXGFUNC(getProductClass);
TRXGFUNC(getSerialNumber);
TRXGFUNC(getCpeMac);
TRXGFUNC(getHardwareVersion);
TRXGFUNC(getSoftwareVersion);
TRXGFUNC(getModemFirmwareVersion);
TRXSFUNC(setProvisioningCode);
TRXGFUNC(getProvisioningCode);
TRXGFUNC(getUpTime);
TRXGFUNC(getFirstUseDate);
TRXGFUNC(getDeviceLog);
TRXGFUNC(getSpecVersion);
TRXGFUNC(getProductType);
TRXGFUNC(getAdditionalSoftwareVersion);

TRXGFUNC(getX_CMCC_IMEI);


/* --------------- add  by  wangminglin-----------------*/
TRXGFUNC(getX_ATP_IMEI);
TRXSFUNC(setX_ATP_AntennaConfig);
TRXGFUNC(getX_ATP_AntennaConfig);
TRXGFUNC(getX_ATP_PCBVersion);
TRXGFUNC(getX_ATP_LastSoftwareVersion);
TRXGFUNC(getX_ATP_ModemProductName);
TRXGFUNC(getX_ATP_SupportVpnNum);
TRXGFUNC(getX_ATP_BOMCode);
TRXGFUNC(getX_ATP_Temperature);
TRXGFUNC(getX_OUI_PLATFORM_VER);
TRXGFUNC(getX_OUI_PRIVATE_SOFTVER);

TRXSFUNC(setX_ATP_PreserveEnable);
TRXGFUNC(getX_ATP_PreserveEnable);
TRXGFUNC(getModemLogSessionStatus);
TRXGFUNC(getX_ATP_EXTCellID);
TRXGFUNC(getDownloadProgress);
TRXGFUNC(getForwardNumberOfEntries);


/* InternetGatewayDevice.Layer3Forwarding.Forwarding.{i}. */
TRXSFUNC(setForwardingDestIPAddress);
TRXGFUNC(getForwardingDestIPAddress);

TRXSFUNC(setForwardingDestSubnetMask);
TRXGFUNC(getForwardingDestSubnetMask);

TRXSFUNC(setForwardingGatewayIPAddress);
TRXGFUNC(getForwardingGatewayIPAddress);

TRXSFUNC(setForwardingInterface);
TRXGFUNC(getForwardingInterface);


/* InternetGatewayDevice.X_ATP_SystemMonitor. */
TRXSFUNC(setRsrpWeakBoundary);
TRXGFUNC(getRsrpWeakBoundary);

/* InternetGatewayDevice.X_ATP_SystemMonitor.LanCheck. */
TRXSFUNC(setLanCheckHost);
TRXGFUNC(getLanCheckHost);

TRXSFUNC(setLanCheckStatus);
TRXGFUNC(getLanCheckStatus);

/* InternetGatewayDevice.X_ATP_SystemMonitor.WanCheck. */
TRXSFUNC(setWanCheckHost);
TRXGFUNC(getWanCheckHost);

TRXSFUNC(setWanCheckStatus);
TRXGFUNC(getWanCheckStatus);

/* InternetGatewayDevice.X_ATP_SystemMonitor.SysMonitor. */
TRXSFUNC(setSysMonitorStatus);
TRXGFUNC(getSysMonitorStatus);

/* InternetGatewayDevice.X_ATP_ModemLog. */
TRXSFUNC(setX_ATP_ModemLogStatus);
TRXGFUNC(getX_ATP_ModemLogStatus);

/*InternetGatewayDevice.X_ATP_PreserveData.{i}. */
TRXSFUNC(setParameterName);
TRXGFUNC(getParameterName);

/*InternetGatewayDevice.UserInterface.X_ATP_Web.UserInfo.{i}.. */
TRXSFUNC(setX_ATP_WebUsername);
TRXGFUNC(getX_ATP_WebUsername);

TRXSFUNC(setX_ATP_WebUserpassword);
TRXGFUNC(getX_ATP_WebUserpassword);




TRXGFUNC(getX_CMCC_ConfigVersion);
TRXGFUNC(getX_CMCC_ModuleVersion);
TRXGFUNC(getX_CMCC_ModuleType);

/* InternetGatewayDevice.DeviceInfo.X_CMCC_TeleComAccount */
TRXGFUNC(getDeviceInfoX_CMCC_TeleComAccountEnable);
TRXSFUNC(setDeviceInfoX_CMCC_TeleComAccountEnable);
TRXGFUNC(getDeviceInfoX_CMCC_TeleComAccountUsername);
TRXSFUNC(setDeviceInfoX_CMCC_TeleComAccountUsername);
TRXGFUNC(getDeviceInfoX_CMCC_TeleComAccountPassword);
TRXSFUNC(setDeviceInfoX_CMCC_TeleComAccountPassword);

/* InternetGatewayDevice.DeviceInfo.X_CMCC_TeleComAccount */
TRXGFUNC(getDeviceInfoX_CMCC_MonitorDescEnable);
TRXSFUNC(setDeviceInfoX_CMCC_MonitorDescEnable);
TRXGFUNC(getDeviceInfoX_CMCC_MonitorDescMonitorNumberOfEntries);
TRXSFUNC(setDeviceInfoX_CMCC_MonitorDescMonitorNumberOfEntries);

/* InternetGatewayDevice.DeviceInfo.X_CMCC_Monitor.i. */
TRXGFUNC(deleteMonitorConfig);
TRXSFUNC(addMonitorConfig);
TRXSFUNC(setDeviceInfoX_CMCC_MonitorMonitorConfigParaList);
TRXGFUNC(getDeviceInfoX_CMCC_MonitorMonitorConfigParaList);
TRXSFUNC(setDeviceInfoX_CMCC_MonitorMonitorConfigTimeList);
TRXGFUNC(getDeviceInfoX_CMCC_MonitorMonitorConfigTimeList);


/* InternetGatewayDevice.DeviceInfo.X_CMCC_Alarm  */
TRXGFUNC(getDeviceInfoX_CMCC_AlarmAlarmNumber);
TRXSFUNC(setDeviceInfoX_CMCC_AlarmAlarmNumber);
	

/* InternetGatewayDevice.X_DeviceInfo. */
TRXGFUNC(getDeviceStatus);
TRXGFUNC(getUserName);
TRXGFUNC(getLoadAverage);
TRXGFUNC(getMemory);

/* InternetGatewayDevice.X_ATP_FireWall. */
TRXSFUNC(setX_ATP_FireWallCurrentLevel);
TRXGFUNC(getX_ATP_FireWallCurrentLevel);


/* InternetGatewayDevice.X_ATP_FireWall.FilterMode.{i}. */
TRXGFUNC(getFilterModeLevel);

TRXSFUNC(setMACMode);
TRXGFUNC(getMACMode);

TRXSFUNC(setIPMode);
TRXGFUNC(getIPMode);

TRXSFUNC(setURLMode);
TRXGFUNC(getURLMode);

/* InternetGatewayDevice.X_ATP_FireWall.UrlBlackFilter.{i}. */
TRXSFUNC(setBlackFilterUrl);
TRXGFUNC(getBlackFilterUrl);


/* InternetGatewayDevice.X_ATP_FireWall.MacBlackFilter.{i}. */
TRXSFUNC(setBlackFilterSourceMACAddress);
TRXGFUNC(getBlackFilterSourceMACAddress);


/*InternetGatewayDevice.X_ATP_FireWall.IpBlackFilter.{i}. */

TRXSFUNC(setBlackFilterSourceIPStart);
TRXGFUNC(getBlackFilterSourceIPStart);

TRXSFUNC(setBlackFilterSourceIPEnd);
TRXGFUNC(getBlackFilterSourceIPEnd);

TRXSFUNC(setBlackFilterDestIPStart);
TRXGFUNC(getBlackFilterDestIPStart);

TRXSFUNC(setBlackFilterDestIPEnd);
TRXGFUNC(getBlackFilterDestIPEnd);


TRXSFUNC(setBlackFilterProtocol);
TRXGFUNC(getBlackFilterProtocol);


TRXSFUNC(setBlackFilterSourcePortStart);
TRXGFUNC(getBlackFilterSourcePortStart);

TRXSFUNC(setBlackFilterSourcePortEnd);
TRXGFUNC(getBlackFilterSourcePortEnd);

TRXSFUNC(setBlackFilterDestPortStart);
TRXGFUNC(getBlackFilterDestPortStart);

TRXSFUNC(setBlackFilterDestPortEnd);
TRXGFUNC(getBlackFilterDestPortEnd);

/*InternetGatewayDevice.X_ATP_FireWall.UrlWhiteFilter.{i}. */

TRXSFUNC(setWhiteFilterUrl);
TRXGFUNC(getWhiteFilterUrl);


/*InternetGatewayDevice.X_ATP_FireWall.MacWhiteFilter.{i}. */

TRXSFUNC(setWhiteFilterSourceMACAddress);
TRXGFUNC(getWhiteFilterSourceMACAddress);


/*InternetGatewayDevice.X_ATP_FireWall.IpWhiteFilter.{i}. */

TRXSFUNC(setWhiteFilterSourceIPStart);
TRXGFUNC(getWhiteFilterSourceIPStart);

TRXSFUNC(setWhiteFilterSourceIPEnd);
TRXGFUNC(getWhiteFilterSourceIPEnd);

TRXSFUNC(setWhiteFilterDestIPStart);
TRXGFUNC(getWhiteFilterDestIPStart);

TRXSFUNC(setWhiteFilterDestIPEnd);
TRXGFUNC(getWhiteFilterDestIPEnd);


TRXSFUNC(setWhiteFilterProtocol);
TRXGFUNC(getWhiteFilterProtocol);


TRXSFUNC(setWhiteFilterSourcePortStart);
TRXGFUNC(getWhiteFilterSourcePortStart);

TRXSFUNC(setWhiteFilterSourcePortEnd);
TRXGFUNC(getWhiteFilterSourcePortEnd);

TRXSFUNC(setWhiteFilterDestPortStart);
TRXGFUNC(getWhiteFilterDestPortStart);

TRXSFUNC(setWhiteFilterDestPortEnd);
TRXGFUNC(getWhiteFilterDestPortEnd);


/*InternetGatewayDevice.X_ATP_FireWall.AccessControl.{i}. */

TRXGFUNC(getService);
TRXGFUNC(getDirection);


TRXSFUNC(setStartIpAddr);
TRXGFUNC(getStartIpAddr);

TRXSFUNC(setEndIpAddr);
TRXGFUNC(getEndIpAddr);

TRXSFUNC(setAccessControlEnable);
TRXGFUNC(getAccessControlEnable);

/* InternetGatewayDevice.DeviceInfo.X_ATP_Informations. */

TRXGFUNC(getDeviceInfoX_ATP_Informationsimsi);
TRXGFUNC(getDeviceInfoX_ATP_Informationssn);
/*InternetGatewayDevice.FreqLock.FreqLData.*/
TRXSFUNC(setFreqLock_Mode);
TRXGFUNC(getFreqLock_Mode);

TRXSFUNC(setFreqBand_Info);
TRXGFUNC(getFreqBand_Info);

TRXSFUNC(setFreqLock_FreqValue);
TRXGFUNC(getFreqLock_FreqValue);

TRXSFUNC(setFreqLock_Cell);
TRXGFUNC(getFreqLock_Cell);

/*InternetGatewayDevice.X_ATP_Vxlan.VXLAN_LAC.*/
TRXSFUNC(setVXLAN_LACEnable);
TRXGFUNC(getVXLAN_LACEnable);
TRXSFUNC(setVXLAN_LACVxlanRemoteIpAddress);
TRXGFUNC(getVXLAN_LACVxlanRemoteIpAddress);
/* InternetGatewayDevice.X_ATP_L2Gre.L2GRE_LAC. */
TRXSFUNC(setL2GRE_LACEnable);
TRXGFUNC(getL2GRE_LACEnable);
TRXSFUNC(setL2GRE_LACGreRemoteIpAddress);
TRXGFUNC(getL2GRE_LACGreRemoteIpAddress);
TRXSFUNC(setL2GRE_LACVlanEnable);
TRXGFUNC(getL2GRE_LACVlanEnable);
TRXSFUNC(setL2GRE_LACDscpMarkMode);
TRXGFUNC(getL2GRE_LACDscpMarkMode);
TRXSFUNC(setL2GRE_LACARPDscpMark);
TRXGFUNC(getL2GRE_LACARPDscpMark);
TRXSFUNC(setL2GRE_LACIPMapVlanPrioEnable);
TRXGFUNC(getL2GRE_LACIPMapVlanPrioEnable);
/* InternetGatewayDevice.X_ATP_L2Gre.VlanSettings. */
TRXSFUNC(setVlanSettingsInterface);
TRXGFUNC(getVlanSettingsInterface);
TRXSFUNC(setVlanSettingsVlanType);
TRXGFUNC(getVlanSettingsVlanType);
TRXSFUNC(setVlanSettingsAdPvidFdEnable);
TRXGFUNC(getVlanSettingsAdPvidFdEnable);
TRXSFUNC(setVlanSettingsPvid);
TRXGFUNC(getVlanSettingsPvid);
/* InternetGatewayDevice.X_ATP_FireWall.Switch. */
TRXSFUNC(setFirewallMainSwitch);
TRXGFUNC(getFirewallMainSwitch);
TRXSFUNC(setFirewallIPFilterSwitch);
TRXGFUNC(getFirewallIPFilterSwitch);
TRXSFUNC(setFirewallWanPortPingSwitch);
TRXGFUNC(getFirewallWanPortPingSwitch);
TRXSFUNC(setFirewallWanWebSwitch);
TRXGFUNC(getFirewallWanWebSwitch);
TRXSFUNC(setfirewallmacfilterswitch);
TRXGFUNC(getfirewallmacfilterswitch);
TRXSFUNC(setfirewallurlfilterswitch);
TRXGFUNC(getfirewallurlfilterswitch);
TRXSFUNC(setfirewallfwstateswitch);
TRXGFUNC(getfirewallfwstateswitch);
/* InternetGatewayDevice.X_ATP_VPN.L2TP_LAC. */
TRXSFUNC(setL2TP_LACHostName);
TRXGFUNC(getL2TP_LACHostName);
TRXSFUNC(setL2TP_LACPassword);
TRXGFUNC(getL2TP_LACPassword);
TRXSFUNC(setL2TP_LACEnable);
TRXGFUNC(getL2TP_LACEnable);
TRXSFUNC(setL2TP_LACBCPEnable);
TRXGFUNC(getL2TP_LACBCPEnable);
TRXSFUNC(setL2TP_LACWanInterface);
TRXGFUNC(getL2TP_LACWanInterface);
TRXSFUNC(setL2TP_LACLNSAddress);
TRXGFUNC(getL2TP_LACLNSAddress);
TRXSFUNC(setL2TP_LACPCAuthMode);
TRXGFUNC(getL2TP_LACPCAuthMode);
TRXSFUNC(setL2TP_LACPppUser);
TRXGFUNC(getL2TP_LACPppUser);
TRXSFUNC(setL2TP_LACPppPass);
TRXGFUNC(getL2TP_LACPppPass);
TRXSFUNC(setL2TP_LACKeepAliveTime);
TRXGFUNC(getL2TP_LACKeepAliveTime);
/* InternetGatewayDevice.X_ATP_AlarmInfo. */
TRXGFUNC(getX_ATP_AlarmInfoSequenceNo);
/* InternetGatewayDevice.X_ATP_AlarmInfo.AlarmInstance.*/
TRXGFUNC(getAlarmInstanceDescription);
TRXGFUNC(getAlarmInstanceLiftDescription);
/* InternetGatewayDevice.X_ATP_Config.dialup. */
TRXSFUNC(setdialuproam_connect);
TRXGFUNC(getdialuproam_connect);
TRXSFUNC(setdialupcurrent_profile);
TRXGFUNC(getdialupcurrent_profile);
/* InternetGatewayDevice.X_ATP_Config.dialup.profile. */
TRXGFUNC(getdialupprofileindex);
TRXSFUNC(setdialupprofile_name);
TRXGFUNC(getdialupprofile_name);
TRXSFUNC(setdialupapn);
TRXGFUNC(getdialupapn);
TRXSFUNC(setdialupusername);
TRXGFUNC(getdialupusername);
TRXSFUNC(setdialuppassword);
TRXGFUNC(getdialuppassword);
TRXSFUNC(setdialupauth_mode);
TRXGFUNC(getdialupauth_mode);
/* InternetGatewayDevice.Services.VoiceService. */
TRXSFUNC(setVoiceServiceX_ATP_UmtsDTMFMethod);
TRXGFUNC(getVoiceServiceX_ATP_UmtsDTMFMethod);
/* InternetGatewayDevice.Services.VoiceService.VoiceProfile. */
TRXGFUNC(getVoiceProfileDTMFMethod);
/* InternetGatewayDevice.Services.VoiceService.VoiceProfile.Line. */
TRXSFUNC(setLineEnable);
TRXGFUNC(getLineEnable);
TRXSFUNC(setLineDirectoryNumber);
TRXGFUNC(getLineDirectoryNumber);
TRXGFUNC(getLineStatus);
TRXSFUNC(setLineX_ATP_ClipEnable);
TRXGFUNC(getLineX_ATP_ClipEnable);
/* InternetGatewayDevice.Services.VoiceService.VoiceProfile.Line.Codec.List. */
TRXGFUNC(getListCodec);
/* InternetGatewayDevice.Services.VoiceService.VoiceProfile.Line.SIP. */
TRXSFUNC(setSipAuthUserName);
TRXGFUNC(getSipAuthUserName);
TRXSFUNC(setSipAuthPassword);
TRXGFUNC(getSipAuthPassword);
/* InternetGatewayDevice.Services.VoiceService.VoiceProfile.SIP. */
TRXSFUNC(setSipProxyServer);
TRXGFUNC(getSipProxyServer);
TRXSFUNC(setSipProxyServerPort);
TRXGFUNC(getSipProxyServerPort);
TRXSFUNC(setSipRegistrarServer);
TRXGFUNC(getSipRegistrarServer);
TRXSFUNC(setSipRegistrarServerPort);
TRXGFUNC(getSipRegistrarServerPort);
TRXSFUNC(setSipX_ATP_SIPDomain);
TRXGFUNC(getSipX_ATP_SIPDomain);
TRXSFUNC(setSipX_SecondProxyServer);
TRXGFUNC(getSipX_SecondProxyServer);
TRXSFUNC(setSipX_SecondProxyServerPort);
TRXGFUNC(getSipX_SecondProxyServerPort);
TRXSFUNC(setSipX_SecondSIPDomain);
TRXGFUNC(getSipX_SecondSIPDomain);
TRXSFUNC(setSipX_SecondRegistrarServer);
TRXGFUNC(getSipX_SecondRegistrarServer);
TRXSFUNC(setSipX_SecondRegistrarServerPort);
TRXGFUNC(getSipX_SecondRegistrarServerPort);
/* InternetGatewayDevice.Services.VoiceService.X_ATP_CommonConfig. */
TRXGFUNC(getVoiceX_ATP_Voipcsselect);
TRXGFUNC(getVoiceX_ATP_CIDSendType);
#endif /* __DEVICEINFOPARAMS_H__ */
