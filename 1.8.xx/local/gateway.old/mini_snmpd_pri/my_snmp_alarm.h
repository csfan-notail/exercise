/******************************************************************************

  Copyright (C), 2008-2012, GBCOM Co., Ltd.

 ******************************************************************************
  File Name     : my_snmp_alarm.h
  Version       : Initial Draft
  Author        : zoucaihong
  Created       : 2012/11/12
  Last Modified :
  Description   : my_snmp_alarm.c header file
  Function List :
  History       :
  1.Date        : 2012/11/12
    Author      : zoucaihong
    Modification: Created file

******************************************************************************/

/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/

/*----------------------------------------------*
 * module-wide global variables                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macros                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * routines' implementations                    *
 *----------------------------------------------*/

#ifndef __MY_SNMP_ALARM_H__
#define __MY_SNMP_ALARM_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*******************************宏定义************************************/
#define MAX_ALARMEXTINFO_LEN    200
#define MAX_EVENTEXTINFO_LEN    200

#define AGENT_ALARM_PORT        1600/*snmpagentd使用1600端口侦听告警消息*/
#define AGENT_ALARM_TIMEOUT     1/*snmpagentd侦听告警消息超时时间，1s*/

/* BEGIN: Added by zoucaihong, 2013/9/24 */
#define ALARM_QUEUE_NUM         64
/* 消息队列类型 */
#define QUEUE_TYPE_ALARM				((UINT32)1)
#define QUEUE_TYPE_EVENT				((UINT32)2)
/* 最大消息发送速率 */
#define MAX_ALARM_SEND_RATE 3
#define MAX_EVENT_SEND_RATE 3
/* END: Added by zoucaihong, 2013/9/24 */

/******************************* 消息号定义 ********************************/
#define EV_AP_REPORT_ALARM_REQ_MSG          ((UINT16) 100)    /* 上报告警请求 */
#define EV_AP_REPORT_EVENT_REQ_MSG          ((UINT16) 102)    /* 上报事件请求 */

/*****************************告警状态定义**********************************/
#define AP_ALARM_STATUS_CLEAR           0		/* 告警清除 */
#define AP_ALARM_STATUS_RAISE	        1		/* 告警产生 */

/******************************** 告警级别定义 ********************************/
#define ALARMLEVEL_FATAL					1		/* 致命 */
#define ALARMLEVEL_MAIN		    			2		/* 主要 */
#define ALARMLEVEL_MINOR					3		/* 次要 */
#define ALARMLEVEL_WARNING  				4		/* 警告 */

/*****************************告警码定义**********************************/
#define ALARM_ID_APCOLDREBOOT       1001    /*AP冷启动*/
#define ALARM_ID_APWARMREBOOT       1002    /*AP热启动*/
#define ALARM_ID_CPU_OVERLOAD       1003    /*AP CPU利用率过高告警*/
#define ALARM_ID_MEM_OVERLOAD       1004    /*AP 内存利用率过高告警*/
/* BEGIN: Added by zoucaihong, 2014/6/27 */
#define ALARM_ID_DEV_OFFLINE        1005    /* AP离线告警 */
/* END: Added by zoucaihong, 2014/6/27 */
#define ALARM_ID_COINTERFRE         2001    /*同频干扰告警*/
#define ALARM_ID_NEINTERFRE         2002    /*邻频干扰告警*/
#define ALARM_ID_WLANDEVINTERFRE    2003    /*WLAN设备干扰告警*/
#define ALARM_ID_OTHERDEVINTERFRE   2004    /*非WLAN设备干扰告警*/

/*****************************结构体定义**********************************/
/* 告警主键 */
typedef struct
{
	unsigned long	    ulAlarmCode;				    /* 告警编号 */
    //char                acAlarmObjectID[MAX_OID_LEN];   /* 告警对象OID */
    unsigned long	    ulAlarmStatus;					/* 告警状态 */
    unsigned long	    ulAlarmChangeTime;				/* 告警产生/清除最后发生时间（合并后时间），单位：秒 */
    unsigned long       ulAlarmSn;                      /* 告警流水号 */
} AP_ALARM_KEY_T;

