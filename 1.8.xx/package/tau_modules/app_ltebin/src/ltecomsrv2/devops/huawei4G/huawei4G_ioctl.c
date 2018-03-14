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


#define ATCOM  "/dev/ttyUSB8"

extern LTE_STATE_INFO_T g_stLteStateInfo;

#if 0
#define LEADCORE_GSM      0
#define LEADCORE_TDS      2
#define LEADCORE_LTE      7 
#endif

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
static LTE_RET_E huawei4G_pin_state (void *pvData, char *pcBuf)
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
static LTE_RET_E huawei4G_pin_switch (void *pvData, char *pcBuf)
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
static LTE_RET_E huawei4G_pin_sw_get( void *pvData, char *pcBuf )
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
        sscanf(pcTmp, "%*[^:]:%s", acBuffTmp);
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
 static LTE_RET_E huawei4G_pin_unlock (void *pvData, char *pcBuf)
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
static LTE_RET_E huawei4G_pin_attempts_get(void *pvData, int iDataLen)
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
static LTE_RET_E huawei4G_pin_alter (void *pvData, char *pcBuf)
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
static LTE_RET_E huawei4G_power_on (void *pvData, char *pcBuf)
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
static LTE_RET_E huawei4G_power_off (void *pvData, char *pcBuf)
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
static LTE_RET_E huawei4G_signal_get  (void *pvData, char *pcBuf)
{
    int iRet = 0;
    char acCiev[] = "+CSQ:";
    char acTmp[16] = {0};

    LTE_SIGNAL_T *pstSignal = (LTE_SIGNAL_T*)pvData;

    if(NULL == pvData)
    {
        return LTE_FAIL;
    }

    if(NULL != strstr(pcBuf, acCiev))
    {
        sscanf(pcBuf, "%*[^:]:%s", acTmp);
        pstSignal->iSignal = atoi(acTmp);
        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }
    

}
#if 0
static LTE_RET_E leadcore_power_get(void *pvData, char *pcBuf)
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
static LTE_RET_E leadcore_nettype_set (void *pvData, char *pcBuf)
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
static LTE_RET_E leadcore_nettype_get(void *pvData, char *pcBuf)
{
    int iRet = 0;
    char acNetPri[16] = {0};
    char acNetType[16] = {0};
    //int iNetPri;
    int iNettype;
    char acNets[32] = {0};

    if (pvData == NULL)
    {
		return LTE_FAIL;
    }
    /*^DDTM: 1,0,7,2,0      ^DDTM: 1,1,7*/
    LTE_NET_TYPE_T *pstNetType = (LTE_NET_TYPE_T *)pvData;
    if(NULL == strstr(pcBuf, "^DDTM:"))
    {
        return LTE_FAIL;
    }

    sscanf(pcBuf, "%*[^,],%[^,],%[^,]", acNetPri, acNetType);
    if( atoi(acNetPri) == 1)  /*单模*/
    {
        //sscanf(pcBuf, "%*[^,],%*[^,],%[^,]", acNetType);
        iNettype = atoi(acNetType);
        if(LEADCORE_LTE == iNettype)
        {
            pstNetType->enNetType = LTE_4G_ONLY;
        }
        else if(LEADCORE_TDS == iNettype)
        {
            pstNetType->enNetType = LTE_3G_ONLY;
        }
        else
        {
            pstNetType->enNetType = LTE_2G_ONLY;
        }
    }
    else if(atoi(acNetPri) == 0)
    {
        sscanf(pcBuf, "%*[^,],%*[^,],%s", acNets);
        if(NULL != strstr(acNets, "0"))
        {
            pstNetType->enNetType |= LTE_2G_ONLY;
        }
        if(NULL != strstr(acNets, "2"))
        {
            pstNetType->enNetType |= LTE_3G_ONLY;
        }
        if(NULL != strstr(acNets, "7"))
        {
            pstNetType->enNetType |= LTE_4G_ONLY;
        }
    }
    else
    {
        pstNetType->enNetType = LTE_4G_ONLY; /*模块暂时支持LTE单模*/
    }
      
    return LTE_OK;
    
}
#endif

static LTE_RET_E huawei4G_imei_get(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    LTE_IMEI_T *pstImei = (LTE_IMEI_T*)pvData;

    //if(NULL != strstr(pcBuf, acCiev))
    //{
        sscanf(pcBuf, "%s", pstImei->acImei);
        //return LTE_OK;
   // }
   // else
   // {
     //   return LTE_FAIL;
   // }
    LTE_LOG(LTE_LOG_INFO,"%d,%s: imei: %s",__LINE__,__func__, pstImei->acImei);
    
    return LTE_OK;

}

