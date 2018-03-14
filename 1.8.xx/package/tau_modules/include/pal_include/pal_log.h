#ifndef __PAL_LOG__H__
#define __PAL_LOG_H__

/* 打印级别 */
#define LOG_OFF                 0xff    /* 不打印 */

#define LOG_EMERG               0       /* system is unusable */
#define LOG_ALERT               1       /* action must be taken immediately */
#define LOG_CRIT                2       /* critical conditions */
#define LOG_ERR                 3       /* error conditions */
#define LOG_WARNING             4       /* warning conditions */
#define LOG_NOTICE              5       /* normal but significant condition */
#define LOG_INFO                6       /* informational */
#define LOG_DEBUG               7       /* debug-level messages */

extern void app_log(char *func, int line, int level, char *format, ... );

#define gw_log(level, format, arg...)                             \
do                                                                  \
{                                                                   \
    app_log((char *)__FUNCTION__, __LINE__, level, format, ##arg);      \
} while(0);      

#define log_info(level, format, arg...)                             \
do                                                                  \
{                                                                   \
    app_log((char *)__FUNCTION__, __LINE__, level, format, ##arg);      \
} while(0);      
#endif
