#ifndef AGENTTYPE_H
#define AGENTTYPE_H

#include   <sys/time.h>
#include   <time.h>


#ifndef AC_USRM_PROC_TS_FILE_EVT
#define AC_USRM_PROC_TS_FILE_EVT    "/proc/ts"
#endif
/* AP-WEB修改时维护本地的配置数据版本号*/
#define INC_CONFIG_VERSION(n)	(n)=((n)/6*6+6+4)

#define DEFAULT_VAP_SSID        "CMCC"

#define IEEE80211_ADDR_LEN 6
#define IEEE80211_IPADDR_LEN 4

#define INVALID_VAP_ID			0XFFFFFFFF			/* 无效的AP ID */
#define INVALID_VLAN_ID			0XFFFFFFFF			/* 无效的VLAN ID */
#define INVALID_WIFIDEVICE_ID	0XFFFFFFFF			/* 无效的无线设备ID */
#define INVALID_WIFI_ID			0XFFFFFFFF			/* 无效的无线模块ID */
#define INVALID_PROTOCOL		0XFFFFFFFF			/* 无效的协议 */
#define INVALID_DATA			0XFFFFFFFF			/* 无效的数据 */

#define INVALID_PERFCOUNTERGROUP_ID	0XFFFFFFFF	/* 无效的性能计数器组ID */

#define WIFI_STATE_DISABLE			1
#define WIFI_STATE_ENABLE			2
#define WIFI_STATE_NEED				1
#define WIFI_STATE_UNNEED			0

/* hc add for dhcp realloc 2009-6-26 */
#define AC_ID_NONE          0			/* 没有ac 链接*/
#define SAMPLE_TIME_DEFAULT 10
#define SAMPLE_RATE_DEFAULT 1

#define AP_AGENT_OPSTATE_UNKNOWN          	0			/* 状态未知 */
#define AP_AGENT_OPSTATE_NORMAL           	1			/* 正常 */
#define AP_AGENT_OPSTATE_FAULT            	2			/* 故障 */
#define AC_LINK_KEEPSTATE_FAULT			    3			/* 上行链路故障 */

/* hc modified for cmcc test 2009-8-28 */
#define AC_LINK_STATE_UNKNOW                        0    /* 未知*/
#define AC_LINK_STATE_NORMAL                        1    /* 正常*/
#define AC_LINK_STATE_NO_DISCOVERY_RSP              2    /*未连接: 没有收到发现响应*/
#define AC_LINK_STATE_NO_CONNCET_RSP                3    /*未连接: 没有收到连接响应*/
#define AC_LINK_STATE_KEEPER_TIME_OUT               4    /*未连接: 保活超时*/
#define AC_LINK_STATE_UP_LINK_ERR                   5    /*未连接: AC 上行链路故障 */
#define AC_LINK_STATE_OTHER_REASON                  6    /*未连接: 其它原因*/

#define SWITCH_NONE					0			/* 没有切换 */
#define FAT_TO_FIT_AP				1			/* FAT AP切换到FIT AP */
#define FIT_TO_FAT_AP				2			/* FIT AP切换到FAT AP */

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

/* BEGIN: Added by 吕建林, 2012/12/16   PN:TASK #257 VAP级流控配置功能 */
#define AGENT_FLOWCTRL_INVALID_VALUE 0
/* END:   Added by 吕建林, 2012/12/16   PN:TASK #257 VAP级流控配置功能 */
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

#define ROGUE_AP_MAX		64                                              /* 非法AP数据 */

#define IEEE80211_ADDR_LEN  6                                               /* IEEE80211 地址长度 */

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
/* 9344 和 wallAP平台 */
typedef struct deviceSign_s
{
	long	lDeviceType;				/* 设备类型 */
	/* -------------------------------------------------------------------------------------------------------- */
	/*	宏的组成原则是：000 000 000 前面三位代表设备系列，中间三位代表有效卡数，最后三位代表子设备类型          */
	/* 	如001，001，001，即1001001：																			*/
	/*  第一个001表示无线MESH路由器系列，第二个001表示使用了1张无线网卡，第三个001表示独立的回传类型			*/
	/* -------------------------------------------------------------------------------------------------------- */
	/* 无线MESH路由器 */
#define sysDeviceType_B1100e		1001001
#define sysDeviceType_B1100e_I      1001009
#define sysDeviceType_B1100e_T		1001002
#define sysDeviceType_B1100e_S		1001003/* 已删除 */
#define sysDeviceType_B1100e_ST		1001004/* 已删除 */
#define sysDeviceType_B1010e		1001005/* 已删除 */
#define sysDeviceType_B1010e_T		1001006/* 已删除 */
#define sysDeviceType_B1010e_S		1001007/* 已删除 */
#define sysDeviceType_B1010e_ST		1001008/* 已删除 */

#define sysDeviceType_B2011e		1002001/* 已删除 */
#define sysDeviceType_B2011e_T		1002002/* 已删除 */
#define sysDeviceType_B2011e_S		1002003/* 已删除 */
#define sysDeviceType_B2011e_ST		1002004/* 已删除 */

#define sysDeviceType_B2101e		1002005
#define sysDeviceType_B2101e_I      1002019
#define sysDeviceType_B2101e_T		1002006
#define sysDeviceType_B2101e_S		1002007/* 已删除 */
#define sysDeviceType_B2101e_ST		1002008/* 已删除 */

#define sysDeviceType_B2020e		1002009/* 已删除 */
#define sysDeviceType_B2020e_T		1002010/* 已删除 */
#define sysDeviceType_B2020e_ST		1002011/* 已删除 */

#define sysDeviceType_B2200e		1002012
#define sysDeviceType_B2200e_I      1002020
#define sysDeviceType_B2200e_T		1002013
#define sysDeviceType_B2200e_ST		1002014/* 已删除 */

#define sysDeviceType_B2101e_E		1002015
#define sysDeviceType_B2200e_Q		1002016/* 已删除 */
#define sysDeviceType_B2200e_QT		1002017/* 已删除 */
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
#define sysDeviceType_B4400e_4T		1004006/* 旧型号：B4400e-T*/
#define sysDeviceType_B4400e_2T		1004007/* 新增型号 */

	/* 无线MESH终端 */
#define sysDeviceType_GT100			2001001
#define sysDeviceType_GT200			2002001
#define sysDeviceType_GT201			2002002
#define sysDeviceType_GT201_A		2002003/* 已删除 */

	/* 室外无线网桥 */
#define sysDeviceType_GQ100			3001001/* 已删除 */
#define sysDeviceType_GQ100_T		3001002/* 已删除 */
#define sysDeviceType_GQ100_S		3001003/* 已删除 */
#define sysDeviceType_GQ100_ST		3001004/* 已删除 */
#define sysDeviceType_GQ200			3002001/* 已删除 */
#define sysDeviceType_GQ200_T		3002002/* 已删除 */
#define sysDeviceType_GQ200_S		3002003/* 已删除 */
#define sysDeviceType_GQ200_ST		3002004/* 已删除 */

	/* 室内无线接入点 */
#define sysDeviceType_GA100i_H		4001001
#define sysDeviceType_GA100i_A		4001002
#define sysDeviceType_GA100i_HE		4001003/* 旧型号：GA100i_AE */
#define sysDeviceType_GA100i_HE_I	4001004
#define sysDeviceType_GA100i_H_I	4001005
#define sysDeviceType_GA100i_A_I	4001006
#define sysDeviceType_GA100i_AE		4001007
#define sysDeviceType_GA100i_AE_I	4001008
#define sysDeviceType_GA200i		4002001
#define sysDeviceType_GA200i_M		4002002
#define sysDeviceType_GA200i_MT		4002003
#define sysDeviceType_GA200i_D		4002004/* 已删除 */
#define sysDeviceType_GA200i_A		4002005/* 已删除 */
#define sysDeviceType_GA200i_AD		4002006/* 已删除 */

#define sysDeviceType_GA200i_EM		4002007
#define sysDeviceType_GA200i_EM_I   4002011
#define sysDeviceType_GA200i_AE		4002008/* 已删除 */
#define sysDeviceType_GA200i_E      4002009
#define sysDeviceType_GA200i_E_I    4002010

	/* 室外无线接入点 */
#define sysDeviceType_GA100e		5001001
#define sysDeviceType_GA100e_A		5001002/* 已删除 */
#define sysDeviceType_GA100e_S		5001003/* 已删除 */
#define sysDeviceType_GA100e_AS		5001004/* 已删除 */
#define sysDeviceType_GA100e_E		5001005/* 旧型号：GA100e_AE */
#define sysDeviceType_GA100e_I		5001006
#define sysDeviceType_GA200e		5002001
#define sysDeviceType_GA200e_A		5002002/* 已删除 */
#define sysDeviceType_GA200e_D		5002003/* 已删除 */
#define sysDeviceType_GA200e_S		5002004/* 已删除 */
#define sysDeviceType_GA200e_AD		5002005/* 已删除 */
#define sysDeviceType_GA200e_AS		5002006/* 已删除 */
#define sysDeviceType_GA200e_DS		5002007/* 已删除 */
#define sysDeviceType_GA200e_ADS	5002008/* 已删除 */
#define sysDeviceType_GA200e_E		5002009/* 旧型号：GA200e_AE */
#define sysDeviceType_GA200e_I		5002010


	/* 轨道交通无线基站 */
#define sysDeviceType_GS100B_L		6001001/* 已删除 */
#define sysDeviceType_GS100B		6001002
#define sysDeviceType_GS100B_G		6001003/* 新增型号 */
#define sysDeviceType_HB4_GS100BD	6002001/* 旧型号：GS200B */
#define sysDeviceType_GS200B		6002002/* 新增型号 */
#define sysDeviceType_GS200B_G		6002003/* 新增型号 */
#define sysDeviceType_GS400B		6004001/* 已删除 */

	/* 轨道交通无线终端 */
#define sysDeviceType_GS100T_L		7001001/* 已删除 */
#define sysDeviceType_GS100T		7001002
#define sysDeviceType_GS100T_G		7001003/* 新增型号 */
#define sysDeviceType_GS200T		7002001
#define sysDeviceType_GS201T		7002002/* 已删除 */
#define sysDeviceType_GS200T_G		7002003/* 新增型号 */

	/* HT2车载WLAN终端 */
#define  sysDeviceType_GRR100		8001001
#define  sysDeviceType_GRR200		8002001
#define  sysDeviceType_GRR201		8002003

	/* HB4室外WLAN终端 */
#define  sysDeviceType_GRR100e		8001002
#define  sysDeviceType_GRR200e		8002002

	/* 2010-7-26 新增: 11n设备型号*/
#define  sysDeviceType_N200H_GA100i_GN11		9001001  // 单卡11gn，500mW，mimo 1×1		11n室内大功率接入点（500mW）
#define  sysDeviceType_N200_GA100i_GN22         9001002  // 单卡11gn，100mW，mimo 2×2		11n室内小功率接入点（100mW）
#define  sysDeviceType_N200_GA200i_AGN22        9002001  // 双卡11agn，100mW，mimo 2×2		11n室内双频小功率接入点（100mW）

/* 2011-6-1 电信测试新增*/
#define sysDeviceType_NB4_GA100e_GN22			10001001		/************* 使用中:11gn,单频2*2,室外,500mW(2 chain) */
#define sysDeviceType_NB4_GA200e_AGN22			10001002		/************* 使用中:11agn,双频2*2,室外,11gn/500mW(2 chain),11an/100mW(2 chain) */

/* BEGIN: Added by lvjianlin, 2013/4/25   PN:ltefi  */
#define sysDeviceType_LTE_Fi_AGN22  9002002             //LTEFI 胖瘦合一
/* END:   Added by lvjianlin, 2013/4/25   PN:ltefi  */

	unsigned char	aucDeviceId[6];		/* 设备ID，6个16进制数 */
	unsigned long	ulDeviceModuleType;	/* 设备主从类型*/
#define sysDeviceModuleType_ALONE	0
#define sysDeviceModuleType_MAIN	1
#define sysDeviceModuleType_SLAVE	2
#define WIFI_DEV_AMP_SET_MASK  	    0x01
#define WIFI_DEV_AMP_VALUE_MASK     0xf8
    T_WIFI_AMP_CFG    stModuleWifiAmpCfg1;
    T_WIFI_AMP_CFG    stModuleWifiAmpCfg2;
    
	unsigned char aucCustomer[64];  /* 配置客户标记 , 以字符串形式的客户代号 默认值全0 */
	unsigned char aucRsv1[64];      /* 预留字段1，默认值为全0 */
	unsigned char aucRsv2[64];      /* 预留字段2，默认值为全0 */
	unsigned char aucRsv3[64];      /* 预留字段3，默认值为全0 */
	unsigned char aucRsv4[64];      /* 预留字段4，默认值为全0 */

	int	setFlag;				    /* 是否配置了设备类型标识 */
} T_deviceSign;
#else
/* AP93, BP44, IXP 平台的产测区结构定义 */
typedef struct deviceSign_s
{
	long	lDeviceType;				/* 设备类型 */
	/* -------------------------------------------------------------------------------------------------------- */
	/*	宏的组成原则是：000 000 000 前面三位代表设备系列，中间三位代表有效卡数，最后三位代表子设备类型          */
	/* 	如001，001，001，即1001001：																			*/
	/*  第一个001表示无线MESH路由器系列，第二个001表示使用了1张无线网卡，第三个001表示独立的回传类型			*/
	/* -------------------------------------------------------------------------------------------------------- */
	/* 无线MESH路由器 */
#define sysDeviceType_B1100e		1001001
#define sysDeviceType_B1100e_I      1001009
#define sysDeviceType_B1100e_T		1001002
#define sysDeviceType_B1100e_S		1001003/* 已删除 */
#define sysDeviceType_B1100e_ST		1001004/* 已删除 */
#define sysDeviceType_B1010e		1001005/* 已删除 */
#define sysDeviceType_B1010e_T		1001006/* 已删除 */
#define sysDeviceType_B1010e_S		1001007/* 已删除 */
#define sysDeviceType_B1010e_ST		1001008/* 已删除 */

#define sysDeviceType_B2011e		1002001/* 已删除 */
#define sysDeviceType_B2011e_T		1002002/* 已删除 */
#define sysDeviceType_B2011e_S		1002003/* 已删除 */
#define sysDeviceType_B2011e_ST		1002004/* 已删除 */

#define sysDeviceType_B2101e		1002005
#define sysDeviceType_B2101e_I      1002019
#define sysDeviceType_B2101e_T		1002006
#define sysDeviceType_B2101e_S		1002007/* 已删除 */
#define sysDeviceType_B2101e_ST		1002008/* 已删除 */

#define sysDeviceType_B2020e		1002009/* 已删除 */
#define sysDeviceType_B2020e_T		1002010/* 已删除 */
#define sysDeviceType_B2020e_ST		1002011/* 已删除 */

#define sysDeviceType_B2200e		1002012
#define sysDeviceType_B2200e_I      1002020
#define sysDeviceType_B2200e_T		1002013
#define sysDeviceType_B2200e_ST		1002014/* 已删除 */

#define sysDeviceType_B2101e_E		1002015
#define sysDeviceType_B2200e_Q		1002016/* 已删除 */
#define sysDeviceType_B2200e_QT		1002017/* 已删除 */
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
#define sysDeviceType_B4400e_4T		1004006/* 旧型号：B4400e-T*/
#define sysDeviceType_B4400e_2T		1004007/* 新增型号 */

	/* 无线MESH终端 */
#define sysDeviceType_GT100			2001001
#define sysDeviceType_GT200			2002001
#define sysDeviceType_GT201			2002002
#define sysDeviceType_GT201_A		2002003/* 已删除 */

	/* 室外无线网桥 */
#define sysDeviceType_GQ100			3001001/* 已删除 */
#define sysDeviceType_GQ100_T		3001002/* 已删除 */
#define sysDeviceType_GQ100_S		3001003/* 已删除 */
#define sysDeviceType_GQ100_ST		3001004/* 已删除 */
#define sysDeviceType_GQ200			3002001/* 已删除 */
#define sysDeviceType_GQ200_T		3002002/* 已删除 */
#define sysDeviceType_GQ200_S		3002003/* 已删除 */
#define sysDeviceType_GQ200_ST		3002004/* 已删除 */

	/* 室内无线接入点 */
#define sysDeviceType_GA100i_H		4001001
#define sysDeviceType_GA100i_A		4001002
#define sysDeviceType_GA100i_HE		4001003/* 旧型号：GA100i_AE */
#define sysDeviceType_GA100i_HE_I	4001004
#define sysDeviceType_GA100i_H_I	4001005
#define sysDeviceType_GA100i_A_I	4001006
#define sysDeviceType_GA100i_AE		4001007
#define sysDeviceType_GA100i_AE_I	4001008
#define sysDeviceType_GA200i		4002001
#define sysDeviceType_GA200i_M		4002002
#define sysDeviceType_GA200i_MT		4002003
#define sysDeviceType_GA200i_D		4002004/* 已删除 */
#define sysDeviceType_GA200i_A		4002005/* 已删除 */
#define sysDeviceType_GA200i_AD		4002006/* 已删除 */

#define sysDeviceType_GA200i_EM		4002007
#define sysDeviceType_GA200i_EM_I   4002011
#define sysDeviceType_GA200i_AE		4002008/* 已删除 */
#define sysDeviceType_GA200i_E      4002009
#define sysDeviceType_GA200i_E_I    4002010

	/* 室外无线接入点 */
#define sysDeviceType_GA100e		5001001
#define sysDeviceType_GA100e_A		5001002/* 已删除 */
#define sysDeviceType_GA100e_S		5001003/* 已删除 */
#define sysDeviceType_GA100e_AS		5001004/* 已删除 */
#define sysDeviceType_GA100e_E		5001005/* 旧型号：GA100e_AE */
#define sysDeviceType_GA100e_I		5001006
#define sysDeviceType_GA200e		5002001
#define sysDeviceType_GA200e_A		5002002/* 已删除 */
#define sysDeviceType_GA200e_D		5002003/* 已删除 */
#define sysDeviceType_GA200e_S		5002004/* 已删除 */
#define sysDeviceType_GA200e_AD		5002005/* 已删除 */
#define sysDeviceType_GA200e_AS		5002006/* 已删除 */
#define sysDeviceType_GA200e_DS		5002007/* 已删除 */
#define sysDeviceType_GA200e_ADS	5002008/* 已删除 */
#define sysDeviceType_GA200e_E		5002009/* 旧型号：GA200e_AE */
#define sysDeviceType_GA200e_I		5002010


	/* 轨道交通无线基站 */
#define sysDeviceType_GS100B_L		6001001/* 已删除 */
#define sysDeviceType_GS100B		6001002
#define sysDeviceType_GS100B_G		6001003/* 新增型号 */
#define sysDeviceType_HB4_GS100BD	6002001/* 旧型号：GS200B */
#define sysDeviceType_GS200B		6002002/* 新增型号 */
#define sysDeviceType_GS200B_G		6002003/* 新增型号 */
#define sysDeviceType_GS400B		6004001/* 已删除 */

	/* 轨道交通无线终端 */
#define sysDeviceType_GS100T_L		7001001/* 已删除 */
#define sysDeviceType_GS100T		7001002
#define sysDeviceType_GS100T_G		7001003/* 新增型号 */
#define sysDeviceType_GS200T		7002001
#define sysDeviceType_GS201T		7002002/* 已删除 */
#define sysDeviceType_GS200T_G		7002003/* 新增型号 */

	/* HT2车载WLAN终端 */
#define  sysDeviceType_GRR100		8001001
#define  sysDeviceType_GRR200		8002001
#define  sysDeviceType_GRR201		8002003

	/* HB4室外WLAN终端 */
#define  sysDeviceType_GRR100e		8001002
#define  sysDeviceType_GRR200e		8002002

	/* 2010-7-26 新增: 11n设备型号*/
#define  sysDeviceType_N200H_GA100i_GN11		9001001  // 单卡11gn，500mW，mimo 1×1		11n室内大功率接入点（500mW）
#define  sysDeviceType_N200_GA100i_GN22         9001002  // 单卡11gn，100mW，mimo 2×2		11n室内小功率接入点（100mW）
#define  sysDeviceType_N200_GA200i_AGN22        9002001  // 双卡11agn，100mW，mimo 2×2		11n室内双频小功率接入点（100mW）

/* 2011-6-1 电信测试新增*/
#define sysDeviceType_NB4_GA100e_GN22			10001001		/************* 使用中:11gn,单频2*2,室外,500mW(2 chain) */
#define sysDeviceType_NB4_GA200e_AGN22			10001002		/************* 使用中:11agn,双频2*2,室外,11gn/500mW(2 chain),11an/100mW(2 chain) */

/* BEGIN: Added by lvjianlin, 2013/4/25   PN:ltefi  */
#define sysDeviceType_LTE_Fi_AGN22  9002002             //LTEFI 胖瘦合一
/* END:   Added by lvjianlin, 2013/4/25   PN:ltefi  */


	unsigned char	aucDeviceId[6];		/* 设备ID，6个16进制数 */
	int		setFlag;					/* 是否配置了设备类型标识 */
	unsigned long	ulDeviceModuleType;	/* 设备主从类型*/
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
     unsigned long ulMulticastSwitch;		/* 允许多播组*/
#define MULTICAST_SWITCH_DISABLE     0		/* 否：缺省为否*/
#define MULTICAST_SWITCH_ENABLE      1		/* 是*/
     unsigned long ulBroadcastSwitch;		/* 允许广播（除ARP，DHCP）*/
#define BROADCAST_SWITCH_DISABLE     0		/* 否：缺省为否*/
#define BROADCAST_SWITCH_ENABLE       1		/* 是*/
     unsigned long ulBMRateLimitSwitch;		/* 广播/多播 是否限速*/
#define BM_RATE_LIMIT_SWITCH_DISABLE      0 /* 否*/
#define BM_RATE_LIMIT_SWITCH_ENABLE       1 /* 是：缺省为是*/
     unsigned long ulMaxRate;               /* 最大速率：个/秒；缺省为*/
} T_ACL_BROADCAST_POLICY;

