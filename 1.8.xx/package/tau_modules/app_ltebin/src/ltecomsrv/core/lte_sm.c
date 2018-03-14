#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "event2/event.h"
#include "lte_com.h"
#include "lte_list.h"
#include "lte_log.h"

extern LTE_EVENT_T g_stLteEventT;

LTE_RET_E lte_sm_init(int *piSM)
{
    return LTE_OK;
}
LTE_RET_E lte_node_state_change(COM_NODE  *pstNode, int iNextState)
{
    int iCurState = 0;

    if(NULL == pstNode)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s,input param is null pointer!\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    iCurState = pstNode->stComMsg.iNodeSM;
    if((iCurState == WAIT_READ) && (iNextState == WRITE_DONE))
    {
        delete_node_from_list(pstNode);
        free(pstNode);
        return LTE_OK;
    }
    pstNode->stComMsg.iNodeSM = iNextState;

    return LTE_OK;
}
LTE_RET_E lte_com_state_change(COM_NODE  *pstNode, int iCurState, int iNextState)
{
    int iRet = 0;

    if(NULL == pstNode)
    {
        LTE_LOG(LTE_LOG_INFO,"%d,%s,input param is null pointer!\n",__LINE__,__func__);
        return LTE_FAIL;
    }

    if((WRITE_ABLE == iCurState) && (WRITE_DISABLE == iNextState))
    {
        if(WAIT_WRITE == pstNode->stComMsg.iNodeSM)
        {
            iRet = lte_com_write(g_stLteEventT.iComFd, pstNode);
            if(LTE_OK != iRet)
            {
                LTE_LOG(LTE_LOG_ERR,"%d,%s,write com failed!\n",__LINE__,__func__);
                return LTE_FAIL;
            }
            else
            {
                /* 修改node、com状态机 */
                g_stLteEventT.enComSM = iNextState;
                return LTE_OK;
            }
        }
        else
        {
            ;
        }
    }
    else if((WRITE_DISABLE == iCurState) && (WRITE_ABLE == iNextState))
    {
        /* 修改node、com状态机 */
        g_stLteEventT.enComSM = iNextState;
        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }
    
    return LTE_FAIL;
}


LTE_RET_E lte_com_sm_tranf_func(int iSM, struct list_head  *pstListHnode)
{
    COM_NODE *pstEntry = NULL;
    COM_NODE *n = NULL;
    char acCmd[128] = {0};
    int  iWriteLen = 0;
    
    switch(iSM)
    {
        case WRITE_ABLE:
            list_for_each_entry_safe(pstEntry, n, pstListHnode, stComHead)
            {
                if(WAIT_WRITE == pstEntry->stComMsg.iNodeSM)
                {
                    snprintf(acCmd, sizeof(acCmd),"%s\r", pstEntry->stComMsg.acAtCmd);
                    iWriteLen = write(g_stLteEventT.iComFd, acCmd, strlen(acCmd));
                    if (iWriteLen != strlen(acCmd))
                    {
                        LTE_LOG(LTE_LOG_ERR,"%d,%s:write com failed\n",__LINE__,__func__);
                        return LTE_FAIL;
                    }
                    /* 修改node、com状态机 */
                    pstEntry->stComMsg.iNodeSM = WAIT_READ;
                    g_stLteEventT.enComSM = WRITE_DISABLE;
                    break;
                }
                else
                {
                    ;
                }
            }
            break;
        case WRITE_DISABLE:
            LTE_LOG(LTE_LOG_ERR, "%d,%s:com disable!\n",__LINE__,__func__);
            break;    
        default :
            break;
    }
    return LTE_OK;
}

LTE_RET_E lte_node_sm_tranf_func(int iCurSm, COM_NODE  *pstNode)
{   
    int iNodeCurSm;
    iNodeCurSm = iCurSm;
start:
    /* WAIT_READ状态的节点只有一个 */
    switch(iNodeCurSm)
    {
        case WAIT_WRITE:
            break;
        case NOT_NEED_WRITE:
            break;
        case WAIT_READ:
            if(NULL == pstNode)
            {
                return LTE_FAIL;
            }
            if(WRITE_ABLE == g_stLteEventT.enComSM)
            {
                pstNode->stComMsg.iNodeSM = WRITE_DONE;
            }
            else
            {
                ;
            }
            if(WRITE_DONE != pstNode->stComMsg.iNodeSM)
            {
                break;
            }
            else
            {
                iNodeCurSm = WRITE_DONE;
                goto start;
            }
        case WRITE_DONE:
            if(NULL == pstNode)
            {
                return LTE_OK;
            }
            delete_node_from_list(pstNode);
            free(pstNode);
            break;
        default :
            break;
    }
    return LTE_OK;
}




