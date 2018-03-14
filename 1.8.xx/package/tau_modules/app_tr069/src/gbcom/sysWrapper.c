/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : sysWrapper.c
  版 本 号   : 初稿
  作    者   : zhangjiasi
  生成日期   : 2015年6月9日
  最近修改   :
  功能描述   : 所有与系统相关，当又不好区分模块的接口定义
  函数列表   :
  修改历史   :
  1.日    期   : 2015年6月9日
    作    者   : zhangjiasi
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include "../SOAPParser/RPCState.h"
#include "ap_types.h"
#include "sysWrapper.h"
#include "deviceInfoParams.h"
#include "utils.h"
#include "informer.h"
#include "om_tr69c.h"
#include "sysWrapper.h"
/*Add by zhanghao 获取配置文件内容 2016/6/15 begin*/
#include "uci_config.h"
#include "tr69_lua.h"
/*Add by zhanghao 获取配置文件内容 end*/

//#include "pal/app/pal_app.h"
#define TAUARRY_LEN 128

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
extern int g_log_level;
extern LTEInfoState lteinfostate;

extern TRxObjNode  deviceInfoDesc[];
extern TRxObjNode  x_deviceInfoDesc[]; 
extern TRxObjNode  DeviceInfoX_CMCC_TeleComAccountDesc[];
extern TRxObjNode  DeviceInfoX_CMCC_MonitorDesc[];
extern TRxObjNode  DeviceInfoX_CMCC_MonitorMonitorConfig[];
extern TRxObjNode  DeviceInfoX_CMCC_MonitorMonitorConfigDesc[];

/*add by wangminglin*/
extern TRxObjNode DeviceConfigDesc[];
extern TRxObjNode X_ATP_FireWallDesc[];
extern TRxObjNode FilterModeDescObj[];
extern TRxObjNode UrlBlackFilterDescObj[];
extern TRxObjNode MacBlackFilterDescObj[];
extern TRxObjNode IpBlackFilterDescObj[];
extern TRxObjNode UrlWhiteFilterDescObj[];
extern TRxObjNode MacWhiteFilterDescObj[];
extern TRxObjNode IpWhiteFilterDescObj[];
extern TRxObjNode AccessControlDescObj[];
extern TRxObjNode X_ATP_SystemMonitorDesc[];
extern TRxObjNode GatewayUserInterfaceDesc[];
extern TRxObjNode X_ATP_WebDesc[];
extern TRxObjNode X_ATP_WebUserInfoDescObj[];

extern TRxObjNode X_ATP_PreserveDataDesc[];
extern TRxObjNode X_ATP_EXTDesc[];
extern TRxObjNode Layer3ForwardingDesc[];
extern TRxObjNode ForwardingDescObj[];

extern TRxObjNode X_ATP_ModemLogDesc[];

extern TRxObjNode DeviceInfoX_ATP_InformationsInstanceDesc[];
extern TRxObjNode FreqLockDesc[];
extern TRxObjNode FreqLDataDesc[];
extern TRxObjNode X_ATP_VxlanDesc[];
extern TRxObjNode VlanMapVniListDesc[];
extern TRxObjNode VlanMapDscpListDesc[];
extern TRxObjNode VXLAN_LACDesc[];
extern TRxObjNode X_ATP_L2GreDesc[];
extern TRxObjNode IpMapDscpListDesc[];
extern TRxObjNode L2GRE_LACDesc[];
extern TRxObjNode IpMapVlanPrioListDesc[];
extern TRxObjNode VlanSettingsDesc[];
extern TRxObjNode VlanPrioMapDscpListDesc[];
extern TRxObjNode VlanListDesc[];
extern TRxObjNode LanCheckDesc[];
extern TRxObjNode WanCheckDesc[];
extern TRxObjNode SysMonitorDesc[];
extern TRxObjNode IpFilterDesc[];
extern TRxObjNode MacFilterDesc[];
extern TRxObjNode UrlFilterDesc[];
extern TRxObjNode SwitchDesc[];
extern TRxObjNode X_ATP_VPNDesc[];
extern TRxObjNode L2TP_LACDesc[];
extern TRxObjNode X_ATP_AlarmInfoDesc[];
extern TRxObjNode AlarmInstanceDesc[];
extern TRxObjNode X_ATP_ConfigDesc[];
extern TRxObjNode dialupDesc[];
extern TRxObjNode profileDesc[];
extern TRxObjNode ServicesDesc[];
extern TRxObjNode VoiceServiceDesc[];
extern TRxObjNode VoiceProfileDesc[];
extern TRxObjNode LineDesc[];
extern TRxObjNode ProfileSIPDesc[];
extern TRxObjNode CodecDesc[];
extern TRxObjNode ListDesc[];
extern TRxObjNode SIPDesc[];
extern TRxObjNode X_ATP_CommonConfigDesc[];
extern TRxObjNode ButtonDesc[];
/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
extern int g_debug;  

extern ACSState acsState;
extern int protect_connect_state_flag;
time_t startTime;

eACSContactedState acsContactedState;

int lastInstanceId;

int uiCFMfd = 0;       /* 用于侦听CGI的消息 */
unsigned int uiAcsIP = 0;       /* ACS服务器IP */
unsigned int uiInformState = 0; /* 主动上报Inform情况 */
unsigned int uiLastCfgState = 0;/* 最近远程业务配置情况 0默认配置成功 */
/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

int getRAMSize(void)
{
	return 230000000 ;
}

