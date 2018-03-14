/*******************************************************************************
* COPYRIGHT GBCOM
********************************************************************************
* 文件内容: 无线驱动API
* 修改历史: 
* 
* 
*                       
*
*******************************************************************************/

#ifndef _GBCOM_WIFI_PAL_H_
#define _GBCOM_WIFI_PAL_H_

/*
 * Public Interface for GBCOM wifi API Layer
 */


/******************************** 宏和常量定义 ********************************/

/* 打印级别控制 */
#define DBG_LVL_DEBUG       7   /* 调试级：调试级别的信息 */
#define DBG_LVL_INFO        6   /* 通知级：提示信息，如驱动程序启动时，打印硬件信息 */
#define DBG_LVL_NOTICE      5   /* 注意级：正常但又重要的条件，用于提醒 */
#define DBG_LVL_WARNING     4   /* 告警级：警告条件，对可能出现问题的情况进行警告 */
#define DBG_LVL_ERROR       3   /* 错误级：错误条件，驱动程序常用KERN_ERR来报告硬件错误 */
#define DBG_LVL_CRIT        2   /* 临界级：临界条件，通常涉及严重的硬件或软件操作失败 */
#define DBG_LVL_ALERT       1   /* 警戒级：报告消息，表示必须采取措施 */
#define DBG_LVL_EMERG       0   /* 致命级：紧急事件消息，系统崩溃之前提示，表示系统不可用 */

#define DBG_LVL_DEFAULT_DEFINE     DBG_LVL_NOTICE

#define DEVNAME_SIZE 16     /* len of interface name(dev->name) */

/* SSID数据结构 */
#define  IEEE80211_SSID_LEN  32             /* max len of ssid */

#define WIFI_API_WEP_KEY_LEN_MAX	    16  /* 128 bits (for now) */

/* 创建VAP时会有些特殊要求，使用flag位表示,复制自ieee80211_defines.h */
#define IEEE80211_CLONE_BSSID           0x0001  /* allocate unique mac/bssid */
#define IEEE80211_CLONE_NOBEACONS       0x0002  /* don't setup beacon timers */
#define IEEE80211_CLONE_WDS             0x0004  /* enable WDS processing */
#define IEEE80211_CLONE_WDSLEGACY       0x0008  /* legacy WDS operation */
#define IEEE80211_PRIMARY_VAP           0x0010  /* primary vap */
#define IEEE80211_P2PDEV_VAP            0x0020  /* p2pdev vap */
#define IEEE80211_P2PGO_VAP             0x0040  /* p2p-go vap */
#define IEEE80211_P2PCLI_VAP            0x0080  /* p2p-client vap */
#define IEEE80211_CLONE_MACADDR         0x0100  /* create vap w/ specified mac/bssid */
#define IEEE80211_CLONE_MATADDR         0x0200  /* create vap w/ specified MAT addr */
#define IEEE80211_WRAP_VAP              0x0400  /* wireless repeater ap vap */


/* 各种参数配置极值 */
#define WIFI_API_FRAG_MAX       2346
#define WIFI_API_FRAG_MIN       256
#define WIFI_API_BINTVAL_MAX    3500
#define WIFI_API_BINTVAL_MIN    40
#define WIFI_API_DTIM_MAX       255
#define WIFI_API_DTIM_MIN       1
#define WIFI_API_AID_DEF        128
#define WIFI_API_CWMAX_MAX      15
#define WIFI_API_CWMIN_MAX      15
#define WIFI_API_AIFS_MAX       15
#define WIFI_API_TXOPLIMIT_MAX  8192
#define WIFI_API_RSSI_MAX       95

/* WMM优先级定义 */
#define WIFI_API_WME_AC_BE  0
#define WIFI_API_WME_AC_BK  1
#define WIFI_API_WME_AC_VI  2    
#define WIFI_API_WME_AC_VO  3

#define WIFI_API_WEP_ASCII  0
#define WIFI_API_WEP_HEX    1

/* BEGIN: Added by wenlei, 2014/5/5   PN:移植获取卡设备能力接口，获取最大功率值 */
/*支持的频段*/
#define	WIFI_API_CAP_FREQ_2G     0x00000001   //bit0：2.4GHz
#define	WIFI_API_CAP_FREQ_5G     0x00000002   //bit1：5GHz

/*支持的MIMO*/
#define	WIFI_API_CAP_MIMO_NONE     0   //0：不支持
#define	WIFI_API_CAP_MIMO_11     1   //1：1×1
#define	WIFI_API_CAP_MIMO_22     2   //2：2×2
#define	WIFI_API_CAP_MIMO_33     3   //3：3×3
/* END:   Added by wenlei, 2014/5/5   PN:移植获取卡设备能力接口，获取最大功率值 */


/******************************** 类型定义 ************************************/

/* NOTE:应当与gbcom_wifi_ioctl.h中的定义保持一致 */
typedef enum{
    WIFI_API_E_SUCCESS              =  0,       /* success */

    WIFI_API_E_DEFAULT              = 1000,     /* unknow error */
    WIFI_API_E_NULL_POINTER         = 1001,     /* NULL pointer */
    WIFI_API_E_INVALID_INTERFACE    = 1002,     /* invalid interface name */
    WIFI_API_E_INVALID_PARAM        = 1003,     /* parameter value out of range*/
    WIFI_API_E_NOT_SUPPORT          = 1004,     /* API not supported */
    WIFI_API_E_BUFSIZE_DIFF         = 1005,     /* buffer size different from user space*/
    WIFI_API_E_MALLOC_FAILED        = 1006,     /* call kmalloc failed */
    WIFI_API_E_IV_OPMODE_WRONG      = 1007,     /* iv_opmode wrong */
    WIFI_API_E_ARRARY_INDEX_WRONG   = 1008,     /* index of arrary is out of max*/
    WIFI_API_E_COPYTOUSER_FAIL      = 1009,
    WIFI_API_E_COPYFROMUSER_FAIL    = 1010,
    WIFI_API_E_STATION_NOT_EXIST    = 1011,     /* station not exist, ni is NULL */
    WIFI_API_E_IEEE80211_CHAN_AYNC  = 1012,     /* channel is ANYC */
    WIFI_API_E_VAP_DOWN             = 1013,     /* vap is not up and not running */
    WIFI_API_E_SOCKET               = 1014,     /* create socket error */
    WIFI_API_E_SYSTEMFAIL           = 1015,     /* 系统调用失败 */
    
}WIFI_API_ERRNO_E;


typedef struct {
    int             len;                        /* strlen of ssid */
    unsigned char   ssid[IEEE80211_SSID_LEN];   /* ssid buffer */
}WIFI_API_SSID_T;


typedef struct {
	WIFI_API_SSID_T ssid ;
	unsigned char   bssid[6] ;
	unsigned char   channel ;
	unsigned char   rssi ;
	int noise ;
}WIFI_API_AP_INFO_T ;

#define WIFI_API_AP_LIST_MAX	64

/* 应与NEIGHBOR_AP_LIST_T保持一致 */
typedef struct{
	unsigned int    ulWifiId;
	unsigned char   aucMac[6];		/*本AP MAC*/
	unsigned char   aucMacPad[2];
	unsigned int    ulChannel;		/*本AP CHANNEL*/
	unsigned int    ulNeighborApNum ;
	WIFI_API_AP_INFO_T stNeighborApList[WIFI_API_AP_LIST_MAX] ;
}WIFI_API_AP_LIST_T ;

/* 应与BGSCAN_RESULT_T保持一致 */
typedef struct {
    u_int32_t ulChanNum;                /*信道数量*/
    u_int32_t ulCurrentChan;            /*当前信道号*/
    u_int32_t ulChan[32];	            /*信道号*/
	u_int32_t ulChanApNum[32];	        /*信道中AP数目(大于RSSI门限的)*/
	u_int32_t ulChanAverageRssi[32];    /*信道中AP的RSSI*/
    int32_t   lChanNoise;               /*当前信道噪声*/
    u_int32_t ulInterference;
} WIFI_API_SCAN_RESULT_T ;

