/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : interface_app.c
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2012/11/7
  Last Modified :
  Description   : snmp模块和上层应用层接口
              app_get_devmac
              app_get_swinfo
              app_get_devmac
              app_get_swinfo
              app_get_sysinfo
              app_get_sys_devinfo
              app_get_uptime
              mem_str2info
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
#include <sys/sysinfo.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/wireless.h>

#include "my_snmp_def.h"
#include "my_snmp_data.h"
#include "mini_snmpd.h"
#include "my_snmp_handle.h"
#include "my_snmp_util.h"
#include "snmp_interface.h"
#include "snmp_nms.h"

#include "config_common.h"
#include "tool.h"
#include "nvram_config.h"
#include "interface.h"

/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/

/*----------------------------------------------*
 * module-wide global variables                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macros                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * routines' implementations                    *
 *----------------------------------------------*/
#ifndef GW_X86
extern int get_internal_model_name(char *model_name);
#endif
UINT8 g_aucDevMac[6]={0};

/*****************************************************************************
 Prototype    : get_system_average_cpu
 Description  : 获取系统调用的平均CPU
 Input        : void
 Output       : None
 Return Value : void
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/4/19
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
int get_system_average_cpu( void )
{
    FILE *pf;
    char cpu[32];
    system("cpu -a | cut -d ' ' -f 4 >/tmp/cpuaverage");
    pf = fopen("/tmp/cpuaverage", "r");
    if(NULL==pf)
        return 1;
    fgets(cpu, 32, pf);
    fclose(pf);
    return atoi(cpu);
}

/*****************************************************************************
 Prototype    : app_get_uptime
 Description  : 获取系统运行时间(单位为s)
 Input        : None
 Output       : None
 Return Value : UINT32
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2012/11/7
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
UINT32 app_get_uptime(void)
{
    struct sysinfo info     = {0};
    int iRet                = -1;

    iRet = sysinfo(&info);

    if (0 == iRet)
    {
        //agent_printf(LOG_INFO, "the os uptime is %ld", info.uptime);
        return info.uptime;
    }
    else
    {
        agent_printf(LOG_ERROR, "get os uptime fail");
        return 0;
    }
    
}/* end app_get_uptime */

/*****************************************************************************
 Prototype    : app_get_devmac
 Description  : 获取设备MAC地址
 Input        : char *pcMac
              : char *devName
 Output       : None
 Return Value : UINT32
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2012/11/7
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
UINT32 app_get_devmac( char *pcMac, char *devName )
{
    #ifdef OPENWRT
    if( ERROR == pal_network_mac_get(pcMac,devName))
        return FALSE;
    #else
    char aucStrCmd[256] = {0};

    FILE *pfData    = NULL;
    char cBuff[128] = {0};
    int iMac[6]     = {0};
    int iLoop       = 0;

    sprintf(aucStrCmd, "ifconfig %s | grep HWaddr | cut -f 2 -d 'H' | cut -b 7-24 > /tmp/devmac.tmp", devName);
    system(aucStrCmd);

    pfData = fopen("/tmp/devmac.tmp", "r");
    if (NULL == pfData)
    {
        agent_printf(LOG_ERROR, "open devmac.tmp fail ,and get %s mac fail", devName);
        return FALSE;
    }
    if (NULL == fgets(cBuff, 128, pfData))
    {
        fclose(pfData);
        system("rm -f /tmp/devmac.tmp");
        agent_printf(LOG_ERROR, "read devmac.tmp fail ,and get %s mac fail", devName);
        return FALSE;
    }
    fclose(pfData);
#ifndef GW_X86
    sscanf(cBuff, "%02x:%02x:%02x:%02x:%02x:%02x",
        &iMac[0], &iMac[1], &iMac[2], &iMac[3], &iMac[4], &iMac[5]);
#else
	sscanf(cBuff, "%02x:%02x:%02x:%02x:%02x:%02x",
		&iMac[0], &iMac[1], &iMac[2], &iMac[3], &iMac[4], &iMac[5]);
#endif
    for (iLoop = 0; iLoop < 6; iLoop++)
    {
        pcMac[iLoop] = iMac[iLoop];
    }
    
    agent_printf(LOG_INFO, "the dev %s mac is "MAC_FMT, devName, MAC_ARG(pcMac));
    
    system("rm -f /tmp/devmac.tmp");
    #endif
    return TRUE;
}/* end app_get_devmac */

/*****************************************************************************
 Prototype    : app_get_sys_devinfo
 Description  : 获取设备基本信息配置
 Input        : AP_CONFIG_SYS_DEVINFO_T *pstDevInfo
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2012/11/7
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
BOOL app_get_sys_devinfo(AP_CONFIG_SYS_DEVINFO_T *pstDevInfo)
{
    char acSysGwddr[NVRAM_SIZE]  = {0};
    char cSysMacAddr[6]            = {0};
    char acValue[NVRAM_SIZE] = {0};
    int iSysIpAddr[4]              = {0};
    char acCmd[128] = {0};
    CHAR acBuf[128] = {0};
    CHAR acIfName[32] = {0};
    CHAR acIfStatus[32] = {0};
    char acForwardMode[32] = {0};
    FILE *pfData = NULL;
    
    int iLoop = 0;
    

    if (NULL == pstDevInfo)
    {
        agent_printf(LOG_ERROR, "pstDevInfo is NULL");
        return FALSE;
    }

    /* 获取IP地址 */
    #ifdef OPENWRT
    pal_network_wan_ip_get(acSysGwddr);
    #else
    nvram_get(acSysGwddr, NVRAM_IPv4"_0_"NVRAM_IP_ADDRESS, NVRAM_SIZE);
    #endif 
    sscanf(acSysGwddr, "%d.%d.%d.%d", 
        &iSysIpAddr[0], &iSysIpAddr[1], &iSysIpAddr[2], &iSysIpAddr[3]);
    for (iLoop = 0; iLoop < 4; iLoop++)
    {
        pstDevInfo->aucIpAddress[iLoop] = iSysIpAddr[iLoop];
    }

    /* 获取设备MAC地址 */
