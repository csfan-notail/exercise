/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : bsp_lte_ioctl.c
  版 本 号   : 初稿
  作    者   :  蔡兵
  生成日期   : 2012年7月23日 星期二
  最近修改   :
  功能描述   : bsp  lte ioctl
  函数列表   :
  修改历史   :
  1.日    期   : 2012年7月23日 星期二
    作    者   : 蔡兵
    修改内容   : 创建文件

******************************************************************************/
/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
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

#include "lte.h"
#include "lte_log.h"
#include "lte_common.h"
#include "lte_usbcom.h"
#include "ndis_qmi_service.h"

 
#define ATCOM  "/dev/ttyUSB2"
#define PIDFILE "/tmp/pidtimer"
#define BUFMAX 16
#define LONGCHEER_BAND_DEFAULT       "default"

typedef enum
{
    UMTS_ONLY = 1,  				
    AUTO      = 2,			     
    GSM_ONLY  = 3,
    GSM_PREFERRED = 4,
    LTE_ONLY  = 5,
    TDSCDMA_ONLY   = 6,
    TDSCDMA_AND_WCDMA = 7
}LTE_LONGCHEER_NETTYPE_E;

static int g_connect_flag=0;
static int g_connect_fail=0;

static LTE_RET_E longcheer_version_get(void *pvData, int iDataLen);

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
 函 数 名  : longcheer_pin_state
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
static LTE_RET_E longcheer_pin_state (void *pvData, int iDataLen)
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
 函 数 名  : longcheer_pin_switch
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
static LTE_RET_E longcheer_pin_switch (void *pvData, int iDataLen)
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
 函 数 名  : longcheer_pin_sw_get
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
static LTE_RET_E longcheer_pin_sw_get( void *pvData, int iDataLen )
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
 函 数 名  : longcheer_pin_unlock
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
 static LTE_RET_E longcheer_pin_unlock (void *pvData, int iDataLen)
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
 函 数 名  : longcheer_pin_attempts_get
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
static LTE_RET_E longcheer_pin_attempts_get(void *pvData, int iDataLen)
{
    return LTE_OK;
}


/*****************************************************************************
 函 数 名  : longcheer_pin_alter
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
static LTE_RET_E longcheer_pin_alter (void *pvData, int iDataLen)
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


static LTE_RET_E longcheer_power_on (void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cfun=1";
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
static LTE_RET_E longcheer_power_get(void *pvData, int iDataLen)
{
    int iRet = 0;    
    FILE* pstFd = NULL;
    char acCiev[] = "+CFUN:";
    char acBuff[128] = {0};
    char acQueCfun[32] = {0};
    LTE_CFUN_QUERY_T *pstCfunQuery = (LTE_CFUN_QUERY_T *)pvData;
    
    LTE_NORMAL_AT_T stAt = {"at+cfun?\r","/tmp/cfunquery","OK",0,ATCOM,0,1};

    if(NULL == pvData)    
    {
        return LTE_FAIL;
    }
    /*执行at指令*/
    iRet = lte_normal_at_cmd(&stAt);
    if (iRet != 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:at command failed",__LINE__,__func__);
        lte_file_unlink(stAt.acFileName);
        /*默认开机*/
        return LTE_FAIL;
    }

    pstFd = fopen(stAt.acFileName,"r");
    if(NULL == pstFd)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:open %s failed\n",__LINE__,__func__, stAt.acFileName);
        lte_file_unlink(stAt.acFileName);
        return LTE_FAIL;
    }
    while(NULL != fgets(acBuff,128,pstFd))
    {
        if(NULL != strstr(acBuff, acCiev))
        {
            sscanf(acBuff, "%*[^:]:%s", acQueCfun);
            break;
        }
    }
    /*close file*/
    fclose(pstFd);
    pstFd = NULL;

    /*没有发现想要的关键字，则返回error*/
    lte_file_unlink(stAt.acFileName);
    pstCfunQuery->iCfunFlag = atoi(acQueCfun);
    LTE_LOG(LTE_LOG_INFO, "%d,%s:acQueCfun %s iCfunFlag %d\n",__LINE__,__func__, acQueCfun, pstCfunQuery->iCfunFlag);

    return LTE_OK;
}
static LTE_RET_E longcheer_power_off (void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cfun=0";
    char acCiev[] = "OK";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    
    LTE_LOG(LTE_LOG_ERR,"%d,%s:zte_micro_b57_5_pin_switch failed\n",__LINE__,__func__);

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



/*****************************************************************************
 函 数 名  : pal_bsp_lte_signal_quality
 功能描述  : 获取电平信号值
 输入参数  : void *pvData    
             int iDataLen  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月23日
    作    者   : 陈键
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E longcheer_signal_get  (void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cind?";
    char acCiev[] = "+CIND:";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_SIGNAL_T *pstSignal = (LTE_SIGNAL_T*)pvData;
    
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
        pstSignal->uiSignal = stLteRecvMsg.uLteCommand.stSignalInfo.uiSignal;
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;

}

static LTE_RET_E longcheer_set_nettype_cmd(LTE_NET_TYPE_T *pstNetType,char *pcComdstm)
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
            sprintf(pcComdstm, "at+mododr=5\r"); /* 仅4g */
            break;
        }
        
        case LTE_3G_ONLY: 
        {
            sprintf(pcComdstm, "at+mododr=7\r");/* 3g 兼容9106*/
            break;
        }
        case LTE_2G_ONLY:
        {
           
            sprintf(pcComdstm, "at+mododr=3\r");/* 仅2g */
            break;
        }
        case LTE_2G_3G_4G:
        {
            sprintf(pcComdstm, "at+mododr=2\r");/* 2g, 3g, 4g ,4g优先 */
            break;
        }
        default:
        {
            LTE_LOG(LTE_LOG_ERR,"%d,%s:unkown net type,default lte prefer",__LINE__,__func__);
            return LTE_FAIL;
        }
    }
    return LTE_OK;

}

/*****************************************************************************
 函 数 名  : pal_bsp_lte_set_net_type
 功能描述  : 网页上选择接入LTE模式 or TD-SCDMA 
             or LTE/TD-SCDMA多模式 lte 优先 or LTE/TD-SCDMA多模式 lte 优先
 输入参数  : 接入技术类别
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月14 星期三
    作    者   : 蔡兵
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E longcheer_nettype_set (void *pvData, int iDataLen)
{
    int iRet = 0;
    int iClientSocketFd = -1;
    char acCmd[32] = {0};
    char acCiev[] = "OK";
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};
    
    if (NULL == pvData)
    {
        return LTE_FAIL;
    }    
    
    LTE_NET_TYPE_T *pstNetType = (LTE_NET_TYPE_T *)pvData;    
    if( LTE_FAIL == longcheer_set_nettype_cmd(pstNetType, acCmd))
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:set netType cmd fail",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    iClientSocketFd = lte_client_sock_init();
    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_NETTYPE_SET;
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

static LTE_RET_E longcheer_nettype_get(void *pvData, int iDataLen)
{
    int iRet = 0;
    int iClientSocketFd = -1;
    char acCmd[] = "at+mododr?";
    char acCiev[] = "+MODODR:";
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


/*****************************************************************************
 函 数 名  : pal_bsp_set_apn
 功能描述  : 根据设置，来给出状态机的接入技术
 输入参数  :  
 输出参数  : 0,auto,1 lte 接入，2 td-s 接入
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月17 星期五
    作    者   : 蔡兵
    修改内容   : 新生成函数
    XXXXXXXXXXXX需要重点关注

*****************************************************************************/

