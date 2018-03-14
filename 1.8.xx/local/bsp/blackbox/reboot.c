#include <stdio.h>
#include <sys/socket.h>
#include <time.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <linux/unistd.h>
#include "upver.h"

#define rc_put_ENTRY_FILENAME     "/proc/record_put"

#define SUBSYS_OMA              2
#define MODULE_OMA             ( SUBSYS_OMA << 8)

/* LOG 类型*/
#define LOG_TYPE_SECURITY       1
#define LOG_TYPE_DEVICE         2
#define LOG_TYPE_OPERATION      3
#define LOG_TYPE_SERVICE        4

/* LOG级别 */
#define  BB_LOG_LEVEL_FATAL       4
#define  BB_LOG_LEVEL_ERROR       3
#define  BB_LOG_LEVEL_WARNING     2 
#define  BB_LOG_LEVEL_INFO        1
#define  BB_LOG_LEVEL_DETAIL      0

#define OMA_MODULE_PLAT       (MODULE_OMA+0)

/* LOG文件编号 */
#define LOG_FILE_CUR            1    /*  log since this reboot */
#define LOG_FILE_ALL            2     /* log since power on */

#define USER_FILE_ITEM_NUM      4
#define USER_FILE_SIZE          sizeof(rc_log_item_t) * USER_FILE_ITEM_NUM

#define LTECOMSEV 			"ltecomsev"
#define LOGSAVE  "logsave"
#define SIMLEDSTATUS 	"simledstatus"
#define LTEAUTO  		"lteauto"
#define LTEGETINFO 		"ltegetinfo"
#define LEDSTATUS 		"ledstatus"
#define ETHDUMP 		"ethdump"
char cmdString[128];

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

int user_file_write(T_RebootInfo * pstRebootInfo)
{

	FILE *fp = NULL;
	USR_FILE_INFO_T stUsrFileInfo;
	int i = 0, iRet = 0;

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
	/* 填写本次重启信息 */
	/* 如果重启次数超出记录区，则从头部开始记录 */
	i = stUsrFileInfo.ucRebootCount;
	while ( i >= MAX_REBOOT_RECORD_NUM )
	{
        i = i - MAX_REBOOT_RECORD_NUM;
    }
	memcpy(&(stUsrFileInfo.stRebootInfo[i]), pstRebootInfo, sizeof(T_RebootInfo));
  
	stUsrFileInfo.ucRecordPos = i;	
    stUsrFileInfo.ucRebootCount ++;
	stUsrFileInfo.iFlag = REBOOT_SOFTRESET;

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
	

	//printf( "user file write done\n");
	fclose(fp);

	return 0;
}

/*******************************************************************************
*******************************************************************************/
unsigned long getCurrentTicks( void )
{
	static unsigned long count = 0;
	struct sysinfo info;

	if( 0 == sysinfo( &info ) )
	{
		count = (unsigned long)info.uptime;
	}

	return count;
}

/*******************************************************************************
*******************************************************************************/
int write_log_item(int iLogFileId, LOG_ITEM_T* pstItem)
{
	FILE *pFile = NULL;
	if (pstItem == NULL)
	{
		printf( "write_log_item: input parameter is invalid\n");		
		return -1;
	}
	if ((iLogFileId == LOG_FILE_CUR) || (iLogFileId == LOG_FILE_ALL))
	{
		pFile = fopen(rc_put_ENTRY_FILENAME, "wb");
	}

	if (pFile == NULL)
	{
		//printf( "write_log_item: fail to open file\n");				
		return -1;
	}  
	fwrite(pstItem, 1, sizeof(LOG_ITEM_T), pFile);
	fclose(pFile);

	return 0;
}

/*******************************************************************************
*******************************************************************************/
void printusage(void)
{
	printf("Usage: reboot [N] [comment] \n");
	printf("       --- [N] delay N seconds to reboot.\n");
	printf("       --- [comment] reason to reboot.\n");
}

/*******************************************************************************
*******************************************************************************/
int main (int argc, char **argv)
{
	int iRet;
	LOG_ITEM_T stLogInfo;
	T_RebootInfo stRebootInfo;
	time_t t;
	struct tm *pt;
	int i, iLen = 0;

	setenv("TZ", "CST-8", 1);

	if (argc == 1)
  {
    memset(cmdString, 0, sizeof(cmdString));
    sprintf(cmdString, "manual reboot");
    goto finish;
  }

	if (argc >= 2)
  {
    /* 不用检查argv[1]是否为数字，不是数字sleep直接退出 */
    sprintf (cmdString, "sleep %s", argv[1]);
    system(cmdString);
  }

	memset(cmdString, 0, sizeof(cmdString));
	for (i = 2; i < argc; i++)
  {
    /* 如果日志超过128个字节，就不再记录超过部分的日志 */
  	iLen = strlen(cmdString) + strlen(argv[i]);
  	if (iLen >= 128 - 2)
    break;

  	strcat(cmdString, argv[i]);
  	strcat(cmdString, " ");
  }
  
finish:
	memset(&stLogInfo, 0, sizeof(stLogInfo));
	stLogInfo.usModuleId = OMA_MODULE_PLAT;
	stLogInfo.usType = LOG_TYPE_OPERATION;
	stLogInfo.usLevel = BB_LOG_LEVEL_FATAL;

	/* 获取日志时间 */
	stLogInfo.tsCurTime.tv_sec = (long)time(NULL);
	stLogInfo.tsCurTime.tv_usec = (long)time(NULL)/1000;
	stLogInfo.tsRsvTime.tv_sec = getCurrentTicks();
	stLogInfo.tsRsvTime.tv_usec = getCurrentTicks()/1000;

	strncpy(stLogInfo.acMessage, cmdString, sizeof(stLogInfo.acMessage)-1);
	stLogInfo.acMessage[sizeof(stLogInfo.acMessage)-1] = '\0';

	/* 写日志 */
	write_log_item(LOG_FILE_CUR, &stLogInfo);
	syslog(0, stLogInfo.acMessage);

    /* 填写重启信息 */
	system("pkill rebootrecordtimer");
	
    memset(&stRebootInfo, 0, sizeof(T_RebootInfo));
    //获得sysDateTime
	t = time( NULL );
	pt = localtime( &t );
	if ( pt != NULL )
	{
	    memcpy(&( stRebootInfo.stCurTime), pt, sizeof(struct tm));
	}
    stRebootInfo.iUptime = getCurrentTicks();
    stRebootInfo.iType = REBOOT_SOFTRESET;
    strcpy(stRebootInfo.acReason, cmdString);
    
	user_file_write(&stRebootInfo);

	system("ltectl disconnect");
	//lte remove attach
	system("ltectl removeattach");
	
	//lte power gpio disable 
	system("gpio 13 out 0");
	system("echo out >/sys/class/gpio/gpio49/direction");
	system("echo 0 >/sys/class/gpio/gpio49/value");

	system("killall -9 "LOGSAVE);
	system("killall -9 "ETHDUMP);
	system("killall -9 "LTEGETINFO);
	system("killall -9 "SIMLEDSTATUS);
	system("killall -9 "LTEAUTO);
	system("killall -9 "LEDSTATUS);
	system("killall -9 "LTECOMSEV);
	
	system("umount /mnt/emmc/logsave/");
	system("umount /mnt/emmc/packet/");
	
	system("/bin/busybox reboot");
	
	
	return 0;
}
