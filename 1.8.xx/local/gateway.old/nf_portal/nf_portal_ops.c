/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : nf_portal_ops.c
  版 本 号   : 初稿
  作    者   : Zhong Huaqing
  生成日期   : 2012年8月15日 星期三
  最近修改   :
  功能描述   : 
  函数列表   :
  
  修改历史   :
  1.日    期   : 2012年8月15日 星期三
    作    者   : Zhong Huaqing
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "nf_portal_main.h"

#include "nf_portal_ioctl.h"
#include "nf_portal_ops.h"
#include "nf_portal_hash.h"
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

/*----------------------------------------------*
 * 函数定义                                       *
 *----------------------------------------------*/


AP_ERROR_CODE_E nf_portal_ioctl_get_free_auth_sw(NF_PORTAL_IOCTL_SW_T *pstIoSW)
{
    if (unlikely(NULL == pstIoSW))
    {
        return AP_E_PARAM;
    }
    pstIoSW->enSW = nf_portal_get_free_auth_sw();
    return AP_E_NONE;
}

AP_ERROR_CODE_E nf_portal_ioctl_set_free_auth_sw(NF_PORTAL_IOCTL_SW_T *pstIoSW)
{
    if (unlikely(NULL == pstIoSW))
    {
        return AP_E_PARAM;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"sw:%u\n",pstIoSW->enSW);
    return  nf_portal_set_free_auth_sw(pstIoSW->enSW);
}

AP_ERROR_CODE_E nf_portal_ioctl_set_host_ip(NF_PORTAL_IOCTL_IP_T *pstIoIp)
{
    if (unlikely(NULL == pstIoIp))
    {
        return AP_E_PARAM;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"host_ip:"IP_FMT"\n",IP_ARG(&(pstIoIp->uiIp)));
    return nf_portal_set_host_ip(pstIoIp->uiIp);
}

AP_ERROR_CODE_E nf_portal_ioctl_get_host_ip(NF_PORTAL_IOCTL_IP_T *pstIoIp)
{
    if (unlikely(NULL == pstIoIp))
    {
        return AP_E_PARAM;
    }
    pstIoIp->uiIp = nf_portal_get_host_ip();
    return AP_E_NONE;
}

AP_ERROR_CODE_E nf_portal_ioctl_set_auth_server_ip(NF_PORTAL_IOCTL_IP_T *pstIoIp)
{
    if (unlikely(NULL == pstIoIp))
    {
        return AP_E_PARAM;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"server_ip:"IP_FMT"\n",IP_ARG(&(pstIoIp->uiIp)));
    return nf_portal_set_auth_server_ip(pstIoIp->uiIp);
}

AP_ERROR_CODE_E nf_portal_ioctl_get_auth_server_ip(NF_PORTAL_IOCTL_IP_T *pstIoIp)
{
    if (unlikely(NULL == pstIoIp))
    {
        return AP_E_PARAM;
    }
    pstIoIp->uiIp = nf_portal_get_auth_server_ip();
    return AP_E_NONE;
}
AP_ERROR_CODE_E nf_portal_ioctl_set_ccs_server_ip(NF_PORTAL_IOCTL_IP_T *pstIoIp)
{
    if (unlikely(NULL == pstIoIp))
    {
        return AP_E_PARAM;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"ccs_ip:"IP_FMT"\n",IP_ARG(&(pstIoIp->uiIp)));
    return nf_portal_set_ccs_server_ip(pstIoIp->uiIp);
}

AP_ERROR_CODE_E nf_portal_ioctl_get_ccs_server_ip(NF_PORTAL_IOCTL_IP_T *pstIoIp)
{
    if (unlikely(NULL == pstIoIp))
    {
        return AP_E_PARAM;
    }
    pstIoIp->uiIp = nf_portal_get_ccs_server_ip();
    return AP_E_NONE;
}
AP_ERROR_CODE_E nf_portal_ioctl_set_snmp_server_ip(NF_PORTAL_IOCTL_IP_T *pstIoIp)
{
    if (unlikely(NULL == pstIoIp))
    {
        return AP_E_PARAM;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"snmp_ip:"IP_FMT"\n",IP_ARG(&(pstIoIp->uiIp)));
    return nf_portal_set_snmp_server_ip(pstIoIp->uiIp);
}

