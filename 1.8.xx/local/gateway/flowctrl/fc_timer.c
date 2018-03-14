/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : fc_sta.c
  版 本 号   : 初稿
  作    者   : Zhong Huaqing
  生成日期   : 2012年8月10日 星期五
  最近修改   :
  功能描述   : 流控相关的定时器操作
  函数列表   :
  修改历史   :
  1.日    期   : 2012年8月10日 星期五
    作    者   : Zhong Huaqing
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include <linux/timer.h>

#include "fc_main.h"
#include "fc_queue.h"

#include "fc_timer.h"
/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/
void fc_tx_timer_handle(ULONG uiData);
void fc_aging_timer_handle(ULONG uiData);
void fc_auto_rate_timer_handle(ULONG uiData);


/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/
//UINT32 g_uiAgingTime = FC_AGINE_TIME_DEF_VALUE;

LOCAL struct timer_list g_stAutoRateTimer;
 


/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 函数定义                                       *
 *----------------------------------------------*/


LOCAL AP_ERROR_CODE_E fc_timer_init(
	struct timer_list *pstTimer,
	void (*function)(ULONG),
	ULONG uiData
	)
{
	init_timer(pstTimer);
	pstTimer->function = function;
	pstTimer->data = uiData; 
	
	return AP_E_NONE;
}

AP_ERROR_CODE_E fc_queue_timer_init(FC_QUEUE_T *pstQ)
{
    FC_LOG(FC_LOG_DEBUG, "[QID %d]%s: Init TX timer.\n", pstQ->uiQId,
        (FC_DIR_US == pstQ->enDir)? "US" : "DS");
    fc_timer_init(&pstQ->stTxIimer, fc_tx_timer_handle, (ULONG)pstQ);
    fc_timer_init(&pstQ->stAgingTimer, fc_aging_timer_handle, (ULONG)pstQ);
    return AP_E_NONE;
}

AP_ERROR_CODE_E fc_queue_tx_timer_restart(FC_QUEUE_T *pstQ, UINT32 uiTickDelay)
{
    if (NULL == pstQ)
    {
        return AP_E_PARAM;
    }
    
    if (0 == pstQ->uiBytePerTick) /*不需要启动定时器*/
    {
        return AP_E_NONE;
    }
    
    mod_timer(&pstQ->stTxIimer, jiffies + uiTickDelay);
    return AP_E_NONE;
}

AP_ERROR_CODE_E fc_queue_aging_timer_restart(FC_QUEUE_T *pstQ, UINT32 uiTickDelay)
{
    if (NULL == pstQ)
    {
        return AP_E_PARAM;
    }
    
    mod_timer(&pstQ->stAgingTimer, jiffies + uiTickDelay);
    return AP_E_NONE;
}

AP_ERROR_CODE_E fc_queue_tx_timer_stop(FC_QUEUE_T *pstQ)
{
    if (NULL == pstQ)
    {
        return AP_E_PARAM;
    }
    
    del_timer(&pstQ->stTxIimer);
    return AP_E_NONE;
}

AP_ERROR_CODE_E fc_queue_aging_timer_stop(FC_QUEUE_T *pstQ)
{
    if (NULL == pstQ)
    {
        return AP_E_PARAM;
    }
    
    del_timer(&pstQ->stAgingTimer);
    return AP_E_NONE;
}

AP_ERROR_CODE_E fc_queue_timer_stop(FC_QUEUE_T *pstQ)
{
    if (NULL == pstQ)
    {
        return AP_E_PARAM;
    }
    
    del_timer(&pstQ->stTxIimer);
    del_timer(&pstQ->stAgingTimer);
    return AP_E_NONE;
}


AP_ERROR_CODE_E fc_auto_rate_timer_init(void)
{
    fc_timer_init(&g_stAutoRateTimer, fc_auto_rate_timer_handle, 0);
    return AP_E_NONE;
}

AP_ERROR_CODE_E fc_auto_rate_timer_restart(UINT32 uiTickDelay)
{
    mod_timer(&g_stAutoRateTimer, jiffies + uiTickDelay);
    return AP_E_NONE;
}

AP_ERROR_CODE_E fc_auto_rate_timer_stop(void)
{
    del_timer(&g_stAutoRateTimer);
    return AP_E_NONE;
}
/*****************************************************************************
 函 数 名  : fc_watchdog
 功能描述  : 队列发送定时器处理函数
 输入参数  : UINT32 data  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月11日 星期六
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
void fc_tx_timer_handle(ULONG uiData)
{
    FC_QUEUE_T *pstQ = (FC_QUEUE_T *)uiData;
    UINT32 uiSendTotal = 0;
    //UINT32 uiDelayExtra;
    
    #if 0
    /*For test.*/
    fc_queue_tx_timer_restart(pstQ, pstQ->uiTxDelay);
    return;
    #endif
    
    uiSendTotal =fc_dequeue(pstQ);
    
    if (uiSendTotal > pstQ->uiBurstSize)
    {
        FC_LOG(FC_LOG_DEBUG, "[%d](%d): tx timer send %d bytes.\n",
            pstQ->uiQId, pstQ->enDir,uiSendTotal);
    }
    
    #if 0
    if (0 == uiSendTotal)
    {
        FC_LOG(FC_LOG_DEBUG, "[%d](%d): tx timer empty running!\n",
            pstQ->uiQId, pstQ->enDir);
    }
    #endif
    
    if (skb_queue_len(&pstQ->stSkbList) > 0)
    {
        /*缓冲区中依然存在报文*/
        #if 0
        uiDelayExtra = (uiSendTotal >> 16);  /*计算方法?*/
        if (uiDelayExtra > 0)
        {
            FC_LOG(FC_LOG_DEBUG, "Timer send %d bytes.\n", uiSendTotal);
            pstQ->uiTxDelay = uiDelayExtra;
        }
        else
        {
            pstQ->uiTxDelay = FC_WATCHDOG_DELAY;
        }
        #endif
        fc_queue_tx_timer_restart(pstQ, pstQ->uiTxDelay);
    }
    else
    {
        /*标识定时器已经停止*/
        FC_THROTTLE_FLAG_CLEAR(pstQ);
    }
    
    return;
}

/*****************************************************************************
 函 数 名  : fc_aging_timer_handle
 功能描述  : 队列老化机制，执行周期默认为10秒
 输入参数  : UINT32 uiData  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月12日 星期日
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
void fc_aging_timer_handle(ULONG uiData)
{
    FC_QUEUE_T *pstQ = (FC_QUEUE_T *)uiData;
    
    
    if (pstQ->stStatsLast10Sec.uiByteTotal == pstQ->stStatsCur.uiByteTotal)
    {
        /*10秒钟内无流量,则删除对应的队列*/
        FC_LOG(FC_LOG_INFO, "Destroy queue(ID %d)\n", pstQ->uiQId);
        fc_queue_destroy(pstQ);
    }
    else
    {
        /*更新统计记录*/
        pstQ->stStatsLast10Sec = pstQ->stStatsCur;
        fc_queue_aging_timer_restart(pstQ, pstQ->uiAgingDelay);  /*默认每10秒运行一次*/
    }
    
    return;
}

/*不需要区分当前规则。执行周期:1秒*/
void fc_auto_rate_timer_handle(ULONG uiData)
{
    FC_DIR_E enDir;
    
    for (enDir = FC_DIR_DS; enDir < FC_DIR_MAX; enDir++)
    {
        fc_queue_auto_rate(enDir);
    }
    
    fc_auto_rate_timer_restart(HZ);
    
    return;
}


