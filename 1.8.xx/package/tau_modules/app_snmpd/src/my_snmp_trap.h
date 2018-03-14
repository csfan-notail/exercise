/******************************************************************************
* COPYRIGHT GBCOM
*******************************************************************************
* 功    能: 
* 修改历史: 
* 2012-9-10     冯静              新建
******************************************************************************/
#ifndef __MINI_SNMPD_TRAP__
#define __MINI_SNMPD_TRAP__

#include "ap_types.h"

/******************************* 定义 ********************************/
/*告警级别定义*/
#define OPEN_MIB_ALARM_LEVEL_FATAL      1   /* 严重 */
#define OPEN_MIB_ALARM_LEVEL_MAIN       2   /* 重要 */
#define OPEN_MIB_ALARM_LEVEL_MINOR      3   /* 次要 */
#define OPEN_MIB_ALARM_LEVEL_NORMAL     4   /* 一般 */
#define OPEN_MIB_ALARM_LEVEL_UNKNOW     5   /* 不确定 */

/*告警类型定义*/
#define OPEN_MIB_ALARM_TYPE_COMMUNICATION  ((UINT32)1) /* 通讯告警 */
#define OPEN_MIB_ALARM_TYPE_ENVIRONMENT    ((UINT32)2) /* 环境告警 */
#define OPEN_MIB_ALARM_TYPE_DEVICE         ((UINT32)3) /* 设备告警 */
#define OPEN_MIB_ALARM_TYPE_HANDLEFAIL     ((UINT32)4) /* 处理错误告警 */
#define OPEN_MIB_ALARM_TYPE_QOS            ((UINT32)5) /* 服务质量告警 */
#define OPEN_MIB_ALARM_TYPE_OTHER          ((UINT32)6) /* 其他告警 */

/*mib中告警状态定义*/
#define ALARM_STATUS_ACTIVE         ((UINT32)1)
#define ALARM_STATUS_CLEAR          ((UINT32)2)

#define OPEN_MIB_TRAP_DES_MAX_NUM       ((UINT32)12)


typedef struct 
{
    UINT32      ulAlarmSn;              /* 告警的序列号 */
    CHAR        acAlarmNeName[32];      /* 网元标识名 */
    UINT32      ulAlarmLevel;           /* 告警原始级别 */
    UINT32      ulAlarmType;            /* 告警原始类型，包括通讯告警、环境告警、设备告警、处理错误告警、服务质量告警等 */
    UINT32      ulAlarmReasonID;        /* 告警原因号, 识告警原因的内部告警号（可选）*/
    CHAR        acAlarmReason[64];      /* 告警原因（可选）*/
    UINT32      ulAlarmRaiseTime;       /* 告警发生时间	TimeTicks(0.01秒) */
    UINT32      ulAlarmStatus;          /* 告警状态	1:活动状态，0:清除 */
    CHAR        acAlarmTitle[64];       /* 告警标题	*/
    CHAR        acAlarmInfo[128];       /* 告警内容	*/
	OID         *pstOID;
	UINT32      ulOIDLen;
}OPEN_MIB_ALARM_TRAP_T;

typedef struct
{
	CHAR        aucSn[CHAR_LEN_32];
	CHAR        aucDevType[CHAR_LEN_32];
    CHAR        acSysTime[CHAR_LEN_32];             /* CPE的系统时间 */
    UINT32      uiSysUptime;           /*设备上电时间*/
	CHAR        aucTargetVersion[CHAR_LEN_32];
	CHAR        aucCurVersion[CHAR_LEN_32];
	CHAR        aucHwVersion[CHAR_LEN_32];
    UINT8       aucManagerIp[IP_ALEN];
	UINT8       aucManagerMac[ETH_ALEN];
	UINT32      uiManageState;                      /* 管理状态：1:discover 2:update image 3:running 4:quit*/
    UINT8       aucManageIpv6[CHAR_LEN_64];                /* 管理口IPV6地址 */
    UINT32      ulWanMode;
    UINT32      uiDailWay;
    CHAR        aucApnSelect[CHAR_LEN_32];
    UINT32      uiAccessMode;
    UINT32      uiPriAccessMode;
    UINT32      uiSimLock;
    CHAR        aucModuleType[CHAR_LEN_32];
    CHAR        aucModuleVer[CHAR_LEN_128];
    CHAR        aucSimICCID[CHAR_LEN_32];
    CHAR        aucModuleIMEI[CHAR_LEN_32];
    CHAR        aucSimIMSI[CHAR_LEN_32];

    UINT32      uiSimState;
    UINT8       aucOperCompany[CHAR_LEN_32];
    UINT32      uiLteSigRssi;
    UINT32      uiLteSigRsrp;
    UINT32      uiLteSigRsrq;
    UINT32      uiLteSigSinr;
    UINT32      uiLteSigLevel;
    UINT32      uiAccessNet;
    CHAR        aucLteBand[CHAR_LEN_32];
    UINT32      uiLteChannel;
    CHAR        aucLteCID[CHAR_LEN_32];
    CHAR        aucLtePCID[CHAR_LEN_32];
    CHAR        aucIp[IP_ALEN]; 
    CHAR        aucIp6[IPV6_ALEN]; 
    CHAR        aucLteCellId[128];
    CHAR        aucLanPortSta[CHAR_LEN_64];
}DISCOVER_TRAP_T;

