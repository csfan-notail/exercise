/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : fc_public.h
  版 本 号   : 初稿
  作    者   : Zhong Huaqing
  生成日期   : 2012年8月10日 星期五
  最近修改   :
  功能描述   : 用户态与内核态公用
  函数列表   :
  修改历史   :
  1.日    期   : 2012年8月10日 星期五
    作    者   : Zhong Huaqing
    修改内容   : 创建文件

******************************************************************************/

#ifndef __FC_PUBLIC_H__
#define __FC_PUBLIC_H__


/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "ap_types.h"

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
#ifndef IFNAMSIZ
#define	IFNAMSIZ	16
#endif

#ifndef SSID_NAMSIZ
#define	SSID_NAMSIZ	32
#endif

#ifndef ETH_ALEN
#define	ETH_ALEN	6
#endif

#ifdef GW_X86
#define FC_QUEUE_NUM_MAX    4096  /*x86最多包含4096个队列*/
#else
#define FC_QUEUE_NUM_MAX    128  /*最多包含128个队列*/
#endif

#define FC_QID_INVALID  (-1)

#define FC_RID_INVALID (-1)  /*radio id*/
#define FC_WID_INVALID (-1)  /*wlan id*/


#define FC_FLAG_ENQUEUE		  (0x55)

#define FC_PROC_FILE        "flowctrl"

 /*----------------------------------------------*
 * 宏操作                                       *
 *----------------------------------------------*/
#define SET_FC_ENQ_FLAG(skb) (skb->cb[35] = FC_FLAG_ENQUEUE)
#define FC_ENQ_FLAG_VALID(skb) (FC_FLAG_ENQUEUE == skb->cb[35])

#define FC_RULE_IS_MULTI(enRule)  (FC_RULE_MULTI & enRule)
#define FC_RULE_IS_AUTO(enRule)   (FC_RULE_AUTO & enRule)

/* BEGIN: Modified by xucongjiang, 2012/10/20 */
#define FC_RULE_IS_VAP_AUTO(enRule)   ((FC_RULE_AUTO & enRule) && (FC_RULE_VAP & enRule))
#define FC_RULE_IS_SSID_AUTO(enRule)   ((FC_RULE_AUTO & enRule) && (FC_RULE_SSID & enRule))
#define FC_RULE_IS_VLAN_AUTO(enRule)   ((FC_RULE_AUTO & enRule) && (FC_RULE_VLAN & enRule))
/* END:   Modified by xucongjiang, 2012/10/20   PN: */

/*----------------------------------------------*
 * 枚举定义                                       *
 *----------------------------------------------*/
typedef enum
{
    FC_RULE_STA = (1<<0),
    FC_RULE_VAP = (1<<1),
    FC_RULE_SSID = (1<<2),
    FC_RULE_AP = (1<<3),
    FC_RULE_MULTI = (1<<4),  /*二级限速: sta + vap/ssid/ap*/
    FC_RULE_AUTO = (1<<5), /*速率自动学习: vap/ssid/ap*/
    FC_RULE_VLAN = (1<<6)
}FC_RULE_E;

typedef enum
{
    FC_DIR_DS = 0,
    FC_DIR_US,
    FC_DIR_MAX
}FC_DIR_E;


typedef enum
{
    FC_PASS_PKT = 0, 
    FC_DROP_PKT,      
    FC_ENQ_PKT        
}FC_PKT_STATE_E;


/*----------------------------------------------*
 * 扩展类型定义                                 *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 结构定义                                       *
 *----------------------------------------------*/
typedef struct
{
    /*STA 属性*/
    UINT8 acStaMac[ETH_ALEN];
    INT32 iRadioId;
    INT32 iWlanId;
    
    /*VAP 属性*/
    CHAR acVapName[IFNAMSIZ];
    
    /*SSID 属性*/
    CHAR acSsidName[SSID_NAMSIZ];

	/*VLAN 属性*/
	INT32 iVid;
    
    /*其它属性*/
    
}FC_MATCH_ATTRIB_T;


typedef struct
{
    UINT32 uiRate;   
    UINT32 uiBurst;  
}FC_QUEUE_RATE_T;


typedef struct
{
    UINT32 uiBytePassFast;   /*报文直接通过*/
    UINT32 uiBytePassDelay;  /*报文经过队列定时器发送*/
    UINT32 uiByteDrop;
    UINT32 uiByteTotal;

}FC_QUEUE_STATS_T;


typedef struct
{
    UINT8 acMac[ETH_ALEN];       /*STA MAC地址*/
    INT32 iRadioId;
    INT32 iWlanId;
	INT32 iVid;
    FC_QUEUE_RATE_T astRate[FC_DIR_MAX];  /*速率*/
}FC_STA_T;

typedef struct
{
    CHAR acVapName[IFNAMSIZ];      /*VAP 设备名称*/
    FC_QUEUE_RATE_T astRate[FC_DIR_MAX];   /*速率*/
}FC_VAP_T;

typedef struct
{
    CHAR acSsidName[SSID_NAMSIZ];      /*SSID名称*/
    FC_QUEUE_RATE_T astRate[FC_DIR_MAX];   /*速率*/
}FC_SSID_T;

typedef struct
{
    FC_QUEUE_RATE_T astRate[FC_DIR_MAX];   /*速率*/
}FC_AP_T;

typedef struct
{
    INT32 vid;      /*VLAN ID*/
    FC_QUEUE_RATE_T astRate[FC_DIR_MAX];   /*速率*/
}FC_VLAN_T;


typedef struct
{
    FC_MATCH_ATTRIB_T stMatchAttrib;
    FC_QUEUE_RATE_T astAdminRate[FC_DIR_MAX];   /*速率*/
    FC_QUEUE_RATE_T astRealRate[FC_DIR_MAX];   /*速率*/
    FC_QUEUE_STATS_T astStats[FC_DIR_MAX];/*报文统计数值*/ 
    
}FC_ENTITY_T; /*Size = 102 Byte */

typedef struct
{
    #define FC_ENTITY_LIST_SIZE  8
    
    FC_ENTITY_T astEntity[FC_ENTITY_LIST_SIZE];      
}FC_ENTITY_LIST_T;

#if 0
typedef struct
{
    BOOL bIsOn;
    FC_QUEUE_RATE_T astRate[FC_DIR_MAX]; 
}FC_QUEUE_AUTO_LEARN_T;
#endif

#endif /* __FC_PUBLIC_H__ */