/*
* Save the TR69 state values across the reboot
*/
void  saveTR69StatusItems( ACSState *a )
{
    /* OM_TR69C_APP_PARAM_T stTr69cAcsInfo = {0};
    UINT32 iLoop;
    
    //memset(&stTr69cAcsInfo, 0, sizeof(OM_TR69C_APP_PARAM_T));

    for (iLoop = TR069_NEW_PARAMETERKEY; iLoop <= TR069_ACS_CONNECT_STATE; ++iLoop)
    {
        stTr69cAcsInfo.eSetFlag = iLoop;
        if(OM_E_NONE != om_tr69c_get_acsinfo(&stTr69cAcsInfo))
        {
            slog(TR69C_LOG_ERR,"getAcsInfo failed");
            return;
        }
    }
 
       // fprintf(stderr, "Save TR69 State Variables to nvram \n");
		
		//fprintf(stderr, "downloadCommandKey=%s\n", a->downloadCommandKey);
		if((NULL != a->downloadCommandKey && strcmp(a->downloadCommandKey, stTr69cAcsInfo.downloadCommandKey)))
		{ 
			strncpy(stTr69cAcsInfo.downloadCommandKey, a->downloadCommandKey, sizeof(stTr69cAcsInfo.downloadCommandKey));
			stTr69cAcsInfo.eSetFlag = TR069_DOWNLOAD_CMD_KEY;
        	if(OM_E_NONE != om_tr69c_set_acsinfo(&stTr69cAcsInfo))
       	 	{
            	slog(TR69C_LOG_ERR,"set nvram TR069_DOWNLOAD_CMD_KEY ERROR");
        	}

		}
       // fprintf(stderr, "rebootCommandKey = %s\n", a->rebootCommandKey);
		if((NULL != a->rebootCommandKey && strcmp(a->rebootCommandKey, stTr69cAcsInfo.rebootCommandKey)))
		{
			strncpy(stTr69cAcsInfo.rebootCommandKey, a->rebootCommandKey, sizeof(stTr69cAcsInfo.rebootCommandKey));
			stTr69cAcsInfo.eSetFlag = TR069_REBOOT_CMD_KEY;
        	if(OM_E_NONE != om_tr69c_set_acsinfo(&stTr69cAcsInfo))
       	 	{
            	slog(TR69C_LOG_ERR,"set nvram TR069_REBOOT_CMD_KEY ERROR");
        	}
		}

	//	fprintf(stderr, "ParameterKey=%s\n", a->parameterKey);  
	//	if((NULL == a->newParameterKey || !strcmp(a->newParameterKey, stTr69cAcsInfo.newParameterKey)))
	//	{

	//	}

		//fprintf(stderr, "dlFaultMsg=%s\n", a->dlFaultMsg);
		if((NULL != a->dlFaultMsg && strcmp(a->dlFaultMsg, stTr69cAcsInfo.dlFaultMsg)))
		{
			strncpy(stTr69cAcsInfo.dlFaultMsg, a->dlFaultMsg, sizeof(stTr69cAcsInfo.dlFaultMsg));
			stTr69cAcsInfo.eSetFlag = TR069_DOWNLOAD_FAULT_MSG;
        	if(OM_E_NONE != om_tr69c_set_acsinfo(&stTr69cAcsInfo))
       	 	{
            	slog(TR69C_LOG_ERR,"set nvram TR069_DOWNLOAD_FAULT_MSG ERROR");
        	}

		}
		//fprintf(stderr, "dlFaultStatus = %d\n", a->dlFaultStatus);
		if(a->dlFaultStatus != stTr69cAcsInfo.dlFaultStatus)
		{
		 	
    		stTr69cAcsInfo.dlFaultStatus = a->dlFaultStatus;
			stTr69cAcsInfo.eSetFlag = TR069_DOWNLOAD_FAULT_CODE;
        	if(OM_E_NONE != om_tr69c_set_acsinfo(&stTr69cAcsInfo))
       	 	{
            	slog(TR69C_LOG_ERR,"set nvram TR069_DOWNLOAD_FAULT_CODE ERROR");
        	}
		}
		//fprintf(stderr, "startDLTime = %ld\n", a->startDLTime);
		if(a->startDLTime != stTr69cAcsInfo.startDLTime)
		{
		 	
    		stTr69cAcsInfo.startDLTime = a->startDLTime;
			stTr69cAcsInfo.eSetFlag = TR069_DOWNLOAD_START_TIME;
        	if(OM_E_NONE != om_tr69c_set_acsinfo(&stTr69cAcsInfo))
       	 	{
            	slog(TR69C_LOG_ERR,"set nvram TR069_DOWNLOAD_START_TIME ERROR");
        	}
		}
		//fprintf(stderr, "endDLTime = %ld\n", a->endDLTime);
     	if(a->endDLTime != stTr69cAcsInfo.endDLTime)
		{
		 	
    		stTr69cAcsInfo.endDLTime = a->endDLTime;
			stTr69cAcsInfo.eSetFlag = TR069_DOWNLOAD_END_TIME;
        	if(OM_E_NONE != om_tr69c_set_acsinfo(&stTr69cAcsInfo))
       	 	{
            	slog(TR69C_LOG_ERR,"set nvram TR069_DOWNLOAD_END_TIME ERROR");
        	}
		}

		//fprintf(stderr, "upgradesManaged = %d\n",acsState.upgradesManaged);
		if(a->upgradesManaged != stTr69cAcsInfo.upgradesManaged)
		{
		 	
    		stTr69cAcsInfo.upgradesManaged = acsState.upgradesManaged;
			stTr69cAcsInfo.eSetFlag = TR069_UPGRADE_ENABLE;
        	if(OM_E_NONE != om_tr69c_set_acsinfo(&stTr69cAcsInfo))
       	 	{
            	slog(TR69C_LOG_ERR,"set nvram TR069_UPGRADE_ENABLE ERROR");
        	}
		}
  	 	//fprintf(stderr, "ACS Contacted State=%d\n", acsContactedState);
		//fprintf(stderr, "ACS protect_connect_state_flag State=%d\n", protect_connect_state_flag);
  	 	if(protect_connect_state_flag <1)
  	 	{
		if (acsContactedState != stTr69cAcsInfo.contactedState)
		{
			stTr69cAcsInfo.contactedState = acsContactedState;
			stTr69cAcsInfo.eSetFlag = TR069_ACS_CONNECT_STATE;
        	if(OM_E_NONE != om_tr69c_set_acsinfo(&stTr69cAcsInfo))
       	 	{
            	slog(TR69C_LOG_ERR,"set nvram TR069_ACS_CONNECT_STATE ERROR");
        	}
		}
  	 	}
	#if 0
    if ((acsContactedState == stTr69cAcsInfo.contactedState) \
        && (a->dlFaultStatus == stTr69cAcsInfo.dlFaultStatus)\
        && (a->startDLTime == stTr69cAcsInfo.startDLTime)\
        && (a->endDLTime == stTr69cAcsInfo.endDLTime)\
        && (NULL == a->dlFaultMsg || !strcmp(a->dlFaultMsg, stTr69cAcsInfo.dlFaultMsg))\
        && (NULL == a->downloadCommandKey || !strcmp(a->downloadCommandKey, stTr69cAcsInfo.downloadCommandKey))\
        && (NULL == a->rebootCommandKey || !strcmp(a->rebootCommandKey, stTr69cAcsInfo.rebootCommandKey))\
		&& (NULL == a->newParameterKey || !strcmp(a->newParameterKey, stTr69cAcsInfo.newParameterKey)))
        //&& !strcmp(a->downloadCommandKey, stTr69cAcsInfo.downloadCommandKey)\
       // && !strcmp(a->rebootCommandKey, stTr69cAcsInfo.rebootCommandKey)\
       // && !strcmp(a->newParameterKey, stTr69cAcsInfo.newParameterKey))
    {
        if (g_debug)    
            fprintf(stderr, "nothing changed , nothing save to nvram\n");
        return ;
    }
    
   // if ((NULL == a->dlFaultMsg || !strcmp(a->dlFaultMsg, stTr69cAcsInfo.dlFaultMsg))\
    //   && (NULL == a->downloadCommandKey || !strcmp(a->downloadCommandKey, stTr69cAcsInfo.downloadCommandKey))\
    //    && (NULL == a->rebootCommandKey || !strcmp(a->rebootCommandKey, stTr69cAcsInfo.rebootCommandKey))\
     //   && (NULL == a->newParameterKey || !strcmp(a->newParameterKey, stTr69cAcsInfo.newParameterKey)))
   // {
   //     if (g_debug)
    //        fprintf(stderr, "nothing changed , nothing save to nvram 2\n");
    //    return ;
  //  }

    // fill State Data structure from acsState data 
    if ( a->downloadCommandKey )
        strncpy(stTr69cAcsInfo.downloadCommandKey, a->downloadCommandKey, sizeof(stTr69cAcsInfo.downloadCommandKey));
    if ( a->rebootCommandKey )
        strncpy(stTr69cAcsInfo.rebootCommandKey, a->rebootCommandKey, sizeof(stTr69cAcsInfo.rebootCommandKey));
    if ( a->newParameterKey )
        strncpy(stTr69cAcsInfo.newParameterKey, a->newParameterKey, sizeof(stTr69cAcsInfo.newParameterKey));
    else if (a->parameterKey)// otherwise keep old one 
        strncpy(stTr69cAcsInfo.newParameterKey, acsState.newParameterKey, sizeof(stTr69cAcsInfo.newParameterKey));

    stTr69cAcsInfo.contactedState = acsContactedState;
    stTr69cAcsInfo.dlFaultStatus = a->dlFaultStatus;
    stTr69cAcsInfo.startDLTime = a->startDLTime;
    stTr69cAcsInfo.endDLTime = a->endDLTime;
    if ( a->dlFaultMsg )
        strncpy(stTr69cAcsInfo.dlFaultMsg, a->dlFaultMsg, sizeof(stTr69cAcsInfo.dlFaultMsg));

	//if ( acsState.downloadCommandKey ) {
		//free(acsState.downloadCommandKey);
		//acsState.downloadCommandKey = NULL;
	//}
    stTr69cAcsInfo.upgradesManaged = acsState.upgradesManaged;
    //循环保存到NVRAM中获取的数据 
    for (iLoop = TR069_NEW_PARAMETERKEY; iLoop <= TR069_ACS_CONNECT_STATE; iLoop++)
    {
        stTr69cAcsInfo.eSetFlag = iLoop;
        // 最后一次循环才保存到flash 
        stTr69cAcsInfo.iUnSaveFlag = (iLoop == TR069_ACS_CONNECT_STATE)?0:1;
        if(OM_E_NONE != om_tr69c_set_acsinfo(&stTr69cAcsInfo))
        {
            slog(TR69C_LOG_ERR,"set nvram %d ERROR", iLoop);
        }
    }
  
    if (g_debug)
    {
        fprintf(stderr, "Save TR69 State Variables to nvram \n");
        fprintf(stderr, "CommandKey(Reboot)=%s\n", a->rebootCommandKey);
        fprintf(stderr, "CommandKey(DL)=%s\n", a->downloadCommandKey);
        fprintf(stderr, "ParameterKey=%s\n", a->parameterKey);
        fprintf(stderr, "ACS Contacted State=%d\n", acsContactedState);
    } 
	#endif */
}

