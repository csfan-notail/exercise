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

#define ATCOM "/dev/ttyUSB2"

#define LEADCORE_BAND_DEFAULT         "default"
#define LEADCORE_BAND_DEFAULT_STR     "000001E000000044"     /* 默认支持的网段:3,7,38,39,40,41 */

#define LEADCORE_BAND_DEFAULT_VALUE_HIG   0x000001E0  /* 默认支持的网段:3,7,38,39,40,41 */
#define LEADCORE_BAND_DEFAULT_VALUE_LOW   0x00000044  

#define LEADCORE_BAND_3_VALUE_HIGH    0x00000000  /* band3对应的锁频值 */
#define LEADCORE_BAND_3_VALUE_LOW     0x00000004  

#define LEADCORE_BAND_38_VALUE_HIGH   0x00000020  /* band 38对应的锁频值 */
#define LEADCORE_BAND_38_VALUE_LOW    0x00000000  

#define LEADCORE_BAND_39_VALUE_HIGH   0x00000040  /* band 39对应的锁频值 */
#define LEADCORE_BAND_39_VALUE_LOW    0x00000000  

#define LEADCORE_BAND_40_VALUE_HIGH   0x00000080  /* band 40对应的锁频值 */
#define LEADCORE_BAND_40_VALUE_LOW    0x00000000  

#define LEADCORE_BAND_41_VALUE_HIGH   0x00000100   /* band 41对应的锁频值 */
#define LEADCORE_BAND_41_VALUE_LOW    0x00000000   


typedef enum
{
	LEADCORE_4G = 7,
	LEADCORE_3G = 2,
	LEADCORE_NONE = 0
}LEADCORE_NETTYPE_E;

static int g_connect_flag = 0;
static int g_network_prepare = 0;
static LTE_RET_E leadcore_version_get(void *pvData, int iDataLen);
static LTE_RET_E leadcore_apn_set(void *pvData, int iDataLen);
static LTE_RET_E leadcore_set_ipv6_prepare(void *pvData, int iDataLen);
static LTE_RET_E leadcore_cgdcont_set(void *pvData, int iDataLen);
static LTE_RET_E leadcore_cgdcont_2g3g_set(void *pvData, int iDataLen);
static LTE_RET_E recv_only_ok(char *pcCmd, LTE_CMD_E iFuncCmd, int iRealFlag);



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
static LTE_RET_E leadcore_pin_state (void *pvData, int iDataLen)
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
static LTE_RET_E leadcore_pin_switch (void *pvData, int iDataLen)
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
static LTE_RET_E leadcore_pin_sw_get( void *pvData, int iDataLen )
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
 static LTE_RET_E leadcore_pin_unlock (void *pvData, int iDataLen)
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
static LTE_RET_E leadcore_pin_attempts_get(void *pvData, int iDataLen)
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
static LTE_RET_E leadcore_pin_alter (void *pvData, int iDataLen)
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


static LTE_RET_E leadcore_power_on (void *pvData, int iDataLen)
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

static LTE_RET_E leadcore_power_off (void *pvData, int iDataLen)
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


static LTE_RET_E leadcore_signal_get  (void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cmer=2,0,0,2,0";
    char acCiev[] = "+CIEV: 2";
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
        pstSignal->uiSignal = stLteRecvMsg.uLteCommand.stSignalInfo.uiSignal;
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;

}

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
    #if 0
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
    #endif
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

static LTE_RET_E leadcore_linkstate_get(void *pvData, int iDataLen)
{
    int iRet = 0;
    int iClientSocketFd = -1;
    char acCmd[] = "at^dqdata=3,1";
    char acCiev[] = "^DQDATA";
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
        pstLinkstate->enLinkstate = stLteRecvMsg.uLteCommand.stLinkInfo.enLinkstate;
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;
    
}

static LTE_RET_E leadcore_imei_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at^dgsn?";
    char acCiev[] = "^DGSN:";
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

