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

#include "lte_log.h"
#include "lte.h"
#include "lte_common.h"

  /*****************************************************************************
 函 数 名  : simple_strtoul
 功能描述  :  将字符串换算成字符数字,代码移植过来，风格不动
 输入参数  :  要转换的字符串，endp 一般设置为NULL，base 转换的进制 
 输出参数  : 
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月15 星期三
    作    者   : 蔡兵
    修改内容   : 新生成函数

*****************************************************************************/

unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base)
{
    unsigned long result = 0,value;
    if (*cp == '0') {
        cp++;
        if ((*cp == 'x') && isxdigit(cp[1])) {
            base = 16;
            cp++;
        }
        if (!base) {
            base = 8;
        }
    }
    if (!base) {
        base = 10;
    }
    while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp-'0' : (islower(*cp)? toupper(*cp) : *cp)-'A'+10) < base)
    {
        result = result*base + value;
        cp++;
    }
    if (endp)
        *endp = (char *)cp;
    return result;
}
/*****************************************************************************
  函 数 名  : lte_set_com
  功能描述  :  设置com口的属性
  输入参数  :  
  输出参数  : 
  返 回 值  : 
  调用函数  : 
  被调函数  : 
  
  修改历史      :
   1.日    期   : 2012年8月15 星期三
     作    者   : 蔡兵
     修改内容   : 新生成函数
 
 *****************************************************************************/

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

 /*****************************************************************************
 函 数 名  : lte_open_serial_com
 功能描述  : 打开串口
 输入参数  :   pvDdata 将带入文件描述符
 输出参数  :   文件描述符
 返 回 值  : -1表示文件入参或者打开文件错误，无需关闭com口，-2 说明是打开com后设置错误，需要关闭com口
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月15 星期三
    作    者   : 蔡兵
    修改内容   : 新生成函数  
  2.日    期   : 2013年2月27 星期三
    作    者   : 蔡兵
    修改内容   : 新生成函数

*****************************************************************************/
int lte_open_serial_com(int *piComFd,char *pcCom)
{
    int iSetAcm = 0;	
    int iComFd = 0;
    
    if (NULL == piComFd)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:invalid para\n",__LINE__,__func__);
        return -1;
    }
    
    //打开串口需要添加属性O_NOCTTY，否则任何输入都会影响进程
    if ((iComFd = open(pcCom,O_RDWR | O_NOCTTY)) < 0) 
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:Can't open usb com ",__LINE__,__func__);
        return -1;
    }
    
    /*将com口设置为非阻塞模式*/
    if (fcntl(iComFd, F_SETFL, O_NONBLOCK) < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:Unable set to NONBLOCK mode",__LINE__,__func__);
    }

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
 /*****************************************************************************
 函 数 名  : lte_close_serial_com
 功能描述  : 解锁关闭串口
 输入参数  : 
 输出参数  :   文件描述符
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年2月27 星期三
    作    者   : 蔡兵
    修改内容   : 新生成函数

*****************************************************************************/
int lte_close_serial_com(int iComFd)
{
    close(iComFd);
    return 0;
}

/*****************************************************************************
 函 数 名  : lte_write_com
 功能描述  : 解锁关闭串口
 输入参数  : 
 输出参数  :   文件描述符
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年2月27 星期三
    作    者   : 蔡兵
    修改内容   : 新生成函数

*****************************************************************************/

int lte_write_com (int iComFd,char  *pcCmd)
{
    int iRet =0;
    int iLength = strlen(pcCmd);
    
    /*向com口写入at 指令*/
    iRet = write(iComFd,pcCmd,iLength);
    if (iRet != iLength)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:write com failed\n",__LINE__,__func__);
        return -1;
    }
    
    return 0;
}

