/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技股份有限公司

 ******************************************************************************
  文 件 名   : ts_ioctl.h
  版 本 号   : 初稿
  作    者   : xucongjiang
  生成日期   : 2012年10月22日
  最近修改   :
  功能描述   : ts_ioctl.c的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2012年10月22日
    作    者   : xucongjiang
    修改内容   : 创建文件

******************************************************************************/
#ifndef __TS_IOCTL_H__
#define __TS_IOCTL_H__


/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "ap_types.h"

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
 * 基础类型定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define TS_PROC_FILE        "ts"

#define TS_IOCTL_CMD_BEGIN 0


#define TS_MOD_VERSION  "v1.0.1"
#define TS_APP_VERSION  "v1.0.1"

#define TS_MOD_DEP_VERSION "v1.0.1"   /*支持内核模块的APP最小版本号*/
#define TS_APP_DEP_VERSION "v1.0.1"   /*支持APP的内核模块最小版本号*/

#ifndef ETH_ALEN
#define ETH_ALEN	6		/* Octets in one ethernet addr	 */
#endif

#define TS_CPE_USR_MAX 32


 /*----------------------------------------------*
 * 宏操作                                       *
 *----------------------------------------------*/


 
/*----------------------------------------------*
 * 枚举定义                                       *
 *----------------------------------------------*/

 
/* 用户类型 */
typedef enum
{
    PPPOE_USER = 1,      /* PPPoE用户 */
    DHCP_USER       /* DHCP获取地址的用户（非PPPoE用户） */
}TS_USER_TYPE_E;


/* 户接入类型 */
typedef enum
{
    WIRED_USER = 1,      /* 有线端口接入的用户 */
    WIRELESS_USER   /* 无线端口接入的用户 */
}LINK_TYPE_E;

typedef enum
{
    TS_IOCTL_CMD_GET_VERSION = TS_IOCTL_CMD_BEGIN,
    
    TS_IOCTL_CMD_SET_DEBUG_LEVEL,
    TS_IOCTL_CMD_GET_DEBUG_LEVEL,
    
    TS_IOCTL_CMD_SET_USR,
    TS_IOCTL_CMD_GET_USR,
    TS_IOCTL_CMD_DEL_USR,
    
    TS_IOCTL_CMD_GET_USR_LIST,
    TS_IOCTL_CMD_FLUSH_ALL_USERS,

    TS_IOCTL_CMD_SET_AGING_TIME,
    TS_IOCTL_CMD_GET_AGING_TIME,
    
}TS_IOCTL_CMD_E;

/*----------------------------------------------*
 * 扩展类型定义                                     *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 结构定义                                       *
 *----------------------------------------------*/
typedef struct
{
    #define TS_VERSION_SIZE  128
    CHAR acModVersion[TS_VERSION_SIZE];    /*内核模块版本号*/
    CHAR acModDepVersion[TS_VERSION_SIZE]; /*支持内核模块的APP模块最小版本号*/
}TS_IOCTL_VERSION_T;


typedef struct
{
    UINT32 uiDebugLevel;
}TS_IOCTL_DEBUG_T;


typedef struct 
{
    UINT8  aucUserMac[ETH_ALEN]; /*USER的MAC地址*/
    TS_USER_TYPE_E enUserType;          /*用户类型：1. PPPoE用户2. IP*/
    LINK_TYPE_E enLinkType;          /*接入类型：1.有线用户2.无线*/
    UINT16 usUserState;         /*用户状态（保留）*/
    ULONG ulUserTxBytes;       /*发包字节数*/
    ULONG ulUserRxBytes;       /*收包字节数*/
    CHAR acPPPoeUsrName[64];   /* 用户类型为PPPoE用户时有效 */
}TS_CPE_USER_INFO_T;


typedef struct
{
    INT32 iUserNum;
    TS_CPE_USER_INFO_T astUserInfo[TS_CPE_USR_MAX];

}TS_CPE_USER_ALL_T;

typedef TS_CPE_USER_INFO_T TS_IOCTL_USER_INFO_T;

typedef struct
{
    ULONG ulTime;   /* Unit: second */
}TS_CPE_AGING_TIME_T;


typedef struct
{
	TS_IOCTL_CMD_E enCmd;

	union
	{
	    TS_IOCTL_VERSION_T stVersion;
	    TS_IOCTL_DEBUG_T stDebug;
	    TS_IOCTL_USER_INFO_T stUserInfo;
	    TS_CPE_USER_ALL_T stUserList;
	    TS_CPE_AGING_TIME_T stAgingTime;
	}unData;
}TS_IOCTL_T;


/*----------------------------------------------*
 * 导出函数原型说明                             *
 *----------------------------------------------*/



#endif /* __TS_IOCTL_H__ */

