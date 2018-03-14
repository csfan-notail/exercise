/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : timeHandlers.c
  版 本 号   : 初稿
  作    者   : zhangjiasi
  生成日期   : 2015年6月4日
  最近修改   :
  功能描述   : 网络时间同步信息。部分标准接口实现
  函数列表   :
  修改历史   :
  1.日    期   : 2015年6月4日
    作    者   : zhangjiasi
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include "utils.h"
#include "ap_types.h"
#include "om_tr69c.h"
#include "om_log.h"
#include "timeParams.h"
#include "timeParams.h"
/*Add by zhanghao 获取配置文件内容 2016/6/15 begin*/
#include "uci_config.h"
#include "tr69_lua.h"
/*Add by zhanghao 获取配置文件内容 end*/
#define TAUARRY_LEN 128

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
extern int g_log_level;

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


 TRX_STATUS getCurrentLocalTime(char **value)
{
	time_t  curtime = time(NULL);
	*value = strdup(getXSIdateTime(&curtime));
	return TRX_OK;
}

/* 本地时间与utc时间的偏移量，格式如：
   +hh:mm
   -hh:mm */
TRX_STATUS setLocalTimeZone(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_TIME_LOCAlTIMEZONE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getLocalTimeZone(char **ppcValue)
{
	char acLocalTimeZone[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_TIME_LOCAlTIMEZONE,acLocalTimeZone))
	{
		*ppcValue = strdup("+00:00");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acLocalTimeZone);
		
	}	
	//*value = strdup("+08:00");
	return TRX_OK;
}
/* 本地市区名称，如：EST+5EDT,M4.1.0/2,M10.5.0/2 */
TRX_STATUS setLocalTimeZoneName(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_TIME_LOCAlTIMEZONENAME,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getLocalTimeZoneName(char **ppcValue)
{
	char acLocalTimeZoneName[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_TIME_LOCAlTIMEZONENAME,acLocalTimeZoneName))
	{
		*ppcValue = strdup("EST+00");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acLocalTimeZoneName);
		
	}	
   // *value = strdup("EST+8EDT");
	return TRX_OK;
}

