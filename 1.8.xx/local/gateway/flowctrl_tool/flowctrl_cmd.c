/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : fc_cmd.c
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

#include "fc_public.h"
#include "fc_ioctl.h"

#include "flowctrl_main.h"
#include "flowctrl_cmd.h"

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
 * 函数定义                                       *
 *----------------------------------------------*/

AP_ERROR_CODE_E fc_get_mac_from_str(CHAR *szMacDesc, UINT8 *pucMac)
{
    UINT32 auiMac[ETH_ALEN];
    UINT32 uiIndex;
    UINT32 uiRet;
    
    if (NULL == szMacDesc || NULL == pucMac)
    {
        return AP_E_PARAM;
    }
    
    uiRet = sscanf(szMacDesc, MAC_FMT, 
		&auiMac[0], &auiMac[1], &auiMac[2], 
		&auiMac[3], &auiMac[4], &auiMac[5]);
    if (6 != uiRet)
    {
        printf("MAC address format error!\n");
        return AP_E_PARAM;
    }
    
	for(uiIndex = 0; uiIndex < ETH_ALEN; uiIndex++)
	{
		pucMac[uiIndex] = auiMac[uiIndex];
	}
	
	return AP_E_NONE;
}

FC_RULE_E fc_cmd_get_current_rule(void)
{
    FC_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_GET_RULE);
    if (AP_E_NONE != enRet)
    {
        printf("Get rule failed. ret %d\n", enRet);
        return -1;
    }
    return stIoctl.u.stRule.enRule;
}

AP_ERROR_CODE_E fc_cmd_show_version(FC_IOCTL_VERSION_T *pstVersion)
{
    if (NULL == pstVersion)
    {
        return AP_E_PARAM;
    }
    
    printf("%-20s: %s\n", "mod version", pstVersion->acModVersion);
    printf("%-20s: %s\n", "app version", FC_APP_VERSION);
    printf("%-20s: %s\n", "mod dep version", pstVersion->acModDepVersion);
    printf("%-20s: %s\n", "app dep version", FC_APP_DEP_VERSION);
    
    return AP_E_NONE;
}

INT32 fc_parse_version_num(CHAR *szVer)
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

AP_ERROR_CODE_E fc_cmd_check_version(FC_IOCTL_VERSION_T *pstVersion)
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
    
    iAppVerNum = fc_parse_version_num(FC_APP_VERSION);
    iModVerNum = fc_parse_version_num(pstVersion->acModVersion);
    
    iAppDepVerNum = fc_parse_version_num(FC_APP_DEP_VERSION);
    iModDepVerNum = fc_parse_version_num(pstVersion->acModDepVersion);
    
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

