/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : interface_route.c
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2012/11/7
  Last Modified :
  Description   : snmp模块和桥及有线驱动接口
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

#if 0
int str2info(char *pcBuf, unsigned int *pulDevInfo, int iMax)
{
	int i = 0;
	char acCurNum[64];
	int iCurNumLoc = 0;
	int iConvertCnt = 0;
	int iByteNum = 0, iDigitBegin = 0;

	if(NULL == pcBuf)
	{
		return -1;
	}
    memset(acCurNum, 0, 64);
	iByteNum = strlen(pcBuf);
	for(i = 0; i < iByteNum; i++)
	{
		if(!isdigit(pcBuf[i]))
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
			acCurNum[iCurNumLoc ++] = pcBuf[i];
			continue;
		}
num_convert:
		pulDevInfo[iConvertCnt] = atoi(acCurNum);
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
}
/*****************************************************************************
 Prototype    : get_interfaces_phyaddress
 Description  : 获取AP有线接口MAC地址
 Input        : UINT8 *pucPhyAddress  
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2012/12/10
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL get_interfaces_phyaddress(UINT8 *pucPhyAddress)
{
    CHAR acCmd[128] = {0};
    FILE *pfData = NULL;
    CHAR acBuf[128] = {0};
    UINT32 aulMac[6] = {0};
    UINT32 ulLoopNo = 0;

    /* 2014/4/3 moded by zhangping */
    sprintf(acCmd, "ifconfig "ETH0" | grep HWaddr | cut -f 2 -d 'H' | cut -b 7-24 > /tmp/ifphyaddress.tmp");
    system(acCmd);

    if ( NULL == (pfData = fopen("/tmp/ifphyaddress.tmp", "r")) )
    {
        agent_printf(LOG_ERR, "open ifphyaddress.tmp failed.\n");
        return FALSE;
    }

    if ( NULL == fgets(acBuf, 128, pfData) )
    {
        fclose(pfData);
        system("rm -rf /tmp/ifphyaddress.tmp");
        agent_printf(LOG_ERR, "open ifphyaddress.tmp failed.\n");
        return FALSE;
    }

    fclose(pfData);
    system("rm -rf /tmp/ifphyaddress.tmp");
    sscanf(acBuf, MAC_FMT, &aulMac[0], &aulMac[1], &aulMac[2],
        &aulMac[3], &aulMac[4],&aulMac[5]);
    for ( ulLoopNo = 0 ; ulLoopNo < 6 ; ulLoopNo++ )
    {
        pucPhyAddress[ulLoopNo] = aulMac[ulLoopNo];
    }
    lprintf(LOG_INFO, "ifPhyAddress:"MAC_FMT, MAC_ARG(pucPhyAddress));

    return TRUE;
}
/*****************************************************************************
 Prototype    : get_interfaces_info
 Description  : 获取AP有线接口速率、管理状态、运行状态、MTU
 Input        : UINT32 *pulIfSpeed        
                UINT32 *pulIfAdminStatus  
                UINT32 *pulIfOperStatus   
                UINT32 *pulIfMtu          
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2012/12/10
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL get_interfaces_info(UINT32 *pulIfSpeed,
    UINT32 *pulIfAdminStatus,
    UINT32 *pulIfOperStatus,
    UINT32 *pulIfMtu)
{
    CHAR acCmd[128] = {0};
    FILE *pfData = NULL;
    CHAR acBuf[128] = {0};
    CHAR *pcPos = NULL;
    CHAR acIfName[32] = {0};
    CHAR acIfStatus[32] = {0};
    
    /*
    * get ifSpeed
    */
    sprintf(acCmd, "phyctl getstatus "ETH0" > /tmp/ifspeed.tmp");/* 2014/4/3 moded by zhangping */
    system(acCmd);
    
    if ( NULL == (pfData = fopen("/tmp/ifspeed.tmp", "r")) )
    {
        agent_printf(LOG_ERR, "open /tmp/ifspeed.tmp failed.\n");
        return FALSE;
    }

	memset(acBuf, 0, 128);    
    if (NULL == fgets(acBuf, 128, pfData))
    {
        fclose(pfData);
        system("rm -f /tmp/ifspeed.tmp");
        agent_printf(LOG_ERROR, "read ifspeed.tmp failed.\n");
        return FALSE;
    }
    
    fclose(pfData);
    system("rm -f /tmp/ifspeed.tmp");
    sscanf(acBuf, "%s %s %dMbsp", acIfName, acIfStatus, pulIfSpeed);

    /*
    * get if adminstatus,operstatus,mtu
    */
    sprintf(acCmd, "ifconfig "ETH0" | grep MTU > /tmp/ifinfo.tmp");/* 2014/4/3 moded by zhangping */
    system(acCmd);
    
    if ( NULL == (pfData = fopen("/tmp/ifinfo.tmp", "r")) )
    {
        agent_printf(LOG_ERR, "open /tmp/ifinfo.tmp failed.\n");
        return FALSE;
    }
    
    if (NULL == fgets(acBuf, 128, pfData))
    {
        fclose(pfData);
        system("rm -f /tmp/ifinfo.tmp");
        agent_printf(LOG_ERROR, "read ifinfo.tmp failed.\n");
        return FALSE;
    }
    
    fclose(pfData);
    system("rm -f /tmp/ifinfo.tmp");

    /*get ifAdminStatus*/
    pcPos = strstr(acBuf, "UP");
    if ( NULL != pcPos )
    {
        *pulIfAdminStatus = 1;
    }
    else
    {
        *pulIfAdminStatus = 2;
    }

    /*get ifOperStatus*/
    pcPos = NULL;
    pcPos = strstr(acBuf, "RUNNIN");
    if ( NULL != pcPos )
    {
        *pulIfOperStatus = 1;
    }
    else
    {
        *pulIfOperStatus = 2;
    }

    /*get ifMtu*/
    pcPos = NULL;
    pcPos = strstr(acBuf, "MTU");
    sscanf(pcPos, "MTU:%d", pulIfMtu);

    agent_printf(LOG_INFO, "adminstatus:%u, operstatus:%u, mtu:%u, speed:%uMbps\n",
        *pulIfAdminStatus,
        *pulIfOperStatus,
        *pulIfMtu,
        *pulIfSpeed);
    
    return TRUE;
}
char * get_convert_start(char *pcBuf)
{
	if(NULL == pcBuf)
	{
		return NULL;
	}

	while(*pcBuf++ != '\0')
	{
		if(*pcBuf == ':')
		{
			return pcBuf;
		}
	}

	return NULL;
}

