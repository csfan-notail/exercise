#include <syslog.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "mini_snmpd.h"

#include "my_snmp_def.h"
#include "my_snmp_data.h"
#include "my_snmp_alarm.h"
#include "my_snmp_handle.h"
#include "my_snmp_util.h"

AP_QUEUE_T      g_alarmQueue;
AP_QUEUE_T      g_eventQueue;
AP_QUEUE_T      g_activeAlarmQueue;
extern OSTOKEN gtSemAlarm;     /* �澯�����ź��� */

extern UINT32 snmp_get_systime(VOID);
extern BOOL ap_agent_send_alarm_trap(AP_ALARM_INFO_T *pstAlarm); 
/*************************************************************************
* ��������: ac_oma_print_alarm_key
* ��    ��: ��ӡ�澯��Ϣ
* ��    ��:
* ��������			    ����/���	    ����
* byLevel				IN				��ӡ����
* msg                   IN			    �澯��Ϣ
* ��������: ��
* ˵    ��:
*************************************************************************/
VOID ap_print_alarm_key(BYTE byLevel, AP_ALARM_KEY_T *msg)
{
	if (msg == NULL)
	{
		return;
	}

	lprintf(byLevel,
		"ulAlarmCode       = %lu\n"
		"ulAulAlarmStatus  = %lu\n"
		"ulAlarmChangeTime = %lu\n"
		"ulAlarmSn         = %lu",
		msg->ulAlarmCode,
		msg->ulAlarmStatus,
		msg->ulAlarmChangeTime,
		msg->ulAlarmSn);
}
/*************************************************************************
* ��������: ac_oma_locate_activealarm_msgc
* ��    ��: ���ݸ澯�롢�澯�����ʶ���澯״̬�����Ѿ����ڵĸ澯
* ��    ��:
* ��������			����/���				����
* msg               IN                      �¼���Ϣ
* ��������: NULL    ʧ��
            other   ���нڵ�ָ��
* ˵    ��:
*************************************************************************/
VOID* ap_locate_activealarm_msg(const AP_ALARM_INFO_T *msg)
{
    /* �ֲ��������� */
    AP_QUEUE_T      *queue = &g_activeAlarmQueue;
    AP_QUEUE_NODE_T *node = NULL;

    /* new add */
    UINT32          ulHashKey = 0;

    /* �������Ϸ��� */
    if (NULL == msg)
    {
        lprintf(LOG_ERROR, "ap_locate_activealarm_msg: msg is null!");
        return NULL;
    }

    /*
     * �Ӹ澯���������ǰ���ң�
     * ԭ���Ǻϲ����һ�η����ĸ澯��
     * �����Ϣ�Ǹ澯�ϱ���Ϣ�����������ҵ����Ǹ澯�����Ϣ���򲻺ϲ�
     * �����Ϣ�Ǹ澯�����Ϣ�����������ҵ����Ǹ澯��Ϣ���򲻺ϲ�
     * ������кϲ�
     */
    ulHashKey = queue->calculateHashKey((const CHAR *)msg, queue->nodeNum);

    /* check ulHashKey validity */
    if (ulHashKey >= queue->nodeNum)
    {
        lprintf(LOG_ERROR, "ap_locate_activealarm_msg:ulHashKey must be between 0 and %lu.",
            queue->nodeNum);
        return NULL;
    }

    node = queue->hashNodeList[ulHashKey].hashNext;
    if (node == NULL)
    {
        return NULL;
    }

    while (node->hashNext != NULL)
    {
        node = node->hashNext;
    }

    while (node != NULL && node->hashPrev != NULL)
    {
        if (queue->compareNode((VOID*)msg, node->data))
        {
            return node;
        }

        node = node->hashPrev;
    }

    return NULL;
}
/*************************************************************************
* ��������: ac_oma_locate_alarm_msgc
* ��    ��: ���ݸ澯�롢�澯�����ʶ���澯״̬�����Ѿ����ڵĸ澯
* ��    ��:
* ��������			����/���				����
* msg               IN                      �¼���Ϣ
* ��������: NULL    ʧ��
            other   ���нڵ�ָ��
* ˵    ��:
*************************************************************************/
VOID* ap_locate_alarm_msg(const AP_ALARM_INFO_T *msg)
{
    /* �ֲ��������� */
    AP_QUEUE_T      *queue = &g_alarmQueue;
    AP_QUEUE_NODE_T *node = NULL;

    /* new add */
    UINT32          ulHashKey = 0;

    /* �������Ϸ��� */
    if (NULL == msg)
    {
        lprintf(LOG_ERROR, "ap_locate_alarm_msg: msg is null!");
        return NULL;
    }

	/* �������ʹ����С��50%�� �򲻽��кϲ�����Ϊ�ϲ���ᵼ������ӡ�� */
	if (queue->usedNum <= (queue->nodeNum >> 1) )
	{
		return NULL;
	}

    /*
     * �Ӹ澯���������ǰ���ң�
     * ԭ���Ǻϲ����һ�η����ĸ澯��
     * �����Ϣ�Ǹ澯�ϱ���Ϣ�����������ҵ����Ǹ澯�����Ϣ���򲻺ϲ�
     * �����Ϣ�Ǹ澯�����Ϣ�����������ҵ����Ǹ澯��Ϣ���򲻺ϲ�
     * ������кϲ�
     */

    /* new add */
    ulHashKey = queue->calculateHashKey((const CHAR *)msg, queue->nodeNum);

    /* check ulHashKey validity */
    if (ulHashKey >= queue->nodeNum)
    {
        lprintf(LOG_ERROR, "ulHashKey must be between 0 and %lu.",
            queue->nodeNum);
        return NULL;
    }

    node = queue->hashNodeList[ulHashKey].hashNext;
    if (node == NULL)
    {
        return NULL;
    }

    while (node->hashNext != NULL)
    {
        node = node->hashNext;
    }

	/* miss the index list head */
    while (node != NULL && node->hashPrev != NULL)
    {
        if (queue->compareNode((VOID*)msg, node->data))
        {
            return node;
        }

        node = node->hashPrev;
    }

    return NULL;
}
/*************************************************************************
* ��������: ac_oma_add_node_to_queue
* ��    ��: ����Ϣ�ŵ������У�����������
* ��    ��:
* ��������			����/���				����
* queue             IN                      ��Ϣ����
* msg               IN                      ��Ϣ����
* ��������: AC_OK: �ɹ�
            AC_FAIL:  ʧ��
* ˵    ��:
*************************************************************************/
UINT32 ap_add_node_to_queue(AP_QUEUE_T *queue, const CHAR* msg)
{
    /* �ֲ��������� */
    AP_QUEUE_NODE_T* node = NULL;
    AP_QUEUE_NODE_T* lastFreeNode = NULL;
    AP_QUEUE_NODE_T* lastUsedNode = NULL;

    /* new add */
    UINT32           ulHashKey = 0;
    AP_QUEUE_NODE_T* locateHashNode = NULL;

    /* ��������Ч�� */
    if (NULL == queue || NULL == msg)
    {
        lprintf(LOG_ERROR, "ap_add_node_to_queue: queue or msg is null!");
        return AP_FAIL;
    }

    /* if there's no free data node, then remove the first used node */
    if (queue->freeHead.next == &queue->freeTail)
    {
        node = queue->usedHead.next;

        /* free the node from used link list */
        queue->usedHead.next = node->next;
        node->next->prev = &queue->usedHead;

        /* add the node to free link list */
        lastFreeNode = queue->freeTail.prev;

        lastFreeNode->next = node;
        node->prev = lastFreeNode;

        node->next = &queue->freeTail;
        queue->freeTail.prev = node;

        /* new add */
        ulHashKey = queue->calculateHashKey(node->data, queue->nodeNum);

        /* check ulHashKey validity */
        if (ulHashKey >= queue->nodeNum)
        {
            lprintf(LOG_ERROR, "ap_add_node_to_queue:ulHashKey is between 0 and %lu.",
                queue->nodeNum);
            return AP_FAIL;
        }

        locateHashNode = queue->hashNodeList[ulHashKey].hashNext;
        while (locateHashNode != NULL)
        {
            if (queue->compareNode(node->data, locateHashNode->data))
            {
                break;
            }

            locateHashNode = locateHashNode->hashNext;
        }

        if (locateHashNode != NULL)
        {
            if (locateHashNode->hashNext != NULL)
            {
                locateHashNode->hashNext->hashPrev = locateHashNode->hashPrev;
            }

            if (locateHashNode->hashPrev != NULL)
            {
                locateHashNode->hashPrev->hashNext = locateHashNode->hashNext;
            }
        }

		queue->usedNum --;
    }

    /* new add */
    ulHashKey = queue->calculateHashKey(msg, queue->nodeNum);

    /* check ulHashKey validity */
    if (ulHashKey >= queue->nodeNum)
    {
        lprintf(LOG_ERROR, "ap_add_node_to_queue:ulHashKey is between 0 and %lu.",
            queue->nodeNum);
        return AP_FAIL;
    }

    /* add the nodeData */
    node = queue->freeHead.next;

    /* free the node from free link list */
    queue->freeHead.next = node->next;
    node->next->prev = &queue->freeHead;

    /* add the node to used link list */
    lastUsedNode = queue->usedTail.prev;

    lastUsedNode->next = node;
    node->prev = lastUsedNode;

    node->next = &queue->usedTail;
    queue->usedTail.prev = node;

	if (NULL == node->data)
	{
		node->data = SNMP_AGENT_MALLOC( queue->dataSize);
		if (NULL == node->data)
		{
			agent_printf(LOG_ERROR, "malloc pucData fail.\n");
			return AP_FAIL;
		}
		memset(node->data, 0, queue->dataSize);
	}

    memcpy(node->data, msg, queue->dataSize);
    node->lastSendTime = 0;

    locateHashNode = &(queue->hashNodeList[ulHashKey]);
    while (locateHashNode->hashNext != NULL)
    {
        locateHashNode = locateHashNode->hashNext;
    }

    locateHashNode->hashNext = node;
    node->hashPrev = locateHashNode;
    node->hashNext = NULL;

	queue->usedNum ++;

    return AP_OK;
}

