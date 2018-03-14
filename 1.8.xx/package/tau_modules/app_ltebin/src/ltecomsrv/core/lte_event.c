#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>

#include "event2/event.h"
#include "lte_com.h"
#include "lte_list.h"
#include "lte_log.h"

extern struct list_head *g_stComListHead;
extern struct list_head *g_stComListenListHead;
extern LTE_EVENT_T g_stLteEventT;
extern LTE_STATE_INFO_T g_stLteStateInfo;


COM_NODE *lte_find_next_node(struct list_head  *pstListHnode, int iNodeState)
{
    int iLoop = 0;
    int iFindFlag = 0; /* 0：节点在等待写入com口，不需要重新创建；1：需要 */
    COM_NODE *pstComListNode = NULL;
    COM_NODE *pstEntry = NULL;
    COM_NODE *n = NULL;

    list_for_each_entry_safe(pstEntry, n, pstListHnode, stComHead)
    {
        if(iNodeState == pstEntry->stComMsg.iNodeSM)
        {
            iFindFlag = 1;
            break;
        }
    }
    
    if(1 != iFindFlag)
    {
        pstEntry = NULL;
    }
    return pstEntry;
}
COM_NODE *lte_find_notneed_node(struct list_head  *pstListHnode, char *pcBuf)
{
    int iLoop = 0;
    int iFindFlag = 0; /* 0：节点在等待写入com口，不需要重新创建；1：需要 */
    COM_NODE *pstComListNode = NULL;
    COM_NODE *pstEntry = NULL;
    COM_NODE *n = NULL;

    list_for_each_entry_safe(pstEntry, n, pstListHnode, stComHead)
    {
        if(NULL != strstr(pcBuf, pstEntry->stComMsg.acCiev))
        {
            iFindFlag = 1;
            break;
        }
    }
    if(1 != iFindFlag)
    {
        pstEntry = NULL;
        
    }
    else
    {
        LTE_LOG(LTE_LOG_INFO,"at cmd:%s,ciev:%s\n", pstEntry->stComMsg.acAtCmd, pstEntry->stComMsg.acCiev);
    }
    return pstEntry;
}

COM_NODE *lte_list_node_search(struct list_head  *pstListHnode, CLIENT_RECV_MSG_T *pstClientRecvT)
{
    int       iFlag = 0;
    COM_NODE *pstComListNode = NULL;
    COM_NODE *pstEntry = NULL;
    COM_NODE *n = NULL;

    if((NULL == pstListHnode)||(NULL == pstClientRecvT))
    {
        LTE_LOG(LTE_LOG_ERR, "input param is NULL pointer!");
        return NULL;
    }
    list_for_each_entry_safe(pstEntry, n, pstListHnode, stComHead)
    {
        if((pstClientRecvT->iNodeSM == pstEntry->stComMsg.iNodeSM)&&(pstEntry->stComMsg.iFuncCmd == pstClientRecvT->iFuncCmd)
            && (0 == strcmp(pstEntry->stComMsg.acCiev, pstClientRecvT->acCiev)))//add by wangqi
        {
            iFlag = 1;
            pstComListNode = pstEntry;
            if(0 != strcmp(pstComListNode->stComMsg.acAtCmd, pstClientRecvT->acAtCmd))
            {
                memset(pstComListNode->stComMsg.acAtCmd, 0, sizeof(pstComListNode->stComMsg.acAtCmd));
                strncpy(pstComListNode->stComMsg.acAtCmd, pstClientRecvT->acAtCmd, sizeof(pstComListNode->stComMsg.acAtCmd));
            }
            else
            {
                ;
            }
            if(0 != strcmp(pstComListNode->stComMsg.acCiev, pstClientRecvT->acCiev))
            {
                memset(pstComListNode->stComMsg.acCiev, 0, sizeof(pstComListNode->stComMsg.acCiev));
                strncpy(pstComListNode->stComMsg.acCiev, pstClientRecvT->acCiev, sizeof(pstComListNode->stComMsg.acCiev));
            }
            pstEntry->stComMsg.iReadType = pstClientRecvT->iComReadType;
            pstEntry->stComMsg.iReturnType = pstClientRecvT->iComReturnType;
            pstEntry->stComMsg.iWriteType = pstClientRecvT->iComWriteType;
            break;
        }
    }
    if(1 != iFlag)
    {
        pstComListNode = NULL;
    }
    return pstComListNode;
}

COM_NODE * lte_find_list_fd(struct list_head  *pstListHnode, int iFd)
{
    int       iLoop = 0;
    int       iFlag = 0;
    COM_NODE *pstComListNode = NULL;
    COM_NODE *pstEntry = NULL;
    COM_NODE *n = NULL;
    
    if(NULL == pstListHnode)
    {
        LTE_LOG(LTE_LOG_ERR, "input param is NULL pointer!");
        return NULL;
    }
    
    list_for_each_entry_safe(pstEntry, n, pstListHnode, stComHead)
    {
        for(iLoop = 0; iLoop < pstEntry->stComMsg.iClientFdNum; ++iLoop)
        {
            if(iFd == pstEntry->stComMsg.aiClientFd[iLoop])
            {
                pstComListNode = pstEntry;
                iFlag = 1;
            }
        }
        iLoop = 0;
    }
    if(1 != iFlag)
    {
        pstComListNode = NULL;
    }
    return pstComListNode;
}


