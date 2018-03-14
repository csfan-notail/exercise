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


#define ATCOM  "/dev/ttyUSB8"

extern LTE_STATE_INFO_T g_stLteStateInfo;

#if 0
#define LEADCORE_GSM      0
#define LEADCORE_TDS      2
#define LEADCORE_LTE      7 
#endif

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
static LTE_RET_E huawei4G_pin_state (void *pvData, char *pcBuf)
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
static LTE_RET_E huawei4G_pin_switch (void *pvData, char *pcBuf)
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
 �� �� ��  : leadcore_pin_sw_get
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
static LTE_RET_E huawei4G_pin_sw_get( void *pvData, char *pcBuf )
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
        pstPinSwState->enState = atoi(acBuffTmp);
    }
    else
    {
        return LTE_FAIL;
    }
   
    return LTE_OK;
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
 static LTE_RET_E huawei4G_pin_unlock (void *pvData, char *pcBuf)
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
static LTE_RET_E huawei4G_pin_attempts_get(void *pvData, int iDataLen)
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
static LTE_RET_E huawei4G_pin_alter (void *pvData, char *pcBuf)
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
static LTE_RET_E huawei4G_power_on (void *pvData, char *pcBuf)
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
static LTE_RET_E huawei4G_power_off (void *pvData, char *pcBuf)
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
static LTE_RET_E huawei4G_signal_get  (void *pvData, char *pcBuf)
{
    int iRet = 0;
    char acCiev[] = "+CSQ:";
    char acTmp[16] = {0};

    LTE_SIGNAL_T *pstSignal = (LTE_SIGNAL_T*)pvData;

    if(NULL == pvData)
    {
        return LTE_FAIL;
    }

    if(NULL != strstr(pcBuf, acCiev))
    {
        sscanf(pcBuf, "%*[^:]:%s", acTmp);
        pstSignal->iSignal = atoi(acTmp);
        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }
    

}
#if 0
static LTE_RET_E leadcore_power_get(void *pvData, char *pcBuf)
{
    return LTE_OK;
}


/*altered by caibing 2013-12-17*/
/*****************************************************************************
 �� �� ��  : leadcore_nettype_set
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
static LTE_RET_E leadcore_nettype_set (void *pvData, char *pcBuf)
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
 �� �� ��  : leadcore_nettype_get
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
static LTE_RET_E leadcore_nettype_get(void *pvData, char *pcBuf)
{
    int iRet = 0;
    char acNetPri[16] = {0};
    char acNetType[16] = {0};
    //int iNetPri;
    int iNettype;
    char acNets[32] = {0};

    if (pvData == NULL)
    {
		return LTE_FAIL;
    }
    /*^DDTM: 1,0,7,2,0      ^DDTM: 1,1,7*/
    LTE_NET_TYPE_T *pstNetType = (LTE_NET_TYPE_T *)pvData;
    if(NULL == strstr(pcBuf, "^DDTM:"))
    {
        return LTE_FAIL;
    }

    sscanf(pcBuf, "%*[^,],%[^,],%[^,]", acNetPri, acNetType);
    if( atoi(acNetPri) == 1)  /*��ģ*/
    {
        //sscanf(pcBuf, "%*[^,],%*[^,],%[^,]", acNetType);
        iNettype = atoi(acNetType);
        if(LEADCORE_LTE == iNettype)
        {
            pstNetType->enNetType = LTE_4G_ONLY;
        }
        else if(LEADCORE_TDS == iNettype)
        {
            pstNetType->enNetType = LTE_3G_ONLY;
        }
        else
        {
            pstNetType->enNetType = LTE_2G_ONLY;
        }
    }
    else if(atoi(acNetPri) == 0)
    {
        sscanf(pcBuf, "%*[^,],%*[^,],%s", acNets);
        if(NULL != strstr(acNets, "0"))
        {
            pstNetType->enNetType |= LTE_2G_ONLY;
        }
        if(NULL != strstr(acNets, "2"))
        {
            pstNetType->enNetType |= LTE_3G_ONLY;
        }
        if(NULL != strstr(acNets, "7"))
        {
            pstNetType->enNetType |= LTE_4G_ONLY;
        }
    }
    else
    {
        pstNetType->enNetType = LTE_4G_ONLY; /*ģ����ʱ֧��LTE��ģ*/
    }
      
    return LTE_OK;
    
}
#endif