void saveACSContactedState()
{
    if (g_debug)
        fprintf(stderr, "saveACSContactedState called , store it in nvram now\n");
    saveTR69StatusItems(&acsState); /* add by zhanghaozhong */

}

eACSContactedState getACSContactedState(void)
{
/*  Need to fetch from persistent memory  */
    OM_TR69C_APP_PARAM_T stTr69cAcsInfo;
	char acBuf[128]={0};
	stTr69cAcsInfo.contactedState=0;
 /*    stTr69cAcsInfo.eSetFlag = TR069_ACS_CONNECT_STATE;
    if(OM_E_NONE != om_tr69c_get_acsinfo(&stTr69cAcsInfo))
    {
       slog(TR69C_LOG_ERR,"[tr69c:%s,%d]get connect_state failed ",__func__,__LINE__);
       stTr69cAcsInfo.contactedState=0;
    }
	acsContactedState = stTr69cAcsInfo.contactedState;

    slog(TR69C_LOG_DEBUG,"getACSContactedState returns %d", acsContactedState); */
	/*f=fopen("/etc/tau.file","rb");
 	if(f==NULL)
 	{
		printf(" tau open fail \n");
		return stTr69cAcsInfo.contactedState;
	}
	if(NULL != fgets(acBuf, sizeof(acBuf),f))
	fclose(f);
	stTr69cAcsInfo.contactedState=atoi(acBuf);
	acsContactedState = stTr69cAcsInfo.contactedState;*/
    char *value=NULL;
	if(UCI_OK != uci_config_get(UCI_ACSCONTACTEDSTATE,acBuf))
	{
		value = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return ;
	}
	else{
		value = strdup(acBuf);
		
	}	
	stTr69cAcsInfo.contactedState=atoi(value);
	if(value)
	{
		free(value);
		value=NULL;
	}
	acsContactedState = stTr69cAcsInfo.contactedState;
	return stTr69cAcsInfo.contactedState;
}

/* 设置当前acs状态 */
void setACSContactedState(eACSContactedState state)
{
     OM_TR69C_APP_PARAM_T stTr69cAcsInfo;
	// FILE* f=NULL;
	 char acBuf[128]={0};
    acsContactedState = state;
    stTr69cAcsInfo.contactedState = state;
    /*if(OM_E_NONE != om_tr69c_set_acsinfo(&stTr69cAcsInfo))
    {
        slog(TR69C_LOG_ERR,"get connect_state failed ");
        return ;
    }
    slog(TR69C_LOG_ERR,"setACSContactedState: %d", acsContactedState); */
    
   /* f=fopen("/etc/tau.file","w");
 	if(f==NULL)
 	{
		printf(" tau open fail \n");
		return -1;
	}
    acBuf=itoa(stTr69cAcsInfo.contactedState); 
	fputs(acBuf,f);
	fclose(f);*/
	//char acBuf[256]={0};
	printf("THE STAT   IS   %d \n",stTr69cAcsInfo.contactedState);
	memset(acBuf,0,sizeof(acBuf));
	strcpy(acBuf,itoa(stTr69cAcsInfo.contactedState)); 
	if(UCI_OK !=uci_config_set(UCI_ACSCONTACTEDSTATE,acBuf))
	{
		slog(TR69C_LOG_ERR,"ERROR");
		return;
	}
	printf("THE acBuf   IS   %s\n",acBuf);
	return;
}

