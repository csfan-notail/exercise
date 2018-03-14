#ifndef AGENTTYPE_H
#define AGENTTYPE_H

#include   <sys/time.h>
#include   <time.h>


#ifndef AC_USRM_PROC_TS_FILE_EVT
#define AC_USRM_PROC_TS_FILE_EVT    "/proc/ts"
#endif
/* AP-WEB�޸�ʱά�����ص��������ݰ汾��*/
#define INC_CONFIG_VERSION(n)	(n)=((n)/6*6+6+4)

#define DEFAULT_VAP_SSID        "CMCC"

#define IEEE80211_ADDR_LEN 6
#define IEEE80211_IPADDR_LEN 4

#define INVALID_VAP_ID			0XFFFFFFFF			/* ��Ч��AP ID */
#define INVALID_VLAN_ID			0XFFFFFFFF			/* ��Ч��VLAN ID */
#define INVALID_WIFIDEVICE_ID	0XFFFFFFFF			/* ��Ч�������豸ID */
#define INVALID_WIFI_ID			0XFFFFFFFF			/* ��Ч������ģ��ID */
#define INVALID_PROTOCOL		0XFFFFFFFF			/* ��Ч��Э�� */
#define INVALID_DATA			0XFFFFFFFF			/* ��Ч������ */

#define INVALID_PERFCOUNTERGROUP_ID	0XFFFFFFFF	/* ��Ч�����ܼ�������ID */

#define WIFI_STATE_DISABLE			1
#define WIFI_STATE_ENABLE			2
#define WIFI_STATE_NEED				1
#define WIFI_STATE_UNNEED			0

/* hc add for dhcp realloc 2009-6-26 */
#define AC_ID_NONE          0			/* û��ac ����*/
#define SAMPLE_TIME_DEFAULT 10
#define SAMPLE_RATE_DEFAULT 1

#define AP_AGENT_OPSTATE_UNKNOWN          	0			/* ״̬δ֪ */
#define AP_AGENT_OPSTATE_NORMAL           	1			/* ���� */
#define AP_AGENT_OPSTATE_FAULT            	2			/* ���� */
#define AC_LINK_KEEPSTATE_FAULT			    3			/* ������·���� */

/* hc modified for cmcc test 2009-8-28 */
#define AC_LINK_STATE_UNKNOW                        0    /* δ֪*/
#define AC_LINK_STATE_NORMAL                        1    /* ����*/
#define AC_LINK_STATE_NO_DISCOVERY_RSP              2    /*δ����: û���յ�������Ӧ*/
#define AC_LINK_STATE_NO_CONNCET_RSP                3    /*δ����: û���յ�������Ӧ*/
#define AC_LINK_STATE_KEEPER_TIME_OUT               4    /*δ����: ���ʱ*/
#define AC_LINK_STATE_UP_LINK_ERR                   5    /*δ����: AC ������·���� */
#define AC_LINK_STATE_OTHER_REASON                  6    /*δ����: ����ԭ��*/

#define SWITCH_NONE					0			/* û���л� */
#define FAT_TO_FIT_AP				1			/* FAT AP�л���FIT AP */
#define FIT_TO_FAT_AP				2			/* FIT AP�л���FAT AP */

#define GW_STATE_INIT				0
#define GW_STATE_GW					1
#define GW_STATE_UNGW				2

#define IS_SET_ALL_NO				0
#define IS_SET_ALL_YES				1
#define VAP_TYPE_AP                 0
#define VAP_TYPE_ADHOC              1

#define KEEPER_STATE_DISCONNECTION	0
#define KEEPER_STATE_CONNECTION		1

#define OMA_TRUE       1
#define OMA_FALSE     0

/* BEGIN: Added by ������, 2012/12/16   PN:TASK #257 VAP���������ù��� */
#define AGENT_FLOWCTRL_INVALID_VALUE 0
/* END:   Added by ������, 2012/12/16   PN:TASK #257 VAP���������ù��� */
/* BEGIN: Added by liyoukun, 2014/1/7   PN: gps-info path */
#ifndef LTEFI_GPS_INFO_PATH
#define LTEFI_GPS_INFO_PATH     "/tmp/gps.info"
#endif
/* END:   Added by liyoukun, 2014/1/7   PN: gps-info path */

#ifdef WIN32
#define TEMP_CONFIG_FILE "c:\\agent-data.dat"
#else
#define TEMP_CONFIG_FILE "/tmp/agent-data.dat"
#endif

#ifdef WIN32
#define TEMP_DEVICE_SIGN_FILE "c:\\device-sign.dat"
#else
#define TEMP_DEVICE_SIGN_FILE "/tmp/device-sign.dat"
#endif

#ifdef WIN32
#define TEMP_ENREGISTER_FILE "c:\\maintennance_enregister.txt"
#else
#define TEMP_ENREGISTER_FILE "/tmp/maintennance_enregister.txt"
#endif

#ifdef WIN32
#define TEMP_ENREGISTER_INFO "c:\\enregister_info.dat"
#else
#define TEMP_ENREGISTER_INFO "/tmp/enregister_info.dat"
#endif

#ifdef WIN32
#define TEMP_SCRIPT_FILE "c:\\tmp"
#else
#define TEMP_SCRIPT_FILE "/tmp/appendScript"
#endif

#ifdef WIN32
#define TLV_CFDATA_FILE "c:\\oma_cfg.dat"
#else
#define TLV_CFDATA_FILE "/tmp/tlvConfigData.dat"
#endif

/* added by laiyongxing at 2009-1-21 */
#ifdef WIN32
#define TEMP_LOGOIMAGE_FILE "c:\\tmp"
#else
#define TEMP_LOGOIMAGE_FILE "/tmp/logo.jpg"
#endif

#ifdef WIN32
#define SYS_SNAPSHOT_FILE "c:\\tmp\\syssnapshot.rar"
#else
#define SYS_SNAPSHOT_FILE              "/tmp/syssnapshot.tar.gz"
#define SYS_SNAPSHOT_TEMP_FILE    "/tmp/syssnapshot.tar"
#define SYS_SNAPSHOT_FILE_DIR       "/tmp/snapfiles"
#define SYS_USNAPSHOT_FILE            "/tmp/snapfiles/usnapshot"
#define SYS_MSNAPSHOT_FILE            "/tmp/snapfiles/msnapshot"
#endif

#define SYS_LOGO_INFO_FILE "/tmp/syslogo_info.dat"
#define SYS_HTTPD_LOGO_BAKFILE "/home/httpd/logo_bak.jpg"
#define SYS_HTTPD_LOGO_FILE "/home/httpd/logo.jpg"
#define SYS_HTML_TOPFRAME_FILE "/home/httpd/TopFrame.html"
#define SYS_HTML_TOPFRAME_BAKFILE "/home/httpd/TopFrame_bak.html"
//end addition

#define DEVICE_INFO_FILE	"/tmp/device-info.dat"
#define MSCFG_FILE	"/tmp/mscfg"
/* hc add 2009-12-2 */
#define OMA_IPADDR_RECORD "/tmp/ipaddr_record"

#define OPERATOR_CMCC           0
#define OPERATOR_UNICOM         1
#define OPERATOR_TELCOM         2

#define SSID_CMCC               "CMCC"
#define SSID_UNICOM             "chinaunicom"
#define SSID_TELCOM             "ChinaNet"

#define OPERATOR_CONF_FILE      "/etc/operator.conf"

#define ROGUE_AP_MAX		64                                              /* �Ƿ�AP���� */

#define IEEE80211_ADDR_LEN  6                                               /* IEEE80211 ��ַ���� */

#define wifiModule_SIZE						4
#define mainDeviceWifiModule_SIZE			2
#define channel_SIZE				        16

#ifndef AC_MOI_LEN
#define AC_MOI_LEN	16
#endif

#define OMA_SNMP_GET_PORT       161
#define OMA_SNMP_TRAP_PORT      162

#define OMA_SNMP_COMMUNITY      "www.gbcom.com.cn"

typedef enum
{
	AP93_TYPE,
	PB44_TYPE,
	AP9344_TYPE,
	IXP_TYPE,
	/* BEGIN: Added by lvjianlin, 2013/4/25   PN:ltefi  */
	TA981_TYPE
	/* END:   Added by lvjianlin, 2013/4/25   PN:ltefi  */
}AP_TYPE;

typedef struct wifi_amp_cfg
{
       unsigned char      ucWifiDevAmpSwitch;
       unsigned char      ucWifiDevAmpGain;
       unsigned char      ucWifiDevAmpMaxInput;
       unsigned char      ucRsv;
}T_WIFI_AMP_CFG;

#if defined(AP9344_OAM) || defined(AP9341_OAM)
/* 9344 �� wallAPƽ̨ */
typedef struct deviceSign_s
{
	long	lDeviceType;				/* �豸���� */
	/* -------------------------------------------------------------------------------------------------------- */
	/*	������ԭ���ǣ�000 000 000 ǰ����λ�����豸ϵ�У��м���λ������Ч�����������λ�������豸����          */
	/* 	��001��001��001����1001001��																			*/
	/*  ��һ��001��ʾ����MESH·����ϵ�У��ڶ���001��ʾʹ����1������������������001��ʾ�����Ļش�����			*/
	/* -------------------------------------------------------------------------------------------------------- */
	/* ����MESH·���� */
#define sysDeviceType_B1100e		1001001
#define sysDeviceType_B1100e_I      1001009
#define sysDeviceType_B1100e_T		1001002
#define sysDeviceType_B1100e_S		1001003/* ��ɾ�� */
#define sysDeviceType_B1100e_ST		1001004/* ��ɾ�� */
#define sysDeviceType_B1010e		1001005/* ��ɾ�� */
#define sysDeviceType_B1010e_T		1001006/* ��ɾ�� */
#define sysDeviceType_B1010e_S		1001007/* ��ɾ�� */
#define sysDeviceType_B1010e_ST		1001008/* ��ɾ�� */

#define sysDeviceType_B2011e		1002001/* ��ɾ�� */
#define sysDeviceType_B2011e_T		1002002/* ��ɾ�� */
#define sysDeviceType_B2011e_S		1002003/* ��ɾ�� */
#define sysDeviceType_B2011e_ST		1002004/* ��ɾ�� */

#define sysDeviceType_B2101e		1002005
#define sysDeviceType_B2101e_I      1002019
#define sysDeviceType_B2101e_T		1002006
#define sysDeviceType_B2101e_S		1002007/* ��ɾ�� */
#define sysDeviceType_B2101e_ST		1002008/* ��ɾ�� */

#define sysDeviceType_B2020e		1002009/* ��ɾ�� */
#define sysDeviceType_B2020e_T		1002010/* ��ɾ�� */
#define sysDeviceType_B2020e_ST		1002011/* ��ɾ�� */

#define sysDeviceType_B2200e		1002012
#define sysDeviceType_B2200e_I      1002020
#define sysDeviceType_B2200e_T		1002013
#define sysDeviceType_B2200e_ST		1002014/* ��ɾ�� */

#define sysDeviceType_B2101e_E		1002015
#define sysDeviceType_B2200e_Q		1002016/* ��ɾ�� */
#define sysDeviceType_B2200e_QT		1002017/* ��ɾ�� */
#define sysDeviceType_B3102e_F		1002018

#define sysDeviceType_B3102e		1003001
#define sysDeviceType_B3102e_T		1003002
#define sysDeviceType_B3201e		1003003
#define sysDeviceType_B3201e_T		1003004
#define sysDeviceType_B3300e		1003005
#define sysDeviceType_B3300e_T		1003006

#define sysDeviceType_B4202e		1004001
#define sysDeviceType_B4202e_T		1004002
#define sysDeviceType_B4301e		1004003
#define sysDeviceType_B4301e_T		1004004
#define sysDeviceType_B4400e		1004005
#define sysDeviceType_B4400e_4T		1004006/* ���ͺţ�B4400e-T*/
#define sysDeviceType_B4400e_2T		1004007/* �����ͺ� */

	/* ����MESH�ն� */
#define sysDeviceType_GT100			2001001
#define sysDeviceType_GT200			2002001
#define sysDeviceType_GT201			2002002
#define sysDeviceType_GT201_A		2002003/* ��ɾ�� */

	/* ������������ */
#define sysDeviceType_GQ100			3001001/* ��ɾ�� */
#define sysDeviceType_GQ100_T		3001002/* ��ɾ�� */
#define sysDeviceType_GQ100_S		3001003/* ��ɾ�� */
#define sysDeviceType_GQ100_ST		3001004/* ��ɾ�� */
#define sysDeviceType_GQ200			3002001/* ��ɾ�� */
#define sysDeviceType_GQ200_T		3002002/* ��ɾ�� */
#define sysDeviceType_GQ200_S		3002003/* ��ɾ�� */
#define sysDeviceType_GQ200_ST		3002004/* ��ɾ�� */

	/* �������߽���� */
#define sysDeviceType_GA100i_H		4001001
#define sysDeviceType_GA100i_A		4001002
#define sysDeviceType_GA100i_HE		4001003/* ���ͺţ�GA100i_AE */
#define sysDeviceType_GA100i_HE_I	4001004
#define sysDeviceType_GA100i_H_I	4001005
#define sysDeviceType_GA100i_A_I	4001006
#define sysDeviceType_GA100i_AE		4001007
#define sysDeviceType_GA100i_AE_I	4001008
#define sysDeviceType_GA200i		4002001
#define sysDeviceType_GA200i_M		4002002
#define sysDeviceType_GA200i_MT		4002003
#define sysDeviceType_GA200i_D		4002004/* ��ɾ�� */
#define sysDeviceType_GA200i_A		4002005/* ��ɾ�� */
#define sysDeviceType_GA200i_AD		4002006/* ��ɾ�� */

#define sysDeviceType_GA200i_EM		4002007
#define sysDeviceType_GA200i_EM_I   4002011
#define sysDeviceType_GA200i_AE		4002008/* ��ɾ�� */
#define sysDeviceType_GA200i_E      4002009
#define sysDeviceType_GA200i_E_I    4002010

	/* �������߽���� */
#define sysDeviceType_GA100e		5001001
#define sysDeviceType_GA100e_A		5001002/* ��ɾ�� */
#define sysDeviceType_GA100e_S		5001003/* ��ɾ�� */
#define sysDeviceType_GA100e_AS		5001004/* ��ɾ�� */
#define sysDeviceType_GA100e_E		5001005/* ���ͺţ�GA100e_AE */
#define sysDeviceType_GA100e_I		5001006
#define sysDeviceType_GA200e		5002001
#define sysDeviceType_GA200e_A		5002002/* ��ɾ�� */
#define sysDeviceType_GA200e_D		5002003/* ��ɾ�� */
#define sysDeviceType_GA200e_S		5002004/* ��ɾ�� */
#define sysDeviceType_GA200e_AD		5002005/* ��ɾ�� */
#define sysDeviceType_GA200e_AS		5002006/* ��ɾ�� */
#define sysDeviceType_GA200e_DS		5002007/* ��ɾ�� */
#define sysDeviceType_GA200e_ADS	5002008/* ��ɾ�� */
#define sysDeviceType_GA200e_E		5002009/* ���ͺţ�GA200e_AE */
#define sysDeviceType_GA200e_I		5002010


	/* �����ͨ���߻�վ */
#define sysDeviceType_GS100B_L		6001001/* ��ɾ�� */
#define sysDeviceType_GS100B		6001002
#define sysDeviceType_GS100B_G		6001003/* �����ͺ� */
#define sysDeviceType_HB4_GS100BD	6002001/* ���ͺţ�GS200B */
#define sysDeviceType_GS200B		6002002/* �����ͺ� */
#define sysDeviceType_GS200B_G		6002003/* �����ͺ� */
#define sysDeviceType_GS400B		6004001/* ��ɾ�� */

	/* �����ͨ�����ն� */
#define sysDeviceType_GS100T_L		7001001/* ��ɾ�� */
#define sysDeviceType_GS100T		7001002
#define sysDeviceType_GS100T_G		7001003/* �����ͺ� */
#define sysDeviceType_GS200T		7002001
#define sysDeviceType_GS201T		7002002/* ��ɾ�� */
#define sysDeviceType_GS200T_G		7002003/* �����ͺ� */

	/* HT2����WLAN�ն� */
#define  sysDeviceType_GRR100		8001001
#define  sysDeviceType_GRR200		8002001
#define  sysDeviceType_GRR201		8002003

	/* HB4����WLAN�ն� */
#define  sysDeviceType_GRR100e		8001002
#define  sysDeviceType_GRR200e		8002002

	/* 2010-7-26 ����: 11n�豸�ͺ�*/
#define  sysDeviceType_N200H_GA100i_GN11		9001001  // ����11gn��500mW��mimo 1��1		11n���ڴ��ʽ���㣨500mW��
#define  sysDeviceType_N200_GA100i_GN22         9001002  // ����11gn��100mW��mimo 2��2		11n����С���ʽ���㣨100mW��
#define  sysDeviceType_N200_GA200i_AGN22        9002001  // ˫��11agn��100mW��mimo 2��2		11n����˫ƵС���ʽ���㣨100mW��

/* 2011-6-1 ���Ų�������*/
#define sysDeviceType_NB4_GA100e_GN22			10001001		/************* ʹ����:11gn,��Ƶ2*2,����,500mW(2 chain) */
#define sysDeviceType_NB4_GA200e_AGN22			10001002		/************* ʹ����:11agn,˫Ƶ2*2,����,11gn/500mW(2 chain),11an/100mW(2 chain) */

/* BEGIN: Added by lvjianlin, 2013/4/25   PN:ltefi  */
#define sysDeviceType_LTE_Fi_AGN22  9002002             //LTEFI ���ݺ�һ
/* END:   Added by lvjianlin, 2013/4/25   PN:ltefi  */

	unsigned char	aucDeviceId[6];		/* �豸ID��6��16������ */
	unsigned long	ulDeviceModuleType;	/* �豸��������*/
#define sysDeviceModuleType_ALONE	0
#define sysDeviceModuleType_MAIN	1
#define sysDeviceModuleType_SLAVE	2
#define WIFI_DEV_AMP_SET_MASK  	    0x01
#define WIFI_DEV_AMP_VALUE_MASK     0xf8
    T_WIFI_AMP_CFG    stModuleWifiAmpCfg1;
    T_WIFI_AMP_CFG    stModuleWifiAmpCfg2;
    
	unsigned char aucCustomer[64];  /* ���ÿͻ���� , ���ַ�����ʽ�Ŀͻ����� Ĭ��ֵȫ0 */
	unsigned char aucRsv1[64];      /* Ԥ���ֶ�1��Ĭ��ֵΪȫ0 */
	unsigned char aucRsv2[64];      /* Ԥ���ֶ�2��Ĭ��ֵΪȫ0 */
	unsigned char aucRsv3[64];      /* Ԥ���ֶ�3��Ĭ��ֵΪȫ0 */
	unsigned char aucRsv4[64];      /* Ԥ���ֶ�4��Ĭ��ֵΪȫ0 */

	int	setFlag;				    /* �Ƿ��������豸���ͱ�ʶ */
} T_deviceSign;
#else
/* AP93, BP44, IXP ƽ̨�Ĳ������ṹ���� */
typedef struct deviceSign_s
{
	long	lDeviceType;				/* �豸���� */
	/* -------------------------------------------------------------------------------------------------------- */
	/*	������ԭ���ǣ�000 000 000 ǰ����λ�����豸ϵ�У��м���λ������Ч�����������λ�������豸����          */
	/* 	��001��001��001����1001001��																			*/
	/*  ��һ��001��ʾ����MESH·����ϵ�У��ڶ���001��ʾʹ����1������������������001��ʾ�����Ļش�����			*/
	/* -------------------------------------------------------------------------------------------------------- */
	/* ����MESH·���� */
#define sysDeviceType_B1100e		1001001
#define sysDeviceType_B1100e_I      1001009
#define sysDeviceType_B1100e_T		1001002
#define sysDeviceType_B1100e_S		1001003/* ��ɾ�� */
#define sysDeviceType_B1100e_ST		1001004/* ��ɾ�� */
#define sysDeviceType_B1010e		1001005/* ��ɾ�� */
#define sysDeviceType_B1010e_T		1001006/* ��ɾ�� */
#define sysDeviceType_B1010e_S		1001007/* ��ɾ�� */
#define sysDeviceType_B1010e_ST		1001008/* ��ɾ�� */

#define sysDeviceType_B2011e		1002001/* ��ɾ�� */
#define sysDeviceType_B2011e_T		1002002/* ��ɾ�� */
#define sysDeviceType_B2011e_S		1002003/* ��ɾ�� */
#define sysDeviceType_B2011e_ST		1002004/* ��ɾ�� */

#define sysDeviceType_B2101e		1002005
#define sysDeviceType_B2101e_I      1002019
#define sysDeviceType_B2101e_T		1002006
#define sysDeviceType_B2101e_S		1002007/* ��ɾ�� */
#define sysDeviceType_B2101e_ST		1002008/* ��ɾ�� */

#define sysDeviceType_B2020e		1002009/* ��ɾ�� */
#define sysDeviceType_B2020e_T		1002010/* ��ɾ�� */
#define sysDeviceType_B2020e_ST		1002011/* ��ɾ�� */

#define sysDeviceType_B2200e		1002012
#define sysDeviceType_B2200e_I      1002020
#define sysDeviceType_B2200e_T		1002013
#define sysDeviceType_B2200e_ST		1002014/* ��ɾ�� */

#define sysDeviceType_B2101e_E		1002015
#define sysDeviceType_B2200e_Q		1002016/* ��ɾ�� */
#define sysDeviceType_B2200e_QT		1002017/* ��ɾ�� */
#define sysDeviceType_B3102e_F		1002018

#define sysDeviceType_B3102e		1003001
#define sysDeviceType_B3102e_T		1003002
#define sysDeviceType_B3201e		1003003
#define sysDeviceType_B3201e_T		1003004
#define sysDeviceType_B3300e		1003005
#define sysDeviceType_B3300e_T		1003006

#define sysDeviceType_B4202e		1004001
#define sysDeviceType_B4202e_T		1004002
#define sysDeviceType_B4301e		1004003
#define sysDeviceType_B4301e_T		1004004
#define sysDeviceType_B4400e		1004005
#define sysDeviceType_B4400e_4T		1004006/* ���ͺţ�B4400e-T*/
#define sysDeviceType_B4400e_2T		1004007/* �����ͺ� */

	/* ����MESH�ն� */
#define sysDeviceType_GT100			2001001
#define sysDeviceType_GT200			2002001
#define sysDeviceType_GT201			2002002
#define sysDeviceType_GT201_A		2002003/* ��ɾ�� */

	/* ������������ */
#define sysDeviceType_GQ100			3001001/* ��ɾ�� */
#define sysDeviceType_GQ100_T		3001002/* ��ɾ�� */
#define sysDeviceType_GQ100_S		3001003/* ��ɾ�� */
#define sysDeviceType_GQ100_ST		3001004/* ��ɾ�� */
#define sysDeviceType_GQ200			3002001/* ��ɾ�� */
#define sysDeviceType_GQ200_T		3002002/* ��ɾ�� */
#define sysDeviceType_GQ200_S		3002003/* ��ɾ�� */
#define sysDeviceType_GQ200_ST		3002004/* ��ɾ�� */

	/* �������߽���� */
#define sysDeviceType_GA100i_H		4001001
#define sysDeviceType_GA100i_A		4001002
#define sysDeviceType_GA100i_HE		4001003/* ���ͺţ�GA100i_AE */
#define sysDeviceType_GA100i_HE_I	4001004
#define sysDeviceType_GA100i_H_I	4001005
#define sysDeviceType_GA100i_A_I	4001006
#define sysDeviceType_GA100i_AE		4001007
#define sysDeviceType_GA100i_AE_I	4001008
#define sysDeviceType_GA200i		4002001
#define sysDeviceType_GA200i_M		4002002
#define sysDeviceType_GA200i_MT		4002003
#define sysDeviceType_GA200i_D		4002004/* ��ɾ�� */
#define sysDeviceType_GA200i_A		4002005/* ��ɾ�� */
#define sysDeviceType_GA200i_AD		4002006/* ��ɾ�� */

#define sysDeviceType_GA200i_EM		4002007
#define sysDeviceType_GA200i_EM_I   4002011
#define sysDeviceType_GA200i_AE		4002008/* ��ɾ�� */
#define sysDeviceType_GA200i_E      4002009
#define sysDeviceType_GA200i_E_I    4002010

	/* �������߽���� */
#define sysDeviceType_GA100e		5001001
#define sysDeviceType_GA100e_A		5001002/* ��ɾ�� */
#define sysDeviceType_GA100e_S		5001003/* ��ɾ�� */
#define sysDeviceType_GA100e_AS		5001004/* ��ɾ�� */
#define sysDeviceType_GA100e_E		5001005/* ���ͺţ�GA100e_AE */
#define sysDeviceType_GA100e_I		5001006
#define sysDeviceType_GA200e		5002001
#define sysDeviceType_GA200e_A		5002002/* ��ɾ�� */
#define sysDeviceType_GA200e_D		5002003/* ��ɾ�� */
#define sysDeviceType_GA200e_S		5002004/* ��ɾ�� */
#define sysDeviceType_GA200e_AD		5002005/* ��ɾ�� */
#define sysDeviceType_GA200e_AS		5002006/* ��ɾ�� */
#define sysDeviceType_GA200e_DS		5002007/* ��ɾ�� */
#define sysDeviceType_GA200e_ADS	5002008/* ��ɾ�� */
#define sysDeviceType_GA200e_E		5002009/* ���ͺţ�GA200e_AE */
#define sysDeviceType_GA200e_I		5002010


	/* �����ͨ���߻�վ */
#define sysDeviceType_GS100B_L		6001001/* ��ɾ�� */
#define sysDeviceType_GS100B		6001002
#define sysDeviceType_GS100B_G		6001003/* �����ͺ� */
#define sysDeviceType_HB4_GS100BD	6002001/* ���ͺţ�GS200B */
#define sysDeviceType_GS200B		6002002/* �����ͺ� */
#define sysDeviceType_GS200B_G		6002003/* �����ͺ� */
#define sysDeviceType_GS400B		6004001/* ��ɾ�� */

	/* �����ͨ�����ն� */
#define sysDeviceType_GS100T_L		7001001/* ��ɾ�� */
#define sysDeviceType_GS100T		7001002
#define sysDeviceType_GS100T_G		7001003/* �����ͺ� */
#define sysDeviceType_GS200T		7002001
#define sysDeviceType_GS201T		7002002/* ��ɾ�� */
#define sysDeviceType_GS200T_G		7002003/* �����ͺ� */

	/* HT2����WLAN�ն� */
#define  sysDeviceType_GRR100		8001001
#define  sysDeviceType_GRR200		8002001
#define  sysDeviceType_GRR201		8002003

	/* HB4����WLAN�ն� */
#define  sysDeviceType_GRR100e		8001002
#define  sysDeviceType_GRR200e		8002002

	/* 2010-7-26 ����: 11n�豸�ͺ�*/
#define  sysDeviceType_N200H_GA100i_GN11		9001001  // ����11gn��500mW��mimo 1��1		11n���ڴ��ʽ���㣨500mW��
#define  sysDeviceType_N200_GA100i_GN22         9001002  // ����11gn��100mW��mimo 2��2		11n����С���ʽ���㣨100mW��
#define  sysDeviceType_N200_GA200i_AGN22        9002001  // ˫��11agn��100mW��mimo 2��2		11n����˫ƵС���ʽ���㣨100mW��

/* 2011-6-1 ���Ų�������*/
#define sysDeviceType_NB4_GA100e_GN22			10001001		/************* ʹ����:11gn,��Ƶ2*2,����,500mW(2 chain) */
#define sysDeviceType_NB4_GA200e_AGN22			10001002		/************* ʹ����:11agn,˫Ƶ2*2,����,11gn/500mW(2 chain),11an/100mW(2 chain) */

/* BEGIN: Added by lvjianlin, 2013/4/25   PN:ltefi  */
#define sysDeviceType_LTE_Fi_AGN22  9002002             //LTEFI ���ݺ�һ
/* END:   Added by lvjianlin, 2013/4/25   PN:ltefi  */


	unsigned char	aucDeviceId[6];		/* �豸ID��6��16������ */
	int		setFlag;					/* �Ƿ��������豸���ͱ�ʶ */
	unsigned long	ulDeviceModuleType;	/* �豸��������*/
#define sysDeviceModuleType_ALONE	0
#define sysDeviceModuleType_MAIN	1
#define sysDeviceModuleType_SLAVE	2
#define WIFI_DEV_AMP_SET_MASK  	    0x01
#define WIFI_DEV_AMP_VALUE_MASK     0xf8
    T_WIFI_AMP_CFG    stModuleWifiAmpCfg1;
    T_WIFI_AMP_CFG    stModuleWifiAmpCfg2;
} T_deviceSign;
#endif

