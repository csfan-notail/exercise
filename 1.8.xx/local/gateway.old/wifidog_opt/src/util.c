/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : util.c
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/11/25
  Last Modified :
  Description   : 
  Function List :
  History       :
  1.Date        : 2013/11/25
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <stdarg.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <netdb.h>
#include <unistd.h>
#include "cJSON.h"
#include "includes.h"
#include "log.h"
#include "util.h"
#include "conf.h"
#include "pal_uci.h"

int g_online_state;

static unsigned char gethexval(char c)
{
	unsigned char  temp = c;

	if(temp >='0' && temp <= '9')
	{
		temp -= '0';
	}
	else if(temp >= 'a' && temp <= 'z')
	{
			temp = temp-'a'+10;
	}
	else if(temp >= 'A' && temp <= 'Z')
	{
			temp = temp-'A'+10;
	}
	else 
	{
		temp = 0;
	}

    return temp;
}

static unsigned char atoh(char * str)
{
	unsigned char hex,temp;
	hex = 0;
	temp = *str;
	temp = gethexval(temp);
	hex = temp << 4;

	temp = *(str+1);
	temp = gethexval(temp);
	hex += temp;

    return hex;
} 

int get_mac(char* file_mac,unsigned char *pMac)
{
    char sMac[20];
    unsigned char i;
    char *start,*next;

    if(NULL == pMac)
    {
    		return 0;
    }
    strcpy(sMac,file_mac);

    start = sMac;
    next = sMac;
    i = 0;

    while(i < 6)
    {
    	next++;
    	if(*next == ':' || *next =='\0')
    	{
    		*next = '\0';
    		pMac[i] = atoh(start);

    		next++;
    		i++;
    		start = next;
    	}
    }
    return 1;
}


int get_ip(char* file_ip,unsigned char *pIpAddr)
{
    char sIpAddr[32];
    unsigned char i;
    char *start,*next;

    if(NULL == file_ip)
    {
        return -1;
    }
    strcpy(sIpAddr,file_ip);

    start = sIpAddr;
    next = sIpAddr;
    i = 0;

    while(i < 4)
    {
        next++;
        if(*next == '.' || *next =='\0')
        {
            *next = '\0';
            pIpAddr[i] = atoi(start);

            next++;
            i++;
            start = next;
        }
    }

    return 0;
}

char *safe_strdup(const char *s)
{
    char *retval = NULL;
    if(!s)
    {
        log_info(__LOG_FAILED__, "safe_strdup called with NULL which would have crashed strdup. Bailing out");
		sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at safe_strdup()");
		exit(1);
    }

    retval = strdup(s);
    if(!retval)
    {
        log_info(__LOG_FAILED__, "Failed to duplicate a string: %s.  Bailing out", strerror(errno));
		sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at safe_strdup() %s", strerror(errno));
        exit(1);
    }
    return retval;
}

void *safe_malloc(size_t size)
{
    void *retval = NULL;
	retval = malloc(size);
	if (!retval) {
		log_info(__LOG_FAILED__, "Failed to malloc %d bytes of memory: %s.  Bailing out", size, strerror(errno));
		sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at safe_malloc()");
		exit(1);
	}

	memset(retval, 0, size);
	return (retval);
}

pid_t safe_fork(void) 
{
	pid_t result;
	result = fork();

	if (result == -1) {
		log_info(__LOG_FAILED__, "Failed to fork: %s.  Bailing out", strerror(errno));
		sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at safe_fork()");
		exit (1);
	}
	return result;
}

char *get_iface_ip(const char *ifname)
{
	struct ifreq if_data;
	struct in_addr in;
	char *ip_str;
	int sockd;
	u_int32_t ip;

	/* Create a socket */
	if ((sockd = socket (AF_INET, SOCK_PACKET, htons(0x8086))) < 0) {
		log_info(__LOG_ERROR__, "socket(): %s", strerror(errno));
		return NULL;
	}

	/* Get IP of internal interface */
	strcpy (if_data.ifr_name, ifname);
    //if_data.ifr_name[strlen(if_data.ifr_name)-1] = '\0';

	/* Get the IP address */
	if (ioctl (sockd, SIOCGIFADDR, &if_data) < 0) {
		log_info(__LOG_ERROR__, "ioctl(): SIOCGIFADDR %s, sockd %d", strerror(errno), sockd);
		return NULL;
	}
	memcpy ((void *) &ip, (void *) &if_data.ifr_addr.sa_data + 2, 4);
	in.s_addr = ip;

	ip_str = inet_ntoa(in);
	close(sockd);
	return safe_strdup(ip_str);
}

