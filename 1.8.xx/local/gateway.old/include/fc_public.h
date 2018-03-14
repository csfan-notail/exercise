/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : fc_public.h
  �� �� ��   : ����
  ��    ��   : Zhong Huaqing
  ��������   : 2012��8��10�� ������
  ����޸�   :
  ��������   : �û�̬���ں�̬����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��8��10�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __FC_PUBLIC_H__
#define __FC_PUBLIC_H__


/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "ap_types.h"

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
#define FC_QUEUE_NUM_MAX    4096  /*x86������4096������*/
#else
#define FC_QUEUE_NUM_MAX    128  /*������128������*/
#endif

#define FC_QID_INVALID  (-1)

#define FC_RID_INVALID (-1)  /*radio id*/
#define FC_WID_INVALID (-1)  /*wlan id*/


#define FC_FLAG_ENQUEUE		  (0x55)

#define FC_PROC_FILE        "flowctrl"

 /*----------------------------------------------*
 * �����                                       *
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
 * ö�ٶ���                                       *
 *----------------------------------------------*/
typedef enum
{
    FC_RULE_STA = (1<<0),
    FC_RULE_VAP = (1<<1),
    FC_RULE_SSID = (1<<2),
    FC_RULE_AP = (1<<3),
    FC_RULE_MULTI = (1<<4),  /*��������: sta + vap/ssid/ap*/
    FC_RULE_AUTO = (1<<5), /*�����Զ�ѧϰ: vap/ssid/ap*/
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
 * ��չ���Ͷ���                                 *
 *----------------------------------------------*/


/*----------------------------------------------*
 * �ṹ����                                       *
 *----------------------------------------------*/
typedef struct
{
    /*STA ����*/
    UINT8 acStaMac[ETH_ALEN];
    INT32 iRadioId;
    INT32 iWlanId;
    
    /*VAP ����*/
    CHAR acVapName[IFNAMSIZ];
    
    /*SSID ����*/
    CHAR acSsidName[SSID_NAMSIZ];

	/*VLAN ����*/
	INT32 iVid;
    
    /*��������*/
    
}FC_MATCH_ATTRIB_T;


typedef struct
{
    UINT32 uiRate;   
    UINT32 uiBurst;  
}FC_QUEUE_RATE_T;


typedef struct
{
    UINT32 uiBytePassFast;   /*����ֱ��ͨ��*/
    UINT32 uiBytePassDelay;  /*���ľ������ж�ʱ������*/
    UINT32 uiByteDrop;
    UINT32 uiByteTotal;

}FC_QUEUE_STATS_T;


typedef struct
{
    UINT8 acMac[ETH_ALEN];       /*STA MAC��ַ*/
    INT32 iRadioId;
    INT32 iWlanId;
	INT32 iVid;
    FC_QUEUE_RATE_T astRate[FC_DIR_MAX];  /*����*/
}FC_STA_T;

typedef struct
{
    CHAR acVapName[IFNAMSIZ];      /*VAP �豸����*/
    FC_QUEUE_RATE_T astRate[FC_DIR_MAX];   /*����*/
}FC_VAP_T;

typedef struct
{
    CHAR acSsidName[SSID_NAMSIZ];      /*SSID����*/
    FC_QUEUE_RATE_T astRate[FC_DIR_MAX];   /*����*/
}FC_SSID_T;

typedef struct
{
    FC_QUEUE_RATE_T astRate[FC_DIR_MAX];   /*����*/
}FC_AP_T;

typedef struct
{
    INT32 vid;      /*VLAN ID*/
    FC_QUEUE_RATE_T astRate[FC_DIR_MAX];   /*����*/
}FC_VLAN_T;


typedef struct
{
    FC_MATCH_ATTRIB_T stMatchAttrib;
    FC_QUEUE_RATE_T astAdminRate[FC_DIR_MAX];   /*����*/
    FC_QUEUE_RATE_T astRealRate[FC_DIR_MAX];   /*����*/
    FC_QUEUE_STATS_T astStats[FC_DIR_MAX];/*����ͳ����ֵ*/ 
    
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

