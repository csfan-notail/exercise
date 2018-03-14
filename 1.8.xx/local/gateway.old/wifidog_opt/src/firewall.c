/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : firewall.c
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/11/26
  Last Modified :
  Description   : 
  Function List :
  History       :
  1.Date        : 2013/11/26
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <syslog.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
/*add@2014.8.6*/
#include <pthread.h>

#include "lib_pa.h"

#include "log.h"
#include "conf.h"
#include "cJSON.h"
#include "util.h"
#include "common.h"
#include "client_list.h"
#include "firewall.h"

/*begin: add @2014.8.6*/
typedef enum{
	WIFIDOG_FREEAUTH_OFF = 0,
	WIFIDOG_FREEAUTH_ON
}en_freeauth;
en_freeauth g_freeauth_switch = WIFIDOG_FREEAUTH_ON;
pthread_mutex_t g_freeauth_mutex = PTHREAD_MUTEX_INITIALIZER;

#define FREEAUTH_SWITCH_LOCK do \
{\
	log_info(__LOG_DEBUG__, "g_freeauth_mutex LOCK");\
	pthread_mutex_lock(&g_freeauth_mutex);\
}while(0);

#define FREEAUTH_SWITCH_UNLOCK do \
{\
	log_info(__LOG_DEBUG__, "g_freeauth_mutex UNLOCK");\
	pthread_mutex_unlock(&g_freeauth_mutex);\
}while(0);

/*end: add @2014.8.6*/


int fw_destroy(void)
{
	int loop;
    char command[256];
	s_config *config = config_get_config();

	if(0 == config_get_config()->child_process)
		return 0;
	
	for(loop=0; loop<FIRST_PAGE_TASK_NUM; ++loop){
	    sprintf(command, "iptables -t nat -D PREROUTING -p tcp -m mark --mark 0x%02x -j DNAT --to-destination %s:%d 1>/dev/null 2>&1", 
			loop+1, config->gw_address, config->gw_redirect_port+loop);
		system(command);
    	log_info(__LOG_DEBUG__, "%s", command);
	}

    portal_set_set_free_auth_sw(1);
    
    return 0;
}

static void full_url_to_server_url(char *fullurl, char *serverurl, int maxlen)
{
	char prefix[32] = "http://";
	char *pt, *pt2;
	int len;

	if((pt = strstr(fullurl, prefix)) != NULL){
		pt += strlen(prefix);
	}
	else
		pt = fullurl;

	if((pt2 = strstr(pt, ":")) != NULL){
		*pt2 = '\0';
	}

	if((pt2 = strstr(pt, "/")) != NULL){
		*pt2 = '\0';
	}

	len = strlen(pt);
	strncpy(serverurl, pt, (len > maxlen ? maxlen : len));

	return;
}