static LTE_RET_E huawei4G_cellid_get(void *pvData, char *pcBuf)
{
     char acCellid[32] = {0};
     char *pcTmp = NULL;
     char *pcCid = acCellid;
     
     if (NULL == pvData)
     {
         return LTE_FAIL;
     }
     //^CECELLID: 46000,25694475,246,6234
     LTE_CELLID_T *pstCellId = (LTE_CELLID_T*)pvData;
#if 0
    if(NULL != strstr(pcBuf, "OK"))
    {
        return LTE_OK;
    }
    else if(NULL != (pcTmp = strstr(pcBuf, "+CEREG:")))
    {
        sscanf(pcTmp,"%*[^\"]\"%*[^\"]\"%*[^\"]\"%[^\"]", acCellid);
        
        LTE_LOG(LTE_LOG_INFO, "%d,%s:cellid [%s]\n",__LINE__,__func__, acCellid);
        if(*pcCid == '0')
            pcCid++;
        strncpy(pstCellId->acCellID, pcCid, sizeof(pstCellId->acCellID));

        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }
#else
    if(NULL != strstr(pcBuf, "OK"))
    {
        return LTE_OK;
    }
    else if(NULL != (pcTmp = strstr(pcBuf, "^CECELLID:")))
    {
        sscanf(pcTmp,"%*[^,],%[^,]", acCellid);
        
        LTE_LOG(LTE_LOG_INFO, "%d,%s:cellid [%s]\n",__LINE__,__func__, acCellid);
        snprintf(pstCellId->acCellID, sizeof(pstCellId->acCellID), "%X", atoi(acCellid));
        //strncpy(pstCellId->acCellID, pcCid, sizeof(pstCellId->acCellID));

        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }
#endif
    

}

