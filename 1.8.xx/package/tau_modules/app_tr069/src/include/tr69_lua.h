#ifndef _TR69_LUA_H__
#define _TR69_LUA_H__

#ifndef UCI_OK 0
#define UCI_OK 0
#endif

#ifndef UCI_FAIL -1
#define UCI_FAIL -1
#endif

/********************************* SNMP INFO *****************************************/
#define UCI_CONFIG_SNMP_TRAP_IP			           "config.mini_snmpd.trap_ip"
#define UCI_CONFIG_SNMP_SWITCH				       "config.mini_snmpd.switch"
#define UCI_CONFIG_SNMP_STATUS				       "config.mini_snmpd.status"
/********************************* LTE INFO *****************************************/
#define UCI_LTEINFO_SIM_STATUS                     "lteinfo.info.sim_status"
#define UCI_LTEINFO_SIM_INFO_IMSI                  "lteinfo.info.sim_info_imsi"
#define UCI_LTEINFO_SIM_INFO_ICCID                 "lteinfo.info.sim_info_iccid"
#define UCI_LTEINFO_PIN_STATUS                     "lteinfo.info.pin_status"
#define UCI_LTEINFO_DIAL_STATUS                    "lteinfo.info.dial_status"
#define UCI_LTEINFO_CURRENT_ACCESS_NETWORK         "lteinfo.info.current_access_network"
#define UCI_LTEINFO_OPERATOR                       "lteinfo.info.operator"
#define UCI_LTEINFO_IPV4_ADDR                      "lteinfo.info.ipv4_addr"
#define UCI_LTEINFO_COMMUNITY_ID                   "lteinfo.info.community_id"               //逻辑小区
#define UCI_LTEINFO_PHYSICAL_COMMUNITY_ID          "lteinfo.info.physical_community_id"      //物理小区
#define UCI_LTEINFO_ENODEB			               "lteinfo.info.enodeb"
#define UCI_LTEINFO_TAC                            "lteinfo.info.tac"
#define UCI_LTEINFO_ACCESS_FREQUENCY               "lteinfo.info.access_frequency"           //频点 
#define UCI_LTEINFO_ACCESS_SPECTRUM                "lteinfo.info.access_spectrum"            //频段
#define UCI_LTEINFO_LOCK_SPECTRUM                  "lteinfo.info.lock_spectrum"              //锁频段
#define UCI_LTEINFO_INFO_RSSI                      "lteinfo.info.info_rssi"
#define UCI_LTEINFO_INFO_RSRP                      "lteinfo.info.info_rsrp"
#define UCI_LTEINFO_INFO_RSRQ                      "lteinfo.info.info_rsrq"
#define UCI_LTEINFO_INFO_SINR                      "lteinfo.info.info_sinr"
#define UCI_LTEINFO_LTE_VER                        "lteinfo.info.lte_ver"
/********************************* FREQUENCY INFO *****************************************/
#define UCI_LTE_GET_FREQUENCY_PASSWD               "lte.frequency.frequency_passwd"
#define UCI_LTE_GET_FREQUENCY_LOCK_COMMUNITY       "lte.frequency.lock_community"
#define UCI_LTE_GET_FREQUENCY_LOCK_SW              "lte.frequency.lock_sw"
#define UCI_LTE_GET_FREQUENCY_FREQUENCY            "lte.frequency.frequency"
/********************************* PIN INFO *****************************************/
#define UCI_LTE_GET_SIM_LOCK_SW                    "lte.sim.lock_sw"
#define UCI_LTE_SET_PIN_LOCK_SW                    "lte.sim.pin_sw"
#define UCI_LTE_GET_PIN_SIMLOCK_CODE               "lte.sim.pin_simlock_code"
#define UCI_LTE_SET_PIN_SIMLOCK_CODE_OLD           "lte.sim.pin_simlock_code_old"
#define UCI_LTE_GET_PIN_CODE                       "lte.sim.pin_code"
#define UCI_LTE_SET_PIN_OLD_CODE                   "lte.sim.pin_old_code"
#define UCI_LTE_SET_PIN_NEW_CODE                   "lte.sim.pin_new_code"
#define UCI_LTE_SET_PIN_FUNCTION_SELECT            "lte.sim.pin_function_select"
#define UCI_LTE_SET_PIN_UNLOCK_SW                  "lte.sim.pin_unlock_sw"
#define UCI_LTE_SET_PIN_STATE                      "lte.sim.pin_state"
/********************************* SYSTEM INFO *****************************************/
#define UCI_SYS_CPU_USES                           "tr069.tr069.cpuuses"
#define UCI_SYS_MEM_FREE						   "tr069.tr069.memfree"
#define UCI_SYS_MEM_TOTAL						   "tr069.tr060.memtotal"
#define UCI_SYS_PRESERVE_ENABLE 				   "tr069.DeviceConfig.X_ATP_PreserveEnable"
/**********************************X_ATP_EXT********************************************/
#define UCI_X_ATP_EXT_MODEMLOGSESSDIONSTATUS       "tr069.X_ATP_EXT.ModemLogSessionStatus"
#define UCI_X_ATP_EXT_CELLID                       "tr069.X_ATP_EXT.CellID"
#define UCI_X_ATP_EXT_DOWNLOADPROGRESS             "tr069.X_ATP_EXT.DownloadProgress"

#define UCI_NETWORK_BR_LAN_IPADDR	"tr069.lan.ipaddr_n"

/**********************************InternetGatewayDevice.DeviceInfo********************************************/
#define UCI_DEVICEINFO_MAMUFACTURER   "tr069.DeviceInfo.Mamufacturer"
#define UCI_DEVICEINFO_MAMUFACTUREROUI   "tr069.DeviceInfo.ManufacturerOUI"
#define UCI_DEVICEINFO_MODELNAME   "tr069.DeviceInfo.ModelName"
#define UCI_DEVICEINFO_DESCRIPTION   "tr069.DeviceInfo.Description"
#define UCI_DEVICEINFO_PRODUCTCLASS   "tr069.DeviceInfo.ProductClass"
#define UCI_DEVICEINFO_SERIALNUMBER   "tr069.DeviceInfo.SerialNumber"
#define UCI_DEVICEINFO_HARDVER   "tr069.DeviceInfo.HardVer"
#define UCI_DEVICEINFO_SOFTVER  "tr069.DeviceInfo.SoftVer"
#define UCI_DEVICEINFO_MODEMFIRMWAREVER  "tr069.DeviceInfo.ModemFirmwareVer"
#define UCI_DEVICEINFO_PROVISIONINGCODE  "tr069.DeviceInfo.ProvisioningCode"

