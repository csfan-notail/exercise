/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : nf_portal_hash.c
  版 本 号   : 初稿
  作    者   : WeiHonggang
  生成日期   : 2013年8月3日 星期六
  最近修改   :
  功能描述   : 相关的hash操作
  函数列表   :
  修改历史   :
  1.日    期   : 2013年8月3日 星期六
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

LOCAL struct hlist_head g_astStaHashTable[NF_PORTAL_STA_HASH_SIZE]; /*STA hash表*/
LOCAL DEFINE_RWLOCK(g_stHashLock);

LOCAL struct hlist_head g_astDestIpBAWHashTable[NF_PORTAL_DEST_IP_BAW_HLIST_SIZE]; /*Dest Ip Black And White List hash表*/
LOCAL DEFINE_RWLOCK(g_stDestIpBAWHashLock);

LOCAL NET_LIST_MAP_T g_astDestNetBAWListMap[NET_MASK_NUM_MAX]; /*Dest Net Black And White List hash表*/
LOCAL DEFINE_RWLOCK(g_stDestNetBAWHashLock);

LOCAL struct hlist_head g_astPortHashTable[NF_PORTAL_PORT_HLIST_SIZE]; /*Port Free Auth List hash表*/
LOCAL DEFINE_RWLOCK(g_stPortHashLock);

LOCAL struct hlist_head g_astMacBAWHashTable[NF_PORTAL_MAC_BAW_HLIST_SIZE]; /*MAC Black And White List hash表*/
LOCAL DEFINE_RWLOCK(g_stMacBAWHashLock);
LOCAL UINT32 g_uiMacWhiteListCount = 0;

LOCAL struct hlist_head g_astUserIpBAWHashTable[NF_PORTAL_USER_IP_BAW_HLIST_SIZE]; /*User Ip Free Auth List hash表*/
LOCAL DEFINE_RWLOCK(g_stUserIpBAWHashLock);

LOCAL struct list_head g_stUrlBAWTable = LIST_HEAD_INIT(g_stUrlBAWTable); /*Url Black And White List,双向链表*/
LOCAL DEFINE_RWLOCK(g_stUrlBAWLock);
LOCAL struct hlist_head g_astUrlIpBAWHashTable[NF_PORTAL_URL_IP_BAW_HLIST_SIZE]; /*Url-IP Black And White List hash表*/
LOCAL UINT32 g_uiUrlWhiteListCount = 0;
LOCAL DEFINE_RWLOCK(g_stUrlIpBAWLock);

LOCAL struct hlist_head g_astPortNumHashTable[NF_PORTAL_PORT_NUM_HLIST_SIZE]; /*Port Num  hash表*/
LOCAL DEFINE_RWLOCK(g_stPortNumHashLock);

/*begin add@2015.01.31 for wechat connect wifi*/
LOCAL struct hlist_head g_astWxlServerHashTable[NF_PORTAL_WXL_SERVERIP_HLIST_SIZE];
LOCAL DEFINE_RWLOCK(g_stWxlServerIpHashLock);
/*begin add@2015.01.31 for wechat connect wifi*/
/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 函数定义                                       *
 *----------------------------------------------*/
UINT32 nf_bit_to_netmask(UINT32 uiBitNum)
{
    UINT32 uiNetMask=0;
    UINT32 ulLoop=0;
    for(ulLoop = 0; ulLoop < uiBitNum; ulLoop++)
    {
        uiNetMask |= 1<<(NET_MASK_NUM_MAX-ulLoop-1);
    }
    return uiNetMask;
}


LOCAL inline UINT32 nf_portal_mac_hash(const UINT8 *pucMac)
{
	UINT32 x;

	x = pucMac[0];
	x = (x << 2) ^ pucMac[1];
	x = (x << 2) ^ pucMac[2];
	x = (x << 2) ^ pucMac[3];
	x = (x << 2) ^ pucMac[4];
	x = (x << 2) ^ pucMac[5];

	x ^= x >> 8;

	return x;
}

LOCAL inline UINT32 nf_portal_mac_ip_hash(const UINT8 *pucMac,const UINT32 uiIp)
{
    /*暂时只用IP*/
    return uiIp;
}

LOCAL struct hlist_head *nf_portal_sta_hash_fn(UINT8 *pucMac)
{
    UINT32 uiIndex;

    if (unlikely(pucMac == NULL))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"mac is null,return\n");
        return NULL;
    }

    uiIndex = nf_portal_mac_hash(pucMac);

    //NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"INDEX:%u\n",(uiIndex&(NF_PORTAL_STA_HASH_SIZE - 1)));
    return &(g_astStaHashTable[uiIndex& (NF_PORTAL_STA_HASH_SIZE - 1)]);
}

NF_PORTAL_STA_ENTRY_T *nf_portal_get_sta_from_htable(UINT8 *pucMac)
{
    NF_PORTAL_STA_ENTRY_T *pstEntry = NULL;
    struct hlist_node *pos = NULL;
    struct hlist_node *n = NULL;
    
    if (unlikely(NULL == pucMac))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"mac is null,return\n");
        return NULL;
    }
    
    NF_PORTAL_HASH_RD_LOCK;
    hlist_for_each_safe(pos, n,nf_portal_sta_hash_fn(pucMac))
    {
        pstEntry = hlist_entry(pos, NF_PORTAL_STA_ENTRY_T, stStaHList);

        if (0 == memcmp(pstEntry->stSta.aucMac,pucMac,ETH_ALEN))
        {
            NF_PORTAL_HASH_RD_UNLOCK;
            return pstEntry; /*Find it. Return at once.*/
        }
    }
	NF_PORTAL_HASH_RD_UNLOCK;
	
	return NULL;
}

UINT32 nf_portal_get_cur_time(VOID)
{
    struct timeval tv;
    do_gettimeofday(&tv);

    return (UINT32)(tv.tv_sec);
}

NF_PORTAL_STA_ENTRY_T *nf_portal_add_sta_to_htable(UINT8 *pucMac,UINT32 uiIp)
{
    struct hlist_head *pstHead = NULL;
    NF_PORTAL_STA_ENTRY_T *pstEntry = NULL;
    
    if (NULL == pucMac)
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"mac is null,return\n");
        return NULL;
    }

    pstEntry = nf_portal_get_sta_from_htable(pucMac);

    if (unlikely(NULL != pstEntry) )
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"default fail,return\n");
        return pstEntry;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"create a new entry\n");
    pstEntry = (NF_PORTAL_STA_ENTRY_T*)(kmalloc(sizeof(NF_PORTAL_STA_ENTRY_T), GFP_ATOMIC));

    if (unlikely(NULL == pstEntry))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"malloc fail,return\n");
        return NULL;
    }
    memset(pstEntry,0,sizeof(NF_PORTAL_STA_ENTRY_T));
    memcpy(pstEntry->stSta.aucMac,pucMac,ETH_ALEN);
    pstEntry->stSta.uiIp = uiIp;
    pstEntry->stSta.enAuthState = STATE_NOT_AUTH;
    pstEntry->stSta.enStaType = TYPE_INIT;
	pstEntry->stSta.uiSec = nf_portal_get_cur_time();
    NF_PORTAL_HASH_WR_LOCK;
    pstHead = nf_portal_sta_hash_fn(pucMac);
    hlist_add_head(&(pstEntry->stStaHList), pstHead);
    NF_PORTAL_HASH_WR_UNLOCK;
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"MAC ="MAC_FMT" IP="IP_FMT"\n",MAC_ARG(pucMac),IP_ARG(&uiIp));
    return pstEntry;
}


AP_ERROR_CODE_E nf_portal_del_sta_from_htable(NF_PORTAL_STA_ENTRY_T *pstEntry)
{  
    if (NULL == pstEntry)
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR, "entry is null,return.\n");
        return AP_E_PARAM;
    }
    
    NF_PORTAL_HASH_WR_LOCK;
    hlist_del_init(&(pstEntry->stStaHList));
    NF_PORTAL_HASH_WR_UNLOCK;

    kfree(pstEntry);
    
    return AP_E_NONE;
}

AP_ERROR_CODE_E nf_portal_update_sta_from_htable(NF_PORTAL_STA_ENTRY_T *pstEntry,UINT32 uiIp)
{  
    if (NULL == pstEntry)
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR, "entry is null,return.\n");
        return AP_E_PARAM;
    }
    
    NF_PORTAL_HASH_WR_LOCK;
    pstEntry->stSta.uiIp = uiIp;
    NF_PORTAL_HASH_WR_UNLOCK;
    
    return AP_E_NONE;
}

AP_ERROR_CODE_E nf_portal_get_sta_list(NF_PORTAL_IOCTL_STA_LIST_T *pstStaList)
{
    NF_PORTAL_STA_ENTRY_T *pstEntry = NULL;
    NF_PORTAL_STA_ATTRIB_T *pstSta = NULL;
    struct hlist_node *pstTmp;
    struct hlist_node *pstTmp2;
    struct hlist_head *pstHead;
    UINT32 uiIndex;
    UINT32 uiEntryNum = 0;
    UINT32 uiSkipNum = 0;
    UINT32 uiStaNum = 0;
    NF_PORTAL_AUTH_STATE_E enState;
    if (NULL == pstStaList)
    {
        return AP_E_PARAM;
    }

    enState = pstStaList->enState;
    
    NF_PORTAL_HASH_RD_LOCK;
    for (uiIndex = 0;uiIndex < NF_PORTAL_STA_HASH_SIZE; uiIndex++)
    {
        pstHead = &g_astStaHashTable[uiIndex];
        hlist_for_each_entry_safe(pstEntry,pstTmp,pstTmp2,pstHead,stStaHList)
        {
            /*跳过之前访问过的队列*/
            if (uiSkipNum < pstStaList->uiStartNum)
            {
                uiSkipNum++;
                uiEntryNum++;
                continue;
            }
            
            /*获取到了足够的表项*/
            if (uiStaNum >= NF_PORTAL_STA_LIST_MAX)
            {
                break;
            }

            if (enState == STATE_MAX)
            {
                /*获取Entity*/
                pstSta = &(pstStaList->astSta[uiStaNum]);
                memcpy(pstSta->aucMac,pstEntry->stSta.aucMac,ETH_ALEN);
                pstSta->uiIp = pstEntry->stSta.uiIp;
                pstSta->uiSec = pstEntry->stSta.uiSec;
                pstSta->enAuthState = pstEntry->stSta.enAuthState;
                pstSta->enStaType = pstEntry->stSta.enStaType;
                pstSta->astTS[NF_PORTAL_DIR_UP].ui4GByte = pstEntry->stSta.astTS[NF_PORTAL_DIR_UP].ui4GByte;
                pstSta->astTS[NF_PORTAL_DIR_UP].uiByte = pstEntry->stSta.astTS[NF_PORTAL_DIR_UP].uiByte;
                pstSta->astTS[NF_PORTAL_DIR_DOWN].ui4GByte = pstEntry->stSta.astTS[NF_PORTAL_DIR_DOWN].ui4GByte;
                pstSta->astTS[NF_PORTAL_DIR_DOWN].uiByte = pstEntry->stSta.astTS[NF_PORTAL_DIR_DOWN].uiByte;
                uiStaNum++;
                uiEntryNum++;
            }
            else
            {
                if (enState == pstEntry->stSta.enAuthState)
                {
                    /*获取Entity*/
                    pstSta = &(pstStaList->astSta[uiStaNum]);
                    memcpy(pstSta->aucMac,pstEntry->stSta.aucMac,ETH_ALEN);
                    pstSta->uiIp = pstEntry->stSta.uiIp;
                    pstSta->uiSec = pstEntry->stSta.uiSec;
                    pstSta->enAuthState = pstEntry->stSta.enAuthState;
                    pstSta->enStaType = pstEntry->stSta.enStaType;
                    pstSta->astTS[NF_PORTAL_DIR_UP].ui4GByte = pstEntry->stSta.astTS[NF_PORTAL_DIR_UP].ui4GByte;
                    pstSta->astTS[NF_PORTAL_DIR_UP].uiByte = pstEntry->stSta.astTS[NF_PORTAL_DIR_UP].uiByte;
                    pstSta->astTS[NF_PORTAL_DIR_DOWN].ui4GByte = pstEntry->stSta.astTS[NF_PORTAL_DIR_DOWN].ui4GByte;
                    pstSta->astTS[NF_PORTAL_DIR_DOWN].uiByte = pstEntry->stSta.astTS[NF_PORTAL_DIR_DOWN].uiByte;
                    uiStaNum++;
                    uiEntryNum++;
                }
                else
                {
                    uiEntryNum++;
                }
            }
        }
    }
    NF_PORTAL_HASH_RD_UNLOCK;
    pstStaList->uiNum = uiStaNum;
    pstStaList->uiStartNum = uiEntryNum;
    return AP_E_NONE;
    
}
NF_PORTAL_STA_TYPE_E nf_portal_get_sta_type(NF_PORTAL_STA_ENTRY_T *pstEntry)
{
    NF_PORTAL_STA_TYPE_E enStaType = TYPE_MAX;
    if (unlikely(NULL == pstEntry))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR, "entry is null,return.\n");
        return TYPE_MAX;
    }

    NF_PORTAL_HASH_RD_LOCK;
    enStaType = pstEntry->stSta.enStaType;
    NF_PORTAL_HASH_RD_UNLOCK;
    return enStaType;
}

