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

#define ATCOM "/dev/ttyUSB2"
#define DISCONNECT_TIME_OUT  5    //500ms*5
#define AUTH_TYPE 0

#define YUGA_GSM_ONLY 13
#define YUGA_TDS_ONLY 15
#define YUGA_WCD_ONLY 14
#define YUGA_LTE_ONLY 38
#define YUGA_GSM_TDS 60
#define YUGA_GSM_WCD 19
#define YUGA_TDS_LTE 65
#define YUGA_WCD_LTE 54
#define YUGA_GSM_TDS_LTE 61
#define YUGA_GSM_WCD_LTE 39

#define YUGA_BAND_DEFAULT   "default"
#define YUGA_BAND_DEFAULT_HIGH_VALUE "1E0"
#define YUGA_BAND_DEFAULT_LOW_VALUE  "15"
#define YUGA_BAND_CLCK_VALUE_1       0x01
#define YUGA_BAND_CLCK_VALUE_3       0x04
#define YUGA_BAND_CLCK_VALUE_5       0x10
#define YUGA_BAND_CLCK_VALUE_38     0x02
#define YUGA_BAND_CLCK_VALUE_39     0x04
#define YUGA_BAND_CLCK_VALUE_40     0x08
#define YUGA_BAND_CLCK_VALUE_41     0x10
#define YUGA_BAND_CLCK_VALUE_38_40      "A0"
#define YUGA_BAND_CLCK_VALUE_39_40      "C0"
#define YUGA_BAND_CLCK_VALUE_38_39_40      "E0"
#define YUGA_BAND_CLCK_VALUE_38_40_41      "1A0"
#define YUGA_BAND_CLCK_VALUE_39_40_41      "1C0"
#define YUGA_BAND_CLCK_VALUE_38_39_40_41      "1E0"

static int g_connect_flag=0;
static int g_connect_fail=0;
static int g_network_prepare = 0;

static LTE_RET_E yuga_imsi_get(void *pvData, int iDataLen);
static LTE_RET_E yuga_apn_set(void *pvData, int iDataLen);


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
        LTE_LOG(LTE_LOG_ERR,"%d,%s:send msg failed, writelen:%d, size%d\n",__LINE__,__func__, iWriteLen, sizeof(
CLIENT_SEND_MSG_T));
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
static LTE_RET_E yuga_power_on (void *pvData, int iDataLen)
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

static LTE_RET_E yuga_power_off (void *pvData, int iDataLen)
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
        LTE_LOG(LTE_LOG_ERR,"%d,%s:yuga_power_off failed, create socket failed\n",__LINE__,__func__);
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

static LTE_RET_E yuga_pin_state (void *pvData, int iDataLen)
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
 函 数 名  : arccra_pin_unlock
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
 static LTE_RET_E yuga_pin_unlock (void *pvData, int iDataLen)
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

static LTE_RET_E yuga_pin_switch (void *pvData, int iDataLen)
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

    if(0 == pstPinSwitch->enSwitch)
    {
        iRet = yuga_pin_unlock(pstPinSwitch->acPin, sizeof(pstPinSwitch->acPin));    //解锁前需先使sim卡处于ready状态
        if(iRet < 0)
        {
            return LTE_FAIL;
        }
    }
    
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

static LTE_RET_E yuga_pin_sw_get( void *pvData, int iDataLen )
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
 函 数 名  : pal_bsp_lte_pin_numremain
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
static LTE_RET_E yuga_pin_attempts_get(void *pvData, int iDataLen)
{
    return LTE_OK;
}
/*****************************************************************************
 函 数 名  : pal_bsp_lte_pin_alter
 功能描述  : 修改pin码,修改密码,只有在pin码开关开启的状态才能修改
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
static LTE_RET_E yuga_pin_alter (void *pvData, int iDataLen)
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

static LTE_RET_E yuga_signal_get (void *pvData, int iDataLen)
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

static LTE_RET_E yuga_set_nettype_cmd(LTE_NET_TYPE_T *pstNetType,char *pcComdstm)
{
    char ucSimType[32] = {0};
    int iMobileNetType[]={YUGA_GSM_ONLY, YUGA_TDS_ONLY, YUGA_LTE_ONLY, YUGA_GSM_TDS, YUGA_TDS_LTE, YUGA_GSM_TDS_LTE};
    int iUnicomNetType[]={YUGA_GSM_ONLY, YUGA_WCD_ONLY, YUGA_LTE_ONLY, YUGA_GSM_WCD, YUGA_WCD_LTE, YUGA_GSM_WCD_LTE};
    int *iNetTypeNum = NULL;
    if( NULL == pstNetType || NULL == pstNetType)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:input parameter is NULL",__LINE__,__func__);
        return LTE_FAIL;

    }
    if(LTE_FAIL == yuga_imsi_get((void *)ucSimType, 5))
    {
        return LTE_FAIL;
    }
    printf("yuga_set_nettype_cmd  == %s\n", ucSimType);
    if(NULL != strstr(LTE_CHINA_MOBILE_CODE, ucSimType))//移动
    {
        iNetTypeNum = iMobileNetType;
    }
    else if(NULL != strstr(LTE_CHINA_UNICOM_CODE, ucSimType))//联通
    {
        iNetTypeNum = iUnicomNetType;
    }
    else if(NULL != strstr(LTE_CHINA_TELECOM_CODE, ucSimType))//电信
    {
        sprintf(pcComdstm, "at^modeconfig=2\r");
        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }
    
    switch(pstNetType->enNetType)
    {
        case LTE_2G_ONLY:
        {
            sprintf(pcComdstm, "at^modeconfig=%d\r", iNetTypeNum[0]); /* 仅GSM */
            break;
        }
        case LTE_3G_ONLY: 
        {
            sprintf(pcComdstm, "at^modeconfig=%d\r", iNetTypeNum[1]); /* 仅TDS */
            break;
        }
        case LTE_4G_ONLY:
        {
            sprintf(pcComdstm, "at^modeconfig=%d\r", iNetTypeNum[2]); /* 仅LTE */
            break;
        }
         case LTE_2G_3G:
        {
            sprintf(pcComdstm, "at^modeconfig=%d\r", iNetTypeNum[3]); /* GSM,TDS */
            break;
        }
          case LTE_3G_4G:
        {
            sprintf(pcComdstm, "at^modeconfig=%d\r", iNetTypeNum[4]); /* LTE,TDS */
            break;
        }
        case LTE_2G_3G_4G:
        {
            sprintf(pcComdstm, "at^modeconfig=%d\r", iNetTypeNum[5]); /* LTE,TDS,GSM */
            break;
        }
        default:
        {
            LTE_LOG(LTE_LOG_ERR,"%d,%s:unknown net typer",__LINE__,__func__);
            sprintf(pcComdstm, "at^modeconfig=2\r");
            break;
            //return LTE_FAIL;
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
static LTE_RET_E yuga_nettype_set (void *pvData, int iDataLen)
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
    if( LTE_FAIL == yuga_set_nettype_cmd(pstNetType, acCmd))
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



/*****************************************************************************
 函 数 名  : arccra_nettype_get
 功能描述  : 获取模块当前配置的的网络类型
 输入参数  : void *pvData  
             int iDataLen  
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年11月25日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_nettype_get(void *pvData, int iDataLen)//no
{
    int iRet = 0;
    int iClientSocketFd = -1;
    char acCmd[] = "at^modeconfig?";
    char acCiev[] = "^MODECONFIG:";
    char *pcBuf = NULL;
    char ucSimType[32] = {0};
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};
    
    LTE_NET_TYPE_T *pstNetType = (LTE_NET_TYPE_T *)pvData;
    
    if(LTE_FAIL == yuga_imsi_get((void *)ucSimType, 5))
    {
        return LTE_FAIL;
    }
    
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

static LTE_RET_E yuga_imei_get (void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+gsn";
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

static LTE_RET_E yuga_cellid_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cellinfo";
    char acCiev[] = "+CELLINFO:";
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

static LTE_RET_E yuga_iccid_get(void *pvData, int iDataLen)//no
{
    /*ICCID: 898600810915F6046777*/
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+iccid";
    char acCiev[] = "ICCID:";
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
        strncpy(pstIccid->acICCID, stLteRecvMsg.uLteCommand.stIccidInfo.acICCID, sizeof(pstIccid->acICCID));
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;

}