AP_ERROR_CODE_E nf_portal_ioctl_get_snmp_server_ip(NF_PORTAL_IOCTL_IP_T *pstIoIp)
{
    if (unlikely(NULL == pstIoIp))
    {
        return AP_E_PARAM;
    }
    pstIoIp->uiIp = nf_portal_get_snmp_server_ip();
    return AP_E_NONE;
}
AP_ERROR_CODE_E nf_portal_ioctl_set_auth_state(NF_PORTAL_IOCTL_AUTH_STATE_T *pstIoAuthState)
{
    NF_PORTAL_STA_ENTRY_T *pstEntry = NULL;
    if (unlikely(NULL == pstIoAuthState))
    {
        return AP_E_PARAM;
    }

    pstEntry = nf_portal_get_sta_from_htable(pstIoAuthState->aucMac,pstIoAuthState->uiIp);

    if (NULL == pstEntry)
    {
        pstEntry = nf_portal_add_sta_to_htable(pstIoAuthState->aucMac,pstIoAuthState->uiIp);
   
        if (unlikely(NULL == pstEntry))
        {
            NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"["MAC_FMT","IP_FMT"] not exist,return\n",MAC_ARG(pstIoAuthState->aucMac),\
                IP_ARG(&(pstIoAuthState->uiIp)));
            return AP_E_PARAM;
        } 
    }
	else{
		if(3 == pstIoAuthState->enState)
			nf_portal_del_sta_from_htable(pstEntry);
	}
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"["MAC_FMT","IP_FMT"]:%d\n",MAC_ARG(pstIoAuthState->aucMac),\
        IP_ARG(&(pstIoAuthState->uiIp)),pstIoAuthState->enState);
    return nf_portal_auth_state_update(pstEntry,pstIoAuthState->enState);
}

AP_ERROR_CODE_E nf_portal_ioctl_get_auth_state(NF_PORTAL_IOCTL_AUTH_STATE_T *pstIoAuthState)
{
    NF_PORTAL_STA_ENTRY_T *pstEntry = NULL;
    if (unlikely(NULL == pstIoAuthState))
    {
        return AP_E_PARAM;
    }

    pstEntry = nf_portal_get_sta_from_htable(pstIoAuthState->aucMac,pstIoAuthState->uiIp);
    if (unlikely(NULL == pstEntry))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"["MAC_FMT","IP_FMT"] not exist,return\n",MAC_ARG(pstIoAuthState->aucMac),\
            IP_ARG(&(pstIoAuthState->uiIp)));
        return AP_E_PARAM;
    }
    
    pstIoAuthState->enState = nf_portal_get_auth_state(pstEntry);
    return AP_E_NONE;
}

AP_ERROR_CODE_E nf_portal_ioctl_get_sta_list(NF_PORTAL_IOCTL_STA_LIST_T *pstIoStaList)
{
    if (unlikely(NULL == pstIoStaList))
    {
        return AP_E_PARAM;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"start num:%u,num:%u,state:%u\n",pstIoStaList->uiStartNum,\
        pstIoStaList->uiNum,pstIoStaList->enState);
    return nf_portal_get_sta_list(pstIoStaList);
}

AP_ERROR_CODE_E nf_portal_ioctl_set_lan_if_name(NF_PORTAL_IOCTL_IF_NAME_T *pstIoIfName)
{
    if (unlikely(NULL == pstIoIfName))
    {
        return AP_E_PARAM;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"lan if name:%s\n",pstIoIfName->aucLanIfName);
    return nf_portal_set_lan_name(pstIoIfName->aucLanIfName);
}

AP_ERROR_CODE_E nf_portal_ioctl_get_lan_if_name(NF_PORTAL_IOCTL_IF_NAME_T *pstIoIfName)
{
    if (unlikely(NULL == pstIoIfName))
    {
        return AP_E_PARAM;
    }
    
    memcpy(pstIoIfName->aucLanIfName,nf_portal_get_lan_name(),IFNAMSIZ);
    return AP_E_NONE;
}

