/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : ntpctl.c
  版 本 号   : 初稿
  作    者   : xiao wei
  生成日期   : 2016年10月14日 星期五
  最近修改   :
  功能描述   :
  函数列表   :
  修改历史   :
  1.日    期   : 2016年10月14日 星期五
    作    者   : xiao wei
    修改内容   : 创建文件

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <syslog.h>
#include <errno.h>
#include <signal.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <netdb.h>
#include <net/if.h>
#include <sys/socket.h>
#include <asm/types.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include "ap_types.h"
#include "util_include/util.h"

typedef struct {
    int pid;
    /* basename of executable file in call to exec(2),
        size from kernel headers */
    char short_cmd[16];
} procps_status_t;

int util_find_pid_by_name(char *pcPidName, int *piPid)
{
    static DIR *pstDir = NULL;
	struct dirent *pstEntry = NULL;
	char *pcName = NULL;
	procps_status_t stCurstatus = {0};
	char acBuf[512] = {0};
	FILE *pstFp = NULL;
	int iPid = 0;
	struct stat stFileStatus = {0};
    
    /*存放各级目录名称*/
	char acDirName[32] = {0};
    
    /*打开proc文件系统*/
	if (!pstDir) 
    {
		pstDir = opendir("/proc");
		if (!pstDir)
        {      
            syslog(3, "Can't open /proc\n");
            return -2;
        }
	}
    /*一次读取proc文件系统下的数据文件夹*/
    while ( 1 ) 
    {
        memset(acBuf,0,sizeof(acBuf));
        memset(acDirName,0,sizeof(acDirName));
        memset(stCurstatus.short_cmd,0,sizeof(stCurstatus.short_cmd));
        
        /*读取参数dir目录流的下个目录进入点，读完没有找到则退出*/
    	if ((pstEntry = readdir(pstDir)) == NULL)
        {
    		closedir(pstDir);
    		pstDir = NULL;
    		return -1;
    	}
        
        /*属性:文件名*/
    	pcName = pstEntry->d_name;
        /*在proc文件系统下每个进程都有其进程号对应的文件夹，且文件夹名称是pid号*/
    	if (!(*pcName >= '0' && *pcName <= '9'))
        {
    		continue;
        }   

    	memset(&stCurstatus, 0, sizeof(procps_status_t));
    	iPid = atoi(pcName);
    	stCurstatus.pid = iPid;
    	sprintf(acDirName, "/proc/%d", iPid);
        
        /*获取文件夹的状态，确定文件的存在性*/
    	if (stat(acDirName, &stFileStatus))
        {
    		continue;
        }   

        memset(acDirName,0,sizeof(acDirName));
    	sprintf(acDirName, "/proc/%d/stat", iPid);
        
    	if((pstFp = fopen(acDirName, "r")) == NULL)
        {
            continue;
        }   

        /*获取文件内容*/
    	pcName = fgets(acBuf, sizeof(acBuf), pstFp);
        
        /*关闭文件*/
    	fclose(pstFp);
        pstFp = NULL;
        
        acBuf[511] = '\0';
        
    	if (pcName == NULL)
        {
    		continue;
        }   

        /*找到最后字符出现的最后地址，并将其返回*/
        /* split into "PID (cmd" and "<rest>" */
    	pcName = strrchr(acBuf, ')');
    	if(pcName == 0 || pcName[1] != ' ')
        {
    		continue;
        }   
    	*pcName = 0;
    	sscanf(acBuf, "%*s (%15c", stCurstatus.short_cmd);
        //printf("stCurstatus.short_cmd is %s\n",stCurstatus.short_cmd);
        if(0 == strncmp(stCurstatus.short_cmd,pcPidName,strlen(pcPidName)))
        {
            /*将文件的目录流读完*/
            while((pstEntry = readdir(pstDir)) != NULL)
            {
                /*将目录流读空*/;
            }
            /*读空后关闭*/   
            closedir(pstDir);
            pstDir = NULL;
            *piPid = iPid;
            return 1;
        }
    }
    //printf("finally return\n");
    return 1;
}

