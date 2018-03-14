/******************************************************************************

                  ��Ȩ���� (C), 2010-2012, �Ϻ�徴�ͨ�ſƼ����޹�˾

 ******************************************************************************
  �� �� ��   : lib_pa.c
  �� �� ��   : ����
  ��    ��   : WeiHonggang
  ��������   : 2013��11��28��
  ����޸�   :
  ��������   : ����ӿ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��11��28��
    ��    ��   : WeiHonggang
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ap_types.h"
#include "nf_portal_public.h"
#include "nf_portal_ioctl.h"
#include "lib_pa.h"
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

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
/* ��ȡ�û���֤״̬0-δ��֤��1-������֤��2-����֤ */
int portal_get_auth_state(unsigned char *pucMac,unsigned int uiIp,unsigned int *puiState)
{
    AP_ERROR_CODE_E enRet;
    NF_PORTAL_IOCTL_AUTH_STATE_T stIoAuthState;

    if (NULL == pucMac)
    {
        return -1;
    }

    if (NULL == puiState)
    {
        return -1;
    }

    memcpy(stIoAuthState.aucMac,pucMac,ETH_ALEN);
    stIoAuthState.uiIp =  ntohl(uiIp);/*�ֽ���ת��*/
    stIoAuthState.enState = *puiState;
    enRet = nf_portal_cmd_get_auth_state(&stIoAuthState);
    if (enRet != AP_E_NONE)
    {
        return -1;
    }
    
    return 0;
} 
 
 
/*�����û���֤״̬0-δ��֤��1-������֤��2-����֤*/
int portal_set_auth_state(unsigned char *pucMac,unsigned int uiIp,unsigned int uiState)
{
    AP_ERROR_CODE_E enRet;
    NF_PORTAL_IOCTL_AUTH_STATE_T stIoAuthState;

    if (NULL == pucMac)
    {
        return -1;
    }

    if ((uiState < STATE_NOT_AUTH) || (uiState > STATE_MAX))
    {
        return -1;
    }

    memcpy(stIoAuthState.aucMac,pucMac,ETH_ALEN);
    stIoAuthState.uiIp =  ntohl(uiIp);/*�ֽ���ת��*/
    stIoAuthState.enState = uiState;
    enRet = nf_portal_cmd_set_auth_state(&stIoAuthState);
    if (enRet != AP_E_NONE)
    {
        return -1;
    }
    
    return 0;
}
/*��������IP��ַ����Ҫ�����ֽ���*/
int portal_set_host_ip(unsigned int uiIp)
{
    AP_ERROR_CODE_E enRet;
    NF_PORTAL_IOCTL_IP_T stIoIp;

    stIoIp.uiIp = ntohl(uiIp);/*�ֽ���ת��*/
    enRet = nf_portal_cmd_set_host_ip(&stIoIp);
    if (enRet != AP_E_NONE)
    {
        return -1;
    }
    
    return 0;
}
/*������֤������IP��ַ����Ҫ�����ֽ���*/
int portal_set_auth_server_ip(unsigned int uiIp)
{
    AP_ERROR_CODE_E enRet;
    NF_PORTAL_IOCTL_IP_T stIoIp;

    stIoIp.uiIp = ntohl(uiIp);/*�ֽ���ת��*/
    enRet = nf_portal_cmd_set_auth_server_ip(&stIoIp);
    if (enRet != AP_E_NONE)
    {
        return -1;
    }
    
    return 0;
}
/*�����豸���������IP��ַ����Ҫ�����ֽ���*/
int portal_set_ccs_server_ip(unsigned int uiIp)
{
    AP_ERROR_CODE_E enRet;
    NF_PORTAL_IOCTL_IP_T stIoIp;

    stIoIp.uiIp = ntohl(uiIp);/*�ֽ���ת��*/
    enRet = nf_portal_cmd_set_ccs_server_ip(&stIoIp);
    if (enRet != AP_E_NONE)
    {
        return -1;
    }
    
    return 0;
}
/*����SNMP������IP��ַ����Ҫ�����ֽ���*/
int portal_set_snmp_server_ip(unsigned int uiIp)
{
    AP_ERROR_CODE_E enRet;
    NF_PORTAL_IOCTL_IP_T stIoIp;

    stIoIp.uiIp = ntohl(uiIp);/*�ֽ���ת��*/
    enRet = nf_portal_cmd_set_snmp_server_ip(&stIoIp);
    if (enRet != AP_E_NONE)
    {
        return -1;
    }
    
    return 0;
}