struct ieee80211_scan_ssid
{
	int len;					            /* length in bytes */
	unsigned char ssid[32];	                /* ssid contents */
};

typedef struct
{
	unsigned long ulDosAttackDetectSwitch;		/* DOS攻击检测 */
#define DOS_ATTACK_DETECT_SWITCH_OFF	0
#define DOS_ATTACK_DETECT_SWITCH_ON		1
	unsigned long ulDosAttackPeriod;			/* 泛洪攻击统计周期 */
	unsigned long ulDosAttackThreshold;			/* 泛洪攻击判定门限 */
	unsigned long ulDosAttackDefenseSwitch;		/* DOS攻击防御 */
#define DOS_ATTACK_DEFENSE_SWITCH_OFF	0
#define DOS_ATTACK_DEFENSE_SWITCH_ON	1
	unsigned long ulInvalidTerminalAgeingTime;	/* 恶意终端老化时间 */
	unsigned long ulInvalidApDetectSwitch;		/* 恶意AP检测 */
	unsigned char aucInvalidApSpecificBssid[6];	/* 恶意AP定义/指定BSSID */
	unsigned char aucResv1[2];
	unsigned char aucInvalidApSpecificOui[3];	/* 恶意AP定义/指定厂商 */
	unsigned char aucResv2[1];
	unsigned long ulInvalidApSameSsidSwitch;	/* 恶意AP定义/指定相同SSID */
	unsigned long ulInvalidApHiddenSsidSwitch;	/* 恶意AP定义/指定隐藏SSID */
	unsigned char aucInvalidSpecificSsid[32];	/* 恶意AP定义/指定SSID */
} WIDS_T;

/* hc modified for telecom test 2009-7-6 */
#define LEGAL_DEVICE_MAX    16

typedef struct{
	unsigned long  ulWtpId;
	unsigned long  wifiDeviceIndex;						/* 设备编号             */	
	unsigned long  dos_detect_switch ;                  /* 泛洪攻击检测开关     */
	unsigned long  period ;                             /* 泛洪攻击统计周期，单位：ms，缺省值为1000ms   */
	unsigned long  limit ;                              /* 泛洪攻击最大接收包数门限，缺省值为50         */
	unsigned long  action ;                             /* 泛洪攻击防御，0：无防御；1：有防御（加入动态黑名单）；缺省值为1，即启动泛洪攻击防御 */
	unsigned long  time_aging ;                         /* 动态黑名单老化时间，单位：s，缺省值为3600s   */
	unsigned long  spoof_detect_switch ;                /* SPOOF检测开关        */
	unsigned long  weekiv_detect_switch ;               /* WEEK IV检测开关      */
	unsigned long  sta_disturb_detect_switch ;          /* 终端干扰检测开关     */
	unsigned long  sta_disturb_limit ;                  /* 终端干扰门限，包速率:pps */

	/* 2010/07/01 fengjing cmcc test */     
    unsigned long  ap_disturb_num_threshold;            /* AP干扰告警门限 */
	unsigned long  sta_disturb_num_threshold;           /*终端干扰告警门限*/
    unsigned long  ap_cointer_threshold;                /* AP同频干扰告警门限，接收信号强度，dBm*/
    unsigned long  ap_neiborinter_threshold;            /* AP邻频干扰告警门限，接收信号强度，dBm */
	
	unsigned long  legal_sta_num ;                      /* 合法终端配置个数     */
	unsigned char  legal_sta_mac[LEGAL_DEVICE_MAX][8] ; /* LEGAL_DEVICE_MAX: 16 */
	unsigned long  ap_detect_switch ;                   /* 可疑AP检测开关       */
	unsigned long  legal_ssid_num ;
	unsigned char  legal_ssid_list[LEGAL_DEVICE_MAX][32] ;      /*SSID以’\0’结束*/
	unsigned long  legal_bssid_num ;
	unsigned char  legal_bssid_list[LEGAL_DEVICE_MAX][8] ;      /*legal_bssid_list[][6], legal_bssid_list[][7]两字节保留*/
	unsigned long  legal_oui_num ;
	unsigned char  legal_oui_list[LEGAL_DEVICE_MAX][4] ;        /* legal_oui_list[][3] 字节保留*/

	unsigned long  ulMOC;
	unsigned char  aucMOI[AC_MOI_LEN];
}WIDS_CONFIG_T ;

/* hc modified for telecom test 2009-7-6 */
typedef struct
{
    unsigned char   mac[6] ;                /*恶意终端MAC地址*/
    unsigned char   mac_pad[2] ;
    unsigned long   channel ;               /*信道号*/
    int             rx_power_max ;          /*接收到的最大信号强度，单位：dBm*/
    int             rx_power ;              /*接收到的信号强度*/
    int             noise ;                 /*底噪，单位：dBm*/
    unsigned long   is_adhoc ;              /*恶意终端操作模式，0：非ADHOC；1：ADHOC*/
    unsigned char   attack_bssid[6] ;       /*受攻击AP的BSSID*/
    unsigned char   attack_bssid_pad[2] ;
    unsigned long   time_first ;            /*首次检测时间，单位：秒，自1970.1.1 00:00:00的秒数*/
    unsigned long   time_last ;             /*未次检测时间，单位：秒*/
    unsigned long   action ;                /*反措施，0：无反措施；1：有反措施（加入动态黑名单）*/

    unsigned long   dos_attack ;            /*DOS攻击，0：否，1：是*/
    unsigned long   dos_attack_num ;        /*DOS攻击次数*/
    unsigned long   spoof_attack ;          /*SPOOF攻击，0：否，1：是*/
    unsigned long   spoof_attack_num ;      /*SPOOF攻击次数*/
    unsigned long   weekiv_attack ;         /*WEEKIV攻击，0：否，1：是*/
    unsigned long   weekiv_attack_num ;     /*WEEKIV攻击次数*/
    unsigned long   disturb_attack ;        /*干扰，0：否，1：是*/
    unsigned long   disturb_attack_num ;    /*干扰次数*/
}ROGUE_STA_T ;


#define ROGUE_STA_MAX       16

typedef struct
{
    unsigned long       num ;
    ROGUE_STA_T         sta_list[ROGUE_STA_MAX] ;
}ROGUE_STA_LIST_T ;


/* 用于封装传递到OMC的消息 */
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
    unsigned long ulSn;                             /*信噪比，单位：dB*/
	int	          lRxPower;
	unsigned long ulFirstTime;
	unsigned long ulLastTime;
	unsigned long ulBeaconInterval;
	unsigned long ulEncrypt;
	/* hc modified for telecom test 2009-7-6 */
    unsigned long ulAction ;                /*是否对可疑AP采取对策*/
    unsigned long ulType;			/* 1: 干扰AP 2: 伪AP */
} ROGUE_AP_T;

/* 用于封装从无线获取的消息 */
typedef struct
{
	//unsigned long ulWtpId;
	unsigned long   ulValidNum;
	ROGUE_AP_T      stRogueApList[ROGUE_AP_MAX];
} ROGUE_AP_INFO_T;

/* 用于封装传递到OMC的消息 */
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
#define dataType_OMA_STRUCT         0           /* OMA结构 */
#define dataType_OMC_SETALL         1           /* OMC setAll字节流 */
	unsigned long	dataVersion;
//#define dataVersion_DV1			0X00000001
#define dataVersion_DV1_6_1_B	0X00000002			/* v1.6.1b */
#define dataVersion_DV1_8_0_A	0X00000003			/* v1.6.1b */

	long			dataState;
#define dataState_NONE 			0X11111111
#define dataState_VALID			0X22222222
	unsigned long	dataLen;						/* bytes of config data */
	unsigned long	checkSum;						/* check sum of whole file */
	int				iRsv[4];						/* 预留位 */
} T_configFileHeader;
/* BEGIN: Added by 吕建林, 2013/1/8   PN:TASK #296 【外场需求】山东移动要求本地转发时AP离线也不广播业务SSID */
#define AGENT_DISABLE_SSID_DISABLE  0
#define AGENT_DISABLE_SSID_ENABLE   1
/* END:   Added by 吕建林, 2013/1/8   PN:TASK #296 【外场需求】山东移动要求本地转发时AP离线也不广播业务SSID */
typedef struct sysBasicSetup_s
{
	unsigned long	ulWtpId;
	/* hc modified for cmcc test 2009-7-18 */
	char            acWtpNEId[32];                  /* AP网元编码 */
	long			sysDeviceType;
#define sysDeviceType_A210i 3						/* A210i */
#define sysDeviceType_A210e 4						/* A210e */
	unsigned char	sysDeviceId[6];					/* 6个16进制字符 */
	char			acRsv[2];
	char			acContact[64];					/* 设备维护联系人 */
	char			acLocation[64];					/* 设备所在位置 */
	char			acCountry[64];					/* 设备所在区县 */	
	unsigned long ulMaxStaNum;
	char			sysHwVersion[32];				/* 字符串 */
	char			sysUbootVersion[32];			/* Uboot版本 */
	char			sysSwVersion[32];				/* 字符串 */
	char			sysDeviceName[64];				/* 字符串 */
	unsigned long	sysCountryCode;					/* 国家码 */
#define COUNTRY_CODE_CHINA		156
#define COUNTRY_CODE_GERMANY	276
	unsigned long	ulRunMode;						/* 运行模式 */
#define RUN_MODE_AUTO			0		/* 自适应*/
#define RUN_MODE_FIT_AP		1		/* 集中转发*/
#define RUN_MODE_FAT_AP		2		/* 本地转发*/

	/* 2009-12-3 pyy 新增*/
	unsigned long ulApTrafficVlan;	/* 0~4094 	
								0:表示不打VLAN 
								其它:表示打相应的VLAN Tag */
	
/* added by laiyongxing at 2009-1-9 */
	unsigned long	ulSysNetId;                     /* NET ID used to identify rogue ap*/
	
    unsigned long ulCollectSwitch;			/* 2010-4-10 pyy 新增: 实时采集开关*/
    /* BEGIN: Added by 吕建林, 2013/1/8   PN:TASK #296 【外场需求】山东移动要求本地转发时AP离线也不广播业务SSID */
    /* AP离线关闭SSID开关，状态为:启用/禁用，默认为开启状态 */
    unsigned long ulDisSsidSwitch;
    /* END:   Added by 吕建林, 2013/1/8   PN:TASK #296 【外场需求】山东移动要求本地转发时AP离线也不广播业务SSID */

    /* BEGIN: Added by lvjianlin, 2013/3/26   PN:BUG #2242  开启ulLinkOffRestartTime 字段 */
    /* BEGIN: Added by 吕建林, 2012/12/5   PN:与AC信息模型不对其，暂时删除该成员 */
    
    unsigned long  ulLinkOffRestartTime;     /* 2012-01-5 新增:
											   bug432 增加AP与AC断链后， AP重启的时间配置 
											   默认值3600 最小值300	单位（s） */
	
    /* END:   Added by 吕建林, 2012/12/5   PN:与AC信息模型不对其，暂时删除该成员 */
    /* END:   Added by lvjianlin, 2013/3/26   PN:BUG #2242  开启ulLinkOffRestartTime 字段 */

    /* add by gwx 2012-10-31 for 用户流量不准确问题，默认值60，单位秒*/
    unsigned long   ulRptUserTrafficTime;
    /* END:   Added by 吕建林, 2012/11/9   PN:TS模块，AC计费流量不准确问题 */								   	

    /* BEGIN: Added by 吕建林, 2012/12/10   PN:TASK #257 VAP级流控配置功能 商用版新增 */
    unsigned long   ulFlowCtrlLevel;        // 带宽控制级别 0：STA  1：VAP 2：AP
    #define AGENT_FLOWCTRL_STA  0
    #define AGENT_FLOWCTRL_VAP  1
    #define AGENT_FLOWCTRL_AP   2
    
    unsigned long   ulTotalUpBandMax;       //上行总带宽控制, 单位：kbps
    unsigned long   ulTotalDownBandMax;     //上行总带宽控制, 单位：kbps
    /* END:   Added by 吕建林, 2012/12/10   PN:TASK #257 VAP级流控配置功能 商用版新增 */
    unsigned long	ulMOC;							/* 对象类标识 */
	unsigned char	aucMOI[AC_MOI_LEN];				/* 对象实例标识 */
	unsigned long	ulAdminState;
	unsigned long	ulOpState;
	unsigned long	ulCurAcId;
	unsigned long ulCfgVersion;
} T_sysBasicSetup;
typedef struct sysBasicSetupAcFile_s
{
	unsigned long	ulWtpId;
	/* hc modified for cmcc test 2009-7-18 */
	char            acWtpNEId[32];                  /* AP网元编码 */
	long			sysDeviceType;
#define sysDeviceType_A210i 3						/* A210i */
#define sysDeviceType_A210e 4						/* A210e */
	unsigned char	sysDeviceId[6];					/* 6个16进制字符 */
	char			acRsv[2];
	char			acContact[64];					/* 设备维护联系人 */	
	char			acLocation[64];					/* 设备所在位置 */
	char			acCountry[64];					/* 设备所在区县 */	
	unsigned long ulMaxStaNum;
	char			sysHwVersion[32];				/* 字符串 */
	char			sysUbootVersion[32];			/* Uboot版本 */
	char			sysSwVersion[32];				/* 字符串 */
	char			sysDeviceName[64];				/* 字符串 */
	unsigned long	sysCountryCode;					/* 国家码 */
#define COUNTRY_CODE_CHINA		156
#define COUNTRY_CODE_GERMANY	276
	unsigned long	ulRunMode;						/* 运行模式 */
#define RUN_MODE_AUTO		0
#define RUN_MODE_FAT_AP		2
#define RUN_MODE_FIT_AP		1
/* added by laiyongxing at 2009-1-9 */
	unsigned long	ulSysNetId;                     /* NET ID used to identify rogue ap*/
	
    unsigned long ulCollectSwitch;			/* 2010-4-10 pyy 新增: 实时采集开关*/
    /* BEGIN: Added by lvjianlin, 2013/3/26   PN:BUG #2242  开启ulLinkOffRestartTime 字段 */
    /* BEGIN: Added by 吕建林, 2012/12/5   PN:与AC信息模型不对其，暂时删除该成员 */
    
    unsigned long  ulLinkOffRestartTime;     /* 2012-01-5 新增:
											   bug432 增加AP与AC断链后， AP重启的时间配置 */
    
    /* END:   Added by 吕建林, 2012/12/5   PN:与AC信息模型不对其，暂时删除该成员 */
    /* END:   Added by lvjianlin, 2013/3/26   PN:BUG #2242  开启ulLinkOffRestartTime 字段 */

    /* add by gwx 2012-10-31 for 用户流量不准确问题，默认值60，单位秒*/
    unsigned long   ulRptUserTrafficTime;
    /* END:   Added by 吕建林, 2012/11/9   PN:TS模块，AC计费流量不准确问题 */
	
	unsigned long	ulMOC;							/* 对象类标识 */
	unsigned char	aucMOI[AC_MOI_LEN];				/* 对象实例标识 */
	unsigned long	ulAdminState;
	unsigned long	ulOpState;
	unsigned long	ulCurAcId;
	unsigned long 	ulCfgVersion;                   /* 配置数据版本号 */
} T_sysBasicSetupAcFile;

typedef struct sysIpSetup_s
{
	unsigned long	ulWtpId;
	long			sysIpOrigin;
#define sysIpOrigin_MANUAL					1		/* 手工配置 */
#define sysIpOrigin_DHCP					2		/* DHCP配置 */
#define sysIpOrigin_AC_DHCP					3		/* AC分配 */
	unsigned char	sysIpAddr[4];
	unsigned char	sysNetMask[4];
	unsigned char	sysDefaultGw[4];
	unsigned char	aucPrimaryDns[4];				/* 主DNS服务器 */
	unsigned char	aucSecondaryDns[4];				/* 从DNS服务器 */
	unsigned long	ulMOC;							/* 对象类标识 */
	unsigned char	aucMOI[AC_MOI_LEN];				/* 对象实例标识 */
} T_sysIpSetup;

typedef struct sysKeeperSetup_s
{
	unsigned long	ulWtpId;
	
	long			sysKeepSwitch;					/* 是否启用守护 */
#define sysKeeperSwitch_OFF					0
#define sysKeeperSwitch_ON					1
	unsigned char	sysKeeperIp0[4];				/* 守护服务器的ip地址，设备将周期性ping该地址 */
	unsigned char	sysKeeperIp1[4];				/* 守护服务器的ip地址，设备将周期性ping该地址 */
	unsigned long	sysKeepInterval;				/* 秒 */
	unsigned long	sysKeepCount;					/* ping超时次数超过这个数时，认为守护失败 */
	unsigned long	isActionReboot;					/* 是否重新启动 */
	unsigned long	isActionDisableAp;				/* 是否断开无线接入 */
	unsigned long	isActionDisableEnet;			/* 是否断开有线接入 */
#define sysKeeperActionOff					0
#define sysKeeperActionOn					1
	int				iRsv[4];						/* 预留位 */
	unsigned long	ulMOC;							/* 对象类标识 */
	unsigned char	aucMOI[AC_MOI_LEN];				/* 对象实例标识 */
} T_sysKeeperSetup;

