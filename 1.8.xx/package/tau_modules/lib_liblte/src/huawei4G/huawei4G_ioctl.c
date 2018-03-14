#include <stdio.h>
#include <sys/select.h>
#include <termios.h>
#include <sys/time.h>
#include <fcntl.h>
#include <string.h>  
#include <sys/types.h>  
#include <errno.h>  
#include <sys/stat.h>   
#include <unistd.h>    
#include <stdlib.h>  
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <setjmp.h>

#include "lte.h"
#include "lte_log.h"
#include "lte_common.h"
#include "lte_usbcom.h"

#define ATCOM "/dev/ttyUSB2"

static LTE_RET_E huawei4G_cerssi_get(LTE_INFO_T *pstLteInfoTmp, int iDataLen);

jmp_buf Socket_Failed;

static void alarm_handler(void)
{
    longjmp(Socket_Failed, 1);
}

static int lte_client_sock_init(void)
{
    struct sockaddr_un un;
    int iRet = 0, iLoop;
    int iSocketFd = 0;
	#if 0
    if (setjmp(Socket_Failed)) /* 实例化异常 */
    {
        if(iSocketFd > 0)
        {
            close(iSocketFd);
        }
        LTE_LOG(LTE_LOG_ERR,"%d,%s:client sock init failed.\n",__LINE__,__func__);
        return -1;
    }
    signal(SIGALRM, alarm_handler); //让内核做好准备，一旦接受到SIGALARM信号,就执行 handler
    alarm(1);
	#endif
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
    	close(iSocketFd);
        return iRet;
    }

    memset(&un,0,sizeof(un));
    un.sun_family = AF_UNIX;
    sprintf(un.sun_path,"%s",LTE_SERVER_SOCKET_NAME);

    iRet = connect(iSocketFd,(struct sockaddr *)&un,sizeof(un));
    if(iRet < 0)
    {
        iRet = -3;
    	close(iSocketFd);
        return iRet;
    }
    return iSocketFd;
    
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
    if(iRet == 0)
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
        
        return LTE_OK;
    }
    
    LTE_LOG(LTE_LOG_INFO,"%d,%s:#[lteclient]# select return fail\n",__LINE__,__func__);
    return LTE_FAIL;
}


/*****************************************************************************
 函 数 名  : leadcore_pin_state
 功能描述  : 获取PIN码状态
 输入参数  : void *pvData    
             int iDataLen  
 输出参数  : 无
 返 回 值  : LTE_RET_E
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月5日
    作    者   : 陈键
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E huawei4G_pin_state (void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cpin?";
    char acCiev[] = "+CPIN:";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_PIN_STATE_T *pstPinState = (LTE_PIN_STATE_T*)pvData;
    
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_PIN_STATE;
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
        pstPinState->enState = stLteRecvMsg.uLteCommand.stPinStateInfo.enState;
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;

}

/*****************************************************************************
 函 数 名  : leadcore_pin_switch
 功能描述  : 开启和关闭pin码功能
 输入参数  : void *pvData    
             int iDataLen  
 输出参数  : 无
 返 回 值  : LTE_RET_E
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月6日星期四
    作    者   : 陈键
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E huawei4G_pin_switch (void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[64] = {0};
    char acCiev[] = "OK";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_PIN_SWITCH_T *pstPinSwitch = (LTE_PIN_SWITCH_T*)pvData;

    snprintf(acCmd, sizeof(acCmd), "at+clck=\"SC\",%d,\"%s\"", pstPinSwitch->enSwitch,pstPinSwitch->acPin);
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_PIN_SWITCH;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
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
/*****************************************************************************
 函 数 名  : leadcore_pin_sw_get
 功能描述  : 获取pin是否开启，1开启，0关闭
 输入参数  : void *pvData  
             int iDataLen  
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年11月6日星期四
    作    者   : liyoukun
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E huawei4G_pin_sw_get( void *pvData, int iDataLen )
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+clck=\"SC\",2";
    char acCiev[] = "+CLCK:";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_PIN_SW_STATE_T *pstPinSwState = (LTE_PIN_SW_STATE_T*)pvData;
    
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_PIN_SW_GET;
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
        pstPinSwState->enState = stLteRecvMsg.uLteCommand.stPinSwInfo.enState;
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;

}
 
/*****************************************************************************
 函 数 名  : leadcore_pin_unlock
 功能描述  : 解pin
 输入参数  : void *pvData    
             int iDataLen  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月6日星期四
    作    者   : 陈键
    修改内容   : 新生成函数

*****************************************************************************/
 static LTE_RET_E huawei4G_pin_unlock (void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[64] = {0};
    char acCiev[] = "OK";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_PIN_T *pstPinUnlock = (LTE_PIN_T*)pvData;

    snprintf(acCmd, sizeof(acCmd), "at+cpin=\"%s\"", pstPinUnlock->acPin);
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_PIN_UNLOCK;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
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

/*****************************************************************************
 函 数 名  : leadcore_pin_attempts_get
 功能描述  : 获取剩余PIN码尝试次数
 输入参数  : void *pvData    
             int iDataLen  
 输出参数  : 无
 返 回 值  : LTE_RET_E
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月2日
    作    者   : 陈键
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E huawei4G_pin_attempts_get(void *pvData, int iDataLen)
{
    return LTE_OK;
}


/*****************************************************************************
 函 数 名  : leadcore_pin_alter
 功能描述  : 修改pin码
 输入参数  : void *pvData    
             int iDataLen  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月6日星期四
    作    者   : 陈键
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E huawei4G_pin_alter (void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[64] = {0};
    char acCiev[] = "OK";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_PIN_ALTER_T *pstPinAlter = (LTE_PIN_ALTER_T*)pvData;
    pstPinAlter->acNewPin[4] = '\0';

    snprintf(acCmd, sizeof(acCmd), "at+cpwd=\"SC\",\"%s\",\"%s\"", pstPinAlter->acOldPin, pstPinAlter->acNewPin);
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_PIN_ALTER;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
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

/*由于华为模块的频点和频段是在驻留网络时自动上报的，因此要得到这两个值，必须先poweroff再poweron*/
static LTE_RET_E huawei4G_listen_lteinfo(char *pcCiev)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    //char acCiev[] = "^LWURC:";//^LWURC: 0,18900,5,18900,5,39
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_INFO_GET;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acCiev, pcCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s\n", stLteMsg.acCiev);
    
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
    //LTE_LOG(LTE_LOG_INFO,"########################:result:%d[0:fail;1:success]\n", stLteRecvMsg.iResult);

    return iRet;

}
//^LWURC: 0,18900,5,18900,5,39
//^CSG:341,-832,-160,-536,0,0

static LTE_RET_E huawei4G_power_on (void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cfun=1";
    char acCiev[] = "OK";
    char *pcBuf = NULL;
    char acCiev1[] = "^LWURC:";
    char acCiev2[] = "^CSG:";
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};
	#if 0
    /*poweron 之前，先监控*/
    if(LTE_OK != huawei4G_listen_lteinfo(acCiev1))
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:huawei4G_listen(band) lteinfo failed!!\n",__LINE__,__func__);
    }
    /*if(LTE_OK != huawei4G_listen_lteinfo(acCiev2))
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:huawei4G_listen(PhyCeillid) lteinfo failed!!\n",__LINE__,__func__);
    }*/
    #endif
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_POWER_ON;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);

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

static LTE_RET_E huawei4G_power_off (void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cfun=0";
    char acCiev[] = "OK";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};    

    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_POWER_OFF;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
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

static LTE_RET_E huawei4G_rssi_get  (int *piSignal)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "AT+CSQ";
    char acCiev[] = "+CSQ:";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == piSignal)
    {
        return LTE_FAIL;
    }
    
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_SIGNAL_GET;
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
        *piSignal = stLteRecvMsg.uLteCommand.stSignalInfo.uiSignal;
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;

}

static LTE_RET_E huawei4G_signal_get  (void *pvData, int iDataLen)
{
    char *pcBuf = NULL;
    LTE_INFO_T stLteInfo = {0};
    int iSignal = 0;

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_SIGNAL_T *pstSignal = (LTE_SIGNAL_T*)pvData;

    if(LTE_OK != huawei4G_cerssi_get(&stLteInfo, sizeof(LTE_INFO_T)))
    {
        LTE_LOG(LTE_LOG_ERR, "get Rsrp Rsrq Sinr fail");
        return LTE_FAIL;
    }
    iSignal = stLteInfo.stCellInfo.iRsrp;
    if(iSignal < -115 || iSignal >= 0)
    {
        pstSignal->uiSignal = 0;    
    }
    else if(iSignal < -105)
    {
        pstSignal->uiSignal = 1;
    }
    else if(iSignal < -95)
    {
        pstSignal->uiSignal = 2;
    }
    else if(iSignal < -85)
    {
        pstSignal->uiSignal = 3;
    }
    else if(iSignal < -75)
    {
        pstSignal->uiSignal = 4;
    }
    else
    {
        pstSignal->uiSignal = 5;
    }
    return LTE_OK;


}
#if 0
static LTE_RET_E leadcore_set_nettype_cmd(LTE_NET_TYPE_T *pstNetType,char *pcComddtm,char *pcComdstm)
{
    if( NULL == pstNetType || NULL == pstNetType)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:input parameter is NULL",__LINE__,__func__);
        return LTE_FAIL;
    }
         
    switch(pstNetType->enNetType)
    {
        case LTE_4G_ONLY:
        {
            sprintf(pcComddtm, "at^ddtm=1,1,7"); /* 仅LTE */
            sprintf(pcComdstm, "at^dstmex=1,7"); 
            break;
        }
        
        case LTE_2G_ONLY:
        {
            sprintf(pcComddtm, "at^ddtm=1,1,0"); /* 仅GSM */
            sprintf(pcComdstm, "at^dstmex=1,0");
            break;
        }
        case LTE_3G_ONLY: 
        {
            sprintf(pcComddtm, "at^ddtm=1,1,2"); /* 仅TDS */
            sprintf(pcComdstm, "at^dstmex=1,2");
            break;
        }
        case LTE_3G_4G: /* TDD,TDS */
        {
            if( pstNetType->enNetTypePri == LTE_3G_PRI)
            {
                sprintf(pcComddtm, "at^ddtm=1,0,2,7"); 
                sprintf(pcComdstm, "at^dstmex=0,2,7");
            }
            else if( pstNetType->enNetTypePri == LTE_4G_PRI)
            {
                sprintf(pcComddtm, "at^ddtm=1,0,7,2");
                sprintf(pcComdstm, "at^dstmex=0,7,2");
            }
            else if( pstNetType->enNetTypePri == LTE_AUTO)
            {
                sprintf(pcComddtm, "at^ddtm=1,0,7,2");
                sprintf(pcComdstm, "at^dstmex=0,7,2");
            }
            else
            {
                LTE_LOG(LTE_LOG_ERR,"%d,%s:NO netTypePri in the netTyper,default 4g pri",__LINE__,__func__);
                sprintf(pcComddtm, "at^ddtm=1,0,7,2");
                sprintf(pcComdstm, "at^dstmex=0,7,2");
            }
            break;
        }
        
        
        default:
        {
            LTE_LOG(LTE_LOG_ERR,"%d,%s:unkown netTyper",__LINE__,__func__);
            return LTE_FAIL;
        }
    }
    return LTE_OK;
    
}

static LTE_RET_E leadcore_nettype_set (void *pvData, int iDataLen)
{  
    int iRet = 0;
    int iClientSocketFd = -1;
    char acCmd1[32] = {0};
    char acCmd2[32] = {0};
    char acCiev[] = "OK";
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};
    
    if (NULL == pvData)
    {
        return LTE_FAIL;
    }    
    
    LTE_NET_TYPE_T *pstNetType = (LTE_NET_TYPE_T *)pvData;    
    if( LTE_FAIL == leadcore_set_nettype_cmd(pstNetType, acCmd1, acCmd2))
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:set netType cmd fail",__LINE__,__func__);
        return LTE_FAIL;
    }
    if( LTE_FAIL == recv_only_ok(acCmd1, LTE_NETTYPE_SET, 0))
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:set netType cmd fail",__LINE__,__func__);
        return LTE_FAIL;
    }
    if(strlen(acCmd2) > 0)
    {
        if( LTE_FAIL == recv_only_ok(acCmd2, LTE_NETTYPE_SET, 0))
        {
            LTE_LOG(LTE_LOG_ERR,"%d,%s:set netType cmd fail",__LINE__,__func__);
            return LTE_FAIL;
        }
    }
   
    return LTE_OK;//iRet;
}


static LTE_RET_E leadcore_nettype_get(void *pvData, int iDataLen)
{
    int iRet = 0;
    int iClientSocketFd = -1;
    char acCmd[] = "at^ddtm?";
    char acCiev[] = "^DDTM:";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};
    
    LTE_NET_TYPE_T *pstNetType = (LTE_NET_TYPE_T *)pvData;

    iClientSocketFd = lte_client_sock_init();
    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_NETTYPE_GET;
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
        pstNetType->enNetType = stLteRecvMsg.uLteCommand.stCurNetTypeInfo.enNetType;
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;
    
}
static LTE_RET_E leadcore_plmn_register(void *pvData, int iDataLen)
{
     return LTE_OK;
}


/*****************************************************************************
 函 数 名  : set_prefer_plmn
 功能描述  : 设置plmn 中td-lte,fdd-lte优先，暂时模块不支持，预留接口
 输入参数  :  
 输出参数  : 
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月17 星期五
    作    者   : 蔡兵
    修改内容   : 新生成函数

*****************************************************************************/

