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

static LTE_RET_E huawei_power_on (void *pvData, int iDataLen)
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

#if 0
static LTE_RET_E huawei_power_on (void *pvData, int iDataLen)
{
    LTE_NORMAL_AT_T stAt = {"at+cfun=1\r","/tmp/cfun","OK",0,ATCOM,0,3};
    int iRet = 0;

    /*执行at指令*/
    iRet = lte_normal_at_cmd(&stAt);
    if (iRet != 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:at command failed\n",__LINE__,__func__);
        lte_file_unlink(stAt.acFileName);
        /*默认开机*/
        return LTE_FAIL;
    }
    
    lte_file_unlink(stAt.acFileName);
    
    if (0 == stAt.iResult)
    {
        LTE_LOG(LTE_LOG_INFO,"%d,%s:LTE POWER ON\n",__LINE__,__func__);
        return LTE_OK;
    } 
    return LTE_OK;
}
#endif

static LTE_RET_E huawei_power_off (void *pvData, int iDataLen)
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
#if 0
static LTE_RET_E huawei_power_off (void *pvData, int iDataLen)
{
    LTE_NORMAL_AT_T stAt = {"at+cfun=0\r","/tmp/cfunoff","ERROR",0,ATCOM,0,1};
    int iRet = 0;
    /*执行at指令*/
    iRet = lte_normal_at_cmd(&stAt);
    if (iRet != 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:at command failed\n",__LINE__,__func__);
        lte_file_unlink(stAt.acFileName);
        /*默认开机*/
        return LTE_FAIL;
    }
    
    lte_file_unlink(stAt.acFileName);
    
    if (0 == stAt.iResult)
    {
        LTE_LOG(LTE_LOG_INFO,"%d,%s:LTE POWER OFF ERROR\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    else
    {
        LTE_LOG(LTE_LOG_INFO,"%d,%s:LTE POWER OFF OK\n",__LINE__,__func__);
        return LTE_OK;
    }
    
    return LTE_OK;
}
#endif

static LTE_RET_E huawei_signal_get (void *pvData, int iDataLen)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char acCmd[] = "at+csq";
    char acCiev[] = "+CSQ:";
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
#if 0
static LTE_RET_E huawei_signal_get (void *pvData, int iDataLen)
{
    unsigned int uiFlag = 0;
    int iRet = 0;
    char acCiev[] = "+CSQ:";
    char acNeedleComma[] = ",";
    char acBuff[64] = {0};
    char acSignalTmp[64] = {0};
    char acBuffTmp[64] = {0};
    char *pcTmp = NULL;
    FILE* pstFd = NULL;
    LTE_RET_E enRet = 0;

    LTE_SIGNAL_T *pstSignal = (LTE_SIGNAL_T*)pvData;
    LTE_NORMAL_AT_T stAt = {"at+csq\r","/tmp/signalquality","OK",0,ATCOM,0,4};
    /*执行at指令*/
    iRet = lte_normal_at_cmd(&stAt);
    if (iRet != 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:at command failed\n",__LINE__,__func__);
        lte_file_unlink(stAt.acFileName);
        /*默认开机*/
        return LTE_FAIL;
    }
    if (0 != stAt.iResult)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:compare AT return value not OK\n",__LINE__,__func__);
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
        pcTmp = strstr(acBuff,acCiev); 
        if(pcTmp != NULL)
        {
            uiFlag = 1;
            break;      
        }
        memset(acBuff,0,64);
    } 
    fclose(pstFd);
    pstFd = NULL;
    lte_file_unlink(stAt.acFileName);
    if(uiFlag == 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:at excute failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    sscanf(acBuff, "%[^:]:%[^,]", acBuffTmp, acSignalTmp);  /*+CSQ: 6,99*/
    pstSignal->uiSignal = atoi(acSignalTmp);
    return LTE_OK;
}
#endif
static LTE_RET_E huawei_linkstate_get(void *pvData, int iDataLen)
{
	if(pvData == NULL)
		return LTE_FAIL;

	LTE_LINKSTATE_T *pstLinkState = (LTE_LINKSTATE_T *)pvData;
	if ( NULL == pstLinkState)
    {
		pstLinkState->enLinkstate = LTE_LINKSTATE_CONNECT_FAIL;
		return LTE_OK;
	}
	int iRet = 0;
    iRet = lte_has_valid_ip();
    if (LTE_OK == iRet)
    {
		pstLinkState->enLinkstate = LTE_LINKSTATE_CONNECTED;
    	return LTE_OK;

    }

    iRet = lte_has_valid_process("/usr/sbin/pppd","/usr/sbin/pppd call");
    if (LTE_OK == iRet)
    {
		pstLinkState->enLinkstate = LTE_LINKSTATE_CONNECTING;
    }
    else
    {
    	pstLinkState->enLinkstate = LTE_LINKSTATE_DISCONNECT;
    }

	return LTE_OK;
}

static LTE_RET_E huawei_imei_get (void *pvData, int iDataLen)
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
#if 0
static LTE_RET_E huawei_imei_get (void *pvData, int iDataLen)
{
    int  iRet = 0;
    FILE *pstFd = NULL;
    char acBuff[64];
    char acBuffTmp[64];
        
    LTE_IMEI_T *pstImeiInfo = (LTE_IMEI_T *)pvData;
    LTE_NORMAL_AT_T stAt = {"at+gsn\r","/tmp/imei","OK",0,ATCOM,0,2};

    if (pvData == NULL)
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
        LTE_LOG(LTE_LOG_ERR,"%d,%s:AT command return not OK",__LINE__,__func__);
        lte_file_unlink(stAt.acFileName);
        return LTE_FAIL;
    } 
    
    pstFd = fopen(stAt.acFileName,"r");
    if (NULL == pstFd)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:open %s failed\n",__LINE__,__func__,stAt.acFileName);
        lte_file_unlink(stAt.acFileName);
        return -1;
    }
    
    while (NULL != fgets(acBuff,64,pstFd))
    { 	
        if((acBuff[0] >= '0') && (acBuff[0] <= '9'))
        {
            sscanf(acBuff, "%s", pstImeiInfo->acImei);
            break;
        }

        memset(acBuff,0,64);
    } 

    fclose(pstFd);
    pstFd = NULL;

    lte_file_unlink(stAt.acFileName);
        
    LTE_LOG(LTE_LOG_INFO,"%d,%s: sim imei: %s",__LINE__,__func__, pstImeiInfo->acImei);
    return LTE_OK;
   
}
#endif
static LTE_RET_E huawei_imsi_get(void *pvData, int iDataLen)
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
#if 0
static LTE_RET_E huawei_imsi_get(void *pvData, int iDataLen)
{
    int  iRet = 0;
    FILE *pstFd = NULL;
    char acBuff[64];
    char acBuffTmp[64];
        
    LTE_IMSI_T *pstImsiInfo = (LTE_IMSI_T *)pvData;
    LTE_NORMAL_AT_T stAt = {"at+cimi\r","/tmp/imsi","OK",0,ATCOM,0,2};

    if (pvData == NULL)
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
        LTE_LOG(LTE_LOG_ERR,"%d,%s:AT command return not OK",__LINE__,__func__);
        lte_file_unlink(stAt.acFileName);
        return LTE_FAIL;
    } 
    
    pstFd = fopen(stAt.acFileName,"r");
    if (NULL == pstFd)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:open %s failed\n",__LINE__,__func__,stAt.acFileName);
        lte_file_unlink(stAt.acFileName);
        return -1;
    }
    
    while (NULL != fgets(acBuff,64,pstFd))
    { 	
        if((acBuff[0] >= '0') && (acBuff[0] <= '9'))
        {
            sscanf(acBuff, "%s", pstImsiInfo->acIMSI);
            break;
        }

        memset(acBuff,0,64);
    } 

    fclose(pstFd);
    pstFd = NULL;

    lte_file_unlink(stAt.acFileName);
        
    LTE_LOG(LTE_LOG_INFO,"%d,%s: sim imsi: %s",__LINE__,__func__, pstImsiInfo->acIMSI);
    return LTE_OK;
}
#endif

