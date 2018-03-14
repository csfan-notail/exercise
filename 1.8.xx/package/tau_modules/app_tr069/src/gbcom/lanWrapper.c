/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : lanWrapper.c
  版 本 号   : 初稿
  作    者   : zhangjiasi
  生成日期   : 2015年6月9日
  最近修改   :
  功能描述   : GBCOM 封装的lan相关接口实现
  函数列表   :
  修改历史   :
  1.日    期   : 2015年6月9日
    作    者   : zhangjiasi
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include "utils.h"
#include "ap_types.h"
#include "om_tr69c.h"
#include "lanDeviceParams.h"
/*Add by zhanghao 获取配置文件内容 2016/6/15 begin*/
#include "uci_config.h"
#include "tr69_lua.h"
/*Add by zhanghao 获取配置文件内容 end*/
//#include "util/util.h"
#define TAUARRY_LEN 128
/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
extern int g_log_level;

extern TRxObjNode  LANConfigSecurityDesc[];
extern TRxObjNode  LANDeviceInstanceDesc[];
extern TRxObjNode LANDeviceLANHostConfigManagementDesc[];
extern TRxObjNode LANDeviceLANHostConfigManagementIPInterfaceInstanceDesc[];
extern TRxObjNode LANDeviceLANEthernetInterfaceConfigInstanceDesc[];
extern TRxObjNode LANDeviceLANEthernetInterfaceConfigStatsDesc[];
extern TRxObjNode LANDeviceHostsDesc[];
extern TRxObjNode DHCPRelaySeverConfigDesc[];

extern TRxObjNode LANDeviceWLANConfigurationInstanceDesc[];
extern TRxObjNode X_ATP_WlanMacFilterDescObj[];
extern TRxObjNode AssociatedDeviceDescObj[];
extern TRxObjNode WEPKeyDescObj[];
extern TRxObjNode PreSharedKeyDescObj[];
extern TRxObjNode DHCPStaticAddressDesc[];
	
/* 允许lan口接入ACS服务器的密码 */
TRX_STATUS setConfigPassword(const char *value)
{
   /*  OM_TR69C_LAN_CONFIG_SECURITY_T stLanCfgSec;
    memcpy(stLanCfgSec.acLanCfgPasswd,value,strlen(value));
    
    if(OM_E_NONE != om_net_tr069_lan_config_serurity_set(&stLanCfgSec))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);
        return TRX_ERR;
    } */

	return TRX_OK;
}
TRX_STATUS getConfigPassword(char **value)
{
    /* OM_TR69C_LAN_CONFIG_SECURITY_T stLanCfgSec;

    if(OM_E_NONE != om_net_tr069_lan_config_serurity_get(&stLanCfgSec))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);
        return TRX_ERR;
    }      

    *value = strdup(stLanCfgSec.acLanCfgPasswd); */
	return TRX_OK;
}

/* LAN侧WLAN接口配置实例数目,每个ssid有一个配置实例 */
TRX_STATUS getLANDevice_LANWLANConfigurationNumberOfEntries(char **ppcValue)
{
	
	char  acConfigurationNumberOfEntries[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_LANWLANCONFIGRATIONNUMBEROFENTRIES,acConfigurationNumberOfEntries))
	{
		*ppcValue = strdup("0");
		slog(TR69C_LOG_ERR,"ERROR");
		return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acConfigurationNumberOfEntries);
				
	}	

	//*value = strdup(" ");
	return TRX_OK;
}

/* The MAC address associated with the IP interface
modeled by this LANDevice instance. 
也就是ARP返回的mac地址。此处默认认为为LAN口地址 */
TRX_STATUS getLANDeviceLANHostConfigManagement_MACAddress(char **value)
{
    /* OM_TR69C_LAN_DHCP_PATAM_T stDHCPInfo = {0};
    CHAR acTemp[CHAR_LEN_32] = {0};

    stDHCPInfo.eFlag = TR69C_LAN_MAC_ADDR;
    if(OM_E_NONE != om_net_tr069_dhcp_get(&stDHCPInfo))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);
        return TRX_ERR;
    }
    sprintf(acTemp,MAC_FMT,MAC_ARG(stDHCPInfo.stDhcpCfg.aucMac));
    *value = strdup(acTemp); */

	return TRX_OK;
}

/* dhcp server配置使能。True为有效，如果设置为false，则cpe将恢复到默认配置 */
TRX_STATUS setLANDeviceLANHostConfigManagement_DHCPServerConfigurable(const char *value)
{
    /* OM_TR69C_LAN_DHCP_PATAM_T stDHCPInfo = {0};
   // CHAR acTemp[CHAR_LEN_32] = {0};
 	slog(TR69C_LOG_DEBUG ,"[tr69c:%s,%d] set DHCPServerConfigurable",__func__,__LINE__);
    stDHCPInfo.eFlag = TR69C_LAN_DHCP_CONFIGURABLE;
    stDHCPInfo.stDhcpCfg.iDhcpConfigurable = atoi(value);

	slog(TR69C_LOG_DEBUG ,"[tr69c:%s,%d] set DHCPServerConfigurable %d",__func__,__LINE__,stDHCPInfo.stDhcpCfg.iDhcpConfigurable);
	
    if(OM_E_NONE != om_net_tr069_dhcp_set(&stDHCPInfo))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);
        return TRX_ERR;
    } */
    
	return TRX_OK;
}
TRX_STATUS getLANDeviceLANHostConfigManagement_DHCPServerConfigurable(char **value)
{
   /*  OM_TR69C_LAN_DHCP_PATAM_T stDHCPInfo = {0};
    CHAR acTemp[CHAR_LEN_32] = {0};

    stDHCPInfo.eFlag = TR69C_LAN_DHCP_CONFIGURABLE;
    if(OM_E_NONE != om_net_tr069_dhcp_get(&stDHCPInfo))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);
        return TRX_ERR;
    }
    sprintf(acTemp,"%d",stDHCPInfo.stDhcpCfg.iDhcpConfigurable);
    *value = strdup(acTemp); */
	return TRX_OK;
}

/* Enables or disables the DHCP server on the LAN
   interface. */
TRX_STATUS setLANDeviceLANHostConfigManagement_DHCPServerEnable(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_DHCPSERVERENABLE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
  	
    return TRX_OK;
}
TRX_STATUS getLANDeviceLANHostConfigManagement_DHCPServerEnable(char **ppcValue)
{
	char  acDHCPServerEnable[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_DHCPSERVERENABLE,acDHCPServerEnable))
	{
		*ppcValue = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acDHCPServerEnable);
		
	}	   
    return TRX_OK;
}

/* 标明当前的dhcp 服务器是执行服务器功能（false）还是relay功能（true） */
TRX_STATUS getLANDeviceLANHostConfigManagement_DHCPRelay(char **value)
{
    /* 暂不支持relay功能 */
	*value = strdup("0");
	return TRX_OK;
}