typedef enum{      
    WIFI_API_STATUS_DISABLE    =  0 ,      /* 功能关闭 */
    WIFI_API_STATUS_ENABLE     =  1 ,      /* 功能开启 */
}WIFI_API_STATUS_E;
/* BEGIN: Added by WeiHonggang, 2014/12/31   PN:TRAC#1056 非法AP检测和干预 */
typedef enum
{
    ROGUE_AP_CMD_SW_GET = 0, /*功能开关*/
    ROGUE_AP_CMD_SW_SET,
    
    ROGUE_AP_CMD_RULE_GET, /*非法AP检测策略*/
    ROGUE_AP_CMD_RULE_SET,
    
    ROGUE_AP_CMD_ACTION_GET,/*对非法AP的处理方式*/
    ROGUE_AP_CMD_ACTION_SET,
    
    ROGUE_AP_CMD_SSID_ADD, /*合法SSID配置*/
    ROGUE_AP_CMD_SSID_DEL,
    ROGUE_AP_CMD_SSID_SHOW,
    ROGUE_AP_CMD_SSID_FLUSH,
    
    ROGUE_AP_CMD_BSSID_ADD, /*合法BSSID配置*/
    ROGUE_AP_CMD_BSSID_DEL,
    ROGUE_AP_CMD_BSSID_SHOW,
    ROGUE_AP_CMD_BSSID_FLUSH,
    
    ROGUE_AP_CMD_OUI_ADD, /*合法OUI配置*/
    ROGUE_AP_CMD_OUI_DEL,
    ROGUE_AP_CMD_OUI_SHOW,
    ROGUE_AP_CMD_OUI_FLUSH,
    
    ROGUE_AP_CMD_SHOW, /*非法AP查看*/
    
    ROGUE_AP_CMD_INTERFERE_ADD ,/*干预AP配置*/
    ROGUE_AP_CMD_INTERFERE_DEL,
    ROGUE_AP_CMD_INTERFERE_SHOW,
    ROGUE_AP_CMD_INTERFERE_FLUSH,

    ROGUE_AP_CMD_DEBUG_SET,
    ROGUE_AP_CMD_DEBUG_GET,
}ROGUE_AP_CMD_E;
typedef enum{
    RULE_SSID = 0,
    RULE_BSSID,
    RULE_OUI,
    RULE_ALL,
    RULE_MAX,
}ROGUE_AP_RULE_E;
typedef enum
{
    SW_OFF = 0,
    SW_ON,
}ROGUE_AP_SW_E;
typedef enum
{
    ACTION_WARN = 0,
    ACTION_INTERFERE,
    ACTION_INTERFERE_AUTO,
    ACTION_MAX,
}ROGUE_AP_ACTION_E;

#ifndef IEEE80211_OUI_LEN
#define IEEE80211_OUI_LEN 3
#endif
#ifndef IEEE80211_ADDR_LEN
#define IEEE80211_ADDR_LEN 6
#endif
#ifndef IEEE80211_SSID_LEN
#define IEEE80211_SSID_LEN 32
#endif

#define NODE_NUM_EACH_TIME 32
typedef union
{
    u_int32_t uiDebugLevel;
    u_int32_t uiSw;
    u_int32_t uiRule;
    u_int32_t uiAction;
    u_int8_t aucSsid[IEEE80211_SSID_LEN];
    u_int8_t aucBssid[IEEE80211_ADDR_LEN];
    u_int8_t aucOui[IEEE80211_OUI_LEN];
    u_int8_t *point;
}ROGUE_AP_DATA_U;
typedef struct
{
    ROGUE_AP_CMD_E enCmd;
    ROGUE_AP_DATA_U unData;
}ROGUE_AP_PARAM_T;
typedef struct
{
    u_int8_t aucSsid[IEEE80211_SSID_LEN];
}ROGUE_AP_SSID_NODE_T;
typedef struct
{
    u_int32_t uiStartNum;
    u_int32_t uiNum;
    ROGUE_AP_SSID_NODE_T astNode[NODE_NUM_EACH_TIME];
}ROGUE_AP_SSID_LIST_T;

typedef struct
{
    u_int8_t aucBssid[IEEE80211_ADDR_LEN];
}ROGUE_AP_BSSID_NODE_T;

typedef struct
{
    u_int32_t uiStartNum;
    u_int32_t uiNum;
    ROGUE_AP_BSSID_NODE_T astNode[NODE_NUM_EACH_TIME];
}ROGUE_AP_BSSID_LIST_T;
typedef struct
{
    u_int8_t aucOui[IEEE80211_OUI_LEN];
}ROGUE_AP_OUI_NODE_T;
typedef struct
{
    u_int32_t uiStartNum;
    u_int32_t uiNum;
    ROGUE_AP_OUI_NODE_T astNode[NODE_NUM_EACH_TIME];
}ROGUE_AP_OUI_LIST_T;
typedef struct
{
    u_int8_t bssid[IEEE80211_ADDR_LEN];
    u_int8_t ssid[IEEE80211_SSID_LEN];
    u_int8_t channel;
}ROGUE_AP_NODE_INFO_T;
typedef struct
{
    unsigned int uiStartNum;
    unsigned int uiNum;
    ROGUE_AP_NODE_INFO_T astNode[NODE_NUM_EACH_TIME];
}ROGUE_AP_LIST_T;
/* END:   Added by WeiHonggang, 2014/12/31   PN:TRAC#1056 非法AP检测和干预 */
/* BEGIN: Added by WeiHonggang, 2015/1/12   PN:TRAC#1085内核扫描定时器归一化整理 */
typedef enum
{
      SCAN_TIMER_CMD_DEBUG_SET = 0,
      SCAN_TIMER_CMD_DEBUG_GET,
}SCAN_TIMER_CMD_E;
typedef union
{
    u_int32_t uiDebugLevel;
}SCAN_TIMER_DATA_U;
typedef struct
{
    SCAN_TIMER_CMD_E enCmd;
    SCAN_TIMER_DATA_U unData;
}SCAN_TIMER_PARAM_T;
/* END:   Added by WeiHonggang, 2015/1/12   PN:TRAC#1085内核扫描定时器归一化整理 */
/* vap工作模式 */
typedef enum{
    WIFI_API_OPMODE_STA     = 0,    /* infrastructure station */
    WIFI_API_OPMODE_HOSTAP  = 1,    /* Software Access Point */
    WIFI_API_OPMODE_IBSS    = 2,    /* IBSS (adhoc) station */
    WIFI_API_OPMODE_MONITOR = 3,    /* Monitor mode */
    WIFI_API_OPMODE_WRAP    = 4,    /* Wireless Repeater AP */
    WIFI_API_OPMODE_P2PGO   = 5,    /* P2P Group Owner */
    WIFI_API_OPMODE_P2PCLI  = 6,    /* P2P client */
    WIFI_API_OPMODE_P2PDEV  = 7,    /* P2P device */

    WIFI_API_OPMODE_MAX,            /* Highest numbered opmode in the list */
}WIFI_API_OPMODE_E;


typedef enum{
    WIFI_API_PHYMODE_AUTO           = 0,    /*  */
    WIFI_API_PHYMODE_11A            = 1,    /*  */
    WIFI_API_PHYMODE_11B            = 2,    /*  */
    WIFI_API_PHYMODE_11G            = 3,    /*  */
    WIFI_API_PHYMODE_11NGHT20       = 4,    /*  */
    WIFI_API_PHYMODE_11NGHT40       = 5,    /*  */
    WIFI_API_PHYMODE_11NGHT40PLUS   = 6,    /*  */
    WIFI_API_PHYMODE_11NGHT40MINUS  = 7,    /*  */
    WIFI_API_PHYMODE_11NAHT20       = 8,    /*  */
    WIFI_API_PHYMODE_11NAHT40       = 9,    /*  */
    WIFI_API_PHYMODE_11NAHT40PLUS   = 10,   /*  */
    WIFI_API_PHYMODE_11NAHT40MINUS  = 11,   /*  */



    WIFI_API_PHYMODE_MAX,
}WIFI_API_PHYMODE_E;