/*����ȫ������֤���� 0-�ر�,1-����*/
int portal_set_set_free_auth_sw(unsigned int uiSW)
 {
    AP_ERROR_CODE_E enRet;
    NF_PORTAL_IOCTL_SW_T stIoSW;

    stIoSW.enSW = uiSW;
    enRet = nf_portal_cmd_set_free_auth_sw(&stIoSW);
    if (enRet != AP_E_NONE)
    {
        return -1;
    }
    
    return 0;
 }
/*����LAN�˿���*/
int portal_set_lan_if_name(unsigned char *pucIfName)
{
    NF_PORTAL_IOCTL_IF_NAME_T stIoLanIfName;
    AP_ERROR_CODE_E enRet;

    if (NULL == pucIfName)
    {
        return -1;
    }
    memcpy(stIoLanIfName.aucLanIfName,pucIfName,IFNAMSIZ);
    enRet = nf_portal_cmd_set_lan_if_name(&stIoLanIfName);
    if (enRet != AP_E_NONE)
    {
        return -1;
    }
    
    return 0;    
}

/*��ȡ�ն��б�*/
int portal_get_sta_list
    (unsigned int *puiStartNum,
    unsigned int *puiStaNum,
    void *ptr)
{
    AP_ERROR_CODE_E enRet;
    NF_PORTAL_IOCTL_STA_LIST_T stIoStaList;
    PORTAL_STA_INFO_T *pstStaInfo = NULL;
    unsigned int uiIndex;
    if (ptr == NULL)
    {
        return -1;
    }
    stIoStaList.uiStartNum = *puiStartNum;
    stIoStaList.uiNum = *puiStaNum;
    stIoStaList.enState = STATE_AUTH;
    pstStaInfo = (PORTAL_STA_INFO_T *)ptr;

    enRet = nf_portal_cmd_get_sta_list(&stIoStaList);
    if (enRet != AP_E_NONE)
    {
        return -1;
    }

    for (uiIndex = 0; uiIndex < stIoStaList.uiNum; uiIndex++)
    {
        memcpy(pstStaInfo->aucMac,stIoStaList.astSta[uiIndex].aucMac,ETH_ALEN);
        pstStaInfo->uiIp = htonl(stIoStaList.astSta[uiIndex].uiIp);/*�ֽ���ת��*/
        pstStaInfo->uiSec = stIoStaList.astSta[uiIndex].uiSec;
        pstStaInfo->ui4GByteUp = stIoStaList.astSta[uiIndex].astTS[NF_PORTAL_DIR_UP].ui4GByte;
        pstStaInfo->uiByteUp = stIoStaList.astSta[uiIndex].astTS[NF_PORTAL_DIR_UP].uiByte;
        pstStaInfo->ui4GByteDown = stIoStaList.astSta[uiIndex].astTS[NF_PORTAL_DIR_DOWN].ui4GByte;
        pstStaInfo->uiByteDown = stIoStaList.astSta[uiIndex].astTS[NF_PORTAL_DIR_DOWN].uiByte;

        pstStaInfo++;
    }
    *puiStartNum = stIoStaList.uiStartNum;
    *puiStaNum = stIoStaList.uiNum;
    return 0; 
    
}

