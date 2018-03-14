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
#define HTTP_MAC_LEN        18

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


typedef enum
{
    CLIENT_AUTH_FREE =0,/*����֤*/
    CLIENT_AUTH_USER_COUNT,/*1 �û��˺���֤*/
    CLIENT_AUTH_PHONE_MESSAGE,/*2 ������֤*/
    CLIENT_AUTH_WEICHAT_PUBILC_NUM,/*3 ΢�Ź��ں�*/
    CLIENT_AUTH_QQ,/*4 QQ��֤*/
    CLIENT_AUTH_WEI_BO,/*5 ΢��*/
    CLIENT_AUTH_RESVER_1,/*6 */
    CLIENT_AUTH_WEIXIN_FAST,/*7 ΢�ſ�����֤*/
    CLIENT_AUTH_RESVER_2,/*8*/
    CLIENT_AUTH_RESVER_3,/*9*/
    CLIENT_AUTH_RESVER_4,/*10*/
    CLIENT_AUTH_RESVER_5,/*11*/
    CLIENT_AUTH_ALIPAY,/*12 ֧����*/
    CLIENT_AUTH_WEIXIN_SAOSAO,/*13 ΢��ɨɨ*/
    CLIENT_AUTH_MAX
}CLIENT_AUTH_TYPE_E;


typedef struct
{
    CLIENT_AUTH_TYPE_E eType;
    char *pAuthTypeCode;
    char *pDefaultUserCount;
}CLIENT_AUTH_TYPE_T;


typedef struct 
{
    CLIENT_AUTH_TYPE_E uiAuthType;
    char aucUserCount[32];
}CLIENT_AUTH_INFO_T;

typedef struct
{
    CLIENT_AUTH_INFO_T stAuthInfo;
}ADD_CLIENT_OTHERINFO_T;
enum {
    FLAG_IP_GET=0,
    FLAG_IP_INVALID,
    FLAG_IP_MAX
}CLIENT_IPFLAG_E;

typedef enum
{
    CLIENT_GET_APINFO_ERROR,
    CLIENT_GET_APINFO_OK
}CLIENT_CONNECT_APINFO_E;

typedef struct
{
    int uiRssi;
    CLIENT_CONNECT_APINFO_E eGetInfoStatus;/*�Ƿ��ȡ��������Ϣ*/
    unsigned char aucApMac[6];/*client �����豸��mac��ַ*/
    char aucSsid[32];/*�����ssid��Ϣ*/
}CLIENT_CONNECT_APINFO_T;

/*����ԭ��*/
typedef enum
{
    CLIENT_LOGOUT_CCS = 0,
    CLIENT_LOGOUT_SPAN_SSID,
    CLIENT_LOGOUT_MAX
}CLIENT_LOGOUT_TYPE_E;


/** @client list struct */
typedef struct _t_client {
    struct	_t_client *next;            /**< @brief Pointer to the next client */
    //time_t	login_time;
    long login_time;
    long login_time_t;/*����ʱ��,��1970������*/
	long online_time;					/**@client online time, 2014.8.4*/
	long online_time_last_update_time;  /**@client online time last update time, 2014.8.4*/
	int  auth_period;					/**@client auth period, 2014.8.14*/
	int  iIpFlag;					    /**@client ip flag, 2015.10.14*/
    char	*ip;			            /**< @brief Client Ip address */
    char	*mac;			            /**< @brief Client Mac address */
    char	*token;			            /**< @brief Client token */
    int fw_connection_state;   			/**< @brief Connection state in the firewall */
    int	fd;			                    /**< @brief Client HTTP socket (valid only during login before one of the _http_* function is called */
    t_counters	counters;	            /**< @brief Counters for input/output of the client. */
    unsigned int bw_down;
    unsigned int bw_up;
    unsigned long long	max_bw_down;	
    unsigned long long	max_bw_up;	
    unsigned long idletimeout;
    unsigned long forcetimeout;
    unsigned int id;
    unsigned int denied_client_flag;    /**@denied client flag 1: need delete it when recv response, 2014.10.10*/
	unsigned int set_logout;
	char	*other_ip;			        /**< @brief other Client Ip address */
    char	*other_mac;			        /**< @brief other Client Mac address */
	char *method_type;/*�ն���֤��ʽ*/
	CLIENT_AUTH_INFO_T stAuthInfo;/*��֤��Ϣ*/
	CLIENT_CONNECT_APINFO_T stClientConnectInfo;
#ifdef PCAP_SUPPORT
	char *qq_id_list;
#endif
	int bw_qos_sta; /*�Ƿ�ʹ���ն˼�������*/
	unsigned int access_cnt;
}t_client;

