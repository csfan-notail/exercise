/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : nf_portal_ioctl.c
  版 本 号   : 初稿
  作    者   : Zhong Huaqing
  生成日期   : 2012年8月15日 星期三
  最近修改   :
  功能描述   : NF_PORTAL模块IO接口，用于和用户态进行交互.
               说明: 本文件中，不实现任何实质性的功能，仅作为一个交互通道。
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
AP_ERROR_CODE_E nf_portal_ioctl_get_version(NF_PORTAL_IOCTL_VERSION_T *pstVersion)
{
    strncpy(pstVersion->acModVersion, NF_PORTAL_MOD_VERSION, NF_PORTAL_VERSION_SIZE);
    strncpy(pstVersion->acModDepVersion, NF_PORTAL_MOD_DEP_VERSION, NF_PORTAL_VERSION_SIZE);
    return AP_E_NONE;
}


LOCAL AP_ERROR_CODE_E nf_portal_ioctl_set_debug_level(NF_PORTAL_IOCTL_DEBUG_T *pstDebug)
{
    if (NULL == pstDebug)
    {
        return AP_E_PARAM;
    }
    return nf_portal_set_debugLevel(pstDebug->uiDebugLevel);
}

LOCAL AP_ERROR_CODE_E nf_portal_ioctl_get_debug_level(NF_PORTAL_IOCTL_DEBUG_T *pstDebug)
{
    if (NULL == pstDebug)
    {
        return AP_E_PARAM;
    }
    pstDebug->uiDebugLevel = nf_portal_get_debugLevel();
    return AP_E_NONE;
}

