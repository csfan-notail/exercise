/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : fc_ssid.c
  �� �� ��   : ����
  ��    ��   : Zhong Huaqing
  ��������   : 2012��8��10�� ������
  ����޸�   :
  ��������   : ����ssid�����ز���
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
#include "fc_ssid.h"


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
 * �����                                   *
 *----------------------------------------------*/


/*----------------------------------------------*
 * ��������                                       *
 *----------------------------------------------*/

/*****************************************************************************
 �� �� ��  : fc_ssid_queue_init
 ��������  : SSID���г�ʼ��
 �������  : FC_QUEUE_T *pstQ    
             FC_SSID_T *pstSsid  
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
AP_ERROR_CODE_E fc_ssid_queue_init(FC_QUEUE_T *pstQ, FC_SSID_T *pstSsid, FC_DIR_E enDir)
{
    if (NULL == pstQ || NULL == pstSsid)
    {
        return AP_E_PARAM;
    }
    
    fc_queue_init(pstQ, enDir);
    
    /*����ƥ������*/
    strncpy(pstQ->stMatchAttrib.acSsidName, pstSsid->acSsidName, SSID_NAMSIZ);
    /*���ö�Ӧ��rule*/
    pstQ->enRule = FC_RULE_SSID;
    
    /*�������������Ϣ*/
    fc_queue_update(pstQ, &pstSsid->astRate[enDir], FC_QUEUE_CREATE);
    
    return AP_E_NONE;
}