static LTE_RET_E huawei_simstate_get(void *pvData, int iDataLen)
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

#if 0
static LTE_RET_E huawei_simstate_get(void *pvData, int iDataLen)
{
	if (pvData == NULL)
		return LTE_FAIL;
	
	int iRet = 0; 

	LTE_SIM_STATE_T *pstSimState = (LTE_SIM_STATE_T *)pvData;
	LTE_NORMAL_AT_T stAt = {"at+cpin?\r","/tmp/pinstate","ERROR",0,ATCOM,0,1};

    	/*执行at指令*/
   	iRet = lte_normal_at_cmd(&stAt);
	if (iRet != 0)
	{
    	LTE_LOG(LTE_LOG_ERR,"%d,%s:at command failed\n",__LINE__,__func__);
    	lte_file_unlink(stAt.acFileName);
    	return LTE_FAIL;
	}
	lte_file_unlink(stAt.acFileName);
	if (stAt.iResult == -1)
	{
		pstSimState->enSimState = LTE_SIM_READY;
	}else if (stAt.iResult == 0){
		pstSimState->enSimState = LTE_SIM_NOT_FOUND;
	}else
		return LTE_FAIL;
	
	return LTE_OK;
}
#endif

static int huawei_cgreg_get(char *pcCmd, char *pcCiev)
{
    int iClientSocketFd = -1;
    int iRet = 0;
    char *pcBuf = NULL;
    
    CLIENT_SEND_MSG_T stLteMsg = {0};
    LTE_SEND_MSG_T stLteRecvMsg = {0};
    LTE_NETWORK_PREPARE_T stNwStatusInfo;
    int iCreg = 0;
    
    LTE_LOG(LTE_LOG_ERR,"%d,%s:huawei_cgreg_get failed\n",__LINE__,__func__);
    
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

static LTE_RET_E huawei_network_prepare(void *pvData, int iDataLen)
{
#if 0
    int iRegsiterStatus = 0;

    char acNeedleCgreg[16] = "+CGREG";  
    char acCmdCgreg[16] =  "at+cgreg?\r";
    
    LTE_NETWORK_PREPARE_T *pstNwStatus = (LTE_NETWORK_PREPARE_T *)pvData;
        /*0 未驻留，1 驻留，5 漫游驻留，3 驻留被拒绝，2正在收网，4 unkown*/
        /*用于td-s/gsm/gprs/edge*/
        iRegsiterStatus = lte_get_cell_register_status(acCmdCgreg,acNeedleCgreg, ATCOM);
    sprintf(pstNwStatus->acNetWorkStatus,"%d", iRegsiterStatus);

        if (-1 == iRegsiterStatus)
        {
            LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_get_cell_register_status command error\n",__LINE__,__func__);
            return  LTE_FAIL;
        }

        if ((1 == iRegsiterStatus) || (5 == iRegsiterStatus))
        {
            return LTE_OK;
        }

        return LTE_FAIL;
    #endif
    int iRegsiterStatus = 1;
    LTE_NETWORK_PREPARE_T *pstNwStatus = (LTE_NETWORK_PREPARE_T *)pvData;
    sprintf(pstNwStatus->acNetWorkStatus, "%d", iRegsiterStatus);
    
    return LTE_OK;
}

static LTE_RET_E huawei_network_status_get(void *pvData, int iDataLen)
{
#if 1
    int iRegsiterStatus = 0;

    char acNeedleCgreg[16] = "+CGREG";  
    char acCmdCgreg[16] =  "at+cgreg?";

    LTE_NETWORK_PREPARE_T *pstNwStatus = (LTE_NETWORK_PREPARE_T *)pvData;
    /*0 未驻留，1 驻留，5 漫游驻留，3 驻留被拒绝，2正在收网，4 unkown*/
    /*用于td-s/gsm/gprs/edge*/
    //iRegsiterStatus = lte_get_cell_register_status(acCmdCgreg,acNeedleCgreg, ATCOM);
    iRegsiterStatus = huawei_cgreg_get(acCmdCgreg, acNeedleCgreg);
    sprintf(pstNwStatus->acNetWorkStatus,"%d", iRegsiterStatus);

    if (-1 == iRegsiterStatus)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_get_cell_register_status command error\n",__LINE__,__func__);
        return  LTE_FAIL;
    }

    if ((1 == iRegsiterStatus) || (5 == iRegsiterStatus))
    {
        return LTE_OK;
    }

    return LTE_FAIL;
#endif
}
static LTE_RET_E huawei_connect(void *pvData, int iDataLen)
{
    /*走wcdma 的拨号流程*/
    system_call("/usr/sbin/pppd call huawei_wcdma 1>/tmp/ppp_state 2>/tmp/ppp_error &");

    return LTE_OK;
}