CLIENT_RECV_MSG_T *lte_msg_read(int *iClient_Fd, void *pvData)
{    
    int   *piReadEv = NULL;
    int    iRecvLen = 0;
    int    iClientFd = 0;
    struct event *pstReadEv = NULL;
    
    CLIENT_RECV_MSG_T *pstRecvMsg = (CLIENT_RECV_MSG_T *)malloc(sizeof(CLIENT_RECV_MSG_T));

    piReadEv = (int *)pvData;
    pstReadEv = (struct event *)(*piReadEv);

    if(NULL == pstReadEv)
    {
        return NULL;
    }
    iClientFd = *iClient_Fd;
    LTE_LOG(LTE_LOG_INFO,"lte_msg_read, iClientFd: %d\n", iClientFd);
    iRecvLen = read(iClientFd, (char *)pstRecvMsg, sizeof(CLIENT_RECV_MSG_T));
    if(iRecvLen < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"read msg failed!\n");
        close(iClientFd);
        event_del(pstReadEv);
        event_free(pstReadEv);
        free(piReadEv);
        free(pstRecvMsg);
        *iClient_Fd = -1;
        return NULL;
    }
    else if(0 == iRecvLen)
    {
        LTE_LOG(LTE_LOG_INFO,"######client close tcp connect!\n");
        close(iClientFd);
        event_del(pstReadEv);
        event_free(pstReadEv);
        free(piReadEv);
        free(pstRecvMsg);
        *iClient_Fd = -1;
        return NULL;
    }
    return pstRecvMsg;
}
LTE_RET_E lte_msg_send(int iResultFlag, int iClientFd, LTE_SEND_MSG_T *pstLteSendMsg, int iSendLen)
{
    int iLen = 0;
    if(NULL == pstLteSendMsg)
    {
        LTE_LOG(LTE_LOG_ERR, "input param is NULL pointer!");
        return LTE_FAIL;
    }
    if(LTE_OK == iResultFlag)
    {
        pstLteSendMsg->iResult = 1;
    }
    else
    {
        pstLteSendMsg->iResult = 0;
    }
    LTE_LOG(LTE_LOG_INFO,"####lte_msg_send:iClientFd[%d],iSendLen[%d]\n", iClientFd, iSendLen);
    iLen = send(iClientFd, (char *)(pstLteSendMsg), iSendLen, 0);
    if(iLen != iSendLen)
    {
        LTE_LOG(LTE_LOG_ERR, "send error, iLen:%d!\n", iLen);
        return LTE_FAIL;
    }
    return LTE_OK;
}
int lte_com_read(int iFd, char *pcBufFile)
{
    
    int iLen = 0;
    int iTotalLen = 0;
    int iBufFd = -1;
    int iWriteLen = 0;
    int iIsFile = 0;
    char acBuf[LTE_COM_MSG_LEN] = {0};

    if(NULL == pcBufFile)
    {
        return LTE_FAIL;
    }
    iBufFd = open(LTE_BUF_FILE_PATH, O_CREAT|O_APPEND|O_WRONLY);
    if(iBufFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR, "open "LTE_BUF_FILE_PATH"failed!");
        return LTE_FAIL;
    }
    /* 读取com数据，并将该数据写入保存文件中 */
    while(1)
    {
        iLen = read(iFd, acBuf, LTE_COM_MSG_LEN-1);
        if(iLen <= 0)
        {
            write(iBufFd, "\n", 2);
            /* com口不再吐数据，退出 */
            break;
        }
        iTotalLen += iLen;
        
        iWriteLen = write(iBufFd, acBuf, strlen(acBuf));
        if(iWriteLen != strlen(acBuf))
        {
            close(iBufFd);
            return -1;
        }
        
        memset(acBuf, 0, sizeof(acBuf));
        iLen = 0;
    }

    LTE_LOG(LTE_LOG_INFO, "lte_com_read:total len[%d]\n", iTotalLen);

    close(iBufFd);
    
    return LTE_OK;
}
LTE_RET_E lte_com_write(int iComFd, COM_NODE  *pstNode)
{
    int iWriteLen = 0;
    char acCmd[256] = {0};
    char acFilePath[64] = {0};
    char acBuf[512] = {0};
    FILE* pstFd = NULL;
    int iBufLen;

    if(NULL == pstNode)
    {
        LTE_LOG(LTE_LOG_ERR, "input param is null poiter!\n")
        return LTE_FAIL;
    }
    /* 判断只写一行还是多行 */
    if(COM_WRITE_TYPE_1 == pstNode->stComMsg.iWriteType)
    {
        snprintf(acCmd, sizeof(acCmd),"%s\r", pstNode->stComMsg.acAtCmd);
        iWriteLen = write(iComFd, acCmd, strlen(acCmd));
        if (iWriteLen != strlen(acCmd))
        {
            LTE_LOG(LTE_LOG_ERR,"%d,%s:write com failed\n",__LINE__,__func__);
            return LTE_FAIL;
        }
    }
    else if(COM_WRITE_TYPE_2 == pstNode->stComMsg.iWriteType)
    {
        /* 读文件 */
        strncpy(acFilePath, pstNode->stComMsg.acAtCmd, sizeof(acFilePath));
        LTE_LOG(LTE_LOG_INFO,"%d,%s:open file: %s\n",__LINE__,__func__, acFilePath);
        pstFd = fopen(acFilePath, "r");
        if(NULL == pstFd)
        {
            LTE_LOG(LTE_LOG_ERR,"%d,%s:open %s failed\n",__LINE__,__func__, acFilePath);
            return LTE_FAIL;
        }
        while(NULL != fgets(acBuf, sizeof(acBuf), pstFd))
        {
            if(NULL == strstr(acBuf, "+"))
            {
                iBufLen = strlen(acBuf);
                acBuf[iBufLen] = 0x1a;
            }
            else
            {
                //snprintf(acBuf, sizeof(acBuf),"%s\r", acBuf);
                iBufLen = strlen(acBuf);
                acBuf[iBufLen - 1] = '\r';
            }
            
            LTE_LOG(LTE_LOG_INFO,"%d,%s:acBuf[%s], iBufLen[%d]\n",__LINE__,__func__, acBuf, iBufLen);
            iWriteLen = write(iComFd, acBuf, strlen(acBuf));
            if (iWriteLen != strlen(acBuf))
            {
                LTE_LOG(LTE_LOG_ERR, "%d,%s:write com failed\n",__LINE__,__func__);
                fclose(pstFd);
                pstFd = NULL;
                return LTE_FAIL;
            }
            memset(acBuf, 0, sizeof(acBuf));
            sleep(1);
        }
        fclose(pstFd);
        pstFd = NULL;
    }

    return LTE_OK;
}
LTE_RET_E lte_com_read_type1_ops(COM_NODE *pstWaitReadNode, char *pcBufLine1, char *pcBufLine2)
{
    int iRet = 0;
    int iLoop = 0;
    LTE_SEND_MSG_T stLteSendMsg;

    if(NULL == pstWaitReadNode)
    {
        return LTE_FAIL;
    }
    if(NULL != strstr(pcBufLine1, pstWaitReadNode->stComMsg.acCiev))
    {
        iRet = lte_info_real_get(pstWaitReadNode->stComMsg.iFuncCmd, &stLteSendMsg, pcBufLine1);
        
        LTE_LOG(LTE_LOG_INFO,"%d,%s:Fd count:[%d]\n",__LINE__,__func__, pstWaitReadNode->stComMsg.iClientFdNum);
        for(iLoop = 0; iLoop < pstWaitReadNode->stComMsg.iClientFdNum; iLoop++)
        {
            //LTE_LOG(LTE_LOG_INFO,"lte_read_com_msg: iRet:%d FD:%d\n", iRet, pstWaitReadNode->stComMsg.aiClientFd[iLoop]);
            iRet = lte_msg_send(iRet, pstWaitReadNode->stComMsg.aiClientFd[iLoop], &stLteSendMsg, sizeof(stLteSendMsg));
        }
        
        if(LTE_OK == iRet)
        {
            g_stLteEventT.enComSM = WRITE_ABLE;
            pstWaitReadNode->stComMsg.iClientFdNum = 0;
        }
        return LTE_OK;
    }
    return LTE_FAIL;
    
}
LTE_RET_E lte_com_read_type2_ops(COM_NODE *pstWaitReadNode, char *pcBufLine1, char *pcBufLine2)
{

}
LTE_RET_E lte_com_read_type3_ops(COM_NODE *pstWaitReadNode, char *pcBufLine1, char *pcBufLine2)
{

}
LTE_RET_E lte_com_read_type4_ops(COM_NODE *pstWaitReadNode, char *pcBufLine1, char *pcBufLine2)
{
    int iRet = 0;
    int iLoop = 0;
    LTE_SEND_MSG_T stLteSendMsg;

    if(NULL == pstWaitReadNode)
    {
        return LTE_FAIL;
    }
    
    if(NULL != strstr(pcBufLine1, pstWaitReadNode->stComMsg.acCiev))
    {
        if(NULL != strstr(pcBufLine2, "OK"))
        {
            iRet = lte_info_real_get(pstWaitReadNode->stComMsg.iFuncCmd, &stLteSendMsg, pcBufLine1);
            LTE_LOG(LTE_LOG_INFO,"lte_read_com_msg:Fd count:%d\n", pstWaitReadNode->stComMsg.iClientFdNum);
            for(iLoop = 0; iLoop < pstWaitReadNode->stComMsg.iClientFdNum; iLoop++)
            {
                //LTE_LOG(LTE_LOG_INFO,"lte_read_com_msg:%d\n", pstWaitReadNode->stComMsg.aiClientFd[iLoop]);
                iRet = lte_msg_send(iRet, pstWaitReadNode->stComMsg.aiClientFd[iLoop], &stLteSendMsg, sizeof(stLteSendMsg));
            }
            
            if(LTE_OK == iRet)
            {
                g_stLteEventT.enComSM = WRITE_ABLE;
            }
            pstWaitReadNode->stComMsg.iClientFdNum = 0;
            return LTE_OK;
        }
    }
    return LTE_FAIL;
}
LTE_RET_E lte_com_read_type5_ops(COM_NODE *pstWaitReadNode, char *pcBufLine1, char *pcBufLine2)
{
    int iRet = 0;
    int iLoop = 0;
    LTE_SEND_MSG_T stLteSendMsg;

    if(NULL == pstWaitReadNode)
    {
        return LTE_FAIL;
    }

    if(NULL != strstr(pcBufLine2, "OK"))
    {
        iRet = lte_info_real_get(pstWaitReadNode->stComMsg.iFuncCmd, &stLteSendMsg, pcBufLine1);
        LTE_LOG(LTE_LOG_INFO,"lte_read_com_msg:Fd count:%d\n", pstWaitReadNode->stComMsg.iClientFdNum);
        for(iLoop = 0; iLoop < pstWaitReadNode->stComMsg.iClientFdNum; iLoop++)
        {
            iRet = lte_msg_send(iRet, pstWaitReadNode->stComMsg.aiClientFd[iLoop], &stLteSendMsg, sizeof(stLteSendMsg));
        }
        
        if(LTE_OK == iRet)
        {
            g_stLteEventT.enComSM = WRITE_ABLE;
        }
        pstWaitReadNode->stComMsg.iClientFdNum = 0;
        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }
}
LTE_RET_E lte_com_read_type6_ops(COM_NODE *pstWaitReadNode, char *pcBufLine1, char *pcBufLine2)
{
    int iRet = 0;
    int iLoop = 0;
    int iFd = 0;
    int iWriteLen = 0;
    char acFilePath[]=LTE_SMS_RECV_FILE;
    LTE_SEND_MSG_T stLteSendMsg;

    if(NULL == pstWaitReadNode)
    {
        return LTE_FAIL;
    }
    /* 解析第一行 */
    if((NULL != strstr(pcBufLine1, pstWaitReadNode->stComMsg.acCiev))||((NULL == strstr(pcBufLine1, ":"))&&(NULL == strstr(pcBufLine1, "OK"))))
    {
        iFd = open(acFilePath,O_CREAT|O_APPEND|O_WRONLY);
        if (iFd < 0)
        {
            LTE_LOG(LTE_LOG_ERR,"%d,%s,open file fail\n",__LINE__,__func__);
            return -1;
        }
        iWriteLen = write(iFd, pcBufLine1, strlen(pcBufLine1));
        if(iWriteLen != strlen(pcBufLine1))
        {
            LTE_LOG(LTE_LOG_ERR,"%d,%s:write file failed\n",__LINE__,__func__);
            close(iFd);
            return -1;
        }
        close(iFd);
    }
    /* 解析第二行 */
    if((NULL != strstr(pcBufLine1, "OK")) || (NULL != strstr(pcBufLine2, "OK")))
    {
        iRet = lte_info_real_get(pstWaitReadNode->stComMsg.iFuncCmd, &stLteSendMsg, pcBufLine1);
        LTE_LOG(LTE_LOG_INFO,"lte_read_com_msg:Fd count:%d\n", pstWaitReadNode->stComMsg.iClientFdNum);
        for(iLoop = 0; iLoop < pstWaitReadNode->stComMsg.iClientFdNum; iLoop++)
        {
            //LTE_LOG(LTE_LOG_INFO,"lte_read_com_msg:%d\n", pstWaitReadNode->stComMsg.aiClientFd[iLoop]);
            iRet = lte_msg_send(iRet, pstWaitReadNode->stComMsg.aiClientFd[iLoop], &stLteSendMsg, sizeof(stLteSendMsg));
        }
        
        if(LTE_OK == iRet)
        {
            g_stLteEventT.enComSM = WRITE_ABLE;
        }
        pstWaitReadNode->stComMsg.iClientFdNum = 0;
        return LTE_OK;
    }
    return LTE_FAIL;
}
LTE_RET_E lte_com_read_type7_ops(COM_NODE *pstWaitReadNode, char *pcBufLine1, char *pcBufLine2)
{
    int iRet = 0;
    int iLoop = 0;
    LTE_SEND_MSG_T stLteSendMsg;
    
    if(NULL == pstWaitReadNode)
    {
        return LTE_FAIL;
    }
    if(NULL != strstr(pcBufLine1, pstWaitReadNode->stComMsg.acCiev))
    {
        iRet = lte_info_real_get(pstWaitReadNode->stComMsg.iFuncCmd, &stLteSendMsg, pcBufLine1);
        LTE_LOG(LTE_LOG_INFO,"lte_read_com_msg:Fd cont:%d\n", pstWaitReadNode->stComMsg.iClientFdNum);
        for(iLoop = 0; iLoop < pstWaitReadNode->stComMsg.iClientFdNum; iLoop++)
        {
            //LTE_LOG(LTE_LOG_INFO,"lte_read_com_msg:%d\n", pstWaitReadNode->stComMsg.aiClientFd[iLoop]);
            iRet = lte_msg_send(iRet, pstWaitReadNode->stComMsg.aiClientFd[iLoop], &stLteSendMsg, sizeof(stLteSendMsg));
        }
        
        if(LTE_OK == iRet)
        {
            g_stLteEventT.enComSM = WRITE_ABLE;
        }
        pstWaitReadNode->stComMsg.iClientFdNum = 0;
        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }

}
LTE_RET_E lte_com_read_type8_ops(COM_NODE *pstWaitReadNode, char *pcBufLine1, char *pcBufLine2)
{
    int iRet = 0;
    int iLoop = 0;
    LTE_SEND_MSG_T stLteSendMsg;
    
    if(NULL == pstWaitReadNode)
    {
        return LTE_FAIL;
    }
    
    if(NULL != strstr(pcBufLine1, pstWaitReadNode->stComMsg.acCiev))
    {
        iRet = lte_info_real_get(pstWaitReadNode->stComMsg.iFuncCmd, &stLteSendMsg, pcBufLine1);
        LTE_LOG(LTE_LOG_INFO,"lte_read_com_msg:Fd cont:%d\n", pstWaitReadNode->stComMsg.iClientFdNum);
        for(iLoop = 0; iLoop < pstWaitReadNode->stComMsg.iClientFdNum; iLoop++)
        {
            //LTE_LOG(LTE_LOG_INFO,"lte_read_com_msg:%d\n", pstWaitReadNode->stComMsg.aiClientFd[iLoop]);
            iRet = lte_msg_send(iRet, pstWaitReadNode->stComMsg.aiClientFd[iLoop], &stLteSendMsg, sizeof(stLteSendMsg));
        }
        
        if(LTE_OK == iRet)
        {
            g_stLteEventT.enComSM = WRITE_ABLE;
        }
        pstWaitReadNode->stComMsg.iClientFdNum = 0;
        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }

}