typedef struct aclBroadcastPolicy_s
{
     unsigned long ulMulticastSwitch;		/* ����ಥ��*/
#define MULTICAST_SWITCH_DISABLE     0		/* ��ȱʡΪ��*/
#define MULTICAST_SWITCH_ENABLE      1		/* ��*/
     unsigned long ulBroadcastSwitch;		/* ����㲥����ARP��DHCP��*/
#define BROADCAST_SWITCH_DISABLE     0		/* ��ȱʡΪ��*/
#define BROADCAST_SWITCH_ENABLE       1		/* ��*/
     unsigned long ulBMRateLimitSwitch;		/* �㲥/�ಥ �Ƿ�����*/
#define BM_RATE_LIMIT_SWITCH_DISABLE      0 /* ��*/
#define BM_RATE_LIMIT_SWITCH_ENABLE       1 /* �ǣ�ȱʡΪ��*/
     unsigned long ulMaxRate;               /* ������ʣ���/�룻ȱʡΪ*/
} T_ACL_BROADCAST_POLICY;

struct ieee80211_scan_ssid
{
	int len;					            /* length in bytes */
	unsigned char ssid[32];	                /* ssid contents */
};

typedef struct
{
	unsigned long ulDosAttackDetectSwitch;		/* DOS������� */
#define DOS_ATTACK_DETECT_SWITCH_OFF	0
#define DOS_ATTACK_DETECT_SWITCH_ON		1
	unsigned long ulDosAttackPeriod;			/* ���鹥��ͳ������ */
	unsigned long ulDosAttackThreshold;			/* ���鹥���ж����� */
	unsigned long ulDosAttackDefenseSwitch;		/* DOS�������� */
#define DOS_ATTACK_DEFENSE_SWITCH_OFF	0
#define DOS_ATTACK_DEFENSE_SWITCH_ON	1
	unsigned long ulInvalidTerminalAgeingTime;	/* �����ն��ϻ�ʱ�� */
	unsigned long ulInvalidApDetectSwitch;		/* ����AP��� */
	unsigned char aucInvalidApSpecificBssid[6];	/* ����AP����/ָ��BSSID */
	unsigned char aucResv1[2];
	unsigned char aucInvalidApSpecificOui[3];	/* ����AP����/ָ������ */
	unsigned char aucResv2[1];
	unsigned long ulInvalidApSameSsidSwitch;	/* ����AP����/ָ����ͬSSID */
	unsigned long ulInvalidApHiddenSsidSwitch;	/* ����AP����/ָ������SSID */
	unsigned char aucInvalidSpecificSsid[32];	/* ����AP����/ָ��SSID */
} WIDS_T;

/* hc modified for telecom test 2009-7-6 */
#define LEGAL_DEVICE_MAX    16

typedef struct{
	unsigned long  ulWtpId;
	unsigned long  wifiDeviceIndex;						/* �豸���             */	
	unsigned long  dos_detect_switch ;                  /* ���鹥����⿪��     */
	unsigned long  period ;                             /* ���鹥��ͳ�����ڣ���λ��ms��ȱʡֵΪ1000ms   */
	unsigned long  limit ;                              /* ���鹥�������հ������ޣ�ȱʡֵΪ50         */
	unsigned long  action ;                             /* ���鹥��������0���޷�����1���з��������붯̬����������ȱʡֵΪ1�����������鹥������ */
	unsigned long  time_aging ;                         /* ��̬�������ϻ�ʱ�䣬��λ��s��ȱʡֵΪ3600s   */
	unsigned long  spoof_detect_switch ;                /* SPOOF��⿪��        */
	unsigned long  weekiv_detect_switch ;               /* WEEK IV��⿪��      */
	unsigned long  sta_disturb_detect_switch ;          /* �ն˸��ż�⿪��     */
	unsigned long  sta_disturb_limit ;                  /* �ն˸������ޣ�������:pps */

	/* 2010/07/01 fengjing cmcc test */     
    unsigned long  ap_disturb_num_threshold;            /* AP���Ÿ澯���� */
	unsigned long  sta_disturb_num_threshold;           /*�ն˸��Ÿ澯����*/
    unsigned long  ap_cointer_threshold;                /* APͬƵ���Ÿ澯���ޣ������ź�ǿ�ȣ�dBm*/
    unsigned long  ap_neiborinter_threshold;            /* AP��Ƶ���Ÿ澯���ޣ������ź�ǿ�ȣ�dBm */
	
	unsigned long  legal_sta_num ;                      /* �Ϸ��ն����ø���     */
	unsigned char  legal_sta_mac[LEGAL_DEVICE_MAX][8] ; /* LEGAL_DEVICE_MAX: 16 */
	unsigned long  ap_detect_switch ;                   /* ����AP��⿪��       */
	unsigned long  legal_ssid_num ;
	unsigned char  legal_ssid_list[LEGAL_DEVICE_MAX][32] ;      /*SSID�ԡ�\0������*/
	unsigned long  legal_bssid_num ;
	unsigned char  legal_bssid_list[LEGAL_DEVICE_MAX][8] ;      /*legal_bssid_list[][6], legal_bssid_list[][7]���ֽڱ���*/
	unsigned long  legal_oui_num ;
	unsigned char  legal_oui_list[LEGAL_DEVICE_MAX][4] ;        /* legal_oui_list[][3] �ֽڱ���*/

	unsigned long  ulMOC;
	unsigned char  aucMOI[AC_MOI_LEN];
}WIDS_CONFIG_T ;

/* hc modified for telecom test 2009-7-6 */
typedef struct
{
    unsigned char   mac[6] ;                /*�����ն�MAC��ַ*/
    unsigned char   mac_pad[2] ;
    unsigned long   channel ;               /*�ŵ���*/
    int             rx_power_max ;          /*���յ�������ź�ǿ�ȣ���λ��dBm*/
    int             rx_power ;              /*���յ����ź�ǿ��*/
    int             noise ;                 /*���룬��λ��dBm*/
    unsigned long   is_adhoc ;              /*�����ն˲���ģʽ��0����ADHOC��1��ADHOC*/
    unsigned char   attack_bssid[6] ;       /*�ܹ���AP��BSSID*/
    unsigned char   attack_bssid_pad[2] ;
    unsigned long   time_first ;            /*�״μ��ʱ�䣬��λ���룬��1970.1.1 00:00:00������*/
    unsigned long   time_last ;             /*δ�μ��ʱ�䣬��λ����*/
    unsigned long   action ;                /*����ʩ��0���޷���ʩ��1���з���ʩ�����붯̬��������*/

    unsigned long   dos_attack ;            /*DOS������0����1����*/
    unsigned long   dos_attack_num ;        /*DOS��������*/
    unsigned long   spoof_attack ;          /*SPOOF������0����1����*/
    unsigned long   spoof_attack_num ;      /*SPOOF��������*/
    unsigned long   weekiv_attack ;         /*WEEKIV������0����1����*/
    unsigned long   weekiv_attack_num ;     /*WEEKIV��������*/
    unsigned long   disturb_attack ;        /*���ţ�0����1����*/
    unsigned long   disturb_attack_num ;    /*���Ŵ���*/
}ROGUE_STA_T ;


#define ROGUE_STA_MAX       16

typedef struct
{
    unsigned long       num ;
    ROGUE_STA_T         sta_list[ROGUE_STA_MAX] ;
}ROGUE_STA_LIST_T ;


/* ���ڷ�װ���ݵ�OMC����Ϣ */
typedef struct
{
	unsigned long       num;
	ROGUE_STA_T         sta_list[wifiModule_SIZE*ROGUE_STA_MAX];
} OMC_ROGUE_STA_T;


#define SCAN_MAX_VENDOR 5
typedef struct
{
	struct ieee80211_scan_ssid ssid;
	unsigned char bssid[IEEE80211_ADDR_LEN];         /* size of 802.11 address */
	unsigned char aucVendor[SCAN_MAX_VENDOR][3];
	unsigned char resv[3];
	unsigned long ulChannel;
    unsigned long ulSn;                             /*����ȣ���λ��dB*/
	int	          lRxPower;
	unsigned long ulFirstTime;
	unsigned long ulLastTime;
	unsigned long ulBeaconInterval;
	unsigned long ulEncrypt;
	/* hc modified for telecom test 2009-7-6 */
    unsigned long ulAction ;                /*�Ƿ�Կ���AP��ȡ�Բ�*/
    unsigned long ulType;			/* 1: ����AP 2: αAP */
} ROGUE_AP_T;

/* ���ڷ�װ�����߻�ȡ����Ϣ */
typedef struct
{
	//unsigned long ulWtpId;
	unsigned long   ulValidNum;
	ROGUE_AP_T      stRogueApList[ROGUE_AP_MAX];
} ROGUE_AP_INFO_T;

/* ���ڷ�װ���ݵ�OMC����Ϣ */
typedef struct
{
	unsigned long   ulWtpId;
	unsigned long   ulValidNum;
	ROGUE_AP_T      stRogueApList[ROGUE_AP_MAX];
} OMC_ROGUE_AP_T;

typedef struct configFileHeader_s
{
	long			deviceType;
	unsigned char	deviceId[6];
    unsigned short  cfgDataType;
#define dataType_OMA_STRUCT         0           /* OMA�ṹ */
#define dataType_OMC_SETALL         1           /* OMC setAll�ֽ��� */
	unsigned long	dataVersion;
//#define dataVersion_DV1			0X00000001
#define dataVersion_DV1_6_1_B	0X00000002			/* v1.6.1b */
#define dataVersion_DV1_8_0_A	0X00000003			/* v1.6.1b */

	long			dataState;
#define dataState_NONE 			0X11111111
#define dataState_VALID			0X22222222
	unsigned long	dataLen;						/* bytes of config data */
	unsigned long	checkSum;						/* check sum of whole file */
	int				iRsv[4];						/* Ԥ��λ */
} T_configFileHeader;
/* BEGIN: Added by ������, 2013/1/8   PN:TASK #296 ���ⳡ����ɽ���ƶ�Ҫ�󱾵�ת��ʱAP����Ҳ���㲥ҵ��SSID */
#define AGENT_DISABLE_SSID_DISABLE  0
#define AGENT_DISABLE_SSID_ENABLE   1
/* END:   Added by ������, 2013/1/8   PN:TASK #296 ���ⳡ����ɽ���ƶ�Ҫ�󱾵�ת��ʱAP����Ҳ���㲥ҵ��SSID */
typedef struct sysBasicSetup_s
{
	unsigned long	ulWtpId;
	/* hc modified for cmcc test 2009-7-18 */
	char            acWtpNEId[32];                  /* AP��Ԫ���� */
	long			sysDeviceType;
#define sysDeviceType_A210i 3						/* A210i */
#define sysDeviceType_A210e 4						/* A210e */
	unsigned char	sysDeviceId[6];					/* 6��16�����ַ� */
	char			acRsv[2];
	char			acContact[64];					/* �豸ά����ϵ�� */
	char			acLocation[64];					/* �豸����λ�� */
	char			acCountry[64];					/* �豸�������� */	
	unsigned long ulMaxStaNum;
	char			sysHwVersion[32];				/* �ַ��� */
	char			sysUbootVersion[32];			/* Uboot�汾 */
	char			sysSwVersion[32];				/* �ַ��� */
	char			sysDeviceName[64];				/* �ַ��� */
	unsigned long	sysCountryCode;					/* ������ */
#define COUNTRY_CODE_CHINA		156
#define COUNTRY_CODE_GERMANY	276
	unsigned long	ulRunMode;						/* ����ģʽ */
#define RUN_MODE_AUTO			0		/* ����Ӧ*/
#define RUN_MODE_FIT_AP		1		/* ����ת��*/
#define RUN_MODE_FAT_AP		2		/* ����ת��*/

	/* 2009-12-3 pyy ����*/
	unsigned long ulApTrafficVlan;	/* 0~4094 	
								0:��ʾ����VLAN 
								����:��ʾ����Ӧ��VLAN Tag */
	
/* added by laiyongxing at 2009-1-9 */
	unsigned long	ulSysNetId;                     /* NET ID used to identify rogue ap*/
	
    unsigned long ulCollectSwitch;			/* 2010-4-10 pyy ����: ʵʱ�ɼ�����*/
    /* BEGIN: Added by ������, 2013/1/8   PN:TASK #296 ���ⳡ����ɽ���ƶ�Ҫ�󱾵�ת��ʱAP����Ҳ���㲥ҵ��SSID */
    /* AP���߹ر�SSID���أ�״̬Ϊ:����/���ã�Ĭ��Ϊ����״̬ */
    unsigned long ulDisSsidSwitch;
    /* END:   Added by ������, 2013/1/8   PN:TASK #296 ���ⳡ����ɽ���ƶ�Ҫ�󱾵�ת��ʱAP����Ҳ���㲥ҵ��SSID */

    /* BEGIN: Added by lvjianlin, 2013/3/26   PN:BUG #2242  ����ulLinkOffRestartTime �ֶ� */
    /* BEGIN: Added by ������, 2012/12/5   PN:��AC��Ϣģ�Ͳ����䣬��ʱɾ���ó�Ա */
    
    unsigned long  ulLinkOffRestartTime;     /* 2012-01-5 ����:
											   bug432 ����AP��AC������ AP������ʱ������ 
											   Ĭ��ֵ3600 ��Сֵ300	��λ��s�� */
	
    /* END:   Added by ������, 2012/12/5   PN:��AC��Ϣģ�Ͳ����䣬��ʱɾ���ó�Ա */
    /* END:   Added by lvjianlin, 2013/3/26   PN:BUG #2242  ����ulLinkOffRestartTime �ֶ� */

    /* add by gwx 2012-10-31 for �û�������׼ȷ���⣬Ĭ��ֵ60����λ��*/
    unsigned long   ulRptUserTrafficTime;
    /* END:   Added by ������, 2012/11/9   PN:TSģ�飬AC�Ʒ�������׼ȷ���� */								   	

    /* BEGIN: Added by ������, 2012/12/10   PN:TASK #257 VAP���������ù��� ���ð����� */
    unsigned long   ulFlowCtrlLevel;        // ������Ƽ��� 0��STA  1��VAP 2��AP
    #define AGENT_FLOWCTRL_STA  0
    #define AGENT_FLOWCTRL_VAP  1
    #define AGENT_FLOWCTRL_AP   2
    
    unsigned long   ulTotalUpBandMax;       //�����ܴ������, ��λ��kbps
    unsigned long   ulTotalDownBandMax;     //�����ܴ������, ��λ��kbps
    /* END:   Added by ������, 2012/12/10   PN:TASK #257 VAP���������ù��� ���ð����� */
    unsigned long	ulMOC;							/* �������ʶ */
	unsigned char	aucMOI[AC_MOI_LEN];				/* ����ʵ����ʶ */
	unsigned long	ulAdminState;
	unsigned long	ulOpState;
	unsigned long	ulCurAcId;
	unsigned long ulCfgVersion;
} T_sysBasicSetup;
typedef struct sysBasicSetupAcFile_s
{
	unsigned long	ulWtpId;
	/* hc modified for cmcc test 2009-7-18 */
	char            acWtpNEId[32];                  /* AP��Ԫ���� */
	long			sysDeviceType;
#define sysDeviceType_A210i 3						/* A210i */
#define sysDeviceType_A210e 4						/* A210e */
	unsigned char	sysDeviceId[6];					/* 6��16�����ַ� */
	char			acRsv[2];
	char			acContact[64];					/* �豸ά����ϵ�� */	
	char			acLocation[64];					/* �豸����λ�� */
	char			acCountry[64];					/* �豸�������� */	
	unsigned long ulMaxStaNum;
	char			sysHwVersion[32];				/* �ַ��� */
	char			sysUbootVersion[32];			/* Uboot�汾 */
	char			sysSwVersion[32];				/* �ַ��� */
	char			sysDeviceName[64];				/* �ַ��� */
	unsigned long	sysCountryCode;					/* ������ */
#define COUNTRY_CODE_CHINA		156
#define COUNTRY_CODE_GERMANY	276
	unsigned long	ulRunMode;						/* ����ģʽ */
#define RUN_MODE_AUTO		0
#define RUN_MODE_FAT_AP		2
#define RUN_MODE_FIT_AP		1
/* added by laiyongxing at 2009-1-9 */
	unsigned long	ulSysNetId;                     /* NET ID used to identify rogue ap*/
	
    unsigned long ulCollectSwitch;			/* 2010-4-10 pyy ����: ʵʱ�ɼ�����*/
    /* BEGIN: Added by lvjianlin, 2013/3/26   PN:BUG #2242  ����ulLinkOffRestartTime �ֶ� */
    /* BEGIN: Added by ������, 2012/12/5   PN:��AC��Ϣģ�Ͳ����䣬��ʱɾ���ó�Ա */
    
    unsigned long  ulLinkOffRestartTime;     /* 2012-01-5 ����:
											   bug432 ����AP��AC������ AP������ʱ������ */
    
    /* END:   Added by ������, 2012/12/5   PN:��AC��Ϣģ�Ͳ����䣬��ʱɾ���ó�Ա */
    /* END:   Added by lvjianlin, 2013/3/26   PN:BUG #2242  ����ulLinkOffRestartTime �ֶ� */

    /* add by gwx 2012-10-31 for �û�������׼ȷ���⣬Ĭ��ֵ60����λ��*/
    unsigned long   ulRptUserTrafficTime;
    /* END:   Added by ������, 2012/11/9   PN:TSģ�飬AC�Ʒ�������׼ȷ���� */
	
	unsigned long	ulMOC;							/* �������ʶ */
	unsigned char	aucMOI[AC_MOI_LEN];				/* ����ʵ����ʶ */
	unsigned long	ulAdminState;
	unsigned long	ulOpState;
	unsigned long	ulCurAcId;
	unsigned long 	ulCfgVersion;                   /* �������ݰ汾�� */
} T_sysBasicSetupAcFile;

typedef struct sysIpSetup_s
{
	unsigned long	ulWtpId;
	long			sysIpOrigin;
#define sysIpOrigin_MANUAL					1		/* �ֹ����� */
#define sysIpOrigin_DHCP					2		/* DHCP���� */
#define sysIpOrigin_AC_DHCP					3		/* AC���� */
	unsigned char	sysIpAddr[4];
	unsigned char	sysNetMask[4];
	unsigned char	sysDefaultGw[4];
	unsigned char	aucPrimaryDns[4];				/* ��DNS������ */
	unsigned char	aucSecondaryDns[4];				/* ��DNS������ */
	unsigned long	ulMOC;							/* �������ʶ */
	unsigned char	aucMOI[AC_MOI_LEN];				/* ����ʵ����ʶ */
} T_sysIpSetup;

typedef struct sysKeeperSetup_s
{
	unsigned long	ulWtpId;
	
	long			sysKeepSwitch;					/* �Ƿ������ػ� */
#define sysKeeperSwitch_OFF					0
#define sysKeeperSwitch_ON					1
	unsigned char	sysKeeperIp0[4];				/* �ػ���������ip��ַ���豸��������ping�õ�ַ */
	unsigned char	sysKeeperIp1[4];				/* �ػ���������ip��ַ���豸��������ping�õ�ַ */
	unsigned long	sysKeepInterval;				/* �� */
	unsigned long	sysKeepCount;					/* ping��ʱ�������������ʱ����Ϊ�ػ�ʧ�� */
	unsigned long	isActionReboot;					/* �Ƿ��������� */
	unsigned long	isActionDisableAp;				/* �Ƿ�Ͽ����߽��� */
	unsigned long	isActionDisableEnet;			/* �Ƿ�Ͽ����߽��� */
#define sysKeeperActionOff					0
#define sysKeeperActionOn					1
	int				iRsv[4];						/* Ԥ��λ */
	unsigned long	ulMOC;							/* �������ʶ */
	unsigned char	aucMOI[AC_MOI_LEN];				/* ����ʵ����ʶ */
} T_sysKeeperSetup;

typedef struct sysServerSetup_s
{	
	unsigned long	ulWtpId;	
	unsigned char	sysFtpServerIpAddr[4];			/* �汾������IP��ַ */
	unsigned long	sysFtpServerPort;				/* �汾�������˿� */
	char			acFtpUserName[32];				/* FTP�û��� */
	char			acFtpPassword[32];				/* FTP���� */
	char			acSwFileName[128];				/* �ļ��� */
	char			acActivateSwVersion[32];		/* �����������汾 */
	unsigned long	ulUpgradeStrategy;				/* �����ٶ� */
#define UPGRADE_STRATEGY_HIGH_SPEED	1				/* ���� */
#define UPGRADE_STRATEGY_LOW_SPEED	2				/* ���� */
#define UPGRADE_STRATEGY_BACKGROUND 3				/* ��̨ */
	unsigned long	ulActivationTime;			    /* ��Чʱ��(����ʱ��) */
    /* hc modified for cmcc test 2009-7-18 */
    unsigned long   ulActivationPolicy;             /* ��Ч��ʽ */
	int				iRsv[4];						/* Ԥ��λ */
	unsigned long	ulMOC;							/* �������ʶ */
	unsigned char	aucMOI[AC_MOI_LEN];				/* ����ʵ����ʶ */
} T_sysSwServerSetup;

/* OMC�������� */
typedef struct sysSnmpSetup_s
{
	unsigned char	sysSnmpTrapIp[4];				/* Snmp �ϱ�IP��ַ */
	unsigned long	sysSnmpTrapPort;				/* Snmp �ϱ��˿� */
	char			sysSnmpCommunity[32];			/* Snmp ͨ�Ź�ͬ�� */
	int				iRsv[4];						/* Ԥ��λ */
} T_sysSnmpSetup;

/* SnmpV2���� */
typedef struct sysSnmpV2Setup_s
{
	char acCommunity[32];							/* ��ͬ�� */
	unsigned long ulRfc1213Flag;					/* RFC1213��׼MIB����Ȩ�� */
#define PERMISSION_FLAG_NONE		0
#define PERMISSION_FLAG_READ_ONLY	1
#define PERMISSION_FLAG_READ_WRITE	2
	unsigned long ulRfc1573Flag;					/* RFC1573��׼MIB����Ȩ�� */
	unsigned long ulRfc1643Flag;					/* RFC1643��׼MIB����Ȩ�� */
	unsigned long ulIeee8021PAEFlag;				/* Ieee8021PAE��׼MIB����Ȩ�� */
	unsigned long ulIeee8021dot11Flag;				/* Ieee8021dot��׼MIB����Ȩ�� */
	unsigned long ulPrivateFlag;					/* ˽��MIB����Ȩ�� */
	unsigned long aulResv[4];
} T_sysSnmpV2Setup;

#define snmpV2TableSize_M   4
typedef T_sysSnmpV2Setup T_sysSnmpV2Table[snmpV2TableSize_M];


/* SnmpV3���� */
typedef struct sysSnmpV3Setup_s
{
	char acUserName[32];							/* �û��� */
	unsigned long ulAuthProtocol;					/* ��ȨЭ�� */
#define SNMPV3_AUTH_PROTOCOL_NONE       0
#define SNMPV3_AUTH_PROTOCOL_HMAC_MD5   1
#define SNMPV3_AUTH_PROTOCOL_HMAC_SHA   2
	char acAuthKey[32];								/* ��Ȩ��Կ */
	unsigned long ulPrivProtocol;					/* ����Э�� */
#define PRIV_PROTOCOL_NONE		0
#define PRIV_PROTOCOL_CBC_DES	1
	char acPrivKey[32];								/* ������Կ */
	unsigned long ulRfc1213Flag;					/* RFC1213��׼MIB����Ȩ�� */
	unsigned long ulRfc1573Flag;					/* RFC1573��׼MIB����Ȩ�� */
	unsigned long ulRfc1643Flag;					/* RFC1643��׼MIB����Ȩ�� */
	unsigned long ulIeee8021PAEFlag;				/* Ieee8021PAE��׼MIB����Ȩ�� */
	unsigned long ulIeee8021dot11Flag;				/* Ieee8021dot��׼MIB����Ȩ�� */
	unsigned long ulPrivateFlag;					/* ˽��MIB����Ȩ�� */
	unsigned long aulResv[4];
} T_sysSnmpV3Setup;

#define snmpV3TableSize_M   4
typedef T_sysSnmpV3Setup T_sysSnmpV3Table[snmpV3TableSize_M];

typedef struct sysVersionSetup_s
{
    unsigned long   neType;                         /* ��Ԫ���� */
#define neType_AP           0						/* AP */
#define neType_INVALID      0xFFFFFFFF			    /* ��Ч */
    char            cfgImVersion[16];               /* ������Ϣģ�Ͱ汾��ͬ���������ļ�ͷ�е�dataVersion */
    unsigned long   configVersion;                  /* �������ݰ汾 */
    char            alarmImVersion[16];             /* �澯��Ϣģ�Ͱ汾 */
    char            perfImVersion[16];              /* ������Ϣģ�Ͱ汾 */
	unsigned long	ulWtpId;
	char			sysSwVersion[32];				/* ����汾 */
	int				iRsv[4];						/* Ԥ��λ */
} T_sysVersionSetup;

typedef struct
{
	unsigned long	ulWtpId;
	unsigned long	ulLbUserNumSwitch;
	unsigned long	ulUserNumPermitted;
	unsigned long	ulUserNumOffsetThreshold;
	unsigned long ulUserNumDynamic;
	unsigned long	ulLbUserQosSwitch;
	unsigned long	ulUserQosPermitted;
	unsigned long	ulUserQosOffsetThreshold;
	unsigned long	ulMOC;							/* �������ʶ */
	unsigned char	aucMOI[AC_MOI_LEN];				/* ����ʵ����ʶ */
} T_sysLoadBalanceSetup;

typedef struct sysLogSetup_s
{
	unsigned long ulWtpId;
	unsigned long ulLogLevel;
	unsigned long ulLogModule;
	unsigned long ulLogToStdOut;
	unsigned long ulLogToServer;
	unsigned char aucLogSvrIp[4];
	unsigned long ulLogSvrPort;
	unsigned long ulLogTransType;
	unsigned long ulMOC;							/* �������ʶ */
	unsigned char aucMOI[AC_MOI_LEN];				/* ����ʵ����ʶ */
} T_sysLogSetup;

typedef struct sysSntpSetup_s
{
	unsigned long ulWtpId;
	unsigned long ulEnableSntp;
	unsigned char aucSntpServerIp[4];
	unsigned long ulPeriod;
	unsigned long aulResv[4];
	unsigned long ulMOC;							/* �������ʶ */
	unsigned char aucMOI[AC_MOI_LEN];				/* ����ʵ����ʶ */
} T_sysSntpSetup;

typedef struct sysPPPoESetup_s
{
	unsigned long ulWtpId;
	unsigned char acUserName[32];
	unsigned char acNetworkName[32];
	unsigned char acPassword[32];
	unsigned long ulDnsMode;
	unsigned char acServiceName[32];
	unsigned char acAcName[32];
	unsigned long aulResv[4];
	unsigned long ulMOC;							/* �������ʶ */
	unsigned char aucMOI[AC_MOI_LEN];				/* ����ʵ����ʶ */
} T_sysPPPoESetup;

typedef struct
{
	unsigned long ulHeartBeatPeriod;            /* ����trap���ڣ���λ���룩*/
	int	iRsv[4];								/* Ԥ��λ */
} T_sysParaSetup;

typedef struct sysPasswordChange_s
{
	char oldPassword[32];
	char newPassword[32];
	int	iRsv[4];								/* Ԥ��λ */
} T_sysPasswordChange;

typedef char T_sysPingSummary[1024];

typedef unsigned char T_sysPingTest[4];

typedef unsigned long T_sysReboot;
#define sysReboot_CHECK			0x44444444

typedef unsigned long T_sysResetConfig;
#define sysResetConfig_CHECK	0x44444444
#define sysResetRsvCfg_CHECK	0x55555555

typedef struct tm T_sysSetRtc;

typedef char T_sysUpdateImage[128];

typedef char T_sysUpdateRamdisk[128];