static LTE_RET_E huawei_disconnect(void *pvData, int iDataLen)
{
    system_call("/usr/sbin/ppp-off ppp0");
    system_call("/usr/sbin/ppp-off ppp1");
	return LTE_OK;
}

static LTE_RET_E huawei_access_net_get(void *pvData, int iDataLen)
{
	int iRet = 0;
    int iClientSocketFd = -1;
    char acCmd[] = "at^sysinfoex";
    char acCiev[] = "^SYSINFOEX:";
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
static LTE_RET_E huawei_access_net_get(void *pvData, int iDataLen)
{
	int iRet = 0; 
    FILE *pstFd = NULL;
    char acBuff[64];
    char acCiev[] = "^SYSINFOEX:";

    char acMode[16] = {0};
    int  iMode = 0;
    
	LTE_ACCESS_NETWORK_T *pstAccessNet = (LTE_ACCESS_NETWORK_T*)pvData;
	LTE_NORMAL_AT_T stAt = {"at^sysinfoex\r","/tmp/accesstype","OK",0,ATCOM,0,1};

    if (pvData == NULL)
		return LTE_FAIL;
	
	/*执行at指令*/
   	iRet = lte_normal_at_cmd(&stAt);
	if (iRet != 0)
	{
    	LTE_LOG(LTE_LOG_ERR,"%d,%s:at command failed\n",__LINE__,__func__);
    	lte_file_unlink(stAt.acFileName);
    	return LTE_FAIL;
	}
    pstFd = fopen(stAt.acFileName,"r");
    if (NULL == pstFd)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:open %s failed\n",__LINE__,__func__,stAt.acFileName);
        lte_file_unlink(stAt.acFileName);
        return -1;
    }

    while (NULL != fgets(acBuff,64,pstFd))  /*^SYSINFOEX:2,3,0,1,,3,"WCDMA",41,"WCDMA"*/
    { 	
        if (NULL != strstr(acBuff,acCiev))
        {
            sscanf(acBuff, "%*[^,],%*[^,],%*[^,],%*[^,],,%[^,]", acMode);
            break;
        }
        memset(acBuff,0,64);
    } 

    fclose(pstFd);
    pstFd = NULL;
    
	lte_file_unlink(stAt.acFileName);
    iMode = atoi(acMode);

    LTE_LOG(LTE_LOG_INFO, "%d,%s:access mode [%s]\n",__LINE__,__func__, acMode);
    if(3 == iMode)
    {
        pstAccessNet->enAccessNet = NET_3G;
    }
    else if(1 == iMode || 1 == iMode)
    {
        pstAccessNet->enAccessNet = NET_2G;
    }
    else
    {
        pstAccessNet->enAccessNet = NET_NONE;
    }
	
	return LTE_OK;
}
#endif

