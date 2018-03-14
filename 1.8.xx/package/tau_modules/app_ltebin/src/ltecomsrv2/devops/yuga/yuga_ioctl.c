/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : bsp_lte_ioctl.c
  版 本 号   : 初稿
  作    者   :  蔡兵
  生成日期   : 2012年7月23日 星期二
  最近修改   :
  功能描述   : bsp  lte ioctl
  函数列表   :
  修改历史   :
  1.日    期   : 2012年7月23日 星期二
    作    者   : 蔡兵
    修改内容   : 创建文件

******************************************************************************/
/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include <stdio.h>
#include <sys/select.h>
#include <termios.h>
#include <sys/time.h>
#include <fcntl.h>
#include <string.h>  
#include <sys/types.h>  
#include <errno.h>  
#include <sys/stat.h>   
#include <unistd.h>    
#include <stdlib.h>  
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/sysinfo.h>

#include "lte_com.h"
#include "lte_log.h"

#define ATCOM  "/dev/ttyUSB2"

#define YUGA_GSM          3
#define YUGA_TDSCDMA      4
#define YUGA_WCDMA        5
#define YUGA_LTE          9 

#define YUGA_GSM_ONLY 13
#define YUGA_TDS_ONLY 15
#define YUGA_WCD_ONLY 14
#define YUGA_LTE_ONLY 38
#define YUGA_GSM_TDS 60
#define YUGA_GSM_WCD 19
#define YUGA_TDS_LTE 65
#define YUGA_WCD_LTE 54
#define YUGA_GSM_TDS_LTE 61
#define YUGA_GSM_WCD_LTE 39
#define YUGA_GSM_WCD_TDS_LTE 2


#define YUGA_2G 0
#define YUGA_3G 2
#define YUGA_4G 7

#define LTE_YUGA_FLAG "CLM920_CDV1"

/*****************************************************************************
 函 数 名  : pal_bsp_lte_pin_state
 功能描述  : 获取PIN码状态
 输入参数  : void *pvData    
             int iDataLen  
 输出参数  : 无
 返 回 值  : LTE_RET_E
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月5日
    作    者   : 陈键
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_pin_state (void *pvData, char *pcBuf)
{
    int iRet = 0; 
    char *pcBufTmp = NULL;
    char *pcPinBuf = NULL;
    char acCiev[] = "+CPIN:";
    char acStateReady[] = "READY";
    char acStatePin[] = "PIN";
    char acStatePuk[] = "PUK";
    
    LTE_PIN_STATE_T *pstPinState = (LTE_PIN_STATE_T*)pvData;

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    pcBufTmp = strstr(pcBuf, acCiev);
    if(NULL == pcBufTmp)
    {
        return LTE_FAIL;
    }
    if(NULL == (pcPinBuf = strstr(pcBufTmp, ":")))
    {
        return LTE_FAIL;
    }

    if(NULL != strstr(pcPinBuf,acStateReady))
    {
        /*PIN 码属于 打开状态*/
        pstPinState->enState = PIN_STATE_READY;
    }
    else if(NULL != strstr(pcPinBuf,acStatePuk))
    {
        /*PIN 码属于 puk 状态*/
        pstPinState->enState = PIN_STATE_PUK;
    }
    else if(NULL != strstr(pcPinBuf,acStatePin))
    {
        /*PIN 码属于 关闭状态*/
        pstPinState->enState = PIN_STATE_PIN;
    }
    else
    {
        return LTE_FAIL;
    }

    return LTE_OK;
}

/*****************************************************************************
 函 数 名  : pal_bsp_lte_pin_switch
 功能描述  : 开启和关闭pin码功能
 输入参数  : void *pvData    
             int iDataLen  
 输出参数  : 无
 返 回 值  : LTE_RET_E
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月6日星期四
    作    者   : 陈键
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_pin_switch (void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte pin switch success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte pin switch failed!",__LINE__,__func__);
    }

    return LTE_FAIL;
}
/*****************************************************************************
 函 数 名  : leadcore_pin_sw_get
 功能描述  : 获取pin是否开启，1开启，0关闭
 输入参数  : void *pvData  
             int iDataLen  
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年11月6日星期四
    作    者   : liyoukun
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_pin_sw_get( void *pvData, char *pcBuf )
{
    int iRet = 0; 
    int iATExeCount = 0;
    int iFlag = 0;
    char *pcTmp = NULL;    
    char acBuffTmp[32] = {0};
    char acCiev[32] = "+CLCK:";
    FILE* pstFd = NULL;
    
    LTE_PIN_SW_STATE_T *pstPinSwState = (LTE_PIN_SW_STATE_T*)pvData;

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    

    if(NULL != (pcTmp = strstr(pcBuf, acCiev)))
    {
        sscanf(pcTmp, "%*[^ ] %s", acBuffTmp);
        printf("%s\n", acBuffTmp);
        pstPinSwState->enState = atoi(acBuffTmp);
    }
    else
    {
        return LTE_FAIL;
    }

    return LTE_OK;
}
 
/*****************************************************************************
 函 数 名  : pal_bsp_lte_pin_save
 功能描述  : 保存pin码到NVRAM
 输入参数  : void *pvData    
             int iDataLen  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月6日星期四
    作    者   : 陈键
    修改内容   : 新生成函数

*****************************************************************************/
 static LTE_RET_E yuga_pin_unlock (void *pvData, char *pcBuf)
{

    char acCiev[] = "OK";
        
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte unlock pin success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte unlock pin failed!",__LINE__,__func__);
    }

    return LTE_FAIL;
}

