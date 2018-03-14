#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/time.h>

#include "lte.h"
#include "libusb.h"
#include "lte_log.h"
#include "lte_common.h"
/* BEGIN: Added by liyoukun, 2014/9/24   PN:gpsģ�黯 */
GPS_DEVICE_INFO g_acGpsDevice[] = {
    {M_LONGCHEER, 0x1c9e, 0x9b05, 0, "", "longcheer"}, /*����ƽ̨*/
    {M_LEADCORE, 0x1ab7, 0x1761, 0, "", "leadcore"}, /* ��оƽ̨ */
    {M_THINKWILL, 0x19f5, 0x9013, B115200, "/dev/ttyUSB1", "thinkwill"}, /* �ſ�ƽ̨  */
    {M_ALTAIR, 0x216f, 0x0043, 0, "", "altair"}, /* ����ƽ̨ */
    {M_ZTE, 0x19d2, 0x0117, B115200, "/dev/ttyUSB1", "zte"}, /* ����ƽ̨ */
    {M_ZTE_CDMA2000, 0x19d2, 0xffed, 0, "", "zte-cdma2000"}, /* ����CDMA2000ƽ̨ */
    {M_ARCCRA, 0x29e0, 0x0303, 0, "", "arccra"}, /* Ǭ��ƽ̨ */
    {M_THINKWILL_CDMA_MI660, 0x19f5, 0x9909, B115200, "/dev/ttyUSB2", "thinkwill-mi660"}, /* �ſ�cdma mi660ƽ̨ */
    {M_YUGA_CDMA_CEM600, 0x257a, 0x1601, B115200, "/dev/ttyUSB2", "yuga-cem600"}, /* ���cdma cem600ƽ̨ ���GPS�ڱ䶯����ttyUSB3��ΪttyUSB2 */
    {M_ARCCRA_SECOND, 0x29e0, 0x0304, 0, "", "arccra"},
    {M_ZTE_MICRO, 0x19d2, 0x0199, 0, "", "zte-micro"},
    {M_HUAWEI, 0x12d1, 0x1c25, 0 ,"", "huawei"},
    {M_YUGA, 0x05c6, 0x9025, B115200, "/dev/ttyUSB3", "yuga"},/*yuga 4G*/
    {M_MARVELL, 0x1286, 0x4e31, 0, "", "marvell"},/*marvell 4G*/
};

static int g_iCloseGpsFlag = 0;/* GPS�ѹرյı�־ */
LTE_RET_E set_gps_close_tag(int iGpsCloseFlag)
{
    g_iCloseGpsFlag = iGpsCloseFlag;
    return LTE_OK;
}

LTE_RET_E get_gps_close_tag(int *piGpsCloseFlag)
{
    if(NULL == piGpsCloseFlag)
    {
        LTE_LOG(LTE_LOG_ERR, "param is NULL pointer!");
        return LTE_FAIL;
    }
    *piGpsCloseFlag = g_iCloseGpsFlag;
    return LTE_OK;
}

