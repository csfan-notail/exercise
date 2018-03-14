#ifndef __LTE_COM_H__
#define __LTE_COM_H__

#include <sys/time.h>

#define LTE_USB_COM_FD_FILE     "/tmp/ltecom.info"
#define LTE_COM_SOCKET_NAME     "/var/run/ltecomsev"
#define LTE_COM_MSG_LEN         512
#define LTE_SMS_RECV_FILE       "/tmp/smsrecv"
#define LTE_SMS_SEND_FILE       "/tmp/smssendfile"
#define LTE_BUF_FILE_PATH       "/tmp/fbuf"
#define LTE_LCID_MASK           0x000000ff
#define LTE_LCTCELLINFO_FILE    "/tmp/ltecellinfo"


#define LTE_CHINA_MOBILE_CODE      "46000,46002,46007"
#define LTE_CHINA_UNICOM_CODE      "46001,46006"
#define LTE_CHINA_TELECOM_CODE     "46003,46005,46011"

#define LTE_CHINA_MOBILE_NAME      "CHINA MOBILE"
#define LTE_CHINA_UNICOM_NAME      "CHINA UNICOM"
#define LTE_CHINA_TELECOM_NAME     "CHINA TELECOM"


#define UINT8        unsigned char

#define AT_MAX_EXECUTE_COUNT    2  /* ATָ�����ִ�д��� */

#ifndef LTE_LIBEVENT_PRIORITY_FD_COM_LEVEL
               
#define LTE_LIBEVENT_PRIORITY_FD_COM_LEVEL   1	
               
#endif /* LTE_LIBEVENT_PRIORITY_FD_COM_LEVEL */

#ifndef LTE_LIBEVENT_PRIORITY_FD_SOCKET_LEVEL
               
#define LTE_LIBEVENT_PRIORITY_FD_SOCKET_LEVEL   3	
               
#endif /* LTE_LIBEVENT_PRIORITY_FD_SOCKET_LEVEL */
#ifndef LTE_LIBEVENT_PRIORITY_FD_ACCEPT_LEVEL
               
#define LTE_LIBEVENT_PRIORITY_FD_ACCEPT_LEVEL   2	
               
#endif /* LTE_LIBEVENT_PRIORITY_FD_ACCEPT_LEVEL */

#define LTE_READ_LTE_INFO_STATIC    1

typedef int EVENT_BASE_ID;
typedef int EVENT_ID;
typedef enum {
    WAIT_WRITE,         //�ȴ�д
    NOT_NEED_WRITE,     //����Ҫд
    WAIT_READ,          //д���ȴ���
    WRITE_DONE,         //�ڵ������ɣ����д�Ͷ�����
}LTE_NODE_SM_E;

typedef enum {
    WRITE_ABLE,     //com�ڿ�д
    WRITE_DISABLE,  //com�ڲ���д
}LTE_COM_SM_E;

typedef enum {
	M_LONGCHEER = 0,
	M_LEADCORE,
	M_THINKWILL,
	M_ALTAIR,
	M_ZTE,
    M_ZTE_CDMA2000,
    M_ARCCRA,
    M_THINKWILL_CDMA_MI660,
    M_YUGA_CDMA_CEM600,
    M_ARCCRA_SECOND,
    M_ZTE_MICRO,
    M_HUAWEI,
    M_YUGA,
    M_MARVELL,
    /*��Ϊ4Gģ��洢ģʽ*/
    M_HUAWEI4G_STORAGE,
    /*��Ϊ4Gģ������ģʽ*/
    M_HUAWEI4G_NETCARD,
	/* Donnot modify this value */
	M_TOTAL_NUM
}LTE_PLATFORM_E;
typedef enum
{
    LTE_ARCCRA_8500 = 0,
    LTE_ARCCRA_8501A,
    LTE_ARCCRA_8501B,
    LTE_ARCCRA_8501C,
    LTE_LONCHEER_9105,
    LTE_LONCHEER_9106,
    LTE_LONCHEER_9115,
    LTE_ALTAIR_LP10_P4,
    LTE_ZTE_MICRO_B57_5,
    LTE_ZTE_MICRO_B57_1,
    LTE_YUGA_CEM600,
    LTE_THINKWILL_MI900,
    LTE_THINKWILL_MI660,
    LTE_ZTE_MF210,
    LTE_ZTE_MC2716,
    LTE_LEADCORE_LC1761,
    LTE_LEADCORE_MD6200,
    LTE_HUAWEI_MU709S_2,
    LTE_YUGA,
    LTE_ZTE_MICRO_B78,
    LTE_MARVELL,
    LTE_HUAWEI4G,

    /* Donnot modify this value */
    LTE_VER_IND_MAX
}LTE_VER_INDEX_E;

