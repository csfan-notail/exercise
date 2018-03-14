/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : gpioex_main.h
  版 本 号   : 初稿
  作    者   : Zhong Huaqing
  生成日期   : 2012年8月10日 星期五
  最近修改   :
  功能描述   : gpioex_main.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2012年8月10日 星期五
    作    者   : Zhong Huaqing
    修改内容   : 创建文件

******************************************************************************/

#ifndef __GPIOEX_MAIN_H__
#define __GPIOEX_MAIN_H__


/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/jiffies.h>
#include <linux/errno.h>
#include <linux/version.h>

#include <linux/skbuff.h>
#include <linux/jhash.h>

#include <linux/netdevice.h>
#include <linux/if_ether.h>

#include <linux/init.h>
#include <linux/mutex.h>
#include <asm/addrspace.h>
#include "xra1403.h"
#include "ar9344.h"
#include <linux/delay.h>
#include "ap_types.h"
#include "gpioex_ops.h"




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

 /*----------------------------------------------*
 * 宏操作                                       *
 *----------------------------------------------*/
#define GPIOEX_LOG(level, fmt, args...)                  \
do{											    \
	if (level <= g_enDebugLevel)					        \
	{											\
		printk("GPIOEX %s(%d): ",__FUNCTION__, __LINE__); \
		printk(fmt, ##args);				    \
	}										    \
}while(0)

/*----------------------------------------------*
 * 枚举定义                                       *
 *----------------------------------------------*/
typedef enum
{
    GPIOEX_LOG_EMERG = 0,	/* system is unusable */
    GPIOEX_LOG_ALERT,	/* action must be taken immediately */
    GPIOEX_LOG_CRIT,	/* critical conditions */
    GPIOEX_LOG_ERR,	/* error conditions */
    GPIOEX_LOG_WARNING,	/* warning conditions */
    GPIOEX_LOG_NOTICE,	/* normal but significant condition */
    GPIOEX_LOG_INFO,	/* informational */
    GPIOEX_LOG_DEBUG,	/* debug-level messages */
}GPIOEX_LOG_LEVEL_E;
/*----------------------------------------------*
 * 扩展类型定义                                     *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 结构定义                                       *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 导出函数原型说明                             *
 *----------------------------------------------*/
 
AP_ERROR_CODE_E gpioex_set_debugLevel(UINT32 uiDebugLevel);
UINT32 gpioex_get_debugLevel(void);

AP_ERROR_CODE_E gpioex_init(void);
AP_ERROR_CODE_E gpioex_destroy(void);

//extern int ath_flash_spi_down_trylock(void);
//extern int ath_flash_spi_down(void);
//extern void ath_flash_spi_up(void);



/*----------------------------------------------*
 * 导出变量说明                             *
 *----------------------------------------------*/
REMOTE GPIOEX_LOG_LEVEL_E g_enDebugLevel;

#endif /* __GPIOEX_MAIN_H__ */

