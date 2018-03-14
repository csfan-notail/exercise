/******************************************************************************

                  ��Ȩ���� (C), 2010-2012, �Ϻ�徴�ͨ�ſƼ����޹�˾

 ******************************************************************************
  �� �� ��   : manageServerParams.h
  �� �� ��   : ����
  ��    ��   : zhangjiasi
  ��������   : 2015��6��4��
  ����޸�   :
  ��������   : manageServerParams.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��6��4��
    ��    ��   : zhangjiasi
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __MANAGESERVERPARAMS_H__
#define __MANAGESERVERPARAMS_H__


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

/* InternetGatewayDevice.ManagementServer. */

SVAR(URL);
SVAR(Username);
SVAR(Password);
SVAR(PeriodicInformEnable );
SVAR(PeriodicInformInterval );
SVAR(PeriodicInformTime );
SVAR(ParameterKey );
SVAR(ConnectionRequestURL );
SVAR(ConnectionRequestUsername );
SVAR(ConnectionRequestPassword );
SVAR(UpgradesManaged);
SVAR(UDPConnectionRequestAddress);
SVAR(UDPConnectionRequestAddressNotificationLimit);
SVAR(STUNEnable);
SVAR(STUNServerAddress);
SVAR(STUNServerPort);
SVAR(STUNUsername);
SVAR(STUNPassword);
SVAR(STUNMaximumKeepAlivePeriod);
SVAR(STUNMinimumKeepAlivePeriod);
SVAR(NATDetected);

SVAR(EnableCWMP);
SVAR(X_ATP_ConnReqPort);


/* ����ԭ�Ͷ��� */
/* InternetGatewayDevice.ManagementServer. */
TRXGFUNC(getCWMPEnable);
TRXGFUNC(getURL);
TRXSFUNC(setURL);
TRXGFUNC(getUsername);
TRXSFUNC(setUsername);
TRXGFUNC(getPassword);
TRXSFUNC(setPassword);
TRXGFUNC(getPeriodicInformEnable);
TRXSFUNC(setPeriodicInformEnable);
TRXGFUNC(getPeriodicInformInterval);
TRXSFUNC(setPeriodicInformInterval);
TRXGFUNC(getPeriodicInformTime);
TRXSFUNC(setPeriodicInformTime);
TRXGFUNC(getParameterKey);
TRXGFUNC(getConnectionRequestURL);
TRXGFUNC(getConnectionRequestUsername);
TRXSFUNC(setConnectionRequestUsername);
TRXGFUNC(getConnectionRequestPassword);
TRXSFUNC(setConnectionRequestPassword);
TRXGFUNC(getUpgradesManaged);
TRXSFUNC(setUpgradesManaged);
TRXGFUNC(getUDPConnectionRequestAddress);
TRXGFUNC(getUDPConnectionRequestAddressNotificationLimit);
TRXSFUNC(setUDPConnectionRequestAddressNotificationLimit);
TRXGFUNC(getSTUNEnable);
TRXSFUNC(setSTUNEnable);
TRXGFUNC(getSTUNServerAddress);
TRXSFUNC(setSTUNServerAddress);
TRXGFUNC(getSTUNServerPort);
TRXSFUNC(setSTUNServerPort);
TRXGFUNC(getSTUNUsername);
TRXSFUNC(setSTUNUsername);
TRXGFUNC(getSTUNPassword);
TRXSFUNC(setSTUNPassword);
TRXGFUNC(getSTUNMaximumKeepAlivePeriod);
TRXSFUNC(setSTUNMaximumKeepAlivePeriod);
TRXGFUNC(getSTUNMinimumKeepAlivePeriod);
TRXSFUNC(setSTUNMinimumKeepAlivePeriod);
TRXGFUNC(getNATDetected);


TRXSFUNC(setmgmtServerEnableCWMP);
TRXGFUNC(getmgmtServerEnableCWMP);

TRXSFUNC(setX_ATP_ConnReqPort);
TRXGFUNC(getX_ATP_ConnReqPort);

void updateConnectionInfo(void);
int getPasswordAuthFlag();
void initBCMLibIF(void);
#endif /* __MANAGESERVERPARAMS_H__ */
