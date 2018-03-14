/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : fc_queue.c
  �� �� ��   : ����
  ��    ��   : Zhong Huaqing
  ��������   : 2012��8��10�� ������
  ����޸�   :
  ��������   : ������ص�hash����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��8��10�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "fc_main.h"
#include "fc_timer.h"
#include "fc_hash.h"

#include "fc_queue.h"
#include "fc_ssid.h"


/*----------------------------------------------*
 * �궨��                                 *
 *----------------------------------------------*/
//#define FC_KEEP_SEQ  1    /*���ڱ���ԭ�ȱ��ĵ�˳��*/
 
/*----------------------------------------------*
 * �ⲿ����˵��                                 *
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
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/
REMOTE int br_dev_queue_push_xmit(struct sk_buff *skb);

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
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
 * ģ�鼶����                                   *
 *----------------------------------------------*/
FC_QUEUE_T g_aastQueue[FC_DIR_MAX][FC_QUEUE_NUM_MAX]; /*�����ж���*/

INT32 g_auiBytePerTick[FC_DIR_MAX];

#if (defined(GW_X86) || defined(GW_ARM))

LOCAL DEFINE_RWLOCK(g_stQLock);

#else
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20))    
rwlock_t g_stQLock = RW_LOCK_UNLOCKED;  /*ʵ������������ + bh����*/
#else
LOCAL DEFINE_RWLOCK(g_stQLock);
#endif

#endif
/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                       *
 *----------------------------------------------*/

FC_QUEUE_T *fc_get_queue_array_by_dir(FC_DIR_E enDir)
{
    return g_aastQueue[enDir];
}
/*****************************************************************************
 �� �� ��  : fc_find_avail_sta_queue
 ��������  : ��ȡ���еĶ���
 �������  : FC_DIR_E enDir  --���ط���
 �������  : ��
 �� �� ֵ  : FC_QUEUE_T
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��11�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

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
        if (!pstPos->bInUse) /*���п���*/
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
 �� �� ��  : fc_get_ap_queue
 ��������  : ΪAP�������ػ�ȡ����
 �������  : FC_DIR_E enDir  
 �������  : ��
 �� �� ֵ  : FC_QUEUE_T
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��13�� ����һ
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

*****************************************************************************/
FC_QUEUE_T *fc_get_ap_queue(FC_DIR_E enDir)
{
    FC_QUEUE_T *pstPos;
    
    pstPos = fc_get_queue_array_by_dir(enDir);
    return pstPos;
}

/*****************************************************************************
 �� �� ��  : fc_get_parent_queue
 ��������  : ��ȡָ�����е��ϼ�����
 �������  : FC_QUEUE_T *pstQ  
 �������  : ��
 �� �� ֵ  : FC_QUEUE_T
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��19�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

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
    
    /*ֻ���ն�ģʽ���ж�Ӧ�ĸ�����*/
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

/* BEGIN: Modified by xucongjiang, 2012/11/9 ����Զ�����ʱ��ɾ�������������Զ�ѧϰ������ */
    if (NULL == pstPQ || !pstPQ->bInUse )
    {
        return AP_E_RESOURCE;
    }
/* END:   Modified by xucongjiang, 2012/11/9   PN: */
    /*��ȡ�������������*/
    astRate->uiRate = TICKBYTE_TO_RATE(pstPQ->uiAdminBytePerTick);
    astRate->uiBurst = pstPQ->uiAdminBurstSize;
    
    return AP_E_NONE;
}