/*************************************************************************
* ��������: ac_oma_remove_node_from_queue
* ��    ��: ������Ϣ���ݣ�����Ϣ������ɾ����Ӧ����Ϣ
* ��    ��:
* ��������			����/���				����
* queue             IN                      ��Ϣ����
* msg               IN                      �������ݣ�ָ��������Ϣ���ݣ�
* ��������: AC_OK �ɹ�
            AC_FAIL ����
*           AC_OMA_QUEUE_NODE_NOT_EXIST �ڵ㲻����
*
* ˵    ��:
*************************************************************************/
UINT32 ap_remove_node_from_queue(AP_QUEUE_T *queue, CHAR* msg)
{
    /* �ֲ��������� */
    AP_QUEUE_NODE_T  *node = NULL;
    AP_QUEUE_NODE_T  *firstFreeNode = NULL;
    INT32			 located = -1;

    /* new add */
    UINT32           ulHashKey = 0;

    /* �������Ϸ��� */
    if (NULL == queue || NULL == msg)
    {
        lprintf(LOG_ERROR, "ap_remove_node_from_queue: queue or msg is null!");
        return AP_FAIL;
    }

    /* new add */
    ulHashKey = queue->calculateHashKey(msg, queue->nodeNum);

    /* check ulHashKey validity */
    if (ulHashKey >= queue->nodeNum)
    {
        lprintf(LOG_ERROR, "ap_remove_node_from_queue:ulHashKey is between 0 and %lu.",
            queue->nodeNum);
        return AP_FAIL;
    }

    node = queue->hashNodeList[ulHashKey].hashNext;
    while (node != NULL)
    {
        if (queue->compareNode(msg, node->data))
        {
            located = 0;
            break;
        }

        node = node->hashNext;
    }

    /* locate for the node */
    /*node = queue->usedHead.next;
    while(node != &queue->usedTail)
    {
        if (queue->compareNode(msg, node->data))
        {
            located = 0;
            break;
        }

        node = node->next;
        if (NULL == node)
        {
            ModuleLogMsg(APP_MODULE_OMA, APP_ERROR, "ac_oma_remove_node_from_queue: node is null!");
            return AC_FAIL;
        }
    }*/

    /* not located */
    if (located != 0)
    {
        lprintf(LOG_ERROR, "ap_remove_node_from_queue: the node to be removed is not existed");
        return AP_FAIL;
    }

    /* remove the node from used node link list */
    node->prev->next = node->next;
    node->next->prev = node->prev;

    /* add the node to the free node link list */
    firstFreeNode = queue->freeHead.next;

    queue->freeHead.next = node;
    node->prev = &queue->freeHead;

    node->next = firstFreeNode;
    firstFreeNode->prev = node;

    /* new add */
    if (node->hashPrev != NULL)
    {
        node->hashPrev->hashNext = node->hashNext;
    }

    if (node->hashNext != NULL)
    {
        node->hashNext->hashPrev = node->hashPrev;
    }
    if(node->data)
        free(node->data);
	/* leijie 2008-09-12 */
	queue->usedNum --;

    return AP_OK;
}