/* getNewInstanceId return an instance id in the integer range that
* has not yet been used.This should be save in persistent memory 
* somewhere and initialized on startup.
* Need to think about integer wrap-around and asigning duplicates.
*  ?????????
*/
int getNewInstanceId(void)
{
    return ++lastInstanceId;
}
/*将配置文件中的 数据写入到 nvram中*/
void write_to_nvram(void)
{
    FILE *pstFp = NULL;
	char acBuf[128] = {0};
	char cmd_str[256] = {0};
   	pstFp= fopen(TR69C_DEFAULT_CONFIG_FILE, "r");
    if (!pstFp){
        slog(TR69C_LOG_ERR, "Open "TR69C_DEFAULT_CONFIG_FILE" fail.");
		return ;     
    }
	fseek(pstFp, 0, SEEK_SET);
	memset(acBuf, 0, sizeof(acBuf));
	memset(cmd_str, 0, sizeof(cmd_str));
	while(NULL != fgets(acBuf, sizeof(acBuf),pstFp))
	{
	 	if(0 != strlen(acBuf))
	 	{	//slog(TR69C_LOG_ERR, " config file str = %s ",acBuf);
			if((acBuf[strlen(acBuf)-2] == 0x0d)||(acBuf[strlen(acBuf)-2] == 0x0a))
			{	
			//	slog(TR69C_LOG_ERR, " config file str 0 = %s ",acBuf);
				acBuf[strlen(acBuf)-2] ='\0';
			//	slog(TR69C_LOG_ERR, " config file str 1 = %s ",acBuf);
				sprintf(cmd_str,"nvram set %s",acBuf);
				system(cmd_str);
			}else if((acBuf[strlen(acBuf)-1] == 0x0d)||(acBuf[strlen(acBuf)-1] == 0x0a))
			{
				acBuf[strlen(acBuf)-1] ='\0';
				//slog(TR69C_LOG_ERR, " config file str 2 = %s ",acBuf);
				sprintf(cmd_str,"nvram set %s",acBuf);
				system(cmd_str);
			}
			memset(cmd_str, 0, sizeof(cmd_str));
			memset(acBuf, 0, sizeof(acBuf));
	 	}
		
	}
	fclose(pstFp);
	return;
}
/* downloadComplete()
*  Called when image has been downloaded. If successful the *buf will point to the
*  image buffer. If *buf is NULL the download failed.
*  Control is returned to the framework with a 1 if the flash was successful.
*  If the flash image failed the return is a 0. Free the buffer prior
*  to return to the framework.
*/
int  downloadComplete(DownloadReq *r, char *buf)
{
    FILE	*f;
    size_t	wlth;
 slog(TR69C_LOG_DEBUG, "downloadComplete start    1");
   
    if (g_debug)
    {
        fprintf(stderr, "downloadComplete --save ACSState and  flash image here\n");
        fprintf(stderr, "Image size is %d\n", r->fileSize);
    }
slog(TR69C_LOG_DEBUG, "downloadComplete start    2");

      
    if ( r->efileType == eFirmwareUpgrade )
    {       
        slog(TR69C_LOG_DEBUG,"[tr69c:%s,%d]eFirmwareUpgrade",__func__,__LINE__);

		f = fopen(TR69C_SOFTWARE_FILE, "w");  /* modify by zhanghaozhong  */
		if (f) 
        {
			int mlth = r->fileSize;
			printf("mlth=%d\n", mlth);
			wlth = fwrite(buf, 1, mlth, f);
			fclose(f);
			//sleep(100);
            if (g_debug)
			    fprintf(stderr, "%s file written with %d bytes\n",TR69C_SOFTWARE_FILE, wlth);
			//updateDownLoadKey( r );
			/* does not return */
			acsState.dlFaultStatus = 0;	/* no download fault */
			acsState.dlFaultMsg = "Download app.tar.gz successful";
			printf("write!\n");
			printf("acsState.downloadCommandKey = %s !!!\n",acsState.downloadCommandKey);
			acsContactedState = eACS_DOWNLOADREBOOT;
			protect_connect_state_flag=0;
			saveTR69StatusItems(&acsState);
			setACSContactedState(eACS_DOWNLOADREBOOT);	
			protect_connect_state_flag=1;
		//	printf("write!  acsContactedState  = %d \n",acsContactedState);
		//	sleep(10);
		//	int actmp = 1;
		//	om_tr69c_Upgrade_Complete_set(actmp);

		printf("om_tr69c_Upgrade_Complete_set  OK!\n");
	#if 1  //注释by duanguiyuan 20160323 		
            if (0 != system("/usr/sbin/vflag localupdate > /dev/null")) /* 本地升级版本 */
                slog(TR69C_LOG_ERR, "tr69c: vflag localupdate ERROR\n");
			
			slog(TR69C_LOG_ERR, "tr69c: vflag localupdate success\n ");
 			//sleep(120);	
			system("/usr/sbin/reboot 5 ebooting system by tr69c upgrade version&");
			printf("Reboot  OK!\n");
#endif
		}
	}
    else if(r->efileType == eVendorConfig)
    {   printf("download config file   begin!\n"); 
        #if 1
        /* 设备的配置文件升级 */
        f = fopen(TR69C_DEFAULT_CONFIG_FILE, "w");  /* modify by zhanghaozhong  */
        if (f) {
            int mlth = r->fileSize;
            wlth = fwrite(buf, 1, mlth, f);
            fclose(f);
            if (g_debug)
            fprintf(stderr, "%s file written with %d bytes\n", TR69C_DEFAULT_CONFIG_FILE, wlth);
            //updateDownLoadKey( r );
            /* does not return */
            acsState.dlFaultStatus = 0; /* no download fault */
            acsState.dlFaultMsg = "Download default_config successful";
			acsContactedState = eACS_DOWNLOADREBOOT;
			
			printf("download from ASC end !!  OK!\n");
			//write_to_nvram();/*cpe上的操作，在tau上没有nvram set命令，故屏蔽掉*/
    		//system("nvram reset "TR69C_DEFAULT_CONFIG_FILE); 

			protect_connect_state_flag=0;
            saveTR69StatusItems(&acsState);
			setACSContactedState(eACS_DOWNLOADREBOOT);
			protect_connect_state_flag=1;
			system("/usr/sbin/reboot 5 ebooting system by tr69c  configfile&");
			printf("Reboot  OK!\n");
			/* 
			if (OM_E_NONE != oma_recover_defaut_config( RECOVER_DEFAULT_CONFIG, TR69C_DEFAULT_CONFIG_FILE))
                slog(TR69C_LOG_ERR, "tr69c: recover config file ERROR\n");
            else
            {
                system("nvram commit");		*//* 保存nvram到flash */
   //             system("/usr/sbin/reboot 3 ebooting system by tr69c set config file&");
     //       }
			
        }
        #endif
    }
    else
    {
        if (g_debug)
            slog(TR69C_LOG_ERR,"efileType from ACS ERROR!");
    }
    free(buf);
	buf=NULL;
    return 1;
}

void wrapperReboot(eACSContactedState rebootContactValue)
{
	printf("write connection  state  0 \n");
	setACSContactedState( rebootContactValue );	 /* reset on BcmCfm_download error */
	//saveTR69StatusItems(&acsState);

	if (g_debug)
	    fprintf(stderr, "CPE is REBOOTING with rebootContactValue =%d\n", rebootContactValue);
   		//system("/usr/sbin/reboot 300 ebooting system by tr69c because of Reboot CMD by ACS&");
   
	//system("reboot 5 normal reboot by tr69c because of Reboot CMD by ACS >/dev/null 2>&1 &");
	system("reboot");   //tau设备上的重启
	
   /* 调用reboot命令,记录重启日志 */
}
/*
* Call library factory reset 
*/
/* 恢复出厂设置 */
void wrapperFactoryReset(void)
{
   /* fprintf(stderr, "CPE is reset to factory settings by OMA\n");
 	//注释by duanguiyuan 20160323 
    if( OM_E_NONE != om_sysmnt_cfg_restore_factory())
        slog(TR69C_LOG_ERR,"[tr69c:%s,%d] ERROR!",__func__,__LINE__);
     
    if (g_debug)
    	fprintf(stderr, "CPE is REBOOTING by wrapperFactoryReset\n"); */
  //  system("/usr/sbin/reboot 2 ebooting system by tr69c because of FactoryReset&");
    // 调用reboot命令,记录重启日志 
   system("restore -a"); 
}

void wrapperSaveConfigurations(void)
{
    slog(TR69C_LOG_NOTICE,"CPE is SAVING its configurations !");
}


TRX_STATUS getDeviceStatus(char **value)
{
    *value = strdup("normal");
    return TRX_OK;
}

TRX_STATUS getUserName(char **value)
{
    *value = strdup("admin");

    return TRX_OK;
}

/* 根据当前的oem厂商，获取oem厂商名 */
TRX_STATUS getManufacturer(char **ppcValue)
{
   /*  OM_TR69C_DEVICE_PARAM_T *stTr69cParam = (OM_TR69C_DEVICE_PARAM_T *)malloc(sizeof(OM_TR69C_DEVICE_PARAM_T)); 
    memset(stTr69cParam, 0, sizeof(OM_TR69C_DEVICE_PARAM_T));
    stTr69cParam->eFlag = TR69C_PARAM_MAN_FACT;  // 设置要操作的内容 
    if(OM_E_NONE != om_tr69c_get_devinfo(stTr69cParam))
    {
        free(stTr69cParam);
        *value = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    *value = strdup(stTr69cParam->Manufacturer);
    free(stTr69cParam); */
	char aManufacturer[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_DEVICEINFO_MAMUFACTURER,aManufacturer))
	{
		*ppcValue = strdup("");
		slog(TR69C_LOG_ERR,"ERROR");
		return TRX_ERR;
	}
	else
	{
		 *ppcValue = strdup(aManufacturer);				  
	}	

	
   // *value = strdup("Huawei Technologies Co., Ltd.");
    return TRX_OK;
}

