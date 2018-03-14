
#ifndef __LTE_H__
#define __LTE_H__
#include "pdu.h"

#define LTEGPSINFO "/tmp/gps.info"
#define LTEGPGGA   "/tmp/gps.gpgga"
/*altered by caibing 07-13*/
//#define AT_COM  "/dev/ttyUSB20"
#define AT_COM1  "/dev/ttyUSB1"
#define AT_COM2  "/dev/ttyUSB2"
#define AT_COM3  "/dev/ttyUSB3"

#define AC_CW_GPS_HEAD                                    0   /*GPS ͷ"$GPRMC" */
#define AC_CW_GPS_DATE                                      1   /*ʱ����*/
#define AC_CW_GPS_State                                         2   /*gps ��λ*/
#define AC_CW_GPS_LATITUDE                               3   /*gps γ��ֵ*/
#define AC_CW_GPS_LATITUDE_DIRECTION           4   /*gps γ��ֵ*/
#define AC_CW_GPS_LONGITUDE                              5   /*gps ����ֵ*/
#define AC_CW_GPS_LONGITUDE_DIRECTION          6   /*gps ����ֵ*/
#define AC_CW_GPS_BUS_VELOCITY                          7   /*�����ٶ�*/
#define AC_CW_GPS_ORIENTATION                           8   /*������ʻ�еķ�λ��*/
#define AC_CW_GPS_DATE_TIME                               9     /*������*/
#define GPS_DECODER_PARA_NUM                            15  /*�����������ĸ���*/

#define LTE_LCID_MASK   0x000000ff

#define AT_MAX_EXECUTE_COUNT    2  /* ATָ�����ִ�д��� */

#define LTE_MODULE_VER_PATH     "/tmp/ltemodulever"


#define LTE_CHINA_MOBILE_CODE      "46000,46002,46007"
#define LTE_CHINA_UNICOM_CODE      "46001,46006"
#define LTE_CHINA_TELECOM_CODE     "46003,46005,46011"

#define LTE_CHINA_MOBILE_NAME      "CHINA MOBILE"
#define LTE_CHINA_UNICOM_NAME      "CHINA UNICOM"
#define LTE_CHINA_TELECOM_NAME     "CHINA TELECOM"

#define LTE_SERVER_SOCKET_NAME "/var/run/ltecomsev"
#define LTE_CLIENT_SOCKET_NAME "/var/run/lteclient"

#define LTE_SMS_RECV_FILE       "/tmp/smsrecv"

#define LTE_SMS_SEND_FILE       "/tmp/smssendfile"
#define LTE_LCTCELLINFO_FILE       "/tmp/ltecellinfo"
/*���ܿ���Կ�ļ�*/
#define LTE_SIM_SECRET_KEY_FILE    "/tmp/simsecretkey"



/*���ܿ���Կ�ļ����ùؼ���*/
#define LTE_SIM_MODE_SELECT   "sim_mode_select:"
#define LTE_SIM_MAN_ID        "man_id:"
#define LTE_SIM_KEY_VER       "key_ver:"
#define LTE_SIM_SECRET_KEY    "secret_key:"



#if 0
typedef enum {
    GPS_OTHER_DATA = 0,
    GPS_GPRMC_DATA,
    GPS_GPGGA_DATA     
}LTE_GPS_DATA_TYPE;
#endif
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

typedef enum {
	LTE_FAIL = 0,
	LTE_OK,
	LTE_WAIT
}LTE_RET_E;

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
    /* BEGIN: Added by wangqi, 2015/5   PN:���ܿ��ӿ�  */
    LTE_SIM_UNLOCK,
    LTE_SIM_DISABLE_UNLOCK,
    LTE_SIM_UNLOCK_SUPPORT,
    LTE_SIM_TYPE_GET,/* ��ȡSIM������:0-��ͨ����1-���ܿ� */
    LTE_SIM_UNLOCK_GET,
    /* END: Added by wangqi, 2015/5   PN:���ܿ��ӿ�  */
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
typedef enum
{
    LTE_2G_ONLY = 0x0001,
    LTE_3G_ONLY = 0x0002,
    LTE_4G_ONLY = 0x0004,
    LTE_2G_3G = 0x0008,
    LTE_3G_4G = 0x0010,
    LTE_2G_3G_4G = 0x0020,
}LTE_NET_TYPE_E;
typedef enum
{
    LTE_NONE = 0x0000,
    LTE_2G_PRI = 0x0001,
    LTE_3G_PRI = 0x0002,
    LTE_4G_PRI = 0x0004,
    LTE_AUTO = 0x0008,
}LTE_NET_TYPE_PRI_E;