/*****************************************************************************
 函 数 名  : lte_read_com
 功能描述  : 读com口信息
 输入参数  : iComfd:文件描述符。pcFilePath:将信息
             iSeconds:读取时间的设置
 输出参数  : 
 返 回 值  : -2 表示打开文件失败
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年2月27 星期三
    作    者   : 蔡兵
    修改内容   : 新生成函数

*****************************************************************************/
int  lte_read_com (int iComfd,char *pcFilePath,int iSecond)
{
  int iFd = 0;
  int iNwrite = 0;
  unsigned char cChar = 0;
  fd_set ReadSetFD;
  int iRet = 0;
  struct timeval stTimeout;
  
  /*阻塞的等待*/
  stTimeout.tv_sec = iSecond;
  /*联芯的采用周期为25ms,因此com口的采样周期为20ms*/
  stTimeout.tv_usec=0L;//1s=1000ms=1000*1000us

  /*打开将要书写的文件*/
  iFd = open(pcFilePath,O_CREAT|O_APPEND|O_WRONLY);
  if (iFd < 0)
  {
      LTE_LOG(LTE_LOG_ERR,"%d,%s,open file fail\n",__LINE__,__func__);
      return -1;
  }
  while ( 1 )
  {
      FD_ZERO(&ReadSetFD);
      FD_SET(iComfd, &ReadSetFD);
          /*依靠超时退出*/
      if (select(iComfd + 1, &ReadSetFD, NULL, NULL, &stTimeout) == 0)
      {
          close(iFd);
          return 0;
      }
      if (FD_ISSET(iComfd, &ReadSetFD)) 
      {
          while (1) 
          {
               iRet = read(iComfd, &cChar, 1);
		  if (iRet < 1)
		  	break;
              /*向文件中书写 从com口中读取到的数据*/
              iNwrite = write(iFd,&cChar,sizeof(unsigned char));
              if (iNwrite != 1)
              {
                  LTE_LOG(LTE_LOG_ERR,"%d,%s:write file failed\n",__LINE__,__func__);
                  close(iFd);
                  return -1;
              }
          }
      }
  }
  
  return 0;
}

int  lte_read_com_report_info(int iComfd, char *pcFilePath, char *pcNeedle, int iSecond)
{
    int     iFd = 0;
    int     iNwrite = 0;
    int     iLoop = 0;
    unsigned char cChar = 0;
    char    acBuff[256] = {0};
    fd_set  ReadSetFD;
    int     iRet = 0;
    struct timeval stTimeout;

    /*阻塞的等待*/
    stTimeout.tv_sec = iSecond;
    /*联芯的采用周期为25ms,因此com口的采样周期为20ms*/
    stTimeout.tv_usec=0L;//1s=1000ms=1000*1000us

    /*打开将要书写的文件*/
    iFd = open(pcFilePath,O_CREAT|O_APPEND|O_WRONLY);
    if (iFd < 0)
    {
      LTE_LOG(LTE_LOG_ERR,"%d,%s,open file fail\n",__LINE__,__func__);
      return -1;
    }
    while ( 1 )
    {
        FD_ZERO(&ReadSetFD);
        FD_SET(iComfd, &ReadSetFD);
          /*依靠超时退出*/
        if (select(iComfd + 1, &ReadSetFD, NULL, NULL, &stTimeout) == 0)
        {
            close(iFd);
            return 0;
        }
        if (FD_ISSET(iComfd, &ReadSetFD)) 
        {
            while (1) 
            {
                iRet = read(iComfd, &cChar, 1);
                if (iRet < 1)
                    break;
                acBuff[iLoop++] = cChar;
                /*向文件中书写 从com口中读取到的数据*/
                iNwrite = write(iFd,&cChar,sizeof(unsigned char));
                if (iNwrite != 1)
                {
                    LTE_LOG(LTE_LOG_ERR,"%d,%s:write file failed\n",__LINE__,__func__);
                    close(iFd);
                    return -1;
                }
                if('\n' == cChar)
                {
                    if(NULL != strstr(acBuff, pcNeedle))
                    {
                        close(iFd);
                        return 0;
                    }
                }
            }
        }
    }

    return 0;

}
/*****************************************************************************
 函 数 名  : lte_file_read_cmp
 功能描述  : 读取文件 同时 比较文件中的字符串
 输入参数  :  pcFileName 文件名， pcNeedle 字符串
 输出参数  :  
 返 回 值  : -2 表明资源型的错误
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月15 星期三
    作    者   : 蔡兵
    修改内容   : 新生成函数

*****************************************************************************/
int lte_file_read_cmp (const char* pcFileName,const char *pcNeedle)
{    
    FILE *pstFile = NULL;
    /*altered by caibing 10-15*/
    /*赋值字符串出错*/
    char acBuff[128] = {0};
    int iRet = 0;
    int iFlag = 0;
    char *pcTmp = NULL;
    
    if ((NULL == pcFileName) || (NULL == pcNeedle))
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:invalid para\n",__LINE__,__func__);
        return -2;
    }
    
    /*需要验证打开空文件的现象*/
    pstFile = fopen(pcFileName,"r");    
    if (NULL == pstFile)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s,open file failed\n",__LINE__,__func__);
        return -2;
    }
    
    while (fgets(acBuff,128,pstFile))
    {
        pcTmp = strstr(acBuff,pcNeedle);
        if(pcTmp)
        {
            iFlag = 1;
            break;
        }
        memset(acBuff,0,128);
    }
    /*关闭文件*/
    fclose(pstFile);

    /*指针赋值为空*/
    pstFile = NULL;
    if (1 == iFlag)
    {
        return 0;
    }
    return -1;
}

