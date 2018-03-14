/******************************************************************************
* COPYRIGHT GBCOM
*******************************************************************************
* 功    能:
* 修改历史:
* 2012-9-10     冯静              新建
******************************************************************************/
#include <syslog.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>

#include <netinet/in.h>
#include <net/if.h>

#include <unistd.h>
#include <fcntl.h>	
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if_link.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <sys/sysinfo.h>

#include "mini_snmpd.h"
#include "my_snmp_def.h"
#include "my_snmp_data.h"
#include "my_snmp_handle.h"
#include "my_snmp_util.h"
#include "my_snmp_config.h"
#include "my_snmp_info.h"
#include "uci_config.h"

//#include "om_types.h"
//#include "om_app.h"
//#include "om_lte.h"
//#include "om_net.h"
//#include "nvram.h"
//#include "nvram_define.h"
#include "util.h"
//#include "pal/app/pal_app.h"


/*********************************全局变量*********************************/
extern SNMP_AGENT_DATA_T *g_pstSnmpAgentData;
extern int g_stati_per;
extern DEV_STATE_E g_enCpeState;

void ipstring_to_num(CHAR* pcIp, UINT8 *pcIpadder)
{
    CHAR* p;
    int i = 0;

    p = strtok(pcIp, ".");
    while (p)
    {
        pcIpadder[i] = atoi(p);
        p = strtok(NULL, ".");

#if 0
        /* For debug */
        printf("pcIpadder[i]=%d\n", pcIpadder[i]);
#endif
        i++;
    }

    return;
}

BOOL snmp_sys_lte_info_init(void)
{
/*    OM_ERROR_CODE_E enRet;
    OM_LTE_MODULE_INFO_T stLteModule;
    OM_LTE_SIM_INFO_T stLteSim;
    static UINT32 bGetSeccess = FALSE;
    UINT32 uiSeccessCount = 0;

    if(TRUE == bGetSeccess)
    {
        return TRUE;
    }
    snmp_log(LOG_NOTICE, "snmp_sys_lte_info_init\n");
    memset(&stLteModule, 0, sizeof(stLteModule));
    memset(&stLteSim, 0, sizeof(stLteSim));

    enRet = om_lte_module_info_get(&stLteModule);
    if (OM_E_NONE != enRet)
    {
        snmp_log(LOG_ERR, "om_lte_module_info_get fail.\n");
        strcpy(g_pstSnmpAgentData->stLteHwInfo.aucModuleType, "N/A");
        strcpy(g_pstSnmpAgentData->stLteHwInfo.aucModuleVer, "N/A");
        strcpy(g_pstSnmpAgentData->stLteHwInfo.aucModuleIMEI, "N/A");
        //return TRUE;
    }
    else
    {
        memcpy(g_pstSnmpAgentData->stLteHwInfo.aucModuleType, stLteModule.aucModuleType, CHAR_LEN_32);
        memcpy(g_pstSnmpAgentData->stLteHwInfo.aucModuleVer, stLteModule.aucModuleVer, CHAR_LEN_128);
        memcpy(g_pstSnmpAgentData->stLteHwInfo.aucModuleIMEI, stLteModule.aucModuleIMEI, CHAR_LEN_32);
        ++uiSeccessCount;
    }
    
    enRet = om_lte_sim_info_get(&stLteSim);
    if (OM_E_NONE != enRet)
    {
        snmp_log(LOG_ERR, "om_lte_sim_info_get fail.\n");
        strcpy(g_pstSnmpAgentData->stLteHwInfo.aucSimICCID, "N/A");
        strcpy(g_pstSnmpAgentData->stLteHwInfo.aucSimIMSI, "N/A");
    }
    else
    {
        memcpy(g_pstSnmpAgentData->stLteHwInfo.aucSimICCID, stLteSim.aucSimICCID, CHAR_LEN_32);
        memcpy(g_pstSnmpAgentData->stLteHwInfo.aucSimIMSI, stLteSim.aucSimIMSI, CHAR_LEN_32);
        ++uiSeccessCount;
    }
    if(2 == uiSeccessCount)
    {
        bGetSeccess = TRUE;
    }
*/
    return TRUE;
}

BOOL snmp_sys_dev_info(void)
{
/*    OM_ERROR_CODE_E enRet;
    OM_APP_SYS_BASIC_INFO_T stBasicInfo;

    memset(&g_pstSnmpAgentData->stSysDevInfo, 0, sizeof(g_pstSnmpAgentData->stSysDevInfo));
    
    enRet = om_app_sys_basic_info_get(&stBasicInfo);
    if (OM_E_NONE != enRet)
    {
        snmp_log(LOG_ERR, "get basic info fail.\n");
        strcpy(g_pstSnmpAgentData->stSysDevInfo.acSysSn, "N/A");
        strcpy(g_pstSnmpAgentData->stSysDevInfo.acSysDevType, "N/A");
        strcpy(g_pstSnmpAgentData->stSysDevInfo.acSysTime, "N/A");
    }
    else
    {
        strncpy(g_pstSnmpAgentData->stSysDevInfo.acSysSn, stBasicInfo.acDeviceSn, strlen(stBasicInfo.acDeviceSn));
        strncpy(g_pstSnmpAgentData->stSysDevInfo.acSysDevType, stBasicInfo.acDeviceType, strlen(stBasicInfo.acDeviceType));
        strncpy(g_pstSnmpAgentData->stSysDevInfo.acSysTime, stBasicInfo.acSysTime, strlen(stBasicInfo.acSysTime));
    }
    
    //strncpy(g_pstSnmpAgentData->stSysDevInfo.acSysUptime, stBasicInfo.acUpTime, strlen(stBasicInfo.acUpTime));
    if(AP_E_NONE != app_get_up_time(&(g_pstSnmpAgentData->stSysDevInfo.uiSysUptime)))
    {
        snmp_log(LOG_ERR, "get up time failed");
    }
    
    */
    
    struct sysinfo info     = {0};
    int iRet                = -1;

    iRet = sysinfo(&info);
         
    g_pstSnmpAgentData->stSysDevInfo.uiSysUptime=info.uptime;
   
    return TRUE;

}

BOOL snmp_sys_sw_info()
{
/*    OM_ERROR_CODE_E enRet;
    OM_APP_SYS_SW_INFO_T stSwInfo; //dp

    memset(&g_pstSnmpAgentData->stSwInfo, 0, sizeof(g_pstSnmpAgentData->stSwInfo));
    memset(&stSwInfo, 0, sizeof(stSwInfo));

    enRet = om_app_get_sw_info(&stSwInfo);
    if (OM_E_NONE != enRet)
    {
        snmp_log(LOG_ERR, "get sw info fail.\n");
        strcpy(g_pstSnmpAgentData->stSwInfo.acSoftwareVersion, "N/A");
        strcpy(g_pstSnmpAgentData->stSwInfo.acTargetVersion, "N/A");
    }
    else
    {
        strncpy(g_pstSnmpAgentData->stSwInfo.acSoftwareVersion, stSwInfo.acSoftwareVersion, strlen(stSwInfo.acSoftwareVersion)); 
        strncpy(g_pstSnmpAgentData->stSwInfo.acTargetVersion, stSwInfo.acTargetVersion, strlen(stSwInfo.acTargetVersion));
    }  
    */
    
    FILE *fp_popen;
    char acBuff[32] ={0};
    fp_popen=popen("vlist | grep running | cut -d':' -f 2 | cut -d' ' -f2","r");
    fgets(acBuff,32,fp_popen);
	pclose(fp_popen);
    
    acBuff[strlen(acBuff)-1] = '\0';
    
    strncpy(g_pstSnmpAgentData->stSwInfo.acSoftwareVersion, acBuff, strlen(acBuff)); 
    
    return TRUE;
}