typedef char T_sysDownloadConfig[128];

typedef char T_sysDownloadLog[128];

typedef char T_sysUploadConfig[128];

/* ͨ��FTP���ظ��������ļ� */
typedef struct
{
    char            acFile[128];
    unsigned char   aucServerIP[4];
    unsigned long   ulServerPort;
    char            acUser[32];
    char            acPwd[32];
}T_sysUploadConfigFile;

/* added by laiyongxing used to hold the logo_image_path */
typedef struct UpdateLogo_s
{
	unsigned long ulSwitch;
#define SWITCH_ENABLE	0x77777711
#define SWITCH_DISABLE	0x77777700
	char imgPath[128];
	char copName[128];
	char title[128];
	long isInfoValid;
#define SYS_LOGO_INFO_INVALID   0
#define SYS_LOGO_INFO_VALID       1
	int imgLen;
	int headLen;
	char acDeviceType[32];
	char acSoftVer[32];
	char acHardVer[32];
} T_SYSLOGO;

typedef struct SnapShot_s
{
	unsigned long ulWsModule;
	unsigned long ulMsModule;
	unsigned long ulBbModule;
	unsigned long ulUModule;
	unsigned long ulCfModule;
	unsigned long ulMsgModule;
	int                 iRsv[4];
} T_SNAPSHOT;

typedef struct AppendCode_s
{
	unsigned long	ulAppendFlag;
#define AppendFlag_DISABLE		0
#define AppendFlag_ENABLE		1
	char			acAppendPath[128];
	unsigned long	ulAppendCodeLen;
	int				iRsv[4];
}T_sysAppendCode;

typedef struct sysUpdateVersionProgress_s
{
	unsigned long	sysUpdateVersionProgressState;
#define sysUpdateVersionProgress_DOING		1
#define sysUpdateVersionProgress_FAIL		2
#define sysUpdateVersionProgress_SUCCESS	3
	unsigned char	sysUpdateVersionProgressDescr[64];
	int				iRsv[4];
} T_sysUpdateVersionProgress;

/* ά����¼ */
typedef struct enregisterFileHeader_s
{
	unsigned short	headLen;					/* bytes of head data */
	unsigned short	dataLen;					/* bytes of file data */
	long			dataState;
	//#define dataState_NONE 	0X11111111
	//#define dataState_VALID 0X22222222
	int				iRsv[4];					/* Ԥ��λ */
} T_enregisterFileHeader;

typedef struct sysPeriodOfValidity_s
{
	unsigned long	sysPeriodSwitch;
#define sysPeriodSwitch_ON		1
#define sysPeriodSwitch_OFF		0
	unsigned long	sysUseTime;					/* ϵͳ�ϵ�ʹ�õ������� */
	unsigned long	sysOverdueTime;				/* ϵͳ�������� */
	int				iRsv[4];					/* Ԥ��λ */
} T_sysPeriodOfValidity;

typedef struct
{
	unsigned long sysPeriodSwitch;
	unsigned long sysUseTime; /* ϵͳ�ϵ�ʹ�õ������� */
	unsigned long sysOverdueTime; /* ϵͳ�������� */
	char acKey[64 + 1];
} T_registerSetup;

typedef struct sysTimer_s
{
	unsigned short	timerHandStatus;
#define  timerHandStatus_SYSPERIOD			0
#define  timerHandStatus_GATEWAYMONITOR		1
#define  timerHandStatus_OMA_TEST			2
#define  sysTimerStatus_RESETCONFIG			3	/* ������λ������ */
	int				iRsv[4];					/* Ԥ��λ */
}T_TIMER;

typedef struct wifiDevice_s
{
	unsigned long	wifiDeviceId;				/* �����豸��� */
	char			wifiDeviceName[32];			/* �����豸���� */
	int				iRsv[4];					/* Ԥ��λ */
} T_wifiDevice;
typedef T_wifiDevice T_wifiDeviceAdd;
typedef T_wifiDevice T_wifiDeviceDelete;

typedef struct wifiDeviceBasicSetup_s
{	
	unsigned long ulWtpId;
	unsigned long	wifiDeviceIndex;						/* �豸��� */
	char			wifiDeviceInfo[32];						/* �豸���� */
	unsigned long	wifiDeviceType;							/* �豸���� */
	unsigned long ulMaxStaNum;                              /* �����������û��� */
#define wifiDeviceType_NONE					0XFFFFFFFF
#define wifiDeviceType_ALONEAP				0
#define wifiDeviceType_BINDAP				1
#define wifiDeviceType_ALONEADHOC			2
#define wifiDeviceType_BINDADHOC			3
#define wifiDeviceType_AP_ADHOC				4
#define wifiDeviceType_STA_ADHOC			5
#define wifiDeviceType_ALONESTA				6
#define wifiDeviceType_BINDSTA				7
#define wifiDeviceType_STA_AP				8
	unsigned long	ulRunMode;								/* ����ģʽ add by yzl 2008-12-05 */
#define RUN_MODE_STA	0
#define RUN_MODE_ADHOC	1
	unsigned long ulWifiModuleNum;					        /* ����ģ����*/
	unsigned long	wifiModuleId[wifiModule_SIZE];			/* ģ��ID�б� */
	unsigned long	wifiModuleType[wifiModule_SIZE];		/* ģ�������б� */
#define wifiModuleType_ALONE				0
#define wifiModuleType_MAIN					1
#define wifiModuleType_SUB					2
	int				iRsv[4];							    /* Ԥ��λ */
	unsigned long ulMOC;							        /* �������ʶ */
	unsigned char aucMOI[AC_MOI_LEN];				        /* ����ʵ����ʶ */
}T_wifiDeviceBasicSetup;

typedef struct wifiDeviceChanSetup_s
{
	unsigned long ulWtpId;
	unsigned long	wifiDeviceIndex;						/* �豸��� */
	unsigned long	ulAbgMode;							    /* A/B/Gģʽ */
#define wifiAbgMode_A						1
#define wifiAbgMode_B 						2
#define wifiAbgMode_G 						4
#define wifiAbgMode_MIXED					6
#define wifiAbgMode_ABGMIXED				7
#define wifiAbgMode_AN						8
#define wifiAbgMode_GN						9
#define wifiAbgMode_BGN						10
#define wifiAbgMode_AN40                    		11
#define wifiAbgMode_GN40                    		12


	unsigned long	ulChannelMode;						    /* �ŵ�ģʽ */
#define CHANNEL_MODE_FULL					0
#define CHANNEL_MODE_HALF 					1
#define CHANNEL_MODE_QUARTER				2
	unsigned long	ulTurboMode;						    /* Turboģʽ */
#define TURBO_MODE_DISABLE	0
#define TURBO_MODE_ENABLE	1
	unsigned long	ulChannelState;						    /* �����ŵ�ģʽ */
#define CHANNELMODE_STATIC					0
#define CHANNELMODE_AUTO					1
	unsigned long	wifiModuleChannelList[channel_SIZE];    /* �ŵ��б� */
	//����b/gģʽ����ȡ�����ŵ�
#define wifiChannel_AUTO					0
#define wifiChannel_CH1 					1
#define wifiChannel_CH2 					2
#define wifiChannel_CH3 					3
#define wifiChannel_CH4 					4
#define wifiChannel_CH5 					5
#define wifiChannel_CH6 					6
#define wifiChannel_CH7 					7
#define wifiChannel_CH8 					8
#define wifiChannel_CH9 					9
#define wifiChannel_CH10 					10
#define wifiChannel_CH11 					11
#define wifiChannel_CH12 					12
#define wifiChannel_CH13 					13
	//����aģʽ����ȡ�����ŵ�,�й�
#define wifiChannel_CH149 					149
#define wifiChannel_CH153 					153
#define wifiChannel_CH157 					157
#define wifiChannel_CH161 					161
#define wifiChannel_CH165 					165
	//����aģʽ����ȡ�����ŵ�,�¹�
#define wifiChannel_CH36 36
#define wifiChannel_CH40 40
#define wifiChannel_CH44 44
#define wifiChannel_CH48 48
#define wifiChannel_CH52 52
#define wifiChannel_CH56 56
#define wifiChannel_CH60 60
#define wifiChannel_CH64 64
#define wifiChannel_CH100 100
#define wifiChannel_CH104 104
#define wifiChannel_CH108 108
#define wifiChannel_CH112 112
#define wifiChannel_CH116 116
#define wifiChannel_CH120 120
#define wifiChannel_CH124 124
#define wifiChannel_CH128 128
#define wifiChannel_CH132 132
#define wifiChannel_CH136 136
#define wifiChannel_CH140 140

#define wifiChannel_INVALID     0
	unsigned long	ulFrequPointQualitySwitch;		/* �ŵ���ѡ���� */
	unsigned long	ulRssiThreshold;				/* �ŵ���ѡRSSI���� */
	long			lNoiseThreshold;				/* �ŵ���ѡ�������� */

	unsigned long	wifiModuleABGModeType;			/* A/B/G�б����ͣ���̬�ֶ� */
#define wifiAbgType_ABGM 0
#define wifiAbgType_BGM 1
#define wifiAbgType_ABG 2
#define wifiAbgType_A 3

	/* 2010-7-27 11n����*/
#define BAND_20M			1
#define BAND_40M			2	
	unsigned long	ulBand;	/* Ƶ�� 	1��20MHz
										2��20/40MHz���Զ���
 										1��20MHz
						*/
	unsigned long	ulRFNum;	/*  �ռ������� 1~4 */
	unsigned long	ulShortGI;	/* SGI���ܿ���
 							0������
							1������*/
	unsigned long	ulAMPDU;	/* A-MPDU���ܿ���
 							0������
							1������
							*/
	unsigned long	ulAMSDU;	/* A-MSDU���ܿ���
	 						 0������
							1������
							*/
 	unsigned long	ulOnly11n;	/* ��11n�û����빦��
 							 0������
							1������
							 ����ʱ��ֻ��11n�ն˿��Թ�������11n�ն��޷�����
							*/
						

	int				iRsv[4];						/* Ԥ��λ */
	unsigned long ulMOC;							/* �������ʶ */
	unsigned char aucMOI[AC_MOI_LEN];				/* ����ʵ����ʶ */
}T_wifiDeviceChanSetup;

typedef struct wifiDeviceRateSetup_s
{
	unsigned long   ulWtpId;
	unsigned long	wifiDeviceIndex;    /* �豸��� */
    /* BEGIN: Added by ������, 2012/12/27   PN:TASK #336 �����������õ�AC�����ʵ�� */
    /* ����ȥ���˶���������ʵ����ã���Ϊ�������Ŀǰ����ixpƽ̨����������
       �ڣ�����ûʲô�ر����� */
    //unsigned long	ulMaxTxRate;	    /* ��������� */
    unsigned long   ulStaUpRateMin;     /* �ն����з�������������� */
    unsigned long   ulStaDownRateMin;   /* �ն������հ������������ */
    /* END:   Added by ������, 2012/12/27   PN:TASK #336 �����������õ�AC�����ʵ�� */
	
	unsigned long	ulMgmTrate;		    /* ��ѡ����֡��������  */
	unsigned long	ulMcasTrate;	    /* ��ѡ�㲥��������  */
	unsigned long	ulMgmtRetrys;	    /* ����֡�ش�����  */
	unsigned long	ulRsv[4];		    /* Ԥ�� */
	unsigned long   ulMOC;				/* �������ʶ */
	unsigned char   aucMOI[AC_MOI_LEN];	/* ����ʵ����ʶ */
}T_wifiDeviceRateSetup;

typedef struct wifiDevicePowerSetup_s
{
	unsigned long ulWtpId;
	unsigned long	wifiDeviceIndex;			 /* �豸��� */
	unsigned long	ulLimitTxPower;	             /* ����书�� */
	unsigned long	ulMaxTxPower;	             /* ����˥������ */
#define MAX_TX_POWER_PER_5			0
#define MAX_TX_POWER_PER_12			1
#define MAX_TX_POWER_PER_20			2
#define MAX_TX_POWER_PER_25			3
#define MAX_TX_POWER_PER_30			4
#define MAX_TX_POWER_PER_50			5
#define MAX_TX_POWER_PER_80			6
#define MAX_TX_POWER_PER_100		7
	unsigned long	ulInterfereDetectSwitch;	/* ���书���Զ�����  */
	unsigned long	ulInterfereDetectPeriod;
	unsigned long	ulMaxInterferePermitted;
	unsigned long	ulRssiWaveRange;
	unsigned long	ulPowerCompensationSwitch;
	unsigned long	ulRsv[4];		            /* Ԥ�� */
	unsigned long ulMOC;						/* �������ʶ */
	unsigned char aucMOI[AC_MOI_LEN];			/* ����ʵ����ʶ */
}T_wifiDevicePowerSetup;

typedef struct wifiDeviceAdvancedSetup_s
{
	unsigned long   ulWtpId;
	unsigned long	wifiDeviceIndex;			/* �豸��� */
	unsigned long	wifiBeaconInterval;			/* BEACON��� */
	unsigned long	wifiDtimInterval;			/* DTIM��� */
	unsigned long	wifiPreambleType;			/* ǰ������ */
#define wifiPreambleType_AUTO				1
#define wifiPreambleType_SHORT				2
#define wifiPreambleType_LONG				3
	unsigned long	wifFragmentationThreshold;	/* ��Ƭ���� */
	unsigned long	wifiRtsThreshold;			/* RTS���� */
	unsigned long	wifiCtsProtectionMode;		/* CTS����ģʽ */
#define wifiCtsProtectionMode_DISABLE		0
#define wifiCtsProtectionMode_ENABLE		1
	unsigned long	ulTxDistOptSwitch;			/* ������뿪�� */
#define txDistOptSwitch_DISABLE	0
#define txDistOptSwitch_ENABLE	1
	unsigned long	ulMaxTxDistance;			/* �������� */
       unsigned long ulLongRtsThreshold;        /* ��֡�ش����� */
	unsigned long ulShortRtsThreshold;			/* ��֡�ش����� */
	unsigned long ulCacheTime;                  /* �������ݰ������� */
	int				iRsv[4];					/* Ԥ��λ */
	unsigned long ulMOC;						/* �������ʶ */
	unsigned char aucMOI[AC_MOI_LEN];			/* ����ʵ����ʶ */
}T_wifiDeviceAdvancedSetup;

typedef struct wifiDeviceResourceSetup_s
{
	unsigned long   ulWtpId;
	unsigned long	wifiDeviceIndex;				  /* �豸��� */
	unsigned long   ulMoniMode;                       /* ���߻������ģʽ */                             
	unsigned long	aulDiffScanChannelList[16];
	unsigned long	ulDiffScanInterval;
	unsigned long	ulDiffScanResidenceTime;
	unsigned long	ulSameScanInterval;
	/* hc modified for cmcc test 2009-7-18 */
	unsigned long   ulNeighAutoScanSwitch;            /* �����ھ��Զ�ɨ�� */
	unsigned long   ulNeighRssiMin;			          /* �ھ��ź�ǿ����С���� */
	unsigned long   ulNeighRssiWave;                  /* �ھ��ź�ǿ�Ȳ�����Χ */
	unsigned long   aulResv[4];
	unsigned long   ulMOC;							  /* �������ʶ */
	unsigned char   aucMOI[AC_MOI_LEN];				  /* ����ʵ����ʶ */
}T_wifiDeviceResourceSetup;

typedef struct wifiDeviceEntry_s
{
	unsigned long				wifiDeviceId;				/* �豸ID */
	T_wifiDeviceBasicSetup		wifiDeviceBasicSetup;		/* ���߻������� */
	T_wifiDeviceChanSetup		stChanSetup;				/* �����ŵ����� */
	T_wifiDeviceRateSetup		stRateSetup;				/* ������������ */
	T_wifiDevicePowerSetup		stPowerSetup;				/* ���߹������� */
	T_wifiDeviceAdvancedSetup	wifiDeviceAdvancedSetup;	/* ���߸߼����� */
	/* hc modified for telecom test 2009-7-6 */
	WIDS_CONFIG_T               stWifiDeviceWidsSetup;      /* WIDS�������� */
	T_wifiDeviceResourceSetup	stWifiDeviceResSetup;		/* ���߻������ */
	int							iRsv[4];					/* Ԥ��λ */
} T_wifiDeviceEntry;

#define wifiDeviceTable_SIZE wifiModule_SIZE
typedef T_wifiDeviceEntry T_wifiDeviceTable[wifiDeviceTable_SIZE];
#define wifiDeviceList_SIZE wifiDeviceTable_SIZE
typedef T_wifiDevice T_wifiDeviceList[wifiDeviceList_SIZE];

/* BEGIN: Added by ������, 2012/12/27   PN:TASK #336 �����������õ�AC�����ʵ�� */
typedef T_wifiDeviceRateSetup T_wifiDeviceRateSetupList[wifiModule_SIZE];
/* END:   Added by ������, 2012/12/27   PN:TASK #336 �����������õ�AC�����ʵ�� */

typedef unsigned long T_wifiDeviceValidWifiList[wifiModule_SIZE];
typedef unsigned long T_apValidWifiDeviceList[wifiDeviceList_SIZE];
typedef unsigned long T_adhocValidWifiDeviceList[wifiDeviceList_SIZE];

typedef struct routeBasicSetup_s
{
	unsigned long ulWtpId;
	
	unsigned long ulNodeGwRunMode;				/* ���ع��� */
#define NODE_GW_RUN_MODE_DISABLE	0
#define NODE_GW_RUN_MODE_ENABLE		1
	unsigned long ulNetDiameter;				/* ������� */
	unsigned char aucRouteFavorFatherMac[6];	/* ��ѡ���ڵ� */
	char acRsv1[2];
	unsigned long ulRouteFavorMinCost;			/* ��ѡ���ڵ������ź�ǿ�� */
	unsigned char aucSysDeviceId[6];			/* 6��16�����ַ� */
	char acRsv[2];
	unsigned long aulResv[4];
	unsigned long ulMOC;						/* �������ʶ */
	unsigned char aucMOI[AC_MOI_LEN];			/* ����ʵ����ʶ */
} T_routeBasicSetup;

typedef struct routerNeighSetup_s
{
	unsigned long ulWtpId;
	
	unsigned long	routeMinNeighborPathcost;	/* �ھ������������� */
	unsigned long	routeHelloInterval;			/* HELLO��� */
	unsigned long	routeHelloCombine;			/* HELLO�ϲ� */
#define HELLO_COMBINE_DISABLE				0
#define HELLO_COMBINE_ENABLE				1
	unsigned long	routeDiffNeighScanThreshold;/* ��Ƶ�ھ�ɨ�����ޣ�·�������� */
	unsigned long	routeDiffNeighScanInterval;	/* ��Ƶ�ھ�ɨ���� */
	int				iRsv[4];					/* Ԥ��λ */
	unsigned long ulMOC;						/* �������ʶ */
	unsigned char aucMOI[AC_MOI_LEN];			/* ����ʵ����ʶ */
} T_routerNeighSetup;

typedef struct routerHandoverSetup_s
{
	unsigned long ulWtpId;
	
	unsigned long	routeHigh4father;			/*  �Ϸ����ڵ��������� */
	unsigned long	routeLow4father;			/*  �Ϸ����ڵ��������� */
	unsigned long	routeHoldQuality;			/* ���ڵ��л��������� */
	unsigned long	routeHoldTime;				/* ���ڵ��л�����ʱ�� */
	unsigned long	routeDuxPathCost;			/* ��·�������� */
#define PATH_COST_FRONT						0
#define PATH_COST_DOUBLE					1
	int				iRsv[4];					/* Ԥ��λ */
	unsigned long ulMOC;						/* �������ʶ */
	unsigned char aucMOI[AC_MOI_LEN];			/* ����ʵ����ʶ */
} T_routerHandoverSetup;

typedef struct routerMscSetup_s
{
	unsigned long ulWtpId;
	
	unsigned long	routeMsRunMode;				/*	�Ƿ������豸���� */
#define routeMsRunMode_DISABLE				0
#define routeMsRunMode_ENABLE				1
	unsigned long	routeMsDeviceName;			/*  ���ڷ�����������Ʊ��ĵ������豸�� */
#define routeMsName_IXP0					0	/*  0=����������ת�� */
#define routeMsName_IXP1					1	/*  1=��ֹ������ת�� */
	unsigned long	routeDataDeviceName;		/*  ���ڷ������������ݱ��ĵ������豸�� */
#define routeDataName_IXP0					0	/*  0=����������ת�� */
#define routeDataName_IXP1					1	/*  1=��ֹ������ת�� */
	unsigned long	routeMsRole;				/*  ���ӽ�ɫ */
#define routeMsRole_AUTO                    0
#define routeMsRole_MASTER                  1
#define routeMsRole_SLAVE                   2
	unsigned long	routeForwardForbidden;		/* ���ӷ�ʽ */
#define routeMsType_LOAD_BALANCE            0   /* ���ɷֵ� */
#define routeMsType_MASTER_SLAVE            1   /* ������ʽ */

	unsigned long	routeMaxMsQueryLoss;		/* ����ʱ�� */
	unsigned long	routeMinimalAirbandQual;	/* ������������ */
	unsigned long	routeWifiAdhocBandWidth;	/* ���ߴ������ */
	int				iRsv[4];					/* Ԥ��λ */
	unsigned long ulMOC;						/* �������ʶ */
	unsigned char aucMOI[AC_MOI_LEN];			/* ����ʵ����ʶ */
} T_routerMscSetup;

typedef struct aclPolicySwitch_s
{
	unsigned long ulPolicySwitch;				/* ACL���� */
#define aclPolicySwitch_OFF					0	/* �رգ��ر�����£�ȱʡ���Բ����� */
#define aclPolicySwitch_ON					1	/* ������ȱʡֵ */
	unsigned long ulDefaultPolicy;				/* ȱʡ���� */
#define ACL_DEFUALT_POLICY_DENY				0	/* ��ֹͨ�� */
#define ACL_DEFUALT_POLICY_PASS				1	/* ����ͨ����ȱʡֵ */
	unsigned long aulRsv[4];
} T_aclPolicySwitch;

#define ACL_SETUP_ENTRY_MAX_SIZE	32
typedef struct
{
	unsigned long ulFilterIndex;				/* ����:Ψһ��־����0��ʼ���� */
	unsigned long ulFilterType;					/* Э������ */
#define FILTER_TYPE_IP				0			/* IP��ַ */
#define FILTER_TYPE_ENET_PRO		1			/* ��̫��Э�� */
#define FILTER_TYPE_IP_PRO			2			/* IPЭ�� */
#define FILTER_TYPE_IP_PRO_PORT		3			/* IPЭ��˿� */
	unsigned long ulDefaultPolicy;				/* ͨ�� */
#define DEFUALT_POLICY_DENY			0			/* ��ֹͨ�� */
#define DEFUALT_POLICY_PASS			1			/* ����ͨ�� */
	unsigned long ulProtocol;					/* Э�� */
	unsigned long ulPort;						/* �˿� */
	unsigned char aucIpAddr[4];					/* IP��ַ */
	unsigned char aucNetMask[4];				/* �������� */
	char acFilterName[32];						/* �������� */
	unsigned long ulValid;                      /*�Ƿ���Ч*/
#define FILTER_POLICY_INVALID                   0
#define FILTER_POLICY_VALID                     1
	unsigned long aulRsv[4];
} T_aclSetup;

typedef struct
{
	unsigned long ulValidNum;
	T_aclSetup stAclSetup[ACL_SETUP_ENTRY_MAX_SIZE];
} T_aclSetupEntry;

typedef struct
{
	unsigned long ulOldIndex;
	unsigned long ulNewIndex;
} T_PROTOCOL_INDEX;

/* �������� */
typedef struct fclBasicSetup_s
{
	unsigned long ulMulticastSwitch;     	/* ����ಥ��*/
#define MULTICAST_SWITCH_DISABLE		0   /* ��*/
#define MULTICAST_SWITCH_ENABLE			1   /* �ǣ�ȱʡΪ��*/
	unsigned long ulBroadcastSwitch;     	/* ����㲥����ARP��DHCP��*/
#define BROADCAST_SWITCH_DISABLE		0   /* ��*/
#define BROADCAST_SWITCH_ENABLE			1   /* �ǣ�ȱʡΪ��*/
	unsigned long ulBMRateLimitSwitch;      /* �㲥/�ಥ �Ƿ�����*/
#define BM_RATE_LIMIT_SWITCH_DISABLE    0   /* ��*/
#define BM_RATE_LIMIT_SWITCH_ENABLE     1   /* �ǣ�ȱʡΪ��*/
	unsigned long ulMaxRate;                /* ������ʣ���/�룻ȱʡΪ*/
	unsigned long	ulFclSwitch;	        /* ���ؿ��� */
#define FCL_SWITCH_DISABLE	0		        /* ���� */
#define FCL_SWITCH_ENABLE	1		        /* ���� */
	unsigned long	ulUserMaxFlow;	        /* ���û������������λΪkbit/s */
	int				iRsv[4];		        /* Ԥ��λ */
} T_fclBasicSetup;

/* IPת�� */
typedef struct ipForwardSetup_s
{
	unsigned long ulIpForwardSwitch;		/* �Ƿ�����IPת�� */
#define FORWARD_SWITCH_DISABLE			0
#define FORWARD_SWITCH_ENABLE			1
	unsigned long aulResv[4];
} T_ipForwardSetup;

/* TCP�Ż� */
#define SERVER_PORT_MAX_NUM				16
#define INVALID_SERVER_PORT				0
typedef struct tcpOptimizationSetup_s
{
	unsigned long ulOptEnable;				/* �Ƿ�����TCP�Ż����� */
	unsigned long ulL2OptEnable;			/* �Ƿ�֧�ֲ�2 */
#define OPT_ENABLE_NO					0
#define OPT_ENABLE_YES					1
	unsigned long ulL3OptEnable;			/* �Ƿ�֧�ֲ�3 */
	unsigned long ulOptiMode;				/* �Ż���ʽ */
#define optiMode_SERVICE                0   /* ҵ��ƽ������ */
#define optiMode_BAND                   1   /* �������� */
#define optiMode_BAND_CSV               2   /* �������ȣ����أ� */
	unsigned long ulConnTimeout;			/* TCP���ӳ�ʱʱ�� */
	unsigned short ausServerPort[SERVER_PORT_MAX_NUM];	/* �˿ں� */
	unsigned long aulResv[4];
} T_tcpOptimizationSetup;

#define aodvNeighborList_SIZE 32
typedef struct aodvNeighbor_s
{
	unsigned char	aodvNeighborMac[6];		/* ȫ���ʾ��Ч */
	unsigned char	aodvNeighborIp[4];
	unsigned long	aodvConnectQuality;		/* ����Ʒ��(�����ֶ�) */
	unsigned long	aodvNeighborLiveTime;
	unsigned long	aodvNeighborSendPkt;
	unsigned long	aodvNeighborRecvPkt;
	char			aodvNeighborPort[16];
	long			aodvNeighborStatus;
#define aodvNeighborStatus_NEIGH 0b0000
#define aodvNeighborStatus_FATHER 0b1000
#define aodvNeighborStatus_GATEWAY 0b0001
#define aodvNeighborStatus_ROOT 0b0011
#define aodvNeighborStatus_FATHERROOT 0b1011
#define aodvNeighborStatus_SON 0b0100
	int				iRsv[4];				/* Ԥ��λ */
} T_aodvNeighbor;
typedef T_aodvNeighbor T_aodvNeighborList[aodvNeighborList_SIZE];

#define aodvRouteList_SIZE 128
typedef struct aodvRoute_s
{
	unsigned char	aodvRouteDstMac[6];		/* ȫ���ʾ��Ч */
	unsigned char	aodvRouteNextHopMac[6];
	unsigned char	aodvRouteNextHopIp[4];
	unsigned long	aodvRouteLivTime;
	unsigned long	aodvRouteForwardPkt;
	char			aodvRoutePort[16];
	long			aodvRouteStatus;
#define aodvRouteStatus_INIT_INIT 0
#define aodvRouteStatus_INIT_RELAY 1
#define aodvRouteStatus_NORMAL_INIT 2
#define aodvRouteStatus_NORMAL_RELAY 3
#define aodvRouteStatus_NORMAL_LOCAL 4
#define aodvRouteStatus_UPDATE_INIT 5
#define aodvRouteStatus_UPDATE_RELAY 6
#define aodvRouteStatus_UPDATE_LOCAL 7
	int				iRsv[4];				/* Ԥ��λ */
} T_aodvRoute;
typedef T_aodvRoute T_aodvRouteList[aodvRouteList_SIZE];

