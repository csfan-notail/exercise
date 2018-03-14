/******************************************************************************

                  版权所有 (C), 2001-2014, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : fc_vlan.c
  版 本 号   : 初稿
  作    者   : 
  生成日期   : 
  最近修改   :
  功能描述   : 流控相关的hash操作
  函数列表   :
  修改历史   :
  1.日    期   : 
    作    者   : 
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "fc_main.h"
#include "fc_queue.h"
#include "fc_hash.h"
#include "fc_vlan.h"

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
 


/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 函数定义                                       *
 *----------------------------------------------*/

/*****************************************************************************
 函 数 名  : fc_vlan_queue_init
 功能描述  : VLAN队列初始化
 输入参数  :   
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 
    作    者   : 
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E fc_vlan_queue_init(FC_QUEUE_T *pstQ, FC_VLAN_T *pstVlan, FC_DIR_E enDir)
{
    if (NULL == pstQ || NULL == pstVlan)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue or VLAN data.\n");
        return AP_E_PARAM;
    }
    
    fc_queue_init(pstQ, enDir);
    
    /*设置匹配属性*/
	pstQ->stMatchAttrib.iVid = pstVlan->vid;
    /*设置对应的rule*/
    pstQ->enRule = FC_RULE_VLAN;
    
    /*设置速率相关信息*/
    fc_queue_update(pstQ, &pstVlan->astRate[enDir], FC_QUEUE_CREATE);
    
    return AP_E_NONE;
}

/*****************************************************************************
 函 数 名  : fc_add_vlan
 功能描述  : 添加基于VLAN的流控规则
 输入参数  : FC_VLAN_T *pstVlan  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 
    作    者   : 
    修改内容   : 新生成函数

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
            /* 获取队列, 同时初始化其中的方向成员*/
            pstQ = fc_find_avail_queue(enDir);
            if (NULL == pstQ)
            {
                FC_LOG(FC_LOG_ERR, "fc_find_avail_queue failed.\n");
                return AP_E_RESOURCE;
            }

            
            /*初始化队列*/
            enRet = fc_vlan_queue_init(pstQ, pstVlan, enDir);
            if (AP_E_NONE != enRet)
            {
                FC_LOG(FC_LOG_ERR, "fc_vlan_queue_init failed. ret %d\n", enRet);
                return AP_E_DEFAULT_FAIL;
            }
            
            /*将队列添加到VLAN hash表中*/
            enRet = fc_add_vlan_queue_to_htable(pstQ, enDir);
            if (AP_E_NONE != enRet)
            {
                FC_LOG(FC_LOG_ERR, "fc_add_vlan_queue_to_htable failed. ret %d\n", enRet);
                return AP_E_DEFAULT_FAIL;
            }
            //vlan 设置没有MULTI(二级限速)即 sta + vap/ssid/ap，所以以下函数均直接返回
           
            /*更新子队列集合*/
            //fc_update_child_queue_set(pstQ);
            /*调整子队列速率*/
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
            
            /*更新字队列集合*/
            //fc_update_child_queue_set(pstQ);
            /*调整子队列速率*/
            //fc_adjust_child_queue_rate(pstQ);
            
            //更新属于该vlan的sta速率，替换队列更新
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
 函 数 名  : fc_del_sta_by_vlan
 功能描述  : 删除基于vlan的自动限速时，某个vlan下的所有终端限速结构
 输入参数  : INT32 vid  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 
    作    者   : 
    修改内容   : 新生成函数

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
 函 数 名  : fc_del_vlan
 功能描述  : 删除基于VLAN的流控规则
 输入参数  : INT32 vid  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 
    作    者   : 
    修改内容   : 新生成函数

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
        
        /*恢复子队列速率为默认值*/
        fc_queue_recover_rate(pstQ);
        
        /*销毁队列*/
        enRet = fc_queue_destroy(pstQ);
        if (AP_E_NONE != enRet)
        {
            FC_LOG(FC_LOG_ERR, "fc_queue_destroy failed. ret %d\n", enRet);
            return AP_E_DEFAULT_FAIL;
        }
        
    }

    /* 删除该vlan下的所有sta */
    fc_del_sta_by_vlan(vid);
    
    return AP_E_NONE;
}


