/******************************************************************************
* COPYRIGHT GBCOM
*******************************************************************************
* 功    能: 
* 修改历史: 
* 2012-9-10     冯静              新建
******************************************************************************/

#include <syslog.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <libgen.h>

#include "snmp_nms.h"
#include "mini_snmpd.h"
#include "my_snmp_def.h"
#include "my_snmp_data.h"
#include "my_snmp_info.h"
#include "my_snmp_handle.h"
#include "my_snmp_config.h"
#include "my_snmp_util.h"
#include "my_snmp_trap.h"
#include "util.h"
//#include "om_types.h"
//#include "om_log.h"
//#include "om_app.h"
#include "ftplib.h"
//#include "om_lte.h"
//#include "om_net.h"
#include "lte.h"
//#include "nvram_define.h"
//#include "nvram.h"

extern SNMP_AGENT_DATA_T *g_pstSnmpAgentData;
extern int g_stati_per;
//extern int wlan_del_vap(AP_CONFIG_SSID_ENTRY_T *pstSsidSetup);
//extern BOOL wlan_add_vap(AP_CONFIG_SSID_ENTRY_T *pstSsidSetup);
BOOL snmp_file_transmit(SNMP_FILE_TRANS_INFO_T *pstFileTrans);
BOOL snmp_shell_script_exec(char* pShellScriptPath);
BOOL snmp_update_shell_files(SNMP_AGENT_DATA_T* pstSnmpAgentData);

extern int wifi_set_chainmask( char *pcDevName, int iChainMask );
extern DEV_STATE_E         g_enCpeState;
extern UINT32       g_uiOfflineTime;
extern UINT32       g_ulSendHeartTime;
extern UINT32       g_uiSendHeartCount;

#define SNMP_UPDATE_FLAG_DOING 1
#define SNMP_UPDATE_FLAG_WAIT 0
static UINT32 g_ulUpFlag = SNMP_UPDATE_FLAG_WAIT;

#define util_system_cmd(x)		system(x)

BOOL ftp_put(UINT8 *pucIp, UINT32 ulPort, CHAR *pcUser, CHAR *pcPwd, CHAR *pcLocalFile, CHAR *pcRemoteFile)
{
    netbuf *pstFtpH = NULL;
    CHAR aucHost[CHAR_LEN_128]= {0};

    if(pucIp == NULL || pcUser == NULL || pcPwd == NULL || pcLocalFile == NULL || pcRemoteFile == NULL)
    {
        snmp_log(LOG_ERR, "FTP param null\n");
        return FALSE;
    }

    FtpInit();
    
    snprintf(aucHost, CHAR_LEN_128, IP_FMT":%d", IP_ARG(pucIp), ulPort);
    if(1!=FtpConnect(aucHost, &pstFtpH))
    {
        snmp_log(LOG_ERR, "FTP connect %s error\n", aucHost);
        if(pstFtpH != NULL)
        {
            FtpClose(pstFtpH);
            snmp_log(LOG_ERR, "FTP close\n");
        }
        return FALSE;
    }
    snmp_log(LOG_INFO, "FTP connect %s success\n", aucHost);
    
    if(1!=FtpLogin(pcUser, pcPwd, pstFtpH))
    {
        snmp_log(LOG_ERR, "FTP login %s %s error\n", pcUser, pcPwd);
        if(pstFtpH != NULL)
        {
            FtpClose(pstFtpH);
            snmp_log(LOG_ERR, "FTP close\n");
        }
        return FALSE;
    }
    snmp_log(LOG_INFO, "FTP login %s %s success\n", pcUser, pcPwd);

    if(1!=FtpPut(pcLocalFile, pcRemoteFile, FTPLIB_IMAGE, pstFtpH))
    {
        snmp_log(LOG_ERR, "FTP put %s %s error\n", pcLocalFile, pcRemoteFile);
        if(pstFtpH != NULL)
        {
            FtpClose(pstFtpH);
            snmp_log(LOG_ERR, "FTP close\n");
        }
        return FALSE;
    }
    snmp_log(LOG_INFO, "FTP put %s %s success\n", pcLocalFile, pcRemoteFile);

    if(pstFtpH != NULL)
    {
        FtpClose(pstFtpH);
        snmp_log(LOG_INFO, "FTP close\n");
    }
    
    return TRUE;
}

BOOL ftp_get(UINT8 *pucIp, UINT32 ulPort, CHAR *pcUser, CHAR *pcPwd, CHAR *pcLocalFile, CHAR *pcRemoteFile)
{
    netbuf *pstFtpH = NULL;
    CHAR aucHost[CHAR_LEN_128]= {0};

    if(pucIp == NULL || pcUser == NULL || pcPwd == NULL || pcLocalFile == NULL || pcRemoteFile == NULL)
    {
        snmp_log(LOG_ERR, "FTP param null\n");
        return FALSE;
    }

    FtpInit();
    
    snprintf(aucHost, CHAR_LEN_128, IP_FMT":%d", IP_ARG(pucIp), ulPort);
    if(1!=FtpConnect(aucHost, &pstFtpH))
    {
        snmp_log(LOG_ERR, "FTP connect %s error\n", aucHost);
        if(pstFtpH != NULL)
        {
            FtpClose(pstFtpH);
            snmp_log(LOG_ERR, "FTP close\n");
        }
        return FALSE;
    }
    snmp_log(LOG_INFO, "FTP connect %s success\n", aucHost);
    
    if(1!=FtpLogin(pcUser, pcPwd, pstFtpH))
    {
        snmp_log(LOG_ERR, "FTP login %s %s error\n", pcUser, pcPwd);
        if(pstFtpH != NULL)
        {
            FtpClose(pstFtpH);
            snmp_log(LOG_ERR, "FTP close\n");
        }
        return FALSE;
    }
    snmp_log(LOG_INFO, "FTP login %s %s success\n", pcUser, pcPwd);

    if(1!=FtpGet(pcLocalFile, pcRemoteFile, FTPLIB_IMAGE, pstFtpH))
    {
        snmp_log(LOG_ERR, "FTP get %s %s error\n", pcLocalFile, pcRemoteFile);
        if(pstFtpH != NULL)
        {
            FtpClose(pstFtpH);
            snmp_log(LOG_ERR, "FTP close\n");
        }
        return FALSE;
    }
    snmp_log(LOG_INFO, "FTP get %s %s success\n", pcLocalFile, pcRemoteFile);

    if(pstFtpH != NULL)
    {
        FtpClose(pstFtpH);
        snmp_log(LOG_INFO, "FTP close\n");
    }
    
    return TRUE;
}

/*****************************************************************************
 函 数 名  : agent_filename_checkout
 功能描述  : 校验文件名是否正确
 输入参数  : SNMP_UPADTE_FILE_INFO_T *pstFileTrans  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年5月22日
    作    者   : 董鹏
    修改内容   : 新生成函数

*****************************************************************************/
BOOL agent_filename_checkout(SNMP_FILE_TRANS_INFO_T *pstFileTrans)
{
	agent_printf(LOG_INFO,"entering this function");
    return TRUE;
}

