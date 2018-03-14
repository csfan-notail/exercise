/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : fc_ioctl.h
  版 本 号   : 初稿
  作    者   : Zhong Huaqing
  生成日期   : 2012年8月10日 星期五
  最近修改   :
  功能描述   : fc_ioctl.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2012年8月10日 星期五
    作    者   : Zhong Huaqing
    修改内容   : 创建文件

******************************************************************************/

#ifndef __FC_IOCTL_H__
#define __FC_IOCTL_H__


/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/

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
 #define FC_IOCTL_CMD_BEGIN 600
/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define FC_MOD_VERSION  "v1.1.1"            /* For cpe flowctrl only */
#define FC_APP_VERSION  "v1.1.1"

#define FC_MOD_DEP_VERSION "v1.1.1"   /*支持内核模块的APP最小版本号*/
#define FC_APP_DEP_VERSION "v1.1.1"   /*支持APP的内核模块最小版本号*/

 /*----------------------------------------------*
 * 宏操作                                       *
 *----------------------------------------------*/


 
/*----------------------------------------------*
 * 枚举定义                                       *
 *----------------------------------------------*/
typedef enum
{
    FC_IOCTL_CMD_GET_VERSION = FC_IOCTL_CMD_BEGIN, /*Added on 2012.10.8*/
    
    FC_IOCTL_CMD_SET_ADMIN,
    FC_IOCTL_CMD_GET_ADMIN,
    
    FC_IOCTL_CMD_SET_DEBUG_LEVEL,
    FC_IOCTL_CMD_GET_DEBUG_LEVEL,
    
    
    FC_IOCTL_CMD_SET_RULE,
    FC_IOCTL_CMD_GET_RULE,
    
    FC_IOCTL_CMD_SET_GEAR,
    FC_IOCTL_CMD_GET_GEAR,
    
    FC_IOCTL_CMD_ADD_STA,
    FC_IOCTL_CMD_DEL_STA,
    
    FC_IOCTL_CMD_ADD_VAP,
    FC_IOCTL_CMD_DEL_VAP,
    
    FC_IOCTL_CMD_ADD_SSID,
    FC_IOCTL_CMD_DEL_SSID,
    
    FC_IOCTL_CMD_ADD_AP,
    FC_IOCTL_CMD_DEL_AP,

	FC_IOCTL_CMD_ADD_VLAN,
    FC_IOCTL_CMD_DEL_VLAN,
    
    FC_IOCTL_CMD_GET_LIST,
    
    FC_IOCTL_CMD_FLUSH,
    
    #if 0
    FC_IOCTL_CMD_SET_AUTO_LEARN,
    FC_IOCTL_CMD_GET_AUTO_LEARN,
    #endif
    
    FC_IOCTL_CMD_SET_AUTO_RATE,
    FC_IOCTL_CMD_GET_AUTO_RATE,
    
    FC_IOCTL_CMD_SET_DEV_SSID,
    FC_IOCTL_CMD_GET_DEV_SSID,
    FC_IOCTL_CMD_GET_DEV_SSID_LIST,
    
}FC_IOCTL_CMD_E;

/*----------------------------------------------*
 * 扩展类型定义                                     *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 结构定义                                       *
 *----------------------------------------------*/
typedef struct
{
    #define FC_VERSION_SIZE  128
    CHAR acModVersion[FC_VERSION_SIZE];    /*内核模块版本号*/
    CHAR acModDepVersion[FC_VERSION_SIZE]; /*支持内核模块的APP模块最小版本号*/
}FC_IOCTL_VERSION_T;


typedef struct
{
    BOOL bEnable;
}FC_IOCTL_ADMIN_T;


typedef struct
{
    INT32 iDebugLevel;
}FC_IOCTL_DEBUG_T;


typedef struct
{
    FC_RULE_E enRule;
}FC_IOCTL_RULE_T;

typedef struct
{
    UINT32 uiGearByte;
}FC_IOCTL_GEAR_T;


typedef struct
{
    FC_STA_T stSta;
}FC_IOCTL_STA_T;


typedef struct
{
    FC_VAP_T stVap;
}FC_IOCTL_VAP_T;


typedef struct
{
    FC_SSID_T stSsid;
}FC_IOCTL_SSID_T;


typedef struct
{
    FC_AP_T stAp;
}FC_IOCTL_AP_T;


typedef struct
{
    FC_VLAN_T stVlan;
}FC_IOCTL_VLAN_T;


typedef struct
{
    FC_ENTITY_LIST_T stList;
    UINT32 uiStartNum;
    UINT32 uiNum;

}FC_IOCTL_ENTITY_LIST_T;

#if 0
typedef struct
{
    FC_QUEUE_AUTO_LEARN_T stAutoLearn;
}FC_IOCTL_AUTO_LEARN_T;
#endif

typedef struct
{
    BOOL bIsOn;
}FC_IOCTL_AUTO_RATE_T;

typedef struct
{
    CHAR acDevName[IFNAMSIZ];
    CHAR acSsid[SSID_NAMSIZ];
}FC_IOCTL_DEV_SSID_T;

typedef struct
{
    #define FC_MAX_VAP_NUM 32
    
    FC_IOCTL_DEV_SSID_T astDevSsidList[FC_MAX_VAP_NUM];
    UINT32 uiNum;
}FC_IOCTL_DEV_SSID_LIST_T;



typedef struct
{
	FC_IOCTL_CMD_E enCmd;

	union{
	    FC_IOCTL_VERSION_T stVersion;
	    FC_IOCTL_ADMIN_T stAdmin;
	    FC_IOCTL_DEBUG_T stDebug;
	    FC_IOCTL_RULE_T stRule;
	    
	    FC_IOCTL_GEAR_T stGear;
	    
	    FC_IOCTL_STA_T stSta;
	    FC_IOCTL_VAP_T stVap;
	    FC_IOCTL_SSID_T stSsid;
	    FC_IOCTL_AP_T stAp;
		FC_IOCTL_VLAN_T stVlan;
	    
	    FC_IOCTL_ENTITY_LIST_T stEntityList;
	    
	    //FC_IOCTL_AUTO_LEARN_T stAutoLearn;
	    FC_IOCTL_AUTO_RATE_T stAutoRate;
	    
	    FC_IOCTL_DEV_SSID_T stDevSsid;
	    FC_IOCTL_DEV_SSID_LIST_T stDevSsidList;
	    
	    
	}u;
}FC_IOCTL_T;




/*----------------------------------------------*
 * 导出函数原型说明                             *
 *----------------------------------------------*/



#endif /* __FC_IOCTL_H__ */

