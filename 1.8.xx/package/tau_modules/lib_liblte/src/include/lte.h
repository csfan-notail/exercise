
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

#define AC_CW_GPS_HEAD                                    0   /*GPS 头"$GPRMC" */
#define AC_CW_GPS_DATE                                      1   /*时分秒*/
#define AC_CW_GPS_State                                         2   /*gps 定位*/
#define AC_CW_GPS_LATITUDE                               3   /*gps 纬度值*/
#define AC_CW_GPS_LATITUDE_DIRECTION           4   /*gps 纬度值*/
#define AC_CW_GPS_LONGITUDE                              5   /*gps 经度值*/
#define AC_CW_GPS_LONGITUDE_DIRECTION          6   /*gps 经度值*/
#define AC_CW_GPS_BUS_VELOCITY                          7   /*汽车速度*/
#define AC_CW_GPS_ORIENTATION                           8   /*汽车行驶中的方位角*/
#define AC_CW_GPS_DATE_TIME                               9     /*年月日*/
#define GPS_DECODER_PARA_NUM                            15  /*最大解析参数的个数*/

#define LTE_LCID_MASK   0x000000ff

#define AT_MAX_EXECUTE_COUNT    2  /* AT指令最大执行次数 */

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
/*加密卡秘钥文件*/
#define LTE_SIM_SECRET_KEY_FILE    "/tmp/simsecretkey"



/*加密卡秘钥文件配置关键字*/
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
    /* BEGIN: Added by wujian, 2014/11/7   PN:添加获取运营商信息 */
    LTE_OPER_INFO_GET,
    /* END:   Added by wujian, 2014/11/7   PN:添加获取运营商信息 */
    /* BEGIN: Added by wujian, 2014/11/12   PN:添加获取支持的网络类型，如支持2g，3g，4g */
    LTE_NETTYPE_INFO_GET,
    /* END:   Added by wujian, 2014/11/12   PN:添加获取支持的网络类型，如支持2g，3g，4g */

    /* BEGIN: Added by wujian, 2014/11/13   PN:添加短信功能相关接口ID */
    LTE_SMS_CENT_CODE_SET,
    LTE_SMS_MODE__SET,
    LTE_SMS_FUNC_INIT,
    LTE_SMS_RECV,
    LTE_SMS_SEND,
    LTE_SMS_DELE,
    LTE_SMS_SUPP,
    /* END:   Added by wujian, 2014/11/13   PN:添加短信功能相关接口ID */
    LTE_IPV6_PDP_SET,
    LTE_APN_SET,
    LTE_VERSION_GET,
    LTE_ENODEBID_GET,
    LTE_TAC_GET,
    LTE_LCID_GET,
    LTE_ALL_CELLID_GET,
    LTE_MOUDLE_MAC_SET,
    LTE_MOUDLE_MAC_GET,
    /* BEGIN: Added by wangqi, 2015/5   PN:加密卡接口  */
    LTE_SIM_UNLOCK,
    LTE_SIM_DISABLE_UNLOCK,
    LTE_SIM_UNLOCK_SUPPORT,
    LTE_SIM_TYPE_GET,/* 获取SIM卡类型:0-普通卡，1-加密卡 */
    LTE_SIM_UNLOCK_GET,
    /* END: Added by wangqi, 2015/5   PN:加密卡接口  */
    LTE_SIGNAL_VALUE_GET,
    LTE_IP_GATEWAY_DNS_GET,
    LTE_ALL_PLMN_MODE_GET,
    LTE_SOUNET_MODE_SET,
    LTE_SIM_UNLOCK_PUK,
    LTE_MODULE_UP_SUPPORT,
    LTE_PIN_INPUT_REMAIN_GET,
    /*华为模块后路由功能*/
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
	char acVersion[128];/* 实际版本名 */
	int iVersionIndex;  /* 版本号索引 */
}LTE_MODULE_VERSION_T;

typedef struct 
{
	char acModuleTypeInd[32];  /* 模块型号索引 */
	char acVersion[256];/* 实际版本名 */
}LTE_MODULE_TYPE_T;

