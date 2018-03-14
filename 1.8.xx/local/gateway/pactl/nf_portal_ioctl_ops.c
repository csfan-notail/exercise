/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : nf_portal_ioctl_ops.c
  �� �� ��   : ����
  ��    ��   : Zhong Huaqing
  ��������   : 2012��8��15�� ������
  ����޸�   :
  ��������   : ����ߵ�ͨ��ioctl����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��8��15�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>  
#include <unistd.h>
#include <fcntl.h>	
#include <sys/ioctl.h>	

#include <netdb.h>
#include "nf_portal_ioctl.h"
#include "procfs_fops.h"

#include "nf_portal_ioctl_main.h"
#include "nf_portal_ioctl_ops.h"

/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/


/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/
;

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                       *
 *----------------------------------------------*/

/*****************************************************************************
 �� �� ��  : nf_portal_ioctl_init
 ��������  : ��ʼ��ϵͳ���
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��27�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

*****************************************************************************/
INT32 nf_portal_ioctl_init(void)
{    
    INT32 iProcFd = FD_INVALID;
    CHAR acProcFileName[NF_PORTAL_FILE_NAME_SIZE];
    
    snprintf(acProcFileName, NF_PORTAL_FILE_NAME_SIZE, 
            "%s/%s", PROC_ROOT_DIR, NF_PORTAL_PROC_FILE);
            
    iProcFd = open(acProcFileName, O_RDWR);
    if (iProcFd < 0)
    {
        printf("Call %s faild. fd = %d\n", __FUNCTION__, iProcFd);
        return FD_INVALID; 
    }
    
    return iProcFd;
}

/*****************************************************************************
 �� �� ��  : nf_portal_ioctl_destroy
 ��������  : ����ϵͳ���
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��27�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

*****************************************************************************/
AP_ERROR_CODE_E nf_portal_ioctl_destroy(INT32 iProcFd)
{
    close(iProcFd);
    return AP_E_NONE;
}

/*****************************************************************************
 �� �� ��  : nf_portal_ioctl
 ��������  : io�����ӿ�
 �������  : NF_PORTAL_IOCTL_T *pstIoctl  
             NF_PORTAL_IOCTL_CMD_E enCmd  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��27�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

*****************************************************************************/
AP_ERROR_CODE_E nf_portal_ioctl(NF_PORTAL_IOCTL_T *pstIoctl, NF_PORTAL_IOCTL_CMD_E enCmd)
{
    INT32 iProcFd;
	INT32 iRet;

	if (NULL == pstIoctl)
	{
		return AP_E_PARAM;
	}
    
    iProcFd = nf_portal_ioctl_init();
    if (FD_INVALID == iProcFd)
    {
        return AP_E_RESOURCE;
    }
    
    pstIoctl->enCmd = enCmd;
	iRet = ioctl(iProcFd, PROCFS_FOPS_IOCTL_CMD, (void*)pstIoctl);
	if (0 != iRet)
	{
		printf("Call %s failed. ret = %d\n", __FUNCTION__, iRet);
		return AP_E_DEFAULT_FAIL;
	}
	
	nf_portal_ioctl_destroy(iProcFd);
	
	//printf("PROCFS_FOPS_IOCTL_CMD = 0x%x\n", PROCFS_FOPS_IOCTL_CMD);
	return AP_E_NONE;
}


/*****************************************************************************
 �� �� ��  : nf_portal_cmd_get_mod_version
 ��������  : ��ȡ�ں�ģ��İ汾��Ϣ
 �������  : NF_PORTAL_IOCTL_VERSION_T *pstVersion  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��11��6�� ���ڶ�
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

*****************************************************************************/
AP_ERROR_CODE_E nf_portal_cmd_get_mod_version(NF_PORTAL_IOCTL_VERSION_T *pstVersion)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;    
    AP_ERROR_CODE_E enRet;
    
    
    if (NULL == pstVersion)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_GET_VERSION); 
    if (AP_E_NONE == enRet)
    {
        *pstVersion = stIoctl.unData.stVersion;
    }
    
    return enRet;
}