/*****************************************************************************
 函 数 名  : lte_file_unlink
 功能描述  : 删除中间文件
 输入参数  :  pcFileName 文件名
 输出参数  :  
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月15 星期三
    作    者   : 蔡兵
    修改内容   : 新生成函数

*****************************************************************************/

int lte_file_unlink (const char* pcFileName)
{
    int iRet = 0;
    iRet = unlink(pcFileName);
    if (iRet < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s unlink the %s failed\n",__LINE__,__func__,pcFileName);
        return -1;
    }
    return 0;
}

/*****************************************************************************
 函 数 名  : lte_normal_at_cmd
 功能描述  : 向com输入at指令
 输入参数  : 
 输出参数  :  
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年8月15 星期三
    作    者   : 陈键
    修改内容   : 新生成函数
    修改历史      :
  2.日    期   : 2013年02月28 星期四
    作    者   : 蔡兵
    修改内容   : 新生成函数

*****************************************************************************/

int lte_normal_at_cmd (LTE_NORMAL_AT_T* pstAtCmd)
{
    if (NULL == pstAtCmd)
    {
        return -1;
    }
    
    int iComFd = 0;
    int iRet   = 0;

    /*向系统日志输入运行的at指令*/
	LTE_LOG(LTE_LOG_INFO,"\n command:%s,%s,%s,%s\n",pstAtCmd->acCom,pstAtCmd->acCmd,pstAtCmd->acFileName,pstAtCmd->acNeedle);

    /* 打开com口 */
    iRet =  lte_open_serial_com(&(pstAtCmd->iComFd),pstAtCmd->acCom);
    if (0 == iRet)
    {
        ;
    }
    else if (-2 == iRet)
    {
        /*打开了com口,需要关闭*/
        lte_close_serial_com(pstAtCmd->iComFd);
        return -1;
    }
    else
    {
        return -1;    
    }

    /*向com口写入at 指令*/
    iRet = lte_write_com(pstAtCmd->iComFd, pstAtCmd->acCmd);
    if (iRet < 0)
    {
        lte_close_serial_com(pstAtCmd->iComFd);
        return -1;
    }

    /*向com口读数据*/
    iRet = lte_read_com(pstAtCmd->iComFd, pstAtCmd->acFileName,pstAtCmd->iSecond);

    /*关闭com口*/
    lte_close_serial_com(pstAtCmd->iComFd);

    /*对读进行判断*/
    if (iRet != 0)
    {        
    	LTE_LOG(LTE_LOG_ERR,"read usb com fail");
        return -1;
    }

    /*对读出的文件进行比较操作*/
    iRet = lte_file_read_cmp(pstAtCmd->acFileName, pstAtCmd->acNeedle);    
    pstAtCmd->iResult = iRet;

    if (-2 == iRet)
    {
    	LTE_LOG(LTE_LOG_ERR,"compare COM return string fail");
        return -1;
    }
    
    return 0;

}

