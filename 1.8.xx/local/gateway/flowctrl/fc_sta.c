/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : fc_sta.c
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
#include "fc_queue.h"
#include "fc_hash.h"
#include "fc_sta.h"

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
 函 数 名  : fc_sta_queue_init
 功能描述  : STA队列初始化
 输入参数  : FC_QUEUE_T *pstStaQ  
             FC_STA_T *pstSta     
             FC_DIR_E enDir       
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月17日 星期五
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E fc_sta_queue_init(FC_QUEUE_T *pstQ, FC_STA_T *pstSta, FC_DIR_E enDir)
{
    if (NULL == pstQ || NULL == pstSta)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue or STA data.\n");
        return AP_E_PARAM;
    }
    
    fc_queue_init(pstQ, enDir);
    
    /*设置匹配属性*/
    memcpy(pstQ->stMatchAttrib.acStaMac, pstSta->acMac, ETH_ALEN);
    /*设置对应的rule*/
    pstQ->enRule = FC_RULE_STA;
    
    /*设置速率相关信息*/
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
    
    /*只有终端模式才有对应的父队列*/
    if (FC_RULE_STA != pstQ->enRule)
    {
        return AP_E_NONE;
    }
    
    
    if (FC_QID_INVALID != pstQ->iPQId) /*之前已添加*/
    {
        #if 0
        if (pstQ->stMatchAttrib.iRadioId != pstSta->iRadioId
            || pstQ->stMatchAttrib.iWlanId != pstSta->iWlanId) /*Parent 被修改*/
        {
            list_del(&pstQ->stStaSet); 
            pstQ->iPQId = FC_QID_INVALID;  /*必须设置为无效值*/
        }
        else
        {
            return AP_E_NONE; /*无需修改*/
        }
        #endif
        
        list_del(&pstQ->stStaSet); 
    }
    
    /*更新终端的rid和wid信息*/
    pstQ->stMatchAttrib.iRadioId = pstSta->iRadioId;
    pstQ->stMatchAttrib.iWlanId = pstSta->iWlanId;

	/*更新终端的vlan信息*/
	pstQ->stMatchAttrib.iVid = pstSta->iVid;
    
    /*获取parent 队列*/
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
    
    /*对于AUTO型规则，只需要更新子队列，不需要更新父队列信息*/
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
 函 数 名  : fc_add_sta
 功能描述  : 添加基于STA的流控规则
 输入参数  : FC_STA_T *pstSta  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月11日 星期六
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

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
            /* 获取队列, 同时初始化其中的方向成员*/
            pstQ = fc_find_avail_queue(enDir);
            if (NULL == pstQ)
            {
                FC_LOG(FC_LOG_ERR, "fc_find_avail_queue failed.\n");
                return AP_E_RESOURCE;
            }
            
            /*初始化队列*/
            enRet = fc_sta_queue_init(pstQ, pstSta, enDir);
            if (AP_E_NONE != enRet)
            {
                FC_LOG(FC_LOG_ERR, "fc_sta_queue_init failed. ret %d\n", enRet);
                return AP_E_DEFAULT_FAIL;
            }
            
            /*将队列添加到STA hash表中*/
            enRet = fc_add_sta_queue_to_htable(pstQ, enDir);
            if (AP_E_NONE != enRet)
            {
                FC_LOG(FC_LOG_ERR, "fc_add_sta_queue_to_htable failed. ret %d\n", enRet);
                return AP_E_DEFAULT_FAIL;
            }
            
            /*更新上一级的内容*/
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
            
            /*更新上一级的内容*/
            fc_update_sta_parent_info(pstQ, pstSta);
            
        }
        
    }
    
    return AP_E_NONE;
}

/*****************************************************************************
 函 数 名  : fc_del_sta
 功能描述  : 删除基于STA的流控规则
 输入参数  : UINT8 *pcMac  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月11日 星期六
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

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

