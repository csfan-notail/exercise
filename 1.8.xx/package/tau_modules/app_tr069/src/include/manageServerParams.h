/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : manageServerParams.h
  版 本 号   : 初稿
  作    者   : zhangjiasi
  生成日期   : 2015年6月4日
  最近修改   :
  功能描述   : manageServerParams.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2015年6月4日
    作    者   : zhangjiasi
    修改内容   : 创建文件

******************************************************************************/
#ifndef __MANAGESERVERPARAMS_H__
#define __MANAGESERVERPARAMS_H__


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

#ifdef CPEVARNAMEINSTANCE
#undef CPEVARNAMEINSTANCE
#endif

/* InternetGatewayDevice.ManagementServer. */

SVAR(URL);
SVAR(Username);
SVAR(Password);
SVAR(PeriodicInformEnable );
SVAR(PeriodicInformInterval );
SVAR(PeriodicInformTime );
SVAR(ParameterKey );
SVAR(ConnectionRequestURL );
SVAR(ConnectionRequestUsername );
SVAR(ConnectionRequestPassword );
SVAR(UpgradesManaged);
SVAR(UDPConnectionRequestAddress);
SVAR(UDPConnectionRequestAddressNotificationLimit);
SVAR(STUNEnable);
SVAR(STUNServerAddress);
SVAR(STUNServerPort);
SVAR(STUNUsername);
SVAR(STUNPassword);
SVAR(STUNMaximumKeepAlivePeriod);
SVAR(STUNMinimumKeepAlivePeriod);
SVAR(NATDetected);

SVAR(EnableCWMP);
SVAR(X_ATP_ConnReqPort);


/* 函数原型定义 */
/* InternetGatewayDevice.ManagementServer. */
TRXGFUNC(getCWMPEnable);
TRXGFUNC(getURL);
TRXSFUNC(setURL);
TRXGFUNC(getUsername);
TRXSFUNC(setUsername);
TRXGFUNC(getPassword);
TRXSFUNC(setPassword);
TRXGFUNC(getPeriodicInformEnable);
TRXSFUNC(setPeriodicInformEnable);
TRXGFUNC(getPeriodicInformInterval);
TRXSFUNC(setPeriodicInformInterval);
TRXGFUNC(getPeriodicInformTime);
TRXSFUNC(setPeriodicInformTime);
TRXGFUNC(getParameterKey);
TRXGFUNC(getConnectionRequestURL);
TRXGFUNC(getConnectionRequestUsername);
TRXSFUNC(setConnectionRequestUsername);
TRXGFUNC(getConnectionRequestPassword);
TRXSFUNC(setConnectionRequestPassword);
TRXGFUNC(getUpgradesManaged);
TRXSFUNC(setUpgradesManaged);
TRXGFUNC(getUDPConnectionRequestAddress);
TRXGFUNC(getUDPConnectionRequestAddressNotificationLimit);
TRXSFUNC(setUDPConnectionRequestAddressNotificationLimit);
TRXGFUNC(getSTUNEnable);
TRXSFUNC(setSTUNEnable);
TRXGFUNC(getSTUNServerAddress);
TRXSFUNC(setSTUNServerAddress);
TRXGFUNC(getSTUNServerPort);
TRXSFUNC(setSTUNServerPort);
TRXGFUNC(getSTUNUsername);
TRXSFUNC(setSTUNUsername);
TRXGFUNC(getSTUNPassword);
TRXSFUNC(setSTUNPassword);
TRXGFUNC(getSTUNMaximumKeepAlivePeriod);
TRXSFUNC(setSTUNMaximumKeepAlivePeriod);
TRXGFUNC(getSTUNMinimumKeepAlivePeriod);
TRXSFUNC(setSTUNMinimumKeepAlivePeriod);
TRXGFUNC(getNATDetected);


TRXSFUNC(setmgmtServerEnableCWMP);
TRXGFUNC(getmgmtServerEnableCWMP);

TRXSFUNC(setX_ATP_ConnReqPort);
TRXGFUNC(getX_ATP_ConnReqPort);

void updateConnectionInfo(void);
int getPasswordAuthFlag();
void initBCMLibIF(void);
#endif /* __MANAGESERVERPARAMS_H__ */
