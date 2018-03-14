/******************************************************************************

  Copyright (C), 2010-2011, GBCOM Co., Ltd.

 ******************************************************************************
  File Name     : aglink_temp.c
  Version       : Initial Draft
  Author        : lvjianlin
  Created       : 2014/11/19
  Last Modified :
  Description   : aglink.c
  Function List :
  History       :
  1.Date        : 2014/11/19
    Author      : lvjianlin
    Modification: Created file

******************************************************************************/
#include <sys/sysinfo.h>
#include <errno.h>
#include <net/if_arp.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include "aglink.h"
#include "ag_module.h"
//#include "agent_type.h"
#include "nvram_config.h"

#ifndef GW_X86
#if defined OPENWRT
#include "pal_wifi.h"
#include "pal_sys.h"
#include "pal_common.h"
#else
#include "gbcom_wifi_pal.h"
#endif
#endif

/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/
extern struct list_head gstIPClist;
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

/* 
 * ===  FUNCTION  =========================================
 *  Name:  aglink_mac_hashfn
 *  Description: 根据mac进行hash查找 
 *  Input : index:序号
 *  Output: none
 *  Return: hash链头部 
 * ====================================================
 */
inline struct hlist_head * aglink_mac_hashfn(u8 *mac, struct hlist_head list[],
	u32 hash_size)
{
	int	x;
	int	index;
	
	x = mac[0];
	x = (x << 2) ^ mac[1];
	x = (x << 2) ^ mac[2];
	x = (x << 2) ^ mac[3];
	x = (x << 2) ^ mac[4];
	x = (x << 2) ^ mac[5];

	x ^= x >> 8;

	index = x & (hash_size - 1);

	return &(list[index]);
}

static s32 aglink_get_free_info(AGLINK_DISK_INFO *pstDiskInfo)
{
	struct sysinfo info;
	sysinfo(&info);

	/* Kernels prior to 2.4.x will return info.mem_unit==0, so cope... */
	if (info.mem_unit==0) {
		info.mem_unit=1;
	}
	if ( info.mem_unit == 1 ) {
		info.mem_unit=1024;

		/* TODO:  Make all this stuff not overflow when mem >= 4 Gib */
		info.totalram/=info.mem_unit;
		info.freeram/=info.mem_unit;
#ifndef __uClinux__
		info.totalswap/=info.mem_unit;
		info.freeswap/=info.mem_unit;
#endif
		info.sharedram/=info.mem_unit;
		info.bufferram/=info.mem_unit;
	} else {
		info.mem_unit/=1024;
		/* TODO:  Make all this stuff not overflow when mem >= 4 Gib */
		info.totalram*=info.mem_unit;
		info.freeram*=info.mem_unit;
#ifndef __uClinux__
		info.totalswap*=info.mem_unit;
		info.freeswap*=info.mem_unit;
#endif
		info.sharedram*=info.mem_unit;
		info.bufferram*=info.mem_unit;
	}

	pstDiskInfo->total = info.totalram+info.totalswap;
	pstDiskInfo->used = (info.totalram-info.freeram)+(info.totalswap-info.freeswap);

	AGLINK_LOG(LOG_INFO, "total:%d used:%d", pstDiskInfo->total, pstDiskInfo->used);
	
	return TRUE;
}

static s32 aglink_get_cpu_info(AGLINK_CPU_RATE  *pstCPURate)
{
	FILE *pfile = NULL;
	u8 ucBuffer[256] = {0};
	static AGLINK_CPU_INFO stpre = {0};
	AGLINK_CPU_INFO stCur = {0};
	u64 uiTotal = 0;
	
	pfile = fopen(FILE_PROC_STAT, "r");
	if(NULL == pfile)
	{
		AGLINK_LOG(LOG_ALERT, "Open file "FILE_PROC_STAT"error(%d)", errno);
		return FALSE;
	}
	if(!fgets(ucBuffer, sizeof(ucBuffer), pfile))
	{
		AGLINK_LOG(LOG_ALERT, "Read file error(%d)", errno);
		fclose(pfile);
		return FALSE;
	}
	fclose(pfile);

	sscanf(ucBuffer, "cpu %Lu %Lu %Lu %Lu %Lu %Lu %Lu",
		&stCur.user, &stCur.nice, &stCur.system, &stCur.idle, &stCur.iowait, &stCur.irq, &stCur.softirq, &stCur.stealstolen, &stCur.guest);

	/*Total*/
	uiTotal = (stCur.user + stCur.nice + stCur.system + stCur.idle + stCur.iowait + stCur.irq + stCur.softirq + stCur.stealstolen + stCur.guest) - \
		(stpre.user + stpre.nice + stpre.system + stpre.idle + stpre.iowait + stpre.irq + stpre.softirq + stpre.stealstolen + stpre.guest);

	if(uiTotal < 1 )
	{
		uiTotal = 1;
	}

	/*cpu rate*/
	pstCPURate->cpu_rate = 100*(uiTotal - stCur.idle + stpre.idle)/uiTotal;
	
	/*system*/
	pstCPURate->system = 100*(stCur.system - stpre.system)/uiTotal;

	/*user*/
	pstCPURate->user = 100*(stCur.user - stpre.user)/uiTotal;

	/*nice*/
	pstCPURate->nice = 100*(stCur.nice - stpre.nice)/uiTotal;

	/*idle*/
	pstCPURate->idle = 100*(stCur.idle - stpre.idle)/uiTotal;
	
	memcpy(&stpre, &stCur, sizeof(AGLINK_CPU_INFO));

	AGLINK_LOG(LOG_INFO, "CPU: %d (system=%d user=%d nice=%d idle=%d)", 
		pstCPURate->cpu_rate, pstCPURate->system, pstCPURate->user, pstCPURate->nice, pstCPURate->idle);
	
	return TRUE;
}

s32 aglink_string_to_num(char *pcBuf, unsigned long *pulDevInfo, int iMax)
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

	iByteNum = strlen(pcBuf);
	for(i = 0; i < iByteNum; i++)
	{
		//if(!isdigit(pcBuf[i]))
		if(( pcBuf[i]>=0x30) && ( pcBuf[i]<=0x39))
		{
			if(0 == iDigitBegin)
			{
				iDigitBegin = 1;
			}
			acCurNum[iCurNumLoc ++] = pcBuf[i];
			continue;
		}
		else
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
num_convert:
		/* 2010-11-1 fengjing modify */
		/* 2011-09-02 fengjing modify : strtoul->my_strtoull */
		pulDevInfo[iConvertCnt] = strtoul(acCurNum, NULL, 10);
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

static s32 aglink_get_wan_stat(u8 *ifname, AGLINK_WAN_STAT *pstWanstat)
{
	FILE *pfile = NULL;
	char *cp, *tp;
	u8 ucbuff[256] = {0}, uctmp[128] = {0};
	u64 tmp[7] = {0};
	
	
	pfile = fopen(FILE_PROC_NET_DEV, "r");
	if(NULL == pfile)
	{
		AGLINK_LOG(LOG_ALERT, "Open file "FILE_PROC_NET_DEV"errno(%d)", errno);
		return 0;
	}

	while(fgets(ucbuff, sizeof(ucbuff), pfile))
	{
		for (cp = ucbuff; isspace(*cp); cp++)

		if (cp[0] != ifname[0])
                continue;

		for (tp = cp; *tp != ':' && *tp; tp++)
			;
		if (*tp == ':')
		{
			*tp++ = '\0';
               	if (strcmp(cp, ifname) != 0)
                    		continue;
			AGLINK_LOG(LOG_INFO, "tp:%s", tp);
			/*sscanf(tp, "%lld %lld%lld %lld %lld %lld %lld %lld %lld %s",
                    		pstWanstat->RecBytes, tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5], tmp[6], pstWanstat->SendBytes, uctmp);
			*/
			unsigned long g_aulDevInfo[16] = {0};
			aglink_string_to_num(tp, g_aulDevInfo, 16);
			AGLINK_LOG(LOG_INFO, "g_aulDevInfo[0]:%lu  g_aulDevInfo[8]:%lu\n", g_aulDevInfo[0], g_aulDevInfo[8]);
			pstWanstat->RecBytes = g_aulDevInfo[0];
			pstWanstat->SendBytes = g_aulDevInfo[8];
			AGLINK_LOG(LOG_INFO, "WAN stat: RecvBytes:%lu SendBytes:%lu", pstWanstat->RecBytes, pstWanstat->SendBytes);
                	fclose(pfile);
                	return 1;
		}
		
	}
	
	return TRUE;
}

static s32 aglink_get_dev_model_id()
{
#if (defined(GW_X86) || defined(GW_ARM) || defined(GW_AC))  //add by cj
		FILE *fFactory;
		char temp_line[0x100];
		int IModelid = 7001002;
		
		if ((fFactory = fopen("/factory/.factory" , "r+")) == NULL)
		{
			return IModelid;
		}
		if(NULL == (fgets(temp_line, 0x100, fFactory)))
		{
			fclose (fFactory);
			return IModelid;
		}
		fclose (fFactory);
		if ( NULL != strstr(temp_line,"CGW3000") )
		{
			IModelid = 7001003;
		}
		AGLINK_LOG(LOG_INFO, "CONFIG_X86 is %s %d", temp_line, IModelid);
		return IModelid;

#elif defined OPENWRT
            /*from factory config*/
            char dev_model_id[32];
            if(!factory_get(dev_model_id,DEVICE_TYPE,32))
                return 7005001;
            else 
                return atoi(dev_model_id);       
#else
		FILE *pf;
		char buf[512];
		char *id;
	
		pf = popen("aptool getdevtype", "r");
		if(NULL == pf)
		{
			AGLINK_LOG(LOG_ERR, "get devtype failed, default 9001001");
			return 9001001;
		}
	
		memset(buf, 0, sizeof(buf));
		fread(buf, sizeof(buf), 1, pf);
		pclose(pf);
		if(NULL == (id = strstr(buf, ":")))
		{
			AGLINK_LOG(LOG_ERR, "get devtype failed, default 9001001");
			return 9001001;
		}
		
		return atoi(++id);
#endif
}

/*****************************************************************************
 Prototype    : aglink_get_ap_work_mode
 Description  : get ap_work_mode
 Input        : void  
 Output       : 
 			0: fit
 			1: fat
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2015/3/30
    Author       : lvjianlin
    Modification : Created function

*****************************************************************************/
s32 aglink_get_ap_work_mode(void)
{
	s8 nvram_value[NVRAM_LEN] = "0";
	s32 ap_work_mode = 0 ;

	/*AP型网关*/
	if(IS_DEV_GW())
	{	
		#ifndef GW_X86
		AGLINK_LOG(LOG_INFO, "AP-GW");
		return AGLINK_FAT_AP;
		#endif
	}
	
	if(0 == AGLINK_CONFIG_GET(nvram_value, AP_WORK_MODE, NVRAM_LEN))
	{	
		AGLINK_LOG(LOG_INFO, "ap_work_mode:%s", nvram_value);
		return atoi(nvram_value);
	}
	else
	{
		AGLINK_LOG(LOG_INFO, "use default mode:0");
		return AGLINK_FIT_AP;
	}
	return TRUE;
}

int aglink_get_ifmac(u8 *mac, s8 *port)
{
    s32 skdf;
    struct ifreq ifr;

    skdf = socket(AF_INET, SOCK_DGRAM, 0);
    if(-1 == skdf){
        AGLINK_LOG(LOG_ERR, "creat socket failed");
        return -1;
    }

    memset(&ifr, 0, sizeof(struct ifreq));
    strncpy(ifr.ifr_name, port, IFNAMSIZ);
    ifr.ifr_name[IFNAMSIZ -1] = '\0';

    if(ioctl(skdf, SIOCGIFHWADDR, &ifr)){
        AGLINK_LOG(LOG_ERR, "ioctl's SIOCGIFHWADDR(%x) get %s mac faile, error: %s", 
            SIOCGIFHWADDR, port, strerror(errno));
        close(skdf);
        return -1;
    }
    close(skdf);
	
    memcpy(mac, (u8 *)ifr.ifr_hwaddr.sa_data, MAC_LEN);
    return 0;
}

s32 aglink_analysis_product_info(AGLINK_WIFI_INFO *pst_wifi, s32 device_id, s8 *buff)
{
	s32 loop;
	s8 *start, *index, *end;
	
	if(NULL == pst_wifi || NULL == buff)
	{
		return FALSE;
	}
	start = index = end = buff;

	/* #解析设备类型id */
	while('\0' != *index && '\n' != *index)
	{
		if(',' == *index)
		{
			end = index;
			*end = '\0';
			if(device_id != atoi(start))
			{
				return FALSE;
			}
			index++;
			break;
		}
		index++;
	}

	/* #解析无线卡信息 */
	start = index;
	loop = 0;
	while('\0' != *index && '\n' != *index)
	{
		if('{' == *index)
		{
			start = ++index;
			continue;
		}

		if('}' == *index)
		{
			end = index;
			*end = '\0';
			if(2 != sscanf(start, "%d,%d", &pst_wifi[loop].freq, &pst_wifi[loop].limit_txpower))
			{
				continue;
			}
			pst_wifi[loop].iflag = 1;
			loop++;
			if(loop == AGLINK_WIFI_NUM)
			{
				return TRUE;
			}
		}
		index++;
	}

	return TRUE;
}


s32 aglink_get_board_info(AGLINK_WIFI_INFO *pst_wifi, s32 device_type)
{
	s32 loop;
	FILE *pf;
	s8 buff[1024];

	memset(pst_wifi, 0, sizeof(AGLINK_WIFI_INFO)*2);
	
	AGLINK_LOG(LOG_INFO, "model id %d", device_type);
	if(NULL == pst_wifi)
	{
		AGLINK_LOG(LOG_ERR,  "the pst_wifi is null.");
		return FALSE;
	}

	pf = fopen(PRODUCT_INFO_PATH, "r");
	if(NULL == pf)
	{
		AGLINK_LOG(LOG_ERR,  "cannot open "PRODUCT_INFO_PATH" file, init product failed.");
		return FALSE;
	}
	memset(buff, 0, sizeof(buff));
	/* # 获取产品信息 */
	while(NULL != fgets(buff, sizeof(buff), pf))
	{
		if(FALSE == aglink_analysis_product_info(pst_wifi, device_type, buff))
		{
			continue;
		}
		break;
	}
	fclose(pf);
	
	return TRUE;
}


s32 aglink_get_wan_ip(u8 *wan_ip)
{
	struct ifreq ifr;
	struct sockaddr_in *wanaddr = NULL, *wanmask = NULL;
	int sockfd;
	
	char wanport[64] = {0};

	#ifdef OPENWRT
	char config[64] = {0};
	AGLINK_CONFIG_GET(config,AG_WAN_PROTO,64);
	if(!strncmp(config,"pppoe",strlen("pppoe")))
		strncpy(wanport,"pppoe-wan",sizeof("pppoe-wan"));
	else{
		if(AGLINK_CONFIG_GET(config,AG_WAN_IFNAME,64) >0)
			strncpy(wanport,config,strlen(config));
		else
			strncpy(wanport,"eth0",sizeof("eth0"));
	}
	
	#else
	char nvram_value[32] = {0};
	nvram_get(nvram_value, NVRAM_IPv4"_"NVRAM_IP_METHOD, 32);
	memset(wanport, 0, sizeof(wanport));
	if(IP_MODEL_PPPOE_CLIENT == atoi(nvram_value)){
	    strcpy(wanport, "ppp0");
	}
	else if(IP_MODEL_DHCP_CLIENT == atoi(nvram_value)){
	    nvram_get(nvram_value, NVRAM_FORWARD_MODE, 32);
	    if(FORWARD_MODE_AP == atoi(nvram_value))
			strcpy(wanport, BR_NAME"0");
	    else
	        strcpy(wanport, ETH0);
	}
	else
		return 0;
	#endif

	while(1)
	{
		AGLINK_LOG(LOG_INFO, "Try to get wan(%s) ip", wanport);
		if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) <= 0)
			return -1;

		strncpy(ifr.ifr_name, wanport, IFNAMSIZ-1);
		if(ioctl(sockfd, SIOCGIFADDR, &ifr) >= 0){
			wanaddr = (struct sockaddr_in *)&(ifr.ifr_addr);
			if(ntohl(wanaddr->sin_addr.s_addr) != 0xC0A80101){ /*不是默认地址，说明已经拿到地址*/
				memcpy(wan_ip, (u8 *)&wanaddr->sin_addr.s_addr, sizeof(wanaddr->sin_addr.s_addr));
				close(sockfd);
				break;
			}
		}
		
		close(sockfd);
		sleep(5);
	}
	
	return 0;
}



s32 aglink_get_devinfo(void)
{
	struct ifconf ifc;
	struct ifreq req[16];
	s32 intrface, ip;
	s8 ifname[16], config[64];
	AGLINK_DEV_INFO  *dev_info = gpstaglinkmgt->pstdev_info;
	char *pt;

	ifc.ifc_len = sizeof(req);
	ifc.ifc_buf = (caddr_t)req;

	if(AGLINK_CONFIG_GET(config, AG_DEV_NAME, 64) == 0){
		strncpy(dev_info->dev_name, config, 63);
	}
	else
		dev_info->dev_name[0] = '\0';

	aglink_get_wan_ip(dev_info->ip);
	
#ifdef BOARD_AP93
	aglink_get_ifmac(dev_info->phy_mac, "ixp0");
#else
	aglink_get_ifmac(dev_info->phy_mac, PHY_IF);
#endif

	if(IS_DEV_GW()){
		if(AGLINK_CONFIG_GET(config, AG_HOTSPOT_NAME, (NVRAM_LEN*2)) != 0){
			AGLINK_LOG(LOG_ERR, "Get hotspot Name error");
			dev_info->hotspot_name[0] = '\0';
		}
		else
			strncpy(dev_info->hotspot_name, config, HOTSPOT_NAME_LEN);

		memcpy(gpstaglinkmgt->mac, dev_info->phy_mac, MAC_LEN);
	}
	else
		dev_info->hotspot_name[0] = '\0';

	dev_info->model_id = aglink_get_dev_model_id();	//aptool getdevtype
	
	#ifndef GW_X86
	#ifdef OPENWRT
	/*wifi info*/
	#else
	aglink_get_board_info(&dev_info->stAgwifiInfo, dev_info->model_id);
	#endif
	#endif
	
	dev_info->ap_work_mode = aglink_get_ap_work_mode();
	AGLINK_LOG(LOG_INFO, "Get Dev info: phy_mac("MAC_FMT"), ip("IP_FMT"), "
		"devtype(%d), hotspot(%s), model_id(%d) ap_work_mode:%d wifiinfo[0]:iflag=%d txpower=%d  wifiinfo[1]:iflag=%d txpower=%d", 
		MAC_ARG(dev_info->phy_mac), IP_ARG(dev_info->ip), dev_info->dev_type, 
		dev_info->hotspot_name, dev_info->model_id, dev_info->ap_work_mode, dev_info->stAgwifiInfo[0].iflag, dev_info->stAgwifiInfo[0].limit_txpower, \
		dev_info->stAgwifiInfo[1].iflag, dev_info->stAgwifiInfo[1].limit_txpower);

	return 0;
}

#ifndef GW_X86

s32 aglink_get_uptime(u32 *pruntime)
{
	struct sysinfo info;
	sysinfo(&info);

	*pruntime = info.uptime;
	AGLINK_LOG(LOG_INFO, "Runtime is %d", *pruntime);
	return TRUE;
}

s32 aglink_get_sw_info(u8 *pswinfo)
{
	FILE    	*pfData = NULL;
    	u8    	acFileName[64] = {"/etc/version.conf"};
    	u8		*pucBuf = NULL;
      	u32		ulLoop;
      	u32		ulLen;
      	u32		i;
      	
	memset(pswinfo, 0, 64);
    	for(ulLoop = 0; ulLoop < 5; ulLoop++)
    	{
		pfData = fopen (acFileName, "rb");
        	if (NULL != pfData)
        	{
            		break;
        	}
    	}
    
    	if (NULL == pfData)
    	{
		AGLINK_LOG(LOG_INFO, " fopen %s fail", acFileName);
        	return FALSE;
    	}

	fseek(pfData, 0, SEEK_END);
	ulLen = ftell(pfData);			/* 获取文件字节数*/
	fseek(pfData, 0, SEEK_SET);

	pucBuf = MEM_CALLOC(ulLen+1);
	if (pucBuf == NULL) 
    	{
		AGLINK_LOG(LOG_ALERT, "MALLOC len:%d fail", ulLen);
		fclose(pfData);
		return FALSE;
	}

	fread(pucBuf, 1, ulLen, pfData);
	fclose(pfData);

	sscanf(pucBuf, "%s\n", pswinfo);
		
    	i = strlen(pswinfo);
    	for (; i>=0; i--)
    	{
		if (	pswinfo[i] == '\r' ||
            		pswinfo[i] == ' '  ||
            		pswinfo[i] == '\t' )
        	{
            		pswinfo[i] = '\0';
        	}
        	else
        	{
            		break;
        	}
    	}

	AGLINK_LOG(LOG_INFO, "SwVersion:%s\n", pswinfo);
	
	MEM_FREE(pucBuf);
	
	return TRUE;
}

s32 aglink_get_hw_info(u32 *phwtype)
{
	u32	ulLoop;
	FILE		*pfData = NULL;
	u8	acFileName[64] = {"/tmp/cpuinfo"};
	u8	*pucBuf = NULL;
	u8	*pucPos = NULL;
	u32	ulLen;

	system("cat /proc/cpuinfo > /tmp/cpuinfo");

	*phwtype = 0;

	for(ulLoop = 0; ulLoop < 5; ulLoop++)
	{
		pfData = fopen (acFileName, "rb");
		if (NULL != pfData)
		{
			break;
		}
	}
	
	if (NULL == pfData)
	{
		AGLINK_LOG(LOG_ALERT, " fopen %s fail\n", acFileName);
		return FALSE;
	}

	fseek(pfData, 0, SEEK_END);
	ulLen = ftell(pfData);			/* 获取文件字节数*/
	fseek(pfData, 0, SEEK_SET);

	pucBuf = MEM_CALLOC(ulLen+1);
	if (pucBuf == NULL) {
		AGLINK_LOG(LOG_ALERT, "MALLOC len:%d fail\n", ulLen);
		fclose(pfData);
		return;
	}

	fread(pucBuf, 1, ulLen, pfData);
	fclose(pfData);

	/*
	printf("ac_cw_wtp_get_hwtype ulLen:%d\n", ulLen);
	printf("ac_cw_wtp_get_hwtype cpuinfo:%s\n", pucBuf);
	*/

	/* 查找XScale */
	pucPos = strstr(pucBuf, "XScale");
	if (pucPos != NULL)
	{
		*phwtype = AG_APHWTYPE_IXP;
		AGLINK_LOG(LOG_INFO,"AG_APHWTYPE_IXP\n");
		goto ret;
	}

	/* 查找Atheros AR7240*/
	pucPos = strstr(pucBuf, "Atheros AR7240");
	if (pucPos != NULL)
	{
		*phwtype  = AG_APHWTYPE_AP93;
		AGLINK_LOG(LOG_INFO,"AG_APHWTYPE_AP93\n");
		goto ret;
	}

	/* 2011-8-8 pyy add: AR7241/AR7242 AP93 */
	/* 查找Atheros AR7241*/
	pucPos = strstr(pucBuf, "Atheros AR7241");
	if (pucPos != NULL)
	{
		*phwtype = AG_APHWTYPE_AP93;
		AGLINK_LOG(LOG_INFO,"AG_APHWTYPE_AP93\n");
		goto ret;
	}

	/* 查找Atheros AR7242*/
	pucPos = strstr(pucBuf, "Atheros AR7242");
	if (pucPos != NULL)
	{
		*phwtype = AG_APHWTYPE_AP93;
		AGLINK_LOG(LOG_INFO,"AG_APHWTYPE_AP93\n");
		goto ret;
	}

	/* begin zsy: by for 9344, 2012/03/14 */
	/* 商用版支持9344平台 */
	pucPos = strstr(pucBuf, "Atheros AR934x");
	if (pucPos != NULL)
	{
		*phwtype = AG_APhWTYPE_9344;
		AGLINK_LOG(LOG_INFO,"AG_APHWTYPE_9344\n");
		goto ret;
	}

	/* BEGIN: Added by Zhou Shouya, 2012/9/13	PN:支持WallAP平台， 芯片是9341 */
	/* 商用版支持9344平台 */
	pucPos = strstr(pucBuf, "Atheros AR9341");
	if (pucPos != NULL)
	{

		*phwtype  = AG_APHWTYPE_TA40;
		AGLINK_LOG(LOG_INFO,"AG_APHWTYPE_TA40\n");
		goto ret;
	}
	/* END:   Added by Zhou Shouya, 2012/9/13 */
	/* BEGIN: Added by lvjianlin, 2013/7/24   PN:AP9331_OAM */
	pucPos = strstr(pucBuf, "Atheros AR933x");
	if (pucPos != NULL)
	{
		*phwtype = AG_APHWTYPE_TA33;
		AGLINK_LOG(LOG_INFO,"AG_APHWTYPE_TA33\n");
		goto ret;
	}
	/* END:   Added by lvjianlin, 2013/7/24   PN:AP9331_OAM */
	/* BEGIN: Added by wangdusun, 2013/8/22   PN:支持TA55平台 */
	pucPos = strstr(pucBuf, "QCA955x");
	if (pucPos != NULL)
	{

		*phwtype = AG_APHWTYPE_TA55;
		AGLINK_LOG(LOG_INFO,"AG_APHWTYPE_TA55\n");
		goto ret;
	}
	/* END:   Added by wangdusun, 2013/8/22   PN:支持TA55平台 */

	pucPos = strstr(pucBuf, "QCA9531");
	if (pucPos != NULL)
	{
		*phwtype = AG_APHWTYPE_TA37;
		AGLINK_LOG(LOG_INFO,"AG_APHWTYPE_TA55\n");
		goto ret;
	}

	pucPos = strstr(pucBuf, "QCA9563");
	if (pucPos != NULL)
	{
		*phwtype = AG_APHWTYPE_TA62;
		AGLINK_LOG(LOG_INFO,"AG_APHWTYPE_TA62\n");
		goto ret;
	}

ret:
	AGLINK_LOG(LOG_INFO,"phwtype:%d\n", *phwtype);

	MEM_FREE(pucBuf);
	return TRUE;
}

