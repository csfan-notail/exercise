/******************************************************************************
* COPYRIGHT GBCOM
*******************************************************************************
* ��    ��: 
* �޸���ʷ: 
* 2012-9-10     �뾲              �½�
******************************************************************************/
#ifndef __MINI_SNMPD_TRAP__
#define __MINI_SNMPD_TRAP__

#include "ap_types.h"

/******************************* ���� ********************************/
/*�澯������*/
#define OPEN_MIB_ALARM_LEVEL_FATAL      1   /* ���� */
#define OPEN_MIB_ALARM_LEVEL_MAIN       2   /* ��Ҫ */
#define OPEN_MIB_ALARM_LEVEL_MINOR      3   /* ��Ҫ */
#define OPEN_MIB_ALARM_LEVEL_NORMAL     4   /* һ�� */
#define OPEN_MIB_ALARM_LEVEL_UNKNOW     5   /* ��ȷ�� */

/*�澯���Ͷ���*/
#define OPEN_MIB_ALARM_TYPE_COMMUNICATION  ((UINT32)1) /* ͨѶ�澯 */
#define OPEN_MIB_ALARM_TYPE_ENVIRONMENT    ((UINT32)2) /* �����澯 */
#define OPEN_MIB_ALARM_TYPE_DEVICE         ((UINT32)3) /* �豸�澯 */
#define OPEN_MIB_ALARM_TYPE_HANDLEFAIL     ((UINT32)4) /* �������澯 */
#define OPEN_MIB_ALARM_TYPE_QOS            ((UINT32)5) /* ���������澯 */
#define OPEN_MIB_ALARM_TYPE_OTHER          ((UINT32)6) /* �����澯 */

/*mib�и澯״̬����*/
#define ALARM_STATUS_ACTIVE         ((UINT32)1)
#define ALARM_STATUS_CLEAR          ((UINT32)2)

#define OPEN_MIB_TRAP_DES_MAX_NUM       ((UINT32)12)


typedef struct 
{
    UINT32      ulAlarmSn;              /* �澯�����к� */
    CHAR        acAlarmNeName[32];      /* ��Ԫ��ʶ�� */
    UINT32      ulAlarmLevel;           /* �澯ԭʼ���� */
    UINT32      ulAlarmType;            /* �澯ԭʼ���ͣ�����ͨѶ�澯�������澯���豸�澯���������澯�����������澯�� */
    UINT32      ulAlarmReasonID;        /* �澯ԭ���, ʶ�澯ԭ����ڲ��澯�ţ���ѡ��*/
    CHAR        acAlarmReason[64];      /* �澯ԭ�򣨿�ѡ��*/
    UINT32      ulAlarmRaiseTime;       /* �澯����ʱ��	TimeTicks(0.01��) */
    UINT32      ulAlarmStatus;          /* �澯״̬	1:�״̬��0:��� */
    CHAR        acAlarmTitle[64];       /* �澯����	*/
    CHAR        acAlarmInfo[128];       /* �澯����	*/
	OID         *pstOID;
	UINT32      ulOIDLen;
}OPEN_MIB_ALARM_TRAP_T;

typedef struct
{
	CHAR        aucSn[CHAR_LEN_32];
	CHAR        aucDevType[CHAR_LEN_32];
    CHAR        acSysTime[CHAR_LEN_32];             /* CPE��ϵͳʱ�� */
    UINT32      uiSysUptime;           /*�豸�ϵ�ʱ��*/
	CHAR        aucTargetVersion[CHAR_LEN_32];
	CHAR        aucCurVersion[CHAR_LEN_32];
	CHAR        aucHwVersion[CHAR_LEN_32];
    UINT8       aucManagerIp[IP_ALEN];
	UINT8       aucManagerMac[ETH_ALEN];
	UINT32      uiManageState;                      /* ����״̬��1:discover 2:update image 3:running 4:quit*/
    UINT8       aucManageIpv6[CHAR_LEN_64];                /* �����IPV6��ַ */
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



/******************************* �澯ͨ�� ********************************/
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

/******************************* ��������********************************/
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