AP_ERROR_CODE_E nf_portal_sta_type_update(NF_PORTAL_STA_ENTRY_T *pstEntry,NF_PORTAL_STA_TYPE_E enStaType)
{
   if (unlikely(NULL == pstEntry))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR, "entry is null,return.\n");
        return AP_E_PARAM;
    }

    NF_PORTAL_HASH_RD_LOCK;
    pstEntry->stSta.enStaType = enStaType;
    pstEntry->stSta.uiSec = nf_portal_get_cur_time();
    NF_PORTAL_HASH_RD_UNLOCK;

    return AP_E_NONE;
}

NF_PORTAL_AUTH_STATE_E nf_portal_get_auth_state(NF_PORTAL_STA_ENTRY_T *pstEntry)
{
    NF_PORTAL_AUTH_STATE_E enState = STATE_MAX;
    if (unlikely(NULL == pstEntry))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR, "entry is null,return.\n");
        return STATE_MAX;
    }

    NF_PORTAL_HASH_RD_LOCK;
    enState = pstEntry->stSta.enAuthState;
    NF_PORTAL_HASH_RD_UNLOCK;
    return enState;
}

AP_ERROR_CODE_E nf_portal_auth_state_update(NF_PORTAL_STA_ENTRY_T *pstEntry,NF_PORTAL_AUTH_STATE_E enState)
{
   if (unlikely(NULL == pstEntry))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR, "entry is null,return.\n");
        return AP_E_PARAM;
    }

    NF_PORTAL_HASH_RD_LOCK;
    pstEntry->stSta.enAuthState = enState;
    pstEntry->stSta.uiSec = nf_portal_get_cur_time();
    NF_PORTAL_HASH_RD_UNLOCK;

    return AP_E_NONE;
}

VOID nf_portal_del_ageing_entry(ULONG ulData)
{	 
    UINT32 uiIndex = 0;
    NF_PORTAL_STA_ENTRY_T *pstEntry = NULL;
    struct hlist_node *pstTmp;
    struct hlist_node *pstTmp2;
    struct hlist_head *pstHead;

    NF_PORTAL_HASH_RD_LOCK;
    for (uiIndex = 0; uiIndex < NF_PORTAL_STA_HASH_SIZE; uiIndex++)
    {
        pstHead = &g_astStaHashTable[uiIndex];
        hlist_for_each_entry_safe(pstEntry,pstTmp,pstTmp2,pstHead,stStaHList)
        {
            if (pstEntry->stSta.enAuthState == STATE_AUTH)
            {
                continue;
            }
            else
            {
                if (time_after(jiffies, (pstEntry->ulAccessTime+ulData)))
                {
                    NF_PORTAL_HASH_RD_UNLOCK;
                    nf_portal_del_sta_from_htable(pstEntry);
                    NF_PORTAL_HASH_RD_LOCK;
                }
            }
        }
    }
    NF_PORTAL_HASH_RD_UNLOCK;
    return;
    }
VOID nf_portal_flush_sta_htbale(VOID)
{	 
    UINT32 uiIndex = 0;
    NF_PORTAL_STA_ENTRY_T *pstEntry = NULL;
    struct hlist_node *pstTmp;
    struct hlist_node *pstTmp2;
    struct hlist_head *pstHead;

    NF_PORTAL_HASH_RD_LOCK;
    for (uiIndex = 0; uiIndex < NF_PORTAL_STA_HASH_SIZE; uiIndex++)
    {
        pstHead = &g_astStaHashTable[uiIndex];
        hlist_for_each_entry_safe(pstEntry,pstTmp,pstTmp2,pstHead,stStaHList)
        {
            NF_PORTAL_HASH_RD_UNLOCK;
            nf_portal_del_sta_from_htable(pstEntry);
            NF_PORTAL_HASH_RD_LOCK;
        }
    }
    NF_PORTAL_HASH_RD_UNLOCK;
    return;
    }
AP_ERROR_CODE_E nf_portal_ts_update(NF_PORTAL_STA_ENTRY_T *pstEntry,NF_PORTAL_DIR_E enDir,UINT32 uiSize)
{
   if (unlikely(NULL == pstEntry))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR, "entry is null,return.\n");
        return AP_E_PARAM;
    }

   NF_PORTAL_HASH_WR_LOCK;
   if (pstEntry->stSta.astTS[enDir].uiByte + uiSize > pstEntry->stSta.astTS[enDir].uiByte)
   {
        pstEntry->stSta.astTS[enDir].uiByte += uiSize;
   }
   else
   {
        pstEntry->stSta.astTS[enDir].uiByte += uiSize;
        pstEntry->stSta.astTS[enDir].ui4GByte ++;
   }
   NF_PORTAL_HASH_WR_UNLOCK;

   return AP_E_NONE;
}

AP_ERROR_CODE_E nf_portal_access_time_update(NF_PORTAL_STA_ENTRY_T *pstEntry)
{
   if (unlikely(NULL == pstEntry))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR, "entry is null,return.\n");
        return AP_E_PARAM;
    }

   NF_PORTAL_HASH_WR_LOCK;
   pstEntry->ulAccessTime = jiffies;
   NF_PORTAL_HASH_WR_UNLOCK;

   return AP_E_NONE;
}

/*目的IP 黑白名单*/
LOCAL inline UINT32 nf_portal_ip_hash(const UINT32 uiIp)
{
    return uiIp;
}

LOCAL struct hlist_head *nf_portal_dest_ip_hash_fn(UINT32 uiIp)
{
    UINT32 uiIndex;

    uiIndex = nf_portal_ip_hash(uiIp);
    
    return &(g_astDestIpBAWHashTable[uiIndex& (NF_PORTAL_DEST_IP_BAW_HLIST_SIZE - 1)]);
}

NF_PORTAL_IP_BAW_ENTRY_T *nf_portal_dest_ip_baw_get(UINT32 uiIp,UINT16 usPortNum)
{
    NF_PORTAL_IP_BAW_ENTRY_T *pstEntry = NULL;
    struct hlist_node *pos = NULL;
    struct hlist_node *n = NULL;
    
    NF_PORTAL_DEST_IP_BAW_HASH_RD_LOCK;
    hlist_for_each_safe(pos, n,nf_portal_dest_ip_hash_fn(uiIp))
    {
        pstEntry = hlist_entry(pos, NF_PORTAL_IP_BAW_ENTRY_T, stIpBAWHList);

        if ((uiIp == pstEntry->stIpBAW.uiIp) && (usPortNum == pstEntry->stIpBAW.usPort))
        {
            NF_PORTAL_DEST_IP_BAW_HASH_RD_UNLOCK;
            return pstEntry; /*Find it. Return at once.*/
        }
    }
	NF_PORTAL_DEST_IP_BAW_HASH_RD_UNLOCK;
	return NULL;
}

AP_ERROR_CODE_E nf_portal_dest_ip_baw_add(UINT32 uiIp,UINT16 usPortNum,NF_PORTAL_BAW_FLAG_E enFlag)
{
    struct hlist_head *pstHead = NULL;
    NF_PORTAL_IP_BAW_ENTRY_T *pstEntry = NULL;

    pstEntry = nf_portal_dest_ip_baw_get(uiIp,usPortNum);

    if (NULL != pstEntry)
    {
        if (pstEntry->stIpBAW.enFlag == enFlag)
        {
            return AP_E_NONE;
        }
        else
        {
            NF_PORTAL_DEST_IP_BAW_HASH_WR_LOCK;
            pstEntry->stIpBAW.enFlag = enFlag;
            NF_PORTAL_DEST_IP_BAW_HASH_WR_UNLOCK;
            return AP_E_NONE;
        }
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"create a new entry\n");
    pstEntry = (NF_PORTAL_IP_BAW_ENTRY_T*)(kmalloc(sizeof(NF_PORTAL_IP_BAW_ENTRY_T), GFP_ATOMIC));
    if (unlikely(NULL == pstEntry))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"malloc fail,return\n");
        return AP_E_MEM_MALLOC;
    }

    memset(pstEntry,0,sizeof(NF_PORTAL_IP_BAW_ENTRY_T));
    pstEntry->stIpBAW.uiIp = uiIp;
    pstEntry->stIpBAW.usPort = usPortNum;
    pstEntry->stIpBAW.enFlag = enFlag;
    
    NF_PORTAL_DEST_IP_BAW_HASH_WR_LOCK;
    pstHead = nf_portal_dest_ip_hash_fn(uiIp);
    hlist_add_head(&(pstEntry->stIpBAWHList), pstHead);
    NF_PORTAL_DEST_IP_BAW_HASH_WR_UNLOCK;
    
    return AP_E_NONE;
}


AP_ERROR_CODE_E nf_portal_dest_ip_baw_del(NF_PORTAL_IP_BAW_ENTRY_T *pstEntry)
{  
    if (NULL == pstEntry)
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR, "entry is null,return.\n");
        return AP_E_PARAM;
    }
    
    NF_PORTAL_DEST_IP_BAW_HASH_WR_LOCK;
    hlist_del_init(&(pstEntry->stIpBAWHList));
    NF_PORTAL_DEST_IP_BAW_HASH_WR_UNLOCK;

    kfree(pstEntry);
    
    return AP_E_NONE;
}

AP_ERROR_CODE_E nf_portal_dest_ip_baw_get_list(NF_PORTAL_IOCTL_IP_BAW_LIST_T *pstIpBAWList)
{
    NF_PORTAL_IP_BAW_ENTRY_T *pstEntry = NULL;
    NF_PORTAL_IP_BAW_ATTRIB_T *pstIpBaw = NULL;
    struct hlist_node *pstTmp;
    struct hlist_node *pstTmp2;
    struct hlist_head *pstHead;
    UINT32 uiIndex;
    UINT32 uiEntryNum = 0;
    UINT32 uiSkipNum = 0;
    UINT32 uiStaNum = 0;
    NF_PORTAL_BAW_FLAG_E enFlag;
    
    if (NULL == pstIpBAWList)
    {
        return AP_E_PARAM;
    }

    enFlag = pstIpBAWList->enFlag;
    
    NF_PORTAL_DEST_IP_BAW_HASH_RD_LOCK;
    for (uiIndex = 0;uiIndex < NF_PORTAL_DEST_IP_BAW_HLIST_SIZE; uiIndex++)
    {
        pstHead = &g_astDestIpBAWHashTable[uiIndex];
        hlist_for_each_entry_safe(pstEntry,pstTmp,pstTmp2,pstHead,stIpBAWHList)
        {
            /*跳过之前访问过的队列*/
            if (uiSkipNum < pstIpBAWList->uiStartNum)
            {
                uiSkipNum++;
                uiEntryNum++;
                continue;
            }
            
            /*获取到了足够的表项*/
            if (uiStaNum >= NF_PORTAL_IP_BAW_LIST_MAX)
            {
                break;
            }

            if (enFlag == pstEntry->stIpBAW.enFlag)
            {
                /*获取Entity*/
                pstIpBaw = &(pstIpBAWList->astIpBaw[uiStaNum]);
                pstIpBaw->uiIp = pstEntry->stIpBAW.uiIp;
                pstIpBaw->usPort = pstEntry->stIpBAW.usPort;
                pstIpBaw->enFlag = pstEntry->stIpBAW.enFlag;
  
                uiStaNum++;
                uiEntryNum++;
            }
            else
            {
                uiEntryNum++;
            }
        }
    }
    NF_PORTAL_DEST_IP_BAW_HASH_RD_UNLOCK;
    pstIpBAWList->uiNum = uiStaNum;
    pstIpBAWList->uiStartNum = uiEntryNum;
    return AP_E_NONE;
    
}

