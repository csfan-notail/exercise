/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : fc_white_list.c
  版 本 号   : 初稿
  作    者   : WeiHonggang
  生成日期   : 2015年8月18日
  最近修改   :
  功能描述   : 流控白名单
  函数列表   :
  修改历史   :
  1.日    期   : 2015年8月18日
    作    者   : WeiHonggang
    修改内容   : 创建文件

******************************************************************************/
/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "fc_main.h"
#include "fc_ioctl.h"
#include "fc_white_list.h"

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20))
#include "linux/ip.h"
#endif
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
LOCAL struct hlist_head g_astDestIpHashTable[FC_DEST_IP_HLIST_SIZE]; /*Dest Ip White List hash表*/
LOCAL DEFINE_RWLOCK(g_stDestIpHashLock);
LOCAL UINT32 g_uiDestIpNum =0;
/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

/*目的IP 黑白名单*/
LOCAL inline UINT32 fc_ip_hash(const UINT32 uiIp)
{
    return uiIp;
}

LOCAL struct hlist_head *fc_dest_ip_hash_fn(UINT32 uiIp)
{
    UINT32 uiIndex;

    uiIndex = fc_ip_hash(uiIp);
    
    return &(g_astDestIpHashTable[uiIndex& (FC_DEST_IP_HLIST_SIZE - 1)]);
}

FC_IP_ENTRY_T *fc_dest_ip_get(UINT32 uiIp)
{
    FC_IP_ENTRY_T *pstEntry = NULL;
    struct hlist_node *pos = NULL;
    struct hlist_node *n = NULL;
    
    FC_DEST_IP_HASH_RD_LOCK;
    hlist_for_each_safe(pos, n,fc_dest_ip_hash_fn(uiIp))
    {
        pstEntry = hlist_entry(pos, FC_IP_ENTRY_T, stIpHList);

        if (uiIp == pstEntry->uiIp)
        {
            FC_DEST_IP_HASH_RD_UNLOCK;
            return pstEntry; /*Find it. Return at once.*/
        }
    }
	FC_DEST_IP_HASH_RD_UNLOCK;
	return NULL;
}

AP_ERROR_CODE_E fc_dest_ip_add(UINT32 uiIp)
{
    struct hlist_head *pstHead = NULL;
    FC_IP_ENTRY_T *pstEntry = NULL;

    pstEntry = fc_dest_ip_get(uiIp);

    if (NULL != pstEntry)
    {
        return AP_E_NONE;
    }
    
    FC_LOG(FC_LOG_INFO,"create a new entry\n");
    pstEntry = (FC_IP_ENTRY_T*)(kmalloc(sizeof(FC_IP_ENTRY_T), GFP_ATOMIC));
    if (unlikely(NULL == pstEntry))
    {
        FC_LOG(FC_LOG_ERR,"malloc fail,return\n");
        return AP_E_MEM_MALLOC;
    }

    memset(pstEntry,0,sizeof(FC_IP_ENTRY_T));
    pstEntry->uiIp = uiIp;
    
    FC_DEST_IP_HASH_WR_LOCK;
    pstHead = fc_dest_ip_hash_fn(uiIp);
    hlist_add_head(&(pstEntry->stIpHList), pstHead);
    g_uiDestIpNum++;
    FC_DEST_IP_HASH_WR_UNLOCK;
    
    return AP_E_NONE;
}

AP_ERROR_CODE_E fc_dest_ip_del(FC_IP_ENTRY_T *pstEntry)
{  
    if (NULL == pstEntry)
    {
        FC_LOG(FC_LOG_ERR, "entry is null,return.\n");
        return AP_E_PARAM;
    }
    
    FC_DEST_IP_HASH_WR_LOCK;
    hlist_del_init(&(pstEntry->stIpHList));
    g_uiDestIpNum--;
    FC_DEST_IP_HASH_WR_UNLOCK;

    kfree(pstEntry);
    
    return AP_E_NONE;
}

AP_ERROR_CODE_E fc_dest_ip_get_list(FC_IOCTL_IP_LIST_T *pstIpList)
{
    FC_IP_ENTRY_T *pstEntry = NULL;
    struct hlist_node *pstTmp;
    struct hlist_node *pstTmp2;
    struct hlist_head *pstHead;
    UINT32 uiIndex;
    UINT32 uiEntryNum = 0;
    UINT32 uiSkipNum = 0;
    UINT32 uiStaNum = 0;
    
    if (NULL == pstIpList)
    {
        return AP_E_PARAM;
    }
    
    FC_DEST_IP_HASH_RD_LOCK;
    for (uiIndex = 0;uiIndex < FC_DEST_IP_HLIST_SIZE; uiIndex++)
    {
        pstHead = &g_astDestIpHashTable[uiIndex];
        hlist_for_each_entry_safe(pstEntry,pstTmp,pstTmp2,pstHead,stIpHList)
        {
            /*跳过之前访问过的队列*/
            if (uiSkipNum < pstIpList->uiStartNum)
            {
                uiSkipNum++;
                uiEntryNum++;
                continue;
            }
            
            /*获取到了足够的表项*/
            if (uiStaNum >= FC_IP_LIST_MAX)
            {
                break;
            }

            /*获取Entity*/
            pstIpList->astIp[uiStaNum].uiIp = pstEntry->uiIp;
            uiStaNum++;
            uiEntryNum++;
        }
    }
    FC_DEST_IP_HASH_RD_UNLOCK;
    
    pstIpList->uiNum = uiStaNum;
    pstIpList->uiStartNum = uiEntryNum;
    return AP_E_NONE;
    
}

