/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : interface_wireless.c
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2012/11/7
  Last Modified :
  Description   : snmp模块和无线驱动接口
  Function List :
  History       :
  1.Date        : 2012/11/7
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/wireless.h>
#include <error.h>

#include "my_snmp_def.h"
#include "my_snmp_data.h"
#include "mini_snmpd.h"
#include "my_snmp_handle.h"
#include "my_snmp_util.h"
#include "snmp_interface.h"

#include "config_common.h"
#include "nvram_config.h"
#include "interface.h"
#include "tool.h"


#ifdef OPENWRT
#include "pal_wifi.h"
#include "pal_network.h"
#else
#include "gbcom_wifi_pal.h"
#endif
extern char *cmd_return_str(char * cmd);

/*****************************************************************************
 Prototype    : wlan_get_wifimac
 Description  : 获取无线设备卡的MAC地址
 Input        : char *pcDevName
                char *pcDevMac
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2012/11/7
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
BOOL wlan_get_wifimac( char *pcDevName, char *pcDevMac )
{
    FILE *pfData = NULL;
    char acStrCmd[256] = {0};
    char cBuff[128] = {0};
    int iMac[6] = {0};
    int iLoop = 0;

    sprintf(acStrCmd, "ifconfig %s | grep HWaddr | cut -f 2 -d 'H' | cut -f 2 -d ' ' > /tmp/macs.tmp", pcDevName);
    system(acStrCmd);

    pfData = fopen("/tmp/macs.tmp", "r");
    if (NULL == pfData)
    {
        agent_printf(LOG_ERROR, "open /tmp/macs.tmp fail");
        system("rm /tmp/macs.tmp");
        return FALSE;
    }
    if (NULL == fgets(cBuff, 128, pfData))
    {
        agent_printf(LOG_ERROR, "open /tmp/macs.tmp fail");
        system("rm /tmp/macs.tmp");
        fclose(pfData);
        return FALSE;
    }
    fclose(pfData);
    system("rm /tmp/macs.tmp");
    sscanf(cBuff, "%02x-%02x-%02x-%02x-%02x-%02x", 
        &iMac[0], &iMac[1], &iMac[2], &iMac[3], &iMac[4], &iMac[5]);

    for (iLoop = 0; iLoop < 6; iLoop++)
    {
        pcDevMac[iLoop] = iMac[iLoop];
    }
    
    return TRUE;
}/* end wlan_get_wifimac */

/*****************************************************************************
 Prototype    : wlan_get_wifibasicinfo
 Description  : 获取无线的基本信息
 Input        : AP_CONFIG_WIFIDEVICEBASIC_ENTRY_T *pstWifiBasicInfo
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2012/11/10
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
BOOL wlan_get_wifibasicinfo(AP_CONFIG_WIFIDEVICEBASIC_T *pstWifiBasicInfo)
{
    char acWifiName[32] = {0};

    int iRet = 0;

    if(NULL == pstWifiBasicInfo)
    {
        agent_printf(LOG_ERROR, "pstWifiBasicInfo is NULL");
        return FALSE;
    }

    pstWifiBasicInfo->ulWifiDeviceType = 0;

    sprintf(acWifiName, "wifi%d", pstWifiBasicInfo->ulWifiId);
    iRet = wlan_get_wifimac(acWifiName, (char *)pstWifiBasicInfo->aucMacAddress);
    if (FALSE == iRet)
    {
        agent_printf(LOG_ERROR, "get %s mac fail", acWifiName);
        return FALSE;
    }
    agent_printf(LOG_DEBUG, "the %s dev aucMacAddress="MAC_FMT" ulWifiDeviceType=%d", 
        acWifiName, MAC_ARG(pstWifiBasicInfo->aucMacAddress), pstWifiBasicInfo->ulWifiDeviceType);
    
    return TRUE;
}/* end wlan_get_wifibasicinfo */

#ifdef OPENWRT
BOOL wlan_get_wifichannelinfo( AP_CONFIG_WIFIDEVICECHANNEL_T *pstWifiChanInfo )
{
    
}

BOOL wlan_get_wifitxpowerinfo( AP_CONFIG_WIFIDEVICEPOWER_T *pstWifiTxPower )
{
}


BOOL wlan_get_wifiadvanceinfo( AP_CONFIG_WIFIDEVICEADVANCE_T *pstApWifiAdvance )
{

}

BOOL wlan_get_wifirateinfo( AP_CONFIG_WIFIDEVICERATE_T *pstApWifiRate )
{
}





BOOL wlan_get_userinfo(AP_STAT_STATION_ENTRY_T *pstUserInfo, UINT32 *pulUserNum)
{
    unsigned long ulMaxEndUser = 0 ;
    unsigned long ulVlanId = 0;
    unsigned long ulVlanEnable = 0;
    char acFrequency[32] = {0};
    char devname[32] = {0};
    char acNvramName[NVRAM_SIZE] = {0};
    char acNvramValue[NVRAM_SIZE] = {0};
    char list[128];
    char vaps_list[32];
    int total;
    int loop =0;
    int iLoop = 0;
    int iCount = 0;

    char acVlanId[32] = {0};
    char acVlanEnable[32] = {0};
    UINT8 aucBssid[6] = {0};
    float fTxRate = 0.0;
    float fErrRate = 0.0;
    unsigned long ulTotalPkt = 0;

    char vap_shortgi[32];
    char vap_band[32];
    char vap_chainmask[32];

    AP_VAP_BASIC_T stVapBasic;
    WIFI_INFO_T stWifiInfo;

    DHCP_CLIENT_INFO stClient;

    if (NULL == pstUserInfo)
    {
        lprintf(LOG_ERROR, "the pstUserInfo is NULL");
        return FALSE;
    }
    if ( NULL == pulUserNum)
    {
        agent_printf(LOG_ERR, "the pulUserNum is NULL.\n");
        return FALSE;
    }

    total = pal_wifi_get_vap_num();

    agent_printf(LOG_ERR, "total %d.\n",total);

    WIFI_API_ENDUSER_STATS_T   stEndUserOmc;
    stEndUserOmc.ulBufLen = sizeof(WIFI_API_STA_STATS_T)*128;
    stEndUserOmc.pstStaStats = malloc(stEndUserOmc.ulBufLen);
    if(NULL == stEndUserOmc.pstStaStats)
    {
        agent_printf(LOG_ERR, "malloc stEndUserOmc.pstStaStats fail.\n");
        return FALSE;
    }

    for (iLoop = 0; iLoop < total; iLoop++)
    {      
        int iRet;
        memset(stEndUserOmc.pstStaStats,0,stEndUserOmc.ulBufLen);
	  stEndUserOmc.usTotalStaNumber = 0;

         /*获取VAP配置vap name,wifiid,bssid*/
        pal_wifi_get_vap_basic_info(&stVapBasic,iLoop);
        sprintf(acFrequency, "%s", 0 == stVapBasic.uiWifiId?"2.4GHz":"5GHz");

        /*获取WIFI配置*/
  	  pal_wifi_get_wifi_info(&stWifiInfo,stVapBasic.uiWifiId);

        iRet = pal_wifi_vap_get_enduseromc(stVapBasic.acVapName, &stEndUserOmc);

	  agent_printf(LOG_INFO, "iRet %d stEndUserOmc.usTotalStaNumber %d \n",iRet,stEndUserOmc.usTotalStaNumber);


        ulMaxEndUser = stEndUserOmc.usTotalStaNumber;
        /*根据结点号获取指定结点统计信息*/
        for(loop = 0 ; loop < ulMaxEndUser ; loop ++)
            { 
                pstUserInfo[iCount].ulStaID = iCount+1;
                memcpy(pstUserInfo[iCount].aucBssid, stVapBasic.aucBssid, 6);

                memcpy(pstUserInfo[iCount].aucSsid,stVapBasic.acSsid,strlen(stVapBasic.acSsid));

				
                pstUserInfo[iCount].ulVlanId = ulVlanId;
                pstUserInfo[iCount].ulVlanEnable = ulVlanEnable;
                memcpy(pstUserInfo[iCount].aucStaMac,stEndUserOmc.pstStaStats[loop].aucStaMac ,6);

                memset(&stClient,0,sizeof(DHCP_CLIENT_INFO));
                strncpy(stClient.dhcp_client_mac,pstUserInfo[iCount].aucStaMac,MAC_LEN);
                pal_dhcp_get_sta_info(&stClient);
                strncpy(pstUserInfo[iCount].aucStaIp,stClient.dhcp_client_ip,4);
                strncpy(pstUserInfo[iCount].aucStaDevType,stClient.dhcp_client_devinfo,32);

				
                pstUserInfo[iCount].ulConnectTime = stEndUserOmc.pstStaStats[loop].ulStaLiveTime;
                pstUserInfo[iCount].ulStaUplinkOctets = stEndUserOmc.pstStaStats[loop].ullTxDataBytes;
                pstUserInfo[iCount].ulStaDownlinkOctets = stEndUserOmc.pstStaStats[loop].ullRxDataBytes;
                pstUserInfo[iCount].iRssi = stEndUserOmc.pstStaStats[loop].cStaRssi;
                memcpy(pstUserInfo[iCount].aucFrequency, acFrequency, 32);
                pstUserInfo[iCount].ulSnr = stEndUserOmc.pstStaStats[loop].cStaRssi - stEndUserOmc.pstStaStats[loop].cChanNoise;
                fTxRate = stEndUserOmc.pstStaStats[loop].ulTxAvrRate/1000;
                sprintf(pstUserInfo[iCount].acTxRate, "%.1f", fTxRate);
				
                /* 错包率统计，单位为万分之一；
                   大于0而小于万分之一的，显示为万分之一；
                   大于等于万分之一的，则正常显示 */

                ulTotalPkt = stEndUserOmc.pstStaStats[loop].ulRxDataFrame + 
                    stEndUserOmc.pstStaStats[loop].ulRxError;
                fErrRate = (float)stEndUserOmc.pstStaStats[loop].ulRxError/ulTotalPkt;
                if ( (0 < fErrRate) && ((float)1/10000 > fErrRate) )
                {
                    pstUserInfo[iCount].ulErrRate = 1;
                }
                else
                {
                    pstUserInfo[iCount].ulErrRate = fErrRate*10000;
                }


	       /*终端协商接入速率设置--待补充*/
		//pstUserInfo[iCount].uAssoRate=		
                
                iCount++;
                if ( iCount >= 128 )
                {
                *pulUserNum = iCount;
                //free
                if ( NULL != stEndUserOmc.pstStaStats )
                {
                    free(stEndUserOmc.pstStaStats);
                    stEndUserOmc.pstStaStats=NULL;
                }
                return TRUE;
                }
            }
            
        }
    //free
    if ( NULL != stEndUserOmc.pstStaStats )
    {
        free(stEndUserOmc.pstStaStats);
        stEndUserOmc.pstStaStats=NULL;
    }
    *pulUserNum = iCount;
    return TRUE;
   
}
BOOL wlan_add_vap(AP_CONFIG_SSID_ENTRY_T *pstSsidSetup)
{

}
int wlan_del_vap(AP_CONFIG_SSID_ENTRY_T *pstSsidSetup)
{
    
}

BOOL agent_check_wifi_mode(UINT32 ulWifiId, UINT32 ulMode)
{

}
BOOL agent_set_wifi5_ch52_power()
{

}

BOOL agent_check_wifi5_ch100_power()
{
}


BOOL agent_check_static_channel_validity(UINT32 ulWifiId, UINT32 ulStaticChannel)
{
    
}

int wifi_device_channel_config(AP_CONFIG_WIFIDEVICECHANNEL_T *pstData, AP_CONFIG_WIFIDEVICECHANNEL_T *pstDataOld)
{

}

#else
/*****************************************************************************
 Prototype    : wlan_get_wifichannelinfo
 Description  : 获取wifi信道信息
 Input        : AP_CONFIG_WIFIDEVICECHANNEL_ENTRY_T *pstWifiChanInfo
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2012/11/7
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
BOOL wlan_get_wifichannelinfo( AP_CONFIG_WIFIDEVICECHANNEL_T *pstWifiChanInfo )
{    
    char nvram_name[32] = "";
    char nvram_value[32] = "";
    UINT32 ulVapLoop = 0;
    char acNvramName[NVRAM_SIZE] = {0};
    char acRadioId[NVRAM_SIZE];
    char list[128];
    char vaps_list[32];
    char devname[32] = {0};
    int total;

    if (NULL == pstWifiChanInfo)
    {
        agent_printf(LOG_ERROR, "pstWifiChanInfo is NULL");
        return FALSE;
    }

    /* mode */
    sprintf(nvram_name, NVRAM_WIFI"%d_mode", pstWifiChanInfo->ulWifiId);
    nvram_get(nvram_value, nvram_name, NVRAM_LEN);
    pstWifiChanInfo->ulAbgMode = atoi(nvram_value);

    /* band */
    sprintf(nvram_name, NVRAM_WIFI"%d_band", pstWifiChanInfo->ulWifiId);
    nvram_get(nvram_value, nvram_name, NVRAM_LEN);
    pstWifiChanInfo->ulBwMode = atoi(nvram_value);

    /* channel */
    /* static channel */
    sprintf(nvram_name, NVRAM_WIFI"%d_channel", pstWifiChanInfo->ulWifiId);
    nvram_get(nvram_value, nvram_name, NVRAM_LEN);
    pstWifiChanInfo->ulStaicChannel = atoi(nvram_value);
    /* current channel */
    if(NvramResultFailed == nvram_get(list, NVRAM_VAP_LIST, sizeof(list)))
    {
        lprintf(LOG_ERR, "there is no vap for devcie.");
        return FALSE;   
    }
    else
    {
        array_string_to_int(list, vaps_list, &total);
    }

    for (ulVapLoop = 0; ulVapLoop < total; ulVapLoop++)
    {
        sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_WIFI_ID, vaps_list[ulVapLoop]);
        nvram_get(acRadioId, acNvramName, NVRAM_SIZE);
        
        if(pstWifiChanInfo->ulWifiId != atoi(acRadioId))
        {
             continue;
        }
        
        sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_VAP_NAME, vaps_list[ulVapLoop]);
        nvram_get(devname, acNvramName, 32);
        wifi_get_channel(devname,(int *)&pstWifiChanInfo->ulChannelUsing);
        break;
    }

    /* rfnum */
    sprintf(nvram_name, NVRAM_WIFI"%d_rfnum", pstWifiChanInfo->ulWifiId);
    nvram_get(nvram_value, nvram_name, NVRAM_LEN);
    pstWifiChanInfo->ulRFNum = atoi(nvram_value);

    sprintf(nvram_name, NVRAM_WIFI"%d_auto_channel_swtich", pstWifiChanInfo->ulWifiId);
    nvram_get(nvram_value, nvram_name, NVRAM_LEN);
    pstWifiChanInfo->ulChannelState = atoi(nvram_value);

    sprintf(nvram_name, NVRAM_WIFI"%d_auto_channel_cly", pstWifiChanInfo->ulWifiId);
    nvram_get(nvram_value, nvram_name, NVRAM_LEN);
    pstWifiChanInfo->ulAutoChanTime = atoi(nvram_value);

    sprintf(nvram_name, NVRAM_WIFI"%d_auto_channel_list", pstWifiChanInfo->ulWifiId);
    nvram_get(pstWifiChanInfo->acAutoChanList, nvram_name, 256);
    
    return TRUE;
} /* end wlan_get_wifichannelinfo */

/*****************************************************************************
 Prototype    : wlan_get_wifitxpowerinfo
 Description  : 获取卡级功率信息
 Input        : AP_CONFIG_WIFIDEVICEPOWER_ENTRY_T *pstWifiTxPower
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2012/11/7
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
BOOL wlan_get_wifitxpowerinfo( AP_CONFIG_WIFIDEVICEPOWER_T *pstWifiTxPower )
{   
    char nvram_name[32];
    char nvram_value[32];

    sprintf(nvram_name, NVRAM_WIFI"%d_limit_txpower", pstWifiTxPower->ulWifiId);
    nvram_get(nvram_value, nvram_name, NVRAM_LEN);
    pstWifiTxPower->ulLimitTxPower = atoi(nvram_value);

    sprintf(nvram_name, NVRAM_WIFI"%d_current_txpower", pstWifiTxPower->ulWifiId);
    nvram_get(nvram_value, nvram_name, NVRAM_LEN);
    pstWifiTxPower->ulMaxTxPower = atoi(nvram_value);
    pstWifiTxPower->ulTransmitPower = pstWifiTxPower->ulLimitTxPower - pstWifiTxPower->ulMaxTxPower;

    agent_printf(LOG_DEBUG, "the wifi%d dev ulTransmitPower=%d, ulLimitTxPower=%d",
        pstWifiTxPower->ulWifiId, pstWifiTxPower->ulTransmitPower, pstWifiTxPower->ulLimitTxPower);
    
    return TRUE;
}/* end wlan_get_wifitxpowerinfo */

BOOL wlan_get_wifiadvanceinfo( AP_CONFIG_WIFIDEVICEADVANCE_T *pstApWifiAdvance )
{
    char nvram_name[32];
    char nvram_value[32];

    sprintf(nvram_name, NVRAM_WIFI"%d_rssi_mode", pstApWifiAdvance->ulWifiId);
    nvram_get(nvram_value, nvram_name, NVRAM_LEN);
    pstApWifiAdvance->ulRssiMode = atoi(nvram_value);

    sprintf(nvram_name, NVRAM_WIFI"%d_rssi_threshold", pstApWifiAdvance->ulWifiId);
    nvram_get(nvram_value, nvram_name, NVRAM_LEN);
    pstApWifiAdvance->iRssi = atoi(nvram_value);
    
    return TRUE;

}

