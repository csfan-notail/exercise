/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : mirror.c
  版 本 号   : 初稿
  作    者   : xiao wei
  生成日期   : 2016年3月6日 星期三
  最近修改   :
  功能描述   : mirror调试模块
  函数列表   :
  修改历史   :
  1.日    期   : 2016年3月6日 星期三
    作    者   : xiao wei
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

#include "mirror_ioctl.h"

#include "ap_types.h"
#include "procfs_fops.h"
#include "mirror.h"

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
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 函数定义                                       *
 *----------------------------------------------*/
AP_ERROR_CODE_E mirror_show_help();
AP_ERROR_CODE_E mirror_show_status();
AP_ERROR_CODE_E mirror_switch(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E mirror_setmirror_port(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E mirror_setobserve_port(INT32 iArgc, CHAR *apcArgv[]);

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

LOCAL MIRROR_CMD_T g_astCmd [MIRROR_CMD_MAX_NO] = 
{
    {"help", mirror_show_help, "mirror help",
            "Show help information. \n"
    },
    {"show", mirror_show_status, "mirror show",
            "Show mirror all config information. \n"
    },
    {"start", mirror_switch, "mirror start",
            "Open mirror function. \n"
    },
    {"stop", mirror_switch, "mirror stop",
            "Close mirror function. \n"
    },
    {"setmirror", mirror_setmirror_port, "mirror setmirror [port-num|close]",
            "Set mirror port number(no support port0) or close mirror port. eg: mirror setmirror close\n"
    },
    {"setobserve", mirror_setobserve_port, "mirror setobserve [port-num] [ingress|egress|both|close]",
            "Set observe port number(no support port0) and rx/tx mode. eg: mirror setobserve 2 both \n"
    },
    {NULL, NULL, NULL, NULL}
};


/*----------------------------------------------*
 * 函数定义                                     *
 *----------------------------------------------*/

MIRROR_CMD_T *mirror_get_cmd(CHAR *szCmdName)
{
    MIRROR_CMD_T *pstPos;
    for (pstPos = g_astCmd; pstPos->fHandler; pstPos++)
    {
        if (STR_MATCH(szCmdName, pstPos->szCmdName))
        {
            return pstPos;
        }
    }
    return NULL;
}

AP_ERROR_CODE_E mirror_show_help()
{
    MIRROR_CMD_T *pstPos;
    
    printf("\nmirror help information: \n\n");
    for (pstPos = g_astCmd; pstPos->fHandler; pstPos++)
    {
        printf("    %-15s -- CMD: %s\n", pstPos->szCmdName, pstPos->szCmdFmt);
        printf("    %-15s    %s\n", "", pstPos->szHelpInfo);
    }

    return AP_E_NONE;
}

AP_ERROR_CODE_E mirror_show_status()
{
    INT32 enRet   =0;
    INT32 iLoop   =0;
    LOCAL MIRROR_IOCTL_T stIoctl = {0}; 
    
    memset(&stIoctl, 0, sizeof(MIRROR_IOCTL_T));

    enRet = mirror_ioctl(&stIoctl, MIRROR_IOCTL_SHOW);
    if (AP_E_NONE == enRet)
    {
        //printf("\nNote : mirror only support LAN port !\n");
        printf("\nMirror status :\n");
        if( MIRROR_DISABLE == stIoctl.stMirrorData.uiMirrorSw )
        {
            printf("\n-----Port mirror disable. -----\n");
        }
        else
        {
            printf("\n-----Port mirror enabled. -----\n");
        } 

        if( stIoctl.stMirrorData.uiMirrorPort > MIRROR_PORT_MAX_NUM )
        {
            printf("\n-----No set mirror port ! -----\n\n");
        }
        else
        {
            printf("\n-----Mirror port: %d      -----\n\n", stIoctl.stMirrorData.uiMirrorPort);
        }

        for( iLoop = stIoctl.stMirrorData.stGmacStatus.uiStartPort; iLoop <= stIoctl.stMirrorData.stGmacStatus.uiEndPort; iLoop++ )  
        {
            if( MIRROR_IOCTL_OBSERVE_PORT_RX == stIoctl.stMirrorData.stObservePort[iLoop].enObservePortCurr )
            {
                printf("Observer port%d open rx.\n", iLoop);
            }
            else if( MIRROR_IOCTL_OBSERVE_PORT_TX == stIoctl.stMirrorData.stObservePort[iLoop].enObservePortCurr )
            {
                printf("Observer port%d open tx.\n", iLoop);
            }
            else if( MIRROR_IOCTL_OBSERVE_PORT_ALL == stIoctl.stMirrorData.stObservePort[iLoop].enObservePortCurr )
            {
                printf("Observer port%d open rx/tx.\n", iLoop);
            }
            else
            {
                printf("Observer port%d close.\n", iLoop);
            }
        } 
    }
    else
    {
        printf("Call mirror_ioctl failed or No command enRet = %d\n", enRet);
        return AP_E_DEFAULT_FAIL;
    }
    printf("\n");

    return AP_E_NONE;
}

AP_ERROR_CODE_E mirror_switch(INT32 iArgc, CHAR *apcArgv[])
{
    INT32 enRet   =0;
    LOCAL MIRROR_IOCTL_T stIoctl = {0}; 
    
    memset(&stIoctl, 0, sizeof(MIRROR_IOCTL_T));

    if( !strcmp(apcArgv[0], "start") )
    {
        stIoctl.stMirrorData.uiMirrorSw = MIRROR_ENABLE;
    }
    else
    {
        stIoctl.stMirrorData.uiMirrorSw = MIRROR_DISABLE;
    }

    enRet = mirror_ioctl(&stIoctl, MIRROR_IOCTL_ENABLE);
    if (AP_E_NONE == enRet)
    {
        if( MIRROR_SET_SUCCESS == stIoctl.stMirrorData.uiSetStatu )
        {
            printf("mirror set success !\n");
        }
        else
        {
            printf("mirror set failed !\n");
        }
    }
    else
    {
        printf("Call mirror_ioctl failed or No command enRet = %d\n", enRet);
        return AP_E_DEFAULT_FAIL;
    }
    
    return AP_E_NONE;
}

AP_ERROR_CODE_E mirror_setmirror_port(INT32 iArgc, CHAR *apcArgv[])
{
    INT32 enRet   =0;
    LOCAL MIRROR_IOCTL_T stIoctl = {0}; 
    
    memset(&stIoctl, 0, sizeof(MIRROR_IOCTL_T));

    if( iArgc < 2 )
    {
        printf("mirror setmirror command iArgc number error !\n");
        return AP_E_DEFAULT_FAIL;
    }
    if(  strcmp(apcArgv[0], "setmirror") \
        || (atoi(apcArgv[1]) < MIRROR_PORT0 ||  atoi(apcArgv[1]) > MIRROR_PORT5 ) )
    {
        printf("mirror setmirror command apcArgv date error !\n");
        return AP_E_DEFAULT_FAIL;
    }

    if( !strcmp(apcArgv[1], "close") )
    {
        stIoctl.stMirrorData.uiMirrorPort = MIRROR_PORT_CLOSE;
    }
    else
    {
        stIoctl.stMirrorData.uiMirrorPort = atoi(apcArgv[1]);
    }

    enRet = mirror_ioctl(&stIoctl, MIRROR_IOCTL_MIRROR);
    if (AP_E_NONE == enRet)
    {
        if( MIRROR_SET_SUCCESS == stIoctl.stMirrorData.uiSetStatu )
        {
            printf("setmirror success !\n");
        }
        else
        {
            printf("setmirror failed !\n");
        }
    }
    else
    {
        printf("Call mirror_ioctl failed or No command enRet = %d\n", enRet);
        return AP_E_DEFAULT_FAIL;
    }
    
    return AP_E_NONE;
}

AP_ERROR_CODE_E mirror_setobserve_port(INT32 iArgc, CHAR *apcArgv[])
{
    INT32 enRet     = 0;
    INT32 iPortMode = 0;
    LOCAL MIRROR_IOCTL_T stIoctl = {0}; 
    
    memset(&stIoctl, 0, sizeof(MIRROR_IOCTL_T));

    if( iArgc < 3 )
    {
        printf("mirror setobserve command iArgc error !\n");
        return AP_E_DEFAULT_FAIL;
    }
    if(  strcmp(apcArgv[0], "setobserve") \
        || (atoi(apcArgv[1]) < MIRROR_PORT0 ||  atoi(apcArgv[1]) > MIRROR_PORT5) \
        || ( strcmp(apcArgv[2],"ingress") && strcmp(apcArgv[2],"egress") && strcmp(apcArgv[2],"both") && strcmp(apcArgv[2],"close") ) )
    {
        printf("mirror setobserve command apcArgv error !\n");
        return AP_E_DEFAULT_FAIL;
    }
    if( !strcmp(apcArgv[2],"ingress") )
    {
        iPortMode = MIRROR_IOCTL_OBSERVE_PORT_RX;
    }
    else if( !strcmp(apcArgv[2],"egress") )
    {
        iPortMode = MIRROR_IOCTL_OBSERVE_PORT_TX;
    }
    else if( !strcmp(apcArgv[2],"both") )
    {
        iPortMode = MIRROR_IOCTL_OBSERVE_PORT_ALL;
    }
    else
    {
        iPortMode = MIRROR_IOCTL_OBSERVE_PORT_NO;
    }
    
    stIoctl.stMirrorData.stObservePort[atoi(apcArgv[1])].enObservePortCurr = iPortMode;
    stIoctl.stMirrorData.uiObservePort = atoi(apcArgv[1]);

    enRet = mirror_ioctl(&stIoctl, MIRROR_IOCTL_OBSERVE);
    if (AP_E_NONE == enRet)
    {
        if( MIRROR_SET_SUCCESS == stIoctl.stMirrorData.uiSetStatu )
        {
            printf("setobserve success !\n");
        }
        else
        {
            printf("setobserve failed !\n");
        }
    }
    else
    {
        printf("Call mirror_ioctl failed or No command enRet = %d\n", enRet);
        return AP_E_DEFAULT_FAIL;
    }

    return AP_E_NONE;
}

INT32 main(INT32 iArgc, CHAR *apcArgv[])
{
	AP_ERROR_CODE_E enRet;
	MIRROR_CMD_T *pstCmd;
    CHAR *szCmdName = NULL;

    //printf("Input iArgc=%d\n", iArgc);

    if( 1 == iArgc ) 
    {
        mirror_show_help();
        return 0;
    }

    /* find mirror cmd */
    szCmdName = apcArgv[1];
    pstCmd = mirror_get_cmd(szCmdName);
    if (NULL == pstCmd)
    {
        printf("Command no find ! please use: mirror help\n");
        return -1;
    }
    
    /* execute cmd */
    iArgc--;
    apcArgv++;
    enRet = pstCmd->fHandler(iArgc, apcArgv);
    if (AP_E_NONE != enRet)
    {
        printf("Command %s executed failed. ret %d\n", szCmdName, enRet);
    }
    
	return 0;
}