/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : fc_main.c
  版 本 号   : 初稿
  作    者   : Zhong Huaqing
  生成日期   : 2012年8月10日 星期五
  最近修改   :
  功能描述   : AP流控模块
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

#include "fc_queue.h"
#include "fc_hash.h"
#include "fc_timer.h"

#include "fc_sta.h"
#include "fc_vap.h"
#include "fc_ssid.h"
#include "fc_ap.h"
#include "fc_white_list.h"
/*----------------------------------------------*
 * 宏定义                                *
 *----------------------------------------------*/
//#define FC_DISABLE_TIMER 1  /*禁用队列发送定时器*/

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
REMOTE FC_PKT_STATE_E (*fc_enqueue_hook)(struct sk_buff *pstSkb, FC_DIR_E enDir);

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
BOOL g_bDebugLevel = FC_LOG_ERR;

BOOL g_bDequeueFast = FALSE;
module_param(g_bDequeueFast, uint, 0600);

BOOL g_bThresholdEnable = TRUE;
module_param(g_bThresholdEnable, uint, 0600);
/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/
 
LOCAL BOOL g_bAdmin = FALSE;
LOCAL FC_RULE_E g_enRule = FC_RULE_STA;

LOCAL BOOL g_bAutoRatenable = FALSE;  /*自动调节速率*/

//LOCAL FC_QUEUE_AUTO_LEARN_T g_stAutoLearn;

LOCAL UINT32 g_uiGearByte = 0;
/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 函数定义                                       *
 *----------------------------------------------*/
#ifdef OPENWRT
LOCAL UINT32 fc_pre_routing_hook(UINT32 hooknum,
		     struct sk_buff *skb,
		     const struct net_device *in,
		     const struct net_device *out,
		     int (*okfn)(struct sk_buff *)){
    int ret = NF_ACCEPT;
    ret =  fc_enqueue(skb,FC_DIR_DS); 
    if (FC_PASS_PKT != ret)
        return NF_DROP;
    else 
        return NF_ACCEPT; 
}
		     
LOCAL UINT32 fc_post_routing_hook(UINT32 hooknum,
		     struct sk_buff *skb,
		     const struct net_device *in,
		     const struct net_device *out,
		     int (*okfn)(struct sk_buff *)){
    int ret = NF_ACCEPT;
    ret =  fc_enqueue(skb,FC_DIR_DS); 
    if (FC_PASS_PKT != ret)
        return NF_DROP;
    else 
        return NF_ACCEPT;
}


LOCAL struct nf_hook_ops fc_hooks[2] = {
    {
        .hook = fc_pre_routing_hook,
        .owner = THIS_MODULE,
        .pf = PF_BRIDGE,
        .hooknum = NF_BR_PRE_ROUTING,
        .priority	= NF_IP_PRI_FC_UP,
    },
    {
        .hook = fc_post_routing_hook,
        .owner = THIS_MODULE,
        .pf = PF_BRIDGE,
        .hooknum = NF_BR_POST_ROUTING,
        .priority	= NF_IP_PRI_FC_DOWN,
    }
};
#endif
AP_ERROR_CODE_E fc_hook_init(void)
{
    fc_enqueue_hook = fc_enqueue;
    return AP_E_NONE;
}

AP_ERROR_CODE_E fc_hook_destroy(void)
{
    fc_enqueue_hook = NULL;
    return AP_E_NONE;
}


void fc_set_admin(BOOL bEnable)
{
    g_bAdmin = bEnable;
    return;
}

BOOL fc_get_admin(void)
{
    return g_bAdmin;
}

void fc_set_debugLevel(INT32 iDebugLevel)
{
    g_bDebugLevel = iDebugLevel;
}
INT32 fc_get_debugLevel(void)
{
    return g_bDebugLevel;
}


FC_RULE_E fc_get_rule(void)
{
    return g_enRule;
}

AP_ERROR_CODE_E fc_set_rule(FC_RULE_E enRule)
{    
    if (g_enRule == enRule)
    {
        return AP_E_NONE;
    }
    
    fc_queue_flush_all(); 
    
    g_enRule = enRule; /*保存配置*/
    
    return AP_E_NONE;
}

