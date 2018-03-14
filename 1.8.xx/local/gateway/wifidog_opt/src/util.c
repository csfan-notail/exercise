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
#include <netdb.h>
#include <unistd.h>

#include "includes.h"
#include "log.h"
#include "util.h"
#include "conf.h"
#include "client_list.h"

int g_online_state;

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

int get_iface_ip(const char *ifname, char *ifip, int len)
{
	struct ifreq ifr;
	int sockfd;
	struct sockaddr_in *wanaddr = NULL;

	if((NULL == ifname) || (NULL == ifip) || (len < 8))
		return -1;

	strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1);

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) <= 0)
		return -1;

	if(ioctl(sockfd, SIOCGIFADDR, &ifr) >= 0){
		wanaddr = (struct sockaddr_in *)&(ifr.ifr_addr);
		strncpy(ifip, inet_ntoa(wanaddr->sin_addr), len);
	}
	else{
		close(sockfd);
		return -1;
	}
	
	close(sockfd);
	return 0;
}

int get_iface_mac(const char *ifname, char *ifmac, int len)
{
	struct ifreq ifr;
	int sockfd;
	char *hwaddr = NULL;
	
	if((NULL == ifname) || (NULL == ifmac) || (len < 18))
		return -1;

	strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1);

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) <= 0)
		return -1;

	if(ioctl(sockfd, SIOCGIFHWADDR, &ifr) >= 0){
		hwaddr = ifr.ifr_hwaddr.sa_data;
		snprintf(ifmac, len, "%02x:%02x:%02x:%02x:%02x:%02x", 
			hwaddr[0] & 0xff, hwaddr[1] & 0xff, hwaddr[2] & 0xff,
			hwaddr[3] & 0xff, hwaddr[4] & 0xff, hwaddr[5] & 0xff
		);		
	}
	else{
		close(sockfd);
		return -1;
	}
	
	close(sockfd);
	return 0;
}

struct in_addr *wd_gethostbyname(const char *name)
{
	struct hostent *he;
	struct in_addr *h_addr, *in_addr_temp;

	/* XXX Calling function is reponsible for free() */
	h_addr = safe_malloc(sizeof(struct in_addr));
	he = gethostbyname(name);
	if (he == NULL) {
		/*调用系统接口失败，从文件中获取*/
		FILE *pf;
		char aucData[32] = {0};
		if((pf = fopen("/tmp/cloudsrv_addr", "r")) != NULL){
			char *pt;
			fgets(aucData, sizeof(aucData), pf);
			fclose(pf);
		
			if((pt = strstr(aucData, "\r")) != NULL)
				*pt = '\0';
			if((pt = strstr(aucData, "\n")) != NULL)
				*pt = '\0';

			if(inet_pton(AF_INET, aucData, h_addr) <= 0){
				free(h_addr);
				log_info(__LOG_ERROR__, "gethostbyname(%s) return NULL, cloudsrv_addr NULL.", name);
				return NULL;
			}
			else
				return h_addr;
		}
		else{
			free(h_addr);
			log_info(__LOG_ERROR__, "gethostbyname(%s) return NULL, error", name);
			return NULL;
		}
	}
	in_addr_temp = (struct in_addr *)he->h_addr_list[0];
	h_addr->s_addr = in_addr_temp->s_addr;
	return h_addr;
}


/*用户MAC黑白名单*/
int client_mac_hash(const char *pucMac)
{
    int index=0;
    unsigned int tmp_mac[6]={0};
    sscanf(pucMac,"%02X:%02X:%02X:%02X:%02X:%02X",
        &tmp_mac[0],&tmp_mac[1],&tmp_mac[2],&tmp_mac[3],&tmp_mac[4],&tmp_mac[5]);
    index = tmp_mac[5]%CLIENT_HASH_SIZE;
	return index;
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


int check_online_interval(unsigned int uiSecond)
{
    if(uiSecond > 0)
    {
        sleep(uiSecond);
    }
    return g_online_state;
}

int set_is_online(int state)
{
    char command[128];

	sys_log_set(WIFIDOG_LOG_FILE, "SET ONLINE STATUS: %d", state);

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




char *util_cmd_return_str(char *cmd)
{
    FILE *pf;
    
    static char buf[128];
    char *tmp = buf;

    pf = popen(cmd, "r");
    if(NULL == pf)
        return 0;
    memset(buf, 0, sizeof(buf));
    //fread(buf, sizeof(buf)-1, 1, pf);
    fgets(buf, sizeof(buf), pf);
    pclose(pf);
    while('\n' != *(++tmp));
    *tmp = '\0';
    return buf;
}

inline int mac_hashfn(unsigned char *mac, int hash_size)
{
	int	x;
	int	index;
	
	x = mac[0];
	x = (x << 2) ^ mac[1];
	x = (x << 2) ^ mac[2];
	x = (x << 2) ^ mac[3];
	x = (x << 2) ^ mac[4];
	x = (x << 2) ^ mac[5];

	x ^= x >> 8;

	index = x & (hash_size - 1);

	return index;
}