static LTE_RET_E longcheer_imei_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+lctsn=0,7";
    char acCiev[] = "+LCTSN:";
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
        strncpy(pstImei->acImei, stLteRecvMsg.uLteCommand.stImeiInfo.acImei, sizeof(pstImei->acImei));
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;

}
static LTE_RET_E longcheer_cellid_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+lctcellinfo";
    char acCiev[] = "CELL_ID:";
    char *pcBuf = NULL;
    
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
        strncpy(pstCellid->acCellID, stLteRecvMsg.uLteCommand.stCellIdInfo.acCellID, sizeof(pstCellid->acCellID));
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;    
    
}

static LTE_RET_E longcheer_iccid_get(void *pvData, int iDataLen)
{
    LTE_ICCID_T *pstIccid = (LTE_ICCID_T *)pvData;
    if (pvData == NULL)
    {
		return LTE_FAIL;
    }
    
    strncpy(pstIccid->acICCID, "N/A", sizeof(pstIccid->acICCID)); /*空的置为n/a*/
    
    return LTE_OK;
}

/*****************************************************************************
 函 数 名  : pal_bsp_register_net_carrier
 功能描述  : 根据设置，来给出状态机的接入技术
 输入参数  :  
 输出参数  : 0,auto,1 lte 接入，2 td-s 接入
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月17 星期五
    作    者   : 蔡兵
    修改内容   : 新生成函数
    XXXXXXXXXXXX需要重点关注

*****************************************************************************/
static LTE_RET_E longcheer_plmn_register(void *pvData, int iDataLen)
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

static LTE_RET_E longcheer_prefer_plmn_set (void *pvData, int iDataLen)
{
    return LTE_OK;
}

/*****************************************************************************
 函 数 名  : pal_bsp_lte_linkstate_get
 功能描述  : 判断lte 是否注册上网络
 输入参数  :  
 输出参数  : 
 返 回 值  :  0,未注册，1 注册,其他的均大于 1 ，这个函数其他的大于1，有异于 pal_bsp
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012 年8月20 星期一
    作    者   : 蔡兵
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E longcheer_linkstate_get(void *pvData, int iDataLen)
{

    if (NULL == pvData)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:param invalid\n",__LINE__,__func__);
        return LTE_FAIL;
    }

    LTE_LINKSTATE_T *pstLinkstate = (LTE_LINKSTATE_T*)pvData;

    	int ret = -1, fd;
	ndis_ipinfo pipinfo;

	system_call("ifconfig usb0 up");
	
    	fd = ndis_open();
	if(fd == -1)
	{
		LTE_LOG(LTE_LOG_ERR,"%d,%s:ndis_open failed\n",__LINE__,__func__);
		goto EXIT;
	}
	
	ret = ndis_get_status(fd,&pipinfo);	
	if(ret<0)
	{
		LTE_LOG(LTE_LOG_ERR,"%d,%s:ndis_get_status failed.\n",__LINE__,__func__);
		ndis_close(fd);
		goto EXIT;
	}
	ndis_close(fd);
	
	if ((pipinfo.i32status_ipv4 == NDIS_CONNECTED) || (pipinfo.i32status_ipv6 == NDIS_CONNECTED)){
    		pstLinkstate->enLinkstate = LTE_LINKSTATE_CONNECTED;
		g_connect_flag = 2;
	}
	else if ((pipinfo.i32status_ipv4 == NDIS_CONNECTING) || (pipinfo.i32status_ipv6 == NDIS_CONNECTING)){
		pstLinkstate->enLinkstate = LTE_LINKSTATE_CONNECTING;
		g_connect_flag = 1;
	}
	else {
		g_connect_flag = 0;
		if (g_connect_fail)
			pstLinkstate->enLinkstate = LTE_LINKSTATE_CONNECT_FAIL;
		else
			pstLinkstate->enLinkstate = LTE_LINKSTATE_DISCONNECT;
	}
	
    LTE_LOG(LTE_LOG_INFO,"%d,%s:get linkstatus is %d\n",__LINE__,__func__,pstLinkstate->enLinkstate);
    return LTE_OK;

EXIT:
	if (g_connect_fail){
		pstLinkstate->enLinkstate = LTE_LINKSTATE_CONNECT_FAIL;
		return LTE_FAIL;
	}
	if (g_connect_flag == 1)
		pstLinkstate->enLinkstate = LTE_LINKSTATE_CONNECTING;
	else if (g_connect_flag == 2)
		pstLinkstate->enLinkstate = LTE_LINKSTATE_CONNECTED;
	else
		pstLinkstate->enLinkstate = LTE_LINKSTATE_DISCONNECT;
	return LTE_FAIL;
    
}

/*****************************************************************************
 函 数 名  : pal_bsp_lte_connect
 功能描述  : 拨号连接
 输入参数  : void *pvData    
             int iDataLen  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年10月25日星期四
    作    者   : 陈键
    修改内容   : 新生成函数

*****************************************************************************/

#define DISCONNECT_TIME_OUT  5    //500ms*5
#define AUTH_TYPE 0

