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

#include "lte.h"
#include "lte_log.h"
#include "lte_common.h"
#include "lte_usbcom.h"

#define ATCOM "/dev/ttyUSB2"


static LTE_RET_E thinkwill_wcdma_mi900_power_on (void *pvData, int iDataLen)
{
    LTE_NORMAL_AT_T stAt = {"at+cfun=1\r","/tmp/cfun","OK",0,ATCOM,0,3};
    int iRet = 0;

    /*ִ��atָ��*/
    iRet = lte_normal_at_cmd(&stAt);
    if (iRet != 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:at command failed\n",__LINE__,__func__);
        lte_file_unlink(stAt.acFileName);
        /*Ĭ�Ͽ���*/
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

static LTE_RET_E thinkwill_wcdma_mi900_power_off (void *pvData, int iDataLen)
{
    LTE_NORMAL_AT_T stAt = {"at+cfun=0\r","/tmp/cfunoff","ERROR",0,ATCOM,0,1};
    int iRet = 0;
    /*ִ��atָ��*/
    iRet = lte_normal_at_cmd(&stAt);
    if (iRet != 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:at command failed\n",__LINE__,__func__);
        lte_file_unlink(stAt.acFileName);
        /*Ĭ�Ͽ���*/
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
static LTE_RET_E thinkwill_wcdma_mi900_signal_get (void *pvData, int iDataLen)
{
    unsigned int uiFlag = 0;
    int iRet = 0;
    char acCiev[] = "+csq:";
    char acNeedleComma[] = ",";
    char acBuff[64] = {0};
    char acSignalTmp[64] = {0};
    char acBuffTmp[64] = {0};
    char *pcTmp = NULL;
    FILE* pstFd = NULL;
    LTE_RET_E enRet = 0;

    LTE_SIGNAL_T *pstSignal = (LTE_SIGNAL_T*)pvData;
    LTE_NORMAL_AT_T stAt = {"at+csq\r","/tmp/signalquality","OK",0,ATCOM,0,4};
    /*ִ��atָ��*/
    iRet = lte_normal_at_cmd(&stAt);
    if (iRet != 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:at command failed\n",__LINE__,__func__);
        lte_file_unlink(stAt.acFileName);
        /*Ĭ�Ͽ���*/
        return LTE_FAIL;
    }
    if (0 != stAt.iResult)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:compare AT return value not OK\n",__LINE__,__func__);
        //lte_file_unlink(stAt.acFileName);
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
    sscanf(acBuff, "%[^:]:%[^,]", acBuffTmp, acSignalTmp);
    pstSignal->uiSignal = atoi(acSignalTmp);
    return LTE_OK;
}

static LTE_RET_E thinkwill_wcdma_mi900_linkstate_get(void *pvData, int iDataLen)
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


static LTE_RET_E thinkwill_wcdma_mi900_imei_get (void *pvData, int iDataLen)
{
    if (pvData == NULL)
		return LTE_FAIL;
    LTE_NORMAL_AT_T stAt = {"at+gsn\r","/tmp/imei","OK",0,ATCOM,0,1};
    int iRet = 0;
    int iLoop = 0;
    FILE *pstFd = NULL;
    char acBuff[64];
    char acNeedleCgreg[16] = "ERROR";
	LTE_IMEI_T *pstImei = (LTE_IMEI_T *)pvData;
    /*ִ��atָ��*/
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
    iRet = LTE_FAIL;
    while (NULL != fgets(acBuff,64,pstFd))
    { 	
        if((acBuff[0] >= '0') && (acBuff[0] <= '9'))
        {
            sscanf(acBuff, "%s", pstImei->acImei);
            iRet = LTE_OK;
            break;
        }

        memset(acBuff,0,64);
    } 
    fclose(pstFd);
    pstFd = NULL;
    lte_file_unlink(stAt.acFileName);
    return iRet;
}
static LTE_RET_E thinkwill_wcdma_mi900_imsi_get(void *pvData, int iDataLen)
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
    
    /*ִ��atָ��*/
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
static LTE_RET_E thinkwill_wcdma_mi900_simstate_get(void *pvData, int iDataLen)
{
	if (pvData == NULL)
		return LTE_FAIL;
	
	int iRet = 0; 

	LTE_SIM_STATE_T *pstSimState = (LTE_SIM_STATE_T *)pvData;
	LTE_NORMAL_AT_T stAt = {"at+cpin?\r","/tmp/pinstate","ERROR",0,ATCOM,0,1};

    	/*ִ��atָ��*/
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
static LTE_RET_E thinkwill_wcdma_mi900_network_prepare(void *pvData, int iDataLen)
{
#if 0
    int iRegsiterStatus = 0;

    char acNeedleCgreg[16] = "+CGREG";  
    char acCmdCgreg[16] =  "at+cgreg?\r";
    
    LTE_NETWORK_PREPARE_T *pstNwStatus = (LTE_NETWORK_PREPARE_T *)pvData;
        /*0 δפ����1 פ����5 ����פ����3 פ�����ܾ���2����������4 unkown*/
        /*����td-s/gsm/gprs/edge*/
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
static LTE_RET_E thinkwill_wcdma_mi900_network_status_get(void *pvData, int iDataLen)
{
#if 1
    int iRegsiterStatus = 0;

    char acNeedleCgreg[16] = "+CGREG";  
    char acCmdCgreg[16] =  "at+cgreg?\r";

    LTE_NETWORK_PREPARE_T *pstNwStatus = (LTE_NETWORK_PREPARE_T *)pvData;
    /*0 δפ����1 פ����5 ����פ����3 פ�����ܾ���2����������4 unkown*/
    /*����td-s/gsm/gprs/edge*/
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
}
static LTE_RET_E thinkwill_wcdma_mi900_connect(void *pvData, int iDataLen)
{
    /*��wcdma �Ĳ�������*/
    system_call("/usr/sbin/pppd call thinkwill_wcdma 1>/tmp/ppp_state 2>/tmp/ppp_error &");

    return LTE_OK;
}

static LTE_RET_E thinkwill_wcdma_mi900_disconnect(void *pvData, int iDataLen)

{
    system_call("/usr/sbin/poff &");
	return LTE_OK;
}

static LTE_RET_E thinkwill_wcdma_mi900_gps_start(void *pvData, int iDataLen)
{
    LTE_NORMAL_AT_T stAt = {"at^gpsstart\r","/tmp/gpsstart","OK",0,ATCOM,0,2};
    int iRet = 0;

    /*ִ��atָ��*/
    iRet = lte_normal_at_cmd(&stAt);
    if (iRet != 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:at command failed",__LINE__,__func__);
        lte_file_unlink(stAt.acFileName);
        /*Ĭ�Ͽ���*/
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
static LTE_RET_E thinkwill_wcdma_mi900_gps_stop(void *pvData, int iDataLen)
{
    LTE_NORMAL_AT_T stAt = {"at^gpsstop\r","/tmp/gpsstop","OK",0,ATCOM,0,2};
    int iRet = 0;

    /*ִ��atָ��*/
    iRet = lte_normal_at_cmd(&stAt);
    if (iRet != 0)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:at command failed",__LINE__,__func__);
        lte_file_unlink(stAt.acFileName);
        /*Ĭ�Ͽ���*/
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
/* ���ļ��л�ȡgps��Ϣ */
static LTE_RET_E thinkwill_wcdma_mi900_gpsinfo_get(char *acGps)
{
    FILE *pfData;
    char acTmp[128] = "";
    int iLoop = 0;
    int iFd = 0;
	int iRet = 0;
	int iNwrite = 0;
	struct flock lock;

    /*���ö���ռ��*/
	lock.l_type = F_RDLCK;  /* ������� */
	lock.l_start = 0;
	lock.l_whence = SEEK_SET;
	lock.l_len = 0;
    //return TRUE;
    iFd = open(LTEGPSINFO, O_RDWR);
	if(iFd < 0)
	{
		LTE_LOG(LTE_LOG_INFO, "FILE:%s FUN:%s LINE:%d open "LTEGPSINFO"fail\n",
            __FILE__, __func__, __LINE__);
		return LTE_FAIL;
	}

    /*�����ȴ�*/
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

    /*�����ļ����н���*/
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
static LTE_RET_E thinkwill_wcdma_mi900_gpsinfo_analysis(char *acGps, LTE_GPS_INFO_T *pstLteGpsInfo)
{
    char	acLongitude[32] = {0};	/*���ھ��� */
	char	acLatitude[32] = {0};		/* ����γ�� */
	char	acVelocity[32] = {0};		/* ������ʻ�ٶ�*/
	char	acOrientation[32] = {0};	/* ��λ�� */
	char	acSiteTime[64] = {0};		/*��λʱ��*/
    char    acGpsInfo[128] = {0},acTmpBuf[32]={0};
    int     ulGpsInfoLen,i = -1,j;
    char     acGpsPara[16][32] = {0};
    char    *szStart=NULL,*szEnd=NULL;
    float   fUnit = 1.852,fVelocity = 0;//һ��=һ����/Сʱ=1.852����/Сʱ
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
    /*ʱ��*/
    memcpy(pstLteGpsInfo->acTime, acGpsPara[AC_CW_GPS_DATE_TIME], strlen(acGpsPara[AC_CW_GPS_DATE_TIME]));
    memcpy(pstLteGpsInfo->acDate, acGpsPara[AC_CW_GPS_DATE],strlen(acGpsPara[AC_CW_GPS_DATE])); 
    /*γ��*/
    memcpy(pstLteGpsInfo->acLatitude,acGpsPara[AC_CW_GPS_LATITUDE_DIRECTION],strlen(acGpsPara[AC_CW_GPS_LATITUDE_DIRECTION]));
    memcpy((pstLteGpsInfo->acLatitude)+(strlen(acGpsPara[AC_CW_GPS_LATITUDE_DIRECTION])),
                acGpsPara[AC_CW_GPS_LATITUDE],strlen(acGpsPara[AC_CW_GPS_LATITUDE]));
    /*����*/
    memcpy(pstLteGpsInfo->acLongitude,acGpsPara[AC_CW_GPS_LONGITUDE_DIRECTION],strlen(acGpsPara[AC_CW_GPS_LONGITUDE_DIRECTION]));
    memcpy((pstLteGpsInfo->acLongitude)+(strlen(acGpsPara[AC_CW_GPS_LONGITUDE_DIRECTION])),
                acGpsPara[AC_CW_GPS_LONGITUDE],strlen(acGpsPara[AC_CW_GPS_LONGITUDE]));
    /*�����ٶȵ�λΪ����/Сʱ,(1��=1����/Сʱ=1.852����/Сʱ)*/
    if(0 != strlen(acGpsPara[AC_CW_GPS_BUS_VELOCITY]))
    {

           fVelocity = atof(acGpsPara[AC_CW_GPS_BUS_VELOCITY]);
           LTE_LOG(LTE_LOG_INFO, "ap_agent_analysis_gpsinfo before fVelocity:[%.3lf]\n", fVelocity);    
           fVelocity = fVelocity * fUnit;
           fVelocityTmp = fVelocity/3.6;/* ���䵥λ����Ϊm/s */
           if(fVelocityTmp > 100)
           {
                
                snprintf(pstLteGpsInfo->acVelocity, 32, "0.0");
                
           }
           else
           {                
               snprintf(pstLteGpsInfo->acVelocity, 32, "%.1f", fVelocityTmp);
           }
            
    }
    /*�����*/
    memcpy(pstLteGpsInfo->acOrientation, acGpsPara[AC_CW_GPS_ORIENTATION], strlen(acGpsPara[AC_CW_GPS_ORIENTATION]));

    return LTE_OK;
}
static LTE_RET_E thinkwill_wcdma_mi900_gps_date_get(void *pvData, int iDataLen)
{
    char   acGpsInfo[512] = {0};
    LTE_GPS_INFO_T stLteGpsInfo = {0};

    LTE_GPS_DATE_T *pstLteGpsDate = (LTE_GPS_DATE_T *)pvData;
    
    thinkwill_wcdma_mi900_gpsinfo_get(acGpsInfo);
    thinkwill_wcdma_mi900_gpsinfo_analysis(acGpsInfo, &stLteGpsInfo);
    sprintf(pstLteGpsDate->acDate, "%s-%s", stLteGpsInfo.acDate, stLteGpsInfo.acTime);
    
    return LTE_OK;
}
/*****************************************************************************
 �� �� ��  : thinkwill_cdma_mi660_gps_position_info_get
 ��������  : ��ȡgps��λ����Ϣ
 �������  : void *pvData  
             int iDataLen  
 �������  : ��
 �� �� ֵ  : static
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2014��9��2�����ڶ�
    ��    ��   : liyoukun
    �޸�����   : �����ɺ���

*****************************************************************************/
static LTE_RET_E thinkwill_wcdma_mi900_gps_pstinfo_get(void *pvData, int iDataLen)
{
    char   acGpsInfo[512] = {0};
    LTE_GPS_INFO_T stLteGpsInfo = {0};

    LTE_GPS_POSTION_T *pstLteGpsDate = (LTE_GPS_POSTION_T *)pvData;
    
    thinkwill_wcdma_mi900_gpsinfo_get(acGpsInfo);
    thinkwill_wcdma_mi900_gpsinfo_analysis(acGpsInfo, &stLteGpsInfo);
    
    strcpy(pstLteGpsDate->acLatitude, stLteGpsInfo.acLatitude);
    strcpy(pstLteGpsDate->acLongitude, stLteGpsInfo.acLongitude);
    strcpy(pstLteGpsDate->acVelocity, stLteGpsInfo.acVelocity);
    strcpy(pstLteGpsDate->acOrientation, stLteGpsInfo.acOrientation);
    
    return LTE_OK;
}

/*****************************************************************************
 �� �� ��  : thinkwill_sup_nettype_get
 ��������  : ��ȡ֧�ֵ��������ͣ���2g��3g��4g
 �������  : void *pvData  
             int iDataLen  
 �������  : ��
 �� �� ֵ  : static
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2014��11��17��
    ��    ��   : wujian
    �޸�����   : �����ɺ���

*****************************************************************************/
static LTE_RET_E thinkwill_sup_nettype_get(void *pvData, int iDataLen)
{
    LTE_NET_TYPE_T *pstNets = (LTE_NET_TYPE_T *)pvData;
    
    pstNets->enNetType = LTE_3G_ONLY;
    pstNets->enNetTypePri = LTE_NONE;
    return LTE_OK;

}
static LTE_RET_E thinkwill_sms_sup_func(void *pvData, int iDataLen)
{
    int flag = 0;
    LTE_SUPPORT_SMS_T *pstSup = (LTE_SUPPORT_SMS_T *)pvData;
    pstSup->enSupSms = NO_SUP_SMS_FUNC;
    return LTE_OK;

}

static LTE_RET_E thinkwill_version_get(void *pvData, int iDataLen)
{
    int iRet = 0;
    char acBuff[64] = {0};
    char acVersion[64] = {0};
    FILE* pstFd = NULL;
    
    LTE_MODULE_VERSION_T *pstVersion = (LTE_MODULE_VERSION_T *)pvData;

    pstVersion->iVersionIndex = LTE_THINKWILL_MI900;
    
    return LTE_OK;
}

LTE_IOCTL_FUNC_T astThinkWillMi900Table[] = 
{    
    {LTE_PIN_STATE,         NULL,           0,0,    NULL       }, 
    {LTE_PIN_SWITCH,        NULL,           0,0,    NULL       }, 
    {LTE_PIN_SW_GET,        NULL,           0,0,    NULL       },
    {LTE_PIN_UNLOCK,        NULL,           0,0,    NULL       }, 
    {LTE_PIN_ATTEMPTS,      NULL,           0,0,    NULL       }, 
    {LTE_PIN_ALTER,         NULL,           0,0,    NULL       }, 
    {LTE_SIGNAL_GET,        thinkwill_wcdma_mi900_signal_get,        0,0,    "thinkwill_wcdma_mi900_wcdma_mi900_signal_get"       }, 
    {LTE_POWER_ON,          thinkwill_wcdma_mi900_power_on,          0,0,    "thinkwill_wcdma_mi900_power_on"       }, 
    {LTE_POWER_OFF,         thinkwill_wcdma_mi900_power_off,         0,0,    "thinkwill_wcdma_mi900_power_off"       }, 
    {LTE_NETTYPE_SET,      NULL,           0,0,    NULL       }, 
    {LTE_NETTYPE_GET,       NULL,           0,0,    NULL       }, 
    {LTE_PLMN_REGISTER,     NULL,           0,0,    NULL       }, 
    {LTE_PREFER_PLMN_SET,   NULL,           0,0,    NULL       }, 
    {LTE_NETWORK_PREPARE,   thinkwill_wcdma_mi900_network_prepare,   0,0,    "thinkwill_wcdma_mi900_network_prepare"       }, 
    {LTE_NETWORK_STATUS_GET,thinkwill_wcdma_mi900_network_status_get,0,0,    "thinkwill_wcdma_mi900_network_status_get"       },
    {LTE_CONNECT,           thinkwill_wcdma_mi900_connect,           0,0,    "thinkwill_wcdma_mi900_connect"       }, 
    {LTE_DISCONNECT,        thinkwill_wcdma_mi900_disconnect,        0,0,    "thinkwill_wcdma_mi900_disconnect"       }, 
    {LTE_LINKSTATE_GET,     thinkwill_wcdma_mi900_linkstate_get,        0,0, "thinkwill_wcdma_mi900_linkstate_get"       }, 
    {LTE_IMEI_GET,          thinkwill_wcdma_mi900_imei_get,          0,0,    "thinkwill_wcdma_mi900_imei_get"       }, 
    {LTE_CELLID_GET,        NULL,           0,0,    NULL       }, 
    {LTE_ICCID_GET,         NULL,           0,0,    NULL       },
    {LTE_IMSI_GET,          thinkwill_wcdma_mi900_imsi_get,          0,0,    "thinkwill_wcdma_mi900_imsi_get"       },
    {LTE_SIM_STATE_GET,     thinkwill_wcdma_mi900_simstate_get,      0,0,    "thinkwill_wcdma_mi900_simstate_get"       }, 
    {LTE_INFO_GET,        NULL,           0,0,    NULL       }, 
    {LTE_ACCESS_NET_GET,        NULL,           0,0,    NULL       }, 
    {LTE_BANDS_GET,        NULL,           0,0,    NULL       }, 
    {LTE_CLCK_BANDS_SET,        NULL,           0,0,    NULL       }, 
    {LTE_CLCK_BANDS_CLOSE,  NULL,            0,0,       NULL       },
    {LTE_CLCK_CELL_GET,     NULL,                        0,0,    NULL   },
    {LTE_CLCK_CELL_SET,        NULL,           0,0,    NULL       },
    {LTE_OPER_INFO_GET,        NULL,           0,0,    NULL          }, 
    {LTE_NETTYPE_INFO_GET,     thinkwill_sup_nettype_get,           0,0,    "thinkwill_sup_nettype_get"          },
    {LTE_GPS_START,         thinkwill_wcdma_mi900_gps_start,           0,0,    "thinkwill_wcdma_mi900_gps_start"       }, 
    {LTE_GPS_STOP,          thinkwill_wcdma_mi900_gps_stop,           0,0,     "thinkwill_wcdma_mi900_gps_stop"       }, 
    {LTE_GPS_DATE_GET,      thinkwill_wcdma_mi900_gps_date_get,           0,0, "thinkwill_wcdma_mi900_gps_date_get"       }, 
    {LTE_GPS_POSITION_INFO_GET, thinkwill_wcdma_mi900_gps_pstinfo_get,           0,0,    "thinkwill_wcdma_mi900_gps_pstinfo_get"       }, 
    /* BEGIN: Added by wujian, 2014/11/13   PN:��Ӷ��Ź�����ؽӿ� */
    {LTE_SMS_CENT_CODE_SET, NULL, 0, 0, NULL },
    {LTE_SMS_MODE__SET, NULL, 0, 0, NULL   },   
    {LTE_SMS_FUNC_INIT, NULL, 0, 0, NULL   },
    {LTE_SMS_RECV, NULL, 0, 0, NULL   },
    {LTE_SMS_SEND, NULL, 0, 0, NULL   },
    {LTE_SMS_DELE, NULL, 0, 0, NULL   },
    {LTE_SMS_SUPP, thinkwill_sms_sup_func, 0, 0, "thinkwill_sms_sup_func"   },
    /* END:   Added by wujian, 2014/11/13   PN:��Ӷ��Ź�����ؽӿ� */
    {LTE_IPV6_PDP_SET, NULL, 0, 0, NULL   },
    /* BEGIN: Added by wujian, 2014/11/17   PN:�������apn�ӿ� */
    {LTE_APN_SET, NULL, 0, 0, NULL   },
    /* END:   Added by wujian, 2014/11/17   PN:�������apn�ӿ� */
    {LTE_VERSION_GET,       thinkwill_version_get,           0,0,    "thinkwill_version_get"       },
    {LTE_ENODEBID_GET,      NULL,     0,0,     NULL    }, 
    {LTE_TAC_GET,           NULL,     0,0,     NULL    },
    {LTE_LCID_GET,          NULL,     0,0,     NULL    },
    
    {LTE_CMD_MAX,           NULL,           0,0,    NULL       }, 
};