/* structs */
typedef enum
{
     CONNECT_MANUAL = 0,
     CONNECT_AUTO
}LTE_CONNECT_MODE_E;
typedef struct 
{
	char acVersion[128];/* ʵ�ʰ汾�� */
	int iVersionIndex;  /* �汾������ */
}LTE_MODULE_VERSION_T;

typedef struct 
{
	char acModuleTypeInd[32];  /* ģ���ͺ����� */
	char acVersion[256];/* ʵ�ʰ汾�� */
}LTE_MODULE_TYPE_T;

typedef struct 
{
    LTE_CONNECT_MODE_E enMode;			 /*1��ʾ�����Զ����ţ�0��ʾ�������Զ�����*/
}LTE_CONNECT_MODE_T; 

typedef enum
{
    PIN_STATE_READY = 0,
    PIN_STATE_PIN = 1,
    PIN_STATE_PUK = 2,
    PIN_SIM_BUSY = 3
}LTE_PIN_STATE_E;

typedef enum
{
    PIN_CLOSE = 0,
    PIN_OPEN = 1
}LTE_PIN_SW_STATE_E;

typedef struct
{
    LTE_PIN_STATE_E enState; /* 0Ϊready״̬��1Ϊpin״̬��2Ϊpuk״̬ */
}LTE_PIN_STATE_T;
typedef struct
{
    LTE_PIN_SW_STATE_E enState; /* 0ΪPIN�ر�״̬��1Ϊpin����״̬ */
}LTE_PIN_SW_STATE_T;


typedef enum
{
    PIN_SWITCH_OFF = 0,
    PIN_SWITCH_ON = 1
}LTE_PIN_SWITCH_E;

typedef enum
{
    CLC_BAND_DISABLE = 0,
    CLC_BAND_ENABLE = 1,/*����*/
    CLC_BAND_ENABLE_ONE = 2 /*����*/
}LTE_CLC_BAND_E;

typedef struct 
{
    char acPin[5];  /* pin�� */
    LTE_PIN_SWITCH_E enSwitch;    /* 0��ʾ�ر�Pin�룬1��ʾ����PIN�빦�� */
} LTE_PIN_SWITCH_T;

typedef struct 
{
    char acPin[5];  /* PIN�� */
} LTE_PIN_T;


typedef struct 
{
	unsigned int uiPinNum;    /* PIN��ʣ����� */
	unsigned int uiPukNum;    /* PUK��ʣ����� */
}LTE_PIN_ATTEMPTS_T;


typedef enum
{
	PIN_AUTO_OFF = 0,
	PIN_AUTO_ON
}LTE_PIN_MODE_E;

typedef struct 
{
	LTE_PIN_MODE_E enMode;  /* 0��ʾ�ر��Զ�PIN�빦�ܣ�1��ʾ�����Զ�PIN�빦�� */
}LTE_PIN_MODE_T;


typedef struct 
{
    char acOldPin[5];   /* �޸�ǰ��PIN�� */
    char acNewPin[5];   /* �޸ĺ��PIN�� */
} LTE_PIN_ALTER_T;


typedef struct 
{
    int iCfunFlag;  /* cfun��־��0��û������Э��ջ��1���Ѿ�����Э��ջ */
} LTE_CFUN_QUERY_T;

typedef struct 
{
    char acApn[32];     /* APN */
} LTE_APN_T;

typedef enum
{
  LTE_CONNECT_IPV4_TYPE = 0x04,
  LTE_CONNECT_IPV6_TYPE = 0x06,
  LTE_CONNECT_UNSPEC_TYPE = 0x08,
  LTE_CONNECT_IPV4V6_TYPE = 0x10  //added by qiao to extern, qulcomm not support.
} LTE_CONNECT_IP_FAMILY;

typedef struct 
{
    int  iIpFamily;
    char acApn[32];     /* APN */
} LTE_CONNECT_PARAM_T;