/* 地址池地址 */
TRX_STATUS setLANDeviceLANHostConfigManagement_MinAddress(const char *pcValue)
{
    /* OM_TR69C_LAN_DHCP_PATAM_T stDHCPInfo = {0};

    stDHCPInfo.eFlag = TR69C_LAN_DHCP_MIN_ADDR;
    //sscanf(value, IP_FMT, IP_ARG(stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.aucStartIp));  
    util_str_to_ip((char *)value,stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.aucStartIp);
    if(OM_E_NONE != om_net_tr069_dhcp_set(&stDHCPInfo))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);
        return TRX_ERR;
    } */
    if(UCI_OK !=uci_config_set(UCI_LANDEVICE_MINADDR,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    return TRX_OK;
}
TRX_STATUS getLANDeviceLANHostConfigManagement_MinAddress(char **ppcValue)
{
   /*  OM_TR69C_LAN_DHCP_PATAM_T stDHCPInfo = {0};
    CHAR szTemp[CHAR_LEN_32] = {0};

    stDHCPInfo.eFlag = TR69C_LAN_DHCP_MIN_ADDR;  

    if(OM_E_NONE != om_net_tr069_dhcp_get(&stDHCPInfo))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    sprintf(szTemp,IP_FMT,IP_ARG(stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.aucStartIp));
    *value = strdup(szTemp); */
    char  acMinAddress[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_MINADDR,acMinAddress))
	{
		*ppcValue = strdup("0.0.0.0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acMinAddress);
		
	}	   
    return TRX_OK;
}
TRX_STATUS setLANDeviceLANHostConfigManagement_MaxAddress(const char *pcValue)
{
   /*  OM_TR69C_LAN_DHCP_PATAM_T stDHCPInfo = {0};

    stDHCPInfo.eFlag = TR69C_LAN_DHCP_MAX_ADDR;

	//sscanf(value, IP_FMT, IP_ARG(stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.aucEndIp)); 
	util_str_to_ip((char *)value,stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.aucEndIp);
	
    if(OM_E_NONE != om_net_tr069_dhcp_set(&stDHCPInfo))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);
        return TRX_ERR;
    } */
    if(UCI_OK !=uci_config_set(UCI_LANDEVICE_MAXADDR,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    return TRX_OK;
}
TRX_STATUS getLANDeviceLANHostConfigManagement_MaxAddress(char **ppcValue)
{
   /*  OM_TR69C_LAN_DHCP_PATAM_T stDHCPInfo = {0};
    CHAR szTemp[CHAR_LEN_32] = {0};

    stDHCPInfo.eFlag = TR69C_LAN_DHCP_MAX_ADDR;  

    if(OM_E_NONE != om_net_tr069_dhcp_get(&stDHCPInfo))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    sprintf(szTemp,IP_FMT,IP_ARG(stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.aucEndIp));
    *value = strdup(szTemp); */
    char  acMaxAddress[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_MAXADDR,acMaxAddress))
	{
		*ppcValue = strdup("0.0.0.0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acMaxAddress);
		
	}	   
    return TRX_OK;
}
TRX_STATUS setLANDeviceLANHostConfigManagement_ReservedAddresses(const char *value)
{
    /* OM_TR69C_LAN_DHCP_PATAM_T stDHCPInfo = {0};

    stDHCPInfo.eFlag = TR69C_LAN_DHCP_RESERVED_ADDR;

	 sprintf(stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.acReservedIp,"%s",value);
	 slog(TR69C_LOG_DEBUG,"[tr69c:%s,%d] ReservedIP = %s",__func__,__LINE__,stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.acReservedIp);
	 
	// memcpy(stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.acReservedIp,value,strlen(stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.acReservedIp));
	//util_str_to_ip(value,stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.acReservedIp);
	
    if(OM_E_NONE != om_net_tr069_dhcp_set(&stDHCPInfo))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);
        return TRX_ERR;
    } */
	return TRX_OK;
}
TRX_STATUS getLANDeviceLANHostConfigManagement_ReservedAddresses(char **value)
{
   /*  OM_TR69C_LAN_DHCP_PATAM_T stDHCPInfo = {0};
 //   CHAR szTemp[CHAR_LEN_32] = {0};
    
    stDHCPInfo.eFlag = TR69C_LAN_DHCP_RESERVED_ADDR;  
    if(OM_E_NONE != om_net_tr069_dhcp_get(&stDHCPInfo))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    *value = strdup(stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.acReservedIp); */
	return TRX_OK;
}
TRX_STATUS setLANDeviceLANHostConfigManagement_SubnetMask(const char *value)
{
  /*   OM_TR69C_LAN_DHCP_PATAM_T stDHCPInfo = {0};

    stDHCPInfo.eFlag = TR69C_LAN_DHCP_NETMASK;

   // sscanf(value, IP_FMT, IP_ARG(stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.aucNetMask));  
	util_str_to_ip((char *)value,stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.aucNetMask);
   
    if(OM_E_NONE != om_net_tr069_dhcp_set(&stDHCPInfo))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);
        return TRX_ERR;
    } */
    return TRX_OK;
}
TRX_STATUS getLANDeviceLANHostConfigManagement_SubnetMask(char **value)
{
   /*  OM_TR69C_LAN_DHCP_PATAM_T stDHCPInfo = {0};
    CHAR szTemp[CHAR_LEN_32] = {0};

    stDHCPInfo.eFlag = TR69C_LAN_DHCP_NETMASK;  

    if(OM_E_NONE != om_net_tr069_dhcp_get(&stDHCPInfo))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    sprintf(szTemp,IP_FMT,IP_ARG(stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.aucNetMask));
    *value = strdup(szTemp); */
    return TRX_OK;
}
TRX_STATUS setLANDeviceLANHostConfigManagement_DNSServers(const char *value)
{
   /*  OM_TR69C_LAN_DHCP_PATAM_T stDHCPInfo = {0};
    CHAR szMaster[CHAR_LEN_64] = {0}; 
    CHAR szSlave[CHAR_LEN_64] = {0}; 
    CHAR *pcTmp = NULL;
    CHAR szTemp[CHAR_LEN_128] = {0};

    //区分当时一共有几个dns服务器 


	slog(TR69C_LOG_DEBUG ,"[tr69c:%s,%d] DNSServers:%s",__func__,__LINE__,value);
    memcpy(szTemp,value,strlen(value));
    if ( NULL == (pcTmp = strstr(value,",")) )
    {
     //  sscanf(value, IP_FMT, IP_ARG(stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.aucMasterDns));  
		util_str_to_ip((char *)value,stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.aucMasterDns);

	 	stDHCPInfo.eFlag = TR69C_LAN_DHCP_DNS_SRV_MASTER;
		
    	slog(TR69C_LOG_DEBUG ,"[tr69c:%s,%d] DNSServers 1 :"IP_FMT,__func__,__LINE__,
			IP_ARG(stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.aucMasterDns));

		if(OM_E_NONE != om_net_tr069_dhcp_set(&stDHCPInfo))
        {
            slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);
            return TRX_ERR;
        }
		slog(TR69C_LOG_DEBUG ,"[tr69c:%s,%d] DNSServers 1 end",__func__,__LINE__);
    }
    else
    {
    	slog(TR69C_LOG_DEBUG ,"[tr69c:%s,%d] DNSServers 2 :%s ",__func__,__LINE__,value);
        strncpy(szMaster,value,strlen(value)-strlen(pcTmp));
        strcpy(szSlave,++pcTmp);
		slog(TR69C_LOG_DEBUG ,"[tr69c:%s,%d] DNSServers 2 szMster:%s szSlave=%s",__func__,__LINE__,szMaster,szSlave);

	   // sscanf(value, IP_FMT, IP_ARG(stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.aucMasterDns));
		util_str_to_ip(szMaster,stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.aucMasterDns);

		stDHCPInfo.eFlag = TR69C_LAN_DHCP_DNS_SRV_MASTER;
    
        if(OM_E_NONE != om_net_tr069_dhcp_set(&stDHCPInfo))
        {
            slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);
            return TRX_ERR;
        }
      // sscanf(value, IP_FMT, IP_ARG(stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.aucSlaveDns));
      	util_str_to_ip(szSlave,stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.aucSlaveDns);

		stDHCPInfo.eFlag = TR69C_LAN_DHCP_DNS_SRV_SLAVE;
    
        if(OM_E_NONE != om_net_tr069_dhcp_set(&stDHCPInfo))
        {
            slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);
            return TRX_ERR;
        }
		slog(TR69C_LOG_DEBUG ,"[tr69c:%s,%d] DNSServers 2 : "IP_FMT",.."IP_FMT,__func__,__LINE__, 
			IP_ARG(stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.aucMasterDns),
                IP_ARG(stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.aucSlaveDns));
    } */
    
    return TRX_OK;
}
TRX_STATUS getLANDeviceLANHostConfigManagement_DNSServers(char **value)
{
   /*  OM_TR69C_LAN_DHCP_PATAM_T stDHCPInfo = {0};
    CHAR szTemp[CHAR_LEN_64] = {0};

    stDHCPInfo.eFlag = TR69C_LAN_DHCP_DNS_SRV_MASTER;  

    if(OM_E_NONE != om_net_tr069_dhcp_get(&stDHCPInfo))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }
//	slog(TR69C_LOG_DEBUG ,"[tr69c:%s,%d] DNSServers get Master ="IP_FMT,__func__,__LINE__,
	//	IP_ARG(stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.aucMasterDns));


    stDHCPInfo.eFlag = TR69C_LAN_DHCP_DNS_SRV_SLAVE;  

    if(OM_E_NONE != om_net_tr069_dhcp_get(&stDHCPInfo))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }
	//slog(TR69C_LOG_DEBUG ,"[tr69c:%s,%d] DNSServers get Slave ="IP_FMT,__func__,__LINE__,
	//	IP_ARG(stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.aucSlaveDns));

	
    sprintf(szTemp,IP_FMT","IP_FMT,
                IP_ARG(stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.aucMasterDns),
                IP_ARG(stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.aucSlaveDns));
    *value = strdup(szTemp); */
    return TRX_OK;
}
TRX_STATUS setLANDeviceLANHostConfigManagement_DomainName(const char *value)
{
    //do nothing
    return TRX_OK;
}
TRX_STATUS getLANDeviceLANHostConfigManagement_DomainName(char **value)
{
    //do nothing
    *value = strdup("");
    return TRX_OK;
}

TRX_STATUS setLANDeviceLANHostConfigManagement_IPRouters(const char *value)
{
    /* OM_TR69C_LAN_DHCP_PATAM_T stDHCPInfo = {0};
	
	slog(TR69C_LOG_DEBUG ,"[tr69c:%s,%d] set  ipRouters  nvram start",__func__,__LINE__);
		
    stDHCPInfo.eFlag = TR69C_LAN_DHCP_IPROUTERS;

	slog(TR69C_LOG_DEBUG ,"[tr69c:%s,%d] set  ipRouters  nvram value = %s",__func__,__LINE__,value);

	util_str_to_ip((char *)value,stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.aucGw);
    //sscanf(value, IP_FMT, IP_ARG(stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.aucGw));
    
	slog(TR69C_LOG_DEBUG ,"[tr69c:%s,%d] set  ipRouters ip =",__func__,__LINE__);

	slog(TR69C_LOG_DEBUG ,IP_FMT, IP_ARG(stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.aucGw));

	if(OM_E_NONE != om_net_tr069_dhcp_set(&stDHCPInfo))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);
        return TRX_ERR;
    }
	
	slog(TR69C_LOG_DEBUG ,"[tr69c:%s,%d] set  ipRouters  nvram end",__func__,__LINE__); */
    return TRX_OK;
}
TRX_STATUS getLANDeviceLANHostConfigManagement_IPRouters(char **value)
{
   /*  OM_TR69C_LAN_DHCP_PATAM_T stDHCPInfo = {0};
    CHAR szTemp[CHAR_LEN_32] = {0};

    stDHCPInfo.eFlag = TR69C_LAN_DHCP_IPROUTERS;  

    if(OM_E_NONE != om_net_tr069_dhcp_get(&stDHCPInfo))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    sprintf(szTemp,IP_FMT,IP_ARG(stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.aucGw));
    *value =strdup(szTemp); */
    return TRX_OK;
}
TRX_STATUS setLANDeviceLANHostConfigManagement_DHCPLeaseTime(const char *pcValue)
{
    /* OM_TR69C_LAN_DHCP_PATAM_T stDHCPInfo = {0};

    stDHCPInfo.eFlag = TR69C_LAN_DHCP_LEASE_TIME;
    stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.ulLease = atoi(value);
    
    if(OM_E_NONE != om_net_tr069_dhcp_set(&stDHCPInfo))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);
        return TRX_ERR;
    } */
    if(UCI_OK !=uci_config_set(UCI_LANDEVICE_DHCPLEASETIME,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    return TRX_OK;
}
TRX_STATUS getLANDeviceLANHostConfigManagement_DHCPLeaseTime(char **ppcValue)
{
   /*  OM_TR69C_LAN_DHCP_PATAM_T stDHCPInfo = {0};
    CHAR szTemp[CHAR_LEN_32] = {0};

    stDHCPInfo.eFlag = TR69C_LAN_DHCP_LEASE_TIME;    
    if(OM_E_NONE != om_net_tr069_dhcp_get(&stDHCPInfo))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);
        return TRX_ERR;
    }
    sprintf(szTemp,"%d",stDHCPInfo.stDhcpCfg.stDhcpv4Cfg.ulLease);
    *value =strdup(szTemp);
     */
    char  acDHCPLeaseTime[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_DHCPLEASETIME,acDHCPLeaseTime))
	{
		*ppcValue = strdup("0.0.0.0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acDHCPLeaseTime);
		
	}	   
    return TRX_OK;
}

TRX_STATUS setLANDeviceLANHostConfigManagement_PassthroughLease(const char *pcValue)
{
    
    return TRX_OK;
}
TRX_STATUS getLANDeviceLANHostConfigManagement_PassthroughLease(char **ppcValue)
{
  
    return TRX_OK;
}
/* 逗号隔开的mac列表。MACAddressControlEnabled为1的话，该值有线，表示列表中
   的mac被允许接入该connection */
TRX_STATUS setLANDeviceLANHostConfigManagement_AllowedMACAddresses(const char *value)
{
   /*  OM_TR69C_LAN_DHCP_PATAM_T stDHCPInfo = {0};

 	slog(TR69C_LOG_DEBUG ,"[tr69c:%s,%d] set allow",__func__,__LINE__);
    stDHCPInfo.eFlag = TR69C_LAN_ALLOWED_MAC_ADDR;
    memcpy(stDHCPInfo.stDhcpCfg.szAllowedMac,value,sizeof(stDHCPInfo.stDhcpCfg.szAllowedMac));
    
    if(OM_E_NONE != om_net_tr069_dhcp_set(&stDHCPInfo))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);
        return TRX_ERR;
    } */
	return TRX_OK;
}
TRX_STATUS getLANDeviceLANHostConfigManagement_AllowedMACAddresses(char **value)
{
/* 	OM_TR69C_LAN_DHCP_PATAM_T stDHCPInfo = {0};

    stDHCPInfo.eFlag = TR69C_LAN_ALLOWED_MAC_ADDR;    
    if(OM_E_NONE != om_net_tr069_dhcp_get(&stDHCPInfo))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);
        return TRX_ERR;
    }
    *value =strdup(stDHCPInfo.stDhcpCfg.szAllowedMac); */
	return TRX_OK;
}

/* lan接口实例表，默认为1 */
TRX_STATUS getLANDeviceLANHostConfigManagement_IPInterfaceNumberOfEntries(char **ppcValue)
{
	char  acIPInterfaceNumberOfEntries[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_IPINTERFACENUMBEROFENTRIES,acIPInterfaceNumberOfEntries))
	{
		*ppcValue = strdup("1");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acIPInterfaceNumberOfEntries);
		
	}	   
	//*value = strdup("1");/* 默认1个 */
	return TRX_OK;
}







/*--------add by wangminlgin-------------*/
TRX_STATUS setLANDeviceLANHostConfigManagement_DHCPStaticAddressNumberOfEntries(const char *pcValue)
{
    if(UCI_OK !=uci_config_set(UCI_LANDEVICE_DHCPSTATICADDRESSNUMBEROFENTRIES,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
	return TRX_OK;
}

TRX_STATUS getLANDeviceLANHostConfigManagement_DHCPStaticAddressNumberOfEntries(char **ppcValue)
{
	char  acNumberOfEntries[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_DHCPSTATICADDRESSNUMBEROFENTRIES,acNumberOfEntries))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acNumberOfEntries);
		
	}	   
	return TRX_OK;
}

TRX_STATUS setLANDeviceLANHostConfigManagement_DHCPRelayEnable(const char *pcValue)
{
    
	return TRX_OK;
}
TRX_STATUS getLANDeviceLANHostConfigManagement_DHCPRelayEnable(char **ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS setLANDeviceLANHostConfigManagement_ServerIPAddr(const char *pcValue)
{
    
	return TRX_OK;
}
TRX_STATUS getLANDeviceLANHostConfigManagement_ServerIPAddr(char **ppcValue)
{
	
	return TRX_OK;
}


TRX_STATUS setLANDeviceLANHostConfigManagement_X_ATP_VlanID(const char *pcValue)
{
   if(UCI_OK !=uci_config_set(UCI_LANDEVICE_X_ATP_VLANID,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
	return TRX_OK;
}

TRX_STATUS getLANDeviceLANHostConfigManagement_X_ATP_VlanID(char **ppcValue)
{
	char  acX_ATP_VlanID[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_X_ATP_VLANID,acX_ATP_VlanID))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_VlanID);
		
	}	   
	return TRX_OK;
}


TRX_STATUS setDHCPStaticAddressEnable(const char *pcValue)
{
    if(UCI_OK !=uci_config_set(UCI_LANDEVICE_LANHOSTCONFIGMANAGEMENT_DHCPSTATICADDR_ENABLE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
	return TRX_OK;
}
TRX_STATUS getDHCPStaticAddressEnable(char **ppcValue)
{
	char  acAddressEnable[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_LANHOSTCONFIGMANAGEMENT_DHCPSTATICADDR_ENABLE,acAddressEnable))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acAddressEnable);
		
	}	   
	return TRX_OK;
}



TRX_STATUS setDHCPStaticAddressChaddr(const char *pcValue)
{
    if(UCI_OK !=uci_config_set(UCI_LANDEVICE_LANHOSTCONFIGMANAGEMENT_DHCPSTATICADDR_CHADDR,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
	return TRX_OK;
}
TRX_STATUS getDHCPStaticAddressChaddr(char **ppcValue)
{
	char  acAddressChaddr[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_LANHOSTCONFIGMANAGEMENT_DHCPSTATICADDR_ENABLE,acAddressChaddr))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acAddressChaddr);
		
	}	   
	return TRX_OK;
}


TRX_STATUS setDHCPStaticAddressYiaddr(const char *pcValue)
{
    if(UCI_OK !=uci_config_set(UCI_LANDEVICE_LANHOSTCONFIGMANAGEMENT_DHCPSTATICADDR_YIADDR,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
	return TRX_OK;
}
TRX_STATUS getDHCPStaticAddressYiaddr(char **ppcValue)
{
	char  acAddressYiaddr[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_LANHOSTCONFIGMANAGEMENT_DHCPSTATICADDR_YIADDR,acAddressYiaddr))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acAddressYiaddr);
		
	}	   
	return TRX_OK;
}



