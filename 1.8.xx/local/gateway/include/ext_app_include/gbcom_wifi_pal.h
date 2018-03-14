/*******************************************************************************
* COPYRIGHT GBCOM
********************************************************************************
* �ļ�����: ��������API
* �޸���ʷ: 
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


/******************************** ��ͳ������� ********************************/

/* ��ӡ������� */
#define DBG_LVL_DEBUG       7   /* ���Լ������Լ������Ϣ */
#define DBG_LVL_INFO        6   /* ֪ͨ������ʾ��Ϣ����������������ʱ����ӡӲ����Ϣ */
#define DBG_LVL_NOTICE      5   /* ע�⼶������������Ҫ���������������� */
#define DBG_LVL_WARNING     4   /* �澯���������������Կ��ܳ��������������о��� */
#define DBG_LVL_ERROR       3   /* ���󼶣���������������������KERN_ERR������Ӳ������ */
#define DBG_LVL_CRIT        2   /* �ٽ缶���ٽ�������ͨ���漰���ص�Ӳ�����������ʧ�� */
#define DBG_LVL_ALERT       1   /* ���伶��������Ϣ����ʾ�����ȡ��ʩ */
#define DBG_LVL_EMERG       0   /* �������������¼���Ϣ��ϵͳ����֮ǰ��ʾ����ʾϵͳ������ */

#define DBG_LVL_DEFAULT_DEFINE     DBG_LVL_NOTICE

#define DEVNAME_SIZE 16     /* len of interface name(dev->name) */

/* SSID���ݽṹ */
#define  IEEE80211_SSID_LEN  32             /* max len of ssid */

#define WIFI_API_WEP_KEY_LEN_MAX	    16  /* 128 bits (for now) */

/* ����VAPʱ����Щ����Ҫ��ʹ��flagλ��ʾ,������ieee80211_defines.h */
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


/* ���ֲ������ü�ֵ */
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

/* WMM���ȼ����� */
#define WIFI_API_WME_AC_BE  0
#define WIFI_API_WME_AC_BK  1
#define WIFI_API_WME_AC_VI  2    
#define WIFI_API_WME_AC_VO  3

#define WIFI_API_WEP_ASCII  0
#define WIFI_API_WEP_HEX    1

/* BEGIN: Added by wenlei, 2014/5/5   PN:��ֲ��ȡ���豸�����ӿڣ���ȡ�����ֵ */
/*֧�ֵ�Ƶ��*/
#define	WIFI_API_CAP_FREQ_2G     0x00000001   //bit0��2.4GHz
#define	WIFI_API_CAP_FREQ_5G     0x00000002   //bit1��5GHz

/*֧�ֵ�MIMO*/
#define	WIFI_API_CAP_MIMO_NONE     0   //0����֧��
#define	WIFI_API_CAP_MIMO_11     1   //1��1��1
#define	WIFI_API_CAP_MIMO_22     2   //2��2��2
#define	WIFI_API_CAP_MIMO_33     3   //3��3��3
/* END:   Added by wenlei, 2014/5/5   PN:��ֲ��ȡ���豸�����ӿڣ���ȡ�����ֵ */


/******************************** ���Ͷ��� ************************************/

/* NOTE:Ӧ����gbcom_wifi_ioctl.h�еĶ��屣��һ�� */
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
    WIFI_API_E_SYSTEMFAIL           = 1015,     /* ϵͳ����ʧ�� */
    
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

/* Ӧ��NEIGHBOR_AP_LIST_T����һ�� */
typedef struct{
	unsigned int    ulWifiId;
	unsigned char   aucMac[6];		/*��AP MAC*/
	unsigned char   aucMacPad[2];
	unsigned int    ulChannel;		/*��AP CHANNEL*/
	unsigned int    ulNeighborApNum ;
	WIFI_API_AP_INFO_T stNeighborApList[WIFI_API_AP_LIST_MAX] ;
}WIFI_API_AP_LIST_T ;

/* Ӧ��BGSCAN_RESULT_T����һ�� */
typedef struct {
    u_int32_t ulChanNum;                /*�ŵ�����*/
    u_int32_t ulCurrentChan;            /*��ǰ�ŵ���*/
    u_int32_t ulChan[32];	            /*�ŵ���*/
	u_int32_t ulChanApNum[32];	        /*�ŵ���AP��Ŀ(����RSSI���޵�)*/
	u_int32_t ulChanAverageRssi[32];    /*�ŵ���AP��RSSI*/
    int32_t   lChanNoise;               /*��ǰ�ŵ�����*/
    u_int32_t ulInterference;
} WIFI_API_SCAN_RESULT_T ;