typedef enum{
    /* vap级参数 */
    WIFI_API_PARAMTYPE_HIDESSID         = 0,    /* 0-禁用，1-启用 */
    WIFI_API_PARAMTYPE_RTS_THRESHOLD    = 1,    /* 0-2347，0或2347为关闭 */
    WIFI_API_PARAMTYPE_FRAG_THRESHOLD   = 2,    /* 256-2346，2346为关闭 */
    WIFI_API_PARAMTYPE_SHORTGI          = 3,    /* 0-禁用，1-启用 */
    WIFI_API_PARAMTYPE_RTS_CTS          = 4,    /* 0-none，1-cts only，2-rts/cts */
    WIFI_API_PARAMTYPE_BINTVAL          = 5,    /* 卡级通用配置: 40-3500 ms */
    WIFI_API_PARAMTYPE_DTIM             = 6,    /* beacon的倍数，取值为1-255 */
    WIFI_API_PARAMTYPE_WDS              = 7,    /* 0-禁用，1-启用 */
    WIFI_API_PARAMTYPE_MAX_AID          = 8,    /* 1-128,AID最大值，即vap最大可关联的用户数 */
    WIFI_API_PARAMTYPE_INACT            = 9,    /* 应大于2倍IEEE80211_INACT_WAIT(1秒或15秒)，小于65535，建议大于60秒 */
    WIFI_API_PARAMTYPE_LEGACYRATE       = 10,   /* 位图:自bit0起，分别表示1、2、5.5、11、6、9、12、18、24、36、48、54M */
    WIFI_API_PARAMTYPE_HTRATE           = 11,   /* 位图:bit0表示MCS0，bit1表示MCS1，依此类推 */
    WIFI_API_PARAMTYPE_WMM_IE           = 12,   /* 0-禁用，1-启用 */
    WIFI_API_PARAMTYPE_COUNTRY_IE       = 13,   /* 卡级通用配置: 0-禁用，1-启用 */
    //WIFI_API_PARAMTYPE_RSSI_LIMIT       = 14,   /* 终端接入信号强度，实际应为信噪比，正值 */
    WIFI_API_PARAMTYPE_PUREN            = 15,   /* 0-禁用，1-启用，仅11n，拒绝11ag用户接入 */
    WIFI_API_PARAMTYPE_PUREG            = 16,   /* 0-禁用，1-启用，仅11g，拒绝11b用户接入 */
    WIFI_API_PARAMTYPE_DISABLE_COEXT    = 17,   /* 0-禁用，1-启用，强制40M */
    WIFI_API_PARAMTYPE_SHORT_PREAMBLE   = 18,   /* 短前导:0-禁用，1-启用 */
    WIFI_API_PARAMTYPE_AP_BRIDGE        = 19,   /* AP桥，0-禁用，1-启用 */
    WIFI_API_PARAMTYPE_AUTHMODE         = 20,   /* 鉴权模式: */
    WIFI_API_PARAMTYPE_CLEAR_APPOPTIE   = 21,   /*  */
    WIFI_API_PARAMTYPE_PRIVCY           = 22,   /*  */
    WIFI_API_PARAMTYPE_DROPUNENCRYPTED  = 23,   /*  */
    /* BEGIN: Added by MaYuHua, 2013/8/6   PN:支持动态RSSI功能 */
    //WIFI_API_PARAMTYPE_DYNAMIC_RSSI_LIMIT = 24,   /* 终端接入信号强度，实际应为信噪比，正值 */
    /* END:   Added by MaYuHua, 2013/8/6   PN:支持动态RSSI功能 */
    //WIFI_API_PARAMTYPE_RSSI_DELSTA_SWITCH = 25,

    /* wifi级参数 */
    WIFI_API_ATHPARAMTYPE_AMPDU         = 100,  /* 0-禁用，1-启用 */
    WIFI_API_ATHPARAMTYPE_AMSDU         = 101,  /* 0-禁用，1-启用 */
    WIFI_API_ATHPARAMTYPE_TX_CHAINMASK  = 102,  /* 位图:bit0表示ch0，bit1表示ch1，bit2表示ch2 */
    WIFI_API_ATHPARAMTYPE_RX_CHAINMASK  = 103,  /* 位图:bit0表示ch0，bit1表示ch1，bit2表示ch2 */
    WIFI_API_ATHPARAMTYPE_COUNTRYID     = 104,  /* 国家码，根据ISO标准定义，自定义241全频道为特例 */
    WIFI_API_ATHPARAMTYPE_MAXTXPOWLIMIT = 105,  /* 最大功率限制,单位为1dbm */
    WIFI_API_ATHPARAMTYPE_MGTRATE       = 106,  /* 管理帧速率，包括beacon，单位为kbps，1000表示1M */
    WIFI_API_ATHPARAMTYPE_MCASTRATE     = 107,  /* 广播帧速率，包括beacon，单位为kbps，1000表示1M */
    WIFI_API_ATHPARAMTYPE_LEGACYRATE    = 108,  /* 位图:自bit0起，分别表示1、2、5.5、11、6、9、12、18、24、36、48、54M */
    WIFI_API_ATHPARAMTYPE_HTRATE        = 109,  /* 位图:bit0表示MCS0，bit1表示MCS1，依此类推 */
    WIFI_API_ATHPARAMTYPE_ACKTIMEOUT    = 110,  /* 单位:usec */
	WIFI_API_ATHPARAMTYPE_RECEIVESENSITIVITY_OPT	= 111,	/* 接收灵敏度优化开关*/


    WIFI_API_PARAMTYPE_MAX,
}WIFI_API_PARAMTYPE_E;



typedef enum{
    WIFI_API_WMM_CWMIN      = 0,    /* 竞争窗口最小值，取值范围0-15 */
    WIFI_API_WMM_CWMAX      = 1,    /* 竞争窗口最大值，取值范围0-15 */
    WIFI_API_WMM_AIFS       = 2,    /* 仲裁帧间隔数，取值范围0-15 */
    WIFI_API_WMM_TXOPLIMIT  = 3,    /* 发送机会最大值，取值范围0-8192 */
    WIFI_API_WMM_ACM        = 4,    /* ACM，取值为0或1,bss only */
}WIFI_API_WMM_PARAM_E;

/* WMM参数，修改整个优先级 */
typedef struct{
    unsigned char    ucAc;           /* Acces Catagory,WMM的优先级分类，0-BE,1-BK,2-VI,3-VO */
    unsigned char    ucBssAcm;       /* ACM，取值为0或1,bss only */
    unsigned char    ucBssAifsn;     /* 发送给STA:仲裁帧间隔数，取值范围0-15 */
    unsigned char    ucBssCwMax;     /* 发送给STA:竞争窗口最大值，取值范围0-15 */
    unsigned char    ucBssCwMin;     /* 发送给STA:竞争窗口最小值，取值范围0-15 */
    unsigned short   usBssTxopLimit; /* 发送给STA:发送机会最大值，取值范围0-8192 */
    unsigned char    ucChanAifsn;    /* AP自用:仲裁帧间隔数，取值范围0-15 */
    unsigned char    ucChanCwMax;    /* AP自用:竞争窗口最大值，取值范围0-15 */
    unsigned char    ucChanCwMin;    /* AP自用:竞争窗口最小值，取值范围0-15 */
    unsigned short   usChanTxopLimit;/* AP自用:发送机会最大值，取值范围0-8192 */
}WIFI_API_WMM_AC_PARAM_T;


/* 统计中所有的TX均表示AP的发送，RX均表示AP的接收 */

/* 无线vap级统计 */
typedef struct{
    unsigned char       ucChannel;              /* ap工作信道 */
    unsigned char       ucStationNum;           /* 当前vap下关联的终端数 */
    unsigned short      usResv;                 /* 无实际含义，保持4字节对齐 */
    
    unsigned char       aucSsid[IEEE80211_SSID_LEN];    /* essid */
    
    unsigned long       ulTxUcastDataFrame;     /* 发送单播数据帧数 */
    unsigned long       ulRxUcastDataFrame;     /* 接收单播数据帧数 */
    unsigned long       ulTxMcastDataFrame;     /* 发送广播数据帧数 */
    unsigned long       ulRxMcastDataFrame;     /* 接收广播数据帧数 */
    unsigned long       ulTxDataFrame;          /* 发送数据帧数，包含广播单播 */
    unsigned long       ulRxDataFrame;          /* 接收数据帧数，包含广播单播 */
    unsigned long long  ullTxDataBytes;         /* 发送数据字节数 */
    unsigned long long  ullRxDataBytes;         /* 接收数据字节数 */

    unsigned long       ulTxErrorTotal;         /* 发送错误帧数 */
    unsigned long       ulRxErrorTotal;         /* 接收错误帧数 */

}WIFI_API_BSS_AIR_STATS_T;

