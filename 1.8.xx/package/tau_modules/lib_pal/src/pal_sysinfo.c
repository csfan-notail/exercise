#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "ap_types.h"
#include "app_include/log.h"

#include "pal_include/pal_sys.h"
#include "pal_factory.h"
#include "pal_include/pal_common.h"


int pal_sys_get_customer_prosn(char *sn)
{
    return factory_get(sn,DEVICE_CUSTOMER_SN,32);
}

int get_customer_prosn(char *sn)
{
    return pal_sys_get_customer_prosn(sn);
}



int get_oem_company(char *company)
{
    FILE *data;
    char *buff;
    int len;

    if(!system("oem company|grep company >"OEM_INFO_PATH))
    {
        if(NULL == (data = fopen(OEM_INFO_PATH, "r")))
        {
            lapp_log(APP_LOG_ERR, "open "OEM_INFO_PATH" failed");
            return ERROR;
        }
        fseek(data, 0, SEEK_END);
        len = ftell(data)-12;
        fseek(data, 12, SEEK_SET);
        buff = NULL;
        buff = (char *)malloc(len+1);
        if(NULL == buff)
        {
            lapp_log(APP_LOG_ERR, "buff malloc failed.");
            fclose(data);
            free(buff);
            return ERROR;
        }
        memset(buff, 0, len+1);
        fread(buff, len+1, 1, data);
        lapp_log(APP_LOG_ERR, "buff = %s", buff);
        fclose(data);

        strcpy(company, buff);
        company[strlen(company)-1]= '\0';
        lapp_log(APP_LOG_ERR, "company = %s", company);
        free(buff);
    }
    return OK;
}

static s32 pal_sys_get_cpu_info(SYS_CPU_RATE  *pstCPURate)
{
	FILE *pfile = NULL;
	u8 ucBuffer[256] = {0};
	static SYS_CPU_INFO stpre = {0};
	SYS_CPU_INFO stCur = {0};
	u64 uiTotal = 0;
	
	pfile = fopen(FILE_PROC_STAT, "r");
	if(NULL == pfile)
	{
		gw_log(LOG_ALERT, "Open file "FILE_PROC_STAT"error(%d)", errno);
		return FALSE;
	}
	if(!fgets(ucBuffer, sizeof(ucBuffer), pfile))
	{
		gw_log(LOG_ALERT, "Read file error(%d)", errno);
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
	
	memcpy(&stpre, &stCur, sizeof(SYS_CPU_INFO));

	gw_log(LOG_INFO, "CPU: %d (system=%d user=%d nice=%d idle=%d)", 
		pstCPURate->cpu_rate, pstCPURate->system, pstCPURate->user, pstCPURate->nice, pstCPURate->idle);
	
	return TRUE;
}


static s32 pal_sys_get_free_info(SYS_DISK_INFO *pstDiskInfo)
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

	gw_log(LOG_INFO, "total:%d used:%d", pstDiskInfo->total, pstDiskInfo->used);
	
	return TRUE;
}



int device_cpu_get(int *cpu)
{
    SYS_CPU_RATE  stCPURate;
    if( FALSE ==pal_sys_get_cpu_info(&stCPURate))
        return ERROR;
    *cpu = stCPURate.cpu_rate;
    return OK;
}

int device_mem_get(int *mem)
{
    SYS_DISK_INFO stDiskInfo;
    if( FALSE == pal_sys_get_free_info(&stDiskInfo))
        return ERROR;
    *mem = stDiskInfo.used;
    return OK;
}

