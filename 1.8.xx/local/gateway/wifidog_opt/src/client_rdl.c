/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : client_rdl.c

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <syslog.h>
#include <errno.h>

#include "httpd.h"
#include "common.h"
#include "conf.h"
#include "log.h"
#include "redirect_portal.h"
#include "list.h"
#include "util.h"
#include "lib_pa.h"
#include "client_rdl.h"
#include "client_ap.h"

struct hlist_head g_user_ios_access_mac_HList[FIRST_PAGE_TASK_NUM][USER_IOS_ACCESS_HASH_SIZE];
pthread_mutex_t user_ios_access_list_mutex[FIRST_PAGE_TASK_NUM];

/*禁止连续访问时间为g_age_time ~ g_age_time+g_check_time*/
/* MAC老化时间不能小于3秒 */
unsigned long g_ios_access_age_time = 360000; /*用户mac老化时间(ms)*/
unsigned long g_ios_access_timeout_time = 20000; /*mozilla访问portal超时时间*/
unsigned long g_ios_access_timeout_time_m = 300000; /*弹出portal后默认切回INIT状态超时时间*/
unsigned long g_ios_network_access_timeout_time = 8000; /*network访问portal超时时间*/

pthread_t useriosaccessage_pid;

struct hlist_head g_user_acl_HList[USER_ACL_LIST_HASH_SIZE];
pthread_mutex_t user_acl_list_mutex;

struct hlist_head g_user_http_record_HList[FIRST_PAGE_TASK_NUM][USER_HTTP_RECORD_HASH_SIZE];
pthread_mutex_t user_http_record_mutex[FIRST_PAGE_TASK_NUM];


inline int user_timeout_check(struct timeval *pstOldtime,unsigned long ulTimeout)
{
	struct timeval time;
	unsigned long t_start_sec, t_start_msec;
	unsigned long t_end_sec, t_end_msec;
	unsigned long t_cost_time_sec, t_cost_time_msec;

	gettimeofday(&time, NULL); 
	t_end_sec = (unsigned long)time.tv_sec;
	t_end_msec = (unsigned long)time.tv_usec/1000;
    t_start_sec = (unsigned long)pstOldtime->tv_sec;
    t_start_msec = (unsigned long)pstOldtime->tv_usec/1000;

    if(t_end_sec < t_start_sec){
    	return TRUE;
    }

    t_cost_time_sec = t_end_sec - t_start_sec;
    if(t_end_msec < t_start_msec){
    	--t_cost_time_sec;
    	t_end_msec += 1000;
    }
    t_cost_time_msec = t_end_msec - t_start_msec;


    if(t_cost_time_sec >= 4294968) /*有可能是系统时间同步前后导致的,为防止32位越界，直接老化*/
    {
        return TRUE;
    }			
    else if((t_cost_time_sec*1000+t_cost_time_msec) >= ulTimeout)
    {
        return TRUE;
    }
    return FALSE;
}
void firstnetwork_init_ioshlist(WD_USER_IOS_ACCESS_T *pstuser)
{
    pstuser->duration = g_ios_access_timeout_time_m;
    pstuser->check = CHECK_INIT;
    memset(&pstuser->init_checktime,0,sizeof(pstuser->init_checktime));    
}