typedef struct apVap_s
{
	char			apSsid[32];
	unsigned long	apId;
	unsigned long	wifiDeviceId;			/* ���������豸ID */
	unsigned long	ulPrivCap;				/* �Ƿ���������Ϣ */
	int				iRsv[4];				/* Ԥ��λ */
} T_apVap;
typedef T_apVap T_apVapAdd;
typedef T_apVap T_apVapDelete;

#define WME_NUM_AC 4
#define wifiDeviceId_NONE	INVALID_WIFIDEVICE_ID
#define apRunMode_DISABLE			0
#define apRunMode_ENABLE			1

#define wifiBroadcastSsid_DISABLE	0
#define wifiBroadcastSsid_ENABLE	1
#define apBroadcastSsid_DISABLE wifiBroadcastSsid_DISABLE
#define apBroadcastSsid_ENABLE wifiBroadcastSsid_ENABLE

#define VAP_TRAFFICMODE_CENTFORWARD		1		/* ����ת��*/
#define VAP_TRAFFICMODE_LOCALFORWARD	2		/* ����ת��*/

/* BEGIN: Added by lvjianlin, 2013/6/20   PN:lte-fi  GPS ALARM  LOCAL PORTAL */
#define VAP_TRAFFICMODE_LOCALTOCENT     3
#define VAP_TRAFFICMODE_ACAUTHAPFORWARD 4
/* END:   Added by lvjianlin, 2013/6/20   PN:lte-fi  GPS ALARM  LOCAL PORTAL */
/* BEGIN: Added by liyoukun, 2013/12/26   PN:record vap mode */
#define LTEFI_FILE_CENTVAP  "/tmp/centvap"
#define LTEFI_FILE_MIXVAP   "/tmp/mixvap"
/* END:   Added by liyoukun, 2013/12/26   PN:record vap mode */
typedef struct apBasicSetup_s
{
	unsigned long 	ulWtpId;
	unsigned long	wifiDeviceId;		
	unsigned long	apVapId;

	long			apRunMode;
	unsigned char	apMac[6];
	char			acRsv[2];
	char			apSsid[32];
	long			apBroadcastSsid;
    /* hc modified for cmcc test 2009-7-18 */
	unsigned long	ulEndUserMaxNum;	      /* ����û������� */
    //unsigned long	ulMaxSimultUsers;        /* ��󲢷��û��� */
    unsigned long	ulMaxSimultTraffic;         	/* ������� */

    /* BEGIN: Added by ������, 2012/12/10   PN:TASK #257 VAP���������ù��� ���ð����� */
    unsigned long   ulTotalUpBandMax;   /*�����ܴ������,��λ��kbps*/
    unsigned long   ulTotalDownBandMax; /*�����ܴ������,��λ��kbps*/
    /* END:   Added by ������, 2012/12/10   PN:TASK #257 VAP���������ù��� ���ð����� */

	/* 2010-4-19 pyy ����*/
    unsigned long ulTrafficMode;			/* ҵ��ת��ģʽ1: ����ת�� 2:����ת�� */
    unsigned long ulApTrafficVlan;			/* ҵ��VLAN 0��ʾ����vlan tag */

    /* 2010-6-24 pyy ����*/
    unsigned long ulPrdOff;     /* �Ƿ��ڹر� */
	unsigned long ulStartTime;  /* ��ʼʱ�� */
	unsigned long ulEndTime;    /* ����ʱ�� */
    /* BEGIN: Added by ������, 2013/1/8   PN:tast#360  beacon�㲥���� */
    /* �������ÿ��أ�Ĭ��Ϊ���� AC_WEB Ĭ�Ͽ���Ϊ 1  ��������Ĭ�Ͽ���Ϊ 0 ��Ҫ��ת*/
    unsigned long ulWifiEnable;
    /* END:   Added by ������, 2013/1/8   PN:tast#360  beacon�㲥���� */
	int				iRsv[4];			        /* Ԥ��λ */
	unsigned long ulMOC;							/* �������ʶ */
	unsigned char aucMOI[AC_MOI_LEN];				/* ����ʵ����ʶ */
} T_apBasicSetup;

/* BEGIN: Added by ������, 2012/12/6   PN:TASK #257 VAP���������ù��� ���ð����� */
typedef 	struct	apVapMacFlowCtrlSetup
{
	unsigned long	ulWtpId;				    /*AP�豸ID */
	unsigned long   ulWifiDeviceId;			    /*�����豸���*/
	unsigned long	ulVapId;				    /* VAP ID */
	unsigned char	aucMac[6];				    /*�û�MAC */
	unsigned char	aucResv[2];				    /*�����ֶΣ�Ϊ�����ֽڶ���*/
	unsigned long	ulUserUpBandMax;		    /*�����û�������, kbps ,ȱʡֵ1024*/
	unsigned long	ulUserDownBandMax;		    /*�����û�������kbps ,ȱʡֵ1024*/
	unsigned long	ulMOC;					    /*�������ʶ�������Զ�����,�·�����Ԫ*/
	unsigned char	aucMOI[AC_MOI_LEN];	        /*����ʵ����ʶ�������Զ�����,�·�����Ԫ*/
}T_apVapMacFlowCtrlSetup;

/* END:   Added by ������, 2012/12/6   PN:TASK #257 VAP���������ù��� ���ð����� */

/* BEGIN: Added by ������, 2013/1/10   PN:TASK #257  -2 VAP-MAC�û����ر���� */
typedef struct  
{
	unsigned long   ulWifiDeviceId;			    /*�����豸���*/
	unsigned long	ulVapId;				    /* VAP ID */
	unsigned char	aucMac[6];				    /*�û�MAC */
	unsigned char	aucResv[2];				    /*�����ֶΣ�Ϊ�����ֽڶ���*/
	unsigned long	ulUserUpBandMax;		    /*�����û�������, kbps ,ȱʡֵ1024*/
	unsigned long	ulUserDownBandMax;		    /*�����û�������kbps ,ȱʡֵ1024*/
    unsigned long   ulBeUsed;                   /* �ñ���Ŀ�Ƿ�ռ�� */
}T_apVapMacFlowCtrlCount;
/* END:   Added by ������, 2013/1/10   PN:TASK #257  -2 VAP-MAC�û����ر���� */

/* BEGIN: Added by lvjianlin, 2013/6/20   PN:lte-fi  GPS ALARM  LOCAL PORTAL */
#define     LTEFI_ACCESSMODE_TD_LTE_FIRST       0   /* TD_LTE���� */
#define     LTEFI_ACCESSMODE_TD_SCDMA_FIRST     1   /* TD_SCDMA���� */
#define     LTEFI_ACCESSMODE_TD_LTE_ONLAY       2   /* ��TD_LTE */
#define     LTEFI_ACCESSMODE_TD_SCDMA_ONLAY     3   /* ��TD_SCDMA */
#define     LTEFI_ACCESSMODE_GSM_FIRST          4   /* GSM���� */
#define     LTEFI_ACCESSMODE_GSM_ONLAY          5   /* ��GSM */

typedef struct  ltefiBasicSetup
{
    unsigned long   ulWtpId;                /* WTP  id */
    unsigned char   acLtefiApn[32];         /* ��������� */
    unsigned long   ulLtefiMtu;             /* MTUֵ */
    unsigned long   ulAccessMode;           /* LTE-FI����ģʽ */
    unsigned long   ulBatterySptTime;       /* ��ع���ʱ�� */
    unsigned char   acBusNo[32];            /* ������� */
    unsigned char   aucApForwardBrIp[4];    /* �ȱ��غ��е�����µ����ص�ַ */
    unsigned char   aucApForwardBrMask[4];  /* �ȱ��غ��е�����µ����ص�ַ���� */
    unsigned char   acGpsServer1[64];       /* GPS��Ϣ�ϱ�������1 */
    unsigned char   acGpsServer2[64];       /* GPS��Ϣ�ϱ�������2 */
    unsigned long   ulGpsReportPeriod;      /* GPS��Ϣ�ϱ����� add by liyoukun*/
    unsigned char   acContextServer1[128];  /* LTE-Fi���������ط�����1 */
    unsigned char   acContextServer2[128];  /* LTE-Fi���������ط�����2 */
    unsigned char   acLtefiPortalIp[64];    /* ltefi�豸����portalIP��ַ add by liyoukun*/
    /* BEGIN: Added by lvjianlin, 2013/7/11   PN:ulDataUpdateTime */
    unsigned long   ulDataUpdateTime;       /* ���ڸ��±��ش洢�ռ����� */
    /* END:   Added by lvjianlin, 2013/7/11   PN:ulDataUpdateTime */
    /* BEGIN: Added by lvjianlin, 2013/7/25   PN:lte-fi DT */
    unsigned char   acDTUserName[64];       /* ·���������½�û��� */
    unsigned char   acDTPass[64];           /* ·���������½���� */
    unsigned char   acDTServerIp[64];       /* ·�������IP */
    unsigned long   ulDTPort;               /* ·��������˿� */
    unsigned long   ulDTEnable;             /* ·������Ƿ����� */
    unsigned long   ulDTCollectInterval;    /* ·����Ϣ�ɼ���� */
    /* END:   Added by lvjianlin, 2013/7/25   PN:lte-fi DT */
    unsigned long   ulMOC;
    unsigned char   aucMOI[AC_MOI_LEN];
}T_apLtefiBasicSetup;
/* END:   Added by lvjianlin, 2013/6/20   PN:lte-fi  GPS ALARM  LOCAL PORTAL */
typedef struct apSecuritySetup_s
{
	unsigned long 	ulWtpId;
	unsigned long	wifiDeviceId;		
	unsigned long	apVapId;
	
	long			apAuthenticationType;
#define wifiAuthenticationType_AUTO			0
#define wifiAuthenticationType_OPENSYSTEM	1
#define wifiAuthenticationType_SHAREDKEY	2
#define wifiAuthenticationType_WPAPSK		3
#define apAuthenticationType_AUTO wifiAuthenticationType_AUTO
#define apAuthenticationType_OPENSYSTEM wifiAuthenticationType_OPENSYSTEM
#define apAuthenticationType_SHAREDKEY wifiAuthenticationType_SHAREDKEY
#define apAuthenticationType_WPAPSK wifiAuthenticationType_WPAPSK
#define staAuthenticationType_WPA2PSK		4
#define staAuthenticationType_MAC			5
#define staAuthenticationType_WAPI			6
	long			apSecurityMode;
#define wifiSecurityMode_DISABLE			0
#define wifiSecurityMode_WEP				1
#define wifiSecurityMode_TKIP				2
#define staSecurityMode_AES					3
#define staSecurityMode_WAPI				4
	unsigned long	ulWapiSetupId;
	unsigned long	ulCertFileId;
	char			acWapiPsk[64];
	char			apWpaPsk[64];
	unsigned long	apKeyRenewal;
	unsigned long	apWepKeyLen;
#define wifiWepKeyLen_64BITS 1
#define wifiWepKeyLen_128BITS 2
#define apWepKeyLen_64BITS wifiWepKeyLen_64BITS
#define apWepKeyLen_128BITS wifiWepKeyLen_128BITS
	unsigned long	ulInputFormat;
#define inputFormat_HEX		1
#define inputFormat_ASCII	2
	unsigned char	apWepKey[26];
    unsigned char	resv[2];
	unsigned long	apWepKeyIndex;
#define apWepKeyIndex_ONE	1
#define apWepKeyIndex_TWO	2
#define apWepKeyIndex_THREE	3
#define apWepKeyIndex_FOUR	4
	unsigned long	ul8021xMode;			/* �Ƿ�����8021.x */
#define WPA_MODE_DISABLE	0
#define WPA_MODE_ENABLE		1
	unsigned long	ulEapolVer;				/* EAP�汾 */
#define EAPOL_VER_1			1
#define EAPOL_VER_2			2
	unsigned long	ulEapAuthPeriod;		/* EAP��Ȩ���� */
	unsigned long	ulWepBroadcastKeyLen;	/* ��̬WEP�㲥��Կ���� */
#define WEP_BROADCAST_KEY_LEN_64BIT		1
#define WEP_BROADCAST_KEY_LEN_128BIT	2
	unsigned long	ulWepUnicastKeyLen;		/* ��̬WEP�㲥��Կ���� */
#define WEP_UNICAST_KEY_LEN_64BIT WEP_BROADCAST_KEY_LEN_64BIT
#define WEP_UNICAST_KEY_LEN_128BIT WEP_BROADCAST_KEY_LEN_128BIT
	unsigned long	ulWepRekeyPeriod;		/* WEP��Կ�������� */
	int				iRsv[4];				/* Ԥ��λ */

	unsigned long ulMOC;							/* �������ʶ */
	unsigned char aucMOI[AC_MOI_LEN];				/* ����ʵ����ʶ */
} T_apSecuritySetup;

typedef struct radiusSerCfg_s
{
	unsigned char aucAuthSrvAddrMaster[4];	/* ������֤������IP */
	unsigned long ulAuthSrvPortMaster;		/* ������֤�������˿� */
	char acAuthSrvSecretMaster[32];			/* ������֤�������������� */
	unsigned char aucAuthSrvAddrSlave[4];	/* ������֤������IP */
	unsigned long ulAuthSrvPortSlave;		/* ������֤�������˿� */
	char acAuthSrvSecretSlave[32];			/* ������֤�������������� */
	unsigned char aucAcctSrvAddrMaster[4];	/* ���üƷѷ�����IP */
	unsigned long ulAcctSrvPortMaster;		/* ���üƷѷ������˿� */
	char acAcctSrvSecretMaster[32];			/* ���üƷѷ������������� */
	unsigned char aucAcctSrvAddrSlave[4];	/* ���üƷѷ�����IP */
	unsigned long ulAcctSrvPortSlave;		/* ���üƷѷ������˿� */
	char acAcctSrvSecretSlave[32];			/* ���üƷѷ������������� */
	unsigned long ulRetryPrimaryInterval;	/* ���÷������������ */
	unsigned long ulAcctInterimInterval;	/* �Ʒ���Ϣ�ϱ���� */
	int iRsv[4];
} T_radiusSerCfg;

typedef struct apAdvancedSetup_s
{
	unsigned long 	ulWtpId;
	unsigned long	wifiDeviceId;		
	unsigned long	apVapId;
	
		unsigned long ulPrivCap;			/* VLAN���� */
#define PRIV_CAP_USER_PORT		0
#define PRIV_CAP_TRUNK_PORT		1
		unsigned long ulPowerCtrl;			/* �������ʿ��� */
#define POWER_CTRL_DISABLE		0
#define POWER_CTRL_ENABLE		1
		unsigned long ulEndUserTime;		/* �û�����ʱ�� */
		unsigned long ulWmmQos;				/* WMM���� */
#define WMM_QOS_DISABLE			0
#define WMM_QOS_ENABLE			1
		unsigned long ulQosType;			/* QoS���� */
#define QosType_DATA_BG         0           /* ��̨����ҵ�� */
#define QosType_DATA_ALT        1           /* ��������ҵ�� */
#define QosType_DATA_VEDIO      2           /* ��Ƶҵ�� */
#define QosType_DATA_AUDIO      3           /* ����ҵ�� */
#define QosType_DATA_AUTO       8           /* �Զ� */
		unsigned long ulAcLowLimit;			/* �û����������ޣ�RSSI�� */
		unsigned long ulAcUpLimit;			/* �û�����������(RSSI) */
        /* BEGIN: Added by ������, 2012/11/28   PN:���ð�֧�ֵ������û����� */
        unsigned long  ulRxRateLimit;              /*�û�����AP������������*/
        unsigned long  ulTxRateLimit;              /*�û�����AP������������*/
        /* END:   Added by ������, 2012/11/28   PN:���ð�֧�ֵ������û����� */

		unsigned long ulAcInterval;			/* �û�������Ƽ�� */

	unsigned long ulMOC;							/* �������ʶ */
	unsigned char aucMOI[AC_MOI_LEN];				/* ����ʵ����ʶ */
} T_apAdvancedSetup;

#define apMacFilterList_SIZE 64
typedef struct apMacFilterSetup_s
{
	long			apMacFilterSwitch;		/* mac���˹��ܵĿ��� */
#define apMacFilterSwitch_OFF 0
#define apMacFilterSwitch_ON 1
	long			apMacFilterDefaultPolicy;
#define apMacFilterDefaultPolicy_DENY 0
#define apMacFilterDefaultPolicy_PASS 1
	unsigned char	apMacFilterList[apMacFilterList_SIZE][6];
	int				iRsv[4];				/* Ԥ��λ */
} T_apMacFilterSetup;

typedef struct apMacFilterStat_s
{
	unsigned long	apFilterStatPassPkt;
	unsigned char	apFilterStatDenyMac[apMacFilterList_SIZE][6];
	unsigned long	apFilterStatDenyPkt[apMacFilterList_SIZE];
	int				iRsv[4];				/* Ԥ��λ */
} T_apMacFilterStat;

/* �����û��б� */
#define RATE_LIST_MAX_NUM		44  //modify by zjj
#define SNR_LIST_MAX_NUM		17   //modify by zjj
/*������Ϣ*/
/*û�б����ֶ�*/
typedef struct  /*��*/
{
	unsigned long  ulRate;			/*	��bpsΪ��λ����Ϊ1Mbps,���ֵΪ1000000 */
//#define WIFI_RATE_1M          0     /*	1M����   */
//#define WIFI_RATE_2M          1     /*	2M����   */
//#define WIFI_RATE_5DOT5M	  2     /*	5.5M���� */
//#define WIFI_RATE_11M         3     /*	11M����  */
//#define WIFI_RATE_6M          4     /*	6M����   */
//#define WIFI_RATE_9M          5     /*	9M����   */
//#define WIFI_RATE_12M         6     /*	12M����  */
//#define WIFI_RATE_18M         7     /*	18M����  */
//#define WIFI_RATE_24M         8     /*	24M����  */
//#define WIFI_RATE_36M         9     /*	36M����  */
//#define WIFI_RATE_48M         10    /*	48M����  */
//#define WIFI_RATE_54M         11    /*	54M����  */
	unsigned long  ulRecvPkt;
//	unsigned long  ulRecvErrorPkt;	/* ���յĴ���� */
//	unsigned long  ulRecvByteNum;	/* �����ֽ��� */
//	unsigned long  ulRecvRetryPkt;  /* �����ش����� */
	/* hc modified for cmcc test 2009-7-20 */
//	unsigned long  ulRecvRestryByetNum; /* �����ش����ֽ��� */
	unsigned long  ulSendPkt;
//	unsigned long  ulSendByteNum;	    /* �����ֽ��� */
//	unsigned long  ulSendLossPkt;
} T_rateInfo;
typedef T_rateInfo T_rateList[RATE_LIST_MAX_NUM];

typedef struct apEndUser_s
{
	unsigned long ulEndUserIndex;
	unsigned char acEndUserMac[6];
	unsigned char		  acResv[2];
	unsigned char acEndUserIp[4];
	unsigned long ulEndUserLiveTime;
	unsigned long ulApEndUserConBgMode;
	/* added by laiyongxing at 2009-1-17 */
	signed char ucEndUserRssi;
	char noise;
	//end addition
	T_rateList    stRateList;
	    unsigned long       stSNRList[SNR_LIST_MAX_NUM];       /*ap�յ����ն˵ĸ��ŵ�ǿ���µİ���ͳ�ƣ���������֡�͹���֡��*/  /*add by zjj*/
	unsigned long ulPktSendToUser;		/* ����Ŀ */
	unsigned long ulPktRecvFromUser;	/* ����Ŀ */
	
	/*add by zjj*/
	unsigned long   ulSendDataBytes;     /*��ap���͸����ն˵����ݰ����ֽ��������ն˽��յ����ݰ����ֽ�����ֻ�����ݰ���*/ /*����*/
        unsigned long   ulRecvDataBytes;     /*ap�Ӵ��ն˽��յ����ݰ����ֽ��������ն˷��͵����ݰ����ֽ�����ֻ�����ݰ���*/   /*����*/
        unsigned long   ulPktSendToUserAll;             /*�Ӵ��ն˷��͵İ�������������֡�͹���֡��*/ /*����*/     
        unsigned long   ulPktRecvFromUserAll;     /*�Ӵ��ն˽��յİ�������������֡�͹���֡��*/   /*����*/
        unsigned long   ulSendDataBytesAll;     /*��ap���͸����ն˵����ֽ��������ն˽��յ����ֽ�������������֡�͹���֡��*/    /*����*/
        unsigned long   ulRecvDataBytesAll;     /*ap�Ӵ��ն˽��յ����ֽ��������ն˷��͵����ֽ�������������֡�͹���֡��*/      /*����*/
        unsigned long    ulRecvRetryPkt;      /*ap�Ӵ��ն˽��յ��ش����������ն˷��͵��ش�������ֻ������֡��*/      /*����*/
        unsigned long    ulSendRetryPkt;     /*��ap���͵����ն˵��ش����������ն˽��յ��ش�������ֻ������֡��*/      /*����*/

	unsigned long ulendUserCapFlag;		/* �ڵ���������*/

	/* 2010/08/06 fengjing cmcc test */
	unsigned long ulRecvFrag;  /* �ն˷��͵ı���Ƭ�İ��� */
 	unsigned long ulSendFrag;  /* AP���ն˷��͵ı���Ƭ�İ��� */

	/* 2011-03-23 fengjing add:����ź�ǿ�ȡ�����ź�ǿ�� */
	/*      ����     */
	 signed char ucMaxRssi;
	 signed char ucMinRssi;
    /* BEGIN: Added by ������, 2012/12/18   PN:task#287  �������󡶹㶫�ƶ�WLAN�豸���ܽӿڼ���Ҫ��v3.2֮���������(��������壩�� */
    //unsigned short usEndUserRate;     /* �ն˽������� */
    unsigned short usEndUserRate;      /*��ǰAP��������*/
    unsigned short usEndUserTxRate;     /*��ǰAP��������*/
    /* END:   Added by ������, 2012/12/18   PN:task#287  �������󡶹㶫�ƶ�WLAN�豸���ܽӿڼ���Ҫ��v3.2֮���������(��������壩�� */

    /* BEGIN: Added by lvjianlin, 2013/3/29   PN:TASK #653 ��X86ƽ̨����ACƽ̨ϵͳ�����ⳡ���󡿹㶫��������ڵ㡰Э���������ʼ��е������ */
    unsigned short  usUserHighAssocRate;    /* Э���������ʼ��е�������ʣ���λM��˫����ʾ */
    /* END:   Added by lvjianlin, 2013/3/29   PN:TASK #653 ��X86ƽ̨����ACƽ̨ϵͳ�����ⳡ���󡿹㶫��������ڵ㡰Э���������ʼ��е������ */
} T_apEndUser;

typedef struct apEndUserSimple_s
{
	unsigned long ulEndUserIndex;
	unsigned char acEndUserMac[6];
	unsigned char		  acResv[2];
	unsigned char acEndUserIp[4];
	unsigned long ulEndUserLiveTime;
	unsigned long ulApEndUserConBgMode;
	/* added by laiyongxing at 2009-1-17 */
	char ucEndUserRssi;
	char noise;
	//end addition
//	T_rateInfo stRate;
	unsigned long ulPktSendToUser;		/* ����Ŀ */
	unsigned long ulPktRecvFromUser;	/* ����Ŀ */
	
	  unsigned long   ulSendDataBytes;     /*��ap���͸����ն˵����ݰ����ֽ��������ն˽��յ����ݰ����ֽ�����ֻ�����ݰ���*/ /*add by zjj*/
        unsigned long   ulRecvDataBytes;     /*ap�Ӵ��ն˽��յ����ݰ����ֽ��������ն˷��͵����ݰ����ֽ�����ֻ�����ݰ���*/   /*����*/
        unsigned long   ulPktSendToUserAll;             /*�Ӵ��ն˷��͵İ�������������֡�͹���֡��*/ /*����*/     
        unsigned long   ulPktRecvFromUserAll;     /*�Ӵ��ն˽��յİ�������������֡�͹���֡��*/   /*����*/
        unsigned long   ulSendDataBytesAll;     /*��ap���͸����ն˵����ֽ��������ն˽��յ����ֽ�������������֡�͹���֡��*/    /*����*/
        unsigned long   ulRecvDataBytesAll;     /*ap�Ӵ��ն˽��յ����ֽ��������ն˷��͵����ֽ�������������֡�͹���֡��*/      /*����*/
        unsigned long    ulRecvRetryPkt;      /*ap�Ӵ��ն˽��յ��ش����������ն˷��͵��ش�������ֻ������֡��*/      /*����*/
        unsigned long    ulSendRetryPkt;     /*��ap���͵����ն˵��ش����������ն˽��յ��ش�������ֻ������֡��*/      /*����*/

	unsigned long ulendUserCapFlag;		/* �ڵ���������*/

	/* 2010/08/06 fengjing cmcc test */
	unsigned long ulRecvFrag;  /* �ն˷��͵ı���Ƭ�İ��� */
 	unsigned long ulSendFrag;  /* AP���ն˷��͵ı���Ƭ�İ��� */

	/* 2011-03-23 fengjing add:����ź�ǿ�ȡ�����ź�ǿ�� */
	/*      ����     */
	 char ucMaxRssi;
	 char ucMinRssi;
     
     /* BEGIN: Added by ������, 2012/12/18   PN:task#287  �������󡶹㶫�ƶ�WLAN�豸���ܽӿڼ���Ҫ��v3.2֮���������(��������壩�� */
     //unsigned short usEndUserRate;     /* �ն˽������� */
     unsigned short usEndUserRate;      /*��ǰAP��������*/
     unsigned short usEndUserTxRate;     /*��ǰAP��������*/
     /* END:   Added by ������, 2012/12/18   PN:task#287  �������󡶹㶫�ƶ�WLAN�豸���ܽӿڼ���Ҫ��v3.2֮���������(��������壩�� */

    
     /* BEGIN: Added by lvjianlin, 2013/3/29   PN:TASK #653 ��X86ƽ̨����ACƽ̨ϵͳ�����ⳡ���󡿹㶫��������ڵ㡰Э���������ʼ��е������ */
     unsigned short  usUserHighAssocRate;    /* Э���������ʼ��е�������ʣ���λM��˫����ʾ */
     /* END:   Added by lvjianlin, 2013/3/29   PN:TASK #653 ��X86ƽ̨����ACƽ̨ϵͳ�����ⳡ���󡿹㶫��������ڵ㡰Э���������ʼ��е������ */

} T_apEndUserSimple;

/* 2010-3-16 pyy �޸�80-->128 */
#define MAX_ENDUSER_ENTRY_I 128

#define apEndUserList_SIZE MAX_ENDUSER_ENTRY_I
typedef T_apEndUser T_apEndUserList[apEndUserList_SIZE];

#define MAX_END_USER		128
#define OMA_ADDR_LEN		6
typedef struct apEndUserMac_s
{
	char acEndUserMac[MAX_END_USER][OMA_ADDR_LEN];
} T_endUserMac;

typedef struct apAccessDeny_s
{
	unsigned char	apAccessDenyMac[6];
	char			apAccessDenyReason[32];
	unsigned long	apAccessDenyTime;
	int				iRsv[4];					/* Ԥ��λ */
} T_apAccessDeny;
#define MAX_ACCESSDENY_ENTRY_I 10
#define apAccessDenyList_SIZE MAX_ACCESSDENY_ENTRY_I
typedef T_apAccessDeny T_apAccessDenyList[apAccessDenyList_SIZE];

typedef struct apVapBasicEntry_s
{
	unsigned long		apVapId;
	T_apBasicSetup	apBasicSetup;
}T_apVapBaiscEntry;

typedef struct apVapEntryRw_s
{
	unsigned long		apVapId;
	T_apBasicSetup		apBasicSetup;
	T_apSecuritySetup	apSecuritySetup;
	T_apAdvancedSetup	stApAdvancedSetup;
	T_apMacFilterSetup	apMacFilterSetup;
	int					iRsv[4];			/* Ԥ��λ */
} T_apVapEntryRw;
typedef struct apVapEntry_s
{
	unsigned long		apVapId;
	T_apBasicSetup		apBasicSetup;
	T_apSecuritySetup	apSecuritySetup;
	T_apAdvancedSetup	stApAdvancedSetup;
	T_apMacFilterSetup	apMacFilterSetup;
	T_apMacFilterStat	apMacFilterStat;
	T_apEndUserList		apEndUserList;
	T_apAccessDenyList	apAccessDenyList;
	int					iRsv[4];			/* Ԥ��λ */
} T_apVapEntry;

