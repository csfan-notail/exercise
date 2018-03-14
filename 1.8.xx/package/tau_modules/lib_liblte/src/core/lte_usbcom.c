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
 �� �� ��  : simple_strtoul
 ��������  :  ���ַ���������ַ�����,������ֲ��������񲻶�
 �������  :  Ҫת�����ַ�����endp һ������ΪNULL��base ת���Ľ��� 
 �������  : 
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��15 ������
    ��    ��   : �̱�
    �޸�����   : �����ɺ���

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
  �� �� ��  : lte_set_com
  ��������  :  ����com�ڵ�����
  �������  :  
  �������  : 
  �� �� ֵ  : 
  ���ú���  : 
  ��������  : 
  
  �޸���ʷ      :
   1.��    ��   : 2012��8��15 ������
     ��    ��   : �̱�
     �޸�����   : �����ɺ���
 
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
    /*������ֹ����ߣ�ѭ����ȡcom����*/

 }

 /*****************************************************************************
 �� �� ��  : lte_open_serial_com
 ��������  : �򿪴���
 �������  :   pvDdata �������ļ�������
 �������  :   �ļ�������
 �� �� ֵ  : -1��ʾ�ļ���λ��ߴ��ļ���������ر�com�ڣ�-2 ˵���Ǵ�com�����ô�����Ҫ�ر�com��
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��15 ������
    ��    ��   : �̱�
    �޸�����   : �����ɺ���  
  2.��    ��   : 2013��2��27 ������
    ��    ��   : �̱�
    �޸�����   : �����ɺ���

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
    
    //�򿪴�����Ҫ�������O_NOCTTY�������κ����붼��Ӱ�����
    if ((iComFd = open(pcCom,O_RDWR | O_NOCTTY)) < 0) 
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:Can't open usb com ",__LINE__,__func__);
        return -1;
    }
    
    /*��com������Ϊ������ģʽ*/
    if (fcntl(iComFd, F_SETFL, O_NONBLOCK) < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:Unable set to NONBLOCK mode",__LINE__,__func__);
    }

    /*����COM�����ʵ���������*/
    if ((iSetAcm= lte_set_com(iComFd)) < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:set com error.\n",__LINE__,__func__);
        return -2;
    }
    
    /*��com���ⲿ��ֵ*/
    *piComFd = iComFd;
    
    return 0;
}
 /*****************************************************************************
 �� �� ��  : lte_close_serial_com
 ��������  : �����رմ���
 �������  : 
 �������  :   �ļ�������
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��2��27 ������
    ��    ��   : �̱�
    �޸�����   : �����ɺ���

*****************************************************************************/
int lte_close_serial_com(int iComFd)
{
    close(iComFd);
    return 0;
}

/*****************************************************************************
 �� �� ��  : lte_write_com
 ��������  : �����رմ���
 �������  : 
 �������  :   �ļ�������
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��2��27 ������
    ��    ��   : �̱�
    �޸�����   : �����ɺ���

*****************************************************************************/

int lte_write_com (int iComFd,char  *pcCmd)
{
    int iRet =0;
    int iLength = strlen(pcCmd);
    
    /*��com��д��at ָ��*/
    iRet = write(iComFd,pcCmd,iLength);
    if (iRet != iLength)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:write com failed\n",__LINE__,__func__);
        return -1;
    }
    
    return 0;
}

/*****************************************************************************
 �� �� ��  : lte_read_com
 ��������  : ��com����Ϣ
 �������  : iComfd:�ļ���������pcFilePath:����Ϣ
             iSeconds:��ȡʱ�������
 �������  : 
 �� �� ֵ  : -2 ��ʾ���ļ�ʧ��
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��2��27 ������
    ��    ��   : �̱�
    �޸�����   : �����ɺ���

*****************************************************************************/
int  lte_read_com (int iComfd,char *pcFilePath,int iSecond)
{
  int iFd = 0;
  int iNwrite = 0;
  unsigned char cChar = 0;
  fd_set ReadSetFD;
  int iRet = 0;
  struct timeval stTimeout;
  
  /*�����ĵȴ�*/
  stTimeout.tv_sec = iSecond;
  /*��о�Ĳ�������Ϊ25ms,���com�ڵĲ�������Ϊ20ms*/
  stTimeout.tv_usec=0L;//1s=1000ms=1000*1000us

  /*�򿪽�Ҫ��д���ļ�*/
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
          /*������ʱ�˳�*/
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
              /*���ļ�����д ��com���ж�ȡ��������*/
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

    /*�����ĵȴ�*/
    stTimeout.tv_sec = iSecond;
    /*��о�Ĳ�������Ϊ25ms,���com�ڵĲ�������Ϊ20ms*/
    stTimeout.tv_usec=0L;//1s=1000ms=1000*1000us

    /*�򿪽�Ҫ��д���ļ�*/
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
          /*������ʱ�˳�*/
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
                /*���ļ�����д ��com���ж�ȡ��������*/
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
 �� �� ��  : lte_file_read_cmp
 ��������  : ��ȡ�ļ� ͬʱ �Ƚ��ļ��е��ַ���
 �������  :  pcFileName �ļ����� pcNeedle �ַ���
 �������  :  
 �� �� ֵ  : -2 ������Դ�͵Ĵ���
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��15 ������
    ��    ��   : �̱�
    �޸�����   : �����ɺ���

*****************************************************************************/
int lte_file_read_cmp (const char* pcFileName,const char *pcNeedle)
{    
    FILE *pstFile = NULL;
    /*altered by caibing 10-15*/
    /*��ֵ�ַ�������*/
    char acBuff[128] = {0};
    int iRet = 0;
    int iFlag = 0;
    char *pcTmp = NULL;
    
    if ((NULL == pcFileName) || (NULL == pcNeedle))
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:invalid para\n",__LINE__,__func__);
        return -2;
    }
    
    /*��Ҫ��֤�򿪿��ļ�������*/
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
    /*�ر��ļ�*/
    fclose(pstFile);

    /*ָ�븳ֵΪ��*/
    pstFile = NULL;
    if (1 == iFlag)
    {
        return 0;
    }
    return -1;
}

