/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : fc_ioctl.c
  版 本 号   : 初稿
  作    者   : Zhong Huaqing
  生成日期   : 2012年8月15日 星期三
  最近修改   :
  功能描述   : 流控模块IO接口，用于和用户态进行交互
  函数列表   :
  
  修改历史   :
  1.日    期   : 2012年8月15日 星期三
    作    者   : Zhong Huaqing
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "fc_main.h"

#include "fc_queue.h"
#include "fc_hash.h"

#include "fc_sta.h"
#include "fc_vap.h"
#include "fc_ssid.h"
#include "fc_ap.h"
#include "fc_vlan.h"

#include "fc_ioctl.h"
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

/*----------------------------------------------*
 * 函数定义                                       *
 *----------------------------------------------*/
AP_ERROR_CODE_E fc_ioctl_get_version(FC_IOCTL_VERSION_T *pstVersion)
{
    strncpy(pstVersion->acModVersion, FC_MOD_VERSION, FC_VERSION_SIZE);
    strncpy(pstVersion->acModDepVersion, FC_MOD_DEP_VERSION, FC_VERSION_SIZE);
    return AP_E_NONE;
}



AP_ERROR_CODE_E fc_ioctl_set_admin(FC_IOCTL_ADMIN_T *pstAdmin)
{
    fc_set_admin(pstAdmin->bEnable);
    return AP_E_NONE;
}

AP_ERROR_CODE_E fc_ioctl_get_admin(FC_IOCTL_ADMIN_T *pstAdmin)
{
    pstAdmin->bEnable = fc_get_admin();
    return AP_E_NONE;
}

AP_ERROR_CODE_E fc_ioctl_set_debug_level(FC_IOCTL_DEBUG_T *pstDebug)
{
    fc_set_debugLevel(pstDebug->iDebugLevel);
    return AP_E_NONE;
}

AP_ERROR_CODE_E fc_ioctl_get_debug_level(FC_IOCTL_DEBUG_T *pstDebug)
{
    pstDebug->iDebugLevel = fc_get_debugLevel();
    return AP_E_NONE;
}

AP_ERROR_CODE_E fc_ioctl_set_rule(FC_IOCTL_RULE_T *pstRule)
{
    fc_set_rule(pstRule->enRule);
    return AP_E_NONE;
}

AP_ERROR_CODE_E fc_ioctl_get_rule(FC_IOCTL_RULE_T *pstRule)
{
    pstRule->enRule = fc_get_rule();
    return AP_E_NONE;
}

AP_ERROR_CODE_E fc_ioctl_set_gear(FC_IOCTL_GEAR_T *pstGear)
{
    fc_set_gear_byte(pstGear->uiGearByte);
    return AP_E_NONE;
}

AP_ERROR_CODE_E fc_ioctl_get_gear(FC_IOCTL_GEAR_T *pstGear)
{
    pstGear->uiGearByte = fc_get_gear_byte();
    return AP_E_NONE;
}


AP_ERROR_CODE_E fc_ioctl_add_sta(FC_IOCTL_STA_T *pstSta)
{
    return fc_add_sta(&pstSta->stSta);
}

AP_ERROR_CODE_E fc_ioctl_del_sta(FC_IOCTL_STA_T *pstSta)
{
    return fc_del_sta(pstSta->stSta.acMac);
}

AP_ERROR_CODE_E fc_ioctl_add_vap(FC_IOCTL_VAP_T *pstVap)
{
    return fc_add_vap(&pstVap->stVap);
}

AP_ERROR_CODE_E fc_ioctl_del_vap(FC_IOCTL_VAP_T *pstVap)
{
    return fc_del_vap(pstVap->stVap.acVapName);
}

AP_ERROR_CODE_E fc_ioctl_add_ssid(FC_IOCTL_SSID_T *pstSsid)
{
    return fc_add_ssid(&pstSsid->stSsid);
}

AP_ERROR_CODE_E fc_ioctl_del_ssid(FC_IOCTL_SSID_T *pstSsid)
{
    return fc_del_ssid(pstSsid->stSsid.acSsidName);
}


AP_ERROR_CODE_E fc_ioctl_add_ap(FC_IOCTL_AP_T *pstAp)
{
    return fc_add_ap(&pstAp->stAp);
}

AP_ERROR_CODE_E fc_ioctl_del_ap(FC_IOCTL_AP_T *pstAp)
{
    return fc_del_ap();
}


AP_ERROR_CODE_E fc_ioctl_add_vlan(FC_IOCTL_VLAN_T *pstVlan)
{
    return fc_add_vlan(&pstVlan->stVlan);
}

AP_ERROR_CODE_E fc_ioctl_del_vlan(FC_IOCTL_VLAN_T *pstVlan)
{
    return fc_del_vlan(pstVlan->stVlan.vid);
}