LTE_RET_E lte_com_read_type9_ops(COM_NODE *pstWaitReadNode, char *pcBufLine1, char *pcBufLine2)
{
    int iRet = 0;
    int iLoop = 0;
    int iFd = 0;
    int iWriteLen = 0;
    LTE_SEND_MSG_T stLteSendMsg;

    if(NULL == pstWaitReadNode)
    {
        return LTE_FAIL;
    }
    /* 解析第一行 */
    if( (NULL != strstr(pcBufLine1, ":"))&&(NULL == strstr(pcBufLine1, "OK")))
    {
        iFd = open(LTE_LCTCELLINFO_FILE,O_CREAT|O_APPEND|O_WRONLY);
        if (iFd < 0)
        {
            LTE_LOG(LTE_LOG_ERR,"%d,%s,open file fail\n",__LINE__,__func__);
            return -1;
        }
        iWriteLen = write(iFd, pcBufLine1, strlen(pcBufLine1));
        if(iWriteLen != strlen(pcBufLine1))
        {
            LTE_LOG(LTE_LOG_ERR,"%d,%s:write file failed\n",__LINE__,__func__);
            close(iFd);
            return -1;
        }
        close(iFd);
    }
    /* 解析第二行 */
    if((NULL != strstr(pcBufLine1, "OK")) || (NULL != strstr(pcBufLine2, "OK")))
    {
        iRet = lte_info_real_get(pstWaitReadNode->stComMsg.iFuncCmd, &stLteSendMsg, pcBufLine1);
        LTE_LOG(LTE_LOG_INFO,"lte_read_com_msg:Fd count:%d\n", pstWaitReadNode->stComMsg.iClientFdNum);
        for(iLoop = 0; iLoop < pstWaitReadNode->stComMsg.iClientFdNum; iLoop++)
        {
            //LTE_LOG(LTE_LOG_INFO,"lte_read_com_msg:%d\n", pstWaitReadNode->stComMsg.aiClientFd[iLoop]);
            iRet = lte_msg_send(iRet, pstWaitReadNode->stComMsg.aiClientFd[iLoop], &stLteSendMsg, sizeof(stLteSendMsg));
        }
        
        if(LTE_OK == iRet)
        {
            g_stLteEventT.enComSM = WRITE_ABLE;
        }
        pstWaitReadNode->stComMsg.iClientFdNum = 0;
        return LTE_OK;
    }
    return LTE_FAIL;
}