/*************************************************************************
* ��������: ac_oma_add_activealarm_msg_to_queue
* ��    ��: ����Ծ�澯��Ϣ�ŵ���Ծ�澯������
* ��    ��:
* ��������			����/���				����
* msg               IN                      �澯��Ϣ
* ��������: AC_OK �ɹ�
            AC_FAIL  ʧ��
* ˵    ��: ����澯���ڣ���ϲ�����Ϣ
*************************************************************************/
UINT32 ap_add_activealarm_msg_to_queue(const AP_ALARM_INFO_T *msg)
{
    /* �ֲ��������� */
    UINT32           ulRet = AP_FAIL;
    AP_QUEUE_NODE_T  *node = NULL;
    AP_ALARM_INFO_T  *nodeData = NULL;
    AP_QUEUE_T       *queue = &g_activeAlarmQueue;

    /* �������Ϸ��� */
    if (NULL == msg)
    {
        lprintf(LOG_ERROR, "ap_add_activealarm_msg_to_queue: msg is null!");
        return AP_FAIL;
    }

    /* ��λ�澯��Ϣ������Ƿ��Ѿ��ڶ����д��ڣ�������ڣ���Ҫ�ϲ� */
    node = (AP_QUEUE_NODE_T*)ap_locate_activealarm_msg(msg);
    if (node != NULL)
    {
        /* ��λ������ϲ�������ʱ�䣬��ˮ�ţ�ԭ���룬�ȼ�����չ��Ϣ��������Ժ;���ʱ�䲻�� */
        if (node->data != NULL)
        {
            nodeData = (AP_ALARM_INFO_T*)node->data;
        }
        else
        {
            lprintf(LOG_ERROR, "node->data is NULL!");
            return AP_FAIL;
        }

        nodeData->stAlarmKey.ulAlarmChangeTime = msg->stAlarmKey.ulAlarmChangeTime;
        nodeData->stAlarmKey.ulAlarmSn = msg->stAlarmKey.ulAlarmSn;

        nodeData->ulAlarmReasonID = msg->ulAlarmReasonID;
        nodeData->ulAlarmLevel = msg->ulAlarmLevel;
		nodeData->ulAlarmClearTime = msg->ulAlarmClearTime;
        strcpy(nodeData->acAlarmExtInfo, msg->acAlarmExtInfo);

        return AP_OK;
    }

    /* δ��λ�� */
    ulRet = ap_add_node_to_queue(queue, (CHAR*)msg);
    if (AP_FAIL == ulRet)
    {
        lprintf(LOG_ERROR, "ap_add_node_to_queue failed!");
        return AP_FAIL;
    }

    return AP_OK;
}
/*************************************************************************
* ��������: ac_oma_remove_activealarm_msg_from_queue
* ��    ��: ����Ծ�澯��Ϣ�ӻ�Ծ�澯������ɾ��
* ��    ��:
* ��������			����/���				����
* msg               IN                      �澯��Ϣ
* ��������: AC_OK �ɹ�
            AC_FAIL  ʧ��
* ˵    ��:
*************************************************************************/
UINT32 ap_remove_activealarm_msg_from_queue(AP_ALARM_KEY_T *msg)
{
    /* �ֲ��������� */
    UINT32  ulRet = AP_FAIL;

    /* �������Ϸ��� */
    if (NULL == msg)
    {
        lprintf(LOG_ERROR, "ap_remove_activealarm_msg_from_queue: msg is null!");
        return AP_FAIL;
    }

    /* ��ָ���澯��Ϣ����Ϣ������ɾ�� */
    ulRet = ap_remove_node_from_queue(&g_activeAlarmQueue, (CHAR*)msg);
    if (AP_OK != ulRet)
    {
        lprintf(LOG_ERROR, "ap_remove_node_from_queue failed!");
		ap_print_alarm_key(LOG_ERROR, msg);

        return AP_FAIL;
    }

    return AP_OK;
}

