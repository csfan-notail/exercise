#define _LARGEFILE_SOURCE
#define _LARGEFILE64_SOURCE
#define _FILE_OFFSET_BITS 64
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
#include <sys/types.h>
#include <fcntl.h>
#include <linux/kernel.h>  
#include <sys/vfs.h>
#include <sys/sysinfo.h>
#include <netinet/if_ether.h>  
#include <netinet/ip.h>  
#include <netinet/udp.h>  
#include <netinet/tcp.h>  
#include <netinet/ip_icmp.h> 

#include "ap_types.h"

#define HAVE_VSNPRINTF 1
#define HAVE_SNPRINTF 1
#include "pcap-int.h"

#define EDP_PCK_SAVE_PATH 				"/mnt/emmc/packet"
#define EDP_PCK_TMP_PATH 				"/tmp/emmc"
#define EDP_PCK_ADD_PCAP_HEAD 				1
#define EDP_PCK_STRUCT_MAX_SIZE				10240
#define EDP_PAC_SYS_RAM_SIZE_MIN			30	//m
#define EDP_PCK_LOOP_NUM	1000
#define LOGSAVE_SAVE_DEL_SIZE 			1//1G delete log file


static int iNewCreateSize = 20; 		//M
static int iNewCreateTime = 5;			//min

#define EDP_TMP_PAC_MAX_SIZE				5//5M
#define EDP_TMP_PAC_MV_NAME					"/tmp/emmc/CBTC_TMP_CAT_PAC.pcap"

time_t edp_file_create_time_lan;			//br-lan PC to usb0 :up 	tx

static unsigned int write_total_size;

pcap_t *handle;

static FILE *fp_lan = NULL;

static char acPacSaveTime[32] = {0};		// emmc save packet wan name
static char acPacSaveNameLan[128] = {0};	// emmc save packet lan name

static char acPacSaveEmmcName[128] = {0};	// emmc save packet lan name

static char acCfgIpLan[64] = {0};			// emmc save lan ip addr
static char acCfgIpWan[64] = {0};			// emmc save wan ip addr

static int iTmpNumLan =1;					//tmp lan remote file number
static int iCount = 0;						//get pcaket write cache 

int ethdump_log_sw = 0; 					//log off/on 0:off 1:on


struct sniff_ethernet {  
        u_char  ether_dhost[ETHER_ADDR_LEN];    /* destination host address */  
        u_char  ether_shost[ETHER_ADDR_LEN];    /* source host address */  
        u_short ether_type;                     /* IP? ARP? RARP? etc */  
};
struct sniff_ip {  
        u_char ip_vhl; /* version << 4 | header length >> 2 */  
        u_char ip_tos; /* type of service */  
        u_short ip_len; /* total length */  
        u_short ip_id; /* identification */  
        u_short ip_off; /* fragment offset field */  
        #define IP_RF 0x8000 /* reserved fragment flag */  
        #define IP_DF 0x4000 /* dont fragment flag */  
        #define IP_MF 0x2000 /* more fragments flag */  
        #define IP_OFFMASK 0x1fff /* mask for fragmenting bits */  
        u_char ip_ttl; /* time to live */  
        u_char ip_p; /* protocol */  
        u_short ip_sum; /* checksum */  
        struct in_addr ip_src,ip_dst; /* source and dest address */  
};
typedef struct
{
	int  iDatePackSize;
	struct pcap_pkthdr stPkthdr;
	char acDatePack[1600];
}ETHDUMP_DATA_PACKET_T;

ETHDUMP_DATA_PACKET_T astDatePack[EDP_PCK_STRUCT_MAX_SIZE] = {{0}};

