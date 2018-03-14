/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : nf_portal_main.c
  版 本 号   : 初稿
  作    者   : WeiHonggang
  生成日期   : 2013年11月22日 星期五
  最近修改   :
  功能描述   : nf_poratl模块
  函数列表   :
  修改历史   :
  1.日    期   : 2013年11月22日 星期五
    作    者   : WeiHonggang
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "nf_portal_main.h"
#include "nf_portal_ioctl.h"
#include "nf_portal_hash.h"
#include "nf_portal_timer.h"

/*----------------------------------------------*
 * 宏定义                                *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20))
REMOTE UINT32 (*br_nf_portal_pre_routing_hook)(UINT32 hooknum,
		     struct sk_buff **pstSkb,
		     const struct net_device *in,
		     const struct net_device *out,
		     int (*okfn)(struct sk_buff *));
#endif
/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
NF_PORTAL_LOG_LEVEL_E g_enDebugLevel = NF_PORTAL_LOG_ERR;

NF_PORTAL_SW_STATE_E g_enGlobalFreeAuth = SW_ON;
NF_PORTAL_SW_STATE_E g_enLanFreeAuth = SW_OFF;
NF_PORTAL_SW_STATE_E g_enNetConnect =  SW_ON;
NF_PORTAL_NET_MODEL_E g_enNetModel = MODEL_ALL;
UINT8 g_aucLanIfName[IFNAMSIZ] = DEF_LAN_IF_NAME;
UINT32 g_uiHostIp = DEF_HOST_IP;
UINT32 g_uiAuthServerIp = DEF_AHTH_SERVER_IP;
UINT32 g_uiCCSServerIp = DEF_CCS_SERVER_IP;
UINT32 g_uiSNMPServerIp = DEF_SNMP_SERVER_IP;
UINT32 g_uiSpecIp = DEF_SPEC_IP;

#ifdef HAVE_MULTI_WAN
//wan 标记以WAN口顺序1,2,3错开，如果有两个按1,2错开，默认全为1. 
NF_PORTAL_WAN_FLAG_T g_astWanFlag[NF_PORTAL_WAN_MAP_SIZE];
//LOCAL DEFINE_RWLOCK(g_stWanFlagLock);
UINT32 g_uiWanStrategy = NF_PORTAL_WAN_STRATEGY_SOURCE_IP; //wan口路由策略
NF_PORTAL_WAN_CFG_T  g_astWanCfg[NF_PORTAL_WAN_MAX_NUM];
UINT32 g_uiWanMark[NF_PORTAL_WAN_MAX_NUM] = {6,7,8};
#endif

BOOL g_bVlanFloodEnable = FALSE;

/*8080,443,5222,5223,5228*/
NF_PORTAL_PORT_NUM_ATTRIB_T g_astLetPassPort[] =
{
    {IPPROTO_TCP,8080},
    {IPPROTO_TCP,443},
    {IPPROTO_TCP,5222},
    {IPPROTO_TCP,5223},
    {IPPROTO_TCP,5228},
};
/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/
 

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 函数定义                                       *
 *----------------------------------------------*/


void print_packet(NF_PORTAL_LOG_LEVEL_E level, UINT8 *szMsg, UINT8 *pucData, UINT32 ucLen)
{
    int i;

	if (level > g_enDebugLevel)
		return;

    if (NULL != szMsg)
    {
        printk("%s\n", szMsg);
    }
    
    for (i = 0 ; i < ucLen ; i++)
    {
        if (0 != i && i % 16 == 0)
        {
            printk("\n");
        }
        printk("%x ", pucData[i]);
    }
    printk("\n");
    
    return ;
}

AP_ERROR_CODE_E nf_portal_set_debugLevel(UINT32 uiDebugLevel)
{
    if (uiDebugLevel > NF_PORTAL_LOG_DEBUG)
    {
        return AP_E_PARAM;
    }
    g_enDebugLevel = uiDebugLevel;
    return AP_E_NONE;
}

UINT32 nf_portal_get_debugLevel(void)
{
    return g_enDebugLevel;
}
AP_ERROR_CODE_E nf_portal_set_host_ip(UINT32 uiIp)
{
    g_uiHostIp = uiIp;
    return AP_E_NONE;
}
UINT32 nf_portal_get_host_ip(VOID)
{
    return g_uiHostIp;
}
AP_ERROR_CODE_E nf_portal_set_auth_server_ip(UINT32 uiIp)
{
    g_uiAuthServerIp = uiIp;
    return AP_E_NONE;
}
UINT32 nf_portal_get_auth_server_ip(VOID)
{
    return g_uiAuthServerIp;
}
AP_ERROR_CODE_E nf_portal_set_ccs_server_ip(UINT32 uiIp)
{
    g_uiCCSServerIp = uiIp;
    return AP_E_NONE;
}
UINT32 nf_portal_get_ccs_server_ip(VOID)
{
    return g_uiCCSServerIp;
}
AP_ERROR_CODE_E nf_portal_set_snmp_server_ip(UINT32 uiIp)
{
    g_uiSNMPServerIp = uiIp;
    return AP_E_NONE;
}
UINT32 nf_portal_get_snmp_server_ip(VOID)
{
    return g_uiSNMPServerIp;
}
AP_ERROR_CODE_E nf_portal_set_free_auth_sw(NF_PORTAL_SW_STATE_E enGlobalFreeAuth)
{
    g_enGlobalFreeAuth = enGlobalFreeAuth;
    return AP_E_NONE;
}

NF_PORTAL_SW_STATE_E nf_portal_get_free_auth_sw(VOID)
{
    return g_enGlobalFreeAuth;
}

AP_ERROR_CODE_E nf_portal_set_lan_name(UINT8 *pucLanIfName)
{
    memcpy(g_aucLanIfName,pucLanIfName,IFNAMSIZ);
    return AP_E_NONE;
}

AP_ERROR_CODE_E nf_portal_set_net_model(NF_PORTAL_NET_MODEL_E enGlobalNetModel)
{
    g_enNetModel = enGlobalNetModel;
    return AP_E_NONE;
}

NF_PORTAL_NET_MODEL_E nf_portal_get_net_model(VOID)
{
    return g_enNetModel;
}


/*begin: add@2015-9-6 for lan_free_auth*/
AP_ERROR_CODE_E nf_portal_set_lan_free_auth_sw(NF_PORTAL_SW_STATE_E enGlobalFreeAuth)
{
    g_enLanFreeAuth = enGlobalFreeAuth;
    return AP_E_NONE;
}

NF_PORTAL_SW_STATE_E nf_portal_get_lan_free_auth_sw(VOID)
{
    return g_enLanFreeAuth;
}
/*end: add@2015-9-6 for lan_free_auth*/

/*begin: add@2015-9-18 for net connect*/
AP_ERROR_CODE_E nf_portal_set_net_connect(NF_PORTAL_SW_STATE_E enGlobalNetConnect)
{
    g_enNetConnect = enGlobalNetConnect;
    return AP_E_NONE;
}

NF_PORTAL_SW_STATE_E nf_portal_get_net_connect(VOID)
{
    return g_enNetConnect;
}
/*end: add@2015-9-6 for net connect*/


UINT8 *nf_portal_get_lan_name(VOID)
{
    return g_aucLanIfName;
}

AP_ERROR_CODE_E nf_portal_set_vlan_flood(BOOL bEnable)
{
    g_bVlanFloodEnable = bEnable;
    return AP_E_NONE;
}
BOOL nf_portal_get_vlan_flood(VOID)
{
    return g_bVlanFloodEnable;
}