static LTE_RET_E huawei4G_iccid_get(void *pvData, char *pcBuf)
{
    /*altered by caibing 2013-11-25*/
    char acCiev[32] = "^ICCID:";
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

static LTE_RET_E huawei4G_imsi_get(void *pvData, char *pcBuf)
{       
    LTE_IMSI_T *pstImsiInfo = (LTE_IMSI_T *)pvData;

    if (pvData == NULL)
    {
		return LTE_FAIL;
    }

    sscanf(pcBuf, "%s", pstImsiInfo->acIMSI);
    return LTE_OK;

}
#if 0

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
static LTE_RET_E leadcore_plmn_register(void *pvData, int iDataLen)
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

static LTE_RET_E leadcore_prefer_plmn_set (void *pvData, int iDataLen)
{
    return LTE_OK;
}
#endif
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
static LTE_RET_E huawei4G_linkstate_get(void *pvData, char *pcBuf)
{
    int iRet = 0;
    int iLinkState = 0;
    char *pcTmp = NULL;
    char acBuffTmp[16];
    char acCiev[] = "^NDISSTATQRY:";
    /*^NDISSTATQRY: 1,,,"IPV4",0,,,"IPV6"*/
    LTE_LINKSTATE_T *pstLinkstate = (LTE_LINKSTATE_T*)pvData;

    if (pvData == NULL)
    {
		return LTE_FAIL;
    }

    if(NULL != (pcTmp = strstr(pcBuf, acCiev)))
    {
        sscanf(pcTmp, "%*[^:]:%[^,]", acBuffTmp);
    }

    iLinkState = atoi(acBuffTmp);
    
    if(0 == iLinkState)
    {
        pstLinkstate->enLinkstate = LTE_LINKSTATE_DISCONNECT;
    }
    else if(1 == iLinkState)
    {
        pstLinkstate->enLinkstate = LTE_LINKSTATE_CONNECTED;
    }

    LTE_LOG(LTE_LOG_INFO,"%d,%s:linkstate:%d",__LINE__,__func__, pstLinkstate->enLinkstate);
    return LTE_OK;
}

static LTE_RET_E huawei4G_network_prepare(void *pvData, char *pcBuf)
{
    char acNeedleCgreg[16] = "+CGREG";
    char acNeedleCereg[16] = "+CEREG";   
    char acNeedleCreg[16] = "+CREG";   

    char acBufTmp[16] = {0};
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


static LTE_RET_E huawei4G_connect(void *pvData, char *pcBuf)
{
    char acCiev[] = "^NDISSTAT:1,";
            
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte connect success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte connect failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;
}


static LTE_RET_E huawei4G_disconnect(void *pvData, char *pcBuf)
{
    char acCiev[] = "^NDISSTAT:0,";

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

static LTE_RET_E huawei4G_sim_state_get(void *pvData, char *pcBuf)
{
	LTE_SIM_STATE_T *pstSimState = (LTE_SIM_STATE_T *)pvData;
	char acCiev[] = "+CPIN:";
	char acBufTmp[16] = {0};
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
	/*
	if (NULL != strstr(pcBuf, acCiev))
	{
        sscanf(pcBuf, "%*[^:]:%s", acBufTmp);
	    if('1' == acBufTmp[0])
	    {
            pstSimState->enSimState = LTE_SIM_READY;
	    }
		else
		{
            pstSimState->enSimState = LTE_SIM_NOT_FOUND;
		}
	}
    else
	{
		pstSimState->enSimState = LTE_SIM_NOT_FOUND;
	}
	
	return LTE_OK;
	*/
}

static LTE_RET_E huawei4G_lte_info_get(void *pvData, char *pcBuf)
{ 
	LTE_INFO_T *pstLteInfo = (LTE_INFO_T *)pvData;
	char acRsrp[32] = {0};
    char acRsrq[32] = {0};
    char acRssi[32] = {0};
    char acSinr[32] = {0};
    char acBand[32] = {0};
    char acChannel[32] = {0};
    char acPhyCeillid[32] = {0};
    char acCellid[32] = {0};
    
	if (pvData == NULL)
		return LTE_FAIL;
    /*^CERSSI:0,0,0,0,255,-103,-16,-4,32639,32639,32639*/
    //printf("pcBuf==%s\n", pcBuf);
   // LTE_LOG(LTE_LOG_NOTICE,"%d,%s:pcBuf==%s",__LINE__,__func__, pcBuf);
    if(NULL != strstr(pcBuf, "^CERSSI:"))
    {
        sscanf(pcBuf, "%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%[^,],%[^,],%[^,]", acRsrp, acRsrq, acSinr);
        pstLteInfo->stCellInfo.iRsrp = atoi(acRsrp);
        pstLteInfo->stCellInfo.iRsrq = atoi(acRsrq);
        pstLteInfo->stCellInfo.iSinr = atoi(acSinr);
    }
    /*^LWURC: 0,18900,5,18900,5,39*/
    else if(NULL != strstr(pcBuf, "^LWURC:"))
    {
        sscanf(pcBuf, "%*[^,],%*[^,],%*[^,],%[^,],%*[^,],%s", acChannel, acBand);     
        strncpy(g_stLteStateInfo.acBand, acBand, sizeof(pstLteInfo->stCellInfo.acBand));
        //if(39 == atoi(acBand))
        //{
        //    g_stLteStateInfo.iChannel = atoi(acChannel)+19450;//由 频率*10 换算成频点
        //}
        //else
        //{
            //pstLteInfo->stCellInfo.iChannel = atoi(acChannel)/10;
            g_stLteStateInfo.iChannel = atoi(acChannel);
        //}
    }
    //^CSG:341,-832,-160,-536,0,0
    else if(NULL != strstr(pcBuf, "^CSG:"))
    {
        sscanf(pcBuf, "%*[^:]:%[^,]", acPhyCeillid);
        strncpy(g_stLteStateInfo.acPhyCellId, acPhyCeillid, sizeof(pstLteInfo->stCellInfo.acBand));
    }
    //^CECELLID: 46000,25694475,246,6234
    else if(NULL != strstr(pcBuf, "^CECELLID:"))
    {
        sscanf(pcBuf, "%*[^,],%[^,],%[^,]", acCellid, acPhyCeillid);
        snprintf(pstLteInfo->stCellInfo.acCellId, sizeof(pstLteInfo->stCellInfo.acCellId), "%X", atoi(acCellid));
        strncpy(pstLteInfo->stCellInfo.acPhyCeillid, acPhyCeillid, sizeof(pstLteInfo->stCellInfo.acPhyCeillid));
    }
    else if(NULL != strstr(pcBuf, "OK"))
    {
        //memset(pstLteInfo, 0, sizeof(LTE_INFO_T));
    }
    else
    {
        return LTE_FAIL;
    }
    g_stLteStateInfo.iLteInfoStaticFLag = 1;

	return LTE_OK;
}

static LTE_RET_E huawei4G_access_net_get(void *pvData, char *pcBuf)
{
    char acCiev[] = "^SYSINFOEX:";//^SYSINFOEX:2,3,0,1,,6,"LTE",101,"LTE"
    char acMode[128] = {0};
    int iMode;
    LTE_ACCESS_NETWORK_T *pstAccessNet = (LTE_ACCESS_NETWORK_T*)pvData;
    char *pcTmp = NULL;

	if (pvData == NULL)
		return LTE_FAIL;
    
    if (NULL != strstr(pcBuf,acCiev))
    {
        sscanf(pcBuf, "%*[^,],%*[^,],%*[^,],%*[^,],%s", acMode);
        pcTmp = strchr(acMode, ',');
        iMode = atoi(pcTmp+1);      
        
        

        if(6 == iMode)
        {
            pstAccessNet->enAccessNet = NET_4G;
        }
        else if(3 == iMode || 4 == iMode)
        {
            pstAccessNet->enAccessNet = NET_3G;
        }
        else if(1 == iMode)
        {
            pstAccessNet->enAccessNet = NET_2G;
        }
        else
        {
            pstAccessNet->enAccessNet = NET_NONE;
        }
        return LTE_OK;
	
    }
    

	return LTE_FAIL;
}
#if 0
static LTE_RET_E leadcore_bands_get(void *pvData, char *pcBuf)
{

	return LTE_OK;

}


static LTE_RET_E leadcore_clck_bands_set(void *pvData, char *pcBuf)
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

static LTE_RET_E leadcore_clck_bands_close(void *pvData, char *pcBuf)
{

    return LTE_OK;

}

static LTE_RET_E leadcore_clckcell_enable_get(void *pvData, char *pcBuf)
{
    return LTE_OK;
}
#endif

static LTE_RET_E huawei4G_clck_cellid_set(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
        
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte clckcell set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte clckcell set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;

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
static LTE_RET_E huawei4G_oper_info_get(void *pvData, char *pcBuf)
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
        //LTE_LOG(LTE_LOG_ERR,"%d,%s:at command success",__LINE__,__func__);
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

#if 0
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
static LTE_RET_E leadcore_sms_mode_set(void *pvData, char *pcBuf)
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
    
    return LTE_OK;

}

/*****************************************************************************
   函 数 名  : leadcore_sms_init
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
static LTE_RET_E leadcore_sms_func_init(void *pvData, char *pcBuf)
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
   函 数 名  : leadcore_cent_code_set
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
static LTE_RET_E leadcore_sms_cent_code_set(void *pvData, char *pcBuf)
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
   函 数 名  : leadcore_sms_recv
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
static LTE_RET_E leadcore_sms_recv(void *pvData, char *pcBuf)
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
 函 数 名  : leadcore_sms_send
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
static LTE_RET_E leadcore_sms_send(void *pvData, char *pcBuf)
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
   函 数 名  : leadcore_sms_dele
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
static LTE_RET_E leadcore_sms_dele(void *pvData, char *pcBuf)
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
 函 数 名  : leadcore_sms_sup_func
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
static LTE_RET_E leadcore_sms_sup_func(void *pvData, char *pcBuf)
{
    LTE_SUPPORT_SMS_T *pstSup = (LTE_SUPPORT_SMS_T *)pvData;
    pstSup->enSupSms = SUP_SMS_FUNC;
    return LTE_OK;

}
static LTE_RET_E leadcore_cgdcont_set(void *pvData, char *pcBuf)
{
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

}
#endif

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
static LTE_RET_E huawei4G_apn_set(void *pvData, char *pcBuf)
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

static LTE_RET_E huawei4G_version_get(void *pvData, char *pcBuf)
{
    int iRet = 0;   
    char acVersion[64] = {0};
    
    LTE_MODULE_VERSION_T *pstVersion = (LTE_MODULE_VERSION_T *)pvData;

    if(NULL == pvData)
    {
        return LTE_FAIL;
    }
    sscanf(pcBuf, "%s", acVersion);
    strncpy(pstVersion->acVersion, acVersion, sizeof(pstVersion->acVersion));
    pstVersion->iVersionIndex = LTE_HUAWEI4G;
    
    return LTE_OK;


}

static LTE_RET_E huawei4G_enodeb_id_get(void *pvData, char *pcBuf)
{
    unsigned long ulEnodeb;
    char acCellid[32] = {0};
    char *pcTmp = NULL;
    LTE_RET_E enRet = 0;
    
    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_ENODB_ID_T *pstEnodebId = (LTE_ENODB_ID_T*)pvData;

    if(NULL != strstr(pcBuf, "OK"))
    {
        return LTE_OK;
    }
    else if(NULL != (pcTmp = strstr(pcBuf, "+CEREG:")))
    {
        sscanf(pcTmp,"%*[^\"]\"%*[^\"]\"%*[^\"]\"%[^\"]", acCellid);
        LTE_LOG(LTE_LOG_INFO, "%d,%s:cellid [%s]\n", __LINE__, __func__, acCellid);
        
        ulEnodeb = strtoul(acCellid, NULL, 16);
        snprintf(pstEnodebId->acEnodebId, sizeof(pstEnodebId->acEnodebId),"%u", (ulEnodeb >> 8));
        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }

    return LTE_OK;

}

/*****************************************************************************
 函 数 名  : huawei4G_tac_get
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
static LTE_RET_E huawei4G_tac_get(void *pvData, char *pcBuf)
{
    unsigned long ulTac;
    char acTacId[32] = {0};
    char *pcTmp = NULL;
    
    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    LTE_TAC_T *pstTac = (LTE_TAC_T *)pvData;

    if(NULL != strstr(pcBuf, "OK"))
    {
        return LTE_OK;
    }
    else if(NULL != (pcTmp = strstr(pcBuf, "+CEREG:")))
    {
        sscanf(pcTmp,"%*[^\"]\"%[^\"]", acTacId);

        LTE_LOG(LTE_LOG_INFO, "%d,%s:acTacId [%s]\n",__LINE__,__func__, acTacId);
        ulTac = strtoul(acTacId, NULL, 16);
        
        snprintf(pstTac->acTac, sizeof(pstTac->acTac),"%u", ulTac);

        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }

    return LTE_OK;

}

static LTE_RET_E huawei4G_lcid_get(void *pvData, char *pcBuf)
{
    unsigned long ulLcid;
    char acCellid[32] = {0};
    char *pcTmp = NULL;
    
    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_LCID_T *pstLcid = (LTE_LCID_T*)pvData;

    if(NULL != strstr(pcBuf, "OK"))
    {
        return LTE_OK;
    }
    else if(NULL != (pcTmp = strstr(pcBuf, "+CEREG:")))
    {
        sscanf(pcTmp,"%*[^\"]\"%*[^\"]\"%*[^\"]\"%[^\"]", acCellid);

        LTE_LOG(LTE_LOG_INFO, "%d,%s:cellid [%s]\n",__LINE__,__func__, acCellid);
        ulLcid = strtoul(acCellid, NULL, 16);
        
        /*共四个字节，取cid的前三个字节*/
        snprintf(pstLcid->acLcid, sizeof(pstLcid->acLcid),"%u", (ulLcid & LTE_LCID_MASK));

        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }
   
}

static LTE_RET_E huawei4G_all_cellid_get(void *pvData, char *pcBuf)
{   
    unsigned long ulCellId;
    unsigned long ulTac;
    char acCellid[32] = {0};
    char acTacId[32] = {0};
    char *pcTmp = NULL;
    char *pcCid = acCellid;

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }

    LTE_ALL_CELLID_T *pstAllCid = (LTE_ALL_CELLID_T *)pvData;

    if(NULL != strstr(pcBuf, "OK"))
    {
       return LTE_OK;
    }
#if 0
    else if(NULL != (pcTmp = strstr(pcBuf, "+CEREG:")))
    {
       sscanf(pcTmp,"%*[^\"]\"%*[^\"]\"%*[^\"]\"%[^\"]", acCellid);
       sscanf(pcTmp,"%*[^\"]\"%[^\"]", acTacId);
    }
    else
    {
       return LTE_FAIL;
    }

    ulCellId = strtoul(acCellid, NULL, 16);
    ulTac = strtoul(acTacId, NULL, 16);
    /*cellid*/  
    if(*pcCid == '0')
        pcCid++;
    strncpy(pstAllCid->acCellId, pcCid, sizeof(pstAllCid->acCellId));
    /*共四个字节，取cid的前三个字节*/
    snprintf(pstAllCid->acEnodebId, sizeof(pstAllCid->acEnodebId),"%u", (ulCellId >> 8));
    /*共四个字节，取cid的第四个字节*/
    snprintf(pstAllCid->acLcid, sizeof(pstAllCid->acLcid),"%u", (ulCellId & LTE_LCID_MASK));
    snprintf(pstAllCid->acTac, sizeof(pstAllCid->acTac),"%u", ulTac);
#else
    //^CECELLID: 46000,25694475,246,6234
    else if(NULL != (pcTmp = strstr(pcBuf, "^CECELLID:")))
    {
       sscanf(pcTmp,"%*[^,],%[^,],%*[^,],%s", acCellid, acTacId);
    }
    else
    {
       return LTE_FAIL;
    }
    ulCellId = atoi(acCellid);
    snprintf(pstAllCid->acCellId, sizeof(pstAllCid->acCellId), "%X", ulCellId);
    snprintf(pstAllCid->acEnodebId, sizeof(pstAllCid->acEnodebId),"%u", (ulCellId >> 8));
    snprintf(pstAllCid->acLcid, sizeof(pstAllCid->acLcid),"%u", (ulCellId & LTE_LCID_MASK));
    strncpy(pstAllCid->acTac, acTacId, sizeof(pstAllCid->acTac));

#endif
    return LTE_OK;

}
#if 0
static LTE_RET_E leadcore_mac_set(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte mac set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte mac set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;
}