typedef struct 
{
    LTE_CONNECT_MODE_E enMode;			 /*1表示开机自动拨号，0表示开机不自动拨号*/
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
    LTE_PIN_STATE_E enState; /* 0为ready状态，1为pin状态，2为puk状态 */
}LTE_PIN_STATE_T;
typedef struct
{
    LTE_PIN_SW_STATE_E enState; /* 0为PIN关闭状态，1为pin开启状态 */
}LTE_PIN_SW_STATE_T;


typedef enum
{
    PIN_SWITCH_OFF = 0,
    PIN_SWITCH_ON = 1
}LTE_PIN_SWITCH_E;

typedef enum
{
    CLC_BAND_DISABLE = 0,
    CLC_BAND_ENABLE = 1,/*多锁*/
    CLC_BAND_ENABLE_ONE = 2 /*单锁*/
}LTE_CLC_BAND_E;

typedef struct 
{
    char acPin[5];  /* pin码 */
    LTE_PIN_SWITCH_E enSwitch;    /* 0表示关闭Pin码，1表示开启PIN码功能 */
} LTE_PIN_SWITCH_T;

typedef struct 
{
    char acPin[5];  /* PIN码 */
} LTE_PIN_T;


typedef struct 
{
	unsigned int uiPinNum;    /* PIN码剩余次数 */
	unsigned int uiPukNum;    /* PUK码剩余次数 */
}LTE_PIN_ATTEMPTS_T;


typedef enum
{
	PIN_AUTO_OFF = 0,
	PIN_AUTO_ON
}LTE_PIN_MODE_E;

typedef struct 
{
	LTE_PIN_MODE_E enMode;  /* 0表示关闭自动PIN码功能，1表示开启自动PIN码功能 */
}LTE_PIN_MODE_T;


typedef struct 
{
    char acOldPin[5];   /* 修改前的PIN码 */
    char acNewPin[5];   /* 修改后的PIN码 */
} LTE_PIN_ALTER_T;


typedef struct 
{
    int iCfunFlag;  /* cfun标志：0，没有启动协议栈；1，已经启动协议栈 */
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
    LTE_CGDCONT_E enPdpStack;   /*0, IPV4单栈；1，ipV6单栈；2，IPV4、V6双栈 */
    int iApnFlag;               /* 0，不设置apn；1，设置apn */
    char acApn[32];     /* APN */
} LTE_CGDCONT_T;

typedef struct 
{
    char acNetWorkStatus[32];     /* 网络状态 */
} LTE_NETWORK_PREPARE_T;
typedef struct 
{
    char acImei[32];    /* IMEI号 */
} LTE_IMEI_T;

typedef struct 
{
    char acCellID[32];    /* cell id */
} LTE_CELLID_T;
typedef struct 
{
    char acICCID[32];    /* ICCID值 */
} LTE_ICCID_T;
typedef struct 
{
    char acIMSI[32];    /* IMSI值 */
} LTE_IMSI_T;

typedef struct 
{
    unsigned int uiSignal;    /* 信号值 */
} LTE_SIGNAL_T;

typedef struct 
{
    char acLinkSwitch[32];  /* on表示定时关闭，off表示定时拨号 */
    int iTimer;   /* 定时时间 */
} LTE_LINKTIMER_T;

typedef struct 
{
    char acLinkSwitch[32];      /* on表示关闭定时开启功能，off表示关闭定时关
                                   闭功能 */
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
    LTE_LINKSTATE_E enLinkstate;   /* 链路状态 */
} LTE_LINKSTATE_T;

typedef struct 
{
   LTE_NET_TYPE_E enNetType;    /* 网络制式 仅3G和仅4G*/
   LTE_NET_TYPE_PRI_E enNetTypePri;
} LTE_NET_TYPE_T;

typedef struct 
{  
    int iCurrentCell;    /* 输出比较的结果 */
}LTE_CELL_T;

typedef struct 
{
    int iMode;/*模式选择 1为自动 0为手动*/
    int iFormat;/*显示格式*/
    char acOper[32];/*运行商标识*/
    char cAct;/*接入技术*/
}LTE_PLMN_T;