static LTE_RET_E leadcore_set_cereg_return_type(int iValue, int iFuncCmd)
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


static LTE_RET_E leadcore_cellid_get(void *pvData, int iDataLen)
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
    iRet = leadcore_set_cereg_return_type(2, LTE_CELLID_GET);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s: leadcore_set_cereg_return_type [2] failed\n",__LINE__,__func__);
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

    if(LTE_OK != leadcore_set_cereg_return_type(0, LTE_CELLID_GET))
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s: leadcore_set_cereg_return_type [0] failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    return iRet;


}

static LTE_RET_E leadcore_iccid_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at^dcid";
    char acCiev[] = "^DCID:";
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

static LTE_RET_E leadcore_imsi_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cimi";
    char acCiev[] = "+CIMI:";
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
        strncpy(pstImsiInfo->acIMSI, stLteRecvMsg.uLteCommand.stImsiInfo.acIMSI, sizeof(pstImsiInfo->acIMSI));
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;
    
}


static int leadcore_cereg_get(char *pcCmd, char *pcCiev)
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

static LTE_RET_E leadcore_network_prepare(void *pvData, int iDataLen)
{
	int iRegsiterStatus = 0;
    char acNeedleCgreg[16] = "+CGREG";
    char acNeedleCereg[16] = "+CEREG";
    char acNeedleCreg[16] = "+CREG";
    
    char acCmdCgreg[16] =  "at+cgreg?";
    char acCmdCereg[16] =  "at+cereg?";
    char acCmdCreg[16] =  "at+creg?";

	LTE_CGDCONT_T stPdpStack = {0};
	char acPDPStack[16] = {0};
	
    LTE_APN_T *pstApn = (LTE_APN_T *)pvData;
    if (g_network_prepare == 0 && iDataLen != 0)
    {
		if (LTE_OK != leadcore_power_off(NULL, 0))
		{
			LTE_LOG(LTE_LOG_ERR, "power off fail");
			return LTE_FAIL;
		}

		 if(NULL != strstr(pstApn->acApn, "+"))
        {
            sscanf(pstApn->acApn, "%[^+]+%s", stPdpStack.acApn, acPDPStack);
            if(NULL != strstr(acPDPStack,"v6"))
            {
                if(NULL != strstr(acPDPStack, "v4"))
                {
                    stPdpStack.enPdpStack = PDP_IPV4V6;
                    stPdpStack.iApnFlag = 1;
                }
                else
                {
                    stPdpStack.enPdpStack = PDP_IPV6;
                    stPdpStack.iApnFlag = 1;
                }
        	}

        	if(LTE_OK != leadcore_set_ipv6_prepare(NULL, 0))
        	{
                LTE_LOG(LTE_LOG_ERR,"%d,%s:leadcore_set_ipv6_prepare error!\n",__LINE__,__func__);
                return LTE_FAIL;
        	}
        }
        else
        {
            strncpy(stPdpStack.acApn, pstApn->acApn, sizeof(stPdpStack.acApn));
            stPdpStack.enPdpStack = PDP_IPV4;
            stPdpStack.iApnFlag = 1;
        }
        
        if (LTE_OK != leadcore_cgdcont_set((void *)&stPdpStack, sizeof(LTE_CGDCONT_T )))
		{
			LTE_LOG(LTE_LOG_ERR, "set apn fail");
			return LTE_FAIL;
		}

        if (LTE_OK != leadcore_cgdcont_2g3g_set((void *)&stPdpStack, sizeof(LTE_CGDCONT_T )))
		{
			LTE_LOG(LTE_LOG_ERR, "set 2g3g apn fail");
			return LTE_FAIL;
		}
        
		if (LTE_OK != leadcore_power_on(NULL, 0))
		{
			LTE_LOG(LTE_LOG_ERR, "power on fail");
			return LTE_FAIL;
		}
		recv_only_ok("AT+CGEQREQ=1,2,64,64,0,0,0,1500,\"0E0\",\"0E0\",,0,0", LTE_IPV6_PDP_SET, 0);
	}

    g_network_prepare++;
	if (g_network_prepare >= DEFAULT_SEARCH_NET_TIMES)
		g_network_prepare = 0;
    
    iRegsiterStatus = leadcore_cereg_get(acCmdCereg, acNeedleCereg);
    if ((1 == iRegsiterStatus) || (5 == iRegsiterStatus))
    {
        g_network_prepare = 0;
        return LTE_OK;
    }
    iRegsiterStatus = leadcore_cereg_get(acCmdCgreg, acNeedleCgreg);
    if ((1 == iRegsiterStatus) || (5 == iRegsiterStatus))
    {
        g_network_prepare = 0;
        return LTE_OK;
    }
    iRegsiterStatus = leadcore_cereg_get(acCmdCreg, acNeedleCreg);
    if ((1 == iRegsiterStatus) || (5 == iRegsiterStatus))
    {
        g_network_prepare = 0;
        return LTE_OK;
    }
    return LTE_FAIL;
}