static LTE_RET_E leadcore_prefer_plmn_set (void *pvData, int iDataLen)
{
    return LTE_OK;
}
#endif
static LTE_RET_E huawei4G_linkstate_get(void *pvData, int iDataLen)
{
    int iRet = 0;
    int iClientSocketFd = -1;
    char acCmd[] = "AT^NDISSTATQRY?";
    char acCiev[] = "^NDISSTATQRY:";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};
    
     LTE_LINKSTATE_T *pstLinkstate = (LTE_LINKSTATE_T*)pvData;

    iClientSocketFd = lte_client_sock_init();
    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_LINKSTATE_GET;
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

    iRet = lte_client_recv(iClientSocketFd, &stLteRecvMsg, 5);
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
        pstLinkstate->enLinkstate = stLteRecvMsg.uLteCommand.stLinkInfo.enLinkstate;
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;
    
}

static LTE_RET_E huawei4G_imei_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "AT+CGSN";
    char acCiev[] = "OK";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
	LTE_IMEI_T *pstImei = (LTE_IMEI_T *)pvData;
    
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_IMEI_GET;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 1;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
    stLteMsg.iComReadType = COM_READ_TYPE_5;
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
        strncpy(pstImei->acImei, stLteRecvMsg.uLteCommand.stImeiInfo.acImei, sizeof(pstImei->acImei));
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;

}

static LTE_RET_E huawei4G_set_cereg_return_type(int iValue, int iFuncCmd)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char *pcBuf = NULL;
    
    char acCmd[32] = {};
    char acCiev[] = "OK";

    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    snprintf(acCmd, sizeof(acCmd),"at+cereg=%d", iValue);
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = iFuncCmd;
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
#if 0

static LTE_RET_E huawei4G_cellid_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cereg?";
    char acCiev[] = "+CEREG:";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    iRet = huawei4G_set_cereg_return_type(2, LTE_CELLID_GET);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s: huawei4G_set_cereg_return_type [2] failed\n",__LINE__,__func__);
        iRet = LTE_FAIL;
        goto fail;
    }
    
    LTE_CELLID_T *pstCellid = (LTE_CELLID_T*)pvData;
    
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        iRet = LTE_FAIL;
        goto fail;
    }
    
    stLteMsg.iFuncCmd = LTE_CELLID_GET;
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
        iRet = LTE_FAIL;
        goto fail;
    }

    iRet = lte_client_recv(iClientSocketFd, &stLteRecvMsg, 2);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_recv failed!\n",__LINE__,__func__);
        close(iClientSocketFd);
        iRet = LTE_FAIL;
        goto fail;
    }
    
    close(iClientSocketFd);
    LTE_LOG(LTE_LOG_INFO,"########################:result:%d[0:fail;1:success]\n", stLteRecvMsg.iResult);

    if(1 == stLteRecvMsg.iResult)
    {
        strncpy(pstCellid->acCellID, stLteRecvMsg.uLteCommand.stCellIdInfo.acCellID, sizeof(pstCellid->acCellID));
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
fail:
    if(LTE_OK != huawei4G_set_cereg_return_type(0, LTE_CELLID_GET))
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s: huawei4G_set_cereg_return_type [0] failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    return iRet;


}

#else
static LTE_RET_E huawei4G_cellid_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at^cecellid?";
    char acCiev[] = "^CECELLID:";
    char *pcBuf = NULL;
    //^CECELLID: 46000,25694475,246,6234
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_CELLID_T *pstCellid = (LTE_CELLID_T*)pvData;
    
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_CELLID_GET;
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
        strncpy(pstCellid->acCellID, stLteRecvMsg.uLteCommand.stCellIdInfo.acCellID, sizeof(pstCellid->acCellID));
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;


}
#endif
static LTE_RET_E huawei4G_iccid_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at^iccid?";
    char acCiev[] = "^ICCID:";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
	LTE_ICCID_T *pstIccid = (LTE_ICCID_T *)pvData;
    
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_ICCID_GET;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 1;
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
        strncpy(pstIccid->acICCID, stLteRecvMsg.uLteCommand.stIccidInfo.acICCID, sizeof(pstIccid->acICCID));
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;
   
}
static LTE_RET_E huawei4G_imsi_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cimi";
    char acCiev[] = "OK";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }

    LTE_IMSI_T *pstImsiInfo = (LTE_IMSI_T *)pvData;
    
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_IMSI_GET;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 1;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
    stLteMsg.iComReadType = COM_READ_TYPE_5;
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
        strncpy(pstImsiInfo->acIMSI, stLteRecvMsg.uLteCommand.stImsiInfo.acIMSI, sizeof(pstImsiInfo->acIMSI));
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;
    
}

static int huawei4G_cereg_get(char *pcCmd, char *pcCiev)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};
    LTE_NETWORK_PREPARE_T stNwStatusInfo;
    int iCreg = 0;
        
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_NETWORK_PREPARE;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, pcCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, pcCiev, sizeof(stLteMsg.acCiev));
    
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
        iCreg = atoi(stLteRecvMsg.uLteCommand.stNwStatusInfo.acNetWorkStatus);
    }
    else
    {
        iCreg = 0;
    }
    
    return iCreg;

}

static LTE_RET_E huawei4G_network_prepare(void *pvData, int iDataLen)
{
	int iRegsiterStatus = 0;
    char acNeedleCgreg[16] = "+CGREG";
    char acNeedleCereg[16] = "+CEREG";
    char acNeedleCreg[16] = "+CREG";
    
    char acCmdCgreg[16] =  "at+cgreg?";
    char acCmdCereg[16] =  "at+cereg?";
    char acCmdCreg[16] =  "at+creg?";
	
    
    iRegsiterStatus = huawei4G_cereg_get(acCmdCereg, acNeedleCereg);
    if ((1 == iRegsiterStatus) || (5 == iRegsiterStatus))
    {
        return LTE_OK;
    }
    iRegsiterStatus = huawei4G_cereg_get(acCmdCgreg, acNeedleCgreg);
    if ((1 == iRegsiterStatus) || (5 == iRegsiterStatus))
    {
        return LTE_OK;
    }
    iRegsiterStatus = huawei4G_cereg_get(acCmdCreg, acNeedleCreg);
    if ((1 == iRegsiterStatus) || (5 == iRegsiterStatus))
    {
        return LTE_OK;
    }
    return LTE_FAIL;
}

static LTE_RET_E huawei4G_connect_begin(char *pcCmd, char *pcCiev)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};
    int iCreg = 0;
    
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_CONNECT;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, pcCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, pcCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
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

    iRet = lte_client_recv(iClientSocketFd, &stLteRecvMsg, 5);
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

LTE_RET_E huawei4G_connect(void *pvData, int iDataLen)
{
    int iRet;
    char acFileName[64] = {"/tmp/device_mode.tmp"};
    FILE *pFile = NULL;
    char acApn[32] = {0};
    char acBuf[32] = {0};
    char acPDPStack[16] = {0};
    char acLteCmd[32] = "AT^NDISDUP=1,1";
    char acCiev[] = "^NDISSTAT:1,";//OK  \r  ^NDISSTAT:1,,,"IPV4"
    char cDeviceMode = '0';
     
    LTE_APN_T *pstApn = (LTE_APN_T *)pvData;
    if(NULL == pstApn)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:input param is null pointer!\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    system_call("ifconfig usb0 up"); 

    iRet = huawei4G_connect_begin(acLteCmd, acCiev);
	if (iRet != LTE_OK)
	{
    	LTE_LOG(LTE_LOG_ERR,"%d,%s:at command failed\n",__LINE__,__func__);
    	return LTE_FAIL;
	}

	#if 1
    else
    {
        if(NULL != strstr(pstApn->acApn, "+"))
        {
            sscanf(pstApn->acApn, "%[^+]+%s", acApn, acPDPStack);
            if(NULL != strstr(acPDPStack,"v6"))
            {
	            sleep(3);
	            system("ifconfig usb0 down up");
        	}
        
	        if(NULL != strstr(acPDPStack, "v4"))
	        {
		        system("ifconfig usb0 0.0.0.0");
                
		        //system("kill -9 $(pidof "DHCLIENT")");

		        //system("killall dhclient");

		        system("kill -9 $(pidof "DHCPC")");

		        system("killall udhcpc");   
        
		        if (NULL !=  (pFile = fopen(acFileName, "rb")))
		        {
		            while (fgets(acBuf, sizeof(acBuf), pFile) != NULL )
		            {     
		                cDeviceMode = acBuf[0];
		                memset(acBuf,0,sizeof(acBuf));
		                break;
		            }
		            fclose(pFile);
		        }
                
                if ( '0' == cDeviceMode) //fat
                {
                    system_call("udhcpc -i usb0 -s /etc/udhcpc.script &");
                }
                else
                {
                    //system("dhclient -p 68 usb0 -cf /var/db/dhclient.conf &");
                } 
            }
        }
        else
        {
            system("ifconfig usb0 0.0.0.0");
                            
            //system("kill -9 $(pidof "DHCLIENT")");
        
            //system("killall dhclient");
            
            system("kill -9 $(pidof "DHCPC")");
            
            system("killall udhcpc");   
            
            if (NULL != (pFile = fopen(acFileName, "rb")))
            {
                while (fgets(acBuf, sizeof(acBuf), pFile) != NULL )
                {     
                    cDeviceMode = acBuf[0];
                    memset(acBuf,0,sizeof(acBuf));
                    break;
                }
                fclose(pFile);
            }
            
	        if ( '0' == cDeviceMode) //fat
	        {
	            system_call("udhcpc -i usb0 -s /etc/udhcpc.script &");
	        }
	        else
	        {
	            //system("dhclient -p 68 usb0 -cf /var/db/dhclient.conf &");
            } 
        }   
    }
	#endif
    return LTE_OK;
    
}

static LTE_RET_E huawei4G_disconnect(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
	int iRet = 0;
	char acCmd[64] = "AT^NDISDUP=1,0";
	char acCiev[] = "^NDISSTAT:0,";
	CLIENT_SEND_MSG_T stLteMsg = {0};
	LTE_SEND_MSG_T stLteRecvMsg = {0};
    
	iClientSocketFd = lte_client_sock_init();

	if(iClientSocketFd < 0)
	{
	   LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
	   return LTE_FAIL;
	}

	stLteMsg.iFuncCmd = LTE_DISCONNECT;
	stLteMsg.iPriority = 0;
	stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));

	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);

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
	#if 1
        system("ifconfig usb0 0.0.0.0");
    
        //system("kill -9 $(pidof "DHCLIENT")");

        //system("killall dhclient");

        system("kill -9 $(pidof "DHCPC")");

        system("killall udhcpc");
	#endif
	    iRet = LTE_OK;
	}
	else
	{
	   iRet = LTE_FAIL;
	}

    return iRet;
}

static LTE_RET_E huawei4G_sim_state_get(void *pvData, int iDataLen)
{
	int iClientSocketFd = -1;
    int iRet = 0;
	char acCmd[] = "at+cpin?";
    char acCiev[] = "+CPIN:";
    //char acCmd[] = "at^rsim?";
    //char acCiev[] = "^RSIM:";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
	LTE_SIM_STATE_T *pstSimState = (LTE_SIM_STATE_T *)pvData;
    
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_SIM_STATE_GET;
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
        pstSimState->enSimState = stLteRecvMsg.uLteCommand.stSimStateInfo.enSimState;
        iRet = LTE_OK;
    }
    else
    {
        pstSimState->enSimState = LTE_SIM_NOT_FOUND;
        iRet = LTE_OK;
    }
    
    return iRet;
}

static LTE_RET_E huawei4G_cerssi_get(LTE_INFO_T *pstLteInfoTmp, int iDataLen)
{
	int iClientSocketFd = -1;
	int iRet = 0;
	char acCmd[] = "at^cerssi?";
	char acCiev[] = "^CERSSI:";
	
	CLIENT_SEND_MSG_T stLteMsg = {0};
	LTE_SEND_MSG_T stLteRecvMsg = {0};

	if (NULL == pstLteInfoTmp)
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
		pstLteInfoTmp->stCellInfo.iRsrp = stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.iRsrp;
		pstLteInfoTmp->stCellInfo.iRsrq = stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.iRsrq;
		pstLteInfoTmp->stCellInfo.iSinr= stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.iSinr;
		
		iRet = LTE_OK;
	}
	else
	{
		iRet = LTE_FAIL;
	}
	
	return iRet;  
}

static LTE_RET_E huawei4G_accessband_get(LTE_INFO_T *pstLteInfoTmp, int iDataLen)
{
	int iClientSocketFd = -1;
	int iRet = 0;
	char acCmd[] = "at";
	char acCiev[] = "OK";
	
	CLIENT_SEND_MSG_T stLteMsg = {0};
	LTE_SEND_MSG_T stLteRecvMsg = {0};

	if (NULL == pstLteInfoTmp)
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
	stLteMsg.iRealFlag = 1;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
	
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
	
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
		pstLteInfoTmp->stCellInfo.iChannel = stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.iChannel;
		strncpy(pstLteInfoTmp->stCellInfo.acBand, stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.acBand, sizeof(pstLteInfoTmp->stCellInfo.acBand));
		
		iRet = LTE_OK;
	}
	else
	{
		iRet = LTE_FAIL;
	}
	
	return iRet;  
}