/*****************************************************************************
 �� �� ��  : fc_queue_gear_rate
 ��������  : �������е�����
 �������  : FC_QUEUE_T *pstQ  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��19�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

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
    
    if (FC_RULE_STA == pstQ->enRule) /*�����κε���*/
    {
        return AP_E_NONE;
    }
    
    uiGearByte = fc_get_gear_byte();
    if (uiGearByte >= FC_MTU)
    {
        return AP_E_NONE;
    }
    
    if (0 == uiGearByte) /*�ָ�������ֵ*/
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
 �� �� ��  : fc_queue_all_gear_rate
 ��������  : �������ж��е�����
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��20�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

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
 �� �� ��  : fc_queue_update
 ��������  : ���¶���������Ϣ
 �������  : FC_QUEUE_T *pstStaQ          --���нṹ
             FC_RATE_T *pstRate           --���ʽṹ
             FC_QUEUE_OPER_TYPE_T enOper  --��������:���д������߸���
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��11�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

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
    
    /*Ĭ������£���ʵ����������������ͬ*/
    pstQ->uiBytePerTick = pstQ->uiAdminBytePerTick;
    pstQ->uiBurstSize = pstQ->uiAdminBurstSize;
    pstQ->uiTxDelay = RATE_TO_DELAY(pstRate->uiRate);
    
    fc_queue_gear_rate(pstQ); /*����ʵ�����е�����*/
    
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
        /*���ʿ������*/
        pstQ->uiTimeCheckPoint = jiffies;
        pstQ->uiByteAvail = pstQ->uiBurstSize;
        //pstQ->uiTxDelay = FC_WATCHDOG_DELAY;
        
        /*���ö��г�ʼ��������С*/
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
            
        if (!pstQ->bInUse) /*����δ��ʹ��*/
        {
            continue;
        }
        
        if (FC_QID_INVALID != pstQ->iPQId) /*STA�Ѿ�������ĳ��parent ����*/
        {
            continue;
        }
        if (FC_RULE_STA != pstQ->enRule) /*ֻ��STA���в�����Ϊ�Ӷ���*/
        {
            continue;
        }
        
        pstPQTmp = fc_get_parent_queue(pstQ);
        if (NULL == pstPQTmp)
        {
            continue;
        }
		
        FC_LOG(FC_LOG_INFO, "sta qid %d, pqid %d\n", pstQ->uiQId, pstPQTmp->uiQId);
        if (pstPQTmp->uiQId == pstPQ->uiQId) /*ƥ��ɹ�*/
        {
            FC_QUEUE_WR_LOCK;
            pstQ->iPQId = pstPQ->uiQId; /*�޸�PQid*/
            list_add_tail(&pstQ->stStaSet, &pstPQ->stStaSet);
            pstQ->stMatchAttrib.iVid = pstPQ->stMatchAttrib.iVid;
            FC_QUEUE_WR_UNLOCK;
        }
        
    }
    
    return AP_E_NONE;
}

/*****************************************************************************
 �� �� ��  : fc_adjust_child_queue_rate
 ��������  : �����Ӷ��е�����
 �������  : FC_QUEUE_T *pstPQ  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��13�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

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

    
    /*�����Ӷ����ܵ����ʺ�����*/
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
    
    /*����ȱ������У�ÿһ�ݵ���ֵ*/
    uiParticle = (pstPQ->uiAdminBytePerTick * 100) / uiStaTotalRate;
    /*��������ʱ������*/
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
        else /*��һ���������㹻����˻ָ�����������*/
        {
            pstQTmp->uiBytePerTick = pstQTmp->uiAdminBytePerTick;
            pstQTmp->uiBurstSize = pstQTmp->uiAdminBurstSize;
        }
        FC_QUEUE_WR_UNLOCK;
    }
    
    return AP_E_NONE;
}

/*****************************************************************************
 �� �� ��  : fc_queue_recover_rate
 ��������  : ���Ӷ������ʻָ�Ϊ����ֵ
 �������  : FC_QUEUE_T *pstPQ  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��13�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

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
 �� �� ��  : fc_queue_skb_list_init
 ��������  : ��ʼ�����б��Ļ������б�
 �������  : FC_QUEUE_T *pstStaQ  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��11�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

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
 �� �� ��  : fc_queue_skb_list_destroy
 ��������  : ���ٶ��л���������
 �������  : FC_QUEUE_T *pstStaQ  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��11�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

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
 �� �� ��  : fc_queue_init
 ��������  : ���г�ʼ��
 �������  : FC_QUEUE_T *pstStaQ  
             FC_DIR_E enDir       
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��11�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

*****************************************************************************/
AP_ERROR_CODE_E fc_queue_init(FC_QUEUE_T *pstQ, FC_DIR_E enDir)
{
    if (NULL == pstQ)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue.\n");
        return AP_E_PARAM;
    }
    
    /*���û�������*/
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
 �� �� ��  : fc_queue_destroy
 ��������  : �������ٹ���
 �������  : FC_QUEUE_T *pstStaQ  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��11�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

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
    
    
    /*�����дӹ�ϣ����ɾ��*/
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
    //INIT_LIST_HEAD(&pstQ->stStaSet); /*���������ʼ��*/

    /* BEGIN: Added by xucongjiang, 2012/11/9 */
    /* ���ɾ�����к�ԭ�ȵ����ݻ����ڵ����� */
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
 �� �� ��  : fc_queue_update_token
 ��������  : ������ת����һ��ʱ���ڿɷ��͵��ֽ���
 �������  : FC_QUEUE_T *pstStaQ  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��11�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

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
	
	/*�����趨�����ʺ�ͻ����С����ʱ����ɶ��пɷ��͵��ֽ���*/
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
	
	/*���¼��ʱ���*/				
	pstQ->uiTimeCheckPoint = uiNow;
	
	return AP_E_NONE;
}

/*****************************************************************************
 �� �� ��  : fc_queue_get_ccache
 ��������  : ��ȡ���л�����
 �������  : FC_QUEUE_T *pstStaQ  
             UINT32 uiPktLen      
 �������  : ��
 �� �� ֵ  : inline
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��11�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

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
        
        /*������С��������¼*/
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
 �� �� ��  : fc_queue_add_ccache
 ��������  : ���Ӷ����еĻ������ռ�
 �������  : FC_QUEUE_T *pstStaQ  
             UINT32 uiPktLen      
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��11�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

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
    if (RX_HANDLER_CONSUMED == eRet)  /*���ͳɹ�*/
    {
        return AP_E_NONE;
    }
