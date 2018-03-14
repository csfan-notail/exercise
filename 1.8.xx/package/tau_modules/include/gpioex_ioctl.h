/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : gpioex_ioctl.h
  版 本 号   : 初稿
  作    者   : Zhong Huaqing
  生成日期   : 2012年8月10日 星期五
  最近修改   :
  功能描述   : gpioex_ioctl.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2012年8月10日 星期五
    作    者   : Zhong Huaqing
    修改内容   : 创建文件

******************************************************************************/

#ifndef __GPIOEX_IOCTL_H__
#define __GPIOEX_IOCTL_H__


/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "ap_types.h"

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
 * 基础类型定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define GPIOEX_PROC_FILE        "gpioex"

#define GPIOEX_IOCTL_CMD_BEGIN 0


#define GPIOEX_MOD_VERSION  "v1.0.1"
#define GPIOEX_APP_VERSION  "v1.0.1"

#define GPIOEX_MOD_DEP_VERSION "v1.0.1"   /*支持内核模块的APP最小版本号*/
#define GPIOEX_APP_DEP_VERSION "v1.0.1"   /*支持APP的内核模块最小版本号*/


 /*----------------------------------------------*
 * 宏操作                                       *
 *----------------------------------------------*/


 
/*----------------------------------------------*
 * 枚举定义                                       *
 *----------------------------------------------*/
typedef enum
{
    GPIOEX_IOCTL_CMD_GET_VERSION = GPIOEX_IOCTL_CMD_BEGIN,
    
    GPIOEX_IOCTL_CMD_SET_DEBUG_LEVEL,
    GPIOEX_IOCTL_CMD_GET_DEBUG_LEVEL,

    GPIOEX_IOCTL_CMD_ALL_MODE_SET,
    GPIOEX_IOCTL_CMD_ALL_MODE_GET,
    GPIOEX_IOCTL_CMD_MODE_SET,
    GPIOEX_IOCTL_CMD_MODE_GET,
    GPIOEX_IOCTL_CMD_ALL_STATE_SET,
    GPIOEX_IOCTL_CMD_ALL_STATE_GET,
    GPIOEX_IOCTL_CMD_STATE_SET,
    GPIOEX_IOCTL_CMD_STATE_GET,
    
    /*可以对模板中的命令进行修改或增减*/
    /*.......*/
}GPIOEX_IOCTL_CMD_E;

/*----------------------------------------------*
 * 扩展类型定义                                     *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 结构定义                                       *
 *----------------------------------------------*/
typedef struct
{
    #define GPIOEX_VERSION_SIZE  128
    CHAR acModVersion[GPIOEX_VERSION_SIZE];    /*内核模块版本号*/
    CHAR acModDepVersion[GPIOEX_VERSION_SIZE]; /*支持内核模块的APP模块最小版本号*/
}GPIOEX_IOCTL_VERSION_T;


typedef struct
{
    UINT32 uiDebugLevel;
}GPIOEX_IOCTL_DEBUG_T;

typedef struct
{
    UINT32 uiValue;
}GPIOEX_IOCTL_MODE_ALL_T;


typedef struct
{
    UINT32 uiNum;
    UINT32 uiValue;
}GPIOEX_IOCTL_MODE_T;


typedef struct
{
    UINT32 uiValue;
}GPIOEX_IOCTL_STATE_ALL_T;


typedef struct
{
    UINT32 uiNum;
    UINT32 uiValue;
}GPIOEX_IOCTL_STATE_T;


/*可以对模板中的命令进行修改或增减*/
/*.......*/

typedef struct
{
	GPIOEX_IOCTL_CMD_E enCmd;

	union
	{
	    GPIOEX_IOCTL_VERSION_T stVersion;
	    GPIOEX_IOCTL_DEBUG_T stDebug;
	    GPIOEX_IOCTL_MODE_ALL_T stAllMode;
	    GPIOEX_IOCTL_MODE_T stMode;
	    GPIOEX_IOCTL_STATE_ALL_T stAllState;
	    GPIOEX_IOCTL_STATE_T stState;
	    
	    /*可以对模板中的命令进行修改或增减*/
        /*.......*/
	}unData;
}GPIOEX_IOCTL_T;




/*----------------------------------------------*
 * 导出函数原型说明                             *
 *----------------------------------------------*/



#endif /* __GPIOEX_IOCTL_H__ */