static LTE_RET_E huawei4G_imei_get(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    LTE_IMEI_T *pstImei = (LTE_IMEI_T*)pvData;

    //if(NULL != strstr(pcBuf, acCiev))
    //{
        sscanf(pcBuf, "%s", pstImei->acImei);
        //return LTE_OK;
   // }
   // else
   // {
     //   return LTE_FAIL;
   // }
    LTE_LOG(LTE_LOG_INFO,"%d,%s: imei: %s",__LINE__,__func__, pstImei->acImei);
    
    return LTE_OK;

}

static LTE_RET_E huawei4G_cellid_get(void *pvData, char *pcBuf)
{
     char acCellid[32] = {0};
     char *pcTmp = NULL;
     char *pcCid = acCellid;
     
     if (NULL == pvData)
     {
         return LTE_FAIL;
     }
     //^CECELLID: 46000,25694475,246,6234
     LTE_CELLID_T *pstCellId = (LTE_CELLID_T*)pvData;
#if 0
    if(NULL != strstr(pcBuf, "OK"))
    {
        return LTE_OK;
    }
    else if(NULL != (pcTmp = strstr(pcBuf, "+CEREG:")))
    {
        sscanf(pcTmp,"%*[^\"]\"%*[^\"]\"%*[^\"]\"%[^\"]", acCellid);
        
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
#else
    if(NULL != strstr(pcBuf, "OK"))
    {
        return LTE_OK;
    }
    else if(NULL != (pcTmp = strstr(pcBuf, "^CECELLID:")))
    {
        sscanf(pcTmp,"%*[^,],%[^,]", acCellid);
        
        LTE_LOG(LTE_LOG_INFO, "%d,%s:cellid [%s]\n",__LINE__,__func__, acCellid);
        snprintf(pstCellId->acCellID, sizeof(pstCellId->acCellID), "%X", atoi(acCellid));
        //strncpy(pstCellId->acCellID, pcCid, sizeof(pstCellId->acCellID));

        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }
#endif
    

}

static LTE_RET_E huawei4G_iccid_get(void *pvData, char *pcBuf)
{
    /*altered by caibing 2013-11-25*/
    char acCiev[32] = "^ICCID:";
    char acIccid[32] = {0};
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
        sscanf(pcTmp, "%*[^ ] %s", acIccid);
        strncpy(pstIccid->acICCID, acIccid, sizeof(pstIccid->acICCID));
        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }
    
}

static LTE_RET_E huawei4G_imsi_get(void *pvData, char *pcBuf)
{       
    LTE_IMSI_T *pstImsiInfo = (LTE_IMSI_T *)pvData;

    if (pvData == NULL)
    {
		return LTE_FAIL;
    }

    sscanf(pcBuf, "%s", pstImsiInfo->acIMSI);
    return LTE_OK;

}
#if 0

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
static LTE_RET_E leadcore_plmn_register(void *pvData, int iDataLen)
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