/*****************************************************************************
 Prototype    : get_interfaces_stats
 Description  : 获取AP有线接口字节数、包数统计
 Input        : INTERFACES_ENTRY_T *pstInterfaces  
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2012/12/10
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL get_interfaces_stats( INTERFACES_ENTRY_T *pstInterfaces)
{
	FILE *pfData = NULL;
	char acBuf[1024];
	char *pcData = NULL;
    char *pcPos = NULL;
	UINT32 aulDevInfo[DEV_STAT_MAX];
	char acCmd[128]={0};
    UINT32 ulLastDevOctets = 0;
    UINT32 ulIfOctets = 0;
    UINT32 ulDelta = 0;
    UINT32 ulDiff = 0;

    /*
    * get ifStats
    */
	if(NULL == (pfData = fopen("/proc/net/dev", "r")))
	{
		agent_printf(LOG_ERR, "open /proc/net/dev failure.\n");
		return FALSE;
	}

	memset(acBuf, 0, 1024);
	while(1)
	{
		fgets(acBuf, 1024, pfData);
        pcPos = strstr(acBuf, ETH0);/* 2014/4/3 moded by zhangping */
		if( NULL != pcPos )
		{
			/* convert string to number info */
			pcData = get_convert_start(acBuf);
			str2info(pcData, aulDevInfo, DEV_STAT_MAX);

            /* 上次内核统计数据 */
            ulLastDevOctets = pstInterfaces->ulIfInOctets%0x7FFFFFFF;
            /* 上次mib统计数据 */
            ulIfOctets = pstInterfaces->ulIfInOctets;
            /* 内核出现2G翻转 */
            if ( ulLastDevOctets > aulDevInfo[R_BYTE] )
            {
                ulDelta = 0x7FFFFFFF+aulDevInfo[R_BYTE]-ulLastDevOctets;
                ulDiff = 0xFFFFFFFF - ulIfOctets;
                if ( ulDiff < ulDelta )
                {
                    pstInterfaces->ulIfInOctets = ulDelta-ulDiff;
                }
                else
                {
                    pstInterfaces->ulIfInOctets += ulDelta;
                }
            }
            else
            {
                ulDelta = aulDevInfo[R_BYTE]-ulLastDevOctets;
                pstInterfaces->ulIfInOctets += ulDelta;
            }
            pstInterfaces->ulIfInUcastPkts = aulDevInfo[R_PACKET] - aulDevInfo[R_MULTICAST];
            pstInterfaces->ulIfInNUcastPkts = aulDevInfo[R_MULTICAST];
            pstInterfaces->ulIfInDiscards = aulDevInfo[R_DROP];
            pstInterfaces->ulIfInErrors = aulDevInfo[R_ERR];

            /* 上次内核统计数据 */
            ulLastDevOctets = pstInterfaces->ulIfOutOctets%0x7FFFFFFF;
            /* 上次mib统计数据 */
            ulIfOctets = pstInterfaces->ulIfOutOctets;
            /* 内核出现2G翻转 */
            if ( ulLastDevOctets > aulDevInfo[T_BYTE] )
            {
                ulDelta = 0x7FFFFFFF+aulDevInfo[T_BYTE]-ulLastDevOctets;
                ulDiff = 0xFFFFFFFF - ulIfOctets;
                if ( ulDiff < ulDelta )
                {
                    pstInterfaces->ulIfOutOctets = ulDelta-ulDiff;
                }
                else
                {
                    pstInterfaces->ulIfOutOctets += ulDelta;
                }
            }
            else
            {
                ulDelta = aulDevInfo[T_BYTE]-ulLastDevOctets;
                pstInterfaces->ulIfOutOctets += ulDelta;
            }
            pstInterfaces->ulIfOutUcastPkts = aulDevInfo[T_PACKET] - aulDevInfo[T_MULTICAST];
            pstInterfaces->ulIfOutNUcastPkts = aulDevInfo[T_MULTICAST];
            pstInterfaces->ulIfOutDiscards = aulDevInfo[T_DROP];
            pstInterfaces->ulIfOutErrors = aulDevInfo[T_ERR];

            break;
		}

		if(feof(pfData))
		{
			break;
		}
	}

	fclose(pfData);

    /*
    * get ifOutQLen
    */
	sprintf(acCmd, "ifconfig "ETH0" | grep txqueuelen > /tmp/iftxqueuelen.tmp");/* 2014/4/3 moded by zhangping */
	system(acCmd);

    pfData = fopen( "/tmp/iftxqueuelen.tmp", "r");
	if(NULL == pfData)
	{
		agent_printf(LOG_ERR, "open /tmp/iftxqueuelen.tmp failed.\n");
		return FALSE;
	}

	memset(acBuf, 0, 1024);
    if ( NULL == fgets(acBuf, 1024, pfData) )
    {
        fclose(pfData);
        system("rm -rf /tmp/iftxqueuelen.tmp");
        agent_printf(LOG_ERR, "read iftxqueuelen.tmp failed.\n");
        return FALSE;
    }

    fclose(pfData);
    system("rm -rf /tmp/iftxqueuelen.tmp");
    
    pcPos = NULL;
    pcPos = strstr(acBuf, "txqueuelen");
    sscanf(pcPos, "txqueuelen:%d", &pstInterfaces->ulIfOutQLen);

	return TRUE;
}
/*****************************************************************************
 Prototype    : route_get_interfaces
 Description  : 获取AP有线接口信息
 Input        : INTERFACES_ENTRY_T *pstInterfaces  
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2012/12/10
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL route_get_interfaces( INTERFACES_ENTRY_T *pstInterfaces )
{
    BOOL bRet = FALSE;
    
    pstInterfaces->ulIfIndex = 1;
    sprintf(pstInterfaces->acIfDescr, ETH0);/* 2014/4/3 moded by zhangping */
    pstInterfaces->ulIfType = 6;

    /*get ifPhyAddress*/
    bRet = get_interfaces_phyaddress(pstInterfaces->aucIfPhysAddress);
    if ( !bRet )
    {
        agent_printf(LOG_ERR, "get_interfaces_phyaddress failed.\n");
    }

    /*get ifAdminStatus,ifOperStatus,ifMtu*/
    bRet = get_interfaces_info( &pstInterfaces->ulIfSpeed,
                                &pstInterfaces->ulIfAdminStatus,
                                &pstInterfaces->ulIfOperStatus,
                                &pstInterfaces->ulIfMtu);
    if ( !bRet )
    {
        agent_printf(LOG_ERR, "get_interfaces_info failed.\n");
        return FALSE;
    }

    /*get interfaces in/out octets/packets stat*/
    bRet = get_interfaces_stats(pstInterfaces);
    if ( !bRet )
    {
        agent_printf(LOG_ERR, "get_interfaces_stats failed.\n");
        return FALSE;
    }

    /*this two do not know how to stats, just set them 0*/
    pstInterfaces->ulIfInUnknownProtos = 0;
    //memcpy(pstInterfaces->aulIfSpecific, &nullOid, (UINT32)nullOidLen);

    return TRUE;
}
#endif