/* BEGIN: Added by Zhou Shouya, 2013/8/29 */
/* 获取卡级速率配置 */
BOOL wlan_get_wifirateinfo( AP_CONFIG_WIFIDEVICERATE_T *pstApWifiRate )
{
    char nvram_name[32];
    char nvram_value[32];
    char rate_basic[256];
    char rate_11n[256];

    sprintf(nvram_name, NVRAM_WIFI"%d"NVRAM_WIFI_RATE_MODE, pstApWifiRate->ulWifiId);
    nvram_get(nvram_value, nvram_name, sizeof(nvram_value));
    pstApWifiRate->ulRateMode = atoi(nvram_value);

    sprintf(nvram_name, NVRAM_WIFI"%d"NVRAM_WIFI_LEGACY_RATE, pstApWifiRate->ulWifiId);
    nvram_get(rate_basic, nvram_name, 256);
    agent_convert_index_to_rate(0, rate_basic, pstApWifiRate->acTransmitSpeedBasic);

    sprintf(nvram_name, NVRAM_WIFI"%d"NVRAM_WIFI_MCS_RATE, pstApWifiRate->ulWifiId);
    nvram_get(rate_11n, nvram_name, 256);
    agent_convert_index_to_rate(1, rate_11n, pstApWifiRate->acTransmitSpeed11n);
    
    return TRUE;
}
/* END:   Added by Zhou Shouya, 2013/8/29 */

#if 0
/*****************************************************************************
 Prototype    : wlan_get_vapstatinfo
 Description  : 获取vap下面的用户统计信息
 Input        : BSS_AIR_STATS_T *pstBssAirStats
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2012/11/8
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
BOOL wlan_get_vapstatinfo( BSS_AIR_STATS_T *pstBssAirStats )
{
    char acDevName[32]          = {0};
    IOCTL_DATA_T stIoctlData    = {0};
    int iRet  = 0;
    
    sprintf(acDevName, "ap%d_%d", pstBssAirStats->ulWifiID, pstBssAirStats->ulVapID);
    stIoctlData.enIoctlType = IOCTL_GET_BSS_STATS;
    stIoctlData.unData.stBssStats.enStatCmd = BSS_STAT_AIR;
    stIoctlData.unData.stBssStats.unStats.stAirStats.ulWifiID = pstBssAirStats->ulWifiID;
    stIoctlData.unData.stBssStats.unStats.stAirStats.ulVapID = pstBssAirStats->ulVapID;
    iRet = ioctl_common(acDevName, &stIoctlData);
    if (FALSE == iRet)
    {
        agent_printf(LOG_ERROR, "get %s dev BSS_AIR_STATS_T fail", acDevName);
        return FALSE;
    }
    memcpy(pstBssAirStats, &stIoctlData.unData.stBssStats.unStats.stAirStats, sizeof(BSS_AIR_STATS_T));
    agent_printf(LOG_DEBUG, "the %s(SSID:%s) dev: ulRecvFrameNum = %d, ulSendFrameNum = %d",
        acDevName,
        pstBssAirStats->acApSsid,
        pstBssAirStats->ulRecvFrameNum, 
        pstBssAirStats->ulSendFrameNum);
    
    return TRUE;
}/* end wlan_get_vapstatinfo */
#endif

/*****************************************************************************
 Prototype    : wlan_get_vapinfo
 Description  : 获取vap信息
 Input        : AP_CONFIG_VAPBASIC_ENTRY_T *pstVapInfo
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2012/11/8
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
BOOL wlan_get_vapinfo( AP_CONFIG_VAPBASIC_ENTRY_T *pstVapInfo )
{
    char acVapName[32] = {0};
    char acNvramName[NVRAM_SIZE] = {0};
    char acNvramValue[NVRAM_SIZE] = {0};
    char acFlowSwitch[NVRAM_SIZE] = {0};
    char acUploadMaxRate[NVRAM_SIZE] = {0};
    char acDownloadMaxRate[NVRAM_SIZE] = {0};

    if (NULL == pstVapInfo)
    {
        agent_printf(LOG_ERROR, "pstVapInfo is NULL");
        return FALSE;
    }

    sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_VAP_NAME, pstVapInfo->ulNvramId);
    nvram_get(acVapName, acNvramName, NVRAM_SIZE);
    /* 获取ssid名称 */
    sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_SSID_NAME, pstVapInfo->ulNvramId);
    nvram_get(pstVapInfo->acSsid, acNvramName, NVRAM_SIZE);

    /* 设备运行状态 */
    sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_SSID_ENABLED, pstVapInfo->ulNvramId);
    nvram_get(acNvramValue, acNvramName, NVRAM_SIZE);
    pstVapInfo->ulSsidEnabled = atoi(acNvramValue);

    /* vap 广播 */
    sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_SSID_HIDE, pstVapInfo->ulNvramId);
    nvram_get(acNvramValue, acNvramName, NVRAM_SIZE);
    pstVapInfo->ulSsidBroadcast = atoi(acNvramValue);

    /* 最大接入用户数 */
    sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_SSID_MAX_USERS, pstVapInfo->ulNvramId);
    nvram_get(acNvramValue, acNvramName, NVRAM_SIZE);
    pstVapInfo->ulMaxSimultUsers = atoi(acNvramValue);

    /* BEGIN: Added by zoucaihong, 2012/11/20 
    获取终端保活时间*/
    sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_STA_INACT, pstVapInfo->ulNvramId);
    nvram_get(acNvramValue, acNvramName, NVRAM_SIZE);
    pstVapInfo->ulEndUserTime= atoi(acNvramValue);
    /* END: Added by zoucaihong, 2012/11/20 */

    /* 获取vap的Bssid */
    link_encap_mac_get(pstVapInfo->aucBssid, acVapName);

    /* BEGIN: Added by zoucaihong, 2013/1/22 */
    /*获取流控开关配置*/
    sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_SSID_TRAFFIC_TYPE, pstVapInfo->ulNvramId);
    nvram_get(acFlowSwitch, acNvramName, NVRAM_SIZE);
    pstVapInfo->ulFlowSwitch = atoi(acFlowSwitch);
    /*获取VAP上链路最大讯务流量*/
    sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_SSID_MAX_UPLOAD, pstVapInfo->ulNvramId);
    nvram_get(acUploadMaxRate, acNvramName, NVRAM_SIZE);
    pstVapInfo->ulUplinkMaxRate = atoi(acUploadMaxRate);
    /*获取VAP下链路最大讯务流量*/
    sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_SSID_MAX_DOWNLOAD, pstVapInfo->ulNvramId);
    nvram_get(acDownloadMaxRate, acNvramName, NVRAM_SIZE);
    pstVapInfo->ulDownlinkMaxRate = atoi(acDownloadMaxRate);
    /* END: Added by zoucaihong, 2013/1/22 */
    /* BEGIN: Added by zoucaihong, 2013/4/23 */
    /*用户级上行最大带宽*/
    sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_STA_MAX_UPLOAD, pstVapInfo->ulNvramId);
    if ( 0 == nvram_get(acNvramValue, acNvramName, NVRAM_SIZE) )
    {
        pstVapInfo->ulUserUpload = atoi(acNvramValue);
    }
    /*用户级下行最大带宽*/
    sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_STA_MAX_DOWNLOAD, pstVapInfo->ulNvramId);
    if ( 0 == nvram_get(acNvramValue, acNvramName, NVRAM_SIZE) )
    {
        pstVapInfo->ulUserDownload = atoi(acNvramValue);
    }
    /* END: Added by zoucaihong, 2013/4/23 */

    //TODO.. vlan
    sprintf(acNvramName, NVRAM_VAP"%d_port_pvid", pstVapInfo->ulNvramId);
    if(0 == nvram_get(acNvramValue, acNvramName, NVRAM_SIZE))
    {
        pstVapInfo->ulVlanId = atoi(acNvramValue);
    }

    sprintf(acNvramName, NVRAM_VAP"%d_port_vlanEabled", pstVapInfo->ulNvramId);
    if(0 == nvram_get(acNvramValue, acNvramName, NVRAM_SIZE))
    {
        pstVapInfo->ulVlanEnabled = atoi(acNvramValue);
    }
   
    agent_printf(LOG_DEBUG, "the %s dev acSsid:%s, ulSsidEnabled=%u, ulSsidBroadcast=%u, ulMaxSimultUsers=%u, aucBssid="MAC_FMT" ",
        acVapName,
        pstVapInfo->acSsid,
        pstVapInfo->ulSsidEnabled,
        pstVapInfo->ulSsidBroadcast,
        pstVapInfo->ulMaxSimultUsers,
        MAC_ARG(pstVapInfo->aucBssid));
    
    return TRUE;
}/* end wlan_get_vapinfo */

/*****************************************************************************
 Prototype    : wlan_get_vapsecurityinfo
 Description  : 获取VAP安全配置信息
 Input        : AP_CONFIG_VAPSECURITY_ENTRY_T *pstVapSecurityInfo  
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2012/11/15
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL wlan_get_vapsecurityinfo(AP_CONFIG_VAPSECURITY_ENTRY_T *pstVapSecurityInfo)
{
    un_vaps st_vapSecu;
    char acNvramName[NVRAM_SIZE] = {0};

    if (NULL == pstVapSecurityInfo)
    {
        agent_printf(LOG_ERROR, "pstVapSecurityInfo is NULL");
        return FALSE;
    }

    memset(&st_vapSecu, 0, sizeof(un_vaps));
    st_vapSecu.len = sizeof(encrypt);
    st_vapSecu.nvram_id = pstVapSecurityInfo->ulIndex-1;
    /* 获取ssid名称 */
    sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_SSID_NAME, st_vapSecu.nvram_id);
    nvram_get(pstVapSecurityInfo->acSsid, acNvramName, NVRAM_SIZE);
    if(NvramResultFailed == vap_encrypt_nvram_get(&st_vapSecu))
    {
        agent_printf(LOG_ERROR, "get pstVapSecurity Info failed");
        return FALSE;
    }

    pstVapSecurityInfo->ulSecurityMode = st_vapSecu.u.st_en.auth_type;
    switch(st_vapSecu.u.st_en.auth_type)
    {
        case OPEN:
            if(DISABLED == st_vapSecu.u.st_en.u.st_open.type)
            {
                pstVapSecurityInfo->ulAuthenticationType = st_vapSecu.u.st_en.u.st_open.type;
            }
            else
            {
                pstVapSecurityInfo->ulAuthenticationType = st_vapSecu.u.st_en.u.st_open.type;
                if(1==st_vapSecu.u.st_en.u.st_open.st_wep.format)
                {
                    if(3==st_vapSecu.u.st_en.u.st_open.st_wep.len)
                    {
                        pstVapSecurityInfo->ulWepKeyFormat = 1;
                    }
                    else
                    {
                        pstVapSecurityInfo->ulWepKeyFormat = 3;
                    }
                }
                else if(2==st_vapSecu.u.st_en.u.st_open.st_wep.format)
                {
                    if(3==st_vapSecu.u.st_en.u.st_open.st_wep.len)
                    {
                        pstVapSecurityInfo->ulWepKeyFormat = 0;
                    }
                    else
                    {
                        pstVapSecurityInfo->ulWepKeyFormat = 2;
                    }
                }
                memcpy(pstVapSecurityInfo->acWepKey, st_vapSecu.u.st_en.u.st_open.st_wep.key, 28);
            }
            break;
            
        case SHARED:
            /* BEGIN: Added by Zhou Shouya, 2013/8/6   PN:VLAN */
            pstVapSecurityInfo->ulAuthenticationType = WEP;//wep
            /* END:   Added by Zhou Shouya, 2013/8/6   PN:VLAN */
            if(1==st_vapSecu.u.st_en.u.st_shared.st_wep.format)
            {
                if(3==st_vapSecu.u.st_en.u.st_shared.st_wep.len)
                {
                    pstVapSecurityInfo->ulWepKeyFormat = 1;
                }
                else
                {
                    pstVapSecurityInfo->ulWepKeyFormat = 3;
                }
            }
            else if(2==st_vapSecu.u.st_en.u.st_shared.st_wep.format)
            {
                if(3==st_vapSecu.u.st_en.u.st_shared.st_wep.len)
                {
                    pstVapSecurityInfo->ulWepKeyFormat = 0;
                }
                else
                {
                    pstVapSecurityInfo->ulWepKeyFormat = 2;
                }
            }
            memcpy(pstVapSecurityInfo->acWepKey, st_vapSecu.u.st_en.u.st_shared.st_wep.key, 28);
            break;
            
        case WPA_PSK:
        case WPA2_PSK:
        case WPA_PSK_WPA2_PSK:
            /* BEGIN: Added by Zhou Shouya, 2013/8/6   PN:VLAN */
            pstVapSecurityInfo->ulAuthenticationType = DISABLED;//disable
            /* END:   Added by Zhou Shouya, 2013/8/6   PN:VLAN */
            pstVapSecurityInfo->ulEncryptType = st_vapSecu.u.st_en.u.st_wpa_per.type;
            pstVapSecurityInfo->ulWpaKeyRenewal = st_vapSecu.u.st_en.u.st_wpa_per.renew;
            memcpy(pstVapSecurityInfo->acWpaKey, st_vapSecu.u.st_en.u.st_wpa_per.key, 64);
            break;

        case WPA:
        case WPA2:
        case WPA_WPA2:
            /* BEGIN: Added by Zhou Shouya, 2013/8/6   PN:VLAN */
            pstVapSecurityInfo->ulAuthenticationType = DISABLED;//disable
            /* END:   Added by Zhou Shouya, 2013/8/6   PN:VLAN */
            pstVapSecurityInfo->ulEncryptType = st_vapSecu.u.st_en.u.st_wpa_ent.type;
            memcpy(pstVapSecurityInfo->aucRadiusAddr, st_vapSecu.u.st_en.u.st_wpa_ent.auth_ip, 4);
            pstVapSecurityInfo->ulRadiusPort = st_vapSecu.u.st_en.u.st_wpa_ent.auth_port;
            memcpy(pstVapSecurityInfo->acRadiusSecu, st_vapSecu.u.st_en.u.st_wpa_ent.auth_key, 64);

            memcpy(pstVapSecurityInfo->aucAccountAddr, st_vapSecu.u.st_en.u.st_wpa_ent.acct_ip, 4);
            memcpy(pstVapSecurityInfo->acAccountSecu, st_vapSecu.u.st_en.u.st_wpa_ent.acct_key, 64);
            pstVapSecurityInfo->ulAccountPort = st_vapSecu.u.st_en.u.st_wpa_ent.acct_port;
            pstVapSecurityInfo->ulRadiusAgingTime = st_vapSecu.u.st_en.u.st_wpa_ent.send_interval;
            /* BEGIN: Added by Zhou Shouya, 2013/8/8   PN:VLAN */
            pstVapSecurityInfo->ulAccountInterval = st_vapSecu.u.st_en.u.st_wpa_ent.acct_interim;
            /* END:   Added by Zhou Shouya, 2013/8/8   PN:VLAN */
            break;
    }
    
    return TRUE;
}

/*****************************************************************************
 Prototype    : wlan_get_stadevtype
 Description  : 获取终端设备类型
 Input        : UINT8 *pucStaMac       
                UINT32 *pulStaDevType  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/23
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL wlan_get_stadevtype( UINT8 *pucStaMac, UINT8 *pucStaDevType )
{
    FILE *pfData    = NULL;
    CHAR acBuf[128] = {0};
    CHAR *szTemp = NULL;
    int i           = 0;
    UINT32 aulMac[6] = {0};
    UINT32 ulLoopNo = 0;
    UINT32 ulLen = 0;
    UINT8 aucMac[6] = {0};
    CHAR acStaOs[32] = {0};
    CHAR acCompany[32] = {0};
    CHAR *pcPos = NULL;
    CHAR *pcStart = NULL;

    if ( NULL == pucStaMac )
    {
        agent_printf(LOG_ERR, "pucStaMac is NULL.\n");
        return FALSE;
    }

    if ( NULL == pucStaDevType )
    {
        agent_printf(LOG_ERR, "pulStaDevType is NULL.\n");
        return FALSE;
    }

    pfData = fopen("/tmp/sta_system_info", "r");
    if (NULL == pfData)
    {
        agent_printf(LOG_ERROR, "open sta_system_info failed.\n");
        return FALSE;
    }

    while ( fgets(acBuf, sizeof(acBuf)-1, pfData) )
    {
        acBuf[sizeof(acBuf)-1] = '\0';

        /* 去除行尾空格 */
        for (i=strlen(acBuf)-1; i >= 0 && IS_EMPTY_CHAR(acBuf[i]); i--)
        {
            acBuf[i] = '\0';
        }

        /* 跳过空格 */
        for(szTemp = acBuf; IS_EMPTY_CHAR(szTemp[0]); szTemp ++);

        /* 判断是否是空行 */
        if (szTemp[0] == '\0')
        {
            continue;
        }
        
        sscanf(szTemp, MAC_FMT, &aulMac[0], &aulMac[1], &aulMac[2],
            &aulMac[3], &aulMac[4], &aulMac[5]);
        pcPos = strstr( szTemp, "," );
        if ( NULL != pcPos )
        {
            ulLen = strlen(",");
            pcStart = pcPos + ulLen;
            pcPos = NULL;
            pcPos = strstr(pcStart, ",");
            if ( NULL != pcPos )
            {
                memset(acStaOs, 0, sizeof(acStaOs));
                memcpy(acStaOs, pcStart, (pcPos-pcStart));
            }
            pcStart = pcPos + ulLen;
            memcpy(acCompany, pcStart, 32);
        }
        agent_printf(LOG_INFO, "sta os info:%s, company:%s", acStaOs, acCompany);
        for ( ulLoopNo = 0 ; ulLoopNo < 6 ; ulLoopNo++ )
        {
            aucMac[ulLoopNo] = aulMac[ulLoopNo];
        }
        if ( 0 == memcmp(aucMac, pucStaMac, 6) )
        {
            memcpy(pucStaDevType, acStaOs, 32);
            break;
        }
        
    }

    fclose(pfData);
    return TRUE;
}