static LTE_RET_E leadcore_prefer_plmn_set (void *pvData, int iDataLen)
{
    return LTE_OK;
}
#endif
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
static LTE_RET_E huawei4G_linkstate_get(void *pvData, char *pcBuf)
{
    int iRet = 0;
    int iLinkState = 0;
    char *pcTmp = NULL;
    char acBuffTmp[16];
    char acCiev[] = "^NDISSTATQRY:";
    /*^NDISSTATQRY: 1,,,"IPV4",0,,,"IPV6"*/
    LTE_LINKSTATE_T *pstLinkstate = (LTE_LINKSTATE_T*)pvData;

    if (pvData == NULL)
    {
		return LTE_FAIL;
    }

    if(NULL != (pcTmp = strstr(pcBuf, acCiev)))
    {
        sscanf(pcTmp, "%*[^:]:%[^,]", acBuffTmp);
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

static LTE_RET_E huawei4G_network_prepare(void *pvData, char *pcBuf)
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


static LTE_RET_E huawei4G_connect(void *pvData, char *pcBuf)
{
    char acCiev[] = "^NDISSTAT:1,";
            
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte connect success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte connect failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;
}


static LTE_RET_E huawei4G_disconnect(void *pvData, char *pcBuf)
{
    char acCiev[] = "^NDISSTAT:0,";

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

static LTE_RET_E huawei4G_sim_state_get(void *pvData, char *pcBuf)
{
	LTE_SIM_STATE_T *pstSimState = (LTE_SIM_STATE_T *)pvData;
	char acCiev[] = "+CPIN:";
	char acBufTmp[16] = {0};
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
	/*
	if (NULL != strstr(pcBuf, acCiev))
	{
        sscanf(pcBuf, "%*[^:]:%s", acBufTmp);
	    if('1' == acBufTmp[0])
	    {
            pstSimState->enSimState = LTE_SIM_READY;
	    }
		else
		{
            pstSimState->enSimState = LTE_SIM_NOT_FOUND;
		}
	}
    else
	{
		pstSimState->enSimState = LTE_SIM_NOT_FOUND;
	}
	
	return LTE_OK;
	*/
}

static LTE_RET_E huawei4G_lte_info_get(void *pvData, char *pcBuf)
{ 
	LTE_INFO_T *pstLteInfo = (LTE_INFO_T *)pvData;
	char acRsrp[32] = {0};
    char acRsrq[32] = {0};
    char acRssi[32] = {0};
    char acSinr[32] = {0};
    char acBand[32] = {0};
    char acChannel[32] = {0};
    char acPhyCeillid[32] = {0};
    char acCellid[32] = {0};
    
	if (pvData == NULL)
		return LTE_FAIL;
    /*^CERSSI:0,0,0,0,255,-103,-16,-4,32639,32639,32639*/
    //printf("pcBuf==%s\n", pcBuf);
   // LTE_LOG(LTE_LOG_NOTICE,"%d,%s:pcBuf==%s",__LINE__,__func__, pcBuf);
    if(NULL != strstr(pcBuf, "^CERSSI:"))
    {
        sscanf(pcBuf, "%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%[^,],%[^,],%[^,]", acRsrp, acRsrq, acSinr);
        pstLteInfo->stCellInfo.iRsrp = atoi(acRsrp);
        pstLteInfo->stCellInfo.iRsrq = atoi(acRsrq);
        pstLteInfo->stCellInfo.iSinr = atoi(acSinr);
    }
    /*^LWURC: 0,18900,5,18900,5,39*/
    else if(NULL != strstr(pcBuf, "^LWURC:"))
    {
        sscanf(pcBuf, "%*[^,],%*[^,],%*[^,],%[^,],%*[^,],%s", acChannel, acBand);     
        strncpy(g_stLteStateInfo.acBand, acBand, sizeof(pstLteInfo->stCellInfo.acBand));
        //if(39 == atoi(acBand))
        //{
        //    g_stLteStateInfo.iChannel = atoi(acChannel)+19450;//�� Ƶ��*10 �����Ƶ��
        //}
        //else
        //{
            //pstLteInfo->stCellInfo.iChannel = atoi(acChannel)/10;
            g_stLteStateInfo.iChannel = atoi(acChannel);
        //}
    }
    //^CSG:341,-832,-160,-536,0,0
    else if(NULL != strstr(pcBuf, "^CSG:"))
    {
        sscanf(pcBuf, "%*[^:]:%[^,]", acPhyCeillid);
        strncpy(g_stLteStateInfo.acPhyCellId, acPhyCeillid, sizeof(pstLteInfo->stCellInfo.acBand));
    }
    //^CECELLID: 46000,25694475,246,6234
    else if(NULL != strstr(pcBuf, "^CECELLID:"))
    {
        sscanf(pcBuf, "%*[^,],%[^,],%[^,]", acCellid, acPhyCeillid);
        snprintf(pstLteInfo->stCellInfo.acCellId, sizeof(pstLteInfo->stCellInfo.acCellId), "%X", atoi(acCellid));
        strncpy(pstLteInfo->stCellInfo.acPhyCeillid, acPhyCeillid, sizeof(pstLteInfo->stCellInfo.acPhyCeillid));
    }
    else if(NULL != strstr(pcBuf, "OK"))
    {
        //memset(pstLteInfo, 0, sizeof(LTE_INFO_T));
    }
    else
    {
        return LTE_FAIL;
    }
    g_stLteStateInfo.iLteInfoStaticFLag = 1;

	return LTE_OK;
}

static LTE_RET_E huawei4G_access_net_get(void *pvData, char *pcBuf)
{
    char acCiev[] = "^SYSINFOEX:";//^SYSINFOEX:2,3,0,1,,6,"LTE",101,"LTE"
    char acMode[128] = {0};
    int iMode;
    LTE_ACCESS_NETWORK_T *pstAccessNet = (LTE_ACCESS_NETWORK_T*)pvData;
    char *pcTmp = NULL;

	if (pvData == NULL)
		return LTE_FAIL;
    
    if (NULL != strstr(pcBuf,acCiev))
    {
        sscanf(pcBuf, "%*[^,],%*[^,],%*[^,],%*[^,],%s", acMode);
        pcTmp = strchr(acMode, ',');
        iMode = atoi(pcTmp+1);      
        
        

        if(6 == iMode)
        {
            pstAccessNet->enAccessNet = NET_4G;
        }
        else if(3 == iMode || 4 == iMode)
        {
            pstAccessNet->enAccessNet = NET_3G;
        }
        else if(1 == iMode)
        {
            pstAccessNet->enAccessNet = NET_2G;
        }
        else
        {
            pstAccessNet->enAccessNet = NET_NONE;
        }
        return LTE_OK;
	
    }
    

	return LTE_FAIL;
}
#if 0
static LTE_RET_E leadcore_bands_get(void *pvData, char *pcBuf)
{

	return LTE_OK;

}


static LTE_RET_E leadcore_clck_bands_set(void *pvData, char *pcBuf)
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

static LTE_RET_E leadcore_clck_bands_close(void *pvData, char *pcBuf)
{

    return LTE_OK;

}

static LTE_RET_E leadcore_clckcell_enable_get(void *pvData, char *pcBuf)
{
    return LTE_OK;
}
#endif

static LTE_RET_E huawei4G_clck_cellid_set(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
        
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte clckcell set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte clckcell set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;

}

/*****************************************************************************
   �� �� ��  : leadcore_oper_info_get
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
static LTE_RET_E huawei4G_oper_info_get(void *pvData, char *pcBuf)
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
        //LTE_LOG(LTE_LOG_ERR,"%d,%s:at command success",__LINE__,__func__);
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

#if 0
/*****************************************************************************
   �� �� ��  : leadcore_sms_mode_set
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
static LTE_RET_E leadcore_sms_mode_set(void *pvData, char *pcBuf)
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
   �� �� ��  : leadcore_sms_init
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
static LTE_RET_E leadcore_sms_func_init(void *pvData, char *pcBuf)
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
   �� �� ��  : leadcore_cent_code_set
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
static LTE_RET_E leadcore_sms_cent_code_set(void *pvData, char *pcBuf)
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
   �� �� ��  : leadcore_sms_recv
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
static LTE_RET_E leadcore_sms_recv(void *pvData, char *pcBuf)
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
 �� �� ��  : leadcore_sms_send
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
static LTE_RET_E leadcore_sms_send(void *pvData, char *pcBuf)
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
   �� �� ��  : leadcore_sms_dele
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
static LTE_RET_E leadcore_sms_dele(void *pvData, char *pcBuf)
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
 �� �� ��  : leadcore_sms_sup_func
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
static LTE_RET_E leadcore_sms_sup_func(void *pvData, char *pcBuf)
{
    LTE_SUPPORT_SMS_T *pstSup = (LTE_SUPPORT_SMS_T *)pvData;
    pstSup->enSupSms = SUP_SMS_FUNC;
    return LTE_OK;

}
static LTE_RET_E leadcore_cgdcont_set(void *pvData, char *pcBuf)
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
#endif

/*****************************************************************************
   �� �� ��  : leadcore_apn_set
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
static LTE_RET_E huawei4G_apn_set(void *pvData, char *pcBuf)
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

static LTE_RET_E huawei4G_version_get(void *pvData, char *pcBuf)
{
    int iRet = 0;   
    char acVersion[64] = {0};
    
    LTE_MODULE_VERSION_T *pstVersion = (LTE_MODULE_VERSION_T *)pvData;

    if(NULL == pvData)
    {
        return LTE_FAIL;
    }
    sscanf(pcBuf, "%s", acVersion);
    strncpy(pstVersion->acVersion, acVersion, sizeof(pstVersion->acVersion));
    pstVersion->iVersionIndex = LTE_HUAWEI4G;
    
    return LTE_OK;


}

static LTE_RET_E huawei4G_enodeb_id_get(void *pvData, char *pcBuf)
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
        sscanf(pcTmp,"%*[^\"]\"%*[^\"]\"%*[^\"]\"%[^\"]", acCellid);
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
 �� �� ��  : huawei4G_tac_get
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
static LTE_RET_E huawei4G_tac_get(void *pvData, char *pcBuf)
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

static LTE_RET_E huawei4G_lcid_get(void *pvData, char *pcBuf)
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
        sscanf(pcTmp,"%*[^\"]\"%*[^\"]\"%*[^\"]\"%[^\"]", acCellid);

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
   
}

static LTE_RET_E huawei4G_all_cellid_get(void *pvData, char *pcBuf)
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
#if 0
    else if(NULL != (pcTmp = strstr(pcBuf, "+CEREG:")))
    {
       sscanf(pcTmp,"%*[^\"]\"%*[^\"]\"%*[^\"]\"%[^\"]", acCellid);
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
#else
    //^CECELLID: 46000,25694475,246,6234
    else if(NULL != (pcTmp = strstr(pcBuf, "^CECELLID:")))
    {
       sscanf(pcTmp,"%*[^,],%[^,],%*[^,],%s", acCellid, acTacId);
    }
    else
    {
       return LTE_FAIL;
    }
    ulCellId = atoi(acCellid);
    snprintf(pstAllCid->acCellId, sizeof(pstAllCid->acCellId), "%X", ulCellId);
    snprintf(pstAllCid->acEnodebId, sizeof(pstAllCid->acEnodebId),"%u", (ulCellId >> 8));
    snprintf(pstAllCid->acLcid, sizeof(pstAllCid->acLcid),"%u", (ulCellId & LTE_LCID_MASK));
    strncpy(pstAllCid->acTac, acTacId, sizeof(pstAllCid->acTac));

#endif
    return LTE_OK;

}
#if 0
static LTE_RET_E leadcore_mac_set(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte mac set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte mac set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;
}

static LTE_RET_E leadcore_mac_get(void *pvData, char *pcBuf)
{
	char acCiev[] = "^QMAC:";
        
    LTE_MOUDLE_MAC_T *pstMac = (LTE_MOUDLE_MAC_T *)pvData;

    if (pvData == NULL)
    {
		return LTE_FAIL;
    }
    if(NULL != strstr(pcBuf, acCiev))
    {
        sscanf(pcBuf, "%*[^ ] %s", pstMac->acMac);
        return LTE_OK;
    }
    else
    {
        return LTE_FAIL;
    }

}

static LTE_RET_E leadcore_sim_unlock(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";

    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:leadcore_bind_machine_card success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:leadcore_bind_machine_card failed!",__LINE__,__func__);
    }    
    return LTE_FAIL;
}

static LTE_RET_E leadcore_sim_disable_unlock(void *pvData, char *pcBuf)
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

static LTE_RET_E leadcore_sim_type_get(void *pvData, char *pcBuf)
{
    char acCiev[] = "D0248103012303820281828D1508";
    LTE_SIM_RANDOM_NUM_T *pstRandNum = (LTE_SIM_RANDOM_NUM_T *)pvData;

    if (pvData == NULL)
    {
		return LTE_FAIL;
    }

    if(NULL == strstr(pcBuf, "AABB00CCDD"))
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:leadcore_remove_bind failed!",__LINE__,__func__);
        return LTE_FAIL;
    }

    if(NULL != strstr(pcBuf, acCiev))
    {
        pstRandNum->iSIMType = ENCRYPTION_SIM;
    }
    else
    {
        pstRandNum->iSIMType = NORMAL_SIM;
    }
    return LTE_OK;

    
}

static LTE_RET_E leadcore_all_plmn_mode_get(void *pvData, char *pcBuf)
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
    /*LTE_LOG(LTE_LOG_INFO, "%d,%s:leadcore_all_plmn_mode_get success!",__LINE__,__func__);
    if(NULL != strstr(pcBuf, "CHINA  MOBILE"))
    {
        pstAllPlmnMode->iAllOper |= MOBILE;
    }
    if(NULL != strstr(pcBuf, "CHN-UNICOM"))
    {
        pstAllPlmnMode->iAllOper |= UNICOM;
    }
    if(NULL != strstr(pcBuf, "CHINA TELECOM"))
    {
        pstAllPlmnMode->iAllOper |= TELECOM;
    }*/
    memset(pstAllPlmnMode->acAllPlmn, 0, sizeof(pstAllPlmnMode->acAllPlmn));
    pObject = pstAllPlmnMode->acAllPlmn;
    while(NULL != (p = strstr(p, "460")))
    {
        sscanf(p, "%[^\"]\",%[^)]", acPlmn, acMode);
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
            break;
        }
        pObject += strlen(pObject);    
    }
    *(pObject - 1) = '\0';
    return LTE_OK;    
}

static LTE_RET_E leadcore_sounet_mode_set(void *pvData, char *pcBuf)
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
   
static LTE_RET_E leadcore_unlock_puk(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";

    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:leadcore_unlock_puk success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:leadcore_unlock_puk failed!",__LINE__,__func__);
    }   
    return LTE_FAIL;
}

