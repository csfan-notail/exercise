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
#include "util.h"
#include "firewall.h"
#include "client_ap.h"

extern char *g_pstExtDebugLevel;
extern int g_ext_dbg_level;
extern int g_iIosAutoPortal;
extern int g_iIosRedirectMode;
extern int g_iUcwebRedirectMode;
/*add@2014.7.29*/
extern long started_time;
/*add@2014.8.6*/
extern int g_freeauth_switch;
extern int g_debug_level;

extern void client_list_statistics(void);

extern void wifidog_update_wxl_whitelist(void);

extern int wifidog_update_gw_info();

extern void set_lan_info();

extern int g_status_main;
extern int g_status_firstpage;
extern int g_status_redirect[];
extern int g_status_platfrom;
extern int g_status_80;
extern int g_status_macro_hot;

extern unsigned int g_stat_8061_access;
extern unsigned int g_stat_8062_access;
extern unsigned int g_stat_html_redirect;
extern unsigned int g_stat_307_redirect;

extern int g_online_state;

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

extern pthread_mutex_t client_list_mutex;
extern pthread_mutex_t roam_client_list_mutex;
extern pid_t g_pid;

static void gctl_stop(int fd)
{
    log_info(__LOG_FAILED__, "The WiFiDog will exit pid(%d).", g_pid);

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


static void gctl_auth_ctrl(char *req)
{
	if(strlen(req) > 128)
		return;
    
    sscanf(req, "auth-ctrl %d,%d,%d\r\n", &g_iIosAutoPortal, &g_iIosRedirectMode, 
		&g_iUcwebRedirectMode);
    log_info(__LOG_DEBUG__, "auth-ctrl %d,%d,%d", g_iIosAutoPortal, g_iIosRedirectMode, 
		g_iUcwebRedirectMode);
	
    return;
}

extern int set_sta_auth_status(char *ip,char *mac, int auth, char *method,char *token, int status);
extern int set_stat_start(char *interval, char *duration);
extern int set_stat_stop();
static void gctl_sta_auth(char *req)
{
	char mac[64];
	int auth;
	
	if(strlen(req) > 64)
		return;
    
    sscanf(req, "sta-auth %s %d\r\n", mac, &auth);
    log_info(__LOG_DEBUG__, "%s", req);
	
	set_sta_auth_status("0.0.0.0",mac, auth, "cmd",NULL, FW_MARK_KNOWN);
	
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

static void gctl_vap(char *req)
{	
	wifidog_update_gw_info();
    
    return;	
}

static void gctl_status(int fd)
{
    int loop;
    char temp[1024];
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
                   #ifdef PCAP_SUPPORT
                   len += sprintf(temp, "Client %d: ip: %s, mac: %s, auth_period: %d, incoming: %lld, outgoing: %lld \n"
                              "           idletimeout: %ld, forcetimeout: %ld, bw_up: %u, bw_down: %u, online_time_last_update_time: %ld\n"
                              "			  online_time: %ld, login_time: %ld, denied_client_flag: %d, last_updated: %ld, fw_connection_state: %d\n"
                              "           token=%s  ", 
                        total+1, p1->ip, p1->mac, p1->auth_period, p1->counters.incoming, p1->counters.outgoing,
                        p1->idletimeout, p1->forcetimeout, p1->bw_up, p1->bw_down, p1->online_time_last_update_time, p1->online_time, p1->login_time,
                        p1->denied_client_flag, p1->counters.last_updated, p1->fw_connection_state, p1->token);
                   
                   if(p1->qq_id_list)
                   {
        			if(strlen(p1->qq_id_list)  < 896-len)
            				len += sprintf(temp+len, "qq_list:%s\n", p1->qq_id_list);
        			else
        			{	
        				/*display about 10 qq nums*/
        				len += sprintf(temp+len,"qq_list:more than we can display \n");
        			}
                   }
                   else
                   {
				len += sprintf(temp+len,"qq_list:null \n");
                   }
                   
                   #else
			len += sprintf(temp, "Client %d: ip: %s, mac: %s, auth_period: %d, incoming: %lld, outgoing: %lld \n"
                          "           idletimeout: %ld, forcetimeout: %ld, bw_up: %u, bw_down: %u, online_time_last_update_time: %ld\n"
                          "			  online_time: %ld, login_time: %ld, denied_client_flag: %d, last_updated: %ld, fw_connection_state: %d\n"
                          "           token=%s\n", 
                    total+1, p1->ip, p1->mac, p1->auth_period, p1->counters.incoming, p1->counters.outgoing,
                    p1->idletimeout, p1->forcetimeout, p1->bw_up, p1->bw_down, p1->online_time_last_update_time, p1->online_time, p1->login_time,
                    p1->denied_client_flag, p1->counters.last_updated, p1->fw_connection_state, p1->token);
                   #endif

            
			len += sprintf(temp + len, "-------------------------------------------------------------------\n");

			if (len >= 1024)
			{
				log_info(__LOG_ERROR__, "len >= 1024 client_list error,len %d, p1: %p, p1->ip: %s",len, p1, p1->ip);
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
	s_config *config = config_get_config();
	
    sprintf(global, "wifidog global config: \n"
        	"----------------------------------------------------\n"
        	"auth_server                  : %s\n"
        	"gw_id                        : %s\n"
        	"online                       : %d\n"
        	"server_mode                  : %d\n"
        	"gw_redirect_port             : %d\n"
        	"gw_redirect_port_num         : %d\n"
        	"gw_first_page_handle_port    : %d\n"
        	"conf_ver                     : %lld\n"
        	"config_ver_tmp               : %lld\n"
        	"cmd_ver                      : %lld\n"
        	"iUser_ver                    : %lld\n"
        	"qos_enable                   : %d\n"
        	"max_client                   : %d\n"
        	"dev_max_client               : %d\n"
        	"bw_up                        : %u\n"
        	"bw_down                      : %u\n"
        	"forcetimeout                 : %lu\n"
        	"idletimeout                  : %lu\n"
        	"wan_flow_report_switch       : %u\n"
        	"wan_flow_report_interval     : %u\n"
        	"g_freeauth_switch            : %d\n"
        	"g_client_access_num          : %d\n"
        	"g_cloudplatform_client_num   : %d\n"
        	"server_mode                  : %d\n"
        	"wxl_ssid                     : %s\n"
        	"wxl_bssid                    : %s\n"
        	"authmode                     : %d,%d,%d,%d,%d,%d,%d,%d\n"
        	"debuglevel                   : %d\n"
        	"lan_interface                : %s\n"
        	"lan_address                  : %s\n"
        	"wan_interface                : %s\n"
        	"wan_address                  : %s\n"
        	"wan_mac                      : %s\n"
        	"snmp_server                  : %s\n"
        	"debug_count                  : 8061(%d) 8062(%d) html(%d) 307(%d)\n"
        	"g_iIosAutoPortal             : %d\n"
        	"g_iIosRedirectMode           : %d\n"
        	"g_iUcwebRedirectMode         : %d\n"
        	"uiHttpadswitch               : %d\n"
        	"uiHttpadwldura               : %d\n"
        	"uiHttpadbldura               : %d\n"
        	"uiHttprdmaxcnt               : %d\n"
        	"uiHttpckIntvl                : %d\n"
        	"uiHttpckStaLimit             : %d\n"
        	"----------------------------------------------------\n",
        	(config->auth_servers ? (config->auth_servers->authserv_hostname 
			? config->auth_servers->authserv_hostname : "") : ""),
			(config->gw_id ? config->gw_id : ""),
			g_online_state,
			config->server_mode,
			config->gw_redirect_port,
			FIRST_PAGE_TASK_NUM,
			config->gw_first_page_handle_port, 
			config->conf_ver,
			config->config_ver_tmp,
			config->cmd_ver, 
			config->iUser_ver,
			config->qos_enable,
			config->max_client, 
			config->dev_max_client, 
			config->bw_up,
			config->bw_down, 
			config->forcetimeout,
			config->idletimeout,
			config->wan_flow_report_switch, 
			config->wan_flow_report_interval,
			g_freeauth_switch, 
			g_client_access_num, 
			g_cloudplatform_client_num,
			config->server_mode,
			(config->wxl_ssid ? config->wxl_ssid : ""),
			(config->wxl_bssid ? config->wxl_bssid : ""),
			auth_mode_support_free(),
			auth_mode_support_user(),
			auth_mode_support_msg(),
			auth_mode_support_qq(),
			auth_mode_support_weibo(),
			auth_mode_support_weixin(),
			auth_mode_support_alipay(),
			auth_mode_support_wxl(),
			config->debuglevel,
			(config->gw_interface ? config->gw_interface : ""),
			(config->gw_address ? config->gw_address : ""),
			(config->wan_interface ? config->wan_interface : ""),
			(config->wan_address ? config->wan_address : ""),
			(config->wan_mac ? config->wan_mac : ""),
			(config->snmp_server ? config->snmp_server : ""),
			g_stat_8061_access, 
			g_stat_8062_access,
			g_stat_html_redirect,
			g_stat_307_redirect,
			g_iIosAutoPortal, 
			g_iIosRedirectMode,
			g_iUcwebRedirectMode,
			config->uiHttpadswitch,
			config->uiHttpadwldura,
			config->uiHttpadbldura,
			config->uiHttprdmaxcnt,
			config->uiHttpckIntvl,
			config->uiHttpckStaLimit
			);

    write(fd, global, strlen(global));
    return;
}
/*end: add@2014.10.11*/

static void gctl_show_reg_ap(int fd)
{
	show_reg_ap_list(fd);
    return;
}

static void gctl_span_ssid_auth(int fd, char *pReq)
{
     char aucRes[128]={0};
     s_config *config = config_get_config();
    if(NULL == pReq)
    {
        log_info(__LOG_ERROR__, "param is NULL");
        return;
    }
   
    if(strstr(pReq,"enable"))
    {
        config->uiSpanSsidAuthSwith = 1;
        sprintf(aucRes,"set span_ssid_auth enable ok\r\n");
    }
    else if(strstr(pReq,"disable"))
    {
        config->uiSpanSsidAuthSwith= 0;
        sprintf(aucRes,"set span_ssid_auth disable ok\r\n");
    }
    else
    {
        sprintf(aucRes,"%d\r\n",config->uiSpanSsidAuthSwith);
    }

    
    write(fd,aucRes,strlen(aucRes));

    
    return;
}



static void gctl_show_cnnt_sta(int fd)
{
	show_sta_cnnt_list(fd);
    return;
}


static void gctl_how_maro_roam_sta(int fd)
{
        int loop;
        char temp[512]={0};
        char client_list[4096]={0};
    
        int client_num=0;
        roam_client*p1=NULL;
        int len = 0;
        int len_t = 0;
        //sprintf(temp,"get roam-sta");
        //write(fd, temp, strlen(temp));
        for(loop = 0; loop < CLIENT_HASH_SIZE; loop++)
        { 
            LOCK_ROAM_CLIENT_LIST();
            p1 = g_roam_client_head[loop].next;
            while(p1)
            {
                len = 0;
                client_num++;
                len += sprintf(temp, "roam client %d: mac:"MAC_FMT" ip:"IP_FMT" Flag=%d  incomming=%lld  outgoing=%lld online_time=%ld token=%s\n",\
                client_num, MAC_ARG(p1->aucIntMac),IP_ARG(p1->aucIntIp), p1->eFlag, p1->incoming, p1->outgoing, p1->online_time,p1->token);
                len_t +=len;
                strcat(client_list, temp);
                p1 = p1->next;
                if(len_t >= 3500)
                {
                    write(fd, client_list, strlen(client_list));
                    memset(client_list, 0, sizeof(client_list));
                    len_t =0;
                }
                
            }
            UNLOCK_ROAM_CLIENT_LIST();
        }
        write(fd, client_list, strlen(client_list));
        memset(client_list, 0, sizeof(client_list));
    
        sprintf(client_list, "Client_nums = %d\n", client_num);
        write(fd, client_list, strlen(client_list));
        return;
}

static void gctl_show_ios_sta(int fd)
{
	show_ios_sta_cnnt_list(fd);
    return;
}

static void gctl_show_sta_acl(int fd)
{
	show_sta_acl_list(fd);
    return;	
}

static void gctl_sta_http_record(int fd, char *req)
{
	s_config *config = config_get_config();

	log_info(__LOG_DEBUG__, "%s", req);

	if(!strncmp(req, "httprecord ", 11)){
	    sscanf(req, "httprecord %d %d %d %d %d %d\r\n", 
			&config->uiHttpadswitch, &config->uiHttpadwldura,
			&config->uiHttpadbldura, &config->uiHttprdmaxcnt,
			&config->uiHttpckIntvl, &config->uiHttpckStaLimit);
	}
	else
		show_sta_http_record(fd);

    return;	
}

static void gctl_lan_info(int fd, char *req)
{
	log_info(__LOG_DEBUG__, "%s", req);

	if(strstr(req, "update")){
	    set_lan_info();
	}
	else
		show_lan_info(fd);

    return;	
}


static void gctl_running_status(int fd)
{
	int loop;
    char buf[512];
    memset(buf, 0, sizeof(buf));

	strncat_format(buf, sizeof(buf), "8060:%d", g_status_main);

	for(loop=0; loop<FIRST_PAGE_TASK_NUM; ++loop){
		strncat_format(buf, sizeof(buf), ":%d:%d", 9061+loop, g_status_redirect[loop]);
	}

	strncat_format(buf, sizeof(buf), ":8062:%d:PLAT:%d:80:%d:MARO_HOT:%d",
			g_status_firstpage,
			g_status_platfrom,
			g_status_80,
			g_status_macro_hot);
	
    write(fd, buf, strlen(buf));

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
    else if(0 == strncmp(req,"ssid-auth",9))
    {
        gctl_span_ssid_auth(fd,req);
    }
	else if(0 == strncmp(req, "showsta", 7))
    {
        gctl_show_cnnt_sta(fd);
    }
    else if(0 == strncmp(req, "roam-sta", 8))
    {
        gctl_how_maro_roam_sta(fd);
    }
	else if(0 == strncmp(req, "showiossta", 10))
    {
        gctl_show_ios_sta(fd);
    }
	else if(0 == strncmp(req, "showaclsta", 10))
    {
        gctl_show_sta_acl(fd);
    }
	else if(0 == strncmp(req, "httprecord", 10))
    {
        gctl_sta_http_record(fd, req);
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
	else if(0 == strncmp(req, "auth-ctrl", 9))
    {
        gctl_auth_ctrl(req);
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
	else if(0 == strncmp(req, "vap-update", 10))
	{
		gctl_vap(req);
	}
	else if(0 == strncmp(req, "run-status", 10))
	{
		gctl_running_status(fd);
	}
	else if(0 == strncmp(req, "laninfo", 7))
	{
		gctl_lan_info(fd, req);
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



