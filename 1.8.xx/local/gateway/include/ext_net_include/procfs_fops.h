/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : procfs_fops.h
  �� �� ��   : ����
  ��    ��   : Zhong Huaqing
  ��������   : 2012��9��21�� ������
  ����޸�   :
  ��������   : ʵ��procfs����ز��������ļ�ֻ�ܰ�����xxx_module.c�ļ��С�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��9��21�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ļ�
    
  2.��    ��   : 2012��11��28�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : ��procfs_fops.c�ļ�������ȫ���Ƶ����ļ��У��Ա�ʹ�ú�ά����

******************************************************************************/

#ifndef __PROC_FOPS_H__
#define __PROC_FOPS_H__


/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
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

#define PROC_ROOT_DIR    "/proc"
#define CMD_MAGIC 'x'
#define CMD_NR    1
#define PROCFS_FOPS_IOCTL_CMD   _IOWR(CMD_MAGIC,CMD_NR,void *)  /*2012-10-26 mod by zhangping */

 /*----------------------------------------------*
 * �����                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ö�ٶ���                                       *
 *----------------------------------------------*/


/*----------------------------------------------*
 * ��չ���Ͷ���                                     *
 *----------------------------------------------*/
#ifdef __KERNEL__
typedef AP_ERROR_CODE_E (*IOCTL_F)(void *pvData);
#endif


/*----------------------------------------------*
 * �ṹ����                                       *
 *----------------------------------------------*/


/*----------------------------------------------*
 * ��������ԭ��˵��                             *
 *----------------------------------------------*/

#ifdef __KERNEL__

IOCTL_F g_fProcIoctl = NULL;


/*�ں˻������*/
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
int procfs_fops_open(struct inode *pstInode, struct file *pstFile)
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
int procfs_fops_close(struct inode *pstInode, struct file *pstFile)
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
#if (defined(GW_X86) || defined(GW_ARM))   //add by cj

int procfs_fops_ioctl(
    struct file *pstFile, 
    UINT32 uiCmd, 
    ULONG ulData)   //�޸�Ϊ2.6.39

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
    ULONG ulData)   //�޸�Ϊ2.6.39    
#endif
    
#endif    
{

    /*�˴�����ʹ�þ�̬����*/
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
    struct proc_dir_entry *s_pstProcEntry = NULL;
    
    /*�˴��ṹ���������Ϊ��̬ģʽ*/
#if (defined(GW_X86) || defined(GW_ARM))  //add by cj

    LOCAL struct file_operations s_stProcFileOper = {
        .unlocked_ioctl      = procfs_fops_ioctl,   //2.6.39Ϊunlocked_ioctl
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
    .unlocked_ioctl      = procfs_fops_ioctl,   //2.6.39Ϊunlocked_ioctl
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

#endif
 

#endif /* __PROC_FOPS_H__ */

