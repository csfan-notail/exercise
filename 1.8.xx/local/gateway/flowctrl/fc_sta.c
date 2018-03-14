/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : fc_sta.c
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
#include "fc_queue.h"
#include "fc_hash.h"
#include "fc_sta.h"

/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/
 


/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                       *
 *----------------------------------------------*/

/*****************************************************************************
 �� �� ��  : fc_sta_queue_init
 ��������  : STA���г�ʼ��
 �������  : FC_QUEUE_T *pstStaQ  
             FC_STA_T *pstSta     
             FC_DIR_E enDir       
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��17�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

*****************************************************************************/
AP_ERROR_CODE_E fc_sta_queue_init(FC_QUEUE_T *pstQ, FC_STA_T *pstSta, FC_DIR_E enDir)
{
    if (NULL == pstQ || NULL == pstSta)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue or STA data.\n");
        return AP_E_PARAM;
    }
    
    fc_queue_init(pstQ, enDir);
    
    /*����ƥ������*/
    memcpy(pstQ->stMatchAttrib.acStaMac, pstSta->acMac, ETH_ALEN);
    /*���ö�Ӧ��rule*/
    pstQ->enRule = FC_RULE_STA;
    
    /*�������������Ϣ*/
    fc_queue_update(pstQ, &pstSta->astRate[enDir], FC_QUEUE_CREATE);
    
    return AP_E_NONE;
}





AP_ERROR_CODE_E fc_update_sta_parent_info(FC_QUEUE_T *pstQ, FC_STA_T *pstSta)
{
    FC_QUEUE_T * pstPQ;
    FC_QUEUE_T * pstQTmp;
    FC_RULE_E enRule;
    
    if (NULL == pstQ || NULL == pstSta)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue or STA data.\n");
        return AP_E_PARAM;
    }
    
    enRule = fc_get_rule();
    if (!FC_RULE_IS_MULTI(enRule)
        && !FC_RULE_IS_AUTO(enRule))
    {
        return AP_E_NONE;
    }
    
    /*ֻ���ն�ģʽ���ж�Ӧ�ĸ�����*/
    if (FC_RULE_STA != pstQ->enRule)
    {
        return AP_E_NONE;
    }
    
    
    if (FC_QID_INVALID != pstQ->iPQId) /*֮ǰ�����*/
    {
        #if 0
        if (pstQ->stMatchAttrib.iRadioId != pstSta->iRadioId
            || pstQ->stMatchAttrib.iWlanId != pstSta->iWlanId) /*Parent ���޸�*/
        {
            list_del(&pstQ->stStaSet); 
            pstQ->iPQId = FC_QID_INVALID;  /*��������Ϊ��Чֵ*/
        }
        else
        {
            return AP_E_NONE; /*�����޸�*/
        }
        #endif
        
        list_del(&pstQ->stStaSet); 
    }
    
    /*�����ն˵�rid��wid��Ϣ*/
    pstQ->stMatchAttrib.iRadioId = pstSta->iRadioId;
    pstQ->stMatchAttrib.iWlanId = pstSta->iWlanId;

	/*�����ն˵�vlan��Ϣ*/
	pstQ->stMatchAttrib.iVid = pstSta->iVid;
    
    /*��ȡparent ����*/
    pstPQ = fc_get_parent_queue(pstQ);
    if (NULL == pstPQ)
    {
        return AP_E_NONE;
    }
	
	if(!(enRule & FC_RULE_VLAN)){
	    FC_LOG(FC_LOG_INFO, "Qid %d find parent , qid %d, vap %s, ssid %s\n",
	        pstQ->uiQId,
	        pstPQ->uiQId,
	        pstPQ->stMatchAttrib.acVapName,
	        pstPQ->stMatchAttrib.acSsidName
	        );
	    
	    
	    strncpy(pstQ->stMatchAttrib.acVapName, 
	            pstPQ->stMatchAttrib.acVapName, 
	            IFNAMSIZ);
	    strncpy(pstQ->stMatchAttrib.acSsidName, 
	            pstPQ->stMatchAttrib.acSsidName, 
	            SSID_NAMSIZ);
	}
    
    /*����AUTO�͹���ֻ��Ҫ�����Ӷ��У�����Ҫ���¸�������Ϣ*/
    if (FC_RULE_IS_AUTO(enRule))
    {
        return AP_E_NONE;
    }
            
    pstQ->iPQId = pstPQ->uiQId;
    list_add_tail(&pstQ->stStaSet, &pstPQ->stStaSet);
    //pstPQ->uiStaNum++;
    
    fc_adjust_child_queue_rate(pstPQ);
    
    list_for_each_entry(pstQTmp, &pstPQ->stStaSet, stStaSet)
    {
        FC_LOG(FC_LOG_INFO, "Queue %d include: %d \n", pstPQ->uiQId, pstQTmp->uiQId);
    }
    
    return AP_E_NONE;
}


