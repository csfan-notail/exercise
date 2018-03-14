#ifndef __CHECKALIVE_H__
#define __CHECKALIVE_H__

#define USB_IF "usb0"
#define PPP_IF "ppp0"

#define SYS_REBOOT "reboot"

#define DEFAULT_ATAIR_IP "169.254.0.10"


#define	LOG_EMERG	0	/* system is unusable */
#define	LOG_ALERT	1	/* action must be taken immediately */
#define	LOG_CRIT	    2	/* critical conditions */
#define	LOG_ERR		3	/* error conditions */
#define	LOG_WARNING	4	/* warning conditions */
#define	LOG_NOTICE	5	/* normal but significant condition */
#define	LOG_INFO	    6	/* informational */
#define	LOG_DEBUG	7	/* debug-level messages */

extern int g_iLogLevel;

#define LOG(level, format, arg...) \
do{                                    \
    if (level <= g_iLogLevel)       \
    {                                  \
        syslog(level,"[checkalive][fun:%s],[line:%d]"format,__func__,__LINE__, ##arg);   \
    }                                  \
}while(0);


#endif /* __SYSMONITOR_H__ */

