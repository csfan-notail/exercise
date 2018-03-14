/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : mirror.h
  版 本 号   : 初稿
  作    者   : xiao wei
  生成日期   : 2016年3月6日 星期三
  最近修改   :
  功能描述   : mirror.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2016年3月6日 星期三
    作    者   : xiao wei
    修改内容   : 创建文件

******************************************************************************/

#ifndef __mirror_MAIN_H__
#define __mirror_MAIN_H__

/*----------------------------------------------*
 * 包含头文件                                   *
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
#define LOCAL                       static

#define MIRROR_VERSION_NUM          "1.0"

#define ATH_MIRROR_ENABLE_ADDRESS               0x0058
#define ATH_MIRROR_MIRRING_PORT_ADDRESS         0x0078
#define ATH_MIRROR_OBSERVE_PORT0_ADDRESS        0x0104
#define ATH_MIRROR_OBSERVE_PORT1_ADDRESS        0x0204
#define ATH_MIRROR_OBSERVE_PORT2_ADDRESS        0x0304
#define ATH_MIRROR_OBSERVE_PORT3_ADDRESS        0x0404
#define ATH_MIRROR_OBSERVE_PORT4_ADDRESS        0x0504
#define ATH_MIRROR_OBSERVE_PORT5_ADDRESS        0x0604

#define ATHR_GMAC_ETH_CFG                       0x18070000
 /*----------------------------------------------*
 * 宏操作                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 枚举定义                                       *
 *----------------------------------------------*/
typedef enum
{
    MIRROR_LOG_EMERG = 0,       /* system is unusable */
    MIRROR_LOG_ALERT,           /* action must be taken immediately */
    MIRROR_LOG_CRIT,            /* critical conditions */
    MIRROR_LOG_ERR,             /* error conditions */
    MIRROR_LOG_WARNING,         /* warning conditions */
    MIRROR_LOG_NOTICE,          /* normal but significant condition */
    MIRROR_LOG_INFO,            /* informational */
    MIRROR_LOG_DEBUG,           /* debug-level messages */
}MIRROR_LOG_LEVEL_E;

MIRROR_LOG_LEVEL_E g_enDebugLevel = MIRROR_LOG_ERR;
//if (level <= g_enDebugLevel)
#define MIRROR_LOG(level, fmt, args...)                  \
do{                                             \
    if (level <= MIRROR_LOG_DEBUG)                            \
    {                                           \
        printk("mirror %s(%d): ",__FUNCTION__, __LINE__); \
        printk(fmt, ##args);                    \
    }                                           \
}while(0)


#endif /* __gpio_MAIN_H__ */