#define UCI_DEVICEINFO_SPACVER  "tr069.DeviceInfo.SpacVer"
#define UCI_DEVICEINFO_UPTIME  "tr069.DeviceInfo.UpTime"
#define UCI_DEVICEINFO_X_ATP_IMEI  "tr069.DeviceInfo.X_ATP_IMEI"
#define UCI_DEVICEINFO_X_ATP_ANTENNACONFIG  "tr069.DeviceInfo.X_ATP_AntennaConfig"
#define UCI_DEVICEINFO_X_ATP_PCBVERSION  "tr069.DeviceInfo.X_ATP_PCBVersion"
#define UCI_DEVICEINFO_X_ATP_LASTSOFTVER  "tr069.DeviceInfo.X_ATP_LastSoftVer"
#define UCI_DEVICEINFO_X_ATP_MODEMPRODUCTNAME  "tr069.DeviceInfo.X_ATP_ModemProductName"
#define UCI_DEVICEINFO_X_ATP_SUPPORTVPNNUM  "tr069.DeviceInfo.X_ATP_SupportVpnNum"
#define UCI_DEVICEINFO_X_ATP_BOMCLODE  "tr069.DeviceInfo.X_ATP_BOMCode"
#define UCI_DEVICEINFO_X_ATP_TEMPERATRUE  "tr069.DeviceInfo.X_ATP_Temperatrue"



/**********************************Layer3Forwarding********************************************/
#define UCI_LAYER3FORWARDING_FORWARDNUMOFENTRIES           "tr069.Layer3Forwarding.ForwardNumOfEntries"
#define UCI_LAYER3FORWARDING_DETIPADDR                     "tr069.Layer3Forwarding_Forwarding.DestIPAddr"
#define UCI_LAYER3FORWARDING_DESTSUBNETMASK                "tr069.Layer3Forwarding_Forwarding.DestSubnetMask"
#define UCI_LAYER3FORWARDING_GATEWAYIPADDRESS              "tr069.Layer3Forwarding_Forwarding.GatewayIPAddress"
#define UCI_LAYER3FORWARDING_INTERFACE                     "tr069.Layer3Forwarding_Forwarding.Interface"

/**********************************IPPIingDiagnostics********************************************/
#define UCI_IPPINGDIAGNOSTICS_STATE "tr069.IPPingDiagnostics.DiagnosticsState"
#define UCI_IPPINGDIAGNOSTICS_INTERFACE "tr069.IPPingDiagnostics.Interface"
#define UCI_IPPINGDIAGNOSTICS_HOST  "tr069.IPPingDiagnostics.Host"
#define UCI_IPPINGDIAGNOSTICS_NUMOFREPETITIONS  "tr069.IPPingDiagnostics.NumOfRepetitions"
#define UCI_IPPINGDIAGNOSTICS_TIMEOUT  "tr069.IPPingDiagnostics.Timeout"
#define UCI_IPPINGDIAGNOSTICS_DATABLOCKSIZE  "tr069.IPPingDiagnostics.DataBlockSize"
#define UCI_IPPINGDIAGNOSTICS_DSCP  "tr069.IPPingDiagnostics.DSCP"
#define UCI_IPPINGDIAGNOSTICS_SUCCESSCOUNT  "tr069.IPPingDiagnostics.SuccessCount"
#define UCI_IPPINGDIAGNOSTICS_FAILERCOUNT  "tr069.IPPingDiagnostics.FailureCount"
#define UCI_IPPINGDIAGNOSTICS_AVERAGERESPONESTIME  "tr069.IPPingDiagnostics.AverageResponesTime"
#define UCI_IPPINGDIAGNOSTICS_MINRESPONESTIME "tr069.IPPingDiagnostics.MinResponesTime"
#define UCI_IPPINGDIAGNOSTICS_MAXRESPONESTIME "tr069.IPPingDiagnostics.MaxResponesTime"



/**********************************X_ATP_SystemMonitor********************************************/
#define UCI_X_ATP_SYSTEMMONITOR_RSRPWEAKBOUNDARY           "tr069.X_ATP_SysMonitor.RsrpWeakBoundary"
#define UCI_X_ATP_SYSTEMMONITORLANCHECK_HOST               "tr069.X_ATP_SysMonitor_LanCheck.Host"
#define UCI_X_ATP_SYSTEMMONITORLANCHECK_STATUS             "tr069.X_ATP_SysMonitor_LanCheck.Status"
#define UCI_X_ATP_SYSTEMMONITORWANCHECK_HOST               "tr069.X_ATP_SysMonitor_WanCheck.Host"
#define UCI_X_ATP_SYSTEMMONITORWANCHECK_STATUS             "tr069.X_ATP_SysMonitor_WanCheck.Status"
#define UCI_X_ATP_SYSTEMMONITORSYSTEMMONITOR_STATUS        "tr069.X_ATP_SysMonitor_SysMonitor.Status"

/**********************************X_ATP_ModemLog********************************************/
#define UCI_X_ATP_MODEMLOG_STATUS                          "tr069.X_ATP_ModemLog.Status"

/**********************************X_ATP_PreserveData********************************************/
#define UCI_X_ATP_PRESERVEDATA_PARAMNAME                   "tr069.X_ATP_PreserveData.ParamName"

/**********************************GatewayUserInterface_X_ATP_Web_UserInfo********************************************/
#define UCI_GATEWAYUSERINTERFACE_X_ATP_WEB_USERINFO_USERNAME                     "tr069.UserInterface_X_ATP_Web_UserInfo.Username"
#define UCI_GATEWAYUSERINTERFACE_X_ATP_WEB_USERINFO_USERPASSWORD                     "tr069.UserInterface_X_ATP_Web_UserInfo.UserPassword"

/**********************************X_ATP_FireWall********************************************/
#define UCI_X_ATP_FIRTEWALL_CURRENTLEVEL                     "tr069.X_ATP_FireWall.CurrentLevel"

/**********************************X_ATP_FireWall_FilterMode********************************************/
#define UCI_X_ATP_FIRTEWALL_FILTERMODE_LEVEL                     "tr069.X_ATP_FireWall_FilterMode.Level"
#define UCI_X_ATP_FIRTEWALL_FILTERMODE_MACMODE                    "tr069.X_ATP_FireWall_FilterMode.MACMode"
#define UCI_X_ATP_FIRTEWALL_FILTERMODE_IPMODE                    "tr069.X_ATP_FireWall_FilterMode.IPMode"
#define UCI_X_ATP_FIRTEWALL_FILTERMODE_URLMODE                   "tr069.X_ATP_FireWall_FilterMode.URLMode"

