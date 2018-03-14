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

#include "lte_com.h"
#include "lte_log.h"

#if 0
#define ICCID_FILE_PATH     "/tmp/zte_iccid.file"

#define ZTE_B57_GSM_ONLY           13
#define ZTE_B57_LTE_ONLY           17
#define ZTE_B57_TDSCDMA_ONLY       15
#define ZTE_B57_GSM_TDSCDMA        18
#define ZTE_B57_TDSCDMA_LTE        20
#define ZTE_B57_GSM_LTE_TDSCDMA    24
#define ZTE_B57_AUTO               2


#define ZTE_B57_5_VERSION   "B57_5-M"
#define ZTE_B57_1_VERSION   "B57_1-V1.0"

#define ZTE_B57_5_FLAG      "B57_5"
#define ZTE_B57_1_FLAG      "B57_1"
#define ZTE_B78_FLAG        "B78-"

extern LTE_STATE_INFO_T g_stLteStateInfo;

static char g_acVersion[128] = {0};
static int  g_iSigRssi = 0;
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
#endif
static LTE_RET_E marvell_pin_state (void *pvData, char *pcBuf)
{
    int iRet = 0; 
    char acCiev[] = "+CPIN:";
    char acStateReady[] = "READY";
    char acStatePin[] = "PIN";
    char acStatePuk[] = "PUK";
    char *pcBufTmp = NULL;
    char *pcPinBuf = NULL;
    
    LTE_PIN_STATE_T *pstPinState = (LTE_PIN_STATE_T*)pvData;

    if ((NULL == pvData)||(NULL == pcBuf))
    {
        return LTE_FAIL;
    }
    pcBufTmp = strstr(pcBuf, acCiev);
    if(NULL == pcBufTmp)
    {
        return LTE_FAIL;
    }
    if(NULL == (pcPinBuf = strstr(pcBufTmp, ":")))
    {
        return LTE_FAIL;
    }

    if(NULL != strstr(pcPinBuf,acStateReady))
    {
        /*PIN 码属于 打开状态*/
        pstPinState->enState = PIN_STATE_READY;
    }
    else if(NULL != strstr(pcPinBuf,acStatePuk))
    {
        /*PIN 码属于 puk 状态*/
        pstPinState->enState = PIN_STATE_PUK;
    }
    else if(NULL != strstr(pcPinBuf,acStatePin))
    {
        /*PIN 码属于 关闭状态*/
        pstPinState->enState = PIN_STATE_PIN;
    }
    else
    {
        return LTE_FAIL;
    }
    return LTE_OK;
}
static LTE_RET_E marvell_pin_switch (void *pvData, char *pcBuf)
{
    
    char acCiev[] = "OK";
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte pin switch success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte pin switch failed!",__LINE__,__func__);
    }



    return LTE_FAIL;

}

static LTE_RET_E marvell_pin_sw_get( void *pvData, char *pcBuf )
{
    int iRet = 0; 
    int iATExeCount = 0;
    int iFlag = 0;
    char acBuff[512] = {0};
    char acBuffTmp[32] = {0};
    char *pcTmp = NULL;
    char acCiev[] = "+CLCK:";
    
    LTE_PIN_SW_STATE_T *pstPinSwState = (LTE_PIN_SW_STATE_T*)pvData;

    if ((NULL == pvData) || (NULL == pcBuf))
    {
        return LTE_FAIL;
    }
    if(NULL != (pcTmp = strstr(pcBuf, acCiev)))
    {
        sscanf(pcTmp, "%*[^:]:%s", acBuffTmp);
    }
    else
    {
        return LTE_FAIL;
    }
    pstPinSwState->enState = atoi(acBuffTmp);
    //LTE_LOG(LTE_LOG_ERR,"zte_micro_b57_5_pin_sw_get state:%d buf:%s \n", pstPinSwState->enState, pcBuf);

    return LTE_OK;

}

static LTE_RET_E marvell_pin_unlock (void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte unlock pin success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte unlock pin failed!",__LINE__,__func__);
    }


    return LTE_FAIL;
}
static LTE_RET_E marvell_pin_alter (void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte alter pin success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte alter pin failed!",__LINE__,__func__);
    }


    return LTE_FAIL;
}
static LTE_RET_E marvell_power_on (void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    int iFd = 0;
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte module on success!",__LINE__,__func__);
        return LTE_OK;
    }

    return LTE_FAIL;
}

static LTE_RET_E marvell_power_off (void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";

    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte module off success!",__LINE__,__func__);
        return LTE_OK;
    }

    LTE_LOG(LTE_LOG_ERR,"%d,%s:lte module off failed!",__LINE__,__func__);    
    return LTE_FAIL;
}

static LTE_RET_E marvell_net_type_set (void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";

    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte net type set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte net type set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;
}

#if 0
static LTE_RET_E zte_micro_b57_5_power_get(void *pvData, char *pcBuf)
{
    return LTE_OK;
}

static LTE_RET_E marvell_nettype_get(void *pvData, char *pcBuf)
{
    int iRet = 0;
    FILE *pstFd = NULL;
    char acBuff[512];
    char acBuffTmp[64];
    char acCiev[] = "^SYSCONFIG:";
    char *pcTmp = NULL;
    char acMode[32] = {0};
    char acTmp[32] = {0};
    int  iMode = 0;
    
	LTE_NET_TYPE_T *pstNetType = (LTE_NET_TYPE_T *)pvData;

    if (pvData == NULL)
    {
		return LTE_FAIL;
    }

	if(NULL != (pcTmp = strstr(pcBuf, acCiev)))
    {
        sscanf(pcTmp, "%[^:]:%[^,]", acTmp, acMode);
    }
    else
    {
        return LTE_FAIL;
    }
    iMode = atoi(acMode);

    LTE_LOG(LTE_LOG_INFO, "%d,%s:access mode %d\n",__LINE__,__func__, iMode);
    /* 获取配置的网络类型 */
    switch(iMode)
    {
        case ZTE_B57_GSM_ONLY:
        {
            pstNetType->enNetType = LTE_2G_ONLY;
            
            break;
        }
        
        case ZTE_B57_LTE_ONLY: 
        {
            pstNetType->enNetType = LTE_4G_ONLY;
            break;
        }
        case ZTE_B57_TDSCDMA_ONLY:
        {
            pstNetType->enNetType = LTE_3G_ONLY;
            break;

        }
        case ZTE_B57_GSM_TDSCDMA:
        {
            pstNetType->enNetType = LTE_2G_3G;
            break;

        }
        case ZTE_B57_TDSCDMA_LTE:
        {
            pstNetType->enNetType = LTE_3G_4G;
            //pstNetType->enNetTypePri = LTE_NONE;
            break;

        }   
        case ZTE_B57_GSM_LTE_TDSCDMA:
        {
            pstNetType->enNetType = LTE_2G_3G_4G;
            //pstNetType->enNetTypePri = LTE_NONE;
            break;

        }   
        case ZTE_B57_AUTO:
        {
            pstNetType->enNetType = LTE_2G_3G_4G;
            //pstNetType->enNetTypePri = LTE_NONE;
            break;

        }
        default:
        {
            LTE_LOG(LTE_LOG_ERR,"%d,%s:unkown net type\n",__LINE__,__func__);
            return LTE_FAIL;
        }
    }
    return LTE_OK;
	
}
#endif

