/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : gpioex_module.c
  �� �� ��   : ����
  ��    ��   : Zhong Huaqing
  ��������   : 2012��8��13�� ����һ
  ����޸�   :
  ��������   : ����ģ���ʼ��
  �����б�   :

  �޸���ʷ   :
  1.��    ��   : 2012��8��13�� ����һ
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>

#include <linux/proc_fs.h>

#include "gpioex_main.h"

#include "procfs_fops.h"
#include "gpioex_ioctl.h"

/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/
REMOTE AP_ERROR_CODE_E gpioex_ioctl_handler(void *pstArg);

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

/*----------------------------------------------*
 * ��������                                       *
 *----------------------------------------------*/


LOCAL int gpioex_init_module(void)
{
	gpioex_init();
	
	procfs_fops_hook_set(gpioex_ioctl_handler);
	procfs_fops_init(GPIOEX_PROC_FILE);
	
	return 0;
}

LOCAL void  gpioex_cleanup_module(void)
{

	procfs_fops_destroy(GPIOEX_PROC_FILE);
	procfs_fops_hook_set(NULL);
	
	gpioex_destroy();
	return;
}


module_init(gpioex_init_module);
module_exit(gpioex_cleanup_module);

MODULE_LICENSE("GPL");

/*End of File*/
