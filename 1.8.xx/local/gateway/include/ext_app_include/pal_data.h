/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : pal_data.h
  �� �� ��   : ����
  ��    ��   : Zhong Huaqing
  ��������   : 2012��7��23�� ����һ
  ����޸�   :
  ��������   : 
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��7��23�� ����һ
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ļ�

  2.��    ��   : 2012��8��15�� ������
    ��    ��   : xucongjiang
    �޸�����   : ����bsp��ص����ݽṹ���궨�弰ö������
******************************************************************************/

#ifndef __PAL_DATA_H__
#define __PAL_DATA_H__


/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
 //#include "../pal/wifi/pal_wifi_data.h"      //wifi�м������������ݶ���
 #include "../pal/include/ndis_qmi_service.h"
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
 * �������Ͷ���                                     *
 *----------------------------------------------*/
 
/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/
 
/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
#define BSP_PORT_LINK_UP       1
#define BSP_PORT_LINK_DOWN     0
#define BSP_PPPOE_USER_NAME_LEN 64
#define BSP_PPPOE_PASSWD_LEN 32
/*altered by caibing*/
#define NVRAM_NAME_MAX_LEN 128
#define NVRAM_VALUE_MAX_LEN 256

#define BSP_VER_NAME_MAX_LENTH  64
#define MUTEX_TEST_WAIT_TIME  10

/*add by caibing 2013-01-13,��д��ʽ����*/
#define LTE_LED_WRITE 0x55
#define LTE_LED_READ  0xaa
/*end by caibing 2013-01-13*/

#ifndef IFNAMSIZ
#define IFNAMSIZ 16
#endif

/*----------------------------------------------*
 * ö�ٶ���                                       *
 *----------------------------------------------*/
/* For wired port */
typedef enum
{
    BSP_PORT_10M = 0,
    BSP_PORT_100M = 1,
    BSP_PORT_1000M = 2,
    BSP_PORT_AUTO,      /*Attention: this value is only used to set port mode*/
    BSP_PORT_100_AUTO  /*Attention: this value is only used to set port mode*/
}BSP_PORT_SPEED_E;

typedef enum
{
    BSP_PORT_HALF_DUPLEX = 0,
    BSP_PORT_FULL_DUPLEX = 1
}BSP_PORT_DUPLEX_E;

/* For pppoe */
typedef enum
{
    PPPOE_CONNECTED,
    PPPOE_CONNECTING,
    PPPOE_UNCONNECTED
}BSP_PPPOE_STATUS_E;

/*For LTE*/
typedef enum
{
    EN_NET_TYPE_LTE_TDSCDMA,		/*֧��LTE��TD-CDMA˫ģ,LTE ����*/
    EN_NET_TYPE_TDSCDMA_LTE,		/*֧��LTE��TD-CDMA˫ģ,TD-S ����*/    
    EN_NET_TYPE_LTE,  				/*��֧��LTE*/
    EN_NET_TYPE_TDSCDMA,			/*��֧��TD-SCDMA*/   
    EN_NET_TYPE_2G                  /* ��2G���뷽ʽ */
}NET_TYPE_E;

/*----------------------------------------------*
 * �ṹ����                                       *
 *----------------------------------------------*/

#ifdef PAL_CMD_BSP
#endif

/*For BSP*/
typedef struct 
{
    CHAR acVersionPath[128];  /* must be absolute path */
}PAL_CMD_BSP_UPDATE_VERSION_T;

typedef struct 
{
    CHAR acRunningVer[BSP_VER_NAME_MAX_LENTH];
    CHAR acTargetVer[BSP_VER_NAME_MAX_LENTH];
    CHAR acVersion1[BSP_VER_NAME_MAX_LENTH];
    CHAR acVersion2[BSP_VER_NAME_MAX_LENTH];
    CHAR acKernelVer[BSP_VER_NAME_MAX_LENTH*2];
    CHAR acBootVer[BSP_VER_NAME_MAX_LENTH];
}PAL_CMD_BSP_SW_INFO_T;

typedef struct 
{
    CHAR acBoardName[64];
    CHAR acCpuType[64];
    CHAR acMemType[64];
    CHAR acFlashType[64];
    UINT32 uiBoardRevision;
    UINT32 uiMemSize;
    UINT32 uiFlashSize;
}PAL_CMD_BSP_HW_INFO_T;