static LTE_RET_E marvell_network_prepare(void *pvData, char *pcBuf)
{
    char acNeedleCgreg[16] = "+CGREG";
    char acNeedleCereg[16] = "+CEREG";   
    char acNeedleCreg[16] = "+CREG";   

    char acBufTmp[16] = {0};
    char *pcTmp = NULL;
    
    if(NULL == pvData)    
    {
        return LTE_FAIL;
    }
    LTE_NETWORK_PREPARE_T *pstNwStatus = (LTE_NETWORK_PREPARE_T *)pvData;
    if(NULL != (pcTmp = strstr(pcBuf, acNeedleCreg)))
    {
        sscanf(pcTmp, "%*[^,],%s", acBufTmp);
    }
    else if(NULL != (pcTmp = strstr(pcBuf, acNeedleCgreg)))
    {
        sscanf(pcTmp, "%*[^,],%s", acBufTmp);
    }
    else if(NULL != (pcTmp = strstr(pcBuf, acNeedleCereg)))
    {
        sscanf(pcTmp, "%*[^,],%s", acBufTmp);
    }

    strncpy(pstNwStatus->acNetWorkStatus, acBufTmp, sizeof(pstNwStatus->acNetWorkStatus));
    
    return LTE_OK;
}
static LTE_RET_E marvell_network_status_get(void *pvData, char *pcBuf)
{
    char acNeedleCgreg[16] = "+CGREG";
    char acNeedleCereg[16] = "+CEREG";   
    char acNeedleCreg[16] = "+CREG";  
    char acNwStatus[32] = {0};
    char *pcTmp = NULL;
	int iRegsiterStatus = 0;

    LTE_NETWORK_PREPARE_T *pstNwStatus = (LTE_NETWORK_PREPARE_T *)pvData;
    
    if(NULL == pvData)    
    {
        return LTE_FAIL;
    }
    /*0 未驻留，1 驻留，5 漫游驻留，3 驻留被拒绝，2正在收网，4 unkown*/
    if(NULL != (pcTmp = strstr(pcBuf, acNeedleCreg)))
    {
        sscanf(pcTmp, "%*[^,],%s", acNwStatus);
    }
    else if(NULL != (pcTmp = strstr(pcBuf, acNeedleCgreg)))
    {
        sscanf(pcTmp, "%*[^,],%s", acNwStatus);
    }
    else if(NULL != (pcTmp = strstr(pcBuf, acNeedleCereg)))
    {
        sscanf(pcTmp, "%*[^,],%s", acNwStatus);
    }
    strncpy(pstNwStatus->acNetWorkStatus, acNwStatus, sizeof(pstNwStatus->acNetWorkStatus));
    //sprintf(pstNwStatus->acNetWorkStatus,"%d", iRegsiterStatus);
    return LTE_OK;

}
static LTE_RET_E marvell_connect(void *pvData, char *pcBuf)
{
    char acCiev[] = "+CGDCONT:";
    char acIP[64] = {0};
            
    if(NULL != strstr(pcBuf, acCiev))
    {
        //+CGDCONT: 5,"IP","cmnet.mnc002.mcc460.gprs","10.191.72.34",,,802110030100108106,
        sscanf(pcBuf, "%*[^,],%*[^,],%*[^,],\"%[^\"]", acIP);
        if(acIP[15] == '\0' && strlen(acIP) >= 7 && (acIP[1] == '.' || acIP[2] == '.'))
        {
            return LTE_OK;
        } 
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte connect failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;

}
static LTE_RET_E marvell_disconnect(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
            
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte disconnect success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte disconnect failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;

}

static LTE_RET_E marvell_signal_get (void *pvData, char *pcBuf)
{
    char acBuffTmp[32] = {0};
    char *pcTmp = NULL;
    char acCiev[] = "+CESQ: ";
    
    LTE_SIGNAL_T *pstSignal = (LTE_SIGNAL_T*)pvData;

    if ((NULL == pvData) || (NULL == pcBuf))
    {
        return LTE_FAIL;
    }
    if(NULL != (pcTmp = strstr(pcBuf, acCiev)))
    {
        sscanf(pcTmp, "%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%s", acBuffTmp);
    }
    else
    {
        return LTE_FAIL;
    }
    pstSignal->iSignal = atoi(acBuffTmp);
    return LTE_OK;
}
#if 0

static LTE_RET_E zte_micro_b57_5_linkstate_get (void *pvData, char *pcBuf)
{
    int iRet = 0;
    int iLinkState = 0;
    char *pcTmp = NULL;
    char acBuffTmp[64];
    char acCiev[] = "+ZGACT:";
    
    LTE_LINKSTATE_T *pstLinkstate = (LTE_LINKSTATE_T*)pvData;

    if (pvData == NULL)
    {
		return LTE_FAIL;
    }

    if(NULL != (pcTmp = strstr(pcBuf, acCiev)))
    {
        sscanf(pcTmp, "%*[^,],%s", acBuffTmp);
    }


    iLinkState = atoi(acBuffTmp);
    
    if(0 == iLinkState)
    {
        pstLinkstate->enLinkstate = LTE_LINKSTATE_DISCONNECT;
    }
    else if(1 == iLinkState)
    {
        pstLinkstate->enLinkstate = LTE_LINKSTATE_CONNECTED;
    }

    LTE_LOG(LTE_LOG_INFO,"%d,%s:linkstate:%d",__LINE__,__func__, pstLinkstate->enLinkstate);
    return LTE_OK;
    
}
#endif
static LTE_RET_E marvell_imei_get (void *pvData, char *pcBuf)
{
    int iRet = 0;
    FILE *pstFd = NULL;
    char acBuff[512];
    char acBuffTmp[64];
    char acCiev[] = "+CGSN:";
    char *pcTmp = NULL;

	LTE_IMEI_T *pstImei = (LTE_IMEI_T *)pvData;

    if (pvData == NULL)
    {
		return LTE_FAIL;
    }

	if(NULL != (pcTmp = strstr(pcBuf, acCiev)))
    {
        sscanf(pcBuf, "%[^:]:%s", acBuff, pstImei->acImei);
    }
    
    LTE_LOG(LTE_LOG_INFO,"%d,%s:lte imei:%s",__LINE__,__func__, pstImei->acImei);
    return LTE_OK;
}
#if 0
static LTE_RET_E zte_micro_b57_5_cellid_get (void *pvData, char *pcBuf)
{
    char    acBuff[256] = {0};
    char    acCellIdTmp[32] = {0};
    char    acCiev[] = "+ZEMCI:";
    char    acOkCiev[] = "OK";
    char    *pcTmp = NULL;    
    unsigned long ulCellId;

    LTE_CELLID_T *pstCellid = (LTE_CELLID_T*)pvData;

    if(NULL != (pcTmp = strstr(pcBuf, acCiev)))
    {
        sscanf(pcTmp, "%*[^:]:%[^,]", acCellIdTmp);
        ulCellId = strtoul(acCellIdTmp, NULL, 10);
        /*cellid*/
        snprintf(pstCellid->acCellID, sizeof(pstCellid->acCellID),"%x", ulCellId);
        return LTE_OK;
    }
    if(NULL != (pcTmp = strstr(pcBuf, acOkCiev)))
    {
        //LTE_LOG(LTE_LOG_ERR,"%d,%s:at command success",__LINE__,__func__);
        return LTE_OK;
    }

    return LTE_FAIL;
}

static LTE_RET_E zte_micro_b57_5_iccid_get(void *pvData, char *pcBuf)
{
    return LTE_OK;
    
}

#endif
static LTE_RET_E marvell_imsi_get(void *pvData, char *pcBuf)
{
    int iRet = 0;
    int iLinkState = 0;
    char acBuffTmp[64];
    char acCiev[] = "OK";
    
    LTE_IMSI_T *pstImsiInfo = (LTE_IMSI_T *)pvData;

    if (pvData == NULL)
    {
		return LTE_FAIL;
    }

    //if(NULL != (pcTmp = strstr(pcBuf, acCiev)))
    //{
        sscanf(pcBuf, "%s", acBuffTmp);
        strncpy(pstImsiInfo->acIMSI, acBuffTmp, sizeof(pstImsiInfo->acIMSI));
        LTE_LOG(LTE_LOG_INFO, "imsi:%s\n", acBuffTmp);
    //}
    
    
    LTE_LOG(LTE_LOG_INFO,"%d,%s: sim imsi: %s %s",__LINE__,__func__, pcBuf, pstImsiInfo->acIMSI);
    return LTE_OK;
}

static LTE_RET_E marvell_simstate_get(void *pvData, char *pcBuf)
{
	int iRet = 0; 
    char acTmp[32] = {0};
    char acCiev[] = "^SYSINFO:";
    char acSimStateTmp[32] = {0};
    char acSimState[32] = {0};
    char *pcTmp = NULL;
    int  iSimState = 0;
	LTE_SIM_STATE_T *pstSimState = (LTE_SIM_STATE_T *)pvData;

    if (pvData == NULL)
		return LTE_FAIL;

    if (NULL != (pcTmp = strstr(pcBuf,acCiev)))
    {
        sscanf(pcTmp, "%[^,],%[^,],%[^,],%[^,],%s", acTmp, acTmp, acTmp, acTmp, acSimStateTmp);
        if(NULL != strstr(acSimStateTmp, ","))
        {
            sscanf(acSimStateTmp, "%[^,]", acSimState);
            iSimState = atoi(acSimState);
        }
        else
        {
            iSimState = atoi(acSimStateTmp);
        }
    }
    else
    {        
        LTE_LOG(LTE_LOG_INFO, "%d,%s:get SimState fail!\n",__LINE__,__func__);
        return LTE_FAIL;
    }

    
    LTE_LOG(LTE_LOG_INFO, "%d,%s:SimState %d\n",__LINE__,__func__, iSimState);
    /* 1表示sim卡有效，0或255表示未插卡或处于PIN状态 */
    if(1 != iSimState)
    {
        pstSimState->enSimState = LTE_SIM_NOT_FOUND;
    }
    else
    {
        pstSimState->enSimState = LTE_SIM_READY;
    }
	return LTE_OK;
}
#if 0

static LTE_RET_E zte_micro_b57_5_stop_report(void)
{
    #if 0
    int iRet = 0;
    char acCiev[] = "OK";
    char acBuf[512] = {0};
    LTE_NORMAL_AT_T stAt = {{"",""},"at+zemci=0\r","/tmp/stopcellinfo", 0, 0, 2};
    
    strncpy(stAt.stNeedle.acNeedle, acCiev, sizeof(stAt.stNeedle.acNeedle));
    
    iRet = lte_normal_at_cmd(&stAt, acBuf);
    if (iRet != 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:at command failed\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    #endif
    return LTE_OK;
}

static LTE_RET_E zte_micro_b57_5_cellinfo_get(LTE_INFO_T *pstLteInfo, char *pcBuf)
{
    unsigned long ulCellId;
    char    acCiev[] = "+ZEMCI:";
    char    acCellIdTmp[32] = {0};
    char    acTacTmp[32] = {0};
    char    acCurBand[32] = {0};
    char    acRsrp[32] = {0};
    char    acRsrq[32] = {0};
    char    acPhyciAndCurfreqcy[32] = {0};
    char    acPhyCi[16] = {0};
    char    acCurfreqcy[16] = {0};
    int     iLoop = 0;
    int     iPhy = 0;
    int     iCurFre = 0;

    if ((NULL == pstLteInfo) || (NULL == pcBuf))
    	return LTE_FAIL;

    //printf("cellinfo pcBuf==%s\n", pcBuf);
    if(NULL != strstr(pcBuf, acCiev))
    {
        sscanf(pcBuf, "%*[^:]:%[^,],%*[^,],%[^,],%*[^,],%*[^,],%*[^,],%[^,],%*[^,],%*[^,],%*[^,],%[^,],%[^,],%*[^,],%*[^,],%[^,]", acCellIdTmp, acTacTmp, acCurBand, acRsrp, acRsrq, acPhyciAndCurfreqcy);

        if(0 == strcmp(acCellIdTmp, " 4294967295"))
        {
            return LTE_FAIL;
        }
        snprintf(pstLteInfo->stCellInfo.acCellId, sizeof(pstLteInfo->stCellInfo.acCellId),"%x", atoi(acCellIdTmp));
        for(iLoop = 0; iLoop < 4; iLoop++)
        {
            acPhyCi[iLoop] = acPhyciAndCurfreqcy[iLoop];
            acCurfreqcy[iLoop] = acPhyciAndCurfreqcy[iLoop + 4];
        }
        
        zte_micro_b57_5_hex_dig(acPhyCi, &iPhy);
        zte_micro_b57_5_hex_dig(acCurfreqcy, &iCurFre);
        strncpy(pstLteInfo->stCellInfo.acBand, acCurBand, sizeof(pstLteInfo->stCellInfo.acBand));
        pstLteInfo->stCellInfo.iRsrp = atoi(acRsrp) - 141;
        pstLteInfo->stCellInfo.iRsrq = (atoi(acRsrq) - 40)/2;
        pstLteInfo->stCellInfo.iChannel = iCurFre;
        sprintf(pstLteInfo->stCellInfo.acPhyCeillid, "%d", iPhy);

        ulCellId = strtoul(acCellIdTmp, NULL, 10);
 
        /*共四个字节，取cid的前三个字节*/
        snprintf(g_stLteStateInfo.acEnodebID, sizeof(g_stLteStateInfo.acEnodebID),"%u", (ulCellId >> 8));
        /*共四个字节，取cid的第四个字节*/
        snprintf(g_stLteStateInfo.acLcid, sizeof(g_stLteStateInfo.acLcid),"%u", (ulCellId & LTE_LCID_MASK));
        strncpy(g_stLteStateInfo.acTac, acTacTmp, sizeof(g_stLteStateInfo.acTac));
        
        strncpy(g_stLteStateInfo.acCellId, pstLteInfo->stCellInfo.acCellId, sizeof(g_stLteStateInfo.acCellId));
        strncpy(g_stLteStateInfo.acBand, acCurBand, sizeof(g_stLteStateInfo.acBand));
        strncpy(g_stLteStateInfo.acPhyCellId, pstLteInfo->stCellInfo.acPhyCeillid, sizeof(g_stLteStateInfo.acPhyCellId));
        g_stLteStateInfo.iChannel = pstLteInfo->stCellInfo.iChannel;
        g_stLteStateInfo.iRSRP = pstLteInfo->stCellInfo.iRsrp;
        g_stLteStateInfo.iRSRQ = pstLteInfo->stCellInfo.iRsrq;
        g_stLteStateInfo.iLteInfoStaticFLag = 1;
        LTE_LOG(LTE_LOG_INFO, "lte_normal_at_last_cmd:%s %s %s %s %d %d %s %s", acCurBand, acRsrp, acRsrq, acPhyciAndCurfreqcy, iPhy, iCurFre, acPhyCi, acCurfreqcy);
    }
    else
    {        
        LTE_LOG(LTE_LOG_ERR, "get cell info failed");
        return LTE_FAIL;
    }

    return LTE_OK;
}

static LTE_RET_E zte_micro_b57_5_lteinfo_signal_get (void *pvData, char *pcBuf)
{
    char acCiev[] = "+ZRSSI:";
    char acSignalOne[32] = {0};
    char acSignalTwo[32] = {0};
    int iSignalOne;
    int iSignalTwo;
    char *pcTmp = NULL;

    LTE_SIGNAL_T *pstSignal = (LTE_SIGNAL_T*)pvData;
    
    if(NULL == pvData)
    {
        return LTE_FAIL;
    }
    if(NULL != (pcTmp = strstr(pcBuf,acCiev)))
    {
        sscanf(pcBuf, "%*[^:]:%[^,],%*[^,],%[^,]", acSignalOne, acSignalTwo);
    }
    else
    {
        return LTE_FAIL;
    }
    //LTE_LOG(LTE_LOG_ERR,"%d,%s: acSignalOne:%s,acSignalTwo:%s\n",__LINE__,__func__, acSignalOne,acSignalTwo);
    iSignalOne = strtoul(acSignalOne, NULL, 10);
    iSignalTwo = strtoul(acSignalTwo, NULL, 10);
    //LTE_LOG(LTE_LOG_ERR,"%d,%s: one:%d,two:%d\n",__LINE__,__func__, iSignalOne,iSignalTwo);
    if(iSignalOne > iSignalTwo)
        pstSignal->iSignal = iSignalOne;
    else
        pstSignal->iSignal = iSignalTwo;
    g_stLteStateInfo.iRssi = pstSignal->iSignal;    
    return LTE_OK;
}
static LTE_RET_E zte_micro_b57_sinr_get(int *piSinr, char *pcBuf)
{
    int iRet = 0;
    int iSinrMax = 0;
    int iSinrTmp = 0;
    int iSinr1 = 0;
    int iSinr2 = 0;
    int iSinr3 = 0;
    int iSinr4 = 0;
    char acSinr1[32] = {0};
    char acSinr2[32] = {0};
    char acSinr3[32] = {0};
    char acSinr4[32] = {0};

    char acCiev[32] = "+ZSINR:";
    char acBuff[256] = {0};
    char *pcTmp = NULL;
    FILE *pstFd = NULL;

    if(NULL == piSinr)
    {
        return LTE_FAIL;
    }

    if(NULL != (pcTmp = strstr(pcBuf, acCiev)))
    {
        sscanf(pcTmp, "%*[^:]:%[^,],%*[^,],%[^,],%*[^,],%[^,],%*[^,],%[^,]", acSinr1, acSinr2, acSinr3, acSinr4);
    }
    else
    {
        return LTE_FAIL;
    }
    /* 获取最大信噪比 */
    iSinrTmp = atoi(acSinr1);
    iSinrMax = iSinrTmp;
    
    iSinrTmp = atoi(acSinr2);
    if(iSinrMax > iSinrTmp)
    {
        ;
    }
    else
    {
        iSinrMax = iSinrTmp;
    }
    
    iSinrTmp = atoi(acSinr3);
    if(iSinrMax > iSinrTmp)
    {
        ;
    }
    else
    {
        iSinrMax = iSinrTmp;
    }
    
    iSinrTmp = atoi(acSinr4);
    if(iSinrMax > iSinrTmp)
    {
        ;
    }
    else
    {
        iSinrMax = iSinrTmp;
    }
    *piSinr = iSinrMax;
    g_stLteStateInfo.iSinr = iSinrMax;
    return LTE_OK;
}


static LTE_RET_E zte_micro_b57_5_lte_info_get(void *pvData, char *pcBuf)
{
    int iRet = 0;
    int iSinr = 0;
    char acSigCiev[] = "+ZRSSI:";
    char acSinrCiev[] = "+ZSINR:";
    char acCelCiev[] = "+ZEMCI:";

    char *pcTmp = NULL;
    
    LTE_SIGNAL_T stSignal = {0};
    
	LTE_INFO_T *pstLteInfo = (LTE_INFO_T *)pvData;

    if (pvData == NULL)
    	return LTE_FAIL;

    if(NULL != (pcTmp = strstr(pcBuf, acSigCiev)))
    {
        iRet = zte_micro_b57_5_lteinfo_signal_get(&stSignal, pcBuf);
        pstLteInfo->stCellInfo.iRssi = stSignal.iSignal;
    }
    else if(NULL != (pcTmp = strstr(pcBuf, acSinrCiev)))
    {
        iRet = zte_micro_b57_sinr_get(&iSinr, pcBuf);
        pstLteInfo->stCellInfo.iSinr = iSinr;
    }
    else if(NULL != (pcTmp = strstr(pcBuf, acCelCiev)))
    {
        iRet = zte_micro_b57_5_cellinfo_get(pstLteInfo, pcBuf);
    }
    else if(NULL != (pcTmp = strstr(pcBuf, "OK")))
    {
        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }
    return iRet;
    #if 0
    char          acNeedle[] = "+ZEMCI:";
    int           iSinr = 0;
    
	LTE_INFO_T *pstLteInfo = (LTE_INFO_T *)pvData;

    if (pvData == NULL)
    	return LTE_FAIL;
	
    if(LTE_OK != zte_micro_b57_5_cellinfo_get(pstLteInfo, acNeedle))
    {
        LTE_LOG(LTE_LOG_ERR, "zte_micro_b57_5_cellinfo_get cell info get fail");
    }
    zte_micro_b57_sinr_get(&iSinr);

    pstLteInfo->stCellInfo.iRssi = g_iSigRssi;
    pstLteInfo->stCellInfo.iSinr = iSinr;
    #endif
    return LTE_OK;
}

static LTE_RET_E zte_micro_b57_5_access_net_get(void *pvData, char *pcBuf)
{
	int iRet = 0; 
    char acTmp[32] = {0};
    char acCiev[] = "^SYSINFO:";
    char *pcTmp = NULL;
    char acMode[32] = {0};
    int  iMode = 0;
    
	LTE_ACCESS_NETWORK_T *pstAccessNet = (LTE_ACCESS_NETWORK_T*)pvData;

    if (NULL != (pcTmp = strstr(pcBuf,acCiev)))
    {
        sscanf(pcTmp, "%[^,],%[^,],%[^,],%[^,]", acTmp, acTmp, acTmp, acMode);
        
        iMode = atoi(acMode);
        
        LTE_LOG(LTE_LOG_INFO, "%d,%s:access mode %d\n",__LINE__,__func__, iMode);
        /* 1表示sim卡有效，0或255表示未插卡或处于PIN状态 */
        if(15 == iMode)
        {
            pstAccessNet->enAccessNet = NET_3G;
        }
        else if(17 == iMode)
        {
            pstAccessNet->enAccessNet = NET_4G;
        }
        else if(3 == iMode)
        {
            pstAccessNet->enAccessNet = NET_2G;
        }
        else
        {
            pstAccessNet->enAccessNet = NET_NONE;
        }
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:get access mode fail\n",__LINE__,__func__);
        return LTE_FAIL;
    }
	return LTE_OK;
}

static LTE_RET_E zte_micro_b57_5_bands_get(void *pvData, char *pcBuf)
{    
    #if 0
    int iVerSionFlag = 0;
	LTE_BANDS_T *pstBands = (LTE_BANDS_T *)pvData;
    /* 38,39,40,41 */
    char b57_5_bands[] = "38,39,40"; 
    char b57_1_bands[] = "3,8,38,39,40";
    /* 1为B57_5模块版本；2为B57_1版本 */
    LTE_LOG(LTE_LOG_INFO, "%d,%s:module version %s\n",__LINE__,__func__, g_acVersion);
    if(NULL != strstr(g_acVersion, ZTE_B57_5_VERSION))
    {
        memcpy(pstBands->bands, b57_5_bands, sizeof(pstBands->bands));
    }
    else if(NULL != strstr(g_acVersion, ZTE_B57_1_VERSION))
    {
        memcpy(pstBands->bands, b57_1_bands, sizeof(pstBands->bands));
    }
    
    pstBands->iClcBandAble = CLC_BAND_ENABLE; /* 支持锁频 */
    #endif
	return LTE_OK;
}
static LTE_RET_E zte_micro_b57_5_clck_bands_set(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte apn set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte apn set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;

}
static LTE_RET_E zte_micro_b57_5_clckcell_enable_get(void *pvData, char *pcBuf)
{
    LTE_CLKCELL_ENABLE_T *pstClkCellEnable = (LTE_CLKCELL_ENABLE_T *)pvData;

    if (pvData == NULL)
    {
		return LTE_FAIL;
    }
    
    /* 中兴微电子支持锁小区 */
    pstClkCellEnable->iClkCellEnable = 1;

    return LTE_OK;
}
static LTE_RET_E zte_micro_b57_5_clck_cellid_set(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
        
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte apn set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte apn set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;

}
#endif
static LTE_RET_E marvell_oper_info_get(void *pvData, char *pcBuf)
{
    int iRet = 0;
    char acCiev[] = "+COPS:";
    char acOkCiev[] = "OK";
    char acOperTmp[16] = {0};
    char *pcTmp = NULL;
    LTE_RET_E enRet = 0;

    LTE_OPER_INFO_T *pstOperInfo = (LTE_OPER_INFO_T *)pvData;
    
    if (pvData == NULL)
    {
        return LTE_FAIL;
    }

    if(NULL != (pcTmp = strstr(pcBuf, acOkCiev)))
    {
        return LTE_OK;
    }
    
    if (NULL != strstr(pcBuf, acCiev))
    {
        sscanf(pcBuf,"%*[^\"]\"%[^\"]", pstOperInfo->acOperName);   /*+COPS: (0,0,"CHINA MOBILE",7,0),(0,0,"CHINA MOBILE",7,0)*/

        /*if(NULL != strstr(LTE_CHINA_MOBILE_CODE, acOperTmp))
        {
            strncpy(pstOperInfo->acOperName, LTE_CHINA_MOBILE_NAME, sizeof(pstOperInfo->acOperName));
        }
        else if(NULL != strstr(LTE_CHINA_UNICOM_CODE, acOperTmp))
        {
            strncpy(pstOperInfo->acOperName, LTE_CHINA_UNICOM_NAME, sizeof(pstOperInfo->acOperName));
        }
        else if(NULL != strstr(LTE_CHINA_TELECOM_CODE, acOperTmp))
        {
            strncpy(pstOperInfo->acOperName, LTE_CHINA_TELECOM_NAME, sizeof(pstOperInfo->acOperName));
        }
        else
        {
            strncpy(pstOperInfo->acOperName, acOperTmp, sizeof(pstOperInfo->acOperName));
        }*/
                
        return LTE_OK;
    }
   
    return LTE_FAIL;

}

static LTE_RET_E zte_micro_b57_5_sup_nettype_get(void *pvData, char *pcBuf)
{
    int flag = 0;
    char acBuff[128] = {0};
    LTE_NET_TYPE_T *pstNets = (LTE_NET_TYPE_T *)pvData;
    //char NetTypeInfo[] = "3G";
    if(NULL == pvData)
    {
        return LTE_FAIL;
    }

    pstNets->enNetType = LTE_4G_ONLY;
    pstNets->enNetTypePri = LTE_NONE;
    return LTE_OK;
}

static LTE_RET_E marvell_sms_cent_code_set(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";

    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte sms cent code set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte sms cent code set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;

}
static LTE_RET_E marvell_sms_mode_set(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";

    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte sms mode set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte sms mode set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;

}
static LTE_RET_E marvell_sms_func_init(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";

    if(NULL != strstr(pcBuf, "+CSMS:"))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte sms init [CSMS] success!",__LINE__,__func__);
        return LTE_OK;
    }
    else if(NULL != strstr(pcBuf, "+CPMS:"))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte sms init [CPMS] success!",__LINE__,__func__);
        return LTE_OK;
    }
    else if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte sms init success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte sms init failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;

}
static LTE_RET_E marvell_sms_recv(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    int  iFd = 0;
    int  iWriteLen = 0;
    LTE_SMS_RECV_T *pstSmsRecv = (LTE_SMS_RECV_T *)pvData;

    if(NULL == pvData)
    {
        return LTE_FAIL;
    }
    strncpy(pstSmsRecv->acSmsRecvFile, LTE_SMS_RECV_FILE, sizeof(pstSmsRecv->acSmsRecvFile));

    
    return LTE_OK;

}
static LTE_RET_E marvell_sms_send(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte sms send success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte sms send failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;

}

