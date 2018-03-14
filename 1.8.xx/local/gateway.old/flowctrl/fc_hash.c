/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : fc_hash.c
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
#include "fc_hash.h"

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
 
LOCAL struct hlist_head g_astStaHashTable[FC_DIR_MAX][FC_STA_HASH_SIZE]; /*STA hash��*/
LOCAL struct hlist_head g_astVapHashTable[FC_DIR_MAX][FC_VAP_HASH_SIZE]; /*VAP hash��*/
LOCAL struct hlist_head g_astSsidHashTable[FC_DIR_MAX][FC_SSID_HASH_SIZE]; /*SSID hash��*/
LOCAL struct hlist_head g_astVlanHashTable[FC_DIR_MAX][FC_VLAN_HASH_SIZE]; /*VLAN hash��*/

#if (defined(GW_X86) || defined(GW_ARM))

LOCAL DEFINE_RWLOCK(g_stHashLock);

#else
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20))    
LOCAL rwlock_t g_stHashLock = RW_LOCK_UNLOCKED;  /*ʵ������������ + bh����*/
#else
LOCAL DEFINE_RWLOCK(g_stHashLock);
#endif
#endif

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                       *
 *----------------------------------------------*/

#ifdef __STA_HASH__
#endif
/* hash�㷨 */
LOCAL inline UINT32 fc_mac_hash(const UINT8 *pcMac)
{
	UINT32 x;

	x = pcMac[0];
	x = (x << 2) ^ pcMac[1];
	x = (x << 2) ^ pcMac[2];
	x = (x << 2) ^ pcMac[3];
	x = (x << 2) ^ pcMac[4];
	x = (x << 2) ^ pcMac[5];

	x ^= x >> 8;

	return x;
}

LOCAL struct hlist_head *fc_sta_hash_fn(const UINT8 *pcMac, FC_DIR_E enDir)
{
	UINT32 uiHash = fc_mac_hash(pcMac);
	return &(g_astStaHashTable[enDir][uiHash & (FC_STA_HASH_SIZE - 1)]);
}


/*****************************************************************************
 �� �� ��  : fc_add_sta_queue
 ��������  : ��������ӵ�STA��Ӧ��hash table��
 �������  : FC_QUEUE_T *pstStaQ  --����
             FC_DIR_E enDir       --���ط���
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��11�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

*****************************************************************************/
AP_ERROR_CODE_E fc_add_sta_queue_to_htable(FC_QUEUE_T *pstQ, FC_DIR_E enDir)
{
    struct hlist_head *pstHead = NULL;
    
    if (NULL == pstQ)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue.\n");
        return AP_E_PARAM;
    }
    
    FC_HASH_WR_LOCK;
    pstHead = fc_sta_hash_fn(pstQ->stMatchAttrib.acStaMac, enDir);
    hlist_add_head(&(pstQ->stStaHList), pstHead);
    FC_HASH_WR_UNLOCK;
    
    return AP_E_NONE;
}

/*****************************************************************************
 �� �� ��  : fc_del_sta_queue_from_htable
 ��������  : �����д�STA hash����ɾ��
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
AP_ERROR_CODE_E fc_del_sta_queue_from_htable(FC_QUEUE_T *pstQ)
{
    if (NULL == pstQ)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue.\n");
        return AP_E_PARAM;
    }
    
    FC_HASH_WR_LOCK;
    hlist_del(&(pstQ->stStaHList));
    FC_HASH_WR_UNLOCK;
    
    return AP_E_NONE;
}

/*****************************************************************************
 �� �� ��  : fc_get_sta_queue
 ��������  : ����STA���ԣ���ȡ��Ӧ�Ķ���
 �������  : UINT8 *pcMac    --STA MAC��ַ
             FC_DIR_E enDir  --���ط���
 �������  : ��
 �� �� ֵ  : FC_QUEUE_T
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��11�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

*****************************************************************************/
FC_QUEUE_T *fc_get_sta_queue_from_htable(UINT8 *pcMac, FC_DIR_E enDir)
{
    FC_QUEUE_T *pstQ = NULL;
    struct hlist_node *pos = NULL;
    
    if (NULL == pcMac)
    {
        FC_LOG(FC_LOG_ERR, "Invalid STA MAC.\n");
        return NULL;
    }

    FC_HASH_WR_LOCK;
    hlist_for_each(pos, fc_sta_hash_fn(pcMac, enDir))
	{
		pstQ = hlist_entry(pos, FC_QUEUE_T, stStaHList);
		if (0 == memcmp(pstQ->stMatchAttrib.acStaMac, pcMac, ETH_ALEN))
		{
		    FC_HASH_WR_UNLOCK;
			return pstQ; /*Find it. Return at once.*/
		}
	}
	FC_HASH_WR_UNLOCK;
	
	return NULL;
}