/* 厂商OUI */
TRX_STATUS getManufacturerOUI(char **ppcValue)
{
    /* OM_TR69C_DEVICE_PARAM_T *stTr69cParam = (OM_TR69C_DEVICE_PARAM_T *)malloc(sizeof(OM_TR69C_DEVICE_PARAM_T)); 
    memset(stTr69cParam, 0, sizeof(OM_TR69C_DEVICE_PARAM_T));
    stTr69cParam->eFlag = TR69C_PARAM_MAN_FACT_OUI;  // 设置要操作的内容 
    if(OM_E_NONE != om_tr69c_get_devinfo(stTr69cParam))
    {
        free(stTr69cParam); 
        *value = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    
    *value = strdup(stTr69cParam->ManufacturerOUI);
	 slog(TR69C_LOG_ERR,">>>>>>>>>>>>>> OUI %s ",stTr69cParam->ManufacturerOUI);
    free(stTr69cParam); */
    char aManufacturerOui[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_DEVICEINFO_MAMUFACTUREROUI,aManufacturerOui))
	{
		*ppcValue = strdup("");
		slog(TR69C_LOG_ERR,"ERROR");
		return TRX_ERR;
	}
	else
	{
		 *ppcValue = strdup(aManufacturerOui);				  
	}	

	printf("aManufacturerOui is &&&&&&& %s \n",*ppcValue);
	//*value = strdup("ACE215");

    return TRX_OK;
}

/* 根据oem，获取当前的设备型号 */
TRX_STATUS getModelName(char **ppcValue)
{
   /*  OM_TR69C_DEVICE_PARAM_T *stTr69cParam = (OM_TR69C_DEVICE_PARAM_T *)malloc(sizeof(OM_TR69C_DEVICE_PARAM_T)); 
    memset(stTr69cParam, 0, sizeof(OM_TR69C_DEVICE_PARAM_T));
    stTr69cParam->eFlag = TR69C_PARAM_MODULE; 
    if(OM_E_NONE != om_tr69c_get_devinfo(stTr69cParam))    
    {
        free(stTr69cParam); 
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    *value = strdup(stTr69cParam->ModelName);  
    free(stTr69cParam); */
    char acModelName[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_DEVICEINFO_MODELNAME,acModelName))
	{
		*ppcValue = strdup("");
		slog(TR69C_LOG_ERR,"ERROR");
		return TRX_ERR;
	}
	else
	{
		 *ppcValue = strdup(acModelName);				  
	}	

	printf("acModelName is &&&&&&& %s \n",*ppcValue);
   // *value = strdup("TAU_H2000");   
    return TRX_OK;
}

/* 根据oem，获取当前的设备描述信息 */
TRX_STATUS getDescription(char **ppcValue)
{
	char acDescription[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_DEVICEINFO_DESCRIPTION,acDescription))
	{
		*ppcValue = strdup("");
		slog(TR69C_LOG_ERR,"ERROR");
		return TRX_ERR;
	}
	else
	{
		 *ppcValue = strdup(acDescription);				  
	}		
	
	//*value = strdup("Huawei Gateway");
    return getModelName(ppcValue);
}

TRX_STATUS getProductType(char **ppcValue)
{
	return TRX_OK;
}
TRX_STATUS getAdditionalSoftwareVersion(char **ppcValue)
{
	return TRX_OK;
}
/* 终端类型,室内室外ltecpe */
TRX_STATUS getProductClass (char **ppcValue)
{
   /*  OM_TR69C_DEVICE_PARAM_T *stTr69cParam = (OM_TR69C_DEVICE_PARAM_T *)malloc(sizeof(OM_TR69C_DEVICE_PARAM_T));
    memset(stTr69cParam, 0, sizeof(OM_TR69C_DEVICE_PARAM_T));
    stTr69cParam->eFlag = TR69C_PARAM_PRODUCT_CLASS;  
    if(OM_E_NONE != om_tr69c_get_devinfo(stTr69cParam))    
    {
        free(stTr69cParam); 
        *value = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
     */

    char acProductClass[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_DEVICEINFO_PRODUCTCLASS,acProductClass))
	{
		*ppcValue = strdup("");
		slog(TR69C_LOG_ERR,"ERROR");
		return TRX_ERR;
	}
	else
	{
		 *ppcValue = strdup(acProductClass);				  
	}	

   // *value = strdup("LTE CPE");  

    return TRX_OK;
}

/* 获取设备序列号 IMEI */
TRX_STATUS getSerialNumber(char **ppcValue)
{
	/* if(strlen(lteinfostate.aucModuleIMEI) >2)
	{
		*value = strdup(lteinfostate.aucModuleIMEI);
		 return TRX_OK;
	}
    OM_TR69C_LTE_MODULE_INFO_T *trLteModule = (OM_TR69C_LTE_MODULE_INFO_T *)malloc(sizeof(OM_TR69C_LTE_MODULE_INFO_T));
    memset(trLteModule, 0, sizeof(OM_TR69C_LTE_MODULE_INFO_T));

    if(OM_E_NONE != om_lte_tr69_module_info_get(trLteModule))    
    {
        free(trLteModule); 
        *value = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_OK;
    }
	memcpy(lteinfostate.aucModuleIMEI,(char*)trLteModule->aucModuleIMEI,sizeof((char*)trLteModule->aucModuleIMEI));
    *value = strdup((char *)trLteModule->aucModuleIMEI);  
	printf("get aucModuleIMEI  000 =%s !!!!!\n",lteinfostate.aucModuleIMEI);
    free(trLteModule); */
    char acSerialNumber[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_DEVICEINFO_SERIALNUMBER,acSerialNumber))
	{
		*ppcValue = strdup("");
		slog(TR69C_LOG_ERR,"ERROR");
		return TRX_ERR;
	}
	else
	{
		 *ppcValue = strdup(acSerialNumber);				  
	}	

	
    //*value = strdup("a12321a");
    return TRX_OK;
}

TRX_STATUS getCpeMac(char **value)
{
/*
	OM_TR69C_DEVICE_PARAM_T *stTr69cParam = (OM_TR69C_DEVICE_PARAM_T *)malloc(sizeof(OM_TR69C_DEVICE_PARAM_T)); 
    memset(stTr69cParam, 0, sizeof(OM_TR69C_DEVICE_PARAM_T));
    stTr69cParam->eFlag = TR69C_PARAM_CPE_MAC;  
    if(OM_E_NONE != om_tr69c_get_devinfo(stTr69cParam))    
    {
        free(stTr69cParam); 
        *value = strdup("");
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    *value = strdup(stTr69cParam->acCpeMac);
    free(stTr69cParam);
    */
    
    *value = strdup("C6:76:A4:CF:FC:01");
   // *value = strdup(lteinfostate.aucDeviceMAC);
    return TRX_OK;
}

TRX_STATUS getHardwareVersion(char **ppcValue)
{
    /* OM_TR69C_DEVICE_PARAM_T *stTr69cParam = (OM_TR69C_DEVICE_PARAM_T *)malloc(sizeof(OM_TR69C_DEVICE_PARAM_T)); 
    memset(stTr69cParam, 0, sizeof(OM_TR69C_DEVICE_PARAM_T));
    stTr69cParam->eFlag = TR69C_PARAM_HARD_VER;  
    if(OM_E_NONE != om_tr69c_get_devinfo(stTr69cParam))    
    {
        slog(TR69C_LOG_ERR,"ERROR");
        *value = strdup("v1.0");
    }    
    else
    {
        *value = strdup(stTr69cParam->acHardVer);
    }
    
    free(stTr69cParam); */
    char acHardwareVersion[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_DEVICEINFO_HARDVER,acHardwareVersion))
	{
		*ppcValue = strdup("");
		slog(TR69C_LOG_ERR,"ERROR");
		return TRX_ERR;
	}
	else
	{
		 *ppcValue = strdup(acHardwareVersion);				  
	}	

	
    return TRX_OK;
}

