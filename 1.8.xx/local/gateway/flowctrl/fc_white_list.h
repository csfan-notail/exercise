/******************************************************************************

                  ��Ȩ���� (C), 2010-2012, �Ϻ�徴�ͨ�ſƼ����޹�˾

 ******************************************************************************
  �� �� ��   : fc_white_list.h
  �� �� ��   : ����
  ��    ��   : WeiHonggang
  ��������   : 2015��8��18��
  ����޸�   :
  ��������   : fc_white_list.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��8��18��
    ��    ��   : WeiHonggang
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/

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
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/

#ifndef __FC_WHITE_LIST_H__
#define __FC_WHITE_LIST_H__

#define FC_DEST_IP_HLIST_SIZE 256 /*Ŀ��IP �����������С*/

#define FC_DEST_IP_HASH_RD_LOCK            read_lock_bh(&g_stDestIpHashLock)
#define FC_DEST_IP_HASH_RD_UNLOCK        read_unlock_bh(&g_stDestIpHashLock)
#define FC_DEST_IP_HASH_WR_LOCK           write_lock_bh(&g_stDestIpHashLock)
#define FC_DEST_IP_HASH_WR_UNLOCK       write_unlock_bh(&g_stDestIpHashLock)

#define IP_HLEN 20 /*IP ͷ����*/
#define ARP_HLEN  28/*ARPͷ����*/
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