static LTE_RET_E yuga_imsi_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cimi";
    char acCiev[] = "+cimi:";
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
        strncpy(pstImsiInfo->acIMSI, stLteRecvMsg.uLteCommand.stImsiInfo.acIMSI, iDataLen);//sizeof(pstImsiInfo->acIMSI));
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;
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
static LTE_RET_E yuga_plmn_register(void *pvData, int iDataLen)
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
static LTE_RET_E yuga_linkstate_get(void *pvData, int iDataLen)
{
    LTE_LINKSTATE_T *pstLinkstate = (LTE_LINKSTATE_T*)pvData;

	int ret = -1, fd;
	ndis_ipinfo pipinfo;

    if (NULL == pvData)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:param invalid\n",__LINE__,__func__);
        return LTE_FAIL;
    }
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

static LTE_RET_E yuga_simstate_get(void *pvData, int iDataLen)
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


static int yuga_begin_connect(LTE_CONNECT_PARAM_T *pstConnectParam)
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

static int yuga_cereg_get(char *pcCmd, char *pcCiev)
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

    iRet = lte_client_recv(iClientSocketFd, &stLteRecvMsg, 1);
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

static LTE_RET_E yuga_network_prepare(void *pvData, int iDataLen)
{
    int iRegsiterStatus = 0;
    char acNeedleCgreg[16] = "+CGREG";
    char acNeedleCereg[16] = "+CEREG";
    char acNeedleCreg[16]  = "+CREG:";
    
    char acCmdCgreg[16] =  "at+cgreg?";
    char acCmdCereg[16] =  "at+cereg?";
    char acCmdCreg[16]  =  "at+creg?";
    if (g_network_prepare == 0 && iDataLen != 0)
    {
		yuga_apn_set(pvData, iDataLen);	
		yuga_power_on(NULL, 0);
	}

    iRegsiterStatus = yuga_cereg_get(acCmdCereg, acNeedleCereg);
    if ((1 == iRegsiterStatus) || (5 == iRegsiterStatus))
    {
        g_network_prepare = 0;
        return LTE_OK;
    }
    iRegsiterStatus = yuga_cereg_get(acCmdCgreg, acNeedleCgreg);
    if ((1 == iRegsiterStatus) || (5 == iRegsiterStatus))
    {
        g_network_prepare = 0;
        return LTE_OK;
    }
    iRegsiterStatus = yuga_cereg_get(acCmdCreg, acNeedleCreg);
    if ((1 == iRegsiterStatus) || (5 == iRegsiterStatus))
    {
        g_network_prepare = 0;
        return LTE_OK;
    }
    
    g_network_prepare++;
    /*add by wangqi 2015.7.1 begin pn: 域格模块有时候驻留不上网络，需要软重启*/
    if (g_network_prepare >= DEFAULT_SEARCH_NET_TIMES)
	{
	    g_network_prepare = 0;
        if (LTE_OK != yuga_power_off(NULL, 0))
		{
			LTE_LOG(LTE_LOG_ERR, "power off fail");
			sleep(1);
			yuga_power_off(NULL, 0);
			//return LTE_FAIL;
		}
		if (LTE_OK != yuga_power_on(NULL, 0))
		{
			LTE_LOG(LTE_LOG_ERR, "power on fail");
			sleep(1);
			yuga_power_on(NULL, 0);
			//return LTE_FAIL;
		}
	}
	/*add by wangqi 2015.7.1 end pn: 域格模块有时候驻留不上网络，需要软重启*/
    return LTE_FAIL;
}