int fw_init(s_config *config)
{
	int loop;
    char command[256];
	char serverurl[256];

    struct in_addr *h_addr = NULL;
    char *ip = NULL;
    fw_destroy();
    /** @配置重定向规则 */
	for(loop=0; loop<FIRST_PAGE_TASK_NUM; ++loop){		
		/* BEGIN: Added by WeiHonggang, 2014/3/12	PN:BUG#0兼容多个下联口无法重定向的问题 */
		//sprintf(command, "iptables -t nat -A PREROUTING -p tcp -m mark --mark 0x01 -j REDIRECT --to-ports %d 1>/dev/null 2>&1", config->gw_redirect_port);
		sprintf(command, "iptables -t nat -I PREROUTING -p tcp -m mark --mark 0x%02x -j DNAT --to-destination %s:%d 1>/dev/null 2>&1", 
			loop+1, config->gw_address,config->gw_redirect_port+loop);
		system(command);
		/* END:   Added by WeiHonggang, 2014/3/12	PN:BUG#0兼容多个下联口无法重定向的问题 */
		log_info(__LOG_DEBUG__, "%s", command);
	}
    
    /** @设置NF模块规则 */
    portal_set_set_free_auth_sw(0);

    /** @配置认证服务器地址 */
    portal_set_auth_server_ip((unsigned)inet_addr(config->auth_servers->last_ip));
    log_info(__LOG_DEBUG__, "authserv_hostname = %s", config->auth_servers->last_ip);

    /** @配置CCS服务器地址 */
    log_info(__LOG_DEBUG__, "ccs server %s", config->ccs_server);
    if(!config->ccs_server)
        config->ccs_server = safe_strdup("www.login-wifi.com");
	memset(serverurl, 0, sizeof(serverurl));
	full_url_to_server_url(config->ccs_server, serverurl, 256);
    h_addr = wd_gethostbyname(serverurl);
    if(NULL == h_addr)
    {
        log_info(__LOG_ERROR__, "Could not config ccs server");
    }
    else
    {
        ip = safe_strdup(inet_ntoa(*h_addr));
        free(h_addr);
        portal_set_ccs_server_ip((unsigned)inet_addr(ip));
        log_info(__LOG_DEBUG__, "ccs_server = %s", ip);
        free(ip);
    }

	/** @配置SNMP服务器地址 */
    log_info(__LOG_DEBUG__, "snmp server %s", config->snmp_server);
    if(!config->snmp_server)
        config->snmp_server = safe_strdup("www.login-wifi.com");
	memset(serverurl, 0, sizeof(serverurl));
	full_url_to_server_url(config->snmp_server, serverurl, 256);
    h_addr = wd_gethostbyname(serverurl);
    if(NULL == h_addr)
    {
        log_info(__LOG_ERROR__, "Could not config snmp server");
    }
    else
    {
        ip = safe_strdup(inet_ntoa(*h_addr));
        free(h_addr);
        portal_set_snmp_server_ip((unsigned)inet_addr(ip));
        log_info(__LOG_DEBUG__, "snmp_server = %s", ip);
        free(ip);
    }

    /** @配置网管主机地址 */
    portal_set_host_ip((unsigned)inet_addr(config->gw_address));

    /** @配置lanif */
    /* BEGIN: Added by WeiHonggang, 2014/3/5   PN:BUG #5099 【优化】兼容下联口分布在不同的桥内的组网 */
    //portal_set_lan_if_name((unsigned char *)"br0");
    portal_set_lan_if_name((unsigned char *)"br");
    /* END:   Added by WeiHonggang, 2014/3/5   PN:BUG #5099 【优化】兼容下联口分布在不同的桥内的组网 */

    
    return 0;
}

int nf_fw_access(fw_access_t type, const char *ip, const char *mac)
{
    unsigned char dst_mac[6];

    mac_converter(mac, dst_mac);
    switch(type)
    {
        case FW_ACCESS_ALLOW:
            //** @认证通过
            log_info(__LOG_DEBUG__, "Set Client ip = %s  mac = %s allow access firewall.", ip, mac);
            portal_set_auth_state(dst_mac, (unsigned int)inet_addr(ip), 2);
            break;

        case FW_ACCESS_DENY:
            //** @认证不通过
            log_info(__LOG_DEBUG__, "Set Client ip = %s  mac = %s deny access firewall.", ip, mac);
            portal_set_auth_state(dst_mac, (unsigned int)inet_addr(ip), 0);
            break;

		case FW_ACCESS_DEL:
            //** @删除条目
            log_info(__LOG_DEBUG__, "Set Client ip = %s  mac = %s del access firewall.", ip, mac);
            portal_set_auth_state(dst_mac, (unsigned int)inet_addr(ip), 3);
            break;
    }
    return 0;
}

char *arp_get(char *req_ip)
{
    FILE           *proc;
	char ip[16] = {0};
	char mac[18] = {0};
	char * reply = NULL;
	int len, i;

    if (!(proc = fopen("/proc/net/arp", "r"))) {
        return NULL;
    }

    /* Skip first line */
	while (!feof(proc) && fgetc(proc) != '\n');

	 /* Find ip, copy mac in reply */
	 reply = NULL;
    while (!feof(proc) && (fscanf(proc, " %15[0-9.] %*s %*s %17[A-Fa-f0-9:] %*s %*s", ip, mac) == 2)) {
		  if (strcmp(ip, req_ip) == 0) {
		  		len = strlen(mac);
				for(i=0; i<len; ++i){
					if(mac[i] >= 'a' && mac[i] <= 'f')
						mac[i] -= ('a' - 'A');
				}
				reply = safe_strdup(mac);
				break;
		  }
    }

    fclose(proc);    
    return reply;
}