typedef struct 
{
    UINT32 uiCpuUsage;
    UINT32 uiMemUsage;
    UINT32 uiMemFree;
    UINT32 uiUptime;
    UINT32 uiTemp;   /* temperature */
}PAL_CMD_BSP_RUNNING_INFO_T;

typedef struct 
{
    UINT32 uiLedNum;    /* the number of led,it can be one of the following value */
    
#define GPIO_RUN_LED 4 
#define GPIOEX_SIGNAL_0 36 
#define GPIOEX_SIGNAL_1 37 
#define GPIOEX_SIGNAL_2 38 
#define GPIOEX_SIGNAL_3 39 
#define GPIOEX_LTE_0 41 
#define GPIOEX_LTE_1 42 
#define GPIOEX_WLAN 46 
#define GPIOEX_WPS 47 

    UINT32 uiOnTime;    /* the time of led lighted. Unit: ms */
    UINT32 uiOffTime;   /* the time of led off. Unit: ms */
    
    /*add by caibing 2013-01-13,��������lte�ĵ�Ʒ�ʽ*/
    unsigned char ucLteLedFlag;  /* LTE��Ʒ�ʽ��־,0xaa��ʾlte��,0x55��ʾд*/
    unsigned int  uiLteLedStatus;   /*lte�Ʊ����õ���ֵ���߻�ȡ����ֵ */
    /*end by caibing 2013-01-13*/
}PAL_CMD_BSP_CONTROL_LED_T;

typedef struct
{
	UINT8 ucButtonId;
	UINT32 ulSeconds;
	UINT32 uiButtonType;
}PAL_CMD_BSP_CONTROL_BUTTON_T;

typedef struct 
{
    CHAR acPortName[IFNAMSIZ];   /* port name, e.g. eth0 */
    UINT32 usLinkStatus;
    BSP_PORT_SPEED_E enSpeed;   /* port speed */
    BSP_PORT_DUPLEX_E enDuplex; /* port duplex */
}PAL_CMD_BSP_WIRED_PORT_STATUS_T;

typedef struct 
{
    CHAR acPortName[IFNAMSIZ];  /* port name, e.g. eth0 */
    BSP_PORT_SPEED_E enSpeed;   /* port speed */
    BSP_PORT_DUPLEX_E uiDuplex; /* port duplex */
}PAL_CMD_BSP_WIRED_PORT_MODE_T;

typedef struct 
{
    CHAR acName[NVRAM_NAME_MAX_LEN];
    CHAR acValue[NVRAM_VALUE_MAX_LEN];
}PAL_CMD_BSP_NVRAM_ONE_T;

typedef struct 
{
    #ifndef NVRAM_SECTOR_SIZE   
    #define NVRAM_SECTOR_SIZE 0x0010000
    /*����С�޸�Ϊ4096���ֽڣ�Ŀǰnvram��ʹ��3060���ֽ�*/
    //#define NVRAM_SECTOR_SIZE 0x2000
    #endif
    CHAR acBuf[NVRAM_SECTOR_SIZE];
}PAL_CMD_BSP_NVRAM_ALL_T;

typedef PAL_CMD_BSP_NVRAM_ALL_T PAL_CMD_BSP_NVRAM_N_T;

typedef struct 
{
    CHAR acName[NVRAM_NAME_MAX_LEN];
}PAL_CMD_BSP_NVRAM_DELETE_T;


typedef struct 
{
    CHAR acUser[BSP_PPPOE_USER_NAME_LEN];
    CHAR acPasswd[BSP_PPPOE_PASSWD_LEN];
}PAL_CMD_BSP_PPPOE_SETUP_T;

typedef struct
{
    BSP_PPPOE_STATUS_E enStatus;
}PAL_CMD_BSP_PPPOE_STATUS_T;

typedef struct 
{
    CHAR acBrName[IFNAMSIZ];
}PAL_CMD_BSP_BRIDGE_T;

typedef struct 
{
    CHAR acBrName[IFNAMSIZ];
    CHAR acPortName[IFNAMSIZ];
}PAL_CMD_BSP_BR_IF_T;

typedef struct 
{
    CHAR acBrName[IFNAMSIZ];  /* bridge name */
    UINT32 uiTime;            /* forward delay time, unit:s */
}PAL_CMD_BSP_BR_FD_T;

typedef struct 
{   
    UINT32 uiIpaddr;
}AP_EVENT_BSP_TM_IPADDR_T;

typedef struct 
{   
    UINT32 uiTime;
}AP_EVENT_BSP_TM_TIME_T;