AP_ERROR_CODE_E fc_cmd_version(INT32 iArgc, CHAR *apcArgv[])
{
    FC_IOCTL_T stIoctl;    
    FC_IOCTL_VERSION_T *pstVersion = NULL;
    AP_ERROR_CODE_E enRet;
    
    
    pstVersion = &stIoctl.u.stVersion;
    
    if (1 == iArgc)
    {
        memset(&stIoctl, 0, sizeof(FC_IOCTL_T));
        enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_GET_VERSION); 
        if (AP_E_NONE == enRet)
        {
            fc_cmd_show_version(pstVersion);
            enRet = fc_cmd_check_version(pstVersion);
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

AP_ERROR_CODE_E fc_cmd_admin(INT32 iArgc, CHAR *apcArgv[])
{
    FC_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    CHAR *szCmd;
    
    memset(&stIoctl, 0, sizeof(FC_IOCTL_T));
    
    if (1 == iArgc)
    {
        enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_GET_ADMIN);
        if (AP_E_NONE == enRet)
        {
            printf("Admin state: %s\n", 
                    stIoctl.u.stAdmin.bEnable? "Enable" : "Disable");
        }
        return enRet;
    }
    else if (2 == iArgc)
    {
        szCmd = apcArgv[1];
        if (0 == strcmp(szCmd, "enable"))
        {
            stIoctl.u.stAdmin.bEnable = TRUE;
        }
        else if (0 == strcmp(szCmd, "disable"))
        {
            stIoctl.u.stAdmin.bEnable = FALSE;
        }
        else
        {
            printf("Invalid parameter!\n");
            return AP_E_PARAM;
        }
        
        enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_SET_ADMIN);
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

AP_ERROR_CODE_E fc_cmd_debug_level(INT32 iArgc, CHAR *apcArgv[])
{
    FC_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    CHAR *szCmd;
    
    memset(&stIoctl, 0, sizeof(FC_IOCTL_T));
    
    if (1 == iArgc)
    {
        enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_GET_DEBUG_LEVEL);
        if (AP_E_NONE == enRet)
        {
            printf("Debug level: %d\n", 
                    stIoctl.u.stDebug.iDebugLevel);
        }
        return enRet;
    }
    else if (2 == iArgc)
    {
        szCmd = apcArgv[1];
        stIoctl.u.stDebug.iDebugLevel = atoi(szCmd);
        enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_SET_DEBUG_LEVEL);
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


AP_ERROR_CODE_E fc_cmd_rule(INT32 iArgc, CHAR *apcArgv[])
{
    FC_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    CHAR *szCmd;
    FC_RULE_E enRule;
    
    memset(&stIoctl, 0, sizeof(FC_IOCTL_T));
    
    if (1 == iArgc)
    {
        enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_GET_RULE);
        if (AP_E_NONE == enRet)
        {
            enRule = stIoctl.u.stRule.enRule;
            //printf("Rule is 0x%x\n", enRule);
            printf("Current rule: %s%s%s\n", 
                    (FC_RULE_STA & enRule) ? "STA" :
                    (FC_RULE_VAP & enRule) ? "VAP" :
                    (FC_RULE_SSID & enRule) ? "SSID" :
                    (FC_RULE_AP & enRule)  ? "AP" :
					(FC_RULE_VLAN & enRule)  ? "VLAN" :
                    "UNKNOWN",
                    FC_RULE_IS_MULTI(enRule) ? " MULTI" : "",
                    FC_RULE_IS_AUTO(enRule) ? " AUTO" : ""
                    );
        }
        return enRet;
    }
    else if (2 == iArgc || 3 == iArgc)
    {
        szCmd = apcArgv[1];
        if (STR_MATCH(szCmd, "sta"))
        {
            stIoctl.u.stRule.enRule = FC_RULE_STA;
        }
        else if (STR_MATCH(szCmd, "vap"))
        {
            stIoctl.u.stRule.enRule = FC_RULE_VAP;
        }
        else if (STR_MATCH(szCmd, "ssid"))
        {
            stIoctl.u.stRule.enRule = FC_RULE_SSID;
        }
        else if (STR_MATCH(szCmd, "ap"))
        {
            stIoctl.u.stRule.enRule = FC_RULE_AP;
        }
		else if (STR_MATCH(szCmd, "vlan"))
        {
            stIoctl.u.stRule.enRule = FC_RULE_VLAN;
        }
        else
        {
            printf("Invalid parameter!\n");
            return AP_E_PARAM;
        }
        
        if (3 == iArgc)
        {
            if (STR_MATCH(szCmd, "sta"))
            {
                printf("Invalid parameter!\n");
                return AP_E_PARAM;
            }
            if (STR_MATCH(apcArgv[2], "multi"))
            {
                stIoctl.u.stRule.enRule |= FC_RULE_MULTI;
            }
            else if (STR_MATCH(apcArgv[2], "auto"))
            {
                stIoctl.u.stRule.enRule |= FC_RULE_AUTO;
            }
        }
        
        enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_SET_RULE);
        if (AP_E_NONE == enRet)
        {
            printf("Set OK! Set rule to be 0x%x\n", stIoctl.u.stRule.enRule);
        }
        return enRet;
    }
    else
    {
        return AP_E_PARAM;
    }
}


AP_ERROR_CODE_E fc_cmd_gear(INT32 iArgc, CHAR *apcArgv[])
{
    FC_IOCTL_T stIoctl;
    UINT32 uiGearByte;
    AP_ERROR_CODE_E enRet;
    
    memset(&stIoctl, 0, sizeof(FC_IOCTL_T));
    
    if (1 == iArgc)
    {
        enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_GET_GEAR);
        if (AP_E_NONE == enRet)
        {
            printf("Gear byte: %d\n", stIoctl.u.stGear.uiGearByte);
        }
        return enRet;
    }
    else if (2 == iArgc)
    {
        uiGearByte = atoi(apcArgv[1]);
        #if 0
        if (uiGearByte >= FC_MTU)
        {
            return AP_E_PARAM;
        }
        #endif
        stIoctl.u.stGear.uiGearByte = uiGearByte;
        
        enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_SET_GEAR);
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

AP_ERROR_CODE_E fc_cmd_add_sta(INT32 iArgc, CHAR *apcArgv[])
{
    FC_IOCTL_T stIoctl;
    FC_IOCTL_STA_T *pstSta;
    AP_ERROR_CODE_E enRet;
    CHAR *szCmd;
    
    UINT32 radio_id;
    UINT32 wlan_id;
    UINT32 uiIndex;
    
    FC_RULE_E enRule;
    
    if (8 != iArgc)
    {
        return AP_E_PARAM;
    }
    uiIndex = 1;
    
    enRule = fc_cmd_get_current_rule();
    if (FC_RULE_STA != enRule 
        && !FC_RULE_IS_MULTI(enRule))
    {
        printf("Current rule is not for STA.\n");
        return AP_E_RESOURCE;
    }
    
    pstSta = &stIoctl.u.stSta;
    
    
    pstSta->stSta.iRadioId = atoi(apcArgv[1]);
    pstSta->stSta.iWlanId  = atoi(apcArgv[2]);
    enRet = fc_get_mac_from_str(apcArgv[3], pstSta->stSta.acMac);
    if (AP_E_NONE != enRet)
    {
        return AP_E_DEFAULT_FAIL;
    }
    
    pstSta->stSta.astRate[FC_DIR_US].uiRate = atoi(apcArgv[4]);
    pstSta->stSta.astRate[FC_DIR_US].uiBurst= atoi(apcArgv[5]);
    pstSta->stSta.astRate[FC_DIR_DS].uiRate = atoi(apcArgv[6]);
    pstSta->stSta.astRate[FC_DIR_DS].uiBurst= atoi(apcArgv[7]);
    
    enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_ADD_STA);
    if (AP_E_NONE == enRet)
    {
        printf("Add STA OK!\n");
    }
    return enRet;
    
}

