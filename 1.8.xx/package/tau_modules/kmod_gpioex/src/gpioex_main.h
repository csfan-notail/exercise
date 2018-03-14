/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : gpioex_main.h
  �� �� ��   : ����
  ��    ��   : Zhong Huaqing
  ��������   : 2012��8��10�� ������
  ����޸�   :
  ��������   : gpioex_main.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��8��10�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __GPIOEX_MAIN_H__
#define __GPIOEX_MAIN_H__


/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/jiffies.h>
#include <linux/errno.h>
#include <linux/version.h>

#include <linux/skbuff.h>
#include <linux/jhash.h>

#include <linux/netdevice.h>
#include <linux/if_ether.h>

#include <linux/init.h>
#include <linux/mutex.h>
#include <asm/addrspace.h>
#include "xra1403.h"
#include "ar9344.h"
#include <linux/delay.h>
#include "ap_types.h"
#include "gpioex_ops.h"




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

 /*----------------------------------------------*
 * �����                                       *
 *----------------------------------------------*/
#define GPIOEX_LOG(level, fmt, args...)                  \
do{											    \
	if (level <= g_enDebugLevel)					        \
	{											\
		printk("GPIOEX %s(%d): ",__FUNCTION__, __LINE__); \
		printk(fmt, ##args);				    \
	}										    \
}while(0)

/*----------------------------------------------*
 * ö�ٶ���                                       *
 *----------------------------------------------*/
typedef enum
{
    GPIOEX_LOG_EMERG = 0,	/* system is unusable */
    GPIOEX_LOG_ALERT,	/* action must be taken immediately */
    GPIOEX_LOG_CRIT,	/* critical conditions */
    GPIOEX_LOG_ERR,	/* error conditions */
    GPIOEX_LOG_WARNING,	/* warning conditions */
    GPIOEX_LOG_NOTICE,	/* normal but significant condition */
    GPIOEX_LOG_INFO,	/* informational */
    GPIOEX_LOG_DEBUG,	/* debug-level messages */
}GPIOEX_LOG_LEVEL_E;
/*----------------------------------------------*
 * ��չ���Ͷ���                                     *
 *----------------------------------------------*/


/*----------------------------------------------*
 * �ṹ����                                       *
 *----------------------------------------------*/


/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������ԭ��˵��                             *
 *----------------------------------------------*/
 
AP_ERROR_CODE_E gpioex_set_debugLevel(UINT32 uiDebugLevel);
UINT32 gpioex_get_debugLevel(void);

AP_ERROR_CODE_E gpioex_init(void);
AP_ERROR_CODE_E gpioex_destroy(void);

//extern int ath_flash_spi_down_trylock(void);
//extern int ath_flash_spi_down(void);
//extern void ath_flash_spi_up(void);



/*----------------------------------------------*
 * ��������˵��                             *
 *----------------------------------------------*/
REMOTE GPIOEX_LOG_LEVEL_E g_enDebugLevel;

#endif /* __GPIOEX_MAIN_H__ */

