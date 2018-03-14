/******************************************************************************

                  ��Ȩ���� (C), 2010-2012, �Ϻ�徴�ͨ�ſƼ����޹�˾

 ******************************************************************************
  �� �� ��   : manageServerParams.c
  �� �� ��   : ����
  ��    ��   : zhangjiasi
  ��������   : 2015��6��4��
  ����޸�   :
  ��������   : ��������������Ϣ���ӿڶ��弰���νṹ���˶���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��6��4��
    ��    ��   : zhangjiasi
    �޸�����   : �����ļ�

******************************************************************************/
/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#define CPEVARNAMEINSTANCE

#include "manageServerParams.h"
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
TRxObjNode  mgmtServerDesc[] = {

	{URL,{{tString,256,0}}, setURL,getURL,NULL,NULL},
    {Username,{{tString,256,0}}, setUsername,getUsername,NULL,NULL},
    {Password,{{tString,256,0}}, setPassword,getPassword,NULL,NULL},
    {PeriodicInformEnable,{{tBool,0,0}}, setPeriodicInformEnable,getPeriodicInformEnable,NULL,NULL},
  	{PeriodicInformInterval,{{tUnsigned,0,0}},setPeriodicInformInterval,getPeriodicInformInterval,NULL,NULL},
  	{PeriodicInformTime ,{{tDateTime,0,0}}, setPeriodicInformTime,getPeriodicInformTime,NULL,NULL},
    {ParameterKey,{{tString,32,0}}, NULL,getParameterKey,NULL,NULL},
  	{ConnectionRequestURL,{{tString,256,0}}, NULL,getConnectionRequestURL,NULL,NULL},
    {ConnectionRequestUsername,{{tString,256,0}}, setConnectionRequestUsername,getConnectionRequestUsername,NULL,NULL},
    {ConnectionRequestPassword,{{tString,256,0}}, setConnectionRequestPassword,getConnectionRequestPassword,NULL,NULL},
	{EnableCWMP,{{tselect,0,0}}, setmgmtServerEnableCWMP,getmgmtServerEnableCWMP,NULL,NULL},
	{X_ATP_ConnReqPort,{{tUnsigned,0,0}}, setX_ATP_ConnReqPort,getX_ATP_ConnReqPort,NULL,NULL},
#if 0
  	{UpgradesManaged,{{tBool,0,0}}, setUpgradesManaged,getUpgradesManaged,NULL,NULL},
  	{UDPConnectionRequestAddress,{{tString,0,0}}, NULL,getUDPConnectionRequestAddress,NULL,NULL},
    {UDPConnectionRequestAddressNotificationLimit,{{tUnsigned,0,0}}, setUDPConnectionRequestAddressNotificationLimit,getUDPConnectionRequestAddressNotificationLimit,NULL,NULL},
    {STUNEnable,{{tBool,0,0}}, setSTUNEnable,getSTUNEnable,NULL,NULL},
    {STUNServerAddress,{{tString,256,0}}, setSTUNServerAddress,getSTUNServerAddress,NULL,NULL},
    {STUNServerPort,{{tUnsigned,16,0}}, setSTUNServerPort,getSTUNServerPort,NULL,NULL},
    {STUNUsername,{{tString,256,0}}, setSTUNUsername,getSTUNUsername,NULL,NULL},
    {STUNPassword,{{tString,256,0}}, setSTUNPassword,getSTUNPassword,NULL,NULL},
    {STUNMaximumKeepAlivePeriod,{{tInt,0,0}}, setSTUNMaximumKeepAlivePeriod,getSTUNMaximumKeepAlivePeriod,NULL,NULL},
    {STUNMinimumKeepAlivePeriod,{{tUnsigned,0,0}}, setSTUNMinimumKeepAlivePeriod,getSTUNMinimumKeepAlivePeriod,NULL,NULL},
    {NATDetected,{{tBool,0,0}}, NULL,getNATDetected,NULL,NULL},

#endif
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