typedef struct 
{   
    UINT32 uiState;
}AP_EVENT_BSP_TM_STATE_T;

/*For LTE*/
typedef struct 
{
    UINT32 uiMode;			 /*1��ʾ�����Զ����ţ�0��ʾ�������Զ�����*/
}PAL_CMD_BSP_LTE_CONNECT_MODE_T; 


typedef struct
{
    UINT32 uiState; /* 0Ϊready״̬��1Ϊpin״̬��2Ϊpuk״̬ */
}PAL_CMD_BSP_LTE_PIN_STATE_T;

typedef struct 
{
    INT8 acPin[5];  /* pin�� */
    UINT32 uiSwitch;    /* 0��ʾ�ر�Pin�룬1��ʾ����PIN�빦�� */
} PAL_CMD_BSP_LTE_PIN_SWITCH_T;

typedef struct 
{
    INT8 acPin[5];  /* PIN�� */
} PAL_CMD_BSP_LTE_PIN_T;


typedef struct 
{
	UINT32 uiPinNum;    /* PIN��ʣ����� */
	UINT32 uiPukNum;    /* PUK��ʣ����� */
} PAL_CMD_BSP_LTE_PIN_ATTEMPTS_T;


typedef struct 
{
	UINT32 uiMode;  /* 0��ʾ�ر��Զ�PIN�빦�ܣ�1��ʾ�����Զ�PIN�빦�� */
} PAL_CMD_BSP_LTE_PIN_MODE_T;


typedef struct 
{
    INT8 acOldPin[5];   /* �޸�ǰ��PIN�� */
    INT8 acNewPin[5];   /* �޸ĺ��PIN�� */
} PAL_CMD_BSP_LTE_PIN_ALTER_T;


typedef struct 
{
    INT8 acApn[32];     /* APN */
} PAL_CMD_BSP_LTE_APN_T;


typedef struct 
{
    INT8 acImei[32];    /* IMEI�� */
} PAL_CMD_BSP_LTE_IMEI_T;

typedef struct 
{
    INT8 acCellID[32];    /* cell id */
} PAL_CMD_BSP_LTE_CELLID_T;
typedef struct 
{
    INT8 acBand[32];    /* band */
} PAL_CMD_BSP_LTE_BAND_T;
typedef struct 
{
    UINT32 uiSignal;    /* �ź�ֵ */
} PAL_CMD_BSP_LTE_SIGNAL_T;

typedef struct 
{
    INT8 acLinkSwitch[32];  /* on��ʾ��ʱ�رգ�off��ʾ��ʱ���� */
    INT32 iTimer;   /* ��ʱʱ�� */
} PAL_CMD_BSP_LTE_LINKTIMER_T;

typedef struct 
{
    INT8 acLinkSwitch[32];      /* on��ʾ�رն�ʱ�������ܣ�off��ʾ�رն�ʱ��
                                   �չ��� */
} PAL_CMD_BSP_LTE_DISLINKTIMER_T;


typedef struct 
{
    INT32 iLinkstate;   /* ��·״̬ */
} PAL_CMD_BSP_LTE_LINKSTATE_T;

typedef struct 
{
   NET_TYPE_E enNetType;    /* ������ʽ */
} PAL_CMD_BSP_LTE_NET_TYPE_T;

typedef struct 
{  
    UINT8 acCmd[32];    /* ����com�ڵ����� */
    UINT8 acFileName[32];   /* ��Ҫ�Ƚϵ��ļ� */
    UINT8 acNeedle[32]; /* �Ƚ��ֶ� */
    INT32 iResult;    /* ����ȽϵĽ�� */
    /*altered by caibing 2013-02-27*/
    UINT8 acCom[32];    /* ������com�� */
    INT32 iComFd;       /*com fd ��*/
    INT32 iSecond;      /*��дcomʱ��*/
}PAL_CMD_BSP_LTE_NORMAL_AT_T;

typedef struct 
{  
    INT32 iCurrentCell;    /* ����ȽϵĽ�� */
}PAL_CMD_BSP_LTE_CELL_T;

typedef struct 
{
    INT32 iMode;/*ģʽѡ�� 1Ϊ�Զ� 0Ϊ�ֶ�*/
    INT32 iFormat;/*��ʾ��ʽ*/
    UINT8 acOper[32];/*�����̱�ʶ*/
    UINT8 cAct;/*���뼼��*/
}PAL_CMD_BSP_LTE_PLMN_T;


