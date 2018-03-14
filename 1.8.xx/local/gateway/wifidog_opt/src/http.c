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
#include <arpa/inet.h>

#include "httpd.h"

#include "conf.h"
#include "util.h"
#include "firewall.h"
#include "config.h"
#include "auth_server.h"
#include "client_list.h"
#include "common.h"
#include "log.h"
#include "http.h"
#include "cloud_platform.h"
/*begin: add@2015.01.27 for wechat connect wifi*/
#include "redirect_portal.h"
#include "b64.h"
/*end: add@2015.01.27 for wechat connect wifi*/

#include "client_ap.h"
#include "cJSON.h"
#include "client_list.h"
#include "client_rdl.h"
#include "gateway.h"
#include "lib_pa.h"

/*add@2014.8.18*/
#ifdef GW_X86_DEL
extern int pthread_hash_by_ip(char *ip);
#else
#endif
extern int get_sta_wifi_cnnt(char *str_mac, char *ap_mac, char *ssid,int uiSsidLen);
extern int get_sta_ap_mac(char *str_mac, char *ap_mac);
extern int user_acl_add_by_mac(unsigned char *mac, unsigned long duration, int type);
int wifidog_send_onoff_client_to_audit(fw_access_t eType, const char *pStaMac,t_client *pstClient);

unsigned int g_stat_8061_access = 0;
unsigned int g_stat_8062_access = 0;
unsigned int g_stat_html_redirect = 0;
unsigned int g_stat_307_redirect = 0;

extern int g_iIosAutoPortal;
extern int g_iIosRedirectMode;
extern int g_iUcwebRedirectMode;

extern int set_sta_auth_status(char *ip, char *mac, int auth, char *method,char *token, int status);

#ifdef HAVE_NETSNIFF

#define WIFIDOG_USER_LOGIN_MSG		1	
#define WIFIDOG_USER_LOGOUT_MSG		2

int g_status_macro_hot;/*macro hot 线程运行情况*/

int g_macro_hot_thread_close =0;/*1:需要关闭线程 */


extern int wifidog_set_thread_name(EN_WIFIDOG_THREAD_INDEX enThreadIndex);

/*begin:2015-9-8 网间漫游*/
int macro_hot_client_handle(char *ip,roam_client *pRoamClient);



struct netsniff_msg_wifidog
{
	char msg_type;
	char user_mac[18]; 	/*format: XX:XX:XX:XX:XX:XX*/
	char user_ip[40]; 	/*format: ipv4: A.B.C.D ipv6: XXXX:XXXX:XXXX:XXXX:XXXX:XXXX:XXXX:XXXX*/
	char info[256];
};

extern int g_netsniff_fd;

void http_alipay_redirect_callback(httpd *webserver,request *r);
void _alipay_auth_json_parse_add(cJSON *pstJsFromServer,cJSON *pstJsToClient,const char *str);
int aliauth_http_parse_json_data(request *r,char **jsdata);
void http_alipay_sendto_client(request *r,char *jsdata);
void http_jinjiang_sendto_client(request *r,char *jsdata);
void http_sta_info_sendto_client(request *r,char *jsdata);
void http_send_200OK_to_client(request *r,char *data);

int http_send_sta_info(request *r);


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
extern pthread_mutex_t roam_client_list_mutex;

extern char *g_pstAppleSuccess;
extern char *g_pstDisIosPortalAuto;

extern int g_client_access_num;
extern en_auth_mode auth_mode[AUTH_MODE_MAX];

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


int http_get_redirect_url(httpd *webserver, request *r, char *rdurl, int len)
{
	char	tmp_url[MAX_BUF],
			*url;
    char proto[10];
    char host[1024];
    char uri[1024];
    char *ptUri = uri;
    int port;
    char *oriUrl;
	s_config	*config = config_get_config();

	t_auth_serv	*auth_server = get_auth_server();
  
	log_info(__LOG_DEBUG__, "[AUTHENTICATION-STEP-[http_get_redirect_url]]. r->clientAddr: %s", r->clientAddr);
	
	memset(tmp_url, 0, sizeof(tmp_url));
    
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

	if (!is_online())
	{
		/* The internet connection is down at the moment  - apologize and do not redirect anywhere */
		http_send_redirect_to_local(r, DEFAULT_OFFLINEFILE, "Redirect to offline");
		log_info(__LOG_DEBUG__, "Sent %s an apology since I am not online - no point sending them to auth server", r->clientAddr);
		goto no_urlset;
	}
	else /* Re-direct them to auth server */
	{
        log_info(__LOG_DEBUG__, "requestHost: %s", r->request.host);
		if('\0' == r->clientMac[0]){
			log_info(__LOG_ERROR__, "NULL == client_mac, arp find mac fail, addr:%s", r->clientAddr);
			goto no_urlset;
		}

		char *urlFragment;
		char ucVersion =1;
		char *protocol = NULL;
		int port = 80;
		char ap_mac[32] = "";
		char ssid[128] = "";

        log_info(__LOG_DEBUG__, "client_list_find is Null r->clientAddr=%s", r->clientAddr);
		safe_asprintf(&urlFragment, "%sgw_address=%s&gw_port=%d&gw_id=%s&ip=%s&mac=%s&version=%d&url=%s",
			auth_server->authserv_login_script_path_fragment,
			config->gw_address,
			config->gw_port, 
			config->gw_id,
			r->clientAddr,
			r->clientMac,
			ucVersion,
			url);
		log_info(__LOG_DEBUG__, "Captured %s requesting [%s] and re-directing them to login page", r->clientAddr, url);
		/*http redirect packet:
		  *http://www.login-wifi.com:80/cmps/admin.php/api/login/?gw_address=172.16.0.1&gw_port=8060&gw_id=xb-x86
		  *&ip=172.16.222.41&mac=48:74:6e:36:17:2d&url=http%3A//www.thinkdifferent.us/BUrWQgVk3FZ/r2yLUfFV4NO.html
		  *&apmac=ac:34:cb:c0:93:ed
		  */

		get_sta_wifi_cnnt(r->clientMac, ap_mac, ssid,sizeof(ssid));

		if (auth_server->authserv_use_ssl) {
			protocol = "https";
			port = auth_server->authserv_ssl_port;
		} else {
			protocol = "http";
			port = auth_server->authserv_http_port;
		}

		snprintf(rdurl, len, "%s://%s:%d%s%s%s%s&wechat2wifi=1",
			protocol,
			auth_server->authserv_hostname,
			port,
			auth_server->authserv_path,
			urlFragment,
			ap_mac,
			ssid
		);

		free(urlFragment);
	}

	if(NULL != url)
		free(url);
	return 0;

no_urlset:
	if(NULL != url)
		free(url);
	return -1;
}
/*begin: add@2015.01.27 for wechat connect wifi*/

static int check_sta_already_auth(request *r, int id)
{
	t_client *client = NULL;
	static long last_online_time = 0;
	static long last_login_time = 0;

	LOCK_CLIENT_LIST();
	client = client_list_find(r->clientAddr, r->clientMac);
    if (NULL != client)
	{
		++ client->access_cnt;
		/*终端已认证说明nf_portal与wifidog不同步*/
		if (FW_MARK_KNOWN == client->fw_connection_state)
		{
			/*避免攻击性访问刷屏*/
			if((client->online_time != last_online_time) || (client->login_time != last_login_time)){
			#ifndef BOARD_9341_21
				debug_log_set(WIFIDOG_DEBUG_USR_LOG_FILE, id, "nf-OTHER: Rah %s %s ot(%ld)-lt(%ld)-it(%ld)-ft(%ld)-cnt(%d)", 
					r->clientAddr, r->clientMac, client->online_time, client->login_time,
					client->idletimeout, client->forcetimeout, client->access_cnt);
			#endif
				last_online_time = client->online_time;
				last_login_time = client->login_time;
			}

			/*首先直接放行*/
			nf_fw_access2(FW_ACCESS_ALLOW, r->clientMac);

			/*然后对于非ios终端，直接重定向到原来访问的url*/
			/*既对于用户无感知，又避免到云平台重复认证*/
			if(NULL == strstr(g_pstAppleSuccess, r->request.host)){
				UNLOCK_CLIENT_LIST();
				http_send_redirect(r, r->request.host, "Redirect to host");
				httpdEndRequest(r);
				return 1;
			}
			else{/*对于ios终端，浏览器访问则重定向苹果官网(避免出现白屏success)*/
				if(NULL != strstr(r->request.userAgent, "Mozilla")){ /*浏览器访问 */
					UNLOCK_CLIENT_LIST();
                	http_send_redirect(r, "www.apple.com", "Redirect to Apple");
					httpdEndRequest(r);
					return 1;
				}
				else{/*对于ios终端，network访问则返回success页面*/
					UNLOCK_CLIENT_LIST();
					send_apple_success(r);
					httpdEndRequest(r);
					return 1;
				}
			}
		}
    }
	UNLOCK_CLIENT_LIST();

	return 0;
}