LTE_RET_E init_libevent(EVENT_BASE_ID *piEBaseID)
{
    int iRet = 0;
    
    if(NULL == piEBaseID)
    {
        LTE_LOG(LTE_LOG_ERR, "input param is NULL pointer!");
        return LTE_FAIL;
    }
    *piEBaseID = 0;

    struct event_base *base = event_base_new();

    *piEBaseID = (EVENT_BASE_ID)base;
    if((iRet = event_base_priority_init(base, EVENT_MAX_PRIORITIES - 1)) < 0)
    {
        event_base_free(base);
        return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO, "init_libevent SUCCESS!!!");
    return LTE_OK;
}
LTE_RET_E lte_event_base_dispatch(struct event_base *pstEventBase)
{
    int iRet = 0;

    if(NULL == pstEventBase)
    {
        LTE_LOG(LTE_LOG_ERR, "input param is NULL pointer!");
        return LTE_FAIL;
    }
    
    iRet = event_base_dispatch(pstEventBase);
    if(iRet < -1)
    {
        LTE_LOG(LTE_LOG_ERR, "event_base_dispatch failed!");
        return LTE_FAIL;
    }
    return LTE_OK;
}


LTE_RET_E lte_list_node_add(CLIENT_RECV_MSG_T *pstClientRecvT, int iClient_Fd)
{
    int iLoop = 0;
    int iCreatFlag = 1; /* 0：节点在等待写入com口，不需要重新创建；1：需要 */
    struct list_head  *pstListHnode;
    COM_NODE *pstComListNode = NULL;
    COM_NODE *pstEntry = NULL;
    COM_NODE *n = NULL;
    
    if(NULL == pstClientRecvT)
    {
        return LTE_FAIL;
    }
    /* 判断收到的节点是哪一种 */
    if(pstClientRecvT->iNodeSM == NOT_NEED_WRITE)
    {
        pstListHnode = g_stComListenListHead;
        /* 查找有无相同的节点 */
        pstComListNode = lte_list_node_search(pstListHnode, pstClientRecvT);
    }
    else if(pstClientRecvT->iNodeSM == WAIT_WRITE)
    {
        pstListHnode = g_stComListHead;
        /* 查找有无相同的节点 */
        pstComListNode = lte_list_node_search(pstListHnode, pstClientRecvT);
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR, "This node do not exit!");
        return LTE_FAIL;
    }
    /* 没找到该节点，则创建一个节点，并将该节点加入到链表中 */
    if(NULL == pstComListNode)
    {
        pstComListNode = list_node_create();

        if(NULL == pstComListNode)
        {
            LTE_LOG(LTE_LOG_ERR,"create list node failed!\n");
            return LTE_FAIL;
        }
        else
        {
            strncpy(pstComListNode->stComMsg.acAtCmd, pstClientRecvT->acAtCmd, sizeof(pstComListNode->stComMsg.acAtCmd));
            strncpy(pstComListNode->stComMsg.acCiev, pstClientRecvT->acCiev, sizeof(pstComListNode->stComMsg.acCiev));
            pstComListNode->stComMsg.iFuncCmd = pstClientRecvT->iFuncCmd;
            pstComListNode->stComMsg.iPriority = pstClientRecvT->iPriority;
            pstComListNode->stComMsg.iNodeSM = pstClientRecvT->iNodeSM;
            pstComListNode->stComMsg.iReadType = pstClientRecvT->iComReadType;
            pstComListNode->stComMsg.iReturnType = pstClientRecvT->iComReturnType;
            pstComListNode->stComMsg.iWriteType = pstClientRecvT->iComWriteType;
            if(0 != add_node_to_list(pstListHnode, pstComListNode))
            {
                LTE_LOG(LTE_LOG_ERR, "add node to list failed!\n");
                free(pstComListNode);
                return LTE_FAIL;
            }
        }
    }
    else
    {
        ;
    }

    if(pstClientRecvT->iServerSendType == SERVER_NOT_SEND)//不需要发送
    {
        return LTE_OK;
    }

    for(iLoop = 0; iLoop < pstComListNode->stComMsg.iClientFdNum; ++iLoop)
    {
        if(iClient_Fd != pstComListNode->stComMsg.aiClientFd[iLoop])
        {
            continue;
        }
        else
        {
            break;
        }
    }
    LTE_LOG(LTE_LOG_INFO,"iLoop:%d, pstComListNode->stComMsg.iClientFdNum:%d clientFd:%d\n", iLoop, pstComListNode->stComMsg.iClientFdNum, iClient_Fd);
    if(iLoop == pstComListNode->stComMsg.iClientFdNum)
    {
        pstComListNode->stComMsg.aiClientFd[pstComListNode->stComMsg.iClientFdNum] = iClient_Fd;
        pstComListNode->stComMsg.iClientFdNum++;
    }

    return LTE_OK;

}
LTE_RET_E lte_analyze_error_buf(COM_NODE *pstWaitReadNode, char *pcBufLine1)
{
    int iRet = 0;
    int iLoop = 0;
    LTE_SEND_MSG_T stLteSendMsg;
    
    if((NULL != strstr(pcBufLine1, "ERROR")) && (NULL != pstWaitReadNode))
    {
        stLteSendMsg.iResult = 0; /* AT指令执行错误 */
        iRet = LTE_FAIL;
        
        LTE_LOG(LTE_LOG_INFO,"at exit error:%s %s %d\n", pstWaitReadNode->stComMsg.acAtCmd, pstWaitReadNode->stComMsg.acCiev, pstWaitReadNode->stComMsg.iClientFdNum);
        for(iLoop = 0; iLoop < pstWaitReadNode->stComMsg.iClientFdNum; iLoop++)
        {
            //LTE_LOG(LTE_LOG_ERR,"######com_msg_handle:lte_send_msg--error:%d %s\n", pstWaitReadNode->stComMsg.aiClientFd[iLoop], pcBufLine1);
            iRet = lte_msg_send(iRet, pstWaitReadNode->stComMsg.aiClientFd[iLoop], &stLteSendMsg, sizeof(stLteSendMsg));
            
        }
        if(LTE_OK == iRet)
        {
            g_stLteEventT.enComSM = WRITE_ABLE;
        }
        pstWaitReadNode->stComMsg.iClientFdNum = 0;
        
        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }
}
LTE_RET_E lte_analyze_buf_notneed(COM_NODE *pstNotNeedNode, COM_NODE *pstWaitReadNode, char *pcBufLine1, char *pcBufLine2)
{
    int iRet = 0;
    int iFlag = 0;
    int iLoop = 0;
    LTE_SEND_MSG_T stLteSendMsg;
    
    if(NULL == pstNotNeedNode)
    {
        return LTE_FAIL;
    }
    /* 进一步判断该pcBufLine1是否为notneed节点所需要的buf */
    if(NULL == pstWaitReadNode)
    {
        iFlag = 1;
    }
    else
    {
        if(NULL != strstr(pcBufLine2, "OK"))
        {
            if(pstWaitReadNode->stComMsg.iReadType == COM_READ_TYPE_1)
            {
                iFlag = 1;
            }
        }
        else 
        {
            iFlag = 1;
        }
    }
    /* 若满足该节点，则进行分析并处理 */
    if(1 == iFlag)
    {
        iRet = lte_info_real_get(pstNotNeedNode->stComMsg.iFuncCmd, &stLteSendMsg, pcBufLine1);
        LTE_LOG(LTE_LOG_INFO,"lte_analyze_buf_notneed:Fd cont:%d", pstNotNeedNode->stComMsg.iClientFdNum);
        for(iLoop = 0; iLoop < pstNotNeedNode->stComMsg.iClientFdNum; iLoop++)
        {
            //LTE_LOG(LTE_LOG_INFO,"lte_analyze_buf_notneed:%d\n", pstWaitReadNode->stComMsg.aiClientFd[iLoop]);
            iRet = lte_msg_send(iRet, pstNotNeedNode->stComMsg.aiClientFd[iLoop], &stLteSendMsg, sizeof(stLteSendMsg));
            if(LTE_OK == iRet)
            {
                g_stLteEventT.enComSM = WRITE_ABLE;
            }
        }
        pstNotNeedNode->stComMsg.iClientFdNum = 0;
    }
    else
    {
        return LTE_FAIL;
    }
    return LTE_OK;
}
LTE_RET_E lte_analyze_buf_waitread(COM_NODE *pstWaitReadNode, char *pcBufLine1, char *pcBufLine2)
{
    int iRet = 0;
    if(NULL == pstWaitReadNode)
    {
        return LTE_FAIL;
    }
    switch(pstWaitReadNode->stComMsg.iReadType)
    {
        case COM_READ_TYPE_1:
            iRet = lte_com_read_type1_ops(pstWaitReadNode, pcBufLine1, pcBufLine2);
            break;
        case COM_READ_TYPE_2:
            
            break;
        case COM_READ_TYPE_3:
            
            break;
        case COM_READ_TYPE_4:
            iRet = lte_com_read_type4_ops(pstWaitReadNode, pcBufLine1, pcBufLine2);
            break;
        case COM_READ_TYPE_5:
            iRet = lte_com_read_type5_ops(pstWaitReadNode, pcBufLine1, pcBufLine2);
            break;
        case COM_READ_TYPE_6:
            iRet = lte_com_read_type6_ops(pstWaitReadNode, pcBufLine1, pcBufLine2);
            break;
        case COM_READ_TYPE_7:
            iRet = lte_com_read_type7_ops(pstWaitReadNode, pcBufLine1, pcBufLine2);
            break;
        case COM_READ_TYPE_8:
            iRet = lte_com_read_type8_ops(pstWaitReadNode, pcBufLine1, pcBufLine2);
            break;
        case COM_READ_TYPE_9:
            iRet = lte_com_read_type9_ops(pstWaitReadNode, pcBufLine1, pcBufLine2);
            break;
        default :
            break;

    }

    return iRet;
}
COM_NODE *lte_send_msg(struct list_head  *pstListHnode, char *pcBuf)
{
    int   iRet = 0;
    int   iEntryLoop = 0;
    int   iWaitFlag = 0;
    int   iSendFlag = 0;
    int   iLoop = 0;
    COM_NODE *pstEntry = NULL;
    COM_NODE *n = NULL;
    COM_NODE *pstWaitReadEntry = NULL;

    LTE_SEND_MSG_T stLteSendMsg;
    
    list_for_each_entry_safe(pstEntry, n, pstListHnode, stComHead)
    {
        if(WAIT_READ == pstEntry->stComMsg.iNodeSM)
        {
            pstWaitReadEntry = pstEntry;
        }
        
        LTE_LOG(LTE_LOG_INFO,"AT CMD:%s CIEV:%s loop:%d\n", pstEntry->stComMsg.acAtCmd, pstEntry->stComMsg.acCiev, iEntryLoop);
        if(NULL != strstr(pcBuf, pstEntry->stComMsg.acCiev))
        {
            /* 只处理状态为wait_read、not_need_write的节点 */
            if(WAIT_READ == pstEntry->stComMsg.iNodeSM)
            {
                iWaitFlag = 1;
                iSendFlag = 1;
            }
            else if(NOT_NEED_WRITE == pstEntry->stComMsg.iNodeSM)
            {
                iSendFlag = 1;
            }
            /* handle node */
            if(1 == iSendFlag)
            {
                iRet = lte_info_real_get(pstEntry->stComMsg.iFuncCmd, &stLteSendMsg, pcBuf);
                LTE_LOG(LTE_LOG_INFO,"lte_send_msg:Fd cont:%d\n", pstEntry->stComMsg.iClientFdNum);
                for(iLoop = 0; iLoop < pstEntry->stComMsg.iClientFdNum; iLoop++)
                {
                    //LTE_LOG(LTE_LOG_INFO,"lte_send_msg:%d\n", pstEntry->stComMsg.aiClientFd[iLoop]);
                    iRet = lte_msg_send(iRet, pstEntry->stComMsg.aiClientFd[iLoop], &stLteSendMsg, sizeof(stLteSendMsg));
                    if((1 == iWaitFlag) && (LTE_OK == iRet))
                    {
                        g_stLteEventT.enComSM = WRITE_ABLE;
                    }
                }
                pstEntry->stComMsg.iClientFdNum = 0;
                iSendFlag = 0;
            }

        }
        iEntryLoop++;
    }
    /* 获取到AT执行错误信息 */
    if((0 == iWaitFlag) && (NULL != strstr(pcBuf, "ERROR")) && (NULL != pstWaitReadEntry))
    {
        stLteSendMsg.iResult = 0; /* AT指令执行错误 */
        iRet = LTE_FAIL;
        
        LTE_LOG(LTE_LOG_INFO,"error:%s %s %d\n", pstWaitReadEntry->stComMsg.acAtCmd, pstWaitReadEntry->stComMsg.acCiev, pstWaitReadEntry->stComMsg.iClientFdNum);
        for(iLoop = 0; iLoop < pstWaitReadEntry->stComMsg.iClientFdNum; iLoop++)
        {
            //LTE_LOG(LTE_LOG_INFO,"######com_msg_handle:lte_send_msg--error:%d %s\n", pstWaitReadEntry->stComMsg.aiClientFd[iLoop], pcBuf);
            iRet = lte_msg_send(iRet, pstWaitReadEntry->stComMsg.aiClientFd[iLoop], &stLteSendMsg, sizeof(stLteSendMsg));
            if(LTE_OK == iRet)
            {
                g_stLteEventT.enComSM = WRITE_ABLE;
            }
        }
        pstWaitReadEntry->stComMsg.iClientFdNum = 0;
    }
    return pstWaitReadEntry;
}
LTE_RET_E lte_commsg_operate(char *pcFilePath)
{
    FILE* pstFd = NULL;
    int iRet = 0;
    int iBufLine1Flag = 1;/* 1，文件第一个read可以使用 */
    int iBufLine2Flag = 1;/* 1，文件第二个read可以使用 */
    int iWaitReadFlag = 1;/* 1，匹配waitread节点 */
    int iFileReadOver = 0;
    char acBufLine1[LTE_COM_MSG_LEN] = {0};
    char acBufLine2[LTE_COM_MSG_LEN] = {0};
    struct list_head  *pstListHnode = NULL;
    struct list_head  *pstNotNeedListHnode = NULL;
    COM_NODE *pstWaitReadNode = NULL;
    COM_NODE *pstNotNeedNode = NULL;
    COM_NODE *pstWaitWriteNode = NULL;
    
    if(NULL == pcFilePath)
    {
        return LTE_FAIL;
    }

    pstListHnode = g_stComListHead;
    pstNotNeedListHnode = g_stComListenListHead;
    
    /* 获取waitread节点，链表中wait_read节点最多只有一个 */
    pstWaitReadNode = lte_find_next_node(pstListHnode, WAIT_READ);

    pstFd = fopen(LTE_BUF_FILE_PATH, "r");
	if( NULL == pstFd )
	{	
	    LTE_LOG(LTE_LOG_ERR,"can't open /tmp/fbuf!!!!\n");
		return LTE_FAIL;
	}
    while(1)
    {
        if(NULL != fgets(acBufLine1, LTE_COM_MSG_LEN, pstFd))
        {
            if(strlen(acBufLine1) < 3)
            {
                continue;
            }
            else
            {
                break;
            }
        }
        else
        {
            fclose(pstFd);
            return LTE_FAIL;
        }
    }
    LTE_LOG(LTE_LOG_INFO,"##################acBufLine1: %s!\n", acBufLine1);
    /* 读文件 */
    while(1)
    {
        if(1 == iFileReadOver)
        {
            break;
        }
        if(NULL == fgets(acBufLine2, LTE_COM_MSG_LEN, pstFd))
        {   
            /* 最后一行空行也用 */
            iFileReadOver = 1;
        }
        else if(strlen(acBufLine2) < 3)
        {            
            /* 从com口中获取的数据，写入文件后，空行有两个字符，
               去除空行，如果读出的是结尾，则不省略 */
            continue;
        }
        LTE_LOG(LTE_LOG_INFO,"##################bufline2:%s!\n", acBufLine2);
        /* 查找NOT_need链表中是否有节点与bufline1匹配的节点 */
        pstNotNeedNode = lte_find_notneed_node(pstNotNeedListHnode, acBufLine1);
        if(NULL != pstNotNeedNode)
        {
            /* 针对查询到notneed节点解析bufline1、bufline2 */
            iRet = lte_analyze_buf_notneed(pstNotNeedNode, pstWaitReadNode, acBufLine1, acBufLine2);
            if(LTE_FAIL != iRet)
            {
                strcpy(acBufLine1, acBufLine2);
                //continue;
            }
            else
            {
                ;
            }
        }
        /* waitread节点是否需要解析bufline1，bufline2 */
        if(1 == iWaitReadFlag)
        {
            /* 针对waitread节点解析acBufLine1，acBufLine2 */
            iRet = lte_analyze_buf_waitread(pstWaitReadNode, acBufLine1, acBufLine2);
            if(LTE_OK == iRet)
            {
                iWaitReadFlag = 0;
            }
            else
            {
                iRet = lte_analyze_error_buf(pstWaitReadNode, acBufLine1);
                if(LTE_OK == iRet)
                {
                    iWaitReadFlag = 0;
                }
            }
            strcpy(acBufLine1, acBufLine2);
        }
    }
    fclose(pstFd);
    LTE_LOG(LTE_LOG_INFO,"read com msg file success!\n");
    /* 状态机修改 */
    if(0 == iWaitReadFlag)
    {   
        lte_node_state_change(pstWaitReadNode, WRITE_DONE);
    }
    pstWaitWriteNode = lte_find_next_node(pstListHnode, WAIT_WRITE);
    iRet = lte_com_state_change(pstWaitWriteNode, g_stLteEventT.enComSM, WRITE_DISABLE);
    if(LTE_OK == iRet)
    {
        iRet = lte_node_state_change(pstWaitWriteNode, WAIT_READ);
    }
    return LTE_OK;
}
void lte_read_com_msg( int iFd, int iEventId, void *pvData )
{
	int   iRet = 0;
    int   iLen = 0;
    int   iLoop = 0;
    int   iEntryLoop = 0;
    int   iWaitFlag = 0;
    int   iSendFlag = 0;
    int   iWriteLen = 0;
    int   iComFileLen = 0;
    int   iIsFile = 0;
    int   iFlag = 0;
    //char  acBuf[256] = {0};/* 如何处理，获取到的字段长度大于256 */
    char  acSendBuf[512] = {0};
    char  acCmd[64] = {0};
    char  acBuf[LTE_COM_MSG_LEN] = {0};
    char  acBufTmp[LTE_COM_MSG_LEN] = {0};
    char  *pcBuf = NULL;
    FILE  *pstFd = NULL;
    COM_NODE *pstWaitReadEntry = NULL;
    COM_NODE *pstEntry = NULL;
    COM_NODE *n = NULL;
    struct list_head  *pstListHnode = NULL;

    LTE_SEND_MSG_T stLteSendMsg;
    LTE_BUF_FILE_T stLteBufFile = {0};
        
    pstListHnode = g_stComListHead;

    char  acBufSaveFile[] = LTE_BUF_FILE_PATH;  /* 从com口中读取的数据，存放
                                                   在该文件中 */
    
    /*lte_com_read, 将从com口中读取的结果写入文件中 */
    iRet = lte_com_read(iFd, acBufSaveFile);

    if(iRet < 0)
    {
        return ;
    }

    /* 对保存com口数据的文件进行解析，并做相应操作 */
    iRet = lte_commsg_operate(acBufSaveFile);
    /* 删除存储com数据的文件 */
    unlink(acBufSaveFile);
    
    #if 0
    printf("#########lte_com_read_msg:%s\n", acBuf);
    /* CLIENT */
    /* 获取wait_read节点 */
    pstWaitReadEntry = lte_find_next_node(pstListHnode, WAIT_READ);
    /* 从文件中一行一行读取结果 */
    pstFd = fopen(LTE_BUF_FILE_PATH, "r")
    
    while(NULL != fgets(acBuf, 256, pstFd))
    {
        /* 是否适合wait_read节点 */
        if()
        {
            switch(pstWaitReadEntry->stComMsg.iReadType)
            {
                case COM_READ_TYPE_4:
                        
                    break;
                case cc:
                    break;
            }
            
        }
        else
        {
            
        }
    }

    /* LISTEN */
    

    /*analyse buffer*/
    for()
    {
        if()
        {
            /*client*/
            /*find wait read node*/
            /*send*/

            /*change com state*/
            
            /*change node state*/

            
        }
        else
        {
            /*listen*/
            /*find wait read node*/
            /*send*/
        }
    }

    /*find next*/

    /*do next*/

    /*lte_msg_send, 返回状态为wait_read的节点*/
    pstWaitReadEntry = lte_send_msg(pstListHnode, pcBuf);
    /*node_state_tranf*/
    if(NULL != pstWaitReadEntry)
    {
        lte_node_sm_tranf_func(pstWaitReadEntry->stComMsg.iNodeSM, pstWaitReadEntry);
    }
    /*com_stat_func*/
    
    /* 判断com口状态，是否可写 */
    iRet = lte_com_sm_tranf_func(g_stLteEventT.enComSM, pstListHnode);
    #endif
    return ;
}
LTE_RET_E lte_delete_fd(COM_NODE *pstNode, int iFd)
{
    int iLoop = 0;
    int iRet = 0;
    COM_NODE *pstWaitWrite = NULL;
    
    if(NULL == pstNode)
    {
        return LTE_FAIL;
    }
    if(pstNode->stComMsg.iClientFdNum > 1)
    {
        for(iLoop = 0; iLoop < pstNode->stComMsg.iClientFdNum; ++iLoop)
        {   
            if(iFd == pstNode->stComMsg.aiClientFd[iLoop])
            {
                pstNode->stComMsg.aiClientFd[iLoop] = pstNode->stComMsg.aiClientFd[pstNode->stComMsg.iClientFdNum - 1];
                pstNode->stComMsg.aiClientFd[pstNode->stComMsg.iClientFdNum - 1] = 0;
                pstNode->stComMsg.iClientFdNum = pstNode->stComMsg.iClientFdNum - 1;
            }
        }
    }
    else
    {
        if(WAIT_READ == pstNode->stComMsg.iNodeSM)
        {
            lte_node_state_change(pstNode, WRITE_DONE);
            g_stLteEventT.enComSM = WRITE_ABLE;
            pstWaitWrite = lte_find_next_node(g_stComListHead, WAIT_WRITE);

            iRet = lte_com_state_change(pstWaitWrite, g_stLteEventT.enComSM, WRITE_DISABLE);
            if(LTE_OK == iRet)
            {
                iRet = lte_node_state_change(pstWaitWrite, WAIT_READ);
            }
        }
        else
        {
            delete_node_from_list(pstNode);
            free(pstNode);
        }
    }
    
    return LTE_OK;
}
void lte_recv_client_msg( int iFd, int iEventId, void *pvData )
{
    int   iRet = 0;
    int   iClient_Fd = 0;
    int   iClient_Fd_Tmp = 0;
    int   iRecvLen = 0;
    int   iWriteLen = 0;
    int   iLoop = 0;
    char  acBuf[128] = {0};
    char  acCmd[128] = {0};
    CLIENT_RECV_MSG_T *pstComMst = NULL;
    COM_NODE *pstComListNode = NULL;
    LTE_SEND_MSG_T stLteSendMsg;
    COM_NODE *pstEntry = NULL;
    COM_NODE *n = NULL;
    COM_NODE *pstWriteNode = NULL;
    struct list_head  *pstListHnode = NULL;
    struct list_head  *pstNotNeedListHnode = NULL;

    iClient_Fd = iFd;
    iClient_Fd_Tmp = iClient_Fd;

    pstListHnode = g_stComListHead;
    pstNotNeedListHnode = g_stComListenListHead;

    pstComMst = lte_msg_read(&iClient_Fd_Tmp, pvData);

    if(NULL == pstComMst)
    {
        if(-1 == iClient_Fd_Tmp)
        {
            /* 从write链中删除FD */
            pstComListNode = lte_find_list_fd(pstListHnode, iClient_Fd);
            lte_delete_fd(pstComListNode, iClient_Fd);
            pstComListNode = NULL;
            /* 从not_need链中删除该FD */
            pstComListNode = lte_find_list_fd(pstNotNeedListHnode, iClient_Fd);
            lte_delete_fd(pstComListNode, iClient_Fd);
            pstComListNode = NULL;
        }
        return ;
    }
    LTE_LOG(LTE_LOG_INFO,"lte_recv_client_msg:AtCmd:%s Ciev:%s iFuncCmd:%d iPriority:%d\n",
        pstComMst->acAtCmd, pstComMst->acCiev, pstComMst->iFuncCmd, pstComMst->iPriority);

    /* 判断是否静态获取 */
    if(LTE_READ_LTE_INFO_STATIC == pstComMst->iRealFlag)
    {
        iRet = lte_info_static_get(pstComMst->iFuncCmd, &stLteSendMsg);
        if(LTE_OK == iRet)
        {
            lte_msg_send(iRet, iClient_Fd, &stLteSendMsg, sizeof(stLteSendMsg));
            free(pstComMst);
            return ;
        }
    }
    /* 向链表中加节点 */
    iRet = lte_list_node_add(pstComMst, iClient_Fd);
    if(LTE_OK != iRet)
    {
        iRet = LTE_FAIL;
        stLteSendMsg.iResult = 0;
        lte_msg_send(iRet, iClient_Fd, &stLteSendMsg, sizeof(stLteSendMsg));
        free(pstComMst);
        return ;
    }
    /* 查找AT指令等待写的节点 */
    pstWriteNode = lte_find_next_node(pstListHnode, WAIT_WRITE);
    if(NULL == pstWriteNode)
    {
        return ;
    }
    /* 此处com口状态若从write-able到disable转变成功，则需修改节点状态从wait-
       _write到wait_read */
    /* 更改com口状态 */
    iRet = lte_com_state_change(pstWriteNode, g_stLteEventT.enComSM, WRITE_DISABLE);
    if(LTE_OK == iRet)
    {
        iRet = lte_node_state_change(pstWriteNode, WAIT_READ);
    }
    //iRet = lte_com_sm_tranf_func(g_stLteEventT.enComSM, pstListHnode);
    free(pstComMst);
    return ;
}
int libevent_creat_event(EVENT_BASE_ID iEventB, 
                                int iFd, 
                                signed short sEvents, 
                                void (*func)(int, signed short, void *), 
                                void * data )
{
    struct event *pstEvent = NULL;
    if (NULL == (struct event_base *)iEventB || NULL == func)
    {
        LTE_LOG(LTE_LOG_ERR, "arg is NULL pointer!");
        return (EVENT_ID)NULL;
    }

    pstEvent = event_new((struct event_base *)iEventB, iFd, sEvents, func, data);
    if(NULL == pstEvent)
    {
        LTE_LOG(LTE_LOG_ERR, "create failed! event_new failed!\n");
        return (EVENT_ID)NULL;
    }
    return (EVENT_ID)pstEvent;
    
}
int  libevent_add_event( EVENT_ID iLibEvent, int iPriority)
{
    struct event *pstEvent = NULL;
    int iRet = 0;

    pstEvent = (struct event *)iLibEvent;

   
    /* 设置事件优先级 */
    if((iRet = event_priority_set(pstEvent, iPriority)) < 0 )
    {
        LTE_LOG(LTE_LOG_ERR, "event_priority_set exit failed");
        event_free(pstEvent);
        return LTE_FAIL;
    }
    if(NULL != pstEvent)
    {
        if (0 != event_add(pstEvent, NULL))
        {
            LTE_LOG(LTE_LOG_ERR, "event_add call fail!");
            
            event_free(pstEvent);
            return LTE_FAIL;
        }
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"libevent_add_event: pstEvent NULL\n");
    }
    return LTE_OK;
}

