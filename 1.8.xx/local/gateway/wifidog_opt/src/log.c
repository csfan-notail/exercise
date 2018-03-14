/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : log.c
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/11/25
  Last Modified :
  Description   : 
  Function List :
  History       :
  1.Date        : 2013/11/25
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <error.h>
#include <syslog.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "conf.h"


/** @记录log日志操作 */
void _log_info(char *filename, int line, int level, char *format, ...)
{
    char buf[28];
    char ident[128];
    va_list vlist;
    s_config *config = config_get_config();
    time_t ts;

    time(&ts);

	/*add@2014.8.15 打印中添加线程id*/
	pthread_t id = pthread_self();
	
    if (config->debuglevel >= level || !config->daemon) {
        va_start(vlist, format);
        fprintf(stdout, "[%d][%.24s][wifidog][pid:%u][%s][%d] ", level, ctime_r(&ts, buf), (unsigned int)id, filename, line);
        vfprintf(stdout, format, vlist);
        fputc('\n', stdout);
        fflush(stdout);

        if (config->log_syslog) {
            sprintf(ident, "[wifidog][%u][%s][%d]", (unsigned int)id, filename, line);
            openlog(ident, LOG_PID, config->syslog_facility);
            vsyslog(level, format, vlist);
            closelog();
        }
    }
}