/*****************************************************************************
 函 数 名  : pal_bsp_lte_pin_numremain
 功能描述  : 获取剩余PIN码尝试次数
 输入参数  : void *pvData    
             int iDataLen  
 输出参数  : 无
 返 回 值  : LTE_RET_E
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月2日
    作    者   : 陈键
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_pin_attempts_get(void *pvData, int iDataLen)
{
    return LTE_OK;
}


/*****************************************************************************
 函 数 名  : pal_bsp_lte_pin_alter
 功能描述  : 修改pin码
 输入参数  : void *pvData    
             int iDataLen  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月6日星期四
    作    者   : 陈键
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_pin_alter (void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte alter pin success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte alter pin failed!",__LINE__,__func__);
    }

    return LTE_FAIL;
}
/*****************************************************************************
 函 数 名  : lte_power_on
 功能描述  : 启动 lte 模块
 输入参数  : iComFd 文件描述符,开不了机，需要系统重启的
 输出参数  : 
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月14 星期三
    作    者   : 蔡兵
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_power_on (void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    int iFd = 0;
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte module on success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }
}

/*****************************************************************************
 函 数 名  : lte_power_off
 功能描述  : 启动 lte 模块
 输入参数  : iComFd 文件描述符
 输出参数  : 
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月14 星期三
    作    者   : 蔡兵
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_power_off (void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";

    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte module off success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte module off failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;

}


/*****************************************************************************
 函 数 名  : pal_bsp_lte_signal_quality
 功能描述  : 获取电平信号值
 输入参数  : void *pvData    
             int iDataLen  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月23日
    作    者   : 陈键
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_signal_get  (void *pvData, char *pcBuf)
{
    int iRet = 0;
    char acCiev[] = "+CIND:";
    char acTmp[16] = {0};

    LTE_SIGNAL_T *pstSignal = (LTE_SIGNAL_T*)pvData;

    if(NULL == pvData)
    {
        return LTE_FAIL;
    }

    if(NULL != strstr(pcBuf, acCiev))
    {
        sscanf(pcBuf, "%*[^,],%[^,]", acTmp);
        pstSignal->iSignal = atoi(acTmp);
        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }
    

}

static LTE_RET_E yuga_power_get(void *pvData, char *pcBuf)
{
    return LTE_OK;
}


/*altered by caibing 2013-12-17*/
/*****************************************************************************
 函 数 名  : leadcore_nettype_set
 功能描述  : 设置网络类型及优先级
 输入参数  : void *pvData  
             int iDataLen  
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年11月25日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_nettype_set (void *pvData, char *pcBuf)
{ 
    char acCiev[] = "OK";

    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte net type set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte net type set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;
}

/*****************************************************************************
 函 数 名  : leadcore_nettype_get
 功能描述  : 获取模块当前配置的的网络类型
 输入参数  : void *pvData  
             int iDataLen  
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年11月25日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_nettype_get(void *pvData, char *pcBuf)
{
    int iRet = 0;
    char acCiev[] = "^MODECONFIG: ";
    char acNetType[16] = {0};
    int iNettype;

    if (pvData == NULL)
    {
		return LTE_FAIL;
    }
    /*^DDTM: 1,0,7,2,0      ^DDTM: 1,1,7*/
    LTE_NET_TYPE_T *pstNetType = (LTE_NET_TYPE_T *)pvData;
    if(NULL == strstr(pcBuf, acCiev))
    {
        return LTE_FAIL;
    }
    sscanf(pcBuf, "%*[^:]:%s", acNetType);
    iNettype = atoi(acNetType);
    if(YUGA_LTE_ONLY == iNettype)
    {
        pstNetType->enNetType = LTE_4G_ONLY;
    }
    else if(YUGA_TDS_ONLY == iNettype || YUGA_WCD_ONLY == iNettype)
    {
        pstNetType->enNetType = LTE_3G_ONLY;
    }
    else if(YUGA_GSM_ONLY == iNettype)
    {
        pstNetType->enNetType = LTE_2G_ONLY;
    }
    else if(YUGA_GSM_TDS == iNettype || YUGA_GSM_WCD == iNettype)
    {
        pstNetType->enNetType = LTE_2G_3G;
    }
    else if(YUGA_TDS_LTE == iNettype || YUGA_WCD_LTE == iNettype)
    {
        pstNetType->enNetType = LTE_3G_4G;
    }
    else if(YUGA_GSM_TDS_LTE == iNettype || YUGA_GSM_WCD_LTE == iNettype)
    {
        pstNetType->enNetType = LTE_2G_3G_4G;
    }
    else if(YUGA_GSM_WCD_TDS_LTE == iNettype)
    {
        pstNetType->enNetType = LTE_AUTO_MODE; /*模块支持自动模式*/
    }
    else
    {
        return LTE_FAIL;
    }
    return LTE_OK;
    
}