/**********************************X_ATP_FireWall_UrlBlackFilter********************************************/
#define UCI_X_ATP_FIRTEWALL_URLBLACKFILTER_URL                  "tr069.X_ATP_FireWall_UrlBlackFilter.Url"

/**********************************X_ATP_FireWall_MACBlackFilter********************************************/
#define UCI_X_ATP_FIRTEWALL_MACBLACKFILTER_SOURCEMACADDR                 "tr069.X_ATP_FireWall_MACBlackFilter.SourceMACAddr"

/**********************************X_ATP_FireWall_IPBlackFilter********************************************/
#define UCI_X_ATP_FIRTEWALL_IPBLACKFILTER_SOURCEIPSTART                "tr069.X_ATP_FireWall_IPBlackFilter.SourceIPStart"
#define UCI_X_ATP_FIRTEWALL_IPBLACKFILTER_SOURCEIPEnd                "tr069.X_ATP_FireWall_IPBlackFilter.SourceIPEnd"
#define UCI_X_ATP_FIRTEWALL_IPBLACKFILTER_DESTIPSTART               "tr069.X_ATP_FireWall_IPBlackFilter.DestIPStart"
#define UCI_X_ATP_FIRTEWALL_IPBLACKFILTER_DESTIPEND              "tr069.X_ATP_FireWall_IPBlackFilter.DestIPEnd"
#define UCI_X_ATP_FIRTEWALL_IPBLACKFILTER_PROTOCOL             "tr069.X_ATP_FireWall_IPBlackFilter.Protocol"
#define UCI_X_ATP_FIRTEWALL_IPBLACKFILTER_SOURCEPORTSTART            "tr069.X_ATP_FireWall_IPBlackFilter.SourcePortStart"
#define UCI_X_ATP_FIRTEWALL_IPBLACKFILTER_SOURCEPORTEND            "tr069.X_ATP_FireWall_IPBlackFilter.SourcePortEnd"
#define UCI_X_ATP_FIRTEWALL_IPBLACKFILTER_DESTPORTSTART            "tr069.X_ATP_FireWall_IPBlackFilter.DestPortStart"
#define UCI_X_ATP_FIRTEWALL_IPBLACKFILTER_DESTPORTEND            "tr069.X_ATP_FireWall_IPBlackFilter.DestPortEnd"

/**********************************X_ATP_FireWall_UrlWhiteFilter********************************************/
#define UCI_X_ATP_FIRTEWALL_URLWHITEFILTER_URL                  "tr069.X_ATP_FireWall_UrlWhiteFilter.Url"

/**********************************X_ATP_FireWall_MACWhiteFilter********************************************/
#define UCI_X_ATP_FIRTEWALL_MACWHITEFILTER_SOURCEMACADDR                 "tr069.X_ATP_FireWall_MACWhiteFilter.SourceMACAddr"

/**********************************X_ATP_FireWall_IPWhiteFilter********************************************/
#define UCI_X_ATP_FIRTEWALL_IPWHITEFILTER_SOURCEIPSTART                "tr069.X_ATP_FireWall_IPWhiteFilter.SourceIPStart"
#define UCI_X_ATP_FIRTEWALL_IPWHITEFILTER_SOURCEIPEnd                "tr069.X_ATP_FireWall_IPWhiteFilter.SourceIPEnd"
#define UCI_X_ATP_FIRTEWALL_IPWHITEFILTER_DESTIPSTART               "tr069.X_ATP_FireWall_IPWhiteFilter.DestIPStart"
#define UCI_X_ATP_FIRTEWALL_IPWHITEFILTER_DESTIPEND              "tr069.X_ATP_FireWall_IPWhiteFilter.DestIPEnd"
#define UCI_X_ATP_FIRTEWALL_IPWHITEFILTER_PROTOCOL             "tr069.X_ATP_FireWall_IPWhiteFilter.Protocol"
#define UCI_X_ATP_FIRTEWALL_IPWHITEFILTER_SOURCEPORTSTART            "tr069.X_ATP_FireWall_IPWhiteFilter.SourcePortStart"
#define UCI_X_ATP_FIRTEWALL_IPWHITEFILTER_SOURCEPORTEND            "tr069.X_ATP_FireWall_IPWhiteFilter.SourcePortEnd"
#define UCI_X_ATP_FIRTEWALL_IPWHITEFILTER_DESTPORTSTART            "tr069.X_ATP_FireWall_IPWhiteFilter.DestPortStart"
#define UCI_X_ATP_FIRTEWALL_IPWHITEFILTER_DESTPORTEND            "tr069.X_ATP_FireWall_IPWhiteFilter.DestPortEnd"

/**********************************X_ATP_FireWall_AccessControl********************************************/
#define UCI_X_ATP_FIRTEWALL_ACCESSCONTROL_SERVICE               "tr069.X_ATP_FireWall_AccessControl.Service"
#define UCI_X_ATP_FIRTEWALL_ACCESSCONTROL_DIRECTION             "tr069.X_ATP_FireWall_AccessControl.Direction"
#define UCI_X_ATP_FIRTEWALL_ACCESSCONTROL_STARTIPADDR           "tr069.X_ATP_FireWall_AccessControl.StartIPAddr"
#define UCI_X_ATP_FIRTEWALL_ACCESSCONTROL_ENDIPADDR           "tr069.X_ATP_FireWall_AccessControl.EndIPAddr"
#define UCI_X_ATP_FIRTEWALL_ACCESSCONTROL_ENABLE          "tr069.X_ATP_FireWall_AccessControl.Enable"

/**********************************InternetGatewayDevice.ManagementServer********************************************/
#define UCI_MANAGEMENTSERVER_URL   "tr069.ManagementServer.URL"
#define UCI_MANAGEMENTSERVER_USERNAME   "tr069.ManagementServer.Username"
#define UCI_MANAGEMENTSERVER_PASSWORD   "tr069.ManagementServer.Password"
#define UCI_MANAGEMENTSERVER_PERIODICINFOENABLE   "tr069.ManagementServer.PeriodicInfoEnable"
#define UCI_MANAGEMENTSERVER_PERIDDICINFOINTERVAL   "tr069.ManagementServer.periddicInfoInterval"
#define UCI_MANAGEMENTSERVER_PARAMKEYL   "tr069.ManagementServer.ParamKey"
#define UCI_MANAGEMENTSERVER_CONNECTIONREQURL   "tr069.ManagementServer.ConnectionReqURL"
#define UCI_MANAGEMENTSERVER_CONNECTIONREQUSERNAME   "tr069.ManagementServer.ConnectionReqUsername"
#define UCI_MANAGEMENTSERVER_CONNECTIONREQPASSWORD   "tr069.ManagementServer.ConnectionReqPassword"