AP_ERROR_CODE_E fc_cmd_del_sta(INT32 iArgc, CHAR *apcArgv[])
{
    FC_IOCTL_T stIoctl;
    FC_IOCTL_STA_T *pstSta;
    AP_ERROR_CODE_E enRet;
    CHAR *szCmd;
    
    FC_RULE_E enRule;
    
    if (2 != iArgc)
    {
        return AP_E_PARAM;
    }
    
    enRule = fc_cmd_get_current_rule();
    if (FC_RULE_STA != enRule 
        && !FC_RULE_IS_MULTI(enRule))

    {
        printf("Current rule is not for STA.\n");
        return AP_E_RESOURCE;
    }
    
    pstSta = &stIoctl.u.stSta;
    enRet = fc_get_mac_from_str(apcArgv[1], pstSta->stSta.acMac);
    if (AP_E_NONE != enRet)
    {
        return AP_E_DEFAULT_FAIL;
    }
    
    enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_DEL_STA);
    if (AP_E_NONE == enRet)
    {
        printf("Del STA OK!\n");
    }
    return enRet;
    
}


AP_ERROR_CODE_E fc_cmd_add_vap(INT32 iArgc, CHAR *apcArgv[])
{
    FC_IOCTL_T stIoctl;
    FC_IOCTL_VAP_T *pstVap;
    AP_ERROR_CODE_E enRet;
    
    FC_RULE_E enRule;
    
    if (6 != iArgc)
    {
        return AP_E_PARAM;
    }
    
    
    enRule = fc_cmd_get_current_rule();
    if (!(FC_RULE_VAP & enRule))
    {
        printf("Current rule is not for VAP.\n");
        return AP_E_RESOURCE;
    }
    
    pstVap = &stIoctl.u.stVap;
    strncpy(pstVap->stVap.acVapName, apcArgv[1], IFNAMSIZ);
    pstVap->stVap.astRate[FC_DIR_US].uiRate = atoi(apcArgv[2]);
    pstVap->stVap.astRate[FC_DIR_US].uiBurst= atoi(apcArgv[3]);
    pstVap->stVap.astRate[FC_DIR_DS].uiRate = atoi(apcArgv[4]);
    pstVap->stVap.astRate[FC_DIR_DS].uiBurst= atoi(apcArgv[5]);
    
    enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_ADD_VAP);
    if (AP_E_NONE == enRet)
    {
        printf("Add VAP OK!\n");
    }
    return enRet;
    
}

AP_ERROR_CODE_E fc_cmd_del_vap(INT32 iArgc, CHAR *apcArgv[])
{
    FC_IOCTL_T stIoctl;
    FC_IOCTL_VAP_T *pstVap;
    AP_ERROR_CODE_E enRet;
    FC_RULE_E enRule;
    
    if (2 != iArgc)
    {
        return AP_E_PARAM;
    }
    
    enRule = fc_cmd_get_current_rule();
    if (!(FC_RULE_VAP & enRule))
    {
        printf("Current rule is not for VAP.\n");
        return AP_E_RESOURCE;
    }
    
    pstVap = &stIoctl.u.stVap;
    strncpy(pstVap->stVap.acVapName, apcArgv[1], IFNAMSIZ);
    
    enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_DEL_VAP);
    if (AP_E_NONE == enRet)
    {
        printf("Del VAP OK!\n");
    }
    return enRet;
    
}


AP_ERROR_CODE_E fc_cmd_add_vlan(INT32 iArgc, CHAR *apcArgv[])
{
    FC_IOCTL_T stIoctl;
    FC_IOCTL_VLAN_T *pstVlan;
    AP_ERROR_CODE_E enRet;
    
    FC_RULE_E enRule;
    
    if (6 != iArgc)
    {
        return AP_E_PARAM;
    }
    
    enRule = fc_cmd_get_current_rule();
    if (!(FC_RULE_VLAN & enRule))
    {
        printf("Current rule is not for VLAN.\n");
        return AP_E_RESOURCE;
    }
    
    pstVlan = &stIoctl.u.stVlan;
    pstVlan->stVlan.vid = atoi(apcArgv[1]);
	if((pstVlan->stVlan.vid <= 0) || (pstVlan->stVlan.vid > 4095))
	{
        printf("Add VLAN INVALID. vid is %d\n", pstVlan->stVlan.vid);
		return AP_E_PARAM;
    }

    pstVlan->stVlan.astRate[FC_DIR_US].uiRate = atoi(apcArgv[2]);
    pstVlan->stVlan.astRate[FC_DIR_US].uiBurst= atoi(apcArgv[3]);
    pstVlan->stVlan.astRate[FC_DIR_DS].uiRate = atoi(apcArgv[4]);
    pstVlan->stVlan.astRate[FC_DIR_DS].uiBurst= atoi(apcArgv[5]);
    
    enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_ADD_VLAN);
    if (AP_E_NONE == enRet)
    {
        printf("Add VLAN %d OK!\n", pstVlan->stVlan.vid);
    }
    return enRet;
    
}