static LTE_RET_E huawei4G_phyceillid_get(LTE_INFO_T *pstLteInfoTmp, int iDataLen)
{
	int iClientSocketFd = -1;
	int iRet = 0;
	char acCmd[] = "at^cecellid?";
	char acCiev[] = "^CECELLID:";
	//^CECELLID: 46000,25694475,246,6234
	CLIENT_SEND_MSG_T stLteMsg = {0};
	LTE_SEND_MSG_T stLteRecvMsg = {0};

	if (NULL == pstLteInfoTmp)
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
		strncpy(pstLteInfoTmp->stCellInfo.acPhyCeillid, stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.acPhyCeillid, sizeof(pstLteInfoTmp->stCellInfo.acPhyCeillid));
		strncpy(pstLteInfoTmp->stCellInfo.acCellId, stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.acCellId, sizeof(pstLteInfoTmp->stCellInfo.acCellId));
		iRet = LTE_OK;
	}
	else
	{
		iRet = LTE_FAIL;
	}
	
	return iRet;  
}


static LTE_RET_E huawei4G_lte_info_get(void *pvData, int iDataLen)
{
    LTE_CELLID_T stCellid = {0};
	LTE_INFO_T *pstLteInfo = (LTE_INFO_T *)pvData;
	int iSignal = 0;
    
	if (pvData == NULL)
		return LTE_FAIL;
    if(LTE_OK != huawei4G_rssi_get(&iSignal))
    {
        LTE_LOG(LTE_LOG_ERR, "get rssi fail");
    }
    else
    {
        if(iSignal >= 0 && iSignal <= 31)
        {
            pstLteInfo->stCellInfo.iRssi = 2*iSignal - 113;
        }
    }
    if(LTE_OK != huawei4G_cerssi_get(pstLteInfo, sizeof(LTE_INFO_T)))
    {
        LTE_LOG(LTE_LOG_ERR, "get Rsrp Rsrq Sinr fail");
    }
    if(LTE_OK != huawei4G_accessband_get(pstLteInfo, sizeof(LTE_INFO_T)))
    {
        LTE_LOG(LTE_LOG_ERR, "get accessband fail");
    }
    if(LTE_OK != huawei4G_phyceillid_get(pstLteInfo, sizeof(LTE_INFO_T)))
    {
        LTE_LOG(LTE_LOG_ERR, "get phyceillid fail");
    }
  
	return LTE_OK;
}

/*****************************************************************************
 函 数 名  : leadcore_access_net_get
 功能描述  : 获取当前的网络制式
 输入参数  : void *pvData  
             int iDataLen  
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月3日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E huawei4G_access_net_get(void *pvData, int iDataLen)
{
    int iRet = 0;
    int iClientSocketFd = -1;
    char acCmd[] = "at^sysinfoex";
    char acCiev[] = "^SYSINFOEX:";//^SYSINFOEX:2,3,0,1,,6,"LTE",101,"LTE"
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};
    
    if (pvData == NULL)
		return LTE_FAIL;
    
	LTE_ACCESS_NETWORK_T *pstAccessNet = (LTE_ACCESS_NETWORK_T*)pvData;

    iClientSocketFd = lte_client_sock_init();
    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_ACCESS_NET_GET;
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
        pstAccessNet->enAccessNet = stLteRecvMsg.uLteCommand.stAccessNetInfo.enAccessNet;
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;

}
#if 0

static LTE_RET_E leadcore_bands_get(void *pvData, int iDataLen)
{
    /* 38,39,40,41 */
    char support_bands[] = "38,39,40,41";
    LTE_BANDS_T *pstBands = (LTE_BANDS_T *)pvData;

	if (pvData == NULL)
		return LTE_FAIL;

    strncpy(pstBands->bands, support_bands, sizeof(pstBands->bands));
    pstBands->iClcBandAble = CLC_BAND_ENABLE; /* 支持锁频 */
   
    
	return LTE_OK;
}

/*****************************************************************************
 函 数 名  : leadcore_64bit_a_add_b
 功能描述  : 在32位系统下，64位数分为高32位，低32位分别相加
 输入参数  : unsigned long ulHighIn    
             unsigned long ulLowIn     

 输出参数  : unsigned long *ulHighOut  
             unsigned long *ulLowOut   
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月10日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E leadcore_64bit_a_add_b(unsigned long ulHighIn, unsigned long ulLowIn,
    unsigned long *ulHighOut, unsigned long *ulLowOut)
{
    if(NULL == ulHighOut || NULL == ulLowOut)
        return LTE_FAIL;
    
    if( (*ulLowOut >> 31) == 1 && (ulLowIn >> 31) == 1)/*有进位*/
        *ulHighOut = *ulHighOut + 1;
    
    *ulLowOut = *ulLowOut + ulLowIn;
    *ulHighOut = *ulHighOut + ulHighIn;
    return LTE_OK;
}


static LTE_RET_E leadcore_clck_bands_set(void *pvData, int iDataLen)
{
    char acCmd[64] = {0};
    int iRet = 0;
    unsigned long iBandValueHigh = 0;
    unsigned long iBandValueLow = 0;
    LTE_BANDS_T *pstBands = (LTE_BANDS_T *)pvData;

	int iClientSocketFd = -1;
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    /* 判断是否为配置默认band */
    if(0 == strlen(pstBands->bands))
    {
        strcpy(pstBands->bands, LEADCORE_BAND_DEFAULT);
    }
    
    if(0 == strcmp(pstBands->bands, LEADCORE_BAND_DEFAULT))
    {
        iBandValueHigh = LEADCORE_BAND_DEFAULT_VALUE_HIG;
        iBandValueLow = LEADCORE_BAND_DEFAULT_VALUE_LOW;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:band[%s] len[%d]\n",__LINE__,__func__,
            pstBands->bands, strlen(pstBands->bands));
        /* band3处理，只有band3时，pstBands->bands长度小于2 */
        if(strlen(pstBands->bands) < 2)
        {
            iBandValueHigh = LEADCORE_BAND_3_VALUE_HIGH;
            iBandValueLow = LEADCORE_BAND_3_VALUE_LOW;
        }
        else
        {
            if(NULL != strstr(pstBands->bands, "3,"))
            {
                leadcore_64bit_a_add_b(LEADCORE_BAND_3_VALUE_HIGH, LEADCORE_BAND_3_VALUE_LOW,
                    &iBandValueHigh, &iBandValueLow);
            }
            if(NULL != strstr(pstBands->bands, "38"))
            {
                leadcore_64bit_a_add_b(LEADCORE_BAND_38_VALUE_HIGH, LEADCORE_BAND_38_VALUE_LOW,
                    &iBandValueHigh, &iBandValueLow);
            }
            if(NULL != strstr(pstBands->bands, "39"))
            {
                leadcore_64bit_a_add_b(LEADCORE_BAND_39_VALUE_HIGH, LEADCORE_BAND_39_VALUE_LOW,
                    &iBandValueHigh, &iBandValueLow);
            }
            if(NULL != strstr(pstBands->bands, "40"))
            {
                leadcore_64bit_a_add_b(LEADCORE_BAND_40_VALUE_HIGH, LEADCORE_BAND_40_VALUE_LOW,
                    &iBandValueHigh, &iBandValueLow);
            }
            if(NULL != strstr(pstBands->bands, "41"))
            {
                leadcore_64bit_a_add_b(LEADCORE_BAND_41_VALUE_HIGH, LEADCORE_BAND_41_VALUE_LOW,
                    &iBandValueHigh, &iBandValueLow); 
            }
        }
    }
    
    if(0 == iBandValueHigh && 0 == iBandValueLow)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:at this band [%s] not support!\n",__LINE__,__func__, pstBands->bands);
        return LTE_FAIL;
    }
    else
    {
        sprintf(acCmd, "at^bandsetex=7,%08x%08x\r", iBandValueHigh, iBandValueLow);
    }
    
    LTE_LOG(LTE_LOG_INFO,"%d,%s:bandValue [%08x%08x] cmd [%s]\n",__LINE__,__func__, iBandValueHigh,
        iBandValueLow, acCmd);

    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    //LTE_CLCK_BANDS_SET
    stLteMsg.iFuncCmd = LTE_CLCK_BANDS_SET;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, "OK", sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
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


static LTE_RET_E leadcore_bands_close (void *pvData, int iDataLen)
{
    int iRet;
    LTE_BANDS_T pstBands = {0};
    strcpy(pstBands.bands, "default");
    iRet = leadcore_clck_bands_set(&pstBands, sizeof(LTE_BANDS_T));
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:leadcore_clck_bands_set failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    return LTE_OK; 
}
#endif
static LTE_RET_E huawei4G_clckcell_enable_get(void *pvData, int iDataLen)
{
    LTE_CLKCELL_ENABLE_T *pstClkCellEnable = (LTE_CLKCELL_ENABLE_T *)pvData;

    if (pvData == NULL)
    {
		return LTE_FAIL;
    }
    
    /* 不支持锁小区 */
    pstClkCellEnable->iClkCellEnable = 1;

    return LTE_OK;    
}

static LTE_RET_E huawei4G_clck_cellid_set(void *pvData, int iDataLen)
{
    char acCmd[64] = {0};
    int iRet = 0;
    int iPhyCellid = 0;
    char acBandValue[64] = {0};
	int iClientSocketFd = -1;
    char acCiev[] = "OK";
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    LTE_CLK_CELL_T *pstCellClkT = (LTE_CLK_CELL_T *)pvData;
	/*change by zhanghao add lock band mode  2016/7/23*/
    int iFreq ,iBand, iLockMode, iBandStart, iBandEnd;
        
    if (pvData == NULL)
		return LTE_FAIL;
    
    //iPhyCellid = atoi(pstCellClkT->acCellid);
        
    /*if((1 == pstCellClkT->iClkEnable)&&(iPhyCellid > 503))
    {   
        LTE_LOG(LTE_LOG_ERR,"%d,%s:PHY cellid is too large, exceed 503!\n",__LINE__,__func__);
        return LTE_FAIL;
    }*/
    if(0 == pstCellClkT->iLockMode)
    {
        sprintf(acCmd, "AT^ freqlock = 0"); /*解锁小区*/
    }
    else if(1 == pstCellClkT->iLockMode || 2 == pstCellClkT->iLockMode || 3 == pstCellClkT->iLockMode)
    {
    	iLockMode  = pstCellClkT->iLockMode;
		iBandStart = atoi(pstCellClkT->acBandStart);
		iBandEnd   = atoi(pstCellClkT->acBandEnd);
        iFreq      = atoi(pstCellClkT->acFreq);
        iPhyCellid = atoi(pstCellClkT->acCellid);
        #if 0
        if(iFreq >= 17850 && iFreq <= 18050)
        {
            iBand = 62;
            iBandStart=0;
            iBandEnd=0;
        }
        else if(iFreq >= 18800 && iFreq <= 19200)
        {
            iBand = 39;
            iBandStart=0;
            iBandEnd=0;
        }
        else if(iFreq == 0)
        {
            if(iBandStart >= 17850 && iBandEnd <= 18050)
            {
                iBand = 62;
            }
            else if(iBandStart >= 18800 && iBandEnd <= 19200)
            {
                iBand = 39;
            }
            else{
                return LTE_FAIL;
            }

        }
        else 
        {
            return LTE_FAIL;
        }
       #endif
       if( 3 == pstCellClkT->iLockMode )
       {
           sprintf(acCmd, "AT^ freqlock =1,\"04\",%d,,,%d,\"02\"",iFreq,iPhyCellid);
       }
       else
       {
           sprintf(acCmd, "AT^ freqlock =1,\"04\",%d,,,,\"02\"",iFreq);
       }
       
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:in param is error!\n",__LINE__,__func__);
        return LTE_FAIL;
    }

	stLteMsg.iFuncCmd = LTE_CLCK_CELL_SET;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
    stLteMsg.iComReadType = COM_READ_TYPE_1;
    stLteMsg.iComWriteType = COM_WRITE_TYPE_1;
    stLteMsg.iComReturnType = COM_RETURN_SINGLE;
    stLteMsg.iNodeSM = WAIT_WRITE;
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
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

static LTE_RET_E huawei4G_set_oper_info_type(void)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char *pcBuf = NULL;
    
    char acCmd[] = "at+cops=3,2";
    char acCiev[] = "OK";

    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};
        
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_OPER_INFO_GET;
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

