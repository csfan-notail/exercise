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

#include "des.h"

#define ATCOM "/dev/ttyUSB0"

/* 针对中兴微模块ICCID获取创建的文件 */
#define ICCID_FILE_PATH "/tmp/zte_iccid.file"
#define ZTE_B57_BAND_1_VALUE       1  /* band 1对应的锁频值 */
#define ZTE_B57_BAND_3_VALUE       4  /* band 4对应的锁频值 */
#define ZTE_B57_BAND_8_VALUE       128  /* band 8对应的锁频值 */
#define ZTE_B57_BAND_38_VALUE      1  /* band 38对应的锁频值 */
#define ZTE_B57_BAND_39_VALUE      2  /* band 39对应的锁频值 */
#define ZTE_B57_BAND_40_VALUE      4  /* band 40对应的锁频值 */
#define ZTE_B57_BAND_41_VALUE      8  /* band 41对应的锁频值 */

#define ZTE_B57_BAND_DEFAULT       "default"
#define ZTE_B57_BAND_DEFAULT_VALUE 7  /* 默认支持的网段:38,39,40 */
#define ZTE_B57_FDD_BAND_DEFAULT_VALUE 132  /* 默认支持的网段:3,8 */
#define ZTE_B78_FDD_BAND_DEFAULT_VALUE 5  /* 默认支持的网段:1,3 */

#define ZTE_B57_5_VERSION   "B57_5-M"
#define ZTE_B57_1_VERSION   "B57_1-V1.0"
#define ZTE_B57_5_FLAG      "B57_5"
#define ZTE_B57_1_FLAG      "B57_1"
#define ZTE_B78_FLAG        "B78-"
#define ZTE_B78_FLAG2       "LCC_LH"


#define ZTE_B57_GSM_ONLY           13
#define ZTE_B57_LTE_ONLY           17
#define ZTE_B57_TDSCDMA_ONLY       15
#define ZTE_B57_GSM_TDSCDMA        18
#define ZTE_B57_TDSCDMA_LTE        20
#define ZTE_B57_GSM_LTE_TDSCDMA    24
#define ZTE_B57_AUTO               2

static int g_network_prepare = 0;
static int g_connect_flag=0;
static int g_connect_fail=0;
int g_iLteClientSock = 0;

static LTE_RET_E zte_micro_b57_5_lte_info_get(void *pvData, int iDataLen);
static LTE_RET_E zte_micro_b57_n_ver_get(void *pvData, int iDataLen);
static LTE_RET_E zte_micro_b57_5_access_net_get(void *pvData, int iDataLen);


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

static int zte_micro_b57_5_strtohex(char cStr)
{
    int iResult = 0;
    switch(cStr)
    {
        case '0':
            iResult = 0;
            break;
        case '1':
            iResult = 1;
            break;
        case '2':
            iResult = 2;
            break;
        case '3':
            iResult = 3;
            break;
        case '4':
            iResult = 4;
            break;
        case '5':
            iResult = 5;
            break;
        case '6':
            iResult = 6;
            break;
        case '7':
            iResult = 7;
            break;
        case '8':
            iResult = 8;
            break;
        case '9':
            iResult = 9;
            break;
        case 'a':
            iResult = 10;
            break;
        case 'b':
            iResult = 11;
            break;
        case 'c':
            iResult = 12;
            break;
        case 'd':
            iResult = 13;
            break;
        case 'e':
            iResult = 14;
            break;
        case 'f':
            iResult = 15;
            break;    
        case 'A':
            iResult = 10;
            break;
        case 'B':
            iResult = 11;
            break;
        case 'C':
            iResult = 12;
            break;
        case 'D':
            iResult = 13;
            break;
        case 'E':
            iResult = 14;
            break;
        case 'F':
            iResult = 15;
            break;    
        default :
            iResult = 0;
            break;
        
    }
    return iResult;
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
    
	LTE_LOG(LTE_LOG_INFO,"#[lteclient]#acAtCmd: %s, acCiev: %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
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
 函 数 名  : zte_micro_b57_5_pin_state
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
static LTE_RET_E zte_micro_b57_5_pin_state (void *pvData, int iDataLen)
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
 函 数 名  : zte_micro_b57_5_pin_switch
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
static LTE_RET_E zte_micro_b57_5_pin_switch (void *pvData, int iDataLen)
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
 函 数 名  : arccra_pin_sw_get
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
static LTE_RET_E zte_micro_b57_5_pin_sw_get( void *pvData, int iDataLen )
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
 static LTE_RET_E zte_micro_b57_5_pin_unlock (void *pvData, int iDataLen)
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
static LTE_RET_E zte_micro_b57_5_pin_attempts_get(void *pvData, int iDataLen)
{
    return LTE_OK;
}


/*****************************************************************************
 函 数 名  : pal_bsp_lte_pin_alter
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
static LTE_RET_E zte_micro_b57_5_pin_alter (void *pvData, int iDataLen)
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

static LTE_RET_E zte_micro_b57_5_file_is_exists(char *pcFile)
{
    if(NULL == pcFile)
    {
        LTE_LOG(LTE_LOG_ERR, "param is null pointer!");
        return LTE_FAIL;
    }
   
    if(0 != access(pcFile, F_OK))
    {
        LTE_LOG(LTE_LOG_ERR, "file [%S] is not exist!", pcFile);
        return LTE_FAIL;
    }
    else
    {
        return LTE_OK;
    }
}


static LTE_RET_E zte_micro_b57_5_power_on (void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cfun=1";
    char acCiev[32] = {0};
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};
    
    if(LTE_OK == zte_micro_b57_5_file_is_exists(ICCID_FILE_PATH))
    {
        strncpy(acCiev, "OK",sizeof(acCiev));
        stLteMsg.iComReadType = COM_READ_TYPE_1;
    }
    else
    {
        strncpy(acCiev, "+ZICCID:",sizeof(acCiev));
        stLteMsg.iComReadType = COM_READ_TYPE_8;
    }
    
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

    iRet = lte_client_recv(iClientSocketFd, &stLteRecvMsg, 8);
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
        LTE_LOG(LTE_LOG_ERR,"%d,%s:########################:poweron result:fail.\n", __LINE__,__func__);
        iRet = LTE_FAIL;
    }
    
    return iRet;   
}
static LTE_RET_E zte_micro_b57_5_power_get(void *pvData, int iDataLen)
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
static LTE_RET_E zte_micro_b57_5_power_off (void *pvData, int iDataLen)
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
        LTE_LOG(LTE_LOG_ERR,"%d,%s:########################:poweroff result:fail.\n", __LINE__,__func__);
        iRet = LTE_FAIL;
    }
    
    return iRet;   

}

static LTE_RET_E zte_micro_b57_5_flight_mode(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cfun=4";
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

    iRet = lte_client_recv(iClientSocketFd, &stLteRecvMsg, 4);
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

/* piMoVersion返回1为B57_5模块版本；2为B57_1版本 */
static LTE_RET_E zte_micro_b57_n_version_get(int *piMoVersion)
{
    FILE* pstFd = NULL;
    char acBuff[64] = {0};
    int iRet = 0;
    int uiFlag = 0;
  
    LTE_MODULE_VERSION_T stVersion = {0};

    if(NULL == piMoVersion)
    {
        return LTE_FAIL;
    }
    iRet = zte_micro_b57_n_ver_get(&stVersion, sizeof(LTE_MODULE_VERSION_T));
    
    strncpy(acBuff, stVersion.acVersion, sizeof(acBuff));
    if(NULL != strstr(acBuff,ZTE_B57_5_VERSION))
    {
        uiFlag = 1; /* 1为B57_5模块版本；2为B57_1版本 */
    }
    else if(NULL != strstr(acBuff,ZTE_B57_1_VERSION))
    {
        uiFlag = 2; /* 1为B57_5模块版本；2为B57_1版本 */
    }
    else if(NULL != strstr(acBuff,ZTE_B78_FLAG) || NULL != strstr(acBuff,ZTE_B78_FLAG2))
    {
        uiFlag = 3; /* 1为B57_5模块版本；2为B57_1版本 ; 3为邮票孔版本*/
    }
    else
    {
        uiFlag = 0;
    }
    *piMoVersion = uiFlag;
    
    return LTE_OK;

}
static LTE_RET_E zte_micro_b57_5_signal_handle(LTE_SIGNAL_T *pstSignal, LTE_SIGNAL_VALUE_T *piSignalValue, int iSignal, int mode)
{
    LTE_ACCESS_NETWORK_T stAccessNet = {0};
    int iGrade[] = {31,25,19,13,7,0,191,173,155,137,119,101,197,144,136,131,126,121};
    int *piGrade = NULL;
    if(LTE_OK != zte_micro_b57_5_access_net_get((void*)&stAccessNet, sizeof(LTE_ACCESS_NETWORK_T)))
    {
        return LTE_FAIL;
    }
    if(1 == mode)//五个等级信号
    {
        if(NULL == pstSignal)
        {
            return LTE_FAIL;
        }
        if(NET_2G == stAccessNet.enAccessNet)
        {
            piGrade = iGrade;
        }
        else if(NET_3G == stAccessNet.enAccessNet)
        {
            piGrade = iGrade + 6;
        }
        else if(NET_4G == stAccessNet.enAccessNet)
        {
            piGrade = iGrade + 12;
        }
        else
        {
            pstSignal->uiSignal = 0;
            return LTE_OK;
        }
        if(iSignal > piGrade[1] && iSignal <= piGrade[0])
        {
            pstSignal->uiSignal = 5;
        }
        else if(iSignal > piGrade[2] && iSignal <= piGrade[1])
        {
            pstSignal->uiSignal = 4;
        }
        else if(iSignal > piGrade[3] && iSignal <= piGrade[2])
        {
            pstSignal->uiSignal = 3;
        }
        else if(iSignal > piGrade[4] && iSignal <= piGrade[3])
        {
            pstSignal->uiSignal = 2;
        }
        else if(iSignal > piGrade[5] && iSignal <= piGrade[4])
        {
            pstSignal->uiSignal = 1;
        }
        else
        {
            pstSignal->uiSignal = 0;
        }
       
    }
    else if(2 == mode)//获取具体值
    {
        if(NULL == piSignalValue)
        {
            return LTE_FAIL;
        }
        if(NET_2G == stAccessNet.enAccessNet)
        {
            piSignalValue->iSignalValue = 2*iSignal-113;
            strcpy(piSignalValue->acSignalName, "rssi");
        }
        else if(NET_3G == stAccessNet.enAccessNet)
        {
            piSignalValue->iSignalValue = iSignal-216;
            strcpy(piSignalValue->acSignalName, "rscp");
        }
        else if(NET_4G == stAccessNet.enAccessNet)
        {
            piSignalValue->iSignalValue = iSignal-241;
            strcpy(piSignalValue->acSignalName, "rsrp");
        }
        else
        {
            
        }
    }
    return LTE_OK;
    
}
static LTE_RET_E zte_micro_b57_5_original_signal_get(int *piSignal)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+csq";
    char acCiev[] = "+CSQ:";
    char *pcBuf = NULL;

    if(NULL == piSignal)
    {
        return LTE_FAIL;
    }
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

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
        iRet = LTE_OK;
        *piSignal = stLteRecvMsg.uLteCommand.stSignalInfo.uiSignal;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    return iRet;
}

static LTE_RET_E zte_micro_b57_5_signal_get (void *pvData, int iDataLen)
{
    int iSignal;
    LTE_SIGNAL_T *pstSignal = (LTE_SIGNAL_T*)pvData;
    if(LTE_OK != zte_micro_b57_5_original_signal_get(&iSignal))
    {
        LTE_LOG(LTE_LOG_ERR, "zte_micro_b57_5_original_signal_get fail");
        return LTE_FAIL;
    }
    if(LTE_OK != zte_micro_b57_5_signal_handle(pstSignal, NULL ,iSignal, 1))
    {
        LTE_LOG(LTE_LOG_ERR, "zte_micro_b57_5_signal_handle fail");
        return LTE_FAIL;
    }
    return LTE_OK;
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
/*altered by caibing 2013-12-17*/
/*****************************************************************************
 函 数 名  : zte_micro_b57_5_set_nettype_cmd
 功能描述  : 设置网络类型cmd参数
 输入参数  : LTE_NET_TYPE_T *pstNetType  
             char *pcComdstm             
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年12月15日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E zte_micro_b57_5_set_nettype_cmd(LTE_NET_TYPE_T *pstNetType,char *pcComdstm)
{
    int iFlag = 0;
    if( NULL == pstNetType || NULL == pstNetType)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:input parameter is NULL",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    switch(pstNetType->enNetType)
    {
        case LTE_2G_ONLY:
        {
            sprintf(pcComdstm, "at^sysconfig=13,0,1,2\r"); /* 仅GSM */
            break;
        }
        case LTE_3G_ONLY: 
        {
            sprintf(pcComdstm, "at^sysconfig=15,0,1,2\r"); /* 仅TDS */
            break;
        }
        case LTE_4G_ONLY:
        {
            sprintf(pcComdstm, "at^sysconfig=17,0,1,2\r"); /* 仅LTE */
            break;
        }
        case LTE_2G_3G:
        {
            if( pstNetType->enNetTypePri == LTE_2G_PRI)
            {
                sprintf(pcComdstm, "at^sysconfig=18,1,1,2\r"); /* GSM,TDS */
            }
            else if( pstNetType->enNetTypePri == LTE_3G_PRI)
            {
                sprintf(pcComdstm, "at^sysconfig=18,2,1,2\r"); /* GSM,TDS */
            }
            else if( pstNetType->enNetTypePri == LTE_AUTO)
            {
                sprintf(pcComdstm, "at^sysconfig=18,2,1,2\r"); /* GSM,TDS */
            }
            else
            {
                LTE_LOG(LTE_LOG_ERR,"%d,%s:NO netTypePri in the  netTyper",__LINE__,__func__);
                return LTE_FAIL;
            }
            break;
        }
        case LTE_3G_4G:
        {
            if( pstNetType->enNetTypePri == LTE_3G_PRI)
            {
                sprintf(pcComdstm, "at^sysconfig=20,5,1,2\r"); /* TDD,TDS */
            }
            else if( pstNetType->enNetTypePri == LTE_4G_PRI)
            {
                sprintf(pcComdstm, "at^sysconfig=20,6,1,2\r"); /* TDD,TDS */
            }
            else if( pstNetType->enNetTypePri == LTE_AUTO)
            {
                sprintf(pcComdstm, "at^sysconfig=20,6,1,2\r"); /* TDD,TDS */
            }
            else
            {
                LTE_LOG(LTE_LOG_ERR,"%d,%s:NO netTypePri in the  netTyper",__LINE__,__func__);
                return LTE_FAIL;
            }
            break;
        }
        case LTE_2G_3G_4G: /* TDD,TDS,GSM */
        {
            if( pstNetType->enNetTypePri == LTE_2G_PRI)
            {
                sprintf(pcComdstm, "at^sysconfig=24,1,1,2\r"); 
            }
            else if( pstNetType->enNetTypePri == LTE_3G_PRI)
            {
                sprintf(pcComdstm, "at^sysconfig=24,2,1,2\r"); 
            }
            else if( pstNetType->enNetTypePri == LTE_4G_PRI)
            {
                sprintf(pcComdstm, "at^sysconfig=24,6,1,2\r");
            }
            else if( pstNetType->enNetTypePri == LTE_AUTO)
            {
                sprintf(pcComdstm, "at^sysconfig=24,6,1,2\r");
            }
            else
            {
                LTE_LOG(LTE_LOG_ERR,"%d,%s:NO netTypePri in the  netTyper",__LINE__,__func__);
                return LTE_FAIL;
            }
            break;
        }
        default:
        {
            LTE_LOG(LTE_LOG_ERR,"%d,%s:unkown netTyper",__LINE__,__func__);
            sprintf(pcComdstm, "at^sysconfig=2,6,1,2\r");//auto select
            //return LTE_FAIL;
        }
    }
    return LTE_OK;

}
static LTE_RET_E zte_micro_b57_5_nettype_set_at (void *pvData, int iDataLen)
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
    LTE_NET_TYPE_T *pstNetType = (LTE_NET_TYPE_T *)pvData;
    if( LTE_FAIL == zte_micro_b57_5_set_nettype_cmd(pstNetType, acCmd) )
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