/*************************************************************************
* ��������: ac_oma_add_alarm_msg_to_queue
* ��    ��: ���澯�ϱ�/�����Ϣ�ŵ��澯������
* ��    ��:
* ��������			����/���				����
* msg               IN                      �澯��Ϣ
* ��������: AC_OK �ɹ�
            AC_FAIL  ʧ��
* ˵    ��: ����澯���ڣ���ϲ�����Ϣ
*************************************************************************/
UINT32 ap_add_alarm_msg_to_queue(const AP_ALARM_INFO_T *msg)
{
    /* �ֲ��������� */
    UINT32           ulRet = AP_FAIL;
    AP_QUEUE_NODE_T  *node = NULL;
    AP_ALARM_INFO_T  *nodeData = NULL;
    AP_QUEUE_T       *queue = NULL;

    /* �������Ϸ��� */
    if (NULL == msg)
    {
        lprintf(LOG_ERROR, "ap_add_alarm_msg_to_queue: msg is null!");
        return AP_FAIL;
    }

    /* ��λ�澯��Ϣ������Ƿ��Ѿ��ڶ����д��ڣ�������ڣ���Ҫ�ϲ� */
    node = (AP_QUEUE_NODE_T*)ap_locate_alarm_msg(msg);
    if (node != NULL)
    {
        /* ��λ������ϲ�������ʱ�䣬��ˮ�ţ�ԭ���룬�ȼ�����չ��Ϣ��������Ժ;���ʱ�䲻�� */
        if (node->data != NULL)
        {
            nodeData = (AP_ALARM_INFO_T*)node->data;
        }
        else
        {
            lprintf(LOG_ERROR, "ap_add_alarm_msg_to_queue:node->data is NULL!");
            return AP_FAIL;
        }

        nodeData->stAlarmKey.ulAlarmChangeTime = msg->stAlarmKey.ulAlarmChangeTime;
        nodeData->stAlarmKey.ulAlarmSn = msg->stAlarmKey.ulAlarmSn;

        nodeData->ulAlarmReasonID = msg->ulAlarmReasonID;
        nodeData->ulAlarmLevel = msg->ulAlarmLevel;
		nodeData->ulAlarmClearTime = msg->ulAlarmClearTime;
        strcpy(nodeData->acAlarmExtInfo, msg->acAlarmExtInfo);

        return AP_OK;
    }

    /* δ��λ�������澯��Ϣ���뵽�澯���� */
	if (!OsSemaphoreP(gtSemAlarm, WAIT_FOREVER, 1))
	{
		agent_printf(LOG_ERR, "gtSemAlarm OsSemaphoreP fail\n");
    	return AP_FAIL;
	}
    queue = &g_alarmQueue;
    ulRet = ap_add_node_to_queue(queue, (CHAR*)msg);
    if (ulRet != AP_OK)
    {
		OsSemaphoreV(gtSemAlarm, 1);
        lprintf(LOG_ERROR, "ap_add_node_to_queue failed!");
        return AP_FAIL;
    }
	OsSemaphoreV(gtSemAlarm, 1);

    return AP_OK;
}

