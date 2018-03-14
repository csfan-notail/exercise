/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : client_list.h
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/11/21
  Last Modified :
  Description   : client_list.c header file
  Function List :
  History       :
  1.Date        : 2013/11/21
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/

#ifndef __CLIENT_LIST_H__
#define __CLIENT_LIST_H__
/*add@2014.8.7*/
#define COMPARE_MAC(MAC1, MAC2) ((MAC1[0] == MAC2[0])&&(MAC1[1] == MAC2[1])&&(MAC1[2] == MAC2[2])&&\
								 (MAC1[3] == MAC2[3])&&(MAC1[4] == MAC2[4])&&(MAC1[5] == MAC2[5]) )

/** @Counters struct for a client's bandwidth usage (in bytes)
 */
typedef struct _t_counters {
    unsigned long long	incoming;	        /**< @brief Incoming data total*/
    unsigned long long	outgoing;	        /**< @brief Outgoing data total*/
    unsigned long long	incoming_history;	/**< @brief Incoming data before wifidog restarted*/
    unsigned long long	outgoing_history;	/**< @brief Outgoing data before wifidog restarted*/
    //time_t	last_updated;	                /**< @brief Last update of the counters */
    long last_updated;	                /**< @brief Last update of the counters */
} t_counters;

/** @client list struct */
typedef struct _t_client {
    struct	_t_client *next;            /**< @brief Pointer to the next client */
    //time_t	login_time;
    long login_time;
	long online_time;					/**@client online time, 2014.8.4*/
	long online_time_last_update_time;  /**@client online time last update time, 2014.8.4*/
	int  auth_period;					/**@client auth period, 2014.8.14*/
    char	*ip;			            /**< @brief Client Ip address */
    char	*mac;			            /**< @brief Client Mac address */
    char	*token;			            /**< @brief Client token */
    unsigned int fw_connection_state;   /**< @brief Connection state in the firewall */
    int	fd;			                    /**< @brief Client HTTP socket (valid only during login before one of the _http_* function is called */
    t_counters	counters;	            /**< @brief Counters for input/output of the client. */
    int iAuthType; /*认证类型可能值为1?9 用户密码为1,优惠券为2,固定密码为3,手 机为4,腾讯QQ为5,新浪微博为6,无密码 为7,微信为8,支付宝为9*/ 
    char *pBblink_userid;   /* userid for bblink */
    unsigned int bw_down;
    unsigned int bw_up;
    unsigned int bw_white_down;//白名单流控
    unsigned int bw_white_up;//白名单流控
    unsigned long long	max_bw_down;	
    unsigned long long	max_bw_up;	
    unsigned long idletimeout;
    unsigned long forcetimeout;
    unsigned int id;
    unsigned int denied_client_flag;    /**@denied client flag 1: need delete it when recv response, 2014.10.10*/
	unsigned int in_cnt;
	unsigned int set_logout;
	char	*other_ip;			        /**< @brief other Client Ip address */
    char	*other_mac;			        /**< @brief other Client Mac address */
}t_client;


/** @client hash size */
#define CLIENT_HASH_SIZE    255

extern t_client g_client_head[CLIENT_HASH_SIZE];

#define LOCK_CLIENT_LIST() do { \
	/*log_info(__LOG_DEBUG__, "client_list_mutex LOCK");*/\
	pthread_mutex_lock(&client_list_mutex); \
} while (0)

#define UNLOCK_CLIENT_LIST() do { \
	/*log_info(__LOG_DEBUG__, "client_list_mutex UNLOCK");*/\
	pthread_mutex_unlock(&client_list_mutex); \
} while (0)

/** add@2014.10.11 当前接入用户数目 */
extern int g_client_access_num;
/** add@2014.10.11 云平台的当前在线用户数目*/
extern int g_cloudplatform_client_num;

/** @brief Get the first element of the list of connected clients
 */
t_client *client_get_first_client(void);

/** @brief Initializes the client list */
void client_list_init(void);

/** @brief Adds a new client to the connections list */
t_client *client_list_append(const char *ip, const char *mac, const char *token);
t_client *client_list_append2(const char *ip, const char *mac, const char *token,
									const char *other_ip, const char *other_mac);
t_client *client_list_append3(const char *ip, const char *mac, const int iAuthType,const char *pBblink_userid, const char *token);

/** @brief Finds a client by its IP and MAC */
t_client *client_list_find(const char *ip, const char *mac);

/** @brief Finds a client only by its IP */
t_client *client_list_find_by_ip(const char *ip); /* needed by firewall.c */

/** @brief Finds a client only by its Mac */
t_client *client_list_find_by_mac(const char *mac); /* needed by wdctl_thread.c */

/** @brief Finds a client by its token */
t_client *client_list_find_by_token(const char *token);

/** @brief Deletes a client from the connections list */
void client_list_delete(t_client *client);

/** @brief Deleltes all client from the connections list */
void client_list_delete_all(void);

void client_list_auth_update(t_client *client);


#endif /* __CLIENT_LIST_H__ */

