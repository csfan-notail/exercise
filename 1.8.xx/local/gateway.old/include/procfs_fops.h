/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : procfs_fops.h
  版 本 号   : 初稿
  作    者   : Zhong Huaqing
  生成日期   : 2012年9月21日 星期五
  最近修改   :
  功能描述   : 实现procfs的相关操作。本文件只能包含于xxx_module.c文件中。
  函数列表   :
  修改历史   :
  1.日    期   : 2012年9月21日 星期五
    作    者   : Zhong Huaqing
    修改内容   : 创建文件
    
  2.日    期   : 2012年11月28日 星期三
    作    者   : Zhong Huaqing
    修改内容   : 将procfs_fops.c文件的内容全部移到本文件中，以便使用和维护。

******************************************************************************/

#ifndef __PROC_FOPS_H__
#define __PROC_FOPS_H__


/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/

#ifdef __KERNEL__

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

#endif  /*__KERNEL__*/

#include "ap_types.h"

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

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
 * 基础类型定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
 
/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

#define PROC_ROOT_DIR    "/proc"
#define CMD_MAGIC 'x'
#define CMD_NR    1
#define PROCFS_FOPS_IOCTL_CMD   _IOWR(CMD_MAGIC,CMD_NR,void *)  /*2012-10-26 mod by zhangping */

 /*----------------------------------------------*
 * 宏操作                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 枚举定义                                       *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 扩展类型定义                                     *
 *----------------------------------------------*/
#ifdef __KERNEL__
typedef AP_ERROR_CODE_E (*IOCTL_F)(void *pvData);
#endif


/*----------------------------------------------*
 * 结构定义                                       *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 导出函数原型说明                             *
 *----------------------------------------------*/

#ifdef __KERNEL__

IOCTL_F g_fProcIoctl = NULL;


/*内核互斥操作*/
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20))
inline void procfs_mutex_lock(struct semaphore *pstMutex)
{
    down(pstMutex);
}
inline void procfs_mutex_unlock(struct semaphore *pstMutex)
{
    up(pstMutex);
}
#else
inline void procfs_mutex_lock(struct mutex *pstMutex)
{
    mutex_lock(pstMutex);
}
inline void procfs_mutex_unlock(struct mutex *pstMutex)
{
    mutex_unlock(pstMutex);
}

#endif

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
int procfs_fops_open(struct inode *pstInode, struct file *pstFile)
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
int procfs_fops_close(struct inode *pstInode, struct file *pstFile)
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
#if (defined(GW_X86) || defined(GW_ARM))   //add by cj

int procfs_fops_ioctl(
    struct file *pstFile, 
    UINT32 uiCmd, 
    ULONG ulData)   //修改为2.6.39

#else
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,39))    
int procfs_fops_ioctl(
    struct inode *pstInode, 
    struct file *pstFile, 
    UINT32 uiCmd, 
    ULONG ulData)
#else
int procfs_fops_ioctl(
    struct file *pstFile, 
    UINT32 uiCmd, 
    ULONG ulData)   //修改为2.6.39    
#endif
    
#endif    
{

    /*此处必须使用静态变量*/
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20))    
    LOCAL DECLARE_MUTEX(s_stProcfsMutex);
#else
    LOCAL DEFINE_MUTEX(s_stProcfsMutex);
#endif

    AP_ERROR_CODE_E enRet;
    
    //printk("uiCmd = %d\n", uiCmd);
    if (PROCFS_FOPS_IOCTL_CMD != uiCmd)
    {
        return -EFAULT;
    }

    if (NULL != g_fProcIoctl)
    {
        procfs_mutex_lock(&s_stProcfsMutex);
        enRet = g_fProcIoctl((void*)ulData);
        procfs_mutex_unlock(&s_stProcfsMutex);
        
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
    struct proc_dir_entry *s_pstProcEntry = NULL;
    
    /*此处结构体必须设置为静态模式*/
#if (defined(GW_X86) || defined(GW_ARM))  //add by cj

    LOCAL struct file_operations s_stProcFileOper = {
        .unlocked_ioctl      = procfs_fops_ioctl,   //2.6.39为unlocked_ioctl
        .open    = procfs_fops_open,
        .release = procfs_fops_close,
    };

#else
    #if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,39))    
    LOCAL struct file_operations s_stProcFileOper = {
    	.ioctl 	 = procfs_fops_ioctl,
    	.open 	 = procfs_fops_open,
    	.release = procfs_fops_close,
    };
    #else
    LOCAL struct file_operations s_stProcFileOper = {
    .unlocked_ioctl      = procfs_fops_ioctl,   //2.6.39为unlocked_ioctl
    .open    = procfs_fops_open,
    .release = procfs_fops_close,
    };
    #endif
    
#endif

    if (NULL == szFileName)
    {
        return AP_E_PARAM;
    }
        
    s_pstProcEntry = create_proc_entry( szFileName, 0644, NULL);
    if (NULL == s_pstProcEntry)
    {
        printk("Create /proc/%s failed.\n", szFileName);
        return AP_E_RESOURCE;
    }
    
    s_pstProcEntry->proc_fops = &s_stProcFileOper;
    s_pstProcEntry->mode = S_IFREG | S_IRUGO | S_IWUSR;
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

#endif
 

#endif /* __PROC_FOPS_H__ */

