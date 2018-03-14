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
     
#include "lte_log.h"
#include "lte_com.h"

     
#define LTE_VERSION_9105_FLAG   "LWA0032"
#define LTE_VERSION_9115_FLAG   "L0A0021"
#define LTE_VERSION_9106_FLAG   "LQAPQ03"

#define ATCOM  "/dev/ttyUSB2"
#define PIDFILE "/tmp/pidtimer"
#define BUFMAX 16
#define LONGCHEER_BAND_DEFAULT       "default"
     
typedef enum
{
     UMTS_ONLY = 1,                  
     AUTO      = 2,               
     GSM_ONLY  = 3,
     GSM_PREFERRED = 4,
     LTE_ONLY  = 5,
     TDSCDMA_ONLY   = 6,
     TDSCDMA_AND_WCDMA = 7
}LTE_LONGCHEER_NETTYPE_E;

static int g_connect_flag=0;
static int g_connect_fail=0;



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
static LTE_RET_E longcheer_pin_state (void *pvData, char *pcBuf)
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
static LTE_RET_E longcheer_pin_switch (void *pvData, char *pcBuf)
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
 函 数 名  : longcheer_pin_sw_get
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
static LTE_RET_E longcheer_pin_sw_get( void *pvData, char *pcBuf )
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
    }
    else
    {
        return LTE_FAIL;
    }
    pstPinSwState->enState = atoi(acBuffTmp);

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
 static LTE_RET_E longcheer_pin_unlock (void *pvData, char *pcBuf)
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
static LTE_RET_E longcheer_pin_attempts_get(void *pvData, int iDataLen)
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
static LTE_RET_E longcheer_pin_alter (void *pvData, char *pcBuf)
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
static LTE_RET_E longcheer_power_on (void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
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
static LTE_RET_E longcheer_power_off (void *pvData, char *pcBuf)
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
static LTE_RET_E longcheer_signal_get  (void *pvData, char *pcBuf)
{
    int iRet = 0;
    char acCiev[] = "+CIND:";
    char acNeedleComma[] = ",";
    char *pcBufTmp = NULL;
    char *pcTmp = NULL;

    LTE_SIGNAL_T *pstSignal = (LTE_SIGNAL_T*)pvData;

    if(NULL == pvData)
    {
        return LTE_FAIL;
    }  
    pcBufTmp = strstr(pcBuf,acCiev); 
    if(pcBufTmp != NULL)
    {
        pcTmp = strtok(pcBufTmp,acNeedleComma);
        if(pcTmp)
        {
            pcTmp = strtok(NULL,acNeedleComma);
            if(pcTmp)
            {
                ;
            }
            else
            {
                return LTE_FAIL;
            }
        }
        else
        {
            return LTE_FAIL;
        }
        
        pstSignal->iSignal = pcTmp[0]-48;

    }
    else
    {
        return LTE_FAIL;
    }

    return LTE_OK;

}

static LTE_RET_E longcheer_power_get(void *pvData, char *pcBuf)
{
    return LTE_OK;
}


/*altered by caibing 2013-12-17*/
/*****************************************************************************
 函 数 名  : longcheer_nettype_set
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
static LTE_RET_E longcheer_nettype_set (void *pvData, char *pcBuf)
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
 函 数 名  : longcheer_nettype_get
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
static LTE_RET_E longcheer_nettype_get(void *pvData, char *pcBuf)
{
    int iRet = 0;
    char *pcTmp = NULL;
    char acTmp[4] = {0};
    int iNettype = 0;

    if (pvData == NULL)
    {
		return LTE_FAIL;
    }

    LTE_NET_TYPE_T *pstNetType = (LTE_NET_TYPE_T *)pvData;

    if(NULL == (pcTmp = strstr(pcBuf, "+MODODR:")))
    {
        return LTE_FAIL;
    }

    sscanf(pcTmp, "%*[^:]:%s", acTmp);
    iNettype = atoi(acTmp);
    switch(iNettype)
    {
        case LTE_ONLY:
        {
            /*only lte*/
            pstNetType->enNetType = LTE_4G_ONLY;
            break;
        }
        
        case TDSCDMA_AND_WCDMA:
        {
            /*only td-s*/
            pstNetType->enNetType = LTE_3G_ONLY;
            break;
        }
        case AUTO:
        {
            pstNetType->enNetType = LTE_2G_3G_4G;
            break;

        }
        case GSM_ONLY:
        {
            pstNetType->enNetType = LTE_2G_ONLY;
            break;
        }   
        default:
        {
            LTE_LOG(LTE_LOG_ERR,"%d,%s:unkown net type\n",__LINE__,__func__);
            return LTE_FAIL;
        }
    }    
    return LTE_OK;
    
}

