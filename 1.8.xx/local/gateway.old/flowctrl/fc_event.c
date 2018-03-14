/******************************************************************************

                  ��Ȩ���� (C), 2010-2012, �Ϻ�徴�ͨ�ſƼ��ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : fc_event.c
  �� �� ��   : ����
  ��    ��   : xucongjiang
  ��������   : 2012��11��8��
  ����޸�   :
  ��������   : �ں������豸֪ͨ��������ش���
  �����б�   :
              fc_del_skb_by_port
              fc_device_event
  �޸���ʷ   :
  1.��    ��   : 2012��11��8��
    ��    ��   : xucongjiang
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "fc_main.h"
#include "fc_queue.h"
/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/
LOCAL INT32 fc_device_event(struct notifier_block *unused, unsigned long event, void *ptr);
/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/
struct notifier_block fc_device_notifier = 
{
    .notifier_call = fc_device_event
};

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/



/*****************************************************************************
 �� �� ��  : fc_del_skb_by_port
 ��������  : ɾ������skb��������ĳ���˿��µ�skb
 �������  : struct net_bridge_port *pstPort  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��10��20��
    ��    ��   : xucongjiang
    �޸�����   : �����ɺ���

*****************************************************************************/
LOCAL AP_ERROR_CODE_E fc_del_skb_by_port(struct net_bridge_port *pstPort)
{
    FC_DIR_E enDir;
    UINT32 uiIndex;
    UINT32 uiPktLen;
    FC_QUEUE_T *pstQ;
    struct sk_buff *pstSkb = NULL;	
    struct sk_buff *pstSkbTmp = NULL;

    
    if (!fc_get_admin()) /*���ر��ر�*/
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
                /*�������л�����*/
                skb_queue_walk_safe(&pstQ->stSkbList, pstSkb, pstSkbTmp)
                {
                    if  ( 0 == strncmp(pstSkb->dev->name, pstPort->dev->name, IFNAMSIZ) )
                    {
                    #if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,15)) /* 2012-10-26 add by zhangping */
                        skb_unlink(pstSkb);
                    #else
                        skb_unlink(pstSkb, &pstQ->stSkbList);
                    #endif

                        /*����ͳ��*/
                        uiPktLen = fc_get_skb_len(pstSkb);
                        FC_UPDATE_STATS_DROP(pstQ, uiPktLen);

                        /* �����ñ��� */
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
 �� �� ��  : fc_device_event
 ��������  : ֪ͨ���¼�����
 �������  : struct notifier_block *unused  
             unsigned long event            
             void *ptr                      
 �������  : ��
 �� �� ֵ  : LOCAL
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��11��8��
    ��    ��   : xucongjiang
    �޸�����   : �����ɺ���

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