AP_ERROR_CODE_E nf_portal_dest_ip_baw_flush(NF_PORTAL_BAW_FLAG_E enFlag)
{
    NF_PORTAL_IP_BAW_ENTRY_T *pstEntry = NULL;
    struct hlist_node *pstTmp;
    struct hlist_node *pstTmp2;
    struct hlist_head *pstHead;
    UINT32 uiIndex;  
    
    NF_PORTAL_DEST_IP_BAW_HASH_RD_LOCK;
    for (uiIndex = 0;uiIndex < NF_PORTAL_DEST_IP_BAW_HLIST_SIZE; uiIndex++)
    {
        pstHead = &g_astDestIpBAWHashTable[uiIndex];
        hlist_for_each_entry_safe(pstEntry,pstTmp,pstTmp2,pstHead,stIpBAWHList)
        {
            if (enFlag == BAW)
            {
                NF_PORTAL_DEST_IP_BAW_HASH_RD_UNLOCK;
                nf_portal_dest_ip_baw_del(pstEntry);
                NF_PORTAL_DEST_IP_BAW_HASH_RD_LOCK;
            }
            else
            {
                if (enFlag == pstEntry->stIpBAW.enFlag)
                {
                    NF_PORTAL_DEST_IP_BAW_HASH_RD_UNLOCK;
                    nf_portal_dest_ip_baw_del(pstEntry);
                    NF_PORTAL_DEST_IP_BAW_HASH_RD_LOCK;
                }
            }
        }
    }
    NF_PORTAL_DEST_IP_BAW_HASH_RD_UNLOCK; 
    return AP_E_NONE;
}

/*目的网段 黑白名单*/
LOCAL inline UINT32 nf_portal_net_hash(const UINT32 uiNet,UINT32 uiMaskNum)
{
    UINT32 uiIndex=0;
    uiIndex=uiNet>>(NET_MASK_NUM_MAX - uiMaskNum);
    uiIndex%=NF_PORTAL_DEST_IP_BAW_HLIST_SIZE;
    return uiIndex;
}

LOCAL struct hlist_head *nf_portal_dest_net_hash_fn(NET_LIST_MAP_T *pstListMap,UINT32 uiNet,UINT32 uiMaskNum)
{
    UINT32 uiIndex;
    uiIndex = nf_portal_net_hash(uiNet,uiMaskNum);
    return &(pstListMap->stDestNetBAWHashTable[uiIndex& (NF_PORTAL_DEST_IP_BAW_HLIST_SIZE - 1)]);
}

NF_PORTAL_NET_BAW_ENTRY_T *nf_portal_dest_net_baw_get(NET_LIST_MAP_T *pstListMap,UINT32 uiNet,UINT32 uiMaskNum)
{
    NF_PORTAL_NET_BAW_ENTRY_T *pstEntry = NULL;
    struct hlist_node *pos = NULL;
    struct hlist_node *n = NULL;

    NF_PORTAL_DEST_NET_BAW_HASH_RD_LOCK;
    hlist_for_each_safe(pos, n,nf_portal_dest_net_hash_fn(pstListMap,uiNet,uiMaskNum))
    {
        pstEntry = hlist_entry(pos, NF_PORTAL_NET_BAW_ENTRY_T, stNetBAWHList);
        if (uiNet == pstEntry->stNetBAW.uiNet)
        {
            NF_PORTAL_DEST_NET_BAW_HASH_RD_UNLOCK;
            return pstEntry; /*Find it. Return at once.*/
        }
    }
	NF_PORTAL_DEST_NET_BAW_HASH_RD_UNLOCK;
	return NULL;
}

NF_PORTAL_NET_BAW_ENTRY_T *nf_portal_check_dest_net_baw(UINT32 uiIp)
{
    NET_LIST_MAP_T *pstListMap=NULL;
    NF_PORTAL_NET_BAW_ENTRY_T *pstEntry=NULL;
    UINT32  uiNet=0;
    UINT32  uiMaskNum=0;
    UINT32  uiMask=0;
    
    NF_PORTAL_DEST_NET_BAW_HASH_RD_LOCK;
    for (uiMaskNum = 0;uiMaskNum < NET_MASK_NUM_MAX; uiMaskNum++)
    {
        pstListMap=&g_astDestNetBAWListMap[uiMaskNum];
        if(0 == pstListMap->uiCount)
        {
            continue;
        }
        uiMask=nf_bit_to_netmask(uiMaskNum+1);
        uiNet=uiIp&uiMask;
        NF_PORTAL_DEST_NET_BAW_HASH_RD_UNLOCK;
        pstEntry = nf_portal_dest_net_baw_get(pstListMap,uiNet,uiMaskNum+1);
        NF_PORTAL_DEST_NET_BAW_HASH_RD_LOCK;
        if(NULL != pstEntry)
        {
            NF_PORTAL_DEST_NET_BAW_HASH_RD_UNLOCK;
            return pstEntry;
        }
    }
    NF_PORTAL_DEST_NET_BAW_HASH_RD_UNLOCK;
    return NULL;
}

NF_PORTAL_NET_BAW_ENTRY_T *nf_portal_dest_net_baw_get_frommap(UINT32 uiNet,UINT32 uiMaskNum)
{
    NET_LIST_MAP_T *pstListMap=NULL;
    
    pstListMap=&g_astDestNetBAWListMap[uiMaskNum-1];
    return nf_portal_dest_net_baw_get(pstListMap,uiNet,uiMaskNum);
}

AP_ERROR_CODE_E nf_portal_dest_net_baw_add(NET_LIST_MAP_T *pstListMap,UINT32 uiNet,UINT32 uiMaskNum,NF_PORTAL_BAW_FLAG_E enFlag)
{
    struct hlist_head *pstHead = NULL;
    NF_PORTAL_NET_BAW_ENTRY_T *pstEntry = NULL;
    
    pstEntry = nf_portal_dest_net_baw_get(pstListMap,uiNet,uiMaskNum);
    if (NULL != pstEntry)
    {
        if (pstEntry->stNetBAW.enFlag == enFlag)
        {
            return AP_E_NONE;
        }
        else
        {
            NF_PORTAL_DEST_NET_BAW_HASH_WR_LOCK;
            pstEntry->stNetBAW.enFlag = enFlag;
            NF_PORTAL_DEST_NET_BAW_HASH_WR_UNLOCK;
            return AP_E_NONE;
        }
    }

    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"create a new entry\n");
    pstEntry = (NF_PORTAL_NET_BAW_ENTRY_T*)(kmalloc(sizeof(NF_PORTAL_NET_BAW_ENTRY_T), GFP_ATOMIC));
    if (unlikely(NULL == pstEntry))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"malloc fail,return\n");
        return AP_E_MEM_MALLOC;
    }

    memset(pstEntry,0,sizeof(NF_PORTAL_IP_BAW_ENTRY_T));
    pstEntry->stNetBAW.uiNet = uiNet;
    pstEntry->stNetBAW.uiMaskNum = uiMaskNum;
    pstEntry->stNetBAW.enFlag = enFlag;
    
    NF_PORTAL_DEST_NET_BAW_HASH_WR_LOCK;
    pstHead = nf_portal_dest_net_hash_fn(pstListMap,uiNet,uiMaskNum);
    hlist_add_head(&(pstEntry->stNetBAWHList), pstHead);
    pstListMap->uiCount++;
    NF_PORTAL_DEST_NET_BAW_HASH_WR_UNLOCK;
    
    return AP_E_NONE;
}


AP_ERROR_CODE_E nf_portal_dest_net_baw_add_tomap(UINT32 uiNet,UINT32 uiMaskNum,NF_PORTAL_BAW_FLAG_E enFlag)
{
    NET_LIST_MAP_T *pstListMap=NULL;
    UINT32 uiMask=0;
    uiMask=nf_bit_to_netmask(uiMaskNum);
    pstListMap=&g_astDestNetBAWListMap[uiMaskNum-1];
    uiNet&=uiMask;
    return nf_portal_dest_net_baw_add(pstListMap,uiNet,uiMaskNum,enFlag);
}

AP_ERROR_CODE_E nf_portal_dest_net_baw_del(NET_LIST_MAP_T *pstNetBawMap,NF_PORTAL_NET_BAW_ENTRY_T *pstEntry)
{  
    if (NULL == pstEntry)
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR, "entry is null,return.\n");
        return AP_E_PARAM;
    }
    
    NF_PORTAL_DEST_NET_BAW_HASH_WR_LOCK;
    hlist_del_init(&(pstEntry->stNetBAWHList));
    pstNetBawMap->uiCount--;
    NF_PORTAL_DEST_NET_BAW_HASH_WR_UNLOCK;

    kfree(pstEntry);
    
    return AP_E_NONE;
}

AP_ERROR_CODE_E nf_portal_dest_net_baw_del_frommap(UINT32 uiNet,UINT32 uiMaskNum,NF_PORTAL_BAW_FLAG_E enFlag)
{
    NF_PORTAL_NET_BAW_ENTRY_T *pstEntry = NULL;
    NET_LIST_MAP_T *pstListMap=NULL;
    
    pstListMap=&g_astDestNetBAWListMap[uiMaskNum-1];
    pstEntry = nf_portal_dest_net_baw_get(pstListMap,uiNet,uiMaskNum);
    if (NULL == pstEntry)
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"ip:"IP_FMT"/%u,flag:%u\n",IP_ARG(&(uiNet)),\
            uiMaskNum,enFlag);
        return AP_E_NOT_EXIST;
    }

    if (pstEntry->stNetBAW.enFlag != enFlag)
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"ip:"IP_FMT"/%u,flag:%u\n",IP_ARG(&(uiNet)),\
            uiMaskNum,enFlag);
        return AP_E_NOT_EXIST;
    }
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"ip:"IP_FMT"/%u,flag:%u\n",IP_ARG(&(uiNet)),\
        uiMaskNum,enFlag);
    return nf_portal_dest_net_baw_del(pstListMap,pstEntry);
}

AP_ERROR_CODE_E nf_portal_dest_net_baw_get_list(NF_PORTAL_IOCTL_NET_BAW_LIST_T *pstNetBAWList)
{
    NF_PORTAL_NET_BAW_ENTRY_T *pstEntry = NULL;
    NF_PORTAL_NET_BAW_ATTRIB_T *pstNetBaw = NULL;
    NET_LIST_MAP_T *pstNetBawMap = NULL;
    struct hlist_node *pstTmp;
    struct hlist_node *pstTmp2;
    struct hlist_head *pstHead;
    UINT32 uiIndex;
    UINT32 uiMaskNum = 0;
    UINT32 uiEntryNum = 0;
    UINT32 uiSkipNum = 0;
    UINT32 uiStaNum = 0;
    NF_PORTAL_BAW_FLAG_E enFlag;
    
    if (NULL == pstNetBAWList)
    {
        return AP_E_PARAM;
    }

    enFlag = pstNetBAWList->enFlag;
    
    NF_PORTAL_DEST_NET_BAW_HASH_RD_LOCK;
    for (uiMaskNum = 0;uiMaskNum < NET_MASK_NUM_MAX; uiMaskNum++)
    {
        pstNetBawMap=&g_astDestNetBAWListMap[uiMaskNum];
        if(0 == pstNetBawMap->uiCount)
        {
            continue;
        }
        for (uiIndex = 0;uiIndex < NF_PORTAL_DEST_NET_BAW_HLIST_SIZE; uiIndex++)
        {
            pstHead = &pstNetBawMap->stDestNetBAWHashTable[uiIndex];
            hlist_for_each_entry_safe(pstEntry,pstTmp,pstTmp2,pstHead,stNetBAWHList)
            {
                /*跳过之前访问过的队列*/
                if (uiSkipNum < pstNetBAWList->uiStartNum)
                {
                    uiSkipNum++;
                    uiEntryNum++;
                    continue;
                }
                
                /*获取到了足够的表项*/
                if (uiStaNum >= NF_PORTAL_NET_BAW_LIST_MAX)
                {
                    break;
                }

                if (enFlag == pstEntry->stNetBAW.enFlag)
                {
                    /*获取Entity*/
                    pstNetBaw = &(pstNetBAWList->astNetBaw[uiStaNum]);
                    pstNetBaw->uiNet = pstEntry->stNetBAW.uiNet;
                    pstNetBaw->uiMaskNum = pstEntry->stNetBAW.uiMaskNum;
                    pstNetBaw->enFlag = pstEntry->stNetBAW.enFlag;
      
                    uiStaNum++;
                    uiEntryNum++;
                }
                else
                {
                    uiEntryNum++;
                }
            }
        }
    }
    NF_PORTAL_DEST_NET_BAW_HASH_RD_UNLOCK;
    pstNetBAWList->uiNum = uiStaNum;
    pstNetBAWList->uiStartNum = uiEntryNum;
    return AP_E_NONE;
    
}

