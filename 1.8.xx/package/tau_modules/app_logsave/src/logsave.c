#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <stdarg.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <getopt.h>
#include <sys/stat.h>  
#include <dirent.h>
#include <sys/vfs.h>
#include "ap_types.h"


static int iLogEachSize	= 1*1024*1024;		//default 1M

static char acPacSaveName[64] = {0};// emmc save packet name

#define LOGSAVE_SAVE_TMPPATH 			"/tmp/log/syslog.log"
#define LOGSAVE_SAVE_LOGPATH 			"/mnt/emmc/logsave"
#define LOGSAVE_SAVE_LOGLENG 			12*1024	//12k mv emmc save
#define LOGSAVE_SAVE_DEL_SIZE 			300	//300M delete log file

#define LOGSAVE_SAVE_NUM_FILE			"/mnt/emmc/logsave/logfilenum"
#define LOGSAVE_FILE_MAX_NUM			99
#define SYSLOG_WRITE_NAME 			"/tmp/syslog_name"
#define LOGSAVE_FILE_FLAG 			"_sys_log"
 
 
int logsave_del_RandomInRange(int min, int max) {  
    int random = rand() % (max-min+1) + min;  
    return random;  
}  
  
void logsave_del_Swap (long long* num1, long long* num2) {  
    long long temp = *num1;  
    *num1 = *num2;  
    *num2 = temp;  
}  
  
int logsave_del_Partition(long long data[], int length, int start, int end) {  
    if (data == NULL || length <= 0 || start < 0 || end >= length)  
        return -1;  
    int index = logsave_del_RandomInRange(start, end);  
    logsave_del_Swap(&data[index], &data[end]);  
    int small = start-1;  
    for (index = start; index < end; ++index) {  
        if (data[index] < data[end]) {  
            small++;  
            if (small != index)  
                logsave_del_Swap(&data[small], &data[index]);  
        }  
    }  
    small++;  
    logsave_del_Swap(&data[small], &data[end]);  
    return small;  
}  
  