#ifndef GW_X86
    if (FALSE == app_get_devmac(cSysMacAddr, "eth0"))
#else
	if (FALSE == app_get_devmac(cSysMacAddr, "eth0"))
#endif
    {
        agent_printf(LOG_ERROR, "get dev MAC Address fail, then set MAC:00:00:00:00:00:00");
        memset(pstDevInfo->aucSysMacAddr, 0, sizeof(pstDevInfo->aucSysMacAddr));
    }
    else
    {
        memcpy(pstDevInfo->aucSysMacAddr, cSysMacAddr, sizeof(cSysMacAddr));
        if ( 0 != memcmp(g_aucDevMac, cSysMacAddr, 6) )
        {
            memcpy(g_aucDevMac, cSysMacAddr, 6);
        }
    }

    /* 获取运行时间, 返回10ms单位 */
    pstDevInfo->ulSysUpTime = app_get_uptime() * 100;
    /* BEGIN: Added by zoucaihong, 2013/1/22 */
    /*
    * get ifSpeed
    */
    /* 2014/4/3 added by zhangping */
    sprintf(acCmd, "phyctl getstatus "ETH0" > /tmp/ifspeed.tmp");
    system(acCmd);
    
    if ( NULL != (pfData = fopen("/tmp/ifspeed.tmp", "r")) )
    {
    	memset(acBuf, 0, 128);    
        if (NULL != fgets(acBuf, 128, pfData))
        {
            sscanf(acBuf, "%s %s %dMbsp", acIfName, acIfStatus, &pstDevInfo->ulIfSpeed);
        }
        fclose(pfData);
        system("rm -f /tmp/ifspeed.tmp");
    }
    /* END: Added by zoucaihong, 2013/1/22 */
    
    /* BEGIN: Added by zoucaihong, 2014/5/15 */
    /* 获取设备型号 */
    app_get_device_id(&pstDevInfo->ulSysModel);
    /* BEGIN: Added by zoucaihong, 2014/8/20 */
#ifndef GW_X86
    #ifdef OPENWRT
    pal_sysinfo_device_internel_model_name(pstDevInfo->acInterModel);
    #else
    get_internal_model_name(pstDevInfo->acInterModel);
    #endif
#else
	strcmp(pstDevInfo->acInterModel, "Gateway");
#endif
    /* END: Added by zoucaihong, 2014/8/20 */
    nvram_get(pstDevInfo->acSysModelName, NVRAM_DEVICE_TYPE, NVRAM_SIZE);
    /* END: Added by zoucaihong, 2014/5/15 */
    pal_sysinfo_device_type(pstDevInfo->acSysModelName);
    
    if(0 != nvram_get(acValue, NVRAM_TIMEOUTINTERVAL, NVRAM_SIZE))
    {
        pstDevInfo->ulTimeOutInterval = DEV_CMS_TIMEOUT;
    }
    else
    {
        pstDevInfo->ulTimeOutInterval = atoi(acValue);
    }
    if(0 != nvram_get(acValue, NVRAM_HEARTBEATPERIOD, NVRAM_SIZE))
    {
        pstDevInfo->ulHeartBeatPeriod = DEV_HEART_PERIOD;
    }
    else
    {
        pstDevInfo->ulHeartBeatPeriod = atoi(acValue);
    }
    /* BEGIN: Added by zoucaihong, 2014/9/23 */
    /* device type
     * NAT模式、route模式        (网关)
     * AP模式                    (AP)
    */
    nvram_get(acForwardMode, NVRAM_FORWARD_MODE, NVRAM_SIZE);
    if ( 2 == atoi(acForwardMode) )
    {
        pstDevInfo->ulDevType = 2;
    }
    else
    {
        pstDevInfo->ulDevType = 1;
        nvram_get(pstDevInfo->acHotId, NVRAM_DEVICE_HOTSPOT_NAME, NVRAM_SIZE);
    }
    #ifdef OPENWRT
    if(pal_sysinfo_device_name(pstDevInfo->acSysName) != 0)
        strcpy(pstDevInfo->acSysName, "GBCOM GW");

    agent_printf(LOG_CRIT, "pstDevInfo->acSysName %s pstDevInfo->acHotId %s ",pstDevInfo->acSysName,pstDevInfo->acHotId);
    #else
    /* get device name */
    if(NvramResultSuccess != nvram_get(pstDevInfo->acSysName, NVRAM_DEVICE_NAME, NVRAM_SIZE))
    {
        agent_printf(LOG_ERR, "get device name failed, default gbcom ap");
        strcpy(pstDevInfo->acSysName, "GBCOM AP");
    }
    /* END: Added by zoucaihong, 2014/9/23 */
    #endif
    

    agent_printf(LOG_INFO, "the dev MAC: "MAC_FMT" the dev IP:"IP_FMT" the dev uptime:%d"
        " sysModel:%d InterModel:%s sysModelName:%s ulDevType:%d acSysName:%s.\n",
        MAC_ARG(pstDevInfo->aucSysMacAddr),
        IP_ARG(pstDevInfo->aucIpAddress),
        pstDevInfo->ulSysUpTime,
        pstDevInfo->ulSysModel,
        pstDevInfo->acInterModel,
        pstDevInfo->acSysModelName,
        pstDevInfo->ulDevType,
        pstDevInfo->acSysName
        );
    return TRUE;
        
} /* end app_get_sys_devinfo */
BOOL app_get_running_version(CHAR *pRVersion)
{
	#ifdef OPENWRT
    return pal_sysinfo_cur_soft_ver(pRVersion);
    #else
	CHAR aucCMD[128]={0};
	FILE *fd=NULL;
	CHAR tmp[32]={0};
	if(NULL == pRVersion)
	{
		agent_printf(LOG_ERR, "pRVersion is NUll");
		return FALSE;
	}
	sprintf(aucCMD,"vlist|grep running|cut -f 2 -d ':' >/tmp/getversion.txt");
	system(aucCMD);
	if(NULL == (fd = fopen("/tmp/getversion.txt","r")))
	{
		system("rm -f /tmp/getversion.txt");
		return FALSE;
	}
	if(NULL==fgets(tmp,32,fd))
	{
		fclose(fd);
		system("rm -f /tmp/getversion.txt");
		agent_printf(LOG_ERR,"can not get file !");
		return FALSE;
	}
	memcpy(pRVersion,tmp+1,32-1);
	/*去掉从临时文件获取到得字符串的\n*/
	pRVersion[strlen(pRVersion)-1]='\0';
	fclose(fd);
	system("rm -f /tmp/getversion.txt");
	return TRUE;
	#endif
}

