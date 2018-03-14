/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : bsp_lte_ioctl.c
  �� �� ��   : ����
  ��    ��   :  �̱�
  ��������   : 2012��7��23�� ���ڶ�
  ����޸�   :
  ��������   : bsp  lte ioctl
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��7��23�� ���ڶ�
    ��    ��   : �̱�
    �޸�����   : �����ļ�

******************************************************************************/
/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
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

#include "lte_com.h"
#include "lte_log.h"


#define ATCOM  "/dev/ttyUSB0"
#define PIDFILE "/tmp/pidtimer"
#define BUFMAX 16
#define ARCCRA_BAND_3             3  
#define ARCCRA_BAND_3_VALUE       "4"  /* band3��Ӧ����Ƶֵ */
#define ARCCRA_BAND_38            38
#define ARCCRA_BAND_38_VALUE      "137438953472"  /* band 38��Ӧ����Ƶֵ */
#define ARCCRA_BAND_39_VALUE      "274877906944"  /* band 39��Ӧ����Ƶֵ */
#define ARCCRA_BAND_40_VALUE      "549755813888"  /* band 40��Ӧ����Ƶֵ */
#define ARCCRA_BAND_41_VALUE      "1099511627776"  /* band 41��Ӧ����Ƶֵ */

#define ARCCRA_BAND_DEFAULT       "default"
//#define ARCCRA_BAND_DEFAULT_VALUE "1786706919637"  /* Ĭ��֧�ֵ�����:1,3,5,7,8,20,38,40,41 */
//#define ARCCRA_BAND_DEFAULT_VALUE "2061584826581"  /* Ĭ��֧�ֵ�����:1,3,5,7,8,20,38,39,40,41 */
#define ARCCRA_BAND_DEFAULT_VALUE "2061584302084"  /* Ĭ��֧�ֵ�����:3,38,39,40,41 */
#define ARCCRA_BAND_DEFAULT_VALUE_SECOND "2061584302080"  /* Ĭ��֧�ֵ�����:38,39,40,41 */

#define ARCCRA_2_M_2_BAND_VEVISION    "MD8500_QR"  /* 2ģ2Ƶ�汾�� 3,38 */
#define ARCCRA_2_M_4_BAND_VEVISION    "MD8501A_HC" /* 2ģ4Ƶ�汾�� 3,38,39,40,41*/
#define ARCCRA_4_M_4_BAND_VEVISION    "MD8501B_HC" /* 4ģ4Ƶ�汾�� 3,38,39,40,41*/
#define ARCCRA_1_M_4_BAND_VEVISION    "MD8501AV2"  /* 1ģ4Ƶ�汾�� 38,39,40,41*/
#define ARCCRA_1_M_4_VEVISION         "MD8501AV2" /* 1ģ4Ƶ�汾�� 38,39,40,41*/
#define ARCCRA_SPE_IMEI               "864242020000769"   /* ����imei�ţ���Ϊ��ģģ�飬�������ڵ�ģģ�鵱����ģģ�鷢�� */
/* BEGIN: Added by wujian, 2014/11/25   PN:��ȡ����������غ궨�� */
#define ARCCRA_GSM_ONLY          1
#define ARCCRA_LTE_ONLY          10
#define ARCCRA_TDS_ONLY          20
#define ARCCRA_WCD_ONLY          2
#define ARCCRA_GSM_TDS           21
#define ARCCRA_GSM_WCD           3
#define ARCCRA_TDS_LTE           30
#define ARCCRA_WCD_LTE           12
#define ARCCRA_GSM_TDS_LTE       31
#define ARCCRA_GSM_WCD_LTE       13



#define ARCCRA_NET_GSM 1
#define ARCCRA_NET_WCDMA 2
#define ARCCRA_NET_CDMA 3
#define ARCCRA_NET_EVDO 4
#define ARCCRA_NET_LTE 5
#define ARCCRA_NET_TDSCDMA 6
/* END:   Added by wujian, 2014/11/25   PN:��ȡ����������غ궨�� */

static int g_connect_flag=0;
static int g_connect_fail=0;


/*****************************************************************************
 �� �� ��  : pal_bsp_lte_pin_state
 ��������  : ��ȡPIN��״̬
 �������  : void *pvData    
             int iDataLen  
 �������  : ��
 �� �� ֵ  : LTE_RET_E
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��11��5��
    ��    ��   : �¼�
    �޸�����   : �����ɺ���

*****************************************************************************/
static LTE_RET_E arccra_pin_state (void *pvData, char *pcBuf)
{

    int iRet = 0; 
    char *pcBufTmp = NULL;
    char *pcPinBuf = NULL;
    char acCiev[] = "+CPIN:";
    char acStateReady[] = "READY";
    char acStatePin[] = "PIN";
    char acStatePuk[] = "PUK";
    
    LTE_PIN_STATE_T *pstPinState = (LTE_PIN_STATE_T*)pvData;

    if (NULL == pvData)
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
        /*PIN ������ ��״̬*/
        pstPinState->enState = PIN_STATE_READY;
    }
    else if(NULL != strstr(pcPinBuf,acStatePuk))
    {
        /*PIN ������ puk ״̬*/
        pstPinState->enState = PIN_STATE_PUK;
    }
    else if(NULL != strstr(pcPinBuf,acStatePin))
    {
        /*PIN ������ �ر�״̬*/
        pstPinState->enState = PIN_STATE_PIN;
    }
    else
    {
        return LTE_FAIL;
    }


    return LTE_OK;
}

