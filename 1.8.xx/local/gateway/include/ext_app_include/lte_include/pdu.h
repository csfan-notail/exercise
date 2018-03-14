/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : pdu.h
  版 本 号   : 初稿
  作    者   : 曹瑜
  生成日期   : 2014年11月26日
  最近修改   :
  功能描述   : pdu.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2014年11月26日
    作    者   : 曹瑜
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>  
#include <unistd.h>
#include <unistd.h>
#include <fcntl.h>	
#include <sys/ioctl.h>	
 
#include "lte_log.h"

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

#ifndef __PDU_H__
#define __PDU_H__

typedef enum 
{
    PDU_E_NONE,                  /* 0, successful */
    PDU_E_RADIOID,               /* invalid radio ID */
    PDU_E_WLANID,                /* invalid WLAN ID */
    PDU_E_IFID,                  /* invalid interface ID */
    PDU_E_CMD_ID,                /* invalid command ID */
    PDU_E_ASYNC_CMD_ID,          /* invalid async command ID */
    PDU_E_EVENT_ID,              /* invalid event ID */
    PDU_E_RANGE,                 /* parameter exceed range */
    PDU_E_PARAM,                 /* other invalid parameter */
    PDU_E_RESOURCE,              /* fail to get resource*/
    PDU_E_NOT_EXIST,             /* Object don't exist */
    PDU_E_INIT,                  /* Feature not initialized. */
    PDU_E_REINIT,                  /* Feature initialized more than once. */
    PDU_E_ORDER,                 /* Operator is not in order */
    PDU_E_MEM_MALLOC,      /* memory malloc error */
    PDU_E_UPGRADE_NOSPACE, /* upgrade no space error */
    PDU_E_NULLPTR,               /*an pointer point to NULL*/
    PDU_E_INVALID_VALUE,         /*Invalid value*/
    PDU_E_INVALID_FILE_NAME,     /*Invalid file name*/
    PDU_E_IOCTL_ERR,             /* ioctl operator error */
    PDU_E_SYSTEM_FAIL,           /* system call failure */
    PDU_E_DEFAULT_FAIL,          /*default failure*/
} PDU_ERROR_CODE_E;

#ifndef LONG
#define LONG signed long
#endif

#ifndef INT32
#define INT32 signed int
#endif

#ifndef INT16
#define INT16 signed short
#endif

#ifndef INT8
#define INT8  char 
#endif

#ifndef UINT32
#define UINT32 unsigned int
#endif

#ifndef ULONG
#define ULONG unsigned long
#endif

#ifndef UINT16
#define UINT16 unsigned short
#endif

#ifndef UINT8
#define UINT8  unsigned char
#endif

#ifndef BOOL
#define BOOL UINT32
#endif

#ifndef CHAR
#define CHAR  INT8
#endif

#ifndef size_t
#define size_t UINT32
#endif

#ifndef VOID
#define VOID void
#endif

#ifndef LOCAL
#define LOCAL  static
#endif

#ifndef REMOTE
#define REMOTE extern
#endif

#define PDU_SEND_UD_LEN         140
#define PDU_LONG_SMS_NUM      8

#define SMS_MAX             50
#define FILE_NAME_MAX_LEN   64
#define BUFFER_MAX_LEN      256
#define SMS_SEND_BUFFER_MAX    512

#define BIT7_MASK           0x7F
#define BIT7_BIT            7
#define ASCII_BIT           8
#define TRANSLATE_LEN       8
#define BIT7_BUFFER_LEN     ((TRANSLATE_LEN * BIT7_BIT)/ASCII_BIT)
#define BIT7_STRING_LEN     ((BIT7_BUFFER_LEN) * 2)

#define PDU_ERROR       -1
#define PDU_OK          0
#define PDU_READMORE    1

#define PDU_STATUS_INIT     0x00
#define PDU_STATUS_DATE     0x01
#define PDU_STATUS_DONE     0x02
#define PDU_STATUS_ERROR    0xFF


#define ADDR_LENGTH_LEN      2
#define ADDR_TYPE_LEN        2
#define ADDR_MAX_LEN        32

