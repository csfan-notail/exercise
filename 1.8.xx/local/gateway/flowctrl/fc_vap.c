/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : fc_vap.c
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
#include "fc_vap.h"

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
 �� �� ��  : fc_vap_queue_init
 ��������  : VAP���г�ʼ��
 �������  : FC_QUEUE_T *pstVapQ  
             FC_VAP_T *pstVaP     
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
AP_ERROR_CODE_E fc_vap_queue_init(FC_QUEUE_T *pstQ, FC_VAP_T *pstVaP, FC_DIR_E enDir)
{
    if (NULL == pstQ || NULL == pstVaP)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue or VAP data.\n");
        return AP_E_PARAM;
    }
    
    fc_queue_init(pstQ, enDir);
    
    /*����ƥ������*/
    strncpy(pstQ->stMatchAttrib.acVapName, pstVaP->acVapName, IFNAMSIZ);
    /*���ö�Ӧ��rule*/
    pstQ->enRule = FC_RULE_VAP;
    
    /*�������������Ϣ*/
    fc_queue_update(pstQ, &pstVaP->astRate[enDir], FC_QUEUE_CREATE);
    
    return AP_E_NONE;
}

/*****************************************************************************
 �� �� ��  : fc_add_vap
 ��������  : ��ӻ���VAP�����ع���
 �������  : FC_VAP_T *pstVaP  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��11�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

*****************************************************************************/
AP_ERROR_CODE_E fc_add_vap(FC_VAP_T *pstVaP)
{
    FC_QUEUE_T *pstQ;
    FC_DIR_E enDir;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstVaP)
    {
        return AP_E_PARAM;
    }
    
    for (enDir = FC_DIR_DS; enDir < FC_DIR_MAX; enDir++)
    {
        pstQ = fc_get_vap_queue_from_htable(pstVaP->acVapName, enDir);
        if (NULL == pstQ)
        {
            FC_LOG(FC_LOG_INFO, "Create VAP.\n");
            /* ��ȡ����, ͬʱ��ʼ�����еķ����Ա*/
            pstQ = fc_find_avail_queue(enDir);
            if (NULL == pstQ)
            {
                FC_LOG(FC_LOG_ERR, "fc_find_avail_queue failed.\n");
                return AP_E_RESOURCE;
            }
            
            /*��ʼ������*/
            enRet = fc_vap_queue_init(pstQ, pstVaP, enDir);
            if (AP_E_NONE != enRet)
            {
                FC_LOG(FC_LOG_ERR, "fc_vap_queue_init failed. ret %d\n", enRet);
                return AP_E_DEFAULT_FAIL;
            }
            
            /*��������ӵ�VAP hash����*/
            enRet = fc_add_vap_queue_to_htable(pstQ, enDir);
            if (AP_E_NONE != enRet)
            {
                FC_LOG(FC_LOG_ERR, "fc_add_vap_queue_to_htable failed. ret %d\n", enRet);
                return AP_E_DEFAULT_FAIL;
            }
            
            /*�����Ӷ��м���*/
            fc_update_child_queue_set(pstQ);
            /*�����Ӷ�������*/
            fc_adjust_child_queue_rate(pstQ);
        }
        else
        {
            FC_LOG(FC_LOG_DEBUG, "Update VAP.\n");
            enRet = fc_queue_update(pstQ, &pstVaP->astRate[enDir], FC_QUEUE_UPDATE);
            if (AP_E_NONE != enRet)
            {
                FC_LOG(FC_LOG_ERR, "fc_vap_queue_update failed. ret %d\n", enRet);
                return AP_E_DEFAULT_FAIL;
            }
            
            /*�����ֶ��м���*/
            fc_update_child_queue_set(pstQ);
            /*�����Ӷ�������*/
            fc_adjust_child_queue_rate(pstQ);
        }
        
    }
    
    return AP_E_NONE;
}

/*****************************************************************************
 �� �� ��  : fc_del_sta_by_vap
 ��������  : ɾ������vap���Զ�����ʱ��ĳ��vap�µ������ն����ٽṹ
 �������  : const CHAR *szVap  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��10��20��
    ��    ��   : xucongjiang
    �޸�����   : �����ɺ���

*****************************************************************************/
AP_ERROR_CODE_E fc_del_sta_by_vap(const CHAR *szVap)
{
    FC_DIR_E enDir;
    UINT32 uiIndex;
    INT32 iRadioId = FC_RID_INVALID;
    INT32 iWlanId = FC_WID_INVALID;
    FC_QUEUE_T *pstQ;
    FC_RULE_E enRule = fc_get_rule();

    if  ( NULL == szVap )
    {
        return AP_E_NULLPTR;
    }

    if  ( !FC_RULE_IS_AUTO(enRule) )
    {
        return AP_E_NONE;
    }

    sscanf(szVap, "ap%d_%d", &iRadioId, &iWlanId);
    
    FC_LOG(FC_LOG_INFO, "RadioId=%d WlanId=%d\n", iRadioId, iWlanId);
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
            if (pstQ->bInUse && 
                iRadioId == pstQ->stMatchAttrib.iRadioId &&
                iWlanId == pstQ->stMatchAttrib.iWlanId
               )
            {
                FC_LOG(FC_LOG_INFO, "delete sta\n");
                //pstQ->bInUse = FALSE;
                //fc_del_sta_queue_from_htable(pstQ);
                fc_queue_destroy(pstQ);
            }
            pstQ++;  /*OK*/
        }
        
    }
    
    return AP_E_NONE;
}




/*****************************************************************************
 �� �� ��  : fc_del_vap
 ��������  : ɾ������VAP�����ع���
 �������  : CHAR *szVapName  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��11�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

*****************************************************************************/
AP_ERROR_CODE_E fc_del_vap(CHAR *szVapName)
{
    FC_QUEUE_T *pstQ;
    FC_DIR_E enDir;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == szVapName)
    {
        return AP_E_PARAM;
    }
    
    for (enDir = FC_DIR_DS; enDir < FC_DIR_MAX; enDir++)
    {
        pstQ = fc_get_vap_queue_from_htable(szVapName, enDir);
        if (NULL == pstQ)
        {
            FC_LOG(FC_LOG_ERR, "VAP not exists!\n");
            return AP_E_RESOURCE;
        }
        
        /*�ָ��Ӷ�������ΪĬ��ֵ*/
        fc_queue_recover_rate(pstQ);
        
        /*���ٶ���*/
        enRet = fc_queue_destroy(pstQ);
        if (AP_E_NONE != enRet)
        {
            FC_LOG(FC_LOG_ERR, "fc_queue_destroy failed. ret %d\n", enRet);
            return AP_E_DEFAULT_FAIL;
        }
        
    }

    /* BEGIN: Modified by xucongjiang, 2012/10/20 */
    /* ɾ����vap�µ�����sta */
    fc_del_sta_by_vap(szVapName);
    /* END:   Modified by xucongjiang, 2012/10/20   PN: */
    
    return AP_E_NONE;
}