static LTE_RET_E yuga_imei_get(void *pvData, char *pcBuf)
{
    //char acCiev[] = "OK";
    LTE_IMEI_T *pstImei = (LTE_IMEI_T*)pvData;
    if(NULL == pcBuf || NULL == pstImei)
    {
        return LTE_FAIL;
    }

    sscanf(pcBuf, "%s", pstImei->acImei);
   
    LTE_LOG(LTE_LOG_INFO,"%d,%s: imei: %s",__LINE__,__func__, pstImei->acImei);
    
    return LTE_OK;

}
static LTE_RET_E yuga_cellid_get(void *pvData, char *pcBuf)
{
    /*+CELLINFO: 38950,9,-1,460,00,1A7C521,185A,40,5,5,(38950,9,-1001,-62,-740),(3790)*/
    char acCellid[32] = {0};
    char *pcTmp = NULL;
    char *pcCid = acCellid;
    char acCiev[] = "+CELLINFO:";
    LTE_CELLID_T *pstCellId = (LTE_CELLID_T*)pvData;
     
    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    if(NULL == (pcTmp = strstr(pcBuf, acCiev)))
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:get cellid error!!!\n",__LINE__,__func__);
        return LTE_FAIL;
    }
 
    sscanf(pcTmp,"%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%[^,]", acCellid);
    if(*pcCid == '0')
        pcCid++;
    strncpy(pstCellId->acCellID, pcCid, sizeof(pstCellId->acCellID));

    return LTE_OK;
}
static LTE_RET_E yuga_iccid_get(void *pvData, char *pcBuf)
{
    /*altered by caibing 2013-11-25*/
    char acCiev[32] = "ICCID:";
    char acIccid[32] = {0};
    char *pcTmp = NULL;
    int iLoop = 0;
    
    LTE_ICCID_T *pstIccid = (LTE_ICCID_T*)pvData;

    if (NULL == pvData)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:input param is NULL pointer\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    if(NULL != (pcTmp = strstr(pcBuf, acCiev)))
    {
        sscanf(pcTmp, "%*[^ ] %s", acIccid);
        strncpy(pstIccid->acICCID, acIccid, sizeof(pstIccid->acICCID));
        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }
    
}

static LTE_RET_E yuga_imsi_get(void *pvData, char *pcBuf)
{
    char acCiev[] = "+cimi:";
        
    LTE_IMSI_T *pstImsiInfo = (LTE_IMSI_T *)pvData;

    if (pvData == NULL)
    {
		return LTE_FAIL;
    }
    if(NULL != strstr(pcBuf, acCiev))
    {
        sscanf(pcBuf, "%*[^:]:%s", pstImsiInfo->acIMSI);
        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }

}

/*****************************************************************************
 函 数 名  : pal_bsp_register_net_carrier
 功能描述  : 根据设置，来给出状态机的接入技术
 输入参数  :  
 输出参数  : 0,auto,1 lte 接入，2 td-s 接入
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月17 星期五
    作    者   : 蔡兵
    修改内容   : 新生成函数
    XXXXXXXXXXXX需要重点关注

*****************************************************************************/
static LTE_RET_E yuga_plmn_register(void *pvData, int iDataLen)
{
     return LTE_OK;
}


/*****************************************************************************
 函 数 名  : set_prefer_plmn
 功能描述  : 设置plmn 中td-lte,fdd-lte优先，暂时模块不支持，预留接口
 输入参数  :  
 输出参数  : 
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月17 星期五
    作    者   : 蔡兵
    修改内容   : 新生成函数

*****************************************************************************/

static LTE_RET_E yuga_prefer_plmn_set (void *pvData, int iDataLen)
{
    return LTE_OK;
}