void lte_client_on_accept( int iFd, int iEventId, void *pvData )
{
    int iClientFd = 0;
    int iSockLen = 0;
    int iRet = 0;
    int *piReadEv = NULL;
    struct sockaddr_un unclient;
    struct event* read_ev;
    EVENT_ID eiReadFD = 0;
    LTE_EVENT_T *pstLteVent = (LTE_EVENT_T *)pvData;
    
    iSockLen = sizeof(unclient);
    iClientFd = accept(iFd, (struct sockaddr*)(&unclient), &iSockLen);
    piReadEv = (int *)malloc(sizeof(int));

    eiReadFD = libevent_creat_event(pstLteVent->eEventBaseID, 
                                    iClientFd, 
                                    EV_READ | EV_PERSIST, 
                                    (void *)lte_recv_client_msg, 
                                    piReadEv);
    *piReadEv = eiReadFD;
    LTE_LOG(LTE_LOG_INFO,"##lte_client_on_accept:iClientFd:%d, eiReadFD:%d\n", iClientFd, eiReadFD);
    iRet = libevent_add_event(eiReadFD, LTE_LIBEVENT_PRIORITY_FD_ACCEPT_LEVEL);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"lte_client_on_accept: libevent_add_event failed!\n");
        free(piReadEv);
        return ;
    }
    LTE_LOG(LTE_LOG_INFO,"##libevent_add_event success!\n");
    return ;
}