typedef enum
{
	PDP_IPV4 = 0,
	PDP_IPV6,
	PDP_IPV4V6
}LTE_CGDCONT_E;
typedef struct 
{
    LTE_CGDCONT_E enPdpStack;   /*0, IPV4��ջ��1��ipV6��ջ��2��IPV4��V6˫ջ */
    int iApnFlag;               /* 0��������apn��1������apn */
    char acApn[32];     /* APN */
} LTE_CGDCONT_T;

typedef struct 
{
    char acNetWorkStatus[32];     /* ����״̬ */
} LTE_NETWORK_PREPARE_T;
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
    unsigned int uiSignal;    /* �ź�ֵ */
} LTE_SIGNAL_T;

typedef struct 
{
    char acLinkSwitch[32];  /* on��ʾ��ʱ�رգ�off��ʾ��ʱ���� */
    int iTimer;   /* ��ʱʱ�� */
} LTE_LINKTIMER_T;

typedef struct 
{
    char acLinkSwitch[32];      /* on��ʾ�رն�ʱ�������ܣ�off��ʾ�رն�ʱ��
                                   �չ��� */
} LTE_DISLINKTIMER_T;

typedef enum
{
	LTE_LINKSTATE_DISCONNECT = 0,
	LTE_LINKSTATE_CONNECTING,
	LTE_LINKSTATE_CONNECTED,
	LTE_LINKSTATE_CONNECT_FAIL
}LTE_LINKSTATE_E;

typedef struct 
{
    LTE_LINKSTATE_E enLinkstate;   /* ��·״̬ */
} LTE_LINKSTATE_T;

typedef struct 
{
   LTE_NET_TYPE_E enNetType;    /* ������ʽ ��3G�ͽ�4G*/
   LTE_NET_TYPE_PRI_E enNetTypePri;
} LTE_NET_TYPE_T;

typedef struct 
{  
    int iCurrentCell;    /* ����ȽϵĽ�� */
}LTE_CELL_T;

typedef struct 
{
    int iMode;/*ģʽѡ�� 1Ϊ�Զ� 0Ϊ�ֶ�*/
    int iFormat;/*��ʾ��ʽ*/
    char acOper[32];/*�����̱�ʶ*/
    char cAct;/*���뼼��*/
}LTE_PLMN_T;


typedef struct 
{
    int iStatus;		/*����״̬*/
    char acChinese[32]; /*���ı���*/
    char acEnglish[32];   /*Ӣ������*/
    char acNuberic[32];  /*����˵��*/
    int iAct;			 /*���뼼��*/
}LTE_SIM_CARRIER_T;

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

typedef enum {
	LTE_V33 = 1001,
	LTE_V38
}LTE_VOL_E;

/*����������*/
typedef struct 
{
	unsigned short usVendor;    /*���̱�ʶ��*/
	unsigned short usProduct;   /*���̲�Ʒ��ʾ��*/
}LTE_DEVICE_DESCRIPTOR;

typedef struct 
{
	LTE_PLATFORM_E enDeviceNum;
	LTE_DEVICE_DESCRIPTOR stDeviceDesc;

	LTE_VOL_E enVoltage;
	char acName[32];
}LTE_DEVICE_INFO;
/* BEGIN: Added by liyoukun, 2014/9/24   PN:gpsģ�黯 */
typedef struct 
{
	LTE_PLATFORM_E enDeviceNum;
	LTE_DEVICE_DESCRIPTOR stDeviceDesc;

    int  iBaudRates;
    char acGpsCom[32];
	char acName[32];
}GPS_DEVICE_INFO;
typedef struct
{
    int iComFd;
    int iTimeOut;
    int iTest; /* �Ƿ�Ϊ����ʱʹ�ã�1Ϊ���⣻0Ϊ����GPS���ݻ�ȡ */
    
}GPS_BASE_CONGFIG_T;
/* END:   Added by liyoukun, 2014/9/24   PN:gpsģ�黯 */
/* BEGIN: Added by liyoukun, 2014/12/15   PN:ģ������ */
typedef struct
{
    char acUsrName[32];
    char acPassWD[32];
    char acServerAdd[32];/* ���ʮ����IP��ַ����xxx.xxx.xxx.xxx */
    char acServerPath[32];/* ģ��汾���ڷ������ϵ�·�� */
}LTE_SERVER_USR_T;
typedef struct
{
    LTE_PLATFORM_E enDeviceNum;

    int  iBaudRates;
    char acUpdCom[32];
    char acName[32];
}LTE_MODULE_UP_T;
/* END:   Added by liyoukun, 2014/12/15   PN:ģ������ */