static LTE_RET_E leadcore_mac_get(void *pvData, char *pcBuf)
{
	char acCiev[] = "^QMAC:";
        
    LTE_MOUDLE_MAC_T *pstMac = (LTE_MOUDLE_MAC_T *)pvData;

    if (pvData == NULL)
    {
		return LTE_FAIL;
    }
    if(NULL != strstr(pcBuf, acCiev))
    {
        sscanf(pcBuf, "%*[^ ] %s", pstMac->acMac);
        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }

}

static LTE_RET_E leadcore_sim_unlock(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";

    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:leadcore_bind_machine_card success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:leadcore_bind_machine_card failed!",__LINE__,__func__);
    }    
    return LTE_FAIL;
}

static LTE_RET_E leadcore_sim_disable_unlock(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";

    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:leadcore_remove_bind success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:leadcore_remove_bind failed!",__LINE__,__func__);
    }   
    return LTE_FAIL;
}

static LTE_RET_E leadcore_sim_type_get(void *pvData, char *pcBuf)
{
    char acCiev[] = "D0248103012303820281828D1508";
    LTE_SIM_RANDOM_NUM_T *pstRandNum = (LTE_SIM_RANDOM_NUM_T *)pvData;

    if (pvData == NULL)
    {
		return LTE_FAIL;
    }

    if(NULL == strstr(pcBuf, "AABB00CCDD"))
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:leadcore_remove_bind failed!",__LINE__,__func__);
        return LTE_FAIL;
    }

    if(NULL != strstr(pcBuf, acCiev))
    {
        pstRandNum->iSIMType = ENCRYPTION_SIM;
    }
    else
    {
        pstRandNum->iSIMType = NORMAL_SIM;
    }
    return LTE_OK;

    
}