AP_ERROR_CODE_E nf_portal_ioctl_dest_ip_baw_add(NF_PORTAL_IOCTL_IP_BAW_T *pstIoIpBaw)
{
    if (unlikely(NULL == pstIoIpBaw))
    {
        return AP_E_PARAM;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"ip:"IP_FMT":%u,flag:%u\n",IP_ARG(&(pstIoIpBaw->stIpBaw.uiIp)),\
        pstIoIpBaw->stIpBaw.usPort,pstIoIpBaw->stIpBaw.enFlag);
    return nf_portal_dest_ip_baw_add(pstIoIpBaw->stIpBaw.uiIp,pstIoIpBaw->stIpBaw.usPort,pstIoIpBaw->stIpBaw.enFlag);
}

AP_ERROR_CODE_E nf_portal_ioctl_dest_ip_baw_del(NF_PORTAL_IOCTL_IP_BAW_T *pstIoIpBaw)
{
    NF_PORTAL_IP_BAW_ENTRY_T *pstEntry = NULL;
    if (unlikely(NULL == pstIoIpBaw))
    {
        return AP_E_PARAM;
    }
    pstEntry = nf_portal_dest_ip_baw_get(pstIoIpBaw->stIpBaw.uiIp,pstIoIpBaw->stIpBaw.usPort);
    if (NULL == pstEntry)
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"ip:"IP_FMT":%u,flag:%u\n",IP_ARG(&(pstIoIpBaw->stIpBaw.uiIp)),\
        pstIoIpBaw->stIpBaw.usPort,pstIoIpBaw->stIpBaw.enFlag);
        return AP_E_NOT_EXIST;
    }

    if (pstEntry->stIpBAW.enFlag != pstIoIpBaw->stIpBaw.enFlag)
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"ip:"IP_FMT":%u,flag:%u\n",IP_ARG(&(pstIoIpBaw->stIpBaw.uiIp)),\
        pstIoIpBaw->stIpBaw.usPort,pstIoIpBaw->stIpBaw.enFlag);
        return AP_E_NOT_EXIST;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"ip:"IP_FMT":%u,flag:%u\n",IP_ARG(&(pstIoIpBaw->stIpBaw.uiIp)),\
        pstIoIpBaw->stIpBaw.usPort,pstIoIpBaw->stIpBaw.enFlag);
    return nf_portal_dest_ip_baw_del(pstEntry);
}

AP_ERROR_CODE_E nf_portal_ioctl_dest_ip_baw_get_list(NF_PORTAL_IOCTL_IP_BAW_LIST_T *pstIoIpBawList)
{
    if (unlikely(NULL == pstIoIpBawList))
    {
        return AP_E_PARAM;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"start num:%u,num:%u,flag:%u\n",pstIoIpBawList->uiStartNum,\
        pstIoIpBawList->uiNum,pstIoIpBawList->enFlag);
    return nf_portal_dest_ip_baw_get_list(pstIoIpBawList);
}

AP_ERROR_CODE_E nf_portal_ioctl_dest_ip_baw_flush(NF_PORTAL_IOCTL_IP_BAW_T *pstIoIpBaw)
{
    if (unlikely(NULL == pstIoIpBaw))
    {
        return AP_E_PARAM;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"flag:%u\n",pstIoIpBaw->stIpBaw.enFlag);
    return nf_portal_dest_ip_baw_flush(pstIoIpBaw->stIpBaw.enFlag);
}

AP_ERROR_CODE_E nf_portal_ioctl_free_auth_port_add(NF_PORTAL_IOCTL_IF_NAME_T *pstIoIfName)
{
    if (unlikely(NULL == pstIoIfName))
    {
        return AP_E_PARAM;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"port name:%s\n",pstIoIfName->aucLanIfName);
    return nf_portal_free_auth_port_add(pstIoIfName->aucLanIfName);
}

AP_ERROR_CODE_E nf_portal_ioctl_free_auth_port_del(NF_PORTAL_IOCTL_IF_NAME_T *pstIoIfName)
{
    NF_PORTAL_PORT_ENTRY_T *pstEntry = NULL;
    if (unlikely(NULL == pstIoIfName))
    {
        return AP_E_PARAM;
    }
    pstEntry = nf_portal_free_auth_port_get(pstIoIfName->aucLanIfName);
    if (NULL == pstEntry)
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"port name:%s\n",pstIoIfName->aucLanIfName);
        return AP_E_NOT_EXIST;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"port name:%s\n",pstIoIfName->aucLanIfName);
    return nf_portal_free_auth_port_del(pstEntry);
}