/* BEGIN:fengjing add 2014-09-10 */
/* ��ȡ�����ն��б� */
int portal_get_all_sta_list
    (unsigned int *puiStartNum,
    unsigned int *puiStaNum,
    void *ptr)
{
    AP_ERROR_CODE_E enRet;
    NF_PORTAL_IOCTL_STA_LIST_T stIoStaList;
    PORTAL_STA_INFO_T *pstStaInfo = NULL;
    unsigned int uiIndex;
    if (ptr == NULL)
    {
        return -1;
    }
    stIoStaList.uiStartNum = *puiStartNum;
    stIoStaList.uiNum = *puiStaNum;
    stIoStaList.enState = STATE_MAX;
    pstStaInfo = (PORTAL_STA_INFO_T *)ptr;

    enRet = nf_portal_cmd_get_sta_list(&stIoStaList);
    if (enRet != AP_E_NONE)
    {
        return -1;
    }

    for (uiIndex = 0; uiIndex < stIoStaList.uiNum; uiIndex++)
    {
        memcpy(pstStaInfo->aucMac,stIoStaList.astSta[uiIndex].aucMac,ETH_ALEN);
        pstStaInfo->uiIp = htonl(stIoStaList.astSta[uiIndex].uiIp);/*�ֽ���ת��*/
        pstStaInfo->uiSec = stIoStaList.astSta[uiIndex].uiSec;
        pstStaInfo->ui4GByteUp = stIoStaList.astSta[uiIndex].astTS[NF_PORTAL_DIR_UP].ui4GByte;
        pstStaInfo->uiByteUp = stIoStaList.astSta[uiIndex].astTS[NF_PORTAL_DIR_UP].uiByte;
        pstStaInfo->ui4GByteDown = stIoStaList.astSta[uiIndex].astTS[NF_PORTAL_DIR_DOWN].ui4GByte;
        pstStaInfo->uiByteDown = stIoStaList.astSta[uiIndex].astTS[NF_PORTAL_DIR_DOWN].uiByte;

        pstStaInfo++;
    }
    *puiStartNum = stIoStaList.uiStartNum;
    *puiStaNum = stIoStaList.uiNum;
    return 0; 
    
}

/* END:fengjing add 2014-09-10 */

/*MAC����������ӣ�ɾ�������*/
int portal_add_mac_black_list(unsigned char *pucMac)
{
    NF_PORTAL_IOCTL_MAC_BAW_T stIoMacBaw;
    AP_ERROR_CODE_E enRet;

    if (NULL == pucMac)
    {
        return -1;
    }

    memcpy(stIoMacBaw.stMacBaw.aucMac,pucMac,ETH_ALEN);
    stIoMacBaw.stMacBaw.enFlag = BLACK;
    enRet = nf_portal_cmd_add_mac_baw(&stIoMacBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }
    
    return 0;
}

int portal_del_mac_black_list(unsigned char *pucMac)
{
    NF_PORTAL_IOCTL_MAC_BAW_T stIoMacBaw;
    AP_ERROR_CODE_E enRet;

    if (NULL == pucMac)
    {
        return -1;
    }

    memcpy(stIoMacBaw.stMacBaw.aucMac,pucMac,ETH_ALEN);
    stIoMacBaw.stMacBaw.enFlag = BLACK;
    enRet = nf_portal_cmd_del_mac_baw(&stIoMacBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }
    
    return 0;
}

int portal_flush_mac_black_list(void)
{
    NF_PORTAL_IOCTL_MAC_BAW_T stIoMacBaw;
    AP_ERROR_CODE_E enRet;

    stIoMacBaw.stMacBaw.enFlag = BLACK;
    enRet = nf_portal_cmd_flush_mac_baw(&stIoMacBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }
    return 0;
}
/*MAC����������ӣ�ɾ�������*/
int portal_add_mac_white_list(unsigned char *pucMac)
{
    NF_PORTAL_IOCTL_MAC_BAW_T stIoMacBaw;
    AP_ERROR_CODE_E enRet;

    if (NULL == pucMac)
    {
        return -1;
    }

    memcpy(stIoMacBaw.stMacBaw.aucMac,pucMac,ETH_ALEN);
    stIoMacBaw.stMacBaw.enFlag = WHITE;
    enRet = nf_portal_cmd_add_mac_baw(&stIoMacBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }
    
    return 0;
}

int portal_del_mac_white_list(unsigned char *pucMac)
{
    NF_PORTAL_IOCTL_MAC_BAW_T stIoMacBaw;
    AP_ERROR_CODE_E enRet;

    if (NULL == pucMac)
    {
        return -1;
    }

    memcpy(stIoMacBaw.stMacBaw.aucMac,pucMac,ETH_ALEN);
    stIoMacBaw.stMacBaw.enFlag = WHITE;
    enRet = nf_portal_cmd_del_mac_baw(&stIoMacBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }
    
    return 0;

}

