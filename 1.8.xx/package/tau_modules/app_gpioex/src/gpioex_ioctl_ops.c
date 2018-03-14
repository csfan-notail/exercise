/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : gpioex_ioctl_ops.c
  版 本 号   : 初稿
  作    者   : Zhong Huaqing
  生成日期   : 2012年8月15日 星期三
  最近修改   :
  功能描述   : 命令工具的通用ioctl操作
  函数列表   :
  修改历史   :
  1.日    期   : 2012年8月15日 星期三
    作    者   : Zhong Huaqing
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>  
#include <unistd.h>
#include <fcntl.h>	
#include <sys/ioctl.h>	

#include "gpioex_ioctl.h"
#include "procfs_fops.h"

#include "gpioex_ioctl_main.h"
#include "gpioex_ioctl_ops.h"

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
;

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 函数定义                                       *
 *----------------------------------------------*/

/*****************************************************************************
 函 数 名  : gpioex_ioctl_init
 功能描述  : 初始化系统入口
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月27日 星期四
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
INT32 gpioex_ioctl_init(void)
{    
    INT32 iProcFd = FD_INVALID;
    CHAR acProcFileName[GPIOEX_FILE_NAME_SIZE];
    
    snprintf(acProcFileName, GPIOEX_FILE_NAME_SIZE, 
            "%s/%s", PROC_ROOT_DIR, GPIOEX_PROC_FILE);
            
    iProcFd = open(acProcFileName, O_RDWR);
    if (iProcFd < 0)
    {
        printf("Call %s faild. fd = %d\n", __FUNCTION__, iProcFd);
        return FD_INVALID; 
    }
    
    return iProcFd;
}

/*****************************************************************************
 函 数 名  : gpioex_ioctl_destroy
 功能描述  : 销毁系统入口
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月27日 星期四
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E gpioex_ioctl_destroy(INT32 iProcFd)
{
    close(iProcFd);
    return AP_E_NONE;
}

/*****************************************************************************
 函 数 名  : gpioex_ioctl
 功能描述  : io操作接口
 输入参数  : GPIOEX_IOCTL_T *pstIoctl  
             GPIOEX_IOCTL_CMD_E enCmd  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月27日 星期四
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E gpioex_ioctl(GPIOEX_IOCTL_T *pstIoctl, GPIOEX_IOCTL_CMD_E enCmd)
{
    INT32 iProcFd;
	INT32 iRet;

	if (NULL == pstIoctl)
	{
		return AP_E_PARAM;
	}
    
    iProcFd = gpioex_ioctl_init();
    if (FD_INVALID == iProcFd)
    {
        return AP_E_RESOURCE;
    }
    
    pstIoctl->enCmd = enCmd;
	iRet = ioctl(iProcFd, PROCFS_FOPS_IOCTL_CMD, (void*)pstIoctl);
	if (0 != iRet)
	{
		printf("Call %s failed. ret = %d\n", __FUNCTION__, iRet);
		return AP_E_DEFAULT_FAIL;
	}
	
	gpioex_ioctl_destroy(iProcFd);
	
	//printf("PROCFS_FOPS_IOCTL_CMD = 0x%x\n", PROCFS_FOPS_IOCTL_CMD);
	return AP_E_NONE;
}


/*****************************************************************************
 函 数 名  : gpioex_cmd_get_mod_version
 功能描述  : 获取内核模块的版本信息
 输入参数  : GPIOEX_IOCTL_VERSION_T *pstVersion  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月6日 星期二
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E gpioex_cmd_get_mod_version(GPIOEX_IOCTL_VERSION_T *pstVersion)
{
    LOCAL GPIOEX_IOCTL_T stIoctl;    
    AP_ERROR_CODE_E enRet;
    
    
    if (NULL == pstVersion)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(GPIOEX_IOCTL_T));
    enRet = gpioex_ioctl(&stIoctl, GPIOEX_IOCTL_CMD_GET_VERSION); 
    if (AP_E_NONE == enRet)
    {
        *pstVersion = stIoctl.unData.stVersion;
    }
    
    return enRet;
}

/*****************************************************************************
 函 数 名  : gpioex_cmd_set_debug_level
 功能描述  : 设置内核模块调试输出级别
 输入参数  : UINT32 uiDebugLevel  -- 0-7, 7 表示输出最多的信息
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月6日 星期二
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E gpioex_cmd_set_debug_level(UINT32 uiDebugLevel)
{
    LOCAL GPIOEX_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    
    if (uiDebugLevel > GPIOEX_DEBUG_LEVEL_MAX)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(GPIOEX_IOCTL_T));
    stIoctl.unData.stDebug.uiDebugLevel = uiDebugLevel;
    enRet = gpioex_ioctl(&stIoctl, GPIOEX_IOCTL_CMD_SET_DEBUG_LEVEL);
    
    return enRet;
}

/*****************************************************************************
 函 数 名  : gpioex_cmd_get_debug_level
 功能描述  : 获取内核模块调试输出级别
 输入参数  : UINT32 *puiDebugLevel  -- 用于存储级别数值
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月6日 星期二
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E gpioex_cmd_get_debug_level(UINT32 *puiDebugLevel)
{
    LOCAL GPIOEX_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    
    if (NULL == puiDebugLevel)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(GPIOEX_IOCTL_T));
    enRet = gpioex_ioctl(&stIoctl, GPIOEX_IOCTL_CMD_GET_DEBUG_LEVEL);
    *puiDebugLevel = stIoctl.unData.stDebug.uiDebugLevel;
    
    return enRet;
}

AP_ERROR_CODE_E gpioex_cmd_set_all_mode(GPIOEX_IOCTL_MODE_ALL_T stAllMode)
{
    LOCAL GPIOEX_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    
    if (stAllMode.uiValue > 0xffff)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(GPIOEX_IOCTL_T));
    stIoctl.unData.stAllMode.uiValue= stAllMode.uiValue;
    enRet = gpioex_ioctl(&stIoctl, GPIOEX_IOCTL_CMD_ALL_MODE_SET);
    
    return enRet;
}

AP_ERROR_CODE_E gpioex_cmd_get_all_mode(GPIOEX_IOCTL_MODE_ALL_T *pstAllMode)
{
    LOCAL GPIOEX_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    
    if (NULL == pstAllMode)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(GPIOEX_IOCTL_T));
    enRet = gpioex_ioctl(&stIoctl, GPIOEX_IOCTL_CMD_ALL_MODE_GET);
    pstAllMode->uiValue = stIoctl.unData.stAllMode.uiValue;
    
    return enRet;
}

AP_ERROR_CODE_E gpioex_cmd_set_mode(GPIOEX_IOCTL_MODE_T stMode)
{
    LOCAL GPIOEX_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    
    if (stMode.uiNum > 15)
    {
        return AP_E_PARAM;
    }
    if (stMode.uiValue > 1)
    {
        return AP_E_PARAM;
    }
    memset(&stIoctl, 0, sizeof(GPIOEX_IOCTL_T));
    stIoctl.unData.stMode.uiNum= stMode.uiNum;
    stIoctl.unData.stMode.uiValue= stMode.uiValue;
    enRet = gpioex_ioctl(&stIoctl, GPIOEX_IOCTL_CMD_MODE_SET);
    
    return enRet;
}

AP_ERROR_CODE_E gpioex_cmd_get_mode(GPIOEX_IOCTL_MODE_T *pstMode)
{
    LOCAL GPIOEX_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    
    if (NULL == pstMode)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(GPIOEX_IOCTL_T));
    stIoctl.unData.stMode.uiNum= pstMode->uiNum;
    enRet = gpioex_ioctl(&stIoctl, GPIOEX_IOCTL_CMD_MODE_GET);
    pstMode->uiValue = stIoctl.unData.stMode.uiValue;
    
    return enRet;
}

AP_ERROR_CODE_E gpioex_cmd_set_all_state(GPIOEX_IOCTL_STATE_ALL_T stAllMode)
{
    LOCAL GPIOEX_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    
    if (stAllMode.uiValue > 0xffff)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(GPIOEX_IOCTL_T));
    stIoctl.unData.stAllMode.uiValue= stAllMode.uiValue;
    enRet = gpioex_ioctl(&stIoctl, GPIOEX_IOCTL_CMD_ALL_STATE_SET);
    
    return enRet;
}

AP_ERROR_CODE_E gpioex_cmd_get_all_state(GPIOEX_IOCTL_STATE_ALL_T *pstAllState)
{
    LOCAL GPIOEX_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    
    if (NULL == pstAllState)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(GPIOEX_IOCTL_T));
    enRet = gpioex_ioctl(&stIoctl, GPIOEX_IOCTL_CMD_ALL_STATE_GET);
    pstAllState->uiValue = stIoctl.unData.stAllState.uiValue;
    
    return enRet;
}

AP_ERROR_CODE_E gpioex_cmd_set_state(GPIOEX_IOCTL_STATE_T stState)
{
    LOCAL GPIOEX_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    
    if (stState.uiNum > 15)
    {
        return AP_E_PARAM;
    }
    if (stState.uiValue > 1)
    {
        return AP_E_PARAM;
    }
    memset(&stIoctl, 0, sizeof(GPIOEX_IOCTL_T));
    stIoctl.unData.stState.uiNum= stState.uiNum;
    stIoctl.unData.stState.uiValue= stState.uiValue;
    enRet = gpioex_ioctl(&stIoctl, GPIOEX_IOCTL_CMD_STATE_SET);
    
    return enRet;
}

AP_ERROR_CODE_E gpioex_cmd_get_state(GPIOEX_IOCTL_STATE_T *pstState)
{
    LOCAL GPIOEX_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    
    if (NULL == pstState)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(GPIOEX_IOCTL_T));
    stIoctl.unData.stState.uiNum= pstState->uiNum;
    enRet = gpioex_ioctl(&stIoctl, GPIOEX_IOCTL_CMD_STATE_GET);
    pstState->uiValue = stIoctl.unData.stState.uiValue;
    
    return enRet;
}