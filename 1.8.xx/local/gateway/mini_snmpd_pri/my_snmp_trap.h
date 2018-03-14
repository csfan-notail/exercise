/******************************************************************************
* COPYRIGHT GBCOM
*******************************************************************************
* 功    能: 
* 修改历史: 
* 2012-9-10     冯静              新建
******************************************************************************/
#ifndef __MINI_SNMPD_TRAP__
#define __MINI_SNMPD_TRAP__

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
BOOL test_snmp_trap(VOID);
BOOL agent_send_apswupdatetrap(VOID);
BOOL agent_send_dev_offline_trap(VOID);
BOOL agent_start_discover(VOID);
BOOL agent_send_heart_beat_trap(VOID);
#endif
