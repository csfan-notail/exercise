#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lte_list.h"
#include "lte_log.h"


struct list_head *g_stComListHead;
struct list_head *g_stComListenListHead;

/* init list */
int  list_init( void )
{
    /* 两个链表，一个是client对应的链表；一个是监听client对应的链表 */
	struct list_head *pstListHead = NULL;
    struct list_head *pstListenListH = NULL;
    
	pstListHead = (struct list_head *)malloc(sizeof(struct list_head));

	if(NULL == pstListHead)
	{
		LTE_LOG(LTE_LOG_ERR, "malloc failled");
		return -1;
	}
    pstListenListH = (struct list_head *)malloc(sizeof(struct list_head));

	if(NULL == pstListenListH)
	{
		LTE_LOG(LTE_LOG_ERR, "malloc failled");
		return -1;
	}
    
    INIT_LIST_HEAD(pstListHead);
    INIT_LIST_HEAD(pstListenListH);
    
	g_stComListHead = pstListHead;
	g_stComListenListHead = pstListenListH;
	return 0;
}

/* create node */
COM_NODE *list_node_create( void )
{
	COM_NODE	*pstListNode = NULL;

	pstListNode = (COM_NODE *)malloc(sizeof(COM_NODE));

	if(NULL == pstListNode)
	{
		LTE_LOG(LTE_LOG_ERR, "malloc failled");
		return NULL;
	}

	memset(pstListNode, 0, sizeof(COM_NODE));

	return pstListNode;
}

/* add node to list */
/* 队尾插入 */
int  add_node_to_list( struct list_head *pstListHnode, COM_NODE *pstListNodeNew )
{
	if((NULL == pstListNodeNew) || (NULL == pstListHnode))
	{
		LTE_LOG(LTE_LOG_ERR, "The point of input is null");
		return -1;
	}
    /* 根据优先级判断是队首插入还是队尾插入 */
    /* 队尾插入 */
    list_add_tail(&(pstListNodeNew->stComHead), pstListHnode);
    
	return 0;
}

/* delete node from list*/
int  delete_node_from_list( COM_NODE  *pstListDelNode )
{
	COM_NODE   *pstPosNode = NULL;
	COM_NODE   *pstTmpNode = NULL;
	COM_NODE   *pstDelNode = NULL;

	if(NULL == pstListDelNode)
	{
		LTE_LOG(LTE_LOG_ERR, "input param is NULL pointer!");
		return 0;
	}

    list_del(&(pstListDelNode->stComHead));
    
	return 0;
}

/* search someone node from the list */

COM_NODE *search_node_from_list( int iFuncCmd )
{
	struct list_head  *pstListHnode = NULL;
    struct list_head  *pos = NULL;
    struct list_head  *n = NULL;    
    COM_NODE   *pstEntry = NULL;
    
	COM_NODE   *pstSearchNode = NULL;

	pstListHnode = g_stComListHead;

	if(NULL == pstListHnode)
	{
		LTE_LOG(LTE_LOG_ERR, "The list is null");
		return NULL;
	}
	
	list_for_each_safe(pos, n, pstListHnode)
	{
	    pstEntry = list_entry(pos, COM_NODE, stComHead);

        if(iFuncCmd == pstEntry->stComMsg.iFuncCmd)
		{
			pstSearchNode = pstEntry;
            break;
		}
	}
	
	return pstSearchNode;
	
}

