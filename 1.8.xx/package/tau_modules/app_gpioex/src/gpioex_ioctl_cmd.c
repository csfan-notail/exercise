/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : gpioex_cmd.c
  版 本 号   : 初稿
  作    者   : Zhong Huaqing
  生成日期   : 2012年8月15日 星期三
  最近修改   :
  功能描述   : 流控命令解析与执行
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

#include "gpioex_ioctl.h"

#include "gpioex_ioctl_main.h"
#include "gpioex_ioctl_ops.h"
#include "gpioex_ioctl_cmd.h"

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/
AP_ERROR_CODE_E gpioex_cmd_help(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E gpioex_cmd_version(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E gpioex_cmd_debug_level(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E gpioex_cmd_all_mode(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E gpioex_cmd_mode(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E gpioex_cmd_all_state(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E gpioex_cmd_state(INT32 iArgc, CHAR *apcArgv[]);




/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/
LOCAL GPIOEX_CMD_T g_astCmd[] =
{
	{"help", gpioex_cmd_help, 
	    "[subcmd]",
	    "Show help information. {} means \"Required\"; [] means \"Optional\"; | means \"One of\""
    },
	{"version", gpioex_cmd_version, 
	    "",
	    "Get Current/Dependent version of application and module"
    },
	{"debuglevel", gpioex_cmd_debug_level, 
	    "[0-7]",
	    "Set or get debug level of kernel module. Set 7 to print the most information"
    },
	{"all_mode", gpioex_cmd_all_mode, 
	    "[0x0-0xffff]",
	    "Set or get all gpio expander mode value"
    },
	{"mode", gpioex_cmd_mode, 
	    "{0-15} [0|1]",
	    "Set or get gpio expander mode value"
    },
    {"all_state", gpioex_cmd_all_state, 
	    "[0x0-0xffff]",
	    "Set or get all gpio expander state value"
    },
    {"state", gpioex_cmd_state, 
	    "{0-15} [0|1]",
	    "Set or get gpio expander state value"
    },

	/*......*/
	
	{NULL, NULL, NULL, NULL}
};

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 函数定义                                       *
 *----------------------------------------------*/

GPIOEX_CMD_T *gpioex_get_cmd(CHAR *szCmdName)
{
    GPIOEX_CMD_T *pstPos;
    for (pstPos = g_astCmd; pstPos->fHandler; pstPos++)
    {
        if (STR_MATCH(szCmdName, pstPos->szCmdName))
        {
            return pstPos;
        }
    }
    return NULL;
}

void gpioex_show_help(CHAR *szCmdName)
{
    GPIOEX_CMD_T *pstPos;
    
    for (pstPos = g_astCmd; pstPos->fHandler; pstPos++)
    {
        if (NULL != szCmdName)
        {
            if (!STR_MATCH(szCmdName, pstPos->szCmdName))
            {
                continue;
            }
        }
        
        printf("    %-15s -- %s\n", pstPos->szCmdName, pstPos->szCmdFmt);
        printf("    %-15s    %s\n", "", pstPos->szHelpInfo);
    }
    return;
}


AP_ERROR_CODE_E gpioex_show_version(GPIOEX_IOCTL_VERSION_T *pstVersion)
{
    if (NULL == pstVersion)
    {
        return AP_E_PARAM;
    }
    
    printf("%-20s: %s\n", "mod version", pstVersion->acModVersion);
    printf("%-20s: %s\n", "app version", GPIOEX_APP_VERSION);
    printf("%-20s: %s\n", "mod dep version", pstVersion->acModDepVersion);
    printf("%-20s: %s\n", "app dep version", GPIOEX_APP_DEP_VERSION);
    
    return AP_E_NONE;
}

INT32 gpioex_parse_version_num(CHAR *szVer)
{
    INT32 iMajor; /*主号*/
    INT32 iMinor; /*次号*/
    INT32 iRev;   /*修订号revision*/
    INT32 iVerNum;
    
    INT32 iNum;
    
    if (NULL == szVer)
    {
        return -1;
    }
    
    iNum = sscanf(szVer, "v%d.%d.%d",
                    &iMajor, &iMinor, &iRev);
    if (3 != iNum)
    {
        printf("Version is invalid.\n");
        return -1;
    }
    if (iMajor > 255 
        || iMinor > 255
        || iRev > 255)  /*版本号非法。*/
    {
        printf("Version is out of range.\n");
        return -1;
    }
    
    iVerNum = (iMajor << 16) + (iMinor << 8) + iRev;
    return iVerNum;
    
}

AP_ERROR_CODE_E gpioex_check_version(GPIOEX_IOCTL_VERSION_T *pstVersion)
{
    INT32 iAppVerNum;
    INT32 iModVerNum;
    
    INT32 iAppDepVerNum;
    INT32 iModDepVerNum;
    
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstVersion)
    {
        return AP_E_PARAM;
    }
    
    iAppVerNum = gpioex_parse_version_num(GPIOEX_APP_VERSION);
    iModVerNum = gpioex_parse_version_num(pstVersion->acModVersion);
    
    iAppDepVerNum = gpioex_parse_version_num(GPIOEX_APP_DEP_VERSION);
    iModDepVerNum = gpioex_parse_version_num(pstVersion->acModDepVersion);
    
    if (iAppVerNum < 0
        || iModVerNum < 0
        || iAppDepVerNum < 0
        || iModDepVerNum < 0
        )
    {
        return AP_E_DEFAULT_FAIL;
    }
    
    if (iAppVerNum >= iModDepVerNum
        && iModVerNum >= iAppDepVerNum)
    {
        return AP_E_NONE;
    }
    else
    {
        return AP_E_DEFAULT_FAIL;
    }
}

AP_ERROR_CODE_E gpioex_cmd_version(INT32 iArgc, CHAR *apcArgv[])
{
    GPIOEX_IOCTL_VERSION_T stVersion;
    AP_ERROR_CODE_E enRet;
    
        
    if (1 == iArgc)
    {
        enRet = gpioex_cmd_get_mod_version(&stVersion);
        if (AP_E_NONE == enRet)
        {
            gpioex_show_version(&stVersion);
            enRet = gpioex_check_version(&stVersion);
            if (AP_E_NONE != enRet)
            {
                printf("Warning: Module and app does not match in version.\n");
                return AP_E_NONE;
            }
        }
        return enRet;
    }
    else
    {
        return AP_E_PARAM;
    }
}


AP_ERROR_CODE_E gpioex_cmd_help(INT32 iArgc, CHAR *apcArgv[])
{
    if (1 == iArgc)
    {
        gpioex_show_help(NULL);
    }
    else if (2 == iArgc)
    {
        gpioex_show_help(apcArgv[1]);
    }
    else
    {
        return AP_E_PARAM;
    }
    
    return AP_E_NONE;
}


/*****************************************************************************
 函 数 名  : gpioex_cmd_debug_level
 功能描述  : 设置/获取调试输出级别
 输入参数  : INT32 iArgc      
             CHAR *apcArgv[]  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月27日 星期四
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E gpioex_cmd_debug_level(INT32 iArgc, CHAR *apcArgv[])
{
    UINT32 uiDebugLevel;
    AP_ERROR_CODE_E enRet;    
    
    if (1 == iArgc)
    {
        enRet = gpioex_cmd_get_debug_level(&uiDebugLevel);
        if (AP_E_NONE == enRet)
        {
            printf("Debug level: %d\n", uiDebugLevel);
        }
        return enRet;
    }
    else if (2 == iArgc)
    {
        uiDebugLevel = atoi(apcArgv[1]);
        enRet = gpioex_cmd_set_debug_level(uiDebugLevel);
        if (AP_E_NONE == enRet)
        {
            printf("Set OK!\n");
        }
        return enRet;
    }
    else
    {
        return AP_E_PARAM;
    }
}

AP_ERROR_CODE_E gpioex_cmd_all_mode(INT32 iArgc, CHAR *apcArgv[])
{
    GPIOEX_IOCTL_MODE_ALL_T stAllMode = {0};
    AP_ERROR_CODE_E enRet;
        
    if (1 == iArgc)
    {
        enRet = gpioex_cmd_get_all_mode(&stAllMode);
        if (AP_E_NONE == enRet)
        {
            printf("Get gpio all mode: %x \n", stAllMode.uiValue);
        }
        return enRet;
    }
    else if (2 == iArgc)
    {
        stAllMode.uiValue = atoi(apcArgv[1]);
        enRet = gpioex_cmd_set_all_mode(stAllMode);
        if (AP_E_NONE == enRet)
        {
            printf("Set OK!\n");
        }
        return enRet;
    }
    else
    {
        return AP_E_PARAM;
    }
}

AP_ERROR_CODE_E gpioex_cmd_mode(INT32 iArgc, CHAR *apcArgv[])
{
    GPIOEX_IOCTL_MODE_T stMode = {0,0};
    AP_ERROR_CODE_E enRet;
    
    
    if (1 == iArgc)
    {
        printf("please input more parameters\n");
        return AP_E_PARAM;
    }
    if (2 == iArgc)
    {
        stMode.uiNum= atoi(apcArgv[1]);
        enRet = gpioex_cmd_get_mode(&stMode);
        if (AP_E_NONE == enRet)
        {
            printf("Get gpio expander %d: %x\n", stMode.uiNum,stMode.uiValue);
        }
        return enRet;
    }
    else if (3 == iArgc)
    {
        stMode.uiNum= atoi(apcArgv[1]);
        stMode.uiValue = atoi(apcArgv[2]);
        enRet = gpioex_cmd_set_mode(stMode);
        if (AP_E_NONE == enRet)
        {
            printf("Set OK!\n");
        }
        return enRet;
    }
    else
    {
        return AP_E_PARAM;
    }
}

AP_ERROR_CODE_E gpioex_cmd_all_state(INT32 iArgc, CHAR *apcArgv[])
{
    GPIOEX_IOCTL_STATE_ALL_T stAllState = {0};
    AP_ERROR_CODE_E enRet;
    
    
    if (1 == iArgc)
    {
        enRet = gpioex_cmd_get_all_state(&stAllState);
        if (AP_E_NONE == enRet)
        {
            printf("Get gpio all state: %x \n", stAllState.uiValue);
        }
        return enRet;
    }
    else if (2 == iArgc)
    {
        stAllState.uiValue = atoi(apcArgv[1]);
        enRet = gpioex_cmd_set_all_state(stAllState);
        if (AP_E_NONE == enRet)
        {
            printf("Set OK!\n");
        }
        return enRet;
    }
    else
    {
        return AP_E_PARAM;
    }
}

AP_ERROR_CODE_E gpioex_cmd_state(INT32 iArgc, CHAR *apcArgv[])
{
    GPIOEX_IOCTL_STATE_T stState = {0,0};
    AP_ERROR_CODE_E enRet;
    
    
    if (1 == iArgc)
    {
        printf("please input more parameters\n");
        return AP_E_PARAM;
    }
    else if (2 == iArgc)
    {
        stState.uiNum= atoi(apcArgv[1]);
        enRet = gpioex_cmd_get_state(&stState);
        if (AP_E_NONE == enRet)
        {
            printf("Get gpio expander %d: %x\n", stState.uiNum,stState.uiValue);
        }
        return enRet;
    }
    else if (3 == iArgc)
    {
        stState.uiNum= atoi(apcArgv[1]);
        stState.uiValue = atoi(apcArgv[2]);
        enRet = gpioex_cmd_set_state(stState);
        if (AP_E_NONE == enRet)
        {
            printf("Set OK!\n");
        }
        return enRet;
    }
    else
    {
        return AP_E_PARAM;
    }
}


