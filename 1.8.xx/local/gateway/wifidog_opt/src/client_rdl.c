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

/*��ֹ��������ʱ��Ϊg_age_time ~ g_age_time+g_check_time*/
/* MAC�ϻ�ʱ�䲻��С��3�� */
unsigned long g_ios_access_age_time = 360000; /*�û�mac�ϻ�ʱ��(ms)*/
unsigned long g_ios_access_timeout_time = 20000; /*mozilla����portal��ʱʱ��*/
unsigned long g_ios_access_timeout_time_m = 300000; /*����portal��Ĭ���л�INIT״̬��ʱʱ��*/
unsigned long g_ios_network_access_timeout_time = 8000; /*network����portal��ʱʱ��*/

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


    if(t_cost_time_sec >= 4294968) /*�п�����ϵͳʱ��ͬ��ǰ���µ�,Ϊ��ֹ32λԽ�磬ֱ���ϻ�*/
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
        	/*Ϊ��֤״̬��¼�����ԣ�
        			������1\2\3�����б���ϻ�������time*/
        	if (CHECK_INIT != pstuser->check)		
        	{
                gettimeofday(&pstuser->time, NULL); //����ֻ��network���޷���ʱ
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
	/*�û�δ�ҵ���������û�*/
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
	        	/*�û��Ѵ��ڣ������ʱ��*/
	        	gettimeofday(&pstuser->time, NULL); 
				pstuser->duration = duration;
				USER_ACL_LIST_UNLOCK();
				return 0;
        	}
		}
	}

	/*�û�δ�ҵ���������û�*/
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
			}*//*����֤�������Ѹ�Ϊ�������ã�����Ҫrestore��*/
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
        	/*�û��Ѵ��ڣ�����¼���*/
			++ pstuser->cnt;
			USER_HTTP_RECORD_UNLOCK(id);
			return 0;
		}
	}

	/*�û�δ�ҵ���������û�*/
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

				/*���ȸ����ն�״̬*/
				if(USER_HTTP_RECORD_IN_WHITE == pstuser->stage){ /*�ն��ڰ�����״̬*/
					if(user_acl_get_by_mac(pstuser->mac, USER_ACL_WHITE) == NULL){ /*���������*/
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
				else if(USER_HTTP_RECORD_IN_BLACK == pstuser->stage){ /*�ն��ں�����״̬*/
					if(user_acl_get_by_mac(pstuser->mac, USER_ACL_BLACK) == NULL){ /*���������*/
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
				else{ /*�ն�������״̬*/
					if(pstuser->cnt > 0)
						pstuser->hit = 1;
					else{ /*û��HTTP���ʣ��ж��Ƿ���Ҫ�ϻ�*/
						if(0 == pstuser->hit){ /*�ն�δ���ʣ��ϻ�*/
							sys_log_set(WIFIDOG_HTTP_LOG_FILE, "Del User Http Record: Mac("MAC_FMT") stage(%d:%s)", 
								MAC_ARG(pstuser->mac), pstuser->stage,
								((USER_HTTP_RECORD_INIT == pstuser->stage) ? "init" : 
								((USER_HTTP_RECORD_OUT_WHITE == pstuser->stage) ? "out-white" : "out-black")));
							hlist_del(&(pstuser->mac_Hlist));
							free(pstuser);
						}
						else
							pstuser->hit = 0; /*���ն�δ���ʣ��¸������ϻ�*/
						continue;
					}
				}

				/*Ȼ�����ն˹���*/
				if(pstuser->cnt < config->uiHttprdmaxcnt){ /*δ���꣬�������*/
					pstuser->cnt = 0;
					continue;
				}

				sys_log_set(WIFIDOG_HTTP_LOG_FILE, "User Exceed: Mac("MAC_FMT") cnt(%d) stage(%d:%s)", 
					MAC_ARG(pstuser->mac), pstuser->cnt, pstuser->stage,
					((USER_HTTP_RECORD_INIT == pstuser->stage) ? "init" : 
					((USER_HTTP_RECORD_OUT_WHITE == pstuser->stage) ? "out-white" : "out-black")));
				
				switch(pstuser->stage)
				{
				case USER_HTTP_RECORD_INIT: /*��ʼ״̬*/
					if(config->uiHttpadwldura > 0){ /*������ʱ����������*/
						gettimeofday(&timevl, NULL);
						srand(timevl.tv_usec);
						uiHttpadwldura = config->uiHttpadwldura*60*1000*(rand()%100 + 50)/100; /*0.5~1.5�������ʱ��*/
						user_acl_add_by_mac(pstuser->mac, uiHttpadwldura, USER_ACL_WHITE);
						pstuser->stage = USER_HTTP_RECORD_IN_WHITE;
						pstuser->cnt = 0;
						pstuser->hit = 1;
					}
					else{ /*��������ʱ���������ƣ�ֱ�Ӽ��������*/
						if(config->uiHttpadbldura > 0){
							user_acl_add_by_mac(pstuser->mac, config->uiHttpadbldura*60*1000, USER_ACL_BLACK);
							pstuser->stage = USER_HTTP_RECORD_IN_BLACK;
							pstuser->cnt = 0;
							pstuser->hit = 1;
						}
						else{ /*��ʱ�ڡ����������ƶ�������*/
							pstuser->cnt = 0;
							pstuser->hit = 1;
						}
					}
					break;
				case USER_HTTP_RECORD_OUT_WHITE: /*��ʱ��������ʱ��*/
				case USER_HTTP_RECORD_OUT_BLACK: /*��ʱ��������ʱ��*/
					if(config->uiHttpadbldura > 0){ /*������ʱ����������*/
						/*�ٴι�����������ʱ������*/
						user_acl_add_by_mac(pstuser->mac, config->uiHttpadbldura*60*1000, USER_ACL_BLACK);
						pstuser->stage = USER_HTTP_RECORD_IN_BLACK;
						pstuser->cnt = 0;
						pstuser->hit = 1;
					}
					else{ /*��������ʱ����������*/
						if(config->uiHttpadwldura > 0){ /*ֻ������ʱ����������*/
							gettimeofday(&timevl, NULL);
							srand(timevl.tv_usec);
							uiHttpadwldura = config->uiHttpadwldura*60*1000*(rand()%100 + 50)/100; /*0.5~1.5�������ʱ��*/
							user_acl_add_by_mac(pstuser->mac, uiHttpadwldura, USER_ACL_WHITE);
							pstuser->stage = USER_HTTP_RECORD_IN_WHITE;
							pstuser->cnt = 0;
							pstuser->hit = 1;
						}
						else{ /*��ʱ�ڡ����������ƶ�������*/
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
		if(0 == iTick%10) /*��Լ1000ms�ϻ�һ��IOS�ն˱���*/
			user_ios_access_age();

		if(0 == iTick%11) /*��Լ1100ms�ϻ�һ����ʱ�����ն˱���*/
			user_acl_age();

		if(0 == iTick%(config->uiHttpckIntvl*10)) /*��Լ10����һ��http����*/
			user_http_record_age();
            if(1 == config->uiSpanSsidAuthSwith)
            {
                if(0 == iTick%(MINT_TICK + 1))
                {
                    ulDisConStaAgeTime = ((config->forcetimeout > config->idletimeout) ? config->forcetimeout : config->idletimeout);
                    if(0 == ulDisConStaAgeTime)
                    {
                        ulDisConStaAgeTime = 60*60;/*һСʱУ��һ��*/
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