/* BEGIN:wbr add 20150706 */
WD_USER_IOS_ACCESS_T *user_ios_access_get_by_mac(int loop,unsigned char *mac,int flag)
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
		if (0 == memcmp(pstuser->mac, mac, 6))
        {	
        	/*为保证状态记录完整性，
        			避免在1\2\3流程中表项被老化，更新time*/
        	if (CHECK_INIT != pstuser->check)		
        	{
                gettimeofday(&pstuser->time, NULL); //避免只弹network的无法超时
        	}
			USER_IOS_ACCESS_LIST_UNLOCK(loop);
			return pstuser;
		}
	}
    if(CHECK_USER_GET == flag)
    {
		USER_IOS_ACCESS_LIST_UNLOCK(loop);
        log_info(__LOG_DEBUG__, "[wbr]CHECK_USER_GET");
    	return NULL;	
    }
	/*用户未找到，则加入用户*/
	pstuser = (WD_USER_IOS_ACCESS_T*)malloc(sizeof(WD_USER_IOS_ACCESS_T));
    if(NULL == pstuser)
    {
		log_info(__LOG_ERROR__, "mallock error\n");	
		USER_IOS_ACCESS_LIST_UNLOCK(loop);
        return NULL;
    }
    firstnetwork_init_ioshlist(pstuser);
	memcpy(pstuser->mac, mac, 6);
	gettimeofday(&pstuser->time, NULL); 
	pstuser->m_times  = 0;
	pstuser->n_times  = 0;
	pstuser->bitmap   = 0;
	hlist_add_head(&(pstuser->mac_Hlist), head);
    log_info(__LOG_DEBUG__, "[wbr]CHECK_USER_INIT");
	USER_IOS_ACCESS_LIST_UNLOCK(loop);
	return pstuser;	
}

static void user_ios_access_age(void)
{
	int i = 0, loop;
	WD_USER_IOS_ACCESS_T *pstuser = NULL;
	struct hlist_node *p, *n;

	for(loop=0; loop<FIRST_PAGE_TASK_NUM; ++loop)
	{
		USER_IOS_ACCESS_LIST_LOCK(loop);
		for(i = 0; i < USER_IOS_ACCESS_HASH_SIZE; ++i)
		{
			hlist_for_each_safe(p, n, &(g_user_ios_access_mac_HList[loop][i]))
			{
				pstuser = hlist_entry(p, WD_USER_IOS_ACCESS_T, mac_Hlist);
                if(TRUE == user_timeout_check(&pstuser->time, g_ios_access_age_time))
                {
					hlist_del(&(pstuser->mac_Hlist));
					free(pstuser);				
                }
			}
		}
		USER_IOS_ACCESS_LIST_UNLOCK(loop);
	}
}


int user_acl_add_by_mac(unsigned char *mac, unsigned long duration, int type)
{
	WD_USER_ACL_T *pstuser = NULL;
	struct hlist_node *p;
	struct hlist_head *head;
	unsigned int index;

	sys_log_set(WIFIDOG_HTTP_LOG_FILE, "Add User Acl: Mac("MAC_FMT") Duration(%d) Type(%s)", 
		MAC_ARG(mac), duration, ((USER_ACL_BLACK == type) ? "black" : "white"));

	if(USER_ACL_BLACK == type){
		if (0 != portal_add_mac_black_list(mac))
	        log_info(__LOG_ERROR__, "portal_add_mac_black_list failed");
	}
	else{
		if (0 != portal_add_mac_free_auth(mac))
	        log_info(__LOG_ERROR__, "portal_add_mac_free_auth failed");		
	}

	index = mac_hashfn(mac, USER_ACL_LIST_HASH_SIZE);
	head = &(g_user_acl_HList[index]);

	USER_ACL_LIST_LOCK();
	hlist_for_each( p, head )
	{
		pstuser = hlist_entry(p, WD_USER_ACL_T, mac_Hlist);
		if (0 == memcmp(pstuser->mac, mac, 6))
        {	
        	if(pstuser->type == type){
	        	/*用户已存在，则更新时间*/
	        	gettimeofday(&pstuser->time, NULL); 
				pstuser->duration = duration;
				USER_ACL_LIST_UNLOCK();
				return 0;
        	}
		}
	}

	/*用户未找到，则加入用户*/
	pstuser = (WD_USER_ACL_T*)malloc(sizeof(WD_USER_ACL_T));
    if(NULL == pstuser)
    {
		log_info(__LOG_ERROR__, "malloc error\n");	
		USER_ACL_LIST_UNLOCK();
        return -1;
    }
	memcpy(pstuser->mac, mac, 6);
	gettimeofday(&pstuser->time, NULL); 
	pstuser->duration = duration;
	pstuser->type = type;
	hlist_add_head(&(pstuser->mac_Hlist), head);
	USER_ACL_LIST_UNLOCK();
	
	return 0;	
}

