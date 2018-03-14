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
#include <unistd.h>
#include <fcntl.h>

#include "log.h"


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
    //log_level = cgic_log_init();
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

    sprintf(ac_log_format, "APP [%s] [func:%s line:%d] %s", aclevel, func, line, format);
    vsyslog(LOG_WARNING, ac_log_format, va_arg);
    if(ac_log_format)
    {
        free(ac_log_format);
    }
    
    va_end(va_arg);
    
    return;
}