/*****************************************************************************
 �� �� ��  : pal_bsp_lte_pin_switch
 ��������  : �����͹ر�pin�빦��
 �������  : void *pvData    
             int iDataLen  
 �������  : ��
 �� �� ֵ  : LTE_RET_E
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��6��������
    ��    ��   : �¼�
    �޸�����   : �����ɺ���

*****************************************************************************/
static LTE_RET_E arccra_pin_switch (void *pvData, char *pcBuf)
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
/*****************************************************************************
 �� �� ��  : arccra_pin_sw_get
 ��������  : ��ȡpin�Ƿ�����1������0�ر�
 �������  : void *pvData  
             int iDataLen  
 �������  : ��
 �� �� ֵ  : static
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2014��11��6��������
    ��    ��   : liyoukun
    �޸�����   : �����ɺ���

*****************************************************************************/
static LTE_RET_E arccra_pin_sw_get( void *pvData, char *pcBuf )
{
    int iRet = 0; 
    int iATExeCount = 0;
    int iFlag = 0;
    char *pcTmp = NULL;    
    char acBuffTmp[32] = {0};
    char acCiev[32] = "+CLCK:";
    FILE* pstFd = NULL;
    
    LTE_PIN_SW_STATE_T *pstPinSwState = (LTE_PIN_SW_STATE_T*)pvData;

    if (NULL == pvData)
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
    LTE_LOG(LTE_LOG_ERR,"arccra_pin_sw_get state:%d buf:%s \n", pstPinSwState->enState, pcBuf);


    return LTE_FAIL;

}
 
/*****************************************************************************
 �� �� ��  : pal_bsp_lte_pin_save
 ��������  : ����pin�뵽NVRAM
 �������  : void *pvData    
             int iDataLen  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��6��������
    ��    ��   : �¼�
    �޸�����   : �����ɺ���

*****************************************************************************/
 static LTE_RET_E arccra_pin_unlock (void *pvData, char *pcBuf)
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

/*****************************************************************************
 �� �� ��  : pal_bsp_lte_pin_numremain
 ��������  : ��ȡʣ��PIN�볢�Դ���
 �������  : void *pvData    
             int iDataLen  
 �������  : ��
 �� �� ֵ  : LTE_RET_E
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��11��2��
    ��    ��   : �¼�
    �޸�����   : �����ɺ���

*****************************************************************************/
static LTE_RET_E arccra_pin_attempts_get(void *pvData, int iDataLen)
{
    return LTE_OK;
}


/*****************************************************************************
 �� �� ��  : pal_bsp_lte_pin_alter
 ��������  : �޸�pin��
 �������  : void *pvData    
             int iDataLen  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��6��������
    ��    ��   : �¼�
    �޸�����   : �����ɺ���

*****************************************************************************/
static LTE_RET_E arccra_pin_alter (void *pvData, char *pcBuf)
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
/*****************************************************************************
 �� �� ��  : lte_power_on
 ��������  : ���� lte ģ��
 �������  : iComFd �ļ�������,�����˻�����Ҫϵͳ������
 �������  : 
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��14 ������
    ��    ��   : �̱�
    �޸�����   : �����ɺ���

*****************************************************************************/
static LTE_RET_E arccra_power_on (void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    int iFd = 0;
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte module on success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }
}

/*****************************************************************************
 �� �� ��  : lte_power_off
 ��������  : ���� lte ģ��
 �������  : iComFd �ļ�������
 �������  : 
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��14 ������
    ��    ��   : �̱�
    �޸�����   : �����ɺ���

*****************************************************************************/
static LTE_RET_E arccra_power_off (void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";

    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte module off success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte module off failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;

}


/*****************************************************************************
 �� �� ��  : pal_bsp_lte_signal_quality
 ��������  : ��ȡ��ƽ�ź�ֵ
 �������  : void *pvData    
             int iDataLen  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��11��23��
    ��    ��   : �¼�
    �޸�����   : �����ɺ���

*****************************************************************************/
static LTE_RET_E arccra_signal_get  (void *pvData, char *pcBuf)
{
    int iRet = 0;
    char acCiev[] = "+CIND:";
    char acNeedleComma[] = ",";
    char *pcBufTmp = NULL;
    char *pcTmp = NULL;

    LTE_SIGNAL_T *pstSignal = (LTE_SIGNAL_T*)pvData;

    if(NULL == pvData)
    {
        return LTE_FAIL;
    }  
    pcBufTmp = strstr(pcBuf,acCiev); 
    if(pcBufTmp != NULL)
    {
        pcTmp = strtok(pcBufTmp,acNeedleComma);
        if(pcTmp)
        {
            pcTmp = strtok(NULL,acNeedleComma);
            if(pcTmp)
            {
                ;
            }
            else
            {
                return LTE_FAIL;
            }
        }
        else
        {
            return LTE_FAIL;
        }
        
        pstSignal->iSignal = pcTmp[0]-48;

    }
    else
    {
        return LTE_FAIL;
    }

    return LTE_OK;

}

static LTE_RET_E arccra_power_get(void *pvData, char *pcBuf)
{
    return LTE_OK;
}


/*altered by caibing 2013-12-17*/
/*****************************************************************************
 �� �� ��  : arccra_nettype_set
 ��������  : �����������ͼ����ȼ�
 �������  : void *pvData  
             int iDataLen  
 �������  : ��
 �� �� ֵ  : static
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2014��11��25��
    ��    ��   : wujian
    �޸�����   : �����ɺ���

*****************************************************************************/
static LTE_RET_E arccra_nettype_set (void *pvData, char *pcBuf)
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