WD_USER_ACL_T * user_acl_get_by_mac(unsigned char *mac, int type)
{
	WD_USER_ACL_T *pstuser = NULL;
	struct hlist_node *p;
	struct hlist_head *head;
	unsigned int index;
	
	index = mac_hashfn(mac, USER_ACL_LIST_HASH_SIZE);
	head = &(g_user_acl_HList[index]);

	USER_ACL_LIST_LOCK();
	hlist_for_each( p, head )
	{
		pstuser = hlist_entry(p, WD_USER_ACL_T, mac_Hlist);
		if (0 == memcmp(pstuser->mac, mac, 6))
        {	
        	if(pstuser->type == type){
				USER_ACL_LIST_UNLOCK();
				return pstuser;
        	}
		}
	}
	USER_ACL_LIST_UNLOCK();
	
	return NULL;	
}

void user_acl_restore(void)
{
	int i = 0;
	WD_USER_ACL_T *pstuser = NULL;
	struct hlist_node *p, *n;

	USER_ACL_LIST_LOCK();
	for(i = 0; i < USER_ACL_LIST_HASH_SIZE; ++i)
	{
		hlist_for_each_safe(p, n, &(g_user_acl_HList[i]))
		{
			pstuser = hlist_entry(p, WD_USER_ACL_T, mac_Hlist);

			sys_log_set(WIFIDOG_HTTP_LOG_FILE, "Restore User Acl: Mac("MAC_FMT") Duration(%d) Type(%s)", 
				MAC_ARG(pstuser->mac), pstuser->duration, ((USER_ACL_BLACK == pstuser->type) ? "black" : "white"));

			if(USER_ACL_BLACK == pstuser->type){
				if (0 != portal_add_mac_black_list(pstuser->mac))
					log_info(__LOG_ERROR__, "portal_add_mac_black_list failed");
			}
			/*else{
				if (0 != portal_add_mac_free_auth(pstuser->mac))
					log_info(__LOG_ERROR__, "portal_add_mac_free_auth failed");
			}*//*免认证白名单已改为增量配置，不需要restore了*/
		}
	}
	USER_ACL_LIST_UNLOCK();
}


static void user_acl_age(void)
{
	int i = 0;
	WD_USER_ACL_T *pstuser = NULL;
	struct hlist_node *p, *n;

	USER_ACL_LIST_LOCK();
	for(i = 0; i < USER_ACL_LIST_HASH_SIZE; ++i)
	{
		hlist_for_each_safe(p, n, &(g_user_acl_HList[i]))
		{
			pstuser = hlist_entry(p, WD_USER_ACL_T, mac_Hlist);
            if(TRUE == user_timeout_check(&pstuser->time, pstuser->duration))
            {
				sys_log_set(WIFIDOG_HTTP_LOG_FILE, "Del User Acl: Mac("MAC_FMT") Duration(%d) Type(%s)", 
					MAC_ARG(pstuser->mac), pstuser->duration, ((USER_ACL_BLACK == pstuser->type) ? "black" : "white"));
				hlist_del(&(pstuser->mac_Hlist));

				if(USER_ACL_BLACK == pstuser->type){
					if (0 != portal_del_mac_black_list(pstuser->mac))
						log_info(__LOG_ERROR__, "portal_del_mac_black_list failed");
				}
				else{
					if (0 != portal_del_mac_free_auth(pstuser->mac))
						log_info(__LOG_ERROR__, "portal_del_mac_free_auth failed");
				}

				free(pstuser);
            }
		}
	}
	USER_ACL_LIST_UNLOCK();

}