static LTE_RET_E yuga_connect(void *pvData, int iDataLen)
{
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
        /*add by wangqi 2015-6-5  begin*/
        //yuga_apn_set(pvData, iDataLen);
        /*add by wangqi 2015-6-5  end*/
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
    if(-1 == yuga_begin_connect(&stConnectParam)){
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


static int yuga_begin_disconnect(int iIPFamily)
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
		ndis_close(fd);
    	return -1;
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
		ndis_close(fd);
    	return -1;
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


}

static LTE_RET_E yuga_disconnect(void *pvData, int iDataLen)
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
    if(-1 == yuga_begin_disconnect(iIPFamily))
		return LTE_FAIL;
	else
		return LTE_OK;

}

static LTE_RET_E yuga_gps_start(void *pvData, int iDataLen)
{
    
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cgps=1";
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
    
    stLteMsg.iFuncCmd = LTE_GPS_START;
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
    //LTE_LOG(LTE_LOG_INFO,"########################:result:%d[0:fail;1:success]\n", stLteRecvMsg.iResult);

    if(1 == stLteRecvMsg.iResult)
    {
        iRet = LTE_OK;
        //system("killall get_gpsinfo");
        //system("get_gpsinfo&");
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;
    
}
static LTE_RET_E yuga_gps_stop(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cgps=0";
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
    
    stLteMsg.iFuncCmd = LTE_GPS_STOP;
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
    //LTE_LOG(LTE_LOG_INFO,"########################:result:%d[0:fail;1:success]\n", stLteRecvMsg.iResult);

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
/* 从文件中获取gps信息 */
static LTE_RET_E yuga_gpsinfo_get(char *acGps)
{
    FILE *pfData;
    char acTmp[128] = "";
    int iLoop = 0;
    int iFd = 0;
	int iRet = 0;
	int iNwrite = 0;
	struct flock lock;
	
	FILE *fp = NULL;
	char acGetData[128] = {0};
    int iGpsinfoState = 0;
    
    /*设置读独占锁*/
	lock.l_type = F_RDLCK;  /* 共享读锁 */
	lock.l_start = 0;
	lock.l_whence = SEEK_SET;
	lock.l_len = 0;
    //return TRUE;
    /*add by wangqi 2015-5-9, begin*/
    fp = popen("ps", "r");
	if ( fp == NULL )
    {
		LTE_LOG(LTE_LOG_ERR, "popen [ps] fail");
		return LTE_FAIL;
	}
	while(NULL != fgets(acGetData, sizeof(acGetData) - 1, fp))
	{
        if(NULL != strstr(acGetData, "get_gpsinfo"))
        {
            iGpsinfoState = 1;
            break;
        }
	}
	pclose(fp);
	if(0 == iGpsinfoState)
	{
        system_call("get_gpsinfo&");
        sleep(3);
	}
	/*add by wangqi 2015-5-9, end*/
    iFd = open(LTEGPSINFO, O_RDWR);
	if(iFd < 0)
	{
		LTE_LOG(LTE_LOG_INFO, "FILE:%s FUN:%s LINE:%d open "LTEGPSINFO"fail\n",
            __FILE__, __func__, __LINE__);
		return LTE_FAIL;
	}

    /*阻塞等待*/
	iRet = fcntl(iFd,F_SETLKW,&lock);
	if(iRet <0)
	{
	    LTE_LOG(LTE_LOG_INFO, "FILE:%s FUN:%s LINE:%d add lock failed "LTEGPSINFO" \n",
            __FILE__, __func__, __LINE__);
	}

    iRet = read(iFd, acTmp, 128);
    if (iRet < 0)
    {
        LTE_LOG(LTE_LOG_INFO, "FILE:%s FUN:%s LINE:%d read file "LTEGPSINFO" fail\n",
            __FILE__, __func__, __LINE__);
    }

    /*对于文件进行解锁*/
	lock.l_type = F_UNLCK;
	iRet = fcntl(iFd,F_SETLKW,&lock);
	if(iRet < 0)
	{
	    LTE_LOG(LTE_LOG_INFO,"FILE:%s FUN:%s LINE:%d unlock file "LTEGPSINFO" fail\n",
            __FILE__, __func__, __LINE__);
    }

    LTE_LOG(LTE_LOG_INFO,"FILE:%s FUN:%s LINE:%d date is :%s ", __FILE__, __func__, __LINE__, acTmp);
    
    close(iFd);
    
    memcpy(acGps, acTmp, 128);
    
    return LTE_OK;
}
static LTE_RET_E yuga_gpsinfo_analysis(char *acGps, LTE_GPS_INFO_T *pstLteGpsInfo)
{
    char	acLongitude[32] = {0};	/*所在经度 */
	char	acLatitude[32] = {0};		/* 所在纬度 */
	char	acVelocity[32] = {0};		/* 公交行驶速度*/
	char	acOrientation[32] = {0};	/* 方位角 */
	char	acSiteTime[64] = {0};		/*定位时间*/
    char    acGpsInfo[128] = {0},acTmpBuf[32]={0};
    int     ulGpsInfoLen,i = -1,j;
    char     acGpsPara[16][32] = {0};
    char    *szStart=NULL,*szEnd=NULL;
    float   fUnit = 1.852,fVelocity = 0;//一节=一海里/小时=1.852公里/小时
    float   fVelocityTmp = 0;
    int     iRet = 0; 
    
    if ((NULL == acGps) || (NULL == pstLteGpsInfo))
    {
        LTE_LOG(LTE_LOG_INFO, "ap_agent_analysis_gpsinfo input param is NULL\n");
        return LTE_FAIL;
    }
    memcpy(acGpsInfo, acGps, sizeof(acGpsInfo));
    ulGpsInfoLen = strlen(acGpsInfo);

    LTE_LOG(LTE_LOG_INFO, "ap_agent_analysis_gpsinfo GpsInfo:[%s]\n", acGpsInfo);    
	    
    szStart = szEnd = acGpsInfo;

    while(szEnd!=NULL )
    {
          if( GPS_DECODER_PARA_NUM <= i )
            {
                    LTE_LOG(LTE_LOG_INFO, "ap_agent_analysis_gpsinfo, only decoder 16 parameter\n");    
                    break;
            }
         if(((szEnd = strstr(szStart,","))!=NULL)&&(szEnd-szStart>=1)&&(ulGpsInfoLen>0))
         {
                memcpy(acGpsPara[++i],szStart,(szEnd-szStart));
                ulGpsInfoLen = ulGpsInfoLen - (szEnd-szStart)-1;
                szStart = szEnd+1;
         }
         else if(((szEnd = strstr(szStart,","))!=NULL)&&(szEnd==szStart)&&(ulGpsInfoLen>0))
         {      
                 ++i;
                 ulGpsInfoLen = ulGpsInfoLen -1;
                 szStart = szEnd+1;
         }
         else if(((szEnd = strstr(szStart,","))==NULL)&&((szEnd = strstr(szStart,"\0"))!=NULL)&&(ulGpsInfoLen>0))
         {  
                memcpy(acGpsPara[++i],szStart,ulGpsInfoLen);
                break;
         }
         else 
         {
                break;
         }
    }
    /*时间*/
    memcpy(pstLteGpsInfo->acTime, acGpsPara[AC_CW_GPS_DATE_TIME], strlen(acGpsPara[AC_CW_GPS_DATE_TIME]));
    memcpy(pstLteGpsInfo->acDate, acGpsPara[AC_CW_GPS_DATE],strlen(acGpsPara[AC_CW_GPS_DATE])); 
    /*纬度*/
    memcpy(pstLteGpsInfo->acLatitude,acGpsPara[AC_CW_GPS_LATITUDE_DIRECTION],strlen(acGpsPara[AC_CW_GPS_LATITUDE_DIRECTION]));
    memcpy((pstLteGpsInfo->acLatitude)+(strlen(acGpsPara[AC_CW_GPS_LATITUDE_DIRECTION])),
                acGpsPara[AC_CW_GPS_LATITUDE],strlen(acGpsPara[AC_CW_GPS_LATITUDE]));
    /*经度*/
    memcpy(pstLteGpsInfo->acLongitude,acGpsPara[AC_CW_GPS_LONGITUDE_DIRECTION],strlen(acGpsPara[AC_CW_GPS_LONGITUDE_DIRECTION]));
    memcpy((pstLteGpsInfo->acLongitude)+(strlen(acGpsPara[AC_CW_GPS_LONGITUDE_DIRECTION])),
                acGpsPara[AC_CW_GPS_LONGITUDE],strlen(acGpsPara[AC_CW_GPS_LONGITUDE]));
    /*汽车速度单位为公里/小时,(1节=1海里/小时=1.852公里/小时)*/
    if(0 != strlen(acGpsPara[AC_CW_GPS_BUS_VELOCITY]))
    {

           fVelocity = atof(acGpsPara[AC_CW_GPS_BUS_VELOCITY]);
           LTE_LOG(LTE_LOG_INFO, "ap_agent_analysis_gpsinfo before fVelocity:[%.3lf]\n", fVelocity);    
           fVelocity = fVelocity * fUnit;
           fVelocityTmp = fVelocity/3.6;/* 将其单位换算为m/s */
           if(fVelocityTmp > 100)
           {
                
                snprintf(pstLteGpsInfo->acVelocity, 32, "0.0");
                
           }
           else
           {                
               snprintf(pstLteGpsInfo->acVelocity, 32, "%.1f", fVelocityTmp);
           }
            
    }
    /*方向角*/
    memcpy(pstLteGpsInfo->acOrientation, acGpsPara[AC_CW_GPS_ORIENTATION], strlen(acGpsPara[AC_CW_GPS_ORIENTATION]));

    return LTE_OK;
}
static LTE_RET_E yuga_gps_date_get(void *pvData, int iDataLen)
{
    char   acGpsInfo[512] = {0};
    LTE_GPS_INFO_T stLteGpsInfo = {0};

    LTE_GPS_DATE_T *pstLteGpsDate = (LTE_GPS_DATE_T *)pvData;
    
    yuga_gpsinfo_get(acGpsInfo);
    yuga_gpsinfo_analysis(acGpsInfo, &stLteGpsInfo);
    sprintf(pstLteGpsDate->acDate, "%s-%s", stLteGpsInfo.acDate, stLteGpsInfo.acTime);
    
    return LTE_OK;
}
/*****************************************************************************
 函 数 名  : thinkwill_cdma_mi660_gps_position_info_get
 功能描述  : 获取gps的位置信息
 输入参数  : void *pvData  
             int iDataLen  
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年9月2日星期二
    作    者   : liyoukun
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_gps_pstinfo_get(void *pvData, int iDataLen)
{
    char   acGpsInfo[512] = {0};
    LTE_GPS_INFO_T stLteGpsInfo = {0};

    LTE_GPS_POSTION_T *pstLteGpsDate = (LTE_GPS_POSTION_T *)pvData;
    
    yuga_gpsinfo_get(acGpsInfo);
    yuga_gpsinfo_analysis(acGpsInfo, &stLteGpsInfo);
    
    strcpy(pstLteGpsDate->acLatitude, stLteGpsInfo.acLatitude);
    strcpy(pstLteGpsDate->acLongitude, stLteGpsInfo.acLongitude);
    strcpy(pstLteGpsDate->acVelocity, stLteGpsInfo.acVelocity);
    strcpy(pstLteGpsDate->acOrientation, stLteGpsInfo.acOrientation);
    
    return LTE_OK;
}


/*****************************************************************************
   函 数 名  : yuga_sms_cent_code_set
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
static LTE_RET_E yuga_sms_cent_code_set(void *pvData, int iDataLen)
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
   函 数 名  : yuga_sms_mode_set
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
static LTE_RET_E yuga_sms_mode_set(void *pvData, int iDataLen)
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
 函 数 名  : yuga_sms_at_csms
 功能描述  : 初始化消息服务类型
 输入参数  : void  
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年12月1日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_sms_at_csms(void)
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
 函 数 名  : yuga_sms_at_cpms
 功能描述  : 初始化消息存储方式
 输入参数  : void  
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年12月1日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_sms_at_cpms(void)
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
 函 数 名  : yuga_sms_at_cnmi
 功能描述  : 初始化新消息通知
 输入参数  : void  
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年12月1日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_sms_at_cnmi(void)
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
   函 数 名  : yuga_sms_at_cgsms
   功能描述  : 初始化消息承载域
   输入参数  : void
   输出参数  : 无
   返 回 值  : static
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2014年12月1日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_sms_at_cgsms(void)
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
 函 数 名  : yuga_sms_at_csmp
 功能描述  : 设置文本模式下短消息参数
 输入参数  : void  
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年12月1日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_sms_at_csmp(void)
{
    int iRet = 0;    
    int iClientSocketFd = -1;
    char acCmd[] = "at+csmp=17,173,0,0";
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
   函 数 名  : zte_micro_b57_5_sms_func_init
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
static LTE_RET_E yuga_sms_func_init(void *pvData, int iDataLen)
{
    int iRet = 0;
    LTE_SMS_Mode_T pstMode = {0};
    pstMode.iSmsMode = 0;/* 默认设为pdu模式 */
    iRet = yuga_sms_at_csms();
    if (iRet != LTE_OK)
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:yuga_sms_at_csms failed\n", __LINE__, __func__);
        return LTE_FAIL;
    }
    iRet = yuga_sms_at_cpms();
    if (iRet != LTE_OK)
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:yuga_sms_at_cpms failed\n", __LINE__, __func__);
        return LTE_FAIL;
    }
    iRet = yuga_sms_at_cnmi();
    if (iRet != LTE_OK)
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:yuga_sms_at_cnmi failed\n", __LINE__, __func__);
        return LTE_FAIL;
    }
