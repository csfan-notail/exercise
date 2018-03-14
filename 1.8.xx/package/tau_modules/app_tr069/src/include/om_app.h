/******************************************************************************

                  ��Ȩ���� (C), 2010-2012, �Ϻ�徴�ͨ�ſƼ����޹�˾

 ******************************************************************************
  �� �� ��   : om_app.h
  �� �� ��   : ����
  ��    ��   : zhangjiasi
  ��������   : 2014��4��22��
  ����޸�   :
  ��������   : om_app.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2014��4��22��
    ��    ��   : zhangjiasi
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __OM_APP_H__
#define __OM_APP_H__
/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "ap_types.h"
#include "om_net.h"
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
#ifndef OEM_INFO_PATH
#define OEM_INFO_PATH       "/tmp/oem_info.txt"
#endif

#define OEM_DATANG_STRING       "datang"

#ifndef SYSLOG_FILE
#define SYSLOG_FILE         "/var/log/messages.txt"
#endif

#ifndef LOG_READ_LEN    
#define LOG_READ_LEN        4000
#endif

#ifndef CMS_RMTMGR_STATE_FILE    
#define CMS_RMTMGR_STATE_FILE    "/tmp/CmsRmtmgrState"
#endif

typedef struct 
{
    int language;
    char username[CHAR_LEN_32];
    char password[CHAR_LEN_32];
}OM_APP_USER_LOGIN_T;


typedef struct 
{
    CHAR acDeviceType[CHAR_LEN_32];
    CHAR acDeviceSn[CHAR_LEN_64];
    CHAR acHardVer[CHAR_LEN_64];
    CHAR acSoftVer[CHAR_LEN_64];
    CHAR acSysTime[CHAR_LEN_32];
    CHAR acUpTime[CHAR_LEN_32];
}OM_APP_SYS_BASIC_INFO_T;

typedef struct 
{
    INT32 uiWanAccessMode;
    INT32 uiForwardMode;
    UINT32 uiIpProtoMode;     //Э��ģʽ
    UINT8 aucWanIp[4];
    CHAR szWanIpv6[CHAR_LEN_256];
    INT32 iIpv6Prefex;
}OM_APP_SYS_WAN_BASIC_INFO_T;

typedef struct 
{
    CHAR    acSysSn[CHAR_LEN_32];                   /* �豸Sn�� */
    CHAR    acSysDevType[CHAR_LEN_32];              /* �豸�ͺ� */
    CHAR    acSysTime[CHAR_LEN_32];                 /* �豸ϵͳʱ�� */
    UINT32  uiSysUpTime;                            /* �豸�ϵ�ʱ�� */
}OM_APP_SYS_DEV_INFO_T;

typedef struct 
{
    CHAR    acSoftwareVersion[CHAR_LEN_128];        /* ��ǰ����汾 */
    CHAR    acTargetVersion[CHAR_LEN_128];          /* Ŀ��汾 */
}OM_APP_SYS_SW_INFO_T;

typedef struct{
    UINT8       aucIp[IP_ALEN];
    UINT8      ulIpMask[IP_ALEN];
    UINT8       acMac[ETH_ALEN];
    UINT8       aucIp6[IPV6_ALEN];
    UINT32      ulIp6Mask;
}OM_APP_SYS_INTFACE_INFO_T;

/* ��Ʒ��Ϣ */
typedef struct 
{
    CHAR szSoftware[32];
    CHAR szHardware[32];
    CHAR szCompany[32];
    CHAR szType[32];
}OM_APP_OEM_INFO_T;

typedef struct 
{
    UINT32 uiDeviceTypeId;
    OM_APP_OEM_INFO_T stOemInfo;
}OM_APP_PRODUCT_INFO_T;


/*********** �������� ************/

typedef struct 
{
    CHAR acSsid[32];
    CHAR acWpaPskKey[64];
}OM_APP_QUICK_SET_SSID_CFG_T;

typedef struct 
{
    OM_NET_WIRED_WAN_CFG_T stWiredCfg;
    OM_APP_QUICK_SET_SSID_CFG_T     stWirelessCfg; 
}OM_APP_QUICK_SET_CFG_T;

/*********** �汾���� ************/
typedef enum 
{
    VER_UPDATE_ONLY=0,          /* ���汾���� */
    VER_UPDATE_AND_CFG_RESTORE  /* �汾�������ָ��������� */
}OM_APP_VER_OPERATE_FLAG_E;