char *get_iface_mac(const char *ifname)
{
	int r, s;
	struct ifreq ifr;
	char *hwaddr, mac[18];

	strcpy(ifr.ifr_name, ifname);

	s = socket(PF_INET, SOCK_DGRAM, 0);
	if (-1 == s) {
		log_info(__LOG_ERROR__, "get_iface_mac socket: %s", strerror(errno));
		return NULL;
	}

	r = ioctl(s, SIOCGIFHWADDR, &ifr);
	if (r == -1) {
		log_info(__LOG_ERROR__, "get_iface_mac ioctl(SIOCGIFHWADDR): %s", strerror(errno));
		close(s);
		return NULL;
	}

	hwaddr = ifr.ifr_hwaddr.sa_data;
	close(s);
	snprintf(mac, sizeof(mac), "%02x:%02x:%02x:%02x:%02x:%02x", 
			hwaddr[0] & 0xff,
			hwaddr[1] & 0xff,
			hwaddr[2] & 0xff,
			hwaddr[3] & 0xff,
			hwaddr[4] & 0xff,
			hwaddr[5] & 0xff
		);

	return safe_strdup(mac);
}

struct in_addr *wd_gethostbyname(const char *name)
{
	struct hostent *he;
	struct in_addr *h_addr, *in_addr_temp;

	/* XXX Calling function is reponsible for free() */
	h_addr = safe_malloc(sizeof(struct in_addr));
	he = gethostbyname(name);
	if (he == NULL) {
		free(h_addr);
		log_info(__LOG_ERROR__, "gethostbyname(%s) return NULL, error", name);
		return NULL;
	}
	in_addr_temp = (struct in_addr *)he->h_addr_list[0];
	h_addr->s_addr = in_addr_temp->s_addr;
	return h_addr;
}

int get_end_ip(const char *ip)
{
    unsigned int tmp_ip[4];

    memset(tmp_ip, 0, sizeof(int)*4);
    if(4 != sscanf(ip, "%d.%d.%d.%d", &tmp_ip[0], &tmp_ip[1], &tmp_ip[2], &tmp_ip[3]))
    {
        log_info(__LOG_ERROR__, "Could not get ip");
        return -1;
    }
    return tmp_ip[3];
}

int update_max_fd(int maxfd, int fd)
{
    return fd > maxfd ? fd : maxfd;
}

int safe_asprintf(char **strp, const char *fmt, ...) {
	va_list ap;
	int retval;

	va_start(ap, fmt);
	retval = safe_vasprintf(strp, fmt, ap);
	va_end(ap);

	return (retval);
}

int safe_vasprintf(char **strp, const char *fmt, va_list ap) {
	int retval;

	retval = vasprintf(strp, fmt, ap);

	if (retval == -1) {
		log_info(__LOG_FAILED__, "Failed to vasprintf: %s.  Bailing out", strerror(errno));
		sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at safe_vasprintf()");
		exit (1);
	}
	return (retval);
}

int is_online(void)
{
    return g_online_state;
}

int set_is_online(int state)
{
    char command[128];

    sprintf(command, "echo %d >"WIFIDOG_STATUS, state);
    g_online_state = state;
    system(command);
    return 1;
}

int mac_converter(const char *src, unsigned char *dst)
{   
    int iRet;
    int mac[6];
    int loop;

	if(strstr(src, ":") != NULL){ /*XX:XX:XX:XX:XX:XX格式*/
	    iRet = sscanf(src, MAC_FMT, &mac[0],&mac[1],&mac[2],&mac[3],&mac[4],&mac[5]);
	    if(6 != iRet)
	    {
	        return 1;
	    }
	}
	else if(strstr(src, ".") != NULL){ /*XX.XX.XX.XX.XX.XX格式*/
	    iRet = sscanf(src, "%02x.%02x.%02x.%02x.%02x.%02x", &mac[0],&mac[1],&mac[2],&mac[3],&mac[4],&mac[5]);
	    if(6 != iRet)
	    {
	        return 1;
	    }		
	}
	else{ /*XXXXXXXXXXXX格式*/
		char tmp_mac[64];
		int idx = 0;
		if(strlen(src) != 12)
			return 1;
			
		for(loop=0; loop<12; ++loop){
			if(((loop % 2) == 0) && (loop != 0)){
				tmp_mac[idx++] = ':';
				tmp_mac[idx++] = src[loop];
			}
			else{
				tmp_mac[idx++] = src[loop];
			}
		}
		tmp_mac[idx] = '\0';
	    iRet = sscanf(tmp_mac, MAC_FMT, &mac[0],&mac[1],&mac[2],&mac[3],&mac[4],&mac[5]);
	    if(6 != iRet)
	    {
	        return 1;
	    }		
	}

    for(loop = 0; loop < 6; loop++)
    {
        dst[loop] = mac[loop];
    }
    return 0;
}