/*****************************************************************************
 �� �� ��  : arccra_nettype_get
 ��������  : ��ȡģ�鵱ǰ���õĵ���������
 �������  : void *pvData  
             int iDataLen  
 �������  : ��
 �� �� ֵ  : static
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2014��11��25��
    ��    ��   : wujian
    �޸�����   : �����ɺ���

*****************************************************************************/
static LTE_RET_E arccra_nettype_get(void *pvData, char *pcBuf)
{
    int iRet = 0;
    int  iFlag = 0;
    char acNeedleComma[] = ":";
    char *pcTmp = NULL;
    char acTmp[4] = {0};
    int iNettype = 0;

    if (pvData == NULL)
    {
		return LTE_FAIL;
    }

    LTE_NET_TYPE_T *pstNetType = (LTE_NET_TYPE_T *)pvData;

    if(NULL == (pcTmp = strstr(pcBuf, "+CTEC:")))
    {
        return LTE_FAIL;
    }

    sscanf(pcTmp, "%*[^,],%s", acTmp);
    iNettype = atoi(acTmp);
    switch(iNettype)
    {
        case ARCCRA_GSM_ONLY:
        {
            pstNetType->enNetType = LTE_2G_ONLY;
            
            break;
        }
        
        case ARCCRA_LTE_ONLY: 
        {
            pstNetType->enNetType = LTE_4G_ONLY;
            break;
        }
        case ARCCRA_TDS_ONLY:
        case ARCCRA_WCD_ONLY:
        {
            pstNetType->enNetType = LTE_3G_ONLY;
            break;

        }
        case ARCCRA_GSM_TDS:
        case ARCCRA_GSM_WCD:
        {
            pstNetType->enNetType = LTE_2G_3G;
            break;

        }
        case ARCCRA_TDS_LTE:
        case ARCCRA_WCD_LTE:
        {
            pstNetType->enNetType = LTE_3G_4G;
            //pstNetType->enNetTypePri = LTE_NONE;
            break;

        }   
        case ARCCRA_GSM_TDS_LTE:
        case ARCCRA_GSM_WCD_LTE:
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


static LTE_RET_E arccra_imei_get(void *pvData, char *pcBuf)
{
    LTE_IMEI_T *pstImei = (LTE_IMEI_T*)pvData;


    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    


    /*ִ��atָ��*/
    if(NULL != pcBuf)
    {
        sscanf(pcBuf, "%s", pstImei->acImei);
    }
    else
    {
        return LTE_FAIL;
    }
    
    LTE_LOG(LTE_LOG_INFO,"%d,%s: imei: %s",__LINE__,__func__, pstImei->acImei);
    
    return LTE_OK;

}
static LTE_RET_E arccra_cellid_get(void *pvData, char *pcBuf)
{
     char acCellid[32] = {0};
     char *pcTmp = NULL;
     char *pcCid = acCellid;
     if (NULL == pvData)
     {
         return LTE_FAIL;
     }
     
     LTE_CELLID_T *pstCellId = (LTE_CELLID_T*)pvData;

    if(NULL != strstr(pcBuf, "OK"))
    {
        return LTE_OK;
    }
    else if(NULL != (pcTmp = strstr(pcBuf, "+CEREG:")))
    {
        sscanf(pcTmp,"%*[^\"]\"%*[^\"]\"%*[^\"]\"%*[^\"]\"%*[^\"]\"%[^\"]", acCellid);
        
        LTE_LOG(LTE_LOG_INFO, "%d,%s:cellid [%s]\n",__LINE__,__func__, acCellid);
        if(*pcCid == '0')
            pcCid++;
        strncpy(pstCellId->acCellID, pcCid, sizeof(pstCellId->acCellID));

        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }
    

}
static LTE_RET_E arccra_iccid_get(void *pvData, char *pcBuf)
{
    /*altered by caibing 2013-11-25*/
    char acCiev[32] = {"+CRSM:"};
    char acIccid[32] = {0};
    char acBuffTmp[64] = {0};
    char *pcTmp = NULL;
    int iLoop = 0;
    
    LTE_ICCID_T *pstIccid = (LTE_ICCID_T*)pvData;

    if (NULL == pvData)
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:input param is NULL pointer\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    if(NULL != (pcTmp = strstr(pcBuf, acCiev)))
    {
        sscanf(pcTmp, "%[^\"]\"%[^\"]", acBuffTmp, acIccid);
        LTE_LOG(LTE_LOG_ERR,"%d,%s:input param is NULL pointer!!acIccid %s\n",__LINE__,__func__, acIccid);
        for(iLoop = 0; iLoop < strlen(acIccid); ++iLoop)
        {
            pstIccid->acICCID[iLoop] = acIccid[iLoop + 1];
            pstIccid->acICCID[iLoop + 1] = acIccid[iLoop];
            ++iLoop;
        }
        LTE_LOG(LTE_LOG_INFO, "%d,%s:iccid [%s]\n",__LINE__,__func__, pstIccid->acICCID);
    }
    else
    {
        return LTE_FAIL;
    }
    
    return LTE_OK;
}

static LTE_RET_E arccra_imsi_get(void *pvData, char *pcBuf)
{
    char acBuffTmp[64];
        
    LTE_IMSI_T *pstImsiInfo = (LTE_IMSI_T *)pvData;

    if (pvData == NULL)
    {
		return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO,"%d,%s: sim imsi: %s",__LINE__,__func__, pcBuf);
    if(NULL != pcBuf)
    {
        sscanf(pcBuf, "%s", pstImsiInfo->acIMSI);
    }
    else
    {
        return LTE_FAIL;
    }
  
    LTE_LOG(LTE_LOG_INFO,"%d,%s: sim imsi: %s",__LINE__,__func__, pstImsiInfo->acIMSI);
    return LTE_OK;
}

/*****************************************************************************
 �� �� ��  : pal_bsp_register_net_carrier
 ��������  : �������ã�������״̬���Ľ��뼼��
 �������  :  
 �������  : 0,auto,1 lte ���룬2 td-s ����
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��17 ������
    ��    ��   : �̱�
    �޸�����   : �����ɺ���
    XXXXXXXXXXXX��Ҫ�ص��ע

*****************************************************************************/
static LTE_RET_E arccra_plmn_register(void *pvData, int iDataLen)
{
     return LTE_OK;
}


/*****************************************************************************
 �� �� ��  : set_prefer_plmn
 ��������  : ����plmn ��td-lte,fdd-lte���ȣ���ʱģ�鲻֧�֣�Ԥ���ӿ�
 �������  :  
 �������  : 
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��17 ������
    ��    ��   : �̱�
    �޸�����   : �����ɺ���

*****************************************************************************/

static LTE_RET_E arccra_prefer_plmn_set (void *pvData, int iDataLen)
{
    return LTE_OK;
}

/*****************************************************************************
 �� �� ��  : pal_bsp_lte_linkstate_get
 ��������  : �ж�lte �Ƿ�ע��������
 �������  :  
 �������  : 
 �� �� ֵ  :  0,δע�ᣬ1 ע��,�����ľ����� 1 ��������������Ĵ���1�������� pal_bsp
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012 ��8��20 ����һ
    ��    ��   : �̱�
    �޸�����   : �����ɺ���

*****************************************************************************/
static LTE_RET_E arccra_linkstate_get(void *pvData, char *pcBuf)
{
    return LTE_OK;
}

/*****************************************************************************
 �� �� ��  : pal_bsp_lte_connect
 ��������  : ��������
 �������  : void *pvData    
             int iDataLen  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��10��25��������
    ��    ��   : �¼�
    �޸�����   : �����ɺ���

*****************************************************************************/

#define DISCONNECT_TIME_OUT  5    //500ms*5
#define AUTH_TYPE 0


static LTE_RET_E arccra_network_prepare(void *pvData, char *pcBuf)
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

//altered by caibing 2013-11-25
static LTE_RET_E arccra_connect(void *pvData, char *pcBuf)
{
    return LTE_OK;
}

/*****************************************************************************
 �� �� ��  : pal_bsp_lte_disconnect
 ��������  : �Ͽ���������
 �������  : void *pvData    
             int iDataLen  
 �������  : ��
 �� �� ֵ  : LTE_RET_E
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��10��25��������
    ��    ��   : �¼�
    �޸�����   : �����ɺ���

*****************************************************************************/

/*altered by caibing 2013-11-25*/
static int arccra_begin_disconnect(int iIPFamily)
{
    /*altered by caibing 2014-09-23*/

	return 0;
}

static LTE_RET_E arccra_disconnect(void *pvData, char *pcBuf)
{
    return LTE_OK;
}

static LTE_RET_E arccra_sim_state_get(void *pvData, char *pcBuf)
{
	int iRet = 0; 

	LTE_SIM_STATE_T *pstSimState = (LTE_SIM_STATE_T *)pvData;
	if (pvData == NULL)
		return LTE_FAIL;
    
	if (NULL != strstr(pcBuf, "+CPIN:"))
	{
		pstSimState->enSimState = LTE_SIM_READY;
	}
    else
	{
		pstSimState->enSimState = LTE_SIM_NOT_FOUND;
	}
	
	return LTE_OK;
}
static LTE_RET_E arccra_qsqinfo_get(void *pvData, char *pcBuf)
{
    char acNeedleStr[] = "+QSQ:";
    char acGsmCiev[] = "GSM";
    char acLteCiev[] = "LTE";
    char acTDCiev[] = "TD-SCDMA";
    char acWcdmaCiev[] = "TD-SCDMA";
	char acTmp[32] = {0};
    char acRsrp[32] = {0};
    char acRsrq[32] = {0};
    char acRssi[32] = {0};
    char acSinr[32] = {0};

   
    LTE_QSQ_T *pstQsqInfo = (LTE_QSQ_T *)pvData;

    if (pvData == NULL)
		return LTE_FAIL;

    if(NULL != strstr(pcBuf, acLteCiev))
    {
        sscanf(pcBuf, "%[^,],%[^,],%[^,],%[^,],%s", acTmp, acRsrp, acRsrq, acRssi, acSinr);
    }
    else if(NULL != strstr(pcBuf, acGsmCiev))
    {
        sscanf(pcBuf, "%[^,],%s", acTmp, acRssi);
        LTE_LOG(LTE_LOG_ERR,"%d,%s:this net is GSM(2G):rssi:%s\n",__LINE__,__func__, acRssi);
    }
    else if((NULL != strstr(pcBuf, acTDCiev))||(NULL != strstr(pcBuf, acWcdmaCiev)))
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:this net is TD-SCDMA(3G)\n",__LINE__,__func__);
        return LTE_FAIL;
    }
    else
    {
        sscanf(pcBuf, "%[^:]:%[^,],%[^,],%[^,],%s", acTmp, acRsrp, acRsrq, acRssi, acSinr);
    }

    pstQsqInfo->iRsrp = atoi(acRsrp);
    pstQsqInfo->iRsrq = atoi(acRsrq);
    pstQsqInfo->iRssi = atoi(acRssi);
    pstQsqInfo->iSinr = atoi(acSinr);

    return LTE_OK;

    
}

static LTE_RET_E arccra_bandinfo_get(void *pvData, char *pcBuf)
{
    int iRet = 0; 
    char acNeedleStr[] = "+QLF:";
    char acLteCiev[] = "LTE";
    char acTDCiev[] = "TD-SCDMA";
    char acWcdmaCiev[] = "TD-SCDMA";
    char acCiev[] = "GSM";
	char acTmp[32] = {0};
    char acChannel[32] = {0};    
    char acBand[32] = {0};
    char acPhyCellid[32] = {0};
	FILE* pstFd = NULL;
	int  iFlag = 0;

    LTE_BAND_T *pstBandInfo = (LTE_BAND_T *)pvData;

    if (pvData == NULL)
		return LTE_FAIL;


    if((NULL != strstr(pcBuf, acTDCiev)) || (NULL != strstr(pcBuf, acWcdmaCiev)))
    {
        return LTE_FAIL;
    }
    else if(NULL != strstr(pcBuf, acLteCiev))
    {
        sscanf(pcBuf, "%[^,],%[^,],%[^,],%s", acTmp, acChannel, acBand, acPhyCellid);
    }
    else
    {
        sscanf(pcBuf, "%[^:]:%[^,],%s", acTmp, acChannel, acBand);
    }

    
    strcpy(pstBandInfo->acBand, acBand);
    pstBandInfo->iChannel = atoi(acChannel);
    strcpy(pstBandInfo->acPhyCellid, acPhyCellid);

    return LTE_OK;
}

static LTE_RET_E arccra_lte_info_get(void *pvData, char *pcBuf)
{
    LTE_BAND_T stBandInfo = {0};
    LTE_QSQ_T  stQsqInfo = {0};
    LTE_CELLID_T stCellid = {0};
    
	LTE_INFO_T *pstLteInfo = (LTE_INFO_T *)pvData;
    
	if (pvData == NULL)
		return LTE_FAIL;

    if(NULL != strstr(pcBuf, "+CEREG:"))
    {
        if (LTE_OK != arccra_cellid_get((void *)(&(stCellid)), pcBuf)){
            LTE_LOG(LTE_LOG_ERR, "get cell id fail");
            //return LTE_FAIL;
        }

    }
    else if( NULL != strstr(pcBuf, "+QSQ:") )
    {
        if (LTE_OK != arccra_qsqinfo_get((void *)(&(stQsqInfo)), pcBuf)){
		LTE_LOG(LTE_LOG_ERR, "get qsq info fail");
		//return LTE_FAIL;
	    }
    }
    else if( NULL != strstr(pcBuf, "+QLF:") )
    {
        if (LTE_OK != arccra_bandinfo_get((void *)(&(stBandInfo)), pcBuf)){
        LTE_LOG(LTE_LOG_ERR, "get band info fail");
        //return LTE_FAIL;
    }
    }
    else if(NULL != strstr(pcBuf, "OK"))
    {
        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }

    
    strcpy(pstLteInfo->stCellInfo.acCellId, stCellid.acCellID);
    pstLteInfo->stCellInfo.iRsrp = stQsqInfo.iRsrp;
    pstLteInfo->stCellInfo.iRsrq = stQsqInfo.iRsrq;
    pstLteInfo->stCellInfo.iRssi = stQsqInfo.iRssi;
    pstLteInfo->stCellInfo.iSinr = stQsqInfo.iSinr;
    pstLteInfo->stCellInfo.iChannel = stBandInfo.iChannel;
    strcpy(pstLteInfo->stCellInfo.acBand, stBandInfo.acBand);
    strcpy(pstLteInfo->stCellInfo.acPhyCeillid, stBandInfo.acPhyCellid);

	return LTE_OK;
}

static LTE_RET_E arccra_access_net_get(void *pvData, char *pcBuf)
{
    char acCiev[] = "+QNSD:";
    char *pcTmp = NULL;

    LTE_ACCESS_NETWORK_T *pstAccessNet = (LTE_ACCESS_NETWORK_T*)pvData;

	if (pvData == NULL)
		return LTE_FAIL;
    
    if (NULL != strstr(pcBuf, acCiev))
    {
        if (NULL != strstr(pcBuf, "LTE"))
        {
            pstAccessNet->enAccessNet = NET_4G;
        }
        else if (NULL != strstr(pcBuf, "SERVICE")) /* �ź�����̫����޷��񣬷���LIMITED SERVICE��NO SERVICE */
        {
            pstAccessNet->enAccessNet = NET_NONE;
        }
        else if((NULL != strstr(pcBuf, "GSM")) || (NULL != strstr(pcBuf, "EDGE"))||(NULL != strstr(pcBuf, "GPRS")))
        {
            pstAccessNet->enAccessNet = NET_2G;
        }
        else
        {
            pstAccessNet->enAccessNet = NET_3G;
        }
        return LTE_OK;
    }

	return LTE_FAIL;
}

static LTE_RET_E arccra_bands_get(void *pvData, char *pcBuf)
{
	if (pvData == NULL)
		return LTE_FAIL;

	return LTE_OK;

}


static LTE_RET_E arccra_clck_bands_set(void *pvData, char *pcBuf)
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

static LTE_RET_E arccra_clck_bands_close(void *pvData, char *pcBuf)
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

static LTE_RET_E arccra_clckcell_enable_get(void *pvData, char *pcBuf)
{
    return LTE_OK;
}
static LTE_RET_E arccra_clck_cellid_set(void *pvData, char *pcBuf)
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
/*****************************************************************************
   �� �� ��  : arccra_nettype_info_get
   ��������  : ��ȡģ��֧�ֵ��������ͣ���2g��3g��4g
   �������  : void *pvData
             int iDataLen
   �������  : ��
   �� �� ֵ  : static
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2014��11��12��
    ��    ��   : wujian
    �޸�����   : �����ɺ���

*****************************************************************************/
static LTE_RET_E arccra_sup_nettype_get(void *pvData, char *pcBuf)
{
    
    return LTE_OK;

}
/*****************************************************************************
   �� �� ��  : arccra_oper_info_get
   ��������  : ��ȡ��Ӫ����Ϣ
   �������  : void *pvData
             int iDataLen
   �������  : ��
   �� �� ֵ  : static
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2014��11��10��
    ��    ��   : wujian
    �޸�����   : �����ɺ���

*****************************************************************************/
static LTE_RET_E arccra_oper_info_get(void *pvData, char *pcBuf)
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
        LTE_LOG(LTE_LOG_ERR,"%d,%s:at command success",__LINE__,__func__);
        return LTE_OK;
    }
    
    if (NULL != strstr(pcBuf, acCiev))
    {
        sscanf(pcBuf,"%*[^\"]\"%[^\"]", acOperTmp);   /*+COPS: 0,2,"46000",7*/

        if(NULL != strstr(LTE_CHINA_MOBILE_CODE, acOperTmp))
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
        }
                
        return LTE_OK;
    }
   
    return LTE_FAIL;

}


