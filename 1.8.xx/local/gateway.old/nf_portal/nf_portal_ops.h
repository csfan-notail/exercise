/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : nf_portal_ops.h
  版 本 号   : 初稿
  作    者   : Zhong Huaqing
  生成日期   : 2012年8月10日 星期五
  最近修改   :
  功能描述   : nf_portal_ops.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2012年8月10日 星期五
    作    者   : Zhong Huaqing
    修改内容   : 创建文件

******************************************************************************/

#ifndef __NF_PORTAL_OPS_H__
#define __NF_PORTAL_OPS_H__


/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/

#include "ap_types.h"

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
 * 基础类型定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/


 /*----------------------------------------------*
 * 宏操作                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 枚举定义                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 扩展类型定义                                     *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 结构定义                                       *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 导出函数原型说明                             *
 *----------------------------------------------*/
AP_ERROR_CODE_E nf_portal_ioctl_get_free_auth_sw(NF_PORTAL_IOCTL_SW_T *pstIoSW);
AP_ERROR_CODE_E nf_portal_ioctl_set_free_auth_sw(NF_PORTAL_IOCTL_SW_T *pstIoSW);
AP_ERROR_CODE_E nf_portal_ioctl_set_host_ip(NF_PORTAL_IOCTL_IP_T *pstIoIp);
AP_ERROR_CODE_E nf_portal_ioctl_get_host_ip(NF_PORTAL_IOCTL_IP_T *pstIoIp);
AP_ERROR_CODE_E nf_portal_ioctl_set_auth_server_ip(NF_PORTAL_IOCTL_IP_T *pstIoIp);
AP_ERROR_CODE_E nf_portal_ioctl_get_auth_server_ip(NF_PORTAL_IOCTL_IP_T *pstIoIp);
AP_ERROR_CODE_E nf_portal_ioctl_set_auth_state(NF_PORTAL_IOCTL_AUTH_STATE_T *pstIoAuthState);
AP_ERROR_CODE_E nf_portal_ioctl_get_auth_state(NF_PORTAL_IOCTL_AUTH_STATE_T *pstIoAuthState);
AP_ERROR_CODE_E nf_portal_ioctl_get_sta_list(NF_PORTAL_IOCTL_STA_LIST_T *pstIoStaList);
AP_ERROR_CODE_E nf_portal_ioctl_set_lan_if_name(NF_PORTAL_IOCTL_IF_NAME_T *pstIoIfName);
AP_ERROR_CODE_E nf_portal_ioctl_get_lan_if_name(NF_PORTAL_IOCTL_IF_NAME_T *pstIoIfName);
AP_ERROR_CODE_E nf_portal_ioctl_dest_ip_baw_add(NF_PORTAL_IOCTL_IP_BAW_T *pstIoIpBaw);
AP_ERROR_CODE_E nf_portal_ioctl_dest_ip_baw_del(NF_PORTAL_IOCTL_IP_BAW_T *pstIoIpBaw);
AP_ERROR_CODE_E nf_portal_ioctl_dest_ip_baw_get_list(NF_PORTAL_IOCTL_IP_BAW_LIST_T *pstIoIpBawList);
AP_ERROR_CODE_E nf_portal_ioctl_dest_ip_baw_flush(NF_PORTAL_IOCTL_IP_BAW_T *pstIoIpBaw);
AP_ERROR_CODE_E nf_portal_ioctl_free_auth_port_add(NF_PORTAL_IOCTL_IF_NAME_T *pstIoIfName);
AP_ERROR_CODE_E nf_portal_ioctl_free_auth_port_del(NF_PORTAL_IOCTL_IF_NAME_T *pstIoIfName);
AP_ERROR_CODE_E nf_portal_ioctl_free_auth_port_get_list(NF_PORTAL_IOCTL_PORT_LIST_T *pstIoPortList);
AP_ERROR_CODE_E nf_portal_ioctl_free_auth_port_flush(VOID);
AP_ERROR_CODE_E nf_portal_ioctl_sta_list_flush(VOID);
AP_ERROR_CODE_E nf_portal_ioctl_set_ccs_server_ip(NF_PORTAL_IOCTL_IP_T *pstIoIp);
AP_ERROR_CODE_E nf_portal_ioctl_get_ccs_server_ip(NF_PORTAL_IOCTL_IP_T *pstIoIp);
AP_ERROR_CODE_E nf_portal_ioctl_set_snmp_server_ip(NF_PORTAL_IOCTL_IP_T *pstIoIp);
AP_ERROR_CODE_E nf_portal_ioctl_get_snmp_server_ip(NF_PORTAL_IOCTL_IP_T *pstIoIp);

