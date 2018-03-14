/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : fc_main.c
  �� �� ��   : ����
  ��    ��   : Zhong Huaqing
  ��������   : 2012��8��10�� ������
  ����޸�   :
  ��������   : AP����ģ��
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

#include "fc_queue.h"
#include "fc_hash.h"
#include "fc_timer.h"

#include "fc_sta.h"
#include "fc_vap.h"
#include "fc_ssid.h"
#include "fc_ap.h"
#include "fc_white_list.h"
/*----------------------------------------------*
 * �궨��                                *
 *----------------------------------------------*/
//#define FC_DISABLE_TIMER 1  /*���ö��з��Ͷ�ʱ��*/

/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/
REMOTE FC_PKT_STATE_E (*fc_enqueue_hook)(struct sk_buff *pstSkb, FC_DIR_E enDir);

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/
BOOL g_bDebugLevel = FC_LOG_ERR;

BOOL g_bDequeueFast = FALSE;
module_param(g_bDequeueFast, uint, 0600);

BOOL g_bThresholdEnable = TRUE;
module_param(g_bThresholdEnable, uint, 0600);
/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/
 
LOCAL BOOL g_bAdmin = FALSE;
LOCAL FC_RULE_E g_enRule = FC_RULE_STA;

LOCAL BOOL g_bAutoRatenable = FALSE;  /*�Զ���������*/

//LOCAL FC_QUEUE_AUTO_LEARN_T g_stAutoLearn;

LOCAL UINT32 g_uiGearByte = 0;
/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                       *
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
    
    g_enRule = enRule; /*��������*/
    
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
    
    if (bStatus) /*�����Զ�����*/
    {
        stAutoLearn.bIsOn = FALSE;
        fc_set_auto_learn(&stAutoLearn);
    }
    else    /*�ر��Զ�����*/
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

/*���ڲ���brige_port�е�ssid�ֶ�*/
inline CHAR*fc_get_ssid_name(struct sk_buff *pstSkb, FC_DIR_E enDir)
{

    #if (defined(GW_X86) || defined(GW_ARM) || defined(OPENWRT))     //����x86�ں� by cj
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
 �� �� ��  : fc_cmp_queue_with_pkt
 ��������  : �Զ�ѧϰʱ���������sta�����ٽṹ�����ж�vap/ssid/vlan�Ƿ���skb�е�
             ��ͬ������ͬ����sta�����ٽṹɾ��
 �������  : struct sk_buff *pstSkb  
             FC_QUEUE_T *pstQ        
             FC_DIR_E enDir          
 �������  : ��
 �� �� ֵ  : LOCAL
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��10��20��
    ��    ��   : xucongjiang
    �޸�����   : �����ɺ���

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
		
		if	( vid != pstQ->stMatchAttrib.iVid ) /*�ն˱��Ĵ�����һ��vlan������*/
		{
			fc_queue_destroy(pstQ);
			return NULL;
		}
	}

    return pstQ;
}

/*****************************************************************************
 �� �� ��  : fc_get_queue_by_pkt
 ��������  : ��ȡ���Ķ�Ӧ�Ķ��нṹ
 �������  : struct sk_buff *pstSkb  
             FC_DIR_E enDir          
 �������  : ��
 �� �� ֵ  : FC_QUEUE_T
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��11�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

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
    
    /*�༶�������Զ�����ѧϰ���ܣ����ǻ���STA����*/
    if (FC_RULE_IS_MULTI(enRule)
        || FC_RULE_IS_AUTO(enRule))
    {
        enRule = FC_RULE_STA; /*������STA��ͬ*/
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
        /*��ȡ�������������*/
        pstSta->astRate[enDir].uiRate = TICKBYTE_TO_RATE(pstPQ->uiAdminBytePerTick);
        pstSta->astRate[enDir].uiBurst = TICKBYTE_TO_RATE(pstPQ->uiAdminBurstSize);
        
    }
    
    
    return AP_E_NONE;
}
#endif