/*****************************************************************************
   �� �� ��  : arccra_sms_mode_set
   ��������  : ���ö���ģʽ��0��ʾpduģʽ��1��ʾ�ı�ģʽ
   �������  : void *pvData
             int iDataLen
   �������  : ��
   �� �� ֵ  : static
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2014��11��18��
    ��    ��   : wujian
    �޸�����   : �����ɺ���

*****************************************************************************/
static LTE_RET_E arccra_sms_mode_set(void *pvData, char *pcBuf)
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
    
    return LTE_OK;

}

/*****************************************************************************
   �� �� ��  : arccra_sms_init
   ��������  : ���Ź��ܳ�ʼ���ӿ�
   �������  : void
   �������  : ��
   �� �� ֵ  : static
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2014��11��13��
    ��    ��   : wujian
    �޸�����   : �����ɺ���

*****************************************************************************/
static LTE_RET_E arccra_sms_func_init(void *pvData, char *pcBuf)
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

/*****************************************************************************
   �� �� ��  : arccra_cent_code_set
   ��������  : ���ö������ĺ�
   �������  : void *pvData
             int iDataLen
   �������  : ��
   �� �� ֵ  : static
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2014��11��13��
    ��    ��   : wujian
    �޸�����   : �����ɺ���

*****************************************************************************/
static LTE_RET_E arccra_sms_cent_code_set(void *pvData, char *pcBuf)
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

