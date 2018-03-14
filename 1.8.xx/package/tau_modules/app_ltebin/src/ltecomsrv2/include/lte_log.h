#ifndef __LTE_LOG_H__
#define __LTE_LOG_H__

extern int g_iLteLogLevel;

#define	LTE_LOG_EMERG	0	/* system is unusable */
#define	LTE_LOG_ALERT	1	/* action must be taken immediately */
#define	LTE_LOG_CRIT	2	/* critical conditions */
#define	LTE_LOG_ERR		3	/* error conditions */
#define	LTE_LOG_WARNING	4	/* warning conditions */
#define	LTE_LOG_NOTICE	5	/* normal but significant condition */
#define	LTE_LOG_INFO	6	/* informational */
#define	LTE_LOG_DEBUG	7	/* debug-level messages */


#define LTE_LOG(level, format, arg...) \
do{                                    \
    if (level <= g_iLteLogLevel)       \
    {                                  \
        syslog(level,"[ltecom] : "format, ##arg);   \
    }                                  \
}while(0);

#endif  /* __LTE_LOG_H__ */
