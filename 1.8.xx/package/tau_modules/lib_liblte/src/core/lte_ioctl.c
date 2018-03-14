#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <termios.h>
#include <fcntl.h>

#include "lte.h"
#include "lte_log.h"
#include "lte_common.h"

static LTE_IOCTL_FUNC_T g_astLteIoctlFuncTable[LTE_CMD_MAX] = {0};
static int lte_connect_async_flag = 0;
static int lte_disconnect_async_flag = 0;
extern int g_moduleID;

LTE_RET_E lte_ioctl_register(LTE_IOCTL_FUNC_T *pstTable)
{
    LTE_IOCTL_FUNC_T *pstItem = pstTable;
    
    if (NULL == pstTable)
    {
        return LTE_FAIL;
    }
    
    
    while (1)
    {
        if (NULL == pstItem)
        {
            break;
        }
        if ( pstItem->iCmd <= LTE_CMD_MIN
            || pstItem->iCmd >= LTE_CMD_MAX)
        {
            break;
        }
        
        g_astLteIoctlFuncTable[pstItem->iCmd] = *pstItem;
        pstItem++;
    }
    
    return LTE_OK;
}
/*
#define USB_IF "usb0"
#define PPP_IF "ppp0"
#define DEFAULT_ATAIR_IP "169.254.0.10"
static int check_lte_ip(int module)
{
	FILE *fp = NULL;
	char if_name[16];
	char cmd[64];
	char line[128];

	if (module == M_THINKWILL){
		strcpy(if_name, PPP_IF);
	}
	else{
		strcpy(if_name, USB_IF);
	}

	sprintf(cmd, "ifconfig %s | grep \"inet addr:\"", if_name);

	fp = popen(cmd, "r");
	if (fp == NULL){
		LOG(LOG_ERR, "popen [%s] fail", cmd);
		return 0;
	}
	fgets(line, sizeof(line), fp);
	pclose(fp);
	
	if (strlen(line) <= 0){
		LOG(LOG_WARNING, "interface [%s] has no ipaddr ", if_name);
		return 0;
	}else{
		if (module == M_ALTAIR && NULL == strstr(line, DEFAULT_ATAIR_IP)){
			g_check_fail_statisic.ip_miss++;
			LOG(LOG_WARNING, "interface [%s] has no usefull ipaddr in Altair", if_name);
			return 0;
		}
	}

	return 1;
}
*/

LTE_RET_E lte_has_valid_ip(void)
{
	FILE *fp = NULL;
    char acInterface[8] = {0};
    char acGetIpCmd[128] = {0};
    char acGetData[128] = {0};
    char *pTmp = NULL;
    int iAltairIpVaild = 0;

	strcpy(acInterface, "ppp0");

	sprintf(acGetIpCmd, "/sbin/ifconfig %s | grep \"inet addr:\"", acInterface);
	fp = popen(acGetIpCmd, "r");
	if ( fp == NULL )
    {
		LTE_LOG(LTE_LOG_ERR, "popen [%s] fail", acGetIpCmd);
		return LTE_FAIL;
	}
	pTmp = fgets(acGetData, sizeof(acGetData), fp);
	pclose(fp);
    if ( (pTmp != NULL) && (strlen(pTmp) != 0))
    {
        return LTE_OK;
    }
    
    return LTE_FAIL;
}
LTE_RET_E lte_has_valid_process(char *process,char *pNeedle)
{
	FILE *fp = NULL;
    char acInterface[8] = {0};
    char acGetCmd[128] = {0};
    char acGetData[128] = {0};
    char *pTmp = NULL;
    int iAltairIpVaild = 0;

	sprintf(acGetCmd, "/bin/ps | grep %s", process);
	fp = popen(acGetCmd, "r");
	if ( fp == NULL )
    {
		LTE_LOG(LTE_LOG_ERR, "popen [%s] fail", acGetCmd);
		return LTE_FAIL;
	}
	pTmp = fgets(acGetData, sizeof(acGetData), fp);
	pclose(fp);
    if ((pTmp != NULL) && (strstr(pTmp,pNeedle) != NULL))
    {
        return LTE_OK;
    }
    
    return LTE_FAIL;
}


static int check_process(char *name)
{
	FILE *fp = NULL;
	char cmd[64];
	char line[128];

	sprintf(cmd, "pidof %s", name);

	fp = popen(cmd, "r");
	if (fp == NULL){
		LTE_LOG(LTE_LOG_ERR, "popen [%s] fail", cmd);
		return 0;
	}
	fgets(line, sizeof(line), fp);
	pclose(fp);
	
	if (strlen(line) <= 0){
		return 0;
	}
	
	return atoi(line);
}