char *first_page_up = 
	"<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">\n"
	"<html>\n"
	"<head>\n"
	"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">\n"
	"<title>Loading ...</title>\n"
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
	int len;
	int loop = webserver->idx;
    unsigned char aucUserMac[6] = {0};
    WD_USER_IOS_ACCESS_T *pstuser = NULL;
	char *client_mac = NULL;
	WD_STA_T *pststa = NULL;
	s_config	*config = config_get_config();
	
    log_info(__LOG_DEBUG__, "r->request.host=[%s], r->request.userAgent=[%s], r->clientAddr:%s", 
							r->request.host, r->request.userAgent, r->clientAddr);

	++g_stat_8061_access;
	
	client_mac = arp_get(r->clientAddr);
	if (NULL == client_mac){
		log_info(__LOG_ERROR__, "arp_get(r->clientAddr) fail, ip: %s", r->clientAddr);
		log_info(__LOG_DEBUG__, "http_callback_redirect2local, clientAddr: %s, client_mac is NULL", r->clientAddr);
		r->clientMac[0] = '\0';
	}
	else{
		log_info(__LOG_DEBUG__, "http_callback_redirect2local, clientAddr: %s, client_mac: %s", 
			r->clientAddr, client_mac);
		strncpy(r->clientMac, client_mac, sizeof(r->clientMac));
		free(client_mac);
	}

	/*检测HTTP攻击*/
	if((config->uiHttpadswitch) && (g_client_access_num >= config->uiHttpckStaLimit)){
		unsigned char mac[6];
		if (r->clientMac[0] != '\0'){
			mac_converter(r->clientMac, mac);
			user_http_record_add_by_mac(mac, loop);
		}
	}
    
	/*获取当前终端信息*/
    if (strstr(r->request.host, "wifiopenapiauth.com"))
    {
		log_info(__LOG_DEBUG__, "from wifiopenapiauth.com then no first page");
		
		/*先放行，避免APP认证时可能不调用auth接口的问题*/
		if (r->clientMac[0] != '\0'){
			set_sta_auth_status(r->clientAddr, r->clientMac, 2, "cmd",NULL, FW_MARK_AUTH_ONGOING);
			if(check_sta_already_auth(r, 9061 + loop)) /*终端已认证了，停止弹portal*/
	        	return;
		}
		
		goto NO_FIRST_PAGE;
	}
	else if(strstr(r->request.host,"1.1.1.1") && strstr(r->readBuf,"appauth?UID="))
	{/*jucheng app_auth  直接重定向到平台*/
	    sys_log_set(WIFIDOG_APP_AUTH,"app_auth:r->buf:%s",r->readBuf);
	    goto NO_FIRST_PAGE;
	}
	else if(strstr(r->request.host,"mezone.gateway"))
	{
		  // sys_log_set(WIFIDOG_REDIRECT_FILE,"request host %s","mezone.gateway");
		   log_info(__LOG_DEBUG__, "request host %s","mezone.gateway");
		   http_send_sta_info(r);
		   return;
	}

	if (r->clientMac[0] != '\0'){
	   if(check_sta_already_auth(r, 9061 + loop)) /*终端已认证了，停止弹portal*/
		   return;
	}

	if(SERVER_MODE_THIRD_PARTY == config_get_config()->server_mode)
	{
		http_send_thirdparty_redirect(webserver, r);
		return;
	}

    /*add@2014.10.13 for specified handle if userAgent is ucweb or oupeng*/
    if (strstr(r->request.userAgent, "UC"))
    {
		log_info(__LOG_DEBUG__, "userAgent is ucweb then goto no first page");
		if(g_iUcwebRedirectMode != 1)
        	goto NO_FIRST_PAGE;
    }
    else if(strstr(r->request.userAgent, "MicroMessenger") && strstr(r->readBuf,"ulWxNum="))
    {/*针对微信连不能识别firstpage*/
        log_info(__LOG_DEBUG__, "MicroMessenger ulWxNum= goto no first page");
        goto NO_FIRST_PAGE;
    }
	

	/*IOS 自动弹Portal处理*/
	/*一般步骤(简称为1/2/3步骤): 
	1. IOS发出NetworkSupport报文，网关回本地network，引导IOS尽快进入步骤2
	2. IOS发出浏览器类型报文，网关将其重定向到Portal，完成自动弹Portal
	3. IOS发出NetworkSupport报文，网关回success，引导IOS 3G切WIFI
	注意:以上流程是一个整体，网关须保证状态记录完整性*/
    if(NULL != strstr(g_pstAppleSuccess, r->request.host))
    {
    	if(strmac_to_mac(r->clientMac, aucUserMac) < 0)
    	{
    		log_info(__LOG_DEBUG__, "[wbr]  %s is invalid client_mac\n", r->clientMac);	
            return;
    	}
        pstuser = user_ios_access_get_by_mac(loop, aucUserMac, CHECK_USER_INIT);
        if(NULL == pstuser)
        {
    		log_info(__LOG_ERROR__, "MAC: %s can not find ios user\n", r->clientMac);	
            return;
        }
		log_info(__LOG_DEBUG__, "[wbr-ap]  %s check state is %02X\n", r->clientMac,pstuser->check);

		/*浏览器类型的访问，重定向到Portal*/
		if(NULL != strstr(r->request.userAgent, "Mozilla"))
        {
			++pstuser->m_times;
			pstuser->bitmap = (pstuser->bitmap << 1) + 0;
            pstuser->check = CHECK_MOZILLA;
        }
		
		/*对于useragent为NetworkSupport的检测报文考虑返回success页面*/
        if(NULL != strstr(r->request.userAgent, "NetworkSupport"))
        {
			++pstuser->n_times;
			pstuser->bitmap = (pstuser->bitmap << 1) + 1;
            //如果终端只弹network，超时8s后回success
            if(CHECK_INIT == pstuser->check &&
                TRUE == user_timeout_check(&pstuser->time,g_ios_network_access_timeout_time))
            {
                send_apple_success(r);
                log_info(__LOG_DEBUG__, "[wbr-ap] request.userAgent=[%s] network out of time send_apple_success", r->request.userAgent);   
                return;
            }
            
            if((CHECK_MOZILLA == pstuser->check) &&
                (TRUE == user_timeout_check(&pstuser->time,g_ios_access_timeout_time)))
            {
                send_apple_success(r);
                log_info(__LOG_DEBUG__, "[wbr-ap] request.userAgent=[%s] send_apple_success", r->request.userAgent);	
				return;
            }
			/*如果之前已经弹出Portal，则返回success页面*/
            else if(CHECK_PORTAL == pstuser->check)
            {
                log_info(__LOG_DEBUG__, "[wbr-ap]--pstuser->duration:%ld",pstuser->duration);	
				if((pststa = get_sta_by_mac(r->clientMac)) == NULL) /*不能得到终端关联信息，可能是第三方AP*/
                {
                    if(TRUE == user_timeout_check(&pstuser->init_checktime, pstuser->duration))
                    {
                        firstnetwork_init_ioshlist(pstuser);
                        send_apple_network(r);
                        log_info(__LOG_DEBUG__, "[wbr-ap] init_checktime");	
                        return;
                    }
                    else
                    {                    
                        send_apple_success(r);
                        log_info(__LOG_DEBUG__, "[wbr-ap] init_checktime.tv_sec%ld init_checktime.tv_usec/1000 %ld",pstuser->init_checktime.tv_sec,pstuser->init_checktime.tv_usec/1000);	
        				return;
                    }
                }
                else
                {
                    free(pststa);
                    send_apple_success(r);
                    log_info(__LOG_DEBUG__, "[wbr-ap] request.userAgent=[%s] send_apple_success", r->request.userAgent);	
    				return;
                }
            }
            /*第一次访问改为本地静态页面*/
            else
            {
                send_apple_network(r);
                log_info(__LOG_DEBUG__, "[wbr-ap] request.userAgent=[%s] send_apple_network", r->request.userAgent);	
                return;
            }            
        }
        log_info(__LOG_DEBUG__, "[WBR] request.userAgent=[%s] NO_FIRST_PAGE", r->request.userAgent);

		if(g_iIosRedirectMode != 1)
        	goto NO_FIRST_PAGE;
		else
			pstuser->check = CHECK_PORTAL; /*first_page直接重定向，则相当于弹Portal了*/
    }

	r->request.query[512] = '\0';
    /*first_page*/	
	strcpy(g_first_page[loop], first_page_up);
	len = strlen(g_first_page[loop]);

	if (strstr(r->request.userAgent, "Mozilla") == NULL){
		/*user agent不带Mozilla则可能是一些APP的HTTP访问*/
		/*因为此类访问很可能被first page挡住，属于无效访问，*/
		/*所以firstpage的跳转语句，填写相对简单的8062跳转，节省生成first page的处理量*/
		/*少数浏览器的HTTP访问也不带Mozilla，则将接受8062端口的307服务*/
		snprintf(g_first_page[loop] + len, (MAX_BUF - 1 - len), 
		 	"http://%s:8062/rdaccess?oriUrl=http://%s%s%s%s",
		 	config->gw_address,
	        r->request.host,
	        r->request.path,
	        r->request.query[0] ? "?" : "",
	        r->request.query);
	}
	else{
		/*user agent带Mozilla则有可能是浏览器访问，此时通过http_get_redirect_url*/
		/*函数生成Portal URL，写入firstpage，然后通过firstpage直接跳转到Portal页面*/
		++g_stat_html_redirect;
		if((http_get_redirect_url(webserver, r, g_first_page[loop] + len, (MAX_BUF - 1 - len))) < 0)
			return;
	}

	len = strlen(g_first_page[loop]);
	strncpy(g_first_page[loop] + len, first_page_down, MAX_BUF-len-1);
    
	log_info(LOG_DEBUG, "client(ip=%s) redirect to first page", r->clientAddr);
	
	httpdOutput(r, g_first_page[loop], HTTP_OUTPUT_FLAG_FIRST_PAGE);
	httpdEndRequest(r);

	return;
/*begin: add@2014.10.13 no first page handle*/
NO_FIRST_PAGE:
	http_callback_404(webserver, r, 0);	
    return;
/*end: add@2014.10.13 no first page handle*/

}