s32 aglink_get_aclinkstate(s32 *iOnline)
{
	FILE *pfData;
	char temp[0x100];
	char *pacLinkState;

	/* 打开文件 */
	pfData = fopen (AC_LINK_STATE_FILE_PATH, "r+");
	if (NULL == pfData)
	{
		*iOnline = 0;	
		return 0;
	}

	memset(temp, 0, 0x100);
	
	/* 获取ac linkState */	
	while (fgets(temp, 0x100, pfData))
	{
		if (strstr(temp, "linkState") != NULL)
		{	
		    	pacLinkState = strchr(temp,'=');		

		    	if (pacLinkState)
		      	{
			     	pacLinkState++;
			     	if(1 == atoi(pacLinkState))
		     		{
					*iOnline = 1;
				}
				else
				{
					*iOnline = 0;
				}
		      	}	
		}
	}
	fclose(pfData);

	return 0;
}

s32 aglink_get_ap_dev_info(AGLINK_AP_DETAIL_INFO_T *stApInfo)
{
	#ifdef OPENWRT
	char device_type[STR_LEN_32];
	if(!factory_get(device_type,DEVICE_TYPE,STR_LEN_32))
		memcpy(stApInfo->acDeviceType, device_type, STR_LEN_32);
	#else
	FILE *pFile = NULL;
	u8	strCmd[256] = {0};
	T_deviceSign stdevData;

	memset(&stdevData, 0, sizeof(T_deviceSign));
	
	if(0 != access(TEMP_DEVICE_SIGN_FILE, R_OK))
	{
		sprintf( strCmd, "flash -t -r mtd2 %s %d", TEMP_DEVICE_SIGN_FILE, sizeof( T_deviceSign ) );
		system( strCmd );
	}
	pFile = fopen(TEMP_DEVICE_SIGN_FILE, "r+b");
	if(NULL == pFile)
	{
		AGLINK_LOG(LOG_ERR, "Open "TEMP_DEVICE_SIGN_FILE" fail!");
		return -1;
	}

	if ( 1 != fread(&stdevData, sizeof(T_deviceSign), 1, pFile))
	{
		AGLINK_LOG(LOG_ERR, "read "TEMP_DEVICE_SIGN_FILE" fail\n");
		fclose(pFile);
		return -1;
	}

	AGLINK_LOG(LOG_ERR, "dev type:%ld", stdevData.lDeviceType);
	switch(stdevData.lDeviceType)
	{
		memset(strCmd, 0, sizeof(strCmd));
		case 9001001:
			sprintf(strCmd, "室内单频1*1");
			memcpy(stApInfo->acDeviceType, strCmd, strlen(strCmd));
			break;
		case 9001002:
			sprintf(strCmd, "室内单频2*2");
			memcpy(stApInfo->acDeviceType, strCmd, strlen(strCmd));
			break;
		case 9002001:
			sprintf(strCmd, "室内双频2*2");
			memcpy(stApInfo->acDeviceType, strCmd, strlen(strCmd));
			break;
		case 10001001:
			sprintf(strCmd, "室外单频2*2");
			memcpy(stApInfo->acDeviceType, strCmd, strlen(strCmd));
			break;
		case 10001002:
			sprintf(strCmd, "室外双频2*2");
			memcpy(stApInfo->acDeviceType, strCmd, strlen(strCmd));
			break;
		case 9003001:
			sprintf(strCmd, "室内双频3*3");
			memcpy(stApInfo->acDeviceType, strCmd, strlen(strCmd));
			break;
		case 9003002:
			sprintf(strCmd, "室内单频3*3");
			memcpy(stApInfo->acDeviceType, strCmd, strlen(strCmd));
			break;
		case 80101:
			sprintf(strCmd, "室内单频3*3");
			memcpy(stApInfo->acDeviceType, strCmd, strlen(strCmd));
			break;
		default:
			sprintf(strCmd, "dev type:%ld", stdevData.lDeviceType);
			memcpy(stApInfo->acDeviceType, strCmd, strlen(strCmd));
			break;
	}
	fclose(pFile);
	#endif

	memcpy(stApInfo->acVendor, "GBCOM", strlen("GBCOM"));
	memcpy(stApInfo->acMac, gpstaglinkmgt->pstdev_info->phy_mac, MAC_LEN);
	memcpy(stApInfo->acIp, gpstaglinkmgt->pstdev_info->ip, IP_LEN);
	
	
	return 0;
}

s32 aglink_array_string_to_int(u8 *src , u8 *dst, s32 *total)
{
	u8 *start, *end;
	u8 *buff;
	int flag;
	
	if(NULL == src || NULL == dst)
	{
		return FALSE;
	}
	buff = NULL;
	buff = (char *)malloc(strlen(src)+1);		/* modify by zhz 2013-12-27 */
	if(NULL == buff)
	{
		return FALSE;
	}
	memcpy(buff, src, strlen(src)+1);

	flag = 0;
	(*total) = 0;
	start = end = buff;
	while('\0' != *end)
	{
		flag = 1;
		if(*end != ',')
		{
			end++;
			continue;
		}
		else
		{
			*end = '\0';
			end++;
			*dst = atoi(start);
			dst++;
			start = end;
			(*total)++;
		}
	}
	if(1 == flag)
	{
		*dst = atoi(start);
		(*total)++;
	}
	if(NULL != buff)
	{
		free(buff);
	}
	return TRUE;
}

s32  aglink_array_string_add_someone(s8 *src, int id)
{
	char tmp[16];
	char *list;
	int len;
	int total, loop, flag;

	if(NULL == src)
	{
		return FALSE;
	}
	len = strlen(src);
	list = NULL;
	list = (char *)malloc(len);
	if(NULL == list)
	{
		return FALSE;
	}
	aglink_array_string_to_int(src, list, &total);
	flag = 0;
	memset(src, 0, len);
	if(0 == total)
	{
		sprintf(tmp, "%d,", id);
		strcat(src, tmp);
	}
	else
	{
		for(loop = 0; loop < total; loop++)
		{
			if(id < list[loop] && 0 == flag)
			{
				sprintf(tmp, "%d,%d,", id, list[loop]);
				flag = 1;
			}
			else
			{
				sprintf(tmp, "%d,", list[loop]);
			}
			strcat(src, tmp);
		}
		if(0 == flag)
		{
			sprintf(tmp, "%d,", id);
			strcat(src, tmp);
		}
	}
	src[strlen(src)-1] = '\0';
	if(NULL != list)
	{
		free(list);
	}
	return TRUE;
}
s32  aglink_array_string_del_someone(char *src, int id)
{
    char tmp[16];
    char *list;
    int len;
    int total, loop, flag;

    if(NULL == src)
    {
        return FALSE;
    }
    len = strlen(src);
    list = NULL;
    list = (char *)malloc(len);
    if(NULL == list)
    {
        return FALSE;
    }
    aglink_array_string_to_int(src, list, &total);
    flag = 0;
    memset(src, 0, len);
    for(loop = 0; loop < total; loop++)
    {
        if(id != list[loop])
        {
            sprintf(tmp, "%d,", list[loop]);
            strcat(src, tmp);
        }
    }
    src[strlen(src)-1] = '\0';
    if(NULL != list)
    {
        free(list);
    }
    return TRUE;
}


#ifdef OPENWRT

s32 aglink_get_ssid_info(AGLINK_AP_SSID_INFO_T *pssidInfo, s32 *pCha2, s32 *pSsidNum)
{
    s32 iLoop = 0, iLoop1 = 0,vap_num=0;	
    WIFI_API_ENDUSER_STATS_T   stEndUserOmc;
    AP_WIFI_T stWifiInfo,*pstWifiInfo;

    
    pstWifiInfo = &stWifiInfo;
    memset(pstWifiInfo,0,sizeof(AP_WIFI_T));
    /*获取当前vap列表*/
    pal_wifi_get_vap_info(pstWifiInfo);
    *pSsidNum = pstWifiInfo->astRadio[0].uiVapNum + pstWifiInfo->astRadio[1].uiVapNum;
    AGLINK_LOG(LOG_INFO,  "uiRadioNum %d   pSsidNum %d ", pstWifiInfo->uiRadioNum,*pSsidNum);

    
    for(iLoop=0;iLoop < pstWifiInfo->uiRadioNum;iLoop++)
    {
        for(iLoop1 = 0; iLoop1 < pstWifiInfo->astRadio[iLoop].uiVapNum; iLoop1++)
        {
            memcpy(pssidInfo[vap_num].acIndex, pstWifiInfo->astRadio[iLoop].astVap[iLoop1].acVapName, strlen(pstWifiInfo->astRadio[iLoop].astVap[iLoop1].acVapName));
            /*get wifi channel*/

            memset(&stEndUserOmc, 0, sizeof(WIFI_API_ENDUSER_STATS_T));
            stEndUserOmc.ulBufLen = sizeof(WIFI_API_STA_STATS_T)*128;
            stEndUserOmc.pstStaStats = malloc(stEndUserOmc.ulBufLen);
            pal_wifi_vap_get_enduseromc(pssidInfo[vap_num].acIndex, &stEndUserOmc);
            pssidInfo[vap_num].iUser_num = stEndUserOmc.usTotalStaNumber;
            free(stEndUserOmc.pstStaStats);
            stEndUserOmc.pstStaStats=NULL;

            memcpy(pssidInfo[vap_num].acSsid, pstWifiInfo->astRadio[iLoop].astVap[iLoop1].acSsid, strlen(pstWifiInfo->astRadio[iLoop].astVap[iLoop1].acSsid));
            AGLINK_LOG(LOG_INFO,  "VAP name:%s SSID name:%s", pssidInfo[vap_num].acIndex,pssidInfo[vap_num].acSsid);
            vap_num++;
        }
    }
    return TRUE;

}


s32 aglink_down_all_vap(s32 wifi_id)
{
	u8 vap_name[32] = {0};
	u8 uci_name[32] = {0};
	u8 uci_value[32] = {0};

	s32 vap_num = 0, iLoop = 0,wifi_index=0;

	vap_num=pal_wifi_get_vap_num();
	
	for(iLoop = 0; iLoop < vap_num; iLoop++)
	{
		sprintf(uci_name,"wireless.@wifi-iface[%d].device",iLoop);
		if(!uci_config_get(uci_name,uci_value)){
			if(strncmp(uci_value,"wifi0",strlen("wifi0")))
				wifi_index=0;
			else
				wifi_index=1;
			if(wifi_id == wifi_index)
			{
				pal_wifi_get_vap_name(iLoop,vap_name);
				aglink_device_down(vap_name);
			}
		
		}
	}
	return TRUE;
}



s32 aglink_up_all_vap(s32 wifi_id)
{
	u8 vap_name[32] = {0};
	u8 uci_name[32] = {0};
	u8 uci_value[32] = {0};

	s32 vap_num = 0, iLoop = 0,wifi_index=0;

	vap_num=pal_wifi_get_vap_num();
	
	for(iLoop = 0; iLoop < vap_num; iLoop++)
	{
		sprintf(uci_name,"wireless.@wifi-iface[%d].device",iLoop);
		if(!uci_config_get(uci_name,uci_value)){
			if(strncmp(uci_value,"wifi0",strlen("wifi0")))
				wifi_index=0;
			else
				wifi_index=1;
			if(wifi_id == wifi_index)
			{
				pal_wifi_get_vap_name(iLoop,vap_name);
				aglink_device_up(vap_name);
			}
		
		}
	}
	return TRUE;
}


s32 aglink_device_down(u8 *vap_name)
{
	char cmd[128]={0};
	sprintf(cmd,"ifconfig %s down",vap_name);
	system(cmd);
	return 0;
}


s32 aglink_device_up(u8 *vap_name)
{
	char cmd[128]={0};
	sprintf(cmd,"ifconfig %s up",vap_name);
	system(cmd);
	return 0;
}


s32 aglink_device_down_up(u8 *vap_name)
{
	char cmd[128]={0};
	sprintf(cmd,"ifconfig %s down up",vap_name);
	system(cmd);
	return 0;
}



#else

s32 aglink_vap_nvram_set(AGLINK_AP_SSID_INFO_T *pstssid, s32 nvram_id, s32 wifi_id, s32 wlan_id)
{
	char nvram_name[NVRAM_LEN];
	char nvram_value[NVRAM_LEN];

	// 获取nvram_id

	sprintf(nvram_name, NVRAM_VAP"%d"NVRAM_VAP_WIFI_ID, nvram_id);
	sprintf(nvram_value, "%d", wifi_id);
	if(NvramResultFailed == nvram_set(nvram_name, nvram_value))
	{
		AGLINK_LOG(LOG_ALERT, "set wifi id failed.");
		return NvramResultFailed;
	}
	sprintf(nvram_name, NVRAM_VAP"%d"NVRAM_VAP_WLAN_ID, nvram_id);
	sprintf(nvram_value, "%d", wlan_id);
	if(NvramResultFailed == nvram_set(nvram_name, nvram_value))
	{
		AGLINK_LOG(LOG_ALERT,  "set wlan id failed.");
		return NvramResultFailed;
	}
	sprintf(nvram_name, NVRAM_VAP"%d"NVRAM_VAP_SSID_MODE, nvram_id);
	sprintf(nvram_value, "0");
	if(NvramResultFailed == nvram_set(nvram_name, nvram_value))
	{
		AGLINK_LOG(LOG_ALERT,  "set ssid mode failed.");
		return NvramResultFailed;
	}
	sprintf(nvram_name, NVRAM_VAP"%d"NVRAM_VAP_SSID_NAME, nvram_id);
	if(NvramResultFailed == nvram_set(nvram_name, pstssid->acSsid))
	{
		AGLINK_LOG(LOG_ALERT, "set ssid name failed.");
		return NvramResultFailed;
	}
	#if 0
	sprintf(nvram_name, NVRAM_VAP"%d"NVRAM_VAP_BSSID, nvram_id);
	sprintf(nvram_value, MAC_FMT, MAC_AGR(pst_vap->bssid));
	if(NvramResultFailed == nvram_set(nvram_name, nvram_value))
	{
		lapp_log(APP_LOG_ERR, "set bssid failed.");
		return NvramResultFailed;
	}
	#endif
	sprintf(nvram_name, NVRAM_VAP"%d"NVRAM_VAP_VAP_NAME, nvram_id);
	sprintf(nvram_value, "ap%d_%d", wifi_id, wlan_id);
	if(NvramResultFailed == nvram_set(nvram_name, nvram_value))
	{
		AGLINK_LOG(LOG_ALERT,  "set vap name failed.");
		return NvramResultFailed;
	}
	//restore_vap(pst_vap->nvram_id);
	if(NvramResultSuccess != nvram_commit())
	{
		AGLINK_LOG(LOG_ALERT, "nvram commit failed");
		return NvramResultFailed;
	}
	return NvramResultSuccess;
}

s32 aglink_vap_nvram_unset(int nvram_id)
{
	char command[128];
	sprintf(command, "nvramcli show | grep vap_%d_ | xargs nvramcli unset", nvram_id);
	system(command);
   	return NvramResultSuccess;
}


s32 aglink_get_wlan_list(s32 wifi_id, s8 *vap_list, s32 *vap_num)
{
	u8 nvram_name[NVRAM_LEN] = {0};
	u8 list[128] = {0};

	#if 0
	memset(vap_list, 0, sizeof(vap_list));
	sprintf(nvram_name, NVRAM_WIFI"%d"NVRAM_WIFI_WLAN_ID_LIST, wifi_id);

	nvram_get(vap_list, nvram_name, sizeof(vap_list));
	aglink_array_string_to_int(vap_list,list, vap_num);

	#else
	memset(vap_list, 0, sizeof(vap_list));
	sprintf(nvram_name, NVRAM_WIFI"%d"NVRAM_WIFI_WLAN_ID_LIST, wifi_id);
	
	if(NvramResultFailed == nvram_get(list, nvram_name, sizeof(list)))
	{
	    AGLINK_LOG(LOG_ALERT, "get %s nvram name failed.", nvram_name);
	    return FALSE;
	}
	aglink_array_string_to_int(list,vap_list, vap_num);
	#endif
	
	return TRUE;
}

s32 aglink_get_vap_name(u8 *vap_name, s32 nvram_id)
{
	u8 nvram_name[NVRAM_LEN];
	sprintf(nvram_name, NVRAM_VAP"%d"NVRAM_VAP_VAP_NAME, nvram_id);
	if(NvramResultFailed == nvram_get(vap_name, nvram_name, NVRAM_LEN))
	{
	    AGLINK_LOG(LOG_ALERT, "get %s nvram name failed.", nvram_name);
	    return NvramResultFailed;
	}
	return NvramResultSuccess;
}
s32 aglink_del_vap(u8 *vap_name)
{
	return wifi_api_vap_destroy(vap_name);
}

s32 aglink_add_vap(AGLINK_AP_SSID_INFO_T *pstssid, u8 *vap_name, u8 *wifi_name)
{
	s32 iRet = 0;
	u8 command[128];
	WIFI_API_SSID_T stSsid;
	
	iRet = wifi_api_vap_create(vap_name, wifi_name, WIFI_API_OPMODE_HOSTAP);
	if(0 != iRet)
	{
	    AGLINK_LOG(LOG_ALERT, "create vap failed, vap_name %s, wifi_name: %s",
	        vap_name, wifi_name);
	    return FALSE;
	}

	memset(&stSsid, 0, sizeof(WIFI_API_SSID_T));
	stSsid.len = strlen(pstssid->acSsid);
	memcpy(stSsid.ssid, pstssid->acSsid, stSsid.len);
	iRet = wifi_api_vap_set_ssid(vap_name, &stSsid);
	if(0 != iRet)
	{
		AGLINK_LOG(LOG_ALERT,  "set ssid name failed, vap_name %s, ssid_name: %s",
		    vap_name, pstssid->acSsid);
		return FALSE;
	}

	sprintf(command, "brctl addif br0 %s", vap_name);
    	system(command);
		
	aglink_device_up(vap_name);

	return TRUE;
	
}

s32 aglink_down_all_vap(s32 wifi_id)
{
	u8  vap_name[32] = {0};
	s8 vap_list[128] = {0};
	s32 vap_num = 0, iLoop = 0;

	aglink_get_wlan_list(wifi_id,  vap_list,  &vap_num);

	for(iLoop = 0; iLoop < vap_num; iLoop++)
	{
		sprintf(vap_name, "ap%d_%d", wifi_id, vap_list[iLoop]);
		aglink_device_down(vap_name);
	}

	return TRUE;
}



s32 aglink_up_all_vap(s32 wifi_id)
{
	u8 vap_name[32] = {0};
	s8 vap_list[128] = {0};
	s32 vap_num = 0, iLoop = 0;

	aglink_get_wlan_list(wifi_id,  vap_list,  &vap_num);

	for(iLoop = 0; iLoop < vap_num; iLoop++)
	{
		sprintf(vap_name, "ap%d_%d", wifi_id, vap_list[iLoop]);
		aglink_device_up(vap_name);
	}

	return TRUE;
}



s32 aglink_get_ssid_info_fat(AGLINK_AP_SSID_INFO_T *pssidInfo, s32 *pCha2, s32 *pSsidNum)
{
	s32 wifi_id = 0, iwifi0 = 0, iRet = -1;
	u8 vap_name[32] = {0},  ssid[32] = {0};
	s8 vap_list[128] = {0};
	s32 vap_num = 0, iLoop = 0;	
	WIFI_API_ENDUSER_STATS_T   stEndUserOmc;

	/*获取当前vap列表，现在仅支持单频设备*/
	aglink_get_wlan_list(wifi_id, vap_list, &vap_num);

	AGLINK_LOG(LOG_INFO, "wifi_id:%d vap_num:%d, vap_list:%s", wifi_id, vap_num, vap_list);
	
	for(iLoop = 0; iLoop < vap_num; iLoop++)
	{
		sprintf(vap_name, "ap%d_%d", wifi_id, vap_list[iLoop]);

		memcpy(pssidInfo[iLoop].acIndex, vap_name, strlen(vap_name));

		/*get wifi channel*/
		if(0 == iwifi0)
		{
			iwifi0 = 1;
			wifi_api_vap_get_channel(vap_name, pCha2);
			AGLINK_LOG(LOG_INFO,  "strCmd:%s iwifi0 channel:%d", vap_name, *pCha2);
		}

		memset(&stEndUserOmc, 0, sizeof(WIFI_API_ENDUSER_STATS_T));
		stEndUserOmc.ulBufLen = sizeof(WIFI_API_STA_STATS_T)*128;
    		stEndUserOmc.pstStaStats = malloc(stEndUserOmc.ulBufLen);
		wifi_api_vap_get_enduseromc(vap_name, &stEndUserOmc);
		pssidInfo[iLoop].iUser_num = stEndUserOmc.usTotalStaNumber;
		free(stEndUserOmc.pstStaStats);
		stEndUserOmc.pstStaStats=NULL;

		memset(ssid, 0, sizeof(ssid));
		iRet = wifi_api_vap_get_ssid(vap_name, ssid, sizeof(ssid));
		if(iRet < 0)
		{
			AGLINK_LOG(LOG_INFO, "there is no vap %s for devcie.", vap_name);
		}	
		memcpy(pssidInfo[iLoop].acSsid, ssid, strlen(ssid));
		AGLINK_LOG(LOG_INFO,  "SSID name:%s", pssidInfo[iLoop].acSsid);

	}
	
	*pSsidNum = vap_num;
	
	return TRUE;
}
s32 aglink_get_ssid_info_fit_two(AGLINK_AP_CONFIG_INFO_T * stApInfo, T_configFileData * pstConfigData)
{
	s32 iLoop;
	s32 iFlag = 0;
	s32 iwifi0 = 0;
	s32 iwifi1 = 0;
	s32 iRet = 0;
	u32 iwifi0Channel = 0;
	u32 iwifi1Channel = 0;
	u8 strCmd[256] = {0};	
	char ssid[32] = {0};

	 WIFI_API_ENDUSER_STATS_T   stEndUserOmc;

	 memcpy(stApInfo->acLocation, pstConfigData->sysBasicSetup.acLocation, sizeof(pstConfigData->sysBasicSetup.acLocation));
	 
	 for(iLoop = 0; iLoop < SSID_NUMBER; iLoop++)
	 {
		 if((INVALID_VAP_ID == pstConfigData->apVapTableRw[iLoop].apVapId) || (pstConfigData->apVapTableRw[iLoop].apVapId >= apVapTable_SIZE))
		 {
			 continue;
		 }
	 

		 if(pstConfigData->apVapTableRw[iLoop].apBasicSetup.ulEndUserMaxNum > stApInfo->total_link)
		 {
			stApInfo->total_link = pstConfigData->apVapTableRw[iLoop].apBasicSetup.ulEndUserMaxNum;
		 }
		 
		 sprintf(strCmd, "ap%ld_%ld", pstConfigData->apVapTableRw[iLoop].apBasicSetup.wifiDeviceId, pstConfigData->apVapTableRw[iLoop].apVapId);
		 memcpy(stApInfo->ssid[iFlag].acIndex, strCmd, strlen(strCmd));
	 
		 if((0 == iwifi0)&&(0 == pstConfigData->apVapTableRw[iLoop].apBasicSetup.wifiDeviceId))
		 {
			 iwifi0 = 1;
			 wifi_api_vap_get_channel(strCmd, &stApInfo->iCha2);
			 AGLINK_LOG(LOG_INFO,  "strCmd:%s iwifi0 channel:%d", strCmd, stApInfo->iCha2);
		 }
	 
		 if((0 == iwifi1)&&(1 == pstConfigData->apVapTableRw[iLoop].apBasicSetup.wifiDeviceId))
		 {
			 iwifi1 = 1;
			 wifi_api_vap_get_channel(strCmd, &stApInfo->iCha5);
			 AGLINK_LOG(LOG_INFO,  "strCmd:%s iwifi1 channel:%d", strCmd, stApInfo->iCha5);
		 }
		 memset(&stEndUserOmc, 0, sizeof(WIFI_API_ENDUSER_STATS_T));
		 stEndUserOmc.ulBufLen = sizeof(WIFI_API_STA_STATS_T)*128;
			 stEndUserOmc.pstStaStats = malloc(stEndUserOmc.ulBufLen);
		 wifi_api_vap_get_enduseromc(strCmd, &stEndUserOmc);
		 stApInfo->ssid[iFlag].iUser_num = stEndUserOmc.usTotalStaNumber;
		 free(stEndUserOmc.pstStaStats);
		 stEndUserOmc.pstStaStats=NULL;
	 
		 //memcpy(stApInfo->ssid[iFlag].acSsid, pstConfigData->apVapTableRw[iLoop].apBasicSetup.apSsid, strlen(pstConfigData->apVapTableRw[iLoop].apBasicSetup.apSsid));
	 
		 memset(ssid, 0, sizeof(ssid));
		 iRet = wifi_api_vap_get_ssid(strCmd, ssid, sizeof(ssid));
		 if(iRet < 0)
		 {
			 AGLINK_LOG(LOG_INFO, "there is no vap %s for devcie.", strCmd);
		 }	 
		 memcpy(stApInfo->ssid[iFlag].acSsid, ssid, strlen(ssid));
		 AGLINK_LOG(LOG_INFO,  "SSID name:%s", stApInfo->ssid[iFlag].acSsid);
	 
		 memset(strCmd, 0, sizeof(strCmd)); 	 
		 switch(pstConfigData->apVapTableRw[iLoop].apSecuritySetup.apAuthenticationType)
		 {
			 case wifiAuthenticationType_OPENSYSTEM:
				 sprintf(strCmd,"Open", strlen("Open"));
				 memcpy(stApInfo->ssid[iFlag].acAuth, strCmd, strlen(strCmd));
				 break;
			 case wifiAuthenticationType_SHAREDKEY:
				 sprintf(strCmd,"Share", strlen("Share"));
				 memcpy(stApInfo->ssid[iFlag].acAuth, strCmd, strlen(strCmd));
				 break;
			 case wifiAuthenticationType_WPAPSK:
				 sprintf(strCmd,"WPA-PSK", strlen("WPA-PSK"));
				 memcpy(stApInfo->ssid[iFlag].acAuth, strCmd, strlen(strCmd));
				 break;
			 case staAuthenticationType_WPA2PSK:
				 sprintf(strCmd,"WPA2-PSK", strlen("WPA2-PSK"));
				 memcpy(stApInfo->ssid[iFlag].acAuth, strCmd, strlen(strCmd));
				 break;
			 case staAuthenticationType_MAC:
				 sprintf(strCmd,"MAC", strlen("MAC"));
				 memcpy(stApInfo->ssid[iFlag].acAuth, strCmd, strlen(strCmd));
				 break;
			 case staAuthenticationType_WAPI:
				 sprintf(strCmd,"WAPI", strlen("WAPI"));
				 memcpy(stApInfo->ssid[iFlag].acAuth, strCmd, strlen(strCmd));
				 break;
			 default:
				 AGLINK_LOG(LOG_ERR, "Authe method is%ld", pstConfigData->apVapTableRw[iLoop].apSecuritySetup.apAuthenticationType);
				 break;
		 }
	 
		 iFlag++;
	 }
	 
	 stApInfo->iSsidNum = iFlag;
	 return 0;

	 
	return TRUE;
}

