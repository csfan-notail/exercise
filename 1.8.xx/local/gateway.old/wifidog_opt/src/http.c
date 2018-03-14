/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : http.c
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/11/27
  Last Modified :
  Description   : 
  Function List :
  History       :
  1.Date        : 2013/11/27
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <ctype.h>

#include <pthread.h>

#include "httpd.h"

#include "conf.h"
#include "cJSON.h"
#include "util.h"
#include "client_list.h"
#include "firewall.h"
#include "config.h"
#include "auth_server.h"
#include "common.h"
#include "log.h"
#include "http.h"
#include "cloud_platform.h"
/*begin: add@2015.01.27 for wechat connect wifi*/
#include "redirect_portal.h"
#include "b64.h"
/*end: add@2015.01.27 for wechat connect wifi*/
#include "client_ap.h"
#include "client_aglink.h"
extern WD_AP_T * ap_find_by_mac(unsigned char *mac);

pthread_mutex_t first_page_mutex = PTHREAD_MUTEX_INITIALIZER;

/*add@2014.8.18*/
#ifdef GW_X86_DEL
extern int pthread_hash_by_ip(char *ip);
#else
#endif
/* BEGIN: Added by WeiHonggang, 2014/2/19   PN:TASK#0平台支持认证模式同步到网关，并根据认证模式进行设置  */
extern int auth_mode_support_weixin(void);
/* END:   Added by WeiHonggang, 2014/2/19   PN:TASK#0平台支持认证模式同步到网关，并根据认证模式进行设置  */
extern int auth_mode_support_alipay(void);
#ifdef HAVE_NETSNIFF

#define WIFIDOG_USER_LOGIN_MSG		1	
#define WIFIDOG_USER_LOGOUT_MSG		2

struct netsniff_msg_wifidog
{
	char msg_type;
	char user_mac[18]; 	/*format: XX:XX:XX:XX:XX:XX*/
	char user_ip[40]; 	/*format: ipv4: A.B.C.D ipv6: XXXX:XXXX:XXXX:XXXX:XXXX:XXXX:XXXX:XXXX*/
	char info[256];
};

extern int g_netsniff_fd;

extern int check_user_ios_access_exist(int loop, char *str_mac);
void http_alipay_redirect_callback(httpd *webserver,request *r);
void _alipay_auth_json_parse_add(cJSON *pstJsFromServer,cJSON *pstJsToClient,const char *str);
int aliauth_http_parse_json_data(request *r,char **jsdata);
void http_alipay_sendto_client(request *r,char *jsdata);


void send_client_login_msg_to_netsniff(char *ip, char *mac, char *info)
{
	struct netsniff_msg_wifidog msg;
	if(g_netsniff_fd <= 0)
		return;
	if((NULL == ip) || (NULL == mac))
		return;

	log_info(__LOG_DEBUG__, "Send client login msg to netsniff. client ip %s mac %s", ip, mac);
	memset(&msg, 0, sizeof(struct netsniff_msg_wifidog));
	msg.msg_type = WIFIDOG_USER_LOGIN_MSG;
	strncpy(msg.user_ip, ip, sizeof(msg.user_ip));
	strncpy(msg.user_mac, mac, sizeof(msg.user_mac));
	if(info != NULL)
		strncpy(msg.info, info, sizeof(msg.info));
	else
		msg.info[0] = '\0';
	
	if( send(g_netsniff_fd, &msg, sizeof(struct netsniff_msg_wifidog), 0) < 0){
		log_info(__LOG_ERROR__, "Send client login msg to netsniff error!");
	}
}

void send_client_logout_msg_to_netsniff(char *ip)
{	
	struct netsniff_msg_wifidog msg;
	if(g_netsniff_fd <= 0)
		return;
	if(NULL == ip)
		return;

	log_info(__LOG_DEBUG__, "Send client logout msg to netsniff. client ip %s", ip);
	memset(&msg, 0, sizeof(struct netsniff_msg_wifidog));
	msg.msg_type = WIFIDOG_USER_LOGOUT_MSG;
	strncpy(msg.user_ip, ip, sizeof(msg.user_ip));
	
	if( send(g_netsniff_fd, &msg, sizeof(struct netsniff_msg_wifidog), 0) < 0){
		log_info(__LOG_ERROR__, "Send client logout msg to netsniff error!");
	}
}

#endif

extern pthread_mutex_t client_list_mutex;
extern char *g_pstAppleSuccess;
extern char *g_pstDisIosPortalAuto;

extern int g_client_access_num;

/** Main request handling thread.
@param args Two item array of void-cast pointers to the httpd and request struct
*/
void process_http(httpd *webserver, request *r)
{
	if (httpdReadRequest(webserver, r) == 0) 
	{
		/*
		 * We read the request fine
		 */
		log_info(__LOG_DEBUG__, "Processing request from %s", r->clientAddr);
		log_info(__LOG_DEBUG__, "Calling httpdProcessRequest() for %s", r->clientAddr);
		httpdProcessRequest(webserver, r);
		log_info(__LOG_DEBUG__, "Returned from httpdProcessRequest() for %s", r->clientAddr);
	}
	else {
		log_info(__LOG_DEBUG__, "No valid request received from %s", r->clientAddr);
	}
	log_info(__LOG_DEBUG__, "Closing connection with %s", r->clientAddr);
	httpdEndRequest(r);
}

/* BEGIN: Added by cj, 2015/3/27   PN:终端认证状态更新 */
int http_auth_sta(char *mac,int iAuthType,char *pBblink_userid)
{
	t_client *client;
	char	*ip;	
	char *token_value = "123456789"; /* unused */
	s_config	*config = NULL;
	t_authresponse auth_response;	
	
	log_info(__LOG_MSG__, "http_auth_sta\n");	
	
	if (NULL == mac)
	{
		log_info(__LOG_ERROR__, "mac is NULL\n");
		return -1;
	}
	
	if (NULL == pBblink_userid)
	{
		log_info(__LOG_ERROR__, "Bblink_userid is NULL\n");
		return -1;
	}

    FILE *proc;
    char acIp[16];
    char acMac[18];
    char arpFlag[4];
    if (!(proc = fopen("/proc/net/arp", "r"))) 
    {
        log_info(__LOG_ERROR__, "/proc/net/arp: open failed");
        return -1;
    }

    /* Skip first line */
    while (!feof(proc) && fgetc(proc) != '\n');

    while (!feof(proc) && (fscanf(proc, " %15[0-9.] %*s %3[A-Za-z0-9] %17[A-Fa-f0-9:] %*s %*s", acIp,arpFlag ,acMac) == 3))
    {
        if (strcmp(acMac, mac) == 0) 
        {
         ip = acIp;
    	{	
    		/* Prepare some variables we'll need below */
    		config = config_get_config();
    		//auth_server = get_auth_server();
    		
    		LOCK_CLIENT_LIST();
    		if ((client = client_list_find(ip, mac)) == NULL) {
    		    /** 判断是否超过最大接入用户数 */
    	        if(g_client_access_num >= config->max_client)
    	        {
    	            UNLOCK_CLIENT_LIST();
    				log_info(__LOG_MSG__, "Redirect to max users %d", config->max_client);
    	            goto fail;
    	        }
    			log_info(__LOG_MSG__, "New client for %s", ip);
    			client_list_append3(ip, mac, iAuthType,pBblink_userid,token_value);
    		}  
    		else {
    			    /**@ 如果用户已经存在,则更新token */
    			if(NULL != client->token)
    			    	free(client->token);
    			client->token = safe_strdup(token_value);
    			log_info(__LOG_DEBUG__, "Client for %s is already in the client list", client->ip);
    		}

    		if ((client = client_list_find(ip, mac)) == NULL) {	
    			UNLOCK_CLIENT_LIST();						
    			log_info(__LOG_MSG__, "This is the client not found page.");
    			continue;
    		}

    		auth_response.authcode = AUTH_ALLOWED;
    		auth_response.ext = malloc(1024);
    		memset(auth_response.ext, 0, 1024);

    		sprintf(auth_response.ext, "%u %u %u %u %lu %lu",
    									config->bw_up,
    									config->bw_down,
    									0,
    									0,
    									config->idletimeout,
    									config->forcetimeout);
    		
    		switch(auth_response.authcode)
    		{
    	        case AUTH_ALLOWED:
    	    		/* Logged in successfully as a regular account */
    	    		log_info(__LOG_MSG__, "Got ALLOWED from central server authenticating token %s from %s at %s - "
    	    				"adding to firewall and redirecting them to portal.", client->token, client->ip, client->mac);
    	    		if(auth_response.ext){
    	    			log_info(__LOG_MSG__, "EXT:[%s]", auth_response.ext);
    	    			sscanf(auth_response.ext, "%u %u %llu %llu %lu %lu", 
    							&client->bw_up, 
    							&client->bw_down, 
    							&client->max_bw_up, 
    							&client->max_bw_down, 
    							&client->idletimeout, 
    							&client->forcetimeout);
    	    			free(auth_response.ext);
    	    		}
    	    		client->fw_connection_state = FW_MARK_KNOWN;
    	    		client->login_time = get_uptime();
    	    		nf_fw_access(FW_ACCESS_ALLOW, client->ip, client->mac);
                    wifidog_aglink_send_add_sta(client);
    	    		if(config->qos_enable)
    	                fw_qos_add(client);
    				break;
    	        default:
    	    		log_info(__LOG_MSG__, "I don't know what the validation code %d means for token %s from %s at %s - sending error message", 
    										auth_response.authcode, client->token, client->ip, client->mac);
    	    	    break;
    		}

    		UNLOCK_CLIENT_LIST();	
    	}
        }
    }
    fclose(proc);    
    return 0;

fail:
    fclose(proc);    
	return -1;
}


