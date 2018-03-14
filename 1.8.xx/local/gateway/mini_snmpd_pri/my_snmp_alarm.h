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

/*******************************�궨��************************************/
#define MAX_ALARMEXTINFO_LEN    200
#define MAX_EVENTEXTINFO_LEN    200

#define AGENT_ALARM_PORT        1600/*snmpagentdʹ��1600�˿������澯��Ϣ*/
#define AGENT_ALARM_TIMEOUT     1/*snmpagentd�����澯��Ϣ��ʱʱ�䣬1s*/

/* BEGIN: Added by zoucaihong, 2013/9/24 */
#define ALARM_QUEUE_NUM         64
/* ��Ϣ�������� */
#define QUEUE_TYPE_ALARM				((UINT32)1)
#define QUEUE_TYPE_EVENT				((UINT32)2)
/* �����Ϣ�������� */
#define MAX_ALARM_SEND_RATE 3
#define MAX_EVENT_SEND_RATE 3
/* END: Added by zoucaihong, 2013/9/24 */

/******************************* ��Ϣ�Ŷ��� ********************************/
#define EV_AP_REPORT_ALARM_REQ_MSG          ((UINT16) 100)    /* �ϱ��澯���� */
#define EV_AP_REPORT_EVENT_REQ_MSG          ((UINT16) 102)    /* �ϱ��¼����� */

/*****************************�澯״̬����**********************************/
#define AP_ALARM_STATUS_CLEAR           0		/* �澯��� */
#define AP_ALARM_STATUS_RAISE	        1		/* �澯���� */

/******************************** �澯������ ********************************/
#define ALARMLEVEL_FATAL					1		/* ���� */
#define ALARMLEVEL_MAIN		    			2		/* ��Ҫ */
#define ALARMLEVEL_MINOR					3		/* ��Ҫ */
#define ALARMLEVEL_WARNING  				4		/* ���� */

/*****************************�澯�붨��**********************************/
#define ALARM_ID_APCOLDREBOOT       1001    /*AP������*/
#define ALARM_ID_APWARMREBOOT       1002    /*AP������*/
#define ALARM_ID_CPU_OVERLOAD       1003    /*AP CPU�����ʹ��߸澯*/
#define ALARM_ID_MEM_OVERLOAD       1004    /*AP �ڴ������ʹ��߸澯*/
/* BEGIN: Added by zoucaihong, 2014/6/27 */
#define ALARM_ID_DEV_OFFLINE        1005    /* AP���߸澯 */
/* END: Added by zoucaihong, 2014/6/27 */
#define ALARM_ID_COINTERFRE         2001    /*ͬƵ���Ÿ澯*/
#define ALARM_ID_NEINTERFRE         2002    /*��Ƶ���Ÿ澯*/
#define ALARM_ID_WLANDEVINTERFRE    2003    /*WLAN�豸���Ÿ澯*/
#define ALARM_ID_OTHERDEVINTERFRE   2004    /*��WLAN�豸���Ÿ澯*/

/*****************************�ṹ�嶨��**********************************/
/* �澯���� */
typedef struct
{
	unsigned long	    ulAlarmCode;				    /* �澯��� */
    //char                acAlarmObjectID[MAX_OID_LEN];   /* �澯����OID */
    unsigned long	    ulAlarmStatus;					/* �澯״̬ */
    unsigned long	    ulAlarmChangeTime;				/* �澯����/��������ʱ�䣨�ϲ���ʱ�䣩����λ���� */
    unsigned long       ulAlarmSn;                      /* �澯��ˮ�� */
} AP_ALARM_KEY_T;

/* alarm trap msg */
typedef struct
{
    AP_ALARM_KEY_T      stAlarmKey;

    unsigned long	    ulAlarmReasonID;				/* �澯ԭ���� */
    unsigned long	    ulAlarmLevel;					/* �澯���� */

    unsigned long       ulAlarmRaiseTime;				/* �澯����/�������ʱ��(time()�����ķ���ֵ)����λ���� */
    unsigned long	    ulAlarmUpTime;					/* �澯����/�����ԣ��ϵ磩ʱ�䣬tickֵ����λ���� */
    unsigned long	    		ulAlarmClearTime;				/* �澯�������ʱ�� */
    char                acAlarmExtInfo[MAX_ALARMEXTINFO_LEN];
} AP_ALARM_INFO_T;

/* �¼����� */
typedef struct
{
	unsigned long	    ulEventCode;				    /* �¼���� */
    //char                acEventObjectID[MAX_OID_LEN];   /* �¼�����OID */
    unsigned long       ulEventRaiseTime;				/* �¼���������ʱ��(time()�����ķ���ֵ)����λ���� */
    unsigned long       ulEventSn;                      /* �¼���ˮ�� */
} AP_EVNET_KEY_T;

typedef struct
{
    AP_EVNET_KEY_T      stEventKey;

    unsigned long	    ulEventReasonID;				/* �¼�ԭ���� */
    unsigned long	    ulEventUpTime;					/* �¼�������ԣ��ϵ磩ʱ�䣬tickֵ����λ���� */

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

    unsigned long           maxSendRate;    /* ��λʱ�䣨�룩�ڷ��͵��������� */
} AP_QUEUE_T;

typedef struct MsgTag
{
    WORD16          wEvent;     /* ��Ϣ�� */
    WORD16          wLen;       /* ��Ϣ���� */
    struct MsgTag   *lptNext;   /* ��һ��Ϣ */
} TMsgStruc;

/**********************************��������**************************************/
extern UINT32 ap_report_alarm_req_msg(UINT8 *pucMsg, UINT16 usMsgLen);
extern UINT32 snmp_init_alarm_queue(VOID);
UINT32 ap_resend_queue_msg(const AP_QUEUE_T *queue);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __MY_SNMP_ALARM_H__ */