typedef struct 
{
    OM_APP_VER_OPERATE_FLAG_E eOperateFlag;
    CHAR szVersionName[32];
}OM_APP_SYSMNT_VER_INFO_T;

typedef struct 
{
    CHAR acVersionName[64];
    CHAR acVersionMd5sum[64];
}OM_APP_SYSMNT_LTE_VER_INFO_T;

/*********** ���ù��� ************/
typedef enum 
{
    SYSMNT_CFG_RERTORE_DO_NOTHING = 0,
    SYSMNT_CFG_RERTORE_FACTORY,
    SYSMNT_CFG_RERTORE_PART_FACTORY,

}OM_APP_SYSMNT_CFG_RESTORE_FLAG_E;

typedef enum 
{
    SYSMNT_CFG_BACKUP_DO_NOTHING = 0,    
    SYSMNT_CFG_BACKUP_CFG_IMPORT,
    SYSMNT_CFG_BACKUP_CFG_EXPORT
}OM_APP_SYSMNT_CFG_BACKUP_FLAG_E;

typedef struct 
{
    OM_APP_SYSMNT_CFG_RESTORE_FLAG_E eRestoreFlag;
    OM_APP_SYSMNT_CFG_BACKUP_FLAG_E eBackupFlag;
	CHAR fac_recv_string[CHAR_LEN_64];
}OM_APP_SYSMNT_CFG_INFO_T;

/*********** �豸���� ************/
typedef enum 
{
    SYSMNT_DEV_DO_NOTHING = 0,
    SYSMNT_DEV_OPERATE_REBOOT,
}OM_APP_SYSMNT_DEV_OPERATE_FLAG_E;

typedef struct 
{
    OM_APP_SYSMNT_DEV_OPERATE_FLAG_E eOperateFlag;
}OM_APP_SYSMNT_DEV_INFO_T;

/*********** ��֤���� ************/
/* �ض������� */
typedef enum 
{
    OM_PORTAL_REDIRECT_OFF = 0,
    OM_PORTAL_REDIRECT_PORTAL_REDIRECT,
    OM_PORTAL_REDIRECT_PORTAL_AUTH
}OM_APP_PORTAL_REDIRECT_MODE_E;

typedef struct 
{
    UINT32 uiRedirectSupport;                   /* �Ƿ���Ҫ֧���ض����� */
    OM_APP_PORTAL_REDIRECT_MODE_E enRedirectType;  /* ҳ���ض��򿪹� */
    CHAR szRedirectAddr[CHAR_LEN_64];           /* �ض����ַ */
    UINT32 uiRedirectPort;                      /* �ض���˿� */
}OM_APP_PORTAL_REDIRECT_INFO_T;

/*********** ʱ����� ************/
typedef enum 
{
    SYSMNT_NTP_OFF = 0,
    SYSMNT_NTP_AUTO,
    SYSMNT_NTP_MANUL
}OM_APP_SYSMNT_NTP_SYN_MODE_E;

typedef struct 
{
    OM_APP_SYSMNT_NTP_SYN_MODE_E eNtpMode;
    CHAR szNtpSvr[CHAR_LEN_64];
    CHAR szNtpSvr2[CHAR_LEN_64];
    UINT32 uiSynPeriod;
    CHAR szYear[CHAR_LEN_32];
    CHAR szMon[CHAR_LEN_32];
    CHAR szDay[CHAR_LEN_32];
    CHAR szHour[CHAR_LEN_32];
    CHAR szMin[CHAR_LEN_32];
    CHAR szSec[CHAR_LEN_32];
}OM_APP_SYSMNT_NTP_INFO_T;

/*********** ��־���� ************/
typedef struct 
{
    UINT32 uiLogStoreSw;
    UINT32 uiLogStoreInterval;
    UINT32 uiRmtLogSw;
    UINT8 aucRmtLogSvrAddr[4];
    UINT32 uiRmtLogCommPort;
}OM_APP_SYSMNT_LOG_INFO_T;


typedef struct 
{
    UINT32 uiUpdateFlag;   //ˢ�±�־λ:1:�ֲ�ˢ��  2:�ύ����
    UINT32 uiAutoUpdateSw;
    UINT32 uiAutoUpdateTime;
}OM_APP_SYSMNT_LOG_REALTIME_INFO_T;

/*********** Զ�̹��� ************/

typedef enum 
{
    RMTMGR_PROTO_SNMP = 1,
    RMTMGR_PROTO_TR069
}OM_APP_RMTMGR_PROTO_E;

