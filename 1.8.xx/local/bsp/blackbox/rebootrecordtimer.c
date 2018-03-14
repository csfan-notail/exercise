#include <stdio.h>
#include <sys/socket.h>
#include <time.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <linux/unistd.h>
#include "upver.h"


#define RECORD_TIME	5

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

int record_hardreboot()
{
	FILE *fp = NULL;
	USR_FILE_INFO_T stUsrFileInfo = {0};
	T_RebootInfo stRebootInfo = {0};
	time_t t;
	struct tm *pt;
	int i = 0, iRet = 0;

	/*读历史重启记录*/
	if(NULL == (fp = fopen(REBOOT_USER_FILE, "rb+"))){
		printf( "%s, open file(%s) error.\n", __func__, REBOOT_USER_FILE);
		return -1;
	}

	if((iRet = fread(&stUsrFileInfo, sizeof(USR_FILE_INFO_T), 1, fp)) != 1){
		printf("%s, read file failed(%d).\n", __func__, iRet);
		goto err;
	}

	if(stUsrFileInfo.iCheck != REBOOT_CHECK){
		fclose(fp);
		return 0;
	}

	/* 定时写下一硬重启记录时间*/
	memset(&stRebootInfo, 0, sizeof(T_RebootInfo));
	stRebootInfo.iType = REBOOT_HARDRESET;
	stRebootInfo.iUptime = getCurrentTicks();
	
	t = time(NULL);
	pt = localtime(&t);
	if (pt != NULL)
		memcpy(&stRebootInfo.stCurTime, pt, sizeof(struct tm));

	strcpy(stUsrFileInfo.stRebootInfo[i].acReason, "hard reboot");
	
	i = stUsrFileInfo.ucRebootCount%MAX_REBOOT_RECORD_NUM;
	memcpy(&(stUsrFileInfo.stRebootInfo[i]), &stRebootInfo, sizeof(T_RebootInfo));

	/*写历史重启记录*/
	rewind(fp);
	if((iRet =fwrite(&stUsrFileInfo, sizeof(USR_FILE_INFO_T), 1, fp)) != 1){
		printf("%s, write file failed(%d).\n", __func__, iRet);
		goto err;
	}

	fclose(fp);

	return 0;

err:
	if(fp)
		fclose(fp);

	return -1;
}

int main()
{
	while(1){
		record_hardreboot();
		sleep(RECORD_TIME);
	}

	return 0;
}