static LTE_RET_E leadcore_all_plmn_mode_get(void *pvData, char *pcBuf)
{
    char acCiev[] = "+COPS:";
    LTE_ALL_PLMN_MODE_T *pstAllPlmnMode = (LTE_ALL_PLMN_MODE_T *)pvData;
    char *p, *pObject;
    char acPlmn[16] = {0};
    char acMode[16] = {0};
    
    if(NULL == (p = strstr(pcBuf, acCiev)))
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:leadcore_all_plmn_mode_get failed!",__LINE__,__func__);
        return LTE_FAIL;
    }
    /*LTE_LOG(LTE_LOG_INFO, "%d,%s:leadcore_all_plmn_mode_get success!",__LINE__,__func__);
    if(NULL != strstr(pcBuf, "CHINA  MOBILE"))
    {
        pstAllPlmnMode->iAllOper |= MOBILE;
    }
    if(NULL != strstr(pcBuf, "CHN-UNICOM"))
    {
        pstAllPlmnMode->iAllOper |= UNICOM;
    }
    if(NULL != strstr(pcBuf, "CHINA TELECOM"))
    {
        pstAllPlmnMode->iAllOper |= TELECOM;
    }*/
    memset(pstAllPlmnMode->acAllPlmn, 0, sizeof(pstAllPlmnMode->acAllPlmn));
    pObject = pstAllPlmnMode->acAllPlmn;
    while(NULL != (p = strstr(p, "460")))
    {
        sscanf(p, "%[^\"]\",%[^)]", acPlmn, acMode);
        p += 8;
        if(0 == strcmp("7", acMode))
        {
            sprintf(pObject, "%s+4G,", acPlmn);
        }
        else if(0 == strcmp("2", acMode))
        {
            sprintf(pObject, "%s+3G,", acPlmn);
        }
        else if(0 == strcmp("0", acMode))
        {
            sprintf(pObject, "%s+2G,", acPlmn);
        }
        else
        {
            break;
        }
        pObject += strlen(pObject);    
    }
    *(pObject - 1) = '\0';
    return LTE_OK;    
}