typedef enum 
{
    RMTMGR_SW_OFF = 0,
    RMTMGR_SW_ON
}OM_APP_RMTMGR_SW_E;

typedef enum 
{
    RMTMGR_OFF = 0,
    RMTMGR_SNMP,
    RMTMGR_TR069,
    RMTMGR_SNMPTR069
}OM_APP_RMTMGR_MODE_E;
typedef struct 
{
    UINT32 uiRmtMgrSw;                  /* Զ�̹����� */
    OM_APP_RMTMGR_PROTO_E eRmtMgrProto; /* ����Э��ѡ�� */
    CHAR   acCmsUrl[CHAR_LEN_128];
	CHAR   acCmsTr69cUrl[CHAR_LEN_128];
    CHAR   acUserName[CHAR_LEN_64];
    CHAR   acUserPasswd[CHAR_LEN_64];
    CHAR   acCpeUrl[CHAR_LEN_128];
    CHAR   acCpeUserName[CHAR_LEN_64];//acs��������cpe�ǣ���ҪЯ�����û��������룬cpe�����ǽ���У��
    CHAR   acCpeUserPasswd[CHAR_LEN_64];
    UINT32 uiEchoSw;
    UINT32 uiEchoInterval;
    UINT32 uiLocalProto;                /* ����ͨ�Ŷ˿� */
}OM_APP_RMTMGR_CFG_T;

typedef enum 
{
    RMTMGR_STATE_ONLINE = 0,
    RMTMGR_STATE_OFFLINE
}OM_APP_RMTMGR_STATE_E;

typedef struct 
{
    OM_APP_RMTMGR_STATE_E eState;
}OM_APP_RMTMGR_STATE_T;


/*********** �û����� ************/

#ifndef USER_MAX
#define USER_MAX    5
#endif

/* �û�����Ȩ�� */
typedef enum 
{
    USERMG_USER_ADD = 0,
    USERMG_USER_DEL,
    USERMG_USER_MODIFY
}OM_APP_USERMGR_OPERATE_FLAG_E;

typedef enum 
{
    USERMGR_PERMISSION_MANAGER = 0,
    USERMGR_PERMISSION_NORMAL
}OM_APP_USERMGR_PERMISSION_LEVEL_E;

typedef enum 
{
    USERMGR_NO_ALERT = 0,
    USERMGR_OLA_PASSWD_WRONG
}OM_APP_USERMGR_ALERT_E;

typedef struct 
{
    UINT32 uiNvramId;
    OM_APP_USERMGR_OPERATE_FLAG_E enFlag;
    CHAR szUserName[CHAR_LEN_32];
    CHAR szOldPwd[CHAR_LEN_32];
    CHAR szNewPwd[CHAR_LEN_32];
    OM_APP_USERMGR_PERMISSION_LEVEL_E enPermission;
    OM_APP_USERMGR_ALERT_E enAlert;
}OM_APP_USERMGR_CFG_T;


/* device config  */
typedef enum 
{
    OM_WIFI_SUPPORT     = 0x00000001,           //if wifi support
    OM_IPV6_SUPPORT     = 0x00000002,           //if ipv6 support
    OM_WAN_CONF_SUPPORT = 0x00000004,           //if wan_config support
    OM_SIM_BING_SUPPORT = 0x00000008,           //if machine_sim bind support    
    OM_QOS_SUPPORT      = 0x00000010,           //if qos support
    OM_MESSAGE_SUPPORT  = 0x00000020,           //if message support
    OM_REDIRECT_SUPPORT = 0x00000040,           //if redirect support
    OM_LTEUPDATE_SUPPORT = 0x00000080,           //if module update support
}OM_MODULE_SUPPORT_FLAG_E;

#define SET_WIFI_ENABLE(x)              x = (x | OM_WIFI_SUPPORT)
#define SET_WIFI_DISABLE(x)             x = (x & (~OM_WIFI_SUPPORT))
#define GET_WIFI_SUPPORT(x)             (x & OM_WIFI_SUPPORT)

#define SET_IPV6_ENABLE(x)              x = (x | OM_IPV6_SUPPORT)
#define SET_IPV6_DISABLE(x)             x = (x & (~OM_IPV6_SUPPORT))
#define GET_IPV6_SUPPORT(x)             (x & OM_IPV6_SUPPORT)

