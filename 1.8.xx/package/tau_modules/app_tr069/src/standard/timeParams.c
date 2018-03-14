/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : timeParams.c
  版 本 号   : 初稿
  作    者   : zhangjiasi
  生成日期   : 2015年6月4日
  最近修改   :
  功能描述   : 网络时间同步相关信息。接口定义及树形结构拓扑定义
  函数列表   :
  修改历史   :
  1.日    期   : 2015年6月4日
    作    者   : zhangjiasi
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "timeParams.h"

#define TIMESTAUS_LEN 0
#define CONFIGTYPE_LEN 0
#define MANNUALTIME_LEN 64
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
TRxObjNode  TimeDesc[] = {
	{Enable,{{tBool,0,0}}, setTimeEnable,getTimeEnable,NULL,NULL},
	{Status,{{tString,TIMESTAUS_LEN,0}},NULL,getTimeStatus,NULL,NULL},
    {NTPServer1,{{tString,64,0}}, setNTPServer1,getNTPServer1,NULL,NULL},
    {NTPServer2,{{tString,64,0}}, setNTPServer2,getNTPServer2,NULL,NULL},
	{CurrentLocalTime,{{tString,64,0}}, NULL,getCurrentLocalTime,NULL,NULL},
	{LocalTimeZoneName,{{tString,64,0}}, setLocalTimeZoneName,getLocalTimeZoneName,NULL,NULL},
#if 0
	{LocalTimeZone,{{tString,6,0}}, setLocalTimeZone,getLocalTimeZone,NULL,NULL},
   	
	{ConfigType,{{tString,CONFIGTYPE_LEN,0}},setConfigType,getConfigType,NULL,NULL},
    {X_ATP_MannualTime,{{tString,MANNUALTIME_LEN,0}},setX_ATP_MannualTime,getX_ATP_MannualTime,NULL,NULL},
#endif  
    {NULL}        
};



