/******************************************************************************
* COPYRIGHT GBCOM
*******************************************************************************
* 功    能: 
* 修改历史: 
* 2012-9-10     冯静              新建
******************************************************************************/

/******************************* 宏定义 ********************************/
#ifndef __MINI_SNMPD_DEF__
#define __MINI_SNMPD_DEF__

/******************************* SNMP字段类型定义 ********************************/
#define TYPE_CHAR		0x01    /* 字符串，多字节 */
#define TYPE_BYTE		0x02    
#define TYPE_SHORT		0x03
#define TYPE_LONG		0x04
#define TYPE_BOOL		0x05    /* 对应TruthValue */
#define TYPE_INT 		0x06
#define TYPE_BIN        0x07
#define TYPE_IP         0x08
#define TYPE_MAC        0x09
#define TYPE_TIME       0x0A
#define TYPE_IPV6       0x0B
#define TYPE_OID        0x0C
#define TYPE_ROWSTATUS  0x0D
#define TYPE_TADDR         0x0E
#define TYPE_LONGLONG      0x0F
#define TYPE_COUNTER32     0x10
#define TYPE_GAUGE32       0x11

/*
 * per registration flags
 */
#define HANDLER_CAN_GETANDGETNEXT     0x01       /* must be able to do both */
#define HANDLER_CAN_SET               0x02           /* implies create, too */
#define HANDLER_CAN_GETBULK           0x04
#define HANDLER_CAN_NOT_CREATE        0x08         /* auto set if ! CAN_SET */
#define HANDLER_CAN_BABY_STEP         0x10
#define HANDLER_CAN_STASH             0x20


#define HANDLER_CAN_RONLY   (HANDLER_CAN_GETANDGETNEXT)
#define HANDLER_CAN_RWRITE  (HANDLER_CAN_GETANDGETNEXT | HANDLER_CAN_SET)
#define HANDLER_CAN_SET_ONLY (HANDLER_CAN_SET | HANDLER_CAN_NOT_CREATE)
#define HANDLER_CAN_DEFAULT (HANDLER_CAN_RONLY | HANDLER_CAN_NOT_CREATE)

#define MIB_R           HANDLER_CAN_RONLY
#define MIB_W           HANDLER_CAN_SET_ONLY
#define MIB_RW          HANDLER_CAN_RWRITE
#define MIB_RC          HANDLER_CAN_RWRITE

#define MIN_OID_LEN	    2
#define MAX_OID_LEN	    128 /* max subid's in an oid */

/******************************* Common definition ********************************/

#ifdef TRUE
#undef TRUE
#define TRUE 1
#else
#define TRUE 1
#endif

#ifdef FALSE
#undef FALSE
#define FALSE 0
#else
#define FALSE 0
#endif

/* BEGIN: Added by zoucaihong, 2012/11/16 */
#define AP_OK           ((unsigned int)0)             /* 成功 */
#define AP_FAIL         ((unsigned int)0xFFFFFFFF)    /* 失败 */

/* END: Added by zoucaihong, 2012/11/16 */

#ifndef UINT8
#define UINT8  unsigned char
#endif

#ifndef UINT16
#define UINT16 unsigned short
#endif

#ifndef UINT32
#define UINT32 unsigned int
#endif

#ifndef UINT64 
#define UINT64 unsigned long long
#endif

#ifndef CHAR
#define CHAR   char
#endif

#ifndef INT32
#define INT32  signed int
#endif

#ifndef BOOL
#define BOOL   int
#endif

#ifndef VOID
#define VOID   void
#endif

#ifndef OID
typedef unsigned long OID;
#endif

#define BYTE   unsigned char
#define WORD16 unsigned short
#define WORD32 unsigned long
#define DWORD  unsigned long

#ifndef LPBYTE
#define LPBYTE unsigned char *
#endif


#ifndef OSTOKEN
#define OSTOKEN WORD32
#endif

#define SNMP_AGENT_MALLOC	malloc
#define SNMP_AGENT_FREE(x) 	\
{                           \
    if (x)                  \
    {                       \
        free(x);            \
        (x) = NULL;         \
    }                       \
}

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)   sizeof(a)/sizeof((a)[0])
#endif

#ifndef OID_LENGTH
#define OID_LENGTH(x)  (sizeof(x)/sizeof(OID))
#endif

#ifndef MAC_FMT
#define MAC_FMT "%02x:%02x:%02x:%02x:%02x:%02x"
#endif

#ifndef MAC_ARG
#define MAC_ARG(x) ((UINT8*)(x))[0], ((UINT8*)(x))[1], ((UINT8*)(x))[2], ((UINT8*)(x))[3], ((UINT8*)(x))[4], ((UINT8*)(x))[5]
#endif

#ifndef IP_FMT
#define IP_FMT	"%u.%u.%u.%u"
#endif

#ifndef IP_ARG
#define IP_ARG(x) ((UINT8*)(x))[0], ((UINT8*)(x))[1], ((UINT8*)(x))[2], ((UINT8*)(x))[3]
#endif

/* BEGIN: Added by zoucaihong, 2013/1/21 */
#define IS_EMPTY_CHAR(x)  ((x) == ' ' || (x) == '\t' || (x) == 10) /* 空格、Tab字符或换行 */
/* END: Added by zoucaihong, 2013/1/21 */

/* 信号量类型 */
#define SEM_COUNT                   0       /* 计数信号量 */
#define SEM_SYNC                    1       /* 同步信号量 */
#define SEM_MUTEX                   2       /* 互斥信号量 */

#define WAIT_FOREVER    0xFFFFFFFF  /* Infinite timeout */
#define NO_WAIT         (WORD32)0   /* don't wait */

extern int errno;

#define OSS_MODULE_VOS 0
#define APP_ERROR 0
#define APP_FATAL 0

/******************************* LOG ********************************/

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

#define LOG_ERROR               LOG_ERR
#define LOG_WARN                LOG_WARNING

#endif