#define SET_WAN_CONF_ENABLE(x)          x = (x | OM_WAN_CONF_SUPPORT)
#define SET_WAN_CONF_DISABLE(x)         x = (x & (~OM_WAN_CONF_SUPPORT))
#define GET_WAN_CONF_SUPPORT(x)         (x & OM_WAN_CONF_SUPPORT)

#define SET_SIM_BIND_ENABLE(x)          x = (x | OM_SIM_BING_SUPPORT)
#define SET_SIM_BIND_DISABLE(x)         x = (x & (~OM_SIM_BING_SUPPORT))
#define GET_SIM_BIND_SUPPORT(x)         (x & OM_SIM_BING_SUPPORT)

#define SET_QOS_ENABLE(x)               x = (x | OM_QOS_SUPPORT)
#define SET_QOS_DISABLE(x)              x = (x & (~OM_QOS_SUPPORT))
#define GET_QOS_SUPPORT(x)              (x & OM_QOS_SUPPORT)

#define SET_MESSAGE_ENABLE(x)           x = (x | OM_MESSAGE_SUPPORT)
#define SET_MESSAGE_DISABLE(x)          x = (x & (~OM_MESSAGE_SUPPORT))
#define GET_MESSAGE_SUPPORT(x)          (x & OM_MESSAGE_SUPPORT)

#define SET_REDIRECT_ENABLE(x)          x = (x | OM_REDIRECT_SUPPORT)
#define SET_REDIRECT_DISABLE(x)         x = (x & (~OM_REDIRECT_SUPPORT))
#define GET_REDIRECT_SUPPORT(x)         (x & OM_REDIRECT_SUPPORT)

#define SET_LTEUPDATE_ENABLE(x)          x = (x | OM_LTEUPDATE_SUPPORT)
#define SET_LTEUPDATE_DISABLE(x)         x = (x & (~OM_LTEUPDATE_SUPPORT))
#define GET_LTEUPDATE_SUPPORT(x)         (x & OM_LTEUPDATE_SUPPORT)


/* �˿����÷�ʽ  0������platform   1���ֶ����� */
typedef enum 
{
    OM_PORT_BASE_PLATFORM = 0,
    OM_PORT_BASE_COFIG
}OM_DEVICE_PORT_CONFIG_WAY_E;

typedef enum 
{
    OM_PORT_TYPE_WAN = 0,
    OM_PORT_TYPE_LAN
}OM_DEVICE_PORT_TYPE_E;

typedef enum 
{
    DEVICE_CONFIG = 1,
    DEVICE_PERFORMACE_TEST,
}OM_DEVICE_OPERATE_E;
typedef struct 
{
	OM_DEVICE_OPERATE_E eFlag;
    OM_DEVICE_PORT_CONFIG_WAY_E enPortCfgWay;
    OM_DEVICE_PORT_TYPE_E enEth0Type;   /* 0:WAN ��  1��LAN�� */
    OM_DEVICE_PORT_TYPE_E enEth1Type;
    UINT32 uiWifiNotSupport;            //1����֧������ 0��֧������
    UINT32 uiRedirectSupport;           /* 0:��֧��  1��֧�� */
    UINT32 uiIpv6Support;               /* ipv6֧�ֿ��� 0:��֧��  1:֧�� */
    UINT32 uiMsgSupport;                /* 0:��֧��  1��֧�� */

    UINT32 uiWanConfSupport;             /* 0:��֧������ 1:֧�� */
    UINT32 uiSimBindSupport;            /* 0:��֧�� 1:֧�� */
    UINT32 uiQosSupport;                /* 0:��֧�� 1:֧�� */
}OM_DEVICE_SUPPORT_CONFIG_T;

typedef struct 
{
    UINT32 eOperateFlag;
    CHAR szVersionName[50];
    CHAR szOemDeviceType[32];
    CHAR szOemVendorName[32];
}OM_DEVICE_UPDATE_CONFIG_T;

