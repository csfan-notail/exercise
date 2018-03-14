/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : nf_portal_main.h
  �� �� ��   : ����
  ��    ��   : WeiHonggang
  ��������   : 2013��11��22�� ������
  ����޸�   :
  ��������   : nf_portal_main.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��11��22�� ������
    ��    ��   : WeiHonggang
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __NF_PORTAL_MAIN_H__
#define __NF_PORTAL_MAIN_H__


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
#if (defined(GW_X86) || defined(GW_ARM))   //����x86���ص��ں˰汾     by cj
#include <linux/rwlock_types.h>
#include <../net/bridge/br_private.h>
#if defined(GW_ARM)
#ifndef BIG_ENDIAN
#define BIG_ENDIAN 
#endif
#endif
#else
#include "br_private.h"
#if (!defined(GW_AC))   //by cj
#ifndef BIG_ENDIAN
#define BIG_ENDIAN 
#endif
#endif
#endif
#include "ap_types.h"
#include "nf_portal_public.h"

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
//#define NF_IP_PRI_PORTAL (NF_IP_PRI_MANGLE-1) /*���Ӻ������ȼ�*/
#define NF_IP_PRI_PORTAL (NF_BR_PRI_FILTER_BRIDGED-1) /*���Ӻ������ȼ�*/

/* BEGIN: Added by WeiHonggang, 2014/3/5   PN:BUG #5088��ͬVLAN�ӿڼӵ�ͬһ�����»ᵼ�µĹ㲥��·����  */
//#define NF_PORTAL_HOOK_NUM 2 /*���Ӻ�����Ŀ*/
#define NF_PORTAL_HOOK_NUM 3 /*���Ӻ�����Ŀ*/
#define VLAN_DEV_PREFIX "eth1"
/* END:   Added by WeiHonggang, 2014/3/5   PN:BUG #5088��ͬVLAN�ӿڼӵ�ͬһ�����»ᵼ�µĹ㲥��·����  */
#define IP_HLEN 20 /*IP ͷ����*/
#define UDP_HLEN 8 /*UDPͷ����*/
#define TCP_HLEN 20 /*TCPͷ����*/
#define DNS_HLEN 12 /*DNSͷ����*/
#define BOOTPC 0x44
#define BOOTPS 0x43
#define DNSS 0x35
#define HTTPS 0x50
/* add@2014.9.24 PN:�к˸�ͨ������Ҫ����8008�˿ڵ�tcp���ģ����ض���*/
#define HTTPS_FORCUSCOM 0x1F48

#define VLAN_HLEN 4

#define PORTAL_NORMAL_MARK  1
#define PORTAL_SPECIAL_MARK 2

#define DEF_LAN_IF_NAME "br0"
#define DEF_HOST_IP 0xC0A81401
#define DEF_AHTH_SERVER_IP 0x0A3C0088
#define DEF_CCS_SERVER_IP 0x0A3C0088
#define DEF_SNMP_SERVER_IP 0x0A3C0088

#if defined (BIG_ENDIAN)
#define DNS_FLAG_QR_R 1
#define DNS_FLAG_OPCODE_S 0
#define DNS_FLAG_TC_NO 0
#define DNS_FLAG_RCODE_OK 0

#else
#define ROOT_DNS_SERVER_FLAGS 0x8180
#define AUTHORITY_DNS_SERVER_FLAGS 0x8580
#endif
#define DNS_ANSWER_TYPE_A 0x0001
/*add@2015.01.31 for wechat connect wifi*/
#define NF_PORTAL_IS_WXL_SERVER_IP 1
#define NF_PORTAL_ISNOT_WXL_SERVER_IP 0


 /*----------------------------------------------*
 * �����                                       *
 *----------------------------------------------*/
