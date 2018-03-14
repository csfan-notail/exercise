/******************************************************************************

                  ��Ȩ���� (C), 2010-2012, �Ϻ�徴�ͨ�ſƼ����޹�˾

 ******************************************************************************
  �� �� ��   : lteDeviceParams.c
  �� �� ��   : ����
  ��    ��   : zhangjiasi
  ��������   : 2015��6��4��
  ����޸�   :
  ��������   : lte�����Ϣ���ӿڶ��弰���νṹ���˶���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��6��4��
    ��    ��   : zhangjiasi
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
 
#include "lteDeviceParams.h"
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
 * ģ�鼶����                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/

