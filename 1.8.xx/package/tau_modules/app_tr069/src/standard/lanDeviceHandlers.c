/******************************************************************************

                  ��Ȩ���� (C), 2010-2012, �Ϻ�徴�ͨ�ſƼ����޹�˾

 ******************************************************************************
  �� �� ��   : lanDeviceHandlers.c
  �� �� ��   : ����
  ��    ��   : zhangjiasi
  ��������   : 2015��6��4��
  ����޸�   :
  ��������   : lan��Ϣ�����ֱ�׼�ӿ�ʵ��
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
#include "utils.h"
#include "lanDeviceParams.h"

/*Add by zhanghao ��ȡ�����ļ����� 2016/6/15 begin*/
#include "uci_config.h"
#include "tr69_lua.h"
/*Add by zhanghao ��ȡ�����ļ����� end*/
#define TAUARRY_LEN 128

extern int g_log_level;

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

/* LAN��������̫���ӿ�����ʵ����Ŀ */
TRX_STATUS getLANDevice_LANEthernetInterfaceNumberOfEntries(char **ppcValue)
{
	char  acInterfaceNumberOfEntries[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_LANETHERNETINTERFACENUMBEROFENTRIES,acInterfaceNumberOfEntries))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acInterfaceNumberOfEntries);
		
	}	
	//*ppcValue = strdup("1");
	return TRX_OK;

}


/*add by wangminglin*/


TRX_STATUS setX_ATP_AutoConfig(const char *pcValue)
{
	if(UCI_OK !=uci_config_set(UCI_LANDEVICE_X_ATP_AUTOCONFIG,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return	TRX_ERR;
	}
    return TRX_OK;
}
TRX_STATUS getX_ATP_AutoConfig(char **ppcValue)
{
	char  acX_ATP_AutoConfig[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_LANDEVICE_X_ATP_AUTOCONFIG,acX_ATP_AutoConfig))
	{
		*ppcValue = strdup("0");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
	}
	else{
		*ppcValue = strdup(acX_ATP_AutoConfig);
		
	}	
    return TRX_OK;
}