TRX_STATUS getSoftwareVersion(char **ppcValue)
{
   /*  OM_TR69C_DEVICE_PARAM_T *stTr69cParam = (OM_TR69C_DEVICE_PARAM_T *)malloc(sizeof(OM_TR69C_DEVICE_PARAM_T));  
    memset(stTr69cParam, 0, sizeof(OM_TR69C_DEVICE_PARAM_T));
    stTr69cParam->eFlag = TR69C_PARAM_SOFT_WARE;  
    if(OM_E_NONE != om_tr69c_get_devinfo(stTr69cParam))    
    {
        slog(TR69C_LOG_ERR,"ERROR");
        *value = strdup("v1.0");
    }
    else
    {
        *value = strdup(stTr69cParam->acSoftVer);
    }
    free(stTr69cParam); */
    char acSoftwareVersion[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_DEVICEINFO_SOFTVER,acSoftwareVersion))
	{
		*ppcValue = strdup("");
		slog(TR69C_LOG_ERR,"ERROR");
		return TRX_ERR;
	}
	else
	{
		 *ppcValue = strdup(acSoftwareVersion);				  
	}	

	
    return TRX_OK;
}

/* 获取固件版本号，即boot version */
TRX_STATUS getModemFirmwareVersion(char **ppcValue)
{
   /*  OM_TR69C_DEVICE_PARAM_T *stTr69cParam = (OM_TR69C_DEVICE_PARAM_T *)malloc(sizeof(OM_TR69C_DEVICE_PARAM_T));  
    memset(stTr69cParam, 0, sizeof(OM_TR69C_DEVICE_PARAM_T));
    stTr69cParam->eFlag = TR69C_PARAM_MODEM_FIRMWARE_VER;  
    if(OM_E_NONE != om_tr69c_get_devinfo(stTr69cParam))    
    {
        slog(TR69C_LOG_ERR,"ERROR");
        *value = strdup("v1.0");
    }
    else
    {
        *value = strdup(stTr69cParam->acModemFirmwareVer);
    }
    free(stTr69cParam); */
    char acModemFirmwareVersion[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_DEVICEINFO_MODEMFIRMWAREVER,acModemFirmwareVersion))
	{
		*ppcValue = strdup("");
		slog(TR69C_LOG_ERR,"ERROR");
		return TRX_ERR;
	}
	else
	{
		 *ppcValue = strdup(acModemFirmwareVersion);				  
	}	

	printf("aModemFirmwareVersion is &&&&&&& %s \n",*ppcValue);
    return TRX_OK;
}

/* 配置码
Identifier of the primary service provider and other
provisioning information, which MAY be used by
the ACS to determine service provider-specific
customization and provisioning parameters.
If non-empty, this argument SHOULD be in the
form of a hierarchical descriptor with one or more
nodes specified. Each node in the hierarchy is
represented as a 4-character sub-string, containing
only numerals or upper-case letters. If there is
more than one node indicated, each node is
separated by a "." (dot). Examples: “TLCO” or
“TLCO.GRP2”.*/
TRX_STATUS setProvisioningCode(const char *pcValue)
{
   /*  OM_TR69C_DEVICE_PARAM_T *stTr69cParam = (OM_TR69C_DEVICE_PARAM_T *)malloc(sizeof(OM_TR69C_DEVICE_PARAM_T)); 
    memset(stTr69cParam, 0, sizeof(OM_TR69C_DEVICE_PARAM_T));
    stTr69cParam->eFlag = TR69C_PARAM_DEV_NAME;  // 设置要操作的内容 
    strcpy(stTr69cParam->acDevName, value);
    if(OM_E_NONE != om_tr69c_set_devinfo(stTr69cParam))
    {
        free(stTr69cParam);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }
    free(stTr69cParam);
    if (acsState.provisioningCode)
		free(acsState.provisioningCode);
	acsState.provisioningCode = strdup(value); */
    if(UCI_OK !=uci_config_set(UCI_DEVICEINFO_PROVISIONINGCODE,pcValue))
	{
		slog(TR69C_LOG_ERR,"ERROR");
        return  TRX_ERR;
	}
    return TRX_OK;
}

TRX_STATUS getProvisioningCode(char **ppcValue)
{
    /* OM_TR69C_DEVICE_PARAM_T *stTr69cDevInfo = (OM_TR69C_DEVICE_PARAM_T *)malloc(sizeof(OM_TR69C_DEVICE_PARAM_T));
    memset(stTr69cDevInfo, 0, sizeof(OM_TR69C_DEVICE_PARAM_T));
    stTr69cDevInfo->eFlag = TR69C_PARAM_PROV_CODE;
    if(OM_E_NONE != om_tr69c_get_devinfo(stTr69cDevInfo))
    {
        free(stTr69cDevInfo);
        slog(TR69C_LOG_ERR,"ERROR");
        return TRX_ERR;
    }     //need to be open 
    if(acsState.provisioningCode)
        free(acsState.provisioningCode);
    acsState.provisioningCode = strdup(stTr69cDevInfo->ProvisioningCode);
    *value = acsState.provisioningCode?strdup(acsState.provisioningCode):strdup("");
    free(stTr69cDevInfo); */
    char acProvisioningCode[TAUARRY_LEN]={0};
	if (NULL != acsState.provisioningCode)
	{
	 	free(acsState.provisioningCode);
		acsState.provisioningCode=NULL;
	}
	
	if(UCI_OK != uci_config_get(UCI_DEVICEINFO_PROVISIONINGCODE,acProvisioningCode))
	{
		*ppcValue = strdup("");
		slog(TR69C_LOG_ERR,"ERROR");
		return TRX_ERR;
	}
	else
	{
		 *ppcValue = strdup(acProvisioningCode);				  
	}	
	 acsState.provisioningCode=strdup(*ppcValue);
	
   
	//*value = strdup(acsState.provisioningCode);
    return TRX_OK;
}

/* 终端上线时长 */
TRX_STATUS getUpTime(char **ppcValue)
{
	/* //time_t  upt;
	char buf[CHAR_LEN_64];

	INT32 uiUpTime = 0;
            
	//upt = time(NULL) - startTime;
	//sprintf(buf, "%lu", upt);       
	



	// 获取设备上电时间 
    if(AP_E_NONE != app_get_up_time(&uiUpTime))
    {
        slog(TR69C_LOG_ERR, "get up time failed");
    }
     sprintf(buf, "%d", uiUpTime);
     *value = strdup(buf);    */
    char acUpTime[TAUARRY_LEN]={0};
	if(UCI_OK != uci_config_get(UCI_DEVICEINFO_UPTIME,acUpTime))
	{
		*ppcValue = strdup("");
		slog(TR69C_LOG_ERR,"ERROR");
		return TRX_ERR;
	}
	else
	{
		 *ppcValue = strdup(acUpTime);				  
	}	

	
	return TRX_OK;
}
/* 终端日志 */
TRX_STATUS getDeviceLog(char **value)
{
	*value = strdup("");
    return TRX_OK;
}