typedef struct 
{
    int iStatus;		/*返回状态*/
    char acChinese[32]; /*中文别名*/
    char acEnglish[32];   /*英文名称*/
    char acNuberic[32];  /*数字说明*/
    int iAct;			 /*接入技术*/
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
    /*华为4G模块存储模式*/
    M_HUAWEI4G_STORAGE,
    /*华为4G模块网卡模式*/
    M_HUAWEI4G_NETCARD,
	/* Donnot modify this value */
	M_TOTAL_NUM
}LTE_PLATFORM_E;

typedef enum {
	LTE_V33 = 1001,
	LTE_V38
}LTE_VOL_E;

/*厂商描述符*/
typedef struct 
{
	unsigned short usVendor;    /*厂商标识符*/
	unsigned short usProduct;   /*厂商产品标示符*/
}LTE_DEVICE_DESCRIPTOR;

typedef struct 
{
	LTE_PLATFORM_E enDeviceNum;
	LTE_DEVICE_DESCRIPTOR stDeviceDesc;

	LTE_VOL_E enVoltage;
	char acName[32];
}LTE_DEVICE_INFO;
/* BEGIN: Added by liyoukun, 2014/9/24   PN:gps模块化 */
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
    int iTest; /* 是否为产测时使用，1为产测；0为正常GPS数据获取 */
    
}GPS_BASE_CONGFIG_T;
/* END:   Added by liyoukun, 2014/9/24   PN:gps模块化 */
/* BEGIN: Added by liyoukun, 2014/12/15   PN:模块升级 */
typedef struct
{
    char acUsrName[32];
    char acPassWD[32];
    char acServerAdd[32];/* 点分十进制IP地址，如xxx.xxx.xxx.xxx */
    char acServerPath[32];/* 模块版本所在服务器上的路径 */
}LTE_SERVER_USR_T;
typedef struct
{
    LTE_PLATFORM_E enDeviceNum;

    int  iBaudRates;
    char acUpdCom[32];
    char acName[32];
}LTE_MODULE_UP_T;
/* END:   Added by liyoukun, 2014/12/15   PN:模块升级 */

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
	int iLockMode;          /*0. 不锁；1. 锁频段；2. 锁频段；3. 锁小区   仅针对华为模块*/    
	char acBandStart[32];	/* 物理小区接入起始频段 */
    char acBandEnd[32];		/*物理小区接入终止频段*/
	/*Add by zhanghao 2016/7/22 end */
	char acCellid[32];  /* 物理小区id */
    char acFreq[32];    /* 物理小区ID接入频点 */
    int  iClkEnable;    /* 0，解锁；1，锁小区 */
}LTE_CLK_CELL_T;

typedef struct
{
	char bands[32];
    int iClcBandAble;/* 是否锁频 */
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
    int   iGpsDataIsValid; /* 0，无效数据；1，有效数据 */
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
    char acOperName[32];/* 运营商名字 */
   // int iOperFlag;                 /* 是否支持获取 */
}LTE_OPER_INFO_T;
/* BEGIN: Added by wujian, 2014/11/12   PN:添加获取支持的网络类型，如支持2g，3g，4g */
#if 0
typedef struct
{
    int iSmsMode; //1表文本模式，0表示pdu模式
}LTE_SMS_Mode_T;

typedef struct
{
    char    acSmsCentCode[LTE_SMS_CENT_NUM]; //短信中心号码
}LTE_SMS_CENT_CODE_T;

typedef struct
{
    char acRecvMsgCode[LTE_SMS_MOBILE_NUM];  //接收端电话号码
    char acMsgCont[LTE_SMS_CONTENT_MAX];     //短信内容
}LTE_SMS_SEND_T;

typedef struct
{
    SMS_INFO stRecvCont[LTE_SMS_COUNT_MAX];
    int iRecvFlag; //0，获取所有的短信;大于1的数则表示存储的短信的索引，即第几条短信
}LTE_SMS_RECV_T;
typedef struct
{
    char acSmsRecvFile[32];/* 接收短信存放的文件路径 */
}LTE_SMS_RECV_SRC_T;