AP_ERROR_CODE_E nf_portal_ioctl_free_auth_port_get_list(NF_PORTAL_IOCTL_PORT_LIST_T *pstIoPortList)
{
    if (unlikely(NULL == pstIoPortList))
    {
        return AP_E_PARAM;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"start num:%u,num:%u\n",pstIoPortList->uiStartNum,\
        pstIoPortList->uiNum);
    return nf_portal_free_auth_port_get_list(pstIoPortList);
}

AP_ERROR_CODE_E nf_portal_ioctl_free_auth_port_flush(VOID)
{
    return nf_portal_free_auth_port_flush();
}

AP_ERROR_CODE_E nf_portal_ioctl_sta_list_flush(VOID)
{
    nf_portal_flush_sta_htbale();
    return AP_E_NONE;
}

AP_ERROR_CODE_E nf_portal_ioctl_mac_baw_add(NF_PORTAL_IOCTL_MAC_BAW_T *pstIoMacBaw)
{
    if (unlikely(NULL == pstIoMacBaw))
    {
        return AP_E_PARAM;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"mac:"MAC_FMT",flag = %u\n",MAC_ARG(pstIoMacBaw->stMacBaw.aucMac),pstIoMacBaw->stMacBaw.enFlag);
    return nf_portal_mac_baw_add(pstIoMacBaw->stMacBaw.aucMac,pstIoMacBaw->stMacBaw.enFlag);
}

AP_ERROR_CODE_E nf_portal_ioctl_mac_baw_del(NF_PORTAL_IOCTL_MAC_BAW_T *pstIoMacBaw)
{
    NF_PORTAL_MAC_BAW_ENTRY_T *pstEntry = NULL;
    if (unlikely(NULL == pstIoMacBaw))
    {
        return AP_E_PARAM;
    }
    pstEntry = nf_portal_mac_baw_get(pstIoMacBaw->stMacBaw.aucMac);
    if (NULL == pstEntry)
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"mac:"MAC_FMT",flag = %u\n",MAC_ARG(pstIoMacBaw->stMacBaw.aucMac),pstIoMacBaw->stMacBaw.enFlag);
        return AP_E_NOT_EXIST;
    }
    if(pstIoMacBaw->stMacBaw.enFlag != pstEntry->stMacBAW.enFlag)
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"mac:"MAC_FMT",flag = %u\n",MAC_ARG(pstIoMacBaw->stMacBaw.aucMac),pstIoMacBaw->stMacBaw.enFlag);
        return AP_E_NOT_EXIST;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"mac:"MAC_FMT",flag = %u\n",MAC_ARG(pstIoMacBaw->stMacBaw.aucMac),pstIoMacBaw->stMacBaw.enFlag);
    return nf_portal_mac_baw_del(pstEntry);
}

AP_ERROR_CODE_E nf_portal_ioctl_mac_baw_get_list(NF_PORTAL_IOCTL_MAC_BAW_LIST_T *pstIoMacBawList)
{
    if (unlikely(NULL == pstIoMacBawList))
    {
        return AP_E_PARAM;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"start num:%u,num:%u,flag:%u\n",pstIoMacBawList->uiStartNum,\
        pstIoMacBawList->uiNum,pstIoMacBawList->enFlag);
    return nf_portal_mac_baw_get_list(pstIoMacBawList);
}

AP_ERROR_CODE_E nf_portal_ioctl_mac_baw_flush(NF_PORTAL_IOCTL_MAC_BAW_T *pstIoMacBaw)
{
    if (unlikely(NULL == pstIoMacBaw))
    {
        return AP_E_PARAM;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"flag:%u\n",pstIoMacBaw->stMacBaw.enFlag);
    return nf_portal_mac_baw_flush(pstIoMacBaw->stMacBaw.enFlag);
}