static LTE_RET_E leadcore_sounet_mode_set(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";

    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:leadcore_sounet_mode_set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:leadcore_sounet_mode_set failed!",__LINE__,__func__);
    }   
    return LTE_FAIL;
}
   
static LTE_RET_E leadcore_unlock_puk(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";

    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:leadcore_unlock_puk success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:leadcore_unlock_puk failed!",__LINE__,__func__);
    }   
    return LTE_FAIL;
}

static LTE_RET_E leadcore_pin_input_remain(void *pvData, char *pcBuf)
{
    char    acTacTmp[32] = {0};
    int     iRet = 0;
    char    acCiev[32] = "^DRAP:";
    char    *pcBufTmp = NULL;

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_PIN_INPUT_REMAIN_T *pstPinInputRemain = (LTE_PIN_INPUT_REMAIN_T *)pvData;
    
    if(NULL != (pcBufTmp = strstr(pcBuf, acCiev)))
    {
        sscanf(pcBufTmp, "%*[^:]:%[^,]", acTacTmp);
    }
    else
    {
        return LTE_FAIL;
    }
    
    pstPinInputRemain->iPinIputRemain = atoi(acTacTmp);
    
    return LTE_OK;
}
#endif
static LTE_RET_E huawei4G_post_route_set(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte post_route set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte post_route set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;

}
static LTE_RET_E huawei4G_nv_datalock(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte nv_datalock set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte nv_datalock set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;

}
static LTE_RET_E huawei4G_post_route_get(void *pvData, char *pcBuf)
{
    char    acTacTmp[32] = {0};
    int     iRet = 0;
    char    acCiev[32] = "+RBMS:";
    char    *pcBufTmp = NULL;

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_POST_ROUTE_SW_T *pstPostRouteSw  = (LTE_POST_ROUTE_SW_T *)pvData;
    
    if(NULL != (pcBufTmp = strstr(pcBuf, acCiev)))
    {
        sscanf(pcBufTmp, "%*[^:]:%s", acTacTmp);
    }
    else
    {
        return LTE_FAIL;
    }
    
    pstPostRouteSw->iPostRouteSw = atoi(acTacTmp);
    
    return LTE_OK;
}

static LTE_RET_E huawei4G_remove_attachments(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte remove_attachments set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte remove_attachments set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;

}

static LTE_RET_E huawei4G_qos_set(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte qos set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte qos set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;

}