/*****************************************************************************
 函 数 名  : pal_bsp_lte_linkstate_get
 功能描述  : 判断lte 是否注册上网络
 输入参数  :  
 输出参数  : 
 返 回 值  :  0,未注册，1 注册,其他的均大于 1 ，这个函数其他的大于1，有异于 pal_bsp
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012 年8月20 星期一
    作    者   : 蔡兵
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_linkstate_get(void *pvData, char *pcBuf)
{
    return LTE_OK;
}

static LTE_RET_E yuga_network_prepare(void *pvData, char *pcBuf)
{
    char acNeedleCgreg[16] = "+CGREG";
    char acNeedleCereg[16] = "+CEREG";   
    char acNeedleCreg[16] = "+CREG";   

    char acBufTmp[32] = {0};
    char *pcTmp = NULL;
    
    if(NULL == pvData)    
    {
        return LTE_FAIL;
    }
    LTE_NETWORK_PREPARE_T *pstNwStatus = (LTE_NETWORK_PREPARE_T *)pvData;
    if(NULL != (pcTmp = strstr(pcBuf, acNeedleCreg)))
    {
        sscanf(pcTmp, "%*[^,],%s", acBufTmp);
    }
    else if(NULL != (pcTmp = strstr(pcBuf, acNeedleCgreg)))
    {
        sscanf(pcTmp, "%*[^,],%s", acBufTmp);
    }
    else if(NULL != (pcTmp = strstr(pcBuf, acNeedleCereg)))
    {
        sscanf(pcTmp, "%*[^,],%s", acBufTmp);
    }

    strncpy(pstNwStatus->acNetWorkStatus, acBufTmp, sizeof(pstNwStatus->acNetWorkStatus));
    
    return LTE_OK;
	
}


static LTE_RET_E yuga_connect(void *pvData, char *pcBuf)
{
    return LTE_OK;
}


static LTE_RET_E yuga_disconnect(void *pvData, char *pcBuf)
{
    return LTE_OK;
}

static LTE_RET_E yuga_gps_start(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";

    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:yuga_gps_start set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:yuga_gps_start set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;
}

static LTE_RET_E yuga_gps_stop(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";

    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:yuga_gps_stop set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:yuga_gps_stop set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;
}


static LTE_RET_E yuga_sim_state_get(void *pvData, char *pcBuf)
{
	LTE_SIM_STATE_T *pstSimState = (LTE_SIM_STATE_T *)pvData;
	if (pvData == NULL)
		return LTE_FAIL;
    
	if (NULL != strstr(pcBuf, "+CPIN:"))
	{
		pstSimState->enSimState = LTE_SIM_READY;
	}
    else
	{
		pstSimState->enSimState = LTE_SIM_NOT_FOUND;
	}
	
	return LTE_OK;
}

static LTE_RET_E yuga_lte_info_get(void *pvData, char *pcBuf)
{
    /*+CELLINFO: 38350,246,10,460,00,188110B,185A,39,5,5,(38350,246,-1057,-98,-761),()*/
    /**/
    LTE_INFO_T *pstLteInfo = (LTE_NET_TYPE_T *)pvData;
    char acBand[32] = {0};//8--- 39
    char acChannel[32] = {0}; // 1--- 38350
    char acRssi[32] = {0};// 15--- -761
    char acRsrq[32] = {0};// 14--- -98
    char acRsrp[32] = {0};// 13--- -1057
    
    char acSinr[32] = {0};// 
   
    char acPhyCellid[32] = {0};// 2--- 246
    char acCellid[32] = {0};// 6--- 188110B
    if(NULL == pstLteInfo)
    {
        return LTE_FAIL;
    }
    if(NULL == strstr(pcBuf, "+CELLINFO:"))
    {
        return LTE_FAIL;
    }

    sscanf(pcBuf, "%*[^ ] %[^,],%[^,],%*[^,],%*[^,],%*[^,],%[^,],%*[^,],%[^,],%*[^(](%*[^,],%*[^,],%[^,],%[^,],%[^)]", acChannel, acPhyCellid, acCellid, acBand, acRsrp, acRsrq, acRssi);

    strcpy(pstLteInfo->stCellInfo.acCellId, acCellid);
    pstLteInfo->stCellInfo.iRsrp = atoi(acRsrp)/10;
    pstLteInfo->stCellInfo.iRsrq = atoi(acRsrq)/10;
    pstLteInfo->stCellInfo.iRssi = atoi(acRssi)/10;
    //pstLteInfo->stCellInfo.iSinr = stQsqInfo.iSinr;
    pstLteInfo->stCellInfo.iChannel = atoi(acChannel);
    strcpy(pstLteInfo->stCellInfo.acBand, acBand);
    strcpy(pstLteInfo->stCellInfo.acPhyCeillid, acPhyCellid);
    
	return LTE_OK;
}