typedef struct sysServerSetup_s
{	
	unsigned long	ulWtpId;	
	unsigned char	sysFtpServerIpAddr[4];			/* 版本服务器IP地址 */
	unsigned long	sysFtpServerPort;				/* 版本服务器端口 */
	char			acFtpUserName[32];				/* FTP用户名 */
	char			acFtpPassword[32];				/* FTP密码 */
	char			acSwFileName[128];				/* 文件名 */
	char			acActivateSwVersion[32];		/* 待激活的软件版本 */
	unsigned long	ulUpgradeStrategy;				/* 下载速度 */
#define UPGRADE_STRATEGY_HIGH_SPEED	1				/* 高速 */
#define UPGRADE_STRATEGY_LOW_SPEED	2				/* 低速 */
#define UPGRADE_STRATEGY_BACKGROUND 3				/* 后台 */
	unsigned long	ulActivationTime;			    /* 生效时间(绝对时间) */
    /* hc modified for cmcc test 2009-7-18 */
    unsigned long   ulActivationPolicy;             /* 生效方式 */
	int				iRsv[4];						/* 预留位 */
	unsigned long	ulMOC;							/* 对象类标识 */
	unsigned char	aucMOI[AC_MOI_LEN];				/* 对象实例标识 */
} T_sysSwServerSetup;

/* OMC参数配置 */
typedef struct sysSnmpSetup_s
{
	unsigned char	sysSnmpTrapIp[4];				/* Snmp 上报IP地址 */
	unsigned long	sysSnmpTrapPort;				/* Snmp 上报端口 */
	char			sysSnmpCommunity[32];			/* Snmp 通信共同体 */
	int				iRsv[4];						/* 预留位 */
} T_sysSnmpSetup;

/* SnmpV2配置 */
typedef struct sysSnmpV2Setup_s
{
	char acCommunity[32];							/* 共同体 */
	unsigned long ulRfc1213Flag;					/* RFC1213标准MIB访问权限 */
#define PERMISSION_FLAG_NONE		0
#define PERMISSION_FLAG_READ_ONLY	1
#define PERMISSION_FLAG_READ_WRITE	2
	unsigned long ulRfc1573Flag;					/* RFC1573标准MIB访问权限 */
	unsigned long ulRfc1643Flag;					/* RFC1643标准MIB访问权限 */
	unsigned long ulIeee8021PAEFlag;				/* Ieee8021PAE标准MIB访问权限 */
	unsigned long ulIeee8021dot11Flag;				/* Ieee8021dot标准MIB访问权限 */
	unsigned long ulPrivateFlag;					/* 私有MIB访问权限 */
	unsigned long aulResv[4];
} T_sysSnmpV2Setup;

#define snmpV2TableSize_M   4
typedef T_sysSnmpV2Setup T_sysSnmpV2Table[snmpV2TableSize_M];


/* SnmpV3配置 */
typedef struct sysSnmpV3Setup_s
{
	char acUserName[32];							/* 用户名 */
	unsigned long ulAuthProtocol;					/* 鉴权协议 */
#define SNMPV3_AUTH_PROTOCOL_NONE       0
#define SNMPV3_AUTH_PROTOCOL_HMAC_MD5   1
#define SNMPV3_AUTH_PROTOCOL_HMAC_SHA   2
	char acAuthKey[32];								/* 鉴权密钥 */
	unsigned long ulPrivProtocol;					/* 加密协议 */
#define PRIV_PROTOCOL_NONE		0
#define PRIV_PROTOCOL_CBC_DES	1
	char acPrivKey[32];								/* 加密密钥 */
	unsigned long ulRfc1213Flag;					/* RFC1213标准MIB访问权限 */
	unsigned long ulRfc1573Flag;					/* RFC1573标准MIB访问权限 */
	unsigned long ulRfc1643Flag;					/* RFC1643标准MIB访问权限 */
	unsigned long ulIeee8021PAEFlag;				/* Ieee8021PAE标准MIB访问权限 */
	unsigned long ulIeee8021dot11Flag;				/* Ieee8021dot标准MIB访问权限 */
	unsigned long ulPrivateFlag;					/* 私有MIB访问权限 */
	unsigned long aulResv[4];
} T_sysSnmpV3Setup;

#define snmpV3TableSize_M   4
typedef T_sysSnmpV3Setup T_sysSnmpV3Table[snmpV3TableSize_M];

typedef struct sysVersionSetup_s
{
    unsigned long   neType;                         /* 网元类型 */
#define neType_AP           0						/* AP */
#define neType_INVALID      0xFFFFFFFF			    /* 无效 */
    char            cfgImVersion[16];               /* 配置信息模型版本，同配置数据文件头中的dataVersion */
    unsigned long   configVersion;                  /* 配置数据版本 */
    char            alarmImVersion[16];             /* 告警信息模型版本 */
    char            perfImVersion[16];              /* 性能信息模型版本 */
	unsigned long	ulWtpId;
	char			sysSwVersion[32];				/* 软件版本 */
	int				iRsv[4];						/* 预留位 */
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
	unsigned long	ulMOC;							/* 对象类标识 */
	unsigned char	aucMOI[AC_MOI_LEN];				/* 对象实例标识 */
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
	unsigned long ulMOC;							/* 对象类标识 */
	unsigned char aucMOI[AC_MOI_LEN];				/* 对象实例标识 */
} T_sysLogSetup;

typedef struct sysSntpSetup_s
{
	unsigned long ulWtpId;
	unsigned long ulEnableSntp;
	unsigned char aucSntpServerIp[4];
	unsigned long ulPeriod;
	unsigned long aulResv[4];
	unsigned long ulMOC;							/* 对象类标识 */
	unsigned char aucMOI[AC_MOI_LEN];				/* 对象实例标识 */
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
	unsigned long ulMOC;							/* 对象类标识 */
	unsigned char aucMOI[AC_MOI_LEN];				/* 对象实例标识 */
} T_sysPPPoESetup;

typedef struct
{
	unsigned long ulHeartBeatPeriod;            /* 心跳trap周期（单位：秒）*/
	int	iRsv[4];								/* 预留位 */
} T_sysParaSetup;