typedef enum{      
    WIFI_API_STATUS_DISABLE    =  0 ,      /* ���ܹر� */
    WIFI_API_STATUS_ENABLE     =  1 ,      /* ���ܿ��� */
}WIFI_API_STATUS_E;
/* BEGIN: Added by WeiHonggang, 2014/12/31   PN:TRAC#1056 �Ƿ�AP���͸�Ԥ */
typedef enum
{
    ROGUE_AP_CMD_SW_GET = 0, /*���ܿ���*/
    ROGUE_AP_CMD_SW_SET,
    
    ROGUE_AP_CMD_RULE_GET, /*�Ƿ�AP������*/
    ROGUE_AP_CMD_RULE_SET,
    
    ROGUE_AP_CMD_ACTION_GET,/*�ԷǷ�AP�Ĵ���ʽ*/
    ROGUE_AP_CMD_ACTION_SET,
    
    ROGUE_AP_CMD_SSID_ADD, /*�Ϸ�SSID����*/
    ROGUE_AP_CMD_SSID_DEL,
    ROGUE_AP_CMD_SSID_SHOW,
    ROGUE_AP_CMD_SSID_FLUSH,
    
    ROGUE_AP_CMD_BSSID_ADD, /*�Ϸ�BSSID����*/
    ROGUE_AP_CMD_BSSID_DEL,
    ROGUE_AP_CMD_BSSID_SHOW,
    ROGUE_AP_CMD_BSSID_FLUSH,
    
    ROGUE_AP_CMD_OUI_ADD, /*�Ϸ�OUI����*/
    ROGUE_AP_CMD_OUI_DEL,
    ROGUE_AP_CMD_OUI_SHOW,
    ROGUE_AP_CMD_OUI_FLUSH,
    
    ROGUE_AP_CMD_SHOW, /*�Ƿ�AP�鿴*/
    
    ROGUE_AP_CMD_INTERFERE_ADD ,/*��ԤAP����*/
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
/* END:   Added by WeiHonggang, 2014/12/31   PN:TRAC#1056 �Ƿ�AP���͸�Ԥ */
/* BEGIN: Added by WeiHonggang, 2015/1/12   PN:TRAC#1085�ں�ɨ�趨ʱ����һ������ */
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
/* END:   Added by WeiHonggang, 2015/1/12   PN:TRAC#1085�ں�ɨ�趨ʱ����һ������ */
/* vap����ģʽ */
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
    /* vap������ */
    WIFI_API_PARAMTYPE_HIDESSID         = 0,    /* 0-���ã�1-���� */
    WIFI_API_PARAMTYPE_RTS_THRESHOLD    = 1,    /* 0-2347��0��2347Ϊ�ر� */
    WIFI_API_PARAMTYPE_FRAG_THRESHOLD   = 2,    /* 256-2346��2346Ϊ�ر� */
    WIFI_API_PARAMTYPE_SHORTGI          = 3,    /* 0-���ã�1-���� */
    WIFI_API_PARAMTYPE_RTS_CTS          = 4,    /* 0-none��1-cts only��2-rts/cts */
    WIFI_API_PARAMTYPE_BINTVAL          = 5,    /* ����ͨ������: 40-3500 ms */
    WIFI_API_PARAMTYPE_DTIM             = 6,    /* beacon�ı�����ȡֵΪ1-255 */
    WIFI_API_PARAMTYPE_WDS              = 7,    /* 0-���ã�1-���� */
    WIFI_API_PARAMTYPE_MAX_AID          = 8,    /* 1-128,AID���ֵ����vap���ɹ������û��� */
    WIFI_API_PARAMTYPE_INACT            = 9,    /* Ӧ����2��IEEE80211_INACT_WAIT(1���15��)��С��65535���������60�� */
    WIFI_API_PARAMTYPE_LEGACYRATE       = 10,   /* λͼ:��bit0�𣬷ֱ��ʾ1��2��5.5��11��6��9��12��18��24��36��48��54M */
    WIFI_API_PARAMTYPE_HTRATE           = 11,   /* λͼ:bit0��ʾMCS0��bit1��ʾMCS1���������� */
    WIFI_API_PARAMTYPE_WMM_IE           = 12,   /* 0-���ã�1-���� */
    WIFI_API_PARAMTYPE_COUNTRY_IE       = 13,   /* ����ͨ������: 0-���ã�1-���� */
    //WIFI_API_PARAMTYPE_RSSI_LIMIT       = 14,   /* �ն˽����ź�ǿ�ȣ�ʵ��ӦΪ����ȣ���ֵ */
    WIFI_API_PARAMTYPE_PUREN            = 15,   /* 0-���ã�1-���ã���11n���ܾ�11ag�û����� */
    WIFI_API_PARAMTYPE_PUREG            = 16,   /* 0-���ã�1-���ã���11g���ܾ�11b�û����� */
    WIFI_API_PARAMTYPE_DISABLE_COEXT    = 17,   /* 0-���ã�1-���ã�ǿ��40M */
    WIFI_API_PARAMTYPE_SHORT_PREAMBLE   = 18,   /* ��ǰ��:0-���ã�1-���� */
    WIFI_API_PARAMTYPE_AP_BRIDGE        = 19,   /* AP�ţ�0-���ã�1-���� */
    WIFI_API_PARAMTYPE_AUTHMODE         = 20,   /* ��Ȩģʽ: */
    WIFI_API_PARAMTYPE_CLEAR_APPOPTIE   = 21,   /*  */
    WIFI_API_PARAMTYPE_PRIVCY           = 22,   /*  */
    WIFI_API_PARAMTYPE_DROPUNENCRYPTED  = 23,   /*  */
    /* BEGIN: Added by MaYuHua, 2013/8/6   PN:֧�ֶ�̬RSSI���� */
    //WIFI_API_PARAMTYPE_DYNAMIC_RSSI_LIMIT = 24,   /* �ն˽����ź�ǿ�ȣ�ʵ��ӦΪ����ȣ���ֵ */
    /* END:   Added by MaYuHua, 2013/8/6   PN:֧�ֶ�̬RSSI���� */
    //WIFI_API_PARAMTYPE_RSSI_DELSTA_SWITCH = 25,

    /* wifi������ */
    WIFI_API_ATHPARAMTYPE_AMPDU         = 100,  /* 0-���ã�1-���� */
    WIFI_API_ATHPARAMTYPE_AMSDU         = 101,  /* 0-���ã�1-���� */
    WIFI_API_ATHPARAMTYPE_TX_CHAINMASK  = 102,  /* λͼ:bit0��ʾch0��bit1��ʾch1��bit2��ʾch2 */
    WIFI_API_ATHPARAMTYPE_RX_CHAINMASK  = 103,  /* λͼ:bit0��ʾch0��bit1��ʾch1��bit2��ʾch2 */
    WIFI_API_ATHPARAMTYPE_COUNTRYID     = 104,  /* �����룬����ISO��׼���壬�Զ���241ȫƵ��Ϊ���� */
    WIFI_API_ATHPARAMTYPE_MAXTXPOWLIMIT = 105,  /* ���������,��λΪ1dbm */
    WIFI_API_ATHPARAMTYPE_MGTRATE       = 106,  /* ����֡���ʣ�����beacon����λΪkbps��1000��ʾ1M */
    WIFI_API_ATHPARAMTYPE_MCASTRATE     = 107,  /* �㲥֡���ʣ�����beacon����λΪkbps��1000��ʾ1M */
    WIFI_API_ATHPARAMTYPE_LEGACYRATE    = 108,  /* λͼ:��bit0�𣬷ֱ��ʾ1��2��5.5��11��6��9��12��18��24��36��48��54M */
    WIFI_API_ATHPARAMTYPE_HTRATE        = 109,  /* λͼ:bit0��ʾMCS0��bit1��ʾMCS1���������� */
    WIFI_API_ATHPARAMTYPE_ACKTIMEOUT    = 110,  /* ��λ:usec */
	WIFI_API_ATHPARAMTYPE_RECEIVESENSITIVITY_OPT	= 111,	/* �����������Ż�����*/


    WIFI_API_PARAMTYPE_MAX,
}WIFI_API_PARAMTYPE_E;