typedef enum {
	LTE_CMD_MIN = 0,
	LTE_PIN_STATE,
	LTE_PIN_SWITCH,
	LTE_PIN_SW_GET,
	LTE_PIN_UNLOCK,
	LTE_PIN_ATTEMPTS,
	LTE_PIN_ALTER,
	LTE_SIGNAL_GET,
	LTE_POWER_ON,
	LTE_POWER_OFF,
	LTE_POWER_STATE_GET,
	LTE_NETTYPE_SET,
	LTE_NETTYPE_GET,
	LTE_PLMN_REGISTER,
	LTE_PREFER_PLMN_SET,
	LTE_NETWORK_PREPARE,
	LTE_NETWORK_STATUS_GET,
	LTE_CONNECT,
	LTE_DISCONNECT,
	LTE_LINKSTATE_GET,
	LTE_IMEI_GET,
	LTE_CELLID_GET,
	LTE_ICCID_GET,
	LTE_IMSI_GET,
	LTE_SIM_STATE_GET,
	LTE_INFO_GET,
	LTE_ACCESS_NET_GET,
	LTE_BANDS_GET,
	LTE_CLCK_BANDS_SET,
	LTE_CLCK_BANDS_CLOSE,
	LTE_CLCK_CELL_GET,
	LTE_CLCK_CELL_SET,
    LTE_GPS_START,
    LTE_GPS_STOP,
    LTE_GPS_DATE_GET,
    LTE_GPS_POSITION_INFO_GET,
    /* BEGIN: Added by wujian, 2014/11/7   PN:��ӻ�ȡ��Ӫ����Ϣ */
    LTE_OPER_INFO_GET,
    /* END:   Added by wujian, 2014/11/7   PN:��ӻ�ȡ��Ӫ����Ϣ */
    /* BEGIN: Added by wujian, 2014/11/12   PN:��ӻ�ȡ֧�ֵ��������ͣ���֧��2g��3g��4g */
    LTE_NETTYPE_INFO_GET,
    /* END:   Added by wujian, 2014/11/12   PN:��ӻ�ȡ֧�ֵ��������ͣ���֧��2g��3g��4g */

    /* BEGIN: Added by wujian, 2014/11/13   PN:��Ӷ��Ź�����ؽӿ�ID */
    LTE_SMS_CENT_CODE_SET,
    LTE_SMS_MODE__SET,
    LTE_SMS_FUNC_INIT,
    LTE_SMS_RECV,
    LTE_SMS_SEND,
    LTE_SMS_DELE,
    LTE_SMS_SUPP,
    /* END:   Added by wujian, 2014/11/13   PN:��Ӷ��Ź�����ؽӿ�ID */
    LTE_IPV6_PDP_SET,
    LTE_APN_SET,
    LTE_VERSION_GET,
    LTE_ENODEBID_GET,
    LTE_TAC_GET,
    LTE_LCID_GET,
    LTE_ALL_CELLID_GET,
    LTE_MOUDLE_MAC_SET,
    LTE_MOUDLE_MAC_GET,
    LTE_SIM_UNLOCK,
    LTE_SIM_DISABLE_UNLOCK,
    LTE_SIM_UNLOCK_SUPPORT,
    LTE_SIM_TYPE_GET,
    LTE_SIM_UNLOCK_GET,
    LTE_SIGNAL_VALUE_GET,
    LTE_IP_GATEWAY_DNS_GET,
    LTE_ALL_PLMN_MODE_GET,
    LTE_SOUNET_MODE_SET,
    LTE_SIM_UNLOCK_PUK,
    LTE_MODULE_UP_SUPPORT,
    LTE_PIN_INPUT_REMAIN_GET,
    /*��Ϊģ���·�ɹ���*/
    LTE_POST_ROUTE_SET,
    LTE_NV_DATALOCK,
    LTE_POST_ROUTE_GET,
    LTE_REMOVE_ATTACH,
    LTE_QOS_DATA_SET,
    LTE_QOS_DATA_GET,
	LTE_DLOAD_PROT_VER_GET,
    LTE_DLOAD_VER_INFO_GET,
    LTE_NV_BACKUP,
    LTE_NV_RESTORE,
    LTE_DLOAD_MODE,
    LTE_DATA_MODE,
    LTE_RESET,
    
    /* lte sim soft and hard switch*/
    LTE_SIM_SWITCH_GET,
    LTE_SIM_SWITCH_SET,
    LTE_SIM_SWITCH_SOFT_SET,
    
	LTE_CMD_MAX
}LTE_CMD_E;


