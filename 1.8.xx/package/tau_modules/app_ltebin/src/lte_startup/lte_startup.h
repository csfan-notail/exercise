#ifndef __LTE_STARTUP_H__
#define __LTE_STARTUP_H__

#define	LOG_EMERG	0	/* system is unusable */
#define	LOG_ALERT	1	/* action must be taken immediately */
#define	LOG_CRIT	2	/* critical conditions */
#define	LOG_ERR		3	/* error conditions */
#define	LOG_WARNING	4	/* warning conditions */
#define	LOG_NOTICE	5	/* normal but significant condition */
#define	LOG_INFO	6	/* informational */
#define	LOG_DEBUG	7	/* debug-level messages */

extern int g_iLogLevel;

#define LOG(level, format, arg...) \
do{                                    \
    if (level <= g_iLogLevel)       \
    {                                  \
        syslog(level,"[lte_startup] : "format, ##arg);   \
    }                                  \
}while(0);


/*厂商描述符*/
typedef struct 
{
	unsigned short usVendor;    /*厂商标识符*/
	unsigned short usProduct;   /*厂商产品标示符*/
}DEVICE_DESCRIPTOR;

typedef struct 
{
	int iDeviceNum;
	DEVICE_DESCRIPTOR stDevDesc;
	char acModules[256];
}DEVICE_INFO;

#if 0
typedef enum {
	M_LONGCHEER = 0,
	M_LEADCORE,
	M_THINKWILL,
	M_ALTAIR,
	M_ZTE,
    M_ZTE_CDMA2000,
    M_ARCCRA,
    M_THINKWILL_CDMA_MI660,
    M_YUGA_CDMA_CEM600,
    M_ARCCRA_SECOND,
    M_ZTE_MICRO,
    M_HUAWEI,
    M_YUGA,
	/* Donnot modify this value */
	M_TOTAL_NUM
}DEVICE_PLATFORM_E;
#endif
#endif /* __LTE_STARTUP_H__ */