/*****************************************************************************
 �� �� ��  : nf_portal_cmd_set_debug_level
 ��������  : �����ں�ģ������������
 �������  : UINT32 uiDebugLevel  -- 0-7, 7 ��ʾ���������Ϣ
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��11��6�� ���ڶ�
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

*****************************************************************************/
AP_ERROR_CODE_E nf_portal_cmd_set_debug_level(UINT32 uiDebugLevel)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    
    if (uiDebugLevel > NF_PORTAL_DEBUG_LEVEL_MAX)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stDebug.uiDebugLevel = uiDebugLevel;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_SET_DEBUG_LEVEL);
    
    return enRet;
}

/*****************************************************************************
 �� �� ��  : nf_portal_cmd_get_debug_level
 ��������  : ��ȡ�ں�ģ������������
 �������  : UINT32 *puiDebugLevel  -- ���ڴ洢������ֵ
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��11��6�� ���ڶ�
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

*****************************************************************************/
AP_ERROR_CODE_E nf_portal_cmd_get_debug_level(UINT32 *puiDebugLevel)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    
    if (NULL == puiDebugLevel)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_GET_DEBUG_LEVEL);
    *puiDebugLevel = stIoctl.unData.stDebug.uiDebugLevel;
    
    return enRet;
}
AP_ERROR_CODE_E nf_portal_cmd_set_sta_type(NF_PORTAL_IOCTL_STA_TYPE_T *pstIoStaType)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    
    if (NULL == pstIoStaType)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    memcpy(stIoctl.unData.stIoStaType.aucMac,pstIoStaType->aucMac,ETH_ALEN);
    
    stIoctl.unData.stIoStaType.enStaType = pstIoStaType->enStaType;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_SET_STA_TYPE);
    
    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_get_sta_type(NF_PORTAL_IOCTL_STA_TYPE_T *pstIoStaType)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoStaType)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    memcpy(stIoctl.unData.stIoStaType.aucMac,pstIoStaType->aucMac,ETH_ALEN);
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_GET_STA_TYPE);
    pstIoStaType->enStaType = stIoctl.unData.stIoStaType.enStaType;
    
    return enRet;
}
AP_ERROR_CODE_E nf_portal_cmd_get_network_model(NF_PORTAL_IOCTL_NET_MODEL_T *pstIoNetModel)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoNetModel)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_GET_NET_MODEL);
    pstIoNetModel->enNetModel = stIoctl.unData.stIoNetModel.enNetModel;
    
    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_set_network_model(NF_PORTAL_IOCTL_NET_MODEL_T *pstIoNetModel)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoNetModel)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoNetModel.enNetModel = pstIoNetModel->enNetModel;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_SET_NET_MODEL);
    
    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_set_auth_state(NF_PORTAL_IOCTL_AUTH_STATE_T *pstIoAuthState)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    
    if (NULL == pstIoAuthState)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    memcpy(stIoctl.unData.stIoAuthState.aucMac,pstIoAuthState->aucMac,ETH_ALEN);
    //stIoctl.unData.stIoAuthState.uiIp = pstIoAuthState->uiIp;
    
    stIoctl.unData.stIoAuthState.enState = pstIoAuthState->enState;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_SET_AUTH_STATE);
    
    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_get_auth_state(NF_PORTAL_IOCTL_AUTH_STATE_T *pstIoAuthState)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoAuthState)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    memcpy(stIoctl.unData.stIoAuthState.aucMac,pstIoAuthState->aucMac,ETH_ALEN);
    //stIoctl.unData.stIoAuthState.uiIp = pstIoAuthState->uiIp;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_GET_AUTH_STATE);
    pstIoAuthState->enState = stIoctl.unData.stIoAuthState.enState;
    
    return enRet;
}