#define apVapTable_SIZE 16
typedef T_apVapEntryRw T_apVapTableRw[apVapTable_SIZE];
typedef T_apVapEntry T_apVapTable[apVapTable_SIZE];
typedef T_apVapBaiscEntry T_apVapBasicTable[apVapTable_SIZE];
#define apVapList_SIZE apVapTable_SIZE
typedef T_apVap T_apVapList[apVapList_SIZE];
typedef T_endUserMac T_apEndUserMac[apVapTable_SIZE];

/* BEGIN: Added by ������, 2012/12/6   PN:TASK #257 VAP���������ù��� ���ð����� */
#define APP_VAP_FLOWCTL_USER_SIZE 128      //AP������û���Ŀ
typedef T_apVapMacFlowCtrlSetup T_apVapFlowCtrlTable[APP_VAP_FLOWCTL_USER_SIZE];
/* END:   Added by ������, 2012/12/6   PN:TASK #257 VAP���������ù��� ���ð����� */

/* BEGIN: Added by ������, 2013/1/10   PN:TASK #257  -2 VAP-MAC�û����ر���� */
typedef T_apVapMacFlowCtrlCount T_apVapFlowCtrlCountTable[APP_VAP_FLOWCTL_USER_SIZE];
/* END:   Added by ������, 2013/1/10   PN:TASK #257  -2 VAP-MAC�û����ر���� */
typedef struct adhocVap_s
{
	char			adhocSsid[32];
	unsigned long	adhocId;
	unsigned long	wifiDeviceId;			/* ���������豸ID */
	int				iRsv[4];				/* Ԥ��λ */
} T_adhocVap;
typedef T_adhocVap T_adhocVapAdd;
typedef T_adhocVap T_adhocVapDelete;

typedef struct adhocBasicSetup_s
{
	unsigned long 	ulWtpId;
	unsigned long	wifiDeviceId;			/* ���������豸ID */
	unsigned long	ulAdhocId;
	
	long			adhocRunMode;
#define adhocRunMode_DISABLE	0
#define adhocRunMode_ENABLE		1
	unsigned char	adhocMac[6];
	char			acRsv[2];
	char			adhocSsid[32];
	long			adhocBroadcastSsid;
#define adhocBroadcastSsid_DISABLE wifiBroadcastSsid_DISABLE
#define adhocBroadcastSsid_ENABLE wifiBroadcastSsid_ENABLE
	unsigned long	ulWifiArithmeticMode;
#define WIFI_ARITHMETIC_MODE_BIND		0
#define WIFI_ARITHMETIC_MODE_QUALITY	1
	unsigned long	ulAntennaMode;			/* ��������ģʽ */
	unsigned long	ulMeshAdhocMode;		/* MESH���� add by yzl 2008-12-8*/
#define MESH_ADHOC_MODE_AC		0			/* MESH���� */
#define MESH_ADHOC_MODE_STA		1			/* MESH�ն� */
#define MESH_ADHOC_MODE_BK		2			/* MESH�ش� */
#define MESH_ADHOC_MODE_ACBK	3			/* MESH����ͻش� */
	unsigned long	ulWmmQos;				/* WMM���� */
#define WMM_QOS_DISABLE			0
#define WMM_QOS_ENABLE			1
	unsigned long	ulQosMap;
#define QOS_MAP_BK				0
#define QOS_MAP_BE				1
#define QOS_MAP_VI				2
#define QOS_MAP_VO				3
#define QOS_MAP_AUTO			8
	unsigned long ulPrivCap;				/* VLAN���� */
	int				iRsv[4];				/* Ԥ��λ */

	unsigned long ulMOC;							/* �������ʶ */
	unsigned char aucMOI[AC_MOI_LEN];				/* ����ʵ����ʶ */
} T_adhocBasicSetup;

typedef struct adhocSecuritySetup_s
{
	unsigned long 	ulWtpId;
	unsigned long	wifiDeviceId;			/* ���������豸ID */
	unsigned long	ulAdhocId;
	
	long			adhocAuthenticationType;
#define adhocAuthenticationType_OPENSYSTEM wifiAuthenticationType_OPENSYSTEM
#define adhocAuthenticationType_SHAREDKEY wifiAuthenticationType_SHAREDKEY
	unsigned long	adhocWepKeyLen;
#define adhocWepKeyLen_64BITS wifiWepKeyLen_64BITS
#define adhocWepKeyLen_128BITS wifiWepKeyLen_128BITS
	char			adhocWepKey[32];
	long			adhocDataEncrypt;
#define adhocDataEncrypt_DISABLE 0
#define adhocDataEncrypt_ENABLE 1
	unsigned long	adhocAuthenticationTimeout;
	unsigned long	adhocAuthenticationRetry;
	int				iRsv[4];					/* Ԥ��λ */

	unsigned long ulMOC;							/* �������ʶ */
	unsigned char aucMOI[AC_MOI_LEN];				/* ����ʵ����ʶ */
} T_adhocSecuritySetup;

#define MAX_SNR_ENTRY_I 8
#define adhocSnrList_SIZE MAX_SNR_ENTRY_I
typedef struct adhocSnr_s
{
	long	adhocChannel;
	long	adhocSnrValue;
	int		iRsv[4];					/* Ԥ��λ */
} T_adhocSnr;
typedef T_adhocSnr T_adhocSnrList[adhocSnrList_SIZE];

#define MAX_ADHOCNEIGHBOR_ENTRY_I 10
#define adhocNeighborList_SIZE MAX_ADHOCNEIGHBOR_ENTRY_I
typedef struct adhocNeighbor_s
{
	unsigned char	adhocNeighborMac[6];
	unsigned char	adhocNeighborIp[4];
	unsigned long	adhocNeighborTxRate;	/* KBPS */
	unsigned long	adhocNeighborRxRate;	/* KBPS */
	unsigned long	adhocNeighborTxPower;	/* ���� */
	long			adhocNeighborRssi;
	int				iRsv[4];				/* Ԥ��λ */
} T_adhocNeighbor;
typedef T_adhocNeighbor T_adhocNeighborList[adhocNeighborList_SIZE];

#define AODV_HOPS_TO_GW_SIZE 16

typedef struct
{
	unsigned long ulMeshGwFlag;                    /* if it is a MESH GW */
#define MESH_GW_FLAG_GW		1
#define MESH_GW_FLAG_UNGW	0
	unsigned long ulInputFlow;                     /* ����ҵ��˿������� */
	unsigned long ulOutputFlow;                    /* ����ҵ��˿ڳ����� */
	unsigned long ulForwardFlow;                   /* ���ж˿ڵ�ת������ */
	unsigned long ulLocalFlow;                     /* ���ж˿ڵı������� */

	unsigned long ulDownlinkDataFlow;              /* ����ҵ������ */
	unsigned long ulUplinkDataFlow;                /* ����ҵ������ */
	unsigned long ulDownlinkMngFlow;               /* ���й������� */
	unsigned long ulUplinkMngFlow;                 /* ���й������� */
	unsigned long ulDownlinkFwdFlow;               /* ����ת������ */
	unsigned long ulUplinkFwdFlow;                 /* ����ת������ */

	unsigned long ulHop2GwNum;
	unsigned char acHop2GwAddr[AODV_HOPS_TO_GW_SIZE][6];

} AODV_NODEINFO_T;

typedef struct adhocVapEntryRw_s
{
	unsigned long			ulAdhocVapId;
	T_adhocBasicSetup		stAdhocBasicSetup;
	T_adhocSecuritySetup	stAdhocSecuritySetup;
	int						iRsv[4];				/* Ԥ��λ */
} T_adhocVapEntryRw;
typedef struct adhocVapEntry_s
{
	unsigned long			ulAdhocVapId;
	T_adhocBasicSetup		stAdhocBasicSetup;
	T_adhocSecuritySetup	stAdhocSecuritySetup;
	T_adhocSnrList			stAdhocSnrList;
	T_adhocNeighborList		stAdhocNeighborList;
	int						iRsv[4];				/* Ԥ��λ */
} T_adhocVapEntry;

#define adhocVapTable_SIZE 4
typedef T_adhocVapEntryRw T_adhocVapTableRw[adhocVapTable_SIZE];
typedef T_adhocVapEntry T_adhocVapTable[adhocVapTable_SIZE];
#define adhocVapList_SIZE adhocVapTable_SIZE
typedef T_adhocVap T_adhocVapList[adhocVapList_SIZE];


#define sysPort_ADHOC_NUM 4
#define sysPort_ENET_NUM 4
#define sysPort_VAP_NUM 16
#define sysPort_NUM (sysPort_ADHOC_NUM + sysPort_ENET_NUM + sysPort_VAP_NUM)
#define sysPortStat_SIZE sysPort_NUM
#define vlanSetupList_SIZE sysPort_VAP_NUM
typedef struct sysPortStat_s
{
	unsigned long	sysPortId;
	char			sysPortName[48];
	unsigned long	sysPortVlanList[vlanSetupList_SIZE];
	unsigned long	sysPortState;
#define sysPortState_NORMAL		1
#define sysPortState_DISABLED	2
#define sysPortState_OFFLINE	3
	unsigned long	sysPortRecvPkt;
	unsigned long	sysPortSendPkt;
	int				iRsv[4];			/* Ԥ��λ */
} T_sysPortStat;

#define vlanPortList_SIZE sysPort_NUM
typedef struct vlanSetup_s
{
	unsigned long	vlanId;
	unsigned long	ulVlanPrio;			/* VLAN���ȼ� */
#define VLAN_PRIO_0		0
#define VLAN_PRIO_1		1
#define VLAN_PRIO_2		2
#define VLAN_PRIO_3		3
//#define QosType_DATA_AUTO       8     /* �Զ� */
	char			vlanName[32];
	unsigned long	ulVlanManageSwitch;
#define vlanManageSwitch_NO		0
#define vlanManageSwitch_YES	1
	unsigned long	vlanPortList[vlanPortList_SIZE];
	unsigned char	aucIpAddr[4];
	unsigned char	aucNetMask[4];
	unsigned char	aucDefaultGw[4];
	unsigned long	ulL2Isolate;		/* ������� */
#define L2_ISOLATE_DISABLE		0
#define L2_ISOLATE_ENABLE		1
	int				iRsv[4];			/* Ԥ��λ */
} T_vlanSetup;
typedef T_vlanSetup T_vlanSetupList[vlanSetupList_SIZE];

typedef T_vlanSetup T_vlanAdd;

typedef unsigned long T_vlanDelete;

typedef T_vlanSetup T_vlanModify;

typedef long T_vlanSwitch;
#define vlanSwitch_OFF 0
#define vlanSwitch_ON 1

typedef struct vlanPort_s
{
	unsigned long	portId;
	char			portName[48];
	unsigned long	portVlanState;
#define portVlanState_NO 0
#define portVlanState_YES 1
	unsigned long	ulPrivCap;
	int				iRsv[4];			/* Ԥ��λ */
} T_vlanPort;
#define vlanPortSelect_SIZE sysPort_NUM
#define INVALID_PORT_ID 0xFFFFFFFF
typedef T_vlanPort T_vlanPortSelect[vlanPortSelect_SIZE];

#define ENETINTERFACE_MAX_NUM	2
typedef struct ethernetInterface_s
{
	unsigned long	ulWtpId;
	unsigned long ulEnetPortId;
	
	unsigned long	ulEnetRunMode;		/* �Ƿ����� */
#define enetRunMode_DISABLE		0
#define enetRunMode_ENABLE		1
	unsigned long	ulSpeedUplex;		/* ����/˫��ģʽ */
#define speedUplex_AUTO			0		/* ����Ӧ */
#define speedUplex_10M			1		/* 10Mȫ˫�� */
#define speedUplex_100M			2		/* 100Mȫ˫�� */
#define speedUplex_1000M		3		/* 1000Mȫ˫�� */
	unsigned long	ulEnetState;		/* ����״̬ */
#define enetState_UPLINK		0		/* ������ */
#define enetState_DOWNLINK		1		/* ������ */
	unsigned long	ulPrivCap;			/* VLAN���� */
	int				iRsv[4];			/* Ԥ��λ */

	unsigned long ulMOC;							/* �������ʶ */
	unsigned char aucMOI[AC_MOI_LEN];				/* ����ʵ����ʶ */
} T_enetInfBasicSetup;

typedef struct
{
	T_enetInfBasicSetup	stEnetInfSetup;
	int		iRsv[4];					/* Ԥ��λ */
} T_enetInfSetup;
typedef T_enetInfSetup T_enetInfTable[ENETINTERFACE_MAX_NUM];

typedef struct configInforce_s
{
	int		iInfoceInvalidFlag;
#define INFOCE_INVALID_FLAG_SET			0
#define INFOCE_INVALID_FLAG_DELAY_SET	2
	time_t  inforceTime;
	int		iRsv[4];					/* Ԥ��λ */
}T_configInforce;

/* CAPWAPЭ������ */
typedef struct capWapSetup_s
{
	unsigned long ulWtpId;					/* WTP�豸ID */
	char		  acSecret[32];				/* WTP�豸�������� */
	unsigned long ulDisType;				/* AC�������� */
#define DIS_TYPE_AUTO		0	
#define DIS_TYPE_DHCP		1
#define DIS_TYPE_DNS		2
#define DIS_TYPE_MULTICAST	3
	char		  acAcURL[64];				/* AC URL */
	unsigned long ulMOC;					/* �������ʶ */
	unsigned char aucMOI[AC_MOI_LEN];		/* ����ʵ����ʶ */
} T_capWapSetup;

typedef struct perfCfg_s
{
	unsigned long	ulPerfTaskNum;
	unsigned long   ulPerfCounterGroupID;
	unsigned long   ulStatus;				/* �Ƿ�������ͳ�� */
#define PERF_DISABLE		0				/* �ر� */
#define PERF_ENABLE			1				/* ���� */
	unsigned long   ulPerfReportInterval;	/* ͳ�Ƽ�� */
	unsigned long   ulRealTimeQuery;		/* �Ƿ���ʵʱ��ѯ */
#define REAL_TIME_QUERY_DISABLE		0		/* �ر� */
#define REAL_TIME_QUERY_ENABLE		1		/* ���� */
	int iRsv[4];							/* Ԥ��λ */
} OMA_PERF_CFG_T;

#define PERF_COUNT	3

typedef struct
{
	unsigned long ulWtpId;
	
    unsigned long   ulAlarmNo;              /* �澯ID */
    char            acAlarmName[32];        /* �澯���� */
    unsigned long   ulAlarmIsValid;         /* �澯�Ƿ���Ч	0:���� 1:���� */

   	unsigned long ulMOC;							/* �������ʶ */
	unsigned char aucMOI[AC_MOI_LEN];				/* ����ʵ����ʶ */
} OMA_ALARM_FILTER_CFG_T;

#define INVALID_ALARM_NO    0xffffffff
#define ALARM_COUNT	        128

typedef struct
{
	unsigned long ulWtpId;
	
    unsigned long   ulCpuLoadPeriod	;	/* CPU���ɲ�������	    5       */
    unsigned long   ulCpuLoadCount	;	/* CPU���ɳ�������	    6       */
    unsigned long   ulCpuLoadHigh	;	/* CPU���ɸ澯��������	90      */
    unsigned long   ulCpuLoadLow	;	/* CPU���ɸ澯��������	80      */

    unsigned long   ulMemLoadPeriod	;	/* �ڴ������ʲ�������	    5   */
    unsigned long   ulMemLoadCount	;	/* �ڴ������ʳ�������		6   */
    unsigned long   ulMemLoadHigh	;	/* �ڴ������ʸ澯��������	85  */
    unsigned long   ulMemLoadLow	;	/* �ڴ������ʸ澯��������	80  */

    unsigned long   ulTempPeriod	;	/* �豸�¶Ȳ�������	    5       */
    unsigned long   ulTempCount	    ;	/* �豸�¶ȳ�������	    6       */
    unsigned long   ulTempHigh	    ;	/* �豸���¸澯��������	55      */
    unsigned long   ulTempLow	    ;	/* �豸���¸澯��������	50      */
    int             iTempHigh	    ;	/* �豸���¸澯��������	-30     */
    int             iTempLow	    ;	/* �豸���¸澯��������	-20     */

    unsigned long ulMOC;							/* �������ʶ */
	unsigned char aucMOI[AC_MOI_LEN];				/* ����ʵ����ʶ */
}OMA_ALARM_THRESHOLD_CFG_T;

/* BEGIN: Added by ������, 2012/12/10   PN:TASK #257 VAP���������ù��� ���ð����� */
typedef 	struct
{
	unsigned long	ulWtpId;					/*AP�豸ID */
	unsigned long	ulWifiDeviceId;			/*�����豸���*/
	unsigned long	ulVapId;					/* VAP ID */
	unsigned char	aucMac[6];				/*�û�MAC */
	unsigned char	aucResv[2];				/*�����ֶΣ�Ϊ�����ֽڶ���*/
	unsigned long	ulUserUpBandMax;		/*�����û�������, kbps ,ȱʡֵ1024*/
	unsigned long	ulUserDownBandMax;		/*�����û�������kbps ,ȱʡֵ1024*/
	unsigned long	ulMOC;					/*�������ʶ�������Զ�����,�·�����Ԫ*/
	unsigned char	aucMOI[AC_MOI_LEN];	/*����ʵ����ʶ�������Զ�����,�·�����Ԫ*/
}OMA_VAP_MAC_FLOWCTRL_SETUP_T;

/* END:   Added by ������, 2012/12/10   PN:TASK #257 VAP���������ù��� ���ð����� */

/* BEGIN: Added by lvjianlin, 2013/6/20   PN:lte-fi  GPS ALARM  LOCAL PORTAL */
typedef struct
{
    unsigned long   ulWtpId;                /* WTP  id */
    unsigned char   acLtefiApn[32];         /* ��������� */
    unsigned long   ulLtefiMtu;             /* MTUֵ */
    unsigned long   ulAccessMode;           /* LTE-FI����ģʽ */
    unsigned long   ulBatterySptTime;       /* ��ع���ʱ�� */
    unsigned char   acBusNo[32];            /* ������� */
    unsigned char   aucApForwardBrIp[4];    /* �ȱ��غ��е�����µ����ص�ַ */
    unsigned char   aucApForwardBrMask[4];  /* �ȱ��غ��е�����µ����ص�ַ���� */
    unsigned char   acGpsServer1[64];       /* GPS��Ϣ�ϱ�������1 */
    unsigned char   acGpsServer2[64];       /* GPS��Ϣ�ϱ�������2 */
    unsigned long   ulGpsReportPeriod;      /* GPS��Ϣ�ϱ����� add by liyoukun*/
    unsigned char   acContextServer1[128];  /* LTE-Fi���������ط�����1 */
    unsigned char   acContextServer2[128];  /* LTE-Fi���������ط�����2 */
    unsigned char   acLtefiPortalIp[64];    /* ltefi�豸����portalIP��ַ add by liyoukun*/
    /* BEGIN: Added by lvjianlin, 2013/7/11   PN:ulDataUpdateTime */
    unsigned long   ulDataUpdateTime;       /* ���ڸ��±��ش洢�ռ����� */
    /* END:   Added by lvjianlin, 2013/7/11   PN:ulDataUpdateTime */
    /* BEGIN: Added by lvjianlin, 2013/7/25   PN:lte-fi DT */
    unsigned char   acDTUserName[64];       /* ·���������½�û��� */
    unsigned char   acDTPass[64];           /* ·���������½���� */
    unsigned char   acDTServerIp[64];       /* ·�������IP */
    unsigned long   ulDTPort;               /* ·��������˿� */
    unsigned long   ulDTEnable;             /* ·������Ƿ����� */
    unsigned long   ulDTCollectInterval;    /* ·����Ϣ�ɼ���� */
    /* END:   Added by lvjianlin, 2013/7/25   PN:lte-fi DT */
    unsigned long   ulMOC;
    unsigned char   aucMOI[AC_MOI_LEN];
}OMA_LTEFI_BASIC_SETUP_T;

/* END:   Added by lvjianlin, 2013/6/20   PN:lte-fi  GPS ALARM  LOCAL PORTAL */
#define SSID_LIST_SIZE	9
typedef struct
{
	unsigned long ulWifiDeviceId;			/* ���������豸ID */
	unsigned long ulRunMode;
#define STA_RUN_MODE_DISABLE	0
#define STA_RUN_MODE_ENABLE		1
	char acMac[6];
	char acRsv[2];
	char acBasicSsid[32];
	unsigned long ulPrivCap;	/* �Ƿ���������Ϣ */
	int iRsv[4];				/* Ԥ���ֶ� */
} STA_BASIC_SETUP_T;

typedef struct
{
	unsigned long ulBeaconMissInterval; /* ��ʧBeacon��� */
	unsigned long ulScanOffChan;		/* �ŵ��л�ʱ�� */
	unsigned long ulRssiThreshold;		/* �л�RSSI���� */
	unsigned long ulRateThreshold;		/* �л�RATE���� */
	unsigned long ulRoamCheck;			/* �л����ʱ�� */
	unsigned long ulRoamDiff;			/* �л��źŲ�ֵ */
	unsigned long ulProbeint;			/* ��ǰ�ŵ�̽������ */
	unsigned long ulBgscanSwitch;		/* BGɨ�迪�� */
#define BG_SCAN_SWITCH_OFF	0
#define BG_SCAN_SWITCH_ON	1
	unsigned long ulRssilpfAth;			/* �����ź�ƽ������ */
	unsigned long ulRssilpfScan;		/* ɨ���ź�ƽ������ */

	/* under field is added by yzl in 2008-12-18 */
	unsigned long ulBgscanintvl;		/* ɨ���� */
	unsigned long ulScanresult;			/* ����ʱ�� */
	unsigned long ulIdleassocdiff;		/* ���й�����ֵ */
	unsigned long ulBgscanidle;			/* ҵ����� */
	int iRsv[4];
} STA_ADVANCED_SETUP_T;

typedef struct
{
	unsigned long ulAuthenticationType;
#define staAuthenticationType_AUTO apAuthenticationType_AUTO
#define staAuthenticationType_OPENSYSTEM apAuthenticationType_OPENSYSTEM
#define staAuthenticationType_SHAREDKEY apAuthenticationType_SHAREDKEY
#define staAuthenticationType_WPAPSK apAuthenticationType_WPAPSK
	unsigned long ulSecurityMode;
#define staSecurityMode_DISABLE 0
#define staSecurityMode_WPAPSK 1
#define staSecurityMode_WEP 2
	unsigned long ulWpaAlgorithm;
#define staWpaAlgorithm_TKIP 1
#define staWpaAlgorithm_AES 2
	char acWpaPsk[64];
	unsigned long ulKeyRenewal;
	unsigned long ulWepKeyLen;
#define staWepKeyLen_64BITS wifiWepKeyLen_64BITS
#define staWepKeyLen_128BITS wifiWepKeyLen_128BITS
	unsigned long ulInputFormat;
#define inputFormat_HEX		1
#define inputFormat_ASCII	2
	unsigned char acWepKey[26];
    unsigned char resv[2];
	unsigned long ulWepKeyIndex;
#define staWepKeyIndex_ONE		1
#define staWepKeyIndex_TWO		2
#define staWepKeyIndex_THREE	3
#define staWepKeyIndex_FOUR		4
	int iRsv[4];
} STA_SECURITY_SETUP_T;

#define MAX_DES_BSSID	10
typedef unsigned char BIND_BSSID_ENTRY[MAX_DES_BSSID][IEEE80211_ADDR_LEN];

/* �����ն���Ϣ */
#define RELAY_CLIENT_MAX  64
typedef struct
{
	char acMac[IEEE80211_ADDR_LEN]; /* �����ն˵�MAC */
	char acIp[IEEE80211_IPADDR_LEN];/* �����ն˵�IP */
	unsigned long ulUcastRecv;		/* ���յ������� */
	unsigned long ulUcastSend;		/* ���͵������� */
	unsigned long ulMcastSend;		/* ���͹㲥���� */
	unsigned long ulLastRecv;		/* �������ʱ�� */
	unsigned long ulLastSend;		/* �������ʱ�� */
} RELAY_CLIENT_T;

typedef RELAY_CLIENT_T RELAY_CLIENT_ENTRY_T[RELAY_CLIENT_MAX];

/* ��ʷ������Ϣ */
typedef struct
{
	unsigned char acRseApmac[IEEE80211_ADDR_LEN];	/* ����AP�ĵ�ַ����ȫ0�����ʾ�ü�¼Ϊ�� */
	unsigned char ucRseChannel;						/* ����AP���ŵ� */
	char cRseLeaveRssi;								/* �뿪��APʱ��RSSIֵ */
	unsigned char ucRseLeaveRate;					/* �뿪��APʱ��RATEֵ */
	unsigned char ucRsv[3];
	unsigned long ulRseJoinTime;					/* ���������AP��ʱ�� */
	unsigned long ulRseLeaveTime;					/* �뿪��APʱ��ʱ�� */
	unsigned long ulRseAssocTime;					/* ������AP�ɹ�ʱ��ʱ�� */
#define RSE_LEAVE_REASON_ROAM			1
#define RSE_LEAVE_REASON_DESBSSID       2
#define RSE_LEAVE_REASON_ASSFAIL        3
	unsigned char ucRseLeaveReason;					/* �뿪��AP��ԭ�� */
	unsigned char aucRsv[3];
} ROAM_STAT_ELEM_T;

#define ROAM_STAT_ELEM_MAX 10
typedef struct
{
	unsigned long ulRsApsInd;							/* ��ǰ��¼���±꣬Ҳ��rs_aps_indΪ���ϵļ�¼ */
	ROAM_STAT_ELEM_T astRsRoamAps[ROAM_STAT_ELEM_MAX];	/* ������Ϣ */
	unsigned long ulRsRoamNum;							/* �л������¹����Ĵ��� */
	unsigned long ulRsNow;								/* ��ǰ��ʱ�� */
	unsigned long ulInterval;
} ROAM_STAT_T;

/* ɨ���ŵ��б���Ϣ */
typedef struct
{
	unsigned char ucChan;
#define IOCTL_CHAN_MODE_11A  1
#define IOCTL_CHAN_MODE_11B  2
#define IOCTL_CHAN_MODE_11G  3
	unsigned char ucChanMode;
#define NO_SCAN_FLAG		1
#define SCAN_FLAG			0
	unsigned char ucScanFlag;
} CHAN_SCAN_INFO_T;

#define MAX_CHAN_NUM         255
typedef CHAN_SCAN_INFO_T CHAN_SCAN_INFO_ENTRY_T[MAX_CHAN_NUM];

#define	IEEE80211_RATE_VAL		0x7f
#define	IEEE80211_CAPINFO_IBSS			0x0002
/*
* 802.11 rate set.
*/
#define	IEEE80211_RATE_SIZE	8		/* 802.11 standard */
#define	IEEE80211_RATE_MAXSIZE	15		/* max rates we'll handle */
typedef struct
{
	unsigned short isr_len;             /* length (mult of 4) */
	unsigned short isr_freq;            /* MHz */
	unsigned short isr_flags;           /* channel flags */
	unsigned char isr_noise;
	unsigned char isr_rssi;
	//u_int8_t isr_intval;				/* beacon interval */
	unsigned short isr_intval;          /* beacon interval */
	unsigned short isr_capinfo;         /* capabilities */
	unsigned char isr_priv_capinfo;
	/* added by lyx at 2009-1-20 */
	unsigned long  isr_beacons;         /* beacons����*/
	unsigned long  isr_lifetime;        /* ͳ��Beaconʱ�䣬��λs */
	//end addition
	unsigned char isr_erp;              /* ERP element */
	unsigned char isr_bssid[IEEE80211_ADDR_LEN];
	unsigned char isr_nrates;
	unsigned char isr_rates[IEEE80211_RATE_MAXSIZE];
	unsigned char isr_ssid_len;         /* SSID length */
	unsigned char isr_ie_len;           /* IE length */
	unsigned char isr_pad[5];
	/* variable length SSID followed by IE data */
} ieee80211req_scan_result;

typedef struct
{
	char acBuf[24*1024];
	int	iLen;
	char acSta1Buf[24*1024];
	int iSta1Len;
	unsigned long ulUpInterval;
}STA_SCAN_INFO_T;