typedef enum{
    WIFI_API_WMM_CWMIN      = 0,    /* ����������Сֵ��ȡֵ��Χ0-15 */
    WIFI_API_WMM_CWMAX      = 1,    /* �����������ֵ��ȡֵ��Χ0-15 */
    WIFI_API_WMM_AIFS       = 2,    /* �ٲ�֡�������ȡֵ��Χ0-15 */
    WIFI_API_WMM_TXOPLIMIT  = 3,    /* ���ͻ������ֵ��ȡֵ��Χ0-8192 */
    WIFI_API_WMM_ACM        = 4,    /* ACM��ȡֵΪ0��1,bss only */
}WIFI_API_WMM_PARAM_E;

/* WMM�������޸��������ȼ� */
typedef struct{
    unsigned char    ucAc;           /* Acces Catagory,WMM�����ȼ����࣬0-BE,1-BK,2-VI,3-VO */
    unsigned char    ucBssAcm;       /* ACM��ȡֵΪ0��1,bss only */
    unsigned char    ucBssAifsn;     /* ���͸�STA:�ٲ�֡�������ȡֵ��Χ0-15 */
    unsigned char    ucBssCwMax;     /* ���͸�STA:�����������ֵ��ȡֵ��Χ0-15 */
    unsigned char    ucBssCwMin;     /* ���͸�STA:����������Сֵ��ȡֵ��Χ0-15 */
    unsigned short   usBssTxopLimit; /* ���͸�STA:���ͻ������ֵ��ȡֵ��Χ0-8192 */
    unsigned char    ucChanAifsn;    /* AP����:�ٲ�֡�������ȡֵ��Χ0-15 */
    unsigned char    ucChanCwMax;    /* AP����:�����������ֵ��ȡֵ��Χ0-15 */
    unsigned char    ucChanCwMin;    /* AP����:����������Сֵ��ȡֵ��Χ0-15 */
    unsigned short   usChanTxopLimit;/* AP����:���ͻ������ֵ��ȡֵ��Χ0-8192 */
}WIFI_API_WMM_AC_PARAM_T;


/* ͳ�������е�TX����ʾAP�ķ��ͣ�RX����ʾAP�Ľ��� */

/* ����vap��ͳ�� */
typedef struct{
    unsigned char       ucChannel;              /* ap�����ŵ� */
    unsigned char       ucStationNum;           /* ��ǰvap�¹������ն��� */
    unsigned short      usResv;                 /* ��ʵ�ʺ��壬����4�ֽڶ��� */
    
    unsigned char       aucSsid[IEEE80211_SSID_LEN];    /* essid */
    
    unsigned long       ulTxUcastDataFrame;     /* ���͵�������֡�� */
    unsigned long       ulRxUcastDataFrame;     /* ���յ�������֡�� */
    unsigned long       ulTxMcastDataFrame;     /* ���͹㲥����֡�� */
    unsigned long       ulRxMcastDataFrame;     /* ���չ㲥����֡�� */
    unsigned long       ulTxDataFrame;          /* ��������֡���������㲥���� */
    unsigned long       ulRxDataFrame;          /* ��������֡���������㲥���� */
    unsigned long long  ullTxDataBytes;         /* ���������ֽ��� */
    unsigned long long  ullRxDataBytes;         /* ���������ֽ��� */

    unsigned long       ulTxErrorTotal;         /* ���ʹ���֡�� */
    unsigned long       ulRxErrorTotal;         /* ���մ���֡�� */

}WIFI_API_BSS_AIR_STATS_T;

/* ����֡�շ�ͳ�� */
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

	/* BEGIN: Added by wenlei, 2014/10/24   PN:�����û�������¼��Ϣ */
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
	/* END:   Added by wenlei, 2014/10/24   PN:�����û�������¼��Ϣ */
	
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
    unsigned short      usStaIndex;         /* ��ţ���ʵ�����壬���ڼ�������0��ʼ */
    unsigned char       aucStaMac[6];       /* sta��mac��ַ */
    
    unsigned long       ulStaLiveTime;      /* ��λ:10���룬MIB�ڵ�����Ҫ��ΪTIMETICKS�������͵�λ����0.01�� */
    unsigned long       ulNiFlags;          /* sta�ĸ���״̬λ��ֱ�Ӹ������������� */

    unsigned char       ucStaMode;
        #define WIFI_END_USER_MODE_A    0       /*Aģʽ*/
        #define WIFI_END_USER_MODE_B    1   	/*��Bģʽ*/
        #define WIFI_END_USER_MODE_BG   2   	/*BGģʽ*/
        #define WIFI_END_USER_MODE_NG	3		/*NGģʽ*/
        #define WIFI_END_USER_MODE_NA	4		/*NAģʽ*/
    unsigned char       ucChannel;          /* STA�����ŵ� */
    char                cStaRssi;           /* �ն��ź�ǿ�� */
    char                cChanNoise;         /* ��ǰ�ŵ����� */

    unsigned long       ulTxAvrRate;
    unsigned long       ulRxAvrRate;
    unsigned long       ulTxLastRate;
    unsigned long       ulRxLastRate;