typedef enum
{
	LTE_SIM_NOT_FOUND = 0,
	LTE_SIM_READY,
}LTE_SIM_STATE_E;

typedef struct
{
	LTE_SIM_STATE_E enSimState;
}LTE_SIM_STATE_T;

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
	LTE_CELLINFO_T stCellInfo;
}LTE_INFO_T;

typedef enum {
	NET_NONE = 0,
    NET_2G,
	NET_3G,
	NET_4G
}LTE_ACCESS_NETWORK_E;

typedef struct
{
	LTE_ACCESS_NETWORK_E enAccessNet;
}LTE_ACCESS_NETWORK_T;
typedef struct
{
    int iClkCellEnable;
}LTE_CLKCELL_ENABLE_T;
typedef struct
{
	/*Add by zhanghao 2016/7/22 begin */
	int iLockMode;          /*0. ������1. ��Ƶ�Σ�2. ��Ƶ�Σ�3. ��С��   ����Ի�Ϊģ��*/    
	char acBandStart[32];	/* ����С��������ʼƵ�� */
    char acBandEnd[32];		/*����С��������ֹƵ��*/
	/*Add by zhanghao 2016/7/22 end */
	char acCellid[32];  /* ����С��id */
    char acFreq[32];    /* ����С��ID����Ƶ�� */
    int  iClkEnable;    /* 0��������1����С�� */
}LTE_CLK_CELL_T;

typedef struct
{
	char bands[32];
    int iClcBandAble;/* �Ƿ���Ƶ */
}LTE_BANDS_T;

typedef struct
{
	char  acDate[32];
    char  acTime[32];
    char  acLongitude[32];
    char  acLatitude[32];
    char  acVelocity[32];
    char  acOrientation[32];
    char  acElevation[32];
    int   iGpsDataIsValid; /* 0����Ч���ݣ�1����Ч���� */
}LTE_GPS_INFO_T;
typedef struct
{
    char   acDate[32];
}LTE_GPS_DATE_T;
typedef struct
{
    char  acLongitude[32];
    char  acLatitude[32];
    char  acVelocity[32];
    char  acOrientation[32];
}LTE_GPS_POSTION_T;

typedef struct
{
    char acOperName[32];/* ��Ӫ������ */
   // int iOperFlag;                 /* �Ƿ�֧�ֻ�ȡ */
}LTE_OPER_INFO_T;
/* BEGIN: Added by wujian, 2014/11/12   PN:��ӻ�ȡ֧�ֵ��������ͣ���֧��2g��3g��4g */
#if 0
typedef struct
{
    int iSmsMode; //1���ı�ģʽ��0��ʾpduģʽ
}LTE_SMS_Mode_T;

typedef struct
{
    char    acSmsCentCode[LTE_SMS_CENT_NUM]; //�������ĺ���
}LTE_SMS_CENT_CODE_T;

typedef struct
{
    char acRecvMsgCode[LTE_SMS_MOBILE_NUM];  //���ն˵绰����
    char acMsgCont[LTE_SMS_CONTENT_MAX];     //��������
}LTE_SMS_SEND_T;

typedef struct
{
    SMS_INFO stRecvCont[LTE_SMS_COUNT_MAX];
    int iRecvFlag; //0����ȡ���еĶ���;����1�������ʾ�洢�Ķ��ŵ����������ڼ�������
}LTE_SMS_RECV_T;
typedef struct
{
    char acSmsRecvFile[32];/* ���ն��Ŵ�ŵ��ļ�·�� */
}LTE_SMS_RECV_SRC_T;

typedef struct
{
    int iDelIndex;          //0��ɾ�����ж���;����1��Ϊɾ���ڼ�������
}LTE_SMS_DEL_T;
#endif
typedef enum
{
    NO_SUP_SMS_FUNC = 0,
    SUP_SMS_FUNC = 1
}LTE_SUPPORT_SMS__E;

