/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : gpioex_ioctl.h
  �� �� ��   : ����
  ��    ��   : Zhong Huaqing
  ��������   : 2012��8��10�� ������
  ����޸�   :
  ��������   : gpioex_ioctl.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��8��10�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __GPIOEX_IOCTL_H__
#define __GPIOEX_IOCTL_H__


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
#define GPIOEX_PROC_FILE        "gpioex"

#define GPIOEX_IOCTL_CMD_BEGIN 0


#define GPIOEX_MOD_VERSION  "v1.0.1"
#define GPIOEX_APP_VERSION  "v1.0.1"

#define GPIOEX_MOD_DEP_VERSION "v1.0.1"   /*֧���ں�ģ���APP��С�汾��*/
#define GPIOEX_APP_DEP_VERSION "v1.0.1"   /*֧��APP���ں�ģ����С�汾��*/


 /*----------------------------------------------*
 * �����                                       *
 *----------------------------------------------*/


 
/*----------------------------------------------*
 * ö�ٶ���                                       *
 *----------------------------------------------*/
typedef enum
{
    GPIOEX_IOCTL_CMD_GET_VERSION = GPIOEX_IOCTL_CMD_BEGIN,
    
    GPIOEX_IOCTL_CMD_SET_DEBUG_LEVEL,
    GPIOEX_IOCTL_CMD_GET_DEBUG_LEVEL,

    GPIOEX_IOCTL_CMD_ALL_MODE_SET,
    GPIOEX_IOCTL_CMD_ALL_MODE_GET,
    GPIOEX_IOCTL_CMD_MODE_SET,
    GPIOEX_IOCTL_CMD_MODE_GET,
    GPIOEX_IOCTL_CMD_ALL_STATE_SET,
    GPIOEX_IOCTL_CMD_ALL_STATE_GET,
    GPIOEX_IOCTL_CMD_STATE_SET,
    GPIOEX_IOCTL_CMD_STATE_GET,
    
    /*���Զ�ģ���е���������޸Ļ�����*/
    /*.......*/
}GPIOEX_IOCTL_CMD_E;

/*----------------------------------------------*
 * ��չ���Ͷ���                                     *
 *----------------------------------------------*/


/*----------------------------------------------*
 * �ṹ����                                       *
 *----------------------------------------------*/
typedef struct
{
    #define GPIOEX_VERSION_SIZE  128
    CHAR acModVersion[GPIOEX_VERSION_SIZE];    /*�ں�ģ��汾��*/
    CHAR acModDepVersion[GPIOEX_VERSION_SIZE]; /*֧���ں�ģ���APPģ����С�汾��*/
}GPIOEX_IOCTL_VERSION_T;


typedef struct
{
    UINT32 uiDebugLevel;
}GPIOEX_IOCTL_DEBUG_T;

typedef struct
{
    UINT32 uiValue;
}GPIOEX_IOCTL_MODE_ALL_T;


typedef struct
{
    UINT32 uiNum;
    UINT32 uiValue;
}GPIOEX_IOCTL_MODE_T;


typedef struct
{
    UINT32 uiValue;
}GPIOEX_IOCTL_STATE_ALL_T;


typedef struct
{
    UINT32 uiNum;
    UINT32 uiValue;
}GPIOEX_IOCTL_STATE_T;


/*���Զ�ģ���е���������޸Ļ�����*/
/*.......*/

typedef struct
{
	GPIOEX_IOCTL_CMD_E enCmd;

	union
	{
	    GPIOEX_IOCTL_VERSION_T stVersion;
	    GPIOEX_IOCTL_DEBUG_T stDebug;
	    GPIOEX_IOCTL_MODE_ALL_T stAllMode;
	    GPIOEX_IOCTL_MODE_T stMode;
	    GPIOEX_IOCTL_STATE_ALL_T stAllState;
	    GPIOEX_IOCTL_STATE_T stState;
	    
	    /*���Զ�ģ���е���������޸Ļ�����*/
        /*.......*/
	}unData;
}GPIOEX_IOCTL_T;




/*----------------------------------------------*
 * ��������ԭ��˵��                             *
 *----------------------------------------------*/



#endif /* __GPIOEX_IOCTL_H__ */

