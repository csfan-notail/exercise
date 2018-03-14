/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : om_wifi.h
  版 本 号   : 初稿
  作    者   : zhangjiasi
  生成日期   : 2014年4月28日
  最近修改   :
  功能描述   : om_wifi.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2014年4月28日
    作    者   : zhangjiasi
    修改内容   : 创建文件

******************************************************************************/
#ifndef __OM_WIFI_H__
#define __OM_WIFI_H__
/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "om_types.h"
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
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#ifndef SSID_NAME_LEN
#define SSID_NAME_LEN           32
#endif

#ifndef WEP_KEY_LEN
#define WEP_KEY_LEN             32
#endif

#ifndef WPA_KEY_LEN
#define WPA_KEY_LEN             64
#endif

#ifndef WIFI_SWITCH_OFF
#define WIFI_SWITCH_OFF         0
#endif

#ifndef WIFI_SWITCH_ON
#define WIFI_SWITCH_ON          1
#endif

#ifndef VAP_NAME           
#define VAP_NAME           "vap0_0"
#endif

#ifndef WIFI_NAME           
#define WIFI_NAME           "wifi0"
#endif

#ifndef VAP
#define VAP                "vap"
#endif

#ifndef WIFI
#define WIFI                "wifi"
#endif 
/* 加密配置脚本路径 */
#define WIFI_ENCRYPT_PATH        ("/tmp/%s.hostapd.conf")

#ifndef SSID_MAX
#define SSID_MAX        8  //一张卡可配置最大ssid个数
#endif

/***************************** 结构体定义 ********************************/

typedef enum 
{
    OM_WIFI_WEB_ALERT_NONE = 0,
    OM_WIFI_WEB_ALERT_WIFI_SWITCH ,  //提示需要打开wifi开关
}OM_WIFI_WEB_ALERT_E;

typedef enum 
{
    OM_SSID_ADD = 1,
    OM_SSID_DELETE = 2,
    OM_SSID_WPS_CONFIG = 3
}OM_WIFI_SSID_OPERATE_E;

/* wifi射频卡数目 */
typedef enum 
{
    WIFI_24GHZ = 0,
    //WIFI_58GHZ,
    WIFI_NUM_MAX
}OM_WIFI_NUM_E;

/* ssid 快捷配置页面 */
typedef enum 
{
    SSID_WPS_SW_OFF = 0,
    SSID_WPS_SW_ON  = 1
}OM_WIFI_SSID_WPS_SW_E;

typedef enum 
{
    SSID_WPS_PBC = 0,
    SSID_WPS_PIN = 1,
}OM_WIFI_SSID_WPS_MODE_E;

typedef struct 
{
    OM_WIFI_SSID_WPS_SW_E eWpsSw;
    OM_WIFI_SSID_WPS_MODE_E eWpsMode;
    CHAR acWpsPinCode[CHAR_LEN_32];
}OM_WIFI_SSID_WPS_T;
typedef struct 
{
    OM_WIFI_SSID_OPERATE_E eOperateFlag;
    UINT32 uiNvramId;
    UINT32 uiWifiId;
    UINT32 uiWlanId;
    CHAR acSsid[SSID_NAME_LEN];
    CHAR acSsidState[CHAR_LEN_32];
    UINT32 uiSsidSecType;
    CHAR acSsidWpaPskPasswd[WPA_KEY_LEN];
    OM_WIFI_SSID_WPS_T stWps;
}OM_WIFI_SSID_QUICK_SET_T;

/* ssid相关 */
typedef enum 
{
    SSID_AUTH_OPEN      = 1,
    SSID_AUTH_SHARED    = 2,
    SSID_AUTH_WPAPSK    = 3,
    SSID_AUTH_WPA2PSK   = 4,
    SSID_AUTH_WPA_WPA2_PSK = 5,
    SSID_AUTH_WPA       = 6,
    SSID_AUTH_WPA2      = 7,
    SSID_AUTH_WPA_WPA2  = 8,
}OM_WIFI_SSID_AUTH_TYPE_E;

typedef enum 
{
    AUTH_OPEN_DISABLE = 0,
    AUTH_OPEN_ENABLE  = 1
}OM_WIFI_SSID_OPEN_MODE_E;

typedef enum 
{
    WEP_KEY_64BIT   = 1,
    WEP_KEY_128BIT  =2
}OM_WIFI_SSID_WEP_KEY_LEN_E;