typedef struct 
{
    INT32 iStatus;		/*����״̬*/
    char acChinese[32]; /*���ı���*/
    char acEnglish[32];   /*Ӣ������*/
    char acNuberic[32];  /*����˵��*/
    INT32 iAct;			 /*���뼼��*/
}PAL_CMD_BSP_LTE_SIM_CARRIER_T;

typedef struct 
{
    UINT32 uiAcessNet;			/*�����־λ����*/
    UINT32 uiDisconnetNet;		/*�Ͽ���־λ����*/
    UINT32 uiDisconnLast;
} PAL_CMD_BSP_LTE_AC_DI_T;

typedef struct 
{
	INT32 iSwitch;		/*����*/
	INT32 iTimes;			/*��ʱ����*/
} PAL_CMD_BSP_LTE_SWITCH_TIMING_T;
typedef struct 
{
	INT32 iSwitch;
	INT32 iTime;
} PAL_CMD_BSP_LTE_NO_THROUGHT_DISC_T;
typedef PAL_CMD_BSP_LTE_NO_THROUGHT_DISC_T PAL_CMD_BSP_LTE_HAS_THROUGHT_CONN_T;

#ifdef PAL_CMD_APP
#endif

/*For APP*/
typedef struct 
{
    UINT32 uiAddr0;
    UINT32 uiAddr1;
}PAL_CMD_APP_DNS_T;
typedef struct
{
UINT32 uiIp;
UINT32 uiSubmask;
UINT32 uiGeteway;
}PAL_APP_LAN_INFO_T;


typedef struct
{
UINT32 uiStartIp;
UINT32 uiEndIp;
UINT32 uiNetMask;
UINT32 uiDefaultGw;
UINT32 uiPrimaryDns;
UINT32 uiSubDns;
UINT32 uiDhcpLease;
}PAL_APP_DHCP_CONFIG_T;


typedef struct 
{
UINT32 uiDnsAddr;
}PAL_APP_DNSMASQ_START_T;


typedef struct 
{
/*NVRAM_NAME_MAX_LEN���ں���ϵͳ�Ѿ�����*/
CHAR sDomainIp[NVRAM_NAME_MAX_LEN];
//UINT32 uiDomaintime;
}PAL_APP_NTP_INFO_T;


typedef struct
{
    UINT32 uiFlowSW;
UINT32 uiUserMaxUpload;
UINT32 uiUserMaxDownload;
}PAL_APP_FLOWCTRL_INFO_T;


typedef struct 
{
UINT32 uiWanIp;
UINT32 uiWanSubmask;
UINT32 uiWanGeteway;
UINT32 uiWanDns1; 
UINT32 uiWanDns2; 
}PAL_APP_WAN_INFO_T;

typedef struct 
{
    CHAR old_pwd[32];
    CHAR new_pwd[32];
}PAL_APP_PASSWD_INFO_T;

/* BEGIN: Added by zhanghaozhong, 2012/12/18   PN:add usr log function */
typedef struct 
{
    CHAR acLog[128];
}PAL_APP_USR_LOG_T;
/* END:   Added by zhanghaozhong, 2012/12/18   PN:add usr log function */

/* BEGIN: Added by zhanghaozhong, 2012/12/26   PN:add wps state get */
typedef struct 
{
    BOOL bWpsState;
}PAL_APP_WPS_STA_T;
/* END:   Added by zhanghaozhong, 2012/12/26   PN:add wps state get */

#ifdef PAL_CMD_WIFI
#endif

/*For WIFI*/

/* BEGIN: Added by lvjianlin, 2013/4/30   PN:ltefi */
/* ac dns struct */
typedef struct
{
     CHAR   acAcURL[64];   /* AC URL */  
}PAL_APP_ACDNS_INFO_T;

/* AP link state 0:���� 1:���� 2:���� 3:������·��ͨ*/
typedef struct
{
    UINT32  uiAclinkstat;
}PAL_APP_ACLINKSTATE_INFO_T;

/* END:   Added by lvjianlin, 2013/4/30   PN:ltefi */

/* BEGIN: Added by lvjianlin, 2013/6/26   PN:lte-fi  GPS ALARM  LOCAL PORTAL */
typedef struct
{
    CHAR    acUserIP[16];
}PAL_APP_USER_IP_INFO_T;
/* END:   Added by lvjianlin, 2013/6/26   PN:lte-fi  GPS ALARM  LOCAL PORTAL */
#endif /* __PAL_DATA_H__ */


