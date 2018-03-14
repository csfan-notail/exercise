/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : fc_main.h
  �� �� ��   : ����
  ��    ��   : Zhong Huaqing
  ��������   : 2012��8��10�� ������
  ����޸�   :
  ��������   : fc_main.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��8��10�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __FC_MAIN_H__
#define __FC_MAIN_H__


/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/jiffies.h>
#include <linux/errno.h>
#include <linux/version.h>

#include <linux/skbuff.h>
#include <linux/jhash.h>

#include <linux/netdevice.h>
#include <linux/if_ether.h>


#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_bridge.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>

#include <linux/module.h>



#if (defined(GW_X86) || defined(GW_ARM))   //����x86���ص��ں˰汾     by cj
#include <../net/bridge/br_private.h>
#else
#include "br_private.h"
#endif

#ifdef GW_ARM
#include <linux/moduleparam.h>
#endif


#include "ap_types.h"
#include "fc_public.h"

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
#define	FC_LOG_EMERG	0	/* system is unusable */
#define	FC_LOG_ALERT	1	/* action must be taken immediately */
#define	FC_LOG_CRIT	2	/* critical conditions */
#define	FC_LOG_ERR		3	/* error conditions */
#define	FC_LOG_WARNING	4	/* warning conditions */
#define	FC_LOG_NOTICE	5	/* normal but significant condition */
#define	FC_LOG_INFO	6	/* informational */
#define	FC_LOG_DEBUG	7	/* debug-level messages */

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
#define NF_IP_PRI_FC_DOWN NF_BR_PRI_LAST-1
#define NF_IP_PRI_FC_UP NF_BR_PRI_FIRST+1
 
#define FC_MTU	1518

#define FC_MAX_CCACHE         (64 << 10) 
//#define FC_FLAG_ENQUEUE		  (0x55)

//#define FC_WATCHDOG_DELAY			1//10

#define FC_QFLAG_THROTTLED 	(1<<0)

#define FC_THRESHOLD_SIZE    350  /*Unit: byte*/

#define FC_MIN_BURST  5120

 /*----------------------------------------------*
 * �����                                       *
 *----------------------------------------------*/

/* ���ʵ�λת�� */
#define RATE_TO_TICKBYTE(rate)  		((rate << 7)/HZ)
#define TICKBYTE_TO_RATE(tick_byte)  	((tick_byte*HZ)>>7)

#define RATE_TO_BURST(rate)     ((rate > FC_MIN_BURST) ? rate : FC_MIN_BURST) 
 
#define RATE_TO_DELAY(rate)   rate ? (((FC_MTU * HZ ) / (rate * 128)) + 1) : 1  /* 2012-11-8 mod by zhangping */

#define FC_MAX(a, b) ((a) > (b) ? (a) : (b))
#define FC_MIN(a, b) ((a) < (b) ? (a) : (b))

#define FC_THROTTLE_FLAG_SET(pstQ)   {pstQ->uiFlag |= FC_QFLAG_THROTTLED;}
#define FC_THROTTLE_FLAG_CLEAR(pstQ) {pstQ->uiFlag &= ~FC_QFLAG_THROTTLED;}
#define FC_THROTTLE_FLAG_CHECK(pstQ) (pstQ->uiFlag & FC_QFLAG_THROTTLED)


//#define SET_FC_ENQ_FLAG(skb) (skb->cb[32] = FC_FLAG_ENQUEUE)
//#define FC_ENQ_FLAG_VALID(skb) (FC_FLAG_ENQUEUE == skb->cb[32])

#define FC_UPDATE_STATS_DROP(pstQ, uiPktLen) {   \
    pstQ->stStatsCur.uiByteDrop += uiPktLen;     \
}

#define FC_UPDATE_STATS_PASS_FAST(pstQ, uiPktLen) {   \
    pstQ->stStatsCur.uiBytePassFast += uiPktLen;     \
}

#define FC_UPDATE_STATS_PASS_DELAY(pstQ, uiPktLen) {   \
    pstQ->stStatsCur.uiBytePassDelay += uiPktLen;     \
}
#if defined OPENWRT
#define DEV_IS_VAP(p) (('a' == p->dev->name[0]) && ('t' == p->dev->name[1]))
#else
#define DEV_IS_VAP(p) (('a' == p->dev->name[0]) && ('p' == p->dev->name[1]))
#endif
#if (defined(GW_X86) || defined(GW_ARM))   //by cj
#define DEV_IS_ETH(p) (('e' == p->dev->name[0]) && ('t' == p->dev->name[1]) && ('h' == p->dev->name[2])&& (('1' == p->dev->name[3])||('2' == p->dev->name[3])))
#elif defined OPENWRT
#define DEV_IS_ETH(p) (('e' == p->dev->name[0]) && ('t' == p->dev->name[1]) && ('h' == p->dev->name[2])&& (('0' == p->dev->name[3])||('1' == p->dev->name[3])))
#else
/* BEGIN: Added by WeiHonggang, 2013/12/27   PN:34 */
/*ta40�����豸����������Ϊeth1*/
//#define DEV_IS_ETH(p) (('e' == p->dev->name[0]) && ('t' == p->dev->name[1]) && ('h' == p->dev->name[2]))
#define DEV_IS_ETH(p) (('e' == p->dev->name[0]) && ('t' == p->dev->name[1]) && ('h' == p->dev->name[2]) && '1' == p->dev->name[3])
/* END:   Added by WeiHonggang, 2013/12/27   PN:34 */
#endif

