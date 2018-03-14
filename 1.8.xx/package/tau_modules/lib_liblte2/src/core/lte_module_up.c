#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include <errno.h>  
#include <sys/stat.h>   
#include <unistd.h>  
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <errno.h>

#include "lte.h"
#include "libusb.h"
#include "lte_log.h"
#include "lte_common.h"

#define BIG_TO_LITTLE(x) ((unsigned int)((((unsigned int)(x)&(unsigned int)0x000000ffU)<<24)|(((unsigned int)(x)&(unsigned int)0x0000ff00U)<<8)|(((unsigned int)(x)&(unsigned int)0x00ff0000U)>>8)|(((unsigned int)(x)&(unsigned int)0xff000000U)>>24)))
/*****************************************************************************
 �� �� ��  : lte_file_is_exists
 ��������  : �ж��ļ��Ƿ����
 �������  : char *pcFile
 �������  : ��
 �� �� ֵ  : static LTE_RET_E
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2014��12��12��������
    ��    ��   : liyoukun
    �޸�����   : �����ɺ���

*****************************************************************************/
static LTE_RET_E lte_file_is_exists(char *pcFile)
{
    if(NULL == pcFile)
    {
        LTE_LOG(LTE_LOG_ERR, "param is null pointer!");
        return LTE_FAIL;
    }
    
    if(0 != access(pcFile, F_OK))
    {
        LTE_LOG(LTE_LOG_ERR, "file [%S] is not exist!", pcFile);
        return LTE_FAIL;
    }
    else
    {
        return LTE_OK;
    }
}
/*****************************************************************************
 �� �� ��  : lte_file_size_get
 ��������  : ��ȡ�ļ���С
 �������  : char *pcFilePath         
             unsigned int *iFileSize  
 �������  : ��
 �� �� ֵ  : static LTE_RET_E
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2014��12��12��������
    ��    ��   : liyoukun
    �޸�����   : �����ɺ���

*****************************************************************************/
static LTE_RET_E lte_file_size_get(char *pcFilePath, unsigned int *iFileSize)
{
    struct stat stFileInfo = {0};

    if((NULL == iFileSize) || (NULL == pcFilePath))
    {
        LTE_LOG(LTE_LOG_ERR, "param is null pointer!");
        return LTE_FAIL;
    }

    if(0 != stat(pcFilePath, &stFileInfo))
    {
        LTE_LOG(LTE_LOG_ERR, "get file[%s] stat failed!", pcFilePath);
        return LTE_FAIL;
    }
    else
    {
        *iFileSize = stFileInfo.st_size;
    }
    
    return LTE_OK;
}


