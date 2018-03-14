/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : timeParams.h
  版 本 号   : 初稿
  作    者   : zhangjiasi
  生成日期   : 2015年6月4日
  最近修改   :
  功能描述   : timeParams.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2015年6月4日
    作    者   : zhangjiasi
    修改内容   : 创建文件

******************************************************************************/
#ifndef __TIMEPARAMS_H__
#define __TIMEPARAMS_H__
/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "tr69cdefs.h"
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

/* InternetGatewayDevice.Time. */
SVAR(Enable);
SVAR(Status);

SVAR(NTPServer1);
SVAR(NTPServer2);
SVAR(CurrentLocalTime);
SVAR(LocalTimeZone);
SVAR(LocalTimeZoneName);
SVAR(ConfigType);
SVAR(X_ATP_MannualTime);



/* 函数原型定义 */
/* InternetGatewayDevice.Time. */

TRXSFUNC(setTimeEnable);
TRXGFUNC(getTimeEnable);
TRXGFUNC(getTimeStatus);

TRXSFUNC(setNTPServer1);
TRXGFUNC(getNTPServer1);
TRXSFUNC(setNTPServer2);
TRXGFUNC(getNTPServer2);
TRXGFUNC(getCurrentLocalTime);
TRXSFUNC(setLocalTimeZone);
TRXGFUNC(getLocalTimeZone);
TRXSFUNC(setLocalTimeZoneName);
TRXGFUNC(getLocalTimeZoneName);
TRXSFUNC(setConfigType);
TRXGFUNC(getConfigType);
TRXSFUNC(setX_ATP_MannualTime);
TRXGFUNC(getX_ATP_MannualTime);




#endif /* __TIMEPARAMS_H__ */