/**********************************Time********************************************/
#define UCI_TIME_ENABLE              	"tr069.Time.Enable"
#define UCI_TIME_STATUS             	"tr069.Time.Status"
#define UCI_TIME_LOCAlTIMEZONE 			"tr069.Time.LocalTimeZone"
#define UCI_TIME_LOCAlTIMEZONENAME 		"tr069.Time.LocalTimeZoneName"

#if 0
#define UCI_TIME_CONFIGTYPE             "tr069.Time.ConfigType"
#define UCI_TIME_NTPSERVER1 			"tr069.Time.NTPServer1"
#define UCI_TIME_NTPSERVER2 			"tr069.Time.NTPServer2"
#define UCI_TIME_X_ATP_MANNUALTIME      "tr069.Time.X_ATP_MannualTime"
#else
#define UCI_TIME_CONFIGTYPE             "config.time_synchronization.time_synchronization_sw"
#define UCI_TIME_NTPSERVER1 			"config.time_synchronization.primary_server"
#define UCI_TIME_NTPSERVER2 			"config.time_synchronization.standby_server"
#define UCI_TIME_X_ATP_MANNUALTIME      "config.time_synchronization.syntime" /* 同步周期*/
#endif

/**********************************LANDevice********************************************/
#define UCI_LANDEVICE_X_ATP_AUTOCONFIG              "tr069.LANDevice.X_ATP_AutoConfig"
#define UCI_LANDEVICE_LANETHERNETINTERFACENUMBEROFENTRIES              "tr069.LANDevice.LANEthernetInterfaceNumberOfEntries"
#define UCI_LANDEVICE_LANWLANCONFIGRATIONNUMBEROFENTRIES              "tr069.LANDevice.LANWLANConfigurationNumberOfEntries"


/**********************************LANDevice_LANHostConfigManagement********************************************/
#define UCI_LANDEVICE_DHCPSERVERENABLE             "tr069.LANDevice_LANHostConfigManagement.DHCPServerEnable"
#define UCI_LANDEVICE_MINADDR            "tr069.LANDevice_LANHostConfigManagement.MinAddr"
#define UCI_LANDEVICE_MAXADDR            "tr069.LANDevice_LANHostConfigManagement.MaxAddr"
#define UCI_LANDEVICE_DHCPLEASETIME            "tr069.LANDevice_LANHostConfigManagement.DHCPLeaseTime"
#define UCI_LANDEVICE_IPINTERFACENUMBEROFENTRIES            "tr069.LANDevice_LANHostConfigManagement.IPInterfaceNumOfEntries"
#define UCI_LANDEVICE_DHCPSTATICADDRESSNUMBEROFENTRIES           "tr069.LANDevice_LANHostConfigManagement.DHCPStaticAddressNumOfEntries"
#define UCI_LANDEVICE_X_ATP_VLANID        "tr069.LANDevice_LANHostConfigManagement.X_ATP_VlanID"

/**********************************LANDevice.LANHostConfigManagement.DHCPStaticAddress********************************************/
#define UCI_LANDEVICE_LANHOSTCONFIGMANAGEMENT_DHCPSTATICADDR_ENABLE        "tr069.LANDevice_LANHostConfigManagement_DHCPStaticAddr.Enable"
#define UCI_LANDEVICE_LANHOSTCONFIGMANAGEMENT_DHCPSTATICADDR_CHADDR       "tr069.LANDevice_LANHostConfigManagement_DHCPStaticAddr.Chaddr"
#define UCI_LANDEVICE_LANHOSTCONFIGMANAGEMENT_DHCPSTATICADDR_YIADDR       "tr069.LANDevice_LANHostConfigManagement_DHCPStaticAddr.Yiaddr"
#define UCI_LANDEVICE_LANHOSTCONFIGMANAGEMENT_DHCPSTATICADDR_X_ATP_VALID       "tr069.LANDevice_LANHostConfigManagement_DHCPStaticAddr.X_ATP_valid"


/**********************************LANDevice_LANHostConfigManagement_IPInterface********************************************/
#define UCI_LANDEVICE_IPINTERFACE_ENABLE             "tr069.LANDevice_LANHostConfigManagement_IPInterface.Enable"
#define UCI_LANDEVICE_IPINTERFACEIPADDR             "tr069.LANDevice_LANHostConfigManagement_IPInterface.IPInterfaceIPAddr"
#define UCI_LANDEVICE_IPINTERFACESUBNETMASK            "tr069.LANDevice_LANHostConfigManagement_IPInterface.IPInterfaceSubnetMask"

#define UCI_LANDEVICE_X_ATP_ROUTEPROTOCOLRX             "tr069.LANDevice_LANHostConfigManagement_IPInterface.X_ATP_RouteProtocolRx"
#define UCI_LANDEVICE_X_ATP_RIPOPERATION             "tr069.LANDevice_LANHostConfigManagement_IPInterface.X_ATP_RIPOperation"
#define UCI_LANDEVICE_X_ATP_Pre_ROUTEPROTOCOL             "tr069.LANDevice_LANHostConfigManagement_IPInterface.X_ATP_Pre_RouteProtocol"

/**********************************LANDevice.{i}.LANEthernetInterfaceConfig********************************************/
#define UCI_LANDEVICE_LANETHERNETINTERFACECOFfG_STATUS             "tr069.LANDevice_LANEthernetInterfaceConfig.Status"

/**********************************LANDevice_LANEthernetInterfaceConfig_Stats********************************************/
#define UCI_LANDEVICE_BYTESSENT             "tr069.LANDevice_LANEthernetInterfaceConfig_Stats.BytesSent"
#define UCI_LANDEVICE_BYTESRECEIVED             "tr069.LANDevice_LANEthernetInterfaceConfig_Stats.BytesReceived"
#define UCI_LANDEVICE_PACKETSSENT             "tr069.LANDevice_LANEthernetInterfaceConfig_Stats.PacketsSent"
#define UCI_LANDEVICE_PACKETSRECEIVED          "tr069.LANDevice_LANEthernetInterfaceConfig_Stats.PacketsReceived"
#define UCI_LANDEVICE_X_ATP_PACKETSSENTERROR             "tr069.LANDevice_LANEthernetInterfaceConfig_Stats.X_ATP_PacketsSentError"
#define UCI_LANDEVICE_X_ATP_PACKETSSENTDROPS             "tr069.LANDevice_LANEthernetInterfaceConfig_Stats.X_ATP_PacketsSentDrops"
#define UCI_LANDEVICE_X_ATP_TIMESTAMP           "tr069.LANDevice_LANEthernetInterfaceConfig_Stats.X_ATP_Timestamp"
#define UCI_LANDEVICE_X_ATP_PACKETSRECEIVEDERRTOR           "tr069.LANDevice_LANEthernetInterfaceConfig_Stats.X_ATP_PacketsReceivedError"

