/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : fc_ap.c
  版 本 号   : 初稿
  作    者   : Zhong Huaqing
  生成日期   : 2012年8月10日 星期五
  最近修改   :
  功能描述   : AP整体流控操作
  函数列表   :
  修改历史   :
  1.日    期   : 2012年8月10日 星期五
    作    者   : Zhong Huaqing
    修改内容   : 创建文件

  2.日    期   : 2012年11月9日
    作    者   : xucongjiang
    修改内容   : 解决ap auto规则的相关的bug
    
******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "fc_main.h"
#include "fc_queue.h"
#include "fc_ap.h"

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


AP_ERROR_CODE_E fc_ap_queue_init(FC_QUEUE_T *pstQ, FC_AP_T *pstAp, FC_DIR_E enDir)
{
    if (NULL == pstQ || NULL == pstAp)
    {
        return AP_E_PARAM;
    }
    
    fc_queue_init(pstQ, enDir);
    
    /*设置对应的rule*/
    pstQ->enRule = FC_RULE_AP;
    
    /*设置速率相关信息*/
    fc_queue_update(pstQ, &pstAp->astRate[enDir], FC_QUEUE_CREATE);
    
    return AP_E_NONE;
}

/*****************************************************************************
 函 数 名  : fc_add_ap
 功能描述  : 添加基于AP的流控规则
 输入参数  : FC_VAP_T *pstAp  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月11日 星期六
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

  2.日    期   : 2012年11月9日
    作    者   : xucongjiang
    修改内容   : ap autu规则的问题
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

    /* BEGIN: Added by xucongjiang, 2012/11/9 解决ap auto规则下重新配置速率不生效的问题 */
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
        if (!pstQ->bInUse) /*队列未被使用*/
        {
            FC_LOG(FC_LOG_INFO, "Create AP.\n");
            /*初始化队列*/
            enRet = fc_ap_queue_init(pstQ, pstAp, enDir);

            if (AP_E_NONE != enRet)
            {
                FC_LOG(FC_LOG_ERR, "fc_vap_queue_init failed. ret %d\n", enRet);
                return AP_E_DEFAULT_FAIL;
            }
            
            /*更新字队列集合*/
            fc_update_child_queue_set(pstQ);
            /*调整子队列速率*/
            fc_adjust_child_queue_rate(pstQ);
        }
        else /*队列已被使用*/
        {
            FC_LOG(FC_LOG_INFO, "Update AP.\n");
            enRet = fc_queue_update(pstQ, &pstAp->astRate[enDir], FC_QUEUE_UPDATE);

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
 函 数 名  : fc_del_ap
 功能描述  : 删除基于AP的流控规则
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月11日 星期六
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

  2.日    期   : 2012年11月9日
    作    者   : xucongjiang
    修改内容   : ap auto
*****************************************************************************/
AP_ERROR_CODE_E fc_del_ap(void)
{
    FC_QUEUE_T *pstQ;
    FC_DIR_E enDir;
    AP_ERROR_CODE_E enRet;
    FC_RULE_E enRule;
    
    /* BEGIN: Added by xucongjiang, 2012/11/9  */
    /* 解决ap auto规则下删除AP流控时，终端限速结构未被删除的问题 */
    enRule = fc_get_rule();
    if ((FC_RULE_AUTO & enRule) && (FC_RULE_AP & enRule))
    {
        return fc_queue_flush_all();
    }
    /* END:   Added by xucongjiang, 2012/11/9   PN: */
    
    for (enDir = FC_DIR_DS; enDir < FC_DIR_MAX; enDir++)
    {
        pstQ = fc_get_ap_queue(enDir);
        
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
    
    return AP_E_NONE;
}