/* BEGIN: Added by WeiHonggang, 2014/3/19   PN:TASK#0 �����ն˳�ʼ����ͳ�� */
    unsigned long       ulTxAssocRate; /*32λ*/
    unsigned long       ulRxAssocRate;/*ͬ��*/
/* END:   Added by WeiHonggang, 2014/3/19   PN:TASK#0 �����ն˳�ʼ����ͳ�� */
    unsigned long       ulTxDataFrame;      /* ��������֡��������������㲥 */
    unsigned long       ulRxDataFrame;      /* ��������֡��������������㲥 */
    unsigned long long  ullTxDataBytes;     /* ���������ֽ���������������㲥 */
    unsigned long long  ullRxDataBytes;     /* ���������ֽ���������������㲥 */
    unsigned long       ulTxDataRetry;      /*  */
    unsigned long       ulRxDataRetry;      /*  */
    unsigned long       ulTxDataFrag;       /*  */
    unsigned long       ulRxDataFrag;       /*  */
    unsigned long       ulTxError;          /*  */
    unsigned long       ulRxError;          /*  */
    
    
}WIFI_API_STA_STATS_T;

typedef struct{
    unsigned short  usTotalStaNumber;      /* vap���ն����� */
    unsigned short  usResv;
    unsigned long   ulBufLen;              /* BUFF��С����WIFI_API_STA_STATS_TΪ��λ */
    WIFI_API_STA_STATS_T *pstStaStats;
}WIFI_API_ENDUSER_STATS_T;

/* BEGIN: Added by wenlei, 2014/4/18   PN:ta55�ӿ������������û����ƽ��빦�� */
typedef struct{
	u_int32_t ulStaDelIntvl;
	u_int32_t ulHighRssi;
	u_int32_t ulLowRssi;
}WIFI_API_LOWRSSI_LIMIT_OLD_T;
typedef struct{
    u_int32_t uiStaDelIntvl;/*�Խ����ն�RSSI���ʱ��*/
    u_int32_t uiRssiLimitDel;/*�ѽ����ն��޳�RSSI����*/
    u_int32_t uiRssiLimit;/*�ն˽���RSSI����*/
    u_int32_t uiRssiLimitProbe;/*probe response�ظ�����*/
    u_int32_t uiKickStaSwitch;/*�Ƿ��޳��ն˿���*/
    u_int32_t uiRule;/*�ն˽�����Ʋ���0 --��̬��1--��̬*/
}WIFI_API_LOWRSSI_LIMIT_T;
typedef struct{
    u_int32_t uiStaDelIntvl;/*�Խ����ն�RSSI���ʱ��*/
    u_int32_t uiRssiLimitDel;/*�ѽ����ն��޳�RSSI����*/
    u_int32_t uiRssiLimit;/*�ն˽���RSSI����*/
    u_int32_t uiRssiLimitProbe;/*probe response�ظ�����*/
    u_int32_t uiKickStaSwitch;/*�Ƿ��޳��ն˿���*/
    u_int32_t uiRule;/*�ն˽�����Ʋ���0 --��̬��1--��̬*/
    u_int32_t uiDynamicRssiLimit;/*��̬RSSI���ޣ����ݽ����û�����̬����*/
    u_int32_t uiAssocRefused;/*�ܾ�����*/   
    u_int32_t uiStaDelCount;/*�޳��ն˴���*/
    u_int32_t uiNoProbeResp;/*���ظ�probe responseͳ��*/
}WIFI_API_LOWRSSI_LIMIT_GET_T;
/* END:   Added by wenlei, 2014/4/18   PN:ta55�ӿ������������û����ƽ��빦�� */

typedef struct
{
    u_int32_t uiSwitch; /*�Ƿ��޳��ն˿���*/
    u_int32_t uiRateLimit;/*�ͽ�MCS�ż�ֵ*/
    u_int32_t uiRateRange;/*������Χ*/
    u_int32_t uiTolerateFreq;/*����Ƶ��*/
    u_int32_t uiAgePeriod;/*�ϻ�ʱ��*/
}WIFI_API_LOWRATELIMIT_SET_T;

typedef WIFI_API_LOWRATELIMIT_SET_T WIFI_API_LOWRATELIMIT_GET_T;

/* BEGIN: Added by wenlei, 2014/5/5   PN:��ֲ��ȡ���豸�����ӿڣ���ȡ�����ֵ */
/* wifi  ���Ļ���������Ϣ*/
typedef struct{
	u_int32_t	cap_freq;		/*֧�ֵ�Ƶ��*/
	u_int32_t	cap_mode;		/*֧�ֵ�����ģʽ*/
	u_int32_t	cap_mimo;		/*֧�ֵ�MIMO ����*/
	u_int32_t	cap_txpow;		/*֧�ֵ������ֵ*/
}WIFI_API_BASIC_CAP_T;
/* END:   Added by wenlei, 2014/5/5   PN:��ֲ��ȡ���豸�����ӿڣ���ȡ�����ֵ */

