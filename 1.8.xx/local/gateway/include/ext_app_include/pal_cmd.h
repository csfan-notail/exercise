/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : pal_cmd.h
  版 本 号   : 初稿
  作    者   : Zhong Huaqing
  生成日期   : 2012年7月23日 星期一
  最近修改   :
  功能描述   : 
  函数列表   :
  修改历史   :
  1.日    期   : 2012年7月23日 星期一
    作    者   : Zhong Huaqing
    修改内容   : 创建文件

******************************************************************************/

#ifndef __PAL_CMD_H__
#define __PAL_CMD_H__


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
 
/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
 #define PAL_CMD_INVALID    (-1)
 #define PAL_CMD_MIN        (0)
 
/*----------------------------------------------*
 * 枚举定义                                       *
 *----------------------------------------------*/


/*For BSP*/
typedef enum pal_cmd_bsp_e
{
    PAL_CMD_BSP_MIN = PAL_CMD_MIN,

    PAL_CMD_BSP_UPDATE_VERSION,

    PAL_CMD_BSP_GET_SW_INFO,

	/*	add by pengyue 9/6/2012	*/
    PAL_CMD_BSP_CONTROL_LED,
    PAL_CMD_BSP_CONTROL_BUTTON,
    /*	end by add	*/    
    
    PAL_CMD_BSP_GET_WIRED_PORT_STATUS,
    PAL_CMD_BSP_SET_WIRED_PORT_MODE,

    PAL_CMD_BSP_NVRAM_GET,
    PAL_CMD_BSP_NVRAM_GET_ALL,
    PAL_CMD_BSP_NVRAM_SET,
    PAL_CMD_BSP_NVRAM_SET_N,
    PAL_CMD_BSP_NVRAM_DELETE,
    PAL_CMD_BSP_NVRAM_SAVE,
    
    PAL_CMD_BSP_PPPOE_SETUP,
    PAL_CMD_BSP_PPPOE_DIAL,
    PAL_CMD_BSP_PPPOE_STOP,
    PAL_CMD_BSP_PPPOE_GET_STATUS,
    PAL_CMD_BSP_PPPOE_SET_DEFAULT,
    
    PAL_CMD_BSP_BR_ADD,
    PAL_CMD_BSP_BR_DEL,
    PAL_CMD_BSP_BRIF_ADD,
    PAL_CMD_BSP_BRIF_DEL,
    PAL_CMD_BSP_BR_SET_FD,
    PAL_CMD_BSP_TRAFFIC_MONITOR_IPADDR,
    PAL_CMD_BSP_TRAFFIC_MONITOR_TIME,
    PAL_CMD_BSP_TRAFFIC_MONITOR_STATE,
    PAL_CMD_BSP_MUTEX_TEST,
    /*for lte pin apn cmd*/
    PAL_CMD_BSP_LTE_PIN_STATE,
    PAL_CMD_BSP_LTE_PIN_SWITCH,
    PAL_CMD_BSP_LTE_PIN_SAVE,
    PAL_CMD_BSP_LTE_PIN_GET,
    PAL_CMD_BSP_LTE_PIN_UNLOCK,
    PAL_CMD_BSP_LTE_PIN_ATTEMPTS,
    PAL_CMD_BSP_LTE_PIN_MODE_SET,
    PAL_CMD_BSP_LTE_PIN_MODE_GET,
    PAL_CMD_BSP_LTE_PIN_ALTER,
    PAL_CMD_BSP_LTE_APN_SET,
    PAL_CMD_BSP_LTE_APN_GET,
    PAL_CMD_BSP_LTE_IMEI_GET,
    PAL_CMD_BSP_LTE_SIGNAL_GET,
    PAL_CMD_BSP_LTE_LINKTIMER,
    PAL_CMD_BSP_LTE_DISLINKTIMER,
    PAL_CMD_BSP_LTE_LINKTIMER_GET,
    /*for lte plmn disconnect connect*/
    PAL_CMD_BSP_LTE_POWER_ON,
    PAL_CMD_BSP_LTE_POWER_OFF,
    PAL_CMD_BSP_LTE_CONNECT_MODE_SET,
    PAL_CMD_BSP_LTE_CONNECT_MODE_GET,
    PAL_CMD_BSP_LTE_NET_TYPE_SET,
    PAL_CMD_BSP_LTE_NET_TYPE_GET,
    PAL_CMD_BSP_LTE_PLMN_MODE_SET,
    PAL_CMD_BSP_LTE_PLMN_MODE_GET,
    PAL_CMD_BSP_LTE_PLMN_OPER_SET,
    PAL_CMD_BSP_LTE_PLMN_OPER_GET,
    PAL_CMD_BSP_LTE_SIM_CARRIER_GET,
    PAL_CMD_BSP_LTE_PLMN_REGISTER,
    PAL_CMD_BSP_LTE_PREFER_PLMN_SET,
    PAL_CMD_BSP_LTE_LINKSTATE_GET,
    /*altered by caibing 2013-2-27*/
    PAL_CMD_BSP_LTE_CELL_GET,
    PAL_CMD_BSP_LTE_CONNECT,
    PAL_CMD_BSP_LTE_SET_DSTM,
    PAL_CMD_BSP_LTE_GET_CELL_TYPE,
    PAL_CMD_BSP_LTE_SET_APN,
    PAL_CMD_BSP_LTE_GET_IMEI,
    PAL_CMD_BSP_LTE_GET_CELLID,
    PAL_CMD_BSP_LTE_GET_BAND,
    PAL_CMD_BSP_LTE_GET_NETTYPE,
    PAL_CMD_BSP_LTE_DISCONNECT,
    PAL_CMD_BSP_MAX
} PAL_CMD_BSP_E;