/* �ն˹���VAP��Ϣ */
typedef struct
{
	unsigned char assocApMac[6];			/* �������С��ͣ��õ�ַ */
	unsigned char aucRsv[2];
	unsigned char assocAPIp[4];				/* �������С�IP��ַ */
	unsigned long assocTimes;				/* �������г���ʱ�� */
	unsigned long ulBgMode;					/* ģʽ */
	unsigned char ucAssocApRssi;			/* ���������ź�ǿ�� */
	char cNoise;							/* AP Noise */
	unsigned char aucRsv1[2];
	T_rateList    stRateList;				/* �����б� */
	unsigned long ulPktSendToAp;			/* ���Ͱ���Ŀ */
	unsigned long ulPktRecvFromAp;			/* ���հ���Ŀ */
} OMA_STA_ASSOCIATE_T;

typedef struct
{
	unsigned long ulStaVapId;
	STA_BASIC_SETUP_T stBasicSetup;
	STA_ADVANCED_SETUP_T stAdvancedSetup;
	STA_SECURITY_SETUP_T stSecuritySetup;
	BIND_BSSID_ENTRY stBindBssidEntry;
    int         iRsv[4];
} STA_VAP_ENTRY_T;

#define STA_VAP_TABLE_SIZE 8
typedef STA_VAP_ENTRY_T OMA_STA_VAP_TABLE[STA_VAP_TABLE_SIZE];

typedef struct _MESH_VLAN_MAP_IP_CFG_T
{
	unsigned long ulVlanId;			/* ����ӳ���VLAN ID */
	char acIp[4];					/* ����ӳ���IP���� */
	char acMask[4];					/* ����ӳ���IP��������(��������ӳ�䣬mask��ȫ0xFF) */
	unsigned long ulRsv[4];			/* �����ֶ� */
} MESH_VLAN_MAP_IP_CFG_T;

/* ����ETH_VLAN_MAP(���߶˿�VLANӳ�����)�������ݽṹ */
#define VLAN_MAP_OFF               0 /* ���������߶˿�VLANӳ�� */
#define VLAN_MAP_IP                1 /* ʹ��IPӳ��ģʽ (ĿǰĬ��ֵ) */
#define VLAN_MAP_OTHER             2 /* ʹ������ӳ��ģʽ */
typedef struct
{
	unsigned long ulMapMode;        /* ����ģʽ */
	unsigned long ulValidNum;
    MESH_VLAN_MAP_IP_CFG_T  stIpVlanMap[vlanSetupList_SIZE];
    //MESH_VLAN_MAP_MAC_CFG_T stMacVlanMap[vlanSetupList_SIZE];

} MESH_ETH_VLAN_MAP_T;

/*
* config file format:
*/

typedef struct configFileData_s
{
	T_configFileHeader		configFileHeader;
	T_sysBasicSetup			sysBasicSetup;
	T_sysIpSetup			sysIpSetup;
	T_sysKeeperSetup		sysKeeperSetup;
	T_sysSwServerSetup		sysSwServerSetup;
	T_sysSnmpSetup			sysSnmpSetup;
	T_sysSnmpV2Table		stSnmpV2Table;
	T_sysSnmpV3Table		stSnmpV3Table;
	T_radiusSerCfg			stRadiusSerCfg;
    T_sysVersionSetup		sysVersionSetup;
	T_sysPasswordChange		sysPasswordChange;
	T_sysPeriodOfValidity	sysPeriodOfValidity;
	/* used to hold logo info added at 2009-1-21 */
	//T_SYSLOGO               stSysLogo;
	char					acRsv[396];
	char					acAppendScript[4096];
	T_sysAppendCode			sysAppendCode;
	T_wifiDeviceTable		wifiDeviceTable;
	T_apVapTableRw			apVapTableRw;
	T_adhocVapTableRw		adhocVapTableRw;
	T_enetInfTable			stEnetInfTable;
	T_aclPolicySwitch		aclPolicySwitch;
	T_fclBasicSetup			stFclBasicSetup;
	T_ipForwardSetup		stIpForwardSetup;
	T_tcpOptimizationSetup	stTcpSetup;
	T_routeBasicSetup		stRouteBasicSetup;
	T_routerNeighSetup		routerNeighSetup;
	T_routerHandoverSetup	routerHandoverSetup;
	T_routerMscSetup		routerMscSetup;
	T_vlanSwitch			vlanSwitch;
	T_vlanSetupList			vlanSetupList;
	T_configInforce			configInforce;
	T_capWapSetup			stCapWapSetup;
	/* added by laiyongxing at 2009-3-02 */
	T_aclSetupEntry            stAclSetup;
	OMA_PERF_CFG_T			    stPerfCfg[PERF_COUNT];
	OMA_ALARM_FILTER_CFG_T	    stAlarmFilterCfg[ALARM_COUNT];
	OMA_ALARM_THRESHOLD_CFG_T	stAlarmThresholdCfg;
	MESH_ETH_VLAN_MAP_T		stVlanMapSetup;
	OMA_STA_VAP_TABLE		stStaVapTable;
	T_sysLoadBalanceSetup	stLoadBalanceSetup;
	T_sysLogSetup			stLogSetup;
	T_sysSntpSetup			stSntpSetup;
	T_sysPPPoESetup			stPPPoESetup;
	T_sysParaSetup          stParaSetup;
    /* BEGIN: Added by ������, 2012/12/6   PN:TASK #257 VAP���������ù��� ���ð����� */
    /* BEGIN: Added by ������, 2013/1/14   PN:TASK #257  -2 VAP-MAC�û����ر���� */
    /* ���ð汾�����޸ģ���capwap��ֲ��usrm */
    #if 0
    T_apVapFlowCtrlTable    stVapMacFlowCtrlSetup;
    #endif
    /* END:   Added by ������, 2013/1/14   PN:TASK #257  -2 VAP-MAC�û����ر���� */
    /* END:   Added by ������, 2012/12/6   PN:TASK #257 VAP���������ù��� ���ð����� */

    /* BEGIN: Added by lvjianlin, 2013/6/20   PN:lte-fi  GPS ALARM  LOCAL PORTAL */
    T_apLtefiBasicSetup     stLtefiBasicSetup;
    /* END:   Added by lvjianlin, 2013/6/20   PN:lte-fi  GPS ALARM  LOCAL PORTAL */

} T_configFileData;

typedef struct deviceModel_s
{
	int iDeviceType;
	int iC[wifiModule_SIZE];
#define WIFI_DISABLE		0
#define WIFI_AP				1
#define WIFI_ADHOC			2
#define WIFI_STA_ADHOC		3
#define WIFI_STA			4
#define WIFI_AP_ADHOC		5
	char cTypeName[32];
	int iWifiIncreaseMode;
#define WIFIINCREASE_DISABLE		0
#define WIFIINCREASE_ENABLE			1
	int iRouteMode[2];
#define ROUTE_MODE_NONE				0
#define ROUTE_MODE_DYNAMIC			1
#define ROUTE_MODE_STATIC			2
	int iWifiCardModeAp;
#define WIFI_CARD_MODE_AP_STANDARD	0
#define WIFI_CARD_MODE_AP_ABG		1
#define WIFI_CARD_MODE_AP_DISABLE	2
	int iWifiCardModeAdhoc[wifiModule_SIZE];
#define WIFI_CARD_MODE_ADHOC_STANDARD	0
#define WIFI_CARD_MODE_ADHOC_A			1
#define WIFI_CARD_MODE_ADHOC_DISABLE	2
	int iAntennaMode;
#define ANTENNA_MODE_DISABLE		0
#define ANTENNA_MODE_ENABLE			1
} T_deviceModel;

typedef struct deviceMac_s
{
	unsigned char ucApMac[apVapTable_SIZE][6];
	unsigned char ucAdhocMac[adhocVapTable_SIZE][6];
}T_deviceMac;

#define AC_DEVICE_INFO_FILE	"/usr/local/ac/config/acabs.dat"
typedef struct deviceInfo_s
{
	int				iFlag;
#define CHECK_FLAG	0xcccccccc
	char			acDvName[32];			/* �豸���� */
	char			acIp[32];				/* �豸IP */
	char			acMac[32];				/* �豸MAC */
	char			acDvId[32];				/* �豸ID */
	char			acSwVersion[32];		/* ����汾 */
	char			acTypeName[32];			/* �豸�ͺ� */
	unsigned long	ulWtpId;

    unsigned char   ucSysVersion;			/* �ļ�ϵͳ */
	unsigned char   ucUpdateStatus;			/* ����״̬ */
	unsigned char   ucOemManu;				/* OEM���� */
	unsigned char   ucVlanID;				/* VLAN ID */

	// add by leijie
	unsigned char   ucCmdSn;                /* �������к� */
	unsigned char   ucCmdStatus;            /* ����ִ��״̬ */
#define CMD_STATUS_NULL         0
#define CMD_STATUS_UNKNOW       1
#define CMD_STATUS_RUNNING      2
#define CMD_STATUS_FAIL         3
#define CMD_STATUS_SUCCESS      4

    unsigned char   ucSnmpSetSn;            /* SNMP�������к� */
    unsigned char   ucSnmpSetStatus;        /* SNMP����״̬ */

    char			acTargetSwVersion[32];  /* ����汾 */
    char			acSwVersion0[32];       /* ����汾0 */
    char			acSwVersion1[32];       /* ����汾0 */

    char			acHwType[32];           /* Ӳ������ */
    char			acHwVersion[32];        /* Ӳ���汾 */

    char			acManagementSSID[32];   /* ����VAP SSID */

    unsigned long   ulUptime;
    unsigned long   ulRebootTime;

    int             iRestoreKeyPressNum;

    char            acErrInfo[32];

    unsigned short  usMacRepeatTime;
    unsigned short  usSnRepeatTime;

    T_WIFI_AMP_CFG  stWifiAmpCfg1;
    T_WIFI_AMP_CFG  stWifiAmpCfg2;

    char            acOperInfo[64];

    unsigned char	ucOperRet;

	unsigned char	ucAbgMode;
	unsigned char	ucChannelState;
    unsigned char	ucChannel;

    unsigned char	ucAuthType;
	unsigned char   ucSecurityMode;

	int             iOperatorId;

    char            acLocalSn[32];         // ���ط����sn

	char			acRsv[22];				/* Ԥ��λ */
}OMA_DEVICE_INFO_T;

typedef struct
{
    unsigned long   ulAcId;                 /*AC ID*/
    char            acAcDevName[32];        /*AC NAME*/
    char            acAcDevType[32];        /*�豸�ͺ�*/
    char            acAcDevSN[6];           /*�豸���кţ�λ�� /etc/gb*/
    unsigned char   aucResv1[2]; 
    char            acAcSwVersion[32];      /*�豸����汾*/
    char            acAcHwVersion[32];      /*�豸Ӳ���汾 */

    unsigned char   aucFitLanIp[4];         /* FIT-LAN IP */
    unsigned char   aucFitLanMask[4];       /* FIT-LAN MASK */     

    unsigned char   aucFatLanIp[4];         /* FAT-LAN IP */
    unsigned char   aucFatLanMask[4];       /* FAT-LAN MASK */ 

    unsigned char   aucL2WanIp[4];          /* L2-WAN IP */
    unsigned char   aucL2WanMask[4];        /* L2-WAN MASK */

    unsigned char   aucL3WanIp[4];          /* L3-WAN IP */
    unsigned char   aucL3WanMask[4];        /* L3-WAN MASK */

    unsigned char   aucDefaultGw[4];        /* ȱʡ����*/
    unsigned char   aucPrimaryDns[4];       /* ����DNS������*/
    unsigned char   aucSecondaryDns[4];     /* ����DNS������*/
} AC_DEVM_DEVICE_ABSTRACT_T;

#define AODV_NEIGH_STATUS_FATHER    1   /* the neigh is my father */
#define AODV_NEIGH_STATUS_SON       2   /* the neigh is my son */
#define AODV_NEIGH_STATUS_OTHER     0   /* other relation */
#define AODV_NEIGHGW_SON            1   /* I am the gw of the neigh */
#define AODV_NEIGHGW_MY_GW          2   /* the neigh is my gw */
#define AODV_NEIGHGW_OTHER_GW       3   /* the neigh is gw, but it's not my gw */
#define AODV_NEIGHGW_OTHER          0   /* the neigh is not gw */
#define AODV_NEIGHBORLIST_SIZE      16  /* max neigh num */

typedef struct
{
	unsigned char aucAodvNeighDeviceId[6];  /* device id */
	unsigned char aucAodvNeighMac[6];       /* MAC */
	unsigned char aucAodvNeighIp[4];        /* IP */
	unsigned long ulAodvLocalCost;          /* local cost */
	unsigned long ulAodvPeerCost;           /* peer cost */
	unsigned long ulAodvNeighStatus;        /* neigh status */
	unsigned long ulAodvNeighGwStatus;      /* gw status */
	unsigned long ulAodvNeighHelloLoss;     /* lost hello number */
	unsigned long ulAodvNeighSendPkt;       /* total num of pkts the neigh send to me */
	unsigned long ulAodvNeighRecvPkt;       /* total num of pkts I send to the neigh */
	char acAodvNeighPortName[16];           /* if name */
	unsigned long ulNeighborChannel;		/* channel */
} AODV_NEIGHBOR_T;

typedef struct
{
	unsigned long ulNum;    /* number of valid neighbors */
	AODV_NEIGHBOR_T astNeighEntry[AODV_NEIGHBORLIST_SIZE];
} AODV_NEIGHBOR_LIST_T;


typedef union
{
	T_configFileHeader		configFileHeader;
	T_sysBasicSetup			sysBasicSetup;
	T_sysIpSetup			sysIpSetup;
	T_sysKeeperSetup		sysKeeperSetup;
	T_sysSwServerSetup		sysSwServerSetup;
	T_sysSnmpSetup			sysSnmpSetup;
    T_sysVersionSetup		sysVersionSetup;
	T_sysPasswordChange		sysPasswordChange;
	T_sysPeriodOfValidity	sysPeriodOfValidity;
	char					acAppendScript[4096];
	T_sysAppendCode			sysAppendCode;
	T_wifiDeviceTable		wifiDeviceTable;
	T_apVapTableRw			apVapTableRw;
	T_adhocVapTableRw		adhocVapTableRw;
	T_enetInfTable			stEnetInfTable;
	T_aclPolicySwitch		aclPolicySwitch;
	T_fclBasicSetup			stFclBasicSetup;
	T_routeBasicSetup		routerBasicSetup;
	T_routerNeighSetup		routerNeighSetup;
	T_routerHandoverSetup	routerHandoverSetup;
	T_routerMscSetup		routerMscSetup;
	T_vlanSwitch			vlanSwitch;
	T_vlanSetupList			vlanSetupList;
	T_configInforce			configInforce;
	T_capWapSetup			stCapWapSetup;
	OMA_PERF_CFG_T			stPerfCfg[PERF_COUNT];
	OMA_ALARM_FILTER_CFG_T	stAlarmFilterCfg[ALARM_COUNT];
    AODV_NEIGHBOR_LIST_T    stAodvNeighbor;
} OMA_DATA_U;

typedef struct msConnectMsg_s
{
	int				iFlag;
#define CHECK_FLAG	0xcccccccc
	unsigned char	aucDeviceId[6];			/* �豸ID��6��16�����ַ� */
	unsigned char	aucSwVersion[32];		/* ����汾 */
	unsigned long	ulSlaveSetState;		/* ���豸����״̬ */
#define SLAVE_SET_STATE_SUCCESS	0
#define SLAVE_SET_STATE_FAILED	1
	unsigned long	ulSlaveInitFlag;		/* ���豸��ʼ����ʶ */
#define SLAVE_INIT_FLAG_INIT	1;
#define SLAVE_INIT_FLAG_UNINIT	0;
	int				iRsv[4];				/* Ԥ��λ */
} OMA_MS_CONNECT_MSG_T;

/************************************************************************/
/*---------------------------------ϵͳ��־------------------------------*/
/************************************************************************/
/* ��ϵͳ�� */
#define SUBSYS_WIFI             1
#define SUBSYS_OMA              2
#define SUBSYS_ROUTE            3
#define SUBSYS_DRIVER           4


/* ��ϵͳ��ȱʡģ��� */
#define MODULE_WIFI            ( SUBSYS_WIFI << 8)
#define MODULE_OMA             ( SUBSYS_OMA << 8)
#define MODULE_ROUTE           ( SUBSYS_ROUTE << 8)
#define MODULE_DRIVER          ( SUBSYS_DRIVER << 8)

#define OMA_MODULE_PLAT       	(MODULE_OMA+0)

#define OMA_MODULE_SYS        	(MODULE_OMA+10)
#define OMA_MODULE_OPERATION   	(MODULE_OMA+11)
#define OMA_MODULE_WIFI       	(MODULE_OMA+12)
#define OMA_MODULE_AP        	(MODULE_OMA+13)
#define OMA_MODULE_ADHOC      	(MODULE_OMA+14)
#define OMA_MODULE_ROUTE      	(MODULE_OMA+15)
#define OMA_MODULE_ETH        	(MODULE_OMA+16)
#define OMA_MODULE_VLAN       	(MODULE_OMA+17)
#define OMA_MODULE_ACL        	(MODULE_OMA+18)
#define OMA_MODULE_FLOW       	(MODULE_OMA+19)
#define OMA_MODULE_STA       	(MODULE_OMA+20)

#define OMA_MODULE_ALARM      	(MODULE_OMA+30)
#define OMA_MODULE_PERF       	(MODULE_OMA+31)
#define OMA_MODULE_TOPO       	(MODULE_OMA+32)
#define OMA_MODULE_SOFT       	(MODULE_OMA+33)


/* LOG ����*/
#define LOG_TYPE_SECURITY        1
#define LOG_TYPE_DEVICE          2
#define LOG_TYPE_OPERATION       3
#define LOG_TYPE_SERVICE         4


/* LOG���� */
#define  BB_LOG_LEVEL_FATAL       4
#define  BB_LOG_LEVEL_ERROR       3
#define  BB_LOG_LEVEL_WARNING     2
#define  BB_LOG_LEVEL_INFO        1
#define  BB_LOG_LEVEL_DETAIL      0


/* LOG�ļ���� */
#define LOG_FILE_CUR            1    /*  log since this reboot */
#define LOG_FILE_ALL            2     /* log since power on */

#define LOG_MAX_ITEM_MSG_LENGTH 128
typedef struct rc_log_item_tag
{
	unsigned short usModuleId;
	unsigned short usType;
	unsigned short usLevel;
	unsigned char rsv1[2];
	struct timeval  tsCurTime;      /* time based on 1970-01-01 00:00:00 */
	struct timeval  tsRsvTime;      /* no use now */

	unsigned char rsv2[32];
	char acMessage[LOG_MAX_ITEM_MSG_LENGTH];
} rc_log_item_t;

typedef rc_log_item_t LOG_ITEM_T;

/* hc add for reboot log 2009-11-4 */
#define MAX_REBOOT_RECORD_NUM   16
#define REBOOT_HARDRESET        0    
#define REBOOT_SOFTRESET        1
#define REBOOT_SHOW_GET_ACCESS  "/tmp/showreboot.flag"
#define REBOOT_USER_FILE        "/proc/userfile"
#define REBOOT_RECORD_FILE      "/tmp/rebootrecord"
#define REBOOT_SOFTRESET_FILE   "/tmp/softreboot"
#define REBOOT_HARDRESET_FILE   "/tmp/hardreboot"

typedef struct reboot_Info

{
    int         iType; /* 0 -- hardReset; 1 -- softReset */
    int         iUptime;
    struct tm   stCurTime; 
    char        acReason[128];
}T_RebootInfo;

typedef struct usr_file_info
{
    int             iCheck;
#define REBOOT_CHECK 0x01010101
    unsigned char   ucRebootCount;   /*  */
    unsigned char   ucRecordPos;
    int             iFlag;          /* 0 -- hardReset; 1 -- softReset */  
    T_RebootInfo    stRebootInfo[MAX_REBOOT_RECORD_NUM];
    //int             iResv[256];
}USR_FILE_INFO_T;


#define MAX_LOG_ITEM_EVERY_PAGE 50
typedef struct log_entry_s
{
	unsigned long ulMaxLogItem;						/* �ܹ���־�� */
	unsigned long ulCurPage;						/* ��ǰ��־��ŵ�ҳ�� */
	LOG_ITEM_T stLogItem[MAX_LOG_ITEM_EVERY_PAGE];	/* ÿ�λ�ȡ����־��ÿ�����50�� */
} LOG_ENTRY_T;

/************************************************************************/
/*                         ��ҳ���ݶ���                                 */
/************************************************************************/

#define MAX_ETHER_NUM		2
#define ETH_MAC_LEN		6

#define ETHE_SPEED_10M		0
#define ETHE_SPEED_100M		1
#define ETHE_SPEED_AUTO		2

typedef struct
{
	unsigned long  ulEnetPortNum;           /* ��̫���˿ں� */     
	char           acEnetPortName[16];      /* ��̫���˿����� */
	char           acEnetPortDesc[32];      /* ��̫���˿����� */
	unsigned long  ulEnetAdminState;        /* ��̫������״̬ */
//   unsigned long ulEnetOpState;           /* ��̫��������״̬ */
	unsigned long  ulLinkState;
    unsigned long  ulSpeed;
#define SPEED_100BASETX_FD	1
#define SPEED_100BASETX_HD	2
#define SPEED_10BASET_FD	3
#define SPEED_10BASET_HD	4
#define SPEED_DISABLE		5
#define NO_LINK	0
#define LINK_OK	1
	unsigned long ulEnetType;
	unsigned long ulEnetMtu;
	unsigned char aucMac[ETH_MAC_LEN];       /* ��̫����MAC  */
	unsigned char aucRsv0[2];

} ETHER_ATTRIBUTE_T;

#define MAX_WIFI_NUM	4
typedef struct
{
	unsigned long ulRadioState;
	unsigned long ulRadioMode;
	unsigned long ulCurRadioMode;           /* ��ǰģʽ */
	unsigned long ulRadioChannelConfig;
	unsigned long ulRadioChannel;
	char acRadioRates[64];
	unsigned long ulMaxTxPwr;
	unsigned long ulTxPwrDown;
	unsigned long ulTxPwrDownScale;         /* ����˥����Χ */
	unsigned long ulAnteGain;               /* �������� */
	unsigned long ulMaxStaNum;              /* �����������ն��� */
	unsigned long ulAutoTxPwr;
	unsigned long ulRadioRole;
} WIFI_ATTRIBUTE_T;

typedef struct
{
	unsigned char aucIp[4];
	struct tm sysDateTime;				/* ������ʱ���� */
	unsigned long ulSysUpTime;			/* ϵͳ�ϵ絽���ڵ����� */
	ETHER_ATTRIBUTE_T stEthAttribute[MAX_ETHER_NUM];
	unsigned long ulRadioNum;			/* ����ģ����Ŀ */
	WIFI_ATTRIBUTE_T stRadioAttribute[MAX_WIFI_NUM];
	unsigned long ulDownlinkDataFlow;	/* ����ҵ������ */
	unsigned long ulUplinkDataFlow;		/* ����ҵ������ */
	unsigned long ulNeighNum;			/* �ھ��� */

	char acWtpName[32];                  /* �豸���� */
	char acWtpDeviceType[32];			 /* �豸���� */
	char acWtpSN[32];					 /* �豸���к� */
	char acRsv[2];
	char acWtpSwVersion[32];             /* ����汾 */
	char acWtpHwVersion[32];             /* Ӳ���汾 */
	char acWtpCpuInfo[32];				 /* CPU��Ϣ */
	char acWtpMemInfo[32];				 /* �ڴ���Ϣ */
	char acWtpManufacturer[32];			 /* �豸������ */
	char acWtpSwDeveloper[32];			 /* ��������� */
	unsigned long ulCountryCode;		 /* ������ */
	unsigned long ulWtpMode;             /* �豸����ģʽ */
	unsigned long ulWtpStatus;           /* ����״̬ */

#define WTP_STATUS_INITING      1       /* ���ڳ�ʼ�� */
#define WTP_STATUS_WORKING      2       /* ���������� */
#define WTP_STATUS_OFFLINE      3       /* ����ֹͣ���� */

	unsigned long ulIpOrigin;
	unsigned char uacWtpNetMask[4];
	unsigned char uacWtpDftGw[4];
	unsigned char uacDnsServer1[4];
	unsigned char uacDnsServer2[4];
	unsigned char uacWtpMacAddr[6];
    unsigned char ucAcLinkState;
	unsigned char uacPadding[1];
} OMA_SYS_INFO_T;

typedef struct
{
	unsigned long ulKeeperSwitch;		/* �Ƿ��ػ� */
	unsigned char aucKeeperIp0[4];		/* �ػ���������ip��ַ���豸��������ping�õ�ַ */
	unsigned char aucKeeperIp1[4];		/* �ػ���������ip��ַ���豸��������ping�õ�ַ */
	unsigned long ulKeeperState;		/* �ػ�״̬ */
} OMA_KEEPER_INFO_T;

typedef struct
{
	unsigned long ulWifiId;				/* ������������ID */
	unsigned long ulVapId;				/* VAP���� */
	char          acSsid[32 + 1];		/* SSID */
	char          resv[3];
	unsigned long ulChannel;			/* �ŵ� */
	unsigned long ulAbgMode;			/* a/b/gģʽ */
	unsigned long ulSecurityMode;		/* ��ȫģʽ */
	unsigned long ulAdhocMode;			/* �ش�ģʽ���ն�/����/�ش��� */
#define ADHOC_MODE_ADHOC	0
#define ADHOC_MODE_STA		1
#define ADHOC_MODE_ACCESS	2
} OMA_ADHOC_INFO;

typedef struct
{
	unsigned long ulValidAdhocNum;
	OMA_ADHOC_INFO stAdhocInfo[adhocVapTable_SIZE];
} OMA_ADHOC_INFO_ENTRY;

typedef struct
{
	unsigned long ulWifiId;				/* ������������ID */
	unsigned long ulVapId;				/* VAP���� */
	char          acSsid[32 + 1];	    /* SSID */
	char          resv[3];
	unsigned long ulRunState;			/* ״̬up/down */
	unsigned long ulChannel;			/* �ŵ� */
	unsigned long ulAbgMode;			/* a/b/gģʽ */
	unsigned long ulSecurityMode;		/* ��ȫģʽ */
	unsigned long ulCurUserNum;			/* ��ǰ�û���Ŀ */
} OMA_AP_INFO;

typedef struct
{
	unsigned long ulValidApNum;
	OMA_AP_INFO	stApInfo[apVapTable_SIZE];
} OMA_AP_INFO_ENTRY;

typedef struct
{
	unsigned long ulWifiId;				/* ������������ID */
	unsigned long ulVapId;				/* VAP���� */
	char acSsid[32 + 1];				/* SSID */
	char acBssid[32 + 1];				/* BSSID */
	unsigned long ulChannel;			/* �ŵ� */
	unsigned long ulAbgMode;			/* a/b/gģʽ */
	unsigned long ulSecurityMode;		/* ��ȫģʽ */
	unsigned long ulRssi;				/* RSSI */
} OMA_STA_INFO;

typedef struct
{
	unsigned long ulValidStaNum;
	OMA_STA_INFO  stStaInfo[STA_VAP_TABLE_SIZE];
} OMA_STA_INFO_ENTRY;

typedef struct
{
    unsigned long ulMemTotal;       /* memory total */
    unsigned long ulUsedMem;        /* memory used */
    unsigned long ulfreeMem;        /* memory freed */
    unsigned long ulStorageTotal;   /* filesys storage total */
    unsigned long ulStorageUsed;    /* filesys storage used */
    unsigned long ulStorageFree;    /* filesys storage free */
} STORAGE_INFO_T;

typedef struct
{
    double dTper;                   /* device temperature */
    unsigned char ucCpuRate;        /* cpu used rate */
    char   mac[32];                 /* manage vlan mac address */
	unsigned long ulWorkMode;       /* work mode */
#define SYS_WORK_MODE_BRIDGE    0
#define SYS_WORK_MODE_ROUTER    1
#define SYS_WORK_MODE_CENTER    2   /* ����ת�� */
} OTHER_INFO_T;