static LTE_RET_E leadcore_pin_input_remain(void *pvData, char *pcBuf)
{
    char    acTacTmp[32] = {0};
    int     iRet = 0;
    char    acCiev[32] = "^DRAP:";
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
#endif
static LTE_RET_E huawei4G_post_route_set(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte post_route set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte post_route set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;

}
static LTE_RET_E huawei4G_nv_datalock(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte nv_datalock set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte nv_datalock set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;

}
static LTE_RET_E huawei4G_post_route_get(void *pvData, char *pcBuf)
{
    char    acTacTmp[32] = {0};
    int     iRet = 0;
    char    acCiev[32] = "+RBMS:";
    char    *pcBufTmp = NULL;

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_POST_ROUTE_SW_T *pstPostRouteSw  = (LTE_POST_ROUTE_SW_T *)pvData;
    
    if(NULL != (pcBufTmp = strstr(pcBuf, acCiev)))
    {
        sscanf(pcBufTmp, "%*[^:]:%s", acTacTmp);
    }
    else
    {
        return LTE_FAIL;
    }
    
    pstPostRouteSw->iPostRouteSw = atoi(acTacTmp);
    
    return LTE_OK;
}

static LTE_RET_E huawei4G_remove_attachments(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte remove_attachments set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte remove_attachments set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;

}

static LTE_RET_E huawei4G_qos_set(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte qos set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte qos set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;

}

static LTE_RET_E huawei4G_qos_get(void *pvData, char *pcBuf)
{
	printf("SCsdfsdgvsdb x\n");
	char    acTacTmp[32] = {0};
    int     iRet = 0;
    char    acCiev[32] = "+CGEQOS:";
    char    *pcBufTmp = NULL;
    char 	acQosCid[32]={0};
	char 	acQosQci[32]={0};
	char 	acQosDlGbr[32]={0};
	char 	acQosUlGbr[32]={0};
	char 	acQosDlMbr[32]={0};
	char 	acQosUlMbr[32]={0};
    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_QOS_DATA_S *pstQosData  = (LTE_QOS_DATA_S *)pvData;
    printf("pbuf = %s\n",pcBuf);
    if(NULL != (pcBufTmp = strstr(pcBuf, acCiev)))
    {
        //sscanf(pcBufTmp, "%*[^:],%s", acTacTmp);
        sscanf(pcBufTmp,"%*[^:]:%[^,]",acQosCid);
		sscanf(pcBufTmp,"%*[^,],%[^,],%*[^,]",acQosQci);
		sscanf(pcBufTmp,"%*[^,],%*[^,],%[^,]",acQosDlGbr);
		sscanf(pcBufTmp,"%*[^,],%*[^,],%*[^,],%[^,]",acQosUlGbr);
		sscanf(pcBufTmp,"%*[^,],%*[^,],%*[^,],%*[^,],%[^,]",acQosDlMbr);
		sscanf(pcBufTmp,"%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%[^,]",acQosUlMbr);
		sscanf(pcBufTmp, "%*[^:],%s", acTacTmp);
		
    }
    else if(NULL != (pcBufTmp = strstr(pcBuf,"OK")))
    {
    	
		
    	return LTE_OK;
        
    }
	else
	{	
		
		return LTE_FAIL;
	}
    pstQosData->iQosCid=atoi(acQosCid);
	pstQosData->iQosQci=atoi(acQosQci);
	pstQosData->iQosDlGbr =atoi(acQosDlGbr);
	pstQosData->iQosUlGbr=atoi(acQosUlGbr);
	pstQosData->iQosDlMbr= atoi(acQosDlMbr);
	pstQosData->iQosUlMbr=atoi(acQosUlMbr);
	
    return LTE_OK;
}

static LTE_RET_E huawei4G_dload_prot_ver_get(void *pvData, char *pcBuf)
{
    return LTE_OK;
}
static LTE_RET_E huawei4G_dload_ver_info_get(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    LTE_DLOAD_VER_INFO_GET_T *pstDloadVerInfo = (LTE_DLOAD_VER_INFO_GET_T*)pvData;

    sscanf(pcBuf,"%s",pstDloadVerInfo->acDloadVerInfo);

    LTE_LOG(LTE_LOG_INFO, "%d,%s:lte dload_ver_info_get success!",__LINE__,__func__);
    
    return LTE_OK;
}
static LTE_RET_E huawei4G_nv_backup(void *pvData, char *pcBuf)
{
    char acTacTmp[16] = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_NV_BACKUP_T *pstNvBackup= (LTE_NV_BACKUP_T *)pvData;
    sscanf(pcBuf, "%s", acTacTmp);
    
    pstNvBackup->iNvBackup = atoi(acTacTmp);
    
    return LTE_OK;
}
static LTE_RET_E huawei4G_nv_restore(void *pvData, char *pcBuf)
{
    char acTacTmp[16] = {0};

    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_NV_RESTORE_T *pstNvRestore= (LTE_NV_RESTORE_T *)pvData;
    sscanf(pcBuf, "%s", acTacTmp);
    
    pstNvRestore->iNvRestore = atoi(acTacTmp);
    
    return LTE_OK;
}
static LTE_RET_E huawei4G_dload_mode(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte dload_mode success!",__LINE__,__func__);
        return LTE_OK;
    }else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte dload_mode failed!",__LINE__,__func__);
    }
    return LTE_FAIL;
}
static LTE_RET_E huawei4G_data_mode(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte data_mode success!",__LINE__,__func__);
        return LTE_OK;
    }else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte data_mode failed!",__LINE__,__func__);
    }
    return LTE_FAIL;
}
static LTE_RET_E huawei4G_reset(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte reset success!",__LINE__,__func__);
        return LTE_OK;
    }else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte_reset failed!",__LINE__,__func__);
    }
    return LTE_FAIL;
}