BOOL nf_portal_is_lan_if(const struct net_device *dev)
{
#if (defined(GW_X86) || defined(GW_ARM))     //兼容x86内核 by cj
    if ((NULL == dev) || (NULL == br_port_get_rcu(dev)) || (NULL == br_port_get_rcu(dev)->br) || (NULL == br_port_get_rcu(dev)->br->dev))
    {
        return FALSE;
    }

/* BEGIN: Added by WeiHonggang, 2014/3/5   PN:BUG #5099 【优化】兼容下联口分布在不同的桥内的组网 */
    //if (0 == strcmp(br_port_get_rcu(dev)->br->dev->name,g_aucLanIfName))
    if (0 == memcmp(br_port_get_rcu(dev)->br->dev->name,g_aucLanIfName,strlen(g_aucLanIfName)))
    {
        return TRUE;
    }
/* END:   Added by WeiHonggang, 2014/3/5   PN:BUG #5099 【优化】兼容下联口分布在不同的桥内的组网 */

    return FALSE;
#else
    #if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,39))
    if ((NULL == dev) || (NULL == dev->br_port) || (NULL == dev->br_port->br) || (NULL == dev->br_port->br->dev))
    {
        return FALSE;
    }
    
    /* BEGIN: Added by WeiHonggang, 2014/3/5   PN:BUG #5099 【优化】兼容下联口分布在不同的桥内的组网 */
    //if (0 == strcmp(dev->br_port->br->dev->name,g_aucLanIfName))
    if (0 == memcmp(dev->br_port->br->dev->name,g_aucLanIfName,strlen(g_aucLanIfName)))
    {
        return TRUE;
    }
   /* END:   Added by WeiHonggang, 2014/3/5   PN:BUG #5099 【优化】兼容下联口分布在不同的桥内的组网 */
   
    #else
    if ((NULL == dev) || (NULL == br_port_get_rcu(dev)) || (NULL == br_port_get_rcu(dev)->br) || (NULL == br_port_get_rcu(dev)->br->dev))
    {
        return FALSE;
    }

/* BEGIN: Added by WeiHonggang, 2014/3/5   PN:BUG #5099 【优化】兼容下联口分布在不同的桥内的组网 */
    //if (0 == strcmp(br_port_get_rcu(dev)->br->dev->name,g_aucLanIfName))
    if (0 == memcmp(br_port_get_rcu(dev)->br->dev->name,g_aucLanIfName,strlen(g_aucLanIfName)))
    {
        return TRUE;
    }
/* END:   Added by WeiHonggang, 2014/3/5   PN:BUG #5099 【优化】兼容下联口分布在不同的桥内的组网 */
    
    #endif

 


    return FALSE;
#endif    
}

LOCAL inline BOOL nf_portal_is_dhcp_frame(UINT16 usSrcPort,UINT16 usDestPort)
{
    if ((usSrcPort == BOOTPC) && (usDestPort == BOOTPS))
    {
        return TRUE;
    }
    
    return FALSE;
}
LOCAL inline BOOL nf_portal_is_dns_frame(UINT16 usPort)
{
    if (usPort == DNSS)
    {
        return TRUE;
    }
    return FALSE;
}
LOCAL inline BOOL nf_portal_is_http_frame(UINT16 usDestPort)
{
	/*begin: modified@ 2014.9.24 PN:中核高通需求，需要拦截8008端口tcp报文*/
    #if 0
    if (usDestPort == HTTPS)
    {
        return TRUE;
    }
    #endif
    if (usDestPort == HTTPS || HTTPS_FORCUSCOM == usDestPort)
    {
        return TRUE;
    }
    /*end: modified@ 2014.9.24 PN:中核高通需求，需要拦截8008端口tcp报文*/

    return FALSE;
}


LOCAL inline UINT32 nf_portal_get_nf_mark(struct sk_buff *skb)
{
    UINT32 uiMark;
    #if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20))    
    uiMark = skb->nfmark;
    #else
    uiMark = skb->mark;
    #endif
    return uiMark;
}

LOCAL inline VOID nf_portal_set_mark(struct sk_buff *skb,UINT32 uiMark)
{
    #if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20))    
    skb->nfmark = uiMark;
    #else
    skb->mark = uiMark;
    #endif
}
LOCAL VOID nf_portal_dns_resolve(struct sk_buff *skb,struct udphdr *pstUdpHdr)
{
    NF_PORTAL_DNS_HDR_T *pstDnsHdr = NULL;
	struct iphdr *pstIpHdr = NULL;
    UINT8 aucUrl[URL_MAX_SIZE] = {"\0"};
    UINT8 *pucPos = NULL;
    UINT32 uiUrlLen = 0;
	UINT32 uiDnsLen = 0;
    UINT32 uiLoop;
    UINT16 usType = 0;
    UINT32 uiIp = 0;
    NF_PORTAL_URL_BAW_ENTRY_T *pstEntry = NULL;
    
    if (unlikely(NULL == skb) || unlikely(NULL == pstUdpHdr))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"skb is null,or pstUdpHdr is null,return\n");
        return;
    }

	/*去UDP头*/
	if (unlikely(ntohs(pstUdpHdr->len) < UDP_HLEN))
	{
		NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"udp total len is less than udp head len,return\n");
        return;
	}

    pstDnsHdr = (NF_PORTAL_DNS_HDR_T *)((UINT8 *)pstUdpHdr +UDP_HLEN);
	uiDnsLen = ntohs(pstUdpHdr->len) - UDP_HLEN;
	pstIpHdr = (struct iphdr *)((UINT8 *)pstUdpHdr - IP_HLEN);
	
	/*解析 DNS头*/
    if (unlikely(NULL == pstDnsHdr))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"pstDnsHdr is null,return\n");
        return;  
    }

	if (unlikely(uiDnsLen < DNS_HLEN))
	{
		NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"dns total len is less than dns head len,return\n");
        return;  
	}
#if defined (BIG_ENDIAN)
    NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"QR=%u,opcode=%u,AA=%u,TC=%u\n"\
        "RD=%u,RA=%u,zero=%u,rcode=%u\n",\
        pstDnsHdr->QR,pstDnsHdr->opcode,pstDnsHdr->AA,pstDnsHdr->TC,\
        pstDnsHdr->RD,pstDnsHdr->RA,pstDnsHdr->zero,pstDnsHdr->rcode);

    if ((pstDnsHdr->QR != DNS_FLAG_QR_R) || (pstDnsHdr->opcode != DNS_FLAG_OPCODE_S)\
        ||(pstDnsHdr->TC != DNS_FLAG_TC_NO) || (pstDnsHdr->rcode != DNS_FLAG_RCODE_OK))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"flags do not match,return\n");
        return;
    }
#else
    NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"flags = 0x%04x\n",ntohs(pstDnsHdr->usFlags));
    if (ROOT_DNS_SERVER_FLAGS != ntohs(pstDnsHdr->usFlags)
		&& AUTHORITY_DNS_SERVER_FLAGS != ntohs(pstDnsHdr->usFlags))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"flags do not match,return\n");
        return;
    }
#endif
    NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"usTransactionId = %u\nusQuestions = %u,usAnswerRRS = %u\n"\
        "usAuthorityRRS = %u,usAdditionalRRS = %u\n",\
        ntohs(pstDnsHdr->usTransactionId),ntohs(pstDnsHdr->usQuestions),\
        ntohs(pstDnsHdr->usAnswerRRS),ntohs(pstDnsHdr->usAuthorityRRS),\
        ntohs(pstDnsHdr->usAdditionalRRS));

    pucPos = (UINT8 *)((UINT8 *)pstDnsHdr +DNS_HLEN);
	uiDnsLen = uiDnsLen - DNS_HLEN;

	
    /*提取URL*/
    for (uiLoop = 0;uiLoop < ntohs(pstDnsHdr->usQuestions); uiLoop++)
    {
        while(0 != (*pucPos))
        {
        	if(uiUrlLen + *pucPos + 1 >= URL_MAX_SIZE)//数组越界检测
        	{
				NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"too long domain,return\n");
				print_packet(NF_PORTAL_LOG_ERR,"Print ip packet",(UINT8 *)pstIpHdr, ntohs(pstIpHdr->tot_len));
				return;
        	}

			if(uiDnsLen < *pucPos + 1)
			{
				NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"dns packet is truncated1,return\n");
				print_packet(NF_PORTAL_LOG_ERR,"Print ip packet",(UINT8 *)pstIpHdr, ntohs(pstIpHdr->tot_len));
				return;
        	}
			
           	memcpy((aucUrl+uiUrlLen),(pucPos+1),*pucPos);
            uiUrlLen += (*pucPos);
			aucUrl[uiUrlLen] = '.';
            uiUrlLen++;
			
			pucPos +=(*pucPos + 1);
			uiDnsLen -=(*pucPos + 1);
        }
        aucUrl[uiUrlLen-1] = '\0';

		if(uiDnsLen < 5)/*1+2+2*/
		{
			NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"dns packet is truncated2,return\n");
			print_packet(NF_PORTAL_LOG_ERR,"Print ip packet",(UINT8 *)pstIpHdr, ntohs(pstIpHdr->tot_len));
			return;
		}
        pucPos += 5; 
		uiDnsLen -= 5;
        uiUrlLen = 0;
        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"url=%s\n",aucUrl);
        
        pstEntry = nf_portal_url_baw_get(aucUrl); /*bug exist*/
        if (NULL == pstEntry)
        {
            return;
        }
        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"pstEntry->stUrlBAW.aucUrl=%s\n",pstEntry->stUrlBAW.aucUrl);
    }

    /*提取IP地址*/
    for(uiLoop = 0; uiLoop < ntohs(pstDnsHdr->usAnswerRRS); uiLoop++)
    {
    	if(uiDnsLen < 10)/*2+2+2+4*/
		{
			NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"dns packet is truncated3,return\n");
			print_packet(NF_PORTAL_LOG_ERR,"Print ip packet",(UINT8 *)pstIpHdr, ntohs(pstIpHdr->tot_len));
			return;
		}
        pucPos += 2; /*指向域名*/
		uiDnsLen -= 2;
        usType = ntohs(*(UINT16 *) pucPos);
        pucPos += 8; /*2+2+4*/
		uiDnsLen -= 8;
        if (usType == DNS_ANSWER_TYPE_A)
        {
        	if(uiDnsLen < 6)
			{
				NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"dns packet is truncated4,return\n");
				print_packet(NF_PORTAL_LOG_ERR,"Print ip packet",(UINT8 *)pstIpHdr, ntohs(pstIpHdr->tot_len));
				return;
			}
            pucPos += 2;
			uiDnsLen -= 2;
            uiIp = ntohl(*(UINT32 *) pucPos);
            NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"IP="IP_FMT"\n",IP_ARG(&uiIp));
            pucPos += 4;/*data len*/
			uiDnsLen -= 4;
            nf_portal_url_ip_baw_add(uiIp,pstEntry,aucUrl);
        }
        else
        {
        	if(uiDnsLen < ntohs(*(UINT16 *)pucPos))
			{
				NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"dns packet is truncated5,return\n");
				return;
			}
            pucPos += (ntohs(*(UINT16 *)pucPos) +2);
        }
    }
    return;
}