/*private mib*/
#define OID_PRIVATE                         4
#define OID_ENTERPRISES                     OID_PRIVATE, 1
#define OID_DLINK                           OID_ENTERPRISES, 171
#define OID_DLINK_WLAN                      OID_DLINK, 10
//#define OID_DLINK_AP_MIB                    OID_DLINK_WLAN, 1

#define OID_DLINK_AP_MIB                    1



#define OID_DLINK_AP_OBJECT_GROUP           OID_DLINK_AP_MIB, 15
#define OID_DLINK_AP_OBJECT                 OID_DLINK_AP_OBJECT_GROUP, 11

#define OID_AP_ALARM_TRAP_OBJECT            OID_DLINK_AP_OBJECT, 1
#define OID_AP_ALARM                        OID_DLINK_AP_OBJECT, 12
/**
 * alarm
 */
#define OID_AP_ALARM_TRAP                   OID_AP_ALARM, 10

#define OID_AP_ALARM_SYS_TRAP               OID_AP_ALARM_TRAP, 1
#define OID_AP_ALARM_WIDS_TRAP              OID_AP_ALARM_TRAP, 2



#define OID_VENDOR                  28723

#define OID_VENDOR_WLAN               OID_VENDOR, 1

#define OID_VENDOR_CPE                OID_VENDOR_WLAN, 15 

#define OID_CPE_CONFIG         OID_VENDOR_CPE, 11

#define OID_CPE_CONFIG_SYS               OID_CPE_CONFIG, 1
#define OID_CPE_SW_INFO                  OID_CPE_CONFIG_SYS, 3


//static SNMP_OID oid_cpe_sw_currVer[] = {OID_CPE_SW_INFO, 3};



/******************************* 告警通告 ********************************/
static SNMP_OID oid_snmp_trap[]                 = {1,3,6,1,6,3,1,1,4,1,0};

static SNMP_OID oid_ap_alarmSn[]                = {OID_AP_ALARM_TRAP_OBJECT, 3, 3};

static SNMP_OID oid_ap_alarmNeName[]            = {OID_AP_ALARM_TRAP_OBJECT, 2, 0};
static SNMP_OID oid_ap_alarmLevel[]             = {OID_AP_ALARM_TRAP_OBJECT, 3, 0};
static SNMP_OID oid_ap_alarmType[]              = {OID_AP_ALARM_TRAP_OBJECT, 4, 0};
static SNMP_OID oid_ap_alarmReasonID[]          = {OID_AP_ALARM_TRAP_OBJECT, 5, 0};
static SNMP_OID oid_ap_alarmReason[]            = {OID_AP_ALARM_TRAP_OBJECT, 6, 0};
static SNMP_OID oid_ap_alarmRaiseTime[]         = {OID_AP_ALARM_TRAP_OBJECT, 7, 0};
static SNMP_OID oid_ap_alarmStatus[]            = {OID_AP_ALARM_TRAP_OBJECT, 8, 0};
static SNMP_OID oid_ap_alarmTitle[]             = {OID_AP_ALARM_TRAP_OBJECT, 9, 0};
static SNMP_OID oid_ap_alarmInfo[]              = {OID_AP_ALARM_TRAP_OBJECT, 10, 0};

