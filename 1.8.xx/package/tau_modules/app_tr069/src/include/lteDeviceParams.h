/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : lteDeviceParams.h
  版 本 号   : 初稿
  作    者   : zhangjiasi
  生成日期   : 2015年6月4日
  最近修改   :
  功能描述   : lteDeviceParams.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2015年6月4日
    作    者   : zhangjiasi
    修改内容   : 创建文件

******************************************************************************/
#ifndef __LTEDEVICEPARAMS_H__
#define __LTEDEVICEPARAMS_H__

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

/* InternetGatewayDevice.X_LteDevice. */
/* InternetGatewayDevice.X_LteDevice.Status. */
SVAR(Status);
SVAR(X_LteDevice);
SVAR(RadioSignalStrength);
SVAR(Txpower);
SVAR(IMEI);
SVAR(IMSI);
SVAR(RSRP);
SVAR(RSRQ);
//SVAR(SINR);
SVAR(Band);
SVAR(BandWidth);
SVAR(SIMSTATUS);
SVAR(RadioMode);
SVAR(CellLocking);
SVAR(FrequencyLocking);
SVAR(RadioFrenquency);
SVAR(RSSI);
SVAR(RegistrationStatus);
SVAR(NetRegStatus);
SVAR(GprsRegStatus);
SVAR(EpsRegStatus);
SVAR(RadioInterference);
SVAR(NetworkName);
SVAR(EnodeBId);
SVAR(GlobeCellId);
SVAR(PhysicsCellId);
SVAR(Iccid);
SVAR(CellID);
SVAR(APNStatus);
SVAR(InNetWorkState);

/* 函数原型定义 */
/* InternetGatewayDevice.X_LteDevice.*/
/* InternetGatewayDevice.X_LteDevice.Status. */
TRXGFUNC(getX_LteDevice_RadioSignalStrength);
TRXGFUNC(getX_LteDevice_Txpower);
TRXGFUNC(getX_LteDevice_IMEI);
TRXGFUNC(getX_LteDevice_IMSI);
TRXGFUNC(getX_LteDevice_RSRP);
TRXGFUNC(getX_LteDevice_RSRQ);
TRXGFUNC(getX_LteDevice_SINR);
TRXGFUNC(getX_LteDevice_Band);
TRXGFUNC(getX_LteDevice_BandWidth);
TRXGFUNC(getX_LteDevice_SIMSTATUS);
TRXGFUNC(getX_LteDevice_RadioMode);
TRXGFUNC(getX_LteDevice_CellLocking);
TRXGFUNC(getX_LteDevice_FrequencyLocking);
TRXGFUNC(getX_LteDevice_RadioFrenquency);
TRXGFUNC(getX_LteDevice_RSSI);
TRXGFUNC(getX_LteDevice_RegistrationStatus);
TRXGFUNC(getX_LteDevice_NetRegStatus);
TRXGFUNC(getX_LteDevice_GprsRegStatus);
TRXGFUNC(getX_LteDevice_EpsRegStatus);
TRXGFUNC(getX_LteDevice_RadioInterference);
TRXGFUNC(getX_LteDevice_NetworkName);
TRXGFUNC(getX_LteDevice_EnodeBId);
TRXGFUNC(getX_LteDevice_GlobeCellId);
TRXGFUNC(getX_LteDevice_PhysicsCellId);
TRXGFUNC(getX_LteDevice_Iccid);
TRXGFUNC(getX_LteDevice_CellID);
TRXGFUNC(getX_LteDevice_APNStatus);
TRXGFUNC(getX_LteDevice_InNetWorkState);



#endif /* __LTEDEVICEPARAMS_H__ */