#if 0
    iRet = yuga_sms_at_cgsms();
    if (iRet != LTE_OK)
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:yuga_sms_at_cgsms failed\n", __LINE__, __func__);
        return LTE_FAIL;
    }
    iRet = yuga_sms_at_csmp();
    if (iRet != LTE_OK)
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:yuga_sms_at_csmp failed\n", __LINE__, __func__);
        return LTE_FAIL;
    }
#endif
    iRet = yuga_sms_mode_set(&pstMode, sizeof(LTE_SMS_Mode_T));
    if (iRet != LTE_OK)
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:yuga_sms_mode_set failed\n", __LINE__, __func__);
        return LTE_FAIL;
    }

    return LTE_OK;

}


static LTE_RET_E yuga_sms_recv(void *pvData, int iDataLen)
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
   函 数 名  : yuga_sms_send
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
static LTE_RET_E yuga_sms_send(void *pvData, int iDataLen)
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
   函 数 名  : yuga_sms_dele
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
static LTE_RET_E yuga_sms_dele(void *pvData, int iDataLen)
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
    else if (pstDeleSms->iDelIndex >= 1 && pstDeleSms->iDelIndex <= 51)
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

static LTE_RET_E yuga_lte_info_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
	int iRet = 0;
	char acCmd[] = "at+cellinfo";
	char acCiev[] = "+CELLINFO:";
	char *pcBuf = NULL;
	LTE_INFO_T *pstLteInfo = (LTE_INFO_T *)pvData;
	
	CLIENT_SEND_MSG_T stLteMsg = {0};
	LTE_SEND_MSG_T stLteRecvMsg = {0};

	if (NULL == pstLteInfo)
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
		pstLteInfo->stCellInfo.iRsrp = stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.iRsrp;
		pstLteInfo->stCellInfo.iRsrq = stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.iRsrq;
		pstLteInfo->stCellInfo.iRssi = stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.iRssi;
		pstLteInfo->stCellInfo.iSinr = stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.iSinr;
		strncpy(pstLteInfo->stCellInfo.acCellId, stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.acCellId, sizeof(pstLteInfo->stCellInfo.acCellId));
		strncpy(pstLteInfo->stCellInfo.acBand, stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.acBand, sizeof(pstLteInfo->stCellInfo.acBand));
        strncpy(pstLteInfo->stCellInfo.acPhyCeillid, stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.acPhyCeillid, sizeof(pstLteInfo->stCellInfo.acPhyCeillid));
        pstLteInfo->stCellInfo.iChannel = stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.iChannel;
        iRet = LTE_OK;
	}
	else
	{
		iRet = LTE_FAIL;
	}
	
	return iRet;  
}