AP_ERROR_CODE_E nf_portal_dest_net_baw_flush(NF_PORTAL_BAW_FLAG_E enFlag)
{
    NF_PORTAL_NET_BAW_ENTRY_T *pstEntry = NULL;
    NET_LIST_MAP_T *pstNetBawMap = NULL;
    struct hlist_node *pstTmp;
    struct hlist_node *pstTmp2;
    struct hlist_head *pstHead;
    UINT32 uiIndex = 0;
    UINT32 uiMaskNum = 0;
    
    NF_PORTAL_DEST_NET_BAW_HASH_RD_LOCK;
    for (uiMaskNum = 0;uiMaskNum < NET_MASK_NUM_MAX; uiMaskNum++)
    {
        pstNetBawMap=&g_astDestNetBAWListMap[uiMaskNum];
        if(0 == pstNetBawMap->uiCount)
        {
            continue;
        }
        for (uiIndex = 0;uiIndex < NF_PORTAL_DEST_NET_BAW_HLIST_SIZE; uiIndex++)
        {
            pstHead = &pstNetBawMap->stDestNetBAWHashTable[uiIndex];
            hlist_for_each_entry_safe(pstEntry,pstTmp,pstTmp2,pstHead,stNetBAWHList)
            {
                if (enFlag == BAW)
                {
                    NF_PORTAL_DEST_NET_BAW_HASH_RD_UNLOCK;
                    nf_portal_dest_net_baw_del(pstNetBawMap,pstEntry);
                    NF_PORTAL_DEST_NET_BAW_HASH_RD_LOCK;
                }
                else
                {
                    if (enFlag == pstEntry->stNetBAW.enFlag)
                    {
                        NF_PORTAL_DEST_NET_BAW_HASH_RD_UNLOCK;
                        nf_portal_dest_net_baw_del(pstNetBawMap,pstEntry);
                        NF_PORTAL_DEST_NET_BAW_HASH_RD_LOCK;
                    }
                }
            }
        }
    }
    NF_PORTAL_DEST_NET_BAW_HASH_RD_UNLOCK; 
    return AP_E_NONE;
}/*免认证IP*/
LOCAL struct hlist_head *nf_portal_user_ip_hash_fn(UINT32 uiIp)
{
    UINT32 uiIndex;

    uiIndex = nf_portal_ip_hash(uiIp);
    
    return &(g_astUserIpBAWHashTable[uiIndex& (NF_PORTAL_USER_IP_BAW_HLIST_SIZE - 1)]);
}

NF_PORTAL_IP_BAW_ENTRY_T *nf_portal_user_ip_baw_get(UINT32 uiIp)
{
    NF_PORTAL_IP_BAW_ENTRY_T *pstEntry = NULL;
    struct hlist_node *pos = NULL;
    struct hlist_node *n = NULL;
    
    NF_PORTAL_USER_IP_BAW_HASH_RD_LOCK;
    hlist_for_each_safe(pos, n,nf_portal_user_ip_hash_fn(uiIp))
    {
        pstEntry = hlist_entry(pos, NF_PORTAL_IP_BAW_ENTRY_T, stIpBAWHList);

        if (uiIp == pstEntry->stIpBAW.uiIp)
        {
            NF_PORTAL_USER_IP_BAW_HASH_RD_UNLOCK;
            return pstEntry; /*Find it. Return at once.*/
        }
    }
	NF_PORTAL_USER_IP_BAW_HASH_RD_UNLOCK;
	return NULL;
}
AP_ERROR_CODE_E nf_portal_user_ip_baw_add(UINT32 uiIp,NF_PORTAL_BAW_FLAG_E enFlag)
{
    struct hlist_head *pstHead = NULL;
    NF_PORTAL_IP_BAW_ENTRY_T *pstEntry = NULL;

    pstEntry = nf_portal_user_ip_baw_get(uiIp);

    if (NULL != pstEntry)
    {
        if (pstEntry->stIpBAW.enFlag == enFlag)
        {
            return AP_E_NONE;
        }
        else
        {
            NF_PORTAL_USER_IP_BAW_HASH_WR_LOCK;
            pstEntry->stIpBAW.enFlag = enFlag;
            NF_PORTAL_USER_IP_BAW_HASH_WR_UNLOCK;
            return AP_E_NONE;
        }
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"create a new entry\n");
    pstEntry = (NF_PORTAL_IP_BAW_ENTRY_T*)(kmalloc(sizeof(NF_PORTAL_IP_BAW_ENTRY_T), GFP_ATOMIC));

    if (unlikely(NULL == pstEntry))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"malloc fail,return\n");
        return AP_E_MEM_MALLOC;
    }

    memset(pstEntry,0,sizeof(NF_PORTAL_IP_BAW_ENTRY_T));
    pstEntry->stIpBAW.uiIp = uiIp;
    pstEntry->stIpBAW.enFlag = enFlag;
    
    NF_PORTAL_USER_IP_BAW_HASH_WR_LOCK;
    pstHead = nf_portal_user_ip_hash_fn(uiIp);
    hlist_add_head(&(pstEntry->stIpBAWHList), pstHead);
    NF_PORTAL_USER_IP_BAW_HASH_WR_UNLOCK;
    
    return AP_E_NONE;
}


AP_ERROR_CODE_E nf_portal_user_ip_baw_del(NF_PORTAL_IP_BAW_ENTRY_T *pstEntry)
{  
    if (NULL == pstEntry)
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR, "entry is null,return.\n");
        return AP_E_PARAM;
    }
    
    NF_PORTAL_USER_IP_BAW_HASH_WR_LOCK;
    hlist_del_init(&(pstEntry->stIpBAWHList));
    NF_PORTAL_USER_IP_BAW_HASH_WR_UNLOCK;

    kfree(pstEntry);
    
    return AP_E_NONE;
}
AP_ERROR_CODE_E nf_portal_user_ip_baw_get_list(NF_PORTAL_IOCTL_IP_BAW_LIST_T *pstIpBAWList)
{
    NF_PORTAL_IP_BAW_ENTRY_T *pstEntry = NULL;
    NF_PORTAL_IP_BAW_ATTRIB_T *pstIpBaw = NULL;
    struct hlist_node *pstTmp;
    struct hlist_node *pstTmp2;
    struct hlist_head *pstHead;
    UINT32 uiIndex;
    UINT32 uiEntryNum = 0;
    UINT32 uiSkipNum = 0;
    UINT32 uiStaNum = 0;
    NF_PORTAL_BAW_FLAG_E enFlag;
    
    if (NULL == pstIpBAWList)
    {
        return AP_E_PARAM;
    }

    enFlag = pstIpBAWList->enFlag;
    
    NF_PORTAL_USER_IP_BAW_HASH_RD_LOCK;
    for (uiIndex = 0;uiIndex < NF_PORTAL_USER_IP_BAW_HLIST_SIZE; uiIndex++)
    {
        pstHead = &g_astUserIpBAWHashTable[uiIndex];
        hlist_for_each_entry_safe(pstEntry,pstTmp,pstTmp2,pstHead,stIpBAWHList)
        {
            /*跳过之前访问过的队列*/
            if (uiSkipNum < pstIpBAWList->uiStartNum)
            {
                uiSkipNum++;
                uiEntryNum++;
                continue;
            }
            
            /*获取到了足够的表项*/
            if (uiStaNum >= NF_PORTAL_IP_BAW_LIST_MAX)
            {
                break;
            }

            if (enFlag == pstEntry->stIpBAW.enFlag)
            {
                /*获取Entity*/
                pstIpBaw = &(pstIpBAWList->astIpBaw[uiStaNum]);
                pstIpBaw->uiIp = pstEntry->stIpBAW.uiIp;
                pstIpBaw->enFlag = pstEntry->stIpBAW.enFlag;
  
                uiStaNum++;
                uiEntryNum++;
            }
            else
            {
                uiEntryNum++;
            }
        }
    }
    NF_PORTAL_USER_IP_BAW_HASH_RD_UNLOCK;
    pstIpBAWList->uiNum = uiStaNum;
    pstIpBAWList->uiStartNum = uiEntryNum;
    return AP_E_NONE;
    
}
AP_ERROR_CODE_E nf_portal_user_ip_baw_flush(NF_PORTAL_BAW_FLAG_E enFlag)
{
    NF_PORTAL_IP_BAW_ENTRY_T *pstEntry = NULL;
    struct hlist_node *pstTmp;
    struct hlist_node *pstTmp2;
    struct hlist_head *pstHead;
    UINT32 uiIndex;  
    
    NF_PORTAL_USER_IP_BAW_HASH_RD_LOCK;
    for (uiIndex = 0;uiIndex < NF_PORTAL_USER_IP_BAW_HLIST_SIZE; uiIndex++)
    {
        pstHead = &g_astUserIpBAWHashTable[uiIndex];
        hlist_for_each_entry_safe(pstEntry,pstTmp,pstTmp2,pstHead,stIpBAWHList)
        {
            if (enFlag == BAW)
            {
                NF_PORTAL_USER_IP_BAW_HASH_RD_UNLOCK;
                nf_portal_user_ip_baw_del(pstEntry);
                NF_PORTAL_USER_IP_BAW_HASH_RD_LOCK;
            }
            else
            {
                if (enFlag == pstEntry->stIpBAW.enFlag)
                {
                    NF_PORTAL_USER_IP_BAW_HASH_RD_UNLOCK;
                    nf_portal_user_ip_baw_del(pstEntry);
                    NF_PORTAL_USER_IP_BAW_HASH_RD_LOCK;
                }
            }
        }
    }
    NF_PORTAL_USER_IP_BAW_HASH_RD_UNLOCK; 
    return AP_E_NONE;
}

/*免认证端口*/
LOCAL inline UINT32 nf_portal_port_name_hash(const UINT8 *pucPortName)
{
    UINT32 x;
    UINT32 uiLen = strlen(pucPortName);
    //NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG, "%s ,len = %d.\n",pucPortName,uiLen);
    x = pucPortName[uiLen-1];
    return x;
}
LOCAL struct hlist_head *nf_portal_port_name_hash_fn(const UINT8 *pucPortName)
{
    UINT32 uiIndex;

    uiIndex = nf_portal_port_name_hash(pucPortName);
    
    return &(g_astPortHashTable[uiIndex& (NF_PORTAL_PORT_HLIST_SIZE - 1)]);
}
NF_PORTAL_PORT_ENTRY_T *nf_portal_free_auth_port_get(UINT8 *pucPortName)
{
    NF_PORTAL_PORT_ENTRY_T *pstEntry = NULL;
    struct hlist_node *pos = NULL;
    struct hlist_node *n = NULL;

    if (unlikely(NULL == pucPortName))
    {
        return NULL;
    }
    
    NF_PORTAL_PORT_HASH_RD_LOCK;
    hlist_for_each_safe(pos, n,nf_portal_port_name_hash_fn(pucPortName))
    {
        pstEntry = hlist_entry(pos, NF_PORTAL_PORT_ENTRY_T, stPortHList);
        if (0 == strcmp(pstEntry->aucPortName,pucPortName))
        {
            NF_PORTAL_PORT_HASH_RD_UNLOCK;
            return pstEntry; /*Find it. Return at once.*/
        }
    }
	NF_PORTAL_PORT_HASH_RD_UNLOCK;
	
	return NULL;
}
AP_ERROR_CODE_E nf_portal_free_auth_port_add(UINT8 *pucPortName)
{
    struct hlist_head *pstHead = NULL;
    NF_PORTAL_PORT_ENTRY_T *pstEntry = NULL;

    if (unlikely(NULL == pucPortName))
    {
        return AP_E_PARAM;
    }
    pstEntry = nf_portal_free_auth_port_get(pucPortName);

    if (NULL != pstEntry)
    {
        return AP_E_NONE;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"create a new entry\n");
    pstEntry = (NF_PORTAL_PORT_ENTRY_T*)(kmalloc(sizeof(NF_PORTAL_PORT_ENTRY_T), GFP_ATOMIC));

    if (unlikely(NULL == pstEntry))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"malloc fail,return\n");
        return AP_E_MEM_MALLOC;
    }
    memset(pstEntry,0,sizeof(NF_PORTAL_PORT_ENTRY_T));
    memcpy(pstEntry->aucPortName,pucPortName,IFNAMSIZ);
    
    NF_PORTAL_PORT_HASH_WR_LOCK;
    pstHead = nf_portal_port_name_hash_fn(pucPortName);
    hlist_add_head(&(pstEntry->stPortHList), pstHead);
    NF_PORTAL_PORT_HASH_WR_UNLOCK;
    
    return AP_E_NONE;
}

