/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : procfs_fops.c
  版 本 号   : 初稿
  作    者   : Zhong Huaqing
  生成日期   : 2012年9月21日 星期五
  最近修改   :
  功能描述   : AP整体流控操作
  函数列表   :
  修改历史   :
  1.日    期   : 2012年9月21日 星期五
    作    者   : Zhong Huaqing
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/proc_fs.h>     
#include <asm/uaccess.h> 
#include <linux/version.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20))    
#include <asm/semaphore.h>
#else
#include <linux/mutex.h>
#endif

#include "ap_types.h"

#include "procfs_fops.h"

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/
LOCAL int procfs_fops_open(struct inode *pstInode, struct file *pstFile);
LOCAL int procfs_fops_close(struct inode *pstInode, struct file *pstFile);
LOCAL int procfs_fops_ioctl(
    struct inode *pstInode, 
    struct file *pstFile, 
    UINT32 uiCmd, 
    ULONG ulData);

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
IOCTL_F g_fProcIoctl = NULL;

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/
LOCAL struct proc_dir_entry *g_pstProcEntry = NULL;

LOCAL struct file_operations g_stProcFileOper = {
	.ioctl 	 = procfs_fops_ioctl,
	.open 	 = procfs_fops_open,
	.release = procfs_fops_close,
};

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20))    
    LOCAL DECLARE_MUTEX(g_stProcfsMutex);
#else
    LOCAL DEFINE_MUTEX(g_stProcfsMutex);
#endif

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 函数定义                                       *
 *----------------------------------------------*/

void inline procfs_mutex_lock(void)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20))    
    down(&g_stProcfsMutex);
#else
    mutex_lock(&g_stProcfsMutex);
#endif
}


void inline procfs_mutex_unlock(void)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20))    
    up(&g_stProcfsMutex);
#else
    mutex_unlock(&g_stProcfsMutex);
#endif
}


/*****************************************************************************
 函 数 名  : procfs_fops_open
 功能描述  : proc文件打开操作
 输入参数  : struct inode *pstInode  
             struct file *pstFile    
 输出参数  : 无
 返 回 值  : LOCAL
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月20日 星期四
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
LOCAL int procfs_fops_open(struct inode *pstInode, struct file *pstFile)
{
	try_module_get(THIS_MODULE);
	return 0;
}

/*****************************************************************************
 函 数 名  : procfs_fops_close
 功能描述  : Proc文件关闭操作
 输入参数  : struct inode *pstInode  
             struct file *pstFile    
 输出参数  : 无
 返 回 值  : LOCAL
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月20日 星期四
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
LOCAL int procfs_fops_close(struct inode *pstInode, struct file *pstFile)
{
	module_put(THIS_MODULE);
	return 0; 							 
}

/*****************************************************************************
 函 数 名  : procfs_fops_ioctl
 功能描述  : Proc文件Ioctl操作
 输入参数  : struct inode *pstInode  
             struct file *pstFile    
             UINT32 uiCmd            
             ULONG ulData            
 输出参数  : 无
 返 回 值  : LOCAL
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月20日 星期四
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
LOCAL int procfs_fops_ioctl(
    struct inode *pstInode, 
    struct file *pstFile, 
    UINT32 uiCmd, 
    ULONG ulData)
{
    AP_ERROR_CODE_E enRet;
    
    //printk("uiCmd = %d\n", uiCmd);
    if (PROCFS_FOPS_IOCTL_CMD != uiCmd)
    {
        return -EFAULT;
    }

    if (NULL != g_fProcIoctl)
    {
        procfs_mutex_lock();
        enRet = g_fProcIoctl((void*)ulData);
        procfs_mutex_unlock();
        if (AP_E_NONE != enRet)
        {
            return -EFAULT;
        }
        return 0;
    }
    
    return -EFAULT;
}

/*****************************************************************************
 函 数 名  : procfs_fops_init
 功能描述  : 创建Proc文件
 输入参数  : CHAR* szFileName  --文件名称
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月20日 星期四
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E procfs_fops_init(CHAR* szFileName)
{
    if (NULL == szFileName)
    {
        return AP_E_PARAM;
    }
        
    g_pstProcEntry = create_proc_entry( szFileName, 0644, NULL);
    if (NULL == g_pstProcEntry)
    {
        printk("Create /proc/%s failed.\n", szFileName);
        return AP_E_RESOURCE;
    }
    
    g_pstProcEntry->proc_fops = &g_stProcFileOper;
    g_pstProcEntry->mode = S_IFREG | S_IRUGO | S_IWUSR;
    //g_pstProcEntry->uid = 0;
	//g_pstProcEntry->gid = 0;
    
    return AP_E_NONE;
}

/*****************************************************************************
 函 数 名  : procfs_fops_destroy
 功能描述  : 删除Proc文件
 输入参数  : CHAR* szFileName  --文件名称
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月20日 星期四
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E procfs_fops_destroy(CHAR* szFileName)
{
    remove_proc_entry(szFileName, NULL);
    return AP_E_NONE;
}

/*****************************************************************************
 函 数 名  : procfs_fops_hook_set
 功能描述  : proc文件操作的ioctl接口钩子初始化
 输入参数  : IOCTL_FUNC_T fProcIoctl  -- 钩子函数对应的实体函数指针
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月20日 星期四
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E procfs_fops_hook_set(IOCTL_F fProcIoctl)
{
    g_fProcIoctl = fProcIoctl;
    return AP_E_NONE;
}