static LTE_RET_E marvell_sms_dele(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";

    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte sms delete success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte sms delete failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;

}

static LTE_RET_E marvell_sms_sup_func(void *pvData, char *pcBuf)
{
    int flag = 0;
    LTE_SUPPORT_SMS_T *pstSup = (LTE_SUPPORT_SMS_T *)pvData;
    
    if(NULL == pvData)
    {
        return LTE_FAIL;
    }
    pstSup->enSupSms = SUP_SMS_FUNC;
    return LTE_OK;

}

#if 0
static LTE_RET_E zte_micro_b57_5_cgdcont_set(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";

    //printf("zte_micro_b57_5_cgdcont_set==%s\n", pcBuf);
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte pdp set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte pdp set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;
}

static LTE_RET_E zte_micro_b57_5_apn_set(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";

    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte apn set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte apn set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;

}
#endif
static LTE_RET_E marvell_ver_get(void *pvData, char *pcBuf)
{
    int iRet = 0;
    char acVersion[64] = {0};
    char *pcTmp = NULL;
    char acCiev[] = "+CGMR:";
     
    LTE_MODULE_VERSION_T *pstVersion = (LTE_MODULE_VERSION_T *)pvData;

    if(NULL == pvData)
    {
        return LTE_FAIL;
    }
    if(NULL == (pcTmp = strstr(pcBuf, acCiev)))
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:get module version failed!",__LINE__,__func__);
        return LTE_FAIL;
    }
    sscanf(pcTmp, "%*[^\"]\"%[^\"]", acVersion);
    strncpy(pstVersion->acVersion, acVersion, sizeof(pstVersion->acVersion));
    pstVersion->iVersionIndex = LTE_MARVELL;   

   
    return LTE_OK;
}
#if 0
static LTE_RET_E zte_micro_b57_n_enodeb_id_get(void *pvData, char *pcBuf)
{
    char    acEnodebTmp[32] = {0};
    int     iRet = 0;
    unsigned long ulEnodeb;
    char    acCiev[32] = "+ZEMCI:";
    char    *pcBufTmp = NULL;

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_ENODB_ID_T *pstEnodebId = (LTE_ENODB_ID_T *)pvData;

    if(NULL != (pcBufTmp = strstr(pcBuf, "+ZEMCI:")))
    {
        sscanf(pcBufTmp, "%*[^:]:%[^,]", acEnodebTmp);
    }
    else if(NULL != strstr(pcBuf, "OK"))
    {
        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }

    ulEnodeb = strtoul(acEnodebTmp, NULL, 10);
    /*共四个字节，取cid的前三个字节*/
    snprintf(pstEnodebId->acEnodebId, sizeof(pstEnodebId->acEnodebId),"%u", (ulEnodeb >> 8));
    LTE_LOG(LTE_LOG_INFO,"%d,%s: acEnodebTmp:%s acEnodeb:%d\n",__LINE__,__func__, acEnodebTmp, ulEnodeb >> 8);

    return LTE_OK;
}
static LTE_RET_E zte_micro_b57_n_tac_get(void *pvData, char *pcBuf)
{
    char    acTacTmp[32] = {0};
    int     iRet = 0;
    char    acCiev[32] = "+ZEMCI:";
    char    *pcBufTmp = NULL;

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_TAC_T *pstTac = (LTE_TAC_T *)pvData;
    
    if(NULL != (pcBufTmp = strstr(pcBuf, "+ZEMCI:")))
    {
        sscanf(pcBufTmp, "%*[^,],%*[^,],%[^,]", acTacTmp);
    }
    else if(NULL != strstr(pcBuf, "OK"))
    {
        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }
    
    strncpy(pstTac->acTac, acTacTmp, sizeof(pstTac->acTac));
    
    LTE_LOG(LTE_LOG_INFO,"%d,%s: acTacTmp:%s\n",__LINE__,__func__, acTacTmp);
    return LTE_OK;
}
static LTE_RET_E zte_micro_b57_n_lcid_get(void *pvData, char *pcBuf)
{
    unsigned long ulLcid;
    FILE    *pstFd = NULL;
    char    acEnodebTmp[32] = {0};
    int     iRet = 0;
    char    acCiev[32] = "+ZEMCI:";
    char    *pcBufTmp = NULL;

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_LCID_T *pstLcid = (LTE_LCID_T *)pvData;

    if(NULL != (pcBufTmp = strstr(pcBuf, "+ZEMCI:")))
    {
        sscanf(pcBufTmp, "%*[^:]:%[^,]", acEnodebTmp);
    }
    else if(NULL != strstr(pcBuf, "OK"))
    {
        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }

    ulLcid = strtoul(acEnodebTmp, NULL, 10);
    /*共四个字节，取cid的前三个字节*/
    snprintf(pstLcid->acLcid, sizeof(pstLcid->acLcid),"%u", (ulLcid & LTE_LCID_MASK));
    LTE_LOG(LTE_LOG_INFO,"%d,%s: acEnodebTmp:%s\n",__LINE__,__func__, acEnodebTmp);
    return LTE_OK;
}
static LTE_RET_E zte_micro_b57_n_all_cellid_get(void *pvData, char *pcBuf)
{
    char    acCellIdTmp[32] = {0};
    char    acTacTmp[32] = {0};
    int     iRet = 0;
    unsigned long ulCellId;
    char    *pcBufTmp = NULL;

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_ALL_CELLID_T *pstAllCid = (LTE_ALL_CELLID_T *)pvData;

    if(NULL != (pcBufTmp = strstr(pcBuf, "+ZEMCI:")))
    {
        sscanf(pcBufTmp, "%*[^:]:%[^,]", acCellIdTmp);        
        sscanf(pcBufTmp, "%*[^,],%*[^,],%[^,]", acTacTmp);
    }
    /*else if(NULL != strstr(pcBuf, "OK"))
    {
        return LTE_OK;
    }*/
    else
    {
        return LTE_FAIL;
    }
    if(0 == strlen(acCellIdTmp))
    {
        return LTE_FAIL;
    }

    ulCellId = strtoul(acCellIdTmp, NULL, 10);
    /*cellid*/
    snprintf(pstAllCid->acCellId, sizeof(pstAllCid->acCellId),"%x", ulCellId);
    /*共四个字节，取cid的前三个字节*/
    snprintf(pstAllCid->acEnodebId, sizeof(pstAllCid->acEnodebId),"%u", (ulCellId >> 8));
    /*共四个字节，取cid的第四个字节*/
    snprintf(pstAllCid->acLcid, sizeof(pstAllCid->acLcid),"%u", (ulCellId & LTE_LCID_MASK));
    strncpy(pstAllCid->acTac, acTacTmp, sizeof(pstAllCid->acTac));
    return LTE_OK;

}