/*****************************************************************************
   �� �� ��  : arccra_sms_recv
   ��������  : ��pduģʽ��ȡ����
   �������  : void *pvData
             int iDataLen
   �������  : ��
   �� �� ֵ  : static
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2014��11��26��
    ��    ��   : wujian
    �޸�����   : �����ɺ���

*****************************************************************************/
static LTE_RET_E arccra_sms_recv(void *pvData, char *pcBuf)
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


/*****************************************************************************
 �� �� ��  : arccra_sms_send
 ��������  : ���Ͷ��ţ�֧�ֳ����ŷ���
 �������  : void *pvData  
             int iDataLen  
 �������  : ��
 �� �� ֵ  : static
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2014��12��17��
    ��    ��   : wujian
    �޸�����   : �����ɺ���

*****************************************************************************/
static LTE_RET_E arccra_sms_send(void *pvData, char *pcBuf)
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

/*****************************************************************************
   �� �� ��  : arccra_sms_dele
   ��������  :  ����ɾ���ӿ�
   �������  : void *pvData
             int iDataLen
   �������  : ��
   �� �� ֵ  : static
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2014��11��13��
    ��    ��   : wujian
    �޸�����   : �����ɺ���

*****************************************************************************/
static LTE_RET_E arccra_sms_dele(void *pvData, char *pcBuf)
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
/*****************************************************************************
 �� �� ��  : arccra_sms_sup_func
 ��������  : ��ȡģ���Ƿ�֧�ֶ��Ź���
 �������  : void *pvData  
             int iDataLen  
 �������  : ��
 �� �� ֵ  : static
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2014��12��8��
    ��    ��   : wujian
    �޸�����   : �����ɺ���

*****************************************************************************/
static LTE_RET_E arccra_sms_sup_func(void *pvData, char *pcBuf)
{
    LTE_SUPPORT_SMS_T *pstSup = (LTE_SUPPORT_SMS_T *)pvData;
    pstSup->enSupSms = SUP_SMS_FUNC;
    return LTE_OK;

}
static LTE_RET_E arccra_cgdcont_set(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";

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
/*****************************************************************************
   �� �� ��  : arccra_apn_set
   ��������  : ����apn
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
static LTE_RET_E arccra_apn_set(void *pvData, char *pcBuf)
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
static LTE_RET_E arccra_version_get(void *pvData, char *pcBuf)
{
    int iRet = 0;
    char acCiev[] = "MD85";    
    char acVersion[64] = {0};
    char *pcTmp = NULL;
    


    LTE_MODULE_VERSION_T *pstVersion = (LTE_MODULE_VERSION_T *)pvData;

    if(NULL == pvData)
    {
        return LTE_FAIL;
    }

    if(NULL != (pcTmp = strstr(pcBuf, acCiev)))
    {
        sscanf(pcTmp, "%s", acVersion);

        if(NULL != strstr(acVersion, ARCCRA_1_M_4_VEVISION))
        {
            pstVersion->iVersionIndex = LTE_ARCCRA_8501C;
        }
        else if(NULL != strstr(acVersion, ARCCRA_2_M_2_BAND_VEVISION))
        {
            pstVersion->iVersionIndex = LTE_ARCCRA_8500;
        }
        else if(NULL != strstr(acVersion, ARCCRA_2_M_4_BAND_VEVISION))
        {
            pstVersion->iVersionIndex = LTE_ARCCRA_8501A;
        }
        else if(NULL != strstr(acVersion, ARCCRA_4_M_4_BAND_VEVISION))
        {
            pstVersion->iVersionIndex = LTE_ARCCRA_8501B;
        }
        else
        {   
            pstVersion->iVersionIndex = LTE_VER_IND_MAX;
        }
        
        strncpy(pstVersion->acVersion, acVersion, sizeof(pstVersion->acVersion));
        return LTE_OK;
    }
    return LTE_FAIL;

}

static LTE_RET_E arccra_enodeb_id_get(void *pvData, char *pcBuf)
{
    unsigned long ulEnodeb;
    char acCellid[32] = {0};
    char *pcTmp = NULL;
    LTE_RET_E enRet = 0;
    
    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_ENODB_ID_T *pstEnodebId = (LTE_ENODB_ID_T*)pvData;

    if(NULL != strstr(pcBuf, "OK"))
    {
        return LTE_OK;
    }
    else if(NULL != (pcTmp = strstr(pcBuf, "+CEREG:")))
    {
        sscanf(pcTmp, "%*[^\"]\"%*[^\"]\"%*[^\"]\"%*[^\"]\"%*[^\"]\"%[^\"]", acCellid);

        LTE_LOG(LTE_LOG_INFO, "%d,%s:cellid [%s]\n", __LINE__, __func__, acCellid);
        
        ulEnodeb = strtoul(acCellid, NULL, 16);
        snprintf(pstEnodebId->acEnodebId, sizeof(pstEnodebId->acEnodebId),"%u", (ulEnodeb >> 8));
        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }

    return LTE_OK;

}

/*****************************************************************************
 �� �� ��  : arccra_tac_get
 ��������  : ��ȡС����������λ������
 �������  : void *pvData  
             int iDataLen  
 �������  : ��
 �� �� ֵ  : static
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��2��4��
    ��    ��   : wujian
    �޸�����   : �����ɺ���

*****************************************************************************/
static LTE_RET_E arccra_tac_get(void *pvData, char *pcBuf)
{
    unsigned long ulTac;
    char acTacId[32] = {0};
    char *pcTmp = NULL;
    
    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    LTE_TAC_T *pstTac = (LTE_TAC_T *)pvData;

    if(NULL != strstr(pcBuf, "OK"))
    {
        return LTE_OK;
    }
    else if(NULL != (pcTmp = strstr(pcBuf, "+CEREG:")))
    {
        sscanf(pcTmp,"%*[^\"]\"%[^\"]", acTacId);

        LTE_LOG(LTE_LOG_INFO, "%d,%s:acTacId [%s]\n",__LINE__,__func__, acTacId);
        ulTac = strtoul(acTacId, NULL, 16);
        
        snprintf(pstTac->acTac, sizeof(pstTac->acTac),"%u", ulTac);

        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }

    return LTE_OK;

}

static LTE_RET_E arccra_lcid_get(void *pvData, char *pcBuf)
{
    unsigned long ulLcid;
    char acCellid[32] = {0};
    char *pcTmp = NULL;
    
    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_LCID_T *pstLcid = (LTE_LCID_T*)pvData;

    if(NULL != strstr(pcBuf, "OK"))
    {
        return LTE_OK;
    }
    else if(NULL != (pcTmp = strstr(pcBuf, "+CEREG:")))
    {
        sscanf(pcTmp,"%*[^\"]\"%*[^\"]\"%*[^\"]\"%*[^\"]\"%*[^\"]\"%[^\"]", acCellid);

        LTE_LOG(LTE_LOG_INFO, "%d,%s:cellid [%s]\n",__LINE__,__func__, acCellid);
        ulLcid = strtoul(acCellid, NULL, 16);
        
        /*���ĸ��ֽڣ�ȡcid��ǰ�����ֽ�*/
        snprintf(pstLcid->acLcid, sizeof(pstLcid->acLcid),"%u", (ulLcid & LTE_LCID_MASK));

        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }
   
    return LTE_OK;

}

static LTE_RET_E arccra_all_cellid_get(void *pvData, char *pcBuf)
{   
    unsigned long ulCellId;
    unsigned long ulTac;
    char acCellid[32] = {0};
    char acTacId[32] = {0};
    char *pcTmp = NULL;
    char *pcCid = acCellid;

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }

    LTE_ALL_CELLID_T *pstAllCid = (LTE_ALL_CELLID_T *)pvData;

    if(NULL != strstr(pcBuf, "OK"))
    {
       return LTE_OK;
    }
    else if(NULL != (pcTmp = strstr(pcBuf, "+CEREG:")))
    {
       sscanf(pcTmp,"%*[^\"]\"%*[^\"]\"%*[^\"]\"%*[^\"]\"%*[^\"]\"%[^\"]", acCellid);
       sscanf(pcTmp,"%*[^\"]\"%[^\"]", acTacId);
    }
    else
    {
       return LTE_FAIL;
    }

    ulCellId = strtoul(acCellid, NULL, 16);
    ulTac = strtoul(acTacId, NULL, 16);
    /*cellid*/
    if(*pcCid == '0')
        pcCid++;
    strncpy(pstAllCid->acCellId, pcCid, sizeof(pstAllCid->acCellId));
    /*���ĸ��ֽڣ�ȡcid��ǰ�����ֽ�*/
    snprintf(pstAllCid->acEnodebId, sizeof(pstAllCid->acEnodebId),"%u", (ulCellId >> 8));
    /*���ĸ��ֽڣ�ȡcid�ĵ��ĸ��ֽ�*/
    snprintf(pstAllCid->acLcid, sizeof(pstAllCid->acLcid),"%u", (ulCellId & LTE_LCID_MASK));
    snprintf(pstAllCid->acTac, sizeof(pstAllCid->acTac),"%u", ulTac);
    return LTE_OK;

}