AP_ERROR_CODE_E nf_portal_ioctl_user_ip_baw_add(NF_PORTAL_IOCTL_IP_BAW_T *pstIoIpBaw)
{
    if (unlikely(NULL == pstIoIpBaw))
    {
        return AP_E_PARAM;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"ip:"IP_FMT",flag:%u\n",IP_ARG(&(pstIoIpBaw->stIpBaw.uiIp)),\
            pstIoIpBaw->stIpBaw.enFlag);
    return nf_portal_user_ip_baw_add(pstIoIpBaw->stIpBaw.uiIp,pstIoIpBaw->stIpBaw.enFlag);
}

AP_ERROR_CODE_E nf_portal_ioctl_user_ip_baw_del(NF_PORTAL_IOCTL_IP_BAW_T *pstIoIpBaw)
{
    NF_PORTAL_IP_BAW_ENTRY_T *pstEntry = NULL;
    if (unlikely(NULL == pstIoIpBaw))
    {
        return AP_E_PARAM;
    }
    pstEntry = nf_portal_user_ip_baw_get(pstIoIpBaw->stIpBaw.uiIp);
    if (NULL == pstEntry)
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"ip:"IP_FMT",flag:%u\n",IP_ARG(&(pstIoIpBaw->stIpBaw.uiIp)),\
            pstIoIpBaw->stIpBaw.enFlag);
        return AP_E_NOT_EXIST;
    }

    if (pstEntry->stIpBAW.enFlag != pstIoIpBaw->stIpBaw.enFlag)
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"ip:"IP_FMT",flag:%u\n",IP_ARG(&(pstIoIpBaw->stIpBaw.uiIp)),\
            pstIoIpBaw->stIpBaw.enFlag);
        return AP_E_NOT_EXIST;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"ip:"IP_FMT",flag:%u\n",IP_ARG(&(pstIoIpBaw->stIpBaw.uiIp)),\
            pstIoIpBaw->stIpBaw.enFlag);
    return nf_portal_user_ip_baw_del(pstEntry);
}

AP_ERROR_CODE_E nf_portal_ioctl_user_ip_baw_get_list(NF_PORTAL_IOCTL_IP_BAW_LIST_T *pstIoIpBawList)
{
    if (unlikely(NULL == pstIoIpBawList))
    {
        return AP_E_PARAM;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"start num:%u,num:%u,flag:%u\n",pstIoIpBawList->uiStartNum,\
        pstIoIpBawList->uiNum,pstIoIpBawList->enFlag);
    return nf_portal_user_ip_baw_get_list(pstIoIpBawList);
}

AP_ERROR_CODE_E nf_portal_ioctl_user_ip_baw_flush(NF_PORTAL_IOCTL_IP_BAW_T *pstIoIpBaw)
{
    if (unlikely(NULL == pstIoIpBaw))
    {
        return AP_E_PARAM;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"flag:%u\n",pstIoIpBaw->stIpBaw.enFlag);
    return nf_portal_user_ip_baw_flush(pstIoIpBaw->stIpBaw.enFlag);
}

AP_ERROR_CODE_E nf_portal_ioctl_url_baw_add(NF_PORTAL_IOCTL_URL_BAW_T *pstIoUrlBaw)
{
    if (unlikely(NULL == pstIoUrlBaw))
    {
        return AP_E_PARAM;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"url:%s,flag:%u\n",pstIoUrlBaw->stUrlBaw.aucUrl,pstIoUrlBaw->stUrlBaw.enFlag);
    return nf_portal_url_baw_add(pstIoUrlBaw->stUrlBaw.aucUrl,pstIoUrlBaw->stUrlBaw.enFlag);
}

