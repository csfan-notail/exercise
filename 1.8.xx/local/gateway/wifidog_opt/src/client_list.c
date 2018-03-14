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
#include <ctype.h>

#include "util.h"
#include "log.h"
#include "conf.h"
#include "firewall.h"
#include "client_list.h"

/** @定义终端列表全局变量 */
t_client g_client_head[CLIENT_HASH_SIZE];

roam_client g_roam_client_head[CLIENT_HASH_SIZE];/*网间漫游*/


/** @ 当前接入用户数目 */
int g_client_access_num = 0;
/** add@2014.10.11 云平台的当前在线用户数目*/
int g_cloudplatform_client_num = 0;
/** @傻瓜流控开关与总带宽*/
int	 lan_bandwidth_switch = 0;
int	 lan_bandwidth = 0;

/** Global mutex to protect access to the client list */
pthread_mutex_t client_list_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t roam_client_list_mutex = PTHREAD_MUTEX_INITIALIZER;

CLIENT_AUTH_TYPE_T gstClientAuthType[CLIENT_AUTH_MAX] =
{
   {CLIENT_AUTH_FREE,"1029999","Free Auth"},
   {CLIENT_AUTH_USER_COUNT,"1029999","User Count"},
   {CLIENT_AUTH_PHONE_MESSAGE,"1020004","Phone Num"},
   {CLIENT_AUTH_WEICHAT_PUBILC_NUM,"1020100","WeiXinPublic"},
   {CLIENT_AUTH_QQ,"1020100","QQ_NUM"},
   {CLIENT_AUTH_WEI_BO,"1020100","WeiBo"},
   {CLIENT_AUTH_RESVER_1,"1029999","Rev_1"},
   {CLIENT_AUTH_WEIXIN_FAST,"1020100","WeiXinFastAuth"},
   {CLIENT_AUTH_RESVER_2,"1029999","Rev_2"},
   {CLIENT_AUTH_RESVER_3,"1029999","Rev_3"},
   {CLIENT_AUTH_RESVER_4,"1029999","Rev_4"},
   {CLIENT_AUTH_RESVER_5,"1029999","Rev_5"},
   {CLIENT_AUTH_ALIPAY,"1020100","ALiPay"},
   {CLIENT_AUTH_WEIXIN_SAOSAO,"1020100","WeiXinSaoSao"},
};
extern int wifidog_send_onoff_client_to_audit(fw_access_t eType,const char *pStaMac,t_client *pstClient);

void client_list_init(void)
{	
	char nvram_value[32];
	
    memset(g_client_head, 0, sizeof(t_client)*CLIENT_HASH_SIZE);
    memset(g_roam_client_head,0,sizeof(roam_client)*CLIENT_HASH_SIZE);
	
	sys_config_get(nvram_value, "lan_bandwidth_switch", 32);
	lan_bandwidth_switch = atoi(nvram_value);
	
	sys_config_get(nvram_value, "lan_bandwidth", 32);
	lan_bandwidth = atoi(nvram_value);
}

extern int sta_add_by_mac(unsigned char *mac, unsigned char *ap_mac, char *ssid);

t_client *client_list_append(const char *ip, char *mac, const char *token,const char *method,ADD_CLIENT_OTHERINFO_T *pstOtherInfo)
{
    t_client *curclient, *prevclient;
    int iHash=0;
	long now = get_uptime();
	int loop, len;

	/*将mac小写转换为大写*/
	len = strlen(mac);
    for(loop = 0; loop < len; loop++)
        mac[loop] = toupper(mac[loop]);

    iHash = client_mac_hash(mac);

    prevclient = NULL;
    curclient = &g_client_head[iHash];
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
	curclient->bw_qos_sta = 0;	/*默认使用全局流控配置*/
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
	curclient->set_logout = 0;
	curclient->access_cnt = 0;

     /*begin:for audit 2015-9-28*/
    if(NULL != pstOtherInfo)
    {
        memcpy(curclient->stAuthInfo.aucUserCount,pstOtherInfo->stAuthInfo.aucUserCount,32);
        curclient->stAuthInfo.uiAuthType = pstOtherInfo->stAuthInfo.uiAuthType;
    }
    /*end:for audit 2015-9-28*/

    prevclient->next = curclient;
    if(NULL != method)
    {
        curclient->method_type = safe_strdup(method);
    }
    else
    {
        curclient->method_type = NULL;
    }
    #ifdef PCAP_SUPPORT
    curclient->qq_id_list = NULL;
    #endif
    /*g_client_access_num++;*//*改为认证过后再计入总数*/
    log_info(__LOG_DEBUG__, "Added a new client to linked list: IP: %s MAC: %s Token: %s", ip, mac, token);
    return curclient;
}