/***************************************        ��������        *************************************/
REMOTE OM_ERROR_CODE_E popen_get_info(CHAR *pacCmd, CHAR *pacInfo, UINT32 uiLength);
REMOTE OM_ERROR_CODE_E om_app_user_login_check( void *pvData );
REMOTE OM_ERROR_CODE_E  om_app_get_sw_info( OM_APP_SYS_SW_INFO_T *pstSysSwInfo );
REMOTE OM_ERROR_CODE_E  om_app_get_sw_info( OM_APP_SYS_SW_INFO_T *pstSysSwInfo );
REMOTE OM_ERROR_CODE_E  om_app_get_wan_info( OM_APP_SYS_INTFACE_INFO_T *pstSysManageInfo );
REMOTE OM_ERROR_CODE_E  om_app_get_device_id( UINT32 *puiDeviceId );
REMOTE OM_ERROR_CODE_E  om_app_get_oem_company( CHAR *szCompany );
REMOTE OM_ERROR_CODE_E  om_app_oem_info_analysis( CHAR *szSrcStr, CHAR *szCmpStr, CHAR *szDstStr );
REMOTE OM_ERROR_CODE_E  om_app_get_device_sn( CHAR *szSn );
REMOTE OM_ERROR_CODE_E  om_app_get_oem_info( UINT32 uiDevTypeId,OM_APP_OEM_INFO_T *pstOemInfo);
OM_ERROR_CODE_E  om_app_sys_basic_info_get( OM_APP_SYS_BASIC_INFO_T *pstSysBasicInfo );
OM_ERROR_CODE_E om_app_set_language_type( INT32 *piLanguge );
REMOTE OM_ERROR_CODE_E  om_app_get_platform( CHAR *platform_str );
/***********************         ��������          ****************************/
REMOTE OM_ERROR_CODE_E om_quick_set_config_set(OM_APP_QUICK_SET_CFG_T *pstQuickSetCfg);
REMOTE OM_ERROR_CODE_E  om_app_get_page_first_flag( INT32 *flag );
REMOTE OM_ERROR_CODE_E  om_app_set_page_first_flag( INT32 flag );
/***********************          ʱ�����          ****************************/
REMOTE OM_ERROR_CODE_E  om_sysmnt_ntp_config_get( OM_APP_SYSMNT_NTP_INFO_T *pstNTPCfg);
REMOTE OM_ERROR_CODE_E  om_sysmnt_ntp_config_nvram(OM_APP_SYSMNT_NTP_INFO_T *pstNTPCfg);
REMOTE OM_ERROR_CODE_E  om_sysmnt_ntp_config_module(OM_APP_SYSMNT_NTP_INFO_T *pstNTPCfg);
REMOTE OM_ERROR_CODE_E  om_sysmnt_ntp_config_set( OM_APP_SYSMNT_NTP_INFO_T *pstNTPCfg);


/***********************         ϵͳ���ù���          ****************************/
REMOTE OM_ERROR_CODE_E  om_sysmnt_cfg_restore_part_factory(char * fac_string);
REMOTE OM_ERROR_CODE_E  om_sysmnt_cfg_restore_factory();
REMOTE OM_ERROR_CODE_E  om_sysmnt_cfg_config_set( OM_APP_SYSMNT_CFG_INFO_T *pstSysmntCfg );
REMOTE OM_ERROR_CODE_E  om_sysmnt_ver_config_set( OM_APP_SYSMNT_VER_INFO_T *pstSysmntVer );
REMOTE OM_ERROR_CODE_E  om_sysmnt_lte_ver_config_set( OM_APP_SYSMNT_LTE_VER_INFO_T *pstSysmntLteVer );
REMOTE OM_ERROR_CODE_E  om_sysmnt_cfg_config_set( OM_APP_SYSMNT_CFG_INFO_T *pstSysmntCfg );
REMOTE OM_ERROR_CODE_E  om_sysmnt_dev_config_set( OM_APP_SYSMNT_DEV_INFO_T *pstSysmntDev );

/***********************         ��־����          ****************************/
REMOTE OM_ERROR_CODE_E  om_sysmnt_log_config_get( OM_APP_SYSMNT_LOG_INFO_T *pstLogCfg);
REMOTE OM_ERROR_CODE_E  om_sysmnt_log_config_nvram(OM_APP_SYSMNT_LOG_INFO_T *pstLogCfg,OM_APP_SYSMNT_LOG_INFO_T *pstLogCfgOld);
REMOTE OM_ERROR_CODE_E  om_sysmnt_log_config_module( OM_APP_SYSMNT_LOG_INFO_T *pstLogCfg,OM_APP_SYSMNT_LOG_INFO_T *pstLogCfgOld);
REMOTE OM_ERROR_CODE_E  om_sysmnt_log_config_set( OM_APP_SYSMNT_LOG_INFO_T *pstLogCfg);

