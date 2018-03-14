#ifndef __LTE_LTE_LIST_H__
#define __LTE_LTE_LIST_H__

#include "list.h"


/* struct declare */
typedef struct __com_msg_t
{
    char acAtCmd[256]; /* 写一行则为AT指令，多行则为文件路径 */
    char acCiev[32];
    int  iFuncCmd;
    int  iPriority;
    int  iNodeSM;
    int  iRealFlag;
    int  iWriteType;
    int  iReadType;
    int  iReturnType;

    int  iClientFdNum;
    int  aiClientFd[32];
}COM_MSG_T;

typedef struct __com_node
{
    COM_MSG_T stComMsg;
    struct list_head stComHead;
}COM_NODE;

#if 0
#define list_for_each_safe(pos, n, head) \
	for((pos) = ((head)->pstNext); (((pos) != NULL)&&(((n) = ((pos)->pstNext)), 1));(pos) = (n))


#define INIT_HLIST_HEAD(ptr) ((ptr)->pstNodeHead = NULL)
#define INIT_HLIST_REAR(ptr) ((ptr)->pstNodeRear = NULL)

typedef struct __com_node
{
    char	acCiev[32];
    int     aiClientFd[32];
	int 	iCliFdNum;

	int 	iCode;
	int 	iReason;
	int 	iSec;
	int 	iUsec;

	struct __Com_node *pstNext;
	
	
}COM_NODE;
#endif

typedef struct __com_hnode
{
	struct __com_node *pstNodeHead;
    struct __com_node *pstNodeRear;
	
}COM_HNODE;

extern int  list_init( void );
extern COM_NODE *list_node_create( void );
extern int  add_node_to_list(struct list_head *pstListHnode, COM_NODE *pstListNodeNew);
extern int  delete_node_from_list( COM_NODE  *pstListDelNode );
extern COM_NODE *search_node_from_list( int iFuncCMd );


#endif  /* __LTE_LTE_LIST_H__ */

