/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : ipPingDiagnosticsParams.h
  版 本 号   : 初稿
  作    者   : zhangjiasi
  生成日期   : 2015年6月4日
  最近修改   :
  功能描述   : ipPingDiagnosticsParams.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2015年6月4日
    作    者   : zhangjiasi
    修改内容   : 创建文件

******************************************************************************/

#ifndef __IPPINGDIAGNOSTICSPARAMS_H__
#define __IPPINGDIAGNOSTICSPARAMS_H__

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

/* InternetGatewayDevice.IPPingDiagnostics. */
SVAR(DiagnosticsState);
SVAR(Interface);
SVAR(Host);
SVAR(NumberOfRepetitions);
SVAR(Timeout);
SVAR(DataBlockSize);
SVAR(DSCP);
SVAR(SuccessCount);
SVAR(FailureCount);
SVAR(AverageResponseTime);
SVAR(MinimumResponseTime);
SVAR(MaximumResponseTime);
SVAR(PingLog);
/* InternetGatewayDevice.X_PPPoE2L2TP.*/
SVAR(Enable);
SVAR(MTUOfUser);
SVAR(L2TPServerNumberOfEntries);
SVAR(L2TPServer);

/* InternetGatewayDevice.X_PPPoE2L2TP_i.*/
SVAR(Device);
SVAR(TunnelMode);
SVAR(LNSServerIPAddress);
SVAR(LNSServerHostName);
SVAR(LNSServerPortName);
SVAR(TunnelAuthenticateEnable);
SVAR(PasswordOfTunnel);


/* 函数原型定义 */
/* InternetGatewayDevice.IPPingDiagnostics. */ 
TRXSFUNC(setIPPingDiagnostics_DiagnosticsState);
TRXGFUNC(getIPPingDiagnostics_DiagnosticsState);
TRXSFUNC(setIPPingDiagnostics_Interface);
TRXGFUNC(getIPPingDiagnostics_Interface);
TRXSFUNC(setIPPingDiagnostics_Host);
TRXGFUNC(getIPPingDiagnostics_Host);
TRXSFUNC(setIPPingDiagnostics_NumberOfRepetitions);
TRXGFUNC(getIPPingDiagnostics_NumberOfRepetitions);
TRXSFUNC(setIPPingDiagnostics_Timeout);
TRXGFUNC(getIPPingDiagnostics_Timeout);
TRXSFUNC(setIPPingDiagnostics_DataBlockSize);
TRXGFUNC(getIPPingDiagnostics_DataBlockSize);
TRXSFUNC(setIPPingDiagnostics_DSCP);
TRXGFUNC(getIPPingDiagnostics_DSCP);
TRXGFUNC(getIPPingDiagnostics_SuccessCount);
TRXGFUNC(getIPPingDiagnostics_FailureCount);
TRXGFUNC(getIPPingDiagnostics_AverageResponseTime);
TRXGFUNC(getIPPingDiagnostics_MinimumResponseTime);
TRXGFUNC(getIPPingDiagnostics_MaximumResponseTime);
TRXGFUNC(getIPPingDiagnostics_Pinglog);

/* InternetGatewayDevice.X_PPPoE2L2TP.*/
TRXSFUNC(setX_PPPoE2L2TP_Enable);
TRXGFUNC(getX_PPPoE2L2TP_Enable);
TRXSFUNC(setX_PPPoE2L2TP_MTUOfUser);
TRXGFUNC(getX_PPPoE2L2TP_MTUOfUser);
TRXGFUNC(getX_PPPoE2L2TP_L2TPServerNumberOfEntries);

/* InternetGatewayDevice.X_PPPoE2L2TP_i.*/
TRXSFUNC(setX_PPPoE2L2TP_Device);
TRXGFUNC(getX_PPPoE2L2TP_Device);
TRXSFUNC(setX_PPPoE2L2TP_TunnelMode);
TRXGFUNC(getX_PPPoE2L2TP_TunnelMode);
TRXSFUNC(setX_PPPoE2L2TP_LNSServerIPAddress);
TRXGFUNC(getX_PPPoE2L2TP_LNSServerIPAddress);
TRXSFUNC(setX_PPPoE2L2TP_LNSServerHostName);
TRXGFUNC(getX_PPPoE2L2TP_LNSServerHostName);
TRXSFUNC(setX_PPPoE2L2TP_LNSServerPortName);
TRXGFUNC(getX_PPPoE2L2TP_LNSServerPortName);
TRXSFUNC(setX_PPPoE2L2TP_TunnelAuthenticateEnable);
TRXGFUNC(getX_PPPoE2L2TP_TunnelAuthenticateEnable);
TRXSFUNC(setX_PPPoE2L2TP_PasswordOfTunnel);
TRXGFUNC(getX_PPPoE2L2TP_PasswordOfTunnel);

//TRXSFUNC(setX_PPPoE2L2TP_Device);
//TRXGFUNC(getX_PPPoE2L2TP_Device);
TRXGFUNC(deletex_PPPoE2L2TP_i);
TRXSFUNC(addx_PPPoE2L2TP_i);

void initIPPingDiagnosticsInstances(void);
void initX_PPPoE2L2TPInstances(void);
void IPping_start(void);
#endif /* __IPPINGDIAGNOSTICSPARAMS_H__ */