AP_ERROR_CODE_E nf_portal_free_auth_port_del(NF_PORTAL_PORT_ENTRY_T *pstEntry)
{  
    if (NULL == pstEntry)
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR, "entry is null,return.\n");
        return AP_E_PARAM;
    }
    
    NF_PORTAL_PORT_HASH_WR_LOCK;
    hlist_del_init(&(pstEntry->stPortHList));
    NF_PORTAL_PORT_HASH_WR_UNLOCK;

    kfree(pstEntry);
    
    return AP_E_NONE;
}

AP_ERROR_CODE_E nf_portal_free_auth_port_get_list(NF_PORTAL_IOCTL_PORT_LIST_T *pstPortList)
{
    NF_PORTAL_PORT_ENTRY_T *pstEntry = NULL;
    NF_PORTAL_IOCTL_IF_NAME_T *pstPortame  = NULL;
    struct hlist_node *pstTmp;
    struct hlist_node *pstTmp2;
    struct hlist_head *pstHead;
    UINT32 uiIndex;
    UINT32 uiEntryNum = 0;
    UINT32 uiSkipNum = 0;
    UINT32 uiStaNum = 0;

    if (NULL == pstPortList)
    {
        return AP_E_PARAM;
    }
    
    NF_PORTAL_PORT_HASH_RD_LOCK;
    for (uiIndex = 0;uiIndex < NF_PORTAL_PORT_HLIST_SIZE; uiIndex++)
    {
        pstHead = &g_astPortHashTable[uiIndex];
        hlist_for_each_entry_safe(pstEntry,pstTmp,pstTmp2,pstHead,stPortHList)
        {
            /*跳过之前访问过的队列*/
            if (uiSkipNum < pstPortList->uiStartNum)
            {
                uiSkipNum++;
                uiEntryNum++;
                continue;
            }
            
            /*获取到了足够的表项*/
            if (uiStaNum >= NF_PORTAL_PORT_LIST_MAX)
            {
                break;
            }

            /*获取Entity*/
            pstPortame = &(pstPortList->astPort[uiStaNum]);
            memcpy(pstPortame,pstEntry->aucPortName,IFNAMSIZ);
            uiStaNum++;
            uiEntryNum++;
        }
    }
    NF_PORTAL_PORT_HASH_RD_UNLOCK;
    pstPortList->uiNum = uiStaNum;
    pstPortList->uiStartNum = uiEntryNum;
    return AP_E_NONE;
    
}

AP_ERROR_CODE_E nf_portal_free_auth_port_flush(VOID)
{
    NF_PORTAL_PORT_ENTRY_T *pstEntry = NULL;
    struct hlist_node *pstTmp;
    struct hlist_node *pstTmp2;
    struct hlist_head *pstHead;
    UINT32 uiIndex;  
    
    NF_PORTAL_PORT_HASH_RD_LOCK;
    for (uiIndex = 0;uiIndex < NF_PORTAL_PORT_HLIST_SIZE; uiIndex++)
    {
        pstHead = &g_astPortHashTable[uiIndex];
        hlist_for_each_entry_safe(pstEntry,pstTmp,pstTmp2,pstHead,stPortHList)
        {
            NF_PORTAL_PORT_HASH_RD_UNLOCK;
            nf_portal_free_auth_port_del(pstEntry);
            NF_PORTAL_PORT_HASH_RD_LOCK;
        }
    }
    NF_PORTAL_PORT_HASH_RD_UNLOCK; 
    return AP_E_NONE;
}
LOCAL struct hlist_head *nf_portal_mac_hash_fn(UINT8 *pucMac)
{
    UINT32 uiIndex;

    uiIndex = nf_portal_mac_hash(pucMac);
    
    return &(g_astMacBAWHashTable[uiIndex& (NF_PORTAL_MAC_BAW_HLIST_SIZE - 1)]);
}

NF_PORTAL_MAC_BAW_ENTRY_T *nf_portal_mac_baw_get(UINT8 *pucMac)
{
    NF_PORTAL_MAC_BAW_ENTRY_T *pstEntry = NULL;
    struct hlist_node *pos = NULL;
    struct hlist_node *n = NULL;

    if (unlikely(NULL == pucMac))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"mac is null,return\n");
        return NULL;
    }
    
    NF_PORTAL_MAC_BAW_HASH_RD_LOCK;
    hlist_for_each_safe(pos, n,nf_portal_mac_hash_fn(pucMac))
    {
        pstEntry = hlist_entry(pos, NF_PORTAL_MAC_BAW_ENTRY_T, stMacBAWHList);

        if (0 == memcmp(pstEntry->stMacBAW.aucMac,pucMac,ETH_ALEN))
        {
            NF_PORTAL_MAC_BAW_HASH_RD_UNLOCK;
            return pstEntry; /*Find it. Return at once.*/
        }
    }
	NF_PORTAL_MAC_BAW_HASH_RD_UNLOCK;
	
	return NULL;
}
AP_ERROR_CODE_E nf_portal_mac_baw_add(UINT8 *pucMac,NF_PORTAL_BAW_FLAG_E enFlag)
{
    struct hlist_head *pstHead = NULL;
    NF_PORTAL_MAC_BAW_ENTRY_T *pstEntry = NULL;

    if (unlikely(NULL == pucMac))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"mac is null,return\n");
        return AP_E_PARAM;
    }
    
    pstEntry = nf_portal_mac_baw_get(pucMac);

    if (NULL != pstEntry)
    {
        if (pstEntry->stMacBAW.enFlag == enFlag)
        {
            return AP_E_NONE;
        }
        else
        {
            NF_PORTAL_MAC_BAW_HASH_WR_LOCK;
            if((pstEntry->stMacBAW.enFlag == WHITE) && (enFlag != WHITE))
            {
              g_uiMacWhiteListCount --;
                NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"MacWhiteListCount = %u\n",g_uiMacWhiteListCount);
            }
            else if ((pstEntry->stMacBAW.enFlag != WHITE) && (enFlag == WHITE))
            {
                g_uiMacWhiteListCount ++;
                NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"MacWhiteListCount = %u\n",g_uiMacWhiteListCount);
            }
            pstEntry->stMacBAW.enFlag = enFlag;
            NF_PORTAL_MAC_BAW_HASH_WR_UNLOCK;
            return AP_E_NONE;
        }
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"create a new entry\n");
    pstEntry = (NF_PORTAL_MAC_BAW_ENTRY_T*)(kmalloc(sizeof(NF_PORTAL_MAC_BAW_ENTRY_T), GFP_ATOMIC));

    if (unlikely(NULL == pstEntry))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"malloc fail,return\n");
        return AP_E_MEM_MALLOC;
    }

    memset(pstEntry,0,sizeof(NF_PORTAL_MAC_BAW_ENTRY_T));
    memcpy(pstEntry->stMacBAW.aucMac,pucMac,ETH_ALEN);
    pstEntry->stMacBAW.enFlag = enFlag;
    
    NF_PORTAL_MAC_BAW_HASH_WR_LOCK;
    pstHead = nf_portal_mac_hash_fn(pucMac);
    hlist_add_head(&(pstEntry->stMacBAWHList), pstHead);
    if (enFlag == WHITE)
    {
        g_uiMacWhiteListCount++;
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"MacWhiteListCount = %u\n",g_uiMacWhiteListCount);
    }
    NF_PORTAL_MAC_BAW_HASH_WR_UNLOCK;
    
    return AP_E_NONE;
}
AP_ERROR_CODE_E nf_portal_mac_baw_del(NF_PORTAL_MAC_BAW_ENTRY_T *pstEntry)
{  
    if (NULL == pstEntry)
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR, "entry is null,return.\n");
        return AP_E_PARAM;
    }
    
    NF_PORTAL_MAC_BAW_HASH_WR_LOCK;
    hlist_del_init(&(pstEntry->stMacBAWHList));
    if (pstEntry->stMacBAW.enFlag == WHITE)
    {
        g_uiMacWhiteListCount --;
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"MacWhiteListCount = %u\n",g_uiMacWhiteListCount);
    }
    NF_PORTAL_MAC_BAW_HASH_WR_UNLOCK;

    kfree(pstEntry);
    
    return AP_E_NONE;
}

AP_ERROR_CODE_E nf_portal_mac_baw_get_list(NF_PORTAL_IOCTL_MAC_BAW_LIST_T *pstMacBAWList)
{
    NF_PORTAL_MAC_BAW_ENTRY_T *pstEntry = NULL;
    NF_PORTAL_MAC_BAW_ATTRIB_T *pstMacBaw = NULL;
    struct hlist_node *pstTmp;
    struct hlist_node *pstTmp2;
    struct hlist_head *pstHead;
    UINT32 uiIndex;
    UINT32 uiEntryNum = 0;
    UINT32 uiSkipNum = 0;
    UINT32 uiStaNum = 0;
    NF_PORTAL_BAW_FLAG_E enFlag;
    
    if (NULL == pstMacBAWList)
    {
        return AP_E_PARAM;
    }

    enFlag = pstMacBAWList->enFlag;
    
    NF_PORTAL_MAC_BAW_HASH_RD_LOCK;
    for (uiIndex = 0;uiIndex < NF_PORTAL_MAC_BAW_HLIST_SIZE; uiIndex++)
    {
        pstHead = &g_astMacBAWHashTable[uiIndex];
        hlist_for_each_entry_safe(pstEntry,pstTmp,pstTmp2,pstHead,stMacBAWHList)
        {
            /*跳过之前访问过的队列*/
            if (uiSkipNum < pstMacBAWList->uiStartNum)
            {
                uiSkipNum++;
                uiEntryNum++;
                continue;
            }
            
            /*获取到了足够的表项*/
            if (uiStaNum >= NF_PORTAL_MAC_BAW_LIST_MAX)
            {
                break;
            }

            if (enFlag == pstEntry->stMacBAW.enFlag)
            {
                /*获取Entity*/
                pstMacBaw = &(pstMacBAWList->astMacBaw[uiStaNum]);
                memcpy(pstMacBaw->aucMac,pstEntry->stMacBAW.aucMac,ETH_ALEN);
                pstMacBaw->enFlag = pstEntry->stMacBAW.enFlag;
  
                uiStaNum++;
                uiEntryNum++;
            }
            else
            {
                uiEntryNum++;
            }
        }
    }
    NF_PORTAL_MAC_BAW_HASH_RD_UNLOCK;
    pstMacBAWList->uiNum = uiStaNum;
    pstMacBAWList->uiStartNum = uiEntryNum;
    return AP_E_NONE;
    
}

AP_ERROR_CODE_E nf_portal_mac_baw_flush(NF_PORTAL_BAW_FLAG_E enFlag)
{
    NF_PORTAL_MAC_BAW_ENTRY_T *pstEntry = NULL;
    struct hlist_node *pstTmp;
    struct hlist_node *pstTmp2;
    struct hlist_head *pstHead;
    UINT32 uiIndex;  
    
    NF_PORTAL_MAC_BAW_HASH_RD_LOCK;
    for (uiIndex = 0;uiIndex < NF_PORTAL_MAC_BAW_HLIST_SIZE; uiIndex++)
    {
        pstHead = &g_astMacBAWHashTable[uiIndex];
        hlist_for_each_entry_safe(pstEntry,pstTmp,pstTmp2,pstHead,stMacBAWHList)
        {
            if (enFlag == BAW)
            {
                NF_PORTAL_MAC_BAW_HASH_RD_UNLOCK;
                nf_portal_mac_baw_del(pstEntry);
                NF_PORTAL_MAC_BAW_HASH_RD_LOCK;
            }
            else
            {
                if (enFlag == pstEntry->stMacBAW.enFlag)
                {
                    NF_PORTAL_MAC_BAW_HASH_RD_UNLOCK;
                    nf_portal_mac_baw_del(pstEntry);
                    NF_PORTAL_MAC_BAW_HASH_RD_LOCK;
                }
            }
        }
    }
    NF_PORTAL_MAC_BAW_HASH_RD_UNLOCK; 
    return AP_E_NONE;
}
UINT32 nf_portal_get_mac_white_list_count(VOID)
{
    return g_uiMacWhiteListCount;
}
/*URL黑白名单*/
LOCAL struct hlist_head *nf_portal_url_ip_hash_fn(UINT32 uiIp)
{
    UINT32 uiIndex;

    uiIndex = nf_portal_ip_hash(uiIp);
    
    return &(g_astUrlIpBAWHashTable[uiIndex& (NF_PORTAL_URL_IP_BAW_HLIST_SIZE - 1)]);
}

