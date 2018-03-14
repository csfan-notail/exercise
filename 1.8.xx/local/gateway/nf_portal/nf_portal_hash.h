/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : nf_portal_hash.h
  版 本 号   : 初稿
  作    者   : Wei Honggang 
  生成日期   : 2013年8月3日 星期六
  最近修改   :
  功能描述   : nf_portal_hash.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2013年8月3日 星期六
    作    者   : Wei Honggang
    修改内容   : 创建文件

******************************************************************************/

#ifndef __NF_PORTAL_HASH_H__
#define __NF_PORTAL_HASH_H__


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
 * 基础类型定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
 
/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define NF_PORTAL_STA_HASH_SIZE    4096
#define NF_PORTAL_DEST_IP_BAW_HLIST_SIZE 1024 /*目的IP 黑白名单链表大小*/
#define NF_PORTAL_DEST_NET_BAW_HLIST_SIZE 1024 /*目的网段 黑白名单链表大小*/
#define NF_PORTAL_PORT_HLIST_SIZE 256 /*免认证端口表项大小*/
#define NF_PORTAL_MAC_BAW_HLIST_SIZE 2048 /*MAC黑白名单链表大小*/
#define NF_PORTAL_USER_IP_BAW_HLIST_SIZE 1024 /*用户IP 黑白名单链表表项大小*/
#define NF_PORTAL_PORT_NUM_HLIST_SIZE  1024/*透传端口号表项大小*/
#define NF_PORTAL_URL_IP_BAW_HLIST_SIZE 1024 /*URL对应的IP 黑白名单链表大小*/
/*begin: add@2015.01.30 for wechat connect wifi*/
#define NF_PORTAL_WXL_SERVERIP_HLIST_SIZE 512
/*end: add@2015.01.30 for wechat connect wifi*/

/*----------------------------------------------*
 * 宏操作                                       *
 *----------------------------------------------*/
 
#define NF_PORTAL_HASH_RD_LOCK            read_lock_bh(&g_stHashLock)
#define NF_PORTAL_HASH_RD_UNLOCK        read_unlock_bh(&g_stHashLock)
#define NF_PORTAL_HASH_WR_LOCK           write_lock_bh(&g_stHashLock)
#define NF_PORTAL_HASH_WR_UNLOCK       write_unlock_bh(&g_stHashLock)

#define NF_PORTAL_DEST_IP_BAW_HASH_RD_LOCK            read_lock_bh(&g_stDestIpBAWHashLock)
#define NF_PORTAL_DEST_IP_BAW_HASH_RD_UNLOCK        read_unlock_bh(&g_stDestIpBAWHashLock)
#define NF_PORTAL_DEST_IP_BAW_HASH_WR_LOCK           write_lock_bh(&g_stDestIpBAWHashLock)
#define NF_PORTAL_DEST_IP_BAW_HASH_WR_UNLOCK       write_unlock_bh(&g_stDestIpBAWHashLock)
#define NF_PORTAL_DEST_NET_BAW_HASH_RD_LOCK            read_lock_bh(&g_stDestNetBAWHashLock)
#define NF_PORTAL_DEST_NET_BAW_HASH_RD_UNLOCK        read_unlock_bh(&g_stDestNetBAWHashLock)
#define NF_PORTAL_DEST_NET_BAW_HASH_WR_LOCK           write_lock_bh(&g_stDestNetBAWHashLock)
#define NF_PORTAL_DEST_NET_BAW_HASH_WR_UNLOCK       write_unlock_bh(&g_stDestNetBAWHashLock)
#define NF_PORTAL_PORT_HASH_RD_LOCK            read_lock_bh(&g_stPortHashLock)
#define NF_PORTAL_PORT_HASH_RD_UNLOCK        read_unlock_bh(&g_stPortHashLock)
#define NF_PORTAL_PORT_HASH_WR_LOCK           write_lock_bh(&g_stPortHashLock)
#define NF_PORTAL_PORT_HASH_WR_UNLOCK       write_unlock_bh(&g_stPortHashLock)

#define NF_PORTAL_MAC_BAW_HASH_RD_LOCK            read_lock_bh(&g_stMacBAWHashLock)
#define NF_PORTAL_MAC_BAW_HASH_RD_UNLOCK        read_unlock_bh(&g_stMacBAWHashLock)
#define NF_PORTAL_MAC_BAW_HASH_WR_LOCK           write_lock_bh(&g_stMacBAWHashLock)
#define NF_PORTAL_MAC_BAW_HASH_WR_UNLOCK       write_unlock_bh(&g_stMacBAWHashLock)