LOCAL UINT16 nf_portal_get_proto(struct sk_buff *skb,UINT32 *uiOffset)
{
    struct ethhdr *pstEthHdr = NULL;
    NF_PORTAL_8021Q_HDR_T *pst8021qHdr = NULL;
    NF_PORTAL_QINQ_HDR_T *pstQinQHdr = NULL;

    pstEthHdr = eth_hdr(skb);
    if (pstEthHdr->h_proto == htons(ETH_P_8021Q))
    {
        uiOffset += VLAN_HLEN;
        pst8021qHdr = (NF_PORTAL_8021Q_HDR_T *)eth_hdr(skb);
        if (pst8021qHdr ->usProto == htons(ETH_P_8021Q))
        {
            uiOffset += VLAN_HLEN;
            pstQinQHdr = (NF_PORTAL_QINQ_HDR_T *)eth_hdr(skb);
            return pstQinQHdr->usProto;
        }
        return pst8021qHdr->usProto;
    }
    return pstEthHdr->h_proto;
}

/* BEGIN: Added by WeiHonggang, 2014/3/5   PN:BUG #5088不同VLAN接口加到同一个桥下会导致的广播环路问题  */
inline BOOL real_dev_is_same(const struct net_device *in,const struct net_device *out)
{
    if(0 == strncmp(in->name, out->name, 5))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/* END:   Added by WeiHonggang, 2014/3/5   PN:BUG #5088不同VLAN接口加到同一个桥下会导致的广播环路问题  */

#ifdef HAVE_MULTI_WAN
/*build 将flag 均匀分布到数组中*/
void get_random_bytes(void *buf, int nbytes);
 
LOCAL UINT32 get_random_number(VOID)
{
    UINT32 randNum;
 
    //printk(KERN_ALERT "Get some real random number.\n");
    get_random_bytes(&randNum, sizeof(UINT32));
    return randNum;
}
//同等带宽的场景较多， 同等带宽场景下面均匀分布。 不同带宽下的场景随机分布。
//wan1 必须enable
AP_ERROR_CODE_E nf_portal_wanflag_build(VOID)
{
	INT32 iLoop;
	INT32 index;
	UINT32 uiTotalBand = 0;
	UINT32 uiTemp = 0;
	UINT32 uiWanNum = 0;
	NF_PORTAL_WAN_CFG_T *pstWan1 = &g_astWanCfg[NF_PORTAL_WAN1 - 1];
	NF_PORTAL_WAN_CFG_T *pstWan2 = &g_astWanCfg[NF_PORTAL_WAN2 - 1];
	NF_PORTAL_WAN_CFG_T *pstWan3 = &g_astWanCfg[NF_PORTAL_WAN3 - 1];
	/*get wan num and total band*/
	if (pstWan1->uiEnable) {uiWanNum++; uiTotalBand += pstWan1->uiWanMaxBand;}
	if (pstWan2->uiEnable) {uiWanNum++; uiTotalBand += pstWan2->uiWanMaxBand;}
	if (pstWan3->uiEnable) {uiWanNum++; uiTotalBand += pstWan3->uiWanMaxBand;}

	if (pstWan1->uiEnable) 
	{
		pstWan1->uiFlagNum=(pstWan1->uiWanMaxBand*NF_PORTAL_WAN_MAP_SIZE)/uiTotalBand;
	}
	else
	{
		pstWan1->uiFlagNum=0;
	}
	if (pstWan2->uiEnable) 
	{
		pstWan2->uiFlagNum=(pstWan2->uiWanMaxBand*NF_PORTAL_WAN_MAP_SIZE)/uiTotalBand;
	}
	else
	{
		pstWan2->uiFlagNum=0;
	}
	if (pstWan3->uiEnable) 
	{
		pstWan3->uiFlagNum=(pstWan3->uiWanMaxBand*NF_PORTAL_WAN_MAP_SIZE)/uiTotalBand;
	}
	else
	{
		pstWan3->uiFlagNum=0;
	}
	
	/*3 band is same*/
	if (3 == uiWanNum && 
		pstWan1->uiFlagNum == pstWan2->uiFlagNum &&
		pstWan1->uiFlagNum == pstWan3->uiFlagNum)
	{
		for (iLoop = 0; iLoop < NF_PORTAL_WAN_MAP_SIZE; iLoop++)
		{
			if (iLoop % 3 == 0) {g_astWanFlag[iLoop].uiWanIndex = pstWan1->uiWanIndex;}
			if (iLoop % 3 == 1) {g_astWanFlag[iLoop].uiWanIndex = pstWan2->uiWanIndex;}
			if (iLoop % 3 == 2) {g_astWanFlag[iLoop].uiWanIndex = pstWan3->uiWanIndex;}
		}
		return AP_E_NONE;
	}
	
	/*2 band is same*/
	if (2 == uiWanNum && 
		pstWan2->uiEnable && 
		pstWan1->uiFlagNum == pstWan2->uiFlagNum)
	{
		for (iLoop = 0; iLoop < NF_PORTAL_WAN_MAP_SIZE; iLoop++)
		{
			if (iLoop % 2 == 0) {g_astWanFlag[iLoop].uiWanIndex = pstWan1->uiWanIndex;}
			if (iLoop % 2 == 1) {g_astWanFlag[iLoop].uiWanIndex = pstWan2->uiWanIndex;}
		}
		return AP_E_NONE;
	}
	else if (2 == uiWanNum && 
			 pstWan3->uiEnable && 
			 pstWan1->uiFlagNum == pstWan3->uiFlagNum)
	{
		for (iLoop = 0; iLoop < NF_PORTAL_WAN_MAP_SIZE; iLoop++)
		{
			if (iLoop % 2 == 0) {g_astWanFlag[iLoop].uiWanIndex = pstWan1->uiWanIndex;}
			if (iLoop % 2 == 1) {g_astWanFlag[iLoop].uiWanIndex = pstWan3->uiWanIndex;}
		}
		return AP_E_NONE;
	}
	
	/*band is not same*/
	for(iLoop = 0; iLoop < NF_PORTAL_WAN_MAP_SIZE; iLoop++)
	{
		if (iLoop < pstWan1->uiFlagNum && 0 != pstWan1->uiFlagNum)	
		{
			g_astWanFlag[iLoop].uiWanIndex = pstWan1->uiWanIndex;
		}
		else if (iLoop < pstWan1->uiFlagNum + pstWan2->uiFlagNum  
			     && 0 != pstWan2->uiFlagNum) 
		{
			g_astWanFlag[iLoop].uiWanIndex = pstWan2->uiWanIndex;
		}
		else if (0 != pstWan3->uiFlagNum)
		{
			g_astWanFlag[iLoop].uiWanIndex = pstWan3->uiWanIndex;
		}
	}
	
    for (iLoop = 0; iLoop < NF_PORTAL_WAN_MAP_SIZE; iLoop++)  
    {  
        index = get_random_number() % (NF_PORTAL_WAN_MAP_SIZE - iLoop) + iLoop;  
        if (index != iLoop)  
        {  
            uiTemp = g_astWanFlag[iLoop].uiWanIndex;  
            g_astWanFlag[iLoop].uiWanIndex = g_astWanFlag[index].uiWanIndex;  
            g_astWanFlag[index].uiWanIndex = uiTemp;
        }  
    }
	return AP_E_NONE;
}

AP_ERROR_CODE_E nf_portal_wanflag_init(VOID)
{
	UINT32 iLoop = 0;
	for (iLoop = 0; iLoop < NF_PORTAL_WAN_MAP_SIZE; iLoop++)
	{
		g_astWanFlag[iLoop].uiWanIndex = NF_PORTAL_WAN1;
	}
	return AP_E_NONE;
}

AP_ERROR_CODE_E nf_portal_wancfg_init(VOID)
{
	UINT32 iLoop = 0;

	nf_portal_wanflag_init();
	for (iLoop = 0; iLoop < NF_PORTAL_WAN_MAX_NUM; iLoop++)
	{
		g_astWanCfg[iLoop].uiWanIndex = iLoop + 1;
		if (NF_PORTAL_WAN1 == g_astWanCfg[iLoop].uiWanIndex)
		{
			g_astWanCfg[iLoop].uiEnable = 1;
		}
		else
		{
			g_astWanCfg[iLoop].uiEnable = 0;
		}
	}
	return AP_E_NONE;
}

UINT32 nf_portal_get_wan_index(UINT32 uiIp, UINT32 uiProto, UINT16 usSrcPort)
{
	UINT32 uiKey = ((uiIp & (0x0000FFFF)) + usSrcPort + uiProto) % NF_PORTAL_WAN_MAP_SIZE;
	UINT32 uiWanIndex = g_astWanFlag[uiKey].uiWanIndex;
	if (uiWanIndex == 0 || uiWanIndex > NF_PORTAL_WAN_MAX_NUM)
	{
		NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"uiWanIndex: %u is illegal. set it wan1\n", uiWanIndex);
		uiWanIndex = NF_PORTAL_WAN1;
	}
	return uiWanIndex;
}