s32 aglink_get_ssid_info_fit(AGLINK_AP_DETAIL_INFO_T *stApInfo, T_configFileData *pstConfigData)
{
	s32 iLoop;
	s32 iFlag = 0;
	s32 iwifi0 = 0;
	s32 iwifi1 = 0;
	s32 iRet = 0;
	u32 iwifi0Channel = 0;
	u32 iwifi1Channel = 0;
	u8 strCmd[256] = {0};	
	char ssid[32] = {0};
	
	 WIFI_API_ENDUSER_STATS_T   stEndUserOmc;

	for(iLoop = 0; iLoop < SSID_NUMBER; iLoop++)
	{
		if((INVALID_VAP_ID == pstConfigData->apVapTableRw[iLoop].apVapId) || (pstConfigData->apVapTableRw[iLoop].apVapId >= apVapTable_SIZE))
		{
			continue;
		}

		
		sprintf(strCmd, "ap%ld_%ld", pstConfigData->apVapTableRw[iLoop].apBasicSetup.wifiDeviceId, pstConfigData->apVapTableRw[iLoop].apVapId);
		memcpy(stApInfo->ssid[iFlag].acIndex, strCmd, strlen(strCmd));

		if((0 == iwifi0)&&(0 == pstConfigData->apVapTableRw[iLoop].apBasicSetup.wifiDeviceId))
		{
			iwifi0 = 1;
			wifi_api_vap_get_channel(strCmd, &stApInfo->iCha2);
			AGLINK_LOG(LOG_INFO,  "strCmd:%s iwifi0 channel:%d", strCmd, stApInfo->iCha2);
		}

		if((0 == iwifi1)&&(1 == pstConfigData->apVapTableRw[iLoop].apBasicSetup.wifiDeviceId))
		{
			iwifi1 = 1;
			wifi_api_vap_get_channel(strCmd, &stApInfo->iCha5);
			AGLINK_LOG(LOG_INFO,  "strCmd:%s iwifi1 channel:%d", strCmd, stApInfo->iCha5);
		}
		memset(&stEndUserOmc, 0, sizeof(WIFI_API_ENDUSER_STATS_T));
		stEndUserOmc.ulBufLen = sizeof(WIFI_API_STA_STATS_T)*128;
    		stEndUserOmc.pstStaStats = malloc(stEndUserOmc.ulBufLen);
		wifi_api_vap_get_enduseromc(strCmd, &stEndUserOmc);
		stApInfo->ssid[iFlag].iUser_num = stEndUserOmc.usTotalStaNumber;
		free(stEndUserOmc.pstStaStats);
		stEndUserOmc.pstStaStats=NULL;

		//memcpy(stApInfo->ssid[iFlag].acSsid, pstConfigData->apVapTableRw[iLoop].apBasicSetup.apSsid, strlen(pstConfigData->apVapTableRw[iLoop].apBasicSetup.apSsid));

		memset(ssid, 0, sizeof(ssid));
		iRet = wifi_api_vap_get_ssid(strCmd, ssid, sizeof(ssid));
		if(iRet < 0)
		{
			AGLINK_LOG(LOG_INFO, "there is no vap %s for devcie.", strCmd);
		}	
		memcpy(stApInfo->ssid[iFlag].acSsid, ssid, strlen(ssid));
		AGLINK_LOG(LOG_INFO,  "SSID name:%s", stApInfo->ssid[iFlag].acSsid);

		memset(strCmd, 0, sizeof(strCmd));		
		switch(pstConfigData->apVapTableRw[iLoop].apSecuritySetup.apAuthenticationType)
		{
			case wifiAuthenticationType_OPENSYSTEM:
				sprintf(strCmd,"Open", strlen("Open"));
				memcpy(stApInfo->ssid[iFlag].acAuth, strCmd, strlen(strCmd));
				break;
			case wifiAuthenticationType_SHAREDKEY:
				sprintf(strCmd,"Share", strlen("Share"));
				memcpy(stApInfo->ssid[iFlag].acAuth, strCmd, strlen(strCmd));
				break;
			case wifiAuthenticationType_WPAPSK:
				sprintf(strCmd,"WPA-PSK", strlen("WPA-PSK"));
				memcpy(stApInfo->ssid[iFlag].acAuth, strCmd, strlen(strCmd));
				break;
			case staAuthenticationType_WPA2PSK:
				sprintf(strCmd,"WPA2-PSK", strlen("WPA2-PSK"));
				memcpy(stApInfo->ssid[iFlag].acAuth, strCmd, strlen(strCmd));
				break;
			case staAuthenticationType_MAC:
				sprintf(strCmd,"MAC", strlen("MAC"));
				memcpy(stApInfo->ssid[iFlag].acAuth, strCmd, strlen(strCmd));
				break;
			case staAuthenticationType_WAPI:
				sprintf(strCmd,"WAPI", strlen("WAPI"));
				memcpy(stApInfo->ssid[iFlag].acAuth, strCmd, strlen(strCmd));
				break;
			default:
				AGLINK_LOG(LOG_ERR, "Authe method is%ld", pstConfigData->apVapTableRw[iLoop].apSecuritySetup.apAuthenticationType);
				break;
		}

		iFlag++;
	}

	stApInfo->iSsidNum = iFlag;
	return 0;
}




int aglink_creat_socket(void)
{
    int skdf;

    skdf = -1;
    skdf = socket(AF_INET, SOCK_DGRAM, 0);
    return skdf;
}


s32 aglink_device_down(u8 *vap_name)
{
	int skdf;
	struct ifreq ifr;

	skdf = -1;
	skdf = aglink_creat_socket();
	if(-1 == skdf)
	{
		AGLINK_LOG(LOG_ALERT, "creat socket failed");
		return FALSE;
	}

	memset(&ifr, 0, sizeof(struct ifreq));
	strcpy(ifr.ifr_name, vap_name);
#if 0
	if(ioctl(skdf, SIOCGIFFLAGS, &ifr)<0)
	{
		lapp_log(APP_LOG_ERR, "icctl failed.");
		return ERROR;
		close(skdf);
	}
#endif
	/* BEGIN: Modified by MaYuHua, 2013/8/9   PN:修改网络接口标志 */
	ifr.ifr_ifru.ifru_flags &= ~IFF_UP;
	ifr.ifr_ifru.ifru_flags |= IFF_MULTICAST;
	/* END:   Modified by MaYuHua, 2013/8/9   PN:修改网络接口标志 */
	if(ioctl(skdf, SIOCSIFFLAGS, &ifr)<0)
	{
		AGLINK_LOG(LOG_ALERT, "icctl failed.");
		return FALSE;
		close(skdf);
	}
	close(skdf);
	
	return TRUE;
}


s32 aglink_device_up(u8 *vap_name)
{
	int skdf;
	struct ifreq ifr;

	skdf = -1;
	skdf = aglink_creat_socket();
	if(-1 == skdf)
	{
		AGLINK_LOG(LOG_ALERT,  "creat socket failed");
		return FALSE;
	}

	memset(&ifr, 0, sizeof(struct ifreq));
	strcpy(ifr.ifr_name, vap_name);
#if 0
	if(ioctl(skdf, SIOCGIFFLAGS, &ifr)<0)
	{
		lapp_log(APP_LOG_ERR, "icctl failed.");
		return ERROR;
		close(skdf);
	}
#endif
   /* BEGIN: Modified by MaYuHua, 2013/8/9	 PN:修改网络接口标志 */
   ifr.ifr_ifru.ifru_flags |= (IFF_UP | IFF_RUNNING|IFF_MULTICAST);
   /* END:	 Modified by MaYuHua, 2013/8/9	 PN:修改网络接口标志 */
	if(ioctl(skdf, SIOCSIFFLAGS, &ifr)<0)
	{
		AGLINK_LOG(LOG_ALERT,"ioctl failed.");
		return FALSE;
		close(skdf);
	}
	close(skdf);
	
	return TRUE;
}

#endif
s32 aglink_get_ap_sys_info(AGLINK_AP_DETAIL_INFO_T *stApInfo)
{
	FILE *pFile = NULL;
	u8  strCmd[256] = {0};
	u32 iLoop = 0;
	
	AGLINK_CPU_RATE stCPURate = {0};
	AGLINK_WAN_STAT stWANstat = {0};
	
	#ifndef OPENWRT
	T_configFileData pstConfigData;
	
	memset(&pstConfigData, 0, sizeof(T_configFileData));
	
	if(0 != access(TEMP_CONFIG_FILE, R_OK))
	{
		sprintf( strCmd, "flash -t -r mtd2 %s %d", TEMP_CONFIG_FILE, sizeof( T_configFileData ) );
		system( strCmd );
	}
	pFile = fopen(TEMP_CONFIG_FILE, "r+b");
	if(NULL == pFile)
	{
		AGLINK_LOG(LOG_ERR, "Open "TEMP_CONFIG_FILE" fail!");
		return -1;
	}

	if ( 1 != fread(&pstConfigData, sizeof( T_configFileData ), 1, pFile))
	{
		AGLINK_LOG(LOG_ERR, "read "TEMP_CONFIG_FILE" fail\n");
		fclose(pFile);
		return -1;
	}
	
	memset(&strCmd, 0, sizeof(strCmd));
	//sprintf(strCmd, "AP%ld", pstConfigData.sysBasicSetup.ulWtpId);
	//memcpy(stApInfo->acfap_sn, strCmd, strlen(strCmd));

	//memcpy(stApInfo->acLocation, pstConfigData.sysBasicSetup.acLocation, strlen(pstConfigData.sysBasicSetup.acLocation));

	aglink_get_aclinkstate(&(stApInfo->iOnline));
	#endif
	

	stApInfo->iRoam = 0;
	memcpy(stApInfo->acMemo, gpstaglinkmgt->pAgconfig->acMemo, strlen(gpstaglinkmgt->pAgconfig->acMemo));
	stApInfo->iRunStatus = 1;
	
	aglink_get_cpu_info(&stCPURate);
	/*sprintf(stApInfo->acCpu_status, "CPU:%d\% (system=%d\% user=%d\% nice=%d\% idle=%d\%)", 
		stCPURate.cpu_rate, stCPURate.system, stCPURate.user, stCPURate.nice, stCPURate.idle);
	*/	
	sprintf(stApInfo->acCpu_status, "%lu%% user,%lu%% sys, %lu%% idle", stCPURate.user, stCPURate.system, stCPURate.idle);
	AGLINK_LOG(LOG_INFO, "CPU status:%s", stApInfo->acCpu_status);
	
	aglink_get_wan_stat(PHY_IF, &stWANstat);
	stApInfo->total_up = stWANstat.SendBytes;
	stApInfo->total_down = stWANstat.RecBytes;
	
	aglink_get_uptime(&stApInfo->runtimelong);	

	aglink_get_sw_info(stApInfo->softversion);
	aglink_get_hw_info(&stApInfo->hardversion);

	stApInfo->ap_work_mode = gpstaglinkmgt->pstdev_info->ap_work_mode;

	#ifdef OPENWRT
	aglink_get_ssid_info(stApInfo->ssid, &stApInfo->iCha2, &stApInfo->iSsidNum);
	#else
	if(AGLINK_FIT_AP == gpstaglinkmgt->pstdev_info->ap_work_mode)
	{
		aglink_get_ssid_info_fit(stApInfo, &pstConfigData);
	}
	/*fat vap*/
	else
	{
		aglink_get_ssid_info_fat(stApInfo->ssid, &stApInfo->iCha2, &stApInfo->iSsidNum);
	}
	#endif
	
	for(iLoop = 0; iLoop < stApInfo->iSsidNum; iLoop++)
	{
		stApInfo->cur_link += stApInfo->ssid[iLoop].iUser_num;
	}
	stApInfo->total_link = gpstaglinkmgt->pAgconfig->total_link;
	
	
	fclose(pFile);
	return 0;
}
#endif
int agent_creat_socket(void)
{
    int skdf;

    skdf = -1;
    skdf = socket(AF_INET, SOCK_DGRAM, 0);
    return skdf;
}
void agent_close_socket( int skdf )
{
    if(-1 == skdf)
    {
        AGLINK_LOG(LOG_ERR, "the skdf = -1");
        return;
    }
    close(skdf);
    return;
}

int aglink_link_encap_mac_get(unsigned char *mac, char *port)
{
    int skdf;

    struct ifreq ifr;

    skdf = -1;
    skdf = agent_creat_socket();
    if(-1 == skdf)
    {
        AGLINK_LOG(LOG_ERR, "creat socket failed");
        return -1;
    }

    memset(&ifr, 0, sizeof(struct ifreq));
    strncpy(ifr.ifr_name, port, IFNAMSIZ);
    ifr.ifr_name[IFNAMSIZ -1] = '\0';
    // define SIOCGIFHWADDR 0x8927
    if(ioctl(skdf, SIOCGIFHWADDR, &ifr))
    {
        perror("perror");
        AGLINK_LOG(LOG_ERR, "ioctl's SIOCGIFHWADDR(%x) get %s mac faile, error: %s", 
            SIOCGIFHWADDR, port, strerror(errno));
        agent_close_socket(skdf);
        return -1;
    }
    agent_close_socket(skdf);
    memcpy(mac, (unsigned char *)ifr.ifr_hwaddr.sa_data, 6);
    return 0;
}
#ifndef GW_X86

s32 aglink_get_ap_wifi_info(char *devname, AP_WIFI_INFO *pstApInfo)
{
	int iWifiIndex = 0;
	int iRet;
	char vapssid[32] = {0};
	char cTmp[4] = {0};
	iRet = wifi_api_vap_get_ssid(devname, vapssid, sizeof(vapssid));
	if(iRet < 0)
	{
		AGLINK_LOG(LOG_ERR,  "there is no vap %s for devcie.", devname);
		return -1;	 
	}

	cTmp[0] = devname[2];
	iWifiIndex = atoi(cTmp);
	AGLINK_LOG(LOG_INFO, "pstApinfo address:%x", pstApInfo);
	AGLINK_LOG(LOG_INFO,  "iWifiIndex=%d, devname:%s, vapssid:%s", iWifiIndex, devname, vapssid);

	if(pstApInfo->astRadio[iWifiIndex].uiVapNum >= AG_MODULE_RADIO_MAX_VAP_NUM)
	{
		AGLINK_LOG(LOG_ERR, "iWifiIndex(%d)`s  vap number bigger than 16", iWifiIndex);
		return -1;
	}
	memcpy(pstApInfo->astRadio[iWifiIndex].astVap[pstApInfo->astRadio[iWifiIndex].uiVapNum].acVapName, devname, strlen(devname));
	memcpy(pstApInfo->astRadio[iWifiIndex].astVap[pstApInfo->astRadio[iWifiIndex].uiVapNum].acSsid, vapssid, strlen(vapssid));
	aglink_link_encap_mac_get(pstApInfo->astRadio[iWifiIndex].astVap[pstApInfo->astRadio[iWifiIndex].uiVapNum].aucBssid, devname);
	pstApInfo->astRadio[iWifiIndex].uiVapNum++;

	return 0;
};
s32 aglink_get_ap_wifi_info_entry(AP_WIFI_INFO *pstApInfo)
{
	s8 func_str[16];
	s8 acVariableAdd[16] = {0};
	s8 *args[2];

	
	sprintf(func_str, "%08x", &aglink_get_ap_wifi_info);
	sprintf(acVariableAdd, "%08x", pstApInfo);
	args[0] = &func_str[0];
	args[1] = &acVariableAdd[0];

	pstApInfo->astRadio[0].uiRadioType = 0;
	pstApInfo->astRadio[1].uiRadioType = 1;
	
	aglink_get_vap_dev(args, 2);
	
	return 0;	
}
#endif
s32 aglink_build_socket()
{
	struct timeval timeout = {SEND_TIMEOUT_VALUE, 0};

	s32 sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	AGLINK_LOG(LOG_INFO, "socket init.");
	
	bzero(&gpstaglinkmgt->sk_addr, sizeof(gpstaglinkmgt->sk_addr));
	gpstaglinkmgt->sk_addr.sin_family = AF_INET;
	gpstaglinkmgt->sk_addr.sin_port = htons(gpstaglinkmgt->port);
	gpstaglinkmgt->sk_addr.sin_addr.s_addr = gpstaglinkmgt->ipaddr;

	if(IS_DEV_GW()){
		/* bind address and port to socket */
		if(bind(sockfd, (struct sockaddr *)&gpstaglinkmgt->sk_addr, sizeof(gpstaglinkmgt->sk_addr)) == -1)
		{
			AGLINK_LOG(LOG_ERR, "socket bind error.");
			close(sockfd);
			exit(-1);
		}
	}

	/*设置server fd发送超时为timeval*/
	setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(struct timeval));
	/*设置server fd接收超时为timeval*/
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));

	gpstaglinkmgt->sockfd = sockfd;

	/* internel socket build */
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	#if 0
	AGLINK_LOG(LOG_INFO, "socket init.");
	
	bzero(&gpstaglinkmgt->sk_addr_internal, sizeof(gpstaglinkmgt->sk_addr_internal));
	gpstaglinkmgt->sk_addr_internal.sin_family = AF_INET;
	gpstaglinkmgt->sk_addr_internal.sin_port = htons(gpstaglinkmgt->usportinternal);
	gpstaglinkmgt->sk_addr_internal.sin_addr.s_addr = gpstaglinkmgt->iaddrinternal;

	/* bind address and port to socket */
	if(bind(sockfd, (struct sockaddr *)&gpstaglinkmgt->sk_addr_internal, sizeof(gpstaglinkmgt->sk_addr_internal)) == -1)
	{
		AGLINK_LOG(LOG_ERR, "socket bind error.");
		close(sockfd);
		exit(-1);
	}

	/*设置server fd发送超时为timeval*/
	setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(struct timeval));
	/*设置server fd接收超时为timeval*/
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));

	gpstaglinkmgt->iSockfdInternal = sockfd;
	#endif
	
	return TRUE;	
}
#ifndef GW_X86

int aglink_ap_send_wifi_info()
{
	u8 buf[AG_BUFFER_SIZE] = {0};
	u8 *p = &buf[0];
	u8 **pnt = &p;
	u32 uiLength = 0;
	s32 iLoop;
	
	AP_WIFI_INFO stApInfo;
	AGLINK_LOG(LOG_INFO, " start!");
	memset(&stApInfo, 0, sizeof(AP_WIFI_INFO));

	AGLINK_LOG(LOG_ERR, "stApinfo address:%x", &stApInfo);
	aglink_get_ap_wifi_info_entry(&stApInfo);

	stApInfo.uiRadioNum = 2;
	stApInfo.uiRadioNum = htonl(stApInfo.uiRadioNum);
	for(iLoop = 0; iLoop < AG_MODULE_AP_MAX_RADIO_NUM; iLoop++)
	{
		stApInfo.astRadio[iLoop].uiRadioType = htonl(stApInfo.astRadio[iLoop].uiRadioType);
		stApInfo.astRadio[iLoop].uiVapNum = htonl(stApInfo.astRadio[iLoop].uiVapNum);
	}

	ENCODE_PUT(pnt, &stApInfo, sizeof(AP_WIFI_INFO));
	
	//return aglink_send_ag_msg(WIFIDOG_IDSET_AP_WIFI_INFO_UPDATE, AG_IPC_MSG_TYPE_OTHER_CPU, &buf[0], uiLength, NULL);
	return 0;
}

s32 aglink_get_module_info(void)
{
	AG_MODULE_IPC_MODULE_LIST_T *pos = NULL;
	u8 buf[AG_BUFFER_SIZE] = {0};
	u8 *p = &buf[0];
	u8 **pnt = &p;
	AGLINKCLI_MSG_T stMsg = {0};
	u32 ulLength = 0;

	stMsg.u.stModuleInfo.iNum = 0;

	list_for_each_entry(pos, &gstIPClist, stModuleList)
	{
		if(AGLINK_MODULE_NUMBER_MAX <= stMsg.u.stModuleInfo.iNum)
		{
			AGLINK_LOG(LOG_INFO,"Module number is bigger than 32, break");
			break;
		}

		memcpy(&stMsg.u.stModuleInfo.stModule[stMsg.u.stModuleInfo.iNum], pos, sizeof(AG_MODULE_IPC_MODULE_LIST_T));
		AGLINK_LOG(LOG_INFO, "%-15s "IP_FMT":%d\n", stMsg.u.stModuleInfo.stModule[stMsg.u.stModuleInfo.iNum].acname, 
			IP_ARG(&(stMsg.u.stModuleInfo.stModule[stMsg.u.stModuleInfo.iNum].stipcskaddr.sin_addr.s_addr)), 
			ntohs(stMsg.u.stModuleInfo.stModule[stMsg.u.stModuleInfo.iNum].stipcskaddr.sin_port));

		stMsg.u.stModuleInfo.iNum++;
		
	}

	if((sizeof(stMsg.u.stModuleInfo)+4) > AG_BUFFER_SIZE)
	{
		AGLINK_LOG(LOG_ALERT, "Msg Len is (%d) bigger than AG_BUFFER_SIZE(6144)", sizeof(stMsg.u.stModuleInfo));
		memcpy(stMsg.u.ucErrMsg, "Msg Length is too bigger!", sizeof("Msg Length is too bigger!"));
		stMsg.iFlag = 0;
		ENCODE_PUTL(pnt, stMsg.iFlag);
		ENCODE_PUT(pnt, stMsg.u.ucErrMsg, sizeof(stMsg.u.ucErrMsg));
	}
	else
	{
		stMsg.iFlag = 1;
		ENCODE_PUTL(pnt, stMsg.iFlag);
		ENCODE_PUT(pnt, &stMsg.u.stModuleInfo, sizeof(stMsg.u.stModuleInfo));
	}
	
	ulLength = p - &buf[0];
	ag_module_ipc_server_send_msg(AGLINK_CLI_GET_MODULE_INFO_ACK, AG_IPC_MSG_TYPE_SELF_CPU, "aglink", (u8 *)&buf, ulLength, NULL);
	return 0;
}