BOOL snmp_sys_hardware_info(void)
{

    /*CPU类型*/
    strcpy(g_pstSnmpAgentData->stHwInfo.aucCpuType, "ar9341");
    /*MEM类型*/
    strcpy(g_pstSnmpAgentData->stHwInfo.aucMemoryType, "DDR1");
    /*CPU大小*/
    g_pstSnmpAgentData->stHwInfo.ulFlashSize = 16;
    /*MEM大小*/
    g_pstSnmpAgentData->stHwInfo.ulMemorySize = 64;
    /*CPE硬件版本号*/
    strcpy(g_pstSnmpAgentData->stHwInfo.aucCpeHwver, "v1.0.0");

    return TRUE;
}

AP_ERROR_CODE_E net_port_get_mac (CHAR *pcPortName, UINT32 *pulMac)
{   
	INT32 iRet;
	INT32 iSocket;
    struct ifreq stIfreq = {0};
    struct sockaddr_in *pstIp, *pstMask;

	if (NULL == pcPortName || NULL == pulMac)
	{
	    return AP_E_PARAM; 
	}
	
	iSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (iSocket < 0)
	{
		return AP_E_RESOURCE;
	}
	
    strncpy(stIfreq.ifr_name, pcPortName, sizeof(stIfreq.ifr_name));
	
    if(NULL != pulMac)
    {
        iRet = ioctl(iSocket, SIOCGIFHWADDR, &stIfreq);
        if (iRet < 0)
        {
        	close(iSocket);
        	return AP_E_RESOURCE;
        }
        //pstMask = ((struct sockaddr_in *)&(stIfreq.ifr_netmask));
        //*pulMask= pstMask->sin_addr.s_addr;
         memcpy(pulMac, (unsigned char *)stIfreq.ifr_hwaddr.sa_data, ETH_ALEN);
    }
   
 
	close(iSocket);
	
    return AP_E_NONE;
}
AP_ERROR_CODE_E net_port_get_ip (CHAR *pcPortName, UINT32 *pulIp)
{   
	INT32 iRet;
	INT32 iSocket;
    struct ifreq stIfreq = {0};
    struct sockaddr_in *pstIp, *pstMask;

	if (NULL == pcPortName || NULL == pulIp)
	{
	    return AP_E_PARAM; 
	}
	
	iSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (iSocket < 0)
	{
		return AP_E_RESOURCE;
	}
	
    strncpy(stIfreq.ifr_name, pcPortName, sizeof(stIfreq.ifr_name));
	
	iRet = ioctl(iSocket, SIOCGIFADDR, &stIfreq);
	if (iRet < 0)
	{
		close(iSocket);
		return AP_E_RESOURCE;
	}
	pstIp = ((struct sockaddr_in *)&(stIfreq.ifr_addr));
	*pulIp = pstIp->sin_addr.s_addr;
  	
//	get ip mask bcast   
//	if(!ioctl(iSocket,SIOCGIFADDR,&stIfreq)){  
//		printf("     inet %s",inet_ntoa(((struct sockaddr_in *)&stIfreq.ifr_addr)->sin_addr));  
//	}  
//	if(!ioctl(iSocket,SIOCGIFNETMASK,&stIfreq)){  
//		printf(" netmask %s",inet_ntoa(((struct sockaddr_in *)&stIfreq.ifr_addr)->sin_addr));  
//	}  
//	if(!ioctl(iSocket,SIOCGIFBRDADDR,&stIfreq)){  
//		printf(" broadcast %s\n",inet_ntoa(((struct sockaddr_in *)&stIfreq.ifr_broadaddr)->sin_addr));  
//	}  
//    char mac_addr[30];  
//    sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",  
//        (unsigned char)stIfreq.ifr_hwaddr.sa_data[0],  
//        (unsigned char)stIfreq.ifr_hwaddr.sa_data[1],  
//        (unsigned char)stIfreq.ifr_hwaddr.sa_data[2],  
//        (unsigned char)stIfreq.ifr_hwaddr.sa_data[3],  
//        (unsigned char)stIfreq.ifr_hwaddr.sa_data[4],  
//        (unsigned char)stIfreq.ifr_hwaddr.sa_data[5]  
//    );         

//	printf("[%25s|%30s:%5d] mac=%s \n",__FILE__,__FUNCTION__,__LINE__, mac_addr);	
	
	close(iSocket);
	
    return AP_E_NONE;
}

BOOL snmp_sys_manage_info(void)
{
//    OM_ERROR_CODE_E enRet;
//    OM_APP_SYS_INTFACE_INFO_T stManageInfo;
//
//    memset(&stManageInfo, 0, sizeof(stManageInfo));
//    enRet = om_app_get_wan_info(&stManageInfo);
//    if (OM_E_NONE != enRet)
//    {
//        snmp_log(LOG_ERR, "om_app_get_wan_info fail.\n");
//        return FALSE;
//    }
	CHAR aucWanName[CHAR_LEN_32] = {0};
	UINT8 aucIp[IP_ALEN];
	UINT8 ulIpMac[IP_ALEN];
	
	net_port_get_ip("usb0", (UINT32 *)(aucIp));
	memcpy(g_pstSnmpAgentData->stManageInfo.acManageIp, aucIp, IP_ALEN);
	//printf("[%25s|%30s:%5d] Ip=%u.%u.%u.%u \n",__FILE__,__FUNCTION__,__LINE__, \
    //	aucIp[0],aucIp[1],aucIp[2],aucIp[3]);	
    net_port_get_mac("usb0", (UINT32 *)(ulIpMac));
	memcpy(g_pstSnmpAgentData->stManageInfo.acManageMac, ulIpMac, ETH_ALEN);
    //printf("[%25s|%30s:%5d] Mac=%02x:%02x:%02x:%02x:%02x:%02x \n",__FILE__,__FUNCTION__,__LINE__, \
    //	ulIpMac[0],ulIpMac[1],ulIpMac[2],ulIpMac[3],ulIpMac[4],ulIpMac[5]);	
    
    g_pstSnmpAgentData->stManageInfo.uiManageState = g_enCpeState;
    //inet_ntop(AF_INET6, stManageInfo.aucIp6, g_pstSnmpAgentData->stManageInfo.aucManageIpv6, CHAR_LEN_64);
	
    return TRUE;
}

BOOL snmp_sys_proto_info(void)
{
/*    char cNvramValue[NVRAM_VALUE_MAX_LEN];
    UINT32 uiIpProtoMode = 0;
    OM_ERROR_CODE_E enRet;

    enRet = nvram_get(cNvramValue, NVRAM_IP_PROTO_MODE, sizeof(cNvramValue));
    if (AP_E_NONE != enRet)
    {
        return FALSE;
    }
    uiIpProtoMode = atoi(cNvramValue);

    if (IP_PROTO_IPV4 == uiIpProtoMode)
    {
        g_pstSnmpAgentData->stProtoInfo.uiIpv4Support = 1;
        g_pstSnmpAgentData->stProtoInfo.uiIpv6Support = 0;
    }
    else if (IP_PROTO_IPV6 == uiIpProtoMode)
    {
        g_pstSnmpAgentData->stProtoInfo.uiIpv4Support = 0;
        g_pstSnmpAgentData->stProtoInfo.uiIpv6Support = 1;
    }
    else if (IP_PROTO_IPV4_IPV6 == uiIpProtoMode)
    {
        g_pstSnmpAgentData->stProtoInfo.uiIpv4Support = 1;
        g_pstSnmpAgentData->stProtoInfo.uiIpv6Support = 1;
    }
    else
    {
        g_pstSnmpAgentData->stProtoInfo.uiIpv4Support = 1;
        g_pstSnmpAgentData->stProtoInfo.uiIpv6Support = 0;
    }
*/
    return TRUE;
}

