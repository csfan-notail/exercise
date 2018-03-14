/******************************************************************************

  Copyright (C), 2008-2012, GBCOM Co., Ltd.

 ******************************************************************************
  File Name     : stam.h
  Version       : Initial Draft
  Author        : xiabing
  Created       : 2014/11/30
  Last Modified :
  Description   : stam.c header file
  Function List :
  History       :
  1.Date        : 2014/11/30
    Author      : xiabing
    Modification: Created file

******************************************************************************/

/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/

/*----------------------------------------------*
 * module-wide global variables                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macros                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * routines' implementations                    *
 *----------------------------------------------*/

#ifndef __STAM_H__
#define __STAM_H__

#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <pthread.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/wireless.h>
#include <linux/if.h>
#include <linux/sockios.h>
	
#include <getopt.h>
#include <time.h>
#include <sys/time.h>
	
#include "wireless_copy.h"
#include "tool.h"
	
#include "ag_module.h"
#include "list.h"
#include "ag_msg.h"
#include "gbcom_wifi_pal.h"

#include <syslog.h>

#define    STAM_LOG_EMERG   0   /* system is unusable */
#define    STAM_LOG_ALERT   1   /* action must be taken immediately */
#define    STAM_LOG_CRIT    2   /* critical conditions */
#define    STAM_LOG_ERR     3   /* error conditions */
#define    STAM_LOG_WARNING 4   /* warning conditions */
#define    STAM_LOG_NOTICE  5   /* normal but significant condition */
#define    STAM_LOG_INFO    6   /* informational */
#define    STAM_LOG_DEBUG   7   /* debug-level messages */

#define STAM_MODE_DAEMON 1
#define STAM_MODE_FOREHEAD 0

extern int g_iStamLoglevel;
extern int g_iStamDaemon; 

#define STAM_PID_FILE "/var/run/stam.pid"


AG_MODULE_IPC_REGISTER gstStamIPCmgt;
struct hlist_head gstStahlist[STA_HASH_SIZE] = {0};
u8 gcWanMac[MAC_LEN] = {0};

pthread_mutex_t sta_hlist_mutex;				/*终端关联信息缓存访问锁*/

#define STA_HLIST_LOCK()		pthread_mutex_lock(&sta_hlist_mutex)
#define STA_HLIST_UNLOCK()		pthread_mutex_unlock(&sta_hlist_mutex)


#define STAM_LOG(level, format, arg...) do{ \
			if (level <= g_iStamLoglevel) \
			{\
				syslog(level, "[stam][%s][%d] "format"\n", __FUNCTION__, __LINE__, ##arg);\
			}\
			if (STAM_MODE_FOREHEAD == g_iStamDaemon) \
			{ \
				fprintf(stderr, "[stam][%s][%d] "format"\n", __FUNCTION__, __LINE__, ##arg);\
			}\
		}while(0)


#endif /* __ALARMSIMU_H__ */
