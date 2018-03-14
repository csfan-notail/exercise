/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : reg_lib.c
  �� �� ��   : ����
  ��    ��   : xiao wei
  ��������   : 2016��3��6�� ������
  ����޸�   :
  ��������   : 
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��3��6�� ������
    ��    ��   : xiao wei
    �޸�����   : �����ļ�

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>  
#include <unistd.h>
#include <fcntl.h>	
#include <sys/ioctl.h>	

#include "ap_types.h"
#include "procfs_fops.h"
#include "reg_ioctl.h"

AP_ERROR_CODE_E register_ioctl_init(void)
{    
    INT32 iProcFd = 0;
    CHAR acProcFileName[32];
    
    snprintf(acProcFileName, 32, "%s/%s", PROC_ROOT_DIR, REG_PROC_FILE);
    iProcFd = open(acProcFileName, O_RDWR);
   
    return iProcFd;
}

AP_ERROR_CODE_E register_ioctl_destroy(INT32 iProcFd)
{
    close(iProcFd);
    return AP_E_NONE;
}

AP_ERROR_CODE_E register_ioctl(REGISTER_IOCTL_T *pstIoctl, REGISTER_IOCTL_CMD enCmd)
{
    INT32 iProcFd;
    INT32 iRet             = 0;
    INT32 iLoop            = 0;
    REGISTER_IOCTL_T stIoctl = {0};
	
    if (NULL == pstIoctl)
    {
        printf("register_ioctl REGISTER_IOCTL_T *pstIoctl = NULL Error! \n");
        return AP_E_PARAM;
    }
    
    iProcFd = register_ioctl_init();
    if (iProcFd < 0)
    {
        printf("Call register_ioctl_init failed. iProcFd = %d\n", iProcFd);
        return AP_E_RESOURCE;
    }   
	
    pstIoctl->enCmd = enCmd;
    iRet = ioctl(iProcFd, PROCFS_FOPS_IOCTL_CMD, (void*)pstIoctl);
    if (0 != iRet)
    {
        printf("Call register ioctl failed. ret = %d\n", iRet);
        register_ioctl_destroy(iProcFd);
        return AP_E_DEFAULT_FAIL;
    }
    
    register_ioctl_destroy(iProcFd);
    
    return AP_E_NONE;
}