#define ADDR_TYPE_MASK               0x70
#define ADDR_TYPE_UNKOWN             0x00
#define ADDR_TYPE_INTERNATIONAL      0x10
#define ADDR_TYPE_INTERNAL           0x00

#define ADDR_IDENTY_MASK             0x0F
#define ADDR_IDENTY_ISDN             0x01
#define ADDR_IDENTY_EX               0x0F

/*地址信息*/
#define ADDR_INFO_TYPE_SERVICE      1
#define ADDR_INFO_TYPE_USER         2

#define TIME_PDU_LEN                6
#define TIME_STRING_LEN             16
#define TIME_ZONE_LEN               2

#define UDH_LDENTIFY_LONG            0
#define UDH_LDENTIFY_NORMAL       1



/*pdu发短信*/
#define FO_LENGTH       2
#define PID_LENGTH      2
#define DSC_LENGTH      2
#define UDLEN_LENGTH    2
#define UDHL_LENGTH    2
#define UDHI_LENGTH    2
#define UDHSH_LENGTH    2
#define UDHSHID_LENGTH    2
#define UDHSHMAX_LENGTH    2
#define UDHSHNUM_LENGTH    2
#define UDH_UD_FIRST_LENGTH    2
#define UD_LENGTH_MAX   256
#define UD_SEND_MAX         140

#define FO_MTI              0x03
#define FO_MTI_SEND  0x01
#define FO_MTI_RECV  0x00
#define FO_RD               0x04
#define FO_VPF              0x18
#define FO_VPF_NONE   0x00
#define FO_VPF_8BIT     0x10
#define FO_VPF_RSV     0x08
#define FO_VPF_4BIT    0x18
#define FO_SRI              0x20
#define FO_UDHI           0x40
#define FO_RP               0x80

#define DSC_CLASS_MASK      0x03
#define DSC_CLASS_0         0x00
#define DSC_CLASS_1         0x01
#define DSC_CLASS_2         0x02
#define DSC_CLASS_3         0x03

#define DSC_TYPE_MASK       0x0C
#define DSC_TYPE_7BIT       0x00
#define DSC_TYPE_8BIT       0x04
#define DSC_TYPE_USC2       0x08
#define DSC_TYPE_RSV        0x0C

/* BEGIN: Added by wujian, 2014/11/21   PN:短信功能相关宏定义 */
#define LTE_SMS_COUNT_MAX   50
#define LTE_SMS_CONTENT_MAX   256
//#define LTE_SMS_MOBILE_NUM   16
#define LTE_SMS_MOBILE_NUM   32
#define LTE_SMS_RECV_TIME   20
#define LTE_SMS_CENT_NUM   32

/* END:   Added by wujian, 2014/11/21   PN:短信功能相关宏定义 */

#define STRING_PUSH(str, strlen, offset)    {(str)+=(offset);(strlen)-=(offset);}
#define STRING_PULL(str, strlen, offset)    {(str)-=(offset);(strlen)+=(offset);}


typedef struct _ADDR_INFO
{
    UINT8 ucLength;
    UINT8 ucType;               /*0~3bit:号码鉴别,4~6bit:数值类型,7bit:保留*/
    CHAR aucAddr[ADDR_MAX_LEN + 1];
}ADDR_INFO;

typedef struct _TIME_INFO
{
    CHAR aucDate[TIME_STRING_LEN];
    CHAR aucTime[TIME_STRING_LEN];
    UINT8 ucZone;
}TIME_INFO;


typedef struct _UDH_INFO
{
    UINT8 ucLength;
    UINT8 ucIdentify;
    UINT8 ucSubHeadLen;
    UINT8 ucId;
    UINT8 ucMax;
    UINT8 ucNum;
}UDH_INFO;

/*pdu发短信*/
typedef struct _USER_DATA
{
    UINT8 ucUDLen;
    UINT8 aucUD[PDU_SEND_UD_LEN];          /*用户数据*/
}USER_DATA;

/****move by by wangqi start 2015-6-9****/
typedef enum
{
    SMS_NOT_HEAD = 0,
    SMS_ADD_HEAD,    
}LTE_SMS_SEND_HEAD_E;

