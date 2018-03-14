/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : fc_queue.c
  版 本 号   : 初稿
  作    者   : Zhong Huaqing
  生成日期   : 2012年8月10日 星期五
  最近修改   :
  功能描述   : 流控相关的hash操作
  函数列表   :
  修改历史   :
  1.日    期   : 2012年8月10日 星期五
    作    者   : Zhong Huaqing
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "fc_main.h"
#include "fc_timer.h"
#include "fc_hash.h"

#include "fc_queue.h"
#include "fc_ssid.h"


/*----------------------------------------------*
 * 宏定义                                 *
 *----------------------------------------------*/
//#define FC_KEEP_SEQ  1    /*用于保持原先报文的顺序*/
 
/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/ 

#if (defined(GW_X86) || defined(GW_ARM) )
    REMOTE rx_handler_result_t (*br_handle_frame_hook)(struct sk_buff **pskb);
#elif defined OPENWRT
    extern rx_handler_result_t br_handle_frame(struct sk_buff **pskb);
#else
    #if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20))    
        REMOTE struct sk_buff *(*br_handle_frame_hook)(struct net_bridge_port *p,
        					struct sk_buff *skb);
    #else
        REMOTE int (*br_handle_frame_hook)(struct net_bridge_port *p, 
                            struct sk_buff **pskb);
    #endif
#endif
/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
REMOTE int br_dev_queue_push_xmit(struct sk_buff *skb);

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
int g_bKeepSequence = TRUE;
module_param(g_bKeepSequence, int, 0600);

#if 0
int g_bKeepSeqUS = FALSE;
int g_bKeepSeqDS = FALSE;
module_param(g_bKeepSeqUS, int, 0600);
module_param(g_bKeepSeqDS, int, 0600);
#endif

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/
FC_QUEUE_T g_aastQueue[FC_DIR_MAX][FC_QUEUE_NUM_MAX]; /*上下行队列*/

INT32 g_auiBytePerTick[FC_DIR_MAX];

#if (defined(GW_X86) || defined(GW_ARM))

LOCAL DEFINE_RWLOCK(g_stQLock);

#else
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20))    
rwlock_t g_stQLock = RW_LOCK_UNLOCKED;  /*实际上是自旋锁 + bh控制*/
#else
LOCAL DEFINE_RWLOCK(g_stQLock);
#endif

#endif
/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 函数定义                                       *
 *----------------------------------------------*/

FC_QUEUE_T *fc_get_queue_array_by_dir(FC_DIR_E enDir)
{
    return g_aastQueue[enDir];
}
/*****************************************************************************
 函 数 名  : fc_find_avail_sta_queue
 功能描述  : 获取空闲的队列
 输入参数  : FC_DIR_E enDir  --流控方向
 输出参数  : 无
 返 回 值  : FC_QUEUE_T
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月11日 星期六
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
FC_QUEUE_T *fc_find_avail_queue(FC_DIR_E enDir)
{
    FC_QUEUE_T *pstPos;
    UINT32 uiIndex;
    
    pstPos = fc_get_queue_array_by_dir(enDir);
    if (NULL == pstPos)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue.\n");
        return NULL;
    }
    
    FC_QUEUE_WR_LOCK;
    for (uiIndex = 0; uiIndex < FC_QUEUE_NUM_MAX; uiIndex++)
    {
        if (!pstPos->bInUse) /*队列空闲*/
        {
            memset(pstPos, 0, sizeof(FC_QUEUE_T));
            pstPos->bInUse = TRUE;
            pstPos->uiQId = uiIndex;
            pstPos->iPQId = FC_QID_INVALID;
            //FC_LOG(FC_LOG_INFO, "dir %d: uiIndex=%d is Available.\n", enDir, uiIndex);
            
            FC_QUEUE_WR_UNLOCK;
            return pstPos;
        }
        else
        {
            //FC_LOG(FC_LOG_DEBUG, "dir %d: uiIndex=%d is In use.\n", enDir, uiIndex);
        }
        pstPos++;
    }
    FC_QUEUE_WR_UNLOCK;
    
    return NULL;
}