LTE_RET_E system_call(char *pcCmd)
{
	int status, i;

	for (i = 0 ; i < 2; i++){
		status = system(pcCmd);
		if (status < 0){
			LTE_LOG(LTE_LOG_ERR, "system call fail [CMD : %s]", pcCmd);
			continue;
		}

		if (WIFEXITED(status)){
			if (0 == WEXITSTATUS(status)){
				LTE_LOG(LTE_LOG_INFO, "system call \"%s\" success", pcCmd);
				return LTE_OK;
			}
			else{
				LTE_LOG(LTE_LOG_ERR, "system call fail [exit code: %d] [CMD: %s]", WEXITSTATUS(status), pcCmd);
			}
		}else{
			LTE_LOG(LTE_LOG_ERR, "system call fail [exit code: %d] [CMD: %s]", WEXITSTATUS(status), pcCmd);
		}
	}
	return LTE_FAIL;
}
#if 0
/* BEGIN: Added by liyoukun, 2014/9/22   PN:添加gps测试接口 */
static int gps_get_com_baud(char *acAtCom)
{
    int     iModule = 0;
    int     iBaudRate = 0;
    char    acAtComTmp[16] = {0};
    LTE_DEVICE_INFO stDev;
    
    if (LTE_FAIL == lte_device_info_get(&stDev))
    {
        LTE_LOG(LTE_LOG_ERR, "lte gps get device info failed!");
        return LTE_FAIL;
    }
    iModule = stDev.enDeviceNum;
    if(NULL == acAtCom)
    {
        syslog(5, "the pram is NULL");
        return -1;
    }
    switch(iModule)
    {
        case M_ZTE_CDMA2000:
            iBaudRate = 3;
            strcpy(acAtComTmp, AT_COM1);
            break;
        case M_THINKWILL_CDMA_MI660:
            iBaudRate = 3;
            strcpy(acAtComTmp, AT_COM2);
            break;  
        case M_THINKWILL:
            iBaudRate = 3;
            strcpy(acAtComTmp, AT_COM1);
            break;
        case M_YUGA_CDMA_CEM600:
            iBaudRate = 3;
			/* 域格GPS口变动，由ttyUSB3改为ttyUSB2 */
			//strcpy(acAtComTmp, AT_COM3);
            strcpy(acAtComTmp, AT_COM2);
            break;    
        default:
            iBaudRate = 3;
            strcpy(acAtComTmp, AT_COM3);
            break;
    }
    
    strcpy(acAtCom, acAtComTmp);
    return iBaudRate;
}

int lte_gps_com_set(int iComFd, int iDeviceSpeedFlag) 
{
    struct termios TtyAttr;
    int DeviceSpeed = B115200;
    int ByteBits = CS8;
    switch(iDeviceSpeedFlag)
    {
        case 1:
            DeviceSpeed = B4800;
            break;
        case 2:
            DeviceSpeed = B9600;
            break;
        case 3:
            DeviceSpeed = B115200;
            break;
        default:
            break;
    }
    memset(&TtyAttr, 0, sizeof(struct termios));
    TtyAttr.c_iflag = IGNPAR;
    TtyAttr.c_cflag = DeviceSpeed | HUPCL | ByteBits | CREAD | CLOCAL;
    TtyAttr.c_cc[VMIN] = 1;

    if (tcsetattr(iComFd, TCSANOW, &TtyAttr) < 0)
    {
        printf("set attr failed\n");
        return LTE_FAIL;
    }
    return LTE_OK;
}
int open_serial_com(char *pcCom, int iDeviceSpeedFlag)
{
    int iSetAcm = 0;	
    int iComFd = 0;
    
    /* 打开串口需要添加属性O_NOCTTY，否则任何输入都会影响进程 */
    //if ((iComFd = open(pcCom, O_RDONLY)) < 0) 
    if ((iComFd = open(pcCom, O_RDWR | O_NOCTTY)) < 0) 
    {
        perror("open failed\n");
        return -1;
    }
    //printf("open the file:%s\n",pcCom);
    /* 将com口设置为非阻塞模式 */
    if (fcntl(iComFd, F_SETFL, O_NONBLOCK) < 0)
    {
        perror("fcntl failed\n"); 
        return -1;
    }

    /*设置COM波特率等其他属性*/
    if (LTE_FAIL == (iSetAcm= lte_gps_com_set(iComFd,iDeviceSpeedFlag)))
    {
        return LTE_FAIL;
    }
     
    return iComFd;
}
/* END:   Added by liyoukun, 2014/9/22   PN:添加gps测试接口 */
#endif
LTE_RET_E lte_ioctl(int iCmd, void *pvData, int iDataLen)
{
    LTE_IOCTL_FUNC_T *pstItem;
    LTE_RET_E enRet;
    
    if (!lte_is_init())
    {
    	  LTE_LOG(LTE_LOG_ERR, "liblte is not init ");
        return LTE_FAIL;
    }
    
    if (iCmd <= LTE_CMD_MIN || iCmd >= LTE_CMD_MAX)
    {
    	LTE_LOG(LTE_LOG_ERR, "CMD ID is not right, must between %d--%d", LTE_CMD_MIN, LTE_CMD_MAX);
        return LTE_FAIL;
    }
    /* 2012-9-4 add start by zhangping */
    /* 加锁 */
    if(LTE_OK != lte_mutex_lock())
    {
    	LTE_LOG(LTE_LOG_ERR, "lte_mutex_lock fail");
        return LTE_FAIL;
    }
    /* 2012-9-4 add end by zhangping */
    pstItem = &g_astLteIoctlFuncTable[iCmd];
    
    if (NULL != pstItem->func)
    {
        enRet = pstItem->func(pvData, iDataLen);
        
        pstItem->uiCount++;
        if (LTE_OK != enRet)
        {
            pstItem->uiCountErr++;
        }

	}
    else
    {
        LTE_LOG(LTE_LOG_ERR, "There is no handler for sync cmd %d", iCmd);
        enRet = LTE_FAIL;
    }
    //sleep(1);
    /* 解锁 */
    if(LTE_OK != lte_mutex_unlock())
    {
        LTE_LOG(LTE_LOG_ERR, "lte_mutex_unlock fail");
        return LTE_FAIL;
    }
    return enRet;
}