typedef enum 
{
    WEP_KEY_HEX     = 1,
    WEP_KEY_ASCII   = 2
}OM_WIFI_SSID_WEP_KEY_FORMAT_E;

typedef enum 
{
    WPA_ENCRYPT_AUTO = 0,
    WPA_ENCRYPT_TKIP,
    WPA_ENCRYPT_AES
}OM_WIFI_SSID_WPA_ENCRYPT_E;

typedef enum 
{
    SSID_ENCODE_UTF_8 = 0, 
    SSID_ENCODE_GB2312
}OM_WIFI_SSID_ENCODE_E;

typedef struct 
{
    UINT32 uiNvramId;
    CHAR acSsid[SSID_NAME_LEN];
    UINT32 uiSsidHideSwitch;
    OM_WIFI_SSID_ENCODE_E eSsidEncodeSelect;
    UINT32 uiWlanMaxUser;
    UINT32 uiAutoDelRelSwitch;
    UINT32 uiAutoDelRelTime;
}OM_WIFI_SSID_BASIC_CFG_T;


typedef struct 
{
    OM_WIFI_SSID_WEP_KEY_LEN_E eWepKeyLen;
    OM_WIFI_SSID_WEP_KEY_FORMAT_E eWepKeyFormat;
    UINT32 uiWepKeyIndex;
    CHAR   acWepKey[WEP_KEY_LEN]; 
}OM_WIFI_SSID_WEP_T;

typedef struct 
{
    OM_WIFI_SSID_OPEN_MODE_E eWepType; 
    OM_WIFI_SSID_WEP_T stWep;
}OM_WIFI_SSID_AUTH_OPEN_T;

typedef struct 
{
    OM_WIFI_SSID_WEP_T stWep;
}OM_WIFI_SSID_AUTH_SHARED_T;

typedef struct 
{
    OM_WIFI_SSID_WPA_ENCRYPT_E eWpaType;
    CHAR   acWpaKey[WPA_KEY_LEN];
    UINT32 uiWpaRenewal;
}OM_WIFI_SSID_AUTH_WPA_PERSONAL_T;

typedef struct 
{
    OM_WIFI_SSID_WPA_ENCRYPT_E eWpaType;
    UINT8 aucAuthSvrIp[4];//认证服务器
    UINT32 uiAuthSvrPort;
    CHAR acAuthSvrKey[64];
    UINT8 aucAcctSvrIp[4];//计费服务器相关
    UINT32 uiAcctSvrPort;
    CHAR acAcctSvrKey[64];
    UINT32 uiAcctMsgInterim;//认证消息间隔
    UINT32 uiSendInterval; //radius服务器老化时间
    
}OM_WIFI_SSID_AUTH_WPA_ENTERPRISE_T;


typedef struct 
{
    UINT32 uiNvramId;
    OM_WIFI_SSID_AUTH_TYPE_E eAuthType;

    union{
        OM_WIFI_SSID_AUTH_OPEN_T stAuthOpen;
        OM_WIFI_SSID_AUTH_SHARED_T  stAuthShared;
        OM_WIFI_SSID_AUTH_WPA_PERSONAL_T stAuthWpaPer;
        OM_WIFI_SSID_AUTH_WPA_ENTERPRISE_T stAuthWpaEnt;
    }u;
    
}OM_WIFI_SSID_ENCRYPT_CFG_T;

typedef struct 
{
    OM_WIFI_SSID_BASIC_CFG_T  stBasicCfg;
    OM_WIFI_SSID_ENCRYPT_CFG_T stSecurityCfg;
}OM_WIFI_SSID_CFG_T;


/* WIFI Flowctrl */

#define FLOWCTRL_BURST_RATE     5000

typedef enum{
    WIFI_FLOWCTRL_DISABLE = 0,
    WIFI_FLOWCTRL_ENABLE = 1
}OM_WIFI_FLOWCTRL_SWITCH_E;

typedef struct {
    OM_WIFI_FLOWCTRL_SWITCH_E eFlowctrlEnable;
    UINT32 uiUpRate;
    UINT32 uiDownRate;
}OM_WIFI_FLOWCTRL_CONFIG_T;


/* 射频相关 */

typedef enum 
{
    IEEE80211_B_ONLY     = 1,
    IEEE80211_G_ONLY     = 2,
    IEEE80211_BG         = 3,
    IEEE80211_N_ONLY     = 4,
    IEEE80211_BGN        = 5,
    IEEE80211_GN         = 6
}OM_WIFI_RF_WIFI_MODE_E;