typedef struct
{
    int iDelIndex;          //0，删除所有短信;大于1则为删除第几条短信
}LTE_SMS_DEL_T;
#endif
typedef enum
{
    NO_SUP_SMS_FUNC = 0,
    SUP_SMS_FUNC = 1
}LTE_SUPPORT_SMS__E;

typedef struct
{
    LTE_SUPPORT_SMS__E enSupSms;/*1 支持短信功能，0 不支持*/
}LTE_SUPPORT_SMS_T;
/* END:   Added by wujian, 2014/11/12   PN:添加短信功能相关数据结构 */

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
    int iUnlockFlag;  /* 0，该模块没有SIM卡解锁模式；1，该模块有SIM卡解锁模式 */
} LTE_UNLOCK_SUPPORT_T;

typedef enum
{
    NORMAL_SIM = 0,/* 0，普通SIM卡；*/
    ENCRYPTION_SIM = 1/* 1，加密SIM卡 */
}LTE_SIM_TYPE_E;


typedef struct 
{
    LTE_SIM_TYPE_E iSIMType;  /* 0，普通SIM卡；1，加密SIM卡 */
    char cRandomNum[64];/* 20字节BCD随机数 */
} LTE_SIM_RANDOM_NUM_T;

typedef struct 
{  
    int man_id; /* 话机厂商代码 ，SIM厂商提供 */
    int key_ver;/* 秘钥版本号， SIM厂商提供*/
    char acSecretKey[512];/* 秘钥组,直接连接起来就行 */
} LTE_SIM_SECRET_KEY_T;

typedef enum
{
    NORMAL_ENCRYPTION_SIM = 0,/*普通卡和加密卡都可用*/
    NORMAL_SIM_ONLY = 1,/* 仅普通SIM卡；*/
    ENCRYPTION_SIM_ONLY = 2/* 仅加密SIM卡 */
}LTE_SIM_MODE_SELECT_E;

typedef enum
{
    SIM_NOT_UNLOCK = 0,/*SIM卡未解锁*/
    SIM_HAS_UNLOCK = 1 /* SIM卡已经解锁*/
}LTE_SIM_UNLOCK_STATE_E;


typedef struct 
{    
    LTE_SIM_UNLOCK_STATE_E iIsUnlock; /*0, SIM卡未解锁； 1，SIM卡已经解锁*/
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
    LTE_CGDCONT_E enPdpStack;   /*0, IPV4单栈；1，ipV6单栈；2，IPV4、V6双栈 */
    UINT8 acIpv4[4];
    UINT8 acGatewayV4[4];
    UINT8 acDns1V4[4];
    UINT8 acDns2V4[4];
    UINT8 acIpv6[16];
    UINT8 acGatewayV6[16];
    UINT8 acDns1V6[16];
    UINT8 acDns2V6[16];
} LTE_IP_GATEWAY_DNS_T;
/* 手动搜网 begin add by wangqi*/
typedef struct 
{    
    char acAllPlmn[256]; /*PLMN+制式，PLMN+制式，PLMN+制式*/
} LTE_ALL_PLMN_MODE_T;

typedef struct 
{    
    char acPlmn[16]; /*PLMN*/
    char acMode[16];
} LTE_SOUNET_MODE_T;

/* 手动搜网 end add by wangqi*/

typedef enum
{
    MODULE_DISABLE_UP = 0,/*不支持模块在设备上升级*/
    MODULE_SUPPORT_UP = 1 /* 支持模块在设备上升级*/
} LTE_MODULE_UP_SUPPORT_E;

typedef struct 
{
    LTE_MODULE_UP_SUPPORT_E iModuleUpFlag;  /* 0-不支持模块在设备上升级；1-支持模块在设备上升级 */
} LTE_MODULE_UP_SUPPORT_T;

typedef struct
{
    int iPinIputRemain;  /* 剩余PIN码输入次数 */
}LTE_PIN_INPUT_REMAIN_T;