int portal_flush_mac_white_list(void)
{
    NF_PORTAL_IOCTL_MAC_BAW_T stIoMacBaw;
    AP_ERROR_CODE_E enRet;

    stIoMacBaw.stMacBaw.enFlag = WHITE;
    enRet = nf_portal_cmd_flush_mac_baw(&stIoMacBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }
    return 0;
}

/*����֤MAC����ӣ�ɾ�������*/
int portal_add_mac_free_auth(unsigned char *pucMac)
{
    NF_PORTAL_IOCTL_MAC_BAW_T stIoMacBaw;
    AP_ERROR_CODE_E enRet;

    if (NULL == pucMac)
    {
        return -1;
    }

    memcpy(stIoMacBaw.stMacBaw.aucMac,pucMac,ETH_ALEN);
    stIoMacBaw.stMacBaw.enFlag = FREE_AUTH;
    enRet = nf_portal_cmd_add_mac_baw(&stIoMacBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }
    
    return 0;
}

int portal_del_mac_free_auth(unsigned char *pucMac)
{
    NF_PORTAL_IOCTL_MAC_BAW_T stIoMacBaw;
    AP_ERROR_CODE_E enRet;

    if (NULL == pucMac)
    {
        return -1;
    }

    memcpy(stIoMacBaw.stMacBaw.aucMac,pucMac,ETH_ALEN);
    stIoMacBaw.stMacBaw.enFlag = FREE_AUTH;
    enRet = nf_portal_cmd_del_mac_baw(&stIoMacBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }
    
    return 0;
}

int portal_flush_mac_free_auth(void)
{
    NF_PORTAL_IOCTL_MAC_BAW_T stIoMacBaw;
    AP_ERROR_CODE_E enRet;

    stIoMacBaw.stMacBaw.enFlag = FREE_AUTH;
    enRet = nf_portal_cmd_flush_mac_baw(&stIoMacBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }
    
    return 0;
}

/*����֤IP����ӣ�ɾ�������*/
int portal_add_ip_free_auth(unsigned int uiIp)
{
    NF_PORTAL_IOCTL_IP_BAW_T stIoIpBaw;
    AP_ERROR_CODE_E enRet;

    stIoIpBaw.stIpBaw.uiIp = ntohl(uiIp);/*�ֽ���ת��*/
    stIoIpBaw.stIpBaw.enFlag = FREE_AUTH;

    enRet = nf_portal_cmd_add_user_ip_baw(&stIoIpBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }

    return 0;
}

int portal_del_ip_free_auth(unsigned int uiIp)
{
    NF_PORTAL_IOCTL_IP_BAW_T stIoIpBaw;
    AP_ERROR_CODE_E enRet;

    stIoIpBaw.stIpBaw.uiIp = ntohl(uiIp);/*�ֽ���ת��*/
    stIoIpBaw.stIpBaw.enFlag = FREE_AUTH;

    enRet = nf_portal_cmd_del_user_ip_baw(&stIoIpBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }

    return 0;
}

int portal_flush_ip_free_auth(void)
{
    NF_PORTAL_IOCTL_IP_BAW_T stIoIpBaw;
    AP_ERROR_CODE_E enRet;

    stIoIpBaw.stIpBaw.enFlag = FREE_AUTH;

    enRet = nf_portal_cmd_flush_user_ip_baw(&stIoIpBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }

    return 0;
}

/*Ŀ��IP����������ӣ�ɾ�������*/
int portal_add_dest_ip_black_list(unsigned int uiIp,unsigned short usPortNum)
{
    NF_PORTAL_IOCTL_IP_BAW_T stIoIpBaw;
    AP_ERROR_CODE_E enRet;

    stIoIpBaw.stIpBaw.uiIp = ntohl(uiIp);/*�ֽ���ת��*/
    stIoIpBaw.stIpBaw.usPort = usPortNum;
    stIoIpBaw.stIpBaw.enFlag = BLACK;

    enRet = nf_portal_cmd_add_dest_ip_baw(&stIoIpBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }

    return 0;
}

int portal_del_dest_ip_black_list(unsigned int uiIp,unsigned short usPortNum)
{
    NF_PORTAL_IOCTL_IP_BAW_T stIoIpBaw;
    AP_ERROR_CODE_E enRet;

    stIoIpBaw.stIpBaw.uiIp = ntohl(uiIp);/*�ֽ���ת��*/
    stIoIpBaw.stIpBaw.usPort = usPortNum;
    stIoIpBaw.stIpBaw.enFlag = BLACK;

    enRet = nf_portal_cmd_del_dest_ip_baw(&stIoIpBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }

    return 0;
}