/* 管理帧收发统计 */
typedef struct{
    unsigned char   aucSsid[IEEE80211_SSID_LEN];
    
    unsigned long   ulTxMgtFrameTotal;  /*  */
    unsigned long   ulRxMgtFrameTotal;  /*  */
    unsigned long   ulTxBeacon;         /*  */
    unsigned long   ulRxBeacon;         /*  */
    unsigned long   ulTxProbeReq;       /*  */
    unsigned long   ulRxProbeReq;       /*  */
    unsigned long   ulTxProbeResp;      /*  */
    unsigned long   ulRxProbeResp;      /*  */
    unsigned long   ulTxAuth;           /*  */
    unsigned long   ulRxAuth;           /*  */
    unsigned long   ulTxAssocReq;       /*  */
    unsigned long   ulRxAssocReq;       /*  */
    unsigned long   ulTxAssocResp;      /*  */
    unsigned long   ulRxAssocResp;      /*  */
    unsigned long   ulTxReAssocReq;     /*  */
    unsigned long   ulRxReAssocReq;     /*  */
    unsigned long   ulTxReAssocResp;    /*  */
    unsigned long   ulRxReAssocResp;    /*  */ 
    unsigned long   ulTxAction;         /*  */
    unsigned long   ulRxAction;         /*  */
    unsigned long   ulTxDeauth;         /*  */
    unsigned long   ulRxDeauth;         /*  */
    unsigned long   ulTxDisassoc;       /*  */
    unsigned long   ulRxDisassoc;       /*  */

}WIFI_API_BSS_MGT_STATS_T;

typedef struct{
    unsigned long   ulRxAuthTotal;      /*  */
    unsigned long   ulRxAuthSucc;       /*  */
    unsigned long   ulRxAuthFail;       /*  */
    
    unsigned long   ulTxAuthTotal;      /*  */
    unsigned long   ulTxAuthSucc;       /*  */
    unsigned long   ulTxAuthFail;       /*  */
    
    unsigned long   ulRxAssocTotal;     /*  */
    unsigned long   ulRxAssocSucc;      /*  */
    unsigned long   ulRxAssocFail;      /*  */
    
    unsigned long   ulTxAssocTotal;     /*  */
    unsigned long   ulTxAssocSucc;      /*  */
    unsigned long   ulTxAssocFail;      /*  */
    
    unsigned long   ulRxReAssocTotal;   /*  */
    unsigned long   ulRxReAssocSucc;    /*  */
    unsigned long   ulRxReAssocFail;    /*  */
    
    unsigned long   ulTxReAssocTotal;   /*  */
    unsigned long   ulTxReAssocSucc;    /*  */
    unsigned long   ulTxReAssocFail;    /*  */
    
    unsigned long   ulTxDeauthTotal;    /*  */
    unsigned long   ulRxDeauthTotal;    /*  */
    
    unsigned long   ulTxDisassocTotal;  /*  */
    unsigned long   ulRxDisassocTotal;  /*  */

	/* BEGIN: Added by wenlei, 2014/10/24   PN:增加用户关联记录信息 */
	unsigned long	ulRxAuthError;
	unsigned long	ulRxAuthBadAlgo;
	unsigned long	ulRxAuthAcl;
	unsigned long	ulRxAuth11aPrior;
	unsigned long	ulRxAuthCounterMeasures;
	unsigned long	ulRxAuthStaTooMany;
       unsigned long ulRxAuthSeq;
       unsigned long ulRxAuthChallenge;
       unsigned long ulRxAuthUnspecified;
	unsigned long	ulRxAuthUnsupported;

       unsigned long ulTxDeauthLeave;
       unsigned long ulTxDeauthNotAuthed;
       unsigned long ulTxDeauthAssocNotAuthed;
       unsigned long ulTxDeauthRsnRequired;
       unsigned long ulTxDeauthNotAssoced;
       unsigned long ulTxDeauthExpire;
       unsigned long ulTxDeauthOthers;

       unsigned long ulRxDeauthLeave;
       unsigned long ulRxDeauthNotAuthed;
       unsigned long ulRxDeauthAssocNotAuthed;
       unsigned long ulRxDeauthRsnRequired;
       unsigned long ulRxDeauthNotAssoced;
       unsigned long ulRxDeauthExpire;
       unsigned long ulRxDeauthOthers;

       unsigned long ulTxDisassocNotAssoced;
       unsigned long ulTxDisassocLeave;
       unsigned long ulTxDisassocRssiTooLow;
       unsigned long ulTxDisassocOthers;
       
       unsigned long ulRxDisassocNotAssoced;
       unsigned long ulRxDisassocLeave;
       unsigned long ulRxDisassocOthers;

	unsigned long	ulRxAssocBss;
	unsigned long	ulRxAssocBadlen;
	unsigned long	ulRxAssocRsnRequired;
	unsigned long	ulRxAssocNotAuth;
	unsigned long	ulRxAssocBadWpaIe;
	unsigned long	ulRxAssocCapMismatch;
	unsigned long	ulRxAssocRateMismatch;
	unsigned long	ulRxAssocPuren;
	unsigned long	ulRxAssoc11NPrior;
	unsigned long	ulRxAssocRssiLow;
    
	unsigned long	ulKickNotAssoced;
	unsigned long	ulKickIoctl;
	unsigned long	ulKickVapStop;
	unsigned long	ulKickRssiLow;
	unsigned long	ulKickPsPoll;
	unsigned long	ulKickInactTimeout;
	unsigned long	ulKickRatectrl;
	unsigned long	ulKickBeaconDisable;
	/* END:   Added by wenlei, 2014/10/24   PN:增加用户关联记录信息 */
	
}WIFI_API_BSS_ASSOC_STATS_T;

typedef struct{
    unsigned long   ulRxErrTotal;       /*  */
    unsigned long   ulRxPhyErr;         /*  */
    unsigned long   ulRxCrcErr;         /*  */
    unsigned long   ulRxFifoErr;        /*  */
    unsigned long   ulRxDecryptErr;     /*  */
    unsigned long   ulRxDemicErr;       /*  */
    unsigned long   ulRxDecryptBusy;    /*  */

}WIFI_API_BSS_PHY_STATS_T;

typedef enum{
    BSS_AIR_STATS   =   0,
    BSS_MGT_STATS   =   1,
    BSS_ASSOC_STATS =   2,
    BSS_PHY_STATS   =   3,
}WIFI_API_BSS_STATS_TYPE_E;

typedef struct{
    WIFI_API_BSS_STATS_TYPE_E   enStatsType;
    
    union{
    WIFI_API_BSS_AIR_STATS_T    stBssAirStats;      /*  */
    WIFI_API_BSS_MGT_STATS_T    stBssMgtStats;      /*  */
    WIFI_API_BSS_ASSOC_STATS_T  stBssAssocStats;    /*  */
    WIFI_API_BSS_PHY_STATS_T    stBssPhyStats;      /*  */
    }unBssStats;
}WIFI_API_BSS_STATS_T;

typedef struct{
    unsigned short      usStaIndex;         /* 编号，无实际意义，用于计数，从0开始 */
    unsigned char       aucStaMac[6];       /* sta的mac地址 */
    
    unsigned long       ulStaLiveTime;      /* 单位:10毫秒，MIB节点类型要求为TIMETICKS，该类型单位就是0.01秒 */
    unsigned long       ulNiFlags;          /* sta的各种状态位，直接复制自无线驱动 */

    unsigned char       ucStaMode;
        #define WIFI_END_USER_MODE_A    0       /*A模式*/
        #define WIFI_END_USER_MODE_B    1   	/*纯B模式*/
        #define WIFI_END_USER_MODE_BG   2   	/*BG模式*/
        #define WIFI_END_USER_MODE_NG	3		/*NG模式*/
        #define WIFI_END_USER_MODE_NA	4		/*NA模式*/
    unsigned char       ucChannel;          /* STA所处信道 */
    char                cStaRssi;           /* 终端信号强度 */
    char                cChanNoise;         /* 当前信道噪声 */

    unsigned long       ulTxAvrRate;
    unsigned long       ulRxAvrRate;
    unsigned long       ulTxLastRate;
    unsigned long       ulRxLastRate;
/* BEGIN: Added by WeiHonggang, 2014/3/19   PN:TASK#0 增加终端初始速率统计 */
    unsigned long       ulTxAssocRate; /*32位*/
    unsigned long       ulRxAssocRate;/*同上*/
/* END:   Added by WeiHonggang, 2014/3/19   PN:TASK#0 增加终端初始速率统计 */
    unsigned long       ulTxDataFrame;      /* 发送数据帧数，包括单播与广播 */
    unsigned long       ulRxDataFrame;      /* 接收数据帧数，包括单播与广播 */
    unsigned long long  ullTxDataBytes;     /* 发送数据字节数，包括单播与广播 */
    unsigned long long  ullRxDataBytes;     /* 接收数据字节数，包括单播与广播 */
    unsigned long       ulTxDataRetry;      /*  */
    unsigned long       ulRxDataRetry;      /*  */
    unsigned long       ulTxDataFrag;       /*  */
    unsigned long       ulRxDataFrag;       /*  */
    unsigned long       ulTxError;          /*  */
    unsigned long       ulRxError;          /*  */
    
    
}WIFI_API_STA_STATS_T;

