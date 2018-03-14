/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : lteDeviceParams.c
  版 本 号   : 初稿
  作    者   : zhangjiasi
  生成日期   : 2015年6月4日
  最近修改   :
  功能描述   : lte相关信息。接口定义及树形结构拓扑定义
  函数列表   :
  修改历史   :
  1.日    期   : 2015年6月4日
    作    者   : zhangjiasi
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
 
#include "lteDeviceParams.h"
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

TRxObjNode lteDeviceX_LteDeviceDesc[] = {
	{RadioSignalStrength,{{tInt,0,0}},
	 NULL,
	 getX_LteDevice_RadioSignalStrength,
	 NULL,NULL},
	{Txpower,{{tInt,0,0}},
	 NULL,
	 getX_LteDevice_Txpower,
	 NULL,NULL},
	{IMEI,{{tString,32,0}},
	 NULL,
	 getX_LteDevice_IMEI,
	 NULL,NULL},
	{IMSI,{{tString,32,0}},
	 NULL,
	 getX_LteDevice_IMSI,
	 NULL,NULL},
	{RSRP,{{tInt,0,0}},
	 NULL,
	 getX_LteDevice_RSRP,
	 NULL,NULL},
	{RSRQ,{{tInt,0,0}},
	 NULL,
	 getX_LteDevice_RSRQ,
	 NULL,NULL},
/*	{SINR,{{tInt,0,0}},
	 NULL,
	 getX_LteDevice_SINR,
	 NULL,NULL},
	 */
	{Band,{{tString,32,0}},
	 NULL,
	 getX_LteDevice_Band,
	 NULL,NULL},
	{BandWidth,{{tString,32,0}},
	 NULL,
	 getX_LteDevice_BandWidth,
	 NULL,NULL},
	{SIMSTATUS,{{tString,16,0}},
	 NULL,
	 getX_LteDevice_SIMSTATUS,
	 NULL,NULL},
	{RadioMode,{{tString,16,0}},
	 NULL,
	 getX_LteDevice_RadioMode,
	 NULL,NULL},
	{CellLocking,{{tString,32,0}},
	 NULL,
	 getX_LteDevice_CellLocking,
	 NULL,NULL},
	{FrequencyLocking,{{tString,32,0}},
	 NULL,
	 getX_LteDevice_FrequencyLocking,
	 NULL,NULL},
	{RadioFrenquency,{{tString,32,0}},
	 NULL,
	 getX_LteDevice_RadioFrenquency,
	 NULL,NULL},
	{RSSI,{{tInt,0,0}},
	 NULL,
	 getX_LteDevice_RSSI,
	 NULL,NULL},
	{RegistrationStatus,{{tString,32,0}},
	 NULL,
	 getX_LteDevice_RegistrationStatus,
	 NULL,NULL}, 
	{NetRegStatus,{{tString,32,0}},
	 NULL,
	 getX_LteDevice_NetRegStatus,
	 NULL,NULL},
	{GprsRegStatus,{{tString,32,0}},
	 NULL,
	 getX_LteDevice_GprsRegStatus,
	 NULL,NULL},
	{EpsRegStatus,{{tString,32,0}},
	 NULL,
	 getX_LteDevice_EpsRegStatus,
	 NULL,NULL},
	{RadioInterference,{{tString,16,0}},
	 NULL,
	 getX_LteDevice_RadioInterference,
	 NULL,NULL},
	{NetworkName,{{tString,32,0}},
	 NULL,
	 getX_LteDevice_NetworkName,
	 NULL,NULL},
	{EnodeBId,{{tString,32,0}},
	 NULL,
	 getX_LteDevice_EnodeBId,
	 NULL,NULL},
	{GlobeCellId,{{tString,32,0}},
	 NULL,
	 getX_LteDevice_GlobeCellId,
	 NULL,NULL},
	{PhysicsCellId,{{tString,32,0}},
	 NULL,
	 getX_LteDevice_PhysicsCellId,
	 NULL,NULL},
	{Iccid,{{tString,32,0}},
	 NULL,
	 getX_LteDevice_Iccid,
	 NULL,NULL},
	{CellID,{{tString,32,0}},
	 NULL,
	 getX_LteDevice_CellID,
	 NULL,NULL},
	{APNStatus,{{tString,16,0}},
	 NULL,
	 getX_LteDevice_APNStatus,
	 NULL,NULL},
	{InNetWorkState,{{tString,16,0}},
	 NULL,
	 getX_LteDevice_InNetWorkState,
	 NULL,NULL},
	{NULL}
};
/*
TRxObjNode  LANDeviceInstanceDesc[] = {
	{instanceIDMASK,{{0}},
	  NULL,
	  NULL,
	  LANDeviceDesc},
};

	{LANHostConfigManagement,{{tObject,0}},
	  NULL,
	  NULL,
	  LANDeviceLANHostConfigManagementDesc},
*/
TRxObjNode x_lteDeviceDesc[] = {
	{Status,{{tObject,0}},
	 NULL,
	 NULL,
	 lteDeviceX_LteDeviceDesc},
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