typedef struct
{
    LTE_SUPPORT_SMS__E enSupSms;/*1 ֧�ֶ��Ź��ܣ�0 ��֧��*/
}LTE_SUPPORT_SMS_T;
/* END:   Added by wujian, 2014/11/12   PN:��Ӷ��Ź���������ݽṹ */

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

typedef struct 
{  
    int man_id; /* �������̴��� ��SIM�����ṩ */
    int key_ver;/* ��Կ�汾�ţ� SIM�����ṩ*/
    char acSecretKey[512];/* ��Կ��,ֱ�������������� */
} LTE_SIM_SECRET_KEY_T;

typedef enum
{
    NORMAL_ENCRYPTION_SIM = 0,/*��ͨ���ͼ��ܿ�������*/
    NORMAL_SIM_ONLY = 1,/* ����ͨSIM����*/
    ENCRYPTION_SIM_ONLY = 2/* ������SIM�� */
}LTE_SIM_MODE_SELECT_E;

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
    char acSignalName[32];
    int iSignalValue;
} LTE_SIGNAL_VALUE_T;

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

typedef enum
{
    MODULE_DISABLE_UP = 0,/*��֧��ģ�����豸������*/
    MODULE_SUPPORT_UP = 1 /* ֧��ģ�����豸������*/
} LTE_MODULE_UP_SUPPORT_E;

typedef struct 
{
    LTE_MODULE_UP_SUPPORT_E iModuleUpFlag;  /* 0-��֧��ģ�����豸��������1-֧��ģ�����豸������ */
} LTE_MODULE_UP_SUPPORT_T;

typedef struct
{
    int iPinIputRemain;  /* ʣ��PIN��������� */
}LTE_PIN_INPUT_REMAIN_T;

typedef enum
{
    POST_ROUTE_CLOSE = 0,/*�رպ�·�ɹ���*/
    POST_ROUTE_OPEN = 1 /*�򿪺�·�ɹ���*/
} LTE_POST_ROUTE_SW_E;

typedef struct 
{
    LTE_POST_ROUTE_SW_E iPostRouteSw; 
} LTE_POST_ROUTE_SW_T;
typedef struct
{
    char acSwer[32];
    char acIosVer[32];
    char acProductName[32];
    char acDloadType[32];
} LTE_DLOAD_PROT_VER_T; 
typedef struct 
{
    char acDloadVerInfo[16]; 
} LTE_DLOAD_VER_INFO_T;
typedef struct 
{
    char acNvDatalock[32]; 
} LTE_NV_DATALOCK_T;
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

/******************************��Ϲ���***************************/
typedef enum 
{
    LTE_E_NONE,                  /* 0, �Ѿ����Ϻţ�������·ͨ */
    LTE_E_DNS,                   /* DNS������ */
    LTE_E_NO_MODULE,             /* û�з���ģ�飬û�ҵ�PID,VID */
    LTE_E_NO_ATTACHED,           /* cat /prob/bus/usb/device û�ҵ��豸PID,VID */
    LTE_E_NO_NET_INTERFACE,      /* û������ */
    LTE_E_NO_LTECOMSEV,          /* 5,ltecomsev���̲����� */
    LTE_E_NO_SIM,                /* SIM��û�м�⵽ */
    LTE_E_SIM_PUK,               /* SIM������PUK״̬ */
    LTE_E_SIGNAL_LOW,            /* û���ź� */
    LTE_E_RESIDENT_NET_FAIL,     /* פ������ʧ�� */
    LTE_E_NO_DIAL,               /* 10,û�в��� */
    LTE_E_NO_DEFAULT_ROUTE,      /* û��Ĭ��·�� */
    LTE_E_DEFAULT_FAIL,          /* default failure*/
} LTE_DIAGNOSE_STATE_E;

/*****************************��Ϲ���****************************/