/* BEGIN: Added by zoucaihong, 2014/6/18 */
BOOL wlan_get_staip(UINT8 *pucMac, UINT8 *pucIp)
{
    CHAR    acCmd[32];
    if ( NULL == pucMac || NULL == pucIp )
    {
        agent_printf(LOG_ERR, "wlan_get_staip pucMac/pucIp is null.\n");
        return FALSE;
    }
    sprintf(acCmd, "user getsta "MAC_FMT, MAC_ARG(pucMac));
    string_ip_tools(cmd_return_str(acCmd), (char *)pucIp);
    return TRUE;
}
/* END: Added by zoucaihong, 2014/6/18 */

#if 0
/*****************************************************************************
 Prototype    : wlan_get_userinfo
 Description  : 获取用户信息
 Input        : AP_STAT_STATION_ENTRY_T *pstUserInfo  
                UINT32 *pulUserNum                    
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/11
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL wlan_get_userinfo(AP_STAT_STATION_ENTRY_T *pstUserInfo, UINT32 *pulUserNum)
{
    unsigned long ulMaxEndUser = 0 ;
    unsigned long ulVlanId = 0;
    unsigned long ulVlanEnable = 0;
    char acFrequency[32] = {0};
    T_wifiState_i tState = {0};
    int loop =0;
    int iLoop = 0;
    int iCount = 0;
    BOOL bRet = 0;
    char devname[32] = {0};
    char acNvramName[NVRAM_SIZE] = {0};
    char acRadioId[NVRAM_SIZE] = {0};
    char acVapId[NVRAM_SIZE] = {0};
    char acVlanId[NVRAM_SIZE] = {0};
    char acVlanEnable[NVRAM_SIZE] = {0};
    char acSsidName[NVRAM_SIZE] = {0};
    char acBssid[6] = {0};
    float fTxRate = 0.0;

    if (NULL == pstUserInfo)
    {
        lprintf(LOG_ERROR, "the pstUserInfo is NULL");
        return FALSE;
    }

    for (iLoop = 0; iLoop < 32; iLoop++)
    {
        sprintf(acNvramName, "vap%d.RadioId", iLoop+1);
        if (0 != nvram_get(acRadioId, acNvramName, 32))
        {
            continue;
        }
        else
        {
            sprintf(acNvramName, "vap%d.WlanId", iLoop+1);
            nvram_get(acVapId, acNvramName, 32);
            sprintf(acNvramName, "vap%d.Ssid", iLoop+1);
            nvram_get(acSsidName, acNvramName, 32);
            /*获取vap所在的vlanid*/
            sprintf(acNvramName, "vap%d.VlanId", iLoop+1);
            if ( 0 != nvram_get(acVlanId, acNvramName, 32))
            {
                ulVlanId = 1;/*default vlan*/
            }
            else
            {
                ulVlanId = (atoi(acVlanId)==0?1:atoi(acVlanId));
            }
            /* BEGIN: Added by zoucaihong, 2013/4/18 */
            /*获取VAP所在的VLAN是否启用*/
            sprintf(acNvramName, "vap%d.isVlan", iLoop+1);
            nvram_get(acVlanEnable, acNvramName, NVRAM_SIZE);
            ulVlanEnable = atoi(acVlanEnable);
            /* END: Added by zoucaihong, 2013/4/18 */
    
            sprintf(devname, "ap%d_%s", atoi(acRadioId) - 1, acVapId);
            lprintf(LOG_INFO, "devname: %s", devname);
            if ( 0 == atoi(acRadioId)- 1 )
            {
                sprintf(acFrequency, "2.4GHz");
            }
            else
            {
                sprintf(acFrequency, "5GHz");
            }
            /* 获取vap的Bssid */
            bRet = get_mac_ioctl(devname, acBssid);
            if (FALSE == bRet)
            {
                agent_printf(LOG_ERROR, "get %s bssid fail", devname);
                return FALSE;
            } 
        }
        
        /*获取结点个数*/
        memset(& tState , 0 , sizeof(tState)) ;
        tState.m = N_endUserTable_I;
        tState.u.endUserEntryOmc.endUserIndex = (unsigned long)(-1) ;

        if(-1 == ieee80211_state_get(devname, &tState)){
        	lprintf(LOG_ERROR, "ieee80211_state_get failed.\n") ;
        	return FALSE;
        }

        ulMaxEndUser = tState.u.endUserEntryOmc.endUserIndex ;
        if(ulMaxEndUser == 0 || ulMaxEndUser>MAX_ENDUSER_ENTRY_I)
            continue;        

        /*根据结点号获取指定结点统计信息*/
        for(loop = 0 ; loop < ulMaxEndUser ; loop ++)
        {
        	memset(&tState , 0 , sizeof(tState)) ;
        	tState.m = N_endUserTable_I;
        	tState.u.endUserEntryOmc.endUserIndex = loop ;
                        
        	if(-1 == ieee80211_state_get(devname , & tState)){
        		agent_printf(LOG_INFO, "get end user(%d) failed.\n" , loop) ;
        		continue ;
        	}
    	    pstUserInfo[iCount].ulStaID = iCount + 1;
            memcpy(pstUserInfo[iCount].aucBssid, acBssid, 6);
    	    memcpy(pstUserInfo[iCount].aucSsid, acSsidName,32);
            pstUserInfo[iCount].ulVlanId = ulVlanId;
            /* BEGIN: Added by zoucaihong, 2013/4/18 */
            /*终端所在VLAN是否启用*/
            pstUserInfo[iCount].ulVlanEnable = ulVlanEnable;
            /* END: Added by zoucaihong, 2013/4/18 */
    	    memcpy(pstUserInfo[iCount].aucStaMac, tState.u.endUserEntryOmc.endUserMac,6);
            /*获取终端设备类型*/
            wlan_get_stadevtype( pstUserInfo[iCount].aucStaMac, pstUserInfo[iCount].aucStaDevType );
    	    pstUserInfo[iCount].ulConnectTime = tState.u.endUserEntryOmc.endUserLiveTime;
    	    pstUserInfo[iCount].ulStaUplinkOctets = tState.u.endUserEntryOmc.ulRecvFromUserDataBytes;
    	    pstUserInfo[iCount].ulStaDownlinkOctets = tState.u.endUserEntryOmc.ulSendToUserDataBytes;
    	    pstUserInfo[iCount].iRssi = tState.u.endUserEntryOmc.iSignalStrength;
    	    memcpy(pstUserInfo[iCount].aucFrequency, acFrequency, 32);
    	    pstUserInfo[iCount].ulSnr = tState.u.endUserEntryOmc.iSignalStrength - tState.u.endUserEntryOmc.iNoise;
            pstUserInfo[iCount].ulTxRate = tState.u.endUserEntryOmc.usEndUserRate;
            fTxRate = (float)tState.u.endUserEntryOmc.usEndUserRate/2;
            if ( 0 == tState.u.endUserEntryOmc.usEndUserRate%2 )
            {
                sprintf(pstUserInfo[iCount].acTxRate, "%.0f", fTxRate);
            }
            else if ( 0 != tState.u.endUserEntryOmc.usEndUserRate%2 )
            {
                sprintf(pstUserInfo[iCount].acTxRate, "%.1f", fTxRate);
            }
            iCount++;
            agent_printf(LOG_INFO, "sta connect time:%u(10ms).", pstUserInfo[iCount].ulConnectTime);
        } 
    }
    
    *pulUserNum = iCount;
    
    return TRUE;
}
#endif

#if 0
/*****************************************************************************
 Prototype    : wlan_get_brmodestainfo
 Description  : 获取桥接模式时对接设备信息
 Input        : AP_STAT_BRMODESTA_ENTRY_T *pstApBrModeStaInfo  
                UINT32 *pulStaNum                              
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/22
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL wlan_get_brmodestainfo( AP_STAT_BRMODESTA_ENTRY_T *pstApBrModeStaInfo, UINT32 *pulStaNum )
{
    UINT32 ulMaxEndUser = 0 ;
    T_wifiState_i tState = {0};
    UINT32 ulLoop =0;
    UINT32 ulCount = 0;
    CHAR acDevName[32] = {0};
    CHAR acNvramName[NVRAM_SIZE] = {0};
    CHAR acRadioId[NVRAM_SIZE] = {0};
    CHAR acVapId[NVRAM_SIZE] = {0};
    CHAR acBrModeVapId[NVRAM_SIZE] = {0};
    float fTxRate = 0.0;

    if ( NULL == pstApBrModeStaInfo )
    {
        agent_printf(LOG_ERROR, "the pstApBrModeStaInfo is NULL.\n");
        return FALSE;
    }

    if ( NULL == pulStaNum )
    {
        agent_printf(LOG_ERR, "the pulStaNum is NULL.\n");
    }

    /*获取开启桥接模式的VAP*/
    sprintf(acNvramName, "bridgeModeVapId");
    if (0 != nvram_get(acBrModeVapId, acNvramName, 32))
    {
        agent_printf(LOG_ERR, "nvram_get acBrModeVapId failed.\n");
        return FALSE;
    }
    /*获取开启桥接模式的VAP所在的卡的ID*/
    sprintf(acNvramName, "vap%d.RadioId", atoi(acBrModeVapId));
    if ( 0!= nvram_get(acRadioId, acNvramName, 32) )
    {
        agent_printf(LOG_ERR, "nvram_get acRadioId failed.\n");
        return FALSE;
    }
    /*获取开启桥接模式的VAP ID*/
    sprintf(acNvramName, "vap%d.WlanId", atoi(acBrModeVapId));
    if ( 0!= nvram_get(acVapId, acNvramName, 32) )
    {
        agent_printf(LOG_ERR, "nvram_get acVapId failed.\n");
        return FALSE;
    }
    sprintf(acDevName, "ap%d_%s", atoi(acRadioId) - 1, acVapId);
    agent_printf(LOG_INFO, "bridge mode devname: %s", acDevName);

    /*获取结点个数*/
    memset(& tState , 0 , sizeof(tState)) ;
    tState.m = N_endUserTable_I;
    tState.u.endUserEntryOmc.endUserIndex = (unsigned long)(-1) ;

    if(-1 == ieee80211_state_get(acDevName, &tState))
    {
    	agent_printf(LOG_ERROR, "ieee80211_state_get failed.\n") ;
    	return FALSE;
    }

    ulMaxEndUser = tState.u.endUserEntryOmc.endUserIndex ;
    if(ulMaxEndUser == 0 || ulMaxEndUser>MAX_ENDUSER_ENTRY_I)
    {
        agent_printf(LOG_ERR, "ulMaxEndUser(%u) is invalid.\n", ulMaxEndUser);
        return FALSE;
    }

    /*根据结点号获取指定结点统计信息*/
    for(ulLoop = 0 ; ulLoop < ulMaxEndUser ; ulLoop ++)
    {
    	memset(&tState , 0 , sizeof(tState)) ;
    	tState.m = N_endUserTable_I;
    	tState.u.endUserEntryOmc.endUserIndex = ulLoop ;
                    
    	if(-1 == ieee80211_state_get(acDevName , & tState))
        {
    		agent_printf(LOG_INFO, "get bridge mode sta(%d) failed.\n" , ulLoop) ;
    		continue ;
    	}
	    pstApBrModeStaInfo[ulCount].ulStaID = ulCount + 1;
        pstApBrModeStaInfo[ulCount].ulMode = 0;/*暂时写死，点对点模式*/
	    memcpy(pstApBrModeStaInfo[ulCount].aucStaMac, tState.u.endUserEntryOmc.endUserMac,6);
	    pstApBrModeStaInfo[ulCount].iRssi = tState.u.endUserEntryOmc.iSignalStrength;
	    pstApBrModeStaInfo[ulCount].ulSnr = tState.u.endUserEntryOmc.iSignalStrength - tState.u.endUserEntryOmc.iNoise;
        pstApBrModeStaInfo[ulCount].ulTxRate = tState.u.endUserEntryOmc.usEndUserRate;
        fTxRate = (float)tState.u.endUserEntryOmc.usEndUserRate/2;
        if ( 0 == tState.u.endUserEntryOmc.usEndUserRate%2 )
        {
            sprintf(pstApBrModeStaInfo[ulCount].acTxRate, "%.0f", fTxRate);
        }
        else if ( 0 != tState.u.endUserEntryOmc.usEndUserRate%2 )
        {
            sprintf(pstApBrModeStaInfo[ulCount].acTxRate, "%.1f", fTxRate);
        }
        ulCount++;
    } 
    *pulStaNum = ulCount;
    
    return TRUE;
}
/*****************************************************************************
 Prototype    : wlan_get_ap24cochainterfinfo
 Description  : AP 2.4G卡同频干扰信息获取
 Input        : AP_STAT_24COCHAINTERFINFO_ENTRY_T *pstAp24CoChaInterfInfo  
                UINT32 *pulNum                                         
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/22
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL wlan_get_ap24cochainterfinfo( AP_STAT_24COCHAINTERFINFO_ENTRY_T *pstAp24CoChaInterfInfo, 
                                           UINT32 *pulNum )
{
    UINT32 ulVapLoop = 0;
    UINT32 ulLoop = 0;
    UINT32 ulCount = 0;
    UINT32 ulWifiNum = 0;
    UINT32 ulWifiId = 0;
    char acNvramName[NVRAM_SIZE] = {0};
    char acWifiNum[NVRAM_SIZE];
    char acRadioId[NVRAM_SIZE];
    char acWlanId[NVRAM_SIZE];
    char acDevName[NVRAM_SIZE];
    IOCTL_DATA_T stIoctlData;

    if ( NULL == pstAp24CoChaInterfInfo )
    {
        agent_printf(LOG_ERR, "wlan_get_ap24cochainterfinfo pstAp24CoChaInterfInfo is null.");
        return FALSE;
    }

    if ( NULL == pulNum )
    {
        agent_printf(LOG_ERR, "wlan_get_ap24cochainterfinfo pulNum is null.");
        return FALSE;
    }

    /* 获取wifi卡的数目, 根据wifiNum */
    sprintf(acNvramName, "wifiNum");
    nvram_get(acWifiNum, acNvramName, NVRAM_SIZE);
    ulWifiNum = atoi(acWifiNum);
    if (0 == ulWifiNum)
    {
        agent_printf(LOG_ERROR, "ulWifiNum is zero");
        return FALSE;
    }

    for (ulVapLoop = 0; ulVapLoop < VAP_MAX_NUM; ulVapLoop++)
    {
        sprintf(acNvramName, "vap%d.RadioId", ulVapLoop);
        if(nvram_get(acRadioId, acNvramName, NVRAM_SIZE))
        {
            continue;
        }
        ulWifiId = atoi(acRadioId) - 1;
        /* 2.4G卡同频干扰信息 */
        if ( ulWifiId == 0 )
        {
            sprintf(acNvramName, "vap%d.WlanId", ulVapLoop);
            if(nvram_get(acWlanId, acNvramName, NVRAM_SIZE))
            {
                continue;
            }
            sprintf(acDevName, "ap%d_%s", ulWifiId, acWlanId);
            memset(&stIoctlData, 0x00, sizeof(stIoctlData));
            stIoctlData.enIoctlType=IOCTL_LOCALAP_GET ;
        	if(FALSE == ioctl_common(acDevName, &stIoctlData))
            {
        		agent_printf(LOG_INFO, "get wifi(0) cochainterf failed.\n") ;
        		continue ;
        	}
            
            for ( ulLoop = 0 ; ulLoop < stIoctlData.unData.stNeighborApList.ulNeighborApNum ; ulLoop++ )
            {
        	    pstAp24CoChaInterfInfo[ulCount].ulIndex = ulCount + 1;
        	    memcpy(pstAp24CoChaInterfInfo[ulCount].aucBssid, 
                    stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].bssid,6);
                strcpy(pstAp24CoChaInterfInfo[ulCount].acSsid, 
                    (char *)stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].ssid.ssid);
                agent_printf(LOG_INFO, "ssidname[%d]:%s",ulLoop,
                    stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].ssid.ssid);
        	    pstAp24CoChaInterfInfo[ulCount].iRssi = stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].rssi;
        	    pstAp24CoChaInterfInfo[ulCount].ulChannel = stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].channel;
                ulCount++;
                /* BEGIN: Added by zoucaihong, 2013/1/28 */
                if ( ulCount >= NEIGHBOR_AP_MAX )
                {
                    *pulNum = ulCount;
                    return TRUE;
                }
                /* END: Added by zoucaihong, 2013/1/28 */
            }
            *pulNum = ulCount;
            return TRUE;
        }
    }
        
    return TRUE;
}