static LTE_RET_E yuga_access_net_get(void *pvData, int iDataLen)
{
	int iRet = 0;
    int iClientSocketFd = -1;
    //char acCmd[] = "at^sysinfo";
    //char acCiev[] = "^SYSINFO:";
    char acCmd[] = "at+cops?";
    char acCiev[] = "+COPS:";
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

static LTE_RET_E yuga_bands_get(void *pvData, int iDataLen)
{
    int flag, iRet = 0;
    LTE_MODULE_VERSION_T pstVersion = {0};
    char acBuff[64] = {0};
    FILE* pstFd = NULL;

    if (pvData == NULL)
    {
        return LTE_FAIL;
    }
    
    LTE_BANDS_T *pstBands = (LTE_BANDS_T *)pvData;

    /* no AT command supported, use software version to distinguish them */
    /* B56: 1,3,5,7,8,38,39,40,41 */

    char bands_yuga_band[] = "1,3,5,38,39,40,41";
   	
    strncpy(pstBands->bands, bands_yuga_band, sizeof(pstBands->bands));

    pstBands->iClcBandAble = CLC_BAND_ENABLE;
    
    return LTE_OK;
}

static LTE_RET_E yuga_band_value_code(char *pcBandHigh, char *pcBandLow, int iBandHigh, int iBandLow)
{
    if(iBandHigh & YUGA_BAND_CLCK_VALUE_41)
    {
        if((iBandHigh & YUGA_BAND_CLCK_VALUE_38) && (iBandHigh & YUGA_BAND_CLCK_VALUE_39) && (iBandHigh & YUGA_BAND_CLCK_VALUE_40))
        {
            strcpy(pcBandHigh, YUGA_BAND_CLCK_VALUE_38_39_40_41);
        }
        else if((iBandHigh & YUGA_BAND_CLCK_VALUE_38) && (iBandHigh & YUGA_BAND_CLCK_VALUE_40))
        {
            strcpy(pcBandHigh, YUGA_BAND_CLCK_VALUE_38_40_41);
        }
        else if((iBandHigh & YUGA_BAND_CLCK_VALUE_39) && (iBandHigh & YUGA_BAND_CLCK_VALUE_40))
        {
            strcpy(pcBandHigh, YUGA_BAND_CLCK_VALUE_39_40_41);
        }
        else
        {
            sprintf(pcBandHigh, "1%d0", iBandHigh & 0Xf);
        }
    }
    else
    {
        if((iBandHigh & YUGA_BAND_CLCK_VALUE_38) && (iBandHigh & YUGA_BAND_CLCK_VALUE_39) && (iBandHigh & YUGA_BAND_CLCK_VALUE_40))
        {
            strcpy(pcBandHigh, YUGA_BAND_CLCK_VALUE_38_39_40);
        }
        else if((iBandHigh & YUGA_BAND_CLCK_VALUE_38) && (iBandHigh & YUGA_BAND_CLCK_VALUE_40))
        {
            strcpy(pcBandHigh, YUGA_BAND_CLCK_VALUE_38_40);
        }
        else if((iBandHigh & YUGA_BAND_CLCK_VALUE_39) && (iBandHigh & YUGA_BAND_CLCK_VALUE_40))
        {
            strcpy(pcBandHigh, YUGA_BAND_CLCK_VALUE_39_40);
        }
        else
        {
            sprintf(pcBandHigh, "%d0", iBandHigh & 0Xf);
        }
    }

    if(iBandLow & YUGA_BAND_CLCK_VALUE_5)
    {
        sprintf(pcBandLow, "1%d", iBandLow & 0X0f);
    }
    else
    {
        sprintf(pcBandLow, "%d", iBandLow & 0X0f);
    }
    
    return LTE_OK;
}

static LTE_RET_E yuga_clck_bands_set(void *pvData, int iDataLen)
{
    int iRet = 0;
    unsigned int iBandValue = 0;
    int iClientSocketFd = -1;
    int iBandHigh = 0;
    int iBandLow = 0;
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};
    char acCmd[64] = {0};
    char acCiev[] = "OK";
    char acBandHighValue[16] = {0};
    char acBandLowValue[16] = {0};

    LTE_BANDS_T *pstBands = (LTE_BANDS_T *)pvData;
    
    /* 判断是否为配置默认band */
    if(0 == strlen(pstBands->bands))
    {
        strcpy(pstBands->bands, YUGA_BAND_DEFAULT);
    }
    
    if(0 == strcmp(pstBands->bands, YUGA_BAND_DEFAULT))
    {
        strcpy(acBandHighValue, YUGA_BAND_DEFAULT_HIGH_VALUE);     
        strcpy(acBandLowValue, YUGA_BAND_DEFAULT_LOW_VALUE);
    }
    else
    {
            if(NULL != strstr(pstBands->bands, "1,"))
            {
                iBandLow |= YUGA_BAND_CLCK_VALUE_1;
            }
            if(NULL != strstr(pstBands->bands, "3,"))
            {
                iBandLow |= YUGA_BAND_CLCK_VALUE_3;
            }
            if(NULL != strstr(pstBands->bands, "5,"))
            {
                iBandLow |= YUGA_BAND_CLCK_VALUE_5;
            }

            if(NULL != strstr(pstBands->bands, "38"))
            {
                iBandHigh |= YUGA_BAND_CLCK_VALUE_38;
            }
            if(NULL != strstr(pstBands->bands, "39"))
            {
                iBandHigh |= YUGA_BAND_CLCK_VALUE_39;
            }
            if(NULL != strstr(pstBands->bands, "40"))
            {
                iBandHigh |= YUGA_BAND_CLCK_VALUE_40;
            }
            if(NULL != strstr(pstBands->bands, "41"))
            {
                iBandHigh |= YUGA_BAND_CLCK_VALUE_41;
            }
            yuga_band_value_code(acBandHighValue, acBandLowValue, iBandHigh, iBandLow);
    }

    sprintf(acCmd, "at^bandconfig=%s,%s\r", acBandHighValue, acBandLowValue);

    printf("yuga_clck_bands_set : %s\n", acCmd);
    
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

static LTE_RET_E yuga_clck_bands_close(void *pvData, int iDataLen)
{
    int iRet;
    LTE_BANDS_T pstBands = {0};
    strcpy(pstBands.bands, "default");
    iRet = yuga_clck_bands_set(&pstBands, sizeof(LTE_BANDS_T));
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:arccra_clck_bands_set failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    return LTE_OK; 
}

static LTE_RET_E yuga_clckcell_enable_get(void *pvData, int iDataLen)
{
    LTE_CLKCELL_ENABLE_T *pstClkCellEnable = (LTE_CLKCELL_ENABLE_T *)pvData;
    LTE_ACCESS_NETWORK_T stAccessNet = {0};
    int iRet;
    
    if (pvData == NULL)
    {
		return LTE_FAIL;
    }
    if(LTE_OK != yuga_access_net_get((void *)&stAccessNet, sizeof(LTE_ACCESS_NETWORK_T)))
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:get access net type error!!!\n",__LINE__,__func__);
        pstClkCellEnable->iClkCellEnable = 0;
        return LTE_OK;
    }
    if(stAccessNet.enAccessNet == NET_4G)
    {
        pstClkCellEnable->iClkCellEnable = 1;
    }
    else
    {
        pstClkCellEnable->iClkCellEnable = 0;
    }
    return LTE_OK;
}
static LTE_RET_E yuga_clck_cellid_set(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;    
    int iPhyCellid = 0;
    int iEarfcn = 0;
    char acCmd[64] = {0};
    char acCiev[] = "OK";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_CLK_CELL_T *pstCellClkT = (LTE_CLK_CELL_T *)pvData;
    
    if(1 == pstCellClkT->iClkEnable)
    {  
        iPhyCellid = atoi(pstCellClkT->acCellid);
        if(iPhyCellid < 0 || iPhyCellid > 503)
        {
            LTE_LOG(LTE_LOG_ERR,"%d,%s:PHY cellid is too large, (0~503)!\n",__LINE__,__func__);
            return LTE_FAIL;
        }
        iEarfcn = atoi(pstCellClkT->acFreq);
        if(iEarfcn < 36000 || iEarfcn > 41589)
        {
            LTE_LOG(LTE_LOG_ERR,"%d,%s:yuga only support clock 4G earfcn, (36000~41589)!\n",__LINE__,__func__);
            return LTE_FAIL;
        }
    }

    if(0 == pstCellClkT->iClkEnable)
    {
        snprintf(acCmd, sizeof(acCmd), "at+celllock=0,0\r");
    }
    else if(1 == pstCellClkT->iClkEnable)
    {
        snprintf(acCmd, sizeof(acCmd), "at+celllock=%s,%s\r", pstCellClkT->acFreq, pstCellClkT->acCellid);
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:in param is error!\n",__LINE__,__func__);
        return LTE_FAIL;
    }

    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
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

static LTE_RET_E yuga_apn_set(void *pvData, int iDataLen)
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

static LTE_RET_E yuga_version_get(void *pvData, int iDataLen)
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

static LTE_RET_E yuga_enodeb_id_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cellinfo";
    char acCiev[] = "+CELLINFO:";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
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
    
    return iRet;
}

