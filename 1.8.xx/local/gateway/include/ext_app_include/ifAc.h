#ifndef IF_AC_H
#define IF_AC_H

/* 类型定义 */
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

#define AC_DB_TBL_FIELD_NUM_MAX     ((UINT32)64)    /* 每个关系表允许包含的最大字段数 */
#define AC_DB_FIELD_NAME_LEN_MAX    ((UINT32)32)    /* 字段名最大长度 */
#define AC_DB_TBL_NAME_LEN_MAX      ((UINT32)32)    /* 关系表名称最大长度 */
#define AC_DB_FIELD_LEN_MAX         ((UINT16)256)   /* 单个字段的最大长度 */   

/* 字段类型 */
#define AC_DB_FIELD_TYPE_CHAR       ((CHAR)'C')     /* CHAR */
#define AC_DB_FIELD_TYPE_UINT8      ((CHAR)'N')     /* UINT8 */
#define AC_DB_FIELD_TYPE_UINT16     ((CHAR)'W')     /* UINT16 */
#define AC_DB_FIELD_TYPE_UINT32     ((CHAR)'D')     /* UINT32 */

/* 字段存盘标记 */
#define AC_DB_FIELD_NOT_SAVE        ((UINT8)0)      /* 字段不需要存盘 */
#define AC_DB_FIELD_SAVE            ((UINT8)1)      /* 字段需存盘 */

#define AC_DB_TMP_DATA_FILE_PATH    "/tmp/wtp.dat"       /* 临时数据文件目录 */
/* BEGIN: Added by 吕建林, 2012/11/24   PN:修改文件名/tmp/acLinkState为/tmp/aclinkstate */
#define AC_LINK_STATE_FILE_PATH		"/tmp/aclinkstate"   /* 保存WTP与AC的链路状态文件目录 */		
/* END:   Added by 吕建林, 2012/11/24   PN:修改文件名/tmp/acLinkState为/tmp/aclinkstate */

/* 其它宏定义 */
#define _packed_1_


/* 文件头结构 */
typedef struct
{
	UINT32  ulVersion;  /* 数据文件版本号 */
	UINT16  usTblNum;   /* 关系表数量 */
	UINT8   aucRsv[2];  /* 填充字节 */
}_packed_1_ AC_DB_FILE_HEADER_T;

/* 关系表头结构 */
typedef struct
{
	CHAR    acTblName[AC_DB_TBL_NAME_LEN_MAX];  /* 关系表名 */
	UINT16  usFieldNum;                         /* 字段数 */
	UINT16  usRecordLen;                        /* 记录长度, 不包含1字节的记录有效标记 */
	UINT32  ulRecordNum;                        /* 记录数 */
}_packed_1_ AC_DB_TBL_HEADER_T;

/* 字段结构体 */
typedef struct
{
	CHAR    acFieldName[AC_DB_FIELD_NAME_LEN_MAX];  /* 字段名 */
	UINT16  usFieldLen;                             /* 字段长度 */
	UINT16  usFieldOffset;                          /* 字段偏移量 */
	UINT8   ucFieldType;                            /* 字段类型 */
	UINT8   ucFieldTag;                             /* 是否为关键字 */
	UINT8   ucSave;                                 /* 是否需存盘 */
	UINT8   aucRsv[1];                              /* 填充字节 */
}_packed_1_ AC_DB_FIELD_T;

/* 字段缓冲区结构体 */
typedef struct
{
    UINT16  usFieldNum; /* 字段数 */
    UINT8   aucRsv[2];  /* 填充字节 */
    AC_DB_FIELD_T   astField[AC_DB_TBL_FIELD_NUM_MAX];  /* 字段信息 */
}_packed_1_ AC_DB_FIELD_BUFF_T;

extern UINT32 oma_db_save_file(VOID);
extern UINT32 oma_ac_link_state_get(VOID);
extern UINT32 oma_fat_fit_ap_handover(VOID);
extern UINT32 oma_set_vap_state(VOID);

#endif