AP_ERROR_CODE_E nf_portal_ioctl_url_baw_del(NF_PORTAL_IOCTL_URL_BAW_T *pstIoUrlBaw)
{
    NF_PORTAL_URL_BAW_ENTRY_T *pstEntry = NULL;
    if (unlikely(NULL == pstIoUrlBaw))
    {
        return AP_E_PARAM;
    }
    pstEntry = nf_portal_url_baw_get_for_user(pstIoUrlBaw->stUrlBaw.aucUrl);
    if (NULL == pstEntry)
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"url:%s,flag:%u\n",pstIoUrlBaw->stUrlBaw.aucUrl,pstIoUrlBaw->stUrlBaw.enFlag);
        return AP_E_NOT_EXIST;
    }
    if (pstEntry->stUrlBAW.enFlag != pstIoUrlBaw->stUrlBaw.enFlag)
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"url:%s,flag:%u\n",pstIoUrlBaw->stUrlBaw.aucUrl,pstIoUrlBaw->stUrlBaw.enFlag);
        return AP_E_NOT_EXIST;;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"url:%s,flag:%u\n",pstIoUrlBaw->stUrlBaw.aucUrl,pstIoUrlBaw->stUrlBaw.enFlag);
    return nf_portal_url_baw_del(pstEntry);
}

AP_ERROR_CODE_E nf_portal_ioctl_url_baw_get_list(NF_PORTAL_IOCTL_URL_BAW_LIST_T *pstIoUrlBawList)
{
    if (unlikely(NULL == pstIoUrlBawList))
    {
        return AP_E_PARAM;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"start num:%u,num:%u,flag:%u\n",pstIoUrlBawList->uiStartNum,\
        pstIoUrlBawList->uiNum,pstIoUrlBawList->enFlag);
    return nf_portal_url_baw_get_list(pstIoUrlBawList);
}

AP_ERROR_CODE_E nf_portal_ioctl_url_ip_baw_get_list(NF_PORTAL_IOCTL_URL_IP_BAW_LIST_T *pstIoUrlIpBawList)
{
    if (unlikely(NULL == pstIoUrlIpBawList))
    {
        return AP_E_PARAM;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"start num:%u,num:%u,flag:%u\n",pstIoUrlIpBawList->uiStartNum,\
        pstIoUrlIpBawList->uiNum,pstIoUrlIpBawList->enFlag);
    return nf_portal_url_ip_baw_get_list(pstIoUrlIpBawList);
}

AP_ERROR_CODE_E nf_portal_ioctl_url_ip_baw_add(NF_PROTAL_IOCTL_URL_IP_BAW_T *pstIoUrlIpBaw)
{
	NF_PORTAL_URL_BAW_ENTRY_T *pstEntry = NULL;
    if (unlikely(NULL == pstIoUrlIpBaw))
    {
        return AP_E_PARAM;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"url:%s,ip:"IP_FMT"\n",pstIoUrlIpBaw->aucUrl,IP_ARG(&(pstIoUrlIpBaw->uiIp)));
	pstEntry = nf_portal_url_baw_get(pstIoUrlIpBaw->aucUrl);
	if (NULL == pstEntry)
		return AP_E_PARAM;

	return nf_portal_url_ip_baw_add(pstIoUrlIpBaw->uiIp,pstEntry,pstIoUrlIpBaw->aucUrl);
}

AP_ERROR_CODE_E nf_portal_ioctl_url_baw_flush(NF_PORTAL_IOCTL_URL_BAW_T *pstIoUrlBaw)
{
    if (unlikely(NULL == pstIoUrlBaw))
    {
        return AP_E_PARAM;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"flag:%u\n",pstIoUrlBaw->stUrlBaw.enFlag);
    return nf_portal_url_baw_flush(pstIoUrlBaw->stUrlBaw.enFlag);
}

AP_ERROR_CODE_E nf_portal_ioctl_port_num_add(NF_PORTAL_IOCTL_PORT_NUM_T *pstIoPortNum)
{
    if (unlikely(NULL == pstIoPortNum))
    {
        return AP_E_PARAM;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,",Port Num:%u,Proto:%u\n",pstIoPortNum->stPortNum.usPortNum,pstIoPortNum->stPortNum.ucIpProto);
    return nf_portal_port_num_add(pstIoPortNum->stPortNum.usPortNum,pstIoPortNum->stPortNum.ucIpProto);
}