static LTE_RET_E yuga_access_net_get(void *pvData, char *pcBuf)
{
    int iRet = 0;
    char acNetType[16] = {0};
    int iNettype;

    if (pvData == NULL)
    {
		return LTE_FAIL;
    }
    /*+COPS: 0,0,"CHINA  MOBILE CMCC",7*/
    LTE_NET_TYPE_T *pstNetType = (LTE_NET_TYPE_T *)pvData;
    if(NULL == strstr(pcBuf, "+COPS:"))
    {
        return LTE_FAIL;
    }

    sscanf(pcBuf, "%*[^,],%*[^,],%*[^,],%s", acNetType);
    iNettype = atoi(acNetType);
    if(YUGA_4G == iNettype)
    {
        pstNetType->enNetType = NET_4G;
    }
    else if(YUGA_3G == iNettype)
    {
        pstNetType->enNetType = NET_3G;
    }
    else if(YUGA_2G == iNettype)
    {
        pstNetType->enNetType = NET_2G;
    }
    else
    {
        pstNetType->enNetType = iNettype;
    }
    return LTE_OK;
    #if 0
    int iRet = 0;
    char acNetType[16] = {0};
    int iNettype;

    if (pvData == NULL)
    {
		return LTE_FAIL;
    }
    /*^SYSINFO: 2,3,0,5,1*/
    LTE_NET_TYPE_T *pstNetType = (LTE_NET_TYPE_T *)pvData;
    if(NULL == strstr(pcBuf, "^SYSINFO:"))
    {
        return LTE_FAIL;
    }

    sscanf(pcBuf, "%*[^,],%*[^,],%*[^,],%[^,]", acNetType);
    iNettype = atoi(acNetType);
    if(YUGA_LTE == iNettype)
    {
        pstNetType->enNetType = NET_4G;
    }
    else if(YUGA_TDSCDMA == iNettype || YUGA_WCDMA == iNettype)
    {
        pstNetType->enNetType = NET_3G;
    }
    else if(YUGA_GSM == iNettype)
    {
        pstNetType->enNetType = NET_2G;
    }
    else
    {
        pstNetType->enNetType = NET_4G; /*模块暂时支持LTE单模*/
    }
    return LTE_OK;
    #endif
}

static LTE_RET_E yuga_bands_get(void *pvData, char *pcBuf)
{
    if (pvData == NULL)
    {
        return LTE_FAIL;
    }

    return LTE_OK;
}


static LTE_RET_E yuga_clck_bands_set(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
        
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte apn set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte apn set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;
}

static LTE_RET_E yuga_clck_bands_close(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
        
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte apn set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte apn set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;
}

static LTE_RET_E yuga_clckcell_enable_get(void *pvData, char *pcBuf)
{
    return LTE_OK;
}
static LTE_RET_E yuga_clck_cellid_set(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
        
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte clckcell set success!",__LINE__,__func__);
        return LTE_OK;
    }
    
    LTE_LOG(LTE_LOG_ERR,"%d,%s:lte clckcell set failed!",__LINE__,__func__);
    return LTE_FAIL;

}
/*****************************************************************************
   函 数 名  : leadcore_nettype_info_get
   功能描述  : 获取模块支持的网络类型，如2g，3g，4g
   输入参数  : void *pvData
             int iDataLen
   输出参数  : 无
   返 回 值  : static
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2014年11月12日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_sup_nettype_get(void *pvData, char *pcBuf)
{
    
    return LTE_OK;

}
/*****************************************************************************
   函 数 名  : leadcore_oper_info_get
   功能描述  : 获取运营商信息
   输入参数  : void *pvData
             int iDataLen
   输出参数  : 无
   返 回 值  : static
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2014年11月10日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_oper_info_get(void *pvData, char *pcBuf)
{

    int iRet = 0;
    char acCiev[] = "+COPS:";
    char acOkCiev[] = "OK";
    char acOperTmp[16] = {0};
    char *pcTmp = NULL;
    LTE_RET_E enRet = 0;

    LTE_OPER_INFO_T *pstOperInfo = (LTE_OPER_INFO_T *)pvData;
    
    if (pvData == NULL)
    {
        return LTE_FAIL;
    }

    if(NULL != (pcTmp = strstr(pcBuf, acOkCiev)))
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:at command success",__LINE__,__func__);
        return LTE_OK;
    }
    
    if (NULL != strstr(pcBuf, acCiev))
    {
        sscanf(pcBuf,"%*[^\"]\"%[^\"]", acOperTmp);   /*+COPS: 0,2,"46000",7*/

        if(NULL != strstr(LTE_CHINA_MOBILE_CODE, acOperTmp))
        {
            strncpy(pstOperInfo->acOperName, LTE_CHINA_MOBILE_NAME, sizeof(pstOperInfo->acOperName));
        }
        else if(NULL != strstr(LTE_CHINA_UNICOM_CODE, acOperTmp))
        {
            strncpy(pstOperInfo->acOperName, LTE_CHINA_UNICOM_NAME, sizeof(pstOperInfo->acOperName));
        }
        else if(NULL != strstr(LTE_CHINA_TELECOM_CODE, acOperTmp))
        {
            strncpy(pstOperInfo->acOperName, LTE_CHINA_TELECOM_NAME, sizeof(pstOperInfo->acOperName));
        }
        else
        {
            strncpy(pstOperInfo->acOperName, acOperTmp, sizeof(pstOperInfo->acOperName));
        }
                
        return LTE_OK;
    }
    
    return LTE_FAIL;

}


