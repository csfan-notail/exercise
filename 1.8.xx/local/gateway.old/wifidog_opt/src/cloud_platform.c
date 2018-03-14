/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : cloud_platform.c
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/12/5
  Last Modified :
  Description   : 与平台定时交互任务 
  Function List :
  History       :
  1.Date        : 2013/12/5
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

#include <pthread.h>

#include "httpd.h"
#include "conf.h"
#include "common.h"
#include "log.h"
#include "config.h"
#include "client_list.h"
#include "firewall.h"
#include "auth_server.h"
#include "lib_pa.h"
#include "cJSON.h"
#include "util.h"
#include "cloud_platform.h"
#include "redirect_portal.h"
#include "client_aglink.h"


/**@ 设备启动时间 */
extern time_t started_time;

extern pthread_mutex_t client_list_mutex;

event_map *eventHead = NULL;

extern int sta_dct_enable(int enable);
extern int sta_dct_rssi(int rssi);
extern int sta_dct_report_time(int interval);

extern int g_stadct_rpt_time;

extern void authsta_sync_report(int *sockfd);
extern void sta_get_keyinfo(int *sockfd);

int ping_timeouts = 0;
cloud_src_ver_t g_cloud_src_ver;    //云平台上最新的资源版本号   在ping response中更新
cloud_src_ver_t g_gw_src_ver;       //网关本地下载的版本号       在脚本downloadcloudsrc.sh 中更新
	
#define DEF_REPORT_TIME				180
int g_stadct_rpt_time = DEF_REPORT_TIME;

/*begin: add@2014.8.26 for reporting flow of wan*/
interface_flow_stats_t g_stInterfaceFlowStats = {0};
/*end: add@2014.8.26 for reporting flow of wan*/



/************ 函数声明 **************/
void client_list_deny_report(int *sockfd);
//void client_list_deny_response(void);
void client_list_deny_response(char *response);

void client_list_report(int *sockfd);
void client_list_request_report(int *sockfd, int begin);
void client_list_request_response(char *response, int *begin, int *count, int *client_num);
void client_list_response(void);
void client_list_statistics(void);
void stadct_list_report(int *sockfd);
void conf_request(int *sockfd);
void conf_response(char *response);
void event_map_cannel(en_event_code code);
event_map *event_map_find(en_event_code code);
static int event_map_init(void);
int event_map_register(int event_code, 
                               int time_interval, 
                               int timeout_count, 
                               void (*request_func)(),
                               void (*response_func)());
void event_map_update(int event_code, 
                              int time_interval, 
                              int timeout_count, 
                              void (*request_func)(),
                              void (*response_func)());
void ping_request(int *sockfd);
void ping_response(char *response);
static void send_client_list(int client_num, char *client_list, int *sockfd, s_config *config, char *send_type);

void filter_string_tool(char *response, char *mark, char format, void(*function)());
void wan_flowing_report_request(int *sockfd);


/*************************** 工具函数 ***************************/
void filter_string_tool(char *response, char *mark, char format, void (*function)())
{
    char *start, *index, *end;

    /** 获取配置参数 */
    if(format == '\0')
    {
        if(NULL != (start = strstr(response, mark)))
        {
            if(NULL != (end = strstr(start, "\n")))
            {
                *end = '\0';
                log_info(__LOG_DEBUG__, "%s: %s, len %d", mark, start, strlen(start));
                if(function && 0 != strlen(start))
                    function(mark, start);
                *end = '\n';
            }
        }
    }
    else
    {
        if(NULL != (start = strstr(response, mark)))
        {
            if(NULL != (end = strstr(start, "\n")))
            {
                *end = '\0';
                index = start;
                index += strlen(mark);
                start = index;
                while(*index != '\0')
                {
                    if(*index != format)
                    {
                        index++;
                        continue;
                    }
                    else
                    {
                        *index = '\0';
                        log_info(__LOG_DEBUG__, "%s: %s, len %d", mark, start, strlen(start));
                        if(function && 0 != strlen(start))
                            function(mark, start);
                        *index = format;
                        start = ++index;
                    }
                }
                *index = '\0';
                log_info(__LOG_DEBUG__, "%s: %s, len %d", mark, start, strlen(start));
                if(function && 0 != strlen(start))
                    function(mark, start);
                *index = format;
                start = ++index;
                *end='\n';
            }
        }
    }
    return;
}


/************************** 处理分支函数 **************************/
/*配置版本号处理*/
static void ping_response_conf_ver(char *mark, char *res)
{
    long long conf_ver;
    
    sscanf(res, CONF_VER"%lld", &conf_ver);
    log_info(__LOG_DEBUG__, "conf_ver = %lld", conf_ver);
    if(config_get_config()->conf_ver != conf_ver)
    {
        event_map_register(EVENT_REQUEST_CONFIG, 3, 1, conf_request, conf_response);
		/*modified @2014.8.13 for conf_ver add in conf_response */
        //config_get_config()->conf_ver = conf_ver;
        config_get_config()->config_ver_tmp = conf_ver;
    }
    return;
}

/* 获取资源版本号，赋值给全局变量 */
static void ping_response_src_tpl(char *mark, char *res)
{
    
    sscanf(res, TPL_VER"%d", &g_cloud_src_ver.iTpl_ver);
    log_info(__LOG_DEBUG__, "tmplt_ver = %d", g_cloud_src_ver.iTpl_ver);
  
    return;
}

static void ping_response_src_product(char *mark, char *res)
{
    
    sscanf(res, PROD_VER"%d", &g_cloud_src_ver.iProduct_ver);
    log_info(__LOG_DEBUG__, "pro_ver = %d", g_cloud_src_ver.iProduct_ver);
    
    return;
}
static void ping_response_src_adver(char *mark, char *res)
{
    
    sscanf(res, AD_VER"%d", &g_cloud_src_ver.iAd_ver);
    log_info(__LOG_DEBUG__, "ad_ver = %d", g_cloud_src_ver.iAd_ver);

    return;
}
/*cmd版本号检测*/
static void ping_response_cmd_ver(char *mark, char *res)
{
    long long cmd_ver;
    
    sscanf(res, CMD_VER"%lld", &cmd_ver);
    log_info(__LOG_DEBUG__, "cmd_ver = %lld", cmd_ver);
    if(config_get_config()->cmd_ver != cmd_ver)
    {
        event_map_register(EVENT_REQUEST_CONFIG, 3, 1, conf_request, conf_response);
        config_get_config()->cmd_ver = cmd_ver;
    }
    return;
}

/*begin: add@2014.10.11 处理云平台的强制用户下线版本号 和 云平台在线用户总数*/
static void ping_response_user_ver(char *mark, char *res)
{
    long long user_ver;
    
    sscanf(res, USER_VER"%lld", &user_ver);
    log_info(__LOG_DEBUG__, "user_ver = %lld", user_ver);
    config_get_config()->iUser_ver = user_ver;
    
    return;
}
static void ping_response_total_user_num(char *mark, char *res)
{
    long long total_user_num;
    
    sscanf(res, TOTAL_USER_NUM"%lld", &total_user_num);
    log_info(__LOG_DEBUG__, "total_user_num = %lld", total_user_num);
    g_cloudplatform_client_num = total_user_num;
    
    return;
}
/*end: add@2014.10.11 处理云平台的强制用户下线版本号 和 云平台在线用户总数*/

static int g_forced_offline_num = 0;
/** @强制用户下线 */
static void ping_response_forced_offline_num(char *mark, char *res)
{
    if(strstr(res, CLIENT_NUM"all"))
    {
        g_forced_offline_num = 0;
        //** 把所有用户都踢下线
        log_info(__LOG_WARNING__, "Deleltes all client from the connections list ");
        LOCK_CLIENT_LIST();
        client_list_delete_all();
        UNLOCK_CLIENT_LIST();
    }
    else
    {
        sscanf(res, CLIENT_NUM"%d", &g_forced_offline_num);
        log_info(__LOG_DEBUG__, "client_num = %d", g_forced_offline_num);
    }
    return;
}

static void ping_response_forced_offline_del(char *mark, char *res)
{
    char ip[16];
	char mac[18];
    t_client *client;
    
    sscanf(res, "%[^|]|%s", ip, mac);
    log_info(__LOG_WARNING__, "offline client ip %s mac %s", ip, mac);
    LOCK_CLIENT_LIST();
    client = client_list_find(ip, mac);
    client_list_delete(client);
    UNLOCK_CLIENT_LIST();
	sys_log_set(WIFIDOG_USR_LOG_FILE, "nf-DENY: forced-offline %s %s", ip, mac);
    nf_fw_access(FW_ACCESS_DENY, ip, mac);
    if(config_get_config()->qos_enable)
        fw_qos_del(mac);
    return;
}

/**@ 黑白名单配置 */
typedef enum {
    EN_QOSENABLE,           /* 流控开关 */
	EN_QOSUP,				/* QOS UP*/
	EN_QOSDOWN,             /* QOS DOWN*/
    EN_TRUSTEDMACLIST,      /* 源MAC 地址白名单 */
    EN_TRUSTEDIPLIST,       /* 目的IP 地址白名单 */
    EN_TRUSTEDWANHOSTLIST,  /* ULR 白名单 */
    EN_BLACKIPLIST,         /* 目的IP 地址黑名单 */
    EN_BLACKWANHOSTLIST,    /* URL 黑名单 */
    EN_BLACKMACLIST,        /* 源MAC 地址黑名单 */
    EN_FREEAUTHMACLIST,     /* 源MAC 地址免认证 */
    EN_FREEAUTHIPLIST,      /* 源IP 地址免认证 */
    EN_MAXSTATIONNUM,       /* 最大接入用户数 :可配置*/
    EN_FORCETIMEOUT,		/* FORCETIMEOUT*/
    EN_IDLETIMEOUT,			/* IDLETIMEOUT*/
    EN_WLANFLOWSTATSWITCH,  /* WAN口流量上报开关*/
    EN_POSTTRUSTEDWANHOSTLIST, /* 认证后 URL 白名单 */
    /* BEGIN: Added by WeiHonggang, 2014/2/18   PN:TASK#0平台支持认证模式同步到网关，并根据认证模式进行设置 */
    EN_AUTHMODE, /*网关支持的认证模式*/
    /* END:   Added by WeiHonggang, 2014/2/18   PN:TASK#0平台支持认证模式同步到网关，并根据认证模式进行设置 */
	EN_STADCTEN,
	EN_STADCTRSSI,
	EN_STADCTRPTTIME,
	EN_MUSTREDIRECTURLIST, /*add@2014.11.02 总是要重定向的url名单*/
	EN_DEVMAXSTATIONNUM,       /* 设备最大接入用户数 :产品定位相关*/
	EN_WXLSERVERIP, /*add@2015.01.28 for wechat connect wifi*/
	EN_OPCODE_MAX,
} en_opcode;

static const struct {
    char *name;
    en_opcode code;
}words[] = {
    {QOSENABLE, EN_QOSENABLE},
	{QOSUP, EN_QOSUP},
	{QOSDOWN, EN_QOSDOWN},
    {TRUSTEDMACLIST, EN_TRUSTEDMACLIST},
    {TRUSTEDIPLIST, EN_TRUSTEDIPLIST},
    {TRUSTEDWANHOSTLIST, EN_TRUSTEDWANHOSTLIST},
    {BLACKIPLIST, EN_BLACKIPLIST},
    {BLACKWANHOSTLIST, EN_BLACKWANHOSTLIST},
    {BLACKMACLIST, EN_BLACKMACLIST},
    {FREEAUTHMACLIST, EN_FREEAUTHMACLIST},
    {FREEAUTHIPLIST, EN_FREEAUTHIPLIST},
    {MAXSTATIONNUM, EN_MAXSTATIONNUM},
    {FORCETIMEOUT, EN_FORCETIMEOUT},
    {IDLETIMEOUT, EN_IDLETIMEOUT},
    {WANFLOWSTATSWITCH, EN_WLANFLOWSTATSWITCH},
    {POSTTRUSTEDWANHOSTLIST, EN_POSTTRUSTEDWANHOSTLIST},
    /* BEGIN: Added by WeiHonggang, 2014/2/18   PN:TASK#0平台支持认证模式同步到网关，并根据认证模式进行设置 */
    {AUTHMODE,EN_AUTHMODE},
    /* END:   Added by WeiHonggang, 2014/2/18   PN:TASK#0平台支持认证模式同步到网关，并根据认证模式进行设置 */
	{STADCTENABLE,EN_STADCTEN},
	{STADCTRSSI,EN_STADCTRSSI},
	{STADCTRPTTIME,EN_STADCTRPTTIME},
    {MUSTREDIRECTURLLIST,EN_MUSTREDIRECTURLIST},
    {MAXDEVSTATIONNUM, EN_DEVMAXSTATIONNUM},
    {WXLSERVERIP, EN_WXLSERVERIP} /*add@2015.01.28 for wechat connect wifi*/
};
/* BEGIN: Added by WeiHonggang, 2014/2/18   PN:TASK#0平台支持认证模式同步到网关，并根据认证模式进行设置 */
en_auth_mode auth_mode[AUTH_MODE_MAX] = 
{
    AUTH_MODE_NOT_SUPPORT,
    AUTH_MODE_NOT_SUPPORT,
    AUTH_MODE_NOT_SUPPORT,
    AUTH_MODE_NOT_SUPPORT,
    AUTH_MODE_NOT_SUPPORT,
    AUTH_MODE_NOT_SUPPORT, 
    AUTH_MODE_NOT_SUPPORT,/*add@2014.10.11 add auth mode: alipay*/
    AUTH_MODE_NOT_SUPPORT,/*add@2015.01.28 for wechat connect wifi*/
};
int auth_mode_index = 0;
/*微信相关URL*/
st_url url_weixin[] = 
{
    {"weixin.qq.com"},
    {"life.qq.com"},
    {"short.weixin.qq.com"},
    {"mmsns.qpic.cn"},
    {"dns.weixin.qq.com"},
    {"mp.weixin.qq.com"},
    {"szshort.weixin.qq.com"},
    {"szextshort.weixin.qq.com"},
    {"wx.qlogo.cn"}
};
/*微博相关URL*/
st_url url_weibo[] =
{
    {"api.weibo.com"},
    {"login.sina.com.cn"},
    {"open.weibo.com"},
    {"tjs.sjs.sinajs.cn"}
};
/*QQ相关URL*/
st_url url_qq[] =
{
    {"ptlogin2.qq.com"},
    {"tajs.qq.com"},
    {"support.qq.com"},
    {"pingtcss.qq.com"},
    {"openapi.qzone.qq.com"},
    {"badjs.qq.com"},
    {"connect.qq.com"},
    {"pub.idqqimg.com"},
    {"qzonestyle.gtimg.cn"},
    {"graph.qq.com"},
    {"isdspeed.qq.com"},
    {"imgcache.qq.com"},
    {"im.qq.com"},
    {"cgi.connect.qq.com"},
    {"pinghot.qq.com"},
    {"ui.ptlogin2.qq.com"},
    {"pingtcss.qq.com"},
    {"openmobile.qq.com"},
    {"isdspeed.qq.com"},
    {"q3.qlogo.cn"},
    {"qzonestyle.gtimg.cn"},
    {"pub.idqqimg.com"},
    {"captcha.qq.com"}
};
/*begin: add@2014.10.11*/
/*alipy相关URL*/
st_url url_alipay[] =
{
    {"alipay.com"},
    {"alipayobjects.com"},
    {"itunes.apple.com"},
    {"phobos.apple.com"},
    {"itunes-apple.com"},
    {"akadns.net"},
    {"edgekey.net"},
    {"mzstatic.com"},
    {"xp.apple.com"},
    {"sb.symcd.com"}
};
int auth_mode_support_alipay(void)
{
    return auth_mode[AUTH_MODE_ALIPAY] ? 1:0;
}
/*end: add@2014.10.11*/
int auth_mode_support_weixin(void)
{
    return auth_mode[AUTH_MODE_WEIXIN] ? 1:0;
}

