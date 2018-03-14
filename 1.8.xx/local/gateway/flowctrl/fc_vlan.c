/******************************************************************************

                  ��Ȩ���� (C), 2001-2014, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : fc_vlan.c
  �� �� ��   : ����
  ��    ��   : 
  ��������   : 
  ����޸�   :
  ��������   : ������ص�hash����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 
    ��    ��   : 
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "fc_main.h"
#include "fc_queue.h"
#include "fc_hash.h"
#include "fc_vlan.h"

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
 �� �� ��  : fc_vlan_queue_init
 ��������  : VLAN���г�ʼ��
 �������  :   
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 
    ��    ��   : 
    �޸�����   : �����ɺ���

*****************************************************************************/
AP_ERROR_CODE_E fc_vlan_queue_init(FC_QUEUE_T *pstQ, FC_VLAN_T *pstVlan, FC_DIR_E enDir)
{
    if (NULL == pstQ || NULL == pstVlan)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue or VLAN data.\n");
        return AP_E_PARAM;
    }
    
    fc_queue_init(pstQ, enDir);
    
    /*����ƥ������*/
	pstQ->stMatchAttrib.iVid = pstVlan->vid;
    /*���ö�Ӧ��rule*/
    pstQ->enRule = FC_RULE_VLAN;
    
    /*�������������Ϣ*/
    fc_queue_update(pstQ, &pstVlan->astRate[enDir], FC_QUEUE_CREATE);
    
    return AP_E_NONE;
}

/*****************************************************************************
 �� �� ��  : fc_add_vlan
 ��������  : ��ӻ���VLAN�����ع���
 �������  : FC_VLAN_T *pstVlan  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 
    ��    ��   : 
    �޸�����   : �����ɺ���

*****************************************************************************/
AP_ERROR_CODE_E fc_add_vlan(FC_VLAN_T *pstVlan)
{
    FC_QUEUE_T *pstQ;
    FC_DIR_E enDir;
    AP_ERROR_CODE_E enRet;
	CHAR szVlanDev[IFNAMSIZ];
    UINT32 uiIndex;

    if (NULL == pstVlan)
    {
        return AP_E_PARAM;
    }
	
	snprintf(szVlanDev, IFNAMSIZ, "eth1.%d", pstVlan->vid);
    
    for (enDir = FC_DIR_DS; enDir < FC_DIR_MAX; enDir++)
    {
        pstQ = fc_get_vlan_queue_from_htable(szVlanDev, enDir);
        if (NULL == pstQ)
        {
            FC_LOG(FC_LOG_INFO, "Create VLAN.\n");
            /* ��ȡ����, ͬʱ��ʼ�����еķ����Ա*/
            pstQ = fc_find_avail_queue(enDir);
            if (NULL == pstQ)
            {
                FC_LOG(FC_LOG_ERR, "fc_find_avail_queue failed.\n");
                return AP_E_RESOURCE;
            }

            
            /*��ʼ������*/
            enRet = fc_vlan_queue_init(pstQ, pstVlan, enDir);
            if (AP_E_NONE != enRet)
            {
                FC_LOG(FC_LOG_ERR, "fc_vlan_queue_init failed. ret %d\n", enRet);
                return AP_E_DEFAULT_FAIL;
            }
            
            /*��������ӵ�VLAN hash����*/
            enRet = fc_add_vlan_queue_to_htable(pstQ, enDir);
            if (AP_E_NONE != enRet)
            {
                FC_LOG(FC_LOG_ERR, "fc_add_vlan_queue_to_htable failed. ret %d\n", enRet);
                return AP_E_DEFAULT_FAIL;
            }
            //vlan ����û��MULTI(��������)�� sta + vap/ssid/ap���������º�����ֱ�ӷ���
           
            /*�����Ӷ��м���*/
            //fc_update_child_queue_set(pstQ);
            /*�����Ӷ�������*/
            //fc_adjust_child_queue_rate(pstQ);
        }
        else
        {
            FC_LOG(FC_LOG_DEBUG, "Update VAP.\n");
            enRet = fc_queue_update(pstQ, &pstVlan->astRate[enDir], FC_QUEUE_UPDATE);
            if (AP_E_NONE != enRet)
            {
                FC_LOG(FC_LOG_ERR, "fc_vlan_queue_update failed. ret %d\n", enRet);
                return AP_E_DEFAULT_FAIL;
            }
            
            /*�����ֶ��м���*/
            //fc_update_child_queue_set(pstQ);
            /*�����Ӷ�������*/
            //fc_adjust_child_queue_rate(pstQ);
            
            //�������ڸ�vlan��sta���ʣ��滻���и���
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
                        pstVlan->vid == pstQ->stMatchAttrib.iVid)
                    {
                       fc_queue_update(pstQ, &pstVlan->astRate[enDir], FC_QUEUE_UPDATE);
                    }
                    pstQ++;  /*OK*/
                }
            }
            
        }
        
    }
    
    return AP_E_NONE;
}

/*****************************************************************************
 �� �� ��  : fc_del_sta_by_vlan
 ��������  : ɾ������vlan���Զ�����ʱ��ĳ��vlan�µ������ն����ٽṹ
 �������  : INT32 vid  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 
    ��    ��   : 
    �޸�����   : �����ɺ���

*****************************************************************************/
AP_ERROR_CODE_E fc_del_sta_by_vlan(INT32 vid)
{
    FC_DIR_E enDir;
    UINT32 uiIndex;
    FC_QUEUE_T *pstQ;
    FC_RULE_E enRule = fc_get_rule();

	if((vid <= 0) || (vid >= 4096))
		return AP_E_PARAM;

    if  ( !FC_RULE_IS_AUTO(enRule) )
    {
        return AP_E_NONE;
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
            if (pstQ->bInUse && 
                vid == pstQ->stMatchAttrib.iVid)
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
 �� �� ��  : fc_del_vlan
 ��������  : ɾ������VLAN�����ع���
 �������  : INT32 vid  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 
    ��    ��   : 
    �޸�����   : �����ɺ���

*****************************************************************************/
AP_ERROR_CODE_E fc_del_vlan(INT32 vid)
{
    FC_QUEUE_T *pstQ;
    FC_DIR_E enDir;
    AP_ERROR_CODE_E enRet;
	CHAR szVlanDev[IFNAMSIZ];

	if((vid <= 0) || (vid >= 4096))
		return AP_E_PARAM;

	snprintf(szVlanDev, IFNAMSIZ, "eth1.%d", vid);
    
    for (enDir = FC_DIR_DS; enDir < FC_DIR_MAX; enDir++)
    {
        pstQ = fc_get_vlan_queue_from_htable(szVlanDev, enDir);
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

    /* ɾ����vlan�µ�����sta */
    fc_del_sta_by_vlan(vid);
    
    return AP_E_NONE;
}