static LTE_RET_E zte_micro_b57_5_nettype_set (void *pvData, int iDataLen)
{
    int iRet = LTE_OK;    
    int iCount = 0;
    
    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    /*for(iCount=0; iCount<5; ++iCount)
    {
        if (LTE_OK == zte_micro_b57_5_flight_mode(NULL, 0))
        {
            break;
        }
    }
    sleep(1);*/
    zte_micro_b57_5_flight_mode(NULL, 0);
    sleep(1);
    for(iCount=0; iCount<5; ++iCount)
    {
        if (LTE_OK == zte_micro_b57_5_nettype_set_at(pvData, iDataLen))
        {
            break;
        }
        sleep(1);
    }
    if(5 == iCount)
    {
        iRet = LTE_FAIL;
    }
    zte_micro_b57_5_power_on(NULL, 0);
    /*for(iCount=0; iCount<5; ++iCount)
    {
        if (LTE_OK == zte_micro_b57_5_power_on(NULL, 0))
        {
            break;
        }
    }*/
    return iRet;
}

static LTE_RET_E zte_micro_b57_5_nettype_get(void *pvData, int iDataLen)
{
    int iRet = 0;
    int iClientSocketFd = -1;
    char acCmd[] = "at^sysconfig?";
    char acCiev[] = "^SYSCONFIG:";
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
static LTE_RET_E zte_micro_b57_5_linkstate_get (void *pvData, int iDataLen)
{
    int iRet = 0;
        int iClientSocketFd = -1;
        char acCmd[] = "at+zgact?";
        char acCiev[] = "+ZGACT:";
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

static LTE_RET_E zte_micro_b57_5_imei_get (void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cgsn";
    char acCiev[] = "+CGSN:";
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
static LTE_RET_E zte_micro_b57_5_imsi_get(void *pvData, int iDataLen)
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
static LTE_RET_E zte_micro_b57_5_iccid_get(void *pvData, int iDataLen)
{
    int iRet = 0;
    FILE *pstFd = NULL;
    char acBuff[64];
    char acBuffTmp[64];
    char acCiev[] = "+ZICCID:";

    LTE_ICCID_T *pstIccid = (LTE_ICCID_T *)pvData;
    //ICCID_FILE_PATH
    if(NULL == pvData)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s: input point null\n",__LINE__,__func__);
        return LTE_FAIL;
    }

    if(0 != access(ICCID_FILE_PATH, R_OK))
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:"ICCID_FILE_PATH" is not exit\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    pstFd = fopen(ICCID_FILE_PATH, "r");
    if (NULL == pstFd)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:open %s failed\n", __LINE__, __func__, ICCID_FILE_PATH);
        return -1;
    }
    while (NULL != fgets(acBuff,64,pstFd))
    {
        if(NULL != strstr(acBuff, acCiev))
        {
            sscanf(acBuff, "%*[^:]:%s", acBuffTmp);
            strncpy(pstIccid->acICCID, acBuffTmp, sizeof(pstIccid->acICCID));
            break;
        }
        memset(acBuff, 0, sizeof(acBuff));
    }
    
    fclose(pstFd);
    pstFd = NULL;
    
    return LTE_OK;
    
}

static LTE_RET_E zte_micro_b57_5_stop_report(void)
{
    int iRet = 0;
    LTE_NORMAL_AT_T stAt = {"at+zemci=0\r","/tmp/stopcellinfo","OK",0,ATCOM,0,1};
    
    iRet = lte_normal_at_cmd(&stAt);
    if (iRet != 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:at command failed\n",__LINE__,__func__);
        lte_file_unlink(stAt.acFileName);
        return LTE_FAIL;
    }
    
    lte_file_unlink(stAt.acFileName);

    return LTE_OK;
}

static LTE_RET_E zte_micro_b57_5_cellid_get (void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+zemci=1";
    char acCiev[] = "+ZEMCI:";
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
    stLteMsg.iRealFlag = 1;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
    stLteMsg.iComReadType = COM_READ_TYPE_8;
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
        zte_micro_b57_stop_zemci(LTE_CELLID_GET);
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
    zte_micro_b57_stop_zemci(LTE_CELLID_GET);
    return iRet;
}



static LTE_RET_E zte_micro_b57_5_simstate_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at^sysinfo";
    char acCiev[] = "^SYSINFO:";
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
        iRet = LTE_FAIL;
    }
    
    return iRet;
  
}
static int zte_micro_b57_cereg_get(char *pcCmd, char *pcCiev)
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
static LTE_RET_E zte_micro_b57_5_network_prepare(void *pvData, int iDataLen)
{
    int iRegsiterStatus = 0;

    char acNeedleCgreg[16] = "+CGREG";
    char acNeedleCereg[16] = "+CEREG";
    char acNeedleCreg[16] = "+CREG";
    
    char acCmdCgreg[16] =  "at+cgreg?";
    char acCmdCereg[16] =  "at+cereg?";
    char acCmdCreg[16] =  "at+creg?";
    LTE_CFUN_QUERY_T stCfunQuery;
    
    iRegsiterStatus = zte_micro_b57_cereg_get(acCmdCereg, acNeedleCereg);
    if ((1 == iRegsiterStatus) || (5 == iRegsiterStatus))
    {
        return LTE_OK;
    }
    iRegsiterStatus = zte_micro_b57_cereg_get(acCmdCgreg, acNeedleCgreg);
    if ((1 == iRegsiterStatus) || (5 == iRegsiterStatus))
    {
        return LTE_OK;
    }
    iRegsiterStatus = zte_micro_b57_cereg_get(acCmdCreg, acNeedleCreg);
    if ((1 == iRegsiterStatus) || (5 == iRegsiterStatus))
    {
        return LTE_OK;
    }
    
    return LTE_FAIL;
    
}
static LTE_RET_E zte_micro_b57_5_network_status_get(void *pvData, int iDataLen)
{
	int iRegsiterStatus = 0;

    char acNeedleCgreg[16] = "+CEREG:";  
    char acCmdCgreg[16] =  "at+cereg?\r";

    LTE_NETWORK_PREPARE_T *pstNwStatus = (LTE_NETWORK_PREPARE_T *)pvData;

    iRegsiterStatus = zte_micro_b57_cereg_get(acCmdCgreg, acNeedleCgreg);
    sprintf(pstNwStatus->acNetWorkStatus,"%d", iRegsiterStatus);
        
	return  LTE_OK;
}