#define UCI_LANDEVICE_X_ATP_PACKETSRECEIVEDDROPS           "tr069.LANDevice_LANEthernetInterfaceConfig_Stats.X_ATP_PacketsReceivedDrops"
#define UCI_LANDEVICE_X_ATP_SENTRATE          "tr069.LANDevice_LANEthernetInterfaceConfig_Stats.X_ATP_SentRate"
#define UCI_LANDEVICE_X_ATP_RECEIVEDRATE          "tr069.LANDevice_LANEthernetInterfaceConfig_Stats.X_ATP_ReceivedRate"

/**********************************LANDevice_WLANConfig********************************************/
#define UCI_LANDEVICE_WLANCONFIG_SSIDADVERTISEMENTENABLED        "tr069.LANDevice_WLANConfig.SSIDAdvertisementEnabled"
#define UCI_LANDEVICE_WLANCONFIGURATION_ENABLED        "tr069.LANDevice_WLANConfig.Enabled"
#define UCI_LANDEVICE_WLANCONFIGURATION_STATUS       "tr069.LANDevice_WLANConfig.Status"
#define UCI_LANDEVICE_WLANCONFIGURATION_BSSID       "tr069.LANDevice_WLANConfig.BSSID"
#define UCI_LANDEVICE_WLANCONFIGURATION_MAXBITRATE       "tr069.LANDevice_WLANConfig.MaxBitRate"
#define UCI_LANDEVICE_WLANCONFIGURATION_CHANNEL       "tr069.LANDevice_WLANConfig.Channel"
#define UCI_LANDEVICE_WLANCONFIGURATION_AUTOCHANNELENABLE       "tr069.LANDevice_WLANConfig.AutoChannelEnable"
#define UCI_LANDEVICE_WLANCONFIGURATION_REGULATORYDOMAIN      "tr069.LANDevice_WLANConfig.RegulatoryDomain"
#define UCI_LANDEVICE_WLANCONFIGURATION_BEACONTYPE      "tr069.LANDevice_WLANConfig.BeaconType"
#define UCI_LANDEVICE_WLANCONFIGURATION_MACADDRCONTROLENABLE      "tr069.LANDevice_WLANConfig.MACAddrControlEnable"
#define UCI_LANDEVICE_WLANCONFIGURATION_STANDARD     "tr069.LANDevice_WLANConfig.Standard"
#define UCI_LANDEVICE_WLANCONFIGURATION_WEPKEYINDEX     "tr069.LANDevice_WLANConfig.WEPKeyIndex"
#define UCI_LANDEVICE_WLANCONFIGURATION_WEPENCRYPTIONLEVEL     "tr069.LANDevice_WLANConfig.WEPEncryptionLevel"
#define UCI_LANDEVICE_WLANCONFIGURATION_BASICENCRYPTIONMODES     "tr069.LANDevice_WLANConfig.BasicENcryptionModes"
#define UCI_LANDEVICE_WLANCONFIGURATION_BASICAUTHMODE     "tr069.LANDevice_WLANConfig.BasicAuthMode"
#define UCI_LANDEVICE_WLANCONFIGURATION_WPAENCTYPTIONMODES    "tr069.LANDevice_WLANConfig.WPAEncryptionModes"
#define UCI_LANDEVICE_WLANCONFIGURATION_WPAAUTHMODE    "tr069.LANDevice_WLANConfig.WPAAuthMode"
#define UCI_LANDEVICE_WLANCONFIGURATION_IEEE11IENCRYPTIONMODES   "tr069.LANDevice_WLANConfig.IEEE11iEncryptionModes"
#define UCI_LANDEVICE_WLANCONFIGURATION_IEEE11IAUTHMODE   "tr069.LANDevice_WLANConfig.IEEE11iAuthMode"
#define UCI_LANDEVICE_WLANCONFIGURATION_TOTALASSOCIATIONS   "tr069.LANDevice_WLANConfig.TotalAssociations"
#define UCI_LANDEVICE_WLANCONFIGURATION_TOTALBYTESSENT  "tr069.LANDevice_WLANConfig.TotalBytesSent"
#define UCI_LANDEVICE_WLANCONFIGURATION_TOTALBYTESRECEIVED  "tr069.LANDevice_WLANConfig.TotalBytesReceived"
#define UCI_LANDEVICE_WLANCONFIGURATION_TOTALPACKETSSENT "tr069.LANDevice_WLANConfig.TotalPacketsSent"
#define UCI_LANDEVICE_WLANCONFIGURATION_TOTALPACKETSRECEIVED "tr069.LANDevice_WLANConfig.TotalPacketsReceived"
#define UCI_LANDEVICE_WLANCONFIGURATION_WMMENABLE "tr069.LANDevice_WLANConfig.WMMEnable"
#define UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_MIXEDENCRYPTIONMODES "tr069.LANDevice_WLANConfig.X_ATP_MixedEncryptionModes"
#define UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_MIXEXAUTHMODE "tr069.LANDevice_WLANConfig.X_ATP_MixexAuthMode"
#define UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_CHANNELUSED  "tr069.LANDevice_WLANConfig.X_ATP_ChannelUsed"
#define UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_WLANVERSION  "tr069.LANDevice_WLANConfig.X_ATP_WLANVersion"
#define UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_POWERVALUE  "tr069.LANDevice_WLANConfig.X_ATP_PowerValue"
#define UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_TOTALBYTESSENTERROR  "tr069.LANDevice_WLANConfig.X_ATP_TotalBytesSentError"
#define UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_TOTALBYTESRECEIVEDERROR  "tr069.LANDevice_WLANConfig.X_ATP_TotalBytesReceivedError"
#define UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_TOTALPACKETSSENTERROR  "tr069.LANDevice_WLANConfig.X_ATP_TotalPacketsSentError"
#define UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_TOTALPACKETSRECEIVEDERROR  "tr069.LANDevice_WLANConfig.X_ATP_TotalPacketsReceivedError"
#define UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_ASSOCIATEDEVICENUM  "tr069.LANDevice_WLANConfig.X_ATP_AssociateDeviceNum"
#define UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_WLANMACFILTERPOLICY "tr069.LANDevice_WLANConfig.X_ATP_WlanMacFilterPolicy"
#define UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_WLANMACFILTERNUM  "tr069.LANDevice_WLANConfig.X_ATP_WlanMacFilterNum"
#define UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_WLANSTAWAKEENABLE  "tr069.LANDevice_WLANConfig.X_ATP_WlanStaWakeEnable"
#define UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_WLANISOLATECONTROL  "tr069.LANDevice_WLANConfig.X_ATP_WlanIsolateControl"
#define UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_WLAN11NBWCONTROL  "tr069.LANDevice_WLANConfig.X_ATP_Wlan11NBWControl"
#define UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_WLAN11NGTCONTROL  "tr069.LANDevice_WLANConfig.X_ATP_Wlan11NGTControl"
#define UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_WLAN11NHTMCS  "tr069.LANDevice_WLANConfig.X_ATP_Wlan11NHtMcs"
#define UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_WLAN11NTXRXSTREAM "tr069.LANDevice_WLANConfig.X_ATP_Wlan11NTxRxStream"