typedef struct
{
    LTE_SMS_SEND_HEAD_E eSmsHead;
    char acSmsHead[LTE_SMS_CONTENT_MAX];
}LTE_SMS_SEND_HEAD_T;

/****move by by wangqi end 2015-6-9****/


typedef struct _SMS_SEND_INFO
{
    ADDR_INFO stDA;          /*接收方号码信息*/
    UINT8 ucFO;
    UINT8 ucDSC;             /*编码标准,6~7bit:为0,5bit:0-未压缩 1-GSM压缩,4bit:0-0~1bit保留 1-0~1bit为信息类别,2~3bit:字母表*/
    USER_DATA stUD[PDU_LONG_SMS_NUM];
    UINT8 ucCount;
    UINT8 ucSmsID;
    LTE_SMS_SEND_HEAD_T *pstSmsSendHead;
}SMS_SEND_INFO;

typedef struct _SMS_INFO
{
    UINT32 ulIndex;			/* 短信序列号 */
    UINT32 ulState;
    ADDR_INFO stSCA;         /*服务中心号码信息*/
    UINT8 ucFO;              /*PDU第一字节,RD/UDHI/SRI/VPF/MMS/MTI*/
    ADDR_INFO stOA;          /*发送方号码信息*/
    UINT8 ucPID;             /*协议描述符,6~7bit:00-自定义,01-GSM03.40,10-RSV,11服务中心特殊分配,0~5bit自定义类型时使用*/
    UINT8 ucDSC;             /*编码标准,6~7bit:为0,5bit:0-未压缩 1-GSM压缩,4bit:0-0~1bit保留 1-0~1bit为信息类别,2~3bit:字母表*/
    TIME_INFO stSCTS;        /*服务中心时间戳*/
    UINT8 ucUDLen ;          /*用户数据长度*/
    UDH_INFO stUDh;         /*用户数据头，FO中UDHI置位有效*/
    UINT8 aucUD[UD_LENGTH_MAX];          /*用户数据*/
    UINT32 ulUDOffset;
    UINT32 ulLenOffset;
}SMS_INFO;

/*alter by wangqi start 2015-6-9*/
typedef struct
{
    int iSmsMode; //1表文本模式，0表示pdu模式
}LTE_SMS_Mode_T;

typedef struct
{
    char    acSmsCentCode[LTE_SMS_CENT_NUM]; //短信中心号码
}LTE_SMS_CENT_CODE_T;


typedef struct
{
    char acRecvMsgCode[LTE_SMS_MOBILE_NUM];  //接收端电话号码
    char acMsgCont[LTE_SMS_CONTENT_MAX];     //短信内容
    LTE_SMS_SEND_HEAD_T stSmsSendHead;
}LTE_SMS_SEND_T;

typedef struct
{
    SMS_INFO stRecvCont[LTE_SMS_COUNT_MAX];
    int iRecvFlag; //0，获取所有的短信;大于1的数则表示存储的短信的索引，即第几条短信
}LTE_SMS_RECV_T;
typedef struct
{
    char acSmsRecvFile[32];/* 接收短信存放的文件路径 */
}LTE_SMS_RECV_SRC_T;

typedef struct
{
    int iDelIndex;          //0，删除所有短信;大于1则为删除第几条短信
}LTE_SMS_DEL_T;
/*alter by wangqi end 2015-6-9*/




extern INT32 bit7_to_ascii(char **ppstStr, UINT32 *pulStrLen, UINT32 ulBegin, INT32 ulLength, UINT8 *pstAscii, UINT32 ulAsciiLen, UINT32 *pulAsciiOff, UINT32 *pulLenOff);

extern INT32 bit8_to_hex(char **ppstStr, UINT32 *pulStrLen, INT32 ulLength, 
                                 UINT8 *pstHex, UINT32 ulHexLen, UINT32 *pulHexOff, UINT32 *pulLenOff);
extern INT8 char_to_hex(char c);
extern int enc_unicode_to_utf8_one(UINT32 unic, UINT8 *pOutput, int outSize);