UINT32 ap_report_alarm_req_msg(UINT8 *pucMsg, UINT16 usMsgLen)
{
	/* �ֲ��������� */
	AP_ALARM_INFO_T stAlarmMsg;
    UINT32 ulRet = AP_FAIL;
    //UINT32 ulTime = 0;

	/* �������Ϸ��� */
	if (NULL == pucMsg)
	{
        lprintf(LOG_ERR, "pucMsg is NULL.");
        return FALSE;
	}

	if (usMsgLen != sizeof(AP_ALARM_INFO_T))
	{
		lprintf(LOG_ERR, "usMsgLen:%u is invalid,shoule be %u.",
			usMsgLen,
			sizeof(AP_ALARM_INFO_T));
		return FALSE;
	}

    memset(&stAlarmMsg, 0, sizeof(AP_ALARM_INFO_T));
    memcpy(&stAlarmMsg, pucMsg, sizeof(AP_ALARM_INFO_T));
    /* ��Ϣ������� */
    ulRet = ap_add_alarm_msg_to_queue(&stAlarmMsg);
    if (ulRet != AP_OK)
    {
        lprintf(LOG_ERR, 
                "ap_add_alarm_msg_to_queue failed.\n");
        return AP_FAIL;
    }
	
#if 0
    if (stAlarmMsg.stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_CLEAR)
	{					
        /* delete active alarm fromm active alarm queue */
        /*ulRet = ap_remove_activealarm_msg_from_queue(&(stAlarmMsg.stAlarmKey));
        if (AP_OK != ulRet)
        {
            lprintf(LOG_ERROR, "ap_remove_activealarm_msg_from_queue failed!");
            return FALSE;
        }*/

        /*���͸澯�����Ϣ*/
        bRet = ap_agent_send_alarm_trap(&stAlarmMsg);
        if ( !bRet )
        {
            lprintf(LOG_ERROR, "ap_agent_send_alarm_trap failed!");
            return FALSE;
        }
        
		return TRUE;
	}
	else if(stAlarmMsg.stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_RAISE)
	{
	    /*���͸澯��Ϣ*/
        ulTime = snmp_get_systime();
        bRet = ap_agent_send_alarm_trap(&stAlarmMsg);
        if ( !bRet )
        {
            lprintf(LOG_ERROR, "ap_agent_send_alarm_trap failed!");
        }
        else
        {
            stAlarmMsg.stAlarmKey.ulAlarmChangeTime = ulTime;
        }
        
        /*����Ծ�澯���뵽��Ծ�澯����*/
        /*ulRet = ap_add_activealarm_msg_to_queue(&(stAlarmMsg.stAlarmKey));
        if (AP_OK != ulRet)
        {
            lprintf(LOG_ERROR, "ap_add_activealarm_msg_to_queue failed!");
            return FALSE;
        }*/
        
		return TRUE;
	}

	lprintf(LOG_ERROR, "ap_report_alarm_req_msg:invalid ulAlarmStatus:%lu.",
		stAlarmMsg.stAlarmKey.ulAlarmStatus);
    
	return FALSE;
#endif
    return AP_OK;
}
UINT32 ap_send_queue_node_msg(AP_QUEUE_NODE_T *pstNode)
{
    agent_printf(LOG_INFO, "into ap_send_queue_node_msg");
    /* �ֲ��������� */
	AP_ALARM_INFO_T stAlarmMsg;
    UINT32  ret = AP_FAIL;

    /* ��������Ч�� */
    if (NULL == pstNode)
    {
        agent_printf(LOG_ERROR, "pstNode is null!");
        return AP_FAIL;
    }

    if (NULL == pstNode->data)
    {
        agent_printf(LOG_ERROR, "pstNode's data is null!");
        return AP_FAIL;
    }

    memset(&stAlarmMsg, 0, sizeof(AP_ALARM_INFO_T));
    memcpy(&stAlarmMsg, pstNode->data, sizeof(AP_ALARM_INFO_T));

    ap_agent_send_alarm_trap(&stAlarmMsg);
    ret = ap_remove_node_from_queue(&g_alarmQueue, pstNode->data);
    if (AP_OK != ret)
    {
        agent_printf(LOG_ERROR, "ap_remove_node_from_queue failed!");
    }
    
    return AP_OK;
}
UINT32 ap_resend_queue_msg(const AP_QUEUE_T *queue)
{
    //agent_printf(LOG_INFO, "into ap_resend_queue_msg");
    /* �ֲ��������� */
    AP_QUEUE_NODE_T     *currNode = NULL;
    AP_QUEUE_NODE_T     *nextNode = NULL;
    UINT32                  ulSendNum = 0;
    UINT32                  ulMaxSendNum = 0;

	/* ��������Ч�� */
	if (NULL == queue)
	{
		agent_printf(LOG_ERROR, "queue is null!");
		return AP_FAIL;
	}

    ulMaxSendNum = queue->maxSendRate;

    currNode = queue->usedHead.next;
    while(currNode != &queue->usedTail)
    {
        nextNode = currNode->next;
        {
            ap_send_queue_node_msg(currNode);

            ulSendNum ++;
            if (ulSendNum >= ulMaxSendNum)
            {
                return AP_OK;
            }
        }

        currNode = nextNode;
        if (NULL == currNode)
        {
            agent_printf(LOG_ERROR, "currNode is null!");
            return AP_FAIL;
        }
    }

    return AP_OK;
}
UINT32 ap_init_alarm_queue
(
    AP_QUEUE_T          *queue,
    UINT32              nodeNum,
    UINT32              dataSize,
    UINT32              ulQueueType,
    UINT32              ulMaxSendRate,
    COMPARE_NODE_FUNC   compareNodeFunc,
    CALCULATE_HASHKEY_FUNC calculateHashKeyFunc
)
{
    /* �ֲ��������� */
    const int       nodeSize = sizeof(AP_QUEUE_NODE_T);
    UINT32          i = 0;

    UINT8           *memAddr = NULL;
    AP_QUEUE_NODE_T    *node;
    AP_QUEUE_NODE_T    *nextNode = NULL;

    /* ��������Ч�� */
    if (0 == dataSize)
    {
        agent_printf(LOG_ERR, "dataSize is 0!");
        return AP_FAIL;
    }

    if (NULL == compareNodeFunc)
    {
        agent_printf(LOG_ERR, "compareNodeFunc is null!");
        return AP_FAIL;
    }

    /* alloc memory for node list */
    queue->nodeNum = nodeNum;
    queue->dataSize = dataSize;

    memAddr = SNMP_AGENT_MALLOC( (nodeSize + nodeSize) * nodeNum );
    if (NULL == memAddr)
    {
        agent_printf(LOG_ERR, "alloc memory for queue failed!");
        return AP_FAIL;
    }

    /* init the compare node function for locate the node */
    queue->msgType = ulQueueType;
    queue->maxSendRate = ulMaxSendRate;
    queue->compareNode = compareNodeFunc;

    /* new add */
    queue->calculateHashKey = calculateHashKeyFunc;

    /* init all node */
    queue->nodeList = (AP_QUEUE_NODE_T*)memAddr;

    /* new add */
    queue->hashNodeList = (AP_QUEUE_NODE_T*)(memAddr + nodeNum * nodeSize);

    /* init free head node */
    queue->freeHead.prev = NULL;

    /* init free data node */
    node = queue->nodeList;
    node->prev = &queue->freeHead;
    node->prev->next = node;
    node->lastSendTime = 0;
    node->data = NULL;

    for(i=1; i<queue->nodeNum; i++)
    {
        nextNode = node + 1;

        node->next = nextNode;
        nextNode->prev = node;

        nextNode->lastSendTime = 0;
        nextNode->data = NULL;

        node++;
    }

    /* init free tail node */
    node->next = &queue->freeTail;
    queue->freeTail.prev = node; /* here node is actually the last free data node */
    queue->freeTail.next = NULL;

    /* init used head and tail node */
    queue->usedHead.next = &queue->usedTail;
    queue->usedHead.prev = NULL;

    queue->usedTail.next = NULL;
    queue->usedTail.prev = &queue->usedHead;

    /* new add */
    node = queue->hashNodeList;
    node->hashPrev = NULL;
    node->hashNext = NULL;
    for (i = 1; i<queue->nodeNum; i++)
    {
        nextNode = node + 1;
        nextNode->hashPrev = NULL;
        nextNode->hashNext = NULL;

        node++;
    }

	/* leijie 2008-09-12 */
	queue->usedNum = 0;

    return AP_OK;
}
BOOL ap_is_same_alarm(const VOID *source, const VOID *dest)
{
    /* �������Ϸ��� */
    if (NULL == source || NULL == dest)
    {
        agent_printf(LOG_ERROR, "source or dest is null!");
        return FALSE;
    }

    if (0 == memcmp(source, dest, sizeof(AP_ALARM_KEY_T)-sizeof(UINT32)*2))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
UINT32  ap_calc_hash_key_alarm(const CHAR *msg, UINT32 ulNodeNum)
{
    /* �ֲ��������� */
    AP_ALARM_INFO_T *pstAlarmInfo = NULL;
	UINT32       ulHash = 0;

    /* �������Ϸ��� */
    if (NULL == msg)
    {
        agent_printf(LOG_ERROR, "msg is null!");
        return ulNodeNum;
    }

    if (ulNodeNum == 0)
    {
        agent_printf(LOG_ERROR, "ulNodeNum is 0.");
        return ulNodeNum;
    }

    pstAlarmInfo = (AP_ALARM_INFO_T*)msg;

    ulHash += pstAlarmInfo->stAlarmKey.ulAlarmCode;
    ulHash %= ulNodeNum;
    return ulHash;
}
UINT32 snmp_init_alarm_queue()
{
    /* �ֲ��������� */
    UINT32 ret = AP_FAIL;

    /* ��ո澯��Ϣ���� */
    memset(&g_alarmQueue, 0, sizeof(g_alarmQueue));

    /* ��ʼ���澯��Ϣ���� */
    ret = ap_init_alarm_queue(&g_alarmQueue,
                    ALARM_QUEUE_NUM,
                    sizeof(AP_ALARM_INFO_T),
                    QUEUE_TYPE_ALARM,
                    MAX_ALARM_SEND_RATE,
                    ap_is_same_alarm,
                    ap_calc_hash_key_alarm);
    if (ret != AP_OK)
    {
        agent_printf(LOG_ERROR, "init g_alarmQueue failed!");
    }

    return ret;
}

