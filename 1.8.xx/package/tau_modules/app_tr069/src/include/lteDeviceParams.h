/******************************************************************************

                  ��Ȩ���� (C), 2010-2012, �Ϻ�徴�ͨ�ſƼ����޹�˾

 ******************************************************************************
  �� �� ��   : lteDeviceParams.h
  �� �� ��   : ����
  ��    ��   : zhangjiasi
  ��������   : 2015��6��4��
  ����޸�   :
  ��������   : lteDeviceParams.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��6��4��
    ��    ��   : zhangjiasi
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __LTEDEVICEPARAMS_H__
#define __LTEDEVICEPARAMS_H__

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

/* ����ԭ�Ͷ��� */
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