typedef struct
{
	OMA_SYS_INFO_T			stSysInfo;
	OMA_KEEPER_INFO_T		stKeeperInfo;
	OMA_ADHOC_INFO_ENTRY	stAdhocInfoEntry;
	OMA_AP_INFO_ENTRY		stApInfoEntry;
	OMA_STA_INFO_ENTRY		stStaInfoEntry;
	STORAGE_INFO_T          stStorageInfo;
	OTHER_INFO_T            stOtherInfo;
} SYS_INFO_T;

typedef struct
{
	unsigned long ulReplaceCV;
	char acfName[128];
} UPDATA_SOFT_VERSION_T;

/************************************************************************/
/*                           ��̬��Ϣ                                   */
/************************************************************************/
typedef struct
{
	char acApSsid[32];					/* VAP SSID */
	unsigned long ulAccessTimes;		/* AP�������û����Ӵ��� */
	unsigned long ulAccessFailTimes;	/* AP�������û�����ʧ�ܴ��� */
	unsigned long ulAccessRetryTimes;	/* AP�������û������Ӵ��� */
	unsigned long ulAccessDenyTimes;	/* AP�������û����Ӿܾ����� */
	unsigned long ulAccessOutTimes1;	/* ���û��뿪�����½�����Ĵ��� */
	unsigned long ulAccessOutTimes2;	/* ��AP������������½�����Ĵ��� */
	unsigned long ulAccessOutTimes3;	/* ���쳣��������½�����Ĵ��� */
	unsigned long ulAccessOutTimes4;	/* ����ԭ���½�����Ĵ��� */
} OMA_GLOBAL_STAT_INFO;

typedef struct
{
	unsigned long ulValidVapNum;
	OMA_GLOBAL_STAT_INFO astGlobalStat[apVapTable_SIZE];
} OMA_GLOBAL_STAT_ENTRY;

/* 2011-09-02 fengjing modify,�����ֶ����ʹ�UINT32->UINT64 */
/* �����ڷ���/�����ֽ���������/�������ݰ��ֽ��� */
typedef struct
{
	/* 2012-05-18 �޸ģ�����VAP ID�Լ�Wifi ID�ֶ� */
    unsigned long  ulVapId;   				 /* VAP ID��ȡֵ0-15 */
    unsigned long  ulWifiId;                 /* Wifi ID��ȡֵ0-3�������豸ID��Ӧ�ñȽ�׼ȷ���ǿ�ID����Ŀǰģ�������豸ID�뿨IDһ�¡�*/

	char acApSsid[32];

	/* 2011-04-16 fengjing add */
	unsigned long ulTxPkts;            /*���͵İ��������߽ӿڽ��յ����ݰ��͹�����ư������������ش�*/
	unsigned long ulRxPkts;            /*���յİ��������߽ӿڽ��յ����ݰ��͹�����ư������������ش�*/
	unsigned long long ulTxOctets;         /*���͵��ֽ��������߽ӿڷ��͵İ��������ֽں͹�������ֽ������������ش�*/
	unsigned long long ulRxOctets;        /*���յ��ֽ��������߽ӿڽ��յİ��������ֽں͹�������ֽ������������ش�*/
	
	unsigned long ulRecvFrameNum;            /* ���յ�����֡�� */
	unsigned long ulSendFrameNum;            /* ���͵�����֡�� */

	/* 2010/07/04 fengjing cmcc test */         
	unsigned long ulRecvPackets;            /* ���յ����ݰ��� */
	unsigned long ulSendPackets;            /* ���͵����ݰ��� */

	/* 2010/07/01 fengjing cmcc test */
	unsigned long ulRecvCtrlFrame;           /* ���յĿ���֡�� */ /*��ֻ����ACK֡��PS-POLL֡���ʵȼ���PS-POLL+�ɹ����͵���֡��Ŀ��������֡�����֡*/
	unsigned long ulSendCtrlFrame;           /* ���͵Ŀ���֡�� */ /*��ֻ����ACK֡���ʵȽ��ڳɹ����յ���֡��Ŀ��������֡�����֡*/
	
	unsigned long long ulRecvBytes;               /* ���յ����ݰ��ֽ��� */
	unsigned long ulRecvErrFrame;            /* �û���(���߲�)���յĴ�֡��*/
	unsigned long ulRecvDiscardFrame;        /* �û���(���߲�)���յĶ�֡��*/
	unsigned long ulRecvRetransFrame;        /* �û���(���߲�)���յ��ش�֡��*/

	unsigned long long ulSendBytes;                /* ���͵����ݰ��ֽ��� */
	unsigned long ulSendErrFrame;             /*  */
	unsigned long ulSendDiscardFrame;
	unsigned long ulSendRetransFrame;         /* �����ŵ������ش���֡�� */ /*�ش�֡��������֡�����֡*/
	/* 2010/07/01 fengjing cmcc test */
	unsigned long ulSendRetransDataPackets;   /* �����ش������ݰ��� */   /*�ش�֡��ָֻ����֡*/
	
	/* hc modified for cmcc test 2009-7-18 */
	unsigned long apUplinkUpdownTimes;       /* Ap �������ж˿�updown ���� */
	unsigned long apDownlinkUpdownTimes;     /* Ap �������ж˿�updown ���� */

	unsigned long ulResouceUsed;
	unsigned long ulBeaconFrameSendNum;
	unsigned long ulARFrameRecvNum;
	unsigned long ulARFrameSendNum;
	unsigned long ulRRFrameRecvNum;
	unsigned long ulRRFrameSendNum;
	unsigned long ulPRFrameRecvNum;
	unsigned long ulPRFrameSendNum;
	unsigned long ulAuthFrameRecvNum;
	unsigned long ulAuthFrameSendNum;
	unsigned long ulDeauthFrameRecvNum;
	unsigned long ulDeauthFrameSendNum;
	unsigned long ulDisassFrameRecvNum;
	unsigned long ulDisassFrameSendNum;
	unsigned long ulOtherFrameRecvNum;
	unsigned long ulOtherFrameSendNum;

	/* 2010-4-10 pyy ����*/
	unsigned long ulRecvFrameUCast;	/* ���е���֡��*/
	unsigned long ulSendFrameUCast;	/* ���е���֡��*/
	unsigned long ulRecvFrameMCast;	/* ���зǵ�����֡��*/
	unsigned long ulSendFrameMCast;	/* ���зǵ�����֡��*/
	
	long		  apSnrBSSIDAverageSignalStrength; 	 /*AP  ���ź�ƽ��ǿ��*/
	unsigned long apSnrBSSIDSignalPkts;				 /*AP  �ܵ��������  */
	unsigned long apSnrBSSIDTxSignalPkts;			 /*AP  ������������*/    /* ���͵Ĺ���֡�� */
	unsigned long apSnrBSSIDRxSignalPkts;			 /*AP  ���յ��������*/    /* ���յĹ���֡�� */
	long		  apSnrBSSIDHighestRxSignalStrength; /*AP  ������ź�ǿ��*/
	long 		  apSnrBSSIDLowestRxSignalStrength;  /*AP  ������ź�ǿ��*/
	unsigned long apChStatsChannel;					 /*AP  ��ǰռ�õ�Ƶ��*/
	unsigned long apChStatsNumStations;				 /*ʹ�ø��ŵ����ն���*/	

	/* 2010/08/06 fengjing cmcc test */
	unsigned long ulSendFragRate; /* bps */ /* ֡�ķֶ����� */
}__attribute__ ((packed)) OMA_AIR_STATS_INFO ;

typedef struct
{
	unsigned long ulValidVapNum;
	OMA_AIR_STATS_INFO astAirStat[apVapTable_SIZE];
} OMA_AIR_STATS_ENTRY;
/* BEGIN: Added by liyoukun, 2014/1/16   PN:�û������Ϳ�ʼͳ���û��ı������� */
/* �û���������ͳ�ƣ��������ʱ����������������� */
typedef struct 
{
    unsigned char aucUserMac[6];
    
    unsigned long uiLocalRxPkts;
    unsigned long uiLocalTxPkts;
    unsigned long uiLocalRxGigaWords;
    unsigned long uiLocalRxBytes;
    unsigned long uiLocalTxGigaWords;
    unsigned long uiLocalTxBytes;  

    unsigned long uiInternetRxPkts;
    unsigned long uiInternetTxPkts;
    unsigned long uiInternetRxGigaWords;
    unsigned long uiInternetRxBytes;
    unsigned long uiInternetTxGigaWords;
    unsigned long uiInternetTxBytes;  

} OMA_USER_STATISTIC_INFO;

/* VAP��ͳ�ƽṹ�� */
typedef struct 
{
    unsigned long  ulVapId;   				 /* VAP ID��ȡֵ0-15 */
    unsigned long  ulWifiId;                 /* Wifi ID��ȡֵ0-3�������豸ID��Ӧ�ñȽ�׼ȷ���ǿ�ID����Ŀǰģ�������豸ID�뿨IDһ�¡�*/

	char acApSsid[32];
    
    unsigned long uiLocalRxPkts;
    unsigned long uiLocalTxPkts;
    unsigned long uiLocalRxGigaWords;
    unsigned long uiLocalRxBytes;
    unsigned long uiLocalTxGigaWords;
    unsigned long uiLocalTxBytes;  

    unsigned long uiInternetRxPkts;
    unsigned long uiInternetTxPkts;
    unsigned long uiInternetRxGigaWords;
    unsigned long uiInternetRxBytes;
    unsigned long uiInternetTxGigaWords;
    unsigned long uiInternetTxBytes;  

} OMA_VAP_STATISTIC_INFO;
/* vap��������ͳ�� */
typedef struct
{
	unsigned long ulValidVapNum;
	OMA_VAP_STATISTIC_INFO astVapFlowStat[apVapTable_SIZE];
} OMA_VAP_STATS_ENTRY;

/* �ϱ���agentʹ�õ��û�ͳ�ƽṹ�壬��TSģ���ȡ */
typedef struct
{
	OMA_USER_STATISTIC_INFO     astApEndUser;
} OMC_VAP_END_USER_LIST;

typedef struct apVapEndUserEntryExt_s 
{
	unsigned long	        ulValidNum;
	OMC_VAP_END_USER_LIST   astApEndUserList[MAX_ENDUSER_ENTRY_I];
} OMC_VAP_END_USER_ENTRY;

/* END:   Added by liyoukun, 2014/1/16   PN:�û������Ϳ�ʼͳ���û��ı������� */

/* 2011-09-03 fengjing add */
typedef struct
{
    /* BEGIN: Added by ������, 2013/2/4   PN:BUG #1738 ��ap93����ƽ̨����mibͳ�ơ����߽ӿ�ͳ�� ap�ϱ������� */
   // unsigned long  ulVapId;   		
    //unsigned long  ulWifiId; 
    unsigned long ulVapId;
    unsigned long ulWifiId;
    /* END:   Added by ������, 2013/2/4   PN:BUG #1738 ��ap93����ƽ̨����mibͳ�ơ����߽ӿ�ͳ�� ap�ϱ������� */
   char acApSsid[32];

	/* 2011-04-16 fengjing add */
	unsigned long ulTxPkts;            /*���͵İ��������߽ӿڽ��յ����ݰ��͹�����ư������������ش�*/
	unsigned long ulRxPkts;            /*���յİ��������߽ӿڽ��յ����ݰ��͹�����ư������������ش�*/

       unsigned long ulTxOctetsHigh;         /*���͵��ֽ��������߽ӿڷ��͵İ��������ֽں͹�������ֽ������������ش�*/     //��λ
       unsigned long ulTxOctetsLow;         /*���͵��ֽ��������߽ӿڷ��͵İ��������ֽں͹�������ֽ������������ش�*/     //��λ
       unsigned long ulRxOctetsHigh;        /*���յ��ֽ��������߽ӿڽ��յİ��������ֽں͹�������ֽ������������ش�*/     //��λ
	unsigned long ulRxOctetsLow;        /*���յ��ֽ��������߽ӿڽ��յİ��������ֽں͹�������ֽ������������ش�*/     //��λ
	
	unsigned long ulRecvFrameNum;            /* ���յ�����֡�� */
	unsigned long ulSendFrameNum;            /* ���͵�����֡�� */

	/* 2010/07/04 fengjing cmcc test */         
	unsigned long ulRecvPackets;            /* ���յ����ݰ��� */
	unsigned long ulSendPackets;            /* ���͵����ݰ��� */

	/* 2010/07/01 fengjing cmcc test */
	unsigned long ulRecvCtrlFrame;           /* ���յĿ���֡�� */ /*��ֻ����ACK֡��PS-POLL֡���ʵȼ���PS-POLL+�ɹ����͵���֡��Ŀ��������֡�����֡*/
	unsigned long ulSendCtrlFrame;           /* ���͵Ŀ���֡�� */ /*��ֻ����ACK֡���ʵȽ��ڳɹ����յ���֡��Ŀ��������֡�����֡*/

       unsigned long ulRecvBytesHigh;               /* ���յ����ݰ��ֽ��� */          //��λ
       unsigned long ulRecvBytesLow;               /* ���յ����ݰ��ֽ��� */          //��λ

	unsigned long ulRecvErrFrame;            /* �û���(���߲�)���յĴ�֡��*/
	unsigned long ulRecvDiscardFrame;        /* �û���(���߲�)���յĶ�֡��*/
	unsigned long ulRecvRetransFrame;        /* �û���(���߲�)���յ��ش�֡��*/

       unsigned long ulSendBytesHigh;                /* ���͵����ݰ��ֽ��� */      //��λ
       unsigned long ulSendBytesLow;                /* ���͵����ݰ��ֽ��� */       //��λ

	unsigned long ulSendErrFrame;             /*  */
	unsigned long ulSendDiscardFrame;
	unsigned long ulSendRetransFrame;         /* �����ŵ������ش���֡�� */ /*�ش�֡��������֡�����֡*/
	/* 2010/07/01 fengjing cmcc test */
	unsigned long ulSendRetransDataPackets;   /* �����ش������ݰ��� */   /*�ش�֡��ָֻ����֡*/
	
	/* hc modified for cmcc test 2009-7-18 */
	unsigned long apUplinkUpdownTimes;       /* Ap �������ж˿�updown ���� */
	unsigned long apDownlinkUpdownTimes;     /* Ap �������ж˿�updown ���� */

	unsigned long ulResouceUsed;
	unsigned long ulBeaconFrameSendNum;
	unsigned long ulARFrameRecvNum;
	unsigned long ulARFrameSendNum;
	unsigned long ulRRFrameRecvNum;
	unsigned long ulRRFrameSendNum;
	unsigned long ulPRFrameRecvNum;
	unsigned long ulPRFrameSendNum;
	unsigned long ulAuthFrameRecvNum;
	unsigned long ulAuthFrameSendNum;
	unsigned long ulDeauthFrameRecvNum;
	unsigned long ulDeauthFrameSendNum;
	unsigned long ulDisassFrameRecvNum;
	unsigned long ulDisassFrameSendNum;
	unsigned long ulOtherFrameRecvNum;
	unsigned long ulOtherFrameSendNum;

	/* 2010-4-10 pyy ����*/
	unsigned long ulRecvFrameUCast;	/* ���е���֡��*/
	unsigned long ulSendFrameUCast;	/* ���е���֡��*/
	unsigned long ulRecvFrameMCast;	/* ���зǵ�����֡��*/
	unsigned long ulSendFrameMCast;	/* ���зǵ�����֡��*/
	
	long		  apSnrBSSIDAverageSignalStrength; 	 /*AP  ���ź�ƽ��ǿ��*/
	unsigned long apSnrBSSIDSignalPkts;				 /*AP  �ܵ��������  */
	unsigned long apSnrBSSIDTxSignalPkts;			 /*AP  ������������*/    /* ���͵Ĺ���֡�� */
	unsigned long apSnrBSSIDRxSignalPkts;			 /*AP  ���յ��������*/    /* ���յĹ���֡�� */
	long		  apSnrBSSIDHighestRxSignalStrength; /*AP  ������ź�ǿ��*/
	long 		  apSnrBSSIDLowestRxSignalStrength;  /*AP  ������ź�ǿ��*/
	unsigned long apChStatsChannel;					 /*AP  ��ǰռ�õ�Ƶ��*/
	unsigned long apChStatsNumStations;				 /*ʹ�ø��ŵ����ն���*/	

	/* 2010/08/06 fengjing cmcc test */
	unsigned long ulSendFragRate; /* bps */ /* ֡�ķֶ����� */
} OMA_AIR_STATS_INFO_DRV;

typedef struct
{
	unsigned long ulValidVapNum;
	OMA_AIR_STATS_INFO_DRV astAirStat[apVapTable_SIZE];
} OMA_AIR_STATS_ENTRY_DRV;

/* 2010-4-11 pyy �����������ܲ�������: �����豸�ŵ������쳣ͳ��*/
typedef struct{

	/* 2010/07/01 fengjing cmcc test */
	unsigned long  ulRecvErrPackets;        /* ���յĴ������ݰ��� */

    unsigned long  ulRecvErrPhy;			/* �ŵ����յ���������� */
    unsigned long  ulRecvErrFcs;			/* �ŵ����յ�FCS MAC�����*/
    unsigned long  ulRecvErrMic;			/* �ŵ����յ�MIC MAC�����*/
    unsigned long  ulRecvErrDecrypt;		/* �ŵ����յĽ���ʧ�ܵ�MAC�����*/
    unsigned long  ulRecvErr;				/* �ŵ����յĴ�֡��*/

	unsigned long ulSnr;
    unsigned long ulChannelReelect;
	
	unsigned long ulSendFragRate;    /* ��Ƭ���� */			 
}OMA_IC_STAT_INFO_T;

typedef struct
{
	unsigned long ulValidVdevNum;
	OMA_IC_STAT_INFO_T	astIcStat[wifiDeviceTable_SIZE];		
}OMA_IC_STAT_ENTRY;

/* 2010-08-26 fengjing ����:��ȡ�ھ�AP��Ϣ */
typedef struct 
{
	struct ieee80211_scan_ssid ssid ;
	unsigned char bssid[6];
    unsigned char channel;
    unsigned char rssi;
    long          noise;
}NEIGHBOR_AP_INFO_T;

#define NEIGHBOR_AP_MAX   64
typedef struct
{ 
	/* 2011-04-09 fengjing modify */
#if 1
	unsigned long ulWifiId;
#endif
    unsigned char aucMac[6];            /*��AP MAC*/
    unsigned char aucMacPad[2];
    unsigned long ulChannel;            /*��AP CHANNEL*/
    unsigned long ulNeighborApNum ;
    NEIGHBOR_AP_INFO_T stNeighborApList[NEIGHBOR_AP_MAX] ;
}NEIGHBOR_AP_LIST_T ;

typedef struct
{
	unsigned long      ulValidVdevNum;
	NEIGHBOR_AP_LIST_T astNeighborAp[wifiDeviceTable_SIZE];
}OMA_NEIGHBOR_AP_ENTRY;

typedef struct
{
	unsigned long ulWifiId;
	NEIGHBOR_AP_INFO_T stNeighborAp;
}OMC_AP_NEIGHAP_T;

typedef struct
{
	unsigned long     ulValidNum;
	OMC_AP_NEIGHAP_T  astNeighAp[NEIGHBOR_AP_MAX];
}OMC_AP_NEIGHAP_LIST_T;

/* 2010/05/15 fengjing �й����Ų�������:����ͳ�� */
typedef struct
{
	char acApSsid[32];
	 /*
	����Ч�������û���·��֤ʧ�ܵĴ���
	��ʱ�������û���·��֤ʧ�ܵĴ���
	��ܾ��������û���·��֤ʧ�ܵĴ���
	����ԭ�����û���·��֤ʧ�ܵĴ���
	����Ч�������û�����ʧ�ܵĴ���
	��ʱ�������û�����ʧ�ܵĴ���
	��ܾ��������û�����ʧ�ܵĴ���
	����ԭ�����û�����ʧ�ܵĴ���
	�û��ع������Դ���
	�û��ع����ɹ���������ѡ��
	����Ч�������û��ع���ʧ�ܵĴ���
	��ʱ�������û��ع���ʧ�ܵĴ���
	��ܾ��������û��ع���ʧ�ܵĴ���
	����ԭ�����û��ع���ʧ�ܵĴ���
	�û������Դ���
	�û�����ɹ�����
	���������������û�����ʧ�ܵĴ���
	����Ч�������û�����ʧ�ܵĴ���
	��ʱ�������û�����ʧ�ܵĴ���
	��ܾ��������û�����ʧ�ܵĴ���
	����ԭ�����û�����ʧ�ܵĴ���
	����·��֤�Ĵ���
	���û��뿪�����½���·��֤�Ĵ���
	��AP������������½���·��֤�Ĵ���
	���쳣��������½���·��֤�Ĵ���
	����ԭ���½���·��֤�Ĵ���
	������Ĵ���
	���û��뿪�����½�����Ĵ���
	��AP������������½�����Ĵ���
	���쳣��������½�����Ĵ���
	����ԭ���½�����Ĵ���
	 */	  

	 // 2010/06/11 fengjing add
	 unsigned long ulAuthSucc;  //����SHARED��WEP����Ȩͨ��
	 
     unsigned long ulAuthFailInvalid;
     unsigned long ulAuthFailTimeout;
     unsigned long ulAuthFailDeny;  //���������������û�������ԭ��
     unsigned long ulAuthFailOther; 

     unsigned long ulAssocFailInvalid;
     unsigned long ulAssocFailTimeout;
     unsigned long ulAssocFailDeny;
     unsigned long ulAssocFailOther; 

     unsigned long ulReassocReq;
     unsigned long ulReassocSucc;
     unsigned long ulReassocFailInvalid;
     unsigned long ulReassocFailTimeout;
     unsigned long ulReassocFailDeny;
     unsigned long ulReassocFailOther; 

     unsigned long ulWapiReq;
     unsigned long ulWapiSucc;
     unsigned long ulWapiFailPwdError;
     unsigned long ulWapiFailInvalid;
     unsigned long ulWapiFailTimeout;
     unsigned long ulWapiFailDeny;
     unsigned long ulWapiFailOther; 

	 unsigned long ulWpaReq;
     unsigned long ulWpaSucc;       //WPA��֤�ɹ�
     unsigned long ulWpaFailPwdError;  //WPA-PSK��Կ������
     unsigned long ulWpaFailOther;

	 unsigned long ulAccessReq;   //�û�����������
     unsigned long ulAccessResp; // AP��Ӧ�û������������
     unsigned long ulAccessSucc;  //�û��ɹ��������������OPEN/SHARED��WPA��WAPI

     unsigned long ulDeauthTotal;
     unsigned long ulDeauthUserLeave;
     unsigned long ulDeauthCapable;
     unsigned long ulDeauthExcept;
     unsigned long ulDeauthOther;

     unsigned long ulDisassocTotal;
     unsigned long ulDisassocUserLeave;
     unsigned long ulDisassocCapable;
     unsigned long ulDisassocExcept;
     unsigned long ulDisassocOther;
}OMA_MGT_STAT_INFO_T;

typedef struct
{
	unsigned long ulValidVapNum;
	OMA_MGT_STAT_INFO_T astMgtStat[apVapTable_SIZE];
}OMA_MGT_STAT_ENTRY;

typedef struct
{
	char aucNeighMacAddr[6];
	char aucResv1[2];
	char aucNeighIpAddr[4];
	unsigned long ulNeighRecvPkt;
	unsigned long ulNeighSendPkt;
	unsigned long ulNeighRepeatSendPkt;
	unsigned long ulNeighWifiFailPkt;
	unsigned long ulNeighPer;
	char ucNeighAvgRssi;
	signed char ucNeighNoise;
	char aucResv2[2];
	signed long ulNeighSignal;
} OMA_ADHOC_LINK_STATS_INFO;

typedef struct
{
	unsigned long ulAdhocId;
	char          acAdhocSsid[32];
	unsigned long ulNeighNum;
#define NEIGH_LINK_MAX       16
	OMA_ADHOC_LINK_STATS_INFO stNeighLinkStats[NEIGH_LINK_MAX];
} OMA_ADHOC_LINK_STATS_ENTRY;

typedef struct
{
	unsigned long ulValidAdhocNum;
	OMA_ADHOC_LINK_STATS_ENTRY	astAdhocLinkStats[adhocVapTable_SIZE];
} OMC_ADHOC_LINK_STATS;

/* hc modified for cmcc test 2009-7-21 */
typedef struct
{
	 char acApSsid[32];
	
     unsigned long ulStaAssoc ;           /*��ǰ��AP�������ն���*/
     unsigned long ulAllStaTime ;         /*��AP�����������ն˵�����ʱ���ܺͣ���λ����*/
     unsigned long ulAssocTimes ;         /*�����ܴ���*/
     unsigned long ulAssocFailTimes ;     /*����ʧ���ܴ���*/ 
     unsigned long ulReassocTimes ;       /*���¹����ܴ���*/
     unsigned long ulDisassocBySta ;      /*ϵͳ���������ն��쳣�Ͽ����ӵ��ܴ���*/
     unsigned long ulAssocFailResource ;  /*���ڽ������Դ���޶����ܾ��������ܴ���*/
     unsigned long ulAssocFailBaseRate ;  /*���ն˲�֧�ֻ������ʼ�Ҫ����������ʶ�����ʧ�ܵ��ܴ���*/
     unsigned long ulAssocFailNotStd ;    /*�ɲ���802.11��׼�ƶ���Χ�ڵ�ԭ�������ʧ�ܵ��ܴ���*/
     unsigned long ulAssocFailOther ;     /*δ֪ԭ������¹���ʧ�ܵ��ܴ���*/
     unsigned long ulAssocFailNotShift ;  /*����֮ǰ�Ĺ����޷�ʶ����ת�ƶ��������¹���ʧ�ܵ��ܴ���*/

	/* 2011-03-23 fengjing add:�����ع���ʧ�ܴ��� */
	unsigned long ulReassocFailTimes ;
	unsigned long ulReassocFailResource ; 
	unsigned long ulReassocFailBaseRate ;         
	unsigned long ulReassocFailNotStd ;       
	unsigned long ulReassocFailOther ;        
	unsigned long ulReassocFailNotShift ;   	 
}BSS_STAT_ASSOC_T ;

typedef struct
{
	unsigned long ulValidVapNum;
	BSS_STAT_ASSOC_T astAssocStaStat[apVapTable_SIZE];
} OMA_ASSOC_STA_STAT_ENTRY;

/* 2011-09-02 fengjing modify,�����ֶ����ʹ�����UINT32->UINT64 */
typedef struct
{
	unsigned long ulDevId;
#if 0
	unsigned long ulRecvTotalFrame;	/* total frames of receive */
	unsigned long ulRecvRightFrame;	/* correct received frames */
	unsigned long ulRecvErrorFrame;	/* error received frames */
	unsigned long ulSendTotalFrame;	/* total frames of send */
	unsigned long ulRecvBytes;		/* total bytes of receive */
	unsigned long ulSendBytes;		/* total bytes of send */
#endif

    unsigned long ulRecvFrameNum;
    unsigned long long ulRecvBytes;
    unsigned long ulRecvUnicastFrameNum;
    unsigned long ulRecvMulticastFrameNum;
    unsigned long ulRecvFailFrameNum;
    unsigned long ulRecvDiscardFrameNum;

    unsigned long ulSendFrameNum;
    unsigned long long ulSendBytes;
    unsigned long ulSendUnicastFrameNum;
    unsigned long ulSendMulticastFrameNum;
    unsigned long ulSendFailFrameNum;
    unsigned long ulSendDiscardFrameNum;

    /* hc modified for cmcc test 2009-7-18 */
    unsigned long ulRecvBroadcastFrameNum;      /* ���߲���յĹ㲥֡�� */
    unsigned long ulRecvRetryFrameNum; 		    /* ���߲���յ��ش�֡�� */
    unsigned long ulSendBroadcastFrameNum;	    /* ���߲෢�͵Ĺ㲥֡�� */
    unsigned long ulSendRetryFrameNum;		    /* ���߲෢�͵��ش�֡�� */
    unsigned long ulDownNum;					/* ��̫���ڽ��ô��� */
    unsigned long ulUpNum;						/* ��̫�������ô��� */

	/* 2011-06-16 fengjing add */
	unsigned long long ulRecvDataBytes;   /* ���߲�������ݰ��ֽ��� */
	unsigned long long ulSendDataBytes;   /* ���߲෢�����ݰ��ֽ��� */

	/*add by zjj*/
	 unsigned long ulAPHeartBeatAvgTimeDelay;    /* AP���AC����������ƽ���ӳ�ʱ�䣬��λ���� */
	  unsigned long ulEchoReqCn;         /* AP���AC�������������ܰ��� */
	  unsigned long ulEchoRespLossCnt;   /* AP��ʧ��AC������Ӧ�ܰ��� */

}__attribute__ ((packed)) ETH_DEV_INFO_T;

