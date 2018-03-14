/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : client_rdl.c

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <syslog.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <stdarg.h>
#include <pthread.h>

#include "httpd.h"

#include "common.h"
#include "conf.h"
#include "auth_server.h"
#include "http.h"
#include "log.h"
#include "cJSON.h"
#include "util.h"
#include "client_list.h"
#include "firewall.h"
#include "redirect_portal.h"

#include <signal.h>
#include <sys/time.h>

#include "list.h"
#include "util.h"

extern int g_ext_dbg_level;
#define STA_RDL_LOG(__level__, __format__, __arg__...) \
		do{ \
			if((LOG_DEBUG == __level__) && (g_ext_dbg_level != 0))\
				syslog(LOG_INFO, "wdpriv-rdl: "__format__, ##__arg__);\
			else if(LOG_DEBUG == __level__)\
				break;\
			else\
				syslog(__level__, "wdpriv-rdl: "__format__, ##__arg__);\
		}while(0)

typedef struct
{
	unsigned char mac[6];          		/* 用户MAC */
	struct timeval time; 	  			/* 用户上次连接的时间*/
	struct hlist_node mac_Hlist;       	/* user mac hash chain */
} WD_USER_T;

/* BEGIN:fengjing add 20140626 */
#if 1/*def _BOXLL_*/
typedef struct
{
	unsigned char mac[6];          		/* 用户MAC */
	int times;                          /* 用户已访问apple网站的次数 */
	struct timeval time; 	  			/* 用户上次连接的时间*/
	struct hlist_node mac_Hlist;       	/* user mac hash chain */
} WD_USER_IOS_ACCESS_T;

/* 主要用作ios用户关联wifi自动弹portal，还能够wifi连接上。 */
#define USER_IOS_HASH_SIZE 1024
struct hlist_head g_user_ios_mac_HList[USER_IOS_HASH_SIZE];
pthread_mutex_t user_ios_list_mutex;

#define USER_IOS_LIST_LOCK()	pthread_mutex_lock(&user_ios_list_mutex)
#define USER_IOS_LIST_UNLOCK()	pthread_mutex_unlock(&user_ios_list_mutex)

/*禁止连续访问时间为g_age_time ~ g_age_time+g_check_time*/
/* MAC老化时间不能小于3秒 */
unsigned long g_ios_age_time = 15000; /*用户mac老化时间(ms)*/
long g_ios_check_time = 1000; /*扫描用户表时间间隔(ms)*/

pthread_t useriosage_pid;


/* BEGIN:fengjing add 20140626 */
#define USER_IOS_ACCESS_HASH_SIZE 1024
struct hlist_head g_user_ios_access_mac_HList[FIRST_PAGE_TASK_NUM][USER_IOS_ACCESS_HASH_SIZE];
pthread_mutex_t user_ios_access_list_mutex[FIRST_PAGE_TASK_NUM];

#define USER_IOS_ACCESS_LIST_LOCK(id)	pthread_mutex_lock(&user_ios_access_list_mutex[id])
#define USER_IOS_ACCESS_LIST_UNLOCK(id)	pthread_mutex_unlock(&user_ios_access_list_mutex[id])

/*禁止连续访问时间为g_age_time ~ g_age_time+g_check_time*/
/* MAC老化时间不能小于3秒 */
unsigned long g_ios_access_age_time = 15000; /*用户mac老化时间(ms)*/
long g_ios_access_check_time = 1000; /*扫描用户表时间间隔(ms)*/

pthread_t useriosaccessage_pid;
/* END:fengjing add 20140626 */


#endif
/* END:fengjing add 20140606 */

static inline int mac_hashfn(unsigned char *mac, int hash_size)
{
	int	x;
	int	index;
	
	x = mac[0];
	x = (x << 2) ^ mac[1];
	x = (x << 2) ^ mac[2];
	x = (x << 2) ^ mac[3];
	x = (x << 2) ^ mac[4];
	x = (x << 2) ^ mac[5];

	x ^= x >> 8;

	index = x & (hash_size - 1);

	return index;
}






/* BEGIN:fengjing add 20140606 */
#if 1/*def _BOXLL_*/

WD_USER_T *user_ios_check_by_mac(unsigned char *mac)
{
	WD_USER_T *pstuser = NULL;
	struct hlist_node *p;
	struct hlist_head *head;
	unsigned int index;
	
	index = mac_hashfn(mac, USER_IOS_HASH_SIZE);
	head = &(g_user_ios_mac_HList[index]);

	USER_IOS_LIST_LOCK();
	hlist_for_each( p, head )
	{
		pstuser = hlist_entry(p, WD_USER_T, mac_Hlist);
		if (!memcmp(pstuser->mac, mac, 6)){
			USER_IOS_LIST_UNLOCK();
			return pstuser;
		}
	}

/* 用户未找到，暂不加入 */
#if 0
	/*用户未找到，则加入用户*/
	pstuser = (WD_USER_T*)malloc(sizeof(WD_USER_T));
	memcpy(pstuser->mac, mac, 6);
	gettimeofday(&pstuser->time, NULL); 
	hlist_add_head(&(pstuser->mac_Hlist), head);
#endif

	USER_IOS_LIST_UNLOCK();
	
	return NULL;	
}

WD_USER_T *user_ios_check_by_mac_add(unsigned char *mac)
{
	WD_USER_T *pstuser = NULL;
	struct hlist_node *p;
	struct hlist_head *head;
	unsigned int index;
	
	index = mac_hashfn(mac, USER_IOS_HASH_SIZE);
	head = &(g_user_ios_mac_HList[index]);

	USER_IOS_LIST_LOCK();
	hlist_for_each( p, head )
	{
		pstuser = hlist_entry(p, WD_USER_T, mac_Hlist);
		if (!memcmp(pstuser->mac, mac, 6)){
			USER_IOS_LIST_UNLOCK();
			return pstuser;
		}
	}

	/*用户未找到，则加入用户*/
	pstuser = (WD_USER_T*)malloc(sizeof(WD_USER_T));
	memcpy(pstuser->mac, mac, 6);
	gettimeofday(&pstuser->time, NULL); 
	hlist_add_head(&(pstuser->mac_Hlist), head);

	log_info(__LOG_DEBUG__, "add user:"MAC_FMT" into user_ios_list\n", MAC_ARG(mac));	
	
	USER_IOS_LIST_UNLOCK();
	
	return NULL;	
}

static void user_ios_age_by_mac(int signo)
{
	int i = 0, age = 0;
	WD_USER_T *pstuser = NULL;
	struct hlist_node *p, *n;
	struct timeval time;
	unsigned long t_start_sec, t_start_msec;
	unsigned long t_end_sec, t_end_msec;
	unsigned long t_cost_time_sec, t_cost_time_msec;

	gettimeofday(&time, NULL); 
	t_end_sec = (unsigned long)time.tv_sec;
	t_end_msec = (unsigned long)time.tv_usec/1000;

	USER_IOS_LIST_LOCK();
	for(i = 0; i < USER_IOS_HASH_SIZE; ++i)
	{
		hlist_for_each_safe(p, n, &(g_user_ios_mac_HList[i]))
		{
			pstuser = hlist_entry(p, WD_USER_T, mac_Hlist);
			t_start_sec = (unsigned long)pstuser->time.tv_sec;
			t_start_msec = (unsigned long)pstuser->time.tv_usec/1000;

			if(t_end_sec < t_start_sec){
				age = 1;
				goto AGE_USER;
			}

			t_cost_time_sec = t_end_sec - t_start_sec;
			if(t_end_msec < t_start_msec){
				--t_cost_time_sec;
				t_end_msec += 1000;
			}
			t_cost_time_msec = t_end_msec - t_start_msec;
			

			if(t_cost_time_sec >= 4294968) /*有可能是系统时间同步前后导致的*/
				age = 1;					/*为防止32位越界，直接老化*/
			else if((t_cost_time_sec*1000+t_cost_time_msec) >= g_ios_age_time){
				age = 1;
			}
			else
				age = 0;
AGE_USER:
			if(age){
				hlist_del(&(pstuser->mac_Hlist));
				free(pstuser);				
			}
		}
	}
	USER_IOS_LIST_UNLOCK();
}

int check_user_ios_exist(char *str_mac)
{
	int user_exist = 0;
	WD_USER_T *pstuser = NULL;
	unsigned char usr_mac[6];
	
	if(strmac_to_mac(str_mac, usr_mac) < 0)
	{
		log_info(__LOG_DEBUG__, "%s is invalid mac\n", str_mac);	
		return -1;
	}

	pstuser = user_ios_check_by_mac(usr_mac);
	if(pstuser != NULL)
	{
		log_info(__LOG_DEBUG__, "user:%s exist in user_ios_list\n", str_mac);	
		user_exist = 1;
	}
	else
	{
		log_info(__LOG_DEBUG__, "user:%s not exist in user_ios_list\n", str_mac);	
	}
	return user_exist;
}

int check_user_ios_exist_add(char *str_mac)
{
	int user_exist = 0;
	WD_USER_T *pstuser = NULL;
	unsigned char usr_mac[6];
	
	if(strmac_to_mac(str_mac, usr_mac) < 0)
	{
		log_info(__LOG_DEBUG__, "%s is invalid mac\n", str_mac);	
		return -1;
	}

	pstuser = user_ios_check_by_mac_add(usr_mac);
	if(pstuser != NULL)
	{
		log_info(__LOG_DEBUG__, "user:%s exist in user_ios_list\n", str_mac);	
		user_exist = 1;
	}
	else
	{	
		log_info(__LOG_DEBUG__, "user:%s exist in user_ios_list\n", str_mac);	
	}
	return user_exist;
}

void thread_user_ios_age(void)
{
	while(1)
	{
		user_ios_age_by_mac(0);
		usleep(g_ios_check_time*1000);
	}
    exit(1);
    return;
}

int user_http_ios_init(void)
{
	int i;
	
	pthread_mutex_init(&user_ios_list_mutex, NULL);

	for(i = 0; i < USER_IOS_HASH_SIZE; ++i){	
		INIT_HLIST_HEAD(&g_user_ios_mac_HList[i]);
	}
	
	if(pthread_create(&useriosage_pid, NULL, (void *)thread_user_ios_age, NULL) != 0){
        log_info(__LOG_ERROR__, "create useriosage_pid failed: %s", strerror(errno));
	}
	STA_RDL_LOG(LOG_INFO, "http ios enabled, age-time %ld ms",
					g_ios_age_time);
	
	return 0;
}


/* BEGIN:fengjing add 20140626 */
WD_USER_IOS_ACCESS_T *user_ios_access_check_by_mac(int loop, unsigned char *mac)
{
	WD_USER_IOS_ACCESS_T *pstuser = NULL;
	struct hlist_node *p;
	struct hlist_head *head;
	unsigned int index;
	
	index = mac_hashfn(mac, USER_IOS_ACCESS_HASH_SIZE);
	head = &(g_user_ios_access_mac_HList[loop][index]);

	USER_IOS_ACCESS_LIST_LOCK(loop);
	hlist_for_each( p, head )
	{
		pstuser = hlist_entry(p, WD_USER_IOS_ACCESS_T, mac_Hlist);
		if (!memcmp(pstuser->mac, mac, 6)){
			/* 次数加1，如果次数达到3次，则放行该ios终端 */
			pstuser->times++;
			if (pstuser->times>=2)
			{
				/*删除此用户*/
				hlist_del(&(pstuser->mac_Hlist));
				USER_IOS_ACCESS_LIST_UNLOCK(loop);
				return pstuser;
			}
			else
			{
				USER_IOS_ACCESS_LIST_UNLOCK(loop);
				return NULL;
			}
		}
	}

	/*用户未找到，则加入用户*/
	pstuser = (WD_USER_IOS_ACCESS_T*)malloc(sizeof(WD_USER_IOS_ACCESS_T));
	memcpy(pstuser->mac, mac, 6);
	pstuser->times = 0;
	gettimeofday(&pstuser->time, NULL); 
	hlist_add_head(&(pstuser->mac_Hlist), head);

	USER_IOS_ACCESS_LIST_UNLOCK(loop);
	
	return NULL;	
}

static void user_ios_access_age_by_mac(int signo)
{
	int i = 0, age = 0, loop;
	WD_USER_IOS_ACCESS_T *pstuser = NULL;
	struct hlist_node *p, *n;
	struct timeval time;
	unsigned long t_start_sec, t_start_msec;
	unsigned long t_end_sec, t_end_msec;
	unsigned long t_cost_time_sec, t_cost_time_msec;

	gettimeofday(&time, NULL); 
	t_end_sec = (unsigned long)time.tv_sec;
	t_end_msec = (unsigned long)time.tv_usec/1000;

	for(loop=0; loop<FIRST_PAGE_TASK_NUM; ++loop)
	{
		USER_IOS_ACCESS_LIST_LOCK(loop);
		for(i = 0; i < USER_IOS_ACCESS_HASH_SIZE; ++i)
		{
			hlist_for_each_safe(p, n, &(g_user_ios_access_mac_HList[loop][i]))
			{
				pstuser = hlist_entry(p, WD_USER_IOS_ACCESS_T, mac_Hlist);
				t_start_sec = (unsigned long)pstuser->time.tv_sec;
				t_start_msec = (unsigned long)pstuser->time.tv_usec/1000;

				if(t_end_sec < t_start_sec){
					age = 1;
					goto AGE_USER;
				}

				t_cost_time_sec = t_end_sec - t_start_sec;
				if(t_end_msec < t_start_msec){
					--t_cost_time_sec;
					t_end_msec += 1000;
				}
				t_cost_time_msec = t_end_msec - t_start_msec;
				

				if(t_cost_time_sec >= 4294968) /*有可能是系统时间同步前后导致的*/
					age = 1;					/*为防止32位越界，直接老化*/
				else if((t_cost_time_sec*1000+t_cost_time_msec) >= g_ios_access_age_time){
					age = 1;
				}
				else
					age = 0;
AGE_USER:
				if(age){
					hlist_del(&(pstuser->mac_Hlist));
					free(pstuser);				
				}
			}
		}
		USER_IOS_ACCESS_LIST_UNLOCK(loop);
	}
}

int check_user_ios_access_exist(int loop, char *str_mac)
{
	int user_exist = 0;
	WD_USER_IOS_ACCESS_T *pstuser = NULL;
	unsigned char usr_mac[6];
	
	if(strmac_to_mac(str_mac, usr_mac) < 0)
	{
		log_info(__LOG_DEBUG__, "%s is invalid mac\n", str_mac);	
		return -1;
	}

	pstuser = user_ios_access_check_by_mac(loop, usr_mac);
	if(pstuser != NULL)
	{
		log_info(__LOG_DEBUG__, "user:%s exist in user_ios_access_list\n", str_mac);	
		free(pstuser);
		user_exist = 1;
	}
	else
	{
		log_info(__LOG_DEBUG__, "user:%s not exist in user_ios_access_list\n", str_mac);	
	}
	return user_exist;
}

void thread_user_ios_access_age(void)
{
	while(1)
	{
		user_ios_access_age_by_mac(0);
		usleep(g_ios_access_check_time*1000);
	}
    exit(1);
    return;
}

int user_http_ios_access_init(void)
{
	int i, j;

	for(i = 0; i < FIRST_PAGE_TASK_NUM; ++i)
		pthread_mutex_init(&user_ios_access_list_mutex[i], NULL);

	for(i = 0; i < FIRST_PAGE_TASK_NUM; ++i){
		for(j = 0; j < USER_IOS_ACCESS_HASH_SIZE; ++j){	
			INIT_HLIST_HEAD(&g_user_ios_access_mac_HList[i][j]);
		}
	}
	
	if(pthread_create(&useriosaccessage_pid, NULL, (void *)thread_user_ios_access_age, NULL) != 0){
        log_info(__LOG_ERROR__, "create useriosaccessage_pid failed: %s", strerror(errno));
	}
	pthread_detach(useriosaccessage_pid);
	STA_RDL_LOG(LOG_INFO, "http ios enabled, age-time %ld ms",
					g_ios_access_age_time);
	
	return 0;
}
/* END:fengjing add 20140626 */

#endif
	
/* END:fengjing add 20140606 */
	