long get_uptime(void)
{
    struct sysinfo info;

    memset(&info, 0, sizeof(sysinfo));
    if(0 == sysinfo(&info))
    {
        return info.uptime;
    }
    return 0;
}

int parp_get(char *req_ip, char *dst_mac)
{
    FILE *proc;
	char ip[16];
	char mac[18];

    if (!(proc = fopen("/proc/net/arp", "r"))) {
        return 1;
    }

    /* Skip first line */
	while (!feof(proc) && fgetc(proc) != '\n');

	 /* Find ip, copy mac in reply */
    while (!feof(proc) && (fscanf(proc, " %15[0-9.] %*s %*s %17[A-Fa-f0-9:] %*s %*s", ip, mac) == 2)) {
		  if (strcmp(ip, req_ip) == 0) {
				strcpy(dst_mac, mac);
				break;
		  }
    }

    fclose(proc);
    return 0;
}

int ap_sta_auth_write(int idletimeout, int forcetimeout)
{
    FILE *pf;
    char buff[128];
    
    pf = fopen("/tmp/.auth_ap_sta", "w+");
    if(NULL == pf)
    {
        log_info(__LOG_DEBUG__, "open /tmp/.auth_ap_sta failed");
        return -1;
    }
    memset(buff, 0, sizeof(buff));
    sprintf(buff, "%d,%d", idletimeout, forcetimeout);
    fwrite(buff, strlen(buff), 1, pf);
    fclose(pf);
    return 0;
}


int tools_str_to_num_list(char *list_str,unsigned char *list_num,unsigned int *iTotal,int iMax)
{
	char *start                     = NULL;
	char buf[STRING_LEN_EX+1]  = "";
	int i                           = 0;
	
	memcpy(buf,list_str,STRING_LEN_EX);
	start = buf;
	for( i = 0; i < STRING_LEN_EX; i++ )
	{
	    if(*iTotal == iMax)
        {
            break;
        }   
		if(',' == buf[i])
		{
			buf[i]='\0';
			list_num[*iTotal]=atoi(start);
			(*iTotal)++;
			start = &buf[i+1];
		}
 		else if('\0' == buf[i])
		{
			list_num[*iTotal]=atoi(start);
            if(0 != i)
            {
    			(*iTotal)++;
            }
			break;
		}
	}
	return 0;
}

int tools_nvram_list_add(char *pcListName, char *pcNumName, char *pcListHead, unsigned int *ulCount,unsigned int *ulNodeNo, unsigned int ulMaxNum, char *pucValue)
{
    char acTmpBuf[NVRAM_VALUE_MAX_LEN]={0};
    char acListStr[NVRAM_VALUE_MAX_LEN]={0};
    char acListNew[NVRAM_VALUE_MAX_LEN]={0};
    char acName[NVRAM_VALUE_MAX_LEN]={0};
    char acValue[NVRAM_VALUE_MAX_LEN]={0};
    unsigned char Numlist[LIST_NUM_MAX]={0};
    //unsigned int ulVapCount=0;
    unsigned int ulLoop=0;
    unsigned int ulVapId=0;
    nvram_get(acListStr, pcListName, NVRAM_VALUE_MAX_LEN);
    //nvram_get(acTmpBuf, NVRAM_WIFI_VAP_NUM, NVRAM_VALUE_MAX_LEN);
    tools_str_to_num_list(acListStr, Numlist, ulCount, LIST_NUM_MAX);
    //ulVapCount=atoi(acTmpBuf);

    if(ulMaxNum == *ulCount)
    {
        return 1;/*已满*/
    }
    for(ulLoop = 0; ulLoop < *ulCount; ulLoop++)
    {
        sprintf(acName,"%s%u",pcListHead,Numlist[ulLoop]);
        if(0 != nvram_get(acValue,acName,NVRAM_VALUE_MAX_LEN))
        {
            continue;
        }
        if(0 == strcmp(acValue,pucValue))
        {
            return 0;//已有记录，不需要添加
        }
    }
    /*新增加VAP在原VAP列表中的位置判断*/
    for(ulVapId= 1; ulVapId <= ulMaxNum; ulVapId++)
    {
        for(ulLoop = 0; ulLoop < *ulCount; ulLoop++)
        {
            if(ulVapId == Numlist[ulLoop])
            {
                break;
            }
        }
        /*如果循环到结束，仍然没有相同的，则就采用该VAPID*/
        if(ulLoop == *ulCount)
        {
            break;
        }
    }
    /*将新的VAP加入到列表末尾*/
    Numlist[*ulCount]=ulVapId;
    *ulNodeNo=ulVapId;
    (*ulCount)++;

    /*将新VAP列表设置到NVRAM*/
    //sprintf(acBuf,"%d",1);
    for(ulLoop = 0; ulLoop < *ulCount; ulLoop++)
    {
        if(0 == ulLoop)
        {
            sprintf(acTmpBuf,"%d",Numlist[ulLoop]);
        }
        else
        {
            sprintf(acTmpBuf,",%d",Numlist[ulLoop]);
        }
        strcat(acListNew,acTmpBuf);
    }

    nvram_set(pcListName, acListNew);
    sprintf(acTmpBuf,"%u",*ulCount);
    nvram_set(pcNumName,acTmpBuf);
    sprintf(acName,"%s%u",pcListHead,*ulNodeNo);
    nvram_set(acName,pucValue);

    return 0;
}