/*begin:2015-8-28 ��������*/
typedef enum
{
    WD_STA_ROAMING_BEGIN,
    WD_STA_ROAMING_DETECT,/*��⵽�����ն˵�����;����δ����*/
    WD_STA_ROAMING_ALLOW,/*���������ն�*/
    WD_STA_ROAMING_MAX
}ROAMING_CLIENT_E;


typedef struct _roam_client
{
    struct	_roam_client *next; 
    ROAMING_CLIENT_E eFlag;
    char aucIntMac[6];
    char aucIntIp[4];
    char *token;
    long online_time;
    unsigned long long	incoming;	        /**< @brief Incoming data total*/
    unsigned long long	outgoing;
}roam_client;
/*end:2015-8-28 ��������*/

/** @client hash size */
#define CLIENT_HASH_SIZE    255

extern t_client g_client_head[CLIENT_HASH_SIZE];
extern roam_client g_roam_client_head[CLIENT_HASH_SIZE];

#define lock_info(level, format, arg...) do{ \
if (level <= g_debug_level) \
{\
	if(__LOG_DEBUG__ == level)\
		syslog(LOG_INFO, "[%s]LOCK_INFO[%s][%d] "format"\n", g_threadname[wifidog_get_thread_index((unsigned int)(pthread_self()))].name, __FUNCTION__, __LINE__, ##arg);\
	else\
		syslog(level, "[%s]LOCK_INFO[%s][%d] "format"\n", g_threadname[wifidog_get_thread_index((unsigned int)(pthread_self()))].name, __FUNCTION__, __LINE__, ##arg);\
}\
if (!g_daemon) \
{ \
	printf("[%s]LOCK_INFO[%s][%d] "format"\n", g_threadname[wifidog_get_thread_index((unsigned int)(pthread_self()))].name, __FUNCTION__, __LINE__, ##arg);\
}\
}while(0)


extern pthread_mutex_t client_list_mutex;

#define LOCK_CLIENT_LIST() do { \
	/*sys_log_set(WIFIDOG_LOCK_LOG, "client-lock: [%s]%s:%d", g_threadname[wifidog_get_thread_index((unsigned int)(pthread_self()))].name, __FUNCTION__, __LINE__)*/; \
	pthread_mutex_lock(&client_list_mutex); \
} while (0)

#define UNLOCK_CLIENT_LIST() do { \
	/*sys_log_set(WIFIDOG_LOCK_LOG, "client-unlock: [%s]%s:%d", g_threadname[wifidog_get_thread_index((unsigned int)(pthread_self()))].name, __FUNCTION__, __LINE__)*/; \
	pthread_mutex_unlock(&client_list_mutex);\
} while (0)

#define LOCK_ROAM_CLIENT_LIST() do { \
	pthread_mutex_lock(&roam_client_list_mutex); \
} while (0)

#define UNLOCK_ROAM_CLIENT_LIST() do { \
	pthread_mutex_unlock(&roam_client_list_mutex); \
} while (0)





/** add@2014.10.11 ��ǰ�����û���Ŀ */
extern int g_client_access_num;
/** add@2014.10.11 ��ƽ̨�ĵ�ǰ�����û���Ŀ*/
extern int g_cloudplatform_client_num;

/** @brief Get the first element of the list of connected clients
 */
t_client *client_get_first_client(void);

/** @brief Initializes the client list */
void client_list_init(void);

/** @brief Adds a new client to the connections list */
t_client *client_list_append(const char *ip, char *mac, const char *token,const char *method,ADD_CLIENT_OTHERINFO_T *pstOtherInfo);
t_client *client_list_append2(const char *ip, char *mac, const char *token,
									const char *other_ip, const char *other_mac,const char *method,ADD_CLIENT_OTHERINFO_T *pstOtherInfo);

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
void client_ip_update(t_client *client,char *ip);

void client_list_up_allqos(void);

void client_change_fw_connection_state(t_client *client,int status,char *pIp);



#endif /* __CLIENT_LIST_H__ */