/*****************************************************************************
 函 数 名  : agent_file_transmit
 功能描述  : 文件传输
 输入参数  : SNMP_UPADTE_FILE_INFO_T *pstFileTrans  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年5月22日
    作    者   : 董鹏
    修改内容   : 新生成函数

*****************************************************************************/
BOOL agent_file_transmit(SNMP_FILE_TRANS_INFO_T *pstFileTrans)
{
	agent_printf(LOG_INFO,"entering this function");
//    CHAR acCmd[CMD_LEN]={0};

    if(NULL == pstFileTrans)
    {
        agent_printf(LOG_ERR,"pstFileTrans is null");
		return FALSE;
    }
    
    if(SNMP_LOAD_NONE == pstFileTrans->ulLoadFlag ||
        pstFileTrans->ulLoadFlag >= SNMP_LOAD_MAX)
    {
        agent_printf(LOG_ERR,"paramter ulLoadFlag Out of Range");
		return FALSE;
    }

    if(SNMP_FILE_NONE == pstFileTrans->ulFileType ||
        pstFileTrans->ulFileType >= SNMP_FILE_MAX)
    {
        agent_printf(LOG_ERR,"paramter ulFileType Out of Range");
		return FALSE;
    }

    if(SNMP_TRANS_NONE == pstFileTrans->ulTransProto ||
        pstFileTrans->ulTransProto >= SNMP_TRANS_MAX)
    {
        agent_printf(LOG_ERR,"paramter ulFileType Out of Range");
		return FALSE;
    }

    if(SNMP_TRANS_FTP != pstFileTrans->ulTransProto)
    {
        agent_printf(LOG_ERR,"trans proto is not ftp");
		return FALSE;
    }

    if(FTP_PORT != pstFileTrans->ulTransPort)
    {
        agent_printf(LOG_ERR,"ulTransPort is not ftp port");
		return FALSE;
    }
    if(SNMP_LOAD_DOWN == pstFileTrans->ulLoadFlag)
    {
        /*ftpX get host port user pwd remote_file local_file log_file currVer tarVer
        */
//        sprintf(acCmd,"ftpX get %u.%u.%u.%u %u %s %s %s %s %s %s %s",
//            pstFileTrans->aucServerIp[0],pstFileTrans->aucServerIp[1],
//            pstFileTrans->aucServerIp[2],pstFileTrans->aucServerIp[3],
//            pstFileTrans->ulTransPort,pstFileTrans->aucTransUser,pstFileTrans->aucTransPasswd,
//            pstFileTrans->aucImageDir,CONFIG_FILE_DIR,
//            UPDATE_IMAGE_LOG,CURR_VERSION,TARG_VERSION);
       
		return ftp_get(pstFileTrans->aucServerIp, pstFileTrans->ulTransPort, 
                    pstFileTrans->aucTransUser,pstFileTrans->aucTransPasswd,
                    CONFIG_FILE_DIR, pstFileTrans->aucImageDir);
    }
    else
    {
        /*ftpX put host port user pwd local_file remote_file log_file currVer tarVer
        */
//        sprintf(acCmd,"ftpX put %u.%u.%u.%u %u %s %s %s %s %s %s %s",
//           pstFileTrans->aucServerIp[0],pstFileTrans->aucServerIp[1],
//            pstFileTrans->aucServerIp[2],pstFileTrans->aucServerIp[3],
//            pstFileTrans->ulTransPort,pstFileTrans->aucTransUser,pstFileTrans->aucTransPasswd,
//            FILE_TRANS_DIR,pstFileTrans->aucImageDir,
//            UPDATE_IMAGE_LOG,CURR_VERSION,TARG_VERSION);
		return ftp_put(pstFileTrans->aucServerIp, pstFileTrans->ulTransPort, 
                   pstFileTrans->aucTransUser,pstFileTrans->aucTransPasswd,
                    FILE_TRANS_DIR, pstFileTrans->aucImageDir);
    }

//    snmp_log(LOG_ERR, "%s\n", acCmd);
//    if(0 != system(acCmd))
//    {
//        agent_printf(LOG_ERR,"acCmd:%s",acCmd);
//        agent_printf(LOG_ERR,"command excute error");
//		return FALSE;
//    }
//    else
//    {
//        agent_printf(LOG_INFO,"acCmd:%s",acCmd);
//    }
    printf("caca!\n");
    return TRUE;
}

/*****************************************************************************
 函 数 名  : agent_flash_image
 功能描述  : 完成版本的FLASH烧录
 输入参数  : SNMP_UPADTE_FILE_INFO_T *pstFileTrans  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年5月22日
    作    者   : 董鹏
    修改内容   : 新生成函数

*****************************************************************************/
BOOL agent_flash_image(VOID)
{
    CHAR acCmd[128]={0};

    /*版本烧录*/
    sprintf(acCmd,"vflag localupdate %s &",CONFIG_FILE_DIR);
    if(0 != system(acCmd))
    {
        printf("command excute error\n");
		return FALSE;
    }
	printf("[%s|%s:%d] acCmd=%s \n",__FILE__,__FUNCTION__,__LINE__,acCmd);	
    return TRUE;
}

/*****************************************************************************
 函 数 名  : snmp_update_image
 功能描述  : 软件升级
 输入参数  : VOID  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年5月22日
    作    者   : 董鹏
    修改内容   : 新生成函数

*****************************************************************************/
BOOL snmp_update_image(SNMP_AGENT_DATA_T* pstSnmpAgentData)
{
	int transmitCurr=0;
    SNMP_AGENT_DATA_T* pstSnmpData = pstSnmpAgentData;

	pstSnmpData->stOperInfo.ulCpeUpdateState=SNMP_UPDATE_STATE_NONE;

	if(FALSE == agent_filename_checkout(&pstSnmpData->stFileTrans))
	{
        agent_printf(LOG_ERR,"agent_filename_checkout fail");
    	pstSnmpData->stFileTrans.ulTransStatus=SNMP_UPDATE_STATE_FILE_WRONG;
        goto end;
	}
    
	if(FALSE == agent_file_transmit(&pstSnmpData->stFileTrans))
	{
		transmitCurr=1;
        agent_printf(LOG_ERR,"agent_file_transmit fail");
    	pstSnmpData->stFileTrans.ulTransStatus=SNMP_UPDATE_STATE_RECV_ERR;
        goto end;

	}

	if(FALSE == agent_flash_image())
	{
        agent_printf(LOG_ERR,"agent_flash_image fail");
    	pstSnmpData->stFileTrans.ulTransStatus=SNMP_UPDATE_STATE_FLASH_ERR;
        goto end;
	}
	
    /*success*/
	pstSnmpData->stFileTrans.ulTransStatus = SNMP_UPDATE_STATE_SUCCESS;
end:

    #if 0
    /* 发送软件升级失败事件 */
    if (SNMP_UPDATE_STATE_SUCCESS != pstSnmpData->stSwOper.ulCpeUpdateState)
    {
        check_event_swupdatefail(pstSnmpData);
    }
    #endif
    
    /* 发送UPDATE 状态 */

    if (FALSE == snmp_send_update_trap(pstSnmpData))
    {
        agent_printf(LOG_ERR,"pstSnmpData->stFileTrans.ulTransStatus=%d\n", pstSnmpData->stFileTrans.ulTransStatus);
        return FALSE;
    }

	if( 1==transmitCurr )
	{
		return FALSE;
	}

    return TRUE;
}


/*****************************************************************************
 函 数 名  : snmp_state_change
 功能描述  : 状态机切换
 输入参数  : CPE_STATE_E enTarget  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年5月22日
    作    者   : 董鹏
    修改内容   : 新生成函数

*****************************************************************************/
BOOL snmp_state_change(DEV_STATE_E enTarget)
{
    UINT32 ulOldState=DEV_STATE_NONE;
    
    if(enTarget >= DEV_STATE_MAX)
    {
    	snmp_log(LOG_ERR,"enTarget beyond %u",enTarget);
        return FALSE;
    }

    ulOldState=g_enCpeState;
    g_enCpeState=enTarget;

	switch(enTarget)
	{
		case DEV_STATE_NONE:
            //om_rmtmgr_state_offline();
			break;
		case DEV_STATE_RUNNING:
            //om_rmtmgr_state_online();
			break;
		default:
            //om_rmtmgr_state_offline();
			break;
	}
 
    return TRUE;
}