/*****************************************************************************
 Prototype    : wlan_get_ap5cochainterfinfo
 Description  : AP 5G卡同频干扰信息获取
 Input        : AP_STAT_5COCHAINTERFINFO_ENTRY_T *pstAp5CoChaInterfInfo  
                UINT32 *pulNum                                         
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/22
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL wlan_get_ap5cochainterfinfo( AP_STAT_5COCHAINTERFINFO_ENTRY_T *pstAp5CoChaInterfInfo, UINT32 *pulNum )
{
    UINT32 ulVapLoop = 0;
    UINT32 ulLoop = 0;
    UINT32 ulCount = 0;
    UINT32 ulWifiNum = 0;
    UINT32 ulWifiId = 0;
    char acNvramName[NVRAM_SIZE] = {0};
    char acWifiNum[NVRAM_SIZE];
    char acRadioId[NVRAM_SIZE];
    char acWlanId[NVRAM_SIZE];
    char acDevName[NVRAM_SIZE];
    IOCTL_DATA_T stIoctlData;

    if ( NULL == pstAp5CoChaInterfInfo )
    {
        agent_printf(LOG_ERR, "wlan_get_ap5cochainterfinfo pstAp5CoChaInterfInfo is null.");
        return FALSE;
    }

    if ( NULL == pulNum )
    {
        agent_printf(LOG_ERR, "wlan_get_ap5cochainterfinfo pulNum is null.");
        return FALSE;
    }

    /* 获取wifi卡的数目, 根据wifiNum */
    sprintf(acNvramName, "wifiNum");
    nvram_get(acWifiNum, acNvramName, NVRAM_SIZE);
    ulWifiNum = atoi(acWifiNum);
    if (0 == ulWifiNum)
    {
        agent_printf(LOG_ERROR, "ulWifiNum is zero");
        return FALSE;
    }

    for (ulVapLoop = 0; ulVapLoop < VAP_MAX_NUM; ulVapLoop++)
    {
        sprintf(acNvramName, "vap%d.RadioId", ulVapLoop);
        if(nvram_get(acRadioId, acNvramName, NVRAM_SIZE))
        {
            continue;
        }
        ulWifiId = atoi(acRadioId) - 1;
        /* 5G卡同频干扰信息 */
        if ( ulWifiId == 1 )
        {
            sprintf(acNvramName, "vap%d.WlanId", ulVapLoop);
            if(nvram_get(acWlanId, acNvramName, NVRAM_SIZE))
            {
                continue;
            }
            sprintf(acDevName, "ap%d_%s", ulWifiId, acWlanId);
            memset(&stIoctlData, 0x00, sizeof(stIoctlData));
            stIoctlData.enIoctlType=IOCTL_LOCALAP_GET ;
        	if(FALSE == ioctl_common(acDevName, &stIoctlData))
            {
        		agent_printf(LOG_INFO, "get wifi(1) cochainterf failed.\n") ;
        		continue ;
        	}
            
            for ( ulLoop = 0 ; ulLoop < stIoctlData.unData.stNeighborApList.ulNeighborApNum ; ulLoop++ )
            {
        	    pstAp5CoChaInterfInfo[ulCount].ulIndex = ulCount + 1;
        	    memcpy(pstAp5CoChaInterfInfo[ulCount].aucBssid, 
                    stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].bssid,6);
                strcpy(pstAp5CoChaInterfInfo[ulCount].acSsid, 
                    (char *)stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].ssid.ssid);
                agent_printf(LOG_INFO, "ssidname[%d]:%s",ulLoop,
                    stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].ssid.ssid);
        	    pstAp5CoChaInterfInfo[ulCount].iRssi = stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].rssi;
        	    pstAp5CoChaInterfInfo[ulCount].ulChannel = stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].channel;
                ulCount++;
                /* BEGIN: Added by zoucaihong, 2013/1/28 */
                if ( ulCount >= NEIGHBOR_AP_MAX )
                {
                    *pulNum = ulCount;
                    return TRUE;
                }
                /* END: Added by zoucaihong, 2013/1/28 */
            }
            *pulNum = ulCount;
            return TRUE;
        }
    }
        
    return TRUE;
}
/*****************************************************************************
 Prototype    : wlan_get_ap24adjchainterfinfo
 Description  : AP 2.4G卡邻频干扰信息获取
 Input        : AP_STAT_24ADJCHAINTERFINFO_ENTRY_T *pstAp24AdjChaInterfInfo  
                UINT32 *pulNum                                           
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/22
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL wlan_get_ap24adjchainterfinfo( AP_STAT_24ADJCHAINTERFINFO_ENTRY_T *pstAp24AdjChaInterfInfo,
                                           UINT32 *pulNum )
{
    UINT32 ulVapLoop = 0;
    UINT32 ulLoop = 0;
    UINT32 ulCount = 0;
    UINT32 ulWifiNum = 0;
    UINT32 ulWifiId = 0;
    unsigned char ulCurrChan = 0;
    char acNvramName[NVRAM_SIZE] = {0};
    char acWifiNum[NVRAM_SIZE];
    char acRadioId[NVRAM_SIZE];
    char acWlanId[NVRAM_SIZE];
    char acDevName[NVRAM_SIZE];
    IOCTL_DATA_T stIoctlData;

    if ( NULL == pstAp24AdjChaInterfInfo )
    {
        agent_printf(LOG_ERR, "wlan_get_ap24adjchainterfinfo pstAp24AdjChaInterfInfo is null.");
        return FALSE;
    }

    if ( NULL == pulNum )
    {
        agent_printf(LOG_ERR, "wlan_get_ap24adjchainterfinfo pulNum is null.");
        return FALSE;
    }

    /* 获取wifi卡的数目, 根据wifiNum */
    sprintf(acNvramName, "wifiNum");
    nvram_get(acWifiNum, acNvramName, NVRAM_SIZE);
    ulWifiNum = atoi(acWifiNum);
    if (0 == ulWifiNum)
    {
        agent_printf(LOG_ERROR, "ulWifiNum is zero");
        return FALSE;
    }

    for (ulVapLoop = 0; ulVapLoop < VAP_MAX_NUM; ulVapLoop++)
    {
        sprintf(acNvramName, "vap%d.RadioId", ulVapLoop);
        if(nvram_get(acRadioId, acNvramName, NVRAM_SIZE))
        {
            continue;
        }
        ulWifiId = atoi(acRadioId) - 1;
        /* 2.4G卡邻频干扰信息 */
        if ( ulWifiId == 0 )
        {
            sprintf(acNvramName, "vap%d.WlanId", ulVapLoop);
            if(nvram_get(acWlanId, acNvramName, NVRAM_SIZE))
            {
                continue;
            }
            sprintf(acDevName, "ap%d_%s", ulWifiId, acWlanId);
            memset(&stIoctlData, 0x00, sizeof(stIoctlData));
            /* 获取当前信道值 */
            stIoctlData.enIoctlType=IOCTL_GET_CHANNEL_NO;
            if(FALSE == ioctl_common(acDevName, &stIoctlData))
            {
                agent_printf(LOG_INFO, "get channel failed");
            }
            ulCurrChan=stIoctlData.unData.ucChannNo;

            memset(&stIoctlData, 0x00, sizeof(stIoctlData));
            stIoctlData.enIoctlType=IOCTL_NEIGHBORAP_GET ;
        	if(FALSE == ioctl_common(acDevName, &stIoctlData))
            {
        		agent_printf(LOG_INFO, "get wifi(0) adjchainterf failed.\n") ;
        		continue ;
        	}
            
            for ( ulLoop = 0 ; ulLoop < stIoctlData.unData.stNeighborApList.ulNeighborApNum ; ulLoop++ )
            {
                if(stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].channel==ulCurrChan)
                {
                    continue;
                }
            
        	    pstAp24AdjChaInterfInfo[ulCount].ulIndex = ulCount + 1;
        	    memcpy(pstAp24AdjChaInterfInfo[ulCount].aucBssid, 
                    stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].bssid,6);
                strcpy(pstAp24AdjChaInterfInfo[ulCount].acSsid, 
                    (char *)stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].ssid.ssid);
                agent_printf(LOG_INFO, "ssidname[%d]:%s",ulLoop,
                    stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].ssid.ssid);
        	    pstAp24AdjChaInterfInfo[ulCount].iRssi = stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].rssi;
        	    pstAp24AdjChaInterfInfo[ulCount].ulChannel = stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].channel;
                ulCount++;
                /* BEGIN: Added by zoucaihong, 2013/1/28 */
                if ( ulCount >= NEIGHBOR_AP_MAX )
                {
                    *pulNum = ulCount;
                    return TRUE;
                }
                /* END: Added by zoucaihong, 2013/1/28 */
            }
            *pulNum = ulCount;
            return TRUE;
        }
    }
        
    return TRUE;
}

/*****************************************************************************
 Prototype    : wlan_get_ap5adjchainterfinfo
 Description  : AP 5G卡邻频干扰信息获取
 Input        : AP_STAT_5ADJCHAINTERFINFO_ENTRY_T *pstAp5AdjChaInterfInfo  
                UINT32 *pulNum                                           
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/22
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL wlan_get_ap5adjchainterfinfo( AP_STAT_5ADJCHAINTERFINFO_ENTRY_T *pstAp5AdjChaInterfInfo,
                                           UINT32 *pulNum )
{
    UINT32 ulVapLoop = 0;
    UINT32 ulLoop = 0;
    UINT32 ulCount = 0;
    UINT32 ulWifiNum = 0;
    UINT32 ulWifiId = 0;
	unsigned char ulCurrChan = 0;
    char acNvramName[NVRAM_SIZE] = {0};
    char acWifiNum[NVRAM_SIZE];
    char acRadioId[NVRAM_SIZE];
    char acWlanId[NVRAM_SIZE];
    char acDevName[NVRAM_SIZE];
    IOCTL_DATA_T stIoctlData;

    if ( NULL == pstAp5AdjChaInterfInfo )
    {
        agent_printf(LOG_ERR, "wlan_get_ap5adjchainterfinfo pstAp5AdjChaInterfInfo is null.");
        return FALSE;
    }

    if ( NULL == pulNum )
    {
        agent_printf(LOG_ERR, "wlan_get_ap5adjchainterfinfo pulNum is null.");
        return FALSE;
    }

    /* 获取wifi卡的数目, 根据wifiNum */
    sprintf(acNvramName, "wifiNum");
    nvram_get(acWifiNum, acNvramName, NVRAM_SIZE);
    ulWifiNum = atoi(acWifiNum);
    if (0 == ulWifiNum)
    {
        agent_printf(LOG_ERROR, "ulWifiNum is zero");
        return FALSE;
    }

    for (ulVapLoop = 0; ulVapLoop < VAP_MAX_NUM; ulVapLoop++)
    {
        sprintf(acNvramName, "vap%d.RadioId", ulVapLoop);
        if(nvram_get(acRadioId, acNvramName, NVRAM_SIZE))
        {
            continue;
        }
        ulWifiId = atoi(acRadioId) - 1;
        /* 5G卡邻频干扰信息 */
        if ( ulWifiId == 1 )
        {
            sprintf(acNvramName, "vap%d.WlanId", ulVapLoop);
            if(nvram_get(acWlanId, acNvramName, NVRAM_SIZE))
            {
                continue;
            }
            sprintf(acDevName, "ap%d_%s", ulWifiId, acWlanId);
            memset(&stIoctlData, 0x00, sizeof(stIoctlData));
			/* 获取当前信道值 */
            stIoctlData.enIoctlType=IOCTL_GET_CHANNEL_NO;
            if(FALSE == ioctl_common(acDevName, &stIoctlData))
            {
                agent_printf(LOG_INFO, "get channel failed");
            }
            ulCurrChan=stIoctlData.unData.ucChannNo;

            memset(&stIoctlData, 0x00, sizeof(stIoctlData));
            stIoctlData.enIoctlType=IOCTL_NEIGHBORAP_GET ;
        	if(FALSE == ioctl_common(acDevName, &stIoctlData))
            {
        		agent_printf(LOG_INFO, "get wifi(1) adjchainterf failed.\n") ;
        		continue ;
        	}
            
            for ( ulLoop = 0 ; ulLoop < stIoctlData.unData.stNeighborApList.ulNeighborApNum ; ulLoop++ )
            {
			
				if(stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].channel==ulCurrChan)
                {
                    continue;
                }
				
        	    pstAp5AdjChaInterfInfo[ulCount].ulIndex = ulCount + 1;
        	    memcpy(pstAp5AdjChaInterfInfo[ulCount].aucBssid, 
                    stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].bssid,6);
                strcpy(pstAp5AdjChaInterfInfo[ulCount].acSsid, 
                    (char *)stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].ssid.ssid);
                agent_printf(LOG_INFO, "ssidname[%d]:%s",ulLoop,
                    stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].ssid.ssid);
        	    pstAp5AdjChaInterfInfo[ulCount].iRssi = stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].rssi;
        	    pstAp5AdjChaInterfInfo[ulCount].ulChannel = stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].channel;
                ulCount++;
                /* BEGIN: Added by zoucaihong, 2013/1/28 */
                if ( ulCount >= NEIGHBOR_AP_MAX )
                {
                    *pulNum = ulCount;
                    return TRUE;
                }
                /* END: Added by zoucaihong, 2013/1/28 */
            }
            *pulNum = ulCount;
            return TRUE;
        }
    }
        
    return TRUE;
}
/*****************************************************************************
 Prototype    : wlan_get_ap24wlandevinterfinfo
 Description  : 获取AP 2.4G卡WLAN设备干扰信息
 Input        : AP_STAT_24WLANDEVINTERFINFO_ENTRY_T *pstAp24WlanDevInterfInfo  
                UINT32 *pulNum                                             
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/23
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL wlan_get_ap24wlandevinterfinfo( AP_STAT_24WLANDEVINTERFINFO_ENTRY_T *pstAp24WlanDevInterfInfo, 
                                             UINT32 *pulNum )
{
    UINT32 ulVapLoop = 0;
    UINT32 ulLoop = 0;
    UINT32 ulCount = 0;
    UINT32 ulRecNum = 0;    
    UINT32 ulWifiNum = 0;
    UINT32 ulWifiId = 0;
    char acNvramName[NVRAM_SIZE] = {0};
    char acWifiNum[NVRAM_SIZE];
    char acRadioId[NVRAM_SIZE];
    char acWlanId[NVRAM_SIZE];
    char acDevName[NVRAM_SIZE];
    IOCTL_DATA_T stIoctlData;

    if ( NULL == pstAp24WlanDevInterfInfo )
    {
        agent_printf(LOG_ERR, "wlan_get_ap24wlandevinterfinfo pstAp24WlanDevInterfInfo is null.");
        return FALSE;
    }

    if ( NULL == pulNum )
    {
        agent_printf(LOG_ERR, "wlan_get_ap24wlandevinterfinfo pulNum is null.");
        return FALSE;
    }

    /* 获取wifi卡的数目, 根据wifiNum */
    sprintf(acNvramName, "wifiNum");
    nvram_get(acWifiNum, acNvramName, NVRAM_SIZE);
    ulWifiNum = atoi(acWifiNum);
    if (0 == ulWifiNum)
    {
        agent_printf(LOG_ERROR, "ulWifiNum is zero");
        return FALSE;
    }

    for (ulVapLoop = 0; ulVapLoop < VAP_MAX_NUM; ulVapLoop++)
    {
        sprintf(acNvramName, "vap%d.RadioId", ulVapLoop);
        if(nvram_get(acRadioId, acNvramName, NVRAM_SIZE))
        {
            continue;
        }
        ulWifiId = atoi(acRadioId) - 1;
        /* 2.4G卡WLAN设备干扰信息 */
        if ( ulWifiId == 0 )
        {
            sprintf(acNvramName, "vap%d.WlanId", ulVapLoop);
            if(nvram_get(acWlanId, acNvramName, NVRAM_SIZE))
            {
                continue;
            }
            sprintf(acDevName, "ap%d_%s", ulWifiId, acWlanId);
            
            /*获取AP 2.4G卡同频干扰信息*/
            memset(&stIoctlData, 0x00, sizeof(stIoctlData));
            stIoctlData.enIoctlType=IOCTL_NEIGHBORAP_GET;
        	if(FALSE == ioctl_common(acDevName, &stIoctlData))
            {
        		agent_printf(LOG_INFO, "get wifi(0) cochainterf failed.\n") ;
        		continue ;
        	}
            
            for ( ulLoop = 0 ; ulLoop < stIoctlData.unData.stNeighborApList.ulNeighborApNum ; ulLoop++ )
            {
        	    pstAp24WlanDevInterfInfo[ulCount].ulIndex = ulCount + 1;
        	    memcpy(pstAp24WlanDevInterfInfo[ulCount].aucBssid, 
                    stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].bssid,6);
                strcpy(pstAp24WlanDevInterfInfo[ulCount].acSsid, 
                    (char *)stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].ssid.ssid);
        	    pstAp24WlanDevInterfInfo[ulCount].iRssi = stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].rssi;
        	    pstAp24WlanDevInterfInfo[ulCount].ulChannel = stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].channel;
                ulCount++;
                /* BEGIN: Added by zoucaihong, 2013/1/28 */
                if ( ulCount >= NEIGHBOR_AP_MAX )
                {
                    break;
                }
                /* END: Added by zoucaihong, 2013/1/28 */
            }
            ulRecNum = ulCount;
            break;
        }
    }
    *pulNum = ulRecNum;

    return TRUE;
}