static LTE_RET_E leadcore_connect_begin(char *pcCmd, char *pcCiev)
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

LTE_RET_E leadcore_connect(void *pvData, int iDataLen)
{
    int iRet;
    char acFileName[64] = {"/tmp/device_mode.tmp"};
    FILE *pFile = NULL;
    char acApn[32] = {0};
    char acBuf[32] = {0};
    char acPDPStack[16] = {0};
    char acLteCmd[32] = "at+cgdata=\"M-0000\",1";
    char acCiev[] = "CONNECT";
    char cDeviceMode = '0';
    if (g_connect_flag)
    {
		/* connecting or connected ... , don't do it again */
		return LTE_OK;
	}
     
    LTE_APN_T *pstApn = (LTE_APN_T *)pvData;
    if(NULL == pstApn)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:input param is null pointer!\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    system_call("ifconfig usb0 up"); 

    iRet = leadcore_connect_begin(acLteCmd, acCiev);
	if (iRet != LTE_OK)
	{
    	LTE_LOG(LTE_LOG_ERR,"%d,%s:at command failed\n",__LINE__,__func__);
        g_connect_flag = 0;
    	return LTE_FAIL;
	}
    else
    {
        g_connect_flag = 2;
        g_network_prepare = 0;
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
                
		        system("kill -9 $(pidof "DHCLIENT")");

		        system("killall dhclient");

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
                            
            system("kill -9 $(pidof "DHCLIENT")");
        
            system("killall dhclient");
            
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
    return LTE_OK;
    
}

static LTE_RET_E leadcore_disconnect(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
	int iRet = 0;
	char acCmd[64] = "at+cgatt=0";
	char acCiev[] = "OK";
	CLIENT_SEND_MSG_T stLteMsg = {0};
	LTE_SEND_MSG_T stLteRecvMsg = {0};

    g_connect_flag = 0;
    g_network_prepare = 0;
    
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
        system("ifconfig usb0 0.0.0.0");
    
        system("kill -9 $(pidof "DHCLIENT")");

        system("killall dhclient");

        system("kill -9 $(pidof "DHCPC")");

        system("killall udhcpc");  
	    iRet = LTE_OK;
	}
	else
	{
	   iRet = LTE_FAIL;
	}

    return iRet;
}

static LTE_RET_E leadcore_sim_state_get(void *pvData, int iDataLen)
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

static LTE_RET_E leadcore_qsqinfo_get(LTE_INFO_T *pstLteInfoTmp, int iDataLen)
{
	int iClientSocketFd = -1;
	int iRet = 0;
	char acCmd[] = "at^qsq?";
	char acCiev[] = "^QSQ:";
	
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
		pstLteInfoTmp->stCellInfo.iRssi= stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.iRssi;
		pstLteInfoTmp->stCellInfo.iSinr= stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.iSinr;
		
		iRet = LTE_OK;
	}
	else
	{
		iRet = LTE_FAIL;
	}
	
	return iRet;  
}