static LTE_RET_E zte_micro_b57_n_sim_unlock(void *pvData, char *pcBuf)
{
    char acCiev[] = "+ZUPCI:";
    char *pcBufTmp = NULL;
    char acSimType[32] = {0};
    int iSimType = 0;

    if(NULL == pvData)
    {
        return LTE_FAIL;
    }
    LTE_SIM_RANDOM_NUM_T *pstRandomNum = (LTE_SIM_RANDOM_NUM_T*)pvData;
    //printf("zte_micro_b57_n_sim_unlock==%s\n", pcBuf);
    if(NULL != (pcBufTmp = strstr(pcBuf, acCiev)))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:zte_micro_b57_n_sim_unlock success!",__LINE__,__func__);
        sscanf(pcBufTmp, "%*[^:]:%[^,]", acSimType); 
        iSimType = atoi(acSimType);
        //printf("iSimType==%d\n", iSimType);
        if(1 == iSimType)//认证成功
        {
            ;
        }
        /*else if(33 == iSimType)//认证失败
        {
            return LTE_FAIL;
        }*/
        else
        {
            return LTE_FAIL;
        }
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:zte_micro_b57_n_sim_unlock failed!",__LINE__,__func__);
    }    
    return LTE_FAIL;
}

static LTE_RET_E zte_micro_b57_n_sim_type_get(void *pvData, char *pcBuf)
{
    char acCiev[] = "+ZUPCI:";
    char *pcBufTmp = NULL;
    char acSimType[32] = {0};
    int iSimType = 0;

    if(NULL == pvData)
    {
        return LTE_FAIL;
    }
    LTE_SIM_RANDOM_NUM_T *pstRandomNum = (LTE_SIM_RANDOM_NUM_T*)pvData;
    //printf("zte_micro_b57_n_sim_unlock==%s\n", pcBuf);
    if(NULL != (pcBufTmp = strstr(pcBuf, acCiev)))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:zte_micro_b57_n_sim_unlock success!",__LINE__,__func__);
        sscanf(pcBufTmp, "%*[^:]:%[^,]", acSimType); 
        iSimType = atoi(acSimType);
        //printf("iSimType==%d\n", iSimType);
        if(35 == iSimType)//拿随机数
        {
            pstRandomNum->iSIMType = ENCRYPTION_SIM;
            sscanf(pcBufTmp, "%*[^\"]\"%[^\"]", pstRandomNum->cRandomNum);
        }
        else
        {
            pstRandomNum->iSIMType = NORMAL_SIM;
            pstRandomNum->cRandomNum[0] = '\0';
        }
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:zte_micro_b57_n_sim_unlock failed!",__LINE__,__func__);
    }    
    return LTE_FAIL;
}