/*altered by caibing 2013-11-25 for longcheer*/
static int longcheer_begin_connect(LTE_CONNECT_PARAM_T *pstConnectParam)
{
    char version[64] = {0};
	int ret = -1;
	int current_status = NDIS_DISCONNECTED;    
	/* 统计内部拨号如果连续 5 次 内部拨号不上则，进行模块级的开关机*/
	static int iCounter = 10;
	int iIPV4Flag = 0;
    int iIPV6Flag = 0;
	ndis_ipinfo pipinfo;
    /*altered by caibing 2014-09-16 begin*/
    char cDeviceMode = '0';
    FILE *pFile = NULL;
    char acFileName[64] = {"/tmp/device_mode.tmp"};
    char acBuf[32] = {0};
    /*altered by caibing 2014-09-16 end*/
	/*优先将usb0 up起来*/
	system_call("ifconfig usb0 up"); 

	int fd = ndis_open();	
	if ( fd == -1)
	{
		LTE_LOG(LTE_LOG_ERR,"%d,%s:ndis_open failed.\n",__LINE__,__func__);
		return -1;
	}

#if 0
	//if get version failed, indicated initliaze error.
	ret = ndis_get_lib_version(fd,version,sizeof(version));
	if ( ret != 0 || strlen(version) < 3 )
	{
		LTE_LOG(LTE_LOG_ERR,"%d,%s:ndis_get_lib_version failed,ret=%d.\n",__LINE__,__func__,ret);
        	ndis_close(fd);
		return -1;		
	}
    	else
	{
		LTE_LOG(LTE_LOG_INFO,"ndis_get_lib_version success,version=%s.\n",version);		
	}
#endif

	ret = extern_ndis_connect(fd,pstConnectParam->acApn,NULL, NULL, AUTH_TYPE, pstConnectParam->iIpFamily);			
	if ( ret != 0 )
	{
		LTE_LOG(LTE_LOG_ERR,"ndis_connect failed,ret=%d.\n",ret);	
		ndis_close(fd);
		return -1;
	}
    	else
	{
		LTE_LOG(LTE_LOG_INFO,"ndis_connect success.\n");
	}
	
	while ( iCounter-- )
	{
		ret = ndis_get_status(fd,&pipinfo);
		if( ret != 0 )
		{
			LTE_LOG(LTE_LOG_ERR,"ndis_get_status failed,ret=%d.\n",ret);					
		}
        	else
		{
			LTE_LOG(LTE_LOG_INFO,"ndis_get_status success.\n");
            
            if (NDIS_CONNECTED == pipinfo.i32status_ipv6)
            {
				sleep(5);
                system("ifconfig usb0 down up");
                LTE_LOG(LTE_LOG_INFO,"connected to internet success\n");
                iIPV6Flag = 1;
            }

			if (NDIS_CONNECTED == pipinfo.i32status_ipv4)
    		{
        		//system_call("ifconfig usb0 0.0.0.0");
        		//system_call("killall -9 udhcpc"); 
        		/*altered by caibing 2014-09-16*/
                system("ifconfig usb0 0.0.0.0");
                
                system("kill -9 $(pidof "DHCLIENT")");

                system("killall dhclient");

                system("kill -9 $(pidof "DHCPC")");

                system("killall udhcpc");    

			   	LTE_LOG(LTE_LOG_INFO,"connected to internet success\n");
    
    			//system_call("udhcpc -i usb0 -s /etc/udhcpc.script &"); 
    			    /*altered by caibing 2014-09-16 begin*/
                if (NULL != (pFile = fopen(acFileName, "rb")))
                {
                    while (fgets(acBuf, sizeof(acBuf), pFile) != NULL )
                    {     
                        cDeviceMode = acBuf[0];
                        memset(acBuf,0,sizeof(acBuf));
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
                }    /*altered by caibing 2014-09-16 end*/
				iIPV4Flag = 1;
            }
            
            if((1 == iIPV4Flag)||(1 == iIPV6Flag))
            {
    			ndis_close(fd);
				return 0;
			}
		}
		usleep(500000);
	}

	if((LTE_CONNECT_IPV4_TYPE == pstConnectParam->iIpFamily)&&(NDIS_CONNECTING == pipinfo.i32status_ipv4))
	{
		ret = extern_ndis_disconnect(fd, pstConnectParam->iIpFamily);
		if( ret != 0 )
		{
			LTE_LOG(LTE_LOG_ERR,"ndis_disconnect failed,ret=%d.\n",ret);	
		}
        	else
		{
			LTE_LOG(LTE_LOG_INFO,"ndis_disconnect success.\n");
		}	
	}
	
	ndis_close(fd);
	return -1;
}

static int longcheer_cereg_get(char *pcCmd, char *pcCiev)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};
    LTE_NETWORK_PREPARE_T stNwStatusInfo;
    int iCreg = 0;
    
    LTE_LOG(LTE_LOG_ERR,"%d,%s:zte_micro_b57_cereg_get failed\n",__LINE__,__func__);
    
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

static LTE_RET_E longcheer_network_prepare(void *pvData, int iDataLen)
{
	int iRegsiterStatus = 0;
    char acNeedleCgreg[16] = "+CGREG";
    char acNeedleCereg[16] = "+CEREG";
    
    char acCmdCgreg[16] =  "at+cgreg?";
    char acCmdCereg[16] =  "at+cereg?";
    
    iRegsiterStatus = longcheer_cereg_get(acCmdCereg, acNeedleCereg);
    if ((1 == iRegsiterStatus) || (5 == iRegsiterStatus))
    {
        return LTE_OK;
    }
    iRegsiterStatus = longcheer_cereg_get(acCmdCgreg, acNeedleCgreg);
    if ((1 == iRegsiterStatus) || (5 == iRegsiterStatus))
    {
        return LTE_OK;
    }
    return LTE_FAIL;
}

static LTE_RET_E longcheer_connect(void *pvData, int iDataLen)
{
	//LTE_LOG(LTE_LOG_ERR,"%d,%s:begin connect\n",__LINE__,__func__);
	LTE_APN_T *pstApn = (LTE_APN_T *)pvData;
    LTE_CONNECT_PARAM_T stConnectParam;
    char  acApn[32] = {0};
    char  acPDPStack[16] = {0};
    int   iIPFamily = LTE_CONNECT_IPV4_TYPE;    
    if (pvData == NULL)
		return LTE_FAIL;

    if(NULL != strstr(pstApn->acApn, "+"))
    {
        sscanf(pstApn->acApn, "%[^+]+%s", acApn, acPDPStack);
        if(NULL != strstr(acPDPStack, "v4v6"))
        {
            iIPFamily = LTE_CONNECT_IPV4V6_TYPE;
        }
        else if(NULL != strstr(acPDPStack, "v4"))
        {
            iIPFamily = LTE_CONNECT_IPV4_TYPE;
        }
        else if(NULL != strstr(acPDPStack, "v6"))
        {
            iIPFamily = LTE_CONNECT_IPV6_TYPE;
        }
    }
    else
    {
        strncpy(acApn, pstApn->acApn, sizeof(acApn));
        iIPFamily = LTE_CONNECT_IPV4_TYPE;
    }
    
    strncpy(stConnectParam.acApn, acApn, sizeof(stConnectParam.acApn));
    stConnectParam.iIpFamily = iIPFamily;
    /*altered by caibing 2014-09-22*/
	#if 1
	if (g_connect_flag)
    {
		/* connecting or connected ... , don't do it again */
		return LTE_OK;
	}
    #endif
	g_connect_flag = 1;

    /*走longcheer的拨号流程*/
    if(-1 == longcheer_begin_connect(&stConnectParam)){
		g_connect_flag = 0;
		g_connect_fail = 1;
		return LTE_FAIL;
    }
    else
    {
        
        g_connect_flag = 2;
		g_connect_fail = 0;
		return LTE_OK;
	}

}