static LTE_RET_E zte_micro_b57_connect_begin(char *pcCmd, char *pcCiev)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};
    int iCreg = 0;
    
    LTE_LOG(LTE_LOG_ERR,"%d,%s:into zte_micro_b57_cereg_get\n",__LINE__,__func__);
    
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
static LTE_RET_E zte_micro_b57_5_connect(void *pvData, int iDataLen)
{
    int iRet = 0;
    
    /*altered by caibing 2014-09-16 begin*/
    char cDeviceMode = '0';
    FILE *pFile = NULL;
    char acFileName[64] = {"/tmp/device_mode.tmp"};
    char acBuf[32] = {0};
    char acApn[32] = {0};
    char acPDPStack[16] = {0};
    char acBrkCmd[32] = "at+zgact=0,1";
    char acLteCmd[32] = "at+zgact=1,1";
    char acGGGCmd[32] = "at+cgact=1,1";
    char acCiev[32] = "OK";
    /*altered by caibing 2014-09-16 end*/
    LTE_APN_T *pstApn = (LTE_APN_T *)pvData;
    
    LTE_NORMAL_AT_T stAt = {"at+zgact=1,1\r","/tmp/connect","OK",0,ATCOM,0,3};
    LTE_NORMAL_AT_T stAt3G = {"at+cgact=1,1\r","/tmp/3gconnect","OK",0,ATCOM,0,3};
    #if 1
    if (g_connect_flag)
    {
		/* connecting or connected ... , don't do it again */
		return LTE_OK;
	}
    #endif
    if(NULL == pstApn)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:input param is null pointer!\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    system_call("ifconfig usb0 up"); 
    #if 0
    lte_normal_at_cmd(&stAt3G);
    lte_file_unlink(stAt3G.acFileName);
    /*执行at指令*/
   	iRet = lte_normal_at_cmd(&stAt);
    #endif
    /*alter by wangqi 2015-6-2 begin*/
    zte_micro_b57_connect_begin(acBrkCmd, acCiev);    
    if(LTE_OK != zte_micro_b57_connect_begin(acGGGCmd, acCiev))//3G 网络拨号流程
    {
        sleep(15);
    }
    /*alter by wangqi 2015-6-2 end*/
    iRet = zte_micro_b57_connect_begin(acLteCmd, acCiev);
	if (iRet != LTE_OK)
	{
    	LTE_LOG(LTE_LOG_ERR,"%d,%s:at command failed\n",__LINE__,__func__);
    	lte_file_unlink(stAt.acFileName);
        g_connect_flag = 0;
		g_connect_fail = 1;
    	return LTE_FAIL;
	}
    else
    {
        g_connect_flag = 2;
		g_connect_fail = 0;

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
    lte_file_unlink(stAt.acFileName);
    return LTE_OK;
}

static LTE_RET_E zte_micro_b57_5_disconnect(void *pvData, int iDataLen)
{
	int iRet = 0;
    LTE_NORMAL_AT_T stAt = {"at+zgact=0,1\r","/tmp/disconnect","OK",0,ATCOM,0,1};
    char acCmd[] = "at+zgact=0,1";
    char acCiev[] = "OK";
    /*执行at指令*/
    #if 0
   	iRet = lte_normal_at_cmd(&stAt);
	if (iRet != 0)
	{
    	LTE_LOG(LTE_LOG_ERR,"%d,%s:at command failed\n",__LINE__,__func__);
    	lte_file_unlink(stAt.acFileName);
    	return LTE_FAIL;
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
    #endif
    if(LTE_OK != zte_micro_b57_connect_begin(acCmd, acCiev))
    {
        lte_file_unlink(stAt.acFileName);
        LTE_LOG(LTE_LOG_ERR,"%d,%s:at command failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    system("ifconfig usb0 0.0.0.0");
        
    system("kill -9 $(pidof "DHCLIENT")");

    system("killall dhclient");

    system("kill -9 $(pidof "DHCPC")");

    system("killall udhcpc");    
	g_connect_flag = 0;
    
    lte_file_unlink(stAt.acFileName);
    return LTE_OK;
}
/* lteinfo包含小区id、信号强度、接入band及频点 */
static LTE_RET_E zte_micro_b57_5_hex_dig(char *pcString, int *piResult)
{
    int     iBaseHex3 = 4096;
    int     iBaseHex2 = 256;
    int     iBaseHex1 = 16;

    int     iResult = 0;
    int     iTmp = 0;

    iTmp = zte_micro_b57_5_strtohex(pcString[0]);
    
    iResult += iTmp * iBaseHex3;
    iTmp = zte_micro_b57_5_strtohex(pcString[1]);
    
    iResult += iTmp * iBaseHex2;
    iTmp = zte_micro_b57_5_strtohex(pcString[2]);
    
    iResult += iTmp * iBaseHex1;
    iTmp = zte_micro_b57_5_strtohex(pcString[3]);
    
    iResult += iTmp;
    *piResult = iResult;
    
    return LTE_OK;
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
    char acCiev[] = "+ZEMCI:";
    char *pcBuf = NULL;
    
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
    stLteMsg.iRealFlag = 1;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
    stLteMsg.iComReadType = COM_READ_TYPE_8;
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
        zte_micro_b57_stop_zemci(LTE_INFO_GET);
        return LTE_FAIL;
    }
    
    close(iClientSocketFd);
    LTE_LOG(LTE_LOG_INFO,"########################:result:%d[0:fail;1:success]\n", stLteRecvMsg.iResult);

    if(1 == stLteRecvMsg.iResult)
    {
        strncpy(pstLteInfo->stCellInfo.acBand, stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.acBand, sizeof(pstLteInfo->stCellInfo.acBand));
        strncpy(pstLteInfo->stCellInfo.acCellId, stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.acCellId, sizeof(pstLteInfo->stCellInfo.acCellId));
        strncpy(pstLteInfo->stCellInfo.acPhyCeillid, stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.acPhyCeillid, sizeof(pstLteInfo->stCellInfo.acPhyCeillid));
        pstLteInfo->stCellInfo.iChannel = stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.iChannel;
        pstLteInfo->stCellInfo.iRsrp = stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.iRsrp;
        pstLteInfo->stCellInfo.iRsrq = stLteRecvMsg.uLteCommand.stLteInfo.stCellInfo.iRsrq;

        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
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
    return LTE_OK;
}
static LTE_RET_E zte_micro_b57_5_access_net_get(void *pvData, int iDataLen)
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
        LTE_LOG(LTE_LOG_INFO,"########################:net:%d\n", stLteRecvMsg.uLteCommand.stAccessNetInfo.enAccessNet);
        pstAccessNet->enAccessNet = stLteRecvMsg.uLteCommand.stAccessNetInfo.enAccessNet;
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    
    return iRet;
}
static LTE_RET_E zte_micro_b57_5_bands_get(void *pvData, int iDataLen)
{    
    int iVerSionFlag = 0;
	LTE_BANDS_T *pstBands = (LTE_BANDS_T *)pvData;
	LTE_MODULE_VERSION_T stVersion = {0};
    /* 38,39,40,41 */
    char b57_5_bands[] = "38,39,40"; 
    //char b57_1_bands[] = "3,8,38,39,40";
    char acBuff[64] = {0};
    char *pcTdd = NULL;
    char *pcFdd = NULL;
    int iCount = 0;
    char acTdd[16] = "TDD";
    char acFdd[16] = "FDD";

    zte_micro_b57_n_ver_get(&stVersion, sizeof(LTE_MODULE_VERSION_T));
    strncpy(acBuff, stVersion.acVersion, sizeof(acBuff));
    if(NULL != strstr(acBuff, ZTE_B57_5_VERSION))
    {
        memcpy(pstBands->bands, b57_5_bands, sizeof(pstBands->bands));
    }
    else if((NULL != strstr(acBuff, ZTE_B78_FLAG) || NULL != strstr(acBuff, ZTE_B78_FLAG2)) && NULL == strstr(acBuff, "TDD"))
    {
        strcpy(pstBands->bands, "1,3,38,39,40");
    }
    else if(0 == strcmp(acBuff, "B57_1-V1.0.0B05") || NULL != strstr(acBuff, "B05_CTA_OT")  || 0 == strcmp(acBuff, "B57_1-V1.0.0B06") || 0 == strcmp(acBuff, "B57_1-V1.0.0B07"))
    {
        strcpy(pstBands->bands, "3,8,38,39,40");
    }
    else
    {
        pcTdd = strstr(acBuff, acTdd);
        pcFdd = strstr(acBuff, acFdd);
        if(pcFdd != NULL)
        {
            *(pcFdd-1) = '\0';
            pcFdd += strlen(acFdd);
            while(*pcFdd != '\0')
            {
                if(*pcFdd == '_')
                {
                    pstBands->bands[iCount] = ',';
                }
                else
                {
                    pstBands->bands[iCount] = *pcFdd;
                }
                ++iCount;
                ++pcFdd;
            }
            if(pcTdd != NULL)
            {
                pstBands->bands[iCount] = ',';
                ++iCount;
            }
        }
        if(pcTdd != NULL)
        {
            pcTdd += strlen(acTdd);
            while(*pcTdd != '\0')
            {
                if(*pcTdd == '_')
                {
                    pstBands->bands[iCount] = ',';
                }
                else
                {
                    pstBands->bands[iCount] = *pcTdd;
                }
                ++iCount;
                ++pcTdd;
            }
        }
        pstBands->bands[iCount] = '\0';
    }
    pstBands->iClcBandAble = CLC_BAND_ENABLE; /* 支持锁频 */
    
	return LTE_OK;
}
static LTE_RET_E zte_micro_b57_clck_bands(char *pcAtCmd, char *pcCiev, int iFunc)
{
    
    int iClientSocketFd = -1;
    int iRet = 0;    
    int iPhyCellid = 0;
    char *pcBuf = NULL;
    
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
    stLteMsg.iFuncCmd = iFunc;
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
static LTE_RET_E zte_micro_b57_5_clck_bands_set(void *pvData, int iDataLen)
{
    char acCmd[64] = {0};
    char acCiev[32] = "OK";
    char acFddCmd[64] = {0};
    int iRet = 0;
    int iBand = 0;
    int iBandValue = 0;
    int iFddBandValue = 0;
    int iFddFlag = 0;
    int iTddFlag = 0;
    int iVerSionFlag = 0;
    char acBandValue[64] = {0};
    char acBandValueTmp[64] = {0};
    LTE_BANDS_T *pstBands = (LTE_BANDS_T *)pvData;

	if (pvData == NULL)
		return LTE_FAIL;

    /* 获取模块软件版本 */
    iRet = zte_micro_b57_n_version_get(&iVerSionFlag);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:get module version fail!\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    #if 0
    /* 判断是否为配置默认band */
    if(0 == strlen(pstBands->bands))
    {
        strcpy(pstBands->bands, ZTE_B57_BAND_DEFAULT);
    }
    
    if(0 == strcmp(pstBands->bands, ZTE_B57_BAND_DEFAULT))
    {
        iBandValue = ZTE_B57_BAND_DEFAULT_VALUE;
        if(3 == iVerSionFlag)
        {
            iFddBandValue = ZTE_B78_FDD_BAND_DEFAULT_VALUE;
        }
        else
        {
            iFddBandValue = ZTE_B57_FDD_BAND_DEFAULT_VALUE;
        }
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:band[%s] len[%d]!\n",__LINE__,__func__, pstBands->bands, strlen(pstBands->bands));
        if(strlen(pstBands->bands) < 2)
        {
            if(3 == atoi(pstBands->bands))
            {
                iFddBandValue = ZTE_B57_BAND_3_VALUE;
            }
            else if(8 == atoi(pstBands->bands))
            {
                iFddBandValue = ZTE_B57_BAND_8_VALUE;
            }
        }
        else
        {
            /* 判断锁的频段是否还有band1 */
            if(('1' == pstBands->bands[strlen(pstBands->bands) - 1])&&(',' == pstBands->bands[strlen(pstBands->bands) - 2]))
            {
                iFddBandValue += ZTE_B57_BAND_1_VALUE;
            }
            else if(('1' == pstBands->bands[0]) && (',' == pstBands->bands[1]))
            {
                iFddBandValue += ZTE_B57_BAND_1_VALUE;
            }
            else
            {
                if(NULL != strstr(pstBands->bands, ",1,"))
                {
                    iFddBandValue += ZTE_B57_BAND_1_VALUE;
                }
            }
            /* 判断锁的频段是否还有band3 */
            if(('3' == pstBands->bands[strlen(pstBands->bands) - 1])&&(',' == pstBands->bands[strlen(pstBands->bands) - 2]))
            {
                iFddBandValue += ZTE_B57_BAND_3_VALUE;
            }
            else if(('3' == pstBands->bands[0]) && (',' == pstBands->bands[1]))
            {
                iFddBandValue += ZTE_B57_BAND_3_VALUE;
            }
            else
            {
                if(NULL != strstr(pstBands->bands, ",3,"))
                {
                    iFddBandValue += ZTE_B57_BAND_3_VALUE;
                }
            }
            /* 判断锁的频段是否还有band8 */
            if(('8' == pstBands->bands[0]) && (',' == pstBands->bands[1]))
            {
                iFddBandValue += ZTE_B57_BAND_8_VALUE;
            }
            else
            {
                if(NULL != strstr(pstBands->bands, ",8"))
                {
                    iFddBandValue += ZTE_B57_BAND_8_VALUE;
                }
            }
            if(NULL != strstr(pstBands->bands, "38"))
            {
                iBandValue += ZTE_B57_BAND_38_VALUE;
            }
            if(NULL != strstr(pstBands->bands, "39"))
            {
                iBandValue += ZTE_B57_BAND_39_VALUE;
            }
            if(NULL != strstr(pstBands->bands, "40"))
            {
                iBandValue += ZTE_B57_BAND_40_VALUE;
            }
            if(NULL != strstr(pstBands->bands, "41"))
            {
                iBandValue += ZTE_B57_BAND_41_VALUE;
            }
        }
    }
    #else
    /* 判断是否为配置默认band */
    
    if(0 == strlen(pstBands->bands) || 0 == strcmp(pstBands->bands, ZTE_B57_BAND_DEFAULT))
    {
        if(LTE_OK != zte_micro_b57_5_bands_get((void *)pstBands, sizeof(LTE_BANDS_T)))
        {
            LTE_LOG(LTE_LOG_ERR,"%d,%s:get module bands fail!\n",__LINE__,__func__);
            return LTE_FAIL;
        }
    }
    
    LTE_LOG(LTE_LOG_ERR,"%d,%s:band[%s] len[%d]!\n",__LINE__,__func__, pstBands->bands, strlen(pstBands->bands));
    if(strlen(pstBands->bands) < 2)
    {
        if(1 == atoi(pstBands->bands))
        {
            iFddBandValue = ZTE_B57_BAND_1_VALUE;
        }
        else if(3 == atoi(pstBands->bands))
        {
            iFddBandValue = ZTE_B57_BAND_3_VALUE;
        }
        else if(8 == atoi(pstBands->bands))
        {
            iFddBandValue = ZTE_B57_BAND_8_VALUE;
        }
    }
    else
    {
        /* 判断锁的频段是否还有band1 */
        if(('1' == pstBands->bands[strlen(pstBands->bands) - 1])&&(',' == pstBands->bands[strlen(pstBands->bands) - 2]))
        {
            iFddBandValue += ZTE_B57_BAND_1_VALUE;
        }
        else if(('1' == pstBands->bands[0]) && (',' == pstBands->bands[1]))
        {
            iFddBandValue += ZTE_B57_BAND_1_VALUE;
        }
        else
        {
            if(NULL != strstr(pstBands->bands, ",1,"))
            {
                iFddBandValue += ZTE_B57_BAND_1_VALUE;
            }
        }
        /* 判断锁的频段是否还有band3 */
        if(('3' == pstBands->bands[strlen(pstBands->bands) - 1])&&(',' == pstBands->bands[strlen(pstBands->bands) - 2]))
        {
            iFddBandValue += ZTE_B57_BAND_3_VALUE;
        }
        else if(('3' == pstBands->bands[0]) && (',' == pstBands->bands[1]))
        {
            iFddBandValue += ZTE_B57_BAND_3_VALUE;
        }
        else
        {
            if(NULL != strstr(pstBands->bands, ",3,"))
            {
                iFddBandValue += ZTE_B57_BAND_3_VALUE;
            }
        }
        /* 判断锁的频段是否还有band8 */
        if(('8' == pstBands->bands[0]) && (',' == pstBands->bands[1]))
        {
            iFddBandValue += ZTE_B57_BAND_8_VALUE;
        }
        else
        {
            if(NULL != strstr(pstBands->bands, ",8"))
            {
                iFddBandValue += ZTE_B57_BAND_8_VALUE;
            }
        }
        if(NULL != strstr(pstBands->bands, "38"))
        {
            iBandValue += ZTE_B57_BAND_38_VALUE;
        }
        if(NULL != strstr(pstBands->bands, "39"))
        {
            iBandValue += ZTE_B57_BAND_39_VALUE;
        }
        if(NULL != strstr(pstBands->bands, "40"))
        {
            iBandValue += ZTE_B57_BAND_40_VALUE;
        }
        if(NULL != strstr(pstBands->bands, "41"))
        {
            iBandValue += ZTE_B57_BAND_41_VALUE;
        }
    }
    #endif
    /*9115*/
    if((0 != iBandValue) || (0 != iFddBandValue))
    {
        sprintf(acCmd, "at+zlockband=%d\r", iBandValue);
        sprintf(acFddCmd, "at+zlteband=%d\r", iFddBandValue);
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:at this band [%s] not support!\n",__LINE__,__func__, pstBands->bands);
        return LTE_FAIL;
    }
    
    
    LTE_LOG(LTE_LOG_INFO, "%d,%s:at this band iBandValue:%d iFddBandValue:%d\n",__LINE__,__func__, iBandValue, iFddBandValue);

    if(1 == iVerSionFlag)/* B57_5 */
    {
        iRet = zte_micro_b57_clck_bands(acCmd, acCiev, LTE_CLCK_BANDS_SET);
        if(LTE_OK != iRet)
        {
            LTE_LOG(LTE_LOG_INFO, "%d,%s:zte_micro_b57_clck_bands fail! at cmd:%s\n",__LINE__,__func__, acCmd);
            return LTE_FAIL;
        }
    }
    else if(2 == iVerSionFlag || 3 == iVerSionFlag)/* b57_1 */
    {
        /* 锁TDD网络 */
        iRet = zte_micro_b57_clck_bands(acCmd, acCiev, LTE_CLCK_BANDS_SET);
        if(LTE_OK != iRet)
        {
            LTE_LOG(LTE_LOG_INFO, "%d,%s:zte_micro_b57_clck_bands fail! at cmd:%s\n",__LINE__,__func__, acCmd);
            return LTE_FAIL;
        }
        /* 锁FDD网络 */
        iRet = zte_micro_b57_clck_bands(acFddCmd, acCiev, LTE_CLCK_BANDS_SET);
        /*if(LTE_OK != iRet)
        {
            LTE_LOG(LTE_LOG_INFO, "%d,%s:zte_micro_b57_clck_bands fail! at cmd:%s\n",__LINE__,__func__, acCmd);
            return LTE_FAIL;
        }*/
    }
	else
    {
		return LTE_FAIL;
    }
    zte_micro_b57_5_power_on(NULL, 0);    
	return LTE_OK;

}


static LTE_RET_E zte_micro_b57_5_clck_bands_close(void *pvData, int iDataLen)
{
    int iRet;
    LTE_BANDS_T pstBands = {0};
    strcpy(pstBands.bands, "default");
    iRet = zte_micro_b57_5_clck_bands_set(&pstBands, sizeof(LTE_BANDS_T));
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:zte_micro_b57_5_clck_bands_set failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    return LTE_OK;
}
static LTE_RET_E zte_micro_b57_5_clckcell_enable_get(void *pvData, int iDataLen)
{
    LTE_CLKCELL_ENABLE_T *pstClkCellEnable = (LTE_CLKCELL_ENABLE_T *)pvData;
    LTE_MODULE_VERSION_T stVersion = {0};
    LTE_ACCESS_NETWORK_T stAccessNet = {0};
    
    if (pvData == NULL)
    {
		return LTE_FAIL;
    }
    
    /* 中兴微电子支持锁小区 */
    zte_micro_b57_n_ver_get(&stVersion, sizeof(LTE_MODULE_VERSION_T));

    if(0 == strcmp(stVersion.acVersion, "B78-V1.0.0B02_TDD38_39_40"))
    {
        pstClkCellEnable->iClkCellEnable = 0;
    }
    else
    {
        if(LTE_OK != zte_micro_b57_5_access_net_get(&stAccessNet, sizeof(LTE_ACCESS_NETWORK_T)))
        {
            pstClkCellEnable->iClkCellEnable = 0;
            return LTE_OK;
        }
        if(NET_4G == stAccessNet.enAccessNet)
        {
            pstClkCellEnable->iClkCellEnable = 1;
        }
        else
        {
            pstClkCellEnable->iClkCellEnable = 0;
        }
    }

    return LTE_OK;
}
static LTE_RET_E zte_micro_b57_5_clck_cellid_set(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;    
    int iPhyCellid = 0;
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
    iPhyCellid = atoi(pstCellClkT->acCellid);
    if((1 == pstCellClkT->iClkEnable)&&(iPhyCellid > 503))
    {   
        LTE_LOG(LTE_LOG_ERR,"%d,%s:PHY cellid is too large, exceed 503!\n",__LINE__,__func__);
        return LTE_FAIL;
    }

    if(0 == pstCellClkT->iClkEnable)
    {
        snprintf(acCmd, sizeof(acCmd), "at+zltelc=0\r");
    }
    else if(1 == pstCellClkT->iClkEnable)
    {
        snprintf(acCmd, sizeof(acCmd), "at+zltelc=1,%s,%s\r", pstCellClkT->acFreq, pstCellClkT->acCellid);
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


static LTE_RET_E zte_micro_b57_set_oper_info_type(void)
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


static LTE_RET_E zte_micro_b57_5_oper_info_get(void *pvData, int iDataLen)
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
    iRet = zte_micro_b57_set_oper_info_type();
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


static LTE_RET_E zte_micro_b57_5_gps_start(void *pvData, int iDataLen)
{
    LTE_NORMAL_AT_T stAt = {"at+gpsstart\r","/tmp/gpsstart","OK",0,ATCOM,0,2};
    int iRet = 0;

    /*执行at指令*/
    iRet = lte_normal_at_cmd(&stAt);
    if (iRet != 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:at command failed",__LINE__,__func__);
        lte_file_unlink(stAt.acFileName);
        /*默认开机*/
        return LTE_FAIL;
    }
    
    lte_file_unlink(stAt.acFileName);
    
    if (0 == stAt.iResult)
    {
        LTE_LOG(LTE_LOG_INFO,"%d,%s:LTE GPS START OK",__LINE__,__func__);
        return LTE_OK;
    } 
    else
    {
        LTE_LOG(LTE_LOG_INFO,"%d,%s:LTE GPS STOP FAILED",__LINE__,__func__);
        return LTE_FAIL;
    }

    return LTE_OK;
}
static LTE_RET_E zte_micro_b57_5_gps_stop(void *pvData, int iDataLen)
{
    LTE_NORMAL_AT_T stAt = {"at+gpsstop\r","/tmp/gpsstop","OK",0,ATCOM,0,2};
    int iRet = 0;

    /*执行at指令*/
    iRet = lte_normal_at_cmd(&stAt);
    if (iRet != 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:at command failed",__LINE__,__func__);
        lte_file_unlink(stAt.acFileName);
        /*默认开机*/
        return LTE_FAIL;
    }
    
    lte_file_unlink(stAt.acFileName);
    
    if (0 == stAt.iResult)
    {
        LTE_LOG(LTE_LOG_INFO,"%d,%s:LTE GPS STOP OK",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_INFO,"%d,%s:LTE GPS STOP FAILED",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    return LTE_OK;
}

static LTE_RET_E zte_micro_b57_5_sup_nettype_get(void *pvData, int iDataLen)
{
    int iRet;
    int iVerSionFlag = 0;
    LTE_MODULE_VERSION_T stVersion = {0};
    
    LTE_NET_TYPE_T *pstNets = (LTE_NET_TYPE_T *)pvData;

    iRet = zte_micro_b57_n_ver_get(&stVersion, sizeof(LTE_MODULE_VERSION_T));
    if(NULL != strstr(stVersion.acVersion, "B05_CTA_OT"))
    {
        pstNets->enNetType = LTE_2G_ONLY | LTE_3G_ONLY | LTE_4G_ONLY | LTE_2G_3G |
            LTE_3G_4G | LTE_2G_3G_4G;
         pstNets->enNetTypePri = LTE_2G_PRI |LTE_3G_PRI |LTE_4G_PRI |LTE_AUTO ;
    }
    else
    {
        pstNets->enNetType = LTE_4G_ONLY;
        pstNets->enNetTypePri = LTE_NONE;
    }
    return LTE_OK;

}
static LTE_RET_E zte_micro_b57_5_cgdcont_set_cmd(void *pvData, int iDataLen)
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

    /*if (LTE_OK != zte_micro_b57_5_power_off(NULL, 0))
    {
        LTE_LOG(LTE_LOG_ERR, "power off fail");
        zte_micro_b57_5_power_off(NULL, 0);
        //return LTE_FAIL;
    }*/
    //usleep(500000);
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
    
    //zte_micro_b57_5_power_on(NULL, 0);
    
    return iRet;   

}

static LTE_RET_E zte_micro_b57_5_cgdcont_set(void *pvData, int iDataLen)
{
    int iCount;
    for(iCount = 0; iCount < 5; iCount++)
    {
        if (LTE_OK == zte_micro_b57_5_power_off(NULL, 0))
        {
            break;
        }
        sleep(1);
    }
    if(LTE_OK != zte_micro_b57_5_cgdcont_set_cmd(pvData, iDataLen))
    {
        sleep(1);
        if(LTE_OK != zte_micro_b57_5_cgdcont_set_cmd(pvData, iDataLen))
        {
            zte_micro_b57_5_power_on(NULL, 0);
            return LTE_FAIL;
        }
    }
    zte_micro_b57_5_power_on(NULL, 0);
    return LTE_OK;
}

/*****************************************************************************
 函 数 名  : zte_micro_b57_5_sms_at_csms
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
static LTE_RET_E zte_micro_b57_5_sms_at_csms(void)
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
 函 数 名  : zte_micro_b57_5_sms_at_cpms
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
static LTE_RET_E zte_micro_b57_5_sms_at_cpms(void)
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
 函 数 名  : zte_micro_b57_5_sms_at_cnmi
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
static LTE_RET_E zte_micro_b57_5_sms_at_cnmi(void)
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
   函 数 名  : zte_micro_b57_5_sms_at_cgsms
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
static LTE_RET_E zte_micro_b57_5_sms_at_cgsms(void)
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
 函 数 名  : zte_micro_b57_5_sms_at_csmp
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
static LTE_RET_E zte_micro_b57_5_sms_at_csmp(void)
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
   函 数 名  : zte_micro_b57_5_sms_mode_set
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
static LTE_RET_E zte_micro_b57_5_sms_mode_set(void *pvData, int iDataLen)
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
static LTE_RET_E zte_micro_b57_5_sms_func_init(void *pvData, int iDataLen)
{
    int iRet = 0;
    LTE_SMS_Mode_T pstMode = {0};
    pstMode.iSmsMode = 0;/* 默认设为pdu模式 */
    iRet = zte_micro_b57_5_sms_at_csms();
    if (iRet != LTE_OK)
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:zte_micro_b57_5_sms_at_csms failed\n", __LINE__, __func__);
        return LTE_FAIL;
    }
    iRet = zte_micro_b57_5_sms_at_cpms();
    if (iRet != LTE_OK)
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:zte_micro_b57_5_sms_at_cpms failed\n", __LINE__, __func__);
        return LTE_FAIL;
    }
    iRet = zte_micro_b57_5_sms_at_cnmi();
    if (iRet != LTE_OK)
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:zte_micro_b57_5_sms_at_cnmi failed\n", __LINE__, __func__);
        return LTE_FAIL;
    }
    iRet = zte_micro_b57_5_sms_at_cgsms();
    if (iRet != LTE_OK)
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:zte_micro_b57_5_sms_at_cgsms failed\n", __LINE__, __func__);
        return LTE_FAIL;
    }
    iRet = zte_micro_b57_5_sms_at_csmp();
    if (iRet != LTE_OK)
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:zte_micro_b57_5_sms_at_csmp failed\n", __LINE__, __func__);
        return LTE_FAIL;
    }
    iRet = zte_micro_b57_5_sms_mode_set(&pstMode, sizeof(LTE_SMS_Mode_T));
    if (iRet != LTE_OK)
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:zte_micro_b57_5_sms_mode_set failed\n", __LINE__, __func__);
        return LTE_FAIL;
    }

    return LTE_OK;

}