BOOL snmp_info_set(VOID *pData, VOID *pOriData)
{
    SNMP_SYS_MANAGE_INFO_T   *pstData    =(SNMP_SYS_MANAGE_INFO_T *)pData;      //新数据

	if (NULL == pData)
	{
		snmp_log(LOG_ERR, "pData %p\n", pData);
		return FALSE;
	}
    
	snmp_log(LOG_INFO, "pstData->uiManageState=%d\n", pstData->uiManageState);

    snmp_state_change(pstData->uiManageState);

	return TRUE;
}

BOOL sys_debug_info_set(VOID *pData, VOID *pOriData)
{
    SNMP_DEBUG_INFO_T   *pstData    =(SNMP_DEBUG_INFO_T *)pData;      //new data 
    SNMP_DEBUG_INFO_T   *pstOriData =(SNMP_DEBUG_INFO_T *)pOriData;   //ori data
    OM_ERROR_CODE_E enRet;
    CHAR acServerIp[CHAR_LEN_32] = {0};
    CHAR acCmd[CHAR_LEN_128] = {0};
    CHAR acInfo[CHAR_LEN_128] = {0};
    INT32 iRet;

    snmp_log(LOG_NOTICE, "l2tp_rmt_info_set\n");
    if (NULL == pData)
	{
		snmp_log(LOG_ERR, "pData %p\n", pData);
		return FALSE;
	}

	if ((pstOriData->acDebugServerIp[0] != pstData->acDebugServerIp[0]) ||
	    (pstOriData->acDebugServerIp[1] != pstData->acDebugServerIp[1]) ||
	    (pstOriData->acDebugServerIp[2] != pstData->acDebugServerIp[2]) ||
	    (pstOriData->acDebugServerIp[3] != pstData->acDebugServerIp[3])
	    )
	{
	    sprintf(acServerIp, "%d.%d.%d.%d", pstData->acDebugServerIp[0], pstData->acDebugServerIp[1], pstData->acDebugServerIp[2], pstData->acDebugServerIp[3]);

        sprintf(acCmd, "cat /tmp/xl2tpd/xl2tpd.conf | grep lns | cut -d = -f 2");
        snmp_log(LOG_INFO, "snmp acCmd=%s\n", acCmd);

//        enRet = popen_get_info(acCmd, acInfo, sizeof(acCmd));
//        if (OM_E_NONE != enRet)
//        {
//            snmp_log(LOG_ERR, "popen failed:%d",enRet);    
//            return enRet;
//        }

        sprintf(acCmd, "sed -i s/%s/%s/g /tmp/xl2tpd/xl2tpd.conf", acInfo, acServerIp);
        snmp_log(LOG_INFO, "snmp acCmd=%s\n", acCmd);
        
        iRet = util_system_cmd(acCmd);
        if ( 0 != iRet )
        {
            snmp_log(LOG_ERR, "call system failed:%d",iRet);
            return iRet;
        }
    }

    if (!STR_MATCH(pstOriData->auDebugServerUserName, pstData->auDebugServerUserName))
    {
        sprintf(acCmd, "cat /tmp/ppp/options.l2tpd.telnet | grep user | awk \'{print $2}\'");
        snmp_log(LOG_INFO, "snmp acCmd=%s\n", acCmd);
        
//        enRet = popen_get_info(acCmd, acInfo, sizeof(acCmd));
//        if (OM_E_NONE != enRet)
//        {
//            snmp_log(LOG_ERR, "popen failed:%d",iRet);    
//            return enRet;
//        }
        
        sprintf(acCmd, "sed -i 10s/%s/%s/g /tmp/ppp/options.l2tpd.telnet", acInfo, pstData->auDebugServerUserName);
        snmp_log(LOG_INFO, "snmp acCmd=%s\n", acCmd);

        iRet = util_system_cmd(acCmd);
        if ( 0 != iRet )
        {
            snmp_log(LOG_ERR, "call system failed:%d",iRet);
            return iRet;
        }
    }

    
    if (!STR_MATCH(pstOriData->auDebugServerPasswd, pstData->auDebugServerPasswd))
    {
        sprintf(acCmd, "cat /tmp/ppp/options.l2tpd.telnet | grep ^password | awk \'{print $2}\'");
        snmp_log(LOG_INFO, "snmp acCmd=%s\n", acCmd);
        
//        enRet = popen_get_info(acCmd, acInfo, sizeof(acCmd));
//        if (OM_E_NONE != enRet)
//        {
//            snmp_log(LOG_ERR, "popen failed:%d",iRet);    
//            return enRet;
//        }

        sprintf(acCmd, "sed -i 11s/%s/%s/g /tmp/ppp/options.l2tpd.telnet", acInfo, pstData->auDebugServerPasswd);
        snmp_log(LOG_INFO, "snmp acCmd=%s\n", acCmd);

        iRet = util_system_cmd(acCmd);
        if ( 0 != iRet )
        {
            snmp_log(LOG_ERR, "call system failed:%d",iRet);
            return iRet;
        }
    }

    return TRUE;
}

BOOL device_heart_info_set(VOID *pData, VOID *pOriData)
{
    SNMP_DEVICE_HEART_INFO_T   *pstData    =(SNMP_DEVICE_HEART_INFO_T *)pData;      

    snmp_log(LOG_NOTICE, "device_heart_info_set\n");

    if (NULL == pData)
	{
		snmp_log(LOG_ERR, "pData %p\n", pData);
		return FALSE;
	}

    if (pstData->uiTrapInterval)
    {
        g_uiOfflineTime = (pstData->uiTrapInterval)/4;
    }
    
    if (pstData->uiHeartInterval)
    {
        g_ulSendHeartTime = (pstData->uiHeartInterval)/4 + 2;
    }

    return TRUE;
}

BOOL snmp_log_cfg_set(VOID *pData, VOID *pOriData)
{
    SNMP_LOG_CFG_T   *pstData    =(SNMP_LOG_CFG_T *)pData;     
    CHAR acCmd[128]={0};
    INT32 iRet;
    
    if (NULL == pData)
	{
		snmp_log(LOG_ERR, "pData %p\n", pData);
		return FALSE;
	}

    snmp_log(LOG_ERR, "log config state %d\n", pstData->uiCfgState);

    if(SNMP_LOG_CFG_CLEANUP == pstData->uiCfgState)
    {
        snprintf(acCmd, sizeof(acCmd), "echo %s > /var/log/messages.txt", "new log");

        snmp_log(LOG_ERR, "%s\n",acCmd);
        iRet = util_system_cmd(acCmd);
        if ( 0 != iRet )
        {
            snmp_log(LOG_ERR, "system failed:%s\n",acCmd);
            return iRet;
        }
    }
    else if(SNMP_LOG_CFG_GET == pstData->uiCfgState)
    {
        iRet = util_system_cmd("cp /var/log/messages.txt /tmp");
        if ( 0 != iRet )
        {
            snmp_log(LOG_ERR, "copy messages.txt fail\n");
            return iRet;
        }

        iRet = util_system_cmd("chmod 777 /tmp/messages.txt");
        if ( 0 != iRet )
        {
            snmp_log(LOG_ERR, "chmod messages.txt fail\n");
            return iRet;
        }

        g_pstSnmpAgentData->stFileTrans.ulFileType = SNMP_FILE_LOG;
        iRet = snmp_file_transmit(&g_pstSnmpAgentData->stFileTrans);
        if ( FALSE == iRet )
        {
            snmp_log(LOG_ERR, "snmp_file_transmit fail %d\n", iRet);
            return iRet;
        }

        g_pstSnmpAgentData->stFileTrans.ulTransStatus = SNMP_UPDATE_STATE_SUCCESS;
        return TRUE;
    }
    else
    {
        snmp_log(LOG_ERR, "log config state %d\n", pstData->uiCfgState);
        return TRUE;
    }

    return TRUE;
}