static LTE_RET_E leadcore_bandinfo_get(LTE_INFO_T *pstLteInfoTmp, int iDataLen)
{
	int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at^dqfreq";
    char acCiev[] = "^DQFREQ:";
    char *pcBuf = NULL;
    
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
		strncpy(pstLteInfoTmp->stCellInfo.acBand, stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.acBand, sizeof(pstLteInfoTmp->stCellInfo.acBand));
        strncpy(pstLteInfoTmp->stCellInfo.acPhyCeillid, stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.acPhyCeillid, sizeof(pstLteInfoTmp->stCellInfo.acPhyCeillid));
        pstLteInfoTmp->stCellInfo.iChannel = stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.iChannel;
        
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;

}

static LTE_RET_E leadcore_lte_info_get(void *pvData, int iDataLen)
{
    LTE_CELLID_T stCellid = {0};
    
	LTE_INFO_T *pstLteInfo = (LTE_INFO_T *)pvData;
    
	if (pvData == NULL)
		return LTE_FAIL;
    if (LTE_OK != leadcore_cellid_get(&stCellid, sizeof(LTE_CELLID_T)))
	{
        LTE_LOG(LTE_LOG_ERR, "get cell id fail");
        //return LTE_FAIL;
    }

	if (LTE_OK != leadcore_qsqinfo_get(pstLteInfo, sizeof(LTE_INFO_T)))
	{
		LTE_LOG(LTE_LOG_ERR, "get qsq info fail");
		//return LTE_FAIL;
	}
    if (LTE_OK != leadcore_bandinfo_get(pstLteInfo, sizeof(LTE_INFO_T)))
	{
        LTE_LOG(LTE_LOG_ERR, "get band info fail");
        //return LTE_FAIL;
    }
    
    strcpy(pstLteInfo->stCellInfo.acCellId, stCellid.acCellID);
  
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
static LTE_RET_E leadcore_access_net_get(void *pvData, int iDataLen)
{
    int iRet = 0;
    int iClientSocketFd = -1;
    char acCmd[] = "at^sysinfo";
    char acCiev[] = "^SYSINFO:";
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

static LTE_RET_E leadcore_clckcell_enable_get(void *pvData, int iDataLen)
{
    LTE_CLKCELL_ENABLE_T *pstClkCellEnable = (LTE_CLKCELL_ENABLE_T *)pvData;
    LTE_MODULE_VERSION_T stVersion = {0};

    if (pvData == NULL)
    {
		return LTE_FAIL;
    }
    if(LTE_OK != leadcore_version_get((void *)&stVersion, sizeof(LTE_MODULE_VERSION_T)))
    {
        return LTE_FAIL;
    }
    
    /* 不支持锁小区 */
    if(0 == strcmp(stVersion.acVersion, "BD_MD6200_HC_V1.0.0B05"))      
    {
        pstClkCellEnable->iClkCellEnable = 1;
    }
    else
    {
        pstClkCellEnable->iClkCellEnable = 0;
    }

    return LTE_OK;    
}
static LTE_RET_E leadcore_clck_cellid_set(void *pvData, int iDataLen)
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
        
    if (pvData == NULL)
		return LTE_FAIL;
    
    iPhyCellid = atoi(pstCellClkT->acCellid);
        
    if((1 == pstCellClkT->iClkEnable)&&(iPhyCellid > 503))
    {   
        LTE_LOG(LTE_LOG_ERR,"%d,%s:PHY cellid is too large, exceed 503!\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    if(0 == pstCellClkT->iClkEnable)
    {
        sprintf(acCmd, "AT^DPCILK=0"); /*解锁小区*/
    }
    else if(1 == pstCellClkT->iClkEnable)
    {
        sprintf(acCmd, "AT^DPCILK=1,%s,%s", pstCellClkT->acFreq, pstCellClkT->acCellid);
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

static LTE_RET_E leadcore_set_oper_info_type(void)
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
static LTE_RET_E leadcore_oper_info_get(void *pvData, int iDataLen)
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
    iRet = leadcore_set_oper_info_type();
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

static LTE_RET_E leadcore_sup_nettype_get(void *pvData, int iDataLen)
{
    LTE_NET_TYPE_T *pstNets = (LTE_NET_TYPE_T *)pvData;
    LTE_MODULE_VERSION_T stVersion = {0};
    
    if(LTE_OK != leadcore_version_get((void *)&stVersion, sizeof(LTE_MODULE_VERSION_T)))
    {
        return LTE_FAIL;
    }
    if(NULL != strstr(stVersion.acVersion, "CMCC"))
    {
        pstNets->enNetType = LTE_4G_ONLY | LTE_3G_ONLY | LTE_3G_4G;
        pstNets->enNetTypePri = LTE_4G_PRI | LTE_3G_PRI;
        return LTE_OK;
    }
    pstNets->enNetType = LTE_4G_ONLY;
    pstNets->enNetTypePri = LTE_NONE;
    return LTE_OK;

}

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
static LTE_RET_E leadcore_apn_set(void *pvData, int iDataLen)
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
	
	snprintf(acCmd, sizeof(acCmd), "at^ddpdn=1,\"%s\"", pstApn->acApn);

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
static LTE_RET_E leadcore_version_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cgmr";
    char acCiev[] = "+CGMR:";
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

static LTE_RET_E leadcore_enodeb_id_get(void *pvData, int iDataLen)
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
    iRet = leadcore_set_cereg_return_type(2, LTE_ENODEBID_GET);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s: leadcore_set_cereg_return_type [2] failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    LTE_ENODB_ID_T *pstEnodebId = (LTE_ENODB_ID_T *)pvData;
    
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
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
        strncpy(pstEnodebId->acEnodebId, stLteRecvMsg.uLteCommand.stEnodeBInfo.acEnodebId, sizeof(pstEnodebId->acEnodebId));
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }

    if(LTE_OK != leadcore_set_cereg_return_type(0, LTE_ENODEBID_GET) )
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s: leadcore_set_cereg_return_type [0] failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    return iRet;

}

/*****************************************************************************
 函 数 名  : leadcore_tac_get
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
static LTE_RET_E leadcore_tac_get(void *pvData, int iDataLen)
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
    iRet = leadcore_set_cereg_return_type(2, LTE_TAC_GET);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s: leadcore_set_cereg_return_type [2] failed\n",__LINE__,__func__);
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
        strncpy(pstTac->acTac, stLteRecvMsg.uLteCommand.stTacInfo.acTac, sizeof(pstTac->acTac));
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }

    if(LTE_OK != leadcore_set_cereg_return_type(0, LTE_TAC_GET) )
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s: leadcore_set_cereg_return_type [0] failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    return iRet;
}

static LTE_RET_E leadcore_lcid_get(void *pvData, int iDataLen)
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
    iRet = leadcore_set_cereg_return_type(2, LTE_LCID_GET);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s: leadcore_set_cereg_return_type [2] failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    LTE_LCID_T *pstLcid = (LTE_LCID_T *)pvData;
    
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
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
        strncpy(pstLcid->acLcid, stLteRecvMsg.uLteCommand.stLcidInfo.acLcid, sizeof(pstLcid->acLcid));
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }

    if(LTE_OK != leadcore_set_cereg_return_type(0, LTE_LCID_GET) )
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s: leadcore_set_cereg_return_type [0] failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    return iRet;
}

static LTE_RET_E leadcore_all_cellid_get(void *pvData, int iDataLen)
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
    iRet = leadcore_set_cereg_return_type(2, LTE_ALL_CELLID_GET);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s: leadcore_set_cereg_return_type [2] failed\n",__LINE__,__func__);
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

    if(LTE_OK != leadcore_set_cereg_return_type(0, LTE_ALL_CELLID_GET) )
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s: leadcore_set_cereg_return_type [0] failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    return iRet;
}

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

#if 0
static LTE_RET_E leadcore_mac_set(void *pvData, int iDataLen)
{
    int iRet = 0;
    char acCmd[64] = {0};
	LTE_MOUDLE_MAC_T *pstMac = (LTE_MOUDLE_MAC_T* )pvData;

    LTE_NORMAL_AT_T stAt = {"", "/tmp/apnset", "OK", 0, ATCOM, 0, 2};
    sprintf(acCmd, "at^qmac=\"%s\"\r", pstMac->acMac);
    strncpy(stAt.acCmd, acCmd, strlen(acCmd));

    /*执行at指令*/
    iRet = lte_normal_at_cmd(&stAt);
    if (iRet != 0)
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:at command failed\n", __LINE__, __func__);
        lte_file_unlink(stAt.acFileName);
        return LTE_FAIL;
    }

    if (0 != stAt.iResult)
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:compare AT return value not OK\n", __LINE__, __func__);
        lte_file_unlink(stAt.acFileName);
        return LTE_FAIL;
    }
    lte_file_unlink(stAt.acFileName);

    return LTE_OK;     
 
}