/* ��ƽ̨��ͨ��ʹ�� */
typedef enum {
    WAIT_WRITE,         //�ȴ�д
    NOT_NEED_WRITE,     //����Ҫд
    WAIT_READ,          //д���ȴ���
    WRITE_DONE,         //�ڵ������ɣ����д�Ͷ�����
}LTE_NODE_SM_E;

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
}CLIENT_SEND_MSG_T;
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
	LTE_CELLID_T stCellIdInfo;
	LTE_ICCID_T  stIccidInfo;
	LTE_IMSI_T   stImsiInfo;
	LTE_SIM_STATE_T stSimStateInfo;
	LTE_INFO_T      stLteInfo;
	LTE_ACCESS_NETWORK_T stAccessNetInfo;
	LTE_BANDS_T  stBandsInfo;
	LTE_CLKCELL_ENABLE_T   stClckCellAbleInfo;
	LTE_OPER_INFO_T stOperInfo;
	LTE_NET_TYPE_T  stSupNetTypeInfo;
    LTE_SMS_RECV_SRC_T  stSmsRecvInfo;
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
	LTE_DLOAD_PROT_VER_T stDloadProtVer;
    LTE_DLOAD_VER_INFO_T stDloadVerInfo;
    LTE_NV_BACKUP_T stNvBackup;
    LTE_NV_RESTORE_T stNvRestore;
    LTE_SIM_SWITCH_T stSimSwitch;
}LTE_SUB_MSG_U;

typedef struct __lte_send_msg_t{
    
    LTE_SUB_MSG_U   uLteCommand;

    int iResult;
}LTE_SEND_MSG_T;

extern LTE_RET_E lte_mutex_lock(void);
extern LTE_RET_E lte_mutex_unlock(void);
extern LTE_RET_E lte_init(int iLogLevel);
extern LTE_RET_E lte_ioctl(int iCmd, void *pvData, int iDataLen);

extern LTE_RET_E lte_pin_state_get(LTE_PIN_STATE_T *pstState);
extern LTE_RET_E lte_pin_switch(LTE_PIN_SWITCH_T *pstSwitch);
extern LTE_RET_E lte_pin_sw_get(LTE_PIN_SW_STATE_T *pstPinSwState);
extern LTE_RET_E lte_pin_unlock(LTE_PIN_T *pstPin);
extern LTE_RET_E lte_pin_attempts(LTE_PIN_ATTEMPTS_T *pstAttempts);
extern LTE_RET_E lte_pin_alter(LTE_PIN_ALTER_T *pstAlter);
extern LTE_RET_E lte_signal_get(LTE_SIGNAL_T *pstSignal);
extern LTE_RET_E lte_power_on(void);
extern LTE_RET_E lte_power_off(void);
extern LTE_RET_E lte_net_type_set(LTE_NET_TYPE_T *pstType);
extern LTE_RET_E lte_net_type_get(LTE_NET_TYPE_T *pstType);
extern LTE_RET_E lte_plmn_register(LTE_PLMN_T *pstPlmn);
extern LTE_RET_E lte_prefer_plmn_set(void);
extern LTE_RET_E lte_network_prepare_get(LTE_NETWORK_PREPARE_T *pstNWStatus);
extern LTE_RET_E lte_network_status_get(LTE_NETWORK_PREPARE_T *pstNWStatus);
extern LTE_RET_E lte_connect(LTE_APN_T *pstApn, int check_switch);
extern LTE_RET_E lte_connect_async(LTE_APN_T *pstApn, int check_switch);
extern LTE_RET_E lte_disconnect(void);
extern LTE_RET_E lte_disconnect_ipv6(char *acIpFamily);
extern LTE_RET_E lte_disconnect_async(void);
extern LTE_RET_E lte_disconnect_ipv6_async(char *acIpFamily);
extern LTE_RET_E lte_linkstate_get(LTE_LINKSTATE_T *pstState);
extern LTE_RET_E lte_imei_get(LTE_IMEI_T *pstImei);
extern LTE_RET_E lte_cellid_get(LTE_CELLID_T *pstCellId);
extern LTE_RET_E lte_iccid_get(LTE_ICCID_T *pstICCID);
extern LTE_RET_E lte_sim_state_get(LTE_SIM_STATE_T *pstSimState);
extern LTE_RET_E lte_info_get(LTE_INFO_T *pstLteInfo);
extern LTE_RET_E lte_access_net_get(LTE_ACCESS_NETWORK_T *pstAccessNet);
extern LTE_RET_E lte_bands_get(LTE_BANDS_T *pstBands);
extern LTE_RET_E lte_clck_bands_set(LTE_BANDS_T *pstBands);
extern LTE_RET_E lte_clck_bands_close(void); /*�ָ�Ĭ����Ƶ*/
extern LTE_RET_E lte_clck_cell_set(LTE_CLK_CELL_T *pstCell);
extern LTE_RET_E lte_clck_cell_enable_get(LTE_CLKCELL_ENABLE_T *pstCellEnableFlag);

