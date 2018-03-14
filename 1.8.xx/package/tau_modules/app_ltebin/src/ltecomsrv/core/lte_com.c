/******************************************************************************

                  版权所有 (C), 2001-2011, 神州数码网络有限公司

 ******************************************************************************
  文 件 名   : lte_com.c
  版 本 号   : 初稿
  作    者   : liyoukun
  生成日期   : 2015年3月3日星期二
  最近修改   :
  功能描述   : lte com主文件，用于初始化
  函数列表   :
  修改历史   :
  1.日    期   : 2015年3月3日星期二
    作    者   : liyoukun
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>  
#include <getopt.h>


#include "lte_com.h"
#include "lte_log.h"

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
static struct option const long_options[] = 
{
    {"-m lte module type, for example, longcher, zte-micro, arccra!",  required_argument,   NULL,'m'},
    {"-l log level!",                                                  required_argument,   NULL,'l'},    
    {"-h help",                                                        no_argument,         NULL,'h'},
    {NULL,                  0,              NULL, 0},
};

LTE_PLATFORM_E g_enLtePlatformType = M_TOTAL_NUM; /* 记录使用模块的型号 */
LTE_MODULE_OPS_T *g_pstLte_module_ops_t = NULL;

LTE_EVENT_T g_stLteEventT = {0};  /* 事件表 */
LTE_STATE_INFO_T g_stLteStateInfo = {0};    /* 静态获取的lte信息列表 */


/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
void usage(void)
{
    int iCount = 0;
    LTE_LOG(LTE_LOG_ERR, "Usage:");
    while (long_options[iCount].name != NULL)
    {
        LTE_LOG(LTE_LOG_ERR, "%s\n", long_options[iCount].name);
        iCount++;
    }
    return ;
}

LTE_RET_E get_parameter(int argc,char** args)
{
    int iIndex = 0 ;
    int iChar ;
    //while((iChar = getopt_long(argc, args, "a:p:s:d:i:c:r:", long_options, &iIndex)) != EOF)
    while((iChar = getopt_long(argc, args, "m:l:h:", long_options, &iIndex)) != EOF)
    {
        switch (iChar)
        {
            case 'm':
            {
                g_enLtePlatformType = atoi(optarg);
                break;
            }
            case 'l':
            {
                g_iLteLogLevel = atoi(optarg);    
                break;
            }
            case 'h':
            {
                usage();    
                return LTE_FAIL;
            }
            default:
            {
                usage();
                return LTE_FAIL;
            }
        }
    }
    
    return LTE_OK;
}

int main(int argc, char** args)
{
    LTE_RET_E enRet = LTE_OK;
    int       iRet = 0;
    char      acCom[32] = {0};
    int       iComFd = 0;
    /* 获取进程入参 */
    enRet = get_parameter(argc, args);
    if(LTE_OK != enRet)
    {
        LTE_LOG(LTE_LOG_ERR, "get param failed!");
        return -1;
    }

    /* 模块USB com口注册 */
    iRet = lte_usb_com_register(g_enLtePlatformType, &g_stLteEventT);
    if(iRet < 0)
    {
        LTE_LOG(LTE_LOG_ERR, "register lte usb com failed!");
        return -1;
    }
    
    /* libevent注册 */
    enRet = libevent_register(&g_stLteEventT);
    if(LTE_OK != enRet)
    {
        LTE_LOG(LTE_LOG_ERR, "register libevent failed!");
        return -1;
    }
    return 0;
}