AP_ERROR_CODE_E fc_set_gear_byte(UINT32 uiGearByte)
{
    if (uiGearByte >= FC_MTU)
    {
        return AP_E_PARAM;
    }
    
    g_uiGearByte = uiGearByte;
    FC_LOG(FC_LOG_DEBUG, "Set gear byte to be %d\n", uiGearByte);
    
    fc_queue_all_gear_rate(); 
    
    return AP_E_NONE;
}

UINT32 fc_get_gear_byte(void)
{
    return g_uiGearByte;
}

#if 0
AP_ERROR_CODE_E fc_auto_learn_init(void)
{
    g_stAutoLearn.bIsOn = FALSE;
    return AP_E_NONE;
}
BOOL fc_get_auto_learn_status(void)
{
    return g_stAutoLearn.bIsOn;
}

AP_ERROR_CODE_E fc_set_auto_learn(FC_QUEUE_AUTO_LEARN_T *pstAutoLearn)
{
    FC_LOG(FC_LOG_DEBUG, "%s, %d/%d, %d/%d\n",
        pstAutoLearn->bIsOn ? "ON" : "OFF",
        pstAutoLearn->astRate[FC_DIR_US].uiRate,
        pstAutoLearn->astRate[FC_DIR_US].uiBurst,
        pstAutoLearn->astRate[FC_DIR_DS].uiRate,
        pstAutoLearn->astRate[FC_DIR_DS].uiBurst
        );
        
    g_stAutoLearn = *pstAutoLearn;
    return AP_E_NONE;
}

AP_ERROR_CODE_E fc_get_auto_learn(FC_QUEUE_AUTO_LEARN_T *pstAutoLearn)
{
    *pstAutoLearn = g_stAutoLearn;
    return AP_E_NONE;
}
#endif

BOOL fc_get_auto_rate_status(void)
{
    return g_bAutoRatenable;
}

AP_ERROR_CODE_E fc_set_auto_rate_status(BOOL bStatus)
{
    #if 0
    //FC_QUEUE_AUTO_LEARN_T stAutoLearn;
    AP_ERROR_CODE_E enRet;
    
    if (bStatus == g_bAutoRatenable)
    {
        return AP_E_NONE;
    }
    
    if (bStatus) /*开启自动速率*/
    {
        stAutoLearn.bIsOn = FALSE;
        fc_set_auto_learn(&stAutoLearn);
    }
    else    /*关闭自动速率*/
    {
       enRet = fc_queue_flush_all(); 
       if (AP_E_NONE != enRet)
       {
           FC_LOG(FC_LOG_ERR, "fc_queue_flush_all failed.");
           return enRet;
       }
    }
    #endif
    
    g_bAutoRatenable = bStatus;
    
    return AP_E_NONE;
}



UINT32 fc_get_skb_len(struct sk_buff *pstSkb)
{
    if (NULL == pstSkb)
    {
        return 0;
    }
    
    return (UINT32)pstSkb->tail - (UINT32)eth_hdr(pstSkb);
}

inline UINT8 *fc_get_sta_mac(struct sk_buff *pstSkb, FC_DIR_E enDir)
{
    struct ethhdr *pstEthHdr = NULL;
	UINT8 *pcMac = NULL;
	
	pstEthHdr = eth_hdr(pstSkb);
	if (FC_DIR_US == enDir)
		pcMac = pstEthHdr->h_source;
	else
		pcMac = pstEthHdr->h_dest;
	
	return pcMac;
}

/*后期采用brige_port中的ssid字段*/
inline CHAR*fc_get_ssid_name(struct sk_buff *pstSkb, FC_DIR_E enDir)
{

    #if (defined(GW_X86) || defined(GW_ARM) || defined(OPENWRT))     //兼容x86内核 by cj
        return NULL;
    #else
        if (NULL == pstSkb->dev->br_port)
        {
            return NULL;
        }
        
        return pstSkb->dev->br_port->acSsid;
    #endif
}