LTE_RET_E lte_pin_state_get(LTE_PIN_STATE_T *pstState)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_PIN_STATE, (void *)pstState, sizeof(LTE_PIN_STATE_T));
}

LTE_RET_E lte_pin_switch(LTE_PIN_SWITCH_T *pstSwitch)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_PIN_SWITCH, (void *)pstSwitch, sizeof(LTE_PIN_SWITCH_T));
}
LTE_RET_E lte_pin_sw_get(LTE_PIN_SW_STATE_T *pstPinSwState)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_PIN_SW_GET, (void *)pstPinSwState, sizeof(LTE_PIN_SW_STATE_T));
}

LTE_RET_E lte_pin_unlock(LTE_PIN_T *pstPin)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_PIN_UNLOCK, (void *)pstPin, sizeof(LTE_PIN_T));
}

LTE_RET_E lte_pin_attempts(LTE_PIN_ATTEMPTS_T *pstAttempts)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_PIN_ATTEMPTS, (void *)pstAttempts, sizeof(LTE_PIN_ATTEMPTS_T));
}

LTE_RET_E lte_pin_alter(LTE_PIN_ALTER_T *pstAlter)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_PIN_ALTER, (void *)pstAlter, sizeof(LTE_PIN_ALTER_T));
}

LTE_RET_E lte_signal_get(LTE_SIGNAL_T *pstSignal)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_SIGNAL_GET, (void *)pstSignal, sizeof(LTE_SIGNAL_T));
}

LTE_RET_E lte_power_on(void)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_POWER_ON, NULL, 0);
}

LTE_RET_E lte_power_off(void)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_POWER_OFF, NULL, 0);
}

LTE_RET_E lte_net_type_set(LTE_NET_TYPE_T *pstType)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_NETTYPE_SET, (void *)pstType, sizeof(LTE_NET_TYPE_T));
}

LTE_RET_E lte_net_type_get(LTE_NET_TYPE_T *pstType)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_NETTYPE_GET, (void *)pstType, sizeof(LTE_NET_TYPE_T));
}

LTE_RET_E lte_plmn_register(LTE_PLMN_T *pstPlmn)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_PLMN_REGISTER, (void *)pstPlmn, sizeof(LTE_PLMN_T));
}

LTE_RET_E lte_prefer_plmn_set(void)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_PREFER_PLMN_SET, NULL, 0);
}
LTE_RET_E lte_network_prepare_get(LTE_NETWORK_PREPARE_T *pstNWStatus)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    
    return lte_ioctl(LTE_NETWORK_PREPARE, (void *)pstNWStatus, 0);
}
LTE_RET_E lte_network_status_get(LTE_NETWORK_PREPARE_T *pstNWStatus)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    
    return lte_ioctl(LTE_NETWORK_STATUS_GET, (void *)pstNWStatus, sizeof(LTE_NETWORK_PREPARE_T));
}