/** The 404 handler is also responsible for redirecting to the auth server */
void http_callback_404(httpd *webserver, request *r, int check_auth)
{
	char	tmp_url[MAX_BUF],
			*url;
    char proto[10];
    char host[1024];
    char uri[1024];
    char *ptUri = uri;
    int port;
    int iLoop=0;
    unsigned char aucUserMac[6]={0};
    WD_USER_IOS_ACCESS_T *pstuser=NULL;
    char *oriUrl;
	s_config	*config = config_get_config();

	/* BEGIN:fengjing add 2014-09-28 */
	/* 增加本地检测重定向事宜 */
	if(strcmp(r->clientAddr, config->gw_address) == 0)
	{
		http_send_200OK_to_client(r, "404 Check OK");
		httpdEndRequest(r);
		return;
	}
	/* END:fengjing add 2014-09-28 */
	t_auth_serv	*auth_server = get_auth_server();

	log_info(__LOG_DEBUG__, "[AUTHENTICATION-STEP-[http-callback-404]]. r->clientAddr: %s", r->clientAddr);
	
	memset(tmp_url, 0, sizeof(tmp_url));
    
    if (strstr(r->request.path, "rdaccess")){
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
	if(strstr(g_pstAppleSuccess, r->request.host)){
		log_info(__LOG_DEBUG__, "[wbr-ap]request.host:%s", r->request.host);
		/*统一改为www.apple.com，避免老用户不弹portal机制下
		最后被弹到其他url造成白屏success*/
		strncpy(r->request.host, APPLE, sizeof(APPLE));  
		/*ios自动portal在path,query较长时会解析失败，且path和query再此没有作用，不填*/
		snprintf(tmp_url, (sizeof(tmp_url) - 1), "http://%s",
							r->request.host); 
	}
	else{
		snprintf(tmp_url, (sizeof(tmp_url) - 1), "http://%s%s%s%s",
							r->request.host,
							r->request.path,
							r->request.query[0] ? "?" : "",
							r->request.query);
	}
	url = httpdUrlEncode(tmp_url);

	if (!is_online())
	{
		/* The internet connection is down at the moment  - apologize and do not redirect anywhere */
		http_send_redirect_to_local(r, DEFAULT_OFFLINEFILE, "Redirect to offline");
		log_info(__LOG_DEBUG__, "Sent %s an apology since I am not online - no point sending them to auth server", r->clientAddr);
	}
	else /* Re-direct them to auth server */
	{
        log_info(__LOG_DEBUG__, "requestHost: %s", r->request.host);
		if (r->clientMac[0] != '\0'){
			if(1 == check_auth){
				if(check_sta_already_auth(r, 8062)){ /*终端已认证了，停止弹portal*/
					if(NULL != url)
						free(url);
					return;
				}
			}
		}
		else{
			log_info(__LOG_ERROR__, "NULL == client_mac, arp find mac fail, addr:%s", r->clientAddr);
			if(NULL != url)
				free(url);
			return;
		}

		char *urlFragment;
		char ucVersion =1;
        log_info(__LOG_DEBUG__, "client_list_find is Null r->clientAddr=%s", r->clientAddr);
		safe_asprintf(&urlFragment, "%sgw_address=%s&gw_port=%d&gw_id=%s&ip=%s&mac=%s&version=%d&url=%s",
			auth_server->authserv_login_script_path_fragment,
			config->gw_address,
			config->gw_port, 
			config->gw_id,
			r->clientAddr,
			r->clientMac,
			ucVersion,
			url);
		log_info(__LOG_DEBUG__, "Captured %s requesting [%s] and re-directing them to login page", r->clientAddr, url);
		/*http redirect packet:
		  *http://www.login-wifi.com:80/cmps/admin.php/api/login/?gw_address=172.16.0.1&gw_port=8060&gw_id=xb-x86
		  *&ip=172.16.222.41&mac=48:74:6e:36:17:2d&url=http%3A//www.thinkdifferent.us/BUrWQgVk3FZ/r2yLUfFV4NO.html
		  *&apmac=ac:34:cb:c0:93:ed
		  */
		++g_stat_307_redirect;
		http_send_redirect_to_auth(r, urlFragment, "Redirect to login page");
        /*标记IOS用户是否弹出PORTAL*/
        if(NULL != strstr(r->request.userAgent,"Mozilla"))
        {
        	if(strmac_to_mac(r->clientMac, aucUserMac) < 0)
        	{
        		log_info(__LOG_DEBUG__, "[404 wbr]  %s is invalid mac\n", r->clientMac);	
        		return;
        	}
            for(iLoop = 0; iLoop < FIRST_PAGE_TASK_NUM; iLoop++)
            {
                pstuser = user_ios_access_get_by_mac(iLoop,aucUserMac,CHECK_USER_GET);
                if(NULL != pstuser)
                {
                    pstuser->check = CHECK_PORTAL;
        		    log_info(__LOG_DEBUG__, "[404 wbr]  request.userAgent[%s]  CHECK_PORTAL\n", r->request.userAgent);	
                    gettimeofday(&pstuser->init_checktime, NULL);
                }
            }
        }
		free(urlFragment);
	}

	if(NULL != url)
		free(url);
}

void http_callback_404_8062(httpd *webserver, request *r)
{
	char *client_mac = NULL;

	client_mac = arp_get(r->clientAddr);
	if (NULL == client_mac){
		log_info(__LOG_ERROR__, "arp_get(r->clientAddr) fail, ip: %s", r->clientAddr);
		log_info(__LOG_DEBUG__, "http_callback_404_8062, clientAddr: %s, client_mac is NULL", r->clientAddr);
		r->clientMac[0] = '\0';
	}
	else{
		log_info(__LOG_DEBUG__, "http_callback_404_8062, clientAddr: %s, client_mac: %s", 
			r->clientAddr, client_mac);
		strncpy(r->clientMac, client_mac, sizeof(r->clientMac));
		free(client_mac);
	}

	++g_stat_8062_access;
	http_callback_404(webserver, r, 1);
}


/*begin: add@2015.01.27 for wechat connect wifi*/
int http_wxl_auth_encrypt(char *in, int len, 
								  char *out, int *outlen)
{
	char *c = out;
	int cnt = 0, tmpcnt = 0;
	base64_encodestate s;
	
	/* initialise the decoder state */
	base64_init_encodestate(&s);
	/* decode the input data */
	cnt = base64_encode_block(in, len, c, &s);
	c += cnt;
	tmpcnt = base64_encode_blockend(c, &s);
	cnt += tmpcnt;
	c += tmpcnt;
	*c = '\0';
	*outlen = cnt;
	return 0;
}

void http_wxl_redirect_callback(httpd *webserver, request *r)
{
	char *message = NULL;
	char *header = NULL;
	char *response = NULL;
	char acTmpBuf[128] = {0};
	char acOutBuf[256] = {0};
	char acEndBuf[256] = {0};
	int iOutLen = 0;
	s_config	*config = config_get_config();
	//t_client *client = NULL;
	char * client_mac = NULL;
	char *pt1, *pt2;
	
	log_info(__LOG_DEBUG__, "[WXL-STEP6-[http_wxl_redirect_callback]] begin. r->clientAddr: %s", r->clientAddr);

	if(!auth_mode_support_wxl()){
		log_info(__LOG_DEBUG__, "http_wxl_redirect_callback buf wxl not support, return.");
		return;
	}
	
	client_mac = arp_get(r->clientAddr);
	if (NULL == client_mac){
		log_info(__LOG_ERROR__, "wxl-blacklist: arp_get(r->clientAddr) fail, ip: %s", r->clientAddr);
		log_info(__LOG_DEBUG__, "clientAddr: %s, client_mac is NULL", r->clientAddr);
		r->clientMac[0] = '\0';
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
		/* Re-direct them to wechat client */
		/*拼接字符串:1.	将手机MAC地址XX:XX:XX:XX:XX:XX、
		  * 网关 MAC地址YY:YY:YY:YY:YY:YY和网关IP W.W.W.W 网关port的合并字串
		  * "XX:XX:XX:XX:XX:XX|YY:YY:YY:YY:YY:YY|W.W.W.W|xxxx" */
		if(1){ /*apmac使用终端实际连接的AP MAC，如果无法获得，则使用网关MAC*/
			char apmac[32] = {0};
			if(r->clientMac[0] != '\0')
				get_sta_ap_mac(r->clientMac, apmac);
			sprintf(acTmpBuf, "%s|%s|%s|%d", r->clientMac, apmac, config->gw_address, config->gw_port);
		}
		else /*apmac使用网关MAC*/
			sprintf(acTmpBuf, "%s|%s|%s|%d", r->clientMac, config->wan_mac, config->gw_address, config->gw_port);

		if(r->clientMac[0] != '\0')
			log_info(__LOG_DEBUG__, "wxl-blacklist: auth-orig: %s", acTmpBuf);
		http_wxl_auth_encrypt(acTmpBuf, strlen(acTmpBuf), acOutBuf, &iOutLen);
		if(r->clientMac[0] != '\0')
			log_info(__LOG_DEBUG__, "wxl-blacklist: auth-encrypt: %s", acOutBuf);

		/*将= 替换为o0o0o*/
		pt1 = pt2 = &acOutBuf[0];
		while((pt1 = strstr(pt2, "=")) != NULL){
			*pt1 = '\0';
			if(pt1 > pt2){
				if((strlen(acEndBuf) + strlen(pt2)) < sizeof(acEndBuf))
					strcat(acEndBuf, pt2);
			}
			if((strlen(acEndBuf) + strlen("o0o0o")) < sizeof(acEndBuf))
				strcat(acEndBuf, "o0o0o");
			pt2 = pt1 + 1;

			if('\0' == *pt2)
				break;
		}
		if(*pt2 != '\0')
			strcat(acEndBuf, pt2);

		if(r->clientMac[0] != '\0')
			log_info(__LOG_DEBUG__, "wxl-blacklist: auth-end-encrypt: %s", acEndBuf);

		char *url;
		safe_asprintf(&url, "http://%s?auth=%s", config->auth_servers->authserv_hostname, acEndBuf);
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

void http_80_redirect2boa_callback(httpd *webserver, request *r)
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
	
	log_info(__LOG_DEBUG__, "[WXL-STEP6-[http_wxl_redirect2boa_callback]] begin. r->clientAddr: %s r->request.host = %s", 
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


void http_jinjiang_auth_callback(httpd *webserver, request *r)
{
	char *jsondata = NULL;

	if(jinjiang_auth_http_parse_json_data(r, &jsondata) == 0)
		http_jinjiang_sendto_client(r,jsondata);

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
		log_info(__LOG_DEBUG__, "Status page requested, forcing authentication");
		httpdForceAuthenticate(r, config->httpdrealm);
		return;
	}

	//status = get_status_text();
	safe_asprintf(&buf, "<pre>%s</pre>", status);
	send_http_page(r, "WEB auth state", buf);
	free(buf);
	free(status);
}


/** @brief Convenience function to redirect the web browser to the auth server
 * @param r The request 
 * @param urlFragment The end of the auth server URL to redirect to (the part after path)
 * @param text The text to include in the redirect header ant the mnual redirect title */
void http_send_redirect_to_auth(request *r, char *urlFragment, char *text)
{
	char *protocol = NULL;
	int port = 80;
	char ap_mac[32] = "";
	char ssid[128] = "";
	t_auth_serv	*auth_server = get_auth_server();
	
	log_info(__LOG_DEBUG__, "[AUTHENTICATION-STEP-[send-redirect-to-auth]]. r->clientAddr: %s", r->clientAddr);

	get_sta_wifi_cnnt(r->clientMac, ap_mac, ssid,sizeof(ssid));

	if (auth_server->authserv_use_ssl) {
		protocol = "https";
		port = auth_server->authserv_ssl_port;
	} else {
		protocol = "http";
		port = auth_server->authserv_http_port;
	}
			    		
	char *url = NULL;
	safe_asprintf(&url, "%s://%s:%d%s%s%s%s&wechat2wifi=1",
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

/*当前终端信息获取接口*/
int http_send_sta_info(request *r)
{
    if(NULL == r)
    {
       // sys_log_set(WIFIDOG_REDIRECT_FILE,"client is null");
        log_info(__LOG_ERROR__,"client is null");
        return -1;
    }
    WD_STA_T *pststa = NULL;
    t_client *pstClient =NULL;
    char acAuthStat = 0;
    s_config *config = NULL;

    char *jsondata =NULL;
    cJSON *pstJsToClient = NULL;

    config = config_get_config();

   /*检查g_sta_Hlist表获得终端的apmac,ssid*/
   //sys_log_set(WIFIDOG_REDIRECT_FILE,"to find sta_mac:%s",r->clientMac);
   
    pststa = get_sta_by_mac(r->clientMac);
    if(NULL == pststa)
    {
        log_info(__LOG_DEBUG__, "can not find sta in g_sta_Hlist");
        //sys_log_set(WIFIDOG_REDIRECT_FILE,"not find sta in g_sta_Hlist");
        return -1;
    }
    //sys_log_set(WIFIDOG_REDIRECT_FILE,"find sta_mac:"MAC_FMT"  ap_mac:"MAC_FMT" ssid:%s", 
    //MAC_ARG(pststa->mac),MAC_ARG(pststa->ap_mac),pststa->ssid);

    /*检查client表获得认证状态*/
    pstClient = client_list_find_by_mac(r->clientMac);
    if(NULL == pstClient)
    {
        log_info(__LOG_DEBUG__, "can not find client in g_client_head ,set Authstat is 0");
       // sys_log_set(WIFIDOG_REDIRECT_FILE,"not find client in g_client_head");
        acAuthStat = 0;
    }
    else
    {
        if(FW_MARK_KNOWN == pstClient->fw_connection_state)
        {
            acAuthStat = 1;/*online*/
        }
        else
        {
            acAuthStat = 0;/*offLine*/
        }
    }
    
    pstJsToClient = cJSON_CreateObject();
    if(NULL == pstJsToClient)
    {
        log_info(__LOG_DEBUG__, "jstoclient creat error ");
		if(pststa != NULL)
			free(pststa);
        return -1;
    }
    char aucData[218];
    memset(aucData,0,sizeof(aucData)/sizeof(aucData[0]));
    cJSON_AddStringToObject(pstJsToClient,"gw_id",config->gw_id);
    cJSON_AddStringToObject(pstJsToClient,"gw_mac",config->wan_mac);
    sprintf(aucData,MAC_FMT,MAC_ARG(pststa->mac));
    cJSON_AddStringToObject(pstJsToClient,"mac",aucData);
    memset(aucData,0,sizeof(aucData)/sizeof(aucData[0]));
    sprintf(aucData,"%d",acAuthStat);
    cJSON_AddStringToObject(pstJsToClient,"can_online",aucData);

    memset(aucData,0,sizeof(aucData)/sizeof(aucData[0]));
    sprintf(aucData,MAC_FMT,MAC_ARG(pststa->ap_mac));
    cJSON_AddStringToObject(pstJsToClient,"ap_mac",aucData);
    cJSON_AddStringToObject(pstJsToClient,"ssid",pststa->ssid);

    jsondata=cJSON_PrintUnformatted(pstJsToClient);
    cJSON_Delete(pstJsToClient); 
    if(NULL == jsondata)
    {
		if(pststa != NULL)
			free(pststa);
        return -1;
    }
    //sys_log_set(WIFIDOG_REDIRECT_FILE,"get sta info: %s ",jsondata);
    

    http_sta_info_sendto_client(r,jsondata);
 //   http_alipay_sendto_client(r,jsondata);
    free(jsondata);
    if(pststa != NULL)
		free(pststa);
    return 0;
}


		
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


/*begin: for audit 2015-9-28*/
int http_get_client_auth_info(CLIENT_AUTH_INFO_T *pstAuthInfo,char *value)
{
    char aucInfo[256],*pt=NULL,aucUser[32]={0},aucType[8]={0};
    
    memset(aucInfo,0,sizeof(aucInfo)/sizeof(aucInfo[0]));
    
    if(NULL == pstAuthInfo || NULL == value)
    {
        log_info(__LOG_ERROR__, "param is NULL , return;");
        return -1;
    }
    sys_log_set(WIFIDOG_STAINFO_AUDIT,"http_get_client_auth_info before auth_info:%s",value);
    strncpy(aucInfo,value,sizeof(aucInfo)/sizeof(aucInfo[0]));
    pt = aucInfo;

    if(NULL == (pt =strstr(aucInfo,",")))
    {
        sys_log_set(WIFIDOG_STAINFO_AUDIT,"authinfo format is error");
        return -1;
    }
    *pt = '\0';
    strncpy(aucUser,aucInfo,32);
    strncpy(aucType,pt+1,8);
    
    strncpy(pstAuthInfo->aucUserCount,aucUser,32);
    pstAuthInfo->uiAuthType = atoi(&aucType[0]);

    sys_log_set(WIFIDOG_STAINFO_AUDIT,"authinfo userCount:%s  type:%d",pstAuthInfo->aucUserCount,pstAuthInfo->uiAuthType);
   

    return 0;
}

/*begin: for audit 2015-9-28*/




/*begin: add@2015.02.11 for wechat connect wifi*/
#define HTTP_TOKEN_OTHER_MAC "other_mac="
#define HTTP_TOKEN_OTHER_TOKEN "other_token="
int http_callback_auth_client_add(request *r, char *ip, char *mac, char *token)
{
	t_client *client;
	int fw_connection_state = FW_MARK_AUTH_ONGOING;
	
	if (NULL == mac || NULL == token){
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
	
	log_info(__LOG_DEBUG__, "http_callback_auth_client_add mac %s token %s",mac, token);

    /*begin:@for audit 获取认证信息 2015-9-28*/
    CLIENT_AUTH_INFO_T stAuthInfo;
    ADD_CLIENT_OTHERINFO_T stAddClientOtherInfo;
    memset(&stAddClientOtherInfo,0,sizeof(ADD_CLIENT_OTHERINFO_T));
    memset(&stAuthInfo,0,sizeof(CLIENT_AUTH_INFO_T));
    httpVar *authinfo = httpdGetVariableByName(r, "authinfo");
    if(NULL != authinfo)
    {
        http_get_client_auth_info(&stAuthInfo,authinfo->value);
        strncpy(stAddClientOtherInfo.stAuthInfo.aucUserCount,stAuthInfo.aucUserCount,32);
        stAddClientOtherInfo.stAuthInfo.uiAuthType = stAuthInfo.uiAuthType;
        log_info(__LOG_DEBUG__, "get authInfo UserCount:%s   AuthType:%d", stAddClientOtherInfo.stAuthInfo.aucUserCount,
stAddClientOtherInfo.stAuthInfo.uiAuthType);
    }
    else
    {
        log_info(__LOG_ERROR__,"http_callback_auth_client_add authinfo is NULL");
        sys_log_set(WIFIDOG_STAINFO_AUDIT,"http_callback_auth_client_add authinfo is NULL");
    }
    /*begin:@for audit 获取认证信息 2015-9-28*/
	
	LOCK_CLIENT_LIST();
	if ((client = client_list_find_by_mac(mac)) == NULL)
    {
	    /** 判断是否超过最大接入用户数 */
        if(g_client_access_num >= config_get_config()->max_client)
        {
            http_send_redirect_to_local(r, DEFAULT_MAXUSERFILE, "Redirect to max users");
            UNLOCK_CLIENT_LIST();
			if(method != NULL)
				free(method);
            return -1;
        }
		if((config_get_config()->dev_max_client > 0) &&
			(g_client_access_num >= config_get_config()->dev_max_client))
		{
            http_send_redirect_to_local(r, DEFAULT_MAXUSERFILE, "Redirect to max users");
            UNLOCK_CLIENT_LIST();
			if(method != NULL)
				free(method);
            return -1;
        }
		log_info(__LOG_DEBUG__, "New client for %s", mac);
		client_list_append(ip, mac, token,method,&stAddClientOtherInfo);
	}
	else
    { /*更新other ip和other mac*/
    	fw_connection_state = client->fw_connection_state;
		if (client->other_mac != NULL){
        	free(client->other_mac);
			client->other_mac = NULL;
		}
	    if (client->other_ip != NULL){
	        free(client->other_ip);
			client->other_ip = NULL;
	    }
	    /*update token*/
        if(NULL != client->token)
        {
	    	free(client->token);
	        client->token = NULL;
        }
        client->token = safe_strdup(token);

        if(NULL != client->method_type)
        {
            free(client->method_type);
            client->method_type = NULL;
        }
        if(NULL != method)
        {
            client->method_type = safe_strdup(method);
        }
	}
	UNLOCK_CLIENT_LIST();

	if(NULL != method)
	{
	    free(method);
	    method = NULL;
	}
	return fw_connection_state;
}

int http_callback_auth_client_add2(request *r, char *ip, char *mac, char *token,
												char *other_ip, char *other_mac)
{
	t_client *client;
	int fw_connection_state = FW_MARK_AUTH_ONGOING;
	
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

	log_info(__LOG_DEBUG__, "http_callback_auth_client_add ip %s mac %s token %s", ip, mac, token);

	/*begin:@for audit 获取认证信息 2015-9-28*/
	CLIENT_AUTH_INFO_T stAuthInfo;
	ADD_CLIENT_OTHERINFO_T stAddClientOtherInfo;
	memset(&stAddClientOtherInfo,0,sizeof(ADD_CLIENT_OTHERINFO_T));
	memset(&stAuthInfo,0,sizeof(CLIENT_AUTH_INFO_T));
	httpVar *authinfo = httpdGetVariableByName(r, "authinfo");
	if(NULL != authinfo)
	{
	    http_get_client_auth_info(&stAuthInfo,authinfo->value);
	    strncpy(stAddClientOtherInfo.stAuthInfo.aucUserCount,stAuthInfo.aucUserCount,32);
	    stAddClientOtherInfo.stAuthInfo.uiAuthType = stAuthInfo.uiAuthType;
	    log_info(__LOG_DEBUG__, "get authInfo UserCount:%s   AuthType:%d", stAddClientOtherInfo.stAuthInfo.aucUserCount,
	stAddClientOtherInfo.stAuthInfo.uiAuthType);
	}
	else
	{
	    sys_log_set(WIFIDOG_STAINFO_AUDIT,"http_callback_auth_client_add2 authinfo is NULL");
	}
	/*end:@for audit 获取认证信息 2015-9-28*/
	
	LOCK_CLIENT_LIST();
	if ((client = client_list_find_by_mac(mac)) == NULL) {
	    /** 判断是否超过最大接入用户数 */
        if(g_client_access_num >= config_get_config()->max_client)
        {
            http_send_redirect_to_local(r, DEFAULT_MAXUSERFILE, "Redirect to max users");
            UNLOCK_CLIENT_LIST();
			if(method != NULL)
				free(method);
            return -1;
        }
		if((config_get_config()->dev_max_client > 0) &&
			(g_client_access_num >= config_get_config()->dev_max_client))
		{
            http_send_redirect_to_local(r, DEFAULT_MAXUSERFILE, "Redirect to max users");
            UNLOCK_CLIENT_LIST();
			if(method != NULL)
				free(method);
            return -1;
        }
		log_info(__LOG_DEBUG__, "New client for %s", mac);
		client_list_append2(ip, mac, token, other_ip, other_mac,method,&stAddClientOtherInfo);
	}
	else{ /*更新other ip和other mac*/
		fw_connection_state = client->fw_connection_state;
		if (client->other_mac != NULL){
        	free(client->other_mac);
		}
	    if (client->other_ip != NULL){
	        free(client->other_ip);
	    }

		client->other_ip = safe_strdup(other_ip);
		client->other_mac = safe_strdup(other_mac);

        /*update token*/
        if(NULL != client->token)
        {
            free(client->token);
            client->token = NULL;
        }
        client->token = safe_strdup(token);

        if(NULL != client->method_type)
        {
            free(client->method_type);
            client->method_type = NULL;
        }
        if(NULL != method )
        {
            client->method_type = safe_strdup(method);
        }
	}
	UNLOCK_CLIENT_LIST();
    if(NULL != method)
	{
	    free(method);
	    method = NULL;
	}
	
	return fw_connection_state;
}
/*end: add@2015.9.17 强制关注公众号*/
httpVar * http_callback_auth_forced_attention(request *r)
{
	t_client	*client;
    httpVar * auth;

    if(!(auth = httpdGetVariableByName(r, "auth")))
    {
        log_info(__LOG_ERROR__, "[wbr-auth]auth  is NULL");        
        return NULL;
    }
    log_info(__LOG_ERROR__, "[wbr-auth]auth:%s", auth->value);
    client = client_list_find_by_mac(r->clientMac);
    if(NULL == client)
    {
        log_info(__LOG_ERROR__, "[wbr-auth]client  is NULL");        
        return NULL;
    }        
    if(NULL == client->token)
    {
        log_info(__LOG_ERROR__, "[wbr-auth]client->token  is NULL");        
        return NULL;
    }
    if(!(strncmp(auth->value,"1",1)))
    {
        nf_fw_access(FW_ACCESS_ALLOW, r->clientMac,client);
	    auth_server_request(r, REQUEST_TYPE_LOGIN, r->clientAddr, r->clientMac, client->token, 0, 0);           
        httpdOutput(r, "\n", HTTP_OUTPUT_FLAG_200_OK);
	    httpdEndRequest(r);
    }
    return auth;
}

/*end: add@2015.02.11 for wechat connect wifi*/
void  http_callback_auth(httpd *webserver, request *r)
{
	httpVar * token;
	httpVar * auth;
	httpVar * other_mac;
	httpVar * other_ip;
	httpVar * other_token;
	char	*mac=NULL;
	httpVar *logout = httpdGetVariableByName(r, "logout");
	httpVar * type;
	int ret = 0;
	
	log_info(__LOG_DEBUG__, "[AUTHENTICATION-STEP-[http-callback-auth]]. r->clientAddr: %s  buf:%s", r->clientAddr,\
	r->readBuf);

	/* 增加本地检测重定向事宜 */
	if(strcmp(r->clientAddr, config_get_config()->gw_address) == 0)
	{
		http_send_200OK_to_client(r, "Auth Check OK");
		httpdEndRequest(r);
		return;
	}

    /*获取client mac地址*/
    if (NULL == (mac = arp_get(r->clientAddr)))
    {
        /* We could not get their MAC address */
        log_info(__LOG_DEBUG__, "Failed to retrieve MAC address for ip %s", r->clientAddr);
        send_http_page(r, "WiFiDog Error", "Failed to retrieve your MAC address");
        return;	
    } 
    else
    {
         strncpy(r->clientMac, mac, HTTP_MAC_LEN);
         log_info(__LOG_DEBUG__, "get ip %s mac:%s\n", r->clientAddr,r->clientMac);
         free(mac);
         mac=NULL;
    }

    /*2015-10-29 app 认证放行begin*/
    type = httpdGetVariableByName(r,"type");
    token = httpdGetVariableByName(r, "token");
    if((NULL != type) &&(0 == strcmp(type->value,"appauth")))
    {
       if(NULL !=token)
       {
            set_sta_auth_status(r->clientAddr, r->clientMac, 2, "AppAuth",token->value,FW_MARK_AUTH_ONGOING);
            sys_log_set(WIFIDOG_APP_AUTH,"fangxing:r->buf:%s",r->readBuf);
        }
	 return;	
    }
    /*2015-10-29 app认证放行end*/

    if((token = httpdGetVariableByName(r, "alitoken")))/*2015-3-16 new alipay auth*/
    {
            http_alipay_token_check_request(webserver, r,token->value);        
    }
    else if ((token = httpdGetVariableByName(r, "token"))) 
    {
		
		/*begin: add@ 2015.02.11 for wechat connect wifi pc connect wifi*/
		/*token=sdfsdfsdfsdfsdfsdfsdf&other_mac=ac:8a:e5:b0:2a:12&other_token=sdfsdfsdfsdfqq*/
		log_info(__LOG_DEBUG__, "token->value = %s", token->value);
		char token_1[64] = {0};
		char token_2[64] = {0};
		char mac_2[64] = {0};
		char ip_2[64] = {0};
		int have_client2 = 0;

		strncpy(token_1, token->value, 64);
		log_info(__LOG_DEBUG__, "token_1 = %s", token_1);

		other_ip = httpdGetVariableByName(r, "other_ip");
		other_mac = httpdGetVariableByName(r, "other_mac");
		other_token = httpdGetVariableByName(r, "other_token");
		type = httpdGetVariableByName(r, "type");
		if (other_ip && other_mac && other_token)
		{			
			strncpy(ip_2, other_ip->value, 64);
			strncpy(mac_2, other_mac->value, 64);
			strncpy(token_2, other_token->value, 64);
			log_info(__LOG_DEBUG__, "token_2 = %s", token_2);
			log_info(__LOG_DEBUG__, "ip_2   = %s", ip_2);
			log_info(__LOG_DEBUG__, "mac_2   = %s", mac_2);
			
			have_client2 = 1;
		}
		else
		{
			have_client2 = 0;
                }
		/*end: add@ 2015.02.11 for wechat connect wifi pc connect wifi*/
		
		/* They supplied variable "token" */
		

		if (1 == have_client2){
			if(!strncmp(r->clientMac, mac_2, HTTP_MAC_LEN)) /*other mac 和设备mac一样*/
				have_client2 = 0;
		}

		if (1 == have_client2)
		{
			ret = http_callback_auth_client_add2(r, r->clientAddr, r->clientMac, token_1,
															ip_2, mac_2);
			if (ret < 0)
				return;

			ret = http_callback_auth_client_add(r, ip_2, mac_2, token_2);
			if (ret < 0)
                return;
		}
		else
		{
			ret = http_callback_auth_client_add(r, r->clientAddr, r->clientMac, token_1);
			if (ret < 0)
				return;	
		}
		if (NULL == logout) 
		{
			/*对于微信连，先回302，放行*/
			if(NULL != type)
            {
				char auth_type[64];
				strncpy(auth_type, type->value, 64);
				if (1 == auth_mode_support_wxl() && 0 == strncmp(auth_type, "wxwifi", 6))
                {
					char *urlFragment = NULL;
					safe_asprintf(&urlFragment, "%s://%s/?res=success", "http", 
						config_get_config()->auth_servers->authserv_hostname);
					/*safe_asprintf(&urlFragment, "%s://%s/?res=already", "http", WXL_REDIRECT_URL);*/
                    if(0 == strncmp(auth_type, "wxwifi2", 7))
                    {
                        http_send_200OK_to_client(r,"success");
                    }
                    else
                    {
					    http_send_redirect(r, urlFragment, "Redirect to portal");
                    }
					free(urlFragment);

                    if(0 == (auth = http_callback_auth_forced_attention(r)))
                    {
						nf_fw_access(FW_ACCESS_ALLOW, r->clientMac,NULL);
		                log_info(__LOG_ERROR__, "[wbr-auth]FW_ACCESS_ALLOW");
                    }
                    else
                    {                               
		                log_info(__LOG_ERROR__, "[wbr-auth]auth:%s",auth->value);
                        goto stay;
                    }
				}
			}

			/*已认证终端则直接放行，避免缓存直接访问8060导致再次认证*/
			if(FW_MARK_KNOWN == ret){
			#ifndef BOARD_9341_21
				debug_log_set(WIFIDOG_DEBUG_USR_LOG_FILE, 8060, "nf-OTHER: Rah-2 %s %s", r->clientAddr, r->clientMac);
			#endif
				nf_fw_access(FW_ACCESS_ALLOW, r->clientMac, NULL);
				http_send_redirect(r, "www.baidu.com", "Redirect to host");
				httpdEndRequest(r);
				goto stay;
			}
			
            auth_server_request(r, REQUEST_TYPE_LOGIN, r->clientAddr, r->clientMac, token_1, 0, 0);
			if (1 == have_client2)
            {
				auth_server_request(r, REQUEST_TYPE_LOGIN, ip_2, mac_2, token_2, 0, 0);
			}
stay:
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
    else 
    {
        if(!(auth = http_callback_auth_forced_attention(r)))
        {
		    /* They did not supply variable "token" */
		    send_http_page(r, "WiFiDog error", "Invalid token");
        }
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
    //char	*mac;
    //if (!(mac = arp_get(r->clientAddr))) /*can't get mac*/
    //{
      //  log_info(__LOG_DEBUG__, "Failed to retrieve MAC address for ip %s", r->clientAddr);
   // } 
    //else /*get mac*/
   // {
        //memcpy(r->clientMac, mac, strlen(mac));
       // r->clientMac[HTTP_MAC_LEN-1] = '\0';
        //free(mac);
        log_info(__LOG_DEBUG__, "  [ip %s  : MAC: %s]", r->clientAddr,r->clientMac);
		
        if (0 != http_callback_auth_client_add(r, r->clientAddr, r->clientMac, token))
        {
            return;
        }	

        /*send token to cloud  */
        ali_auth_server_request(r, REQUEST_TYPE_LOGIN, r->clientAddr, r->clientMac, token, 0, 0);
    //}
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
	
	log_info(__LOG_DEBUG__, "[AUTHENTICATION-STEP-[http_callback_roaming]]. r->clientAddr: %s", r->clientAddr);

#ifndef BOARD_9341_21
	sys_log_set(WIFIDOG_APICALL_LOG_FILE, "http_callback_roaming r->clientAddr:%s", r->clientAddr);
#endif	

	if ((token = httpdGetVariableByName(r, "token"))) {
		/* They supplied variable "token" */
		if (!(mac = arp_get(r->clientAddr))) {
			/* We could not get their MAC address */
			log_info(__LOG_DEBUG__, "Failed to retrieve MAC address for ip %s", r->clientAddr);
			send_http_page(r, "WiFiDog Error", "Failed to retrieve your MAC address");
		} else {
			/* We have their MAC address */
			memcpy(r->clientMac, mac, strlen(mac));
			r->clientMac[HTTP_MAC_LEN-1] = '\0';
			free(mac);
			LOCK_CLIENT_LIST();
			if ((client = client_list_find_by_mac(r->clientMac)) == NULL) {
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
				log_info(__LOG_DEBUG__, "New client for %s", r->clientAddr);
				ADD_CLIENT_OTHERINFO_T stOtherInfo;
                            memset(&stOtherInfo,0,sizeof(ADD_CLIENT_OTHERINFO_T));
                            strcpy(stOtherInfo.stAuthInfo.aucUserCount,"Roam_GW");
				client = client_list_append(r->clientAddr, r->clientMac, token->value,NULL,NULL);
			} 
 			else {
 			    /**@ 如果用户已经存在,则更新token */
				if(NULL != client->token)
 			    	free(client->token);
 			    client->token = safe_strdup(token->value);
				log_info(__LOG_DEBUG__, "Client for %s is already in the client list", client->ip);
			}

			if(client != NULL){
				/*漫游用户直接放行，并且重定向到原访问网址*/
				client->fw_connection_state = FW_MARK_KNOWN;
				client->login_time = now;
				client->online_time_last_update_time = now;
				client->auth_period  = now - client->auth_period;
				sys_log_set(WIFIDOG_USR_LOG_FILE, "nf-ALLOW: ROAMING %s %s", client->ip, client->mac);
				client_list_up_allqos();
				nf_fw_access(FW_ACCESS_ALLOW, client->mac,client);
				client->bw_up = config_get_config()->bw_up;
				client->bw_down = config_get_config()->bw_down;
				
				if(config_get_config()->qos_enable)
					fw_qos_add(client->mac, client->bw_up, client->bw_down);

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
			UNLOCK_CLIENT_LIST();
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
    char sta_mac_buf[1024] = {'\0'};
	
    log_info(__LOG_DEBUG__, "http callback sta get itsmac r->clientAddr:%s", r->clientAddr);
	char *client_mac = NULL;
	char actmp[32] = "";
    char acApMac[32] = "";
	char ssid[128] = "";

#ifndef BOARD_9341_21
	sys_log_set(WIFIDOG_APICALL_LOG_FILE, "http_callback_sta_get_itsmac r->clientAddr:%s", r->clientAddr);
#endif

	client_mac = arp_get(r->clientAddr);
    if (client_mac){
        if (0 == get_sta_wifi_cnnt(client_mac, actmp, ssid,sizeof(ssid)) ){
			sscanf(actmp, "&apmac=%s", acApMac);
        }
        else{
			strcpy(acApMac, "00:00:00:00:00:00");
        }
        
        snprintf(sta_mac_buf, 1023, "{\"resultCode\":%d,\"data\":\"{\\\"sta_mac\\\":\\\"%s\\\", \\\"ap_mac\\\":\\\"%s\\\"}\"}",
                               0, client_mac, acApMac);

        log_info(__LOG_DEBUG__, "sta_mac_buf: %s", sta_mac_buf);
    }
    else{
		snprintf(sta_mac_buf, 1023, "{\"resultCode\":%d}", 1);
    }

	if (NULL != client_mac)
	{
		free(client_mac);
	}

	httpdOutput(r, sta_mac_buf, HTTP_OUTPUT_FLAG_200_OK);
	httpdEndRequest(r);
	
    return;
}
/*END: add@2014.11.14 终端获取它的mac地址*/


void http_callback_sta_get_auth(httpd *webserver, request *r)
{
	t_client *client;
    char sta_auth_buf[1024] = {'\0'};
	
    log_info(__LOG_DEBUG__, "http callback sta get auth r->clientAddr:%s", r->clientAddr);
	char *client_mac = NULL;

#ifndef BOARD_9341_21
	sys_log_set(WIFIDOG_APICALL_LOG_FILE, "http_callback_sta_get_auth r->clientAddr:%s", r->clientAddr);
#endif
    
	client_mac = arp_get(r->clientAddr);
    if (client_mac){
		LOCK_CLIENT_LIST();
		client = client_list_find(r->clientAddr, client_mac);
		if (NULL != client)
		{
			snprintf(sta_auth_buf, 1023, "{\"resultCode\":0,\"data\":\"{\\\"auth\\\":\\\"%d\\\"}\"}",
					client->fw_connection_state);
		}
		else{
			snprintf(sta_auth_buf, 1023, "{\"resultCode\":0,\"data\":\"{\\\"auth\\\":\\\"0\\\"}\"}");
    	}
		UNLOCK_CLIENT_LIST();
    }
    else{
		snprintf(sta_auth_buf, 1023, "{\"resultCode\":0,\"data\":\"{\\\"auth\\\":\\\"0\\\"}\"}");
    }

	if (NULL != client_mac)
	{
		free(client_mac);
	}

	log_info(__LOG_DEBUG__, "sta_auth_buf: %s", sta_auth_buf);

	httpdOutput(r, sta_auth_buf, HTTP_OUTPUT_FLAG_200_OK);
	httpdEndRequest(r);
	
    return;
}



/*BEGIN: add@2014.12.25 终端下线*/
void http_callback_sta_logout(httpd *webserver, request *r)
{
    char sta_logout_buf[256] = {'\0'};
	int result = -1;
	char *client_mac = NULL;
	t_client *client;
	s_config *config = config_get_config();
	httpVar *val;
	unsigned char mac[6], mac2[6];
	
    log_info(__LOG_DEBUG__, "http callback sta logout r->clientAddr:%s", r->clientAddr);

#ifndef BOARD_9341_21
	sys_log_set(WIFIDOG_APICALL_LOG_FILE, "http_callback_sta_logout r->clientAddr:%s", r->clientAddr);
#endif

	client_mac = arp_get(r->clientAddr);
    if (client_mac)
	{
		val = httpdGetVariableByName(r, "mac");
		if(val != NULL)
		{
			mac_converter(val->value, mac);
			mac_converter(client_mac, mac2);
			if(!memcmp(mac, mac2, 6)) /*校验调用者mac和接口中mac是否相同*/
			{
				/*立即在nf_portal中下线*/
				sys_log_set(WIFIDOG_USR_LOG_FILE, "nf-DENY: app-sta-logout %s %s", r->clientAddr, client_mac);
			    nf_fw_access(FW_ACCESS_DENY, client_mac,NULL);
				
			    if (config->auth_servers != NULL) {
					memcpy(r->clientMac, client_mac, strlen(client_mac));
					r->clientMac[HTTP_MAC_LEN-1] = '\0';
					
					/* fengjing add 2015-01-07 */
					LOCK_CLIENT_LIST();
					if ((client = client_list_find_by_mac(r->clientMac)) != NULL){
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
		}

		free(client_mac);
    }

    snprintf(sta_logout_buf, 255, "{\"resultCode\":%d,\"data\":\"\"}", result);

    log_info(__LOG_DEBUG__, "sta_logout_buf: %s", sta_logout_buf);

	httpdOutput(r, sta_logout_buf, HTTP_OUTPUT_FLAG_200_OK);
	httpdEndRequest(r);

    return;
}
/*END: add@2014.11.25 终端下线*/

void http_callback_thirdpartyap(httpd *webserver, request *r)
{
	unsigned long duration = 30000; /*默认30秒*/
	httpVar *val;
    struct timeval time; 	  	
    WD_USER_IOS_ACCESS_T *pstuser = NULL;
    unsigned char aucUserMac[6] = {0};
	char *client_mac = NULL;
    int iLoop;

    log_info(__LOG_DEBUG__, "[wbr-ap]--http_callback_thirdpartyap");

#ifndef BOARD_9341_21
	sys_log_set(WIFIDOG_APICALL_LOG_FILE, "http_callback_thirdpartyap r->clientAddr:%s", r->clientAddr);
#endif
	
    gettimeofday(&time, NULL); 
    client_mac = arp_get(r->clientAddr);
	if(client_mac != NULL){
	    if(strmac_to_mac(client_mac, aucUserMac) < 0)
	    {
	    	log_info(__LOG_DEBUG__, "[wbr-ap]  %s is invalid client_mac", client_mac);
	        return;
	    }
		for(iLoop = 0; iLoop < FIRST_PAGE_TASK_NUM; iLoop++)
		{    
	        pstuser = user_ios_access_get_by_mac(iLoop, aucUserMac, CHECK_USER_GET);
	        if(NULL != pstuser)
	        {
	        	log_info(__LOG_DEBUG__, "[wbr-ap]http_callback_thirdpartyap r->clientAddr:%s", r->clientAddr);
	        	val = httpdGetVariableByName(r, "duration");
	        	if(val != NULL)
	        		duration = atoi(val->value) * 1000;
	            pstuser->duration       = duration;
	            pstuser->init_checktime = time;
	        }
	    }
	}
	httpdOutput(r, "\n", HTTP_OUTPUT_FLAG_200_OK);
	httpdEndRequest(r);

    return;
}

void http_callback_sta_access(httpd *webserver, request *r)
{
    char sta_white_buf[256] = {'\0'};
	int result = 0;
	char *client_mac = NULL;
	unsigned char mac[6];
	unsigned long duration = 30000; /*默认30秒*/
	httpVar *val;

	log_info(__LOG_DEBUG__, "http_callback_sta_access r->clientAddr:%s", r->clientAddr);
#ifndef BOARD_9341_21
	sys_log_set(WIFIDOG_APICALL_LOG_FILE, "http_callback_sta_access r->clientAddr:%s", r->clientAddr);
#endif

	val = httpdGetVariableByName(r, "duration");
	if(val != NULL)
		duration = atoi(val->value) * 1000;
	
	val = httpdGetVariableByName(r, "mac");
	if(val != NULL)
		mac_converter(val->value, mac);
	else{ /*获取终端自身mac*/
		client_mac = arp_get(r->clientAddr);
		if (NULL == client_mac){
			log_info(__LOG_ERROR__, "arp_get(r->clientAddr) fail, ip: %s", r->clientAddr);
			log_info(__LOG_DEBUG__, "http_callback_404_8062, clientAddr: %s, client_mac is NULL", r->clientAddr);
			result = -1;
		}
		else{
			log_info(__LOG_DEBUG__, "http_callback_sta_access, clientAddr: %s, client_mac: %s", 
				r->clientAddr, client_mac);
			mac_converter(client_mac, mac);
			free(client_mac);
		}
	}

	/*在nf_portal中设置临时白名单*/
	if(result != -1){
		log_info(__LOG_DEBUG__, "sta add temp whitelist %s %s", r->clientAddr, client_mac);
		user_acl_add_by_mac(mac, duration, USER_ACL_WHITE);	
	}

	snprintf(sta_white_buf, 255, "{\"resultCode\":%d,\"data\":\"\"}", result);
	log_info(__LOG_DEBUG__, "sta_white_buf: %s", sta_white_buf);
	
	httpdOutput(r, sta_white_buf, HTTP_OUTPUT_FLAG_200_OK);
	httpdEndRequest(r);

    return;
}


void send_http_page(request *r, const char *title, const char* message)
{
#if 0
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
    httpdOutput(r, buffer, HTTP_OUTPUT_FLAG_COMMON);
    free(buffer);
#else  /*不添加页面消息体*/
	httpdAddVariable(r, "title", title);
    httpdAddVariable(r, "message", message);
	httpdOutput(r, "", HTTP_OUTPUT_FLAG_COMMON);
#endif
}

void send_apple_network(request *r)
{
    char *buf;
    struct stat stat_info;
    int fd;
    ssize_t written;

    fd=open(APPLE_NETWORK, O_RDONLY);
    if (fd==-1) {
        log_info(__LOG_FAILED__, "Failed to open HTML message file "APPLE_NETWORK": %s", strerror(errno));
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
    log_info(__LOG_DEBUG__, "Send HTML appple-network.html suceess");
    close(fd);
    httpdOutput(r, buf,HTTP_OUTPUT_FLAG_COMMON);
    free(buf);

	httpdEndRequest(r);
    return;
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
    httpdOutput(r, buf, HTTP_OUTPUT_FLAG_APPLE);
    free(buf);

	httpdEndRequest(r);
    return;
}


/*
ssid 不需要时:调用函数赋值为""
token 不需要时:调用函数赋值为""
*/
t_authcode auth_server_request(request *r,
                               const char *request_type, 
                               const char *ip, 
                               const char *mac, 
                               const char *token, 
                               unsigned long long int incoming, 
                               unsigned long long int outgoing)
{

    if(NULL == r || NULL == request_type ||NULL ==  ip ||NULL==token)
    {
        log_info(__LOG_ERROR__, "param is NULL return\n");
        return (AUTH_ERROR);
    }


	char buf[MAX_BUF]={0};
	char aucSendAuthSta[32]={0};
	char aucToken[64]={0};
	t_auth_serv	*auth_server = NULL;
	auth_server = get_auth_server();
	s_config *config = config_get_config();
    r->authSock = connect_auth_server();
    r->authSock_timeout = get_uptime();
	if (r->authSock == SOCKET_INVALID) {
		if(r->clientSock != SOCKET_INVALID){
			close(r->clientSock);
			r->clientSock = SOCKET_INVALID;
		}
		/* Could not connect to any auth server */
		return (AUTH_ERROR);
	}

	/**
	 * TODO: XXX change the PHP so we can harmonize stage as request_type
	 * everywhere.
	 */
	 /*begin:@2015-8-19 add ap_man=  ap_ssid 字段*/
        char ap_mac[32] = "";
	 char ssid[128] = "";
	 get_sta_wifi_cnnt(r->clientMac, ap_mac, ssid,sizeof(ssid));
        /*end:@2015-8-19*/
	memset(buf, 0, sizeof(buf));


    /*begin:2015-8-26 for 网间漫游*/
    memset(aucSendAuthSta,0,sizeof(aucSendAuthSta));
    int uiFlagSet= 0;
    if(0 == strcmp(token,"") && 0 == strcmp(request_type,REQUEST_TYPE_LOGIN))
    {/*上报在线终端*/
        strcpy(aucSendAuthSta,"&free_auth=1");
        uiFlagSet = 1;
        
    }
    else
    {
        sprintf(aucToken,"&token=%s",token);
    }
    /*begin:2015-8-26 for 网间漫游*/

	
	snprintf(buf, (sizeof(buf) - 1),
		"GET %s%sstage=%s&ip=%s&mac=%s%s&incoming=%llu&outgoing=%llu&gw_id=%s%s%s%s&version=1.0 HTTP/1.0\r\n"
		"User-Agent: WiFiDog %s\r\n"
		"Host: %s\r\n"
		"\r\n",
		auth_server->authserv_path,
		auth_server->authserv_auth_script_path_fragment,
		request_type,
		ip,
		mac,
		aucToken,
		incoming,
		outgoing,
        config->gw_id,
                ap_mac,
                ssid,
                aucSendAuthSta,
		VERSION,
		auth_server->authserv_hostname
	);

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


/*logout 时ip地址可设置为"0.0.0.0"*/
t_authcode __auth_server_request_sta_login_logout(request *r,
                               const char *request_type, 
                               const char *ip, 
                               const char *mac, 
                               const char *token, 
                               const  char *pSsid, 
                               const char *pStrApMac)
{

    if(NULL == r || NULL == request_type ||NULL ==  ip ||NULL==token)
    {
        log_info(__LOG_ERROR__, "param is NULL return\n");
        return (AUTH_ERROR);
    }


    char buf[MAX_BUF];
    char aucSendAuthSta[32];
    char *safe_token;
    char aucFormat[256]={0};
    t_auth_serv	*auth_server = NULL;
    auth_server = get_auth_server();
    s_config *config = config_get_config();
    r->authSock = connect_auth_server();
    r->authSock_timeout = get_uptime();
    if (r->authSock == SOCKET_INVALID) 
    {
		if(r->clientSock != SOCKET_INVALID)
		{
			close(r->clientSock);
			r->clientSock = SOCKET_INVALID;
		}
		/* Could not connect to any auth server */
		return (AUTH_ERROR);
    }

    char ap_mac[32] = "";
    char ssid[128] = "";
    if(NULL != pSsid)
    {
        sprintf(ssid,"&ssid=%s",pSsid);
    }
    else
    {
        sprintf(ssid,"&ssid=");
    }
    if(NULL != pStrApMac)
    {
        sprintf(ap_mac,"&apmac=%s",pStrApMac);
    }
    else
    {
        sprintf(ap_mac,"%s,","&apmac=");
    }
    
    memset(buf, 0, sizeof(buf));
    
    
    memset(aucSendAuthSta,0,sizeof(aucSendAuthSta));
    if(0 == strcmp(token,"") && 0 == strcmp(request_type,REQUEST_TYPE_LOGIN))
    {/*上报在线终端*/
                
                strcpy(aucFormat,"GET %s%sstage=%s&ip=%s&mac=%s%s&incoming=0&outgoing=0&gw_id=%s%s%s&free_auth=1&version=1.0 HTTP/1.0\r\n"
		"User-Agent: WiFiDog %s\r\n"
		"Host: %s\r\n"
		"\r\n");
    }
    else
    {
            strcpy(aucFormat,"GET %s%sstage=%s&ip=%s&mac=%s&token=%s&incoming=0&outgoing=0&gw_id=%s%s%s&version=1.0 HTTP/1.0\r\n"
		"User-Agent: WiFiDog %s\r\n"
		"Host: %s\r\n"
		"\r\n");
    }
	
    safe_token = httpdUrlEncode(token);
	snprintf(buf, (sizeof(buf) - 1),aucFormat,
		auth_server->authserv_path,
		auth_server->authserv_auth_script_path_fragment,
		request_type,
		ip,
		mac,
		safe_token,
        config->gw_id,
                ap_mac,
                ssid,
		VERSION,
		auth_server->authserv_hostname
	);
    free(safe_token);
    
	log_info(__LOG_DEBUG__, "Sending HTTP request to auth server: [%s]\n", buf);
	sys_log_set(WIFIDOG_SSID_AUTH,"send onOff to server:%s",buf);
	/*send login auth page to auth server:
	  *[GET /cmps/admin.php/api/auth/?stage=login&ip=172.16.222.41&mac=48:74:6e:36:17:2d&
	  *token=93230d4f9111b12fd92543cf133618926dd9371e&incoming=0&outgoing=0&gw_id=xb-x86
	  *&version=1.0 HTTP/1.0#015#012User-Agent: WiFiDog 1.0.0#015#012Host: www.login-wifi.com
	  *#015#012#015#012]
	  */
	 // sys_log_set("/tmp/wd_ccs_auth.log","buf:%s",buf);
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
	if (r->authSock == SOCKET_INVALID) 
	{
		/* Could not connect to any auth server */
		if(SOCKET_INVALID == r->clientSock)
		{
		    close(r->clientSock);
                  r->clientSock = SOCKET_INVALID;
		}
		return (AUTH_ERROR);
	}

	/**
	 * TODO: XXX change the PHP so we can harmonize stage as request_type
	 * everywhere.
	 */
	 /*begin:@2015-8-19 add ap_man=  ap_ssid 字段*/
        char ap_mac[32] = "";
	 char ssid[128] = "";
	 get_sta_wifi_cnnt(r->clientMac, ap_mac, ssid,sizeof(ssid));
        /*end:@2015-8-19*/
	memset(buf, 0, sizeof(buf));
    safe_token = httpdUrlEncode(token);
	snprintf(buf, (sizeof(buf) - 1),
		"GET %s%sstage=%s&ip=%s&mac=%s&alitoken=%s&incoming=%llu&outgoing=%llu&gw_id=%s%s%s&version=1.0 HTTP/1.0\r\n"
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
               ap_mac,
               ssid,
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


void _authenticate_client_create_by_getuserstatus(request *r)
{
    char *token=NULL;
    t_client *client=NULL;
    
    token = http_parse_name_of_value(r->readBuf,"token");
	
    if(NULL == token)
    {
        log_info(__LOG_DEBUG__, "can not find <token> value in %s\n", r->readBuf);
        return;
    }
    if(NULL != strstr(r->readBuf,REQUEST_TYPE_GETUSER))
    {
    	LOCK_CLIENT_LIST();
        if(NULL == (client = client_list_find_by_mac(r->clientMac)))
        {
            client=client_list_append("0.0.0.0",r->clientMac,token,NULL,NULL);
    		log_info(__LOG_DEBUG__, "%02X,%02X,%p",r->authSock,r->clientSock,r);
            http_add_sta_to_server(client);
        }
    	UNLOCK_CLIENT_LIST();
    }
    else if(NULL != strstr(r->readBuf,REQUEST_TYPE_LOGIN))
    {
    
    	LOCK_CLIENT_LIST();
        if(NULL != (client = client_list_find_by_mac(r->clientMac)))
        {
            if(NULL != client->token)
            {
                free(client->token);
                client->token=NULL;
            }
            if(NULL == client->token)
            {
                client->token=safe_strdup(token);
        		log_info(__LOG_DEBUG__, "client->token:: %s ", client->token);
            }
        }
    	UNLOCK_CLIENT_LIST();
    }
    if(NULL == token)
    {
        free(token);
        token=NULL;
    }
    return;
}

int http_check_auth_info(request *pstRequest,char *pStrStaMac,char *pToken)
{
    //sys_log_set(WIFIDOG_SSID_AUTH,"***begin http_check_auth_info");
    if(NULL == pstRequest || NULL == pStrStaMac)
    {
        log_info(__LOG_ERROR__, "param is NULL,Return\n");
        return -1;
    }
    
    WD_STA_AUTH_INFO_T stNewStaAuthInfo,stOldStaAuthInfo;
    memset((char *)&stNewStaAuthInfo,0,sizeof(WD_STA_AUTH_INFO_T));
    memset((char *)&stOldStaAuthInfo,0,sizeof(WD_STA_AUTH_INFO_T));
    WD_UPDATE_AUTH_INFO_E eRet;
    s_config *pConf = config_get_config();
    char aucApStrMac[18] = {0};
    unsigned char aucApIntMac[6]={0};
    char *pAuthSsid= NULL,*pApMac= NULL;
    
    pAuthSsid = http_parse_name_of_value(pstRequest->readBuf,"ssid");
    pApMac = http_parse_name_of_value(pstRequest->readBuf,"apmac");
    if(NULL != pAuthSsid)
    {
        strncpy(stNewStaAuthInfo.aucLastAuthSsid,pAuthSsid,32);
        free(pAuthSsid);
    }
    if(NULL != pApMac)
    {
        strmac_to_mac(pApMac,aucApIntMac);
        memcpy(stNewStaAuthInfo.aucLastAuthApMac,aucApIntMac,6);
        free(pApMac);
    }
    if(pToken)
    {
        strcpy(stNewStaAuthInfo.aucToken,pToken);
    }
   // sys_log_set(WIFIDOG_SSID_AUTH,"uiSpanSsidAuthSwith=%d",pConf->uiSpanSsidAuthSwith);
    stNewStaAuthInfo.uiLastAuthStatus = 1;
    log_info(__LOG_DEBUG__,"update_authinfo staMac:%s ssid:%s token:%s uiSpanSsidAuthSwith=%d\n",pStrStaMac,stNewStaAuthInfo.aucLastAuthSsid,stNewStaAuthInfo.aucToken,\
    pConf->uiSpanSsidAuthSwith);
    eRet = sta_update_auth_info(pStrStaMac,&stNewStaAuthInfo,&stOldStaAuthInfo);
    //sys_log_set(WIFIDOG_SSID_AUTH,"<http_check_auth_info> SpanSsidAuthSwith=%d eRet=%d",pConf->uiSpanSsidAuthSwith,eRet);
    if(1 == pConf->uiSpanSsidAuthSwith)
    {
        sprintf(aucApStrMac,MAC_FMT,MAC_ARG(stOldStaAuthInfo.aucLastAuthApMac));
        if(WD_UPDATE_AUTH_INFO_ERR == eRet)
        {
            log_info(__LOG_ERROR__,"WD_UPDATE_AUTH_INFO_ERR,Return -1\n");
            return -1;
        }
        else if(WD_UPDATE_AUTH_INFO_DIFF_SSID == eRet && (1==stOldStaAuthInfo.uiLastAuthStatus))
        {
            http_sta_onoff_to_server(REQUEST_TYPE_LOGOUT,pStrStaMac,stOldStaAuthInfo.aucLastAuthSsid,aucApStrMac,stOldStaAuthInfo.aucToken,CLIENT_LOGOUT_SPAN_SSID,pstRequest);
            sys_log_set(WIFIDOG_SSID_AUTH,"<http_check_auth_info> Deny WD_UPDATE_AUTH_INFO_DIFF_SSID Sta:%s  LastAuthSsid:%s NowAuthSsid:%s",pStrStaMac,\
                stOldStaAuthInfo.aucLastAuthSsid,stNewStaAuthInfo.aucLastAuthSsid);
            return -1;
        }
        else if(WD_UPDATE_AUTH_INFO_SAME_SSID == eRet && (1==stOldStaAuthInfo.uiLastAuthStatus))
        {
            sys_log_set(WIFIDOG_SSID_AUTH,"<http_check_auth_info> SAME_SSID Down LastAuth Sta:%s  NowAuthSsid:%s ApMac:"MAC_FMT" Token:%s LastAuthSsid:%s",pStrStaMac,\
                stNewStaAuthInfo.aucLastAuthSsid,MAC_ARG(stNewStaAuthInfo.aucLastAuthApMac),stNewStaAuthInfo.aucToken,stOldStaAuthInfo.aucLastAuthSsid);
            http_sta_onoff_to_server(REQUEST_TYPE_LOGOUT,pStrStaMac,stOldStaAuthInfo.aucLastAuthSsid,aucApStrMac,stOldStaAuthInfo.aucToken,CLIENT_LOGOUT_SPAN_SSID,pstRequest);
        }
        
    }

    return 0;
}

#define IS_NEW_ALIPAY ((strstr(r->readBuf, "Aliauth:")) && (strstr(r->readBuf,"open_id1")))
void _authenticate_client(request *r, char *ip, char *mac, char *other_ip, char *other_mac)
{
	t_client	*client=NULL;
	char *tmp=NULL;
	char *stage=NULL;
	char *urlFragment = NULL;
	s_config	*config = NULL;
	t_auth_serv	*auth_server = NULL;
	t_authresponse auth_response={0};
	long now = get_uptime();
	long auth_period = 0;


        WD_STA_AUTH_INFO_T stNewStaAuthInfo,stOldStaAuthInfo;
        memset((char *)&stNewStaAuthInfo,0,sizeof(WD_STA_AUTH_INFO_T));
        memset((char *)&stOldStaAuthInfo,0,sizeof(WD_STA_AUTH_INFO_T));
        WD_UPDATE_AUTH_INFO_E eRet;
        s_config *pConf = config_get_config();
        char aucApStrMac[18] = {0};
        //unsigned char aucApIntMac[6]={0};
        //char *pAuthSsid= NULL,*pApMac= NULL;


    memset(&auth_response,0,sizeof(t_authresponse));
    
	if((NULL == ip) || (NULL == mac))
		return;

	memset(&auth_response, 0, sizeof(t_authresponse));

	log_info(__LOG_DEBUG__, "_authenticate_client ip: %s mac: %s", ip, mac);

	if ((tmp = strstr(r->readBuf, "Auth: "))) 
	{
		if (sscanf(tmp, "Auth: %d", (int *)&auth_response.authcode) == 1) 
		{
			log_info(__LOG_DEBUG__, "Auth server returned authentication code %d", auth_response.authcode);
			if(AUTH_ALLOWED == auth_response.authcode)
			{
                _authenticate_client_create_by_getuserstatus(r);
                if((tmp = strstr(tmp, "\n")))
				{
					auth_response.ext = safe_strdup(tmp+1);
        			log_info(__LOG_DEBUG__, "auth_response.ext:%s",auth_response.ext);
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
	client = client_list_find_by_mac(mac);
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
    
    stage = http_parse_name_of_value(r->readBuf,"stage");
    if(NULL == stage)
    {
		log_info(__LOG_ERROR__, "Auth server did not return expected parameterstage");
		goto end;
    }
	switch(auth_response.authcode) 
	{
    	case AUTH_ERROR:
    		/* Error talking to central server */
    		log_info(__LOG_ERROR__, "Got %d from central server authenticating token %s from %s at %s", 
    		    auth_response.authcode, client->token, client->ip, client->mac);
            if(IS_NEW_ALIPAY)
    		{
    		    goto end;
    		}
    		send_http_page(r, "Error!", "Error: We did not get a valid answer from the central server");
    		break;
    	case AUTH_DENIED:
    		/* Central server said invalid token */
    		log_info(__LOG_DEBUG__, "Got DENIED from central server authenticating token %s from %s at %s - redirecting them to denied message", 
    		    client->token, client->ip, client->mac);
            /*begin: 2015-12-8 更新关联表认证信息*/
            eRet = sta_update_auth_info(mac,&stNewStaAuthInfo,&stOldStaAuthInfo);
            if(1 == pConf->uiSpanSsidAuthSwith)
            {
                if(1 == stOldStaAuthInfo.uiLastAuthStatus)
                {
                    http_sta_onoff_to_server(REQUEST_TYPE_LOGOUT,mac,stOldStaAuthInfo.aucLastAuthSsid,aucApStrMac,stOldStaAuthInfo.aucToken,CLIENT_LOGOUT_SPAN_SSID,r);
                }
            }
            /*end: 2015-12-8 更新关联表认证信息*/

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
    		log_info(__LOG_DEBUG__, "Got VALIDATION from central server authenticating token %s from %s at %s"
    				"- adding to firewall and redirecting them to activate message", client->token,
    				client->ip, client->mac);
    		client->fw_connection_state = FW_MARK_PROBATION;
			sys_log_set(WIFIDOG_USR_LOG_FILE, "nf-ALLOW: AUTH_VALIDATION %s %s", client->ip, client->mac);
			client_list_up_allqos();
    		nf_fw_access(FW_ACCESS_ALLOW, client->mac,client);
    		if(config->qos_enable)
                fw_qos_add(client->mac, client->bw_up, client->bw_down);
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
    				"adding to firewall and redirecting them to portal.",
    				client->token, client->ip, client->mac);
            if(auth_response.ext){
    			log_info(__LOG_MSG__, "EXT:[%s]", auth_response.ext);
    			sscanf(auth_response.ext, "%u %u %llu %llu %lu %lu", &client->bw_up, &client->bw_down, 
                    &client->max_bw_up, &client->max_bw_down, &client->idletimeout, &client->forcetimeout);
            }
            else{
				client->idletimeout = config->idletimeout;
				client->forcetimeout = config->forcetimeout;
				client->bw_up = config->bw_up;
				client->bw_down = config->bw_down;
            }


            /*begin: 2015-12-8 更新关联表认证信息*/
            /*初始化newAuthInfo==*/
            /*跨ssid级认证--更新认证信息*/
            if(http_check_auth_info(r,mac,client->token)<0)
            {
                goto end;
            }
             /*end: 2015-12-8 更新关联表认证信息*/

			
			sys_log_set(WIFIDOG_USR_LOG_FILE, "nf-ALLOW: AUTH_ALLOWED %s %s %d %s", 
				client->ip, client->mac, client->fw_connection_state,
				(client->method_type != NULL) ? client->method_type : "");
    		client->fw_connection_state = FW_MARK_KNOWN;
    		client->login_time = now;
			/*online_time_last_update_time*/
			client->online_time_last_update_time = now;
			auth_period = now - client->auth_period;
			client->auth_period = auth_period;
			if((client->bw_up > 0) || (client->bw_down > 0)){
				if((config->bw_up != client->bw_up) || (config->bw_down != client->bw_down))
					client->bw_qos_sta = 1;
			}
			
			client_list_up_allqos();
    		nf_fw_access(FW_ACCESS_ALLOW, client->mac,client);
    		if(config->qos_enable)
                fw_qos_add(client->mac, client->bw_up, client->bw_down);
			
			if(auth_mode_support_alipay()){
				if(IS_NEW_ALIPAY)
				{
					log_info(__LOG_DEBUG__, "******aliauth allowd. return.****");
				    goto end;
				}
			}
			/*begin: modified @2015.01.28 for wechat connect wifi*/
			if (auth_mode_support_wxl() 
                && (NULL != client->method_type) 
                && (0 == strncmp(client->method_type,"wxwifi",6)))
			{
#if 0
				safe_asprintf(&urlFragment, "%s://%s/?res=success", "http", WXL_REDIRECT_URL);
				/*safe_asprintf(&urlFragment, "%s://%s/?res=already", "http", WXL_REDIRECT_URL);*/
				http_send_redirect(r, urlFragment, "Redirect to portal");
				free(urlFragment);
#endif
				log_info(__LOG_DEBUG__, "******wxl auth allowd. return.****");
			}
			/*end: modified @2015.01.28 for wechat connect wifi*/
			else
			{
				safe_asprintf(&urlFragment, "%sgw_id=%s&token=%s",
						auth_server->authserv_portal_script_path_fragment,
						config->gw_id,
						client->token);
				log_info(__LOG_DEBUG__, "******other auth allowd. redirect to portal.****");
				http_send_redirect_to_auth(r, urlFragment, "Redirect to portal");
				free(urlFragment);
			}
			break;
        case AUTH_VALIDATION_FAILED:
    		 /* Client had X minutes to validate account by email and didn't = too late */
    		log_info(__LOG_DEBUG__, "Got VALIDATION_FAILED from central server authenticating token %s from %s at %s "
    				"- redirecting them to failed_validation message",
    				client->token, client->ip, client->mac);


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
    if(NULL != auth_response.ext)
    {
        free(auth_response.ext);
        auth_response.ext=NULL;
    }
    if(NULL != stage)
    {
        free(stage);
        stage=NULL;
    }
    
	return;
}

void authenticate_client(request *r)
{
	char ip[64] = {0};
	char mac[64] = {0};
	char other_ip[64] = {0};
	char other_mac[64] = {0};
    char *jsondata = NULL,aliflag = 0;
	log_info(__LOG_DEBUG__, "[AUTHENTICATION-STEP-[authenticate-client]]. r->clientAddr: %s", r->clientAddr);
    if((int )(AUTH_MODE_SUPPORT == (int )auth_mode[AUTH_MODE_ALIPAY]) && (strstr(r->readBuf, "Aliauth:")) && (strstr(r->readBuf, "open_id1")))
    {
        //log_info(__LOG_DEBUG__, "*****alipay*******");
        if(0 != aliauth_http_parse_json_data(r,&jsondata))
        {
            goto end;
        }
        //sys_log_set(WIFIDOG_USR_LOG_FILE,"data:%s",jsondata);
        aliflag = 1;
    }

	/*begin: add@2015.02.11 for wechat connect wifi*/

	/** @解析认证状态 */
	/*根据现有流程，收到一条消息就会关闭连接*/
	/*所以仅根据client结构提取other_mac和other_ip*/
    
    /*提取请求终端的ip、mac*/
	strcpy(ip, r->clientAddr);
	strcpy(mac, r->clientMac);
	_authenticate_client(r, ip, mac, other_ip, other_mac);

	/*提取请求终端关联的other ip/mac(例如微信连PC的ip/mac)*/
	if((strlen(other_ip) > 0) && (strlen(other_mac) > 0))
		_authenticate_client(r, other_ip, other_mac, NULL, NULL);

    if(1 == aliflag)
    {
        http_alipay_sendto_client(r,jsondata);
        free(jsondata);
    }
end:    
    r->freeFlag = ON_FREE;
    httpdEndRequest(r);
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
        free(jsondata);
        log_info(__LOG_DEBUG__, "jsfromserver parse error ");
        return -1;
    }
    
    pstJsToClient = cJSON_CreateObject();
    if(NULL == pstJsToClient)
    {
        free(jsondata);
        cJSON_Delete(pstJsFromServer);
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
    
    cJSON_Delete(pstJsFromServer);
    cJSON_Delete(pstJsToClient);
    free(jsondata);
    
    if(NULL == *jsdata)
    {
        return -1;
    }
   // sys_log_set(WIFIDOG_USR_LOG_FILE,"alipay jsData to alipay: %s ",*jsdata);
    
    
    return 0;
}



void http_alipay_sendto_client(request *r,char *jsdata)
{
    httpdSetResponse(r, "200 OK");/**/
    httpdSetContentType(r, "text/plain");
    sys_log_set(WIFIDOG_USR_LOG_FILE,"send to alipay client jsdata:%s",jsdata);
    httpdSendContentBody(r,jsdata,strlen(jsdata));
}

void http_sta_info_sendto_client(request *r,char *jsdata)
{
    httpdSetResponse(r, "200 OK");/**/
    httpdSetContentType(r, "application/json");
    sys_log_set(WIFIDOG_USR_LOG_FILE,"send to alipay client jsdata:%s",jsdata);
    httpdSendContentBodyAjax(r,jsdata,strlen(jsdata));
}

void http_send_200OK_to_client(request *r,char *data)
{
    if(-1 == r->clientSock)
    {
        log_info(__LOG_ERROR__, "jstoclient creat error ");
        return;
    }
    httpdSetResponse(r, "200 OK");/**/
    httpdSetContentType(r, "text/plain");
    httpdSendContentBody(r, data, strlen(data));
}


void http_jinjiang_sendto_client(request *r,char *jsdata)
{
    httpdSetResponse(r, "200 OK");/**/
    httpdSetContentType(r, "text/plain");
    sys_log_set(WIFIDOG_USR_LOG_FILE,"send to jinjiang client jsdata:%s",jsdata);
    httpdSendContentBody(r,jsdata,strlen(jsdata));

	free(jsdata);
}




void http_send_to_local(request *r, char *html_file, char *text)
{
    /*
	** Setup for a standard response
	*/
	strcpy(r->response.headers,
		"Server: Technologies Embedded Server\n"); 
	strcpy(r->response.contentType, "text/html");
	strcpy(r->response.response,"200 Output Follows\n");
	r->response.headersSent = 0;
    
    http_send_redirect_to_local(r, html_file, text);
    httpdEndRequest(r);
    
    return;
}


void http_send_thirdparty_redirect(httpd *webserver, request *r)
{
#define BOA_NEW_PORT 8090
	char *message = NULL;
	char *header = NULL;
	char *response = NULL;
	char host_path[128];
	char *pt;
	s_config *config = config_get_config();
	char *client_mac = NULL;
	unsigned int mac[6];
	unsigned int gw_mac[6];

	client_mac = arp_get(r->clientAddr);
	if(NULL == client_mac)
		return;
	
	strncpy(host_path, r->request.host, 128);
	if((pt = strstr(host_path, "//")) != NULL) /*去掉host中的子路径*/
		*pt = '\0';
	
	log_info(__LOG_DEBUG__, "[http_send_thirdparty_redirect] begin. r->clientAddr: %s r->request.host = %s", 
		r->clientAddr, r->request.host);
	
	char *url;
	/*http://addr/index.jsp?HID=100001&MA=8C705A24C850&UIP=192.168.1.12*/
	/*&NI=6C4D5A24C851&OS=http://www.google.com%2F&SIP=10.0.0.13*/
	sscanf(client_mac, MAC_FMT, &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
	sscanf(config->wan_mac, MAC_FMT, &gw_mac[0], &gw_mac[1], &gw_mac[2], &gw_mac[3], &gw_mac[4], &gw_mac[5]);
	safe_asprintf(&url, "http://%s/index.jsp?MA=%02X%02X%02X%02X%02X%02X&SIP=%s&UIP=%s&NI=%02X%02X%02X%02X%02X%02X&OS=%s", 
		(config->auth_servers ? (config->auth_servers->authserv_hostname 
			? config->auth_servers->authserv_hostname : "") : ""), 
		mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], 
		r->clientAddr, config->gw_address, 
		gw_mac[0], gw_mac[1], gw_mac[2], gw_mac[3], gw_mac[4], gw_mac[5], 
		host_path);
	log_info(__LOG_DEBUG__, "Redirecting client browser to %s", url);
	safe_asprintf(&header, "Location: %s", url);
	safe_asprintf(&response, "301 Redirecting\n");	
	httpdSetResponse(r, response);
	httpdAddHeader(r, header);
	free(response);
	free(header);	
	safe_asprintf(&message, "如果没自动跳转, 请<a href='%s'>点击</a>.", url);
	send_http_page(r, "Redirection to message", message);
	free(message);
	free(url);
	free(client_mac);

	httpdEndRequest(r);
	return;
}

extern int set_sta_flowctrl(char *ip, char *mac, unsigned int bw_up, unsigned int bw_down);
extern int set_sta_force_timeout(char *ip, char *mac, int force_timeout);


int jinjiang_auth_http_parse_json_data(request *r,char **jsdata)
{
    char *jsondata =NULL;
    char *data = NULL;
    cJSON *pstJsFromServer = NULL,*pstJsToClient = NULL;
	cJSON *pstJsMac = NULL;
	cJSON *pstJsIp = NULL;
	cJSON *pstUnits = NULL;
	cJSON *pstExpire = NULL;
	cJSON *pstBandUP = NULL;
	cJSON *pstBandDown = NULL;
	char sta_mac[32];
	int expire = 0;
	unsigned int bw_up = 0, bw_down = 0;
	char jc = '{';
	int loop;

    if(NULL == r){
        log_info(__LOG_DEBUG__, "request is NULL");
        return -1;
    }

    /*获得json数据*/
    data = memmem(r->readBuf, HTTP_READ_BUF_LEN, &jc, 1);
	if(NULL == data)
		return -1;

    safe_asprintf(&jsondata, "%s", data);
    if(NULL == jsondata)
        return -1;

    log_info(__LOG_DEBUG__, "jsfromserver data:[%s]", jsondata);
    
    pstJsFromServer = cJSON_Parse(jsondata);
    if(NULL == pstJsFromServer){
		free(jsondata);
        log_info(__LOG_DEBUG__, "jsfromserver parse error ");
        return -1;
    }
    
    pstJsToClient = cJSON_CreateObject();
    if(NULL == pstJsToClient){
        log_info(__LOG_DEBUG__, "jstoclient create error ");
		free(jsondata);
		cJSON_Delete(pstJsFromServer);
        return -1;
    }

	/*设置终端上线*/
    if((pstJsMac = cJSON_GetObjectItem(pstJsFromServer, "MAC_ADDR")) == NULL)
		goto send_client_error;
	else{
		if(pstJsMac->type != cJSON_String)
			goto send_client_error;
		else{
			if(strlen(pstJsMac->valuestring) < 12)
				goto send_client_error;

			if(strstr(pstJsMac->valuestring, ":")) /*mac地址是xx:xx:xx:xx:xx:xx格式*/
				strncpy(sta_mac, pstJsMac->valuestring, 32);
			else{/*mac地址是xxxxxxxxxxxx格式*/
				for(loop=0; loop<6; ++loop){
					sta_mac[loop*3] = pstJsMac->valuestring[loop*2];
					sta_mac[loop*3+1] = pstJsMac->valuestring[loop*2+1];
					sta_mac[loop*3+2] = ':';
				}
				sta_mac[loop*3-1] = '\0';
			}
		}
		if((pstJsIp = cJSON_GetObjectItem(pstJsFromServer, "SIP")) != NULL){
			if(pstJsIp->type != cJSON_String)
				goto send_client_error;
			set_sta_auth_status(pstJsIp->valuestring, sta_mac, 2, "80auth",NULL, FW_MARK_KNOWN);
		}
		else{
			set_sta_auth_status(r->clientAddr, sta_mac, 2, "80auth", NULL,FW_MARK_KNOWN);
		}
	}
	
	/*设置终端硬性断开时间*/
	if((pstExpire = cJSON_GetObjectItem(pstJsFromServer, "EXPIRY_TIME")) != NULL){
		if(pstExpire->type == cJSON_Number)
			expire = pstExpire->valueint;
		else if(pstExpire->type == cJSON_String)
			expire = atoi(pstExpire->valuestring);
		else
			goto send_client_error;

		if((pstUnits = cJSON_GetObjectItem(pstJsFromServer, "UNITS")) != NULL){
			char strunits[32];
			if(pstUnits->type != cJSON_String)
				goto send_client_error;
			strncpy(strunits, pstUnits->valuestring, 32);
			if(!strcmp(strunits, "SECONDS"))
				expire = expire;
			else if(!strcmp(strunits, "MINUTES"))
				expire *= 60;
			else if(!strcmp(strunits, "HOURS"))
				expire *= 3600;
			else if(!strcmp(strunits, "DAYS"))
				expire *= (3600*24);
			else
				expire *= 60; /*默认单位为分钟*/
		}
		else{
			expire *= 60; /*默认单位为分钟*/
		}
	}
	else{
		expire = 2880 * 60; /*默认为2880分钟*/
	}
	if(pstJsIp != NULL)
		set_sta_force_timeout(pstJsIp->valuestring, sta_mac, expire);
	else
		set_sta_force_timeout(r->clientAddr, sta_mac, expire);
		
	/*设置终端流控*/
	if((pstBandUP = cJSON_GetObjectItem(pstJsFromServer, "BANDWIDTH_MAX_UP")) != NULL){
		if(pstBandUP->type == cJSON_String)
			bw_up = atoi(pstBandUP->valuestring);
		else if(pstBandUP->type == cJSON_Number)
			bw_up = pstBandUP->valueint;
		else
			goto send_client_error;
	}
	if((pstBandDown = cJSON_GetObjectItem(pstJsFromServer, "BANDWIDTH_MAX_DOWN")) != NULL){
		if(pstBandDown->type == cJSON_String)
			bw_down = atoi(pstBandDown->valuestring);
		else if(pstBandDown->type == cJSON_Number)
			bw_down = pstBandDown->valueint;
		else
			goto send_client_error;
	}
	if(pstJsIp != NULL)
		set_sta_flowctrl(pstJsIp->valuestring, sta_mac, bw_up, bw_down);
	else
		set_sta_flowctrl(r->clientAddr, sta_mac, bw_up, bw_down);

	/*返回值*/
	cJSON_AddStringToObject(pstJsToClient,"RESULT","OK");
	if(pstJsIp != NULL)
		cJSON_AddStringToObject(pstJsToClient,"IP",pstJsIp->valuestring);
	else
		cJSON_AddStringToObject(pstJsToClient,"IP",r->clientAddr);
	
    *jsdata = cJSON_PrintUnformatted(pstJsToClient); 

	cJSON_Delete(pstJsFromServer);
	cJSON_Delete(pstJsToClient); 
	free(jsondata);

    if(NULL == *jsdata)
		return -1;

	// sys_log_set(WIFIDOG_USR_LOG_FILE,"jsData to jiajiang: %s ",*jsdata);

    return 0;

send_client_error:
	cJSON_AddStringToObject(pstJsToClient,"RESULT","ERROR");
	if(pstJsIp != NULL)
		cJSON_AddStringToObject(pstJsToClient,"IP",pstJsIp->valuestring);
	else
		cJSON_AddStringToObject(pstJsToClient,"IP",r->clientAddr);
	cJSON_AddStringToObject(pstJsToClient,"ERROR_NUM", "");
	cJSON_AddStringToObject(pstJsToClient,"ERROR_DESC", "Parsing error");

	*jsdata = cJSON_PrintUnformatted(pstJsToClient); 
	
	cJSON_Delete(pstJsFromServer);
	cJSON_Delete(pstJsToClient); 
	free(jsondata);

    if(NULL == *jsdata)
		return -1;

	return 0;
}


/**
begin:网间漫游2015-8-26
**/
void http_send_check_to_server(unsigned char *ip,unsigned char *pucStaMac)
{
        request *r; 
        pthread_mutex_lock(&g_client_request_8060);
        if(NULL == (r = get_invalid_sockfd(client_request, CLIENT_TCP_8060_NUM)))
        {
            log_info(__LOG_WARNING__, "Could not find a invalid request data");
            pthread_mutex_unlock(&g_client_request_8060);
            return;
        }
        sprintf(r->clientMac,MAC_FMT,MAC_ARG(pucStaMac));
        sprintf(r->clientAddr,IP_FMT,IP_ARG(ip));
        r->clientMac[HTTP_MAC_LEN-1]=0;
        sys_log_set(WIFIDOG_MARCO_HOT_LOG, "send sta_mac:%s <stage=getuserstatus> to server",r->clientMac);
        auth_server_request(r, REQUEST_TYPE_GETUSER,r->clientAddr, r->clientMac, "", 0, 0);
        pthread_mutex_unlock(&g_client_request_8060);
        return;
}


/*上报用户认证上线状态*/
void http_add_sta_to_server(t_client *client)
{
    request *r; 
    //pthread_mutex_lock(&g_client_request_8060);
    if(NULL == (r = get_invalid_sockfd(client_request, CLIENT_TCP_8060_NUM)))
    {
        log_info(__LOG_WARNING__, "Could not find a invalid request data");
        return;
    }
    strcpy(r->clientAddr,client->ip);
    strcpy(r->clientMac,client->mac);
   // sys_log_set("/tmp/wd_ccs_auth.log","ip=%s",client->ip);
    auth_server_request(r, REQUEST_TYPE_LOGIN, r->clientAddr, r->clientMac,"", 0, 0);
    //pthread_mutex_unlock(&g_client_request_8060);

    return;
}
/*上报用户认证下线状态*/
void http_del_sta_to_server(t_client *client,CLIENT_LOGOUT_TYPE_E eType)
{
    request *r; 
    char aucReqType[64]={0};
    
    //pthread_mutex_unlock(&g_client_request_8060);
    if(NULL == (r = get_invalid_sockfd(client_request, CLIENT_TCP_8060_NUM)))
    {
        log_info(__LOG_WARNING__, "Could not find a invalid request data");
        return;
    }
    strcpy(r->clientAddr,"0.0.0.0");
    strcpy(r->clientMac,client->mac);
    sprintf(aucReqType,"%s&logout_type=%d",REQUEST_TYPE_LOGOUT,eType);
    auth_server_request(r, aucReqType, r->clientAddr, r->clientMac, client->token, 0, 0);
    //pthread_mutex_unlock(&g_client_request_8060);

    return;
}


void http_sta_onoff_to_server(char *pRequestType,char *pStrStaMac,char *pSsid,char *pStrApMac,char *pToken,CLIENT_LOGOUT_TYPE_E eType,request *r)
{
    request *pstRequest = NULL;
    char aucReqType[64]={0};
    char aucIp[18]={0};
    strcpy(aucIp,"0.0.0.0");
    if(NULL == pStrStaMac || NULL == pRequestType)
    {
        log_info(__LOG_ERROR__, "pStrStaMac is NULL return\n");
        return;
    }

    if(NULL == r)
    {
        if(NULL == (pstRequest = get_invalid_sockfd(client_request, CLIENT_TCP_8060_NUM)))
        {
            log_info(__LOG_WARNING__, "Could not find a invalid request data");
            return;
        }
   }
   else
   {
        pstRequest = r;
        strcpy(aucIp,r->clientAddr);
   }
   if(0 == strcmp(pRequestType,REQUEST_TYPE_LOGOUT))
   {
     sprintf(aucReqType,"%s&logout_type=%d",REQUEST_TYPE_LOGOUT,eType);
   }
   else
   {
        sprintf(aucReqType,pRequestType);
   }

    __auth_server_request_sta_login_logout(pstRequest,aucReqType,aucIp,pStrStaMac,pToken,pSsid,pStrApMac);

    return;

}






int macro_hot_client_handle(char *ip,roam_client *pRoamClient)
{
    if(NULL == ip || NULL == pRoamClient)
    {
        return -1;
    }
    t_client *client =NULL;
    char aucStrMac[32]={0};
    sprintf(aucStrMac,MAC_FMT,MAC_ARG(pRoamClient->aucIntMac));

    LOCK_CLIENT_LIST();
    client = client_list_find_by_mac(aucStrMac);
    if(NULL == client)
    {
        /** 判断是否超过最大接入用户数 */
        if(g_client_access_num >= config_get_config()->max_client)
        {
            log_info(__LOG_DEBUG__, "over mac_client:%d\n", config_get_config()->max_client);
            sys_log_set(WIFIDOG_MARCO_HOT_LOG,"mac:%s over max_client:%d\n", aucStrMac,config_get_config()->max_client);
            UNLOCK_CLIENT_LIST();
            return -1;
        }
        if((config_get_config()->dev_max_client > 0) &&(g_client_access_num >= config_get_config()->dev_max_client))
        {
            log_info(__LOG_DEBUG__, "over dev_mac_client:%d\n", config_get_config()->dev_max_client);
            sys_log_set(WIFIDOG_MARCO_HOT_LOG,"mac:%s over dev_max_client:%d\n", aucStrMac,config_get_config()->dev_max_client);
            UNLOCK_CLIENT_LIST();
            return -1;
        }

        ADD_CLIENT_OTHERINFO_T stOtherInfo;
        memset(&stOtherInfo,0,sizeof(ADD_CLIENT_OTHERINFO_T));
        strcpy(stOtherInfo.stAuthInfo.aucUserCount,"Roam_GW");
        client=client_list_append(ip,aucStrMac,pRoamClient->token,NULL,&stOtherInfo);
    }
    else
    {
        if(NULL != client->token)
        {
            free(client->token);
        }
        client->token=safe_strdup(pRoamClient->token);
        
    }

    client->forcetimeout = config_get_config()->forcetimeout;
    client->idletimeout = config_get_config()->idletimeout;
    client->counters.incoming = pRoamClient->incoming;
    client->counters.outgoing = pRoamClient->outgoing;
    client->bw_up = config_get_config()->bw_up;
    client->bw_down = config_get_config()->bw_down;
    if(config_get_config()->qos_enable)
    {
        fw_qos_add(client->mac, client->bw_up, client->bw_down);    
    }
    client->fw_connection_state = FW_MARK_KNOWN;
    client->online_time = pRoamClient->online_time;
    client->auth_period = 0;  
    nf_fw_access(FW_ACCESS_ALLOW, client->mac,client);

    sys_log_set(WIFIDOG_USR_LOG_FILE,"nf-ALLOW: macro-hot-sync ip:%s  mac:%s",client->ip,client->mac);
    sys_log_set(WIFIDOG_MARCO_HOT_LOG,"nf-ALLOW: macro-hot-sync client_IP:%s  client_mac:%s",client->ip,client->mac);
    UNLOCK_CLIENT_LIST();
    
    return 0;
}




/*begin:网间漫游2015-8-28*/
void thread_macro_hot(void)
{
    s_config *config;
    config = config_get_config();
    sys_log_set(WIFIDOG_MARCO_HOT_LOG, "Thread thread_macro_hot Start..");

    wifidog_set_thread_name(WIFIDOG_THREAD_MACRO_HOT);
    config->macro_hotspot_thread_status = 1;
    
    unsigned int index;
    t_client *curclient = NULL;
	char	*ip;	
    //roam_client g_roam_client_head[CLIENT_HASH_SIZE];
    while(1)
    {

        if(1 == g_macro_hot_thread_close)
        {
            config->macro_hotspot_thread_status =0;
            sys_log_set(WIFIDOG_MARCO_HOT_LOG, "Thread thread_macro_hot Stop..");
            return ;
        }
    
        sleep(2);
        g_status_macro_hot= (g_status_macro_hot+1)%0x40000;
        // log_info(__LOG_DEBUG__, "*******find roam mac  ip\n");
        LOCK_CLIENT_LIST();
        for(index=0; index < CLIENT_HASH_SIZE; index++)
        {
            curclient = g_client_head[index].next;
            while(NULL != curclient)
            {
                if(FLAG_IP_INVALID == curclient->iIpFlag)
                {
                    if(NULL != (ip=arp_get_ip(curclient->mac)))
                    {
                        client_ip_update(curclient,ip);
                        curclient->iIpFlag=FLAG_IP_GET;
                    }
                }
                curclient = curclient->next;
            }
        }
        UNLOCK_CLIENT_LIST();
    }
    return;
    
}

/*end:网间漫游2015-8-28*/











