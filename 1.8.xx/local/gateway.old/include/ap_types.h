/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : ap_types.h
  版 本 号   : 初稿
  作    者   : Zhong Huaqing
  生成日期   : 2012年7月23日 星期一
  最近修改   :
  功能描述   : 定义AP基本数据类型和函数返回值
  函数列表   :
  修改历史   :
  1.日    期   : 2012年7月23日 星期一
    作    者   : Zhong Huaqing
    修改内容   : 创建文件

******************************************************************************/

#ifndef __AP_TYPES_H__
#define __AP_TYPES_H__


/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 基础类型定义                                     *
 *----------------------------------------------*/
#ifndef LONG
#define LONG signed long
#endif

#ifndef INT32
#define INT32 signed int
#endif

#ifndef INT16
#define INT16 signed short
#endif

#ifndef INT8
#define INT8  signed char 
#endif

#ifndef UINT32
#define UINT32 unsigned int
#endif

#ifndef ULONG
#define ULONG unsigned long
#endif

#ifndef UINT16
#define UINT16 unsigned short
#endif

#ifndef UINT8
#define UINT8  unsigned char
#endif

#ifndef BOOL
#define BOOL UINT32
#endif

#ifndef CHAR
#define CHAR  INT8
#endif

#ifndef size_t
#define size_t UINT32
#endif

#ifndef VOID
#define VOID void
#endif

#ifndef LOCAL
#define LOCAL  static
#endif

#ifndef REMOTE
#define REMOTE extern
#endif


#ifndef __KERNEL__
typedef char s8;
typedef unsigned char u8;

typedef short s16;
typedef unsigned short u16;

typedef int s32;
typedef unsigned int u32;

#if __WORDSIZE == 64
typedef long s64;
typedef unsigned long u64;
#else
typedef long long s64;
typedef unsigned long long u64;
#endif
#endif
/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
#define TRUE   1
#define FALSE  0

#define OK      0
#define ERROR   1
 
#ifndef NULL
#define NULL 0
#endif
/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

 /*----------------------------------------------*
 * 宏操作                                       *
 *----------------------------------------------*/
#define MAC_LEN 6
#ifndef MAC_FMT
#define MAC_FMT     "%02x:%02x:%02x:%02x:%02x:%02x"
#endif
#ifndef MAC_ARG
#define MAC_ARG(x) ((UINT8*)(x))[0],((UINT8*)(x))[1],((UINT8*)(x))[2],((UINT8*)(x))[3],((UINT8*)(x))[4],((UINT8*)(x))[5]
#endif
#ifndef IP_FMT
#define IP_FMT	    "%u.%u.%u.%u"
#endif
#ifndef IP_ARG
#define IP_ARG(x) ((UINT8*)(x))[0], ((UINT8*)(x))[1], ((UINT8*)(x))[2], ((UINT8*)(x))[3]
#endif
 /*字符串比较*/
#define STR_MATCH(szStr1, szStr2) (0 == strcmp(szStr1, szStr2))
/*限定长度的字符串比较*/
#define STR_NMATCH(szStr1, szStr2, uiSize) (0 == strncmp(szStr1, szStr2, uiSize))
/*内存块比较*/
#define MEM_MATCH(uiAddr1, uiAddr2, uiSize) (0 == memcmp(uiAddr1, uiAddr2, uiSize))

 
/*----------------------------------------------*
 * 枚举定义                                       *
 *----------------------------------------------*/
typedef enum 
{
    AP_E_NONE,                  /* 0, successful */
    AP_E_RADIOID,               /* invalid radio ID */
    AP_E_WLANID,                /* invalid WLAN ID */
    AP_E_IFID,                  /* invalid interface ID */
    AP_E_CMD_ID,                /* invalid command ID */
    AP_E_ASYNC_CMD_ID,          /* invalid async command ID */
    AP_E_EVENT_ID,              /* invalid event ID */
    AP_E_RANGE,                 /* parameter exceed range */
    AP_E_PARAM,                 /* other invalid parameter */
    AP_E_RESOURCE,              /* fail to get resource*/
    AP_E_NOT_EXIST,             /* Object don't exist */
    AP_E_INIT,                  /* Feature not initialized. */
    AP_E_REINIT,                  /* Feature initialized more than once. */
    AP_E_ORDER,                 /* Operator is not in order */
    AP_E_MEM_MALLOC,      /* memory malloc error */
    AP_E_UPGRADE_NOSPACE, /* upgrade no space error */
    AP_E_NULLPTR,               /*an pointer point to NULL*/
    AP_E_INVALID_VALUE,         /*Invalid value*/
    AP_E_INVALID_FILE_NAME,     /*Invalid file name*/
    AP_E_IOCTL_ERR,             /* ioctl operator error */
    AP_E_SYSTEM_FAIL,           /* system call failure */
    AP_E_DEFAULT_FAIL,          /*default failure*/
} AP_ERROR_CODE_E;





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


/*----------------------------------------------*
 * 扩展类型定义                                     *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 结构定义                                       *
 *----------------------------------------------*/



#endif /* __AP_TYPES_H__ */