/*For APP*/
typedef enum pal_cmd_app_e
{
    PAL_CMD_APP_MIN = PAL_CMD_BSP_MAX,
        
    PAL_CMD_APP_SET_DNS,
    PAL_CMD_APP_GET_DNS,
    /* BEGIN: Added by zhanghaozhong, 2012/12/26   PN:add reset button's interface */
    PAL_CMD_APP_DEFAULT_SET,
    /* END:   Added by zhanghaozhong, 2012/12/26   PN:add reset button's interface */
    /* BEGIN: Added by zhanghaozhong, 2012/12/26   PN:add wps state get */
    PAL_CMD_APP_WPS_STA_GET,
    /* END:   Added by zhanghaozhong, 2012/12/26   PN:add wps state get */
    /* BEGIN: Added by zhanghaozhong, 2012/12/19   PN:add usr log function */
    PAL_CMD_APP_USR_LOG_WRITE,
    /* END:   Added by zhanghaozhong, 2012/12/19   PN:add usr log function */
    PAL_CMD_APP_LAN_SET,
    PAL_CMD_APP_LAN_GET,
    PAL_CMD_APP_CONFIGFILE_CREATE, 
    PAL_CMD_APP_DHCPSERVER_START,
    PAL_CMD_APP_DHCPSERVER_STOP,
    PAL_CMD_APP_DHCPSERVER_GET,
    PAL_CMD_APP_DHCPSERVER_SET,
    PAL_CMD_APP_DNSMASQ_START,
    PAL_CMD_APP_DNSMASQ_STOP,
    PAL_CMD_APP_NTPCLIENT_START,
    PAL_CMD_APP_DTPCLIENT_STOP,
    PAL_CMD_APP_FLOWCTRL_START,
    PAL_CMD_APP_FLOWCTRL_STOP,
    PAL_CMD_APP_FLOWCTRL_GET,
    PAL_CMD_APP_FLOWCTRL_SET,
    PAL_CMD_APP_ACL_START,
    PAL_CMD_APP_ACL_STOP,
    PAL_CMD_APP_ACL_GET,
    PAL_CMD_APP_ACL_SET,
    PAL_CMD_APP_WAN_GET,
    /* BEGIN: Added by lvjianlin, 2013/4/28   PN:ltefi */
    PAL_CMD_APP_ACDNS_GET,
    PAL_CMD_APP_ACDNS_SET,
    PAL_CMD_APP_ACLINKSTATE_GET,
    /* END:   Added by lvjianlin, 2013/4/28   PN:ltefi */
    /* BEGIN: Added by lvjianlin, 2013/6/26   PN:lte-fi  GPS ALARM  LOCAL PORTAL */
    PAL_CMD_APP_CONNECTINTERNET_SET,
    /* END:   Added by lvjianlin, 2013/6/26   PN:lte-fi  GPS ALARM  LOCAL PORTAL */
    PAL_CMD_APP_MAX
    
} PAL_CMD_APP_E;


/*For WIFI*/
typedef enum pal_cmd_wifi_e
{
    PAL_CMD_WIFI_MIN = PAL_CMD_APP_MAX,
    
    /*vap级基本配置*/    
    PAL_CMD_WIFI_SET_VAP_BASIC_CFG,
    PAL_CMD_WIFI_GET_VAP_BASIC_CFG,

    /*wifi级基本配置*/    
    PAL_CMD_WIFI_SET_WIFI_BASIC_CFG,
    PAL_CMD_WIFI_GET_WIFI_BASIC_CFG,

    /*安全*/
    PAL_CMD_WIFI_SET_VAP_PRIVACY_CFG,
    PAL_CMD_WIFI_GET_VAP_PRIVACY_CFG,
    
    /*速率*/
    PAL_CMD_WIFI_SET_RATE_CFG,
    PAL_CMD_WIFI_GET_RATE_CFG,

    /*802.11无线模式及信道*/
    PAL_CMD_WIFI_SET_MODE_BANDWIDTH_CFG,
    PAL_CMD_WIFI_GET_MODE_BANDWIDTH_CFG,
    

    /*WMM*/    

    
    PAL_CMD_WIFI_MAX
} PAL_CMD_WIFI_E;


 #define PAL_CMD_MAX PAL_CMD_WIFI_MAX


/*----------------------------------------------*
 * 结构定义                                       *
 *----------------------------------------------*/



#endif /* __PAL_CMD_H__ */