typedef struct{
    unsigned short  usTotalStaNumber;      /* vap下终端总数 */
    unsigned short  usResv;
    unsigned long   ulBufLen;              /* BUFF大小，以WIFI_API_STA_STATS_T为单位 */
    WIFI_API_STA_STATS_T *pstStaStats;
}WIFI_API_ENDUSER_STATS_T;

/* BEGIN: Added by wenlei, 2014/4/18   PN:ta55接口整理，低质量用户限制接入功能 */
typedef struct{
	u_int32_t ulStaDelIntvl;
	u_int32_t ulHighRssi;
	u_int32_t ulLowRssi;
}WIFI_API_LOWRSSI_LIMIT_OLD_T;
typedef struct{
    u_int32_t uiStaDelIntvl;/*对接入终端RSSI检测时间*/
    u_int32_t uiRssiLimitDel;/*已接入终端剔除RSSI门限*/
    u_int32_t uiRssiLimit;/*终端接入RSSI门限*/
    u_int32_t uiRssiLimitProbe;/*probe response回复门限*/
    u_int32_t uiKickStaSwitch;/*是否剔除终端开关*/
    u_int32_t uiRule;/*终端接入控制策略0 --静态，1--动态*/
}WIFI_API_LOWRSSI_LIMIT_T;
typedef struct{
    u_int32_t uiStaDelIntvl;/*对接入终端RSSI检测时间*/
    u_int32_t uiRssiLimitDel;/*已接入终端剔除RSSI门限*/
    u_int32_t uiRssiLimit;/*终端接入RSSI门限*/
    u_int32_t uiRssiLimitProbe;/*probe response回复门限*/
    u_int32_t uiKickStaSwitch;/*是否剔除终端开关*/
    u_int32_t uiRule;/*终端接入控制策略0 --静态，1--动态*/
    u_int32_t uiDynamicRssiLimit;/*动态RSSI门限，根据接入用户数动态调整*/
    u_int32_t uiAssocRefused;/*拒绝次数*/   
    u_int32_t uiStaDelCount;/*剔除终端次数*/
    u_int32_t uiNoProbeResp;/*不回复probe response统计*/
}WIFI_API_LOWRSSI_LIMIT_GET_T;
/* END:   Added by wenlei, 2014/4/18   PN:ta55接口整理，低质量用户限制接入功能 */

typedef struct
{
    u_int32_t uiSwitch; /*是否剔除终端开关*/
    u_int32_t uiRateLimit;/*低阶MCS门槛值*/
    u_int32_t uiRateRange;/*浮动范围*/
    u_int32_t uiTolerateFreq;/*容忍频率*/
    u_int32_t uiAgePeriod;/*老化时间*/
}WIFI_API_LOWRATELIMIT_SET_T;

typedef WIFI_API_LOWRATELIMIT_SET_T WIFI_API_LOWRATELIMIT_GET_T;

/* BEGIN: Added by wenlei, 2014/5/5   PN:移植获取卡设备能力接口，获取最大功率值 */
/* wifi  卡的基本能力信息*/
typedef struct{
	u_int32_t	cap_freq;		/*支持的频段*/
	u_int32_t	cap_mode;		/*支持的无线模式*/
	u_int32_t	cap_mimo;		/*支持的MIMO 策略*/
	u_int32_t	cap_txpow;		/*支持的最大功率值*/
}WIFI_API_BASIC_CAP_T;
/* END:   Added by wenlei, 2014/5/5   PN:移植获取卡设备能力接口，获取最大功率值 */

#if 1 //GBCOM_ACS_EAP
/* BEGIN: Added by wenlei, 2014/7/30   PN:添加自动信道功能 */
enum
{
	WIFI_API_ACS_MODE_OFF		=	0,
	WIFI_API_ACS_MODE_SIMPLE	=	1,
	WIFI_API_ACS_MODE_TERMLY	=	2,
};

enum
{
	WIFI_API_ACS_SCAN_FOREGROUND	=	0,
	WIFI_API_ACS_SCAN_BACKGROUND	=	1,
};
enum
{
	WIFI_API_ACS_SCAN_FREQ_2G		=	0,
	WIFI_API_ACS_SCAN_FREQ_5G		=	1,
	WIFI_API_ACS_SCAN_FREQ_ALL		=	2,
};
#define WIFI_API_CHAN_MAX	255

typedef struct
{
	u_int32_t	ulMode;			/*  自动信道工作模式*/
	u_int32_t	ulScanMode;		/*  扫描模式*/
	u_int32_t	ulPeriod;		/*  自动信道定时启用周期，单位(s)*/
	u_int32_t	ul5GExtend;		/*  自动信道启用时5G  是否支持扩展频段*/
}WIFI_API_ACS_CFG_T;

typedef struct
{
	u_int32_t	ulMode;			/*  自动信道工作模式*/
	u_int32_t	ulScanMode;		/*  扫描模式*/
	u_int32_t	ulPeriod;		/*  自动信道定时启用周期，单位(s)*/
	u_int32_t	ul5GExtend;		/*  自动信道启用时5G  是否支持扩展频段*/
	u_int32_t	ulBestChan;		/*  自动信道选出的最优信道*/
	u_int32_t	ulScanFreq;		/*  自动信道扫描时扫描的频段*/
    int32_t     iChanMaxrssi[WIFI_API_CHAN_MAX];			/* 各信道最大信号强度值*/
    int32_t     iChanNoisefloor[WIFI_API_CHAN_MAX];      	/* 各信道底噪值*/
    int32_t  	iChanLoad[WIFI_API_CHAN_MAX];		 		/* 各信道空口利用率*/
}WIFI_API_ACS_STATS_T;
/* END:   Added by wenlei, 2014/7/30   PN:添加自动信道功能 */
#endif

/* BEGIN: Added by wenlei, 2014/8/12   PN:task#322 增加功率调试工具 */
#define WIFI_API_AR9300_RATES_OFDM_OFFSET    0
#define WIFI_API_AR9300_RATES_CCK_OFFSET     4
#define WIFI_API_AR9300_RATES_HT20_OFFSET    8
#define WIFI_API_AR9300_RATES_HT40_OFFSET    22
typedef enum{
    WIFI_API_ALL_TARGET_LEGACY_6_24,
    WIFI_API_ALL_TARGET_LEGACY_36,
    WIFI_API_ALL_TARGET_LEGACY_48,
    WIFI_API_ALL_TARGET_LEGACY_54,
    WIFI_API_ALL_TARGET_LEGACY_1L_5L,
    WIFI_API_ALL_TARGET_LEGACY_5S,
    WIFI_API_ALL_TARGET_LEGACY_11L,
    WIFI_API_ALL_TARGET_LEGACY_11S,
    WIFI_API_ALL_TARGET_HT20_0_8_16,
    WIFI_API_ALL_TARGET_HT20_1_3_9_11_17_19,
    WIFI_API_ALL_TARGET_HT20_4,
    WIFI_API_ALL_TARGET_HT20_5,
    WIFI_API_ALL_TARGET_HT20_6,
    WIFI_API_ALL_TARGET_HT20_7,
    WIFI_API_ALL_TARGET_HT20_12,
    WIFI_API_ALL_TARGET_HT20_13,
    WIFI_API_ALL_TARGET_HT20_14,
    WIFI_API_ALL_TARGET_HT20_15,
    WIFI_API_ALL_TARGET_HT20_20,
    WIFI_API_ALL_TARGET_HT20_21,
    WIFI_API_ALL_TARGET_HT20_22,
    WIFI_API_ALL_TARGET_HT20_23,
    WIFI_API_ALL_TARGET_HT40_0_8_16,
    WIFI_API_ALL_TARGET_HT40_1_3_9_11_17_19,
    WIFI_API_ALL_TARGET_HT40_4,
    WIFI_API_ALL_TARGET_HT40_5,
    WIFI_API_ALL_TARGET_HT40_6,
    WIFI_API_ALL_TARGET_HT40_7,
    WIFI_API_ALL_TARGET_HT40_12,
    WIFI_API_ALL_TARGET_HT40_13,
    WIFI_API_ALL_TARGET_HT40_14,
    WIFI_API_ALL_TARGET_HT40_15,
    WIFI_API_ALL_TARGET_HT40_20,
    WIFI_API_ALL_TARGET_HT40_21,
    WIFI_API_ALL_TARGET_HT40_22,
    WIFI_API_ALL_TARGET_HT40_23,
    WIFI_API_AR9300_RATE_SIZE,
} WIFI_API_AR9300_RATES;