static LTE_RET_E zte_micro_b57_n_sim_disable_unlock(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";

    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:leadcore_remove_bind success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:leadcore_remove_bind failed!",__LINE__,__func__);
    }   
    return LTE_FAIL;
}

void ipstring_to_num(char *pcIp, UINT8 *pcIpadder)
{
    char *p;
    int i = 0;

    p = strtok(pcIp, ".");
    while (p)
    {
        pcIpadder[i] = atoi(p);
        p = strtok(NULL, ".");

#if 0
        /* For debug */
        printf("pcIpadder[i]=%d\n", pcIpadder[i]);
#endif
        i++;
    }

    return;
}


static LTE_RET_E separation_v4_v6_address(char *pSrc, UINT8 *pucDesV4, UINT8 *pucDesV6)
{
    char acDesV4[32] = {0};
    //char acDesV6[128] = {0};
    char *pDesV4 = acDesV4;
    //char *pDesV6 = acDesV6;
    char *pPoint = pSrc;
    int iCount = 0;
    while(*pSrc != '\0')
    {
        if(*pSrc == '.')
        {
            ++iCount;
        }
        if(iCount == 4)
        {
            break;
        }
        
        *pDesV4 = *pSrc;
        ++pSrc;
        ++pDesV4;
    }
    ipstring_to_num(acDesV4, pucDesV4);
    ++pSrc;
    ipstring_to_num(pSrc, pucDesV6);
    //strcpy(pDesV6, pSrc);
    return LTE_OK;
}