AP_ERROR_CODE_E fc_cmd_del_vlan(INT32 iArgc, CHAR *apcArgv[])
{
    FC_IOCTL_T stIoctl;
    FC_IOCTL_VLAN_T *pstVlan;
    AP_ERROR_CODE_E enRet;
    FC_RULE_E enRule;
    
    if (2 != iArgc)
    {
        return AP_E_PARAM;
    }
    
    enRule = fc_cmd_get_current_rule();
    if (!(FC_RULE_VLAN & enRule))
    {
        printf("Current rule is not for VLAN.\n");
        return AP_E_RESOURCE;
    }
    
    pstVlan = &stIoctl.u.stVlan;
	pstVlan->stVlan.vid = atoi(apcArgv[1]);
	if((pstVlan->stVlan.vid <= 0) || (pstVlan->stVlan.vid > 4095))
	{
		printf("Del VLAN INVALID. vid is %d\n", pstVlan->stVlan.vid);
		return AP_E_PARAM;
	}
    
    enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_DEL_VLAN);
    if (AP_E_NONE == enRet)
    {
        printf("Del VLAN %d OK!\n", pstVlan->stVlan.vid);
    }
    return enRet;
    
}



AP_ERROR_CODE_E fc_cmd_add_ssid(INT32 iArgc, CHAR *apcArgv[])
{
    FC_IOCTL_T stIoctl;
    FC_IOCTL_SSID_T *pstSsid;
    AP_ERROR_CODE_E enRet;
    FC_RULE_E enRule;
    
    if (6 != iArgc)
    {
        return AP_E_PARAM;
    }
    
    enRule = fc_cmd_get_current_rule();
    if (!(FC_RULE_SSID & enRule))
    {
        printf("Current rule is not for SSID.\n");
        return AP_E_RESOURCE;
    }
    
    pstSsid = &stIoctl.u.stSsid;
    strncpy(pstSsid->stSsid.acSsidName, apcArgv[1], SSID_NAMSIZ);
    pstSsid->stSsid.astRate[FC_DIR_US].uiRate = atoi(apcArgv[2]);
    pstSsid->stSsid.astRate[FC_DIR_US].uiBurst= atoi(apcArgv[3]);
    pstSsid->stSsid.astRate[FC_DIR_DS].uiRate = atoi(apcArgv[4]);
    pstSsid->stSsid.astRate[FC_DIR_DS].uiBurst= atoi(apcArgv[5]);
    
    enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_ADD_SSID);
    if (AP_E_NONE == enRet)
    {
        printf("Add SSID OK!\n");
    }
    return enRet;
    
}



AP_ERROR_CODE_E fc_cmd_del_ssid(INT32 iArgc, CHAR *apcArgv[])
{
    FC_IOCTL_T stIoctl;
    FC_IOCTL_SSID_T *pstSsid;
    AP_ERROR_CODE_E enRet;
    FC_RULE_E enRule;
    
    if (2 != iArgc)
    {
        return AP_E_PARAM;
    }
    
    enRule = fc_cmd_get_current_rule();
    if (!(FC_RULE_SSID & enRule))
    {
        printf("Current rule is not for SSID.\n");
        return AP_E_RESOURCE;
    }
    
    pstSsid = &stIoctl.u.stSsid;
    strncpy(pstSsid->stSsid.acSsidName, apcArgv[1], SSID_NAMSIZ);
    
    enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_DEL_SSID);
    if (AP_E_NONE == enRet)
    {
        printf("Del SSID OK!\n");
    }
    return enRet;
    
}

AP_ERROR_CODE_E fc_cmd_add_ap(INT32 iArgc, CHAR *apcArgv[])
{
    FC_IOCTL_T stIoctl;
    FC_IOCTL_AP_T *pstAp;
    AP_ERROR_CODE_E enRet;
    FC_RULE_E enRule;
    
    if (5 != iArgc)
    {
        return AP_E_PARAM;
    }
    
    enRule = fc_cmd_get_current_rule();
    if (!(FC_RULE_AP & enRule))
    {
        printf("Current rule is not for AP.\n");
        return AP_E_RESOURCE;
    }
    
    pstAp = &stIoctl.u.stAp;
    pstAp->stAp.astRate[FC_DIR_US].uiRate = atoi(apcArgv[1]);
    pstAp->stAp.astRate[FC_DIR_US].uiBurst= atoi(apcArgv[2]);
    pstAp->stAp.astRate[FC_DIR_DS].uiRate = atoi(apcArgv[3]);
    pstAp->stAp.astRate[FC_DIR_DS].uiBurst= atoi(apcArgv[4]);
    
    enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_ADD_AP);
    if (AP_E_NONE == enRet)
    {
        printf("Add AP OK!\n");
    }
    return enRet;
    
}