typedef struct
{
    u_int8_t aucEepTxpow2G[WIFI_API_AR9300_RATE_SIZE];        /* eeprom  里2G  频段各速率下对应的功率 */
    u_int8_t aucEepTxpow5G[WIFI_API_AR9300_RATE_SIZE];        /* eeprom  里5G  频段各速率下对应的功率 */
    u_int8_t aucRegTxpow[WIFI_API_AR9300_RATE_SIZE];        /* 寄存器 里各速率下对应的功率 */
    u_int8_t is2G;                   
}WIFI_API_TXPOW_INFO_T;
/* END:   Added by wenlei, 2014/8/12   PN:task#322 增加功率调试工具 */

/* BEGIN: Added by wenlei, 2014/10/20   PN:增加用户关联记录信息 */
#define WIFI_API_LOG_MAX_NODE_NUM		16
#define WIFI_API_LOG_MAX_MSG_LEN		128
#define WIFI_API_LOG_MAX_MSG_NUM		32

typedef struct
{
	u_int8_t mac[6];
	u_int32_t assoc_fail_times;
	u_int32_t disassoc_times;
	u_int32_t kick_times;
	u_int8_t msg_num;
	u_int8_t msg[WIFI_API_LOG_MAX_MSG_NUM][WIFI_API_LOG_MAX_MSG_LEN];
}WIFI_API_NODE_ASSOC_MSG_T;

typedef struct
{
	u_int8_t node_num;
	WIFI_API_NODE_ASSOC_MSG_T node_log[WIFI_API_LOG_MAX_NODE_NUM];
}WIFI_API_ASSOC_LOG_T;
/* END:   Added by wenlei, 2014/10/20   PN:增加用户关联记录信息 */


/******************************** 全局变量声明 ********************************/

/******************************** 外部函数原形声明 ****************************/

/*****************************************************************************
 函 数 名  : wifi_api_vap_create
 功能描述  : 创建VAP
 输入参数  : szVapName  :   interface name of the vap  
             szWifiName :   wifi卡名称   
             eWlanMode  :   模式，ap、sta等
             usFlag     :   特殊标志位
 输出参数  : 
 返 回 值  : 0 on success, and WIFI_ERRNO_E if on failure. 
*****************************************************************************/
int wifi_api_vap_create(const char *szVapName,const char *szWifiName,WIFI_API_OPMODE_E eWlanMode);

/*****************************************************************************
 函 数 名  : wifi_api_vap_destroy
 功能描述  : 删除VAP
 输入参数  : szVapName   :   interface name of the vap             
 输出参数  : 
 返 回 值  : 0 on success, and WIFI_ERRNO_E if on failure. 
*****************************************************************************/
int wifi_api_vap_destroy(const char *szVapName);


/*****************************************************************************
 函 数 名  : wifi_api_vap_get_channel
 功能描述  : 获取当前VAP信道
 输入参数  : szVapName   :   interface name of the vap             
 输出参数  : pchannel    :   channel number
 返 回 值  : 0 on success, and WIFI_ERRNO_E if on failure. 
*****************************************************************************/
int wifi_api_vap_get_channel(const char *szVapName, unsigned int *pulChannel);



/*****************************************************************************
 函 数 名  : wifi_api_vap_set_channel
 功能描述  : 设置当前VAP信道
 输入参数  : szVapName   :   interface name of the vap             
             channel     :   channel number
 输出参数  : 
 返 回 值  : 0 on success, and WIFI_ERRNO_E if on failure. 
*****************************************************************************/
int wifi_api_vap_set_channel(const char *szVapName, unsigned int ulChannel);



/*****************************************************************************
 函 数 名  : wifi_api_vap_get_ssid
 功能描述  : 获取当前VAP的ssid
 输入参数  : szVapName   :   interface name of the vap  
             bufsiz      :   bufsiz of outparam pcSsid to store ssidname string ; it should no samller than 32 Bytes
 输出参数  : pcSsid      :   ssidname from wlan driver
 返 回 值  : 0 on success, and WIFI_ERRNO_E if on failure. 
*****************************************************************************/
int wifi_api_vap_get_ssid(const char *szVapName, char *pcSsid, const int bufsiz);


/*******************************************************************************
* 函数名称: wifi_api_vap_set_ssid
* 功    能: 配置vap的ssid
* 参    数: 
* 参数名称       输入/输出       描述
* szVapName         IN           vap设备
* pstSsid           IN           SSID
* 函数返回: 0       -正确
*           其他    -错误
* 说    明: [可选。描述本函数使用是需要注意的地方，实现方法上的关键点]
*******************************************************************************/
int wifi_api_vap_set_ssid(const char *szVapName, WIFI_API_SSID_T *pstSsid);



/*******************************************************************************
* 函数名称: wifi_api_vap_set_mode
* 功    能: 配置ap模式
* 参    数: 
* 参数名称       输入/输出       描述
* szVapName         IN           vap设备
* enPhyMode         IN           ap的物理层模式
* 函数返回: 0       -正确
*           其他    -错误
* 说    明: [可选。描述本函数使用是需要注意的地方，实现方法上的关键点]
*******************************************************************************/
int wifi_api_vap_set_mode(const char *szVapName, WIFI_API_PHYMODE_E enPhyMode);


/*******************************************************************************
* 函数名称: wifi_api_vap_set_txpower
* 功    能: 配置wep加密
* 参    数: 
* 参数名称       输入/输出       描述
* szVapName         IN           vap设备
* ulTxPower         IN           发射功率，单位dbm
* 函数返回: 0       -正确
*           其他    -错误
* 说    明: [可选。描述本函数使用是需要注意的地方，实现方法上的关键点]
*******************************************************************************/
int wifi_api_vap_set_txpower(const char *szVapName,unsigned int ulTxPower);


/*******************************************************************************
* 函数名称: wifi_api_vap_set_wmm
* 功    能: 配置wep加密
* 参    数: 
* 参数名称       输入/输出       描述
* szVapName         IN           vap设备
* enParamType       IN           wmm参数类型
* ucIsBss           IN           0-ap自用的参数，1-配置给sta的参数
* ucAc              IN           wmm优先级
* ulVal             IN           参数具体的值
* 函数返回: 0       -正确
*           其他    -错误
* 说    明: [可选。描述本函数使用是需要注意的地方，实现方法上的关键点]
*******************************************************************************/
int wifi_api_vap_set_wmm(const char *szVapName,
                                    WIFI_API_WMM_PARAM_E enParamType,
                                    unsigned char ucIsBss,
                                    unsigned char ucAc,
                                    unsigned int ulVal);


/*******************************************************************************
* 函数名称: wifi_api_vap_set_wmm_ac
* 功    能: 配置wep加密
* 参    数: 
* 参数名称       输入/输出       描述
* szVapName         IN           vap设备
* stWmmAcParam      IN           整个优先级的参数配置
* 函数返回: 0       -正确
*           其他    -错误
* 说    明: [可选。描述本函数使用是需要注意的地方，实现方法上的关键点]
*******************************************************************************/
int wifi_api_vap_set_wmm_ac(const char *szVapName,WIFI_API_WMM_AC_PARAM_T stWmmAcParam);