AP_ERROR_CODE_E nf_portal_ioctl_mac_baw_add(NF_PORTAL_IOCTL_MAC_BAW_T *pstIoMacBaw);
AP_ERROR_CODE_E nf_portal_ioctl_mac_baw_del(NF_PORTAL_IOCTL_MAC_BAW_T *pstIoMacBaw);
AP_ERROR_CODE_E nf_portal_ioctl_mac_baw_get_list(NF_PORTAL_IOCTL_MAC_BAW_LIST_T *pstIoMacBawList);
AP_ERROR_CODE_E nf_portal_ioctl_mac_baw_flush(NF_PORTAL_IOCTL_MAC_BAW_T *pstIoMacBaw);
AP_ERROR_CODE_E nf_portal_ioctl_user_ip_baw_add(NF_PORTAL_IOCTL_IP_BAW_T *pstIoIpBaw);
AP_ERROR_CODE_E nf_portal_ioctl_user_ip_baw_del(NF_PORTAL_IOCTL_IP_BAW_T *pstIoIpBaw);
AP_ERROR_CODE_E nf_portal_ioctl_user_ip_baw_get_list(NF_PORTAL_IOCTL_IP_BAW_LIST_T *pstIoIpBawList);
AP_ERROR_CODE_E nf_portal_ioctl_user_ip_baw_flush(NF_PORTAL_IOCTL_IP_BAW_T *pstIoIpBaw);
AP_ERROR_CODE_E nf_portal_ioctl_url_baw_add(NF_PORTAL_IOCTL_URL_BAW_T *pstIoUrlBaw);
AP_ERROR_CODE_E nf_portal_ioctl_url_baw_del(NF_PORTAL_IOCTL_URL_BAW_T *pstIoUrlBaw);
AP_ERROR_CODE_E nf_portal_ioctl_url_baw_get_list(NF_PORTAL_IOCTL_URL_BAW_LIST_T *pstIoUrlBawList);
AP_ERROR_CODE_E nf_portal_ioctl_url_baw_flush(NF_PORTAL_IOCTL_URL_BAW_T *pstIoUrlBaw);
AP_ERROR_CODE_E nf_portal_ioctl_port_num_add(NF_PORTAL_IOCTL_PORT_NUM_T *pstIoPortNum);
AP_ERROR_CODE_E nf_portal_ioctl_port_num_del(NF_PORTAL_IOCTL_PORT_NUM_T *pstIoPortNum);
AP_ERROR_CODE_E nf_portal_ioctl_port_num_get_list(NF_PORTAL_IOCTL_PORT_NUM_LIST_T *pstIoPortNumList);
AP_ERROR_CODE_E nf_portal_ioctl_port_num_flush(NF_PORTAL_IOCTL_PORT_NUM_T *pstIoPortNum);
AP_ERROR_CODE_E nf_portal_ioctl_url_ip_baw_get_list(NF_PORTAL_IOCTL_URL_IP_BAW_LIST_T *pstIoUrlIpBawList);
AP_ERROR_CODE_E nf_portal_ioctl_url_ip_baw_add(NF_PROTAL_IOCTL_URL_IP_BAW_T *pstIoUrlIpBaw);
/*begin: add@2015.01.31 for wechat connect wifi*/
AP_ERROR_CODE_E nf_portal_ioctl_wxl_serverip_add(NF_PORTAL_IOCTL_WXL_SERVERIP_T *pstIoWxlServerIp);
AP_ERROR_CODE_E nf_portal_ioctl_wxl_serverip_del(NF_PORTAL_IOCTL_WXL_SERVERIP_T *pstIoWxlServerIp);
AP_ERROR_CODE_E nf_portal_ioctl_wxl_serverip_get_list(NF_PORTAL_IOCTL_WXL_SERVERIP_LIST_T *pstIoWxlServerIpList);
AP_ERROR_CODE_E nf_portal_ioctl_wxl_serverip_flush(void);
/*end: add@2015.01.31 for wechat connect wifi*/

/*----------------------------------------------*
 * 导出变量说明                             *
 *----------------------------------------------*/

#endif /* __NF_PORTAL_OPS_H__ */