/*****************************************************************************
 Prototype    : wlan_get_ap5wlandevinterfinfo
 Description  : 获取AP 5G卡WLAN设备干扰信息
 Input        : AP_STAT_5WLANDEVINTERFINFO_ENTRY_T *pstAp5WlanDevInterfInfo  
                UINT32 *pulNum                                             
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/23
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL wlan_get_ap5wlandevinterfinfo( AP_STAT_5WLANDEVINTERFINFO_ENTRY_T *pstAp5WlanDevInterfInfo, 
                                             UINT32 *pulNum )
{
    UINT32 ulVapLoop = 0;
    UINT32 ulLoop = 0;
    UINT32 ulCount = 0;
    UINT32 ulRecNum = 0;
    UINT32 ulWifiNum = 0;
    UINT32 ulWifiId = 0;
    char acNvramName[NVRAM_SIZE] = {0};
    char acWifiNum[NVRAM_SIZE];
    char acRadioId[NVRAM_SIZE];
    char acWlanId[NVRAM_SIZE];
    char acDevName[NVRAM_SIZE];
    IOCTL_DATA_T stIoctlData;

    if ( NULL == pstAp5WlanDevInterfInfo )
    {
        agent_printf(LOG_ERR, "wlan_get_ap5wlandevinterfinfo pstAp5WlanDevInterfInfo is null.");
        return FALSE;
    }

    if ( NULL == pulNum )
    {
        agent_printf(LOG_ERR, "wlan_get_ap5wlandevinterfinfo pulNum is null.");
        return FALSE;
    }

    /* 获取wifi卡的数目, 根据wifiNum */
    sprintf(acNvramName, "wifiNum");
    nvram_get(acWifiNum, acNvramName, NVRAM_SIZE);
    ulWifiNum = atoi(acWifiNum);
    if (0 == ulWifiNum)
    {
        agent_printf(LOG_ERROR, "ulWifiNum is zero");
        return FALSE;
    }

    for (ulVapLoop = 0; ulVapLoop < VAP_MAX_NUM; ulVapLoop++)
    {
        sprintf(acNvramName, "vap%d.RadioId", ulVapLoop);
        if(nvram_get(acRadioId, acNvramName, NVRAM_SIZE))
        {
            continue;
        }
        ulWifiId = atoi(acRadioId) - 1;
        /* 5G卡WLAN设备干扰信息 */
        if ( ulWifiId == 1 )
        {
            sprintf(acNvramName, "vap%d.WlanId", ulVapLoop);
            if(nvram_get(acWlanId, acNvramName, NVRAM_SIZE))
            {
                continue;
            }
            sprintf(acDevName, "ap%d_%s", ulWifiId, acWlanId);
            
            /*获取AP 5G卡同频干扰信息*/
            memset(&stIoctlData, 0x00, sizeof(stIoctlData));
            stIoctlData.enIoctlType=IOCTL_NEIGHBORAP_GET;
        	if(FALSE == ioctl_common(acDevName, &stIoctlData))
            {
        		agent_printf(LOG_INFO, "get wifi(0) cochainterf failed.\n") ;
        		continue ;
        	}
            
            for ( ulLoop = 0 ; ulLoop < stIoctlData.unData.stNeighborApList.ulNeighborApNum ; ulLoop++ )
            {
        	    pstAp5WlanDevInterfInfo[ulCount].ulIndex = ulCount + 1;
        	    memcpy(pstAp5WlanDevInterfInfo[ulCount].aucBssid, 
                    stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].bssid,6);
                strcpy(pstAp5WlanDevInterfInfo[ulCount].acSsid, 
                    (char *)stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].ssid.ssid);
        	    pstAp5WlanDevInterfInfo[ulCount].iRssi = stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].rssi;
        	    pstAp5WlanDevInterfInfo[ulCount].ulChannel = stIoctlData.unData.stNeighborApList.stNeighborApList[ulLoop].channel;
                ulCount++;
                /* BEGIN: Added by zoucaihong, 2013/1/28 */
                if ( ulCount >= NEIGHBOR_AP_MAX )
                {
                    break;
                }
                /* END: Added by zoucaihong, 2013/1/28 */
            }
            ulRecNum = ulCount;
            break;
        }
    }
    *pulNum = ulRecNum;

    return TRUE;
}
#endif
#if 0
/*****************************************************************************
 Prototype    : get_rouge_ap
 Description  : 
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/5/14
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
int get_rouge_ap(AP_STAT_24ROGUEAPINFO_ENTRY_T *pstAp24RogueApInfo, char *devname)
{
    int index;
    int loop;
    WIFI_API_AP_LIST_T st_list;

    index = 0;
    memset(&st_list, 0, sizeof(WIFI_API_AP_LIST_T));
    if(0 == wifi_api_vap_get_neighborap(devname, &st_list))
    {
        for(loop = 0; loop < st_list.ulNeighborApNum; loop++)
        {
            pstAp24RogueApInfo[index].iRssi = st_list.stNeighborApList[loop].rssi;
            pstAp24RogueApInfo[index].ulChannel = st_list.stNeighborApList[loop].channel;
            memcpy(pstAp24RogueApInfo[index].acSsid, st_list.stNeighborApList[loop].ssid.ssid, 32);
            memcpy(pstAp24RogueApInfo[index].aucBssid, st_list.stNeighborApList[loop].bssid, 6);
            index++;
        }
    }
    return index;
}

/* BEGIN: Added by zoucaihong, 2013/5/10 */
BOOL wlan_get_ap24rogueapinfo( AP_STAT_24ROGUEAPINFO_ENTRY_T *pstAp24RogueApInfo,
                                           UINT32 *pulNum )
{
    AP_STAT_24ROGUEAPINFO_ENTRY_T stRouge[TABLE_MAXRECNUM_ROGUEAP];
    struct leg_ap leg[16];
    char tmp[32];
    int index, index_leg, count;
    int loop, loop_j, flag;

    index = 0;
    memset(stRouge, 0, sizeof(AP_STAT_24ROGUEAPINFO_ENTRY_T)*TABLE_MAXRECNUM_ROGUEAP);
    index = get_rouge_ap(stRouge, "ap0_0");
    index_leg = leg_get_all(leg); 
    count = 0;
    for(loop = 0; loop<index; loop++)
    {
        flag = 0;
        for(loop_j = 0; loop_j < index_leg; loop_j++)
        {
            if(0==memcmp(stRouge[loop].acSsid, leg[loop_j].ssid, strlen(leg[loop_j].ssid)))
            {
                sprintf(tmp, "%02X:%02X:%02X", 
                    stRouge[loop].aucBssid[0],
                    stRouge[loop].aucBssid[1],
                    stRouge[loop].aucBssid[2]);
                if(0 == memcmp(tmp, leg[loop_j].bssid, strlen("FF-FF-FF")))
                {
                    flag = 1;
                    break;
                }
                sprintf(tmp, "%02x:%02x:%02x", 
                    stRouge[loop].aucBssid[0],
                    stRouge[loop].aucBssid[1],
                    stRouge[loop].aucBssid[2]);
                if(0 == memcmp(tmp, leg[loop_j].bssid, strlen("FF-FF-FF")))
                {
                    flag = 1;
                    break;
                }
                sprintf(tmp, "%02X-%02X-%02X", 
                    stRouge[loop].aucBssid[0],
                    stRouge[loop].aucBssid[1],
                    stRouge[loop].aucBssid[2]);
                if(0 == memcmp(tmp, leg[loop_j].bssid, strlen("FF-FF-FF")))
                {
                    flag = 1;
                    break;
                }
                sprintf(tmp, "%02x-%02x-%02x", 
                    stRouge[loop].aucBssid[0],
                    stRouge[loop].aucBssid[1],
                    stRouge[loop].aucBssid[2]);
                if(0 == memcmp(tmp, leg[loop_j].bssid, strlen("FF-FF-FF")))
                {
                    flag = 1;
                    break;
                }
            }
        }
        if(0 == flag)
        {
            pstAp24RogueApInfo[count].ulIndex = count+1;
            pstAp24RogueApInfo[count].iRssi = stRouge[loop].iRssi;
            pstAp24RogueApInfo[count].ulChannel = stRouge[loop].ulChannel;
            memcpy(&pstAp24RogueApInfo[count].acSsid, stRouge[loop].acSsid, 32);
            memcpy(&pstAp24RogueApInfo[count].aucBssid, stRouge[loop].aucBssid, 6);
            count++;
        }
    }
    (*pulNum)=count;
    return TRUE;
}

BOOL wlan_get_ap5rogueapinfo( AP_STAT_5ROGUEAPINFO_ENTRY_T *pstAp5RogueApInfo,
                                           UINT32 *pulNum )
{
    AP_STAT_5ROGUEAPINFO_ENTRY_T stinfo[TABLE_MAXRECNUM_ROGUEAP];
    struct leg_ap leg[16];
    char tmp[32];
    int index, index_leg, count;
    int loop, loop_j, flag;

    memset(stinfo, 0, sizeof(AP_STAT_5ROGUEAPINFO_ENTRY_T)*TABLE_MAXRECNUM_ROGUEAP);
    index = 0;
    index = get_rouge_ap(stinfo, "ap1_0");
    index_leg = leg_get_all(leg); 
    count=0;
    for(loop = 0; loop<index; loop++)
    {
        flag = 0;
        for(loop_j = 0; loop_j < index_leg; loop_j++)
        {
            agent_printf(LOG_ERR, "acSsid %s, ssid %s",
                stinfo[loop].acSsid, leg[loop_j].ssid);
            if(0==memcmp(stinfo[loop].acSsid, leg[loop_j].ssid, strlen(leg[loop_j].ssid)))
            {
                sprintf(tmp, "%02X-%02X-%02X", 
                    stinfo[loop].aucBssid[0],
                    stinfo[loop].aucBssid[1],
                    stinfo[loop].aucBssid[2]);
                agent_printf(LOG_ERR, "aucBssid: %s, bssid: %s", tmp, leg[loop_j].bssid);
                if(0 == memcmp(tmp, leg[loop_j].bssid, strlen("FF-FF-FF")))
                {
                    flag = 1;
                    break;
                }
                sprintf(tmp, "%02x-%02x-%02x", 
                    stinfo[loop].aucBssid[0],
                    stinfo[loop].aucBssid[1],
                    stinfo[loop].aucBssid[2]);
                if(0 == memcmp(tmp, leg[loop_j].bssid, strlen("FF-FF-FF")))
                {
                    flag = 1;
                    break;
                }
            }
        }

        if(0 == flag)
        {
            pstAp5RogueApInfo[count].ulIndex = count+1;
            pstAp5RogueApInfo[count].iRssi = stinfo[loop].iRssi;
            pstAp5RogueApInfo[count].ulChannel = stinfo[loop].ulChannel;
            memcpy(&pstAp5RogueApInfo[count].acSsid, stinfo[loop].acSsid, 32);
            memcpy(&pstAp5RogueApInfo[count].aucBssid, stinfo[loop].aucBssid, 6);
            count++;
        }
    }    
    (*pulNum) = count;
    return TRUE;
}

