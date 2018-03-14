/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : fc_module.c
  �� �� ��   : ����
  ��    ��   : Zhong Huaqing
  ��������   : 2012��8��13�� ����һ
  ����޸�   :
  ��������   : ����ģ���ʼ��
  �����б�   :
              flow_ctrl_cleanup_module
              flow_ctrl_init_module
              flow_ctrl_ioctl_set
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

#include "fc_main.h"
#include "fc_queue.h"

#include "procfs_fops.h"

/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/
REMOTE AP_ERROR_CODE_E fc_ioctl_handler(void *pstArg);

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

/* BEGIN: Modified by xucongjiang, 2012/10/20 */
REMOTE struct notifier_block fc_device_notifier;
/* END:   Modified by xucongjiang, 2012/10/20   PN: */
 

LOCAL int flow_ctrl_init_module(void)
{
	flowctrl_init();
	fc_hook_init();
	
	procfs_fops_hook_set(fc_ioctl_handler);

    /* BEGIN: Modified by xucongjiang, 2012/10/20 */
    register_netdevice_notifier(&fc_device_notifier);
    /* END:   Modified by xucongjiang, 2012/10/20   PN: */
	
	procfs_fops_init(FC_PROC_FILE);
	
	return 0;
}

LOCAL void  flow_ctrl_cleanup_module(void)
{
		
	procfs_fops_hook_set(NULL);
	fc_hook_destroy();
	fc_queue_flush_all();

	/* BEGIN: Modified by xucongjiang, 2012/10/20 */
	unregister_netdevice_notifier(&fc_device_notifier);
	/* END:   Modified by xucongjiang, 2012/10/20   PN: */
	
	procfs_fops_destroy(FC_PROC_FILE);
	return;
}


module_init(flow_ctrl_init_module);
module_exit(flow_ctrl_cleanup_module);

MODULE_LICENSE("GPL");

/*End of File*/