s32 aglink_get_online_info()
{
	u8 buf[AG_BUFFER_SIZE] = {0};
	u8 *p = &buf[0];
	u8 **pnt = &p;

	AGLINKCLI_MSG_T stMsg = {0};
	u32 ulLength = 0;

	if(IS_DEV_AP())
	{
		/*too long*/
		if((sizeof(stMsg.u.stOnline)+4) > AG_BUFFER_SIZE)
		{
			AGLINK_LOG(LOG_ALERT, "Msg Len is (%d) bigger than AG_BUFFER_SIZE(6144)", sizeof(stMsg.u.stOnline));
			memcpy(stMsg.u.ucErrMsg, "Msg Length is too bigger!", sizeof("Msg Length is too bigger!"));
			stMsg.iFlag = 0;
			ENCODE_PUTL(pnt, stMsg.iFlag);
			ENCODE_PUT(pnt, stMsg.u.ucErrMsg, sizeof(stMsg.u.ucErrMsg));			
		}
		/*send msg*/
		else
		{
			/*在线状态+ 在线时长+网关MAC/IP*/
			stMsg.iFlag = 1;
			stMsg.u.stOnline.uiOnlinestate = gpstaglinkmgt->dev_reg_ack_ok;
			stMsg.u.stOnline.uiOnlinetm = 0;
			stMsg.u.stOnline.uiSerIp= gpstaglinkmgt->ipaddr;
			memcpy(stMsg.u.stOnline.ucSerMAC, gpstaglinkmgt->mac, MAC_LEN);
			ENCODE_PUTL(pnt, stMsg.iFlag);
			ENCODE_PUT(pnt, &stMsg.u.stOnline, sizeof(stMsg.u.stOnline));
			AGLINK_LOG(LOG_INFO, "Onlinestate:%d Onlinetm:%d  SerIP:"IP_FMT" MAC:"MAC_FMT, 
				stMsg.u.stOnline.uiOnlinestate, stMsg.u.stOnline.uiOnlinetm, IP_ARG(&stMsg.u.stOnline.uiSerIp), stMsg.u.stOnline.ucSerMAC);
		}
	}
	else		/*GW*/
	{
		stMsg.iFlag = 0;
		memcpy(stMsg.u.ucErrMsg, "This device is GW! NO Online state!", sizeof("This device is GW! NO Online state!"));
		ENCODE_PUTL(pnt, stMsg.iFlag);
		ENCODE_PUT(pnt, stMsg.u.ucErrMsg, sizeof(stMsg.u.ucErrMsg));
	}
	
	ulLength = p - &buf[0];

	ag_module_ipc_server_send_msg(AGLINK_CLI_GET_ONLINE_INFO_ACK, AG_IPC_MSG_TYPE_SELF_CPU, "aglink", (u8 *)&buf, ulLength, NULL);
	return 0;
}
s32 aglink_get_ap_list(u8 **pnt, int iLength)
{
	u8 buf[AG_BUFFER_SIZE] = {0};
	u8 *sp = &buf[0];
	u8 **spnt = &sp;
	ap_node *pstaNode;
	AGLINKCLI_MSG_T stMsg = {0};
	AGLINKCLI_MSG_T stRecMsg = {0};
	s32 iSerialNum = 0, ulLength = 0;
	
	if(IS_DEV_AP())
	{
		stMsg.iFlag = 0;
		memcpy(stMsg.u.ucErrMsg, "This device is AP! NO AP list!", sizeof("This device is AP! NO AP list!"));
		ENCODE_PUTL(spnt, stMsg.iFlag);
		ENCODE_PUT(spnt, stMsg.u.ucErrMsg, sizeof(stMsg.u.ucErrMsg));
	}
	else		/*GW*/
	{
		int iLoop = 0, ibegin = 0;
		struct hlist_head *head;
		struct hlist_node *p;

		/*多次获取ap列表*/
		if(iLength == (4 + sizeof(AGLINKCLI_AP_LIST_T)))
		{
			DECODE_GETL(pnt, stRecMsg.iFlag);
			DECODE_GET(&stRecMsg.u.stAplistInfo, pnt, sizeof(AGLINKCLI_AP_LIST_T));
			ibegin = stRecMsg.u.stAplistInfo.ibegin;
		}

		
		stMsg.u.stAplistInfo.iNum = 0;

		AP_HLIST_LOCK();
		for(iLoop= 0; iLoop < AGLINK_GW_AP_NUM_MAX; iLoop++)
		{
			
			head = &(gpstaglinkmgt->ap_list[iLoop]);
			hlist_for_each(p, head)
			{
				if(ibegin > iSerialNum)
				{
					iSerialNum++;
					continue;
				}
				pstaNode = hlist_entry(p, ap_node, id_hlist);
				
				memcpy(&stMsg.u.stAplistInfo.stApInfo[stMsg.u.stAplistInfo.iNum], pstaNode, sizeof(ap_node));
				iSerialNum++;
				stMsg.u.stAplistInfo.iNum++;
				if(AGLINK_GW_AP_NUM_MAX <= stMsg.u.stAplistInfo.iNum)
				{
					goto SEND_AP_LIST;
				}
			}
		}
		SEND_AP_LIST:
			stMsg.u.stAplistInfo.ibegin=iSerialNum;
		
		AP_HLIST_UNLOCK();

		stMsg.iFlag = 1;
		ENCODE_PUTL(spnt, stMsg.iFlag);
		ENCODE_PUT(spnt, &stMsg.u.stAplistInfo, sizeof(AGLINKCLI_AP_LIST_T));
	}

	ulLength = sp - &buf[0];
	ag_module_ipc_server_send_msg(AGLINK_CLI_GET_AP_LIST_ACK, AG_IPC_MSG_TYPE_SELF_CPU, "aglink", (u8 *)&buf, ulLength, NULL);
	return 0;
}
s32 aglink_keeplive_tm(u8 **pnt, int iLength)
{
	u8 buf[AG_BUFFER_SIZE] = {0};
	u8 *sp = &buf[0];
	u8 **spnt = &sp;
	s32 ulLength = 0;
	AGLINKCLI_MSG_T stRecMsg = {0};
	AGLINKCLI_MSG_T stMsg = {0};


	/*获取当前保活间隔*/
	if(iLength == 0)
	{
		AGLINK_LOG(LOG_INFO, "GET keeplive timer(%ds)!", gpstaglinkmgt->keepalive_tv.tv_sec);
		stMsg.u.stKeeplive.uiKeeptm = gpstaglinkmgt->keepalive_tv.tv_sec;
		stMsg.iFlag = 1;
		ENCODE_PUTL(spnt, stMsg.iFlag);
		ENCODE_PUT(spnt, &stMsg.u.stKeeplive, sizeof(AGLINKCLI_KEEPLIVE_T));
	}
	/*设置tm，*/
	else //if((iLength - sizeof(AG_LINK_MSG_HEAD_T)) == sizeof(AGLINKCLI_MSG_T))
	{
		DECODE_GETL(pnt, stRecMsg.iFlag);
		DECODE_GET(&stRecMsg.u.stKeeplive, pnt, sizeof(AGLINKCLI_KEEPLIVE_T));
		AGLINK_LOG(LOG_INFO, "SET keeplive timer(%ds)!", stRecMsg.u.stKeeplive.uiKeeptm);

		if(gpstaglinkmgt->keepalive_tv.tv_sec != stRecMsg.u.stKeeplive.uiKeeptm)
		{
			gpstaglinkmgt->keepalive_tv.tv_sec = stRecMsg.u.stKeeplive.uiKeeptm;

			if(IS_DEV_AP())
			{
				/*在线*/
				if(TRUE == gpstaglinkmgt->dev_reg_ack_ok)
				{
					event_del(&gpstaglinkmgt->keepalive_timer_ev);
					event_add(&gpstaglinkmgt->keepalive_timer_ev, &gpstaglinkmgt->keepalive_tv);
				}
			}
		}
		stMsg.iFlag = 1;
		stMsg.u.stKeeplive.uiKeeptm = gpstaglinkmgt->keepalive_tv.tv_sec;
		ENCODE_PUTL(spnt, stMsg.iFlag);
		ENCODE_PUT(spnt, &stMsg.u.stKeeplive, sizeof(AGLINKCLI_KEEPLIVE_T));
	}

	ulLength = sp - &buf[0];
	
SEND_KEEPLIVE_TM_ACK:
	ag_module_ipc_server_send_msg(AGLINK_CLI_SET_KEEPLIVE_TM_ACK, AG_IPC_MSG_TYPE_SELF_CPU, "aglink", (u8 *)&buf, ulLength, NULL);
	return 0;
}


s32 aglink_recv_reboot_msg(u8 **pnt, u32 uiMsgLen)
{
	u8 ucgwmac[MAC_LEN] = {0};
	u8 ucShellCmd[128] = {0};
	if(NULL == pnt)
	{
		AGLINK_LOG(LOG_ALERT, "Param NULL!");
		return FALSE;
	}
	/*if((uiMsgLen-sizeof(AG_LINK_MSG_HEAD_T))!=MAC_LEN)
	{
		AGLINK_LOG(LOG_ALERT, "Msg length(%d) error", uiMsgLen-sizeof(AG_LINK_MSG_HEAD_T));
		return FALSE;
	}*/

	DECODE_GET(ucgwmac, pnt, MAC_LEN);

	if(0 != memcmp(ucgwmac, gpstaglinkmgt->mac, MAC_LEN))
	{
		AGLINK_LOG(LOG_DEBUG, "MAC ERROR gpstaglinkmgt->mac:"MAC_FMT" gwmac:"MAC_FMT, MAC_ARG(gpstaglinkmgt->mac), MAC_ARG(ucgwmac));
		return FALSE;
	}

	system("reboot 5  Aglink recv reboot msg! &");

	
	return TRUE;
}

s32 aglink_recv_update_msg(u8 **pnt, u32 uiMsgLen)
{
	u8 ucgwmac[MAC_LEN] = {0};
	u8 ucShellCmd[128] = {0};
	VERSION_UPDATE_T stUpdate = {0};
	if(NULL == pnt)
	{
		AGLINK_LOG(LOG_ALERT, "Param NULL!");
		return FALSE;
	}
	/*if((uiMsgLen-sizeof(AG_LINK_MSG_HEAD_T))!=(MAC_LEN+sizeof(VERSION_UPDATE_T)))
	{
		AGLINK_LOG(LOG_ALERT, "Msg length(%d) error", uiMsgLen-sizeof(AG_LINK_MSG_HEAD_T));
		return FALSE;
	}*/

	DECODE_GET(ucgwmac, pnt, MAC_LEN);

	if(0 != memcmp(ucgwmac, gpstaglinkmgt->mac, MAC_LEN))
	{
		AGLINK_LOG(LOG_DEBUG, "MAC ERROR gpstaglinkmgt->mac:"MAC_FMT" gwmac:"MAC_FMT, MAC_ARG(gpstaglinkmgt->mac), MAC_ARG(ucgwmac));
		return FALSE;
	}
	DECODE_GET(&stUpdate, pnt, sizeof(VERSION_UPDATE_T));

	AG_NTOHL(stUpdate.iType);
	AG_NTOHL(stUpdate.aucServerPort);
	
	AGLINK_LOG(LOG_DEBUG, "acUrl:%s Type:%d Server ip"IP_FMT" port:%d  USER:%s passwd:%s", 
		stUpdate.aucUrl, stUpdate.iType, IP_ARG(stUpdate.aucServerIP), stUpdate.aucServerPort, stUpdate.aucUser, stUpdate.aucPasswd);
		
	return TRUE;
}



#ifdef OPENWRT
s32 aglink_set_channel(s32 wifi_id, s32 channel)
{
	u8 vap_name[32] = {0};
	s8 vap_list[128] = {0};
	s32 vap_num = 0, iLoop = 0;

	//aglink_get_wlan_list(wifi_id,  vap_list,  &vap_num);

	for(iLoop = 0; iLoop < vap_num; iLoop++)
	{
		sprintf(vap_name, "ap%d_%d", wifi_id, vap_list[iLoop]);
		//wifi_api_vap_set_channel(vap_name, (unsigned int)channel);
	}
}
s32 aglink_agconfig_ssid(AGLINK_AP_SSID_INFO_T *pstssid, s32 *pIndex, s32 iWifi, s32 iMax)
{
	/*变更配置文件,重启wifi服务*/
	u8 nvram_name[NVRAM_LEN] = {0}, nvram_value[NVRAM_LEN]={0};
	u8 vap_name[32];
	u8 wifi_name[32];
	s32 iLoop = 0, inloop = 0, vap_num = 0, vap_id=0, total = 0, iRet  = 0;

	u32 uiIndex=0;
	u32 uiVapIndex=0;
	
	AP_VAP_BASIC_T stVapBasic;

	if(iMax >SSID_NUMBER)
		iMax = SSID_NUMBER;

	for(iLoop = 0; iLoop < iMax; iLoop++)
	{
		/*update*/
		if(AG_SSID_UPDATE == pIndex[iLoop])
		{
			/*设置ssid  name*/
			pal_wifi_set_vap_uci(iLoop,"ssid",pstssid[iLoop].acSsid);

			pal_wifi_get_vap_name(iLoop,vap_name);

			pal_wifi_set_ssid(vap_name,pstssid[iLoop].acSsid);
			
			AGLINK_LOG(LOG_INFO, "iLoop:%d  ssid:%s vap:%s  vap_name:%s", 
				iLoop, pstssid[iLoop].acSsid, pstssid[iLoop].acIndex, vap_name);
		}
		/*ADD*/
		else if(AG_SSID_ADD == pIndex[iLoop])
		{
			memset(&stVapBasic,0,sizeof(AP_VAP_BASIC_T));
			stVapBasic.uiIndex=pal_wifi_get_vap_num();
			stVapBasic.uiWifiId = pstssid[iLoop].acIndex;
			stVapBasic.uiWifiId =0;

			strncpy(stVapBasic.network,"lan",strlen("lan"));
			memcpy(stVapBasic.acSsid,pstssid[iLoop].acSsid,strlen(pstssid[iLoop].acSsid));
			
			pal_wifi_add_vap_uci(&stVapBasic,iLoop);

			pal_wifi_get_vap_name(stVapBasic.uiIndex,stVapBasic.acVapName);
			
			AGLINK_LOG(LOG_INFO, "stVapBasic.uiIndex %d stVapBasic.uiWifiId %d  ,vap_name:%s  stVapBasic.acSsid%s ",\
								stVapBasic.uiIndex, stVapBasic.uiWifiId,stVapBasic.acVapName,stVapBasic.acSsid);

			/*添加vap*/
			pal_wifi_add_vap(stVapBasic.acVapName,stVapBasic.uiWifiId);
			/*配置ssid*/
			pal_wifi_set_ssid(stVapBasic.acVapName,stVapBasic.acSsid);
			/*添加到网桥*/
			pal_net_bridge_add_if(stVapBasic.network,stVapBasic.acVapName);
			
			/*开启vap*/
			aglink_device_up(stVapBasic.acVapName);
			
			AGLINK_LOG(LOG_INFO, "ssid name:%s iLoop:%d ", 
				pstssid[iLoop].acSsid,iLoop);
		
		}
		/*DEL*/
		else if(AG_SSID_DEL == pIndex[iLoop])
		{
			memset(&stVapBasic,0,sizeof(AP_VAP_BASIC_T));
			pal_wifi_get_vap_name(iLoop,stVapBasic.acVapName);

			pal_wifi_get_vap_network(iLoop,stVapBasic.network);

			AGLINK_LOG(LOG_INFO, "vap name:%s network :%s ssid name:%s iLoop:%d ",\ 
				stVapBasic.acVapName,stVapBasic.network,pstssid[iLoop].acSsid,iLoop);

			pal_net_bridge_del_if(stVapBasic.network,stVapBasic.acVapName);
			
			pal_wifi_delete_vap(stVapBasic.acVapName);

			

		}
			
	}

	/*del vap from uci*/
	for(iLoop = 0; iLoop < iMax; iLoop++)
	{
		if(AG_SSID_DEL == pIndex[iLoop])
		{
			pal_wifi_delete_vap_uci(iLoop-uiVapIndex);
			uiVapIndex++;
		}	
	}
	
	return TRUE;
}

s32 aglink_agconfig_update(AGLINK_AP_CONFIG_INFO_T *pstConfig)
{
	
}

#else
s32 aglink_set_channel(s32 wifi_id, s32 channel)
{
	u8 vap_name[32] = {0};
	s8 vap_list[128] = {0};
	s32 vap_num = 0, iLoop = 0;

	aglink_get_wlan_list(wifi_id,  vap_list,  &vap_num);

	for(iLoop = 0; iLoop < vap_num; iLoop++)
	{
		sprintf(vap_name, "ap%d_%d", wifi_id, vap_list[iLoop]);
		wifi_api_vap_set_channel(vap_name, (unsigned int)channel);
	}
}

s32 	aglink_set_max_user(s32 wifi_id, s32 max_user)
{
	u8 vap_name[32] = {0};
	s8 vap_list[128] = {0},  nvram_name[NVRAM_LEN]={0}, nvram_value[NVRAM_LEN] = {0};
	s32 vap_num = 0, iLoop = 0;

	aglink_get_wlan_list(wifi_id,  vap_list,  &vap_num);

	for(iLoop = 0; iLoop < vap_num; iLoop++)
	{
		sprintf(vap_name, "ap%d_%d", wifi_id, vap_list[iLoop]);
		wifi_api_vap_set_param(vap_name, WIFI_API_PARAMTYPE_MAX_AID, max_user);
		sprintf(nvram_name, NVRAM_VAP"%d"NVRAM_VAP_SSID_MAX_USERS, vap_list[iLoop]);
		sprintf(nvram_value, "%d", max_user);
		if(NvramResultFailed == nvram_set(nvram_name, nvram_value))
		{
			AGLINK_LOG(LOG_ALERT,, "set ssid max users failed, vap:%s, max_user:%d", vap_name, max_user);
		}
	}
	return TRUE;
}

s32 aglink_agconfig_ssid(AGLINK_AP_SSID_INFO_T *pstssid, s32 *pIndex, s32 iWifi, s32 iMax)
{
	WIFI_API_SSID_T st_ssid;
	u8 nvram_name[NVRAM_LEN] = {0}, nvram_value[NVRAM_LEN]={0};
	u8 vap_name[32];
	s32 iLoop = 0, inloop = 0, vap_num = 0, nvram_id=0, total = 0, iRet  = 0;
	s8 list_tmp[32] = {0};
	s8 list[128]  = {0}, vap_list[128] = {0};

	if(iMax >SSID_NUMBER)
		iMax = SSID_NUMBER;
	
	for(iLoop = 0; iLoop < iMax; iLoop++)
	{

		if(NvramResultFailed == nvram_get(list, NVRAM_VAP_LIST, sizeof(list)))
		{
		    AGLINK_LOG(LOG_ALERT, "get vap list failed.");
		    list[0]='\0';
		    if(0 == iWifi)
		    {
		        nvram_id = 0;
		    }
		    else if(1 == iWifi)
		    {
		        nvram_id = 16;
		    }
		}
		else
		{
		    aglink_array_string_to_int(list, list_tmp, &total);
		}
		
		/*update*/
		if(AG_SSID_UPDATE == pIndex[iLoop])
		{
			/*双频*/
			if(1 == gpstaglinkmgt->pstdev_info->stAgwifiInfo[1].iflag)
			{
				//nvram_id = iLoop;
				nvram_id = iLoop*2;
				if(1==iWifi)
				{
					//nvram_id = 16+iLoop;
					nvram_id = iLoop*2 + 1;
				}
			}
			else
			{
				nvram_id = iLoop;
			}
			
			/*设置ssid  name*/
			sprintf(nvram_name, "vap_%d"NVRAM_VAP_SSID_NAME, nvram_id);
			AGLINK_CONFIG_SET(nvram_name, pstssid[iLoop].acSsid);
			st_ssid.len = strlen(pstssid[iLoop].acSsid);
			memcpy(st_ssid.ssid, pstssid[iLoop].acSsid, st_ssid.len);
			sprintf(vap_name, "ap%d_%d", iWifi, iLoop);
			wifi_api_vap_set_ssid(vap_name, &st_ssid);
			AGLINK_LOG(LOG_INFO, "iLoop:%d nvram_id:%d ssid:%s vap:%s  vap_name:%s", 
				iLoop, nvram_id, pstssid[iLoop].acSsid, pstssid[iLoop].acIndex, vap_name);
		}
		/*ADD*/
		else if(AG_SSID_ADD == pIndex[iLoop])
		{
			sprintf(vap_name, "ap%d_%d", iWifi, iLoop);
			AGLINK_LOG(LOG_INFO, "vap_name:%s", vap_name);
			sprintf(nvram_value,"wifi%d", iWifi);
			if(FALSE ==aglink_add_vap(&pstssid[iLoop], vap_name, nvram_value))
			{
				return FALSE;
			}
			#if 0
			if(FALSE == aglink_get_wlan_list(iWifi, vap_list, &vap_num))
			{
				return FALSE;
			}
			#endif
			memset(vap_list, 0, sizeof(vap_list));
			sprintf(nvram_name, NVRAM_WIFI"%d"NVRAM_WIFI_WLAN_ID_LIST, iWifi);

			if(NvramResultFailed == nvram_get(vap_list, nvram_name, sizeof(vap_list)))
			{
			    AGLINK_LOG(LOG_ALERT, "get %s nvram name failed.", nvram_name);
			    return FALSE;
			}

			AGLINK_LOG(LOG_INFO, "vap_list:%s ", vap_list);
			#if 0
			/* 0~15: 2.4GHz, 16~31: 5GHz 二阶段做 5G not support*/
			for(inloop = 0; ((inloop < 32)&&(0 != total)); inloop++)
			{
				if(1 == iWifi && inloop < 16)
				{
					continue;
				}
			    if(inloop != list_tmp[inloop])
			    {
			        nvram_id = inloop;
			        break;
			    }
			}
			#endif
			/*双频*/
			if(1 == gpstaglinkmgt->pstdev_info->stAgwifiInfo[1].iflag)
			{
				//nvram_id = iLoop;
				nvram_id = iLoop*2;
				if(1==iWifi)
				{
					//nvram_id = 16+iLoop;
					nvram_id = iLoop*2 + 1;
				}
			}
			else
			{
				nvram_id = iLoop;
			}
			
			if(NvramResultFailed == aglink_vap_nvram_set(&pstssid[iLoop], nvram_id, iWifi, iLoop))
			{
				return FALSE;
			}
			aglink_array_string_add_someone(list, nvram_id);
			aglink_array_string_add_someone(vap_list, iLoop);
			AGLINK_LOG(LOG_INFO, "vap name:%s nvram_id:%d  iLoop:%d  list:%s vap_list:%s", 
				pstssid[iLoop].acSsid, nvram_id, iLoop, list, vap_list);
			if(NvramResultFailed == nvram_set(NVRAM_VAP_LIST, list))
			{
				AGLINK_LOG(LOG_ALERT, "set vap list failed");
				return FALSE;
			}
			sprintf(nvram_name, NVRAM_WIFI"%d"NVRAM_WIFI_WLAN_ID_LIST, iWifi);
			if(NvramResultFailed == nvram_set(nvram_name, vap_list))
			{
				AGLINK_LOG(LOG_ALERT, "set wlan list failed");
				return FALSE;
			}
		}
		/*DEL*/
		else if(AG_SSID_DEL == pIndex[iLoop])
		{
			/*双频*/
			if(1 == gpstaglinkmgt->pstdev_info->stAgwifiInfo[1].iflag)
			{
				//nvram_id = iLoop;
				nvram_id = iLoop*2;
				if(1==iWifi)
				{
					//nvram_id = 16+iLoop;
					nvram_id = iLoop*2 + 1;
				}
			}
			else
			{
				nvram_id = iLoop;
			}
			
			if(NvramResultFailed ==aglink_get_vap_name(pstssid[iLoop].acSsid, nvram_id))
			{
				AGLINK_LOG(LOG_ALERT, "get vap name error");
				return FALSE;
			}
			aglink_del_vap(pstssid[iLoop].acSsid);
			AGLINK_LOG(LOG_INFO, "vap_name:%s", pstssid[iLoop].acSsid);
			#if 0
			if(FALSE == aglink_get_wlan_list(iWifi, vap_list, &vap_num))
			{
				return FALSE;
			}
			#endif
			memset(vap_list, 0, sizeof(vap_list));
			sprintf(nvram_name, NVRAM_WIFI"%d"NVRAM_WIFI_WLAN_ID_LIST, iWifi);

			if(NvramResultFailed == nvram_get(vap_list, nvram_name, sizeof(vap_list)))
			{
			    AGLINK_LOG(LOG_ALERT, "get %s nvram name failed.", nvram_name);
			    return FALSE;
			}
			
			aglink_vap_nvram_unset(nvram_id);
			aglink_array_string_del_someone(list, nvram_id);
        		aglink_array_string_del_someone(vap_list, iLoop);
			AGLINK_LOG(LOG_INFO, "vap name:%s nvram_id:%d  iLoop:%d list:%s vap_list:%s", 
				pstssid[iLoop].acSsid, nvram_id, iLoop, list, vap_list);

			if(NvramResultFailed == nvram_set(NVRAM_VAP_LIST, list))
			{
				AGLINK_LOG(LOG_ALERT, "set vap list failed");
				return FALSE;
			}
			sprintf(nvram_name, NVRAM_WIFI"%d"NVRAM_WIFI_WLAN_ID_LIST, iWifi);
			if(NvramResultFailed == nvram_set(nvram_name, vap_list))
			{
				AGLINK_LOG(LOG_ALERT, "set wlan list failed");
				return FALSE;
			}
		}
			
	}
	return TRUE;
}