void logsave_del_GetLeastNumbers(long long* input, int n, long long* output, int k) {  
    if (input == NULL || n <= 0 || output == NULL || k <= 0 || k>n)  
        return;  
    int start = 0;  
    int end = n-1;  
    int i=0;
    int index = logsave_del_Partition(input, n, start, end);  
    while (index != k-1) {  
        if (index > k-1) {  
            end = index - 1;  
            index = logsave_del_Partition(input, n, start, end);  
        } else {  
            start = index + 1;  
            index = logsave_del_Partition(input, n, start, end);  
        }  
    }  
    for (i=0; i<k; ++i) {  
        output[i] = input[i];  
    }  
}
void logsave_popen_ret(char *acCmd, char *retval)
{
	FILE *fp_p = NULL;
	char acBuff[32] = {0};
	fp_p = popen(acCmd,"r");
	if(fp_p){
		fgets(acBuff,32,fp_p);
		acBuff[strlen(acBuff)-1] = '\0';
		strcpy(retval,acBuff);
	}
	pclose(fp_p);
}
unsigned long long logsave_get_space_size(char *pcPath)  
{  
    if(pcPath == NULL)  
    {	
    	//syslog(LOG_ERR,"[%s:%d] get space size failed %s\n", __func__, __LINE__,pcPath);
		return -1;
    }
    struct statfs diskInfo;  
    statfs(pcPath,&diskInfo);  
	
	unsigned long long blocksize = diskInfo.f_bsize; 
    //printf("[%s:%d] diskInfo.f_bfree*totalBlocks=%lu \n", __func__, __LINE__, diskInfo.f_bfree*diskInfo.f_bsize);
      
    //unsigned long long freeDisk = diskInfo.f_bfree*totalBlocks;  
    unsigned long long availableDisk = diskInfo.f_bavail * blocksize;

  	
    return availableDisk>>20; 
}  
int logsave_delete_log_file(char *pcPath)
{
	DIR *dirptr = NULL;  
	struct dirent *entry; 
	char *pcGetNum;

	int iLoop = 0;
	char acDelFile[64] = {0};
	char acCmd[64] = {0};
	// Modify: by xiaowei, 2017/03/20 11:45 PN: Delete the current number after ten file
	
	char acRetval[32] = {0};
	logsave_popen_ret("ls "LOGSAVE_SAVE_LOGPATH"/  | grep  "LOGSAVE_FILE_FLAG" | wc -l",acRetval);
	//printf("get all log number : %d \n", atoi(acRetval));
	
	if(atoi(acRetval) <= 0){
		//printf("log file not find ! num: %d \n", atoi(acRetval));
		//syslog(LOG_ERR,"[%s:%d] log file not find!\n", __func__, __LINE__);
		return -1;  
	}
	
	long long *alllog = NULL;
	alllog = (long long *)malloc(sizeof(long long)*atoi(acRetval)+1);
	
	if(NULL == alllog){
		//printf("malloc failed! ");
		//syslog(LOG_ERR,"[%s:%d] malloc failed!\n", __func__, __LINE__);
		return -1;  
	}
	long long get_date = 0;
	int time = 0;
	dirptr = opendir(pcPath);
	if( dirptr == NULL)
	{  
		free(alllog);	
		//printf("open dir failed! dir: %s", pcPath);
		//syslog(LOG_ERR,"[%s:%d] open dir failed! %s\n", __func__, __LINE__,pcPath);
		return -1;  
	}else{  
		while (entry = readdir(dirptr))  
		{  
			if(strcmp(entry->d_name,".") && strcmp(entry->d_name,".."))
			{
				//printf("[%s:%d] entry->d_name=%s \n", __func__, __LINE__, entry->d_name);
				pcGetNum=strstr(entry->d_name,"_sys");
				if(NULL==pcGetNum)
					continue;
								
				sscanf(entry->d_name,"%lld"LOGSAVE_FILE_FLAG,&get_date);
				//printf("%d:%lld \n",time, get_date);
				alllog[time] = get_date;
				time++;
			}
		}
		closedir(dirptr);
	}
    long long del_log[10];  
    logsave_del_GetLeastNumbers(alllog, atoi(acRetval), del_log, 10); 
    free(alllog);
    
	//delete 10 day log file
	for(iLoop = 0; iLoop < 10; iLoop++)
	{
		memset(acDelFile,0,sizeof(acDelFile));
		memset(acCmd,0,sizeof(acCmd));
		sprintf(acDelFile,"%s/%lld"LOGSAVE_FILE_FLAG,LOGSAVE_SAVE_LOGPATH,del_log[iLoop]);
		sprintf(acCmd,"rm -rf %s",acDelFile);
		system(acCmd);
		//printf("[%s:%d] delete log file :%s \n", __func__, __LINE__, acCmd);
		//syslog(LOG_INFO,"[%s:%d] delete log file :%s \n", __func__, __LINE__,acCmd);
	}
	
	return 0;
}
int logsave_create_save_file()
{
	char acBuff[64] = {0};
	time_t timep;
	struct tm *p;
	time(&timep);
	p=localtime(&timep);
	
	char acTime[64] = {0};
	
	memset(acPacSaveName,0,64);
	snprintf(acTime,32,"%04d%02d%02d%02d%02d%02d", \
			(1900+p->tm_year),( 1+p->tm_mon), p->tm_mday,p->tm_hour, p->tm_min, p->tm_sec);

	snprintf(acBuff,64,"%s"LOGSAVE_FILE_FLAG,acTime);
	//		(1900+p->tm_year),( 1+p->tm_mon), p->tm_mday,p->tm_hour, p->tm_min);
	snprintf(acPacSaveName,64,"%s/%s", LOGSAVE_SAVE_LOGPATH,acBuff);
	//printf("[%s:%d] acPacSaveName=%s \n", __func__, __LINE__, acPacSaveName);
	memset(acBuff,0,64);
	snprintf(acBuff,64,"echo '' > %s",acPacSaveName);
	system(acBuff);
	memset(acBuff,0,64);
	snprintf(acBuff,64,"touch %s",acPacSaveName);
	system(acBuff);
	
	// Modify: by xiaowei, 2017/07/05 14:32 PN: add syslog write file name to /tmp
	memset(acBuff,0,64);
	snprintf(acBuff,64,"echo '%s"LOGSAVE_FILE_FLAG"' > "SYSLOG_WRITE_NAME,acTime);
	system(acBuff);
	
	// Modify: by xiaowei, 2017/03/20 11:37 PN: Delete the Find Continued file
/*	DIR *dirptr = NULL;  
	struct dirent *entry; 
	int iPacSaveCreate = 0;
	dirptr = opendir(LOGSAVE_SAVE_LOGPATH);
	if( dirptr == NULL)
	{  
		printf("open dir failed! dir: %s", LOGSAVE_SAVE_LOGPATH);
		syslog(LOG_ERR,"[%s:%d] open dir failed! dir: %s \n", __func__, __LINE__,LOGSAVE_SAVE_LOGPATH);
		return -1;  
	}else{  
		while (entry = readdir(dirptr))  
		{  
			//printf("[%s:%d] entry->d_name=%s \n", __func__, __LINE__, entry->d_name);
			if(strstr(entry->d_name,acBuff)!=NULL)
			{
				printf("[%s:%d] find file name: %s\n", __func__, __LINE__,entry->d_name);
				iPacSaveCreate = 1;
			}
		}
		if(0 == iPacSaveCreate)
		{
			memset(acBuff,0,32);
			sprintf(acBuff,"touch %s",acPacSaveName);
			system(acBuff);
			printf("[%s:%d] touch acPacSaveName=%s \n", __func__, __LINE__, acPacSaveName);
			syslog(LOG_INFO,"[%s:%d] touch log file acPacSaveName=%s \n", __func__, __LINE__,acPacSaveName);
		}
		closedir(dirptr);
	}
*/	
	return 0;
}

