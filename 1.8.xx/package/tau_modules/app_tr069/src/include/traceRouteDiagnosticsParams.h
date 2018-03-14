/******************************************************************************

                  ��Ȩ���� (C), 2010-2012, �Ϻ�徴�ͨ�ſƼ����޹�˾

 ******************************************************************************
  �� �� ��   : traceRouteDiagnosticsParams.h
  �� �� ��   : ����
  ��    ��   : zhangjiasi
  ��������   : 2015��6��4��
  ����޸�   :
  ��������   : traceRouteDiagnosticsParams.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��6��4��
    ��    ��   : zhangjiasi
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __TRACEROUTEDIAGNOSTICSPARAMS_H__
#define __TRACEROUTEDIAGNOSTICSPARAMS_H__


/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "tr69cdefs.h"
/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
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