#if 1 //GBCOM_ACS_EAP
/* BEGIN: Added by wenlei, 2014/7/30   PN:����Զ��ŵ����� */
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
	u_int32_t	ulMode;			/*  �Զ��ŵ�����ģʽ*/
	u_int32_t	ulScanMode;		/*  ɨ��ģʽ*/
	u_int32_t	ulPeriod;		/*  �Զ��ŵ���ʱ�������ڣ���λ(s)*/
	u_int32_t	ul5GExtend;		/*  �Զ��ŵ�����ʱ5G  �Ƿ�֧����չƵ��*/
}WIFI_API_ACS_CFG_T;

typedef struct
{
	u_int32_t	ulMode;			/*  �Զ��ŵ�����ģʽ*/
	u_int32_t	ulScanMode;		/*  ɨ��ģʽ*/
	u_int32_t	ulPeriod;		/*  �Զ��ŵ���ʱ�������ڣ���λ(s)*/
	u_int32_t	ul5GExtend;		/*  �Զ��ŵ�����ʱ5G  �Ƿ�֧����չƵ��*/
	u_int32_t	ulBestChan;		/*  �Զ��ŵ�ѡ���������ŵ�*/
	u_int32_t	ulScanFreq;		/*  �Զ��ŵ�ɨ��ʱɨ���Ƶ��*/
    int32_t     iChanMaxrssi[WIFI_API_CHAN_MAX];			/* ���ŵ�����ź�ǿ��ֵ*/
    int32_t     iChanNoisefloor[WIFI_API_CHAN_MAX];      	/* ���ŵ�����ֵ*/
    int32_t  	iChanLoad[WIFI_API_CHAN_MAX];		 		/* ���ŵ��տ�������*/
}WIFI_API_ACS_STATS_T;
/* END:   Added by wenlei, 2014/7/30   PN:����Զ��ŵ����� */
#endif

/* BEGIN: Added by wenlei, 2014/8/12   PN:task#322 ���ӹ��ʵ��Թ��� */
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
    u_int8_t aucEepTxpow2G[WIFI_API_AR9300_RATE_SIZE];        /* eeprom  ��2G  Ƶ�θ������¶�Ӧ�Ĺ��� */
    u_int8_t aucEepTxpow5G[WIFI_API_AR9300_RATE_SIZE];        /* eeprom  ��5G  Ƶ�θ������¶�Ӧ�Ĺ��� */
    u_int8_t aucRegTxpow[WIFI_API_AR9300_RATE_SIZE];        /* �Ĵ��� ��������¶�Ӧ�Ĺ��� */
    u_int8_t is2G;                   
}WIFI_API_TXPOW_INFO_T;
/* END:   Added by wenlei, 2014/8/12   PN:task#322 ���ӹ��ʵ��Թ��� */

/* BEGIN: Added by wenlei, 2014/10/20   PN:�����û�������¼��Ϣ */
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
/* END:   Added by wenlei, 2014/10/20   PN:�����û�������¼��Ϣ */


/******************************** ȫ�ֱ������� ********************************/

/******************************** �ⲿ����ԭ������ ****************************/

/*****************************************************************************
 �� �� ��  : wifi_api_vap_create
 ��������  : ����VAP
 �������  : szVapName  :   interface name of the vap  
             szWifiName :   wifi������   
             eWlanMode  :   ģʽ��ap��sta��
             usFlag     :   �����־λ
 �������  : 
 �� �� ֵ  : 0 on success, and WIFI_ERRNO_E if on failure. 
*****************************************************************************/
int wifi_api_vap_create(const char *szVapName,const char *szWifiName,WIFI_API_OPMODE_E eWlanMode);

/*****************************************************************************
 �� �� ��  : wifi_api_vap_destroy
 ��������  : ɾ��VAP
 �������  : szVapName   :   interface name of the vap             
 �������  : 
 �� �� ֵ  : 0 on success, and WIFI_ERRNO_E if on failure. 
*****************************************************************************/
int wifi_api_vap_destroy(const char *szVapName);


/*****************************************************************************
 �� �� ��  : wifi_api_vap_get_channel
 ��������  : ��ȡ��ǰVAP�ŵ�
 �������  : szVapName   :   interface name of the vap             
 �������  : pchannel    :   channel number
 �� �� ֵ  : 0 on success, and WIFI_ERRNO_E if on failure. 
*****************************************************************************/
int wifi_api_vap_get_channel(const char *szVapName, unsigned int *pulChannel);



/*****************************************************************************
 �� �� ��  : wifi_api_vap_set_channel
 ��������  : ���õ�ǰVAP�ŵ�
 �������  : szVapName   :   interface name of the vap             
             channel     :   channel number
 �������  : 
 �� �� ֵ  : 0 on success, and WIFI_ERRNO_E if on failure. 
*****************************************************************************/
int wifi_api_vap_set_channel(const char *szVapName, unsigned int ulChannel);



/*****************************************************************************
 �� �� ��  : wifi_api_vap_get_ssid
 ��������  : ��ȡ��ǰVAP��ssid
 �������  : szVapName   :   interface name of the vap  
             bufsiz      :   bufsiz of outparam pcSsid to store ssidname string ; it should no samller than 32 Bytes
 �������  : pcSsid      :   ssidname from wlan driver
 �� �� ֵ  : 0 on success, and WIFI_ERRNO_E if on failure. 
*****************************************************************************/
int wifi_api_vap_get_ssid(const char *szVapName, char *pcSsid, const int bufsiz);


/*******************************************************************************
* ��������: wifi_api_vap_set_ssid
* ��    ��: ����vap��ssid
* ��    ��: 
* ��������       ����/���       ����
* szVapName         IN           vap�豸
* pstSsid           IN           SSID
* ��������: 0       -��ȷ
*           ����    -����
* ˵    ��: [��ѡ������������ʹ������Ҫע��ĵط���ʵ�ַ����ϵĹؼ���]
*******************************************************************************/
int wifi_api_vap_set_ssid(const char *szVapName, WIFI_API_SSID_T *pstSsid);



