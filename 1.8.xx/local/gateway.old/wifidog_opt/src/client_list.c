/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : client_list.c
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/11/21
  Last Modified :
  Description   : 
  Function List :
  History       :
  1.Date        : 2013/11/21
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <syslog.h>
#include <stdarg.h>

#include <pthread.h>
#include "cJSON.h"
#include "util.h"
#include "log.h"
#include "conf.h"
#include "client_list.h"
#include "firewall.h"

/** @定义终端列表全局变量 */
t_client g_client_head[CLIENT_HASH_SIZE];

/** @ 当前接入用户数目 */
int g_client_access_num = 0;
/** add@2014.10.11 云平台的当前在线用户数目*/
int g_cloudplatform_client_num = 0;

/** Global mutex to protect access to the client list */
pthread_mutex_t client_list_mutex = PTHREAD_MUTEX_INITIALIZER;

void client_list_init(void)
{
    memset(g_client_head, 0, sizeof(t_client)*CLIENT_HASH_SIZE);
}

extern int sta_add_by_mac(unsigned char *mac, unsigned char *ap_mac, char *ssid);

t_client *client_list_append(const char *ip, const char *mac, const char *token)
{
    t_client *curclient, *prevclient;
    int endip;
	long now = get_uptime();

    if(-1 == (endip = get_end_ip(ip)))
    {
        return NULL;
    }
    prevclient = NULL;
    curclient = &g_client_head[endip%CLIENT_HASH_SIZE];
    while(NULL != curclient)
    {
        prevclient = curclient;
        curclient = prevclient->next;
    }

    curclient = safe_malloc(sizeof(t_client));
    curclient->login_time = get_uptime();
    curclient->ip = safe_strdup(ip);
    curclient->mac = safe_strdup(mac);
	curclient->other_ip = NULL;
    curclient->other_mac = NULL;
	if (token)
    	curclient->token = safe_strdup(token);
    curclient->counters.incoming = curclient->counters.incoming_history = curclient->counters.outgoing = curclient->counters.outgoing_history = 0;
    curclient->forcetimeout = curclient->idletimeout = curclient->max_bw_up = curclient->max_bw_down = curclient->bw_up = curclient->bw_down = 0;
    curclient->counters.last_updated = get_uptime();
    curclient->id = 0; /** @原先设计配置流控的标记 */
    curclient->fw_connection_state = FW_MARK_AUTH_ONGOING; /**@ 终端处于正在认证中 */
	/*begin: add @2014.8.4*/
	curclient->bw_down = 0;
	curclient->bw_up = 0;
	curclient->max_bw_down = 0;
	curclient->max_bw_up = 0;
	curclient->idletimeout = 0;
	curclient->forcetimeout = 0;
	curclient->online_time = 0;
	curclient->online_time_last_update_time = now;
	curclient->auth_period = now;
    curclient->denied_client_flag = 0;
	/*end: add @2014.8.4*/
		
    curclient->next = NULL;
	curclient->in_cnt = 0;
	curclient->set_logout = 0;

    prevclient->next = curclient;

    /*g_client_access_num++;*//*改为认证过后再计入总数*/
    log_info(__LOG_DEBUG__, "Added a new client to linked list: IP: %s MAC: %s Token: %s", ip, mac, token);
    return curclient;
}