int find_gps_device(struct libusb_device_descriptor *pstDesc)
{
	int loop = 0;
	
	for (loop = 0; loop < M_TOTAL_NUM; loop++)
	{
		if(pstDesc->idVendor == g_acGpsDevice[loop].stDeviceDesc.usVendor && 
		   pstDesc->idProduct == g_acGpsDevice[loop].stDeviceDesc.usProduct){

		   	return g_acGpsDevice[loop].enDeviceNum;
		}
	}
	return M_TOTAL_NUM;
}
LTE_RET_E gps_device_info_get(GPS_DEVICE_INFO *pstGpsDevInfo)
{
    libusb_device **devs, *dev;
	struct libusb_device_descriptor desc;
	int ret, moduleNum, loop;
	ssize_t cnt;

    if(NULL == pstGpsDevInfo)
    {
        LTE_LOG(LTE_LOG_ERR, "param is NULL!");
        return LTE_FAIL;
    }
    
	ret = libusb_init(NULL);
	if (ret < 0)
		return LTE_FAIL;

	cnt = libusb_get_device_list(NULL, &devs);
	if (cnt < 0){
		libusb_exit(NULL);
		return LTE_FAIL;
	}

	loop = 0;
	while ((dev = devs[loop++]) != NULL) {
		
		ret = libusb_get_device_descriptor(dev, &desc);
		if (ret < 0) {
            
			LTE_LOG(LTE_LOG_ERR, "failed to get device descriptor");
            moduleNum = M_TOTAL_NUM;
            break;
		}
		moduleNum = find_gps_device(&desc);
		if (moduleNum < M_TOTAL_NUM)
			break;
	}
		
	libusb_free_device_list(devs, 1);
	libusb_exit(NULL);
    
    /* ��ȡlte�豸gps��ص���Ϣ */
    if(moduleNum < M_TOTAL_NUM)
    {
        memcpy(pstGpsDevInfo, &(g_acGpsDevice[moduleNum]), sizeof(GPS_DEVICE_INFO));
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR, "unkown lte module [%d], cannot get device info!", moduleNum);
        return LTE_FAIL;
    }
    
	return LTE_OK;
}
LTE_RET_E gps_com_attribute_set(int iComFd, int iDeviceSpeed)
{
    struct termios TtyAttr;
    int DeviceSpeed = B115200;
    int ByteBits = CS8;

    DeviceSpeed = iDeviceSpeed;
    
    memset(&TtyAttr, 0, sizeof(struct termios));
    TtyAttr.c_iflag = IGNPAR;
    TtyAttr.c_cflag = DeviceSpeed | HUPCL | ByteBits | CREAD | CLOCAL;
    TtyAttr.c_cc[VMIN] = 1;
    
    /* ��com������Ϊ������ģʽ */
    if (fcntl(iComFd, F_SETFL, O_NONBLOCK) < 0)
    {
        LTE_LOG(LTE_LOG_ERR, "gps com fcntl failed\n"); 
        return LTE_FAIL;
    }
    /* ����com�ڵĲ����ʵ����� */
    if (tcsetattr(iComFd, TCSANOW, &TtyAttr) < 0)
    {
        LTE_LOG(LTE_LOG_ERR, "gps com attr set failed\n");
        return LTE_FAIL;
    }
    return LTE_OK;
}
LTE_RET_E gps_serial_com_open(int *piComFd)
{
    GPS_DEVICE_INFO stGpsDevInfo = {0};
    char acCom[32] = {0};
    int iDeviceSpeed = B115200;
    int iRet = 0;
    int iComFd = 0; /* ��gps com�ں��Ӧ��fd */
    
    if(1 == g_iCloseGpsFlag)
    {
        LTE_LOG(LTE_LOG_ERR, "GPS com do not allowed use!\n");
        return LTE_FAIL;
    }

    if(NULL == piComFd)
    {
        LTE_LOG(LTE_LOG_ERR, "param [piComFd] is NULL\n");
        *piComFd = -1;
        return LTE_FAIL;
    }
    if(LTE_OK != gps_device_info_get(&stGpsDevInfo))
    {
        LTE_LOG(LTE_LOG_ERR, "gps_device_info_get get gps device failed!\n");
        *piComFd = -1;
        return LTE_FAIL;
    }
    
    iDeviceSpeed = stGpsDevInfo.iBaudRates;
    strcpy(acCom, stGpsDevInfo.acGpsCom);
    /* �򿪴�����Ҫ�������O_NOCTTY�������κ����붼��Ӱ����� */
    if ((iComFd = open(acCom, O_RDWR | O_NOCTTY)) < 0) 
    {
        LTE_LOG(LTE_LOG_ERR, "open gps com [%s] failed\n", acCom);
        *piComFd = -1;
        return LTE_FAIL;
    }

    /*����gps COM�ڵ�����*/
    if (LTE_FAIL == gps_com_attribute_set(iComFd, iDeviceSpeed))
    {
        *piComFd = -1;
        close(iComFd);
        return LTE_FAIL;
    }

    *piComFd = iComFd;

    return LTE_OK;
}
LTE_RET_E gps_serial_com_close(int iComFd)
{
    if(iComFd < 0)
    {
        LTE_LOG(LTE_LOG_ERR, "gps iComFd do not close!\n");
    }
    else
    {
        close(iComFd);
    }
    return 0;
}
//LTE_RET_E gps_data_read(GPS_BASE_CONGFIG_T *pstGpsConfgT, char *pcBuf, char *pcNeedle)
LTE_RET_E gps_data_read(GPS_BASE_CONGFIG_T *pstGpsConfgT, char *pcBuf, int iBuf_len)
{
    struct timeval stTimeout;
    fd_set ReadSetFD;
    char acBuf[128] = {0};
    unsigned char cChar = 0;

    int iComFd = 0;
    int iRet = 0;
    int iLoop = 0;
    int iTimeOut = 0;
    
    iComFd = pstGpsConfgT->iComFd;
    iTimeOut = pstGpsConfgT->iTimeOut;
    /*�����ĵȴ�*/
    stTimeout.tv_sec = iTimeOut;
    stTimeout.tv_usec=0L;//1s=1000ms=1000*1000us
    LTE_LOG(LTE_LOG_INFO, "gps_data_read start get gps data!");

    FD_ZERO(&ReadSetFD);
    FD_SET(iComFd, &ReadSetFD);
    if (select(iComFd + 1, &ReadSetFD, NULL, NULL, &stTimeout) == 0)
    {
        return LTE_FAIL;
    }
    if (FD_ISSET(iComFd, &ReadSetFD)) 
    {
        while (1) 
        {
            iRet = read(iComFd, &cChar, 1);
            if ( (iRet < 0) || (iRet ==  0) )
            {
                FD_CLR(iComFd,&ReadSetFD);
                return LTE_FAIL;
            }
            acBuf[iLoop++] = cChar;
            if('\n' == cChar)
            {
                iLoop = 0;
                FD_CLR(iComFd,&ReadSetFD);
                break;
                #if 0
                if(NULL != strstr(acBuf, pcNeedle))
                {
                    break;
                }
                else
                {
                    memset(acBuf, 0, sizeof(acBuf));
                    continue;
                }
                #endif
            }
        }
    }
    strncpy(pcBuf, acBuf, iBuf_len);
    memset(acBuf, 0, sizeof(acBuf));
    LTE_LOG(LTE_LOG_INFO, "lte gps date [%s]!", pcBuf);
    return LTE_OK;
}
static LTE_RET_E gps_gpgga_data_analysis(char *acGps, char *pcElevation)
{
    char acTmp[128] = "";
    char  *szStart=NULL,*szEnd=NULL;
    int iLoop = 0;
    int iFd = 0;
	int iRet = 0;
	int iNwrite = 0;
    
    if((NULL == acGps)||(NULL == pcElevation))
    {
        LTE_LOG(LTE_LOG_ERR, "param pointer is NULL\n");  
        return LTE_FAIL;
    }
    /* �ж�gpgga�����Ƿ���Ч */
    if(NULL == strstr(acGps, "M"))
    {
        LTE_LOG(LTE_LOG_WARNING, "gps_gpgga_data_analysis gps data is invalid\n");
        //return LTE_FAIL;
    }
    memcpy(acTmp, acGps, 128);
    szStart = acTmp;
    szEnd = acTmp ;
    

    LTE_LOG(LTE_LOG_INFO, "GPS gpgga date :%s ", acTmp);
    while(NULL != szEnd)
    {
        
        if( GPS_DECODER_PARA_NUM <= iLoop )
        {
              LTE_LOG(LTE_LOG_ERR, "gps_gpgga_data_analysis, only decoder 16 parameter\n");    
              return LTE_FAIL;
        }
        /* ��ȡ�ļ��еĵ�9���ֶ� */
        if(NULL == szStart)
        {
            LTE_LOG(LTE_LOG_ERR, "gps_gpgga_data_analysis, szStart is NULL, there is no parameter!\n");
            return LTE_FAIL;
        }
        szEnd = strstr(szStart,",");
        if((9 == iLoop)&&(szEnd != NULL))
        {
            memcpy(pcElevation, szStart, (szEnd-szStart));
            szStart = szEnd = NULL;
            break;
        }
        else
        {
            //szEnd =  szEnd + 1;
            szStart = szEnd + 1;
            iLoop++;
        }
    }
    
    return LTE_OK;
}
static LTE_RET_E gps_gprmc_data_analysis(char *acGps, LTE_GPS_INFO_T *pstLteGpsInfo)
{
    char    acGpsInfo[128] = {0},acTmpBuf[32]={0};
    int     ulGpsInfoLen,i = -1,j;
    char     acGpsPara[16][32] = {0};
    char    *szStart=NULL,*szEnd=NULL;
    float   fUnit = 1.852,fVelocity = 0;//һ��=һ����/Сʱ=1.852����/Сʱ
    float   fVelocityTmp = 0;
    int     iRet = 0; 
    
    if ((NULL == acGps) || (NULL == pstLteGpsInfo))
    {
        LTE_LOG(LTE_LOG_INFO, "gps_gprmc_data_analysis input param is NULL\n");
        return LTE_FAIL;
    }
    /* �ж�gprmc�����Ƿ���Ч */
    if(NULL == strstr(acGps, "A"))
    {
        LTE_LOG(LTE_LOG_WARNING, "gps_gprmc_data_analysis gps data is invalid\n");
        //return LTE_FAIL;
    }
    memcpy(acGpsInfo, acGps, sizeof(acGpsInfo));
    ulGpsInfoLen = strlen(acGpsInfo);

    LTE_LOG(LTE_LOG_INFO, "gps_gprmc_data_analysis GpsInfo:[%s]\n", acGpsInfo);    
	    
    szStart = szEnd = acGpsInfo;

    while(szEnd!=NULL )
    {
        if( GPS_DECODER_PARA_NUM <= i )
        {
            LTE_LOG(LTE_LOG_INFO, "gps_gprmc_data_analysis, only decoder 16 parameter\n");    
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
           LTE_LOG(LTE_LOG_INFO, "gps_gprmc_data_analysis before fVelocity:[%.3lf]\n", fVelocity);    
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

LTE_RET_E gps_data_get_and_analysis(GPS_BASE_CONGFIG_T *pstGpsConfgT, LTE_GPS_INFO_T *pstLteGpsInfo)
{

    char *pcNeedle1 = "$GPRMC"; 
    char *pcNeedle2 = "$GPGGA";
    char *pcNeedle3 = "$";
    int  iGpggaFlag = 0;
    int  iGprmcFlag = 0;
    int  iCount = 0;
    char acBuf[128] = {0};
    char acElevation[128] = {0};
    
    if((NULL == pstGpsConfgT)||(NULL == pstLteGpsInfo))
    {
        LTE_LOG(LTE_LOG_ERR, "param is NULL pointer\n");
        return LTE_FAIL;
    }
    /* gps������ж����� */
    if(1 == pstGpsConfgT->iTest)
    {
        LTE_LOG(LTE_LOG_INFO, "gps test use\n");
        if((LTE_OK != gps_data_read(pstGpsConfgT, acBuf, sizeof(acBuf))) || (strlen(acBuf) < 16))
        {
            return LTE_FAIL;
        }
        else
        {
            if(NULL != strstr(acBuf, pcNeedle3))
            {
                return LTE_OK;
            }
            else
            {
                return LTE_FAIL;
            }
        }
        //return gps_data_read(pstGpsConfgT, acBuf, pcNeedle3);
    }
    else
    {        
        while(1)
        {
            LTE_LOG(LTE_LOG_INFO, "gps_data_get_and_analysis:gps date get!\n");
            memset(acBuf, 0, sizeof(acBuf));
            if((LTE_OK != gps_data_read(pstGpsConfgT, acBuf, sizeof(acBuf))) || (strlen(acBuf) < 16))
            {
                pstLteGpsInfo->iGpsDataIsValid = 0;
                return LTE_FAIL;
            }
            else
            {
                if(NULL != strstr(acBuf, pcNeedle2))
                {
                    LTE_LOG(LTE_LOG_INFO, "gps date:[%s]\n", acBuf);
                    if(LTE_OK != gps_gpgga_data_analysis(acBuf, acElevation))
                    {
                        pstLteGpsInfo->iGpsDataIsValid = 0;
                        return LTE_FAIL;
                    }
                    memcpy(pstLteGpsInfo->acElevation, acElevation, sizeof(pstLteGpsInfo->acElevation));
                    memset(acElevation, 0, sizeof(acElevation));
                    iGpggaFlag = 1;
                }
                else if(NULL != strstr(acBuf, pcNeedle1))
                {
                    LTE_LOG(LTE_LOG_INFO, "gps date:[%s]\n", acBuf);
                    if(LTE_OK != gps_gprmc_data_analysis(acBuf, pstLteGpsInfo))
                    {
                        pstLteGpsInfo->iGpsDataIsValid = 0;
                        return LTE_FAIL;
                    }
                    iGprmcFlag = 1;
                }
            }
            /* �ж�gpgga���ݺ�gprmc�����Ƿ񶼶��� */
            if((1 == iGprmcFlag)&&(1 == iGpggaFlag))
            {
                iGprmcFlag = 0;
                iGpggaFlag = 0;
                break;
            }
            else if(iCount > 8)//GPS����֡һ�鹲8��
            {
                iCount = 0;
                pstLteGpsInfo->iGpsDataIsValid = 0;
            }
            iCount++;
        }
        
        LTE_LOG(LTE_LOG_INFO, "gps date:[%s %s %s %s %s %s %s]\n", pstLteGpsInfo->acDate, 
                                                                   pstLteGpsInfo->acTime, 
                                                                   pstLteGpsInfo->acLatitude, 
                                                                   pstLteGpsInfo->acLongitude, 
                                                                   pstLteGpsInfo->acOrientation, 
                                                                   pstLteGpsInfo->acVelocity, 
                                                                   pstLteGpsInfo->acElevation);
        pstLteGpsInfo->iGpsDataIsValid = 1;
        #if 0
        /* ��ȡgpgga���ݲ����� */
        memset(acBuf, 0, sizeof(acBuf));
        
        if(LTE_OK != gps_data_read(pstGpsConfgT, acBuf, pcNeedle2))
        {
            pstLteGpsInfo->iGpsDataIsValid = 0;
            return LTE_FAIL;
        }
        LTE_LOG(LTE_LOG_INFO, "gps date:[%s]\n", acBuf);
        if(LTE_OK != gps_gpgga_data_analysis(acBuf, acElevation))
        {
            pstLteGpsInfo->iGpsDataIsValid = 0;
            return LTE_FAIL;
        }
        memcpy(pstLteGpsInfo->acElevation, acElevation, sizeof(pstLteGpsInfo->acElevation));
        memset(acElevation, 0, sizeof(acElevation));
        
        /* ��ȡgprmc���ݲ����� */
        if(LTE_OK != gps_data_read(pstGpsConfgT, acBuf, pcNeedle1))
        {
            pstLteGpsInfo->iGpsDataIsValid = 0;
            return LTE_FAIL;
        }
        LTE_LOG(LTE_LOG_INFO, "gps date:[%s]\n", acBuf);
        if(LTE_OK != gps_gprmc_data_analysis(acBuf, pstLteGpsInfo))
        {
            pstLteGpsInfo->iGpsDataIsValid = 0;
            return LTE_FAIL;
        }
        
        LTE_LOG(LTE_LOG_INFO, "gps date:[%s %s %s %s %s %s %s]\n", pstLteGpsInfo->acDate, 
                                                                   pstLteGpsInfo->acTime, 
                                                                   pstLteGpsInfo->acLatitude, 
                                                                   pstLteGpsInfo->acLongitude, 
                                                                   pstLteGpsInfo->acOrientation, 
                                                                   pstLteGpsInfo->acVelocity, 
                                                                   pstLteGpsInfo->acElevation);
        pstLteGpsInfo->iGpsDataIsValid = 1;
        #endif
        return LTE_OK;
    }
    
    return LTE_OK;
}
/* END:   Added by liyoukun, 2014/9/24   PN:gpsģ�黯 */
/* BEGIN: Added by liyoukun, 2014/9/22   PN:���gps���Խӿ� */
LTE_RET_E lte_gps_test(void)
{
    int iRet = 0;
    int iComFd = 0;
    int iTestResult = 0;
    GPS_BASE_CONGFIG_T stGpsConfgT = {0};
    LTE_GPS_INFO_T stLteGpsInfo = {0};
    char acAtCmd[16] = "at\r";
    int iCount;
    
    iRet = lte_gps_func_start();
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_gps_func_start failed!");
        return LTE_FAIL;
    }
    if(LTE_OK == gps_serial_com_open(&iComFd))
    {
        /*altered by caibing */
        if (strlen(acAtCmd) != write(iComFd, acAtCmd, strlen(acAtCmd)))
        {
            LTE_LOG(LTE_LOG_ERR, "%d,%s:write com failed\n",__LINE__,__func__);
        }
        /*altered by caibing */
        stGpsConfgT.iComFd = iComFd;
        stGpsConfgT.iTest = 1;
        stGpsConfgT.iTimeOut = 5; /* ��ʱʱ�䵥λΪ�� */
        for(iCount=0; iCount<5; iCount++)
        {
            iTestResult = gps_data_get_and_analysis(&stGpsConfgT, &stLteGpsInfo);
            if(LTE_OK == iTestResult)
            {
                break;
            }
        }
        /*
        iTestResult = gps_data_get_and_analysis(&stGpsConfgT, &stLteGpsInfo);
        {
            iTestResult = LTE_OK;
            iTestResult = gps_data_get_and_analysis(&stGpsConfgT, &stLteGpsInfo);
        }*/
    }
    gps_serial_com_close(iComFd);

    iRet = lte_gps_func_stop();
    if(LTE_OK != iRet)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_gps_func_stop failed!");
        return LTE_FAIL;
    }
    if(LTE_OK != iTestResult)
    {
        return LTE_FAIL;
    }
    return LTE_OK;
}
/* END:   Added by liyoukun, 2014/9/22   PN:���gps���Խӿ� */
/* BEGIN: Added by liyoukun, 2014/9/25   PN:gpsģ�黯 */
LTE_RET_E gps_valid_data_get(int iComFd, int iTimeOut, LTE_GPS_INFO_T *pstLteGpsInfo)
{
    GPS_BASE_CONGFIG_T stGpsConfgT = {0};
    int iRet = 0;
    
    stGpsConfgT.iComFd = iComFd;
    stGpsConfgT.iTimeOut = iTimeOut;/* ��ʱʱ�䵥λΪ�� */
    stGpsConfgT.iTest = 0;

    if(1 == g_iCloseGpsFlag)
    {
        LTE_LOG(LTE_LOG_ERR, "GPS com do not allowed use!\n");
        
        return LTE_FAIL;
    }

    if(NULL == pstLteGpsInfo)
    {
        LTE_LOG(LTE_LOG_ERR, "param is NULL pointer\n");
        return LTE_FAIL;
    }
    
    if(LTE_OK != gps_data_get_and_analysis(&stGpsConfgT, pstLteGpsInfo))
    {
        return LTE_FAIL;
    }
    else
    {
        return LTE_OK;
    }
}
/* END:   Added by liyoukun, 2014/9/25   PN:gpsģ�黯 */

