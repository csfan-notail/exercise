/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : fc_main.h
  版 本 号   : 初稿
  作    者   : Zhong Huaqing
  生成日期   : 2012年8月10日 星期五
  最近修改   :
  功能描述   : fc_main.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2012年8月10日 星期五
    作    者   : Zhong Huaqing
    修改内容   : 创建文件

******************************************************************************/

#ifndef __FC_MAIN_H__
#define __FC_MAIN_H__


/*----------------------------------------------*
 * 包含头文件                                   *
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



#if (defined(GW_X86) || defined(GW_ARM))   //兼容x86网关的内核版本     by cj
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
#define	FC_LOG_EMERG	0	/* system is unusable */
#define	FC_LOG_ALERT	1	/* action must be taken immediately */
#define	FC_LOG_CRIT	2	/* critical conditions */
#define	FC_LOG_ERR		3	/* error conditions */
#define	FC_LOG_WARNING	4	/* warning conditions */
#define	FC_LOG_NOTICE	5	/* normal but significant condition */
#define	FC_LOG_INFO	6	/* informational */
#define	FC_LOG_DEBUG	7	/* debug-level messages */

/*----------------------------------------------*
 * 宏定义                                       *
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
 * 宏操作                                       *
 *----------------------------------------------*/

/* 速率单位转换 */
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
/*ta40网关设备下联口命名为eth1*/
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
 * 枚举定义                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 扩展类型定义                                     *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 结构定义                                       *
 *----------------------------------------------*/

typedef struct
{
    BOOL    bInUse;   /*标记该队列是否已被使用*/
    
    UINT32  uiQId;   /*队列ID号*/
    
    FC_DIR_E  enDir;        /*队列所处的流控方向*/
    
    FC_RULE_E enRule;       /*对应的规则*/
    FC_MATCH_ATTRIB_T stMatchAttrib;    /*匹配属性*/
    
    /*二级流控*/
    struct list_head stStaSet;
 /*终端队列集合*/
    UINT32 uiStaNum;            /*终端队列数量*/
    //UINT32 uiStaTotalRate;      /*子队列速率总和*/
    INT32  iPQId;               /*上一级队列的Qid号,默认值为-1*/
    
    struct hlist_node stStaHList; /*用于组织STA Hash表*/
    struct hlist_node stVapHList; /*用于组织VAP Hash表*/
    struct hlist_node stSsidHList; /*用于组织SSID Hash表*/
	struct hlist_node stVlanHList; /*用于组织VLAN Hash表*/
    
    /*报文缓存*/
    struct sk_buff_head stSkbList; /*存储报文的链表*/
    UINT32 uiCurCcache;  /*Current ccache*/
    UINT32 uiMinCcache;  /*Record the minimum ccache the queue has.*/
    
    /*定时器相关数据*/
	struct timer_list stTxIimer; /*报文发送定时器*/
	UINT32 uiTxDelay;             /*定时器启动间隔*/
	struct timer_list stAgingTimer; /*自动速率调整时的老化定时器*/
	UINT32 uiAgingDelay;             /*定时器启动间隔*/
	
	/*速率控制相关*/
    UINT32 uiAdminBytePerTick;  /*配置速率*/
    UINT32 uiAdminBurstSize;    /*配置突发大小*/
    UINT32 uiBytePerTick;  /*真实速率*/
    UINT32 uiBurstSize;    /*真实突发大小*/
    
    UINT32  uiByteAvail;        /*队列当前可发送的字节数*/
    UINT32 uiTimeCheckPoint;   /*内核tick 检测点*/
    
    
    
    
    /*报文统计*/
    FC_QUEUE_STATS_T stStatsCur;        /*当前统计*/
    FC_QUEUE_STATS_T stStatsLast1Sec;   /*前1秒的统计*/
    FC_QUEUE_STATS_T stStatsLast10Sec;   /*前10秒的统计*/
    
    
    UINT32 uiFlag; /*队列状态，目前主要记录定时器的工作状态*/
}FC_QUEUE_T;




/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 导出函数原型说明                             *
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
 * 导出变量说明                             *
 *----------------------------------------------*/
REMOTE BOOL g_bDebugLevel;

#endif /* __FC_MAIN_H__ */

