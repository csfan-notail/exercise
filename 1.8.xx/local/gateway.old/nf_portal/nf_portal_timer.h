/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : nf_portal_timer.h
  版 本 号   : 初稿
  作    者   : WeiHonggang
  生成日期   : 2013年8月8日 星期四
  最近修改   :
  功能描述   : nf_portal_timer.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2013年8月8日 星期四
    作    者   : WeiHonggang
    修改内容   : 创建文件

******************************************************************************/

#ifndef __NF_PORTAL_TIMER_H__
#define __NF_PORTAL_TIMER_H__


/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/

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

#define NF_PORTAL_DEF_INTEVAL (60*3*HZ)
 /*----------------------------------------------*
 * 宏操作                                       *
 *----------------------------------------------*/



 
/*----------------------------------------------*
 * 枚举定义                                       *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 扩展类型定义                                     *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 结构定义                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 导出函数原型说明                             *
 *----------------------------------------------*/
AP_ERROR_CODE_E nf_portal_timer_init(VOID);
AP_ERROR_CODE_E nf_portal_timer_destroy(VOID);
#endif /* __NF_PORTAL_TIMER_H__ */

