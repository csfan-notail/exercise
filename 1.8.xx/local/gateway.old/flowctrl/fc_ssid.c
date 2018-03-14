/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : fc_ssid.c
  版 本 号   : 初稿
  作    者   : Zhong Huaqing
  生成日期   : 2012年8月10日 星期五
  最近修改   :
  功能描述   : 基于ssid的流控操作
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
#include "fc_ssid.h"


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
 * 宏操作                                   *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 函数定义                                       *
 *----------------------------------------------*/

/*****************************************************************************
 函 数 名  : fc_ssid_queue_init
 功能描述  : SSID队列初始化
 输入参数  : FC_QUEUE_T *pstQ    
             FC_SSID_T *pstSsid  
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
AP_ERROR_CODE_E fc_ssid_queue_init(FC_QUEUE_T *pstQ, FC_SSID_T *pstSsid, FC_DIR_E enDir)
{
    if (NULL == pstQ || NULL == pstSsid)
    {
        return AP_E_PARAM;
    }
    
    fc_queue_init(pstQ, enDir);
    
    /*设置匹配属性*/
    strncpy(pstQ->stMatchAttrib.acSsidName, pstSsid->acSsidName, SSID_NAMSIZ);
    /*设置对应的rule*/
    pstQ->enRule = FC_RULE_SSID;
    
    /*设置速率相关信息*/
    fc_queue_update(pstQ, &pstSsid->astRate[enDir], FC_QUEUE_CREATE);
    
    return AP_E_NONE;
}

/*****************************************************************************
 函 数 名  : fc_add_ssid
 功能描述  : 添加基于VAP的流控规则
 输入参数  : FC_SSID_T *pstSsid  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月11日 星期六
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

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
            /* 获取队列, 同时初始化其中的方向成员*/
            pstQ = fc_find_avail_queue(enDir);
            if (NULL == pstQ)
            {
                FC_LOG(FC_LOG_ERR, "fc_find_avail_queue failed.\n");
                return AP_E_RESOURCE;
            }
            
            /*初始化队列*/
            enRet = fc_ssid_queue_init(pstQ, pstSsid, enDir);
            if (AP_E_NONE != enRet)
            {
                FC_LOG(FC_LOG_ERR, "fc_ssid_queue_init failed. ret %d\n", enRet);
                return AP_E_DEFAULT_FAIL;
            }
            
            /*将队列添加到SSID hash表中*/
            enRet = fc_add_ssid_queue_to_htable(pstQ, enDir);
            if (AP_E_NONE != enRet)
            {
                FC_LOG(FC_LOG_ERR, "fc_add_ssid_queue_to_htable failed. ret %d\n", enRet);
                return AP_E_DEFAULT_FAIL;
            }
            
            /*更新字队列集合*/
            fc_update_child_queue_set(pstQ);
            /*调整子队列速率*/
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
            
            /*更新字队列集合*/
            fc_update_child_queue_set(pstQ);
            /*调整子队列速率*/
            fc_adjust_child_queue_rate(pstQ);
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

/*****************************************************************************
 函 数 名  : fc_get_dev_by_name
 功能描述  : 根据Linux内核版本，使用不同的系统接口获取设备结构
 输入参数  : CHAR *szDevName  
 输出参数  : 无
 返 回 值  : struct
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月12日 星期三
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

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
 函 数 名  : fc_set_dev_ssid
 功能描述  : 为某个设备设置SSID字符信息
 输入参数  : CHAR *szSsidName  
             CHAR *szDevName   
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月23日 星期四
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

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
 函 数 名  : fc_get_dev_ssid
 功能描述  : 获取某个设备的SSID字符信息
 输入参数  : CHAR *szSsidName  
             CHAR *szDevName   
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月23日 星期四
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

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