TRX_STATUS setDHCPStaticAddressX_ATP_valid(const char *pcValue)
{
    if(UCI_OK !=uci_config_set(UCI_LANDEVICE_LANHOSTCONFIGMANAGEMENT_DHCPSTATICADDR_X_ATP_VALID,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
	return TRX_OK;
}
TRX_STATUS getDHCPStaticAddressX_ATP_valid(char **ppcValue)
{
	char  acX_ATP_valid[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_LANHOSTCONFIGMANAGEMENT_DHCPSTATICADDR_X_ATP_VALID,acX_ATP_valid))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_valid);
		
	}	   
	return TRX_OK;
}



TRX_STATUS setLANDeviceLANHostConfigManagementIPInterface_X_ATP_RouteProtocolRx(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_X_ATP_ROUTEPROTOCOLRX,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getLANDeviceLANHostConfigManagementIPInterface_X_ATP_RouteProtocolRx(char **ppcValue)
{
	char  acX_ATP_RouteProtocolRx[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_X_ATP_ROUTEPROTOCOLRX,acX_ATP_RouteProtocolRx))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_RouteProtocolRx);
		
	}	   
	return TRX_OK;
}



TRX_STATUS setLANDeviceLANHostConfigManagementIPInterface_X_ATP_RIPOperation(const char *pcValue)
{
	if(UCI_OK != uci_config_set(UCI_LANDEVICE_X_ATP_RIPOPERATION,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
	return TRX_OK;
}
TRX_STATUS getLANDeviceLANHostConfigManagementIPInterface_X_ATP_RIPOperation(char **ppcValue)
{
	char  acX_ATP_RIPOperation[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_X_ATP_RIPOPERATION,acX_ATP_RIPOperation))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_RIPOperation);
		
	}	   
	return TRX_OK;
}