typedef enum {
	LTE_FAIL = 0,
	LTE_OK,
	LTE_WAIT
}LTE_RET_E;
typedef enum
{
    PIN_STATE_READY = 0,
    PIN_STATE_PIN = 1,
    PIN_STATE_PUK = 2,
    PIN_SIM_BUSY = 3
}LTE_PIN_STATE_E;
typedef enum
{
    PIN_SWITCH_OFF = 0,
    PIN_SWITCH_ON = 1
}LTE_PIN_SWITCH_E;

typedef enum
{
    PIN_CLOSE = 0,
    PIN_OPEN = 1
}LTE_PIN_SW_STATE_E;

typedef enum
{
	LTE_SIM_NOT_FOUND = 0,
	LTE_SIM_READY,
}LTE_SIM_STATE_E;
typedef enum {
	NET_NONE = 0,
    NET_2G,
	NET_3G,
	NET_4G
}LTE_ACCESS_NETWORK_E;

typedef enum
{
	LTE_LINKSTATE_DISCONNECT = 0,
	LTE_LINKSTATE_CONNECTING,
	LTE_LINKSTATE_CONNECTED,
	LTE_LINKSTATE_CONNECT_FAIL
}LTE_LINKSTATE_E;
typedef enum
{
    CLC_BAND_DISABLE = 0,
    CLC_BAND_ENABLE = 1
}LTE_CLC_BAND_E;

typedef enum
{
    LTE_AUTO_MODE = 0x0000,//Ϊ��ģ�豸(�������7ģ�豸)���
    LTE_2G_ONLY = 0x0001,
    LTE_3G_ONLY = 0x0002,
    LTE_4G_ONLY = 0x0004,
    LTE_2G_3G = 0x0001 |0x0002,
    LTE_3G_4G = 0x0002 |0x0004,
    LTE_2G_3G_4G = 0x0001 | 0x0002 |0x0004,
}LTE_NET_TYPE_E;
typedef enum
{
    LTE_NONE = 0x0000,
    LTE_2G_PRI = 0x0001,
    LTE_3G_PRI = 0x0002,
    LTE_4G_PRI = 0x0004,
    LTE_AUTO = 0x0008,
}LTE_NET_TYPE_PRI_E;
typedef enum
{
    NO_SUP_SMS_FUNC = 0,
    SUP_SMS_FUNC = 1
}LTE_SUPPORT_SMS_E;

typedef enum
{
	PDP_IPV4 = 0,
	PDP_IPV6,
	PDP_IPV4V6
}LTE_CGDCONT_E;

typedef enum
{
	COM_WRITE_TYPE_1 = 0,  /* �����͵Ľڵ㣬ֻ����ڵ�дһ��ATָ�� */
    COM_WRITE_TYPE_2 = 1   /* �����͵Ľڵ㣬��Ҫд��ATָ��󣬻��к����ַ�д
                              ��com�� */
}LTE_COM_WRITE_TYPE_E;

typedef enum
{
	COM_READ_TYPE_1 = 0,  /* ATָ��ظ�ʽ��ΪOK */
    COM_READ_TYPE_2 = 1,  /* ATָ��ظ�ʽ��ΪERROR */
    COM_READ_TYPE_3 = 2,  /* ATָ��ظ�ʽ��+ xxxx ERROR: 10 */
    COM_READ_TYPE_4 = 3,  /* ATָ��ظ�ʽ��+CIEV: xxxxxx\n OK \n */
    COM_READ_TYPE_5 = 4,  /* ATָ��ظ�ʽ�� xxxxxx\n OK \n */
    COM_READ_TYPE_6 = 5,  /* ATָ��ظ�ʽ��+CIEV: xxxxxx\n +CIEV: xxxxxx\n +CIEV: xxxxxx\n OK \n  */
    COM_READ_TYPE_7 = 6,  /* ��com�ڶ�ȡ�����ݸ�ʽ: +CIEV:xxxxxx\n */
    COM_READ_TYPE_8 = 7,   /* ��com�ڶ�ȡ�����ݸ�ʽ: OK\n  +CIEV:xxxxxx\n +CIEV:xxxxxx\n */
    COM_READ_TYPE_9 = 8  /* ATָ��ظ�ʽ��+CIEV1: xxxxxx\n +CIEV2: xxxxxx\n +CIEV3: xxxxxx\n OK \n  */
    
}LTE_COM_READ_TYPE_E;