/**********************************LANDevice_WLANConfig_WPS********************************************/
#define UCI_LANDEVICE_WLANCONFIGURATIONWPS_ENABLE "tr069.LANDevice_WLANConfig_WPS.Enable"
#define UCI_LANDEVICE_WLANCONFIGURATIONWPS_X_ATP_WPSMODE "tr069.LANDevice_WLANConfig_WPS.X_ATP_WPSMode"
#define UCI_LANDEVICE_WLANCONFIGURATIONWPS_DEVICEPASSWORD "tr069.LANDevice_WLANConfig_WPS.DevicePassword"

/**********************************LANDevice_WLANConfig_X_ATP_WanMacFilter********************************************/
#define UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_WLANSRCMACADDR   "tr069.LANDevice_WLANConfig_X_ATP_WanMacFilter.X_ATP_WlanSrcMacAddr"
#define UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_SSID1   "tr069.LANDevice_WLANConfig_X_ATP_WanMacFilter.X_ATP_SSID1"
#define UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_SSID2   "tr069.LANDevice_WLANConfig_X_ATP_WanMacFilter.X_ATP_SSID2"
#define UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_SSID3   "tr069.LANDevice_WLANConfig_X_ATP_WanMacFilter.X_ATP_SSID3"
#define UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_SSID4  "tr069.LANDevice_WLANConfig_X_ATP_WanMacFilter.X_ATP_SSID4"

/**********************************LANDevice_WLANConfig_AssociateDevice********************************************/
#define UCI_LANDEVICE_WLANCONFIGURATION_ASSOCIATEDEVICEMACADDR  "tr069.LANDevice_WLANConfig_AssociateDevice.AssociateDeviceMACAddr"

/**********************************LANDevice_WLANConfig_WEPKey********************************************/
#define UCI_LANDEVICE_WLANCONFIGURATION_WEPKEY  "tr069.LANDevice_WLANConfig_WEPKey.WEPKey"

/**********************************LANDevice_WLANConfig_PSK********************************************/
#define UCI_LANDEVICE_WLANCONFIGURATION_PSK  "tr069.LANDevice_WLANConfig_PSK.PSK"

/**********************************WANDevice*******************************************/
#define UCI_WANDEVICE_WANCONNECTIONNUBEROFENTRIES  "tr069.WANDevice.WANConnectionNumOfEntries"


/**********************************WANDevice_WANConnectionDevice_X_ATP_APN********************************************/
#define UCI_WANDEVICE_X_ATP_APN_APN  "tr069.WANDevice_WANConnectionDevice_X_ATP_APN.APN"
#define UCI_WANDEVICE_X_ATP_APN_AUTHPROTOCOL  "tr069.WANDevice_WANConnectionDevice_X_ATP_APN.AuthProtocol"
#define UCI_WANDEVICE_X_ATP_APN_USERNAME  "tr069.WANDevice_WANConnectionDevice_X_ATP_APN.Username"
#define UCI_WANDEVICE_X_ATP_APN_PASSWORD  "tr069.WANDevice_WANConnectionDevice_X_ATP_APN.Password"
#define UCI_WANDEVICE_X_ATP_APN_DIALNUM  "tr069.WANDevice_WANConnectionDevice_X_ATP_APN.DialNum"
#define UCI_WANDEVICE_X_ATP_APN_AUTOAPN  "tr069.WANDevice_WANConnectionDevice_X_ATP_APN.AutoAPN"

