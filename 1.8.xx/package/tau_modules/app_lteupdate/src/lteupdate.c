/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : lteupdate.c
  版 本 号   : 初稿
  作    者   : xiao wei
  生成日期   : 2016年6月28日 星期二
  最近修改   :
  功能描述   :
  函数列表   :
  修改历史   :
  1.日    期   : 2016年6月28日 星期二
    作    者   : xiao wei
    修改内容   : 创建文件

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>  
#include <unistd.h>
#include <sys/types.h>
#include <syslog.h>
#include <sys/sysinfo.h>

#include <sys/select.h>
#include <termios.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/stat.h>   
#include <unistd.h>    
#include <signal.h>
#include <sys/ioctl.h>

#include "ap_types.h"
#include "lte.h"

#define FILE_CRC_CHECK 		 			0
#define FILE_HDLC_SEND 		 			1
#define FILE_UNIT_ALIGN_TYPE 			4
#define FILE_UNIT_ALIGN_TYPE 			4
#define FILE_UNIT_BLOCK_SIZE 			4096

#define HDLC_NO_DATA_CMD_BIT			5
#define HDLC_WRITE_INIT_CMD_BIT			11
#define HDLC_WRITE_DATA_CMD_BIT			6
#define HDLC_WRITE_END_CMD_BIT			22

#define LTE_AT_CMD_NO_READ				0
#define LTE_AT_CMD_READ_RET 			1
#define LTEUPDATE_RUN_MIN_MEM 			30
#define LTE_UPDATE_FILE_PATH 			"/tmp/lteupdate.bin"//"/mnt/emmc/lteupdate.bin"
#define LTE_DEV_ACMCOM					"/dev/ttyUSB2"
#define LTE_UPDATE_WEBLOG_INFO			"/tmp/lteupdate.info"
#define LTE_COM_READ_RETURN				"/tmp/atcomret"
#define WEBUPDATE_ERROR					"ERROR  :"
#define WEBUPDATE_INFO					"INFOS  :"
#define WEBUPDATE_SUCCES				"SUCCESS:"

#define LTE_COM_READ_TIME				25*1000L
#define LTE_COM_READ_DELAY_TIME			1000*1000L

//#define FREE(p) {free(p), (p)=NULL;}
#define FREE_POINTER(p) if(NULL!=p) { \
                        free(p); \
                        p = NULL; \
                    }
#define CHECK_POINTER(p) if(NULL!=p) { \
                        p = NULL; \
                    }     

static int iLteReadDelay;
                   
typedef unsigned char BYTE;		
typedef unsigned short WORD;	//u
typedef unsigned long DWORD;	//lu/lx

typedef enum {
	HDLC_CMD_ACK 	= 0x02,			//命令正常响应
	HDLC_CMD_NAK 	= 0x03,			//命令非法响应
	HDLC_CMD_RESET 	= 0x0a,			//复位命令
	HDLC_CMD_VERREQ = 0x0c,			//软件信息请求
	HDLC_CMD_VERRSP = 0x0d,			//软件信息返回
	HDLC_CMD_DLOAD_INIT = 0x41,		//一次下载初始化命令
	HDLC_CMD_DATA_PACKET= 0x42,		//下载数据包传输命令
	HDLC_CMD_DLOAD_END 	= 0x43,		//结束一次下载命令
	HDLC_CMD_VERIFY 	= 0x45,		//产品ID查询与返回命令
	HDLC_CMD_TCPU_VER 	= 0x46		//单板软件版本查询与返回命令
}HDLC_DATA_CMD_E;
    
#pragma pack(1)
typedef struct {//sAppFileHeader
    DWORD Reserved1;			//保留以兼容早期的下载bin文件
    DWORD Reserved2;            //保留以兼容早期的下载bin文件
    BYTE  szFileID[50];         //标识文件类型的特征码       
    BYTE  szDispName[34];       //显示名字                   
}FILE_APP_FILE_HEADER_S;
typedef struct {//sAppUnitHeader
    DWORD dwMagicNum;			//取0xA5 5A AA 55                                             
    DWORD dwHeaderLen;   		//本文件头长度,从dwMagicNum开始的长度,包括CRC                 
    DWORD dwHeaderVersion;		//针对本结构填1,以后升级时增加                                
    BYTE  UnlockCode[8];		//区分待下载数据的code,8字节的任意数据,不同平台使用不同的值   
    DWORD image_type;			//本段文件的类型                                              
    DWORD dwDataLen;			//本段数据的实际有效长度,不含填充字节                         
    BYTE  szDate[16];			//字符串表示                                                  
    BYTE  szTime[16];			//字符串表示                                                  
    BYTE  szDispName[32];		//本段数据包括下载的版本号,产品名称,软件版本号                
    WORD  wHeadCRC;				//从dwMagicNum校验到dwBlockSize                               
    DWORD dwBlockSize;			//帧大小,目前各产品均为4096,每一dwBlockSize数据计算一次CRC校验
}FILE_APP_UNIT_HEADER_S;

typedef struct 
{
    char acCmd[5120];    /* 输入com口的命令 */
    UINT8 acFileName[32];   /* 需要比较的文件 */
    UINT8 acNeedle[32]; /* 比较字段 */
    INT32 iResult;    /* 输出比较的结果 */
    UINT8 acCom[32];    /* 操作的com口 */
    INT32 iComFd;       /*com fd 号*/
    INT32 iSecond;      /*读写com时长*/
}LTE_NORMAL_AT_CMD_T;
                         
