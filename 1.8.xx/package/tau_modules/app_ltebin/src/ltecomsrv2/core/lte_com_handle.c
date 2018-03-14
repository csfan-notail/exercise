#include <stdio.h>
#include <termios.h>
#include <sys/time.h>
#include <fcntl.h>
#include <string.h>  
#include <sys/types.h>  
#include <errno.h>  
#include <sys/stat.h>   
#include <unistd.h>    
#include <stdlib.h>  
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/sysinfo.h>

#include "lte_com.h"
#include "lte_log.h"

int  lte_set_com(int iComFd) 
{
   struct termios TtyAttr;
   int DeviceSpeed = B115200;
   int ByteBits = CS8;

   memset(&TtyAttr, 0, sizeof(struct termios));
   TtyAttr.c_iflag = IGNPAR;
   TtyAttr.c_cflag = DeviceSpeed | HUPCL | ByteBits | CREAD | CLOCAL;
   TtyAttr.c_cc[VMIN] = 1;

   if (tcsetattr(iComFd, TCSANOW, &TtyAttr) < 0)
   {
       LTE_LOG(LTE_LOG_ERR,"%d,%s:Unable to set comm port",__LINE__,__func__);
       return -1;
   }
   return 0;
   /*程序将终止在这边，循环读取com内容*/

}

int lte_open_serial_com(int *piComFd,char *pcCom)
{
    int iSetAcm = 0;	
    int iComFd = 0;
    int flags = 0;
    if (NULL == piComFd)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:invalid para\n",__LINE__,__func__);
        return -1;
    }
    
    //打开串口需要添加属性O_NOCTTY，否则任何输入都会影响进程
    if ((iComFd = open(pcCom,O_RDWR | O_NOCTTY | O_NONBLOCK)) < 0) 
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:Can't open usb com ",__LINE__,__func__);
        return -1;
    }
    
    /*将com口设置为非阻塞模式*/
    flags = fcntl(iComFd, F_GETFL, 0);
    LTE_LOG(LTE_LOG_ERR,"%d,%s:get fcntl flags==%d",__LINE__,__func__, flags);
    #if 0
    if (fcntl(iComFd, F_SETFL, flags | O_NONBLOCK) < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:Unable set to NONBLOCK mode",__LINE__,__func__);
    }
    #endif
    /*设置COM波特率等其他属性*/
    if ((iSetAcm= lte_set_com(iComFd)) < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:set com error.\n",__LINE__,__func__);
        return -2;
    }
    
    /*将com向外部赋值*/
    *piComFd = iComFd;
    
    return 0;
}
int lte_close_serial_com(int iComFd)
{
    close(iComFd);
    return 0;
}


int lte_usb_com_init(int *piComFd, char *pcAtCom)
{
    int iComFd = 0;
    
    int iRet   = 0;
    
    if ((NULL == piComFd) || (NULL == pcAtCom))
    {
       LTE_LOG(LTE_LOG_ERR,"%d,%s, input param is null pointer!\n",__LINE__,__func__);
       return -1;
    }

    /* 打开ttyUSB* 口，已进行lte配置及模块信息获取 */
	LTE_LOG(LTE_LOG_INFO,"\n at com:%s\n", pcAtCom);
    /* 打开com口 */
    iRet =  lte_open_serial_com(&iComFd, pcAtCom);
    if (0 == iRet)
    {
       ;
    }
    else if (-2 == iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"set com failed\n");
       /*打开了com口,需要关闭*/
       lte_close_serial_com(iComFd);
       return -1;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"open com failed\n");
        return -1;    
    }
    
    LTE_LOG(LTE_LOG_INFO,"\n Open at com [%s] success!, com fd [%d]\n", pcAtCom, iComFd);
    
    *piComFd = iComFd;
    
    return 0;
}

int lte_usb_com_register(LTE_PLATFORM_E enLtePlatformType, LTE_EVENT_T *pstLteEvent)
{
    char acCom[32] = {0};
    int  iRet = 0;
    int  iComFd = 0;
    if (NULL == pstLteEvent)
    {
       LTE_LOG(LTE_LOG_ERR,"%d,%s, input param is null pointer!\n",__LINE__,__func__);
       return -1;
    }
    
    /* 注册模块的操作方法 */
    iRet = lte_module_init(enLtePlatformType, acCom, sizeof(acCom));
    if(0 == iRet)
    {
        LTE_LOG(LTE_LOG_ERR, "register lte ops failed!");
        return -1;
    }
    
    /* 注册usb com口 */
    iRet = lte_usb_com_init(&iComFd, acCom);
    if(iRet < 0)
    {
        LTE_LOG(LTE_LOG_ERR, "register lte usb com failed!");
        return -1;
    }
    
    pstLteEvent->iComFd = iComFd;
    pstLteEvent->enComSM = WRITE_ABLE;
    
    /* 初始化，com口读取数据解析的链表 */
    list_init();
    return 0;
}