typedef enum 
{
    RF_BAND_20MHZ = 1,
    RF_BAND_40MHZ ,
    RF_BAND_20_40MHZ_AUTO
}OM_WIFI_RF_BAND_E;

typedef struct 
{
    UINT32  uiWifiSwitch;
    UINT32  uiFreq;         /* 工作频段 1:2.4G 2:5G */
    OM_WIFI_RF_WIFI_MODE_E eWifiMode;
    UINT32  uiChannel;
    UINT32  uiCurChannel;
    OM_WIFI_RF_BAND_E eBand;
    UINT32  uiRfNum;
    UINT32  uiShortGiSw;
    UINT32  uiAmpduSw;
    UINT32  uiAmsduSw;
    UINT32  uiMaxTxPowLimit;
    UINT32  uiTxPower;

}OM_WIFI_RF_CFG_T;


/***************************** 函数声明 ********************************/
/* wifi basic ssid */
extern OM_ERROR_CODE_E  om_wifi_ssid_basic_config_get( OM_WIFI_SSID_BASIC_CFG_T *pstSsidBasicCfg);
extern OM_ERROR_CODE_E  om_wifi_ssid_basic_config_nvram( OM_WIFI_SSID_BASIC_CFG_T *pstSsidBasicCfg,
                                                                OM_WIFI_SSID_BASIC_CFG_T *pstSsidBasicCfgOld,
                                                                OM_WIFI_WEB_ALERT_E *peAlertFlag);
extern OM_ERROR_CODE_E  om_wifi_ssid_basic_encrypt_change( OM_WIFI_SSID_BASIC_CFG_T *pstSsidBasicCfg,
                                                                OM_WIFI_SSID_BASIC_CFG_T *pstSsidBasicCfgOld );

extern OM_ERROR_CODE_E  om_wifi_ssid_basic_config_module( OM_WIFI_SSID_BASIC_CFG_T *pstSsidBasicCfg,OM_WIFI_WEB_ALERT_E *peAlertFlag);
extern OM_ERROR_CODE_E  om_wifi_ssid_basic_config_set( OM_WIFI_SSID_BASIC_CFG_T *pstSsidBasicCfg,OM_WIFI_WEB_ALERT_E *peAlertFlag);

/* wifi encrypt  */
extern OM_ERROR_CODE_E  om_wifi_ssid_encrypt_config_get( UINT32 uiNvramId,UINT32 *puiAuthType,
                                                    OM_WIFI_SSID_AUTH_OPEN_T *pstAuthOpen,
                                                    OM_WIFI_SSID_AUTH_WPA_PERSONAL_T *pstWpaCfg, 
                                                    OM_WIFI_SSID_AUTH_WPA_ENTERPRISE_T *pstWpaEntCfg);
extern OM_ERROR_CODE_E  om_wifi_ssid_encrypt_config_nvram(OM_WIFI_SSID_ENCRYPT_CFG_T *pstSsidEncryCfg,OM_WIFI_WEB_ALERT_E *peAlertFlag);
extern OM_ERROR_CODE_E  om_wifi_ssid_encrypt_config_module(OM_WIFI_SSID_ENCRYPT_CFG_T *pstSsidEncryCfg,OM_WIFI_WEB_ALERT_E *peAlertFlag);
extern OM_ERROR_CODE_E  om_wifi_ssid_encrypt_config_set(OM_WIFI_SSID_ENCRYPT_CFG_T *pstSsidEncryCfg,OM_WIFI_WEB_ALERT_E *peAlertFlag);

extern  OM_ERROR_CODE_E  om_wifi_ssid_config_get( OM_WIFI_SSID_CFG_T *pstWifiSsidCfg,
                                                    OM_WIFI_SSID_AUTH_WPA_PERSONAL_T *pstWpaCfg, 
                                                    OM_WIFI_SSID_AUTH_WPA_ENTERPRISE_T *pstWpaEntCfg);
extern  OM_ERROR_CODE_E  om_wifi_ssid_config_nvram( OM_WIFI_SSID_CFG_T *pstSsidCfg,OM_WIFI_WEB_ALERT_E *peAlertFlag);
extern  OM_ERROR_CODE_E  om_wifi_ssid_config_module(OM_WIFI_SSID_CFG_T *pstSsidCfg,OM_WIFI_WEB_ALERT_E *peAlertFlag);
extern  OM_ERROR_CODE_E  om_wifi_ssid_config_set( void *pvData,OM_WIFI_WEB_ALERT_E *peAlertFlag);