static LTE_RET_E huawei4G_sim_switch_set(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte sim switch set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte sim switch set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;

}
static LTE_RET_E huawei4G_sim_switch_get(void *pvData, char *pcBuf)
{
    int     iRet = 0;
    char    acCiev[32] = "^HVSST:";
    char 	acSimSw[32]={0};
	char    *pcBufTmp = NULL;
	
    if (NULL == pvData)
    {
        return LTE_FAIL;
    }
    
    LTE_SIM_SWITCH_T *pstSimSwitch  = (LTE_SIM_SWITCH_T *)pvData;
	
	
    if(NULL != (pcBufTmp = strstr(pcBuf, acCiev)))
    {
        sscanf(pcBufTmp, "%*[^:]:%s", acSimSw);
		
        sscanf(acSimSw, "%[^,],%*[^,],%*[^,],%*[^,],", acSimSw);
		
    }
    else if(NULL != (pcBufTmp = strstr(pcBuf,"OK")))
    {
    	return LTE_OK;
        
    }
	else
	{	
		return LTE_FAIL;
	}
    pstSimSwitch->iSimSw=atoi(acSimSw);

    return LTE_OK;
}
static LTE_RET_E huawei4G_sim_switch_soft_set(void *pvData, char *pcBuf)
{
    char acCiev[] = "OK";
    
    if(NULL != strstr(pcBuf, acCiev))
    {
        LTE_LOG(LTE_LOG_INFO, "%d,%s:lte sim switch soft set success!",__LINE__,__func__);
        return LTE_OK;
    }
    else
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:lte sim switch soft set failed!",__LINE__,__func__);
    }
    
    return LTE_FAIL;

}

