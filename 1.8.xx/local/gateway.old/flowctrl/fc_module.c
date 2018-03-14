/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : fc_module.c
  版 本 号   : 初稿
  作    者   : Zhong Huaqing
  生成日期   : 2012年8月13日 星期一
  最近修改   :
  功能描述   : 流控模块初始化
  函数列表   :
              flow_ctrl_cleanup_module
              flow_ctrl_init_module
              flow_ctrl_ioctl_set
  修改历史   :
  1.日    期   : 2012年8月13日 星期一
    作    者   : Zhong Huaqing
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>

#include <linux/proc_fs.h>

#include "fc_main.h"
#include "fc_queue.h"

#include "procfs_fops.h"

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
REMOTE AP_ERROR_CODE_E fc_ioctl_handler(void *pstArg);

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 函数定义                                       *
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