int lte_get_cell_register_status(char *pcRegisterCmd,char *pcNeedle, char *pcUsbCom)
{
    if ( NULL == pcRegisterCmd)
    {
        return -1;
    }
    
    int iRet = 0;
    int iRegisterStatus = 0;
    LTE_RET_E enRet = 0;
    FILE  *pstFd = NULL;
    char acBuff[64] = {0};
    char acNeedle[64] = {0};
    int iStrlen = 0;
    LTE_NORMAL_AT_T stAtCmd = {"","/tmp/statusresiter","OK",0,"",0,1};
    memcpy(stAtCmd.acCom, pcUsbCom, sizeof(stAtCmd.acCom));

    char acNeedleComma[] = ",";
    char *pcTmp = NULL;    
    int iFlag = 0;
    
    /*拷入待较比的字符串 +CGREG,+CEREG,*/
    iStrlen = strlen(pcNeedle);
    strncpy(acNeedle,pcNeedle,iStrlen);

    /*拷入待传入的cmd*/
    iStrlen = strlen(pcRegisterCmd);
    strncpy(stAtCmd.acCmd,pcRegisterCmd,iStrlen);
    
    /*excute at command*/   
    iRet = lte_normal_at_cmd(&stAtCmd);
    if (iRet != 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:at command failed\n",__LINE__,__func__);
        lte_file_unlink(stAtCmd.acFileName);
        return -1;
    }
       
    if (-1 == stAtCmd.iResult)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:compare AT return value not OK\n",__LINE__,__func__);
        lte_file_unlink(stAtCmd.acFileName);
        return -1;
    }

    pstFd = fopen(stAtCmd.acFileName,"r");
    if (NULL == pstFd)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:open %s failed\n",__LINE__,__func__,stAtCmd.acFileName);
        lte_file_unlink(stAtCmd.acFileName);
        return -1;
    }

    while (NULL != fgets(acBuff,64,pstFd))
    { 	
        pcTmp = strstr(acBuff,acNeedle); 
        if (pcTmp != NULL)
        {
            iFlag = 1;
            break;      
        }
        memset(acBuff,0,64);
    } 
    
    fclose(pstFd);
    pstFd = NULL;
    lte_file_unlink(stAtCmd.acFileName);

    if (0 == iFlag)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:com can't read key word\n",__LINE__,__func__);
        return -1;
    }
    /*待切分的字符串:+CGREG: 1,1*/
    pcTmp = strtok(acBuff,acNeedleComma);
    if (pcTmp)
    {
        pcTmp = strtok(NULL,acNeedleComma);
        if (NULL == pcTmp)
        {
            LTE_LOG(LTE_LOG_ERR,"%d,%s:com can't read key word\n",__LINE__,__func__);
            return -1;
        }
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:com can't read key word\n",__LINE__,__func__);
        return -1;
    }
    
    iRegisterStatus = (int)simple_strtoul(pcTmp,NULL,10);
    LTE_LOG(LTE_LOG_INFO,"%d,%s:register status %d\n",__LINE__,__func__,iRegisterStatus);
    return iRegisterStatus;
    
}
int lte_normal_at_last_cmd(LTE_NORMAL_AT_T *pstAtCmd, char *pcNeedle)
{
    int iComFd = 0;
    int iRet   = 0;

    if (NULL == pstAtCmd)
    {
       return -1;
    }

    /*向系统日志输入运行的at指令*/
    LTE_LOG(LTE_LOG_INFO,"\n command:%s,%s,%s,%s\n",pstAtCmd->acCom,pstAtCmd->acCmd,pstAtCmd->acFileName,pstAtCmd->acNeedle);

    /* 打开com口 */
    iRet =  lte_open_serial_com(&(pstAtCmd->iComFd),pstAtCmd->acCom);
    if (0 == iRet)
    {
       ;
    }
    else if (-2 == iRet)
    {
       /*打开了com口,需要关闭*/
       lte_close_serial_com(pstAtCmd->iComFd);
       return -1;
    }
    else
    {
       return -1;    
    }

    /*向com口写入at 指令*/
    iRet = lte_write_com(pstAtCmd->iComFd, pstAtCmd->acCmd);
    if (iRet < 0)
    {
       lte_close_serial_com(pstAtCmd->iComFd);
       return -1;
    }

    /*向com口读数据*/
    iRet = lte_read_com_report_info(pstAtCmd->iComFd, pstAtCmd->acFileName,pcNeedle, pstAtCmd->iSecond);

    /*关闭com口*/
    lte_close_serial_com(pstAtCmd->iComFd);

    /*对读进行判断*/
    if (iRet != 0)
    {        
       LTE_LOG(LTE_LOG_ERR,"read usb com fail");
       return -1;
    }

    /*对读出的文件进行比较操作*/
    iRet = lte_file_read_cmp(pstAtCmd->acFileName, pstAtCmd->acNeedle);    
    pstAtCmd->iResult = iRet;

    if (-2 == iRet)
    {
       LTE_LOG(LTE_LOG_ERR,"compare COM return string fail");
       return -1;
    }

    return 0;

}