static LTE_RET_E zte_micro_b57_5_ip_gw_dns_get(void *pvData, char *pcBuf)
{
    char acCiev[] = "+ZGIPDNS:";
    char acIpType[16] = {0};
    char acIP[80]={0};
    char acGw[80]={0};
    char acDns1[80]={0};
    char acDns2[80]={0};

    //printf("pcBuf==%s\n", pcBuf);
    if(NULL == pvData)
    {
        return LTE_FAIL;
    }
    LTE_IP_GATEWAY_DNS_T *pstIpGateWay = (LTE_IP_GATEWAY_DNS_T *)pvData;
    memset(pstIpGateWay, 0, sizeof(LTE_IP_GATEWAY_DNS_T));
    if(NULL == strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:zte_micro_b57_5_ip_gateway_dns_get failed!",__LINE__,__func__);        
        return LTE_FAIL;
    }
    
    sscanf(pcBuf, "%*[^\"]\"%[^\"]\",\"%[^\"]\",\"%[^\"]\",\"%[^\"]\",\"%[^\"]", acIpType, acIP, acGw, acDns1, acDns2);
    //printf("acIpType==%s,acIP==%s, acGw==%s, acDns1==%s, acDns2==%s\n", acIpType, acIP, acGw, acDns1, acDns2);
    if(0 == strcmp(acIpType, "IPV4V6"))
    {
        pstIpGateWay->enPdpStack = PDP_IPV4V6;
        #if 0
        sscanf(acIP, IP_FMT"."IPV6_FMT, IP_SSCAN_ARG(pstIpGateWay->acIpv4),IPV6_SSCAN_ARG(pstIpGateWay->acIpv6));
        sscanf(acGw, IP_FMT"."IPV6_FMT, IP_SSCAN_ARG(pstIpGateWay->acGatewayV4),IPV6_SSCAN_ARG(pstIpGateWay->acGatewayV6));
        sscanf(acDns1, IP_FMT"."IPV6_FMT, IP_SSCAN_ARG(pstIpGateWay->acDns1V4),IPV6_SSCAN_ARG(pstIpGateWay->acDns1V6));
        sscanf(acDns2, IP_FMT"."IPV6_FMT, IP_SSCAN_ARG(pstIpGateWay->acDns2V4),IPV6_SSCAN_ARG(pstIpGateWay->acDns2V6));
        #endif
        separation_v4_v6_address(acIP, pstIpGateWay->acIpv4, pstIpGateWay->acIpv6);
        separation_v4_v6_address(acGw, pstIpGateWay->acGatewayV4, pstIpGateWay->acGatewayV6);
        separation_v4_v6_address(acDns1, pstIpGateWay->acDns1V4, pstIpGateWay->acDns1V6);
        separation_v4_v6_address(acDns2, pstIpGateWay->acDns2V4, pstIpGateWay->acDns2V6);
    }
    else if(0 == strcmp(acIpType, "IPV6"))
    {
        pstIpGateWay->enPdpStack = PDP_IPV6;

        ipstring_to_num(acIP, pstIpGateWay->acIpv6);
        ipstring_to_num(acGw, pstIpGateWay->acGatewayV6);
        ipstring_to_num(acDns1, pstIpGateWay->acDns1V6);
        ipstring_to_num(acDns2, pstIpGateWay->acDns2V6);
        #if 0
        sscanf(acIP, IPV6_FMT, IPV6_SSCAN_ARG(pstIpGateWay->acIpv6));
        sscanf(acGw, IPV6_FMT, IPV6_SSCAN_ARG(pstIpGateWay->acGatewayV6));
        sscanf(acDns1, IPV6_FMT, IPV6_SSCAN_ARG(pstIpGateWay->acDns1V6));
        sscanf(acDns2, IPV6_FMT, IPV6_SSCAN_ARG(pstIpGateWay->acDns2V6));
        
        strncpy(pstIpGateWay->acIpv6, acIP, sizeof(pstIpGateWay->acIpv6));
        strncpy(pstIpGateWay->acGatewayV6, acGw, sizeof(pstIpGateWay->acGatewayV6));
        strncpy(pstIpGateWay->acDns1V6, acDns1, sizeof(pstIpGateWay->acDns1V6));
        strncpy(pstIpGateWay->acDns2V6, acDns2, sizeof(pstIpGateWay->acDns2V6));
        #endif
    }
    else if(0 == strcmp(acIpType, "IP"))
    {
        pstIpGateWay->enPdpStack = PDP_IPV4;
        
        ipstring_to_num(acIP, pstIpGateWay->acIpv4);
        ipstring_to_num(acGw, pstIpGateWay->acGatewayV4);
        ipstring_to_num(acDns1, pstIpGateWay->acDns1V4);
        ipstring_to_num(acDns2, pstIpGateWay->acDns2V4);
    }
    else
    {
        return LTE_FAIL;
    }
    
    return LTE_OK;
}