AP_ERROR_CODE_E fc_dest_ip_flush(void)
{
    FC_IP_ENTRY_T *pstEntry = NULL;
    struct hlist_node *pstTmp;
    struct hlist_node *pstTmp2;
    struct hlist_head *pstHead;
    UINT32 uiIndex;  
    
    FC_DEST_IP_HASH_RD_LOCK;
    for (uiIndex = 0;uiIndex < FC_DEST_IP_HLIST_SIZE; uiIndex++)
    {
        pstHead = &g_astDestIpHashTable[uiIndex];
        hlist_for_each_entry_safe(pstEntry,pstTmp,pstTmp2,pstHead,stIpHList)
        {
            FC_DEST_IP_HASH_RD_UNLOCK;
            fc_dest_ip_del(pstEntry);
            FC_DEST_IP_HASH_RD_LOCK;
        }
    }
    FC_DEST_IP_HASH_RD_UNLOCK; 
    return AP_E_NONE;
}

AP_ERROR_CODE_E fc_white_list_hash_init(void)
{
    UINT32 uiIndex;
    
    for (uiIndex = 0; uiIndex < FC_DEST_IP_HLIST_SIZE; uiIndex++)
    {
        INIT_HLIST_HEAD(&g_astDestIpHashTable[uiIndex]);
    }
    return AP_E_NONE;
    
}

AP_ERROR_CODE_E fc_white_list_hash_destroy(void)
{
    fc_dest_ip_flush();
    return AP_E_NONE;
}


LOCAL UINT16 fc_get_proto(struct sk_buff *skb,UINT32 *uiOffset)
{
    struct ethhdr *pstEthHdr = NULL;
    FC_8021Q_HDR_T *pst8021qHdr = NULL;
    FC_QINQ_HDR_T *pstQinQHdr = NULL;

    pstEthHdr = eth_hdr(skb);
    if (pstEthHdr->h_proto == htons(ETH_P_8021Q))
    {
        uiOffset += VLAN_HLEN;
        pst8021qHdr = (FC_8021Q_HDR_T *)eth_hdr(skb);
        if (pst8021qHdr ->usProto == htons(ETH_P_8021Q))
        {
            uiOffset += VLAN_HLEN;
            pstQinQHdr = (FC_QINQ_HDR_T *)eth_hdr(skb);
            return pstQinQHdr->usProto;
        }
        return pst8021qHdr->usProto;
    }
    return pstEthHdr->h_proto;
}

LOCAL inline UINT32 fc_get_ip_from_pkt_arp(struct sk_buff *pstSkb, FC_DIR_E enDir,UINT32 uiOffset)
{
    FC_ARP_HDR_T *pstArpHdr = NULL;
    UINT32 uiIP = IP_INVALID;
    if (unlikely(pstSkb->len <(ARP_HLEN + uiOffset)))
    {
        FC_LOG(FC_LOG_ERR,"skb is too short(%u)\n",pstSkb->len);
        return IP_INVALID; 
    }
    pstArpHdr  = (FC_ARP_HDR_T *)((UINT8 *)eth_hdr(pstSkb) +ETH_HLEN +uiOffset);

    if (unlikely(pstArpHdr == NULL))
    {
        FC_LOG(FC_LOG_ERR,"arp header is null\n");
        return IP_INVALID;
    }
    uiIP = (enDir == FC_DIR_DS) ? pstArpHdr->ar_sip : pstArpHdr->ar_tip;

    FC_LOG(FC_LOG_DEBUG,"ip:"IP_FMT"\n",IP_ARG(&uiIP));
    return ntohl(uiIP);

}
LOCAL inline UINT32 fc_get_ip_from_pkt_ip(struct sk_buff *pstSkb, FC_DIR_E enDir,UINT32 uiOffset)
{
    struct iphdr *pstIpHdr = NULL;
    UINT32 uiIP = IP_INVALID;
    if (unlikely(pstSkb->len <(IP_HLEN + uiOffset)))
    {
        FC_LOG(FC_LOG_ERR,"skb is too short(%u)\n",pstSkb->len);
        return IP_INVALID; 
    }
    pstIpHdr  = (struct iphdr *)((UINT8 *)eth_hdr(pstSkb) +ETH_HLEN +uiOffset);

    if (unlikely(pstIpHdr == NULL))
    {
        FC_LOG(FC_LOG_ERR,"ip header is null\n");
        return IP_INVALID;
    }
    uiIP = (enDir == FC_DIR_DS) ? pstIpHdr->saddr : pstIpHdr->daddr;

    FC_LOG(FC_LOG_DEBUG,"ip:"IP_FMT"\n",IP_ARG(&uiIP));
    return ntohl(uiIP);
}
UINT32 fc_get_ip_from_pkt(struct sk_buff *pstSkb, FC_DIR_E enDir)
{
    UINT32 uiIp =0;
    UINT32 uiOffset = 0;
    UINT16 usProto =0;

    usProto = fc_get_proto(pstSkb,&uiOffset);

    switch (usProto)
    {
        case htons(ETH_P_ARP):
            uiIp = fc_get_ip_from_pkt_arp(pstSkb,enDir,uiOffset);
            break;
        case htons(ETH_P_IP):
            uiIp = fc_get_ip_from_pkt_ip(pstSkb,enDir,uiOffset);
            break;
        default:
            uiIp = IP_INVALID;
            break;
    }
    return uiIp;
}
BOOL fc_should_pass_by_white_list(struct sk_buff *pstSkb, FC_DIR_E enDir)
{
    UINT32 uiIp = IP_INVALID;

    if(g_uiDestIpNum == 0)
    {
        return FALSE;
    }
    if (unlikely(pstSkb == NULL))
    {
        return FALSE;
    }

    uiIp = fc_get_ip_from_pkt(pstSkb,enDir);

    return (NULL == fc_dest_ip_get(uiIp)) ? FALSE:TRUE;
}