/*****************************************************************************
   函 数 名  : leadcore_oper_info_get
   功能描述  : 获取运营商信息
   输入参数  : void *pvData
             int iDataLen
   输出参数  : 无
   返 回 值  : static
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2014年11月10日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E huawei4G_oper_info_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cops?";
    char acCiev[] = "+COPS:";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    iRet = huawei4G_set_oper_info_type();
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s: huawei4G_oper_info_get failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    LTE_OPER_INFO_T *pstOperInfo = (LTE_OPER_INFO_T *)pvData;
    
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_OPER_INFO_GET;
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
        strncpy(pstOperInfo->acOperName, stLteRecvMsg.uLteCommand.stOperInfo.acOperName, sizeof(pstOperInfo->acOperName));
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;

}

static LTE_RET_E huawei4G_sup_nettype_get(void *pvData, int iDataLen)
{
    LTE_NET_TYPE_T *pstNets = (LTE_NET_TYPE_T *)pvData;

    pstNets->enNetType = LTE_4G_ONLY;
    pstNets->enNetTypePri = LTE_NONE;
    return LTE_OK;

}
#if 0

/*****************************************************************************
   函 数 名  : leadcore_sms_at_csms
   功能描述  : 初始化消息的服务类型
   输入参数  : void
   输出参数  : 无
   返 回 值  : static
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2014年11月13日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E leadcore_sms_at_csms(void)
{
    int iRet = 0;    
    int iClientSocketFd = -1;
    char acCmd[] = "at+csms=0";
    char acCiev[] = "+CSMS:";
    char *pcBuf = NULL;
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};
    
    iClientSocketFd = lte_client_sock_init();
    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_SMS_FUNC_INIT;
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
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;
}

/*****************************************************************************
   函 数 名  : leadcore_sms_at_cpms
   功能描述  : 初始化消息的存储方式
   输入参数  : void
   输出参数  : 无
   返 回 值  : static
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2014年11月13日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E leadcore_sms_at_cpms(void)
{
    int iRet = 0;    
    int iClientSocketFd = -1;
    char acCmd[] = "at+cpms=\"SM\",\"SM\",\"SM\"";
    char acCiev[] = "+CPMS:";
    char *pcBuf = NULL;
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};
    
    iClientSocketFd = lte_client_sock_init();
    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_SMS_FUNC_INIT;
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
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;

}

/*****************************************************************************
   函 数 名  : leadcore_sms_at_cnmi
   功能描述  : 初始化新短信通知
   输入参数  : void
   输出参数  : 无
   返 回 值  : static
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2014年11月13日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E leadcore_sms_at_cnmi(void)
{
    int iRet = 0;    
    int iClientSocketFd = -1;
    char acCmd[] = "at+cnmi=2,1,2,2,0";
    char acCiev[] = "OK";
    char *pcBuf = NULL;
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};
    
    iClientSocketFd = lte_client_sock_init();
    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_SMS_FUNC_INIT;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
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

/*****************************************************************************
   函 数 名  : leadcore_sms_at_cgsms
   功能描述  : 初始化消息承载域
   输入参数  : void
   输出参数  : 无
   返 回 值  : static
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2014年11月13日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E leadcore_sms_at_cgsms(void)
{
    int iRet = 0;    
    int iClientSocketFd = -1;
    char acCmd[] = "at+cgsms=1";
    char acCiev[] = "OK";
    char *pcBuf = NULL;
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};
    
    iClientSocketFd = lte_client_sock_init();
    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_SMS_FUNC_INIT;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
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


/*****************************************************************************
   函 数 名  : leadcore_sms_mode_set
   功能描述  : 设置短信模式，0表示pdu模式，1表示文本模式
   输入参数  : void *pvData
             int iDataLen
   输出参数  : 无
   返 回 值  : static
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2014年11月18日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E leadcore_sms_mode_set(void *pvData, int iDataLen)
{
    int iRet = 0;    
    int iClientSocketFd = -1;
    char acCmd[64] = {0};
    char acCiev[] = "OK";
    char *pcBuf = NULL;
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};
    if (NULL == pvData)
    {
        return LTE_FAIL;
    }

    LTE_SMS_Mode_T *pstMode = (LTE_SMS_Mode_T *)pvData;

    if (pstMode->iSmsMode < 0 || pstMode->iSmsMode > 1)
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:mode error\n", __LINE__, __func__);
        return LTE_FAIL;
    }
    
    iClientSocketFd = lte_client_sock_init();
    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    sprintf(acCmd, "at+cmgf=%d", pstMode->iSmsMode);
    stLteMsg.iFuncCmd = LTE_SMS_MODE__SET;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
    strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
    strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
    LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
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

/*****************************************************************************
   函 数 名  : leadcore_sms_init
   功能描述  : 短信功能初始化接口
   输入参数  : void
   输出参数  : 无
   返 回 值  : static
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2014年11月13日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E leadcore_sms_func_init(void *pvData, int iDataLen)
{
    int iRet = 0;
    LTE_SMS_Mode_T pstMode = {0};
    pstMode.iSmsMode = 0;/* 默认设为pdu模式 */
    iRet = leadcore_sms_at_csms();
    if (iRet != LTE_OK)
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:leadcore_sms_at_csms failed\n", __LINE__, __func__);
        return LTE_FAIL;
    }
    iRet = leadcore_sms_at_cpms();
    if (iRet != LTE_OK)
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:leadcore_sms_at_cpms failed\n", __LINE__, __func__);
        return LTE_FAIL;
    }
    iRet = leadcore_sms_at_cnmi();
    if (iRet != LTE_OK)
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:leadcore_sms_at_cnmi failed\n", __LINE__, __func__);
        return LTE_FAIL;
    }
    iRet = leadcore_sms_at_cgsms();
    if (iRet != LTE_OK)
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:leadcore_sms_at_cgsms failed\n", __LINE__, __func__);
        return LTE_FAIL;
    }
    iRet = leadcore_sms_mode_set(&pstMode, sizeof(LTE_SMS_Mode_T));
    if (iRet != LTE_OK)
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:leadcore_sms_mode_set failed\n", __LINE__, __func__);
        return LTE_FAIL;
    }

    return LTE_OK;

}

/*****************************************************************************
   函 数 名  : leadcore_cent_code_set
   功能描述  : 设置短信中心号
   输入参数  : void *pvData
             int iDataLen
   输出参数  : 无
   返 回 值  : static
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2014年11月13日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E leadcore_sms_cent_code_set(void *pvData, int iDataLen)
{
    int iRet = 0;
    int iClientSocketFd = -1;
    char acCmd[64] = {0};
    char acCiev[] = "OK";
    char *pcBuf = NULL;
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};
    if (NULL == pvData)
    {
        return LTE_FAIL;
    }

    LTE_SMS_CENT_CODE_T *pstCentCode = (LTE_SMS_CENT_CODE_T*)pvData;

    iClientSocketFd = lte_client_sock_init();
    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    sprintf(acCmd, "at+csca=\"%s\",145\r", pstCentCode->acSmsCentCode);

    stLteMsg.iFuncCmd = LTE_SMS_CENT_CODE_SET;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
    strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
    strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
    LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
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

/*****************************************************************************
   函 数 名  : leadcore_sms_recv
   功能描述  : 按pdu模式读取短信
   输入参数  : void *pvData
             int iDataLen
   输出参数  : 无
   返 回 值  : static
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2014年11月26日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E leadcore_sms_recv(void *pvData, int iDataLen)
{
    int iRet = 0;
    int iClientSocketFd = -1;
    char acCmd[64] = {0};
    //char acCiev[] = "+CMGL:";
    char acCiev[16] = {0};
    char *pcBuf = NULL;
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};
    
    if (NULL == pvData)
    {
        return LTE_FAIL;
    }

    LTE_SMS_RECV_T *pstRecvInfo = (LTE_SMS_RECV_T*)pvData;
    
    iClientSocketFd = lte_client_sock_init();
    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }

    if (0 == pstRecvInfo->iRecvFlag)
    {
        sprintf(acCmd, "at+cmgl=4\r");
        stLteMsg.iComReadType = COM_READ_TYPE_6;
        stLteMsg.iComReturnType = COM_RETURN_MUTIL;
        strncpy(acCiev, "+CMGL:", sizeof(acCiev));
    }
    else if (pstRecvInfo->iRecvFlag >= 1 && pstRecvInfo->iRecvFlag <= 50)
    {
        sprintf(acCmd, "at+cmgr=%d\r", pstRecvInfo->iRecvFlag);
        stLteMsg.iComReadType = COM_READ_TYPE_6;
        stLteMsg.iComReturnType = COM_RETURN_SINGLE;
        strncpy(acCiev, "+CMGR:", sizeof(acCiev));
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:pstRecvInfo->iRecvFlag  is not 0~50 \n", __LINE__, __func__);
        return LTE_FAIL;
    }

    stLteMsg.iFuncCmd = LTE_SMS_RECV;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
    
    //stLteMsg.iComReadType = COM_READ_TYPE_1;
    stLteMsg.iComWriteType = COM_WRITE_TYPE_1;
    //stLteMsg.iComReturnType = COM_RETURN_SINGLE;
    stLteMsg.iNodeSM = WAIT_WRITE;
    strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
    strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
    LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);

    iRet = lte_client_send(iClientSocketFd, &stLteMsg);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_send failed!\n",__LINE__,__func__);
        close(iClientSocketFd);
        return LTE_FAIL;
    }

    iRet = lte_client_recv(iClientSocketFd, &stLteRecvMsg, 4);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_recv failed!\n",__LINE__,__func__);
        close(iClientSocketFd);
        return LTE_FAIL;
    }
    
    close(iClientSocketFd);
    LTE_LOG(LTE_LOG_INFO,"########################:result:%d[0:fail;1:success]\n", stLteRecvMsg.iResult);

    if(1 != stLteRecvMsg.iResult)
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:sms_recv read failed!\n", __LINE__, __func__);
        return LTE_FAIL;
    }
    else
    {
        if (0 == pstRecvInfo->iRecvFlag)  /* 读取所有短信 */
        {
            iRet = sms_recv(LTE_SMS_RECV_FILE, pstRecvInfo->stRecvCont, sizeof(pstRecvInfo->stRecvCont)/sizeof(SMS_INFO));
            if (PDU_E_NONE != iRet)
            {
                LTE_LOG(LTE_LOG_ERR, "%d,%s:sms_recv read all sms failed\n", __LINE__, __func__);
                return LTE_FAIL;
            }
        }
        else
        {
            iRet = sms_recv(LTE_SMS_RECV_FILE, pstRecvInfo->stRecvCont, 1);
            if (PDU_E_NONE != iRet)
            {
                LTE_LOG(LTE_LOG_ERR, "%d,%s:sms_recv read one sms failed\n", __LINE__, __func__);
                return LTE_FAIL;
            }
        }
    }
    lte_file_unlink(LTE_SMS_RECV_FILE);
    return LTE_OK;
   
}

/*****************************************************************************
   函 数 名  : leadcore_sms_send
   功能描述  : 发送短信
   输入参数  : void *pvData
             int iDataLen
   输出参数  : 无
   返 回 值  : static
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2014年11月18日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E leadcore_sms_send(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iSmsFd;
    char acCmd[64] = {0};
    char acCiev[] = "OK";
    char *pcBuf = NULL;
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};
    
    int iRet, iLoop;
    int iSmsLen = 0;
    int iSmsNum = 0;
    int iWriteLen = 0;
    char acTmp[PDU_LONG_SMS_NUM][SMS_SEND_BUFFER_MAX] = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }

    LTE_SMS_SEND_T *pstSendInfo = (LTE_SMS_SEND_T *)pvData;
    
    LTE_NORMAL_AT_T stAt = {"", "/tmp/sendinfo", "OK", 0, ATCOM, 0, 2};
    
    /*iRet = pdu_encode_sms(pstSendInfo->acRecvMsgCode, strlen(pstSendInfo->acRecvMsgCode), pstSendInfo->acMsgCont, 
        strlen(pstSendInfo->acMsgCont), acTmp, SMS_SEND_BUFFER_MAX, &iSmsNum );*/
    iRet = pdu_encode_sms(pstSendInfo, acTmp, SMS_SEND_BUFFER_MAX, &iSmsNum );
    if(PDU_E_NONE != iRet)
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s: pdu_encode_sms failed\n", __LINE__, __func__);
        return LTE_FAIL;
    }

    iClientSocketFd = lte_client_sock_init();
    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }

    stLteMsg.iFuncCmd = LTE_SMS_SEND;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
    strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
    LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
    stLteMsg.iComReadType = COM_READ_TYPE_1;
    stLteMsg.iComWriteType = COM_WRITE_TYPE_2;
    stLteMsg.iComReturnType = COM_RETURN_SINGLE;
    stLteMsg.iNodeSM = WAIT_WRITE;

    for(iLoop=0; iLoop<iSmsNum; iLoop++)
    {
        iSmsLen = strlen(acTmp[iLoop])/2 -1;
        sprintf(acCmd, "at+cmgs=%d\n", iSmsLen);

        iSmsFd = open(LTE_SMS_SEND_FILE, O_CREAT|O_TRUNC|O_WRONLY);
        if(iSmsFd < 0)
        {
            LTE_LOG(LTE_LOG_ERR, "open %s failed", LTE_SMS_SEND_FILE);
            return LTE_FAIL;
        }
        iWriteLen = write(iSmsFd, acCmd, strlen(acCmd));
        iWriteLen = write(iSmsFd, acTmp[iLoop], strlen(acTmp[iLoop]));
        close(iSmsFd);
        
        strncpy(stLteMsg.acAtCmd, LTE_SMS_SEND_FILE, sizeof(stLteMsg.acAtCmd));

        iRet = lte_client_send(iClientSocketFd, &stLteMsg);
        if(LTE_OK != iRet)
        {
            LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_send failed!\n",__LINE__,__func__);
            close(iClientSocketFd);
            return LTE_FAIL;
        }

        iRet = lte_client_recv(iClientSocketFd, &stLteRecvMsg, 4);
        if(LTE_OK != iRet)
        {
            LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_recv failed!\n",__LINE__,__func__);
            close(iClientSocketFd);
            return LTE_FAIL;
        }
        LTE_LOG(LTE_LOG_INFO,"########################:result:%d[0:fail;1:success]\n", stLteRecvMsg.iResult);
        if(1 != stLteRecvMsg.iResult)
        {   
            close(iClientSocketFd);
            return LTE_FAIL;
        }
    }

    close(iClientSocketFd);  
    return LTE_OK;
}


