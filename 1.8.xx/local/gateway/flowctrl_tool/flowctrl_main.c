/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : flowctrl_main.c
  �� �� ��   : ����
  ��    ��   : Zhong Huaqing
  ��������   : 2012��8��15�� ������
  ����޸�   :
  ��������   : ���ع���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��8��15�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include <stdio.h>
#include <errno.h>  
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>  
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>		
#include <unistd.h>		
#include <sys/ioctl.h>	
#include <net/if.h> 
#include <netinet/in.h> 
#include <sys/errno.h>
#include <getopt.h>


#include "fc_public.h"
#include "fc_ioctl.h"

#include "flowctrl_main.h"
#include "flowctrl_cmd.h"

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

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/
//int g_stSocketFd;

INT32 g_iProcFd;
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

AP_ERROR_CODE_E fc_ioctl_init(void)
{
    #define FC_FILE_NAME_SIZE 128
    
    CHAR acProcFileName[FC_FILE_NAME_SIZE];
    INT32 iFd;
    
    snprintf(acProcFileName, FC_FILE_NAME_SIZE, 
            "%s/%s", PROC_ROOT_DIR, FC_PROC_FILE);
            
    g_iProcFd = open(acProcFileName, O_RDWR);
    if (g_iProcFd < 0)
    {
        printf("fc_ioctl_init faild. fd = %d\n", g_iProcFd);
        return AP_E_RESOURCE;
    }
    
    return AP_E_NONE;
}

AP_ERROR_CODE_E fc_ioctl_destroy(void)
{
    close(g_iProcFd);
    return AP_E_NONE;
}

AP_ERROR_CODE_E fc_ioctl(FC_IOCTL_T *pstIoctl, FC_IOCTL_CMD_E enCmd)
{
	AP_ERROR_CODE_E enRet;

	if (NULL == pstIoctl)
	{
		return AP_E_PARAM;
	}
    
    enRet = fc_ioctl_init();
    if (AP_E_NONE != enRet)
    {
        return AP_E_RESOURCE;
    }
    
    pstIoctl->enCmd = enCmd;
	enRet = ioctl(g_iProcFd, PROCFS_FOPS_IOCTL_CMD, pstIoctl);
	if (AP_E_NONE != enRet)
	{
		printf("fc_ioctl() failed. ret = %d\n", enRet);
	}
	fc_ioctl_destroy();
	
	return enRet;
}


INT32 main(INT32 iArgc, CHAR *apcArgv[])
{
    FC_CMD_T *pstCmd;
    CHAR *szCmdName = NULL;
    AP_ERROR_CODE_E enRet;
    
    if (iArgc < 2)
    {
        printf("Parameters invalid!\n");
        fc_show_help();
        return -1;
    }
    
    /*���������Ӧ�ṹ*/
    szCmdName = apcArgv[1];
    pstCmd = fc_get_cmd(szCmdName);
    if (NULL == pstCmd)
    {
        printf("Command invalid!\n");
        fc_show_help();
        return -1;
    }
    
    /*ִ������*/
    iArgc--;
	apcArgv++;
	enRet = pstCmd->fHandler(iArgc, apcArgv);
	if (AP_E_NONE != enRet)
	{
	    printf("Command %s executed failed. ret %d\n", szCmdName, enRet);
	}
    
    return 0;
}