/*****************************************************************************
 Prototype    : app_get_swinfo
 Description  : 获取软件硬件版本号
                1. 目前只获取软件版本号
 Input        : AP_CONFIG_SYS_SWINFO_T *pstSwInfo
 Output       : None
 Return Value : UINT32
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2012/11/7
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
UINT32 app_get_swinfo(AP_CONFIG_SYS_SWINFO_T *pstSwInfo)
{
    nvram_get(pstSwInfo->acOemVersion, NVRAM_SOFT_VERSION, NVRAM_SIZE);
    agent_printf(LOG_INFO, "the dev software %s", pstSwInfo->acSoftwareVersion);

    /*获取当前版本号*/
	if(FALSE == app_get_running_version(pstSwInfo->acSoftwareVersion))
	{
        agent_printf(LOG_ERR,"wifi_get_running_version fail");
		return FALSE;
	}	
    return TRUE;
} /* end app_get_swinfo */

/*****************************************************************************
 Prototype    : app_get_hdinfo
 Description  : 获取硬件信息
 Input        : BOOL
 Output       : None
 Return Value : AP_CONFIG_SYS_HWINFO_T *pstHdInfo
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2012/11/16
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
BOOL app_get_hdinfo(AP_CONFIG_SYS_HWINFO_T *pstHdInfo)
{
    /* 获取CPU类型 */
    //strcpy(pstHdInfo->acCpuType, "Atheros AR934x");
    app_get_cpuinfo(pstHdInfo->acCpuType, &pstHdInfo->ulHardwareType);
    app_get_boardinfo(&pstHdInfo->ulBoardVersion);

    /* 内存信息 */
    strcpy(pstHdInfo->acMemType, "SDRAM 64M, FLASH 16M");

    /* 内存大小 */
    pstHdInfo->ulMemSize = 58300;

    /* 磁盘大小 */
    pstHdInfo->ulDiskSize = 16384;
    
    /* 获取硬件版本号 */
    nvram_get(pstHdInfo->acHardwareVersion, NVRAM_HARD_VERSION, NVRAM_SIZE);
    
    return TRUE;
}/* end app_get_hdinfo */

/*****************************************************************************
 Prototype    : mem_str2info
 Description  : char to int
 Input        : char *pcBuff
                unsigned int *pucBuff
                int iMax
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2012/11/7
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
BOOL mem_str2info( char *pcBuff, unsigned int *pucBuff, int iMax )
{
    int i = 0;
	char acCurNum[64];
	int iCurNumLoc = 0;
	int iConvertCnt = 0;
	int iByteNum = 0, iDigitBegin = 0;

	if(NULL == pcBuff)
	{
		return -1;
	}

	iByteNum = strlen(pcBuff);
	for(i = 0; i < iByteNum; i++)
	{
		if(!isdigit(pcBuff[i]))
		{
			if(0 == iDigitBegin)
			{
				continue;
			}
			else 
			{
				goto num_convert;
			}
		}
		else
		{
			if(0 == iDigitBegin)
			{
				iDigitBegin = 1;
			}
			acCurNum[iCurNumLoc ++] = pcBuff[i];
			continue;
		}
num_convert:
		pucBuff[iConvertCnt] = atoi(acCurNum);
		iDigitBegin = 0;
		iCurNumLoc = 0;
		memset(acCurNum, 0, 64);
		iConvertCnt ++;
		if(iConvertCnt >= iMax)
		{
			return 0;
		}
	}

	return 0;
}/* end mem_str2info */

