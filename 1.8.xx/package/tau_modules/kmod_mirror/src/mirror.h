/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : mirror.h
  �� �� ��   : ����
  ��    ��   : xiao wei
  ��������   : 2016��3��6�� ������
  ����޸�   :
  ��������   : mirror.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��3��6�� ������
    ��    ��   : xiao wei
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __mirror_MAIN_H__
#define __mirror_MAIN_H__

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
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
#define LOCAL                       static

#define MIRROR_VERSION_NUM          "1.0"

#define ATH_MIRROR_ENABLE_ADDRESS               0x0058
#define ATH_MIRROR_MIRRING_PORT_ADDRESS         0x0078
#define ATH_MIRROR_OBSERVE_PORT0_ADDRESS        0x0104
#define ATH_MIRROR_OBSERVE_PORT1_ADDRESS        0x0204
#define ATH_MIRROR_OBSERVE_PORT2_ADDRESS        0x0304
#define ATH_MIRROR_OBSERVE_PORT3_ADDRESS        0x0404
#define ATH_MIRROR_OBSERVE_PORT4_ADDRESS        0x0504
#define ATH_MIRROR_OBSERVE_PORT5_ADDRESS        0x0604

#define ATHR_GMAC_ETH_CFG                       0x18070000
 /*----------------------------------------------*
 * �����                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ö�ٶ���                                       *
 *----------------------------------------------*/
typedef enum
{
    MIRROR_LOG_EMERG = 0,       /* system is unusable */
    MIRROR_LOG_ALERT,           /* action must be taken immediately */
    MIRROR_LOG_CRIT,            /* critical conditions */
    MIRROR_LOG_ERR,             /* error conditions */
    MIRROR_LOG_WARNING,         /* warning conditions */
    MIRROR_LOG_NOTICE,          /* normal but significant condition */
    MIRROR_LOG_INFO,            /* informational */
    MIRROR_LOG_DEBUG,           /* debug-level messages */
}MIRROR_LOG_LEVEL_E;

MIRROR_LOG_LEVEL_E g_enDebugLevel = MIRROR_LOG_ERR;
//if (level <= g_enDebugLevel)
#define MIRROR_LOG(level, fmt, args...)                  \
do{                                             \
    if (level <= MIRROR_LOG_DEBUG)                            \
    {                                           \
        printk("mirror %s(%d): ",__FUNCTION__, __LINE__); \
        printk(fmt, ##args);                    \
    }                                           \
}while(0)


#endif /* __gpio_MAIN_H__ */