/*****************************************************************************
 函 数 名  : fc_get_ap_queue
 功能描述  : 为AP整机流控获取队列
 输入参数  : FC_DIR_E enDir  
 输出参数  : 无
 返 回 值  : FC_QUEUE_T
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月13日 星期一
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
FC_QUEUE_T *fc_get_ap_queue(FC_DIR_E enDir)
{
    FC_QUEUE_T *pstPos;
    
    pstPos = fc_get_queue_array_by_dir(enDir);
    return pstPos;
}

/*****************************************************************************
 函 数 名  : fc_get_parent_queue
 功能描述  : 获取指定队列的上级队列
 输入参数  : FC_QUEUE_T *pstQ  
 输出参数  : 无
 返 回 值  : FC_QUEUE_T
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月19日 星期三
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
FC_QUEUE_T *fc_get_parent_queue(FC_QUEUE_T *pstQ)
{
    FC_RULE_E enRule;
    CHAR acVapName[IFNAMSIZ];
	CHAR acVlanDev[IFNAMSIZ];
    struct net_device * pstDev;
    FC_QUEUE_T *pstPQ;
    
    if (NULL == pstQ)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue.\n");
        return NULL;
    }
    
    enRule = fc_get_rule();
    if (!FC_RULE_IS_MULTI(enRule)
        && !FC_RULE_IS_AUTO(enRule))
    {
        return NULL;
    }
    
    /*只有终端模式才有对应的父队列*/
    if (FC_RULE_STA != pstQ->enRule)
    {
        return NULL;
    }
    
    
    if (FC_RULE_AP & enRule)
    {
        return fc_get_ap_queue(pstQ->enDir);
    }
    
    snprintf(acVapName, IFNAMSIZ, "ap%u_%u", 
            pstQ->stMatchAttrib.iRadioId,
            pstQ->stMatchAttrib.iWlanId
            );
    
    if (FC_RULE_VAP & enRule)
    {
       return fc_get_vap_queue_from_htable(acVapName, pstQ->enDir);
    }

	snprintf(acVlanDev, IFNAMSIZ, "eth1.%d", pstQ->stMatchAttrib.iVid);
	if (FC_RULE_VLAN & enRule)
    {
       return fc_get_vlan_queue_from_htable(acVlanDev, pstQ->enDir);
    }

#if (!defined(GW_X86) && !defined(GW_ARM) && !defined(OPENWRT))     
    if (FC_RULE_SSID & enRule)
    {
        pstDev = fc_get_dev_by_name(acVapName);
        if (NULL == pstDev)
        {
            return NULL;
        }

        if (NULL == pstDev->br_port)
        {
            dev_put(pstDev);
            return NULL;
        }
        
        pstPQ = fc_get_ssid_queue_from_htable(pstDev->br_port->acSsid, pstQ->enDir);

        dev_put(pstDev);
        return pstPQ;
    }
#endif

    return NULL;
}


AP_ERROR_CODE_E fc_queue_get_auto_learn_rate(
    FC_DIR_E enDir,
    CHAR *acDevName,
    FC_QUEUE_RATE_T *astRate)
{
    FC_RULE_E enRule;
    struct net_device * pstDev;
    FC_QUEUE_T *pstPQ = NULL;
    
    if (NULL == acDevName || NULL == astRate)
    {
        return AP_E_PARAM;
    }
    
    enRule = fc_get_rule();
    
    if (FC_RULE_AP & enRule)
    {
        pstPQ = fc_get_ap_queue(enDir);
    }
        
    if (FC_RULE_VAP & enRule)
    {
       pstPQ = fc_get_vap_queue_from_htable(acDevName, enDir);
    }

	if (FC_RULE_VLAN & enRule)
    {
       pstPQ = fc_get_vlan_queue_from_htable(acDevName, enDir);
    }

#if (!defined(GW_X86) && !defined(GW_ARM) && !defined(OPENWRT))
    if (FC_RULE_SSID & enRule)
    {
        pstDev = fc_get_dev_by_name(acDevName);
        if (NULL == pstDev)
        {
            return AP_E_RESOURCE;
        }
        
        if (NULL == pstDev->br_port)
        {
            dev_put(pstDev);
            return AP_E_NULLPTR;
        }
        
        pstPQ = fc_get_ssid_queue_from_htable(pstDev->br_port->acSsid, enDir);
        dev_put(pstDev);
    }
#endif

/* BEGIN: Modified by xucongjiang, 2012/11/9 解决自动限速时，删除父结点后仍能自动学习的问题 */
    if (NULL == pstPQ || !pstPQ->bInUse )
    {
        return AP_E_RESOURCE;
    }
/* END:   Modified by xucongjiang, 2012/11/9   PN: */
    /*获取总体的配置速率*/
    astRate->uiRate = TICKBYTE_TO_RATE(pstPQ->uiAdminBytePerTick);
    astRate->uiBurst = pstPQ->uiAdminBurstSize;
    
    return AP_E_NONE;
}

/*****************************************************************************
 函 数 名  : fc_queue_gear_rate
 功能描述  : 调整队列的速率
 输入参数  : FC_QUEUE_T *pstQ  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月19日 星期三
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E fc_queue_gear_rate(FC_QUEUE_T *pstQ)
{
    UINT32 uiGearByte;
    //UINT32 uiReal;
    
    if (NULL == pstQ)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue.\n");
        return AP_E_PARAM;
    }
    
    if (FC_RULE_STA == pstQ->enRule) /*不做任何调整*/
    {
        return AP_E_NONE;
    }
    
    uiGearByte = fc_get_gear_byte();
    if (uiGearByte >= FC_MTU)
    {
        return AP_E_NONE;
    }
    
    if (0 == uiGearByte) /*恢复至配置值*/
    {
        pstQ->uiBytePerTick = pstQ->uiAdminBytePerTick;
        pstQ->uiBurstSize = pstQ->uiAdminBurstSize;
    }
    else
    {
        pstQ->uiBytePerTick = (pstQ->uiAdminBytePerTick * FC_MTU) / (FC_MTU - uiGearByte);
        pstQ->uiBurstSize = RATE_TO_BURST(TICKBYTE_TO_RATE(pstQ->uiBytePerTick));
    }
    pstQ->uiTxDelay = RATE_TO_DELAY(TICKBYTE_TO_RATE(pstQ->uiBytePerTick));
    
    FC_LOG(FC_LOG_INFO, "Qid %d, rule %d, uiBytePerTick %d, uiTxDelay %d\n",
            pstQ->uiQId,
            pstQ->enRule,
            pstQ->uiBytePerTick,
            pstQ->uiTxDelay
            );
    
    return AP_E_NONE;
}

