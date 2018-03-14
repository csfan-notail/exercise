/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : mirror_lib.c
  版 本 号   : 初稿
  作    者   : xiao wei
  生成日期   : 2016年3月6日 星期三
  最近修改   :
  功能描述   : 
  函数列表   :
  修改历史   :
  1.日    期   : 2016年3月6日 星期三
    作    者   : xiao wei
    修改内容   : 创建文件

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
#include "mirror_ioctl.h"

 
INT32 mirror_ioctl_init(void)
{    
    INT32 iProcFd = 0;
    CHAR acProcFileName[32];
    
    snprintf(acProcFileName, 32, "%s/%s", PROC_ROOT_DIR, MIRROR_PROC_FILE);
    iProcFd = open(acProcFileName, O_RDWR);
   
    return iProcFd;
}

AP_ERROR_CODE_E mirror_ioctl_destroy(INT32 iProcFd)
{
    close(iProcFd);
    return AP_E_NONE;
}

AP_ERROR_CODE_E mirror_ioctl(MIRROR_IOCTL_T *pstIoctl, MIRROR_IOCTL_CMD enCmd)
{
    INT32 iProcFd;
    INT32 iRet             = 0;
    INT32 iLoop            = 0;
    INT32 iMirrorPort      = 0;
    INT32 iObservePort     = 0;
    MIRROR_IOCTL_T stIoctl = {0};
	
    if (NULL == pstIoctl)
    {
        syslog(3,"mirror_ioctl MIRROR_IOCTL_T *pstIoctl = NULL Error! \n");
        return AP_E_PARAM;
    }
    
    /* porcfile ioctl init */
    iProcFd = mirror_ioctl_init();
    if (iProcFd < 0)
    {
        syslog(3,"Call mirror_ioctl_init failed. iProcFd = %d\n", iProcFd);
        return AP_E_RESOURCE;
    }
	
    /* check mirror port and observe port : Not equal */
    if( MIRROR_IOCTL_MIRROR == enCmd || MIRROR_IOCTL_OBSERVE == enCmd )
    {
        memcpy(&stIoctl,pstIoctl,sizeof(MIRROR_IOCTL_T));
        //use CMD show get mirror port and observe port
        stIoctl.enCmd = MIRROR_IOCTL_SHOW;
        //save current mirror port and observe port
        iMirrorPort = stIoctl.stMirrorData.uiMirrorPort;
        iObservePort = stIoctl.stMirrorData.uiObservePort;
		
        iRet = ioctl(iProcFd, PROCFS_FOPS_IOCTL_CMD, &stIoctl);
        if (0 != iRet)
        {
            syslog(4,"call ioctl check mirror/observe repeat failed!\n");
        }
        else
        {
            //printf("call ioctl check mirror and observe port!\n");
            if( MIRROR_IOCTL_MIRROR == enCmd )
            { 
                //check all get observe port
                #if 0
                for( iLoop = stIoctl.stMirrorData.stGmacStatus.uiStartPort; iLoop <= stIoctl.stMirrorData.stGmacStatus.uiEndPort; iLoop++ )  
                {
                    //curr observe port use status
                    if( MIRROR_IOCTL_OBSERVE_PORT_NO != stIoctl.stMirrorData.stObservePort[iLoop].enObservePortCurr)
                    {
                        //find mirror equal observe port
                        if( iLoop == iMirrorPort )
                        {
                            syslog(3,"Set mirror of the ports has been set to observe port!\n");
                            return AP_E_DEFAULT_FAIL;
                        }
                    }
                }
				#endif
                if( MIRROR_PORT_CLOSE != iMirrorPort )
                {
                    if ( iMirrorPort < stIoctl.stMirrorData.stGmacStatus.uiStartPort || iMirrorPort > stIoctl.stMirrorData.stGmacStatus.uiEndPort )
                    {
                        syslog(3,"Set the port is beyond the scope Can't use port number !\n");
                        return AP_E_DEFAULT_FAIL;
                    }
                }
            }
            else if( MIRROR_IOCTL_OBSERVE == enCmd )
            {
            	#if 0
                if( iObservePort == stIoctl.stMirrorData.uiMirrorPort)
                {
                    syslog(3,"Set observe of the ports has been set to mirror port!\n");
                    return AP_E_DEFAULT_FAIL;
                }
                #endif
                if( MIRROR_IOCTL_OBSERVE_PORT_NO != iObservePort )
                {
                    if ( iObservePort < stIoctl.stMirrorData.stGmacStatus.uiStartPort || iObservePort > stIoctl.stMirrorData.stGmacStatus.uiEndPort )
                    {
                        syslog(3,"Set the port is beyond the scope Can't use port number !\n");
                        return AP_E_DEFAULT_FAIL;
                    }
                }
            }
         }
    }    
	
    pstIoctl->enCmd = enCmd;
    iRet = ioctl(iProcFd, PROCFS_FOPS_IOCTL_CMD, (void*)pstIoctl);
    if (0 != iRet)
    {
        syslog(3,"Call ioctl failed. ret = %d\n", iRet);
        mirror_ioctl_destroy(iProcFd);
        return AP_E_DEFAULT_FAIL;
    }
    
    mirror_ioctl_destroy(iProcFd);
    
    return AP_E_NONE;
}