UINT32 nf_portal_get_mark(UINT32 uiWanIndex)
{
	UINT32 uiMark = 0;
	if (uiWanIndex == 0 || uiWanIndex > NF_PORTAL_WAN_MAX_NUM)
	{
		NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"uiWanIndex: %u is illegal. return wan1 mark\n", uiWanIndex);
		uiMark = g_uiWanMark[NF_PORTAL_WAN1 - 1];
	}
	else
	{
		uiMark = g_uiWanMark[uiWanIndex - 1];
	}
	return uiMark;
}

inline BOOL nf_portal_set_forward_mark(struct sk_buff *skb, UINT32 uiIp, 
												 UINT32 uiProto, UINT16 usSrcPort)
{ 
	UINT32 uiMark = 0;
	UINT32 uiWanIndex = NF_PORTAL_WAN1;
	/*参数检查*/
    if (unlikely(NULL == skb))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"skb is null,but accept yet\n");
        return FALSE;
    }
	#if 1
	/*查看策略类型*/
	switch(g_uiWanStrategy){
		/*策略1: ip 段静态划分*/
		//TODO
		case NF_PORTAL_WAN_STRATEGY_STATIC_IP:
			break;
		/*策略2: 自动配置(基于终端)*/
		case NF_PORTAL_WAN_STRATEGY_SOURCE_IP:
			uiWanIndex = nf_portal_get_wan_index(uiIp, 0, 0);
			break;
		/*策略3: 自动配置(基于终端会话)*/
	    //TODO
	    case NF_PORTAL_WAN_STRATEGY_SESSION:
			uiWanIndex = nf_portal_get_wan_index(uiIp, uiProto, usSrcPort);
			break;
		default:
			break;
	}
	#endif
	uiMark = nf_portal_get_mark(uiWanIndex);
	
    NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"uiMark: %u\n", uiMark);
	
	nf_portal_set_mark(skb, uiMark);
	return TRUE;
}
#endif


inline int get_mac_offset(UINT8 *aucMac, UINT32 uiHashSize)
{
	int	x;
	int	index;

	if(uiHashSize <= 1)
		return 0;
	
	x = aucMac[0];
	x = (x << 2) ^ aucMac[1];
	x = (x << 2) ^ aucMac[2];
	x = (x << 2) ^ aucMac[3];
	x = (x << 2) ^ aucMac[4];
	x = (x << 2) ^ aucMac[5];

	x ^= x >> 8;

	index = x & (uiHashSize - 1);

	return index;
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20))
LOCAL UINT32 nf_portal_pre_routing_hook(UINT32 hooknum,
		     struct sk_buff **pstSkb,
		     const struct net_device *in,
		     const struct net_device *out,
		     int (*okfn)(struct sk_buff *))
#else
LOCAL UINT32 nf_portal_pre_routing_hook(UINT32 hooknum,
		     struct sk_buff *skb,
		     const struct net_device *in,
		     const struct net_device *out,
		     int (*okfn)(struct sk_buff *))