/*****************************************************************************
 函 数 名  : fc_queue_all_gear_rate
 功能描述  : 调整所有队列的速率
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月20日 星期四
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E fc_queue_all_gear_rate(void)
{
    FC_QUEUE_T *pstQArray;
    FC_QUEUE_T *pstQ;
    
    FC_DIR_E enDir;
    UINT32 uiIndex;
    AP_ERROR_CODE_E enRet;
         
    
    for (enDir = FC_DIR_DS; enDir < FC_DIR_MAX; enDir++)
    {    
        pstQArray = fc_get_queue_array_by_dir(enDir);
        if (NULL == pstQArray)
        {
            return AP_E_RESOURCE;
        }
        
        for (uiIndex = 0; uiIndex < FC_QUEUE_NUM_MAX; uiIndex++)
        {
            pstQ = &pstQArray[uiIndex];
            
            if (!pstQ->bInUse)
            {
                continue;
            }
            
            enRet = fc_queue_gear_rate(pstQ);
            if (AP_E_NONE != enRet)
            {
                return enRet;
            }
        }
    }
    
    return AP_E_NONE;    
}
/*****************************************************************************
 函 数 名  : fc_queue_update
 功能描述  : 更新队列配置信息
 输入参数  : FC_QUEUE_T *pstStaQ          --队列结构
             FC_RATE_T *pstRate           --速率结构
             FC_QUEUE_OPER_TYPE_T enOper  --操作类型:队列创建或者更新
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月11日 星期六
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E fc_queue_update(
    FC_QUEUE_T *pstQ, 
    FC_QUEUE_RATE_T *pstRate,
    FC_QUEUE_OPER_TYPE_T enOper)
{
    if (NULL == pstQ || NULL == pstRate)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue or Rate.\n");
        return AP_E_PARAM;
    }
    
        
    pstQ->uiAdminBytePerTick = RATE_TO_TICKBYTE(pstRate->uiRate);
    pstQ->uiAdminBurstSize = FC_MAX(pstRate->uiBurst, FC_MTU);
    
    /*默认情况下，真实速率与配置速率相同*/
    pstQ->uiBytePerTick = pstQ->uiAdminBytePerTick;
    pstQ->uiBurstSize = pstQ->uiAdminBurstSize;
    pstQ->uiTxDelay = RATE_TO_DELAY(pstRate->uiRate);
    
    fc_queue_gear_rate(pstQ); /*调整实际运行的速率*/
    
    pstQ->uiAgingDelay = FC_AGINE_TIME_DEF_VALUE;
    
    FC_LOG(FC_LOG_INFO, "QID = %d, Oper %d, Dir %d: rate %d, burst %d, delay %d, HZ %d\n", 
        pstQ->uiQId,
        enOper, pstQ->enDir,
        pstRate->uiRate, 
        pstRate->uiBurst,
        pstQ->uiTxDelay,
        HZ);
    
    if (FC_QUEUE_CREATE == enOper)
    {
        /*速率控制相关*/
        pstQ->uiTimeCheckPoint = jiffies;
        pstQ->uiByteAvail = pstQ->uiBurstSize;
        //pstQ->uiTxDelay = FC_WATCHDOG_DELAY;
        
        /*设置队列初始缓冲区大小*/
        pstQ->uiCurCcache = FC_MAX_CCACHE;
        pstQ->uiMinCcache = FC_MAX_CCACHE;
    }
    
    return AP_E_NONE;
}