/*add by wangqi 2015-5-25 begin*/
LTE_RET_E sim_mode_select_connect(char *pcFileName)
{
    LTE_SIM_TYPE_E enSimType = 0;
    LTE_SIM_SECRET_KEY_T stSecretKey = {0};
    LTE_SIM_MODE_SELECT_E enSimModeSelect = 0;
    LTE_SIM_UNLOCK_STATE_T stSimUnlockState = {0};
    FILE *pFile = NULL;
    char acBuf[512] = {0};
    char *pTmp = NULL;
    if(NULL == pcFileName)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:input param is null pointer!\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    if (access(pcFileName, F_OK) != 0)
    {
        return LTE_OK;
    }
    if (NULL == (pFile = fopen(pcFileName, "r")))
    {
        return LTE_FAIL;
    }
    while (fgets(acBuf, sizeof(acBuf), pFile) != NULL )
    {
        if(NULL != (pTmp = strstr(acBuf, LTE_SIM_MODE_SELECT)))
        {
            enSimModeSelect = atoi(pTmp+strlen(LTE_SIM_MODE_SELECT));
        }
        else if(NULL != (pTmp = strstr(acBuf, LTE_SIM_MAN_ID)))
        {
            stSecretKey.man_id = atoi(pTmp+strlen(LTE_SIM_MAN_ID));
        }
        else if(NULL != (pTmp = strstr(acBuf, LTE_SIM_KEY_VER)))
        {
            stSecretKey.key_ver = atoi(pTmp+strlen(LTE_SIM_KEY_VER));
        }
        else if(NULL != (pTmp = strstr(acBuf, LTE_SIM_SECRET_KEY)))
        {
            strncpy(stSecretKey.acSecretKey, pTmp+strlen(LTE_SIM_SECRET_KEY), sizeof(acBuf)-strlen(LTE_SIM_SECRET_KEY));
        }
        else
        {
            return LTE_FAIL;
        }
        pTmp = NULL;
        memset(acBuf,0,sizeof(acBuf));
    }
    fclose(pFile);
    
    if(LTE_FAIL == lte_ioctl(LTE_SIM_TYPE_GET, (void *)&enSimType, sizeof(LTE_SIM_TYPE_E)))
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:get sim type fail!!",__LINE__,__func__);
        return LTE_FAIL;
    }
    if(ENCRYPTION_SIM == enSimType)
    {
        if(NORMAL_SIM_ONLY == enSimModeSelect)
        {
            LTE_LOG(LTE_LOG_ERR,"%d,%s:Current mode is NORMAL_SIM_ONLY,please replace SIM.",__LINE__,__func__);
            return LTE_FAIL;
        }
        else
        {
            if(LTE_FAIL == lte_ioctl(LTE_SIM_UNLOCK_GET, (void *)&stSimUnlockState, sizeof(LTE_SIM_UNLOCK_STATE_T)))
            {
                LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_ioctl get LTE_SIM_UNLOCK_GET fail.",__LINE__,__func__);
                return LTE_FAIL;
            }
            if(SIM_HAS_UNLOCK == stSimUnlockState.iIsUnlock)
            {
                return LTE_OK;
            }
            if(strlen(stSecretKey.acSecretKey) < 160)//秘钥错误
            {
                return LTE_FAIL;
            }
            if(LTE_FAIL == lte_ioctl(LTE_SIM_UNLOCK, (void *)&stSecretKey, sizeof(LTE_SIM_SECRET_KEY_T)))
            {
                LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_ioctl LTE_SIM_UNLOCK error!!",__LINE__,__func__);
                return LTE_FAIL;
            }
        }
    }
    else//普通卡
    {
        if(ENCRYPTION_SIM_ONLY == enSimModeSelect)
        {
            LTE_LOG(LTE_LOG_ERR,"%d,%s:Current mode is ENCRYPTION_SIM_ONLY,please replace SIM.",__LINE__,__func__);
            return LTE_FAIL;
        }
    }
    return LTE_OK;
}
/*add by wangqi 2015-5-25 end*/


/*altered by caibing 2014-09-22*/
LTE_RET_E lte_connect(LTE_APN_T *pstApn, int check_flag)
{
	int i;
	LTE_UNLOCK_SUPPORT_T stUnlockSupport = {0};
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
	#if 1
    /*altered by wangqi 2015-5-25 begin*/
    if(LTE_OK == lte_ioctl(LTE_SIM_UNLOCK_SUPPORT, (void *)&stUnlockSupport, sizeof(LTE_UNLOCK_SUPPORT_T)))
    {
        if(1 == stUnlockSupport.iUnlockFlag)//如果支持加密卡功能，才有选定SIM卡
        {
            if(LTE_OK != sim_mode_select_connect(LTE_SIM_SECRET_KEY_FILE))
            {
                LTE_LOG(LTE_LOG_ERR,"%d,%s:sim_mode_select_connect fail.",__LINE__,__func__);
                return LTE_FAIL;
            }
        }
    }
    
    /*altered by wangqi 2015-5-25 end*/
    /*全频段收索仅需要2min,加上系统操作2条指令,4s*/
    /*因此DEFAULT_SEARCH_NET_TIMES 25 即可*/
    for (i = 0; i < DEFAULT_SEARCH_NET_TIMES; i++)
    {
        if (LTE_OK == lte_ioctl(LTE_NETWORK_PREPARE, (void *)pstApn, sizeof(LTE_APN_T)))
        {
    	  	LTE_LOG(LTE_LOG_INFO, "search network success");
        	break;
        }
        usleep(200*1000);
    }
    if (i >= DEFAULT_SEARCH_NET_TIMES)
    {
    	lte_power_off();
	//	sleep(2);
		lte_power_on();
        LTE_LOG(LTE_LOG_ERR, "search network fail after %d times, poweroff,poweron", DEFAULT_SEARCH_NET_TIMES);
        return LTE_FAIL;
    }
	#endif
    LTE_LOG(LTE_LOG_ERR, "lte_ioctl connect");
    if (LTE_OK != lte_ioctl(LTE_CONNECT, (void *)pstApn, sizeof(LTE_APN_T)))
    {
        LTE_LOG(LTE_LOG_ERR, "lte_ioctl connect fail");
        return LTE_FAIL;
    }
    return LTE_OK;
}
#if 0
LTE_RET_E lte_connect(LTE_APN_T *pstApn, int check_flag)
{
	int i;
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    /*全频段收索仅需要2min,加上系统操作2条指令,4s*/
    /*因此DEFAULT_SEARCH_NET_TIMES 25 即可*/
    for (i = 0; i < DEFAULT_SEARCH_NET_TIMES; i++)
        {
         if (LTE_OK == lte_ioctl(LTE_NETWORK_PREPARE, (void *)pstApn, sizeof(LTE_APN_T))){
	  	LTE_LOG(LTE_LOG_INFO, "search network success");
        	break;
    	  }
	  sleep(1);
    }
    if (i >= DEFAULT_SEARCH_NET_TIMES){
        LTE_LOG(LTE_LOG_ERR, "search network fail after %d times", DEFAULT_SEARCH_NET_TIMES);
	  return LTE_FAIL;
    }
    if (LTE_OK != lte_ioctl(LTE_CONNECT, (void *)pstApn, sizeof(LTE_APN_T))){
	  LTE_LOG(LTE_LOG_ERR, "lte_ioctl connect fail");
        return LTE_FAIL;
    }else{
    	 if (check_flag && !check_process("checkalive"))
       	system_call("checkalive &");
	if (check_process("ltecmd"))
		system_call("killall -9 ltecmd");
    }
    return LTE_OK;
}
#endif
/*altered by caibing 2014-09-22*/
LTE_RET_E lte_connect_async(LTE_APN_T *pstApn, int check_flag)
{
	if (lte_connect_async_flag)
		return LTE_WAIT;
	
	pid_t pid, cpid;

	if (LTE_OK != lte_init(g_iLteLogLevel))
	{
    	return LTE_FAIL;
	}
	pid = fork();
	if(pid == 0)
	{
		cpid = fork();
		if (cpid == 0)
        {
			lte_connect_async_flag = 1;
			int i;
			for (i = 0; i < DEFAULT_SEARCH_NET_TIMES; i++)
            {
     			if (LTE_OK == lte_ioctl(LTE_NETWORK_PREPARE, (void *)pstApn, sizeof(LTE_APN_T)))
                {
      				LTE_LOG(LTE_LOG_INFO, "search network success");
    				break;
	  			}
	  			sleep(1);
			}
            
			if (LTE_OK != lte_ioctl(LTE_CONNECT, (void *)pstApn, sizeof(LTE_APN_T)))
            {
	  			LTE_LOG(LTE_LOG_ERR, "lte_ioctl connect fail");
			}
			lte_connect_async_flag = 0;
			exit(0);
		}
		exit(0);
	}
	else if(pid > 0) 
	{
    	return LTE_WAIT;
	}
	else
	{
    	LTE_LOG(LTE_LOG_ERR, "fork() fail");
    	return LTE_FAIL;
	}
}