typedef enum
{
	COM_RETURN_SINGLE = 0,   /* ATָ���1�� */
    COM_RETURN_MUTIL = 1     /* ATָ��ض������ */
}LTE_COM_RETURN_TYPE_E;


typedef enum
{
    SERVER_NEED_SEND = 0,   /* �������˽��յ����ݺ���Ҫ���͸��ͻ��� */
    SERVER_NOT_SEND = 1     /* �������˽��յ����ݺ���Ҫ���͸��ͻ��� */
}LTE_SERVER_SEND_TYPE_E;

typedef struct
{
    char acAtCmd[256];
    char acCiev[32];
    int  iFuncCmd;
    int  iPriority;
    int  iNodeSM;
    int  iRealFlag;
    int  iComWriteType; /* ��com��дATָ��ĸ�ʽ��д��λ���дһ�� */
    int  iComReadType;  /* ��com�ڶ�ȡ���ݣ�����ʱ�ĸ�ʽ */
    int  iComReturnType;
    int  iServerSendType;/* �������˽��յ����ݺ��Ƿ���Ҫ���͸��ͻ��� */
}CLIENT_RECV_MSG_T;
typedef struct __lte_timer_t
{
    struct timeval  stTimeVal;
    
    EVENT_ID eiTimer;
    
}LTE_EVENT_TIMER_T;
typedef struct __lte_fd_t
{
    int iFd_v4;
    int iPort_v4;
    int iNumber;
    
    EVENT_ID eiFD;
    
}LTE_EVENT_FD_T;
typedef struct __lte_event_t
{
    LTE_EVENT_FD_T      stFdEvent;
    LTE_EVENT_FD_T      stComEvent;
    LTE_EVENT_TIMER_T   stOperInfoTimer;
    LTE_EVENT_TIMER_T   stSigTimer;
    LTE_EVENT_TIMER_T   stSimStateTimer;
    LTE_EVENT_TIMER_T   stAccessNetTimer;
    LTE_EVENT_TIMER_T   stLinkTimer;
    LTE_EVENT_TIMER_T   stCellidTimer;
    LTE_EVENT_TIMER_T   stLteInfoTimer;
    EVENT_BASE_ID       eEventBaseID;
    
    int                 iComFd;
    LTE_COM_SM_E        enComSM;
}LTE_EVENT_T;

typedef  struct __lte_module_ops
{
    LTE_RET_E (*lte_pin_state_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_pin_switch)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_pin_switch_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_pin_unlock)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_pin_attempts)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_pin_code_alter)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_signal_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_power_on)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_power_off)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_power_state_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_net_type_set)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_net_type_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_plmn_register)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_plmn_prefer_set)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_net_work_prepare)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_net_work_status_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_connect)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_disconnect)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_link_state_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_imei_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_cellid_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_iccid_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_imsi_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_sim_state_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_info_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_access_net_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_bands_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_clck_bands)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_clck_cell_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_clck_cell)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_gps_start)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_gps_stop)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_oper_info_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_nettype_info_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_sms_cent_code_set)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_sms_mode_set)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_sms_func_init)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_sms_recv)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_sms_send)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_sms_del)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_sms_supinfo_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_sms_ip_stack_set)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_apn_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_apn_set)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_version_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_enodebid_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_tac_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_lcid_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_all_cellid_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_mac_set)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_mac_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_sim_unlock)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_sim_disable_unlock)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_sim_type_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_ip_gw_dns_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_all_plmn_mode_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_sounet_mode_set)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_sim_puk_unlock)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_pin_input_remain)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_post_route_set)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_nv_datalock)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_post_route_get)(void *pvData, char *pcBuf); 
    LTE_RET_E (*lte_remove_attach)(void *pvData, char *pcBuf);
	LTE_RET_E (*lte_qos_set)(void *pvData, char *pcBuf);
	LTE_RET_E (*lte_qos_get)(void *pvData, char *pcBuf);
	LTE_RET_E (*lte_dload_prot_ver_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_dload_ver_info_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_nv_backup)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_nv_restore)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_dload_mode)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_data_mode)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_reset)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_sim_switch_set)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_sim_switch_get)(void *pvData, char *pcBuf);
    LTE_RET_E (*lte_sim_switch_soft_set)(void *pvData, char *pcBuf);
    
}LTE_MODULE_OPS_T;