/*****************************************************************************
 函 数 名  : pal_bsp_lte_disconnect
 功能描述  : 断开网络连接
 输入参数  : void *pvData    
             int iDataLen  
 输出参数  : 无
 返 回 值  : LTE_RET_E
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年10月25日星期四
    作    者   : 陈键
    修改内容   : 新生成函数

*****************************************************************************/

/*altered by caibing 2013-11-25*/
static int longcheer_begin_disconnect(int iIPFamily)
{
    /*altered by caibing 2014-09-23*/
	g_connect_flag = 0;
	g_connect_fail = 0;
    	int ret = -1;
	ndis_ipinfo pipinfo;
	int disnnecting_timeout = DISCONNECT_TIME_OUT;
	int fd;

	system_call("ifconfig usb0 up");

	fd = ndis_open();
	if(fd == -1)
	{
		LTE_LOG(LTE_LOG_ERR,"%d,%s:ndis_open failed\n",__LINE__,__func__);
		return -1;
	}
	
	ret = ndis_get_status(fd,&pipinfo);	
	if(ret<0)
	{
		LTE_LOG(LTE_LOG_ERR,"%d,%s:init failed.\n",__LINE__,__func__);
		goto EXIT_PROGRAM;	
	}else{
		if ((pipinfo.i32status_ipv4 == NDIS_DISCONNECTED) && (iIPFamily == LTE_CONNECT_IPV4_TYPE)){
			LTE_LOG(LTE_LOG_WARNING,"%d,%s:it is already disconnect.\n",__LINE__,__func__);
			ndis_close(fd);
			return 0;
		}
        if ((pipinfo.i32status_ipv6 == NDIS_DISCONNECTED) && (iIPFamily == LTE_CONNECT_IPV6_TYPE)){
			LTE_LOG(LTE_LOG_WARNING,"%d,%s:it is already disconnect.\n",__LINE__,__func__);
			ndis_close(fd);
			return 0;
		}
        if((iIPFamily == LTE_CONNECT_IPV4V6_TYPE) && ((pipinfo.i32status_ipv4 == NDIS_DISCONNECTED) || (pipinfo.i32status_ipv6 == NDIS_DISCONNECTED)))
        {
            LTE_LOG(LTE_LOG_WARNING,"%d,%s:it is already disconnect.\n",__LINE__,__func__);
			ndis_close(fd);
			return 0;
        }
	}
	
	ret = extern_ndis_disconnect(fd, iIPFamily);
	if(ret!=0)
	{
		LTE_LOG(LTE_LOG_ERR,"%d,%s:ndis_disconnect failed,ret=%d.\n",__LINE__,__func__, ret);
		goto EXIT_PROGRAM;		
	}
    else
	{
		LTE_LOG(LTE_LOG_INFO,"%d,%s:ndis_disconnect success.\n",__LINE__,__func__);
		//system_call("ifconfig usb0 0.0.0.0");
		//system_call("killall -9 udhcpc"); 
		/*altered by caibing 2014-09-16*/
        system("ifconfig usb0 0.0.0.0");
        
        system("kill -9 $(pidof "DHCLIENT")");

        system("killall dhclient");

        system("kill -9 $(pidof "DHCPC")");

        system("killall udhcpc");    

		g_connect_flag = 0;
	}		
	
	while(disnnecting_timeout--)
	{
		ret = ndis_get_status(fd,&pipinfo);
		if(ret!=0)
		{
			LTE_LOG(LTE_LOG_ERR,"%d,%s:ndis_get_status failed,ret=%d.\n",__LINE__,__func__,ret);
		}else{
			if ((pipinfo.i32status_ipv4 == NDIS_DISCONNECTED) || (pipinfo.i32status_ipv6 == NDIS_DISCONNECTED)){
				ndis_close(fd);
				return 0;
			}
		}
		usleep(500000);
	}

EXIT_PROGRAM:	
	ndis_close(fd);
    	return -1;
}

static LTE_RET_E longcheer_disconnect(void *pvData, int iDataLen)
{
    int iIPFamily = 4;
    char *pcIPFamily = (char *)pvData;
    
    if(NULL == pvData)
    {
        iIPFamily = LTE_CONNECT_IPV4_TYPE;
    }
    else
    {
        if(NULL != strstr(pcIPFamily, "v4v6"))
        {
            iIPFamily = LTE_CONNECT_IPV4V6_TYPE;
        }
        else if(NULL != strstr(pcIPFamily, "v4"))
        {
            iIPFamily = LTE_CONNECT_IPV4_TYPE;
        }
        else if(NULL != strstr(pcIPFamily, "v6"))
        {
            iIPFamily = LTE_CONNECT_IPV6_TYPE;
        }
    }
    if(-1 == longcheer_begin_disconnect(iIPFamily))
		return LTE_FAIL;
	else
		return LTE_OK;

}

static LTE_RET_E longcheer_sim_state_get(void *pvData, int iDataLen)
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

static LTE_RET_E longcheer_cellinfo_get(char *acCiev)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+lctcellinfo";
    //char acCiev[] = "CELL_ID:";
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
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
    stLteMsg.iComReadType = COM_READ_TYPE_6;
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

static LTE_RET_E longcheer_lteinfo_analyze(char *acFilePath, LTE_CELLINFO_T *pstCellInfo)
{
    FILE  *pstFd = NULL;
    int band;
	char acBuff[64] = {0};
    char acRssi[16] = {0};
    char acRsrp[16] = {0};
    char acRsrq[16] = {0};
    char acSinr[16] = {0};
    char acband[16] = {0};
    char acChannel[16] = {0};
    
    pstFd = fopen(acFilePath,"r");
    if (NULL == pstFd)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:open %s failed\n",__LINE__,__func__,acFilePath);
        return LTE_FAIL;
    }
    while (NULL != fgets(acBuff,64,pstFd))
    { 	
        if(NULL != strstr(acBuff, "CELL_ID:"))
        {
            sscanf(acBuff, "%*[^:]:%s", pstCellInfo->acCellId);
        }
        else if(NULL != strstr(acBuff, "RSSI:"))
        {
            sscanf(acBuff, "%*[^:]:%s", acRssi);
            pstCellInfo->iRssi = -1 * atoi(acRssi);
        }
        else if(NULL != strstr(acBuff, "RSRP:"))
        {
            sscanf(acBuff, "%*[^:]:%s", acRsrp);
            pstCellInfo->iRsrp = atoi(acRsrp);
        }
        else if(NULL != strstr(acBuff, "RSRQ:"))
        {
            sscanf(acBuff, "%*[^:]:%s", acRsrq);
            pstCellInfo->iRsrq = atoi(acRsrq);
        }
        else if(NULL != strstr(acBuff, "SINR:"))
        {
            sscanf(acBuff, "%*[^:]:%s", acSinr);
            pstCellInfo->iSinr = atoi(acSinr)/5 - 20;
        }
        else if(NULL != strstr(acBuff, "BAND:"))
        {
            sscanf(acBuff, "%*[^:]:%s", acband);
            band = atoi(acband);
        	if (band > 119 && band < 163) { // LTE bands
        		band -= 119;
        		sprintf(pstCellInfo->acBand, "%d", band);
        	}
        	else if (band > 162) { // TD-SCDMA bands
        		pstCellInfo->acBand[0] = band - 163 + 'A';
        		pstCellInfo->acBand[1] = '\0';
        	}
        }
        else if(NULL != strstr(acBuff, "EARFCN_DL:"))
        {
            sscanf(acBuff, "%*[^:]:%s", acChannel); 
            pstCellInfo->iChannel = atoi(acChannel);
        }
        
        memset(acBuff,0,64);
    }
    strncpy(pstCellInfo->acPhyCeillid, "N/A", sizeof(pstCellInfo->acPhyCeillid)); /*空的置为n/a*/
    fclose(pstFd);
    lte_file_unlink(acFilePath);

    return LTE_OK;
   
}