#endif
{
    struct ethhdr *pstEthHdr = NULL;
    struct iphdr *pstIpHdr = NULL;
    struct udphdr *pstUdpHdr = NULL;
    struct tcphdr *pstTcpHdr = NULL;
    UINT8 aucMac[ETH_ALEN];
	UINT32 uiMarkOffset; 
    UINT32 uiIp;
    UINT32 uiDestIp;
    UINT16 usDestPort;
#ifdef HAVE_MULTI_WAN	
	UINT32 uiProto = 2; //TCP(0)UDP(1)OTHER(2)
	UINT16 usSrcPort;
#endif
    #ifdef AD_SUPPORT
    UINT32 uiMark=0;
    #endif
    NF_PORTAL_STA_ENTRY_T *pstEntry = NULL;
    NF_PORTAL_AUTH_STATE_E enState = STATE_MAX;
    NF_PORTAL_STA_TYPE_E enStatype = TYPE_MAX;
    UINT32 uiOffset = 0;
    NF_PORTAL_IP_BAW_ENTRY_T *pstIpBawEntry = NULL;
    NF_PORTAL_NET_BAW_ENTRY_T *pstNetBawEntry = NULL;
    NF_PORTAL_MAC_BAW_ENTRY_T *pstMacBawEntry = NULL;
    NF_PORTAL_URL_IP_BAW_ENTRY_T *pstUrlIpBawEntry = NULL;
    NF_PORTAL_URL_BAW_ENTRY_T *pstUrlBaw = NULL;
	UINT32 uiListDrop = 0;
    /*全局免认证开关，亦是模块功能开关*/
    if ( SW_ON == nf_portal_get_free_auth_sw() )
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"free auth sw enable,accept\n");
        goto nf_accept;
    }
    #if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20))
    struct sk_buff *skb = NULL;
    if (unlikely(NULL == pstSkb))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"pstSkb is null,but accept yet\n");
        return NF_ACCEPT;
    }
    skb = *pstSkb;
    #endif
    /*参数检查*/
    if (unlikely(NULL == skb))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"skb is null,but accept yet\n");
        return NF_ACCEPT;
    }

    if (unlikely(NULL == in))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"in device is null,but accept yet\n");
        return NF_ACCEPT;
    }
    
    //NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"IN->NAME=%s,OUT->NAME=%s\n",(in == NULL)?"NULL":(in->name),(out == NULL)?"NULL":(out->name));

    /*只对指定桥报文做处理*/
    if (FALSE == nf_portal_is_lan_if(in))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"not in lan br,accept\n");
        return NF_ACCEPT;
    }

    /*解析报文二层和三层信息获取MAC地址和IP地址*/
    pstEthHdr = eth_hdr(skb);
    if (unlikely(NULL == pstEthHdr))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"eth header is null,but accept yet\n");
        return NF_ACCEPT;
    }
    memcpy(aucMac,pstEthHdr->h_source,ETH_ALEN);
    uiMarkOffset = get_mac_offset(aucMac, PORTAL_MARK_NUM);

    if (htons(ETH_P_IP) != nf_portal_get_proto(skb,&uiOffset))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"no ip pkt,accept\n");
        return NF_ACCEPT;
    }
    if (unlikely(skb->len <IP_HLEN))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"skb is too short(%u),but accept yet\n",skb->len);
        return NF_ACCEPT;
    }
    pstIpHdr = (struct iphdr *)((UINT8 *)eth_hdr(skb) +ETH_HLEN +uiOffset);

    if (unlikely(NULL == pstIpHdr))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"ip header is null,but accept yet\n");
        return NF_ACCEPT;
    }
    uiIp = ntohl(pstIpHdr->saddr);
    uiDestIp = ntohl(pstIpHdr->daddr);
    if (uiIp == 0)
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"ip is zero,dhcp request probably,accept\n");
        return NF_ACCEPT;
    }
    
    /*主机IP和server IP报文放行,提前到这里*/
    if (uiDestIp == g_uiAuthServerIp)    //断网如果还能认证，不合适
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,IP_FMT"to host or auth server or ccs server,accept\n",IP_ARG(&uiDestIp));
        goto nf_accept;
    }
    if( (uiDestIp == g_uiHostIp) || (uiDestIp == g_uiCCSServerIp) || (uiDestIp == g_uiSNMPServerIp) )
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,IP_FMT"to host or auth server or ccs server,accept\n",IP_ARG(&uiDestIp));
        return NF_ACCEPT;
    }

     /*ADVERTISEMENT SUPPORT*/
    #ifdef AD_SUPPORT
    uiMark = nf_portal_get_nf_mark(skb);
    nf_portal_set_mark(skb,PORTAL_PROXY_MARK);
    #endif

	/*特殊IP处理:例如10.1.0.6用于微信连、支付宝认证*/
	if(uiDestIp == g_uiSpecIp)
	{
	    if (pstIpHdr->protocol == IPPROTO_TCP)
		{
			if (unlikely(skb->len <(IP_HLEN +TCP_HLEN)))
			{
				NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"skb is too short(%u),but accept yet\n",skb->len);
				return NF_ACCEPT;
			}
			pstTcpHdr = (struct tcphdr *)((UINT8 *)(pstIpHdr) + (pstIpHdr->ihl)*4);
			
			if (unlikely(NULL == pstTcpHdr))
			{
				NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"tcp header is null,but accept yet\n");
				return NF_ACCEPT;
			}

			if (nf_portal_is_http_frame(ntohs(pstTcpHdr->dest))){
				NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"special ip frame,mark then accept\n");
				nf_portal_set_mark(skb,PORTAL_SPECIAL_MARK);
				return NF_ACCEPT;
			}
	    }
	}

	#if 0
    /*ADVERTISEMENT SUPPORT*/
    #ifdef AD_SUPPORT
    uiMark = nf_portal_get_nf_mark(skb);
    nf_portal_set_mark(skb,PORTAL_PROXY_MARK);
    if (pstIpHdr->protocol == IPPROTO_TCP)
    {
        if (likely(skb->len >=(IP_HLEN +TCP_HLEN)))
        {
            pstTcpHdr = (struct tcphdr *)((UINT8 *)(pstIpHdr) + (pstIpHdr->ihl)*4);
            if (likely(pstTcpHdr))
            {
                if (nf_portal_is_http_frame(ntohs(pstTcpHdr->dest))){
                    NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG," http frame !\n");

                    pstIpBawEntry = nf_portal_dest_ip_baw_get(uiDestIp,0);
                    if (NULL != pstIpBawEntry)
                    {
                        if ((pstIpBawEntry->stIpBAW.enFlag == FREE_ADVERTISING))
                        {
                            /*目的是免广告IP，去除标记*/
                            NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,IP_FMT"in free advertising ip  list,do not set mark ! uiMark %d \n",IP_ARG(&uiDestIp),uiMark);
                            nf_portal_set_mark(skb,uiMark);
                        }
                    }
                    
                    pstUrlIpBawEntry= nf_portal_url_ip_baw_get(uiDestIp);
                    if (NULL != pstUrlIpBawEntry)
                    {
                        pstUrlBaw = pstUrlIpBawEntry->pstUrlBaw;
                        if ((NULL != pstUrlBaw) && (pstUrlBaw->stUrlBAW.enFlag == FREE_ADVERTISING))
                        {
                            /*目的是免广告URL，去除标记*/
                            NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"%s in free advertising url  list,do not set mark !\n",pstUrlBaw->stUrlBAW.aucUrl);
                            nf_portal_set_mark(skb,uiMark);
                        }
                    }
                }
            }
        }
    }
    
    #endif
	#endif
    /*免认证端口*/
    /*20140220 免认证端口判断位置调整到这里
    MAC白名单只针对免认证端口生效
    白名单功能开发时没有对应的使用场景，暂时按照新的需求演进*/

    /*begin:lan口免认证修改:2015-9-6*/
    if(SW_ON == nf_portal_get_lan_free_auth_sw())
    {/*lan 口免认证开启*/
        //printk("LAN FREE: is On,sta_lan_port:%s\n",in->name);
        if(0 == strncmp("eth",in->name,3))
        {
           // printk("cmp LAN FREE: is On,sta_lan_port:%s\n",in->name);
            goto nf_accept;
        }
    }
    /*end:lan free*/
	
    if (NULL != nf_portal_free_auth_port_get((UINT8 *)(in->name)))
    {
        if (0 != nf_portal_get_mac_white_list_count())
        {
            pstMacBawEntry = nf_portal_mac_baw_get(aucMac);
            if (NULL == pstMacBawEntry)
            {
                NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,MAC_FMT"mac is not in the white list,drop\n",MAC_ARG(aucMac));
                return NF_DROP;
            }
            else
            {
                NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,MAC_FMT"mac is in the white list,accept\n",MAC_ARG(aucMac));
				goto nf_accept;
            }
        }
        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"free auth port:%s,accept\n",in->name);
		goto nf_accept;
    }
    /*用户MAC 黑白名单，免认证MAC相关*/
    /*20140220移除MAC白名单操作*/
#if 0
    if (0 != nf_portal_get_mac_white_list_count())
    {
        pstMacBawEntry = nf_portal_mac_baw_get(aucMac);
        if (NULL == pstMacBawEntry)
        {
            NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,MAC_FMT"mac is not in the white list,drop\n",MAC_ARG(aucMac));
            return NF_DROP;
        }
        else
        {
            NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,MAC_FMT"mac is in the white list,accept\n",MAC_ARG(aucMac));
            return NF_ACCEPT;
        }
    }
    else
#endif
    {
        pstMacBawEntry = nf_portal_mac_baw_get(aucMac);

        if (NULL != pstMacBawEntry)
        {
            if (pstMacBawEntry->stMacBAW.enFlag == BLACK)
            {
                NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,MAC_FMT"mac is in the black list,drop\n",MAC_ARG(aucMac));
                return NF_DROP;
            }
            else if (pstMacBawEntry->stMacBAW.enFlag == FREE_AUTH)
            {
                NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,MAC_FMT"mac is in the free auth list,accept\n",MAC_ARG(aucMac));
				goto nf_accept;
            }
        }
    }

    /*用户IP免认证*/
    pstIpBawEntry = nf_portal_user_ip_baw_get(uiIp);
    if (NULL != pstIpBawEntry)
    {
        if (pstIpBawEntry->stIpBAW.enFlag == FREE_AUTH)
        {
            NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,IP_FMT"user ip in the free auth list,accept\n",IP_ARG(&uiIp));
			goto nf_accept;
        }
    }
    
    /*解析四层报文，获取目的端口号*/
    if (pstIpHdr->protocol == IPPROTO_UDP)
    {
        if (unlikely(skb->len <(IP_HLEN +UDP_HLEN)))
        {
            NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"skb is too short(%u),but accept yet\n",skb->len);
            return NF_ACCEPT;
        }
        
        pstUdpHdr = (struct udphdr *)((UINT8 *)(pstIpHdr) + (pstIpHdr->ihl)*4);
        if (unlikely(NULL == pstUdpHdr))
        {
            NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"udp header is null,but accept yet\n");
            return NF_ACCEPT;
        }
        usDestPort = ntohs(pstUdpHdr->dest);
#ifdef HAVE_MULTI_WAN
		usSrcPort  = ntohs(pstUdpHdr->source);
		uiProto = 1;
