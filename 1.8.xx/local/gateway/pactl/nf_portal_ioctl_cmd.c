/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : nf_portal_cmd.c
  版 本 号   : 初稿
  作    者   : Zhong Huaqing
  生成日期   : 2012年8月15日 星期三
  最近修改   :
  功能描述   : 流控命令解析与执行
  函数列表   :
  修改历史   :
  1.日    期   : 2012年8月15日 星期三
    作    者   : Zhong Huaqing
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
#include "time.h"
#include "nf_portal_ioctl.h"

#include "nf_portal_ioctl_main.h"
#include "nf_portal_ioctl_ops.h"
#include "nf_portal_ioctl_cmd.h"

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/
AP_ERROR_CODE_E nf_portal_cmd_help(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E nf_portal_cmd_version(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E nf_portal_cmd_debug_level(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E nf_portal_cmd_auth_state(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E nf_portal_cmd_sta_type(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E nf_portal_cmd_network_model(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E nf_portal_cmd_host_ip(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E nf_portal_cmd_auth_server_ip(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E nf_portal_cmd_free_auth_sw(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E nf_portal_cmd_sta_list(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E nf_portal_cmd_lan_if_name(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E nf_portal_cmd_white_list(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E nf_portal_cmd_black_list(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E nf_portal_cmd_ccs_server_ip(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E nf_portal_cmd_snmp_server_ip(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E nf_portal_cmd_free_auth_list(INT32 iArgc, CHAR *apcArgv[]);
#ifdef HAVE_MULTI_WAN
AP_ERROR_CODE_E nf_portal_cmd_wan_config(INT32 iArgc, CHAR *apcArgv[]);
#endif
AP_ERROR_CODE_E nf_portal_cmd_pass_port_num_list(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E nf_portal_cmd_must_redirect_list(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E nf_portal_cmd_free_advertising_list(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E nf_portal_cmd_wxl_serverip_list(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E nf_portal_cmd_lan_free_auth_sw(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E nf_portal_cmd_net_connect(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E nf_portal_cmd_vlan_flood(INT32 iArgc, CHAR *apcArgv[]);

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/
LOCAL NF_PORTAL_CMD_T g_astCmd[] =
{
	{"help", nf_portal_cmd_help, 
	    "[subcmd]",
	    "Show help information. {} means \"Required\"; [] means \"Optional\"; | means \"One of\""
    },
	{"version", nf_portal_cmd_version, 
	    "",
	    "Get Current/Dependent version of application and module"
    },
	{"debuglevel", nf_portal_cmd_debug_level, 
	    "[0-7]",
	    "Set or get debug level of kernel module. Set 7 to print the most information"
    },
	{"auth_state", nf_portal_cmd_auth_state, 
	    "{xx:xx:xx:xx:xx:xx} [0|1|2]",
	    "Set or get authentication state,0-not_auth,1-being_auth,2-auth"
    },
    	{"host_ip", nf_portal_cmd_host_ip, 
	    "[xxx.xxx.xxx.xxx]",
	    "Set or get host ip"
    },
    	{"server_ip", nf_portal_cmd_auth_server_ip, 
	    "[xxx.xxx.xxx.xxx]",
	    "Set or get auth server ip"
    },
    	{"ccs_ip", nf_portal_cmd_ccs_server_ip, 
	    "[xxx.xxx.xxx.xxx]",
	    "Set or get ccs server ip"
    },
    	{"snmp_ip", nf_portal_cmd_snmp_server_ip, 
	    "[xxx.xxx.xxx.xxx]",
	    "Set or get snmp server ip"
    },
    	{"free_auth", nf_portal_cmd_free_auth_sw, 
	    "[enable|disable]",
	    "Enable or disable free auth sw"
    },
    /*begin:2015-9-6 lan free auth*/
    {"lan_free_auth",nf_portal_cmd_lan_free_auth_sw,
        "{show|set} {enable |disable}",
        "show-show lan free auth status; set enable|disable-set lan free status"
    },
    /*end:2015-9-6 lan free auth*/
    	{"stalist", nf_portal_cmd_sta_list, 
	    "[0|1|2 |flush]",
	    "Get all stations,0-not_auth,1-being_auth,2-auth"
    },
    	{"lanif", nf_portal_cmd_lan_if_name, 
	    "[lan if name]",
	    "Get or set lan if name"
    }, 
    	{"white_list", nf_portal_cmd_white_list,
	    "{mac|ip|url} {add {mac|ip [port] |url |net/mask} |del {mac|ip [port]|url |net/mask} | show |flush }",
	    "Add ,Del,Show or Flush white list,mac--user's mac,ip--destination ip,destination port is optional"
    },
       {"black_list", nf_portal_cmd_black_list,
	    "{mac|ip|url} {add {mac|ip [port_num] |url |net/mask} |del {mac|ip [port_num] |url |net/mask} | show |flush }",
	    "Add ,Del,Show or Flush black list,mac--user's mac,ip--destination ip,destination port is optional"
    },
       {"free_auth_list", nf_portal_cmd_free_auth_list,
	    "{mac|ip|port|url|https_url} {add {url|ip|port_name|url|https_url} |del {mac|ip|port_name|url|https_url}|show|flush}",
	    "Add ,Del,Show or Flush free auth list,mac--user's mac,ip--user's ip"
    },
       {"let_pass", nf_portal_cmd_pass_port_num_list,
	    "{tcp|udp} {add {port_num} | del {port_num} | show |flush }",
	    "Add ,Del,Show or Flush destination port num list which should be let pass"
    },
       {"must_redirect_list", nf_portal_cmd_must_redirect_list,
	    "{mac|ip|url} {add {mac|ip [port] |url} |del {mac|ip [port]|url} | show |flush }",
	    "Add ,Del,Show or Flush white list,mac--user's mac,ip--destination ip,destination port is optional"
    },
    {"free_advertising_list", nf_portal_cmd_free_advertising_list,
	    "{mac|ip|url} {add {mac|ip [port] |url} |del {mac|ip [port]|url} | show |flush }",
	    "Add ,Del,Show or Flush white list,mac--user's mac,ip--destination ip,destination port is optional"
    },
    /*begin: add@2015.01.31 for wechat connect wifi*/
       {"wxl_serverip_list", nf_portal_cmd_wxl_serverip_list,
	    "{add {ip mask} |del {ip mask} | show |flush }",
	    "Add ,Del,Show or Flush wxl serverip list, ip--server ip,mask--serverip mask"
    },
    /*end: add@2015.01.31 for wechat connect wifi*/
#ifdef HAVE_MULTI_WAN
	   {"wan_conf", nf_portal_cmd_wan_config,
		"{show|showmap|set {wan1|wan2|wan3} {enable band [x]|disable}}",
		"config wan1 wan2 wan3, ex. pactl wan_conf wan2 enable band 50. 50=50Mbps"
	}, 
#endif
	/*......*/
    {"net_connect",nf_portal_cmd_net_connect,
        "{show|set} {up |down}",
        "show-show network connectivity status; set up|down-set network connectivity status"
    },
	{"sta_type", nf_portal_cmd_sta_type, 
	    "{xx:xx:xx:xx:xx:xx} [0|1|2|3]",
	    "Set or get sta type,0-init,1-pc,2-mobile,3-other"
    },
	{"net_model", nf_portal_cmd_network_model, 
        "{show|set} {all|pc|mobile}",
        "show or set network model status; set all|pc|mobile surf the Internet"
    },
	{"vlan_flood", nf_portal_cmd_vlan_flood,
	    "{disable|enable}}",
	    "config vlan flood"
    },
	{NULL, NULL, NULL, NULL}
};

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 函数定义                                       *
 *----------------------------------------------*/

NF_PORTAL_CMD_T *nf_portal_get_cmd(CHAR *szCmdName)
{
    NF_PORTAL_CMD_T *pstPos;
    for (pstPos = g_astCmd; pstPos->fHandler; pstPos++)
    {
        if (STR_MATCH(szCmdName, pstPos->szCmdName))
        {
            return pstPos;
        }
    }
    return NULL;
}

void nf_portal_show_help(CHAR *szCmdName)
{
    NF_PORTAL_CMD_T *pstPos;
    
    for (pstPos = g_astCmd; pstPos->fHandler; pstPos++)
    {
        if (NULL != szCmdName)
        {
            if (!STR_MATCH(szCmdName, pstPos->szCmdName))
            {
                continue;
            }
        }
        
        printf("    %-15s -- %s\n", pstPos->szCmdName, pstPos->szCmdFmt);
        printf("    %-15s    %s\n", "", pstPos->szHelpInfo);
    }
    return;
}


AP_ERROR_CODE_E nf_portal_show_version(NF_PORTAL_IOCTL_VERSION_T *pstVersion)
{
    if (NULL == pstVersion)
    {
        return AP_E_PARAM;
    }
    
    printf("%-20s: %s\n", "mod version", pstVersion->acModVersion);
    printf("%-20s: %s\n", "app version", NF_PORTAL_APP_VERSION);
    printf("%-20s: %s\n", "mod dep version", pstVersion->acModDepVersion);
    printf("%-20s: %s\n", "app dep version", NF_PORTAL_APP_DEP_VERSION);
    
    return AP_E_NONE;
}

INT32 nf_portal_parse_version_num(CHAR *szVer)
{
    INT32 iMajor; /*主号*/
    INT32 iMinor; /*次号*/
    INT32 iRev;   /*修订号revision*/
    INT32 iVerNum;
    
    INT32 iNum;
    
    if (NULL == szVer)
    {
        return -1;
    }
    
    iNum = sscanf(szVer, "v%d.%d.%d",
                    &iMajor, &iMinor, &iRev);
    if (3 != iNum)
    {
        printf("Version is invalid.\n");
        return -1;
    }
    if (iMajor > 255 
        || iMinor > 255
        || iRev > 255)  /*版本号非法。*/
    {
        printf("Version is out of range.\n");
        return -1;
    }
    
    iVerNum = (iMajor << 16) + (iMinor << 8) + iRev;
    return iVerNum;
    
}

AP_ERROR_CODE_E nf_portal_check_version(NF_PORTAL_IOCTL_VERSION_T *pstVersion)
{
    INT32 iAppVerNum;
    INT32 iModVerNum;
    
    INT32 iAppDepVerNum;
    INT32 iModDepVerNum;
    
    AP_ERROR_CODE_E enRet;
    
    if (NULL == pstVersion)
    {
        return AP_E_PARAM;
    }
    
    iAppVerNum = nf_portal_parse_version_num(NF_PORTAL_APP_VERSION);
    iModVerNum = nf_portal_parse_version_num(pstVersion->acModVersion);
    
    iAppDepVerNum = nf_portal_parse_version_num(NF_PORTAL_APP_DEP_VERSION);
    iModDepVerNum = nf_portal_parse_version_num(pstVersion->acModDepVersion);
    
    if (iAppVerNum < 0
        || iModVerNum < 0
        || iAppDepVerNum < 0
        || iModDepVerNum < 0
        )
    {
        return AP_E_DEFAULT_FAIL;
    }
    
    if (iAppVerNum >= iModDepVerNum
        && iModVerNum >= iAppDepVerNum)
    {
        return AP_E_NONE;
    }
    else
    {
        return AP_E_DEFAULT_FAIL;
    }
}

AP_ERROR_CODE_E nf_portal_cmd_version(INT32 iArgc, CHAR *apcArgv[])
{
    NF_PORTAL_IOCTL_VERSION_T stVersion;
    AP_ERROR_CODE_E enRet;
    
        
    if (1 == iArgc)
    {
        enRet = nf_portal_cmd_get_mod_version(&stVersion);
        if (AP_E_NONE == enRet)
        {
            nf_portal_show_version(&stVersion);
            enRet = nf_portal_check_version(&stVersion);
            if (AP_E_NONE != enRet)
            {
                printf("Warning: Module and app does not match in version.\n");
                return AP_E_NONE;
            }
        }
        return enRet;
    }
    else
    {
        return AP_E_PARAM;
    }
}


AP_ERROR_CODE_E nf_portal_cmd_help(INT32 iArgc, CHAR *apcArgv[])
{
    if (1 == iArgc)
    {
        nf_portal_show_help(NULL);
    }
    else if (2 == iArgc)
    {
        nf_portal_show_help(apcArgv[1]);
    }
    else
    {
        return AP_E_PARAM;
    }
    
    return AP_E_NONE;
}


/*****************************************************************************
 函 数 名  : nf_portal_cmd_debug_level
 功能描述  : 设置/获取调试输出级别
 输入参数  : INT32 iArgc      
             CHAR *apcArgv[]  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月27日 星期四
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E nf_portal_cmd_debug_level(INT32 iArgc, CHAR *apcArgv[])
{
    UINT32 uiDebugLevel;
    AP_ERROR_CODE_E enRet;
    
    
    if (1 == iArgc)
    {
        enRet = nf_portal_cmd_get_debug_level(&uiDebugLevel);
        if (AP_E_NONE == enRet)
        {
            printf("Debug level: %d\n", uiDebugLevel);
        }
        return enRet;
    }
    else if (2 == iArgc)
    {
        uiDebugLevel = atoi(apcArgv[1]);
        enRet = nf_portal_cmd_set_debug_level(uiDebugLevel);
        if (AP_E_NONE == enRet)
        {
            printf("Set OK!\n");
        }
        return enRet;
    }
    else
    {
        return AP_E_PARAM;
    }
}

AP_ERROR_CODE_E nf_portal_cmd_sta_type(INT32 iArgc, CHAR *apcArgv[])
{
    NF_PORTAL_IOCTL_STA_TYPE_T stIoStaType;
    AP_ERROR_CODE_E enRet;

    if (!((iArgc == 2) || (iArgc == 3)))
    {
        return AP_E_PARAM;
    }

    sscanf(apcArgv[1],"%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",&(stIoStaType.aucMac[0]),\
        &(stIoStaType.aucMac[1]),\
        &(stIoStaType.aucMac[2]),\
        &(stIoStaType.aucMac[3]),\
        &(stIoStaType.aucMac[4]),\
        &(stIoStaType.aucMac[5]));

    if (2 == iArgc)
    {
        enRet = nf_portal_cmd_get_sta_type(&stIoStaType);
        if (AP_E_NONE == enRet)
        {
            printf("["MAC_FMT"]:%d\n", MAC_ARG(stIoStaType.aucMac),stIoStaType.enStaType);
        }
        return enRet;
    }
    else if (3 == iArgc)
    {
        stIoStaType.enStaType = atoi(apcArgv[2]);
        enRet = nf_portal_cmd_set_sta_type(&stIoStaType);
        if (AP_E_NONE == enRet)
        {
            printf("Set OK!\n");
        }
        return enRet;
    }
    else
    {
        return AP_E_PARAM;
    }
}

AP_ERROR_CODE_E nf_portal_cmd_network_model(INT32 iArgc, CHAR *apcArgv[])
{
    NF_PORTAL_IOCTL_NET_MODEL_T stIoNetModel;
    AP_ERROR_CODE_E enRet = AP_E_PARAM;
    
    if (2 == iArgc)
    {
        if(STR_MATCH(apcArgv[1],"show"))
        {
            enRet = nf_portal_cmd_get_network_model(&stIoNetModel);
            if (AP_E_NONE == enRet)
            {
                if(0 == stIoNetModel.enNetModel)
                {
                    printf("NETWORK MODEL:all\n");
                }
                else if(1 == stIoNetModel.enNetModel)
                {
                    printf("NETWORK MODEL:pc\n");
                }
                else
                {
                    printf("NETWORK MODEL:mobile\n");
                }
            }
            return enRet;
        }
        return AP_E_PARAM;
    }
    else if (3 == iArgc)
    {
        if(STR_MATCH(apcArgv[1],"set"))
        {
            if (STR_MATCH(apcArgv[2],"all"))
            {
                stIoNetModel.enNetModel = MODEL_ALL;
            }
            else if (STR_MATCH(apcArgv[2],"pc"))
            {
                stIoNetModel.enNetModel = MODEL_PC;
            }
            else if (STR_MATCH(apcArgv[2],"mobile"))
            {
                stIoNetModel.enNetModel = MODEL_MOBILE;
            }            
            else
            {
                return AP_E_PARAM;
            }
            enRet = nf_portal_cmd_set_network_model(&stIoNetModel);
            if (AP_E_NONE == enRet)
            {
                printf("Set OK!\n");
            }
            return enRet;
        }
        else
        {
            return AP_E_PARAM;
        }
        
        
    }
    else
    {
        return AP_E_PARAM;
    }
}

AP_ERROR_CODE_E nf_portal_cmd_auth_state(INT32 iArgc, CHAR *apcArgv[])
{
    NF_PORTAL_IOCTL_AUTH_STATE_T stIoAuthState={0};
    AP_ERROR_CODE_E enRet=AP_E_NONE;
    UINT32 uiLoop;
    if (!((iArgc == 2) || (iArgc == 3)))
    {
        return AP_E_PARAM;
    }

    sscanf(apcArgv[1],"%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",&(stIoAuthState.aucMac[0]),\
        &(stIoAuthState.aucMac[1]),\
        &(stIoAuthState.aucMac[2]),\
        &(stIoAuthState.aucMac[3]),\
        &(stIoAuthState.aucMac[4]),\
        &(stIoAuthState.aucMac[5]));
    
    if (2 == iArgc)
    {
        enRet = nf_portal_cmd_get_auth_state(&stIoAuthState);
        if (AP_E_NONE == enRet)
        {
            printf("["MAC_FMT"-"IP_FMT"]:%d\n", MAC_ARG(stIoAuthState.aucMac),\
                IP_ARG(&(stIoAuthState.uiIp)),stIoAuthState.enState);
        }
        return enRet;
    }
    else if (3 == iArgc)
    {
        stIoAuthState.enState = atoi(apcArgv[2]);
        enRet = nf_portal_cmd_set_auth_state(&stIoAuthState);
        if (AP_E_NONE == enRet)
        {
            printf("Set OK!\n");
        }
        return enRet;
    }
    else
    {
        return AP_E_PARAM;
    }
}

AP_ERROR_CODE_E nf_portal_cmd_host_ip(INT32 iArgc, CHAR *apcArgv[])
{
    NF_PORTAL_IOCTL_IP_T stIoIp;
    AP_ERROR_CODE_E enRet;
    UINT32 uiTmp[4];
    UINT32 uiLoop;
    UINT32 uiIp;
    if (1 == iArgc)
    {
        enRet = nf_portal_cmd_get_host_ip(&stIoIp);
        if (AP_E_NONE == enRet)
        {
            uiIp = htonl(stIoIp.uiIp);
            printf("HOST IP:"IP_FMT"\n", IP_ARG(&(uiIp)));
        }
        return enRet;
    }
    else if (2 == iArgc)
    {
        sscanf(apcArgv[1],"%d.%d.%d.%d",&uiTmp[0],&uiTmp[1],&uiTmp[2],&uiTmp[3]);
        for(uiLoop = 0; uiLoop < 4;uiLoop++)
        {
            ((UINT8 *)&(uiIp))[uiLoop] = uiTmp[uiLoop];
        }
        stIoIp.uiIp = ntohl(uiIp);
        enRet = nf_portal_cmd_set_host_ip(&stIoIp);
        if (AP_E_NONE == enRet)
        {
            printf("Set OK!\n");
        }
        return enRet;
    }
    else
    {
        return AP_E_PARAM;
    }
}

AP_ERROR_CODE_E nf_portal_cmd_auth_server_ip(INT32 iArgc, CHAR *apcArgv[])
{
    NF_PORTAL_IOCTL_IP_T stIoIp;
    AP_ERROR_CODE_E enRet;
    UINT32 uiTmp[4];
    UINT32 uiLoop;
    UINT32 uiIp;
    
    if (1 == iArgc)
    {
        enRet = nf_portal_cmd_get_auth_server_ip(&stIoIp);
        if (AP_E_NONE == enRet)
        {
            uiIp = htonl(stIoIp.uiIp);
            printf("AUTH SERVER IP:"IP_FMT"\n", IP_ARG(&(uiIp)));
        }
        return enRet;
    }
    else if (2 == iArgc)
    {
        sscanf(apcArgv[1],"%d.%d.%d.%d",&uiTmp[0],&uiTmp[1],&uiTmp[2],&uiTmp[3]);
        for(uiLoop = 0; uiLoop < 4;uiLoop++)
        {
            ((UINT8 *)&(uiIp))[uiLoop] = uiTmp[uiLoop];
        }
        stIoIp.uiIp = ntohl(uiIp);
        enRet = nf_portal_cmd_set_auth_server_ip(&stIoIp);
        if (AP_E_NONE == enRet)
        {
            printf("Set OK!\n");
        }
        return enRet;
    }
    else
    {
        return AP_E_PARAM;
    }
}

AP_ERROR_CODE_E nf_portal_cmd_ccs_server_ip(INT32 iArgc, CHAR *apcArgv[])
{
    NF_PORTAL_IOCTL_IP_T stIoIp;
    AP_ERROR_CODE_E enRet;
    UINT32 uiTmp[4];
    UINT32 uiLoop;
    UINT32 uiIp;
    if (1 == iArgc)
    {
        enRet = nf_portal_cmd_get_ccs_server_ip(&stIoIp);
        if (AP_E_NONE == enRet)
        {
            uiIp = htonl(stIoIp.uiIp);
            printf("CCS SERVER IP:"IP_FMT"\n", IP_ARG(&(uiIp)));
        }
        return enRet;
    }
    else if (2 == iArgc)
    {
        sscanf(apcArgv[1],"%d.%d.%d.%d",&uiTmp[0],&uiTmp[1],&uiTmp[2],&uiTmp[3]);
        for(uiLoop = 0; uiLoop < 4;uiLoop++)
        {
            ((UINT8 *)&(uiIp))[uiLoop] = uiTmp[uiLoop];
        }
        
        stIoIp.uiIp = ntohl(uiIp);
        enRet = nf_portal_cmd_set_ccs_server_ip(&stIoIp);
        if (AP_E_NONE == enRet)
        {
            printf("Set OK!\n");
        }
        return enRet;
    }
    else
    {
        return AP_E_PARAM;
    }
}

AP_ERROR_CODE_E nf_portal_cmd_snmp_server_ip(INT32 iArgc, CHAR *apcArgv[])
{
    NF_PORTAL_IOCTL_IP_T stIoIp;
    AP_ERROR_CODE_E enRet;
    UINT32 uiTmp[4];
    UINT32 uiLoop;
    UINT32 uiIp;
    if (1 == iArgc)
    {
        enRet = nf_portal_cmd_get_snmp_server_ip(&stIoIp);
        if (AP_E_NONE == enRet)
        {
            uiIp = htonl(stIoIp.uiIp);
            printf("SNMP SERVER IP:"IP_FMT"\n", IP_ARG(&(uiIp)));
        }
        return enRet;
    }
    else if (2 == iArgc)
    {
        sscanf(apcArgv[1],"%d.%d.%d.%d",&uiTmp[0],&uiTmp[1],&uiTmp[2],&uiTmp[3]);
        for(uiLoop = 0; uiLoop < 4;uiLoop++)
        {
            ((UINT8 *)&(uiIp))[uiLoop] = uiTmp[uiLoop];
        }
        
        stIoIp.uiIp = ntohl(uiIp);
        enRet = nf_portal_cmd_set_snmp_server_ip(&stIoIp);
        if (AP_E_NONE == enRet)
        {
            printf("Set OK!\n");
        }
        return enRet;
    }
    else
    {
        return AP_E_PARAM;
    }
}

AP_ERROR_CODE_E nf_portal_cmd_free_auth_sw(INT32 iArgc, CHAR *apcArgv[])
{
    NF_PORTAL_IOCTL_SW_T stIoSW;
    AP_ERROR_CODE_E enRet;
    
    if (1 == iArgc)
    {
        enRet = nf_portal_cmd_get_free_auth_sw(&stIoSW);
        if (AP_E_NONE == enRet)
        {
            printf("FREE AUTH SW:%s\n",stIoSW.enSW?"enable":"disable");
        }
        return enRet;
    }
    else if (2 == iArgc)
    {
        if (STR_MATCH(apcArgv[1],"enable"))
        {
            stIoSW.enSW = SW_ON;
        }
        else if (STR_MATCH(apcArgv[1],"disable"))
        {
            stIoSW.enSW = SW_OFF;
        }
        else
        {
            return AP_E_PARAM;
        }
        enRet = nf_portal_cmd_set_free_auth_sw(&stIoSW);
        if (AP_E_NONE == enRet)
        {
            printf("Set OK!\n");
        }
        return enRet;
    }
    else
    {
        return AP_E_PARAM;
    }
}

/*begin:2015-9-6 lan free auth */
AP_ERROR_CODE_E nf_portal_cmd_lan_free_auth_sw(INT32 iArgc, CHAR *apcArgv[])
{
    NF_PORTAL_IOCTL_SW_T stIoSW;
    AP_ERROR_CODE_E enRet = AP_E_PARAM;
    
    if (2 == iArgc)
    {
        if(STR_MATCH(apcArgv[1],"show"))
        {
            enRet = nf_portal_cmd_get_lan_free_auth_sw(&stIoSW);
            if (AP_E_NONE == enRet)
            {
                printf("FREE AUTH SW:%s\n",stIoSW.enSW?"enable":"disable");
            }
            return enRet;
        }
        return AP_E_PARAM;
    }
    else if (3 == iArgc)
    {
        if(STR_MATCH(apcArgv[1],"set"))
        {
            if (STR_MATCH(apcArgv[2],"enable"))
            {
                stIoSW.enSW = SW_ON;
            }
            else if (STR_MATCH(apcArgv[2],"disable"))
            {
                stIoSW.enSW = SW_OFF;
            }
            else
            {
                return AP_E_PARAM;
            }
            enRet = nf_portal_cmd_set_lan_free_auth_sw(&stIoSW);
            if (AP_E_NONE == enRet)
            {
                printf("Set OK!\n");
            }
            return enRet;
        }
        else
        {
            return AP_E_PARAM;
        }
        
        
    }
    else
    {
        return AP_E_PARAM;
    }
}
/*end:2015-9-6 lan free auth */
/*start:2015-9-18 net connect */
AP_ERROR_CODE_E nf_portal_cmd_net_connect(INT32 iArgc, CHAR *apcArgv[])
{
    NF_PORTAL_IOCTL_SW_T stIoSW;
    AP_ERROR_CODE_E enRet = AP_E_PARAM;
    
    if (2 == iArgc)
    {
        if(STR_MATCH(apcArgv[1],"show"))
        {
            enRet = nf_portal_cmd_get_net_connect(&stIoSW);
            if (AP_E_NONE == enRet)
            {
                printf("Network Connectivity:%s\n",stIoSW.enSW?"up":"down");
            }
            return enRet;
        }
        return AP_E_PARAM;
    }
    else if (3 == iArgc)
    {
        if(STR_MATCH(apcArgv[1],"set"))
        {
            if (STR_MATCH(apcArgv[2],"up"))
            {
                stIoSW.enSW = SW_ON;
            }
            else if (STR_MATCH(apcArgv[2],"down"))
            {
                stIoSW.enSW = SW_OFF;
            }
            else
            {
                return AP_E_PARAM;
            }
            enRet = nf_portal_cmd_set_net_connect(&stIoSW);
            if (AP_E_NONE == enRet)
            {
                printf("Set OK!\n");
            }
            return enRet;
        }
        else
        {
            return AP_E_PARAM;
        }
    }
    else
    {
        return AP_E_PARAM;
    }
}
/*end:2015-9-6 net connect */
struct tm *nf_portal_trans_time(UINT32 uiSec)
{
    return localtime((const time_t *)&(uiSec));
}

#define IP_FMT_LENGTH	    "%-3u.%-3u.%-3u.%-3u"
AP_ERROR_CODE_E nf_portal_cmd_sta_list(INT32 iArgc, CHAR *apcArgv[])
{
    NF_PORTAL_IOCTL_STA_LIST_T stIoStaList;
    AP_ERROR_CODE_E enRet;
    UINT32 uiLineNum = 110;
    UINT32 uiLoop, uiLoop2;
    struct tm *pstTime = NULL;
    UINT32 uiStaNum = 0;
    UINT32 uiIp;
	INT8 strIp[16];
	
    if (!((1 == iArgc) || (2 == iArgc)))
    {
        return AP_E_PARAM;
    }
    
    if (1 == iArgc)
    {
        stIoStaList.enState = STATE_MAX;
    }
    else if (2 == iArgc)
    {
        if (STR_MATCH(apcArgv[1],"flush"))
        {
            enRet = nf_portal_cmd_sta_list_flush();
            if (AP_E_NONE == enRet)
            {
                printf("FLUSH ALL\n");
            }
            return enRet;
        }
        
        stIoStaList.enState = atoi(apcArgv[1]);
        if ((stIoStaList.enState > STATE_MAX) ||(stIoStaList.enState < STATE_NOT_AUTH))
        {
            return AP_E_PARAM;
        }
    }

    stIoStaList.uiStartNum = 0;
    stIoStaList.uiNum = 0;
    printf("%-17s %-15s %-5s %-5s %-13s %-22s %-22s\n","MAC","IP","STATE","TYPE","TIME","          TS-UP","          TS-DOWN");
    SHOW_SPLIT_LINE(uiLineNum);
    while(TRUE)
    {
        //stIoStaList.uiStartNum += stIoStaList.uiNum;
        
        enRet = nf_portal_cmd_get_sta_list(&stIoStaList);
        if (AP_E_NONE != enRet)
        {
            printf("get sta list fail ret = %d",enRet);
            return enRet;
        }
        uiStaNum +=stIoStaList.uiNum;
        for (uiLoop = 0; uiLoop < stIoStaList.uiNum;uiLoop++)
        {
            pstTime = nf_portal_trans_time(stIoStaList.astSta[uiLoop].uiSec);
            uiIp = htonl(stIoStaList.astSta[uiLoop].uiIp);
			memset(strIp, 0, sizeof(strIp));
			snprintf(strIp, 16, IP_FMT, IP_ARG(&(uiIp)));
			for(uiLoop2=0; uiLoop2<15; ++uiLoop2){
				if((strIp[uiLoop2] >= '0') && (strIp[uiLoop2] <= '9'))
					continue;
				if(strIp[uiLoop2] == '.')
					continue;
				strIp[uiLoop2] = ' ';
			}
			strIp[15] = '\0';
            printf(MAC_FMT" %s %-5d %-5d [%02d-%02d:%02d:%02d] %10u:%-10u %10u:%-10u\n",\
                MAC_ARG(stIoStaList.astSta[uiLoop].aucMac),strIp,\
                stIoStaList.astSta[uiLoop].enAuthState,\
                stIoStaList.astSta[uiLoop].enStaType,\
                pstTime->tm_mday,pstTime->tm_hour,pstTime->tm_min,pstTime->tm_sec,\
                stIoStaList.astSta[uiLoop].astTS[NF_PORTAL_DIR_UP].ui4GByte,\
                stIoStaList.astSta[uiLoop].astTS[NF_PORTAL_DIR_UP].uiByte,\
                stIoStaList.astSta[uiLoop].astTS[NF_PORTAL_DIR_DOWN].ui4GByte,\
                stIoStaList.astSta[uiLoop].astTS[NF_PORTAL_DIR_DOWN].uiByte);
        }

        if (stIoStaList.uiNum < NF_PORTAL_STA_LIST_MAX)
        {
            break;
        }
    }

    SHOW_SPLIT_LINE(uiLineNum);
    printf("Total num:%u\n",uiStaNum);
    return AP_E_NONE;
}

AP_ERROR_CODE_E nf_portal_cmd_lan_if_name(INT32 iArgc, CHAR *apcArgv[])
{
    NF_PORTAL_IOCTL_IF_NAME_T stIoLanIfName;
    AP_ERROR_CODE_E enRet;
    
    if (1 == iArgc)
    {
        enRet = nf_portal_cmd_get_lan_if_name(&stIoLanIfName);
        if (AP_E_NONE == enRet)
        {
            printf("LAN IF NAME:%s\n",stIoLanIfName.aucLanIfName);
        }
        return enRet;
    }
    else if (2 == iArgc)
    {
        memcpy(stIoLanIfName.aucLanIfName,apcArgv[1],IFNAMSIZ);
        enRet = nf_portal_cmd_set_lan_if_name(&stIoLanIfName);
        if (AP_E_NONE == enRet)
        {
            printf("Set OK!\n");
        }
        return enRet;
    }
    else
    {
        return AP_E_PARAM;
    }
}

AP_ERROR_CODE_E nf_portal_cmd_free_auth_port(INT32 iArgc, CHAR *apcArgv[])
{
    NF_PORTAL_IOCTL_IF_NAME_T stIoPortName;
    NF_PORTAL_IOCTL_PORT_LIST_T stIoPortList;
    AP_ERROR_CODE_E enRet;
    UINT32 uiLineNum = 20;
    UINT32 uiLoop;
    
    if (!((3 == iArgc) || (4 == iArgc)))
    {
        return AP_E_PARAM;
    }
    if (3 == iArgc)
    {
        /*清空表项*/
        if (STR_MATCH(apcArgv[2],"flush"))
        {
            enRet = nf_portal_cmd_free_auth_port_flush();
            if (enRet == AP_E_NONE)
            {
                printf("FLUSH ALL\n");
            }
            return enRet;
        }
        /*查看所有表项*/
        else if (STR_MATCH(apcArgv[2],"show"))
        {
            stIoPortList.uiNum = 0;
            stIoPortList.uiStartNum = 0;
            
            printf("PORT NAME IN THE LIST:\n");
            SHOW_SPLIT_LINE(uiLineNum);
            while(TRUE)
            {
                enRet = nf_portal_cmd_get_free_auth_port_list(&stIoPortList);
                if (enRet != AP_E_NONE)
                {
                    printf("Get list fail\n");
                }

                for (uiLoop = 0; uiLoop < stIoPortList.uiNum;uiLoop++)
                {
                    printf("%s\n",stIoPortList.astPort[uiLoop].aucLanIfName);
                }

                if (stIoPortList.uiNum < NF_PORTAL_PORT_LIST_MAX)
                {
                    break;
                }
            }
            
            SHOW_SPLIT_LINE(uiLineNum);
            printf("Total num:%u\n",stIoPortList.uiStartNum); 
            return AP_E_NONE;
        }
        else
        {
            return AP_E_PARAM;
        }
    }
    else if (4 == iArgc)
    {
        if (STR_MATCH(apcArgv[2],"add"))
        {
            memcpy(stIoPortName.aucLanIfName,apcArgv[3],IFNAMSIZ);
            enRet = nf_portal_cmd_add_free_auth_port(&stIoPortName);
            if (enRet == AP_E_NONE)
            {
                printf("Add Port Ok\n");
            }
            return enRet;
        }
        else if (STR_MATCH(apcArgv[2],"del"))
        {
            memcpy(stIoPortName.aucLanIfName,apcArgv[3],IFNAMSIZ);
            enRet = nf_portal_cmd_del_free_auth_port(&stIoPortName);
            if (enRet == AP_E_NONE)
            {
                printf("Del Port Ok\n");
            }
            return enRet;
        }
        else
        {
            return AP_E_PARAM;
        }
    }
    return AP_E_PARAM;
}
/*****************************************************************************
 函 数 名  : nf_portal_cmd_mac_baw_list
 功能描述  : 用户MAC黑白名单设置
 输入参数  : INT32 iArgc                  
             CHAR *apcArgv[]              
             NF_PORTAL_BAW_FLAG_E enFlag  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年12月17日
    作    者   : WeiHonggang
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E nf_portal_cmd_mac_baw_list(INT32 iArgc, CHAR *apcArgv[],NF_PORTAL_BAW_FLAG_E enFlag)
{
    NF_PORTAL_IOCTL_MAC_BAW_T stIoMacBaw;
    NF_PORTAL_IOCTL_MAC_BAW_LIST_T stIoMacBawList;
    AP_ERROR_CODE_E enRet;
    UINT32 uiListNum = 0;
    UINT32 uiLineNum = 20;
    UINT32 uiLoop;

    if (!((3 == iArgc) || (4 == iArgc)))
    {
        return AP_E_PARAM;
    }
        
    if (3 == iArgc)
    {
        /*清空表项*/
        if (STR_MATCH(apcArgv[2],"flush"))
        {
            stIoMacBaw.stMacBaw.enFlag = enFlag;
            enRet = nf_portal_cmd_flush_mac_baw(&stIoMacBaw);
            if (enRet == AP_E_NONE)
            {
                printf("FLUSH ALL\n");
            }
            return enRet;
        }
        /*查看所有条目*/
        else if (STR_MATCH(apcArgv[2],"show"))
        {
            stIoMacBawList.uiNum = 0;
            stIoMacBawList.uiStartNum = 0;
            stIoMacBawList.enFlag = enFlag;
            
            printf("USER MAC IN THE LIST:\n");
            SHOW_SPLIT_LINE(uiLineNum);
            while(TRUE)
            {
                enRet = nf_portal_cmd_get_mac_baw_list(&stIoMacBawList);
                if (enRet != AP_E_NONE)
                {
                    printf("Get list fail\n");
                }

                uiListNum += stIoMacBawList.uiNum;
                for (uiLoop = 0; uiLoop < stIoMacBawList.uiNum;uiLoop++)
                {
                    printf(MAC_FMT"\n",MAC_ARG(&stIoMacBawList.astMacBaw[uiLoop].aucMac));
                }

                if (stIoMacBawList.uiNum < NF_PORTAL_MAC_BAW_LIST_MAX)
                {
                    break;
                }
            }
            
            SHOW_SPLIT_LINE(uiLineNum);
            printf("Total num:%u\n",uiListNum); 
            return AP_E_NONE;
        }
        else
        {
            return AP_E_PARAM;
        }
    }
    else if (4 == iArgc)
    {
        /*增加表项*/
        if (STR_MATCH(apcArgv[2],"add"))
        {
            sscanf(apcArgv[3],"%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",&(stIoMacBaw.stMacBaw.aucMac[0]),\
                &(stIoMacBaw.stMacBaw.aucMac[1]),\
                &(stIoMacBaw.stMacBaw.aucMac[2]),\
                &(stIoMacBaw.stMacBaw.aucMac[3]),\
                &(stIoMacBaw.stMacBaw.aucMac[4]),\
                &(stIoMacBaw.stMacBaw.aucMac[5]));

            stIoMacBaw.stMacBaw.enFlag = enFlag;
            enRet = nf_portal_cmd_add_mac_baw(&stIoMacBaw);
            if (enRet == AP_E_NONE)
            {
                printf("Add Rule To The List OK\n");
            }
            return enRet;
        }
        /*删除条目*/
        else if (STR_MATCH(apcArgv[2],"del"))
        {
            sscanf(apcArgv[3],"%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",&(stIoMacBaw.stMacBaw.aucMac[0]),\
                &(stIoMacBaw.stMacBaw.aucMac[1]),\
                &(stIoMacBaw.stMacBaw.aucMac[2]),\
                &(stIoMacBaw.stMacBaw.aucMac[3]),\
                &(stIoMacBaw.stMacBaw.aucMac[4]),\
                &(stIoMacBaw.stMacBaw.aucMac[5]));

            stIoMacBaw.stMacBaw.enFlag = enFlag;
            enRet = nf_portal_cmd_del_mac_baw(&stIoMacBaw);
            if (enRet == AP_E_NONE)
            {
                printf("Del Rule From The List OK\n");
            }
            return enRet;
        }
        else
        {
            return AP_E_PARAM;
        }
    }
    
    return AP_E_PARAM;
}
/*****************************************************************************
 函 数 名  : nf_portal_cmd_dest_ip_baw_list
 功能描述  : 目的IP黑白名单设置
 输入参数  : INT32 iArgc                  
             CHAR *apcArgv[]              
             NF_PORTAL_BAW_FLAG_E enFlag  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年12月17日
    作    者   : WeiHonggang
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E nf_portal_cmd_dest_ip_baw_list(INT32 iArgc, CHAR *apcArgv[],NF_PORTAL_BAW_FLAG_E enFlag)
{
    NF_PORTAL_IOCTL_IP_BAW_T stIoIpBaw;
    NF_PORTAL_IOCTL_IP_BAW_LIST_T stIoIpBawList;
    AP_ERROR_CODE_E enRet;
    UINT32 uiLineNum = 20;
    UINT32 uiLoop;
    UINT32 uiTmp[4];
    UINT32 uiListNum = 0;
    UINT32 uiIp;

    if (!((3 == iArgc) || (4 == iArgc) || (5 == iArgc)))
    {
        return AP_E_PARAM;
    }
    
    if (3 == iArgc)
    {
        /*清空表项*/
        if (STR_MATCH(apcArgv[2],"flush"))
        {
            stIoIpBaw.stIpBaw.enFlag = enFlag;
            enRet = nf_portal_cmd_flush_dest_ip_baw(&stIoIpBaw);
            if (enRet == AP_E_NONE)
            {
                printf("FLUSH ALL\n");
            }
            return enRet;
        }
        /*获取所有表项*/
        else if (STR_MATCH(apcArgv[2],"show"))
        {
            stIoIpBawList.uiNum = 0;
            stIoIpBawList.uiStartNum = 0;
            stIoIpBawList.enFlag = enFlag;
            
            printf("DEST IP IN THE LIST:\n");
            SHOW_SPLIT_LINE(uiLineNum);
            while(TRUE)
            {
                enRet = nf_portal_cmd_get_dest_ip_baw_list(&stIoIpBawList);
                if (enRet != AP_E_NONE)
                {
                    printf("Get list fail\n");
                }

                uiListNum += stIoIpBawList.uiNum;
                for (uiLoop = 0; uiLoop < stIoIpBawList.uiNum;uiLoop++)
                {
                    uiIp = htonl(stIoIpBawList.astIpBaw[uiLoop].uiIp);
                    printf(IP_FMT,IP_ARG(&uiIp));
                    if (stIoIpBawList.astIpBaw[uiLoop].usPort == 0)
                    {
                        printf("\n");
                    }
                    else
                    {
                        printf(":%u\n",stIoIpBawList.astIpBaw[uiLoop].usPort);
                    }
                }

                if (stIoIpBawList.uiNum < NF_PORTAL_IP_BAW_LIST_MAX)
                {
                    break;
                }
            }
            
            SHOW_SPLIT_LINE(uiLineNum);
            printf("Total num:%u\n",uiListNum); 
            return AP_E_NONE;
        }
        else
        {
            return AP_E_PARAM;
        }
    }
    else if (4 == iArgc || 5 == iArgc)
    {
        /*增加表项条目,IP地址必选，端口号可选*/
        if (STR_MATCH(apcArgv[2],"add"))
        {
            sscanf(apcArgv[3],"%d.%d.%d.%d",&uiTmp[0],&uiTmp[1],&uiTmp[2],&uiTmp[3]);
            for(uiLoop = 0; uiLoop < 4;uiLoop++)
            {
                ((UINT8 *)&(uiIp))[uiLoop] = uiTmp[uiLoop];
            }

            stIoIpBaw.stIpBaw.uiIp = ntohl(uiIp);
            if (5 == iArgc)
            {
                stIoIpBaw.stIpBaw.usPort = atoi(apcArgv[4]);
            }
            else
            {
                stIoIpBaw.stIpBaw.usPort = 0;
            }
            stIoIpBaw.stIpBaw.enFlag = enFlag;
            enRet = nf_portal_cmd_add_dest_ip_baw(&stIoIpBaw);
            if (enRet == AP_E_NONE)
            {
                printf("Add Rule To The List Ok\n");
            }
            return enRet;
        }
        /*删除表项条目，IP地址必选，端口号可选*/
        else if (STR_MATCH(apcArgv[2],"del"))
        {
            sscanf(apcArgv[3],"%d.%d.%d.%d",&uiTmp[0],&uiTmp[1],&uiTmp[2],&uiTmp[3]);
            for(uiLoop = 0; uiLoop < 4;uiLoop++)
            {
                ((UINT8 *)&(uiIp))[uiLoop] = uiTmp[uiLoop];
            }
            stIoIpBaw.stIpBaw.uiIp = ntohl(uiIp);
            
            if (5 == iArgc)
            {
                stIoIpBaw.stIpBaw.usPort = atoi(apcArgv[4]);
            }
            else
            {
                stIoIpBaw.stIpBaw.usPort = 0;
            }
            stIoIpBaw.stIpBaw.enFlag = enFlag;
            enRet = nf_portal_cmd_del_dest_ip_baw(&stIoIpBaw);
            if (enRet == AP_E_NONE)
            {
                printf("Del Rule From The List Ok\n");
            }
            return enRet;
        }
        else
        {
            return AP_E_PARAM;
        }
    }
    return AP_E_PARAM;
}
/*****************************************************************************
 函 数 名  : nf_portal_cmd_user_ip_baw_list
 功能描述  : 用户IP黑白名单设置
 输入参数  : INT32 iArgc                  
             CHAR *apcArgv[]              
             NF_PORTAL_BAW_FLAG_E enFlag  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年12月17日
    作    者   : WeiHonggang
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E nf_portal_cmd_user_ip_baw_list(INT32 iArgc, CHAR *apcArgv[],NF_PORTAL_BAW_FLAG_E enFlag)
{
    NF_PORTAL_IOCTL_IP_BAW_T stIoIpBaw;
    NF_PORTAL_IOCTL_IP_BAW_LIST_T stIoIpBawList;

    AP_ERROR_CODE_E enRet;
    UINT32 uiLineNum = 20;
    UINT32 uiLoop;
    UINT32 uiTmp[4];
    UINT32 uiListNum = 0;
    UINT32 uiIp;

    if (!((3 == iArgc) || (4 == iArgc)))
    {
        return AP_E_PARAM;
    }
    
    if (3 == iArgc)
    {
        /*清空表项*/
        if (STR_MATCH(apcArgv[2],"flush"))
        {
            stIoIpBaw.stIpBaw.enFlag = enFlag;
            enRet = nf_portal_cmd_flush_user_ip_baw(&stIoIpBaw);
            if (enRet == AP_E_NONE)
            {
                printf("FLUSH ALL\n");
            }
            return enRet;
        }
        /*查看所有表项*/
        else if (STR_MATCH(apcArgv[2],"show"))
        {
            stIoIpBawList.uiNum = 0;
            stIoIpBawList.uiStartNum = 0;
            stIoIpBawList.enFlag = enFlag;
            
            printf("USER IP IN THE LIST:\n");
            SHOW_SPLIT_LINE(uiLineNum);
            while(TRUE)
            {
                enRet = nf_portal_cmd_get_user_ip_baw_list(&stIoIpBawList);
                if (enRet != AP_E_NONE)
                {
                    printf("Get list fail\n");
                }

                uiListNum += stIoIpBawList.uiNum;
                for (uiLoop = 0; uiLoop < stIoIpBawList.uiNum;uiLoop++)
                {
                    uiIp = htonl(stIoIpBawList.astIpBaw[uiLoop].uiIp);
                    printf(IP_FMT"\n",IP_ARG(&uiIp));
                }

                if (stIoIpBawList.uiNum < NF_PORTAL_IP_BAW_LIST_MAX)
                {
                    break;
                }
            }
            
            SHOW_SPLIT_LINE(uiLineNum);
            printf("Total num:%u\n",uiListNum); 
            return AP_E_NONE;
        }
        else
        {
            return AP_E_PARAM;
        }
    }
    else if (4 == iArgc)
    {
        /*增加表项*/
        if (STR_MATCH(apcArgv[2],"add"))
        {
            sscanf(apcArgv[3],"%d.%d.%d.%d",&uiTmp[0],&uiTmp[1],&uiTmp[2],&uiTmp[3]);
            for(uiLoop = 0; uiLoop < 4;uiLoop++)
            {
                ((UINT8 *)&(uiIp))[uiLoop] = uiTmp[uiLoop];
            }
            stIoIpBaw.stIpBaw.uiIp = ntohl(uiIp);
            stIoIpBaw.stIpBaw.enFlag = enFlag;
            enRet = nf_portal_cmd_add_user_ip_baw(&stIoIpBaw);
            if (enRet == AP_E_NONE)
            {
                printf("Add Rule To The List Ok\n");
            }
            return enRet;
        }
        /*删除表项*/
        else if (STR_MATCH(apcArgv[2],"del"))
        {
            sscanf(apcArgv[3],"%d.%d.%d.%d",&uiTmp[0],&uiTmp[1],&uiTmp[2],&uiTmp[3]);
            for(uiLoop = 0; uiLoop < 4;uiLoop++)
            {
                ((UINT8 *)&(uiIp))[uiLoop] = uiTmp[uiLoop];
            }

            stIoIpBaw.stIpBaw.uiIp = ntohl(uiIp);
            stIoIpBaw.stIpBaw.enFlag = enFlag;
            enRet = nf_portal_cmd_del_user_ip_baw(&stIoIpBaw);
            if (enRet == AP_E_NONE)
            {
                printf("Del Rule From The List Ok\n");
            }
            return enRet;
        }
        else
        {
            return AP_E_PARAM;
        }
    }
    return AP_E_PARAM;
}
/*****************************************************************************
 函 数 名  : nf_portal_cmd_url_baw_list
 功能描述  : URL黑白名单设置
 输入参数  : INT32 iArgc                  
             CHAR *apcArgv[]              
             NF_PORTAL_BAW_FLAG_E enFlag  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年12月17日
    作    者   : WeiHonggang
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E nf_portal_cmd_url_baw_list(INT32 iArgc, CHAR *apcArgv[],NF_PORTAL_BAW_FLAG_E enFlag)
{
    NF_PORTAL_IOCTL_URL_BAW_T stIoUrlBaw;
    NF_PORTAL_IOCTL_URL_BAW_LIST_T stIoUrlBawList;
    NF_PORTAL_IOCTL_URL_IP_BAW_LIST_T stIoUrlIpBawList;
    AP_ERROR_CODE_E enRet;
    UINT32 uiListNum = 0;
    UINT32 uiLineNum = 20;
    UINT32 uiLoop;
    UINT32 uiIp;

    if (!((3 == iArgc) || (4 == iArgc)))
    {
        return AP_E_PARAM;
    }
        
    if (3 == iArgc)
    {
        /*清空表项*/
        if (STR_MATCH(apcArgv[2],"flush"))
        {
            stIoUrlBaw.stUrlBaw.enFlag = enFlag;
            enRet = nf_portal_cmd_flush_url_baw(&stIoUrlBaw);
            if (enRet == AP_E_NONE)
            {
                printf("FLUSH ALL\n");
            }
            return enRet;
        }
        /*查看表项*/
        else if (STR_MATCH(apcArgv[2],"show"))
        {
            stIoUrlBawList.uiNum = 0;
            stIoUrlBawList.uiStartNum = 0;
            stIoUrlBawList.enFlag = enFlag;
            
            printf("URL IN THE LIST:\n");
            SHOW_SPLIT_LINE(uiLineNum);
            while(TRUE)
            {
                enRet = nf_portal_cmd_get_url_baw_list(&stIoUrlBawList);
                if (enRet != AP_E_NONE)
                {
                    printf("Get list fail\n");
                }

                uiListNum += stIoUrlBawList.uiNum;
                for (uiLoop = 0; uiLoop < stIoUrlBawList.uiNum;uiLoop++)
                {
                    printf("%s\n",stIoUrlBawList.astUrlBaw[uiLoop].aucUrl);
                }

                if (stIoUrlBawList.uiNum < NF_PORTAL_URL_BAW_LIST_MAX)
                {
                    break;
                }
            }
            
            SHOW_SPLIT_LINE(uiLineNum);
            printf("Total num:%u\n",uiListNum); 

            stIoUrlIpBawList.uiNum = 0;
            stIoUrlIpBawList.uiStartNum = 0;
            stIoUrlIpBawList.enFlag = enFlag;
            uiListNum = 0;
            
            printf("URL related IPs:\n");
            SHOW_SPLIT_LINE(uiLineNum);
            
            while(TRUE)
            {
                enRet = nf_portal_cmd_get_url_ip_baw_list(&stIoUrlIpBawList);
                if (enRet != AP_E_NONE)
                {
                    printf("Get list fail\n");
                }
                
                uiListNum += stIoUrlIpBawList.uiNum;
                for (uiLoop = 0; uiLoop < stIoUrlIpBawList.uiNum;uiLoop++)
                {
                    uiIp = htonl(stIoUrlIpBawList.astUrlIpBaw[uiLoop].uiIp);
                    printf(IP_FMT"  %s\n",IP_ARG(&uiIp),stIoUrlIpBawList.astUrlIpBaw[uiLoop].aucUrl);
                }

                if (stIoUrlIpBawList.uiNum < NF_PORTAL_URL_IP_BAW_LIST_MAX)
                {
                    break;
                }
            }
            SHOW_SPLIT_LINE(uiLineNum);
            printf("Total num:%u\n",uiListNum); 
            return AP_E_NONE;
        }
        else
        {
            return AP_E_PARAM;
        }
    }
    else if (4 == iArgc)
    {
        /*增加表项*/
        if (STR_MATCH(apcArgv[2],"add"))
        {
            memcpy(stIoUrlBaw.stUrlBaw.aucUrl,apcArgv[3],URL_MAX_SIZE);
            stIoUrlBaw.stUrlBaw.enFlag = enFlag;
            enRet = nf_portal_cmd_add_url_baw(&stIoUrlBaw);
            if (enRet == AP_E_NONE)
            {
                printf("Add Rule To The List OK\n");
            }
            return enRet;
        }
        /*删除表项*/
        else if (STR_MATCH(apcArgv[2],"del"))
        {
            memcpy(stIoUrlBaw.stUrlBaw.aucUrl,apcArgv[3],URL_MAX_SIZE);
            stIoUrlBaw.stUrlBaw.enFlag = enFlag;
            enRet = nf_portal_cmd_del_url_baw(&stIoUrlBaw);
            if (enRet == AP_E_NONE)
            {
                printf("Del Rule From The List OK\n");
            }
            return enRet;
        }
        else
        {
            return AP_E_PARAM;
        }
    }
    
    return AP_E_PARAM;
}
/*****************************************************************************
 函 数 名  : nf_portal_cmd_dest_net_baw_list
 功能描述  : 目的IP黑白名单设置
 输入参数  : INT32 iArgc                  
             CHAR *apcArgv[]              
             NF_PORTAL_BAW_FLAG_E enFlag  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年12月17日
    作    者   : Liwen
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E nf_portal_cmd_dest_net_baw_list(INT32 iArgc, CHAR *apcArgv[],NF_PORTAL_BAW_FLAG_E enFlag)
{
    NF_PORTAL_IOCTL_NET_BAW_T stIoNetBaw;
    NF_PORTAL_IOCTL_NET_BAW_LIST_T stIoNetBawList;
    AP_ERROR_CODE_E enRet;
    UINT32 uiLineNum = 20;
    UINT32 uiLoop;
    UINT32 uiTmp[4];
    UINT32 uiMaskBitNum;
    UINT32 uiListNum = 0;
    UINT32 uiNet;
    UINT32 uiMaskNum;

    if (!((3 == iArgc) || (4 == iArgc) || (5 == iArgc)))
    {
        return AP_E_PARAM;
    }
    
    if (3 == iArgc)
    {
        /*清空表项*/
        if (STR_MATCH(apcArgv[2],"flush"))
        {
            stIoNetBaw.stNetBaw.enFlag = enFlag;
            enRet = nf_portal_cmd_flush_dest_net_baw(&stIoNetBaw);
            if (enRet == AP_E_NONE)
            {
                printf("FLUSH ALL\n");
            }
            return enRet;
        }
        /*获取所有表项*/
        else if (STR_MATCH(apcArgv[2],"show"))
        {
            stIoNetBawList.uiNum = 0;
            stIoNetBawList.uiStartNum = 0;
            stIoNetBawList.enFlag = enFlag;
            
            printf("DEST IP IN THE LIST:\n");
            SHOW_SPLIT_LINE(uiLineNum);
            while(TRUE)
            {
                enRet = nf_portal_cmd_get_dest_net_baw_list(&stIoNetBawList);
                if (enRet != AP_E_NONE)
                {
                    printf("Get list fail\n");
                }

                uiListNum += stIoNetBawList.uiNum;
                for (uiLoop = 0; uiLoop < stIoNetBawList.uiNum;uiLoop++)
                {
                    uiNet = htonl(stIoNetBawList.astNetBaw[uiLoop].uiNet);
                    uiMaskNum = stIoNetBawList.astNetBaw[uiLoop].uiMaskNum;
                    printf(IP_FMT"/%u\n",IP_ARG(&uiNet),uiMaskNum);
                }

                if (stIoNetBawList.uiNum < NF_PORTAL_NET_BAW_LIST_MAX)
                {
                    break;
                }
            }
            
            SHOW_SPLIT_LINE(uiLineNum);
            printf("Total num:%u\n",uiListNum); 
            return AP_E_NONE;
        }
        else
        {
            return AP_E_PARAM;
        }
    }
    else if (4 == iArgc)
    {
        /*增加表项条目,IP地址必选，端口号可选*/
        if (STR_MATCH(apcArgv[2],"add"))
        {
            sscanf(apcArgv[3],"%d.%d.%d.%d/%d",&uiTmp[0],&uiTmp[1],&uiTmp[2],&uiTmp[3],&uiMaskBitNum);
            for(uiLoop = 0; uiLoop < 4;uiLoop++)
            {
                ((UINT8 *)&(uiNet))[uiLoop] = uiTmp[uiLoop];
            }

            stIoNetBaw.stNetBaw.uiNet = ntohl(uiNet);
            if(0 == (stIoNetBaw.stNetBaw.uiMaskNum = uiMaskBitNum))
            {
                printf("Netmask is Error\n");
                return AP_E_INVALID_VALUE;
            }
            stIoNetBaw.stNetBaw.enFlag = enFlag;
            enRet = nf_portal_cmd_add_dest_net_baw(&stIoNetBaw);
            if (enRet == AP_E_NONE)
            {
                printf("Add Rule To The List Ok\n");
            }
            return enRet;
        }
        /*删除表项条目，IP地址必选，端口号可选*/
        else if (STR_MATCH(apcArgv[2],"del"))
        {
            sscanf(apcArgv[3],"%d.%d.%d.%d",&uiTmp[0],&uiTmp[1],&uiTmp[2],&uiTmp[3]);
            for(uiLoop = 0; uiLoop < 4;uiLoop++)
            {
                ((UINT8 *)&(uiNet))[uiLoop] = uiTmp[uiLoop];
            }
            stIoNetBaw.stNetBaw.uiNet = ntohl(uiNet);
            
            stIoNetBaw.stNetBaw.enFlag = enFlag;
            enRet = nf_portal_cmd_del_dest_net_baw(&stIoNetBaw);
            if (enRet == AP_E_NONE)
            {
                printf("Del Rule From The List Ok\n");
            }
            return enRet;
        }
        else
        {
            return AP_E_PARAM;
        }
    }
    return AP_E_PARAM;
}

/*****************************************************************************
 函 数 名  : nf_portal_cmd_white_list
 功能描述  : 白名单设置，包括目的IP，源MAC，URL白名单
 输入参数  : INT32 iArgc      
             CHAR *apcArgv[]  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年12月17日
    作    者   : WeiHonggang
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E nf_portal_cmd_white_list(INT32 iArgc, CHAR *apcArgv[])
{
    AP_ERROR_CODE_E enRet;
    
    if (iArgc < 3)
    {
        return AP_E_PARAM;
    }

    if (STR_MATCH(apcArgv[1],"ip"))
    {
        enRet = nf_portal_cmd_dest_ip_baw_list(iArgc,apcArgv,WHITE);
        return enRet;
    }
    else if (STR_MATCH(apcArgv[1],"mac"))
    {
        enRet = nf_portal_cmd_mac_baw_list(iArgc,apcArgv,WHITE);
        return enRet;
    }
    else if (STR_MATCH(apcArgv[1],"url"))
    {
        enRet = nf_portal_cmd_url_baw_list(iArgc,apcArgv,WHITE);
        return enRet;
    }
    else if (STR_MATCH(apcArgv[1],"net"))
    {
        enRet = nf_portal_cmd_dest_net_baw_list(iArgc,apcArgv,WHITE);
        return enRet;
    }
    return AP_E_PARAM;
}
/*****************************************************************************
 函 数 名  : nf_portal_cmd_black_list
 功能描述  : 黑名单配置，包括目的IP，用户MAC，URL黑名单
 输入参数  : INT32 iArgc      
             CHAR *apcArgv[]  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年12月17日
    作    者   : WeiHonggang
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E nf_portal_cmd_black_list(INT32 iArgc, CHAR *apcArgv[])
{
    AP_ERROR_CODE_E enRet;
    
    if (iArgc < 3)
    {
        return AP_E_PARAM;
    }

    if (STR_MATCH(apcArgv[1],"ip"))
    {
        enRet = nf_portal_cmd_dest_ip_baw_list(iArgc,apcArgv,BLACK);
        return enRet;
    }
    else if (STR_MATCH(apcArgv[1],"mac"))
    {
        enRet = nf_portal_cmd_mac_baw_list(iArgc,apcArgv,BLACK);
        return enRet;
    }
    else if (STR_MATCH(apcArgv[1],"url"))
    {
        enRet = nf_portal_cmd_url_baw_list(iArgc,apcArgv,BLACK);
        return enRet;
    }
    else if (STR_MATCH(apcArgv[1],"net"))
    {
        enRet = nf_portal_cmd_dest_net_baw_list(iArgc,apcArgv,BLACK);
        return enRet;
    }
    return AP_E_PARAM;
}
/*BEGIN: add@2014.11.03 for 强制重定向url cmd list*/
AP_ERROR_CODE_E nf_portal_cmd_must_redirect_list(INT32 iArgc, CHAR *apcArgv[])
{
    AP_ERROR_CODE_E enRet;
    
    if (iArgc < 3)
    {
        return AP_E_PARAM;
    }

    if (STR_MATCH(apcArgv[1],"url"))
    {
        enRet = nf_portal_cmd_url_baw_list(iArgc,apcArgv,MUST_REDIRECT);
        return enRet;
    }
    else if (STR_MATCH(apcArgv[1],"ip"))
    {
        enRet = nf_portal_cmd_dest_ip_baw_list(iArgc,apcArgv,MUST_REDIRECT);
        return enRet;
    }
    else if (STR_MATCH(apcArgv[1],"mac"))
    {
        enRet = nf_portal_cmd_mac_baw_list(iArgc,apcArgv,MUST_REDIRECT);
        return enRet;
    }
    return AP_E_PARAM;
}
/*END: add@2014.11.03 for 强制重定向url cmd list*/


AP_ERROR_CODE_E nf_portal_cmd_free_advertising_list(INT32 iArgc, CHAR *apcArgv[])
{
    AP_ERROR_CODE_E enRet;
    
    if (iArgc < 3)
    {
        return AP_E_PARAM;
    }

    if (STR_MATCH(apcArgv[1],"url"))
    {
        enRet = nf_portal_cmd_url_baw_list(iArgc,apcArgv,FREE_ADVERTISING);
        return enRet;
    }
    else if (STR_MATCH(apcArgv[1],"ip"))
    {
        enRet = nf_portal_cmd_dest_ip_baw_list(iArgc,apcArgv,FREE_ADVERTISING);
        return enRet;
    }
    else if (STR_MATCH(apcArgv[1],"mac"))
    {
        enRet = nf_portal_cmd_mac_baw_list(iArgc,apcArgv,FREE_ADVERTISING);
        return enRet;
    }
    return AP_E_PARAM;
}


#ifdef HAVE_MULTI_WAN
AP_ERROR_CODE_E nf_portal_cmd_wan_config(INT32 iArgc, CHAR *apcArgv[])
{
    AP_ERROR_CODE_E enRet;
	NF_PORTAL_IOCTL_WAN_CONF_T stWanConf;
	NF_PORTAL_IOCTL_WAN_MAP_T stWanMap;
	NF_PORTAL_IOCTL_WAN_LIST_T stWanList;
	UINT32 uiLineNum = 20;
	UINT32 uiLoop = 0;
	UINT32 uiListNum = 0;
    if (iArgc < 2)
    {
		printf("iArgc(%d) < 2\n", iArgc);
		return AP_E_PARAM;
    }

	memset(&stWanConf, 0, sizeof(stWanConf));
    if (STR_MATCH(apcArgv[1], "show"))
    {
		if (iArgc != 2)
		{
			printf("iArgc(%d) != 2\n", iArgc);
			return AP_E_PARAM;
		}
        
        printf("ALL WAN CONFIG:\n");
        SHOW_SPLIT_LINE(uiLineNum);
		enRet = nf_portal_cmd_wan_conf_show(&stWanList);
		if (enRet != AP_E_NONE)
        {
            printf("Get list fail\n");
			return AP_E_PARAM;
        }
		for (uiLoop = 0; uiLoop < stWanList.uiNum; uiLoop++)
        {
            printf("index: %u, enable: %u, band: %u, flagnum: %u\n",
				   stWanList.astWanConf[uiLoop].uiWanIndex,
				   stWanList.astWanConf[uiLoop].uiEnable,
				   stWanList.astWanConf[uiLoop].uiBand,
				   stWanList.astWanConf[uiLoop].uiFlagNum);
        }
		SHOW_SPLIT_LINE(uiLineNum);
		return AP_E_NONE;
    }
    else if (STR_MATCH(apcArgv[1], "showmap"))
    {
		if (iArgc != 2)
		{
			printf("iArgc(%d) != 2\n", iArgc);
			return AP_E_PARAM;
		}
		stWanMap.uiNum = 0;
        stWanMap.uiStartNum = 0;
		printf("WAN MAP LIST:\n");
        SHOW_SPLIT_LINE(uiLineNum);
		while(TRUE)
        {
            enRet = nf_portal_cmd_wan_map_show(&stWanMap);
            if (enRet != AP_E_NONE)
            {
                printf("Get list fail\n");
				return AP_E_PARAM;
            }

            uiListNum += stWanMap.uiNum;
            for (uiLoop = 0; uiLoop < stWanMap.uiNum;uiLoop++)
            {
                printf("%u ", stWanMap.auiWanMap[uiLoop]);
				if (0 == (uiLoop + 1) % 10)
				{
					printf("\n");
				}
            }

            if (stWanMap.uiNum < NF_PORTAL_WAN_MAP_LIST_MAX)
            {
                break;
            }
        }
        
        SHOW_SPLIT_LINE(uiLineNum);
        printf("Total num:%u\n",uiListNum); 
        return enRet;
    }
    else if (STR_MATCH(apcArgv[1], "set"))
    {
		if (STR_MATCH(apcArgv[2], "wan1"))
		{
			stWanConf.uiWanIndex = 1;
		}
		else if (STR_MATCH(apcArgv[2], "wan2"))
		{
			stWanConf.uiWanIndex = 2;
		}
		else if (STR_MATCH(apcArgv[2], "wan3"))
		{
			stWanConf.uiWanIndex = 3;
		}
		else
		{
			printf("not wan1 wan2 or wan3\n");
			return AP_E_PARAM;
		}

		if ((STR_MATCH(apcArgv[3], "disable")))
		{
			if (iArgc != 4)
			{
				printf("iArgc(%d) != 5\n", iArgc);
				return AP_E_PARAM;
			}
			stWanConf.uiEnable = 0;
		}
		else if ((STR_MATCH(apcArgv[3], "enable")))
		{
			if (iArgc != 6)
			{
				printf("iArgc(%d) != 6\n", iArgc);
				return AP_E_PARAM;
			}
			if (!(STR_MATCH(apcArgv[4], "band")))
			{
				printf("not band\n");
				return AP_E_PARAM;
			}
			stWanConf.uiBand = atoi(apcArgv[5]);
			stWanConf.uiEnable = 1;
		}
		else
		{
			printf("not enable or disable\n");
			return AP_E_PARAM;
		}
		enRet = nf_portal_cmd_wan_conf_set(&stWanConf);
        return enRet;
    }
	printf("not show showmap or set\n");
    return AP_E_PARAM;
}
#endif

/*****************************************************************************
 函 数 名  : nf_portal_cmd_free_auth_list
 功能描述  : 免认证设置，包括用户IP，用户MAC，源端口设备
 输入参数  : INT32 iArgc      
             CHAR *apcArgv[]  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年12月17日
    作    者   : WeiHonggang
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E nf_portal_cmd_free_auth_list(INT32 iArgc, CHAR *apcArgv[])
{
    AP_ERROR_CODE_E enRet;
    
    if (iArgc < 3)
    {
        return AP_E_PARAM;
    }

    if (STR_MATCH(apcArgv[1],"ip"))
    {
        enRet = nf_portal_cmd_user_ip_baw_list(iArgc,apcArgv,FREE_AUTH);
        return enRet;
    }
    else if (STR_MATCH(apcArgv[1],"mac"))
    {
        enRet = nf_portal_cmd_mac_baw_list(iArgc,apcArgv,FREE_AUTH);
        return enRet;
    }
    else if (STR_MATCH(apcArgv[1],"port"))
    {
        enRet = nf_portal_cmd_free_auth_port(iArgc,apcArgv);
        return enRet;
    }
    else if (STR_MATCH(apcArgv[1],"url"))
    {
        enRet = nf_portal_cmd_url_baw_list(iArgc,apcArgv,FREE_AUTH);
        return enRet;
    }
	else if (STR_MATCH(apcArgv[1],"https_url"))
    {
        enRet = nf_portal_cmd_url_baw_list(iArgc,apcArgv,HTTPS_FREE_AUTH);
        return enRet;
    }
    return AP_E_PARAM;
}

AP_ERROR_CODE_E nf_portal_cmd_pass_port_num_list_proto(INT32 iArgc, CHAR *apcArgv[],UINT8 ucIpProto)
{
    NF_PORTAL_IOCTL_PORT_NUM_T stIoPortNum;
    NF_PORTAL_IOCTL_PORT_NUM_LIST_T stIoPortNumList;
    AP_ERROR_CODE_E enRet;
    UINT32 uiListNum = 0;
    UINT32 uiLineNum = 20;
    UINT32 uiLoop;

    if (!((3 == iArgc) || (4 == iArgc)))
    {
        return AP_E_PARAM;
    }
        
    if (3 == iArgc)
    {
        if (STR_MATCH(apcArgv[2],"flush"))
        {
            stIoPortNum.stPortNum.ucIpProto = ucIpProto;
            enRet = nf_portal_cmd_flush_pass_port_num(&stIoPortNum);
            if (enRet == AP_E_NONE)
            {
                printf("FLUSH ALL\n");
            }
            return enRet;
        }
        else if (STR_MATCH(apcArgv[2],"show"))
        {
            stIoPortNumList.uiNum = 0;
            stIoPortNumList.uiStartNum = 0;
            stIoPortNumList.ucIpProto = ucIpProto;
            
            printf("PORT NUM IN THE LIST\n");
            SHOW_SPLIT_LINE(uiLineNum);
            while(TRUE)
            {
                enRet = nf_portal_cmd_get_pass_port_num_list(&stIoPortNumList);
                if (enRet != AP_E_NONE)
                {
                    printf("Get list fail\n");
                }

                uiListNum += stIoPortNumList.uiNum;
                for (uiLoop = 0; uiLoop < stIoPortNumList.uiNum;uiLoop++)
                {
                    printf("%u\n",stIoPortNumList.astPortNum[uiLoop].stPortNum.usPortNum);
                }

                if (stIoPortNumList.uiNum < NF_PORTAL_PORT_NUM_LIST_MAX)
                {
                    break;
                }
            }
            
            SHOW_SPLIT_LINE(uiLineNum);
            printf("Total num:%u\n",uiListNum); 
            return AP_E_NONE;
        }
        else
        {
            return AP_E_PARAM;
        }
    }
    else if (4 == iArgc)
    {
        if (STR_MATCH(apcArgv[2],"add"))
        {
            stIoPortNum.stPortNum.usPortNum = atoi(apcArgv[3]);
            stIoPortNum.stPortNum.ucIpProto = ucIpProto;
            enRet = nf_portal_cmd_add_pass_port_num(&stIoPortNum);
            if (enRet == AP_E_NONE)
            {
                printf("Add Rule To The List OK\n");
            }
            return enRet;
        }
        else if (STR_MATCH(apcArgv[2],"del"))
        {
            stIoPortNum.stPortNum.usPortNum = atoi(apcArgv[3]);
            stIoPortNum.stPortNum.ucIpProto = ucIpProto;
            enRet = nf_portal_cmd_del_pass_port_num(&stIoPortNum);
            if (enRet == AP_E_NONE)
            {
                printf("Del Rule From The List OK\n");
            }
            return enRet;
        }
        else
        {
            return AP_E_PARAM;
        }
    }
    
    return AP_E_PARAM;

}

AP_ERROR_CODE_E nf_portal_cmd_pass_port_num_list(INT32 iArgc, CHAR *apcArgv[])
{
    AP_ERROR_CODE_E enRet;
    
    if (iArgc < 3)
    {
        return AP_E_PARAM;
    }

    if (STR_MATCH(apcArgv[1],"tcp"))
    {
        enRet = nf_portal_cmd_pass_port_num_list_proto(iArgc,apcArgv,IPPROTO_TCP);
        return enRet;
    }
    else if (STR_MATCH(apcArgv[1],"udp"))
    {
        enRet = nf_portal_cmd_pass_port_num_list_proto(iArgc,apcArgv,IPPROTO_UDP);
        return enRet;
    }
    
    return AP_E_PARAM;
}
/*begin: add@2015.01.31 for wechat connect wifi*/
AP_ERROR_CODE_E nf_portal_cmd_wxl_serverip_list(INT32 iArgc, CHAR *apcArgv[])
{
	AP_ERROR_CODE_E enRet;
	NF_PORTAL_IOCTL_WXL_SERVERIP_T stWxlServerIp;
	NF_PORTAL_IOCTL_WXL_SERVERIP_LIST_T stWxlServerIpList;
	UINT32 uiTmp[4] = {0};
	UINT32 uiIp = 0;
	UINT32 uiMask = 0;
	UINT32 uiLoop = 0;
	UINT32 uiListNum = 0;
    UINT32 uiLineNum = 20;
	
	if (iArgc < 2)
	{
		printf("iArgc < 2\n");
		return AP_E_PARAM;
	}

	memset(&stWxlServerIp, 0, sizeof(stWxlServerIp));
	memset(&stWxlServerIpList, 0, sizeof(stWxlServerIpList));

	if (STR_MATCH(apcArgv[1],"add"))
	{
		if (iArgc != 4)
		{
			printf("add, iArgc(%d) != 4 \n", iArgc);
			return AP_E_PARAM;
		}
		sscanf(apcArgv[2],"%d.%d.%d.%d",&uiTmp[0],&uiTmp[1],&uiTmp[2],&uiTmp[3]);
        for(uiLoop = 0; uiLoop < 4;uiLoop++)
        {
            ((UINT8 *)&(uiIp))[uiLoop] = uiTmp[uiLoop];
        }  
        uiIp = ntohl(uiIp);
		uiMask = atoi(apcArgv[3]);
		stWxlServerIp.stWxlServerIp.uiIp = uiIp;
		stWxlServerIp.stWxlServerIp.uiMask = uiMask;
		printf("ip: %d, mask: %d\n", uiIp, uiMask);
		enRet = nf_portal_cmd_add_wxl_serverip(&stWxlServerIp);
		return enRet;
	}
	else if (STR_MATCH(apcArgv[1],"del"))
	{
		if (iArgc != 4)
		{
			printf("del, iArgc(%d) != 4 \n", iArgc);
			return AP_E_PARAM;
		}
		sscanf(apcArgv[2],"%d.%d.%d.%d",&uiTmp[0],&uiTmp[1],&uiTmp[2],&uiTmp[3]);
        for(uiLoop = 0; uiLoop < 4;uiLoop++)
        {
            ((UINT8 *)&(uiIp))[uiLoop] = uiTmp[uiLoop];
        }  
        uiIp = ntohl(uiIp);
		uiMask = atoi(apcArgv[3]);
		stWxlServerIp.stWxlServerIp.uiIp = uiIp;
		stWxlServerIp.stWxlServerIp.uiMask = uiMask;
		printf("ip: %d, mask: %d\n", uiIp, uiMask);
		enRet = nf_portal_cmd_del_wxl_serverip(&stWxlServerIp);
		return enRet;
	}
	else if (STR_MATCH(apcArgv[1],"show"))
	{		
		stWxlServerIpList.uiNum = 0;
        stWxlServerIpList.uiStartNum = 0;
        
        printf("WXL SERVERIP IN THE LIST\n");
        SHOW_SPLIT_LINE(uiLineNum);
        while(TRUE)
        {
            enRet = nf_portal_cmd_get_wxl_serverip_list(&stWxlServerIpList);
            if (enRet != AP_E_NONE)
            {
                printf("Get list fail\n");
            }

            uiListNum += stWxlServerIpList.uiNum;
            for (uiLoop = 0; uiLoop < stWxlServerIpList.uiNum;uiLoop++)
            {
            	uiIp = htonl(stWxlServerIpList.astWxlServerip[uiLoop].uiIp);
                printf(IP_FMT"/%u\n", IP_ARG(&uiIp), 
								 stWxlServerIpList.astWxlServerip[uiLoop].uiMask);
            }

            if (stWxlServerIpList.uiNum < NF_PORTAL_WXL_SERVERIP_NUM_LIST_MAX)
            {
                break;
            }
        }
        
        SHOW_SPLIT_LINE(uiLineNum);
        printf("Total num:%u\n",uiListNum); 
        return AP_E_NONE;
	}
	else if (STR_MATCH(apcArgv[1],"flush"))
	{
		enRet = nf_portal_cmd_flush_wxl_serverip(&stWxlServerIp);
		return enRet;
	}
	printf("apcArgv[1] :%s error\n", apcArgv[1]);
	
	return AP_E_PARAM;
}

/*end: add@2015.01.31 for wechat connect wifi*/

AP_ERROR_CODE_E nf_portal_cmd_vlan_flood(INT32 iArgc, CHAR *apcArgv[])
{
    AP_ERROR_CODE_E enRet;
	BOOL bEnable;

    if (1 == iArgc)
    {
        enRet = nf_portal_cmd_get_vlan_flood(&bEnable);
        if (AP_E_NONE == enRet)
        {
            printf("VLAN FLOOD: %s\n", (bEnable ? "enable" : "diable"));
        }
        return enRet;
    }
    else if (2 == iArgc)
    {
		if (STR_MATCH(apcArgv[1], "enable"))
		{
			enRet = nf_portal_cmd_set_vlan_flood(TRUE);
			return enRet;
		}
		else if (STR_MATCH(apcArgv[1], "disable"))
		{
			enRet = nf_portal_cmd_set_vlan_flood(FALSE);
			return enRet;
		}
    }

    return AP_E_PARAM;
}