/*****************************************************************************
 �� �� ��  : lte_cifs_mount
 ��������  : ���������豸�����Ŀ��汾
 �������  : LTE_SERVER_USR_T *pstSmbServerInfo  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2014��12��12��������
    ��    ��   : liyoukun
    �޸�����   : �����ɺ���

*****************************************************************************/
LTE_RET_E lte_cifs_mount(LTE_SERVER_USR_T *pstSmbServerInfo)
{
    char acCmd[256] = {0};
    int  iRet = 0;
    if(NULL == pstSmbServerInfo)
    {
        return LTE_FAIL;
    }

    sprintf(acCmd, "mount -t cifs -o username=%s,password=%s,dir_mode=0755,file_mode=0755,nounix,noserverino //%s%s %s", 
                    pstSmbServerInfo->acUsrName, 
                    pstSmbServerInfo->acPassWD, 
                    pstSmbServerInfo->acServerAdd, 
                    pstSmbServerInfo->acServerPath, 
                    LTE_MODULE_VER_PATH);

    if(0 != access(LTE_MODULE_VER_PATH, F_OK))
    {
        LTE_LOG(LTE_LOG_NOTICE, LTE_MODULE_VER_PATH"is not exist");
        if(0 != mkdir(LTE_MODULE_VER_PATH, 0777))
        {
            LTE_LOG(LTE_LOG_ERR, LTE_MODULE_VER_PATH" creat failed");
            return LTE_FAIL;
        }
    }
    else
    {
        LTE_LOG(LTE_LOG_INFO, LTE_MODULE_VER_PATH" exist");
    }
    
    iRet = system_call(acCmd);
    if(LTE_OK != iRet)
    {
        return LTE_FAIL;
    }
    else
    {
        return LTE_OK;
    }
    
}
/*****************************************************************************
 �� �� ��  : lte_com_get
 ��������  : ��ȡ����ʹ�õ�com��
 �������  : char *acTtyUsb     ģ������ʹ�õ�com��
             int iLen           ��һ�����󳤶�
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2014��12��15������һ
    ��    ��   : liyoukun
    �޸�����   : �����ɺ���

*****************************************************************************/
LTE_RET_E lte_com_get(char *acTtyUsb, int iLen)
{
    int iRet = 0;
    LTE_PLATFORM_E enDeviceNum;
    LTE_DEVICE_INFO stDevInfo = {0};

    if(NULL == acTtyUsb)
    {
        LTE_LOG(LTE_LOG_ERR, "input param is null point!");
        return LTE_FAIL;
    }
    
    iRet = lte_device_info_get(&stDevInfo);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR, "Get module type failed!");
        return LTE_FAIL;
    }
    enDeviceNum = stDevInfo.enDeviceNum;
    switch(enDeviceNum)
    {
        case M_ZTE_MICRO:
			strncpy(acTtyUsb, "/dev/ttyUSB3", iLen);
			break;    
		default:
			LTE_LOG(LTE_LOG_ERR, "this moudle do not exit!");
			return LTE_FAIL;
    }
    return LTE_OK;
}
/*****************************************************************************
 �� �� ��  : lte_com_attribute_set
 ��������  : ����com������
 �������  : int iComFd        
             int iDeviceSpeed  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2014��12��16�����ڶ�
    ��    ��   : liyoukun
    �޸�����   : �����ɺ���

*****************************************************************************/
LTE_RET_E lte_com_attribute_set(int iComFd, int iDeviceSpeed)
{
    struct termios TtyAttr;
    int DeviceSpeed = B115200;
    int ByteBits = CS8;

    DeviceSpeed = iDeviceSpeed;
    
    memset(&TtyAttr, 0, sizeof(struct termios));
    TtyAttr.c_iflag = IGNPAR;
    TtyAttr.c_cflag = DeviceSpeed | HUPCL | ByteBits | CREAD | CLOCAL;
    TtyAttr.c_cc[VMIN] = 1;
    
    /* ��com������Ϊ������ģʽ */
    #if 0
    if (fcntl(iComFd, F_SETFL, O_NONBLOCK) < 0)
    {
        LTE_LOG(LTE_LOG_ERR, "update com fcntl failed\n"); 
        return LTE_FAIL;
    }
    #endif
    /* ����com�ڵĲ����ʵ����� */
    if (tcsetattr(iComFd, TCSANOW, &TtyAttr) < 0)
    {
        LTE_LOG(LTE_LOG_ERR, "update com attr set failed\n");
        return LTE_FAIL;
    }
    return LTE_OK;
}
/*****************************************************************************
 �� �� ��  : lte_up_serial_com_open
 ��������  : ��com��
 �������  : int *piComFd  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2014��12��16�����ڶ�
    ��    ��   : liyoukun
    �޸�����   : �����ɺ���

*****************************************************************************/
LTE_RET_E lte_up_serial_com_open(int *piComFd)
{
    char acCom[32] = {0};
    int iDeviceSpeed = B115200;
    int iRet = 0;
    int iComFd = 0; /* ��lte up com�ں��Ӧ��fd */
    
    if(NULL == piComFd)
    {
        LTE_LOG(LTE_LOG_ERR, "param [piComFd] is NULL\n");
        *piComFd = -1;
        return LTE_FAIL;
    }
    if(LTE_OK != lte_com_get(acCom, sizeof(acCom)))
    {
        LTE_LOG(LTE_LOG_ERR, "lte_com_get get lte up com failed!\n");
        *piComFd = -1;
        return LTE_FAIL;
    }
    
    /* �򿪴�����Ҫ�������O_NOCTTY�������κ����붼��Ӱ����� */
    if ((iComFd = open(acCom, O_RDWR | O_NOCTTY)) < 0) 
    {
        LTE_LOG(LTE_LOG_ERR, "open lte up com [%s] failed\n", acCom);
        *piComFd = -1;
        return LTE_FAIL;
    }

    /*����gps COM�ڵ�����*/
    if (LTE_FAIL == lte_com_attribute_set(iComFd, iDeviceSpeed))
    {
        *piComFd = -1;
        /* ����com�����ԣ�ʧ����ر�com�� */
        close(iComFd);
        return LTE_FAIL;
    }

    *piComFd = iComFd;
    LTE_LOG(LTE_LOG_INFO, "open lte up com [%s] success\n", acCom);

    return LTE_OK;
}
/*****************************************************************************
 �� �� ��  : gps_serial_com_close
 ��������  : �ر�com��
 �������  : int iComFd  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2014��12��16�����ڶ�
    ��    ��   : liyoukun
    �޸�����   : �����ɺ���

*****************************************************************************/
LTE_RET_E lte_up_serial_com_close(int iComFd)
{
    if(iComFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR, "lte up iComFd had been close!\n");
    }
    else
    {
        close(iComFd);
    }
    return 0;
}
LTE_RET_E lte_up_file_transfer(char *pcSrcFileName, char *pcDstFileName)
{
    int iCount = 0;
    int iFileSize = 0;
    int iRBufLen = 0;
    int iWBufLen = 0;
    int iSrcFd = 0;
    int iDstFd = 0;
    
    char acFilePath[64] = {0};
    char acDstFilePath[64] = {0};
    char acBuf[1024] = {0};
    char acBufTmp[1024] = {0};

    if((NULL == pcSrcFileName) || (NULL == pcDstFileName))
    {
        LTE_LOG(LTE_LOG_ERR, "param [pcSrcFileName pcDstFileName] is NULL\n");
        return LTE_FAIL;
    }
    
    sprintf(acFilePath, "%s/%s", LTE_MODULE_VER_PATH, pcSrcFileName);
    sprintf(acDstFilePath, "%s/%s", LTE_MODULE_VER_PATH, pcDstFileName);
    if(LTE_OK != lte_file_is_exists(acFilePath))
    {   
        LTE_LOG(LTE_LOG_ERR, "File[%s] does not exists!\n", acFilePath);
        return LTE_FAIL;
    }

    if(LTE_OK != lte_file_size_get(acFilePath, &iFileSize))
    {   
        LTE_LOG(LTE_LOG_ERR, "Get file [%s] size failed!\n", acFilePath);
        return LTE_FAIL;
    }

    iSrcFd = open(acFilePath, O_RDONLY);
    if(iSrcFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR, "Open file [%s] size failed!\n", acFilePath);
        return LTE_FAIL;
    }
    iDstFd = open(acDstFilePath, O_WRONLY|O_CREAT);
    if(iDstFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR, "Open file [%s] size failed!\n", acDstFilePath);
        return LTE_FAIL;
    }
    /* ���汾��Сд��Ŀ���ļ���4�ֽڡ� */
    iWBufLen = write(iDstFd, (char *)(&iFileSize), sizeof(int));
    if(iWBufLen < 0)
    {
        LTE_LOG(LTE_LOG_ERR, "write data to file [%s] failed!\n", acDstFilePath);
        close(iDstFd);
        close(iSrcFd);
        return LTE_FAIL;
    }
    while(1)
    {
        iRBufLen= read(iSrcFd, acBuf, sizeof(acBuf));
        if(iRBufLen < 0)
        {
            LTE_LOG(LTE_LOG_ERR, "read data from file [%s] failed!\n", acFilePath);
            continue;
        }
        if(0 == iRBufLen)
        {
            LTE_LOG(LTE_LOG_ERR, "read file [%s] over!\n", acFilePath);
            break;
        }
        iWBufLen = 0;
        while((iWBufLen += write(iDstFd, acBuf+iWBufLen, iRBufLen-iWBufLen)) != iRBufLen)
        {
            iCount++;
            if(iCount > 6)
            {
                LTE_LOG(LTE_LOG_ERR, "write data to file [%s] failed!\n", acDstFilePath);
                close(iDstFd);
                close(iSrcFd);
                return LTE_FAIL;
            }
        }
    }
    close(iDstFd);
    close(iSrcFd);
    return LTE_OK;
}
int get_write_data_result(int iComfd)
{
    int iRet = 0;
    int iRetval = 0;
	
    while( (iRet = read(iComfd, &iRetval, sizeof(iRetval)) ) <= 0 )
    {
        LTE_LOG(LTE_LOG_ERR, "Do not read data\n");
        usleep(100);
    }
    
    return iRetval;
}