REMOTE OM_ERROR_CODE_E  om_sysmnt_log_realtime_config_get(CHAR *pstBuff ,OM_APP_SYSMNT_LOG_REALTIME_INFO_T *pstRealtimeLogCfg );
REMOTE OM_ERROR_CODE_E  om_sysmnt_log_realtime_config_nvram(OM_APP_SYSMNT_LOG_REALTIME_INFO_T *pstRealtimeLogCfg);
REMOTE OM_ERROR_CODE_E  om_sysmnt_log_realtime_config_module( OM_APP_SYSMNT_LOG_REALTIME_INFO_T *pstRealtimeLogCfg);
REMOTE OM_ERROR_CODE_E  om_sysmnt_log_realtime_config_set( OM_APP_SYSMNT_LOG_REALTIME_INFO_T *pstRealtimeLogCfg);

/***********************         �û�����          ****************************/
REMOTE OM_ERROR_CODE_E  om_sysmnt_user_info_init( void );
REMOTE OM_ERROR_CODE_E  om_sysmnt_user_info_get(CHAR * szUserInfoList, INT32 * piUserNum, INT32 iLanguage);
REMOTE OM_ERROR_CODE_E  om_sysmnt_user_add( OM_APP_USERMGR_CFG_T *pstUserCfg );
REMOTE OM_ERROR_CODE_E  om_sysmnt_user_del( OM_APP_USERMGR_CFG_T *pstUserCfg );
REMOTE OM_ERROR_CODE_E  om_sysmnt_user_modify( OM_APP_USERMGR_CFG_T *pstUserCfg );
REMOTE OM_ERROR_CODE_E  om_sysmnt_user_nvram_set( OM_APP_USERMGR_CFG_T *pstUserCfg );
REMOTE OM_ERROR_CODE_E  om_sysmnt_user_nvram_unset( OM_APP_USERMGR_CFG_T *pstUserCfg );
REMOTE OM_ERROR_CODE_E  om_sysmnt_user_nvram_modify( OM_APP_USERMGR_CFG_T *pstUserCfg );
REMOTE OM_ERROR_CODE_E  om_sysmnt_user_config_nvram( OM_APP_USERMGR_CFG_T *pstUserCfg );

/***********************         ��֤����          ****************************/
REMOTE OM_ERROR_CODE_E  om_portal_redirect_config_get( OM_APP_PORTAL_REDIRECT_INFO_T *pstRedirectCfg);
REMOTE OM_ERROR_CODE_E  om_portal_redirect_config_nvram(OM_APP_PORTAL_REDIRECT_INFO_T *pstRedirectCfg,
                                                            OM_APP_PORTAL_REDIRECT_INFO_T *pstRedirectCfgOld);
REMOTE OM_ERROR_CODE_E  om_portal_redirect_config_module(OM_APP_PORTAL_REDIRECT_INFO_T *pstRedirectCfg,
                                                            OM_APP_PORTAL_REDIRECT_INFO_T *pstRedirectCfgOld);
REMOTE OM_ERROR_CODE_E  om_portal_redirect_config_set( OM_APP_PORTAL_REDIRECT_INFO_T *pstRedirectCfg);

/***********************         Զ�̹���          ****************************/
REMOTE OM_ERROR_CODE_E  om_rmtmgr_cfg_get( OM_APP_RMTMGR_CFG_T *pstRmtMgrCfg );
REMOTE OM_ERROR_CODE_E  om_rmtmgr_config_nvram( OM_APP_RMTMGR_CFG_T *pstRmtMgrCfg);
REMOTE OM_ERROR_CODE_E  om_rmtmgr_config_module(OM_APP_RMTMGR_CFG_T *pstRmtMgrCfg);
REMOTE OM_ERROR_CODE_E  om_rmtmgr_cfg_set( OM_APP_RMTMGR_CFG_T *pstRmtMgrCfg );
REMOTE OM_ERROR_CODE_E  om_rmtmgr_state_get( OM_APP_RMTMGR_STATE_T *pstRmtMgrState );
REMOTE OM_ERROR_CODE_E  om_rmtmgr_state_set( OM_APP_RMTMGR_STATE_T *pstRmtMgrState );
REMOTE OM_ERROR_CODE_E  om_rmtmgr_state_online(void);
REMOTE OM_ERROR_CODE_E  om_rmtmgr_state_offline(void);

REMOTE OM_ERROR_CODE_E  om_app_module_support_flag_get( OM_DEVICE_SUPPORT_CONFIG_T *pstSupportCfg );
REMOTE OM_ERROR_CODE_E om_tr69c_reboot_get(int *actmp);
REMOTE OM_ERROR_CODE_E om_tr69c_reboot_set(int actmp);

#endif /* __OM_APP_H__ */