typedef struct
{
    LTE_PIN_STATE_E enState; /* 0Ϊready״̬��1Ϊpin״̬��2Ϊpuk״̬ */
}LTE_PIN_STATE_T;
typedef struct 
{
    char acPin[5];  /* pin�� */
    LTE_PIN_SWITCH_E enSwitch;    /* 0��ʾ�ر�Pin�룬1��ʾ����PIN�빦�� */
} LTE_PIN_SWITCH_T;

typedef struct
{
    LTE_PIN_SW_STATE_E enState; /* 0ΪPIN�ر�״̬��1Ϊpin����״̬ */
}LTE_PIN_SW_STATE_T;
typedef struct 
{
    int iCfunFlag;  /* cfun��־��0��û������Э��ջ��1���Ѿ�����Э��ջ */
} LTE_CFUN_QUERY_T;
typedef struct 
{
    char acNetWorkStatus[32];     /* ����״̬ */
} LTE_NETWORK_PREPARE_T;

typedef struct 
{
    int iSignal;    /* �ź�ֵ */
} LTE_SIGNAL_T;
typedef struct 
{
    LTE_LINKSTATE_E enLinkstate;   /* ��·״̬ */
} LTE_LINKSTATE_T;

typedef struct 
{
    char acImei[32];    /* IMEI�� */
} LTE_IMEI_T;
typedef struct 
{
    char acCellID[32];    /* cell id */
} LTE_CELLID_T;

typedef struct 
{
    char acICCID[32];    /* ICCIDֵ */
} LTE_ICCID_T;
typedef struct 
{
    char acIMSI[32];    /* IMSIֵ */
} LTE_IMSI_T;
typedef struct
{
	LTE_SIM_STATE_E enSimState;
}LTE_SIM_STATE_T;
typedef struct
{
	int iRssi;      // dbm
	int iRsrp;     // dbm
	int iRsrq;     // db
	int iSinr;     // db
}LTE_QSQ_T;
typedef struct
{
	char acBand[4];
	int iChannel;
    char acPhyCellid[32];
}LTE_BAND_T;

typedef struct
{
	char acCellId[32];
    char acPhyCeillid[32];
	int iRssi;      // dbm
	int iRsrp;     // dbm
	int iRsrq;     // db
	int iSinr;     // db
	char acBand[4];
	int iChannel;
}LTE_CELLINFO_T;

typedef struct
{
	LTE_CELLINFO_T stCellInfo;
}LTE_INFO_T;

typedef struct
{
	LTE_ACCESS_NETWORK_E enAccessNet;
}LTE_ACCESS_NETWORK_T;

typedef struct
{
	char bands[32];
    int  iClcBandAble;/* �Ƿ���Ƶ */
}LTE_BANDS_T;
typedef struct
{
    int iClkCellEnable;
}LTE_CLKCELL_ENABLE_T;
typedef struct
{
    char acOperName[32];/* ��Ӫ������ */
}LTE_OPER_INFO_T;
typedef struct 
{
   LTE_NET_TYPE_E enNetType;    /* ������ʽ ��3G�ͽ�4G*/
   LTE_NET_TYPE_PRI_E enNetTypePri;
} LTE_NET_TYPE_T;
typedef struct
{
    LTE_SUPPORT_SMS_E enSupSms;/*1 ֧�ֶ��Ź��ܣ�0 ��֧��*/
}LTE_SUPPORT_SMS_T;
typedef struct
{
    char acSmsRecvFile[32];/* ���ն��Ŵ�ŵ��ļ�·�� */
}LTE_SMS_RECV_T;


