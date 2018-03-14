#include <stdio.h>
#include <sys/socket.h>
#include <time.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <linux/unistd.h>
#include "upver.h"


char cmdString[256];
char strCmd[128];
/*******************************************************************************
*******************************************************************************/
inline int	SYSTEM(char *cmd)
{
	 int iRet = 0;
	 int iLoop;
    
    	if(NULL == cmd)
    	{
        	return -1;
    	}
    
    	/* execute this cmd */
    	for(iLoop = 0; iLoop < 5; iLoop++)
    	{
    		iRet = system(cmd);
		if(iRet != 0)
		{
			//syslog(5, "SYSTEM %s fail, iRet:%d\n", cmd, iRet);
			continue;
		}
		break;
    	}
    	if(iRet != 0)
	{
		syslog(5, "SYSTEM %s fail, iRet:%d\n", cmd, iRet);
	}
    
	return iRet;
}

unsigned long getCurrentTicks( void )
{
	static unsigned long count = 0;
	struct sysinfo info;

	if( 0 == sysinfo( &info ) )
	{
		count = (unsigned long)info.uptime;
	}
	printf("current ticks is %d\n", count);

	return count;
}

/*******************************************************************************
*******************************************************************************/
int make_reboot_record(USR_FILE_INFO_T * pstUsrFileInfo)
{
    FILE *fp = NULL;
	int i = 0, iNum = 0, iRet = 0, j = 0;

	if(NULL == (fp = fopen(REBOOT_RECORD_FILE, "wb")))
	{
		printf( "user_file_write:open /tmp/rebootrecord error\n");
		return AP_MISC_ERROR;
	}
	
	if (pstUsrFileInfo->ucRebootCount >= MAX_REBOOT_RECORD_NUM)
	  iNum = MAX_REBOOT_RECORD_NUM;
	else
	  iNum = pstUsrFileInfo->ucRebootCount;

    fprintf(fp, "********************** lastest reboot records **********************\n");
    fprintf(fp, "reboot count : %d\n", pstUsrFileInfo->ucRebootCount);

	for ( i = 0; i < iNum; i++)
	{
        /* 生成reboot历史记录 */
        j = (MAX_REBOOT_RECORD_NUM + pstUsrFileInfo->ucRecordPos -i)%MAX_REBOOT_RECORD_NUM;
    
        if (REBOOT_SOFTRESET == pstUsrFileInfo->stRebootInfo[j].iType)
        {
            sprintf(cmdString,"soft reboot");
        }
        else
        {
            sprintf(cmdString,"hard reboot");
        }
        fprintf(fp, "latest reboot(%d): %s, uptime is %0.2dh%0.2dm%0.2ds, time is %0.4d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d; %s\n", 
                                i,
                                cmdString,
                                pstUsrFileInfo->stRebootInfo[j].iUptime/3600,
                                (pstUsrFileInfo->stRebootInfo[j].iUptime%3600)/60,
                                pstUsrFileInfo->stRebootInfo[j].iUptime%60,
                                pstUsrFileInfo->stRebootInfo[j].stCurTime.tm_year + 1900,
                                pstUsrFileInfo->stRebootInfo[j].stCurTime.tm_mon + 1,
                                pstUsrFileInfo->stRebootInfo[j].stCurTime.tm_mday,
                                pstUsrFileInfo->stRebootInfo[j].stCurTime.tm_hour,
                                pstUsrFileInfo->stRebootInfo[j].stCurTime.tm_min,
                                pstUsrFileInfo->stRebootInfo[j].stCurTime.tm_sec,
                                pstUsrFileInfo->stRebootInfo[j].acReason);
    }
    fprintf(fp, "************************ reboot records end ************************\n");
    fclose(fp);
  
    return 0;
}