AP_ERROR_CODE_E fc_cmd_del_ap(INT32 iArgc, CHAR *apcArgv[])
{
    FC_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    FC_RULE_E enRule;
    
    if (1 != iArgc)
    {
        return AP_E_PARAM;
    }
    
    enRule = fc_cmd_get_current_rule();
    if (!(FC_RULE_AP & enRule))
    {
        printf("Current rule is not for AP.\n");
        return AP_E_RESOURCE;
    }
    
    enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_DEL_AP);
    if (AP_E_NONE == enRet)
    {
        printf("Del AP OK!\n");
    }
    return enRet;
    
}


CHAR *fc_cmd_get_parent_desc(FC_RULE_E enRule, FC_ENTITY_T *pstEntity)
{
    if (!(enRule & FC_RULE_MULTI)
        && !(enRule & FC_RULE_AUTO))
    {
        return "--";
    }
    
    if (FC_RID_INVALID == pstEntity->stMatchAttrib.iRadioId
        || FC_WID_INVALID == pstEntity->stMatchAttrib.iWlanId)
    {
        return "--";
    }
    
    enRule &= (~(FC_RULE_MULTI | FC_RULE_AUTO));
    switch(enRule)
    {
        case FC_RULE_VAP:
            return pstEntity->stMatchAttrib.acVapName;
            break;
        case FC_RULE_SSID:
            return pstEntity->stMatchAttrib.acSsidName;
            break;
        case FC_RULE_AP:
            return "AP";
            break;
		case FC_RULE_VLAN:
            return "VLAN";
            break;
        default:
            return NULL;
    }
    return NULL;
}

AP_ERROR_CODE_E fc_cmd_get_list(INT32 iArgc, CHAR *apcArgv[])
{
    FC_IOCTL_T stIoctl;
    FC_IOCTL_ENTITY_LIST_T *pstList;
    FC_ENTITY_T *pstEntity;
    FC_QUEUE_STATS_T *pstStats;
    
    UINT32 uiIndex;
    INT32 uiNum = 128;
    CHAR acAdminRate[128];
    CHAR acRealRate[128];
    CHAR acStats[256];
    
    CHAR acRuleDesc[128];
    
    CHAR *szParent = NULL;
    
    FC_RULE_E enRule;
    AP_ERROR_CODE_E enRet;
    
    
    if (1 != iArgc)
    {
        return AP_E_PARAM;
    }
    
    enRule = fc_cmd_get_current_rule();
    
    pstList = &stIoctl.u.stEntityList;

    
    /*显示表头*/
    printf("\n");
    //printf("%-18s%-8s%-8s%-8s%-8s%-30s%-30s\n",
    printf("%-18s%-18s%-32s%-32s%-64s\n",
        (FC_RULE_STA & enRule)? "STA MAC" :
        (FC_RULE_VAP & enRule)? "VAP NAME" :
        (FC_RULE_SSID & enRule)? "SSID" :
		(FC_RULE_VLAN & enRule)? "VLAN ID" :
        (FC_RULE_AP & enRule)? "AP" : "UNKNOWN", 
        "parent",
        "admin_rate", 
        "real_rate", 
        "stats"
        );
    SHOW_SPLIT_LINE(uiNum);
    fflush(stdout);
    
    pstList->uiNum= 0;
    pstList->uiStartNum = 0;
    while(TRUE)
    {
        pstList->uiStartNum += pstList->uiNum; /*Move on*/
        
        enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_GET_LIST);
        if (AP_E_NONE != enRet)
        {
            printf("Get list failed! ret %d\n", enRet);
        }
        
        /*显示*/
        for (uiIndex = 0; uiIndex < pstList->uiNum; uiIndex++)
        {
            pstEntity = &(pstList->stList.astEntity[uiIndex]);
            pstStats = pstEntity->astStats;
            
            szParent = fc_cmd_get_parent_desc(enRule, pstEntity);
            
            sprintf(acAdminRate, "%d:%d/%d:%d",
                    pstEntity->astAdminRate[FC_DIR_US].uiRate,
                    pstEntity->astAdminRate[FC_DIR_US].uiBurst,
                    pstEntity->astAdminRate[FC_DIR_DS].uiRate,
                    pstEntity->astAdminRate[FC_DIR_DS].uiBurst
                    );
            
            sprintf(acRealRate, "%d:%d/%d:%d",
                    pstEntity->astRealRate[FC_DIR_US].uiRate,
                    pstEntity->astRealRate[FC_DIR_US].uiBurst,
                    pstEntity->astRealRate[FC_DIR_DS].uiRate,
                    pstEntity->astRealRate[FC_DIR_DS].uiBurst
                    );
            
            sprintf(acStats, "%llu:%llu/%llu:%llu",
                pstStats[FC_DIR_US].uiBytePassFast 
                    + pstStats[FC_DIR_US].uiBytePassDelay,
                pstStats[FC_DIR_US].uiByteDrop,
                
                pstStats[FC_DIR_DS].uiBytePassFast 
                    + pstStats[FC_DIR_DS].uiBytePassDelay,
                pstStats[FC_DIR_DS].uiByteDrop
                );
                
            
            
            if (FC_RID_INVALID != pstEntity->stMatchAttrib.iRadioId
                && FC_WID_INVALID != pstEntity->stMatchAttrib.iWlanId)
            {
                sprintf(acRuleDesc, MAC_FMT, MAC_ARG(pstEntity->stMatchAttrib.acStaMac));
            }
            else
            {
                switch (enRule & (~(FC_RULE_MULTI | FC_RULE_AUTO)))
                {
                    case FC_RULE_STA:
                        sprintf(acRuleDesc, MAC_FMT, MAC_ARG(pstEntity->stMatchAttrib.acStaMac));
                        break;
                    case FC_RULE_VAP:
                        strncpy(acRuleDesc, pstEntity->stMatchAttrib.acVapName, IFNAMSIZ);
                        break;
                    case FC_RULE_SSID:
                        strncpy(acRuleDesc, pstEntity->stMatchAttrib.acSsidName, SSID_NAMSIZ);
                        break;
                    case FC_RULE_AP:
                        sprintf(acRuleDesc, "AP");
                        break;
					case FC_RULE_VLAN:
                        sprintf(acRuleDesc, "%04d", pstEntity->stMatchAttrib.iVid);
                        break;
                    default:
                        return AP_E_RESOURCE;
                }
            }
            
            
            printf("%-18s%-18s%-32s%-32s%-64s\n",
                    acRuleDesc,
                    szParent,
                    acAdminRate,
                    acRealRate,
                    acStats
                    );
                                
        }
        
        /*已经获取全部的list*/
        if (pstList->uiNum < FC_ENTITY_LIST_SIZE)
        {
            break;
        }
    }
    SHOW_SPLIT_LINE(uiNum);
    printf("Total num: %d\n\n", pstList->uiStartNum + pstList->uiNum);
    
    return AP_E_NONE;
    
}