#ifdef __VAP_HASH__
#endif

LOCAL inline UINT32 fc_dev_name_hash(const CHAR *szDevName)
{
    return full_name_hash(szDevName, strnlen(szDevName, IFNAMSIZ));
}

LOCAL struct hlist_head *fc_vap_hash_fn(const CHAR *szVapName, FC_DIR_E enDir)
{
	UINT32 uiHash = fc_dev_name_hash(szVapName);
	return &(g_astVapHashTable[enDir][uiHash & (FC_VAP_HASH_SIZE - 1)]);
}

LOCAL struct hlist_head *fc_vlan_hash_fn(const int vid, FC_DIR_E enDir)
{
	UINT32 uiHash = (vid % FC_VLAN_HASH_SIZE);
	return &(g_astVlanHashTable[enDir][uiHash & (FC_VLAN_HASH_SIZE - 1)]);
}


/*****************************************************************************
 �� �� ��  : fc_add_vap_queue_to_htable
 ��������  : ��������ӵ� VAP ��Ӧ��hash table��
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
AP_ERROR_CODE_E fc_add_vap_queue_to_htable(FC_QUEUE_T *pstQ, FC_DIR_E enDir)
{
    struct hlist_head *pstHead = NULL;
    
    if (NULL == pstQ)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue.\n");
        return AP_E_PARAM;
    }
    
    FC_HASH_WR_LOCK;
    pstHead = fc_vap_hash_fn(pstQ->stMatchAttrib.acVapName, enDir);
    hlist_add_head(&(pstQ->stVapHList), pstHead);
    FC_HASH_WR_UNLOCK;
    
    return AP_E_NONE;
}

/*****************************************************************************
 �� �� ��  : fc_del_vap_queue_from_htable
 ��������  : �����д�VAP hash����ɾ��
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
AP_ERROR_CODE_E fc_del_vap_queue_from_htable(FC_QUEUE_T *pstQ)
{
    if (NULL == pstQ)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue.\n");
        return AP_E_PARAM;
    }
    
    FC_HASH_WR_LOCK;
    hlist_del(&(pstQ->stVapHList));
    FC_HASH_WR_UNLOCK;
    
    return AP_E_NONE;
}

/*****************************************************************************
 �� �� ��  : fc_get_vap_queue_from_htable
 ��������  : ����VAP���ƣ���ȡ��Ӧ�Ķ���
 �������  : CHAR *szVapName  
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
FC_QUEUE_T *fc_get_vap_queue_from_htable(CHAR *szVapName, FC_DIR_E enDir)
{
    FC_QUEUE_T *pstQ = NULL;
    struct hlist_node *pos = NULL;
    
    if (NULL == szVapName)
    {
        return NULL;
    }
    
    FC_HASH_WR_LOCK;
    hlist_for_each(pos, fc_vap_hash_fn(szVapName, enDir))
	{
		pstQ = hlist_entry(pos, FC_QUEUE_T, stVapHList);
		if (0 == strncmp(pstQ->stMatchAttrib.acVapName, szVapName, IFNAMSIZ))
		{
		    FC_HASH_WR_UNLOCK;
			return pstQ; /*Find it. Return it at once.*/
		}
	}
	FC_HASH_WR_UNLOCK;
	
	return NULL;
}