BOOL snmp_lte_hw_info(void)
{
/*    OM_LTE_STATE_T stLteStateInfo = {0};
    CHAR acTemp[NVRAM_VALUE_MAX_LEN] = {0};
    OM_ERROR_CODE_E enRet;

    enRet = om_lte_state_get_asyn(&stLteStateInfo);
    if (OM_E_NONE != enRet)
    {
        snmp_log(LOG_ERR, "om_lte_state_get_asyn fail.\n");
        return FALSE;
    }

    if (AP_E_NONE != nvram_get(acTemp, NVRAM_LTE_PIN_SIMLOCK_SW, NVRAM_VALUE_MAX_LEN))
    {
        snmp_log(LOG_ERR, "nvram get fail %s.\n", NVRAM_LTE_PIN_SIMLOCK_SW);
        return FALSE;
    }
    g_pstSnmpAgentData->stLteHwInfo.uiSimLock = atoi(acTemp);
    g_pstSnmpAgentData->stLteHwInfo.uiSimState = stLteStateInfo.uiSimState;
    snmp_sys_lte_info_init();
*/
    return TRUE;
}

BOOL snmp_lte_net_info(void)
{
 /*   OM_ERROR_CODE_E enRet;
    OM_LTE_STATE_T stLteStateInfo = {0};
    CHAR acTemp[NVRAM_VALUE_MAX_LEN] = {0};

    snmp_log(LOG_NOTICE, "snmp_lte_net_info\n");
    enRet = om_lte_state_get_asyn(&stLteStateInfo);
    if (OM_E_NONE != enRet)
    {
        snmp_log(LOG_ERR, "om_lte_state_get_asyn fail.\n");
        return FALSE;
    }

    if (AP_E_NONE != nvram_get(acTemp, NVRAM_LTE_DAIL_WAY, NVRAM_VALUE_MAX_LEN))
    {
        snmp_log(LOG_ERR, "nvram get fail %s.\n", NVRAM_LTE_DAIL_WAY);
        return FALSE;
    }

    memcpy(g_pstSnmpAgentData->stLteNetInfo.aucOperCompany, stLteStateInfo.szOperCompany, CHAR_LEN_32);   
    g_pstSnmpAgentData->stLteNetInfo.uiLteDailWay = atoi(acTemp);
    g_pstSnmpAgentData->stLteNetInfo.uiLteDailState = stLteStateInfo.uiLteDialState;
    g_pstSnmpAgentData->stLteNetInfo.uiLteSigRssi = stLteStateInfo.iLteSignalRssi;
    g_pstSnmpAgentData->stLteNetInfo.uiLteSigRsrp = stLteStateInfo.iLteSignalRsrp;
    g_pstSnmpAgentData->stLteNetInfo.uiLteSigRsrq = stLteStateInfo.iLteSignalRsrq;
    g_pstSnmpAgentData->stLteNetInfo.uiLteSigSinr = stLteStateInfo.iLteSignalSinr;
    g_pstSnmpAgentData->stLteNetInfo.uiLteSigLevel = stLteStateInfo.iLteSignalLevel;
    g_pstSnmpAgentData->stLteNetInfo.uiAccessNet = stLteStateInfo.stAccessNetMode.enAccessNet;
*/
	
	char  acNowLteClid[32] = {0};
	char  acNowLteRssi[32] = {0};
	char  acNowLteRsrp[32] = {0};
	char  acNowLteRsrq[32] = {0};
	char  acNowLteSinr[32] = {0};
	 
	uci_config_get(UCI_LTEINFO_INFO_RSSI,acNowLteRssi);
	uci_config_get(UCI_LTEINFO_INFO_RSRP,acNowLteRsrp);
	uci_config_get(UCI_LTEINFO_INFO_RSRQ,acNowLteRsrq);
	uci_config_get(UCI_LTEINFO_INFO_SINR,acNowLteSinr);
	
	g_pstSnmpAgentData->stLteNetInfo.uiLteSigRssi = atoi(acNowLteRssi);
    g_pstSnmpAgentData->stLteNetInfo.uiLteSigRsrp = atoi(acNowLteRsrp);
    g_pstSnmpAgentData->stLteNetInfo.uiLteSigRsrq = atoi(acNowLteRsrq);
    g_pstSnmpAgentData->stLteNetInfo.uiLteSigSinr = atoi(acNowLteSinr);
	
    return TRUE;
}

BOOL snmp_lte_cfg_info(void)
{
/*    OM_ERROR_CODE_E enRet;
    OM_LTE_STATE_T stLteStateInfo;
    CHAR acTemp[NVRAM_VALUE_MAX_LEN] = {0};

    memset(&stLteStateInfo, 0, sizeof(stLteStateInfo));

    snmp_log(LOG_NOTICE, "snmp_lte_cfg_info\n");

    if (AP_E_NONE != nvram_get(acTemp, NVRAM_LTE_ACCESS_MODE, NVRAM_VALUE_MAX_LEN))
    {
        snmp_log(LOG_ERR, "nvram get fail %s.\n", NVRAM_LTE_ACCESS_MODE);
        return FALSE;
    }
    g_pstSnmpAgentData->stLteCfgInfo.uiAccessMode = atoi(acTemp);

    if (AP_E_NONE != nvram_get(acTemp, NVRAM_LTE_PRIMODE_SELECT, NVRAM_VALUE_MAX_LEN))
    {
        snmp_log(LOG_ERR, "nvram get fail %s.\n", NVRAM_LTE_PRIMODE_SELECT);
        return FALSE;
    }
    g_pstSnmpAgentData->stLteCfgInfo.uiPriAccessMode = atoi(acTemp);

    enRet = om_lte_apn_get(g_pstSnmpAgentData->stLteCfgInfo.aucApnSelect, CHAR_LEN_32);
    if (OM_E_NONE != enRet)
    {
        snmp_log(LOG_ERR, "om_lte_apn_get fail.\n");
        return FALSE;
    }

    enRet = om_lte_state_get_asyn(&stLteStateInfo);
    if (OM_E_NONE != enRet)
    {
        snmp_log(LOG_ERR, "om_lte_state_get_asyn fail.\n");
        return FALSE;
    }

    memcpy(g_pstSnmpAgentData->stLteCfgInfo.aucLteBandList, stLteStateInfo.stLteBandList.bands, CHAR_LEN_32);
    memcpy(g_pstSnmpAgentData->stLteCfgInfo.aucLteBand, stLteStateInfo.acLteBand, CHAR_LEN_32);
    g_pstSnmpAgentData->stLteCfgInfo.uiLteChannel = stLteStateInfo.uiLteChannel;
    if(0 == strlen(stLteStateInfo.acLteBandLocked))
    {
        strcpy((char *)g_pstSnmpAgentData->stLteCfgInfo.aucLteBandLocked, "N/A");
    }
    else
    {
        memcpy(g_pstSnmpAgentData->stLteCfgInfo.aucLteBandLocked, stLteStateInfo.acLteBandLocked, CHAR_LEN_32);
    }
    memcpy(g_pstSnmpAgentData->stLteCfgInfo.aucLteCID, stLteStateInfo.acCommunityId, CHAR_LEN_32);
    memcpy(g_pstSnmpAgentData->stLteCfgInfo.aucLtePCID, stLteStateInfo.acPhyCommunityId, CHAR_LEN_32);
    memcpy(g_pstSnmpAgentData->stLteCfgInfo.aucLtePCIDLocked, stLteStateInfo.stCellLock.acCellid, CHAR_LEN_32);
    memcpy(g_pstSnmpAgentData->stLteCfgInfo.aucLteChannelLocked, stLteStateInfo.stCellLock.acFreq, CHAR_LEN_32);
*/
    return TRUE;
}