unsigned long logsave_get_file_size(const char *path)  
{  
    FILE *fp_save;
	unsigned long filesize = 0;
	
	fp_save=fopen(path, "r");
	if(NULL!=fp_save)
	{
		fseek(fp_save, 0L, SEEK_END);  
    	filesize = ftell(fp_save);  
    	fclose(fp_save);  
    	
    	return filesize; 
	}
		
    return 0;  
}  

void logsave_loop_get_packet()
{
	FILE *fp_save;
	FILE *fp_tmp;
	int read_len = 0;

	//time_t timep;
	//struct tm *p;
	//time(&timep);
	//p=gmtime(&timep);
	unsigned long ulFileSize = 0;
	unsigned long long dCurrSaveSize = 0;
	char acReadBuff[LOGSAVE_SAVE_LOGLENG*64] = {0};
	
	while(1){
		//00:00 create new *.log file
		/*
		time(&timep);
		if(p)
			p=NULL;
		p=localtime(&timep);
		
		printf("[%s:%d] p->tm_hour=%d p->tm_min=%d\n",__func__,__LINE__,p->tm_hour,p->tm_min);
		if(0==p->tm_hour && 0==p->tm_min)
		{
			printf("[%s:%d] p->tm_hour=%d p->tm_min=%d\n",__func__,__LINE__,p->tm_hour,p->tm_min);
			logsave_create_save_file();
		}
		*/
		
		dCurrSaveSize = 0;
		dCurrSaveSize=logsave_get_space_size(LOGSAVE_SAVE_LOGPATH);
		//printf("[%s:%d] dCurrSaveSize=%llu M\n", __func__, __LINE__, dCurrSaveSize);
		if(dCurrSaveSize <= LOGSAVE_SAVE_DEL_SIZE)
		{
			//printf("[%s:%d] save size < %d M delete log\n",__func__,__LINE__,LOGSAVE_SAVE_DEL_SIZE);
			logsave_delete_log_file(LOGSAVE_SAVE_LOGPATH);
		}
		
		// Add: by xiaowei, 2017/03/20 13:24 PN: File size check and create new file
		ulFileSize = logsave_get_file_size(acPacSaveName);
		//printf("[%s:%d] ulFileSize=%d iLogEachSize=%d\n", __func__, __LINE__, ulFileSize,iLogEachSize);
		//2?????~?iLogEachSize????
		//if(iLogEachSize <= ulFileSize)
		//{
		//	logsave_create_save_file();
		//}
		
		ulFileSize = logsave_get_file_size(LOGSAVE_SAVE_TMPPATH);
		
		if(LOGSAVE_SAVE_LOGLENG <= ulFileSize)
		{
			//printf("[%s:%d] mv tmp packet ... %d\n",__func__,__LINE__,ulFileSize);
			
			fp_save=fopen(acPacSaveName, "a+");
			if(NULL!=fp_save)
			{
				fp_tmp=fopen(LOGSAVE_SAVE_TMPPATH, "r");
				if(NULL!=fp_tmp)
				{
					read_len = fread(acReadBuff, 1, ulFileSize, fp_tmp);  
					//printf("[%s:%d] fread read_len=%d!\n",__func__,__LINE__,read_len);
					if(read_len > 0)
					{
						read_len = fwrite(acReadBuff, 1, ulFileSize, fp_save);  
						//printf("[%s:%d] fwrite read_len=%d!\n",__func__,__LINE__,read_len);
					}
					fclose(fp_tmp);
					//printf("[%s:%d] fclose(fp_tmp);\n",__func__,__LINE__);
				}else{
					//printf("[%s:%d] %s fopen failed !\n",__func__,__LINE__,LOGSAVE_SAVE_TMPPATH);
				}
				
				fclose(fp_save);
    			system("rm -rf "LOGSAVE_SAVE_TMPPATH);
				//printf("[%s:%d] fclose(fp_save);\n",__func__,__LINE__);
			}else{
				//printf("[%s:%d] %s fopen failed !\n",__func__,__LINE__,acPacSaveName);
			}
			
			//printf("[%s:%d] save ok \n",__func__,__LINE__);
		}
		sleep(10);
    }
		
}

int main(int argc, char **argv)
{
    int iRet = 0;
	
	char c;
	int option_index = 1;	
	static const char short_options[] = "s:";
    static const struct option long_options[] = {
        { "s",           			1, 0, 's' },    // save locallog size
        { "help",          			0, 0, 'h' },
        { NULL,         			0, 0,  0  }
	};
	while (1) 
	{	    
		c = getopt_long(argc, argv, short_options, long_options, &option_index);
		if (c == -1) {
			break;
		}
		switch (c) {
        case 's':
            iLogEachSize = atoi(optarg)*1024;
            break;   
		default:
			//help 
		    ;
		}
    }
    iRet = logsave_create_save_file();
    if(0!=iRet)
    {
    	printf("Couldn't find default save path: %s\n", LOGSAVE_SAVE_LOGPATH);
    	syslog(LOG_ERR,"[%s:%d] Couldn't find default save path: %s \n", __func__, __LINE__,LOGSAVE_SAVE_LOGPATH);
    }
    
	logsave_loop_get_packet();
    
    return 0;
}