AP_ERROR_CODE_E nf_portal_cmd_set_host_ip(NF_PORTAL_IOCTL_IP_T *pstIoIp)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    
    if (NULL == pstIoIp)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoIp.uiIp = pstIoIp->uiIp;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_SET_HOST_IP);
    
    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_get_host_ip(NF_PORTAL_IOCTL_IP_T *pstIoIp)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoIp)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_GET_HOST_IP);
    pstIoIp->uiIp = stIoctl.unData.stIoIp.uiIp;
    
    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_set_auth_server_ip(NF_PORTAL_IOCTL_IP_T *pstIoIp)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    
    if (NULL == pstIoIp)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoIp.uiIp = pstIoIp->uiIp;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_SET_AUTH_SERVER_IP);
    
    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_get_auth_server_ip(NF_PORTAL_IOCTL_IP_T *pstIoIp)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoIp)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_GET_AUTH_SERVER_IP);
    pstIoIp->uiIp = stIoctl.unData.stIoIp.uiIp;
    
    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_set_ccs_server_ip(NF_PORTAL_IOCTL_IP_T *pstIoIp)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    
    if (NULL == pstIoIp)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoIp.uiIp = pstIoIp->uiIp;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_SET_CCS_SERVER_IP);
    
    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_get_ccs_server_ip(NF_PORTAL_IOCTL_IP_T *pstIoIp)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoIp)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_GET_CCS_SERVER_IP);
    pstIoIp->uiIp = stIoctl.unData.stIoIp.uiIp;
    
    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_set_snmp_server_ip(NF_PORTAL_IOCTL_IP_T *pstIoIp)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    
    if (NULL == pstIoIp)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoIp.uiIp = pstIoIp->uiIp;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_SET_SNMP_SERVER_IP);
    
    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_get_snmp_server_ip(NF_PORTAL_IOCTL_IP_T *pstIoIp)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoIp)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_GET_SNMP_SERVER_IP);
    pstIoIp->uiIp = stIoctl.unData.stIoIp.uiIp;
    
    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_set_free_auth_sw(NF_PORTAL_IOCTL_SW_T *pstIoSW)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    
    if (NULL == pstIoSW)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoSW.enSW = pstIoSW->enSW;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_SET_FREE_AUTH_SW);
    
    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_get_free_auth_sw(NF_PORTAL_IOCTL_SW_T *pstIoSW)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoSW)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_GET_FREE_AUTH_SW);
    pstIoSW->enSW = stIoctl.unData.stIoSW.enSW;
    
    return enRet;
}


/*begin:2015-9-6 lan free auth*/
AP_ERROR_CODE_E nf_portal_cmd_get_lan_free_auth_sw(NF_PORTAL_IOCTL_SW_T *pstIoSW)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoSW)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_GET_LAN_FREE_AUTH_SW);
    pstIoSW->enSW = stIoctl.unData.stIoSW.enSW;
    
    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_set_lan_free_auth_sw(NF_PORTAL_IOCTL_SW_T *pstIoSW)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoSW)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoSW.enSW = pstIoSW->enSW;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_SET_LAN_FREE_AUTH_SW);
    
    return enRet;
}
/*end:2015-9-6 lan_free_auth*/

/*begin:2015-9-18 net connect*/
AP_ERROR_CODE_E nf_portal_cmd_get_net_connect(NF_PORTAL_IOCTL_SW_T *pstIoSW)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoSW)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_GET_NET_CONNECT);
    pstIoSW->enSW = stIoctl.unData.stIoSW.enSW;
    
    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_set_net_connect(NF_PORTAL_IOCTL_SW_T *pstIoSW)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
       
    if (NULL == pstIoSW)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoSW.enSW = pstIoSW->enSW;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_SET_NET_CONNECT);
    
    return enRet;
}

/*end:2015-9-18 net connect*/

