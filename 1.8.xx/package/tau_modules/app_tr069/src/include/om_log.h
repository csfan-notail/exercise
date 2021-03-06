/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : om_log.h
  版 本 号   : 初稿
  作    者   : Zhong Huaqing
  生成日期   : 2012年7月23日 星期一
  最近修改   :
  功能描述   : om_log.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2012年7月23日 星期一
    作    者   : Zhong Huaqing
    修改内容   : 创建文件

******************************************************************************/

#ifndef __OM_LOG_H__
#define __OM_LOG_H__






/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "om_types.h"
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
REMOTE INT32 g_iOmLogLevel;
/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 基础类型定义                                     *
 *----------------------------------------------*/
 
 
/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
#define	OM_LOG_EMERG	0	/* system is unusable */
#define	OM_LOG_ALERT	1	/* action must be taken immediately */
#define	OM_LOG_CRIT	    2	/* critical conditions */
#define	OM_LOG_ERR		3	/* error conditions */
#define	OM_LOG_WARNING	4	/* warning conditions */
#define	OM_LOG_NOTICE	5	/* normal but significant condition */
#define	OM_LOG_INFO	    6	/* informational */
#define	OM_LOG_DEBUG	7	/* debug-level messages */


/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define OM_LOG(level, format, arg...) \
do{                                    \
    if (level <= g_iOmLogLevel)       \
    {                                  \
        syslog(level,"{OM}[fun:%s],[line:%d]"format, __func__,__LINE__,##arg);   \
    }                                  \
}while(0);


 /*----------------------------------------------*
  * 宏定义                                       *
  *----------------------------------------------*/

 /*----------------------------------------------*
 * 宏操作                                       *
 *----------------------------------------------*/


 
/*----------------------------------------------*
 * 枚举定义                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 结构定义                                       *
 *----------------------------------------------*/



#endif /* __OM_LOG_H__ */

