/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : client_dct.c

******************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <time.h>
#include "log.h"
#include "ag_module.h"       //aglink 头文件
#include "conf.h"

#include <config_common.h>

extern AG_MODULE_IPC_REGISTER g_stWifidogAglink;

extern int g_ext_dbg_level;
#define STA_DCT_LOG(__level__, __format__, __arg__...) \
		do{ \
			if((LOG_DEBUG == __level__) && (g_ext_dbg_level != 0))\
				syslog(LOG_INFO, "wdpriv-dct: "__format__, ##__arg__);\
			else if(LOG_DEBUG == __level__)\
				break;\
			else\
				syslog(__level__, "wdpriv-dct: "__format__, ##__arg__);\
		}while(0)

#define STADCT_BUF_LEN			6144

unsigned char g_mac_buf[STADCT_BUF_LEN];
unsigned int g_mac_pos = 0;

pthread_mutex_t g_stadct_buf_mutex;
#define STADCT_BUF_LOCK()		pthread_mutex_lock(&g_stadct_buf_mutex)
#define STADCT_BUF_UNLOCK()		pthread_mutex_unlock(&g_stadct_buf_mutex)

#define STADCT_LEN	8   /*mac:6 rssi:1 quit:1*/

int sta_dct_add_by_mac(unsigned char *ap_mac, unsigned char *sta_mac, char rssi)
{
	int dbm = 0;
	STA_DCT_LOG(LOG_DEBUG, "sta dct add mac "MAC_FMT" rssi %d", MAC_ARG(sta_mac), rssi);

	STADCT_BUF_LOCK();
	if((STADCT_BUF_LEN - g_mac_pos) >= STADCT_LEN){
		memcpy(g_mac_buf + g_mac_pos, sta_mac, 6);
		g_mac_pos += 6;

		dbm = (int)rssi;
		*(char *)(g_mac_buf + g_mac_pos) = dbm;
		++g_mac_pos;

		*(char *)(g_mac_buf + g_mac_pos) = 1;
		++g_mac_pos;
	}
	STADCT_BUF_UNLOCK();
	
	return 0;	
}

int sta_dct_del_by_mac(unsigned char *ap_mac, unsigned char *sta_mac)
{
	STA_DCT_LOG(LOG_DEBUG, "sta dct add mac "MAC_FMT"", MAC_ARG(sta_mac));

	STADCT_BUF_LOCK();
	if((STADCT_BUF_LEN - g_mac_pos) >= STADCT_LEN){
		memcpy(g_mac_buf + g_mac_pos, sta_mac, 6);
		g_mac_pos += 6;

		*(char *)(g_mac_buf + g_mac_pos) = 0;
		++g_mac_pos;

		*(char *)(g_mac_buf + g_mac_pos) = 0;
		++g_mac_pos;
	}
	STADCT_BUF_UNLOCK();

	return 0;
}



int stadct_send_list_report(void *func, void *config, int *sockfd)
{
	int i;
	char sta_list[7168];
	int ttl_sta_num = g_mac_pos/STADCT_LEN;
	int sta_num;
	char temp[64];

	void (*send_stadct_list)(int , char *, int *, void *, char *) = func;
	
    memset(sta_list, 0, sizeof(sta_list));
    sta_num = 0;

	STADCT_BUF_LOCK();
	for(i=0; i<ttl_sta_num; ++i){
		sprintf(temp, MAC_FMT"|%d|%d;", MAC_ARG(g_mac_buf + i*STADCT_LEN), 
			*(char *)(g_mac_buf + i*STADCT_LEN + 6),
			*(char *)(g_mac_buf + i*STADCT_LEN + 7));
		if(strlen(sta_list)+strlen(temp) > (sizeof(sta_list)-1))
        {
            send_stadct_list(sta_num, sta_list, sockfd, config, "stadctlist");
            sta_num = 0;
            memset(sta_list, 0, sizeof(sta_list));
        }

		strcat(sta_list, temp);
        ++sta_num;
	}
	
	if(sta_num > 0)
		send_stadct_list(sta_num, sta_list, sockfd, config, "stadctlist");
	g_mac_pos = 0;
	STADCT_BUF_UNLOCK(); 

	return 0;
}

int sta_dct_enable(int enable)
{
	int iRet = -1;
	unsigned char msg;
	static unsigned char en = (unsigned char)LOCATOR_DISABLE_WIFIDOG;
	
	STA_DCT_LOG(LOG_INFO, "Send sta dct %s to aglink", (enable ? "enable" : "disable"));

	msg = (unsigned char)(enable ? LOCATOR_ENABLE_WIFIDOG : LOCATOR_DISABLE_WIFIDOG);
	if(msg == en)
		return 0;
	en = msg;

#ifndef GW_X86
	/*配置到本地*/
	iRet = ag_module_ipc_client_send_msg(LOCATOR_IDSET_SW,
		    							  AG_IPC_MSG_TYPE_SELF_CPU,
		    							  &g_stWifidogAglink,
		    							  (unsigned char *)(&msg),
		    							  1);
	if (iRet < 0){
		log_info(__LOG_ERROR__, "LOCATOR_IDSET_SW send to self fail");
    }else{
		log_info(__LOG_DEBUG__, "LOCATOR_IDSET_SW send to self success");
	}
#endif

	/*配置到所有AP*/
	iRet = ag_module_ipc_client_send_msg(LOCATOR_IDSET_SW,
		    							  AG_IPC_MSG_TYPE_OTHER_CPU,
		    							  &g_stWifidogAglink,
		    							  (unsigned char *)(&msg),
		    							  1);
	if (iRet < 0){
		log_info(__LOG_ERROR__, "LOCATOR_IDSET_SW send to ap fail");
    }else{
		log_info(__LOG_DEBUG__, "LOCATOR_IDSET_SW send to ap success");
	}
	
	return 0;
}

int sta_dct_rssi(int rssi)
{
	int iRet;
	char msg;
	STA_DCT_LOG(LOG_INFO, "Send sta dct rssi(%d) to aglink", rssi);

	msg = (char)rssi;
	
#ifndef GW_X86
	/*配置到本地*/
	iRet = ag_module_ipc_client_send_msg(LOCATOR_IDSET_RSSI,
		    							  AG_IPC_MSG_TYPE_SELF_CPU,
		    							  &g_stWifidogAglink,
		    							  (unsigned char *)(&msg),
		    							  1);
	if (iRet < 0){
		log_info(__LOG_ERROR__, "LOCATOR_IDSET_RSSI send to self fail");
    }else{
		log_info(__LOG_DEBUG__, "LOCATOR_IDSET_RSSI send to self success");
	}
#endif

	/*配置到所有AP*/
	iRet = ag_module_ipc_client_send_msg(LOCATOR_IDSET_RSSI,
		    							  AG_IPC_MSG_TYPE_OTHER_CPU,
		    							  &g_stWifidogAglink,
		    							  (unsigned char *)(&msg),
		    							  1);
	if (iRet < 0){
		log_info(__LOG_ERROR__, "LOCATOR_IDSET_RSSI send to ap fail");
    }else{
		log_info(__LOG_DEBUG__, "LOCATOR_IDSET_RSSI send to ap success");
	}
	
	return 0;
}

int sta_dct_quit_timeout(int timeout)
{
	int iRet;
	char msg[32];
	STA_DCT_LOG(LOG_INFO, "Send sta dct quit timeout(%d) to aglink", timeout);

	snprintf(msg, sizeof(msg), "%d", timeout);
	
#ifndef GW_X86
	/*配置到本地*/
	iRet = ag_module_ipc_client_send_msg(LOCATOR_IDSET_QUITTIMEOUT,
		    							  AG_IPC_MSG_TYPE_SELF_CPU,
		    							  &g_stWifidogAglink,
		    							  (unsigned char *)(msg),
		    							  sizeof(msg));
	if (iRet < 0){
		log_info(__LOG_ERROR__, "LOCATOR_IDSET_QUITTIMEOUT send to self fail");
    }else{
		log_info(__LOG_DEBUG__, "LOCATOR_IDSET_QUITTIMEOUT send to self success");
	}
#endif

	/*配置到所有AP*/
	iRet = ag_module_ipc_client_send_msg(LOCATOR_IDSET_QUITTIMEOUT,
		    							  AG_IPC_MSG_TYPE_OTHER_CPU,
		    							  &g_stWifidogAglink,
		    							  (unsigned char *)(msg),
		    							  sizeof(msg));
	if (iRet < 0){
		log_info(__LOG_ERROR__, "LOCATOR_IDSET_QUITTIMEOUT send to ap fail");
    }else{
		log_info(__LOG_DEBUG__, "LOCATOR_IDSET_QUITTIMEOUT send to ap success");
	}
	
	return 0;
}


int sta_dct_init(void)
{
	pthread_mutex_init(&g_stadct_buf_mutex, NULL);
	return 0;
}

