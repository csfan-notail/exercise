/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : lib_pa.c
  版 本 号   : 初稿
  作    者   : WeiHonggang
  生成日期   : 2013年11月28日
  最近修改   :
  功能描述   : 对外接口
  函数列表   :
  修改历史   :
  1.日    期   : 2013年11月28日
    作    者   : WeiHonggang
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ap_types.h"
#include "nf_portal_public.h"
#include "nf_portal_ioctl.h"
#include "lib_pa.h"
/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
/* 获取用户认证状态0-未认证，1-正在认证，2-已认证 */
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
    stIoAuthState.uiIp =  ntohl(uiIp);/*字节序转换*/
    stIoAuthState.enState = *puiState;
    enRet = nf_portal_cmd_get_auth_state(&stIoAuthState);
    if (enRet != AP_E_NONE)
    {
        return -1;
    }
    
    return 0;
} 
 
 
/*设置用户认证状态0-未认证，1-正在认证，2-已认证*/
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
    stIoAuthState.uiIp =  ntohl(uiIp);/*字节序转换*/
    stIoAuthState.enState = uiState;
    enRet = nf_portal_cmd_set_auth_state(&stIoAuthState);
    if (enRet != AP_E_NONE)
    {
        return -1;
    }
    
    return 0;
}
/*设置主机IP地址，需要网络字节序*/
int portal_set_host_ip(unsigned int uiIp)
{
    AP_ERROR_CODE_E enRet;
    NF_PORTAL_IOCTL_IP_T stIoIp;

    stIoIp.uiIp = ntohl(uiIp);/*字节序转换*/
    enRet = nf_portal_cmd_set_host_ip(&stIoIp);
    if (enRet != AP_E_NONE)
    {
        return -1;
    }
    
    return 0;
}
/*设置认证服务器IP地址，需要网络字节序*/
int portal_set_auth_server_ip(unsigned int uiIp)
{
    AP_ERROR_CODE_E enRet;
    NF_PORTAL_IOCTL_IP_T stIoIp;

    stIoIp.uiIp = ntohl(uiIp);/*字节序转换*/
    enRet = nf_portal_cmd_set_auth_server_ip(&stIoIp);
    if (enRet != AP_E_NONE)
    {
        return -1;
    }
    
    return 0;
}
/*设置设备管理服务器IP地址，需要网络字节序*/
int portal_set_ccs_server_ip(unsigned int uiIp)
{
    AP_ERROR_CODE_E enRet;
    NF_PORTAL_IOCTL_IP_T stIoIp;

    stIoIp.uiIp = ntohl(uiIp);/*字节序转换*/
    enRet = nf_portal_cmd_set_ccs_server_ip(&stIoIp);
    if (enRet != AP_E_NONE)
    {
        return -1;
    }
    
    return 0;
}
/*设置SNMP服务器IP地址，需要网络字节序*/
int portal_set_snmp_server_ip(unsigned int uiIp)
{
    AP_ERROR_CODE_E enRet;
    NF_PORTAL_IOCTL_IP_T stIoIp;

    stIoIp.uiIp = ntohl(uiIp);/*字节序转换*/
    enRet = nf_portal_cmd_set_snmp_server_ip(&stIoIp);
    if (enRet != AP_E_NONE)
    {
        return -1;
    }
    
    return 0;
}

/*设置全局免认证开关 0-关闭,1-开启*/
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
/*设置LAN端口名*/
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

/*获取终端列表*/
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
        pstStaInfo->uiIp = htonl(stIoStaList.astSta[uiIndex].uiIp);/*字节序转换*/
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
/* 获取所有终端列表 */
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
        pstStaInfo->uiIp = htonl(stIoStaList.astSta[uiIndex].uiIp);/*字节序转换*/
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

/*MAC黑名单，添加，删除，清空*/
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
/*MAC白名单，添加，删除，清空*/
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

/*免认证MAC，添加，删除，清空*/
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

/*免认证IP，添加，删除，清空*/
int portal_add_ip_free_auth(unsigned int uiIp)
{
    NF_PORTAL_IOCTL_IP_BAW_T stIoIpBaw;
    AP_ERROR_CODE_E enRet;

    stIoIpBaw.stIpBaw.uiIp = ntohl(uiIp);/*字节序转换*/
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

    stIoIpBaw.stIpBaw.uiIp = ntohl(uiIp);/*字节序转换*/
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

/*目的IP黑名单，添加，删除，清空*/
int portal_add_dest_ip_black_list(unsigned int uiIp,unsigned short usPortNum)
{
    NF_PORTAL_IOCTL_IP_BAW_T stIoIpBaw;
    AP_ERROR_CODE_E enRet;

    stIoIpBaw.stIpBaw.uiIp = ntohl(uiIp);/*字节序转换*/
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

    stIoIpBaw.stIpBaw.uiIp = ntohl(uiIp);/*字节序转换*/
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

/*目的IP白名单，添加，删除，清空*/
int portal_add_dest_ip_white_list(unsigned int uiIp,unsigned short usPortNum)
{
    NF_PORTAL_IOCTL_IP_BAW_T stIoIpBaw;
    AP_ERROR_CODE_E enRet;

    stIoIpBaw.stIpBaw.uiIp = ntohl(uiIp);/*字节序转换*/
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

    stIoIpBaw.stIpBaw.uiIp = ntohl(uiIp);/*字节序转换*/
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
/*目的IP白名单，添加，删除，清空*/
int portal_add_dest_ip_must_redirect_list(unsigned int uiIp,unsigned short usPortNum)
{
    NF_PORTAL_IOCTL_IP_BAW_T stIoIpBaw;
    AP_ERROR_CODE_E enRet;

    stIoIpBaw.stIpBaw.uiIp = ntohl(uiIp);/*字节序转换*/
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

    stIoIpBaw.stIpBaw.uiIp = ntohl(uiIp);/*字节序转换*/
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

/*URL 黑名单，添加，删除，清空*/
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

/*begin: add@2014.11.02 for 必须重定向url名单*/
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

/*end: add@2014.11.02 for 必须重定向url名单*/

/*URL 白名单，添加，删除，清空*/
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

/*URL免认证名单，添加，删除，清空*/
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