/* wifi rf */
extern  OM_ERROR_CODE_E  om_wifi_rf_config_get( OM_WIFI_RF_CFG_T *pstRfCfg);
extern  OM_ERROR_CODE_E  om_wifi_rf_config_nvram( OM_WIFI_RF_CFG_T *pstRfCfg);
extern  OM_ERROR_CODE_E  om_wifi_rf_config_module( OM_WIFI_RF_CFG_T *pstRfCfg);
extern  OM_ERROR_CODE_E  om_wifi_rf_config_set( void *pvData);

/* wifi ssid interface */
LOCAL int om_wifi_socket_create( void );
LOCAL OM_ERROR_CODE_E  om_wifi_socket_close( INT32 iFd );
REMOTE OM_ERROR_CODE_E  om_wifi_device_up( CHAR *pcVapName );
REMOTE OM_ERROR_CODE_E  om_wifi_device_down( CHAR *pcVapName );
REMOTE OM_ERROR_CODE_E  om_wifi_vap_down_all( UINT32 uiWifiId );
REMOTE OM_ERROR_CODE_E  om_wifi_vap_up_all( UINT32 uiWifiId );
REMOTE OM_ERROR_CODE_E  om_wifi_ssid_name_set( CHAR *pcVapName, CHAR *pcSsidName);
REMOTE OM_ERROR_CODE_E  om_wifi_ssid_hide_set( CHAR *pcVapName,UINT32 uiSwitch );
REMOTE OM_ERROR_CODE_E  om_wifi_ssid_max_user_set( CHAR *pcVapName,UINT32 uiUserMax);
REMOTE OM_ERROR_CODE_E  om_wifi_ssid_encrypt_set(CHAR *pcVapName, CHAR *pcSsidName,OM_WIFI_SSID_ENCRYPT_CFG_T *pstSecurityCfg );
REMOTE OM_ERROR_CODE_E  om_wifi_ssid_encrypt_clear(CHAR *pcVapName );
REMOTE OM_ERROR_CODE_E  om_wifi_ssid_encrypt_wep( CHAR *pcVapName, OM_WIFI_SSID_WEP_T *pstWep );
REMOTE OM_ERROR_CODE_E  om_wifi_ssid_encrypt_open( CHAR *pcVapName,OM_WIFI_SSID_AUTH_OPEN_T *pstAuthOpen );
REMOTE OM_ERROR_CODE_E  om_wifi_ssid_encrypt_shared( CHAR *pcVapName,OM_WIFI_SSID_WEP_T *pstWep );
REMOTE OM_ERROR_CODE_E  om_wifi_ssid_encrypt_wpa_personal( CHAR *pcVapName, CHAR *pcSsidName, UINT32 uiAuthType, OM_WIFI_SSID_AUTH_WPA_PERSONAL_T *pstWpaPersonal );
REMOTE OM_ERROR_CODE_E  om_wifi_ssid_encrypt_wpa_enterprise( CHAR *pcVapName, CHAR *pcSsidName, UINT32 uiAuthType, OM_WIFI_SSID_AUTH_WPA_ENTERPRISE_T *pstWpaEnterprise );
REMOTE OM_ERROR_CODE_E  om_wifi_ssid_encrypt_wps_mode_set( CHAR *szVapName,CHAR *szSsidName);
REMOTE OM_ERROR_CODE_E  om_wifi_ssid_encrypt_wps_pbc(void);
REMOTE OM_ERROR_CODE_E  om_wifi_ssid_encrypt_wps_pin(CHAR *pcPin);


/* wifi rf interface */