AP_ERROR_CODE_E fc_del_sta_parent_info(FC_QUEUE_T *pstQ)
{
    FC_QUEUE_T *pstPQ;
    
    if (NULL == pstQ)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue.\n");
        return AP_E_PARAM;
    }
    
    pstQ->iPQId = FC_QID_INVALID;
    list_del(&pstQ->stStaSet);
    
    pstPQ = fc_get_parent_queue(pstQ);
    if (NULL != pstPQ)
    {
        return fc_adjust_child_queue_rate(pstPQ);
    }
    
    return AP_E_NONE;
}


/*****************************************************************************
 �� �� ��  : fc_add_sta
 ��������  : ��ӻ���STA�����ع���
 �������  : FC_STA_T *pstSta  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��11�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

*****************************************************************************/
AP_ERROR_CODE_E fc_add_sta(FC_STA_T *pstSta)
{
    FC_QUEUE_T *pstQ;
    FC_DIR_E enDir;
    AP_ERROR_CODE_E enRet;

    if (NULL == pstSta)
    {
        return AP_E_PARAM;
    }
    
    for (enDir = FC_DIR_DS; enDir < FC_DIR_MAX; enDir++)
    {
        pstQ = fc_get_sta_queue_from_htable(pstSta->acMac, enDir);
        if (NULL == pstQ)
        {
            FC_LOG(FC_LOG_INFO, "Create STA.\n");
            /* ��ȡ����, ͬʱ��ʼ�����еķ����Ա*/
            pstQ = fc_find_avail_queue(enDir);
            if (NULL == pstQ)
            {
                FC_LOG(FC_LOG_ERR, "fc_find_avail_queue failed.\n");
                return AP_E_RESOURCE;
            }
            
            /*��ʼ������*/
            enRet = fc_sta_queue_init(pstQ, pstSta, enDir);
            if (AP_E_NONE != enRet)
            {
                FC_LOG(FC_LOG_ERR, "fc_sta_queue_init failed. ret %d\n", enRet);
                return AP_E_DEFAULT_FAIL;
            }
            
            /*��������ӵ�STA hash����*/
            enRet = fc_add_sta_queue_to_htable(pstQ, enDir);
            if (AP_E_NONE != enRet)
            {
                FC_LOG(FC_LOG_ERR, "fc_add_sta_queue_to_htable failed. ret %d\n", enRet);
                return AP_E_DEFAULT_FAIL;
            }
            
            /*������һ��������*/
            fc_update_sta_parent_info(pstQ, pstSta);
        }
        else
        {
            FC_LOG(FC_LOG_INFO, "Update STA.\n");
            enRet = fc_queue_update(pstQ, &pstSta->astRate[enDir], FC_QUEUE_UPDATE);
            if (AP_E_NONE != enRet)
            {
                FC_LOG(FC_LOG_ERR, "fc_sta_queue_update failed. ret %d\n", enRet);
                return AP_E_DEFAULT_FAIL;
            }
            
            /*������һ��������*/
            fc_update_sta_parent_info(pstQ, pstSta);
            
        }
        
    }
    
    return AP_E_NONE;
}

/*****************************************************************************
 �� �� ��  : fc_del_sta
 ��������  : ɾ������STA�����ع���
 �������  : UINT8 *pcMac  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��11�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

*****************************************************************************/
AP_ERROR_CODE_E fc_del_sta(UINT8 *pcMac)
{
    FC_QUEUE_T *pstQ;
    FC_DIR_E enDir;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pcMac)
    {
        return AP_E_PARAM;
    }
    
    for (enDir = FC_DIR_DS; enDir < FC_DIR_MAX; enDir++)
    {
        pstQ = fc_get_sta_queue_from_htable(pcMac, enDir);
        if (NULL == pstQ)
        {
            FC_LOG(FC_LOG_ERR, "STA not exists!\n");
            return AP_E_RESOURCE;
        }
        
        fc_del_sta_parent_info(pstQ);
        
        enRet = fc_queue_destroy(pstQ);
        if (AP_E_NONE != enRet)
        {
            FC_LOG(FC_LOG_ERR, "fc_queue_destroy failed. ret %d\n", enRet);
            return AP_E_DEFAULT_FAIL;
        }
    }
    
    return AP_E_NONE;
}