AP_ERROR_CODE_E fc_cmd_flush(INT32 iArgc, CHAR *apcArgv[])
{
    FC_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    
    if (1 != iArgc)
    {
        return AP_E_PARAM;
    }
        
    enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_FLUSH);
    if (AP_E_NONE == enRet)
    {
        printf("Flush Queues OK!\n");
    }
    return enRet;
    
}


AP_ERROR_CODE_E fc_cmd_auto_rate(INT32 iArgc, CHAR *apcArgv[])
{
    FC_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    CHAR *szCmd;
    
    memset(&stIoctl, 0, sizeof(FC_IOCTL_T));
    
    if (1 == iArgc)
    {
        enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_GET_AUTO_RATE);
        if (AP_E_NONE == enRet)
        {
            printf("Auto-rate state: %s\n", 
                    stIoctl.u.stAutoRate.bIsOn ? "ON" : "OFF");
        }
        return enRet;
    }
    else if (2 == iArgc)
    {
        szCmd = apcArgv[1];
        if (STR_MATCH(szCmd, "on"))
        {
            stIoctl.u.stAutoRate.bIsOn = TRUE;
        }
        else if (STR_MATCH(szCmd, "off"))
        {
            stIoctl.u.stAutoRate.bIsOn = FALSE;
        }
        else
        {
            printf("Invalid parameter!\n");
            return AP_E_PARAM;
        }
        
        enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_SET_AUTO_RATE);
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