/*****************************************************************************
   函 数 名  : zte_micro_b57_5_sms_cent_code_set
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
static LTE_RET_E zte_micro_b57_5_sms_cent_code_set(void *pvData, int iDataLen)
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
   函 数 名  : zte_micro_b57_5_sms_recv
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
static LTE_RET_E zte_micro_b57_5_sms_recv(void *pvData, int iDataLen)
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
   函 数 名  : zte_micro_b57_5_sms_send
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
static LTE_RET_E zte_micro_b57_5_sms_send(void *pvData, int iDataLen)
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
   函 数 名  : zte_micro_b57_5_sms_dele
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
static LTE_RET_E zte_micro_b57_5_sms_dele(void *pvData, int iDataLen)
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
/*****************************************************************************
 函 数 名  : zte_micro_b57_5_sms_sup_func
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
static LTE_RET_E zte_micro_b57_5_sms_sup_func(void *pvData, int iDataLen)
{
    int flag = 0;
    LTE_SUPPORT_SMS_T *pstSup = (LTE_SUPPORT_SMS_T *)pvData;
    pstSup->enSupSms = SUP_SMS_FUNC;
    return LTE_OK;

}
/*****************************************************************************
   函 数 名  : zte_micro_b57_5_apn_set
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
static LTE_RET_E zte_micro_b57_5_apn_set(void *pvData, int iDataLen)
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
    if (LTE_OK != zte_micro_b57_5_power_off(NULL, 0))
    {
        LTE_LOG(LTE_LOG_ERR, "power off fail");
        return LTE_FAIL;
    }

    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        zte_micro_b57_5_power_on(NULL, 0);
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
        zte_micro_b57_5_power_on(NULL, 0);
        return LTE_FAIL;
    }

    iRet = lte_client_recv(iClientSocketFd, &stLteRecvMsg, 5);
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_recv failed!\n",__LINE__,__func__);
        close(iClientSocketFd);
        zte_micro_b57_5_power_on(NULL, 0);
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
    
    zte_micro_b57_5_power_on(NULL, 0);
   
    return iRet;   

}
static LTE_RET_E zte_micro_b57_n_ver_get(void *pvData, int iDataLen)
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

/*****************************************************************************
 函 数 名  : zte_micro_b57_n_enodeb_id_get
 功能描述  : 获取enodeB ID
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
static LTE_RET_E zte_micro_b57_n_enodeb_id_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+zemci=1";
    char acCiev[] = "+ZEMCI:";
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
    stLteMsg.iRealFlag = 1;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
    stLteMsg.iComReadType = COM_READ_TYPE_8;
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
        zte_micro_b57_stop_zemci(LTE_ENODEBID_GET);
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
    zte_micro_b57_stop_zemci(LTE_ENODEBID_GET);
    return iRet;
}

/*****************************************************************************
 函 数 名  : zte_micro_b57_n_tac_get
 功能描述  : 获取小区所属的区位跟踪码
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
static LTE_RET_E zte_micro_b57_n_tac_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+zemci=1";
    char acCiev[] = "+ZEMCI:";
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
    stLteMsg.iRealFlag = 1;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
    stLteMsg.iComReadType = COM_READ_TYPE_8;
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
        zte_micro_b57_stop_zemci(LTE_TAC_GET);
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
    zte_micro_b57_stop_zemci(LTE_TAC_GET);
    return iRet;

}

static LTE_RET_E zte_micro_b57_n_lcid_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+zemci=1";
    char acCiev[] = "+ZEMCI:";
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
    stLteMsg.iRealFlag = 1;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
    stLteMsg.iComReadType = COM_READ_TYPE_8;
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
        zte_micro_b57_stop_zemci(LTE_LCID_GET);
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
    zte_micro_b57_stop_zemci(LTE_LCID_GET);
    return iRet;
    
}

static LTE_RET_E zte_micro_b57_n_all_cellid_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+zemci=1";
    char acCiev[] = "+ZEMCI:";
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
    stLteMsg.iRealFlag = 1;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
    stLteMsg.iComReadType = COM_READ_TYPE_8;
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
        zte_micro_b57_stop_zemci(LTE_ALL_CELLID_GET);
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
    zte_micro_b57_stop_zemci(LTE_ALL_CELLID_GET);
    return iRet;
    
}

static LTE_RET_E zte_micro_b57_n_mac_set(void *pvData, int iDataLen)
{
    return LTE_OK;
}

static LTE_RET_E sim_unlock_communication(char *pcCmd, LTE_SIM_RANDOM_NUM_T *pstSimRandomNum, int iFuncCmd, int iRealFlag)
{
    int iClientSocketFd = -1;
	int iRet = 0;
	//char acCmd[] = "AT+ZUTP=FF,FF,DF,FF,7F,7F,00,DF,FF,03,02,1F,7F,81,81,2F,03,2F,00,00";
	char acCiev[] = "+ZU";//"+ZUPCI:";"+ZUEND:";

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

	stLteMsg.iComReadType = COM_READ_TYPE_8;
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
       if(NULL != pstSimRandomNum)
       {
            pstSimRandomNum->iSIMType = stLteRecvMsg.uLteCommand.stSimRandomNum.iSIMType;
            strncpy(pstSimRandomNum->cRandomNum, stLteRecvMsg.uLteCommand.stSimRandomNum.cRandomNum, sizeof(pstSimRandomNum->cRandomNum));
       }
	   iRet = LTE_OK;
	}
	else
	{
	   iRet = LTE_FAIL;
	}

	return iRet;
}

static LTE_RET_E string_not_calculate(char *src, char *dst)
{
    char acHex[] = "0123456789ABCDEF";
    while('\0' != *src)
    {
        if(*src >= '0' && *src <= '9')
        {
            *dst = acHex[15-(*src-'0')];
        }
        else if(*src >= 'A' && *src <= 'F')
        {
            *dst = acHex[5-(*src-'A')];
        }
        else if(*src >= 'a' && *src <= 'f')
        {
            *dst = acHex[5-(*src-'a')];
        }
        else
        {
            return LTE_FAIL;
        }
        ++src;
        ++dst;
    }
    return LTE_OK;
}

static LTE_RET_E zte_micro_b57_n_sim_type_get(void *pvData, int iDataLen)
{
    LTE_SIM_RANDOM_NUM_T stSimRandomNum = {0};
    char acCmd1[] = "AT+ZUTP=FF,FF,DF,FF,7F,7F,00,DF,FF,03,02,1F,7F,81,81,2F,03,2F,00,00";
    
    if(NULL == pvData)
    {
        return LTE_FAIL;
    }
    LTE_SIM_TYPE_E *penSIMType = (LTE_SIM_TYPE_E*)pvData;

    if(LTE_FAIL == sim_unlock_communication(acCmd1, &stSimRandomNum, LTE_SIM_TYPE_GET, 1))
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:AT+ZUTP=FF,FF,DF,FF,7F,7F,00,DF,FF,03,02,1F,7F,81,81,2F,03,2F,00,00",__LINE__,__func__);
        return LTE_FAIL;
    }
    *penSIMType = stSimRandomNum.iSIMType;
   
    return LTE_OK;
}


static LTE_RET_E zte_micro_b57_n_sim_unlock(void *pvData, int iDataLen)
{
    LTE_SIM_RANDOM_NUM_T stSimRandomNum = {0};
    LTE_ICCID_T stIccid = {0};
    char acCmd1[] = "AT+ZUTP=FF,FF,DF,FF,7F,7F,00,DF,FF,03,02,1F,7F,81,81,2F,03,2F,00,00";
    char acCmd2[256] = {0};
    char acRand1[64] = {0};
    char acRand2[64] = {0};
    char acMainSecretKey[33] = {0};
    char acIccidSecretKey[33] = {0};
    char acRand1SecretKey[33] = {0};
    char acSecretResult[33] = {0};
    char acStrNot[32] = {0};
    char acIccidLast8[32] = {0};
    int iSecretKeyNum ;
    int iCount;
    
    if(NULL == pvData)
    {
        return LTE_FAIL;
    }
    LTE_SIM_SECRET_KEY_T *pcSecretKey = (LTE_SIM_SECRET_KEY_T*)pvData;
    if(strlen(pcSecretKey->acSecretKey) != 160)
    {
        LTE_LOG(LTE_LOG_ERR,"Secret Key length error");
        return LTE_FAIL;
    }
    if(LTE_FAIL == sim_unlock_communication(acCmd1, &stSimRandomNum, LTE_SIM_TYPE_GET, 1))
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:AT+ZUTP=FF,FF,DF,FF,7F,7F,00,DF,FF,03,02,1F,7F,81,81,2F,03,2F,00,00",__LINE__,__func__);
        return LTE_FAIL;
    }
    if(ENCRYPTION_SIM == stSimRandomNum.iSIMType)
    {
        if(LTE_FAIL == zte_micro_b57_5_iccid_get((void *)&stIccid, sizeof(LTE_ICCID_T)))
        {
            LTE_LOG(LTE_LOG_ERR,"zte_micro_b57_n_sim_unlock:get iccid error!!!!");
            return LTE_FAIL;
        }
        /*翻转iccid高低位*/
        for(iCount=0; iCount<16; iCount+=2)
        {
            acIccidLast8[iCount] = stIccid.acICCID[iCount+5];
            acIccidLast8[iCount+1] = stIccid.acICCID[iCount+4];
        }
        
        
        memcpy(acRand1, stSimRandomNum.cRandomNum, 8);
        strcpy(acRand1+8, "20202020");
        memcpy(acRand2, stSimRandomNum.cRandomNum+8, 32);
        if(acRand1[7]>='0' && acRand1[7]<='9')
        {
            iSecretKeyNum = (acRand1[7]&0x07)%5;
        }
        else
        {
            iSecretKeyNum = ((acRand1[7]+1)&0x07)%5;
        }
        //printf("rand number== %s, %s, %s, %d\n", stSimRandomNum.cRandomNum, acRand1, acRand2, iSecretKeyNum);
        memcpy(acMainSecretKey, pcSecretKey->acSecretKey+32*iSecretKeyNum, 32);
        /*ICCID分离*/
        Do_3DES(acIccidLast8,acMainSecretKey,acIccidSecretKey,'e');
        string_not_calculate(acIccidLast8, acStrNot);
        //printf("acIccidLast8==%s,%s, %s\n", acIccidLast8, acMainSecretKey, acStrNot);
        Do_3DES(acStrNot,acMainSecretKey,acIccidSecretKey+16,'e');
        //printf("acIccidSecretKey== %s\n", acIccidSecretKey);
        /*Rand1分离*/
        Do_3DES(acRand1,acIccidSecretKey,acRand1SecretKey,'e');
        memset(acStrNot, 0, sizeof(acStrNot));
        string_not_calculate(acRand1, acStrNot);
        Do_3DES(acStrNot,acIccidSecretKey,acRand1SecretKey+16,'e');
        /*认证算法*/
        Do_3DES(acRand2,acRand1SecretKey,acSecretResult,'e');
        Do_3DES(acRand2+16,acRand1SecretKey,acSecretResult+16,'e');
        
        sprintf(acCmd2, "AT+ZUTR=35,3,1,0,,8,\"%02X%02X%s\"", pcSecretKey->man_id, pcSecretKey->key_ver, acSecretResult);
        //printf("acCmd2==%s\n", acCmd2);
        if(LTE_FAIL == sim_unlock_communication(acCmd2, NULL, LTE_SIM_UNLOCK, 0))
        {
            LTE_LOG(LTE_LOG_ERR,"%d,%s:%s",__LINE__,__func__, acCmd2);
            return LTE_FAIL;
        }
        
    }
    else if(NORMAL_SIM == stSimRandomNum.iSIMType)
    {
        printf("normal SIM card\n");
    }
   
    return LTE_OK;
}

