/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : nf_portal_ops.h
  �� �� ��   : ����
  ��    ��   : Zhong Huaqing
  ��������   : 2012��8��10�� ������
  ����޸�   :
  ��������   : nf_portal_ops.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��8��10�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __NF_PORTAL_OPS_H__
#define __NF_PORTAL_OPS_H__


/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/

#include "ap_types.h"

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
 * �������Ͷ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/


 /*----------------------------------------------*
 * �����                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ö�ٶ���                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��չ���Ͷ���                                     *
 *----------------------------------------------*/


/*----------------------------------------------*
 * �ṹ����                                       *
 *----------------------------------------------*/


/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������ԭ��˵��                             *
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
 * ��������˵��                             *
 *----------------------------------------------*/

#endif /* __NF_PORTAL_OPS_H__ */

