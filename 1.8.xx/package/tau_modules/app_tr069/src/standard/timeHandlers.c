/******************************************************************************

                  ��Ȩ���� (C), 2010-2012, �Ϻ�徴�ͨ�ſƼ����޹�˾

 ******************************************************************************
  �� �� ��   : timeHandlers.c
  �� �� ��   : ����
  ��    ��   : zhangjiasi
  ��������   : 2015��6��4��
  ����޸�   :
  ��������   : ����ʱ��ͬ����Ϣ�����ֱ�׼�ӿ�ʵ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��6��4��
    ��    ��   : zhangjiasi
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include "utils.h"
#include "ap_types.h"
#include "om_tr69c.h"
#include "om_log.h"
#include "timeParams.h"
#include "timeParams.h"
/*Add by zhanghao ��ȡ�����ļ����� 2016/6/15 begin*/
#include "uci_config.h"
#include "tr69_lua.h"
/*Add by zhanghao ��ȡ�����ļ����� end*/
#define TAUARRY_LEN 128

/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/
extern int g_log_level;

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


 TRX_STATUS getCurrentLocalTime(char **value)
{
	time_t  curtime = time(NULL);
	*value = strdup(getXSIdateTime(&curtime));
	return TRX_OK;
}

/* ����ʱ����utcʱ���ƫ��������ʽ�磺
   +hh:mm
   -hh:mm */
TRX_STATUS setLocalTimeZone(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_TIME_LOCAlTIMEZONE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getLocalTimeZone(char **ppcValue)
{
	char acLocalTimeZone[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_TIME_LOCAlTIMEZONE,acLocalTimeZone))
	{
		*ppcValue = strdup("+00:00");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acLocalTimeZone);
		
	}	
	//*value = strdup("+08:00");
	return TRX_OK;
}
/* �����������ƣ��磺EST+5EDT,M4.1.0/2,M10.5.0/2 */
TRX_STATUS setLocalTimeZoneName(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_TIME_LOCAlTIMEZONENAME,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
	
	return TRX_OK;
}
TRX_STATUS getLocalTimeZoneName(char **ppcValue)
{
	char acLocalTimeZoneName[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_TIME_LOCAlTIMEZONENAME,acLocalTimeZoneName))
	{
		*ppcValue = strdup("EST+00");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acLocalTimeZoneName);
		
	}	
   // *value = strdup("EST+8EDT");
	return TRX_OK;
}