#define NF_PORTAL_USER_IP_BAW_HASH_RD_LOCK            read_lock_bh(&g_stUserIpBAWHashLock)
#define NF_PORTAL_USER_IP_BAW_HASH_RD_UNLOCK        read_unlock_bh(&g_stUserIpBAWHashLock)
#define NF_PORTAL_USER_IP_BAW_HASH_WR_LOCK           write_lock_bh(&g_stUserIpBAWHashLock)
#define NF_PORTAL_USER_IP_BAW_HASH_WR_UNLOCK       write_unlock_bh(&g_stUserIpBAWHashLock)

#define NF_PORTAL_URL_BAW_RD_LOCK            read_lock_bh(&g_stUrlBAWLock)
#define NF_PORTAL_URL_BAW_RD_UNLOCK        read_unlock_bh(&g_stUrlBAWLock)
#define NF_PORTAL_URL_BAW_WR_LOCK           write_lock_bh(&g_stUrlBAWLock)
#define NF_PORTAL_URL_BAW_WR_UNLOCK       write_unlock_bh(&g_stUrlBAWLock)

#define NF_PORTAL_PORT_NUM_HASH_RD_LOCK            read_lock_bh(&g_stPortNumHashLock)
#define NF_PORTAL_PORT_NUM_HASH_RD_UNLOCK        read_unlock_bh(&g_stPortNumHashLock)
#define NF_PORTAL_PORT_NUM_HASH_WR_LOCK           write_lock_bh(&g_stPortNumHashLock)
#define NF_PORTAL_PORT_NUM_HASH_WR_UNLOCK       write_unlock_bh(&g_stPortNumHashLock)

#define NF_PORTAL_URL_IP_BAW_HASH_RD_LOCK            read_lock_bh(&g_stUrlIpBAWLock)
#define NF_PORTAL_URL_IP_BAW_HASH_RD_UNLOCK        read_unlock_bh(&g_stUrlIpBAWLock)
#define NF_PORTAL_URL_IP_BAW_HASH_WR_LOCK           write_lock_bh(&g_stUrlIpBAWLock)
#define NF_PORTAL_URL_IP_BAW_HASH_WR_UNLOCK       write_unlock_bh(&g_stUrlIpBAWLock)
/*begin: add@2015.01.31 for wechat connect wifi*/
#define NF_PORTAL_WXL_SERVERIP_HASH_RD_LOCK          read_lock_bh(&g_stWxlServerIpHashLock)
#define NF_PORTAL_WXL_SERVERIP_HASH_RD_UNLOCK          read_unlock_bh(&g_stWxlServerIpHashLock)
#define NF_PORTAL_WXL_SERVERIP_HASH_WR_LOCK          write_lock_bh(&g_stWxlServerIpHashLock)
#define NF_PORTAL_WXL_SERVERIP_HASH_WR_UNLOCK          write_unlock_bh(&g_stWxlServerIpHashLock)
/*end: add@2015.01.31 for wechat connect wifi*/

/*----------------------------------------------*
 * 枚举定义                                       *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 扩展类型定义                                     *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 结构定义                                       *
 *----------------------------------------------*/
#define NET_MASK_NUM_MAX    32
typedef enum{
    LIST_DISABLE=0,
    LIST_ENABLE=1
}LIST_ENABLE_E;
typedef struct{
    LIST_ENABLE_E eListEnable;
    UINT32  uiCount;
    struct hlist_head stDestNetBAWHashTable[NF_PORTAL_DEST_IP_BAW_HLIST_SIZE];
}NET_LIST_MAP_T;

/*----------------------------------------------*
 * 导出函数原型说明                             *
 *----------------------------------------------*/