static LTE_RET_E leadcore_mac_get(void *pvData, int iDataLen)
{
    FILE* pstFd = NULL;
    char acBuff[64] = {0};
    int iRet = 0;
    char acCiev[] = "^QMAC:";
    char acVersion[64] = {0};
    
	LTE_MOUDLE_MAC_T *pstMac = (LTE_MOUDLE_MAC_T* )pvData;
    LTE_NORMAL_AT_T stAt = {"at^qmac?\r","/tmp/macget","^QMAC",0,ATCOM,0,2};

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
        return LTE_FAIL;
    }
   
    if (0 != stAt.iResult)
    {
        LTE_LOG(LTE_LOG_INFO,"%d,%s:LTE get version fail",__LINE__,__func__);
        lte_file_unlink(stAt.acFileName);
        return LTE_FAIL;
    }
    pstFd = fopen(stAt.acFileName,"r");
    if(NULL == pstFd)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:open %s failed\n",__LINE__,__func__,stAt.acFileName);
        lte_file_unlink(stAt.acFileName);
        return LTE_FAIL;
    }
    
    while(NULL != fgets(acBuff,64,pstFd))
    {   
        if(NULL != strstr(acBuff, acCiev))
        {
            sscanf(acBuff, "%*[^ ] %s", pstMac->acMac);
        }
        memset(acBuff,0,64);
    }
    fclose(pstFd);
    pstFd = NULL;
    lte_file_unlink(stAt.acFileName);

    return LTE_OK;
}
#endif

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


