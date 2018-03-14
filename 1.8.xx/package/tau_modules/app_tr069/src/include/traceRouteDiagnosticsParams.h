/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : traceRouteDiagnosticsParams.h
  版 本 号   : 初稿
  作    者   : zhangjiasi
  生成日期   : 2015年6月4日
  最近修改   :
  功能描述   : traceRouteDiagnosticsParams.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2015年6月4日
    作    者   : zhangjiasi
    修改内容   : 创建文件

******************************************************************************/
#ifndef __TRACEROUTEDIAGNOSTICSPARAMS_H__
#define __TRACEROUTEDIAGNOSTICSPARAMS_H__


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

/* InternetGatewayDevice.TraceRouteDiagnostics. */
SVAR(DiagnosticsState);
SVAR(Interface);
SVAR(Host);
SVAR(NumberOfTries);
SVAR(Timeout);
SVAR(DataBlockSize);
SVAR(DSCP);
SVAR(MaxHopCount);
SVAR(ResponseTime);
SVAR(HopsNumberOfEntries);
SVAR(TraceLog);
SVAR(RouteHops);

SVAR(HopHost);
SVAR(HopHostAddress);
SVAR(HopErrorCode);
SVAR(HopRTTimes);
/* InternetGatewayDevice.TraceRouteDiagnostics.RouteHops. */

TRXSFUNC(deleteTraceRouteDiagnosticsRouteHops);
TRXGFUNC(addTraceRouteDiagnosticsRouteHops);

TRXSFUNC(setTraceRouteDiagnostics_DiagnosticsState);
TRXGFUNC(getTraceRouteDiagnostics_DiagnosticsState);
TRXSFUNC(setTraceRouteDiagnostics_Interface);
TRXGFUNC(getTraceRouteDiagnostics_Interface);
TRXSFUNC(setTraceRouteDiagnostics_Host);
TRXGFUNC(getTraceRouteDiagnostics_Host);
TRXSFUNC(setTraceRouteDiagnostics_NumberOfTries);
TRXGFUNC(getTraceRouteDiagnostics_NumberOfTries);
TRXSFUNC(setTraceRouteDiagnostics_Timeout);
TRXGFUNC(getTraceRouteDiagnostics_Timeout);
TRXSFUNC(setTraceRouteDiagnostics_DataBlockSize);
TRXGFUNC(getTraceRouteDiagnostics_DataBlockSize);
TRXSFUNC(setTraceRouteDiagnostics_DSCP);
TRXGFUNC(getTraceRouteDiagnostics_DSCP);
TRXSFUNC(setTraceRouteDiagnostics_MaxHopCount);
TRXGFUNC(getTraceRouteDiagnostics_MaxHopCount);
TRXGFUNC(getTraceRouteDiagnostics_ResponseTime);
TRXGFUNC(getTraceRouteDiagnostics_RouteHopsNumberOfEntries);
TRXGFUNC(getTraceRouteDiagnostics_TraceRoutelog);

TRXGFUNC(getTraceRouteDiagnostics_Hops_HopHost);
TRXGFUNC(getTraceRouteDiagnostics_Hops_HopHostAddress);
TRXGFUNC(getTraceRouteDiagnostics_Hops_HopErrorCode);
TRXGFUNC(getTraceRouteDiagnostics_Hops_HopRTTimes);

/****************************************************************************************************/
/****************************************** X_CMCC_PPPOE_EMULATOR related     *********************************/
/****************************************************************************************************/
/* InternetGatewayDevice.X_CMCC_PPPOE_EMULATOR.*/
SVAR(Username);
SVAR(Password);
SVAR(WANInterface);
SVAR(PPPAuthenticationProtocol);
SVAR(RetryTimes);
SVAR(Result);
SVAR(PPPSessionID);
SVAR(ExternalIPAddress);
SVAR(DefaultGateway);
SVAR(PPPOEEmulatorlog);

TRXSFUNC(setPPPOE_Emulator_RetryTimes);
TRXGFUNC(getPPPOE_Emulator_RetryTimes);
TRXSFUNC(setPPPOE_Emulator_PPPAuthenticationProtocol);
TRXGFUNC(getPPPOE_Emulator_PPPAuthenticationProtocol);
TRXSFUNC(setPPPOE_Emulator_WANInterface);
TRXGFUNC(getPPPOE_Emulator_WANInterface);
TRXSFUNC(setPPPOE_Emulator_Password);
TRXGFUNC(getPPPOE_Emulator_Password);
TRXSFUNC(setPPPOE_Emulator_Username);
TRXGFUNC(getPPPOE_Emulator_Username);
TRXSFUNC(setPPPOE_Emulator_DiagnosticsState);
TRXGFUNC(getPPPOE_Emulator_DiagnosticsState);
TRXGFUNC(getPPPOE_Emulator_Result);
TRXGFUNC(getPPPOE_Emulator_PPPSessionID);
TRXGFUNC(getPPPOE_Emulator_ExternalIPAddress);
TRXGFUNC(getPPPOE_Emulator_DefaultGateway);
TRXGFUNC(getPPPOEEmulator_log);

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

void initTraceRouteDiagnosticsInstances(void);
//altered by caibing 11-15

//void initX_PPPOE_EmulatorInstances(void);
void TraceRoute_start(void);
void om_PPPOE_Emulator_Start(void);
#endif /* __TRACEROUTEDIAGNOSTICSPARAMS_H__ */