static LTE_RET_E zte_micro_b57_n_sim_disable_unlock(void *pvData, int iDataLen)
{
    return LTE_OK;
}


static LTE_RET_E zte_micro_b57_n_sim_unlock_support(void *pvData, int iDataLen)
{
    LTE_UNLOCK_SUPPORT_T *pstUnlockSupport = (LTE_UNLOCK_SUPPORT_T* )pvData;
    if(NULL == pstUnlockSupport || iDataLen < sizeof(LTE_UNLOCK_SUPPORT_T))
    {
        return LTE_FAIL;
    }
    pstUnlockSupport->iUnlockFlag = 1;
    return LTE_OK;
}

static LTE_RET_E zte_micro_b57_n_sim_unlock_get(void *pvData, int iDataLen)
{
    int iRet = 0;
    int iClientSocketFd = -1;
    char acCmd[] = "at";
    char acCiev[] = "OK";
    char *pcBuf = NULL;
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

static LTE_RET_E zte_micro_b57_5_signal_value_get(void *pvData, int iDataLen)
{
    int iSignal;
    if(NULL == pvData)
    {
        return LTE_FAIL;
    }
    LTE_SIGNAL_VALUE_T *piSignalValue = (LTE_SIGNAL_VALUE_T *)pvData;
    if(LTE_OK != zte_micro_b57_5_original_signal_get(&iSignal))
    {
        return LTE_FAIL;
    }
    if(LTE_OK != zte_micro_b57_5_signal_handle(NULL , piSignalValue, iSignal, 2))
    {
        return LTE_FAIL;
    }
    return LTE_OK;
}

static LTE_RET_E zte_micro_b57_5_ip_gw_dns_get(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+cgact=1,1";
    char acCiev[] = "+ZGIPDNS:";
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
        
    LTE_IP_GATEWAY_DNS_T *pstIpGatewayDns = (LTE_IP_GATEWAY_DNS_T *)pvData;
    iClientSocketFd = lte_client_sock_init();

    if(iClientSocketFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_client_sock_init failed, create socket failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    
    stLteMsg.iFuncCmd = LTE_IP_GATEWAY_DNS_GET;
    stLteMsg.iPriority = 0;
    stLteMsg.iRealFlag = 0;
	strncpy(stLteMsg.acAtCmd, acCmd, sizeof(stLteMsg.acAtCmd));
	strncpy(stLteMsg.acCiev, acCiev, sizeof(stLteMsg.acCiev));
    
	LTE_LOG(LTE_LOG_INFO,"%s %s\n", stLteMsg.acAtCmd, stLteMsg.acCiev);
    
    stLteMsg.iComReadType = COM_READ_TYPE_8;
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
        *pstIpGatewayDns = stLteRecvMsg.uLteCommand.stIpGatewayDns;
        iRet = LTE_OK;
    }
    else
    {
        iRet = LTE_FAIL;
    }
    return iRet;
}


static LTE_RET_E zte_micro_b57_n_all_plmn_mode_get(void *pvData, int iDataLen)
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
        if (LTE_OK == zte_micro_b57_5_power_off(NULL, 0))
        {
            break;
        }
        sleep(1);
    }
    sleep(1);
    for(iCount=0; iCount<5; iCount++)
    {
        if (LTE_OK == zte_micro_b57_5_power_on(NULL, 0))
        {
            break;
        }
        sleep(1);
    }
    
    for(iCount=0; iCount<60; iCount++)
    {
        if (LTE_OK == zte_micro_b57_5_network_prepare(NULL, 0))
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

static LTE_RET_E zte_micro_b57_n_sounet_mode_set(void *pvData, int iDataLen)
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

    if(LTE_OK != zte_micro_b57_5_nettype_set((void *)&stNetType, sizeof(LTE_NET_TYPE_T)))
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

static LTE_RET_E zte_micro_b57_n_puk_unlock(void *pvData, int iDataLen)
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

static LTE_RET_E zte_micro_b57_n_module_up_support(void *pvData, int iDataLen)
{
    
    LTE_MODULE_UP_SUPPORT_T *pstModuleUpSupp = (LTE_MODULE_UP_SUPPORT_T* )pvData;
    LTE_MODULE_VERSION_T stVersion = {0};
    if(NULL == pstModuleUpSupp || iDataLen < sizeof(LTE_MODULE_UP_SUPPORT_T))
    {
        return LTE_FAIL;
    }
    zte_micro_b57_n_ver_get(&stVersion, sizeof(LTE_MODULE_VERSION_T));
    if(0 == strcmp(stVersion.acVersion, "B78-V1.0.0B02_TDD38_39_40") || stVersion.iVersionIndex == LTE_ZTE_MICRO_B57_5 || 0 == strcmp(stVersion.acVersion, "B57_1-V1.0.0B05_TDD38_39_40"))
    {
        pstModuleUpSupp->iModuleUpFlag = MODULE_DISABLE_UP;
    }
    else
    {
        pstModuleUpSupp->iModuleUpFlag = MODULE_SUPPORT_UP;
    }
    
    return LTE_OK;
}
/*****************************************************************************
 函 数 名  : zte_micro_b57_n_pin_input_remain
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

static LTE_RET_E zte_micro_b57_n_pin_input_remain(void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "AT+ZRAP?";
    char acCiev[] = "+ZRAP:";
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



LTE_IOCTL_FUNC_T astZtemicroB57_5Table[] = 
{    
    {LTE_PIN_STATE,         zte_micro_b57_5_pin_state,           0,0,    "zte_micro_b57_5_pin_state"       }, 
    {LTE_PIN_SWITCH,        zte_micro_b57_5_pin_switch,          0,0,    "zte_micro_b57_5_pin_switch"       }, 
    {LTE_PIN_SW_GET,        zte_micro_b57_5_pin_sw_get,          0,0,    "zte_micro_b57_5_pin_sw_get"       },
    {LTE_PIN_UNLOCK,        zte_micro_b57_5_pin_unlock,          0,0,    "zte_micro_b57_5_pin_unlock"       }, 
    {LTE_PIN_ATTEMPTS,      NULL,           0,0,    NULL       }, 
    {LTE_PIN_ALTER,         zte_micro_b57_5_pin_alter,           0,0,    "zte_micro_b57_5_pin_alter"       }, 
    {LTE_SIGNAL_GET,        zte_micro_b57_5_signal_get,          0,0,    "zte_micro_b57_5_signal_get"       }, 
    {LTE_POWER_ON,          zte_micro_b57_5_power_on,            0,0,    "zte_micro_b57_5_power_on"       }, 
    {LTE_POWER_OFF,         zte_micro_b57_5_power_off ,          0,0,    "zte_micro_b57_5_power_off "       }, 
    {LTE_NETTYPE_SET,       zte_micro_b57_5_nettype_set,         0,0,    "zte_micro_b57_5_nettype_set"       }, 
    {LTE_NETTYPE_GET,       zte_micro_b57_5_nettype_get,         0,0,    NULL       }, 
    {LTE_PLMN_REGISTER,     NULL,           0,0,    NULL       }, 
    {LTE_PREFER_PLMN_SET,   NULL,           0,0,    NULL       }, 
    {LTE_NETWORK_PREPARE,   zte_micro_b57_5_network_prepare,     0,0,    "zte_micro_b57_5_network_prepare"       }, 
    {LTE_NETWORK_STATUS_GET,zte_micro_b57_5_network_status_get,  0,0,    "zte_micro_b57_5_network_status_get"       },
    {LTE_CONNECT,           zte_micro_b57_5_connect,             0,0,    "zte_micro_b57_5_connect"       }, 
    {LTE_DISCONNECT,        zte_micro_b57_5_disconnect,          0,0,    "zte_micro_b57_5_disconnect"       }, 
    {LTE_LINKSTATE_GET,     zte_micro_b57_5_linkstate_get,       0,0,    "zte_micro_b57_5_linkstate_get"       }, 
    {LTE_IMEI_GET,          zte_micro_b57_5_imei_get,            0,0,    "zte_micro_b57_5_imei_get"       }, 
    {LTE_CELLID_GET,        zte_micro_b57_5_cellid_get,          0,0,    "zte_micro_b57_5_cellid_get"       }, 
    {LTE_ICCID_GET,         zte_micro_b57_5_iccid_get,           0,0,    "zte_micro_b57_5_iccid_get"       },
    {LTE_IMSI_GET,          zte_micro_b57_5_imsi_get,            0,0,    "zte_micro_b57_5_imsi_get"       },
    {LTE_SIM_STATE_GET,     zte_micro_b57_5_simstate_get,        0,0,    "zte_micro_b57_5_simstate_get"       }, 
    {LTE_INFO_GET,          zte_micro_b57_5_lte_info_get,        0,0,    "zte_micro_b57_5_lte_info_get"       }, 
    {LTE_ACCESS_NET_GET,    zte_micro_b57_5_access_net_get,      0,0,    "zte_micro_b57_5_access_net_get"       }, 
    {LTE_BANDS_GET,         zte_micro_b57_5_bands_get,           0,0,    "zte_micro_b57_5_bands_get"       }, 
    {LTE_CLCK_BANDS_SET,    zte_micro_b57_5_clck_bands_set,      0,0,    "zte_micro_b57_5_clck_bands_set"       }, 
    {LTE_CLCK_BANDS_CLOSE,  zte_micro_b57_5_clck_bands_close,      0,0,    "zte_micro_b57_5_clck_bands_set"       },
    {LTE_CLCK_CELL_GET,     zte_micro_b57_5_clckcell_enable_get, 0,0,    "zte_micro_b57_5_clckcell_enable_get"   },
    {LTE_CLCK_CELL_SET,     zte_micro_b57_5_clck_cellid_set,     0,0,    "zte_micro_b57_5_clck_cellid_set"       },
    {LTE_OPER_INFO_GET,     zte_micro_b57_5_oper_info_get,       0,0,    "zte_micro_b57_5_oper_info_get"          }, 
    {LTE_NETTYPE_INFO_GET,  zte_micro_b57_5_sup_nettype_get,           0,0,    "zte_micro_b57_5_sup_nettype_get"          },
    {LTE_GPS_START,         NULL,           0,0,    NULL       }, 
    {LTE_GPS_STOP,          NULL,           0,0,    NULL       }, 
    {LTE_GPS_DATE_GET,      NULL,           0,0,    NULL       }, 
    {LTE_GPS_POSITION_INFO_GET,        NULL,           0,0,    NULL       }, 
     /* BEGIN: Added by wujian, 2014/11/13   PN:添加短信功能相关接口 */
    {LTE_SMS_CENT_CODE_SET, zte_micro_b57_5_sms_cent_code_set, 0, 0, "zte_micro_b57_5_sms_cent_code_set" },
    {LTE_SMS_MODE__SET,     zte_micro_b57_5_sms_mode_set, 0, 0, "zte_micro_b57_5_sms_mode_set"   },
    {LTE_SMS_FUNC_INIT,     zte_micro_b57_5_sms_func_init, 0, 0, "zte_micro_b57_5_sms_func_init"   },
    {LTE_SMS_RECV,          zte_micro_b57_5_sms_recv, 0, 0,   "zte_micro_b57_5_sms_recv" },
    {LTE_SMS_SEND,          zte_micro_b57_5_sms_send, 0, 0, "zte_micro_b57_5_sms_send"   },
    {LTE_SMS_DELE,          zte_micro_b57_5_sms_dele, 0, 0, "zte_micro_b57_5_sms_dele"   },
    {LTE_SMS_SUPP,          zte_micro_b57_5_sms_sup_func, 0, 0, "zte_micro_b57_5_sms_sup_func"   },
    /* END:   Added by wujian, 2014/11/13   PN:添加短信功能相关接口 */
    {LTE_IPV6_PDP_SET,      zte_micro_b57_5_cgdcont_set, 0,0,  "zte_micro_b57_5_cgdcont_set"   },
    {LTE_APN_SET,           zte_micro_b57_5_apn_set,     0,0,  "zte_micro_b57_5_apn_set"       }, 
    {LTE_VERSION_GET,       zte_micro_b57_n_ver_get,     0,0,  "zte_micro_b57_n_ver_get"       }, 
    {LTE_ENODEBID_GET,      zte_micro_b57_n_enodeb_id_get,     0,0,  "zte_micro_b57_n_enodeb_id_get"       }, 
    {LTE_TAC_GET,           zte_micro_b57_n_tac_get,     0,0,  "zte_micro_b57_n_tac_get"       },
    {LTE_LCID_GET,          zte_micro_b57_n_lcid_get,     0,0,  "zte_micro_b57_n_lcid_get"       }, 
    {LTE_ALL_CELLID_GET,    zte_micro_b57_n_all_cellid_get,     0,0,  "zte_micro_b57_n_all_cellid_get"       }, 
    {LTE_MOUDLE_MAC_SET,    zte_micro_b57_n_mac_set,         0,0, "zte_micro_b57_n_mac_set"    },
    /* BEGIN: Added by wangqi, 2015/5/5   PN:添加设置机卡互锁接口 */
    {LTE_SIM_UNLOCK,        zte_micro_b57_n_sim_unlock, 0,0, "zte_micro_b57_n_sim_unlock"},
    {LTE_SIM_DISABLE_UNLOCK, zte_micro_b57_n_sim_disable_unlock,     0,0, NULL},    
    {LTE_SIM_UNLOCK_SUPPORT,zte_micro_b57_n_sim_unlock_support, 0,0, "zte_micro_b57_n_sim_unlock_support"},
    {LTE_SIM_TYPE_GET,      zte_micro_b57_n_sim_type_get, 0,0, "zte_micro_b57_n_sim_type_get"},
    {LTE_SIM_UNLOCK_GET,    zte_micro_b57_n_sim_unlock_get, 0,0,  "zte_micro_b57_n_sim_unlock_get"},
    /* END:   Added by wangqi, 2015/5/5   PN:添加设置机卡互锁接口 */ 
    {LTE_SIGNAL_VALUE_GET,  zte_micro_b57_5_signal_value_get, 0,0,    "zte_micro_b57_5_signal_value_get"},
    {LTE_IP_GATEWAY_DNS_GET,zte_micro_b57_5_ip_gw_dns_get, 0,0,  "zte_micro_b57_5_ip_gw_dns_get"},
    {LTE_ALL_PLMN_MODE_GET,      zte_micro_b57_n_all_plmn_mode_get,  0,0, "zte_micro_b57_n_all_plmn_mode_get"    },
    {LTE_SOUNET_MODE_SET,   zte_micro_b57_n_sounet_mode_set,  0,0, "zte_micro_b57_n_sounet_mode_set"    },
    {LTE_SIM_UNLOCK_PUK,	zte_micro_b57_n_puk_unlock, 0,0, "zte_micro_b57_n_puk_unlock"	},
    {LTE_MODULE_UP_SUPPORT, zte_micro_b57_n_module_up_support, 0,0, "zte_micro_b57_n_module_up_support"},
    {LTE_PIN_INPUT_REMAIN_GET,  zte_micro_b57_n_pin_input_remain, 0,0, "zte_micro_b57_n_pin_input_remain"},
    
    {LTE_CMD_MAX,           NULL,           0,0,    NULL       }, 
};