int http_close_sta(char *mac)
{
    t_client *client;
    FILE *proc;
    char acIp[16];
    char acMac[18];
    char arpFlag[4];
    char *ip = NULL;

    if (NULL == mac)
    {
        log_info(__LOG_ERROR__, "mac is NULL\n");
        return -1;
    }
    if (!(proc = fopen("/proc/net/arp", "r"))) 
    {
        log_info(__LOG_ERROR__, "/proc/net/arp: open failed");
        return -1;
    }

    /* Skip first line */
	while (!feof(proc) && fgetc(proc) != '\n');

    while (!feof(proc) && (fscanf(proc, " %15[0-9.] %*s %3[A-Za-z0-9] %17[A-Fa-f0-9:] %*s %*s", acIp,arpFlag ,acMac) == 3))
    {
        if (strcmp(acMac, mac) == 0) 
        {
        	ip = acIp;
            LOCK_CLIENT_LIST();
            client = client_list_find(ip,mac);
            if(NULL == client)
            {
                UNLOCK_CLIENT_LIST();
                log_info(__LOG_MSG__, "client_list_find is NULL,sta_mac=%s,find arpflag is %s",mac,arpFlag);
                continue;
            }
            else
            {
                nf_fw_access(FW_ACCESS_DENY, client->ip, client->mac);
                wifidog_aglink_send_del_sta(client);
                client_list_delete(client);
                log_info(__LOG_MSG__, "close the sta [%s] connect with network success!,arpflag is %s",mac,arpFlag);
            }
            UNLOCK_CLIENT_LIST();
        }
    }
    fclose(proc);    
    return 0;
}

/* END:   Added by cj, 2015/3/27   PN:终端认证状态更新 */
/*----------------------------------------------------------------------*
 * parse url on form:
 *  "<proto>://<host>[:<port>][<uri>]"
 *  returns
 *    0 if parse ok
 *   -1 if parse failed
 *  port sets to 0 if no port is specified in URL
 *  uri is set to "" if no URI is specified
 */
int www_ParseUrl(const char *url, char *proto, char *host, int *port, char *uri)
{
  int n;
  char *p;

  *port = 0;
  strcpy(uri, "");

  /* proto */
  p = (char *) url;
  if ((p = strchr(url, ':')) == NULL) {
    return -1;
  }
  n = p - url;
  strncpy(proto, url, n);
  proto[n] = '\0';

  /* skip "://" */
  if (*p++ != ':') return -1;
  /*if (*p++ != '/') return -1;
  if () return -1;*/
  while(*p++ != '/');

  /* host */
  {
    char *hp = host;
    
    while (*p && *p != ':' && *p != '/') {
      *hp++ = *p++;
    }
    *hp = '\0';
  }
  if (strlen(host) == 0)
    return -1;

  /* end */
  if (*p == '\0') {
    *port = 0;
    strcpy(uri, "");
    return 0;
  }

  /* port */
  if (*p == ':') {
    char buf[10];
    char *pp = buf;

    p++;
    while (isdigit(*p)) {
      *pp++ = *p++;
    }
    *pp = '\0';
    if (strlen(buf) == 0)
      return -1;
    *port = atoi(buf);
  }
  
  /* uri */
  if (*p == '/') {
    char *up = uri;
    while ((*up++ = *p++));
  }

  return 0;
}

#if 0 /*for debug*/
#define JUMP_INFO	"正在为您连接网络，如果没有跳转，请点击 "
#define JUMP_HERE	"[这里]"
#else
#define JUMP_INFO	""
#define JUMP_HERE	""
#endif

extern int g_iIosAutoPortal;
char *first_page_up = 
	"<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">\n"
	"<html>\n"
	"<head>\n"
	"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\">\n"
	"<title>Loading ...</title>\n"
	"<p align=\"center\"><font color=\"#000000\" size=\"10\">"JUMP_INFO"</font> <a style=\"text-decoration: none\" href=\"";
	

char *first_page_middle = 
	"\"><font color=\"#0066ff\" size=\"10\">"JUMP_HERE"</font></a>.</p>\n"
	"<script type=\"text/javascript\">\n"
	"	delayURL(\"";

char *first_page_down = 
	"\");\n"
	"	function delayURL(url) {\n"
	"			window.top.location.href = url;\n"
	"	}\n"
	"</script>\n"
	"</head>\n"
	"<body>\n"
	"	<div></div>\n"
	"</body>\n"
	"</html>\n";

char g_first_page[FIRST_PAGE_TASK_NUM][MAX_BUF];

void http_callback_redirect2local(httpd *webserver, request *r)
{
#ifdef GW_X86
#define MAX_HTML_IDX		2048
#else
#define MAX_HTML_IDX		128
#endif

	int len;
	char fp_file_path[128], fp_file[64];
	FILE* fh;
	static int idx[FIRST_PAGE_TASK_NUM] = {0};
	char *iosstr = NULL;
	int loop = webserver->idx;
	
    log_info(__LOG_DEBUG__, "r->request.host=[%s], r->request.userAgent=[%s], r->clientAddr:%s", 
							r->request.host, r->request.userAgent, r->clientAddr);
    if (strstr(r->request.host, "wifiopenapiauth.com"))
    {
		log_info(__LOG_DEBUG__, "from wifiopenapiauth.com then no first page");
		//http_callback_404(webserver, r);
		goto NO_FIRST_PAGE;
	}
    /*add@2014.10.13 for specified handle if userAgent is ucweb or oupeng*/
    if (strstr(r->request.userAgent, "UC"))
    {
		log_info(__LOG_DEBUG__, "userAgent is ucweb then goto no first page");
        goto NO_FIRST_PAGE;
    }
	if (strstr(r->request.userAgent, "Oupeng"))
    {
		log_info(__LOG_DEBUG__, "userAgent is Oupeng then goto no first page");
        goto NO_FIRST_PAGE;
    }

	iosstr = strstr(g_pstAppleSuccess, r->request.host);
	
#if 0 /*去掉"禁止ios自动弹portal"机制，改为3次即弹success方式*/
    /* 微信认证，提前处理，否则，推送的host为本地host，而非苹果的host */
    if ( ((1 == auth_mode_support_weixin()) || (1 == auth_mode_support_alipay() ) )&& 
        	  (iosstr != NULL) &&
			  (g_iIosForceAutoPortal != 1)) /*如果配置了强制自动弹portal，则不自动返回success*/
    {
		log_info(__LOG_DEBUG__, "send_apple_success1");
        send_apple_success(r);
    }
#else
    if ( (iosstr != NULL) && (0 == g_iIosAutoPortal))
    {
		log_info(__LOG_DEBUG__, "deny auto portal, send_apple_success.");
        send_apple_success(r);
    }
    else if ( (iosstr != NULL) && (2 == g_iIosAutoPortal) && 
		((1 == auth_mode_support_weixin()) || (1 == auth_mode_support_alipay())))
    {
		log_info(__LOG_DEBUG__, "deny auto portal when third-party auth, send_apple_success.");
        send_apple_success(r);
    }
	/*add@2015.02.01 for wechat connect wifi*/
	//else if (auth_mode_support_wxl())
	//{
	//	log_info(__LOG_DEBUG__, "auth_mode support wxl then send_apple_success");
	//	send_apple_success(r);
	//}
#endif
    else
    {	
		s_config *config = config_get_config();
		char *client_mac = NULL;
		if (iosstr != NULL)
		{
			client_mac = arp_get(r->clientAddr);
			if (NULL == client_mac)
			{
				log_info(__LOG_ERROR__, "arp_get(r->clientAddr) fail, ip: %s", r->clientAddr);
				log_info(__LOG_DEBUG__, "http_callback_redirect2local, clientAddr: %s, client_mac is NULL", r->clientAddr);
			}
			else{
				log_info(__LOG_DEBUG__, "http_callback_redirect2local, clientAddr: %s, client_mac: %s", 
					r->clientAddr, client_mac);
			}
			
			if (NULL != client_mac &&
				check_user_ios_access_exist(loop, client_mac) > 0)
			{
				log_info(__LOG_DEBUG__, "check user exist, send_apple_success");
                send_apple_success(r);
				goto end;
			}
			else
			{
				goto NO_FIRST_PAGE;
			}
		}
		else
		{
			goto delay;
		}
	
delay:
    	strcpy(g_first_page[loop], first_page_up);
		len = strlen(g_first_page[loop]);

		r->request.query[512] = '\0';
		
        snprintf(g_first_page[loop] + len, (MAX_BUF - 1 - len), 
		 	"http://%s:8062/redirect?oriUrl=http://%s%s%s%s",
		 	config->gw_address,
            r->request.host,
            r->request.path,
            r->request.query[0] ? "?" : "",
            r->request.query);
		
		len = strlen(g_first_page[loop]);
		strncpy(g_first_page[loop] + len, first_page_middle, MAX_BUF-len-1);

		len = strlen(g_first_page[loop]);
		snprintf(g_first_page[loop] + len, (MAX_BUF - 1 - len), 
		 	"http://%s:8062/redirect?oriUrl=http://%s%s%s%s",
		 	config->gw_address,
            r->request.host,
            r->request.path,
            r->request.query[0] ? "?" : "",
            r->request.query);

		len = strlen(g_first_page[loop]);
		strncpy(g_first_page[loop] + len, first_page_down, MAX_BUF-len-1);

		sprintf(fp_file, "%s%02d%04d.html", PORTAL_FIRST_PAGE, loop, idx[loop]);
		sprintf(fp_file_path, "%s%s", LIGHTTPD_ROOT_DOC, fp_file);
        
        /* 将原访问网站带到重定向后的页面 */
		/*LOCK_FIRST_PAGE();*/
		log_info(LOG_DEBUG, "client(ip=%s) is not exist then redirect to first page", 
								r->clientAddr);
		if ((fh = fopen(fp_file_path, "w+")) != NULL) {
			fwrite(g_first_page[loop], strlen(g_first_page[loop]), 1, fh);
			fclose(fh);
			http_send_to_local(r, fp_file, "Redirect to local page");
    	}
		else
		{
			log_info(LOG_WARNING, "fopen file: %s fail. can't redirect to first-page", fp_file_path);
		}
		/*UNLOCK_FIRST_PAGE();*/

		idx[loop] = ((idx[loop] + 1) % MAX_HTML_IDX);

end:
		#if 1/*def _BOXLL_*/
		if (NULL != client_mac)
		{
			free(client_mac);
		}
		#endif
    }
    return;
/*begin: add@2014.10.13 no first page handle*/
NO_FIRST_PAGE:
	http_callback_404(webserver, r);
    return;
/*end: add@2014.10.13 no first page handle*/

}