AP_ERROR_CODE_E fc_ioctl_get_list(FC_IOCTL_ENTITY_LIST_T *pstList)
{
    UINT32 uiNum;
    uiNum = fc_queue_get_list(&pstList->stList, pstList->uiStartNum);
    pstList->uiNum = uiNum;
    
    return AP_E_NONE;
}


AP_ERROR_CODE_E fc_ioctl_flush(void)
{
    return fc_queue_flush_all();
}

#if 0
AP_ERROR_CODE_E fc_ioctl_set_auto_learn(FC_IOCTL_AUTO_LEARN_T *pstAutoLearn)
{
    return fc_set_auto_learn(&pstAutoLearn->stAutoLearn);
}

AP_ERROR_CODE_E fc_ioctl_get_auto_learn(FC_IOCTL_AUTO_LEARN_T *pstAutoLearn)
{
    return fc_get_auto_learn(&pstAutoLearn->stAutoLearn);
}
#endif

AP_ERROR_CODE_E fc_ioctl_set_auto_rate(FC_IOCTL_AUTO_RATE_T *pstAutoRate)
{
    fc_set_auto_rate_status(pstAutoRate->bIsOn);
    return AP_E_NONE;
}

AP_ERROR_CODE_E fc_ioctl_get_auto_rate(FC_IOCTL_AUTO_RATE_T *pstAutoRate)
{
    pstAutoRate->bIsOn = fc_get_auto_rate_status();
    return AP_E_NONE;
}


AP_ERROR_CODE_E fc_ioctl_set_dev_ssid(FC_IOCTL_DEV_SSID_T *pstDevSsid)
{
    return fc_set_dev_ssid(pstDevSsid->acDevName, pstDevSsid->acSsid);
}

AP_ERROR_CODE_E fc_ioctl_get_dev_ssid(FC_IOCTL_DEV_SSID_T *pstDevSsid)
{
    return fc_get_dev_ssid(pstDevSsid->acDevName, pstDevSsid->acSsid);
}

AP_ERROR_CODE_E fc_ioctl_get_dev_ssid_list(FC_IOCTL_DEV_SSID_LIST_T *pstDevSsidList)
{
    return fc_get_dev_ssid_list(pstDevSsidList);
}



