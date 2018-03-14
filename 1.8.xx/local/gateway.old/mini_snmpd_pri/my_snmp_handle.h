/******************************************************************************
* COPYRIGHT GBCOM
*******************************************************************************
* ��    ��: 
* �޸���ʷ: 
* 2012-9-10     �뾲              �½�
******************************************************************************/

/******************************* �궨�� ********************************/
#ifndef __MINI_SNMPD_HANDLE__
#define __MINI_SNMPD_HANDLE__

/******************************* ��״̬���� ********************************/
#define SNMP_COLUMN_STATE_NULL      0
#define SNMP_COLUMN_STATE_SET       1

#define SNMP_TABLE_STATE_NULL       0   
#define SNMP_TABLE_STATE_SET        1   /* ������SET */
#define SNMP_TABLE_STATE_SETALL     2   /* �����ж��Ѿ���SET */

#define SNMP_TABLE_STATE_WAIT      3   /* ����/ȫ����SET����RowStatus��CreateAndWait */


/******************************* �������� ********************************/
#define MAX_SNMP_OID_SIZE	    32//32->MAX_NR_SUBIDS
#define MAX_TABLE_COLUMN_NUM    40
#define MAX_KEY_COLUMN_NUM      4
//#define MAX_TABLE_COLUMN_ENUM_NUM 20

/******************************* �ṹ���� ********************************/
typedef BOOL (SNMP_KEY_VALID_FUNC)(UINT32 ulTableID, UINT32 ulTableRecLen, VOID *pKey);
typedef BOOL (SNMP_TABLE_VALID_FUNC)(VOID *pData, VOID *pOriData);

typedef struct
{
	UINT32   ulValue;
	CHAR     acDisValue[32];
}SNMP_COLUMN_ENUM_T;

typedef struct 
{
    UINT16          usColumnID;
    UINT8           ucColumnType;
	UINT8           ucMibOper;

    UINT16          usColumnLen;
    UINT16          usOffSet;

    //CHAR            acColumnName[32];
	//UINT32          ulEnumNum;
	//SNMP_COLUMN_ENUM_T astColumnEnums[MAX_TABLE_COLUMN_ENUM_NUM];
	INT32           lMin;
	INT32           lMax;
	//CHAR            acUnits[32];

	/* ����ȱʡֵ */
	UINT32         ulDefValue;
	INT32           lDefValue;
	UINT8           *pucDefValue;
}SNMP_COLUMN_T;

typedef struct 
{
    UINT32              ulTableID;
    CHAR                acTableName[32];
	BOOL				bIsTable;		

    UINT32              ulOIDSize;          /* OID���� */
    OID                 astOID[MAX_SNMP_OID_SIZE];

    UINT32              ulMaxTableRecNum;   /* ����¼�� */
    UINT32              ulTableRecLen;      /* ��Ӧ�ı�ṹ���� */

    SNMP_KEY_VALID_FUNC     *pfKeyValidFunc;
    SNMP_TABLE_VALID_FUNC   *pfTableValidFunc;

    UINT32              ulColumnNum;
    UINT32              ulKeyColumnNum;

    SNMP_COLUMN_T       astColumns[MAX_TABLE_COLUMN_NUM];
    SNMP_COLUMN_T       *apstKeyColumns[MAX_KEY_COLUMN_NUM];

    VOID                *pTableData;
    UINT32              ulTableRecNum;      /* ��ǰ��¼�� */

    VOID                *pOriTableData;     /* �޸�ǰ��ԭʼ���� */

    VOID                *pNewTableData;     /* ���Ӳ���ʱ����ʱ���¼ */

    UINT32              ulSetStartTime;     /* table add��¼�Ŀ�ʼʱ�� */
    UINT32              aulColumnState[MAX_TABLE_COLUMN_NUM];	
}SNMP_TABLE_T;

typedef struct 
{
    UINT32              ulTableNum;
    SNMP_TABLE_T        astTables[MAX_TABLE_NUM];
}SNMP_TABLE_INFO_T;


/******************************* �������� ********************************/

BOOL snmp_init_table_info(SNMP_TABLE_T  *pstTable,
                          BOOL          bIsTable,
                          UINT32        ulTableID,
                          CHAR          *szTableName,
                          SNMP_TABLE_VALID_FUNC* pfTableValidFunc,
                          UINT32        ulOIDSize,
                          OID           *pstOID,
                          UINT32        ulMaxTableRecNum,
                          UINT32        ulTableRecLen);

BOOL snmp_init_column_info(SNMP_TABLE_T  *pstTable,
						   CHAR          *szColumnName,
                           UINT8         ucColumnType,                           
                           UINT16        usColumnLen,
                           UINT16        usOffSet,
						   UINT8         ucMibOper,
                           BOOL          bIsKeyField);

BOOL snmp_init_column_enum_info(SNMP_TABLE_T  *pstTable, 
	                                      CHAR *szEnum);
BOOL snmp_init_column_range(SNMP_TABLE_T  *pstTable, 
                                    INT32 lMin, 
                                    INT32 lMax);
BOOL snmp_init_column_units(SNMP_TABLE_T  *pstTable, 
								   CHAR *szUnits);

BOOL snmp_init_column_defval(SNMP_TABLE_T  *pstTable, 
						   VOID *pValue, UINT32 ulValue);

BOOL snmp_get_column(SNMP_TABLE_T *pstTable,
                           UINT16    usColumnID,
                           oid_t     *pRow,
                           void      *data_context,
                           response_t *response, 
                           client_t *client);
BOOL snmp_set_column(UINT32     ulColumnType,
                     UINT32     ulColumnLen,
                     VOID       *pInValue,
                     UINT32     ulInLen,
                     VOID       *pOutValue);
int snmp_row_build(SNMP_TABLE_T *pstTable, 
						UINT32 ulColumnIndex, 
						CHAR  *pValueAddr, 
						oid_t *row);
SNMP_TABLE_T *snmp_get_next_table(SNMP_TABLE_T *pstPrevTable);

BOOL snmp_scalar_get_column(SNMP_TABLE_T *pstTable, 
							UINT32 ulColumnIndex, 
							response_t *response, 
							client_t *client);
BOOL snmp_table_get_column(SNMP_TABLE_T *pstTable, 
							UINT32 ulColumnIndex, 
							oid_t *row, 
							CHAR * pValueAddr, 
							response_t *response, 
							client_t *client);

CHAR *snmp_get_scalar_data(SNMP_TABLE_T *pstTable);
BOOL snmp_get_table_data(SNMP_TABLE_T *pstTable);

int handle_snmp_get(request_t *request, response_t *response, client_t *client);
int handle_snmp_getnext(request_t *request, response_t *response, client_t *client);
int handle_snmp_set(request_t *request, response_t *response, client_t *client);
int handle_snmp_getbulk(request_t *request, response_t *response, client_t *client);

int snmp_scalar_handler(SNMP_TABLE_T *pstTable, request_t *request, int i, response_t *response, client_t *client);
int snmp_table_handler(SNMP_TABLE_T *pstTable, request_t *request, int i, response_t *response, client_t *client);

#endif

