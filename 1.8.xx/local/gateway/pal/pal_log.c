/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : log.c
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/5/9
  Last Modified :
  Description   : CGI 日志
  Function List :
  History       :
  1.Date        : 2013/5/9
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <stdarg.h>
#include <time.h>

#include "app_public.h"
#include "log.h"

/*--------- function extern ---------*/
int get_log_time(char * log_time);
int cgic_log_init(void);

/*****************************************************************************
 Prototype    : get_log_time
 Description  : 获取系统时间， 将日期写入log日志中
 Input        : void
 Output       : None
 Return Value : int
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/5/16
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
int get_log_time(char *log_time)
{
    time_t now;
    struct tm *gt;

    memset(&now, 0, sizeof(time_t));
    memset(log_time, 0, TIME_LEN);
    time(&now);
    gt = gmtime(&now);
    sprintf(log_time, "%04d-%02d-%02d %02d:%02d:%02d", 
        (gt->tm_year+1900), gt->tm_mon, gt->tm_mday, gt->tm_hour, gt->tm_min, gt->tm_sec);
    return 0;
}

/*****************************************************************************
 Prototype    : cgic_log_init
 Description  : 日志初始化
 Input        : void
 Output       : None
 Return Value : int
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/5/9
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
int cgic_log_init(void)
{
    FILE *pflog;
    char buff[32];
    int log_level;

    log_level = CGI_LOG_DEBUG;
    pflog = fopen(CGIC_LOG_LEVEL, "r");
    if(NULL == pflog)
    {
        log_level = CGI_LOG_DEBUG;
        return log_level;
    }
    fgets(buff, 32, pflog);
    fclose(pflog);

    log_level = atoi(buff);

    return log_level;
}

/*****************************************************************************
 Prototype    : cgic_log
 Description  : CGI日志
 Input        : char *func, int line, int level, char *format, ...
 Output       : None
 Return Value : void
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/5/9
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
void cgic_log(char *func, int line, int level, char *format, ... )
{
    //FILE *pflog;
    va_list va_arg;
    char aclevel[16];
    char ac_log_time[TIME_LEN];
    char *ac_log_format;
    int log_level;

    log_level = cgic_log_init();
    if(level <= log_level)
    {
        memset(aclevel, 0, 16);
        switch(level)
        {
            case CGI_LOG_ERR:
                strcpy(aclevel, "ERROR");
                break;
                
            case CGI_LOG_WARN:
                strcpy(aclevel, "WARNING");
                break;
                
            case CGI_LOG_FAIL:
                strcpy(aclevel, "FAIL");
                break;
                
            case CGI_LOG_MSG:
                strcpy(aclevel, "MSG");
                break;
                
            case CGI_LOG_DEBUG:
                strcpy(aclevel, "DEBUG");
                break;
                
            default:
                sprintf(aclevel, "UNKNOW (%d)", level);
                break;
        }
    }
    else
    {
        return;
    }

    memset(ac_log_time, 0, TIME_LEN);
    get_log_time(ac_log_time);
    
    va_start(va_arg, format);
    ac_log_format = NULL;
    ac_log_format = (char *)malloc(FORMAT_LEN);
    if(NULL == ac_log_format)
    {
        syslog(LOG_WARNING, "ac_log_format malloc failed");
        return;
    }
#if 0 //del by zhz ajax 执行后太多打印，内存爆满
    pflog = fopen(CGIC_LOG_PATH, "a");
    if(NULL == pflog)
    {
        syslog(LOG_WARNING, "cgic's log open failed");
        return;
    }

    va_start(va_arg, format);
    
    /* write cgic's log; the g_log_format size do not detect */
    ac_log_format = NULL;
    ac_log_format = (char *)malloc(FORMAT_LEN);
    if(NULL == ac_log_format)
    {
        syslog(LOG_WARNING, "ac_log_format malloc failed");
        return;
    }
    memset(ac_log_format, 0, FORMAT_LEN);
    sprintf(ac_log_format, "[%s] [%s] [func:%s line:%d] %s\n", ac_log_time, aclevel, func, line, format);
    vfprintf(pflog, ac_log_format, va_arg);
    fclose(pflog);
#endif
    /* write syslog */
    sprintf(ac_log_format, "[CGI][%s][%s][%d] %s", aclevel, func, line, format);
    vsyslog(LOG_WARNING, ac_log_format, va_arg);
    if(ac_log_format)
    {
        free(ac_log_format);
    }
    
    va_end(va_arg);
    
    return;
}



/*****************************************************************************
 Prototype    : app_log
 Description  : 应用层日志模块
 Input        : char *func    
                int line      
                int level     
                char *format  
                ...           
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/5/27
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
void app_log(char *func, int line, int level, char *format, ... )
{
    va_list va_arg;
    char aclevel[16];
    char *ac_log_format;
    int log_level;

    log_level = APP_LOG_DEBUG;
    log_level = cgic_log_init();
    if(level <= log_level)
    {
        memset(aclevel, 0, 16);
        switch(level)
        {
            case APP_LOG_ERR:
                strcpy(aclevel, "ERROR");
                break;
                
            case APP_LOG_WARN:
                strcpy(aclevel, "WARNING");
                break;
                
            case APP_LOG_FAIL:
                strcpy(aclevel, "FAIL");
                break;
                
            case APP_LOG_MSG:
                strcpy(aclevel, "MSG");
                break;
                
            case APP_LOG_DEBUG:
                strcpy(aclevel, "DEBUG");
                break;
                
            default:
                sprintf(aclevel, "UNKNOW (%d)", level);
                break;
        }
    }
    else
    {
        return;
    }
    
    va_start(va_arg, format);
    
    /* write app's log; the g_log_format size do not detect */
    ac_log_format = NULL;
    ac_log_format = (char *)malloc(FORMAT_LEN);
    if(NULL == ac_log_format)
    {
        syslog(LOG_WARNING, "ac_log_format malloc failed");
        return;
    }

    sprintf(ac_log_format, "[APP][%s][%s][%d] %s", aclevel, func, line, format);
    vsyslog(LOG_WARNING, ac_log_format, va_arg);
    if(ac_log_format)
    {
        free(ac_log_format);
    }
    
    va_end(va_arg);
    
    return;
}

/*****************************************************************************
 Prototype    : cgi_web_log_init
 Description  : 
 Input        : int total  
                int len    
 Output       : None
 Return Value : char
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/7/5
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
char *cgi_web_log_init(int total, int len)
{
    char *log;

    log = NULL;
    log = (char *)malloc(total * len);
    if(NULL == log)
    {
        lcgic_log(CGI_LOG_FAIL, "log malloc failed.");
        return NULL;
    }
    memset(log, 0, total*len);
    return log;
}

/*****************************************************************************
 Prototype    : cgi_web_log_free
 Description  : 
 Input        : char *log  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/7/5
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
void cgi_web_log_free(char *log)
{
    if(NULL != log)
    {
        free(log);
    }
    return ;
}