#define FC_LOG(level, fmt, args...)                  \
do{											    \
	if (level <= g_bDebugLevel)					        \
	{											\
		printk("flowctrl %s(%d): ",__FUNCTION__, __LINE__); \
		printk(fmt, ##args);				    \
	}										    \
}while(0)

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20))    
#define skb_queue_walk_safe(queue, skb, tmp)		\
            for (skb = (queue)->next, tmp = skb->next;  \
                 skb != (struct sk_buff *)(queue);      \
                 skb = tmp, tmp = skb->next)
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

typedef struct
{
    BOOL    bInUse;   /*��Ǹö����Ƿ��ѱ�ʹ��*/
    
    UINT32  uiQId;   /*����ID��*/
    
    FC_DIR_E  enDir;        /*�������������ط���*/
    
    FC_RULE_E enRule;       /*��Ӧ�Ĺ���*/
    FC_MATCH_ATTRIB_T stMatchAttrib;    /*ƥ������*/
    
    /*��������*/
    struct list_head stStaSet;
 /*�ն˶��м���*/
    UINT32 uiStaNum;            /*�ն˶�������*/
    //UINT32 uiStaTotalRate;      /*�Ӷ��������ܺ�*/
    INT32  iPQId;               /*��һ�����е�Qid��,Ĭ��ֵΪ-1*/
    
    struct hlist_node stStaHList; /*������֯STA Hash��*/
    struct hlist_node stVapHList; /*������֯VAP Hash��*/
    struct hlist_node stSsidHList; /*������֯SSID Hash��*/
	struct hlist_node stVlanHList; /*������֯VLAN Hash��*/
    
    /*���Ļ���*/
    struct sk_buff_head stSkbList; /*�洢���ĵ�����*/
    UINT32 uiCurCcache;  /*Current ccache*/
    UINT32 uiMinCcache;  /*Record the minimum ccache the queue has.*/
    
    /*��ʱ���������*/
	struct timer_list stTxIimer; /*���ķ��Ͷ�ʱ��*/
	UINT32 uiTxDelay;             /*��ʱ���������*/
	struct timer_list stAgingTimer; /*�Զ����ʵ���ʱ���ϻ���ʱ��*/
	UINT32 uiAgingDelay;             /*��ʱ���������*/
	
	/*���ʿ������*/
    UINT32 uiAdminBytePerTick;  /*��������*/
    UINT32 uiAdminBurstSize;    /*����ͻ����С*/
    UINT32 uiBytePerTick;  /*��ʵ����*/
    UINT32 uiBurstSize;    /*��ʵͻ����С*/
    
    UINT32  uiByteAvail;        /*���е�ǰ�ɷ��͵��ֽ���*/
    UINT32 uiTimeCheckPoint;   /*�ں�tick ����*/
    
    
    
    
    /*����ͳ��*/
    FC_QUEUE_STATS_T stStatsCur;        /*��ǰͳ��*/
    FC_QUEUE_STATS_T stStatsLast1Sec;   /*ǰ1���ͳ��*/
    FC_QUEUE_STATS_T stStatsLast10Sec;   /*ǰ10���ͳ��*/
    
    
    UINT32 uiFlag; /*����״̬��Ŀǰ��Ҫ��¼��ʱ���Ĺ���״̬*/
}FC_QUEUE_T;




/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������ԭ��˵��                             *
 *----------------------------------------------*/
AP_ERROR_CODE_E flowctrl_init(void);
void fc_set_admin(BOOL bEnable);
BOOL fc_get_admin(void);
void fc_set_debugLevel(INT32 iDebugLevel);
INT32 fc_get_debugLevel(void);
FC_RULE_E fc_get_rule(void);
AP_ERROR_CODE_E fc_set_rule(FC_RULE_E enRule);
AP_ERROR_CODE_E fc_set_gear_byte(UINT32 uiGearByte);
UINT32 fc_get_gear_byte(void);
//BOOL fc_get_auto_learn_status(void);
BOOL fc_get_auto_rate_status(void);
AP_ERROR_CODE_E fc_set_auto_rate_status(BOOL bStatus);
UINT32 fc_get_skb_len(struct sk_buff *skb);

FC_PKT_STATE_E fc_enqueue(struct sk_buff *pstSkb, FC_DIR_E enDir);
AP_ERROR_CODE_E fc_dequeue(FC_QUEUE_T *pstQ);

AP_ERROR_CODE_E fc_hook_init(void);
AP_ERROR_CODE_E fc_hook_destroy(void);


/*----------------------------------------------*
 * ��������˵��                             *
 *----------------------------------------------*/
REMOTE BOOL g_bDebugLevel;

#endif /* __FC_MAIN_H__ */

