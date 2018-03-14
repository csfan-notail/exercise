/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : fc_ap.c
  �� �� ��   : ����
  ��    ��   : Zhong Huaqing
  ��������   : 2012��8��10�� ������
  ����޸�   :
  ��������   : AP�������ز���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��8��10�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ļ�

  2.��    ��   : 2012��11��9��
    ��    ��   : xucongjiang
    �޸�����   : ���ap auto�������ص�bug
    
******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "fc_main.h"
#include "fc_queue.h"
#include "fc_ap.h"

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


AP_ERROR_CODE_E fc_ap_queue_init(FC_QUEUE_T *pstQ, FC_AP_T *pstAp, FC_DIR_E enDir)
{
    if (NULL == pstQ || NULL == pstAp)
    {
        return AP_E_PARAM;
    }
    
    fc_queue_init(pstQ, enDir);
    
    /*���ö�Ӧ��rule*/
    pstQ->enRule = FC_RULE_AP;
    
    /*�������������Ϣ*/
    fc_queue_update(pstQ, &pstAp->astRate[enDir], FC_QUEUE_CREATE);
    
    return AP_E_NONE;
}

/*****************************************************************************
 �� �� ��  : fc_add_ap
 ��������  : ��ӻ���AP�����ع���
 �������  : FC_VAP_T *pstAp  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��11�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��11��9��
    ��    ��   : xucongjiang
    �޸�����   : ap autu���������
*****************************************************************************/
AP_ERROR_CODE_E fc_add_ap(FC_AP_T *pstAp)
{
    FC_QUEUE_T *pstQ;
    FC_DIR_E enDir;
    AP_ERROR_CODE_E enRet;
    FC_RULE_E enRule;
    
    if (NULL == pstAp)
    {
        return AP_E_PARAM;
    }

    /* BEGIN: Added by xucongjiang, 2012/11/9 ���ap auto�����������������ʲ���Ч������ */
    enRule = fc_get_rule();
    if ((FC_RULE_AUTO & enRule) && (FC_RULE_AP & enRule))
    {
        fc_queue_flush_all();
    }
    /* END:   Added by xucongjiang, 2012/11/9   PN: */
    
    for (enDir = FC_DIR_DS; enDir < FC_DIR_MAX; enDir++)
    {
        pstQ = fc_get_ap_queue(enDir);
        if (NULL == pstQ )
        {
            FC_LOG(FC_LOG_ERR, "Invalid Queue.\n");
            return AP_E_RESOURCE;
        }
        if (!pstQ->bInUse) /*����δ��ʹ��*/
        {
            FC_LOG(FC_LOG_INFO, "Create AP.\n");
            /*��ʼ������*/
            enRet = fc_ap_queue_init(pstQ, pstAp, enDir);

            if (AP_E_NONE != enRet)
            {
                FC_LOG(FC_LOG_ERR, "fc_vap_queue_init failed. ret %d\n", enRet);
                return AP_E_DEFAULT_FAIL;
            }
            
            /*�����ֶ��м���*/
            fc_update_child_queue_set(pstQ);
            /*�����Ӷ�������*/
            fc_adjust_child_queue_rate(pstQ);
        }
        else /*�����ѱ�ʹ��*/
        {
            FC_LOG(FC_LOG_INFO, "Update AP.\n");
            enRet = fc_queue_update(pstQ, &pstAp->astRate[enDir], FC_QUEUE_UPDATE);

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
 �� �� ��  : fc_del_ap
 ��������  : ɾ������AP�����ع���
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��11�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��11��9��
    ��    ��   : xucongjiang
    �޸�����   : ap auto
*****************************************************************************/
AP_ERROR_CODE_E fc_del_ap(void)
{
    FC_QUEUE_T *pstQ;
    FC_DIR_E enDir;
    AP_ERROR_CODE_E enRet;
    FC_RULE_E enRule;
    
    /* BEGIN: Added by xucongjiang, 2012/11/9  */
    /* ���ap auto������ɾ��AP����ʱ���ն����ٽṹδ��ɾ�������� */
    enRule = fc_get_rule();
    if ((FC_RULE_AUTO & enRule) && (FC_RULE_AP & enRule))
    {
        return fc_queue_flush_all();
    }
    /* END:   Added by xucongjiang, 2012/11/9   PN: */
    
    for (enDir = FC_DIR_DS; enDir < FC_DIR_MAX; enDir++)
    {
        pstQ = fc_get_ap_queue(enDir);
        
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
    
    return AP_E_NONE;
}

