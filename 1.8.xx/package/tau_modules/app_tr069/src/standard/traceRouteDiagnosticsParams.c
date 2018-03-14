/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : traceRouteDiagnosticsParams.c
  版 本 号   : 初稿
  作    者   : zhangjiasi
  生成日期   : 2015年6月4日
  最近修改   :
  功能描述   : trace route诊断相关信息。接口定义及树形结构拓扑定义
  函数列表   :
  修改历史   :
  1.日    期   : 2015年6月4日
    作    者   : zhangjiasi
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "traceRouteDiagnosticsParams.h"
#include "rootParams.h"
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
TRxObjNode TraceRouteDiagnosticsRouteHopsInstance[] = {
	{HopHost,{{tString,256,0}},
	 NULL,
	 getTraceRouteDiagnostics_Hops_HopHost,
	 NULL,NULL},
	 {HopHostAddress,{{tString,0,0}},
	 NULL,
	 getTraceRouteDiagnostics_Hops_HopHostAddress,
	 NULL,NULL},
	 {HopErrorCode,{{tUnsigned,0,0}},
	 NULL,
	 getTraceRouteDiagnostics_Hops_HopErrorCode,
	 NULL,NULL},
	 {HopRTTimes,{{tString,16,0}},
	 NULL,
	 getTraceRouteDiagnostics_Hops_HopRTTimes,
	 NULL,NULL},
   {NULL}
};
TRxObjNode TraceRouteDiagnosticsRouteHopsDesc[] = {
	  {instanceIDMASK,{{0}},
	   	deleteTraceRouteDiagnosticsRouteHops,
	  	addTraceRouteDiagnosticsRouteHops,
	   TraceRouteDiagnosticsRouteHopsInstance},
	       {NULL}
};


TRxObjNode TraceRouteDiagnosticsDesc[] = {
	{DiagnosticsState,{{tString,16,0}},
	 setTraceRouteDiagnostics_DiagnosticsState,
	 getTraceRouteDiagnostics_DiagnosticsState,
	 NULL,NULL},
	{Interface,{{tString,256,0}},
	 setTraceRouteDiagnostics_Interface,
	 getTraceRouteDiagnostics_Interface,
	 NULL,NULL},
	{Host,{{tString,256,0}},
	 setTraceRouteDiagnostics_Host,
	 getTraceRouteDiagnostics_Host,
	 NULL,NULL},
	{NumberOfTries,{{tUnsigned,0,0}},
	 setTraceRouteDiagnostics_NumberOfTries,
	 getTraceRouteDiagnostics_NumberOfTries,
	 NULL,NULL},
	{Timeout,{{tUnsigned,0,0}},
	 setTraceRouteDiagnostics_Timeout,
	 getTraceRouteDiagnostics_Timeout,
	 NULL,NULL},
	{DataBlockSize,{{tUnsigned,0,0}},
	 setTraceRouteDiagnostics_DataBlockSize,
	 getTraceRouteDiagnostics_DataBlockSize,
	 NULL,NULL},
	{DSCP,{{tUnsigned,0,0}},
	 setTraceRouteDiagnostics_DSCP,
	 getTraceRouteDiagnostics_DSCP,
	 NULL,NULL},
	{MaxHopCount,{{tUnsigned,0,0}},
	 setTraceRouteDiagnostics_MaxHopCount,
	 getTraceRouteDiagnostics_MaxHopCount,
	 NULL,NULL},
	 {TraceLog,{{tString,4096,0}},
	 NULL,
	 getTraceRouteDiagnostics_TraceRoutelog,
	 NULL,NULL},
	{ResponseTime,{{tUnsigned,0,0}},
	 NULL,
	 getTraceRouteDiagnostics_ResponseTime,
	 NULL,NULL},
	{HopsNumberOfEntries,{{tUnsigned,0,0}},
	 NULL,
	 getTraceRouteDiagnostics_RouteHopsNumberOfEntries,
	 NULL,NULL},
 	{RouteHops,{{tInstance,0}},
	 NULL,NULL,
	 TraceRouteDiagnosticsRouteHopsDesc},
	{NULL}
};



//altered by caibing 11-15

#if 0
TRxObjNode x_CMCC_PPPOE_EmulatorDesc[] = {
	{DiagnosticsState,{{tString,16,0}},
	 setPPPOE_Emulator_DiagnosticsState,
	 getPPPOE_Emulator_DiagnosticsState,
	 NULL,NULL},
	 
	{Username,{{tString,64,0}},
	 setPPPOE_Emulator_Username,
	 getPPPOE_Emulator_Username,
	 NULL,NULL},
	{Password,{{tString,64,0}},
	 setPPPOE_Emulator_Password,
	 getPPPOE_Emulator_Password,
	 NULL,NULL},
	{WANInterface,{{tString,256,0}},
	 setPPPOE_Emulator_WANInterface,
	 getPPPOE_Emulator_WANInterface,
	 NULL,NULL},

	{PPPAuthenticationProtocol,{{tString,16,0}},
	 setPPPOE_Emulator_PPPAuthenticationProtocol,
	 getPPPOE_Emulator_PPPAuthenticationProtocol,
	 NULL,NULL},
	{RetryTimes,{{tUnsigned,0,0}},
	 setPPPOE_Emulator_RetryTimes,
	 getPPPOE_Emulator_RetryTimes,
	 NULL,NULL},

	{Result,{{tString,16,0}},
	 NULL,
	 getPPPOE_Emulator_Result,
	 NULL,NULL},

	{PPPSessionID,{{tUnsigned,0,0}},
	 NULL,
	 getPPPOE_Emulator_PPPSessionID,
	 NULL,NULL},

	{ExternalIPAddress,{{tString,16,0}},
	 NULL,
	 getPPPOE_Emulator_ExternalIPAddress,
	 NULL,NULL},

	{DefaultGateway,{{tString,16,0}},
	 NULL,
	 getPPPOE_Emulator_DefaultGateway,
	 NULL,NULL},
	 
	 {PPPOEEmulatorlog,{{tString,4096,0}},
	 NULL,
	 getPPPOEEmulator_log,
	 NULL,NULL},
	{NULL}
};
#endif
/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