/*****************************************************************************
 Prototype    : app_get_sysinfo
 Description  : 获取设备系统信息
                1. 获取设备CPU
                2. 获取设备Memory
 Input        : AP_STAT_SYS_T *pstStatSys
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2012/11/7
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
BOOL app_get_sysinfo( AP_STAT_SYS_T *pstStatSys )
{
    char cBuff[1024] = {0};
    char cTemp[1024] = {0};
    char acNvramValue[32] = {0};
    FILE *pfData     = NULL;
    char *pacBuff    = NULL;
    char *pacTemp    = NULL;
    unsigned int aiMemInfo[MEM_MAX] = {0};
    float fMemUsage = 0.0;

    /* 获取cpu */
    system("cpu >"SNMP_CPU);
    pfData = fopen(SNMP_CPU, "r");
    if (NULL == pfData)
    {
        agent_printf(LOG_ERROR, "get CPU fail");
        /* 这里不能直接return , 还要处理其他统计 */
    }
    else if (NULL != fgets(cBuff, 128, pfData))
    {
        strcpy(cTemp, "busy ");
        fclose(pfData);
        pacBuff = strstr(cBuff, cTemp);
        if (NULL != pacBuff)
        {
            pacBuff = pacBuff + strlen(cTemp);
            pacTemp = pacBuff;
            while ('\0' != pacBuff)
            {
                if ('%' == *pacBuff)
                {
                    *pacBuff = '\0';
                    pstStatSys->ulCPURTUsage = atoi(pacTemp)==0?1:atoi(pacTemp);
                    agent_printf(LOG_INFO, "the dev cpu: %d", pstStatSys->ulCPURTUsage);
                    break;
                }
                pacBuff++;
            }

            pacBuff = NULL;
            pacTemp = NULL;
        }
        
    }
    else
        fclose(pfData);
    system("rm -f "SNMP_CPU);
    pfData = NULL;

    /* 获取CPU峰值 */
    nvram_get(acNvramValue, "CpuHighUsage", 32);
    if(0==atoi(acNvramValue))
        pstStatSys->ulCPUPeakUsage=0;
    else
        pstStatSys->ulCPUPeakUsage=atoi(acNvramValue);

    if(pstStatSys->ulCPURTUsage > pstStatSys->ulCPUPeakUsage)
        pstStatSys->ulCPUPeakUsage = pstStatSys->ulCPURTUsage;

    /* 获取CPU平均利用率 */
    pstStatSys->ulCPUAvgUsage = get_system_average_cpu();
    if(0==pstStatSys->ulCPUAvgUsage)
        pstStatSys->ulCPUAvgUsage = 1;
    
    /* 获取memory */
    system("free >"SNMP_MEM);
    pfData = fopen(SNMP_MEM, "r");
    if (NULL == pfData)
    {
        agent_printf(LOG_ERROR, "get Memory fail");
        /* 这里不能直接return , 还要处理其他统计 */
    }
    else 
    {
        while(TRUE)
        {
            memset(cBuff, 0, sizeof(cBuff));
            if (NULL == fgets(cBuff, 1024, pfData))
            {
                fclose(pfData);
                break;
            }

            if (!memcmp(cBuff, "Total", strlen("Total")))
            {
                mem_str2info(cBuff, aiMemInfo, MEM_MAX);
                fclose(pfData);
                break;
            }
        }
    }
    fMemUsage = (float)(aiMemInfo[MEM_USED])/(float)(aiMemInfo[MEM_TOTAL]);
    pstStatSys->ulMemRTUsage = (UINT32)(fMemUsage*100);

    memset(acNvramValue, 0, 32);
    if(0==nvram_get(acNvramValue, "MemPeakUsage", 32))
        pstStatSys->ulMemPeakUsage=atoi(acNvramValue);
    if(pstStatSys->ulMemPeakUsage < pstStatSys->ulMemRTUsage)
    {
        pstStatSys->ulMemPeakUsage=pstStatSys->ulMemRTUsage;
        sprintf(acNvramValue, "%d", pstStatSys->ulMemPeakUsage);
        nvram_set("MemPeakUsage", acNvramValue);
    }
    
    lprintf(LOG_INFO, "used:%u, total:%u, fMemUsage:%f, ulMemRTUsage:%u",
        aiMemInfo[MEM_USED],aiMemInfo[MEM_TOTAL],fMemUsage,pstStatSys->ulMemRTUsage);
    
    system("rm -f "SNMP_MEM);
    return TRUE;
}/* end app_get_sysinfo */

/*****************************************************************************
 Prototype    : get_wifi_vap_number
 Description  : 获取wifi卡的数目和vap的数目
 Input        : SSID_T *pstSsid
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2012/11/10
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
BOOL get_wifi_vap_number( SSID_T *pstSsid )
{
    int iVapLoop  = 0;
    char acWifiNum[NVRAM_SIZE] = {0};
    char acNvramName[NVRAM_SIZE] = {0};
    char acNvramValue[NVRAM_SIZE] = {0};
    char list[128] = {0};
    char vaps_list[32] = {0};
    
    if (NULL == pstSsid)
    {
        agent_printf(LOG_ERROR, "pstSsid is NULL");
        return FALSE;
    }

    /* 获取wifi卡的数目, 根据wifiNum */
    nvram_get(acWifiNum, NVRAM_WIFI_NUMBERS, NVRAM_LEN);
    pstSsid->ulWifiNum = atoi(acWifiNum);
    if (0 == pstSsid->ulWifiNum)
    {
        agent_printf(LOG_ERROR, "ulWifiNum is zero");
        return FALSE;
    }

    if(NvramResultFailed == nvram_get(list, NVRAM_VAP_LIST, sizeof(list)))
    {
        agent_printf(LOG_ERROR, "the vap list is null");
        pstSsid->ulSsidNum = 0;
        return TRUE;
    }

    memset(vaps_list, 0, sizeof(vaps_list));
    array_string_to_int(list, vaps_list, (int *)&pstSsid->ulSsidNum);
    
    /* 解析vap */
    for (iVapLoop = 0; iVapLoop < pstSsid->ulSsidNum; iVapLoop++)
    {
        sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_WLAN_ID, vaps_list[iVapLoop]);
        nvram_get(acNvramValue, acNvramName, NVRAM_LEN);
        pstSsid->astSsidInfo[iVapLoop].ulVapId = atoi(acNvramValue);

        sprintf(acNvramName, NVRAM_VAP"%d"NVRAM_VAP_WIFI_ID, vaps_list[iVapLoop]);
        nvram_get(acNvramValue, acNvramName, NVRAM_LEN);
        pstSsid->astSsidInfo[iVapLoop].ulWifiId= atoi(acNvramValue);
        
        pstSsid->astSsidInfo[iVapLoop].ulNvramId = vaps_list[iVapLoop];
    }
    
    return TRUE;
}/* end get_wifi_vap_number */

/*****************************************************************************
 Prototype    : app_get_trapdesinfo
 Description  : 获取trap服务器信息
 Input        : AP_CONFIG_TRAPDES_ENTRY_T *pstTrapDesInfo  
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2012/11/15
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL app_get_trapdesinfo(AP_CONFIG_TRAPDES_ENTRY_T *pstTrapDesInfo)
{
    CHAR acTrapDesIp[NVRAM_SIZE]={0};
    CHAR acTrapDesPort[NVRAM_SIZE]={0};
    CHAR acTrapSwitch[NVRAM_SIZE] = {0};
    UINT32 aulIp[4]={0};
    UINT32 ulLoop = 0;

    if ( NULL == pstTrapDesInfo )
    {
        lprintf(LOG_ERR, "pstTrapDesInfo is NULL.");
    }
    
    if ( 0 == nvram_get(acTrapDesIp, NVRAM_TRAPDESIP, NVRAM_SIZE)
        && '\0' != acTrapDesIp[0] )
    {
        sscanf(acTrapDesIp, IP_FMT, &aulIp[0],&aulIp[1],&aulIp[2],&aulIp[3]);
        for ( ulLoop = 0 ; ulLoop <4 ; ulLoop++ )
        {
            pstTrapDesInfo->aucTrapDesIPAddress[ulLoop] = aulIp[ulLoop];
        }
    }
    
    nvram_get(acTrapDesPort, NVRAM_TRAPDESPORT, NVRAM_SIZE);
    pstTrapDesInfo->ulTrapDesPort = atoi(acTrapDesPort);

    /* BEGIN: Added by zoucaihong, 2013/3/29 */
    nvram_get(pstTrapDesInfo->acTrapCommunity, NVRAM_TRAPCOMMUITY, NVRAM_SIZE);
    /* END: Added by zoucaihong, 2013/3/29 */
    nvram_get(acTrapSwitch, NVRAM_TRAPDSWTICH, NVRAM_SIZE);
    pstTrapDesInfo->ulSwtich = atoi(acTrapSwitch);
    
    lprintf(LOG_INFO, "acTrapDesIp:%s, ulTrapDesPort:%u", acTrapDesIp, pstTrapDesInfo->ulTrapDesPort);

    return TRUE;
}
#ifdef OPENWRT
BOOL app_get_vlaninfo(UINT32 *total, AP_CONFIG_VLAN_ENTRY_T *pstVlanInfo)
{
}