int tools_nvram_list_del(char *pcListName, char *pcNumName, char *pcListHead, unsigned int *ulCount,char *pucValue)
{
    char acTmpBuf[NVRAM_VALUE_MAX_LEN]={0};
    char acName[NVRAM_VALUE_MAX_LEN]={0};
    char acValue[NVRAM_VALUE_MAX_LEN]={0};
    char acListStr[NVRAM_VALUE_MAX_LEN]={0};
    char acListNew[NVRAM_VALUE_MAX_LEN]={0};
    unsigned char Numlist[LIST_NUM_MAX]={0};
    unsigned int  ulIndex=0;
    //unsigned int ulVapCount=0;
    unsigned int ulLoop=0;
    unsigned int ulTotal=0;
    nvram_get(acListStr, pcListName, NVRAM_VALUE_MAX_LEN);
    //nvram_get(acTmpBuf, NVRAM_WIFI_VAP_NUM, NVRAM_VALUE_MAX_LEN);
    tools_str_to_num_list(acListStr,Numlist, ulCount,LIST_NUM_MAX);
    ulTotal=*ulCount;
    /*删除nvram中该VAP的列表信息*/
    for(ulLoop = 0; ulLoop < ulTotal; ulLoop++)
    {
        sprintf(acName,"%s%u",pcListHead,Numlist[ulLoop]);
        if(0 != nvram_get(acValue,acName,NVRAM_VALUE_MAX_LEN))
        {
            ulIndex++;
            (*ulCount)--;
            continue;
        }
        /*遇到需要删除的节点，则不放到列表中，跳过*/
        if(0 == strcmp(acValue,pucValue))
        {
            nvram_unset(acName);
            ulIndex++;
            (*ulCount)--;
            continue;
        }
        if(ulIndex == ulLoop)
        {
            sprintf(acTmpBuf,"%d",Numlist[ulLoop]);
        }
        else
        {
            sprintf(acTmpBuf,",%d",Numlist[ulLoop]);
        }
        strcat(acListNew,acTmpBuf);
    }
    nvram_set(pcListName,acListNew);
    sprintf(acTmpBuf,"%u",*ulCount);
    nvram_set(pcNumName,acTmpBuf);
    return 0;
}

int tools_nvram_list_get(char *pcListName, char *pcNumName, char *pcListHead, unsigned int *ulCount,char acListValue[LIST_NUM_MAX][NVRAM_VALUE_MAX_LEN])
{
    char acName[NVRAM_VALUE_MAX_LEN]={0};
    char acListStr[NVRAM_VALUE_MAX_LEN]={0};
    unsigned char Numlist[LIST_NUM_MAX]={0};
    //unsigned int ulVapCount=0;
    unsigned int ulLoop=0;

    nvram_get(acListStr, pcListName, NVRAM_VALUE_MAX_LEN);
    //nvram_get(acTmpBuf, NVRAM_WIFI_VAP_NUM, NVRAM_VALUE_MAX_LEN);
    tools_str_to_num_list(acListStr,Numlist, ulCount,LIST_NUM_MAX);
    /*删除nvram中该VAP的列表信息*/
    for(ulLoop = 0; ulLoop < *ulCount; ulLoop++)
    {
        sprintf(acName,"%s%u",pcListHead,Numlist[ulLoop]);
        nvram_get(acListValue[ulLoop],acName,NVRAM_VALUE_MAX_LEN);
    }
    return 0;
}