AP_ERROR_CODE_E fc_update_child_queue_set(FC_QUEUE_T *pstPQ)
{
    FC_QUEUE_T *pstQArray;
    FC_QUEUE_T *pstQ;
    FC_QUEUE_T *pstPQTmp;
    UINT32 uiIndex;
    

    FC_RULE_E enRule;
    
    if (NULL == pstPQ)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue.\n");
        return AP_E_PARAM;
    }
    
    enRule = fc_get_rule();
    if (!FC_RULE_IS_MULTI(enRule))
    {
        return AP_E_NONE;
    }

    
    pstQArray = fc_get_queue_array_by_dir(pstPQ->enDir);
    
    for (uiIndex = 0; uiIndex < FC_QUEUE_NUM_MAX; uiIndex++)
    {
        pstQ = &pstQArray[uiIndex];
            
        if (!pstQ->bInUse) /*队列未被使用*/
        {
            continue;
        }
        
        if (FC_QID_INVALID != pstQ->iPQId) /*STA已经归属于某个parent 队列*/
        {
            continue;
        }
        if (FC_RULE_STA != pstQ->enRule) /*只有STA队列才能作为子队列*/
        {
            continue;
        }
        
        pstPQTmp = fc_get_parent_queue(pstQ);
        if (NULL == pstPQTmp)
        {
            continue;
        }
		
        FC_LOG(FC_LOG_INFO, "sta qid %d, pqid %d\n", pstQ->uiQId, pstPQTmp->uiQId);
        if (pstPQTmp->uiQId == pstPQ->uiQId) /*匹配成功*/
        {
            FC_QUEUE_WR_LOCK;
            pstQ->iPQId = pstPQ->uiQId; /*修改PQid*/
            list_add_tail(&pstQ->stStaSet, &pstPQ->stStaSet);
            pstQ->stMatchAttrib.iVid = pstPQ->stMatchAttrib.iVid;
            FC_QUEUE_WR_UNLOCK;
        }
        
    }
    
    return AP_E_NONE;
}

/*****************************************************************************
 函 数 名  : fc_adjust_child_queue_rate
 功能描述  : 调整子队列的速率
 输入参数  : FC_QUEUE_T *pstPQ  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月13日 星期四
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E fc_adjust_child_queue_rate(FC_QUEUE_T *pstPQ)
{
    FC_QUEUE_T *pstQTmp;
    UINT32 uiStaTotalRate = 0;
    UINT32 uiStaNum = 0;
    UINT32 uiParticle;
    
    UINT32 uiBytePerTick;
    UINT32 uiRate;
    
    FC_RULE_E enRule;
    
    if (NULL == pstPQ)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue.\n");
        return AP_E_PARAM;
    }
    
    enRule = fc_get_rule();
    if (!FC_RULE_IS_MULTI(enRule))
    {
        return AP_E_NONE;
    }

    
    /*计算子队列总的速率和数量*/
    list_for_each_entry(pstQTmp, &pstPQ->stStaSet, stStaSet)
    {
        uiStaTotalRate += pstQTmp->uiAdminBytePerTick;
        uiStaNum++;
        FC_LOG(FC_LOG_INFO, "Qid %d, rate %d\n",
                pstQTmp->uiQId,
                TICKBYTE_TO_RATE(pstQTmp->uiAdminBytePerTick)
                );
    }
    
    if (0 == uiStaNum || 0 == uiStaTotalRate) /* 2012-11-7 mod by zhangping */
    {
        return AP_E_NONE;
    }
    
    FC_LOG(FC_LOG_INFO, "PQid %d, rate %d, burst %d, stanum %d\n",
            pstPQ->uiQId,
            TICKBYTE_TO_RATE(pstPQ->uiAdminBytePerTick),
            pstPQ->uiBurstSize,
            uiStaNum
            );
    
    /*计算等比速率中，每一份的数值*/
    uiParticle = (pstPQ->uiAdminBytePerTick * 100) / uiStaTotalRate;
    /*设置运行时的速率*/
    list_for_each_entry(pstQTmp, &pstPQ->stStaSet, stStaSet)
    {
        uiBytePerTick = (uiParticle * pstQTmp->uiAdminBytePerTick) / 100;
        
        FC_QUEUE_WR_LOCK;
        if (uiBytePerTick < pstQTmp->uiAdminBytePerTick)
        {
            pstQTmp->uiBytePerTick = uiBytePerTick;
            uiRate = TICKBYTE_TO_RATE(pstQTmp->uiBytePerTick);
            pstQTmp->uiBurstSize = RATE_TO_BURST(uiRate);
        }
        else /*高一级的速率足够大，因此恢复至配置速率*/
        {
            pstQTmp->uiBytePerTick = pstQTmp->uiAdminBytePerTick;
            pstQTmp->uiBurstSize = pstQTmp->uiAdminBurstSize;
        }
        FC_QUEUE_WR_UNLOCK;
    }
    
    return AP_E_NONE;
}