/*****************************************************************************
   函 数 名  : leadcore_sms_dele
   功能描述  :  短信删除接口
   输入参数  : void *pvData
             int iDataLen
   输出参数  : 无
   返 回 值  : static
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2014年11月13日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E leadcore_sms_dele(void *pvData, int iDataLen)
{
    int iRet = 0;    
    int iClientSocketFd = -1;
    char acCmd[64] = {0};
    char acCiev[] = "OK";
    char *pcBuf = NULL;
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};
    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_SMS_DEL_T *pstDeleSms = (LTE_SMS_DEL_T*)pvData;

    if (0 == pstDeleSms->iDelIndex)
    {
        sprintf(acCmd, "at+cmgd=1,4\r" );
    }
    else if (pstDeleSms->iDelIndex >= 1 && pstDeleSms->iDelIndex <= 50)
    {
        sprintf(acCmd, "at+cmgd=%d\r", pstDeleSms->iDelIndex-1 );
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s pstDeleSms->iDelIndex is not 0~50 \n", __LINE__, __func__);
        return LTE_FAIL;
    }
    
    iClientSocketFd = lte_client_sock_init();
    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_SMS_DELE;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
    strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
    strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
    LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
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


static LTE_RET_E leadcore_sms_sup_func(void *pvData, int iDataLen)
{
    int flag = 0;
    LTE_SUPPORT_SMS_T *pstSup = (LTE_SUPPORT_SMS_T *)pvData;
    pstSup->enSupSms = SUP_SMS_FUNC;
    return LTE_OK;

}

static LTE_RET_E leadcore_set_ipv6_prepare(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at^dipv6sa=1";
    char acCiev[] = "OK";
    char *pcBuf = NULL;

    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_IPV6_PDP_SET;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
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


static LTE_RET_E leadcore_cgdcont_set(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[64] = {0};
    char acCiev[] = "OK";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_CGDCONT_T *pstPdpSet = (LTE_CGDCONT_T*)pvData;

    if(0 != pstPdpSet->iApnFlag)
    {
        if(PDP_IPV4 == pstPdpSet->enPdpStack)
        {
            snprintf(acCmd, sizeof(acCmd), "at^ddpdn=1,\"%s\"", pstPdpSet->acApn);
        }
        else if(PDP_IPV6 == pstPdpSet->enPdpStack)
        {
            snprintf(acCmd, sizeof(acCmd), "at^ddpdn=2,\"%s\"", pstPdpSet->acApn);
        }
        else if(PDP_IPV4V6 == pstPdpSet->enPdpStack)
        {
            snprintf(acCmd, sizeof(acCmd), "at^ddpdn=3,\"%s\"", pstPdpSet->acApn);
        }
    }
    else
    {
        if(PDP_IPV4 == pstPdpSet->enPdpStack)
        {
            snprintf(acCmd, sizeof(acCmd), "at^ddpdn=1,\"\"");
        }
        else if(PDP_IPV6 == pstPdpSet->enPdpStack)
        {
            snprintf(acCmd, sizeof(acCmd), "at^ddpdn=2,\"\"");
        }
        else if(PDP_IPV4V6 == pstPdpSet->enPdpStack)
        {
            snprintf(acCmd, sizeof(acCmd), "at^ddpdn=3,\"\"");
        }
    }

    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_IPV6_PDP_SET;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
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
/*****************************************************************************
   函 数 名  : leadcore_apn_set
   功能描述  : 设置apn
   输入参数  : void *pvData
             int iDataLen
   输出参数  : 无
   返 回 值  : static
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年6月25日
    作    者   : wangqi
    修改内容   : 新生成函数

*****************************************************************************/