static LTE_RET_E arccra_sim_unlock(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:arccra_sim_unlock success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:arccra_sim_unlock failed!",__LINE__,__func__);
    }    
    return LTE_FAIL;
}

static LTE_RET_E arccra_sim_disable_unlock(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:arccra_sim_disable_unlock success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:arccra_sim_disable_unlock failed!",__LINE__,__func__);
    }   
    return LTE_FAIL;
}

static LTE_RET_E arccra_pin_input_remain(void *pvData, char *pcBuf)
{
    char    acTacTmp[32] = {0};
    int     iRet = 0;
    char    acCiev[32] = "+QPINPUK:";
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




LTE_MODULE_OPS_T stLte_md8500_ops_t = {
    .lte_pin_state_get       = arccra_pin_state,
    .lte_pin_switch          = arccra_pin_switch,    
    .lte_pin_switch_get      = arccra_pin_sw_get,
    .lte_pin_unlock          = arccra_pin_unlock,
    .lte_pin_attempts        = NULL,
    .lte_pin_code_alter      = arccra_pin_alter,
    .lte_signal_get          = arccra_signal_get,
    .lte_power_on            = arccra_power_on,
    .lte_power_off           = arccra_power_off,
    .lte_power_state_get     = NULL,
    .lte_net_type_set        = arccra_nettype_set,
    .lte_net_type_get        = arccra_nettype_get,
    .lte_plmn_register       = NULL,
    .lte_plmn_prefer_set     = NULL,
    .lte_net_work_prepare    = arccra_network_prepare,
    .lte_net_work_status_get = NULL,
    .lte_connect             = arccra_connect,
    .lte_disconnect          = arccra_disconnect,
    .lte_link_state_get      = arccra_linkstate_get,
    .lte_imei_get            = arccra_imei_get,
    .lte_cellid_get          = arccra_cellid_get,
    .lte_iccid_get           = arccra_iccid_get,
    .lte_imsi_get            = arccra_imsi_get,
    .lte_sim_state_get       = arccra_sim_state_get,
    .lte_info_get            = arccra_lte_info_get,
    .lte_access_net_get      = arccra_access_net_get,
    .lte_bands_get           = arccra_bands_get,
    .lte_clck_bands          = arccra_clck_bands_set,
    .lte_clck_cell_get       = arccra_clckcell_enable_get,
    .lte_clck_cell           = arccra_clck_cellid_set,
    .lte_gps_start           = NULL, 
    .lte_gps_stop            = NULL,
    .lte_oper_info_get       = arccra_oper_info_get,
    .lte_nettype_info_get    = arccra_sup_nettype_get,
    .lte_sms_cent_code_set   = arccra_sms_cent_code_set,
    .lte_sms_mode_set        = arccra_sms_mode_set,
    .lte_sms_func_init       = arccra_sms_func_init,
    .lte_sms_recv            = arccra_sms_recv,
    .lte_sms_send            = arccra_sms_send,
    .lte_sms_del             = arccra_sms_dele,
    .lte_sms_supinfo_get     = arccra_sms_sup_func,
    .lte_sms_ip_stack_set    = arccra_cgdcont_set,
    .lte_apn_get             = NULL,
    .lte_apn_set             = arccra_apn_set,
    .lte_version_get         = arccra_version_get,
    .lte_enodebid_get        = arccra_enodeb_id_get,
    .lte_tac_get             = arccra_tac_get,
    .lte_lcid_get            = arccra_lcid_get,
    .lte_all_cellid_get      = arccra_all_cellid_get,
    .lte_sim_unlock          = arccra_sim_unlock,
    .lte_sim_disable_unlock  = arccra_sim_disable_unlock,
    .lte_sim_type_get        = NULL,
    .lte_ip_gw_dns_get       = NULL,
    .lte_all_plmn_mode_get   = NULL,
    .lte_sounet_mode_set     = NULL,
    .lte_sim_puk_unlock		 = NULL,
    .lte_pin_input_remain    = arccra_pin_input_remain,
};