/*****************************************************************************
 函 数 名  : fc_queue_recover_rate
 功能描述  : 将子队列速率恢复为配置值
 输入参数  : FC_QUEUE_T *pstPQ  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月13日 星期四
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E fc_queue_recover_rate(FC_QUEUE_T *pstPQ)
{
    FC_QUEUE_T *pstQTmp;
    
    if (NULL == pstPQ)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue.\n");
        return AP_E_PARAM;
    }
    
    FC_QUEUE_WR_LOCK;
    list_for_each_entry(pstQTmp, &pstPQ->stStaSet, stStaSet)
    {
        pstQTmp->uiBytePerTick = pstQTmp->uiAdminBytePerTick;
        pstQTmp->uiBurstSize = pstQTmp->uiAdminBurstSize;
        
        pstQTmp->iPQId = FC_QID_INVALID;
        //INIT_LIST_HEAD(&pstQTmp->stStaSet);
    }
    FC_QUEUE_WR_UNLOCK;
    
    return AP_E_NONE;
}

/*****************************************************************************
 函 数 名  : fc_queue_skb_list_init
 功能描述  : 初始化队列报文缓存区列表
 输入参数  : FC_QUEUE_T *pstStaQ  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月11日 星期六
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E fc_queue_skb_list_init(FC_QUEUE_T *pstQ)
{
    if (NULL == pstQ)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue.\n");
        return AP_E_PARAM;
    }
    
    skb_queue_head_init(&pstQ->stSkbList);
    return AP_E_NONE;
}

/*****************************************************************************
 函 数 名  : fc_queue_skb_list_destroy
 功能描述  : 销毁队列缓存区内容
 输入参数  : FC_QUEUE_T *pstStaQ  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月11日 星期六
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E fc_queue_skb_list_destroy(FC_QUEUE_T *pstQ)
{
    struct sk_buff* pstSkb = NULL;	
    struct sk_buff* pstSkbTmp = NULL;
    
    if (NULL == pstQ)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue.\n");
        return AP_E_PARAM;
    }
    
    list_for_each_safe(pstSkb, pstSkbTmp, (struct sk_buff*)&pstQ->stSkbList)
    {
        kfree_skb(pstSkb);
    }
    
    return AP_E_NONE;
}

/*****************************************************************************
 函 数 名  : fc_queue_init
 功能描述  : 队列初始化
 输入参数  : FC_QUEUE_T *pstStaQ  
             FC_DIR_E enDir       
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月11日 星期六
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E fc_queue_init(FC_QUEUE_T *pstQ, FC_DIR_E enDir)
{
    if (NULL == pstQ)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue.\n");
        return AP_E_PARAM;
    }
    
    /*设置基本属性*/
    pstQ->bInUse = TRUE;
    //pstQ->enRule = fc_get_rule();
    pstQ->enDir = enDir;
    
    pstQ->stMatchAttrib.iRadioId = FC_RID_INVALID;
    pstQ->stMatchAttrib.iWlanId = FC_WID_INVALID;
	pstQ->stMatchAttrib.iVid = 0;
    
    INIT_LIST_HEAD(&pstQ->stStaSet);
    
    fc_queue_timer_init(pstQ);
    fc_queue_skb_list_init(pstQ);
    
    if (fc_get_auto_rate_status())
    {
        fc_queue_aging_timer_restart(pstQ, pstQ->uiAgingDelay);
    }
    return AP_E_NONE;
}

/*****************************************************************************
 函 数 名  : fc_queue_destroy
 功能描述  : 队列销毁工作
 输入参数  : FC_QUEUE_T *pstStaQ  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月11日 星期六
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E fc_queue_destroy(FC_QUEUE_T *pstQ)
{    
    if (NULL == pstQ)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue.\n");
        return AP_E_PARAM;
    }
    
    FC_LOG(FC_LOG_INFO, "Destroy queue(ID %d).\n", pstQ->uiQId);
    
    
    pstQ->bInUse = FALSE;
    
    
    /*将队列从哈希表中删除*/
    if (FC_RULE_STA == pstQ->enRule)
    {
        fc_del_sta_queue_from_htable(pstQ);
    }
    else if (FC_RULE_VAP == pstQ->enRule)
    {
        fc_del_vap_queue_from_htable(pstQ);
    }
    else if (FC_RULE_SSID == pstQ->enRule)
    {
        fc_del_ssid_queue_from_htable(pstQ);
    }
    
    fc_queue_timer_stop(pstQ);
    fc_queue_skb_list_destroy(pstQ);
    //INIT_LIST_HEAD(&pstQ->stStaSet); /*；链表结点初始化*/

    /* BEGIN: Added by xucongjiang, 2012/11/9 */
    /* 解决删除队列后，原先的数据还存在的问题 */
    memset(pstQ, 0, sizeof(FC_QUEUE_T));
    /* END:   Added by xucongjiang, 2012/11/9   PN: */

    return AP_E_NONE;
}

AP_ERROR_CODE_E fc_queue_flush_all(void)
{
    FC_DIR_E enDir;
    UINT32 uiIndex;
    FC_QUEUE_T *pstQ;
    AP_ERROR_CODE_E enRet;
    
    for (enDir = FC_DIR_DS; enDir < FC_DIR_MAX; enDir++)
    {
        pstQ = fc_get_queue_array_by_dir(enDir);
        if (NULL == pstQ)
        {
            FC_LOG(FC_LOG_ERR, "Invalid Queue.\n");
            continue;
        }
        
        for (uiIndex = 0; uiIndex < FC_QUEUE_NUM_MAX; uiIndex++)
        {
            if (pstQ->bInUse)
            {
                 enRet = fc_queue_destroy(pstQ);
                if (AP_E_NONE != enRet)
                {
                    FC_LOG(FC_LOG_INFO, "Destroy queue(%d,%d) failed. ret = %d\n",
                            enDir, uiIndex, enRet);
                    return AP_E_DEFAULT_FAIL;
                }
            }
            pstQ++;  /*OK*/
        }
    }
    
    return AP_E_NONE;
}