int auth_mode_support_wxl(void)
{
	return auth_mode[AUTH_MODE_WXL] ? 1:0;
}

extern void http_port80_handle(void);
pthread_t http_port80_handle_pid; //task#416 wechat connect wifi @2015.01.27
void wxl_whitelist_download(int *sockfd);


static void conf_set_by_auth_mode(void)
{
    int loop;
	struct hostent *he;
	struct in_addr *in_addr_temp;
	static int thread_80 = 0;
	static int wxl_ip = 0;
	static int wxl_uplist = 0;
	int i = 0;
	
    log_info(__LOG_DEBUG__, "auth_mode:[%d|%d|%d|%d|%d|%d|%d|%d]\n",auth_mode[0],\
        auth_mode[1],auth_mode[2],auth_mode[3],auth_mode[4],auth_mode[5],auth_mode[6],auth_mode[7]);
    
    /*支持QQ认证*/
    if ((int )auth_mode[AUTH_MODE_QQ] == AUTH_MODE_SUPPORT)
    {
        for(loop = 0; loop < sizeof(url_qq)/sizeof(st_url); loop++)
        {
            if (0 != portal_add_url_free_auth_list(url_qq[loop].url))
                log_info(__LOG_ERROR__, "portal_add_url_free_auth_list failed");

			he = gethostbyname((char *)url_qq[loop].url);
			if (he != NULL){
				in_addr_temp = (struct in_addr *)he->h_addr_list[0];
				if (0 != portal_add_url_ip_white_list(url_qq[loop].url, ntohl(in_addr_temp->s_addr)))
                    log_info(__LOG_ERROR__, "portal_add_url_ip_white_list failed");
			}
        }
    }
    /*支持新浪微博认证*/
    if ((int )auth_mode[AUTH_MODE_WEIBO] == AUTH_MODE_SUPPORT)
    {
        for(loop = 0; loop < sizeof(url_weibo)/sizeof(st_url); loop++)
        {
            if (0 != portal_add_url_free_auth_list(url_weibo[loop].url))
                log_info(__LOG_ERROR__, "portal_add_url_free_auth_list failed");

			he = gethostbyname((char *)url_weibo[loop].url);
			if (he != NULL){
				in_addr_temp = (struct in_addr *)he->h_addr_list[0];
				if (0 != portal_add_url_ip_white_list(url_weibo[loop].url, ntohl(in_addr_temp->s_addr)))
                    log_info(__LOG_ERROR__, "portal_add_url_ip_white_list failed");
			}
        }
    }
    /*支持微信认证*/
    if ((int )auth_mode[AUTH_MODE_WEIXIN] == AUTH_MODE_SUPPORT)
    {
        for(loop = 0; loop < sizeof(url_weixin)/sizeof(st_url); loop++)
        {
            if (0 != portal_add_url_free_auth_list(url_weixin[loop].url))
                log_info(__LOG_ERROR__, "portal_add_url_free_auth_list failed");

			he = gethostbyname((char *)url_weixin[loop].url);
			if (he != NULL){
				i = 0;
				while(he->h_addr_list[i]){
					in_addr_temp = (struct in_addr *)he->h_addr_list[i];
					if (0 != portal_add_url_ip_white_list(url_weixin[loop].url, ntohl(in_addr_temp->s_addr)))
                    	log_info(__LOG_ERROR__, "portal_add_url_ip_white_list failed");
					i++;
				}
			}
        }
    }
    /*begin:add@2014.10.11 支持支付宝认证*/
    if ((int )auth_mode[AUTH_MODE_ALIPAY] == AUTH_MODE_SUPPORT)
    {
        for(loop = 0; loop < sizeof(url_alipay)/sizeof(st_url); loop++)
        {
            if (0 != portal_add_url_free_auth_list(url_alipay[loop].url))
                log_info(__LOG_ERROR__, "portal_add_url_free_auth_list failed");

			he = gethostbyname((char *)url_alipay[loop].url);
			if (he != NULL){
				in_addr_temp = (struct in_addr *)he->h_addr_list[0];
				if (0 != portal_add_url_ip_white_list(url_alipay[loop].url, ntohl(in_addr_temp->s_addr)))
                    log_info(__LOG_ERROR__, "portal_add_url_ip_white_list failed");
			}
        }
    }
    /*end:add@2014.10.11 支持支付宝认证*/

	/*微信连固定IP处理*/
	if (((int )auth_mode[AUTH_MODE_WXL] == AUTH_MODE_SUPPORT) && !wxl_ip){
		system("ip addr add "WXL_ADDRESS"/32 dev br0");
		log_info(__LOG_MSG__, "create wxl ip "WXL_ADDRESS" on br0");
		wxl_ip = 1;
	}
	if(((int )auth_mode[AUTH_MODE_WXL] != AUTH_MODE_SUPPORT) && wxl_ip){
		system("ip addr delete "WXL_ADDRESS"/32 dev br0");
		log_info(__LOG_MSG__, "delete wxl ip "WXL_ADDRESS" on br0");
		wxl_ip = 0;	
	}

	/*80端口线程*/
	if (((int )auth_mode[AUTH_MODE_WXL] == AUTH_MODE_SUPPORT) ||
		((int )auth_mode[AUTH_MODE_ALIPAY] == AUTH_MODE_SUPPORT))
	{
		if(!thread_80){
			if(pthread_create(&http_port80_handle_pid, NULL, (void *)http_port80_handle, NULL) != 0)
			{
				log_info(__LOG_ERROR__, "Failed to create a new thread (http_port80_handle) - exiting");
				sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at conf_set_task()");
				exit(1);
			}
			pthread_detach(http_port80_handle_pid);
			log_info(__LOG_DEBUG__, "Create a new thread (http_port80_handle) success. pid %u", 
				(unsigned int)http_port80_handle_pid);

			thread_80 = 1;
		}
	}

	/*微信连白名单更新任务*/
	if (((int )auth_mode[AUTH_MODE_WXL] == AUTH_MODE_SUPPORT) ||
		((int )auth_mode[AUTH_MODE_WEIXIN] == AUTH_MODE_SUPPORT))
	{
		if(!wxl_uplist){
			event_map_register(EVENT_DOWNLOAD_WXL_WHITELIST, (config_get_config()->checkinterval)*(60*24), 
				0, wxl_whitelist_download, NULL);
			wxl_uplist = 1;
		}
	}
	else{
		if(wxl_uplist){
			event_map_cannel(EVENT_DOWNLOAD_WXL_WHITELIST);
			wxl_uplist = 0;
		}
	}

    /*初始化，防止跑飞*/
    auth_mode_index = 0;
    return;
}
/* END:   Added by WeiHonggang, 2014/2/18   PN:TASK#0平台支持认证模式同步到网关，并根据认证模式进行设置 */
/*begin: add@2015.01.31 for wechat connect wifi*/
static void wifidog_download_wxl_whitelist(void)
{
#define WHITE_LIST_TGT	"/cmps/admin.php/public/update_whiteList"
#define WHITE_LIST_FILE	"/tmp/wxl_white_list.dat"

	static int ver = 1;
	char cmd[512];

	/*从云平台下载微信连白名单*/
	/*httpgetinfo "www.login-wifi.com" "/cmps/admin.php/public/update_whiteList" "/tmp/wxl_white_list.dat" 8*/
	system("killall httpgetinfo");
	snprintf(cmd, 512, "httpgetinfo \"%s\" \"%s\" \"%s\" %d \"gctl up-wxl\" 1>/dev/null 2>&1 &", 
		"www.login-wifi.com", WHITE_LIST_TGT, WHITE_LIST_FILE, ver++);
	system(cmd);

	return;
}