/*****************************************************************************
 �� �� ��  : fc_add_ssid
 ��������  : ��ӻ���VAP�����ع���
 �������  : FC_SSID_T *pstSsid  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��11�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

*****************************************************************************/
AP_ERROR_CODE_E fc_add_ssid(FC_SSID_T *pstSsid)
{
    FC_QUEUE_T *pstQ;
    FC_DIR_E enDir;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstSsid)
    {
        return AP_E_PARAM;
    }
    
    for (enDir = FC_DIR_DS; enDir < FC_DIR_MAX; enDir++)
    {
        pstQ = fc_get_ssid_queue_from_htable(pstSsid->acSsidName, enDir);
        if (NULL == pstQ)
        {
            FC_LOG(FC_LOG_INFO, "Create SSID.\n");
            /* ��ȡ����, ͬʱ��ʼ�����еķ����Ա*/
            pstQ = fc_find_avail_queue(enDir);
            if (NULL == pstQ)
            {
                FC_LOG(FC_LOG_ERR, "fc_find_avail_queue failed.\n");
                return AP_E_RESOURCE;
            }
            
            /*��ʼ������*/
            enRet = fc_ssid_queue_init(pstQ, pstSsid, enDir);
            if (AP_E_NONE != enRet)
            {
                FC_LOG(FC_LOG_ERR, "fc_ssid_queue_init failed. ret %d\n", enRet);
                return AP_E_DEFAULT_FAIL;
            }
            
            /*��������ӵ�SSID hash����*/
            enRet = fc_add_ssid_queue_to_htable(pstQ, enDir);
            if (AP_E_NONE != enRet)
            {
                FC_LOG(FC_LOG_ERR, "fc_add_ssid_queue_to_htable failed. ret %d\n", enRet);
                return AP_E_DEFAULT_FAIL;
            }
            
            /*�����ֶ��м���*/
            fc_update_child_queue_set(pstQ);
            /*�����Ӷ�������*/
            fc_adjust_child_queue_rate(pstQ);
        }
        else
        {
            FC_LOG(FC_LOG_INFO, "Update SSID.\n");
            enRet = fc_queue_update(pstQ, &pstSsid->astRate[enDir], FC_QUEUE_UPDATE);
            if (AP_E_NONE != enRet)
            {
                FC_LOG(FC_LOG_ERR, "fc_ssid_queue_update failed. ret %d\n", enRet);
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
AP_ERROR_CODE_E fc_del_ssid(CHAR *szSsidName)
{
    FC_QUEUE_T *pstQ;
    FC_DIR_E enDir;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == szSsidName)
    {
        return AP_E_PARAM;
    }
    
    for (enDir = FC_DIR_DS; enDir < FC_DIR_MAX; enDir++)
    {
        pstQ = fc_get_ssid_queue_from_htable(szSsidName, enDir);
        if (NULL == pstQ)
        {
            FC_LOG(FC_LOG_ERR, "ssid not exists!\n");
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
    
    return AP_E_NONE;
}

/*****************************************************************************
 �� �� ��  : fc_get_dev_by_name
 ��������  : ����Linux�ں˰汾��ʹ�ò�ͬ��ϵͳ�ӿڻ�ȡ�豸�ṹ
 �������  : CHAR *szDevName  
 �������  : ��
 �� �� ֵ  : struct
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��12�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

*****************************************************************************/
struct net_device *fc_get_dev_by_name(CHAR *szDevName)
{
    if (NULL == szDevName)
    {
        return NULL;
    }
    
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,20))    
    return dev_get_by_name(&init_net, szDevName);
#else
    return dev_get_by_name(szDevName);
#endif
}

/*****************************************************************************
 �� �� ��  : fc_set_dev_ssid
 ��������  : Ϊĳ���豸����SSID�ַ���Ϣ
 �������  : CHAR *szSsidName  
             CHAR *szDevName   
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��23�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

*****************************************************************************/
AP_ERROR_CODE_E fc_set_dev_ssid(CHAR *szDevName, CHAR *szSsidName)
{
#if (defined(GW_X86) || defined(GW_ARM) || defined(OPENWRT))
    return AP_E_NONE;   
#else

    struct net_device *pstDev;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == szSsidName || NULL == szDevName)
    {
        return AP_E_PARAM;
    }
    
    pstDev = fc_get_dev_by_name(szDevName);
    if (NULL == pstDev)
    {
        FC_LOG(FC_LOG_ERR, "Failed to find dev: %s\n", szDevName);
        return AP_E_RESOURCE;
    }
    
    if (NULL == pstDev->br_port)
    {
        FC_LOG(FC_LOG_ERR, "Port is not under bridge.\n");
        enRet = AP_E_RESOURCE;
        goto end;
    }
    
    strncpy(pstDev->br_port->acSsid, szSsidName, SSID_NAMSIZ);

end:    
    dev_put(pstDev);
    return AP_E_NONE;
    
#endif    
}

/*****************************************************************************
 �� �� ��  : fc_get_dev_ssid
 ��������  : ��ȡĳ���豸��SSID�ַ���Ϣ
 �������  : CHAR *szSsidName  
             CHAR *szDevName   
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��23�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

*****************************************************************************/
AP_ERROR_CODE_E fc_get_dev_ssid(CHAR *szDevName, CHAR *szSsidName)
{
#if (defined(GW_X86) || defined(GW_ARM)|| defined(OPENWRT))
        return AP_E_NONE;   
#else

    struct net_device *pstDev;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == szSsidName || NULL == szDevName)
    {
        return AP_E_PARAM;
    }
    
    pstDev = fc_get_dev_by_name(szDevName);
    if (NULL == pstDev)
    {
        FC_LOG(FC_LOG_ERR, "Failed to find dev: %s\n", szDevName);
        return AP_E_RESOURCE;
    }
    
    if (NULL == pstDev->br_port)
    {
        FC_LOG(FC_LOG_ERR, "Port is not under bridge.\n");
        enRet = AP_E_RESOURCE;
        goto end;
    }
    
    strncpy(szSsidName, pstDev->br_port->acSsid, SSID_NAMSIZ);

end:
    dev_put(pstDev);
    return AP_E_NONE;
    
#endif    
}

AP_ERROR_CODE_E fc_get_dev_ssid_list(FC_IOCTL_DEV_SSID_LIST_T *pstDevSsidList)
{
#if (defined(GW_X86) || defined(GW_ARM)|| defined(OPENWRT))
        return AP_E_NONE;   
#else

    struct net_device *pstDev;
    FC_IOCTL_DEV_SSID_T *pstDevSsid;
    UINT32 uiNum = 0;
    
    if (pstDevSsidList)
    {
        return AP_E_PARAM;
    }
    
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,20))
    for_each_netdev(&init_net, pstDev)
#else
    for (pstDev = dev_base; pstDev; pstDev = pstDev->next)
#endif
    {
        if (uiNum >= FC_MAX_VAP_NUM)
        {
            break;
        }
        
        if (NULL == pstDev->br_port)
        {
            continue;
        }
        if (NULL == pstDev->br_port->acSsid)
        {
            continue;
        }
        
        pstDevSsid = &pstDevSsidList->astDevSsidList[uiNum];
        strncpy(pstDevSsid->acDevName, pstDev->name, IFNAMSIZ);
        strncpy(pstDevSsid->acSsid, pstDev->br_port->acSsid, SSID_NAMSIZ);
        uiNum++;
        
    }
    
    pstDevSsidList->uiNum = uiNum;
    
    return AP_E_NONE;
    
#endif

}