typedef enum
{
    POST_ROUTE_CLOSE = 0,/*关闭后路由功能*/
    POST_ROUTE_OPEN = 1 /*打开后路由功能*/
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

/******************************诊断工具***************************/
typedef enum 
{
    LTE_E_NONE,                  /* 0, 已经拨上号，并且网路通 */
    LTE_E_DNS,                   /* DNS有问题 */
    LTE_E_NO_MODULE,             /* 没有发现模块，没找到PID,VID */
    LTE_E_NO_ATTACHED,           /* cat /prob/bus/usb/device 没找到设备PID,VID */
    LTE_E_NO_NET_INTERFACE,      /* 没有网口 */
    LTE_E_NO_LTECOMSEV,          /* 5,ltecomsev进程不存在 */
    LTE_E_NO_SIM,                /* SIM卡没有检测到 */
    LTE_E_SIM_PUK,               /* SIM卡进入PUK状态 */
    LTE_E_SIGNAL_LOW,            /* 没有信号 */
    LTE_E_RESIDENT_NET_FAIL,     /* 驻留网络失败 */
    LTE_E_NO_DIAL,               /* 10,没有拨号 */
    LTE_E_NO_DEFAULT_ROUTE,      /* 没有默认路由 */
    LTE_E_DEFAULT_FAIL,          /* default failure*/
} LTE_DIAGNOSE_STATE_E;

/*****************************诊断工具****************************/

/* 新平台，通信使用 */
typedef enum {
    WAIT_WRITE,         //等待写
    NOT_NEED_WRITE,     //不需要写
    WAIT_READ,          //写入后等待读
    WRITE_DONE,         //节点操作完成，完成写和读操作
}LTE_NODE_SM_E;

typedef enum
{
	COM_WRITE_TYPE_1 = 0,  /* 该类型的节点，只需向节点写一次AT指令 */
    COM_WRITE_TYPE_2 = 1   /* 该类型的节点，需要写完AT指令后，还有后续字符写
                              入com口 */
}LTE_COM_WRITE_TYPE_E;

typedef enum
{
	COM_READ_TYPE_1 = 0,  /* AT指令返回格式仅为OK */
    COM_READ_TYPE_2 = 1,  /* AT指令返回格式仅为ERROR */
    COM_READ_TYPE_3 = 2,  /* AT指令返回格式似+ xxxx ERROR: 10 */
    COM_READ_TYPE_4 = 3,  /* AT指令返回格式似+CIEV: xxxxxx\n OK \n */
    COM_READ_TYPE_5 = 4,  /* AT指令返回格式似 xxxxxx\n OK \n */
    COM_READ_TYPE_6 = 5,  /* AT指令返回格式似+CIEV: xxxxxx\n +CIEV: xxxxxx\n +CIEV: xxxxxx\n OK \n  */
    COM_READ_TYPE_7 = 6,  /* 从com口读取的数据格式: +CIEV:xxxxxx\n */
    COM_READ_TYPE_8 = 7,   /* 从com口读取的数据格式: OK\n  +CIEV:xxxxxx\n +CIEV:xxxxxx\n */
    COM_READ_TYPE_9 = 8  /* AT指令返回格式似+CIEV1: xxxxxx\n +CIEV2: xxxxxx\n +CIEV3: xxxxxx\n OK \n  */
}LTE_COM_READ_TYPE_E;

typedef enum
{
	COM_RETURN_SINGLE = 0,   /* AT指令返回1条 */
    COM_RETURN_MUTIL = 1     /* AT指令返回多条结果 */
}LTE_COM_RETURN_TYPE_E;


typedef enum
{
    SERVER_NEED_SEND = 0,   /* 服务器端接收到数据后需要发送给客户端 */
    SERVER_NOT_SEND = 1     /* 服务器端接收到数据后不需要发送给客户端 */
}LTE_SERVER_SEND_TYPE_E;

typedef struct
{
    char acAtCmd[256];
    char acCiev[32];
    int  iFuncCmd;
    int  iPriority;
    int  iNodeSM;
    int  iRealFlag;
    int  iComWriteType; /* 往com口写AT指令的格式，写多次还是写一次 */
    int  iComReadType;  /* 从com口读取数据，解析时的格式 */
    int  iComReturnType;
    int  iServerSendType;/* 服务器端接收到数据后是否需要发送给客户端 */
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
extern LTE_RET_E lte_clck_bands_close(void); /*恢复默认锁频*/
extern LTE_RET_E lte_clck_cell_set(LTE_CLK_CELL_T *pstCell);
extern LTE_RET_E lte_clck_cell_enable_get(LTE_CLKCELL_ENABLE_T *pstCellEnableFlag);

extern LTE_RET_E lte_device_info_get(LTE_DEVICE_INFO *pstDevInfo);

extern LTE_RET_E lte_gps_func_start(void);
extern LTE_RET_E lte_gps_func_stop(void);
extern LTE_RET_E lte_gps_date_get(LTE_GPS_DATE_T *pstGpsDate);
extern LTE_RET_E lte_gps_postion_get(LTE_GPS_POSTION_T *pstGpsPosInfo);
extern LTE_RET_E gps_serial_com_open(int *piComFd);
extern LTE_RET_E gps_serial_com_close(int iComFd);
/* 设置关闭GPS的接口
 * int iGpsCloseFlag  是否关闭GPS的标识
 */
extern LTE_RET_E set_gps_close_tag(int iGpsCloseFlag);

/* 设置关闭GPS的接口
 * int *piGpsCloseFlag  GPS是否关闭的标识
 */
extern LTE_RET_E get_gps_close_tag(int *piGpsCloseFlag);

/* 用于GPS产测的接口 */
extern LTE_RET_E lte_gps_test(void);

/* 获取GPS有效数据
 * int iCOmFd    打开GPS COM口返回的FD
 * int iTimeOut  从COM口直接获取GPS数据的超时时间
 * LTE_GPS_INFO_T *pstLteGpsInfo 解析后返回的GPS数据，包括时间、经度、纬度、方向角、速度、海拔
 */
extern LTE_RET_E gps_valid_data_get(int iComFd, int iTimeOut, LTE_GPS_INFO_T *pstLteGpsInfo);
/* BEGIN: Added by wujian, 2014/11/10   PN:添加获取运营商信息接口 */
extern LTE_RET_E lte_oper_info_get(LTE_OPER_INFO_T *pstOperInfo);
/* END:   Added by wujian, 2014/11/10   PN:添加获取运营商信息接口 */

/* BEGIN: Added by wujian, 2014/11/12   PN:添加获取支持的网络类型，如支持2g，3g，4g */
extern LTE_RET_E lte_sup_nettype_get(LTE_NET_TYPE_T *pstNetsInfo);
/* END:   Added by wujian, 2014/11/12   PN:添加获取支持的网络类型，如支持2g，3g，4g */

/* BEGIN: Added by wujian, 2014/11/13   PN:添加短信功能相关接口 */
extern LTE_RET_E lte_sms_cent_code_set(LTE_SMS_CENT_CODE_T *pstCentCode);
extern LTE_RET_E lte_sms_mode__set(LTE_SMS_Mode_T *pstSmsMode);
extern LTE_RET_E lte_sms_func_init(void);
extern LTE_RET_E lte_sms_recv(LTE_SMS_RECV_T *pstRecv);
extern LTE_RET_E lte_sms_send(LTE_SMS_SEND_T *pstSend);
extern LTE_RET_E lte_sms_dele(LTE_SMS_DEL_T *pstDele);
extern LTE_RET_E lte_sms_sup_func(LTE_SUPPORT_SMS_T *pstSupSms);
/* END:   Added by wujian, 2014/11/13   PN:添加短信功能相关接口 */
extern LTE_RET_E lte_pdp_cgdcont_set(LTE_CGDCONT_T *pstPdpSet);
/* BEGIN: Added by wujian, 2014/11/17   PN:添加设置apn接口 */
extern LTE_RET_E lte_apn_set(LTE_APN_T *pstApn);

/* END:   Added by wujian, 2014/11/17   PN:添加设置apn接口 */
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