/*******************************************************************************
* 函数名称: wifi_api_wifi_set_priority11a
* 功    能: 配置5G  优先接入功能
* 参    数: 
* 参数名称       输入/输出       描述
* szWifiName         IN           wifi设备
* iPollicy          IN           是否开启11a优先
* ulDelay           IN           延迟时间
* ulTimeOut         IN           将终端从列表中删除的时间
* 函数返回: 0       -正确
*           其他    -错误
* 说    明: 建议默认配置为ulDelay = 3，ulTimeOut = 180or300
            delay时间不用太久，足够阻止sta的前几次接入尝试即可，实际测试发现基本能在1秒内完成
            timeout不能设置过短，建议配置为用户可能会尝试重试接入的时间，
                    如果时间过短，可能用户尚未反应过来便再次进入delay状态，导致始终无法接入
*******************************************************************************/
int wifi_api_wifi_set_priority11a(const char *szWifiName,
                                        int iPollicy,
                                        unsigned int ulDelay,
                                        unsigned int ulTimeOut);


/*******************************************************************************
* 函数名称: wifi_api_wifi_set_priority11n
* 功    能: 配置11N  优先接入功能
* 参    数: 
* 参数名称       输入/输出       描述
* szWifiName         IN           wifi设备
* iPollicy          IN           是否开启11a优先
* ulDelay           IN           延迟时间
* ulTimeOut         IN           将终端从列表中删除的时间
* 函数返回: 0       -正确
*           其他    -错误
* 说    明: 基本逻辑同wifi_api_wifi_set_priority11a
*******************************************************************************/
int wifi_api_wifi_set_priority11n(const char *szWifiName,
                                        int iPollicy,
                                        unsigned int ulDelay,
                                        unsigned int ulTimeOut);


/*******************************************************************************
* 函数名称: wifi_api_vap_set_param
* 功    能: 配置vap级参数
* 参    数: 
* 参数名称       输入/输出       描述
* szVapName         IN           vap设备
* enType            IN           参数类型
* ulParam           IN           参数值
* 函数返回: 0       -正确
*           其他    -错误
* 说    明: 部分参数实际上是wifi级的参数，但sdk的实现就是vap级的，我们也只好继续如此
*******************************************************************************/
int wifi_api_vap_set_param(const char *szVapName,WIFI_API_PARAMTYPE_E enType,unsigned int ulParam);
//int wifi_api_vap_get_param(const char *szVapName,WIFI_API_PARAMTYPE_E enType,u_int32_t *pulParam);


/*******************************************************************************
* 函数名称: wifi_api_wifi_set_param
* 功    能: 配置wifi级参数
* 参    数: 
* 参数名称       输入/输出       描述
* szVapName         IN           vap设备
* enType            IN           参数类型
* ulParam           IN           参数值
* 函数返回: 0       -正确
*           其他    -错误
* 说    明: [可选。描述本函数使用是需要注意的地方，实现方法上的关键点]
*******************************************************************************/
int wifi_api_wifi_set_param(const char *szWifiName,WIFI_API_PARAMTYPE_E enType,unsigned int ulParam);
//int wifi_api_wifi_get_param(const char *szWifiName,WIFI_API_PARAMTYPE_E enType,u_int32_t *pulParam);




/*******************************************************************************
* 函数名称: wifi_api_vap_set_wepkey
* 功    能: 配置wep加密
* 参    数: 
* 参数名称       输入/输出       描述
* szVapName         IN           vap设备
* ucEncOn           IN           是否开启加密
* acKey             IN           wep密钥
* usKeyLen          IN           wep密钥长度
* ucKeyType         IN           密钥类型，0-ascii，1-hex
* 函数返回: 0       -正确
*           其他    -错误
* 说    明: [可选。描述本函数使用是需要注意的地方，实现方法上的关键点]
*******************************************************************************/
int wifi_api_vap_set_wepkey(const char *szVapName,
                                    unsigned char ucEncOn,
                                    char acKey[32],
                                    unsigned short usKeyLen,
                                    unsigned char ucKeyType,
                                    unsigned char ucKeyId);



/*******************************************************************************
* 函数名称: wifi_api_vap_trig_bgscan
* 功    能: 触发一次扫描
* 参    数: 
* 参数名称       输入/输出       描述
* szVapName         IN           vap设备
* 函数返回: 0       -正确
*           其他    -错误
* 说    明: [可选。描述本函数使用是需要注意的地方，实现方法上的关键点]
*******************************************************************************/
int wifi_api_vap_trig_bgscan(const char *szVapName);



/*******************************************************************************
* 函数名称: wifi_api_vap_get_localap
* 功    能: 获取同频AP扫描结果
* 参    数: 
* 参数名称       输入/输出       描述
* szVapName         IN           vap设备
* pstNeighborApList OUT         扫描结果数据结构体
* 函数返回: 0       -正确
*           其他    -错误
* 说    明: [可选。描述本函数使用是需要注意的地方，实现方法上的关键点]
*******************************************************************************/
int wifi_api_vap_get_localap(const char *szVapName,WIFI_API_AP_LIST_T *pstNeighborApList);


/*******************************************************************************
* 函数名称: wifi_api_vap_get_neighborap
* 功    能: 获取邻频AP扫描结果
* 参    数: 
* 参数名称       输入/输出       描述
* szVapName         IN           vap设备
* pstNeighborApList OUT         扫描结果数据结构体
* 函数返回: 0       -正确
*           其他    -错误
* 说    明: [可选。描述本函数使用是需要注意的地方，实现方法上的关键点]
*******************************************************************************/
int wifi_api_vap_get_neighborap(const char *szVapName,WIFI_API_AP_LIST_T *pstNeighborApList);



/*******************************************************************************
* 函数名称: wifi_api_vap_get_scan_result
* 功    能: 获取扫描结果
* 参    数: 
* 参数名称       输入/输出       描述
* szVapName         IN           vap设备
* pstScanResult     OUT         扫描结果数据结构体
* 函数返回: 0       -正确
*           其他    -错误
* 说    明: [可选。描述本函数使用是需要注意的地方，实现方法上的关键点]
*******************************************************************************/
int wifi_api_vap_get_scan_result(const char *szVapName,WIFI_API_SCAN_RESULT_T *pstScanResult);



/*******************************************************************************
* 函数名称: wifi_api_vap_get_bss_stats
* 功    能: 获取VAP统计信息
* 参    数: 
* 参数名称       输入/输出       描述
* szVapName         IN           vap设备
* pstBssStats     IN/OUT        统计数据结构体
* 函数返回: 0       -正确
*           其他    -错误
* 说    明: 具体数据结构内分为多类统计，注意指定获取统计类别
*******************************************************************************/
int wifi_api_vap_get_bss_stats(const char *szVapName,WIFI_API_BSS_STATS_T *pstBssStats);



/*******************************************************************************
* 函数名称: wifi_api_vap_get_enduseromc
* 功    能: 获取终端统计信息
* 参    数: 
* 参数名称       输入/输出       描述
* szVapName         IN           vap设备
* pstStats         IN/OUT        统计数据结构体
* 函数返回: 0       -正确
*           其他    -错误
* 说    明: 入参中WIFI_API_ENDUSER_STATS_T包含指针和长度，
*           在调用函数前，务必要保证已经申请了足够多的内存，并在使用后释放
*******************************************************************************/
int wifi_api_vap_get_enduseromc(const char *szVapName,WIFI_API_ENDUSER_STATS_T *pstStats);


/*****************************************************************************
 函 数 名  : wifi_api_sta_authorized
 功能描述  : 打开或者关闭某个STA的鉴权标志位
 输入参数  : szVapName   :   interface name of the vap    
             addr        :   MAC address of STA
             authorized  :   1 -- open ;  0 -- close
 输出参数  : 
 返 回 值  : 0 on success, and WIFI_ERRNO_E if on failure. 
*****************************************************************************/
int wifi_api_sta_authorized(const char *szVapName, const char *addr, int authorized);