int user_http_record_add_by_mac(unsigned char *mac, int id)
{
	WD_USER_HTTP_RECORD_T *pstuser = NULL;
	struct hlist_node *p;
	struct hlist_head *head;
	unsigned int index;
	
	index = mac_hashfn(mac, USER_HTTP_RECORD_HASH_SIZE);
	head = &(g_user_http_record_HList[id][index]);

	USER_HTTP_RECORD_LOCK(id);
	hlist_for_each( p, head )
	{
		pstuser = hlist_entry(p, WD_USER_HTTP_RECORD_T, mac_Hlist);
		if (0 == memcmp(pstuser->mac, mac, 6))
        {	
        	/*用户已存在，则更新计数*/
			++ pstuser->cnt;
			USER_HTTP_RECORD_UNLOCK(id);
			return 0;
		}
	}

	/*用户未找到，则加入用户*/
	pstuser = (WD_USER_HTTP_RECORD_T*)malloc(sizeof(WD_USER_HTTP_RECORD_T));
    if(NULL == pstuser)
    {
		log_info(__LOG_ERROR__, "malloc error\n");	
		USER_HTTP_RECORD_UNLOCK(id);
        return -1;
    }
	memcpy(pstuser->mac, mac, 6);
	pstuser->stage = USER_HTTP_RECORD_INIT;
	pstuser->cnt = 1;
	pstuser->hit = 1;
	hlist_add_head(&(pstuser->mac_Hlist), head);
	sys_log_set(WIFIDOG_HTTP_LOG_FILE, "Add User Http Record: Mac("MAC_FMT")", 
		MAC_ARG(pstuser->mac));
	USER_HTTP_RECORD_UNLOCK(id);
	
	return 0;
}