/*****************************************************************************
 函 数 名  : flow_ctrl_ioctl_handler
 功能描述  : IO交互统一接口
 输入参数  : void  *pstArg  --用户态空间的指针
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月15日 星期三
    作    者   : Zhong Huaqing
    修改内容   : 新生成函数

*****************************************************************************/
AP_ERROR_CODE_E fc_ioctl_handler(void *pstArg)
{
	LOCAL FC_IOCTL_T stIoctl;
	BOOL bGetOper = FALSE;
	AP_ERROR_CODE_E enRet = AP_E_NONE;
	
    if (NULL == pstArg)
    {
        return AP_E_PARAM;
    }
    
	if (copy_from_user(&stIoctl, pstArg, sizeof(FC_IOCTL_T)))
	{
	    FC_LOG(FC_LOG_ERR, "Copy from user failed.\n");
		return AP_E_DEFAULT_FAIL;
	}
	
	FC_LOG(FC_LOG_DEBUG, "stIoctl.enCmd = %d\n", stIoctl.enCmd);
	
	switch (stIoctl.enCmd)
	{
        case FC_IOCTL_CMD_GET_VERSION: 
            enRet = fc_ioctl_get_version(&stIoctl.u.stVersion);
            bGetOper = TRUE; 
            break;
            
	    /* set/get admin */
	    case FC_IOCTL_CMD_SET_ADMIN: 
	        enRet = fc_ioctl_set_admin(&stIoctl.u.stAdmin); 
	        break;
        case FC_IOCTL_CMD_GET_ADMIN: 
            enRet = fc_ioctl_get_admin(&stIoctl.u.stAdmin); 
            bGetOper = TRUE; 
            break;
            
        /* set/get debug level */
        case FC_IOCTL_CMD_SET_DEBUG_LEVEL: 
            enRet = fc_ioctl_set_debug_level(&stIoctl.u.stDebug); 
            break;
        case FC_IOCTL_CMD_GET_DEBUG_LEVEL: 
            enRet = fc_ioctl_get_debug_level(&stIoctl.u.stDebug);
            bGetOper = TRUE; 
            break;
        
        
        /* set/get rule */
        case FC_IOCTL_CMD_SET_RULE: 
            fc_ioctl_set_rule(&stIoctl.u.stRule); 
            break;
        case FC_IOCTL_CMD_GET_RULE: 
            fc_ioctl_get_rule(&stIoctl.u.stRule); 
            bGetOper = TRUE; 
            break;
        
        /* set/get gear */
        case FC_IOCTL_CMD_SET_GEAR: 
            fc_ioctl_set_gear(&stIoctl.u.stGear); 
            break;
        case FC_IOCTL_CMD_GET_GEAR: 
            fc_ioctl_get_gear(&stIoctl.u.stGear); 
            bGetOper = TRUE; 
            break;
        
        /* add/del/get sta */
        case FC_IOCTL_CMD_ADD_STA: 
            enRet = fc_ioctl_add_sta(&stIoctl.u.stSta); 
            break;
        case FC_IOCTL_CMD_DEL_STA: 
            enRet = fc_ioctl_del_sta(&stIoctl.u.stSta); 
            break;
        
        /* add/del/get vap */
        case FC_IOCTL_CMD_ADD_VAP: 
            enRet = fc_ioctl_add_vap(&stIoctl.u.stVap); 
            break;
        case FC_IOCTL_CMD_DEL_VAP: 
            enRet = fc_ioctl_del_vap(&stIoctl.u.stVap); 
            break;
        
        /* add/del/get ssid */
        case FC_IOCTL_CMD_ADD_SSID: 
            enRet = fc_ioctl_add_ssid(&stIoctl.u.stSsid); 
            break;
        case FC_IOCTL_CMD_DEL_SSID: 
            enRet = fc_ioctl_del_ssid(&stIoctl.u.stSsid); 
            break;
        
        /* add/del/get ap */
        case FC_IOCTL_CMD_ADD_AP: 
            enRet = fc_ioctl_add_ap(&stIoctl.u.stAp); 
            break;
        case FC_IOCTL_CMD_DEL_AP: 
            enRet = fc_ioctl_del_ap(&stIoctl.u.stAp); 
            break;

		/* add/del vlan */
        case FC_IOCTL_CMD_ADD_VLAN: 
            enRet = fc_ioctl_add_vlan(&stIoctl.u.stVlan); 
            break;
        case FC_IOCTL_CMD_DEL_VLAN: 
            enRet = fc_ioctl_del_vlan(&stIoctl.u.stVlan); 
            break;
        
        /*Get list*/
        case FC_IOCTL_CMD_GET_LIST: 
            enRet = fc_ioctl_get_list(&stIoctl.u.stEntityList);
            bGetOper = TRUE; 
            break;
        
        /* Flush queue  */
        case FC_IOCTL_CMD_FLUSH: 
            enRet = fc_ioctl_flush(); 
            break;
        
        #if 0
        /* Auto learn */
        case FC_IOCTL_CMD_SET_AUTO_LEARN: 
            enRet = fc_ioctl_set_auto_learn(&stIoctl.u.stAutoLearn); 
            break;
        case FC_IOCTL_CMD_GET_AUTO_LEARN: 
            enRet = fc_ioctl_get_auto_learn(&stIoctl.u.stAutoLearn); 
            bGetOper = TRUE;
            break;
        #endif
        
         /* Auto rate */
        case FC_IOCTL_CMD_SET_AUTO_RATE: 
            enRet = fc_ioctl_set_auto_rate(&stIoctl.u.stAutoRate); 
            break;
        case FC_IOCTL_CMD_GET_AUTO_RATE: 
            enRet = fc_ioctl_get_auto_rate(&stIoctl.u.stAutoRate); 
            bGetOper = TRUE;
            break;
        
         /* set/get ssid , get ssid list*/
        case FC_IOCTL_CMD_SET_DEV_SSID: 
            enRet = fc_ioctl_set_dev_ssid(&stIoctl.u.stDevSsid); 
            break;
        case FC_IOCTL_CMD_GET_DEV_SSID: 
            enRet = fc_ioctl_get_dev_ssid(&stIoctl.u.stDevSsid); 
            bGetOper = TRUE;
            break;
        case FC_IOCTL_CMD_GET_DEV_SSID_LIST: 
            enRet = fc_ioctl_get_dev_ssid_list(&stIoctl.u.stDevSsidList); 
            bGetOper = TRUE;
            break;
        
        default:
			FC_LOG(FC_LOG_ERR, "Unknown cmd %d\n", stIoctl.enCmd);
			return  AP_E_PARAM;
			break;
	}
	
	if (AP_E_NONE != enRet)
	{
	    return AP_E_DEFAULT_FAIL;
	}
	
	if (bGetOper)
	{
    	if (copy_to_user(pstArg, &stIoctl, sizeof(FC_IOCTL_T)))
    	{
    		FC_LOG(FC_LOG_ERR, "Copy to user failed.\n");
    		return AP_E_DEFAULT_FAIL;
    	}
	}
	
	return AP_E_NONE;
}