typedef struct sysPasswordChange_s
{
	char oldPassword[32];
	char newPassword[32];
	int	iRsv[4];								/* 预留位 */
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

/* 通过FTP下载更新配置文件 */
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

/* 维护记录 */
typedef struct enregisterFileHeader_s
{
	unsigned short	headLen;					/* bytes of head data */
	unsigned short	dataLen;					/* bytes of file data */
	long			dataState;
	//#define dataState_NONE 	0X11111111
	//#define dataState_VALID 0X22222222
	int				iRsv[4];					/* 预留位 */
} T_enregisterFileHeader;

typedef struct sysPeriodOfValidity_s
{
	unsigned long	sysPeriodSwitch;
#define sysPeriodSwitch_ON		1
#define sysPeriodSwitch_OFF		0
	unsigned long	sysUseTime;					/* 系统上电使用的总秒数 */
	unsigned long	sysOverdueTime;				/* 系统过期秒数 */
	int				iRsv[4];					/* 预留位 */
} T_sysPeriodOfValidity;

typedef struct
{
	unsigned long sysPeriodSwitch;
	unsigned long sysUseTime; /* 系统上电使用的总秒数 */
	unsigned long sysOverdueTime; /* 系统过期秒数 */
	char acKey[64 + 1];
} T_registerSetup;

typedef struct sysTimer_s
{
	unsigned short	timerHandStatus;
#define  timerHandStatus_SYSPERIOD			0
#define  timerHandStatus_GATEWAYMONITOR		1
#define  timerHandStatus_OMA_TEST			2
#define  sysTimerStatus_RESETCONFIG			3	/* 长按复位键配置 */
	int				iRsv[4];					/* 预留位 */
}T_TIMER;

typedef struct wifiDevice_s
{
	unsigned long	wifiDeviceId;				/* 无线设备编号 */
	char			wifiDeviceName[32];			/* 无线设备名称 */
	int				iRsv[4];					/* 预留位 */
} T_wifiDevice;
typedef T_wifiDevice T_wifiDeviceAdd;
typedef T_wifiDevice T_wifiDeviceDelete;

typedef struct wifiDeviceBasicSetup_s
{	
	unsigned long ulWtpId;
	unsigned long	wifiDeviceIndex;						/* 设备编号 */
	char			wifiDeviceInfo[32];						/* 设备描述 */
	unsigned long	wifiDeviceType;							/* 设备类型 */
	unsigned long ulMaxStaNum;                              /* 最大允许接入用户数 */
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
	unsigned long	ulRunMode;								/* 运行模式 add by yzl 2008-12-05 */
#define RUN_MODE_STA	0
#define RUN_MODE_ADHOC	1
	unsigned long ulWifiModuleNum;					        /* 无线模块数*/
	unsigned long	wifiModuleId[wifiModule_SIZE];			/* 模块ID列表 */
	unsigned long	wifiModuleType[wifiModule_SIZE];		/* 模块类型列表 */
#define wifiModuleType_ALONE				0
#define wifiModuleType_MAIN					1
#define wifiModuleType_SUB					2
	int				iRsv[4];							    /* 预留位 */
	unsigned long ulMOC;							        /* 对象类标识 */
	unsigned char aucMOI[AC_MOI_LEN];				        /* 对象实例标识 */
}T_wifiDeviceBasicSetup;

typedef struct wifiDeviceChanSetup_s
{
	unsigned long ulWtpId;
	unsigned long	wifiDeviceIndex;						/* 设备编号 */
	unsigned long	ulAbgMode;							    /* A/B/G模式 */
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


	unsigned long	ulChannelMode;						    /* 信道模式 */
#define CHANNEL_MODE_FULL					0
#define CHANNEL_MODE_HALF 					1
#define CHANNEL_MODE_QUARTER				2
	unsigned long	ulTurboMode;						    /* Turbo模式 */
#define TURBO_MODE_DISABLE	0
#define TURBO_MODE_ENABLE	1
	unsigned long	ulChannelState;						    /* 工作信道模式 */
#define CHANNELMODE_STATIC					0
#define CHANNELMODE_AUTO					1
	unsigned long	wifiModuleChannelList[channel_SIZE];    /* 信道列表 */
	//对于b/g模式，可取以下信道
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
	//对于a模式，可取以下信道,中国
#define wifiChannel_CH149 					149
#define wifiChannel_CH153 					153
#define wifiChannel_CH157 					157
#define wifiChannel_CH161 					161
#define wifiChannel_CH165 					165
	//对于a模式，可取以下信道,德国
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
	unsigned long	ulFrequPointQualitySwitch;		/* 信道重选周期 */
	unsigned long	ulRssiThreshold;				/* 信道重选RSSI门限 */
	long			lNoiseThreshold;				/* 信道重选底噪门限 */

	unsigned long	wifiModuleABGModeType;			/* A/B/G列表类型，动态字段 */
#define wifiAbgType_ABGM 0
#define wifiAbgType_BGM 1
#define wifiAbgType_ABG 2
#define wifiAbgType_A 3

	/* 2010-7-27 11n新增*/
#define BAND_20M			1
#define BAND_40M			2	
	unsigned long	ulBand;	/* 频宽 	1：20MHz
										2：20/40MHz（自动）
 										1：20MHz
						*/
	unsigned long	ulRFNum;	/*  空间流数量 1~4 */
	unsigned long	ulShortGI;	/* SGI功能开关
 							0：禁用
							1：启用*/
	unsigned long	ulAMPDU;	/* A-MPDU功能开关
 							0：禁用
							1：启用
							*/
	unsigned long	ulAMSDU;	/* A-MSDU功能开关
	 						 0：禁用
							1：启用
							*/
 	unsigned long	ulOnly11n;	/* 仅11n用户接入功能
 							 0：禁用
							1：启用
							 启用时，只有11n终端可以关联，非11n终端无法关联
							*/
						

	int				iRsv[4];						/* 预留位 */
	unsigned long ulMOC;							/* 对象类标识 */
	unsigned char aucMOI[AC_MOI_LEN];				/* 对象实例标识 */
}T_wifiDeviceChanSetup;

typedef struct wifiDeviceRateSetup_s
{
	unsigned long   ulWtpId;
	unsigned long	wifiDeviceIndex;    /* 设备编号 */
    /* BEGIN: Added by 吕建林, 2012/12/27   PN:TASK #336 无线速率配置的AC侧相关实现 */
    /* 表里去掉了对最大发送速率的配置，因为这个配置目前仅在ixp平台里有驱动接
       口，而且没什么特别需求 */
    //unsigned long	ulMaxTxRate;	    /* 最大发送速率 */
    unsigned long   ulStaUpRateMin;     /* 终端上行发包速率最低门限 */
    unsigned long   ulStaDownRateMin;   /* 终端下行收包速率最低门限 */
    /* END:   Added by 吕建林, 2012/12/27   PN:TASK #336 无线速率配置的AC侧相关实现 */
	
	unsigned long	ulMgmTrate;		    /* 优选管理帧发送速率  */
	unsigned long	ulMcasTrate;	    /* 优选广播发送速率  */
	unsigned long	ulMgmtRetrys;	    /* 管理帧重传次数  */
	unsigned long	ulRsv[4];		    /* 预留 */
	unsigned long   ulMOC;				/* 对象类标识 */
	unsigned char   aucMOI[AC_MOI_LEN];	/* 对象实例标识 */
}T_wifiDeviceRateSetup;

typedef struct wifiDevicePowerSetup_s
{
	unsigned long ulWtpId;
	unsigned long	wifiDeviceIndex;			 /* 设备编号 */
	unsigned long	ulLimitTxPower;	             /* 最大发射功率 */
	unsigned long	ulMaxTxPower;	             /* 功率衰减配置 */
#define MAX_TX_POWER_PER_5			0
#define MAX_TX_POWER_PER_12			1
#define MAX_TX_POWER_PER_20			2
#define MAX_TX_POWER_PER_25			3
#define MAX_TX_POWER_PER_30			4
#define MAX_TX_POWER_PER_50			5
#define MAX_TX_POWER_PER_80			6
#define MAX_TX_POWER_PER_100		7
	unsigned long	ulInterfereDetectSwitch;	/* 发射功率自动调整  */
	unsigned long	ulInterfereDetectPeriod;
	unsigned long	ulMaxInterferePermitted;
	unsigned long	ulRssiWaveRange;
	unsigned long	ulPowerCompensationSwitch;
	unsigned long	ulRsv[4];		            /* 预留 */
	unsigned long ulMOC;						/* 对象类标识 */
	unsigned char aucMOI[AC_MOI_LEN];			/* 对象实例标识 */
}T_wifiDevicePowerSetup;

typedef struct wifiDeviceAdvancedSetup_s
{
	unsigned long   ulWtpId;
	unsigned long	wifiDeviceIndex;			/* 设备编号 */
	unsigned long	wifiBeaconInterval;			/* BEACON间隔 */
	unsigned long	wifiDtimInterval;			/* DTIM间隔 */
	unsigned long	wifiPreambleType;			/* 前导类型 */
#define wifiPreambleType_AUTO				1
#define wifiPreambleType_SHORT				2
#define wifiPreambleType_LONG				3
	unsigned long	wifFragmentationThreshold;	/* 分片门限 */
	unsigned long	wifiRtsThreshold;			/* RTS门限 */
	unsigned long	wifiCtsProtectionMode;		/* CTS保护模式 */
#define wifiCtsProtectionMode_DISABLE		0
#define wifiCtsProtectionMode_ENABLE		1
	unsigned long	ulTxDistOptSwitch;			/* 传输距离开关 */
#define txDistOptSwitch_DISABLE	0
#define txDistOptSwitch_ENABLE	1
	unsigned long	ulMaxTxDistance;			/* 最大传输距离 */
       unsigned long ulLongRtsThreshold;        /* 长帧重传门限 */
	unsigned long ulShortRtsThreshold;			/* 短帧重传门限 */
	unsigned long ulCacheTime;                  /* 接受数据包生存期 */
	int				iRsv[4];					/* 预留位 */
	unsigned long ulMOC;						/* 对象类标识 */
	unsigned char aucMOI[AC_MOI_LEN];			/* 对象实例标识 */
}T_wifiDeviceAdvancedSetup;

typedef struct wifiDeviceResourceSetup_s
{
	unsigned long   ulWtpId;
	unsigned long	wifiDeviceIndex;				  /* 设备编号 */
	unsigned long   ulMoniMode;                       /* 无线环境监测模式 */                             
	unsigned long	aulDiffScanChannelList[16];
	unsigned long	ulDiffScanInterval;
	unsigned long	ulDiffScanResidenceTime;
	unsigned long	ulSameScanInterval;
	/* hc modified for cmcc test 2009-7-18 */
	unsigned long   ulNeighAutoScanSwitch;            /* 启用邻居自动扫描 */
	unsigned long   ulNeighRssiMin;			          /* 邻居信号强度最小门限 */
	unsigned long   ulNeighRssiWave;                  /* 邻居信号强度波动范围 */
	unsigned long   aulResv[4];
	unsigned long   ulMOC;							  /* 对象类标识 */
	unsigned char   aucMOI[AC_MOI_LEN];				  /* 对象实例标识 */
}T_wifiDeviceResourceSetup;

typedef struct wifiDeviceEntry_s
{
	unsigned long				wifiDeviceId;				/* 设备ID */
	T_wifiDeviceBasicSetup		wifiDeviceBasicSetup;		/* 无线基本配置 */
	T_wifiDeviceChanSetup		stChanSetup;				/* 无线信道配置 */
	T_wifiDeviceRateSetup		stRateSetup;				/* 无线速率配置 */
	T_wifiDevicePowerSetup		stPowerSetup;				/* 无线功率配置 */
	T_wifiDeviceAdvancedSetup	wifiDeviceAdvancedSetup;	/* 无线高级配置 */
	/* hc modified for telecom test 2009-7-6 */
	WIDS_CONFIG_T               stWifiDeviceWidsSetup;      /* WIDS参数配置 */
	T_wifiDeviceResourceSetup	stWifiDeviceResSetup;		/* 无线环境监测 */
	int							iRsv[4];					/* 预留位 */
} T_wifiDeviceEntry;

#define wifiDeviceTable_SIZE wifiModule_SIZE
typedef T_wifiDeviceEntry T_wifiDeviceTable[wifiDeviceTable_SIZE];
#define wifiDeviceList_SIZE wifiDeviceTable_SIZE
typedef T_wifiDevice T_wifiDeviceList[wifiDeviceList_SIZE];

/* BEGIN: Added by 吕建林, 2012/12/27   PN:TASK #336 无线速率配置的AC侧相关实现 */
typedef T_wifiDeviceRateSetup T_wifiDeviceRateSetupList[wifiModule_SIZE];
/* END:   Added by 吕建林, 2012/12/27   PN:TASK #336 无线速率配置的AC侧相关实现 */

typedef unsigned long T_wifiDeviceValidWifiList[wifiModule_SIZE];
typedef unsigned long T_apValidWifiDeviceList[wifiDeviceList_SIZE];
typedef unsigned long T_adhocValidWifiDeviceList[wifiDeviceList_SIZE];

typedef struct routeBasicSetup_s
{
	unsigned long ulWtpId;
	
	unsigned long ulNodeGwRunMode;				/* 网关功能 */
#define NODE_GW_RUN_MODE_DISABLE	0
#define NODE_GW_RUN_MODE_ENABLE		1
	unsigned long ulNetDiameter;				/* 最大跳数 */
	unsigned char aucRouteFavorFatherMac[6];	/* 优选父节点 */
	char acRsv1[2];
	unsigned long ulRouteFavorMinCost;			/* 优选父节点的最低信号强度 */
	unsigned char aucSysDeviceId[6];			/* 6个16进制字符 */
	char acRsv[2];
	unsigned long aulResv[4];
	unsigned long ulMOC;						/* 对象类标识 */
	unsigned char aucMOI[AC_MOI_LEN];			/* 对象实例标识 */
} T_routeBasicSetup;

typedef struct routerNeighSetup_s
{
	unsigned long ulWtpId;
	
	unsigned long	routeMinNeighborPathcost;	/* 邻居质量门限下限 */
	unsigned long	routeHelloInterval;			/* HELLO间隔 */
	unsigned long	routeHelloCombine;			/* HELLO合并 */
#define HELLO_COMBINE_DISABLE				0
#define HELLO_COMBINE_ENABLE				1
	unsigned long	routeDiffNeighScanThreshold;/* 异频邻居扫描门限（路径质量） */
	unsigned long	routeDiffNeighScanInterval;	/* 异频邻居扫描间隔 */
	int				iRsv[4];					/* 预留位 */
	unsigned long ulMOC;						/* 对象类标识 */
	unsigned char aucMOI[AC_MOI_LEN];			/* 对象实例标识 */
} T_routerNeighSetup;

typedef struct routerHandoverSetup_s
{
	unsigned long ulWtpId;
	
	unsigned long	routeHigh4father;			/*  合法父节点质量上限 */
	unsigned long	routeLow4father;			/*  合法父节点质量下限 */
	unsigned long	routeHoldQuality;			/* 父节点切换保护质量 */
	unsigned long	routeHoldTime;				/* 父节点切换保护时间 */
	unsigned long	routeDuxPathCost;			/* 链路质量类型 */
#define PATH_COST_FRONT						0
#define PATH_COST_DOUBLE					1
	int				iRsv[4];					/* 预留位 */
	unsigned long ulMOC;						/* 对象类标识 */
	unsigned char aucMOI[AC_MOI_LEN];			/* 对象实例标识 */
} T_routerHandoverSetup;

typedef struct routerMscSetup_s
{
	unsigned long ulWtpId;
	
	unsigned long	routeMsRunMode;				/*	是否启用设备主备 */
#define routeMsRunMode_DISABLE				0
#define routeMsRunMode_ENABLE				1
	unsigned long	routeMsDeviceName;			/*  用于发送主备间控制报文的网络设备名 */
#define routeMsName_IXP0					0	/*  0=允许主备间转发 */
#define routeMsName_IXP1					1	/*  1=禁止主备间转发 */
	unsigned long	routeDataDeviceName;		/*  用于发送主备间数据报文的网络设备名 */
#define routeDataName_IXP0					0	/*  0=允许主备间转发 */
#define routeDataName_IXP1					1	/*  1=禁止主备间转发 */
	unsigned long	routeMsRole;				/*  主从角色 */
#define routeMsRole_AUTO                    0
#define routeMsRole_MASTER                  1
#define routeMsRole_SLAVE                   2
	unsigned long	routeForwardForbidden;		/* 主从方式 */
#define routeMsType_LOAD_BALANCE            0   /* 负荷分担 */
#define routeMsType_MASTER_SLAVE            1   /* 主备方式 */

	unsigned long	routeMaxMsQueryLoss;		/* 保活时长 */
	unsigned long	routeMinimalAirbandQual;	/* 无线质量下限 */
	unsigned long	routeWifiAdhocBandWidth;	/* 无线传输带宽 */
	int				iRsv[4];					/* 预留位 */
	unsigned long ulMOC;						/* 对象类标识 */
	unsigned char aucMOI[AC_MOI_LEN];			/* 对象实例标识 */
} T_routerMscSetup;

typedef struct aclPolicySwitch_s
{
	unsigned long ulPolicySwitch;				/* ACL开关 */
#define aclPolicySwitch_OFF					0	/* 关闭：关闭情况下，缺省策略不可配 */
#define aclPolicySwitch_ON					1	/* 开启：缺省值 */
	unsigned long ulDefaultPolicy;				/* 缺省策略 */
#define ACL_DEFUALT_POLICY_DENY				0	/* 禁止通过 */
#define ACL_DEFUALT_POLICY_PASS				1	/* 允许通过：缺省值 */
	unsigned long aulRsv[4];
} T_aclPolicySwitch;

#define ACL_SETUP_ENTRY_MAX_SIZE	32
typedef struct
{
	unsigned long ulFilterIndex;				/* 索引:唯一标志，从0开始递增 */
	unsigned long ulFilterType;					/* 协议类型 */
#define FILTER_TYPE_IP				0			/* IP地址 */
#define FILTER_TYPE_ENET_PRO		1			/* 以太网协议 */
#define FILTER_TYPE_IP_PRO			2			/* IP协议 */
#define FILTER_TYPE_IP_PRO_PORT		3			/* IP协议端口 */
	unsigned long ulDefaultPolicy;				/* 通过 */
#define DEFUALT_POLICY_DENY			0			/* 禁止通过 */
#define DEFUALT_POLICY_PASS			1			/* 允许通过 */
	unsigned long ulProtocol;					/* 协议 */
	unsigned long ulPort;						/* 端口 */
	unsigned char aucIpAddr[4];					/* IP地址 */
	unsigned char aucNetMask[4];				/* 子网掩码 */
	char acFilterName[32];						/* 规则名称 */
	unsigned long ulValid;                      /*是否生效*/
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

/* 流量控制 */
typedef struct fclBasicSetup_s
{
	unsigned long ulMulticastSwitch;     	/* 允许多播组*/
#define MULTICAST_SWITCH_DISABLE		0   /* 否*/
#define MULTICAST_SWITCH_ENABLE			1   /* 是：缺省为是*/
	unsigned long ulBroadcastSwitch;     	/* 允许广播（除ARP，DHCP）*/
#define BROADCAST_SWITCH_DISABLE		0   /* 否*/
#define BROADCAST_SWITCH_ENABLE			1   /* 是：缺省为是*/
	unsigned long ulBMRateLimitSwitch;      /* 广播/多播 是否限速*/
#define BM_RATE_LIMIT_SWITCH_DISABLE    0   /* 否*/
#define BM_RATE_LIMIT_SWITCH_ENABLE     1   /* 是：缺省为是*/
	unsigned long ulMaxRate;                /* 最大速率：个/秒；缺省为*/
	unsigned long	ulFclSwitch;	        /* 流控开关 */
#define FCL_SWITCH_DISABLE	0		        /* 禁用 */
#define FCL_SWITCH_ENABLE	1		        /* 启用 */
	unsigned long	ulUserMaxFlow;	        /* 单用户最大流量，单位为kbit/s */
	int				iRsv[4];		        /* 预留位 */
} T_fclBasicSetup;

/* IP转发 */
typedef struct ipForwardSetup_s
{
	unsigned long ulIpForwardSwitch;		/* 是否启用IP转发 */
#define FORWARD_SWITCH_DISABLE			0
#define FORWARD_SWITCH_ENABLE			1
	unsigned long aulResv[4];
} T_ipForwardSetup;

/* TCP优化 */
#define SERVER_PORT_MAX_NUM				16
#define INVALID_SERVER_PORT				0
typedef struct tcpOptimizationSetup_s
{
	unsigned long ulOptEnable;				/* 是否启用TCP优化功能 */
	unsigned long ulL2OptEnable;			/* 是否支持层2 */
#define OPT_ENABLE_NO					0
#define OPT_ENABLE_YES					1
	unsigned long ulL3OptEnable;			/* 是否支持层3 */
	unsigned long ulOptiMode;				/* 优化方式 */
#define optiMode_SERVICE                0   /* 业务平滑优先 */
#define optiMode_BAND                   1   /* 带宽优先 */
#define optiMode_BAND_CSV               2   /* 带宽优先（保守） */
	unsigned long ulConnTimeout;			/* TCP连接超时时间 */
	unsigned short ausServerPort[SERVER_PORT_MAX_NUM];	/* 端口号 */
	unsigned long aulResv[4];
} T_tcpOptimizationSetup;

#define aodvNeighborList_SIZE 32
typedef struct aodvNeighbor_s
{
	unsigned char	aodvNeighborMac[6];		/* 全零表示无效 */
	unsigned char	aodvNeighborIp[4];
	unsigned long	aodvConnectQuality;		/* 连接品质(新增字段) */
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
	int				iRsv[4];				/* 预留位 */
} T_aodvNeighbor;
typedef T_aodvNeighbor T_aodvNeighborList[aodvNeighborList_SIZE];

#define aodvRouteList_SIZE 128
typedef struct aodvRoute_s
{
	unsigned char	aodvRouteDstMac[6];		/* 全零表示无效 */
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
	int				iRsv[4];				/* 预留位 */
} T_aodvRoute;
typedef T_aodvRoute T_aodvRouteList[aodvRouteList_SIZE];

typedef struct apVap_s
{
	char			apSsid[32];
	unsigned long	apId;
	unsigned long	wifiDeviceId;			/* 所属无线设备ID */
	unsigned long	ulPrivCap;				/* 是否开启能力信息 */
	int				iRsv[4];				/* 预留位 */
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

#define VAP_TRAFFICMODE_CENTFORWARD		1		/* 集中转发*/
#define VAP_TRAFFICMODE_LOCALFORWARD	2		/* 本地转发*/

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
	unsigned long	ulEndUserMaxNum;	      /* 最大用户接入数 */
    //unsigned long	ulMaxSimultUsers;        /* 最大并发用户数 */
    unsigned long	ulMaxSimultTraffic;         	/* 最大流量 */

    /* BEGIN: Added by 吕建林, 2012/12/10   PN:TASK #257 VAP级流控配置功能 商用版新增 */
    unsigned long   ulTotalUpBandMax;   /*上行总带宽控制,单位：kbps*/
    unsigned long   ulTotalDownBandMax; /*下行总带宽控制,单位：kbps*/
    /* END:   Added by 吕建林, 2012/12/10   PN:TASK #257 VAP级流控配置功能 商用版新增 */

	/* 2010-4-19 pyy 新增*/
    unsigned long ulTrafficMode;			/* 业务转发模式1: 集中转发 2:本地转发 */
    unsigned long ulApTrafficVlan;			/* 业务VLAN 0表示不打vlan tag */

    /* 2010-6-24 pyy 新增*/
    unsigned long ulPrdOff;     /* 是否定期关闭 */
	unsigned long ulStartTime;  /* 起始时间 */
	unsigned long ulEndTime;    /* 结束时间 */
    /* BEGIN: Added by 吕建林, 2013/1/8   PN:tast#360  beacon广播抑制 */
    /* 无线启用开关，默认为启用 AC_WEB 默认开启为 1  无线驱动默认开启为 0 需要翻转*/
    unsigned long ulWifiEnable;
    /* END:   Added by 吕建林, 2013/1/8   PN:tast#360  beacon广播抑制 */
	int				iRsv[4];			        /* 预留位 */
	unsigned long ulMOC;							/* 对象类标识 */
	unsigned char aucMOI[AC_MOI_LEN];				/* 对象实例标识 */
} T_apBasicSetup;

/* BEGIN: Added by 吕建林, 2012/12/6   PN:TASK #257 VAP级流控配置功能 商用版新增 */
typedef 	struct	apVapMacFlowCtrlSetup
{
	unsigned long	ulWtpId;				    /*AP设备ID */
	unsigned long   ulWifiDeviceId;			    /*无线设备编号*/
	unsigned long	ulVapId;				    /* VAP ID */
	unsigned char	aucMac[6];				    /*用户MAC */
	unsigned char	aucResv[2];				    /*保留字段，为了四字节对齐*/
	unsigned long	ulUserUpBandMax;		    /*上行用户最大带宽, kbps ,缺省值1024*/
	unsigned long	ulUserDownBandMax;		    /*下行用户最大带宽，kbps ,缺省值1024*/
	unsigned long	ulMOC;					    /*对象类标识，网管自动生成,下发到网元*/
	unsigned char	aucMOI[AC_MOI_LEN];	        /*对象实例标识，网管自动生成,下发到网元*/
}T_apVapMacFlowCtrlSetup;

/* END:   Added by 吕建林, 2012/12/6   PN:TASK #257 VAP级流控配置功能 商用版新增 */

/* BEGIN: Added by 吕建林, 2013/1/10   PN:TASK #257  -2 VAP-MAC用户流控表更新 */
typedef struct  
{
	unsigned long   ulWifiDeviceId;			    /*无线设备编号*/
	unsigned long	ulVapId;				    /* VAP ID */
	unsigned char	aucMac[6];				    /*用户MAC */
	unsigned char	aucResv[2];				    /*保留字段，为了四字节对齐*/
	unsigned long	ulUserUpBandMax;		    /*上行用户最大带宽, kbps ,缺省值1024*/
	unsigned long	ulUserDownBandMax;		    /*下行用户最大带宽，kbps ,缺省值1024*/
    unsigned long   ulBeUsed;                   /* 该表项目是否被占用 */
}T_apVapMacFlowCtrlCount;
/* END:   Added by 吕建林, 2013/1/10   PN:TASK #257  -2 VAP-MAC用户流控表更新 */

/* BEGIN: Added by lvjianlin, 2013/6/20   PN:lte-fi  GPS ALARM  LOCAL PORTAL */
#define     LTEFI_ACCESSMODE_TD_LTE_FIRST       0   /* TD_LTE优先 */
#define     LTEFI_ACCESSMODE_TD_SCDMA_FIRST     1   /* TD_SCDMA优先 */
#define     LTEFI_ACCESSMODE_TD_LTE_ONLAY       2   /* 仅TD_LTE */
#define     LTEFI_ACCESSMODE_TD_SCDMA_ONLAY     3   /* 仅TD_SCDMA */
#define     LTEFI_ACCESSMODE_GSM_FIRST          4   /* GSM优先 */
#define     LTEFI_ACCESSMODE_GSM_ONLAY          5   /* 仅GSM */

typedef struct  ltefiBasicSetup
{
    unsigned long   ulWtpId;                /* WTP  id */
    unsigned char   acLtefiApn[32];         /* 接入点名称 */
    unsigned long   ulLtefiMtu;             /* MTU值 */
    unsigned long   ulAccessMode;           /* LTE-FI接入模式 */
    unsigned long   ulBatterySptTime;       /* 电池供电时间 */
    unsigned char   acBusNo[32];            /* 公交编号 */
    unsigned char   aucApForwardBrIp[4];    /* 先本地后集中的情况下的网关地址 */
    unsigned char   aucApForwardBrMask[4];  /* 先本地后集中的情况下的网关地址掩码 */
    unsigned char   acGpsServer1[64];       /* GPS信息上报服务器1 */
    unsigned char   acGpsServer2[64];       /* GPS信息上报服务器2 */
    unsigned long   ulGpsReportPeriod;      /* GPS信息上报周期 add by liyoukun*/
    unsigned char   acContextServer1[128];  /* LTE-Fi上内容下载服务器1 */
    unsigned char   acContextServer2[128];  /* LTE-Fi上内容下载服务器2 */
    unsigned char   acLtefiPortalIp[64];    /* ltefi设备本地portalIP地址 add by liyoukun*/
    /* BEGIN: Added by lvjianlin, 2013/7/11   PN:ulDataUpdateTime */
    unsigned long   ulDataUpdateTime;       /* 定期更新本地存储空间内容 */
    /* END:   Added by lvjianlin, 2013/7/11   PN:ulDataUpdateTime */
    /* BEGIN: Added by lvjianlin, 2013/7/25   PN:lte-fi DT */
    unsigned char   acDTUserName[64];       /* 路测服务器登陆用户名 */
    unsigned char   acDTPass[64];           /* 路测服务器登陆密码 */
    unsigned char   acDTServerIp[64];       /* 路测服务器IP */
    unsigned long   ulDTPort;               /* 路测服务器端口 */
    unsigned long   ulDTEnable;             /* 路测服务是否启用 */
    unsigned long   ulDTCollectInterval;    /* 路测信息采集间隔 */
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
	unsigned long	ul8021xMode;			/* 是否启用8021.x */
#define WPA_MODE_DISABLE	0
#define WPA_MODE_ENABLE		1
	unsigned long	ulEapolVer;				/* EAP版本 */
#define EAPOL_VER_1			1
#define EAPOL_VER_2			2
	unsigned long	ulEapAuthPeriod;		/* EAP鉴权周期 */
	unsigned long	ulWepBroadcastKeyLen;	/* 动态WEP广播密钥长度 */
#define WEP_BROADCAST_KEY_LEN_64BIT		1
#define WEP_BROADCAST_KEY_LEN_128BIT	2
	unsigned long	ulWepUnicastKeyLen;		/* 动态WEP点播密钥长度 */
#define WEP_UNICAST_KEY_LEN_64BIT WEP_BROADCAST_KEY_LEN_64BIT
#define WEP_UNICAST_KEY_LEN_128BIT WEP_BROADCAST_KEY_LEN_128BIT
	unsigned long	ulWepRekeyPeriod;		/* WEP密钥更新周期 */
	int				iRsv[4];				/* 预留位 */

	unsigned long ulMOC;							/* 对象类标识 */
	unsigned char aucMOI[AC_MOI_LEN];				/* 对象实例标识 */
} T_apSecuritySetup;

typedef struct radiusSerCfg_s
{
	unsigned char aucAuthSrvAddrMaster[4];	/* 主用认证服务器IP */
	unsigned long ulAuthSrvPortMaster;		/* 主用认证服务器端口 */
	char acAuthSrvSecretMaster[32];			/* 主用认证服务器接入密码 */
	unsigned char aucAuthSrvAddrSlave[4];	/* 备用认证服务器IP */
	unsigned long ulAuthSrvPortSlave;		/* 备用认证服务器端口 */
	char acAuthSrvSecretSlave[32];			/* 备用认证服务器接入密码 */
	unsigned char aucAcctSrvAddrMaster[4];	/* 主用计费服务器IP */
	unsigned long ulAcctSrvPortMaster;		/* 主用计费服务器端口 */
	char acAcctSrvSecretMaster[32];			/* 主用计费服务器接入密码 */
	unsigned char aucAcctSrvAddrSlave[4];	/* 备用计费服务器IP */
	unsigned long ulAcctSrvPortSlave;		/* 备用计费服务器端口 */
	char acAcctSrvSecretSlave[32];			/* 备用计费服务器接入密码 */
	unsigned long ulRetryPrimaryInterval;	/* 主用服务器重连间隔 */
	unsigned long ulAcctInterimInterval;	/* 计费信息上报间隔 */
	int iRsv[4];
} T_radiusSerCfg;

typedef struct apAdvancedSetup_s
{
	unsigned long 	ulWtpId;
	unsigned long	wifiDeviceId;		
	unsigned long	apVapId;
	
		unsigned long ulPrivCap;			/* VLAN属性 */
#define PRIV_CAP_USER_PORT		0
#define PRIV_CAP_TRUNK_PORT		1
		unsigned long ulPowerCtrl;			/* 包级功率控制 */
#define POWER_CTRL_DISABLE		0
#define POWER_CTRL_ENABLE		1
		unsigned long ulEndUserTime;		/* 用户保活时间 */
		unsigned long ulWmmQos;				/* WMM功能 */
#define WMM_QOS_DISABLE			0
#define WMM_QOS_ENABLE			1
		unsigned long ulQosType;			/* QoS类型 */
#define QosType_DATA_BG         0           /* 后台数据业务 */
#define QosType_DATA_ALT        1           /* 交互数据业务 */
#define QosType_DATA_VEDIO      2           /* 视频业务 */
#define QosType_DATA_AUDIO      3           /* 语音业务 */
#define QosType_DATA_AUTO       8           /* 自动 */
		unsigned long ulAcLowLimit;			/* 用户接入下门限（RSSI） */
		unsigned long ulAcUpLimit;			/* 用户接入上门限(RSSI) */
        /* BEGIN: Added by 吕建林, 2012/11/28   PN:商用版支持低速率用户接入 */
        unsigned long  ulRxRateLimit;              /*用户接入AP下行速率限制*/
        unsigned long  ulTxRateLimit;              /*用户接入AP上行速率限制*/
        /* END:   Added by 吕建林, 2012/11/28   PN:商用版支持低速率用户接入 */

		unsigned long ulAcInterval;			/* 用户接入控制间隔 */

	unsigned long ulMOC;							/* 对象类标识 */
	unsigned char aucMOI[AC_MOI_LEN];				/* 对象实例标识 */
} T_apAdvancedSetup;

#define apMacFilterList_SIZE 64
typedef struct apMacFilterSetup_s
{
	long			apMacFilterSwitch;		/* mac过滤功能的开关 */
#define apMacFilterSwitch_OFF 0
#define apMacFilterSwitch_ON 1
	long			apMacFilterDefaultPolicy;
#define apMacFilterDefaultPolicy_DENY 0
#define apMacFilterDefaultPolicy_PASS 1
	unsigned char	apMacFilterList[apMacFilterList_SIZE][6];
	int				iRsv[4];				/* 预留位 */
} T_apMacFilterSetup;

typedef struct apMacFilterStat_s
{
	unsigned long	apFilterStatPassPkt;
	unsigned char	apFilterStatDenyMac[apMacFilterList_SIZE][6];
	unsigned long	apFilterStatDenyPkt[apMacFilterList_SIZE];
	int				iRsv[4];				/* 预留位 */
} T_apMacFilterStat;

/* 最终用户列表 */
#define RATE_LIST_MAX_NUM		44  //modify by zjj
#define SNR_LIST_MAX_NUM		17   //modify by zjj
/*速率信息*/
/*没有保留字段*/
typedef struct  /*简化*/
{
	unsigned long  ulRate;			/*	以bps为单位，如为1Mbps,则该值为1000000 */
//#define WIFI_RATE_1M          0     /*	1M速率   */
//#define WIFI_RATE_2M          1     /*	2M速率   */
//#define WIFI_RATE_5DOT5M	  2     /*	5.5M速率 */
//#define WIFI_RATE_11M         3     /*	11M速率  */
//#define WIFI_RATE_6M          4     /*	6M速率   */
//#define WIFI_RATE_9M          5     /*	9M速率   */
//#define WIFI_RATE_12M         6     /*	12M速率  */
//#define WIFI_RATE_18M         7     /*	18M速率  */
//#define WIFI_RATE_24M         8     /*	24M速率  */
//#define WIFI_RATE_36M         9     /*	36M速率  */
//#define WIFI_RATE_48M         10    /*	48M速率  */
//#define WIFI_RATE_54M         11    /*	54M速率  */
	unsigned long  ulRecvPkt;
//	unsigned long  ulRecvErrorPkt;	/* 接收的错包数 */
//	unsigned long  ulRecvByteNum;	/* 接收字节数 */
//	unsigned long  ulRecvRetryPkt;  /* 接收重传包数 */
	/* hc modified for cmcc test 2009-7-20 */
//	unsigned long  ulRecvRestryByetNum; /* 接收重传包字节数 */
	unsigned long  ulSendPkt;
//	unsigned long  ulSendByteNum;	    /* 发送字节数 */
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
	    unsigned long       stSNRList[SNR_LIST_MAX_NUM];       /*ap收到此终端的各信道强度下的包数统计（包含数据帧和管理帧）*/  /*add by zjj*/
	unsigned long ulPktSendToUser;		/* 包数目 */
	unsigned long ulPktRecvFromUser;	/* 包数目 */
	
	/*add by zjj*/
	unsigned long   ulSendDataBytes;     /*从ap发送给此终端的数据包总字节数，即终端接收的数据包总字节数（只含数据包）*/ /*新增*/
        unsigned long   ulRecvDataBytes;     /*ap从此终端接收的数据包总字节数，即终端发送的数据包总字节数（只含数据包）*/   /*新增*/
        unsigned long   ulPktSendToUserAll;             /*从此终端发送的包数（包含数据帧和管理帧）*/ /*新增*/     
        unsigned long   ulPktRecvFromUserAll;     /*从此终端接收的包数（包含数据帧和管理帧）*/   /*新增*/
        unsigned long   ulSendDataBytesAll;     /*从ap发送给此终端的总字节数，即终端接收的总字节数（包含数据帧和管理帧）*/    /*新增*/
        unsigned long   ulRecvDataBytesAll;     /*ap从此终端接收的总字节数，即终端发送的总字节数（包含数据帧和管理帧）*/      /*新增*/
        unsigned long    ulRecvRetryPkt;      /*ap从此终端接收的重传包数，即终端发送的重传包数（只含数据帧）*/      /*新增*/
        unsigned long    ulSendRetryPkt;     /*从ap发送到此终端的重传包数，即终端接收的重传包数（只含数据帧）*/      /*新增*/

	unsigned long ulendUserCapFlag;		/* 节点能力属性*/

	/* 2010/08/06 fengjing cmcc test */
	unsigned long ulRecvFrag;  /* 终端发送的被分片的包数 */
 	unsigned long ulSendFrag;  /* AP向终端发送的被分片的包数 */

	/* 2011-03-23 fengjing add:最高信号强度、最低信号强度 */
	/*      新增     */
	 signed char ucMaxRssi;
	 signed char ucMinRssi;
    /* BEGIN: Added by 吕建林, 2012/12/18   PN:task#287  关于征求《广东移动WLAN设备网管接口技术要求v3.2之补充条款二(征求意见稿）》 */
    //unsigned short usEndUserRate;     /* 终端接入速率 */
    unsigned short usEndUserRate;      /*当前AP接收速率*/
    unsigned short usEndUserTxRate;     /*当前AP发送速率*/
    /* END:   Added by 吕建林, 2012/12/18   PN:task#287  关于征求《广东移动WLAN设备网管接口技术要求v3.2之补充条款二(征求意见稿）》 */

    /* BEGIN: Added by lvjianlin, 2013/3/29   PN:TASK #653 【X86平台】【AC平台系统】【外场需求】广东网管需求节点“协商无线速率集中的最高速 */
    unsigned short  usUserHighAssocRate;    /* 协商无线速率集中的最高速率，单位M，双倍表示 */
    /* END:   Added by lvjianlin, 2013/3/29   PN:TASK #653 【X86平台】【AC平台系统】【外场需求】广东网管需求节点“协商无线速率集中的最高速 */
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
	unsigned long ulPktSendToUser;		/* 包数目 */
	unsigned long ulPktRecvFromUser;	/* 包数目 */
	
	  unsigned long   ulSendDataBytes;     /*从ap发送给此终端的数据包总字节数，即终端接收的数据包总字节数（只含数据包）*/ /*add by zjj*/
        unsigned long   ulRecvDataBytes;     /*ap从此终端接收的数据包总字节数，即终端发送的数据包总字节数（只含数据包）*/   /*新增*/
        unsigned long   ulPktSendToUserAll;             /*从此终端发送的包数（包含数据帧和管理帧）*/ /*新增*/     
        unsigned long   ulPktRecvFromUserAll;     /*从此终端接收的包数（包含数据帧和管理帧）*/   /*新增*/
        unsigned long   ulSendDataBytesAll;     /*从ap发送给此终端的总字节数，即终端接收的总字节数（包含数据帧和管理帧）*/    /*新增*/
        unsigned long   ulRecvDataBytesAll;     /*ap从此终端接收的总字节数，即终端发送的总字节数（包含数据帧和管理帧）*/      /*新增*/
        unsigned long    ulRecvRetryPkt;      /*ap从此终端接收的重传包数，即终端发送的重传包数（只含数据帧）*/      /*新增*/
        unsigned long    ulSendRetryPkt;     /*从ap发送到此终端的重传包数，即终端接收的重传包数（只含数据帧）*/      /*新增*/

	unsigned long ulendUserCapFlag;		/* 节点能力属性*/

	/* 2010/08/06 fengjing cmcc test */
	unsigned long ulRecvFrag;  /* 终端发送的被分片的包数 */
 	unsigned long ulSendFrag;  /* AP向终端发送的被分片的包数 */

	/* 2011-03-23 fengjing add:最高信号强度、最低信号强度 */
	/*      新增     */
	 char ucMaxRssi;
	 char ucMinRssi;
     
     /* BEGIN: Added by 吕建林, 2012/12/18   PN:task#287  关于征求《广东移动WLAN设备网管接口技术要求v3.2之补充条款二(征求意见稿）》 */
     //unsigned short usEndUserRate;     /* 终端接入速率 */
     unsigned short usEndUserRate;      /*当前AP接收速率*/
     unsigned short usEndUserTxRate;     /*当前AP发送速率*/
     /* END:   Added by 吕建林, 2012/12/18   PN:task#287  关于征求《广东移动WLAN设备网管接口技术要求v3.2之补充条款二(征求意见稿）》 */

    
     /* BEGIN: Added by lvjianlin, 2013/3/29   PN:TASK #653 【X86平台】【AC平台系统】【外场需求】广东网管需求节点“协商无线速率集中的最高速 */
     unsigned short  usUserHighAssocRate;    /* 协商无线速率集中的最高速率，单位M，双倍表示 */
     /* END:   Added by lvjianlin, 2013/3/29   PN:TASK #653 【X86平台】【AC平台系统】【外场需求】广东网管需求节点“协商无线速率集中的最高速 */

} T_apEndUserSimple;

/* 2010-3-16 pyy 修改80-->128 */
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
	int				iRsv[4];					/* 预留位 */
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
	int					iRsv[4];			/* 预留位 */
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
	int					iRsv[4];			/* 预留位 */
} T_apVapEntry;

#define apVapTable_SIZE 16
typedef T_apVapEntryRw T_apVapTableRw[apVapTable_SIZE];
typedef T_apVapEntry T_apVapTable[apVapTable_SIZE];
typedef T_apVapBaiscEntry T_apVapBasicTable[apVapTable_SIZE];
#define apVapList_SIZE apVapTable_SIZE
typedef T_apVap T_apVapList[apVapList_SIZE];
typedef T_endUserMac T_apEndUserMac[apVapTable_SIZE];

/* BEGIN: Added by 吕建林, 2012/12/6   PN:TASK #257 VAP级流控配置功能 商用版新增 */
#define APP_VAP_FLOWCTL_USER_SIZE 128      //AP下最大用户数目
typedef T_apVapMacFlowCtrlSetup T_apVapFlowCtrlTable[APP_VAP_FLOWCTL_USER_SIZE];
/* END:   Added by 吕建林, 2012/12/6   PN:TASK #257 VAP级流控配置功能 商用版新增 */

/* BEGIN: Added by 吕建林, 2013/1/10   PN:TASK #257  -2 VAP-MAC用户流控表更新 */
typedef T_apVapMacFlowCtrlCount T_apVapFlowCtrlCountTable[APP_VAP_FLOWCTL_USER_SIZE];
/* END:   Added by 吕建林, 2013/1/10   PN:TASK #257  -2 VAP-MAC用户流控表更新 */
typedef struct adhocVap_s
{
	char			adhocSsid[32];
	unsigned long	adhocId;
	unsigned long	wifiDeviceId;			/* 所属无线设备ID */
	int				iRsv[4];				/* 预留位 */
} T_adhocVap;
typedef T_adhocVap T_adhocVapAdd;
typedef T_adhocVap T_adhocVapDelete;

typedef struct adhocBasicSetup_s
{
	unsigned long 	ulWtpId;
	unsigned long	wifiDeviceId;			/* 所属无线设备ID */
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
	unsigned long	ulAntennaMode;			/* 智能天线模式 */
	unsigned long	ulMeshAdhocMode;		/* MESH属性 add by yzl 2008-12-8*/
#define MESH_ADHOC_MODE_AC		0			/* MESH接入 */
#define MESH_ADHOC_MODE_STA		1			/* MESH终端 */
#define MESH_ADHOC_MODE_BK		2			/* MESH回传 */
#define MESH_ADHOC_MODE_ACBK	3			/* MESH接入和回传 */
	unsigned long	ulWmmQos;				/* WMM功能 */
#define WMM_QOS_DISABLE			0
#define WMM_QOS_ENABLE			1
	unsigned long	ulQosMap;
#define QOS_MAP_BK				0
#define QOS_MAP_BE				1
#define QOS_MAP_VI				2
#define QOS_MAP_VO				3
#define QOS_MAP_AUTO			8
	unsigned long ulPrivCap;				/* VLAN属性 */
	int				iRsv[4];				/* 预留位 */

	unsigned long ulMOC;							/* 对象类标识 */
	unsigned char aucMOI[AC_MOI_LEN];				/* 对象实例标识 */
} T_adhocBasicSetup;

typedef struct adhocSecuritySetup_s
{
	unsigned long 	ulWtpId;
	unsigned long	wifiDeviceId;			/* 所属无线设备ID */
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
	int				iRsv[4];					/* 预留位 */

	unsigned long ulMOC;							/* 对象类标识 */
	unsigned char aucMOI[AC_MOI_LEN];				/* 对象实例标识 */
} T_adhocSecuritySetup;

#define MAX_SNR_ENTRY_I 8
#define adhocSnrList_SIZE MAX_SNR_ENTRY_I
typedef struct adhocSnr_s
{
	long	adhocChannel;
	long	adhocSnrValue;
	int		iRsv[4];					/* 预留位 */
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
	unsigned long	adhocNeighborTxPower;	/* 毫瓦 */
	long			adhocNeighborRssi;
	int				iRsv[4];				/* 预留位 */
} T_adhocNeighbor;
typedef T_adhocNeighbor T_adhocNeighborList[adhocNeighborList_SIZE];

#define AODV_HOPS_TO_GW_SIZE 16

typedef struct
{
	unsigned long ulMeshGwFlag;                    /* if it is a MESH GW */
#define MESH_GW_FLAG_GW		1
#define MESH_GW_FLAG_UNGW	0
	unsigned long ulInputFlow;                     /* 所有业务端口入流量 */
	unsigned long ulOutputFlow;                    /* 所有业务端口出流量 */
	unsigned long ulForwardFlow;                   /* 所有端口的转发流量 */
	unsigned long ulLocalFlow;                     /* 所有端口的本地流量 */

	unsigned long ulDownlinkDataFlow;              /* 下行业务流量 */
	unsigned long ulUplinkDataFlow;                /* 上行业务流量 */
	unsigned long ulDownlinkMngFlow;               /* 下行管理流量 */
	unsigned long ulUplinkMngFlow;                 /* 上行管理流量 */
	unsigned long ulDownlinkFwdFlow;               /* 下行转发流量 */
	unsigned long ulUplinkFwdFlow;                 /* 上行转发流量 */

	unsigned long ulHop2GwNum;
	unsigned char acHop2GwAddr[AODV_HOPS_TO_GW_SIZE][6];

} AODV_NODEINFO_T;

typedef struct adhocVapEntryRw_s
{
	unsigned long			ulAdhocVapId;
	T_adhocBasicSetup		stAdhocBasicSetup;
	T_adhocSecuritySetup	stAdhocSecuritySetup;
	int						iRsv[4];				/* 预留位 */
} T_adhocVapEntryRw;
typedef struct adhocVapEntry_s
{
	unsigned long			ulAdhocVapId;
	T_adhocBasicSetup		stAdhocBasicSetup;
	T_adhocSecuritySetup	stAdhocSecuritySetup;
	T_adhocSnrList			stAdhocSnrList;
	T_adhocNeighborList		stAdhocNeighborList;
	int						iRsv[4];				/* 预留位 */
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
	int				iRsv[4];			/* 预留位 */
} T_sysPortStat;

#define vlanPortList_SIZE sysPort_NUM
typedef struct vlanSetup_s
{
	unsigned long	vlanId;
	unsigned long	ulVlanPrio;			/* VLAN优先级 */
#define VLAN_PRIO_0		0
#define VLAN_PRIO_1		1
#define VLAN_PRIO_2		2
#define VLAN_PRIO_3		3
//#define QosType_DATA_AUTO       8     /* 自动 */
	char			vlanName[32];
	unsigned long	ulVlanManageSwitch;
#define vlanManageSwitch_NO		0
#define vlanManageSwitch_YES	1
	unsigned long	vlanPortList[vlanPortList_SIZE];
	unsigned char	aucIpAddr[4];
	unsigned char	aucNetMask[4];
	unsigned char	aucDefaultGw[4];
	unsigned long	ulL2Isolate;		/* 二层隔离 */
#define L2_ISOLATE_DISABLE		0
#define L2_ISOLATE_ENABLE		1
	int				iRsv[4];			/* 预留位 */
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
	int				iRsv[4];			/* 预留位 */
} T_vlanPort;
#define vlanPortSelect_SIZE sysPort_NUM
#define INVALID_PORT_ID 0xFFFFFFFF
typedef T_vlanPort T_vlanPortSelect[vlanPortSelect_SIZE];

#define ENETINTERFACE_MAX_NUM	2
typedef struct ethernetInterface_s
{
	unsigned long	ulWtpId;
	unsigned long ulEnetPortId;
	
	unsigned long	ulEnetRunMode;		/* 是否启用 */
#define enetRunMode_DISABLE		0
#define enetRunMode_ENABLE		1
	unsigned long	ulSpeedUplex;		/* 速率/双工模式 */
#define speedUplex_AUTO			0		/* 自适应 */
#define speedUplex_10M			1		/* 10M全双工 */
#define speedUplex_100M			2		/* 100M全双工 */
#define speedUplex_1000M		3		/* 1000M全双工 */
	unsigned long	ulEnetState;		/* 网口状态 */
#define enetState_UPLINK		0		/* 上联口 */
#define enetState_DOWNLINK		1		/* 下联口 */
	unsigned long	ulPrivCap;			/* VLAN属性 */
	int				iRsv[4];			/* 预留位 */

	unsigned long ulMOC;							/* 对象类标识 */
	unsigned char aucMOI[AC_MOI_LEN];				/* 对象实例标识 */
} T_enetInfBasicSetup;

typedef struct
{
	T_enetInfBasicSetup	stEnetInfSetup;
	int		iRsv[4];					/* 预留位 */
} T_enetInfSetup;
typedef T_enetInfSetup T_enetInfTable[ENETINTERFACE_MAX_NUM];

typedef struct configInforce_s
{
	int		iInfoceInvalidFlag;
#define INFOCE_INVALID_FLAG_SET			0
#define INFOCE_INVALID_FLAG_DELAY_SET	2
	time_t  inforceTime;
	int		iRsv[4];					/* 预留位 */
}T_configInforce;

/* CAPWAP协议配置 */
typedef struct capWapSetup_s
{
	unsigned long ulWtpId;					/* WTP设备ID */
	char		  acSecret[32];				/* WTP设备接入密码 */
	unsigned long ulDisType;				/* AC发现类型 */
#define DIS_TYPE_AUTO		0	
#define DIS_TYPE_DHCP		1
#define DIS_TYPE_DNS		2
#define DIS_TYPE_MULTICAST	3
	char		  acAcURL[64];				/* AC URL */
	unsigned long ulMOC;					/* 对象类标识 */
	unsigned char aucMOI[AC_MOI_LEN];		/* 对象实例标识 */
} T_capWapSetup;

typedef struct perfCfg_s
{
	unsigned long	ulPerfTaskNum;
	unsigned long   ulPerfCounterGroupID;
	unsigned long   ulStatus;				/* 是否开启性能统计 */
#define PERF_DISABLE		0				/* 关闭 */
#define PERF_ENABLE			1				/* 开启 */
	unsigned long   ulPerfReportInterval;	/* 统计间隔 */
	unsigned long   ulRealTimeQuery;		/* 是否开启实时查询 */
#define REAL_TIME_QUERY_DISABLE		0		/* 关闭 */
#define REAL_TIME_QUERY_ENABLE		1		/* 开启 */
	int iRsv[4];							/* 预留位 */
} OMA_PERF_CFG_T;

#define PERF_COUNT	3

typedef struct
{
	unsigned long ulWtpId;
	
    unsigned long   ulAlarmNo;              /* 告警ID */
    char            acAlarmName[32];        /* 告警名称 */
    unsigned long   ulAlarmIsValid;         /* 告警是否有效	0:禁用 1:启用 */

   	unsigned long ulMOC;							/* 对象类标识 */
	unsigned char aucMOI[AC_MOI_LEN];				/* 对象实例标识 */
} OMA_ALARM_FILTER_CFG_T;

#define INVALID_ALARM_NO    0xffffffff
#define ALARM_COUNT	        128

typedef struct
{
	unsigned long ulWtpId;
	
    unsigned long   ulCpuLoadPeriod	;	/* CPU负荷采样周期	    5       */
    unsigned long   ulCpuLoadCount	;	/* CPU负荷持续次数	    6       */
    unsigned long   ulCpuLoadHigh	;	/* CPU负荷告警产生门限	90      */
    unsigned long   ulCpuLoadLow	;	/* CPU负荷告警消除门限	80      */

    unsigned long   ulMemLoadPeriod	;	/* 内存利用率采样周期	    5   */
    unsigned long   ulMemLoadCount	;	/* 内存利用率持续次数		6   */
    unsigned long   ulMemLoadHigh	;	/* 内存利用率告警产生门限	85  */
    unsigned long   ulMemLoadLow	;	/* 内存利用率告警消除门限	80  */

    unsigned long   ulTempPeriod	;	/* 设备温度采样周期	    5       */
    unsigned long   ulTempCount	    ;	/* 设备温度持续次数	    6       */
    unsigned long   ulTempHigh	    ;	/* 设备高温告警产生门限	55      */
    unsigned long   ulTempLow	    ;	/* 设备高温告警消除门限	50      */
    int             iTempHigh	    ;	/* 设备低温告警产生门限	-30     */
    int             iTempLow	    ;	/* 设备低温告警消除门限	-20     */

    unsigned long ulMOC;							/* 对象类标识 */
	unsigned char aucMOI[AC_MOI_LEN];				/* 对象实例标识 */
}OMA_ALARM_THRESHOLD_CFG_T;

/* BEGIN: Added by 吕建林, 2012/12/10   PN:TASK #257 VAP级流控配置功能 商用版新增 */
typedef 	struct
{
	unsigned long	ulWtpId;					/*AP设备ID */
	unsigned long	ulWifiDeviceId;			/*无线设备编号*/
	unsigned long	ulVapId;					/* VAP ID */
	unsigned char	aucMac[6];				/*用户MAC */
	unsigned char	aucResv[2];				/*保留字段，为了四字节对齐*/
	unsigned long	ulUserUpBandMax;		/*上行用户最大带宽, kbps ,缺省值1024*/
	unsigned long	ulUserDownBandMax;		/*下行用户最大带宽，kbps ,缺省值1024*/
	unsigned long	ulMOC;					/*对象类标识，网管自动生成,下发到网元*/
	unsigned char	aucMOI[AC_MOI_LEN];	/*对象实例标识，网管自动生成,下发到网元*/
}OMA_VAP_MAC_FLOWCTRL_SETUP_T;

/* END:   Added by 吕建林, 2012/12/10   PN:TASK #257 VAP级流控配置功能 商用版新增 */

/* BEGIN: Added by lvjianlin, 2013/6/20   PN:lte-fi  GPS ALARM  LOCAL PORTAL */
typedef struct
{
    unsigned long   ulWtpId;                /* WTP  id */
    unsigned char   acLtefiApn[32];         /* 接入点名称 */
    unsigned long   ulLtefiMtu;             /* MTU值 */
    unsigned long   ulAccessMode;           /* LTE-FI接入模式 */
    unsigned long   ulBatterySptTime;       /* 电池供电时间 */
    unsigned char   acBusNo[32];            /* 公交编号 */
    unsigned char   aucApForwardBrIp[4];    /* 先本地后集中的情况下的网关地址 */
    unsigned char   aucApForwardBrMask[4];  /* 先本地后集中的情况下的网关地址掩码 */
    unsigned char   acGpsServer1[64];       /* GPS信息上报服务器1 */
    unsigned char   acGpsServer2[64];       /* GPS信息上报服务器2 */
    unsigned long   ulGpsReportPeriod;      /* GPS信息上报周期 add by liyoukun*/
    unsigned char   acContextServer1[128];  /* LTE-Fi上内容下载服务器1 */
    unsigned char   acContextServer2[128];  /* LTE-Fi上内容下载服务器2 */
    unsigned char   acLtefiPortalIp[64];    /* ltefi设备本地portalIP地址 add by liyoukun*/
    /* BEGIN: Added by lvjianlin, 2013/7/11   PN:ulDataUpdateTime */
    unsigned long   ulDataUpdateTime;       /* 定期更新本地存储空间内容 */
    /* END:   Added by lvjianlin, 2013/7/11   PN:ulDataUpdateTime */
    /* BEGIN: Added by lvjianlin, 2013/7/25   PN:lte-fi DT */
    unsigned char   acDTUserName[64];       /* 路测服务器登陆用户名 */
    unsigned char   acDTPass[64];           /* 路测服务器登陆密码 */
    unsigned char   acDTServerIp[64];       /* 路测服务器IP */
    unsigned long   ulDTPort;               /* 路测服务器端口 */
    unsigned long   ulDTEnable;             /* 路测服务是否启用 */
    unsigned long   ulDTCollectInterval;    /* 路测信息采集间隔 */
    /* END:   Added by lvjianlin, 2013/7/25   PN:lte-fi DT */
    unsigned long   ulMOC;
    unsigned char   aucMOI[AC_MOI_LEN];
}OMA_LTEFI_BASIC_SETUP_T;

/* END:   Added by lvjianlin, 2013/6/20   PN:lte-fi  GPS ALARM  LOCAL PORTAL */
#define SSID_LIST_SIZE	9
typedef struct
{
	unsigned long ulWifiDeviceId;			/* 所属无线设备ID */
	unsigned long ulRunMode;
#define STA_RUN_MODE_DISABLE	0
#define STA_RUN_MODE_ENABLE		1
	char acMac[6];
	char acRsv[2];
	char acBasicSsid[32];
	unsigned long ulPrivCap;	/* 是否开启能力信息 */
	int iRsv[4];				/* 预留字段 */
} STA_BASIC_SETUP_T;

typedef struct
{
	unsigned long ulBeaconMissInterval; /* 丢失Beacon间隔 */
	unsigned long ulScanOffChan;		/* 信道切回时长 */
	unsigned long ulRssiThreshold;		/* 切换RSSI门限 */
	unsigned long ulRateThreshold;		/* 切换RATE门限 */
	unsigned long ulRoamCheck;			/* 切换检查时机 */
	unsigned long ulRoamDiff;			/* 切换信号差值 */
	unsigned long ulProbeint;			/* 当前信道探测周期 */
	unsigned long ulBgscanSwitch;		/* BG扫描开关 */
#define BG_SCAN_SWITCH_OFF	0
#define BG_SCAN_SWITCH_ON	1
	unsigned long ulRssilpfAth;			/* 数据信号平滑参数 */
	unsigned long ulRssilpfScan;		/* 扫描信号平滑参数 */

	/* under field is added by yzl in 2008-12-18 */
	unsigned long ulBgscanintvl;		/* 扫描间隔 */
	unsigned long ulScanresult;			/* 可信时限 */
	unsigned long ulIdleassocdiff;		/* 空闲关联差值 */
	unsigned long ulBgscanidle;			/* 业务空闲 */
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

/* 代理终端信息 */
#define RELAY_CLIENT_MAX  64
typedef struct
{
	char acMac[IEEE80211_ADDR_LEN]; /* 代理终端的MAC */
	char acIp[IEEE80211_IPADDR_LEN];/* 代理终端的IP */
	unsigned long ulUcastRecv;		/* 接收单播个数 */
	unsigned long ulUcastSend;		/* 发送单播个数 */
	unsigned long ulMcastSend;		/* 发送广播个数 */
	unsigned long ulLastRecv;		/* 最近接收时间 */
	unsigned long ulLastSend;		/* 最近发送时间 */
} RELAY_CLIENT_T;

typedef RELAY_CLIENT_T RELAY_CLIENT_ENTRY_T[RELAY_CLIENT_MAX];

/* 历史关联信息 */
typedef struct
{
	unsigned char acRseApmac[IEEE80211_ADDR_LEN];	/* 关联AP的地址，若全0，则表示该记录为空 */
	unsigned char ucRseChannel;						/* 关联AP的信道 */
	char cRseLeaveRssi;								/* 离开该AP时的RSSI值 */
	unsigned char ucRseLeaveRate;					/* 离开该AP时的RATE值 */
	unsigned char ucRsv[3];
	unsigned long ulRseJoinTime;					/* 决定加入该AP的时间 */
	unsigned long ulRseLeaveTime;					/* 离开该AP时的时间 */
	unsigned long ulRseAssocTime;					/* 关联该AP成功时的时间 */
#define RSE_LEAVE_REASON_ROAM			1
#define RSE_LEAVE_REASON_DESBSSID       2
#define RSE_LEAVE_REASON_ASSFAIL        3
	unsigned char ucRseLeaveReason;					/* 离开该AP的原因 */
	unsigned char aucRsv[3];
} ROAM_STAT_ELEM_T;

#define ROAM_STAT_ELEM_MAX 10
typedef struct
{
	unsigned long ulRsApsInd;							/* 当前记录的下标，也即rs_aps_ind为最老的记录 */
	ROAM_STAT_ELEM_T astRsRoamAps[ROAM_STAT_ELEM_MAX];	/* 关联信息 */
	unsigned long ulRsRoamNum;							/* 切换或重新关联的次数 */
	unsigned long ulRsNow;								/* 当前的时间 */
	unsigned long ulInterval;
} ROAM_STAT_T;

/* 扫描信道列表信息 */
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
	unsigned long  isr_beacons;         /* beacons个数*/
	unsigned long  isr_lifetime;        /* 统计Beacon时间，单位s */
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

/* 终端关联VAP信息 */
typedef struct
{
	unsigned char assocApMac[6];			/* 关联ＡＰ　ＭＡＣ地址 */
	unsigned char aucRsv[2];
	unsigned char assocAPIp[4];				/* 关联ＡＰ　IP地址 */
	unsigned long assocTimes;				/* 关连ＡＰ持续时间 */
	unsigned long ulBgMode;					/* 模式 */
	unsigned char ucAssocApRssi;			/* 关联ＡＰ信号强度 */
	char cNoise;							/* AP Noise */
	unsigned char aucRsv1[2];
	T_rateList    stRateList;				/* 速率列表 */
	unsigned long ulPktSendToAp;			/* 发送包数目 */
	unsigned long ulPktRecvFromAp;			/* 接收包数目 */
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
	unsigned long ulVlanId;			/* 进行映射的VLAN ID */
	char acIp[4];					/* 进行映射的IP网段 */
	char acMask[4];					/* 进行映射的IP网段掩码(若需主机映射，mask填全0xFF) */
	unsigned long ulRsv[4];			/* 扩充字段 */
} MESH_VLAN_MAP_IP_CFG_T;

/* 进行ETH_VLAN_MAP(有线端口VLAN映射策略)配置数据结构 */
#define VLAN_MAP_OFF               0 /* 不启用有线端口VLAN映射 */
#define VLAN_MAP_IP                1 /* 使用IP映射模式 (目前默认值) */
#define VLAN_MAP_OTHER             2 /* 使用其它映射模式 */
typedef struct
{
	unsigned long ulMapMode;        /* 策略模式 */
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
    /* BEGIN: Added by 吕建林, 2012/12/6   PN:TASK #257 VAP级流控配置功能 商用版新增 */
    /* BEGIN: Added by 吕建林, 2013/1/14   PN:TASK #257  -2 VAP-MAC用户流控表更新 */
    /* 商用版本流控修改，由capwap移植到usrm */
    #if 0
    T_apVapFlowCtrlTable    stVapMacFlowCtrlSetup;
    #endif
    /* END:   Added by 吕建林, 2013/1/14   PN:TASK #257  -2 VAP-MAC用户流控表更新 */
    /* END:   Added by 吕建林, 2012/12/6   PN:TASK #257 VAP级流控配置功能 商用版新增 */

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
	char			acDvName[32];			/* 设备名称 */
	char			acIp[32];				/* 设备IP */
	char			acMac[32];				/* 设备MAC */
	char			acDvId[32];				/* 设备ID */
	char			acSwVersion[32];		/* 软件版本 */
	char			acTypeName[32];			/* 设备型号 */
	unsigned long	ulWtpId;

    unsigned char   ucSysVersion;			/* 文件系统 */
	unsigned char   ucUpdateStatus;			/* 更新状态 */
	unsigned char   ucOemManu;				/* OEM厂商 */
	unsigned char   ucVlanID;				/* VLAN ID */

	// add by leijie
	unsigned char   ucCmdSn;                /* 命令序列号 */
	unsigned char   ucCmdStatus;            /* 命令执行状态 */
#define CMD_STATUS_NULL         0
#define CMD_STATUS_UNKNOW       1
#define CMD_STATUS_RUNNING      2
#define CMD_STATUS_FAIL         3
#define CMD_STATUS_SUCCESS      4

    unsigned char   ucSnmpSetSn;            /* SNMP操作序列号 */
    unsigned char   ucSnmpSetStatus;        /* SNMP操作状态 */

    char			acTargetSwVersion[32];  /* 软件版本 */
    char			acSwVersion0[32];       /* 软件版本0 */
    char			acSwVersion1[32];       /* 软件版本0 */

    char			acHwType[32];           /* 硬件类型 */
    char			acHwVersion[32];        /* 硬件版本 */

    char			acManagementSSID[32];   /* 管理VAP SSID */

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

    char            acLocalSn[32];         // 本地分配的sn

	char			acRsv[22];				/* 预留位 */
}OMA_DEVICE_INFO_T;

typedef struct
{
    unsigned long   ulAcId;                 /*AC ID*/
    char            acAcDevName[32];        /*AC NAME*/
    char            acAcDevType[32];        /*设备型号*/
    char            acAcDevSN[6];           /*设备序列号：位于 /etc/gb*/
    unsigned char   aucResv1[2]; 
    char            acAcSwVersion[32];      /*设备软件版本*/
    char            acAcHwVersion[32];      /*设备硬件版本 */

    unsigned char   aucFitLanIp[4];         /* FIT-LAN IP */
    unsigned char   aucFitLanMask[4];       /* FIT-LAN MASK */     

    unsigned char   aucFatLanIp[4];         /* FAT-LAN IP */
    unsigned char   aucFatLanMask[4];       /* FAT-LAN MASK */ 

    unsigned char   aucL2WanIp[4];          /* L2-WAN IP */
    unsigned char   aucL2WanMask[4];        /* L2-WAN MASK */

    unsigned char   aucL3WanIp[4];          /* L3-WAN IP */
    unsigned char   aucL3WanMask[4];        /* L3-WAN MASK */

    unsigned char   aucDefaultGw[4];        /* 缺省网关*/
    unsigned char   aucPrimaryDns[4];       /* 主用DNS服务器*/
    unsigned char   aucSecondaryDns[4];     /* 备用DNS服务器*/
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
	unsigned char	aucDeviceId[6];			/* 设备ID：6个16进制字符 */
	unsigned char	aucSwVersion[32];		/* 软件版本 */
	unsigned long	ulSlaveSetState;		/* 从设备配置状态 */
#define SLAVE_SET_STATE_SUCCESS	0
#define SLAVE_SET_STATE_FAILED	1
	unsigned long	ulSlaveInitFlag;		/* 从设备初始化标识 */
#define SLAVE_INIT_FLAG_INIT	1;
#define SLAVE_INIT_FLAG_UNINIT	0;
	int				iRsv[4];				/* 预留位 */
} OMA_MS_CONNECT_MSG_T;

/************************************************************************/
/*---------------------------------系统日志------------------------------*/
/************************************************************************/
/* 子系统号 */
#define SUBSYS_WIFI             1
#define SUBSYS_OMA              2
#define SUBSYS_ROUTE            3
#define SUBSYS_DRIVER           4


/* 子系统的缺省模块号 */
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


/* LOG 类型*/
#define LOG_TYPE_SECURITY        1
#define LOG_TYPE_DEVICE          2
#define LOG_TYPE_OPERATION       3
#define LOG_TYPE_SERVICE         4


/* LOG级别 */
#define  BB_LOG_LEVEL_FATAL       4
#define  BB_LOG_LEVEL_ERROR       3
#define  BB_LOG_LEVEL_WARNING     2
#define  BB_LOG_LEVEL_INFO        1
#define  BB_LOG_LEVEL_DETAIL      0


/* LOG文件编号 */
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
	unsigned long ulMaxLogItem;						/* 总共日志数 */
	unsigned long ulCurPage;						/* 当前日志存放的页数 */
	LOG_ITEM_T stLogItem[MAX_LOG_ITEM_EVERY_PAGE];	/* 每次获取的日志，每次最多50条 */
} LOG_ENTRY_T;

/************************************************************************/
/*                         主页内容定义                                 */
/************************************************************************/

#define MAX_ETHER_NUM		2
#define ETH_MAC_LEN		6

#define ETHE_SPEED_10M		0
#define ETHE_SPEED_100M		1
#define ETHE_SPEED_AUTO		2

typedef struct
{
	unsigned long  ulEnetPortNum;           /* 以太网端口号 */     
	char           acEnetPortName[16];      /* 以太网端口名称 */
	char           acEnetPortDesc[32];      /* 以太网端口描述 */
	unsigned long  ulEnetAdminState;        /* 以太网管理状态 */
//   unsigned long ulEnetOpState;           /* 以太网口运行状态 */
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
	unsigned char aucMac[ETH_MAC_LEN];       /* 以太网口MAC  */
	unsigned char aucRsv0[2];

} ETHER_ATTRIBUTE_T;

#define MAX_WIFI_NUM	4
typedef struct
{
	unsigned long ulRadioState;
	unsigned long ulRadioMode;
	unsigned long ulCurRadioMode;           /* 当前模式 */
	unsigned long ulRadioChannelConfig;
	unsigned long ulRadioChannel;
	char acRadioRates[64];
	unsigned long ulMaxTxPwr;
	unsigned long ulTxPwrDown;
	unsigned long ulTxPwrDownScale;         /* 功率衰减范围 */
	unsigned long ulAnteGain;               /* 天线增益 */
	unsigned long ulMaxStaNum;              /* 最大允许接入终端数 */
	unsigned long ulAutoTxPwr;
	unsigned long ulRadioRole;
} WIFI_ATTRIBUTE_T;

typedef struct
{
	unsigned char aucIp[4];
	struct tm sysDateTime;				/* 年月日时分秒 */
	unsigned long ulSysUpTime;			/* 系统上电到现在的秒数 */
	ETHER_ATTRIBUTE_T stEthAttribute[MAX_ETHER_NUM];
	unsigned long ulRadioNum;			/* 无线模块数目 */
	WIFI_ATTRIBUTE_T stRadioAttribute[MAX_WIFI_NUM];
	unsigned long ulDownlinkDataFlow;	/* 下行业务流量 */
	unsigned long ulUplinkDataFlow;		/* 上行业务流量 */
	unsigned long ulNeighNum;			/* 邻居数 */

	char acWtpName[32];                  /* 设备名称 */
	char acWtpDeviceType[32];			 /* 设备类型 */
	char acWtpSN[32];					 /* 设备序列号 */
	char acRsv[2];
	char acWtpSwVersion[32];             /* 软件版本 */
	char acWtpHwVersion[32];             /* 硬件版本 */
	char acWtpCpuInfo[32];				 /* CPU信息 */
	char acWtpMemInfo[32];				 /* 内存信息 */
	char acWtpManufacturer[32];			 /* 设备制造商 */
	char acWtpSwDeveloper[32];			 /* 软件开发商 */
	unsigned long ulCountryCode;		 /* 国家码 */
	unsigned long ulWtpMode;             /* 设备工作模式 */
	unsigned long ulWtpStatus;           /* 工作状态 */

#define WTP_STATUS_INITING      1       /* 正在初始化 */
#define WTP_STATUS_WORKING      2       /* 正常服务中 */
#define WTP_STATUS_OFFLINE      3       /* 离线停止服务 */

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
	unsigned long ulKeeperSwitch;		/* 是否守护 */
	unsigned char aucKeeperIp0[4];		/* 守护服务器的ip地址，设备将周期性ping该地址 */
	unsigned char aucKeeperIp1[4];		/* 守护服务器的ip地址，设备将周期性ping该地址 */
	unsigned long ulKeeperState;		/* 守护状态 */
} OMA_KEEPER_INFO_T;

typedef struct
{
	unsigned long ulWifiId;				/* 所属无线网卡ID */
	unsigned long ulVapId;				/* VAP索引 */
	char          acSsid[32 + 1];		/* SSID */
	char          resv[3];
	unsigned long ulChannel;			/* 信道 */
	unsigned long ulAbgMode;			/* a/b/g模式 */
	unsigned long ulSecurityMode;		/* 安全模式 */
	unsigned long ulAdhocMode;			/* 回传模式（终端/接入/回传） */
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
	unsigned long ulWifiId;				/* 所属无线网卡ID */
	unsigned long ulVapId;				/* VAP索引 */
	char          acSsid[32 + 1];	    /* SSID */
	char          resv[3];
	unsigned long ulRunState;			/* 状态up/down */
	unsigned long ulChannel;			/* 信道 */
	unsigned long ulAbgMode;			/* a/b/g模式 */
	unsigned long ulSecurityMode;		/* 安全模式 */
	unsigned long ulCurUserNum;			/* 当前用户数目 */
} OMA_AP_INFO;

typedef struct
{
	unsigned long ulValidApNum;
	OMA_AP_INFO	stApInfo[apVapTable_SIZE];
} OMA_AP_INFO_ENTRY;

typedef struct
{
	unsigned long ulWifiId;				/* 所属无线网卡ID */
	unsigned long ulVapId;				/* VAP索引 */
	char acSsid[32 + 1];				/* SSID */
	char acBssid[32 + 1];				/* BSSID */
	unsigned long ulChannel;			/* 信道 */
	unsigned long ulAbgMode;			/* a/b/g模式 */
	unsigned long ulSecurityMode;		/* 安全模式 */
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
#define SYS_WORK_MODE_CENTER    2   /* 集中转发 */
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
/*                           动态信息                                   */
/************************************************************************/
typedef struct
{
	char acApSsid[32];					/* VAP SSID */
	unsigned long ulAccessTimes;		/* AP的无线用户链接次数 */
	unsigned long ulAccessFailTimes;	/* AP的无线用户链接失败次数 */
	unsigned long ulAccessRetryTimes;	/* AP的无线用户重链接次数 */
	unsigned long ulAccessDenyTimes;	/* AP的无线用户链接拒绝次数 */
	unsigned long ulAccessOutTimes1;	/* 因用户离开而导致解关联的次数 */
	unsigned long ulAccessOutTimes2;	/* 因AP能力不足而导致解关联的次数 */
	unsigned long ulAccessOutTimes3;	/* 因异常情况而导致解关联的次数 */
	unsigned long ulAccessOutTimes4;	/* 其它原因导致解关联的次数 */
} OMA_GLOBAL_STAT_INFO;

typedef struct
{
	unsigned long ulValidVapNum;
	OMA_GLOBAL_STAT_INFO astGlobalStat[apVapTable_SIZE];
} OMA_GLOBAL_STAT_ENTRY;

/* 2011-09-02 fengjing modify,部分字段类型从UINT32->UINT64 */
/* 集中在发送/接收字节数、发送/接收数据包字节数 */
typedef struct
{
	/* 2012-05-18 修改：增加VAP ID以及Wifi ID字段 */
    unsigned long  ulVapId;   				 /* VAP ID，取值0-15 */
    unsigned long  ulWifiId;                 /* Wifi ID，取值0-3，无线设备ID，应该比较准确的是卡ID，因目前模型无线设备ID与卡ID一致。*/

	char acApSsid[32];

	/* 2011-04-16 fengjing add */
	unsigned long ulTxPkts;            /*发送的包数，无线接口接收的数据包和管理控制包数，不包含重传*/
	unsigned long ulRxPkts;            /*接收的包数，无线接口接收的数据包和管理控制包数，不包含重传*/
	unsigned long long ulTxOctets;         /*发送的字节数，无线接口发送的包含数据字节和管理控制字节数，不包含重传*/
	unsigned long long ulRxOctets;        /*接收的字节数，无线接口接收的包含数据字节和管理控制字节数，不包含重传*/
	
	unsigned long ulRecvFrameNum;            /* 接收的数据帧数 */
	unsigned long ulSendFrameNum;            /* 发送的数据帧数 */

	/* 2010/07/04 fengjing cmcc test */         
	unsigned long ulRecvPackets;            /* 接收的数据包数 */
	unsigned long ulSendPackets;            /* 发送的数据包数 */

	/* 2010/07/01 fengjing cmcc test */
	unsigned long ulRecvCtrlFrame;           /* 接收的控制帧数 */ /*暂只计算ACK帧及PS-POLL帧，故等价于PS-POLL+成功发送单播帧数目，含数据帧与管理帧*/
	unsigned long ulSendCtrlFrame;           /* 发送的控制帧数 */ /*暂只计算ACK帧，故等介于成功接收单播帧数目，含数据帧与管理帧*/
	
	unsigned long long ulRecvBytes;               /* 接收的数据包字节数 */
	unsigned long ulRecvErrFrame;            /* 用户侧(无线侧)接收的错帧数*/
	unsigned long ulRecvDiscardFrame;        /* 用户侧(无线侧)接收的丢帧数*/
	unsigned long ulRecvRetransFrame;        /* 用户侧(无线侧)接收的重传帧数*/

	unsigned long long ulSendBytes;                /* 发送的数据包字节数 */
	unsigned long ulSendErrFrame;             /*  */
	unsigned long ulSendDiscardFrame;
	unsigned long ulSendRetransFrame;         /* 无线信道下行重传的帧数 */ /*重传帧，含数据帧与管理帧*/
	/* 2010/07/01 fengjing cmcc test */
	unsigned long ulSendRetransDataPackets;   /* 下行重传的数据包数 */   /*重传帧，只指数据帧*/
	
	/* hc modified for cmcc test 2009-7-18 */
	unsigned long apUplinkUpdownTimes;       /* Ap 无线上行端口updown 次数 */
	unsigned long apDownlinkUpdownTimes;     /* Ap 无线下行端口updown 次数 */

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

	/* 2010-4-10 pyy 新增*/
	unsigned long ulRecvFrameUCast;	/* 上行单播帧数*/
	unsigned long ulSendFrameUCast;	/* 下行单播帧数*/
	unsigned long ulRecvFrameMCast;	/* 上行非单播的帧数*/
	unsigned long ulSendFrameMCast;	/* 下行非单播的帧数*/
	
	long		  apSnrBSSIDAverageSignalStrength; 	 /*AP  的信号平均强度*/
	unsigned long apSnrBSSIDSignalPkts;				 /*AP  总的信令包数  */
	unsigned long apSnrBSSIDTxSignalPkts;			 /*AP  发射的信令包数*/    /* 发送的管理帧数 */
	unsigned long apSnrBSSIDRxSignalPkts;			 /*AP  接收的信令包数*/    /* 接收的管理帧数 */
	long		  apSnrBSSIDHighestRxSignalStrength; /*AP  的最高信号强度*/
	long 		  apSnrBSSIDLowestRxSignalStrength;  /*AP  的最低信号强度*/
	unsigned long apChStatsChannel;					 /*AP  当前占用的频点*/
	unsigned long apChStatsNumStations;				 /*使用该信道的终端数*/	

	/* 2010/08/06 fengjing cmcc test */
	unsigned long ulSendFragRate; /* bps */ /* 帧的分段速率 */
}__attribute__ ((packed)) OMA_AIR_STATS_INFO ;

typedef struct
{
	unsigned long ulValidVapNum;
	OMA_AIR_STATS_INFO astAirStat[apVapTable_SIZE];
} OMA_AIR_STATS_ENTRY;
/* BEGIN: Added by liyoukun, 2014/1/16   PN:用户接入后就开始统计用户的本地流量 */
/* 用户级的流量统计，包含访问本地流量和上网流量 */
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

/* VAP级统计结构体 */
typedef struct 
{
    unsigned long  ulVapId;   				 /* VAP ID，取值0-15 */
    unsigned long  ulWifiId;                 /* Wifi ID，取值0-3，无线设备ID，应该比较准确的是卡ID，因目前模型无线设备ID与卡ID一致。*/

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
/* vap级的流量统计 */
typedef struct
{
	unsigned long ulValidVapNum;
	OMA_VAP_STATISTIC_INFO astVapFlowStat[apVapTable_SIZE];
} OMA_VAP_STATS_ENTRY;

/* 上报给agent使用的用户统计结构体，从TS模块获取 */
typedef struct
{
	OMA_USER_STATISTIC_INFO     astApEndUser;
} OMC_VAP_END_USER_LIST;

typedef struct apVapEndUserEntryExt_s 
{
	unsigned long	        ulValidNum;
	OMC_VAP_END_USER_LIST   astApEndUserList[MAX_ENDUSER_ENTRY_I];
} OMC_VAP_END_USER_ENTRY;

/* END:   Added by liyoukun, 2014/1/16   PN:用户接入后就开始统计用户的本地流量 */

/* 2011-09-03 fengjing add */
typedef struct
{
    /* BEGIN: Added by 吕建林, 2013/2/4   PN:BUG #1738 【ap93】【平台】【mib统计】无线接口统计 ap上报有问题 */
   // unsigned long  ulVapId;   		
    //unsigned long  ulWifiId; 
    unsigned long ulVapId;
    unsigned long ulWifiId;
    /* END:   Added by 吕建林, 2013/2/4   PN:BUG #1738 【ap93】【平台】【mib统计】无线接口统计 ap上报有问题 */
   char acApSsid[32];

	/* 2011-04-16 fengjing add */
	unsigned long ulTxPkts;            /*发送的包数，无线接口接收的数据包和管理控制包数，不包含重传*/
	unsigned long ulRxPkts;            /*接收的包数，无线接口接收的数据包和管理控制包数，不包含重传*/

       unsigned long ulTxOctetsHigh;         /*发送的字节数，无线接口发送的包含数据字节和管理控制字节数，不包含重传*/     //高位
       unsigned long ulTxOctetsLow;         /*发送的字节数，无线接口发送的包含数据字节和管理控制字节数，不包含重传*/     //低位
       unsigned long ulRxOctetsHigh;        /*接收的字节数，无线接口接收的包含数据字节和管理控制字节数，不包含重传*/     //高位
	unsigned long ulRxOctetsLow;        /*接收的字节数，无线接口接收的包含数据字节和管理控制字节数，不包含重传*/     //低位
	
	unsigned long ulRecvFrameNum;            /* 接收的数据帧数 */
	unsigned long ulSendFrameNum;            /* 发送的数据帧数 */

	/* 2010/07/04 fengjing cmcc test */         
	unsigned long ulRecvPackets;            /* 接收的数据包数 */
	unsigned long ulSendPackets;            /* 发送的数据包数 */

	/* 2010/07/01 fengjing cmcc test */
	unsigned long ulRecvCtrlFrame;           /* 接收的控制帧数 */ /*暂只计算ACK帧及PS-POLL帧，故等价于PS-POLL+成功发送单播帧数目，含数据帧与管理帧*/
	unsigned long ulSendCtrlFrame;           /* 发送的控制帧数 */ /*暂只计算ACK帧，故等介于成功接收单播帧数目，含数据帧与管理帧*/

       unsigned long ulRecvBytesHigh;               /* 接收的数据包字节数 */          //高位
       unsigned long ulRecvBytesLow;               /* 接收的数据包字节数 */          //低位

	unsigned long ulRecvErrFrame;            /* 用户侧(无线侧)接收的错帧数*/
	unsigned long ulRecvDiscardFrame;        /* 用户侧(无线侧)接收的丢帧数*/
	unsigned long ulRecvRetransFrame;        /* 用户侧(无线侧)接收的重传帧数*/

       unsigned long ulSendBytesHigh;                /* 发送的数据包字节数 */      //高位
       unsigned long ulSendBytesLow;                /* 发送的数据包字节数 */       //低位

	unsigned long ulSendErrFrame;             /*  */
	unsigned long ulSendDiscardFrame;
	unsigned long ulSendRetransFrame;         /* 无线信道下行重传的帧数 */ /*重传帧，含数据帧与管理帧*/
	/* 2010/07/01 fengjing cmcc test */
	unsigned long ulSendRetransDataPackets;   /* 下行重传的数据包数 */   /*重传帧，只指数据帧*/
	
	/* hc modified for cmcc test 2009-7-18 */
	unsigned long apUplinkUpdownTimes;       /* Ap 无线上行端口updown 次数 */
	unsigned long apDownlinkUpdownTimes;     /* Ap 无线下行端口updown 次数 */

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

	/* 2010-4-10 pyy 新增*/
	unsigned long ulRecvFrameUCast;	/* 上行单播帧数*/
	unsigned long ulSendFrameUCast;	/* 下行单播帧数*/
	unsigned long ulRecvFrameMCast;	/* 上行非单播的帧数*/
	unsigned long ulSendFrameMCast;	/* 下行非单播的帧数*/
	
	long		  apSnrBSSIDAverageSignalStrength; 	 /*AP  的信号平均强度*/
	unsigned long apSnrBSSIDSignalPkts;				 /*AP  总的信令包数  */
	unsigned long apSnrBSSIDTxSignalPkts;			 /*AP  发射的信令包数*/    /* 发送的管理帧数 */
	unsigned long apSnrBSSIDRxSignalPkts;			 /*AP  接收的信令包数*/    /* 接收的管理帧数 */
	long		  apSnrBSSIDHighestRxSignalStrength; /*AP  的最高信号强度*/
	long 		  apSnrBSSIDLowestRxSignalStrength;  /*AP  的最低信号强度*/
	unsigned long apChStatsChannel;					 /*AP  当前占用的频点*/
	unsigned long apChStatsNumStations;				 /*使用该信道的终端数*/	

	/* 2010/08/06 fengjing cmcc test */
	unsigned long ulSendFragRate; /* bps */ /* 帧的分段速率 */
} OMA_AIR_STATS_INFO_DRV;

typedef struct
{
	unsigned long ulValidVapNum;
	OMA_AIR_STATS_INFO_DRV astAirStat[apVapTable_SIZE];
} OMA_AIR_STATS_ENTRY_DRV;

/* 2010-4-11 pyy 辽宁电信网管测试新增: 无线设备信道接收异常统计*/
typedef struct{

	/* 2010/07/01 fengjing cmcc test */
	unsigned long  ulRecvErrPackets;        /* 接收的错误数据包数 */

    unsigned long  ulRecvErrPhy;			/* 信道接收的物理层错包数 */
    unsigned long  ulRecvErrFcs;			/* 信道接收的FCS MAC错包数*/
    unsigned long  ulRecvErrMic;			/* 信道接收的MIC MAC错包数*/
    unsigned long  ulRecvErrDecrypt;		/* 信道接收的解密失败的MAC错包数*/
    unsigned long  ulRecvErr;				/* 信道接收的错帧数*/

	unsigned long ulSnr;
    unsigned long ulChannelReelect;
	
	unsigned long ulSendFragRate;    /* 分片速率 */			 
}OMA_IC_STAT_INFO_T;

typedef struct
{
	unsigned long ulValidVdevNum;
	OMA_IC_STAT_INFO_T	astIcStat[wifiDeviceTable_SIZE];		
}OMA_IC_STAT_ENTRY;

/* 2010-08-26 fengjing 新增:获取邻居AP信息 */
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
    unsigned char aucMac[6];            /*本AP MAC*/
    unsigned char aucMacPad[2];
    unsigned long ulChannel;            /*本AP CHANNEL*/
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

/* 2010/05/15 fengjing 中国电信测试新增:管理统计 */
typedef struct
{
	char acApSsid[32];
	 /*
	因无效而导致用户链路验证失败的次数
	因超时而导致用户链路验证失败的次数
	因拒绝而导致用户链路验证失败的次数
	其它原因导致用户链路验证失败的次数
	因无效而导致用户关联失败的次数
	因超时而导致用户关联失败的次数
	因拒绝而导致用户关联失败的次数
	其它原因导致用户关联失败的次数
	用户重关联尝试次数
	用户重关联成功次数（可选）
	因无效而导致用户重关联失败的次数
	因超时而导致用户重关联失败的次数
	因拒绝而导致用户重关联失败的次数
	其它原因导致用户重关联失败的次数
	用户鉴别尝试次数
	用户鉴别成功次数
	因密码错误而导致用户鉴别失败的次数
	因无效而导致用户鉴别失败的次数
	因超时而导致用户鉴别失败的次数
	因拒绝而导致用户鉴别失败的次数
	其它原因导致用户鉴别失败的次数
	解链路验证的次数
	因用户离开而导致解链路验证的次数
	因AP能力不足而导致解链路验证的次数
	因异常情况而导致解链路验证的次数
	其它原因导致解链路验证的次数
	解关联的次数
	因用户离开而导致解关联的次数
	因AP能力不足而导致解关联的次数
	因异常情况而导致解关联的次数
	其它原因导致解关联的次数
	 */	  

	 // 2010/06/11 fengjing add
	 unsigned long ulAuthSucc;  //包含SHARED（WEP）鉴权通过
	 
     unsigned long ulAuthFailInvalid;
     unsigned long ulAuthFailTimeout;
     unsigned long ulAuthFailDeny;  //包含由于最大接入用户数限制原因
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
     unsigned long ulWpaSucc;       //WPA认证成功
     unsigned long ulWpaFailPwdError;  //WPA-PSK密钥错误码
     unsigned long ulWpaFailOther;

	 unsigned long ulAccessReq;   //用户请求接入次数
     unsigned long ulAccessResp; // AP响应用户接入请求次数
     unsigned long ulAccessSucc;  //用户成功接入次数，包含OPEN/SHARED，WPA，WAPI

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
	
     unsigned long ulStaAssoc ;           /*当前与AP关联的终端数*/
     unsigned long ulAllStaTime ;         /*自AP启动后，所有终端的在线时间总和，单位：秒*/
     unsigned long ulAssocTimes ;         /*关联总次数*/
     unsigned long ulAssocFailTimes ;     /*关联失败总次数*/ 
     unsigned long ulReassocTimes ;       /*重新关联总次数*/
     unsigned long ulDisassocBySta ;      /*系统启动以来终端异常断开连接的总次数*/
     unsigned long ulAssocFailResource ;  /*由于接入点资源有限而被拒绝关联的总次数*/
     unsigned long ulAssocFailBaseRate ;  /*因终端不支持基本速率集要求的所有速率而关联失败的总次数*/
     unsigned long ulAssocFailNotStd ;    /*由不在802.11标准制定范围内的原因而关联失败的总次数*/
     unsigned long ulAssocFailOther ;     /*未知原因而导致关联失败的总次数*/
     unsigned long ulAssocFailNotShift ;  /*由于之前的关联无法识别与转移而导致重新关联失败的总次数*/

	/* 2011-03-23 fengjing add:增加重关联失败次数 */
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

/* 2011-09-02 fengjing modify,部分字段类型从类型UINT32->UINT64 */
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
    unsigned long ulRecvBroadcastFrameNum;      /* 有线侧接收的广播帧数 */
    unsigned long ulRecvRetryFrameNum; 		    /* 有线侧接收的重传帧数 */
    unsigned long ulSendBroadcastFrameNum;	    /* 有线侧发送的广播帧数 */
    unsigned long ulSendRetryFrameNum;		    /* 有线侧发送的重传帧数 */
    unsigned long ulDownNum;					/* 以太网口禁用次数 */
    unsigned long ulUpNum;						/* 以太网口启用次数 */

	/* 2011-06-16 fengjing add */
	unsigned long long ulRecvDataBytes;   /* 有线侧接收数据包字节数 */
	unsigned long long ulSendDataBytes;   /* 有线侧发送数据包字节数 */

	/*add by zjj*/
	 unsigned long ulAPHeartBeatAvgTimeDelay;    /* AP向该AC发送心跳的平均延迟时间，单位毫秒 */
	  unsigned long ulEchoReqCn;         /* AP向该AC发送心跳请求总包数 */
	  unsigned long ulEchoRespLossCnt;   /* AP丢失该AC心跳响应总包数 */

}__attribute__ ((packed)) ETH_DEV_INFO_T;

typedef struct
{
	int iDevNum;					/* number of device */
	ETH_DEV_INFO_T astDevInfo[ENETINTERFACE_MAX_NUM];	/* device of info */
} NET_INFO_T;




typedef struct
{
	char acWtpName[64];                  /* 设备名称 */
	char acWtpNEId[32];                  /* 设备网元编码 */
	//unsigned long ulWtpDeviceType;	 /* 设备类型 */
	char acDeviceType[32];               /* 设备类型 */
	char acWtpSN[6];					 /* 设备序列号 */
	char acRsv[2];

	char acWtpLocation[64];              /* 设备位置 */
	char acWtpSwName[64];                /* 软件名称 */
	char acWtpSwVersion[32];             /* 软件版本 */
	char acWtpHwVersion[32];             /* 硬件版本 */
	char acWtpCpuInfo[128];				 /* CPU信息 */
	char acWtpMemInfo[128];				 /* 内存信息 */
	char acWtpManufacturer[32];			 /* 设备制造商 */
	char acWtpSwDeveloper[32];			 /* 软件开发商 */
	unsigned long ulCountryCode;		 /* 国家码 */
	unsigned long ulWtpMode;             /* 设备工作模式 */
	unsigned long ulWtpStatus;           /* 工作状态 */
    unsigned long ulWtpMoniMode;         /* Ap  监视模式 */
	unsigned char aucWtpIpAddr[4];		 /* 设备IP地址 */
	unsigned long ulIpOrigin;
	unsigned char uacWtpNetMask[4];
	unsigned char uacWtpDftGw[4];
	unsigned char uacDnsServer1[4];
	unsigned char uacDnsServer2[4];
	unsigned char uacWtpMacAddr[6];
	unsigned char uacPadding[2];
	unsigned long ulWtpTime;			 /* 设备时间 */
	unsigned long ulWtpUpTime;           /* 设备运行时间 */
    unsigned long ulWtpConnTime;         /* 设备上线时长 */
	
	unsigned long ulWtpMemoryTotal;      /* 内存总大小 */
	unsigned long ulWtpMemoryUsed;       /* 当前内存使用率 */
	unsigned long ulWtpMemoryUsedMax;    /* 最大内存使用率 */
	unsigned long ulWtpMemeoryUsedAverage; /* 平均内存使用率 */
	unsigned long ulWtpCpuUsed;          /* 当前CPU  使用率 */
    unsigned long ulWtpCpuUsedMax;       /* 最大CPU  使用率 */
    unsigned long ulWtpCpuUsedAverage;   /* 平均CPU  使用率 */
	unsigned long ulWtpFlashTotal;       /* 文件系统容量 */
	unsigned long ulWtpFlashRemain;      /* 文件系统剩余空间 */
	int           iWtpTemperature;		 /* 设备工作温度 */
	ETHER_ATTRIBUTE_T stEthAttribute[MAX_ETHER_NUM];
	unsigned long ulRadioNum;
	WIFI_ATTRIBUTE_T stRadioAttribute[MAX_WIFI_NUM];
#if 0 //removed by leijie 2009-04-29
	OMA_ADHOC_INFO_ENTRY	stAdhocInfoEntry;
	OMA_AP_INFO_ENTRY		stApInfoEntry;
#endif
	char aucResv[32];
} DEVICE_ABSTRACT_INFO_T;

/*DEVICE_ABSTRACT_INFO_T的简化结构*/
typedef struct
{
	char acWtpCpuInfo[32];				 /* CPU信息 */
	char acWtpMemInfo[32];				 /* 内存信息 */
	
	unsigned long ulWtpTime;                           /* 设备时间 */
	unsigned long ulWtpUpTime;           /* 设备运行时间 */
	unsigned long ulWtpConnTime;         /* 设备上线时长 */

	unsigned long ulWtpMemoryTotal;      /* 内存总大小 */
	unsigned long ulWtpMemoryUsed;       /* 当前内存使用率 */
	unsigned long ulWtpMemoryUsedMax;    /* 最大内存使用率 */
	unsigned long ulWtpMemeoryUsedAverage; /* 平均内存使用率 */
	unsigned long ulWtpCpuUsed;          /* 当前CPU  使用率 */
	unsigned long ulWtpCpuUsedMax;       /* 最大CPU  使用率 */
	unsigned long ulWtpCpuUsedAverage;   /* 平均CPU  使用率 */
	unsigned long ulWtpFlashTotal;       /* 文件系统容量 */
	unsigned long ulWtpFlashRemain;      /* 文件系统剩余空间 */
	int           iWtpTemperature;                 /* 设备工作温度 */

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

#define AGENT_TBL_INFO_NUM_MAX	64	/* AP上的最大关系表数量*/

/* AC下发的配置数据版本号与本地当前配置数据版本号相同, 不需要同步*/
#define AP_AGENT_DATAVER_SAME		0x11	


/* AP基本配置数据文件 结构*/
typedef struct
{
	unsigned long ulWtpId;			/* AP ID */
	unsigned long ulCountryCode;	/* 国家码*/
	unsigned long ulDeviceType;	    /* 设备类型*/
	unsigned char aucDeviceId[6];	/* 设备序列号*/
	unsigned char aucRsv0[2];		/* 填充字节*/

	unsigned long ulIpOrigin;		/* IP地址分配方式*/
	unsigned char aucIpAddr[4];		/* IP地址*/
	unsigned char aucNetMask[4];	/* 掩码*/
	unsigned char aucDefaultGw[4];	/* 缺省网关*/
	unsigned char aucPrimaryDns[4];	/* 主DNS服务器 */
	unsigned char aucSecondaryDns[4];	/* 从DNS服务器 */

	char			acSecret[32];		/* AC接入密码*/
	unsigned long ulDisType;			/* AC发现方式*/
	char		  	acAcURL[64];		/* AC URL */
}T_deviceBasicCfg;

/* 数据更新控制头*/
typedef struct
{
	unsigned long 		ulDeviceType;		/* 设备类型*/
	unsigned char		aucDeviceId[6];		/* 设备序列号*/
	unsigned char		aucResv[2];			/* 填充字节*/
	char				acCfgModVer[16];	/* 信息模型版本号*/
	unsigned long 		ulCfgVer;			/* 配置数据版本号*/
	unsigned long 		ulValidNum;			/* 发生变更的对象数目*/
	unsigned long 		aulHandle[AGENT_DATA_UPDATE_ID_MAX];	/* 发生变更对象标识列表*/
} T_agentDataUpdateHeader;

typedef struct
{
	unsigned long 		ulValid;		/* 是否有效*/
	unsigned long		ulId;		    /* 标识*/
    	void		*pfDataGen;        	/* capwap用于组织对象数据的函数指针*/
    	void		*pfDataUpdate;		/* oma用于处理数据的函数指针*/
} T_agentDataUpdateHandle;

typedef struct
{
	unsigned long	ulValid;			/* 是否有效*/
	char			acTblName[32];	    /* 关系表名*/
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

#define AP_AGENT_BASIC_CONFIG_FILE "/tmp/basiccfg.dat"		/* AP基本配置数据文件 */

#define AP_AGENT_TIMERID_CHECK_ACLINKSTATE		TIMER0		/* 检查AP-AC链路状态定时器*/
#define AP_AGENT_TIMERID_SEND_OMASTART			TIMER1		/* 发送AP OMA启动消息定时器*/
#define AP_AGENT_TIMERID_SEND_HEARTBEART			TIMER2		/* 心跳上报定时器*/
#define AP_AGENT_TIMERID_CHECK_ALARM				TIMER3		/* 检查告警/事件定时器*/
#define AP_AGENT_TIMERID_CHECK_PERF				TIMER4		/* 检查性能定时器*/
#define AP_AGENT_TIMERID_DELAY_SWUPGRADE		TIMER5		/* 启动版本升级下载定时器*/
#define AP_AGENT_TIMERID_CHECK_DHCPSTATE			TIMER6		/* 检查DHCP状态定时器*/
#define AP_AGENT_TIMERID_CHECK_DHCPIPCHG		TIMER7		/* 周期性检查DHCP续约情况*/
#define AP_AGENT_TIMERID_TIMERHANDLE				TIMER8
#define AP_AGENT_TIMERID_COLLECTPARAM_GET		TIMER9		/*获取性能采集参数*/
#define AP_AGENT_TIMERID_COLLECT_NORMAL			TIMER10		/*常规采集*/
#define AP_AGENT_TIMERID_COLLECT_RT				TIMER11		/*实时采集*/

/* BEGIN: Added by zhoushouya, 2012/5/21   PN:986 */
#define AP_AGENT_TIMERID_CHECK_L2WD				TIMER12 /* 检查l2wd模块是否和powerap的数据一致 */
/* END:   Added by zhoushouya, 2012/5/21 */

/* BEGIN: Added by lvjianlin, 2013/3/20   PN:BUG #2135 【IXP】【大容量环境】交换机重启或者AC重启后，部分三层发现的AP无法上线 */
#define AP_AGENT_TIMERID_AP_IP_CHANGE           TIMER13
/* END:   Added by lvjianlin, 2013/3/20   PN:BUG #2135 【IXP】【大容量环境】交换机重启或者AC重启后，部分三层发现的AP无法上线 */

/* BEGIN: Added by lvjianlin, 2013/6/19   PN:lte-fi  GPS ALARM  LOCAL PORTAL */
//#define AP_AGENT_TIMERID_LTE_CHECK_GPIO         TIMER14     /* 监控GPIO状态 */
//#define AP_AGENT_TIMERID_LTE_REBOOT_AP          TIMER15     /* AP重启定时器 */
#define AP_AGENT_TIMERID_CHECK_GPIO				TIMER14     /* BEGIN: Added by 董鹏, 2013/6/19   PN: 5s钟检测GPIO口状态定时器 */
#define AP_AGENT_TIMERID_CHECK_BATTERY			TIMER15     /* BEGIN: Added by 董鹏, 2013/6/19   PN: 5s钟检测GPIO口状态定时器 */
#define AP_AGENT_TIMERID_BATTERY_ALARM_CLEAR    TIMER16     
/* END:   Added by lvjianlin, 2013/6/19   PN:lte-fi  GPS ALARM  LOCAL PORTAL */

/* BEGIN: Added by lvjianlin, 2013/8/6   PN:lte-fi updatedata bug and report time */
#define AP_AGENT_TIMERID_REPORT_BSSID           TIMER17
/* END:   Added by lvjianlin, 2013/8/6   PN:lte-fi updatedata bug and report time */


/* BEGIN: Added by liyoukun, 2013/12/3   PN:task#0  GPS信息上报修改，不仅通过echo上报AC，还上报给GPS服务器 */
#define AP_AGENT_TIMERID_REPORT_GPSINFO           TIMER18
/* END:   Added by liyoukun, 2013/12/3   PN:task#0  GPS信息上报修改，不仅通过echo上报AC，还上报给GPS服务器 */

/* BEGIN: Added by liyoukun, 2014/1/20   PN:检测lte网络模式，切换后上报告警 */
#define AP_AGENT_TIMERID_CHECK_LTENETTYPE       TIMER19
/* END:   Added by liyoukun, 2014/1/20   PN:检测lte网络模式，切换后上报告警 */
/* 定时器消息*/
#define AP_AGENT_TIMEOUT_CHECK_ACLINKSTATE		EV_TIMER0    /*  检查AP-AC链路状态定时器超时消息*/
#define AP_AGENT_TIMEOUT_SEND_OMASTART			EV_TIMER1
#define AP_AGENT_TIMEOUT_SEND_HEARTBEART		EV_TIMER2
#define AP_AGENT_TIMEOUT_CHECK_ALARM			EV_TIMER3
#define AP_AGENT_TIMEOUT_CHECK_PERF				EV_TIMER4
#define AP_AGENT_TIMEOUT_DELAY_SWUPGRADE		EV_TIMER5
#define AP_AGENT_TIMEOUT_CHECK_DHCPSTATE		EV_TIMER6	/* 检查DHCP状态定时器*/
#define AP_AGENT_TIMEOUT_CHECK_DHCPIPCHG		EV_TIMER7	
#define AP_AGENT_TIMEOUT_TIMERHANDLE			EV_TIMER8
#define AP_AGENT_TIMEOUT_COLLECTPARAM_GET		EV_TIMER9
#define AP_AGENT_TIMEOUT_COLLECT_NORMAL			EV_TIMER10
#define AP_AGENT_TIMEOUT_COLLECT_RT				EV_TIMER11

/* BEGIN: Added by zhoushouya, 2012/5/21   PN:986 */
#define AP_AGENT_TIMEOUT_CHECK_L2WD			    EV_TIMER12 /* 检查l2wd模块是否和powerap的数据一致 */
/* END:   Added by zhoushouya, 2012/5/21 */

/* BEGIN: Added by lvjianlin, 2013/3/20   PN:BUG #2135 【IXP】【大容量环境】交换机重启或者AC重启后，部分三层发现的AP无法上线 */
#define AP_AGENT_TIMEOUT_AP_IP_CHANGE           EV_TIMER13
/* END:   Added by lvjianlin, 2013/3/20   PN:BUG #2135 【IXP】【大容量环境】交换机重启或者AC重启后，部分三层发现的AP无法上线 */

/* BEGIN: Added by lvjianlin, 2013/6/19   PN:lte-fi  GPS ALARM  LOCAL PORTAL */
//#define AP_AGENT_TIMEOUT_LTE_CHECK_GPIO         EV_TIMER14
//#define AP_AGENT_TIMEOUT_LTE_REBOOT_AP          EV_TIMER15
#define AP_AGENT_TIMEOUT_CHECK_GPIO			    EV_TIMER14 /* BEGIN: Added by 董鹏, 2013/6/19   PN:定时5s检测GPIO口状态超时消息 */
#define AP_AGENT_TIMEOUT_CHECK_BETTRY			EV_TIMER15 
#define AP_AGENT_TIMEOUT_BATTERY_ALARM_CLEAR    EV_TIMER16
/* END:   Added by lvjianlin, 2013/6/19   PN:lte-fi  GPS ALARM  LOCAL PORTAL */

/* BEGIN: Added by lvjianlin, 2013/8/6   PN:lte-fi updatedata bug and report time */
#define AP_AGENT_TIMEOUT_REPORT_BSSID           EV_TIMER17
/* END:   Added by lvjianlin, 2013/8/6   PN:lte-fi updatedata bug and report time */

/* BEGIN: Added by liyoukun, 2013/12/3   PN:task#0  GPS信息上报修改，不仅通过echo上报AC，还上报给GPS服务器 */
#define AP_AGENT_TIMEOUT_REPORT_GPSINFO         EV_TIMER18
/* END:   Added by liyoukun, 2013/12/3   PN:task#0  GPS信息上报修改，不仅通过echo上报AC，还上报给GPS服务器 */
/* BEGIN: Added by liyoukun, 2014/1/20   PN:检测lte网络模式，切换后上报告警 */
#define AP_AGENT_TIMEOUT_LTE_NETTYPE            EV_TIMER19
/* END:   Added by liyoukun, 2014/1/20   PN:检测lte网络模式，切换后上报告警 */
#define AP_AGENT_TIMERLEN_CHECK_ACLINKSTATE	  	20			/* 单位:100ms */
#define AP_AGENT_TIMERLEN_SEND_OMASTART			100			/* 单位:100ms */
#define AP_AGENT_TIMERLEN_SEND_HEARTBEART		50
#define AP_AGENT_TIMERLEN_CHECK_ALARM			10
#define AP_AGENT_TIMERLEN_CHECK_PERF				60
#define AP_AGENT_TIMERLEN_DELAY_SWUPGRADE		50
#define AP_AGENT_TIMERLEN_CHECK_DHCPSTATE		10	
#define AP_AGENT_TIMERLEN_CHECK_DHCPIPCHG		300			/* 单位:100ms */
#define AP_AGENT_TIMERLEN_TIMERHANDLE			10			/* 单位:100ms */
#define AP_AGENT_TIMERLEN_COLLECTPARAM_GET_INIT	50
#define AP_AGENT_TIMERLEN_COLLECTPARAM_GET		3000
#define AP_AGENT_TIMERLEN_COLLECT_NORMAL		100
#define AP_AGENT_TIMERLEN_COLLECT_RT				50
#define AP_AGENT_TIMERLEN_CHECK_L2WD			3000

/* BEGIN: Added by lvjianlin, 2013/3/20   PN:BUG #2135 【IXP】【大容量环境】交换机重启或者AC重启后，部分三层发现的AP无法上线 */
#define AP_AGENT_TIMERLEN_CHECK_IPCHANGE_RES    30
/* END:   Added by lvjianlin, 2013/3/20   PN:BUG #2135 【IXP】【大容量环境】交换机重启或者AC重启后，部分三层发现的AP无法上线 */

/* BEGIN: Added by lvjianlin, 2013/6/21   PN:lte-fi  GPS ALARM  LOCAL PORTAL */
#define AP_AGENT_TIMERLEN_CHECK_GPIO			50          /* BEGIN: Added by 董鹏, 2013/6/19   PN:定时5s检测GPIO口状态超时消息 */
#define AP_AGENT_TIMERLEN_CHECK_BATTERY			12000       /* BEGIN: Added by 董鹏, 2013/6/19   PN:定时20min上报告警消息 */
#define AP_AGENT_TIMERLEN_BATTERY_ALARM_CLEAN   600
/* END:   Added by lvjianlin, 2013/6/21   PN:lte-fi  GPS ALARM  LOCAL PORTAL */

/* BEGIN: Added by liyoukun, 2013/12/3   PN:task#0  GPS信息上报修改，不仅通过echo上报AC，还上报给GPS服务器 */
#define AP_AGENT_TIMERLEN_REPORT_GPSINFO		100          /* 单位：100ms，默认GPS上报周期 */
/* END:   Added by liyoukun, 2013/12/3   PN:task#0  GPS信息上报修改，不仅通过echo上报AC，还上报给GPS服务器 */
/* BEGIN: Added by lvjianlin, 2013/8/6   PN:lte-fi updatedata bug and report time */
#define AP_AGENT_TIMERLEN_REPORT_BSSID          300
/* END:   Added by lvjianlin, 2013/8/6   PN:lte-fi updatedata bug and report time */
/* BEGIN: Added by liyoukun, 2014/1/20   PN:检测lte网络模式，切换后上报告警 */
#define AP_AGENT_TIMERLEN_CHECK_LTE_NETTYPE         50      /* 检测周期5s，单位100ms */
/* END:   Added by liyoukun, 2014/1/20   PN:检测lte网络模式，切换后上报告警 */
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
/* BEGIN: Added by 吕建林, 2012/12/13   PN:TASK #257 VAP级流控配置功能 商用版新增 */
extern unsigned int     g_ulVapMacFlowCtrlSetupId;
/* END:   Added by 吕建林, 2012/12/13   PN:TASK #257 VAP级流控配置功能 商用版新增 */
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
/* BEGIN: Added by 吕建林, 2012/12/11   PN:TASK #257 VAP级流控配置功能 商用版新增 */
extern unsigned int ap_agent_set_flowctrl(void);

/* END:   Added by 吕建林, 2012/12/11   PN:TASK #257 VAP级流控配置功能 商用版新增 */

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
/* BEGIN: Added by 吕建林, 2012/12/6   PN:TASK #257 VAP级流控配置功能 商用版新增 */
extern unsigned int     ac_cw_wtp_gen_agentdat_vapmacflowctrlsetup(T_configFileData * pstData);
/* END:   Added by 吕建林, 2012/12/6   PN:TASK #257 VAP级流控配置功能 商用版新增 */

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