static LTE_RET_E huawei4G_qos_get(void *pvData, char *pcBuf)
{
	printf("SCsdfsdgvsdb x\n");
	char    acTacTmp[32] = {0};
    int     iRet = 0;
    char    acCiev[32] = "+CGEQOS:";
    char    *pcBufTmp = NULL;
    char 	acQosCid[32]={0};
	char 	acQosQci[32]={0};
	char 	acQosDlGbr[32]={0};
	char 	acQosUlGbr[32]={0};
	char 	acQosDlMbr[32]={0};
	char 	acQosUlMbr[32]={0};
    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_QOS_DATA_S *pstQosData  = (LTE_QOS_DATA_S *)pvData;
    printf("pbuf = %s\n",pcBuf);
    if(NULL != (pcBufTmp = strstr(pcBuf, acCiev)))
    {
        //sscanf(pcBufTmp, "%*[^:],%s", acTacTmp);
        sscanf(pcBufTmp,"%*[^:]:%[^,]",acQosCid);
		sscanf(pcBufTmp,"%*[^,],%[^,],%*[^,]",acQosQci);
		sscanf(pcBufTmp,"%*[^,],%*[^,],%[^,]",acQosDlGbr);
		sscanf(pcBufTmp,"%*[^,],%*[^,],%*[^,],%[^,]",acQosUlGbr);
		sscanf(pcBufTmp,"%*[^,],%*[^,],%*[^,],%*[^,],%[^,]",acQosDlMbr);
		sscanf(pcBufTmp,"%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%[^,]",acQosUlMbr);
		sscanf(pcBufTmp, "%*[^:],%s", acTacTmp);
		
    }
    else if(NULL != (pcBufTmp = strstr(pcBuf,"OK")))
    {
    	
		
    	return LTE_OK;
        
    }
	else
	{	
		
		return LTE_FAIL;
	}
    pstQosData->iQosCid=atoi(acQosCid);
	pstQosData->iQosQci=atoi(acQosQci);
	pstQosData->iQosDlGbr =atoi(acQosDlGbr);
	pstQosData->iQosUlGbr=atoi(acQosUlGbr);
	pstQosData->iQosDlMbr= atoi(acQosDlMbr);
	pstQosData->iQosUlMbr=atoi(acQosUlMbr);
	
    return LTE_OK;
}

static LTE_RET_E huawei4G_dload_prot_ver_get(void *pvData, char *pcBuf)
{
    return LTE_OK;
}
static LTE_RET_E huawei4G_dload_ver_info_get(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    LTE_DLOAD_VER_INFO_GET_T *pstDloadVerInfo = (LTE_DLOAD_VER_INFO_GET_T*)pvData;

    sscanf(pcBuf,"%s",pstDloadVerInfo->acDloadVerInfo);

    LTE_LOG(LTE_LOG_INFO, "%d,%s:lte dload_ver_info_get success!",__LINE__,__func__);
    
    return LTE_OK;
}
static LTE_RET_E huawei4G_nv_backup(void *pvData, char *pcBuf)
{
    char acTacTmp[16] = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_NV_BACKUP_T *pstNvBackup= (LTE_NV_BACKUP_T *)pvData;
    sscanf(pcBuf, "%s", acTacTmp);
    
    pstNvBackup->iNvBackup = atoi(acTacTmp);
    
    return LTE_OK;
}
static LTE_RET_E huawei4G_nv_restore(void *pvData, char *pcBuf)
{
    char acTacTmp[16] = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_NV_RESTORE_T *pstNvRestore= (LTE_NV_RESTORE_T *)pvData;
    sscanf(pcBuf, "%s", acTacTmp);
    
    pstNvRestore->iNvRestore = atoi(acTacTmp);
    
    return LTE_OK;
}
static LTE_RET_E huawei4G_dload_mode(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte dload_mode success!",__LINE__,__func__);
        return LTE_OK;
    }else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte dload_mode failed!",__LINE__,__func__);
    }
    return LTE_FAIL;
}
static LTE_RET_E huawei4G_data_mode(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte data_mode success!",__LINE__,__func__);
        return LTE_OK;
    }else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte data_mode failed!",__LINE__,__func__);
    }
    return LTE_FAIL;
}
static LTE_RET_E huawei4G_reset(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte reset success!",__LINE__,__func__);
        return LTE_OK;
    }else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_reset failed!",__LINE__,__func__);
    }
    return LTE_FAIL;
}