t_client *client_list_append2(const char *ip, const char *mac, const char *token,
									const char *other_ip, const char *other_mac)
{
    t_client *curclient, *prevclient;
    int endip;
	long now = get_uptime();

    if(-1 == (endip = get_end_ip(ip)))
    {
        return NULL;
    }
    prevclient = NULL;
    curclient = &g_client_head[endip%CLIENT_HASH_SIZE];
    while(NULL != curclient)
    {
        prevclient = curclient;
        curclient = prevclient->next;
    }

    curclient = safe_malloc(sizeof(t_client));
    curclient->login_time = get_uptime();
    curclient->ip = safe_strdup(ip);
    curclient->mac = safe_strdup(mac);
	curclient->other_ip = safe_strdup(other_ip);
    curclient->other_mac = safe_strdup(other_mac);
	if (token)
    	curclient->token = safe_strdup(token);
    curclient->counters.incoming = curclient->counters.incoming_history = curclient->counters.outgoing = curclient->counters.outgoing_history = 0;
    curclient->forcetimeout = curclient->idletimeout = curclient->max_bw_up = curclient->max_bw_down = curclient->bw_up = curclient->bw_down = 0;
    curclient->counters.last_updated = get_uptime();
    curclient->id = 0; /** @原先设计配置流控的标记 */
    curclient->fw_connection_state = FW_MARK_AUTH_ONGOING; /**@ 终端处于正在认证中 */
	/*begin: add @2014.8.4*/
	curclient->bw_down = 0;
	curclient->bw_up = 0;
	curclient->max_bw_down = 0;
	curclient->max_bw_up = 0;
	curclient->idletimeout = 0;
	curclient->forcetimeout = 0;
	curclient->online_time = 0;
	curclient->online_time_last_update_time = now;
	curclient->auth_period = now;
    curclient->denied_client_flag = 0;
	/*end: add @2014.8.4*/
		
    curclient->next = NULL;
	curclient->in_cnt = 0;
	curclient->set_logout = 0;

    prevclient->next = curclient;

    /*g_client_access_num++;*//*改为认证过后再计入总数*/
    log_info(__LOG_DEBUG__, "Added a new client to linked list: IP: %s MAC: %s Token: %s", ip, mac, token);
    return curclient;
}