NF_PORTAL_URL_IP_BAW_ENTRY_T *nf_portal_url_ip_baw_get(UINT32 uiIp)
{
    NF_PORTAL_URL_IP_BAW_ENTRY_T *pstEntry = NULL;
    struct hlist_node *pos = NULL;
    struct hlist_node *n = NULL;
    
    NF_PORTAL_URL_IP_BAW_HASH_RD_LOCK;
    hlist_for_each_safe(pos, n,nf_portal_url_ip_hash_fn(uiIp))
    {
        pstEntry = hlist_entry(pos, NF_PORTAL_URL_IP_BAW_ENTRY_T, stIpBAWHList);

        if (uiIp == pstEntry->stIpBAW.uiIp)
        {
            NF_PORTAL_URL_IP_BAW_HASH_RD_UNLOCK;
            return pstEntry; /*Find it. Return at once.*/
        }
    }
	NF_PORTAL_URL_IP_BAW_HASH_RD_UNLOCK;
	return NULL;
}

AP_ERROR_CODE_E nf_portal_url_ip_baw_add(UINT32 uiIp,NF_PORTAL_URL_BAW_ENTRY_T *pstUrlBaw,UINT8 *pucUrl)
{
    struct hlist_head *pstHead = NULL;
    NF_PORTAL_URL_IP_BAW_ENTRY_T *pstEntry = NULL;

    if ((NULL == pstUrlBaw) || (NULL == pucUrl))
    {
        return AP_E_PARAM;
    }

	if(16843009 == uiIp) /*1.1.1.1的ip有特殊用途，不得加入白名单*/
		return AP_E_PARAM;
    
    pstEntry = nf_portal_url_ip_baw_get(uiIp);

    if (NULL != pstEntry)
    {
        return AP_E_NONE;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"create a new entry\n");
    pstEntry = (NF_PORTAL_URL_IP_BAW_ENTRY_T*)(kmalloc(sizeof(NF_PORTAL_URL_IP_BAW_ENTRY_T), GFP_ATOMIC));
    if (unlikely(NULL == pstEntry))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"malloc fail,return\n");
        return AP_E_MEM_MALLOC;
    }

    memset(pstEntry,0,sizeof(NF_PORTAL_URL_IP_BAW_ENTRY_T));
    pstEntry->stIpBAW.uiIp = uiIp;
    pstEntry->pstUrlBaw = pstUrlBaw;
    memcpy(pstEntry->aucRealUrl,pucUrl,URL_MAX_SIZE);
    
    NF_PORTAL_URL_IP_BAW_HASH_WR_LOCK;
    pstHead = nf_portal_url_ip_hash_fn(uiIp);
    hlist_add_head(&(pstEntry->stIpBAWHList), pstHead);
    list_add(&pstEntry->stIpList,&(pstUrlBaw->stIpList));
    NF_PORTAL_URL_IP_BAW_HASH_WR_UNLOCK;
    
    return AP_E_NONE;
}

AP_ERROR_CODE_E nf_portal_url_ip_baw_get_list(NF_PORTAL_IOCTL_URL_IP_BAW_LIST_T *pstUrlIpBAWList)
{
    NF_PORTAL_URL_IP_BAW_ENTRY_T *pstEntry = NULL;
    NF_PROTAL_IOCTL_URL_IP_BAW_T *pstIoUrlIpBaw = NULL;
    struct hlist_node *pstTmp;
    struct hlist_node *pstTmp2;
    struct hlist_head *pstHead;
    UINT32 uiIndex;
    UINT32 uiEntryNum = 0;
    UINT32 uiSkipNum = 0;
    UINT32 uiStaNum = 0;
    NF_PORTAL_BAW_FLAG_E enFlag;
    
    if (NULL == pstUrlIpBAWList)
    {
        return AP_E_PARAM;
    }

    enFlag = pstUrlIpBAWList->enFlag;
    
    NF_PORTAL_URL_IP_BAW_HASH_RD_LOCK;
    for (uiIndex = 0;uiIndex < NF_PORTAL_URL_IP_BAW_HLIST_SIZE; uiIndex++)
    {
        pstHead = &g_astUrlIpBAWHashTable[uiIndex];
        hlist_for_each_entry_safe(pstEntry,pstTmp,pstTmp2,pstHead,stIpBAWHList)
        {
            /*跳过之前访问过的队列*/
            if (uiSkipNum < pstUrlIpBAWList->uiStartNum)
            {
                uiSkipNum++;
                uiEntryNum++;
                continue;
            }
            
            /*获取到了足够的表项*/
            if (uiStaNum >= NF_PORTAL_URL_IP_BAW_LIST_MAX)
            {
                break;
            }

            
            if ((NULL != pstEntry->pstUrlBaw) && (enFlag == pstEntry->pstUrlBaw->stUrlBAW.enFlag))
            {
                /*获取Entity*/
                pstIoUrlIpBaw = &(pstUrlIpBAWList->astUrlIpBaw[uiStaNum]);
                pstIoUrlIpBaw->uiIp = pstEntry->stIpBAW.uiIp;
                //memcpy(pstIoUrlIpBaw->aucUrl,pstEntry->pstUrlBaw->stUrlBAW.aucUrl,URL_MAX_SIZE);
                memcpy(pstIoUrlIpBaw->aucUrl,pstEntry->aucRealUrl,URL_MAX_SIZE);
                
                uiStaNum++;
                uiEntryNum++;
            }
            else
            {
                uiEntryNum++;
            }
        }
    }
    NF_PORTAL_URL_IP_BAW_HASH_RD_UNLOCK;
    pstUrlIpBAWList->uiNum = uiStaNum;
    pstUrlIpBAWList->uiStartNum = uiEntryNum;
    return AP_E_NONE;
    
}
AP_ERROR_CODE_E nf_portal_url_ip_baw_del(NF_PORTAL_URL_IP_BAW_ENTRY_T *pstEntry)
{  
    if (NULL == pstEntry)
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR, "entry is null,return.\n");
        return AP_E_PARAM;
    }
    
    NF_PORTAL_URL_IP_BAW_HASH_WR_LOCK;
    hlist_del_init(&(pstEntry->stIpBAWHList));
    list_del_init(&pstEntry->stIpList);
    NF_PORTAL_URL_IP_BAW_HASH_WR_UNLOCK;

    kfree(pstEntry);
    
    return AP_E_NONE;
}

AP_ERROR_CODE_E nf_portal_url_ip_baw_flush(NF_PORTAL_URL_BAW_ENTRY_T *pstUrlBaw)
{
    NF_PORTAL_URL_IP_BAW_ENTRY_T *pstEntry = NULL;
    NF_PORTAL_URL_IP_BAW_ENTRY_T *pstTmp;
    
    NF_PORTAL_URL_IP_BAW_HASH_RD_LOCK;
    list_for_each_entry_safe(pstEntry,pstTmp,&(pstUrlBaw->stIpList),stIpList)
    {
        NF_PORTAL_URL_IP_BAW_HASH_RD_UNLOCK;
        nf_portal_url_ip_baw_del(pstEntry);
        NF_PORTAL_URL_IP_BAW_HASH_RD_LOCK;
    }
    NF_PORTAL_URL_IP_BAW_HASH_RD_UNLOCK; 

    return AP_E_NONE;
}
NF_PORTAL_URL_BAW_ENTRY_T *nf_portal_url_baw_get(UINT8 *pucUrl)
{
    NF_PORTAL_URL_BAW_ENTRY_T *pstEntry = NULL;
    NF_PORTAL_URL_BAW_ENTRY_T  *pstTmp;

    if (unlikely(NULL == pucUrl))
    {
        return NULL;
    }
    
    NF_PORTAL_URL_BAW_RD_LOCK;
    list_for_each_entry_safe(pstEntry,pstTmp,&g_stUrlBAWTable,stUrlBAWList)
    {
        //NF_PORTAL_LOG(NF_PORTAL_LOG_ERR, "pucUrl = %s,pstEntry->stUrlBAW.aucUrl=%s.\n",pucUrl,pstEntry->stUrlBAW.aucUrl);
        //if (0 == memcmp(pstEntry->stUrlBAW.aucUrl,pucUrl,strlen(pstEntry->stUrlBAW.aucUrl)))
        if (NULL != strstr(pucUrl,pstEntry->stUrlBAW.aucUrl))
        {
            NF_PORTAL_URL_BAW_RD_UNLOCK;
            return pstEntry;
        }
    }
    NF_PORTAL_URL_BAW_RD_UNLOCK;
    return NULL;
}

NF_PORTAL_URL_BAW_ENTRY_T *nf_portal_url_baw_get_for_user(UINT8 *pucUrl)
{
    NF_PORTAL_URL_BAW_ENTRY_T *pstEntry = NULL;
    NF_PORTAL_URL_BAW_ENTRY_T  *pstTmp;

    if (unlikely(NULL == pucUrl))
    {
        return NULL;
    }
    
    NF_PORTAL_URL_BAW_RD_LOCK;
    list_for_each_entry_safe(pstEntry,pstTmp,&g_stUrlBAWTable,stUrlBAWList)
    {
        if (0 == memcmp(pstEntry->stUrlBAW.aucUrl,pucUrl,max(strlen(pucUrl),strlen(pstEntry->stUrlBAW.aucUrl))))
        {
            NF_PORTAL_URL_BAW_RD_UNLOCK;
            return pstEntry;
        }
    }
    NF_PORTAL_URL_BAW_RD_UNLOCK;
    return NULL;
}

AP_ERROR_CODE_E nf_portal_url_baw_add(UINT8 *pucUrl,NF_PORTAL_BAW_FLAG_E enFlag)
{
    NF_PORTAL_URL_BAW_ENTRY_T *pstEntry = NULL;

    if (unlikely(NULL == pucUrl))
    {
        return AP_E_PARAM;
    }

    pstEntry = nf_portal_url_baw_get_for_user(pucUrl);

    if (NULL != pstEntry)
    {
        if (pstEntry->stUrlBAW.enFlag == enFlag)
        {
            return AP_E_NONE;
        }
        else
        {
            NF_PORTAL_URL_BAW_WR_LOCK;
            if((pstEntry->stUrlBAW.enFlag == WHITE) && (enFlag != WHITE))
            {
              g_uiUrlWhiteListCount --;
                NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"UrlWhiteListCount = %u\n",g_uiUrlWhiteListCount);
            }
            else if ((pstEntry->stUrlBAW.enFlag != WHITE) && (enFlag == WHITE))
            {
                g_uiUrlWhiteListCount ++;
                NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"UrlWhiteListCount = %u\n",g_uiUrlWhiteListCount);
            }
            pstEntry->stUrlBAW.enFlag = enFlag;            
            NF_PORTAL_URL_BAW_WR_UNLOCK;
            return AP_E_NONE;
        }
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"create a new entry\n");
    pstEntry = (NF_PORTAL_URL_BAW_ENTRY_T*)(kmalloc(sizeof(NF_PORTAL_URL_BAW_ENTRY_T), GFP_ATOMIC));

    if (unlikely(NULL == pstEntry))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"malloc fail,return\n");
        return AP_E_MEM_MALLOC;
    }

    memset(pstEntry,0,sizeof(NF_PORTAL_URL_BAW_ENTRY_T)); 
    memcpy(pstEntry->stUrlBAW.aucUrl,pucUrl,URL_MAX_SIZE);
    pstEntry->stUrlBAW.enFlag = enFlag;
    INIT_LIST_HEAD(&pstEntry->stIpList);
    
    NF_PORTAL_URL_BAW_WR_LOCK;
    list_add_tail(&pstEntry->stUrlBAWList,&g_stUrlBAWTable);
    if (enFlag == WHITE)
    {
        g_uiUrlWhiteListCount++;
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"UrlWhiteListCount = %u\n",g_uiUrlWhiteListCount);
    }
    NF_PORTAL_URL_BAW_WR_UNLOCK;
    
    return AP_E_NONE;
}