LTE_RET_E lte_up_file_back_transfer(char *pcSrcFileName, char *pcDstFileName)
{
    int iCount = 0;
    int iFileSize = 0;
    int iRBufLen = 0;
    int iWBufLen = 0;
    int iSrcFd = 0;
    int iDstFd = 0;
    
    char acFilePath[64] = {0};
    char acDstFilePath[64] = {0};
    char acBuf[1024] = {0};
    char acBufTmp[1024] = {0};

    if((NULL == pcSrcFileName) || (NULL == pcDstFileName))
    {
        LTE_LOG(LTE_LOG_ERR, "param [pcSrcFileName pcDstFileName] is NULL\n");
        return LTE_FAIL;
    }
    
    sprintf(acFilePath, "%s/%s", LTE_MODULE_VER_PATH, pcSrcFileName);
    sprintf(acDstFilePath, "%s/%s", LTE_MODULE_VER_PATH, pcDstFileName);
    if(LTE_OK != lte_file_is_exists(acFilePath))
    {   
        LTE_LOG(LTE_LOG_ERR, "File[%s] does not exists!\n", acFilePath);
        return LTE_FAIL;
    }

    if(LTE_OK != lte_file_size_get(acFilePath, &iFileSize))
    {   
        LTE_LOG(LTE_LOG_ERR, "Get file [%s] size failed!\n", acFilePath);
        return LTE_FAIL;
    }

    iSrcFd = open(acFilePath, O_RDONLY);
    if(iSrcFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR, "Open file [%s] size failed!\n", acFilePath);
        return LTE_FAIL;
    }
    iDstFd = open(acDstFilePath, O_WRONLY|O_CREAT);
    if(iDstFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR, "Open file [%s] size failed!\n", acDstFilePath);
        return LTE_FAIL;
    }
    /* ���汾��Сд��Ŀ���ļ���4�ֽڡ� */
    iRBufLen = read(iSrcFd, (char *)(&iFileSize), sizeof(int));
    if(iRBufLen < 0)
    {
        LTE_LOG(LTE_LOG_ERR, "read file size [%s] failed!\n", acFilePath);
        close(iDstFd);
        close(iSrcFd);
        return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_ERR, "read file size [%d] success!\n", iFileSize);
    while(1)
    {
        iRBufLen= read(iSrcFd, acBuf, sizeof(acBuf));
        if(iRBufLen < 0)
        {
            LTE_LOG(LTE_LOG_ERR, "read data from file [%s] failed!\n", acFilePath);
            continue;
        }
        if(0 == iRBufLen)
        {
            LTE_LOG(LTE_LOG_ERR, "read file [%s] over!\n", acFilePath);
            break;
        }
        while((iWBufLen = write(iDstFd, acBuf, iRBufLen)) != iRBufLen)
        {
            iCount++;
            if(iCount > 6)
            {
                LTE_LOG(LTE_LOG_ERR, "write data to file [%s] failed!\n", acDstFilePath);
                close(iDstFd);
                close(iSrcFd);
                return LTE_FAIL;
            }
        }
    }
    close(iDstFd);
    close(iSrcFd);
    return LTE_OK;
}
LTE_RET_E lte_update(char *pcSrcFileName)
{
    unsigned int iFileSize = 0;
    unsigned int iFileSizeOld = 0;
    int iWriteSize = 0;
    int iRBufLen = 0;
    int iWBufLen = 0;
    int iCount = 0;
    int iSrcFd = 0;
    int iLoop = 0;
    int iRetval = 0;
    int iComFd = 0;

    unsigned char cBUf;
    char acFilePath[64] = {0};
    unsigned char acBuf[1024 * 4 - 1] = {0};
    
    if(NULL == pcSrcFileName)
    {
        LTE_LOG(LTE_LOG_ERR, "param [pcSrcFileName] is NULL\n");
        return LTE_FAIL;
    }
    /* �ж���û��·����û�еĻ���ʹ��Ĭ��·�� */
    if(NULL == strstr(pcSrcFileName, "/"))
    {
        sprintf(acFilePath, "%s/%s", LTE_MODULE_VER_PATH, pcSrcFileName);
    }
    else
    {
        strncpy(acFilePath, pcSrcFileName, sizeof(acFilePath));
    }
    
    /* �ж��ļ��Ƿ���ڣ�����ȡ�ļ���С */
    if(LTE_OK != lte_file_is_exists(acFilePath))
    {   
        LTE_LOG(LTE_LOG_ERR, "File[%s] does not exists!\n", acFilePath);
        return LTE_FAIL;
    }

    if(LTE_OK != lte_file_size_get(acFilePath, &iFileSize))
    {   
        LTE_LOG(LTE_LOG_ERR, "Get file [%s] size failed!\n", acFilePath);
        return LTE_FAIL;
    }
    /* ��ֻ��ģʽ�򿪰汾�ļ� */
    iSrcFd = open(acFilePath, O_RDONLY);
    if(iSrcFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR, "Open file [%s] size failed!\n", acFilePath);
        return LTE_FAIL;
    }
    /* ��ģ������ʹ�õ�ר��log�� */
    if(LTE_OK != lte_up_serial_com_open(&iComFd))
    {
        LTE_LOG(LTE_LOG_ERR, "Open serial com failed!\n");
        /* �ر��Ѵ򿪵��ļ� */
        close(iSrcFd);
        return LTE_FAIL;
    }
    /* ���汾��Сд��Ŀ���ļ���4�ֽڡ� */
    iFileSizeOld = iFileSize;
    iFileSize = BIG_TO_LITTLE(iFileSizeOld);
    LTE_LOG(LTE_LOG_ERR, "write version size %x success, write:%x!\n", iFileSizeOld, iFileSize);
    iWBufLen = write(iComFd, (char *)(&iFileSize), sizeof(int));
    if(iWBufLen < 0)
    {
        LTE_LOG(LTE_LOG_ERR, "write data to serial com failed, Comfd:%d!\n", iComFd);
        lte_up_serial_com_close(iComFd);
        close(iSrcFd);
        return LTE_FAIL;
    }
    iWriteSize += sizeof(int);
    LTE_LOG(LTE_LOG_ERR, "write version size %d success, write:%d!\n", iFileSizeOld, iWriteSize);
    if((iRetval = get_write_data_result(iComFd)) != 0)
    {
        LTE_LOG(LTE_LOG_ERR, "write data to com failed! errno:%d\n", errno);
        lte_up_serial_com_close(iComFd);
        close(iSrcFd);
        return LTE_FAIL;
    }
    /* ��ȡ�汾�ļ�������д������com���� */
    while(1)
    {
        iCount = 0;
        #if 0
        iRBufLen = read(iSrcFd, &cBUf, sizeof(unsigned char));
        if(iRBufLen < 0)
        {
            LTE_LOG(LTE_LOG_ERR, "read data from file [%s] failed!\n", acFilePath);
            continue;
        }
        if(0 == iRBufLen)
        {
            LTE_LOG(LTE_LOG_ERR, "read file [%s] over!\n", acFilePath);
            break;
        }
        iLoop++;
        LTE_LOG(LTE_LOG_ERR, "write file len [%d][%c]!\n", iLoop, cBUf);
        while((iWBufLen = write(iComFd, &cBUf, iRBufLen)) != iRBufLen)
        {
            LTE_LOG(LTE_LOG_ERR, "write data to com failed! iWBufLen %d iRBufLen %d errno,%d\n", iWBufLen, iRBufLen, errno);
            iCount++;
            if(iCount > 6)
            {
                LTE_LOG(LTE_LOG_ERR, "write data to com failed! errno:%d\n", errno);
                lte_up_serial_com_close(iComFd);
                close(iSrcFd);
                return LTE_FAIL;
            }
        }
        sleep(1);
        #else
        iRBufLen= read(iSrcFd, acBuf, sizeof(acBuf));
        if(iRBufLen < 0)
        {
            LTE_LOG(LTE_LOG_ERR, "read data from file [%s] failed!\n", acFilePath);
            continue;
        }
        if(0 == iRBufLen)
        {
            LTE_LOG(LTE_LOG_ERR, "read file [%s] over!\n", acFilePath);
            break;
        }
        iWBufLen = 0;
        while((iWBufLen += write(iComFd, acBuf+iWBufLen, iRBufLen-iWBufLen)) != iRBufLen)
        {
            LTE_LOG(LTE_LOG_ERR, "write data to com failed! iWBufLen %d iRBufLen %d errno,%d\n", iWBufLen, iRBufLen, errno);
            iCount++;
            if(iCount > 6)
            {
                LTE_LOG(LTE_LOG_ERR, "write data to com failed! errno:%d\n", errno);
                lte_up_serial_com_close(iComFd);
                close(iSrcFd);
                return LTE_FAIL;
            }
            //usleep(10000);
        }
        iWriteSize += iWBufLen;
        LTE_LOG(LTE_LOG_ERR, "write file size [%d], written [%d]\n", iFileSizeOld, iWriteSize);
        if((iRetval = get_write_data_result(iComFd)) != 0)
        {
            LTE_LOG(LTE_LOG_ERR, "write data to com failed! errno:%d\n", errno);
            lte_up_serial_com_close(iComFd);
            close(iSrcFd);
            return LTE_FAIL;
        }
        memset(acBuf, 0, sizeof(acBuf));
        //usleep(10000);
        #endif
    }
    
    lte_up_serial_com_close(iComFd);
    close(iSrcFd);
    return LTE_OK;
}