/*****************************************************************************
   函 数 名  : leadcore_sms_mode_set
   功能描述  : 设置短信模式，0表示pdu模式，1表示文本模式
   输入参数  : void *pvData
             int iDataLen
   输出参数  : 无
   返 回 值  : static
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2014年11月18日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_sms_mode_set(void *pvData, char *pcBuf)
{

    char acCiev[] = "OK";

    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte sms mode set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte sms mode set failed!",__LINE__,__func__);
    }

    return LTE_FAIL;

}

/*****************************************************************************
   函 数 名  : yuga_sms_func_init
   功能描述  : 短信功能初始化接口
   输入参数  : void
   输出参数  : 无
   返 回 值  : static
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2014年11月13日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_sms_func_init(void *pvData, char *pcBuf)
{

    char acCiev[] = "OK";

    if(NULL != strstr(pcBuf, "+CSMS:"))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte sms init [CSMS] success!",__LINE__,__func__);
        return LTE_OK;
    }
    else if(NULL != strstr(pcBuf, "+CPMS:"))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte sms init [CPMS] success!",__LINE__,__func__);
        return LTE_OK;
    }
    else if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte sms init success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte sms init failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;

}

/*****************************************************************************
   函 数 名  : yuga_sms_cent_code_set
   功能描述  : 设置短信中心号
   输入参数  : void *pvData
             int iDataLen
   输出参数  : 无
   返 回 值  : static
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2014年11月13日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_sms_cent_code_set(void *pvData, char *pcBuf)
{

    char acCiev[] = "OK";

    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte sms cent code set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte sms cent code set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;

}

/*****************************************************************************
   函 数 名  : yuga_sms_recv
   功能描述  : 按pdu模式读取短信
   输入参数  : void *pvData
             int iDataLen
   输出参数  : 无
   返 回 值  : static
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2014年11月26日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_sms_recv(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    int  iFd = 0;
    int  iWriteLen = 0;
    LTE_SMS_RECV_T *pstSmsRecv = (LTE_SMS_RECV_T *)pvData;

    if(NULL == pvData)
    {
        return LTE_FAIL;
    }
    strncpy(pstSmsRecv->acSmsRecvFile, LTE_SMS_RECV_FILE, sizeof(pstSmsRecv->acSmsRecvFile));

    return LTE_OK;
}


/*****************************************************************************
 函 数 名  : yuga_sms_send
 功能描述  : 发送短信，支持长短信发送
 输入参数  : void *pvData  
             int iDataLen  
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年12月17日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_sms_send(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte sms send success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte sms send failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;
}

/*****************************************************************************
   函 数 名  : yuga_sms_dele
   功能描述  :  短信删除接口
   输入参数  : void *pvData
             int iDataLen
   输出参数  : 无
   返 回 值  : static
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2014年11月13日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_sms_dele(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte sms delete success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte sms delete failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;
}
/*****************************************************************************
 函 数 名  : yuga_sms_sup_func
 功能描述  : 获取模块是否支持短信功能
 输入参数  : void *pvData  
             int iDataLen  
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年12月8日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_sms_sup_func(void *pvData, char *pcBuf)
{
    LTE_SUPPORT_SMS_T *pstSup = (LTE_SUPPORT_SMS_T *)pvData;
    pstSup->enSupSms = SUP_SMS_FUNC;

    return LTE_OK;

}
static LTE_RET_E yuga_cgdcont_set(void *pvData, char *pcBuf)
{
    #if 0
    char acCiev[] = "OK";

    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte pdp set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte pdp set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;
    #endif
    return LTE_OK;
}
/*****************************************************************************
   函 数 名  : leadcore_apn_set
   功能描述  : 设置apn
   输入参数  : void *pvData
             int iDataLen
   输出参数  : 无
   返 回 值  : static
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2014年11月17日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_apn_set(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";

    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte apn set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte apn set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;

}

static LTE_RET_E yuga_version_get(void *pvData, char *pcBuf)
{
    int iRet = 0;  
    char acVersion[64] = {0};
    char *pcTmp = NULL;
    
    LTE_MODULE_VERSION_T *pstVersion = (LTE_MODULE_VERSION_T *)pvData;

    if(NULL == pvData)
    {
        return LTE_FAIL;
    }
    if(NULL != strstr(pcBuf, LTE_YUGA_FLAG))
    {
        sscanf(pcBuf, "%[^ ] ", acVersion);
        pstVersion->iVersionIndex = LTE_YUGA;
    }
    else
    {   
        pstVersion->iVersionIndex = LTE_VER_IND_MAX;
    }
    
    strncpy(pstVersion->acVersion, acVersion, sizeof(pstVersion->acVersion));
    
    return LTE_OK;
}

static LTE_RET_E yuga_enodeb_id_get(void *pvData, char *pcBuf)
{
    /*+CELLINFO: 38950,9,-1,460,00,1A7C521,185A,40,5,5,(38950,9,-1001,-62,-740),(3790)*/
    unsigned long ulEnodeb;
    char acCellid[32] = {0};
    char *pcTmp = NULL;
    char acCiev[] = "+CELLINFO:";
    LTE_ENODB_ID_T *pstEnodebId = (LTE_ENODB_ID_T*)pvData;
     
    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    if(NULL == (pcTmp = strstr(pcBuf, acCiev)))
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:get cellid error!!!\n",__LINE__,__func__);
        return LTE_FAIL;
    }
 
    sscanf(pcTmp,"%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%[^,]", acCellid);
    ulEnodeb = strtoul(acCellid, NULL, 16);
    snprintf(pstEnodebId->acEnodebId, sizeof(pstEnodebId->acEnodebId),"%u", (ulEnodeb >> 8));

    return LTE_OK;

}