AP_ERROR_CODE_E nf_portal_url_baw_del(NF_PORTAL_URL_BAW_ENTRY_T *pstEntry)
{
    if (unlikely(NULL == pstEntry))
    {
        return AP_E_PARAM;
    }
    
    NF_PORTAL_URL_BAW_WR_LOCK;
    list_del_init(&pstEntry->stUrlBAWList);
    if (pstEntry->stUrlBAW.enFlag == WHITE)
    {
        g_uiUrlWhiteListCount --;
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"UrlWhiteListCount = %u\n",g_uiUrlWhiteListCount);
    }
    NF_PORTAL_URL_BAW_WR_UNLOCK;

    nf_portal_url_ip_baw_flush(pstEntry);
    kfree(pstEntry);
    return AP_E_NONE;
}

AP_ERROR_CODE_E nf_portal_url_baw_get_list(NF_PORTAL_IOCTL_URL_BAW_LIST_T *pstUrlBAWList)
{
    NF_PORTAL_URL_BAW_ENTRY_T *pstEntry = NULL;
    NF_PORTAL_URL_BAW_ATTRIB_T *pstUrlBaw = NULL;
    NF_PORTAL_URL_BAW_ENTRY_T *pstTmp;

    UINT32 uiEntryNum = 0;
    UINT32 uiSkipNum = 0;
    UINT32 uiStaNum = 0;
    NF_PORTAL_BAW_FLAG_E enFlag;
    
    if (NULL == pstUrlBAWList)
    {
        return AP_E_PARAM;
    }

    enFlag = pstUrlBAWList->enFlag;

    NF_PORTAL_URL_BAW_RD_LOCK;
    list_for_each_entry_safe(pstEntry,pstTmp,&g_stUrlBAWTable,stUrlBAWList)
    {
        /*跳过之前访问过的队列*/
        if (uiSkipNum < pstUrlBAWList->uiStartNum)
        {
            uiSkipNum++;
            uiEntryNum++;
            continue;
        }
            
        /*获取到了足够的表项*/
        if (uiStaNum >= NF_PORTAL_URL_BAW_LIST_MAX)
        {
            break;
        }

        if (enFlag == pstEntry->stUrlBAW.enFlag)
        {
            /*获取Entity*/
            pstUrlBaw = &(pstUrlBAWList->astUrlBaw[uiStaNum]);
            memcpy(pstUrlBaw->aucUrl,pstEntry->stUrlBAW.aucUrl,URL_MAX_SIZE);
            pstUrlBaw->enFlag = pstEntry->stUrlBAW.enFlag;

            uiStaNum++;
            uiEntryNum++;
        }
        else
        {
            uiEntryNum++;
        }
    }
    NF_PORTAL_URL_BAW_RD_UNLOCK;
    pstUrlBAWList->uiNum = uiStaNum;
    pstUrlBAWList->uiStartNum = uiEntryNum;
    return AP_E_NONE;
}

AP_ERROR_CODE_E nf_portal_url_baw_flush(NF_PORTAL_BAW_FLAG_E enFlag)
{
    NF_PORTAL_URL_BAW_ENTRY_T *pstEntry = NULL;
    NF_PORTAL_URL_BAW_ENTRY_T *pstTmp;
    
    NF_PORTAL_URL_BAW_RD_LOCK;
    list_for_each_entry_safe(pstEntry,pstTmp,&g_stUrlBAWTable,stUrlBAWList)
    {
        if (enFlag == BAW)
        {
            NF_PORTAL_URL_BAW_RD_UNLOCK;
            nf_portal_url_baw_del(pstEntry);
            NF_PORTAL_URL_BAW_RD_LOCK;
        }
        else
        {
            if (enFlag == pstEntry->stUrlBAW.enFlag)
            {
                NF_PORTAL_URL_BAW_RD_UNLOCK;
                nf_portal_url_baw_del(pstEntry);
                NF_PORTAL_URL_BAW_RD_LOCK;
            }
        }
    }
    NF_PORTAL_URL_BAW_RD_UNLOCK; 
    return AP_E_NONE;
}
UINT32 nf_portal_get_url_white_list_count(VOID)
{
    return g_uiUrlWhiteListCount;
}
/*透传端口*/
LOCAL inline UINT32 nf_portal_port_num_hash(const UINT16 usPortNum)
{
    return usPortNum;
}

LOCAL struct hlist_head *nf_portal_port_num_hash_fn(UINT16 usPortNum)
{
    UINT32 uiIndex;

    uiIndex = nf_portal_port_num_hash(usPortNum);
    
    return &(g_astPortNumHashTable[uiIndex& (NF_PORTAL_PORT_NUM_HLIST_SIZE - 1)]);
}

NF_PORTAL_PORT_NUM_ENTRY_T *nf_portal_port_num_get(UINT16 usPortNum,UINT8 ucIpProto)
{
    NF_PORTAL_PORT_NUM_ENTRY_T *pstEntry = NULL;
    struct hlist_node *pos = NULL;
    struct hlist_node *n = NULL;
    
    NF_PORTAL_PORT_NUM_HASH_RD_LOCK;
    hlist_for_each_safe(pos, n,nf_portal_port_num_hash_fn(usPortNum))
    {
        pstEntry = hlist_entry(pos, NF_PORTAL_PORT_NUM_ENTRY_T, stPortNumHList);

        if ((usPortNum == pstEntry->stPortNum.usPortNum) && (ucIpProto == pstEntry->stPortNum.ucIpProto))
        {
            NF_PORTAL_PORT_NUM_HASH_RD_UNLOCK;
            return pstEntry; /*Find it. Return at once.*/
        }
    }
	NF_PORTAL_PORT_NUM_HASH_RD_UNLOCK;
	return NULL;
}

AP_ERROR_CODE_E nf_portal_port_num_add(UINT16 usPortNum,UINT8 ucIpProto)
{
    struct hlist_head *pstHead = NULL;
    NF_PORTAL_PORT_NUM_ENTRY_T *pstEntry = NULL;

    pstEntry = nf_portal_port_num_get(usPortNum,ucIpProto);

    if (NULL != pstEntry)
    {
        return AP_E_NONE;
    }
    
    NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"create a new entry\n");
    pstEntry = (NF_PORTAL_PORT_NUM_ENTRY_T*)(kmalloc(sizeof(NF_PORTAL_PORT_NUM_ENTRY_T), GFP_ATOMIC));
    if (unlikely(NULL == pstEntry))
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"malloc fail,return\n");
        return AP_E_MEM_MALLOC;
    }

    memset(pstEntry,0,sizeof(NF_PORTAL_PORT_NUM_ENTRY_T));
    pstEntry->stPortNum.ucIpProto = ucIpProto;
    pstEntry->stPortNum.usPortNum = usPortNum;
    
    NF_PORTAL_PORT_NUM_HASH_WR_LOCK;
    pstHead = nf_portal_port_num_hash_fn(usPortNum);
    hlist_add_head(&(pstEntry->stPortNumHList), pstHead);
    NF_PORTAL_PORT_NUM_HASH_WR_UNLOCK;
    
    return AP_E_NONE;
}


AP_ERROR_CODE_E nf_portal_port_num_del(NF_PORTAL_PORT_NUM_ENTRY_T *pstEntry)
{  
    if (NULL == pstEntry)
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR, "entry is null,return.\n");
        return AP_E_PARAM;
    }
    
    NF_PORTAL_PORT_NUM_HASH_WR_LOCK;
    hlist_del_init(&(pstEntry->stPortNumHList));
    NF_PORTAL_PORT_NUM_HASH_WR_UNLOCK;

    kfree(pstEntry);
    
    return AP_E_NONE;
}

AP_ERROR_CODE_E nf_portal_port_num_get_list(NF_PORTAL_IOCTL_PORT_NUM_LIST_T *pstPortNumList)
{
    NF_PORTAL_PORT_NUM_ENTRY_T *pstEntry = NULL;
    NF_PORTAL_PORT_NUM_ATTRIB_T *pstPortNum = NULL;
    struct hlist_node *pstTmp;
    struct hlist_node *pstTmp2;
    struct hlist_head *pstHead;
    UINT32 uiIndex;
    UINT32 uiEntryNum = 0;
    UINT32 uiSkipNum = 0;
    UINT32 uiStaNum = 0;
    
    if (NULL == pstPortNumList)
    {
        return AP_E_PARAM;
    }
    
    NF_PORTAL_PORT_NUM_HASH_RD_LOCK;
    for (uiIndex = 0;uiIndex < NF_PORTAL_PORT_NUM_HLIST_SIZE; uiIndex++)
    {
        pstHead = &g_astPortNumHashTable[uiIndex];
        hlist_for_each_entry_safe(pstEntry,pstTmp,pstTmp2,pstHead,stPortNumHList)
        {
            /*跳过之前访问过的队列*/
            if (uiSkipNum < pstPortNumList->uiStartNum)
            {
                uiSkipNum++;
                uiEntryNum++;
                continue;
            }
            
            /*获取到了足够的表项*/
            if (uiStaNum >= NF_PORTAL_PORT_NUM_LIST_MAX)
            {
                break;
            }

            if (pstPortNumList->ucIpProto == pstEntry->stPortNum.ucIpProto)
            {
                /*获取Entity*/
                pstPortNum = &(pstPortNumList->astPortNum[uiStaNum].stPortNum);
                pstPortNum->ucIpProto = pstEntry->stPortNum.ucIpProto;
                pstPortNum->usPortNum = pstEntry->stPortNum.usPortNum;
  
                uiStaNum++;
                uiEntryNum++;
            }
            else
            {
                uiEntryNum++;
            }
        }
    }
    NF_PORTAL_PORT_NUM_HASH_RD_UNLOCK;
    pstPortNumList->uiNum = uiStaNum;
    pstPortNumList->uiStartNum = uiEntryNum;
    return AP_E_NONE;
    
}

AP_ERROR_CODE_E nf_portal_port_num_flush(UINT8 ucIpProto)
{
    NF_PORTAL_PORT_NUM_ENTRY_T *pstEntry = NULL;
    struct hlist_node *pstTmp;
    struct hlist_node *pstTmp2;
    struct hlist_head *pstHead;
    UINT32 uiIndex;  
    
    NF_PORTAL_PORT_NUM_HASH_RD_LOCK;
    for (uiIndex = 0;uiIndex < NF_PORTAL_PORT_NUM_HLIST_SIZE; uiIndex++)
    {
        pstHead = &g_astPortNumHashTable[uiIndex];
        hlist_for_each_entry_safe(pstEntry,pstTmp,pstTmp2,pstHead,stPortNumHList)
        {
            if (ucIpProto == pstEntry->stPortNum.ucIpProto)
            {
                NF_PORTAL_PORT_NUM_HASH_RD_UNLOCK;
                nf_portal_port_num_del(pstEntry);
                NF_PORTAL_PORT_NUM_HASH_RD_LOCK;
            }
        }
    }
    NF_PORTAL_PORT_NUM_HASH_RD_UNLOCK; 
    return AP_E_NONE;
}

/*begin: add@2015.01.31 for wechat connect wifi*/
struct hlist_head *nf_portal_wxl_serverip_hash_fn(UINT32 uiIp)
{
	UINT32 uiIndex;

	uiIndex = ((uiIp & 0xFFFFFF00) >> 8);//需要优化
	
	return &(g_astWxlServerHashTable[uiIndex % NF_PORTAL_WXL_SERVERIP_HLIST_SIZE]);
}

NF_PORTAL_WXL_SERVERIP_ENTRY_T *nf_portal_wxl_serverip_get(UINT32 uiIp)
{
    NF_PORTAL_WXL_SERVERIP_ENTRY_T *pstEntry = NULL;
    struct hlist_node *pos = NULL;
    struct hlist_node *n = NULL;

	UINT32 uiServerIp = 0;
	UINT32 uiServerIpMask = 0;
	UINT32 uiMask;

    NF_PORTAL_WXL_SERVERIP_HASH_RD_LOCK;
    hlist_for_each_safe(pos, n, nf_portal_wxl_serverip_hash_fn(uiIp))
    {
        pstEntry = hlist_entry(pos, NF_PORTAL_WXL_SERVERIP_ENTRY_T, stWxlServerIpHlist);

		uiServerIp = pstEntry->uiIp;
		uiServerIpMask = pstEntry->uiMask;
		uiMask = (0xFFFFFFFF << (32 - uiServerIpMask));

		if ((uiIp & uiMask) == (pstEntry->uiIp & uiMask))
        {	
            NF_PORTAL_WXL_SERVERIP_HASH_RD_UNLOCK;
            return pstEntry; /*Find it. Return at once.*/
        }
    }
	NF_PORTAL_WXL_SERVERIP_HASH_RD_UNLOCK;
	return NULL;
}