static void user_http_record_age(void)
{
	int loop = 0, id = 0;
	WD_USER_HTTP_RECORD_T *pstuser = NULL;
	s_config *config = config_get_config();
	struct timeval timevl;
	struct hlist_node *p, *n;
	int uiHttpadwldura;

	for(id = 0; id < FIRST_PAGE_TASK_NUM; ++id)
	{
		USER_HTTP_RECORD_LOCK(id);
		for(loop = 0; loop < USER_HTTP_RECORD_HASH_SIZE; ++loop)
		{
			hlist_for_each_safe(p, n, &(g_user_http_record_HList[id][loop]))
			{
				pstuser = hlist_entry(p, WD_USER_HTTP_RECORD_T, mac_Hlist);

				/*首先更新终端状态*/
				if(USER_HTTP_RECORD_IN_WHITE == pstuser->stage){ /*终端在白名单状态*/
					if(user_acl_get_by_mac(pstuser->mac, USER_ACL_WHITE) == NULL){ /*白名单解除*/
						pstuser->stage = USER_HTTP_RECORD_OUT_WHITE;
						if(pstuser->cnt > 0)
							pstuser->hit = 1;
					}
					else{
						if(pstuser->cnt > 0){
							sys_log_set(WIFIDOG_HTTP_LOG_FILE, "User("MAC_FMT") in white list but cnt(%d)",
								MAC_ARG(pstuser->mac), pstuser->cnt);
						}
					}
				}
				else if(USER_HTTP_RECORD_IN_BLACK == pstuser->stage){ /*终端在黑名单状态*/
					if(user_acl_get_by_mac(pstuser->mac, USER_ACL_BLACK) == NULL){ /*黑名单解除*/
						pstuser->stage = USER_HTTP_RECORD_OUT_BLACK;
						if(pstuser->cnt > 0)
							pstuser->hit = 1;
					}
					else{
						if(pstuser->cnt > 0){
							sys_log_set(WIFIDOG_HTTP_LOG_FILE, "User("MAC_FMT") in black list but cnt(%d)",
								MAC_ARG(pstuser->mac), pstuser->cnt);
						}
					}
				}
				else{ /*终端在其它状态*/
					if(pstuser->cnt > 0)
						pstuser->hit = 1;
					else{ /*没有HTTP访问，判断是否需要老化*/
						if(0 == pstuser->hit){ /*终端未访问，老化*/
							sys_log_set(WIFIDOG_HTTP_LOG_FILE, "Del User Http Record: Mac("MAC_FMT") stage(%d:%s)", 
								MAC_ARG(pstuser->mac), pstuser->stage,
								((USER_HTTP_RECORD_INIT == pstuser->stage) ? "init" : 
								((USER_HTTP_RECORD_OUT_WHITE == pstuser->stage) ? "out-white" : "out-black")));
							hlist_del(&(pstuser->mac_Hlist));
							free(pstuser);
						}
						else
							pstuser->hit = 0; /*置终端未访问，下个周期老化*/
						continue;
					}
				}

				/*然后处理终端攻击*/
				if(pstuser->cnt < config->uiHttprdmaxcnt){ /*未超标，清零计数*/
					pstuser->cnt = 0;
					continue;
				}

				sys_log_set(WIFIDOG_HTTP_LOG_FILE, "User Exceed: Mac("MAC_FMT") cnt(%d) stage(%d:%s)", 
					MAC_ARG(pstuser->mac), pstuser->cnt, pstuser->stage,
					((USER_HTTP_RECORD_INIT == pstuser->stage) ? "init" : 
					((USER_HTTP_RECORD_OUT_WHITE == pstuser->stage) ? "out-white" : "out-black")));
				
				switch(pstuser->stage)
				{
				case USER_HTTP_RECORD_INIT: /*初始状态*/
					if(config->uiHttpadwldura > 0){ /*启用临时白名单机制*/
						gettimeofday(&timevl, NULL);
						srand(timevl.tv_usec);
						uiHttpadwldura = config->uiHttpadwldura*60*1000*(rand()%100 + 50)/100; /*0.5~1.5倍的随机时间*/
						user_acl_add_by_mac(pstuser->mac, uiHttpadwldura, USER_ACL_WHITE);
						pstuser->stage = USER_HTTP_RECORD_IN_WHITE;
						pstuser->cnt = 0;
						pstuser->hit = 1;
					}
					else{ /*不启用临时白名单机制，直接加入黑名单*/
						if(config->uiHttpadbldura > 0){
							user_acl_add_by_mac(pstuser->mac, config->uiHttpadbldura*60*1000, USER_ACL_BLACK);
							pstuser->stage = USER_HTTP_RECORD_IN_BLACK;
							pstuser->cnt = 0;
							pstuser->hit = 1;
						}
						else{ /*临时黑、白名单机制都不启用*/
							pstuser->cnt = 0;
							pstuser->hit = 1;
						}
					}
					break;
				case USER_HTTP_RECORD_OUT_WHITE: /*临时白名单超时后*/
				case USER_HTTP_RECORD_OUT_BLACK: /*临时黑名单超时后*/
					if(config->uiHttpadbldura > 0){ /*启用临时黑名单机制*/
						/*再次攻击，加入临时黑名单*/
						user_acl_add_by_mac(pstuser->mac, config->uiHttpadbldura*60*1000, USER_ACL_BLACK);
						pstuser->stage = USER_HTTP_RECORD_IN_BLACK;
						pstuser->cnt = 0;
						pstuser->hit = 1;
					}
					else{ /*不启用临时黑名单机制*/
						if(config->uiHttpadwldura > 0){ /*只启用临时白名单机制*/
							gettimeofday(&timevl, NULL);
							srand(timevl.tv_usec);
							uiHttpadwldura = config->uiHttpadwldura*60*1000*(rand()%100 + 50)/100; /*0.5~1.5倍的随机时间*/
							user_acl_add_by_mac(pstuser->mac, uiHttpadwldura, USER_ACL_WHITE);
							pstuser->stage = USER_HTTP_RECORD_IN_WHITE;
							pstuser->cnt = 0;
							pstuser->hit = 1;
						}
						else{ /*临时黑、白名单机制都不启用*/
							pstuser->cnt = 0;
							pstuser->hit = 1;
						}
					}
					break;
				case USER_HTTP_RECORD_IN_WHITE:
				case USER_HTTP_RECORD_IN_BLACK:
				default:
					break;
				}
	        }
		}
		USER_HTTP_RECORD_UNLOCK(id);
	}
}

#define MINT_TICK 10*60


