#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <termios.h>
#include <sys/time.h>
#include <fcntl.h>
#include <string.h>  
#include <sys/types.h>  
#include <errno.h>  
#include <sys/stat.h>   
#include <unistd.h>   
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <sys/un.h>
#include <sys/socket.h>

#include "event2/event.h"
#include "lte.h"
#include "lte_log.h"
#include "lteclient.h"
static int lte_client_sock_init(void)
{
    struct sockaddr_un un;
    int iRet = 0, iLoop;
    int iSocketFd = 0;
    
    iSocketFd = socket(AF_UNIX,SOCK_STREAM,0);
    if(iSocketFd < 0)
    {
    	return -1;
    }

    memset(&un,0,sizeof(un));
    un.sun_family = AF_UNIX;
    sprintf(un.sun_path,"%s",LTE_CLIENT_SOCKET_NAME);
    unlink(un.sun_path);

    iRet = bind(iSocketFd,(struct sockaddr *)&un,sizeof(un));
    if(iRet < 0)
    {
        iRet = -2;
    	goto err;
    }

    memset(&un,0,sizeof(un));
    un.sun_family = AF_UNIX;
    sprintf(un.sun_path,"%s",LTE_SERVER_SOCKET_NAME);

    iRet = connect(iSocketFd,(struct sockaddr *)&un,sizeof(un));
    if(iRet < 0)
    {
        iRet = -3;
    	goto err;
    }
    
    return iSocketFd;
    
err:
    close(iSocketFd);
    return iRet;
}
static LTE_RET_E lte_client_send(int iClientSocketFd, CLIENT_SEND_MSG_T *stLteMsg)
{
    char *pcBuf = NULL;
    int  iWriteLen = 0;
        
    pcBuf = (char *)(stLteMsg);
    iWriteLen = write(iClientSocketFd, pcBuf, sizeof(CLIENT_SEND_MSG_T));
    if(iWriteLen != sizeof(CLIENT_SEND_MSG_T))
    {   
        LTE_LOG(LTE_LOG_ERR,"%d,%s:send msg failed, writelen:%d, size%d\n",__LINE__,__func__, iWriteLen, sizeof(CLIENT_SEND_MSG_T));
        return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO,"lte client write %d %d\n", iWriteLen, sizeof(CLIENT_SEND_MSG_T));
    return LTE_OK;

}
static LTE_RET_E lte_client_recv(int iClientSocketFd, LTE_SEND_MSG_T *pstLteMsg, int iTimeOut)
{
    int iRet = 0;
    int iRecvLen = 0;
    fd_set ReadSetFD;
    struct timeval tv;

    tv.tv_sec = iTimeOut;
    tv.tv_usec = 0;
    FD_ZERO(&ReadSetFD);
    FD_SET(iClientSocketFd, &ReadSetFD);

    iRet = select (iClientSocketFd + 1, &ReadSetFD, NULL, NULL, &tv);
    if(iRet < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:select timeout! return\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    if(FD_ISSET(iClientSocketFd, &ReadSetFD))
    {
        iRecvLen = read(iClientSocketFd, (char *)(pstLteMsg), sizeof(LTE_SEND_MSG_T));
        if(iRecvLen <= 0)
        {
            return LTE_FAIL;
        }
        LTE_LOG(LTE_LOG_INFO,"#[lteclient]# :recv:%d, %d\n", iRecvLen, sizeof(LTE_SEND_MSG_T));
    }

    LTE_LOG(LTE_LOG_INFO,"%d,%s:#[lteclient]# lte_client_recv success\n",__LINE__,__func__);
    return LTE_OK;
}
static LTE_RET_E zte_micro_b57_stop_zemci(int iLteFunc)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char *pcBuf = NULL;
    
    char acCmd[] = "at+zemci=0";
    char acCiev[] = "OK";

    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};
        
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = iLteFunc;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_ERR,"#[lteclient]#acAtCmd: %s, acCiev: %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
    stLteMsg.iComReadType = COM_READ_TYPE_1;
    stLteMsg.iComWriteType = COM_WRITE_TYPE_1;
    stLteMsg.iComReturnType = COM_RETURN_SINGLE;
    stLteMsg.iNodeSM = WAIT_WRITE;

    iRet = lte_client_send(iClientSocketFd, &stLteMsg);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_send failed!\n",__LINE__,__func__);
        close(iClientSocketFd);
        return LTE_FAIL;
    }

    iRet = lte_client_recv(iClientSocketFd, &stLteRecvMsg, 2);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_recv failed!\n",__LINE__,__func__);
        close(iClientSocketFd);
        return LTE_FAIL;
    }
    
    close(iClientSocketFd);
    LTE_LOG(LTE_LOG_INFO,"########################:result:%d[0:fail;1:success]\n", stLteRecvMsg.iResult);

    if(1 == stLteRecvMsg.iResult)
    {
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;
}