#endif
        /*目的IP黑白名单，需要匹配端口号*/
        pstIpBawEntry = nf_portal_dest_ip_baw_get(uiDestIp,usDestPort);
        if (NULL != pstIpBawEntry)
        {
            if (pstIpBawEntry->stIpBAW.enFlag == BLACK)
            {
                NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,IP_FMT":%u,ip in the black list,drop\n",IP_ARG(&uiDestIp),usDestPort);
				uiListDrop = 1;
				goto ip_list_drop;
            }
            else if (pstIpBawEntry->stIpBAW.enFlag == WHITE)
            {
			#ifdef AD_SUPPORT
			nf_portal_set_mark(skb,uiMark);
			#endif
                NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,IP_FMT":%u,ip in the white list,accept\n",IP_ARG(&uiDestIp),usDestPort);
				goto nf_accept;
            }
        }
    }
    else if (pstIpHdr->protocol == IPPROTO_TCP)
    {
        if (unlikely(skb->len <(IP_HLEN +TCP_HLEN)))
        {
            NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"skb is too short(%u),but accept yet\n",skb->len);
            return NF_ACCEPT;
        }
        pstTcpHdr = (struct tcphdr *)((UINT8 *)(pstIpHdr) + (pstIpHdr->ihl)*4);

        if (unlikely(NULL == pstTcpHdr))
        {
            NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"tcp header is null,but accept yet\n");
            return NF_ACCEPT;
        }

        usDestPort = ntohs(pstTcpHdr->dest);
#ifdef HAVE_MULTI_WAN
		usSrcPort  = ntohs(pstTcpHdr->source);
		uiProto = 0;
#endif
        /*目的IP黑白名单，需要匹配端口号*/
        pstIpBawEntry = nf_portal_dest_ip_baw_get(uiDestIp,usDestPort);
        if (NULL != pstIpBawEntry)
        {
            if (pstIpBawEntry->stIpBAW.enFlag == BLACK)
            {
                NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,IP_FMT":%u,ip in the black list,drop\n",IP_ARG(&uiDestIp),usDestPort);
                uiListDrop = 1;
				goto ip_list_drop;
            }
            else if (pstIpBawEntry->stIpBAW.enFlag == WHITE)
            {
                NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,IP_FMT":%u,ip in the white list,accept\n",IP_ARG(&uiDestIp),usDestPort);
				#ifdef AD_SUPPORT
				nf_portal_set_mark(skb,uiMark);
				#endif
				goto nf_accept;
            }
			/*begin: add@2014.11.20 dest ip must_redirect */
            else if (pstIpBawEntry->stIpBAW.enFlag == MUST_REDIRECT)
            {
                NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,IP_FMT":%u,ip in the must redirect list,mark and redirect\n",IP_ARG(&uiDestIp),usDestPort);
                nf_portal_set_mark(skb,PORTAL_NORMAL_MARK+uiMarkOffset);
            	return NF_ACCEPT;
            }
            /*end: add@2014.11.20 dest ip must_redirect */
        }
    }
    /*针对端口号为0的表项，单独对IP地址做索引，再次查表*/
    pstIpBawEntry = nf_portal_dest_ip_baw_get(uiDestIp,0);
    if (NULL != pstIpBawEntry)
    {
        if (pstIpBawEntry->stIpBAW.enFlag == WHITE)
        {
            NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,IP_FMT"ip in the white list,accept\n",IP_ARG(&uiDestIp));
			goto nf_accept;
        }
        else if (pstIpBawEntry->stIpBAW.enFlag == BLACK)
        {
            NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,IP_FMT"ip in the black list,drop\n",IP_ARG(&uiDestIp));
            uiListDrop = 1;
			goto ip_list_drop;
        }
    }
    /*网段地址做索引*/
    pstNetBawEntry = nf_portal_check_dest_net_baw(uiDestIp);
    if (NULL != pstNetBawEntry)
    {
        if (pstNetBawEntry->stNetBAW.enFlag == WHITE)
        {
            NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,IP_FMT"net in the white list,accept\n",IP_ARG(&pstNetBawEntry->stNetBAW.uiNet));
			#ifdef AD_SUPPORT
			nf_portal_set_mark(skb,uiMark);
			#endif
			goto nf_accept;
        }
        else if (pstNetBawEntry->stNetBAW.enFlag == BLACK)
        {
            NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,IP_FMT"net in the black list,drop\n",IP_ARG(&pstNetBawEntry->stNetBAW.uiNet));
            uiListDrop = 1;
			goto ip_list_drop;
        }
    }