/*******************************************************************************
* ��������: wifi_api_vap_set_mode
* ��    ��: ����apģʽ
* ��    ��: 
* ��������       ����/���       ����
* szVapName         IN           vap�豸
* enPhyMode         IN           ap�������ģʽ
* ��������: 0       -��ȷ
*           ����    -����
* ˵    ��: [��ѡ������������ʹ������Ҫע��ĵط���ʵ�ַ����ϵĹؼ���]
*******************************************************************************/
int wifi_api_vap_set_mode(const char *szVapName, WIFI_API_PHYMODE_E enPhyMode);


/*******************************************************************************
* ��������: wifi_api_vap_set_txpower
* ��    ��: ����wep����
* ��    ��: 
* ��������       ����/���       ����
* szVapName         IN           vap�豸
* ulTxPower         IN           ���书�ʣ���λdbm
* ��������: 0       -��ȷ
*           ����    -����
* ˵    ��: [��ѡ������������ʹ������Ҫע��ĵط���ʵ�ַ����ϵĹؼ���]
*******************************************************************************/
int wifi_api_vap_set_txpower(const char *szVapName,unsigned int ulTxPower);


/*******************************************************************************
* ��������: wifi_api_vap_set_wmm
* ��    ��: ����wep����
* ��    ��: 
* ��������       ����/���       ����
* szVapName         IN           vap�豸
* enParamType       IN           wmm��������
* ucIsBss           IN           0-ap���õĲ�����1-���ø�sta�Ĳ���
* ucAc              IN           wmm���ȼ�
* ulVal             IN           ���������ֵ
* ��������: 0       -��ȷ
*           ����    -����
* ˵    ��: [��ѡ������������ʹ������Ҫע��ĵط���ʵ�ַ����ϵĹؼ���]
*******************************************************************************/
int wifi_api_vap_set_wmm(const char *szVapName,
                                    WIFI_API_WMM_PARAM_E enParamType,
                                    unsigned char ucIsBss,
                                    unsigned char ucAc,
                                    unsigned int ulVal);


/*******************************************************************************
* ��������: wifi_api_vap_set_wmm_ac
* ��    ��: ����wep����
* ��    ��: 
* ��������       ����/���       ����
* szVapName         IN           vap�豸
* stWmmAcParam      IN           �������ȼ��Ĳ�������
* ��������: 0       -��ȷ
*           ����    -����
* ˵    ��: [��ѡ������������ʹ������Ҫע��ĵط���ʵ�ַ����ϵĹؼ���]
*******************************************************************************/
int wifi_api_vap_set_wmm_ac(const char *szVapName,WIFI_API_WMM_AC_PARAM_T stWmmAcParam);



/*******************************************************************************
* ��������: wifi_api_wifi_set_priority11a
* ��    ��: ����5G  ���Ƚ��빦��
* ��    ��: 
* ��������       ����/���       ����
* szWifiName         IN           wifi�豸
* iPollicy          IN           �Ƿ���11a����
* ulDelay           IN           �ӳ�ʱ��
* ulTimeOut         IN           ���ն˴��б���ɾ����ʱ��
* ��������: 0       -��ȷ
*           ����    -����
* ˵    ��: ����Ĭ������ΪulDelay = 3��ulTimeOut = 180or300
            delayʱ�䲻��̫�ã��㹻��ֹsta��ǰ���ν��볢�Լ��ɣ�ʵ�ʲ��Է��ֻ�������1�������
            timeout�������ù��̣���������Ϊ�û����᳢ܻ�����Խ����ʱ�䣬
                    ���ʱ����̣������û���δ��Ӧ�������ٴν���delay״̬������ʼ���޷�����
*******************************************************************************/
int wifi_api_wifi_set_priority11a(const char *szWifiName,
                                        int iPollicy,
                                        unsigned int ulDelay,
                                        unsigned int ulTimeOut);


/*******************************************************************************
* ��������: wifi_api_wifi_set_priority11n
* ��    ��: ����11N  ���Ƚ��빦��
* ��    ��: 
* ��������       ����/���       ����
* szWifiName         IN           wifi�豸
* iPollicy          IN           �Ƿ���11a����
* ulDelay           IN           �ӳ�ʱ��
* ulTimeOut         IN           ���ն˴��б���ɾ����ʱ��
* ��������: 0       -��ȷ
*           ����    -����
* ˵    ��: �����߼�ͬwifi_api_wifi_set_priority11a
*******************************************************************************/
int wifi_api_wifi_set_priority11n(const char *szWifiName,
                                        int iPollicy,
                                        unsigned int ulDelay,
                                        unsigned int ulTimeOut);


/*******************************************************************************
* ��������: wifi_api_vap_set_param
* ��    ��: ����vap������
* ��    ��: 
* ��������       ����/���       ����
* szVapName         IN           vap�豸
* enType            IN           ��������
* ulParam           IN           ����ֵ
* ��������: 0       -��ȷ
*           ����    -����
* ˵    ��: ���ֲ���ʵ������wifi���Ĳ�������sdk��ʵ�־���vap���ģ�����Ҳֻ�ü������
*******************************************************************************/
int wifi_api_vap_set_param(const char *szVapName,WIFI_API_PARAMTYPE_E enType,unsigned int ulParam);
//int wifi_api_vap_get_param(const char *szVapName,WIFI_API_PARAMTYPE_E enType,u_int32_t *pulParam);