static LTE_RET_E zte_micro_b57_5_listen_cellinfo(void)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    //char acCmd[] = "at+zemci=1";
    char acCiev[] = "+ZEMCI:";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    
    LTE_LOG(LTE_LOG_ERR,"%d,%s:zte_micro_b57_5_listen_cellinfo lteclient\n",__LINE__,__func__);
    
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_INFO_GET;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
	//strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
    stLteMsg.iComReadType = COM_READ_TYPE_7;
    stLteMsg.iComWriteType = COM_WRITE_TYPE_1;
    stLteMsg.iComReturnType = COM_RETURN_SINGLE;
    stLteMsg.iNodeSM = NOT_NEED_WRITE;
    stLteMsg.iServerSendType = SERVER_NOT_SEND;

    iRet = lte_client_send(iClientSocketFd, &stLteMsg);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_send failed!\n",__LINE__,__func__);
        close(iClientSocketFd);
        return LTE_FAIL;
    }
    
    close(iClientSocketFd);
    LTE_LOG(LTE_LOG_INFO,"########################:result:%d[0:fail;1:success]\n", stLteRecvMsg.iResult);

    return iRet;

}

static LTE_RET_E zte_micro_b57_5_lteinfo_signal_get (int *iRssi, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+zrssi";
    char acCiev[] = "+ZRSSI:";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == iRssi)
    {
        return LTE_FAIL;
    }
    
    LTE_LOG(LTE_LOG_DEBUG,"%d,%s:zte_micro_b57_5_pin_state lteclient.\n",__LINE__,__func__);
    
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_INFO_GET;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
    stLteMsg.iComReadType = COM_READ_TYPE_4;
    stLteMsg.iComWriteType = COM_WRITE_TYPE_1;
    stLteMsg.iComReturnType = COM_RETURN_SINGLE;
    stLteMsg.iNodeSM = WAIT_WRITE;

    iRet = lte_client_send(iClientSocketFd, &stLteMsg);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_send failed!\n",__LINE__,__func__);
        close(iClientSocketFd);
        return LTE_FAIL;
    }

    iRet = lte_client_recv(iClientSocketFd, &stLteRecvMsg, 2);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_recv failed!\n",__LINE__,__func__);
        close(iClientSocketFd);
        return LTE_FAIL;
    }
    
    close(iClientSocketFd);
    LTE_LOG(LTE_LOG_INFO,"########################:result:%d[0:fail;1:success]\n", stLteRecvMsg.iResult);

    if(1 == stLteRecvMsg.iResult)
    {
        *iRssi = stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.iRssi;
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;
}
static LTE_RET_E zte_micro_b57_sinr_get(int *piSinr)
{   
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+zsinr";
    char acCiev[] = "+ZSINR:";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    
    LTE_LOG(LTE_LOG_DEBUG,"%d,%s:zte_micro_b57_5_pin_state lteclient.\n",__LINE__,__func__);
    //LTE_MODULE_VERSION_T *pstVersion = (LTE_MODULE_VERSION_T *)pvData;
    
    if(NULL == piSinr)
    {
        return LTE_FAIL;
    }
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_INFO_GET;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
    stLteMsg.iComReadType = COM_READ_TYPE_4;
    stLteMsg.iComWriteType = COM_WRITE_TYPE_1;
    stLteMsg.iComReturnType = COM_RETURN_SINGLE;
    stLteMsg.iNodeSM = WAIT_WRITE;

    iRet = lte_client_send(iClientSocketFd, &stLteMsg);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_send failed!\n",__LINE__,__func__);
        close(iClientSocketFd);
        return LTE_FAIL;
    }

    iRet = lte_client_recv(iClientSocketFd, &stLteRecvMsg, 2);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_recv failed!\n",__LINE__,__func__);
        close(iClientSocketFd);
        return LTE_FAIL;
    }
    
    close(iClientSocketFd);
    LTE_LOG(LTE_LOG_INFO,"########################:result:%d[0:fail;1:success]\n", stLteRecvMsg.iResult);

    if(1 == stLteRecvMsg.iResult)
    {
        *piSinr = stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.iSinr;
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    return iRet;
   
}
static LTE_RET_E zte_micro_b57_5_cellinfo_get(LTE_INFO_T *pstLteInfo)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+zemci=1";
    char acCiev[] = "OK";//"+ZEMCI:";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pstLteInfo)
    {
        return LTE_FAIL;
    }
    
    LTE_LOG(LTE_LOG_DEBUG,"%d,%s:zte_micro_b57_5_pin_state lteclient.\n",__LINE__,__func__);
    //LTE_MODULE_VERSION_T *pstVersion = (LTE_MODULE_VERSION_T *)pvData;
    
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_INFO_GET;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
    stLteMsg.iComReadType = COM_READ_TYPE_1;//COM_READ_TYPE_8;
    stLteMsg.iComWriteType = COM_WRITE_TYPE_1;
    stLteMsg.iComReturnType = COM_RETURN_SINGLE;
    stLteMsg.iNodeSM = WAIT_WRITE;

    iRet = lte_client_send(iClientSocketFd, &stLteMsg);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_send failed!\n",__LINE__,__func__);
        close(iClientSocketFd);
        return LTE_FAIL;
    }

    iRet = lte_client_recv(iClientSocketFd, &stLteRecvMsg, 2);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_recv failed!\n",__LINE__,__func__);
        close(iClientSocketFd);
        sleep(2);
        zte_micro_b57_stop_zemci(LTE_INFO_GET);
        return LTE_FAIL;
    }
    
    close(iClientSocketFd);
    LTE_LOG(LTE_LOG_INFO,"########################:result:%d[0:fail;1:success]\n", stLteRecvMsg.iResult);

    if(1 == stLteRecvMsg.iResult)
    {
        #if 0
        strncpy(pstLteInfo->stCellInfo.acBand, stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.acBand, sizeof(pstLteInfo->stCellInfo.acBand));
        strncpy(pstLteInfo->stCellInfo.acCellId, stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.acCellId, sizeof(pstLteInfo->stCellInfo.acCellId));
        strncpy(pstLteInfo->stCellInfo.acPhyCeillid, stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.acPhyCeillid, sizeof(pstLteInfo->stCellInfo.acPhyCeillid));
        pstLteInfo->stCellInfo.iChannel = stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.iChannel;
        pstLteInfo->stCellInfo.iRsrp = stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.iRsrp;
        pstLteInfo->stCellInfo.iRsrq = stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.iRsrq;
        #endif
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    sleep(2);
    zte_micro_b57_stop_zemci(LTE_INFO_GET);
    return iRet;

}
static LTE_RET_E zte_micro_b57_5_lte_info_get(void *pvData, int iDataLen)
{
    LTE_BAND_T    stBandInfo = {0};
    LTE_SIGNAL_T  stRssiInfo = {0};
    int           iSinr = 0;
    int           iRssi = 0;
	LTE_INFO_T *pstLteInfo = (LTE_INFO_T *)pvData;
	static int    s_iTimerCount = 0;

    if (pvData == NULL)
    	return LTE_FAIL;
	
    if(LTE_OK != zte_micro_b57_5_lteinfo_signal_get(&iRssi, sizeof(LTE_SIGNAL_T)))
    {
        LTE_LOG(LTE_LOG_ERR, "get rssi info fail");
    }
    if(LTE_OK != zte_micro_b57_sinr_get(&iSinr))
    {
        LTE_LOG(LTE_LOG_ERR, "get sinr info fail");
    }
    if(LTE_OK != zte_micro_b57_5_cellinfo_get(pstLteInfo))
    {
        LTE_LOG(LTE_LOG_ERR, "zte_micro_b57_5_cellinfo_get cell info get fail");
    }
    
    pstLteInfo->stCellInfo.iRssi = iRssi;
    pstLteInfo->stCellInfo.iSinr = iSinr;
    if(s_iTimerCount%10 == 0)
    {
        zte_micro_b57_5_listen_cellinfo();
    }
    ++s_iTimerCount;
    return LTE_OK;
}
/*****************************************************************************
 函 数 名  : lte_timer_create
 功能描述  : 创建定时器
 输入参数  : LTE_EVENT_T *pstLteEventT       
             LTE_EVENT_TIMER_T *pstSigTimer  
             int uiTimeInterval              
             void (*callback)(int            
             short                           
             void *)                         
             void *arg                       
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年2月3日星期二
    作    者   : liyoukun
    修改内容   : 新生成函数

*****************************************************************************/
LTE_RET_E lte_timer_create( LTE_LIB_EVENT_T *piLibeventID, LTE_EVENT_TIMER_T *pstTimer, int uiTimeInterval, void (*callback)(int, short, void *), void *arg )
{
    
    struct event *pstEv = NULL;

    if((NULL == piLibeventID) || (NULL == pstTimer))
    {
        LTE_LOG(LTE_LOG_ERR, "input param is NULL pointer!");
        return LTE_FAIL;
    }
    
    pstTimer->stTimeVal.tv_sec = uiTimeInterval;
    pstTimer->stTimeVal.tv_usec = 0;

    
    /* 创建定时器 */
    pstEv = evtimer_new((struct event_base *)(piLibeventID->eEventBaseID), callback, arg);

    if(NULL == pstEv)
    {
       LTE_LOG(LTE_LOG_ERR, "evtimer_new call fail, do not get a event!");
       return LTE_FAIL;
    }

    pstTimer->eiTimer = (EVENT_ID)pstEv;
    LTE_LOG(LTE_LOG_INFO,"even_id:%d\n", pstTimer->eiTimer);
    /* 将该事件加入事件堆中 */
    if(0 != event_add((struct event *)(pstTimer->eiTimer), &(pstTimer->stTimeVal)))
    {
       LTE_LOG(LTE_LOG_ERR, "event_add call fail, do not add a event to event_base!");
       event_free(pstEv);
       return LTE_FAIL;
    }
    return LTE_OK;
}
/*****************************************************************************
 函 数 名  : lte_timer_close
 功能描述  : 关闭timer定时器
 输入参数  : EVENT_ID even_id  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年1月27日星期二
    作    者   : liyoukun
    修改内容   : 新生成函数

*****************************************************************************/
void lte_timer_close( EVENT_ID even_id )
{
    if ( 0 ==  even_id)
    {
        LTE_LOG(LTE_LOG_INFO, "Timer event is not exist!\n");
        return ;
    }
    
    if (NULL == (struct event *) even_id)
    {
        LTE_LOG(LTE_LOG_ERR, "evtimer is null, call fail!\n");
        return ;
    }
    if (0 != event_del((struct event *)even_id))
    {
        LTE_LOG(LTE_LOG_ERR, "event_del call fail!\n");
        return ;
    }

    event_free((struct event *)even_id);
    return ;
}
LTE_RET_E lte_timer_event_add(EVENT_ID even_id, struct timeval *timer_tv)
{
    if ( (0 ==  even_id) || (NULL == timer_tv))
    {
        LTE_LOG(LTE_LOG_INFO, "Timer event is not exist!\n");
        return LTE_FAIL;
    }
    if(0 != event_add((struct event *)even_id, timer_tv))
    {
       LTE_LOG(LTE_LOG_ERR, "event_add call fail, do not add a event to event_base!");
       event_free((struct event *)even_id);
       return LTE_FAIL;
    }
    return LTE_OK;
}