static LTE_RET_E longcheer_imei_get(void *pvData, char *pcBuf)
{
    LTE_IMEI_T *pstImei = (LTE_IMEI_T*)pvData;

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    if(NULL != strstr(pcBuf, "+LCTSN:"))
    {
        sscanf(pcBuf, "%*[^\"]\"%[^\"]", pstImei->acImei);
    }
    else
    {
        return LTE_FAIL;
    }
    
    LTE_LOG(LTE_LOG_INFO,"%d,%s: imei: %s",__LINE__,__func__, pstImei->acImei);
    
    return LTE_OK;

}
static LTE_RET_E longcheer_cellid_get(void *pvData, char *pcBuf)
{
    FILE  *pstFd = NULL;
    char acBuff[64] = {0};
    LTE_CELLID_T *pstCellId = (LTE_CELLID_T*)pvData;
    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    pstFd = fopen(LTE_LCTCELLINFO_FILE,"r");
    if (NULL == pstFd)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:open %s failed\n",__LINE__,__func__, LTE_LCTCELLINFO_FILE);
        return LTE_FAIL;
    }
    while (NULL != fgets(acBuff,64,pstFd))
    { 	
        if(NULL != strstr(acBuff, "CELL_ID:"))
        {
            sscanf(acBuff, "%*[^:]:%s", pstCellId->acCellID);
            fclose(pstFd);
            unlink(LTE_LCTCELLINFO_FILE);
            return LTE_OK;
        }
        
        memset(acBuff,0,64);
    }
    
    return LTE_FAIL;
    
}

static LTE_RET_E longcheer_iccid_get(void *pvData, char *pcBuf)
{

    return LTE_OK;
}