typedef struct 
{
    LTE_CGDCONT_E enPdpStack;   /*0, IPV4��ջ��1��ipV6��ջ��2��IPV4��V6˫ջ */
    int  iApnFlag;               /* 0��������apn��1������apn */
    char acApn[32];     /* APN */
} LTE_CGDCONT_T;
typedef struct 
{
    char acApn[32];     /* APN */
} LTE_APN_T;

typedef struct 
{
	char acVersion[128];/* ʵ�ʰ汾�� */
	int iVersionIndex;  /* �汾������ */
}LTE_MODULE_VERSION_T;
typedef struct
{
    char acEnodebId[16]; /*enodeB ID*/
}LTE_ENODB_ID_T;
typedef struct
{
    char acTac[16]; /*tac*/
}LTE_TAC_T;
typedef struct
{
    char acLcid[16]; /*lcid*/
}LTE_LCID_T;

typedef struct
{
    char acCellId[32]; /*cellid*/
    char acEnodebId[32]; /*enodeB ID*/
    char acLcid[32]; /*lcid*/
    char acTac[32]; /*tac*/
}LTE_ALL_CELLID_T;

typedef struct
{
    char acMac[32]; /*mac*/
}LTE_MOUDLE_MAC_T;

typedef struct 
{
    int iUnlockFlag;  /* 0����ģ��û��SIM������ģʽ��1����ģ����SIM������ģʽ */
} LTE_UNLOCK_SUPPORT_T;

typedef enum
{
    NORMAL_SIM = 0,/* 0����ͨSIM����*/
    ENCRYPTION_SIM = 1/* 1������SIM�� */
}LTE_SIM_TYPE_E;


typedef struct 
{
    LTE_SIM_TYPE_E iSIMType;  /* 0����ͨSIM����1������SIM�� */
    char cRandomNum[64];/* 20�ֽ�BCD����� */
} LTE_SIM_RANDOM_NUM_T;

typedef enum
{
    SIM_NOT_UNLOCK = 0,/*SIM��δ����*/
    SIM_HAS_UNLOCK = 1 /* SIM���Ѿ�����*/
}LTE_SIM_UNLOCK_STATE_E;


typedef struct 
{    
    LTE_SIM_UNLOCK_STATE_E iIsUnlock; /*0, SIM��δ������ 1��SIM���Ѿ�����*/
} LTE_SIM_UNLOCK_STATE_T;

typedef struct 
{    
	char acPuk[16];
	char acPinNew[16];
} LTE_SIM_UNLOCK_PUK_T;

typedef struct 
{   
    LTE_CGDCONT_E enPdpStack;   /*0, IPV4��ջ��1��ipV6��ջ��2��IPV4��V6˫ջ */
    UINT8 acIpv4[4];
    UINT8 acGatewayV4[4];
    UINT8 acDns1V4[4];
    UINT8 acDns2V4[4];
    UINT8 acIpv6[16];
    UINT8 acGatewayV6[16];
    UINT8 acDns1V6[16];
    UINT8 acDns2V6[16];
} LTE_IP_GATEWAY_DNS_T;

/* �ֶ����� begin add by wangqi*/
typedef struct 
{    
    char acAllPlmn[256]; /*PLMN+��ʽ��PLMN+��ʽ��PLMN+��ʽ*/
} LTE_ALL_PLMN_MODE_T;

typedef struct 
{    
    char acPlmn[16]; /*PLMN*/
    char acMode[16];
} LTE_SOUNET_MODE_T;

/* �ֶ����� end add by wangqi*/

typedef struct
{
    int iPinIputRemain;  /* ʣ��PIN��������� */
}LTE_PIN_INPUT_REMAIN_T;

/*��Ϊ��·�ɿ���*/
typedef enum
{
    POST_ROUTE_CLOSE = 0,/*�رպ�·�ɹ���*/
    POST_ROUTE_OPEN = 1 /*�򿪺�·�ɹ���*/
} LTE_POST_ROUTE_SW_E;

typedef struct 
{
    LTE_POST_ROUTE_SW_E iPostRouteSw; 
} LTE_POST_ROUTE_SW_T;


typedef struct{
    char acSwer[32];
    char acIosVer[32];
    char acProductName[32];
    char acDloadType[32];
} LTE_DLOAD_PROT_VER_GET_T;
typedef struct{
    char acDloadVerInfo[16];
}LTE_DLOAD_VER_INFO_GET_T;
typedef struct{
    int iNvBackup;
}LTE_NV_BACKUP_T;
typedef struct{
    int iNvRestore;
}LTE_NV_RESTORE_T;

