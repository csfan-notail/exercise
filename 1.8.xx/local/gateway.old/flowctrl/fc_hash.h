/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : flow_ctrl.h
  �� �� ��   : ����
  ��    ��   : Zhong Huaqing
  ��������   : 2012��8��10�� ������
  ����޸�   :
  ��������   : flow_ctrl.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��8��10�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __FC_HASH_H__
#define __FC_HASH_H__


/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "fc_main.h"
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
 * �������Ͷ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/
 
/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
#define FC_STA_HASH_SIZE    FC_QUEUE_NUM_MAX
#define FC_VAP_HASH_SIZE    FC_QUEUE_NUM_MAX
#define FC_SSID_HASH_SIZE   FC_QUEUE_NUM_MAX
#define FC_VLAN_HASH_SIZE   FC_QUEUE_NUM_MAX


 /*----------------------------------------------*
 * �����                                       *
 *----------------------------------------------*/
 
#if 1
#define FC_HASH_RD_LOCK		read_lock_bh(&g_stHashLock)
#define FC_HASH_RD_UNLOCK	read_unlock_bh(&g_stHashLock)
#define FC_HASH_WR_LOCK		write_lock_bh(&g_stHashLock)
#define FC_HASH_WR_UNLOCK	write_unlock_bh(&g_stHashLock)

#else
#define FC_HASH_RD_LOCK		
#define FC_HASH_RD_UNLOCK	
#define FC_HASH_WR_LOCK		
#define FC_HASH_WR_UNLOCK	

#endif

 
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
 * ��������ԭ��˵��                             *
 *----------------------------------------------*/
AP_ERROR_CODE_E fc_hash_init(void);

AP_ERROR_CODE_E fc_add_sta_queue_to_htable(FC_QUEUE_T *pstQ, FC_DIR_E enDir);
FC_QUEUE_T *fc_get_sta_queue_from_htable(UINT8 *pcMac, FC_DIR_E enDir);
AP_ERROR_CODE_E fc_del_sta_queue_from_htable(FC_QUEUE_T *pstQ);

AP_ERROR_CODE_E fc_add_vap_queue_to_htable(FC_QUEUE_T *pstQ, FC_DIR_E enDir);
FC_QUEUE_T *fc_get_vap_queue_from_htable(CHAR *szVapName, FC_DIR_E enDir);
AP_ERROR_CODE_E fc_del_vap_queue_from_htable(FC_QUEUE_T *pstQ);

AP_ERROR_CODE_E fc_add_ssid_queue_to_htable(FC_QUEUE_T *pstQ, FC_DIR_E enDir);
AP_ERROR_CODE_E fc_del_ssid_queue_from_htable(FC_QUEUE_T *pstQ);
FC_QUEUE_T *fc_get_ssid_queue_from_htable(CHAR *szSsidName, FC_DIR_E enDir);
AP_ERROR_CODE_E fc_add_vlan_queue_to_htable(FC_QUEUE_T *pstQ, FC_DIR_E enDir);
AP_ERROR_CODE_E fc_del_vlan_queue_from_htable(FC_QUEUE_T *pstQ);
FC_QUEUE_T *fc_get_vlan_queue_from_htable(CHAR *szVlanDev, FC_DIR_E enDir);


#endif /* __FC_HASH_H__ */

