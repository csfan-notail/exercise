/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : reg_ioctl.c
  版 本 号   : 初稿
  作    者   : xiao wei
  生成日期   : 2016年3月6日 星期三
  最近修改   :
  功能描述   : 
  函数列表   :
  修改历史   :
  1.日    期   : 2016年3月6日 星期三
    作    者   : xiao wei
    修改内容   : 创建文件

******************************************************************************/
#ifndef __REG_IOCTL_H__
#define __REG_IOCTL_H__

#include "ap_types.h"

#define REG_PROC_FILE            "register"

#define REG_SWITCH_PHY_PORT_1		0x200
#define REG_SWITCH_PHY_PORT_2		0x300
#define REG_SWITCH_PHY_PORT_3		0x400
#define REG_SWITCH_PHY_PORT_4		0x500

typedef enum{
    REGISTER_IOCTL_SUCCESS,
    REGISTER_IOCTL_FAILED
}REGISTER_IOCTL_RET;

typedef enum{
    REGISTER_IOCTL_SHOW,
    REGISTER_IOCTL_READ,
    REGISTER_IOCTL_WRITE
}REGISTER_IOCTL_CMD;

typedef enum{
    REGISTER_IOCTL_ADDR_GLOBAL,
    REGISTER_IOCTL_ADDR_SWITCH,
    REGISTER_IOCTL_ADDR_PHY,
}REGISTER_ADDR_TYPE;

typedef struct{  
	UINT32 uiRegPhy;      
    UINT32 uiRegAddr;       
    UINT32 uiRegValu;                             
    UINT32 uiRetValu;  
    REGISTER_ADDR_TYPE enRegType;  
}REGISTER_IOCTL_DATE_T;                                                

typedef struct{
    REGISTER_IOCTL_CMD            enCmd; 
    REGISTER_IOCTL_DATE_T         stRegData;
}REGISTER_IOCTL_T; 

extern AP_ERROR_CODE_E register_ioctl(REGISTER_IOCTL_T *pstIoctl, REGISTER_IOCTL_CMD enCmd);

#endif /* __REG_IOCTL_H__ */