/**********************************WANDevice_WANConnectionDevice_WANIPConnection********************************************/
#define UCI_WANDEVICE_PORTMAPPING_ENABLE "tr069.WANDevice_WANConnectionDevice_WANIPConnection.Enable"
#define UCI_WANDEVICE_PORTMAPPING_CONNECTIONSTATUS "tr069.WANDevice_WANConnectionDevice_WANIPConnection.ConnectionStatus"
#define UCI_WANDEVICE_WANIPCONNECTION_NAME "tr069.WANDevice_WANConnectionDevice_WANIPConnection.Name"
#define UCI_WANDEVICE_WANIPCONNECTION_LASTCONNECTIONERROR "tr069.WANDevice_WANConnectionDevice_WANIPConnection.LastConnectionError"
#define UCI_WANDEVICE_WANIPCONNECTION_NATENABLED "tr069.WANDevice_WANConnectionDevice_WANIPConnection.NATEnabled"
#define UCI_WANDEVICE_WANIPCONNECTION_EXTERNALIPADDRESS "tr069.WANDevice_WANConnectionDevice_WANIPConnection.ExternalIPAddress"
#define UCI_WANDEVICE_WANIPCONNECTION_SUBNETMASK "tr069.WANDevice_WANConnectionDevice_WANIPConnection.SubnetMask"
#define UCI_WANDEVICE_WANIPCONNECTION_DEFAULTGATEWAY "tr069.WANDevice_WANConnectionDevice_WANIPConnection.DefaultGateway"
#define UCI_WANDEVICE_WANIPCONNECTION_MACADDR "tr069.WANDevice_WANConnectionDevice_WANIPConnection.MACAddr"
#define UCI_WANDEVICE_WANIPCONNECTION_CONNECTIONTIiGGER "tr069.WANDevice_WANConnectionDevice_WANIPConnection.ConnectionTrigger"
#define UCI_WANDEVICE_WANIPCONNECTION_PORTMAPPINGNUMBEROFENTRIES "tr069.WANDevice_WANConnectionDevice_WANIPConnection.PortMappingNumOfEntries"
#define UCI_WANDEVICE_PORTMAPPING_MAXMTUSIZE "tr069.WANDevice_WANConnectionDevice_WANIPConnection.MaxMTUSize"
#define UCI_WANDEVICE_PORTMAPPING_X_ATP_APN  "tr069.WANDevice_WANConnectionDevice_WANIPConnection.X_ATP_APN"
#define UCI_WANDEVICE_PORTMAPPING_X_ATP_AUTHPROTOCOL  "tr069.WANDevice_WANConnectionDevice_WANIPConnection.X_ATP_AuthProtocol"
#define UCI_WANDEVICE_PORTMAPPING_X_ATP_USERNAME  "tr069.WANDevice_WANConnectionDevice_WANIPConnection.X_ATP_Username"
#define UCI_WANDEVICE_PORTMAPPING_X_ATP_DIALNUM  "tr069.WANDevice_WANConnectionDevice_WANIPConnection.X_ATP_DialNum"
#define UCI_WANDEVICE_PORTMAPPING_X_ATP_AUTOAPN  "tr069.WANDevice_WANConnectionDevice_WANIPConnection.X_ATP_AutoAPN"
/**********************************WANDevice_WANConnectionDevice_WANIPConnection_PortMapping********************************************/
#define UCI_WANDEVICE_PORTMAPPING_PORTMAPPINGENABLED  "tr069.WANDevice_WANConnectionDevice_WANIPConnection_PortMapping.PortMappingEnabled"
#define UCI_WANDEVICE_PORTMAPPING_INTERNALCLIENT  "tr069.WANDevice_WANConnectionDevice_WANIPConnection_PortMapping.InternalClient"
#define UCI_WANDEVICE_PORTMAPPING_REMOTEHOST  "tr069.WANDevice_WANConnectionDevice_WANIPConnection_PortMapping.RemoteHost"
#define UCI_WANDEVICE_PORTMAPPING_EXTERNALPORT  "tr069.WANDevice_WANConnectionDevice_WANIPConnection_PortMapping.ExternalPort"
#define UCI_WANDEVICE_PORTMAPPING_EPER   "tr069.WANDevice_WANConnectionDevice_WANIPConnection_PortMapping.ExternalPortEndRange"
#define UCI_WANDEVICE_PORTMAPPING_INTERNALPORT  "tr069.WANDevice_WANConnectionDevice_WANIPConnection_PortMapping.InternalPort"
#define UCI_WANDEVICE_PORTMAPPING_PORTMAPPINGPROTOCOL  "tr069.WANDevice_WANConnectionDevice_WANIPConnection_PortMapping.PortMappingProtocol"

/**********************************InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.WANPPPConnection********************************************/
#define UCI_WANDEVICE_WANPPPCONNECTION_ENABLE  "tr069.WANDevice_WANConnectionDevice_WANPPPConnection.Enable"
#define UCI_WANDEVICE_WANPPPCONNECTION_NAME    "tr069.WANDevice_WANConnectionDevice_WANPPPConnection.Name"
#define UCI_WANDEVICE_WANPPPCONNECTION_LASTCONNECTIONERROR    "tr069.WANDevice_WANConnectionDevice_WANPPPConnection.LastConnectionError"
#define UCI_WANDEVICE_WANPPPCONNECTION_USERNAME    "tr069.WANDevice_WANConnectionDevice_WANPPPConnection.Username"
#define UCI_WANDEVICE_WANPPPCONNECTION_PASSWORD    "tr069.WANDevice_WANConnectionDevice_WANPPPConnection.Password"
#define UCI_WANDEVICE_WANPPPCONNECTION_EXTERNALIPADDRESS  "tr069.WANDevice_WANConnectionDevice_WANPPPConnection.ExternalIPAddress"
#define UCI_WANDEVICE_WANPPPCONNECTION_DNSENABLED  "tr069.WANDevice_WANConnectionDevice_WANPPPConnection.DNSEnabled"
#define UCI_WANDEVICE_WANPPPCONNECTION_DNSSERVERS  "tr069.WANDevice_WANConnectionDevice_WANPPPConnection.DNSServers"
#define UCI_WANDEVICE_WANPPPCONNECTION_MACADDRESS  "tr069.WANDevice_WANConnectionDevice_WANPPPConnection.MACAddress"


/**********************************WANDevice_WANConnectionDevice_WANIPConnection_X_ATP_CAR********************************************/
#define UCI_WANDEVICE_X_ATP_CAR_ENABLE  "tr069.WANDevice_WANConnectionDevice_WANIPConnection_X_ATP_CAR.Enable"
#define UCI_WANDEVICE_X_ATP_CAR_UPLINKRATE  "tr069.WANDevice_WANConnectionDevice_WANIPConnection_X_ATP_CAR.UplinkRate"

/**********************************WANDevice_WANConnectionDevice_WANIPConnection_X_ATP_DMZ********************************************/
#define UCI_WANDEVICE_X_ATP_DMZ_DMZENABLE  "tr069.WANDevice_WANConnectionDevice_WANIPConnection_X_ATP_DMZ.DMZEnable"
#define UCI_WANDEVICE_X_ATP_DMZ_DMZHOSTIPADDR  "tr069.WANDevice_WANConnectionDevice_WANIPConnection_X_ATP_DMZ.DMZHostIPAddr"