LTE_RET_E lte_disconnect(void)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_DISCONNECT, NULL, 0);
    /* kill  checkalive 由om来做 */
}
LTE_RET_E lte_disconnect_ipv6(char *acIpFamily)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_DISCONNECT, (void *)acIpFamily, strlen(acIpFamily)+1);
    /* kill  checkalive 由om来做 */
}

LTE_RET_E lte_disconnect_async(void)
{
	if (lte_disconnect_async_flag)
		return LTE_WAIT;
	
	pid_t pid, cpid;

	if (LTE_OK != lte_init(g_iLteLogLevel))
	{
    	return LTE_FAIL;
	}
	
	pid = fork();
	if(pid == 0)
	{
		cpid = fork();
		if (cpid == 0)
        {
			lte_disconnect_async_flag = 1;
			if( LTE_OK != lte_ioctl(LTE_DISCONNECT, NULL, 0))
			{
				LTE_LOG(LTE_LOG_ERR, "lte_ioctl disconnect fail");
			}
			/* kill  checkalive 由om来做 */
			lte_disconnect_async_flag = 0;
			exit(0);
		}
		exit(0);
	}
	else if(pid > 0) 
	{
    	return LTE_WAIT;
   	}
	else
	{
    	LTE_LOG(LTE_LOG_ERR, "fork() fail");
    	return LTE_FAIL;
	}
}
LTE_RET_E lte_disconnect_ipv6_async(char *acIpFamily)
{
	if (lte_disconnect_async_flag)
		return LTE_WAIT;
	
	pid_t pid, cpid;

	if (LTE_OK != lte_init(g_iLteLogLevel))
	{
    	return LTE_FAIL;
	}
	
	pid = fork();
	if(pid == 0)
	{
		cpid = fork();
		if (cpid == 0)
        {
			lte_disconnect_async_flag = 1;
			if( LTE_OK != lte_ioctl(LTE_DISCONNECT, (void *)acIpFamily, strlen(acIpFamily)+1))
			{
				LTE_LOG(LTE_LOG_ERR, "lte_ioctl disconnect fail");
			}
			/* kill  checkalive 由om来做 */
			lte_disconnect_async_flag = 0;
			exit(0);
		}
		exit(0);
	}
	else if(pid > 0) 
	{
    	return LTE_WAIT;
   	}
	else
	{
    	LTE_LOG(LTE_LOG_ERR, "fork() fail");
    	return LTE_FAIL;
	}
}

LTE_RET_E lte_linkstate_get(LTE_LINKSTATE_T *pstState)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_LINKSTATE_GET, (void *)pstState, sizeof(LTE_LINKSTATE_T));
}

LTE_RET_E lte_imei_get(LTE_IMEI_T *pstImei)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_IMEI_GET, (void *)pstImei, sizeof(LTE_IMEI_T));
}

LTE_RET_E lte_cellid_get(LTE_CELLID_T *pstCellId)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_CELLID_GET, (void *)pstCellId, sizeof(LTE_CELLID_T));
}