BOOL snmp_lte_wifi_info(void)
{
/*
    snmp_log(LOG_NOTICE, "snmp_lte_wifi_info\n");
    g_pstSnmpAgentData->stWifiInfo.ulWifiSupport = 0;
    */
    return TRUE;
}

BOOL snmp_lte_wan_state_info(void)
{
/*    OM_NET_TS_LIST_T stDevList;
    OM_APP_SYS_INTFACE_INFO_T stWanInfo;
    CHAR acNvramValue[CHAR_LEN_128] = {0};
    UINT32 ulMask;
    OM_ERROR_CODE_E enRet;
    UINT64 ullAllBytes = 0;

    memset(&stDevList, 0, sizeof(stDevList));
    memset(&stWanInfo, 0, sizeof(stWanInfo));
    enRet = nvram_get(acNvramValue, NVRAM_WAN_ACCESS_MODE, sizeof(acNvramValue));
    if (OM_E_NONE != enRet)
    {
        return enRet;
    }

    enRet = om_net_wan_name_get(stDevList.acDevName, sizeof(stDevList.acDevName));
    if (OM_E_NONE != enRet)
    {
        snmp_log(LOG_ERR, "om_net_wired_wan_get_ifname error %d", enRet);
        return FALSE;
    }

    enRet = om_net_get_dev_ts(&stDevList);
    if (OM_E_NONE != enRet)
    {
        snmp_log(LOG_ERR, "om_net_get_dev_ts error %d", enRet);
        return FALSE;
    }

    enRet = om_app_get_wan_info(&stWanInfo);
    if (OM_E_NONE != enRet)
    {
        snmp_log(LOG_ERR, "om_app_get_wan_info error %d", enRet);
        return FALSE;
    }

    g_pstSnmpAgentData->stWanStateInfo.ulWanMode = atoi(acNvramValue) - 1;
    g_pstSnmpAgentData->stWanStateInfo.ulRxBytes = stDevList.stTsIpv4.uiDownBytes + stDevList.stTsIpv6.uiDownBytes;
    g_pstSnmpAgentData->stWanStateInfo.ulRxGB = stDevList.stTsIpv4.uiDownGiga + stDevList.stTsIpv6.uiDownGiga;
    if ((0xFFFFFFFF - stDevList.stTsIpv4.uiDownBytes) < stDevList.stTsIpv6.uiDownBytes)
    {
        g_pstSnmpAgentData->stWanStateInfo.ulRxGB += 1;
    }

    g_pstSnmpAgentData->stWanStateInfo.ulTxBytes = stDevList.stTsIpv4.uiUpBytes + stDevList.stTsIpv6.uiUpBytes;
    g_pstSnmpAgentData->stWanStateInfo.ulTxGB = stDevList.stTsIpv4.uiUpGiga + stDevList.stTsIpv6.uiUpGiga;
    if ((0xFFFFFFFF - stDevList.stTsIpv4.uiUpBytes) < stDevList.stTsIpv6.uiUpBytes)
    {
        g_pstSnmpAgentData->stWanStateInfo.ulTxGB += 1;
    }

    strncpy(g_pstSnmpAgentData->stWanStateInfo.aucWanName, stDevList.acDevName, CHAR_LEN_32);
    memcpy(g_pstSnmpAgentData->stWanStateInfo.aucIp, stWanInfo.aucIp, IP_ALEN);

    if (stWanInfo.ulIpMask == 0)
    {
        ulMask = 0;
    }
    else
    {
        for (ulMask = 1; ulMask <= 32; ulMask++)
        {
            if (((*(UINT32*)stWanInfo.ulIpMask) & (0x1 << (32 - ulMask))) == 0)
            {
                break;
            }
        }
        ulMask -= 1;
    }

    g_pstSnmpAgentData->stWanStateInfo.ulIpMask = ulMask;
    inet_ntop(AF_INET6, stWanInfo.aucIp6, g_pstSnmpAgentData->stWanStateInfo.aucIpv6, CHAR_LEN_64);
    g_pstSnmpAgentData->stWanStateInfo.ulIp6Mask = stWanInfo.ulIp6Mask;
    memcpy(g_pstSnmpAgentData->stWanStateInfo.aucMac, stWanInfo.acMac, ETH_ALEN);
    ullAllBytes = g_pstSnmpAgentData->stWanStateInfo.ulRxGB;
    ullAllBytes = (ullAllBytes<<32) + g_pstSnmpAgentData->stWanStateInfo.ulRxBytes;
    sprintf(g_pstSnmpAgentData->stWanStateInfo.aucRxAllBytes, "%llu", ullAllBytes);
    ullAllBytes = g_pstSnmpAgentData->stWanStateInfo.ulTxGB;
    ullAllBytes = (ullAllBytes<<32) + g_pstSnmpAgentData->stWanStateInfo.ulTxBytes;
    sprintf(g_pstSnmpAgentData->stWanStateInfo.aucTxAllBytes, "%llu", ullAllBytes);
*/
    return TRUE;
}

BOOL snmp_lte_wan_bytes_info(void)
{
/*    OM_NET_TS_LIST_T stDevList;
    OM_ERROR_CODE_E enRet;
    UINT64 ullAllBytes = 0;

    memset(&stDevList, 0, sizeof(stDevList));

    enRet = om_net_wan_name_get(stDevList.acDevName, sizeof(stDevList.acDevName));
    if (OM_E_NONE != enRet)
    {
        snmp_log(LOG_ERR, "om_net_wired_wan_get_ifname error %d", enRet);
        return FALSE;
    }

    enRet = om_net_get_dev_ts(&stDevList);
    if (OM_E_NONE != enRet)
    {
        snmp_log(LOG_ERR, "om_net_get_dev_ts error %d", enRet);
        return FALSE;
    }


    g_pstSnmpAgentData->stWanStateInfo.ulRxBytes = stDevList.stTsIpv4.uiDownBytes + stDevList.stTsIpv6.uiDownBytes;
    g_pstSnmpAgentData->stWanStateInfo.ulRxGB = stDevList.stTsIpv4.uiDownGiga + stDevList.stTsIpv6.uiDownGiga;
    if ((0xFFFFFFFF - stDevList.stTsIpv4.uiDownBytes) < stDevList.stTsIpv6.uiDownBytes)
    {
        g_pstSnmpAgentData->stWanStateInfo.ulRxGB += 1;
    }

    g_pstSnmpAgentData->stWanStateInfo.ulTxBytes = stDevList.stTsIpv4.uiUpBytes + stDevList.stTsIpv6.uiUpBytes;
    g_pstSnmpAgentData->stWanStateInfo.ulTxGB = stDevList.stTsIpv4.uiUpGiga + stDevList.stTsIpv6.uiUpGiga;
    if ((0xFFFFFFFF - stDevList.stTsIpv4.uiUpBytes) < stDevList.stTsIpv6.uiUpBytes)
    {
        g_pstSnmpAgentData->stWanStateInfo.ulTxGB += 1;
    }

    strncpy(g_pstSnmpAgentData->stWanStateInfo.aucWanName, stDevList.acDevName, CHAR_LEN_32);
    ullAllBytes = g_pstSnmpAgentData->stWanStateInfo.ulRxGB;
    ullAllBytes = (ullAllBytes<<32) + g_pstSnmpAgentData->stWanStateInfo.ulRxBytes;
    sprintf(g_pstSnmpAgentData->stWanStateInfo.aucRxAllBytes, "%llu", ullAllBytes);
    ullAllBytes = g_pstSnmpAgentData->stWanStateInfo.ulTxGB;
    ullAllBytes = (ullAllBytes<<32) + g_pstSnmpAgentData->stWanStateInfo.ulTxBytes;
    sprintf(g_pstSnmpAgentData->stWanStateInfo.aucTxAllBytes, "%llu", ullAllBytes);

  */  return TRUE;
}