int fw_set_free_auth(void)
{
	/*begin: modified@2014.8.6 for adding g_freeauth_switch*/
	FREEAUTH_SWITCH_LOCK;
	if (WIFIDOG_FREEAUTH_OFF == g_freeauth_switch){
		g_freeauth_switch = WIFIDOG_FREEAUTH_ON;
		FREEAUTH_SWITCH_UNLOCK;
		return portal_set_set_free_auth_sw(1);
	}
	FREEAUTH_SWITCH_UNLOCK;	
	/*end: modified@2014.8.6 for adding g_freeauth_switch*/
	return 0;
}

int fw_set_auth(void)
{
	/*begin: modified@2014.8.6 for adding g_freeauth_switch*/
	FREEAUTH_SWITCH_LOCK;
	if (WIFIDOG_FREEAUTH_ON == g_freeauth_switch){
		g_freeauth_switch = WIFIDOG_FREEAUTH_OFF;		
		FREEAUTH_SWITCH_UNLOCK;
		return portal_set_set_free_auth_sw(0);
	}
	FREEAUTH_SWITCH_UNLOCK;
	/*end: modified@2014.8.6 for adding g_freeauth_switch*/
	return 0;
}

int g_fw_qos_init = 0;

int fw_qos_init(void)
{
	int rule = -1;
    char value[64];
    if(sys_config_get(value, WIFIDOG_VLAN_FC_RULE) >= 0)
    	rule = atoi(value);

	if(rule <= 0){
	    system("flowctrl admin enable");
	    system("flowctrl rule sta");
		g_fw_qos_init = 1;
	}

    return 0;
}

int fw_qos_reinit(void)
{
	if(0 == g_fw_qos_init){
		sys_config_set(WIFIDOG_VLAN_FC_RULE, "-1");
	    system("flowctrl admin enable");
	    system("flowctrl rule sta");
		g_fw_qos_init = 1;
	}

    return 0;
}


int fw_qos_add(t_client *client)
{
    char command[128];
    unsigned int ulTotal=0;
    int upload=0;
    int download=0;

    if(NULL == client)
    {
		log_info(__LOG_DEBUG__, "client is null, then return");
        return -1;
    }

    if(0 == tools_nvram_listnode_get(NVRAM_WD_WHITE_MAC_LIST,NVRAM_WD_WHITE_MAC_NUM,NVRAM_WD_WHITE_MAC_HEAD,&ulTotal,client->mac))
    {
        upload=client->bw_up;
        download=client->bw_down;
    }
    else
    {
        upload=client->bw_white_up;
        download=client->bw_white_down;
    }
	if(0 == g_fw_qos_init){
		sys_config_set(WIFIDOG_VLAN_FC_RULE, "-1");
	    system("flowctrl admin enable");
	    system("flowctrl rule sta");
		g_fw_qos_init = 1;
	}

	if (0 == upload && 0 == download){
		log_info(__LOG_DEBUG__, "0 == upload && 0 == download, then return");
		return 0;
	}

    sprintf(command, "flowctrl addsta 0 0 %s %d %d %d %d",
        client->mac,
        upload, upload > 5000 ? upload : 5000,
        download, download > 5000 ? download : 5000);

    log_info(__LOG_DEBUG__, "add sta: %s", command);

    return system(command);
}


int fw_qos_del(const char *mac)
{
	if(g_fw_qos_init != 0){
		char command[128];
	    sprintf(command, "flowctrl delsta %s", mac);

	    log_info(__LOG_DEBUG__, "del sta: %s", command);

	    return system(command);
	}

	return 0;
}
/* BEGIN: Added by WeiHonggang, 2014/4/9   PN:BUG #5289  云平台关闭流控，终端重新认证，仍被限速 */
int fw_qos_flush(void)
{
    log_info(__LOG_DEBUG__, "flush all stations");

	if(g_fw_qos_init != 0){
		sys_config_set(WIFIDOG_VLAN_FC_RULE, "0");
		g_fw_qos_init = 0;

	    return system("flowctrl flush");
	}
	
	return 0;
}
/* END:   Added by WeiHonggang, 2014/4/9 */