s32 pal_get_hw_info(u32 *phwtype)
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
		gw_log(LOG_ALERT, " fopen %s fail\n", acFileName);
		return FALSE;
	}

	fseek(pfData, 0, SEEK_END);
	ulLen = ftell(pfData);			/* 获取文件字节数*/
	fseek(pfData, 0, SEEK_SET);

	pucBuf = MEM_CALLOC(ulLen+1);
	if (pucBuf == NULL) {
		gw_log(LOG_ALERT, "MALLOC len:%d fail\n", ulLen);
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
		*phwtype = HWTYPE_IXP;
		gw_log(LOG_INFO,"AG_APHWTYPE_IXP\n");
		goto ret;
	}

	/* 查找Atheros AR7240*/
	pucPos = strstr(pucBuf, "Atheros AR7240");
	if (pucPos != NULL)
	{
		*phwtype  = HWTYPE_AP93;
		gw_log(LOG_INFO,"AG_APHWTYPE_AP93\n");
		goto ret;
	}

	/* 2011-8-8 pyy add: AR7241/AR7242 AP93 */
	/* 查找Atheros AR7241*/
	pucPos = strstr(pucBuf, "Atheros AR7241");
	if (pucPos != NULL)
	{
		*phwtype = HWTYPE_AP93;
		gw_log(LOG_INFO,"AG_APHWTYPE_AP93\n");
		goto ret;
	}

	/* 查找Atheros AR7242*/
	pucPos = strstr(pucBuf, "Atheros AR7242");
	if (pucPos != NULL)
	{
		*phwtype = HWTYPE_AP93;
		gw_log(LOG_INFO,"AG_APHWTYPE_AP93\n");
		goto ret;
	}

	/* begin zsy: by for 9344, 2012/03/14 */
	/* 商用版支持9344平台 */
	pucPos = strstr(pucBuf, "Atheros AR934x");
	if (pucPos != NULL)
	{
		*phwtype = HWTYPE_9344;
		gw_log(LOG_INFO,"AG_APHWTYPE_9344\n");
		goto ret;
	}

	/* BEGIN: Added by Zhou Shouya, 2012/9/13	PN:支持WallAP平台， 芯片是9341 */
	/* 商用版支持9344平台 */
	pucPos = strstr(pucBuf, "Atheros AR9341");
	if (pucPos != NULL)
	{

		*phwtype  = HWTYPE_TA40;
		gw_log(LOG_INFO,"AG_APHWTYPE_TA40\n");
		goto ret;
	}
	/* END:   Added by Zhou Shouya, 2012/9/13 */
	/* BEGIN: Added by lvjianlin, 2013/7/24   PN:AP9331_OAM */
	pucPos = strstr(pucBuf, "Atheros AR933x");
	if (pucPos != NULL)
	{
		*phwtype = HWTYPE_TA33;
		gw_log(LOG_INFO,"AG_APHWTYPE_TA33\n");
		goto ret;
	}
	/* END:   Added by lvjianlin, 2013/7/24   PN:AP9331_OAM */
	/* BEGIN: Added by wangdusun, 2013/8/22   PN:支持TA55平台 */
	pucPos = strstr(pucBuf, "QCA955x");
	if (pucPos != NULL)
	{

		*phwtype = HWTYPE_TA55;
		gw_log(LOG_INFO,"AG_APHWTYPE_TA55\n");
		goto ret;
	}
	/* END:   Added by wangdusun, 2013/8/22   PN:支持TA55平台 */

	pucPos = strstr(pucBuf, "QCA9531");
	if (pucPos != NULL)
	{
		*phwtype = HWTYPE_TA37;
		gw_log(LOG_INFO,"AG_APHWTYPE_TA37\n");
		goto ret;
	}

	pucPos = strstr(pucBuf, "QCA9563");
	if (pucPos != NULL)
	{
		*phwtype = HWTYPE_TA62;
		gw_log(LOG_INFO,"AG_APHWTYPE_TA62\n");
		goto ret;
	}

ret:
	gw_log(LOG_INFO,"phwtype:%d\n", *phwtype);

	MEM_FREE(pucBuf);
	return TRUE;
}



int pal_sysinfo_cur_soft_ver(char *app_ver)
{
    int ret=0;
    int targetver=0;
    char factory_value[FACTORY_VALUE_MAX_LEN]={0};
    char factory_name[FACTORY_NAME_MAX_LEN]={0};
    if(!app_ver)
        return FALSE;
    
    ret = factory_get(factory_value,"targetver",64);
    if(ret)
        return FALSE;

    targetver = atoi(factory_value);
    sprintf(factory_name,"mtdblock%d",targetver);
    ret = factory_get(factory_value,factory_name,64);
    if(ret)
        return FALSE;
    memcpy(app_ver,factory_value,strlen(factory_value));
    gw_log(LOG_ERR,"current soft ver : (%s) targetver(%d)",app_ver,targetver);
    return TRUE;
}



int pal_sysinfo_device_model_id(void)
{
    char device_model_id[32]={0};
    if(!factory_get(device_model_id,DEVICE_MODEL_ID,32))
        return 7005001;
    else 
        return atoi(device_model_id);    
}


int pal_sysinfo_device_internel_model_name(char *int_model_name)
{
    return factory_get(int_model_name,DEVICE_INT_MODEL_NAME,32);
}


int pal_sysinfo_device_name(char *device_name)
{
    return factory_get(device_name,DEVICE_NAME,32);
}


int pal_sysinfo_device_type(char *device_type)
{
    return factory_get(device_type,DEVICE_TYPE,32);
}