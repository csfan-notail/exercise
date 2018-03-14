/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : manageServerParams.c
  版 本 号   : 初稿
  作    者   : zhangjiasi
  生成日期   : 2015年6月4日
  最近修改   :
  功能描述   : 管理服务器相关信息。接口定义及树形结构拓扑定义
  函数列表   :
  修改历史   :
  1.日    期   : 2015年6月4日
    作    者   : zhangjiasi
    修改内容   : 创建文件

******************************************************************************/
/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#define CPEVARNAMEINSTANCE

#include "manageServerParams.h"
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
TRxObjNode  mgmtServerDesc[] = {

	{URL,{{tString,256,0}}, setURL,getURL,NULL,NULL},
    {Username,{{tString,256,0}}, setUsername,getUsername,NULL,NULL},
    {Password,{{tString,256,0}}, setPassword,getPassword,NULL,NULL},
    {PeriodicInformEnable,{{tBool,0,0}}, setPeriodicInformEnable,getPeriodicInformEnable,NULL,NULL},
  	{PeriodicInformInterval,{{tUnsigned,0,0}},setPeriodicInformInterval,getPeriodicInformInterval,NULL,NULL},
  	{PeriodicInformTime ,{{tDateTime,0,0}}, setPeriodicInformTime,getPeriodicInformTime,NULL,NULL},
    {ParameterKey,{{tString,32,0}}, NULL,getParameterKey,NULL,NULL},
  	{ConnectionRequestURL,{{tString,256,0}}, NULL,getConnectionRequestURL,NULL,NULL},
    {ConnectionRequestUsername,{{tString,256,0}}, setConnectionRequestUsername,getConnectionRequestUsername,NULL,NULL},
    {ConnectionRequestPassword,{{tString,256,0}}, setConnectionRequestPassword,getConnectionRequestPassword,NULL,NULL},
	{EnableCWMP,{{tselect,0,0}}, setmgmtServerEnableCWMP,getmgmtServerEnableCWMP,NULL,NULL},
	{X_ATP_ConnReqPort,{{tUnsigned,0,0}}, setX_ATP_ConnReqPort,getX_ATP_ConnReqPort,NULL,NULL},
#if 0
  	{UpgradesManaged,{{tBool,0,0}}, setUpgradesManaged,getUpgradesManaged,NULL,NULL},
  	{UDPConnectionRequestAddress,{{tString,0,0}}, NULL,getUDPConnectionRequestAddress,NULL,NULL},
    {UDPConnectionRequestAddressNotificationLimit,{{tUnsigned,0,0}}, setUDPConnectionRequestAddressNotificationLimit,getUDPConnectionRequestAddressNotificationLimit,NULL,NULL},
    {STUNEnable,{{tBool,0,0}}, setSTUNEnable,getSTUNEnable,NULL,NULL},
    {STUNServerAddress,{{tString,256,0}}, setSTUNServerAddress,getSTUNServerAddress,NULL,NULL},
    {STUNServerPort,{{tUnsigned,16,0}}, setSTUNServerPort,getSTUNServerPort,NULL,NULL},
    {STUNUsername,{{tString,256,0}}, setSTUNUsername,getSTUNUsername,NULL,NULL},
    {STUNPassword,{{tString,256,0}}, setSTUNPassword,getSTUNPassword,NULL,NULL},
    {STUNMaximumKeepAlivePeriod,{{tInt,0,0}}, setSTUNMaximumKeepAlivePeriod,getSTUNMaximumKeepAlivePeriod,NULL,NULL},
    {STUNMinimumKeepAlivePeriod,{{tUnsigned,0,0}}, setSTUNMinimumKeepAlivePeriod,getSTUNMinimumKeepAlivePeriod,NULL,NULL},
    {NATDetected,{{tBool,0,0}}, NULL,getNATDetected,NULL,NULL},

#endif
    {NULL}    
};
/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

