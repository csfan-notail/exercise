#ifndef IF_AC_H
#define IF_AC_H

/* ���Ͷ��� */
/*
typedef char  CHAR;
typedef unsigned char  UINT8;
typedef unsigned short UINT16;
typedef unsigned long  UINT32;

typedef char    INT8;
typedef short   INT16;
typedef int     INT32;
typedef int     BOOL;
typedef char*   STRING;

#define VOID void
*/

#define AC_DB_TBL_FIELD_NUM_MAX     ((UINT32)64)    /* ÿ����ϵ���������������ֶ��� */
#define AC_DB_FIELD_NAME_LEN_MAX    ((UINT32)32)    /* �ֶ�����󳤶� */
#define AC_DB_TBL_NAME_LEN_MAX      ((UINT32)32)    /* ��ϵ��������󳤶� */
#define AC_DB_FIELD_LEN_MAX         ((UINT16)256)   /* �����ֶε���󳤶� */   

/* �ֶ����� */
#define AC_DB_FIELD_TYPE_CHAR       ((CHAR)'C')     /* CHAR */
#define AC_DB_FIELD_TYPE_UINT8      ((CHAR)'N')     /* UINT8 */
#define AC_DB_FIELD_TYPE_UINT16     ((CHAR)'W')     /* UINT16 */
#define AC_DB_FIELD_TYPE_UINT32     ((CHAR)'D')     /* UINT32 */

/* �ֶδ��̱�� */
#define AC_DB_FIELD_NOT_SAVE        ((UINT8)0)      /* �ֶβ���Ҫ���� */
#define AC_DB_FIELD_SAVE            ((UINT8)1)      /* �ֶ������ */

#define AC_DB_TMP_DATA_FILE_PATH    "/tmp/wtp.dat"       /* ��ʱ�����ļ�Ŀ¼ */
/* BEGIN: Added by ������, 2012/11/24   PN:�޸��ļ���/tmp/acLinkStateΪ/tmp/aclinkstate */
#define AC_LINK_STATE_FILE_PATH		"/tmp/aclinkstate"   /* ����WTP��AC����·״̬�ļ�Ŀ¼ */		
/* END:   Added by ������, 2012/11/24   PN:�޸��ļ���/tmp/acLinkStateΪ/tmp/aclinkstate */

/* �����궨�� */
#define _packed_1_


/* �ļ�ͷ�ṹ */
typedef struct
{
	UINT32  ulVersion;  /* �����ļ��汾�� */
	UINT16  usTblNum;   /* ��ϵ������ */
	UINT8   aucRsv[2];  /* ����ֽ� */
}_packed_1_ AC_DB_FILE_HEADER_T;

/* ��ϵ��ͷ�ṹ */
typedef struct
{
	CHAR    acTblName[AC_DB_TBL_NAME_LEN_MAX];  /* ��ϵ���� */
	UINT16  usFieldNum;                         /* �ֶ��� */
	UINT16  usRecordLen;                        /* ��¼����, ������1�ֽڵļ�¼��Ч��� */
	UINT32  ulRecordNum;                        /* ��¼�� */
}_packed_1_ AC_DB_TBL_HEADER_T;

/* �ֶνṹ�� */
typedef struct
{
	CHAR    acFieldName[AC_DB_FIELD_NAME_LEN_MAX];  /* �ֶ��� */
	UINT16  usFieldLen;                             /* �ֶγ��� */
	UINT16  usFieldOffset;                          /* �ֶ�ƫ���� */
	UINT8   ucFieldType;                            /* �ֶ����� */
	UINT8   ucFieldTag;                             /* �Ƿ�Ϊ�ؼ��� */
	UINT8   ucSave;                                 /* �Ƿ������ */
	UINT8   aucRsv[1];                              /* ����ֽ� */
}_packed_1_ AC_DB_FIELD_T;

/* �ֶλ������ṹ�� */
typedef struct
{
    UINT16  usFieldNum; /* �ֶ��� */
    UINT8   aucRsv[2];  /* ����ֽ� */
    AC_DB_FIELD_T   astField[AC_DB_TBL_FIELD_NUM_MAX];  /* �ֶ���Ϣ */
}_packed_1_ AC_DB_FIELD_BUFF_T;

extern UINT32 oma_db_save_file(VOID);
extern UINT32 oma_ac_link_state_get(VOID);
extern UINT32 oma_fat_fit_ap_handover(VOID);
extern UINT32 oma_set_vap_state(VOID);

#endif