int tools_nvram_listnode_get(char *pcListName, char *pcNumName, char *pcListHead, unsigned int *ulCount,char *pucValue)
{
    char acName[NVRAM_VALUE_MAX_LEN]={0};
    char acValue[NVRAM_VALUE_MAX_LEN]={0};
    char acListStr[NVRAM_VALUE_MAX_LEN]={0};
    unsigned char Numlist[LIST_NUM_MAX]={0};
    //unsigned int ulVapCount=0;
    unsigned int ulLoop=0;
    nvram_get(acListStr, pcListName, NVRAM_VALUE_MAX_LEN);
    //nvram_get(acTmpBuf, NVRAM_WIFI_VAP_NUM, NVRAM_VALUE_MAX_LEN);
    tools_str_to_num_list(acListStr,Numlist, ulCount,LIST_NUM_MAX);
    /*删除nvram中该VAP的列表信息*/
    for(ulLoop = 0; ulLoop < *ulCount; ulLoop++)
    {
        sprintf(acName,"%s%u",pcListHead,Numlist[ulLoop]);
        if(0 != nvram_get(acValue,acName,NVRAM_VALUE_MAX_LEN))
        {
            continue;
        }
        /*遇到需要删除的节点，则不放到列表中，跳过*/
        if(0 == strcmp(acValue,pucValue))
        {
            return Numlist[ulLoop];
        }
    }
    return 0;
}



/*****************************************************************************
 函 数 名  : 
 功能描述  : 
 输入参数  : 
 输出参数  : 
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史  :

*****************************************************************************/
int http_get_url_info(char* requestpath, char* host,unsigned short port, char *header, char *post_data,
                                REQ_TYPE_E reqType, pFunc pfunc,void* out)
{	
	unsigned int ret = -1;
	
	char out_file_name[64] = {0};

	/* 系统命令 */
	char acCmd[1024*20] = {0};

	FILE *f=NULL;
	unsigned int len=0;
	
    time_t              stCurrTime;
    static struct tm    stLocalTime;
	char *data=NULL;

	/* BEGIN:2014-09-02 fengjing add */
	/* 增加检测外网是否通 */
	struct in_addr *h_addr;	
	h_addr = wd_gethostbyname(host);
	if(!h_addr)
    {
        //TODO ... 无法解析服务器流程处理
        log_info(LOG_ERR, "Marking lianlian server [%s] as bad", host);
        return 1;
    }
    else
    {
    	free(h_addr);
    }
	/* END:2014-09-02 fengjing add */
    stCurrTime = time(NULL);
    memcpy(&stLocalTime, localtime(&stCurrTime), sizeof(stLocalTime));
	
    sprintf(out_file_name, "/tmp/%04d%02d%02d%02d%02d%02d.dat",
            (1900+stLocalTime.tm_year),
            (1+stLocalTime.tm_mon),
            stLocalTime.tm_mday,
            stLocalTime.tm_hour,
            stLocalTime.tm_min,
            stLocalTime.tm_sec);

	/* 区分http和https下载方式 */
	/* 发送http请求 */
	/* curl下载带限速 */
	/* curl -o localfile --limit-rate 1000B  url */
	sprintf(acCmd, "%s%s -d \'%s%s\' http://%s:%d%s %s",
#if (defined(GW_X86) || defined(GW_ARM) || defined(GW_AC))
			"curl -o ",
#else
			"curl -o ",
//				"/mnt/sd/bin/curl -o ",
#endif
			out_file_name,
			header,post_data,
			host,
			port,
			requestpath,
			(reqType == HTTP_REQ_TYPE_ASYNC)? " 1>/dev/null 2>&1 &":"");

	log_info(LOG_INFO, "%s\n", acCmd);
	
	system(acCmd);
	
	log_info(LOG_INFO, "send http request %s \n", requestpath);
	if (NULL == pfunc)
	{
		goto fail;
	}
#if 1
    if(NULL == (f=fopen(out_file_name,"rb")))
    {
		goto fail;
    }
	fseek(f,0,SEEK_END);

	len=ftell(f);
	fseek(f,0,SEEK_SET);

	data =(char*)malloc(len+1);
	fread(data,1,len,f);
	data[len] = '\0';

	fclose(f);
#else
	data =(char*)malloc(1024);
    //strcpy(data,"{\"result_code\":0,\"request_type\":\"list\",\"request_id\":2,\"data\":[{\"dir_name\":\"httpd/\"},{\"dir_name\":\"liwen/\"}]}");
    strcpy(data,"{\"result_code\":0,\"request_type\":\"sync\",\"request_id\":\"2\",\"priority\":2,\"data\":[{\"resource_id\":0,\"process_type\":2,\"md5\":\"dsadsadas\",\"dir_name\":\"httpd/test/\",\"file_name\":\"tess.txt\",\"file_url\":\"http://testmh.cwifi.cn:8080/asdsa.txt\",\"file_method\":0},{\"resource_id\":0,\"process_type\":2,\"md5\":\"dsadsadas\",\"dir_name\":\"liwen/liwen2/\",\"file_name\":\"liwen.txt\",\"file_url\":\"http://testmh.cwifi.cn:8080/asdsaa.txt\",\"file_method\":0}]}");
    //strcpy(data,"{\"result_code\":0}");
#endif
    
	log_info(LOG_INFO, "%s\n", data);

	
	cJSON *json=NULL;
	
	/* JSON报文解析 */
	json=cJSON_Parse(data);
	if (!json) 
	{
		log_info(LOG_ERR, "Error before: [%s]\n", cJSON_GetErrorPtr());
		goto fail;
	}
	else
	{
		/*out=cJSON_Print(json);
		cJSON_Delete(json);
		printf("%s\n",out);
		free(out);*/
		
		/* 待实现 */
		ret = pfunc(json, out);
		
		cJSON_Delete(json);
	}

	free(data);

	if (out_file_name[0] != '\0')
	{
		remove(out_file_name);
	}
    return 0;
	
fail:
	if(NULL != data)
	{
		free(data);
	}
	if (out_file_name[0] != '\0')
	{
		remove(out_file_name);
	}
    return 1;
}