void initDeviceInfoInstances(void)
{
    TRxObjNode *n;
    int id;
    InstanceDesc  *deviceInfoIdp;
	InstanceDesc  *DeviceInfoX_CMCC_TeleComAccountIdp;
	InstanceDesc  *DeviceInfoX_ATP_InformationsIdp;
	
	InstanceDesc  *DeviceInfoX_CMCC_MonitorMonitorConfigIdp;
	InstanceDesc  *DeviceInfoX_CMCC_MonitorMonitorConfigDescIdp;
	

    
    n = deviceInfoDesc;
    id = 1;
    deviceInfoIdp = getNewInstanceDesc(n, NULL, id);
	n = DeviceInfoX_ATP_InformationsInstanceDesc;
	DeviceInfoX_ATP_InformationsIdp = getNewInstanceDesc(n, deviceInfoIdp, id);
	//altered by caibing 2016-11-15
#if 0
	n = DeviceInfoX_CMCC_TeleComAccountDesc;
	DeviceInfoX_CMCC_TeleComAccountIdp = getNewInstanceDesc(n, deviceInfoIdp, id);
	
	n = DeviceInfoX_CMCC_MonitorDesc;
	DeviceInfoX_CMCC_TeleComAccountIdp = getNewInstanceDesc(n, deviceInfoIdp, id);

	
	n = DeviceInfoX_CMCC_MonitorMonitorConfig;
	DeviceInfoX_CMCC_MonitorMonitorConfigIdp = getNewInstanceDesc(n, DeviceInfoX_CMCC_TeleComAccountIdp,id);

	
	n = DeviceInfoX_CMCC_MonitorMonitorConfigDesc;
	DeviceInfoX_CMCC_MonitorMonitorConfigDescIdp = getNewInstanceDesc(n, DeviceInfoX_CMCC_MonitorMonitorConfigIdp,id);
#endif
}

void initX_DeviceInfoInstances(void)
{
    TRxObjNode *n;
    int id;
    InstanceDesc  *x_DeviceInfoIdp;
    
    n = x_deviceInfoDesc;
    id = 1;
    x_DeviceInfoIdp = getNewInstanceDesc(n, NULL, id);
}


void initDeviceConfigInstances(void)
{
    TRxObjNode *n;
    int id;
    InstanceDesc  *DeviceConfigIdp;
    
    n = DeviceConfigDesc;
    id = 1;
    DeviceConfigIdp = getNewInstanceDesc(n, NULL, id);
}

/*void initX_ATP_FireWallInstances()
{
	TRxObjNode *n;
    int id;
    InstanceDesc  *X_ATP_FireWallIdp;
	
	InstanceDesc  *FilterModeDescObjIdp;
	InstanceDesc  *UrlBlackFilterDescObjIdp;
	InstanceDesc  *MacBlackFilterDescObjIdp;
	InstanceDesc  *IpBlackFilterDescObjIdp;
	InstanceDesc  *UrlWhiteFilterDescObjIdp;
	InstanceDesc  *MacWhiteFilterDescObjIdp;
    InstanceDesc  *IpWhiteFilterDescObjIdp;
	InstanceDesc  *AccessControlDescObjIdp;
    n = X_ATP_FireWallDesc;
    id = 1;
    X_ATP_FireWallIdp = getNewInstanceDesc(n, NULL, id);

  	n = FilterModeDescObj;
	FilterModeDescObjIdp = getNewInstanceDesc(n, X_ATP_FireWallIdp, id);

	n = UrlBlackFilterDescObj;
	UrlBlackFilterDescObjIdp = getNewInstanceDesc(n, X_ATP_FireWallIdp, id);

	n = MacBlackFilterDescObj;
	MacBlackFilterDescObjIdp = getNewInstanceDesc(n, X_ATP_FireWallIdp, id);

	n = IpBlackFilterDescObj;
	IpBlackFilterDescObjIdp = getNewInstanceDesc(n, X_ATP_FireWallIdp, id);

	n = UrlWhiteFilterDescObj;
	UrlWhiteFilterDescObjIdp = getNewInstanceDesc(n, X_ATP_FireWallIdp, id);

	n = MacWhiteFilterDescObj;
	MacWhiteFilterDescObjIdp = getNewInstanceDesc(n, X_ATP_FireWallIdp, id);

	n = IpWhiteFilterDescObj;
	IpWhiteFilterDescObjIdp = getNewInstanceDesc(n, X_ATP_FireWallIdp, id);

	n = AccessControlDescObj;
	AccessControlDescObjIdp = getNewInstanceDesc(n, X_ATP_FireWallIdp, id);


}*/
void initX_ATP_FireWallInstances()
{
	TRxObjNode *n;
    int id;
    InstanceDesc  *X_ATP_FireWallIdp;
	
	InstanceDesc  *IpFilterIdp;
	InstanceDesc  *MacFilterIdp;
	InstanceDesc  *UrlFilterIdp;
	InstanceDesc  *SwitchIdp;
	
	
    n = X_ATP_FireWallDesc;
    id = 1;
    X_ATP_FireWallIdp = getNewInstanceDesc(n, NULL, id);
  	n = IpFilterDesc;
	IpFilterIdp = getNewInstanceDesc(n, X_ATP_FireWallIdp, id);
	
	n = MacFilterDesc;
	MacFilterIdp = getNewInstanceDesc(n, X_ATP_FireWallIdp, id);
	
	n = UrlFilterDesc;
	UrlFilterIdp = getNewInstanceDesc(n, X_ATP_FireWallIdp, id);
	
	n = SwitchDesc;
	SwitchIdp = getNewInstanceDesc(n, X_ATP_FireWallIdp, id);
}