BOOL snmp_lte_user_info(void)
{
/*    OM_NET_TS_LIST_T astUserList[TABLE_MAXRECNUM_LTE_USER];
    INT32 iNum = 0;
    INT32 iLoop = 0;

    memset(astUserList, 0, sizeof(astUserList));
    if (OM_E_NONE != om_net_user_list_get(astUserList, &iNum, TABLE_MAXRECNUM_LTE_USER))
    {
        snmp_log(LOG_ERR, "om_net_user_list_get error ");
        return FALSE;
    }

    memset(g_pstSnmpAgentData->stWanUserInfo, 0, sizeof(g_pstSnmpAgentData->stWanUserInfo));

    for (iLoop = 0; iLoop < iNum; iLoop++)
    {
        g_pstSnmpAgentData->stWanUserInfo[iLoop].uiUserId = iLoop + 1;
        memcpy(g_pstSnmpAgentData->stWanUserInfo[iLoop].aucUserMac, astUserList[iLoop].aucMac, ETH_ALEN);
        g_pstSnmpAgentData->stWanUserInfo[iLoop].uiUserType = astUserList[iLoop].uiType;
        g_pstSnmpAgentData->stWanUserInfo[iLoop].ulRxBytes = astUserList[iLoop].stTsIpv4.uiUpBytes + astUserList[iLoop].stTsIpv6.uiUpBytes;
        g_pstSnmpAgentData->stWanUserInfo[iLoop].ulRxGB = astUserList[iLoop].stTsIpv4.uiUpGiga + astUserList[iLoop].stTsIpv6.uiUpGiga;
        if ((0xFFFFFFFF - astUserList[iLoop].stTsIpv4.uiUpBytes) < astUserList[iLoop].stTsIpv6.uiUpBytes)
        {
            g_pstSnmpAgentData->stWanUserInfo[iLoop].ulRxGB += 1;
        }
        g_pstSnmpAgentData->stWanUserInfo[iLoop].ulTxBytes = astUserList[iLoop].stTsIpv4.uiDownBytes + astUserList[iLoop].stTsIpv6.uiDownBytes;
        g_pstSnmpAgentData->stWanUserInfo[iLoop].ulTxGB = astUserList[iLoop].stTsIpv4.uiDownGiga + astUserList[iLoop].stTsIpv6.uiDownGiga;
        if ((0xFFFFFFFF - astUserList[iLoop].stTsIpv4.uiDownBytes) < astUserList[iLoop].stTsIpv6.uiDownBytes)
        {
            g_pstSnmpAgentData->stWanUserInfo[iLoop].ulTxGB += 1;
        }
        memcpy(g_pstSnmpAgentData->stWanUserInfo[iLoop].aucUserIp, &(astUserList[iLoop].uiIp), IP_ALEN);
        inet_ntop(AF_INET6, astUserList[iLoop].aucIpv6, g_pstSnmpAgentData->stWanUserInfo[iLoop].aucUserIpv6, CHAR_LEN_64);
    }
#if 0
    for (iLoop = 0; iLoop < iNum; iLoop++)
    {
        printf("g_pstSnmpAgentData->stWanUserInfo[iLoop].uiUserId=%d\n", g_pstSnmpAgentData->stWanUserInfo[iLoop].uiUserId);
    }
#endif
*/
    return TRUE;
}
/*add by wangqi begin*/
BOOL snmp_lte_keepalive_cfg(void)
{/*
    OM_LTE_KEEPALIVE_CFG_T stLteKaCfg;
    OM_ERROR_CODE_E enRet;
    
    memset(&g_pstSnmpAgentData->stLteKPCfg, 0, sizeof(g_pstSnmpAgentData->stLteKPCfg));
    memset(&stLteKaCfg, 0, sizeof(stLteKaCfg));
    enRet = om_lte_keepalive_config_get(&stLteKaCfg);
    if (OM_E_NONE != enRet)
    {
        snmp_log(LOG_ERR, "om_net_get_dev_ts error %d", enRet);
        return FALSE;
    }
    g_pstSnmpAgentData->stLteKPCfg.uiInterval = stLteKaCfg.uiCheckInterval;
    g_pstSnmpAgentData->stLteKPCfg.uiSwitch   = stLteKaCfg.eCheckSw;
    g_pstSnmpAgentData->stLteKPCfg.uiNum      = 1;
*/
    return TRUE;
}
/*add by wangqi end*/
BOOL snmp_lte_keepalive_info(void)
{
/*    OM_LTE_KEEPALIVE_CFG_T stLteKaCfg;
    OM_ERROR_CODE_E enRet;

    memset(g_pstSnmpAgentData->stLteKPInfo, 0, sizeof(g_pstSnmpAgentData->stLteKPInfo));

    g_pstSnmpAgentData->stLteKPInfo[0].uiId = 1;
    g_pstSnmpAgentData->stLteKPInfo[0].uiId = ENUM_ROWSTATUS_ACTIVE;

    memset(&stLteKaCfg, 0, sizeof(stLteKaCfg));
    enRet = om_lte_keepalive_config_get(&stLteKaCfg);
    if (OM_E_NONE != enRet)
    {
        snmp_log(LOG_ERR, "om_net_get_dev_ts error %d", enRet);
        return FALSE;
    }

    if (stLteKaCfg.eCheckSw == LTE_KEEPALIVE_SWITCH_ON)
    {
        memcpy(g_pstSnmpAgentData->stLteKPInfo[0].aucIp, stLteKaCfg.aucCheckIp, IP_ALEN);
        strncpy(g_pstSnmpAgentData->stLteKPInfo[0].aucIpv6, "N/A", CHAR_LEN_64);
    }
*/
    return TRUE;
}

/*add by wangqi begin*/
BOOL snmp_lte_pppoe_user_num(void)
{
/*    OM_NET_TS_LIST_T astUserList[TABLE_MAXRECNUM_LTE_USER];
    INT32 iNum = 0;

    memset(astUserList, 0, sizeof(astUserList));
    if (OM_E_NONE != om_net_user_list_get(astUserList, &iNum, TABLE_MAXRECNUM_LTE_USER))
    {
        snmp_log(LOG_ERR, "om_net_user_list_get error ");
        g_pstSnmpAgentData->stPppoeUserNum.uiUserNum= 0;
        return FALSE;
    }
    g_pstSnmpAgentData->stPppoeUserNum.uiUserNum= iNum;
    */
    return TRUE;
}
/*add by wangqi end*/

BOOL snmp_lte_pppoe_user_info(void)
{
/*    OM_NET_TS_LIST_T astUserList[TABLE_MAXRECNUM_LTE_USER];
    INT32 iNum = 0;
    INT32 iLoop = 0;

    memset(astUserList, 0, sizeof(astUserList));
    if (OM_E_NONE != om_net_user_list_get(astUserList, &iNum, TABLE_MAXRECNUM_LTE_USER))
    {
        snmp_log(LOG_ERR, "om_net_user_list_get error ");
        return FALSE;
    }

    memset(g_pstSnmpAgentData->stPppoeUserInfo, 0, sizeof(g_pstSnmpAgentData->stPppoeUserInfo));

    for(iLoop = 0; iLoop < iNum; iLoop++)
    {
        g_pstSnmpAgentData->stPppoeUserInfo[iLoop].uiUserId = iLoop + 1;
        memcpy(g_pstSnmpAgentData->stPppoeUserInfo[iLoop].aucPppoeIp , &astUserList[iLoop].uiIp,IP_ALEN);
        inet_ntop(AF_INET6, astUserList[iLoop].aucIpv6,g_pstSnmpAgentData->stPppoeUserInfo[iLoop].aucPppoeIpv6,CHAR_LEN_64);
        memcpy(g_pstSnmpAgentData->stPppoeUserInfo[iLoop].pppoeServerName, "N/A", sizeof("N/A"));
        if(0 == strlen(astUserList[iLoop].acUsername))
        {
            memcpy(g_pstSnmpAgentData->stPppoeUserInfo[iLoop].pppoeUserName,"N/A", sizeof("N/A"));
        }else
        {
            memcpy(g_pstSnmpAgentData->stPppoeUserInfo[iLoop].pppoeUserName,astUserList[iLoop].acUsername, sizeof(astUserList[iLoop].acUsername));
        }
        memcpy(g_pstSnmpAgentData->stPppoeUserInfo[iLoop].pppoePassword, "N/A", sizeof("N/A"));
    }
*/
    return TRUE;
}