LTE_RET_E lte_iccid_get(LTE_ICCID_T *pstICCID)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_ICCID_GET, (void *)pstICCID, sizeof(LTE_ICCID_T));
}
LTE_RET_E lte_imsi_get(LTE_IMSI_T *pstImsi)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_IMSI_GET, (void *)pstImsi, sizeof(LTE_IMSI_T));
}

LTE_RET_E lte_sim_state_get(LTE_SIM_STATE_T *pstSimState)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_SIM_STATE_GET, (void *)pstSimState, sizeof(LTE_SIM_STATE_T));
}

LTE_RET_E lte_info_get(LTE_INFO_T *pstLteInfo)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_INFO_GET, (void *)pstLteInfo, sizeof(LTE_INFO_T));
}

LTE_RET_E lte_access_net_get(LTE_ACCESS_NETWORK_T *pstAccessNet)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_ACCESS_NET_GET, (void *)pstAccessNet, sizeof(LTE_ACCESS_NETWORK_T));
}

LTE_RET_E lte_bands_get(LTE_BANDS_T *pstBands)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_BANDS_GET, (void *)pstBands, sizeof(LTE_BANDS_T));
}


LTE_RET_E lte_clck_bands_set(LTE_BANDS_T *pstBands)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_CLCK_BANDS_SET, (void *)pstBands, sizeof(LTE_BANDS_T));
}

LTE_RET_E lte_clck_bands_close(void)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_CLCK_BANDS_CLOSE, NULL, 0);
}

LTE_RET_E lte_clck_cell_enable_get(LTE_CLKCELL_ENABLE_T *pstCellEnableFlag)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_CLCK_CELL_GET, (void *)pstCellEnableFlag, sizeof(LTE_CLKCELL_ENABLE_T));
}
LTE_RET_E lte_clck_cell_set(LTE_CLK_CELL_T *pstCell)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_CLCK_CELL_SET, (void *)pstCell, sizeof(LTE_CLK_CELL_T));
}

LTE_RET_E lte_gps_func_start(void)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_GPS_START, NULL, 0);
}
LTE_RET_E lte_gps_func_stop(void)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_GPS_STOP, NULL, 0);
}

LTE_RET_E lte_gps_date_get(LTE_GPS_DATE_T *pstGpsDate)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_GPS_DATE_GET, (void *)pstGpsDate, sizeof(LTE_GPS_DATE_T));
}

LTE_RET_E lte_gps_postion_get(LTE_GPS_POSTION_T *pstGpsPosInfo)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_GPS_POSITION_INFO_GET, (void *)pstGpsPosInfo, sizeof(LTE_GPS_POSTION_T));
}
LTE_RET_E lte_oper_info_get(LTE_OPER_INFO_T *pstOperInfo)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_OPER_INFO_GET, (void *)pstOperInfo, sizeof(LTE_OPER_INFO_T));
}

LTE_RET_E lte_sup_nettype_get(LTE_NET_TYPE_T *pstNetsInfo)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_NETTYPE_INFO_GET, (void *)pstNetsInfo, sizeof(LTE_NET_TYPE_T));
}
LTE_RET_E lte_sms_cent_code_set(LTE_SMS_CENT_CODE_T *pstCentCode)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_SMS_CENT_CODE_SET, (void *)pstCentCode, sizeof(LTE_SMS_CENT_CODE_T));
}
LTE_RET_E lte_sms_mode__set(LTE_SMS_Mode_T *pstSmsMode)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_SMS_MODE__SET, (void *)pstSmsMode, sizeof(LTE_SMS_Mode_T));
}
LTE_RET_E lte_sms_func_init(void)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_SMS_FUNC_INIT, NULL, 0);
}
LTE_RET_E lte_sms_recv(LTE_SMS_RECV_T *pstRecv)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_SMS_RECV, (void *)pstRecv, sizeof(LTE_SMS_RECV_T));
}
LTE_RET_E lte_sms_send(LTE_SMS_SEND_T *pstSend)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_SMS_SEND, (void *)pstSend, sizeof(LTE_SMS_SEND_T));
}
LTE_RET_E lte_sms_dele(LTE_SMS_DEL_T *pstDele)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_SMS_DELE, (void *)pstDele, sizeof(LTE_SMS_DEL_T));
}
LTE_RET_E lte_sms_sup_func(LTE_SUPPORT_SMS_T *pstSupSms)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_SMS_SUPP, (void *)pstSupSms, sizeof(LTE_SUPPORT_SMS_T));
}

LTE_RET_E lte_pdp_cgdcont_set(LTE_CGDCONT_T *pstPdpSet)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_IPV6_PDP_SET, (void *)pstPdpSet, sizeof(LTE_APN_T));
}
LTE_RET_E lte_apn_set(LTE_APN_T *pstApn)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_APN_SET, (void *)pstApn, sizeof(LTE_APN_T));
}
LTE_RET_E lte_version_get(LTE_MODULE_TYPE_T *pstModuleType)
{
    LTE_MODULE_VERSION_T stVersion = {0};
    
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    if(LTE_FAIL != lte_ioctl(LTE_VERSION_GET, (void *)(&stVersion), sizeof(LTE_MODULE_VERSION_T)))
    {
        if(stVersion.iVersionIndex >= LTE_VER_IND_MAX)
        {
            return LTE_FAIL;
        }
        else
        {
            sprintf(pstModuleType->acModuleTypeInd, "Lte-moudle-%d", stVersion.iVersionIndex);
            strcpy(pstModuleType->acVersion, stVersion.acVersion);
        }
    }
    else
    {
        return LTE_FAIL;
    }
    return LTE_OK;
}

