/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : fc_sta.c
  �� �� ��   : ����
  ��    ��   : Zhong Huaqing
  ��������   : 2012��8��10�� ������
  ����޸�   :
  ��������   : ������صĶ�ʱ������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��8��10�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include <linux/timer.h>

#include "fc_main.h"
#include "fc_queue.h"

#include "fc_timer.h"
/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/
void fc_tx_timer_handle(ULONG uiData);
void fc_aging_timer_handle(ULONG uiData);
void fc_auto_rate_timer_handle(ULONG uiData);


/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/
//UINT32 g_uiAgingTime = FC_AGINE_TIME_DEF_VALUE;

LOCAL struct timer_list g_stAutoRateTimer;
 


/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                       *
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
    
    if (0 == pstQ->uiBytePerTick) /*����Ҫ������ʱ��*/
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
 �� �� ��  : fc_watchdog
 ��������  : ���з��Ͷ�ʱ��������
 �������  : UINT32 data  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��11�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

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
        /*����������Ȼ���ڱ���*/
        #if 0
        uiDelayExtra = (uiSendTotal >> 16);  /*���㷽��?*/
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
        /*��ʶ��ʱ���Ѿ�ֹͣ*/
        FC_THROTTLE_FLAG_CLEAR(pstQ);
    }
    
    return;
}

/*****************************************************************************
 �� �� ��  : fc_aging_timer_handle
 ��������  : �����ϻ����ƣ�ִ������Ĭ��Ϊ10��
 �������  : UINT32 uiData  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��12�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

*****************************************************************************/
void fc_aging_timer_handle(ULONG uiData)
{
    FC_QUEUE_T *pstQ = (FC_QUEUE_T *)uiData;
    
    
    if (pstQ->stStatsLast10Sec.uiByteTotal == pstQ->stStatsCur.uiByteTotal)
    {
        /*10������������,��ɾ����Ӧ�Ķ���*/
        FC_LOG(FC_LOG_INFO, "Destroy queue(ID %d)\n", pstQ->uiQId);
        fc_queue_destroy(pstQ);
    }
    else
    {
        /*����ͳ�Ƽ�¼*/
        pstQ->stStatsLast10Sec = pstQ->stStatsCur;
        fc_queue_aging_timer_restart(pstQ, pstQ->uiAgingDelay);  /*Ĭ��ÿ10������һ��*/
    }
    
    return;
}

/*����Ҫ���ֵ�ǰ����ִ������:1��*/
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