INT32 lte_set_com(INT32 iComFd) 
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
		syslog(LOG_ERR,"[%s:%d] lte open set com port tcsetattr error !\n",__func__,__LINE__);
	    return -1;
	}
	return 0;
}
INT32 lte_open_serial_com(INT32 *piComFd,CHAR *pcCom)
{
    INT32 iSetAcm = 0;	
    INT32 iComFd = 0;
    
    if (NULL == piComFd || NULL == pcCom)
    {
    	printf("[%s:%d] lte can't open serial com NULL == piComFd NULL == pcCom\n",__func__,__LINE__);
    	syslog(LOG_ERR,"[%s:%d] lte open serial com pcCom=NULL !\n",__func__,__LINE__);
        return -1;
    }
    //打开串口需要添加属性O_NOCTTY，否则任何输入都会影响进程
    if ((iComFd = open((char*)pcCom,O_RDWR | O_NOCTTY)) < 0) 
    {
    	printf("[%s:%d] lte can't open serial com iComFd<0 pcCom=%s\n",__func__,__LINE__,pcCom);
    	syslog(LOG_ERR,"[%s:%d] lte can't open serial com iComFd<0 pcCom=%s\n",__func__,__LINE__,pcCom);
        return -1;
    }
    /*将com口设置为非阻塞模式*/
    if (fcntl(iComFd, F_SETFL, O_NONBLOCK) < 0)
    {
    	syslog(LOG_ERR,"[%s:%d] lte open unable set to NONBLOCK mod\n",__func__,__LINE__);
    }
    /*设置COM波特率等其他属性*/
    if ((iSetAcm= lte_set_com(iComFd)) < 0)
    {
    	printf("[%s:%d] lte can't open serial com lte_set_com error\n",__func__,__LINE__);
    	syslog(LOG_ERR,"[%s:%d] lte open set com parameter error !\n",__func__,__LINE__);
        return -2;
    }
    *piComFd = iComFd;
    
    return 0;
}
INT32 lte_read_com(int iComfd,char *pcFilePath, char *pcComReadStr)
{
	int iFd = 0;
	int iNwrite = 0;
	unsigned char cChar[32] = {0};
	fd_set ReadSetFD;
	int iRet = 0;
	struct timeval stTimeout;
  
	if(NULL==pcComReadStr || NULL==pcFilePath)
	{
		syslog(LOG_ERR,"[%s:%d] lte read serial com pcFilePath=NULL \n",__func__,__LINE__);
		return -1;
	}
	
	/*阻塞的等待*/
	//stTimeout.tv_sec = 1;
	stTimeout.tv_sec = 0;
	/*联芯的采用周期为25ms,因此com口的采样周期为20ms*/
	//stTimeout.tv_usec=0L;//1s=1000ms=1000*1000us
	stTimeout.tv_usec=iLteReadDelay;
	/*打开将要书写的文件*/
	iFd = open(pcFilePath,O_CREAT|O_APPEND|O_WRONLY);
	if (iFd < 0)
	{
		syslog(LOG_ERR,"[%s:%d] lte read open tmp file failed \n",__func__,__LINE__);
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
				iRet = read(iComfd, cChar, 32);
		  		if (iRet < 1)
		  		{
		  			break;
	            }
	            /*向文件中书写 从com口中读取到的数据*/
	            iNwrite = write(iFd,cChar,32);
	            if (iNwrite < 0)
	            {
	            	syslog(LOG_ERR,"[%s:%d] lte read write data file failed \n",__func__,__LINE__);
	                close(iFd);
	                return -1;
	            }
	            else{
	            	printf("[%s:%d] read :%s\n",__func__,__LINE__,cChar);
	            	memcpy(pcComReadStr,cChar,32);
	            }
	        }
	    } 
	}
  
  return 0;
}
INT32 lte_write_com(INT32 iComFd,char *pcCmd, int iCmdLeng)
{
    INT32 iRet =0;
    INT32 iLength = iCmdLeng;

    /*向com口写入at 指令*/
	iRet = write(iComFd,pcCmd,iLength);
    if (iRet != iLength)
    {
    	printf("write iRet=%d\n",iRet);
    	syslog(LOG_ERR,"[%s:%d] lte write serial com ATCMD failed \n",__func__,__LINE__);
        return -1;
    }
    	
    return 0;
}
INT32 lte_normal_at_cmd(char *pcAtCmd, int iCmdLeng, int iReadRet, char *pcComReadStr)
{
    INT32 iRet   = 0;
	
	if(NULL == pcAtCmd || NULL == pcComReadStr)
	{
		syslog(LOG_ERR,"[%s:%d] ATCMD: struct =NULL !\n",__func__,__LINE__);
		return -1;
	}
		
	LTE_NORMAL_AT_CMD_T stAtCmd = {{0}};
	memcpy(stAtCmd.acCmd,pcAtCmd,iCmdLeng);
	snprintf((char*)&stAtCmd.acFileName,sizeof(stAtCmd.acFileName),"%s",LTE_COM_READ_RETURN);
	snprintf((char*)&stAtCmd.acNeedle,sizeof(stAtCmd.acNeedle),"%s","OK");
	snprintf((char*)&stAtCmd.acCom,sizeof(stAtCmd.acCom),"%s",LTE_DEV_ACMCOM);
	snprintf((char*)&stAtCmd.iSecond,sizeof(stAtCmd.iSecond),"%d",3);
		
    iRet = lte_open_serial_com(&(stAtCmd.iComFd),(CHAR*)stAtCmd.acCom);
    if (0 == iRet)
    {
    	syslog(LOG_ERR,"[%s:%d] lte open serial com failed iRet=0 \n",__func__,__LINE__);
    	//return -1;
    }
    else if (-2 == iRet)
    {
		/*打开了com口,需要关闭*/
		close(stAtCmd.iComFd);
		printf("[%s:%d] lte open serial com failed iRet=-2 \n",__func__,__LINE__);
		//PAL_LOG(PAL_LOG_ERR,"\n exit from here:%d,%s\n",__LINE__,__func__);
		syslog(LOG_ERR,"[%s:%d] lte serial com close iRet=-2\n",__func__,__LINE__);
		return -1;
    }
    else
    {
    	printf("[%s:%d] lte open serial com failed iRet= else \n",__func__,__LINE__);
    	syslog(LOG_ERR,"[%s:%d] lte open serial com failed iRet= else ...\n",__func__,__LINE__);
        return -1;    
    }

    /*向com口写入at 指令*/
    iRet = lte_write_com(stAtCmd.iComFd,pcAtCmd,iCmdLeng);
    if (iRet < 0)
    {
        close(stAtCmd.iComFd);
    	syslog(LOG_ERR,"[%s:%d] lte serial com write error !\n",__func__,__LINE__);
        return -1;
    }
    
	if(LTE_AT_CMD_READ_RET == iReadRet)
	{
    	/*向com口读数据*/
    	iRet = lte_read_com(stAtCmd.iComFd, (char*)stAtCmd.acFileName, pcComReadStr);
    	if(!strcmp(pcComReadStr,""))
    	{
    		int iLoop = 0;
    		for(iLoop=0; iLoop<200; iLoop++)
    		{
    			printf("[%s:%d] ~~~~~~~~~~~~~~~read time :%d !\n",__func__,__LINE__,iLoop);
    			lte_read_com(stAtCmd.iComFd, (char*)stAtCmd.acFileName, pcComReadStr);
    			if(strcmp(pcComReadStr,""))
    			{
    				break;
    			}
    		}
    	}
	}
    /*关闭com口*/
    close(stAtCmd.iComFd);
	
    return 0;
}
static int lte_webupdate_info(char *pcUpdateInfo)
{
	FILE *fp_log;
	char acWeblogInfo[64] = {0};
	int iLength = strlen(pcUpdateInfo);
	
	if(NULL == pcUpdateInfo )
	{
		syslog(LOG_ERR,"[%s:%d] pcUpdateInfo or fp_log=NULL \n",__func__,__LINE__);
		return -1;
	}
	if(64 < iLength)
	{
		iLength=64;
	}
	
	fp_log=fopen(LTE_UPDATE_WEBLOG_INFO,"a+");
    if(NULL==fp_log)
    {
    	printf("[%s:%d] lteupdate fopen tmp loginfo failed ! \n",__func__,__LINE__);
        syslog(LOG_ERR,"[%s:%d] lteupdate fopen tmp loginfo failed  ! \n",__func__,__LINE__);
        return -1;
    }
	
	snprintf(acWeblogInfo,iLength,"%s",pcUpdateInfo);
	//fseek(fp_log,0,SEEK_SET);
	//printf("[%s:%d] acWeblogInfo=%s \n",__func__,__LINE__,acWeblogInfo);
	fwrite(acWeblogInfo,iLength,1,fp_log);
	fprintf(fp_log,"\n");
	fclose(fp_log);	
	
	return 0;
}
static unsigned int ReverseBytes_16(unsigned int value)
{
	return (unsigned int)((value & 0xFFU) << 8 | (value & 0xFF00U) >> 8);
}
static unsigned long ReverseBytes(unsigned long value)
{
	return (value & 0x000000FFU) << 24 | (value & 0x0000FF00U) << 8 | (value & 0x00FF0000U) >> 8 | (value & 0xFF000000U) >> 24;
}
static unsigned short crc16(unsigned char *data, int length)  
{  
	unsigned char i;  
	unsigned short crc = 0xffff;
    while(length--)  
    {  
		crc ^= *data++;
		for (i = 0; i < 8; ++i)  
		{  
			if (crc & 1)  
				crc = (crc >> 1) ^ 0x8408;
			else  
				crc = (crc >> 1);  
		}  
	}
	return ~crc;
}
void hdlc_read_data_escape(char *pcHdlcReadData, int iHdlcDataLength, char *pcHdlcRet)
{
	int iDataLengHead = 0;
	int iDataLengTail = 0;
	int iLoop = 0;
	
	if(0x7e != (unsigned char)pcHdlcReadData[0])
	{
		printf("[%s:%d] header error !\n",__func__,__LINE__);
		syslog(LOG_ERR,"[%s:%d] header cmd error ! \n",__func__,__LINE__);
	}
	
	for(iLoop=0; iLoop < iHdlcDataLength; iLoop++)
	{
		if(0x7e == (unsigned char)pcHdlcReadData[iLoop] )
		{
			if( (HDLC_CMD_VERIFY == (unsigned char)pcHdlcReadData[iLoop+1] || \
				HDLC_CMD_TCPU_VER == (unsigned char)pcHdlcReadData[iLoop+1]) && 0 == iLoop )
			{
				//因获取模块内的版本号之前会增肌一个0x18的字节乱码,故去除
				if(HDLC_CMD_TCPU_VER == (unsigned char)pcHdlcReadData[iLoop+1])
				{
					iDataLengHead = iLoop+1+2;
				}else{
					iDataLengHead = iLoop+1+1;
				}
			}
			if( (HDLC_CMD_ACK == (unsigned char)pcHdlcReadData[iLoop+1]  || \
				HDLC_CMD_NAK == (unsigned char)pcHdlcReadData[iLoop+1] ) && 0 == iLoop )
			{
				//memset(pcHdlcRet,HDLC_CMD_ACK,sizeof(char));
				pcHdlcRet[0] = HDLC_CMD_ACK;
				break;
			}
			if(iDataLengHead < iLoop)
			{
				iDataLengTail = iLoop - 2;
				pcHdlcReadData = pcHdlcReadData + iDataLengHead;
				memcpy(pcHdlcRet,pcHdlcReadData,iDataLengTail-iDataLengHead);
				break;
			}
		}
	}

	return ;
}
int hdlc_write_no_data_cmd(char iDataFrameCmd, char *pcComReadStr)
{
	char iDataFrameHead = 0x7e;
	char iDataFrameTail = 0x7e;
	WORD usSendDataCrc = 0;
	char acDataSend[5] = {0};
	int iRet = 0;
	
	if(NULL == pcComReadStr)
	{
		printf("[%s:%d] pcComReadStr=NULL \n",__func__,__LINE__);
		syslog(LOG_ERR,"[%s:%d] pcComReadStr=NULL \n",__func__,__LINE__);
	}

	usSendDataCrc = (WORD)ReverseBytes_16(crc16((unsigned char *)&iDataFrameCmd,1));

	acDataSend[0]=iDataFrameHead;
	acDataSend[1]=iDataFrameCmd;
	memcpy(&acDataSend[2],&usSendDataCrc,2);
	acDataSend[4]=iDataFrameTail;

	iRet=lte_normal_at_cmd(acDataSend,HDLC_NO_DATA_CMD_BIT,LTE_AT_CMD_READ_RET,pcComReadStr);
	if(0!=iRet)
		return -1;
	
	return 0;
}
int hdlc_write_data_config(FILE_APP_UNIT_HEADER_S *pstFileUnitHead, char iDataFrameCmd, char *pcComReadStr)
{
	char iDataFrameHead = 0x7e;
	char iDataFrameTail = 0x7e;
	int iRet = 0;
	if(NULL == pcComReadStr)
	{
		printf("[%s:%d] pcComReadStr=NULL \n",__func__,__LINE__);
		syslog(LOG_ERR,"[%s:%d] pstFileUnitHead=NULL \n",__func__,__LINE__);
	}
	
	WORD usSendDataCrc = 0;
	char acDataSend[27] = {0};
	
	DWORD image_type = 0;
	DWORD dwDataLen = 0;
	image_type = ReverseBytes(pstFileUnitHead->image_type);
	dwDataLen = ReverseBytes(pstFileUnitHead->dwDataLen);
	
	if(HDLC_CMD_DLOAD_INIT == (int)iDataFrameCmd )
	{
		//5+11
		char acDataSendInit[1+HDLC_WRITE_INIT_CMD_BIT] = {0};
		
		memset(acDataSendInit,iDataFrameCmd,sizeof(char));
		memcpy(&acDataSendInit[1],&image_type,sizeof(DWORD));
		memcpy(&acDataSendInit[5],&dwDataLen,sizeof(DWORD));
		usSendDataCrc = (WORD)ReverseBytes_16(crc16((unsigned char *)acDataSendInit,1+HDLC_WRITE_INIT_CMD_BIT));
		
		acDataSend[0]=iDataFrameHead;
		acDataSend[1]=iDataFrameCmd;
		memcpy(&acDataSend[2],&image_type,sizeof(DWORD));
		memcpy(&acDataSend[6],&dwDataLen,sizeof(DWORD));
		memcpy(&acDataSend[13],&usSendDataCrc,sizeof(WORD));
		memset(&acDataSend[15],iDataFrameTail,sizeof(char));
		
		iRet=lte_normal_at_cmd(acDataSend,HDLC_NO_DATA_CMD_BIT+HDLC_WRITE_INIT_CMD_BIT,LTE_AT_CMD_READ_RET,pcComReadStr);
		if(0!=iRet)
			return -1;
	}else if(HDLC_CMD_DLOAD_END == (int)iDataFrameCmd )
	{	
		//5+22
		char acDataSendEnd[1+HDLC_WRITE_END_CMD_BIT] = {0};
		
		memset(acDataSendEnd,iDataFrameCmd,sizeof(char));
		memcpy(&acDataSendEnd[1],&dwDataLen,sizeof(DWORD));
		memcpy(&acDataSendEnd[7],&image_type,sizeof(DWORD));
		usSendDataCrc = (WORD)ReverseBytes_16(crc16((unsigned char *)acDataSendEnd,1+HDLC_WRITE_END_CMD_BIT));
		
		acDataSend[0]=iDataFrameHead;
		acDataSend[1]=iDataFrameCmd;
		memcpy(&acDataSend[2],&dwDataLen,sizeof(DWORD));
		memcpy(&acDataSend[8],&image_type,sizeof(DWORD));
		memcpy(&acDataSend[24],&usSendDataCrc,sizeof(WORD));
		memset(&acDataSend[26],iDataFrameTail,sizeof(char));
		
		iRet=lte_normal_at_cmd(acDataSend,HDLC_NO_DATA_CMD_BIT+HDLC_WRITE_END_CMD_BIT,LTE_AT_CMD_READ_RET,pcComReadStr);
		if(0!=iRet)
			return -1;
	}
	
	return 0;
}
int hdlc_ver_check(FILE_APP_UNIT_HEADER_S *pstFileUnitHead)
{
	char acFileVerId[16] = {0};
	char acFileVerNum[32] = {0};
	char acComReadInfo[32] = {0};
	char acComEscaInfo[32] = {0};
	char acComVerId[16] = {0};
	char acComVerNum[32] = {0};
	char acComBuff[16] = {0};
	int iRet=0;
	if(NULL == pstFileUnitHead)
	{
		printf("[%s:%d] pstFileUnitHead = NULL \n",__func__,__LINE__);
		syslog(LOG_ERR,"[%s:%d] pstFileUnitHead=NULL \n",__func__,__LINE__);
		return -1;
	}
	
	sscanf((char*)pstFileUnitHead->szDispName,":%[^:]:%s",acFileVerId,acFileVerNum);
	printf("[%s:%d] acFileVerId=%s acFileVerNum=%s\n",__func__,__LINE__,acFileVerId,acFileVerNum);
	
	//compare file version id and lte module version id
	iRet=hdlc_write_no_data_cmd(HDLC_CMD_VERIFY,acComReadInfo);
	if(0!=iRet)
		return -1;
	hdlc_read_data_escape(acComReadInfo,32,acComEscaInfo);
	sscanf(acComEscaInfo,"%s %s",acComBuff,acComVerId);
	printf("[%s:%d] acComVerId=%s \n",__func__,__LINE__,acComVerId);
	if( strcmp(acFileVerId,acComVerId) )
	{
		printf("[%s:%d] curr bin file version id error ! \n",__func__,__LINE__);
		syslog(LOG_ERR,"[%s:%d] curr bin file version id error ! \n",__func__,__LINE__);
		return -1;
	}
	syslog(LOG_ERR,"[%s:%d] curr bin file version id =%s ! \n",__func__,__LINE__,acFileVerId);
	
	//compare file version num and lte module version num
	memset(acComReadInfo,0,32);
	memset(acComEscaInfo,0,32);
	iRet=hdlc_write_no_data_cmd(HDLC_CMD_TCPU_VER,acComReadInfo);
	if(0!=iRet)
		return -1;
	hdlc_read_data_escape(acComReadInfo,32,acComEscaInfo);
	sscanf(acComEscaInfo,"%s",acComVerNum);
	
	printf("[%s:%d] acComVerNum=%s \n",__func__,__LINE__,acComVerNum);
	if( !strcmp(acFileVerNum,acComVerNum) )
	{
		printf("[%s:%d] the same as the current bin file version number ! \n",__func__,__LINE__);
		syslog(LOG_NOTICE,"[%s:%d] the same as the current bin file version number ! \n",__func__,__LINE__);
	}
	syslog(LOG_ERR,"[%s:%d] curr bin file version number =%s ! \n",__func__,__LINE__,acFileVerNum);
	
	return 0;
}
int hdlc_init(FILE_APP_UNIT_HEADER_S *pstFileUnitHead)
{
	printf("[%s:%d] hdlc_init\n",__func__,__LINE__);

	char acComReadInfo[32] = {0};
	char acComEscaInfo[32] = {0};
	int iRet=0;
	
	if(NULL == pstFileUnitHead)
	{
		printf("[%s:%d] pstFileUnitHead = NULL \n",__func__,__LINE__);
		syslog(LOG_ERR,"[%s:%d] pstFileUnitHead=NULL \n",__func__,__LINE__);
		return -1;
	}
	
	//hdlc send data init cmd
	memset(acComReadInfo,0,32);
	iRet=hdlc_write_data_config(pstFileUnitHead,HDLC_CMD_DLOAD_INIT,acComReadInfo);
	if(0!=iRet)
		return -1;
	hdlc_read_data_escape(acComReadInfo,32,acComEscaInfo);
	printf("[%s:%d] acComEscaInfo=0x%0x \n",__func__,__LINE__,acComEscaInfo[0]);
	if(HDLC_CMD_ACK != acComEscaInfo[0])
	{
		printf("[%s:%d] hdlc init return failed \n",__func__,__LINE__);
		syslog(LOG_ERR,"[%s:%d]  hdlc init return failed ! \n",__func__,__LINE__);
		return -1;
	}
	
	return 0;
}
int hdlc_end(FILE_APP_UNIT_HEADER_S *pstFileUnitHead)
{
	printf("[%s:%d] hdlc_end \n",__func__,__LINE__);
	char acComReadInfo[32] = {0};
	char acComEscaInfo[32] = {0};
	if(NULL == pstFileUnitHead)
	{
		printf("[%s:%d] pstFileUnitHead = NULL \n",__func__,__LINE__);
		syslog(LOG_ERR,"[%s:%d] pstFileUnitHead=NULL \n",__func__,__LINE__);
		return -1;
	}
	
	hdlc_write_data_config(pstFileUnitHead,HDLC_CMD_DLOAD_END,acComReadInfo);
	hdlc_read_data_escape(acComReadInfo,32,acComEscaInfo);
	printf("[%s:%d] acComEscaInfo=0x%0x \n",__func__,__LINE__,acComEscaInfo[0]);
	if(HDLC_CMD_ACK != acComEscaInfo[0])
	{
		printf("[%s:%d] hdlc init return failed \n",__func__,__LINE__);
		syslog(LOG_ERR,"[%s:%d]  hdlc init return failed ! \n",__func__,__LINE__);
		return -1;
	}
	
	return 0;
}
int hdlc_send(char *pucSendData, WORD iSendDataSize, int iSendDataCrcNum)
{
	printf("[%s:%d] hdlc_send \n",__func__,__LINE__);

	char iDataFrameCmd = HDLC_CMD_DATA_PACKET;
	char acComReadInfo[32] = {0};
	char acSendDataBuff[4096*2] = {0};
	char cHdlcDataInsBit = 0x7d;
	char cHdlcDataEscBit7d = 0x7d^0x20;
	char cHdlcDataEscBit7e = 0x7e^0x20;
	char acSendDataCrcBuff[5120] = {0};
	
	if(NULL == pucSendData)
	{
		printf("[%s:%d] pucSendData=NULL Error !\n",__func__,__LINE__);
		syslog(LOG_ERR,"[%s:%d] pucSendData=NULL \n",__func__,__LINE__);
		return -1;
	}
	
	WORD usSendDataCrc = 0;
	int iSendDataPackLeng = 0;
	int iLoop = 0;
	int iRet=0;
	//0x7e + 0x42 + add bit + data + crc + 0x7e
	iSendDataPackLeng = 1+1+HDLC_WRITE_DATA_CMD_BIT+iSendDataSize+2+1;

	printf("[%s:%d] iSendDataPackLeng=%d\n",__func__,__LINE__,iSendDataPackLeng);
	
	acSendDataBuff[0]=0x7e;
	acSendDataBuff[1]=iDataFrameCmd;
	memcpy(&acSendDataBuff[2],&iSendDataCrcNum,sizeof(int));
	memcpy(&acSendDataBuff[6],&iSendDataSize,sizeof(WORD));
	memcpy(&acSendDataBuff[8],pucSendData,sizeof(char)*iSendDataSize);
	
	//1+HDLC_WRITE_DATA_CMD_BIT+iSendDataSize+2+1 : 0x7e + cmd + cmd bit + data + crc + 0x7e
	usSendDataCrc = (WORD)ReverseBytes_16(crc16((unsigned char*)(acSendDataBuff+1),1+HDLC_WRITE_DATA_CMD_BIT+iSendDataSize));
	//printf("[%s:%d] usSendDataCrc=0x%04x\n",__func__,__LINE__,usSendDataCrc);
	
	acSendDataCrcBuff[0]=0x7e;
	acSendDataCrcBuff[1]=iDataFrameCmd;
	memcpy(&acSendDataCrcBuff[2],&iSendDataCrcNum,sizeof(int));
	memcpy(&acSendDataCrcBuff[6],&iSendDataSize,sizeof(WORD));
	memcpy(&acSendDataCrcBuff[8],pucSendData,sizeof(char)*iSendDataSize);
	memcpy(&acSendDataCrcBuff[8+iSendDataSize],&usSendDataCrc,sizeof(WORD));
	
	int iSendDataFirNum = 0;
	int iSendDataSecNum = 0;
	int iSendDataSizeAll = 0;
	//iSendDataSize +2 add crc check WORD two bytes
	for(iLoop = 0; iLoop < 8+iSendDataSize +2; iLoop++)
	{
		if(((0x7e == (unsigned char)acSendDataCrcBuff[iLoop])&&0!=iLoop&&(8+iSendDataSize +2)!=iLoop) || \
			((0x7d == (unsigned char)acSendDataCrcBuff[iLoop])&&0!=iLoop&&(8+iSendDataSize +2!=iLoop)) )
		{
			if(0==iSendDataFirNum)
			{
				memcpy(&acSendDataBuff[0],acSendDataCrcBuff,iLoop);
				acSendDataBuff[0+iLoop]=cHdlcDataInsBit;
				if(0x7d==(unsigned char)acSendDataCrcBuff[iLoop])
				{
					acSendDataBuff[0+iLoop+1]=cHdlcDataEscBit7d;
				}else{
					acSendDataBuff[0+iLoop+1]=cHdlcDataEscBit7e;
				}
				iSendDataFirNum = 0+iLoop+1+1;
			}else{
				memcpy(&acSendDataBuff[iSendDataFirNum],acSendDataCrcBuff+iSendDataSecNum,iLoop-iSendDataSecNum+1);
				acSendDataBuff[iSendDataFirNum+(iLoop-iSendDataSecNum)]=cHdlcDataInsBit;
				if(0x7d==(unsigned char)acSendDataCrcBuff[iLoop])
				{
					acSendDataBuff[iSendDataFirNum+(iLoop-iSendDataSecNum)+1]=cHdlcDataEscBit7d;
				}else{
					memset(&acSendDataBuff[iSendDataFirNum+(iLoop-iSendDataSecNum)+1],cHdlcDataEscBit7e,sizeof(char));
					acSendDataBuff[iSendDataFirNum+(iLoop-iSendDataSecNum)+1]=cHdlcDataEscBit7e;
				}
				iSendDataFirNum = iSendDataFirNum+(iLoop-iSendDataSecNum+1)+1;
			}
			
			iSendDataSecNum = iLoop +1;
			iSendDataSizeAll++;
		}
		if(iLoop == 8+iSendDataSize +2-1)
		{
			if(0==iSendDataFirNum)
			{
				memcpy(&acSendDataBuff[0],acSendDataCrcBuff,iLoop+1);
			}else{
				memcpy(&acSendDataBuff[iSendDataFirNum],acSendDataCrcBuff+iSendDataSecNum,iLoop-iSendDataSecNum+1);
			}
			break;
		}
	}
	acSendDataBuff[iSendDataPackLeng + iSendDataSizeAll-1]=0x7e;
	printf("[%s:%d] iSendDataSizeAll=%d\n",__func__,__LINE__,iSendDataSizeAll);	
	
	iRet=lte_normal_at_cmd(acSendDataBuff,iSendDataPackLeng + iSendDataSizeAll,LTE_AT_CMD_READ_RET,acComReadInfo);
	if(0 != iRet)
	{
		return 0;
	}
	//printf("[%s:%d] acComReadInfo=%s \n",__func__,__LINE__,acComReadInfo);
	char acComEscaInfo[32] = {0};
	if(!strcmp(acComReadInfo,""))
	{	
		printf("[%s:%d] return error ---------------------->Error \n",__func__,__LINE__);
		return -1;
	}
	//printf("[%s:%d]acComReadInfo[i]=0x%02x\n",__func__,__LINE__,acComReadInfo[1]);
	hdlc_read_data_escape(acComReadInfo,32,acComEscaInfo);
	printf("[%s:%d] acComEscaInfo ---------------------->0x%0x \n\n",__func__,__LINE__,acComEscaInfo[0]);

	return 0;
}
int file_data_analysis(int state)
{
	DWORD ulHeadSize = 0;
	DWORD ulDataSize = 0;
	DWORD ulBlockSize = 0;
	DWORD ulCrcNum = 0;
	WORD *pusCrcAllData = NULL;
	FILE *fp = NULL;
	char acDataRead[5120] = {0};
	int iRet = 0;
	int iLoop = 0;
	int iDataCheckSize = 0;
	int iAlignAddBit = 0;
	double dAlignCheck = 0;
	char acAlignSize[4] = {0};
    int iFileReadTimes = 0;
    
    FILE_APP_FILE_HEADER_S stAppFileHeader = {0};
    FILE_APP_UNIT_HEADER_S stAppUnitHeader = {0};
    
    //fp=fopen("/tmp/B710S0UPDATE_11.236.03.00.121.BIN","rb");
    fp=fopen(LTE_UPDATE_FILE_PATH,"rb");
    if(NULL==fp)
    {
    	printf("[%s:%d] fopen failed ! \n",__func__,__LINE__);
    	lte_webupdate_info(WEBUPDATE_ERROR"fopen update.bin failed !");
    	syslog(LOG_ERR,"[%s:%d] fopen update.bin failed !\n",__func__,__LINE__);
        return -1;
    }else{
    	printf("[%s:%d]------------start----------- \n",__func__,__LINE__);
    	syslog(LOG_NOTICE,"[%s:%d] fopen bin file !\n",__func__,__LINE__);
        iRet=fread(&stAppFileHeader,sizeof(FILE_APP_FILE_HEADER_S),1,fp);
        
        printf("[%s:%d]------------header----------- \n",__func__,__LINE__);
        memset(&stAppUnitHeader,0,sizeof(FILE_APP_UNIT_HEADER_S));
        
        while(fread(&stAppUnitHeader,sizeof(FILE_APP_UNIT_HEADER_S),1,fp))
        {
        	iFileReadTimes++;
        		
        	printf("[%s:%d]------------%d----------- \n",__func__,__LINE__,iFileReadTimes);
        	
        	ulHeadSize = ReverseBytes(stAppUnitHeader.dwHeaderLen);
        	ulDataSize = ReverseBytes(stAppUnitHeader.dwDataLen);
        	ulBlockSize = ReverseBytes(stAppUnitHeader.dwBlockSize);
        	ulCrcNum = ReverseBytes(stAppUnitHeader.dwDataLen)/ulBlockSize;
        	if(ReverseBytes(stAppUnitHeader.dwDataLen)%ulBlockSize || ulCrcNum == 0)
        	{
        		ulCrcNum++;
        	}
        	
        	//printf("[%s:%d] image_type=0x%lx dwDataLen=%lu ulCrcNum=%lu\n",__func__,__LINE__,ReverseBytes(stAppUnitHeader.image_type),ulDataSize,ulCrcNum);
			
			if(pusCrcAllData)
				pusCrcAllData=NULL;
			pusCrcAllData = (WORD*)malloc(sizeof(WORD)*ulCrcNum);
			memset(pusCrcAllData,0,sizeof(WORD)*ulCrcNum);
			fread(pusCrcAllData,sizeof(WORD)*ulCrcNum,1,fp);
			
			if(FILE_HDLC_SEND == state)
			{
				if(1 == iFileReadTimes)
				{
					lte_webupdate_info(WEBUPDATE_INFO"file data version id check ... ");
					iRet = hdlc_ver_check(&stAppUnitHeader);
					if(-1 == iRet)
					{
						printf("[%s:%d] file data version check failed ! \n",__func__,__LINE__);
						lte_webupdate_info(WEBUPDATE_ERROR"file data version check failed !");
						syslog(LOG_ERR,"[%s:%d] file data version check failed !\n",__func__,__LINE__);
						break;
					}
				}
				lte_webupdate_info(WEBUPDATE_INFO"file data unit hdlc init cmd ...");
				iRet = hdlc_init(&stAppUnitHeader);
				if(-1 == iRet)
				{
					printf("[%s:%d] file data unit hdlc init cmd  failed ! \n",__func__,__LINE__);
					lte_webupdate_info(WEBUPDATE_ERROR"file data unit hdlc init cmd failed !");
					syslog(LOG_ERR,"[%s:%d] file data unit hdlc init cmd  failed !\n",__func__,__LINE__);
					return -1;
				}
				syslog(LOG_NOTICE,"[%s:%d] file data unit hdlc init cmd end \n",__func__,__LINE__);
				lte_webupdate_info(WEBUPDATE_INFO"file data unit send hdlc bytes stream...");
			}else{
				lte_webupdate_info(WEBUPDATE_INFO"file data unit crc check ...");
			}
			for(iLoop=0; iLoop<ulCrcNum; iLoop++)
			{
				if (iLoop == ulCrcNum - 1) {
					iDataCheckSize = ulDataSize -(iLoop)*ulBlockSize;
				}else{
					iDataCheckSize = ulBlockSize;
				}

				fread(acDataRead,sizeof(BYTE)*iDataCheckSize,1,fp);
				
				if(FILE_HDLC_SEND == state)
				{
					printf("[%s:%d]***************file unit:%d last:%lu***************\n",__func__,__LINE__,iFileReadTimes,ulCrcNum-iLoop);

					iRet = hdlc_send(acDataRead,iDataCheckSize,iLoop+1);
					if(-1 == iRet)
					{
						printf("[%s:%d] file data unit hdlc send failed  ! \n",__func__,__LINE__);
						lte_webupdate_info(WEBUPDATE_ERROR"file data unit hdlc send failed !");
						syslog(LOG_ERR,"[%s:%d] file data unit hdlc send failed !\n",__func__,__LINE__);
						free(pusCrcAllData);
						pusCrcAllData=NULL;
						fclose(fp);
						return -1;
					}
				}else{
					if( crc16((unsigned char*)acDataRead,iDataCheckSize) != ReverseBytes_16(pusCrcAllData[iLoop]) )
					{
						printf("[%s:%d] crc error:0x%04x!=0x%04x\n",__func__,__LINE__,crc16((unsigned char*)acDataRead,iDataCheckSize),ReverseBytes_16(pusCrcAllData[iLoop]));
						lte_webupdate_info(WEBUPDATE_ERROR"file data unit hdlc crc check failed !");
						syslog(LOG_ERR,"[%s:%d] file data unit hdlc file unit crc check failed !\n",__func__,__LINE__);
						free(pusCrcAllData);
						pusCrcAllData=NULL;
						fclose(fp);
						return -1;
					}
				}
				memset(acDataRead,0,sizeof(BYTE)*iDataCheckSize);
			}
			free(pusCrcAllData);
			pusCrcAllData=NULL;
			
			if(FILE_HDLC_SEND == state)
			{
				lte_webupdate_info(WEBUPDATE_INFO"file data unit hdlc end  cmd ...");
				iRet = hdlc_end(&stAppUnitHeader);
				if(-1 == iRet)
				{
					printf("[%s:%d] file data unit hdlc end failed  ! \n",__func__,__LINE__);
					lte_webupdate_info(WEBUPDATE_ERROR"file data unit hdlc  end failed !");
					syslog(LOG_ERR,"[%s:%d] file data unit hdlc end failed !\n",__func__,__LINE__);
					return -1;
				}
				syslog(LOG_NOTICE,"[%s:%d] file data unit hdlc end end \n",__func__,__LINE__);
			}
			
			dAlignCheck = (double)(ulHeadSize+ulDataSize)/4;
			//unit header ,crc ,data all 4 size align and add align bit
			if((dAlignCheck - (int)dAlignCheck) > 0)
			{
				iAlignAddBit = FILE_UNIT_ALIGN_TYPE - ((ulHeadSize+ulDataSize)-(int)dAlignCheck*FILE_UNIT_ALIGN_TYPE)%FILE_UNIT_ALIGN_TYPE;
				fread(acAlignSize,iAlignAddBit,1,fp);
			}
			ulCrcNum = 0;
			ulDataSize = 0;
        	ulBlockSize = 0;
        	iDataCheckSize = 0;
			iAlignAddBit = 0;

			memset(&stAppUnitHeader,0,sizeof(FILE_APP_UNIT_HEADER_S));
			printf("[%s:%d]------------end----------- \n",__func__,__LINE__);
			syslog(LOG_NOTICE,"[%s:%d] file data unit hdlc end \n",__func__,__LINE__);
		}
		printf("[%s:%d]------------over----------- \n",__func__,__LINE__);
		lte_webupdate_info(WEBUPDATE_INFO"file crc check or hdlc send ok...");
		
		syslog(LOG_NOTICE,"[%s:%d] file crc check or hdlc send ok !\n",__func__,__LINE__);
		
        fclose(fp);
        syslog(LOG_NOTICE,"[%s:%d] close file !\n",__func__,__LINE__);
    }
    
    return 0;
}
int main(int iArgc, char *apcArgv[])
{
	int iRet = 0;
	char acComReadInfo[32] = {0};
	int iNoNvbackupCmd = 0;
	
	if (2 == iArgc && !strcmp(apcArgv[1],"-a"))
    {
        printf("[%s:%d] lteupdate set lte CMD: 0x0a ! \n",__func__,__LINE__);
		hdlc_write_no_data_cmd(HDLC_CMD_RESET,acComReadInfo);
        syslog(LOG_ERR,"[%s:%d] lteupdate set lte CMD: 0x0a ! \n",__func__,__LINE__);
        return 0;
    }else if(2 == iArgc && !strcmp(apcArgv[1],"-n"))
    {
    	printf("[%s:%d] lteupdate config no nvbackup cmd \n",__func__,__LINE__);
		iNoNvbackupCmd = 1;
        syslog(LOG_ERR,"[%s:%d] lteupdate config no nvbackup cmd  \n",__func__,__LINE__);
    }
	
	if(access(LTE_UPDATE_FILE_PATH,F_OK)!=0)
	{
		printf("[%s:%d] lteupdate bin file not find ! \n",__func__,__LINE__);
		lte_webupdate_info(WEBUPDATE_ERROR"lteupdate bin file not find !");
		syslog(LOG_ERR,"[%s:%d] lteupdate bin file not find ! \n",__func__,__LINE__);
		return 0;
	}
	
	system("killall -9 ltecomsev");
	system("echo '' > /tmp/lteupdate.info");
	
	printf("[%s:%d]-----------lte update start !-----------\n",__func__,__LINE__);
    syslog(LOG_NOTICE,"[%s:%d]-----------lte update start !-----------\n",__func__,__LINE__);
    lte_webupdate_info(WEBUPDATE_INFO"-----------lte update start !-----------");
    
	//lte backup nvram data
	if(0 == iNoNvbackupCmd)
	{
		iLteReadDelay = LTE_COM_READ_DELAY_TIME;
		printf("[%s:%d] ATCMD: at^nvbackup\n",__func__,__LINE__);
		lte_webupdate_info(WEBUPDATE_INFO"ATCMD: at^nvbackup ");
		LTE_NORMAL_AT_CMD_T stAtNvbackup = {"at^nvbackup\r",LTE_COM_READ_RETURN,"OK",0,LTE_DEV_ACMCOM	,0,3};
		iRet=lte_normal_at_cmd((char*)stAtNvbackup.acCmd,strlen(stAtNvbackup.acCmd),LTE_AT_CMD_READ_RET,acComReadInfo);
		if(0 != iRet)
		{
			lte_webupdate_info(WEBUPDATE_ERROR"ATCMD: lte_normal_at_cmd error ");
			syslog(LOG_ERR,"[%s:%d] ATCMD: lte_normal_at_cmd error \n",__func__,__LINE__);
			return 0;
		}
		if(!strstr(acComReadInfo,"0"))
		{
			lte_webupdate_info(WEBUPDATE_ERROR"ATCMD: at^nvbackup ");
			syslog(LOG_ERR,"[%s:%d] ATCMD: at^nvbackup error !\n",__func__,__LINE__);
			return 0;
		}
		syslog(LOG_NOTICE,"[%s:%d] ATCMD: at^nvbackup end\n",__func__,__LINE__);
		iLteReadDelay = LTE_COM_READ_TIME;
	}
		
	//lte config godload mode, godload don't read the return value : LTE_AT_CMD_NO_READ
	memset(acComReadInfo,0,32);
	printf("[%s:%d] ATCMD: at^godload \n",__func__,__LINE__);
	lte_webupdate_info(WEBUPDATE_INFO"ATCMD: at^godload ");
	LTE_NORMAL_AT_CMD_T stAtGodload = {"at^godload\r",LTE_COM_READ_RETURN,"OK",0,LTE_DEV_ACMCOM,0,3};
	iRet=lte_normal_at_cmd((char*)stAtGodload.acCmd,strlen(stAtGodload.acCmd),LTE_AT_CMD_NO_READ,acComReadInfo);
	if(0 != iRet)
	{
		lte_webupdate_info(WEBUPDATE_ERROR"ATCMD: lte_normal_at_cmd error ");
		syslog(LOG_ERR,"[%s:%d] ATCMD: lte_normal_at_cmd error \n",__func__,__LINE__);
		return 0;
	}
	syslog(LOG_NOTICE,"[%s:%d] ATCMD: at^godload end\n",__func__,__LINE__);
	printf("[%s:%d] ATCMD: at^godload end \n",__func__,__LINE__);
	
	//lte config godload mode have to wait for ttyUSB2 up
	sleep(20);

	//lte config datamode mode
	memset(acComReadInfo,0,32);
	printf("[%s:%d] ATCMD: at^datamode \n",__func__,__LINE__);
	lte_webupdate_info(WEBUPDATE_INFO"ATCMD: at^datamode ");
	LTE_NORMAL_AT_CMD_T stAtDatamode = {"at^datamode\r",LTE_COM_READ_RETURN,"OK",0,LTE_DEV_ACMCOM,0,3};
	iRet=lte_normal_at_cmd((char*)stAtDatamode.acCmd,strlen(stAtDatamode.acCmd),LTE_AT_CMD_READ_RET,acComReadInfo);
	if(0 != iRet)
	{
		lte_webupdate_info(WEBUPDATE_ERROR"ATCMD: lte_normal_at_cmd error ");
		syslog(LOG_ERR,"[%s:%d] ATCMD: lte_normal_at_cmd error \n",__func__,__LINE__);
		return 0;
	}
	if(!strstr(acComReadInfo,"OK"))
	{
		lte_webupdate_info(WEBUPDATE_ERROR"ATCMD: at^datamode ");
		syslog(LOG_ERR,"[%s:%d] ATCMD: at^datamode error !\n",__func__,__LINE__);
		return 0;
	}
	syslog(LOG_NOTICE,"[%s:%d] ATCMD: at^datamode end\n",__func__,__LINE__);
	
	//lte config open at echo
	memset(acComReadInfo,0,32);
	printf("[%s:%d] ATCMD: ate1 \n",__func__,__LINE__);
	lte_webupdate_info(WEBUPDATE_INFO"ATCMD: ate1 ");
	LTE_NORMAL_AT_CMD_T stAtE1 = {"ate1\r",LTE_COM_READ_RETURN,"OK",0,LTE_DEV_ACMCOM	,0,3};
	iRet = lte_normal_at_cmd((char*)stAtE1.acCmd,strlen(stAtE1.acCmd),LTE_AT_CMD_READ_RET,acComReadInfo);
	if(0 != iRet)
	{
		lte_webupdate_info(WEBUPDATE_ERROR"ATCMD: lte_normal_at_cmd error ");
		syslog(LOG_ERR,"[%s:%d] ATCMD: lte_normal_at_cmd error \n",__func__,__LINE__);
		return 0;
	}
	if(!strstr(acComReadInfo,"OK"))
	{
		lte_webupdate_info(WEBUPDATE_ERROR"ATCMD: ate1 ");
		syslog(LOG_ERR,"[%s:%d] ATCMD: ate1 error !\n",__func__,__LINE__);
		//return 0;
	}
	syslog(LOG_NOTICE,"[%s:%d] ATCMD: ate1 end\n",__func__,__LINE__);
	
	//update bin file crc check 
	printf("[%s:%d] file data crc check... \n",__func__,__LINE__);
	lte_webupdate_info(WEBUPDATE_INFO"file data crc check...");
	iRet = file_data_analysis(FILE_CRC_CHECK);
	if(0 != iRet)
	{
		lte_webupdate_info(WEBUPDATE_ERROR"file data crc check error !");
		syslog(LOG_ERR,"[%s:%d] file data crc check error !\n",__func__,__LINE__);
		return 0;
	}
	syslog(LOG_NOTICE,"[%s:%d] file data analysis crc check end\n",__func__,__LINE__);

	//update bin file send hdlc bytes stream
	printf("[%s:%d] file data send hdlc bytes stream... \n",__func__,__LINE__);
	lte_webupdate_info(WEBUPDATE_INFO"file data send hdlc bytes stream...");
	iRet = file_data_analysis(FILE_HDLC_SEND);
	if(0 != iRet)
	{
		lte_webupdate_info(WEBUPDATE_ERROR"file data send hdlc bytes stream error !");
		syslog(LOG_ERR,"[%s:%d] file data send hdlc bytes stream error !\n",__func__,__LINE__);
		return 0;
	}
	syslog(LOG_NOTICE,"[%s:%d] file data send hdlc bytes stream end\n",__func__,__LINE__);

	//send file data end HDLC_CMD_RESET lte
	memset(acComReadInfo,0,32);
	printf("[%s:%d] ATCMD: 0x0a [send hdlc end] \n",__func__,__LINE__);
	lte_webupdate_info(WEBUPDATE_INFO"ATCMD: 0x0a [send hdlc end] ");
	hdlc_write_no_data_cmd(HDLC_CMD_RESET,acComReadInfo);
	syslog(LOG_NOTICE,"[%s:%d] ATCMD: 0x0a [send hdlc end] end\n",__func__,__LINE__);
	
	sleep(20);

	//lte restore nvram data
	if(0 == iNoNvbackupCmd)
	{
		iLteReadDelay = LTE_COM_READ_DELAY_TIME;
		printf("[%s:%d] ATCMD: at^nvrestore \n",__func__,__LINE__);
		lte_webupdate_info(WEBUPDATE_INFO"ATCMD: at^nvrestore ");
		LTE_NORMAL_AT_CMD_T stAtNvrestore = {"at^nvrestore\r",LTE_COM_READ_RETURN,"OK",0,LTE_DEV_ACMCOM	,0,3};
		iRet=lte_normal_at_cmd((char*)stAtNvrestore.acCmd,strlen(stAtNvrestore.acCmd),LTE_AT_CMD_READ_RET,acComReadInfo);
		if(0 != iRet)
		{
			lte_webupdate_info(WEBUPDATE_ERROR"ATCMD: lte_normal_at_cmd error ");
			syslog(LOG_ERR,"[%s:%d] ATCMD: lte_normal_at_cmd error \n",__func__,__LINE__);
			return 0;
		}
		if(!strstr(acComReadInfo,"0"))
		{
			lte_webupdate_info(WEBUPDATE_ERROR"ATCMD: at^nvrestore ");
			syslog(LOG_ERR,"[%s:%d] ATCMD: at^nvrestore error !\n",__func__,__LINE__);
			return 0;
		}
		syslog(LOG_NOTICE,"[%s:%d] ATCMD: at^nvrestore end\n",__func__,__LINE__);
		iLteReadDelay = LTE_COM_READ_TIME;
	}
	
	/*
	//lte last reset
	memset(acComReadInfo,0,32);
	printf("[%s:%d] ATCMD: at^reset \n",__func__,__LINE__);
	lte_webupdate_info(WEBUPDATE_INFO"ATCMD: at^reset");
	LTE_NORMAL_AT_CMD_T stAtReset = {"at^reset\r",LTE_COM_READ_RETURN,"OK",0,LTE_DEV_ACMCOM	,0,3};
	lte_normal_at_cmd((char*)stAtReset.acCmd,strlen(stAtReset.acCmd),LTE_AT_CMD_READ_RET,acComReadInfo);
	if(!strstr(acComReadInfo,"OK"))
	{
		lte_webupdate_info(WEBUPDATE_ERROR"ATCMD: at^reset error !");
		syslog(LOG_ERR,"[%s:%d] ATCMD: at^reset error !\n",__func__,__LINE__);
		return 0;
	}
	syslog(LOG_NOTICE,"[%s:%d] ATCMD: at^resetend\n",__func__,__LINE__);
	sleep(10);
	*/
	lte_webupdate_info(WEBUPDATE_SUCCES"-----------lte update over ! -----------");
    syslog(LOG_NOTICE,"[%s:%d]-----------lte update over ! -----------\n",__func__,__LINE__);

    return 0;
}