LTE_IOCTL_FUNC_T astLeadCoreTable[] = 
{    
    {LTE_PIN_STATE,         leadcore_pin_state,           0,0,    "leadcore_pin_state"       }, 
    {LTE_PIN_SWITCH,        leadcore_pin_switch,           0,0,    "leadcore_pin_switch"       }, 
    {LTE_PIN_SW_GET,        leadcore_pin_sw_get,           0,0,    "leadcore_pin_sw_get"       },
    {LTE_PIN_UNLOCK,        leadcore_pin_unlock,           0,0,    "leadcore_pin_unlock"       }, 
    {LTE_PIN_ATTEMPTS,      leadcore_pin_attempts_get,           0,0,    "leadcore_pin_attempts_get"       }, 
    {LTE_PIN_ALTER,         leadcore_pin_alter,           0,0,    "leadcore_pin_alter"       }, 
    {LTE_SIGNAL_GET,        leadcore_signal_get,           0,0,    "leadcore_signal_get"       }, 
    {LTE_POWER_ON,          leadcore_power_on,           0,0,    "leadcore_power_on"       }, 
    {LTE_POWER_OFF,         leadcore_power_off,           0,0,    "leadcore_power_off"       }, 
    {LTE_NETTYPE_SET,       leadcore_nettype_set,           0,0,    "leadcore_nettype_set"       }, 
    {LTE_NETTYPE_GET,       leadcore_nettype_get,           0,0,    "leadcore_nettype_get"       }, 
    {LTE_PLMN_REGISTER,     leadcore_plmn_register,           0,0,    "leadcore_plmn_register"       }, 
    {LTE_PREFER_PLMN_SET,   leadcore_prefer_plmn_set,           0,0,    "leadcore_prefer_plmn_set"       }, 
    {LTE_NETWORK_PREPARE,   leadcore_network_prepare,           0,0,    "leadcore_network_prepare"       }, 
    {LTE_NETWORK_STATUS_GET,NULL,           0,0,    NULL       },
    {LTE_CONNECT,           leadcore_connect,           0,0,    "leadcore_connect"       }, 
    {LTE_DISCONNECT,        leadcore_disconnect,           0,0,    "leadcore_disconnect"       }, 
    {LTE_LINKSTATE_GET,     leadcore_linkstate_get,           0,0,    "leadcore_linkstate_get"       }, 
    {LTE_IMEI_GET,          leadcore_imei_get,           0,0,    "leadcore_imei_get"       }, 
    {LTE_CELLID_GET,        leadcore_cellid_get,           0,0,    "leadcore_cellid_get"       }, 
    {LTE_ICCID_GET,         leadcore_iccid_get,           0,0,    "leadcore_iccid_get"       },
    {LTE_IMSI_GET,          leadcore_imsi_get,           0,0,    "leadcore_imsi_get"       },
    {LTE_SIM_STATE_GET,     leadcore_sim_state_get,           0,0,    "leadcore_sim_state_get"       }, 
    {LTE_INFO_GET,          leadcore_lte_info_get,           0,0,    "leadcore_lte_info_get"       }, 
    {LTE_ACCESS_NET_GET,    leadcore_access_net_get,           0,0,    "leadcore_access_net_get"       }, 
    {LTE_BANDS_GET,         leadcore_bands_get,           0,0,    "leadcore_bands_get"       }, 
    {LTE_CLCK_BANDS_SET,    leadcore_clck_bands_set,           0,0, "leadcore_clck_bands_set"       },
    {LTE_CLCK_BANDS_CLOSE,  leadcore_bands_close,            0,0,       NULL       },
    {LTE_CLCK_CELL_GET,     leadcore_clckcell_enable_get,           0,0,    "leadcore_clckcell_enable_get"   },
    {LTE_CLCK_CELL_SET,     leadcore_clck_cellid_set,           0,0,    NULL       },
    {LTE_OPER_INFO_GET,     leadcore_oper_info_get, 0, 0, "leadcore_oper_info_get"          },
    {LTE_NETTYPE_INFO_GET,  leadcore_sup_nettype_get, 0, 0, "leadcore_sup_nettype_get"          },
    {LTE_GPS_START,        NULL,           0,0,    NULL       }, 
    {LTE_GPS_STOP,         NULL,           0,0,    NULL       }, 
    {LTE_GPS_DATE_GET,        NULL,           0,0,    NULL       }, 
    {LTE_GPS_POSITION_INFO_GET,        NULL,           0,0,    NULL       }, 

    /* BEGIN: Added by wujian, 2014/11/13   PN:添加短信功能相关接口 */
    {LTE_SMS_CENT_CODE_SET, leadcore_sms_cent_code_set, 0, 0, "leadcore_sms_cent_code_set" },
    {LTE_SMS_MODE__SET,     leadcore_sms_mode_set, 0, 0, "leadcore_sms_mode_set"   },
    {LTE_SMS_FUNC_INIT,     leadcore_sms_func_init, 0, 0, "leadcore_sms_func_init"   },
    {LTE_SMS_RECV,          leadcore_sms_recv, 0, 0,        "leadcore_sms_recv"   },
    {LTE_SMS_SEND,          leadcore_sms_send, 0, 0,        "leadcore_sms_send"   },
    {LTE_SMS_DELE,          leadcore_sms_dele, 0, 0,        "leadcore_sms_dele"   },
    {LTE_SMS_SUPP,          leadcore_sms_sup_func, 0, 0,    "leadcore_sms_sup_func"   },
    /* END:   Added by wujian, 2014/11/13   PN:添加短信功能相关接口 */
    {LTE_IPV6_PDP_SET,      leadcore_cgdcont_set, 0, 0,     "leadcore_cgdcont_set"   },
    /* BEGIN: Added by wujian, 2014/11/17   PN:添加设置apn接口 */
    {LTE_APN_SET,           leadcore_apn_set,         0,0, "leadcore_apn_set"   },
    /* END:   Added by wujian, 2014/11/17   PN:添加设置apn接口 */
    {LTE_VERSION_GET,       leadcore_version_get,     0,0, "leadcore_version_get"       },
    {LTE_ENODEBID_GET,      leadcore_enodeb_id_get,   0,0, "leadcore_enodeb_id_get"    }, 
    {LTE_TAC_GET,           leadcore_tac_get,         0,0, "leadcore_tac_get"    },
    {LTE_LCID_GET,          leadcore_lcid_get,        0,0, "leadcore_lcid_get"    },
    {LTE_ALL_CELLID_GET,    leadcore_all_cellid_get,  0,0, "leadcore_all_cellid_get"    },
    {LTE_MOUDLE_MAC_SET,    leadcore_mac_set,         0,0, "leadcore_mac_set"    },
    {LTE_MOUDLE_MAC_GET,    leadcore_mac_get,         0,0, "leadcore_mac_get"    },
    /* BEGIN: Added by wangqi, 2015/5/5   PN:添加设置机卡互锁接口 */
    {LTE_SIM_UNLOCK,        leadcore_sim_unlock, 0,0,      "leadcore_sim_unlock"},
    {LTE_SIM_DISABLE_UNLOCK,leadcore_sim_disable_unlock, 0,0, "leadcore_sim_disable_unlock"},
    {LTE_SIM_UNLOCK_SUPPORT,leadcore_sim_unlock_support, 0,0, "leadcore_sim_unlock_support"},
    {LTE_SIM_TYPE_GET,      leadcore_sim_type_get, 0,0,    "leadcore_sim_type_get"},
    {LTE_SIM_UNLOCK_GET,    leadcore_sim_unlock_get, 0,0,  "leadcore_sim_unlock_get"},
    /* END:   Added by wangqi, 2015/5/5   PN:添加设置机卡互锁接口 */    
    {LTE_ALL_PLMN_MODE_GET,      leadcore_all_plmn_mode_get,  0,0, "leadcore_all_plmn_mode_get"    },
    {LTE_SOUNET_MODE_SET,   leadcore_sounet_mode_set,  0,0, "leadcore_sounet_mode_set"    },
    {LTE_SIM_UNLOCK_PUK,    leadcore_unlock_puk,  0,0, "leadcore_puk_unlock"    },
    {LTE_MODULE_UP_SUPPORT, leadcore_module_up_support, 0,0, "leadcore_module_up_support"},
    {LTE_PIN_INPUT_REMAIN_GET,  leadcore_pin_input_remain, 0,0, "leadcore_pin_input_remain"},

    
    {LTE_CMD_MAX,           NULL,           0,0,    NULL       }, 
};

