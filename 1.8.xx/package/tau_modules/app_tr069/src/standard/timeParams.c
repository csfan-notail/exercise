/******************************************************************************

                  ��Ȩ���� (C), 2010-2012, �Ϻ�徴�ͨ�ſƼ����޹�˾

 ******************************************************************************
  �� �� ��   : timeParams.c
  �� �� ��   : ����
  ��    ��   : zhangjiasi
  ��������   : 2015��6��4��
  ����޸�   :
  ��������   : ����ʱ��ͬ�������Ϣ���ӿڶ��弰���νṹ���˶���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��6��4��
    ��    ��   : zhangjiasi
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "timeParams.h"

#define TIMESTAUS_LEN 0
#define CONFIGTYPE_LEN 0
#define MANNUALTIME_LEN 64
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
TRxObjNode  TimeDesc[] = {
	{Enable,{{tBool,0,0}}, setTimeEnable,getTimeEnable,NULL,NULL},
	{Status,{{tString,TIMESTAUS_LEN,0}},NULL,getTimeStatus,NULL,NULL},
    {NTPServer1,{{tString,64,0}}, setNTPServer1,getNTPServer1,NULL,NULL},
    {NTPServer2,{{tString,64,0}}, setNTPServer2,getNTPServer2,NULL,NULL},
	{CurrentLocalTime,{{tString,64,0}}, NULL,getCurrentLocalTime,NULL,NULL},
	{LocalTimeZoneName,{{tString,64,0}}, setLocalTimeZoneName,getLocalTimeZoneName,NULL,NULL},
#if 0
	{LocalTimeZone,{{tString,6,0}}, setLocalTimeZone,getLocalTimeZone,NULL,NULL},
   	
	{ConfigType,{{tString,CONFIGTYPE_LEN,0}},setConfigType,getConfigType,NULL,NULL},
    {X_ATP_MannualTime,{{tString,MANNUALTIME_LEN,0}},setX_ATP_MannualTime,getX_ATP_MannualTime,NULL,NULL},
#endif  
    {NULL}        
};



