/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : fc_white_list.h
  版 本 号   : 初稿
  作    者   : WeiHonggang
  生成日期   : 2015年8月18日
  最近修改   :
  功能描述   : fc_white_list.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2015年8月18日
    作    者   : WeiHonggang
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/

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

#ifndef __FC_WHITE_LIST_H__
#define __FC_WHITE_LIST_H__

#define FC_DEST_IP_HLIST_SIZE 256 /*目的IP 白名单链表大小*/

#define FC_DEST_IP_HASH_RD_LOCK            read_lock_bh(&g_stDestIpHashLock)
#define FC_DEST_IP_HASH_RD_UNLOCK        read_unlock_bh(&g_stDestIpHashLock)
#define FC_DEST_IP_HASH_WR_LOCK           write_lock_bh(&g_stDestIpHashLock)
#define FC_DEST_IP_HASH_WR_UNLOCK       write_unlock_bh(&g_stDestIpHashLock)

#define IP_HLEN 20 /*IP 头长度*/
#define ARP_HLEN  28/*ARP头长度*/
#define VLAN_HLEN	4	

#define IP_INVALID 0
typedef struct
{
    UINT32 uiIp;
    struct hlist_node stIpHList;
}FC_IP_ENTRY_T;


typedef struct
{
    struct ethhdr stEthHdr;
    UINT16 usVlanId;
    UINT16 usProto;
}__attribute__((packed)) FC_8021Q_HDR_T;
typedef struct
{
    FC_8021Q_HDR_T st8021qHdr;
    UINT16 usVlanId;
    UINT16 usProto;
}__attribute__((packed)) FC_QINQ_HDR_T;
typedef struct
{
    struct arphdr stArpHdr;
    unsigned char ar_sha[ETH_ALEN];	/* sender hardware address	*/
    UINT32         ar_sip;		/* sender IP address		*/
    unsigned char ar_tha[ETH_ALEN];	/* target hardware address	*/
    UINT32 ar_tip;		/* target IP address		*/
}__attribute__((packed)) FC_ARP_HDR_T;


FC_IP_ENTRY_T *fc_dest_ip_get(UINT32 uiIp);
AP_ERROR_CODE_E fc_dest_ip_add(UINT32 uiIp);
AP_ERROR_CODE_E fc_dest_ip_del(FC_IP_ENTRY_T *pstEntry);
AP_ERROR_CODE_E fc_dest_ip_get_list(FC_IOCTL_IP_LIST_T *pstIpList);
AP_ERROR_CODE_E fc_dest_ip_flush(void);
AP_ERROR_CODE_E fc_white_list_hash_init(void);
AP_ERROR_CODE_E fc_white_list_hash_destroy(void);
BOOL fc_should_pass_by_white_list(struct sk_buff *pstSkb, FC_DIR_E enDir);
#endif /* __FC_WHITE_LIST_H__ */