s32 aglink_agconfig_update_fat(AGLINK_AP_CONFIG_INFO_T *pstConfig)
{
	u8 acShellcmd[128] = {0};

	if(NULL == pstConfig)
	{
		AGLINK_LOG(LOG_ALERT, "Param NULL!");
		return FALSE;
	}

	if(0 != memcmp(pstConfig->acMac, gpstaglinkmgt->pstdev_info->phy_mac, MAC_LEN))
	{
		AGLINK_LOG(LOG_DEBUG, "MAC ERROR pstConfig->acMac:"MAC_FMT" gpstaglinkmgt->pstdev_info->phy_mac:"MAC_FMT, 
			MAC_ARG(pstConfig->acMac), MAC_ARG(gpstaglinkmgt->pstdev_info->phy_mac));
		return FALSE;
	}

	if(memcmp(pstConfig->acMemo, gpstaglinkmgt->pAgconfig->acMemo, NVRAM_LEN*4))	
	{
		AGLINK_CONFIG_SET(AG_CONFIG_MEMO, pstConfig->acMemo);
		memcpy(gpstaglinkmgt->pAgconfig->acMemo, pstConfig->acMemo, NVRAM_LEN*4);
	}


	if(memcmp(pstConfig->acLocation, gpstaglinkmgt->pAgconfig->acLocation, NVRAM_LEN*2))
	{
		AGLINK_CONFIG_SET(AG_CONFIG_LOCATION, pstConfig->acLocation);
		memcpy(gpstaglinkmgt->pAgconfig->acLocation, pstConfig->acLocation, NVRAM_LEN*2);
	}

	if(pstConfig->iSsidNum != gpstaglinkmgt->pAgconfig->iSsidNum ||memcmp(pstConfig->ssid, gpstaglinkmgt->pAgconfig->ssid, sizeof(AGLINK_AP_SSID_INFO_T)*SSID_NUMBER))
	{
		int iIndex[SSID_NUMBER] = {0}, iLoop = 0, iMax = 0, iFlag = 0, iMin = 0;
		iMax = pstConfig->iSsidNum > gpstaglinkmgt->pAgconfig->iSsidNum? pstConfig->iSsidNum: gpstaglinkmgt->pAgconfig->iSsidNum;
		iMin = pstConfig->iSsidNum < gpstaglinkmgt->pAgconfig->iSsidNum? pstConfig->iSsidNum: gpstaglinkmgt->pAgconfig->iSsidNum;

		AGLINK_LOG(LOG_INFO, "stSsidCfg.iSsidNum:%d gpstaglinkmgt->pAgconfig->iSsidNum:%d", 
					pstConfig->iSsidNum , gpstaglinkmgt->pAgconfig->iSsidNum);

		/*查看需要更新的内容*/
		for(iLoop = 0; iLoop < iMin; iLoop++)
		{
			if(memcmp(&pstConfig->ssid[iLoop].acSsid, &gpstaglinkmgt->pAgconfig->ssid[iLoop].acSsid, 32))
			{
				iIndex[iLoop] = AG_SSID_UPDATE;
			}
		}

		/*ADD VAP*/
		if(pstConfig->iSsidNum > gpstaglinkmgt->pAgconfig->iSsidNum)
		{
			for(iLoop; iLoop < iMax; iLoop++)
			{
				iIndex[iLoop] = AG_SSID_ADD;
			}
		}
		/*Del ap*/
		else if(pstConfig->iSsidNum < gpstaglinkmgt->pAgconfig->iSsidNum)
		{	
			for(iLoop; iLoop < iMax; iLoop++)
			{
				iIndex[iLoop] = AG_SSID_DEL;
			}
		}

		for(iLoop = 0; iLoop < iMax; iLoop++)
		{
			AGLINK_LOG(LOG_INFO, "iIndex[%d]=%d", iLoop, iIndex[iLoop]);
		}

		if(FALSE == aglink_agconfig_ssid(pstConfig->ssid, iIndex, 0, iMax))
		{
			return FALSE;
		}
		if(1 == gpstaglinkmgt->pstdev_info->stAgwifiInfo[1].iflag)
		{
			if(FALSE == aglink_agconfig_ssid(pstConfig->ssid, iIndex, 1, iMax))
			{
				return FALSE;
			}
		}
		
		gpstaglinkmgt->pAgconfig->iSsidNum = pstConfig->iSsidNum;
		//memcpy(gpstaglinkmgt->pAgconfig->ssid, pstConfig->ssid, sizeof(AGLINK_AP_SSID_INFO_T)*SSID_NUMBER);
		for(iLoop = 0; iLoop < pstConfig->iSsidNum; iLoop++)
		{
			memcpy(gpstaglinkmgt->pAgconfig->ssid[iLoop].acSsid, pstConfig->ssid[iLoop].acSsid, sizeof(pstConfig->ssid[iLoop].acSsid));
		}
		
	}

	if(pstConfig->iCha2 != gpstaglinkmgt->pAgconfig->iCha2)
	{
		sprintf(acShellcmd, "%d", pstConfig->iCha2);
		AGLINK_CONFIG_SET(AG_CONFIG_CHANNEL_24, acShellcmd);

		aglink_down_all_vap(0);
        	aglink_set_channel(0, pstConfig->iCha2);
        	aglink_up_all_vap(0);
	}

	if(pstConfig->iCha5 != gpstaglinkmgt->pAgconfig->iCha5)
	{
		sprintf(acShellcmd, "%d", pstConfig->iCha5);
		AGLINK_CONFIG_SET(AG_CONFIG_CHANNEL_5, acShellcmd);
		aglink_down_all_vap(1);
        	aglink_set_channel(1, pstConfig->iCha2);
        	aglink_up_all_vap(1);
	}


	if(pstConfig->total_link != gpstaglinkmgt->pAgconfig->total_link)
	{
		memset(acShellcmd, 0 ,sizeof(acShellcmd));
		sprintf(acShellcmd, "%d", pstConfig->total_link);
		AGLINK_CONFIG_SET(AG_CONFIG_TOTAL_LINK, acShellcmd);
		gpstaglinkmgt->pAgconfig->total_link = pstConfig->total_link;
		aglink_down_all_vap(0);
        	aglink_set_max_user(0, pstConfig->total_link);
        	aglink_up_all_vap(0);
	}
	return TRUE;
}

s32 aglink_agconfig_update_fit(AGLINK_AP_CONFIG_INFO_T *pstConfig)
{
	if(NULL == pstConfig)
	{
		AGLINK_LOG(LOG_ALERT, "Param NULL!");
		return FALSE;
	}

	if(0 != memcmp(pstConfig->acMac, gpstaglinkmgt->pstdev_info->phy_mac, MAC_LEN))
	{
		AGLINK_LOG(LOG_DEBUG, "MAC ERROR pstConfig->acMac:"MAC_FMT" gpstaglinkmgt->pstdev_info->phy_mac:"MAC_FMT, 
			MAC_ARG(pstConfig->acMac), MAC_ARG(gpstaglinkmgt->pstdev_info->phy_mac));
		return FALSE;
	}

	if(memcmp(pstConfig->acMemo, gpstaglinkmgt->pAgconfig->acMemo, NVRAM_LEN*4))	
	{
		AGLINK_CONFIG_SET(AG_CONFIG_MEMO, pstConfig->acMemo);
		memcpy(gpstaglinkmgt->pAgconfig->acMemo, pstConfig->acMemo, NVRAM_LEN*4);
	}

	
	return TRUE;
}
#endif
s32 aglink_recv_config_msg(u8 **pnt, u32 uiMsgLen)
{
	u8 ucgwmac[MAC_LEN] = {0};
	u8 ucShellCmd[128] = {0};
	AGLINK_AP_CONFIG_INFO_T stConfig = {0};
	s32 iLoop = 0;
	if(NULL == pnt)
	{
		AGLINK_LOG(LOG_ALERT, "Param NULL!");
		return FALSE;
	}
	/*if((uiMsgLen-sizeof(AG_LINK_MSG_HEAD_T))!=(MAC_LEN+sizeof(AGLINK_AP_CONFIG_INFO_T)))
	{
		AGLINK_LOG(LOG_ALERT, "Msg length(%d) error", uiMsgLen-sizeof(AG_LINK_MSG_HEAD_T));
		return FALSE;
	}*/

	DECODE_GET(ucgwmac, pnt, MAC_LEN);

	if(0 != memcmp(ucgwmac, gpstaglinkmgt->mac, MAC_LEN))
	{
		AGLINK_LOG(LOG_DEBUG, "MAC ERROR gpstaglinkmgt->mac:"MAC_FMT" gwmac:"MAC_FMT, MAC_ARG(gpstaglinkmgt->mac), MAC_ARG(ucgwmac));
		return FALSE;
	}

	DECODE_GET(&stConfig, pnt, sizeof(AGLINK_AP_CONFIG_INFO_T));

	AG_NTOHL(stConfig.iCha2);
	AG_NTOHL(stConfig.iCha5);
	AG_NTOHL(stConfig.total_link);
	AG_NTOHL(stConfig.iSsidNum);
	for(iLoop = 0; iLoop < SSID_NUMBER; iLoop++)
	{
		AG_NTOHL(stConfig.ssid[iLoop].iUser_num);
	}
	

	AGLINK_LOG(LOG_INFO, "Location:%s iCha2:%d iCha5:%d total_link:%d Memo:%s, iSsidNum:%d",
		stConfig.acLocation, stConfig.iCha2, stConfig.iCha5, stConfig.total_link, stConfig.acMemo, stConfig.iSsidNum);
	for(iLoop = 0; iLoop < stConfig.iSsidNum; iLoop++)
	{
		AGLINK_LOG(LOG_INFO, "SSID[%d]:%s", iLoop,  stConfig.ssid[iLoop].acSsid);
	}
	#ifdef OPENWRT
	aglink_agconfig_update(&stConfig);
	#else
	if(AGLINK_FAT_AP == gpstaglinkmgt->pstdev_info->ap_work_mode)
	{
		aglink_agconfig_update_fat(&stConfig);
	}
	else
	{
		aglink_agconfig_update_fit(&stConfig);
	}
	#endif
	
	if(NvramResultSuccess != nvram_commit())
	{
		AGLINK_LOG(LOG_ALERT, "nvram commit failed");
		return FALSE;
	}
	return TRUE;
}

s32 aglink_recv_ssidcfg_msg(u8 **pnt, u32 uiMsgLen)
{	
	u8 ucgwmac[MAC_LEN] = {0};
	AGLINK_AP_SSID_CFG_T stSsidCfg = {0};
	s32 iLoop = 0, iRet = 0;
	
	if(NULL == pnt)
	{
		AGLINK_LOG(LOG_ALERT, "Param NULL!");
		return FALSE;
	}
	/*if((uiMsgLen-sizeof(AG_LINK_MSG_HEAD_T))!=sizeof(AGLINK_AP_SSID_CFG_T))
	{
		AGLINK_LOG(LOG_ALERT, "Msg length(%d) error", uiMsgLen-sizeof(AG_LINK_MSG_HEAD_T));
		return FALSE;
	}*/

	DECODE_GET(ucgwmac, pnt, MAC_LEN);

	if(0 != memcmp(ucgwmac, gpstaglinkmgt->mac, MAC_LEN))
	{
		AGLINK_LOG(LOG_ALERT, "MAC ERROR gpstaglinkmgt->mac:"MAC_FMT" gwmac:"MAC_FMT, MAC_ARG(gpstaglinkmgt->mac), MAC_ARG(ucgwmac));
		return FALSE;
	}

	DECODE_GET(&stSsidCfg, pnt, sizeof(AGLINK_AP_SSID_CFG_T));

	AG_NTOHL(stSsidCfg.iSsidNum);
	for(iLoop= 0; iLoop < SSID_NUMBER; iLoop++)
	{
		AG_NTOHL(stSsidCfg.ssid[iLoop].iUser_num);
	}

	if(stSsidCfg.iSsidNum != gpstaglinkmgt->pAgconfig->iSsidNum ||memcmp(stSsidCfg.ssid, gpstaglinkmgt->pAgconfig->ssid, sizeof(AGLINK_AP_SSID_INFO_T)*SSID_NUMBER))
	{
		int iIndex[SSID_NUMBER] = {0}, iLoop = 0, iMax = 0, iFlag = 0, iMin = 0;
		iMax = stSsidCfg.iSsidNum > gpstaglinkmgt->pAgconfig->iSsidNum? stSsidCfg.iSsidNum: gpstaglinkmgt->pAgconfig->iSsidNum;
		iMin = stSsidCfg.iSsidNum < gpstaglinkmgt->pAgconfig->iSsidNum? stSsidCfg.iSsidNum: gpstaglinkmgt->pAgconfig->iSsidNum;

		AGLINK_LOG(LOG_INFO, "stSsidCfg.iSsidNum:%d gpstaglinkmgt->pAgconfig->iSsidNum:%d", 
			stSsidCfg.iSsidNum , gpstaglinkmgt->pAgconfig->iSsidNum);
		
		/*查看需要更新的内容*/
		for(iLoop = 0; iLoop < iMin; iLoop++)
		{
			if(memcmp(&stSsidCfg.ssid[iLoop].acSsid, &gpstaglinkmgt->pAgconfig->ssid[iLoop].acSsid, 32))
			{
				iIndex[iLoop] = AG_SSID_UPDATE;
			}
		}

		/*ADD VAP*/
		if(stSsidCfg.iSsidNum > gpstaglinkmgt->pAgconfig->iSsidNum)
		{
			for(iLoop; iLoop < iMax; iLoop++)
			{
				iIndex[iLoop] = AG_SSID_ADD;
			}
		}
		/*Del ap*/
		else if (stSsidCfg.iSsidNum < gpstaglinkmgt->pAgconfig->iSsidNum)
		{	
			for(iLoop; iLoop < iMax; iLoop++)
			{
				iIndex[iLoop] = AG_SSID_DEL;
			}
		}

		for(iLoop = 0; iLoop < iMax; iLoop++)
		{
			AGLINK_LOG(LOG_INFO, "iIndex[%d]=%d", iLoop, iIndex[iLoop]);
		}


		AGLINK_LOG(LOG_INFO, "2.4G:%d  5G:%d", gpstaglinkmgt->pstdev_info->stAgwifiInfo[0].iflag, gpstaglinkmgt->pstdev_info->stAgwifiInfo[1].iflag);
		
		if(FALSE == aglink_agconfig_ssid(stSsidCfg.ssid, iIndex, 0, iMax))
		{
			return FALSE;
		}
		
		
		gpstaglinkmgt->pAgconfig->iSsidNum = stSsidCfg.iSsidNum;
		//memcpy(gpstaglinkmgt->pAgconfig->ssid, stSsidCfg.ssid, sizeof(AGLINK_AP_SSID_INFO_T)*SSID_NUMBER);
		for(iLoop = 0; iLoop < stSsidCfg.iSsidNum; iLoop++)
		{
			memcpy(gpstaglinkmgt->pAgconfig->ssid[iLoop].acSsid, stSsidCfg.ssid[iLoop].acSsid, sizeof(stSsidCfg.ssid[iLoop].acSsid));
		}
	}

	if(NvramResultSuccess != nvram_commit())
	{
		AGLINK_LOG(LOG_ALERT, "nvram commit failed");
		return FALSE;
	}
	
	return TRUE;
}

#endif
int aglink_ag_socket_init()
{
#define SEND_TIMEOUT_VALUE	10
	struct timeval timeout = {SEND_TIMEOUT_VALUE, 0};

	s32 sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	AGLINK_LOG(LOG_INFO, "socket init.");
	
	bzero(&gpstaglinkmgt->sk_addr, sizeof(gpstaglinkmgt->sk_addr));
	gpstaglinkmgt->sk_addr.sin_family = AF_INET;
	gpstaglinkmgt->sk_addr.sin_port = htons(gpstaglinkmgt->port);
	gpstaglinkmgt->sk_addr.sin_addr.s_addr = gpstaglinkmgt->ipaddr;

	if(IS_DEV_GW()){
		/* bind address and port to socket */
		if(bind(sockfd, (struct sockaddr *)&gpstaglinkmgt->sk_addr, sizeof(gpstaglinkmgt->sk_addr)) == -1)
		{
			AGLINK_LOG(LOG_ERR, "socket bind error.");
			close(sockfd);
			exit(-1);
		}
	}

	/*设置server fd发送超时为timeval*/
	setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(struct timeval));
	/*设置server fd接收超时为timeval*/
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));

	gpstaglinkmgt->sockfd = sockfd;

	return 0;
}

/* 
 * ===  FUNCTION  =========================================
 *  Name:  aglink_parse_ag_msg
 *  Description: 消息解析函数,若发给主线程则在本函数中处理，若为其他模块消息，则调用IPC 线程接口就行处理
 *  Input : buf:消息buf
 *          len:消息长度
 *          sk_addr:消息发送方的的skaddr
 *  Output: none
 *  Return: 0: success <0: fail
 * ====================================================
 */
s32 aglink_parse_ag_msg(u8 *buf, u32 len, struct sockaddr_in *sk_addr)
{
	s8 i;
	u8 *p = buf;
	u8 **pnt = &p;
	u32 rlen = 0;
	s32 ret = 0;
	AG_LINK_MSG_HEAD_T stMsgHead;

	if(len < sizeof(AG_LINK_MSG_HEAD_T))
	{
		return FALSE;
	}
	
	memset(&stMsgHead, 0, sizeof(AG_LINK_MSG_HEAD_T));
	/* 解析消息头 */
	//DECODE_GET(&stMsgHead, pnt, sizeof(AG_LINK_MSG_HEAD_T));
	DECODE_GETL(pnt, stMsgHead.usMsgId);
	DECODE_GETW(pnt, stMsgHead.usMsgType);
	DECODE_GETW(pnt, stMsgHead.uiUnicast);
	DECODE_GET(stMsgHead.acname, pnt, MODULE_NAME_LENGTH);
	DECODE_GETL(pnt, stMsgHead.uiMsgLen);
	AGLINK_LOG(LOG_INFO, "Module name:%s MsgId:%d MsgType:%d, MsgLen:%d", stMsgHead.acname, stMsgHead.usMsgId, stMsgHead.usMsgType, stMsgHead.uiMsgLen);

	/* ! aglink module   the message dispath by IPC thread */
	#if 0
	if(0 != memcmp(stMsgHead.acname, "aglink", strlen("aglink")))
	{
		ag_module_ipc_server_send_msg_to_internal(&stMsgHead, buf, len, NULL);
		return FALSE;
	}
	#endif
	if(stMsgHead.usMsgId > AGLINK_MSG_MAX)
	{
		ag_module_ipc_server_send_msg_to_internal(&stMsgHead, buf, len, NULL);
		return FALSE;
	}
	
	switch(stMsgHead.usMsgId)
	{
	case AGLINK_MSG_DEV_REGISTER_MSG:
		ret = aglink_gw_recv_dev_register_msg(pnt, sk_addr);
		break;
	case AGLINK_MSG_DEV_REGISTER_ACK:
		ret = aglink_ap_recv_dev_register_ack_msg(pnt);
		break;
	case AGLINK_MSG_DEV_NAK_MSG:
		ret = aglink_ap_recv_dev_nak_msg(pnt);
		break;
	case AGLINK_MSG_DEV_KEEPLIVE_MSG:
		ret = aglink_gw_recv_dev_keepalive_msg(pnt, sk_addr);
		break;
	case AGLINK_MSG_DEV_KEEPLIVE_ACK_MSG:
		ret = aglink_ap_recv_dev_keepalive_ack_msg(pnt);
		break;
	case AGLINK_MSG_AP_INFO:
		AGLINK_LOG(LOG_INFO, "recv fit ap info msg");
		aglink_gw_recv_ap_info_msg(stMsgHead, pnt);
		break;
	#if 0	
	case AGLINK_IDSET_DEV_LOGOUT_ACK:
		AGLINK_LOG(LOG_INFO, "recv dev logout ack msg.");
		break;
	case AGLINK_IDSET_STA_ADD:
		AGLINK_LOG(LOG_INFO, "recv AGLINK_IDSET_STA_ADD msg");
		aglink_gw_recv_sta_connect_msg(pnt, stMsgHead.usMsgId);
		break;
	case AGLINK_IDSET_STA_DEL:
		AGLINK_LOG(LOG_INFO, "recv AGLINK_IDSET_STA_DEL msg");
		aglink_gw_recv_sta_connect_msg(pnt, stMsgHead.usMsgId);
		break;
	#endif		
	#ifndef GW_X86
	#if 0
	case AGLINK_CLI_GET_MODULE_INFO:
		AGLINK_LOG(LOG_INFO, "recv AGLINK_CLI_GET_MODULE_INFO");
		aglink_get_module_info();
		break;
	case AGLINK_CLI_GET_ONLINE_INFO:
		AGLINK_LOG(LOG_INFO, "recv AGLINK_CLI_GET_ONLINE_INFO");
		aglink_get_online_info();
		break;
	case AGLINK_CLI_GET_AP_LIST:
		AGLINK_LOG(LOG_INFO, "recv AGLINK_CLI_GET_AP_LIST");
		aglink_get_ap_list(pnt, stMsgHead.uiMsgLen);
		break;
	case AGLINK_CLI_SET_KEEPLIVE_TM:
		AGLINK_LOG(LOG_INFO, "recv AGLINK_CLI_SET_KEEPLIVE_TM");
		aglink_keeplive_tm(pnt, stMsgHead.uiMsgLen);
		break;
	#endif
	case AGLINK_MSG_DEV_REBOOT_MSG:
		AGLINK_LOG(LOG_INFO, "recv AGLINK_MSG_DEV_REBOOT_MSG");
		aglink_recv_reboot_msg(pnt, stMsgHead.uiMsgLen);
		break;
	case AGLINK_MSG_DEV_UPDATE_MSG:
		AGLINK_LOG(LOG_INFO, "recv AGLINK_MSG_DEV_UPDATE_MSG");
		aglink_recv_update_msg(pnt, stMsgHead.uiMsgLen);
		break;
	case AGLINK_MSG_DEV_CONFIG_MSG:
		AGLINK_LOG(LOG_INFO, "recv AGLINK_MSG_DEV_CONFIG_MSG");
		aglink_recv_config_msg(pnt, stMsgHead.uiMsgLen);
		break;
	case AGLINK_MSG_DEV_SSIDCFG_MSG:
		AGLINK_LOG(LOG_INFO, "recv AGLINK_MSG_DEV_SSIDCFG_MSG");
		aglink_recv_ssidcfg_msg(pnt, stMsgHead.uiMsgLen);
		break;

	case AGLINK_MSG_SYN_AP_LIST:
		AGLINK_LOG(LOG_INFO, "recv AGLINK_MSG_SYN_AP_LIST");
		
		break;
	#endif	
		#if 0
	case STA_REGISTER_ACK_MSG:
		AGLINK_LOG(LOG_INFO, "recv user register ack msg.");
		break;
	case STA_CONNECT_MSG:
		aglink_gw_recv_sta_connect_msg(pnt, hdr.msg_type);
		break;
	case STA_DISCONNECT_MSG:
		aglink_gw_recv_sta_connect_msg(pnt, hdr.msg_type);
		break;
	case STA_DETECT_MSG:
		AGLINK_LOG(LOG_DEBUG, "recv sta detect msg");
		aglink_gw_recv_stadct_msg(pnt);
		break;
	case STA_DETECT_TIME_MSG:
		AGLINK_LOG(LOG_DEBUG, "recv sta detect time msg");
		aglink_ap_recv_stadct_time_msg(pnt);
		break;
	case STA_DETECT_ENABLE_MSG:
		AGLINK_LOG(LOG_DEBUG, "recv sta detect enable msg");
		aglink_ap_recv_stadct_enable_msg(TRUE);
		break;
	case STA_DETECT_DISABLE_MSG:
		AGLINK_LOG(LOG_DEBUG, "recv sta detect diable msg");
		aglink_ap_recv_stadct_enable_msg(FALSE);
		break;
	case STA_DETECT_RSSI_MSG:
		AGLINK_LOG(LOG_INFO, "recv sta detect rssi msg");
		aglink_ap_recv_stadct_rssi_msg(pnt);
		break;
		#endif
	default:
		break;
	}
	
	return ret;
}