static LTE_RET_E longcheer_lte_info_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+lctcellinfo";
    char acCiev[] = "OK";
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

	LTE_INFO_T *pstLteInfo = (LTE_INFO_T *)pvData;
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
        iRet = longcheer_lteinfo_analyze(LTE_LCTCELLINFO_FILE, &(pstLteInfo->stCellInfo));
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

static LTE_RET_E longcheer_imsi_get(void *pvData, int iDataLen)
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

    /*at+cimi
       460007643159800*/
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

static LTE_RET_E longcheer_access_net_get(void *pvData, int iDataLen)
{
    int iRet = 0;
    int iClientSocketFd = -1;
    char acCmd[] = "at+psrat";
    char acCiev[] = "+PSRAT:";
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

static LTE_RET_E longcheer_bands_get(void *pvData, int iDataLen)
{
    LTE_MODULE_VERSION_T pstVersion = {0};
	LTE_BANDS_T *pstBands = (LTE_BANDS_T *)pvData;
	/* no AT command supported, use software version to distinguish them */
	/* 9105: 7,38,39,40 */
	/* 9115: 1,3,5,7,8,38,39,40,41 */
	char bands_9105[] = "7,38,39,40";
    char bands_9106[] = "1,3,5,7,8";
	char bands_9115[] = "1,3,7,8,38,39,40,41";
	if (pvData == NULL)
		return LTE_FAIL;

	if( LTE_FAIL == longcheer_version_get(&pstVersion, sizeof(LTE_MODULE_VERSION_T) ))
	{
	    LTE_LOG(LTE_LOG_ERR,"%d,%s:get version fail",__LINE__,__func__);
	    return LTE_FAIL;
	}

    if( pstVersion.iVersionIndex == LTE_LONCHEER_9105)
	{
		strncpy(pstBands->bands, bands_9105, sizeof(pstBands->bands));
	}
	else if( pstVersion.iVersionIndex == LTE_LONCHEER_9106)
	{
		strncpy(pstBands->bands, bands_9106, sizeof(pstBands->bands));
	}
	else if( pstVersion.iVersionIndex == LTE_LONCHEER_9115)
	{
		strncpy(pstBands->bands, bands_9115, sizeof(pstBands->bands));
	}
	else
	{
		LTE_LOG(LTE_LOG_ERR,"%d,%s:unknown version\n",__LINE__,__func__);
	    return LTE_FAIL;
	}
	
    pstBands->iClcBandAble = CLC_BAND_ENABLE_ONE;
	return LTE_OK;
    
}

static LTE_RET_E longcheer_clck_bands(char *pcAtCmd, char *pcCiev)
{
    int iClientSocketFd = -1;
    int iRet = 0;    
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if ((NULL == pcAtCmd) || (NULL == pcCiev))
    {
        return LTE_FAIL;
    }
    
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
	strncpy(stLteMsg.acAtCmd, pcAtCmd, sizeof(stLteMsg.acAtCmd));
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


static LTE_RET_E longcheer_clck_bands_set(void *pvData, int iDataLen)
{
    int iRet;    
    int iBandValue = 0;
    int iTDDFlag = 0;
    int iElseFlag= 0;
    char acCmd[64] = {0};
    char acCmdTmp[64] = {0};
    char acCiev[] = "OK";
    LTE_MODULE_VERSION_T pstVersion = {0};   
    LTE_BANDS_T *pstBands = (LTE_BANDS_T *)pvData;

    if (pvData == NULL)
    {
		return LTE_FAIL;
    }
    
    if( LTE_FAIL == longcheer_version_get(&pstVersion, sizeof(LTE_MODULE_VERSION_T) ))
	{
	    LTE_LOG(LTE_LOG_ERR,"%d,%s:get version fail",__LINE__,__func__);
	    return LTE_FAIL;
	}
	
    if( pstVersion.iVersionIndex == LTE_LONCHEER_9105) /* 9105模块锁频 */ /* AT+LCTMOBAPREF=xxx,xxx */
    {
        if(0 == strlen(pstBands->bands))
        {
            strcpy(pstBands->bands, LONGCHEER_BAND_DEFAULT);
        }

        if(0 == strcmp(pstBands->bands, LONGCHEER_BAND_DEFAULT))
        {
            iBandValue = 0; /* 打开当前设备所有支持的LTE频率 */
        }
        else
        {
            if(NULL != strstr(pstBands->bands, "7"))
            {
                iBandValue = 1; 
            }
            else if(NULL != strstr(pstBands->bands, "38"))
            {
                iBandValue = 2; 
            }
            else if(NULL != strstr(pstBands->bands, "39"))
            {
                iBandValue = 3; 
            }
            else if(NULL != strstr(pstBands->bands, "40"))
            {
                iBandValue = 4; 
            }
            else
            {
                LTE_LOG(LTE_LOG_INFO, "%d,%s:longcheer do not support this band\n",__LINE__,__func__);
                return LTE_FAIL; 
            }
        }
        /* AT+LCTMOBAPREF */
        sprintf(acCmd, "at+lctmobapref=5,%d\r", iBandValue);
    }
    else if( pstVersion.iVersionIndex == LTE_LONCHEER_9115) /*9115，只考虑单锁*/
    {
        if(0 == strlen(pstBands->bands))
        {
            strcpy(pstBands->bands, LONGCHEER_BAND_DEFAULT);
        }

        if(0 == strcmp(pstBands->bands, LONGCHEER_BAND_DEFAULT))
        {
            sprintf(acCmd, "at+nvrw=1,6828,\"%s\"\r", "C5000000E001");
        }
        else
        {
            if(2 == strlen(pstBands->bands))
            {
                if(NULL != strstr(pstBands->bands, "38"))
                {
                    iTDDFlag = 1;
                    sprintf(acCmd, "at+nvrw=1,6828,\"%s\"\r", "000000002000");
                }
                else if(NULL != strstr(pstBands->bands, "39"))
                {
                    iTDDFlag = 1;
                    sprintf(acCmd, "at+nvrw=1,6828,\"%s\"\r", "000000004000");
                }
                else if(NULL != strstr(pstBands->bands, "40"))
                {
                    iTDDFlag = 1;
                    sprintf(acCmd, "at+nvrw=1,6828,\"%s\"\r", "000000008000");
                }
                else if(NULL != strstr(pstBands->bands, "41"))
                {
                    iTDDFlag = 1;
                    sprintf(acCmd, "at+nvrw=1,6828,\"%s\"\r", "000000000001");
                }
                else
                {
                    LTE_LOG(LTE_LOG_INFO, "%d,%s:longcheer 9115 do not support this band\n",__LINE__,__func__);
                    return LTE_FAIL; 
                }
                    
            }
            else if(1 == strlen(pstBands->bands))
            {
                if(NULL != strstr(pstBands->bands, "1"))
                {
                    sprintf(acCmd, "at+nvrw=1,6828,\"%s\"\r", "01");
                }
                else if(NULL != strstr(pstBands->bands, "3"))
                {
                    sprintf(acCmd, "at+nvrw=1,6828,\"%s\"\r", "04");
                }
                else if(NULL != strstr(pstBands->bands, "7"))
                {
                    sprintf(acCmd, "at+nvrw=1,6828,\"%s\"\r", "40");
                }
                else if(NULL != strstr(pstBands->bands, "8"))
                {
                    sprintf(acCmd, "at+nvrw=1,6828,\"%s\"\r", "80");
                }
                else
                {
                    LTE_LOG(LTE_LOG_INFO, "%d,%s:longcheer 9115 do not support this band\n",__LINE__,__func__);
                    return LTE_FAIL; 
                }
            }
            else
            {
                LTE_LOG(LTE_LOG_INFO, "%d,%s: 9115 only support clck one band at the same time\n",__LINE__,__func__);
                return LTE_FAIL; 
            }
                
        }
        
    }
    else if( pstVersion.iVersionIndex == LTE_LONCHEER_9106)
    {
        if(0 == strlen(pstBands->bands))
        {
            strcpy(pstBands->bands, LONGCHEER_BAND_DEFAULT);
        }

        if(0 == strcmp(pstBands->bands, LONGCHEER_BAND_DEFAULT))
        {
            sprintf(acCmd, "at+nvrw=1,6828,\"%s\"\r", "D5000000A0000");
        }
        else
        {
            if(NULL != strstr(pstBands->bands, "3"))
            {
                iElseFlag = 1;
                iBandValue = iBandValue + 0x04;
                sprintf(acCmd, "at+nvrw=1,6828,\"%02x\"\r", iBandValue);
            }
            if(NULL != strstr(pstBands->bands, "8"))
            {
                iElseFlag = 1;
                iBandValue = iBandValue + 0x80;
                sprintf(acCmd, "at+nvrw=1,6828,\"%02x\"\r", iBandValue);
            }

            if(iElseFlag == 0)
            {
                LTE_LOG(LTE_LOG_INFO, "%d,%s:longcheer 9115 do not support this band\n",__LINE__,__func__);
                return LTE_FAIL; 
            }
        }
    }
    else
    {
    	LTE_LOG(LTE_LOG_INFO, "%d,%s:this lte version do not support clck bands\n",__LINE__,__func__);
        return LTE_OK; 
    }
    
    LTE_LOG(LTE_LOG_INFO, "%d,%s:acCmd:%s\n",__LINE__,__func__, acCmd);
    if( 1 == iTDDFlag)  /*9115锁TDD时要写入该值，该值对锁FDD无影响*/
    {
        strncpy(acCmdTmp, "at+nvrw=1,10,\"001E\"\r", sizeof(acCmdTmp));
        iRet = longcheer_clck_bands(acCmdTmp, acCiev);
        if (iRet != LTE_OK)
        {
            LTE_LOG(LTE_LOG_ERR,"%d,%s:longcheer_clck_bands failed\n",__LINE__,__func__);
            return LTE_FAIL;
        }
    }
    iRet = longcheer_clck_bands(acCmd, acCiev);
    if (iRet != LTE_OK)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:longcheer_clck_bands failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    return LTE_OK;
}

static LTE_RET_E longcheer_clck_bands_close(void *pvData, int iDataLen)
{
    int iRet;
    LTE_BANDS_T pstBands = {0};
    strcpy(pstBands.bands, "default");
    iRet = longcheer_clck_bands_set(&pstBands, sizeof(LTE_BANDS_T));
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:longcheer_clck_bands_set failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    return LTE_OK; 
}


/*****************************************************************************
 函 数 名  : longcheer_nettype_info_get
 功能描述  : 获取模块支持的网络类型
 输入参数  : void *pvData  
             int iDataLen  
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年11月12日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E longcheer_sup_nettype_get(void *pvData, int iDataLen)
{
    LTE_NET_TYPE_T *pstNets = (LTE_NET_TYPE_T *)pvData;
	
	if (pvData == NULL)
		return LTE_FAIL;
    
    pstNets->enNetType = LTE_2G_ONLY| LTE_3G_ONLY | LTE_4G_ONLY | LTE_2G_3G_4G;
    pstNets->enNetTypePri = LTE_NONE;
    return LTE_OK;
}

static LTE_RET_E longcheer_clckcell_enable_get(void *pvData, int iDataLen)
{
	if (pvData == NULL)
			return LTE_FAIL;
    LTE_CLKCELL_ENABLE_T *pstClkCellEnable = (LTE_CLKCELL_ENABLE_T *)pvData;

    pstClkCellEnable->iClkCellEnable = 0; /*不支持锁小区*/

	return LTE_OK;
}


static LTE_RET_E longcheer_set_oper_info_type(void)
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
   函 数 名  : longcheer_oper_info_get
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
static LTE_RET_E longcheer_oper_info_get(void *pvData, int iDataLen)
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
    iRet = longcheer_set_oper_info_type();
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s: zte_micro_b57_set_oper_info_type failed\n",__LINE__,__func__);
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


static LTE_RET_E longcheer_sms_sup_func(void *pvData, int iDataLen)
{
    int flag = 0;
    LTE_SUPPORT_SMS_T *pstSup = (LTE_SUPPORT_SMS_T *)pvData;
    pstSup->enSupSms = NO_SUP_SMS_FUNC;
    return LTE_OK;

}
static LTE_RET_E longcheer_cgdcont_set(void *pvData, int iDataLen)
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
            snprintf(acCmd, sizeof(acCmd), "at+cgdcont=1,\"IP\",\"%s\"\r", pstPdpSet->acApn);
        }
        else if(PDP_IPV6 == pstPdpSet->enPdpStack)
        {
            snprintf(acCmd, sizeof(acCmd), "at+cgdcont=1,\"IPV6\",\"%s\"\r", pstPdpSet->acApn);
        }
        else if(PDP_IPV4V6 == pstPdpSet->enPdpStack)
        {
            snprintf(acCmd, sizeof(acCmd), "at+cgdcont=1,\"IPV4V6\",\"%s\"\r", pstPdpSet->acApn);
        }
    }
    else
    {
        if(PDP_IPV4 == pstPdpSet->enPdpStack)
        {
            snprintf(acCmd, sizeof(acCmd), "at+cgdcont=1,\"IP\"\r");
        }
        else if(PDP_IPV6 == pstPdpSet->enPdpStack)
        {
            snprintf(acCmd, sizeof(acCmd), "at+cgdcont=1,\"IPV6\"\r");
        }
        else if(PDP_IPV4V6 == pstPdpSet->enPdpStack)
        {
            snprintf(acCmd, sizeof(acCmd), "at+cgdcont=1,\"IPV4V6\"\r");
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

static LTE_RET_E longcheer_apn_set(void *pvData, int iDataLen)
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
	
	snprintf(acCmd, sizeof(acCmd), "at+cgdcont=1,\"IP\",\"%s\"", pstApn->acApn);

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
static LTE_RET_E longcheer_version_get(void *pvData, int iDataLen)
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


static LTE_RET_E longcheer_enodeb_id_get(void *pvData, int iDataLen)
{
    int iRet;
    LTE_CELLID_T pstCellid = {0};
    unsigned long ulTmp;

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    LTE_ENODB_ID_T *pstEnodebId = (LTE_ENODB_ID_T *)pvData;
    
    iRet = longcheer_cellid_get(&pstCellid, sizeof(LTE_CELLID_T));
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s: longcheer_cellid_get fail\n",__LINE__,__func__ );
        return LTE_FAIL;
    }

    ulTmp = strtoul(pstCellid.acCellID, NULL, 16);
    LTE_LOG(LTE_LOG_INFO,"%d,%s: ulTmp:%d\n",__LINE__,__func__,ulTmp );
    iRet = snprintf(pstEnodebId->acEnodebId, sizeof(pstEnodebId->acEnodebId), "%u", (ulTmp >> 8));
    if (iRet <= 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s: snprintf fail\n",__LINE__,__func__ );
        return LTE_FAIL;
    }

    return LTE_OK;
}

/*****************************************************************************
 函 数 名  : longcheer_tac_get
 功能描述  : 获取小区所属的跟踪区位码 2字节
 输入参数  : void *pvData  
             int iDataLen  
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年2月3日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E longcheer_tac_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+lctcellinfo";
    char acCiev[] = "LAC_ID:";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_TAC_T *pstTac = (LTE_TAC_T *)pvData;
    
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_TAC_GET;
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
        strncpy(pstTac->acTac, stLteRecvMsg.uLteCommand.stTacInfo.acTac, sizeof(pstTac->acTac));
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;  
   
}

static LTE_RET_E longcheer_lcid_get(void *pvData, int iDataLen)
{
    int iRet;
    LTE_CELLID_T pstCellid = {0};
    unsigned long ulLcid;

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    LTE_LCID_T *pstLcid = (LTE_LCID_T *)pvData;
    
    iRet = longcheer_cellid_get(&pstCellid, sizeof(LTE_CELLID_T));
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s: longcheer_cellid_get fail\n",__LINE__,__func__ );
        return LTE_FAIL;
    }

    ulLcid = strtoul(pstCellid.acCellID, NULL, 16);
    iRet = snprintf(pstLcid->acLcid, sizeof(pstLcid->acLcid), "%u", (ulLcid & LTE_LCID_MASK));
    if (iRet <= 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s: snprintf fail\n",__LINE__,__func__ );
        return LTE_FAIL;
    }

    return LTE_OK;
}

static LTE_RET_E longcheer_set_cereg_return_type(int iValue, int iFuncCmd)
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


static LTE_RET_E longcheer_all_cellid_get(void *pvData, int iDataLen)
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
    iRet = longcheer_set_cereg_return_type(2, LTE_ALL_CELLID_GET);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s: longcheer_set_cereg_return_type [2] failed\n",__LINE__,__func__);
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

    if(LTE_OK != longcheer_set_cereg_return_type(0, LTE_ALL_CELLID_GET) )
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s: longcheer_set_cereg_return_type [0] failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    return iRet;
}

static LTE_RET_E longcheer_mac_set(void *pvData, int iDataLen)
{
    return LTE_OK;
}

static LTE_RET_E longcheer_sim_unlock_support(void *pvData, int iDataLen)
{
    LTE_UNLOCK_SUPPORT_T *pstUnlockSupport = (LTE_UNLOCK_SUPPORT_T* )pvData;
    if(NULL == pstUnlockSupport || iDataLen < sizeof(LTE_UNLOCK_SUPPORT_T))
    {
        return LTE_FAIL;
    }
    pstUnlockSupport->iUnlockFlag = 0;
    return LTE_OK;
}

static LTE_RET_E longcheer_module_up_support(void *pvData, int iDataLen)
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
 函 数 名  : longcheer_pin_input_remain
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

static LTE_RET_E longcheer_pin_input_remain(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "AT+CPNNUM";
    char acCiev[] = "OK";
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
        pstPinInputRemain->iPinIputRemain = stLteRecvMsg.uLteCommand.stPinInputRemain.iPinIputRemain;
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;

}



LTE_IOCTL_FUNC_T astLongCheerTable[] = 
{    
    {LTE_PIN_STATE,         longcheer_pin_state,            0,0,    "longcheer_pin_state"           },
    {LTE_PIN_SWITCH,        longcheer_pin_switch,           0,0,    "longcheer_pin_switch"          },
    {LTE_PIN_SW_GET,        longcheer_pin_sw_get,           0,0,    "longcheer_pin_sw_get"          },
    {LTE_PIN_UNLOCK,        longcheer_pin_unlock,           0,0,    "longcheer_pin_unlock"          },
    {LTE_PIN_ATTEMPTS,      longcheer_pin_attempts_get,    0,0,    "longcheer_ping_attempts_get"   },
    {LTE_PIN_ALTER,         longcheer_pin_alter,            0,0,    "longcheer_pin_alter"           },
    {LTE_SIGNAL_GET,        longcheer_signal_get,           0,0,    "longcheer_signal_get"          },
    {LTE_POWER_ON,          longcheer_power_on,             0,0,    "longcheer_power_on"            },
    {LTE_POWER_OFF,         longcheer_power_off,            0,0,    "longcheer_power_off"           },
    {LTE_NETTYPE_SET,       longcheer_nettype_set,         0,0,    "longcheer_nettype_set"        },    
    {LTE_NETTYPE_GET,       longcheer_nettype_get,          0,0,   "longcheer_nettype_get"         }, 
    {LTE_PLMN_REGISTER,     longcheer_plmn_register,        0,0,    "longcheer_plmn_register"       },    
    {LTE_PREFER_PLMN_SET,   longcheer_prefer_plmn_set,      0,0,    "longcheer_prefer_plmn_set"     },    
    {LTE_NETWORK_PREPARE,   longcheer_network_prepare,      0,0,    "longcheer_network_prepare"             }, 
    {LTE_NETWORK_STATUS_GET,NULL,           0,0,    NULL       },
    {LTE_CONNECT,           longcheer_connect,              0,0,    "longcheer_connect"             },    
    {LTE_DISCONNECT,        longcheer_disconnect,           0,0,    "longcheer_disconnect"          }, 
    {LTE_LINKSTATE_GET,     longcheer_linkstate_get,        0,0,   "longcheer_linkstate_get"       }, 
    {LTE_IMEI_GET,          longcheer_imei_get,             0,0,    "longcheer_imei_get"            }, 
    {LTE_CELLID_GET,        longcheer_cellid_get,           0,0,    "longcheer_cellid_get"          }, 	
    {LTE_ICCID_GET,         longcheer_iccid_get,           0,0,    "longcheer_iccid_get"       },
    {LTE_IMSI_GET,          longcheer_imsi_get,           0,0,    NULL       },
    {LTE_SIM_STATE_GET,     longcheer_sim_state_get,           0,0,    "longcheer_sim_state_get"          }, 
    {LTE_INFO_GET,          longcheer_lte_info_get,           0,0,    "longcheer_lte_info_get"       },
    {LTE_ACCESS_NET_GET,    longcheer_access_net_get,           0,0,    "longcheer_access_net_get"       }, 
    {LTE_BANDS_GET,         longcheer_bands_get,           0,0,    "longcheer_bands_get"       }, 
    {LTE_CLCK_BANDS_SET,    longcheer_clck_bands_set,           0,0, "longcheer_clck_bands_set"       }, 
    {LTE_CLCK_BANDS_CLOSE,  longcheer_clck_bands_close,        0,0,  "longcheer_clck_bands_close"       },
     /* BEGIN: Added by wujian, 2014/11/7   PN:添加获取运营商信息接口 */
    {LTE_OPER_INFO_GET,     longcheer_oper_info_get,           0,0,    "longcheer_oper_info_get"          }, 
     /* END:   Added by wujian, 2014/11/7   PN:添加获取运营商信息接口 */
    
     /* BEGIN: Added by wujian, 2014/11/12   PN:添加获取支持的网络类型，如支持2g，3g，4g */
    {LTE_NETTYPE_INFO_GET,  longcheer_sup_nettype_get,           0,0,    "longcheer_nets_info_get"          }, 
     /* END:   Added by wujian, 2014/11/12   PN:添加获取支持的网络类型，如支持2g，3g，4g */
    {LTE_CLCK_CELL_GET,     longcheer_clckcell_enable_get,       0,0,    "longcheer_clckcell_enable_get"   },
    {LTE_CLCK_CELL_SET,        NULL,           0,0,    NULL       },
    {LTE_GPS_START,        NULL,           0,0,    NULL       }, 
    {LTE_GPS_STOP,        NULL,           0,0,    NULL       }, 
    {LTE_GPS_DATE_GET,        NULL,           0,0,    NULL       }, 
    {LTE_GPS_POSITION_INFO_GET,        NULL,           0,0,    NULL       }, 
    /* BEGIN: Added by wujian, 2014/11/13   PN:添加短信功能相关接口 */
    {LTE_SMS_CENT_CODE_SET, NULL, 0, 0, NULL },
    {LTE_SMS_MODE__SET, NULL, 0, 0, NULL   },
    {LTE_SMS_FUNC_INIT, NULL, 0, 0, NULL   },
    {LTE_SMS_RECV, NULL, 0, 0, NULL   },
    {LTE_SMS_SEND, NULL, 0, 0, NULL   },
    {LTE_SMS_DELE, NULL, 0, 0, NULL   },
    {LTE_SMS_SUPP,          longcheer_sms_sup_func, 0, 0, "longcheer_sms_sup_func"   },
    /* END:   Added by wujian, 2014/11/13   PN:添加短信功能相关接口 */
    {LTE_IPV6_PDP_SET,      longcheer_cgdcont_set, 0, 0, "longcheer_cgdcont_set"   },
    /* BEGIN: Added by wujian, 2014/11/17   PN:添加设置apn接口 */
    {LTE_APN_SET,           longcheer_apn_set,           0,0,    "longcheer_apn_set"       }, 
    /* END:   Added by wujian, 2014/11/17   PN:添加设置apn接口 */
    {LTE_VERSION_GET,       longcheer_version_get,           0,0,    "longcheer_version_get"       },
    {LTE_ENODEBID_GET,      longcheer_enodeb_id_get,     0,0,  "longcheer_enodeb_id_get"       }, 
    {LTE_TAC_GET,           longcheer_tac_get,     0,0,  "longcheer_tac_get"       },
    {LTE_LCID_GET,          longcheer_lcid_get,     0,0,  "longcheer_lcid_get"       },
    {LTE_ALL_CELLID_GET,    longcheer_all_cellid_get,    0,0,  "longcheer_all_cellid_get"       },
    {LTE_MOUDLE_MAC_SET,    longcheer_mac_set,         0,0, "longcheer_mac_set"    },
    /* BEGIN: Added by wangqi, 2015/5/5   PN:添加设置机卡互锁接口 */
    {LTE_SIM_UNLOCK,         NULL, 0,0, NULL},
    {LTE_SIM_DISABLE_UNLOCK, NULL,     0,0, NULL},
    {LTE_SIM_UNLOCK_SUPPORT,longcheer_sim_unlock_support, 0,0, "longcheer_sim_unlock_support"},
    /* END:   Added by wangqi, 2015/5/5   PN:添加设置机卡互锁接口 */ 
    {LTE_MODULE_UP_SUPPORT, longcheer_module_up_support, 0,0, "longcheer_module_up_support"},
    {LTE_PIN_INPUT_REMAIN_GET,  longcheer_pin_input_remain, 0,0, "longcheer_pin_input_remain"},

    {LTE_CMD_MAX,        NULL,           0,0,    NULL       }, 
};
