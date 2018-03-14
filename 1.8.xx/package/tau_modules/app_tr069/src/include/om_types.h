/******************************************************************************

                  ��Ȩ���� (C), 2010-2012, �Ϻ�徴�ͨ�ſƼ����޹�˾

 ******************************************************************************
  �� �� ��   : om_type.h
  �� �� ��   : ����
  ��    ��   : zhanghaozhong
  ��������   : 2013��3��8��
  ����޸�   :
  ��������   : ����IAD�����������ͺͺ�������ֵ
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��3��8��
    ��    ��   : zhanghaozhong
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/

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



#ifndef __OM_TYPES_H__
#define __OM_TYPES_H__

#ifndef MAX_CMD_LEN
#define MAX_CMD_LEN         128
#endif

#ifndef LONG
#define LONG signed long
#endif

#ifndef INT32
#define INT32 signed int
#endif

#ifndef INT16
#define INT16 signed short
#endif

#ifndef INT8
#define INT8  signed char 
#endif

#ifndef UINT32
#define UINT32 unsigned int
#endif

#ifndef ULONG
#define ULONG unsigned long
#endif

#ifndef UINT16
#define UINT16 unsigned short
#endif

#ifndef UINT8
#define UINT8  unsigned char
#endif

#ifndef BOOL
#define BOOL UINT32
#endif

#ifndef CHAR
#define CHAR  INT8
#endif

#ifndef size_t
#define size_t UINT32
#endif

#ifndef VOID
#define VOID void
#endif

#ifndef LOCAL
#define LOCAL  static
#endif

#ifndef REMOTE
#define REMOTE extern
#endif


/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif
 
#ifndef NULL
#define NULL 0
#endif

#ifndef CHAR_LEN_64
#define CHAR_LEN_64 64
#endif

#ifndef CHAR_LEN_32
#define CHAR_LEN_32 32
#endif

 
/*----------------------------------------------*
 * ö�ٶ���                                       *
 *----------------------------------------------*/
 /****************** �������� *********************/
typedef enum 
{
    OM_E_NONE,                  /* 0, successful */
    OM_E_RADIOID,               /* invalid radio ID */
    OM_E_WLANID,                /* invalid WLAN ID */
    OM_E_IFID,                  /* invalid interface ID */
    OM_E_CMD_ID,                /* invalid command ID */
    OM_E_ASYNC_CMD_ID,          /* invalid async command ID */
    OM_E_EVENT_ID,              /* invalid event ID */
    OM_E_VALUE,                 /*Invalid value*/
    OM_E_FILE_NAME,             /*Invalid file name*/
    OM_E_RANGE,                 /* parameter exceed range */
    OM_E_PARAM,                 /* other invalid parameter */
    OM_E_RESOURCE,              /* fail to get resource*/
    OM_E_NOT_EXIST,             /* Object don't exist */
    OM_E_INIT,                  /* Feature not initialized. */
    OM_E_REINIT,                /* Feature initialized more than once. */
    OM_E_ORDER,                 /* Operator is not in order */
    OM_E_MEM_MALLOC,            /* memory malloc error */
    OM_E_UPGRADE_NOSPACE,       /* upgrade no space error */
    OM_E_NULLPTR,               /*an pointer point to NULL*/
    OM_E_SYSTEM_CMD,            /* system cmd fail */
    OM_E_DEFAULT_FAIL,          /*default failure*/
} OM_ERROR_CODE_E;


/*----------------------------------------------*
 * ��չ���Ͷ���                                     *
 *----------------------------------------------*/


/*----------------------------------------------*
 * �ṹ����                                       *
 *----------------------------------------------*/



#endif /* __OM_TYPES_H__ */