/*****************************************************************************
 �� �� ��  : lte_file_unlink
 ��������  : ɾ���м��ļ�
 �������  :  pcFileName �ļ���
 �������  :  
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��15 ������
    ��    ��   : �̱�
    �޸�����   : �����ɺ���

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
 �� �� ��  : lte_normal_at_cmd
 ��������  : ��com����atָ��
 �������  : 
 �������  :  
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��8��15 ������
    ��    ��   : �¼�
    �޸�����   : �����ɺ���
    �޸���ʷ      :
  2.��    ��   : 2013��02��28 ������
    ��    ��   : �̱�
    �޸�����   : �����ɺ���

*****************************************************************************/

int lte_normal_at_cmd (LTE_NORMAL_AT_T* pstAtCmd)
{
    if (NULL == pstAtCmd)
    {
        return -1;
    }
    
    int iComFd = 0;
    int iRet   = 0;

    /*��ϵͳ��־�������е�atָ��*/
	LTE_LOG(LTE_LOG_INFO,"\n command:%s,%s,%s,%s\n",pstAtCmd->acCom,pstAtCmd->acCmd,pstAtCmd->acFileName,pstAtCmd->acNeedle);

    /* ��com�� */
    iRet =  lte_open_serial_com(&(pstAtCmd->iComFd),pstAtCmd->acCom);
    if (0 == iRet)
    {
        ;
    }
    else if (-2 == iRet)
    {
        /*����com��,��Ҫ�ر�*/
        lte_close_serial_com(pstAtCmd->iComFd);
        return -1;
    }
    else
    {
        return -1;    
    }

    /*��com��д��at ָ��*/
    iRet = lte_write_com(pstAtCmd->iComFd, pstAtCmd->acCmd);
    if (iRet < 0)
    {
        lte_close_serial_com(pstAtCmd->iComFd);
        return -1;
    }

    /*��com�ڶ�����*/
    iRet = lte_read_com(pstAtCmd->iComFd, pstAtCmd->acFileName,pstAtCmd->iSecond);

    /*�ر�com��*/
    lte_close_serial_com(pstAtCmd->iComFd);

    /*�Զ������ж�*/
    if (iRet != 0)
    {        
    	LTE_LOG(LTE_LOG_ERR,"read usb com fail");
        return -1;
    }

    /*�Զ������ļ����бȽϲ���*/
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
    
    /*������ϱȵ��ַ��� +CGREG,+CEREG,*/
    iStrlen = strlen(pcNeedle);
    strncpy(acNeedle,pcNeedle,iStrlen);

    /*����������cmd*/
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
    /*���зֵ��ַ���:+CGREG: 1,1*/
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

    /*��ϵͳ��־�������е�atָ��*/
    LTE_LOG(LTE_LOG_INFO,"\n command:%s,%s,%s,%s\n",pstAtCmd->acCom,pstAtCmd->acCmd,pstAtCmd->acFileName,pstAtCmd->acNeedle);

    /* ��com�� */
    iRet =  lte_open_serial_com(&(pstAtCmd->iComFd),pstAtCmd->acCom);
    if (0 == iRet)
    {
       ;
    }
    else if (-2 == iRet)
    {
       /*����com��,��Ҫ�ر�*/
       lte_close_serial_com(pstAtCmd->iComFd);
       return -1;
    }
    else
    {
       return -1;    
    }

    /*��com��д��at ָ��*/
    iRet = lte_write_com(pstAtCmd->iComFd, pstAtCmd->acCmd);
    if (iRet < 0)
    {
       lte_close_serial_com(pstAtCmd->iComFd);
       return -1;
    }

    /*��com�ڶ�����*/
    iRet = lte_read_com_report_info(pstAtCmd->iComFd, pstAtCmd->acFileName,pcNeedle, pstAtCmd->iSecond);

    /*�ر�com��*/
    lte_close_serial_com(pstAtCmd->iComFd);

    /*�Զ������ж�*/
    if (iRet != 0)
    {        
       LTE_LOG(LTE_LOG_ERR,"read usb com fail");
       return -1;
    }

    /*�Զ������ļ����бȽϲ���*/
    iRet = lte_file_read_cmp(pstAtCmd->acFileName, pstAtCmd->acNeedle);    
    pstAtCmd->iResult = iRet;

    if (-2 == iRet)
    {
       LTE_LOG(LTE_LOG_ERR,"compare COM return string fail");
       return -1;
    }

    return 0;

}