int app_set_vlaninfo(AP_CONFIG_VLAN_ENTRY_T *pstVlanInfo, int flag)
{
}
#else
/*****************************************************************************
 Prototype    : app_get_vlaninfo
 Description  : 获取VLAN配置信息
 Input        : UINT32 ulLoop                        
                AP_CONFIG_VLAN_ENTRY_T *pstVlanInfo  
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2012/11/23
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL app_get_vlaninfo(UINT32 *total, AP_CONFIG_VLAN_ENTRY_T *pstVlanInfo)
{
    int loop;
    char nvram_name[NVRAM_SIZE];
    char nvram_value[NVRAM_SIZE];
    char list[128];
    char vlans[32];

    if(NULL == pstVlanInfo)
    {
        lprintf(LOG_ERR, "the pstVlanInfo is null.");
        return FALSE;
    }

    if(NvramResultFailed == nvram_get(list, NVRAM_VLAN_LIST, sizeof(list)))
    {
        lprintf(LOG_INFO, "no vlan info");
        *total = 0;
        return TRUE;
    }

    memset(vlans, 0, sizeof(vlans));
    array_string_to_int(list, vlans, (int *)total);

    for(loop = 0; loop < *total; loop++)
    {
        pstVlanInfo[loop].ulIndex = loop+1;
        pstVlanInfo[loop].ulRowStatus = ENUM_ROWSTATUS_ACTIVE;
        
        sprintf(nvram_name, NVRAM_VLAN"%d_nvram_id", vlans[loop]);
        nvram_get(nvram_value, nvram_name, NVRAM_SIZE);
        pstVlanInfo[loop].ulNvramId = atoi(nvram_value);
        
        sprintf(nvram_name, NVRAM_VLAN"%d_name", vlans[loop]);
        nvram_get(nvram_value, nvram_name, NVRAM_SIZE);
        memcpy(pstVlanInfo[loop].acVlanName, nvram_value, strlen(nvram_value));

        sprintf(nvram_name, NVRAM_VLAN"%d_tag", vlans[loop]);
        nvram_get(nvram_value, nvram_name, NVRAM_SIZE);
        pstVlanInfo[loop].ulVlanId = atoi(nvram_value);

        sprintf(nvram_name, NVRAM_VLAN"%d_priority", vlans[loop]);
        nvram_get(nvram_value, nvram_name, NVRAM_SIZE);
        pstVlanInfo[loop].ulVlanPrio = atoi(nvram_value);

        lprintf(LOG_INFO, "ulVlanId:%u, "
        "acVlanName:%s, "
        "ulNvramId:%u, "
        "ulVlanPrio:%u",
        pstVlanInfo[loop].ulVlanId, 
        pstVlanInfo[loop].acVlanName,
        pstVlanInfo[loop].ulNvramId,
        pstVlanInfo[loop].ulVlanPrio);
    }

    return TRUE;
}

/*****************************************************************************
 Prototype    : app_set_vlaninfo
 Description  : 
 Input        : AP_CONFIG_VLAN_ENTRY_T *pstVlanInfo  
                int flag: 0. delete, 1. modify, 2. create                            
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/7/26
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
int app_set_vlaninfo(AP_CONFIG_VLAN_ENTRY_T *pstVlanInfo, int flag)
{
    int loop;
    int total;
    char nvram_name[NVRAM_SIZE];
    char nvram_value[NVRAM_SIZE];
    char list[128];
    char vlans[32];
    char command[128];
    
    if(NULL == pstVlanInfo)
    {
        lprintf(LOG_ERR, "the pstVlanInfo is null.");
        return FALSE;
    }

    lprintf(LOG_DEBUG, "vlan nvram id: %d, flag: %d",
        pstVlanInfo->ulNvramId, flag);

    if(0 == flag)
    {
        sprintf(nvram_name, NVRAM_VLAN"%d_nvram_id", pstVlanInfo->ulNvramId);
        nvram_unset(nvram_name);
        sprintf(nvram_name, NVRAM_VLAN"%d_name", pstVlanInfo->ulNvramId);
        nvram_unset(nvram_name);
        sprintf(nvram_name, NVRAM_VLAN"%d_tag", pstVlanInfo->ulNvramId);
        nvram_unset(nvram_name);
        sprintf(nvram_name, NVRAM_VLAN"%d_priority", pstVlanInfo->ulNvramId);
        nvram_unset(nvram_name);

        nvram_get(list, NVRAM_VLAN_LIST, sizeof(list));
        array_string_del_someone(list, pstVlanInfo->ulNvramId);
        nvram_set(NVRAM_VLAN_LIST, list);
        nvram_get(nvram_value, NVRAM_VLAN_TOTAL, NVRAM_SIZE);
        total = atoi(nvram_value)-1;
        sprintf(nvram_value, "%d", total);
        nvram_set(NVRAM_VLAN_TOTAL, nvram_value);
        

        // delete
        sprintf(command, "ifconfig br%d down", pstVlanInfo->ulNvramId);
        system(command);
        sprintf(command, "brctl delbr br%d", pstVlanInfo->ulNvramId);
        system(command);
        /* 2014/4/3 added by zhangping */
        sprintf(command, "vconfig rem "ETH0".%d", pstVlanInfo->ulVlanId);
        system(command);

        /* 删除VLAN需同时修改绑定该VLAN的VAP的配置 */
        del_vlan_to_vap(pstVlanInfo->ulVlanId);
    }else if(1 == flag)
    {
        sprintf(nvram_name, NVRAM_VLAN"%d_name", pstVlanInfo->ulNvramId);
        nvram_set(nvram_name, pstVlanInfo->acVlanName);
        sprintf(nvram_name, NVRAM_VLAN"%d_priority", pstVlanInfo->ulNvramId);
        sprintf(nvram_value, "%d", pstVlanInfo->ulVlanPrio);
        nvram_set(nvram_name, nvram_value);
    }else if(2 == flag)
    {
        if(NvramResultFailed == nvram_get(list, NVRAM_VLAN_LIST, 128))
        {
            lprintf(LOG_DEBUG, "the nvram has no this field, defualt nvram id 0");
            pstVlanInfo->ulNvramId = 1;
        }
        else
        {
            array_string_to_int(list, vlans, &total);
            for(loop = 1; loop <= VLAN_MAX; loop++)
            {
                if(loop != vlans[loop-1])
                {
                    pstVlanInfo->ulNvramId = loop;
                    lprintf(LOG_ERR, "the vlan nvram id %d", pstVlanInfo->ulNvramId);
                    break;
                }
            }
        }
    
        sprintf(nvram_name, NVRAM_VLAN"%d_nvram_id", pstVlanInfo->ulNvramId);
        sprintf(nvram_value, "%d", pstVlanInfo->ulNvramId);
        nvram_set(nvram_name, nvram_value);
        sprintf(nvram_name, NVRAM_VLAN"%d_name", pstVlanInfo->ulNvramId);
        nvram_set(nvram_name, pstVlanInfo->acVlanName);
        sprintf(nvram_name, NVRAM_VLAN"%d_tag", pstVlanInfo->ulNvramId);
        sprintf(nvram_value, "%d", pstVlanInfo->ulVlanId);
        nvram_set(nvram_name, nvram_value);
        sprintf(nvram_name, NVRAM_VLAN"%d_priority", pstVlanInfo->ulNvramId);
        sprintf(nvram_value, "%d", pstVlanInfo->ulVlanPrio);
        nvram_set(nvram_name, nvram_value);
        array_string_add_someone(list, pstVlanInfo->ulNvramId);
        nvram_set(NVRAM_VLAN_LIST, list);
        nvram_get(nvram_value, NVRAM_VLAN_TOTAL, NVRAM_SIZE);
        total = atoi(nvram_value)+1;
        sprintf(nvram_value, "%d", total);
        nvram_set(NVRAM_VLAN_TOTAL, nvram_value);

        // add
        sprintf(command, "brctl addbr br%d", pstVlanInfo->ulNvramId);
        system(command);
        /* 2014/4/3 moded by zhangping */
        sprintf(command, "vconfig add "ETH0" %d", pstVlanInfo->ulVlanId);
        system(command);
        sprintf(command, "brctl addif br%d "ETH0".%d", pstVlanInfo->ulNvramId, pstVlanInfo->ulVlanId);
        system(command);
        sprintf(command, "ifconfig br%d up && ifconfig "ETH0".%d up",
            pstVlanInfo->ulNvramId, pstVlanInfo->ulVlanId);
        system(command);
    }
    else
    {
        return FALSE;
    }
    nvram_commit();

    return TRUE;
}
#endif

