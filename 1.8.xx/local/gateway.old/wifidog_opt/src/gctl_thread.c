/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : gctl_thread.c
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/12/11
  Last Modified :
  Description   : gctl 命令行处理函数
  Function List :
  History       :
  1.Date        : 2013/12/11
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>

#include <stdarg.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>

#include "conf.h"
#include "client_list.h"
#include "log.h"
#include "gctl.h"
#include "gctl_thread.h"
/*add@2014.7.29*/
#include "cJSON.h"
#include "util.h"

#include "client_ap.h"

extern char *g_pstExtDebugLevel;
extern int g_ext_dbg_level;
extern int g_iIosAutoPortal;
/*add@2014.7.29*/
extern long started_time;
/*add@2014.8.6*/
extern int g_freeauth_switch;
extern int g_debug_level;

extern void client_list_statistics(void);

extern int sta_dct_update(void);

#define GCTL_TRD_LOG(__level__, __format__, __arg__...) \
		do{ \
			if((LOG_DEBUG == __level__) && (g_ext_dbg_level != 0))\
				syslog(LOG_INFO, "wdpriv-gctl: "__format__, ##__arg__);\
			else if(LOG_DEBUG == __level__)\
				break;\
			else\
				syslog(__level__, "wdpriv-gctl: "__format__, ##__arg__);\
		}while(0)

extern void wifidog_update_wxl_whitelist(void);

/* netsniff related*/
#ifdef HAVE_NETSNIFF

int g_netsniff_fd = -1;

/* netsniff thread pid */
pthread_t sniff_pid;

void netsniff_request_ctl()
{
	int size;
	unsigned char buf[256];

	log_info(__LOG_DEBUG__, "begin netsniff request ctl");
	
	do{
		if((size = recv(g_netsniff_fd, buf, 256, 0)) > 0){
			continue;
		}
	} while(size > 0);

	log_info(__LOG_DEBUG__, "netsniff disconnect.");
	close(g_netsniff_fd);
	g_netsniff_fd = -1;
}

#endif

int g_aglink_fd = -1;
/* aglink thread pid */
pthread_t aglink_pid;

#define AP_CNNT_MSG			1	
#define AP_DISCNNT_MSG		2
#define STA_CNNT_MSG		3
#define STA_DISCNNT_MSG		4
#define STA_DCT_MSG			5


/*wifidog消息头结构*/
struct wifidog_msg_hdr
{
	unsigned short msg_type;
	unsigned short msg_len; 
};

unsigned char g_rbuf[4096];

int aglink_msg_parse(unsigned char *buf, int size, int *pos)
{
	int rst_size = size;
	unsigned char *pt = buf;
	struct wifidog_msg_hdr msghdr;

	while(rst_size > 0)
	{
		if(rst_size < 4)
			goto parse_end;
		
		msghdr.msg_type = (*pt << 8) + *(pt+1);
		msghdr.msg_len = (*(pt+2) << 8) + *(pt+3);

		if(rst_size < (msghdr.msg_len+4))
			goto parse_end;

		switch(msghdr.msg_type)
		{
		case AP_CNNT_MSG:
			{
				unsigned char mac[6] = {0};
				unsigned char ip[16] = {0};
				unsigned int model_id;
				char name[64] = {0};
				WD_AP_RADIO_T stRadio;
				memset(&stRadio, 0, sizeof(stRadio));
				if(msghdr.msg_len < 90)
					break;
				memcpy(mac, pt+4, 6);
				memcpy(ip, pt+10, 16);
				model_id = (*(pt+26) << 24) + (*(pt+27) << 16) 
					+ (*(pt+28) << 8) + *(pt+29);
				strncpy(name, (char *)pt+30, 64);
				
				ap_add_by_mac(mac, ip, model_id, name, 1, &stRadio);
			}
			break;
		case AP_DISCNNT_MSG:
			{
				unsigned char mac[6] = {0};
				if(msghdr.msg_len < 6)
					break;
				memcpy(mac, pt+4, 6);
				ap_del_by_mac(mac);
			}
			break;
		case STA_CNNT_MSG:
			{
				unsigned char sta_mac[6] = {0};
				unsigned char ap_mac[6] = {0};
				if(msghdr.msg_len < 12)
					break;
				memcpy(sta_mac, pt+4, 6);
				memcpy(ap_mac, pt+10, 6);
				sta_add_by_mac(sta_mac, ap_mac,NULL);
			}
			break;
		case STA_DISCNNT_MSG:
			{
				unsigned char sta_mac[6] = {0};
				if(msghdr.msg_len < 6)
					break;
				memcpy(sta_mac, pt+4, 6);
				sta_del_by_mac(sta_mac,NULL,NULL);
			}
			break;
		case STA_DCT_MSG:
			{
				unsigned char sta_mac[6] = {0};
				unsigned char rssi;
				if(msghdr.msg_len < 7)
					break;
				
				memcpy(sta_mac, pt+4, 6);
				rssi = *(pt+10);
				sta_dct_add_by_mac(NULL,sta_mac, rssi);
			}
			break;
		default:
			*pos = 0;
			break;
		}

		rst_size -= (4 + msghdr.msg_len);
		pt += (4 + msghdr.msg_len);
	}

parse_end:
	if(rst_size > 0){
		memmove(g_rbuf, pt, rst_size);
		*pos = rst_size;
	}
	else
		*pos = 0;
	
	return 0;
}

void aglink_request_ctl()
{
	int size, pos = 0;

	log_info(__LOG_DEBUG__, "begin aglink request ctl");

	if(g_pstExtDebugLevel != NULL)
		g_ext_dbg_level = atoi(g_pstExtDebugLevel);
		
	do{
		if((size = recv(g_aglink_fd, (g_rbuf+pos), (4096-pos), 0)) > 0){
			aglink_msg_parse(g_rbuf, size+pos, &pos);
			continue;
		}
	} while(size > 0);

	log_info(__LOG_DEBUG__, "aglink disconnect.");
	close(g_aglink_fd);
	g_aglink_fd = -1;
}



extern pthread_mutex_t client_list_mutex;
extern pid_t g_pid;
extern int do_wifidog_clear();

static void gctl_stop(int fd)
{
    log_info(__LOG_FAILED__, "The WiFiDog will exit pid(%d).", g_pid);

    do_wifidog_clear();
    
    kill(g_pid, 9);
    return;
}

static void gctl_debug(char *req)
{
    int level;
    s_config *config = config_get_config();
    
    sscanf(req, "debug %d\r\n", &level);
    log_info(__LOG_DEBUG__, "debug level %d", level);
    
    config->debuglevel = level;
	g_debug_level = level;
    return;
}

static void gctl_extdebug(char *req)
{
    int level;
    s_config *config = config_get_config();
    
    sscanf(req, "ext-debug %d\r\n", &level);
    log_info(__LOG_DEBUG__, "ext-debug level %d", level);
    
    config->ext_debuglevel = level;
	g_ext_dbg_level = level;
	
    return;
}


static void gctl_ios_auth_auto(char *req)
{
    char value[128];

	if(strlen(req) > 128)
		return;
    
    sscanf(req, "ios-auth-auto %s\r\n", value);
    log_info(__LOG_DEBUG__, "ios-auth-auto %s", value);

	if(!strcmp(value, "allow-all")){
		g_iIosAutoPortal = 1;
		sys_config_set(WIFIDOG_IOS_AUTO_PROTAL, "1");
	}
	else if(!strcmp(value, "deny-all")){
		g_iIosAutoPortal = 0;
		sys_config_set(WIFIDOG_IOS_AUTO_PROTAL, "0");
	}
	else if(!strcmp(value, "deny-third")){
		g_iIosAutoPortal = 2;
		sys_config_set(WIFIDOG_IOS_AUTO_PROTAL, "2");
	}
	
    return;
}

extern int set_sta_auth_status(char *ip, char *mac, int auth);
extern int set_stat_start(char *interval, char *duration);
extern int set_stat_stop();
static void gctl_sta_auth(char *req)
{
	char ip[64], mac[64];
	int auth;
	
	if(strlen(req) > 64)
		return;
    
    sscanf(req, "sta-auth %s %s %d\r\n", ip, mac, &auth);
    log_info(__LOG_DEBUG__, "%s", req);
	
	set_sta_auth_status(ip, mac, auth);
	
    return;
}

void gctl_stat_start(char *req)
{
	char interval[32], duration[32];
	if(strlen(req) > 64)
		return;

	sscanf(req, "stat-start %s %s\r\n", interval, duration);	

	set_stat_start(interval, duration);
}


static void gctl_stat_stop(char *req)
{
	if(strlen(req) > 64)
		return;
    
	set_stat_stop();
	
    return;
}


static void gctl_up_wxl_whitelist(char *req)
{
	if(strlen(req) > 64)
		return;
    
	wifidog_update_wxl_whitelist();
	
    return;
}



static void gctl_status(int fd)
{
    int loop;
    char temp[512];
    char client_list[4096];

    int total;
    int client_num;
    t_client *p1;
	int len = 0;
	int len_t = 0;

	/*首先更新统计信息*/
	client_list_statistics();

    /**@ 获取终端列表信息 */
    client_num = total = 0;
    memset(client_list, 0, sizeof(client_list));	
	len = sprintf(client_list, "wifidog run information: started-time: %ld, up-time: %ld, g_freeauth_switch: %d, g_client_access_num %d\n", 
								started_time, get_uptime()-started_time, g_freeauth_switch, g_client_access_num);
	sprintf(client_list + len, "-------------------------------------------------------------------\n");

	for(loop = 0; loop < CLIENT_HASH_SIZE; loop++)
    { 
        LOCK_CLIENT_LIST();
        p1 = g_client_head[loop].next;
        while(p1)
        {
			len = 0;
			len += sprintf(temp, "Client %d: ip: %s, mac: %s, auth_period: %d, incoming: %lld, outgoing: %lld \n"
                          "           idletimeout: %ld, forcetimeout: %ld, bw_up: %u, bw_down: %u, online_time_last_update_time: %ld\n"
                          "			  online_time: %ld, login_time: %ld, denied_client_flag: %d, last_updated: %ld, fw_connection_state: %d\n"
                          "           token=%s\n", 
                    total+1, p1->ip, p1->mac, p1->auth_period, p1->counters.incoming, p1->counters.outgoing,
                    p1->idletimeout, p1->forcetimeout, p1->bw_up, p1->bw_down, p1->online_time_last_update_time, p1->online_time, p1->login_time,
                    p1->denied_client_flag, p1->counters.last_updated, p1->fw_connection_state, p1->token);
			len += sprintf(temp + len, "-------------------------------------------------------------------\n");

			if (len >= 512)
			{
				log_info(__LOG_ERROR__, "len >= 512 client_list error, p1: %p, p1->ip: %s", p1, p1->ip);
				UNLOCK_CLIENT_LIST();
				return;
			}
			len_t += len;
			
			strcat(client_list, temp);
            client_num++;
            total++;
            p1 = p1->next;
            if(len_t >= 3500)
            {
                write(fd, client_list, strlen(client_list));
                client_num = 0;
                memset(client_list, 0, sizeof(client_list));
            }
        }
        UNLOCK_CLIENT_LIST();
    }
    write(fd, client_list, strlen(client_list));
    memset(client_list, 0, sizeof(client_list));

    sprintf(client_list, "Client_nums = %d\n", total);
    write(fd, client_list, strlen(client_list));
    return;
}

/*begin: add@2014.10.11*/
static void gctl_global(int fd)
{
	char global[2048] = {0};
    sprintf(global, "wifidog global config: \n "
        	"----------------------------------------------------\n "
        	"daemon        			 	  : %d\n"
        	"debuglevel    			 	  : %d\n"
        	"gw_redirect_port        	  : %d\n"
        	"gw_redirect_port_num         : %d\n"
        	"gw_first_page_handle_port    : %d\n"
        	"conf_ver					  : %lld\n"
        	"config_ver_tmp			      : %lld\n"
        	"cmd_ver			      	  : %lld\n"
        	"iUser_ver			      	  : %lld\n"
        	"qos_enable					  : %d\n"
        	"max_client			          : %d\n"
        	"dev_max_client			      : %d\n"
        	"ios_auto_portal		      : %d\n"
        	"bw_up			      	      : %u\n"
        	"bw_down			      	  : %u\n"
        	"forcetimeout			      : %lu\n"
        	"idletimeout			      : %lu\n"
        	"wan_flow_report_switch		  : %u\n"
        	"wan_flow_report_interval	  : %u\n"
        	"----------------------------------------------------\n"
        	"g_freeauth_switch		  	  : %d\n"
        	"g_client_access_num	      : %d\n"
        	"g_cloudplatform_client_num	  : %d\n",
			config_get_config()->daemon, 
			config_get_config()->debuglevel,
			config_get_config()->gw_redirect_port,
			FIRST_PAGE_TASK_NUM,
			config_get_config()->gw_first_page_handle_port, 
			config_get_config()->conf_ver,
			config_get_config()->config_ver_tmp,
			config_get_config()->cmd_ver, 
			config_get_config()->iUser_ver,
			config_get_config()->qos_enable,
			config_get_config()->max_client, 
			config_get_config()->dev_max_client, 
			config_get_config()->ios_auto_portal,
			config_get_config()->bw_up,
			config_get_config()->bw_down, 
			config_get_config()->forcetimeout,
			config_get_config()->idletimeout,
			config_get_config()->wan_flow_report_switch, 
			config_get_config()->wan_flow_report_interval,
			g_freeauth_switch, 
			g_client_access_num, g_cloudplatform_client_num);

    write(fd, global, strlen(global));
    return;
}
/*end: add@2014.10.11*/

static void gctl_show_reg_ap(int fd)
{
	show_reg_ap_list(fd);
    return;
}


static void gctl_show_cnnt_sta(int fd)
{
	show_sta_cnnt_list(fd);
    return;
}


/*
 Prototype    : handler
 Description  : 返回值: 0:正常流程、1:错误流程、2:表示不需要关闭socket
*/
static int handler(int fd)
{
    int read_bytes, len;
    int done;
    int loop;
    char req[1024];

    done = 0;
    memset(req, 0, sizeof(req));
    read_bytes = len = 0;
    while(!done)
    {
        len = read(fd, req + read_bytes, sizeof(req)-read_bytes);

        for(loop = read_bytes; loop < read_bytes+len; loop++)
        {
            if(req[loop] == '\n' || req[loop] == '\r' || req[loop] == '\0')
            {
                req[loop] = '\0';
                done = 1;
                break;
            }
        }
        read_bytes += len;
    }

    log_info(__LOG_DEBUG__, "gctl send request %s", req);
    if(0 == strncmp(req, "status", 6))
    {
        gctl_status(fd);
    }
    else if(0 == strncmp(req, "global", 6))
    {
        gctl_global(fd);
    }
    else if(0 == strncmp(req, "stop", 4))
    {
        gctl_stop(fd);
    }
	else if(0 == strncmp(req, "showap", 6))
    {
        gctl_show_reg_ap(fd);
    }
	else if(0 == strncmp(req, "showsta", 7))
    {
        gctl_show_cnnt_sta(fd);
    }
#ifdef HAVE_NETSNIFF
	else if (0 == strncmp(req, "netsniff-request", 16)) {
		g_netsniff_fd = fd;
		if(pthread_create(&sniff_pid, NULL, (void *)netsniff_request_ctl, NULL) != 0)
		{
            log_info(__LOG_ERROR__, "create sniff_pid failed: %s", strerror(errno));
		}
		else
			pthread_detach(sniff_pid);
		return 2;
	}
#endif
    else if(0 == strncmp(req, "debug", 5))
    {
        gctl_debug(req);
    }
	else if(0 == strncmp(req, "ext-debug", 9))
    {
        gctl_extdebug(req);
    }
	else if(0 == strncmp(req, "ios-auth-auto", 13))
    {
        gctl_ios_auth_auto(req);
    }
	else if(0 == strncmp(req, "sta-auth", 8))
	{
		gctl_sta_auth(req);
	}
	else if(0 == strncmp(req, "stat-start", 10))
	{
		gctl_stat_start(req);
	}
	else if(0 == strncmp(req, "stat-stop", 9))
	{
		gctl_stat_stop(req);
	}
	else if(0 == strncmp(req, "up-wxl-whitelist", 16))
	{
		gctl_up_wxl_whitelist(req);
	}
    else
    {
        log_info(__LOG_ERROR__, "Unknow request[%s].", req);
        return 1;
    }
    return 0;
}
extern int wifidog_set_thread_name(EN_WIFIDOG_THREAD_INDEX enThreadIndex);
void thread_gctl(void)
{
    int c_fd;
    int s_fd;
    socklen_t len;
    struct sockaddr_un addr;

    int iRet;

	/*add@2015.01.28 SET THREAD NAME*/
	wifidog_set_thread_name(WIFIDOG_THREAD_GCTL);

    s_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(-1 == s_fd)
    {
        log_info(__LOG_ERROR__, "Failed to create a new socket: %s", strerror(errno));
		sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at thread_gctl() socket");
        exit(1);
    }

    /**@ delete this file */
    unlink(DEFAULT_SOCK);
    bzero(&addr, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, DEFAULT_SOCK);

    if(bind(s_fd, (struct sockaddr *)&addr,  strlen(addr.sun_path)+sizeof(addr.sun_family)))
    {
        log_info(__LOG_ERROR__, "Failed to bind: %s", strerror(errno));
        close(s_fd);
		sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at thread_gctl() bind");
        exit(1);
    }

    if(listen(s_fd, 5))
    {
        log_info(__LOG_ERROR__, "Failed to listen socket: %s", strerror(errno));
        close(s_fd);
		sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at thread_gctl() listen");
        exit(1);
    }

    while(1)
    {
        len = sizeof(addr);
        bzero(&addr, len);
        c_fd = accept(s_fd, (struct sockaddr *)&addr, &len);
        if(-1 == c_fd)
        {
            log_info(__LOG_ERROR__, "Accept failed on socket: %s", strerror(errno));
        }
        else
        {
            /*log_info(__LOG_ERROR__, "Accept connent on socket %d (%s)", c_fd, addr.sun_path);*/
            iRet = handler(c_fd);
            if(2 != iRet)
                close(c_fd);
        }
    }
        
    return;
}