LTE_RET_E lte_enodeb_id_get(LTE_ENODB_ID_T *pstEnodeId)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_ENODEBID_GET, (void *)pstEnodeId, sizeof(LTE_ENODB_ID_T));
}

LTE_RET_E lte_tac_get(LTE_TAC_T *pstTac)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_TAC_GET, (void *)pstTac, sizeof(LTE_TAC_T));
}

LTE_RET_E lte_lcid_get(LTE_LCID_T *pstLcid)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_LCID_GET, (void *)pstLcid, sizeof(LTE_LCID_T));
}

LTE_RET_E lte_all_cellid_get(LTE_ALL_CELLID_T *pstAllCid)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_ALL_CELLID_GET, (void *)pstAllCid, sizeof(LTE_ALL_CELLID_T));
}

LTE_RET_E lte_moudle_mac_set(LTE_MOUDLE_MAC_T *pstMac)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_MOUDLE_MAC_SET, (void *)pstMac, sizeof(LTE_MOUDLE_MAC_T));
}

LTE_RET_E lte_moudle_mac_get(LTE_MOUDLE_MAC_T *pstMac)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_MOUDLE_MAC_GET, (void *)pstMac, sizeof(LTE_MOUDLE_MAC_T));
}
LTE_RET_E lte_moudle_sim_unlock(LTE_SIM_SECRET_KEY_T *pstSecretKey)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_SIM_UNLOCK, (void *)pstSecretKey, sizeof(LTE_SIM_SECRET_KEY_T));
}
LTE_RET_E lte_moudle_sim_disable_unlock(void)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_SIM_DISABLE_UNLOCK, NULL, 0);
}

LTE_RET_E lte_moudle_sim_unlock_support(LTE_UNLOCK_SUPPORT_T *pstUnlockSupport)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_SIM_UNLOCK_SUPPORT, (void *)pstUnlockSupport, sizeof(LTE_UNLOCK_SUPPORT_T));
}

LTE_RET_E lte_moudle_sim_type_get(LTE_SIM_TYPE_E *penSimType)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_SIM_TYPE_GET, (void *)penSimType, sizeof(LTE_SIM_TYPE_E));
}

LTE_RET_E lte_moudle_sim_unlock_get(LTE_SIM_UNLOCK_STATE_T *pstSimUnlockState)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_SIM_UNLOCK_GET, (void *)pstSimUnlockState, sizeof(LTE_SIM_UNLOCK_STATE_T));
}

LTE_RET_E lte_signal_value_get(LTE_SIGNAL_VALUE_T *pstSignalValue)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_SIGNAL_VALUE_GET, (void *)pstSignalValue, sizeof(LTE_SIGNAL_VALUE_T));
}

LTE_RET_E lte_ip_gateway_dns_get(LTE_IP_GATEWAY_DNS_T *pstIpGwDns)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_IP_GATEWAY_DNS_GET, (void *)pstIpGwDns, sizeof(LTE_IP_GATEWAY_DNS_T));
}

LTE_RET_E lte_all_plmn_mode_get(LTE_ALL_PLMN_MODE_T *pstAllPlmnMode)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_ALL_PLMN_MODE_GET, (void *)pstAllPlmnMode, sizeof(LTE_ALL_PLMN_MODE_T));
}

LTE_RET_E lte_sounet_mode_set(LTE_SOUNET_MODE_T *pstSounetMode)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_SOUNET_MODE_SET, (void *)pstSounetMode, sizeof(LTE_SOUNET_MODE_T));
}

LTE_RET_E lte_sim_puk_unlock(LTE_SIM_UNLOCK_PUK_T *pstPukUnlock)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_SIM_UNLOCK_PUK, (void *)pstPukUnlock, sizeof(LTE_SIM_UNLOCK_PUK_T));
}

LTE_RET_E lte_moudle_up_support(LTE_MODULE_UP_SUPPORT_T *pstModuleUpSupp)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_MODULE_UP_SUPPORT, (void *)pstModuleUpSupp, sizeof(LTE_MODULE_UP_SUPPORT_T));
}


LTE_RET_E lte_pin_input_remain_get(LTE_PIN_INPUT_REMAIN_T *pstPinIputRemain)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_PIN_INPUT_REMAIN_GET, (void *)pstPinIputRemain, sizeof(LTE_PIN_INPUT_REMAIN_T));
}

LTE_RET_E lte_post_route_set(LTE_POST_ROUTE_SW_T *pstPostRouteSw)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_POST_ROUTE_SET, (void *)pstPostRouteSw, sizeof(LTE_POST_ROUTE_SW_T));
}