AP_ERROR_CODE_E nf_portal_cmd_get_sta_list(NF_PORTAL_IOCTL_STA_LIST_T *pstIoStaList)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoStaList)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoStaList.enState = pstIoStaList->enState;
    stIoctl.unData.stIoStaList.uiStartNum = pstIoStaList->uiStartNum;
    stIoctl.unData.stIoStaList.uiNum = pstIoStaList->uiNum;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_GET_STA_LIST);
    memcpy(pstIoStaList,&(stIoctl.unData.stIoStaList),sizeof(NF_PORTAL_IOCTL_STA_LIST_T));
    
    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_sta_list_flush(VOID)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_STA_LIST_FLUSH);
    
    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_get_lan_if_name(NF_PORTAL_IOCTL_IF_NAME_T *pstIoIfName)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoIfName)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_GET_LAN_IF_NAME);
    memcpy(pstIoIfName->aucLanIfName,stIoctl.unData.stIoIfName.aucLanIfName,IFNAMSIZ);
    
    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_set_lan_if_name(NF_PORTAL_IOCTL_IF_NAME_T *pstIoIfName)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoIfName)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    memcpy(stIoctl.unData.stIoIfName.aucLanIfName,pstIoIfName->aucLanIfName,IFNAMSIZ);    
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_SET_LAN_IF_NAME);

    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_add_free_auth_port(NF_PORTAL_IOCTL_IF_NAME_T *pstIoIfName)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoIfName)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    memcpy(stIoctl.unData.stIoIfName.aucLanIfName,pstIoIfName->aucLanIfName,IFNAMSIZ);    
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_ADD_FREE_AUTH_PORT);

    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_del_free_auth_port(NF_PORTAL_IOCTL_IF_NAME_T *pstIoIfName)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoIfName)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    memcpy(stIoctl.unData.stIoIfName.aucLanIfName,pstIoIfName->aucLanIfName,IFNAMSIZ);    
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_DEL_FREE_AUTH_PORT);

    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_free_auth_port_flush(VOID)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_FREE_AUTH_PORT_FLUSH);

    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_get_free_auth_port_list(NF_PORTAL_IOCTL_PORT_LIST_T *pstIoPortList)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoPortList)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoPortList.uiNum = pstIoPortList->uiNum;
    stIoctl.unData.stIoPortList.uiStartNum = pstIoPortList->uiStartNum;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_GET_FREE_AUTH_PORT_LIST);
    memcpy(pstIoPortList,&(stIoctl.unData.stIoPortList),sizeof(NF_PORTAL_IOCTL_PORT_LIST_T));
    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_add_dest_ip_baw(NF_PORTAL_IOCTL_IP_BAW_T *pstIoIpBaw)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoIpBaw)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoIpBaw.stIpBaw.uiIp = pstIoIpBaw->stIpBaw.uiIp;
    stIoctl.unData.stIoIpBaw.stIpBaw.usPort = pstIoIpBaw->stIpBaw.usPort;
    stIoctl.unData.stIoIpBaw.stIpBaw.enFlag = pstIoIpBaw->stIpBaw.enFlag;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_ADD_DEST_IP_BAW);

    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_del_dest_ip_baw(NF_PORTAL_IOCTL_IP_BAW_T *pstIoIpBaw)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoIpBaw)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoIpBaw.stIpBaw.uiIp = pstIoIpBaw->stIpBaw.uiIp;
    stIoctl.unData.stIoIpBaw.stIpBaw.usPort = pstIoIpBaw->stIpBaw.usPort;
    stIoctl.unData.stIoIpBaw.stIpBaw.enFlag = pstIoIpBaw->stIpBaw.enFlag;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_DEL_DEST_IP_BAW);

    return enRet;
}
AP_ERROR_CODE_E nf_portal_cmd_flush_dest_ip_baw(NF_PORTAL_IOCTL_IP_BAW_T *pstIoIpBaw)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoIpBaw)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoIpBaw.stIpBaw.enFlag = pstIoIpBaw->stIpBaw.enFlag;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_DEST_IP_BAW_FLUSH);

    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_get_dest_ip_baw_list(NF_PORTAL_IOCTL_IP_BAW_LIST_T *pstIoIpBawList)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoIpBawList)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoIpBawList.uiStartNum = pstIoIpBawList->uiStartNum;
    stIoctl.unData.stIoIpBawList.uiNum = pstIoIpBawList->uiNum;
    stIoctl.unData.stIoIpBawList.enFlag = pstIoIpBawList->enFlag;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_GET_DEST_IP_BAW_LIST);
    memcpy(pstIoIpBawList,&(stIoctl.unData.stIoIpBawList),sizeof(NF_PORTAL_IOCTL_IP_BAW_LIST_T));
    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_add_dest_net_baw(NF_PORTAL_IOCTL_NET_BAW_T *pstIoNetBaw)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoNetBaw)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoNetBaw.stNetBaw.uiNet = pstIoNetBaw->stNetBaw.uiNet;
    stIoctl.unData.stIoNetBaw.stNetBaw.uiMaskNum= pstIoNetBaw->stNetBaw.uiMaskNum;
    stIoctl.unData.stIoNetBaw.stNetBaw.enFlag = pstIoNetBaw->stNetBaw.enFlag;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_ADD_DEST_NET_BAW);

    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_del_dest_net_baw(NF_PORTAL_IOCTL_NET_BAW_T *pstIoNetBaw)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoNetBaw)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoNetBaw.stNetBaw.uiNet = pstIoNetBaw->stNetBaw.uiNet;
    stIoctl.unData.stIoNetBaw.stNetBaw.uiMaskNum= pstIoNetBaw->stNetBaw.uiMaskNum;
    stIoctl.unData.stIoNetBaw.stNetBaw.enFlag = pstIoNetBaw->stNetBaw.enFlag;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_DEL_DEST_NET_BAW);

    return enRet;
}
AP_ERROR_CODE_E nf_portal_cmd_flush_dest_net_baw(NF_PORTAL_IOCTL_NET_BAW_T *pstIoNetBaw)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoNetBaw)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoNetBaw.stNetBaw.enFlag = pstIoNetBaw->stNetBaw.enFlag;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_DEST_NET_BAW_FLUSH);

    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_get_dest_net_baw_list(NF_PORTAL_IOCTL_NET_BAW_LIST_T *pstIoNetBawList)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoNetBawList)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoNetBawList.uiStartNum = pstIoNetBawList->uiStartNum;
    stIoctl.unData.stIoNetBawList.uiNum = pstIoNetBawList->uiNum;
    stIoctl.unData.stIoNetBawList.enFlag = pstIoNetBawList->enFlag;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_GET_DEST_NET_BAW_LIST);
    memcpy(pstIoNetBawList,&(stIoctl.unData.stIoNetBawList),sizeof(NF_PORTAL_IOCTL_NET_BAW_LIST_T));
    return enRet;
}