/*****************************************************************************
 �� �� ��  : fc_add_vlan_queue_to_htable
 ��������  : ��������ӵ� VLAN ��Ӧ��hash table��
 �������  : FC_QUEUE_T *pstStaQ  
             FC_DIR_E enDir       
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 
    ��    ��   : 
    �޸�����   : �����ɺ���

*****************************************************************************/
AP_ERROR_CODE_E fc_add_vlan_queue_to_htable(FC_QUEUE_T *pstQ, FC_DIR_E enDir)
{
    struct hlist_head *pstHead = NULL;
    
    if (NULL == pstQ)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue.\n");
        return AP_E_PARAM;
    }
    
    FC_HASH_WR_LOCK;
    pstHead = fc_vlan_hash_fn(pstQ->stMatchAttrib.iVid, enDir);
    hlist_add_head(&(pstQ->stVlanHList), pstHead);
    FC_HASH_WR_UNLOCK;
    
    return AP_E_NONE;
}


/*****************************************************************************
 �� �� ��  : fc_del_vlan_queue_from_htable
 ��������  : �����д�VLAN hash����ɾ��
 �������  : FC_QUEUE_T *pstStaQ  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 
    ��    ��   : 
    �޸�����   : �����ɺ���

*****************************************************************************/
AP_ERROR_CODE_E fc_del_vlan_queue_from_htable(FC_QUEUE_T *pstQ)
{
    if (NULL == pstQ)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue.\n");
        return AP_E_PARAM;
    }
    
    FC_HASH_WR_LOCK;
    hlist_del(&(pstQ->stVlanHList));
    FC_HASH_WR_UNLOCK;
    
    return AP_E_NONE;
}



/*****************************************************************************
 �� �� ��  : fc_get_vlan_queue_from_htable
 ��������  : ����VLAN ID����ȡ��Ӧ�Ķ���
 �������  : CHAR *szVlAN  
             FC_DIR_E enDir   
 �������  : ��
 �� �� ֵ  : FC_QUEUE_T
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 
    ��    ��   : 
    �޸�����   : �����ɺ���

*****************************************************************************/
FC_QUEUE_T *fc_get_vlan_queue_from_htable(CHAR *szVlanDev, FC_DIR_E enDir)
{
    FC_QUEUE_T *pstQ = NULL;
    struct hlist_node *pos = NULL;
	INT32 vid;
	CHAR *pt;

    if (NULL == szVlanDev)
    {
        return NULL;
    }

	if((pt = strstr(szVlanDev, ".")) != NULL)
		sscanf(pt+1, "%d", &vid);
	else
		vid = 1;
	if((vid <= 0) || (vid >= 4096))
		return NULL;
    
    FC_HASH_WR_LOCK;
    hlist_for_each(pos, fc_vlan_hash_fn(vid, enDir))
	{
		pstQ = hlist_entry(pos, FC_QUEUE_T, stVlanHList);
		if (pstQ->stMatchAttrib.iVid == vid)
		{
		    FC_HASH_WR_UNLOCK;
			return pstQ; /*Find it. Return it at once.*/
		}
	}
	FC_HASH_WR_UNLOCK;
	
	return NULL;
}


#ifdef __SSID_HASH__
#endif

LOCAL inline UINT32 fc_ssid_name_hash(const CHAR *szSsidName)
{
    return full_name_hash(szSsidName, strnlen(szSsidName, SSID_NAMSIZ));
}

LOCAL struct hlist_head *fc_ssid_hash_fn(const CHAR *szSsidName, FC_DIR_E enDir)
{
	UINT32 uiHash = fc_dev_name_hash(szSsidName);
	return &(g_astVapHashTable[enDir][uiHash & (FC_SSID_HASH_SIZE - 1)]);
}

