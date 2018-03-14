/******************************************************************************
* COPYRIGHT GBCOM CO.,LTD   
*******************************************************************************
  �� �� ��   : lte_diagnos.c
  �� �� ��   : ����
  ��    ��   : wangqi
  ��������   : 2015��10��23��
  ��������   : lteģ�������Ϲ���
******************************************************************************/
    
#include <stdio.h>
#include <unistd.h>
#include <string.h>
    
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
    

#include "lte.h"
#include "libusb.h"
#include "lte_log.h"
#include "lte_common.h"



static LTE_RET_E ping(char *paddress)
{
    char acPingCmd[128] = {0};
    char  acGetData[128] = {0};
    FILE *fp = NULL;
    char *pTmp = NULL;
    char acNeedle[32] = {"100% packet loss"};
    sprintf(acPingCmd,"%s %s %s","/tmp/ping -c 2",paddress,"| grep \"packets transmitted\"");
	fp = popen(acPingCmd, "r");
	if ( fp == NULL )
    {
		LTE_LOG(LTE_LOG_ERR, "popen [%s] fail", acPingCmd);
		return LTE_FAIL;
	}
	pTmp = fgets(acGetData, sizeof(acGetData), fp);
	pclose(fp);
    
    if ( pTmp != NULL )
    {
        /*δ�ҵ���˵��ping ����Ч*/
        if ( strstr(acGetData, acNeedle) == NULL )
        {
            return LTE_OK;
        }
    }
    /*�������ping����Ч*/
    return LTE_FAIL;

}