AP_ERROR_CODE_E nf_portal_auth_state_update(NF_PORTAL_STA_ENTRY_T *pstEntry,NF_PORTAL_AUTH_STATE_E enState);
AP_ERROR_CODE_E nf_portal_hash_init(VOID);
NF_PORTAL_STA_ENTRY_T *nf_portal_get_sta_from_htable(UINT8 *pucMac);
NF_PORTAL_STA_ENTRY_T *nf_portal_add_sta_to_htable(UINT8 *pucMac,UINT32 uiIp);
AP_ERROR_CODE_E nf_portal_del_sta_from_htable(NF_PORTAL_STA_ENTRY_T *pstEntry);
AP_ERROR_CODE_E nf_portal_update_sta_from_htable(NF_PORTAL_STA_ENTRY_T *pstEntry,UINT32 uiIp);
NF_PORTAL_AUTH_STATE_E nf_portal_get_auth_state(NF_PORTAL_STA_ENTRY_T *pstEntry);
VOID nf_portal_del_ageing_entry(ULONG ulData);
AP_ERROR_CODE_E nf_portal_ts_update(NF_PORTAL_STA_ENTRY_T *pstEntry,NF_PORTAL_DIR_E enDir,UINT32 uiSize);
AP_ERROR_CODE_E nf_portal_access_time_update(NF_PORTAL_STA_ENTRY_T *pstEntry);
AP_ERROR_CODE_E nf_portal_get_sta_list(NF_PORTAL_IOCTL_STA_LIST_T *pstStaList);
VOID nf_portal_flush_sta_htbale(VOID);
NF_PORTAL_IP_BAW_ENTRY_T *nf_portal_dest_ip_baw_get(UINT32 uiIp,UINT16 usPortNum);
AP_ERROR_CODE_E nf_portal_dest_ip_baw_add(UINT32 uiIp,UINT16 usPortNum,NF_PORTAL_BAW_FLAG_E enFlag);
AP_ERROR_CODE_E nf_portal_dest_ip_baw_del(NF_PORTAL_IP_BAW_ENTRY_T *pstEntry);
AP_ERROR_CODE_E nf_portal_dest_ip_baw_get_list(NF_PORTAL_IOCTL_IP_BAW_LIST_T *pstIpBAWList);
AP_ERROR_CODE_E nf_portal_dest_ip_baw_flush(NF_PORTAL_BAW_FLAG_E enFlag);
NF_PORTAL_NET_BAW_ENTRY_T *nf_portal_dest_net_baw_get(NET_LIST_MAP_T *pstListMap,UINT32 uiNet,UINT32 uiMaskNum);
NF_PORTAL_NET_BAW_ENTRY_T *nf_portal_check_dest_net_baw(UINT32 uiNet);
NF_PORTAL_NET_BAW_ENTRY_T *nf_portal_dest_net_baw_get_frommap(UINT32 uiNet,UINT32 uiMaskNum);
AP_ERROR_CODE_E nf_portal_dest_net_baw_add(NET_LIST_MAP_T *pstListMap,UINT32 uiNet,UINT32 uiMaskNum,NF_PORTAL_BAW_FLAG_E enFlag);
AP_ERROR_CODE_E nf_portal_dest_net_baw_add_tomap(UINT32 uiNet,UINT32 uiMaskNum,NF_PORTAL_BAW_FLAG_E enFlag);
AP_ERROR_CODE_E nf_portal_dest_net_baw_del(NET_LIST_MAP_T *pstNetBawMap,NF_PORTAL_NET_BAW_ENTRY_T *pstEntry);
AP_ERROR_CODE_E nf_portal_dest_net_baw_del_frommap(UINT32 uiNet,UINT32 uiMaskNum,NF_PORTAL_BAW_FLAG_E enFlag);
AP_ERROR_CODE_E nf_portal_dest_net_baw_get_list(NF_PORTAL_IOCTL_NET_BAW_LIST_T *pstNetBAWList);
AP_ERROR_CODE_E nf_portal_dest_net_baw_flush(NF_PORTAL_BAW_FLAG_E enFlag);