typedef struct
{
	int iDevNum;					/* number of device */
	ETH_DEV_INFO_T astDevInfo[ENETINTERFACE_MAX_NUM];	/* device of info */
} NET_INFO_T;




typedef struct
{
	char acWtpName[64];                  /* �豸���� */
	char acWtpNEId[32];                  /* �豸��Ԫ���� */
	//unsigned long ulWtpDeviceType;	 /* �豸���� */
	char acDeviceType[32];               /* �豸���� */
	char acWtpSN[6];					 /* �豸���к� */
	char acRsv[2];

	char acWtpLocation[64];              /* �豸λ�� */
	char acWtpSwName[64];                /* ������� */
	char acWtpSwVersion[32];             /* ����汾 */
	char acWtpHwVersion[32];             /* Ӳ���汾 */
	char acWtpCpuInfo[128];				 /* CPU��Ϣ */
	char acWtpMemInfo[128];				 /* �ڴ���Ϣ */
	char acWtpManufacturer[32];			 /* �豸������ */
	char acWtpSwDeveloper[32];			 /* ��������� */
	unsigned long ulCountryCode;		 /* ������ */
	unsigned long ulWtpMode;             /* �豸����ģʽ */
	unsigned long ulWtpStatus;           /* ����״̬ */
    unsigned long ulWtpMoniMode;         /* Ap  ����ģʽ */
	unsigned char aucWtpIpAddr[4];		 /* �豸IP��ַ */
	unsigned long ulIpOrigin;
	unsigned char uacWtpNetMask[4];
	unsigned char uacWtpDftGw[4];
	unsigned char uacDnsServer1[4];
	unsigned char uacDnsServer2[4];
	unsigned char uacWtpMacAddr[6];
	unsigned char uacPadding[2];
	unsigned long ulWtpTime;			 /* �豸ʱ�� */
	unsigned long ulWtpUpTime;           /* �豸����ʱ�� */
    unsigned long ulWtpConnTime;         /* �豸����ʱ�� */
	
	unsigned long ulWtpMemoryTotal;      /* �ڴ��ܴ�С */
	unsigned long ulWtpMemoryUsed;       /* ��ǰ�ڴ�ʹ���� */
	unsigned long ulWtpMemoryUsedMax;    /* ����ڴ�ʹ���� */
	unsigned long ulWtpMemeoryUsedAverage; /* ƽ���ڴ�ʹ���� */
	unsigned long ulWtpCpuUsed;          /* ��ǰCPU  ʹ���� */
    unsigned long ulWtpCpuUsedMax;       /* ���CPU  ʹ���� */
    unsigned long ulWtpCpuUsedAverage;   /* ƽ��CPU  ʹ���� */
	unsigned long ulWtpFlashTotal;       /* �ļ�ϵͳ���� */
	unsigned long ulWtpFlashRemain;      /* �ļ�ϵͳʣ��ռ� */
	int           iWtpTemperature;		 /* �豸�����¶� */
	ETHER_ATTRIBUTE_T stEthAttribute[MAX_ETHER_NUM];
	unsigned long ulRadioNum;
	WIFI_ATTRIBUTE_T stRadioAttribute[MAX_WIFI_NUM];
#if 0 //removed by leijie 2009-04-29
	OMA_ADHOC_INFO_ENTRY	stAdhocInfoEntry;
	OMA_AP_INFO_ENTRY		stApInfoEntry;
#endif
	char aucResv[32];
} DEVICE_ABSTRACT_INFO_T;

/*DEVICE_ABSTRACT_INFO_T�ļ򻯽ṹ*/
typedef struct
{
	char acWtpCpuInfo[32];				 /* CPU��Ϣ */
	char acWtpMemInfo[32];				 /* �ڴ���Ϣ */
	
	unsigned long ulWtpTime;                           /* �豸ʱ�� */
	unsigned long ulWtpUpTime;           /* �豸����ʱ�� */
	unsigned long ulWtpConnTime;         /* �豸����ʱ�� */

	unsigned long ulWtpMemoryTotal;      /* �ڴ��ܴ�С */
	unsigned long ulWtpMemoryUsed;       /* ��ǰ�ڴ�ʹ���� */
	unsigned long ulWtpMemoryUsedMax;    /* ����ڴ�ʹ���� */
	unsigned long ulWtpMemeoryUsedAverage; /* ƽ���ڴ�ʹ���� */
	unsigned long ulWtpCpuUsed;          /* ��ǰCPU  ʹ���� */
	unsigned long ulWtpCpuUsedMax;       /* ���CPU  ʹ���� */
	unsigned long ulWtpCpuUsedAverage;   /* ƽ��CPU  ʹ���� */
	unsigned long ulWtpFlashTotal;       /* �ļ�ϵͳ���� */
	unsigned long ulWtpFlashRemain;      /* �ļ�ϵͳʣ��ռ� */
	int           iWtpTemperature;                 /* �豸�����¶� */

	ETHER_ATTRIBUTE_T stEthAttribute[MAX_ETHER_NUM];

	unsigned long ulRadioNum;
	WIFI_ATTRIBUTE_T stRadioAttribute[MAX_WIFI_NUM];
} AP_ABSTRACT_INFO_T;

/* hc modified for cmcc test 2009-7-28 */
typedef struct cpu_mem_sample
{
    unsigned long ulCpuSample;	
    unsigned long ulCpuSampleOld;		
    unsigned long ulCpuSampleRate;			
    unsigned long ulCpuSamplePos;
    unsigned char ucCpuResample;

    unsigned long ulMemSample;			
    unsigned long ulMemSampleOld;		
    unsigned long ulMemSampleRate;		
    unsigned long ulMemSamplePos;
    unsigned char ucMemResample;
}CPU_MEM_SAMPLE_T;

 
#define AGENT_DATA_UPDATE_ID_MIN	1
#define AGENT_DATA_UPDATE_ID_MAX	64

#define AGENT_TBL_INFO_NUM_MAX	64	/* AP�ϵ�����ϵ������*/

/* AC�·����������ݰ汾���뱾�ص�ǰ�������ݰ汾����ͬ, ����Ҫͬ��*/
#define AP_AGENT_DATAVER_SAME		0x11	


/* AP�������������ļ� �ṹ*/
typedef struct
{
	unsigned long ulWtpId;			/* AP ID */
	unsigned long ulCountryCode;	/* ������*/
	unsigned long ulDeviceType;	    /* �豸����*/
	unsigned char aucDeviceId[6];	/* �豸���к�*/
	unsigned char aucRsv0[2];		/* ����ֽ�*/

	unsigned long ulIpOrigin;		/* IP��ַ���䷽ʽ*/
	unsigned char aucIpAddr[4];		/* IP��ַ*/
	unsigned char aucNetMask[4];	/* ����*/
	unsigned char aucDefaultGw[4];	/* ȱʡ����*/
	unsigned char aucPrimaryDns[4];	/* ��DNS������ */
	unsigned char aucSecondaryDns[4];	/* ��DNS������ */

	char			acSecret[32];		/* AC��������*/
	unsigned long ulDisType;			/* AC���ַ�ʽ*/
	char		  	acAcURL[64];		/* AC URL */
}T_deviceBasicCfg;

/* ���ݸ��¿���ͷ*/
typedef struct
{
	unsigned long 		ulDeviceType;		/* �豸����*/
	unsigned char		aucDeviceId[6];		/* �豸���к�*/
	unsigned char		aucResv[2];			/* ����ֽ�*/
	char				acCfgModVer[16];	/* ��Ϣģ�Ͱ汾��*/
	unsigned long 		ulCfgVer;			/* �������ݰ汾��*/
	unsigned long 		ulValidNum;			/* ��������Ķ�����Ŀ*/
	unsigned long 		aulHandle[AGENT_DATA_UPDATE_ID_MAX];	/* ������������ʶ�б�*/
} T_agentDataUpdateHeader;

typedef struct
{
	unsigned long 		ulValid;		/* �Ƿ���Ч*/
	unsigned long		ulId;		    /* ��ʶ*/
    	void		*pfDataGen;        	/* capwap������֯�������ݵĺ���ָ��*/
    	void		*pfDataUpdate;		/* oma���ڴ������ݵĺ���ָ��*/
} T_agentDataUpdateHandle;

typedef struct
{
	unsigned long	ulValid;			/* �Ƿ���Ч*/
	char			acTblName[32];	    /* ��ϵ����*/
} T_agentApTotalCfgTblInfo;

typedef struct
{
	unsigned long ulModeSetFlag;
	unsigned long ulChannelSetFlag;
	unsigned long ul11nCwmModeSetFlag;
	unsigned long ul11nShortGiSetFlag;
	unsigned long ul11nAmpduFlag;
	unsigned long ul11nAmsduFlag;
	unsigned long ul11nTxChainMaskFlag;
	unsigned long ul11nRxChainMaskFlag;
	unsigned long ul11nPurenSetFlag;
} T_WIFIDEVICEPARASETFLAG;

typedef unsigned long (*AGENT_DATA_GEN_HANDLE)(T_configFileData	*pstData);
typedef unsigned long (*AGENT_DATA_UPDATE_HANDLE)(T_configFileData	*pstData);

#define AP_AGENT_BASIC_CONFIG_FILE "/tmp/basiccfg.dat"		/* AP�������������ļ� */

#define AP_AGENT_TIMERID_CHECK_ACLINKSTATE		TIMER0		/* ���AP-AC��·״̬��ʱ��*/
#define AP_AGENT_TIMERID_SEND_OMASTART			TIMER1		/* ����AP OMA������Ϣ��ʱ��*/
#define AP_AGENT_TIMERID_SEND_HEARTBEART			TIMER2		/* �����ϱ���ʱ��*/
#define AP_AGENT_TIMERID_CHECK_ALARM				TIMER3		/* ���澯/�¼���ʱ��*/
#define AP_AGENT_TIMERID_CHECK_PERF				TIMER4		/* ������ܶ�ʱ��*/
#define AP_AGENT_TIMERID_DELAY_SWUPGRADE		TIMER5		/* �����汾�������ض�ʱ��*/
#define AP_AGENT_TIMERID_CHECK_DHCPSTATE			TIMER6		/* ���DHCP״̬��ʱ��*/
#define AP_AGENT_TIMERID_CHECK_DHCPIPCHG		TIMER7		/* �����Լ��DHCP��Լ���*/
#define AP_AGENT_TIMERID_TIMERHANDLE				TIMER8
#define AP_AGENT_TIMERID_COLLECTPARAM_GET		TIMER9		/*��ȡ���ܲɼ�����*/
#define AP_AGENT_TIMERID_COLLECT_NORMAL			TIMER10		/*����ɼ�*/
#define AP_AGENT_TIMERID_COLLECT_RT				TIMER11		/*ʵʱ�ɼ�*/

/* BEGIN: Added by zhoushouya, 2012/5/21   PN:986 */
#define AP_AGENT_TIMERID_CHECK_L2WD				TIMER12 /* ���l2wdģ���Ƿ��powerap������һ�� */
/* END:   Added by zhoushouya, 2012/5/21 */

/* BEGIN: Added by lvjianlin, 2013/3/20   PN:BUG #2135 ��IXP������������������������������AC�����󣬲������㷢�ֵ�AP�޷����� */
#define AP_AGENT_TIMERID_AP_IP_CHANGE           TIMER13
/* END:   Added by lvjianlin, 2013/3/20   PN:BUG #2135 ��IXP������������������������������AC�����󣬲������㷢�ֵ�AP�޷����� */

/* BEGIN: Added by lvjianlin, 2013/6/19   PN:lte-fi  GPS ALARM  LOCAL PORTAL */
//#define AP_AGENT_TIMERID_LTE_CHECK_GPIO         TIMER14     /* ���GPIO״̬ */
//#define AP_AGENT_TIMERID_LTE_REBOOT_AP          TIMER15     /* AP������ʱ�� */
#define AP_AGENT_TIMERID_CHECK_GPIO				TIMER14     /* BEGIN: Added by ����, 2013/6/19   PN: 5s�Ӽ��GPIO��״̬��ʱ�� */
#define AP_AGENT_TIMERID_CHECK_BATTERY			TIMER15     /* BEGIN: Added by ����, 2013/6/19   PN: 5s�Ӽ��GPIO��״̬��ʱ�� */
#define AP_AGENT_TIMERID_BATTERY_ALARM_CLEAR    TIMER16     
/* END:   Added by lvjianlin, 2013/6/19   PN:lte-fi  GPS ALARM  LOCAL PORTAL */

/* BEGIN: Added by lvjianlin, 2013/8/6   PN:lte-fi updatedata bug and report time */
#define AP_AGENT_TIMERID_REPORT_BSSID           TIMER17
/* END:   Added by lvjianlin, 2013/8/6   PN:lte-fi updatedata bug and report time */


/* BEGIN: Added by liyoukun, 2013/12/3   PN:task#0  GPS��Ϣ�ϱ��޸ģ�����ͨ��echo�ϱ�AC�����ϱ���GPS������ */
#define AP_AGENT_TIMERID_REPORT_GPSINFO           TIMER18
/* END:   Added by liyoukun, 2013/12/3   PN:task#0  GPS��Ϣ�ϱ��޸ģ�����ͨ��echo�ϱ�AC�����ϱ���GPS������ */

/* BEGIN: Added by liyoukun, 2014/1/20   PN:���lte����ģʽ���л����ϱ��澯 */
#define AP_AGENT_TIMERID_CHECK_LTENETTYPE       TIMER19
/* END:   Added by liyoukun, 2014/1/20   PN:���lte����ģʽ���л����ϱ��澯 */
/* ��ʱ����Ϣ*/
#define AP_AGENT_TIMEOUT_CHECK_ACLINKSTATE		EV_TIMER0    /*  ���AP-AC��·״̬��ʱ����ʱ��Ϣ*/
#define AP_AGENT_TIMEOUT_SEND_OMASTART			EV_TIMER1
#define AP_AGENT_TIMEOUT_SEND_HEARTBEART		EV_TIMER2
#define AP_AGENT_TIMEOUT_CHECK_ALARM			EV_TIMER3
#define AP_AGENT_TIMEOUT_CHECK_PERF				EV_TIMER4
#define AP_AGENT_TIMEOUT_DELAY_SWUPGRADE		EV_TIMER5
#define AP_AGENT_TIMEOUT_CHECK_DHCPSTATE		EV_TIMER6	/* ���DHCP״̬��ʱ��*/
#define AP_AGENT_TIMEOUT_CHECK_DHCPIPCHG		EV_TIMER7	
#define AP_AGENT_TIMEOUT_TIMERHANDLE			EV_TIMER8
#define AP_AGENT_TIMEOUT_COLLECTPARAM_GET		EV_TIMER9
#define AP_AGENT_TIMEOUT_COLLECT_NORMAL			EV_TIMER10
#define AP_AGENT_TIMEOUT_COLLECT_RT				EV_TIMER11

/* BEGIN: Added by zhoushouya, 2012/5/21   PN:986 */
#define AP_AGENT_TIMEOUT_CHECK_L2WD			    EV_TIMER12 /* ���l2wdģ���Ƿ��powerap������һ�� */
/* END:   Added by zhoushouya, 2012/5/21 */

/* BEGIN: Added by lvjianlin, 2013/3/20   PN:BUG #2135 ��IXP������������������������������AC�����󣬲������㷢�ֵ�AP�޷����� */
#define AP_AGENT_TIMEOUT_AP_IP_CHANGE           EV_TIMER13
/* END:   Added by lvjianlin, 2013/3/20   PN:BUG #2135 ��IXP������������������������������AC�����󣬲������㷢�ֵ�AP�޷����� */

/* BEGIN: Added by lvjianlin, 2013/6/19   PN:lte-fi  GPS ALARM  LOCAL PORTAL */
//#define AP_AGENT_TIMEOUT_LTE_CHECK_GPIO         EV_TIMER14
//#define AP_AGENT_TIMEOUT_LTE_REBOOT_AP          EV_TIMER15
#define AP_AGENT_TIMEOUT_CHECK_GPIO			    EV_TIMER14 /* BEGIN: Added by ����, 2013/6/19   PN:��ʱ5s���GPIO��״̬��ʱ��Ϣ */
#define AP_AGENT_TIMEOUT_CHECK_BETTRY			EV_TIMER15 
#define AP_AGENT_TIMEOUT_BATTERY_ALARM_CLEAR    EV_TIMER16
/* END:   Added by lvjianlin, 2013/6/19   PN:lte-fi  GPS ALARM  LOCAL PORTAL */

/* BEGIN: Added by lvjianlin, 2013/8/6   PN:lte-fi updatedata bug and report time */
#define AP_AGENT_TIMEOUT_REPORT_BSSID           EV_TIMER17
/* END:   Added by lvjianlin, 2013/8/6   PN:lte-fi updatedata bug and report time */

/* BEGIN: Added by liyoukun, 2013/12/3   PN:task#0  GPS��Ϣ�ϱ��޸ģ�����ͨ��echo�ϱ�AC�����ϱ���GPS������ */
#define AP_AGENT_TIMEOUT_REPORT_GPSINFO         EV_TIMER18
/* END:   Added by liyoukun, 2013/12/3   PN:task#0  GPS��Ϣ�ϱ��޸ģ�����ͨ��echo�ϱ�AC�����ϱ���GPS������ */
/* BEGIN: Added by liyoukun, 2014/1/20   PN:���lte����ģʽ���л����ϱ��澯 */
#define AP_AGENT_TIMEOUT_LTE_NETTYPE            EV_TIMER19
/* END:   Added by liyoukun, 2014/1/20   PN:���lte����ģʽ���л����ϱ��澯 */
#define AP_AGENT_TIMERLEN_CHECK_ACLINKSTATE	  	20			/* ��λ:100ms */
#define AP_AGENT_TIMERLEN_SEND_OMASTART			100			/* ��λ:100ms */
#define AP_AGENT_TIMERLEN_SEND_HEARTBEART		50
#define AP_AGENT_TIMERLEN_CHECK_ALARM			10
#define AP_AGENT_TIMERLEN_CHECK_PERF				60
#define AP_AGENT_TIMERLEN_DELAY_SWUPGRADE		50
#define AP_AGENT_TIMERLEN_CHECK_DHCPSTATE		10	
#define AP_AGENT_TIMERLEN_CHECK_DHCPIPCHG		300			/* ��λ:100ms */
#define AP_AGENT_TIMERLEN_TIMERHANDLE			10			/* ��λ:100ms */
#define AP_AGENT_TIMERLEN_COLLECTPARAM_GET_INIT	50
#define AP_AGENT_TIMERLEN_COLLECTPARAM_GET		3000
#define AP_AGENT_TIMERLEN_COLLECT_NORMAL		100
#define AP_AGENT_TIMERLEN_COLLECT_RT				50
#define AP_AGENT_TIMERLEN_CHECK_L2WD			3000

/* BEGIN: Added by lvjianlin, 2013/3/20   PN:BUG #2135 ��IXP������������������������������AC�����󣬲������㷢�ֵ�AP�޷����� */
#define AP_AGENT_TIMERLEN_CHECK_IPCHANGE_RES    30
/* END:   Added by lvjianlin, 2013/3/20   PN:BUG #2135 ��IXP������������������������������AC�����󣬲������㷢�ֵ�AP�޷����� */

/* BEGIN: Added by lvjianlin, 2013/6/21   PN:lte-fi  GPS ALARM  LOCAL PORTAL */
#define AP_AGENT_TIMERLEN_CHECK_GPIO			50          /* BEGIN: Added by ����, 2013/6/19   PN:��ʱ5s���GPIO��״̬��ʱ��Ϣ */
#define AP_AGENT_TIMERLEN_CHECK_BATTERY			12000       /* BEGIN: Added by ����, 2013/6/19   PN:��ʱ20min�ϱ��澯��Ϣ */
#define AP_AGENT_TIMERLEN_BATTERY_ALARM_CLEAN   600
/* END:   Added by lvjianlin, 2013/6/21   PN:lte-fi  GPS ALARM  LOCAL PORTAL */

/* BEGIN: Added by liyoukun, 2013/12/3   PN:task#0  GPS��Ϣ�ϱ��޸ģ�����ͨ��echo�ϱ�AC�����ϱ���GPS������ */
#define AP_AGENT_TIMERLEN_REPORT_GPSINFO		100          /* ��λ��100ms��Ĭ��GPS�ϱ����� */
/* END:   Added by liyoukun, 2013/12/3   PN:task#0  GPS��Ϣ�ϱ��޸ģ�����ͨ��echo�ϱ�AC�����ϱ���GPS������ */
/* BEGIN: Added by lvjianlin, 2013/8/6   PN:lte-fi updatedata bug and report time */
#define AP_AGENT_TIMERLEN_REPORT_BSSID          300
/* END:   Added by lvjianlin, 2013/8/6   PN:lte-fi updatedata bug and report time */
/* BEGIN: Added by liyoukun, 2014/1/20   PN:���lte����ģʽ���л����ϱ��澯 */
#define AP_AGENT_TIMERLEN_CHECK_LTE_NETTYPE         50      /* �������5s����λ100ms */
/* END:   Added by liyoukun, 2014/1/20   PN:���lte����ģʽ���л����ϱ��澯 */
extern unsigned int		gulBasicSetupId;
extern unsigned int	 	gulIpSetupId;
extern unsigned int	 	gulLoadBalanceSetupId;
extern unsigned int	 	gulSwServerSetupId;
extern unsigned int	 	gulLogSetupId;
extern unsigned int	 	gulSntpSetupId;
extern unsigned int	 	gulWifiDeviceTableId;
extern unsigned int	 	gulApVapTableRwId;
extern unsigned int	 	gulAdhocVapTableRwId;
extern unsigned int	 	gulEnetInfTableId;
extern unsigned int	 	gulRouteBasicSetupId;
extern unsigned int	 	gulRouteNeighSetupId;
extern unsigned int	 	gulRouterHandoverSetupId;
extern unsigned int	 	gulRouteMscSetupId;	
extern unsigned int	 	gulCapWapSetupId;	
extern unsigned int	 	gulPerfCfgId;	
extern unsigned int	 	gulAlarmfiltercfgId;	
extern unsigned int	 	gulAlarmThresholdCfgId;
/* BEGIN: Added by ������, 2012/12/13   PN:TASK #257 VAP���������ù��� ���ð����� */
extern unsigned int     g_ulVapMacFlowCtrlSetupId;
/* END:   Added by ������, 2012/12/13   PN:TASK #257 VAP���������ù��� ���ð����� */
/* BEGIN: Added by lvjianlin, 2013/6/21   PN:lte-fi  GPS ALARM  LOCAL PORTAL */
extern unsigned int     gulLteFiBasicSetupId;
/* END:   Added by lvjianlin, 2013/6/21   PN:lte-fi  GPS ALARM  LOCAL PORTAL */
extern T_agentDataUpdateHandle	gastDUHandle[AGENT_DATA_UPDATE_ID_MAX];

extern unsigned int ap_agent_update_sysbasicsetup(T_configFileData	*pstData);
extern unsigned int ap_agent_update_sysipsetup(T_configFileData	*pstData);
extern unsigned int ap_agent_update_syskeepersetup(T_configFileData	*pstData);
extern unsigned int ap_agent_update_loadbalancesetup(T_configFileData	*pstData);
extern unsigned int ap_agent_update_sysswserversetup(T_configFileData	*pstData);
extern unsigned int ap_agent_update_logsetup(T_configFileData	*pstData);
extern unsigned int ap_agent_update_sntpsetup(T_configFileData	*pstData);
extern unsigned int ap_agent_update_wifidevicetable(T_configFileData	*pstData);
extern unsigned int ap_agent_update_apvaptablerw(T_configFileData	*pstData);
extern unsigned int ap_agent_update_adhocvaptablerw(T_configFileData	*pstData);
extern unsigned int ap_agent_update_enetinftable(T_configFileData	*pstData);
extern unsigned int ap_agent_update_routebasicsetup(T_configFileData	*pstData);
extern unsigned int ap_agent_update_routerneighsetup(T_configFileData	*pstData);
extern unsigned int ap_agent_update_routerhandoversetup(T_configFileData	*pstData);
extern unsigned int ap_agent_update_routermscsetup(T_configFileData	*pstData);
extern unsigned int ap_agent_update_capwapsetup(T_configFileData	*pstData);
extern unsigned int ap_agent_update_perfcfg(T_configFileData	*pstData);
extern unsigned int ap_agent_update_alarmfiltercfg(T_configFileData	*pstData);
extern unsigned int ap_agent_update_alarmthresholdcfg(T_configFileData	*pstData);
/* BEGIN: Added by ������, 2012/12/11   PN:TASK #257 VAP���������ù��� ���ð����� */
extern unsigned int ap_agent_set_flowctrl(void);

/* END:   Added by ������, 2012/12/11   PN:TASK #257 VAP���������ù��� ���ð����� */

extern unsigned int		ac_cw_wtp_gen_agentdat_sysbasicsetup(T_configFileData	*pstData);
extern unsigned int		ac_cw_wtp_gen_agentdat_sysipsetup(T_configFileData	*pstData);
extern unsigned int		ac_cw_wtp_gen_agentdat_loadbalancesetup(T_configFileData	*pstData);
extern unsigned int		ac_cw_wtp_gen_agentdat_sysswserversetup(T_configFileData	*pstData);
extern unsigned int		ac_cw_wtp_gen_agentdat_logsetup(T_configFileData	*pstData);
extern unsigned int		ac_cw_wtp_gen_agentdat_sntpsetup(T_configFileData	*pstData);
extern unsigned int		ac_cw_wtp_gen_agentdat_wifidevicetable(T_configFileData	*pstData);
extern unsigned int		ac_cw_wtp_gen_agentdat_apvaptablerw(T_configFileData	*pstData);
extern unsigned int		ac_cw_wtp_gen_agentdat_adhocvaptablerw(T_configFileData	*pstData);
extern unsigned int		ac_cw_wtp_gen_agentdat_enetinftable(T_configFileData	*pstData);
extern unsigned int		ac_cw_wtp_gen_agentdat_routebasicsetup(T_configFileData	*pstData);
extern unsigned int		ac_cw_wtp_gen_agentdat_routerneighsetup(T_configFileData	*pstData);
extern unsigned int		ac_cw_wtp_gen_agentdat_routerhandoversetup(T_configFileData	*pstData);
extern unsigned int		ac_cw_wtp_gen_agentdat_routermscsetup(T_configFileData	*pstData);
extern unsigned int		ac_cw_wtp_gen_agentdat_capwapsetup(T_configFileData	*pstData);
extern unsigned int		ac_cw_wtp_gen_agentdat_perfcfg(T_configFileData	*pstData);
extern unsigned int		ac_cw_wtp_gen_agentdat_alarmfiltercfg(T_configFileData	*pstData);
extern unsigned int		ac_cw_wtp_gen_agentdat_alarmthresholdcfg(T_configFileData	*pstData);
/* BEGIN: Added by ������, 2012/12/6   PN:TASK #257 VAP���������ù��� ���ð����� */
extern unsigned int     ac_cw_wtp_gen_agentdat_vapmacflowctrlsetup(T_configFileData * pstData);
/* END:   Added by ������, 2012/12/6   PN:TASK #257 VAP���������ù��� ���ð����� */

/* BEGIN: Added by lvjianlin, 2013/6/20   PN:lte-fi  GPS ALARM  LOCAL PORTAL */
extern unsigned int     ac_cw_wtp_gen_agentdat_ltefibasicsetup(T_configFileData *pstData);
/* END:   Added by lvjianlin, 2013/6/20   PN:lte-fi  GPS ALARM  LOCAL PORTAL */

extern unsigned int	 	ap_agent_read_apbasiccfg(T_deviceBasicCfg *pstCfg);
extern unsigned int		ap_agent_save_apbasiccfg(T_deviceBasicCfg *pstCfg);

/* BEGIN: Added by zhoushouya, 2012/5/21   PN:986 */
extern void ac_cw_wtp_check_l2wd(void);
/* END:   Added by zhoushouya, 2012/5/21 */

extern T_deviceBasicCfg	gstDevBasicCfg;

#endif