TRX_STATUS setLANDeviceLANHostConfigManagementIPInterface_X_ATP_Pre_RouteProtocol(const char *pcValue)
{
	if(UCI_OK != uci_config_set(UCI_LANDEVICE_X_ATP_Pre_ROUTEPROTOCOL,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getLANDeviceLANHostConfigManagementIPInterface_X_ATP_Pre_RouteProtocol(char **ppcValue)
{
	char  acX_ATP_Pre_RouteProtocol[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_X_ATP_Pre_ROUTEPROTOCOL,acX_ATP_Pre_RouteProtocol))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_Pre_RouteProtocol);
		
	}	   
	return TRX_OK;
}



TRX_STATUS getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_PacketsSentError(char **ppcValue)
{
	char  acX_ATP_PacketsSentErro[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_X_ATP_PACKETSSENTERROR,acX_ATP_PacketsSentErro))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_PacketsSentErro);
	
	}	   
	return TRX_OK;
}

TRX_STATUS getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_PacketsSentDrops(char **ppcValue)
{
	char acX_ATP_PacketsSentDrops[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_X_ATP_PACKETSSENTDROPS,acX_ATP_PacketsSentDrops))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_PacketsSentDrops);
	
	}	   
	return TRX_OK;
}


TRX_STATUS getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_Timestamp(char **ppcValue)
{
	char  acX_ATP_Timestamp[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_X_ATP_TIMESTAMP,acX_ATP_Timestamp))
	{
		*ppcValue = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_Timestamp);
	
	}	   
	return TRX_OK;
}






TRX_STATUS setSSIDAdvertisementEnabled(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIG_SSIDADVERTISEMENTENABLED,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
  	
	return TRX_OK;
}
TRX_STATUS getSSIDAdvertisementEnabled(char **ppcValue)
{
	char  acSSIDEnabled[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIG_SSIDADVERTISEMENTENABLED,acSSIDEnabled))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acSSIDEnabled);
	}	   
	return TRX_OK;
}



TRX_STATUS setWLANConfigurationEnable(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_ENABLED,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
   	
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationEnable(char **ppcValue)
{
	char  acWlanConfigurationEnable[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_ENABLED,acWlanConfigurationEnable))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWlanConfigurationEnable);
		
	}	   
	return TRX_OK;
}




TRX_STATUS getWLANConfigurationStatus(char **ppcValue)
{
	char  acWLANConfigurationStatus[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_STATUS,acWLANConfigurationStatus))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWLANConfigurationStatus);
	}	   
	return TRX_OK;
}


TRX_STATUS getWLANConfigurationBSSID(char **ppcValue)
{
	char  acWLANConfigurationBSSID[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_BSSID,acWLANConfigurationBSSID))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWLANConfigurationBSSID);
	
	}	   
	return TRX_OK;
}





TRX_STATUS setWLANConfigurationMaxBitRate(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_MAXBITRATE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
   	
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationMaxBitRate(char **ppcValue)
{
	char  acMaxBitRate[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_MAXBITRATE,acMaxBitRate))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acMaxBitRate);
	
	}	   
	return TRX_OK;
}




TRX_STATUS setWLANConfigurationChannel(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_CHANNEL,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
   	
    
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationChannel(char **ppcValue)
{
	char  acChannel[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_CHANNEL,acChannel))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acChannel);
	
	}	   
	return TRX_OK;
}



TRX_STATUS setWLANConfigurationAutoChannelEnable(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_AUTOCHANNELENABLE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationAutoChannelEnable(char **ppcValue)
{
	char  acAutoChannelEnable[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_AUTOCHANNELENABLE,acAutoChannelEnable))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acAutoChannelEnable);
	
	}	   
	return TRX_OK;
}



TRX_STATUS setWLANConfigurationRegulatoryDomain(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_REGULATORYDOMAIN,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationRegulatoryDomain(char **ppcValue)
{
	char  acRegulatoryDomain[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_REGULATORYDOMAIN,acRegulatoryDomain))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acRegulatoryDomain);
	
	}	   
	return TRX_OK;
}