/*****************************************************************************
 函 数 名  : fc_queue_update_token
 功能描述  : 将速率转换成一定时间内可发送的字节数
 输入参数  : FC_QUEUE_T *pstStaQ  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月11日 星期六
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E fc_queue_update_token(FC_QUEUE_T *pstQ)
{
    UINT32 uiNow;
	UINT32 uiTimeDiff;
	
	if (NULL == pstQ)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue.\n");
        return AP_E_PARAM;
    }
    
	uiNow = jiffies;
	uiTimeDiff = (uiNow - pstQ->uiTimeCheckPoint);
	
	/*根据设定的速率和突发大小，将时间差换算成队列可发送的字节数*/
	while(uiTimeDiff > 0)
	{
		pstQ->uiByteAvail += pstQ->uiBytePerTick;
		if (pstQ->uiByteAvail > pstQ->uiBurstSize)
		{
		    /*Max value of token is burst size.*/
			pstQ->uiByteAvail = pstQ->uiBurstSize;  
			break;
		}
		uiTimeDiff--;
	}
	
	#if 0
	FC_LOG(FC_LOG_DEBUG, "token %d, t_c %u, now %u, HZ %d\n", 
					pstStaQ->uiByteAvail, 
					pstStaQ->uiTimeCheckPoint, 
					uiNow, HZ);
	#endif
	
	/*更新检测时间点*/				
	pstQ->uiTimeCheckPoint = uiNow;
	
	return AP_E_NONE;
}

/*****************************************************************************
 函 数 名  : fc_queue_get_ccache
 功能描述  : 获取队列缓存区
 输入参数  : FC_QUEUE_T *pstStaQ  
             UINT32 uiPktLen      
 输出参数  : 无
 返 回 值  : inline
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月11日 星期六
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
FC_PKT_STATE_E fc_queue_get_ccache(FC_QUEUE_T *pstQ, UINT32 uiPktLen)
{    
    FC_PKT_STATE_E enPktState = FC_ENQ_PKT;
    
    if (NULL == pstQ)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue.\n");
        return FC_DROP_PKT;
    }
    
    FC_QUEUE_WR_LOCK;
    if (pstQ->uiCurCcache >= uiPktLen)
    {
        pstQ->uiCurCcache -= uiPktLen;
        
        /*更新最小缓存区记录*/
        if (pstQ->uiCurCcache < pstQ->uiMinCcache)
        {
            pstQ->uiMinCcache = pstQ->uiCurCcache;
        }
        enPktState = FC_ENQ_PKT;
    }
    else
    {
        enPktState = FC_DROP_PKT;
    }
    FC_QUEUE_WR_UNLOCK;
    
    return enPktState;
}

/*****************************************************************************
 函 数 名  : fc_queue_add_ccache
 功能描述  : 增加队列中的缓存区空间
 输入参数  : FC_QUEUE_T *pstStaQ  
             UINT32 uiPktLen      
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月11日 星期六
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E fc_queue_add_ccache(FC_QUEUE_T *pstQ, UINT32 uiPktLen)
{
    if (NULL == pstQ)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue.\n");
        return AP_E_PARAM;
    }
    
    FC_QUEUE_WR_LOCK;
    pstQ->uiCurCcache += uiPktLen;
    if (unlikely(pstQ->uiCurCcache > FC_MAX_CCACHE))
    {
        FC_LOG(FC_LOG_WARNING, "Warning! Ccache is over maximum %d\n", FC_MAX_CCACHE);
        pstQ->uiCurCcache = FC_MAX_CCACHE;
    }
    FC_QUEUE_WR_UNLOCK;
    
    return AP_E_NONE;
}

AP_ERROR_CODE_E fc_queue_send_pkt_us(struct sk_buff *pstSkb)
{
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,20)) 
    INT32 iRet;
#endif

#if (defined(GW_X86) || defined(GW_ARM) || defined(OPENWRT))
    rx_handler_result_t eRet;
#endif

    if (NULL == pstSkb)
    {
        FC_LOG(FC_LOG_ERR, "Invalid pstSkb.\n");
        return AP_E_PARAM;
    }
    
 
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,20)) 
#if (defined(GW_X86) || defined(GW_ARM) )   //by cj
    eRet = br_handle_frame_hook(&pstSkb);
    if (RX_HANDLER_CONSUMED == eRet)  /*发送成功*/
    {
        return AP_E_NONE;
    }
#elif  defined OPENWRT
    eRet = br_handle_frame(&pstSkb);
    if (RX_HANDLER_CONSUMED == eRet)  /*发送成功*/
    {
        return AP_E_NONE;
    }