AP_ERROR_CODE_E nf_portal_ioctl_port_num_del(NF_PORTAL_IOCTL_PORT_NUM_T *pstIoPortNum)
{
    NF_PORTAL_PORT_NUM_ENTRY_T *pstEntry = NULL;
    if (unlikely(NULL == pstIoPortNum))
    {
        return AP_E_PARAM;
    }
    pstEntry = nf_portal_port_num_get(pstIoPortNum->stPortNum.usPortNum,pstIoPortNum->stPortNum.ucIpProto);
    if (NULL == pstEntry)
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,",Port Num:%u,Proto:%u\n",pstIoPortNum->stPortNum.usPortNum,pstIoPortNum->stPortNum.ucIpProto);
        return AP_E_NOT_EXIST;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,",Port Num:%u,Proto:%u\n",pstIoPortNum->stPortNum.usPortNum,pstIoPortNum->stPortNum.ucIpProto);
    return nf_portal_port_num_del(pstEntry);
}

AP_ERROR_CODE_E nf_portal_ioctl_port_num_get_list(NF_PORTAL_IOCTL_PORT_NUM_LIST_T *pstIoPortNumList)
{
    if (unlikely(NULL == pstIoPortNumList))
    {
        return AP_E_PARAM;
    }

    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"start num:%u,num:%u,proto:%u\n",pstIoPortNumList->uiStartNum,\
        pstIoPortNumList->uiNum,pstIoPortNumList->ucIpProto);
    return nf_portal_port_num_get_list(pstIoPortNumList);
}

AP_ERROR_CODE_E nf_portal_ioctl_port_num_flush(NF_PORTAL_IOCTL_PORT_NUM_T *pstIoPortNum)
{
    if (unlikely(NULL == pstIoPortNum))
    {
        return AP_E_PARAM;
    }

    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"porto:%u\n",pstIoPortNum->stPortNum.ucIpProto);
    return nf_portal_port_num_flush(pstIoPortNum->stPortNum.ucIpProto);
}
/*begin: add@2015.01.31 for wechat connect wifi*/
AP_ERROR_CODE_E nf_portal_ioctl_wxl_serverip_add(NF_PORTAL_IOCTL_WXL_SERVERIP_T *pstIoWxlServerIp)
{
    if (unlikely(NULL == pstIoWxlServerIp))
    {
        return AP_E_PARAM;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"wxl serverip: %u/%u\n",pstIoWxlServerIp->stWxlServerIp.uiIp,
														     pstIoWxlServerIp->stWxlServerIp.uiMask);
    return nf_portal_wxl_serverip_add(pstIoWxlServerIp->stWxlServerIp.uiIp,pstIoWxlServerIp->stWxlServerIp.uiMask);
}

AP_ERROR_CODE_E nf_portal_ioctl_wxl_serverip_del(NF_PORTAL_IOCTL_WXL_SERVERIP_T *pstIoWxlServerIp)
{
    NF_PORTAL_WXL_SERVERIP_ENTRY_T *pstEntry = NULL;
    if (unlikely(NULL == pstIoWxlServerIp))
    {
        return AP_E_PARAM;
    }
    pstEntry = nf_portal_wxl_serverip_get(pstIoWxlServerIp->stWxlServerIp.uiIp);
    if (NULL == pstEntry)
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"ip: %u , wxl serverip is not exist\n",pstIoWxlServerIp->stWxlServerIp.uiIp);
        return AP_E_NOT_EXIST;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"wxl serverip: %u/%u del\n",pstIoWxlServerIp->stWxlServerIp.uiIp,
														         pstIoWxlServerIp->stWxlServerIp.uiMask);
    return nf_portal_wxl_serverip_del(pstEntry);
}

AP_ERROR_CODE_E nf_portal_ioctl_wxl_serverip_get_list(NF_PORTAL_IOCTL_WXL_SERVERIP_LIST_T *pstIoWxlServerIpList)
{
    if (unlikely(NULL == pstIoWxlServerIpList))
    {
        return AP_E_PARAM;
    }

    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"start num:%u,num:%u\n",pstIoWxlServerIpList->uiStartNum,\
        pstIoWxlServerIpList->uiNum);
    return nf_portal_wxl_serverip_get_list(pstIoWxlServerIpList);
}

AP_ERROR_CODE_E nf_portal_ioctl_wxl_serverip_flush(void)
{
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"flush wxl serverip list\n");
    return nf_portal_wxl_serverip_flush();
}
/*end: add@2015.01.31 for wechat connect wifi*/