/*****************************************************************************
 Prototype    : app_get_alarmconfig
 Description  : 获取CPU/内存告警配置信息
 Input        : AP_CONFIG_ALARM_T *pstAlarmConfig  
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2012/11/23
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL app_get_alarmconfig( AP_CONFIG_ALARM_T *pstAlarmConfig )
{
    CHAR acNvramName[NVRAM_SIZE]={0};
    CHAR acNvramValue[NVRAM_SIZE]={0};
    int iRet = 0;

    if ( NULL == pstAlarmConfig )
    {
        lprintf(LOG_ERR, "pstAlarmConfig is NULL.");
    }

    sprintf(acNvramName, "%s", NVRAM_ALARMCPULOADHIGH);
    iRet = nvram_get(acNvramValue, acNvramName, NVRAM_SIZE);
    if ( 0 == iRet )
    {
        pstAlarmConfig->ulCpuLoadHigh = atoi(acNvramValue);
    }

    sprintf(acNvramName, "%s", NVRAM_ALARMCPULOADLOW);
    iRet = nvram_get(acNvramValue, acNvramName, NVRAM_SIZE);
    if ( 0 == iRet )
    {
        pstAlarmConfig->ulCpuLoadLow = atoi(acNvramValue);
    }

    sprintf(acNvramName, "%s", NVRAM_ALARMMEMLOADHIGH);
    iRet = nvram_get(acNvramValue, acNvramName, NVRAM_SIZE);
    if ( 0 == iRet )
    {
        pstAlarmConfig->ulMemLoadHigh = atoi(acNvramValue);
    }

    sprintf(acNvramName, "%s", NVRAM_ALARMMEMLOADLOW);
    iRet = nvram_get(acNvramValue, acNvramName, NVRAM_SIZE);
    if ( 0 == iRet )
    {
        pstAlarmConfig->ulMemLoadLow = atoi(acNvramValue);
    }

    return TRUE;
}
#if 0
/* BEGIN: Added by zoucaihong, 2012/12/5 */
BOOL get_system_sysdescr(char *pcSysDescr)
{
	char strCmd[128] = {0};
	char temp_line[128]={0};
	FILE *f;

	sprintf(strCmd,  "uname -a > /tmp/uname.tmp");
	system( strCmd );
	if ((f = fopen("/tmp/uname.tmp" , "r+")) != NULL)
	{
		fgets(temp_line, 128, f);

		if ('\n' == temp_line[strlen(temp_line)-1])
		{
			temp_line[strlen(temp_line)-1] = 0;
		}

		memcpy(pcSysDescr, temp_line, sizeof(temp_line));
		
		fclose(f);
		return TRUE;
	} 

	fclose(f);
	return FALSE;
}
/*****************************************************************************
 Prototype    : app_get_system
 Description  : get the system group info of RFC1213
 Input        : SYSTEM_T *pstSystem  
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2012/12/5
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL app_get_system(SYSTEM_T *pstSystem)
{
    BOOL bRet = FALSE;
    CHAR acNvramName[NVRAM_SIZE] = {0};
    CHAR *szDlinkOid = VENDOR;

    /*get sysDescr*/
    bRet = get_system_sysdescr(pstSystem->acSysDescr);
    if ( !bRet )
    {
        lprintf(LOG_ERR, "get_system_sysdescr failed.\n");
    }

    /*get sysObjectID*/
    memcpy(pstSystem->acSysObjectID, szDlinkOid, strlen(szDlinkOid));

    /*get sysUpTime*/
    pstSystem->ulSysUpTime = app_get_uptime() * 100;

    /*get sysContact*/
    sprintf(acNvramName, "%s", NVRAM_SYSCONTACT);
    nvram_get(pstSystem->acSysContact, acNvramName, NVRAM_SIZE);

    /*get sysName*/
    sprintf(acNvramName, "%s", NVRAM_SYSNAME);
    nvram_get(pstSystem->acSysName, acNvramName, NVRAM_SIZE);

    /*get sysLocation*/
    sprintf(acNvramName, "%s", NVRAM_SYSLOCATION);
    nvram_get(pstSystem->acSysLocation, acNvramName, NVRAM_SIZE);

    /*get sysServices*/
    pstSystem->ulSysServices = 72;

    return TRUE;
}
/* END: Added by zoucaihong, 2012/12/5 */