/*****************************************************************************
 函 数 名  : leadcore_tac_get
 功能描述  : 获取小区所属的区位跟踪码
 输入参数  : void *pvData  
             int iDataLen  
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年2月4日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_tac_get(void *pvData, char *pcBuf)
{
    /*+CELLINFO: 38950,9,-1,460,00,1A7C521,185A,40,5,5,(38950,9,-1001,-62,-740),(3790)*/
    unsigned long ulTac;
    char acTacId[32] = {0};
    char *pcTmp = NULL;
    char acCiev[] = "+CELLINFO:";
    LTE_TAC_T *pstTac = (LTE_TAC_T *)pvData;
     
    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    if(NULL == (pcTmp = strstr(pcBuf, acCiev)))
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:get cellid error!!!\n",__LINE__,__func__);
        return LTE_FAIL;
    }
 
    sscanf(pcTmp,"%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%[^,]", acTacId);
    ulTac = strtoul(acTacId, NULL, 16);  
    snprintf(pstTac->acTac, sizeof(pstTac->acTac),"%u", ulTac);

    return LTE_OK;

}

static LTE_RET_E yuga_lcid_get(void *pvData, char *pcBuf)
{
    /*+CELLINFO: 38950,9,-1,460,00,1A7C521,185A,40,5,5,(38950,9,-1001,-62,-740),(3790)*/
    unsigned long ulLcid;
    char acCellid[32] = {0};
    char *pcTmp = NULL;
    char acCiev[] = "+CELLINFO:";
    LTE_LCID_T *pstLcid = (LTE_LCID_T*)pvData;
     
    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    if(NULL == (pcTmp = strstr(pcBuf, acCiev)))
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:get cellid error!!!\n",__LINE__,__func__);
        return LTE_FAIL;
    }
 
    sscanf(pcTmp,"%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%[^,]", acCellid);
    ulLcid = strtoul(acCellid, NULL, 16);
        
    /*共四个字节，取Cellid的低8位*/
    snprintf(pstLcid->acLcid, sizeof(pstLcid->acLcid),"%u", (ulLcid & LTE_LCID_MASK));

    return LTE_OK;
}