#elif  defined OPENWRT
    eRet = br_handle_frame(&pstSkb);
    if (RX_HANDLER_CONSUMED == eRet)  /*���ͳɹ�*/
    {
        return AP_E_NONE;
    }
#else
    pstSkb = br_handle_frame_hook(pstSkb->dev->br_port, pstSkb);
    if (NULL == pstSkb) /*���ͳɹ�*/
    {
        return AP_E_NONE;
    }
#endif

#else
    iRet = br_handle_frame_hook(pstSkb->dev->br_port, &pstSkb);
    if (1 == iRet)  /*���ͳɹ�*/
    {
        return AP_E_NONE;
    }
#endif

    /*����ʧ��*/
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
 �� �� ��  : fc_queue_pass_allowed
 ��������  : ȷ�������Ƿ�����ñ��ķ���
 �������  : FC_QUEUE_T *pstQ  --���нṹ
             struct sk_buff *skb  --�����͵ı���
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��11�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

*****************************************************************************/
FC_PKT_STATE_E fc_queue_pass_allowed(
    FC_QUEUE_T *pstQ, 
    struct sk_buff *pstSkb,
    FC_PASS_TYPE_T enPassType)
{
    UINT32 uiPktLen;
    INT32 uiNewToken; /*����Ϊ�з�������*/
    FC_PKT_STATE_E enPktState = FC_PASS_PKT;
    UINT32 skbQLen;
    
    //struct sk_buff *pstSkb = *ppstSkb;
    if (NULL == pstQ)
    {
        return FC_PASS_PKT;
    }
    
    /*����Ϊ0ʱ����ʾ����������*/
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
        pstQ->uiByteAvail = uiNewToken; /*���µ�ǰ�����ֽ���*/
                
        /*���Ĵ��ڶ�����,���͸ñ��ĺ����Ӷ��еĻ�������С*/
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
 �� �� ��  : fc_queue_need_update_auto_rate
 ��������  : �жϵ�ǰ�����Ƿ���Ҫ����
 �������  : FC_DIR_E enDir       
             INT32 uiBytePerTick  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��13�� ����һ
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

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
 �� �� ��  : fc_queue_auto_rate
 ��������  : ʵ���Զ��������ʵĹ���
 �������  : FC_DIR_E enDir  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��13�� ����һ
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

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
    
    /*����ǰһ���������*/
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
        /*����ͳ�Ƽ�¼*/
        pstQ[uiIndex].stStatsLast1Sec.uiByteTotal
                        = pstQ[uiIndex].stStatsCur.uiByteTotal;
        
        /*ͳ����Ч������Ŀ*/
        uiQNum++;
    }
    
    /*����ƽ��ֵ*/
    uiBytePerTick = uiTotalByteDiff / (uiQNum * HZ);
    
    /*����Ҫ��������*/
    if (!fc_queue_need_update_auto_rate(enDir, uiBytePerTick))
    {   
        return AP_E_NONE;
    }
    
    /*�޸Ķ�������*/
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
    
    /*������������*/
    g_auiBytePerTick[enDir] = uiBytePerTick;
    
    return AP_E_NONE; 
}



/*****************************************************************************
 �� �� ��  : fc_queue_get_list
 ��������  : ��ȡ��ǰ�Ķ����б�
 �������  : FC_IOCTL_ENTITY_LIST_T *pstEntityList
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��15�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

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
            
            /*����֮ǰ���ʹ��Ķ���*/
            if (uiSkipNum < uiStartNum)
            {
                //printk("skip [%d](%d)\n", pstQ->uiQId, enDir);
                uiSkipNum++;
                continue;
            }
            
            /*��ȡ�����㹻�Ķ���*/
            if (uiQNum >= FC_ENTITY_LIST_SIZE)
            {
                break;
            }
            
            /*��ȡEntity*/
            pstEntity = &(pstList->astEntity[uiQNum]);
            
            /*��ȡ��������*/
            pstEntity->stMatchAttrib = pstQ->stMatchAttrib;
            
            /*��ȡ��������*/
            pstEntity->astAdminRate[enDir].uiRate = TICKBYTE_TO_RATE(pstQ->uiAdminBytePerTick);
            pstEntity->astAdminRate[enDir].uiBurst = pstQ->uiAdminBurstSize;
            pstEntity->astRealRate[enDir].uiRate = TICKBYTE_TO_RATE(pstQ->uiBytePerTick);
            pstEntity->astRealRate[enDir].uiBurst = pstQ->uiBurstSize;
            
            /*��ȡͳ��*/
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