extern LTE_RET_E lte_device_info_get(LTE_DEVICE_INFO *pstDevInfo);

extern LTE_RET_E lte_gps_func_start(void);
extern LTE_RET_E lte_gps_func_stop(void);
extern LTE_RET_E lte_gps_date_get(LTE_GPS_DATE_T *pstGpsDate);
extern LTE_RET_E lte_gps_postion_get(LTE_GPS_POSTION_T *pstGpsPosInfo);
extern LTE_RET_E gps_serial_com_open(int *piComFd);
extern LTE_RET_E gps_serial_com_close(int iComFd);
/* ���ùر�GPS�Ľӿ�
 * int iGpsCloseFlag  �Ƿ�ر�GPS�ı�ʶ
 */
extern LTE_RET_E set_gps_close_tag(int iGpsCloseFlag);

/* ���ùر�GPS�Ľӿ�
 * int *piGpsCloseFlag  GPS�Ƿ�رյı�ʶ
 */
extern LTE_RET_E get_gps_close_tag(int *piGpsCloseFlag);

/* ����GPS����Ľӿ� */
extern LTE_RET_E lte_gps_test(void);

/* ��ȡGPS��Ч����
 * int iCOmFd    ��GPS COM�ڷ��ص�FD
 * int iTimeOut  ��COM��ֱ�ӻ�ȡGPS���ݵĳ�ʱʱ��
 * LTE_GPS_INFO_T *pstLteGpsInfo �����󷵻ص�GPS���ݣ�����ʱ�䡢���ȡ�γ�ȡ�����ǡ��ٶȡ�����
 */
extern LTE_RET_E gps_valid_data_get(int iComFd, int iTimeOut, LTE_GPS_INFO_T *pstLteGpsInfo);
/* BEGIN: Added by wujian, 2014/11/10   PN:��ӻ�ȡ��Ӫ����Ϣ�ӿ� */
extern LTE_RET_E lte_oper_info_get(LTE_OPER_INFO_T *pstOperInfo);
/* END:   Added by wujian, 2014/11/10   PN:��ӻ�ȡ��Ӫ����Ϣ�ӿ� */

/* BEGIN: Added by wujian, 2014/11/12   PN:��ӻ�ȡ֧�ֵ��������ͣ���֧��2g��3g��4g */
extern LTE_RET_E lte_sup_nettype_get(LTE_NET_TYPE_T *pstNetsInfo);
/* END:   Added by wujian, 2014/11/12   PN:��ӻ�ȡ֧�ֵ��������ͣ���֧��2g��3g��4g */

/* BEGIN: Added by wujian, 2014/11/13   PN:��Ӷ��Ź�����ؽӿ� */
extern LTE_RET_E lte_sms_cent_code_set(LTE_SMS_CENT_CODE_T *pstCentCode);
extern LTE_RET_E lte_sms_mode__set(LTE_SMS_Mode_T *pstSmsMode);
extern LTE_RET_E lte_sms_func_init(void);
extern LTE_RET_E lte_sms_recv(LTE_SMS_RECV_T *pstRecv);
extern LTE_RET_E lte_sms_send(LTE_SMS_SEND_T *pstSend);
extern LTE_RET_E lte_sms_dele(LTE_SMS_DEL_T *pstDele);
extern LTE_RET_E lte_sms_sup_func(LTE_SUPPORT_SMS_T *pstSupSms);
/* END:   Added by wujian, 2014/11/13   PN:��Ӷ��Ź�����ؽӿ� */
extern LTE_RET_E lte_pdp_cgdcont_set(LTE_CGDCONT_T *pstPdpSet);
/* BEGIN: Added by wujian, 2014/11/17   PN:�������apn�ӿ� */
extern LTE_RET_E lte_apn_set(LTE_APN_T *pstApn);