/*�û�MAC�ڰ��������*/
AP_ERROR_CODE_E nf_portal_cmd_add_mac_baw(NF_PORTAL_IOCTL_MAC_BAW_T *pstIoMacBaw)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoMacBaw)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    memcpy(stIoctl.unData.stIoMacBaw.stMacBaw.aucMac,pstIoMacBaw->stMacBaw.aucMac,ETH_ALEN);
    stIoctl.unData.stIoMacBaw.stMacBaw.enFlag = pstIoMacBaw->stMacBaw.enFlag;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_ADD_MAC_BAW);

    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_del_mac_baw(NF_PORTAL_IOCTL_MAC_BAW_T *pstIoMacBaw)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoMacBaw)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    memcpy(stIoctl.unData.stIoMacBaw.stMacBaw.aucMac,pstIoMacBaw->stMacBaw.aucMac,ETH_ALEN);
    stIoctl.unData.stIoMacBaw.stMacBaw.enFlag = pstIoMacBaw->stMacBaw.enFlag;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_DEL_MAC_BAW);

    return enRet;
}
AP_ERROR_CODE_E nf_portal_cmd_flush_mac_baw(NF_PORTAL_IOCTL_MAC_BAW_T *pstIoMacBaw)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoMacBaw)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoMacBaw.stMacBaw.enFlag = pstIoMacBaw->stMacBaw.enFlag;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_MAC_BAW_FLUSH);

    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_get_mac_baw_list(NF_PORTAL_IOCTL_MAC_BAW_LIST_T *pstIoMacBawList)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoMacBawList)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoMacBawList.uiStartNum = pstIoMacBawList->uiStartNum;
    stIoctl.unData.stIoMacBawList.uiNum = pstIoMacBawList->uiNum;
    stIoctl.unData.stIoMacBawList.enFlag = pstIoMacBawList->enFlag;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_GET_MAC_BAW_LIST);
    memcpy(pstIoMacBawList,&(stIoctl.unData.stIoMacBawList),sizeof(NF_PORTAL_IOCTL_MAC_BAW_LIST_T));
    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_add_user_ip_baw(NF_PORTAL_IOCTL_IP_BAW_T *pstIoIpBaw)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoIpBaw)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoIpBaw.stIpBaw.uiIp = pstIoIpBaw->stIpBaw.uiIp;
    stIoctl.unData.stIoIpBaw.stIpBaw.enFlag = pstIoIpBaw->stIpBaw.enFlag;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_ADD_USER_IP_BAW);

    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_del_user_ip_baw(NF_PORTAL_IOCTL_IP_BAW_T *pstIoIpBaw)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoIpBaw)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoIpBaw.stIpBaw.uiIp = pstIoIpBaw->stIpBaw.uiIp;
    stIoctl.unData.stIoIpBaw.stIpBaw.enFlag = pstIoIpBaw->stIpBaw.enFlag;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_DEL_USER_IP_BAW);

    return enRet;
}
AP_ERROR_CODE_E nf_portal_cmd_flush_user_ip_baw(NF_PORTAL_IOCTL_IP_BAW_T *pstIoIpBaw)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoIpBaw)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoIpBaw.stIpBaw.enFlag = pstIoIpBaw->stIpBaw.enFlag;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_USER_IP_BAW_FLUSH);

    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_get_user_ip_baw_list(NF_PORTAL_IOCTL_IP_BAW_LIST_T *pstIoIpBawList)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoIpBawList)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoIpBawList.uiStartNum = pstIoIpBawList->uiStartNum;
    stIoctl.unData.stIoIpBawList.uiNum = pstIoIpBawList->uiNum;
    stIoctl.unData.stIoIpBawList.enFlag = pstIoIpBawList->enFlag;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_GET_USER_IP_BAW_LIST);
    memcpy(pstIoIpBawList,&(stIoctl.unData.stIoIpBawList),sizeof(NF_PORTAL_IOCTL_IP_BAW_LIST_T));
    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_add_pass_port_num(NF_PORTAL_IOCTL_PORT_NUM_T *pstIoPortNum)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoPortNum)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoPortNum.stPortNum.ucIpProto = pstIoPortNum->stPortNum.ucIpProto;
    stIoctl.unData.stIoPortNum.stPortNum.usPortNum = pstIoPortNum->stPortNum.usPortNum;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_ADD_PASS_PORT_NUM);

    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_del_pass_port_num(NF_PORTAL_IOCTL_PORT_NUM_T *pstIoPortNum)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoPortNum)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoPortNum.stPortNum.ucIpProto = pstIoPortNum->stPortNum.ucIpProto;
    stIoctl.unData.stIoPortNum.stPortNum.usPortNum = pstIoPortNum->stPortNum.usPortNum;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_DEL_PASS_PORT_NUM);

    return enRet;
}
AP_ERROR_CODE_E nf_portal_cmd_flush_pass_port_num(NF_PORTAL_IOCTL_PORT_NUM_T *pstIoPortNum)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoPortNum)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoPortNum.stPortNum.ucIpProto = pstIoPortNum->stPortNum.ucIpProto;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_PASS_PORT_NUM_FLUSH);

    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_get_pass_port_num_list(NF_PORTAL_IOCTL_PORT_NUM_LIST_T *pstIoPortNumList)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoPortNumList)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoPortNumList.uiStartNum = pstIoPortNumList->uiStartNum;
    stIoctl.unData.stIoPortNumList.uiNum = pstIoPortNumList->uiNum;
    stIoctl.unData.stIoPortNumList.ucIpProto = pstIoPortNumList->ucIpProto;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_GET_PASS_PORT_NUM_LIST);
    memcpy(pstIoPortNumList,&(stIoctl.unData.stIoPortNumList),sizeof(NF_PORTAL_IOCTL_PORT_NUM_LIST_T));
    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_add_url_baw(NF_PORTAL_IOCTL_URL_BAW_T *pstIoUrlBaw)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    int iLoopdns = 0;        
    struct hostent *he;        
    struct in_addr *in_addr_temp;
    char **pptr;
    char   str[32];
    if (NULL == pstIoUrlBaw)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    memcpy(stIoctl.unData.stIoUrlBaw.stUrlBaw.aucUrl,pstIoUrlBaw->stUrlBaw.aucUrl,URL_MAX_SIZE);
    stIoctl.unData.stIoUrlBaw.stUrlBaw.enFlag = pstIoUrlBaw->stUrlBaw.enFlag;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_ADD_URL_BAW);
	he = gethostbyname((char *)pstIoUrlBaw->stUrlBaw.aucUrl);            
	if (he != NULL)
	{   
		pptr=he->h_addr_list;
		for(; *pptr!=NULL; pptr++)
		{
			in_addr_temp = (struct in_addr *)*pptr;
			memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
			memcpy(stIoctl.unData.stIoUrlIpBaw.aucUrl,pstIoUrlBaw->stUrlBaw.aucUrl,URL_MAX_SIZE);
			stIoctl.unData.stIoUrlIpBaw.uiIp = ntohl(in_addr_temp->s_addr);
			//printf("gethostbyname url:%s ip:%x %s\n",stIoctl.unData.stIoUrlIpBaw.aucUrl,stIoctl.unData.stIoUrlIpBaw.uiIp, inet_ntop(he->h_addrtype, *pptr, str, sizeof(str)));
			enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_ADD_URL_IP_BAW);
		}
	}

    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_del_url_baw(NF_PORTAL_IOCTL_URL_BAW_T *pstIoUrlBaw)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoUrlBaw)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    memcpy(stIoctl.unData.stIoUrlBaw.stUrlBaw.aucUrl,pstIoUrlBaw->stUrlBaw.aucUrl,URL_MAX_SIZE);
    stIoctl.unData.stIoUrlBaw.stUrlBaw.enFlag = pstIoUrlBaw->stUrlBaw.enFlag;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_DEL_URL_BAW);

    return enRet;
}
AP_ERROR_CODE_E nf_portal_cmd_flush_url_baw(NF_PORTAL_IOCTL_URL_BAW_T *pstIoUrlBaw)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoUrlBaw)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoUrlBaw.stUrlBaw.enFlag = pstIoUrlBaw->stUrlBaw.enFlag;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_URL_BAW_FLUSH);

    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_get_url_baw_list(NF_PORTAL_IOCTL_URL_BAW_LIST_T *pstIoUrlBawList)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoUrlBawList)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoUrlBawList.uiStartNum = pstIoUrlBawList->uiStartNum;
    stIoctl.unData.stIoUrlBawList.uiNum = pstIoUrlBawList->uiNum;
    stIoctl.unData.stIoUrlBawList.enFlag = pstIoUrlBawList->enFlag;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_GET_URL_BAW_LIST);
    memcpy(pstIoUrlBawList,&(stIoctl.unData.stIoUrlBawList),sizeof(NF_PORTAL_IOCTL_URL_BAW_LIST_T));
    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_get_url_ip_baw_list(NF_PORTAL_IOCTL_URL_IP_BAW_LIST_T *pstIoUrlIpBawList)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoUrlIpBawList)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoUrlIpBawList.uiStartNum = pstIoUrlIpBawList->uiStartNum;
    stIoctl.unData.stIoUrlIpBawList.uiNum = pstIoUrlIpBawList->uiNum;
    stIoctl.unData.stIoUrlIpBawList.enFlag = pstIoUrlIpBawList->enFlag;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_GET_URL_IP_BAW_LIST);
    memcpy(pstIoUrlIpBawList,&(stIoctl.unData.stIoUrlIpBawList),sizeof(NF_PORTAL_IOCTL_URL_IP_BAW_LIST_T));
    return enRet;
}