TRX_STATUS setWLANConfigurationSSID(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_REGULATORYDOMAIN,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationSSID(char **ppcValue)
{
	char  acRegulatoryDomain[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_REGULATORYDOMAIN,acRegulatoryDomain))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acRegulatoryDomain);
	
	}	   
	return TRX_OK;
}



TRX_STATUS setWLANConfigurationBeaconType(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_BEACONTYPE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationBeaconType(char **ppcValue)
{
	char acBeaconType[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_BEACONTYPE,acBeaconType))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acBeaconType);
	
	}	   
	return TRX_OK;
}



TRX_STATUS setWLANConfigurationMACAddressControlEnabled(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_MACADDRCONTROLENABLE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationMACAddressControlEnabled(char **ppcValue)
{
	char  acMACAddressControlEnabled[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_MACADDRCONTROLENABLE,acMACAddressControlEnabled))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acMACAddressControlEnabled);
	
	}	   
	return TRX_OK;
}



TRX_STATUS setWLANConfigurationStandard(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_STANDARD,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationStandard(char **ppcValue)
{
	char  acStandard[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_STANDARD,acStandard))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
	}
	else{
		*ppcValue = strdup(acStandard);
	
	}	   
	return TRX_OK;
}



TRX_STATUS setWLANConfigurationWEPKeyIndex(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_WEPKEYINDEX,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationWEPKeyIndex(char **ppcValue)
{
	char acWEPKeyIndex[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_WEPKEYINDEX,acWEPKeyIndex))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWEPKeyIndex);
	
	}	   
	return TRX_OK;
}


TRX_STATUS setWLANConfigurationWEPEncryptionLevel(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_WEPENCRYPTIONLEVEL,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationWEPEncryptionLevel(char **ppcValue)
{
	char acWEPEncryptionLevel[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_WEPENCRYPTIONLEVEL,acWEPEncryptionLevel))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWEPEncryptionLevel);
	
	}	   
	return TRX_OK;
}



TRX_STATUS setWLANConfigurationBasicEncryptionModes(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_BASICENCRYPTIONMODES,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationBasicEncryptionModes(char **ppcValue)
{
	char acBasicEncryptionModes[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_BASICENCRYPTIONMODES,acBasicEncryptionModes))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acBasicEncryptionModes);
	
	}	   
	return TRX_OK;
}



TRX_STATUS setWLANConfigurationBasicAuthenticationMode(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_BASICAUTHMODE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationBasicAuthenticationMode(char **ppcValue)
{
	char acBasicAuthenticationMode[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_BASICAUTHMODE,acBasicAuthenticationMode))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acBasicAuthenticationMode);
	
	}	   
	return TRX_OK;
}


TRX_STATUS setWLANConfigurationWPAEncryptionModes(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_WPAENCTYPTIONMODES,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationWPAEncryptionModes(char **ppcValue)
{
	char  acWPAEncryptionModes[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_WPAENCTYPTIONMODES,acWPAEncryptionModes))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWPAEncryptionModes);
	
	}	   
	return TRX_OK;
}




TRX_STATUS setWLANConfigurationWPAAuthenticationMode(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_WPAAUTHMODE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationWPAAuthenticationMode(char **ppcValue)
{
	char acWPAAuthenticationMode[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_WPAAUTHMODE,acWPAAuthenticationMode))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWPAAuthenticationMode);
	
	}	   
	return TRX_OK;
}



TRX_STATUS setWLANConfigurationIEEE11iEncryptionModes(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_IEEE11IENCRYPTIONMODES,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationIEEE11iEncryptionModes(char **ppcValue)
{
	char  acIEEE11iEncryptionModes[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_IEEE11IENCRYPTIONMODES,acIEEE11iEncryptionModes))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acIEEE11iEncryptionModes);
	
	}	   
	return TRX_OK;
}



TRX_STATUS setWLANConfigurationIEEE11iAuthenticationMode(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_IEEE11IAUTHMODE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationIEEE11iAuthenticationMode(char **ppcValue)
{
	char acIEEE11iAuthenticationMode[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_IEEE11IAUTHMODE,acIEEE11iAuthenticationMode))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acIEEE11iAuthenticationMode);
	
	}	   
	return TRX_OK;
}


TRX_STATUS getWLANConfigurationTotalAssociations(char **ppcValue)
{
	char acTotalAssociations[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_TOTALASSOCIATIONS,acTotalAssociations))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acTotalAssociations);
	
	}	   
	return TRX_OK;
}


TRX_STATUS getWLANConfigurationTotalBytesSent(char **ppcValue)
{
	char acTotalBytesSent[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_TOTALBYTESSENT,acTotalBytesSent))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acTotalBytesSent);
	
	}	   
	return TRX_OK;
}


TRX_STATUS getWLANConfigurationTotalBytesReceived(char **ppcValue)
{
	char acTotalBytesReceived[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_TOTALBYTESRECEIVED,acTotalBytesReceived))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acTotalBytesReceived);
	
	}	   
	return TRX_OK;
}



TRX_STATUS getWLANConfigurationTotalPacketsSent(char **ppcValue)
{
	char acTotalPacketsSent[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_TOTALPACKETSSENT,acTotalPacketsSent))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acTotalPacketsSent);
	
	}	   
	return TRX_OK;
}


TRX_STATUS getWLANConfigurationTotalPacketsReceived(char **ppcValue)
{
	char acTotalPacketsReceived[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_TOTALPACKETSRECEIVED,acTotalPacketsReceived))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acTotalPacketsReceived);
	
	}	   
	return TRX_OK;
}



TRX_STATUS setWLANConfigurationWMMEnable(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_WMMENABLE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
    
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationWMMEnable(char **ppcValue)
{
	char acWMMEnable[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_WMMENABLE,acWMMEnable))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWMMEnable);
	
	}	   
	return TRX_OK;
}



TRX_STATUS setWLANConfigurationX_ATP_MixedEncryptionModes(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_MIXEDENCRYPTIONMODES,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationX_ATP_MixedEncryptionModes(char **ppcValue)
{
	char acX_ATP_MixedEncryptionModes[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_MIXEDENCRYPTIONMODES,acX_ATP_MixedEncryptionModes))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_MixedEncryptionModes);
	
	}	   
	return TRX_OK;
}


TRX_STATUS setWLANConfigurationX_ATP_MixedAuthenticationMode(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_MIXEXAUTHMODE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationX_ATP_MixedAuthenticationMode(char **ppcValue)
{
	char acX_ATP_MixedAuthenticationMode[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_MIXEXAUTHMODE,acX_ATP_MixedAuthenticationMode))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_MixedAuthenticationMode);
	
	}	   
	return TRX_OK;
}


TRX_STATUS getWLANConfigurationX_ATP_ChannelUsed(char **ppcValue)
{
	char acX_ATP_ChannelUsed[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_CHANNELUSED,acX_ATP_ChannelUsed))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_ChannelUsed);
	
	}	   
	return TRX_OK;
}





TRX_STATUS getWLANConfigurationX_ATP_WLANVersion(char **ppcValue)
{
	char acX_ATP_WLANVersion[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_WLANVERSION,acX_ATP_WLANVersion))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_WLANVersion);
	
	}	   
	return TRX_OK;
}



TRX_STATUS setWLANConfigurationX_ATP_PowerValue(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_POWERVALUE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationX_ATP_PowerValue(char **ppcValue)
{
	char acX_ATP_PowerValue[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_POWERVALUE,acX_ATP_PowerValue))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_PowerValue);
	
	}	   
	return TRX_OK;
}


TRX_STATUS getWLANConfigurationX_ATP_TotalBytesSentError(char **ppcValue)
{
	char acX_ATP_TotalBytesSentError[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_TOTALBYTESSENTERROR,acX_ATP_TotalBytesSentError))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_TotalBytesSentError);
	
	}	   
	return TRX_OK;
}