int portal_flush_dest_ip_black_list(void)
{
    NF_PORTAL_IOCTL_IP_BAW_T stIoIpBaw;
    AP_ERROR_CODE_E enRet;

    stIoIpBaw.stIpBaw.enFlag = BLACK;

    enRet = nf_portal_cmd_flush_dest_ip_baw(&stIoIpBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }

    return 0;
}

/*Ŀ��IP����������ӣ�ɾ�������*/
int portal_add_dest_ip_white_list(unsigned int uiIp,unsigned short usPortNum)
{
    NF_PORTAL_IOCTL_IP_BAW_T stIoIpBaw;
    AP_ERROR_CODE_E enRet;

    stIoIpBaw.stIpBaw.uiIp = ntohl(uiIp);/*�ֽ���ת��*/
    stIoIpBaw.stIpBaw.usPort = usPortNum;
    stIoIpBaw.stIpBaw.enFlag = WHITE;

    enRet = nf_portal_cmd_add_dest_ip_baw(&stIoIpBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }

    return 0;
}

int portal_del_dest_ip_white_list(unsigned int uiIp,unsigned short usPortNum)
{
    NF_PORTAL_IOCTL_IP_BAW_T stIoIpBaw;
    AP_ERROR_CODE_E enRet;

    stIoIpBaw.stIpBaw.uiIp = ntohl(uiIp);/*�ֽ���ת��*/
    stIoIpBaw.stIpBaw.usPort = usPortNum;
    stIoIpBaw.stIpBaw.enFlag = WHITE;

    enRet = nf_portal_cmd_del_dest_ip_baw(&stIoIpBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }

    return 0;
}

int portal_flush_dest_ip_white_list(void)
{
    NF_PORTAL_IOCTL_IP_BAW_T stIoIpBaw;
    AP_ERROR_CODE_E enRet;

    stIoIpBaw.stIpBaw.enFlag = WHITE;

    enRet = nf_portal_cmd_flush_dest_ip_baw(&stIoIpBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }

    return 0;
}
/*begin: add@2014.11.20 for must redirect dest-ip*/
/*Ŀ��IP����������ӣ�ɾ�������*/
int portal_add_dest_ip_must_redirect_list(unsigned int uiIp,unsigned short usPortNum)
{
    NF_PORTAL_IOCTL_IP_BAW_T stIoIpBaw;
    AP_ERROR_CODE_E enRet;

    stIoIpBaw.stIpBaw.uiIp = ntohl(uiIp);/*�ֽ���ת��*/
    stIoIpBaw.stIpBaw.usPort = usPortNum;
    stIoIpBaw.stIpBaw.enFlag = MUST_REDIRECT;

    enRet = nf_portal_cmd_add_dest_ip_baw(&stIoIpBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }

    return 0;
}

int portal_del_dest_ip_must_redirect_list(unsigned int uiIp,unsigned short usPortNum)
{
    NF_PORTAL_IOCTL_IP_BAW_T stIoIpBaw;
    AP_ERROR_CODE_E enRet;

    stIoIpBaw.stIpBaw.uiIp = ntohl(uiIp);/*�ֽ���ת��*/
    stIoIpBaw.stIpBaw.usPort = usPortNum;
    stIoIpBaw.stIpBaw.enFlag = MUST_REDIRECT;

    enRet = nf_portal_cmd_del_dest_ip_baw(&stIoIpBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }

    return 0;
}

int portal_flush_dest_ip_must_redirect_list(void)
{
    NF_PORTAL_IOCTL_IP_BAW_T stIoIpBaw;
    AP_ERROR_CODE_E enRet;

    stIoIpBaw.stIpBaw.enFlag = MUST_REDIRECT;

    enRet = nf_portal_cmd_flush_dest_ip_baw(&stIoIpBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }

    return 0;
}
/*end: add@2014.11.20 for must redirect dest-ip*/

