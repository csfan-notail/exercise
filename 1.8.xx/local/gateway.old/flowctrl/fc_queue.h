/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : fc_queue.h
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

#ifndef __FC_QUEUE_H__
#define __FC_QUEUE_H__


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


 /*----------------------------------------------*
 * 宏操作                                       *
 *----------------------------------------------*/
#if 1
#define FC_QUEUE_RD_LOCK		read_lock_bh(&g_stQLock)
#define FC_QUEUE_RD_UNLOCK		read_unlock_bh(&g_stQLock)
#define FC_QUEUE_WR_LOCK		write_lock_bh(&g_stQLock)
#define FC_QUEUE_WR_UNLOCK		write_unlock_bh(&g_stQLock)
#else
#define FC_QUEUE_RD_LOCK		
#define FC_QUEUE_RD_UNLOCK		
#define FC_QUEUE_WR_LOCK		
#define FC_QUEUE_WR_UNLOCK		

#endif

 
/*----------------------------------------------*
 * 枚举定义                                       *
 *----------------------------------------------*/
typedef enum
{
    FC_QUEUE_CREATE = 0,
    FC_QUEUE_UPDATE,
}FC_QUEUE_OPER_TYPE_T;

typedef enum
{
    FC_PASS_FAST = 0,
    FC_PASS_DELAY,
}FC_PASS_TYPE_T;


/*----------------------------------------------*
 * 扩展类型定义                                     *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 结构定义                                       *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 导出函数原型说明                             *
 *----------------------------------------------*/
 AP_ERROR_CODE_E fc_queue_init(FC_QUEUE_T *pstQ, FC_DIR_E enDir);
FC_QUEUE_T *fc_find_avail_queue(FC_DIR_E enDir);
AP_ERROR_CODE_E fc_queue_update(
    FC_QUEUE_T *pstQ, 
    FC_QUEUE_RATE_T *pstRate,
    FC_QUEUE_OPER_TYPE_T enOper);

AP_ERROR_CODE_E fc_queue_get_auto_learn_rate(
    FC_DIR_E enDir,
    CHAR *acDevName,
    FC_QUEUE_RATE_T *astRate);

AP_ERROR_CODE_E fc_queue_all_gear_rate(void);
AP_ERROR_CODE_E fc_update_child_queue_set(FC_QUEUE_T *pstPQ);
AP_ERROR_CODE_E fc_adjust_child_queue_rate(FC_QUEUE_T *pstPQ);
AP_ERROR_CODE_E fc_queue_recover_rate(FC_QUEUE_T *pstPQ);

FC_QUEUE_T *fc_get_queue_array_by_dir(FC_DIR_E enDir);
AP_ERROR_CODE_E fc_queue_destroy(FC_QUEUE_T *pstQ);
FC_QUEUE_T *fc_get_ap_queue(FC_DIR_E enDir);
FC_QUEUE_T *fc_get_parent_queue(FC_QUEUE_T *pstQ);
FC_PKT_STATE_E fc_queue_pass_allowed(
    FC_QUEUE_T *pstQ, 
    struct sk_buff *pstSkb,
    FC_PASS_TYPE_T enPassType);
FC_PKT_STATE_E fc_queue_get_ccache(FC_QUEUE_T *pstQ, UINT32 uiPktLen);
AP_ERROR_CODE_E fc_queue_send_pkt(FC_QUEUE_T *pstQ, struct sk_buff *pstSkb);
AP_ERROR_CODE_E fc_queue_flush_all(void);
AP_ERROR_CODE_E fc_queue_auto_rate(FC_DIR_E enDir);
UINT32 fc_queue_get_list(FC_ENTITY_LIST_T *pstList, UINT32 uiStartNum);


#endif /* __FC_QUEUE_H__ */