/* END:   Added by wujian, 2014/11/17   PN:�������apn�ӿ� */
extern LTE_RET_E lte_version_get(LTE_MODULE_TYPE_T *pstModuleType);
extern LTE_RET_E lte_cifs_mount(LTE_SERVER_USR_T *pstSmbServerInfo);
extern LTE_RET_E lte_up_file_transfer(char *pcSrcFileName, char *pcDstFileName);
extern LTE_RET_E lte_update(char *pcSrcFileName);
extern LTE_RET_E lte_enodeb_id_get(LTE_ENODB_ID_T *pstEnodeId);
extern LTE_RET_E lte_tac_get(LTE_TAC_T *pstTac);
extern LTE_RET_E lte_lcid_get(LTE_LCID_T *pstLcid);
extern LTE_RET_E lte_all_cellid_get(LTE_ALL_CELLID_T *pstAllCid);
extern LTE_RET_E lte_moudle_mac_set(LTE_MOUDLE_MAC_T *pstMac);
extern LTE_RET_E lte_moudle_mac_get(LTE_MOUDLE_MAC_T *pstMac);
extern LTE_RET_E lte_moudle_sim_unlock(LTE_SIM_SECRET_KEY_T *pcSecretKey);
extern LTE_RET_E lte_moudle_sim_disable_unlock(void);
extern LTE_RET_E lte_moudle_sim_unlock_support(LTE_UNLOCK_SUPPORT_T *pstUnlockSupport);
extern LTE_RET_E lte_moudle_sim_type_get(LTE_SIM_TYPE_E *penSimType);
extern LTE_RET_E lte_moudle_sim_unlock_get(LTE_SIM_UNLOCK_STATE_T *pstSimUnlockState);
extern LTE_RET_E lte_signal_value_get(LTE_SIGNAL_VALUE_T *pstSignalValue);
extern LTE_RET_E lte_ip_gateway_dns_get(LTE_IP_GATEWAY_DNS_T *pstIpGwDns);
extern LTE_RET_E lte_all_plmn_mode_get(LTE_ALL_PLMN_MODE_T *pstAllPlmnMode);
extern LTE_RET_E lte_sounet_mode_set(LTE_SOUNET_MODE_T *pstSounetMode);
extern LTE_RET_E lte_sim_puk_unlock(LTE_SIM_UNLOCK_PUK_T *pstPukUnlock);
extern LTE_RET_E lte_moudle_up_support(LTE_MODULE_UP_SUPPORT_T *pstModuleUpSupp);
extern LTE_RET_E lte_pin_input_remain_get(LTE_PIN_INPUT_REMAIN_T *pstPinIputRemain);
extern LTE_DIAGNOSE_STATE_E lte_diagnose(void);
extern LTE_RET_E lte_post_route_set(LTE_POST_ROUTE_SW_T *pstPostRouteSw);
extern LTE_RET_E lte_nv_datalock(LTE_NV_DATALOCK_T *pstNvDatalock);
extern LTE_RET_E lte_post_route_get(LTE_POST_ROUTE_SW_T *pstPostRouteSw);
extern LTE_RET_E lte_remove_attach_set(void);
extern LTE_RET_E lte_qos_set(LTE_QOS_DATA_S*pstQosData);
extern LTE_RET_E lte_qos_get(LTE_QOS_DATA_S*pstQosData);
extern LTE_RET_E lte_dload_prot_ver_get(LTE_DLOAD_PROT_VER_T *pstDloadProtVer);
extern LTE_RET_E lte_dload_ver_info_get(LTE_DLOAD_VER_INFO_T *pstDloadVerInfo);
extern LTE_RET_E lte_nv_backup(LTE_NV_BACKUP_T *patNvBackup);
extern LTE_RET_E lte_nv_restore(LTE_NV_RESTORE_T *pstNvRestore);
extern LTE_RET_E lte_dload_mode(void);
extern LTE_RET_E lte_data_mode(void);
extern LTE_RET_E lte_reset(void);
extern LTE_RET_E lte_sim_switch_set(LTE_SIM_SWITCH_T *pstSimSwitch);
extern LTE_RET_E lte_sim_switch_get(LTE_SIM_SWITCH_T *pstSimSwitch);
extern LTE_RET_E lte_sim_switch_soft_set(LTE_SIM_SWITCH_T *pstSimSwitch);


#endif /* __LTE_H__ */