/*URL ����������ӣ�ɾ�������*/
int portal_add_url_black_list(unsigned char *pucUrl)
{
    NF_PORTAL_IOCTL_URL_BAW_T stIoUrlBaw;
    AP_ERROR_CODE_E enRet;

    if (NULL == pucUrl)
    {
        return -1;
    }

    memcpy(stIoUrlBaw.stUrlBaw.aucUrl,pucUrl,URL_MAX_SIZE);
    stIoUrlBaw.stUrlBaw.enFlag = BLACK;

    enRet = nf_portal_cmd_add_url_baw(&stIoUrlBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }

    return 0;
}

int portal_del_url_black_list(unsigned char *pucUrl)
{
    NF_PORTAL_IOCTL_URL_BAW_T stIoUrlBaw;
    AP_ERROR_CODE_E enRet;

    if (NULL == pucUrl)
    {
        return -1;
    }

    memcpy(stIoUrlBaw.stUrlBaw.aucUrl,pucUrl,URL_MAX_SIZE);
    stIoUrlBaw.stUrlBaw.enFlag = BLACK;

    enRet = nf_portal_cmd_del_url_baw(&stIoUrlBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }

    return 0;
}

int portal_flush_url_black_list(void)
{
    NF_PORTAL_IOCTL_URL_BAW_T stIoUrlBaw;
    AP_ERROR_CODE_E enRet;

    stIoUrlBaw.stUrlBaw.enFlag = BLACK;

    enRet = nf_portal_cmd_flush_url_baw(&stIoUrlBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }

    return 0;
}

/*begin: add@2014.11.02 for �����ض���url����*/
int portal_add_url_must_redirect_list(unsigned char *pucUrl)
{
    NF_PORTAL_IOCTL_URL_BAW_T stIoUrlBaw;
    AP_ERROR_CODE_E enRet;

    if (NULL == pucUrl)
    {
        return -1;
    }

    memcpy(stIoUrlBaw.stUrlBaw.aucUrl,pucUrl,URL_MAX_SIZE);
    stIoUrlBaw.stUrlBaw.enFlag = MUST_REDIRECT;

    enRet = nf_portal_cmd_add_url_baw(&stIoUrlBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }

    return 0;
}

int portal_del_url_must_redirect_list(unsigned char *pucUrl)
{
    NF_PORTAL_IOCTL_URL_BAW_T stIoUrlBaw;
    AP_ERROR_CODE_E enRet;

    if (NULL == pucUrl)
    {
        return -1;
    }

    memcpy(stIoUrlBaw.stUrlBaw.aucUrl,pucUrl,URL_MAX_SIZE);
    stIoUrlBaw.stUrlBaw.enFlag = MUST_REDIRECT;

    enRet = nf_portal_cmd_del_url_baw(&stIoUrlBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }

    return 0;
}

int portal_flush_url_must_redirect_list(void)
{
    NF_PORTAL_IOCTL_URL_BAW_T stIoUrlBaw;
    AP_ERROR_CODE_E enRet;

    stIoUrlBaw.stUrlBaw.enFlag = MUST_REDIRECT;

    enRet = nf_portal_cmd_flush_url_baw(&stIoUrlBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }

    return 0;
}

/*end: add@2014.11.02 for �����ض���url����*/

/*URL ����������ӣ�ɾ�������*/
int portal_add_url_white_list(unsigned char *pucUrl)
{
    NF_PORTAL_IOCTL_URL_BAW_T stIoUrlBaw;
    AP_ERROR_CODE_E enRet;

    if (NULL == pucUrl)
    {
        return -1;
    }

    memcpy(stIoUrlBaw.stUrlBaw.aucUrl,pucUrl,URL_MAX_SIZE);
    stIoUrlBaw.stUrlBaw.enFlag = WHITE;

    enRet = nf_portal_cmd_add_url_baw(&stIoUrlBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }

    return 0;
}

int portal_del_url_white_list(unsigned char *pucUrl)
{
    NF_PORTAL_IOCTL_URL_BAW_T stIoUrlBaw;
    AP_ERROR_CODE_E enRet;

    if (NULL == pucUrl)
    {
        return -1;
    }

    memcpy(stIoUrlBaw.stUrlBaw.aucUrl,pucUrl,URL_MAX_SIZE);
    stIoUrlBaw.stUrlBaw.enFlag = WHITE;

    enRet = nf_portal_cmd_del_url_baw(&stIoUrlBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }

    return 0;
}