void initX_ATP_SystemMonitorInstances()
{
	TRxObjNode *n;
    int id;
    InstanceDesc  *X_ATP_SystemMonitorIdp;
	InstanceDesc  *LanCheckIdp;
	InstanceDesc  *WanCheckIdp;
	InstanceDesc  *SysMonitorIdp;
	
    
    n = X_ATP_SystemMonitorDesc;
    id = 1;
    X_ATP_SystemMonitorIdp = getNewInstanceDesc(n, NULL, id);
	
	n = LanCheckDesc;
    LanCheckIdp = getNewInstanceDesc(n, X_ATP_SystemMonitorIdp, id);
	
	n = WanCheckDesc;
    WanCheckIdp = getNewInstanceDesc(n, X_ATP_SystemMonitorIdp, id);
	
	n = SysMonitorDesc;
    SysMonitorIdp = getNewInstanceDesc(n, X_ATP_SystemMonitorIdp, id);
}
void initFreqLockInstances(void)
{
	TRxObjNode *n;
    int id;
    InstanceDesc  *FreqLockIdp;
	InstanceDesc  *FreqLDataIdp;
	
    
    n = FreqLockDesc;
    id = 1;
    FreqLockIdp = getNewInstanceDesc(n, NULL, id);
	
	n = FreqLDataDesc;
    FreqLDataIdp = getNewInstanceDesc(n, FreqLockIdp, id);
}
void initX_ATP_VxlanInstances(void)
{
	TRxObjNode *n;
    int id;
    InstanceDesc  *X_ATP_VxlanIdp;
	InstanceDesc  *VlanMapVniListIdp;
	InstanceDesc  *VlanMapDscpListIdp;
	InstanceDesc  *VXLAN_LACIdp;
	
    
    n = X_ATP_VxlanDesc;
    id = 1;
    X_ATP_VxlanIdp = getNewInstanceDesc(n, NULL, id);
	
	n = VlanMapVniListDesc;
    VlanMapVniListIdp = getNewInstanceDesc(n, X_ATP_VxlanIdp, id);
	
	n = VlanMapDscpListDesc;
    VlanMapDscpListIdp = getNewInstanceDesc(n, X_ATP_VxlanIdp, id);
	
	n = VXLAN_LACDesc;
    VXLAN_LACIdp = getNewInstanceDesc(n, X_ATP_VxlanIdp, id);
}
void initX_ATP_L2GreInstances(void)
{
	TRxObjNode *n;
    int id;
    InstanceDesc  *X_ATP_L2GreIdp;
	InstanceDesc  *IpMapDscpListIdp;
	InstanceDesc  *L2GRE_LACIdp;
	InstanceDesc  *IpMapVlanPrioListIdp;
    InstanceDesc  *VlanSettingsIdp;
	InstanceDesc  *VlanPrioMapDscpListIdp;
	InstanceDesc  *VlanListIdp;
    n = X_ATP_L2GreDesc;
    id = 1;
    X_ATP_L2GreIdp = getNewInstanceDesc(n, NULL, id);
	
	n = IpMapDscpListDesc;
    IpMapDscpListIdp = getNewInstanceDesc(n, X_ATP_L2GreIdp, id);
	
	n = L2GRE_LACDesc;
    L2GRE_LACIdp = getNewInstanceDesc(n, X_ATP_L2GreIdp, id);
	
	n = IpMapVlanPrioListDesc;
    IpMapVlanPrioListIdp = getNewInstanceDesc(n, X_ATP_L2GreIdp, id);
	
	n = VlanSettingsDesc;
    VlanSettingsIdp = getNewInstanceDesc(n, X_ATP_L2GreIdp, id);
	
	n = VlanPrioMapDscpListDesc;
    VlanPrioMapDscpListIdp = getNewInstanceDesc(n, X_ATP_L2GreIdp, id);
	
	n = VlanListDesc;
    VlanListIdp = getNewInstanceDesc(n, X_ATP_L2GreIdp, id);
	
	
}
void initX_ATP_VPNInstances(void)
{
	TRxObjNode *n;
    int id;
    InstanceDesc  *X_ATP_VPNIdp;
	InstanceDesc  *L2TP_LACIdp;
	
    n = X_ATP_VPNDesc;
    id = 1;
    X_ATP_VPNIdp = getNewInstanceDesc(n, NULL, id);
	
	n = L2TP_LACDesc;
    L2TP_LACIdp = getNewInstanceDesc(n, X_ATP_VPNIdp, id);
	
}
void initX_ATP_AlarmInfoInstances(void)
{
	TRxObjNode *n;
    int id;
    InstanceDesc  *X_ATP_AlarmInfoIdp;
	InstanceDesc  *AlarmInstanceIdp;
	
    n = X_ATP_AlarmInfoDesc;
    id = 1;
    X_ATP_AlarmInfoIdp = getNewInstanceDesc(n, NULL, id);
	
	n = AlarmInstanceDesc;
    AlarmInstanceIdp = getNewInstanceDesc(n, X_ATP_AlarmInfoIdp, id);
	
}
void initX_ATP_ConfigInstances(void)
{
	TRxObjNode *n;
    int id;
    InstanceDesc  *X_ATP_ConfigIdp;
	InstanceDesc  *dialupIdp;
	InstanceDesc  *profileIdp;
	
    n = X_ATP_ConfigDesc;
    id = 1;
    X_ATP_ConfigIdp = getNewInstanceDesc(n, NULL, id);
	
	n = dialupDesc;
    dialupIdp = getNewInstanceDesc(n, X_ATP_ConfigIdp, id);
	n = profileDesc;
    profileIdp = getNewInstanceDesc(n, dialupIdp, id);
	
}
void initServicesInstances(void)
{
	TRxObjNode *n;
    int id;
    InstanceDesc  *ServicesIdp;
	InstanceDesc  *VoiceServiceIdp;
	InstanceDesc  *VoiceProfileIdp;
	InstanceDesc  *X_ATP_CommonConfigIdp;
	InstanceDesc  *X_ATP_CommonConfigButtonIdp;
	InstanceDesc  *VoiceProfileLineIdp;
	InstanceDesc  *VoiceProfileSIPIdp;
	InstanceDesc  *VoiceProfileLineCodecIdp;
	InstanceDesc  *VoiceProfileLineSIPIdp;
	InstanceDesc  *VoiceProfileLineCodecListIdp;
	
    n = ServicesDesc;
    id = 1;
    ServicesIdp = getNewInstanceDesc(n, NULL, id);
	
	n = VoiceServiceDesc;
    VoiceServiceIdp = getNewInstanceDesc(n, ServicesIdp, id);

	n = VoiceProfileDesc;
    VoiceProfileIdp = getNewInstanceDesc(n, VoiceServiceIdp, id);
	
	n = X_ATP_CommonConfigDesc;
    X_ATP_CommonConfigIdp = getNewInstanceDesc(n, VoiceServiceIdp, id);
	
	n = ButtonDesc;
    X_ATP_CommonConfigButtonIdp = getNewInstanceDesc(n, X_ATP_CommonConfigIdp, id);
	
	n = LineDesc;
    VoiceProfileLineIdp = getNewInstanceDesc(n, VoiceProfileIdp, id);
	
	n = ProfileSIPDesc;
    VoiceProfileSIPIdp = getNewInstanceDesc(n, VoiceProfileIdp, id);
	
	n = CodecDesc;
    VoiceProfileLineCodecIdp = getNewInstanceDesc(n, VoiceProfileLineIdp, id);
	
	n = ListDesc;
    VoiceProfileLineCodecListIdp = getNewInstanceDesc(n, VoiceProfileLineCodecIdp, id);
	
	n = SIPDesc;
    VoiceProfileLineSIPIdp = getNewInstanceDesc(n, VoiceProfileLineIdp, id);
	
	
}
void initGatewayUserInterfaceInstances(void)
{
	TRxObjNode *n;
    int id;
    InstanceDesc  *GatewayUserInterfaceIdp;
	InstanceDesc  *X_ATP_WebDescIdp;
    InstanceDesc  *X_ATP_WebUserInfoDescObjIdp;
	
    n = GatewayUserInterfaceDesc;
    id = 1;
    GatewayUserInterfaceIdp = getNewInstanceDesc(n, NULL, id);

	n = X_ATP_WebDesc;
	X_ATP_WebDescIdp = getNewInstanceDesc(n, GatewayUserInterfaceIdp, id);
/*
	n = X_ATP_WebUserInfoDescObj;
	X_ATP_WebUserInfoDescObjIdp = getNewInstanceDesc(n, X_ATP_WebDescIdp, id);
*/
}
void initX_ATP_PreserveDataInstances(void)
{
	TRxObjNode *n;
    int id;
    InstanceDesc  *X_ATP_PreserveIdp;
	
    
    n = X_ATP_PreserveDataDesc;
    id = 1;
    X_ATP_PreserveIdp = getNewInstanceDesc(n, NULL, id);
}

void initX_ATP_EXTInstances()
{
	TRxObjNode *n;
    int id;
    InstanceDesc  *X_ATP_EXTIdp;
	
    
    n = X_ATP_EXTDesc;
    id = 1;
    X_ATP_EXTIdp = getNewInstanceDesc(n, NULL, id);

}

void initLayer3ForwardingInstances()
{
	TRxObjNode *n;
    int id;
    InstanceDesc  *Layer3ForwardingIdp;
	 InstanceDesc  *ForwardingDescObjIdp;
    
    n = Layer3ForwardingDesc;
    id = 1;
    Layer3ForwardingIdp = getNewInstanceDesc(n, NULL, id);	

	n = ForwardingDescObj;
	ForwardingDescObjIdp = getNewInstanceDesc(n, Layer3ForwardingIdp, id);	

}

void initX_ATP_ModemLogInstances(void)
{
	TRxObjNode *n;
    int id;
    InstanceDesc  *X_ATP_ModemLogIdp;
	
    
    n = X_ATP_ModemLogDesc;
    id = 1;
    X_ATP_ModemLogIdp = getNewInstanceDesc(n, NULL, id);


}
/* Add by zhanghao 2016/6/16 TAU 相关信息 end */