static LTE_RET_E huawei4G_sim_switch_set(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte sim switch set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte sim switch set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;

}
static LTE_RET_E huawei4G_sim_switch_get(void *pvData, char *pcBuf)
{
    int     iRet = 0;
    char    acCiev[32] = "^HVSST:";
    char 	acSimSw[32]={0};
	char    *pcBufTmp = NULL;
	
    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_SIM_SWITCH_T *pstSimSwitch  = (LTE_SIM_SWITCH_T *)pvData;
	
	
    if(NULL != (pcBufTmp = strstr(pcBuf, acCiev)))
    {
        sscanf(pcBufTmp, "%*[^:]:%s", acSimSw);
		
        sscanf(acSimSw, "%[^,],%*[^,],%*[^,],%*[^,],", acSimSw);
		
    }
    else if(NULL != (pcBufTmp = strstr(pcBuf,"OK")))
    {
    	return LTE_OK;
        
    }
	else
	{	
		return LTE_FAIL;
	}
    pstSimSwitch->iSimSw=atoi(acSimSw);

    return LTE_OK;
}
static LTE_RET_E huawei4G_sim_switch_soft_set(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte sim switch soft set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte sim switch soft set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;

}

LTE_MODULE_OPS_T stLte_huawei4G_ops_t = {
    .lte_pin_state_get       = huawei4G_pin_state,
    .lte_pin_switch          = huawei4G_pin_switch,    
    .lte_pin_switch_get      = huawei4G_pin_sw_get,
    .lte_pin_unlock          = huawei4G_pin_unlock,
    .lte_pin_attempts        = NULL,
    .lte_pin_code_alter      = huawei4G_pin_alter,
    .lte_signal_get          = huawei4G_signal_get,
    .lte_power_on            = huawei4G_power_on,
    .lte_power_off           = huawei4G_power_off,
    .lte_power_state_get     = NULL,
    .lte_net_type_set        = NULL,
    .lte_net_type_get        = NULL,
    .lte_plmn_register       = NULL,
    .lte_plmn_prefer_set     = NULL,
    .lte_net_work_prepare    = huawei4G_network_prepare,
    .lte_net_work_status_get = NULL,
    .lte_connect             = huawei4G_connect,
    .lte_disconnect          = huawei4G_disconnect,
    .lte_link_state_get      = huawei4G_linkstate_get,
    .lte_imei_get            = huawei4G_imei_get,
    .lte_cellid_get          = huawei4G_cellid_get,
    .lte_iccid_get           = huawei4G_iccid_get,
    .lte_imsi_get            = huawei4G_imsi_get,
    .lte_sim_state_get       = huawei4G_sim_state_get,
    .lte_info_get            = huawei4G_lte_info_get,
    .lte_access_net_get      = huawei4G_access_net_get,
    .lte_bands_get           = NULL,
    .lte_clck_bands          = NULL,
    .lte_clck_cell_get       = NULL,
    .lte_clck_cell           = huawei4G_clck_cellid_set,
    .lte_gps_start           = NULL, 
    .lte_gps_stop            = NULL,
    .lte_oper_info_get       = huawei4G_oper_info_get,
    .lte_nettype_info_get    = NULL,
    .lte_sms_cent_code_set   = NULL,
    .lte_sms_mode_set        = NULL,
    .lte_sms_func_init       = NULL,
    .lte_sms_recv            = NULL,
    .lte_sms_send            = NULL,
    .lte_sms_del             = NULL,
    .lte_sms_supinfo_get     = NULL,
    .lte_sms_ip_stack_set    = NULL,
    .lte_apn_get             = NULL,
    .lte_apn_set             = huawei4G_apn_set,
    .lte_version_get         = huawei4G_version_get,
    .lte_enodebid_get        = huawei4G_enodeb_id_get,
    .lte_tac_get             = huawei4G_tac_get,
    .lte_lcid_get            = huawei4G_lcid_get,
    .lte_all_cellid_get      = huawei4G_all_cellid_get,
    .lte_mac_set             = NULL,
    .lte_mac_get             = NULL,
    .lte_sim_unlock          = NULL,
    .lte_sim_disable_unlock  = NULL,
    .lte_sim_type_get        = NULL,
    .lte_all_plmn_mode_get   = NULL,
    .lte_sounet_mode_set     = NULL,
    .lte_sim_puk_unlock 	 = NULL,
    .lte_pin_input_remain    = NULL,
    .lte_post_route_set      = huawei4G_post_route_set,
    .lte_nv_datalock         = huawei4G_nv_datalock,
    .lte_post_route_get      = huawei4G_post_route_get,
    .lte_remove_attach       = huawei4G_remove_attachments,
    .lte_qos_set             = huawei4G_qos_set,
    .lte_qos_get             = huawei4G_qos_get,
	.lte_dload_prot_ver_get     = huawei4G_dload_prot_ver_get,
    .lte_dload_ver_info_get     = huawei4G_dload_ver_info_get,
    .lte_nv_backup           = huawei4G_nv_backup,
    .lte_nv_restore          = huawei4G_nv_restore,
    .lte_dload_mode          = huawei4G_dload_mode,
    .lte_data_mode           = huawei4G_data_mode,
    .lte_reset               = huawei4G_reset,
    .lte_sim_switch_set      = huawei4G_sim_switch_set,
    .lte_sim_switch_get      = huawei4G_sim_switch_get,
    .lte_sim_switch_soft_set = huawei4G_sim_switch_soft_set,
};

