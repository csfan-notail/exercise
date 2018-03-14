/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : flow_ctrl.h
  版 本 号   : 初稿
  作    者   : Zhong Huaqing
  生成日期   : 2012年8月10日 星期五
  最近修改   :
  功能描述   : flow_ctrl.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2012年8月10日 星期五
    作    者   : Zhong Huaqing
    修改内容   : 创建文件

******************************************************************************/

#ifndef __FC_HASH_H__
#define __FC_HASH_H__


/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "fc_main.h"
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
 * 基础类型定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
 
/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define FC_STA_HASH_SIZE    FC_QUEUE_NUM_MAX
#define FC_VAP_HASH_SIZE    FC_QUEUE_NUM_MAX
#define FC_SSID_HASH_SIZE   FC_QUEUE_NUM_MAX
#define FC_VLAN_HASH_SIZE   FC_QUEUE_NUM_MAX


 /*----------------------------------------------*
 * 宏操作                                       *
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
 * 枚举定义                                       *
 *----------------------------------------------*/



/*----------------------------------------------*
 * 扩展类型定义                                     *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 结构定义                                       *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 导出函数原型说明                             *
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

