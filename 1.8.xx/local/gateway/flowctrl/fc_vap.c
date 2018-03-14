/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : fc_vap.c
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
#include "fc_vap.h"

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
 函 数 名  : fc_vap_queue_init
 功能描述  : VAP队列初始化
 输入参数  : FC_QUEUE_T *pstVapQ  
             FC_VAP_T *pstVaP     
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
AP_ERROR_CODE_E fc_vap_queue_init(FC_QUEUE_T *pstQ, FC_VAP_T *pstVaP, FC_DIR_E enDir)
{
    if (NULL == pstQ || NULL == pstVaP)
    {
        FC_LOG(FC_LOG_ERR, "Invalid Queue or VAP data.\n");
        return AP_E_PARAM;
    }
    
    fc_queue_init(pstQ, enDir);
    
    /*设置匹配属性*/
    strncpy(pstQ->stMatchAttrib.acVapName, pstVaP->acVapName, IFNAMSIZ);
    /*设置对应的rule*/
    pstQ->enRule = FC_RULE_VAP;
    
    /*设置速率相关信息*/
    fc_queue_update(pstQ, &pstVaP->astRate[enDir], FC_QUEUE_CREATE);
    
    return AP_E_NONE;
}

/*****************************************************************************
 函 数 名  : fc_add_vap
 功能描述  : 添加基于VAP的流控规则
 输入参数  : FC_VAP_T *pstVaP  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月11日 星期六
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

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
            /* 获取队列, 同时初始化其中的方向成员*/
            pstQ = fc_find_avail_queue(enDir);
            if (NULL == pstQ)
            {
                FC_LOG(FC_LOG_ERR, "fc_find_avail_queue failed.\n");
                return AP_E_RESOURCE;
            }
            
            /*初始化队列*/
            enRet = fc_vap_queue_init(pstQ, pstVaP, enDir);
            if (AP_E_NONE != enRet)
            {
                FC_LOG(FC_LOG_ERR, "fc_vap_queue_init failed. ret %d\n", enRet);
                return AP_E_DEFAULT_FAIL;
            }
            
            /*将队列添加到VAP hash表中*/
            enRet = fc_add_vap_queue_to_htable(pstQ, enDir);
            if (AP_E_NONE != enRet)
            {
                FC_LOG(FC_LOG_ERR, "fc_add_vap_queue_to_htable failed. ret %d\n", enRet);
                return AP_E_DEFAULT_FAIL;
            }
            
            /*更新子队列集合*/
            fc_update_child_queue_set(pstQ);
            /*调整子队列速率*/
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
            
            /*更新字队列集合*/
            fc_update_child_queue_set(pstQ);
            /*调整子队列速率*/
            fc_adjust_child_queue_rate(pstQ);
        }
        
    }
    
    return AP_E_NONE;
}

/*****************************************************************************
 函 数 名  : fc_del_sta_by_vap
 功能描述  : 删除基于vap的自动限速时，某个vap下的所有终端限速结构
 输入参数  : const CHAR *szVap  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年10月20日
    作    者   : xucongjiang
    修改内容   : 新生成函数

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
 函 数 名  : fc_del_vap
 功能描述  : 删除基于VAP的流控规则
 输入参数  : CHAR *szVapName  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月11日 星期六
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

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

    /* BEGIN: Modified by xucongjiang, 2012/10/20 */
    /* 删除该vap下的所有sta */
    fc_del_sta_by_vap(szVapName);
    /* END:   Modified by xucongjiang, 2012/10/20   PN: */
    
    return AP_E_NONE;
}