/*******************************************************************************
* ��������: wifi_api_wifi_set_param
* ��    ��: ����wifi������
* ��    ��: 
* ��������       ����/���       ����
* szVapName         IN           vap�豸
* enType            IN           ��������
* ulParam           IN           ����ֵ
* ��������: 0       -��ȷ
*           ����    -����
* ˵    ��: [��ѡ������������ʹ������Ҫע��ĵط���ʵ�ַ����ϵĹؼ���]
*******************************************************************************/
int wifi_api_wifi_set_param(const char *szWifiName,WIFI_API_PARAMTYPE_E enType,unsigned int ulParam);
//int wifi_api_wifi_get_param(const char *szWifiName,WIFI_API_PARAMTYPE_E enType,u_int32_t *pulParam);




/*******************************************************************************
* ��������: wifi_api_vap_set_wepkey
* ��    ��: ����wep����
* ��    ��: 
* ��������       ����/���       ����
* szVapName         IN           vap�豸
* ucEncOn           IN           �Ƿ�������
* acKey             IN           wep��Կ
* usKeyLen          IN           wep��Կ����
* ucKeyType         IN           ��Կ���ͣ�0-ascii��1-hex
* ��������: 0       -��ȷ
*           ����    -����
* ˵    ��: [��ѡ������������ʹ������Ҫע��ĵط���ʵ�ַ����ϵĹؼ���]
*******************************************************************************/
int wifi_api_vap_set_wepkey(const char *szVapName,
                                    unsigned char ucEncOn,
                                    char acKey[32],
                                    unsigned short usKeyLen,
                                    unsigned char ucKeyType,
                                    unsigned char ucKeyId);



/*******************************************************************************
* ��������: wifi_api_vap_trig_bgscan
* ��    ��: ����һ��ɨ��
* ��    ��: 
* ��������       ����/���       ����
* szVapName         IN           vap�豸
* ��������: 0       -��ȷ
*           ����    -����
* ˵    ��: [��ѡ������������ʹ������Ҫע��ĵط���ʵ�ַ����ϵĹؼ���]
*******************************************************************************/
int wifi_api_vap_trig_bgscan(const char *szVapName);



/*******************************************************************************
* ��������: wifi_api_vap_get_localap
* ��    ��: ��ȡͬƵAPɨ����
* ��    ��: 
* ��������       ����/���       ����
* szVapName         IN           vap�豸
* pstNeighborApList OUT         ɨ�������ݽṹ��
* ��������: 0       -��ȷ
*           ����    -����
* ˵    ��: [��ѡ������������ʹ������Ҫע��ĵط���ʵ�ַ����ϵĹؼ���]
*******************************************************************************/
int wifi_api_vap_get_localap(const char *szVapName,WIFI_API_AP_LIST_T *pstNeighborApList);


/*******************************************************************************
* ��������: wifi_api_vap_get_neighborap
* ��    ��: ��ȡ��ƵAPɨ����
* ��    ��: 
* ��������       ����/���       ����
* szVapName         IN           vap�豸
* pstNeighborApList OUT         ɨ�������ݽṹ��
* ��������: 0       -��ȷ
*           ����    -����
* ˵    ��: [��ѡ������������ʹ������Ҫע��ĵط���ʵ�ַ����ϵĹؼ���]
*******************************************************************************/
int wifi_api_vap_get_neighborap(const char *szVapName,WIFI_API_AP_LIST_T *pstNeighborApList);



/*******************************************************************************
* ��������: wifi_api_vap_get_scan_result
* ��    ��: ��ȡɨ����
* ��    ��: 
* ��������       ����/���       ����
* szVapName         IN           vap�豸
* pstScanResult     OUT         ɨ�������ݽṹ��
* ��������: 0       -��ȷ
*           ����    -����
* ˵    ��: [��ѡ������������ʹ������Ҫע��ĵط���ʵ�ַ����ϵĹؼ���]
*******************************************************************************/
int wifi_api_vap_get_scan_result(const char *szVapName,WIFI_API_SCAN_RESULT_T *pstScanResult);



/*******************************************************************************
* ��������: wifi_api_vap_get_bss_stats
* ��    ��: ��ȡVAPͳ����Ϣ
* ��    ��: 
* ��������       ����/���       ����
* szVapName         IN           vap�豸
* pstBssStats     IN/OUT        ͳ�����ݽṹ��
* ��������: 0       -��ȷ
*           ����    -����
* ˵    ��: �������ݽṹ�ڷ�Ϊ����ͳ�ƣ�ע��ָ����ȡͳ�����
*******************************************************************************/
int wifi_api_vap_get_bss_stats(const char *szVapName,WIFI_API_BSS_STATS_T *pstBssStats);



/*******************************************************************************
* ��������: wifi_api_vap_get_enduseromc
* ��    ��: ��ȡ�ն�ͳ����Ϣ
* ��    ��: 
* ��������       ����/���       ����
* szVapName         IN           vap�豸
* pstStats         IN/OUT        ͳ�����ݽṹ��
* ��������: 0       -��ȷ
*           ����    -����
* ˵    ��: �����WIFI_API_ENDUSER_STATS_T����ָ��ͳ��ȣ�
*           �ڵ��ú���ǰ�����Ҫ��֤�Ѿ��������㹻����ڴ棬����ʹ�ú��ͷ�
*******************************************************************************/
int wifi_api_vap_get_enduseromc(const char *szVapName,WIFI_API_ENDUSER_STATS_T *pstStats);


/*****************************************************************************
 �� �� ��  : wifi_api_sta_authorized
 ��������  : �򿪻��߹ر�ĳ��STA�ļ�Ȩ��־λ
 �������  : szVapName   :   interface name of the vap    
             addr        :   MAC address of STA
             authorized  :   1 -- open ;  0 -- close
 �������  : 
 �� �� ֵ  : 0 on success, and WIFI_ERRNO_E if on failure. 
*****************************************************************************/
int wifi_api_sta_authorized(const char *szVapName, const char *addr, int authorized);