/*****************************************************************************
 函 数 名  : thinkwill_sup_nettype_get
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
static LTE_RET_E huawei_sup_nettype_get(void *pvData, int iDataLen)
{
    LTE_NET_TYPE_T *pstNets = (LTE_NET_TYPE_T *)pvData;
    
    pstNets->enNetType = LTE_3G_ONLY;
    pstNets->enNetTypePri = LTE_NONE;
    return LTE_OK;

}
static LTE_RET_E huawei_sms_sup_func(void *pvData, int iDataLen)
{
    int flag = 0;
    LTE_SUPPORT_SMS_T *pstSup = (LTE_SUPPORT_SMS_T *)pvData;
    pstSup->enSupSms = NO_SUP_SMS_FUNC;
    return LTE_OK;

}

static LTE_RET_E huawei_version_get(void *pvData, int iDataLen)
{
    int iRet = 0;
    char acBuff[64] = {0};
    char acVersion[64] = {0};
    FILE* pstFd = NULL;
    
    LTE_MODULE_VERSION_T *pstVersion = (LTE_MODULE_VERSION_T *)pvData;

    pstVersion->iVersionIndex = LTE_HUAWEI_MU709S_2;
    
    return LTE_OK;
}

static LTE_RET_E huawei_sim_unlock_support(void *pvData, int iDataLen)
{
    LTE_UNLOCK_SUPPORT_T *pstUnlockSupport = (LTE_UNLOCK_SUPPORT_T* )pvData;
    if(NULL == pstUnlockSupport || iDataLen < sizeof(LTE_UNLOCK_SUPPORT_T))
    {
        return LTE_FAIL;
    }
    pstUnlockSupport->iUnlockFlag = 0;
    return LTE_OK;
}


LTE_IOCTL_FUNC_T astHuaweiTable[] = 
{    
    {LTE_PIN_STATE,         NULL,           0,0,    NULL       }, 
    {LTE_PIN_SWITCH,        NULL,           0,0,    NULL       }, 
    {LTE_PIN_SW_GET,        NULL,           0,0,    NULL       },
    {LTE_PIN_UNLOCK,        NULL,           0,0,    NULL       }, 
    {LTE_PIN_ATTEMPTS,      NULL,           0,0,    NULL       }, 
    {LTE_PIN_ALTER,         NULL,           0,0,    NULL       }, 
    {LTE_SIGNAL_GET,        huawei_signal_get,        0,0,    "huawei_signal_get"       }, 
    {LTE_POWER_ON,          huawei_power_on,          0,0,    "huawei_power_on"       }, 
    {LTE_POWER_OFF,         huawei_power_off,         0,0,    "huawei_power_off"       }, 
    {LTE_NETTYPE_SET,      NULL,           0,0,    NULL       }, 
    {LTE_NETTYPE_GET,       NULL,           0,0,    NULL       }, 
    {LTE_PLMN_REGISTER,     NULL,           0,0,    NULL       }, 
    {LTE_PREFER_PLMN_SET,   NULL,           0,0,    NULL       }, 
    {LTE_NETWORK_PREPARE,   huawei_network_prepare,   0,0,    "huawei_network_prepare"       }, 
    {LTE_NETWORK_STATUS_GET,huawei_network_status_get,0,0,    "huawei_network_status_get"       },
    {LTE_CONNECT,           huawei_connect,           0,0,    "huawei_connect"       }, 
    {LTE_DISCONNECT,        huawei_disconnect,        0,0,    "huawei_disconnect"       }, 
    {LTE_LINKSTATE_GET,     huawei_linkstate_get,        0,0, "huawei_linkstate_get"       }, 
    {LTE_IMEI_GET,          huawei_imei_get,          0,0,    "huawei_imei_get"       }, 
    {LTE_CELLID_GET,        NULL,           0,0,    NULL       }, 
    {LTE_ICCID_GET,         NULL,           0,0,    NULL       },
    {LTE_IMSI_GET,          huawei_imsi_get,          0,0,    "huawei_imsi_get"       },
    {LTE_SIM_STATE_GET,     huawei_simstate_get,      0,0,    "huawei_simstate_get"       }, 
    {LTE_INFO_GET,        NULL,           0,0,    NULL       }, 
    {LTE_ACCESS_NET_GET,    huawei_access_net_get,           0,0,    "huawei_access_net_get"       }, 
    {LTE_BANDS_GET,        NULL,           0,0,    NULL       }, 
    {LTE_CLCK_BANDS_SET,        NULL,           0,0,    NULL       }, 
    {LTE_CLCK_BANDS_CLOSE,  NULL,            0,0,       NULL       },
    {LTE_CLCK_CELL_GET,     NULL,                        0,0,    NULL   },
    {LTE_CLCK_CELL_SET,        NULL,           0,0,    NULL       },
    {LTE_OPER_INFO_GET,        NULL,           0,0,    NULL          }, 
    {LTE_NETTYPE_INFO_GET,  huawei_sup_nettype_get,           0,0,    "huawei_sup_nettype_get"          },
    {LTE_GPS_START,         NULL,           0,0,    NULL      }, 
    {LTE_GPS_STOP,          NULL,           0,0,    NULL      }, 
    {LTE_GPS_DATE_GET,      NULL,           0,0,    NULL      }, 
    {LTE_GPS_POSITION_INFO_GET, NULL,           0,0,   NULL      }, 
    /* BEGIN: Added by wujian, 2014/11/13   PN:添加短信功能相关接口 */
    {LTE_SMS_CENT_CODE_SET, NULL, 0, 0, NULL },
    {LTE_SMS_MODE__SET, NULL, 0, 0, NULL   },   
    {LTE_SMS_FUNC_INIT, NULL, 0, 0, NULL   },
    {LTE_SMS_RECV, NULL, 0, 0, NULL   },
    {LTE_SMS_SEND, NULL, 0, 0, NULL   },
    {LTE_SMS_DELE, NULL, 0, 0, NULL   },
    {LTE_SMS_SUPP,          huawei_sms_sup_func, 0, 0,            "huawei_sms_sup_func"   },
    /* END:   Added by wujian, 2014/11/13   PN:添加短信功能相关接口 */
    {LTE_IPV6_PDP_SET, NULL, 0, 0, NULL   },
    /* BEGIN: Added by wujian, 2014/11/17   PN:添加设置apn接口 */
    {LTE_APN_SET, NULL, 0, 0, NULL   },
    /* END:   Added by wujian, 2014/11/17   PN:添加设置apn接口 */
    {LTE_VERSION_GET,       huawei_version_get,           0,0,    "huawei_version_get"       },
    {LTE_ENODEBID_GET,      NULL,     0,0,     NULL    }, 
    {LTE_TAC_GET,           NULL,     0,0,     NULL    },
    {LTE_LCID_GET,          NULL,     0,0,     NULL    },

    {LTE_SIM_UNLOCK_SUPPORT,huawei_sim_unlock_support, 0,0, "huawei_sim_unlock_support"},
    {LTE_CMD_MAX,           NULL,           0,0,    NULL       }, 
};