REMOTE OM_ERROR_CODE_E  om_wifi_rf_wifi_mode_11b( const CHAR *szVapName);
REMOTE OM_ERROR_CODE_E  om_wifi_rf_wifi_mode_11g( const CHAR *szVapName);
REMOTE OM_ERROR_CODE_E  om_wifi_rf_wifi_mode_pure_g( CHAR *szVapName, INT32 iParam );
REMOTE OM_ERROR_CODE_E  om_wifi_rf_wifi_mode_pure_n( CHAR *szVapName, INT32 iParam );
REMOTE OM_ERROR_CODE_E  om_wifi_rf_wifi_band_set( OM_WIFI_RF_WIFI_MODE_E eWifiMode,OM_WIFI_RF_BAND_E eBand,CHAR *acVapName );
REMOTE OM_ERROR_CODE_E  om_wifi_rf_wifi_mode_set( OM_WIFI_RF_WIFI_MODE_E eWifiMode,OM_WIFI_RF_BAND_E eBand,CHAR *acVapName );
REMOTE OM_ERROR_CODE_E  om_wifi_rf_channel_set( CHAR *szVapName,UINT32 uiChannel );
REMOTE OM_ERROR_CODE_E  om_wifi_rf_channel_get( CHAR *szVapName,UINT32 *puiChannel);
REMOTE OM_ERROR_CODE_E  om_wifi_rf_rfnum_set( CHAR *szWifiName,UINT32 uiRfNum );
REMOTE OM_ERROR_CODE_E  om_wifi_rf_shortgi_set( CHAR *szVapName,UINT32 uiShortGi );
REMOTE OM_ERROR_CODE_E  om_wifi_rf_ampdu_set( CHAR *szVapName,UINT32 uiAmpdu );
REMOTE OM_ERROR_CODE_E  om_wifi_rf_amsdu_set( CHAR *szVapName,UINT32 uiAmsdu );
REMOTE OM_ERROR_CODE_E  om_wifi_rf_maxtxpowlimit_set( CHAR *szWifiName,UINT32 uiLimit );
REMOTE OM_ERROR_CODE_E  om_wifi_rf_txpower_set( CHAR *szVapName,UINT32 uiTxPower );

/* wifi quick set  */
extern OM_ERROR_CODE_E  om_wifi_array_add_someone( CHAR *src,int id );
extern OM_ERROR_CODE_E  om_wifi_array_del_someone( CHAR *src,int id );
extern OM_ERROR_CODE_E  om_wifi_kill_hostapd( CHAR *szVapName );
extern OM_ERROR_CODE_E  om_wifi_vap_get_new_wlan_id( OM_WIFI_SSID_QUICK_SET_T *pstSsidQuickCfg );
extern OM_ERROR_CODE_E  om_wifi_auth_type_int_to_string( INT32 id,CHAR *dst );
extern OM_ERROR_CODE_E  om_wifi_vap_info_list_get( CHAR *szSsidInfoList,INT32 *piSsidNum);
extern OM_ERROR_CODE_E  om_wifi_vap_default_config_set( OM_WIFI_SSID_QUICK_SET_T *pstSsidQuickCfg,CHAR *pcVapName);
extern OM_ERROR_CODE_E  om_wifi_vap_default_config_nvram_set( OM_WIFI_SSID_CFG_T *pstSsidCfg,INT32 iNvramId,OM_WIFI_WEB_ALERT_E *peAlertFlag);
extern OM_ERROR_CODE_E  om_wifi_vap_create( OM_WIFI_SSID_QUICK_SET_T *pstSsidQuickCfg ,CHAR *szVapName, CHAR *szWifiName );
extern OM_ERROR_CODE_E  om_wifi_vap_del(OM_WIFI_SSID_QUICK_SET_T *pstSsidQuickCfg);
extern OM_ERROR_CODE_E  om_wifi_vap_quick_set_config_module(OM_WIFI_SSID_QUICK_SET_T *pstSsidQuickCfg,OM_WIFI_WEB_ALERT_E *peAlertFlag);
extern OM_ERROR_CODE_E  om_wifi_vap_nvram_set( OM_WIFI_SSID_QUICK_SET_T *pstSsidQuickCfg );
extern OM_ERROR_CODE_E  om_wifi_vap_nvram_unset( UINT32 uiNvramId );
extern OM_ERROR_CODE_E  om_wifi_vap_quick_set_config_nvram( OM_WIFI_SSID_QUICK_SET_T *pstSsidQuickCfg,OM_WIFI_WEB_ALERT_E *peAlertFlag );
extern OM_ERROR_CODE_E  om_wifi_wps_config_get( OM_WIFI_SSID_WPS_T *pstWps);
extern OM_ERROR_CODE_E  om_wifi_wps_config_nvram( OM_WIFI_SSID_WPS_T *pstWps);
extern OM_ERROR_CODE_E  om_wifi_wps_config_module(OM_WIFI_SSID_WPS_T *pstWps);




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __OM_WIFI_H__ */