s32 aglink_parse_agcli_msg(u8 *buf, u32 len, struct sockaddr_in *sk_addr)
{
	
	s8 i;
	u8 *p = buf;
	u8 **pnt = &p;
	u32 rlen = 0;
	s32 ret = 0;
	AG_LINK_MSG_HEAD_T stMsgHead;

	if(len < sizeof(AG_LINK_MSG_HEAD_T))
	{
		return FALSE;
	}
	
	memset(&stMsgHead, 0, sizeof(AG_LINK_MSG_HEAD_T));
	/* 解析消息头 */
	//DECODE_GET(&stMsgHead, pnt, sizeof(AG_LINK_MSG_HEAD_T));
	DECODE_GETL(pnt, stMsgHead.usMsgId);
	DECODE_GETW(pnt, stMsgHead.usMsgType);
	DECODE_GETW(pnt, stMsgHead.uiUnicast);
	DECODE_GET(stMsgHead.acname, pnt, MODULE_NAME_LENGTH);
	DECODE_GETL(pnt, stMsgHead.uiMsgLen);
	AGLINK_LOG(LOG_INFO, "Module name:%s MsgId:%d MsgType:%d, MsgLen:%d", stMsgHead.acname, stMsgHead.usMsgId, stMsgHead.usMsgType, stMsgHead.uiMsgLen);

	switch(stMsgHead.usMsgId)
	{
#ifndef GW_X86
		case AGLINK_CLI_GET_MODULE_INFO:
			AGLINK_LOG(LOG_INFO, "recv AGLINK_CLI_GET_MODULE_INFO");
			aglink_get_module_info();
			break;
		case AGLINK_CLI_GET_ONLINE_INFO:
			AGLINK_LOG(LOG_INFO, "recv AGLINK_CLI_GET_ONLINE_INFO");
			aglink_get_online_info();
			break;
		case AGLINK_CLI_GET_AP_LIST:
			AGLINK_LOG(LOG_INFO, "recv AGLINK_CLI_GET_AP_LIST");
			aglink_get_ap_list(pnt, stMsgHead.uiMsgLen);
			break;
		case AGLINK_CLI_SET_KEEPLIVE_TM:
			AGLINK_LOG(LOG_INFO, "recv AGLINK_CLI_SET_KEEPLIVE_TM");
			aglink_keeplive_tm(pnt, stMsgHead.uiMsgLen);
			break;
#endif			
		default:
			AGLINK_LOG(LOG_ALERT, "recv MsgId:%d", stMsgHead.usMsgId);
			break;
	}

	return TRUE;
}
/* 
 * ===  FUNCTION  =========================================
 *  Name:  aglink_recv_ag_msg
 *  Description: 接收消息
 *  Input : fd: event相关fd
 *		   event: event相关事件
 *		   arg:相关参数
 *  Output: none
 *  Return: 0: success <0: fail
 * ====================================================
 */
void aglink_recv_ag_msg(evutil_socket_t fd, short event, void *arg)
{
	s32 size;
	s32 sockfd = gpstaglinkmgt->sockfd;
	static u32 restlen = 0;
	u8 ucRecvbuf[AG_BUFFER_SIZE] = {0};
	struct sockaddr_in sk_addr;
	socklen_t len = sizeof(sk_addr);

	size = recvfrom(sockfd, (void *)ucRecvbuf, AG_BUFFER_SIZE, 0, (struct sockaddr *)&sk_addr, &len); 
	if (-1 == size){
		AGLINK_LOG(LOG_ERR, "receive data error!\n");

		if(IS_DEV_AP()){
			event_del(&gpstaglinkmgt->keepalive_timer_ev);
			gpstaglinkmgt->keepalive_times = 0;
		}

		event_del(&gpstaglinkmgt->rcv_ev);
		close(gpstaglinkmgt->sockfd);
		gpstaglinkmgt->sockfd = 0;

		if(IS_DEV_AP()){
			sleep(1);
			/*初始化网关-AP通信socket*/
			if(aglink_ag_socket_init() < 0){
				AGLINK_LOG(LOG_ERR, "socket init failed!");
				exit(-1);
			}
#ifndef GW_X86			
			/*发送设备注册消息*/
			aglink_ap_send_dev_register_msg();
#endif
		}
	}
	else{
		aglink_parse_ag_msg(ucRecvbuf, size, &sk_addr);
	}

	return;
}



/*****************************************************************************
 Prototype    : aglink_send_ag_msg
 Description  :  aglink主线程 发送消息函数 if MsgID/MsgType is NULL , 不封装他头部
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/12/2
    Author       : lvjianlin
    Modification : Created function

*****************************************************************************/
u32 aglink_send_ag_msg(u32 usMsgID, u16 uiMsgType, u8 *pbuff, u32 iMsgLen, u8 *dmac)
{
	AG_LINK_MSG_HEAD_T stMsgHead;
	u8 ucSendbuf[AG_BUFFER_SIZE] = {0};
	u8 *p = ucSendbuf;
	u8 **pnt = &p;
	u32 uitotallen;
	
	AGLINK_LOG(LOG_INFO, "Send ag msg (type %d)", uiMsgType);

	/* 不需要封装头部 */
	if((NULL ==usMsgID) &&(NULL == uiMsgType))
	{
		uitotallen = iMsgLen;

		if(uitotallen > AG_BUFFER_SIZE)
		{
			AGLINK_LOG(LOG_ALERT, "Msg length is %d ,bigger than AG_BUFFER_SIZE", uitotallen);
			return FALSE;
		}
		
		//AG_MSG_LOCK();
	}
	else
	{
		uitotallen = iMsgLen + sizeof(AG_LINK_MSG_HEAD_T);

		if(uitotallen > AG_BUFFER_SIZE)
		{
			AGLINK_LOG(LOG_ALERT, "Msg length is %d ,bigger than AG_BUFFER_SIZE", uitotallen);
			return FALSE;
		}
		memset(&stMsgHead, 0 ,sizeof(AG_LINK_MSG_HEAD_T));
		stMsgHead.usMsgId = usMsgID;
		stMsgHead.usMsgType = uiMsgType;
		memcpy(&stMsgHead.acname, "aglink", strlen("aglink"));
		stMsgHead.uiMsgLen = iMsgLen;

		//AG_MSG_LOCK();
		/* 封装头部 */
		//ENCODE_PUT(pnt, &stMsgHead, sizeof(AG_LINK_MSG_HEAD_T));
		ENCODE_PUTL(pnt, stMsgHead.usMsgId);
		ENCODE_PUTW(pnt, stMsgHead.usMsgType);		
		ENCODE_PUTW(pnt, stMsgHead.uiUnicast);
		ENCODE_PUT(pnt,stMsgHead.acname, MODULE_NAME_LENGTH);
		ENCODE_PUTL(pnt, stMsgHead.uiMsgLen);
		

		AGLINK_LOG(LOG_INFO, "MsgId:%d MsgType:%d ModuleName:%s MsgLen:%d uitotallen:%d AG_LINK_MSG_HEAD_T:%d ", 
			stMsgHead.usMsgId, stMsgHead.usMsgType, stMsgHead.acname, stMsgHead.uiMsgLen, uitotallen, sizeof(AG_LINK_MSG_HEAD_T));
	}
	
	/* 封装消息体 */
	if(iMsgLen > 0)
	{
		ENCODE_PUT(pnt, pbuff, iMsgLen);
	}

	/*发送消息*/
	if(IS_DEV_AP())
	{

		AG_MODULE_LOG(LOG_INFO, "Socket:%d  Dest ip:"IP_FMT" port:%d", gpstaglinkmgt->sockfd,
			IP_ARG(&(gpstaglinkmgt->sk_addr.sin_addr.s_addr)), ntohs(gpstaglinkmgt->sk_addr.sin_port));
		
		if( sendto(gpstaglinkmgt->sockfd, ucSendbuf, uitotallen, 0, 
			(struct sockaddr *)&gpstaglinkmgt->sk_addr, sizeof(gpstaglinkmgt->sk_addr)) < 0)
		{
			AGLINK_LOG(LOG_ERR, "send msg error: %s(errno: %d)", strerror(errno), errno);
			//AG_MSG_UNLOCK();
			return -1;
		}
	}
	else
	{
		struct hlist_node *pos;
		struct hlist_head *head;
		ap_node *ap;

		if(dmac != NULL){ /*单播发送*/
			head = aglink_mac_hashfn(dmac, gpstaglinkmgt->ap_list, AP_HASH_SIZE);
			
			AP_HLIST_LOCK();
			hlist_for_each(pos, head)
			{
				ap = hlist_entry(pos, ap_node, id_hlist);
				if (!memcmp(ap->mac, dmac, MAC_LEN)){
					if( sendto(gpstaglinkmgt->sockfd, ucSendbuf, uitotallen, 0, 
						(struct sockaddr *)&ap->sk_addr, sizeof(ap->sk_addr)) < 0)
					{
						AGLINK_LOG(LOG_ERR, "send msg error: %s(errno: %d)", strerror(errno), errno);
						AP_HLIST_UNLOCK();
						//AG_MSG_UNLOCK();
						return -1;
					}
					break;
				}
			}		
			AP_HLIST_UNLOCK();
		}
		else{/*广播发送*/
			s32 i;
			AP_HLIST_LOCK();
			for(i = 0; i < AP_HASH_SIZE; ++i){
				head = &gpstaglinkmgt->ap_list[i];
				
				hlist_for_each(pos, head)
				{
					ap = hlist_entry(pos, ap_node, id_hlist);
					if( sendto(gpstaglinkmgt->sockfd, ucSendbuf, uitotallen, 0, 
						(struct sockaddr *)&ap->sk_addr, sizeof(ap->sk_addr)) < 0)
					{
						AGLINK_LOG(LOG_ERR, "send ap "MAC_FMT"msg error: %s(errno: %d)", 
							MAC_ARG(ap->mac), strerror(errno), errno);
					}
				}
			}
			AP_HLIST_UNLOCK();	
		}
	}	
	//AG_MSG_UNLOCK();
	
	return TRUE;
}

s32 aglink_send_apmsg_to_wifidog(u8 * mac, u8 * ip, u32 model_id, s8 * dev_name, AP_WIFI_INFO *pstApInfo, s32 type)
{
	u8 buf[AG_BUFFER_SIZE] = {0};
	u8 *p = &buf[0];
	u8 **pnt = &p;
	WIFIOG_AP_REGISTER_T stMsg;

	memset(&stMsg, 0, sizeof(WIFIOG_AP_REGISTER_T));
	
	AGLINK_LOG(LOG_DEBUG, "aglink send ap msg to wifidog, apmac "MAC_FMT" dev_name:%s type:%d",
		MAC_ARG(mac), dev_name, type);

	memcpy(stMsg.aucMac, mac, MAC_LEN);
	memcpy(stMsg.aucIp, ip, IP_LEN);
	stMsg.uiModelId = htonl(model_id);
	memcpy(stMsg.acName, dev_name, strlen(dev_name));
	if(NULL != pstApInfo)
	{
		memcpy(&stMsg.stApInfo, pstApInfo, sizeof(AP_WIFI_INFO));
	}
	
	if(DEV_LOGOUT_MSG == type)
	{
		ag_module_ipc_server_send_msg(WIFIDOG_MSG_AP_LOGOUT, AG_IPC_MSG_TYPE_SELF_CPU, "aglink", (u8 *)&stMsg, sizeof(WIFIOG_AP_LOGOUT_T), NULL);
		ag_module_ipc_server_send_msg(SNMP_MSG_AP_DEL, AG_IPC_MSG_TYPE_SELF_CPU, "aglink",  (u8 *)&stMsg, sizeof(WIFIOG_AP_LOGOUT_T), NULL);
	}
	else if(DEV_REGISTER_MSG == type)
	{
		ag_module_ipc_server_send_msg(WIFIDOG_MSG_AP_REGISTER, AG_IPC_MSG_TYPE_SELF_CPU, "aglink", (u8 *)&stMsg, sizeof(WIFIOG_AP_REGISTER_T), NULL);		
		ag_module_ipc_server_send_msg(SNMP_MSG_AP_ADD, AG_IPC_MSG_TYPE_SELF_CPU, "aglink",  (u8 *)&stMsg, sizeof(WIFIOG_AP_REGISTER_T), NULL);
	}
	else
	{
		AGLINK_LOG(LOG_INFO, "Unknown type!");
	}
	
	
	return FALSE;
}
s32 aglink_gw_update_config_to_ap(void)
{
	return FALSE;
}
s32 aglink_wifidog_msg_parse(const u8 * msgbuf, s32 msglen, s32 * pos)
{
	return FALSE;
}

s32 aglink_sync_stamsg_to_wifidog(void)
{
	return FALSE;
}

u32 aglink_sync_apmsg_to_wifidog(void)
{
	return FALSE;
}

s32 aglink_gw_send_dev_register_ack_msg(u8 *mac)
{
	u8 buf[AG_BUFFER_SIZE] = {0};
	u8 *p = &buf[0];
	u8 **pnt = &p;
	AGLINK_DEV_INFO  *dev_info = gpstaglinkmgt->pstdev_info;
	u32 uiLength = 0;

	gpstaglinkmgt->dev_reg_ack_ok = FALSE;

	/*mac addr 6 bytes*/
	ENCODE_PUT(pnt, dev_info->phy_mac, MAC_LEN);
	/*LAN ip*/
	ENCODE_PUT(pnt, dev_info->lan_ip, MAC_LEN);

	uiLength = p - &buf[0];

	AGLINK_LOG(LOG_INFO, "send dev register ack msg. dmac "MAC_FMT, MAC_ARG(mac));

	return aglink_send_ag_msg(AGLINK_MSG_DEV_REGISTER_ACK, AG_IPC_MSG_TYPE_OTHER_CPU, &buf[0], uiLength, mac);
}

s32 aglink_ap_recv_dev_register_ack_msg(u8 **pnt)
{
	AGLINK_LOG(LOG_INFO, "recv dev register ack msg.");
	if(IS_DEV_AP()){
		char wan_mac[32];
		/*mac addr 6 bytes*/
		DECODE_GET(gpstaglinkmgt->mac, pnt, MAC_LEN);
		AGLINK_LOG(LOG_INFO, "Get server WAN MAC addr "MAC_FMT, MAC_ARG(gpstaglinkmgt->mac));
		sprintf(wan_mac, "%02x-%02x-%02x-%02x-%02x-%02x", 
			gpstaglinkmgt->mac[0], gpstaglinkmgt->mac[1],
			gpstaglinkmgt->mac[2], gpstaglinkmgt->mac[3],
			gpstaglinkmgt->mac[4], gpstaglinkmgt->mac[5]);
		AGLINK_CONFIG_SET(AG_GW_WAN_MAC, wan_mac);

		if(!gpstaglinkmgt->dev_reg_ack_ok){
			gpstaglinkmgt->dev_reg_ack_ok = TRUE;
			event_add(&gpstaglinkmgt->keepalive_timer_ev, &gpstaglinkmgt->keepalive_tv);
		}
		ag_module_ipc_server_send_msg(BC_MSG_AP_REGISTER_TO_GW_SUCCESS, AG_IPC_MSG_TYPE_SELF_CPU, "aglink", NULL, 0, NULL);

        #ifndef GW_X86
		aglink_ap_send_ap_info_msg();
		#endif
		
		#if 0
		aglink_ap_sync_sta_connect_msg();

		/*AP在收到网关注册ACK后再初始化终端探测功能*/
		if(ag->sta_dct_enable && !ag->sta_dct_init){
			aglink_stadct_init(ag->dev_info->dev_type);
			ag->sta_dct_init = TRUE;
		}
		#endif
	}
	
	return 0;
}

s32 aglink_gw_recv_dev_register_msg(u8 **pnt, struct sockaddr_in *sk_addr)
{
	AGLINK_LOG(LOG_INFO, "recv dev register msg.");
	if(IS_DEV_GW())
	{
		struct hlist_node *pos;
		struct hlist_head *head;
		ap_node *ap, *new_ap;
		AP_WIFI_INFO stApInfo;
		s32 ap_exist = 0, iLoop = 0, iInLoop = 0;
		u32 ap_update = FALSE;

		memset(&stApInfo, 0, sizeof(AP_WIFI_INFO));
		
		if((new_ap = MEM_CALLOC(sizeof(ap_node))) == NULL){
			AGLINK_LOG(LOG_ERR, "alloc ap node failed !");
			return FALSE;
		}
		
		/*mac addr 6 bytes*/
		DECODE_GET(new_ap->mac, pnt, MAC_LEN);
		/*ip addr 16 bytes*/
		DECODE_GET(new_ap->ip, pnt, IP_LEN);
		/*model id 4 bytes*/
		DECODE_GETL(pnt, new_ap->model_id);
		/*dev name 64 bytes*/
		DECODE_GET(new_ap->dev_name, pnt, 64);
		/* wifi info */
		DECODE_GET(&stApInfo, pnt, sizeof(AP_WIFI_INFO));

		for(iLoop = 0; iLoop < AG_MODULE_AP_MAX_RADIO_NUM; iLoop++)
		{
			stApInfo.astRadio[iLoop].uiRadioType = ntohl(stApInfo.astRadio[iLoop].uiRadioType);
			stApInfo.astRadio[iLoop].uiVapNum = ntohl(stApInfo.astRadio[iLoop].uiVapNum);
		}
		stApInfo.uiRadioNum = ntohl(stApInfo.uiRadioNum);

		for(iLoop = 0; iLoop < AG_MODULE_AP_MAX_RADIO_NUM; iLoop++)
		{
			AGLINK_LOG(LOG_INFO, "WifiId:%d , vapNum:%d",stApInfo.astRadio[iLoop].uiRadioType, stApInfo.astRadio[iLoop].uiVapNum);
			for(iInLoop = 0; iInLoop < stApInfo.astRadio[iLoop].uiVapNum; iInLoop++)
			{
				AGLINK_LOG(LOG_INFO, "VapName:%s acVapName:%s acVapName:"MAC_FMT, 
					stApInfo.astRadio[iLoop].astVap[iInLoop].acVapName, stApInfo.astRadio[iLoop].astVap[iInLoop].acSsid, MAC_ARG(stApInfo.astRadio[iLoop].astVap[iInLoop].aucBssid));
			}
		}
		
		AGLINK_LOG(LOG_INFO, "register dev: "MAC_FMT" "IP_FMT" model_id %d name %s",
			MAC_ARG(new_ap->mac), IP_ARG(new_ap->ip), new_ap->model_id, new_ap->dev_name);

		memcpy(&new_ap->sk_addr, sk_addr, sizeof(struct sockaddr_in));

		gettimeofday(&new_ap->time, NULL); 
		//memcpy(&new_ap->stWifiInfo, &stApInfo, sizeof(AP_WIFI_INFO));

		/*将AP加入到AP链中*/
		AP_HLIST_LOCK();
		head = aglink_mac_hashfn(new_ap->mac, gpstaglinkmgt->ap_list, AP_HASH_SIZE);
		
		hlist_for_each(pos, head)
		{
			ap = hlist_entry(pos, ap_node, id_hlist);
			if (!memcmp(ap->mac, new_ap->mac, MAC_LEN)){
				ap_exist = 1;
				break;
			}
		}
		if(!ap_exist){
			AGLINK_LOG(LOG_INFO, "add AP "MAC_FMT, MAC_ARG(new_ap->mac));
			hlist_add_head(&(new_ap->id_hlist), head);
			aglink_send_apmsg_to_wifidog(new_ap->mac, new_ap->ip, 
				new_ap->model_id, new_ap->dev_name, &stApInfo, DEV_REGISTER_MSG);
		}
		else{
			AGLINK_LOG(LOG_INFO, "update AP "MAC_FMT, MAC_ARG(new_ap->mac));
			memcpy(&ap->time, &new_ap->time, sizeof(struct timeval));
			memcpy(&ap->sk_addr, &new_ap->sk_addr, sizeof(struct sockaddr_in));
			
			if(memcmp(ap->ip, new_ap->ip, IP_LEN)){
				memcpy(ap->ip, new_ap->ip, IP_LEN);
				ap_update = TRUE;
			}
			if(ap->model_id != new_ap->model_id){
				ap->model_id = new_ap->model_id;
				ap_update = TRUE;
			}
			if(strncmp(ap->dev_name, new_ap->dev_name, 64)){
				strncpy(ap->dev_name, new_ap->dev_name, 64);
				ap_update = TRUE;
			}
			#if 0
			if(strncmp(&ap->stWifiInfo, &new_ap->stWifiInfo, sizeof(AP_WIFI_INFO)))
			{
				strncpy(&ap->stWifiInfo, &new_ap->stWifiInfo, sizeof(AP_WIFI_INFO));
				ap_update = TRUE;
			}
			#endif

			//if(ap_update){
				aglink_send_apmsg_to_wifidog(ap->mac, ap->ip, 
					ap->model_id, ap->dev_name, &stApInfo, DEV_REGISTER_MSG);
			//}
			MEM_FREE(new_ap);
		}
		AP_HLIST_UNLOCK();

		aglink_gw_send_dev_register_ack_msg(ap_exist ? ap->mac : new_ap->mac);

		aglink_gw_update_config_to_ap();
	}

	return 0;
}



/* 
 * ===  FUNCTION  =========================================
 *  Name:  aglink_ap_send_dev_register_msg
 *  Description: AP向网关发送AP注册消息
 *  Input : none
 *  Output: none
 *  Return: 0: success <0: fail
 * ====================================================
 */
#ifndef GW_X86
u32 aglink_ap_send_dev_register_msg(void)
{
	static s32 siCount = 0;
	u8 buf[AG_BUFFER_SIZE] = {0};
	u8 *p = &buf[0];
	u8 **pnt = &p;
	u32 uiLength = 0;
	AP_WIFI_INFO stApInfo;

	memset(&stApInfo, 0, sizeof(AP_WIFI_INFO));
	
	/* 等待其他模块注册成功，头两次调用不发送注册消息*/
	if(siCount < 3)
	{
		AGLINK_LOG(LOG_INFO, " wait other module register, return ;");
		siCount++;
		return TRUE;
	}
	
	AGLINK_DEV_INFO  *dev_info = gpstaglinkmgt->pstdev_info;

	gpstaglinkmgt->dev_reg_ack_ok = FALSE;
	

	/*mac addr 6 bytes*/
	ENCODE_PUT(pnt, dev_info->phy_mac, MAC_LEN);

	/*ip addr 16 bytes*/
	ENCODE_PUT(pnt, dev_info->ip, IP_LEN);

	/*model id 4 bytes*/
	ENCODE_PUTL(pnt, dev_info->model_id);

	/*dev name 64 bytes*/
	ENCODE_PUT(pnt, dev_info->dev_name, 64);

	aglink_get_ap_wifi_info_entry(&stApInfo);

	
	AGLINK_LOG(LOG_ERR, "stApinfo address:%x", &stApInfo);

	s32 iLoop;
	stApInfo.uiRadioNum = 2;
	stApInfo.uiRadioNum = htonl(stApInfo.uiRadioNum);
	for(iLoop = 0; iLoop < AG_MODULE_AP_MAX_RADIO_NUM; iLoop++)
	{
		stApInfo.astRadio[iLoop].uiRadioType = htonl(stApInfo.astRadio[iLoop].uiRadioType);
		stApInfo.astRadio[iLoop].uiVapNum = htonl(stApInfo.astRadio[iLoop].uiVapNum);
	}

	ENCODE_PUT(pnt, &stApInfo, sizeof(AP_WIFI_INFO));

	uiLength = p - &buf[0];

	AGLINK_LOG(LOG_INFO, "send dev register msg to server :"IP_FMT" mac("MAC_FMT"), ip("IP_FMT"), model_id(%d), name(%s) uiLength:%lu  ", 
		IP_ARG(&gpstaglinkmgt->ipaddr), MAC_ARG(dev_info->phy_mac), IP_ARG(dev_info->ip), dev_info->model_id, dev_info->dev_name, uiLength);

	return aglink_send_ag_msg(AGLINK_MSG_DEV_REGISTER_MSG, AG_IPC_MSG_TYPE_OTHER_CPU, &buf[0], uiLength, NULL);
}