TRX_STATUS getWLANConfigurationX_ATP_TotalBytesReceivedError(char **ppcValue)
{
	char acX_ATP_TotalBytesReceivedError[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_TOTALBYTESRECEIVEDERROR,acX_ATP_TotalBytesReceivedError))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_TotalBytesReceivedError);
	
	}	   
	return TRX_OK;
}


TRX_STATUS getWLANConfigurationX_ATP_TotalPacketsSentError(char **ppcValue)
{
	char acX_ATP_TotalPacketsSentError[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_TOTALPACKETSSENTERROR,acX_ATP_TotalPacketsSentError))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_TotalPacketsSentError);
	
	}	   
	return TRX_OK;
}


TRX_STATUS getWLANConfigurationX_ATP_TotalPacketsReceivedError(char **ppcValue)
{
	char acX_ATP_TotalPacketsReceivedError[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_TOTALPACKETSRECEIVEDERROR,acX_ATP_TotalPacketsReceivedError))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_TotalPacketsReceivedError);
	
	}	   
	return TRX_OK;
}





TRX_STATUS setWLANConfigurationX_ATP_AssociateDeviceNum(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_ASSOCIATEDEVICENUM,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationX_ATP_AssociateDeviceNum(char **ppcValue)
{
	char acX_ATP_AssociateDeviceNum[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_ASSOCIATEDEVICENUM,acX_ATP_AssociateDeviceNum))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_AssociateDeviceNum);
	
	}	   
	return TRX_OK;
}



TRX_STATUS setWLANConfigurationX_ATP_WlanMacFilterpolicy(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_WLANMACFILTERPOLICY,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationX_ATP_WlanMacFilterpolicy(char **ppcValue)
{
	char acX_ATP_WlanMacFilterpolicy[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_WLANMACFILTERPOLICY,acX_ATP_WlanMacFilterpolicy))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_WlanMacFilterpolicy);
	
	}	   
	return TRX_OK;
}



TRX_STATUS getWLANConfigurationX_ATP_WlanMacFilternum(char **ppcValue)
{
	char acX_ATP_WlanMacFilternum[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_WLANMACFILTERNUM,acX_ATP_WlanMacFilternum))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_WlanMacFilternum);
	
	}	   
	return TRX_OK;
}



TRX_STATUS setWLANConfigurationX_ATP_WlanStaWakeEnable(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_WLANSTAWAKEENABLE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationX_ATP_WlanStaWakeEnable(char **ppcValue)
{
	char acX_ATP_WlanStaWakeEnable[0]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_WLANSTAWAKEENABLE,acX_ATP_WlanStaWakeEnable))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_WlanStaWakeEnable);
	
	}	   
	return TRX_OK;
}




TRX_STATUS setWLANConfigurationX_ATP_WlanIsolateControl(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_WLANISOLATECONTROL,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationX_ATP_WlanIsolateControl(char **ppcValue)
{
	
	char acX_ATP_WlanIsolateControl[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_WLANISOLATECONTROL,acX_ATP_WlanIsolateControl))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_WlanIsolateControl);
	
	}	   
	return TRX_OK;
}


TRX_STATUS setWLANConfigurationX_ATP_Wlan11NBWControl(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_WLAN11NBWCONTROL,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationX_ATP_Wlan11NBWControl(char **ppcValue)
{
	char acX_ATP_Wlan11NBWControl[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_WLAN11NBWCONTROL,acX_ATP_Wlan11NBWControl))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_Wlan11NBWControl);
	
	}	   
	return TRX_OK;
}



TRX_STATUS setWLANConfigurationX_ATP_Wlan11NGIControl(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_WLAN11NGTCONTROL,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationX_ATP_Wlan11NGIControl(char **ppcValue)
{
	char acX_ATP_Wlan11NGIControl[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_WLAN11NGTCONTROL,acX_ATP_Wlan11NGIControl))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_Wlan11NGIControl);
	
	}	   
	return TRX_OK;
}



TRX_STATUS setWLANConfigurationX_ATP_Wlan11NHtMcs(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_WLAN11NHTMCS,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationX_ATP_Wlan11NHtMcs(char **ppcValue)
{
	char acX_ATP_Wlan11NHtMcs[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_WLAN11NHTMCS,acX_ATP_Wlan11NHtMcs))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_Wlan11NHtMcs);
	
	}	   
	return TRX_OK;
}



TRX_STATUS getWLANConfigurationX_ATP_Wlan11NTxRxStream(char **ppcValue)
{
	char acX_ATP_Wlan11NTxRxStream[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_WLAN11NTXRXSTREAM,acX_ATP_Wlan11NTxRxStream))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_Wlan11NTxRxStream);
	
	}	   
	return TRX_OK;
}



TRX_STATUS getWPSEnable(char **ppcValue)
{
	char acWPSEnable[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATIONWPS_ENABLE,acWPSEnable))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWPSEnable);
	
	}	   
	return TRX_OK;
}


TRX_STATUS getWPSX_ATP_WPSMode(char **ppcValue)
{
	char acX_ATP_WPSMode[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATIONWPS_X_ATP_WPSMODE,acX_ATP_WPSMode))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_WPSMode);
	
	}	   
	return TRX_OK;
}

TRX_STATUS getWPSDevicePassword(char **ppcValue)
{
	char acWPSDevicePassword[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATIONWPS_DEVICEPASSWORD,acWPSDevicePassword))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWPSDevicePassword);
	
	}	   
	return TRX_OK;
}

TRX_STATUS setX_ATP_WlanSrcMacAddress(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_WLANSRCMACADDR,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
    
	return TRX_OK;
}
TRX_STATUS getX_ATP_WlanSrcMacAddress(char **ppcValue)
{
	char acX_ATP_WlanSrcMacAddress[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_WLANSRCMACADDR,acX_ATP_WlanSrcMacAddress))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_WlanSrcMacAddress);
	
	}	   
	return TRX_OK;
}




TRX_STATUS setX_ATP_SSID1(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_SSID1,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getX_ATP_SSID1(char **ppcValue)
{
	char acX_ATP_SSID1[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_SSID1,acX_ATP_SSID1))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_SSID1);
	
	}	   
	return TRX_OK;
}


TRX_STATUS setX_ATP_SSID2(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_SSID2,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getX_ATP_SSID2(char **ppcValue)
{
	char acX_ATP_SSID2[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_SSID2,acX_ATP_SSID2))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_SSID2);
	
	}	   
	return TRX_OK;
}


TRX_STATUS setX_ATP_SSID3(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_SSID3,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getX_ATP_SSID3(char **ppcValue)
{
	char acX_ATP_SSID3[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_SSID3,acX_ATP_SSID3))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_SSID3);
	
	}	   
	return TRX_OK;
}



TRX_STATUS setX_ATP_SSID4(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_SSID4,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getX_ATP_SSID4(char **ppcValue)
{
	char acX_ATP_SSID4[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_X_ATP_SSID4,acX_ATP_SSID4))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_SSID4);
	
	}	   
	return TRX_OK;
}



TRX_STATUS getAssociatedDeviceMACAddress(char **ppcValue)
{
	char acMACAddress[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_ASSOCIATEDEVICEMACADDR,acMACAddress))
	{
		*ppcValue = strdup("00-00-00-00-00-00");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acMACAddress);
	
	}	   
	return TRX_OK;
}

TRX_STATUS setWLANConfigurationWEPKey(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_WEPKEY,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationWEPKey(char **ppcValue)
{
	char acWEPKey[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_WEPKEY,acWEPKey))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acWEPKey);
	
	}	   
	return TRX_OK;
}



TRX_STATUS setWLANConfigurationPreSharedKey(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_WLANCONFIGURATION_PSK,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    
	return TRX_OK;
}
TRX_STATUS getWLANConfigurationPreSharedKey(char **ppcValue)
{
	char acPreSharedKey[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_WLANCONFIGURATION_PSK,acPreSharedKey))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acPreSharedKey);
	
	}	   
	return TRX_OK;
}