BOOL snmp_device_oper_set(VOID *pData, VOID *pOriData)
{
	SNMP_OPER_INFO_T   *pstOriData =(SNMP_OPER_INFO_T *)pOriData;
	SNMP_OPER_INFO_T   *pstData    =(SNMP_OPER_INFO_T *)pData;   
	INT32 iRet;
    BOOL bRet;
    
	
    if (NULL == pData)
	{
		snmp_log(LOG_ERR, "pData %p\n", pData);
		return FALSE;
	}

    if (pstData->ulCpeUpdateState > 0 && pstData->ulCpeUpdateState < 4)
    {
        if(SNMP_UPDATE_FLAG_WAIT == g_ulUpFlag)
        {
            g_ulUpFlag=SNMP_UPDATE_FLAG_DOING;
            bRet = snmp_update_image(g_pstSnmpAgentData);
            if (FALSE == bRet)
            {
                snmp_log(LOG_ERR, "update image failed:%d\n", bRet);
                g_ulUpFlag= SNMP_UPDATE_FLAG_WAIT;
                return bRet;
            }

            if (1 == pstData->ulCpeUpdateState || 3 == pstData->ulCpeUpdateState)
            {
            	
                iRet = system("reboot 250 rebooting by CMS update 1>/dev/null 2>&1 &");
                if (0 != iRet)
                {
                    snmp_log(LOG_ERR, "reboot failed:%d", iRet);
                    g_ulUpFlag= SNMP_UPDATE_FLAG_WAIT;
                    return iRet;
                }
                
            }

        }
        
        return TRUE;

    }

    if(1 == pstData->ulScript)
    {
        bRet = snmp_update_shell_files(g_pstSnmpAgentData);
        if(FALSE == bRet)
        {
            snmp_log(LOG_ERR, "update shell file failed:%d\n",bRet);
            return bRet;
        }

    }

    if (SNMP_DEVICE_REBOOT_RESET == pstData->ulReboot)
    {
        iRet = util_system_cmd("reboot 3 rebooting by CMS update 1>/dev/null 2>&1 &");
        if ( 0 != iRet )
        {
            snmp_log(LOG_ERR, "reboot failed:%d",iRet);
            return iRet;
        }

        return TRUE;

    }
   

	return TRUE;
}

BOOL snmp_file_trans_set(VOID *pData, VOID *pOriData)
{
    snmp_log(LOG_NOTICE, "snmp_file_trans_set\n");
    return TRUE;
}
BOOL snmp_file_transmit(SNMP_FILE_TRANS_INFO_T *pstFileTrans)
{
	agent_printf(LOG_INFO,"entering this function");
    CHAR acFile[CHAR_LEN_128]={0};
    CHAR acLocalFile[CHAR_LEN_128]={0};
    CHAR acRemoteFile[CHAR_LEN_128]={0};

    if(NULL == pstFileTrans)
    {
        snmp_log(LOG_ERR,"pstFileTrans is null");
		return FALSE;
    }
    
    if(SNMP_LOAD_NONE == pstFileTrans->ulLoadFlag ||
        pstFileTrans->ulLoadFlag >= SNMP_LOAD_MAX)
    {
        snmp_log(LOG_ERR,"paramter ulLoadFlag Out of Range");
		return FALSE;
    }

    if(SNMP_TRANS_NONE == pstFileTrans->ulTransProto ||
        pstFileTrans->ulTransProto >= SNMP_TRANS_MAX)
    {
        snmp_log(LOG_ERR,"paramter ulFileType Out of Range");
		return FALSE;
    }

    if(SNMP_TRANS_FTP != pstFileTrans->ulTransProto)
    {
        snmp_log(LOG_ERR,"trans proto is not ftp");
		return FALSE;
    }

    if(SNMP_FILE_SW == pstFileTrans->ulFileType)
    {
        strncpy(acFile, SW_FILE_NAME, CHAR_LEN_128);
        snprintf(acLocalFile, CHAR_LEN_128, "%s%s", SW_FILE_DIR, acFile);
    }
    else if(SNMP_FILE_LOG == pstFileTrans->ulFileType)
    {
        strncpy(acFile, LOG_FILE_NAME, CHAR_LEN_128);
        snprintf(acLocalFile, CHAR_LEN_128, "%s%s", LOG_FILE_DIR, acFile);
    }
    else if(SNMP_FILE_SHELL == pstFileTrans->ulFileType)
    {
        strncpy(acFile, basename(pstFileTrans->aucImageDir), CHAR_LEN_128);
        snprintf(acLocalFile, CHAR_LEN_128, "%s%s", SHELL_FILE_DIR, acFile);
    }
    else
    {
        agent_printf(LOG_ERR,"file type not support %d", pstFileTrans->ulFileType);
		return FALSE;
    }

    snprintf(acRemoteFile, CHAR_LEN_128, "%s", pstFileTrans->aucImageDir);
    if(SNMP_LOAD_DOWN == pstFileTrans->ulLoadFlag)
    {
        
        snmp_log(LOG_ERR, "Local[%s] Remote[%s]",acLocalFile, acRemoteFile);
        return ftp_get(pstFileTrans->aucServerIp, pstFileTrans->ulTransPort, 
                    pstFileTrans->aucTransUser,pstFileTrans->aucTransPasswd,
                    acLocalFile, acRemoteFile);
       
    }
    else
    {
        snmp_log(LOG_ERR, "Local[%s] Remote[%s]",acLocalFile, acRemoteFile);
        return ftp_put(pstFileTrans->aucServerIp, pstFileTrans->ulTransPort, 
                    pstFileTrans->aucTransUser,pstFileTrans->aucTransPasswd,
                    acLocalFile, acRemoteFile);
    }

    return TRUE;
}
BOOL snmp_shell_script_exec(char* pShellScriptPath)
{
 /*   CHAR *pFileName = NULL;
    CHAR acDirFileName[64]={0};
    CHAR acShellCommand[128]={0};
    INT32 iRet;

    if(NULL == pShellScriptPath)
    {
        agent_printf(LOG_ERR,"pShellScriptPath is NULL");
        return FALSE;
    }
    pFileName = basename(pShellScriptPath);
    sprintf(acDirFileName,"%s%s",SHELL_FILE_DIR, pFileName);
    agent_printf(LOG_DEBUG,"acDirFileName is %s",acDirFileName);
    //dos2unix 转换
    memset(acShellCommand,0,128);
    sprintf(acShellCommand,"dos2unix %s",acDirFileName);
    iRet = util_system_cmd(acShellCommand);
    if(iRet != 0)
    {
        snmp_log(LOG_ERR, "run %s fail:%d",acShellCommand,iRet);
        return FALSE;
    }
    
    sprintf(acShellCommand,"chmod +x %s",acDirFileName);
    iRet = util_system_cmd(acShellCommand);
    if(iRet != 0)
    {
        snmp_log(LOG_ERR, "run %s fail:%d",acShellCommand,iRet);
        return FALSE;
    }
    
    memset(acShellCommand,0,128);
    sprintf(acShellCommand,"%s./%s",SHELL_FILE_DIR,pFileName);
    iRet = util_system_cmd(acShellCommand);
    if(iRet != 0)
    {
        snmp_log(LOG_ERR, "run %s fail:%d",acShellCommand,iRet);
        return FALSE;

    }
    */
    return TRUE;
}
BOOL snmp_update_shell_files(SNMP_AGENT_DATA_T* pstSnmpAgentData)
{
    SNMP_AGENT_DATA_T* pstSnmpData = pstSnmpAgentData;

	if(FALSE == agent_filename_checkout(&pstSnmpData->stFileTrans))
	{
        agent_printf(LOG_ERR,"agent_filename_checkout fail");
    	pstSnmpData->stFileTrans.ulTransStatus=SNMP_UPDATE_STATE_FILE_WRONG;
        goto end;
	}
    
	if(FALSE == snmp_file_transmit(&pstSnmpData->stFileTrans))
	{
        agent_printf(LOG_ERR,"agent_file_transmit fail");
    	pstSnmpData->stFileTrans.ulTransStatus=SNMP_UPDATE_STATE_RECV_ERR;
        goto end;

	}

	if(FALSE == snmp_shell_script_exec(pstSnmpData->stFileTrans.aucImageDir))
	{
        agent_printf(LOG_ERR,"snmp_shell_script_exec fail");
    	pstSnmpData->stFileTrans.ulTransStatus=SNMP_UPDATE_STATE_FLASH_ERR;
        goto end;
	}

    /*success*/
	pstSnmpData->stFileTrans.ulTransStatus = SNMP_UPDATE_STATE_SUCCESS;
    return TRUE;
end:
    return FALSE;
    #if 0
    /* 发送软件升级失败事件 */
    if (SNMP_UPDATE_STATE_SUCCESS != pstSnmpData->stSwOper.ulCpeUpdateState)
    {
        check_event_swupdatefail(pstSnmpData);
    }
    #endif
    
    /* 发送UPDATE 状态 */
    #if 0
    if(FALSE == snmp_send_update_trap(pstSnmpData))
    {
        //printf("pstSnmpData->stSwOper.ulCpeUpdateState=%d\n", pstSnmpData->stSwOper.ulCpeUpdateState);
		return FALSE;
    }
    #endif
    
}