void dis_connet_sta_age(unsigned long ulDisConStaAgeTime)
{
    if(0 == ulDisConStaAgeTime)
    {
        return;
    }
    log_info(LOG_DEBUG, "run ulDisConStaAgeTime=%ld\n", ulDisConStaAgeTime);
    //sys_log_set(WIFIDOG_SSID_AUTH, "run ulDisConStaAgeTime=%ld\n", ulDisConStaAgeTime);
    WD_STA_T *pststa =NULL;
    struct hlist_node *p,*n;
    struct hlist_head *head;
    int loop;
    char aucStrMac[32] = {0};

    
    for(loop = 0; loop < STA_HASH_SIZE; loop++)
    {
        STA_LIST_LOCK();
        head = &(g_sta_HList[loop]);
        hlist_for_each_safe(p, n, head)
        {
            pststa = hlist_entry(p, WD_STA_T, mac_Hlist);
            if(1 == pststa->uiDisAssociaFlag)
            {
                sprintf(aucStrMac,MAC_FMT,MAC_ARG(pststa->mac));
                if((pststa->ulDisAssociaTime + ulDisConStaAgeTime) < get_uptime())
                {
                    hlist_del(&(pststa->mac_Hlist));
                    log_info(LOG_DEBUG, "del disconnetc STA:%s\n ",aucStrMac);
                    free(pststa);
                    //sys_log_set(WIFIDOG_SSID_AUTH,"del disconnetc STA_MAC:%s",aucStrMac);
                }
            }
        }
	
        STA_LIST_UNLOCK();
    }
    
    return;
}

void thread_user_access_age(void)
{
	int iTick = 1;
	s_config *config = config_get_config();
    unsigned long ulDisConStaAgeTime = 0;
 
	while(1)
	{
		if(0 == iTick%10) /*大约1000ms老化一次IOS终端表项*/
			user_ios_access_age();

		if(0 == iTick%11) /*大约1100ms老化一次临时名单终端表项*/
			user_acl_age();

		if(0 == iTick%(config->uiHttpckIntvl*10)) /*大约10秒检查一次http攻击*/
			user_http_record_age();
            if(1 == config->uiSpanSsidAuthSwith)
            {
                if(0 == iTick%(MINT_TICK + 1))
                {
                    ulDisConStaAgeTime = ((config->forcetimeout > config->idletimeout) ? config->forcetimeout : config->idletimeout);
                    if(0 == ulDisConStaAgeTime)
                    {
                        ulDisConStaAgeTime = 60*60;/*一小时校验一次*/
                    }
                    log_info(LOG_DEBUG,"update ulDisConStaAgeTime=%ld  foceTime=%ld idleTime=%ld",\
                    ulDisConStaAgeTime,config->forcetimeout,config->idletimeout);
                }
                if(0 == iTick%(ulDisConStaAgeTime*10+1))
                {
                    dis_connet_sta_age(ulDisConStaAgeTime);
                }
            }
		++ iTick;
            usleep(100000);/*100ms*/
	}
	
	sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at thread_user_access_age()");
    exit(1);
    return;
}

int user_access_init(void)
{
	int i, j;

	for(i = 0; i < FIRST_PAGE_TASK_NUM; ++i)
		pthread_mutex_init(&user_ios_access_list_mutex[i], NULL);

	for(i = 0; i < FIRST_PAGE_TASK_NUM; ++i){
		for(j = 0; j < USER_IOS_ACCESS_HASH_SIZE; ++j){	
			INIT_HLIST_HEAD(&g_user_ios_access_mac_HList[i][j]);
		}
	}

	pthread_mutex_init(&user_acl_list_mutex, NULL);
	for(j = 0; j < USER_ACL_LIST_HASH_SIZE; ++j){	
		INIT_HLIST_HEAD(&g_user_acl_HList[j]);
	}

	for(i = 0; i < FIRST_PAGE_TASK_NUM; ++i){
		pthread_mutex_init(&user_http_record_mutex[i], NULL);
		for(j = 0; j < USER_HTTP_RECORD_HASH_SIZE; ++j){	
			INIT_HLIST_HEAD(&g_user_http_record_HList[i][j]);
		}
	}
	
	if(pthread_create(&useriosaccessage_pid, NULL, (void *)thread_user_access_age, NULL) != 0){
        log_info(__LOG_ERROR__, "create useriosaccessage_pid failed: %s", strerror(errno));
	}
	pthread_detach(useriosaccessage_pid);
	
	return 0;
}