#define ETH_LOG(format, arg...) \
    do{ \
        if(ethdump_log_sw)	\
        	printf("[%25s:%5d] "format,__func__,__LINE__, ##arg);\
    }while(0)

long long ethdump_get_space_size(char *pcPath)  
{  
    if(pcPath == NULL)  
    {	
    	ETH_LOG(" get space size failed %s\n",pcPath);
		return -1;
    }
    
    ETH_LOG(" pcPath=%s \n", pcPath);
    struct statfs diskInfo;  
    statfs(pcPath,&diskInfo);  
	
	unsigned long long blocksize = diskInfo.f_bsize;
    unsigned long long totalsize = blocksize * diskInfo.f_blocks;
    ETH_LOG("Total_size = %llu B = %llu KB = %llu MB = %llu GB\n",   
        totalsize, totalsize>>10, totalsize>>20, totalsize>>30);  
      
    unsigned long long freeDisk = diskInfo.f_bfree * blocksize; 
    unsigned long long availableDisk = diskInfo.f_bavail * blocksize; 
    ETH_LOG("Disk_free = %llu MB = %llu GB Disk_available = %llu MB = %llu GB\n",   
        freeDisk>>20, freeDisk>>30, availableDisk>>20, availableDisk>>30); 

    return availableDisk>>20;  
} 

static void ethdump_sig_exit(int sig)
{
    if(fp_lan){
        fflush(fp_lan);
    } 
    pcap_close(handle);
	
    signal(sig, SIG_DFL);
    kill(getpid(),sig);
    
    return;
}


int RandomInRange(int min, int max) {  
    int random = rand() % (max-min+1) + min;  
    return random;  
}  
  
void Swap (long long* num1, long long* num2) {  
    long long temp = *num1;  
    *num1 = *num2;  
    *num2 = temp;  
}  
  
int Partition(long long data[], int length, int start, int end) {  
    if (data == NULL || length <= 0 || start < 0 || end >= length)  
        return -1;  
    int index = RandomInRange(start, end);  
    Swap(&data[index], &data[end]);  
    int small = start-1;  
    for (index = start; index < end; ++index) {  
        if (data[index] < data[end]) {  
            small++;  
            if (small != index)  
                Swap(&data[small], &data[index]);  
        }  
    }  
    small++;  
    Swap(&data[small], &data[end]);  
    return small;  
}  
  
void GetLeastNumbers(long long* input, int n, long long* output, int k) {  
    if (input == NULL || n <= 0 || output == NULL || k <= 0 || k>n)  
        return;  
    int start = 0;  
    int end = n-1;  
    int i=0;
    int index = Partition(input, n, start, end);  
    while (index != k-1) {  
        if (index > k-1) {  
            end = index - 1;  
            index = Partition(input, n, start, end);  
        } else {  
            start = index + 1;  
            index = Partition(input, n, start, end);  
        }  
    }  
    for (i=0; i<k; ++i) {  
        output[i] = input[i];  
    }  
}
void logsave_popen_get_val(char *acCmd, char *retval)
{
	FILE *fp_p = NULL;
	char acBuff[32] = {0};
	fp_p = popen(acCmd,"r");
	if(fp_p){
		fgets(acBuff,32,fp_p);
		acBuff[strlen(acBuff)-1] = '\0';
		strcpy(retval,acBuff);
		//printf("[%s:%d] acBuff=%s retval=%s\n",__FUNCTION__,__LINE__,acBuff,retval);	
	}
	pclose(fp_p);
}
int ethdump_delete_log_file(char *pcPath)
{
	DIR *dirptr = NULL;  
	struct dirent *entry; 
	char *pcGetNum;
	int iTmpNum = 99999999;
	int iLoop = 0;
	int iDelFileNum = 0;
	char acDelFile[64] = {0};
	char acCmd[64] = {0};

	
	char acRetval[32] = {0};
	logsave_popen_get_val("ls /mnt/emmc/packet/  | grep CBTC_ | wc -l",acRetval);
	ETH_LOG("get all log number : %d \n", atoi(acRetval));
	
	if(atoi(acRetval) <= 0){
		ETH_LOG("log file not find ! num: %d \n", atoi(acRetval));
		//syslog(LOG_ERR,"[%s:%d] log file not find!\n", __func__, __LINE__);
		return -1;  
	}
	
	long long *alllog = NULL;
	alllog = (long long *)malloc(sizeof(long long)*atoi(acRetval)+1);
	
	if(NULL == alllog){
		ETH_LOG("malloc failed! ");
		//syslog(LOG_ERR,"[%s:%d] malloc failed!\n", __func__, __LINE__);
		return -1;  
	}
	long long get_date = 0;
	int time = 0;
	dirptr = opendir(pcPath);
	if( dirptr == NULL)
	{  
		free(alllog);	
		ETH_LOG("open dir failed! dir: %s", pcPath);
		//syslog(LOG_ERR,"[%s:%d] open dir failed! %s\n", __func__, __LINE__,pcPath);
		return -1;  
	}else{  
		while (entry = readdir(dirptr))  
		{  
			if(strcmp(entry->d_name,".") && strcmp(entry->d_name,".."))
			{
				//printf("[%s:%d] entry->d_name=%s \n", __func__, __LINE__, entry->d_name);
				pcGetNum=strstr(entry->d_name,"CBTC_");
				if(NULL==pcGetNum)
					continue;
				//pcGetNum++;
				//if(iTmpNum > atoi(pcGetNum))
				//{
				//	iTmpNum = atoi(pcGetNum);
				//}
								
				sscanf(entry->d_name,"CBTC_%lld.pcap",&get_date);
				//printf("%d:%lld \n",time, get_date);
				alllog[time] = get_date;
				time++;
				
			}
		}
		closedir(dirptr);
	}

    long long del_log[10];  
    GetLeastNumbers(alllog, atoi(acRetval), del_log, 10); 
    free(alllog);
    
	//delete 10 day log file
	for(iLoop = 0; iLoop < 10; iLoop++)
	{
		memset(acDelFile,0,sizeof(acDelFile));
		memset(acCmd,0,sizeof(acCmd));
		sprintf(acDelFile,"%s/CBTC_%lld.pcap",EDP_PCK_SAVE_PATH,del_log[iLoop]);
		sprintf(acCmd,"rm -rf %s",acDelFile);
		system(acCmd);
		ETH_LOG("delete log file :%s \n", acCmd);
	}
	
	return 0;
}


int ethdump_create_save_name(int iCreateFileNum, char *pcCreateFileName)
{
	char acCmd[128] = {0};
	char acName[128] = {0};
	
	snprintf(acName,128,"CBTC_%s.pcap",acPacSaveTime);
	sprintf(acCmd,"echo '%s' > /tmp/CBTC_name",acName);
	system(acCmd);
	
	//get emmc space size  
	unsigned long long dCurrSaveSize = 0;
	//unsigned long long dSaveSize = 60*1024*1024;
	dCurrSaveSize=ethdump_get_space_size(EDP_PCK_SAVE_PATH);
	ETH_LOG(" dCurrSaveSize=%llu [%d]\n",dCurrSaveSize,1024*LOGSAVE_SAVE_DEL_SIZE);
	if(dCurrSaveSize <= 1024*LOGSAVE_SAVE_DEL_SIZE)
	{
		ETH_LOG("save size < 1 G delete .pcap\n");
		//syslog(LOG_ERR,"[%s:%d] packet file creat space full :%f G\n", __func__, __LINE__,dCurrSaveSize/1024/1024/1024);
		ethdump_delete_log_file(EDP_PCK_SAVE_PATH);
		
	}
	
	memset(acCmd,0,128);
	sprintf(acCmd,"rm -rf "EDP_PCK_TMP_PATH"/*;mkdir "EDP_PCK_TMP_PATH";touch "EDP_PCK_TMP_PATH"/%s",acName);
	system(acCmd);
	ETH_LOG("acCmd=%s\n",acCmd);
	
	snprintf(pcCreateFileName,128,EDP_PCK_TMP_PATH"/%s",acName);
	
	snprintf(acPacSaveEmmcName,128,EDP_PCK_SAVE_PATH"/%s",acName);
	memset(acCmd,0,128);
	sprintf(acCmd,"touch %s",acPacSaveEmmcName);
	system(acCmd);
	ETH_LOG("acCmd=%s\n",acCmd);
	
	ETH_LOG("acPacSaveNameLan=%s\n",pcCreateFileName);
    ETH_LOG("acPacSaveEmmcName=%s\n",acPacSaveEmmcName);
    
	return 0;
}
static void ethdump_write_pcap_head(pcap_t *p, FILE *pFile)
{
	struct pcap_file_header hdr;
 	int linktype;
 	size_t ret;

	linktype = dlt_to_linktype(p->linktype);
	if (linktype == -1) {
		fprintf(stderr, "link-layer type %d isn't supported in savefiles", p->linktype);
		return ;
	}
	linktype |= p->linktype_ext;

	hdr.magic = 0xa1b2c3d4;
	hdr.version_major = PCAP_VERSION_MAJOR;
	hdr.version_minor = PCAP_VERSION_MINOR;

	hdr.thiszone = p->tzoff;
	hdr.snaplen = p->snapshot;
	hdr.sigfigs = 0x0000ffff;
	hdr.linktype = linktype;

	ret = fwrite(&hdr, 1, sizeof(hdr), pFile);
	if  ( ret != sizeof(hdr) )
	{
	    perror("write file err:");
	    //syslog(LOG_ERR,"[%s:%d] create save file add packet header error \n", __func__, __LINE__);
	    exit(-1);
	}

}
int ethdump_fopen_add_head(char *pcFopenFileName)
{
	FILE *pGetFp = NULL;
		
	pGetFp = fopen(pcFopenFileName, "w+");
	if( !pGetFp )
	{
		perror("Fopen add head error:");
		//syslog(LOG_ERR,"[%s:%d] file add header fopen %s failed \n", __func__, __LINE__,pcFopenFileName);
		exit(-1);
	}
	else
	{
		ethdump_write_pcap_head(handle, pGetFp);
	}
	fclose(pGetFp);
	
    return 0;
}
FILE* ethdump_fopen_add_packet(char *pcFopenFileName)
{
	FILE *pGetFp = NULL;
	
	pGetFp = fopen(pcFopenFileName, "w+");
	if( !pGetFp )
	{
		perror("Fopen add pack error:");
		//syslog(LOG_ERR,"[%s:%d] file add header fopen %s failed \n", __func__, __LINE__,pcFopenFileName);
		exit(-1);
	}
	
    return pGetFp;
}
/*
static void ethdump_packet_write_file(const struct pcap_pkthdr* pkthdr, const u_char* packet, FILE *pFile)
{
	size_t ret;
	int iFileSize =0;
	
	ret = fwrite(pkthdr, 1, sizeof(struct pcap_pkthdr), pFile);
    if  ( ret != sizeof(struct pcap_pkthdr) )
    {
        perror("Fwrite file err: ");	
        syslog(LOG_ERR,"[%s:%d] fwrite to pcap_pkthdr erroe\n", __func__, __LINE__);
        exit(-1);
    }
    ret = fwrite(packet, 1, pkthdr->caplen, pFile);
	if  ( ret != pkthdr->caplen )
	{
		perror("Fwrite file err: ");
		syslog(LOG_ERR,"[%s:%d] fwrite to pcap_pkthdr erroe\n", __func__, __LINE__);
	    exit(-1);
	}
}
*/
int ethdump_check_memory_size()
{
	struct sysinfo s_info;
	int error;
	error = sysinfo(&s_info);
	
  	if(s_info.freeram/1024/1024 < EDP_PAC_SYS_RAM_SIZE_MIN)
  	{
  		//printf("\n\ncode error=%d\n",error);  
  		//printf("[%s:%d] total:%lu free:%lu\n",__func__,__LINE__,s_info.totalram/1024/1024, s_info.freeram/1024/1024);
  		return -1;
  	}
  	
	return 0;
}
int ethdump_get_file_size(const char *path)  
{  
	int filesize = 0;      
    struct stat statbuff;  
    if(stat(path, &statbuff) < 0){  
        return filesize;  
    }else{  
        filesize = statbuff.st_size;  
    }  
    
    return filesize;
}  
 
void ethdump_pcap_callback(u_char *argument,const struct pcap_pkthdr* pkthdr,const u_char *packet)  
{  
    if(pkthdr->caplen )
    {
   		//printf("==================================================ethdump_pcap_callback: %d\n",iCount);   
   		astDatePack[iCount].iDatePackSize = pkthdr->caplen;
		memcpy(astDatePack[iCount].acDatePack,packet,pkthdr->caplen);
		
		astDatePack[iCount].stPkthdr.ts.tv_sec = pkthdr->ts.tv_sec;
		astDatePack[iCount].stPkthdr.ts.tv_usec = pkthdr->ts.tv_usec;
		astDatePack[iCount].stPkthdr.caplen = pkthdr->caplen;
		astDatePack[iCount].stPkthdr.len = pkthdr->len;
		
		iCount++;
		if(iCount == EDP_PCK_STRUCT_MAX_SIZE)
   			iCount = 0;
	}
}  

void ethdump_create_file_handle(FILE *fp_files,int iTimeDiff, int stCurrTime_t)
{
	time_t timep;
	struct tm *p;
	
	int iGetFileSize = 0;
	char acTouchCmd[128] = {0};
	char acCurrTime[128] = {0};
	char acMvCmd[128] = {0};
	char acCatCmd[128] = {0};
		
	//printf("[%s:%d] \n",__FUNCTION__,__LINE__); 
	
	//判断当前emmc待写入文件的大小
	//如果大小达到指定大小,或时间到达指定时间,则新建emmc待写文件
	FILE *pGetFp = NULL;
	pGetFp = fopen(acPacSaveEmmcName, "r");
	if( !pGetFp )
	{
		perror("Fopen save emmc error:");
		return ;
	}else{
		fseek(pGetFp, 0L, SEEK_END);
		iGetFileSize = ftell(pGetFp);
		fclose(pGetFp);
		//printf("[%s:%d] acPacSaveEmmcName=%s \n",__FUNCTION__,__LINE__,acPacSaveEmmcName);  
		//printf("[%s:%d] iGetFileSize=%d %d\n",__FUNCTION__,__LINE__,iGetFileSize,1024*1024*iNewCreateSize);  
		if(iTimeDiff >= 60*iNewCreateTime || \
		( iTimeDiff < 60*iNewCreateTime && iGetFileSize > 1024*1024*iNewCreateSize))
		{
			time(&timep);
			p=localtime(&timep);
			snprintf(acCurrTime,128,"%04d%02d%02d%02d%02d%02d", \
				(1900+p->tm_year),( 1+p->tm_mon), p->tm_mday,p->tm_hour, p->tm_min, p->tm_sec);
			
			//当前emmc待写文件为0不继续创建文件
			//会使时间分片不一致,即把两个时间段的空文件合并,如一分钟分片会有两分钟分片
			if(0 != iGetFileSize){
				snprintf(acPacSaveEmmcName,128,EDP_PCK_SAVE_PATH"/CBTC_%s.pcap ",acCurrTime);
				acPacSaveEmmcName[strlen(acPacSaveEmmcName)-1] = '\0';
				snprintf(acTouchCmd,128,"touch %s",acPacSaveEmmcName);
				system(acTouchCmd);
				
				ethdump_fopen_add_head(acPacSaveEmmcName);
				
				ETH_LOG("acTouchCmd=%s \n",acTouchCmd);  
			}
			
			//更新计时时间
			edp_file_create_time_lan = stCurrTime_t;
		}
		
	}
	
	//printf("[%s:%d] write_total_size=%d %d\n",__FUNCTION__,__LINE__,write_total_size,1024*1024*EDP_TMP_PAC_MAX_SIZE);  
	//内存临时写文件到达5M,或配置时间到达,写入emmc
	if(write_total_size > 1024*1024*EDP_TMP_PAC_MAX_SIZE)
	{
		fclose(fp_files);
		if(fp_files)
			fp_files=NULL;
		
		snprintf(acMvCmd,128,"mv %s %s", acPacSaveNameLan,EDP_TMP_PAC_MV_NAME);
		system(acMvCmd);
		ETH_LOG(" acMvCmd=%s \n",acMvCmd);  
		
		time(&timep);
		p=localtime(&timep);
		snprintf(acCurrTime,128,"%04d%02d%02d%02d%02d%02d", \
			(1900+p->tm_year),( 1+p->tm_mon), p->tm_mday,p->tm_hour, p->tm_min, p->tm_sec);
		
		snprintf(acCatCmd,128,"cat %s >> %s &", EDP_TMP_PAC_MV_NAME,acPacSaveEmmcName);
		system(acCatCmd);
		ETH_LOG(" acCatCmd=%s \n",acCatCmd); 
		
		//重新打开内存临时写文件
		fp_files=ethdump_fopen_add_packet(acPacSaveNameLan);
		
		//内存临时写文件大小计数清零
		write_total_size = 0;
	}
	
	
	//判断emmc剩余控件大小
	unsigned long long dCurrSaveSize = 0;
	dCurrSaveSize=ethdump_get_space_size(EDP_PCK_SAVE_PATH);
	
	ETH_LOG(" dCurrSaveSize=%llu [%d]\n",dCurrSaveSize,1024*LOGSAVE_SAVE_DEL_SIZE);
	if(dCurrSaveSize <= 1024*LOGSAVE_SAVE_DEL_SIZE)
	{
		ETH_LOG(" save size < 1 G delete .pcap\n");
		ethdump_delete_log_file(EDP_PCK_SAVE_PATH);
	}
	
}

void *ethdump_pthread_write()
{
	struct sniff_ip *ip; 
	int iNum = 0;
	char ip_src[32],ip_dst[32];
	// Add: by xiaowei, 2017/03/20 16:42 PN: get time difference
	int iTimeDiff_lan = 0;
	time_t stGetTime_t;
     
	while(1)
	{
		//printf("========================ethdump_pthread_write: %d\n",iCount);     
		for(iNum = 0; iNum < EDP_PCK_STRUCT_MAX_SIZE; iNum++)
		{
			stGetTime_t = time(NULL);
			iTimeDiff_lan = (int)abs(difftime(stGetTime_t, edp_file_create_time_lan));
   			//printf("[%s:%d] time difference: lan %d wan:%d seconds\n",__func__,__LINE__,iTimeDiff_lan,iTimeDiff_wan);
			
			if(astDatePack[iNum].iDatePackSize)	
			{
				//printf("[%d %d]\n",iNum,astDatePack[iNum].iDatePackSize);	
				ip = (struct sniff_ip*)(astDatePack[iNum].acDatePack+sizeof(struct sniff_ethernet));
				strncpy(ip_src,inet_ntoa(ip->ip_src),sizeof(ip_src));
				strncpy(ip_dst,inet_ntoa(ip->ip_dst),sizeof(ip_dst));  
				//printf("[%s:%d] Des IP:%15s \n",__FUNCTION__,__LINE__,inet_ntoa(ip->ip_dst)); 
				//printf("[%s:%d] Src IP:%15s \n",__FUNCTION__,__LINE__,inet_ntoa(ip->ip_src));  
				
				if(	(0 == strcmp(acCfgIpWan,ip_dst) && 0 == strcmp(acCfgIpLan,ip_src))
					|| (0 == strcmp(acCfgIpLan,ip_dst) && 0 == strcmp(acCfgIpWan,ip_src))
				)
				{
					ETH_LOG("Des IP:%15s Src IP:%15s\n",ip_dst,ip_src); 
					// Add: by xiaowei,2017/03/20 16:10 PN: 增加本地按照时间和大小分片
					ethdump_create_file_handle(fp_lan,iTimeDiff_lan,stGetTime_t);
					fwrite(&astDatePack[iNum].stPkthdr,1,sizeof(struct pcap_pkthdr),fp_lan);
				fwrite(&astDatePack[iNum].acDatePack,1,astDatePack[iNum].stPkthdr.caplen,fp_lan);
					
				}
				
				//当前已写如内存临时文件的总包大小
				write_total_size = write_total_size + astDatePack[iNum].iDatePackSize;
				astDatePack[iNum].iDatePackSize = 0;

			}else{
				iNum--;
				//ETH_LOG(" usleep(200*1000)\n");  
				usleep(200*1000);
			}
		}
		printf("\n");	
	}
	
	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    char *dev;          /* The device to sniff on */
    char errbuf[PCAP_ERRBUF_SIZE];  /* Error string */
    struct bpf_program fp;      /* The compiled filter */
    char filter_exp[128] = "";  /* The filter expression */
    bpf_u_int32 mask;       /* Our netmask */
    bpf_u_int32 net;        /* Our IP */
    char acInterface[32] = "br-lan";
    int iRet = 0;
    
	int option_index = 1;	
	char c;
    static const char short_options[] = "i:d:s:m:r:w:l:t:f:o:h";
    static const struct option long_options[] = {
        { "interface",      		1, 0, 'i' },    // 抓包端口
        { "dst host ip",            1, 0, 'd' },  	// 目的ip/url
        { "src host ip",            1, 0, 's' }, 	 // 源ip/url
        { "mac",           			1, 0, 'm' },    // 按mac过滤
        { "w",           			1, 0, 'w' },    // save int wan
        { "l",           			1, 0, 'l' },    // save int lan
        { "t",           			1, 0, 't' },    // create file time: min
        { "f",           			1, 0, 'f' },    // create file size: M
        { "o",           			1, 0, 'o' },    // create file size: M
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
		case 'i':
		    strncpy(acInterface, optarg, sizeof(acInterface));
			break;
        case 'm':
            snprintf(filter_exp,sizeof(filter_exp), "tcp or udp and ether host %s", optarg);
            break;
        case 'd':
            snprintf(filter_exp,sizeof(filter_exp), "dst host %s", optarg);
            break; 
        case 's':
            snprintf(filter_exp,sizeof(filter_exp), "src host %s", optarg);
            break;   
        case 'w':
        	snprintf(acCfgIpWan,32, "%s", optarg);
            break;
        case 'l':
        	snprintf(acCfgIpLan,32, "%s", optarg);
            break; 
        case 't':
        	iNewCreateTime = atoi(optarg);		//min
            break; 
        case 'f':
        	iNewCreateSize = atoi(optarg);		//M
            break;  
        case 'o':
        	ethdump_log_sw = atoi(optarg);		//debug printf on
            break;    
                              
		default:
			//help 
		    ;
		}
    }
    ETH_LOG("\n");	
    time_t timep;
	struct tm *p;
	time(&timep);
	p=localtime(&timep);
	
	snprintf(acPacSaveTime,128,"%04d%02d%02d%02d%02d%02d", \
			(1900+p->tm_year),( 1+p->tm_mon), p->tm_mday,p->tm_hour, p->tm_min, p->tm_sec);
	
    //create lan packet save file
    iRet = ethdump_create_save_name(iTmpNumLan, acPacSaveNameLan);
    if(0!=iRet)
    {
    //	printf("Couldn't find default save path: %s\n", EDP_PCK_SAVE_PATH);
    	//syslog(LOG_ERR,"[%s:%d] Couldn't find default save path: %s\n", __func__, __LINE__,EDP_PCK_SAVE_PATH);
    	return 0;
    }
    
    // Add: by xiaowei, 2017/03/20 16:39 PN: create local file time
    edp_file_create_time_lan = time(NULL);
    
    signal(SIGINT,  ethdump_sig_exit);
    signal(SIGTERM, ethdump_sig_exit);
    //printf("[%s:%d] \n",__FUNCTION__,__LINE__);	
    // Define the device 
    dev = pcap_lookupdev(errbuf);
    if (dev == NULL) {
    	printf("Couldn't find default device: %s\n", errbuf);
    	//syslog(LOG_ERR,"[%s:%d] Couldn't find default device: %s\n", __func__, __LINE__,errbuf);
        return 0;
    }
    // Find the properties for the device 
    if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
    	//printf("Couldn't get netmask for device %s: %s\n", dev, errbuf);
        net = 0;
        mask = 0;
    }
    // Open the session in promiscuous mode 
    handle = pcap_open_live(acInterface, BUFSIZ, 1, 1514, errbuf);
    if (handle == NULL) {
    	//printf("Couldn't open device %s: %s\n", dev, errbuf);
    	//syslog(LOG_ERR,"[%s:%d] Couldn't open device %s: %s\n", __func__, __LINE__,dev, errbuf);
        return 0;
    }
    // Compile and apply the filter 

    if (pcap_compile(handle, &fp, filter_exp, 1, net) == -1) {
    	//printf("Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
    	//syslog(LOG_ERR,"[%s:%d] Couldn't parse filter %s: %s\n", __func__, __LINE__,filter_exp, pcap_geterr(handle));
        return 0;
    }
    //syslog(LOG_ERR,"[%s:%d] curr filter_exp=%s\n", __func__, __LINE__,filter_exp);
    
    if (pcap_setfilter(handle, &fp) == -1) {
    	//printf("Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
    	//syslog(LOG_ERR,"[%s:%d] Couldn't install filter %s: %s\n", __func__, __LINE__,filter_exp, pcap_geterr(handle));
        return -1;
    }
	
	//save first file: add pcap head
	fp_lan=ethdump_fopen_add_packet(acPacSaveNameLan);
	ethdump_fopen_add_head(acPacSaveEmmcName);
	
	pthread_t thread[2] = {0};
	
	if((iRet = pthread_create(&thread[0], NULL, ethdump_pthread_write, NULL)) != 0) 
		ETH_LOG(" pthread_create failed !\n");
	else
		ETH_LOG(" pthread_create success !\n");
	
    pcap_loop(handle, 0, ethdump_pcap_callback, NULL);
    
    pcap_close(handle);
    
    return 0;
}