LTE_MODULE_OPS_T stLte_huawei4G_ops_t = {
    .lte_pin_state_get       = huawei4G_pin_state,
    .lte_pin_switch          = huawei4G_pin_switch,    
    .lte_pin_switch_get      = huawei4G_pin_sw_get,
    .lte_pin_unlock          = huawei4G_pin_unlock,
    .lte_pin_attempts        = NULL,
    .lte_pin_code_alter      = huawei4G_pin_alter,
    .lte_signal_get          = huawei4G_signal_get,
    .lte_power_on            = huawei4G_power_on,
    .lte_power_off           = huawei4G_power_off,
    .lte_power_state_get     = NULL,
    .lte_net_type_set        = NULL,
    .lte_net_type_get        = NULL,
    .lte_plmn_register       = NULL,
    .lte_plmn_prefer_set     = NULL,
    .lte_net_work_prepare    = huawei4G_network_prepare,
    .lte_net_work_status_get = NULL,
    .lte_connect             = huawei4G_connect,
    .lte_disconnect          = huawei4G_disconnect,
    .lte_link_state_get      = huawei4G_linkstate_get,
    .lte_imei_get            = huawei4G_imei_get,
    .lte_cellid_get          = huawei4G_cellid_get,
    .lte_iccid_get           = huawei4G_iccid_get,
    .lte_imsi_get            = huawei4G_imsi_get,
    .lte_sim_state_get       = huawei4G_sim_state_get,
    .lte_info_get            = huawei4G_lte_info_get,
    .lte_access_net_get      = huawei4G_access_net_get,
    .lte_bands_get           = NULL,
    .lte_clck_bands          = NULL,
    .lte_clck_cell_get       = NULL,
    .lte_clck_cell           = huawei4G_clck_cellid_set,
    .lte_gps_start           = NULL, 
    .lte_gps_stop            = NULL,
    .lte_oper_info_get       = huawei4G_oper_info_get,
    .lte_nettype_info_get    = NULL,
    .lte_sms_cent_code_set   = NULL,
    .lte_sms_mode_set        = NULL,
    .lte_sms_func_init       = NULL,
    .lte_sms_recv            = NULL,
    .lte_sms_send            = NULL,
    .lte_sms_del             = NULL,
    .lte_sms_supinfo_get     = NULL,
    .lte_sms_ip_stack_set    = NULL,
    .lte_apn_get             = NULL,
    .lte_apn_set             = huawei4G_apn_set,
    .lte_version_get         = huawei4G_version_get,
    .lte_enodebid_get        = huawei4G_enodeb_id_get,
    .lte_tac_get             = huawei4G_tac_get,
    .lte_lcid_get            = huawei4G_lcid_get,
    .lte_all_cellid_get      = huawei4G_all_cellid_get,
    .lte_mac_set             = NULL,
    .lte_mac_get             = NULL,
    .lte_sim_unlock          = NULL,
    .lte_sim_disable_unlock  = NULL,
    .lte_sim_type_get        = NULL,
    .lte_all_plmn_mode_get   = NULL,
    .lte_sounet_mode_set     = NULL,
    .lte_sim_puk_unlock 	 = NULL,
    .lte_pin_input_remain    = NULL,
    .lte_post_route_set      = huawei4G_post_route_set,
    .lte_nv_datalock         = huawei4G_nv_datalock,
    .lte_post_route_get      = huawei4G_post_route_get,
    .lte_remove_attach       = huawei4G_remove_attachments,
    .lte_qos_set             = huawei4G_qos_set,
    .lte_qos_get             = huawei4G_qos_get,
	.lte_dload_prot_ver_get     = huawei4G_dload_prot_ver_get,
    .lte_dload_ver_info_get     = huawei4G_dload_ver_info_get,
    .lte_nv_backup           = huawei4G_nv_backup,
    .lte_nv_restore          = huawei4G_nv_restore,
    .lte_dload_mode          = huawei4G_dload_mode,
    .lte_data_mode           = huawei4G_data_mode,
    .lte_reset               = huawei4G_reset,
    .lte_sim_switch_set      = huawei4G_sim_switch_set,
    .lte_sim_switch_get      = huawei4G_sim_switch_get,
    .lte_sim_switch_soft_set = huawei4G_sim_switch_soft_set,
};