AP_ERROR_CODE_E fc_cmd_show(INT32 iArgc, CHAR *apcArgv[])
{
    FC_IOCTL_T stIoctl;
    
    FC_IOCTL_ADMIN_T stAdmin;
    FC_IOCTL_DEBUG_T stDebug;
    FC_IOCTL_RULE_T stRule;
    
    FC_IOCTL_GEAR_T stGear;
    
    //FC_IOCTL_AUTO_LEARN_T stAutoLearn;
    FC_IOCTL_AUTO_RATE_T stAutoRate;
    
    AP_ERROR_CODE_E enRet;
    
    
    if (1 != iArgc)
    {
        return AP_E_PARAM;
    }
    
    /*Get Admin*/
    enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_GET_ADMIN);
    if (AP_E_NONE != enRet)
    {
        return enRet;
    }
    stAdmin = stIoctl.u.stAdmin;
    
    /*Get debug level*/
    enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_GET_DEBUG_LEVEL);
    if (AP_E_NONE != enRet)
    {
        return enRet;
    }
    stDebug = stIoctl.u.stDebug;
    
    /*Get rule*/
    enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_GET_RULE);
    if (AP_E_NONE != enRet)
    {
        return enRet;
    }
    stRule = stIoctl.u.stRule;
    
    /*Get gear*/
    enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_GET_GEAR);
    if (AP_E_NONE != enRet)
    {
        return enRet;
    }
    stGear = stIoctl.u.stGear;
    
    #if 0
    /*Get stAutoLearn*/
    enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_GET_AUTO_LEARN);
    if (AP_E_NONE != enRet)
    {
        return enRet;
    }
    stAutoLearn = stIoctl.u.stAutoLearn;
    #endif
    
    /*Get stAutoRate*/
    enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_GET_AUTO_RATE);
    if (AP_E_NONE != enRet)
    {
        return enRet;
    }
    stAutoRate = stIoctl.u.stAutoRate;
    
    printf("\n");
    
    printf("%-15s: %s\n", "Admin", stAdmin.bEnable ? "Enable" : "Disable");
    printf("%-15s: %d\n", "Debug level", stDebug.iDebugLevel);
    printf("%-15s: %s%s%s\n", "Rule",
                    (FC_RULE_STA & stRule.enRule) ? "STA" :
                    (FC_RULE_VAP & stRule.enRule) ? "VAP" :
                    (FC_RULE_SSID & stRule.enRule) ? "SSID" :
                    (FC_RULE_AP & stRule.enRule)  ? "AP" :
					(FC_RULE_VLAN & stRule.enRule)  ? "VLAN" :
                    "UNKNOWN",
                    (FC_RULE_MULTI & stRule.enRule) ? " MULTI" : "",
                    (FC_RULE_AUTO & stRule.enRule) ? " AUTO" : ""
                    );
    printf("%-15s: %d\n", "Gear byte", stGear.uiGearByte);
    
    #if 0
    if (stAutoLearn.stAutoLearn.bIsOn)
    {
        printf("%-15s: ON;  usrate %d usburst %d; dsrate %d dsburst %d\n",
                   "Auto Learning",
                   stAutoLearn.stAutoLearn.astRate[FC_DIR_US].uiRate,
                   stAutoLearn.stAutoLearn.astRate[FC_DIR_US].uiBurst,
                   stAutoLearn.stAutoLearn.astRate[FC_DIR_DS].uiRate,
                   stAutoLearn.stAutoLearn.astRate[FC_DIR_DS].uiBurst
                   );
    }
    else
    {
        printf("%-15s: OFF\n", "Auto Learning");
    }
    #endif
    
    printf("%-15s: %s\n", "Auto Rate", stAutoRate.bIsOn ? "ON" : "OFF");
    //printf("\n");   
    
    return fc_cmd_get_list(iArgc, apcArgv);
        
}



AP_ERROR_CODE_E fc_cmd_dev_ssid(INT32 iArgc, CHAR *apcArgv[])
{
    FC_IOCTL_T stIoctl;
    AP_ERROR_CODE_E enRet;
    CHAR *szCmd;
    UINT32 uiIndex;
    
    memset(&stIoctl, 0, sizeof(FC_IOCTL_T));
    
    if (1 == iArgc) /*Get list*/
    {
        
        enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_GET_DEV_SSID_LIST);
        if (AP_E_NONE != enRet)
        {
            printf("Get failed.\n");
            return enRet;
        }
        printf("%-16s %-32s\n", "dev name", "ssid");
        SHOW_SPLIT_LINE(50);
        for (uiIndex = 0; uiIndex < stIoctl.u.stDevSsidList.uiNum; uiIndex++)
        {
            printf("%-16s %-32s\n", 
                stIoctl.u.stDevSsidList.astDevSsidList[uiIndex].acDevName,
                stIoctl.u.stDevSsidList.astDevSsidList[uiIndex].acSsid
                );
        }
        SHOW_SPLIT_LINE(50);
        printf("Total item num: %d\n", stIoctl.u.stDevSsidList.uiNum);
        
        return AP_E_NONE;
    }
    if (2 == iArgc) /*Get dev ssid*/
    {
        strncpy(stIoctl.u.stDevSsid.acDevName, apcArgv[1], IFNAMSIZ);
        enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_GET_DEV_SSID);
        if (AP_E_NONE == enRet)
        {
            printf("%s's ssid: %s\n", 
                    stIoctl.u.stDevSsid.acDevName,
                    stIoctl.u.stDevSsid.acSsid);
        }
        return enRet;
    }
    else if (3 == iArgc) /*Set dev ssid*/
    {
        strncpy(stIoctl.u.stDevSsid.acDevName, apcArgv[1], IFNAMSIZ);
        strncpy(stIoctl.u.stDevSsid.acSsid, apcArgv[2], SSID_NAMSIZ);
        
        enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_SET_DEV_SSID);
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