/*******************************************************************************
*******************************************************************************/
int reboot_file_write(void)
{
	FILE *fp = NULL;
	FILE *file = NULL;
	time_t t;
	struct tm *pt;
	USR_FILE_INFO_T stUsrFileInfo = {0};
	int i = 0, iRet = 0;
	
	if (0 == access(REBOOT_SHOW_GET_ACCESS, R_OK))
	  return AP_MISC_SUCCESS;

	if(NULL == (fp = fopen(REBOOT_USER_FILE, "rb")))
	{
		printf( "user_file_write:open userfile error\n");
		return AP_MISC_ERROR;
	}
	else
	{
        iRet = fread( &stUsrFileInfo, sizeof( USR_FILE_INFO_T ), 1, fp );
        fclose(fp);
	}
	/* 如果是第一次启动，初始化重启记录 */
	if (REBOOT_CHECK != stUsrFileInfo.iCheck )
	{
        sprintf(strCmd, "touch %s", REBOOT_HARDRESET_FILE);
        SYSTEM(strCmd);

        printf("******** system cold reset, initialize reboot record area ********\n");
//    printf("*******stUsrFileInfo.iCheck is %d\n",stUsrFileInfo.iCheck);
        memset(&stUsrFileInfo, 0, sizeof(USR_FILE_INFO_T));
        stUsrFileInfo.iCheck = REBOOT_CHECK;
        stUsrFileInfo.iFlag = REBOOT_HARDRESET;
        stUsrFileInfo.ucRebootCount = 0;
	}
	else
	{
	    if(REBOOT_HARDRESET == stUsrFileInfo.iFlag) /* 如果本次为硬重启 */
  	    {
            sprintf(strCmd, "touch %s", REBOOT_HARDRESET_FILE);
            SYSTEM(strCmd);

    	    /* 增加本次重启记录 */
    	    i = stUsrFileInfo.ucRebootCount%MAX_REBOOT_RECORD_NUM;
#if 0
    	    stUsrFileInfo.stRebootInfo[i].iType = REBOOT_HARDRESET;
    	    stUsrFileInfo.stRebootInfo[i].iUptime = getCurrentTicks();
    	    t = time(NULL);
    	    pt = localtime(&t);
    	    if (pt != NULL)
    	    {
    		    memcpy(&(stUsrFileInfo.stRebootInfo[i].stCurTime), pt, sizeof(struct tm));
    	    }
    	    strcpy(stUsrFileInfo.stRebootInfo[i].acReason, "hard reboot");
#endif
    	    stUsrFileInfo.ucRecordPos = i;
    	    stUsrFileInfo.ucRebootCount ++;
	    }
	    else /* 如果本次为软重启 */
	    {
            sprintf(strCmd, "touch %s", REBOOT_SOFTRESET_FILE);
            SYSTEM(strCmd);

	        stUsrFileInfo.iFlag = REBOOT_HARDRESET;
	    }
	}

    /* 保存重启记录 */
    if(NULL == (fp = fopen(REBOOT_USER_FILE, "wb")))
	{
		printf( "user_file_write:open userfile error\n");
		return AP_MISC_ERROR;
	}
    if (1 != fwrite(&stUsrFileInfo, sizeof(USR_FILE_INFO_T), 1, fp))
	{
        printf("user_file_write: write file failed");
        fclose(fp);
        return AP_MISC_ERROR;
	}
	fclose(fp);

	/* 解析并保存重启记录到tmp目录 */
	make_reboot_record(&stUsrFileInfo);

    sprintf(strCmd, "touch %s", REBOOT_SHOW_GET_ACCESS);
    SYSTEM(strCmd);
	
	return AP_MISC_SUCCESS;
}

int main (int argc, char **argv)
{
	if (argc == 2)
	{
        if (NULL != strstr(argv[1], "get"))
        {
            reboot_file_write();
		SYSTEM("rebootrecordtimer &");
        }
	}

    SYSTEM ("cat "REBOOT_RECORD_FILE);

	return 0;
}
