/******************************************************************************
* COPYRIGHT GBCOM
*******************************************************************************
* ��    ��: 
* �޸���ʷ: 
* 2012-9-10     �뾲              �½�
******************************************************************************/
#ifndef __MINI_SNMPD_TRAP__
#define __MINI_SNMPD_TRAP__

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
BOOL test_snmp_trap(VOID);
BOOL agent_send_apswupdatetrap(VOID);
BOOL agent_send_dev_offline_trap(VOID);
BOOL agent_start_discover(VOID);
BOOL agent_send_heart_beat_trap(VOID);
#endif