static void lte_lteinfo_timer_handle(int iFd, short event, void *pvData)
{
    int iRet = 0;
    LTE_INFO_T stLteInfo = {0};
    if(NULL == pvData)
    {
        LTE_LOG(LTE_LOG_ERR, "input param is NULL pointer!");
        return ;
    }
    LTE_EVENT_TIMER_T *pstTimerEvent = (LTE_EVENT_TIMER_T *)pvData;
    
    LTE_LOG(LTE_LOG_INFO,"############eventid:%d\n", pstTimerEvent->eiTimer);
    /* 从USB COM中获取lte signal信息 */
    iRet = zte_micro_b57_5_lte_info_get(&stLteInfo, sizeof(LTE_INFO_T));
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR, "get lte info failed!");
    }

    /* 重新启动定时器 */
    if(0 != event_add((struct event *)(pstTimerEvent->eiTimer), &(pstTimerEvent->stTimeVal)))
	{
		LTE_LOG(LTE_LOG_ERR, "event_add call failed, do not add sig_timer to event_base!");
		event_free((struct event *)(pstTimerEvent->eiTimer));
		return ;
	}
	return ;
}
LTE_RET_E lte_lteinfo_timer_register(LTE_LIB_EVENT_T *pstLteEventT)
{   
    int iRet = 0;
    if(NULL == pstLteEventT)
    {
        LTE_LOG(LTE_LOG_ERR, "input param is NULL pointer!");
        return LTE_FAIL;
    }
    iRet = lte_timer_create(pstLteEventT, &(pstLteEventT->stLteInfoTimer), LTE_CELLID_TIMER_INTERVAL, lte_lteinfo_timer_handle, &(pstLteEventT->stLteInfoTimer));
	if( LTE_OK != iRet )
	{
	    LTE_LOG(LTE_LOG_ERR, "add lteinfo_timer failed");
	}
    #if 0
    else
    {
        /* 设置事件优先级 */
        if((iRet = event_priority_set((struct event *)(pstLteEventT->stLteInfoTimer.eiTimer), LTE_LIBEVENT_PRIORITY_TIMER_LEVEL)) < 0 )
        {
            LTE_LOG(LTE_LOG_ERR, "event_priority_set exit failed");
            return LTE_FAIL;
        }
    }
  
    LTE_LOG(LTE_LOG_INFO,"lte_lteinfo_timer_register\n");
    iRet = lte_timer_event_add(pstLteEventT->stLteInfoTimer.eiTimer, &(pstLteEventT->stLteInfoTimer.stTimeVal));
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_timer_event_add exit failed");
    }
    #endif
    
    return iRet;
}
LTE_RET_E libevent_init(EVENT_BASE_ID *piEBaseID)
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
    #if 0
    if((iRet = event_base_priority_init(base, EVENT_MAX_PRIORITIES - 1)) < 0)
    {
        event_base_free(base);
        return LTE_FAIL;
    }
    #endif
    LTE_LOG(LTE_LOG_INFO, "init_libevent SUCCESS!!!");
    return LTE_OK;
}
LTE_RET_E lib_event_base_dispatch(struct event_base *pstEventBase)
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
int main(int argc, char** args)
{
	EVENT_BASE_ID iLibEventBaseID;
    LTE_LIB_EVENT_T stLibEvent = {0};
    
    libevent_init(&iLibEventBaseID);
    stLibEvent.eEventBaseID = iLibEventBaseID;

    if(LTE_OK != zte_micro_b57_5_listen_cellinfo())
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lteclient listen send failed!\n",__LINE__,__func__);
        return -1;
    }
    
    lte_lteinfo_timer_register(&stLibEvent);
    lib_event_base_dispatch((struct event_base *)(iLibEventBaseID));
	return 0;
}