/*add by wangqi begin*/
BOOL snmp_lte_pppoe_all_user_names(void)
{
/*    CHAR acTemp[NVRAM_VALUE_MAX_LEN] = {0};
    memset(&g_pstSnmpAgentData->stPppoeAllUserNames, 0, sizeof(g_pstSnmpAgentData->stPppoeAllUserNames));
    if(AP_E_NONE != nvram_get(acTemp,NEW_PPPOE_USER_NAME, NVRAM_VALUE_MAX_LEN))
    {
        snmp_log(LOG_ERR, "nvram get NEW_PPPOE_USER_NAME failed");
        strcpy(g_pstSnmpAgentData->stPppoeAllUserNames.uiAllUserNames ,"N/A");
    }
    else
    {
        if(0 == strlen(acTemp))
        {
            strcpy(g_pstSnmpAgentData->stPppoeAllUserNames.uiAllUserNames ,"N/A");
        }
        else
        {
            strncpy(g_pstSnmpAgentData->stPppoeAllUserNames.uiAllUserNames ,acTemp, NVRAM_VALUE_MAX_LEN-1);
        }        
    }
    
#if 0

    OM_NET_TS_LIST_T astUserList[TABLE_MAXRECNUM_LTE_USER];
    INT32 iNameLen = 0, iNum = 0;
    INT32 iLoop = 0, iTotalLen = 0;

    memset(&g_pstSnmpAgentData->stPppoeAllUserNames, 0, sizeof(g_pstSnmpAgentData->stPppoeAllUserNames));
    memset(astUserList, 0, sizeof(astUserList));
    if (OM_E_NONE != om_net_user_list_get(astUserList, &iNum, TABLE_MAXRECNUM_LTE_USER))
    {
        snmp_log(LOG_ERR, "om_net_user_list_get error ");
        strcpy(g_pstSnmpAgentData->stPppoeAllUserNames.uiAllUserNames ,"N/A");
        return FALSE;
    }   

    for(iLoop = 0; iLoop < iNum; iLoop++)
    {
        iNameLen = strlen(astUserList[iLoop].acUsername);
        if(0 != iNameLen && sizeof(g_pstSnmpAgentData->stPppoeAllUserNames) > iTotalLen + iNameLen)
        {
            if(iTotalLen != 0)
            {
                g_pstSnmpAgentData->stPppoeAllUserNames.uiAllUserNames[iTotalLen] = ',';
                ++iTotalLen;
            }
            strncpy(g_pstSnmpAgentData->stPppoeAllUserNames.uiAllUserNames+iTotalLen ,astUserList[iLoop].acUsername, iNameLen);
            iTotalLen += iNameLen;
        }
        else
        {
            break;
        }
    }
    g_pstSnmpAgentData->stPppoeAllUserNames.uiAllUserNames[sizeof(g_pstSnmpAgentData->stPppoeAllUserNames)-1] = '\0';
    if(0 == strlen(g_pstSnmpAgentData->stPppoeAllUserNames.uiAllUserNames))
    {
        strcpy(g_pstSnmpAgentData->stPppoeAllUserNames.uiAllUserNames ,"N/A");
    }
#endif
*/
    return TRUE;
}
/*add by wangqi end*/


BOOL snmp_lte_enodb_info(void)
{
 /*   OM_LTE_STATE_T stLteState = {0};
    om_lte_state_get_asyn(&stLteState);
    if(0 == strlen(stLteState.stEnodeBid.acEnodebId))
    {
        strcpy(g_pstSnmpAgentData->stLteEnodb.acLteENodeBId, "N/A");
    }
    else
    {
        memcpy(g_pstSnmpAgentData->stLteEnodb.acLteENodeBId, stLteState.stEnodeBid.acEnodebId, sizeof(g_pstSnmpAgentData->stLteEnodb.acLteENodeBId));
    }
    
    if(0 == strlen(stLteState.stLcId.acLcid))
    {
        strcpy(g_pstSnmpAgentData->stLteEnodb.acLteLCID, "N/A");
    }
    else
    {
        memcpy(g_pstSnmpAgentData->stLteEnodb.acLteLCID, stLteState.stLcId.acLcid, sizeof(g_pstSnmpAgentData->stLteEnodb.acLteLCID));
    }
    
    if(0 == strlen(stLteState.stTac.acTac))
    {
        strcpy(g_pstSnmpAgentData->stLteEnodb.acLteTac, "N/A");
    }
    else
    {
        memcpy(g_pstSnmpAgentData->stLteEnodb.acLteTac, stLteState.stTac.acTac, sizeof(g_pstSnmpAgentData->stLteEnodb.acLteTac));
    }
    */
    return TRUE;
}