/*****************************************************************************
 函 数 名  : fc_cmp_queue_with_pkt
 功能描述  : 自动学习时，如果存在sta的限速结构，则判断vap/ssid/vlan是否与skb中的
             相同，若不同，则将sta的限速结构删除
 输入参数  : struct sk_buff *pstSkb  
             FC_QUEUE_T *pstQ        
             FC_DIR_E enDir          
 输出参数  : 无
 返 回 值  : LOCAL
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年10月20日
    作    者   : xucongjiang
    修改内容   : 新生成函数

*****************************************************************************/
LOCAL FC_QUEUE_T *fc_cmp_queue_with_pkt(
        struct sk_buff *pstSkb, 
        FC_QUEUE_T *pstQ, 
        FC_DIR_E enDir)
{
    FC_RULE_E enRule;

    
    if  ( NULL == pstQ || NULL == pstSkb )
    {
        return NULL;
    }
    
    enRule = fc_get_rule();
    if  ( FC_RULE_IS_VAP_AUTO(enRule) )
    {
        INT32 iRadioId = FC_RID_INVALID;
        INT32 iWlanId = FC_WID_INVALID;
        CHAR *szVapName = pstSkb->dev->name;
        
        sscanf(szVapName, "ap%d_%d", &iRadioId, &iWlanId);
        if  ( iWlanId != pstQ->stMatchAttrib.iWlanId ||
              iRadioId != pstQ->stMatchAttrib.iRadioId )
        {
            fc_queue_destroy(pstQ);
            return NULL;
        }
    }
    else if  ( FC_RULE_IS_SSID_AUTO(enRule) )
    {
        CHAR *szSsidName = fc_get_ssid_name(pstSkb, enDir); 
        if  ( 0 != strncmp(pstQ->stMatchAttrib.acSsidName, szSsidName, IFNAMSIZ) )
        {
            fc_queue_destroy(pstQ);
            return NULL;
        }
    }
	else if  ( FC_RULE_IS_VLAN_AUTO(enRule) )
	{
		INT32 vid;
		CHAR *pt;
		CHAR *szVlanDev = pstSkb->dev->name;

		if((pt = strstr(szVlanDev, ".")) != NULL)
			sscanf(pt+1, "%d", &vid);
		else
			vid = 1;
		
		if	( vid != pstQ->stMatchAttrib.iVid ) /*终端报文从另外一个vlan进来了*/
		{
			fc_queue_destroy(pstQ);
			return NULL;
		}
	}

    return pstQ;
}

/*****************************************************************************
 函 数 名  : fc_get_queue_by_pkt
 功能描述  : 获取报文对应的队列结构
 输入参数  : struct sk_buff *pstSkb  
             FC_DIR_E enDir          
 输出参数  : 无
 返 回 值  : FC_QUEUE_T
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月11日 星期六
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
FC_QUEUE_T *fc_get_queue_by_pkt(struct sk_buff *pstSkb, FC_DIR_E enDir)
{
    FC_QUEUE_T *pstQ = NULL;
    FC_RULE_E enRule;
    UINT8 *pcMac;
    CHAR *szVapName;
    CHAR *szSsidName;
	CHAR *szVlanDev;
    
    if (NULL == pstSkb)
    {
        return NULL;
    }
    
    enRule = fc_get_rule();
    
    /*多级流控与自动速率学习功能，都是基于STA队列*/
    if (FC_RULE_IS_MULTI(enRule)
        || FC_RULE_IS_AUTO(enRule))
    {
        enRule = FC_RULE_STA; /*规则与STA相同*/
    }

    switch (enRule)
    {
        case FC_RULE_STA: 
            pcMac = fc_get_sta_mac(pstSkb, enDir);
            pstQ = fc_get_sta_queue_from_htable(pcMac, enDir);
            break;
            
        case FC_RULE_VAP: 
            szVapName = pstSkb->dev->name;
            pstQ = fc_get_vap_queue_from_htable(szVapName, enDir);
            break;
            
        case FC_RULE_SSID: 
            szSsidName = fc_get_ssid_name(pstSkb, enDir); 
            pstQ = fc_get_ssid_queue_from_htable(szSsidName, enDir);
            break;
            
        case FC_RULE_AP: 
            //Do something later.
            pstQ = fc_get_ap_queue(enDir);
            break;

		case FC_RULE_VLAN: 
			szVlanDev = pstSkb->dev->name;
            pstQ = fc_get_vlan_queue_from_htable(szVlanDev, enDir);
            break;
            
        default:
            FC_LOG(FC_LOG_ERR, "Invalid rule value %d\n", enRule);
            return NULL;
            break;
    }
    
    return fc_cmp_queue_with_pkt(pstSkb, pstQ, enDir);
}



