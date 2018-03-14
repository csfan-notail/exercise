#ifndef __TS_PUBLIC__
#define __TS_PUBLIC__


#ifdef __KERNEL__

#else
#define SIOCGAC		0x8984		/*	*/
#define SIOCSAC		0x8985		/* 	*/
#endif


#include "ap_types.h"

#define IS_TS_CMD(cmd)  ((cmd) >= 300 && (cmd) < 400)

/* ��������ͨ��USRMģ���·� */
#define TS_USRM_ADD_USER_CMD       300
#define TS_USRM_DEL_USER_CMD       301
#define TS_USRM_RESET_USER_CMD     302

/* AAA�·����� */
#define TS_AAA_GET_MULTI_USER_CMD   303
#define TS_AAA_GET_USER_STAT_CMD    304
#define TS_AAA_SET_USER_STAT_CMD    310

/* ��������ͨ��L2W�·� */
#define TS_DEVM_ADD_LANBR_CMD       305
#define TS_DEVM_DEL_LANBR_CMD       306
#define TS_DEVM_ADD_WANBR_CMD       307
#define TS_DEVM_DEL_WANBR_CMD       308

/* ���Խӿ� */
#define TS_PRINT_SET                309

#define TS_PORT_NAME_LEN        8


typedef enum
{
    TS_DIR_DS = 0,
    TS_DIR_US
}TS_DIR_E;

typedef enum
{
    TS_DIR_RX = 0,
    TS_DIR_TX
}TS_INET_DIR_E;


/* �û�̬TSģ����USRM��Ľӿڣ� ֪ͨTSģ����Ҫ����ͳ�Ƶ��û�mac��ַ */

typedef struct
{
    #define TS_VERSION_SIZE  128
    CHAR acModVersion[TS_VERSION_SIZE];    /*�ں�ģ��汾��*/
    CHAR acModDepVersion[TS_VERSION_SIZE]; /*֧���ں�ģ���APPģ����С�汾��*/
}TS_VERSION_T;

typedef struct
{
    UINT32 uiDebugLevel;
}TS_DEBUG_T;

typedef struct
{
    UINT32 uiTsSwitch;
    #define TS_OFF  0
    #define TS_ON   1
}TS_SWITCH_T;

typedef struct{
    UINT32 uiGearByte;
    UINT32 uiFlag;
#define BR_GEAR     2
#define INET_GEAR   3
}TS_GEAR_T;



typedef struct {
    UINT8   aucUserMac[6]; 
} TS_USER_T;


typedef struct {
    CHAR   acDevName[TS_PORT_NAME_LEN]; 
    UINT32  uiLayer;
} TS_DEV_T;

typedef struct{
    UINT32 uiLayer;
    UINT32 uiRxPkts;
    UINT32 uiTxPkts;
    UINT32 uiRxGigaWords;
    UINT32 uiRxBytes;
    UINT32 uiTxGigaWords;
    UINT32 uiTxBytes;

    UINT32 uiTxBytesLast;
    UINT32 uiRxBytesLast;
}TS_IPV6_T;

/* TSģ��ͳ�ƹ��ӵĽṹ�� */
typedef struct 
{
    UINT8 aucUserMac[6];
    UINT32 uiRxPkts;
    UINT32 uiTxPkts;
    UINT32 uiRxGigaWords;
    UINT32 uiRxBytes;
    UINT32 uiTxGigaWords;
    UINT32 uiTxBytes;
    
    UINT32 uiTxBytesLast;
    UINT32 uiRxBytesLast;

    UINT32 uiRxAppBytes;
    UINT32 uiTxAppBytes;
    UINT32 uiRxAppGigaWords;
    UINT32 uiTxAppGigaWords;
    UINT32 uiTxAppBytesLast;
    UINT32 uiRxAppBytesLast;

    TS_IPV6_T stIpv6Ts;
    ULONG  ulRandom;
} TS_USER_STATISTIC_T;


typedef struct 
{
    CHAR acDevName[TS_PORT_NAME_LEN];
    UINT32 uiLayer;
    UINT32 uiRxPkts;
    UINT32 uiTxPkts;
    UINT32 uiRxGigaWords;
    UINT32 uiRxBytes;
    UINT32 uiTxGigaWords;
    UINT32 uiTxBytes;
    
    UINT32 uiTxBytesLast;
    UINT32 uiRxBytesLast;

    UINT32 uiRxAppBytes;
    UINT32 uiTxAppBytes;
    UINT32 uiRxAppGigaWords;
    UINT32 uiTxAppGigaWords;
    UINT32 uiTxAppBytesLast;
    UINT32 uiRxAppBytesLast;

    TS_IPV6_T stIpv6Ts;
    ULONG  ulRandom;
} TS_DEV_STATISTIC_T;

typedef struct{
    UINT32 uiRxGigaWords;
    UINT32 uiRxBytes;
    UINT32 uiTxGigaWords;
    UINT32 uiTxBytes;
}TS_INFO_T;

typedef struct{
    TS_INFO_T stIPv4;
    TS_INFO_T stIPv6;
}TS_LTE_T;


/* �ں�̬ͳ��ģ����Ʒ�ģ���ӿڽṹ��:���ͳ����Ϣ(Usr) */
typedef struct
{
    #define TS_USER_STAT_LIST_SIZE 128
    TS_USER_STATISTIC_T astUserStatList[TS_USER_STAT_LIST_SIZE ];
    UINT32  uiUserNum;  /* ʵ�ʻ�ȡ�����û���Ŀ */
    
}TS_USER_ENTRY_LIST_T;

/* �ں�̬ͳ��ģ����Ʒ�ģ���ӿڽṹ��:���ͳ����Ϣ(Dev) */
typedef struct
{
    #define TS_DEV_STAT_LIST_SIZE 32
    TS_DEV_STATISTIC_T astDevStatList[TS_DEV_STAT_LIST_SIZE ];
    UINT32  uiDevNum;  /* ʵ�ʻ�ȡ���Ķ˿���Ŀ */
    TS_LTE_T stLte;
}TS_DEV_ENTRY_LIST_T;



#endif
