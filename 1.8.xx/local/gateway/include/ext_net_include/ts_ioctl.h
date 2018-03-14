/******************************************************************************

                  ��Ȩ���� (C), 2010-2012, �Ϻ�徴�ͨ�ſƼ��ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : ts_ioctl.h
  �� �� ��   : ����
  ��    ��   : xucongjiang
  ��������   : 2012��10��22��
  ����޸�   :
  ��������   : ts_ioctl.c��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��10��22��
    ��    ��   : xucongjiang
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __TS_IOCTL_H__
#define __TS_IOCTL_H__


/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "ap_types.h"

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
 * �������Ͷ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/
/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
#define TS_PROC_FILE        "ts"

#define TS_IOCTL_CMD_BEGIN 0


#define TS_MOD_VERSION  "v1.0.1"
#define TS_APP_VERSION  "v1.0.1"

#define TS_MOD_DEP_VERSION "v1.0.1"   /*֧���ں�ģ���APP��С�汾��*/
#define TS_APP_DEP_VERSION "v1.0.1"   /*֧��APP���ں�ģ����С�汾��*/

#ifndef ETH_ALEN
#define ETH_ALEN	6		/* Octets in one ethernet addr	 */
#endif

#define TS_CPE_USR_MAX 32


 /*----------------------------------------------*
 * �����                                       *
 *----------------------------------------------*/


 
/*----------------------------------------------*
 * ö�ٶ���                                       *
 *----------------------------------------------*/

 
/* �û����� */
typedef enum
{
    PPPOE_USER = 1,      /* PPPoE�û� */
    DHCP_USER       /* DHCP��ȡ��ַ���û�����PPPoE�û��� */
}TS_USER_TYPE_E;


/* ���������� */
typedef enum
{
    WIRED_USER = 1,      /* ���߶˿ڽ�����û� */
    WIRELESS_USER   /* ���߶˿ڽ�����û� */
}LINK_TYPE_E;

typedef enum
{
    TS_IOCTL_CMD_GET_VERSION = TS_IOCTL_CMD_BEGIN,
    
    TS_IOCTL_CMD_SET_DEBUG_LEVEL,
    TS_IOCTL_CMD_GET_DEBUG_LEVEL,
    
    TS_IOCTL_CMD_SET_USR,
    TS_IOCTL_CMD_GET_USR,
    TS_IOCTL_CMD_DEL_USR,
    
    TS_IOCTL_CMD_GET_USR_LIST,
    TS_IOCTL_CMD_FLUSH_ALL_USERS,

    TS_IOCTL_CMD_SET_AGING_TIME,
    TS_IOCTL_CMD_GET_AGING_TIME,
    
}TS_IOCTL_CMD_E;

/*----------------------------------------------*
 * ��չ���Ͷ���                                     *
 *----------------------------------------------*/


/*----------------------------------------------*
 * �ṹ����                                       *
 *----------------------------------------------*/
typedef struct
{
    #define TS_VERSION_SIZE  128
    CHAR acModVersion[TS_VERSION_SIZE];    /*�ں�ģ��汾��*/
    CHAR acModDepVersion[TS_VERSION_SIZE]; /*֧���ں�ģ���APPģ����С�汾��*/
}TS_IOCTL_VERSION_T;


typedef struct
{
    UINT32 uiDebugLevel;
}TS_IOCTL_DEBUG_T;


typedef struct 
{
    UINT8  aucUserMac[ETH_ALEN]; /*USER��MAC��ַ*/
    TS_USER_TYPE_E enUserType;          /*�û����ͣ�1. PPPoE�û�2. IP*/
    LINK_TYPE_E enLinkType;          /*�������ͣ�1.�����û�2.����*/
    UINT16 usUserState;         /*�û�״̬��������*/
    ULONG ulUserTxBytes;       /*�����ֽ���*/
    ULONG ulUserRxBytes;       /*�հ��ֽ���*/
    CHAR acPPPoeUsrName[64];   /* �û�����ΪPPPoE�û�ʱ��Ч */
}TS_CPE_USER_INFO_T;


typedef struct
{
    INT32 iUserNum;
    TS_CPE_USER_INFO_T astUserInfo[TS_CPE_USR_MAX];

}TS_CPE_USER_ALL_T;

typedef TS_CPE_USER_INFO_T TS_IOCTL_USER_INFO_T;

typedef struct
{
    ULONG ulTime;   /* Unit: second */
}TS_CPE_AGING_TIME_T;


typedef struct
{
	TS_IOCTL_CMD_E enCmd;

	union
	{
	    TS_IOCTL_VERSION_T stVersion;
	    TS_IOCTL_DEBUG_T stDebug;
	    TS_IOCTL_USER_INFO_T stUserInfo;
	    TS_CPE_USER_ALL_T stUserList;
	    TS_CPE_AGING_TIME_T stAgingTime;
	}unData;
}TS_IOCTL_T;


/*----------------------------------------------*
 * ��������ԭ��˵��                             *
 *----------------------------------------------*/



#endif /* __TS_IOCTL_H__ */