LTE_RET_E lte_nv_datalock(LTE_NV_DATALOCK_T *pstNvDatalock)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_NV_DATALOCK, (void *)pstNvDatalock, sizeof(LTE_NV_DATALOCK_T));
}

LTE_RET_E lte_post_route_get(LTE_POST_ROUTE_SW_T *pstPostRouteSw)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_POST_ROUTE_GET, (void *)pstPostRouteSw, sizeof(LTE_POST_ROUTE_SW_T));
}
LTE_RET_E lte_remove_attach_set(void)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_REMOVE_ATTACH, (void *)NULL, 0);
}



LTE_RET_E lte_qos_set(LTE_QOS_DATA_S *pstQosData)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_QOS_DATA_SET, (void *)pstQosData, sizeof(LTE_QOS_DATA_S));
}


LTE_RET_E lte_qos_get(LTE_QOS_DATA_S*pstQosData)
{
	if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
	return lte_ioctl(LTE_QOS_DATA_GET, (void *)pstQosData, sizeof(LTE_QOS_DATA_S));
		
}

LTE_RET_E lte_dload_prot_ver_get(LTE_DLOAD_PROT_VER_T *pstDloadProtVer)
{
    if(LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_DLOAD_PROT_VER_GET,(void*)pstDloadProtVer,sizeof(LTE_DLOAD_PROT_VER_T));
}
LTE_RET_E lte_dload_ver_info_get(LTE_DLOAD_VER_INFO_T *pstDloadVerInfo)
{
    if(LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_DLOAD_VER_INFO_GET,(void*)pstDloadVerInfo,sizeof(LTE_DLOAD_VER_INFO_T));
}
LTE_RET_E lte_nv_backup(LTE_NV_BACKUP_T *pstNvBackup)
{
    if(LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_NV_BACKUP,(void*)pstNvBackup,sizeof(LTE_NV_BACKUP_T));
}
LTE_RET_E lte_nv_restore(LTE_NV_RESTORE_T *pstNvRestore)
{
    if(LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_NV_RESTORE,(void*)pstNvRestore,sizeof(LTE_NV_RESTORE_T));
}
LTE_RET_E lte_dload_mode(void)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_DLOAD_MODE, (void *)NULL, 0);
}
LTE_RET_E lte_data_mode(void)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_DATA_MODE, (void *)NULL, 0);
}
LTE_RET_E lte_reset(void)
{
    if (LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_RESET, (void *)NULL, 0);
}

#if 0
LTE_RET_E lte_gps_test(void)
{
    int iRet = 0;
    int iBaudRate = 0;
    int iReadBytes = 0;
    int iComFd = 0;
    int iCount = 0;
    char acAtCom[16] = {0};
    unsigned char aucComBuf[512] = {0};
    iRet = lte_gps_func_start();
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_gps_func_start failed!");
        return LTE_FAIL;
    }
    /* 获取com口，以及波特率 */
    while(-1 == (iBaudRate = gps_get_com_baud(acAtCom)))
    {
        sleep(10);
        if(5 == iCount)
        {
            LTE_LOG(LTE_LOG_ERR, "gps_get_com_baud fail!");
            return LTE_FAIL;
        }
        iCount++;
    }
    /* 打开com口 */
    iComFd  = open_serial_com(acAtCom,iBaudRate); //波特率根据iBaudRate适应
    if ( iComFd < 0 )
    {
        LTE_LOG(LTE_LOG_ERR, "open_serial_com fail!");
        close(iComFd);
        return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO, "open com: %s %d %d\n", acAtCom,iBaudRate, iComFd);
    iCount = 0;
    while(1)
    {
        /*读取gps信息*/
        iReadBytes = read(iComFd,aucComBuf,512); 
        if ( iReadBytes < 0 )
        {
            printf("read gps info failed\n");
            sleep(3);
            if(10 == iCount)
            {
                LTE_LOG(LTE_LOG_ERR, "gps info get fail!");
                close(iComFd);
                return LTE_FAIL;
            }
            iCount++;
            continue;
        }
        else
        {
            LTE_LOG(LTE_LOG_INFO, "gps info: %s\n", aucComBuf);
            break;
        }
    }
    close(iComFd);
    iRet = lte_gps_func_stop();
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_gps_func_stop failed!");
        return LTE_FAIL;
    }
    return LTE_OK;
}
#endif

LTE_RET_E lte_sim_switch_get(LTE_SIM_SWITCH_T *pstSimSwitch)
{
    if(LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_SIM_SWITCH_GET,(void*)pstSimSwitch,sizeof(LTE_SIM_SWITCH_T));
}
LTE_RET_E lte_sim_switch_set(LTE_SIM_SWITCH_T *pstSimSwitch)
{
    if(LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    return lte_ioctl(LTE_SIM_SWITCH_SET,(void*)pstSimSwitch,sizeof(LTE_SIM_SWITCH_T));
}
LTE_RET_E lte_sim_switch_soft_set(LTE_SIM_SWITCH_T *pstSimSwitch)
{
    if(LTE_OK != lte_init(g_iLteLogLevel))
    {
        return LTE_FAIL;
    }
    
    return lte_ioctl(LTE_SIM_SWITCH_SOFT_SET,(void*)pstSimSwitch,sizeof(LTE_SIM_SWITCH_T));
}