typedef struct{
	int iSimSw;
	int iSimChoice;
	int iSimEnable;
	char acSimImsi[16];
	int iSimMnclen;
	int iSimSuauthpara;
	int iSimSualg;
	char acSimKey[64];
	char acSimOp[64];
}LTE_SIM_SWITCH_T;

/*add new acmt*/

typedef struct
{
	int iQosCid;
	int iQosQci;
	int iQosDlGbr;
	int iQosUlGbr;
	int iQosDlMbr;
	int iQosUlMbr;
	int iArgcNumber;
	
}LTE_QOS_DATA_S;







typedef struct __lte_state_info
{
    char acLteVersion[128];    /* ģ��ʵ�ʰ汾�� */
    int  iVersionIndex;        /* ģ������ */
    int  iClcBandAble;         /* �Ƿ�֧����Ƶ */
    int  iClkCellEnable;       /* �Ƿ�֧����С�� */
    int  iSignal;              /* ģ���ƽ�ź� */
    int  iRssi;
    int  iRSRP;
    int  iRSRQ;
    int  iSinr;
    int  iChannel;
    int  iSimUnlockState;
    char acBand[32];
    char acCellId[32];
    char acPhyCellId[32];
    char acImei[32];
    char acIccid[32];
    char acImsi[32];
    char acBands[32];
    char acOperInfo[32];
    char acEnodebID[32];
    char acTac[16];
    char acLcid[16];    
    char acMac[32];
    
    LTE_SIM_STATE_E enSimState;/* sim��״̬ */
    LTE_PIN_STATE_E enPinState;/* pin״̬��Ϣ */
    LTE_PIN_SW_STATE_E enPinSwState;
    LTE_ACCESS_NETWORK_E enAccessNet;/* ��������type */
    LTE_NET_TYPE_E enSupNetType;    /* ������ʽ ��3G�ͽ�4G*/
    LTE_NET_TYPE_E enCurNetType;    /* ������ʽ ��3G�ͽ�4G*/
    LTE_NET_TYPE_PRI_E enNetTypePri; /* �������ȼ� */
    LTE_LINKSTATE_E enLinkState;/* ����״̬ */
    LTE_SUPPORT_SMS_E enSupSms;/* �Ƿ�֧�ֶ��� */
    LTE_SIM_RANDOM_NUM_T stSimRandNum;

    int iPinStaticFlag;
    int iPinSwStaticFlag;
    int iNetTypeStaticFlag;
    int iSignalStaticFlag;
    int iLinkStaticFlag;
    int iIMEIStaticFlag;
    int iIMSIStaticFLag;
    int iLteInfoStaticFLag;
    int iICCIDStaticFLag;
    int iSimStaticFlag;
    int iACCNetStaticFlag;
    int iOperInfoStaticFlag;
    int iVersionStaticFlag;
    int iEnodebIDStaticFlag;
    int iTacStaticFlag;
    int iLcidStaticFlag;
    int iCellIDStaticFlag;
    int iAllCidStaticFlag;
    int iMacStaticFlag;
    int iSimRandStaticFlag;
    int iSimUnlockStateFlag;
}LTE_STATE_INFO_T;