/*****************************************************************************
 �� �� ��  : fc_add_ssid_queue_to_htable
 ��������  : ��������ӵ� SSID ��Ӧ��hash table��
 �������  : FC_QUEUE_T *pstQ  
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
AP_ERROR_CODE_E fc_add_ssid_queue_to_htable(FC_QUEUE_T *pstQ, FC_DIR_E enDir)
{
    struct hlist_head *pstHead = NULL;
    
    if (NULL == pstQ)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue.\n");
        return AP_E_PARAM;
    }
    
    FC_HASH_WR_LOCK;
    pstHead = fc_ssid_hash_fn(pstQ->stMatchAttrib.acSsidName, enDir);
    hlist_add_head(&(pstQ->stSsidHList), pstHead);
    FC_HASH_WR_UNLOCK;
    
    return AP_E_NONE;
}

/*****************************************************************************
 �� �� ��  : fc_del_ssid_queue_from_htable
 ��������  : �����д�SSID hash����ɾ��
 �������  : FC_QUEUE_T *pstQ  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��11�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

*****************************************************************************/
AP_ERROR_CODE_E fc_del_ssid_queue_from_htable(FC_QUEUE_T *pstQ)
{
    if (NULL == pstQ)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue.\n");
        return AP_E_PARAM;
    }
    
    FC_HASH_WR_LOCK;
    hlist_del(&(pstQ->stSsidHList));
    FC_HASH_WR_UNLOCK;
    
    return AP_E_NONE;
}

/*****************************************************************************
 �� �� ��  : fc_get_vap_queue_from_htable
 ��������  : ����VAP���ƣ���ȡ��Ӧ�Ķ���
 �������  : CHAR *szVapName  
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
FC_QUEUE_T *fc_get_ssid_queue_from_htable(CHAR *szSsidName, FC_DIR_E enDir)
{
    FC_QUEUE_T *pstQ = NULL;
    struct hlist_node *pos = NULL;
    
    if (NULL == szSsidName)
    {
        FC_LOG(FC_LOG_ERR, "Invalid SSID Name.\n");
        return NULL;
    }
    
    FC_HASH_WR_LOCK;
    hlist_for_each(pos, fc_ssid_hash_fn(szSsidName, enDir))
	{
		pstQ = hlist_entry(pos, FC_QUEUE_T, stSsidHList);
		if (0 == strncmp(pstQ->stMatchAttrib.acSsidName, szSsidName, SSID_NAMSIZ))
		{
		    FC_HASH_WR_UNLOCK;
			return pstQ; /*Find it. Return it at once.*/
		}
	}
	FC_HASH_WR_UNLOCK;
	
	return NULL;
}


AP_ERROR_CODE_E fc_hash_init(void)
{
    FC_DIR_E enDir;
    UINT32 uiIndex;
    
    for (enDir = FC_DIR_DS; enDir < FC_DIR_MAX; enDir++)
    {
        /*For STA*/
        for (uiIndex = 0; uiIndex < FC_STA_HASH_SIZE; uiIndex++)
        {
            INIT_HLIST_HEAD(&g_astStaHashTable[enDir][uiIndex]);
        }
        
        /*For VAP*/
        for (uiIndex = 0; uiIndex < FC_VAP_HASH_SIZE; uiIndex++)
        {
            INIT_HLIST_HEAD(&g_astVapHashTable[enDir][uiIndex]);
        }
        
        /*For SSID*/
        for (uiIndex = 0; uiIndex < FC_SSID_HASH_SIZE; uiIndex++)
        {
            INIT_HLIST_HEAD(&g_astSsidHashTable[enDir][uiIndex]);
        }

		/*For VLAN*/
        for (uiIndex = 0; uiIndex < FC_VLAN_HASH_SIZE; uiIndex++)
        {
            INIT_HLIST_HEAD(&g_astVlanHashTable[enDir][uiIndex]);
        }
    }
    
    return AP_E_NONE;
    
}