extern INT32 pdu_addr(char **ppstStr, UINT32 *pulStrLen, ADDR_INFO *pstAddr, UINT32 ulAddrInfoType);
extern INT32 pdu_analyse(char **ppstStr, UINT32 *pulStrLen, UINT32 ulEnd, SMS_INFO *pstSms);
extern INT32 pdu_dsc(char **ppstStr, UINT32 *pulStrLen, UINT8 *pucDSC);
extern INT32 pdu_fo(char **ppstStr, UINT32 *pulStrLen, UINT8 *pucFO);
extern INT32 pdu_pid(char **ppstStr, UINT32 *pulStrLen, UINT8 *pucPID);
extern INT32 pdu_scts(char **ppstStr, UINT32 *pulStrLen, TIME_INFO *pstSCTS);
extern INT32 pdu_to_telephone_number(char *pstStr, UINT32 ulStrLen, char *pstTel, UINT32 ulTelLen);
extern INT32 pdu_to_time(char *pstStr, char *pstTime, UINT32 ulTimeLen, char cSign);
extern INT32 pdu_ud_len(char **ppstStr, UINT32 *pulStrLen, UINT8 *pucUDLen);
extern INT32 sms_recv(char * pucFile, SMS_INFO *pstSms, UINT32 ulSmsMax);
extern INT32 string_to_hex(char *pstStr, UINT32 ulStrLen, UINT8 *pucHex, UINT32 ulHexLen);
extern INT32 usc2_to_utf8(char **ppstStr, UINT32 *pulStrLen, INT32 ulLength, 
                                 UINT8 *pstUtf8, UINT32 ulUtf8Len, UINT32 *pulUtf8Off, UINT32 *pulLenOff);


extern INT32 pdu_convert_tele_num(char *pstDst, UINT32 ulDstLen, char *pstSrc, UINT32 ulSrcLen);
extern INT32 pdu_encode(SMS_SEND_INFO *pstSms, UINT8 ucCount, char *pucPdu, UINT32 ulPduLen);
extern INT32 pdu_encode_da(SMS_SEND_INFO *pstSms, char **ppucPdu, UINT32 *pulPduLen);
extern INT32 pdu_encode_dcs(SMS_SEND_INFO *pstSms, char **ppucPdu, UINT32 *pulPduLen);
extern INT32 pdu_encode_fo(SMS_SEND_INFO *pstSms, char **ppucPdu, UINT32 *pulPduLen);
extern INT32 pdu_encode_mr(SMS_SEND_INFO *pstSms, char **ppucPdu, UINT32 *pulPduLen);
extern INT32 pdu_encode_pid(SMS_SEND_INFO *pstSms, char **ppucPdu, UINT32 *pulPduLen);

/*extern INT32 pdu_encode_sms(UINT8 *pstAddr, INT32 iAddrLen, UINT8 *pstData, INT32 iDataLen, 
    INT8 (*pcSmsStr)[SMS_SEND_BUFFER_MAX], INT32 iSmsStrLen, INT32 *pcSmsNum);*/
extern INT32 pdu_encode_sms(LTE_SMS_SEND_T *pstSendInfo, INT8 (*pcSmsStr)[SMS_SEND_BUFFER_MAX], INT32 iSmsStrLen, INT32 *pcSmsNum);

extern INT32 pdu_encode_ud(SMS_SEND_INFO *pstSms, UINT8 ucCount, char **ppucPdu, UINT32 *pulPduLen);
extern INT32 pdu_encode_vp(SMS_SEND_INFO *pstSms, char **ppucPdu, UINT32 *pulPduLen);
extern INT32 pdu_get_utf8_size(UINT8 utf);
extern UINT8 pdu_sms_id(void);
extern UINT8 pdu_ud_encode_type(char *pucUd, UINT32 ulUdLen);
extern INT32 pdu_utf8_to_bit7(UINT8 * pInput, INT32 ulInLen, UINT8 *pOutput, INT32 ulOutLen, UINT32 *pOutOff);
extern UINT32 pdu_utf8_to_unicode_one(UINT8 * pInput, INT32 ulInLen, UINT8 *pOutput, INT32 ulOutLen, UINT32 *pOutOff);
#endif /* __PDU_H__ */