AP_ERROR_CODE_E nf_portal_cmd_add_url_ip_baw(NF_PROTAL_IOCTL_URL_IP_BAW_T *pstIoUrlIpBaw)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoUrlIpBaw)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    memcpy(stIoctl.unData.stIoUrlIpBaw.aucUrl,pstIoUrlIpBaw->aucUrl,URL_MAX_SIZE);
    stIoctl.unData.stIoUrlIpBaw.uiIp = pstIoUrlIpBaw->uiIp;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_ADD_URL_IP_BAW);

    return enRet;
}

/*begin: add@2015.01.31 for wechat connect wifi*/
AP_ERROR_CODE_E nf_portal_cmd_add_wxl_serverip(NF_PORTAL_IOCTL_WXL_SERVERIP_T *pstIoWxlServerIp)
{
	LOCAL NF_PORTAL_IOCTL_T stIoctl;
	AP_ERROR_CODE_E enRet;
	
	if (NULL == pstIoWxlServerIp)
	{
		return AP_E_PARAM;
	}
	
	memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
	stIoctl.unData.stIoWxlServerIp.stWxlServerIp.uiIp = pstIoWxlServerIp->stWxlServerIp.uiIp;
	stIoctl.unData.stIoWxlServerIp.stWxlServerIp.uiMask = pstIoWxlServerIp->stWxlServerIp.uiMask;
	enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_ADD_WXL_SERVERIP);

	return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_del_wxl_serverip(NF_PORTAL_IOCTL_WXL_SERVERIP_T *pstIoWxlServerIp)
{
	LOCAL NF_PORTAL_IOCTL_T stIoctl;
	AP_ERROR_CODE_E enRet;
	
	if (NULL == pstIoWxlServerIp)
	{
		return AP_E_PARAM;
	}
	
	memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
	stIoctl.unData.stIoWxlServerIp.stWxlServerIp.uiIp = pstIoWxlServerIp->stWxlServerIp.uiIp;
	stIoctl.unData.stIoWxlServerIp.stWxlServerIp.uiMask = pstIoWxlServerIp->stWxlServerIp.uiMask;
	enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_DEL_WXL_SERVERIP);

	return enRet;
}
AP_ERROR_CODE_E nf_portal_cmd_get_wxl_serverip_list(NF_PORTAL_IOCTL_WXL_SERVERIP_LIST_T *pstIoWxlServerIpList)
{
	LOCAL NF_PORTAL_IOCTL_T stIoctl;
	AP_ERROR_CODE_E enRet;
	
	if (NULL == pstIoWxlServerIpList)
	{
		return AP_E_PARAM;
	}
	
	memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
	stIoctl.unData.stIoWxlServerIpList.uiStartNum = pstIoWxlServerIpList->uiStartNum;
    stIoctl.unData.stIoWxlServerIpList.uiNum = pstIoWxlServerIpList->uiNum;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_GET_WXL_SERVERIP_LIST);
    memcpy(pstIoWxlServerIpList,&(stIoctl.unData.stIoWxlServerIpList),sizeof(NF_PORTAL_IOCTL_WXL_SERVERIP_LIST_T));

	return enRet;
}
AP_ERROR_CODE_E nf_portal_cmd_flush_wxl_serverip()
{
	LOCAL NF_PORTAL_IOCTL_T stIoctl;
	AP_ERROR_CODE_E enRet;
	
	memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
	enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_WXL_SERVERIP_FLUSH);

	return enRet;
}
/*end: add@2015.01.31 for wechat connect wifi*/