typedef union
{
    LTE_PIN_STATE_T stPinStateInfo;
    LTE_PIN_SWITCH_T stPinSwConfig;
    LTE_PIN_SW_STATE_T stPinSwInfo;
    LTE_NET_TYPE_T  stCurNetTypeInfo;
    LTE_NETWORK_PREPARE_T stNwStatusInfo;
	LTE_SIGNAL_T stSignalInfo;
    LTE_LINKSTATE_T stLinkInfo;
    LTE_IMEI_T   stImeiInfo;
    LTE_CELLID_T   stCellIdInfo;
    LTE_ICCID_T  stIccidInfo;
	LTE_IMSI_T   stImsiInfo;
    LTE_SIM_STATE_T stSimStateInfo;
    LTE_INFO_T      stLteInfo;
    LTE_ACCESS_NETWORK_T stAccessNetInfo;
    LTE_BANDS_T  stBandsInfo;
    LTE_CLKCELL_ENABLE_T   stClckCellAbleInfo;
    LTE_OPER_INFO_T stOperInfo;
    LTE_NET_TYPE_T  stSupNetTypeInfo;
    LTE_SMS_RECV_T  stSmsRecvInfo;
    LTE_SUPPORT_SMS_T stSupSmsInfo;
    LTE_CGDCONT_T   stCgdContConfig;
    LTE_APN_T       stApnConfig;
    LTE_MODULE_VERSION_T   stVersionInfo;
    LTE_ENODB_ID_T  stEnodeBInfo;
    LTE_TAC_T       stTacInfo;
    LTE_LCID_T      stLcidInfo;
    LTE_ALL_CELLID_T  stAllcidInfo;
    LTE_MOUDLE_MAC_T stMacInfo;
    LTE_SIM_RANDOM_NUM_T stSimRandomNum;
    LTE_SIM_UNLOCK_STATE_T stSimUnlockState;
    LTE_IP_GATEWAY_DNS_T stIpGatewayDns;
    LTE_ALL_PLMN_MODE_T stAllPlmnMode;
    LTE_SIM_UNLOCK_PUK_T stPukUnlock;
    LTE_PIN_INPUT_REMAIN_T stPinInputRemain;
    LTE_POST_ROUTE_SW_T stPostRouteSw;
	LTE_QOS_DATA_S      stQosData;
	LTE_DLOAD_PROT_VER_GET_T stDloadProtVer;
    LTE_DLOAD_VER_INFO_GET_T stDloadVerInfo;
    LTE_NV_BACKUP_T stNvBackup;
    LTE_NV_RESTORE_T stNvRestore;
    LTE_SIM_SWITCH_T stSimSwitch;
}LTE_SUB_MSG_U;

typedef struct __lte_msg_t{
	
	LTE_SUB_MSG_U	uLteCommand;

	LTE_CMD_E 	enLteCmd;
    int iResult;

}LTE_MSG_T;
typedef struct __lte_send_msg_t{
    
    LTE_SUB_MSG_U   uLteCommand;

    int iResult;
}LTE_SEND_MSG_T;

typedef struct __lte_com_msg{

    char acBuf[LTE_COM_MSG_LEN];
    struct __lte_com_msg *pstNext;
    
}LTE_COM_MSG_T;

typedef struct __lte_buf_file{

    char acBuf[LTE_COM_MSG_LEN];
    int  iFlag;
}LTE_BUF_FILE_T;


#define IP_FMT	    "%u.%u.%u.%u"
#define IP_ARG(x) ((UINT8*)(x))[0], ((UINT8*)(x))[1], ((UINT8*)(x))[2], ((UINT8*)(x))[3]
#define IP_SSCAN_ARG(x) &((UINT8*)(x))[0], &((UINT8*)(x))[1], &((UINT8*)(x))[2], &((UINT8*)(x))[3]
#define IPV6_FMT	"%u.%u.%u.%u.%u.%u.%u.%u.%u.%u.%u.%u.%u.%u.%u.%u"
#define IPV6_ARG(x) ((UINT8*)(x))[0], ((UINT8*)(x))[1], ((UINT8*)(x))[2], ((UINT8*)(x))[3],\
                    ((UINT8*)(x))[4], ((UINT8*)(x))[5], ((UINT8*)(x))[6], ((UINT8*)(x))[7], \
                    ((UINT8*)(x))[8], ((UINT8*)(x))[9], ((UINT8*)(x))[10], ((UINT8*)(x))[11],\
                    ((UINT8*)(x))[12], ((UINT8*)(x))[13], ((UINT8*)(x))[14], ((UINT8*)(x))[15]
                    
#define IPV6_SSCAN_ARG(x) &((UINT8*)(x))[0], &((UINT8*)(x))[1], &((UINT8*)(x))[2], &((UINT8*)(x))[3],\
                    &((UINT8*)(x))[4], &((UINT8*)(x))[5], &((UINT8*)(x))[6], &((UINT8*)(x))[7], \
                    &((UINT8*)(x))[8], &((UINT8*)(x))[9], &((UINT8*)(x))[10], &((UINT8*)(x))[11],\
                    &((UINT8*)(x))[12], &((UINT8*)(x))[13], &((UINT8*)(x))[14], &((UINT8*)(x))[15]

#endif  /* __LTE_COM_H__ */