/*add by wangqi begin*/
BOOL snmp_lte_station_lock(void)
{
/*    CHAR acTemp[NVRAM_VALUE_MAX_LEN] = {0};
    CHAR acList[LTE_LOGIC_CELL_MAX] = {0};
    UINT32 eCellSw=0, iIdNum=0;
    memset(&g_pstSnmpAgentData->stLteStationLock, 0, sizeof(g_pstSnmpAgentData->stLteStationLock));
    if(AP_E_NONE != nvram_get(acTemp,NVRAM_LTE_LOGIC_CELL_SW, NVRAM_VALUE_MAX_LEN))
    {
        snmp_log(LOG_ERR, "nvram get NVRAM_LTE_LOGIC_CELL_SW failed");
        return FALSE;
    }
    
    eCellSw = atoi(acTemp);
    //printf("eCellSw==%d\n", eCellSw);
    g_pstSnmpAgentData->stLteStationLock.ulLteLockType = eCellSw;
    if(LTE_CELL_SW_CELL == eCellSw)
    {
        memset(acTemp,0,sizeof(acTemp));
        if(AP_E_NONE != nvram_get(acTemp,NVRAM_LTE_CELL_AUTO_LOCK_SW, NVRAM_VALUE_MAX_LEN))
        {
            snmp_log(LOG_ERR, "nvram get NVRAM_LTE_CELL_AUTO_LOCK_SW failed");
            return FALSE;
        }
        g_pstSnmpAgentData->stLteStationLock.ulLteAutoLock = atoi(acTemp);
        //printf("ulLteAutoLock==%d\n", g_pstSnmpAgentData->stLteStationLock.ulLteAutoLock);
        memset(acTemp,0,sizeof(acTemp));
        if(AP_E_NONE != nvram_get(acTemp,NVRAM_LTE_CELL_AUTO_LOCK_NUM, NVRAM_VALUE_MAX_LEN))
        {
            snmp_log(LOG_ERR, "nvram get NVRAM_LTE_CELL_AUTO_LOCK_NUM failed");
            return FALSE;
        }
        g_pstSnmpAgentData->stLteStationLock.ulLteAutoMax = atoi(acTemp);
        //printf("ulLteAutoMax==%d\n", g_pstSnmpAgentData->stLteStationLock.ulLteAutoMax);
        memset(acTemp,0,sizeof(acTemp));
        if(AP_E_NONE != nvram_get(acTemp,NVRAM_LTE_LOGIC_CELL_LIST, NVRAM_VALUE_MAX_LEN))
        {
            snmp_log(LOG_ERR, "nvram get NVRAM_LTE_LOGIC_CELL_LIST failed");
            iIdNum = 0;
        }
        else
        {
            util_array_string_to_int(acTemp,acList,&iIdNum);
        }
        g_pstSnmpAgentData->stLteStationLock.ulLteLockNum = iIdNum;
    }
    else if (LTE_CELL_SW_ENODEB == eCellSw)
    {
        memset(acTemp,0,sizeof(acTemp));
        if(AP_E_NONE != nvram_get(acTemp,NVRAM_LTE_ENODEB_AUTO_LOCK_SW, NVRAM_VALUE_MAX_LEN))
        {
            snmp_log(LOG_ERR, "nvram get NVRAM_LTE_ENODEB_AUTO_LOCK_SW failed");
            return FALSE;
        }
        g_pstSnmpAgentData->stLteStationLock.ulLteAutoLock = atoi(acTemp);

        memset(acTemp,0,sizeof(acTemp));
        if(AP_E_NONE != nvram_get(acTemp,NVRAM_LTE_ENODEB_AUTO_LOCK_NUM, NVRAM_VALUE_MAX_LEN))
        {
            snmp_log(LOG_ERR, "nvram get NVRAM_LTE_ENODEB_AUTO_LOCK_NUM failed");
            return FALSE;
        }
        g_pstSnmpAgentData->stLteStationLock.ulLteAutoMax = atoi(acTemp);

        memset(acTemp,0,sizeof(acTemp));
        if(AP_E_NONE != nvram_get(acTemp,NVRAM_LTE_ENODEB_LIST, NVRAM_VALUE_MAX_LEN))
        {
            snmp_log(LOG_ERR, "nvram get NVRAM_LTE_ENODEB_LIST failed");
            iIdNum = 0;
        }
        else
        {
            util_array_string_to_int(acTemp,acList,&iIdNum);
        }
        g_pstSnmpAgentData->stLteStationLock.ulLteLockNum = iIdNum;
    }
    else if (LTE_CELL_SW_TAC == eCellSw )
    {
        memset(acTemp,0,sizeof(acTemp));
        if(AP_E_NONE != nvram_get(acTemp,NVRAM_LTE_TAC_AUTO_LOCK_SW, NVRAM_VALUE_MAX_LEN))
        {
            snmp_log(LOG_ERR, "nvram get NVRAM_LTE_TAC_AUTO_LOCK_SW failed");
            return FALSE;
        }
        g_pstSnmpAgentData->stLteStationLock.ulLteAutoLock= atoi(acTemp);

        memset(acTemp,0,sizeof(acTemp));
        if(AP_E_NONE != nvram_get(acTemp,NVRAM_LTE_TAC_AUTO_LOCK_NUM, NVRAM_VALUE_MAX_LEN))
        {
            snmp_log(LOG_ERR, "nvram get NVRAM_LTE_TAC_AUTO_LOCK_NUM failed");
            return FALSE;
        }
        g_pstSnmpAgentData->stLteStationLock.ulLteAutoMax = atoi(acTemp);

        memset(acTemp,0,sizeof(acTemp));
        if(AP_E_NONE != nvram_get(acTemp,NVRAM_LTE_TAC_LIST, NVRAM_VALUE_MAX_LEN))
        {
            snmp_log(LOG_ERR, "nvram get NVRAM_LTE_TAC_LIST failed");
            iIdNum = 0;
        }
        else
        {
            util_array_string_to_int(acTemp,acList,&iIdNum);
        }        
        g_pstSnmpAgentData->stLteStationLock.ulLteLockNum = iIdNum;
    }
    */
    return TRUE;
}

#if 0
BOOL snmp_lte_station_lock_table(void)
{
/*    OM_LTE_CELL_CFG_T  stLteCellCfg = {0};
    int i;
    CHAR *pcTmp=NULL;
    int iNum = 0;
    
    stLteCellCfg.pcCellList=(CHAR*)malloc(CHAR_LEN_32*LTE_LOGIC_CELL_MAX);
    stLteCellCfg.pcEnodeList=stLteCellCfg.pcCellList;
    stLteCellCfg.pcTacList=stLteCellCfg.pcCellList;
    pcTmp = stLteCellCfg.pcCellList;
    memset(stLteCellCfg.pcCellList,0,CHAR_LEN_32*LTE_LOGIC_CELL_MAX);
    om_lte_cell_config_get(&stLteCellCfg);
    
    g_pstSnmpAgentData->stLteStationLock.ulLteLockType = stLteCellCfg.eCellSw;
    if(LTE_CELL_SW_CELL == stLteCellCfg.eCellSw)
    {
        iNum = stLteCellCfg.uiCellNum;
    }
    else if (LTE_CELL_SW_ENODEB == stLteCellCfg.eCellSw)
    {
        iNum = stLteCellCfg.uiEnodeNum;
    }
    else if (LTE_CELL_SW_TAC == stLteCellCfg.eCellSw )
    {
       iNum = stLteCellCfg.uiTacNum;
    }

    for(i=0;i < iNum && i < TABLE_MAXRECNUM_LTE_LOCK_TABLE;i++)
    {
         memcpy(g_pstSnmpAgentData->stLteStationLockTable[i].ulLteLockID,pcTmp+i*32,CHAR_LEN_32);
    }
*/
    return TRUE;
}
#endif

