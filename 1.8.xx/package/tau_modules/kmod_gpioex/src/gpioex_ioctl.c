/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : gpioex_ioctl.c
  版 本 号   : 初稿
  作    者   : Zhong Huaqing
  生成日期   : 2012年8月15日 星期三
  最近修改   :
  功能描述   : GPIOEX模块IO接口，用于和用户态进行交互.
               说明: 本文件中，不实现任何实质性的功能，仅作为一个交互通道。
  函数列表   :
  
  修改历史   :
  1.日    期   : 2012年8月15日 星期三
    作    者   : Zhong Huaqing
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "gpioex_main.h"

#include "gpioex_ioctl.h"
#include "gpioex_ops.h"
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
 * 宏定义                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 函数定义                                       *
 *----------------------------------------------*/
AP_ERROR_CODE_E gpioex_ioctl_get_version(GPIOEX_IOCTL_VERSION_T *pstVersion)
{
    strncpy(pstVersion->acModVersion, GPIOEX_MOD_VERSION, GPIOEX_VERSION_SIZE);
    strncpy(pstVersion->acModDepVersion, GPIOEX_MOD_DEP_VERSION, GPIOEX_VERSION_SIZE);
    return AP_E_NONE;
}


LOCAL AP_ERROR_CODE_E gpioex_ioctl_set_debug_level(GPIOEX_IOCTL_DEBUG_T *pstDebug)
{
    if (NULL == pstDebug)
    {
        return AP_E_PARAM;
    }
    return gpioex_set_debugLevel(pstDebug->uiDebugLevel);
}

LOCAL AP_ERROR_CODE_E gpioex_ioctl_get_debug_level(GPIOEX_IOCTL_DEBUG_T *pstDebug)
{
    if (NULL == pstDebug)
    {
        return AP_E_PARAM;
    }
    pstDebug->uiDebugLevel = gpioex_get_debugLevel();
    return AP_E_NONE;
}

LOCAL AP_ERROR_CODE_E gpioex_ioctl_mode_all_set(GPIOEX_IOCTL_MODE_ALL_T *pstMode)
{
    if (NULL == pstMode)
    {
        return AP_E_PARAM;
    }
    return gpioex_mode_all_set(pstMode->uiValue);
}

LOCAL AP_ERROR_CODE_E gpioex_ioctl_mode_all_get(GPIOEX_IOCTL_MODE_ALL_T *pstMode)
{
    if (NULL == pstMode)
    {
        return AP_E_PARAM;
    }
    return gpioex_mode_all_get(&pstMode->uiValue);
}
LOCAL AP_ERROR_CODE_E gpioex_ioctl_mode_set(GPIOEX_IOCTL_MODE_T *pstMode)
{
    if (NULL == pstMode)
    {
        return AP_E_PARAM;
    }
    return gpioex_mode_set(pstMode->uiNum,pstMode->uiValue);
}
LOCAL AP_ERROR_CODE_E gpioex_ioctl_mode_get(GPIOEX_IOCTL_MODE_T *pstMode)
{
    if (NULL == pstMode)
    {
        return AP_E_PARAM;
    }
    return gpioex_mode_get(pstMode->uiNum,&pstMode->uiValue);
}

LOCAL AP_ERROR_CODE_E gpioex_ioctl_state_all_set(GPIOEX_IOCTL_STATE_ALL_T *pstState)
{
    if (NULL == pstState)
    {
        return AP_E_PARAM;
    }
    return gpioex_state_all_set(pstState->uiValue);
}
LOCAL AP_ERROR_CODE_E gpioex_ioctl_state_all_get(GPIOEX_IOCTL_STATE_ALL_T *pstState)
{
    if (NULL == pstState)
    {
        return AP_E_PARAM;
    }
    return gpioex_state_all_get(&pstState->uiValue);
}
LOCAL AP_ERROR_CODE_E gpioex_ioctl_state_set(GPIOEX_IOCTL_STATE_T *pstState)
{
    if (NULL == pstState)
    {
        return AP_E_PARAM;
    }
    return gpioex_state_set(pstState->uiNum,pstState->uiValue);
}