ip_list_drop:
	if(1 == uiListDrop){
		/*微信连白名单的报文仍然放行*/
		if (NF_PORTAL_IS_WXL_SERVER_IP == nf_portal_wxl_serverip_judge(uiDestIp)){
			#ifdef AD_SUPPORT
			nf_portal_set_mark(skb,uiMark);
			#endif
			goto nf_accept;
		}
		
		/*URL白名单的报文仍然放行*/
		pstUrlIpBawEntry= nf_portal_url_ip_baw_get(uiDestIp);
		if (NULL != pstUrlIpBawEntry)
		{
			pstUrlBaw = pstUrlIpBawEntry->pstUrlBaw;
			if ((NULL != pstUrlBaw) && (pstUrlBaw->stUrlBAW.enFlag == FREE_AUTH))
			{
				NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"%s in url free auth list, no mark then accept\n",pstUrlBaw->stUrlBAW.aucUrl);	
				#ifdef AD_SUPPORT
				nf_portal_set_mark(skb,uiMark);
				#endif
				goto nf_accept;
			}
		}
		
		return NF_DROP;
	}
	
    /*查找表项，流程优先考虑已认证终端性能*/
    pstEntry = nf_portal_get_sta_from_htable(aucMac);
    if (NULL == pstEntry)
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"nf_portal_add_sta_to_htable\n");
        pstEntry = nf_portal_add_sta_to_htable(aucMac,uiIp);
        if (unlikely(NULL == pstEntry))
        {
            NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"nf_portal_add_sta_to_htable fail\n");
			goto nf_accept;
        }
    }
	else
    {
        nf_portal_update_sta_from_htable(pstEntry,uiIp);
    }
    /*根据终端类型判断是否放行*/
    enStatype = nf_portal_get_sta_type(pstEntry);
    if(MODEL_PC == nf_portal_get_net_model())
    {
        if(2 == enStatype)
        {
            return NF_DROP;   
        }
    }
    if(MODEL_MOBILE == nf_portal_get_net_model())
    {
        if(1 == enStatype)
        {
            return NF_DROP;   
        }
    }
    
    enState = nf_portal_get_auth_state(pstEntry);
    /*已认证终端，直接放行*/
    if (enState == STATE_AUTH)
    {
        /*URL黑白名单*/
     //   if ((NULL != pstTcpHdr) && (nf_portal_is_http_frame(ntohs(pstTcpHdr->dest))))
        {
            pstUrlIpBawEntry= nf_portal_url_ip_baw_get(uiDestIp);
            if (0 != nf_portal_get_url_white_list_count()) /*存在URL白名单，只有白名单中的URL才能访问*/
            {
                if (NULL != pstUrlIpBawEntry)
                {
                    pstUrlBaw = pstUrlIpBawEntry->pstUrlBaw;
                    if ((NULL != pstUrlBaw) && (pstUrlBaw->stUrlBAW.enFlag == WHITE))
                    {
                        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"%s in url white list,accept\n",pstUrlBaw->stUrlBAW.aucUrl);           
                    }
                    else /*目的不是白名单中URL，丢弃*/
                    {
                        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"%s in url black or free auth list,drop\n",pstUrlBaw->stUrlBAW.aucUrl);
                        return NF_DROP;
                    }
                }
                else /*目的不是白名单中URL，丢弃*/
                {
                    /*do something*/
                    NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG," not in url white list,drop\n");
                    return NF_DROP;
                }
            }
            else
            {
                if (NULL != pstUrlIpBawEntry)
                {
                    pstUrlBaw = pstUrlIpBawEntry->pstUrlBaw;
                    if ((NULL != pstUrlBaw) && (pstUrlBaw->stUrlBAW.enFlag == BLACK))
                    {/*目的是黑名单中URL，丢弃*/
                        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"%s in url black list,drop\n",pstUrlBaw->stUrlBAW.aucUrl);
                        return NF_DROP;                
                    }
                    /*BEGIN: add@2014.11.02 for 必须重定向url名单*/
                    else if((NULL != pstUrlBaw) && (pstUrlBaw->stUrlBAW.enFlag == MUST_REDIRECT))
                    {
						NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"%s in url must_redirect list,mark and redirect\n",pstUrlBaw->stUrlBAW.aucUrl);
                        nf_portal_set_mark(skb,PORTAL_NORMAL_MARK+uiMarkOffset);
            			return NF_ACCEPT;
                    }
                    /*END: add@2014.11.02 for 必须重定向url名单*/
                }
            }
        }
        nf_portal_ts_update(pstEntry, NF_PORTAL_DIR_UP, skb->len);
        nf_portal_access_time_update(pstEntry);
        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,MAC_FMT"-"IP_FMT":already auth,accept\n",MAC_ARG(aucMac),IP_ARG(&uiIp));
		goto nf_accept;
    }
    else
    {
        nf_portal_access_time_update(pstEntry);
    }

    /*未认证终端，继续报文解析*/
	/*解析HTTPS报文*/
	if (pstIpHdr->protocol == IPPROTO_TCP){
		usDestPort = ntohs(pstTcpHdr->dest);
		if(443 == usDestPort){
			pstUrlIpBawEntry = nf_portal_url_ip_baw_get(uiDestIp);/*HTTPS报文白名单放行*/
		    if (NULL != pstUrlIpBawEntry)
		    {
		        pstUrlBaw = pstUrlIpBawEntry->pstUrlBaw;
		        if ((NULL != pstUrlBaw) && (pstUrlBaw->stUrlBAW.enFlag == HTTPS_FREE_AUTH))
		        {
		            NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"%s in https url free auth list\n",pstUrlBaw->stUrlBAW.aucUrl);           
					goto nf_accept;
		        }
		    }
		}
	}

    /*对于url白名单中的IP，全端口放行*/
	pstUrlIpBawEntry = nf_portal_url_ip_baw_get(uiDestIp);
    if (NULL != pstUrlIpBawEntry)
    {
        pstUrlBaw = pstUrlIpBawEntry->pstUrlBaw;
        if ((NULL != pstUrlBaw) && (pstUrlBaw->stUrlBAW.enFlag == FREE_AUTH))
        {
        		#ifdef AD_SUPPORT
			nf_portal_set_mark(skb,uiMark);
			#endif
            NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"%s in url free auth list\n",pstUrlBaw->stUrlBAW.aucUrl);           
			goto nf_accept;
        }
    }

	/*begin: add@2015.01.30 for wechat connect wifi*/
	/*判断目的ip是否在微信server ip 库*/
	if (NF_PORTAL_IS_WXL_SERVER_IP == nf_portal_wxl_serverip_judge(uiDestIp))
	{
		#ifdef AD_SUPPORT
		nf_portal_set_mark(skb,uiMark);
		#endif
		goto nf_accept;
	}
	/*end: add@2015.01.30 for wechat connect wifi*/

    /*UDP报文,DHCP报文和DNS报文放行，其余丢弃*/
    if (pstIpHdr->protocol == IPPROTO_UDP)
    {
        if (unlikely(skb->len <(IP_HLEN +UDP_HLEN)))
        {
            NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"skb is too short(%u),but accept yet\n",skb->len);
            return NF_ACCEPT;
        }
        
        pstUdpHdr = (struct udphdr *)((UINT8 *)(pstIpHdr) + (pstIpHdr->ihl)*4);
        if (unlikely(NULL == pstUdpHdr))
        {
            NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"udp header is null,but accept yet\n");
            return NF_ACCEPT;
        }
        
        if (nf_portal_is_dhcp_frame(ntohs(pstUdpHdr->source), ntohs(pstUdpHdr->dest)))
        {
            NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"dhcp frame,accept always\n");
            return NF_ACCEPT;
        }

        if (nf_portal_is_dns_frame(ntohs(pstUdpHdr->dest)))
        {
            NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"dns frame,accept always\n");
            return NF_ACCEPT;
        }

        if (NULL != nf_portal_port_num_get(ntohs(pstUdpHdr->dest),IPPROTO_UDP))
        {
            NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"port num(%u),accept always\n",ntohs(pstUdpHdr->dest));
			goto nf_accept;
        }
        /*其余丢弃*/
        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"drop all udp frames except dhcp & dns,port num(%u)\n",ntohs(pstUdpHdr->dest));
        return NF_DROP;
    }
    
    /*TCP报文*/
    if (pstIpHdr->protocol == IPPROTO_TCP)
    {
        if (unlikely(skb->len <(IP_HLEN +TCP_HLEN)))
        {
            NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"skb is too short(%u),but accept yet\n",skb->len);
            return NF_ACCEPT;
        }
        pstTcpHdr = (struct tcphdr *)((UINT8 *)(pstIpHdr) + (pstIpHdr->ihl)*4);

        if (unlikely(NULL == pstTcpHdr))
        {
            NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"tcp header is null,but accept yet\n");
            return NF_ACCEPT;
        }

        /*DNS报文*/
        if (nf_portal_is_dns_frame(ntohs(pstTcpHdr->dest)))
        {
            NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"dns frame,accept always\n");
            return NF_ACCEPT;
        }
        
        if (nf_portal_is_http_frame(ntohs(pstTcpHdr->dest)))
        {
	        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"http frame,mark then accept\n");
	        nf_portal_set_mark(skb,PORTAL_NORMAL_MARK+uiMarkOffset);
			
			return NF_ACCEPT;
        }

#if 0 /*前面已经放行了URL白名单中的所有报文*/
        /*放行端口*/
        /*2013 02 19 修改:TCP放行端口仅对免认证url有效*/
        pstUrlIpBawEntry= nf_portal_url_ip_baw_get(uiDestIp);
        if (NULL != pstUrlIpBawEntry)
        {
            pstUrlBaw = pstUrlIpBawEntry->pstUrlBaw;
            if ((NULL != pstUrlBaw) && (pstUrlBaw->stUrlBAW.enFlag == FREE_AUTH) &&\
                (NULL != nf_portal_port_num_get(ntohs(pstTcpHdr->dest),IPPROTO_TCP)))
            {
                NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"%s in url free auth list, port num(%u),accept always\n",\
                    pstUrlBaw->stUrlBAW.aucUrl,ntohs(pstTcpHdr->dest));           
				goto nf_accept;
            }
        }
#endif
        /*其余丢弃*/
        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"drop all tcp frames except http & dns,port num (%u)\n",ntohs(pstTcpHdr->dest));
        return NF_DROP;
    }
    
    //NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"other protos,but accept yet\n");
    //return NF_ACCEPT;
    NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"other protos,drop all\n");
    return NF_DROP;
    
nf_accept:
    if( SW_ON != nf_portal_get_net_connect())
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"Net_Connect is down ,drop all\n");
        return NF_DROP;
    }

#ifdef HAVE_MULTI_WAN
	//nf_portal_set_mark(skb,PORTAL_NORMAL_MARK);
	//pstEthHdr pstIpHdr 一定不为空
	nf_portal_set_forward_mark(skb, uiIp, uiProto, usSrcPort);
#endif
	return NF_ACCEPT;	
}
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20))
LOCAL UINT32 nf_portal_post_routing_hook(UINT32 hooknum,
		     struct sk_buff **pstSkb,
		     const struct net_device *in,
		     const struct net_device *out,
		     int (*okfn)(struct sk_buff *))
#else
LOCAL UINT32 nf_portal_post_routing_hook(UINT32 hooknum,
		     struct sk_buff *skb,
		     const struct net_device *in,
		     const struct net_device *out,
		     int (*okfn)(struct sk_buff *))
