/******************************************************************************

                  ��Ȩ���� (C), 2010-2012, �Ϻ�徴�ͨ�ſƼ����޹�˾

 ******************************************************************************
  �� �� ��   : timeParams.h
  �� �� ��   : ����
  ��    ��   : zhangjiasi
  ��������   : 2015��6��4��
  ����޸�   :
  ��������   : timeParams.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��6��4��
    ��    ��   : zhangjiasi
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __TIMEPARAMS_H__
#define __TIMEPARAMS_H__
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

/* InternetGatewayDevice.Time. */
SVAR(Enable);
SVAR(Status);

SVAR(NTPServer1);
SVAR(NTPServer2);
SVAR(CurrentLocalTime);
SVAR(LocalTimeZone);
SVAR(LocalTimeZoneName);
SVAR(ConfigType);
SVAR(X_ATP_MannualTime);



/* ����ԭ�Ͷ��� */
/* InternetGatewayDevice.Time. */

TRXSFUNC(setTimeEnable);
TRXGFUNC(getTimeEnable);
TRXGFUNC(getTimeStatus);

TRXSFUNC(setNTPServer1);
TRXGFUNC(getNTPServer1);
TRXSFUNC(setNTPServer2);
TRXGFUNC(getNTPServer2);
TRXGFUNC(getCurrentLocalTime);
TRXSFUNC(setLocalTimeZone);
TRXGFUNC(getLocalTimeZone);
TRXSFUNC(setLocalTimeZoneName);
TRXGFUNC(getLocalTimeZoneName);
TRXSFUNC(setConfigType);
TRXGFUNC(getConfigType);
TRXSFUNC(setX_ATP_MannualTime);
TRXGFUNC(getX_ATP_MannualTime);




#endif /* __TIMEPARAMS_H__ */