static LTE_RET_E leadcore_cgdcont_2g3g_set(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[64] = {0};
    char acCiev[] = "OK";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_CGDCONT_T *pstPdpSet = (LTE_CGDCONT_T*)pvData;

    if(0 != pstPdpSet->iApnFlag)
    {
        if(PDP_IPV4 == pstPdpSet->enPdpStack)
        {
            snprintf(acCmd, sizeof(acCmd), "at+cgdcont=1,\"IP\",\"%s\"", pstPdpSet->acApn);
        }
        else if(PDP_IPV6 == pstPdpSet->enPdpStack)
        {
            snprintf(acCmd, sizeof(acCmd), "at+cgdcont=1,\"IPV6\",\"%s\"", pstPdpSet->acApn);
        }
        else if(PDP_IPV4V6 == pstPdpSet->enPdpStack)
        {
            snprintf(acCmd, sizeof(acCmd), "at+cgdcont=1,\"IPV4V6\",\"%s\"", pstPdpSet->acApn);
        }
    }
    else
    {
        if(PDP_IPV4 == pstPdpSet->enPdpStack)
        {
            snprintf(acCmd, sizeof(acCmd), "at+cgdcont=1,\"IP\"");
        }
        else if(PDP_IPV6 == pstPdpSet->enPdpStack)
        {
            snprintf(acCmd, sizeof(acCmd), "at+cgdcont=1,\"IPV6\"");
        }
        else if(PDP_IPV4V6 == pstPdpSet->enPdpStack)
        {
            snprintf(acCmd, sizeof(acCmd), "at+cgdcont=1,\"IPV4V6\"");
        }
    }

    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_IPV6_PDP_SET;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
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
#endif

/*****************************************************************************
   函 数 名  : leadcore_apn_set
   功能描述  : 设置apn
   输入参数  : void *pvData
             int iDataLen
   输出参数  : 无
   返 回 值  : static
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2014年11月17日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E huawei4G_apn_set(void *pvData, int iDataLen)
{
	int iClientSocketFd = -1;
	int iRet = 0;
	char acCmd[64] = {0};
	char acCiev[] = "OK";
	char *pcBuf = NULL;

	CLIENT_SEND_MSG_T stLteMsg = {0};
	LTE_SEND_MSG_T stLteRecvMsg = {0};

	if (NULL == pvData)
	{
	   return LTE_FAIL;
	}
	LTE_APN_T *pstApn = (LTE_APN_T*)pvData;
	
	snprintf(acCmd, sizeof(acCmd), "at+cgdcont=0,\"IP\",\"%s\"", pstApn->acApn);

	iClientSocketFd = lte_client_sock_init();

	if(iClientSocketFd < 0)
	{
	   LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
	   return LTE_FAIL;
	}

	stLteMsg.iFuncCmd = LTE_APN_SET;
	stLteMsg.iPriority = 0;
	stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));

	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);

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

static LTE_RET_E huawei4G_version_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cgmr";
    char acCiev[] = "OK";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_MODULE_VERSION_T *pstVersion = (LTE_MODULE_VERSION_T *)pvData;
    
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_VERSION_GET;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 1;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
    stLteMsg.iComReadType = COM_READ_TYPE_5;
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
        strncpy(pstVersion->acVersion, stLteRecvMsg.uLteCommand.stVersionInfo.acVersion, sizeof(pstVersion->acVersion));
        pstVersion->iVersionIndex = stLteRecvMsg.uLteCommand.stVersionInfo.iVersionIndex;
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;

}

static LTE_RET_E huawei4G_enodeb_id_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cereg?";
    char acCiev[] = "+CEREG:";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    iRet = huawei4G_set_cereg_return_type(2, LTE_ENODEBID_GET);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s: huawei4G_set_cereg_return_type [2] failed\n",__LINE__,__func__);
        iRet = LTE_FAIL;
        goto fail;
    }
    
    LTE_ENODB_ID_T *pstEnodebId = (LTE_ENODB_ID_T *)pvData;
    
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        iRet = LTE_FAIL;
        goto fail;
    }
    
    stLteMsg.iFuncCmd = LTE_ENODEBID_GET;
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
        iRet = LTE_FAIL;
        goto fail;
    }

    iRet = lte_client_recv(iClientSocketFd, &stLteRecvMsg, 2);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_recv failed!\n",__LINE__,__func__);
        close(iClientSocketFd);
        iRet = LTE_FAIL;
        goto fail;
    }
    
    close(iClientSocketFd);
    LTE_LOG(LTE_LOG_INFO,"########################:result:%d[0:fail;1:success]\n", stLteRecvMsg.iResult);

    if(1 == stLteRecvMsg.iResult)
    {
        strncpy(pstEnodebId->acEnodebId, stLteRecvMsg.uLteCommand.stEnodeBInfo.acEnodebId, sizeof(pstEnodebId->acEnodebId));
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
fail:
    if(LTE_OK != huawei4G_set_cereg_return_type(0, LTE_ENODEBID_GET) )
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s: huawei4G_set_cereg_return_type [0] failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    return iRet;

}

/*****************************************************************************
 函 数 名  : huawei4G_tac_get
 功能描述  : 获取小区所属的区位跟踪码
 输入参数  : void *pvData  
             int iDataLen  
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年2月4日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E huawei4G_tac_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cereg?";
    char acCiev[] = "+CEREG:";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    iRet = huawei4G_set_cereg_return_type(2, LTE_TAC_GET);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s: huawei4G_set_cereg_return_type [2] failed\n",__LINE__,__func__);
        iRet = LTE_FAIL;
        goto fail;
    }
    
    LTE_TAC_T *pstTac = (LTE_TAC_T *)pvData;
    
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        iRet = LTE_FAIL;
        goto fail;
    }
    
    stLteMsg.iFuncCmd = LTE_TAC_GET;
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
        iRet = LTE_FAIL;
        goto fail;
    }

    iRet = lte_client_recv(iClientSocketFd, &stLteRecvMsg, 2);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_recv failed!\n",__LINE__,__func__);
        close(iClientSocketFd);
        iRet = LTE_FAIL;
        goto fail;
    }
    
    close(iClientSocketFd);
    LTE_LOG(LTE_LOG_INFO,"########################:result:%d[0:fail;1:success]\n", stLteRecvMsg.iResult);

    if(1 == stLteRecvMsg.iResult)
    {
        strncpy(pstTac->acTac, stLteRecvMsg.uLteCommand.stTacInfo.acTac, sizeof(pstTac->acTac));
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
fail:
    if(LTE_OK != huawei4G_set_cereg_return_type(0, LTE_TAC_GET) )
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s: huawei4G_set_cereg_return_type [0] failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    return iRet;
}

static LTE_RET_E huawei4G_lcid_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cereg?";
    char acCiev[] = "+CEREG:";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    iRet = huawei4G_set_cereg_return_type(2, LTE_LCID_GET);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s: huawei4G_set_cereg_return_type [2] failed\n",__LINE__,__func__);
        iRet = LTE_FAIL;
        goto fail;
    }
    
    LTE_LCID_T *pstLcid = (LTE_LCID_T *)pvData;
    
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        iRet = LTE_FAIL;
        goto fail;
    }
    
    stLteMsg.iFuncCmd = LTE_LCID_GET;
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
        iRet = LTE_FAIL;
        goto fail;
    }

    iRet = lte_client_recv(iClientSocketFd, &stLteRecvMsg, 2);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_recv failed!\n",__LINE__,__func__);
        close(iClientSocketFd);
        iRet = LTE_FAIL;
        goto fail;
    }
    
    close(iClientSocketFd);
    LTE_LOG(LTE_LOG_INFO,"########################:result:%d[0:fail;1:success]\n", stLteRecvMsg.iResult);

    if(1 == stLteRecvMsg.iResult)
    {
        strncpy(pstLcid->acLcid, stLteRecvMsg.uLteCommand.stLcidInfo.acLcid, sizeof(pstLcid->acLcid));
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
fail:
    if(LTE_OK != huawei4G_set_cereg_return_type(0, LTE_LCID_GET) )
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s: huawei4G_set_cereg_return_type [0] failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    return iRet;
}
#if 0
static LTE_RET_E huawei4G_all_cellid_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cereg?";
    char acCiev[] = "+CEREG:";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    iRet = huawei4G_set_cereg_return_type(2, LTE_ALL_CELLID_GET);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s: leadcore_set_cereg_return_type [2] failed\n",__LINE__,__func__);
        iRet = LTE_FAIL;
        goto fail;
    }
    
    LTE_ALL_CELLID_T *pstAllcid = (LTE_ALL_CELLID_T *)pvData;
    
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        iRet = LTE_FAIL;
        goto fail;
    }
    
    stLteMsg.iFuncCmd = LTE_ALL_CELLID_GET;
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
        iRet = LTE_FAIL;
        goto fail;
    }

    iRet = lte_client_recv(iClientSocketFd, &stLteRecvMsg, 2);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_recv failed!\n",__LINE__,__func__);
        close(iClientSocketFd);
        iRet = LTE_FAIL;
        goto fail;
    }
    
    close(iClientSocketFd);
    LTE_LOG(LTE_LOG_INFO,"########################:result:%d[0:fail;1:success]\n", stLteRecvMsg.iResult);

    if(1 == stLteRecvMsg.iResult)
    {
        strncpy(pstAllcid->acCellId, stLteRecvMsg.uLteCommand.stAllcidInfo.acCellId, sizeof(pstAllcid->acCellId));
        strncpy(pstAllcid->acEnodebId, stLteRecvMsg.uLteCommand.stAllcidInfo.acEnodebId, sizeof(pstAllcid->acEnodebId));
        strncpy(pstAllcid->acLcid, stLteRecvMsg.uLteCommand.stAllcidInfo.acLcid, sizeof(pstAllcid->acLcid));
        strncpy(pstAllcid->acTac, stLteRecvMsg.uLteCommand.stAllcidInfo.acTac, sizeof(pstAllcid->acTac));

        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
fail:

    if(LTE_OK != huawei4G_set_cereg_return_type(0, LTE_ALL_CELLID_GET) )
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s: leadcore_set_cereg_return_type [0] failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    return iRet;
}
#else
static LTE_RET_E huawei4G_all_cellid_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at^cecellid?";
    char acCiev[] = "^CECELLID:";
    char *pcBuf = NULL;
    //^CECELLID: 46000,25694475,246,6234
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_ALL_CELLID_T *pstAllcid = (LTE_ALL_CELLID_T *)pvData;
    
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_ALL_CELLID_GET;
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
        strncpy(pstAllcid->acCellId, stLteRecvMsg.uLteCommand.stAllcidInfo.acCellId, sizeof(pstAllcid->acCellId));
        strncpy(pstAllcid->acEnodebId, stLteRecvMsg.uLteCommand.stAllcidInfo.acEnodebId, sizeof(pstAllcid->acEnodebId));
        strncpy(pstAllcid->acLcid, stLteRecvMsg.uLteCommand.stAllcidInfo.acLcid, sizeof(pstAllcid->acLcid));
        strncpy(pstAllcid->acTac, stLteRecvMsg.uLteCommand.stAllcidInfo.acTac, sizeof(pstAllcid->acTac));

        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;
}

#endif

#if 0
static LTE_RET_E leadcore_mac_set(void *pvData, int iDataLen)
{
	int iClientSocketFd = -1;
	int iRet = 0;
	char acCmd[64] = {0};
	char acCiev[] = "OK";

	CLIENT_SEND_MSG_T stLteMsg = {0};
	LTE_SEND_MSG_T stLteRecvMsg = {0};

	if (NULL == pvData)
	{
	   return LTE_FAIL;
	}
	LTE_MOUDLE_MAC_T *pstMac = (LTE_MOUDLE_MAC_T* )pvData;
	
	snprintf(acCmd, sizeof(acCmd), "at^qmac=\"%s\"", pstMac->acMac);

	iClientSocketFd = lte_client_sock_init();

	if(iClientSocketFd < 0)
	{
	   LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
	   return LTE_FAIL;
	}

	stLteMsg.iFuncCmd = LTE_MOUDLE_MAC_SET;
	stLteMsg.iPriority = 0;
	stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));

	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);

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


static LTE_RET_E leadcore_mac_get(void *pvData, int iDataLen)
{
	int iClientSocketFd = -1;
	int iRet = 0;
	char acCmd[] = "at^qmac?";
	char acCiev[] = "^QMAC:";

	CLIENT_SEND_MSG_T stLteMsg = {0};
	LTE_SEND_MSG_T stLteRecvMsg = {0};

	if (NULL == pvData)
	{
	   return LTE_FAIL;
	}
	LTE_MOUDLE_MAC_T *pstMac = (LTE_MOUDLE_MAC_T* )pvData;
	
	iClientSocketFd = lte_client_sock_init();

	if(iClientSocketFd < 0)
	{
	   LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
	   return LTE_FAIL;
	}

	stLteMsg.iFuncCmd = LTE_MOUDLE_MAC_GET;
	stLteMsg.iPriority = 0;
	stLteMsg.iRealFlag = 1;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));

	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);

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
       strncpy(pstMac->acMac, stLteRecvMsg.uLteCommand.stMacInfo.acMac, sizeof(pstMac->acMac));
	   iRet = LTE_OK;
	}
	else
	{
	   iRet = LTE_FAIL;
	}

	return iRet;
}

static LTE_RET_E recv_only_ok(char *pcCmd, LTE_CMD_E iFuncCmd, int iRealFlag)
{
	int iClientSocketFd = -1;
	int iRet = 0;
	//char acCmd[] = "";
	char acCiev[] = "OK";

	CLIENT_SEND_MSG_T stLteMsg = {0};
	LTE_SEND_MSG_T stLteRecvMsg = {0};

	if (NULL == pcCmd)
	{
	   return LTE_FAIL;
	}
	
	iClientSocketFd = lte_client_sock_init();

	if(iClientSocketFd < 0)
	{
	   LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
	   return LTE_FAIL;
	}

	stLteMsg.iFuncCmd = iFuncCmd;
	stLteMsg.iPriority = 0;
	stLteMsg.iRealFlag = iRealFlag;
	strncpy(stLteMsg.acAtCmd, pcCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);

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

static LTE_RET_E leadcore_sim_unlock(void *pvData, int iDataLen)
{
    char acDautype[] = "at^dautype=2";
    char acDsimkey[256] = {0};//"at^dsimkey=1,12,1,D76286A16AFD3030935E594B55EBEA961AC150D72B2F64512BF70CA7CD30ABF356ED803C0B9822F627374FDF8E14247769E472C715B2974BAA725A64E57A0E9EEED682DAFBEFE92DE4B0155DF1DC3DF6";

    if(NULL == pvData)
    {
        return LTE_FAIL;
    }
    LTE_SIM_SECRET_KEY_T *pstSecretKey = (LTE_SIM_SECRET_KEY_T *)pvData;
    if(strlen(pstSecretKey->acSecretKey) != 160)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:pstSecretKey->acSecretKey length failed!\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    if(LTE_OK != recv_only_ok(acDautype, LTE_SIM_UNLOCK, 0))
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_recv failed!\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    sprintf(acDsimkey, "at^dsimkey=1,%d,%d,%s", pstSecretKey->man_id, pstSecretKey->key_ver, pstSecretKey->acSecretKey);
    if(LTE_OK != recv_only_ok(acDsimkey, LTE_SIM_UNLOCK, 0))
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_recv failed!\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    return LTE_OK;
}

static LTE_RET_E leadcore_sim_disable_unlock(void *pvData, int iDataLen)
{
    char acDautype[] = "at^dautype=0";
    char acDsimkey[] = "at^dsimkey=0";

    if(LTE_OK != recv_only_ok(acDautype, LTE_SIM_DISABLE_UNLOCK, 0))
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_recv failed!\n",__LINE__,__func__);
        return LTE_FAIL;
    }

    if(LTE_OK != recv_only_ok(acDsimkey, LTE_SIM_DISABLE_UNLOCK, 0))
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_recv failed!\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    return LTE_OK;
}

static LTE_RET_E leadcore_sim_unlock_support(void *pvData, int iDataLen)
{
    LTE_UNLOCK_SUPPORT_T *pstUnlockSupport = (LTE_UNLOCK_SUPPORT_T* )pvData;
    if(NULL == pstUnlockSupport || iDataLen < sizeof(LTE_UNLOCK_SUPPORT_T))
    {
        return LTE_FAIL;
    }
    pstUnlockSupport->iUnlockFlag = 1;
    return LTE_OK;
}
static LTE_RET_E leadcore_sim_type_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "AT^USATDEBUG";
    char acCiev[] = "AABB00CCDD";
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_SIM_TYPE_E *penSimType = (LTE_SIM_TYPE_E *)pvData;
    
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_SIM_TYPE_GET;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 1;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
    stLteMsg.iComReadType = COM_READ_TYPE_7;
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
        *penSimType = stLteRecvMsg.uLteCommand.stSimRandomNum.iSIMType;
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;
}

static LTE_RET_E leadcore_sim_unlock_get(void *pvData, int iDataLen)
{
    char acCmd[] = "at";
    int iClientSocketFd = -1;
	int iRet = 0;
	//char acCmd[] = "";
	char acCiev[] = "OK";

	CLIENT_SEND_MSG_T stLteMsg = {0};
	LTE_SEND_MSG_T stLteRecvMsg = {0};

	if(pvData == NULL)
	{
        return LTE_FAIL;
	}
	LTE_SIM_UNLOCK_STATE_T *pstSimUnlockState = (LTE_SIM_UNLOCK_STATE_T *)pvData;
	iClientSocketFd = lte_client_sock_init();

	if(iClientSocketFd < 0)
	{
	   LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
	   return LTE_FAIL;
	}

	stLteMsg.iFuncCmd = LTE_SIM_UNLOCK_GET;
	stLteMsg.iPriority = 0;
	stLteMsg.iRealFlag = 1;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);

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
	   *pstSimUnlockState = stLteRecvMsg.uLteCommand.stSimUnlockState;
	   iRet = LTE_OK;
	}
	else
	{
	   iRet = LTE_FAIL;
	}

	return iRet;
}

static LTE_RET_E leadcore_all_plmn_mode_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cops=?";
    char acCiev[] = "+COPS:";
    char *pcBuf = NULL;
    int iCount = 0;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_ALL_PLMN_MODE_T *pstAllPlmnMode = (LTE_ALL_PLMN_MODE_T *)pvData;
    for(iCount=0; iCount<5; iCount++)
    {
        if (LTE_OK == leadcore_power_off(NULL, 0))
        {
            break;
        }
        sleep(1);
    }
    sleep(1);
    for(iCount=0; iCount<5; iCount++)
    {
        if (LTE_OK == leadcore_power_on(NULL, 0))
        {
            break;
        }
        sleep(1);
    }
    
    for(iCount=0; iCount<60; iCount++)
    {
        if (LTE_OK == leadcore_network_prepare(NULL, 0))
        {
            break;
        }
        sleep(1);
    }
    
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_ALL_PLMN_MODE_GET;
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

    iRet = lte_client_recv(iClientSocketFd, &stLteRecvMsg, 150);
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
        strncpy(pstAllPlmnMode->acAllPlmn, stLteRecvMsg.uLteCommand.stAllPlmnMode.acAllPlmn, sizeof(pstAllPlmnMode->acAllPlmn));
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;
}


static LTE_RET_E leadcore_sounet_mode_set(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[64] = {0};
    char acCiev[] = "OK";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};
    LTE_NET_TYPE_T stNetType = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_SOUNET_MODE_T *pstSounetMode = (LTE_SOUNET_MODE_T *)pvData;

    if(0 == strcmp(pstSounetMode->acMode, "4G"))
    {
        stNetType.enNetType = LTE_4G_ONLY;
        snprintf(acCmd, sizeof(acCmd), "at+cops=1,2,\"%s\",7", pstSounetMode->acPlmn);
    }
    else if(0 == strcmp(pstSounetMode->acMode, "3G"))
    {
        stNetType.enNetType = LTE_3G_ONLY;
        snprintf(acCmd, sizeof(acCmd), "at+cops=1,2,\"%s\",2", pstSounetMode->acPlmn);
    }
    else if(0 == strcmp(pstSounetMode->acMode, "2G"))
    {
        stNetType.enNetType = LTE_2G_ONLY;
        snprintf(acCmd, sizeof(acCmd), "at+cops=1,2,\"%s\",0", pstSounetMode->acPlmn);
    }
    else
    {
        return LTE_FAIL;
    }
    
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_SOUNET_MODE_SET;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
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

    iRet = lte_client_recv(iClientSocketFd, &stLteRecvMsg, 30);
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
        iRet = leadcore_nettype_set((void *)&stNetType, sizeof(LTE_NET_TYPE_T));
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;
}

static LTE_RET_E leadcore_unlock_puk(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[32] = {0};
    char acCiev[] = "OK";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_SIM_UNLOCK_PUK_T *pstUnlockPuk = (LTE_SIM_UNLOCK_PUK_T *)pvData;
	snprintf(acCmd, sizeof(acCmd), "at+cpin=\"%s\",\"%s\"", pstUnlockPuk->acPuk,pstUnlockPuk->acPinNew);
    iClientSocketFd = lte_client_sock_init();       
    
    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_SIM_UNLOCK_PUK;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
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

static LTE_RET_E leadcore_module_up_support(void *pvData, int iDataLen)
{
    
    LTE_MODULE_UP_SUPPORT_T *pstModuleUpSupp = (LTE_MODULE_UP_SUPPORT_T* )pvData;
    LTE_MODULE_VERSION_T stVersion = {0};
    if(NULL == pstModuleUpSupp || iDataLen < sizeof(LTE_MODULE_UP_SUPPORT_T))
    {
        return LTE_FAIL;
    }
    pstModuleUpSupp->iModuleUpFlag = MODULE_DISABLE_UP;
    return LTE_OK;
}
/*****************************************************************************
 函 数 名  : leadcore_pin_input_remain
 功能描述  : 获取PIN码输入剩余次数
 输入参数  : void *pvData    
             int iDataLen  
 输出参数  : 无
 返 回 值  : LTE_RET_E
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年9月21日
    作    者   : 王奇
    修改内容   : 新生成函数

*****************************************************************************/

static LTE_RET_E leadcore_pin_input_remain(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "AT^DRAP";
    char acCiev[] = "^DRAP:";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_PIN_INPUT_REMAIN_T *pstPinInputRemain = (LTE_PIN_INPUT_REMAIN_T *)pvData;
    
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_PIN_INPUT_REMAIN_GET;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
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
        pstPinInputRemain->iPinIputRemain = stLteRecvMsg.uLteCommand.stPinInputRemain.iPinIputRemain;
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;

}
#endif

static LTE_RET_E huawei4G_post_route_set(void *pvData, int iDataLen)
{
	int iClientSocketFd = -1;
	int iRet = 0;
	char acCmd[64] = {0};
	char acCiev[] = "OK";
	char *pcBuf = NULL;

	CLIENT_SEND_MSG_T stLteMsg = {0};
	LTE_SEND_MSG_T stLteRecvMsg = {0};

	if (NULL == pvData)
	{
	   return LTE_FAIL;
	}
	LTE_POST_ROUTE_SW_T *pstPostRouteSw = (LTE_POST_ROUTE_SW_T*)pvData;
	
	snprintf(acCmd, sizeof(acCmd), "at+rbms=%d", pstPostRouteSw->iPostRouteSw);

	iClientSocketFd = lte_client_sock_init();

	if(iClientSocketFd < 0)
	{
	   LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
	   return LTE_FAIL;
	}

	stLteMsg.iFuncCmd = LTE_POST_ROUTE_SET;
	stLteMsg.iPriority = 0;
	stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));

	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);

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

/*想要往华为模块nvram中读/写数据，必须先用datalock解锁*/
static LTE_RET_E huawei4G_nv_datalock(void *pvData, int iDataLen)
{
	int iClientSocketFd = -1;
	int iRet = 0;
	char acCmd[64] = {0};
	char acCiev[] = "OK";
	char *pcBuf = NULL;

	CLIENT_SEND_MSG_T stLteMsg = {0};
	LTE_SEND_MSG_T stLteRecvMsg = {0};

	if (NULL == pvData)
	{
	   return LTE_FAIL;
	}
	LTE_NV_DATALOCK_T *pstNvDatalock = (LTE_NV_DATALOCK_T*)pvData;
	
	snprintf(acCmd, sizeof(acCmd), "at^datalock=%s", pstNvDatalock->acNvDatalock);

	iClientSocketFd = lte_client_sock_init();

	if(iClientSocketFd < 0)
	{
	   LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
	   return LTE_FAIL;
	}

	stLteMsg.iFuncCmd = LTE_NV_DATALOCK;
	stLteMsg.iPriority = 0;
	stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));

	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);

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