#endif
{
    struct ethhdr *pstEthHdr = NULL;
    struct iphdr *pstIpHdr = NULL;
    UINT8 aucMac[ETH_ALEN];
    UINT32 uiIp;
    NF_PORTAL_STA_ENTRY_T *pstEntry = NULL;
    NF_PORTAL_AUTH_STATE_E enState = STATE_MAX;
    UINT32 uiOffset = 0;
    struct udphdr *pstUdpHdr = NULL;
    /*全局免认证开关，亦是模块功能开关*/
    if (SW_ON == nf_portal_get_free_auth_sw())
    {
        return NF_ACCEPT;
    }

    #if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20))
    struct sk_buff *skb = NULL;
    if (unlikely(NULL == pstSkb))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"pstSkb is null,but accept yet\n");
        return NF_ACCEPT;
    }
    skb = *pstSkb;
    #endif
    /*参数检查*/
    if (unlikely(NULL == skb))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"skb is null,but accept yet\n");
        return NF_ACCEPT;
    }
    if (unlikely(NULL == out))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"out device is null,but accept yet\n");
        return NF_ACCEPT;
    }

    //NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"IN->NAME=%s,OUT->NAME=%s\n",(in == NULL)?"NULL":(in->name),(out == NULL)?"NULL":(out->name));

    /*只对指定桥报文做处理*/
    if ( FALSE == nf_portal_is_lan_if(out))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"not in lan br,accept\n");
        return NF_ACCEPT;
    }
    /*解析报文二层和三层信息获取MAC地址和IP地址*/

    pstEthHdr = eth_hdr(skb);
    if (unlikely(NULL == pstEthHdr))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"eth header is null,but accept yet\n");
        return NF_ACCEPT;
    }
    memcpy(aucMac,pstEthHdr->h_dest,ETH_ALEN);
    
    if (htons(ETH_P_IP) != nf_portal_get_proto(skb,&uiOffset))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"no ip pkt,accept\n");
        return NF_ACCEPT;    
    }
    
    if (unlikely(skb->len <IP_HLEN))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"skb is too short(%u),but accept yet\n",skb->len);
        return NF_ACCEPT; 
    }
    pstIpHdr =(struct iphdr *)((UINT8 *)eth_hdr(skb) +ETH_HLEN +uiOffset);

    if (unlikely(NULL == pstIpHdr))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"ip header is null,but accept yet\n");
        return NF_ACCEPT;
    }
    uiIp = ntohl(pstIpHdr->daddr);

    /*解析四层报文*/
    if (pstIpHdr->protocol == IPPROTO_UDP)
    {
        if (unlikely(skb->len <(IP_HLEN +UDP_HLEN)))
        {
            NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"skb is too short(%u),but accept yet\n",skb->len);
            return NF_ACCEPT;
        }
        
        pstUdpHdr = (struct udphdr *)((UINT8 *)(pstIpHdr) + (pstIpHdr->ihl)*4);
        if (unlikely(NULL == pstUdpHdr))
        {
            NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"udp header is null,but accept yet\n");
            return NF_ACCEPT;
        }

        /*解析DNS报文获取URL和IP地址*/
        if (nf_portal_is_dns_frame(ntohs(pstUdpHdr->source)))
        {
        	if((ntohs(pstIpHdr->tot_len) - (pstIpHdr->ihl)*4) >= ntohs(pstUdpHdr->len))
            	nf_portal_dns_resolve(skb,pstUdpHdr);
        }
    }
    /*只做统计之用，不丢弃报文*/
    pstEntry = nf_portal_get_sta_from_htable(aucMac);
    if (pstEntry != NULL)
    {
        enState = nf_portal_get_auth_state(pstEntry);

        if (enState == STATE_AUTH)
        {
            NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,MAC_FMT"-"IP_FMT":already auth,accept\n",MAC_ARG(aucMac),IP_ARG(&uiIp));
            nf_portal_ts_update(pstEntry, NF_PORTAL_DIR_DOWN, skb->len);
            nf_portal_access_time_update(pstEntry);
            return NF_ACCEPT;
        }
        else
        {
            NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,MAC_FMT"-"IP_FMT":not auth,but accept yet\n",MAC_ARG(aucMac),IP_ARG(&uiIp));
            nf_portal_access_time_update(pstEntry);
            return NF_ACCEPT;
        }
    }

    /*suitable?*/
    NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,MAC_FMT"-"IP_FMT":others,accept yet\n",MAC_ARG(aucMac),IP_ARG(&uiIp));
    return NF_ACCEPT;
}
/* BEGIN: Added by WeiHonggang, 2014/3/5   PN:BUG #5088不同VLAN接口加到同一个桥下会导致的广播环路问题  */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20))
LOCAL UINT32 nf_portal_forwarding_hook(UINT32 hooknum,
		     struct sk_buff **pstSkb,
		     const struct net_device *in,
		     const struct net_device *out,
		     int (*okfn)(struct sk_buff *))
#else
LOCAL UINT32 nf_portal_forwarding_hook(UINT32 hooknum,
		     struct sk_buff *skb,
		     const struct net_device *in,
		     const struct net_device *out,
		     int (*okfn)(struct sk_buff *))
#endif
{  
    #if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20))
    struct sk_buff *skb = NULL;
    if (unlikely(NULL == pstSkb))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"pstSkb is null,but accept yet\n");
        return NF_ACCEPT;
    }
    skb = *pstSkb;
    #endif
    /*参数检查*/
    if (unlikely(NULL == skb))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"skb is null,but accept yet\n");
        return NF_ACCEPT;
    }
    if (unlikely(NULL == in) || unlikely(NULL == out))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"in or out device is null,but accept yet\n");
        return NF_ACCEPT;
    }
    NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"IN->NAME=%s,OUT->NAME=%s\n",(in == NULL)?"NULL":(in->name),(out == NULL)?"NULL":(out->name));
    /*同一物理设备虚拟出的vlan端口互相隔离*/
	if(!g_bVlanFloodEnable){
	    if (TRUE == real_dev_is_same(in,out))
	    {
	        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"real dev is the same,drop pkt\n");
	        return NF_DROP;
	    }
	}
    return NF_ACCEPT;
}
/* END:   Added by WeiHonggang, 2014/3/5   PN:BUG #5088不同VLAN接口加到同一个桥下会导致的广播环路问题  */
LOCAL struct nf_hook_ops portal_hooks[NF_PORTAL_HOOK_NUM] = {
    {
        .hook = nf_portal_pre_routing_hook,
        .owner = THIS_MODULE,
        //.pf = PF_INET,
        //.hooknum = NF_INET_PRE_ROUTING,
        .pf = PF_BRIDGE,
        .hooknum = NF_BR_PRE_ROUTING,
        .priority	= NF_IP_PRI_PORTAL,
    },
{
        .hook = nf_portal_post_routing_hook,
        .owner = THIS_MODULE,
        //.pf = PF_INET,
        //.hooknum = NF_INET_POST_ROUTING,
        .pf = PF_BRIDGE,
        .hooknum = NF_BR_POST_ROUTING,
        .priority	= NF_IP_PRI_PORTAL,
},
/* BEGIN: Added by WeiHonggang, 2014/3/5   PN:BUG #5088不同VLAN接口加到同一个桥下会导致的广播环路问题  */
{
        .hook = nf_portal_forwarding_hook,
        .owner = THIS_MODULE,
        .pf = PF_BRIDGE,
        .hooknum = NF_BR_FORWARD,
        .priority	= NF_IP_PRI_PORTAL,
},
/* END:   Added by WeiHonggang, 2014/3/5   PN:BUG #5088不同VLAN接口加到同一个桥下会导致的广播环路问题  */
};

AP_ERROR_CODE_E nf_portal_hook_init(VOID)
{
    #if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20))    
    UINT32 uiLoop;
    for (uiLoop = 0; uiLoop < NF_PORTAL_HOOK_NUM;uiLoop++)
    {
        nf_register_hook(&portal_hooks[uiLoop]);
    }
    br_nf_portal_pre_routing_hook = nf_portal_pre_routing_hook;
    #else
    nf_register_hooks(portal_hooks,NF_PORTAL_HOOK_NUM);
    #endif

    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"register hooks\n");
    return AP_E_NONE;
}
AP_ERROR_CODE_E nf_portal_hook_destroy(VOID)
{
    #if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20))    
    UINT32 uiLoop;
    for (uiLoop = 0; uiLoop < NF_PORTAL_HOOK_NUM;uiLoop++)
    {
        nf_unregister_hook(&portal_hooks[uiLoop]);
    }
    br_nf_portal_pre_routing_hook = NULL;
    #else
    nf_unregister_hooks(portal_hooks,NF_PORTAL_HOOK_NUM);
    #endif
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"unregister hooks\n");
    return AP_E_NONE;
}
AP_ERROR_CODE_E nf_portal_init(void)
{
    UINT32 uiLoop;
    nf_portal_hook_init();
    nf_portal_hash_init();
    nf_portal_timer_init();
#ifdef HAVE_MULTI_WAN
	nf_portal_wancfg_init();
#endif
    for (uiLoop = 0; uiLoop < sizeof(g_astLetPassPort)/sizeof(NF_PORTAL_PORT_NUM_ATTRIB_T);uiLoop++)
    {
        nf_portal_port_num_add(g_astLetPassPort[uiLoop].usPortNum,g_astLetPassPort[uiLoop].ucIpProto);
    }
 
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO, "NF_PORATL init.\n");
    return AP_E_NONE;
}

AP_ERROR_CODE_E nf_portal_destroy(void)
{
    nf_portal_hook_destroy();
    nf_portal_timer_destroy();
    nf_portal_hash_destroy();
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO, "NF_PORTAL destroy.\n");
    return AP_E_NONE;
}