#if 0
BOOL snmp_lte_cfg_info_set(VOID * pData, VOID * pOriData)
{
    //SNMP_LTE_CFG_INFO_T  *pstData    =(SNMP_LTE_CFG_INFO_T *)pData;   
    
    snmp_log(LOG_NOTICE, "snmp_lte_cfg_info_set\n");

    if (NULL == pData || NULL == pOriData)
	{
		snmp_log(LOG_ERR, "pData or pOriData is null.\n");
		return FALSE;
	}
    snmp_update_shell_files(g_pstSnmpAgentData);

	
}
#endif


/*begin add by wangqi,2015-3-24*/
BOOL snmp_lte_cfg_info_set(VOID * pData, VOID * pOriData)
{
/* 
    SNMP_LTE_CFG_INFO_T *pstData = NULL;
    SNMP_LTE_CFG_INFO_T *pstOriData = NULL;
//    OM_LTE_ACCESS_MODE_CFG_T stAccessMode;
//    OM_LTE_ACCESS_CFG_T  stAccessCfg;
//    CHAR cNvramValue[NVRAM_VALUE_MAX_LEN] = {0};
    INT32 iRet = 0;
    if (NULL == pData || NULL == pOriData)
	{
		snmp_log(LOG_ERR, "pData or pOriData is null.\n");
		return FALSE;
	}
	pstData    = (SNMP_LTE_CFG_INFO_T *)pData;
	pstOriData = (SNMP_LTE_CFG_INFO_T *)pOriData;
    //printf("0, old==%s, new==%s\n", pstOriData->aucApnSelect, pstData->aucApnSelect);
	if(!STR_MATCH(pstData->aucApnSelect, pstOriData->aucApnSelect))
	{
        //设置APN
        if (LTE_OK != lte_init(4))//g_iLteLogLevel
        {
            snmp_log(LOG_ERR, "lte_init err.\n");
            return FALSE;
        }
        if(LTE_OK != lte_ioctl(LTE_APN_SET, (void *)pstData->aucApnSelect, sizeof(LTE_APN_T)))
        {
            snmp_log(LOG_ERR, "set ApnSelect err.\n");
            return FALSE;
        }
        //设置到nvram字段
        if(STR_MATCH(pstData->aucApnSelect, LTE_APN_CMNET_STR))
        {
            sprintf(cNvramValue, "%d", LTE_APN_CMNET);
        }
        else if(STR_MATCH(pstData->aucApnSelect, LTE_APN_UNINET_STR))
        {
            sprintf(cNvramValue, "%d", LTE_APN_UNINET);
        }
        else if(STR_MATCH(pstData->aucApnSelect, LTE_APN_3GNET_STR))
        {
            sprintf(cNvramValue, "%d", LTE_APN_3GNET);
        }
        else if(STR_MATCH(pstData->aucApnSelect, LTE_APN_CTNET_STR))
        {
            sprintf(cNvramValue, "%d", LTE_APN_CTNET);
        }
        else
        {
            if(AP_E_NONE != nvram_set(NVRAM_LTE_APN_CUSTOM, pstData->aucApnSelect))
            {
                snmp_log(LOG_ERR, "nvram_set error.\n");
                return FALSE;
            } 
            sprintf(cNvramValue, "%d", LTE_APN_SELFDEFINED);
        }
        if(AP_E_NONE != nvram_set(NVRAM_LTE_APN_SELECT, cNvramValue))
        {
            snmp_log(LOG_ERR, "nvram_set error.\n");
            return FALSE;
        } 
        
	}
	//printf("555 old==%d,%d   new==%d, %d\n", pstOriData->uiPriAccessMode, pstOriData->uiAccessMode,
	                                        pstData->uiPriAccessMode, pstData->uiAccessMode);
	if(pstData->uiPriAccessMode != pstOriData->uiPriAccessMode || 
	    pstData->uiAccessMode != pstOriData->uiAccessMode)
	{
        //设置优先模式
        //设置接入模式
        memset(&stAccessCfg, 0, sizeof(stAccessCfg));
        om_lte_net_type_config_get(&stAccessCfg);
        if(stAccessCfg.stNetTypeInfo.enNetType & pstData->uiAccessMode)
        {
            stAccessMode.eAccessMode = (OM_LTE_ACCESS_MODE_E)pstData->uiAccessMode;
        }
        else
        {
            snmp_log(LOG_ERR,"cms set access mode err,the module does not support this access mode,[%d].\n",stAccessCfg.stNetTypeInfo.enNetType);
            return TRUE;
        }
        stAccessMode.ePriAccessMode = (OM_LTE_PRI_ACCESS_MODE_E)pstData->uiPriAccessMode;
        iRet = om_lte_update_access_mode(&stAccessMode);
        if ( iRet != LTE_OK )
        {
            snmp_log(LOG_ERR, "lte access mode set failed,mode:%d,pri mode:%d",
                                    stAccessMode.eAccessMode,
                                    stAccessMode.ePriAccessMode);
            return FALSE;
        }
        memset(cNvramValue,0,sizeof(cNvramValue));
        sprintf(cNvramValue, "%d", pstData->uiPriAccessMode);
        if(AP_E_NONE != nvram_set(NVRAM_LTE_PRIMODE_SELECT, cNvramValue))
        {
            snmp_log(LOG_ERR, "nvram_set error.\n");
            return FALSE;
        }
        memset(cNvramValue,0,sizeof(cNvramValue));
        sprintf(cNvramValue, "%d", pstData->uiAccessMode);
        if(AP_E_NONE != nvram_set(NVRAM_LTE_ACCESS_MODE, cNvramValue))
        {
            snmp_log(LOG_ERR, "nvram_set error.\n");
            return FALSE;
        } 
	}
	if(0 != strcmp((char *)pstData->aucLteBandLocked, (char *)pstOriData->aucLteBandLocked))
	{
        //锁定频段
	}
	if(!STR_MATCH(pstData->aucLtePCIDLocked, pstOriData->aucLtePCIDLocked))
	{
        //锁定物理小区
	}
	if(!STR_MATCH(pstData->aucLteChannelLocked, pstOriData->aucLteChannelLocked))
	{
        //锁定频点
	}
	
	nvram_commit();
	
	*/

	return TRUE;
}