#ifdef HAVE_MULTI_WAN
AP_ERROR_CODE_E nf_portal_cmd_wan_conf_set(NF_PORTAL_IOCTL_WAN_CONF_T *pstIoWanCfg)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoWanCfg)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoWanCfg.uiWanIndex = pstIoWanCfg->uiWanIndex;
	stIoctl.unData.stIoWanCfg.uiEnable = pstIoWanCfg->uiEnable;
	stIoctl.unData.stIoWanCfg.uiBand = pstIoWanCfg->uiBand;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_WAN_CONFIG_SET);

    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_wan_conf_show(NF_PORTAL_IOCTL_WAN_LIST_T *pstIoWanList)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoWanList)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_WAN_LIST_GET);
    memcpy(pstIoWanList,&(stIoctl.unData.stIoWanList),sizeof(NF_PORTAL_IOCTL_WAN_LIST_T));

    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_wan_map_show(NF_PORTAL_IOCTL_WAN_MAP_T *pstIoWanMap)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstIoWanMap)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoWanMap.uiStartNum = pstIoWanMap->uiStartNum;
    stIoctl.unData.stIoWanMap.uiNum = pstIoWanMap->uiNum;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_WAN_MAP_GET);
    memcpy(pstIoWanMap,&(stIoctl.unData.stIoWanMap),sizeof(NF_PORTAL_IOCTL_WAN_MAP_T));
    
    return enRet;
}
#endif

AP_ERROR_CODE_E nf_portal_cmd_get_vlan_flood(BOOL *pstEnable)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstEnable)
    {
        return AP_E_PARAM;
    }
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_GET_VLAN_FLOOD);
    *pstEnable = stIoctl.unData.stIoVlanFlood.bEnable;
    
    return enRet;
}

AP_ERROR_CODE_E nf_portal_cmd_set_vlan_flood(BOOL bEnable)
{
    LOCAL NF_PORTAL_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    memset(&stIoctl, 0, sizeof(NF_PORTAL_IOCTL_T));
    stIoctl.unData.stIoVlanFlood.bEnable = bEnable;
    enRet = nf_portal_ioctl(&stIoctl, NF_PORTAL_IOCTL_CMD_SET_VLAN_FLOOD);

    return enRet;
}