LTE_RET_E libevent_com_event_register(LTE_EVENT_T *pstLteEventT)
{
    int iRet = 0;
    if(NULL == pstLteEventT)
    {
        LTE_LOG(LTE_LOG_ERR, "input param is NULL pointer!");
        return LTE_FAIL;
    }
    /* 将socket通信事件加入事件列表中 */
    
    pstLteEventT->stComEvent.eiFD = libevent_creat_event(pstLteEventT->eEventBaseID, pstLteEventT->iComFd,
        EV_READ | EV_PERSIST, (void *)lte_read_com_msg, pstLteEventT);
    if(0 == pstLteEventT->stComEvent.eiFD)
    {
        LTE_LOG(LTE_LOG_ERR, "libevent_fd_add FAIL! ");
        return LTE_FAIL;     
    }
    iRet = libevent_add_event(pstLteEventT->stComEvent.eiFD, LTE_LIBEVENT_PRIORITY_FD_COM_LEVEL);

    LTE_LOG(LTE_LOG_INFO, "socket init success, eventId: %u!", pstLteEventT->stComEvent.eiFD);
    return LTE_OK;

}
LTE_RET_E libevent_fd_event_register(LTE_EVENT_T *pstLteEventT)
{
    struct sockaddr_un unServer;
    short    iServerIp[16] = {0};
    short    iFlag = 1;
    int      iValue = 0;
    int      iFd = 0;
    int      iRet = 0;

    if(NULL == pstLteEventT)
    {
        LTE_LOG(LTE_LOG_ERR, "input param is NULL pointer!");
        return LTE_FAIL;
    }
    /* 初始化socket通信 */
    if((iFd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        LTE_LOG(LTE_LOG_ERR, "create socket failed!");
        return LTE_FAIL;
    }
    pstLteEventT->stFdEvent.iFd_v4 = iFd;

    memset(&unServer, 0, sizeof(unServer));
    
    unServer.sun_family = AF_UNIX;
    
    sprintf(unServer.sun_path,"%s",LTE_COM_SOCKET_NAME);
    unlink(unServer.sun_path);
    
    iRet = bind(iFd, (struct sockaddr *)&unServer,sizeof(unServer));
    if(iRet < 0)
    {
        LTE_LOG(LTE_LOG_ERR, "create socket failed!");
        close(iFd);
        return LTE_FAIL;
    }
    iRet = listen(iFd,5);/* 监听server Fd，等待连接队列的最大长度为5 */
    if(iRet < 0)
    {
        LTE_LOG(LTE_LOG_ERR, "create socket failed!");
        close(iFd);
        return LTE_FAIL;
    }

    /* 将socket通信事件加入事件列表中 */
    pstLteEventT->stFdEvent.eiFD = libevent_creat_event(pstLteEventT->eEventBaseID, pstLteEventT->stFdEvent.iFd_v4, EV_READ | EV_PERSIST, (void *)lte_client_on_accept, pstLteEventT);
    if(0 == pstLteEventT->stFdEvent.eiFD)
    {
        LTE_LOG(LTE_LOG_ERR, "libevent_fd_add FAIL! ");
        close(pstLteEventT->stFdEvent.iFd_v4);
        pstLteEventT->stFdEvent.iFd_v4 = 0;
        return LTE_FAIL;     
    }
    LTE_LOG(LTE_LOG_INFO,"##libevent_fd_event_register\n");
    iRet = libevent_add_event(pstLteEventT->stFdEvent.eiFD, LTE_LIBEVENT_PRIORITY_FD_SOCKET_LEVEL);

    LTE_LOG(LTE_LOG_INFO, "socket init success, eventId: %u!", pstLteEventT->stFdEvent.eiFD);
    return LTE_OK;

}

LTE_RET_E libevent_register(LTE_EVENT_T *pstLteEventT)
{
    EVENT_BASE_ID iEventBaseId = 0;
    LTE_RET_E     enRet = LTE_FAIL;

    if(NULL == pstLteEventT)
    {
        LTE_LOG(LTE_LOG_ERR, "input param is NULL pointer!");
        return LTE_FAIL;
    }
    /* libevent init, create event_base ID */
    enRet = init_libevent(&iEventBaseId);
    if(LTE_OK != enRet)
    {
        LTE_LOG(LTE_LOG_ERR, "init_libevent failed!");
        return LTE_FAIL;
    }
    pstLteEventT->eEventBaseID = iEventBaseId;
    
    /* libevent 事件注册 */
    libevent_com_event_register(pstLteEventT);
    libevent_fd_event_register(pstLteEventT);
    
    /* start Event dispatching loop */
    enRet = lte_event_base_dispatch((struct event_base *)(iEventBaseId));
    if(LTE_OK != enRet)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_event_base_dispatch failed!");
        return LTE_FAIL;
    }
    return LTE_OK;
}