/*****************************************************************************
 Prototype    : app_get_advance_5gprior
 Description  : 获取5G优先参数
 Input        : None
 Output       : None
 Return Value : int
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/3/11
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
BOOL app_get_advance_5gprior(AP_CONFIG_ADVANCE_5GPRIOR_T *pstAdvance5gPrior)
{
    char acNvramValue[32];
    if(!nvram_get(acNvramValue, NVRAM_DEV"prior_5g", 32))
    {
        pstAdvance5gPrior->ul5GPriSwitch = atoi(acNvramValue);
    }
    else
    {
        pstAdvance5gPrior->ul5GPriSwitch = 0;
    }
    if(!nvram_get(acNvramValue, NVRAM_DEV"prior_5g_delayTime", 32))
    {
        pstAdvance5gPrior->ul2dot4GDelayTime = atoi(acNvramValue);
    }
    else
    {
        pstAdvance5gPrior->ul2dot4GDelayTime = 3;
    }
    if(!nvram_get(acNvramValue, NVRAM_DEV"prior_5g_StaTimeOut", 32))
    {
        pstAdvance5gPrior->ulStaTimesOut = atoi(acNvramValue);
    }
    else
    {
        pstAdvance5gPrior->ulStaTimesOut = 180;
    }
    return TRUE;
}

BOOL app_set_advanced_5gprior(AP_CONFIG_ADVANCE_5GPRIOR_T *pstAdvance5gPrior)
{
    int loop;
    int total;
    int wifiId=0xffffffff;
    char list[128];
    char vaps[32];
    char nvram_name[32];
    char nvram_value[32];
    char wifi_name[32];

    if(NvramResultFailed == nvram_get(list, NVRAM_VAP_LIST, sizeof(list)))
    {
        lprintf(LOG_ERR, "get NVRMA_VAP_LIST failed for nvram.");
        return FALSE;
    }
    array_string_to_int(list, vaps, &total);
    for(loop = 0; loop < total; loop++)
    {
        sprintf(nvram_name, NVRAM_VAP"%d"NVRAM_VAP_WIFI_ID, vaps[loop]);
        if(NvramResultFailed == nvram_get(nvram_value, nvram_name, NVRAM_LEN))
        {
            lprintf(LOG_ERR, "get %s failed for nvram.", nvram_name);
            return FALSE;
        }
        if ( wifiId == atoi(nvram_value) )
        {
            continue;
        }
        wifiId = atoi(nvram_value);
        sprintf(wifi_name, "wifi%d", wifiId);
        wlan_set_5g(wifi_name, pstAdvance5gPrior->ul5GPriSwitch,
            pstAdvance5gPrior->ul2dot4GDelayTime,
            pstAdvance5gPrior->ulStaTimesOut);
    }
    return TRUE;
}

/*****************************************************************************
 Prototype    : app_get_advance_11nprior
 Description  : 
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/3/11
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
BOOL app_get_advance_11nprior(AP_CONFIG_ADVANCE_11NPRIOR_T *pstAdvance11nPrior)
{
    char acNvramValue[32];
    if(!nvram_get(acNvramValue, NVRAM_DEV"prior_11n", 32))
    {
        pstAdvance11nPrior->ul11nPriSwitch = atoi(acNvramValue);
    }
    else
    {
        pstAdvance11nPrior->ul11nPriSwitch = 0;
    }
    if(!nvram_get(acNvramValue, NVRAM_DEV"prior_11n_delayTime", 32))
    {
        pstAdvance11nPrior->ul11bgDelayTime = atoi(acNvramValue);
    }
    else
    {
        pstAdvance11nPrior->ul11bgDelayTime = 3;
    }
    if(!nvram_get(acNvramValue, NVRAM_DEV"prior_11n_StaTimeOut", 32))
    {
        pstAdvance11nPrior->ulStaTimesOut = atoi(acNvramValue);
    }
    else
    {
        pstAdvance11nPrior->ulStaTimesOut = 180;
    }
    return TRUE;
}/* end app_get_advance_11nprior */