/*****************************************************************************
 函 数 名  : wifi_api_sta_deauth
 功能描述  : 发送deauth帧给某个STA
 输入参数  : szVapName   :   interface name of the vap    
             addr        :   MAC address of STA
             reason_code :   802.11 reason code
 输出参数  : 
 返 回 值  : 0 on success, and WIFI_ERRNO_E if on failure. 
*****************************************************************************/
int wifi_api_sta_deauth(const char *szVapName, const char *addr,int reason_code);
int wifi_api_sta_disassoc(const char *szVapName, const char *addr,int reason_code);

/*获取低质量终端限制接入功能参数*/
int wifi_api_get_low_rssi_limit(const char *szVapName, WIFI_API_LOWRSSI_LIMIT_GET_T *pstLowrssiLimit);
int wifi_api_set_low_rssi_limit(const char *szVapName, WIFI_API_LOWRSSI_LIMIT_T *pstLowrssiLimit);
/*****************************************************************************
 函 数 名  : wifi_api_set_lowrssi_limit
 功能描述  : 配置低质量终端限制接入功能参数
 输入参数  : szVapName   :   interface name of the vap    
             			pstLowrssiLimit   : param
 输出参数  : 
 返 回 值  : 0 on success, and WIFI_ERRNO_E if on failure. 
*****************************************************************************/

int wifi_api_set_lowrssi_limit(const char *szVapName, WIFI_API_LOWRSSI_LIMIT_OLD_T *pstLowrssiLimit);

int wifi_api_get_lowratelimit(const char *szVapName, WIFI_API_LOWRATELIMIT_GET_T *pstLowRate);
int wifi_api_set_lowratelimit(const char *szVapName, WIFI_API_LOWRATELIMIT_SET_T *pstLowRateSet);


/*****************************************************************************
 函 数 名  : wifi_api_wifi_get_basic_cap
 功能描述  : 获取卡设备的基本能力信息
 输入参数  :  
             szWifiName :   wifi卡名称 
             pstBasicCap :   基本能力信息结构体
 输出参数  : 
 返 回 值  : 0 on success, and WIFI_ERRNO_E if on failure. 
*****************************************************************************/

int wifi_api_wifi_get_basic_cap(const char *szWifiName, WIFI_API_BASIC_CAP_T *pstBasicCap);

/* BEGIN: Added by wenlei, 2014/7/30   PN:添加自动信道功能 */
/*****************************************************************************
 函 数 名  : wifi_api_wifi_set_acs_config
 功能描述  : 自动信道功能配置
 输入参数  :  
             szWifiName :   wifi卡名称 
             pstAcsCfg :   自动信道配置参数
 输出参数  : 
 返 回 值  : 0 on success, and WIFI_ERRNO_E if on failure. 
*****************************************************************************/

#if 1//GBCOM_ACS_EAP
/* BEGIN: Added by wenlei, 2014/7/30   PN:添加自动信道功能 */

int wifi_api_wifi_set_acs_config(const char *szWifiName, WIFI_API_ACS_CFG_T *pstAcsCfg);

/*****************************************************************************
 函 数 名  : wifi_api_wifi_get_acs_stats
 功能描述  : 获取自动信道相关状态
 输入参数  :  
             szWifiName :   wifi卡名称 
             pstAcsStats :   自动信道相关状态参数
 输出参数  : 
 返 回 值  : 0 on success, and WIFI_ERRNO_E if on failure. 
*****************************************************************************/

int wifi_api_wifi_get_acs_stats(const char *szWifiName, WIFI_API_ACS_STATS_T *pstAcsStats);

/* END:   Added by wenlei, 2014/7/30   PN:添加自动信道功能 */
#endif

/*****************************************************************************
 函 数 名  : wifi_api_wifi_get_txpow_info
 功能描述  : 获取功率相关信息如eeprom  信息、寄存器信息等
 输入参数  :  
             szWifiName :   wifi卡名称 
             pstTxpowInfo :  功率相关信息
 输出参数  : 
 返 回 值  : 0 on success, and WIFI_ERRNO_E if on failure. 
*****************************************************************************/

int wifi_api_wifi_get_txpow_info(const char *szWifiName, WIFI_API_TXPOW_INFO_T *pstTxpowInfo);


/*****************************************************************************
 函 数 名  : wifi_api_vap_get_assoc_log
 功能描述  : 获取关联日志信息
 输入参数  :  
             szVapName :   vap  名称 
             pstAssocLog : 关联日志信息
 输出参数  : 
 返 回 值  : 0 on success, and WIFI_ERRNO_E if on failure. 
*****************************************************************************/

int wifi_api_vap_get_assoc_log(const char *szVapName, WIFI_API_ASSOC_LOG_T *pstAssocLog);

/* BEGIN: Added by WeiHonggang, 2014/12/31   PN:TRAC#1056 非法AP检测和干预 */
int wifi_api_wifi_set_rogue_ap_sw(const char *szWifiName,u_int32_t uiRogueApSw);
int wifi_api_wifi_get_rogue_ap_sw(const char *szWifiName,u_int32_t *puiRogueApSw);
int wifi_api_wifi_set_rogue_ap_rule(const char *szWifiName,u_int32_t uiRogueApRule);
int wifi_api_wifi_get_rogue_ap_rule(const char *szWifiName,u_int32_t *puiRogueApRule);
int wifi_api_wifi_set_rogue_ap_action(const char *szWifiName,u_int32_t uiRogueApAction);
int wifi_api_wifi_get_rogue_ap_action(const char *szWifiName,u_int32_t *puiRogueApAction);
int wifi_api_wifi_rogue_ap_show_ssid(const char *szWifiName,u_int8_t *point);
int wifi_api_wifi_rogue_ap_add_ssid(const char *szWifiName,u_int8_t *pucSsid);
int wifi_api_wifi_rogue_ap_del_ssid(const char *szWifiName,u_int8_t *pucSsid);
int wifi_api_wifi_rogue_ap_show_bssid(const char *szWifiName,u_int8_t *point);
int wifi_api_wifi_rogue_ap_add_bssid(const char *szWifiName,u_int8_t *pucBssid);
int wifi_api_wifi_rogue_ap_del_bssid(const char *szWifiName,u_int8_t *pucBssid);
int wifi_api_wifi_rogue_ap_show_oui(const char *szWifiName,u_int8_t *point);
int wifi_api_wifi_rogue_ap_add_oui(const char *szWifiName,u_int8_t *pucOui);
int wifi_api_wifi_rogue_ap_del_oui(const char *szWifiName,u_int8_t *pucOui);
int wifi_api_wifi_rogue_ap_show_interfere_ap(const char *szWifiName,u_int8_t *point);
int wifi_api_wifi_rogue_ap_add_interfere_ap(const char *szWifiName,u_int8_t *pucBssid);
int wifi_api_wifi_rogue_ap_del_interfere_ap(const char *szWifiName,u_int8_t *pucBssid);
int wifi_api_wifi_rogue_ap_show(const char *szWifiName,u_int8_t *point);
int wifi_api_wifi_rogue_ap_flush_ssid(const char *szWifiName);
int wifi_api_wifi_rogue_ap_flush_bssid(const char *szWifiName);
int wifi_api_wifi_rogue_ap_flush_oui(const char *szWifiName);
int wifi_api_wifi_rogue_ap_flush_interfere_ap(const char *szWifiName);
int wifi_api_wifi_set_rogue_ap_debug_level(const char *szWifiName,u_int32_t uiDebugLevel);
int wifi_api_wifi_get_rogue_ap_debug_level(const char *szWifiName,u_int32_t *puiDebugLevel);
/* END:   Added by WeiHonggang, 2014/12/31   PN:TRAC#1056 非法AP检测和干预 */
/* BEGIN: Added by WeiHonggang, 2015/1/12   PN:TRAC#1085内核扫描定时器归一化整理 */
int wifi_api_wifi_set_scan_timer_debug_level(const char *szWifiName,u_int32_t uiDebugLevel);
int wifi_api_wifi_get_scan_timer_debug_level(const char *szWifiName,u_int32_t *puiDebugLevel);
/* END:   Added by WeiHonggang, 2015/1/12   PN:TRAC#1085内核扫描定时器归一化整理 */
/******************************** 头文件保护结尾 ******************************/
#endif  /* _GBCOM_WIFI_API_H_ */
/******************************** 头文件结束 **********************************/