/**********************************WANDevice_WANConnectionDevice_WANIPConnection_X_ATP_WirelessStatus********************************************/
#define UCI_WANDEVICE_X_ATP_WIRELESSSTATUS_PLMN  "tr069.WANDevice_WANConnectionDevice_WANIPConnection_X_ATP_WirelessStatus.PLMN"
#define UCI_WANDEVICE_X_ATP_WIRELESSSTATUS_SERSTATUS  "tr069.WANDevice_WANConnectionDevice_WANIPConnection_X_ATP_WirelessStatus.ServiceStatus"
#define UCI_WANDEVICE_X_ATP_WIRELESSSTATUS_BANDWIDTH  "tr069.WANDevice_WANConnectionDevice_WANIPConnection_X_ATP_WirelessStatus.Bandwidth"
#define UCI_WANDEVICE_X_ATP_WIRELESSSTATUS_CELLID  "tr069.WANDevice_WANConnectionDevice_WANIPConnection_X_ATP_WirelessStatus.CellID"
#define UCI_WANDEVICE_X_ATP_WIRELESSSTATUS_SIGNALSTRENGTH  "tr069.WANDevice_WANConnectionDevice_WANIPConnection_X_ATP_WirelessStatus.SignalStrength"
#define UCI_WANDEVICE_X_ATP_WIRELESSSTATUS_RSSI  "tr069.WANDevice_WANConnectionDevice_WANIPConnection_X_ATP_WirelessStatus.RSSI"
#define UCI_WANDEVICE_X_ATP_WIRELESSSTATUS_RSRP  "tr069.WANDevice_WANConnectionDevice_WANIPConnection_X_ATP_WirelessStatus.RSRP"
#define UCI_WANDEVICE_X_ATP_WIRELESSSTATUS_RSRQ  "tr069.WANDevice_WANConnectionDevice_WANIPConnection_X_ATP_WirelessStatus.RSRQ"
#define UCI_WANDEVICE_X_ATP_WIRELESSSTATUS_SINR  "tr069.WANDevice_WANConnectionDevice_WANIPConnection_X_ATP_WirelessStatus.SINR"
#define UCI_WANDEVICE_X_ATP_WIRELESSSTATUS_ROAM  "tr069.WANDevice_WANConnectionDevice_WANIPConnection_X_ATP_WirelessStatus.Roam"
#define UCI_WANDEVICE_X_ATP_WIRELESSSTATUS_FREQUENCY "tr069.WANDevice_WANConnectionDevice_WANIPConnection_X_ATP_WirelessStatus.Frequency"
/* add by duanguiyuan 20161118*/
#define UCI_LTEINFO_RSSI 		"lteinfo.info.info_rssi"
#define UCI_LTEINFO_RSRP 		"lteinfo.info.info_rsrp"
#define UCI_LTEINFO_RSRQ 		"lteinfo.info.info_rsrq"
#define UCI_LTEINFO_SINR 		"lteinfo.info.info_sinr"
#define UCI_LTEINFO_CELLID 		"lteinfo.info.physical_community_id"

/**********************************WANDevice_WANConnectionDevice_WANIPConnection_Stats********************************************/
#define UCI_WANDEVICE_IPCONNECTION_ETHERNETBYTESSENT  "tr069.WANDevice_WANConnectionDevice_WANIPConnection_Stats.EthernetBytesSent"
#define UCI_WANDEVICE_IPCONNECTION_ETHERNETBTTESRECEIVED  "tr069.WANDevice_WANConnectionDevice_WANIPConnection_Stats.EthernetBytesReceived"
#define UCI_WANDEVICE_IPCONNECTION_ETHERNETPACKETSSENT  "tr069.WANDevice_WANConnectionDevice_WANIPConnection_Stats.EthernetPacketsSent"
#define UCI_WANDEVICE_IPCONNECTION_ETHERNETPACKETSRECEIVED  "tr069.WANDevice_WANConnectionDevice_WANIPConnection_Stats.EthernetPacketsReceived"
#define UCI_WANDEVICE_IPCONNECTION_EESENT  "tr069.WANDevice_WANConnectionDevice_WANIPConnection_Stats.EthernetErrorsSent"
#define UCI_WANDEVICE_IPCONNECTION_EERECEIVED  "tr069.WANDevice_WANConnectionDevice_WANIPConnection_Stats.EthernetErrorsReceived"
#define UCI_WANDEVICE_IPCONNECTION_EDPSENT "tr069.WANDevice_WANConnectionDevice_WANIPConnection_Stats.EthernetDiscardPacketsSent"
#define UCI_WANDEVICE_IPCONNECTION_EDPRECEVIVED "tr069.WANDevice_WANConnectionDevice_WANIPConnection_Stats.EthernetDiscardPacketsReceived"
#define UCI_WANDEVICE_IPCONNECTION_X_ATP_EURATE  "tr069.WANDevice_WANConnectionDevice_WANIPConnection_Stats.X_ATP_EthernetUplinkRate"
#define UCI_WANDEVICE_IPCONNECTION_X_ATP_EDRATE  "tr069.WANDevice_WANConnectionDevice_WANIPConnection_Stats.X_ATP_EthernetDownlinkRate"

/**********************************retrieveTR69StatusItems********************************************/
#define UCI_RETRIEVETR69STATUSITEMS_DCKEY  "tr069.retrieveTR69StatusItems.downloadCommandKey"
#define UCI_RETRIEVETR69STATUSITEMS_REBOOTCONMANDKEY  "tr069.retrieveTR69StatusItems.rebootCommandKey"
#define UCI_RETRIEVETR69STATUSITEMS_NEWPARAMETERKEY  "tr069.retrieveTR69StatusItems.newParameterKey"
#define UCI_RETRIEVETR69STATUSITEMS_DLFAULTSTATUS  "tr069.retrieveTR69StatusItems.dlFaultStatus"
#define UCI_RETRIEVETR69STATUSITEMS_STARTDLTIME  "tr069.retrieveTR69StatusItems.startDLTime"
#define UCI_RETRIEVETR69STATUSITEMS_ENDDLTIME  "tr069.retrieveTR69StatusItems.endDLTime"
#define UCI_RETRIEVETR69STATUSITEMS_DLFAULTMSG  "tr069.retrieveTR69StatusItems.dlFaultMsg"
#define UCI_RETRIEVETR69STATUSITEMS_INFORMSTATE  "tr069.retrieveTR69StatusItems.informState"
#define UCI_RETRIEVETR69STATUSITEMS_INFORMCWMPENABLE  "tr069.retrieveTR69StatusItems.informCWMPEnable"


/********************************** InternetGatewayDevice.WANDevice.{i}.X_ATP_AlarmInstance ********************************************/
#define UCI_WANDevice_X_ATP_Alarm_CNT  				"tr069.X_ATP_Alarm.cnt"
#define UCI_WANDevice_X_ATP_Alarm_LAN1  			"tr069.X_ATP_Alarm.lan1"
#define UCI_WANDevice_X_ATP_Alarm_LAN2  			"tr069.X_ATP_Alarm.lan2"
#define UCI_WANDevice_X_ATP_Alarm_LAN3  			"tr069.X_ATP_Alarm.lan3"
#define UCI_WANDevice_X_ATP_Alarm_LAN4  			"tr069.X_ATP_Alarm.lan4"
#define UCI_WANDevice_X_ATP_Alarm_LanCheck  		"tr069.X_ATP_Alarm.LanCheck"
#define UCI_WANDevice_X_ATP_Alarm_STATUS_N  		"tr069.X_ATP_Alarm.stat_lan"
#define UCI_WANDevice_X_ATP_Alarm_TIME_N  			"tr069.X_ATP_Alarm.time_lan"

#define UCI_ACSCONTACTEDSTATE  "tr069.TauacsContactedState.acsContactedState"
#endif