BOOL snmp_lte_station_lock_set(VOID * pData, VOID * pOriData)
{
/*    SNMP_LTE_STATION_LOCK_T *pstData = NULL;
    SNMP_LTE_STATION_LOCK_T *pstOriData = NULL;
    OM_LTE_CELL_CFG_T  stLteCellCfg = {0};
    
    if (NULL == pData || NULL == pOriData)
	{
		snmp_log(LOG_ERR, "pData or pOriData is null.\n");
		return FALSE;
	}
	pstData    = (SNMP_LTE_STATION_LOCK_T *)pData;
	pstOriData = (SNMP_LTE_STATION_LOCK_T *)pOriData;
    /printf("into snmp_lte_station_lock_set new:%d,%d,%d,%d; old:%d,%d,%d,%d\n", pstData->ulLteLockType,
        pstData->ulLteAutoLock, pstData->ulLteAutoMax, pstData->ulLteLockNum, pstOriData->ulLteLockType,
        pstOriData->ulLteAutoLock, pstOriData->ulLteAutoMax, pstOriData->ulLteLockNum);
	if(pstData->ulLteLockType != pstOriData->ulLteLockType || pstData->ulLteAutoLock != pstOriData->ulLteAutoLock
	        || pstData->ulLteAutoMax != pstOriData->ulLteAutoMax)
	{
        stLteCellCfg.eCellSw = pstData->ulLteLockType;
        switch(stLteCellCfg.eCellSw)
        {
            case LTE_CELL_SW_OFF:
                stLteCellCfg.eOperateFlag = LTE_CELL_SW_CFG; 
                break;
            case LTE_CELL_SW_CELL:
                stLteCellCfg.eOperateFlag = LTE_CELL_AUTO_LOCK_CFG;
                stLteCellCfg.uiCellAutoLockSw = pstData->ulLteAutoLock;
                stLteCellCfg.uiCellAutoLockNumMax = pstData->ulLteAutoMax;
                break;
            case LTE_CELL_SW_ENODEB:
                stLteCellCfg.eOperateFlag = LTE_ENB_AUTO_LOCK_CFG;
                stLteCellCfg.uiEnbIdAutoLockSw = pstData->ulLteAutoLock;
                stLteCellCfg.uiEnbIdAutoLockNumMax = pstData->ulLteAutoMax;
                break;
            case LTE_CELL_SW_TAC:
                stLteCellCfg.eOperateFlag = LTE_TAC_AUTO_LOCK_CFG;
                stLteCellCfg.uiTacAutoLockSw = pstData->ulLteAutoLock;
                stLteCellCfg.uiTacAutoLockNumMax= pstData->ulLteAutoMax;
                break;
            default:
                snmp_log(LOG_ERR, "pstData->ulLteLockType error.\n");
                stLteCellCfg.eCellSw = LTE_CELL_SW_OFF;
                stLteCellCfg.eOperateFlag = LTE_CELL_SW_CFG;
        }
        /printf("snmp_lte_station_lock_set 00  eCellSw==%d, eOperateFlag==%d, uiTacAutoLockNumMax==%d\n", 
                    stLteCellCfg.eCellSw, stLteCellCfg.eOperateFlag, stLteCellCfg.uiTacAutoLockNumMax);
        if ( 0 != om_lte_cell_config_set(&stLteCellCfg))
        {
            snmp_log(LOG_ERR, "get lte cell config set failed");
            return FALSE;
        }
    }
    */
	return TRUE;
}


/*************************************************************************
功能:根据ID名获取ID编号，并删除(pcNewValue==0)或修改(pcNewValue!=NULL)
*************************************************************************/
BOOL nv_value_delOrChange(const char *listName, const char *pcIdList, char *idName, char *pcNewValue, UINT32 idNum)
{

/*    CHAR cNvramName[NVRAM_PARAM_MAX_LEN] = {0};
    CHAR cNvramValue[NVRAM_VALUE_MAX_LEN] = {0};
    CHAR acList[CHAR_LEN_256] = {0};
    if(NULL == listName || NULL == pcIdList || NULL == idName)
    {
        snmp_log(LOG_ERR, "listName or idName is NULL.\n");
        return FALSE;
    }
    if(AP_E_NONE != nvram_get(acList, listName, sizeof(acList)))
    {
        OM_LOG(LOG_ERR, "nvram get NVRAM_LTE_LOGIC_CELL_LIST failed");
        return FALSE;
    }
    memset(cNvramName, 0, NVRAM_PARAM_MAX_LEN);
    memset(cNvramValue, 0, NVRAM_VALUE_MAX_LEN);
    sprintf(cNvramName, "%s%d", pcIdList, idNum);
    if(AP_E_NONE != nvram_get(cNvramValue,cNvramName, NVRAM_VALUE_MAX_LEN))
    {
        snmp_log(LOG_ERR, "nvram get NVRAM_LTE_LOGIC_CELL_LIST failed");
        return FALSE;
    }
    //printf("%s, %s , %s\n", cNvramValue, idName, pcNewValue);
    if(0 == strcmp(cNvramValue, idName))
    {
        if(pcNewValue == NULL)//删除
        {
            if(AP_E_NONE != nvram_unset(cNvramName))
            {
                snmp_log(LOG_ERR, "nvram unset cell id failed");
                return FALSE;
            }
            util_array_del_someone(acList,idNum);

            if(AP_E_NONE != nvram_set(listName, acList))
            {
                snmp_log(LOG_ERR, "nvram set NVRAM_LTE_LOGIC_CELL_LIST failed");
                return FALSE;
            }
        }
        else
        {
            if(AP_E_NONE != nvram_set(cNvramName, pcNewValue))
            {
                snmp_log(LOG_ERR, "nvram set NVRAM_LTE_LOGIC_CELL_LIST failed");
                return FALSE;
            }
        }
        nvram_commit();
        return TRUE;
    }
    snmp_log(LOG_ERR, "Don't find this ID.\n");
    
    */
    return FALSE;
}