/*InternetGatewayDevice.LANDevice.{i}.LANEthernetInterfaceConfig.{i}.Stats.*/
TRX_STATUS getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_PacketsReceivedError(char **ppcValue)
{
	char  acPacketsReceivedError[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_X_ATP_PACKETSRECEIVEDERRTOR,acPacketsReceivedError))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acPacketsReceivedError);
		
	}	   
	return TRX_OK;
}

TRX_STATUS getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_PacketsError(char **ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_PacketsDrops(char **ppcValue)
{
	
	return TRX_OK;
}
TRX_STATUS getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_PacketsReceivedDrops(char **ppcValue)
{
	char  acX_ATP_PacketsReceivedDrops[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_X_ATP_PACKETSRECEIVEDDROPS,acX_ATP_PacketsReceivedDrops))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_PacketsReceivedDrops);
		
	}	   
	return TRX_OK;
}

TRX_STATUS getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_SentRate(char **ppcValue)
{
	char  acX_ATP_SentRate[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_X_ATP_SENTRATE,acX_ATP_SentRate))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_SentRate);
		
	}	   
	return TRX_OK;
}

TRX_STATUS getLANDeviceLANEthernetInterfaceConfigStats_X_ATP_ReceivedRate(char **ppcValue)
{
	char  acX_ATP_ReceivedRate[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_X_ATP_RECEIVEDRATE,acX_ATP_ReceivedRate))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_ReceivedRate);
		
	}	   
	return TRX_OK;
}

/* 是否启用该接口，暂时不支持设置 */
TRX_STATUS setLANDeviceLANHostConfigManagementIPInterface_Enable(const char *pcValue)
{	
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_IPINTERFACE_ENABLE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
	return TRX_OK;
}
TRX_STATUS getLANDeviceLANHostConfigManagementIPInterface_Enable(char **ppcValue)
{	
	char acIPInterface_Enable[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_IPINTERFACE_ENABLE,acIPInterface_Enable))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acIPInterface_Enable);
		
	}	

	return TRX_OK;
}

/* ip address */
TRX_STATUS setLANDeviceLANHostConfigManagementIPInterface_IPInterfaceIPAddress(const char *pcValue)
{
   /*  OM_TR69C_LAN_PARAM_T  stLanParam = {0};

    if (NULL ==  value )
    {
        slog(TR69C_LOG_ERR, "[%s,%d]pointer is NULL",__func__,__LINE__);
        return TRX_ERR;
    }

    stLanParam.eFlag = TR69C_LAN_IP_ADDR_SET;
	
	util_str_to_ip((char *)value,stLanParam.stLanCfg.aucLanIp);	
   // sscanf(value, IP_FMT, IP_ARG(stLanParam.stLanCfg.aucLanIp));

    if(OM_E_NONE != om_net_tr069_lan_handle(&stLanParam))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    } */
    if(UCI_OK !=uci_config_set(UCI_LANDEVICE_IPINTERFACEIPADDR,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    return TRX_OK;
}
TRX_STATUS getLANDeviceLANHostConfigManagementIPInterface_IPInterfaceIPAddress(char **ppcValue)
{
    /* OM_TR69C_LAN_PARAM_T  stLanParam = {0};
    CHAR szTemp[CHAR_LEN_32] = {0};

    stLanParam.eFlag = TR69C_LAN_IP_ADDR_GET;  

    if(OM_E_NONE != om_net_tr069_lan_handle(&stLanParam))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    sprintf(szTemp,IP_FMT,IP_ARG(stLanParam.stLanCfg.aucLanIp));
    *value = strdup(szTemp);
    sprintf(szTemp,"1gjhgjg");
    slog(TR69C_LOG_DEBUG, "[%s,%d]sztmp = %s",__func__,__LINE__,szTemp); */
    char  acIPInterfaceIPAddress[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_IPINTERFACEIPADDR,acIPInterfaceIPAddress))
	{
		*ppcValue = strdup("0.0.0.0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acIPInterfaceIPAddress);
		
	}	   
    return TRX_OK;
}
TRX_STATUS setLANDeviceLANHostConfigManagementIPInterface_IPInterfaceSubnetMask(const char *pcValue)
{
   /*  OM_TR69C_LAN_PARAM_T  stLanParam = {0};

    if (NULL ==  value )
    {
        slog(TR69C_LOG_ERR, "[%s,%d]pointer is NULL",__func__,__LINE__);
        return TRX_ERR;
    }

    stLanParam.eFlag = TR69C_LAN_IP_NETMASK_SET;

	util_str_to_ip((char *)value,stLanParam.stLanCfg.aucLanNetMask);
 //   sscanf(value, IP_FMT, IP_ARG(stLanParam.stLanCfg.aucLanNetMask));  

    if(OM_E_NONE != om_net_tr069_lan_handle(&stLanParam))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    } */
    if(UCI_OK !=uci_config_set(UCI_LANDEVICE_IPINTERFACESUBNETMASK,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    return TRX_OK;
}
TRX_STATUS getLANDeviceLANHostConfigManagementIPInterface_IPInterfaceSubnetMask(char **ppcValue)
{
   /*  OM_TR69C_LAN_PARAM_T  stLanParam = {0};
    CHAR szTemp[CHAR_LEN_32] = {0};

    stLanParam.eFlag = TR69C_LAN_IP_NETMASK_GET;  

    if(OM_E_NONE != om_net_tr069_lan_handle(&stLanParam))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    sprintf(szTemp,IP_FMT,IP_ARG(stLanParam.stLanCfg.aucLanNetMask));

    *value = strdup(szTemp); */
    char  acIPInterfaceSubnetMask[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_IPINTERFACESUBNETMASK,acIPInterfaceSubnetMask))
	{
		*ppcValue = strdup("255.255.255.255");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acIPInterfaceSubnetMask);
		
	}	   
    
    return TRX_OK;
}

/* 该接口分配地址的方式：
Represents the addressing method used to assign
the LAN-side IP address of the CPE on this
interface.
   “DHCP”
   “Static”
   “AutoIP” */
TRX_STATUS setLANDeviceLANHostConfigManagementIPInterface_IPInterfaceAddressingType(const char *value)
{
	return TRX_OK;
}
TRX_STATUS getLANDeviceLANHostConfigManagementIPInterface_IPInterfaceAddressingType(char **value)
{
	*value = strdup("DHCP");
	return TRX_OK;
}

/* InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1. */
/* Enables or disables this interface. */
TRX_STATUS setLANDeviceLANEthernetInterfaceConfig_Enable(const char *value)
{
    return setLANDeviceLANHostConfigManagementIPInterface_Enable(value);
}
TRX_STATUS getLANDeviceLANEthernetInterfaceConfig_Enable(char **ppcValue)
{
    return getLANDeviceLANHostConfigManagementIPInterface_Enable(ppcValue);
}

/* 当前接口状态：
“Up”
“NoLink”
“Error” (OPTIONAL)
“Disabled" */
TRX_STATUS getLANDeviceLANEthernetInterfaceConfig_Status(char **ppcValue)
{
	char  acInterfaceConfig_Status[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_LANETHERNETINTERFACECOFfG_STATUS,acInterfaceConfig_Status))
	{
		*ppcValue = strdup("Up");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acInterfaceConfig_Status);
	
	}	   
    //*value = strdup("Up");
	return TRX_OK;
}
TRX_STATUS getLANDeviceLANEthernetInterfaceConfig_MACAddress(char **value)
{
    return getLANDeviceLANHostConfigManagement_MACAddress(value);
}