int  util_popen_cmd( char *pcCmd,char *pcOut,unsigned int uiOutLenMax )
{
    FILE *pstFp = NULL;
    
    if (NULL == pcCmd || NULL == pcOut)
    {
        syslog(3, "[util_popen_cmd] pointer is NULL");

        return 1;
    }

    pstFp = popen(pcCmd, "r");
    if (NULL == pstFp)
    {
        syslog(3, "[util_popen_cmd] pointer is NULL");
        return 1;
    }

    if (fgets(pcOut, uiOutLenMax, pstFp))
    {
        pcOut[strlen(pcOut) - 1] = '\0';
        pclose(pstFp);
    }
    else
    {
        pclose(pstFp);
    }
    
    return 0;
}

int util_system_cmd( char *szCmd )
{
    int iRet;
    int iLoop;

    if  ( NULL == szCmd )
    {
        return 1;
    }

    /* 此处为使util独立，直接采用syslog打印信息 */
    for  ( iLoop = 0 ; iLoop < 5 ; iLoop++ )
    {
         iRet = system(szCmd);
         if  ( -1 != iRet && WIFEXITED(iRet) && 0 == WEXITSTATUS(iRet))
         {
            /* 7 表示日志级别为DEBUG级别 */
             //syslog(7, "[util_system_cmd] Success: %s\n", szCmd);
             return 0;
         }
    }
    /* 3 表示日志级别为ERR级别 */
    syslog(3, "[util_system_cmd] Failed: %s,status = %d\n", szCmd, iRet);

    return iRet;
}