t_client *client_list_append2(const char *ip, char *mac, const char *token,
									const char *other_ip, const char *other_mac,const char *method,ADD_CLIENT_OTHERINFO_T *pstOtherInfo)
{
    t_client *curclient, *prevclient;
    int iHash=0;
	long now = get_uptime();
	int loop, len;

	/*将mac小写转换为大写*/
	len = strlen(mac);
    for(loop = 0; loop < len; loop++)
        mac[loop] = toupper(mac[loop]);

    iHash = client_mac_hash(mac);

    prevclient = NULL;
    curclient = &g_client_head[iHash];
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
	curclient->bw_qos_sta = 0;	/*默认使用全局流控配置*/
	curclient->max_bw_down = 0;
	curclient->max_bw_up = 0;
	curclient->idletimeout = 0;
	curclient->forcetimeout = 0;
	curclient->online_time = 0;
	curclient->online_time_last_update_time = now;
	curclient->auth_period = now;
    curclient->denied_client_flag = 0;
	curclient->access_cnt = 0;
	/*end: add @2014.8.4*/


    if(NULL != method)
    {
        curclient->method_type = safe_strdup(method);
    }
    else
    {
        curclient->method_type = NULL;
    }

    #ifdef PCAP_SUPPORT
    curclient->qq_id_list = NULL;
    #endif

    curclient->next = NULL;
	curclient->set_logout = 0;
     /*begin:for audit 2015-9-28*/
    if(NULL != pstOtherInfo)
    {
        memcpy(curclient->stAuthInfo.aucUserCount,pstOtherInfo->stAuthInfo.aucUserCount,32);
        curclient->stAuthInfo.uiAuthType = pstOtherInfo->stAuthInfo.uiAuthType;
    }
     /*begin:for audit 2015-9-28*/
    prevclient->next = curclient;

    /*g_client_access_num++;*//*改为认证过后再计入总数*/
    log_info(__LOG_DEBUG__, "Added a new client to linked list: IP: %s MAC: %s Token: %s", ip, mac, token);
    return curclient;
}


t_client *client_list_find(const char *ip, const char *mac)
{
    t_client *curclient=NULL;
    int iHash=0;

    iHash = client_mac_hash(mac);

    curclient = g_client_head[iHash].next;
    while(NULL != curclient)
    {
        if((0 == strcasecmp(curclient->mac, mac))&&(0 == strcmp(curclient->ip, ip)))
        {
            return curclient;
        }
        curclient = curclient->next;
    }
    return NULL;
}





t_client *client_list_find_by_ip(const char *ip)
{
    t_client *curclient=NULL;
    int iLoop=0;

    for(iLoop=0; iLoop < CLIENT_HASH_SIZE; iLoop++)
    {
        curclient = g_client_head[iLoop].next;
        while(NULL != curclient)
        {
            if(0 == strcmp(curclient->ip, ip))
            {
                return curclient;
            }
            curclient = curclient->next;
        }
    }
    return NULL;
}

t_client *client_list_find_by_mac(const char *mac)
{
    t_client *curclient=NULL;
    int iHash=0;

    iHash = client_mac_hash(mac);

    curclient = g_client_head[iHash].next;
    while(NULL != curclient)
    {
        if(0 == strcasecmp(curclient->mac, mac))
        {
            return curclient;
        }
        curclient = curclient->next;
    }
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

    if(NULL != client->method_type)
    {
        free(client->method_type);
        client->method_type = NULL;
    }

    #ifdef PCAP_SUPPORT
    if(NULL != client->qq_id_list)
    {
        free(client->qq_id_list);
        client->qq_id_list = NULL;
    }
    #endif

    free(client);
    client = NULL;
    return;
}

void client_list_delete(t_client *client)
{
    t_client *curclient=NULL;
    int iHash=0;
	int	 qos_enable = config_get_config()->qos_enable;
    iHash = client_mac_hash(client->mac);

    curclient = &g_client_head[iHash];
    while(NULL != curclient->next)
    {
        if(curclient->next == client)
        {
            curclient->next=client->next;
			if(qos_enable||lan_bandwidth_switch)
				fw_qos_del(client->mac);
            _client_list_free_node(client);
			client_list_up_allqos();
            g_client_access_num--;
            log_info(__LOG_DEBUG__, "delete client found");
            return;
        }
        curclient = curclient->next;
    }
    log_info(__LOG_ERROR__, "Node to delete could not be found.");
    return;
}

void client_change_fw_connection_state(t_client *client,int status,char *pIp)
{
    if(NULL == client)
    {
        log_info(__LOG_ERROR__, "client is NULL return\n");
        return;
    }
    client->fw_connection_state = status;
    if(pIp)
    {
        if(client->ip)
        {
            free(client->ip);
            client->ip = NULL;
            client->ip = safe_strdup(pIp);
        }
    }
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
			nf_fw_access(FW_ACCESS_DENY, cur->mac, NULL);
            ptr->next = cur->next;
            
            if(qos_enable||lan_bandwidth_switch)
			{
				fw_qos_del(cur->mac);	
			}
            _client_list_free_node(cur);
        }
    }
    g_client_access_num = 0;
    return;
}
void client_ip_update(t_client *client,char *ip)
{
    if(NULL == client)
    {
        return;
    }
    if(NULL != client->ip && 0 != strcmp(client->ip,ip))
    {
        free(client->ip);
        client->ip=NULL;
    }
    if(NULL == client->ip)
    {
        client->ip = safe_strdup(ip);
    }
    return;
}


//本地傻瓜流控，在云流控生效时无效
void client_list_up_allqos(void)
{		
	int	 loop;
	int	 ave_bandwidth = -1;
	t_client *ptr;
	
	if(config_get_config()->qos_enable)
		return;
		
	if((lan_bandwidth_switch == 1) && (g_client_access_num > 0))
	{	
		ave_bandwidth =(int) (lan_bandwidth*1000/g_client_access_num);
		for(loop = 0; loop < CLIENT_HASH_SIZE; loop++)
		{
			ptr = g_client_head[loop].next;
			while(NULL != ptr)
			{
				fw_qos_del(ptr->mac);
				fw_qos_add(ptr->mac, ave_bandwidth, ave_bandwidth);
				ptr->bw_up = ave_bandwidth;
				ptr->bw_down = ave_bandwidth;
				
				ptr = ptr->next;
			}
		}
	}
	return;	   
}

