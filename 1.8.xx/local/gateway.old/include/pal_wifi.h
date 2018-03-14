#ifndef __PAL_WIFI_H__
#define __PAL_WIFI_H__

#include "ap_types.h"
#include "log.h"

#define VAP_NUMBER			32


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


#ifndef IEEE80211_OUI_LEN
#define IEEE80211_OUI_LEN 3
#endif
#ifndef IEEE80211_ADDR_LEN
#define IEEE80211_ADDR_LEN 6
#endif
#ifndef IEEE80211_SSID_LEN
#define IEEE80211_SSID_LEN 32
#endif

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

#define AP_MAX_RADIO_NUM 2
#define AP_RADIO_MAX_SSID_NUM 16
#define AP_MAX_VAP_NUM AP_MAX_RADIO_NUM*AP_RADIO_MAX_SSID_NUM

typedef struct
{
	UINT32  ulAuthenticationType; /* 0: disabled, 1:wep */
	UINT32  ulSecurityMode;     /*��������:1:open;
	                                    2: shared
	                                    3:wpa-psk
	                                    4:wpa2-psk
	                                    5:psk-mixed
	                                    6:wpa
	                                    7:wpa2
	                                    8:wpa-wpa2-mixed
	                            */
	UINT32  ulEncryptType; /*���ܻ���:0:TKIP; 1:AES*/      
	CHAR    acWpaKey[64];   
	UINT32  ulWpaKeyRenewal;
		      
	UINT32  ulWepKeyFormat; 
	UINT32  ulWepBit;
	CHAR    acWepKey[32];

}AP_VAP_ENCRYPT_T;
typedef struct
{
	unsigned int uiIndex;
	unsigned int uiWifiId;
	unsigned int uiVapId;
	unsigned int uiSsidMode;
	unsigned int uiSsidEnable;
	unsigned int uiSsidHide;
	char acVapName[8];				/*vap_name athxx*/
	unsigned char aucBssid[6];          /*bssid*/
	char acSsid[32];					/*ssid*/
	char network[32];					/*bridge*/
}AP_VAP_BASIC_T;

typedef struct
{
	unsigned int uiIndex;
	unsigned int uiSsidEnable;
	unsigned int uiSsidHide;
	char acVapName[8];
	unsigned char aucBssid[6];          /*bssid*/
	char acSsid[32];					/*ssid*/
	AP_VAP_ENCRYPT_T stVapEncrypt;
} AP_VAP_T;

typedef struct
{
	unsigned int uiRadioId;			    /*radio id*/
	unsigned int uiRadioType;           /*radio type: 0(2g), 1(5g)*/
	unsigned int uiVapNum;
	AP_VAP_T astVap[AP_RADIO_MAX_SSID_NUM];
}AP_RADIO_T;

typedef struct {
	
	u32 uiRadioNum;
	AP_RADIO_T astRadio[AP_MAX_RADIO_NUM];
}AP_WIFI_T;



typedef struct
{
	unsigned int uiShortGi;
	unsigned int uiRfnum;
	unsigned int uiBand;
}WIFI_ADVANCE_INFO_T;

typedef struct
{
	unsigned int uiHwMode;
	unsigned int uiChannelMode;
	unsigned int uiChannel;
	unsigned int ulMaxTxPower;
	unsigned int ulDecayTxPower;
	unsigned int ulRateMode;
	
}WIFI_BASIC_INFO_T;
typedef struct
{
	unsigned int uiWifiId;
	unsigned char ucWifiName[32];
	WIFI_BASIC_INFO_T stBasicInfo;
	WIFI_ADVANCE_INFO_T stAdvanceINfo;
}WIFI_INFO_T;

int pal_wifi_get_vap_name(unsigned int uiIndex,char *acVapName);
int pal_wifi_set_ssid(unsigned char *vap_name,unsigned char *ssid_name);
int pal_wifi_set_ssid_hide(char *vap_name, int hide);
int pal_wifi_set_ssid_enabled(char *vap_name, int enabled);
#endif