/*****************************************************************************
 �� �� ��  : wifi_api_sta_deauth
 ��������  : ����deauth֡��ĳ��STA
 �������  : szVapName   :   interface name of the vap    
             addr        :   MAC address of STA
             reason_code :   802.11 reason code
 �������  : 
 �� �� ֵ  : 0 on success, and WIFI_ERRNO_E if on failure. 
*****************************************************************************/
int wifi_api_sta_deauth(const char *szVapName, const char *addr,int reason_code);
int wifi_api_sta_disassoc(const char *szVapName, const char *addr,int reason_code);

/*��ȡ�������ն����ƽ��빦�ܲ���*/
int wifi_api_get_low_rssi_limit(const char *szVapName, WIFI_API_LOWRSSI_LIMIT_GET_T *pstLowrssiLimit);
int wifi_api_set_low_rssi_limit(const char *szVapName, WIFI_API_LOWRSSI_LIMIT_T *pstLowrssiLimit);
/*****************************************************************************
 �� �� ��  : wifi_api_set_lowrssi_limit
 ��������  : ���õ������ն����ƽ��빦�ܲ���
 �������  : szVapName   :   interface name of the vap    
             			pstLowrssiLimit   : param
 �������  : 
 �� �� ֵ  : 0 on success, and WIFI_ERRNO_E if on failure. 
*****************************************************************************/

int wifi_api_set_lowrssi_limit(const char *szVapName, WIFI_API_LOWRSSI_LIMIT_OLD_T *pstLowrssiLimit);

int wifi_api_get_lowratelimit(const char *szVapName, WIFI_API_LOWRATELIMIT_GET_T *pstLowRate);
int wifi_api_set_lowratelimit(const char *szVapName, WIFI_API_LOWRATELIMIT_SET_T *pstLowRateSet);


/*****************************************************************************
 �� �� ��  : wifi_api_wifi_get_basic_cap
 ��������  : ��ȡ���豸�Ļ���������Ϣ
 �������  :  
             szWifiName :   wifi������ 
             pstBasicCap :   ����������Ϣ�ṹ��
 �������  : 
 �� �� ֵ  : 0 on success, and WIFI_ERRNO_E if on failure. 
*****************************************************************************/

int wifi_api_wifi_get_basic_cap(const char *szWifiName, WIFI_API_BASIC_CAP_T *pstBasicCap);

/* BEGIN: Added by wenlei, 2014/7/30   PN:����Զ��ŵ����� */
/*****************************************************************************
 �� �� ��  : wifi_api_wifi_set_acs_config
 ��������  : �Զ��ŵ���������
 �������  :  
             szWifiName :   wifi������ 
             pstAcsCfg :   �Զ��ŵ����ò���
 �������  : 
 �� �� ֵ  : 0 on success, and WIFI_ERRNO_E if on failure. 
*****************************************************************************/

#if 1//GBCOM_ACS_EAP
/* BEGIN: Added by wenlei, 2014/7/30   PN:����Զ��ŵ����� */

int wifi_api_wifi_set_acs_config(const char *szWifiName, WIFI_API_ACS_CFG_T *pstAcsCfg);

/*****************************************************************************
 �� �� ��  : wifi_api_wifi_get_acs_stats
 ��������  : ��ȡ�Զ��ŵ����״̬
 �������  :  
             szWifiName :   wifi������ 
             pstAcsStats :   �Զ��ŵ����״̬����
 �������  : 
 �� �� ֵ  : 0 on success, and WIFI_ERRNO_E if on failure. 
*****************************************************************************/

int wifi_api_wifi_get_acs_stats(const char *szWifiName, WIFI_API_ACS_STATS_T *pstAcsStats);

/* END:   Added by wenlei, 2014/7/30   PN:����Զ��ŵ����� */
#endif

/*****************************************************************************
 �� �� ��  : wifi_api_wifi_get_txpow_info
 ��������  : ��ȡ���������Ϣ��eeprom  ��Ϣ���Ĵ�����Ϣ��
 �������  :  
             szWifiName :   wifi������ 
             pstTxpowInfo :  ���������Ϣ
 �������  : 
 �� �� ֵ  : 0 on success, and WIFI_ERRNO_E if on failure. 
*****************************************************************************/

int wifi_api_wifi_get_txpow_info(const char *szWifiName, WIFI_API_TXPOW_INFO_T *pstTxpowInfo);


/*****************************************************************************
 �� �� ��  : wifi_api_vap_get_assoc_log
 ��������  : ��ȡ������־��Ϣ
 �������  :  
             szVapName :   vap  ���� 
             pstAssocLog : ������־��Ϣ
 �������  : 
 �� �� ֵ  : 0 on success, and WIFI_ERRNO_E if on failure. 
*****************************************************************************/

int wifi_api_vap_get_assoc_log(const char *szVapName, WIFI_API_ASSOC_LOG_T *pstAssocLog);

/* BEGIN: Added by WeiHonggang, 2014/12/31   PN:TRAC#1056 �Ƿ�AP���͸�Ԥ */
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
/* END:   Added by WeiHonggang, 2014/12/31   PN:TRAC#1056 �Ƿ�AP���͸�Ԥ */
/* BEGIN: Added by WeiHonggang, 2015/1/12   PN:TRAC#1085�ں�ɨ�趨ʱ����һ������ */
int wifi_api_wifi_set_scan_timer_debug_level(const char *szWifiName,u_int32_t uiDebugLevel);
int wifi_api_wifi_get_scan_timer_debug_level(const char *szWifiName,u_int32_t *puiDebugLevel);
/* END:   Added by WeiHonggang, 2015/1/12   PN:TRAC#1085�ں�ɨ�趨ʱ����һ������ */
/******************************** ͷ�ļ�������β ******************************/
#endif  /* _GBCOM_WIFI_API_H_ */
/******************************** ͷ�ļ����� **********************************/