BOOL app_set_advanced_11nprior(AP_CONFIG_ADVANCE_11NPRIOR_T *pstAdvance11nPrior)
{
    int loop;
    int total;
    int wifiId=0xffffffff;
    char list[128];
    char vaps[32];
    char nvram_name[32];
    char nvram_value[32];
    char wifi_name[32];

    if(NvramResultFailed == nvram_get(list, NVRAM_VAP_LIST, sizeof(list)))
    {
        lprintf(LOG_ERR, "get NVRMA_VAP_LIST failed for nvram.");
        return FALSE;
    }
    array_string_to_int(list, vaps, &total);
    for(loop = 0; loop < total; loop++)
    {
        sprintf(nvram_name, NVRAM_VAP"%d"NVRAM_VAP_WIFI_ID, vaps[loop]);
        if(NvramResultFailed == nvram_get(nvram_value, nvram_name, NVRAM_LEN))
        {
            lprintf(LOG_ERR, "get %s failed for nvram.", nvram_name);
            return FALSE;
        }
        if ( wifiId == atoi(nvram_value) )
        {
            continue;
        }
        wifiId = atoi(nvram_value);
        sprintf(wifi_name, "wifi%d", wifiId);
        wlan_set_11n(wifi_name, pstAdvance11nPrior->ul11nPriSwitch,
        pstAdvance11nPrior->ul11bgDelayTime,
        pstAdvance11nPrior->ulStaTimesOut);
    }
    return TRUE;
}

/* BEGIN: Added by zoucaihong, 2013/5/13 */
BOOL app_get_ap_legalap( AP_CONFIG_LEGALAP_ENTRY_T *pstSysLegalApConfig, UINT32 *pulRecNum)
{
    int loop, loop_j;
    char nvramName[32];
    char bssid[32];
    int ibssid[6];
    int index;

    if(NULL==pstSysLegalApConfig)
        return FALSE;

    index=0;
    for(loop=0;loop<16;loop++)
    {
        sprintf(nvramName, "legap_%d_oui", loop);
        if(0!=nvram_get(bssid, nvramName, 32))
        {
            continue;
        }
        sscanf(bssid, "%02x:%02x:%02x:%02x:%02x:%02x",
            &ibssid[0],&ibssid[1],&ibssid[2],&ibssid[3],&ibssid[4],&ibssid[5]);
        for(loop_j=0;loop_j<6;loop_j++)
        {
            pstSysLegalApConfig[index].aucLegalApBssid[loop_j] = ibssid[loop_j];
        }
        sprintf(nvramName, "legap_%d_ssid", loop);
        nvram_get(pstSysLegalApConfig[index].acLegalApSsid, nvramName, 32);
        pstSysLegalApConfig[index].ulIndex = index+1;
        index++;
    }
    *pulRecNum = index;
    return TRUE;
}
BOOL app_get_ap_flowctl(AP_CONFIG_FLOWCTL_T *pstSysFlowCtl)
{
    char acCtorl[32]={0};
    char acUpload[32]={0};
    char acDownload[32]={0};
    char acStaUpload[32]={0};
    char acStaDownload[32]={0};

    nvram_get(acCtorl, "ap_flow_switch", 32);
	nvram_get(acUpload, "ap_upload", 32);
	nvram_get(acDownload, "ap_download", 32);
	nvram_get(acStaUpload, "ap_sta_upload", 32);
	nvram_get(acStaDownload, "ap_sta_download", 32);

    pstSysFlowCtl->ulFlowSwitch = atoi(acCtorl);
    pstSysFlowCtl->ulApUplinkMaxRate = atoi(acUpload);
    pstSysFlowCtl->ulApDwlinkMaxRate= atoi(acDownload);
    pstSysFlowCtl->ulStaUplinkMaxRate = atoi(acStaUpload);
    pstSysFlowCtl->ulStaDwlinkMaxRate= atoi(acStaDownload);
	
    return TRUE;
}
#endif

VOID app_get_config_version(CHAR *pcConfigVersion)
{

    CHAR acNvramName[NVRAM_NAME_MAX_LEN]={0};
    CHAR acNvramValue[NVRAM_VALUE_MAX_LEN]={0};
    int iRet = 0;

    if ( NULL == pcConfigVersion )
    {
        lprintf(LOG_ERR, "pcConfigVersion is NULL.");
    }

    sprintf(acNvramName, "%s", NVRAM_CONFIGVERSION);
    iRet = nvram_get(acNvramValue, acNvramName, NVRAM_VALUE_MAX_LEN);
    if ( 0 == iRet )
    {
        memcpy(pcConfigVersion, acNvramValue, NVRAM_VALUE_MAX_LEN);
    }
    
    return;    
}



int app_vlan_modify(int br_id, int br_id_old, char *vap_name)
{
    char command[128];

    sprintf(command, "brctl delif br%d %s", br_id_old, vap_name);
    system(command);
    printf("shell exec %s\n", command);

    sprintf(command, "brctl addif br%d %s", br_id, vap_name);
    system(command);
    printf("shell exec %s\n", command);
    return TRUE;
}

int app_vlan_eabled(int br_id, int state, char *vap_name)
{
    char command[128];

    printf("br_id: %d, state: %d\n", br_id, state);

    if(0 == state)
    {
        sprintf(command, "brctl delif br%d %s", br_id, vap_name);
        system(command);
        printf("shell exec %s\n", command);

        sprintf(command, "brctl addif br0 %s", vap_name);
        system(command);
        printf("shell exec %s\n", command);
    }
    else if(1 == state)
    {
        sprintf(command, "brctl delif br0 %s", vap_name);
        system(command);
        printf("shell exec %s\n", command);
    
        sprintf(command, "brctl addif br%d %s", br_id, vap_name);
        system(command);
        printf("shell exec %s\n", command);
    }

    return TRUE;
}
#if 0
int leg_get_all( struct leg_ap *leg )
{
    int loop;
    char nvramName[32];
    int index;

    if(NULL==leg)
        return FALSE;

    index=0;
    for(loop=0;loop<LEG_AP_NUMS;loop++)
    {
        sprintf(nvramName, "legap_%d_oui", loop);
        if(0!=nvram_get(leg[index].bssid, nvramName, 32))
        {
            continue;
        }
        sprintf(nvramName, "legap_%d_ssid", loop);
        nvram_get(leg[index].ssid, nvramName, 32);
        sprintf(nvramName, "legap_%d_nvram_id", loop);
        nvram_get(leg[index].id, nvramName, 32);
        index++;
    }
    
    return index;
}
#endif
/* END: Added by zoucaihong, 2013/5/13 */

