/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技股份有限公司

 ******************************************************************************
  文 件 名   : fc_event.c
  版 本 号   : 初稿
  作    者   : xucongjiang
  生成日期   : 2012年11月8日
  最近修改   :
  功能描述   : 内核网络设备通知链流控相关处理
  函数列表   :
              fc_del_skb_by_port
              fc_device_event
  修改历史   :
  1.日    期   : 2012年11月8日
    作    者   : xucongjiang
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "fc_main.h"
#include "fc_queue.h"
/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/
LOCAL INT32 fc_device_event(struct notifier_block *unused, unsigned long event, void *ptr);
/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
struct notifier_block fc_device_notifier = 
{
    .notifier_call = fc_device_event
};

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/



/*****************************************************************************
 函 数 名  : fc_del_skb_by_port
 功能描述  : 删除流控skb缓存区的某个端口下的skb
 输入参数  : struct net_bridge_port *pstPort  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年10月20日
    作    者   : xucongjiang
    修改内容   : 新生成函数

*****************************************************************************/
LOCAL AP_ERROR_CODE_E fc_del_skb_by_port(struct net_bridge_port *pstPort)
{
    FC_DIR_E enDir;
    UINT32 uiIndex;
    UINT32 uiPktLen;
    FC_QUEUE_T *pstQ;
    struct sk_buff *pstSkb = NULL;	
    struct sk_buff *pstSkbTmp = NULL;

    
    if (!fc_get_admin()) /*流控被关闭*/
    {   
        return AP_E_NONE;
    }
    
    if  ( unlikely(NULL == pstPort) )
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue.\n");
        return AP_E_NULLPTR;
    }
    
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
                /*遍历队列缓存区*/
                skb_queue_walk_safe(&pstQ->stSkbList, pstSkb, pstSkbTmp)
                {
                    if  ( 0 == strncmp(pstSkb->dev->name, pstPort->dev->name, IFNAMSIZ) )
                    {
                    #if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,15)) /* 2012-10-26 add by zhangping */
                        skb_unlink(pstSkb);
                    #else
                        skb_unlink(pstSkb, &pstQ->stSkbList);
                    #endif

                        /*更新统计*/
                        uiPktLen = fc_get_skb_len(pstSkb);
                        FC_UPDATE_STATS_DROP(pstQ, uiPktLen);

                        /* 丢弃该报文 */
                        kfree_skb(pstSkb);
                        continue;
                    }
                }
            }
            pstQ++;  /*OK*/
        }
        
    }
    
    return AP_E_NONE;
}


/*****************************************************************************
 函 数 名  : fc_device_event
 功能描述  : 通知链事件处理
 输入参数  : struct notifier_block *unused  
             unsigned long event            
             void *ptr                      
 输出参数  : 无
 返 回 值  : LOCAL
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月8日
    作    者   : xucongjiang
    修改内容   : 新生成函数

*****************************************************************************/
LOCAL INT32 fc_device_event(struct notifier_block *unused, unsigned long event, void *ptr)
{
    struct net_device *dev = ptr;
    
#if (defined(GW_X86) || defined(GW_ARM) || defined(OPENWRT))
    struct net_bridge_port *pstPort = br_port_get_rcu(dev);
#else
    struct net_bridge_port *pstPort = dev->br_port;
#endif

    /* not a port of a bridge */
    if ( pstPort == NULL )
    {
        return NOTIFY_DONE;
    }	
	
	switch (event) 
	{		
	case NETDEV_DOWN:
        FC_LOG(FC_LOG_ALERT, "recv %s NETDEV_DOWN\n", dev->name);
        fc_del_skb_by_port(pstPort);
		break;
				
	case NETDEV_UNREGISTER:
	    FC_LOG(FC_LOG_ALERT, "recv %s NETDEV_UNREGISTER\n", dev->name);
	    //fc_queue_flush_all();
	    break;
	} 
	
	return NOTIFY_DONE;
}