#if 0
BOOL nv_value_delOrChange(const char *listName, const char *pcIdList, char *idName, char *pcNewValue)
{
    CHAR cNvramName[NVRAM_PARAM_MAX_LEN] = {0};
    CHAR cNvramValue[NVRAM_VALUE_MAX_LEN] = {0};
    CHAR acList[CHAR_LEN_256] = {0};
    CHAR *p = acList;
    UINT32 idNum = 0;
    if(NULL == listName || NULL == pcIdList || NULL == idName)
    {
        snmp_log(LOG_ERR, "listName or idName is NULL.\n");
        return FALSE;
    }
    if(AP_E_NONE != nvram_get(acList, listName, sizeof(acList)))
    {
        OM_LOG(LOG_ERR, "nvram get NVRAM_LTE_LOGIC_CELL_LIST failed");
        return FALSE;
    }
    while(*p != '\0')
    {
        idNum = 0;
        while(*p == ',' || *p == ' ')
        {
            ++p;
        }
        while(*p != ',' && *p != '\0' && *p != ' ')
        {
            idNum = idNum*10+*p-'0';
            ++p;
        }
        memset(cNvramName, 0, NVRAM_PARAM_MAX_LEN);
        memset(cNvramValue, 0, NVRAM_VALUE_MAX_LEN);
        sprintf(cNvramName, "%s%d", pcIdList, idNum);
        if(AP_E_NONE != nvram_get(cNvramValue,cNvramName, NVRAM_VALUE_MAX_LEN))
        {
            snmp_log(LOG_ERR, "nvram get NVRAM_LTE_LOGIC_CELL_LIST failed");
            return FALSE;
        }
        //printf("%s, %s , %s\n", cNvramValue, idName, pcNewValue);
        if(0 == strcmp(cNvramValue, idName))
        {
            if(pcNewValue == NULL)//删除
            {
                if(AP_E_NONE != nvram_unset(cNvramName))
                {
                    snmp_log(LOG_ERR, "nvram unset cell id failed");
                    return FALSE;
                }
                util_array_del_someone(acList,idNum);

                if(AP_E_NONE != nvram_set(listName, acList))
                {
                    snmp_log(LOG_ERR, "nvram set NVRAM_LTE_LOGIC_CELL_LIST failed");
                    return FALSE;
                }
            }
            else
            {
                if(AP_E_NONE != nvram_set(cNvramName, pcNewValue))
                {
                    snmp_log(LOG_ERR, "nvram set NVRAM_LTE_LOGIC_CELL_LIST failed");
                    return FALSE;
                }
            }
            nvram_commit();
            return TRUE;
        }
    }
    snmp_log(LOG_ERR, "Don't find this ID.\n");
    return FALSE;
}
#endif

BOOL snmp_lte_station_lock_table_set(VOID * pData, VOID * pOriData)
{/*
    SNMP_LTE_STATION_LOCK_TABLE_T *pstData = NULL;
    SNMP_LTE_STATION_LOCK_TABLE_T *pstOriData = NULL;
    SNMP_LTE_STATION_LOCK_TABLE_T *pstConfig  = g_pstSnmpAgentData->stLteStationLockTable;
    OM_LTE_CELL_SW_E eCellSw;
    OM_LTE_CELL_CFG_T  stLteCellCfg = {0};
    UINT32 ulLoopNo = 0;
    CHAR acTemp[NVRAM_VALUE_MAX_LEN] = {0};
    UINT32 ulNum = 0;
    printf("0  into snmp_lte_station_lock_table_set\n");
    if (NULL == pData)
	{
		snmp_log(LOG_ERR, "pData is null.\n");
		return FALSE;
	}
	//printf("1  into snmp_lte_station_lock_table_set\n");
	pstData = (SNMP_LTE_STATION_LOCK_TABLE_T *)pData;
	if(AP_E_NONE != nvram_get(acTemp,NVRAM_LTE_LOGIC_CELL_SW, NVRAM_VALUE_MAX_LEN))
    {
        snmp_log(LOG_ERR, "nvram get NVRAM_LTE_LOGIC_CELL_LIST failed");
    }

    eCellSw = atoi(acTemp);
    //printf("2  into snmp_lte_station_lock_table_set == %d\n", eCellSw);
	if (NULL == pOriData)
	{//修改或删除记录
		snmp_log(LOG_DEBUG, "change table.\n");
		//printf("3  into snmp_lte_station_lock_table_set\n");
		for(ulLoopNo = 0; ulLoopNo < TABLE_MAXRECNUM_LTE_LOCK_TABLE; ulLoopNo++)
		{
            if(0 == strcmp(pstData[ulLoopNo].ulLteLockID, "") && 
                0 == strcmp(pstConfig[ulLoopNo].ulLteLockID, ""))
            {
                continue;
            }
            if(pstData[ulLoopNo].ulLteLockCfgState == ENUM_ROWSTATUS_DESTROY && 
                pstConfig[ulLoopNo].ulLteLockCfgState == ENUM_ROWSTATUS_DESTROY)
            {
                continue;
            }
            if(pstData[ulLoopNo].ulLteLockNumID != pstConfig[ulLoopNo].ulLteLockNumID)
            {
                continue;
            }
            //printf("4  OLD==%s NEW==%s \n", pstConfig[ulLoopNo].ulLteLockID, pstData[ulLoopNo].ulLteLockID);
            if(0 != strcmp(pstData[ulLoopNo].ulLteLockID, pstConfig[ulLoopNo].ulLteLockID))
            {
                ulNum = pstData[ulLoopNo].ulLteLockNumID-1;
                if(LTE_CELL_SW_CELL == eCellSw)
                {
                    nv_value_delOrChange(NVRAM_LTE_LOGIC_CELL_LIST, NVRAM_LTE_LOGIC_CELL_ID_, pstConfig[ulLoopNo].ulLteLockID, pstData[ulLoopNo].ulLteLockID, ulNum);                    
                }
                else if (LTE_CELL_SW_ENODEB == eCellSw)
                {
                    nv_value_delOrChange(NVRAM_LTE_ENODEB_LIST, NVRAM_LTE_ENODEB_ID_, pstConfig[ulLoopNo].ulLteLockID, pstData[ulLoopNo].ulLteLockID, ulNum);
                }
                else if (LTE_CELL_SW_TAC == eCellSw )
                {
                    nv_value_delOrChange(NVRAM_LTE_TAC_LIST, NVRAM_LTE_TAC_, pstConfig[ulLoopNo].ulLteLockID, pstData[ulLoopNo].ulLteLockID, ulNum);
                }
            }
            //printf("5  OLD==%d NEW==%d \n", pstConfig[ulLoopNo].ulLteLockCfgState, pstData[ulLoopNo].ulLteLockCfgState);
            if(pstData[ulLoopNo].ulLteLockCfgState != pstConfig[ulLoopNo].ulLteLockCfgState)
            {
                ulNum = pstData[ulLoopNo].ulLteLockNumID-1;
                if(pstData[ulLoopNo].ulLteLockCfgState == ENUM_ROWSTATUS_DESTROY)
                {
                    if(LTE_CELL_SW_CELL == eCellSw)
                    {
                        nv_value_delOrChange(NVRAM_LTE_LOGIC_CELL_LIST, NVRAM_LTE_LOGIC_CELL_ID_, pstData[ulLoopNo].ulLteLockID, NULL, ulNum);
                    }
                    else if (LTE_CELL_SW_ENODEB == eCellSw)
                    {
                        nv_value_delOrChange(NVRAM_LTE_ENODEB_LIST, NVRAM_LTE_ENODEB_ID_, pstData[ulLoopNo].ulLteLockID, NULL, ulNum);                       
                    }
                    else if (LTE_CELL_SW_TAC == eCellSw )
                    {
                        nv_value_delOrChange(NVRAM_LTE_TAC_LIST, NVRAM_LTE_TAC_, pstData[ulLoopNo].ulLteLockID, NULL, ulNum);                   
                    }
                }
            }
            
		}
		return TRUE;
	}
	else
	{//添加
	    snmp_log(LOG_DEBUG, "add table.\n");
        pstOriData = (SNMP_LTE_STATION_LOCK_TABLE_T *)pOriData;
        stLteCellCfg.eCellSw = eCellSw;
        stLteCellCfg.eOperateFlag = LTE_CELL_ADD;
        //printf("6  add id==%s\n", pstData->ulLteLockID);
        if(LTE_CELL_SW_CELL == eCellSw)
        {
            memcpy(stLteCellCfg.szNewCellId, pstData->ulLteLockID, CHAR_LEN_32);
        }
        else if (LTE_CELL_SW_ENODEB == eCellSw)
        {
            memcpy(stLteCellCfg.szNewEnodebId, pstData->ulLteLockID, CHAR_LEN_32);
        }
        else if (LTE_CELL_SW_TAC == eCellSw )
        {
           memcpy(stLteCellCfg.szNewTac, pstData->ulLteLockID, CHAR_LEN_32);
        }
        else
        {
            return FALSE;
        }
        if ( 0 != om_lte_cell_config_set(&stLteCellCfg))
        {
            snmp_log(LOG_ERR, "get lte cell config set failed");
            return FALSE;
        }
        return TRUE;
	}

*/

}