#if 0
AP_ERROR_CODE_E fc_sta_get_auto_learn_rate(CHAR *acVapName, FC_STA_T *pstSta)
{
    FC_RULE_E enRule;
    struct net_device * pstDev;
    FC_QUEUE_T *pstPQ;
    FC_DIR_E enDir;
    
    enRule = fc_get_rule();
    
    for (enDir = FC_DIR_DS; enDir < FC_DIR_MAX; enDir++)
    {
        if (FC_RULE_AP & enRule)
        {
            pstPQ = fc_get_ap_queue(enDir);
        }
            
        if (FC_RULE_VAP & enRule)
        {
           pstPQ = fc_get_vap_queue_from_htable(acVapName, enDir);
        }
        
        if (FC_RULE_SSID & enRule)
        {
            pstDev = fc_get_dev_by_name(acVapName);
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
        /*获取总体的配置速率*/
        pstSta->astRate[enDir].uiRate = TICKBYTE_TO_RATE(pstPQ->uiAdminBytePerTick);
        pstSta->astRate[enDir].uiBurst = TICKBYTE_TO_RATE(pstPQ->uiAdminBurstSize);
        
    }
    
    
    return AP_E_NONE;
}
#endif

/*****************************************************************************
 函 数 名  : fc_auto_learn
 功能描述  : 实现流控规则自动学习功能
 输入参数  : struct sk_buff *pstSkb  
             FC_DIR_E enDir          
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月13日 星期一
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E fc_auto_learn(struct sk_buff *pstSkb, FC_DIR_E enDir)
{
    FC_RULE_E enRule;
    LOCAL FC_STA_T stSta;
    UINT8 *pcMac;
    CHAR *acDevName;
    INT32 iArgNum;
    FC_DIR_E enDirTmp;
    AP_ERROR_CODE_E enRet;
    //INT32 iArgNum;
    
    if (NULL == pstSkb)
    {
        return AP_E_PARAM;
    }
    
    //fc_get_auto_learn(&stAutoLearn);
    enRule = fc_get_rule();
    
    FC_LOG(FC_LOG_DEBUG,"enRule = 0x%x\n", enRule);
    /*自动学习功能未开启*/
    if (!(FC_RULE_AUTO & enRule))
    {
        return AP_E_NONE;
    }
    
    /*获取STA的MAC地址*/
    pcMac = fc_get_sta_mac(pstSkb, enDir);
    if(pcMac == NULL)
	 return AP_E_PARAM;
	
    if (0x01 & pcMac[0]) /*组播MAC地址*/
    {
        FC_LOG(FC_LOG_DEBUG, "STA MAC Type is Multicast or Broadcast.\n");
        return AP_E_DEFAULT_FAIL;
    }
    memcpy(stSta.acMac, pcMac, ETH_ALEN);
    
    /*获取自动学习的速率*/
    acDevName = pstSkb->dev->name;
    for (enDirTmp = FC_DIR_DS; enDirTmp < FC_DIR_MAX; enDirTmp++)
    {
        enRet = fc_queue_get_auto_learn_rate(
                            enDirTmp, 
                            acDevName, 
                            &stSta.astRate[enDirTmp]);
        if (AP_E_NONE != enRet)
        {
            FC_LOG(FC_LOG_DEBUG,"STA "MAC_FMT" (dir %d)auto learn failed.\n", 
                    MAC_ARG(pcMac), enDirTmp);
            return enRet;
        }
    }

	if (FC_RULE_VLAN & enRule){
		/*获取vlan id*/
		INT32 vid;
		CHAR *pt;
		if((pt = strstr(acDevName, ".")) != NULL)
			sscanf(pt+1, "%d", &vid);
		else
			vid = 1;
	
		if((vid <= 0) || (vid >= 4096))
			return AP_E_PARAM;
		stSta.iVid = vid;
	}
    
/* BEGIN: Modified by xucongjiang, 2012/11/7   问题单号:CPE上终端很可能是从有线端口接入的 */
//#if 1  
    /*获取radio id 和wlan id*/
    if(FC_RULE_VAP & enRule)
    {
        iArgNum = sscanf(acDevName, "ap%u_%u", &stSta.iRadioId, &stSta.iWlanId);
        if (iArgNum != 2)
        {
            return AP_E_DEFAULT_FAIL;
        }
    }
    
    FC_LOG(FC_LOG_INFO,"STA (%s) "MAC_FMT" auto learn: %d:%d/%d:%d\n",
                    acDevName, 
                    MAC_ARG(stSta.acMac), 
                    stSta.astRate[FC_DIR_US].uiRate,
                    stSta.astRate[FC_DIR_US].uiBurst,
                    stSta.astRate[FC_DIR_DS].uiRate,
                    stSta.astRate[FC_DIR_DS].uiBurst
                    );  
    return fc_add_sta(&stSta);   
    /* END:   Modified by xucongjiang, 2012/11/7 */
}