/*�ж�ģ���Ƿ���ڣ��Ƿ��ϵ�*/
static LTE_RET_E lte_module_is_attached(LTE_DEVICE_INFO *pstDevInfo)
{
	int iRet = 0;
	LTE_PLATFORM_E enDeviceNum;

	FILE *fp = NULL;
	char acTmp[128] = {0};
    char acGetData[128] = {0};
    char *pTmp = NULL;

    snprintf(acTmp, sizeof(acTmp), "cat /proc/bus/usb/devices |grep \"Vendor=%04x ProdID=%04x\"", pstDevInfo->stDeviceDesc.usVendor, pstDevInfo->stDeviceDesc.usProduct);
	fp = popen(acTmp, "r");
	if ( fp == NULL )
    {
		LTE_LOG(LTE_LOG_ERR,"%d,%s:cat /proc/bus/usb/devices failed!\n",__LINE__,__func__);
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
/*�ж�usb0/ppp0�˿��Ƿ����*/
static LTE_RET_E network_interface_is_exists(int iModuleType)
{
    char acTmp[128] = {0};
    FILE *fp = NULL;
    char acGetData[128] = {0};
    char *pTmp = NULL;
    if ((iModuleType == M_THINKWILL) ||(iModuleType == M_ZTE)||(iModuleType == M_ZTE_CDMA2000)||(iModuleType == M_THINKWILL_CDMA_MI660) ||
        (iModuleType ==  M_YUGA_CDMA_CEM600) || (iModuleType ==  M_HUAWEI))
    {
		strcpy(acTmp, "ifconfig ppp0 |grep \"Device not found\"");
	}
	else
    {
		strcpy(acTmp, "ifconfig usb0 |grep \"Device not found\"");
	}
	fp = popen(acTmp, "r");
	if ( fp == NULL )
    {
		LTE_LOG(LTE_LOG_ERR,"%d,%s:ifconfig usb0 |grep \"Device not found\" failed!\n",__LINE__,__func__);
		return LTE_FAIL;
	}
	pTmp = fgets(acGetData, sizeof(acGetData), fp);
	pclose(fp);

    if ( (pTmp != NULL) && (strlen(pTmp) != 0))
    {
        return LTE_FAIL;
    }
  
    return LTE_OK;
}

/*�ж�ltecomsev�����Ƿ����*/
static LTE_RET_E process_is_exists(char *pcShallCmd, char *pcProcessName )
{
    FILE *fp = NULL;
    char acGetData[128] = {0};
    char *pTmp = NULL;

    fp = popen(pcShallCmd, "r");
	if ( fp == NULL )
    {
		LTE_LOG(LTE_LOG_ERR,"%d,%s:%s failed!\n",__LINE__,__func__, pcShallCmd);
		return LTE_FAIL;
	}
	while((pTmp = fgets(acGetData, sizeof(acGetData), fp)) != NULL)
	{
	    //printf("pTmp==%s\d", pTmp);
        if(NULL != strstr(acGetData, pcProcessName))
        {
            pclose(fp);
            return LTE_OK;
        }
	}
	pclose(fp);
  
    return LTE_FAIL;
}
/*���sim���Ƿ����*/

/*����Ƿ�פ������*/

/*����ź�ֵ*/

/*����Ƿ�������*/

/*�ж�Ĭ��·���Ƿ����*/

LTE_DIAGNOSE_STATE_E lte_diagnose(void)
{
    
    LTE_DEVICE_INFO stDevInfo = {0};
    LTE_SIM_STATE_T stSimState = {0};
    LTE_PIN_STATE_T stPinState = {0};
    LTE_SIGNAL_T stSignal = {0};
    LTE_NETWORK_PREPARE_T stNWStatus = {0};
    LTE_LINKSTATE_T stLinkState = {0};
    if(LTE_OK == ping("baidu.com"))
    {
        return LTE_E_NONE;//����ͨ
    }
    if(LTE_OK == ping("8.8.8.8"))
    {
        return LTE_E_DNS;//DNS������
    }
    /*ȷ��ģ��*/
	if(LTE_OK != lte_device_info_get(&stDevInfo))
	{
	    LTE_LOG(LTE_LOG_ERR, "%d,%s: Get module type failed!",__LINE__,__func__);
	    return LTE_E_NO_MODULE;//û�ҵ�ģ��
	}
    /*�ж�ģ���Ƿ���ڣ��Ƿ��ϵ�*/
    if(LTE_OK != lte_module_is_attached(&stDevInfo))
    {
        LTE_LOG(LTE_LOG_ERR,"%d,%s:module hasn't find!\n",__LINE__,__func__);
        return LTE_E_NO_ATTACHED;//û�ҵ�ģ��
    }
	/*�ж�usb0/ppp0�˿��Ƿ����*/
	if(LTE_OK != network_interface_is_exists(stDevInfo.enDeviceNum))
	{
        return LTE_E_NO_NET_INTERFACE;
	}
	/*�ж�ltecomsev�����Ƿ����*/
	if(LTE_OK != process_is_exists("ps -aux |grep comsev", "ltecomsev"))
	{
        LTE_LOG(LTE_LOG_ERR, "%d,%s:ltecomsev process isn't exist!",__LINE__,__func__);
        return LTE_E_NO_LTECOMSEV;
	}

	/*���sim���Ƿ����*/
	if(LTE_OK != lte_sim_state_get(&stSimState))
	{
        LTE_LOG(LTE_LOG_ERR, "%d,%s:lte_sim_state_get fail!",__LINE__,__func__);
        return LTE_E_DEFAULT_FAIL;
	}
	if(LTE_SIM_READY != stSimState.enSimState)
	{
	    LTE_LOG(LTE_LOG_ERR, "%d,%s:SIM card don't found !",__LINE__,__func__);
        return LTE_E_NO_SIM;//û��SIM��
	}
	if(LTE_OK != lte_pin_state_get(&stPinState))
	{
        LTE_LOG(LTE_LOG_ERR, "%d,%s:lte_pin_state_get fail!",__LINE__,__func__);
        return LTE_E_DEFAULT_FAIL;
	}
	if(stPinState.enState == PIN_STATE_PUK)
	{
        return LTE_E_NO_SIM;//PUK״̬
	}
	/*����ź�ֵ*/
	if(LTE_OK != lte_signal_get(&stSignal))
	{
        LTE_LOG(LTE_LOG_ERR, "%d,%s:lte_signal_get fail!",__LINE__,__func__);
        return LTE_E_DEFAULT_FAIL;
	}
	if(stSignal.uiSignal == 0)
	{
        return LTE_E_SIGNAL_LOW;
	}
	/*����Ƿ�פ������*/
	if (LTE_OK != lte_network_prepare_get(&stNWStatus))
	{
        LTE_LOG(LTE_LOG_ERR, "%d,%s:lte_network_prepare_get fail!",__LINE__,__func__);
        return LTE_E_RESIDENT_NET_FAIL;
	}
	/*����Ƿ�������*/
	if(LTE_OK != lte_linkstate_get(&stLinkState))
	{
        LTE_LOG(LTE_LOG_ERR, "%d,%s:lte_linkstate_get fail!",__LINE__,__func__);
        return LTE_E_DEFAULT_FAIL;
	}
	if(LTE_LINKSTATE_CONNECTED != stLinkState.enLinkstate)
	{
        return LTE_E_NO_DIAL;
	}
	/*�ж�Ĭ��·���Ƿ����*/
	if(LTE_OK != process_is_exists("route |grep default", "default"))
	{
        LTE_LOG(LTE_LOG_ERR, "%d,%s:default route isn't exist!",__LINE__,__func__);
        return LTE_E_NO_DEFAULT_ROUTE;
	}
	return LTE_E_NONE;
}