int get_local_time(char *pacTime)
{
    time_t t={0};
    struct tm *ltime=NULL;
    time(&t);
    if(NULL == (ltime=localtime(&t)))
    {
        return 1;
    }
    sprintf(pacTime,"%d-%02d-%02d %02d:%02d:%02d",
        ltime->tm_year+1900,
        ltime->tm_mon+1,
        ltime->tm_mday,
        ltime->tm_hour,
        ltime->tm_min,
        ltime->tm_sec
        );
    return 0;
}


int strmac_to_mac(char *str_mac, unsigned char *mac)
{
	int i;
	unsigned char val1 = 0, val2 = 0;
	
	/*str_mac: xx:xx:xx:xx:xx:xx*/
	for(i=0; i<17; ++i)
	{
		switch(i%3)
		{
		case 0:	
			if((str_mac[i] >= 'a') && (str_mac[i] <= 'f')){
				val2 = str_mac[i] - 'a' + 0xA;
			}
			else if((str_mac[i] >= 'A') && (str_mac[i] <= 'F')){
				val2 = str_mac[i] - 'A' + 0xA;
			}
			else if((str_mac[i] >= '0') && (str_mac[i] <= '9')){
				val2 = str_mac[i] - '0';
			}
			else
				return -1;
			break;
		case 1:
			if((str_mac[i] >= 'a') && (str_mac[i] <= 'f')){
				val1 = str_mac[i] - 'a' + 0xA;
			}
			else if((str_mac[i] >= 'A') && (str_mac[i] <= 'F')){
				val1 = str_mac[i] - 'A' + 0xA;
			}
			else if((str_mac[i] >= '0') && (str_mac[i] <= '9')){
				val1 = str_mac[i] - '0';
			}
			else
				return -1;
			if(i<2)
				mac[0] = val1 + val2*16;
			else if(i<5)
				mac[1] = val1 + val2*16;
			else if(i<8)
				mac[2] = val1 + val2*16;
			else if(i<11)
				mac[3] = val1 + val2*16;
			else if(i<14)
				mac[4] = val1 + val2*16;
			else
				mac[5] = val1 + val2*16;
			break;
		case 2:
			if(str_mac[i] != ':')
				return -1;
			break;
		default:
			break;
		}
	}

	return 0; 
}