/* END: Added by zoucaihong, 2013/5/10 */
/*****************************************************************************
 Prototype    : wlan_get_apnonwlandevinterfinfo
 Description  : 获取非WLAN设备干扰信息
 Input        : AP_STAT_NONWLANDEVINTERFINFO_ENTRY_T *pstApNonWlanDevInterfInfo  
                UINT32 *pulNum                                                   
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/23
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL wlan_get_apnonwlandevinterfinfo( AP_STAT_NONWLANDEVINTERFINFO_ENTRY_T *pstApNonWlanDevInterfInfo, UINT32 *pulNum )
{
    char acNvramName[32];
    char acNoise[32];
    char acChannel[32];
    int iLoop;
    int iNum=0;
    int index;

    index = 1;
    for(iLoop=0; iLoop<2; iLoop++)
    {
        sprintf(acNvramName, "no_wifi%d.Noise", iLoop);
        if ( nvram_get(acNoise, acNvramName, 32) )
        {
            continue;
        }
        pstApNonWlanDevInterfInfo->iNoise=atoi(acNoise);
        
        sprintf(acNvramName, "no_wifi%d.Channle", iLoop);
        if ( nvram_get(acChannel, acNvramName, 32) )
        {
            continue;
        }
        pstApNonWlanDevInterfInfo->ulChannel=atoi(acChannel);
        
        sprintf(acNvramName, "no_wifi%d.Date", iLoop);
        if( nvram_get(pstApNonWlanDevInterfInfo->acTime, acNvramName, 32) )
        {
            continue;
        }
        pstApNonWlanDevInterfInfo->ulIndex=index++;
        pstApNonWlanDevInterfInfo->ulWifiId=iLoop;
        pstApNonWlanDevInterfInfo++;
        iNum++;
    }
    *pulNum=iNum;
    return TRUE;
}
#endif
/*****************************************************************************
 Prototype    : wlan_get_userinfo
 Description  : 获取用户信息
 Input        : AP_STAT_STATION_ENTRY_T *pstUserInfo  
                UINT32 *pulUserNum                    
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/11
    Author       : zoucaihong
    Modification : Created function
  2.Date         : 2013/7/25
    Author       : mayuhua
    Modification : Modified function

*****************************************************************************/
BOOL wlan_get_userinfo(AP_STAT_STATION_ENTRY_T *pstUserInfo, UINT32 *pulUserNum)
{
    unsigned long ulMaxEndUser = 0 ;
    unsigned long ulVlanId = 0;
    unsigned long ulVlanEnable = 0;
    char acFrequency[32] = {0};
    char devname[32] = {0};
    char acNvramName[NVRAM_SIZE] = {0};
    char acNvramValue[NVRAM_SIZE] = {0};
    char list[128];
    char vaps_list[32];
    int total;
    int loop =0;
    int iLoop = 0;
    int iCount = 0;
    /* BEGIN: Added by Zhou Shouya, 2013/8/2   PN:VLAN */
    char acVlanId[32] = {0};
    char acVlanEnable[32] = {0};
    UINT8 aucBssid[6] = {0};
    float fTxRate = 0.0;
    float fErrRate = 0.0;
    unsigned long ulTotalPkt = 0;
    /* END:   Added by Zhou Shouya, 2013/8/2   PN:VLAN */
    char vap_shortgi[32];
    char vap_band[32];
    char vap_chainmask[32];

    if (NULL == pstUserInfo)
    {
        lprintf(LOG_ERROR, "the pstUserInfo is NULL");
        return FALSE;
    }
    if ( NULL == pulUserNum)
    {
        agent_printf(LOG_ERR, "the pulUserNum is NULL.\n");
        return FALSE;
    }
    if(NvramResultFailed == nvram_get(list, NVRAM_VAP_LIST, sizeof(list)))
    {
        lprintf(LOG_ERR, "there is no vap for devcie.");
        return FALSE;   
    }
    else
    {
        array_string_to_int(list, vaps_list, &total);
    }

    WIFI_API_ENDUSER_STATS_T   stEndUserOmc;
    stEndUserOmc.ulBufLen = sizeof(WIFI_API_STA_STATS_T)*128;
    stEndUserOmc.pstStaStats = malloc(stEndUserOmc.ulBufLen);
    if(NULL == stEndUserOmc.pstStaStats)
    {
        agent_printf(LOG_ERR, "malloc stEndUserOmc.pstStaStats fail.\n");
        return FALSE;
    }

    for (iLoop = 0; iLoop < total; iLoop++)
    {      
        int iRet;
        memset(stEndUserOmc.pstStaStats,0,stEndUserOmc.ulBufLen);
        sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_VAP_NAME, vaps_list[iLoop]);
        nvram_get(devname, acNvramName, 32);
        sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_WIFI_ID, vaps_list[iLoop]);
        nvram_get(acNvramValue, acNvramName, NVRAM_SIZE);
        sprintf(acFrequency, "%s", 0 == atoi(acNvramValue)?"2.4GHz":"5GHz");
        sprintf(acNvramName, NVRAM_WIFI"%d_shortgi", atoi(acNvramValue));
        nvram_get(vap_shortgi, acNvramName, NVRAM_SIZE);
        sprintf(acNvramName, NVRAM_WIFI"%d_band", atoi(acNvramValue));
        nvram_get(vap_band, acNvramName, NVRAM_SIZE);
        sprintf(acNvramName, NVRAM_WIFI"%d_rfnum", atoi(acNvramValue));
        nvram_get(vap_chainmask, acNvramName, NVRAM_SIZE);
        /* 获取VLAN ID */
        sprintf(acNvramName, NVRAM_VAP"%d_port_pvid", vaps_list[iLoop]);
        nvram_get(acVlanId, acNvramName, 32);
        ulVlanId = atoi(acVlanId);
        /* 获取VLAN是否启用 */
        sprintf(acNvramName, NVRAM_VAP"%d_port_vlanEabled", vaps_list[iLoop]);
        nvram_get(acVlanEnable, acNvramName, 32);
        ulVlanEnable = atoi(acVlanEnable);
        /* 获取vap的Bssid */
        link_encap_mac_get(aucBssid, devname);

        iRet = wifi_api_vap_get_enduseromc(devname, &stEndUserOmc);
        if(iRet != 0)
        {
            continue;
        }
        else
        {
            ulMaxEndUser = stEndUserOmc.usTotalStaNumber;
            /*根据结点号获取指定结点统计信息*/
            for(loop = 0 ; loop < ulMaxEndUser ; loop ++)
                { 
                    pstUserInfo[iCount].ulStaID = iCount+1;
                    /* BEGIN: Modified by Zhou Shouya, 2013/8/2   PN:VLAN */
                    #if 0
                    sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_BSSID, vaps_list[iLoop]);
                    nvram_get(acNvramValue, acNvramName, NVRAM_SIZE);
                    string_mac_tools(acNvramValue, pstUserInfo[iCount].aucBssid);
                    #endif
                    memcpy(pstUserInfo[iCount].aucBssid, aucBssid, 6);
                    /* END:   Modified by Zhou Shouya, 2013/8/2   PN:VLAN */
                    sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_SSID_NAME, vaps_list[iLoop]);
                    nvram_get((char *)pstUserInfo[iCount].aucSsid, acNvramName, NVRAM_SIZE);
                    pstUserInfo[iCount].ulVlanId = ulVlanId;
                    pstUserInfo[iCount].ulVlanEnable = ulVlanEnable;
                    memcpy(pstUserInfo[iCount].aucStaMac,stEndUserOmc.pstStaStats[loop].aucStaMac ,6);
                    /* BEGIN: Added by zoucaihong, 2014/6/18 */
                    wlan_get_staip(pstUserInfo[iCount].aucStaMac, pstUserInfo[iCount].aucStaIp);
                    /* END: Added by zoucaihong, 2014/6/18 */
                    wlan_get_stadevtype( pstUserInfo[iCount].aucStaMac, pstUserInfo[iCount].aucStaDevType );
                    pstUserInfo[iCount].ulConnectTime = stEndUserOmc.pstStaStats[loop].ulStaLiveTime;
                    pstUserInfo[iCount].ulStaUplinkOctets = stEndUserOmc.pstStaStats[loop].ullTxDataBytes;
                    pstUserInfo[iCount].ulStaDownlinkOctets = stEndUserOmc.pstStaStats[loop].ullRxDataBytes;
                    pstUserInfo[iCount].iRssi = stEndUserOmc.pstStaStats[loop].cStaRssi;
                    memcpy(pstUserInfo[iCount].aucFrequency, acFrequency, 32);
                    pstUserInfo[iCount].ulSnr = stEndUserOmc.pstStaStats[loop].cStaRssi - stEndUserOmc.pstStaStats[loop].cChanNoise;
                    fTxRate = stEndUserOmc.pstStaStats[loop].ulTxAvrRate/1000;
                    sprintf(pstUserInfo[iCount].acTxRate, "%.1f", fTxRate);
                    /* BEGIN: Modified by Zou caihong, 2013/9/13 */
                    /* 错包率统计，单位为万分之一；
                       大于0而小于万分之一的，显示为万分之一；
                       大于等于万分之一的，则正常显示 */
                    #if 0
                    pstUserInfo[iCount].ulErrRate = stEndUserOmc.pstStaStats[loop].ulRxError; //
                    #endif
                    ulTotalPkt = stEndUserOmc.pstStaStats[loop].ulRxDataFrame + 
                        stEndUserOmc.pstStaStats[loop].ulRxError;
                    fErrRate = (float)stEndUserOmc.pstStaStats[loop].ulRxError/ulTotalPkt;
                    if ( (0 < fErrRate) && ((float)1/10000 > fErrRate) )
                    {
                        pstUserInfo[iCount].ulErrRate = 1;
                    }
                    else
                    {
                        pstUserInfo[iCount].ulErrRate = fErrRate*10000;
                    }
                    /* END:   Modified by Zou caihong, 2013/9/13 */
                    switch(stEndUserOmc.pstStaStats[loop].ucStaMode)
                    {
                        case WIFI_END_USER_MODE_A:
                        case WIFI_END_USER_MODE_BG:
                            pstUserInfo[iCount].uAssoRate = 54;
                            break;
                        case WIFI_END_USER_MODE_B:
                            pstUserInfo[iCount].uAssoRate = 11;
                            break;
                        case WIFI_END_USER_MODE_NG:
                        case WIFI_END_USER_MODE_NA:
                            if(0 == atoi(vap_band))
                            {
                                if(0 == atoi(vap_shortgi))
                                {
                                    if(1 == atoi(vap_chainmask))
                                    {
                                        pstUserInfo[iCount].uAssoRate = 65;
                                    }
                                    else
                                    {
                                        pstUserInfo[iCount].uAssoRate = 130;
                                    }
                                }
                                else
                                {
                                    if(1 == atoi(vap_chainmask))
                                    {
                                        pstUserInfo[iCount].uAssoRate = 72;
                                    }
                                    else
                                    {
                                        pstUserInfo[iCount].uAssoRate = 135;
                                    }
                                }
                            }
                            else
                            {
                                if(0 == atoi(vap_shortgi))
                                {
                                    if(1 == atoi(vap_chainmask))
                                    {
                                        pstUserInfo[iCount].uAssoRate = 135;
                                    }
                                    else
                                    {
                                        pstUserInfo[iCount].uAssoRate = 270;
                                    }
                                }
                                else
                                {
                                    if(1 == atoi(vap_chainmask))
                                    {
                                        pstUserInfo[iCount].uAssoRate = 150;
                                    }
                                    else
                                    {
                                        pstUserInfo[iCount].uAssoRate = 300;
                                    }
                                }
                            }
                            break;
                        default:
                            pstUserInfo[iCount].uAssoRate = 144;
                            break;
                    }
                    iCount++;
                    if ( iCount >= 128 )
                    {
                    *pulUserNum = iCount;
                    //free
                    if ( NULL != stEndUserOmc.pstStaStats )
                    {
                        free(stEndUserOmc.pstStaStats);
                        stEndUserOmc.pstStaStats=NULL;
                    }
                    return TRUE;
                    }
                }
            }
        }
    //free
    if ( NULL != stEndUserOmc.pstStaStats )
    {
        free(stEndUserOmc.pstStaStats);
        stEndUserOmc.pstStaStats=NULL;
    }
    *pulUserNum = iCount;
    return TRUE;
}
#if 0
/*****************************************************************************
 Prototype    : wlan_get_ap24cochainterfinfo
 Description  : AP 2.4G卡同频干扰信息获取
 Input        : AP_STAT_24COCHAINTERFINFO_ENTRY_T *pstAp24CoChaInterfInfo  
                UINT32 *pulNum                                         
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/22
    Author       : zoucaihong
    Modification : Created function
  2.Date         : 2013/7/25
    Author       : mayuhua
    Modification : Modified function

*****************************************************************************/
BOOL wlan_get_ap24cochainterfinfo( AP_STAT_24COCHAINTERFINFO_ENTRY_T *pstAp24CoChaInterfInfo, 
                                           UINT32 *pulNum )
{
    UINT32 ulVapLoop = 0;
    UINT32 ulLoop = 0;
    UINT32 ulCount = 0;
    char acNvramName[NVRAM_SIZE] = {0};
    char acRadioId[NVRAM_SIZE];
    char list[128];
    char vaps_list[32];
    char devname[32] = {0};
    int total;
    unsigned long ulLocalApNum = 0;

    if ( NULL == pstAp24CoChaInterfInfo )
    {
        agent_printf(LOG_ERR, "wlan_get_ap24cochainterfinfo pstAp24CoChaInterfInfo is null.");
        return FALSE;
    }

    if ( NULL == pulNum )
    {
        agent_printf(LOG_ERR, "wlan_get_ap24cochainterfinfo pulNum is null.");
        return FALSE;
    }
     
    if(NvramResultFailed == nvram_get(list, NVRAM_VAP_LIST, sizeof(list)))
    {
        lprintf(LOG_ERR, "there is no vap for devcie.");
        return FALSE;   
    }
    else
    {
        array_string_to_int(list, vaps_list, &total);
    }

    WIFI_API_AP_LIST_T st24CochainInterfInfo;
    
    for (ulVapLoop = 0; ulVapLoop < total; ulVapLoop++)
    {
        int iRet;
        memset(st24CochainInterfInfo.stNeighborApList,0,sizeof(WIFI_API_AP_INFO_T)*WIFI_API_AP_LIST_MAX);

         sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_WIFI_ID, vaps_list[ulVapLoop]);
         nvram_get(acRadioId, acNvramName, NVRAM_SIZE);

         if(0 != atoi(acRadioId))
         {
            continue;
         }

         sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_VAP_NAME, vaps_list[ulVapLoop]);
         nvram_get(devname, acNvramName, 32);

         iRet = wifi_api_vap_get_localap(devname,&st24CochainInterfInfo);
         if(iRet != 0)
         {
         printf("%s:iRet = %d\n",__func__,iRet);
         continue;
         }
         else
         {  
            ulLocalApNum = st24CochainInterfInfo.ulNeighborApNum;
            for ( ulLoop = 0 ; ulLoop < ulLocalApNum; ulLoop++ )
            {
        	    pstAp24CoChaInterfInfo[ulCount].ulIndex = ulCount + 1;
        	    memcpy(pstAp24CoChaInterfInfo[ulCount].aucBssid, st24CochainInterfInfo.stNeighborApList[ulLoop].bssid,6);
                strcpy(pstAp24CoChaInterfInfo[ulCount].acSsid, st24CochainInterfInfo.stNeighborApList[ulLoop].ssid.ssid);
        	    pstAp24CoChaInterfInfo[ulCount].iRssi = st24CochainInterfInfo.stNeighborApList[ulLoop].rssi-95;
        	    pstAp24CoChaInterfInfo[ulCount].ulChannel = st24CochainInterfInfo.stNeighborApList[ulLoop].channel;
                ulCount++;
                
                if ( ulCount >= WIFI_API_AP_LIST_MAX )
                {
                    *pulNum = ulCount;
                    return TRUE;
                }
                
            }
          
        }
        break;
      }
      *pulNum = ulCount;
     return TRUE;
}
        
/*****************************************************************************
 Prototype    : wlan_get_ap5cochainterfinfo
 Description  : AP 5G卡同频干扰信息获取
 Input        : AP_STAT_5COCHAINTERFINFO_ENTRY_T *pstAp5CoChaInterfInfo  
                UINT32 *pulNum                                         
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/22
    Author       : zoucaihong
    Modification : Created function
  2.Date         : 2013/7/26
    Author       : mayuhua
    Modification : Modified function

*****************************************************************************/
BOOL wlan_get_ap5cochainterfinfo( AP_STAT_5COCHAINTERFINFO_ENTRY_T *pstAp5CoChaInterfInfo, UINT32 *pulNum )
{
    UINT32 ulVapLoop = 0;
    UINT32 ulLoop = 0;
    UINT32 ulCount = 0;
    char acNvramName[NVRAM_SIZE] = {0};
    char acRadioId[NVRAM_SIZE];
    char list[128];
    char vaps_list[32];
    char devname[32] = {0};
    int total;
    unsigned long ulLocalApNum = 0;

    if ( NULL == pstAp5CoChaInterfInfo )
    {
        agent_printf(LOG_ERR, "wlan_get_ap5cochainterfinfo pstAp5CoChaInterfInfo is null.");
        return FALSE;
    }

    if ( NULL == pulNum )
    {
        agent_printf(LOG_ERR, "wlan_get_ap5cochainterfinfo pulNum is null.");
        return FALSE;
    }

     if(NvramResultFailed == nvram_get(list, NVRAM_VAP_LIST, sizeof(list)))
    {
        lprintf(LOG_ERR, "there is no vap for devcie.");
        return FALSE;   
    }
    else
    {
        array_string_to_int(list, vaps_list, &total);
    }

    WIFI_API_AP_LIST_T st5CochainInterfInfo;
    for (ulVapLoop = 0; ulVapLoop < total; ulVapLoop++)
    {   
      
        int iRet;
        memset(st5CochainInterfInfo.stNeighborApList,0,sizeof(WIFI_API_AP_INFO_T)*WIFI_API_AP_LIST_MAX);
        sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_WIFI_ID, vaps_list[ulVapLoop]);
        nvram_get(acRadioId, acNvramName, NVRAM_SIZE);

         if(1 != atoi(acRadioId))
         {
            continue;
         }
         sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_VAP_NAME, vaps_list[ulVapLoop]);
         nvram_get(devname, acNvramName, 32);
         iRet = wifi_api_vap_get_localap(devname,&st5CochainInterfInfo);
         if(iRet != 0)
         {
         printf("%s:iRet = %d\n",__func__,iRet);
         continue;
         }
         else
         {
            ulLocalApNum = st5CochainInterfInfo.ulNeighborApNum;
            for ( ulLoop = 0 ; ulLoop < ulLocalApNum; ulLoop++ )
            {
                pstAp5CoChaInterfInfo[ulCount].ulIndex = ulCount + 1;
        	    memcpy(pstAp5CoChaInterfInfo[ulCount].aucBssid, st5CochainInterfInfo.stNeighborApList[ulLoop].bssid,6);
                strcpy(pstAp5CoChaInterfInfo[ulCount].acSsid, st5CochainInterfInfo.stNeighborApList[ulLoop].ssid.ssid);
        	    pstAp5CoChaInterfInfo[ulCount].iRssi = st5CochainInterfInfo.stNeighborApList[ulLoop].rssi-95;
        	    pstAp5CoChaInterfInfo[ulCount].ulChannel = st5CochainInterfInfo.stNeighborApList[ulLoop].channel;
                ulCount++;
                if ( ulCount >= WIFI_API_AP_LIST_MAX )
                {
                     *pulNum = ulCount;
                     return TRUE;
                }
          }
      }
      break;
    }
     *pulNum = ulCount;
     return TRUE;
}


