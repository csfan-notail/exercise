/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : lanDeviceHandlers.c
  版 本 号   : 初稿
  作    者   : zhangjiasi
  生成日期   : 2015年6月4日
  最近修改   :
  功能描述   : lan信息。部分标准接口实现
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
#include "utils.h"
#include "lanDeviceParams.h"

/*Add by zhanghao 获取配置文件内容 2016/6/15 begin*/
#include "uci_config.h"
#include "tr69_lua.h"
/*Add by zhanghao 获取配置文件内容 end*/
#define TAUARRY_LEN 128

extern int g_log_level;

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

/* LAN侧有线以太网接口配置实例数目 */
TRX_STATUS getLANDevice_LANEthernetInterfaceNumberOfEntries(char **ppcValue)
{
	char  acInterfaceNumberOfEntries[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_LANETHERNETINTERFACENUMBEROFENTRIES,acInterfaceNumberOfEntries))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acInterfaceNumberOfEntries);
		
	}	
	//*ppcValue = strdup("1");
	return TRX_OK;

}


/*add by wangminglin*/


TRX_STATUS setX_ATP_AutoConfig(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_X_ATP_AUTOCONFIG,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    return TRX_OK;
}
TRX_STATUS getX_ATP_AutoConfig(char **ppcValue)
{
	char  acX_ATP_AutoConfig[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_X_ATP_AUTOCONFIG,acX_ATP_AutoConfig))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_AutoConfig);
		
	}	
    return TRX_OK;
}