#else
    pstSkb = br_handle_frame_hook(pstSkb->dev->br_port, pstSkb);
    if (NULL == pstSkb) /*发送成功*/
    {
        return AP_E_NONE;
    }
#endif

#else
    iRet = br_handle_frame_hook(pstSkb->dev->br_port, &pstSkb);
    if (1 == iRet)  /*发送成功*/
    {
        return AP_E_NONE;
    }
#endif

    /*发送失败*/
    if (NULL != pstSkb)
    {
        kfree_skb(pstSkb);
    }
    FC_LOG(FC_LOG_ERR, "FC timer Send packet to upstream failed.\n");
    return AP_E_DEFAULT_FAIL;
   
    return AP_E_NULLPTR;
}

AP_ERROR_CODE_E fc_queue_send_pkt_ds(struct sk_buff *pstSkb)
{
    if (NULL == pstSkb)
    {
        FC_LOG(FC_LOG_ERR, "Invalid pstSkb.\n");
        return AP_E_PARAM;
    }
    
    br_dev_queue_push_xmit(pstSkb);
    return AP_E_NONE;
}

AP_ERROR_CODE_E fc_queue_send_pkt(FC_QUEUE_T *pstQ, struct sk_buff *pstSkb)
{
    AP_ERROR_CODE_E enRet = AP_E_NONE;
    
    if (FC_DIR_US == pstQ->enDir)
    {
        enRet = fc_queue_send_pkt_us(pstSkb);
    }
    else
    {
        enRet = fc_queue_send_pkt_ds(pstSkb);
    }
    
    return enRet;
}

/*****************************************************************************
 函 数 名  : fc_queue_pass_allowed
 功能描述  : 确定队列是否允许该报文发送
 输入参数  : FC_QUEUE_T *pstQ  --队列结构
             struct sk_buff *skb  --待发送的报文
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月11日 星期六
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
FC_PKT_STATE_E fc_queue_pass_allowed(
    FC_QUEUE_T *pstQ, 
    struct sk_buff *pstSkb,
    FC_PASS_TYPE_T enPassType)
{
    UINT32 uiPktLen;
    INT32 uiNewToken; /*必须为有符号类型*/
    FC_PKT_STATE_E enPktState = FC_PASS_PKT;
    UINT32 skbQLen;
    
    //struct sk_buff *pstSkb = *ppstSkb;
    if (NULL == pstQ)
    {
        return FC_PASS_PKT;
    }
    
    /*速率为0时，表示不进行限速*/
    if (0 == pstQ->uiBytePerTick)
    {
        return FC_PASS_PKT;
    }
    
    if (g_bKeepSequence && FC_PASS_FAST == enPassType)
    {
        #if 0 /*For debug.*/
        if ((g_bKeepSeqUS && FC_DIR_US == pstQ->enDir)
            || (g_bKeepSeqDS && FC_DIR_DS == pstQ->enDir))
        #endif
        
        skbQLen = skb_queue_len((&pstQ->stSkbList)); 
        if (skbQLen > 0)
        {
            FC_LOG(FC_LOG_DEBUG, "Enq Packet.\n");
            //enPktState = fc_queue_keep_sequence(pstQ, ppstSkb);
            return FC_ENQ_PKT;
        }
    }
    
    fc_queue_update_token(pstQ);
    
    
    
    uiPktLen = fc_get_skb_len(pstSkb);
    uiNewToken = pstQ->uiByteAvail - uiPktLen;
    
    if (uiNewToken >= 0)
    {  
        enPktState = FC_PASS_PKT;
        pstQ->uiByteAvail = uiNewToken; /*更新当前可用字节数*/
                
        /*报文处于队列中,则发送该报文后，增加队列的缓存区大小*/
        if (FC_PASS_DELAY == enPassType)
        {
            fc_queue_add_ccache(pstQ, uiPktLen);
        }
    }
    else
    {
        enPktState = FC_DROP_PKT;
    }
    
    return enPktState;
}

