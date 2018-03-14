/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : procfs_fops.c
  �� �� ��   : ����
  ��    ��   : Zhong Huaqing
  ��������   : 2012��9��21�� ������
  ����޸�   :
  ��������   : AP�������ز���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��9��21�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
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
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/
LOCAL int procfs_fops_open(struct inode *pstInode, struct file *pstFile);
LOCAL int procfs_fops_close(struct inode *pstInode, struct file *pstFile);
LOCAL int procfs_fops_ioctl(
    struct inode *pstInode, 
    struct file *pstFile, 
    UINT32 uiCmd, 
    ULONG ulData);

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/
IOCTL_F g_fProcIoctl = NULL;

/*----------------------------------------------*
 * ģ�鼶����                                   *
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
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                       *
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
 �� �� ��  : procfs_fops_open
 ��������  : proc�ļ��򿪲���
 �������  : struct inode *pstInode  
             struct file *pstFile    
 �������  : ��
 �� �� ֵ  : LOCAL
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��20�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

*****************************************************************************/
LOCAL int procfs_fops_open(struct inode *pstInode, struct file *pstFile)
{
	try_module_get(THIS_MODULE);
	return 0;
}

/*****************************************************************************
 �� �� ��  : procfs_fops_close
 ��������  : Proc�ļ��رղ���
 �������  : struct inode *pstInode  
             struct file *pstFile    
 �������  : ��
 �� �� ֵ  : LOCAL
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��20�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

*****************************************************************************/
LOCAL int procfs_fops_close(struct inode *pstInode, struct file *pstFile)
{
	module_put(THIS_MODULE);
	return 0; 							 
}

/*****************************************************************************
 �� �� ��  : procfs_fops_ioctl
 ��������  : Proc�ļ�Ioctl����
 �������  : struct inode *pstInode  
             struct file *pstFile    
             UINT32 uiCmd            
             ULONG ulData            
 �������  : ��
 �� �� ֵ  : LOCAL
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��20�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

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
 �� �� ��  : procfs_fops_init
 ��������  : ����Proc�ļ�
 �������  : CHAR* szFileName  --�ļ�����
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��20�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

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
 �� �� ��  : procfs_fops_destroy
 ��������  : ɾ��Proc�ļ�
 �������  : CHAR* szFileName  --�ļ�����
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��20�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

*****************************************************************************/
AP_ERROR_CODE_E procfs_fops_destroy(CHAR* szFileName)
{
    remove_proc_entry(szFileName, NULL);
    return AP_E_NONE;
}

/*****************************************************************************
 �� �� ��  : procfs_fops_hook_set
 ��������  : proc�ļ�������ioctl�ӿڹ��ӳ�ʼ��
 �������  : IOCTL_FUNC_T fProcIoctl  -- ���Ӻ�����Ӧ��ʵ�庯��ָ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��20�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

*****************************************************************************/
AP_ERROR_CODE_E procfs_fops_hook_set(IOCTL_F fProcIoctl)
{
    g_fProcIoctl = fProcIoctl;
    return AP_E_NONE;
}

