/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : ap_types.h
  �� �� ��   : ����
  ��    ��   : Zhong Huaqing
  ��������   : 2012��7��23�� ����һ
  ����޸�   :
  ��������   : ����AP�����������ͺͺ�������ֵ
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��7��23�� ����һ
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __AP_TYPES_H__
#define __AP_TYPES_H__


/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/
extern int g_iSysLogLevel;

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �������Ͷ���                                     *
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

#ifndef UINT64 
#define UINT64 unsigned long long
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

#define OK      0
#define ERROR   1

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


#ifndef __file__
#define __file__ __FILE__
#endif
#ifndef __func__
#define __func__ __FUNCTION__
#endif
#ifndef __line__
#define __line__ __LINE__
#endif


/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/
#ifndef TRUE 
#define TRUE   1
#endif 
#ifndef FALSE
#define FALSE  0
#endif
 
#ifndef NULL
#define NULL 0
#endif

#ifndef ETH_ALEN
#define ETH_ALEN	6		/* Octets in one ethernet addr	 */
#endif 

#ifndef IP_ALEN
#define IP_ALEN		4	   /* Octets in one ip addr	 */
#endif 

#ifndef IPV6_ALEN
#define IPV6_ALEN		16	   /* Octets in one ipv6 addr	 */
#endif 

#ifndef LINE_LEN
#define LINE_LEN	128		/* The length of each line	 */
#endif 

#ifndef CMD_LEN
#define CMD_LEN             128   //ͨ�������
#endif

#ifndef CHAR_LEN_32
#define CHAR_LEN_32   	    32    //ͨ���ַ�������
#endif

#ifndef CHAR_LEN_64
#define CHAR_LEN_64         64
#endif

#ifndef CHAR_LEN_128
#define CHAR_LEN_128        128
#endif

#ifndef CHAR_LEN_256
#define CHAR_LEN_256        256 
#endif

#ifndef LOG_INFO
#define LOG_EMERG       0       /* system is unusable */
#define LOG_ALERT       1       /* action must be taken immediately */
#define LOG_CRIT        2       /* critical conditions */
#define LOG_ERR         3       /* error conditions */
#define LOG_WARNING     4       /* warning conditions */
#define LOG_NOTICE      5       /* normal but significant condition */
#define LOG_INFO        6       /* informational */
#define LOG_DEBUG       7       /* debug-level messages */
#endif

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �����                                       *
 *----------------------------------------------*/
#define MAC_FMT     "%02x:%02x:%02x:%02x:%02x:%02x"
#define MAC_ARG(x) ((UINT8*)(x))[0],((UINT8*)(x))[1],((UINT8*)(x))[2],((UINT8*)(x))[3],((UINT8*)(x))[4],((UINT8*)(x))[5]

#define IP_FMT	    "%u.%u.%u.%u"
#define IP_ARG(x) ((UINT8*)(x))[0], ((UINT8*)(x))[1], ((UINT8*)(x))[2], ((UINT8*)(x))[3]

#define LEARN_FMT   "          %s"
#define LEARN_ARG(x) (x?"enable":"disable")

#define LEASE_FMT   "          %u"

 /*----------------------------------------------*
 * �����                                       *
 *----------------------------------------------*/
 
 /*�ַ����Ƚ�*/
#define STR_MATCH(szStr1, szStr2) (0 == strcmp(szStr1, szStr2))
/*�޶����ȵ��ַ����Ƚ�*/
#define STR_NMATCH(szStr1, szStr2, uiSize) (0 == strncmp(szStr1, szStr2, uiSize))
/*�ڴ��Ƚ�*/
#define MEM_MATCH(uiAddr1, uiAddr2, uiSize) (0 == memcmp(uiAddr1, uiAddr2, uiSize))

#define MAC_FMT         "%02x:%02x:%02x:%02x:%02x:%02x"
#define MAC_AGR(x)      ((unsigned char *)(x))[0], ((unsigned char *)(x))[1], ((unsigned char *)(x))[2], \
                        ((unsigned char *)(x))[3], ((unsigned char *)(x))[4], ((unsigned char *)(x))[5]

#define IP_FMT          "%u.%u.%u.%u"
#define IP_AGR(x)       ((unsigned char*)(x))[0], ((unsigned char*)(x))[1], ((unsigned char*)(x))[2], ((unsigned char*)(x))[3]

#define sys_log(level, format, arg...) \
do{                                    \
    if (level <= g_iSysLogLevel)       \
    {                                  \
        syslog(level, "[line:%d] [func:%s]"format, __LINE__, __FUNCTION__, ##arg);\
    }                                  \
}while(0);

#define App_log(level, format, arg...) \
do{                                    \
    if (level <= LOG_DEBUG)       \
    {                                  \
        syslog(level, "App log: [line:%d] [func:%s]"format, __LINE__, __FUNCTION__, ##arg);\
    }                                  \
}while(0);

/*----------------------------------------------*
 * ö�ٶ���                                       *
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




/*----------------------------------------------*
 * ��չ���Ͷ���                                     *
 *----------------------------------------------*/


/*----------------------------------------------*
 * �ṹ����                                       *
 *----------------------------------------------*/



#endif /* __AP_TYPES_H__ */
