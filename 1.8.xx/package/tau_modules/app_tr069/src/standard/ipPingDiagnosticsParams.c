/*
 * Copyright (C) GBCOM ltd.,co 2015
 *
 * This is a part of project tr69cd, which implements TR069 protocal(aka
 * CWMP).
 * 
 * This is a PRIVATE software.
 * All rights reserved.
 */

#include "ipPingDiagnosticsParams.h"

 TRxObjNode IPPingDiagnosticsDesc[] = {
	{DiagnosticsState,{{tString,16,0}},
	 setIPPingDiagnostics_DiagnosticsState,
	 getIPPingDiagnostics_DiagnosticsState,
	 NULL,NULL},
	{Interface,{{tString,256,0}},
	 setIPPingDiagnostics_Interface,
	 getIPPingDiagnostics_Interface,
	 NULL,NULL},
	 {Host,{{tString,256,0}},setIPPingDiagnostics_Host,getIPPingDiagnostics_Host,NULL,NULL},
	{NumberOfRepetitions,{{tUnsigned,0,0}},
	 setIPPingDiagnostics_NumberOfRepetitions,
	 getIPPingDiagnostics_NumberOfRepetitions,
	 NULL,NULL},
	{Timeout,{{tUnsigned,0,0}},
	 setIPPingDiagnostics_Timeout,
	 getIPPingDiagnostics_Timeout,
	 NULL,NULL},
	{DataBlockSize,{{tUnsigned,0,0}},
	 setIPPingDiagnostics_DataBlockSize,
	 getIPPingDiagnostics_DataBlockSize,
	 NULL,NULL},
	{DSCP,{{tUnsigned,0,0}},
	 setIPPingDiagnostics_DSCP,
	 getIPPingDiagnostics_DSCP,
	 NULL,NULL},
	{SuccessCount,{{tUnsigned,0,0}},
	 NULL,
	 getIPPingDiagnostics_SuccessCount,
	 NULL,NULL},
	{FailureCount,{{tUnsigned,0,0}},
	 NULL,
	 getIPPingDiagnostics_FailureCount,
	 NULL,NULL},
	{AverageResponseTime,{{tUnsigned,0,0}},
	 NULL,
	 getIPPingDiagnostics_AverageResponseTime,
	 NULL,NULL},
 	{MinimumResponseTime,{{tUnsigned,0,0}},
	 NULL,
	 getIPPingDiagnostics_MinimumResponseTime,
	 NULL,NULL},
	{MaximumResponseTime,{{tUnsigned,0,0}},
	 NULL,
	 getIPPingDiagnostics_MaximumResponseTime,
	 NULL,NULL},
	 //{PingLog,{{tString,4096,0}}, NULL,getIPPingDiagnostics_Pinglog, NULL,NULL},
	{NULL}
};

TRxObjNode x_PPPoE2L2TP_i_Desc[] = {
	{Device,{{tBool,0,0}},
	 setX_PPPoE2L2TP_Device,
	 getX_PPPoE2L2TP_Device,
	 NULL,NULL},
	 {TunnelMode,{{tBool,0,0}},
	 setX_PPPoE2L2TP_TunnelMode,
	 getX_PPPoE2L2TP_TunnelMode,
	 NULL,NULL},
	 {LNSServerIPAddress,{{tBool,0,0}},
	 setX_PPPoE2L2TP_LNSServerIPAddress,
	 getX_PPPoE2L2TP_LNSServerIPAddress,
	 NULL,NULL},
	 {LNSServerHostName,{{tBool,0,0}},
	 setX_PPPoE2L2TP_LNSServerHostName,
	 getX_PPPoE2L2TP_LNSServerHostName,
	 NULL,NULL},
	 {LNSServerPortName,{{tBool,0,0}},
	 setX_PPPoE2L2TP_LNSServerPortName,
	 getX_PPPoE2L2TP_LNSServerPortName,
	 NULL,NULL},
	 {TunnelAuthenticateEnable,{{tBool,0,0}},
	 setX_PPPoE2L2TP_TunnelAuthenticateEnable,
	 getX_PPPoE2L2TP_TunnelAuthenticateEnable,
	 NULL,NULL},
	 {PasswordOfTunnel,{{tBool,0,0}},
	 setX_PPPoE2L2TP_PasswordOfTunnel,
	 getX_PPPoE2L2TP_PasswordOfTunnel,
	 NULL,NULL},
	{NULL}
};

TRxObjNode  x_PPPoE2L2TP_i[] = {
    {instanceIDMASK,{{0}},
		addx_PPPoE2L2TP_i,
	   deletex_PPPoE2L2TP_i,
	   x_PPPoE2L2TP_i_Desc},   
};

TRxObjNode x_PPPoE2L2TPDesc[] = {
	  
	{Enable,{{tBool,0,0}},
	 setX_PPPoE2L2TP_Enable,
	 getX_PPPoE2L2TP_Enable,
	 NULL,NULL},
	{MTUOfUser,{{tInt,0,0}},
	 setX_PPPoE2L2TP_MTUOfUser,
	 getX_PPPoE2L2TP_MTUOfUser,
	 NULL,NULL},
	 {L2TPServerNumberOfEntries,{{tInt,0,0}},
	 NULL,
	 getX_PPPoE2L2TP_L2TPServerNumberOfEntries,
	 NULL,NULL},

 	{L2TPServer,{{tInstance,0}},
	  NULL,
	  NULL,
	  x_PPPoE2L2TP_i},
	
	{NULL}
};



