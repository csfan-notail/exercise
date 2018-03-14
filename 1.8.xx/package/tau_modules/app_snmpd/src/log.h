/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : log.h
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/5/9
  Last Modified :
  Description   : log.c header file
  Function List :
  History       :
  1.Date        : 2013/5/9
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#ifndef __LOG_H__
#define __LOG_H__

/*---------------cgi log 级别 ---------------*/
enum en_log_level
{
    CGI_LOG_FAIL = 1,
    CGI_LOG_ERR,
    CGI_LOG_WARN,
    CGI_LOG_MSG,
    CGI_LOG_DEBUG
};

#define CGIC_LOG_LEVEL  "/tmp/cigc.level"
#define CGIC_LOG_PATH   "/var/log/cgic.log"

/*---------------app log 级别 ---------------*/
enum en_app_log
{
    APP_LOG_FAIL = 1,
    APP_LOG_ERR,
    APP_LOG_WARN,
    APP_LOG_MSG,
    APP_LOG_DEBUG
};

/* 日志缓冲区大小 */
#define FORMAT_LEN  1024

/* 日志中的时间记录 */
#define TIME_LEN    128

extern char *cgi_web_log_init(int total, int len);
extern void cgi_web_log_free(char *log);
extern void cgic_log(char *func, int line, int level, char *format, ...);
extern void app_log(char *func, int line, int level, char *format, ...);
extern void redir_log(char *func, int line, int level, char *format, ...);
    
#define lcgic_log(level, format, arg...)                            \
do                                                                  \
{                                                                   \
    cgic_log((char *)__func__, __line__, level, format, ##arg);     \
} while(0);                                                         \


#define lapp_log(level, format, arg...)                             \
do                                                                  \
{                                                                   \
    app_log((char *)__func__, __line__, level, format, ##arg);      \
} while(0);                                                         \

#define lredir_log(level, format, arg...)                           \
do                                                                  \
{                                                                   \
    redir_log((char *)__func__, __line__, level, format, ##arg);    \
}while(0);

#endif /* __LOG_H__ */