NF_PORTAL_PORT_ENTRY_T *nf_portal_free_auth_port_get(UINT8 *pucPortName);
AP_ERROR_CODE_E nf_portal_free_auth_port_add(UINT8 *pucPortName);
AP_ERROR_CODE_E nf_portal_free_auth_port_del(NF_PORTAL_PORT_ENTRY_T *pstEntry);
AP_ERROR_CODE_E nf_portal_free_auth_port_get_list(NF_PORTAL_IOCTL_PORT_LIST_T *pstPortList);
AP_ERROR_CODE_E nf_portal_free_auth_port_flush(VOID);
NF_PORTAL_PORT_ENTRY_T *nf_portal_free_auth_port_get(UINT8 *pucPortName);
NF_PORTAL_IP_BAW_ENTRY_T *nf_portal_baw_get(UINT32 uiIp);
AP_ERROR_CODE_E nf_portal_mac_baw_flush(NF_PORTAL_BAW_FLAG_E enFlag);
UINT32 nf_portal_get_mac_white_list_count(VOID);
AP_ERROR_CODE_E nf_portal_mac_baw_add(UINT8 *pucMac,NF_PORTAL_BAW_FLAG_E enFlag);
NF_PORTAL_MAC_BAW_ENTRY_T *nf_portal_mac_baw_get(UINT8 *pucMac);
AP_ERROR_CODE_E nf_portal_mac_baw_del(NF_PORTAL_MAC_BAW_ENTRY_T *pstEntry);
AP_ERROR_CODE_E nf_portal_mac_baw_get_list(NF_PORTAL_IOCTL_MAC_BAW_LIST_T *pstMacBAWList);
NF_PORTAL_IP_BAW_ENTRY_T *nf_portal_user_ip_baw_get(UINT32 uiIp);
AP_ERROR_CODE_E nf_portal_user_ip_baw_add(UINT32 uiIp,NF_PORTAL_BAW_FLAG_E enFlag);
AP_ERROR_CODE_E nf_portal_user_ip_baw_del(NF_PORTAL_IP_BAW_ENTRY_T *pstEntry);
AP_ERROR_CODE_E nf_portal_user_ip_baw_get_list(NF_PORTAL_IOCTL_IP_BAW_LIST_T *pstIpBAWList);
AP_ERROR_CODE_E nf_portal_user_ip_baw_flush(NF_PORTAL_BAW_FLAG_E enFlag);
NF_PORTAL_URL_BAW_ENTRY_T *nf_portal_url_baw_get(UINT8 *pucUrl);
AP_ERROR_CODE_E nf_portal_url_baw_add(UINT8 *pucUrl,NF_PORTAL_BAW_FLAG_E enFlag);
AP_ERROR_CODE_E nf_portal_url_baw_del(NF_PORTAL_URL_BAW_ENTRY_T *pstEntry);
AP_ERROR_CODE_E nf_portal_url_baw_get_list(NF_PORTAL_IOCTL_URL_BAW_LIST_T *pstUrlBAWList);
AP_ERROR_CODE_E nf_portal_url_baw_flush(NF_PORTAL_BAW_FLAG_E enFlag);
AP_ERROR_CODE_E nf_portal_hash_destroy(VOID);
NF_PORTAL_PORT_NUM_ENTRY_T *nf_portal_port_num_get(UINT16 usPortNum,UINT8 ucIpProto);
AP_ERROR_CODE_E nf_portal_port_num_add(UINT16 usPortNum,UINT8 ucIpProto);
AP_ERROR_CODE_E nf_portal_port_num_del(NF_PORTAL_PORT_NUM_ENTRY_T *pstEntry);
AP_ERROR_CODE_E nf_portal_port_num_get_list(NF_PORTAL_IOCTL_PORT_NUM_LIST_T *pstPortNumList);
AP_ERROR_CODE_E nf_portal_port_num_flush(UINT8 ucIpProto);
AP_ERROR_CODE_E nf_portal_url_ip_baw_get_list(NF_PORTAL_IOCTL_URL_IP_BAW_LIST_T *pstUrlIpBAWList);
NF_PORTAL_URL_BAW_ENTRY_T *nf_portal_url_baw_get_for_user(UINT8 *pucUrl);
AP_ERROR_CODE_E nf_portal_url_ip_baw_add(UINT32 uiIp,NF_PORTAL_URL_BAW_ENTRY_T *pstUrlBaw,UINT8 *pucUrl);
UINT32 nf_portal_get_url_white_list_count(VOID);
NF_PORTAL_URL_IP_BAW_ENTRY_T *nf_portal_url_ip_baw_get(UINT32 uiIp);
/*begin: add@2015.01.31 for wechat connect wifi*/
NF_PORTAL_WXL_SERVERIP_ENTRY_T *nf_portal_wxl_serverip_get(UINT32 uiIp);
AP_ERROR_CODE_E nf_portal_wxl_serverip_add(UINT32 uiIp, UINT32 uiMask);
AP_ERROR_CODE_E nf_portal_wxl_serverip_del(NF_PORTAL_WXL_SERVERIP_ENTRY_T *pstEntry);
AP_ERROR_CODE_E nf_portal_wxl_serverip_get_list(NF_PORTAL_IOCTL_WXL_SERVERIP_LIST_T *pstWxlServerIpList);
AP_ERROR_CODE_E nf_portal_wxl_serverip_flush(void);
NF_PORTAL_STA_TYPE_E nf_portal_get_sta_type(NF_PORTAL_STA_ENTRY_T *pstEntry);
AP_ERROR_CODE_E nf_portal_sta_type_update(NF_PORTAL_STA_ENTRY_T *pstEntry,NF_PORTAL_STA_TYPE_E enStaType);

UINT32 nf_portal_wxl_serverip_judge(UINT32 uiIp);
/*end: add@2015.01.31 for wechat connect wifi*/

#ifdef HAVE_MULTI_WAN
AP_ERROR_CODE_E nf_portal_wan_conf_set(UINT32 uiWanIndex, UINT32 uiEnable, UINT32 uiBand);
AP_ERROR_CODE_E nf_portal_wan_conf_get(NF_PORTAL_IOCTL_WAN_LIST_T *pstIoWanList);
AP_ERROR_CODE_E nf_portal_wan_map_get(NF_PORTAL_IOCTL_WAN_MAP_T *pstIoWanMap);
#endif

#endif /* __NF_PORTAL_HASH_H__ */