s32 aglink_ap_send_dev_keepalive_msg(void)
{
	u8 buf[AG_BUFFER_SIZE] = {0};
	u8 *p = &buf[0];
	u8 **pnt = &p;
	u32 uiLength = 0;
	AGLINK_DISK_INFO stDiskInfo = {0};
	AGLINK_CPU_RATE stCPUinfo = {0};
	AGLINK_WAN_STAT stWANstat = {0};

	AGLINK_DEV_INFO  *dev_info = gpstaglinkmgt->pstdev_info;

	aglink_get_free_info(&stDiskInfo);
	aglink_get_cpu_info(&stCPUinfo);
	aglink_get_wan_stat(PHY_IF, &stWANstat);
	/*mac addr 6 bytes*/
	ENCODE_PUT(pnt, dev_info->phy_mac, MAC_LEN);

	/*disk info*/
	ENCODE_PUTL(pnt, stDiskInfo.total);
	ENCODE_PUTL(pnt, stDiskInfo.used);

	/*cpu rate*/
	ENCODE_PUTL(pnt, stCPUinfo.cpu_rate);
	ENCODE_PUTL(pnt, stCPUinfo.system);
	ENCODE_PUTL(pnt, stCPUinfo.user);
	ENCODE_PUTL(pnt, stCPUinfo.nice);
	ENCODE_PUTL(pnt, stCPUinfo.idle);

	/*WAN stat*/
	//ENCODE_PUT(pnt, &stWANstat.RecBytes, sizeof(stWANstat.RecBytes));
	//ENCODE_PUT(pnt, &stWANstat.SendBytes, sizeof(stWANstat.SendBytes));
	ENCODE_PUTL(pnt, stWANstat.RecBytes);
	ENCODE_PUTL(pnt, stWANstat.SendBytes);
	
	
	uiLength = p - &buf[0];

	AGLINK_LOG(LOG_DEBUG, "send dev keepalive msg: ap-mac("MAC_FMT"), keepalive repeat %d\n", 
		MAC_ARG(dev_info->phy_mac), gpstaglinkmgt->keepalive_times);

	++gpstaglinkmgt->keepalive_times;

	return aglink_send_ag_msg(AGLINK_MSG_DEV_KEEPLIVE_MSG, AG_IPC_MSG_TYPE_OTHER_CPU, &buf[0], uiLength, NULL);
}
#endif
s32 aglink_gw_recv_dev_keepalive_msg(u8 **pnt, struct sockaddr_in *sk_addr)
{
	AGLINK_DISK_INFO stDiskInfo = {0};
	AGLINK_CPU_RATE stCPUinfo = {0};
	AGLINK_WAN_STAT stWANstat = {0};

	s32 ret = 0;
	AGLINK_LOG(LOG_DEBUG, "recv dev keepalive msg.");
	
	if(IS_DEV_GW())
	{
		struct hlist_node *pos;
		struct hlist_head *head;
		ap_node *ap;
		u8 mac[MAC_LEN];
		s32 ap_exist = 0;
		
		/*mac addr 6 bytes*/
		DECODE_GET(mac, pnt, MAC_LEN);

		/*disk info*/
		DECODE_GETL(pnt, stDiskInfo.total);
		DECODE_GETL(pnt, stDiskInfo.used);
		AGLINK_LOG(LOG_INFO, "total:%d used:%d", stDiskInfo.total, stDiskInfo.used);

		/*cpu rate*/
		DECODE_GETL(pnt, stCPUinfo.cpu_rate);
		DECODE_GETL(pnt, stCPUinfo.system);
		DECODE_GETL(pnt, stCPUinfo.user);
		DECODE_GETL(pnt, stCPUinfo.nice);
		DECODE_GETL(pnt, stCPUinfo.idle);
		AGLINK_LOG(LOG_INFO, "CPU: %d (system=%d user=%d nice=%d idle=%d)", 
				stCPUinfo.cpu_rate, stCPUinfo.system, stCPUinfo.user, stCPUinfo.nice, stCPUinfo.idle);

		/*WAN stat*/
		//DECODE_GET(&stWANstat.RecBytes, pnt, sizeof(stWANstat.RecBytes));
		//DECODE_GET(&stWANstat.SendBytes, pnt, sizeof(stWANstat.SendBytes));
		DECODE_GETL(pnt, stWANstat.RecBytes);
		DECODE_GETL(pnt, stWANstat.SendBytes);
		AGLINK_LOG(LOG_INFO, "WAN stat: RecvBytes:%lu SendBytes:%lu", stWANstat.RecBytes, stWANstat.SendBytes);

	
		/*更新AP在线时间戳*/
		AP_HLIST_LOCK();
		head = aglink_mac_hashfn(mac, gpstaglinkmgt->ap_list, AP_HASH_SIZE);
		hlist_for_each(pos, head)
		{
			ap = hlist_entry(pos, ap_node, id_hlist);
			if (!memcmp(ap->mac, mac, MAC_LEN)){
				ap_exist = 1;
				break;
			}
		}
		
		if(!ap_exist){
			ap_node *new_ap;
			AGLINK_LOG(LOG_INFO, "AP "MAC_FMT" doesn't exist, send nak", MAC_ARG(mac));

			/*创建临时表项*/
			if((new_ap = MEM_CALLOC(sizeof(ap_node))) == NULL){
				AGLINK_LOG(LOG_ERR, "alloc ap node failed !");
				AP_HLIST_UNLOCK();
				return -1;
			}
			memcpy(new_ap->mac, mac, MAC_LEN);
			memcpy(&new_ap->sk_addr, sk_addr, sizeof(struct sockaddr_in));
			head = aglink_mac_hashfn(new_ap->mac, gpstaglinkmgt->ap_list, AP_HASH_SIZE);
			hlist_add_head(&(new_ap->id_hlist), head);
		}
		else{
			/*更新AP在线时间点*/
			gettimeofday(&ap->time, NULL); 
		}
		AP_HLIST_UNLOCK();

		if(!ap_exist)
			ret = aglink_gw_send_dev_nak_msg(mac, sk_addr);
		else
			ret = aglink_gw_send_dev_keepalive_ack_msg(mac);
	}

	return ret;
}

s32 aglink_gw_send_dev_keepalive_ack_msg(u8 *mac)
{
	u8 buf[AG_BUFFER_SIZE] = {0};
	u8 *p = &buf[0];
	u8 **pnt = &p;
	u32 uiLength = 0;
	AGLINK_DEV_INFO  *dev_info = gpstaglinkmgt->pstdev_info;

	/*mac addr 6 bytes*/
	ENCODE_PUT(pnt, dev_info->phy_mac, MAC_LEN);
	
	uiLength = p - &buf[0];
	AGLINK_LOG(LOG_INFO, "send dev keepalive ack msg. dmac "MAC_FMT, MAC_ARG(mac));
	
	return aglink_send_ag_msg(AGLINK_MSG_DEV_KEEPLIVE_ACK_MSG, AG_IPC_MSG_TYPE_OTHER_CPU, &buf[0], uiLength, mac);
}

/* 
 * ===  FUNCTION  =========================================
 *  Name:  aglink_ap_recv_dev_keepalive_ack_msg
 *  Description: AP接收设备保活ACK消息
 *  Input : none
 *  Output: none
 *  Return: 0: success <0: fail
 * ====================================================
 */
s32 aglink_ap_recv_dev_keepalive_ack_msg(u8 **pnt)
{
	u8 ucgwmac[MAC_LEN] = {0};
	AGLINK_LOG(LOG_DEBUG, "recv dev keepalive ack msg.");
	
	if(IS_DEV_AP()){
		DECODE_GET(ucgwmac, pnt, MAC_LEN);
		if(0 != memcmp(ucgwmac, gpstaglinkmgt->mac, MAC_LEN))
		{
			AGLINK_LOG(LOG_INFO, "gpstaglinkmgt->mac:"MAC_FMT" gwmac:"MAC_FMT"", MAC_ARG(gpstaglinkmgt->mac), MAC_ARG(ucgwmac));
			return 0;
		}
		gpstaglinkmgt->keepalive_times = 0;
	}

	return TRUE;
}

s32 aglink_gw_send_dev_nak_msg(u8 *mac, struct sockaddr_in *sk_addr)
{
	struct ag_msg_ctrlhdr hdr;
	u8 buf[AG_BUFFER_SIZE] = {0};
	u8 *p = &buf[0];
	u8 **pnt = &p;
	u32 uiLength = 0;
	AGLINK_DEV_INFO  *dev_info = gpstaglinkmgt->pstdev_info;

	/*mac addr 6 bytes*/
	ENCODE_PUT(pnt, dev_info->phy_mac, MAC_LEN);

	uiLength = p - &buf[0];

	AGLINK_LOG(LOG_INFO, "send dev nak msg");

	return aglink_send_ag_msg(AGLINK_MSG_DEV_NAK_MSG, AG_IPC_MSG_TYPE_OTHER_CPU, &buf[0], uiLength, mac);
}

s32 aglink_ap_recv_dev_nak_msg(u8 **pnt)
{
	u8 ucgwmac[MAC_LEN] = {0};
	AGLINK_LOG(LOG_DEBUG, "recv dev nak msg.");
	
	if(IS_DEV_AP()){

		DECODE_GET(ucgwmac, pnt, MAC_LEN);
		if(0 != memcmp(ucgwmac, gpstaglinkmgt->mac, MAC_LEN))
		{
			AGLINK_LOG(LOG_DEBUG, "gpstaglinkmgt->mac:"MAC_FMT" gwmac:"MAC_FMT, MAC_ARG(gpstaglinkmgt->mac), MAC_ARG(ucgwmac));
			return TRUE;
		}
		
		event_del(&gpstaglinkmgt->keepalive_timer_ev);
		gpstaglinkmgt->keepalive_times = 0;
#ifndef GW_X86

		/*重新向网关注册*/
		aglink_ap_send_dev_register_msg();
#endif
	}

	return 0;
}
#ifndef GW_X86
s32 aglink_ap_send_ap_info_msg(void)
{
	u8 buf[2048] = {0};
	u8 *p = &buf[0];
	u8 **pnt = &p;
	struct ag_msg_ctrlhdr hdr;
	s32 iLoop = 0;
	
	s32 iRet;
	AGLINK_AP_DETAIL_INFO_T stApInfo;
	memset(&stApInfo, 0, sizeof(AGLINK_AP_DETAIL_INFO_T));

	AGLINK_LOG(LOG_INFO, "aglink_ap_send_ap_info_msg.");

	if(-1 == (iRet =aglink_get_ap_dev_info(&stApInfo)))
	{
		return -1;
	}
	if(-1 == (iRet =aglink_get_ap_sys_info(&stApInfo)))
	{
		return -1;
	}

	AGLINK_LOG(LOG_INFO, "%d--acfap_sn:%s acDeviceType:%s acLocation:%s, acVendor:%s  MAC:"MAC_FMT" iOnline:%d iCha2:%d iCha5:%d iSsidNum:%d", sizeof(AGLINK_AP_DETAIL_INFO_T),
		stApInfo.acfap_sn, stApInfo.acDeviceType, stApInfo.acLocation, stApInfo.acVendor,   MAC_ARG(stApInfo.acMac), stApInfo.iOnline, stApInfo.iCha2, stApInfo.iCha5, stApInfo.iSsidNum);

	for(iLoop = 0; iLoop < stApInfo.iSsidNum; iLoop++)
	{
		AG_HTONL(stApInfo.ssid[iLoop].iUser_num);
	}

	AG_HTONL(stApInfo.iOnline);
	AG_HTONL(stApInfo.iCha2);
	AG_HTONL(stApInfo.iCha5);
	AG_HTONL(stApInfo.iRoam);
	AG_HTONL(stApInfo.cur_link);
	AG_HTONL(stApInfo.total_link);
	AG_HTONL(stApInfo.iRunStatus);
	AG_HTONL(stApInfo.total_up);
	AG_HTONL(stApInfo.total_down);
	AG_HTONL(stApInfo.runtimelong);
	AG_HTONL(stApInfo.hardversion);
	AG_HTONL(stApInfo.ap_work_mode);
	AG_HTONL(stApInfo.iSsidNum);
	
	/*mac addr 6 bytes*/
	ENCODE_PUT(pnt, &stApInfo, sizeof(AGLINK_AP_DETAIL_INFO_T));

	#if 0	
	hdr.msg_type = FIT_AP_INFO;
	hdr.msg_len = p - &buf[0];
	#endif
	u32 uiLength = 0;
	uiLength = p - &buf[0];

	aglink_send_ag_msg(SNMP_MSG_AP_INFO, AG_IPC_MSG_TYPE_OTHER_CPU, &buf[0], uiLength, NULL);

	aglink_send_ag_msg(AGLINK_MSG_AP_INFO, AG_IPC_MSG_TYPE_OTHER_CPU, &buf[0], uiLength, NULL); 	//(&hdr, buf, NULL);

	return TRUE;
	
}
#endif
s32 aglink_gw_recv_ap_info_msg(AG_LINK_MSG_HEAD_T hdr, u8 **buf)
{
	struct wifidog_msg_ctrlhdr  msghd;
	memset(&msghd, 0, sizeof(struct wifidog_msg_ctrlhdr));
	AGLINK_LOG(LOG_DEBUG, "msgtype:%d, msg len:%d",hdr.usMsgId, hdr.uiMsgLen);
	msghd.msg_type = hdr.usMsgId;
	msghd.msg_len = hdr.uiMsgLen;
#ifdef _BOXLL_	
	return aglink_send_boxll_msg(&msghd, *buf);
#else
	return 0;
#endif
}

#if 0	
s32 aglink_gw_recv_sta_connect_msg(u8 **Msgbuf, u32 usMsgId)
{
	
	u8 buf[64];
	u8 *p = &buf[0];
	u8 **pnt = &p;
	BOXLL_EVENT_STA stStaInfo;
	struct wifidog_msg_ctrlhdr  msghd;

	memset(&msghd, 0, sizeof(struct wifidog_msg_ctrlhdr));
	memset(&stStaInfo, 0, sizeof(BOXLL_EVENT_STA));
	memcpy(&stStaInfo, *Msgbuf, sizeof(BOXLL_EVENT_STA));

	AGLINK_LOG(LOG_INFO, "STA mac "MAC_FMT" vap name:%s  essid:%s", MAC_ARG(stStaInfo.aucStaMac), stStaInfo.acVapName, stStaInfo.acSsid);

	/*ap mac addr 6 bytes*/
	ENCODE_PUT(pnt, stStaInfo.aucApMac, MAC_LEN);
	/*sta mac addr 6 bytes*/
	ENCODE_PUT(pnt, stStaInfo.aucStaMac, MAC_LEN);
	ENCODE_PUT(pnt, stStaInfo.acSsid, SSID_LEN);
	
	if(AGLINK_IDSET_STA_ADD == usMsgId)
	{
		msghd.msg_type = WIFIDOG_STA_CNNT_MSG;
	}
	else if(AGLINK_IDSET_STA_DEL == usMsgId)
	{
		msghd.msg_type = WIFIDOG_STA_DISCNNT_MSG;
	}
	else
	{
		AGLINK_LOG(LOG_ERR, " Msg id error(%d)", usMsgId);
		return FALSE;
	}

	msghd.msg_len = p - &buf[0];
#ifdef _BOXLL_	
	return aglink_send_boxll_msg(&msghd, buf);
#else	
	return 0;
#endif
}
#endif
void aglink_config_check_timer_cb(s32 fd, short event, void *arg)
{	
	u8 config[65];
	u32 ag_server_ip;

	AGLINK_LOG(LOG_DEBUG, "config check");
	/*获取网关IP变更*/
	if(IS_DEV_AP()){
		if(AGLINK_CONFIG_GET(config, AG_SERVER_IP_CONFIG, 64) == 0)
		{
			if(inet_pton(AF_INET, config, &ag_server_ip) > 0){
				if(gpstaglinkmgt ->ipaddr != ag_server_ip){
					gpstaglinkmgt->ipaddr = ag_server_ip;

					event_del(&gpstaglinkmgt->keepalive_timer_ev);
					gpstaglinkmgt->keepalive_times = 0;
					
					event_del(&gpstaglinkmgt->rcv_ev);
					close(gpstaglinkmgt->sockfd);
					gpstaglinkmgt->sockfd = 0;
					sleep(1);
					
					/*初始化网关-AP通信socket*/
					if(aglink_ag_socket_init() < 0){
						AGLINK_LOG(LOG_ERR, "socket init failed!");
						exit(-1);
					}
#ifndef GW_X86
					/*重新向网关注册*/
					aglink_ap_send_dev_register_msg();
#endif
				}
			}
		}
	}
	
	/*设置DEBUG level*/
	if(AGLINK_CONFIG_GET(config, AG_DEBUG_CONFIG, 64) == 0)
	{
		if(!strcmp(config, "common"))
			gpstaglinkmgt->debug_level = DEBUG_LEVER_COMMON;
		else if(!strcmp(config, "verbose"))
			gpstaglinkmgt->debug_level = DEBUG_LEVER_VERBOSE;
		else
			gpstaglinkmgt->debug_level = DEBUG_LEVER_NONE;


		/*if(ag->debug_level != DEBUG_LEVER_NONE){
			AGLINK_LOG(LOG_DEBUG, "fd(%d)-line(%d)-size(%d)-ver(%s)", 
				ag->l_mgr->sockfd, ag->debug_line, ag->debug_size, AG_VER);
		}*/
	}

	event_add(&gpstaglinkmgt->config_check_timer_ev, &gpstaglinkmgt->config_check_tv);
}

/* 
 * ===  FUNCTION  =========================================
 *  Name:  aglink_ap_send_keepalive_timer_cb
 *  Description: AP定期向网关发送保活消息
 *  Input : fd: event相关fd
 *		   event: event相关事件
 *		   arg:相关参数
 *  Output: none
 *  Return: none
 * ====================================================
 */
void aglink_ap_send_keepalive_timer_cb(s32 fd, short event, void *arg)
{	
	if(gpstaglinkmgt->keepalive_times >= KEEPALIVE_MAX_TIMES){
		AGLINK_LOG(LOG_INFO, "can't receive keepalive ack for %u times\n", 
			gpstaglinkmgt->keepalive_times);

		/*重新向网关注册*/
		gpstaglinkmgt->keepalive_times = 0;
#ifndef GW_X86
		/*发送设备注册消息*/
		aglink_ap_send_dev_register_msg();
#endif
		return;
	}
#ifndef GW_X86
	aglink_ap_send_dev_keepalive_msg();
#endif
	event_add(&gpstaglinkmgt->keepalive_timer_ev, &gpstaglinkmgt->keepalive_tv);
}

void aglink_ap_send_ap_info_timer_cb(s32 fd, short event, void *arg)
{
#ifndef GW_X86
	aglink_ap_send_ap_info_msg();
	aglink_ap_send_wifi_info();
#endif
#ifdef _BOXLL_
	event_add(&gpstaglinkmgt->b_mgr->report_ap_info_ev, &gpstaglinkmgt->b_mgr->ap_info_tv);
#endif
	return ;
}

void aglink_ack_recv_check_cb(s32 fd, short event, void *arg)
{	
	s32 i;
	struct usr_cache_info *cache;
	struct hlist_node *p, *n;
	static s32 dev_reg_status = 0;

	AGLINK_LOG(LOG_DEBUG, "begin gpstaglinkmgt->dev_reg_ack_ok=%d", gpstaglinkmgt->dev_reg_ack_ok);
	
/*检测设备注册信息ACK*/
	if((!gpstaglinkmgt->dev_reg_ack_ok) && (IS_DEV_AP())){
		if(0 == dev_reg_status) /*尚未收到ACK*/
			dev_reg_status = 1; /*标志为1*/
		else{  /*仍未收到ACK*/
#ifndef GW_X86
			aglink_ap_send_dev_register_msg();  /*重新发送*/
#endif
			dev_reg_status = 0;
		}
	}
	else
		dev_reg_status = 0;

	event_add(&gpstaglinkmgt->ack_check_timer_ev, &gpstaglinkmgt->ack_check_tv);
}


void aglink_gw_age_ap_cb(s32 fd, short event, void *arg)
{
	int i = 0, age = 0;
	ap_node *ap = NULL;
	struct hlist_node *p, *n;
	struct timeval time;
	unsigned long t_start_sec, t_start_msec;
	unsigned long t_end_sec, t_end_msec;
	unsigned long t_cost_time_sec;

	gettimeofday(&time, NULL); 
	t_end_sec = (unsigned long)time.tv_sec;
	t_end_msec = (unsigned long)time.tv_usec/1000;

	int iOutTm = gpstaglinkmgt->keepalive_tv.tv_sec*KEEPALIVE_MAX_TIMES;

	AP_HLIST_LOCK();
	for(i = 0; i < AP_HASH_SIZE; ++i)
	{
		hlist_for_each_safe(p, n, &(gpstaglinkmgt->ap_list[i]))
		{
			ap = hlist_entry(p, ap_node, id_hlist);
			t_start_sec = (unsigned long)ap->time.tv_sec;
			t_start_msec = (unsigned long)ap->time.tv_usec/1000;

			if(t_end_sec < t_start_sec){
				age = 1;
				goto AGE_CLIENT;
			}

			t_cost_time_sec = t_end_sec - t_start_sec;
			if(t_end_msec < t_start_msec)
				--t_cost_time_sec;
			
			//if(t_cost_time_sec >= AP_AGE_TIME)
			if(t_cost_time_sec >= iOutTm)
				age = 1;
			else
				age = 0;
AGE_CLIENT:
			if(age){
				AGLINK_LOG(LOG_INFO, "age ap "MAC_FMT, MAC_ARG(ap->mac));
				aglink_send_apmsg_to_wifidog(ap->mac, ap->ip, 
					ap->model_id, ap->dev_name, NULL, DEV_LOGOUT_MSG);
				hlist_del(&(ap->id_hlist));
				MEM_FREE(ap);				
			}
		}
	}
	AP_HLIST_UNLOCK();

	event_add(&gpstaglinkmgt->ap_age_timer_ev, &gpstaglinkmgt->ap_age_tv);
}