/* 接口收发包配置 */
TRX_STATUS getLANDeviceLANEthernetInterfaceConfigStats_BytesSent(char **ppcValue)
{
   /*  OM_TR69C_LAN_STATS_T stTsStats;
    CHAR szTemp[CHAR_LEN_32] = {0};
    slog(TR69C_LOG_ERR," Sent start 0"); 

    if(OM_E_NONE != om_net_tr069_lan_stats(&stTsStats))
    {
        slog(TR69C_LOG_ERR," ERROR");        
        return TRX_ERR;
    }
slog(TR69C_LOG_ERR," Sent start 1"); 
    sprintf(szTemp,"%d",stTsStats.stTsList.stTsIpv4.uiUpBytes);
slog(TR69C_LOG_ERR," Sent start 2"); 
    *value = strdup(szTemp); */
    char  acBytesSent[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_BYTESSENT,acBytesSent))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acBytesSent);
	
	}	   
	return TRX_OK;
}
TRX_STATUS getLANDeviceLANEthernetInterfaceConfigStats_BytesReceived(char **ppcValue)
{
    /* OM_TR69C_LAN_STATS_T stTsStats;
    CHAR szTemp[CHAR_LEN_32] = {0};

    if(OM_E_NONE != om_net_tr069_lan_stats(&stTsStats))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    sprintf(szTemp,"%d",stTsStats.stTsList.stTsIpv4.uiDownBytes);

    *value = strdup(szTemp); */
	char  acBytesReceived[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_BYTESRECEIVED,acBytesReceived))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acBytesReceived);
	
	}	   
	return TRX_OK;	
}

TRX_STATUS getLANDeviceLANEthernetInterfaceConfigStats_PacketsSent(char **ppcValue)
{
    /* OM_TR69C_LAN_STATS_T stTsStats;
    CHAR szTemp[CHAR_LEN_32] = {0};

    if(OM_E_NONE != om_net_tr069_lan_stats(&stTsStats))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    sprintf(szTemp,"%d",stTsStats.stTsList.stTsIpv4.uiUpGiga);

    *value = strdup(szTemp); */
    char  acPacketsSent[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_PACKETSSENT,acPacketsSent))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acPacketsSent);
	
	}	   
	return TRX_OK;
}
TRX_STATUS getLANDeviceLANEthernetInterfaceConfigStats_PacketsReceived(char **ppcValue)
{
   /*  OM_TR69C_LAN_STATS_T stTsStats;
    CHAR szTemp[CHAR_LEN_32] = {0};

    if(OM_E_NONE != om_net_tr069_lan_stats(&stTsStats))
    {
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR",__func__,__LINE__);        
        return TRX_ERR;
    }

    sprintf(szTemp,"%d",stTsStats.stTsList.stTsIpv4.uiDownGiga);

    *value = strdup(szTemp); */
    char  acPacketsReceived[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_PACKETSRECEIVED,acPacketsReceived))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acPacketsReceived);
	
	}	   
	return TRX_OK;
}
/* init host instances;include:get host total_number ,then init hosts instance */
int initLANDeviceHostsHostInstance()
{
    int iNum;
    
	return iNum;
}
/* LAN侧的host主机信息，包括通过DHCP方式分配地址的，或者通过静态指定的主机 */
TRX_STATUS getLANDeviceHosts_HostNumberOfEntries(char **value)
{
    int iNum;
    iNum = initLANDeviceHostsHostInstance();
    *value = iNum?strdup(itoa(iNum)):strdup("0");
    return TRX_OK;
}

/* 获取IP地址 */
TRX_STATUS getLANDeviceHostsHost_IPAddress(char **value)
{
	InstanceDesc *idp = getCurrentInstanceDesc();
	UINT32 index;
    
	index = idp->instanceID - 1;
    // *value = g_stTr69cHostTAB.stHoetTable[index].acIPAddress?
      //  strdup(g_stTr69cHostTAB.stHoetTable[index].acIPAddress):strdup("");
    return TRX_OK;
}

TRX_STATUS getLANDeviceHostsHost_MACAddress(char **value)
{
	InstanceDesc *idp = getCurrentInstanceDesc();
	UINT32 index;
    
	index = idp->instanceID - 1;
    //*value = g_stTr69cHostTAB.stHoetTable[index].acMACAddress?
        //strdup(g_stTr69cHostTAB.stHoetTable[index].acMACAddress):strdup("");
    return TRX_OK;
}

/* host主机名。可以为未知 */
TRX_STATUS getLANDeviceHostsHost_HostName(char **value)
{
	//InstanceDesc *idp = getCurrentInstanceDesc();

    return TRX_ERR;
}

/* 当前host连入cpe所使用的方式：
   “Ethernet”
   “USB”
   “802.11”
   “HomePNA”
   “HomePlug”
   “MoCA”
   “Other” */
TRX_STATUS getLANDeviceHostsHost_InterfaceType(char **value)
{
	InstanceDesc *idp = getCurrentInstanceDesc();
	UINT32 index;
    
	index = idp->instanceID - 1;
    //*value = g_stTr69cHostTAB.stHoetTable[index].uiInterfaceType?
        //strdup("802.11"):strdup("Ethernet");
    return TRX_OK;
}

void initLANConfigSecurityInstances()
{
    TRxObjNode *n;
    int id;
    InstanceDesc  *lanCfgSecurityIdp;
    
    n = LANConfigSecurityDesc;
    id = 1;
    lanCfgSecurityIdp = getNewInstanceDesc(n, NULL, id);
}

void initLANDeviceInstances()
{
    TRxObjNode *n;
    int id;
    InstanceDesc  *lanDeviceIdp;
    InstanceDesc  *lanHostCfgMgmtIdp;
    InstanceDesc  *lanHostCfgMgmtIpInterfaceIdp;
    InstanceDesc  *lanEthInterfaceCfgIdp;
    InstanceDesc  *lanEthInterfaceCfgStatsIdp;
    InstanceDesc  *lanDeviceHostIdp;
	InstanceDesc  *lanDevicewlanconfIdp;

	InstanceDesc  *X_ATP_WlanMacFilterDescObjIdp;
	InstanceDesc  *AssociatedDeviceDescObjIdp;
	InstanceDesc  *WEPKeyDescObjIdp;
	InstanceDesc  *PreSharedKeyDescObjIdp;

	InstanceDesc  *DHCPRelaySeverConfigIdp;
	InstanceDesc  *DHCPStaticAddressIdp;
	
    n = LANDeviceInstanceDesc;
    id = 1;
    lanDeviceIdp = getNewInstanceDesc(n, NULL, id);

    /* lan ethernet interface cofig */
    n = LANDeviceLANEthernetInterfaceConfigInstanceDesc;
    lanEthInterfaceCfgIdp = getNewInstanceDesc(n, lanDeviceIdp, id);

    n = LANDeviceLANEthernetInterfaceConfigStatsDesc;
    lanEthInterfaceCfgStatsIdp = getNewInstanceDesc(n, lanEthInterfaceCfgIdp, id);

	/* lan host config management  */
    n = LANDeviceLANHostConfigManagementDesc;
    lanHostCfgMgmtIdp = getNewInstanceDesc(n, lanDeviceIdp, id);
	
   	n = LANDeviceLANHostConfigManagementIPInterfaceInstanceDesc;
    lanHostCfgMgmtIpInterfaceIdp = getNewInstanceDesc(n, lanHostCfgMgmtIdp, id);
	
	
   	n = DHCPRelaySeverConfigDesc;
    DHCPRelaySeverConfigIdp = getNewInstanceDesc(n, lanHostCfgMgmtIdp, id);
	n = DHCPStaticAddressDesc;
    DHCPStaticAddressIdp = getNewInstanceDesc(n, lanHostCfgMgmtIdp, id);
    
#if 0
    /* lan host config management  */

 

	n = LANDeviceWLANConfigurationInstanceDesc;
	lanDevicewlanconfIdp = getNewInstanceDesc(n, lanDeviceIdp, id);

	n = X_ATP_WlanMacFilterDescObj;
	X_ATP_WlanMacFilterDescObjIdp = getNewInstanceDesc(n, lanDevicewlanconfIdp, id);

	n = AssociatedDeviceDescObj;
	X_ATP_WlanMacFilterDescObjIdp = getNewInstanceDesc(n, lanDevicewlanconfIdp, id);

	n = WEPKeyDescObj;
	WEPKeyDescObjIdp = getNewInstanceDesc(n, lanDevicewlanconfIdp, id);

	n = PreSharedKeyDescObj;
	PreSharedKeyDescObjIdp = getNewInstanceDesc(n, lanDevicewlanconfIdp, id);

    /* hosts */
    n = LANDeviceHostsDesc;
    lanDeviceHostIdp = getNewInstanceDesc(n, lanDeviceIdp, id);
  #endif  
    initLANDeviceHostsHostInstance();
}