static LTE_RET_E yuga_all_cellid_get(void *pvData, char *pcBuf)
{   
    /*+CELLINFO: 38950,9,-1,460,00,1A7C521,185A,40,5,5,(38950,9,-1001,-62,-740),(3790)*/
    unsigned long ulCellId;
    unsigned long ulTac;
    char acCellid[32] = {0};
    char acTacId[32] = {0};
    char *pcTmp = NULL;
    char *pcCid = acCellid;
    char acCiev[] = "+CELLINFO:";
    LTE_ALL_CELLID_T *pstAllCid = (LTE_ALL_CELLID_T *)pvData;
     
    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    if(NULL == (pcTmp = strstr(pcBuf, acCiev)))
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:get cellid error!!!\n",__LINE__,__func__);
        return LTE_FAIL;
    }
 
    sscanf(pcTmp,"%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%[^,],%[^,]", acCellid, acTacId);
    ulCellId = strtoul(acCellid, NULL, 16);
    ulTac = strtoul(acTacId, NULL, 16);
    /*cellid*/  
    if(*pcCid == '0')
        pcCid++;
    strncpy(pstAllCid->acCellId, pcCid, sizeof(pstAllCid->acCellId));
    /*共四个字节，取CellId的前三个字节*/
    snprintf(pstAllCid->acEnodebId, sizeof(pstAllCid->acEnodebId),"%u", (ulCellId >> 8));
    /*共四个字节，取CellId的第四个字节*/
    snprintf(pstAllCid->acLcid, sizeof(pstAllCid->acLcid),"%u", (ulCellId & LTE_LCID_MASK));
    snprintf(pstAllCid->acTac, sizeof(pstAllCid->acTac),"%u", ulTac);

    return LTE_OK;


}

static LTE_RET_E yuga_mac_set(void *pvData, char *pcBuf)
{
    return LTE_OK;
}

static LTE_RET_E yuga_mac_get(void *pvData, char *pcBuf)
{
    return LTE_OK;
}

static LTE_RET_E yuga_puk_unlock(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";

    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:zte_micro_b57_n_puk_unlock success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:zte_micro_b57_n_puk_unlock failed!",__LINE__,__func__);
    }   

    return LTE_FAIL;
}

LTE_MODULE_OPS_T stLte_yuga_ops_t = {
    .lte_pin_state_get       = yuga_pin_state,
    .lte_pin_switch          = yuga_pin_switch,    
    .lte_pin_switch_get      = yuga_pin_sw_get,
    .lte_pin_unlock          = yuga_pin_unlock,
    .lte_pin_attempts        = NULL,
    .lte_pin_code_alter      = yuga_pin_alter,
    .lte_signal_get          = yuga_signal_get,
    .lte_power_on            = yuga_power_on,
    .lte_power_off           = yuga_power_off,
    .lte_power_state_get     = NULL,
    .lte_net_type_set        = yuga_nettype_set,
    .lte_net_type_get        = yuga_nettype_get,
    .lte_plmn_register       = NULL,
    .lte_plmn_prefer_set     = NULL,
    .lte_net_work_prepare    = yuga_network_prepare,
    .lte_net_work_status_get = NULL,
    .lte_connect             = yuga_connect,
    .lte_disconnect          = yuga_disconnect,
    .lte_link_state_get      = yuga_linkstate_get,
    .lte_imei_get            = yuga_imei_get,
    .lte_cellid_get          = yuga_cellid_get,
    .lte_iccid_get           = yuga_iccid_get,
    .lte_imsi_get            = yuga_imsi_get,
    .lte_sim_state_get       = yuga_sim_state_get,
    .lte_info_get            = yuga_lte_info_get,
    .lte_access_net_get      = yuga_access_net_get,
    .lte_bands_get           = yuga_bands_get,
    .lte_clck_bands          = yuga_clck_bands_set,
    .lte_clck_cell_get       = yuga_clckcell_enable_get,
    .lte_clck_cell           = yuga_clck_cellid_set,
    .lte_gps_start           = yuga_gps_start, 
    .lte_gps_stop            = yuga_gps_stop,
    .lte_oper_info_get       = yuga_oper_info_get,
    .lte_nettype_info_get    = yuga_sup_nettype_get,
    .lte_sms_cent_code_set   = yuga_sms_cent_code_set,
    .lte_sms_mode_set        = yuga_sms_mode_set,
    .lte_sms_func_init       = yuga_sms_func_init,
    .lte_sms_recv            = yuga_sms_recv,
    .lte_sms_send            = yuga_sms_send,
    .lte_sms_del             = yuga_sms_dele,
    .lte_sms_supinfo_get     = yuga_sms_sup_func,
    .lte_sms_ip_stack_set    = yuga_cgdcont_set,
    .lte_apn_get             = NULL,
    .lte_apn_set             = yuga_apn_set,
    .lte_version_get         = yuga_version_get,
    .lte_enodebid_get        = yuga_enodeb_id_get,
    .lte_tac_get             = yuga_tac_get,
    .lte_lcid_get            = yuga_lcid_get,
    .lte_all_cellid_get      = yuga_all_cellid_get,
    .lte_mac_set             = yuga_mac_set,
    .lte_mac_get             = yuga_mac_get,
    .lte_sim_puk_unlock      = yuga_puk_unlock,
};