/*****************************************************************************
 函 数 名  : fc_enqueue
 功能描述  : 报文入队处理
 输入参数  : struct sk_buff *pstSkb  --待处理的报文
             FC_DIR_E enDir          --数据流方向
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月11日 星期六
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
FC_PKT_STATE_E fc_enqueue(struct sk_buff *pstSkb, FC_DIR_E enDir)
{
    FC_QUEUE_T *pstQ = NULL;
    UINT32 uiPktLen;
    FC_PKT_STATE_E enPktState;
    FC_PKT_STATE_E enEnqState;
	
	FC_RULE_E enRule;
    
    //struct sk_buff *pstSkb = *ppstSkb;

    
    if (unlikely(NULL == pstSkb))
    {
        FC_LOG(FC_LOG_ERR, "pstSkb=0x%08x\n", (UINT32)pstSkb);
        return FC_PASS_PKT;
    }
    
	/*报文已入队*/
	if (FC_ENQ_FLAG_VALID(pstSkb))
	{
	    return FC_PASS_PKT;
	}
	
    if (!fc_get_admin()) /*流控被关闭*/
    {   
        return FC_PASS_PKT;
    }
    /* BEGIN: Modified by xucongjiang, 2012/11/7   PN:CPE流控定制修改 */
#if 0   
    if ( !DEV_IS_VAP(pstSkb) ) 
    {
        return FC_PASS_PKT;
    }
#else
    if ( !DEV_IS_ETH(pstSkb) && !DEV_IS_VAP(pstSkb) ) 
    {
        return FC_PASS_PKT;
    }