t_client *client_list_append3(const char *ip, const char *mac, const int iAuthType, const char *pBblink_userid,const char *token)
{
    t_client *curclient, *prevclient;
    int endip;
	long now = get_uptime();

    if(-1 == (endip = get_end_ip(ip)))
    {
        return NULL;
    }
    prevclient = NULL;
    curclient = &g_client_head[endip%CLIENT_HASH_SIZE];
    while(NULL != curclient)
    {
        prevclient = curclient;
        curclient = prevclient->next;
    }

    curclient = safe_malloc(sizeof(t_client));
    curclient->login_time = get_uptime();
    curclient->ip = safe_strdup(ip);
    curclient->mac = safe_strdup(mac);
	curclient->other_ip = NULL;
    curclient->other_mac = NULL;
	if (token)
    	curclient->token = safe_strdup(token);
    curclient->counters.incoming = curclient->counters.incoming_history = curclient->counters.outgoing = curclient->counters.outgoing_history = 0;
    curclient->forcetimeout = curclient->idletimeout = curclient->max_bw_up = curclient->max_bw_down = curclient->bw_up = curclient->bw_down = 0;
    curclient->counters.last_updated = get_uptime();
    curclient->id = 0; /** @原先设计配置流控的标记 */
    curclient->fw_connection_state = FW_MARK_AUTH_ONGOING; /**@ 终端处于正在认证中 */
	/*begin: add @2014.8.4*/
	curclient->bw_down = 0;
	curclient->bw_up = 0;
	curclient->max_bw_down = 0;
	curclient->max_bw_up = 0;
	curclient->idletimeout = 0;
	curclient->forcetimeout = 0;
	curclient->online_time = 0;
	curclient->online_time_last_update_time = now;
	curclient->auth_period = now;
    curclient->denied_client_flag = 0;
	/*end: add @2014.8.4*/

    curclient->iAuthType = iAuthType;
    curclient->pBblink_userid = safe_strdup(pBblink_userid);
		
    curclient->next = NULL;
	curclient->in_cnt = 0;
	curclient->set_logout = 0;

    prevclient->next = curclient;

    /*g_client_access_num++;*//*改为认证过后再计入总数*/
    
    log_info(__LOG_DEBUG__, "Added a new client to linked list: IP: %s MAC: %s Token: %s AuthType: %d Bblink_userid:%s", ip, mac, token,iAuthType,curclient->pBblink_userid);
    return curclient;
}
t_client *client_list_find(const char *ip, const char *mac)
{
    t_client *ptr;
    int endip;
	log_info(__LOG_DEBUG__, "client_list_find, ip: %s, mac: %s", ip, mac);
    if(-1 == (endip = get_end_ip(ip)))
    {
        return NULL;
    }
    ptr = g_client_head[endip%CLIENT_HASH_SIZE].next;
    while(NULL != ptr)
    {
        if(0 == strcmp(ptr->ip, ip) && 0 == strcasecmp(ptr->mac, mac))
        {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

t_client *client_list_find_by_ip(const char *ip)
{
    t_client *ptr;
    int endip;

    if(-1 == (endip = get_end_ip(ip)))
    {
        return NULL;
    }
    ptr = g_client_head[endip%CLIENT_HASH_SIZE].next;
    while(NULL != ptr)
    {
        if(0 == strcmp(ptr->ip, ip))
        {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

t_client *client_list_find_by_mac(const char *mac)
{
    return NULL;
}

t_client *client_list_find_by_token(const char *token)
{
    return NULL;
}

void _client_list_free_node(t_client *client)
{
#ifdef HAVE_NETSNIFF
	extern void send_client_logout_msg_to_netsniff(char *ip);
	if (client->ip != NULL)
		send_client_logout_msg_to_netsniff(client->ip);
	if (client->other_ip != NULL)
		send_client_logout_msg_to_netsniff(client->other_ip);	
#endif	
    if (client->mac != NULL)
        free(client->mac);

    if (client->ip != NULL)
        free(client->ip);

    if (client->other_mac != NULL)
        free(client->other_mac);

	if (client->other_ip != NULL)
        free(client->other_ip);

    if (client->token != NULL)
        free(client->token);

    free(client);
    client = NULL;
    return;
}

void client_list_delete(t_client *client)
{
    t_client *ptr;
    int endip;

    if(NULL == client)
        return;

    if(-1 == (endip = get_end_ip(client->ip)))
    {
        log_info(__LOG_ERROR__, "clinet not found");
        return;
    }

    ptr = &g_client_head[endip%CLIENT_HASH_SIZE];
    while(NULL != ptr->next && ptr->next != client)
    {
        ptr = ptr->next;
    }
    if(NULL == ptr->next)
    {
        log_info(__LOG_ERROR__, "Node to delete could not be found.");
    }
    else
    {
        log_info(__LOG_DEBUG__, "delete client found");
        ptr->next = client->next;
        _client_list_free_node(client);
    }
    g_client_access_num--;
    log_info(__LOG_DEBUG__, "delete client completed");
    return;
}

void client_list_delete_all(void)
{
    int loop;
    int qos_enable = config_get_config()->qos_enable;
    t_client *ptr, *cur;

	sys_log_set(WIFIDOG_USR_LOG_FILE, "nf-DENY: delete-all");

    for(loop = 0; loop < CLIENT_HASH_SIZE; loop++)
    {
        ptr = &g_client_head[loop];
        while(NULL != ptr->next)
        {   
            cur = ptr->next;
            ptr->next = cur->next;
            nf_fw_access(FW_ACCESS_DENY, cur->ip, cur->mac);
            if(qos_enable)
                fw_qos_del(cur->mac);
            _client_list_free_node(cur);
        }
    }
    g_client_access_num = 0;
    return;
}

void client_list_auth_update(t_client *client)
{
	int loop;
    t_client *ptr, *cur;
	
	if(client != NULL){
		if(0 == client->in_cnt){
			g_client_access_num++;
			client->in_cnt = 1;
		}
	}

	/*删除mac重复表项*/
    for(loop = 0; loop < CLIENT_HASH_SIZE; loop++){
        ptr = &g_client_head[loop];
        while(NULL != ptr->next){   
            cur = ptr->next;
			if(!strcmp(client->mac, cur->mac)){
				if(strcmp(client->ip, cur->ip)){
					nf_fw_access(FW_ACCESS_DEL, cur->ip, cur->mac);
					ptr->next = cur->next;
					_client_list_free_node(cur);
					g_client_access_num--;
					continue;
				}
			}
			ptr = cur;
			ptr->next = cur->next;
        }
    }
}

void lock_client_list()
{
	LOCK_CLIENT_LIST();
}

void unlock_client_list()
{
	UNLOCK_CLIENT_LIST();
}