/*static LTE_RET_E zte_micro_b57_n_sim_unlock_get(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    if(NULL == pvData)
    {
        return LTE_FAIL;
    }
    LTE_SIM_UNLOCK_STATE_T *pstUnlockState = (LTE_SIM_UNLOCK_STATE_T *)pvData;
    if(NULL != strstr(pcBuf, acCiev))
    {
        pstUnlockState->iIsUnlock = g_stLteStateInfo.iSimUnlockState;
        LTE_LOG(LTE_LOG_INFO, "%d,%s:zte_micro_b57_n_sim_unlock_get success!",__LINE__,__func__);
        return LTE_OK;
    }
    LTE_LOG(LTE_LOG_ERR,"%d,%s:zte_micro_b57_n_sim_unlock_get failed!",__LINE__,__func__);
    return LTE_FAIL;
}*/


static LTE_RET_E zte_micro_b57_n_all_plmn_mode_get(void *pvData, char *pcBuf)
{
    char acCiev[] = "+COPS:";
    LTE_ALL_PLMN_MODE_T *pstAllPlmnMode = (LTE_ALL_PLMN_MODE_T *)pvData;
    char *p, *pObject;
    char acPlmn[16] = {0};
    char acMode[16] = {0};
    
    if(NULL == (p = strstr(pcBuf, acCiev)))
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:leadcore_all_plmn_mode_get failed!",__LINE__,__func__);
        return LTE_FAIL;
    }
    memset(pstAllPlmnMode->acAllPlmn, 0, sizeof(pstAllPlmnMode->acAllPlmn));
    pObject = pstAllPlmnMode->acAllPlmn;
    while(NULL != (p = strstr(p, "460")))
    {
        sscanf(p, "%[^\"]\",%[^,]", acPlmn, acMode);
        //printf("acPlmn==%s, acMode==%s\n", acPlmn, acMode);
        p += 8;
        if(0 == strcmp("7", acMode))
        {
            sprintf(pObject, "%s+4G,", acPlmn);
        }
        else if(0 == strcmp("2", acMode))
        {
            sprintf(pObject, "%s+3G,", acPlmn);
        }
        else if(0 == strcmp("0", acMode))
        {
            sprintf(pObject, "%s+2G,", acPlmn);
        }
        else
        {
            continue;
        }
        pObject += strlen(pObject);    
    }
    *(pObject - 1) = '\0';
    return LTE_OK;    
}

