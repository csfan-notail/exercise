/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : mirror_ioctl.h
  版 本 号   : 初稿
  作    者   : xiao wei
  生成日期   : 2016年13月6日 星期三
  最近修改   :
  功能描述   : mirror_mian.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2016年3月6日 星期三
    作    者   : xiao wei
    修改内容   : 创建文件

******************************************************************************/

#ifndef __MIRROR_IOCTL_H__
#define __MIRROR_IOCTL_H__


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
 * 基础类型定义                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/


//0x18700000 CONFIG_ATHR_SWITCH_ONLY_MODE  CONFIG_ATHR_PHY_SWAP

/*#ifdef CONFIG_ATHR_SWITCH_ONLY_MODE
//port1-5
    #define CONFIG_ATHR_START_PORT              1
    #define CONFIG_ATHR_END_PORT                5
#else   
//port2-5 or port 1-4
    #ifdef CONFIG_ATHR_PHY_SWAP
        //port2-5
        #define CONFIG_ATHR_START_PORT              2
        #define CONFIG_ATHR_END_PORT                5
    #else
        //port1-4
        #define CONFIG_ATHR_START_PORT              1
        #define CONFIG_ATHR_END_PORT                4
    #endif
#endif */

//DS port1-5 port0: CPU 
#define MIRROR_PORT_MAX_NUM         5
#define MIRROR_PROC_FILE            "mirror"
 
/*----------------------------------------------*
 * 枚举定义                                       *
 *----------------------------------------------*/
typedef enum{
    MIRROR_DISABLE,                 
    MIRROR_ENABLE
}MIRROR_IOCTL_FUN;

typedef enum{
    MIRROR_SET_FAILED,
    MIRROR_SET_SUCCESS
}MIRROR_IOCTL_SET;


typedef enum{
    MIRROR_PORT_CLOSE = 6 ,          //uiMirrorPort = 0 ,close mirror port
    MIRROR_IOCTL_OBSERVE_PORT_CLOSE
}MIRROR_IOCTL_PORT_STATU;

typedef enum{
    MIRROR_PORT0,               //default don't allow set port0 
    MIRROR_PORT1,
    MIRROR_PORT2,
    MIRROR_PORT3,
    MIRROR_PORT4,
    MIRROR_PORT5
}MIRROR_IOCTL_PORT;

typedef enum{
    MIRROR_IOCTL_DISABLE,
    MIRROR_IOCTL_ENABLE,
    MIRROR_IOCTL_SHOW,
    MIRROR_IOCTL_MIRROR,
    MIRROR_IOCTL_OBSERVE
}MIRROR_IOCTL_CMD;

typedef enum{
    MIRROR_IOCTL_OBSERVE_PORT_NO,
    MIRROR_IOCTL_OBSERVE_PORT_RX,
    MIRROR_IOCTL_OBSERVE_PORT_TX,
    MIRROR_IOCTL_OBSERVE_PORT_ALL
}MIRROR_IOCTL_OBSERVE_PORT_CURR_T;

//串口打印的是phy的端口,寄存器配置的是mac的端口
//GMAC : MAC5/MAC1 MAC2 MAC3 MAC4 
//CONFIG_ATHR_SWITCH_ONLY_MODE=1 || CONFIG_ATHR_PHY_SWAP=0/1: MAC5 MAC2 MAC3 MAC4 
//CONFIG_ATHR_SWITCH_ONLY_MODE=0 || CONFIG_ATHR_PHY_SWAP=0/1: MAC2 MAC3 MAC4 (MAC5 :WAN)
typedef enum{
    MIRROR_IOCTL_GMAC_SW_NO_MODE,                   //0x18070000 bit6 and bit7 all :0  use port2-4
    MIRROR_IOCTL_GMAC_SW_PHY_SWAP,                  //0x18070000 bit7 :1  bsp config.h: CONFIG_ATHR_PHY_SWAP = 1; use port2-4
    MIRROR_IOCTL_GMAC_SW_ONLY_MODE,                 //0x18070000 bit6 :1  bsp config.h: CONFIG_ATHR_SWITCH_ONLY_MODE = 1; use port2-5
    MIRROR_IOCTL_GMAC_SW_ALL_MODE                   //0x18070000 bit6 and bit7 all :1  use port2-5
}MIRROR_GMCA_PORT_STA;

/*----------------------------------------------*
 * 结构定义                                     *
 *----------------------------------------------*/

typedef struct{
    MIRROR_GMCA_PORT_STA  enPortSta;
    UINT32 uiStartPort;
    UINT32 uiEndPort;
}MIRROR_IOCTL_GMAC_PORT_E;

typedef struct{
    MIRROR_IOCTL_OBSERVE_PORT_CURR_T enObservePortCurr;                 //开启RX/TX或全部
}MIRROR_IOCTL_OBSERVE_PORT_T;

typedef struct{
    UINT32 uiMirrorSw;                                                  //端口镜像功能开启状态
    UINT32 uiMirrorPort;                                                //开启镜像的port number
    UINT32 uiObservePort;                                               //当前用户下发配置的观察口port number
    MIRROR_IOCTL_OBSERVE_PORT_T stObservePort[MIRROR_PORT_MAX_NUM];     //观察口的设置
    UINT32 uiSetStatu;                                                  //配置状态,成功/失败
    MIRROR_IOCTL_GMAC_PORT_E    stGmacStatus;                           //switch/phy端口使用的port number
}MIRROR_IOCTL_DATE_T;

typedef struct{
    MIRROR_IOCTL_CMD            enCmd;                                          //ioctl控制层cmd  
    MIRROR_IOCTL_DATE_T         stMirrorData;                                   //配置功能状态
}MIRROR_IOCTL_T; 


/*----------------------------------------------*
 * 导出函数原型说明                             *
 *----------------------------------------------*/
 
extern AP_ERROR_CODE_E mirror_ioctl(MIRROR_IOCTL_T *pstIoctl, MIRROR_IOCTL_CMD enCmd);

#endif /* __FC_IOCTL_H__ */