s32 aglink_connect_wifidog(void)
{
	s32 sockfd, len;
	struct sockaddr_un servaddr;
	s8 *usock_path = UNIX_SOCK_PATH;
	s8 usock_req[32] = UNIX_SOCK_REQUEST;

	if( (sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){
		/*printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);*/
		return -1;
	}
	
	memset (&servaddr, 0, sizeof (struct sockaddr_un));
	servaddr.sun_family = AF_UNIX;
	strncpy (servaddr.sun_path, usock_path, strlen(usock_path));
	len = sizeof (servaddr.sun_family) + strlen (servaddr.sun_path);

	AGLINK_LOG(LOG_DEBUG, "try to connect wifidog.");
	
	if( connect(sockfd, (struct sockaddr *)&servaddr, len) == -1 ){
		/*printf("wifidog connect error: %s(errno: %d)\n", strerror(errno), errno);*/
		close(sockfd);
		return -1;
	}

	AGLINK_LOG(LOG_INFO, "connected to wifidog.");

	if( send(sockfd, usock_req, strlen(usock_req)+1, 0) < 0){
		AGLINK_LOG(LOG_ERR, "Send client login msg to aglink error!");
	}

	return sockfd;
}

void aglink_recv_from_wifidog_msg(evutil_socket_t fd, short event, void *arg)
{
	s32 size;
	s32 pos = 0;
	u8 *recvbuf = gpstaglinkmgt->w_mgt->buf;
	
	if((size = recv(fd, recvbuf, AG_BUFFER_SIZE, 0)) <= 0){
		AGLINK_LOG(LOG_ERR, "recv wifidog msg size %d, error: %s(errno: %d)", 
			size, strerror(errno), errno);
		event_del(&gpstaglinkmgt->w_mgt->rcv_ev);
		close(fd);
		gpstaglinkmgt->w_mgt->sockfd = 0;

		/*重新添加定时器连接wifidog*/
		event_add(&gpstaglinkmgt->w_mgt->cnnt_timer_ev, &gpstaglinkmgt->w_mgt->cnnt_tv); 
	}
	else{
		aglink_wifidog_msg_parse(recvbuf, size, &pos);
		
		do{
			if((size = recv(fd, (recvbuf+pos), (AG_BUFFER_SIZE-pos), MSG_DONTWAIT)) > 0){
				aglink_wifidog_msg_parse(recvbuf, size+pos, &pos);
			}
		} while(size > 0);
	}
	
	return;
}


void aglink_wifidog_connect_timer_cb(s32 fd, short event, void *arg)
{
	s32 sockfd;
	if((sockfd = aglink_connect_wifidog()) <= 0){
		/*重新添加定时器连接wifidog*/
		event_add(&gpstaglinkmgt->w_mgt->cnnt_timer_ev, &gpstaglinkmgt->w_mgt->cnnt_tv);
	}
	else{
		s32 flags = fcntl(sockfd, F_GETFL, 0);
		fcntl(sockfd, F_SETFL, flags | O_NONBLOCK); /*设置为非阻塞模式*/
		gpstaglinkmgt->w_mgt->sockfd = sockfd;
		event_set(&gpstaglinkmgt->w_mgt->rcv_ev, sockfd, EV_READ | EV_PERSIST, 
			aglink_recv_from_wifidog_msg, (void *)&gpstaglinkmgt->w_mgt->rcv_ev);
		event_add(&gpstaglinkmgt->w_mgt->rcv_ev, NULL);

		sleep(2);
		aglink_sync_apmsg_to_wifidog();
		aglink_sync_stamsg_to_wifidog();
	}
}

s32 aglink_wifidog_connect_init()
{
	s32 sockfd;
	AGLINK_LOG(LOG_ERR, "wifidog connect init.");
	
	evtimer_set(&gpstaglinkmgt->w_mgt->cnnt_timer_ev, aglink_wifidog_connect_timer_cb, NULL);

	if((sockfd = aglink_connect_wifidog()) <= 0){
		event_add(&gpstaglinkmgt->w_mgt->cnnt_timer_ev, &gpstaglinkmgt->w_mgt->cnnt_tv);
	}
	else{
		s32 flags = fcntl(sockfd, F_GETFL, 0);
		fcntl(sockfd, F_SETFL, flags | O_NONBLOCK); /*设置为非阻塞模式*/
		gpstaglinkmgt->w_mgt->sockfd = sockfd;
		event_set(&gpstaglinkmgt->w_mgt->rcv_ev, sockfd, EV_READ | EV_PERSIST, 
			aglink_recv_from_wifidog_msg, (void *)&gpstaglinkmgt->w_mgt->rcv_ev);
		
		event_add(&gpstaglinkmgt->w_mgt->rcv_ev, NULL);

		sleep(2);
		aglink_sync_apmsg_to_wifidog();
		aglink_sync_stamsg_to_wifidog();
	}

	return 0;
}

#ifdef _BOXLL_

s32 aglink_send_boxll_msg(struct wifidog_msg_ctrlhdr *msghdr, u8 *buf)
{
	u8 *sendbuf = gpstaglinkmgt->b_mgr->buf;
	u8 *p = sendbuf;
	u8 **pnt = &p;
	s32 ret = 0;
	u32 totallen = msghdr->msg_len + sizeof(struct wifidog_msg_ctrlhdr);

	AGLINK_LOG(LOG_DEBUG, "Send boxll msg (type %d)", msghdr->msg_type);
	if(gpstaglinkmgt->b_mgr->sockfd <= 0){
		AGLINK_LOG(LOG_DEBUG, "boxll fd not ready!!");
		return -1;
	}
	
	if(totallen > MAXLEN){
		AGLINK_LOG(LOG_DEBUG, "totallen=%d", totallen);
		return -1;
	}
	
	if(IS_DEV_AP()){
		AGLINK_LOG(LOG_DEBUG, "device is ap mode");
		return -1;
	}

	/*封装消息头部*/
	ENCODE_PUTW(pnt, msghdr->msg_type);
	ENCODE_PUTW(pnt, msghdr->msg_len);

	/*封装消息体*/
	ENCODE_PUT(pnt, buf, msghdr->msg_len);
	
	/*发送消息*/
	ret = send(gpstaglinkmgt->b_mgr->sockfd, sendbuf, totallen, 0);
	if(ret < 0){
		AGLINK_LOG(LOG_ERR, "Send msg to boxll error! %s(errno: %d)\n", 
			strerror(errno),errno);
	} else {
		AGLINK_LOG(LOG_DEBUG, "Send msg to boxll succeed");
	}

	return ret;
}

s32 aglink_boxll_msg_parse(const u8 *msgbuf, s32 msglen, s32 *pos)
{
	u8 msgtype;
	s32 rst_size = msglen;
	u8 *pt = msgbuf;

	while(rst_size > 0)
	{
		msgtype = *pt;
	
		switch(msgtype)
		{
		default:
			break;
		}
	}

parse_end:
	if(rst_size > 0){
		memmove(gpstaglinkmgt->b_mgr->buf, pt, rst_size);
		*pos = rst_size;
	}

	return 0;
}


void aglink_recv_boxll_msg(evutil_socket_t fd, short event, void *arg)
{
	s32 size;
	s32 pos = 0;
	u8 *recvbuf = gpstaglinkmgt->b_mgr->buf;
	
	if((size = recv(fd, recvbuf, MAXLEN, 0)) <= 0){
		AGLINK_LOG(LOG_ERR, "recv boxll msg size %d, error: %s(errno: %d)", 
			size, strerror(errno), errno);
		event_del(&gpstaglinkmgt->b_mgr->rcv_ev);
		close(fd);
		gpstaglinkmgt->b_mgr->sockfd = 0;

		/*重新添加定时器连接boxll*/
		event_add(&gpstaglinkmgt->b_mgr->cnnt_timer_ev, &gpstaglinkmgt->b_mgr->cnnt_tv); 
	}
	else{
		aglink_boxll_msg_parse(recvbuf, size, &pos);
		
		do{
			if((size = recv(fd, (recvbuf+pos), (MAXLEN-pos), MSG_DONTWAIT)) > 0){
				aglink_boxll_msg_parse(recvbuf, size+pos, &pos);
			}
		} while(size > 0);
	}
	
	return;
}

s32 aglink_connect_to_boxll(void)
{
	s32 sockfd, len;
	struct sockaddr_un servaddr;
	s8 *usock_path = UNIX_SOCK_PATH_BOXLL;
	s8 usock_req[32] = UNIX_SOCK_REQUEST;

	if( (sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){
		/*printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);*/
		return -1;
	}
	
	memset (&servaddr, 0, sizeof (struct sockaddr_un));
	servaddr.sun_family = AF_UNIX;
	strncpy (servaddr.sun_path, usock_path, strlen(usock_path));
	len = sizeof (servaddr.sun_family) + strlen (servaddr.sun_path);

	AGLINK_LOG(LOG_DEBUG, "try to connect boxll.");
	
	if( connect(sockfd, (struct sockaddr *)&servaddr, len) == -1 ){
		/*printf("wifidog connect error: %s(errno: %d)\n", strerror(errno), errno);*/
		close(sockfd);
		return -1;
	}

	AGLINK_LOG(LOG_INFO, "connected to boxll.");

	if( send(sockfd, usock_req, strlen(usock_req)+1, 0) < 0){
		AGLINK_LOG(LOG_ERR, "Send client login msg to boxll error!");
	}

	return sockfd;
}

void aglink_boxll_cnnt_timer_cb(s32 fd, short event, void *arg)
{
	s32 sockfd;
	if((sockfd = aglink_connect_to_boxll()) <= 0){
		/*重新添加定时器连接wifidog*/
		event_add(&gpstaglinkmgt->b_mgr->cnnt_timer_ev, &gpstaglinkmgt->b_mgr->cnnt_tv);
	}
	else{
		s32 flags = fcntl(sockfd, F_GETFL, 0);
		fcntl(sockfd, F_SETFL, flags | O_NONBLOCK); /*设置为非阻塞模式*/
		gpstaglinkmgt->b_mgr->sockfd = sockfd;
		event_set(&gpstaglinkmgt->b_mgr->rcv_ev, sockfd, EV_READ | EV_PERSIST, 
			aglink_recv_boxll_msg, (void *)&gpstaglinkmgt->b_mgr->rcv_ev);
		event_add(&gpstaglinkmgt->b_mgr->rcv_ev, NULL);

		sleep(2);
	}
}

s32 aglink_boxll_cnnt_init()
{
	s32 sockfd;
	AGLINK_LOG(LOG_ERR, "boxll connect init.");
	
	evtimer_set(&gpstaglinkmgt->b_mgr->cnnt_timer_ev, aglink_boxll_cnnt_timer_cb, NULL);

	if((sockfd = aglink_connect_to_boxll()) <= 0){
		event_add(&gpstaglinkmgt->b_mgr->cnnt_timer_ev, &gpstaglinkmgt->b_mgr->cnnt_tv);
	}
	else{
		s32 flags = fcntl(sockfd, F_GETFL, 0);
		fcntl(sockfd, F_SETFL, flags | O_NONBLOCK); /*设置为非阻塞模式*/
		gpstaglinkmgt->b_mgr->sockfd = sockfd;
		event_set(&gpstaglinkmgt->b_mgr->rcv_ev, sockfd, EV_READ | EV_PERSIST, 
			aglink_recv_boxll_msg, (void *)&gpstaglinkmgt->b_mgr->rcv_ev);
		
		event_add(&gpstaglinkmgt->b_mgr->rcv_ev, NULL);

		sleep(2);
	}

	return 0;
}

#endif
s32 aglink_socket_info_init()
{
	s8 ipmd[NVRAM_LEN], ipgw[NVRAM_LEN];
	s8 server_ip[NVRAM_LEN*2], serv_port[NVRAM_LEN];
	u32 ag_server_ip;

	/*循环直至获取到正确的gateway server ip*/
	if(IS_DEV_AP()){
		while(1){
			if(AGLINK_CONFIG_GET(ipmd, AG_IP_GET_METHOD, NVRAM_LEN) == 0){
				if(!strcmp(ipmd, "0")){ /*静态IP地址，则默认网关地址*/
					if(AGLINK_CONFIG_GET(ipgw, AG_IP_GATEWAY, NVRAM_LEN) == 0)
						AGLINK_CONFIG_WRITE(AG_SERVER_IP_CONFIG, ipgw);
				}
			}
			
			while(AGLINK_CONFIG_GET(server_ip, AG_SERVER_IP_CONFIG, (NVRAM_LEN*2)) != 0){
				/*AGLINK_LOG(LOG_ERR, "Get server ip error");*/
				sleep(2);
			}
			if(inet_pton(AF_INET, server_ip, &ag_server_ip) <= 0){
				AGLINK_LOG(LOG_ERR, "inet_pton error for %s\n", server_ip);
				AGLINK_CONFIG_ERASE(AG_SERVER_IP_CONFIG);
				sleep(2);
			}
			else
				break;
		}
	}
	else{
		while(1){
			while(AGLINK_CONFIG_GET(server_ip, AG_LAN_IP_CONFIG, (NVRAM_LEN*2)) != 0){
				AGLINK_LOG(LOG_ERR, "Get gateway ip error");
				sleep(2);
			}
			//if(inet_pton(AF_INET, server_ip, &ag_server_ip) <= 0){
			if(inet_aton(server_ip, &ag_server_ip) <= 0){
				AGLINK_LOG(LOG_ERR, "inet_pton error for %s\n", server_ip);
				sleep(2);
			}
			else
				break;
		}
	}
	gpstaglinkmgt->ipaddr = ag_server_ip;

	if(AGLINK_CONFIG_GET(serv_port, AG_SERVER_PORT_CONFIG, NVRAM_LEN) != 0){
		strncpy(serv_port, DEFAULT_SERVER_PORT, NVRAM_LEN);
		AGLINK_CONFIG_SET(AG_SERVER_PORT_CONFIG, DEFAULT_SERVER_PORT);
	}
	gpstaglinkmgt->port = atoi(serv_port);

	/* internel socket init */
	memset(server_ip, 0, sizeof(server_ip));
	memset(serv_port, 0, sizeof(serv_port));

	while(1)
	{
		if(AGLINK_CONFIG_GET(server_ip, AG_IP_INTERNEL, NVRAM_LEN) != 0)
		{
			AGLINK_LOG(LOG_ERR, "Internel communication using default IP:"DEFAULT_IP_INTERNEL);
			strncpy(server_ip, DEFAULT_IP_INTERNEL, NVRAM_LEN);
		}
		if(inet_pton(AF_INET, server_ip, &ag_server_ip) <= 0)
		{
			AGLINK_LOG(LOG_ERR, "inet_pton error for %s\n", server_ip);
			sleep(2);
		}
		else
		{
			break;
		}
	}
	if(AGLINK_CONFIG_GET(serv_port, AG_PORT_INTERNEL, NVRAM_LEN) != 0)
	{
		AGLINK_LOG(LOG_ERR, "Using default port:"DEFAULT_PORT_INTERNEL);
		strncpy(serv_port, DEFAULT_PORT_INTERNEL, NVRAM_LEN);
	}
	gpstaglinkmgt->iaddrinternal = ag_server_ip;
	gpstaglinkmgt->usportinternal = atoi(serv_port);
	
	AGLINK_LOG(LOG_INFO, "Get aglink config. Server: "IP_FMT":%d,  Internel server:"IP_FMT":%d\n", 
		IP_ARG(&gpstaglinkmgt->ipaddr), gpstaglinkmgt->port,  IP_ARG(&gpstaglinkmgt->iaddrinternal), gpstaglinkmgt->usportinternal);

	aglink_build_socket();
	
	return TRUE;
}

#ifndef GW_X86	
s32 ag_module_Agconfig_init(AGLINK_AP_CONFIG_INFO_T *pstAgConfig)
{
	#define NVRAM_LEN_DOUBLE  NVRAM_LEN*2
	#define NVRAM_LEN_QUADRUPLE	NVRAM_LEN*4
	
	
	FILE *pFile = NULL;
	u8	strCmd[256] = {0};
	u8 nvram_value_DOU[NVRAM_LEN_DOUBLE] = {0};
	u8 nvram_value_qua[NVRAM_LEN_QUADRUPLE] = {0};
	if(NULL == pstAgConfig)
	{
		AGLINK_LOG(LOG_ALERT, "Param is NULL,return");
		return FALSE;
	}

	memcpy(pstAgConfig->acMac, gpstaglinkmgt->pstdev_info->phy_mac, MAC_LEN);

	if(0 == AGLINK_CONFIG_GET(nvram_value_qua, AG_CONFIG_MEMO, NVRAM_LEN_QUADRUPLE))
	{
		memcpy(pstAgConfig->acMemo, nvram_value_qua, NVRAM_LEN_QUADRUPLE);
	}
	
	#ifdef OPENWRT
	/*sysinfo*/
	/*location*/
	if(0 == AGLINK_CONFIG_GET(nvram_value_DOU, AG_CONFIG_LOCATION, NVRAM_LEN_DOUBLE))
	{	
		memcpy(pstAgConfig->acLocation, nvram_value_DOU, NVRAM_LEN_DOUBLE);
	}
	/*location default value*/
	else
	{
		AGLINK_LOG(LOG_ALERT, "use defautl location value");
		memcpy(pstAgConfig->acLocation, AG_CONFIG_LOCATION_DEFAULT, strlen(AG_CONFIG_LOCATION_DEFAULT));
	}

	if(0 == AGLINK_CONFIG_GET(nvram_value_DOU, AG_CONFIG_CHANNEL_5, NVRAM_LEN_DOUBLE))
	{
		pstAgConfig->iCha2 = atoi(nvram_value_DOU);
	}
	else
	{
		pstAgConfig->iCha2= 1;
	}
	
	
	/*total_link*/
	if(0 == AGLINK_CONFIG_GET(nvram_value_DOU, AG_CONFIG_TOTAL_LINK, NVRAM_LEN_DOUBLE))
	{
		pstAgConfig->total_link = atoi(nvram_value_DOU);
	}
	/*total default value*/
	else
	{	
		AGLINK_LOG(LOG_ALERT, "use defautl total value");
		pstAgConfig->total_link = 128;
	}

	/*ssid info*/
	aglink_get_ssid_info(pstAgConfig->ssid, &pstAgConfig->iCha2, &pstAgConfig->iSsidNum);
	#else
	/*fat ap*/
	if(AGLINK_FAT_AP == gpstaglinkmgt->pstdev_info->ap_work_mode)
	{
		/*location*/
		if(0 == AGLINK_CONFIG_GET(nvram_value_DOU, AG_CONFIG_LOCATION, NVRAM_LEN_DOUBLE))
		{	
			memcpy(pstAgConfig->acLocation, nvram_value_DOU, NVRAM_LEN_DOUBLE);
		}
		/*location default value*/
		else
		{
			AGLINK_LOG(LOG_ALERT, "use defautl location value");
			memcpy(pstAgConfig->acLocation, AG_CONFIG_LOCATION_DEFAULT, strlen(AG_CONFIG_LOCATION_DEFAULT));
		}

		if(0 == AGLINK_CONFIG_GET(nvram_value_DOU, AG_CONFIG_CHANNEL_5, NVRAM_LEN_DOUBLE))
		{
			pstAgConfig->iCha2 = atoi(nvram_value_DOU);
		}
		else
		{
			pstAgConfig->iCha2= 1;
		}
		
		
		/*total_link*/
		if(0 == AGLINK_CONFIG_GET(nvram_value_DOU, AG_CONFIG_TOTAL_LINK, NVRAM_LEN_DOUBLE))
		{
			pstAgConfig->total_link = atoi(nvram_value_DOU);
		}
		/*total default value*/
		else
		{	
			AGLINK_LOG(LOG_ALERT, "use defautl total value");
			pstAgConfig->total_link = 128;
		}
		aglink_get_ssid_info_fat(pstAgConfig->ssid, &pstAgConfig->iCha2, &pstAgConfig->iSsidNum);
	}
	/*fit ap*/
	else
	{
		T_configFileData pstConfigData = {0};
		memset(&pstConfigData, 0, sizeof(T_configFileData));
		
		if(0 != access(TEMP_CONFIG_FILE, R_OK))
		{
			sprintf( strCmd, "flash -t -r mtd2 %s %d", TEMP_CONFIG_FILE, sizeof( T_configFileData ) );
			system( strCmd );
		}
		pFile = fopen(TEMP_CONFIG_FILE, "r+b");
		if(NULL == pFile)
		{
			AGLINK_LOG(LOG_ERR, "Open "TEMP_CONFIG_FILE" fail!");
			return -1;
		}
	
		if ( 1 != fread(&pstConfigData, sizeof( T_configFileData ), 1, pFile))
		{
			AGLINK_LOG(LOG_ERR, "read "TEMP_CONFIG_FILE" fail\n");
			fclose(pFile);
			return -1;
		}
		
		aglink_get_ssid_info_fit_two(pstAgConfig, &pstConfigData);
		
	}
	#endif

	AGLINK_LOG(LOG_INFO, "acLocation:%s icha2:%d total_link:%d acmemo:%s", 
		pstAgConfig->acLocation, pstAgConfig->iCha2, pstAgConfig->total_link, pstAgConfig->acMemo);

	#undef	NVRAM_LEN_QUADRUPLE
	#undef 	NVRAM_LEN_DOUBLE
	return TRUE;
}
#endif

/*****************************************************************************
 Prototype    : ag_module_init_aglink
 Description  : aglink config init
 Input        : u8 *pagmodule  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/11/19
    Author       : lvjianlin
    Modification : Created function

*****************************************************************************/
s32 ag_module_init_aglink(u8 *pagmodule)
{
	s32 i;
	s8 forward_mode[NVRAM_LEN*2];
	
	AG_MODULE_SETUP *pstModuleSetup = NULL;	

	if(NULL == pagmodule)
	{
		AGLINK_LOG(LOG_ALERT, "pagmodule is NULL\n", pagmodule);
		return FALSE;
	}

	pstModuleSetup = (AG_MODULE_SETUP *)pagmodule;

	if((gpstaglinkmgt = MEM_CALLOC(sizeof(AGLINK_MGT)))==NULL)
	{
		AGLINK_LOG(LOG_ALERT, "malloc AGLINK_MGT failed!");
		return FALSE;
	}

	if((gpstaglinkmgt->pstdev_info = MEM_CALLOC(sizeof(AGLINK_DEV_INFO))) == NULL)
	{
		AGLINK_LOG(LOG_ALERT, "malloc AGLINK_DEV_INFO failed!");
		return FALSE;
	}

	if((gpstaglinkmgt->w_mgt = MEM_CALLOC(sizeof(struct wifidog_mgt ))) == NULL)
	{
		AGLINK_LOG(LOG_ALERT, "malloc struct wifidog_mgt failed");
		return FALSE;
	}
	#ifdef _BOXLL_
	if((gpstaglinkmgt->b_mgr = MEM_CALLOC(sizeof(struct boxll_mgt))) == NULL)
	{
		AGLINK_LOG(LOG_ALERT, "malloc struct  boxll_mgt failed");
	}
	#endif
	gpstaglinkmgt->pstdev_info->hotspot_name[0] = '\0';

	if(NULL == (gpstaglinkmgt->pAgconfig = MEM_CALLOC(sizeof(AGLINK_AP_CONFIG_INFO_T))))
	{
		AGLINK_LOG(LOG_ALERT, "malloc AGLINK_AP_CONFIG_INFO_T failed!");
	}
	AGLINK_LOG(LOG_ALERT, "");
	#ifndef OPENWRT
	/*获知当前设备角色: AP or 网关*/
	while(AGLINK_CONFIG_GET(forward_mode, NET_FORWARD_MODE, (NVRAM_LEN*2)) != 0)
					sleep(2);

	
	if(!strcmp(forward_mode, "2")){
		gpstaglinkmgt->pstdev_info->dev_type = DEV_TYPE_AP;
		AGLINK_LOG(LOG_INFO, "get aglink dev type AP.");
	}
	else{
		gpstaglinkmgt->pstdev_info->dev_type = DEV_TYPE_GW;
		AGLINK_LOG(LOG_INFO, "get aglink dev type Gateway.");
	}
	#else
	gpstaglinkmgt->pstdev_info->dev_type = DEV_TYPE_GW;
	#endif
	
	gpstaglinkmgt->w_mgt->cnnt_tv.tv_sec = KEEPALIVE_INTERVAL;
	gpstaglinkmgt->w_mgt->cnnt_tv.tv_usec = 0;
	#if _BOXLL_
	gpstaglinkmgt->b_mgr->cnnt_tv.tv_sec = WIFIDOG_CNNT_INTERVAL;
	gpstaglinkmgt->b_mgr->cnnt_tv.tv_usec = 0;
	gpstaglinkmgt->b_mgr->ap_info_tv.tv_sec = AGLINK_REPORT_AP_INFO;
	gpstaglinkmgt->b_mgr->ap_info_tv.tv_usec = 0;
	#endif
	gpstaglinkmgt->keepalive_tv.tv_sec = KEEPALIVE_INTERVAL;
	gpstaglinkmgt->keepalive_tv.tv_usec = 0;
	gpstaglinkmgt->ack_check_tv.tv_sec = ACK_CHECK_INTERVAL;
	gpstaglinkmgt->ack_check_tv.tv_usec = 0;
	gpstaglinkmgt->config_check_tv.tv_sec = CONFIG_CHECK_INTERVAL;
	gpstaglinkmgt->config_check_tv.tv_usec = 0;
	gpstaglinkmgt->ap_age_tv.tv_sec = AP_AGE_INTERVAL;
	gpstaglinkmgt->ap_age_tv.tv_usec = 0;
	gpstaglinkmgt->keepalive_times = 0;
	gpstaglinkmgt->dev_reg_ack_ok = 0;
	
	for(i = 0; i < AP_HASH_SIZE; ++i)
		INIT_HLIST_HEAD(&gpstaglinkmgt->ap_list[i]);
	pthread_mutex_init(&gpstaglinkmgt->ap_hlist_mutex, NULL);
	
	for(i = 0; i < STA_HASH_SIZE; ++i)
		INIT_HLIST_HEAD(&gpstaglinkmgt->sta_list[i]);
	pthread_mutex_init(&gpstaglinkmgt->sta_hlist_mutex, NULL);

	pthread_mutex_init(&gpstaglinkmgt->ag_msg_mutex, NULL);

	gpstaglinkmgt->debug_level = DEBUG_LEVER_NONE;
	aglink_socket_info_init();
	/*获取设备信息*/
	aglink_get_devinfo();
#ifndef GW_X86

	if(IS_DEV_AP())
	{
		ag_module_Agconfig_init(gpstaglinkmgt->pAgconfig);
	}

	if(IS_DEV_GW())
	{		
		/*AP型网关*/
		#ifndef GW_X86
		ag_module_Agconfig_init(gpstaglinkmgt->pAgconfig);
		#endif
	}
#endif	
	/*ag_module层与aglink配置信息建立关联*/
	((AG_MODULE_SETUP *)pagmodule)->pmoduleinfo = gpstaglinkmgt;
	
	return TRUE;
}

s32 ag_module_contral_init_aglink(u8 *pagmodule)
{
    	
	return TRUE;
}

s32 ag_module_bussiness_init_aglink()
{
	
    	return TRUE;
}

s32 ag_module_event_aglink(u8 *pagmodule)
{
	AG_MODULE_SETUP *pstModuleSetup = NULL;	

	if(NULL == pagmodule)
	{
		AGLINK_LOG(LOG_ALERT, "param is NULL!");
		return FALSE;
	}

	gpstaglinkmgt->main_base = event_init();

	if(IS_DEV_GW())
	{
		#if 0
		/*初始化wifidog连接*/
		if(aglink_wifidog_connect_init() < 0){
			AGLINK_LOG(LOG_ERR, "wifidog connect init failed!");
			exit(-1);
		}
		#endif

		#ifdef _BOXLL_
		if(aglink_boxll_cnnt_init() < 0){
			AGLINK_LOG(LOG_ERR, "boxll connect init failed!");
			exit(-1);
		}
		#endif
	}	

	/* 注册ap-gw接受消息socket */
	event_set(&gpstaglinkmgt->rcv_ev, gpstaglinkmgt->sockfd,  EV_READ | EV_PERSIST, 
		aglink_recv_ag_msg, (void *)&gpstaglinkmgt->rcv_ev);
	event_add(&gpstaglinkmgt->rcv_ev, NULL);

	/*配置检查Event*/
	evtimer_set(&gpstaglinkmgt->config_check_timer_ev, aglink_config_check_timer_cb, NULL);
	event_add(&gpstaglinkmgt->config_check_timer_ev, &gpstaglinkmgt->config_check_tv);
	AGLINK_LOG(LOG_INFO, "event check!");

	/*ACK检查Event*/
	evtimer_set(&gpstaglinkmgt->ack_check_timer_ev, aglink_ack_recv_check_cb, NULL);
	event_add(&gpstaglinkmgt->ack_check_timer_ev, &gpstaglinkmgt->ack_check_tv);
	AGLINK_LOG(LOG_INFO, "ack check!");
	
	/*AP向 网关发送设备注册消息*/
	if(IS_DEV_AP()){
		//aglink_ap_send_dev_register_msg();
		/*设置keepalive 流程维护回调*/
		evtimer_set(&gpstaglinkmgt->keepalive_timer_ev, 
			aglink_ap_send_keepalive_timer_cb, NULL);
		/* 设置AP信息上报回调函数 */
		
		#ifdef _BOXLL_
		evtimer_set(&gpstaglinkmgt->b_mgr->report_ap_info_ev, aglink_ap_send_ap_info_timer_cb, NULL);		
		event_add(&gpstaglinkmgt->b_mgr->report_ap_info_ev, &gpstaglinkmgt->b_mgr->ap_info_tv);
		#endif
	}

	if(IS_DEV_GW())
	{
		/*网关开启AP老化Event*/
		evtimer_set(&gpstaglinkmgt->ap_age_timer_ev, aglink_gw_age_ap_cb, NULL);
		event_add(&gpstaglinkmgt->ap_age_timer_ev, &gpstaglinkmgt->ap_age_tv);
	}
	#if 0
	if(ag_module_ipc_thread_entry() != TRUE)
	{
		AG_MODULE_LOG(LOG_ERR, "ag_module_ipc_thread_start failed");
		return FALSE;
	}
	#endif

	//ag_module_ipc_start();
	if(ag_module_ipc_start() != TRUE)
	{
		AG_MODULE_LOG(LOG_ERR, "ag_module_ipc_thread_start failed");
		return FALSE;
	}
	
	sleep(5);
	AG_MODULE_LOG(LOG_ERR, "event base_loop");
	/*进入主循环*/
	event_base_loop(gpstaglinkmgt->main_base, 0);	

	AG_MODULE_LOG(LOG_ERR, "event base_loop exit");
			
    	return TRUE;
}