static LTE_RET_E zte_micro_b57_n_sounet_mode_set(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";

    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:leadcore_sounet_mode_set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:leadcore_sounet_mode_set failed!",__LINE__,__func__);
    }   
    return LTE_FAIL;
}

static LTE_RET_E zte_micro_b57_n_puk_unlock(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";

    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:zte_micro_b57_n_puk_unlock success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:zte_micro_b57_n_puk_unlock failed!",__LINE__,__func__);
    }   

    return LTE_FAIL;
}
#endif

static LTE_RET_E marvell_pin_input_remain(void *pvData, char *pcBuf)
{
    char    acTacTmp[32] = {0};
    int     iRet = 0;
    char    acCiev[32] = "+EPIN:";
    char    *pcBufTmp = NULL;

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_PIN_INPUT_REMAIN_T *pstPinInputRemain = (LTE_PIN_INPUT_REMAIN_T *)pvData;
    
    if(NULL != (pcBufTmp = strstr(pcBuf, acCiev)))
    {
        sscanf(pcBufTmp, "%*[^:]:%[^,]", acTacTmp);
    }
    else
    {
        return LTE_FAIL;
    }
    
    pstPinInputRemain->iPinIputRemain = atoi(acTacTmp);
    
    return LTE_OK;
}



LTE_MODULE_OPS_T stLte_marvell_ops_t = {
    .lte_pin_state_get       = marvell_pin_state,
    .lte_pin_switch          = marvell_pin_switch,    
    .lte_pin_switch_get      = marvell_pin_sw_get,
    .lte_pin_unlock          = marvell_pin_unlock,
    .lte_pin_attempts        = NULL,
    .lte_pin_code_alter      = marvell_pin_alter,
    .lte_signal_get          = marvell_signal_get,
    .lte_power_on            = marvell_power_on,
    .lte_power_off           = marvell_power_off,
    .lte_power_state_get     = NULL,
    .lte_net_type_set        = marvell_net_type_set,
    .lte_net_type_get        = NULL,
    .lte_plmn_register       = NULL,
    .lte_plmn_prefer_set     = NULL,
    .lte_net_work_prepare    = marvell_network_prepare,
    .lte_net_work_status_get = marvell_network_status_get,
    .lte_connect             = marvell_connect,
    .lte_disconnect          = NULL,
    .lte_link_state_get      = NULL,
    .lte_imei_get            = marvell_imei_get,
    .lte_cellid_get          = NULL,
    .lte_iccid_get           = NULL,
    .lte_imsi_get            = marvell_imsi_get,
    .lte_sim_state_get       = marvell_simstate_get,
    .lte_info_get            = NULL,
    .lte_access_net_get      = NULL,
    .lte_bands_get           = NULL,
    .lte_clck_bands          = NULL,
    .lte_clck_cell_get       = NULL,
    .lte_clck_cell           = NULL,
    .lte_gps_start           = NULL, 
    .lte_gps_stop            = NULL,
    .lte_oper_info_get       = marvell_oper_info_get,
    .lte_nettype_info_get    = NULL,
    .lte_sms_cent_code_set   = marvell_sms_cent_code_set,
    .lte_sms_mode_set        = marvell_sms_mode_set,
    .lte_sms_func_init       = marvell_sms_func_init,
    .lte_sms_recv            = marvell_sms_recv,
    .lte_sms_send            = marvell_sms_send,
    .lte_sms_del             = marvell_sms_dele,
    .lte_sms_supinfo_get     = marvell_sms_sup_func,
    .lte_sms_ip_stack_set    = NULL,
    .lte_apn_get             = NULL,
    .lte_apn_set             = NULL,
    .lte_version_get         = marvell_ver_get,
    .lte_enodebid_get        = NULL,
    .lte_tac_get             = NULL,
    .lte_lcid_get            = NULL,
    .lte_all_cellid_get      = NULL,
    .lte_sim_unlock          = NULL,
    .lte_sim_disable_unlock  = NULL,
    .lte_sim_type_get        = NULL,
    .lte_ip_gw_dns_get       = NULL,
    .lte_all_plmn_mode_get   = NULL,
    .lte_sounet_mode_set     = NULL,
    .lte_sim_puk_unlock		 = NULL,
    .lte_pin_input_remain    = marvell_pin_input_remain,
};