static LTE_RET_E longcheer_imsi_get(void *pvData, char *pcBuf)
{
    LTE_IMSI_T *pstImsiInfo = (LTE_IMSI_T *)pvData;

    if (pvData == NULL)
    {
		return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO,"%d,%s: sim imsi: %s",__LINE__,__func__, pcBuf);
    if(NULL != pcBuf)
    {
        sscanf(pcBuf, "%s", pstImsiInfo->acIMSI);
    }
    else
    {
        return LTE_FAIL;
    }
  
    LTE_LOG(LTE_LOG_INFO,"%d,%s: sim imsi: %s",__LINE__,__func__, pstImsiInfo->acIMSI);
    return LTE_OK;
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
static LTE_RET_E longcheer_plmn_register(void *pvData, int iDataLen)
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

static LTE_RET_E longcheer_prefer_plmn_set (void *pvData, int iDataLen)
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
static LTE_RET_E longcheer_linkstate_get(void *pvData, char *pcBuf)
{
    return LTE_OK;
}

/*****************************************************************************
 函 数 名  : pal_bsp_lte_connect
 功能描述  : 拨号连接
 输入参数  : void *pvData    
             int iDataLen  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年10月25日星期四
    作    者   : 陈键
    修改内容   : 新生成函数

*****************************************************************************/

#define DISCONNECT_TIME_OUT  5    //500ms*5
#define AUTH_TYPE 0


static LTE_RET_E longcheer_network_prepare(void *pvData, char *pcBuf)
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

//altered by caibing 2013-11-25
static LTE_RET_E longcheer_connect(void *pvData, char *pcBuf)
{
    return LTE_OK;
}

/*****************************************************************************
 函 数 名  : pal_bsp_lte_disconnect
 功能描述  : 断开网络连接
 输入参数  : void *pvData    
             int iDataLen  
 输出参数  : 无
 返 回 值  : LTE_RET_E
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年10月25日星期四
    作    者   : 陈键
    修改内容   : 新生成函数

*****************************************************************************/

/*altered by caibing 2013-11-25*/
static int longcheer_begin_disconnect(int iIPFamily)
{
    /*altered by caibing 2014-09-23*/

	return 0;
}

static LTE_RET_E longcheer_disconnect(void *pvData, char *pcBuf)
{
    return LTE_OK;
}

static LTE_RET_E longcheer_sim_state_get(void *pvData, char *pcBuf)
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


static LTE_RET_E longcheer_lte_info_get(void *pvData, char *pcBuf)
{
    return LTE_OK;
    
}

static LTE_RET_E longcheer_access_net_get(void *pvData, char *pcBuf)
{
    char acCiev[] = "+PSRAT:";

    LTE_ACCESS_NETWORK_T *pstAccessNet = (LTE_ACCESS_NETWORK_T*)pvData;

	if (pvData == NULL)
		return LTE_FAIL;
    
    if (NULL != strstr(pcBuf, acCiev))
    {
    	if (NULL != strstr(pcBuf, "LTE"))
    	{
    		pstAccessNet->enAccessNet = NET_4G;
    	}
      	else if (NULL != strstr(pcBuf, "NONE"))
    	{
    		pstAccessNet->enAccessNet = NET_NONE;
    	}
        else if((NULL != strstr(pcBuf, "GSM")) || (NULL != strstr(pcBuf, "EDGE"))||(NULL != strstr(pcBuf, "GPRS")))
        {
            pstAccessNet->enAccessNet = NET_2G;
        }
      	else
        {
    		pstAccessNet->enAccessNet = NET_3G;
	    }
        return LTE_OK;
	}

	return LTE_FAIL;
}

static LTE_RET_E longcheer_bands_get(void *pvData, char *pcBuf)
{

	return LTE_OK;
}


static LTE_RET_E longcheer_clck_bands_set(void *pvData, char *pcBuf)
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

static LTE_RET_E longcheer_clck_bands_close(void *pvData, char *pcBuf)
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

static LTE_RET_E longcheer_clckcell_enable_get(void *pvData, char *pcBuf)
{
    return LTE_OK;
}
static LTE_RET_E longcheer_clck_cellid_set(void *pvData, char *pcBuf)
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
/*****************************************************************************
   函 数 名  : longcheer_nettype_info_get
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
static LTE_RET_E longcheer_sup_nettype_get(void *pvData, char *pcBuf)
{
    
    return LTE_OK;

}
/*****************************************************************************
   函 数 名  : longcheer_oper_info_get
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
static LTE_RET_E longcheer_oper_info_get(void *pvData, char *pcBuf)
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
 函 数 名  : longcheer_sms_sup_func
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
static LTE_RET_E longcheer_sms_sup_func(void *pvData, char *pcBuf)
{
    LTE_SUPPORT_SMS_T *pstSup = (LTE_SUPPORT_SMS_T *)pvData;
    pstSup->enSupSms = SUP_SMS_FUNC;
    return LTE_OK;

}
static LTE_RET_E longcheer_cgdcont_set(void *pvData, char *pcBuf)
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
/*****************************************************************************
   函 数 名  : longcheer_apn_set
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
static LTE_RET_E longcheer_apn_set(void *pvData, char *pcBuf)
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
static LTE_RET_E longcheer_version_get(void *pvData, char *pcBuf)
{
    int iRet = 0;
    char acCiev[] = "L";    
    char acVersion[64] = {0};
    char *pcTmp = NULL;
    
    LTE_MODULE_VERSION_T *pstVersion = (LTE_MODULE_VERSION_T *)pvData;

    if(NULL == pvData)
    {
        return LTE_FAIL;
    }

    if(NULL != (pcTmp = strstr(pcBuf, acCiev)))
    {
        sscanf(pcTmp, "%s", acVersion);

        if(NULL != strstr(acVersion, LTE_VERSION_9105_FLAG))
        {
            pstVersion->iVersionIndex = LTE_LONCHEER_9105;
        }
        else if(NULL != strstr(acVersion, LTE_VERSION_9115_FLAG))
        {
            pstVersion->iVersionIndex = LTE_LONCHEER_9115;
        }
        else if(NULL != strstr(acVersion, LTE_VERSION_9106_FLAG))
        {
            pstVersion->iVersionIndex = LTE_LONCHEER_9106;
        }
        else
        {   
            pstVersion->iVersionIndex = LTE_VER_IND_MAX;
        }
        
        strncpy(pstVersion->acVersion, acVersion, sizeof(pstVersion->acVersion));
        
        return LTE_OK;
    }
    return LTE_FAIL;

}

static LTE_RET_E longcheer_enodeb_id_get(void *pvData, char *pcBuf)
{
    
    return LTE_OK;

}

/*****************************************************************************
 函 数 名  : longcheer_tac_get
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
static LTE_RET_E longcheer_tac_get(void *pvData, char *pcBuf)
{
    unsigned long ulTac;
    FILE  *pstFd = NULL;
    char acTacId[32] = {0};
    char acBuff[64] = {0};
    LTE_TAC_T *pstTac = (LTE_TAC_T *)pvData;
    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    pstFd = fopen(LTE_LCTCELLINFO_FILE,"r");
    if (NULL == pstFd)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:open %s failed\n",__LINE__,__func__, LTE_LCTCELLINFO_FILE);
        return LTE_FAIL;
    }
    while (NULL != fgets(acBuff,64,pstFd))
    { 	
        if(NULL != strstr(acBuff, "LAC_ID:"))
        {
            sscanf(acBuff, "%*[^:]:%s", acTacId);
            
            ulTac = strtoul(acTacId, NULL, 16);
            snprintf(pstTac->acTac, sizeof(pstTac->acTac),"%u", ulTac);
            fclose(pstFd);
            unlink(LTE_LCTCELLINFO_FILE);
            return LTE_OK;
        }
        
        memset(acBuff,0,64);
    }
    
    return LTE_FAIL;

}

static LTE_RET_E longcheer_lcid_get(void *pvData, char *pcBuf)
{
    
    return LTE_OK;

}

static LTE_RET_E longcheer_all_cellid_get(void *pvData, char *pcBuf)
{   
    unsigned long ulCellId;
    unsigned long ulTac;
    char acCellid[32] = {0};
    char acTacId[32] = {0};
    char *pcTmp = NULL;

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }

    LTE_ALL_CELLID_T *pstAllCid = (LTE_ALL_CELLID_T *)pvData;

    if(NULL != strstr(pcBuf, "OK"))
    {
       return LTE_OK;
    }
    else if(NULL != (pcTmp = strstr(pcBuf, "+CEREG:")))
    {
       sscanf(pcTmp,"%*[^\"]\"%*[^\"]\"%*[^\"]\"%*[^\"]\"%*[^\"]\"%[^\"]", acCellid);
       sscanf(pcTmp,"%*[^\"]\"%[^\"]", acTacId);
    }
    else
    {
       return LTE_FAIL;
    }

    ulCellId = strtoul(acCellid, NULL, 16);
    ulTac = strtoul(acTacId, NULL, 16);
    /*cellid*/  
    strncpy(pstAllCid->acCellId, acCellid, sizeof(pstAllCid->acCellId));
    /*共四个字节，取cid的前三个字节*/
    snprintf(pstAllCid->acEnodebId, sizeof(pstAllCid->acEnodebId),"%u", (ulCellId >> 8));
    /*共四个字节，取cid的第四个字节*/
    snprintf(pstAllCid->acLcid, sizeof(pstAllCid->acLcid),"%u", (ulCellId & LTE_LCID_MASK));
    snprintf(pstAllCid->acTac, sizeof(pstAllCid->acTac),"%u", ulTac);
    return LTE_OK;

}