/** The 404 handler is also responsible for redirecting to the auth server */
void http_callback_404(httpd *webserver, request *r)
{
	char	tmp_url[MAX_BUF],
			*url;
    char proto[10];
    char host[1024];
    char uri[1024];
    char *ptUri = uri;
    int port;
	int send_authok = 0;

    
    char *oriUrl;
	s_config	*config = config_get_config();
	/* BEGIN:fengjing add 2014-09-28 */
	/* 增加本地检测重定向事宜 */
	if(strcmp(r->clientAddr, config->gw_address) == 0)
	{
		http_send_redirect_to_local(r, AUTH_OK, "Redirect to auth ok");
		return;
	}
	/* END:fengjing add 2014-09-28 */
	t_auth_serv	*auth_server = get_auth_server();
	t_client *client = NULL;
    /*begin: add@2014.8.7   need free client_mac*/
	char * client_mac = NULL;
	log_info(__LOG_MSG__, "[AUTHENTICATION-STEP-[http-callback-404]]. r->clientAddr: %s", r->clientAddr);
	
	client_mac = arp_get(r->clientAddr);
	if (NULL == client_mac)
	{
		log_info(__LOG_ERROR__, "arp_get(r->clientAddr) fail, ip: %s", r->clientAddr);
		log_info(__LOG_DEBUG__, "http_callback_404, clientAddr: %s, client_mac is NULL", r->clientAddr);
	}
	else{
		log_info(__LOG_DEBUG__, "http_callback_404, clientAddr: %s, client_mac: %s", 
			r->clientAddr, client_mac);
	}
	/*end: add@2014.8.7*/
	memset(tmp_url, 0, sizeof(tmp_url));
    
    if (strstr(r->request.path, "redirect")){
        if (NULL != (oriUrl = strstr(r->request.query, "http"))){
            www_ParseUrl(oriUrl, proto, host, &port, uri);
            if (0 == port)
                sprintf(r->request.host, "%s", host);
            else
                sprintf(r->request.host, "%s:%d", host, port);
            while('\0' != *ptUri && '?' != *ptUri++);
            strcpy(r->request.query, ptUri);
            *ptUri = '\0';
            strcpy(r->request.path, uri);
    		log_info(__LOG_DEBUG__, "[r->request.host=%s] [r->request.path=%s] [r->request.query=%s]", r->request.host, r->request.path, r->request.query);
        }
    }

    r->request.query[512] = '\0';
    
	/* 
	 * XXX Note the code below assumes that the client's request is a plain
	 * http request to a standard port. At any rate, this handler is called only
	 * if the internet/auth server is down so it's not a huge loss, but still.
	 */
    snprintf(tmp_url, (sizeof(tmp_url) - 1), "http://%s%s%s%s",
                        r->request.host,
                        r->request.path,
                        r->request.query[0] ? "?" : "",
                        r->request.query);
	url = httpdUrlEncode(tmp_url);

	if (!is_online()) {
		/* The internet connection is down at the moment  - apologize and do not redirect anywhere */
		http_send_redirect_to_local(r, DEFAULT_OFFLINEFILE, "Redirect to offline");
		log_info(__LOG_MSG__, "Sent %s an apology since I am not online - no point sending them to auth server", r->clientAddr);
	}
	#if 0
	else if (!is_auth_online()) {
		/* The auth server is down at the moment - apologize and do not redirect anywhere */
		char * buf;
		safe_asprintf(&buf, 
			"<p>很抱歉, 热点当前不能连接到认证服务器.</p>"
			"<p>请稍等片刻再<a href='%s'>重试</a>, 或者联系管理人员并告知该情况</p>", tmp_url);

                send_http_page(r, "~-~认证页面无法打开", buf);
		free(buf);
		log_info(__LOG_MSG__, "Sent %s an apology since auth server not online - no point sending them to auth server", r->clientAddr);
	}
	#endif
	else {
		/* Re-direct them to auth server */

        /**@ 如果iphone 正在关联的状态上, 访问特定的几个url: www.itools.com www.appleiphonecell.com www.apple.com 
         **  则直接回复success 页面
         修正:只有支持微信认证，才会做此操作
         */
        log_info(__LOG_DEBUG__, "requestHost: %s", r->request.host);
        /* BEGIN: Added by WeiHonggang, 2014/2/19   PN:TASK#0平台支持认证模式同步到网关，并根据认证模式进行设置  */
        //if(NULL != strstr(g_pstAppleSuccess, r->request.host))
        #if 0
        if((1 == auth_mode_support_weixin()) && (NULL != strstr(g_pstAppleSuccess, r->request.host)))
        /* END:   Added by WeiHonggang, 2014/2/19   PN:TASK#0平台支持认证模式同步到网关，并根据认证模式进行设置  */

        {
            send_apple_success(r);
        }
		else
		{
		#endif
        /* add by zhz 防止iPhone自动推两次portal */
			if (NULL != client_mac)
			{
				LOCK_CLIENT_LIST();

				client = client_list_find(r->clientAddr, client_mac);
				
		        if (NULL != client){
					/*用户已认证说明nf_portal与wifidog不同步*/
					if (FW_MARK_KNOWN == client->fw_connection_state){
						log_info(LOG_WARNING, "client(ip=%s mac=%s) is ACCESS_ALLOW, then notify nf_portal set client state",
												client->ip, client->mac);
						sys_log_set(WIFIDOG_USR_LOG_FILE, "nf-ALLOW: cb-404 %s %s", client->ip, client->mac);
						client_list_auth_update(client);
						nf_fw_access(FW_ACCESS_ALLOW, client->ip, client->mac);
					}
					else{
						log_info(LOG_WARNING, "client(ip=%s mac=%s) exist but not ACCESS_ALLOW", client->ip, client->mac);
					}	
					if(NULL != strstr(g_pstAppleSuccess, r->request.host)){
						send_authok = 1;
					}
		        }
				
				UNLOCK_CLIENT_LIST();
			}
			else{
			    log_info(__LOG_ERROR__, "NULL == client_mac, arp find mac fail, addr:%s", r->clientAddr);
			    return;
			}

			if(send_authok != 0){
	            http_send_to_local(r, AUTH_OK, "Redirect to OK page");
	            log_info(__LOG_DEBUG__, "client_list_find is not Null  r->clientAddr=%s", r->clientAddr);
			}
	        else
			{
	    		char *urlFragment;
	    		char ucVersion =1;
	            log_info(__LOG_DEBUG__, "client_list_find is Null r->clientAddr=%s", r->clientAddr);
				strncpy(r->clientMac, client_mac, HTTP_MAC_LEN);
	    		safe_asprintf(&urlFragment, "%sgw_address=%s&gw_port=%d&gw_id=%s&ip=%s&mac=%s&version=%d&url=%s",
	    			auth_server->authserv_login_script_path_fragment,
	    			config->gw_address,
	    			config->gw_port, 
	    			config->gw_id,
	    			r->clientAddr,
	    			client_mac,
	    			ucVersion,
	    			url);
	    		log_info(__LOG_DEBUG__, "Captured %s requesting [%s] and re-directing them to login page", r->clientAddr, url);
				/*http redirect packet:
				  *http://www.login-wifi.com:80/cmps/admin.php/api/login/?gw_address=172.16.0.1&gw_port=8060&gw_id=xb-x86
				  *&ip=172.16.222.41&mac=48:74:6e:36:17:2d&url=http%3A//www.thinkdifferent.us/BUrWQgVk3FZ/r2yLUfFV4NO.html
				  *&apmac=ac:34:cb:c0:93:ed
				  */
				http_send_redirect_to_auth(r, urlFragment, "Redirect to login page");
	    		free(urlFragment);
	        }

			free(client_mac);
		}
	free(url);
}
/*begin: add@2015.01.27 for wechat connect wifi*/
int http_wxl_auth_encrypt(char *in, int len, 
								  char *out, int *outlen)
{
	char *c = out;
	int cnt = 0;
	base64_encodestate s;
	
	/* initialise the decoder state */
	base64_init_encodestate(&s);
	/* decode the input data */
	cnt = base64_encode_block(in, len, c, &s);
	c += cnt;
	*c = '\0';
	*outlen = cnt;
	return 0;
}

extern int get_sta_ap_mac(char *str_mac, char *ap_mac);
extern int get_sta_ap_mac2(char *str_mac, char *ap_mac);

extern int get_sta_ap_mac2(char *str_mac, char *ap_mac);


void http_wxl_redirect_callback(httpd *webserver, request *r)
{
	char *message = NULL;
	char *header = NULL;
	char *response = NULL;
	char acTmpBuf[1024] = {0};
	char acOutBuf[1024] = {0};
//	char acOutBuf1[256] = {0};
//	char acEndBuf[256] = {0};
	int iOutLen = 0;
	int iLoop = 0;
//	int iLoop1 = 0;
	s_config	*config = config_get_config();
	//t_client *client = NULL;
	char * client_mac = NULL;
	unsigned char aucBssid[6] = {0};
	
	log_info(__LOG_MSG__, "[WXL-STEP6-[http_wxl_redirect_callback]] begin. r->clientAddr: %s", r->clientAddr);
	
	client_mac = arp_get(r->clientAddr);
	if (NULL == client_mac){
		log_info(__LOG_ERROR__, "arp_get(r->clientAddr) fail, ip: %s", r->clientAddr);
		log_info(__LOG_DEBUG__, "clientAddr: %s, client_mac is NULL", r->clientAddr);
	}
	else{
		log_info(__LOG_DEBUG__, "clientAddr: %s, client_mac: %s",r->clientAddr, client_mac);
		strncpy(r->clientMac, client_mac, HTTP_MAC_LEN);		
		free(client_mac);
	}

	if (!is_online()){
		log_info(__LOG_WARNING__, "[WXL-STEP6-[http_wxl_redirect_callback]] gw is offline, clientAddr: %s",r->clientAddr);
	}
	else{
		WD_AP_T *pstap;
		unsigned char apmac[6];
		struct timeval now;
		char ap_mac[32] = "";
		char tmp[5];
		unsigned long long tv_msec;

		gettimeofday(&now, NULL);
		tv_msec = (unsigned long long)now.tv_sec*1000;
		tv_msec += (unsigned long long)now.tv_usec / 1000;
		get_sta_ap_mac2(r->clientMac, ap_mac);
		strmac_to_mac(ap_mac, apmac);
		pstap = ap_find_by_mac(apmac);
		if(pstap != NULL)
			memcpy((char *)aucBssid, (char *)pstap->astRadio[0].astVap[0].aucBssid, 6);

		/* Re-direct them to wechat client */
		/*拼接字符串:1.	将手机MAC地址XX:XX:XX:XX:XX:XX、
		  * 网关 MAC地址YY:YY:YY:YY:YY:YY和网关IP W.W.W.W 网关port的合并字串
		  * "XX:XX:XX:XX:XX:XX|YY:YY:YY:YY:YY:YY|W.W.W.W|xxxx" */
		//sprintf(acTmpBuf, "%s|%s|%s|%d", r->clientMac, config->wan_mac, config->gw_address, config->gw_port);
		/*贝联参数*/
		/*gwid=61a57f3fabd4bde1b295d5e28b3730fc&user_ip=192.168.1.101&mac=90:fd:61:6
			7:7a:46&router_ver=2.3.0&timestamp=1418891009502846156&bssid=00:01:40:01:0
			7:c5&apmac=00:01:40:01:08:c4*/
		sprintf(acTmpBuf, "gwid=%s&user_ip=%s&mac=%s&router_ver=%s&timestamp=%llu&bssid=%02x:%02x:%02x:%02x:%02x:%02x&apmac=%s", 
			config->gw_id,
			r->clientAddr,
			r->clientMac,
			"1.1.1.0",
			tv_msec, /*毫秒时间戳*/
			aucBssid[0], aucBssid[1], aucBssid[2], aucBssid[3], aucBssid[4], aucBssid[5],
			ap_mac);
		
		log_info(__LOG_DEBUG__, "wxl acTmpBuf: %s", acTmpBuf);
		http_wxl_auth_encrypt(acTmpBuf, strlen(acTmpBuf), acOutBuf, &iOutLen);

		log_info(__LOG_DEBUG__, "wxl acOutBuf: %s", acOutBuf);
#if 1/*贝联需要将字符串 0~4字符 与5~9字符交换*/
		for (iLoop = 0; iLoop < iOutLen; iLoop++){
			strncpy(tmp, &acOutBuf[0], 5);
			strncpy(&acOutBuf[0], &acOutBuf[5], 5);
			strncpy(&acOutBuf[5], tmp, 5);
		}
		char *url;
		safe_asprintf(&url, "http://%s?auth=%s", WXL_REDIRECT_URL, acOutBuf);
#else
		for (iLoop = 0, iLoop1 = 0; iLoop < iOutLen; iLoop++){
			acOutBuf1[iLoop1] = acOutBuf[iLoop];
			if (acOutBuf1[iLoop] == '='){
				acOutBuf1[iLoop] = '\0';
				strcat(acEndBuf, acOutBuf1);
				strcat(acEndBuf, "o0o0o");
				iLoop1 = 0;
			}
			iLoop1++;
		}
		strcat(acEndBuf, acOutBuf1);
		log_info(__LOG_DEBUG__, "aucEndBuf: %s", acEndBuf);
		char *url;
		safe_asprintf(&url, "http://%s?auth=%s", WXL_REDIRECT_URL, acEndBuf);
#endif		
		

		log_info(__LOG_DEBUG__, "Redirecting client browser to %s", url);
		safe_asprintf(&header, "Location: %s", url);
		safe_asprintf(&response, "302 Redirecting\n");	
		httpdSetResponse(r, response);
		httpdAddHeader(r, header);
		free(response);
		free(header);	
		safe_asprintf(&message, "如果没自动跳转, 请<a href='%s'>点击</a>.", url);
		send_http_page(r, "Redirection to message", message);
		free(message);
		free(url);
	}
	return;
}

void http_wxl_redirect2boa_callback(httpd *webserver, request *r)
{
#define BOA_NEW_PORT 8090
	char *message = NULL;
	char *header = NULL;
	char *response = NULL;
	char host_path[128];
	char *pt;
	
	strncpy(host_path, r->request.host, 128);
	if((pt = strstr(host_path, "//")) != NULL) /*去掉host中的子路径*/
		*pt = '\0';
	
	log_info(__LOG_MSG__, "[WXL-STEP6-[http_wxl_redirect2boa_callback]] begin. r->clientAddr: %s r->request.host = %s", 
		r->clientAddr, r->request.host);
	
	char *url;
	safe_asprintf(&url, "http://%s:%d", host_path, BOA_NEW_PORT);
	log_info(__LOG_DEBUG__, "Redirecting client browser to %s", url);
	safe_asprintf(&header, "Location: %s", url);
	safe_asprintf(&response, "302 Redirecting\n");	
	httpdSetResponse(r, response);
	httpdAddHeader(r, header);
	free(response);
	free(header);	
	safe_asprintf(&message, "如果没自动跳转, 请<a href='%s'>点击</a>.", url);
	send_http_page(r, "Redirection to message", message);
	free(message);
	free(url);

	return;
}

/*end: add@2015.01.27 for wechat connect wifi*/


/*alipay auth 2015-3-6*/
void http_alipay_redirect_callback(httpd *webserver,request *r)
{
    char *header = NULL;
    char *response = NULL;
    char *message = NULL;
    s_config	*config = config_get_config();

     
    #if 1
    char *pcBegin = NULL,*pcEnd = NULL;
    pcBegin = strstr(r->readBuf,"alipaytoken=");
    pcEnd = strstr(r->readBuf,"HTTP/");
    if((NULL == pcBegin) || (NULL == pcEnd))
    {
        log_info(__LOG_DEBUG__,"not find alipaytoken");
        return;
    }
    pcBegin += strlen("alipaytoken=");
   
   *(--pcEnd) = '\0';
    char *token=NULL;
    safe_asprintf(&token,"%s",pcBegin);
    if(NULL == token)
    {
        return;
    }
    *pcEnd = ' ';
    log_info(__LOG_DEBUG__,"****alitoken=%s",token);
   #endif

    if (!is_online())
    {
		log_info(__LOG_WARNING__, "[ALIAUTH-[http_alipay_redirect_callback]] gw is offline, clientAddr: %s",r->clientAddr);
    }
    else
    {
        char *url;
        safe_asprintf(&url, "http://%s:8060/wifidog/auth?alitoken=%s",config->gw_address, token);/*重定向到本地8060端口*/
        log_info(__LOG_DEBUG__, "Redirecting client browser to %s", url);
        safe_asprintf(&header, "Location: %s", url);
        safe_asprintf(&response, "302 Redirecting\n");	
        httpdSetResponse(r, response);
        httpdAddHeader(r, header);
        free(response);
        free(header);	
        safe_asprintf(&message, "如果没自动跳转, 请<a href='%s'>点击</a>.", url);
        send_http_page(r, "Redirection to message", message);
        free(message);
        free(url);
    }
    free(token);
    return;
}





void http_callback_wifidog(httpd *webserver, request *r)
{
	send_http_page(r, "WEB", "This is the WEB authentication router background status page.");
}

void http_callback_about(httpd *webserver, request *r)
{
	send_http_page(r, "About WEB", "This is a customized version of the WEB authentication router[<strong>" VERSION "</strong>]");
}

void http_callback_status(httpd *webserver, request *r)
{
	const s_config *config = config_get_config();
	char * status = NULL;
	char *buf;

	if (config->httpdusername && 
			(strcmp(config->httpdusername, r->request.authUser) ||
			 strcmp(config->httpdpassword, r->request.authPassword))) {
		log_info(__LOG_MSG__, "Status page requested, forcing authentication");
		httpdForceAuthenticate(r, config->httpdrealm);
		return;
	}

	//status = get_status_text();
	safe_asprintf(&buf, "<pre>%s</pre>", status);
	send_http_page(r, "WEB auth state", buf);
	free(buf);
	free(status);
}

extern int get_sta_wifi_cnnt(char *str_mac, char *ap_mac, char *ssid);

/** @brief Convenience function to redirect the web browser to the auth server
 * @param r The request 
 * @param urlFragment The end of the auth server URL to redirect to (the part after path)
 * @param text The text to include in the redirect header ant the mnual redirect title */
void http_send_redirect_to_auth(request *r, char *urlFragment, char *text)
{
	char *protocol = NULL;
	int port = 80;
	char ap_mac[32] = "";
	char ssid[64] = "";
	t_auth_serv	*auth_server = get_auth_server();
	
	log_info(__LOG_DEBUG__, "[AUTHENTICATION-STEP-[send-redirect-to-auth]]. r->clientAddr: %s", r->clientAddr);

	get_sta_wifi_cnnt(r->clientMac, ap_mac, ssid);

	if (auth_server->authserv_use_ssl) {
		protocol = "https";
		port = auth_server->authserv_ssl_port;
	} else {
		protocol = "http";
		port = auth_server->authserv_http_port;
	}
			    		
	char *url = NULL;
	safe_asprintf(&url, "%s://%s:%d%s%s%s%s",
		protocol,
		auth_server->authserv_hostname,
		port,
		auth_server->authserv_path,
		urlFragment,
		ap_mac,
		ssid
	);
	http_send_redirect(r, url, text);
	free(url);	
}

/* BEGIN: Added by Zhou Shouya, 2014/1/22   PN:支持本地页面重定向 */
void http_send_redirect_to_local(request *r, char *urlFragment, char *text)
{
    char *url = NULL;
    char *header = NULL;
    char *response = NULL;
    char *message = NULL;
    
    safe_asprintf(&url, "http://%s:8080%s",
        config_get_config()->gw_address,
        urlFragment
    );
    log_info(__LOG_DEBUG__, "Redirecting client browser to %s", url);
    safe_asprintf(&header, "Location: %s", url);
    if(text) {
		safe_asprintf(&response, "307 %s\n", text);	
	}
	else {
		safe_asprintf(&response, "307 %s\n", "Redirecting");		
	}
	httpdSetResponse(r, response);
	httpdAddHeader(r, header);
	safe_asprintf(&message, "如果没自动跳转, 请<a href='%s'>点击</a>.", url);
	send_http_page(r, text ? text : "Redirection to message", message);
	free(response);
	free(header);
	free(message);
    free(url);
    return;
}
/* END:   Added by Zhou Shouya, 2014/1/22 */

		
#define IP_FMT	"%u.%u.%u.%u"
#define IP_ARG(x) ((unsigned char*)(x))[0], ((unsigned char*)(x))[1], ((unsigned char*)(x))[2], ((unsigned char*)(x))[3]


/** @brief Sends a redirect to the web browser 
 * @param r The request 
 * @param url The url to redirect to
 * @param text The text to include in the redirect header and the manual redirect link title.  NULL is acceptable */
void http_send_redirect(request *r, char *url, char *text)
{
		char *message = NULL;
		char *header = NULL;
		char *response = NULL;
		
		/* Re-direct them to auth server */
		log_info(__LOG_DEBUG__, "Redirecting client browser to %s", url);
		/*auth result redirect to client browser:
		  *http://www.login-wifi.com:80/cmps/admin.php/api/portal/?gw_id=xb-x86&token=93230d4f9111b12fd92543
		  *cf133618926dd9371e&apmac=ac:34:cb:c0:93:ed
		  */
		safe_asprintf(&header, "Location: %s",
			url
		);
		if(text) {
			safe_asprintf(&response, "307 %s\n",
				text
			);	
		}
		else {
			safe_asprintf(&response, "307 %s\n",
				"Redirecting"
			);		
		}	
		httpdSetResponse(r, response);
		httpdAddHeader(r, header);
		free(response);
		free(header);	
		safe_asprintf(&message, "如果没自动跳转, 请<a href='%s'>点击</a>.", url);
		send_http_page(r, text ? text : "Redirection to message", message);
		free(message);
}

/*begin: add@2015.02.11 for wechat connect wifi*/
#define HTTP_TOKEN_OTHER_MAC "other_mac="
#define HTTP_TOKEN_OTHER_TOKEN "other_token="
int http_callback_auth_client_add(request *r, char *ip, char *mac, char *token)
{
	t_client *client;
	
	if (NULL == ip || NULL == mac || NULL == token){
		log_info(__LOG_ERROR__, "NULL == ip || NULL == mac || NULL == token, fail");
		return -1;
	}

    	/*2015-3-31 wxl认证增加认证方式字段*/
	httpVar *type = httpdGetVariableByName(r, "type");
	char *method = NULL;
	if(NULL != type)
	{
	    if(NULL != type->value)
	    {
	        method = safe_strdup(type->value);
	    }
	    log_info(__LOG_DEBUG__,"****auth type:%s*****",type->value);
	}
	else
	{
	    method = NULL;
	    //sys_log_set(WIFIDOG_USR_LOG_FILE,"****auth type:NULL*****");
	}




	log_info(__LOG_MSG__, "http_callback_auth_client_add ip %s mac %s token %s", ip, mac, token);
	
	LOCK_CLIENT_LIST();
	if ((client = client_list_find(ip, mac)) == NULL) {
	    /** 判断是否超过最大接入用户数 */
        if(g_client_access_num >= config_get_config()->max_client)
        {
            http_send_redirect_to_local(r, DEFAULT_MAXUSERFILE, "Redirect to max users");
            UNLOCK_CLIENT_LIST();
            return -1;
        }
		if((config_get_config()->dev_max_client > 0) &&
			(g_client_access_num >= config_get_config()->dev_max_client))
		{
            http_send_redirect_to_local(r, DEFAULT_MAXUSERFILE, "Redirect to max users");
            UNLOCK_CLIENT_LIST();
            return -1;
        }
		log_info(__LOG_MSG__, "New client for %s", ip);
		client_list_append(ip, mac, token);
	}
	else{ /*更新other ip和other mac*/
		if (client->other_mac != NULL){
        	free(client->other_mac);
			client->other_mac = NULL;
		}
	    if (client->other_ip != NULL){
	        free(client->other_ip);
			client->other_ip = NULL;
	    }

            #if 0
            if(NULL != client->method_type)
            {
	            free(client->method_type);
	            client->method_type = NULL;
            }
            if(NULL != method)
            {
	            client->method_type = safe_strdup(method);
            }
            #endif
	}
	UNLOCK_CLIENT_LIST();

	if(NULL != method)
	{
	    free(method);
	    method = NULL;
	}
	return 0;
}

int http_callback_auth_client_add2(request *r, char *ip, char *mac, char *token,
												char *other_ip, char *other_mac)
{
	t_client *client;
	
	if (NULL == ip || NULL == mac || NULL == token){
		log_info(__LOG_ERROR__, "NULL == ip || NULL == mac || NULL == token, fail");
		return -1;
	}

           /*2015-3-31 wxl认证增加认证方式字段*/
    	httpVar *type = httpdGetVariableByName(r, "type");
	char *method =NULL;
        if(NULL != type)
	{
	    if(NULL != type->value)
	    {
	        method = safe_strdup(type->value);
	    }
	    log_info(__LOG_DEBUG__,"****auth type:%s*****",type->value);
	}
	else
	{
	    method = NULL;
	    //sys_log_set(WIFIDOG_USR_LOG_FILE,"****auth type:NULL*****");
	}

	log_info(__LOG_MSG__, "http_callback_auth_client_add ip %s mac %s token %s", ip, mac, token);
	
	LOCK_CLIENT_LIST();
	if ((client = client_list_find(ip, mac)) == NULL) {
	    /** 判断是否超过最大接入用户数 */
        if(g_client_access_num >= config_get_config()->max_client)
        {
            http_send_redirect_to_local(r, DEFAULT_MAXUSERFILE, "Redirect to max users");
            UNLOCK_CLIENT_LIST();
            return -1;
        }
		if((config_get_config()->dev_max_client > 0) &&
			(g_client_access_num >= config_get_config()->dev_max_client))
		{
            http_send_redirect_to_local(r, DEFAULT_MAXUSERFILE, "Redirect to max users");
            UNLOCK_CLIENT_LIST();
            return -1;
        }
		log_info(__LOG_MSG__, "New client for %s", ip);
		client_list_append2(ip, mac, token, other_ip, other_mac);
	}
	else{ /*更新other ip和other mac*/
		if (client->other_mac != NULL){
        	free(client->other_mac);
		}
	    if (client->other_ip != NULL){
	        free(client->other_ip);
	    }

		client->other_ip = safe_strdup(other_ip);
		client->other_mac = safe_strdup(other_mac);

            #if 0
            if(NULL != client->method_type)
            {
                free(client->method_type);
                client->method_type = NULL;
            }
            if(NULL != method )
            {
                client->method_type = safe_strdup(method);
            }
            #endif
	}
	UNLOCK_CLIENT_LIST();
    	if(NULL != method)
	{
	    free(method);
	    method = NULL;
	}
	
	return 0;
}

/*end: add@2015.02.11 for wechat connect wifi*/
void  http_callback_auth(httpd *webserver, request *r)
{
	httpVar * token;
	httpVar * other_mac;
	httpVar * other_ip;
	httpVar * other_token;
	char	*mac;
	httpVar *logout = httpdGetVariableByName(r, "logout");
	
	log_info(__LOG_MSG__, "[AUTHENTICATION-STEP-[http-callback-auth]]. r->clientAddr: %s", r->clientAddr);

        
        if((token = httpdGetVariableByName(r, "alitoken")))/*2015-3-16 new alipay auth*/
	{
            http_alipay_token_check_request(webserver, r,token->value);        
	}
	else if ((token = httpdGetVariableByName(r, "token"))) 
	{
		
		/*begin: add@ 2015.02.11 for wechat connect wifi pc connect wifi*/
		/*token=sdfsdfsdfsdfsdfsdfsdf&other_mac=ac:8a:e5:b0:2a:12&other_token=sdfsdfsdfsdfqq*/
		log_info(__LOG_MSG__, "token->value = %s", token->value);
		char token_1[64] = {0};
		char token_2[64] = {0};
		char mac_2[64] = {0};
		char ip_2[64] = {0};
		int have_client2 = 0;

		strncpy(token_1, token->value, 64);
		log_info(__LOG_MSG__, "token_1 = %s", token_1);

		other_ip = httpdGetVariableByName(r, "other_ip");
		other_mac = httpdGetVariableByName(r, "other_mac");
		other_token = httpdGetVariableByName(r, "other_token");

		if (other_ip && other_mac && other_token)
		{			
			strncpy(ip_2, other_ip->value, 64);
			strncpy(mac_2, other_mac->value, 64);
			strncpy(token_2, other_token->value, 64);
			log_info(__LOG_MSG__, "token_2 = %s", token_2);
			log_info(__LOG_MSG__, "ip_2   = %s", ip_2);
			log_info(__LOG_MSG__, "mac_2   = %s", mac_2);
			
			have_client2 = 1;
		}
		else
		{
			have_client2 = 0;
            }
		/*end: add@ 2015.02.11 for wechat connect wifi pc connect wifi*/
		
		/* They supplied variable "token" */
		
		if (!(mac = arp_get(r->clientAddr)))
		{
			/* We could not get their MAC address */
			log_info(__LOG_MSG__, "Failed to retrieve MAC address for ip %s", r->clientAddr);
			send_http_page(r, "WiFiDog Error", "Failed to retrieve your MAC address");
		} 
		else 
		{
			/* We have their MAC address */
			strncpy(r->clientMac, mac, HTTP_MAC_LEN);
			free(mac);

			if (1 == have_client2){
				if(!strncmp(r->clientMac, mac_2, HTTP_MAC_LEN)) /*other mac 和设备mac一样*/
					have_client2 = 0;
			}

			if (1 == have_client2)
			{
				if (0 != http_callback_auth_client_add2(r, r->clientAddr, r->clientMac, token_1,
																ip_2, mac_2))
				{
					return;
				}
				if (0 != http_callback_auth_client_add(r, ip_2, mac_2, token_2)){
					return;
				}
			}
			else
			{
				if (0 != http_callback_auth_client_add(r, r->clientAddr, r->clientMac, token_1))
				{
					return;
				}	
			}

			if (!logout) 
			{
	            auth_server_request(r, REQUEST_TYPE_LOGIN, r->clientAddr, r->clientMac, token_1, 0, 0);
				if (1 == have_client2){
					auth_server_request(r, REQUEST_TYPE_LOGIN, ip_2, mac_2, token_2, 0, 0);
				}
#ifdef HAVE_NETSNIFF
				{
					httpVar *info = httpdGetVariableByName(r, "info");
					if(NULL == info){
						send_client_login_msg_to_netsniff(r->clientAddr, r->clientMac, NULL);
						if (1 == have_client2)
							send_client_login_msg_to_netsniff(ip_2, mac_2, NULL);
					}
					else{
						send_client_login_msg_to_netsniff(r->clientAddr, r->clientMac, info->value);
						if (1 == have_client2)
							send_client_login_msg_to_netsniff(ip_2, mac_2, info->value);
					}
				}
#endif
			}
		}
	} else {
		/* They did not supply variable "token" */
		send_http_page(r, "WiFiDog error", "Invalid token");
	}
	return;
}



/*
* new alipay gateway send token to cloud check
* add client list
* send token to cloud
*/
void  http_alipay_token_check_request(httpd *webserver, request *r,char *token)
{
    char	*mac;
    if (!(mac = arp_get(r->clientAddr))) /*can't get mac*/
    {
        log_info(__LOG_MSG__, "Failed to retrieve MAC address for ip %s", r->clientAddr);
    } 
    else /*get mac*/
    {
        memcpy(r->clientMac, mac, strlen(mac));
        r->clientMac[HTTP_MAC_LEN-1] = '\0';
        free(mac);
        log_info(__LOG_MSG__, "  [ip %s  : MAC: %s]", r->clientAddr,r->clientMac);
		
        if (0 != http_callback_auth_client_add(r, r->clientAddr, r->clientMac, token))
        {
            return;
        }	

        /*send token to cloud  */
        ali_auth_server_request(r, REQUEST_TYPE_LOGIN, r->clientAddr, r->clientMac, token, 0, 0);
    }
	return;
}



void  http_callback_roaming(httpd *webserver, request *r)
{
	t_client *client;
	httpVar * token;
	char	*mac;
	long now = get_uptime();
	char *urlFragment = NULL;
	char *oriurl = NULL, *str = NULL;
	
	log_info(__LOG_MSG__, "[AUTHENTICATION-STEP-[http_callback_roaming]]. r->clientAddr: %s", r->clientAddr);
	if ((token = httpdGetVariableByName(r, "token"))) {
		/* They supplied variable "token" */
		if (!(mac = arp_get(r->clientAddr))) {
			/* We could not get their MAC address */
			log_info(__LOG_MSG__, "Failed to retrieve MAC address for ip %s", r->clientAddr);
			send_http_page(r, "WiFiDog Error", "Failed to retrieve your MAC address");
		} else {
			/* We have their MAC address */
			memcpy(r->clientMac, mac, strlen(mac));
			r->clientMac[HTTP_MAC_LEN-1] = '\0';
			free(mac);
			LOCK_CLIENT_LIST();
			if ((client = client_list_find(r->clientAddr, r->clientMac)) == NULL) {
			    /** 判断是否超过最大接入用户数 */
                if(g_client_access_num >= config_get_config()->max_client)
                {
                    http_send_redirect_to_local(r, DEFAULT_MAXUSERFILE, "Redirect to max users");
                    UNLOCK_CLIENT_LIST();
                    return;
                }
				if((config_get_config()->dev_max_client > 0) &&
					(g_client_access_num >= config_get_config()->dev_max_client))
				{
                    http_send_redirect_to_local(r, DEFAULT_MAXUSERFILE, "Redirect to max users");
                    UNLOCK_CLIENT_LIST();
                    return;
                }
				log_info(__LOG_MSG__, "New client for %s", r->clientAddr);
				client = client_list_append(r->clientAddr, r->clientMac, token->value);
			} 
 			else {
 			    /**@ 如果用户已经存在,则更新token */
				if(NULL != client->token)
 			    	free(client->token);
 			    client->token = safe_strdup(token->value);
				log_info(__LOG_DEBUG__, "Client for %s is already in the client list", client->ip);
			}
			UNLOCK_CLIENT_LIST();

			if(client != NULL){
				/*漫游用户直接放行，并且重定向到原访问网址*/
				client->fw_connection_state = FW_MARK_KNOWN;
				client->login_time = now;
				client->online_time_last_update_time = now;
				client->auth_period  = now - client->auth_period;
				sys_log_set(WIFIDOG_USR_LOG_FILE, "nf-ALLOW ROAMING %s %s", client->ip, client->mac);
				client_list_auth_update(client);
				nf_fw_access(FW_ACCESS_ALLOW, client->ip, client->mac);
				client->bw_up = config_get_config()->bw_up;
				client->bw_down = config_get_config()->bw_down;
				
				if(config_get_config()->qos_enable)
					fw_qos_add(client);

				if((oriurl = strstr(r->readBuf, "&oriUrl=")) != NULL){
					oriurl += strlen("&oriUrl=");
					if((str = strstr(oriurl, " HTTP/1.1")) != NULL)
						*str = '\0';
					
					safe_asprintf(&urlFragment, "%s", oriurl);
				}
				else{
					safe_asprintf(&urlFragment, "%s", "http://www.baidu.com");
				}
				http_send_redirect(r, urlFragment, "Redirect to oriUrl");
				free(urlFragment);
			}
		}
	} else {
		/* They did not supply variable "token" */
		send_http_page(r, "WiFiDog error", "Invalid token");
	}
	return;
}

/*BEGIN: add@2014.11.14 终端获取它的mac地址*/
void http_callback_sta_get_itsmac(httpd *webserver, request *r)
{
	char fp_file_path[128];
	FILE* fh;
    char sta_mac_html_buf[1024] = {'\0'};
	
    log_info(__LOG_DEBUG__, "http callback sta get itsmac r->clientAddr:%s", r->clientAddr);
	char *client_mac = NULL;
	char actmp[32] = "";
    char acApMac[32] = "";
	char ssid[64] = "";
    
	client_mac = arp_get(r->clientAddr);
    if (client_mac){
        if (0 == get_sta_wifi_cnnt(client_mac, actmp, ssid) ){
			sscanf(actmp, "&apmac=%s", acApMac);
        }
        else{
			strcpy(acApMac, "00:00:00:00:00:00");
        }
        
        snprintf(sta_mac_html_buf, 1023, "{\"resultCode\":%d,\"data\":\"{\\\"sta_mac\\\":\\\"%s\\\", \\\"ap_mac\\\":\\\"%s\\\"}\"}",
                               0, client_mac, acApMac);

        log_info(__LOG_DEBUG__, "sta_mac_html_buf: %s", sta_mac_html_buf);
		sprintf(fp_file_path, "%s%s", LIGHTTPD_ROOT_DOC, STA_MAC_PAGE);
    }
    else{
		snprintf(sta_mac_html_buf, 1023, "{\"resultCode\":%d}", 1);
		sprintf(fp_file_path, "%s%s", LIGHTTPD_ROOT_DOC, STA_MAC_PAGE);
    }

    
	if ((fh = fopen(fp_file_path, "w+")) != NULL) {
		fwrite(sta_mac_html_buf, strlen(sta_mac_html_buf), 1, fh);
		fclose(fh);
		http_send_to_local(r, STA_MAC_PAGE, "Redirect to local page: sta_mac.html");
	}
	else
	{
		log_info(LOG_WARNING, "fopen file: %s fail. can't redirect to sta_mac.html", fp_file_path);
	}

	if (NULL != client_mac)
	{
		free(client_mac);
	}
    return;
}
/*END: add@2014.11.14 终端获取它的mac地址*/


/*BEGIN: add@2014.12.25 终端下线*/
void http_callback_sta_logout(httpd *webserver, request *r)
{
	char fp_file_path[128];
	FILE* fh;
    char sta_logout_html_buf[256] = {'\0'};
	int result = 0;
	char *client_mac = NULL;
	t_client *client;
	s_config *config = config_get_config();
	
    log_info(__LOG_DEBUG__, "http callback sta logout r->clientAddr:%s", r->clientAddr);
    
	client_mac = arp_get(r->clientAddr);
    if (client_mac){
		/*立即立即在nf_portal中下线*/
		sys_log_set(WIFIDOG_USR_LOG_FILE, "nf-DENY: app-sta-logout %s %s", r->clientAddr, client_mac);
	    nf_fw_access(FW_ACCESS_DENY, r->clientAddr, client_mac);
		
	    if (config->auth_servers != NULL) {
			memcpy(r->clientMac, client_mac, strlen(client_mac));
			r->clientMac[HTTP_MAC_LEN-1] = '\0';
			
			/* fengjing add 2015-01-07 */
			LOCK_CLIENT_LIST();
			if ((client = client_list_find(r->clientAddr, r->clientMac)) != NULL){
				/*设置标记，会在deny任务中再上报一次*/
				/*wifidog中删除用户仅在deny任务的response中进行*/
				client->set_logout = 1;
				client->fw_connection_state = FW_MARK_PROBATION;
				log_info(__LOG_DEBUG__, "passive_user_logout");
				/* fengjing add 2015-01-07 */
				/*立即通过接口auth?stage=logout上报给云平台*/
				passive_user_logout(client);
			}
			/* fengjing add 2015-01-07 */
			UNLOCK_CLIENT_LIST();
			
	    }

		result = 0;
    }
    else{
		result = -1;
    }

    snprintf(sta_logout_html_buf, 255, "{\"resultCode\":%d,\"data\":\"\"}", result);

    log_info(__LOG_DEBUG__, "sta_logout_html_buf: %s", sta_logout_html_buf);
	sprintf(fp_file_path, "%s%s", LIGHTTPD_ROOT_DOC, LOGOUT_PAGE);

	if ((fh = fopen(fp_file_path, "w+")) != NULL) {
		fwrite(sta_logout_html_buf, strlen(sta_logout_html_buf), 1, fh);
		fclose(fh);
		http_send_to_local(r, LOGOUT_PAGE, "Redirect to local page: sta_logout.html");
	}
	else
	{
		log_info(LOG_WARNING, "fopen file: %s fail. can't redirect to sta_logout.html", fp_file_path);
	}

	if (NULL != client_mac)
	{
		free(client_mac);
	}
    return;
}
/*END: add@2014.11.25 终端下线*/


void send_http_page(request *r, const char *title, const char* message)
{
    s_config	*config = config_get_config();
    char *buffer;
    struct stat stat_info;
    int fd;
    ssize_t written;

    fd=open(config->htmlmsgfile, O_RDONLY);
    if (fd==-1) {
        log_info(__LOG_FAILED__, "Failed to open HTML message file %s: %s", config->htmlmsgfile, strerror(errno));
        return;
    }

    if (fstat(fd, &stat_info)==-1) {
        log_info(__LOG_FAILED__, "Failed to stat HTML message file: %s", strerror(errno));
        close(fd);
        return;
    }

    buffer=(char*)safe_malloc(stat_info.st_size+1);
    written=read(fd, buffer, stat_info.st_size);
    if (written==-1) {
        log_info(__LOG_FAILED__, "Failed to read HTML message file: %s", strerror(errno));
        free(buffer);
        close(fd);
        return;
    }
    close(fd);

    buffer[written]=0;
    httpdAddVariable(r, "title", title);
    httpdAddVariable(r, "message", message);
    //httpdAddVariable(r, "nodeID", config->gw_id);
    httpdOutput(r, buffer);
    free(buffer);
}

void send_apple_success(request *r)
{
    char *buf;
    struct stat stat_info;
    int fd;
    ssize_t written;

    fd=open(APPLE_SUCCESS, O_RDONLY);
    if (fd==-1) {
        log_info(__LOG_FAILED__, "Failed to open HTML message file "APPLE_SUCCESS": %s", strerror(errno));
        return;
    }

    if (fstat(fd, &stat_info)==-1) {
        log_info(__LOG_FAILED__, "Failed to stat HTML message file: %s", strerror(errno));
        close(fd);
        return;
    }

    buf=(char*)safe_malloc(stat_info.st_size+1);
    written=read(fd, buf, stat_info.st_size);
    if (written==-1) {
        log_info(__LOG_FAILED__, "Failed to read HTML message file: %s", strerror(errno));
        free(buf);
        close(fd);
        return;
    }
    log_info(__LOG_DEBUG__, "Send HTML appple-success.html suceess");
    close(fd);
    httpdOutput(r, buf);
    free(buf);
    return;
}

t_authcode auth_server_request(request *r,
                               const char *request_type, 
                               const char *ip, 
                               const char *mac, 
                               const char *token, 
                               unsigned long long int incoming, 
                               unsigned long long int outgoing)
{
	char buf[MAX_BUF];
    char *safe_token;
	t_auth_serv	*auth_server = NULL;
	auth_server = get_auth_server();
	s_config *config = config_get_config();
    r->authSock = connect_auth_server();
    r->authSock_timeout = get_uptime();
	if (r->authSock == SOCKET_INVALID) {
		/* Could not connect to any auth server */
		return (AUTH_ERROR);
	}

	/**
	 * TODO: XXX change the PHP so we can harmonize stage as request_type
	 * everywhere.
	 */
	memset(buf, 0, sizeof(buf));
    safe_token = httpdUrlEncode(token);
	snprintf(buf, (sizeof(buf) - 1),
		"GET %s%sstage=%s&ip=%s&mac=%s&token=%s&incoming=%llu&outgoing=%llu&gw_id=%s&version=1.0 HTTP/1.0\r\n"
		"User-Agent: WiFiDog %s\r\n"
		"Host: %s\r\n"
		"\r\n",
		auth_server->authserv_path,
		auth_server->authserv_auth_script_path_fragment,
		request_type,
		ip,
		mac,
		safe_token,
		incoming,
		outgoing,
        config->gw_id,
		VERSION,
		auth_server->authserv_hostname
	);
    free(safe_token);

	log_info(__LOG_DEBUG__, "Sending HTTP request to auth server: [%s]\n", buf);
	/*send login auth page to auth server:
	  *[GET /cmps/admin.php/api/auth/?stage=login&ip=172.16.222.41&mac=48:74:6e:36:17:2d&
	  *token=93230d4f9111b12fd92543cf133618926dd9371e&incoming=0&outgoing=0&gw_id=xb-x86
	  *&version=1.0 HTTP/1.0#015#012User-Agent: WiFiDog 1.0.0#015#012Host: www.login-wifi.com
	  *#015#012#015#012]
	  */
	send(r->authSock, buf, strlen(buf), 0);
	r->freeFlag = NOT_FREE;
    return(AUTH_ALLOWED);
}

/*2015-3-12 ali_auth send ailitoken to cloud*/
t_authcode ali_auth_server_request(request *r,
                               const char *request_type, 
                               const char *ip, 
                               const char *mac, 
                                char *token, 
                               unsigned long long int incoming, 
                               unsigned long long int outgoing)
{
	char buf[MAX_BUF];
    char *safe_token;
	t_auth_serv	*auth_server = NULL;
	auth_server = get_auth_server();
	s_config *config = config_get_config();
    r->authSock = connect_auth_server();
    r->authSock_timeout = get_uptime();
	if (r->authSock == SOCKET_INVALID) {
		/* Could not connect to any auth server */
		return (AUTH_ERROR);
	}

	/**
	 * TODO: XXX change the PHP so we can harmonize stage as request_type
	 * everywhere.
	 */
	memset(buf, 0, sizeof(buf));
    safe_token = httpdUrlEncode(token);
	snprintf(buf, (sizeof(buf) - 1),
		"GET %s%sstage=%s&ip=%s&mac=%s&alitoken=%s&incoming=%llu&outgoing=%llu&gw_id=%s&version=1.0 HTTP/1.0\r\n"
		"User-Agent: WiFiDog %s\r\n"
		"Host: %s\r\n"
		"\r\n",
		auth_server->authserv_path,
		auth_server->authserv_auth_script_path_fragment,
		request_type,
		ip,
		mac,
		safe_token,
		incoming,
		outgoing,
        config->gw_id,
		VERSION,
		auth_server->authserv_hostname
	);
    free(safe_token);

	log_info(__LOG_DEBUG__, "Sending HTTP request to auth server: [%s]\n", buf);
	/*send login auth page to auth server:
	  *[GET /cmps/admin.php/api/auth/?stage=login&ip=172.16.222.41&mac=48:74:6e:36:17:2d&
	  *token=93230d4f9111b12fd92543cf133618926dd9371e&incoming=0&outgoing=0&gw_id=xb-x86
	  *&version=1.0 HTTP/1.0#015#012User-Agent: WiFiDog 1.0.0#015#012Host: www.login-wifi.com
	  *#015#012#015#012]
	  */
	send(r->authSock, buf, strlen(buf), 0);
	r->freeFlag = NOT_FREE;
    return(AUTH_ALLOWED);
}






#define IS_NEW_ALIPAY ((strstr(r->readBuf, "Aliauth:")) && (strstr(r->readBuf,"open_id1")))
void _authenticate_client(request *r, char *ip, char *mac, char *other_ip, char *other_mac)
{
	t_client	*client;
	char *tmp;
	char *urlFragment = NULL;
	s_config	*config = NULL;
	t_auth_serv	*auth_server = NULL;
	t_authresponse auth_response;
	long now = get_uptime();
	long auth_period = 0;

	if((NULL == ip) || (NULL == mac))
		return;

	log_info(__LOG_MSG__, "_authenticate_client ip: %s mac: %s", ip, mac);

	if ((tmp = strstr(r->readBuf, "Auth: "))) 
	{
		if (sscanf(tmp, "Auth: %d", (int *)&auth_response.authcode) == 1) 
		{
			log_info(__LOG_DEBUG__, "Auth server returned authentication code %d", auth_response.authcode);
			if(AUTH_ALLOWED == auth_response.authcode)
			{
				if((tmp = strstr(tmp, "\n")))
				{
					auth_response.ext = safe_strdup(tmp+1);
				}
			}
		} 
		else 
		{
			log_info(__LOG_ERROR__, "Auth server did not return expected authentication code");
			return;
		}
	}
	else 
	{
		return;
	}

	LOCK_CLIENT_LIST();
	/* can't trust the client to still exist after n seconds have passed */
	client = client_list_find(ip, mac);
	
	if (client == NULL) {
		log_info(__LOG_ERROR__, "Could not find client node for %s (%s)", ip, mac);
		goto end;
	}
	log_info(__LOG_DEBUG__, " ip: %s  mac: %s  token: %s ", client->ip, client->mac, client->token);
	/*end: add@2015.02.11 for wechat connect wifi*/

	if((other_ip != NULL) && (client->other_ip != NULL))
		strcpy(other_ip, client->other_ip);
	if((other_mac != NULL) && (client->other_mac != NULL))
		strcpy(other_mac, client->other_mac);
	
	/* add@2014.8.6 user_access is crashed*/
	if ((tmp = strstr(r->readBuf, "User_access is crashed")))
	{
		log_info(__LOG_ERROR__, "User_access is crashed then set freeauth");
		sys_log_set(WIFIDOG_SERVER_LOG_FILE, "Set NF free-auth by authenticate_client User_access crashed.");
		fw_set_free_auth();
		goto end;
	}

	/* Prepare some variables we'll need below */
	config = config_get_config();
	auth_server = get_auth_server();

	switch(auth_response.authcode) 
	{
    	case AUTH_ERROR:
    		/* Error talking to central server */
    		log_info(__LOG_ERROR__, "Got %d from central server authenticating token %s from %s at %s", auth_response.authcode, client->token, client->ip, client->mac);
            if(IS_NEW_ALIPAY)
    		{
    		    goto end;
    		}
    		send_http_page(r, "Error!", "Error: We did not get a valid answer from the central server");
    		break;
    	case AUTH_DENIED:
    		/* Central server said invalid token */
    		log_info(__LOG_MSG__, "Got DENIED from central server authenticating token %s from %s at %s - redirecting them to denied message", client->token, client->ip, client->mac);

            if(IS_NEW_ALIPAY)
    		{
    		    goto end;
    		}
    		
    		safe_asprintf(&urlFragment, "%smessage=%s",
    			auth_server->authserv_msg_script_path_fragment,
    			GATEWAY_MESSAGE_DENIED
    		);

             http_send_redirect_to_auth(r, urlFragment, "Redirect to denied message");
    		free(urlFragment);
    		break;
        case AUTH_VALIDATION:
    		/* They just got validated for X minutes to check their email */
    		log_info(__LOG_MSG__, "Got VALIDATION from central server authenticating token %s from %s at %s"
    				"- adding to firewall and redirecting them to activate message", client->token,
    				client->ip, client->mac);
    		client->fw_connection_state = FW_MARK_PROBATION;
			sys_log_set(WIFIDOG_USR_LOG_FILE, "nf-ALLOW AUTH_VALIDATION %s %s", client->ip, client->mac);
			client_list_auth_update(client);
    		nf_fw_access(FW_ACCESS_ALLOW, client->ip, client->mac);
    		if(config->qos_enable)
                fw_qos_add(client);
    		safe_asprintf(&urlFragment, "%smessage=%s",
    			auth_server->authserv_msg_script_path_fragment,
    			GATEWAY_MESSAGE_ACTIVATE_ACCOUNT
    		);
			/*modified@2014.8.19 do nothing when if user authenticated*/
    		//http_send_redirect_to_auth(r, urlFragment, "Redirect to activate message");

			free(urlFragment);
    	    break;
        case AUTH_ALLOWED:
    		/* Logged in successfully as a regular account */
    		log_info(__LOG_MSG__, "Got ALLOWED from central server authenticating token %s from %s at %s - "
    				"adding to firewall and redirecting them to portal.", client->token, client->ip, client->mac);
    		if(auth_response.ext){
    			log_info(__LOG_MSG__, "EXT:[%s]", auth_response.ext);
    			sscanf(auth_response.ext, "%u %u %llu %llu %lu %lu", &client->bw_up, &client->bw_down, &client->max_bw_up, &client->max_bw_down, &client->idletimeout, &client->forcetimeout);
    			free(auth_response.ext);
    		}
    		client->fw_connection_state = FW_MARK_KNOWN;
    		client->login_time = now;
			/*online_time_last_update_time*/
			client->online_time_last_update_time = now;
			auth_period = now - client->auth_period;
			client->auth_period = auth_period;
			sys_log_set(WIFIDOG_USR_LOG_FILE, "nf-ALLOW AUTH_ALLOWED %s %s", client->ip, client->mac);
			client_list_auth_update(client);
    		nf_fw_access(FW_ACCESS_ALLOW, client->ip, client->mac);
    		if(config->qos_enable)
                fw_qos_add(client);
                if(IS_NEW_ALIPAY)
                {
                   // log_info(__LOG_MSG__, "******aliauth allowd****");
                    goto end;
                }
			/*begin: modified @2015.01.28 for wechat connect wifi*/
			if (auth_mode_support_wxl()){
				safe_asprintf(&urlFragment, "%s://%s/?res=success", "http", WXL_REDIRECT_URL);
				http_send_redirect(r, urlFragment, "Redirect to portal");
				free(urlFragment);
			}
			else{
				safe_asprintf(&urlFragment, "%sgw_id=%s&token=%s",
						auth_server->authserv_portal_script_path_fragment,
						config->gw_id,
						client->token);
				http_send_redirect_to_auth(r, urlFragment, "Redirect to portal");
				free(urlFragment);
			}
			/*end: modified @2015.01.28 for wechat connect wifi*/
			break;
        case AUTH_VALIDATION_FAILED:
    		 /* Client had X minutes to validate account by email and didn't = too late */
    		log_info(__LOG_MSG__, "Got VALIDATION_FAILED from central server authenticating token %s from %s at %s "
    				"- redirecting them to failed_validation message", client->token, client->ip, client->mac);


            if(IS_NEW_ALIPAY)
    		{
    		    goto end;
    		}
    		safe_asprintf(&urlFragment, "%smessage=%s",
    			auth_server->authserv_msg_script_path_fragment,
    			GATEWAY_MESSAGE_ACCOUNT_VALIDATION_FAILED
    		);

    		http_send_redirect_to_auth(r, urlFragment, "Redirect to failed validation message");
    		free(urlFragment);
    	    break;
        default:
    		log_info(__LOG_ERROR__, "I don't know what the validation code %d means for token %s from %s at %s - sending error message", auth_response.authcode, client->token, client->ip, client->mac);
            if(IS_NEW_ALIPAY)
    		{
    		    goto end;
    		}
    		send_http_page(r, "Internal Error", "We can not validate your request at this time");
    	    break;
	}

end:
	UNLOCK_CLIENT_LIST();
	return;
}




void _alipay_auth_json_parse_add(cJSON *pstJsFromServer,cJSON *pstJsToClient,const char *str)
{
    if((NULL == pstJsFromServer) || (NULL == pstJsToClient) || (NULL == str))
    {
        log_info(__LOG_DEBUG__, "parm is NULL");
        return;
    }
    cJSON *pstJsTemp;
    pstJsTemp = NULL;

    pstJsTemp = cJSON_GetObjectItem(pstJsFromServer,str);
    if(NULL != pstJsTemp)
    {
        //sys_log_set(WIFIDOG_USR_LOG_FILE, "parse from serverjson,name:%s   value:%s ",str,pstJsTemp->valuestring);
        cJSON_AddStringToObject(pstJsToClient,str,pstJsTemp->valuestring);
    }
    else
    {
       // sys_log_set(WIFIDOG_USR_LOG_FILE,"parse %s from serverjs not found",str);
        cJSON_AddStringToObject(pstJsToClient,str,NULL);
    }

    return;
    
}

/*parse json data from cloud*/
int aliauth_http_parse_json_data(request *r,char **jsdata)
{
    char *jsondata =NULL;
    char *data = NULL;
    cJSON *pstJsFromServer = NULL,*pstJsToClient = NULL;


    if(NULL == r)
    {
        log_info(__LOG_DEBUG__, "request is NULL");
        return -1;
    }
    
    if(NULL == (data = strstr(r->readBuf, "Aliauth:")))/*json数据不存在*/
    {
        log_info(__LOG_DEBUG__, "not found aliauth json data");
        return -1;
    }

    /*获得json数据*/
    data += strlen("Aliauth:");
  
    safe_asprintf(&jsondata,"%s",data);
    if(NULL == jsondata)
    {
        return -1;
    }
    log_info(__LOG_DEBUG__, "jsfromserver data:[%s]",jsondata);
    
    pstJsFromServer = cJSON_Parse(jsondata);
    if(NULL == pstJsFromServer)
    {

        log_info(__LOG_DEBUG__, "jsfromserver parse error ");
        return -1;
    }
    
    pstJsToClient = cJSON_CreateObject();
    if(NULL == pstJsToClient)
    {
        log_info(__LOG_DEBUG__, "jstoclient creat error ");
        return -1;
    }
    
    _alipay_auth_json_parse_add(pstJsFromServer,pstJsToClient,"code");
    _alipay_auth_json_parse_add(pstJsFromServer,pstJsToClient,"msg");
    _alipay_auth_json_parse_add(pstJsFromServer,pstJsToClient,"open_id1");
    _alipay_auth_json_parse_add(pstJsFromServer,pstJsToClient,"open_id2");
    _alipay_auth_json_parse_add(pstJsFromServer,pstJsToClient,"auth_url");
  
    cJSON_AddStringToObject(pstJsToClient,"method","post");
    *jsdata=cJSON_PrintUnformatted(pstJsToClient);  
    if(NULL == *jsdata)
    {
        return -1;
    }
   // sys_log_set(WIFIDOG_USR_LOG_FILE,"alipay jsData to alipay: %s ",*jsdata);
    cJSON_Delete(pstJsFromServer);
    cJSON_Delete(pstJsToClient); 
    
    return 0;
}



void http_alipay_sendto_client(request *r,char *jsdata)
{
    httpdSetResponse(r, "200 OK");/**/
    httpdSetContentType(r, "text/plain");
    sys_log_set(WIFIDOG_USR_LOG_FILE,"send to alipay client jsdata:%s",jsdata);
    httpdSendContentBody(r,jsdata,strlen(jsdata));
}


extern en_auth_mode auth_mode[AUTH_MODE_MAX];
void authenticate_client(request *r)
{
	char ip[64] = {0};
	char mac[64] = {0};
	char other_ip[64] = {0};
	char other_mac[64] = {0};

        char *jsondata = NULL,aliflag = 0;
	log_info(__LOG_MSG__, "[AUTHENTICATION-STEP-[authenticate-client]]. r->clientAddr: %s", r->clientAddr);
        if(((int)AUTH_MODE_SUPPORT == (int)auth_mode[AUTH_MODE_ALIPAY]) && (strstr(r->readBuf, "Aliauth:")) && (strstr(r->readBuf, "open_id1")))
        {
            //log_info(__LOG_MSG__, "*****alipay*******");
            if(0 != aliauth_http_parse_json_data(r,&jsondata))
            {
                goto end;
            }
            //sys_log_set(WIFIDOG_USR_LOG_FILE,"data:%s",jsondata);
            aliflag = 1;
        }


	
	/*begin: add@2015.02.11 for wechat connect wifi*/


	/** @解析认证状态 */
#if 0 /*根据消息中的ip、mac提取，目前不启用*/
	if ((tmp = strstr(r->readBuf, "Ip=")))
	{										 
		char *tmp2;
		char auth_value[128];
		strncpy(auth_value, tmp, 128);
		tmp = &auth_value[0];

		tmp += strlen("Ip=");
		if ((tmp2 = strstr(tmp, "&")))
			*tmp2 = '\0';
		else{
			log_info(__LOG_ERROR__, "Auth server did not return expected authentication code");
			goto end;			
		}
		strncpy(ip, tmp, 64);

		if((tmp = strstr(tmp2 + 1, "Mac=")) == NULL){
			log_info(__LOG_ERROR__, "Auth server did not return expected authentication code");
			goto end;			
		}
			
		tmp += strlen("Mac=");
		if ((tmp2 = strstr(tmp, "&")))
			*tmp2 = '\0';
		else{
			log_info(__LOG_ERROR__, "Auth server did not return expected authentication code");
			goto end;			
		}
		strncpy(mac, tmp, 64);
	}
	else
#endif
	/*根据现有流程，收到一条消息就会关闭连接*/
	/*所以仅根据client结构提取other_mac和other_ip*/

	{   /*提取请求终端的ip、mac*/
		strcpy(ip, r->clientAddr);
		strcpy(mac, r->clientMac);
		_authenticate_client(r, ip, mac, other_ip, other_mac);

		/*提取请求终端关联的other ip/mac(例如微信连PC的ip/mac)*/
		if((strlen(other_ip) > 0) && (strlen(other_mac) > 0))
			_authenticate_client(r, other_ip, other_mac, NULL, NULL);
	}

            if(1 == aliflag)
            {
                http_alipay_sendto_client(r,jsondata);
                free(jsondata);
            }
     
end:    
        r->freeFlag = ON_FREE;
        httpdEndRequest(r);

}


void http_send_to_local(request *r, char *html_file, char *text)
{
    /*
	** Setup for a standard response
	*/
	strcpy(r->response.headers,
		"Server: Hughes Technologies Embedded Server\n"); 
	strcpy(r->response.contentType, "text/html");
	strcpy(r->response.response,"200 Output Follows\n");
	r->response.headersSent = 0;
    
    http_send_redirect_to_local(r, html_file, text);
    httpdEndRequest(r);
    
    return;
}