/*****************************************************************************
 函 数 名  : fc_queue_need_update_auto_rate
 功能描述  : 判断当前速率是否需要调整
 输入参数  : FC_DIR_E enDir       
             INT32 uiBytePerTick  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月13日 星期一
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
BOOL fc_queue_need_update_auto_rate(FC_DIR_E enDir, INT32 uiBytePerTick)
{
    INT32 uiOldRate;
    INT32 uiResult;
    
    uiOldRate = g_auiBytePerTick[enDir];
    uiResult = (100*(uiOldRate - uiBytePerTick))/uiOldRate;
    
    if (uiResult < 0)
    {
        uiResult = -uiResult;
    }
    
    return (uiResult > 10);
}


/*****************************************************************************
 函 数 名  : fc_queue_auto_rate
 功能描述  : 实现自动调节速率的功能
 输入参数  : FC_DIR_E enDir  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月13日 星期一
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E fc_queue_auto_rate(FC_DIR_E enDir)
{
    FC_QUEUE_T *pstQ;
    UINT32 uiIndex;
    UINT32 uiTotalByteDiff = 0;
    UINT32 uiQNum = 0;
    INT32 uiBytePerTick;
    UINT32 uiBurstSize;
    
    
    pstQ = fc_get_queue_array_by_dir(enDir);
    if (NULL == pstQ)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue.\n");
        return AP_E_PARAM;
    }
    
    /*计算前一秒的总速率*/
    for (uiIndex = 0; uiIndex < FC_QUEUE_NUM_MAX; uiIndex++)
    {
        if (!pstQ[uiIndex].bInUse)
        {
            continue;
        }
        
        
        pstQ[uiIndex].stStatsCur.uiByteTotal =
                     pstQ[uiIndex].stStatsCur.uiBytePassFast
                   + pstQ[uiIndex].stStatsCur.uiBytePassDelay
                   + pstQ[uiIndex].stStatsCur.uiByteDrop;
                   
                   
        uiTotalByteDiff += (pstQ[uiIndex].stStatsCur.uiByteTotal 
                            - pstQ[uiIndex].stStatsLast1Sec.uiByteTotal);
        /*更新统计记录*/
        pstQ[uiIndex].stStatsLast1Sec.uiByteTotal
                        = pstQ[uiIndex].stStatsCur.uiByteTotal;
        
        /*统计有效队列数目*/
        uiQNum++;
    }
    
    /*计算平均值*/
    uiBytePerTick = uiTotalByteDiff / (uiQNum * HZ);
    
    /*不需要调整速率*/
    if (!fc_queue_need_update_auto_rate(enDir, uiBytePerTick))
    {   
        return AP_E_NONE;
    }
    
    /*修改队列速率*/
    uiBurstSize = 5 * TICKBYTE_TO_RATE(uiBytePerTick);
    for (uiIndex = 0; uiIndex < FC_QUEUE_NUM_MAX; uiIndex++)
    {
        if (!pstQ[uiIndex].bInUse)
        {
            continue;
        }
        pstQ[uiIndex].uiBytePerTick = uiBytePerTick;
        pstQ[uiIndex].uiBurstSize = uiBurstSize;
    }
    
    /*保存最新速率*/
    g_auiBytePerTick[enDir] = uiBytePerTick;
    
    return AP_E_NONE; 
}



/*****************************************************************************
 函 数 名  : fc_queue_get_list
 功能描述  : 获取当前的队列列表
 输入参数  : FC_IOCTL_ENTITY_LIST_T *pstEntityList
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月15日 星期三
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
UINT32 fc_queue_get_list(FC_ENTITY_LIST_T *pstList, UINT32 uiStartNum)
{
    FC_QUEUE_T *pstQArray;
    FC_QUEUE_T *pstQ;
    FC_ENTITY_T *pstEntity;
    
    FC_DIR_E enDir;
    UINT32 uiIndex;
        
    UINT32 uiQNum;
    UINT32 uiSkipNum;
    
    if (NULL == pstList)
    {
        return AP_E_PARAM;
    }
    
    
    for (enDir = FC_DIR_DS; enDir < FC_DIR_MAX; enDir++)
    {
        uiQNum = 0;
        uiSkipNum = 0;
        
        pstQArray = fc_get_queue_array_by_dir(enDir);
        
        for (uiIndex = 0; uiIndex < FC_QUEUE_NUM_MAX; uiIndex++)
        {
            pstQ = &pstQArray[uiIndex];
            
            if (!pstQ->bInUse)
            {
                continue;
            }
            
            /*跳过之前访问过的队列*/
            if (uiSkipNum < uiStartNum)
            {
                //printk("skip [%d](%d)\n", pstQ->uiQId, enDir);
                uiSkipNum++;
                continue;
            }
            
            /*获取到了足够的队列*/
            if (uiQNum >= FC_ENTITY_LIST_SIZE)
            {
                break;
            }
            
            /*获取Entity*/
            pstEntity = &(pstList->astEntity[uiQNum]);
            
            /*获取队列属性*/
            pstEntity->stMatchAttrib = pstQ->stMatchAttrib;
            
            /*获取速率配置*/
            pstEntity->astAdminRate[enDir].uiRate = TICKBYTE_TO_RATE(pstQ->uiAdminBytePerTick);
            pstEntity->astAdminRate[enDir].uiBurst = pstQ->uiAdminBurstSize;
            pstEntity->astRealRate[enDir].uiRate = TICKBYTE_TO_RATE(pstQ->uiBytePerTick);
            pstEntity->astRealRate[enDir].uiBurst = pstQ->uiBurstSize;
            
            /*获取统计*/
            pstEntity->astStats[enDir] = pstQ->stStatsCur; 
            
            FC_LOG(FC_LOG_INFO, "[%d](%d): %d/%d\n", 
                pstQ->uiQId, enDir, 
                pstQ->uiCurCcache,
                pstQ->uiMinCcache
                );
            
            uiQNum++;
            
        }
    }
    
    //printk("uiStartNum = %d, uiQNum = %d\n", uiStartNum, uiQNum);
    return uiQNum;
    
}