/******************************************************************
功能:获取ID列表
******************************************************************/
BOOL snmp_lte_station_lock_table(void)
{
/*    CHAR acTemp[NVRAM_VALUE_MAX_LEN] = {0};
    CHAR acValue[NVRAM_VALUE_MAX_LEN] = {0};
    INT32 iLoop;
    CHAR acList[LTE_LOGIC_CELL_MAX] = {0};
    UINT32 ulLteLockType,iIdNum=0;

    memset(acTemp,0,sizeof(acTemp));
    if(AP_E_NONE != nvram_get(acTemp,NVRAM_LTE_LOGIC_CELL_SW, NVRAM_VALUE_MAX_LEN))
    {
        snmp_log(LOG_ERR, "nvram get NVRAM_LTE_LOGIC_CELL_SW failed");
        return FALSE;
    }
    ulLteLockType = atoi(acTemp);
    g_pstSnmpAgentData->stLteStationLock.ulLteLockType = ulLteLockType;
    memset(g_pstSnmpAgentData->stLteStationLockTable, 0, sizeof(g_pstSnmpAgentData->stLteStationLockTable));
    if(LTE_CELL_SW_CELL == ulLteLockType)
    {
        memset(acTemp,0,sizeof(acTemp));
        if(AP_E_NONE != nvram_get(acTemp,NVRAM_LTE_LOGIC_CELL_LIST, NVRAM_VALUE_MAX_LEN))
        {
            snmp_log(LOG_ERR, "nvram get NVRAM_LTE_LOGIC_CELL_LIST failed");
            return FALSE;
        }
        util_array_string_to_int(acTemp,acList,&iIdNum);
        for ( iLoop = 0; iLoop < iIdNum && (0 != iIdNum) ; iLoop++ )
        {
            memset(acTemp,0,sizeof(acTemp));
            sprintf(acTemp,NVRAM_LTE_LOGIC_CELL_ID_"%d",acList[iLoop]);
            if(AP_E_NONE != nvram_get(acValue,acTemp, NVRAM_VALUE_MAX_LEN))
            {
                snmp_log(LOG_ERR, "nvram get %s failed",acTemp);
                continue;
            }
            g_pstSnmpAgentData->stLteStationLockTable[iLoop].ulLteLockNumID = (UINT32)acList[iLoop]+1;
            memcpy(g_pstSnmpAgentData->stLteStationLockTable[iLoop].ulLteLockID,acValue,CHAR_LEN_32);
            //printf("ulLteLockNumID==%d\n", g_pstSnmpAgentData->stLteStationLockTable[iLoop].ulLteLockNumID);
        }
    }
    else if(LTE_CELL_SW_ENODEB == ulLteLockType)
    {
        memset(acTemp,0,sizeof(acTemp));
        if(AP_E_NONE != nvram_get(acTemp,NVRAM_LTE_ENODEB_LIST, NVRAM_VALUE_MAX_LEN))
        {
            snmp_log(LOG_ERR, "nvram get NVRAM_LTE_LOGIC_CELL_LIST failed");
            return FALSE;
        }
        util_array_string_to_int(acTemp,acList,&iIdNum);
        for ( iLoop = 0; iLoop < iIdNum && (0 != iIdNum) ; iLoop++ )
        {
            memset(acTemp,0,sizeof(acTemp));
            sprintf(acTemp,NVRAM_LTE_ENODEB_ID_"%d",acList[iLoop]);
            if(AP_E_NONE != nvram_get(acValue,acTemp, NVRAM_VALUE_MAX_LEN))
            {
                snmp_log(LOG_ERR, "nvram get %s failed",acTemp);
                continue;
            }
            g_pstSnmpAgentData->stLteStationLockTable[iLoop].ulLteLockNumID = (UINT32)acList[iLoop]+1;
            memcpy(g_pstSnmpAgentData->stLteStationLockTable[iLoop].ulLteLockID,acValue,16);
        }
    }
    else if(LTE_CELL_SW_TAC == ulLteLockType)
    {
        memset(acTemp,0,sizeof(acTemp));
        if(AP_E_NONE != nvram_get(acTemp,NVRAM_LTE_TAC_LIST, NVRAM_VALUE_MAX_LEN))
        {
            snmp_log(LOG_ERR, "nvram get NVRAM_LTE_LOGIC_CELL_LIST failed");
            return FALSE;
        }
        util_array_string_to_int(acTemp,acList,&iIdNum);
        for ( iLoop = 0; iLoop < iIdNum && (0 != iIdNum) ; iLoop++ )
        {
            memset(acTemp,0,sizeof(acTemp));
            sprintf(acTemp,NVRAM_LTE_TAC_"%d",acList[iLoop]);
            if(AP_E_NONE != nvram_get(acValue,acTemp, NVRAM_VALUE_MAX_LEN))
            {
                snmp_log(LOG_ERR, "nvram get %s failed",acTemp);
                continue;
            }
            g_pstSnmpAgentData->stLteStationLockTable[iLoop].ulLteLockNumID = (UINT32)acList[iLoop]+1;
            memcpy(g_pstSnmpAgentData->stLteStationLockTable[iLoop].ulLteLockID,acValue,16);
        }
    }
    */
    return TRUE;
}

BOOL snmp_lte_station_all_lock_id(void)
{
/*    int iLoop = 0, iNameLen = 0, iTotalLen = 0, iLoop1 = 0;
    CHAR *paLockID = NULL;
    int iEnodeb = 0, iLcid = 0;
    memset(&g_pstSnmpAgentData->stLteStationAllLockID, 0, sizeof(g_pstSnmpAgentData->stLteStationAllLockID));
    if (FALSE == snmp_lte_station_lock_table())
    {
        snmp_log(LOG_ERR, "get lte wan user info fail\n");
        strcpy(g_pstSnmpAgentData->stLteStationAllLockID.acLteLockAllID,"N/A");
        return FALSE;
    }   
    if(LTE_CELL_SW_CELL == g_pstSnmpAgentData->stLteStationLock.ulLteLockType)
    {
        for(iLoop = 0; iLoop < TABLE_MAXRECNUM_LTE_LOCK_TABLE; iLoop++)
        {
            iNameLen = strlen(g_pstSnmpAgentData->stLteStationLockTable[iLoop].ulLteLockID);
            if(0 == iNameLen)
            {
                break;
            }
            if(sizeof(g_pstSnmpAgentData->stLteStationAllLockID) > iTotalLen + iNameLen + 2)
            {
                paLockID=(char *)(g_pstSnmpAgentData->stLteStationLockTable[iLoop].ulLteLockID);
                if(iTotalLen != 0)
                {
                    g_pstSnmpAgentData->stLteStationAllLockID.acLteLockAllID[iTotalLen] = ',';
                    ++iTotalLen;
                }
                for(iLoop1 = 0, iEnodeb = 0; iLoop1 < iNameLen-2; iLoop1++)
                {
                    if(*(paLockID+iLoop1)<='9' && *(paLockID+iLoop1)>='0')
                    {
                        iEnodeb = iEnodeb*16 + *(paLockID+iLoop1)-'0';
                    }
                    else if(*(paLockID+iLoop1)<='F' && *(paLockID+iLoop1)>='A')
                    {
                        iEnodeb = iEnodeb*16 + *(paLockID+iLoop1)-'A'+10;
                    }
                    else if(*(paLockID+iLoop1)<='f' && *(paLockID+iLoop1)>='a')
                    {
                        iEnodeb = iEnodeb*16 + *(paLockID+iLoop1)-'a'+10;
                    }
                    else
                    {
                        snmp_log(LOG_ERR, "cell id error\n");
                        return FALSE;
                    }          
                }
                for(iLcid = 0; iLoop1 < iNameLen; iLoop1++)
                {
                    if(*(paLockID+iLoop1)<='9' && *(paLockID+iLoop1)>='0')
                    {
                        iLcid = iLcid*16 + *(paLockID+iLoop1)-'0';
                    }
                    else if(*(paLockID+iLoop1)<='F' && *(paLockID+iLoop1)>='A')
                    {
                        iLcid = iLcid*16 + *(paLockID+iLoop1)-'A'+10;
                    }
                    else if(*(paLockID+iLoop1)<='f' && *(paLockID+iLoop1)>='a')
                    {
                        iLcid = iLcid*16 + *(paLockID+iLoop1)-'a'+10;
                    }
                    else
                    {
                        snmp_log(LOG_ERR, "cell id error\n");
                        return FALSE;
                    }        
                }
                iTotalLen +=sprintf(g_pstSnmpAgentData->stLteStationAllLockID.acLteLockAllID+iTotalLen, "%d/%d", iEnodeb, iLcid);
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        for(iLoop = 0; iLoop < TABLE_MAXRECNUM_LTE_LOCK_TABLE; iLoop++)
        {
            iNameLen = strlen(g_pstSnmpAgentData->stLteStationLockTable[iLoop].ulLteLockID);
            if(0 == iNameLen)
            {
                break;
            }
            if(sizeof(g_pstSnmpAgentData->stLteStationAllLockID) > iTotalLen + iNameLen)
            {
                if(iTotalLen != 0)
                {
                    g_pstSnmpAgentData->stLteStationAllLockID.acLteLockAllID[iTotalLen] = ',';
                    ++iTotalLen;
                }
                strncpy(g_pstSnmpAgentData->stLteStationAllLockID.acLteLockAllID+iTotalLen ,g_pstSnmpAgentData->stLteStationLockTable[iLoop].ulLteLockID, iNameLen);
                iTotalLen += iNameLen;
            }
            else
            {
                break;
            }
        }
    }
    
    g_pstSnmpAgentData->stLteStationAllLockID.acLteLockAllID[sizeof(g_pstSnmpAgentData->stLteStationAllLockID)-1] = '\0';
    if(0 == strlen(g_pstSnmpAgentData->stLteStationAllLockID.acLteLockAllID))
    {
        strcpy(g_pstSnmpAgentData->stLteStationAllLockID.acLteLockAllID ,"N/A");
    }
*/
    return TRUE; 
    
}
/*add by wangqi end*/


