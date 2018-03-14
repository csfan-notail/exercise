/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : log.h
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/11/25
  Last Modified :
  Description   : log.h header file
  Function List :
  History       :
  1.Date        : 2013/11/25
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/

#ifndef __LOG_H__
#define __LOG_H__

/** @日志级别 */
#define __LOG_FAILED__  LOG_EMERG
#define __LOG_ERROR__   LOG_ERR
#define __LOG_WARNING__ LOG_WARNING
#define __LOG_MSG__     LOG_INFO
//#define __LOG_DEBUG__   LOG_DEBUG
#define __LOG_DEBUG__   LOG_DEBUG

/** @日志记录接口 */
/*BEGIN: modified @2014.8.26 */
//#define log_info(level, format...) _log_info(__FILE__, __LINE__, level, format)
//void _log_info(char *filename, int line, int level, char *format, ...);
#include <pthread.h>
#include <syslog.h>

typedef enum {
	WIFIDOG_THREAD_MASTER = 0,
	WIFIDOG_THREAD_GCTL,
	WIFIDOG_THREAD_PLATFORM,
	WIFIDOG_THREAD_8061,
	WIFIDOG_THREAD_8062,
	WIFIDOG_THREAD_80,
	WIFIDOG_THREAD_AGLINK,
	WIFIDOG_THREAD_8061MULTI,
	WIFIDOG_THREAD_STAKEY,
	WIFIDOG_THREAD_MAX
}EN_WIFIDOG_THREAD_INDEX;

struct __thread_name{
	char *name;
	EN_WIFIDOG_THREAD_INDEX enThreadIndex;
};

extern int wifidog_get_thread_index(unsigned int pid);
extern int g_debug_level;
extern int g_daemon;
extern struct __thread_name g_threadname[WIFIDOG_THREAD_MAX];

#define log_info(level, format, arg...) do{ \
if (level <= g_debug_level) \
{\
	syslog(level, "[%s][%s][%d] "format"\n", g_threadname[wifidog_get_thread_index((unsigned int)(pthread_self()))].name, __FUNCTION__, __LINE__, ##arg);\
}\
if (!g_daemon) \
{ \
	fprintf(stderr, "[%s][%s][%d] "format"\n", g_threadname[wifidog_get_thread_index((unsigned int)(pthread_self()))].name, __FUNCTION__, __LINE__, ##arg);\
}\
}while(0)
/*END: modified @2014.8.26 */

#endif /* __LOG_H__ */