INT32 main(INT32 iArgc, CHAR *apcArgv[])
{
    CHAR acTemp[128];
    CHAR szCmd[CHAR_LEN_128] = {0};
    CHAR szCmd2[CHAR_LEN_128] = {0};
    UINT32 uiNtpMode = 0;
    CHAR szNtpSvr[CHAR_LEN_64] = {0};
    CHAR szNtpSvr2[CHAR_LEN_64] = {0};
    UINT32 uiNtpSynPeriod = 0;
    INT32 iRet = 0;
    CHAR szCmd1[CHAR_LEN_128] = {0};
    CHAR szBuf[CHAR_LEN_256] = {0};
    INT32 iPid = 0;
    INT32 iFailCount = 0;
	CHAR acBuff[128] = {0};
	CHAR acFip[128] = {0};
	CHAR acSip[128] = {0};
	CHAR acMode[128] = {0};
	INT32 port = 0;
	INT32 mode = 0;
    #define CMD_EXEC_FAILED_TIME        1
    #define CMD_EXEC_SUCCESS_TIME       2
    #define TIME_EXEC_NOT_SUCCESS_TIME  5
	UINT32 ntpRunTotal = 0;
	
//    if(AP_E_NONE != nvram_get(acTemp,NVRAM_SYSMNT_NTP_MODE, NVRAM_VALUE_MAX_LEN))
//    {
//        syslog(3, "nvram get NVRAM_SYSMNT_NTP_MODE failed");
//    }
//    uiNtpMode = atoi(acTemp);
//
//    if ( 1 != uiNtpMode )// 1:×??ˉí?2?ê±??
//    {
//        return 0;
//    }
//
//    if(AP_E_NONE != nvram_get(szNtpSvr,NVRAM_SYSMNT_NTP_SYN_SERVER, CHAR_LEN_64))
//    {
//        syslog(3, "nvram get NVRAM_SYSMNT_NTP_SYN_SERVER failed");
//        return 1;
//    }
//	if(AP_E_NONE != nvram_get(szNtpSvr2,NVRAM_SYSMNT_NTP_SYN_SERVER2, CHAR_LEN_64))
//    {
//        syslog(3, "nvram get NVRAM_SYSMNT_NTP_SYN_SERVER2 failed");
//        //return 1;
//    }
//    
//    if(AP_E_NONE != nvram_get(acTemp,NVRAM_SYSMNT_NTP_SYN_PREIOD, NVRAM_VALUE_MAX_LEN))
//    {
//        syslog(3, "nvram get NVRAM_SYSMNT_NTP_SYN_PREIOD failed");
//        return 1;
//    }
	char c;
	int option_index = 1;	
	static const char short_options[] = "m:i:p:f:s:";
    static const struct option long_options[] = {
		{ "mode",      		1, 0, 'm' },    // 模式 
		{ "time",      		1, 0, 'i' },    // 同步周期 秒
		{ "port",      		1, 0, 'p' },    // 服务器端口
		{ "mipaddr",      		1, 0, 'f' },    // 主服务器IP地址
		{ "sipaddr",      		1, 0, 's' },    // 备服务器IP地址
        { "help",          			0, 0, 'l' },
	    { NULL,         			0, 0,  0  }
	};
	
	while(1)
	{
		c = getopt_long(iArgc, apcArgv, short_options, long_options, &option_index);
		if (c == -1) {
			break;
		}
		switch (c) {
		case 'm':
		    strncpy(acMode, optarg, sizeof(acMode));
			break;
		case 'i':
		    strncpy(acTemp, optarg, sizeof(acTemp));
			break;
		case 'p':	
			strncpy(acBuff, optarg, sizeof(acBuff));
			break;
		case 'f':	
			strncpy(acFip, optarg, sizeof(acFip));
			break;
		case 's':	
			strncpy(acSip, optarg, sizeof(acSip));
			break;
		case 'l':	
			break;
		default:
			//help 
		    ;
		}
		
	}
    uiNtpSynPeriod = atoi(acTemp);
	port = atoi(acBuff);
	//mode = atoi(acMode);
	/*if ( 0 != mode )// 1:×??ˉí?2?ê±??
    {
        return 0;
    }*/
	//printf("hahahahahahaahahahahaahahhahahahaahhaaha\n");
    memset(szCmd,0,sizeof(szCmd));
    sprintf(szCmd,"ntpclient -c 1 -i %d -s -l -D -t -p %d -h %s >/dev/null 1>/dev/null &",uiNtpSynPeriod,port,acFip);
    memset(szCmd2,0,sizeof(szCmd2));
    sprintf(szCmd2,"ntpclient -c 1 -i %d -s -l -D -t -p %d -h %s >/dev/null 1>/dev/null &",uiNtpSynPeriod,port,acSip);
    memset(szCmd1,0,sizeof(szCmd1));
	//printf("wewrewtwtwtwtwtwtwtwtwtwtwtwtwtwtwtwtwtwtwtwt\n");
    sprintf(szCmd1,"date | grep 'CST 1970'");
    do
    {
        /* ??è?μ±?°ntpclientê?·?ò??-?ú?? */
      //  iRet = util_find_pid_by_name("ntpclient",&iPid);
       // if ( 1 != iRet )
       // {
        	//printf("1111111111111111111111111111111111111111111111111\n");
			//printf("ntpRunTotal =%ld \n",ntpRunTotal);
        	if( 0 == ntpRunTotal%2 )
        	{
				util_system_cmd("killall -9 ntpclient");
            	iRet = util_system_cmd(szCmd);
				printf("ntpclient szCmd=%s \n",szCmd);
				//printf("222222222222222222222222222222222222222222222222\n");
        	}
        	if( 1 == ntpRunTotal%2 )
        	{
				util_system_cmd("killall -9 ntpclient");
        		iRet = util_system_cmd(szCmd2);
				printf("ntpclient szCmd2=%s \n",szCmd2);
				//printf("3333333333333333333333333333333333333333333333333333\n");
        	}     
            if ( 0 != iRet )
            {
                syslog(3, "line:%d,command<%s> exec failed:%d",__LINE__,szCmd,iRet);
                sleep(CMD_EXEC_FAILED_TIME);
                continue;
            }
            else //?üá??′DD3é1|oó￡?μè′yí?2?±¨??μ???±¨
            {
                sleep(CMD_EXEC_SUCCESS_TIME);
            }
            
       // }
		//printf("4444444444444444444444444444444444444444444444444\n");
			printf("ntpclient OK \n");
        /* 2é?′μ±?°ê?·?ò??-í?2?3é1|￡?è?1??′3é1|￡??ò2?μè′yí?2??ü?ú */
			memset(szBuf,0,sizeof(szBuf));
       
                	ntpRunTotal++;
					if( 10000==ntpRunTotal )
					{
						ntpRunTotal = 0;
						printf("ntpRunTotal =10000 \n");
					}
              //  }
               // sleep(TIME_EXEC_NOT_SUCCESS_TIME);
                
            //    continue;
        mode = uiNtpSynPeriod+10;
        sleep(mode);
       
    } while ( 1 );


    return 0;
}