AP_ERROR_CODE_E fc_cmd_dest_ip_white_list(INT32 iArgc, CHAR *apcArgv[])
{
    FC_IOCTL_T stIoctl;
    FC_IOCTL_IP_LIST_T *pstIpList;
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

    memset(&stIoctl, 0, sizeof(FC_IOCTL_T));
    
    if (3 == iArgc)
    {
        /*清空表项*/
        if (STR_MATCH(apcArgv[2],"flush"))
        {
            enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_FLUSH_IP_LIST);
            if (enRet == AP_E_NONE)
            {
                printf("FLUSH ALL\n");
            }
            return enRet;
        }
        /*获取所有表项*/
        else if (STR_MATCH(apcArgv[2],"show"))
        {
            pstIpList = &stIoctl.u.stIpList;
            pstIpList->uiNum = 0;
            pstIpList->uiStartNum = 0;
            
            printf("DEST IP IN THE LIST:\n");
            SHOW_SPLIT_LINE(uiLineNum);
            while(TRUE)
            {
                enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_GET_IP_LIST);
                if (enRet != AP_E_NONE)
                {
                    printf("Get list fail\n");
                }

                uiListNum += pstIpList->uiNum;
                for (uiLoop = 0; uiLoop < pstIpList->uiNum;uiLoop++)
                {
                    uiIp = htonl(pstIpList->astIp[uiLoop].uiIp);
                    printf(IP_FMT"\n",IP_ARG(&uiIp));
                }

                if (pstIpList->uiNum < FC_IP_LIST_MAX)
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
            sscanf(apcArgv[3],"%d.%d.%d.%d",&uiTmp[0],&uiTmp[1],&uiTmp[2],&uiTmp[3]);
            for(uiLoop = 0; uiLoop < 4;uiLoop++)
            {
                ((UINT8 *)&(uiIp))[uiLoop] = uiTmp[uiLoop];
            }

            stIoctl.u.stIp.uiIp = ntohl(uiIp);

            enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_ADD_IP);
            if (enRet == AP_E_NONE)
            {
                printf("Add Rule To The List Ok\n");
            }
            return enRet;
        }
        else if (STR_MATCH(apcArgv[2],"del"))
        {
            sscanf(apcArgv[3],"%d.%d.%d.%d",&uiTmp[0],&uiTmp[1],&uiTmp[2],&uiTmp[3]);
            for(uiLoop = 0; uiLoop < 4;uiLoop++)
            {
                ((UINT8 *)&(uiIp))[uiLoop] = uiTmp[uiLoop];
            }
            stIoctl.u.stIp.uiIp = ntohl(uiIp);
            
            enRet = fc_ioctl(&stIoctl, FC_IOCTL_CMD_DEL_IP);
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
AP_ERROR_CODE_E fc_cmd_white_list(INT32 iArgc, CHAR *apcArgv[])
{
    AP_ERROR_CODE_E enRet;
    
    if (iArgc < 3)
    {
        return AP_E_PARAM;
    }

    if (STR_MATCH(apcArgv[1],"ip"))
    {
        enRet = fc_cmd_dest_ip_white_list(iArgc,apcArgv);
        return enRet;
    }
    return AP_E_PARAM;
}
AP_ERROR_CODE_E fc_cmd_help(INT32 iArgc, CHAR *apcArgv[])
{
    fc_show_help();
    return AP_E_NONE;
}

FC_CMD_T g_astCmd[] =
{
	{"version", fc_cmd_version, ""},
	{"admin", fc_cmd_admin, "[ enable | disable]"},
	{"debuglevel", fc_cmd_debug_level, "[ 0-7]"},
	{"rule", fc_cmd_rule, "[ sta | {{vap | ssid | ap | vlan} [multi | auto]} ]"},
	
	{"gear", fc_cmd_gear, "[0 - 1517]"},
	
	{"addsta", fc_cmd_add_sta, "rid wid xx:xx:xx:xx:xx:xx uprate upburst downrate downburst"},
	{"delsta", fc_cmd_del_sta, "xx:xx:xx:xx:xx:xx"},
	
	{"addvap", fc_cmd_add_vap, "VAPNAME uprate upburst downrate downburst"},
	{"delvap", fc_cmd_del_vap, "VAPNAME"},
		
	{"addssid", fc_cmd_add_ssid, "SSIDNAME uprate upburst downrate downburst"},
	{"delssid", fc_cmd_del_ssid, "SSIDNAME"},
			
	{"addap", fc_cmd_add_ap, "uprate downrate burst_size"},
	{"delap", fc_cmd_del_ap, ""},

	{"addvlan", fc_cmd_add_vlan, "VLANID uprate upburst downrate downburst"},
	{"delvlan", fc_cmd_del_vlan, "VLANID"},
	
	{"getlist", fc_cmd_get_list, ""},
	
	{"flush", fc_cmd_flush, ""},
	
	//{"auto-learn", fc_cmd_auto_learn, "[off | on uprate upburst downrate downburst ]"},
	{"auto-rate", fc_cmd_auto_rate, "[on | off]"},
	
	{"devssid", fc_cmd_dev_ssid, "[devname [ssid]]"},
	
	{"show", fc_cmd_show, ""},
    	{"white_list", fc_cmd_white_list,"{ip} {add ip |del ip | show |flush }"},
	{"help", fc_cmd_help, ""},
	
	{NULL, NULL, NULL}
};

FC_CMD_T *fc_get_cmd(CHAR *szCmdName)
{
    FC_CMD_T *pstPos;
    for (pstPos = g_astCmd; pstPos->fHandler; pstPos++)
    {
        if (STR_MATCH(szCmdName, pstPos->szCmdName))
        {
            return pstPos;
        }
    }
    return NULL;
}

void fc_show_help(void)
{
    FC_CMD_T *pstPos;
    for (pstPos = g_astCmd; pstPos->fHandler; pstPos++)
    {
        printf("\t%-20s -- %s\n", pstPos->szCmdName, pstPos->szHelpInfo);
    }
    return;
}