static LTE_RET_E yuga_tac_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cellinfo";
    char acCiev[] = "+CELLINFO:";
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
    
    return iRet;
}

static LTE_RET_E yuga_lcid_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cellinfo";
    char acCiev[] = "+CELLINFO:";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
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
    
    return iRet;
}

static LTE_RET_E yuga_all_cellid_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cellinfo";
    char acCiev[] = "+CELLINFO:";
    char *pcBuf = NULL;
    
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

static LTE_RET_E yuga_set_oper_info_type(void)
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

static LTE_RET_E yuga_oper_info_get(void *pvData, int iDataLen)
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
    iRet = yuga_set_oper_info_type();
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

static LTE_RET_E yuga_puk_unlock(void *pvData, int iDataLen)
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

/*****************************************************************************
 函 数 名  : yuga_sup_nettype_get
 功能描述  : 获取支持的网络类型，如2g，3g，4g
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
static LTE_RET_E yuga_sup_nettype_get(void *pvData, int iDataLen)
{
    int iRet;
    LTE_NET_TYPE_T *pstNets = (LTE_NET_TYPE_T *)pvData;
    LTE_MODULE_VERSION_T stVersion;

    pstNets->enNetType = LTE_2G_ONLY | LTE_3G_ONLY | LTE_4G_ONLY | LTE_2G_3G |
            LTE_3G_4G | LTE_2G_3G_4G;
    pstNets->enNetTypePri = LTE_NONE ;
#if 0   
    iRet = yuga_version_get(&stVersion, sizeof(LTE_MODULE_VERSION_T));
    if(NULL != strstr(stVersion.acVersion, "B56"))                //为域格七模模块添加B56
    {
        pstNets->enNetType = LTE_2G_ONLY | LTE_3G_ONLY | LTE_4G_ONLY | LTE_2G_3G |
            LTE_3G_4G | LTE_2G_3G_4G;
         pstNets->enNetTypePri = LTE_NONE ;
    } 
    else
    {
        pstNets->enNetType = LTE_4G_ONLY;
        pstNets->enNetTypePri = LTE_NONE;
    }
#endif

    return LTE_OK;

}

/*****************************************************************************
 函 数 名  : yuga_sms_sup_func
 功能描述  : 获取是否支持短信功能
 输入参数  : void *pvData  
             int iDataLen  
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年12月3日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E yuga_sms_sup_func(void *pvData, int iDataLen)
{
    int iRet;
    LTE_MODULE_VERSION_T stVersion = {0};
    LTE_SUPPORT_SMS_T *pstSup = (LTE_SUPPORT_SMS_T *)pvData;

    pstSup->enSupSms = SUP_SMS_FUNC;
#if 0
    iRet = yuga_version_get(&stVersion, sizeof(LTE_MODULE_VERSION_T));
    if(NULL != strstr(stVersion.acVersion, "B56"))                //为域格七模模块添加B56
    {
        pstSup->enSupSms = SUP_SMS_FUNC;
    } 
    else
    {
        pstSup->enSupSms = NO_SUP_SMS_FUNC;
    }
#endif

    return LTE_OK;   
}

static LTE_RET_E yuga_sim_unlock_support(void *pvData, int iDataLen)
{
    LTE_UNLOCK_SUPPORT_T *pstUnlockSupport = (LTE_UNLOCK_SUPPORT_T* )pvData;
    if(NULL == pstUnlockSupport || iDataLen < sizeof(LTE_UNLOCK_SUPPORT_T))
    {
        return LTE_FAIL;
    }
    pstUnlockSupport->iUnlockFlag = 0;
    return LTE_OK;
}


LTE_IOCTL_FUNC_T astYugaCLM920Table[] = 
{    
    {LTE_PIN_STATE,         yuga_pin_state,           0,0,  "yuga_pin_state"       }, 
    {LTE_PIN_SWITCH,        yuga_pin_switch,           0,0, "yuga_pin_switch"       }, 
    {LTE_PIN_SW_GET,        yuga_pin_sw_get,           0,0, "yuga_pin_sw_get"       },
    {LTE_PIN_UNLOCK,        yuga_pin_unlock,           0,0, "yuga_pin_unlock"       }, 
    {LTE_PIN_ATTEMPTS,      yuga_pin_attempts_get,    0,0,  "yuga_pin_attempts_get"       }, 
    {LTE_PIN_ALTER,         yuga_pin_alter,           0,0,  "yuga_pin_alter"       }, 
    {LTE_SIGNAL_GET,        yuga_signal_get,        0,0,    "yuga_wcdma_mi900_signal_get"       }, 
    {LTE_POWER_ON,          yuga_power_on,          0,0,    "yuga_power_on"       }, 
    {LTE_POWER_OFF,         yuga_power_off,         0,0,    "yuga_power_off"       }, 
    {LTE_NETTYPE_SET,       yuga_nettype_set,       0,0,    "yuga_nettype_set"       }, 
    {LTE_NETTYPE_GET,       yuga_nettype_get,       0,0,    "yuga_nettype_get"       }, 
    {LTE_PLMN_REGISTER,     yuga_plmn_register,     0,0,    "yuga_plmn_register"       }, 
    {LTE_PREFER_PLMN_SET,   NULL,           0,0,    NULL       }, 
    {LTE_NETWORK_PREPARE,   yuga_network_prepare,   0,0,    "yuga_network_prepare"       }, 
    {LTE_NETWORK_STATUS_GET,NULL,0,0,    NULL       },
    {LTE_CONNECT,           yuga_connect,           0,0,    "yuga_connect"       }, 
    {LTE_DISCONNECT,        yuga_disconnect,        0,0,    "yuga_disconnect"       }, 
    {LTE_LINKSTATE_GET,     yuga_linkstate_get,     0,0,    "yuga_linkstate_get"       }, 
    {LTE_IMEI_GET,          yuga_imei_get,          0,0,    "yuga_imei_get"       }, 
    {LTE_CELLID_GET,        yuga_cellid_get,        0,0,    "yuga_cellid_get"       }, 
    {LTE_ICCID_GET,         yuga_iccid_get,         0,0,    "yuga_iccid_get"       },
    {LTE_IMSI_GET,          yuga_imsi_get,          0,0,    "yuga_imsi_get"       },
    {LTE_SIM_STATE_GET,     yuga_simstate_get,      0,0,    "yuga_simstate_get"       }, 
    {LTE_INFO_GET,          yuga_lte_info_get,      0,0,    "yuga_lte_info_get"       }, 
    {LTE_ACCESS_NET_GET,    yuga_access_net_get,    0,0,    "yuga_access_net_get"       }, 
    {LTE_BANDS_GET,         yuga_bands_get,         0,0,    "yuga_bands_get"       }, 
    {LTE_CLCK_BANDS_SET,    yuga_clck_bands_set,    0,0,    "yuga_clck_bands_set"       }, 
    {LTE_CLCK_BANDS_CLOSE,  yuga_clck_bands_close,  0,0,    "yuga_clck_bands_close"       },
    {LTE_CLCK_CELL_GET,     yuga_clckcell_enable_get,0,0,   "yuga_clckcell_enable_get"   },
    {LTE_CLCK_CELL_SET,     yuga_clck_cellid_set,   0,0,    "yuga_clck_cellid_set"       },
    {LTE_OPER_INFO_GET,     yuga_oper_info_get,     0,0,    "yuga_oper_info_get"          }, 
    {LTE_NETTYPE_INFO_GET,  yuga_sup_nettype_get,   0,0,    "yuga_sup_nettype_get"          },
    {LTE_GPS_START,         yuga_gps_start,         0,0,    "yuga_gps_start"      }, 
    {LTE_GPS_STOP,          yuga_gps_stop,          0,0,    "yuga_gps_start"    }, 
    {LTE_GPS_DATE_GET,      yuga_gps_date_get,      0,0,    "yuga_gps_date_get" }, 
    {LTE_GPS_POSITION_INFO_GET, yuga_gps_pstinfo_get, 0,0,  "yuga_gps_pstinfo_get"    }, 
    /* BEGIN: Added by wujian, 2014/11/13   PN:添加短信功能相关接口 */
    {LTE_SMS_CENT_CODE_SET, yuga_sms_cent_code_set, 0,0,    "yuga_sms_cent_code_set" },
    {LTE_SMS_MODE__SET,     yuga_sms_mode_set ,     0,0,    "yuga_sms_mode_set"   },   
    {LTE_SMS_FUNC_INIT,     yuga_sms_func_init,     0,0,    "yuga_sms_func_init"   },
    {LTE_SMS_RECV,          yuga_sms_recv ,         0,0,    "yuga_sms_recv"   },
    {LTE_SMS_SEND,          yuga_sms_send,          0,0,    "yuga_sms_send"   },
    {LTE_SMS_DELE,          yuga_sms_dele ,         0,0,    "yuga_sms_dele"   },
    {LTE_SMS_SUPP,          yuga_sms_sup_func,      0,0,    "yuga_sms_sup_func"   },
    /* END:   Added by wujian, 2014/11/13   PN:添加短信功能相关接口 */
    {LTE_IPV6_PDP_SET, NULL, 0, 0, NULL   },
    /* BEGIN: Added by wujian, 2014/11/17   PN:添加设置apn接口 */
    {LTE_APN_SET,           yuga_apn_set, 0, 0,             "yuga_apn_set"   },
    /* END:   Added by wujian, 2014/11/17   PN:添加设置apn接口 */
    {LTE_VERSION_GET,       yuga_version_get,        0,0,    "yuga_version_get"    },
    {LTE_ENODEBID_GET,      yuga_enodeb_id_get,     0,0,     "yuga_enodeb_id_get"    }, 
    {LTE_TAC_GET,           yuga_tac_get,     0,0,     "yuga_tac_get"    },
    {LTE_LCID_GET,          yuga_lcid_get,     0,0,     "yuga_lcid_get"    },  
    {LTE_ALL_CELLID_GET,    yuga_all_cellid_get,     0,0,  "yuga_all_cellid_get"      },
    {LTE_SIM_UNLOCK_SUPPORT,yuga_sim_unlock_support, 0,0, "yuga_sim_unlock_support"},
    {LTE_SIM_UNLOCK_PUK,	yuga_puk_unlock, 0,0, "zte_micro_b57_n_puk_unlock"	},
    
    {LTE_CMD_MAX,           NULL,           0,0,    NULL       }, 
};