#define NF_PORTAL_LOG(level, fmt, args...)                  \
do{											    \
	if (level <= g_enDebugLevel)					        \
	{											\
		printk("NF_PORTAL %s(%d): ",__FUNCTION__, __LINE__); \
		printk(fmt, ##args);				    \
	}										    \
}while(0)

/*----------------------------------------------*
 * ö�ٶ���                                       *
 *----------------------------------------------*/
typedef enum
{
    NF_PORTAL_LOG_EMERG = 0,	/* system is unusable */
    NF_PORTAL_LOG_ALERT,	/* action must be taken immediately */
    NF_PORTAL_LOG_CRIT,	/* critical conditions */
    NF_PORTAL_LOG_ERR,	/* error conditions */
    NF_PORTAL_LOG_WARNING,	/* warning conditions */
    NF_PORTAL_LOG_NOTICE,	/* normal but significant condition */
    NF_PORTAL_LOG_INFO,	/* informational */
    NF_PORTAL_LOG_DEBUG,	/* debug-level messages */
}NF_PORTAL_LOG_LEVEL_E;

/*----------------------------------------------*
 * ��չ���Ͷ���                                     *
 *----------------------------------------------*/


/*----------------------------------------------*
 * �ṹ����                                       *
 *----------------------------------------------*/

typedef struct
{
    NF_PORTAL_STA_ATTRIB_T stSta;/*STA ������Ϣ */
    ULONG ulAccessTime; /*���·���ʱ��*/
    struct hlist_node stStaHList;/*������֯STA Hash��*/
}NF_PORTAL_STA_ENTRY_T;

typedef struct
{
    struct ethhdr stEthHdr;
    UINT16 usVlanId;
    UINT16 usProto;
}__attribute__((packed)) NF_PORTAL_8021Q_HDR_T;
typedef struct
{
    NF_PORTAL_8021Q_HDR_T st8021qHdr;
    UINT16 usVlanId;
    UINT16 usProto;
}__attribute__((packed)) NF_PORTAL_QINQ_HDR_T;

typedef struct
{
    UINT16 usTransactionId;/*�Ự��ʶ*/
    #if defined (BIG_ENDIAN) /*��־λ*/
    UINT16 QR:1,
               opcode:4,
               AA:1,
               TC:1,
               RD:1,
               RA:1,
               zero:3,
               rcode:4;
    #else
    /*
    UINT16 RA:1,
               zero:3,
               rcode:4,
               QR:1,
               opcode:4,
               AA:1,
               TC:1,
               RD:1;
   */
   UINT16 usFlags;
    #endif
    UINT16 usQuestions;/*������*/
    UINT16 usAnswerRRS;/*Ӧ����*/
    UINT16 usAuthorityRRS;/*��Ȩ��*/
    UINT16 usAdditionalRRS;/*������Ϣ��*/
}__attribute__((packed)) NF_PORTAL_DNS_HDR_T;

typedef struct
{
    NF_PORTAL_IP_BAW_ATTRIB_T stIpBAW;
    struct hlist_node stIpBAWHList;
}NF_PORTAL_IP_BAW_ENTRY_T;
typedef struct
{
    UINT8 aucPortName[IFNAMSIZ];
    struct hlist_node stPortHList;
}NF_PORTAL_PORT_ENTRY_T;

typedef struct
{
    NF_PORTAL_MAC_BAW_ATTRIB_T stMacBAW;
    struct hlist_node stMacBAWHList;
}NF_PORTAL_MAC_BAW_ENTRY_T;

typedef struct
{
    NF_PORTAL_URL_BAW_ATTRIB_T stUrlBAW;
    struct list_head stUrlBAWList;
    struct list_head stIpList;
}NF_PORTAL_URL_BAW_ENTRY_T;

typedef struct
{
    NF_PORTAL_IP_BAW_ATTRIB_T stIpBAW;
    struct hlist_node stIpBAWHList;
    struct list_head stIpList;
    NF_PORTAL_URL_BAW_ENTRY_T *pstUrlBaw;
    UINT8 aucRealUrl[URL_MAX_SIZE];
}NF_PORTAL_URL_IP_BAW_ENTRY_T;

typedef struct
{
    NF_PORTAL_PORT_NUM_ATTRIB_T stPortNum;
    struct hlist_node stPortNumHList;
}NF_PORTAL_PORT_NUM_ENTRY_T;

/*begin: add@2015.01.31 for wechat connect wifi*/
typedef struct
{
	UINT32 uiMask;
	UINT32 uiIp;
	struct hlist_node stWxlServerIpHlist;
}NF_PORTAL_WXL_SERVERIP_ENTRY_T;
/*end: add@2015.01.31 for wechat connect wifi*/

/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������ԭ��˵��                             *
 *----------------------------------------------*/
 
AP_ERROR_CODE_E nf_portal_set_debugLevel(UINT32 uiDebugLevel);
UINT32 nf_portal_get_debugLevel(void);

AP_ERROR_CODE_E nf_portal_init(void);
AP_ERROR_CODE_E nf_portal_destroy(void);
UINT32 nf_portal_get_host_ip(VOID);
AP_ERROR_CODE_E nf_portal_set_host_ip(UINT32 uiIp);
AP_ERROR_CODE_E nf_portal_set_auth_server_ip(UINT32 uiIp);
UINT32 nf_portal_get_auth_server_ip(VOID);
AP_ERROR_CODE_E nf_portal_set_free_auth_sw(NF_PORTAL_SW_STATE_E enGlobalFreeAuth);
NF_PORTAL_SW_STATE_E nf_portal_get_free_auth_sw(VOID);
AP_ERROR_CODE_E nf_portal_set_lan_name(UINT8 *pucLanIfName);
UINT8 *nf_portal_get_lan_name(VOID);
AP_ERROR_CODE_E nf_portal_set_ccs_server_ip(UINT32 uiIp);
UINT32 nf_portal_get_ccs_server_ip(VOID);
AP_ERROR_CODE_E nf_portal_set_snmp_server_ip(UINT32 uiIp);
UINT32 nf_portal_get_snmp_server_ip(VOID);
/*----------------------------------------------*
 * ��������˵��                             *
 *----------------------------------------------*/
REMOTE NF_PORTAL_LOG_LEVEL_E g_enDebugLevel;

#endif /* __NF_PORTAL_MAIN_H__ */