/*****************************************************************************
 Prototype    : wlan_get_ap24adjchainterfinfo
 Description  : AP 2.4G卡邻频干扰信息获取
 Input        : AP_STAT_24ADJCHAINTERFINFO_ENTRY_T *pstAp24AdjChaInterfInfo  
                UINT32 *pulNum                                           
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/22
    Author       : zoucaihong
    Modification : Created function
  2.Date         : 2013/7/26
    Author       : mayuhua
    Modification : Modified function
*****************************************************************************/
BOOL wlan_get_ap24adjchainterfinfo( AP_STAT_24ADJCHAINTERFINFO_ENTRY_T *pstAp24AdjChaInterfInfo,
                                           UINT32 *pulNum )
{
    UINT32 ulVapLoop = 0;
    UINT32 ulLoop = 0;
    UINT32 ulCount = 0;
    char acNvramName[NVRAM_SIZE] = {0};
    char acRadioId[NVRAM_SIZE];
    char list[128];
    char vaps_list[32];
    char devname[32] = {0};
    int total;
    unsigned long ulNeighApNum = 0;
    UINT32 ulCurChannel = 0;
    char nvram_name[32] = {0};
    char nvram_value[32] = {0};
    
    if ( NULL == pstAp24AdjChaInterfInfo )
    {
        agent_printf(LOG_ERR, "wlan_get_ap24adjchainterfinfo pstAp24AdjChaInterfInfo is null.");
        return FALSE;
    }

    if ( NULL == pulNum )
    {
        agent_printf(LOG_ERR, "wlan_get_ap24adjchainterfinfo pulNum is null.");
        return FALSE;
    }

    if(NvramResultFailed == nvram_get(list, NVRAM_VAP_LIST, sizeof(list)))
    {
        lprintf(LOG_ERR, "there is no vap for devcie.");
        return FALSE;   
    }
    else
    {
        array_string_to_int(list, vaps_list, &total);
    }

    WIFI_API_AP_LIST_T st24AdjchainInterfInfo;
    for (ulVapLoop = 0; ulVapLoop < total; ulVapLoop++)
    {
        
        int iRet;
        memset(st24AdjchainInterfInfo.stNeighborApList,0,sizeof(WIFI_API_AP_INFO_T)*WIFI_API_AP_LIST_MAX);
        
        sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_WIFI_ID, vaps_list[ulVapLoop]);
        nvram_get(acRadioId, acNvramName, NVRAM_SIZE);
        
        if(0 != atoi(acRadioId))
        {
             continue;
        }
        
        sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_VAP_NAME, vaps_list[ulVapLoop]);
        nvram_get(devname, acNvramName, 32);
        /* 获取wifi0当前使用信道 */
        wifi_get_channel(devname, &ulCurChannel);
        iRet = wifi_api_vap_get_neighborap(devname,&st24AdjchainInterfInfo);
        if(iRet != 0)
        {
           printf("%s:iRet = %d\n",__func__,iRet);
           continue;
        }
        else
         {
            ulNeighApNum = st24AdjchainInterfInfo.ulNeighborApNum;
            for ( ulLoop = 0 ; ulLoop < ulNeighApNum; ulLoop++ )
            {
                /* 过滤同频信道 */
                if ( st24AdjchainInterfInfo.stNeighborApList[ulLoop].channel == ulCurChannel )
                {
                    continue;
                }
                pstAp24AdjChaInterfInfo[ulCount].ulIndex = ulCount + 1;
        	    memcpy(pstAp24AdjChaInterfInfo[ulCount].aucBssid, st24AdjchainInterfInfo.stNeighborApList[ulLoop].bssid,6);
                strcpy(pstAp24AdjChaInterfInfo[ulCount].acSsid,st24AdjchainInterfInfo.stNeighborApList[ulLoop].ssid.ssid);
        	    pstAp24AdjChaInterfInfo[ulCount].iRssi = st24AdjchainInterfInfo.stNeighborApList[ulLoop].rssi-95;
        	    pstAp24AdjChaInterfInfo[ulCount].ulChannel = st24AdjchainInterfInfo.stNeighborApList[ulLoop].channel;
        	    ulCount++;
                
                if ( ulCount >= WIFI_API_AP_LIST_MAX )
                {
                    *pulNum = ulCount;
                    return TRUE;
                }
               }
             }
             break;
         }
     *pulNum = ulCount;
     return TRUE;
}
/*****************************************************************************
 Prototype    : wlan_get_ap5adjchainterfinfo
 Description  : AP 5G卡邻频干扰信息获取
 Input        : AP_STAT_5ADJCHAINTERFINFO_ENTRY_T *pstAp5AdjChaInterfInfo  
                UINT32 *pulNum                                           
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/22
    Author       : zoucaihong
    Modification : Created function
  2.Date         : 2013/7/26
    Author       : mayuhua
    Modification : Modified function

*****************************************************************************/
BOOL wlan_get_ap5adjchainterfinfo( AP_STAT_5ADJCHAINTERFINFO_ENTRY_T *pstAp5AdjChaInterfInfo,
                                           UINT32 *pulNum )
{
    UINT32 ulVapLoop = 0;
    UINT32 ulLoop = 0;
    UINT32 ulCount = 0;
    char acNvramName[NVRAM_SIZE] = {0};
    char acRadioId[NVRAM_SIZE];
    char list[128];
    char vaps_list[32];
    char devname[32] = {0};
    int total;
    unsigned long ulNeighApNum = 0;
    UINT32 ulCurChannel = 0;
    char nvram_name[32] = {0};
    char nvram_value[32] = {0};

    if ( NULL == pstAp5AdjChaInterfInfo )
    {
        agent_printf(LOG_ERR, "wlan_get_ap5adjchainterfinfo pstAp5AdjChaInterfInfo is null.");
        return FALSE;
    }

    if ( NULL == pulNum )
    {
        agent_printf(LOG_ERR, "wlan_get_ap5adjchainterfinfo pulNum is null.");
        return FALSE;
    }
    
    if(NvramResultFailed == nvram_get(list, NVRAM_VAP_LIST, sizeof(list)))
    {
        lprintf(LOG_ERR, "there is no vap for devcie.");
        return FALSE;   
    }
    else
    {
         array_string_to_int(list, vaps_list, &total);
    }
  
    WIFI_API_AP_LIST_T st5AdjchainInterfInfo;
    
    for (ulVapLoop = 0; ulVapLoop < total; ulVapLoop++)
    {
        int iRet;
        memset(st5AdjchainInterfInfo.stNeighborApList,0,sizeof(WIFI_API_AP_INFO_T)*WIFI_API_AP_LIST_MAX);
        
        sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_WIFI_ID, vaps_list[ulVapLoop]);
        nvram_get(acRadioId, acNvramName, NVRAM_SIZE);
        
        if(1 != atoi(acRadioId))
        {
             continue;
        }
        sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_VAP_NAME, vaps_list[ulVapLoop]);
        nvram_get(devname, acNvramName, 32);
        /* 获取wifi1当前使用信道 */
        wifi_get_channel(devname, &ulCurChannel);
        iRet = wifi_api_vap_get_neighborap(devname,&st5AdjchainInterfInfo);
        if(iRet != 0)
        {
            printf("%s:iRet = %d\n",__func__,iRet);
            continue;
        }
        else
        {
          ulNeighApNum = st5AdjchainInterfInfo.ulNeighborApNum;
          for ( ulLoop = 0 ; ulLoop < ulNeighApNum; ulLoop++ )
            {
                /* 过滤同频信道 */
                if ( st5AdjchainInterfInfo.stNeighborApList[ulLoop].channel == ulCurChannel )
                {
                    continue;
                }
				pstAp5AdjChaInterfInfo[ulCount].ulIndex = ulCount + 1;
        	    memcpy(pstAp5AdjChaInterfInfo[ulCount].aucBssid, st5AdjchainInterfInfo.stNeighborApList[ulLoop].bssid,6);
                strcpy(pstAp5AdjChaInterfInfo[ulCount].acSsid,st5AdjchainInterfInfo.stNeighborApList[ulLoop].ssid.ssid);
        	    pstAp5AdjChaInterfInfo[ulCount].iRssi = st5AdjchainInterfInfo.stNeighborApList[ulLoop].rssi-95;
        	    pstAp5AdjChaInterfInfo[ulCount].ulChannel = st5AdjchainInterfInfo.stNeighborApList[ulLoop].channel;
        	    ulCount++;
        
                if ( ulCount >= WIFI_API_AP_LIST_MAX)
                {
                    *pulNum = ulCount;
                    return TRUE;
                }
             }
          }
          break;
     }
     *pulNum = ulCount;
     return TRUE;
}
/*****************************************************************************
 Prototype    : wlan_get_ap24wlandevinterfinfo
 Description  : 获取AP 2.4G卡WLAN设备干扰信息
 Input        : AP_STAT_24WLANDEVINTERFINFO_ENTRY_T *pstAp24WlanDevInterfInfo  
                UINT32 *pulNum                                             
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/23
    Author       : zoucaihong
    Modification : Created function
  2.Date         : 2013/7/28
    Author       : mayuhua
    Modification : Modified function
*****************************************************************************/
BOOL wlan_get_ap24wlandevinterfinfo( AP_STAT_24WLANDEVINTERFINFO_ENTRY_T *pstAp24WlanDevInterfInfo, 
                                             UINT32 *pulNum )
{
    UINT32 ulVapLoop = 0;
    UINT32 ulLoop = 0;
    UINT32 ulCount = 0;
    char acNvramName[NVRAM_SIZE] = {0};
    char acRadioId[NVRAM_SIZE];
    char list[128];
    char vaps_list[32];
    char devname[32] = {0};
    int total;
    unsigned long ulInterfApNum = 0;

    if ( NULL == pstAp24WlanDevInterfInfo )
    {
        agent_printf(LOG_ERR, "wlan_get_ap24wlandevinterfinfo pstAp24WlanDevInterfInfo is null.");
        return FALSE;
    }

    if ( NULL == pulNum )
    {
        agent_printf(LOG_ERR, "wlan_get_ap24wlandevinterfinfo pulNum is null.");
        return FALSE;
    }
     
    if(NvramResultFailed == nvram_get(list, NVRAM_VAP_LIST, sizeof(list)))
    {
        lprintf(LOG_ERR, "there is no vap for devcie.");
        return FALSE;   
    }
    else
    {
        array_string_to_int(list, vaps_list, &total);
    }

    WIFI_API_AP_LIST_T st24WlanInterfInfo;
 
    
    for (ulVapLoop = 0; ulVapLoop < total; ulVapLoop++)
    {
        int iRetLocal;
        int iRetNeighbor;
        memset(st24WlanInterfInfo.stNeighborApList,0,sizeof(WIFI_API_AP_INFO_T)*WIFI_API_AP_LIST_MAX);

         sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_WIFI_ID, vaps_list[ulVapLoop]);
         nvram_get(acRadioId, acNvramName, NVRAM_SIZE);

         if(0 != atoi(acRadioId))
         {
            continue;
         }

         sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_VAP_NAME, vaps_list[ulVapLoop]);
         nvram_get(devname, acNvramName, 32);

         #if 0
         /***********************采集2.4G同频干扰VAP信息**************************************/
         iRetLocal = wifi_api_vap_get_localap(devname,&st24WlanInterfInfo);
         if(iRetLocal != 0)
         {
         printf("%s:iRetLocal = %d\n",__func__,iRetLocal);
         continue;
         }
         else
         {  
            ulInterfApNum = st24WlanInterfInfo.ulNeighborApNum;
            for ( ulLoop = 0 ; ulLoop < ulInterfApNum; ulLoop++ )
            {
        	    pstAp24WlanDevInterfInfo[ulCount].ulIndex = ulCount + 1;
        	    memcpy(pstAp24WlanDevInterfInfo[ulCount].aucBssid, st24WlanInterfInfo.stNeighborApList[ulLoop].bssid,6);
                strcpy(pstAp24WlanDevInterfInfo[ulCount].acSsid, st24WlanInterfInfo.stNeighborApList[ulLoop].ssid.ssid);
        	    pstAp24WlanDevInterfInfo[ulCount].iRssi = st24WlanInterfInfo.stNeighborApList[ulLoop].rssi;
        	    pstAp24WlanDevInterfInfo[ulCount].ulChannel = st24WlanInterfInfo.stNeighborApList[ulLoop].channel;
                ulCount++;
                
                if ( ulCount >= WIFI_API_AP_LIST_MAX )
                {
                    *pulNum = ulCount;
                    return TRUE;
                }
                
            }
          
        }
        #endif 
        
        /***********************采集2.4G邻频干扰VAP信息**************************************/
         //memset(st24WlanInterfInfo.stNeighborApList,0,sizeof(WIFI_API_AP_INFO_T)*WIFI_API_AP_LIST_MAX);
         iRetNeighbor = wifi_api_vap_get_neighborap(devname,&st24WlanInterfInfo);

         if(iRetNeighbor != 0)
         {
         printf("%s:iRetNeighbor = %d\n",__func__,iRetNeighbor);
         continue;
         }
         else
         {  
            ulInterfApNum = st24WlanInterfInfo.ulNeighborApNum;
            for ( ulLoop = 0 ; ulLoop < ulInterfApNum; ulLoop++ )
            {
        	    pstAp24WlanDevInterfInfo[ulCount].ulIndex = ulCount + 1;
        	    memcpy(pstAp24WlanDevInterfInfo[ulCount].aucBssid, st24WlanInterfInfo.stNeighborApList[ulLoop].bssid,6);
                strcpy(pstAp24WlanDevInterfInfo[ulCount].acSsid, st24WlanInterfInfo.stNeighborApList[ulLoop].ssid.ssid);
        	    pstAp24WlanDevInterfInfo[ulCount].iRssi = st24WlanInterfInfo.stNeighborApList[ulLoop].rssi-95;
        	    pstAp24WlanDevInterfInfo[ulCount].ulChannel = st24WlanInterfInfo.stNeighborApList[ulLoop].channel;
                ulCount++;
                
                if ( ulCount >= WIFI_API_AP_LIST_MAX )
                {
                    *pulNum = ulCount;
                    return TRUE;
                }
                
            }
          
        }
        break;
      }
     *pulNum = ulCount;
     return TRUE;
}

/*****************************************************************************
 Prototype    : wlan_get_ap5wlandevinterfinfo
 Description  : 获取AP 5G卡WLAN设备干扰信息
 Input        : AP_STAT_5WLANDEVINTERFINFO_ENTRY_T *pstAp5WlanDevInterfInfo  
                UINT32 *pulNum                                             
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/23
    Author       : zoucaihong
    Modification : Created function
  2.Date         : 2013/7/28
    Author       : mayuhua
    Modification : Modified function

*****************************************************************************/
BOOL wlan_get_ap5wlandevinterfinfo( AP_STAT_5WLANDEVINTERFINFO_ENTRY_T *pstAp5WlanDevInterfInfo, 
                                             UINT32 *pulNum )
{
    UINT32 ulVapLoop = 0;
    UINT32 ulLoop = 0;
    UINT32 ulCount = 0;
    char acNvramName[NVRAM_SIZE] = {0};
    char acRadioId[NVRAM_SIZE];
    char list[128];
    char vaps_list[32];
    char devname[32] = {0};
    int total;
    unsigned long ulInterfApNum = 0;

    if ( NULL == pstAp5WlanDevInterfInfo )
    {
        agent_printf(LOG_ERR, "wlan_get_ap5wlandevinterfinfo pstAp5WlanDevInterfInfo is null.");
        return FALSE;
    }

    if ( NULL == pulNum )
    {
        agent_printf(LOG_ERR, "wlan_get_ap5wlandevinterfinfo pulNum is null.");
        return FALSE;
    }
     
    if(NvramResultFailed == nvram_get(list, NVRAM_VAP_LIST, sizeof(list)))
    {
        lprintf(LOG_ERR, "there is no vap for devcie.");
        return FALSE;   
    }
    else
    {
        array_string_to_int(list, vaps_list, &total);
    }

    WIFI_API_AP_LIST_T st5WlanInterfInfo;
 
    
    for (ulVapLoop = 0; ulVapLoop < total; ulVapLoop++)
    {
        int iRetLocal;
        int iRetNeighbor;
        memset(st5WlanInterfInfo.stNeighborApList,0,sizeof(WIFI_API_AP_INFO_T)*WIFI_API_AP_LIST_MAX);

         sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_WIFI_ID, vaps_list[ulVapLoop]);
         nvram_get(acRadioId, acNvramName, NVRAM_SIZE);

         if(1 != atoi(acRadioId))
         {
            continue;
         }

         sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_VAP_NAME, vaps_list[ulVapLoop]);
         nvram_get(devname, acNvramName, 32);

         #if 0
         /***********************采集5G同频干扰VAP信息**************************************/
         iRetLocal = wifi_api_vap_get_localap(devname,&st5WlanInterfInfo);
         if(iRetLocal != 0)
         {
         printf("%s:iRetLocal = %d\n",__func__,iRetLocal);
         continue;
         }
         else
         {  
            ulInterfApNum = st5WlanInterfInfo.ulNeighborApNum;
            printf("ulInterfApNum1 = %d+++++++++++++++++++++++++++++",ulInterfApNum);
            for ( ulLoop = 0 ; ulLoop < ulInterfApNum; ulLoop++ )
            {
        	    pstAp5WlanDevInterfInfo[ulCount].ulIndex = ulCount + 1;
        	    memcpy(pstAp5WlanDevInterfInfo[ulCount].aucBssid, st5WlanInterfInfo.stNeighborApList[ulLoop].bssid,6);
                strcpy(pstAp5WlanDevInterfInfo[ulCount].acSsid, st5WlanInterfInfo.stNeighborApList[ulLoop].ssid.ssid);
        	    pstAp5WlanDevInterfInfo[ulCount].iRssi = st5WlanInterfInfo.stNeighborApList[ulLoop].rssi;
        	    pstAp5WlanDevInterfInfo[ulCount].ulChannel = st5WlanInterfInfo.stNeighborApList[ulLoop].channel;
                ulCount++;
                
                if ( ulCount >= WIFI_API_AP_LIST_MAX )
                {
                    *pulNum = ulCount;
                    return TRUE;
                }
                
            }
          
        }
        printf("ulCount1 = %d+++++++++++++++++++++++++++++\n",ulCount);
        #endif
        
        /***********************采集5G邻频干扰VAP信息**************************************/
         //memset(st5WlanInterfInfo.stNeighborApList,0,sizeof(WIFI_API_AP_INFO_T)*WIFI_API_AP_LIST_MAX);
         iRetNeighbor = wifi_api_vap_get_neighborap(devname,&st5WlanInterfInfo);

         if(iRetNeighbor != 0)
         {
         printf("%s:iRetNeighbor = %d\n",__func__,iRetNeighbor);
         continue;
         }
         else
         {  
            ulInterfApNum = st5WlanInterfInfo.ulNeighborApNum;
            printf("ulInterfApNum2 = %d+++++++++++++++++++++++++++++",ulInterfApNum);
            printf("ulCount2 = %d+++++++++++++++++++++++++++++\n",ulCount);
            for ( ulLoop = 0 ; ulLoop < ulInterfApNum; ulLoop++ )
            {
        	    pstAp5WlanDevInterfInfo[ulCount].ulIndex = ulCount + 1;
        	    memcpy(pstAp5WlanDevInterfInfo[ulCount].aucBssid, st5WlanInterfInfo.stNeighborApList[ulLoop].bssid,6);
                strcpy(pstAp5WlanDevInterfInfo[ulCount].acSsid, st5WlanInterfInfo.stNeighborApList[ulLoop].ssid.ssid);
        	    pstAp5WlanDevInterfInfo[ulCount].iRssi = st5WlanInterfInfo.stNeighborApList[ulLoop].rssi-95;
        	    pstAp5WlanDevInterfInfo[ulCount].ulChannel = st5WlanInterfInfo.stNeighborApList[ulLoop].channel;
                ulCount++;
                
                if ( ulCount >= WIFI_API_AP_LIST_MAX )
                {
                    *pulNum = ulCount;
                    return TRUE;
                }
                
            }
          
        }
        printf("ulCount3 = %d+++++++++++++++++++++++++++++\n",ulCount);
        
        break;
      }
     *pulNum = ulCount;
     return TRUE;
}
#endif

/*****************************************************************************
 Prototype    : wlan_add_vap
 Description  : 创建VAP
 Input        : None
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/3/2
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
BOOL wlan_add_vap(AP_CONFIG_SSID_ENTRY_T *pstSsidSetup)
{
    int nvram_id = 0;
    int total;
    int loop;
    char list[128];
    char vap_list[32];
    char nvram_name[NVRAM_SIZE];
    char wifi_name[32];
    char vap_name[32];

    vap st_vap;
    
    if(NULL == pstSsidSetup)
    {
        agent_printf(LOG_ERR, "the pstSsidSetup is null.");
        return FALSE;
    }    

    // 查找卡级序列位置
    memset(list, 0, sizeof(list));
    sprintf(nvram_name, NVRAM_WIFI"%d"NVRAM_WIFI_WLAN_ID_LIST, pstSsidSetup->ulWifiId);
    if(NvramResultFailed == nvram_get(list, nvram_name, sizeof(list)))
    {
        agent_printf(LOG_ERR, "get "NVRAM_WIFI"%d"NVRAM_WIFI_WLAN_ID_LIST" fail.\n",
            pstSsidSetup->ulWifiId);
        list[0]='\0';
        pstSsidSetup->ulVapId = 0;
    }
    else
    {
        memset(vap_list, 0, sizeof(vap_list));
        array_string_to_int(list, vap_list, &total);
        /* BEGIN: Added by Zhou Shouya, 2013/9/6 */
        if ( total >= 16 )
        {
            agent_printf(LOG_ERR, "total:%u, can't create vap in wifi:%u.\n",
                total, pstSsidSetup->ulWifiId);
            return FALSE;
        }
        /* END:   Added by Zhou Shouya, 2013/9/6 */

        /* BEGIN: Added by zoucaihong, 2014/5/27 */
        if ( 0 == total )
        {
            /* 创建第一个SSID */
            pstSsidSetup->ulVapId = 0;
        }
        else
        {
            /* 非第一个SSID */
            for(loop = 0; loop < 16; loop++)
            {
                if(loop != vap_list[loop])
                {
                    pstSsidSetup->ulVapId = loop;
                    break;
                }
            }
        }
        /* END: Added by zoucaihong, 2014/5/27 */
    }
    
    array_string_add_someone(list, pstSsidSetup->ulVapId);
    nvram_set(nvram_name, list);

    // 查找设备序列位置
	memset(list, 0, sizeof(list));   
    if(NvramResultFailed == nvram_get(list, NVRAM_VAP_LIST, sizeof(list)))
    {
        agent_printf(LOG_ERR, "get "NVRAM_VAP_LIST" fail.\n");
        list[0] = '\0';
        nvram_id = 0;
    }
    else
    {
        memset(vap_list, 0, sizeof(vap_list));
        array_string_to_int(list, vap_list, &total);
        /* BEGIN: Added by zoucaihong, 2014/5/27 */
        if ( 0 == total )
        {
            /* 创建第一个SSID */
            nvram_id = 0;
        }
        else
        {
            for(loop = 0; loop < 32; loop++)
            {
                if(loop != vap_list[loop])
                {
                    nvram_id = loop;
                    break;
                }
            }
        }
        /* END: Added by zoucaihong, 2014/5/27 */
    }
    array_string_add_someone(list, nvram_id);
    nvram_set(NVRAM_VAP_LIST, list);

    // create vap
    memset(&st_vap, 0, sizeof(vap));
    st_vap.nvram_id = nvram_id;
    st_vap.ssid_mode = 0;
    st_vap.wifi_freq = pstSsidSetup->ulWifiId;
    st_vap.wifi_id = pstSsidSetup->ulWifiId;
    st_vap.wlan_id = pstSsidSetup->ulVapId;
    memcpy(st_vap.ssid_name, pstSsidSetup->acSsidName, strlen(pstSsidSetup->acSsidName));
    sprintf(wifi_name, WIFINAME"%d", st_vap.wifi_id);
    sprintf(vap_name, "ap%d_%d", st_vap.wifi_id, st_vap.wlan_id);
    /* BEGIN: Modified by Zhou Shouya, 2013/9/6 */
    if ( OK != wlan_set_vap_create(&st_vap, vap_name, wifi_name) )
    {
        agent_printf(LOG_ERR, "wlan_set_vap_create fail.\n");
        return FALSE;
    }
    /* END:   Modified by Zhou Shouya, 2013/9/6 */

    vap_nvram_set(&st_vap);
       
    return TRUE;
}/* end wlan_add_vap */