LOCAL AP_ERROR_CODE_E gpioex_ioctl_state_get(GPIOEX_IOCTL_STATE_T *pstState)
{
    if (NULL == pstState)
    {
        return AP_E_PARAM;
    }
    return gpioex_state_get(pstState->uiNum,&pstState->uiValue);
}

/*****************************************************************************
 函 数 名  : gpioex_ioctl_handler
 功能描述  : IO交互统一接口
 输入参数  : void  *pstArg  --用户态空间的指针
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月15日 星期三
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E gpioex_ioctl_handler(void *pstArg)
{
	LOCAL GPIOEX_IOCTL_T stIoctl;
	BOOL bGetOps = FALSE; /*取值为真时，表示需要向用户态复制数据*/
	AP_ERROR_CODE_E enRet = AP_E_NONE;
	
    if (NULL == pstArg)
    {
        return AP_E_PARAM;
    }
    
	if (copy_from_user(&stIoctl, pstArg, sizeof(GPIOEX_IOCTL_T)))
	{
	    GPIOEX_LOG(GPIOEX_LOG_ERR, "Copy from user failed.\n");
		return AP_E_DEFAULT_FAIL;
	}
//	GPIOEX_LOG(GPIOEX_LOG_DEBUG, "stIoctl.enCmd = %d\n", stIoctl.enCmd);
	
	
	switch (stIoctl.enCmd)
	{
	    /*移植时，以下三个命令需要保留*/
        case GPIOEX_IOCTL_CMD_GET_VERSION: 
            enRet = gpioex_ioctl_get_version(&stIoctl.unData.stVersion);
            bGetOps = TRUE; 
            break;
            
	    case GPIOEX_IOCTL_CMD_SET_DEBUG_LEVEL:
	        enRet = gpioex_ioctl_set_debug_level(&stIoctl.unData.stDebug); 
	        break;
	    case GPIOEX_IOCTL_CMD_GET_DEBUG_LEVEL:
	        enRet = gpioex_ioctl_get_debug_level(&stIoctl.unData.stDebug); 
	        bGetOps = TRUE;
	        break;
	        
	    case GPIOEX_IOCTL_CMD_ALL_MODE_SET: 
	        enRet = gpioex_ioctl_mode_all_set(&stIoctl.unData.stAllMode); 
	        break;
        case GPIOEX_IOCTL_CMD_ALL_MODE_GET: 
            enRet = gpioex_ioctl_mode_all_get(&stIoctl.unData.stAllMode); 
            bGetOps = TRUE; 
            break;
   
	    case GPIOEX_IOCTL_CMD_MODE_SET: 
	        enRet = gpioex_ioctl_mode_set(&stIoctl.unData.stMode); 
	        break;
        case GPIOEX_IOCTL_CMD_MODE_GET: 
            enRet = gpioex_ioctl_mode_get(&stIoctl.unData.stMode); 
            bGetOps = TRUE; 
            break;
   
	    case GPIOEX_IOCTL_CMD_ALL_STATE_SET: 
	        enRet = gpioex_ioctl_state_all_set(&stIoctl.unData.stAllState); 
	        break;
        case GPIOEX_IOCTL_CMD_ALL_STATE_GET: 
            enRet = gpioex_ioctl_state_all_get(&stIoctl.unData.stAllState); 
            bGetOps = TRUE; 
            break;
   
	    case GPIOEX_IOCTL_CMD_STATE_SET: 
	        enRet = gpioex_ioctl_state_set(&stIoctl.unData.stState); 
	        break;
        case GPIOEX_IOCTL_CMD_STATE_GET: 
            enRet = gpioex_ioctl_state_get(&stIoctl.unData.stState); 
            bGetOps = TRUE; 
            break;
   
   
        default:
			GPIOEX_LOG(GPIOEX_LOG_ERR, "Unknown cmd %d\n", stIoctl.enCmd);
			return  AP_E_PARAM;
			break;
	}
	
	if (AP_E_NONE != enRet)
	{
	    return AP_E_DEFAULT_FAIL;
	}
	
	if (bGetOps)
	{
    	if (copy_to_user(pstArg, &stIoctl, sizeof(GPIOEX_IOCTL_T)))
    	{
    		GPIOEX_LOG(GPIOEX_LOG_ERR, "Copy to user failed.\n");
    		return AP_E_DEFAULT_FAIL;
    	}
	}
	
	return AP_E_NONE;
}