BOOL snmp_keepalive_cfg_set(VOID * pData, VOID * pOriData)
{
/*    SNMP_LTEKP_CFG_T *pstData = NULL;
    SNMP_LTEKP_CFG_T *pstOriData = NULL;
    CHAR cNvramValue[NVRAM_VALUE_MAX_LEN] = {0};
    INT32 iRet = 0, stateChange = 0;
    if (NULL == pData || NULL == pOriData)
	{
		snmp_log(LOG_ERR, "pData or pOriData is null.\n");
		return FALSE;
	}
	pstData    = (SNMP_LTEKP_CFG_T *)pData;
	pstOriData = (SNMP_LTEKP_CFG_T *)pOriData;
	if(pstData->uiSwitch != pstOriData->uiSwitch)
	{
        //保活开关
        sprintf(cNvramValue, "%d", pstData->uiSwitch);
        if(AP_E_NONE != nvram_set(NVRAM_LTE_CHECKALIVE_SW, cNvramValue))
        {
            snmp_log(LOG_ERR, "nvram_set error.\n");
            return FALSE;
        }
        stateChange = 1;
	}
	if(pstData->uiInterval != pstOriData->uiInterval)
    {
        //保活周期
        sprintf(cNvramValue, "%d", pstData->uiInterval);
        if(AP_E_NONE != nvram_set(NVRAM_LTE_CHECKALIVE_INTERVAL, cNvramValue))
        {
            snmp_log(LOG_ERR, "nvram_set error.\n");
            return FALSE;
        }
        stateChange = 1;
    }
	nvram_commit();
	if(1 == stateChange)
	{
    	iRet = util_system_cmd("killall -9 checkalive");
        if ( AP_E_NONE != iRet)
        {
            OM_LOG(LOG_ERR, "killall checkalive failed:%d, reboot take effect",iRet);
        }
    }
    
    */
	return TRUE;
}

BOOL snmp_keepalive_info_set(VOID * pData, VOID * pOriData)
{
/*    SNMP_LTEKP_INFO_T *pstData = NULL;
    SNMP_LTEKP_INFO_T *pstOriData = NULL;
    SNMP_LTEKP_INFO_T *pstConfig  = g_pstSnmpAgentData->stLteKPInfo;
    CHAR cNvramValue[NVRAM_VALUE_MAX_LEN] = {0};
    INT32 iRet = 0, ulLoopNo = 0;
    CHAR acTmp[128] = {0};
    //printf("000000000\n");
    if (NULL == pData )
	{
		snmp_log(LOG_ERR, "pData is null.\n");
		return FALSE;
	}
	//printf("1111111111\n");
	pstData    = (SNMP_LTEKP_INFO_T *)pData;
    if(NULL == pstOriData)
    {//修改或删除记录
        for(ulLoopNo = 0; ulLoopNo < TABLE_MAXRECNUM_LTE_KEEPALIVE; ulLoopNo++)
        {
            if(pstData[ulLoopNo].uiCfgState == ENUM_ROWSTATUS_DESTROY &&
                pstConfig[ulLoopNo].uiCfgState == ENUM_ROWSTATUS_DESTROY)
            {
                continue;
            }
            //printf("new ip==%d,old ip==%d\n", *((int *)pstData[ulLoopNo].aucIp), *((int *)pstConfig[ulLoopNo].aucIp));
        	if (0 != strncmp(pstData[ulLoopNo].aucIp,pstConfig[ulLoopNo].aucIp,4) )
            {
                sprintf(cNvramValue, IP_FMT,IP_ARG(pstData[ulLoopNo].aucIp));
                //printf("set ip==%s\n", cNvramValue);
                if(AP_E_NONE != nvram_set(NVRAM_LTE_CHECKALIVE_IP, cNvramValue))
                {
                    snmp_log(LOG_ERR, "nvram_set error.\n");
                    return FALSE;
                }
                
                iRet = util_system_cmd("killall -9 checkalive");
                if ( AP_E_NONE != iRet)
                {
                    OM_LOG(LOG_ERR, "killall checkalive failed:%d",iRet);
                }
                memset(acTmp,0,sizeof(acTmp));
                sprintf(acTmp,IP_FMT,IP_ARG(pstConfig[ulLoopNo].aucIp));
                iRet = om_net_whitelist_ip_del(acTmp);
            
                memset(acTmp,0,sizeof(acTmp));
                sprintf(acTmp,IP_FMT,IP_ARG(pstData[ulLoopNo].aucIp));
                iRet = om_net_whitelist_ip_add(acTmp);
                if ( OM_E_NONE != iRet)
                {
                    snmp_log(LOG_ERR, "check alilve default ip white list add failed");
                }

            }     
            if(pstData[ulLoopNo].uiCfgState != pstConfig[ulLoopNo].uiCfgState)
            {
                if(pstData[ulLoopNo].uiCfgState == ENUM_ROWSTATUS_DESTROY)
                {
                    sprintf(cNvramValue, "0.0.0.0");
                    if(AP_E_NONE != nvram_set(NVRAM_LTE_CHECKALIVE_IP, cNvramValue))
                    {
                        snmp_log(LOG_ERR, "nvram_set error.\n");
                        return FALSE;
                    }
                    iRet = util_system_cmd("killall -9 checkalive");
                    if ( AP_E_NONE != iRet)
                    {
                        OM_LOG(LOG_ERR, "killall checkalive failed:%d",iRet);
                    }
                    memset(acTmp, 0, sizeof(acTmp));
                    sprintf(acTmp, IP_FMT, IP_ARG(pstConfig[ulLoopNo].aucIp));
                    iRet = om_net_whitelist_ip_del(acTmp);
                }
            }
        }    
    }
    else
    {
        //添加
        //printf("33333333333\n");
        pstOriData = (SNMP_LTEKP_INFO_T *)pOriData;

        sprintf(cNvramValue, IP_FMT,IP_ARG(pstData->aucIp));
        if(AP_E_NONE != nvram_set(NVRAM_LTE_CHECKALIVE_IP, cNvramValue))
        {
            snmp_log(LOG_ERR, "nvram_set error.\n");
            return FALSE;
        }
        
        iRet = util_system_cmd("killall -9 checkalive");
        if ( AP_E_NONE != iRet)
        {
            OM_LOG(LOG_ERR, "killall checkalive failed:%d",iRet);
        }
        memset(acTmp,0,sizeof(acTmp));
        sprintf(acTmp,IP_FMT,IP_ARG(pstOriData->aucIp));
        iRet = om_net_whitelist_ip_del(acTmp);
    
        memset(acTmp,0,sizeof(acTmp));
        sprintf(acTmp,IP_FMT,IP_ARG(pstData->aucIp));
        iRet = om_net_whitelist_ip_add(acTmp);
        if ( OM_E_NONE != iRet)
        {
            snmp_log(LOG_ERR, "check alilve default ip white list add failed");
        }

    }

    
*/
	return TRUE;
}


/*end add by wangqi,2015-3-24*/