static SNMP_OID oid_ap_alarm_sys_apWarmStart_trap[]         = {OID_AP_ALARM_SYS_TRAP, 1, 0};
//static SNMP_OID oid_ap_alarm_sys_apWarmStartClear_trap[]         = {OID_AP_ALARM_SYS_TRAP, 2, 0};
static SNMP_OID oid_ap_alarm_sys_apColdStart_trap[]         = {OID_AP_ALARM_SYS_TRAP, 2, 0};
//static SNMP_OID oid_ap_alarm_sys_apColdStartClear_trap[]         = {OID_AP_ALARM_SYS_TRAP, 4, 0};
static SNMP_OID oid_ap_alarm_sys_apCpuUsageTooHigh_trap[]         = {OID_AP_ALARM_SYS_TRAP, 3, 0};
static SNMP_OID oid_ap_alarm_sys_apCpuUsageTooHighClear_trap[]         = {OID_AP_ALARM_SYS_TRAP, 4, 0};
static SNMP_OID oid_ap_alarm_sys_apMemUsageTooHigh_trap[]         = {OID_AP_ALARM_SYS_TRAP, 5, 0};
static SNMP_OID oid_ap_alarm_sys_apMemUsageTooHighClear_trap[]         = {OID_AP_ALARM_SYS_TRAP, 6, 0};

static SNMP_OID oid_ap_alarm_wids_cointerfre_trap[]         = {OID_AP_ALARM_WIDS_TRAP, 1, 0};
static SNMP_OID oid_ap_alarm_wids_cointerfreClear_trap[]         = {OID_AP_ALARM_WIDS_TRAP, 2, 0};
static SNMP_OID oid_ap_alarm_wids_neinterfre_trap[]         = {OID_AP_ALARM_WIDS_TRAP, 3, 0};
static SNMP_OID oid_ap_alarm_wids_neinterfreClear_trap[]         = {OID_AP_ALARM_WIDS_TRAP, 4, 0};
static SNMP_OID oid_ap_alarm_wids_wlanDevInterfre_trap[]         = {OID_AP_ALARM_WIDS_TRAP, 5, 0};
static SNMP_OID oid_ap_alarm_wids_wlanDevInterfreClear_trap[]         = {OID_AP_ALARM_WIDS_TRAP, 6, 0};
static SNMP_OID oid_ap_alarm_wids_otherDevInterfre_trap[]         = {OID_AP_ALARM_WIDS_TRAP, 7, 0};
static SNMP_OID oid_ap_alarm_wids_otherDevInterfreClear_trap[]         = {OID_AP_ALARM_WIDS_TRAP, 8, 0};
/* END: Added by zoucaihong, 2013/1/23 */

#define SNMP_INIT_TRAP_VAR(pstVarList, oid_trap, oid_len)                                        \
{                                                                                       \
    if (-1 == snmp_add_trap_var(&pstVarList, oid_snmp_trap, OID_LENGTH(oid_snmp_trap),\
                                  TYPE_OID, oid_trap, oid_len))                \
    {                                                                                   \
        return FALSE;                                                                   \
    }                                                                                   \
}

#define SNMP_ADD_TRAP_VAR(pstVarList, pstOID, ucVarType, var)                       \
{                                                                                   \
    if (TYPE_CHAR == ucVarType)                                                     \
    {                                                                               \
        if (-1 == snmp_add_trap_var(&pstVarList, pstOID, OID_LENGTH(pstOID),      \
                                      ucVarType, (VOID*)var, strlen((CHAR*)(var)))) \
        {                                                                           \
           return FALSE;                                                            \
        }                                                                           \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        if (-1 == snmp_add_trap_var(&pstVarList, pstOID, OID_LENGTH(pstOID),      \
                                      ucVarType, (VOID*)&(var), sizeof(var)))       \
        {                                                                           \
           return FALSE;                                                            \
        }                                                                           \
    }                                                                               \
}

/******************************* 函数声明********************************/
int snmp_udp_send(unsigned char *pucServerIP, unsigned short usServerPort, unsigned int ulTimeout,
                  void *pstReqMsg, unsigned int ulReqMsgLen,
                  void *pRcvMsg,   unsigned int ulMaxRcvMsgLen, unsigned int *pulRcvMsgLen,
                  SNMP_TLV_T	*pstReqID);
BOOL snmp_send_discover_trap(SNMP_AGENT_DATA_T *g_pstSnmpAgentData);
BOOL snmp_send_inform_trap(SNMP_AGENT_DATA_T *pstSnmpData);
//BOOL snmp_send_northbound_inform_trap(SNMP_AGENT_DATA_T *pstSnmpData);

BOOL snmp_send_update_trap(SNMP_AGENT_DATA_T* pstSnmpData);
BOOL snmp_trap(SNMP_AGENT_DATA_T* pstSnmpAgentData);

int is_ip_address(const char *s);
BOOL get_ip_by_url(const char *name, UINT32 *res);
void ipToNum(CHAR* ip, UINT8 *pucSections);


#endif
