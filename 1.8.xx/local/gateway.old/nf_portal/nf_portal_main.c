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
UINT8 g_aucLanIfName[IFNAMSIZ] = DEF_LAN_IF_NAME;
UINT32 g_uiHostIp = DEF_HOST_IP;
UINT32 g_uiAuthServerIp = DEF_AHTH_SERVER_IP;
UINT32 g_uiCCSServerIp = DEF_CCS_SERVER_IP;
UINT32 g_uiSNMPServerIp = DEF_SNMP_SERVER_IP;

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
UINT8 *nf_portal_get_lan_name(VOID)
{
    return g_aucLanIfName;
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
    if ((0 == memcmp(in->name,VLAN_DEV_PREFIX,(sizeof(VLAN_DEV_PREFIX)-1)))    \
        &&(0 == memcmp(out->name,VLAN_DEV_PREFIX,(sizeof(VLAN_DEV_PREFIX)-1))))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/* END:   Added by WeiHonggang, 2014/3/5   PN:BUG #5088不同VLAN接口加到同一个桥下会导致的广播环路问题  */


void printcode(unsigned char *ptr, u32 ulLen, u32 ulLevel)
{
	u32 	ulLoop;
	char aucBuf[128];
	u32 	ulDataLen = 128;
	u32 	ulLine;
	unsigned char *p;
	
	if (ulLen < ulDataLen )
		ulDataLen = ulLen;
	
	ulLine = ulDataLen/16;
    
    	if(ulLevel >= g_enDebugLevel) 
		return; 

	p = ptr;
    	
    	for(ulLoop=0; ulLoop < ulLine; ulLoop++)
    	{
        	printk("%02x %02x %02x %02x %02x %02x %02x %02x-%02x %02x %02x %02x %02x %02x %02x %02x\n", p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7],p[8],p[9],p[10],p[11],p[12],p[13],p[14],p[15]);
        	p = p+16;
    	} 
    
    	aucBuf[0] = 0;
    	for (ulLoop=0; ulLoop < ulLen%16; ulLoop++)
    	{
        	sprintf(aucBuf + strlen(aucBuf), "%02x ", p[ulLoop]);    
    	}
    	printk(aucBuf);
    
    	printk("\n");
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
    UINT32 uiIp;
    UINT32 uiDestIp;
    UINT16 usDestPort;
    NF_PORTAL_STA_ENTRY_T *pstEntry = NULL;
    NF_PORTAL_AUTH_STATE_E enState = STATE_MAX;
    UINT32 uiOffset = 0;
    NF_PORTAL_IP_BAW_ENTRY_T *pstIpBawEntry = NULL;
    NF_PORTAL_MAC_BAW_ENTRY_T *pstMacBawEntry = NULL;
    NF_PORTAL_URL_IP_BAW_ENTRY_T *pstUrlIpBawEntry = NULL;
    NF_PORTAL_URL_BAW_ENTRY_T *pstUrlBaw = NULL;
    /*全局免认证开关，亦是模块功能开关*/
    if (SW_ON == nf_portal_get_free_auth_sw())
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"free auth sw enable,accept\n");
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
    //pstEthHdr = eth_hdr(skb);
    pstEthHdr = skb_mac_header(skb);
    if (unlikely(NULL == pstEthHdr))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"eth header is null,but accept yet\n");
        return NF_ACCEPT;
    }
    memcpy(aucMac,pstEthHdr->h_source,ETH_ALEN);

    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"skb->len %d  in dev :%s src "MAC_FMT"  dst "MAC_FMT"   \n",skb->len,in->name,MAC_ARG(pstEthHdr->h_source),MAC_ARG(pstEthHdr->h_dest));

    printcode((u8 *)pstEthHdr, 128,NF_PORTAL_LOG_DEBUG);


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
    if ((uiDestIp == g_uiHostIp) ||(uiDestIp == g_uiAuthServerIp) || (uiDestIp == g_uiCCSServerIp) ||
		(uiDestIp == g_uiSNMPServerIp))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,IP_FMT"to host or auth server or ccs server,accept\n",IP_ARG(&uiDestIp));
        return NF_ACCEPT;
    }
	
    /*免认证端口*/
    /*20140220 免认证端口判断位置调整到这里
    MAC白名单只针对免认证端口生效
    白名单功能开发时没有对应的使用场景，暂时按照新的需求演进*/
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
                return NF_ACCEPT;
            }
        }
        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"free auth port:%s,accept\n",in->name);
        return NF_ACCEPT;
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
                return NF_ACCEPT;
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
            return NF_ACCEPT;
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
        /*目的IP黑白名单*/
        pstIpBawEntry = nf_portal_dest_ip_baw_get(uiDestIp,usDestPort);
        if (NULL != pstIpBawEntry)
        {
            if (pstIpBawEntry->stIpBAW.enFlag == BLACK)
            {
                NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,IP_FMT":%u,ip in the black list,drop\n",IP_ARG(&uiDestIp),usDestPort);
                return NF_DROP;
            }
            else if (pstIpBawEntry->stIpBAW.enFlag == WHITE)
            {
                NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,IP_FMT":%u,ip in the white list,accept\n",IP_ARG(&uiDestIp),usDestPort);
                return NF_ACCEPT;
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
        /*目的IP黑白名单*/
        pstIpBawEntry = nf_portal_dest_ip_baw_get(uiDestIp,usDestPort);
        if (NULL != pstIpBawEntry)
        {
            if (pstIpBawEntry->stIpBAW.enFlag == BLACK)
            {
                NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,IP_FMT":%u,ip in the black list,drop\n",IP_ARG(&uiDestIp),usDestPort);
                return NF_DROP;
            }
            else if (pstIpBawEntry->stIpBAW.enFlag == WHITE)
            {
                NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,IP_FMT":%u,ip in the white list,accept\n",IP_ARG(&uiDestIp),usDestPort);
                return NF_ACCEPT;
            }
            /*begin: add@2014.11.20 dest ip must_redirect */
            else if (pstIpBawEntry->stIpBAW.enFlag == MUST_REDIRECT)
            {
                NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,IP_FMT":%u,ip in the must redirect list,mark and redirect\n",IP_ARG(&uiDestIp),usDestPort);
                nf_portal_set_mark(skb,PORTAL_NORMAL_MARK);
            	return NF_ACCEPT;
            }
            /*end: add@2014.11.20 dest ip must_redirect */
        }
    }
    /*单独对IP地址做索引，再次查表*/
    pstIpBawEntry = nf_portal_dest_ip_baw_get(uiDestIp,0);
    if (NULL != pstIpBawEntry)
    {
        if (pstIpBawEntry->stIpBAW.enFlag == WHITE)
        {
            NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,IP_FMT"ip in the white list,accept\n",IP_ARG(&uiDestIp));
            return NF_ACCEPT;
        }
        else if (pstIpBawEntry->stIpBAW.enFlag == BLACK)
        {
            NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,IP_FMT"ip in the black list,drop\n",IP_ARG(&uiDestIp));
            return NF_DROP;
        }
    }
    
    /*查找表项，流程优先考虑已认证终端性能*/
    pstEntry = nf_portal_get_sta_from_htable(aucMac,uiIp);
    if (NULL == pstEntry)
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"nf_portal_add_sta_to_htable\n");
        pstEntry = nf_portal_add_sta_to_htable(aucMac,uiIp);
        if (unlikely(NULL == pstEntry))
        {
            NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"nf_portal_add_sta_to_htable fail\n");
            return NF_ACCEPT;
        }
    }

    enState = nf_portal_get_auth_state(pstEntry);
    /*已认证终端，直接放行*/
    if (enState == STATE_AUTH)
    {
        /*URL黑白名单*/
        if ((NULL != pstTcpHdr) && (nf_portal_is_http_frame(ntohs(pstTcpHdr->dest))))
        {
            pstUrlIpBawEntry= nf_portal_url_ip_baw_get(uiDestIp);
            if (0 != nf_portal_get_url_white_list_count())
            {
                if (NULL != pstUrlIpBawEntry)
                {
                    pstUrlBaw = pstUrlIpBawEntry->pstUrlBaw;
                    if ((NULL != pstUrlBaw) && (pstUrlBaw->stUrlBAW.enFlag == WHITE))
                    {
                        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"%s in url white list,accept\n",pstUrlBaw->stUrlBAW.aucUrl);           
                    }
                    else
                    {
                        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"%s in url black or free auth list,drop\n",pstUrlBaw->stUrlBAW.aucUrl);
                        return NF_DROP;  
                    }
                }
                else
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
                    {
                        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"%s in url black list,drop\n",pstUrlBaw->stUrlBAW.aucUrl);
                        return NF_DROP;                   
                    }
                    /*BEGIN: add@2014.11.02 for 必须重定向url名单*/
                    else if((NULL != pstUrlBaw) && (pstUrlBaw->stUrlBAW.enFlag == MUST_REDIRECT))
                    {
						NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"%s in url must_redirect list,mark and redirect\n",pstUrlBaw->stUrlBAW.aucUrl);
                        nf_portal_set_mark(skb,PORTAL_NORMAL_MARK);
            			return NF_ACCEPT;
                    }
                    /*END: add@2014.11.02 for 必须重定向url名单*/
                }
            }
        }
        nf_portal_ts_update(pstEntry, NF_PORTAL_DIR_UP, skb->len);
        nf_portal_access_time_update(pstEntry);
        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,MAC_FMT"-"IP_FMT":already auth,accept\n",MAC_ARG(aucMac),IP_ARG(&uiIp));
        return NF_ACCEPT;
    }
    else
    {
        nf_portal_access_time_update(pstEntry);
    }

    /*未认证终端，继续报文解析*/
    /*对于url白名单中的IP，全端口放行*/
	pstUrlIpBawEntry= nf_portal_url_ip_baw_get(uiDestIp);
    if (NULL != pstUrlIpBawEntry)
    {
        pstUrlBaw = pstUrlIpBawEntry->pstUrlBaw;
        if ((NULL != pstUrlBaw) && (pstUrlBaw->stUrlBAW.enFlag == FREE_AUTH))
        {
            NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"%s in url free auth list, no mark then accept\n",pstUrlBaw->stUrlBAW.aucUrl);           
            return NF_ACCEPT;
        }
    }

	/*begin: add@2015.01.30 for wechat connect wifi*/
	/*判断目的ip是否在微信server ip 库*/
	if (NF_PORTAL_IS_WXL_SERVER_IP == nf_portal_wxl_serverip_judge(uiDestIp))
	{
		return NF_ACCEPT;
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
            return NF_ACCEPT;
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
            nf_portal_set_mark(skb,PORTAL_NORMAL_MARK);
            return NF_ACCEPT;
            
        }
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
                return NF_ACCEPT;
            }
        }

        /*其余丢弃*/
        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"drop all tcp frames except http & dns,port num (%u)\n",ntohs(pstTcpHdr->dest));
        return NF_DROP;
    }
    
    //NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"other protos,but accept yet\n");
    //return NF_ACCEPT;
    NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"other protos,drop all\n");
    return NF_DROP;
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
    pstEntry = nf_portal_get_sta_from_htable(aucMac,uiIp);
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
    if (TRUE == real_dev_is_same(in,out))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"real dev is the same,drop pkt\n");
        return NF_DROP;
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