/*****************************************************************************
 Prototype    : wlan_del_vap
 Description  : 删除VAP
 Input        : None
 Output       : None
 Return Value : int
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/3/4
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
int wlan_del_vap(AP_CONFIG_SSID_ENTRY_T *pstSsidSetup)
{
    char list[128];
    char nvram_name[NVRAM_SIZE];
    char vap_name[32];

    if(NULL == pstSsidSetup)
    {
        agent_printf(LOG_ERR, "the pstSsidSetup is null.");
        return FALSE;
    }

    // 删除配置区数据
    vap_nvram_unset(pstSsidSetup->ulIndex-1);

    /* 删除存储在设备的序号 */
    nvram_get(list, NVRAM_VAP_LIST, sizeof(list));
    array_string_del_someone(list, pstSsidSetup->ulIndex-1);
    nvram_set(NVRAM_VAP_LIST, list);

    /* 删除存储在卡级的序号 */
    sprintf(nvram_name, NVRAM_WIFI"%d_nvram_id_list", pstSsidSetup->ulWifiId);
    nvram_get(list, nvram_name, sizeof(list));
    array_string_del_someone(list, pstSsidSetup->ulVapId);
    nvram_set(nvram_name, list);
    nvram_commit();

    // 删除vap
    sprintf(vap_name, "ap%d_%d", pstSsidSetup->ulWifiId, pstSsidSetup->ulVapId);
    wlan_set_vap_destroy(vap_name);
    
    return TRUE;
}
/*****************************************************************************
 Prototype    : agent_check_wifi_mode
 Description  : 检查卡的配置无线模式的合法性
 Input        : UINT32 ulWifiId  
                UINT32 ulMode    
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/8/2
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
BOOL agent_check_wifi_mode(UINT32 ulWifiId, UINT32 ulMode)
{
    if ( 0 == ulWifiId )
    {
        switch ( ulMode )
        {
            case MODE_11B :
            case MODE_11G :
            case MODE_11B_11G  :
            case MODE_11GN  :
            case MODE_11G_11GN  :
            case MODE_11B_11G_11GN  :
                agent_printf(LOG_INFO, "ulMode:%u is valid.\n", ulMode);
                break;
            default:
                agent_printf(LOG_INFO, "ulMode:%u is invalid.\n", ulMode);
                return FALSE;
        }
    }
    else if ( 1 == ulWifiId )
    {
        switch ( ulMode )
        {
            case MODE_11A :
            case MODE_11AN :
            case MODE_11A_11AN :
                agent_printf(LOG_INFO, "ulMode:%u is valid.\n", ulMode);
                break;
            default:
                agent_printf(LOG_INFO, "ulMode:%u is invalid.\n", ulMode);
                return FALSE;
        }
    }
    return TRUE;
}
BOOL agent_set_wifi5_ch52_power()
{
    UINT32  ulLimitTxPower = 0;
    UINT32  ulMaxTxPower = 0;
    UINT32  ulTransmitPower = 0;

    char nvram_name[32];
    char nvram_value[32];
    char vap_name[32];

    /* 5G卡使用静态100信道以内的最大发射功率为15dBm */
    ulLimitTxPower = 15;
    
    /* 获取当前功率衰减配置 */
    sprintf(nvram_name, NVRAM_WIFI"%d_current_txpower", 1);
    nvram_get(nvram_value, nvram_name, NVRAM_LEN);
    ulMaxTxPower = atoi(nvram_value);
    
    /* 功率衰减最大值为10dBm */
    if ( 10 < ulMaxTxPower )
    {
        ulMaxTxPower = 10;
        ulTransmitPower = ulLimitTxPower -ulMaxTxPower;
    }
    /* 设置5G卡使用静态100信道以内的最大发射功率 */
    sprintf(nvram_name, NVRAM_WIFI"%d_limit_txpower", 1);
    sprintf(nvram_value, "%d", ulLimitTxPower);
    nvram_set(nvram_name, nvram_value);

    /* 设置当前发射功率 */
    get_vap_name_for_wifi_id(vap_name, 1);
    wlan_set_txpower(vap_name, ulTransmitPower);
    nvram_commit();

    return TRUE;
}
BOOL agent_check_wifi5_ch100_power()
{
    UINT32  ulLimitTxPower = 0;
    UINT32  ulMaxTxPower = 0;
    UINT32  ulTransmitPower = 0;

    char nvram_name[32];
    char nvram_value[32];
    char vap_name[32];

    /* 获取当前最大发射功率 */
    sprintf(nvram_name, NVRAM_WIFI"%d_limit_txpower", 1);
    nvram_get(nvram_value, nvram_name, NVRAM_LEN);
    ulLimitTxPower = atoi(nvram_value);
    if ( 22 == ulLimitTxPower )
    {
        return TRUE;
    }
    else
    {
        /* 5G卡使用静态100信道及以上时的最大发射功率为22dBm */
        ulLimitTxPower = 22;
        
        /* 获取当前功率衰减配置 */
        sprintf(nvram_name, NVRAM_WIFI"%d_current_txpower", 1);
        nvram_get(nvram_value, nvram_name, NVRAM_LEN);
        ulMaxTxPower = atoi(nvram_value);
        
        /* 功率衰减最大值为10dBm */
        if ( 10 < ulMaxTxPower )
        {
            ulMaxTxPower = 10;
            ulTransmitPower = ulLimitTxPower -ulMaxTxPower;
        }
        /* 设置5G卡使用静态100信道及以上时的最大发射功率 */
        sprintf(nvram_name, NVRAM_WIFI"%d_limit_txpower", 1);
        sprintf(nvram_value, "%d", ulLimitTxPower);
        nvram_set(nvram_name, nvram_value);

        /* 设置当前发射功率 */
        get_vap_name_for_wifi_id(vap_name, 1);
        wlan_set_txpower(vap_name, ulTransmitPower);
        nvram_commit();
    }

    return TRUE;
}
BOOL agent_check_static_channel_validity(UINT32 ulWifiId, UINT32 ulStaticChannel)
{
    /* 2.4G静态信道范围:1,2,3,4,5,6,7,8,9,10,11 */
    if ( 0 == ulWifiId && ulStaticChannel > ENUM_APWIFICHANNEL_CH11 )
    {
        agent_printf(LOG_ERR, "wifi0 static channel:%u invalid.\n", ulStaticChannel);
        return FALSE;
    }
    /* 5G静态信道范围:52,56,60,64,100,104,108,112,116,132,136,140,149,153,157,161,165 */
    else if( 1 == ulWifiId )
    {
        switch ( ulStaticChannel )
        {
            case ENUM_APWIFICHANNEL_CH52 :
            case ENUM_APWIFICHANNEL_CH56 :
            case ENUM_APWIFICHANNEL_CH60 :
            case ENUM_APWIFICHANNEL_CH64 :
            case ENUM_APWIFICHANNEL_CH100 :
            case ENUM_APWIFICHANNEL_CH104 :
            case ENUM_APWIFICHANNEL_CH108 :
            case ENUM_APWIFICHANNEL_CH112 :
            case ENUM_APWIFICHANNEL_CH116 :
            case ENUM_APWIFICHANNEL_CH132 :
            case ENUM_APWIFICHANNEL_CH136 :
            case ENUM_APWIFICHANNEL_CH140 :
            case ENUM_APWIFICHANNEL_CH149 :
            case ENUM_APWIFICHANNEL_CH153 :
            case ENUM_APWIFICHANNEL_CH157 :
            case ENUM_APWIFICHANNEL_CH161 :
            case ENUM_APWIFICHANNEL_CH165 :
                break;
            default:
                agent_printf(LOG_ERR, "wifi1 static channel:%u invalide.\n", ulStaticChannel);
                return FALSE;
        }
    }
    return TRUE;
}
int wifi_device_channel_config(AP_CONFIG_WIFIDEVICECHANNEL_T *pstData, AP_CONFIG_WIFIDEVICECHANNEL_T *pstDataOld)
{
    char wifi_name[32];
    char nvram_name[32];
    char nvram_value[32];
    
    if(NULL == pstData || NULL == pstDataOld)
    {
        return FALSE;
    }

    if(pstData->ulAbgMode != pstDataOld->ulAbgMode
        || pstData->ulBwMode != pstDataOld->ulBwMode)
    {
        /* BEGIN: Added by Zhou Shouya, 2013/8/2   PN:VLAN */
        /* 增加配置模式合法性检查 */
        if ( FALSE == agent_check_wifi_mode(pstData->ulWifiId, pstData->ulAbgMode) )
        {
            agent_printf(LOG_ERR, "agent_check_wifi_mode fail.\n");
            return FALSE;
        }
        /* 频宽模式:bw20MHz-only(0), bw20MHz/40MHz-auto(1),bw40Hz-only(2) */
        if ( pstData->ulBwMode > 2 )
        {
            agent_printf(LOG_ERR, "ulBwMode:%u invalid.\n", pstData->ulBwMode);
            return FALSE;
        }
        /* END:   Added by Zhou Shouya, 2013/8/2   PN:VLAN */
        wlan_vap_down_all(pstData->ulWifiId);
        wlan_set_mode_all(pstData->ulWifiId, pstData->ulAbgMode, pstData->ulBwMode);
        wlan_vap_up_all(pstData->ulWifiId, wifi_name);
        sprintf(nvram_name, NVRAM_WIFI"%d_mode", pstData->ulWifiId);
        sprintf(nvram_value, "%d", pstData->ulAbgMode);
        nvram_set(nvram_name, nvram_value);

        sprintf(nvram_name, NVRAM_WIFI"%d_band", pstData->ulWifiId);
        sprintf(nvram_value, "%d", pstData->ulBwMode);
        nvram_set(nvram_name, nvram_value);
    }
    
    #if 0
    if(pstData->ulStaicChannel != pstDataOld->ulStaicChannel)
    {
        wlan_vap_down_all(pstData->ulWifiId);
        wlan_set_channel_all(pstData->ulWifiId, pstData->ulStaicChannel);
        wlan_vap_up_all(pstData->ulWifiId, wifi_name);
        sprintf(nvram_name, NVRAM_WIFI"%d_channel", pstData->ulWifiId);
        sprintf(nvram_value, "%d", pstData->ulStaicChannel);
        nvram_set(nvram_name, nvram_value);
    }
    #endif

    if(pstData->ulRFNum != pstDataOld->ulRFNum)
    {
        /* BEGIN: Added by Zhou Shouya, 2013/8/7   PN:VLAN */
        /* 空间流数:one(1),two(2) */
        if ( pstData->ulRFNum<1 || pstData->ulRFNum>2 )
        {
            agent_printf(LOG_ERR, "ulRFNum:%u invalid.\n", pstData->ulRFNum);
            return FALSE;
        }
        /* END:   Added by Zhou Shouya, 2013/8/7   PN:VLAN */
        sprintf(wifi_name, WIFINAME"%d", pstData->ulWifiId);
        wlan_vap_down_all(pstData->ulWifiId);
        wlan_set_rfnum(wifi_name, pstData->ulRFNum);
        wlan_vap_up_all(pstData->ulWifiId, wifi_name);

        sprintf(nvram_name, NVRAM_WIFI"%d_rfnum", pstData->ulWifiId);
        sprintf(nvram_value, "%d", pstData->ulRFNum);
        nvram_set(nvram_name, nvram_value);
    }

    if(pstData->ulChannelState != pstDataOld->ulChannelState)
    {
        /* BEGIN: Added by Zhou Shouya, 2013/8/7   PN:VLAN */
        /* 信道模式:static(0),auto(1) */
        if ( pstData->ulChannelState > 1 )
        {
            agent_printf(LOG_ERR, "ulChannelState:%u invalid.\n", pstData->ulChannelState);
            return FALSE;
        }
        /* END:   Added by Zhou Shouya, 2013/8/7   PN:VLAN */
        sprintf(nvram_value, "%d", pstData->ulChannelState);
        sprintf(nvram_name, NVRAM_WIFI"%d_auto_channel_swtich", pstData->ulWifiId);
        nvram_set(nvram_name, nvram_value);

        /* BEGIN: Added by Zhou Shouya, 2013/8/2   PN:VLAN */
        /* 如果为静态信道，当使用的静态信道为52时，需配置功率衰减，使最大功率为16dBm */
        if ( AUTOCHANNEL_OFF == pstData->ulChannelState )
        {
            if ( ENUM_APWIFICHANNEL_CH100 > pstData->ulStaicChannel )
            {
                agent_set_wifi5_ch52_power();
            }
        }
        /* END:   Added by Zhou Shouya, 2013/8/2   PN:VLAN */
    }

    /* BEGIN: Added by Zhou Shouya, 2013/8/2   PN:VLAN */
    /* 当信道模式为静态信道时才可以配置静态信道 */
    if ( AUTOCHANNEL_OFF == pstData->ulChannelState )
    {
        if(pstData->ulStaicChannel != pstDataOld->ulStaicChannel)
        {
            /*静态信道合法性检查*/
            if ( FALSE == agent_check_static_channel_validity(pstData->ulWifiId, pstData->ulStaicChannel) )
            {
                agent_printf(LOG_ERR, "agent_check_static_channel_validity fail.\n");
                return FALSE;
            }
            wlan_vap_down_all(pstData->ulWifiId);
            wlan_set_channel_all(pstData->ulWifiId, pstData->ulStaicChannel);
            wlan_vap_up_all(pstData->ulWifiId, wifi_name);
            sprintf(nvram_name, NVRAM_WIFI"%d_channel", pstData->ulWifiId);
            sprintf(nvram_value, "%d", pstData->ulStaicChannel);
            nvram_set(nvram_name, nvram_value);

            /* 如果为静态信道，当使用的静态信道为100以内时，最大功率为15dBm */
            if ( ENUM_APWIFICHANNEL_CH100 >= pstData->ulStaicChannel )
            {
                agent_set_wifi5_ch52_power();
            }
            /* BEGIN: Added by Zhou Shouya, 2013/8/5   PN:VLAN */
            /* 静态100信道及以上的最大发射功率为22dBm */
            else
            {
                agent_check_wifi5_ch100_power();
            }
            /* END:   Added by Zhou Shouya, 2013/8/5   PN:VLAN */
        }
    }
    /* END:   Added by Zhou Shouya, 2013/8/2   PN:VLAN */

    if(pstData->ulAutoChanTime != pstDataOld->ulAutoChanTime)
    {
        /* BEGIN: Added by Zhou Shouya, 2013/8/1   PN:VLAN */
        /* 动态信道扫描时间范围为5min-24h */
        if ( 5*60 > pstData->ulAutoChanTime
            || 60*60*24 < pstData->ulAutoChanTime)
        {
            agent_printf(LOG_ERR, "File:%s Line:%d ulAutoChanTime:%u is invalid.\n",
                __file__, __line__, pstData->ulAutoChanTime);
            return FALSE;
        }
        /* END:   Added by Zhou Shouya, 2013/8/1   PN:VLAN */
        sprintf(nvram_value, "%d", pstData->ulAutoChanTime);
        sprintf(nvram_name, NVRAM_WIFI"%d_auto_channel_cly", pstData->ulWifiId);
        nvram_set(nvram_name, nvram_value);
    }

    agent_printf(LOG_INFO, "pstData->acAutoChanList:%s, pstDataOld->acAutoChanList:%s.",
        pstData->acAutoChanList, pstDataOld->acAutoChanList);
    if(0 != memcmp(pstData->acAutoChanList, pstDataOld->acAutoChanList, sizeof(pstData->acAutoChanList)))
    {
        /* 根据卡配置的模式检查动态列表中的信道是否合法 */
        if ( FALSE == agent_checkvalid_auto_channel(pstData->ulAbgMode, pstData->acAutoChanList) )
        {
            agent_printf(LOG_ERR, "agent_checkvalid_auto_channel fail.");
            return FALSE;
        }
        sprintf(nvram_name, NVRAM_WIFI"%d_auto_channel_list", pstData->ulWifiId);
        nvram_set(nvram_name, pstData->acAutoChanList);
    }

    return TRUE;
}



#endif