/*****************************************************************************
 函 数 名  : nf_portal_ioctl_handler
 功能描述  : IO交互统一接口
 输入参数  : void  *pstArg  --用户态空间的指针
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月15日 星期三
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E nf_portal_ioctl_handler(void *pstArg)
{
	LOCAL NF_PORTAL_IOCTL_T stIoctl;
	BOOL bGetOps = FALSE; /*取值为真时，表示需要向用户态复制数据*/
	AP_ERROR_CODE_E enRet = AP_E_NONE;
	
    if (NULL == pstArg)
    {
        return AP_E_PARAM;
    }
    
	if (copy_from_user(&stIoctl, pstArg, sizeof(NF_PORTAL_IOCTL_T)))
	{
	    NF_PORTAL_LOG(NF_PORTAL_LOG_ERR, "Copy from user failed.\n");
		return AP_E_DEFAULT_FAIL;
	}
	NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG, "stIoctl.enCmd = %d\n", stIoctl.enCmd);
	
	
	switch (stIoctl.enCmd)
	{
	    /*移植时，以下三个命令需要保留*/
        case NF_PORTAL_IOCTL_CMD_GET_VERSION: 
            enRet = nf_portal_ioctl_get_version(&stIoctl.unData.stVersion);
            bGetOps = TRUE; 
            break;
            
	    case NF_PORTAL_IOCTL_CMD_SET_DEBUG_LEVEL:
	        enRet = nf_portal_ioctl_set_debug_level(&stIoctl.unData.stDebug); 
	        break;
	    case NF_PORTAL_IOCTL_CMD_GET_DEBUG_LEVEL:
	        enRet = nf_portal_ioctl_get_debug_level(&stIoctl.unData.stDebug); 
	        bGetOps = TRUE;
	        break;
	    case NF_PORTAL_IOCTL_CMD_SET_AUTH_STATE:
               enRet = nf_portal_ioctl_set_auth_state(&stIoctl.unData.stIoAuthState);
               break;
           case NF_PORTAL_IOCTL_CMD_GET_AUTH_STATE:
               enRet = nf_portal_ioctl_get_auth_state(&stIoctl.unData.stIoAuthState);
               bGetOps = TRUE;
               break;
           case NF_PORTAL_IOCTL_CMD_SET_HOST_IP:
               enRet = nf_portal_ioctl_set_host_ip(&stIoctl.unData.stIoIp);
               break;
           case NF_PORTAL_IOCTL_CMD_GET_HOST_IP:
                enRet = nf_portal_ioctl_get_host_ip(&stIoctl.unData.stIoIp);
                bGetOps = TRUE;
                break;
            case NF_PORTAL_IOCTL_CMD_SET_AUTH_SERVER_IP:
                enRet = nf_portal_ioctl_set_auth_server_ip(&stIoctl.unData.stIoIp);
                break;
            case NF_PORTAL_IOCTL_CMD_GET_AUTH_SERVER_IP:
                enRet = nf_portal_ioctl_get_auth_server_ip(&stIoctl.unData.stIoIp);
                bGetOps = TRUE;
                break;
            case NF_PORTAL_IOCTL_CMD_SET_CCS_SERVER_IP:
                enRet = nf_portal_ioctl_set_ccs_server_ip(&stIoctl.unData.stIoIp);
                break;
            case NF_PORTAL_IOCTL_CMD_GET_CCS_SERVER_IP:
                enRet = nf_portal_ioctl_get_ccs_server_ip(&stIoctl.unData.stIoIp);
                bGetOps = TRUE;
                break;
			case NF_PORTAL_IOCTL_CMD_SET_SNMP_SERVER_IP:
                enRet = nf_portal_ioctl_set_snmp_server_ip(&stIoctl.unData.stIoIp);
                break;
            case NF_PORTAL_IOCTL_CMD_GET_SNMP_SERVER_IP:
                enRet = nf_portal_ioctl_get_snmp_server_ip(&stIoctl.unData.stIoIp);
                bGetOps = TRUE;
                break;
            case NF_PORTAL_IOCTL_CMD_SET_FREE_AUTH_SW:
                enRet = nf_portal_ioctl_set_free_auth_sw(&stIoctl.unData.stIoSW);
                break;
            case NF_PORTAL_IOCTL_CMD_GET_FREE_AUTH_SW:
                enRet = nf_portal_ioctl_get_free_auth_sw(&stIoctl.unData.stIoSW);
                bGetOps = TRUE;
                break;
            case NF_PORTAL_IOCTL_CMD_GET_STA_LIST:
                enRet = nf_portal_ioctl_get_sta_list(&stIoctl.unData.stIoStaList);
                bGetOps = TRUE;
                break;
            case NF_PORTAL_IOCTL_CMD_SET_LAN_IF_NAME:
                enRet = nf_portal_ioctl_set_lan_if_name(&stIoctl.unData.stIoIfName);
                break;
            case NF_PORTAL_IOCTL_CMD_GET_LAN_IF_NAME:
                enRet =  nf_portal_ioctl_get_lan_if_name(&stIoctl.unData.stIoIfName);
                bGetOps = TRUE;
                break;
            case NF_PORTAL_IOCTL_CMD_ADD_DEST_IP_BAW:
                enRet = nf_portal_ioctl_dest_ip_baw_add(&stIoctl.unData.stIoIpBaw);
                break;
            case NF_PORTAL_IOCTL_CMD_DEL_DEST_IP_BAW:
                enRet = nf_portal_ioctl_dest_ip_baw_del(&stIoctl.unData.stIoIpBaw);
                break;
            case NF_PORTAL_IOCTL_CMD_GET_DEST_IP_BAW_LIST:
                enRet = nf_portal_ioctl_dest_ip_baw_get_list(&stIoctl.unData.stIoIpBawList);
                bGetOps = TRUE;
                break;
            case NF_PORTAL_IOCTL_CMD_DEST_IP_BAW_FLUSH:
                enRet = nf_portal_ioctl_dest_ip_baw_flush(&stIoctl.unData.stIoIpBaw);
                break;
            case NF_PORTAL_IOCTL_CMD_ADD_FREE_AUTH_PORT:
                enRet = nf_portal_ioctl_free_auth_port_add(&stIoctl.unData.stIoIfName);
                break;
            case NF_PORTAL_IOCTL_CMD_DEL_FREE_AUTH_PORT:
                enRet = nf_portal_ioctl_free_auth_port_del(&stIoctl.unData.stIoIfName);
                break;
            case NF_PORTAL_IOCTL_CMD_GET_FREE_AUTH_PORT_LIST:
                enRet = nf_portal_ioctl_free_auth_port_get_list(&stIoctl.unData.stIoPortList);
                bGetOps = TRUE;
                break;
            case NF_PORTAL_IOCTL_CMD_FREE_AUTH_PORT_FLUSH:
                enRet = nf_portal_ioctl_free_auth_port_flush();
                break;
            case NF_PORTAL_IOCTL_CMD_STA_LIST_FLUSH:
                enRet = nf_portal_ioctl_sta_list_flush();
                break;
            case NF_PORTAL_IOCTL_CMD_ADD_MAC_BAW:
                enRet = nf_portal_ioctl_mac_baw_add(&stIoctl.unData.stIoMacBaw);
                break;
            case NF_PORTAL_IOCTL_CMD_DEL_MAC_BAW:
                enRet = nf_portal_ioctl_mac_baw_del(&stIoctl.unData.stIoMacBaw);
                break;
            case NF_PORTAL_IOCTL_CMD_GET_MAC_BAW_LIST:
                enRet = nf_portal_ioctl_mac_baw_get_list(&stIoctl.unData.stIoMacBawList);
                bGetOps = TRUE;
                break;
            case NF_PORTAL_IOCTL_CMD_MAC_BAW_FLUSH:
                enRet = nf_portal_ioctl_mac_baw_flush(&stIoctl.unData.stIoMacBaw);
                break;
            case NF_PORTAL_IOCTL_CMD_ADD_USER_IP_BAW:
                enRet = nf_portal_ioctl_user_ip_baw_add(&stIoctl.unData.stIoIpBaw);
                break;
            case NF_PORTAL_IOCTL_CMD_DEL_USER_IP_BAW:
                enRet = nf_portal_ioctl_user_ip_baw_del(&stIoctl.unData.stIoIpBaw);
                break;
            case NF_PORTAL_IOCTL_CMD_GET_USER_IP_BAW_LIST:
                enRet = nf_portal_ioctl_user_ip_baw_get_list(&stIoctl.unData.stIoIpBawList);
                bGetOps = TRUE;
                break;
            case NF_PORTAL_IOCTL_CMD_USER_IP_BAW_FLUSH:
                enRet = nf_portal_ioctl_user_ip_baw_flush(&stIoctl.unData.stIoIpBaw);
                break;
            case NF_PORTAL_IOCTL_CMD_ADD_URL_BAW:
                enRet = nf_portal_ioctl_url_baw_add(&stIoctl.unData.stIoUrlBaw);
                break;
            case NF_PORTAL_IOCTL_CMD_DEL_URL_BAW:
                enRet = nf_portal_ioctl_url_baw_del(&stIoctl.unData.stIoUrlBaw);
                break;
            case NF_PORTAL_IOCTL_CMD_GET_URL_BAW_LIST:
                enRet = nf_portal_ioctl_url_baw_get_list(&stIoctl.unData.stIoUrlBawList);
                bGetOps = TRUE;
                break;
            case NF_PORTAL_IOCTL_CMD_GET_URL_IP_BAW_LIST:
                enRet = nf_portal_ioctl_url_ip_baw_get_list(&stIoctl.unData.stIoUrlIpBawList);
                bGetOps = TRUE;
                break;
			case NF_PORTAL_IOCTL_CMD_ADD_URL_IP_BAW:
				enRet = nf_portal_ioctl_url_ip_baw_add(&stIoctl.unData.stIoUrlIpBaw);;
				break;
            case NF_PORTAL_IOCTL_CMD_URL_BAW_FLUSH:
                enRet = nf_portal_ioctl_url_baw_flush(&stIoctl.unData.stIoUrlBaw);
                break;
            case NF_PORTAL_IOCTL_CMD_ADD_PASS_PORT_NUM:
                enRet = nf_portal_ioctl_port_num_add(&stIoctl.unData.stIoPortNum);
                break;
            case NF_PORTAL_IOCTL_CMD_DEL_PASS_PORT_NUM:
                enRet = nf_portal_ioctl_port_num_del(&stIoctl.unData.stIoPortNum);
                break;
            case NF_PORTAL_IOCTL_CMD_GET_PASS_PORT_NUM_LIST:
                enRet = nf_portal_ioctl_port_num_get_list(&stIoctl.unData.stIoPortNumList);
                bGetOps = TRUE;
                break;
            case NF_PORTAL_IOCTL_CMD_PASS_PORT_NUM_FLUSH:
                enRet = nf_portal_ioctl_port_num_flush(&stIoctl.unData.stIoPortNum);
                break;
			/*begin: add@2015.01.31 for wechat connect wifi*/
			case NF_PORTAL_IOCTL_CMD_ADD_WXL_SERVERIP:
                enRet = nf_portal_ioctl_wxl_serverip_add(&stIoctl.unData.stIoWxlServerIp);
                break;
            case NF_PORTAL_IOCTL_CMD_DEL_WXL_SERVERIP:
                enRet = nf_portal_ioctl_wxl_serverip_del(&stIoctl.unData.stIoWxlServerIp);
                break;
            case NF_PORTAL_IOCTL_CMD_GET_WXL_SERVERIP_LIST:
                enRet = nf_portal_ioctl_wxl_serverip_get_list(&stIoctl.unData.stIoWxlServerIpList);
                bGetOps = TRUE;
                break;
            case NF_PORTAL_IOCTL_CMD_WXL_SERVERIP_FLUSH:
                enRet = nf_portal_ioctl_wxl_serverip_flush();
                break;
			/*end: add@2015.01.31 for wechat connect wifi*/
                
        default:
			NF_PORTAL_LOG(NF_PORTAL_LOG_ERR, "Unknown cmd %d\n", stIoctl.enCmd);
			return  AP_E_PARAM;
			break;
	}
	
	if (AP_E_NONE != enRet)
	{
	    return AP_E_DEFAULT_FAIL;
	}
	
	if (bGetOps)
	{
    	if (copy_to_user(pstArg, &stIoctl, sizeof(NF_PORTAL_IOCTL_T)))
    	{
    		NF_PORTAL_LOG(NF_PORTAL_LOG_ERR, "Copy to user failed.\n");
    		return AP_E_DEFAULT_FAIL;
    	}
	}
	
	return AP_E_NONE;
}