static void wifidog_set_default_wxl_serverip(void)
{
#define WHITE_LIST_FILE	"/tmp/wxl_white_list.dat"

	int loop;
	struct hostent *he;
	struct in_addr *in_addr_temp;
	int set_default = 1;

#if 1
	/*首先设置普通微信白名单*/
	for(loop = 0; loop < sizeof(url_weixin)/sizeof(st_url); loop++)
    {
        if (0 != portal_add_url_free_auth_list(url_weixin[loop].url))
            log_info(__LOG_ERROR__, "portal_add_url_free_auth_list failed");

		he = gethostbyname((char *)url_weixin[loop].url);
		if (he != NULL){
			in_addr_temp = (struct in_addr *)he->h_addr_list[0];
			if (0 != portal_add_url_ip_white_list(url_weixin[loop].url, ntohl(in_addr_temp->s_addr)))
                log_info(__LOG_ERROR__, "portal_add_url_ip_white_list failed");
		}
    }
#endif

	/*然后设置微信连白名单*/
	if(set_default){ /*无法获取平台微信连白名单，设置默认名单*/
		portal_add_wxl_serverip((unsigned int)inet_addr("14.17.41.162"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("14.17.42.45"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("14.17.44.26"), 31);
		portal_add_wxl_serverip((unsigned int)inet_addr("14.17.52.128"), 27);
		portal_add_wxl_serverip((unsigned int)inet_addr("14.18.245.160"), 27);
		portal_add_wxl_serverip((unsigned int)inet_addr("27.115.124.22"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("27.115.124.90"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("58.251.61.128"), 26);
		portal_add_wxl_serverip((unsigned int)inet_addr("61.151.186.120"), 29);
		portal_add_wxl_serverip((unsigned int)inet_addr("61.151.224.41"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("101.226.35.19"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("101.226.76.175"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("101.226.129.128"), 25);
		portal_add_wxl_serverip((unsigned int)inet_addr("101.227.131.0"), 25);
		portal_add_wxl_serverip((unsigned int)inet_addr("101.227.169.128"), 25);
		portal_add_wxl_serverip((unsigned int)inet_addr("103.7.28.192"), 26);
		portal_add_wxl_serverip((unsigned int)inet_addr("103.7.29.249"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("103.7.31.128"), 26);
		portal_add_wxl_serverip((unsigned int)inet_addr("111.30.132.25"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("111.30.135.157"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("112.64.200.240"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("112.90.74.144"), 28);
		portal_add_wxl_serverip((unsigned int)inet_addr("112.90.77.128"), 25);
		portal_add_wxl_serverip((unsigned int)inet_addr("117.135.130.177"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("117.135.148.21"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("117.135.169.19"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("117.135.169.91"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("117.135.171.75"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("117.135.171.128"), 27);
		portal_add_wxl_serverip((unsigned int)inet_addr("117.144.242.26"), 31);
		portal_add_wxl_serverip((unsigned int)inet_addr("117.185.24.123"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("117.185.117.128"), 27);
		portal_add_wxl_serverip((unsigned int)inet_addr("120.198.201.185"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("120.198.203.163"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("123.126.122.123"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("123.151.10.185"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("123.151.14.18"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("123.151.152.100"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("125.39.247.176"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("140.206.160.128"), 25);
		portal_add_wxl_serverip((unsigned int)inet_addr("140.207.54.0"), 25);
		portal_add_wxl_serverip((unsigned int)inet_addr("140.207.135.32"), 31);
		portal_add_wxl_serverip((unsigned int)inet_addr("140.207.135.125"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("163.177.65.222"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("163.177.68.157"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("163.177.83.213"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("163.177.89.128"), 26);
		portal_add_wxl_serverip((unsigned int)inet_addr("180.153.9.128"), 27);
		portal_add_wxl_serverip((unsigned int)inet_addr("180.153.81.160"), 29);
		portal_add_wxl_serverip((unsigned int)inet_addr("180.153.82.27"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("182.254.5.187"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("182.254.8.161"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("182.254.10.0"), 25);
		portal_add_wxl_serverip((unsigned int)inet_addr("182.254.11.128"), 26);
		portal_add_wxl_serverip((unsigned int)inet_addr("182.254.18.165"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("182.254.21.114"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("182.254.34.0"), 26);
		portal_add_wxl_serverip((unsigned int)inet_addr("182.254.38.105"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("182.254.99.0"), 24);
		portal_add_wxl_serverip((unsigned int)inet_addr("183.61.38.165"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("183.61.46.128"), 25);
		portal_add_wxl_serverip((unsigned int)inet_addr("183.61.49.128"), 26);
		portal_add_wxl_serverip((unsigned int)inet_addr("183.195.232.64"), 26);
		portal_add_wxl_serverip((unsigned int)inet_addr("183.195.233.78"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("183.232.88.150"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("183.232.93.143"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("183.232.93.208"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("183.232.95.61"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("183.232.98.128"), 26);
		portal_add_wxl_serverip((unsigned int)inet_addr("183.232.103.128"), 26);
		portal_add_wxl_serverip((unsigned int)inet_addr("183.232.125.128"), 26);
		portal_add_wxl_serverip((unsigned int)inet_addr("203.205.128.96"), 28);
		portal_add_wxl_serverip((unsigned int)inet_addr("203.205.129.101"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("203.205.140.32"), 28);
		portal_add_wxl_serverip((unsigned int)inet_addr("203.205.143.153"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("203.205.147.128"), 25);
		portal_add_wxl_serverip((unsigned int)inet_addr("203.205.151.160"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("203.205.151.238"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("203.205.160.0"), 25);
		portal_add_wxl_serverip((unsigned int)inet_addr("203.205.161.0"), 24);
		portal_add_wxl_serverip((unsigned int)inet_addr("203.205.166.152"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("203.205.167.128"), 25);
		portal_add_wxl_serverip((unsigned int)inet_addr("203.205.179.152"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("220.249.243.39"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("119.147.74.101"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("119.147.74.30"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("119.147.74.78"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("14.17.32.221"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("183.61.38.202"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("183.61.38.193"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("219.133.60.208"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("219.133.60.203"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("219.133.60.201"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("219.133.60.204"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("112.90.83.94"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("58.250.135.110"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("58.250.135.111"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("163.177.73.40"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("163.177.83.180"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("163.177.83.187"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("163.177.83.183"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("182.254.4.151"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("183.232.88.163"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("183.232.119.140"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("183.232.125.166"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("112.90.83.94"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("163.177.65.194"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("163.177.65.191"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("163.177.73.40"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("163.177.83.180"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("163.177.83.187"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("163.177.83.183"), 32);
		portal_add_wxl_serverip((unsigned int)inet_addr("163.177.65.188"), 32);
	}

	return;
}

void wifidog_update_wxl_whitelist(void)
{
#define WHITE_LIST_FILE	"/tmp/wxl_white_list.dat"

	char buf[256];
	char *pt, *pt2;
	char ip[128];
	char mask[32];
	FILE *fp;
	fp = fopen(WHITE_LIST_FILE, "r");
	if(!fp)
		return;

	portal_flush_wxl_serverip();

	while(fgets(buf, 256, fp) != NULL)
	{
		if((pt = strstr(buf, "=> ")) != NULL){
			if((pt2 = strstr(pt, "\n")) != NULL)
				*pt2 = '\0';

			pt += strlen("=> ");
			strncpy(ip, pt, 128);
			if((pt = strstr(ip, "/")) != NULL){
				*pt = '\0';
				strncpy(mask, pt+1, 32);
				portal_add_wxl_serverip((unsigned int)inet_addr(ip), atoi(mask));
			}
			else{
				portal_add_wxl_serverip((unsigned int)inet_addr(ip), 32);
			}
		}
	}

	fclose(fp);

	return;
}


/*end: add@2015.01.31 for wechat connect wifi*/

static void conf_set_default_whitelist(void)
{
	int iRet, iLoop;
	char value[512];
	char name[64];
      char gateway[32] = {0};   /* 网关 */
      char *pOption43Ip;
	unsigned int uiOption43;
	static int def_wxl_list = 0;


    for (iLoop = 0; iLoop < LAN_MAX; ++iLoop)
    {
        if(iLoop == 0)
            sprintf(name, "network.lan.ipaddr");
        else
            sprintf(name, "network.lan%d.ipaddr", iLoop);
            
        iRet = sys_config_get(gateway, name);
        if(iRet > 0){
        log_info(__LOG_ERROR__, "get dhcp gateway %s", gateway);

		/*把DHCP地址池的LAN口IP加入白名单*/
		if (0 != portal_add_dest_ip_white_list((unsigned int)inet_addr(gateway), 0))
			log_info(__LOG_ERROR__, "portal_add_dest_ip_white_list %s failed", gateway);
        }

        if(iLoop==0)
            sprintf(name, "dhcp.lan.dhcp_option");
        else
            sprintf(name, "dhcp.@dhcp[%d].dhcp_option", iLoop);
        
        iRet = sys_config_get(value, name);
        if(iRet > 0){
            pOption43Ip = strstr(value,"43,");
                if(pOption43Ip){
                uiOption43 = (unsigned int)inet_addr(pOption43Ip+3);
                	if((uiOption43 != INADDR_NONE) && (uiOption43 > 0)){
                		/*把Option 43的IP加入白名单*/
                		if (0 != portal_add_dest_ip_white_list(uiOption43, 0))
                			log_info(__LOG_ERROR__, "portal_add_dest_ip_white_list %s failed", pOption43Ip);			
                	}
             }

        }
        
    }

	

#ifdef GW_AC
	/*允许AP组播发现AC的报文*/
	if (0 != portal_add_dest_ip_white_list((unsigned int)inet_addr("224.7.11.2"), 0))
		log_info(__LOG_ERROR__, "portal_add_dest_ip_white_list 224.7.11.2 failed");
#endif

	/*begin: add@2015.01.30 for wechat connect wifi*/
	/*将wxl 的lan ip加入白名单*/
	if (0 != portal_add_dest_ip_white_list((unsigned int)inet_addr(WXL_ADDRESS), 0))
			log_info(__LOG_ERROR__, "portal_add_dest_ip_white_list %s failed", value);

	if (auth_mode_support_wxl() || auth_mode_support_weixin()){
		if(!def_wxl_list){ /*第一次获取配置时先设置默认的wxl白名单*/
			log_info(__LOG_DEBUG__, "set default wxl whitelist");
			portal_flush_wxl_serverip();
			wifidog_set_default_wxl_serverip();

			/*下载最新白名单，完毕后更新*/
			wifidog_download_wxl_whitelist();
			def_wxl_list = 1;
		}
	}
	/*end: add@2015.01.30 for wechat connect wifi*/

    return;
}


/*其他URL*/
/*支持视频解码的JS*/
st_url url_others[] = 
{
    {"html5media.googlecode.com"}
};
static void conf_set_other_url(void)
{
    int loop;
    
    for(loop = 0; loop < sizeof(url_others)/sizeof(st_url); loop++)
    {
        if (0 != portal_add_url_free_auth_list(url_others[loop].url))
            log_info(__LOG_ERROR__, "portal_add_url_free_auth_list failed");
    }

	/*华讯方舟具体需求，ip:182.254.190.28 MUST_REDIRECT*/
    if (0 != portal_add_dest_ip_must_redirect_list((unsigned)inet_addr("182.254.190.28"), 80))
            log_info(__LOG_ERROR__, "portal_add_dest_ip_must_redirect_list failed");
    
    return;
}

static int _scan_distance_to_rssi(int scan_m/*感知距离，单位米*/)
{
	int rssi;
	if(scan_m > 90)
		rssi = 5;
	else if((scan_m >= 35) && (scan_m <= 90)){
		rssi = ((235 - (2*scan_m))/11);
	}
	else if((scan_m >= 20) && (scan_m < 35)){
		rssi = ((80 - scan_m)/3);
	}
	else if((scan_m >= 9) && (scan_m < 20)){
		rssi = ((420 - (10*scan_m))/11);
	}
	else if((scan_m >= 5) && (scan_m < 9)){
		rssi = 39 - scan_m;
	}
	else
		rssi = (113 - (9 * scan_m))/2;
	
	rssi += NOISE_FLOOR;

	return rssi;
}

static void conf_handle(char *mark, char *res, int *config_change_flag)
{
    int code;
    int loop;
	
	int before_switch = config_get_config()->wan_flow_report_switch;

    unsigned char mac[6];
    unsigned char ip[17];
    unsigned short port;
	
    char *index, *pt;

    code = -1;
    for(loop = 0; loop < EN_OPCODE_MAX; loop++)
    {
        if(0 == strcasecmp(mark, words[loop].name))
        {
            code = words[loop].code;
        }
    }

    switch(code)
    {
        case EN_QOSENABLE:
			log_info(__LOG_DEBUG__, "CONFIG:EN_QOSENABLE ");
            if(strstr(res, "yes")){
                config_get_config()->qos_enable = 1;
				fw_qos_reinit();
            }
            else
            {
                config_get_config()->qos_enable = 0;
                /* BEGIN: Added by WeiHonggang, 2014/4/9   PN:BUG #5289  云平台关闭流控，终端重新认证，仍被限速 */
                if (0 != fw_qos_flush())
                    log_info(__LOG_ERROR__, "fw_qos_flush() failed");                
                /* END:   Added by WeiHonggang, 2014/4/9   PN:BUG #5289  云平台关闭流控，终端重新认证，仍被限速 */
            }
            break;
		
	    case EN_QOSUP:
			if (config_get_config()->bw_up != atoi(res)){
				config_get_config()->config_change_flag[CONFIG_CHANGE_BW_UP] = 1;
			}
			config_get_config()->bw_up = atoi(res);
			log_info(__LOG_DEBUG__, "CONFIG:EN_QOSUP :%d", config_get_config()->bw_up);
			break;
		case EN_QOSDOWN:
			if (config_get_config()->bw_down != atoi(res)){
				config_get_config()->config_change_flag[CONFIG_CHANGE_BW_UP] = 1;
			}
			config_get_config()->bw_down = atoi(res);
			config_get_config()->config_change_flag[CONFIG_CHANGE_BW_DOWN] = 1;
			log_info(__LOG_DEBUG__, "CONFIG:EN_QOSDOWN :%d", config_get_config()->bw_down);
			break;
		
        case EN_TRUSTEDMACLIST:
            mac_converter(res, mac);
            log_info(__LOG_DEBUG__,  "CONFIG:EN_TRUSTEDMACLIST  "MAC_FMT, MAC_ARG(mac));
            if (0 != portal_add_mac_white_list(mac))
                log_info(__LOG_ERROR__, "portal_add_mac_white_list(%s) failed", mac); 
            break;
        case EN_TRUSTEDIPLIST:
            log_info(__LOG_DEBUG__, "CONFIG:EN_TRUSTEDIPLIST(ip: %s)", res);
            if (0 != portal_add_dest_ip_white_list((unsigned)inet_addr(res), 0)){
                log_info(__LOG_ERROR__, "portal_add_dest_ip_white_list failed");
            }
            break;
        case EN_TRUSTEDWANHOSTLIST:
			log_info(__LOG_DEBUG__, "CONFIG:EN_TRUSTEDWANHOSTLIST(host: %s)", res);
            if (0 != portal_add_url_free_auth_list((unsigned char *)res))
                log_info(__LOG_ERROR__, "portal_add_url_free_auth_list failed");
			else{ /*自动加入url对应的ip名单*/
				struct hostent *he;
				struct in_addr *in_addr_temp;
				he = gethostbyname((char *)res);
				if (he != NULL){
					in_addr_temp = (struct in_addr *)he->h_addr_list[0];
					if (0 != portal_add_url_ip_white_list((unsigned char *)res, ntohl(in_addr_temp->s_addr)))
		                log_info(__LOG_ERROR__, "portal_add_url_ip_white_list failed");
				}
			}
            break;
        case EN_BLACKIPLIST:
            sscanf(res, "%[^:]:%hd", ip, &port);
            log_info(__LOG_DEBUG__, "CONFIG:EN_BLACKIPLIST: %s %d", ip, port);
            if (0 != portal_add_dest_ip_black_list((unsigned)inet_addr((char *)ip), port))
                log_info(__LOG_ERROR__, "portal_add_dest_ip_black_list(%s, %d) failed", ip, port);
            break;
        case EN_BLACKWANHOSTLIST:
            index = strstr(res, ":");
            if(NULL != index)
            {
                *index = '\0';
				log_info(__LOG_DEBUG__, "CONFIG:EN_BLACKWANHOSTLIST(host: %s)", res);
	            if (0 != portal_add_url_black_list((unsigned char *)res))
	                log_info(__LOG_ERROR__, "portal_add_url_black_list(%s) failed", res);
				*index = ':';
            }
			else{
				log_info(__LOG_DEBUG__, "CONFIG:EN_BLACKWANHOSTLIST(host: %s)", res);
	            if (0 != portal_add_url_black_list((unsigned char *)res))
	                log_info(__LOG_ERROR__, "portal_add_url_black_list(%s) failed", res);
			}
            break;
        case EN_BLACKMACLIST:
            mac_converter(res, mac);
            log_info(__LOG_DEBUG__,  "CONFIG:EN_BLACKMACLIST "MAC_FMT, MAC_ARG(mac));
            if (0 != portal_add_mac_black_list(mac))
                log_info(__LOG_ERROR__, "portal_add_mac_black_list failed");
            break;
        case EN_FREEAUTHMACLIST:
            mac_converter(res, mac);
            log_info(__LOG_DEBUG__,  "CONFIG:EN_FREEAUTHMACLIST "MAC_FMT, MAC_ARG(mac));
            if (0 != portal_add_mac_free_auth(mac))
                log_info(__LOG_ERROR__, "portal_add_mac_free_auth failed");
            break;
        case EN_FREEAUTHIPLIST:
            log_info(__LOG_DEBUG__, "CONFIG:EN_FREEAUTHIPLIST: %s", res);
            if (0 != portal_add_ip_free_auth((unsigned)inet_addr(res)))
                log_info(__LOG_ERROR__, "portal_add_ip_free_auth failed");
            break;
        case EN_MAXSTATIONNUM:
            log_info(__LOG_DEBUG__, "CONFIG:EN_MAXSTATIONNUM: %s", res);
            config_get_config()->max_client = atoi(res);
            break;
		/*begin: add@2014.8.1*/
		case EN_FORCETIMEOUT:
			if (config_get_config()->forcetimeout != atoi(res)){
				config_get_config()->config_change_flag[CONFIG_CHANGE_BW_UP] = 1;
			}
			config_get_config()->forcetimeout = atoi(res);
			config_get_config()->config_change_flag[CONFIG_CHANGE_FORCETIMEOUT] = 1;
			log_info(__LOG_DEBUG__, "CONFIG:EN_FORCETIMEOUT: %lu", config_get_config()->forcetimeout);		
			break;
		case EN_IDLETIMEOUT:
			if (config_get_config()->idletimeout != atoi(res)){
				config_get_config()->config_change_flag[CONFIG_CHANGE_BW_UP] = 1;
			}
			config_get_config()->idletimeout = atoi(res);
			config_get_config()->config_change_flag[CONFIG_CHANGE_IDLETIMEOUT] = 1;
			log_info(__LOG_DEBUG__, "CONFIG:EN_IDLETIMEOUT: %lu", config_get_config()->idletimeout);
			break;
		/*end: add@2014.8.1*/
		/*begin: add@2014.8.26*/
		case EN_WLANFLOWSTATSWITCH:/*0:关闭，1:开启*/
			config_get_config()->wan_flow_report_switch = atoi(res);
			log_info(__LOG_DEBUG__, "CONFIG:EN_WLANFLOWSTATSWITCH: %u", config_get_config()->wan_flow_report_switch);
			if (0 == config_get_config()->wan_flow_report_switch && 1 == before_switch){
				event_map_cannel(EVENT_REPORT_WAN_FLOWING);
			}else if(1 == config_get_config()->wan_flow_report_switch && 0 == before_switch){
				event_map_register(EVENT_REPORT_WAN_FLOWING, 
									(config_get_config()->checkinterval)*(config_get_config()->wan_flow_report_interval), 
									0, wan_flowing_report_request, NULL);
			}
			break;
		/*end: add@2014.8.26*/
        case EN_POSTTRUSTEDWANHOSTLIST:
			log_info(__LOG_DEBUG__, "CONFIG:EN_POSTTRUSTEDWANHOSTLIST: %s", (unsigned char *)res);
            if (0 != portal_add_url_white_list((unsigned char *)res))
                log_info(__LOG_ERROR__, "portal_add_url_white_list failed");
            break;
        /* BEGIN: Added by WeiHonggang, 2014/2/18   PN:TASK#0平台支持认证模式同步到网关，并根据认证模式进行设置 */
        case EN_AUTHMODE:
            auth_mode[auth_mode_index] = ((NULL != strstr(res,"1")) ? AUTH_MODE_SUPPORT:AUTH_MODE_NOT_SUPPORT);
            auth_mode_index ++;
            auth_mode_index%=AUTH_MODE_MAX;
            break;
        /* END:   Added by WeiHonggang, 2014/2/18   PN:TASK#0平台支持认证模式同步到网关，并根据认证模式进行设置 */
		case EN_STADCTEN:
			sta_dct_enable((NULL != strstr(res,"yes")) ? 1 : 0);
			break;
		case EN_STADCTRSSI:
			pt = strstr(res, " ");
			if(pt != NULL){
				int rssi = _scan_distance_to_rssi(atoi(pt+1));
				sta_dct_rssi(rssi);
			}
			break;
		case EN_STADCTRPTTIME:
			pt = strstr(res, " ");
			if(pt != NULL){
				int interval = atoi(pt+1);
				syslog(LOG_INFO, "Set sta dct report interval(%d)", interval);
				if((g_stadct_rpt_time != interval) && (interval > 0)){
					event_map_update(EVENT_REPORT_STADCT, 
					((config_get_config()->checkinterval)*interval/60), 
					0, stadct_list_report, NULL);
					
					g_stadct_rpt_time = interval;
				}
			}
			break;
        /*add@2014.11.02 for 必须要重定向url名单*/
        case EN_MUSTREDIRECTURLIST:
            log_info(__LOG_DEBUG__, "CONFIG:EN_MUSTREDIRECTURLIST: %s", (unsigned char *)res);
            if (0 != portal_add_url_must_redirect_list((unsigned char *)res))
                log_info(__LOG_ERROR__, "portal_add_url_must_redirect_list failed");
            break;
		case EN_DEVMAXSTATIONNUM:
            log_info(__LOG_DEBUG__, "CONFIG:EN_DEVMAXSTATIONNUM: %s", res);
            config_get_config()->dev_max_client = atoi(res);
            break;
		/*begin: add@2015.01.28 for wechat connect wifi*/
		case EN_WXLSERVERIP:
			log_info(__LOG_DEBUG__, "CONFIG:EN_WXLSERVERIP(ip: %s)", res);
            if (0 != portal_add_dest_ip_white_list((unsigned)inet_addr(res), 0))
                log_info(__LOG_ERROR__, "portal_add_dest_ip_white_list failed");
		/*end: add@2015.01.28 for wechat connect wifi*/
		default:
            log_info(__LOG_ERROR__, "Could not find code{%d}", code);
            break;
    }
    return;
}

/*************************** 上报接口 ***************************/

/**@ 终端统计 */
void client_list_statistics(void)
{
    int loop;
    unsigned int count;
    unsigned int num;
    char ip[32];
    char mac[32];
    t_client *ptr = NULL;
    PORTAL_STA_INFO_T pa_sta[8];
    struct in_addr in;

    num = 0;
    count = 8;
    while (8 == count)
    {
        memset(pa_sta, 0, sizeof(PORTAL_STA_INFO_T));
        count = 0;
        if(-1 == portal_get_sta_list(&num, &count, (void *)pa_sta))
        {
            log_info(__LOG_ERROR__, "get nf_portal client list failed.");
            return;
        }
        for(loop = 0; loop < count; loop++)
        {
            in.s_addr = pa_sta[loop].uiIp;
            strcpy(ip, inet_ntoa(in));
            sprintf(mac, MAC_FMT, MAC_ARG(pa_sta[loop].aucMac));
            log_info(__LOG_DEBUG__, "client ip %s, mac %s", ip, mac);
            LOCK_CLIENT_LIST();
            ptr = client_list_find(ip, mac);
            if(!ptr)
            {
                portal_set_auth_state(pa_sta[loop].aucMac, pa_sta[loop].uiIp, 0);
            }
            else
            {
            	unsigned long long	uiByteTemp;
                ptr->counters.incoming_history = ptr->counters.incoming;
                ptr->counters.outgoing_history = ptr->counters.outgoing;
				
				uiByteTemp = pa_sta[loop].ui4GByteDown;
				uiByteTemp <<= 32;
                ptr->counters.incoming = (unsigned long long)pa_sta[loop].uiByteDown + uiByteTemp;

				uiByteTemp = pa_sta[loop].ui4GByteUp;
				uiByteTemp <<= 32;
                ptr->counters.outgoing = (unsigned long long)pa_sta[loop].uiByteUp + uiByteTemp;
				
                if(ptr->counters.incoming != ptr->counters.incoming_history
                && ptr->counters.outgoing != ptr->counters.outgoing_history)
                {
                    ptr->counters.last_updated = get_uptime();
                }
            }
            UNLOCK_CLIENT_LIST();
        }
    }

    return;
}



void read_src_loacl_ver()
{
    FILE *fh = NULL;
     //CLOUD_SRC_VER_STORE_FILE 中存放了上次下载的版本号
    if ((fh = fopen(CLOUD_SRC_VER_STORE_FILE, "r"))) {  /* 获取最近一次下载资源的版本号，初始时(包括切换云平台)所有的版本号均为0 */
		if (3 != fscanf(fh, "%d\n%d\n%d", 
            &g_gw_src_ver.iTpl_ver, 
            &g_gw_src_ver.iProduct_ver, 
            &g_gw_src_ver.iAd_ver))
        {      
            memset(&g_gw_src_ver, 0, sizeof(cloud_src_ver_t));
            log_info(__LOG_FAILED__, "can not sscanf %s", CLOUD_SRC_VER_STORE_FILE);
        }
        log_info(__LOG_DEBUG__, "download src fscanf 3  iTpl_ver=%d iProduct_ver=%d iAd_ver=%d \n\n", 
            g_gw_src_ver.iTpl_ver,
            g_gw_src_ver.iProduct_ver,
            g_gw_src_ver.iAd_ver);
		fclose(fh);
	}
    return;
}
/*****************************************************************************
 函 数 名  : download_src
 功能描述  : Portal 下沉，从云平台下载文件到网关本地，调用shell脚本下载
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年12月18日
    作    者   : zhanghaozhong
    修改内容   : 新生成函数

*****************************************************************************/
int download_src()
{
    char acUrl[512];
    
    s_config *config = NULL;

    config = config_get_config();
    
    read_src_loacl_ver();   /* 获取本地资源版本号 */

   
    /* 1. 判断模板的版本号是否需要下载 */
    /*

    http://server/cmps/static/zip/tpl/tpl_xx.zip                    xx:云平台维护的模板版本号 
    http://server/cmps/static/zip/product/product_yy_xx.zip         xx:云平台维护的模板版本号  yy:gw_id
    http://server/cmps/static/zip/adver/adver_yy_xx.zip             xx:云平台维护的模板版本号  yy:gw_id

    */
    if (((0 == g_gw_src_ver.iTpl_ver) || (g_cloud_src_ver.iTpl_ver != g_gw_src_ver.iTpl_ver))
        && 0 != g_cloud_src_ver.iTpl_ver)
    {
        //downloadcloudsrc.sh url save_path version src_type
        sprintf(acUrl, "downloadcloudsrc.sh %s:%d/cmps/static/zip/tpl/tpl_%d.zip "LIGHTTPD_ROOT_DOC""TPL_SRC_FILE" %d tpl tpl_%d.zip 1>/dev/null 2>&1 &",
                        config->auth_servers->authserv_hostname,
                        config->auth_servers->authserv_http_port,
                        g_cloud_src_ver.iTpl_ver,
                        g_cloud_src_ver.iTpl_ver,
                        g_cloud_src_ver.iTpl_ver);
        log_info(__LOG_MSG__, "download src from cloud tpl, cloud tpl ver=%d, local tpl ver=%d",
            g_cloud_src_ver.iTpl_ver,
            g_gw_src_ver.iTpl_ver);
        log_info(__LOG_DEBUG__, "acUrl: %s", acUrl);        
        system(acUrl);
    }
    /* 2. 判断产品的版本号是否需要下载 */
    if (((0 == g_gw_src_ver.iProduct_ver) || (g_cloud_src_ver.iProduct_ver != g_gw_src_ver.iProduct_ver))
        && 0 != g_cloud_src_ver.iProduct_ver)
    {
        sprintf(acUrl, "downloadcloudsrc.sh %s:%d/cmps/static/zip/product/product_%s_%d.zip "LIGHTTPD_ROOT_DOC""PRODUCT_SRC_FILE" %d product product_%s_%d.zip 1>/dev/null 2>&1 &",
                        config->auth_servers->authserv_hostname,
                        config->auth_servers->authserv_http_port,
                        config->gw_id,
                        g_cloud_src_ver.iProduct_ver,
                        g_cloud_src_ver.iProduct_ver,
                        config->gw_id,
                        g_cloud_src_ver.iProduct_ver);
        log_info(__LOG_MSG__, "download src from cloud product, cloud product ver=%d, local product ver=%d",
            g_cloud_src_ver.iProduct_ver,
            g_gw_src_ver.iProduct_ver);
        log_info(__LOG_DEBUG__, "acUrl: %s", acUrl);
        system(acUrl);
    }
    /* 3. 判断广告的版本号是否需要下载 */
    if (((0 == g_gw_src_ver.iAd_ver) || (g_cloud_src_ver.iAd_ver != g_gw_src_ver.iAd_ver))
        && 0 != g_cloud_src_ver.iAd_ver)
    {
 
        sprintf(acUrl, "downloadcloudsrc.sh %s:%d/cmps/static/zip/adver/adver_%s_%d.zip "LIGHTTPD_ROOT_DOC""AD_SRC_FILE" %d adv adver_%s_%d.zip 1>/dev/null 2>&1 &",
                        config->auth_servers->authserv_hostname,
                        config->auth_servers->authserv_http_port,
                        config->gw_id,
                        g_cloud_src_ver.iAd_ver,
                        g_cloud_src_ver.iAd_ver,
                        config->gw_id,
                        g_cloud_src_ver.iAd_ver);
        log_info(__LOG_MSG__, "download src from cloud adver, cloud adver ver=%d, local adver ver=%d",
            g_cloud_src_ver.iAd_ver,
            g_gw_src_ver.iAd_ver);
        log_info(__LOG_DEBUG__, "acUrl: %s", acUrl);
        system(acUrl);
    }
    return 0;
}

/**
 * @设备向平台发送保活request,
 * @URL:http://auth_server/ping/?gw_id=&sys_uptime=&sys_memfree=&sys_load=&wifidog_uptime=
        res_tpl_version=&res_product_version=&res_adver_version=&userversion=&totalusernum=
 */
void ping_request(int *sockfd)
{
	char req[MAX_BUF];
	FILE* fh;
	unsigned long int sys_uptime  = 0;
	unsigned int      sys_memfree = 0;
	float             sys_load    = 0;

    s_config *config = config_get_config();
	
	//long long conf_ver = 0;
	//long long cmd_ver = 0;
	t_auth_serv	*auth_server = NULL;
	
	auth_server = get_auth_server();
    
    /* BEGIN: Added by cj, 2015/3/30   问题单号:获取本地配置 */
	get_local_config();
	/* END:   Added by cj, 2015/3/30 */
	
	ping_timeouts++;
	//是否需要设置下线标记, 这里检测原因是无法建立server连接,则不会进入ping_response
    if(ping_timeouts > config->pingtimeouts
        && ON_LINE == is_online())
    {
        //* 认为设备离线, 配置为免认证
        sys_log_set(WIFIDOG_SERVER_LOG_FILE, "Set NF free-auth by ping_request timeout.");
        fw_set_free_auth();
		sys_log_set(WIFIDOG_SERVER_LOG_FILE, "Set off-line by ping_request timeout.");
        set_is_online(OFF_LINE);
        log_info(__LOG_FAILED__, "the device is off line.");
    }
	log_info(__LOG_DEBUG__, "Entering ping_request()");

	/*
	 * Populate uptime, memfree and load
	 */
	if ((fh = fopen("/proc/uptime", "r"))) {
		fscanf(fh, "%lu", &sys_uptime);
		fclose(fh);
	}
	if ((fh = fopen("/proc/meminfo", "r"))) {
		while (!feof(fh)) {
			if (fscanf(fh, "MemFree: %u", &sys_memfree) == 0) {
				/* Not on this line */
				while (!feof(fh) && fgetc(fh) != '\n');
			}
			else {
				/* Found it */
				break;
			}
		}
		fclose(fh);
	}
	if ((fh = fopen("/proc/loadavg", "r"))) {
		fscanf(fh, "%f", &sys_load);
		fclose(fh);
	}

    read_src_loacl_ver();   /* 获取本地资源版本号 add by zhz */

	/*
	 * Prep & send req
	 */
	snprintf(req, sizeof(req) - 1,
			"GET %s%sgw_id=%s&sys_uptime=%lu&sys_memfree=%u&sys_load=%.2f&wifidog_uptime=%lu"
			"&res_tpl_version=%d&res_product_version=%d&res_adver_version=%d&userversion=%lld&totalusernum=%d&model=%s&mac=%s&version=1.0 HTTP/1.0\r\n"
			"User-Agent: WiFiDog %s\r\n"
			"Host: %s\r\n"
			"\r\n",
			auth_server->authserv_path,
			auth_server->authserv_ping_script_path_fragment,
			config->gw_id,
			sys_uptime,
			sys_memfree,
			sys_load,
			(long unsigned int)((long unsigned int)get_uptime() - (long unsigned int)started_time),
			g_gw_src_ver.iTpl_ver,
			g_gw_src_ver.iProduct_ver,
			g_gw_src_ver.iAd_ver,
			/*add@2014.10.11 add user version and current user num*/
			config->iUser_ver,
			g_client_access_num,
			config->model,
			config->wan_mac,
			VERSION,
			auth_server->authserv_hostname);

    *sockfd = connect_auth_server();
    if(SOCKET_INVALID == *sockfd)
    {
        log_info(__LOG_WARNING__, "Creating auth socket failed");
        return;
    }
	log_info(__LOG_DEBUG__, "HTTP request[ping] to Server: [%s]", req);
	send(*sockfd, req, strlen(req), 0);
	return;
}


static void send_client_list(int client_num, char *client_list, int *sockfd, s_config *config, char *send_type)
{
    char req[MAX_BUF*2];
#if 0
    fd_set fds;
    struct timeval timeout;
#endif
    if(0 == client_num)
        return;

    client_list[strlen(client_list)-1] = '\0';
    memset(req, 0, sizeof(MAX_BUF*2));
    snprintf(req, sizeof(req)-1,
            "GET %s%sstage=%s&client_num=%d&client_list=%s&gw_id=%s&version=1.0 HTTP/1.0\r\n"
            "User-Agent: WiFiDog %s\r\n"
    		"Host: %s\r\n"
    		"\r\n",
    		config->auth_servers->authserv_path,
    		config->auth_servers->authserv_auth_script_path_fragment,
    		send_type,
    		client_num,
    		client_list,
    		config->gw_id,
    		VERSION,
    		config->auth_servers->authserv_hostname);    

    log_info(__LOG_DEBUG__, "HTTP Request[send client list] to Server: [%s]", req);
#if 0
    if(-1 == send(*sockfd, req, strlen(req), 0))
    {
        if(SOCKET_INVALID != *sockfd)
        {
            close(*sockfd);
            *sockfd = SOCKET_INVALID;
        }
        *sockfd = connect_auth_server();
        if(SOCKET_INVALID == *sockfd)
        {
            log_info(__LOG_WARNING__, "Creating auth socket failed");
            return;
        }
        send(*sockfd, req, strlen(req), 0);
    }
#endif
    /**@ 修改发送流程, 每次发送等待回复, 确认超时 */
    if(SOCKET_INVALID != *sockfd)
        close(*sockfd);

    *sockfd = connect_auth_server();
    if(SOCKET_INVALID == *sockfd)
    {
        log_info(__LOG_WARNING__, "Creating auth socket failed");
        return;
    }
    send(*sockfd, req, strlen(req), 0);

	/*begin: modifed@2014.10.10 for delete user info when recv response*/
    #if 0
    /** 等待平台确认消息 */
    FD_ZERO(&fds);
    FD_SET(*sockfd, &fds);
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    select(*sockfd+1, &fds, NULL, NULL, &timeout);
    /** 关闭 socket */
    close(*sockfd);
    *sockfd = SOCKET_INVALID;
    #endif
    /*end: modifed@2014.10.10 for delete user info when recv response*/

    return;
}

/**@ 终端强行下线请求， gw->云平台   终端下线通知云平台*/
void client_list_deny_report(int *sockfd)
{
    char temp[256];
    char client_list[2048];
    int client_num;
    int loop;
    //time_t now, idletimeout;
    long now, idletimeout;
    t_client *p1, *p2;
    s_config *config = config_get_config(); 
	char logbuf[512];

    /** @统计终端列表 */
    client_list_statistics();

    client_num = 0;
    memset(client_list, 0, sizeof(client_list));
    now = get_uptime();
    for(loop = 0; loop < CLIENT_HASH_SIZE; loop++)
    {
        LOCK_CLIENT_LIST();
        p1 = g_client_head[loop].next;
        while(p1)
        {
            p2 = p1->next;
            /*add@2014.10.10*/
			if (p1->denied_client_flag)
                p1->denied_client_flag = 0;
#if 0 //中传需求，code:liwen
            idletimeout = config->checkinterval * config->clienttimeout;
            if(p1->idletimeout > 0)
            {
                idletimeout = p1->idletimeout;
            }
#else
            idletimeout = config->clienttimeout;
#endif
			/*modified: 2014.8.4 forcetimeout */
			/*
           			 if(((p1->forcetimeout>0)&&(p1->login_time + p1->forcetimeout <= now)) ||
            				((p1->counters.incoming == p1->counters.incoming_history)&&(p1->counters.outgoing == p1->counters.outgoing_history)
            	 			&& (p1->forcetimeout>0&&(p1->counters.last_updated + idletimeout <= now))
            				)	
              			)*/
			
            p1->online_time += (now - p1->online_time_last_update_time);
			p1->online_time_last_update_time = now;
            if(((p1->forcetimeout>0)&&(p1->online_time >= p1->forcetimeout)) ||
 				((p1->counters.incoming == p1->counters.incoming_history)&&(p1->counters.outgoing == p1->counters.outgoing_history)
 	 			&& (p1->forcetimeout>0&&(p1->counters.last_updated + idletimeout <= now))) ||
 	 			(1 == p1->set_logout)
 	 			)
            {
            	/*client offline for force timeout log*/
            	if ((p1->forcetimeout>0)&&(p1->online_time >= p1->forcetimeout)){
					log_info(__LOG_DEBUG__, "client(ip %s mac %s) is force-timeout, forcetimeout:%lu, online-time:%lu", 
											 p1->ip, p1->mac, p1->forcetimeout, p1->online_time);
					sprintf(logbuf, "nf-DENY: %s %s forcetimeout:%lu, online-time:%lu",
						p1->ip, p1->mac, p1->forcetimeout, p1->online_time);
				}
				/*client offline for idle timeout log*/
				if ((p1->counters.incoming == p1->counters.incoming_history)&&(p1->counters.outgoing == p1->counters.outgoing_history)
            	     && (p1->forcetimeout>0&&(1 == config->iAgeingSwitch)&&(p1->counters.last_updated + idletimeout <= now))
            	   ){
					log_info(__LOG_DEBUG__, "client(ip %s mac %s) is idle-timeout, incoming:%llu, outgoing:%llu, idletimeout:%lu", 
											 p1->ip, p1->mac, p1->counters.incoming, p1->counters.outgoing, idletimeout);
					sprintf(logbuf, "nf-DENY: %s %s idletimeout:%lu", p1->ip, p1->mac, idletimeout);
    				/*begin: add@2015.04.20*/
                    nf_fw_access(FW_ACCESS_DENY, p1->ip, p1->mac);
                    wifidog_aglink_send_del_sta(p1);
                    client_list_delete(p1);
                    /*end: add@2015.04.20*/
				}
                sys_log_set(WIFIDOG_USR_LOG_FILE, logbuf);
				/*begin: add@2014.10.10*/
                #if 0
                nf_fw_access(FW_ACCESS_DENY, p1->ip, p1->mac);
                if(config->qos_enable)
                    fw_qos_del(p1->mac);
                #endif
                p1->denied_client_flag = 1;
                /*end: add@2014.10.10*/
                sprintf(temp, "%s|%s|%s|%lld|%lld;", 
                    p1->ip, p1->mac,
                    p1->token, p1->counters.incoming/1024, p1->counters.outgoing/1024);
                if(strlen(client_list)+strlen(temp) > sizeof(client_list))
                {
                    send_client_list(client_num, client_list, sockfd, config, "logout");
                    client_num++;
                    memset(client_list, 0, sizeof(client_list));                    
                }
                strcat(client_list, temp);
                client_num++;
                /*modified@2014.10.10*/
                //client_list_delete(p1);
            }
            p1 = p2;
        }
        UNLOCK_CLIENT_LIST();
    }
	log_info(__LOG_DEBUG__, "client_list_deny_report");
    send_client_list(client_num, client_list, sockfd, config, "logout");
	return;
}

/**@ 获取终端列表请求 */
void client_list_request_report(int *sockfd, int begin)
{
    char req[MAX_BUF];
    s_config *config = config_get_config();

    memset(req, 0, sizeof(MAX_BUF));
    snprintf(req, sizeof(req)-1,
            "GET %s%sgw_id=%s&begin=%d&version=1.0 HTTP/1.0\r\n"
            "User-Agent: WiFiDog %s\r\n"
			"Host: %s\r\n"
			"\r\n",
			config->auth_servers->authserv_path,
			config->auth_servers->authserv_client_script_path_fragment,
			config->gw_id,
			begin,
			VERSION,
			config->auth_servers->authserv_hostname);

	*sockfd = connect_auth_server();
    if(SOCKET_INVALID == *sockfd)
    {
        log_info(__LOG_WARNING__, "Creating auth socket failed");
        return;
    }
    log_info(__LOG_DEBUG__, "HTTP[client_list_request] Request to Server: [%s]", req);
	send(*sockfd, req, strlen(req), 0);
	return;
}

/**@ 上报终端信息 */
void client_list_report(int *sockfd)
{
    int loop;
    t_client *p1;

    int client_num;
    char client_list[7168];
    char temp[256];
    
    s_config *config = config_get_config();    

    /** @统计终端列表 */
    client_list_statistics();

    memset(client_list, 0, sizeof(client_list));
    client_num = 0;
    for(loop = 0; loop < CLIENT_HASH_SIZE; loop++)
    {
        LOCK_CLIENT_LIST();
        p1 = g_client_head[loop].next;
        while(p1)
        {      
            sprintf(temp, "%s|%s|%s|%lld|%lld;", 
                    p1->ip, p1->mac,
                    p1->token, p1->counters.incoming/1024, p1->counters.outgoing/1024);

            if(strlen(client_list)+strlen(temp) > sizeof(client_list))
            {
                send_client_list(client_num, client_list, sockfd, config, "counters");
                client_num = 0;
                memset(client_list, 0, sizeof(client_list));
            }
                    
            strcat(client_list, temp);
            client_num++;
            p1 = p1->next;
        }
        UNLOCK_CLIENT_LIST();
    }
	log_info(__LOG_DEBUG__, "client_list_report");
    send_client_list(client_num, client_list, sockfd, config, "counters");
	return;
}


static void send_ap_list(int ap_num, char *ap_list, int *sockfd, void *cfg, char *send_type)
{
    char req[MAX_BUF*2];

    fd_set fds;
    struct timeval timeout;
	s_config *config = (s_config *)cfg;
	log_info(__LOG_DEBUG__, "");
    if(0 == ap_num)
        return;
	log_info(__LOG_DEBUG__, "ap_list: %s", ap_list);
	ap_list[strlen(ap_list)-1] = '\0';
    memset(req, 0, sizeof(MAX_BUF*2));
    snprintf(req, sizeof(req)-1,
            "GET %s%sstage=%s&ap_num=%d&ap_list=%s&gw_id=%s&version=1.0 HTTP/1.0\r\n"
            "User-Agent: WiFiDog %s\r\n"
    		"Host: %s\r\n"
    		"\r\n",
    		config->auth_servers->authserv_path,
    		config->auth_servers->authserv_auth_script_path_fragment,
    		send_type,
    		ap_num,
    		ap_list,
    		config->gw_id,
    		VERSION,
    		config->auth_servers->authserv_hostname);    

    log_info(__LOG_DEBUG__, "HTTP Request[send_ap_list] to Server: [%s]", req);
	//syslog(LOG_INFO, "HTTP Request[send_ap_list] to Server: [%s]", req);
	
    /**@ 修改发送流程, 每次发送等待回复, 确认超时 */
    if(SOCKET_INVALID != *sockfd)
        close(*sockfd);
	log_info(__LOG_DEBUG__, "");
    *sockfd = connect_auth_server();
    if(SOCKET_INVALID == *sockfd)
    {
        log_info(__LOG_WARNING__, "Creating auth socket failed");
        return;
    }
	log_info(__LOG_DEBUG__, "");
    send(*sockfd, req, strlen(req), 0);

    /** 等待平台确认消息 */
    FD_ZERO(&fds);
    FD_SET(*sockfd, &fds);
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    select(*sockfd+1, &fds, NULL, NULL, &timeout);
    /** 关闭 socket */
    close(*sockfd);
    *sockfd = SOCKET_INVALID;
    log_info(__LOG_DEBUG__, "");
    return;
}

extern int ap_send_list_report(void *func, void *config, int *sockfd);

/**@ 上报AP信息 */
void ap_list_report(int *sockfd)
{
    s_config *config = config_get_config();    

	ap_send_list_report(send_ap_list, (void *)config, sockfd);
	
	return;
}


static void send_stadct_list(int sta_num, char *sta_list, int *sockfd, void *cfg, char *send_type)
{
    char req[MAX_BUF*2];

    fd_set fds;
    struct timeval timeout;
	s_config *config = (s_config *)cfg;

    if(0 == sta_num)
        return;

	sta_list[strlen(sta_list)-1] = '\0';
    memset(req, 0, sizeof(MAX_BUF*2));
    snprintf(req, sizeof(req)-1,
            "GET %s%sstage=%s&sta_num=%d&sta_list=%s&gw_id=%s&version=1.0 HTTP/1.0\r\n"
            "User-Agent: WiFiDog %s\r\n"
    		"Host: %s\r\n"
    		"\r\n",
    		config->auth_servers->authserv_path,
    		config->auth_servers->authserv_auth_script_path_fragment,
    		send_type,
    		sta_num,
    		sta_list,
    		config->gw_id,
    		VERSION,
    		config->auth_servers->authserv_hostname);    

    log_info(__LOG_DEBUG__, "HTTP Request[send_stadct_list] to Server: [%s]", req);

    /**@ 修改发送流程, 每次发送等待回复, 确认超时 */
    if(SOCKET_INVALID != *sockfd)
        close(*sockfd);

    *sockfd = connect_auth_server();
    if(SOCKET_INVALID == *sockfd)
    {
        log_info(__LOG_WARNING__, "Creating auth socket failed");
        return;
    }
    send(*sockfd, req, strlen(req), 0);

    /** 等待平台确认消息 */
    FD_ZERO(&fds);
    FD_SET(*sockfd, &fds);
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    select(*sockfd+1, &fds, NULL, NULL, &timeout);
    /** 关闭 socket */
    close(*sockfd);
    *sockfd = SOCKET_INVALID;
    
    return;
}


extern int stadct_send_list_report(void *func, void *config, int *sockfd);


/**@ 上报终端探测信息 */
void stadct_list_report(int *sockfd)
{
    s_config *config = config_get_config();    

	stadct_send_list_report(send_stadct_list, (void *)config, sockfd);
	
	return;
}

/**BEGIN: add@2014.8.25 for task#86 上报wan口流量信息*/
void wan_flowing_report_request(int *sockfd)
{
	fd_set fds;
    struct timeval timeout;
	char req[MAX_BUF] = {0};
	s_config *config = config_get_config();
	char cmd[64] = {0};
	char cBuff1[64] = {0};
	char cBuff2[64] = {0};
	FILE *fp1 = NULL;
	FILE *fp2 = NULL;
	unsigned long long total_incoming = 0;
	unsigned long long total_outgoing = 0;
	log_info(__LOG_DEBUG__, "sizeof(unsigned long long) : %u", sizeof(unsigned long long));
	system("echo 0 > /var/wan.up");
	system("echo 0 > /var/wan.down");

	sprintf(cmd, "ifconfig %s | grep bytes | awk '{print $2}' | awk -F : '{print $2}' > /var/wan.up", config->wan_interface);
	system(cmd);
	sprintf(cmd, "ifconfig %s | grep bytes | awk '{print $6}' | awk -F : '{print $2}' > /var/wan.down", config->wan_interface);
	system(cmd);

	fp1 = fopen("/var/wan.up", "r");
	fp2 = fopen("/var/wan.down", "r");
    if (NULL == fp1 || NULL == fp2){
		log_info(__LOG_ERROR__, "fopen /var/wan.up or /var/wan.down fail");
		return;
	}
	if (NULL != fgets(cBuff1, 63, fp1))
	{
		sscanf(cBuff1, "%llu", &total_incoming);
	}
	if (NULL != fgets(cBuff2, 63, fp2))
	{
		sscanf(cBuff2, "%llu", &total_outgoing);
	}
	fclose(fp1);
	fclose(fp2);

	if (( total_incoming >= g_stInterfaceFlowStats.wan_total_incoming )&& 
		( total_outgoing >= g_stInterfaceFlowStats.wan_total_outgoing ) )
	{
		/*正常情况*/
		g_stInterfaceFlowStats.wan_last_report_incoming = total_incoming - g_stInterfaceFlowStats.wan_total_incoming;
		g_stInterfaceFlowStats.wan_last_report_outging = total_outgoing - g_stInterfaceFlowStats.wan_total_outgoing;
		g_stInterfaceFlowStats.wan_total_incoming = total_incoming;
		g_stInterfaceFlowStats.wan_total_outgoing = total_outgoing;
	}else if(( total_incoming < g_stInterfaceFlowStats.wan_total_incoming )&& 
			 ( total_outgoing < g_stInterfaceFlowStats.wan_total_outgoing ) )
	{
		/*说明网关发生过重启*/
		g_stInterfaceFlowStats.wan_total_incoming = total_incoming;
		g_stInterfaceFlowStats.wan_total_outgoing = total_outgoing;	
		g_stInterfaceFlowStats.wan_last_report_incoming = total_incoming;
		g_stInterfaceFlowStats.wan_last_report_outging = total_outgoing;
	}
	else{
		log_info(LOG_WARNING, "wan flow report error");
		return;
	}

	log_info(LOG_DEBUG, "wan_total_incoming: %llu, wan_total_outgoing: %llu", 
						g_stInterfaceFlowStats.wan_total_incoming,
						g_stInterfaceFlowStats.wan_total_outgoing);
	log_info(LOG_DEBUG, "wan_last_report_incoming: %llu, wan_last_report_outging: %llu", 
						g_stInterfaceFlowStats.wan_last_report_incoming,
						g_stInterfaceFlowStats.wan_last_report_outging);
	snprintf(req, sizeof(req)-1,
            "GET %sstage=%d&incoming=%lld&outgoing=%lld&gw_id=%s&version=1.0 HTTP/1.0\r\n"
            "User-Agent: WiFiDog %s\r\n"
    		"Host: %s\r\n"
    		"\r\n",
    		config->auth_servers->authserv_wan_flow_report_script_path_fragment,
    		1,
    		g_stInterfaceFlowStats.wan_last_report_incoming/1024,
    		g_stInterfaceFlowStats.wan_last_report_outging/1024,
    		config->gw_id,
    		VERSION,
    		config->auth_servers->authserv_hostname);    

    log_info(__LOG_DEBUG__, "HTTP Request[wan_flowing_report_request] to Server: [%s]", req);

    if(SOCKET_INVALID != *sockfd)
        close(*sockfd);

    *sockfd = connect_auth_server();
    if(SOCKET_INVALID == *sockfd)
    {
        log_info(__LOG_WARNING__, "Creating auth socket failed");
        return;
    }
    send(*sockfd, req, strlen(req), 0);

    /** 等待平台确认消息 */
    FD_ZERO(&fds);
    FD_SET(*sockfd, &fds);
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    select(*sockfd+1, &fds, NULL, NULL, &timeout);
    /** 关闭 socket */
    close(*sockfd);
    *sockfd = SOCKET_INVALID;
}
/**END: add@2014.8.25 for task#86 上报wan口流量信息*/


void wxl_whitelist_download(int *sockfd)
{
	if(SOCKET_INVALID != *sockfd)
        close(*sockfd);

	*sockfd = SOCKET_INVALID;

	wifidog_download_wxl_whitelist();
	
	return;
}

void conf_request(int *sockfd)
{
    char req[MAX_BUF];
    s_config *config = config_get_config();    

    memset(req, 0, sizeof(MAX_BUF));
    snprintf(req, sizeof(req)-1,
            "GET %s%sgw_id=%s&version=1.0 HTTP/1.0\r\n"
            "User-Agent: WiFiDog %s\r\n"
			"Host: %s\r\n"
			"\r\n",
			config->auth_servers->authserv_path,
			config->auth_servers->authserv_conf_script_path_fragment,
			config->gw_id,
			VERSION,
			config->auth_servers->authserv_hostname);

	*sockfd = connect_auth_server();
    if(SOCKET_INVALID == *sockfd)
    {
        log_info(__LOG_WARNING__, "Creating auth socket failed");
        return;
    }
    log_info(__LOG_DEBUG__, "HTTP Request[conf_request] to Server: [%s]", req);
	send(*sockfd, req, strlen(req), 0);
	return;
}

/*************************** 消息回复处理接口 ***************************/

/**
 *@处理平台回的ping response
 */
void ping_response(char *response)
{
    char *index = NULL;

    log_info(__LOG_DEBUG__, "HTTP Response[ping_response] form server: [%s]", response);
	sys_log_set(WIFIDOG_SERVER_LOG_FILE, "\n[Ping_response] form server: [%s]", response);
	
	/*Pong 报文处理*/
	if (NULL != (index = strstr(response, "Pong"))){
		log_info(__LOG_DEBUG__, "Auth server says Pong!");
        //1. 设置上线标记和 freeauth关闭 
        ping_timeouts = 0;
        if(OFF_LINE == is_online())
        {   
        	sys_log_set(WIFIDOG_SERVER_LOG_FILE, "Set auth by ping_response.");
			fw_set_auth();
			sys_log_set(WIFIDOG_SERVER_LOG_FILE, "Set on-line by ping_response.");
            set_is_online(ON_LINE);
            log_info(__LOG_FAILED__, "the device is on line.");
        }
		fw_set_auth();
		
        //2. 处理配置文件更新
        filter_string_tool(response, CONF_VER, '\0', ping_response_conf_ver);
        //3. 处理命令更新
        filter_string_tool(response, CMD_VER, '\0', ping_response_cmd_ver);
        /*add@2014.10.11 处理云平台的强制用户下线版本号 和 云平台在线用户总数*/
        filter_string_tool(response, USER_VER, '\0', ping_response_user_ver);
        filter_string_tool(response, TOTAL_USER_NUM, '\0', ping_response_total_user_num);
        
        //4. 处理平台强制下线终端
        g_forced_offline_num = 0;
        filter_string_tool(response, CLIENT_NUM, '\0', ping_response_forced_offline_num);
        if(0 != g_forced_offline_num)
            filter_string_tool(response, CLIENT_LIST, ';', ping_response_forced_offline_del);
        //5、下载资源文件portal 下沉
        filter_string_tool(response, TPL_VER, '\0', ping_response_src_tpl);
        filter_string_tool(response, PROD_VER, '\0', ping_response_src_product);
        filter_string_tool(response, AD_VER, '\0', ping_response_src_adver);
        download_src();
    }
	/*platform crash报文处理*/
    else if(NULL != (index = strstr(response, "User_access is crashed")))
    {
        log_info(__LOG_WARNING__, "Auth server says User_access is crashed!");
        //是否需要设置下线标记
        if(ping_timeouts >= config_get_config()->pingtimeouts
            && ON_LINE == is_online())
        {
            //* 认为设备离线, 配置为免认证
            sys_log_set(WIFIDOG_SERVER_LOG_FILE, "Set NF free-auth by ping_response User_access crashed.");
			fw_set_free_auth();
            set_is_online(OFF_LINE);
            log_info(__LOG_FAILED__, "the device is off line.");
        }
		log_info(__LOG_ERROR__, "Auth server said 'User_access is crashed' and device is online, then set freeauth on");
		if (ON_LINE == is_online())
		{
			fw_set_free_auth();
		}
    }
	/*not Pong & not platform crash*/
    else 
    {
		log_info(__LOG_DEBUG__, "Auth server says no Pong and no platform crash!");
		//是否需要设置下线标记
        if(ping_timeouts >= config_get_config()->pingtimeouts
            && ON_LINE == is_online())
        {
            //* 认为设备离线, 配置为免认证
			fw_set_free_auth();
            set_is_online(OFF_LINE);
            log_info(__LOG_FAILED__, "the device is off line.");
        }
    }
    return;
}

/*begin: modified@2014.10.10 for delete user info in client_list_deny_response*/
//void client_list_deny_response(void)
void client_list_deny_response(char *response)
{    
	t_client *p1, *p2;
    int loop = 0;
    s_config *config = config_get_config(); 
    
    log_info(__LOG_DEBUG__, "HTTP Response[client_list_deny_response] form server: [%s]", response);
	sys_log_set(WIFIDOG_SERVER_LOG_FILE, "\n[client_list_deny_response] form server: [%s]", response);
    
    for(loop = 0; loop < CLIENT_HASH_SIZE; loop++)
    {
        LOCK_CLIENT_LIST();
        p1 = g_client_head[loop].next;
        while(p1)
        {
			p2 = p1->next;
            if (p1->denied_client_flag)
            {
	            nf_fw_access(FW_ACCESS_DENY, p1->ip, p1->mac);
	            if(config->qos_enable)
	                fw_qos_del(p1->mac);
	            client_list_delete(p1);
            }
            p1 = p2;
        }
        UNLOCK_CLIENT_LIST();
    }

    return;
}
/*end: modified@2014.10.10 for delete user info in client_list_deny_response*/

/**@ 同步平台终端列表 */
/**@ BUGS: 同步后终端的流量信息将会清空 */
/*needed redefined*/
void client_list_request_response(char *response, int *begin, int *count, int *client_num)
{   
    char *start, *index, *end;
    char ip[16];
	char mac[18];
	char token[83];
	unsigned long long incoming;
	unsigned long long outgoing;
	long online_time;
    t_client *client;
	char tmpbuf[256];
	char logbuf[1024];

    int loop = 0;
	/*http response:
	  * client_num=\nip=|mac=|token=|incoming=|outgoing=|onlinetime=
	  */
    log_info(__LOG_ERROR__, "HTTP Response[client_list_request_response] buf [%s]", response);

    start = index = end = NULL;
	logbuf[0] = '\n';

    if(NULL != (start = strstr(response, CLIENT_COUNT)))
    {
        if(NULL != (end = strstr(start, "\n")))
        {
            *end = '\0';
            sscanf(start, CLIENT_COUNT"%d", count);
            log_info(__LOG_ERROR__, CLIENT_COUNT" %d", *count);
            *end = '\n';
        }
    }

    if(NULL != (start = strstr(response, CLIENT_BEGIN)))
    {
        if(NULL != (end = strstr(start, "\n")))
        {
            *end = '\0';
            sscanf(start, CLIENT_BEGIN"%d", begin);
            log_info(__LOG_ERROR__, CLIENT_BEGIN" %d", *begin);
            *end = '\n';
        }
    }
    
    if(NULL != (start = strstr(response, CLIENT_NUM)))
    {
        if(NULL != (end = strstr(start, "\n")))
        {
            *end = '\0';
            sscanf(start, CLIENT_NUM"%d", client_num);
            log_info(__LOG_ERROR__, CLIENT_NUM" %d", *client_num);
            *end = '\n';
        }
    }

    if(0 != *client_num)
    {
        if(NULL != (start = strstr(response, CLIENT_LIST)))
        {
            if(NULL != (end = strstr(start, "\n")))
            {
                *end = '\0';
                index = strstr(start, "=");
                start = ++index;
                while(*index != '\0')
                {
                    if(*index != ';')
                    {
                        index++;
                        continue;
                    }
                    else
                    {
                        *index = '\0';
                        sscanf(start, "%[^|]|%[^|]|%[^|]|%lld|%lld|%ld|", ip, mac, token, &incoming, &outgoing, &online_time);
                        log_info(__LOG_ERROR__, "loop[%d], client ip %s mac %s incoming %lld outgoing %lld online_time %ld token %s ", 
												loop++, ip, mac, incoming, outgoing, online_time, token);
                        if(NULL == client_list_find(ip, mac))
                        {
                            if  (NULL == client_list_append(ip, mac, token))
                            {
								log_info(__LOG_ERROR__, "client_list_append fail, ip: %s, mac: %s", ip, mac);
                                continue;
                            }
                            client = client_list_find(ip, mac);
							/*begin:@2014.8.4*/
		                    //client->idletimeout = 86400;
		                    //client->forcetimeout = 600;
		                    client->forcetimeout = config_get_config()->forcetimeout;
                    		client->idletimeout = config_get_config()->idletimeout;
							client->counters.incoming = incoming*1024;
							client->counters.outgoing = outgoing*1024;
							client->bw_up = config_get_config()->bw_up;
							client->bw_down = config_get_config()->bw_down;


							log_info(__LOG_ERROR__, "client_list_append  ip: %s, mac: %s client->bw_up %d client->bw_down:%d  config_get_config()->qos_enable %d",\
								ip, mac,client->bw_up,client->bw_down,config_get_config()->qos_enable);
							
							if(config_get_config()->qos_enable)
							{
								fw_qos_add(client);	
							}
                            client->fw_connection_state = FW_MARK_KNOWN;
    		                //client->login_time = get_uptime();
    		                client->online_time = online_time;
							client->auth_period = 0;
							/*end: 2014.8.4*/
							
							sprintf(tmpbuf, "nf-ALLOW client-list-sync %s %s\n", ip, mac);
							if(strlen(logbuf) + strlen(tmpbuf) >= 1023){
								sys_log_set(WIFIDOG_USR_LOG_FILE, logbuf);
								logbuf[0] = '\0';
							}
							strcat(logbuf, tmpbuf);

							client_list_auth_update(client);
                            nf_fw_access(FW_ACCESS_ALLOW, ip, mac);
                        }
                        *index = ';';
                        start = ++index;
                    }
                }
                *index = '\0';
                sscanf(start, "%[^|]|%[^|]|%[^|]|%lld|%lld|%ld", ip, mac, token, &incoming, &outgoing,&online_time);
                log_info(__LOG_ERROR__, "loop[%d], client ip %s mac %s incoming %lld outgoing %lld, online_time %ld token %s", 
										loop, ip, mac, incoming, outgoing, online_time, token);
                if(NULL == client_list_find(ip, mac))
                {
                    if  (NULL == client_list_append(ip, mac, token))
                    {
						log_info(__LOG_ERROR__, "client_list_append fail, ip: %s, mac: %s", ip, mac);
                        return;
                    }
					sprintf(tmpbuf, "nf-ALLOW client-list-sync %s %s\n", ip, mac);
					if(strlen(logbuf) + strlen(tmpbuf) >= 1023){
						sys_log_set(WIFIDOG_USR_LOG_FILE, logbuf);
						sys_log_set(WIFIDOG_USR_LOG_FILE, tmpbuf);
					}
					else{
						strcat(logbuf, tmpbuf);
						sys_log_set(WIFIDOG_USR_LOG_FILE, logbuf);
					}

                    nf_fw_access(FW_ACCESS_ALLOW, ip, mac);
                    client = client_list_find(ip, mac);
					client_list_auth_update(client);
					/*begin: 2014.8.4*/
                    //client->idletimeout = 86400;
                    //client->forcetimeout = 600;
                    client->forcetimeout = config_get_config()->forcetimeout;
                    client->idletimeout = config_get_config()->idletimeout;
					client->counters.incoming = incoming*1024;
					client->counters.outgoing = outgoing*1024;
					client->bw_up = config_get_config()->bw_up;
					client->bw_down = config_get_config()->bw_down;
					if(config_get_config()->qos_enable)
					{
						fw_qos_add(client);	
					}
                    client->fw_connection_state = FW_MARK_KNOWN;
	                //client->login_time = get_uptime();
	                client->online_time = online_time;
					client->auth_period = 0;
					/*end: 2014.8.4*/
                }
                *index = ';';
                start = ++index;
                *end='\n';
            }
        }
    }
    
    return;
}

void client_list_response(void)
{
    /** @暂时不需要什么操作,后续完善流程 */

    return;
}

/*add @2014.8.1 for setting global config to online clients, include config type: 
  *EN_QOSUP, EN_QOSDOWN, EN_FORCETIMEOUT, EN_IDLETIMEOUT
  */
static int global_config_to_clients()
{
    int loop = 0;
    t_client *p1 = NULL;
	
	log_info(__LOG_DEBUG__, "global_config_to_clients enter");

	if(config_get_config()->config_change_flag[CONFIG_CHANGE_BW_UP] || 
	   config_get_config()->config_change_flag[CONFIG_CHANGE_BW_DOWN] || 
	   config_get_config()->config_change_flag[CONFIG_CHANGE_FORCETIMEOUT] || 
	   config_get_config()->config_change_flag[CONFIG_CHANGE_IDLETIMEOUT] )
	{ 
		log_info(__LOG_DEBUG__, "global_config_to_clients config changed");
		for(loop = 0; loop < CLIENT_HASH_SIZE; loop++)
		{		
			LOCK_CLIENT_LIST();
			p1 = g_client_head[loop].next;
			while(p1)
			{
				if (config_get_config()->config_change_flag[CONFIG_CHANGE_BW_UP])
					p1->bw_up = config_get_config()->bw_up;
				if (config_get_config()->config_change_flag[CONFIG_CHANGE_BW_DOWN])
					p1->bw_down = config_get_config()->bw_down;
				if (config_get_config()->config_change_flag[CONFIG_CHANGE_FORCETIMEOUT])
					p1->forcetimeout = config_get_config()->forcetimeout;
				if (config_get_config()->config_change_flag[CONFIG_CHANGE_IDLETIMEOUT])
					p1->idletimeout = config_get_config()->idletimeout;
				if(config_get_config()->qos_enable && 
					(config_get_config()->config_change_flag[CONFIG_CHANGE_BW_UP] || 
					 config_get_config()->config_change_flag[CONFIG_CHANGE_BW_DOWN]))
				{
					log_info(__LOG_DEBUG__, "global config: bw_up, bw_down changed, then config to online clients");
					fw_qos_add(p1);	
				}
				
				p1 = p1->next;
			}
			UNLOCK_CLIENT_LIST();
		}
	}
	config_get_config()->config_change_flag[CONFIG_CHANGE_BW_UP] = 0;
	config_get_config()->config_change_flag[CONFIG_CHANGE_BW_DOWN] = 0;
	config_get_config()->config_change_flag[CONFIG_CHANGE_FORCETIMEOUT] = 0;
	config_get_config()->config_change_flag[CONFIG_CHANGE_IDLETIMEOUT] = 0; 
	return 0;
}
void conf_response(char *response)
{	
	log_info(__LOG_DEBUG__, "Http Response[conf_response] [%s]", response);
	sys_log_set(WIFIDOG_SERVER_LOG_FILE, "\n[Conf_response] form server: [%s]", response);
	
	//syslog(LOG_INFO, "Http Response[conf_response] [%s]", response);
	/*modified @2014.8.13 for conf_ver add in conf_response */
    //config_get_config()->conf_ver = conf_ver;
    config_get_config()->conf_ver = config_get_config()->config_ver_tmp;
	
    event_map_cannel(EVENT_REQUEST_CONFIG);

    /* 清空配置 */
    portal_flush_mac_black_list();
    portal_flush_mac_white_list();
    portal_flush_mac_free_auth();
    portal_flush_ip_free_auth();
    portal_flush_dest_ip_black_list();
    portal_flush_dest_ip_white_list();
    portal_flush_url_black_list();
    portal_flush_url_white_list();
    portal_flush_url_free_auth_list();
    /*add@2014.11.03 清空强制重定向url名单*/
    portal_flush_url_must_redirect_list();
    portal_flush_dest_ip_must_redirect_list();

    /* 重新配置*/  
	/*add@2015.01.28 for wechat connect wifi*/
	filter_string_tool(response, WXLSERVERIP, ',', conf_handle);
	
    filter_string_tool(response, QOSENABLE, ',', conf_handle);
	/*begin: add @2014.8.1 for adding configuration*/
	filter_string_tool(response, QOSUP, ',', conf_handle);
	filter_string_tool(response, QOSDOWN, ',', conf_handle);
	/*end: add @2014.8.1 for adding configuration*/
    filter_string_tool(response, TRUSTEDMACLIST, ',', conf_handle);
    filter_string_tool(response, TRUSTEDIPLIST, ',', conf_handle);
    filter_string_tool(response, TRUSTEDWANHOSTLIST, ',', conf_handle);
    filter_string_tool(response, BLACKIPLIST, ',', conf_handle);
    filter_string_tool(response, BLACKWANHOSTLIST, ',', conf_handle);
    filter_string_tool(response, BLACKMACLIST, ',', conf_handle);
    filter_string_tool(response, FREEAUTHMACLIST, ',', conf_handle);
    filter_string_tool(response, FREEAUTHIPLIST, ',', conf_handle);
    filter_string_tool(response, MAXSTATIONNUM, ',', conf_handle);
    filter_string_tool(response, POSTTRUSTEDWANHOSTLIST, ',', conf_handle);
	/*begin: add @2014.8.1 for adding configuration*/
	filter_string_tool(response, FORCETIMEOUT, ',', conf_handle);
	filter_string_tool(response, IDLETIMEOUT, ',', conf_handle);
	filter_string_tool(response, WANFLOWSTATSWITCH, ',', conf_handle);
	/*end: add @2014.8.1 for adding configuration*/
	
    /* BEGIN: Added by WeiHonggang, 2014/2/18   PN:TASK#0平台支持认证模式同步到网关，并根据认证模式进行设置 */
    filter_string_tool(response, AUTHMODE, ',', conf_handle);
    conf_set_by_auth_mode();
    /* END:   Added by WeiHonggang, 2014/2/18   PN:TASK#0平台支持认证模式同步到网关，并根据认证模式进行设置 */

    /*add@2014.11.03 */
    filter_string_tool(response, MUSTREDIRECTURLLIST, ',', conf_handle);
    
	conf_set_default_whitelist();

    conf_set_other_url();
	filter_string_tool(response, STADCTENABLE, '\0', conf_handle);
	filter_string_tool(response, STADCTRSSI, '\0', conf_handle);
	filter_string_tool(response, STADCTRPTTIME, '\0', conf_handle);

	/*add: @2014.8.1 some conf must configed to clients*/
	global_config_to_clients();
	
	return;
}

int event_map_register(int event_code, 
                              int time_interval, 
                              int timeout_count, 
                              void (*request_func)(), 
                              void (*response_func)())
{
    event_map *cur = NULL;
    event_map *prev = NULL;

    prev = eventHead;
    while(NULL != prev->next)
    {
        prev = prev->next;
    }
    cur = safe_malloc(sizeof(event_map));
    cur->event_code = event_code;
    cur->time_interval = time_interval;
    cur->timeout_count = timeout_count;
    cur->request_func = request_func;
    cur->response_func = response_func;
    cur->last_time = get_uptime();
    cur->count = 0;
    cur->next = NULL;

    prev->next = cur;
    return 0;
}

static int event_map_init(void)
{
    /**@ 初始化时间列表 */
    if(NULL == eventHead)
    {
        eventHead = safe_malloc(sizeof(event_map));
    }
    event_map_register(EVENT_PING, config_get_config()->checkinterval, 0, ping_request, ping_response);
    /*modified@2014.10.10 for delete user info in client_list_deny_response*/
    //event_map_register(EVENT_DENY_CLIENT, (config_get_config()->checkinterval)*10, 0, client_list_deny_report, NULL);
    event_map_register(EVENT_DENY_CLIENT, (config_get_config()->checkinterval)*10, 0, client_list_deny_report, client_list_deny_response);

    event_map_register(EVENT_REPORT_CLIENT, (config_get_config()->checkinterval)*20, 0, client_list_report, NULL);
	event_map_register(EVENT_REPORT_AP, (config_get_config()->checkinterval)*5, 0, ap_list_report, NULL);
	event_map_register(EVENT_REPORT_STADCT, 
		((config_get_config()->checkinterval)*g_stadct_rpt_time/60), 
		0, stadct_list_report, NULL);
	
    if(0 != config_get_config()->conf_ver)
    {
        event_map_register(EVENT_REQUEST_CONFIG, 3, 1, conf_request, conf_response);
    }
	/*add@ 2014.8.25 for task#86*/
	if (0 != config_get_config()->wan_flow_report_interval)
	event_map_register(EVENT_REPORT_WAN_FLOWING, 
						(config_get_config()->checkinterval)*(config_get_config()->wan_flow_report_interval), 
						0, wan_flowing_report_request, NULL);

    event_map_register(EVENT_SYNC_AUTHSTA_LIST, (config_get_config()->checkinterval)*3, 0, 
		authsta_sync_report, NULL);
    
	event_map_register(EVENT_SYNC_STA_KEY, 
						(config_get_config()->iStaKeyIntval), 
						0, sta_get_keyinfo, NULL);
    return 0;
}

event_map *event_map_find(en_event_code code)
{
    event_map *cur = NULL;

	if(NULL == eventHead)
		return NULL;

    cur = eventHead->next;
    while(NULL != cur)
    {
        if(cur->event_code == code)
        {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

void event_map_cannel(en_event_code code)
{
    event_map *cur = NULL;
    event_map *pre = NULL;

	if(NULL == eventHead)
		return;

    pre = eventHead;
    cur = eventHead->next;
    while(NULL != cur)
    {
        if(cur->event_code == code)
        {
            pre->next = cur->next;
            free(cur);
            return;
        }
        pre = cur;
        cur = cur->next;
    }
    return ;
}

void event_map_update(int event_code, 
                             int time_interval, 
                             int timeout_count, 
                             void (*request_func)(), 
                             void (*response_func)())
{
    event_map *p;
    p = event_map_find(event_code);
    if(!p)
        return;

    p->time_interval = time_interval;
    p->timeout_count = timeout_count;
    p->request_func = request_func;
    p->response_func = response_func;
    p->last_time = get_uptime();
    p->count = 0;
    return;
}

/**@ 与平台交互线程 */
extern int wifidog_set_thread_name(EN_WIFIDOG_THREAD_INDEX enThreadIndex);
void thread_platform(void)
{
    int sockfd[EVENT_MAX] = {0};
    int bytes[EVENT_MAX] = {0};
    int totals[EVENT_MAX] = {0};
    int max_sockfd= SOCKET_INVALID;
    int loop;
	s_config *config = config_get_config();
	int tmpFd = SOCKET_INVALID;

    char *buf[EVENT_MAX];
    int result;
	int buf_size;
    fd_set fds;
    struct timeval timeout;

	sys_log_set(WIFIDOG_PLATFORM_FILE, "Thread thread_platform() Start..");

    event_map *index = NULL;

	/*add@2015.01.28 SET THREAD NAME*/
	wifidog_set_thread_name(WIFIDOG_THREAD_PLATFORM);

	for(loop = 0; loop < EVENT_MAX; ++loop){
		if(EVENT_REQUEST_CONFIG == loop)
			buf_size = MAX_BUF*32;
		else
			buf_size = MAX_BUF;

		buf[loop] = safe_malloc(buf_size);
	}

    event_map_init();

	sys_log_set(WIFIDOG_PLATFORM_FILE, "Event map init ok.");
    
	/*BEGIN: add @2014.8.26*/
	char cBuff1[64] = {0};
	char cBuff2[64] = {0};
	FILE *fp1 = fopen("/var/wan.up", "r");
	FILE *fp2 = fopen("/var/wan.down", "r");
    if (NULL == fp1 || NULL == fp2){
		log_info(__LOG_ERROR__, "fopen /var/wan.up or /var/wan.down fail");
		system("touch /var/wan.up");
		system("touch /var/wan.down");
		g_stInterfaceFlowStats.wan_total_incoming = 0;
		g_stInterfaceFlowStats.wan_total_outgoing = 0;
	}
	else{		
		if (NULL != fgets(cBuff1, 63, fp1))
		{
			sscanf(cBuff1, "%llu", &g_stInterfaceFlowStats.wan_total_incoming);
		}
		if (NULL != fgets(cBuff2, 63, fp2))
		{
			sscanf(cBuff2, "%llu", &g_stInterfaceFlowStats.wan_total_outgoing);
		}
		fclose(fp1);
		fclose(fp2);
	}
	sys_log_set(WIFIDOG_PLATFORM_FILE, "Update wan flow file.");
	log_info(__LOG_ERROR__, "g_stInterfaceFlowStats.wan_total_incoming: %llu, g_stInterfaceFlowStats.wan_total_outgoing: %llu",
						g_stInterfaceFlowStats.wan_total_incoming, g_stInterfaceFlowStats.wan_total_outgoing);
	/*END: add @2014.8.26*/
	
    /**@ 初始化socket */
    for(loop = 0; loop < EVENT_MAX; loop++)
    {
        sockfd[loop] = SOCKET_INVALID;
    }

	/*send ap list*/	
	sys_log_set(WIFIDOG_PLATFORM_FILE, "First time send ap list.");
	ap_send_list_report(send_ap_list, (void *)config, &tmpFd);
	
	sys_log_set(WIFIDOG_PLATFORM_FILE, "Start to check event.");
    while(1)
    {
        /**@ 检测事件是否触发 */
        for(loop = 0; loop < EVENT_MAX; loop++)
        {
            index = event_map_find(loop);
            if(index && (get_uptime() - index->last_time >= index->time_interval))
            {
                if(SOCKET_INVALID != sockfd[loop])
                {
                    close(sockfd[loop]);
                    sockfd[loop] = SOCKET_INVALID;
					sys_log_set(WIFIDOG_SERVER_LOG_FILE, 
						"\n[Event %d] socket valid, maybe no resp received last time.", loop);
                }
                if(NULL != index->request_func)
                    (index->request_func)(&sockfd[loop]);
                index->last_time = get_uptime();
                /**@ 清空接收buf */
				if(EVENT_REQUEST_CONFIG == loop)
					buf_size = MAX_BUF*32;
				else
					buf_size = MAX_BUF;

				memset(buf[loop], 0, buf_size);
            }
        }

        /**@ 设置fd */
        FD_ZERO(&fds);
        for(loop = 0; loop < EVENT_MAX; loop++)
        {
            if(SOCKET_INVALID == sockfd[loop])
            {
                continue;
            }
            FD_SET(sockfd[loop], &fds);
            max_sockfd = update_max_fd(max_sockfd, sockfd[loop]);
        }
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        result = select(max_sockfd+1, &fds, NULL, NULL, &timeout);
        if(result > 0)
        {
            /**@ 接收事件回复 */
            for(loop = 0; loop < EVENT_MAX; loop++)
            {
               if(SOCKET_INVALID == sockfd[loop])
                {
                    continue;
                }
                if(FD_ISSET(sockfd[loop], &fds))
                {
					if(EVENT_REQUEST_CONFIG == loop)
						buf_size = MAX_BUF*32;
					else
						buf_size = MAX_BUF;

					bytes[loop] = read(sockfd[loop], buf[loop] + totals[loop], buf_size - totals[loop]);
                    if(0 == bytes[loop])
                    {
                        buf[loop][totals[loop]] = '\0';
                        log_info(__LOG_DEBUG__, "sockfd %d", sockfd[loop]);

						if((totals[loop] > 0) && (strlen(buf[loop]) > 0)){
	                        index = event_map_find(loop);
	                        if(index)
	                        {
	                            if(NULL != index->response_func){
	                                (index->response_func)(buf[loop]);
									sys_log_set(WIFIDOG_SERVER_LOG_FILE, 
										"\n[Event %d] msg %d bytes", loop, totals[loop]);
	                            }
	                        }
						}
						else{
							sys_log_set(WIFIDOG_SERVER_LOG_FILE, 
								"\n[Event %d] receive empty(%d) msg", loop, totals[loop]);
						}
						
                        close(sockfd[loop]);
                        sockfd[loop] = SOCKET_INVALID;
                        bytes[loop] = totals[loop] = 0; 
                    }
                    else if(bytes[loop] > 0)
                    {
                        totals[loop] += bytes[loop];
                    }
                    else
                    {
                    	sys_log_set(WIFIDOG_SERVER_LOG_FILE, 
							"\n[Event %d] recv msg error %d", loop, bytes[loop]);
                        close(sockfd[loop]);
                        sockfd[loop] = SOCKET_INVALID;
                    }
                }
            }
        }
        else if(result < 0)
        {
            //@ 如何处理数据
            log_info(__LOG_ERROR__, "platform select error.");
        }
    }

	sys_log_set(WIFIDOG_PLATFORM_FILE, "Quit check event.");

	for(loop=0; loop<EVENT_MAX; ++loop){
		if(buf[loop] != NULL)
			free(buf[loop]);
	}
    
    return;
}

/********************************@ 网关初始化需要调用的函数 *************************************/

static void conf_init(char *mark, char *res)
{
    long long conf_ver;
    
    sscanf(res, CONF_VER"%lld", &conf_ver);
    log_info(__LOG_DEBUG__, "conf_ver = %lld", conf_ver);
    if(config_get_config()->conf_ver != conf_ver)
    {
        config_get_config()->conf_ver = conf_ver;
    }
    return;
}

/** 
 * @检测设备是否能在平台上线,
 * 上线后启动业务流程配置
 */
int first_ping(void)
{
    int sockfd = SOCKET_INVALID;
    int result;
    fd_set fds;
    int bytes, totalbytes;

    char buf[MAX_BUF];

    memset(buf, 0, sizeof(buf));

    //* 设备设备为离线状态
    sys_log_set(WIFIDOG_SERVER_LOG_FILE, "Set off-line by first_ping.");
    set_is_online(OFF_LINE);
    
    ping_request(&sockfd);
    if(SOCKET_INVALID == sockfd)
        return 1;
    bytes = totalbytes = 0;
    while(1)
    {
        FD_ZERO(&fds);
        FD_SET(sockfd, &fds);

        result = select(sockfd + 1, &fds, NULL, NULL, NULL);
        if(result > 0)
        {
            bytes = read(sockfd, buf + totalbytes, MAX_BUF - (totalbytes + 1));
            if(bytes < 0 )
            {
                close(sockfd);
                return 1;
            }
            else if(bytes == 0)
            {
                buf[totalbytes] = '\0';
                log_info(__LOG_DEBUG__, "HTTP Ping Response [%s]", buf);
                if(strstr(buf, "Pong"))
                {
                    set_is_online(ON_LINE);
                    log_info(__LOG_DEBUG__, "Auth Server Says: Pong.");
                    filter_string_tool(buf, CONF_VER, '\0', conf_init);
                    /* 获取portal 下沉资源版本号 */
                    filter_string_tool(buf, TPL_VER, '\0', ping_response_src_tpl);
                    filter_string_tool(buf, PROD_VER, '\0', ping_response_src_product);
                    filter_string_tool(buf, AD_VER, '\0', ping_response_src_adver);
                    close(sockfd);
                    return 0;
                }
                else
                {
                    log_info(__LOG_ERROR__, "Auth Server not to says Pong.");
                    close(sockfd);
                    return 1;
                }
            }
            else
            {
                totalbytes += bytes;
            }
        }
        else
        {
            log_info(__LOG_ERROR__, "Error reading data via select() from auth server: %s", strerror(errno));
            close(sockfd);
            return 1;
        }
    }

    return 0;
}

/**@ 设备启动第一次上线时，获取平台的在线列表 */
void first_client_request(void)
{
    int sockfd = SOCKET_INVALID;
    char buf[MAX_BUF*2];

    int result;
    fd_set fds;
    struct timeval timeout;

    int begin = 0;
    int count = 0;
    int client_num = 0;
    int total, bytes;
    //add@2015.4.03 防止云平台没有响应wifidog 一直阻塞
    int wait_times = 0;/*select 超过次数，达到3次重新发送请求*/
    int retry_send_request_times = 0; /*发送request次数，达到20次(5分钟)则放弃获取*/

    while(1)
    {
        retry_send_request_times ++;
        if(retry_send_request_times > 20)
        {
            log_info(__LOG_ERROR__, "can't recv client_request response. then give up!!!");
            return; /*放弃获取，避免平台获取列表不正常导致wifidog频繁重启*/
        }
    
        client_list_request_report(&sockfd, begin);
        if(SOCKET_INVALID == sockfd)
        {
            log_info(__LOG_ERROR__, "Could not connect platform.");
            return;
        }

        total = bytes = 0;
        while(1)
        {
            wait_times++;
            if( wait_times > 3 ){
                   log_info(__LOG_ERROR__, "can't recv client_request response. then resend request");
                   break;
            }
        
            FD_ZERO(&fds);
            FD_SET(sockfd, &fds);
            timeout.tv_sec = 5;
            timeout.tv_usec = 0;
            result = select(sockfd+1, &fds, NULL, NULL, &timeout);
            if(result > 0)
            {
                bytes = read(sockfd, buf + total, sizeof(buf) - (total + 1));
                if(0 == bytes)
                {
                    buf[total] = '\0';
                    client_list_request_response(buf, &begin, &count, &client_num);
                    if(count <= begin+client_num)
                    {
                        close(sockfd);
                        return;
                    }
                    begin += client_num;
                    retry_send_request_times--;
                    break;
                }
                else if(bytes > 0)
                {
                    total += bytes;
                }
                else
                {
                    break;
                }
            }
        }
        close(sockfd);
    }
    return;
}

int passive_user_logout(t_client *client)
{
    int sockfd = SOCKET_INVALID;
	char client_list[256]={0};
    s_config *config = config_get_config();

    sockfd = connect_auth_server();
    if(SOCKET_INVALID == sockfd)
    {
    	log_info(__LOG_WARNING__, "connect_auth_server fail");
        return 1;
	}
    log_info(__LOG_MSG__, "connect_auth_server succeed");
		
	/*  */
	sprintf(client_list, "%s|%s|%s|%lld|%lld;", 
                    client->ip, client->mac,
                    client->token, client->counters.incoming/1024, client->counters.outgoing/1024);
	send_client_list(1, client_list, &sockfd, config, "logout");

#if 0
    int result;
    fd_set fds;
    int bytes, totalbytes;
    struct timeval timeout;
	char buf[MAX_BUF];

	memset(buf, 0, sizeof(buf));

	bytes = totalbytes = 0;
    while(1)
    {
        FD_ZERO(&fds);
        FD_SET(sockfd, &fds);

        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        result = select(sockfd + 1, &fds, NULL, NULL, &timeout);
        if(result > 0)
        {
            bytes = read(sockfd, buf + totalbytes, MAX_BUF - (totalbytes + 1));
            if(bytes < 0 )
            {
                close(sockfd);
                return 1;
            }
            else if(bytes == 0)
            {
                buf[totalbytes] = '\0';
				//buf
                log_info(__LOG_DEBUG__, "HTTP Response [%s]", buf);
				//暂不处理应答
				return 0;
            }
            else
            {
                totalbytes += bytes;
            }
        }
        else
        {
            log_info(__LOG_ERROR__, "Error reading data via select() from auth server: %s", strerror(errno));
            close(sockfd);
            return 1;
        }
    }
#endif
    return 0;
}

int set_sta_auth_status(char *ip, char *mac, int auth)
{
	t_client *client;
	
	LOCK_CLIENT_LIST();
    client = client_list_find(ip, mac);
	if(client != NULL){
		client->fw_connection_state = ((auth == 2) ? FW_MARK_KNOWN : FW_MARK_PROBATION);
		client_list_auth_update(client);
		nf_fw_access(((auth == 2) ? FW_ACCESS_ALLOW : FW_ACCESS_DENY), 
			client->ip, client->mac);
	}
	else{ /*调试命令，终端不存在则添加*/
		client = client_list_append(ip, mac, "user-test-token");
		if(client != NULL){
			client->fw_connection_state = ((auth == 2) ? FW_MARK_KNOWN : FW_MARK_PROBATION);
			nf_fw_access(((auth == 2) ? FW_ACCESS_ALLOW : FW_ACCESS_DENY), client->ip, client->mac);
		}
	}

	UNLOCK_CLIENT_LIST();

	return 0;
}


int set_stat_start(char *interval, char *duration)
{
	char cmd[128];
	log_info(__LOG_MSG__, "Stat start. interval %s duration %s", interval, duration);

	sprintf(cmd, "sysdebug get-stat start %s %s", interval, duration);

	system(cmd);
	return 0;
}

int set_stat_stop()
{
	char cmd[128];
	log_info(__LOG_MSG__, "Stat stop.");

	sprintf(cmd, "sysdebug get-stat stop");

	system(cmd);
	return 0;
}


int bblink_mac_baw_init(void)
{
	char acListValue[LIST_NUM_MAX][NVRAM_VALUE_MAX_LEN];
    unsigned int ulCount=0;
    unsigned int ulLoop=0;
	unsigned char aucmac[6];
    memset(acListValue,0,sizeof(acListValue));
    tools_nvram_list_get(NVRAM_WD_WHITE_MAC_LIST,NVRAM_WD_WHITE_MAC_NUM,NVRAM_WD_WHITE_MAC_HEAD,&ulCount,acListValue);
    for(ulLoop = 0; ulLoop < ulCount; ulLoop++)
    {
        get_mac(acListValue[ulLoop],aucmac);
		portal_add_mac_white_list(aucmac);
    }
    tools_nvram_list_get(NVRAM_WD_BLACK_MAC_LIST,NVRAM_WD_BLACK_MAC_NUM,NVRAM_WD_BLACK_MAC_HEAD,&ulCount,acListValue);
    for(ulLoop = 0; ulLoop < ulCount; ulLoop++)
    {
        get_mac(acListValue[ulLoop],aucmac);
		portal_add_mac_black_list(aucmac);
    }
	
	return 0;
}
int bblink_url_baw_init(void)
{
	char acListValue[LIST_NUM_MAX][NVRAM_VALUE_MAX_LEN];
    unsigned int ulCount=0;
    unsigned int ulLoop=0;
    memset(acListValue,0,sizeof(acListValue));
    tools_nvram_list_get(NVRAM_WD_WHITE_URL_LIST,NVRAM_WD_WHITE_URL_NUM,NVRAM_WD_WHITE_URL_HEAD,&ulCount,acListValue);
    for(ulLoop = 0; ulLoop < ulCount; ulLoop++)
    {
		portal_add_url_white_list((unsigned char *)acListValue[ulLoop]);
    }
    tools_nvram_list_get(NVRAM_WD_BLACK_URL_LIST,NVRAM_WD_BLACK_URL_NUM,NVRAM_WD_BLACK_URL_HEAD,&ulCount,acListValue);
    for(ulLoop = 0; ulLoop < ulCount; ulLoop++)
    {
		portal_add_url_black_list((unsigned char *)acListValue[ulLoop]);
    }
	return 0;
}
void get_local_config()
{
	s_config	*config = NULL;
	/* 是否首次 */
	static int flag = 0;

	char value[128];

	FILE *pf;
    char buff[128];

	int qos_enable = 0;
	int bw_up = 0;
	int bw_down = 0;

	int idletimeout = 0;
	int forcetimeout = 0;

	config = config_get_config();


	#ifdef OPENWRT
	if(-1 == sys_config_get(value,QOS_ENABLE))
   	{
    		qos_enable = 0;
   	}
	else
		sscanf(value,"%d",&qos_enable);

	if(-1 == sys_config_get(value,FLOW_CTL_UP))
	{
		bw_up = 0;
	}
	else
	{
	    sscanf(value,"%d",&bw_up);
	}
	if(-1 == sys_config_get(value,FLOW_CTL_DOWN))
	{
		bw_down = 0;
	}
	else
	{
	    sscanf(value,"%d",&bw_down);
	}
	
	#else
	
	/* 流控配置 */
	pf = fopen("/tmp/.flow_ap_sta", "rb");
	if(NULL == pf)
	{
		qos_enable = 0;
	}
	else
	{
		memset(buff, 0, sizeof(buff));
		fread(buff, sizeof(buff), 1, pf);
		fclose(pf);
		sscanf(buff, "%d,%d,%d", &(qos_enable), 
								 &(bw_up),
								 &(bw_down));
	}
	#endif
	log_info(__LOG_DEBUG__, "qos_enable: %d, upload: %d, download: %d\n",
        					qos_enable, 
        					bw_up, 
        					bw_down);
	if (config->qos_enable != qos_enable)
	{
		config->qos_enable = qos_enable;
		//config->config_change_flag[]=1;
	}
	if (config->bw_up != bw_up)
	{
		config->bw_up = bw_up;
		config->config_change_flag[CONFIG_CHANGE_BW_UP]=1;
	}
	if (config->bw_down != bw_down)
	{
		config->bw_down = bw_down;
		config->config_change_flag[CONFIG_CHANGE_BW_DOWN]=1;
	}

	/* 认证配置 */
	pf = fopen("/tmp/.auth_ap_sta", "rb");
    if(NULL == pf)
    {
    	forcetimeout = 86400;
    	idletimeout = 900;
    }
    else
    {
        memset(buff, 0, sizeof(buff));
        fread(buff, sizeof(buff), 1, pf);
        fclose(pf);
        sscanf(buff, "%d,%d", &(idletimeout), 
								 &(forcetimeout));
    }
    log_info(__LOG_DEBUG__, "idletimeout: %d, forcetimeout: %d\n",
        					idletimeout, 
        					forcetimeout);

	if (config->idletimeout != idletimeout)
	{
		config->idletimeout = idletimeout;
		config->config_change_flag[CONFIG_CHANGE_IDLETIMEOUT]=1;
	}
	if (config->forcetimeout != forcetimeout)
	{
		config->forcetimeout = forcetimeout;
		config->config_change_flag[CONFIG_CHANGE_FORCETIMEOUT]=1;
	}

	if (flag)
	{
		/* 对当前配置生效到已认证用户 */
		global_config_to_clients();
	}
	else
	{		
		flag = 1;
	}
	
	return;
}