int portal_flush_url_white_list(void)
{
    NF_PORTAL_IOCTL_URL_BAW_T stIoUrlBaw;
    AP_ERROR_CODE_E enRet;

    stIoUrlBaw.stUrlBaw.enFlag = WHITE;

    enRet = nf_portal_cmd_flush_url_baw(&stIoUrlBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }

    return 0;
}

/*URL����֤��������ӣ�ɾ�������*/
int portal_add_url_free_auth_list(unsigned char *pucUrl)
{
    NF_PORTAL_IOCTL_URL_BAW_T stIoUrlBaw;
    AP_ERROR_CODE_E enRet;

    if (NULL == pucUrl)
    {
        return -1;
    }

    memcpy(stIoUrlBaw.stUrlBaw.aucUrl,pucUrl,URL_MAX_SIZE);
    stIoUrlBaw.stUrlBaw.enFlag = FREE_AUTH;

    enRet = nf_portal_cmd_add_url_baw(&stIoUrlBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }

    return 0;
}

int portal_del_url_free_auth_list(unsigned char *pucUrl)
{
    NF_PORTAL_IOCTL_URL_BAW_T stIoUrlBaw;
    AP_ERROR_CODE_E enRet;

    if (NULL == pucUrl)
    {
        return -1;
    }

    memcpy(stIoUrlBaw.stUrlBaw.aucUrl,pucUrl,URL_MAX_SIZE);
    stIoUrlBaw.stUrlBaw.enFlag = FREE_AUTH;

    enRet = nf_portal_cmd_del_url_baw(&stIoUrlBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }

    return 0;
}

int portal_flush_url_free_auth_list(void)
{
    NF_PORTAL_IOCTL_URL_BAW_T stIoUrlBaw;
    AP_ERROR_CODE_E enRet;

    stIoUrlBaw.stUrlBaw.enFlag = FREE_AUTH;

    enRet = nf_portal_cmd_flush_url_baw(&stIoUrlBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }

    return 0;
}


int portal_add_url_ip_white_list(unsigned char *pucUrl, unsigned int uiIp)
{
    NF_PROTAL_IOCTL_URL_IP_BAW_T stIoUrlIpBaw;
    AP_ERROR_CODE_E enRet;

    if (NULL == pucUrl)
    {
        return -1;
    }

    memcpy(stIoUrlIpBaw.aucUrl,pucUrl,URL_MAX_SIZE);
    stIoUrlIpBaw.uiIp = uiIp;

    enRet = nf_portal_cmd_add_url_ip_baw(&stIoUrlIpBaw);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }

    return 0;
}

/*begin: add@2015.01.31 for wechat connect wifi*/
int portal_add_wxl_serverip(unsigned int uiIp, unsigned int uiMask)
{
    NF_PORTAL_IOCTL_WXL_SERVERIP_T stIoWxlServerIp;
    AP_ERROR_CODE_E enRet;

    stIoWxlServerIp.stWxlServerIp.uiIp = uiIp;
	stIoWxlServerIp.stWxlServerIp.uiMask = uiMask;

    enRet = nf_portal_cmd_add_wxl_serverip(&stIoWxlServerIp);

    if (enRet != AP_E_NONE)
    {
        return -1;
    }

    return 0;
}

int portal_del_wxl_serverip(unsigned int uiIp, unsigned int uiMask)
{
	NF_PORTAL_IOCTL_WXL_SERVERIP_T stIoWxlServerIp;
	AP_ERROR_CODE_E enRet;

	stIoWxlServerIp.stWxlServerIp.uiIp = uiIp;
	stIoWxlServerIp.stWxlServerIp.uiMask = uiMask;

	enRet = nf_portal_cmd_del_wxl_serverip(&stIoWxlServerIp);

	if (enRet != AP_E_NONE)
	{
		return -1;
	}

	return 0;
}

int portal_flush_wxl_serverip(void)
{
    AP_ERROR_CODE_E enRet;

    enRet = nf_portal_cmd_flush_wxl_serverip();

    if (enRet != AP_E_NONE)
    {
        return -1;
    }

    return 0;
}
/*end: add@2015.01.31 for wechat connect wifi*/