/* alarm trap msg */
typedef struct
{
    AP_ALARM_KEY_T      stAlarmKey;

    unsigned long	    ulAlarmReasonID;				/* 告警原因编号 */
    unsigned long	    ulAlarmLevel;					/* 告警级别 */

    unsigned long       ulAlarmRaiseTime;				/* 告警产生/清除绝对时间(time()函数的返回值)，单位：秒 */
    unsigned long	    ulAlarmUpTime;					/* 告警产生/清除相对（上电）时间，tick值，单位：秒 */
    unsigned long	    		ulAlarmClearTime;				/* 告警清除绝对时间 */
    char                acAlarmExtInfo[MAX_ALARMEXTINFO_LEN];
} AP_ALARM_INFO_T;

/* 事件主键 */
typedef struct
{
	unsigned long	    ulEventCode;				    /* 事件编号 */
    //char                acEventObjectID[MAX_OID_LEN];   /* 事件对象OID */
    unsigned long       ulEventRaiseTime;				/* 事件产生绝对时间(time()函数的返回值)，单位：秒 */
    unsigned long       ulEventSn;                      /* 事件流水号 */
} AP_EVNET_KEY_T;

typedef struct
{
    AP_EVNET_KEY_T      stEventKey;

    unsigned long	    ulEventReasonID;				/* 事件原因编号 */
    unsigned long	    ulEventUpTime;					/* 事件产生相对（上电）时间，tick值，单位：秒 */

    char                acEventExtInfo[MAX_EVENTEXTINFO_LEN];
} AP_EVENT_INFO_T;

typedef struct apQueueNode
{
    struct apQueueNode 	*next;
    struct apQueueNode 	*prev;
    
    /* new add */
    struct apQueueNode  *hashNext;
    struct apQueueNode  *hashPrev;

    unsigned long       lastSendTime;   /* the lastest time the msg send in second */
    char                *data;          /* point to the node data */
} AP_QUEUE_NODE_T;

typedef int (*COMPARE_NODE_FUNC)(VOID* source, VOID* dest);

/* new add */
typedef unsigned long (*CALCULATE_HASHKEY_FUNC)(const CHAR *data, UINT32 nodeNum);

typedef struct
{
    AP_QUEUE_NODE_T     freeHead;   /* head of free node list */
    AP_QUEUE_NODE_T     freeTail;   /* tail of free node list */

    AP_QUEUE_NODE_T     usedHead;   /* tail of used node list */
    AP_QUEUE_NODE_T  	usedTail;   /* tail of used node list */

    unsigned long       	msgType;    /* MSG_ALARM / MSG_EVENT */
    COMPARE_NODE_FUNC   	compareNode;/* compare the node */

    unsigned long       	nodeNum;
    unsigned long       	dataSize;   /* size of msg */
    AP_QUEUE_NODE_T     *nodeList;  /* node list for queue */

    CALCULATE_HASHKEY_FUNC  calculateHashKey;  /* calculate node hash key function */
    AP_QUEUE_NODE_T     *hashNodeList;     /* node list for hash table */

	unsigned long			usedNum;		/* used node number */

    unsigned long           maxSendRate;    /* 单位时间（秒）内发送的最大包个数 */
} AP_QUEUE_T;

typedef struct MsgTag
{
    WORD16          wEvent;     /* 消息号 */
    WORD16          wLen;       /* 消息长度 */
    struct MsgTag   *lptNext;   /* 下一消息 */
} TMsgStruc;

/**********************************函数声明**************************************/
extern UINT32 ap_report_alarm_req_msg(UINT8 *pucMsg, UINT16 usMsgLen);
extern UINT32 snmp_init_alarm_queue(VOID);
UINT32 ap_resend_queue_msg(const AP_QUEUE_T *queue);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __MY_SNMP_ALARM_H__ */
