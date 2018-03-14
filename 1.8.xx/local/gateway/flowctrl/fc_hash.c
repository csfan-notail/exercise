/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : fc_hash.c
  版 本 号   : 初稿
  作    者   : Zhong Huaqing
  生成日期   : 2012年8月10日 星期五
  最近修改   :
  功能描述   : 流控相关的hash操作
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
#include "fc_hash.h"

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/
 
LOCAL struct hlist_head g_astStaHashTable[FC_DIR_MAX][FC_STA_HASH_SIZE]; /*STA hash表*/
LOCAL struct hlist_head g_astVapHashTable[FC_DIR_MAX][FC_VAP_HASH_SIZE]; /*VAP hash表*/
LOCAL struct hlist_head g_astSsidHashTable[FC_DIR_MAX][FC_SSID_HASH_SIZE]; /*SSID hash表*/
LOCAL struct hlist_head g_astVlanHashTable[FC_DIR_MAX][FC_VLAN_HASH_SIZE]; /*VLAN hash表*/

#if (defined(GW_X86) || defined(GW_ARM))

LOCAL DEFINE_RWLOCK(g_stHashLock);

#else
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20))    
LOCAL rwlock_t g_stHashLock = RW_LOCK_UNLOCKED;  /*实际上是自旋锁 + bh控制*/
#else
LOCAL DEFINE_RWLOCK(g_stHashLock);
#endif
#endif

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 函数定义                                       *
 *----------------------------------------------*/

#ifdef __STA_HASH__
#endif
/* hash算法 */
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
 函 数 名  : fc_add_sta_queue
 功能描述  : 将队列添加到STA对应的hash table中
 输入参数  : FC_QUEUE_T *pstStaQ  --队列
             FC_DIR_E enDir       --流控方向
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月11日 星期六
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

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
 函 数 名  : fc_del_sta_queue_from_htable
 功能描述  : 将队列从STA hash表中删除
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
 函 数 名  : fc_get_sta_queue
 功能描述  : 根据STA属性，获取对应的队列
 输入参数  : UINT8 *pcMac    --STA MAC地址
             FC_DIR_E enDir  --流控方向
 输出参数  : 无
 返 回 值  : FC_QUEUE_T
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月11日 星期六
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

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
 函 数 名  : fc_add_vap_queue_to_htable
 功能描述  : 将队列添加到 VAP 对应的hash table中
 输入参数  : FC_QUEUE_T *pstStaQ  
             FC_DIR_E enDir       
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月11日 星期六
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

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
 函 数 名  : fc_del_vap_queue_from_htable
 功能描述  : 将队列从VAP hash表中删除
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
 函 数 名  : fc_get_vap_queue_from_htable
 功能描述  : 根据VAP名称，获取对应的队列
 输入参数  : CHAR *szVapName  
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
 函 数 名  : fc_add_vlan_queue_to_htable
 功能描述  : 将队列添加到 VLAN 对应的hash table中
 输入参数  : FC_QUEUE_T *pstStaQ  
             FC_DIR_E enDir       
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 
    作    者   : 
    修改内容   : 新生成函数

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
 函 数 名  : fc_del_vlan_queue_from_htable
 功能描述  : 将队列从VLAN hash表中删除
 输入参数  : FC_QUEUE_T *pstStaQ  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 
    作    者   : 
    修改内容   : 新生成函数

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
 函 数 名  : fc_get_vlan_queue_from_htable
 功能描述  : 根据VLAN ID，获取对应的队列
 输入参数  : CHAR *szVlAN  
             FC_DIR_E enDir   
 输出参数  : 无
 返 回 值  : FC_QUEUE_T
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 
    作    者   : 
    修改内容   : 新生成函数

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
 函 数 名  : fc_add_ssid_queue_to_htable
 功能描述  : 将队列添加到 SSID 对应的hash table中
 输入参数  : FC_QUEUE_T *pstQ  
             FC_DIR_E enDir       
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月11日 星期六
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

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
 函 数 名  : fc_del_ssid_queue_from_htable
 功能描述  : 将队列从SSID hash表中删除
 输入参数  : FC_QUEUE_T *pstQ  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月11日 星期六
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

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
 函 数 名  : fc_get_vap_queue_from_htable
 功能描述  : 根据VAP名称，获取对应的队列
 输入参数  : CHAR *szVapName  
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