AP_ERROR_CODE_E nf_portal_wxl_serverip_add(UINT32 uiIp, UINT32 uiMask)
{
	struct hlist_head *pstHead = NULL;
	NF_PORTAL_WXL_SERVERIP_ENTRY_T *pstEntry = NULL;

	pstEntry = nf_portal_wxl_serverip_get(uiIp);

	if (NULL != pstEntry)
	{
		return AP_E_NONE;
	}
	
	NF_PORTAL_LOG(NF_PORTAL_LOG_INFO,"create a new entry\n");
	pstEntry = (NF_PORTAL_WXL_SERVERIP_ENTRY_T*)(kmalloc(sizeof(NF_PORTAL_WXL_SERVERIP_ENTRY_T), GFP_ATOMIC));
	if (unlikely(NULL == pstEntry))
	{
		NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"malloc fail,return\n");
		return AP_E_MEM_MALLOC;
	}

	memset(pstEntry,0,sizeof(NF_PORTAL_WXL_SERVERIP_ENTRY_T));
	pstEntry->uiIp = uiIp;
	pstEntry->uiMask = uiMask;
	
	NF_PORTAL_WXL_SERVERIP_HASH_WR_LOCK;
	pstHead = nf_portal_wxl_serverip_hash_fn(uiIp);
	hlist_add_head(&(pstEntry->stWxlServerIpHlist), pstHead);
	NF_PORTAL_WXL_SERVERIP_HASH_WR_UNLOCK;
	
	return AP_E_NONE;
}

AP_ERROR_CODE_E nf_portal_wxl_serverip_del(NF_PORTAL_WXL_SERVERIP_ENTRY_T *pstEntry)
{  
    if (NULL == pstEntry)
    {
        NF_PORTAL_LOG(NF_PORTAL_LOG_ERR, "entry is null,return.\n");
        return AP_E_PARAM;
    }
    
    NF_PORTAL_WXL_SERVERIP_HASH_WR_LOCK;
    hlist_del_init(&(pstEntry->stWxlServerIpHlist));
    NF_PORTAL_WXL_SERVERIP_HASH_WR_UNLOCK;

    kfree(pstEntry);
    
    return AP_E_NONE;
}

AP_ERROR_CODE_E nf_portal_wxl_serverip_get_list(NF_PORTAL_IOCTL_WXL_SERVERIP_LIST_T *pstWxlServerIpList)
{
	NF_PORTAL_WXL_SERVERIP_ENTRY_T *pstEntry = NULL;
	NF_PORTAL_WXL_SERVERIP_ATTRIB_T *pstWxlServerIp = NULL;
	struct hlist_node *pstTmp;
	struct hlist_node *pstTmp2;
	struct hlist_head *pstHead;
	UINT32 uiIndex;
	UINT32 uiEntryNum = 0;
	UINT32 uiSkipNum = 0;
	UINT32 uiStaNum = 0;
	
	if (NULL == pstWxlServerIpList)
	{
		return AP_E_PARAM;
	}
	
	NF_PORTAL_WXL_SERVERIP_HASH_RD_LOCK;
	for (uiIndex = 0;uiIndex < NF_PORTAL_WXL_SERVERIP_HLIST_SIZE; uiIndex++)
	{
		pstHead = &g_astWxlServerHashTable[uiIndex];
		hlist_for_each_entry_safe(pstEntry,pstTmp,pstTmp2,pstHead,stWxlServerIpHlist)
		{
			/*跳过之前访问过的队列*/
			if (uiSkipNum < pstWxlServerIpList->uiStartNum)
			{
				uiSkipNum++;
				uiEntryNum++;
				continue;
			}
			
			/*获取到了足够的表项*/
			if (uiStaNum >= NF_PORTAL_WXL_SERVERIP_NUM_LIST_MAX)
			{
				break;
			}
			
			/*获取Entity*/
			pstWxlServerIp = &(pstWxlServerIpList->astWxlServerip[uiStaNum]);
			pstWxlServerIp->uiIp = pstEntry->uiIp;
			pstWxlServerIp->uiMask = pstEntry->uiMask;

			uiStaNum++;
			uiEntryNum++;
		}
	}
	NF_PORTAL_WXL_SERVERIP_HASH_RD_UNLOCK;
	pstWxlServerIpList->uiNum = uiStaNum;
	pstWxlServerIpList->uiStartNum = uiEntryNum;
	return AP_E_NONE;
	
}

AP_ERROR_CODE_E nf_portal_wxl_serverip_flush(void)
{
	NF_PORTAL_WXL_SERVERIP_ENTRY_T *pstEntry = NULL;
	struct hlist_node *pstTmp;
	struct hlist_node *pstTmp2;
	struct hlist_head *pstHead;
	UINT32 uiIndex;  
	
	NF_PORTAL_WXL_SERVERIP_HASH_RD_LOCK;
	for (uiIndex = 0;uiIndex < NF_PORTAL_WXL_SERVERIP_HLIST_SIZE; uiIndex++)
	{
		pstHead = &g_astWxlServerHashTable[uiIndex];
		hlist_for_each_entry_safe(pstEntry,pstTmp,pstTmp2,pstHead,stWxlServerIpHlist)
		{
			NF_PORTAL_WXL_SERVERIP_HASH_RD_UNLOCK;
			nf_portal_wxl_serverip_del(pstEntry);
			NF_PORTAL_WXL_SERVERIP_HASH_RD_LOCK;
		}
	}
	NF_PORTAL_WXL_SERVERIP_HASH_RD_UNLOCK; 
	return AP_E_NONE;
}

UINT32 nf_portal_wxl_serverip_judge(UINT32 uiIp)
{
	NF_PORTAL_WXL_SERVERIP_ENTRY_T *pstEntry = NULL;
	pstEntry = nf_portal_wxl_serverip_get(uiIp);
	if (pstEntry != NULL)
	{
		NF_PORTAL_LOG(NF_PORTAL_LOG_DEBUG,"uiIp: %u wxl serverip list\n", uiIp);
		return NF_PORTAL_IS_WXL_SERVER_IP;
	}
	return NF_PORTAL_ISNOT_WXL_SERVER_IP;
}
/*end: add@2015.01.31 for wechat connect wifi*/

AP_ERROR_CODE_E nf_portal_hash_destroy(VOID)
{
    nf_portal_flush_sta_htbale();
    nf_portal_dest_ip_baw_flush(BAW);
    nf_portal_dest_net_baw_flush(BAW);
    nf_portal_free_auth_port_flush();
    nf_portal_mac_baw_flush(BAW);
    nf_portal_user_ip_baw_flush(BAW);
    nf_portal_url_baw_flush(BAW);
    nf_portal_port_num_flush(IPPROTO_TCP);
    nf_portal_port_num_flush(IPPROTO_UDP);
	/*add@2015.01.31 for wechat connect wifi*/
	nf_portal_wxl_serverip_flush();
    return AP_E_NONE;
}
/*****************************************************************************
 函 数 名  : nf_portal_hash_init
 功能描述  : hash链表表头初始化
 输入参数  : VOID  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年8月12日
    作    者   : WeiHonggang
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E nf_portal_hash_init(VOID)
{
    UINT32 uiIndex;
    
    for (uiIndex = 0; uiIndex < NF_PORTAL_STA_HASH_SIZE; uiIndex++)
    {
        INIT_HLIST_HEAD(&g_astStaHashTable[uiIndex]);
    }
    
    for (uiIndex = 0; uiIndex < NF_PORTAL_DEST_IP_BAW_HLIST_SIZE; uiIndex++)
    {
        INIT_HLIST_HEAD(&g_astDestIpBAWHashTable[uiIndex]);
    }
    
    for (uiIndex = 0; uiIndex < NF_PORTAL_PORT_HLIST_SIZE; uiIndex++)
    {
        INIT_HLIST_HEAD(&g_astPortHashTable[uiIndex]);
    }
    
    for (uiIndex = 0; uiIndex < NF_PORTAL_MAC_BAW_HLIST_SIZE; uiIndex++)
    {
        INIT_HLIST_HEAD(&g_astMacBAWHashTable[uiIndex]);
    }
    
    for (uiIndex = 0; uiIndex < NF_PORTAL_USER_IP_BAW_HLIST_SIZE; uiIndex++)
    {
        INIT_HLIST_HEAD(&g_astUserIpBAWHashTable[uiIndex]);
    }
    for (uiIndex = 0; uiIndex < NF_PORTAL_PORT_NUM_HLIST_SIZE; uiIndex++)
    {
        INIT_HLIST_HEAD(&g_astPortNumHashTable[uiIndex]);
    }

    for (uiIndex = 0; uiIndex < NF_PORTAL_URL_IP_BAW_HLIST_SIZE; uiIndex++)
    {
        INIT_HLIST_HEAD(&g_astUrlIpBAWHashTable[uiIndex]);
    }
	
    /*begin: add@2015.01.31 for wechat connect wifi*/
	for (uiIndex = 0; uiIndex < NF_PORTAL_WXL_SERVERIP_HLIST_SIZE; uiIndex++)
    {
        INIT_HLIST_HEAD(&g_astWxlServerHashTable[uiIndex]);
    }
	/*end: add@2015.01.31 for wechat connect wifi*/
    return AP_E_NONE;
}

#ifdef HAVE_MULTI_WAN
AP_ERROR_CODE_E nf_portal_wan_conf_set(UINT32 uiWanIndex, UINT32 uiEnable, UINT32 uiBand)
{
	if (uiWanIndex < 1 || uiWanIndex > NF_PORTAL_WAN_MAX_NUM)
	{
		NF_PORTAL_LOG(NF_PORTAL_LOG_ERR,"ERROR wan index: %u\n", uiWanIndex);
		return AP_E_PARAM;
	}
	g_astWanCfg[uiWanIndex - 1].uiWanIndex = uiWanIndex;
	g_astWanCfg[uiWanIndex - 1].uiEnable = uiEnable;
	g_astWanCfg[uiWanIndex - 1].uiWanMaxBand = uiBand;

	/*wan flag rebuild*/
	nf_portal_wanflag_init();
	nf_portal_wanflag_build();
	
	return AP_E_NONE;
}

AP_ERROR_CODE_E nf_portal_wan_map_get(NF_PORTAL_IOCTL_WAN_MAP_T *pstIoWanMap)
{
	UINT32 uiLoop = 0;
	UINT32 uiNum = 0;
	if (unlikely(NULL == pstIoWanMap))
    {
        return AP_E_PARAM;
    }
	for (uiLoop = 0; uiLoop < NF_PORTAL_WAN_MAP_SIZE; uiLoop++)
	{
		if (uiLoop < pstIoWanMap->uiStartNum)
		{
			uiLoop++;
			continue;
		}
		if (uiNum >= NF_PORTAL_WAN_MAP_LIST_MAX)
		{
			break;
		}
		pstIoWanMap->auiWanMap[uiLoop] = g_astWanFlag[uiLoop].uiWanIndex;
		uiNum++;
	}
	pstIoWanMap->uiNum = uiNum;
	pstIoWanMap->uiStartNum = uiLoop;
	return AP_E_NONE;
}

AP_ERROR_CODE_E nf_portal_wan_conf_get(NF_PORTAL_IOCTL_WAN_LIST_T *pstIoWanList)
{
	UINT32 uiLoop = 0;
	if (unlikely(NULL == pstIoWanList))
    {
        return AP_E_PARAM;
    }
	pstIoWanList->uiNum = NF_PORTAL_WAN_MAX_NUM;
	for (uiLoop = 0; uiLoop < NF_PORTAL_WAN_MAX_NUM; uiLoop++)
	{
		pstIoWanList->astWanConf[uiLoop].uiWanIndex = g_astWanCfg[uiLoop].uiWanIndex;
		pstIoWanList->astWanConf[uiLoop].uiEnable = g_astWanCfg[uiLoop].uiEnable;
		pstIoWanList->astWanConf[uiLoop].uiBand = g_astWanCfg[uiLoop].uiWanMaxBand;
		pstIoWanList->astWanConf[uiLoop].uiFlagNum = g_astWanCfg[uiLoop].uiFlagNum;
	}
	return AP_E_NONE;
}
#endif