/*****************************************************************************
 �� �� ��  : fc_auto_learn
 ��������  : ʵ�����ع����Զ�ѧϰ����
 �������  : struct sk_buff *pstSkb  
             FC_DIR_E enDir          
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��13�� ����һ
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

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
    /*�Զ�ѧϰ����δ����*/
    if (!(FC_RULE_AUTO & enRule))
    {
        return AP_E_NONE;
    }
    
    /*��ȡSTA��MAC��ַ*/
    pcMac = fc_get_sta_mac(pstSkb, enDir);
    if(pcMac == NULL)
	 return AP_E_PARAM;
	
    if (0x01 & pcMac[0]) /*�鲥MAC��ַ*/
    {
        FC_LOG(FC_LOG_DEBUG, "STA MAC Type is Multicast or Broadcast.\n");
        return AP_E_DEFAULT_FAIL;
    }
    memcpy(stSta.acMac, pcMac, ETH_ALEN);
    
    /*��ȡ�Զ�ѧϰ������*/
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
		/*��ȡvlan id*/
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
    
/* BEGIN: Modified by xucongjiang, 2012/11/7   ���ⵥ��:CPE���ն˺ܿ����Ǵ����߶˿ڽ���� */
//#if 1  
    /*��ȡradio id ��wlan id*/
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
 �� �� ��  : fc_enqueue
 ��������  : ������Ӵ���
 �������  : struct sk_buff *pstSkb  --������ı���
             FC_DIR_E enDir          --����������
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��11�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

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
    
	/*���������*/
	if (FC_ENQ_FLAG_VALID(pstSkb))
	{
	    return FC_PASS_PKT;
	}
	
    if (!fc_get_admin()) /*���ر��ر�*/
    {   
        return FC_PASS_PKT;
    }
    /* BEGIN: Modified by xucongjiang, 2012/11/7   PN:CPE���ض����޸� */
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
    /*��ȡ���нṹ*/
    pstQ = fc_get_queue_by_pkt(pstSkb, enDir);
    
    if (NULL == pstQ) /* ���в����� */
    {
        /*�����Զ����*/
        fc_auto_learn(pstSkb, enDir);
        return FC_PASS_PKT;
    }
    

    /*ȷ�������Ƿ�����ͨ��*/
    enPktState = fc_queue_pass_allowed(pstQ, pstSkb, FC_PASS_FAST);
    if (FC_PASS_PKT == enPktState)
    {
        FC_LOG(FC_LOG_DEBUG, "[%d](%d): Pass %d bytes.\n", 
                                pstQ->uiQId, enDir, uiPktLen);
        FC_UPDATE_STATS_PASS_FAST(pstQ, uiPktLen);
        
        //*ppstSkb = pstSkb; 
        return FC_PASS_PKT;
    }    
    
       
    
    /*�������*/
    enEnqState = fc_queue_get_ccache(pstQ, uiPktLen);
    if (FC_ENQ_PKT == enEnqState) /*�������*/
    {
        FC_LOG(FC_LOG_DEBUG, "[%d](%d): Enqueue %d bytes.\n", 
                                pstQ->uiQId, enDir, uiPktLen);
        SET_FC_ENQ_FLAG(pstSkb);  /*��־�����Ѿ����*/
        skb_queue_tail(&pstQ->stSkbList, pstSkb); /*��������ӵ����еĻ�������*/
        
        /*�������еı��ķ��Ͷ�ʱ��*/
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
        kfree_skb(pstSkb); /*�ͷű��Ŀռ�*/
        //printk("[%d](%d):Drop %d bytes\n", pstQ->uiQId, enDir, uiPktLen);
        FC_UPDATE_STATS_DROP(pstQ, uiPktLen);
    }
    
    return enEnqState;
}

/*****************************************************************************
 �� �� ��  : fc_dequeue
 ��������  : ���ж�ʱ�����ķ��ʹ�����̡����������ṩ�����з��Ͷ�ʱ��ʹ�á�
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
    
    /*�������л�����*/
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
        
        /*����ͳ��*/
        FC_UPDATE_STATS_PASS_DELAY(pstQ, uiPktLen);
        uiSendTotal += uiPktLen;
    }
    
    return uiSendTotal;
}


/*****************************************************************************
 �� �� ��  : flowctrl_init
 ��������  : ��ʼ��
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��11�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

*****************************************************************************/
AP_ERROR_CODE_E flowctrl_init(void)
{
    AP_ERROR_CODE_E enRet;
    
    enRet = fc_hash_init();
    
    return enRet;
}