static LTE_RET_E longcheer_pin_input_remain(void *pvData, char *pcBuf)
{
    char    acTacTmp[32] = {0};
    int     iRet = 0;
    char    acCiev[32] = "PIN1=";
    char    *pcBufTmp = NULL;

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_PIN_INPUT_REMAIN_T *pstPinInputRemain = (LTE_PIN_INPUT_REMAIN_T *)pvData;

    if(NULL != (pcBufTmp = strstr(pcBuf, acCiev)))
    {
        sscanf(pcBufTmp, "%*[^=]=%[^;]", acTacTmp);
    }
    else
    {
        return LTE_FAIL;
    }
    
    pstPinInputRemain->iPinIputRemain = atoi(acTacTmp);
    
    return LTE_OK;
}



LTE_MODULE_OPS_T stLte_longcheer_ops_t = {
    .lte_pin_state_get       = longcheer_pin_state,
    .lte_pin_switch          = longcheer_pin_switch,    
    .lte_pin_switch_get      = longcheer_pin_sw_get,
    .lte_pin_unlock          = longcheer_pin_unlock,
    .lte_pin_attempts        = NULL,
    .lte_pin_code_alter      = longcheer_pin_alter,
    .lte_signal_get          = longcheer_signal_get,
    .lte_power_on            = longcheer_power_on,
    .lte_power_off           = longcheer_power_off,
    .lte_power_state_get     = NULL,
    .lte_net_type_set        = longcheer_nettype_set,
    .lte_net_type_get        = longcheer_nettype_get,
    .lte_plmn_register       = NULL,
    .lte_plmn_prefer_set     = NULL,
    .lte_net_work_prepare    = longcheer_network_prepare,
    .lte_net_work_status_get = NULL,
    .lte_connect             = longcheer_connect,
    .lte_disconnect          = longcheer_disconnect,
    .lte_link_state_get      = longcheer_linkstate_get,
    .lte_imei_get            = longcheer_imei_get,
    .lte_cellid_get          = longcheer_cellid_get,
    .lte_iccid_get           = longcheer_iccid_get,
    .lte_imsi_get            = longcheer_imsi_get,
    .lte_sim_state_get       = longcheer_sim_state_get,
    .lte_info_get            = longcheer_lte_info_get,
    .lte_access_net_get      = longcheer_access_net_get,
    .lte_bands_get           = longcheer_bands_get,
    .lte_clck_bands          = longcheer_clck_bands_set,
    .lte_clck_cell_get       = longcheer_clckcell_enable_get,
    .lte_clck_cell           = longcheer_clck_cellid_set,
    .lte_gps_start           = NULL, 
    .lte_gps_stop            = NULL,
    .lte_oper_info_get       = longcheer_oper_info_get,
    .lte_nettype_info_get    = longcheer_sup_nettype_get,
    .lte_sms_cent_code_set   = NULL,
    .lte_sms_mode_set        = NULL,
    .lte_sms_func_init       = NULL,
    .lte_sms_recv            = NULL,
    .lte_sms_send            = NULL,
    .lte_sms_del             = NULL,
    .lte_sms_supinfo_get     = longcheer_sms_sup_func,
    .lte_sms_ip_stack_set    = longcheer_cgdcont_set,
    .lte_apn_get             = NULL,
    .lte_apn_set             = longcheer_apn_set,
    .lte_version_get         = longcheer_version_get,
    .lte_enodebid_get        = longcheer_enodeb_id_get,
    .lte_tac_get             = longcheer_tac_get,
    .lte_lcid_get            = longcheer_lcid_get,
    .lte_all_cellid_get      = longcheer_all_cellid_get,
    .lte_sim_unlock          = NULL,
    .lte_sim_disable_unlock  = NULL,
    .lte_sim_type_get        = NULL,
    .lte_ip_gw_dns_get       = NULL,
    .lte_all_plmn_mode_get   = NULL,
    .lte_sounet_mode_set     = NULL,
    .lte_sim_puk_unlock		 = NULL,
    .lte_pin_input_remain    = longcheer_pin_input_remain,

};