static LTE_RET_E huawei4G_post_route_get(void *pvData, int iDataLen)
{
	int iClientSocketFd = -1;
	int iRet = 0;
	char acCmd[] = "at+rbms?";
	char acCiev[] = "+RBMS:";
	char *pcBuf = NULL;

	CLIENT_SEND_MSG_T stLteMsg = {0};
	LTE_SEND_MSG_T stLteRecvMsg = {0};

	if (NULL == pvData)
	{
	   return LTE_FAIL;
	}
	LTE_POST_ROUTE_SW_T *pstPostRouteSw = (LTE_POST_ROUTE_SW_T*)pvData;
	
	iClientSocketFd = lte_client_sock_init();

	if(iClientSocketFd < 0)
	{
	   LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
	   return LTE_FAIL;
	}

	stLteMsg.iFuncCmd = LTE_POST_ROUTE_GET;
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
	   pstPostRouteSw->iPostRouteSw = stLteRecvMsg.uLteCommand.stPostRouteSw.iPostRouteSw;
	   iRet = LTE_OK;
	}
	else
	{
	   iRet = LTE_FAIL;
	}

	return iRet;
}

static LTE_RET_E huawei4G_remove_attachments(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cgatt=0";
    char acCiev[] = "OK";
    char *pcBuf = NULL;

    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
       LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
       return LTE_FAIL;
    }

    stLteMsg.iFuncCmd = LTE_REMOVE_ATTACH;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
    strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
    strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));

    LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);

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
static LTE_RET_E huawei4G_qos_set(void *pvData, int iDataLen)
{
	int iClientSocketFd = -1;
	int iRet = 0;
	char acCmd[64] = {0};
	char acCiev[] = "OK";
	char *pcBuf = NULL;

	CLIENT_SEND_MSG_T stLteMsg = {0};
	LTE_SEND_MSG_T stLteRecvMsg = {0};

	if (NULL == pvData)
	{
	   return LTE_FAIL;
	}
	LTE_QOS_DATA_S *pstQosData = (LTE_QOS_DATA_S*)pvData;

	if(pstQosData->iArgcNumber==6)
	{
		snprintf(acCmd, sizeof(acCmd), "at+cgeqos=%d,%d,%d,%d,%d,%d", pstQosData->iQosCid,pstQosData->iQosQci,
		pstQosData->iQosDlGbr,pstQosData->iQosUlGbr,pstQosData->iQosDlMbr,pstQosData->iQosUlMbr);
	}
	if(pstQosData->iArgcNumber==5)
	{
		snprintf(acCmd, sizeof(acCmd), "at+cgeqos=%d,%d,%d,%d,%d", pstQosData->iQosCid,pstQosData->iQosQci,
		pstQosData->iQosDlGbr,pstQosData->iQosUlGbr,pstQosData->iQosDlMbr);
	}
	if(pstQosData->iArgcNumber==4)
	{
		snprintf(acCmd, sizeof(acCmd), "at+cgeqos=%d,%d,%d,%d", pstQosData->iQosCid,pstQosData->iQosQci,
		pstQosData->iQosDlGbr,pstQosData->iQosUlGbr);
	}
	if(pstQosData->iArgcNumber==3)
	{
		snprintf(acCmd, sizeof(acCmd), "at+cgeqos=%d,%d,%d", pstQosData->iQosCid,pstQosData->iQosQci,
		pstQosData->iQosDlGbr);
	}
	if(pstQosData->iArgcNumber==2)
	{
		snprintf(acCmd, sizeof(acCmd), "at+cgeqos=%d,%d", pstQosData->iQosCid,pstQosData->iQosQci);
	}
	if(pstQosData->iArgcNumber==1)
	{
		snprintf(acCmd, sizeof(acCmd), "at+cgeqos=%d", pstQosData->iQosCid);
	}
	iClientSocketFd = lte_client_sock_init();

	if(iClientSocketFd < 0)
	{
	   LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
	   return LTE_FAIL;
	}

	stLteMsg.iFuncCmd = LTE_QOS_DATA_SET;
	stLteMsg.iPriority = 0;
	stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));

	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);

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

static LTE_RET_E huawei4G_qos_get(void *pvData, int iDataLen)
{
	int iClientSocketFd = -1;
	int iRet = 0;
	char acCmd[] = "at+cgeqos?";
	char acCiev[] = "+CGEQOS:";
	char *pcBuf = NULL;

	CLIENT_SEND_MSG_T stLteMsg = {0};
	LTE_SEND_MSG_T stLteRecvMsg = {0};

	if (NULL == pvData)
	{
	   return LTE_FAIL;
	}
	LTE_QOS_DATA_S *pstQosData = (LTE_QOS_DATA_S*)pvData;
	
	iClientSocketFd = lte_client_sock_init();

	if(iClientSocketFd < 0)
	{
	   LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
	   return LTE_FAIL;
	}

	stLteMsg.iFuncCmd = LTE_QOS_DATA_GET;
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
		
	   *pstQosData= stLteRecvMsg.uLteCommand.stQosData;
	   iRet = LTE_OK;
	}
	else
	{
	   iRet = LTE_FAIL;
	}
	
	return iRet;
}

static LTE_RET_E meige_lte_dload_prot_ver_get(char *acFilePath, LTE_DLOAD_PROT_VER_T *pstDloadProtVer)
{
    FILE  *pstFd = NULL;
    char acBuff[64] = {0};
    
    pstFd = fopen(acFilePath,"r");
    if (NULL == pstFd)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:open %s failed\n",__LINE__,__func__,acFilePath);
        return LTE_FAIL;
    }

    while (NULL != fgets(acBuff,64,pstFd))
    {      
        if(NULL != strstr(acBuff, "swver:"))
        {
        	//sscanf(acBuff, "%*[^:]:%s", pstDloadProtVer->acSwer);
            strncpy(pstDloadProtVer->acSwer,acBuff,sizeof(pstDloadProtVer->acSwer));
            //sscanf(acBuff, "%s", pstDloadProtVer->acSwer);
        }
        else if(NULL != strstr(acBuff, "isover:"))
        {
            strncpy(pstDloadProtVer->acIosVer,acBuff,sizeof(pstDloadProtVer->acIosVer));
        }
        else if(NULL != strstr(acBuff, "product name:"))
        {
        	strncpy(pstDloadProtVer->acProductName,acBuff,sizeof(pstDloadProtVer->acProductName));
        }
        else if(NULL != strstr(acBuff, "dload type:"))
        {
        	strncpy(pstDloadProtVer->acDloadType,acBuff,sizeof(pstDloadProtVer->acDloadType));
        }

        memset(acBuff,0,64);
    }
    fclose(pstFd);
    lte_file_unlink(acFilePath);
    return LTE_OK;
}