#endif
    /* END:   Modified by xucongjiang, 2012/11/7   PN:1 */

	enRule = fc_get_rule();
	if((FC_RULE_VAP & enRule) || (FC_RULE_SSID & enRule)){
		//if ( DEV_IS_ETH(pstSkb) )
		//	return FC_PASS_PKT;
	}
	else if(FC_RULE_VLAN & enRule){
		if ( DEV_IS_VAP(pstSkb) )
			return FC_PASS_PKT;
	}

	uiPktLen = fc_get_skb_len(pstSkb);

	/*For packet of small size*/
	if (g_bThresholdEnable
	    && uiPktLen <= FC_THRESHOLD_SIZE)
    {
        return FC_PASS_PKT;
    }

    if (TRUE == fc_should_pass_by_white_list(pstSkb,enDir))
    {
        FC_LOG(FC_LOG_DEBUG, "in the ip white list,pass\n");
        return FC_PASS_PKT;
    }
    /*获取队列结构*/
    pstQ = fc_get_queue_by_pkt(pstSkb, enDir);
    
    if (NULL == pstQ) /* 队列不存在 */
    {
        /*规则自动添加*/
        fc_auto_learn(pstSkb, enDir);
        return FC_PASS_PKT;
    }
    

    /*确定报文是否允许通过*/
    enPktState = fc_queue_pass_allowed(pstQ, pstSkb, FC_PASS_FAST);
    if (FC_PASS_PKT == enPktState)
    {
        FC_LOG(FC_LOG_DEBUG, "[%d](%d): Pass %d bytes.\n", 
                                pstQ->uiQId, enDir, uiPktLen);
        FC_UPDATE_STATS_PASS_FAST(pstQ, uiPktLen);
        
        //*ppstSkb = pstSkb; 
        return FC_PASS_PKT;
    }    
    
       
    
    /*报文入队*/
    enEnqState = fc_queue_get_ccache(pstQ, uiPktLen);
    if (FC_ENQ_PKT == enEnqState) /*允许入队*/
    {
        FC_LOG(FC_LOG_DEBUG, "[%d](%d): Enqueue %d bytes.\n", 
                                pstQ->uiQId, enDir, uiPktLen);
        SET_FC_ENQ_FLAG(pstSkb);  /*标志报文已经入队*/
        skb_queue_tail(&pstQ->stSkbList, pstSkb); /*将报文添加到队列的缓存区中*/
        
        /*启动队列的报文发送定时器*/
        if (!FC_THROTTLE_FLAG_CHECK(pstQ))
        {
            FC_LOG(FC_LOG_DEBUG, "[%d](%d): Restart tx timer.\n", pstQ->uiQId, enDir);
            FC_THROTTLE_FLAG_SET(pstQ);
            fc_queue_tx_timer_restart(pstQ, pstQ->uiTxDelay);
        }
    }
    else
    {
        FC_LOG(FC_LOG_DEBUG, "[%d](%d): Drop %d bytes.\n", 
                                pstQ->uiQId, enDir, uiPktLen);
        kfree_skb(pstSkb); /*释放报文空间*/
        //printk("[%d](%d):Drop %d bytes\n", pstQ->uiQId, enDir, uiPktLen);
        FC_UPDATE_STATS_DROP(pstQ, uiPktLen);
    }
    
    return enEnqState;
}

/*****************************************************************************
 函 数 名  : fc_dequeue
 功能描述  : 队列定时器报文发送处理过程。本函数仅提供给队列发送定时器使用。
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
UINT32 fc_dequeue(FC_QUEUE_T *pstQ)
{
    struct sk_buff *pstSkb = NULL;	
    struct sk_buff *pstSkbTmp = NULL;	
    UINT32 uiPktLen;
    UINT32 uiSendTotal = 0;
    
    FC_PKT_STATE_E enPktState;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstQ)
    {
        return 0;    
    }
    
    /*遍历队列缓存区*/
    skb_queue_walk_safe(&pstQ->stSkbList, pstSkb, pstSkbTmp)
    {
        enPktState = fc_queue_pass_allowed(pstQ, pstSkb, FC_PASS_DELAY);
        if (FC_DROP_PKT == enPktState)
        {
            break;
        }
        #if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,15)) /* 2012-10-23 add by zhangping */
            skb_unlink(pstSkb);
        #else
            skb_unlink(pstSkb, &pstQ->stSkbList);
        #endif
        
        uiPktLen = fc_get_skb_len(pstSkb);
        enRet = fc_queue_send_pkt(pstQ, pstSkb);
        
        /*更新统计*/
        FC_UPDATE_STATS_PASS_DELAY(pstQ, uiPktLen);
        uiSendTotal += uiPktLen;
    }
    
    return uiSendTotal;
}


/*****************************************************************************
 函 数 名  : flowctrl_init
 功能描述  : 初始化
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月11日 星期六
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E flowctrl_init(void)
{
    AP_ERROR_CODE_E enRet;
    
    enRet = fc_hash_init();
    
    return enRet;
}