static LTE_RET_E huawei4G_dload_prot_ver(void *pvData, int iDataLen)
{
	int iClientSocketFd = -1;
	int iRet = 0;
	char acCmd[] = "at^dloadinfo?\r";
	char acCiev[] = "OK";
	char *pcBuf = NULL;

	CLIENT_SEND_MSG_T stLteMsg = {0};
	LTE_SEND_MSG_T stLteRecvMsg = {0};
	
	if (NULL == pvData)
	{
	   return LTE_FAIL;
	}
	LTE_DLOAD_PROT_VER_T *pstDloadProtVer = (LTE_DLOAD_PROT_VER_T*)pvData;
	iClientSocketFd = lte_client_sock_init();
	if(iClientSocketFd < 0)
	{
	   LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
	   return LTE_FAIL;
	}

	stLteMsg.iFuncCmd = LTE_DLOAD_PROT_VER_GET;
	stLteMsg.iPriority = 0;
	stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));

	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);

	stLteMsg.iComReadType = COM_READ_TYPE_9;
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
		iRet = meige_lte_dload_prot_ver_get(LTE_LCTCELLINFO_FILE, pstDloadProtVer);
		if(LTE_OK != iRet)
		{
			LTE_LOG(LTE_LOG_ERR,"%d,%s:longcheer_lteinfo_analyze failed!\n",__LINE__,__func__);
			return LTE_FAIL;
		}
		iRet = LTE_OK;
	}
	else
	{
	   iRet = LTE_FAIL;
	}

	return iRet;
}
static LTE_RET_E huawei4G_dload_ver_info(void *pvData, int iDataLen)
{
	int iClientSocketFd = -1;
	int iRet = 0;
	char acCmd[] = "at^dloadver?\r";
	char acCiev[] = "OK";
	char *pcBuf = NULL;

	CLIENT_SEND_MSG_T stLteMsg = {0};
	LTE_SEND_MSG_T stLteRecvMsg = {0};
	
	if (NULL == pvData)
	{
	   return LTE_FAIL;
	}
	LTE_DLOAD_VER_INFO_T *pstDloadVerInfo = (LTE_DLOAD_VER_INFO_T*)pvData;
	iClientSocketFd = lte_client_sock_init();
	if(iClientSocketFd < 0)
	{
	   LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
	   return LTE_FAIL;
	}

	stLteMsg.iFuncCmd = LTE_DLOAD_VER_INFO_GET;
	stLteMsg.iPriority = 0;
	stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));

	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);

	stLteMsg.iComReadType = COM_READ_TYPE_5;
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
        strncpy(pstDloadVerInfo->acDloadVerInfo,stLteRecvMsg.uLteCommand.stDloadVerInfo.acDloadVerInfo,sizeof(pstDloadVerInfo->acDloadVerInfo));
		iRet = LTE_OK;
	}
	else
	{
	   iRet = LTE_FAIL;
	}

	return iRet;
}
static LTE_RET_E huawei4G_nv_backup(void *pvData, int iDataLen)
{
	int iClientSocketFd = -1;
	int iRet = 0;
	char acCmd[] = "at^nvbackup\r";
	char acCiev[] = "OK";
	char *pcBuf = NULL;

	CLIENT_SEND_MSG_T stLteMsg = {0};
	LTE_SEND_MSG_T stLteRecvMsg = {0};
	
	if (NULL == pvData)
	{
	   return LTE_FAIL;
	}
	LTE_NV_BACKUP_T *pstNvBackup = (LTE_NV_BACKUP_T*)pvData;
	iClientSocketFd = lte_client_sock_init();
	if(iClientSocketFd < 0)
	{
	   LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
	   return LTE_FAIL;
	}

	stLteMsg.iFuncCmd = LTE_NV_BACKUP;
	stLteMsg.iPriority = 0;
	stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));

	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);

	stLteMsg.iComReadType = COM_READ_TYPE_5;
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
        pstNvBackup->iNvBackup = stLteRecvMsg.uLteCommand.stNvBackup.iNvBackup;
		iRet = LTE_OK;
	}
	else
	{
	   iRet = LTE_FAIL;
	}

	return iRet;
}
static LTE_RET_E huawei4G_nv_restore(void *pvData, int iDataLen)
{
	int iClientSocketFd = -1;
	int iRet = 0;
	char acCmd[] = "at^nvrestore\r";
	char acCiev[] = "OK";
	char *pcBuf = NULL;

	CLIENT_SEND_MSG_T stLteMsg = {0};
	LTE_SEND_MSG_T stLteRecvMsg = {0};
	
	if (NULL == pvData)
	{
	   return LTE_FAIL;
	}
	LTE_NV_RESTORE_T *pstNvRestore = (LTE_NV_RESTORE_T*)pvData;
	iClientSocketFd = lte_client_sock_init();
	if(iClientSocketFd < 0)
	{
	   LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
	   return LTE_FAIL;
	}

	stLteMsg.iFuncCmd = LTE_NV_RESTORE;
	stLteMsg.iPriority = 0;
	stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));

	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);

	stLteMsg.iComReadType = COM_READ_TYPE_5;
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
        pstNvRestore->iNvRestore = stLteRecvMsg.uLteCommand.stNvRestore.iNvRestore;
		iRet = LTE_OK;
	}
	else
	{
	   iRet = LTE_FAIL;
	}

	return iRet;
}
static LTE_RET_E huawei4G_dload_mode(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at^godload\r";
    char acCiev[] = "OK";
    char *pcBuf = NULL;

    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
       LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
       return LTE_FAIL;
    }

    stLteMsg.iFuncCmd = LTE_DLOAD_MODE;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
    strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
    strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));

    LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);

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
static LTE_RET_E huawei4G_data_mode(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at^datamode\r";
    char acCiev[] = "OK";
    char *pcBuf = NULL;

    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
       LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
       return LTE_FAIL;
    }

    stLteMsg.iFuncCmd = LTE_DATA_MODE;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
    strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
    strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));

    LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);

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
static LTE_RET_E huawei4G_reset(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at^reset\r";
    char acCiev[] = "OK";
    char *pcBuf = NULL;

    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
       LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
       return LTE_FAIL;
    }

    stLteMsg.iFuncCmd = LTE_RESET;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
    strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
    strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));

    LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);

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
static LTE_RET_E huawei4G_sim_switch_get(void *pvData, int iDataLen)
{
	int iClientSocketFd = -1;
	int iRet = 0;
	char acCmd[] = "at^hvsst?\r";
	char acCiev[] = "OK";
	char *pcBuf = NULL;

	CLIENT_SEND_MSG_T stLteMsg = {0};
	LTE_SEND_MSG_T stLteRecvMsg = {0};
	
	if (NULL == pvData)
	{
	   return LTE_FAIL;
	}
	LTE_SIM_SWITCH_T *pstSimSwitch = (LTE_SIM_SWITCH_T*)pvData;
	iClientSocketFd = lte_client_sock_init();
	if(iClientSocketFd < 0)
	{
	   LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
	   return LTE_FAIL;
	}
	
	
	stLteMsg.iFuncCmd = LTE_SIM_SWITCH_GET;
	stLteMsg.iPriority = 0;
	stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));

	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);

	stLteMsg.iComReadType = COM_READ_TYPE_5;
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
		pstSimSwitch->iSimSw = stLteRecvMsg.uLteCommand.stSimSwitch.iSimSw;
		iRet = LTE_OK;
	}
	else
	{
	   iRet = LTE_FAIL;
	}

	return iRet;
}
static LTE_RET_E huawei4G_sim_switch_set(void *pvData, int iDataLen)
{
	int iClientSocketFd = -1;
	int iRet = 0;
	char acCmd[64] = {0};
	char acCmdLast[64] = {0};
	char acCiev[] = "OK";
	char *pcBuf = NULL;

	CLIENT_SEND_MSG_T stLteMsg = {0};
	LTE_SEND_MSG_T stLteRecvMsg = {0};

	if (NULL == pvData)
	{
	   return LTE_FAIL;
	}
	LTE_SIM_SWITCH_T *pstSimSwitch = (LTE_SIM_SWITCH_T*)pvData;

	snprintf(acCmd, sizeof(acCmd), "AT^HVSST=%d,%d",pstSimSwitch->iSimChoice,pstSimSwitch->iSimEnable);

	LTE_LOG(LTE_LOG_INFO,"%d,%s: acCmd=%s \n",__LINE__,__func__,acCmd);
	
	iClientSocketFd = lte_client_sock_init();

	if(iClientSocketFd < 0)
	{
	   LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
	   return LTE_FAIL;
	}

	stLteMsg.iFuncCmd = LTE_SIM_SWITCH_SET;
	stLteMsg.iPriority = 0;
	stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));

	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);

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
static LTE_RET_E huawei4G_sim_switch_soft_set(void *pvData, int iDataLen)
{
	int iClientSocketFd = -1;
	int iRet = 0;
	char acCmd[128] = {0};
	char acCiev[] = "OK";
	char *pcBuf = NULL;

	CLIENT_SEND_MSG_T stLteMsg = {0};
	LTE_SEND_MSG_T stLteRecvMsg = {0};

	if (NULL == pvData)
	{
	   return LTE_FAIL;
	}
	LTE_SIM_SWITCH_T *pstSimSwitch = (LTE_SIM_SWITCH_T*)pvData;
	
	
	snprintf(acCmd, sizeof(acCmd), "AT^HVSSSINFO=\"%s\",%d,\"%s\",\"%s\"", \
			pstSimSwitch->acSimImsi,pstSimSwitch->iSimMnclen,pstSimSwitch->acSimKey,pstSimSwitch->acSimOp);
	
	LTE_LOG(LTE_LOG_INFO,"%d,%s: acCmd=%s \n",__LINE__,__func__,acCmd);
	
	iClientSocketFd = lte_client_sock_init();

	if(iClientSocketFd < 0)
	{
	   LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
	   return LTE_FAIL;
	}

	stLteMsg.iFuncCmd = LTE_SIM_SWITCH_SOFT_SET;
	stLteMsg.iPriority = 0;
	stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));

	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);

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
LTE_IOCTL_FUNC_T astHuawei4GTable[] = 
{    
    {LTE_PIN_STATE,         huawei4G_pin_state,            0,0,    "huawei4G_pin_state"       }, 
    {LTE_PIN_SWITCH,        huawei4G_pin_switch,           0,0,    "huawei4G_pin_switch"       }, 
    {LTE_PIN_SW_GET,        huawei4G_pin_sw_get,           0,0,    "huawei4G_pin_sw_get"       },
    {LTE_PIN_UNLOCK,        huawei4G_pin_unlock,           0,0,    "huawei4G_pin_unlock"       }, 
    {LTE_PIN_ATTEMPTS,      huawei4G_pin_attempts_get,     0,0,    "huawei4G_pin_attempts_get"       }, 
    {LTE_PIN_ALTER,         huawei4G_pin_alter,            0,0,    "huawei4G_pin_alter"       }, 
    {LTE_SIGNAL_GET,        huawei4G_signal_get,           0,0,    "huawei4G_signal_get"       }, 
    {LTE_POWER_ON,          huawei4G_power_on,             0,0,    "huawei4G_power_on"       }, 
    {LTE_POWER_OFF,         huawei4G_power_off,            0,0,    "huawei4G_power_off"       }, 
    {LTE_NETTYPE_SET,       NULL,           0,0,    "leadcore_nettype_set"       }, 
    {LTE_NETTYPE_GET,       NULL,           0,0,    "leadcore_nettype_get"       }, 
    {LTE_PLMN_REGISTER,     NULL,           0,0,    "leadcore_plmn_register"       }, 
    {LTE_PREFER_PLMN_SET,   NULL,           0,0,    "leadcore_prefer_plmn_set"       }, 
    {LTE_NETWORK_PREPARE,   huawei4G_network_prepare,      0,0,    "huawei4G_network_prepare"       }, 
    {LTE_NETWORK_STATUS_GET,NULL,           0,0,    NULL       },
    {LTE_CONNECT,           huawei4G_connect,              0,0,    "huawei4G_connect"       }, 
    {LTE_DISCONNECT,        huawei4G_disconnect,           0,0,    "huawei4G_disconnect"       }, 
    {LTE_LINKSTATE_GET,     huawei4G_linkstate_get,        0,0,    "huawei4G_linkstate_get"       }, 
    {LTE_IMEI_GET,          huawei4G_imei_get,             0,0,    "huawei4G_imei_get"       }, 
    {LTE_CELLID_GET,        huawei4G_cellid_get,           0,0,    "huawei4G_cellid_get"       }, 
    {LTE_ICCID_GET,         huawei4G_iccid_get,            0,0,    "huawei4G_iccid_get"       },
    {LTE_IMSI_GET,          huawei4G_imsi_get,             0,0,    "huawei4G_imsi_get"       },
    {LTE_SIM_STATE_GET,     huawei4G_sim_state_get,        0,0,    "huawei4G_sim_state_get"       }, 
    {LTE_INFO_GET,          huawei4G_lte_info_get,         0,0,    "huawei4G_lte_info_get"       }, 
    {LTE_ACCESS_NET_GET,    huawei4G_access_net_get,       0,0,    "huawei4G_access_net_get"       }, 
    {LTE_BANDS_GET,         NULL,           0,0,    "leadcore_bands_get"       }, 
    {LTE_CLCK_BANDS_SET,    NULL,           0,0, "leadcore_clck_bands_set"       },
    {LTE_CLCK_BANDS_CLOSE,  NULL,            0,0,       NULL       },
    {LTE_CLCK_CELL_GET,     huawei4G_clckcell_enable_get,  0,0,    "huawei4G_clckcell_enable_get"   },
    {LTE_CLCK_CELL_SET,     huawei4G_clck_cellid_set,           0,0,    NULL       },
    {LTE_OPER_INFO_GET,     huawei4G_oper_info_get,        0,0, "huawei4G_oper_info_get"          },
    {LTE_NETTYPE_INFO_GET,  huawei4G_sup_nettype_get,      0,0, "huawei4G_sup_nettype_get"          },
    {LTE_GPS_START,        NULL,           0,0,    NULL       }, 
    {LTE_GPS_STOP,         NULL,           0,0,    NULL       }, 
    {LTE_GPS_DATE_GET,        NULL,           0,0,    NULL       }, 
    {LTE_GPS_POSITION_INFO_GET,        NULL,           0,0,    NULL       }, 

    /* BEGIN: Added by wujian, 2014/11/13   PN:添加短信功能相关接口 */
    {LTE_SMS_CENT_CODE_SET, NULL, 0, 0, "leadcore_sms_cent_code_set" },
    {LTE_SMS_MODE__SET,     NULL, 0, 0, "leadcore_sms_mode_set"   },
    {LTE_SMS_FUNC_INIT,     NULL, 0, 0, "leadcore_sms_func_init"   },
    {LTE_SMS_RECV,          NULL, 0, 0,        "leadcore_sms_recv"   },
    {LTE_SMS_SEND,          NULL, 0, 0,        "leadcore_sms_send"   },
    {LTE_SMS_DELE,          NULL, 0, 0,        "leadcore_sms_dele"   },
    {LTE_SMS_SUPP,          NULL, 0, 0,    "leadcore_sms_sup_func"   },
    /* END:   Added by wujian, 2014/11/13   PN:添加短信功能相关接口 */
    {LTE_IPV6_PDP_SET,      NULL, 0, 0,     "leadcore_cgdcont_set"   },
    /* BEGIN: Added by wujian, 2014/11/17   PN:添加设置apn接口 */
    {LTE_APN_SET,           huawei4G_apn_set,         0,0, "huawei4G_apn_set"   },
    /* END:   Added by wujian, 2014/11/17   PN:添加设置apn接口 */
    {LTE_VERSION_GET,       huawei4G_version_get,     0,0, "huawei4G_version_get"       },
    {LTE_ENODEBID_GET,      huawei4G_enodeb_id_get,   0,0, "huawei4G_enodeb_id_get"    }, 
    {LTE_TAC_GET,           huawei4G_tac_get,         0,0, "huawei4G_tac_get"    },
    {LTE_LCID_GET,          huawei4G_lcid_get,        0,0, "huawei4G_lcid_get"    },
    {LTE_ALL_CELLID_GET,    huawei4G_all_cellid_get,  0,0, "huawei4G_all_cellid_get"    },
    {LTE_MOUDLE_MAC_SET,    NULL,         0,0, "leadcore_mac_set"    },
    {LTE_MOUDLE_MAC_GET,    NULL,         0,0, "leadcore_mac_get"    },
    /* BEGIN: Added by wangqi, 2015/5/5   PN:添加设置机卡互锁接口 */
    {LTE_SIM_UNLOCK,        NULL, 0,0,      "leadcore_sim_unlock"},
    {LTE_SIM_DISABLE_UNLOCK,NULL, 0,0, "leadcore_sim_disable_unlock"},
    {LTE_SIM_UNLOCK_SUPPORT,NULL, 0,0, "leadcore_sim_unlock_support"},
    {LTE_SIM_TYPE_GET,      NULL, 0,0,    "leadcore_sim_type_get"},
    {LTE_SIM_UNLOCK_GET,    NULL, 0,0,  "leadcore_sim_unlock_get"},
    /* END:   Added by wangqi, 2015/5/5   PN:添加设置机卡互锁接口 */  
    {LTE_SIGNAL_VALUE_GET,  NULL, 0,0,    "leadcore_sim_type_get"},
    {LTE_IP_GATEWAY_DNS_GET,NULL, 0,0,  "leadcore_sim_type_get"},
    {LTE_ALL_PLMN_MODE_GET,      NULL,  0,0, "leadcore_all_plmn_mode_get"    },
    {LTE_SOUNET_MODE_SET,   NULL,  0,0, "leadcore_sounet_mode_set"    },
    {LTE_SIM_UNLOCK_PUK,    NULL,  0,0, "leadcore_puk_unlock"    },
    {LTE_MODULE_UP_SUPPORT, NULL, 0,0, "leadcore_module_up_support"},
    {LTE_PIN_INPUT_REMAIN_GET,  NULL, 0,0, "leadcore_pin_input_remain"},
    {LTE_POST_ROUTE_SET,huawei4G_post_route_set,  0,0, "huawei4G_post_route_set"    },
    {LTE_NV_DATALOCK,   huawei4G_nv_datalock,     0,0, "huawei4G_nv_datalock"    },
    {LTE_POST_ROUTE_GET,huawei4G_post_route_get,  0,0, "huawei4G_post_route_get"    },
    {LTE_REMOVE_ATTACH,huawei4G_remove_attachments,  0,0, "huawei4G_remove_attachments"    },
    {LTE_QOS_DATA_SET,huawei4G_qos_set,  0,0, "huawei4G_qos_set"    },
    {LTE_QOS_DATA_GET,huawei4G_qos_get,  0,0, "huawei4G_qos_get"    },
    {LTE_DLOAD_PROT_VER_GET,huawei4G_dload_prot_ver,  0,0, "huawei4G_dload_prot_ver"    },
    {LTE_DLOAD_VER_INFO_GET,huawei4G_dload_ver_info,  0,0, "huawei4G_dload_ver_info"    },
    {LTE_NV_BACKUP, huawei4G_nv_backup, 0, 0, "huawei4G_nv_backup"},
    {LTE_NV_RESTORE, huawei4G_nv_restore, 0, 0, "huawei4G_nv_restore"},
    {LTE_DLOAD_MODE, huawei4G_dload_mode, 0, 0, "huawei4G_dload_mode"},
    {LTE_DATA_MODE, huawei4G_data_mode, 0, 0, "huawei4G_dload_mode"},
    {LTE_RESET, huawei4G_reset, 0, 0, "huawei4G_reset"},
    {LTE_SIM_SWITCH_SOFT_SET, huawei4G_sim_switch_soft_set, 0, 0, "huawei4G_sim_switch_soft_set"},
    {LTE_SIM_SWITCH_SET, huawei4G_sim_switch_set, 0, 0, "huawei4G_sim_switch_set"},
    {LTE_SIM_SWITCH_GET, huawei4G_sim_switch_get, 0, 0, "huawei4G_sim_switch_get"},
    {LTE_CMD_MAX,           NULL,           0,0,    NULL       }, 
};

