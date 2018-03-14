#include <stdio.h>
#include <string.h>  
#include <stdlib.h>  

#include "lte.h"
#include "lte_log.h"
#include "lte_common.h"
#include "ltecontrolclient.h"

static int g_connect_flag = 0;
static int g_connect_fail = 0;
static int g_network_prepare_flag = 0;

static LTE_RET_E altair_pin_state(void *pvData, int iDataLen)
{
	if (pvData == NULL)
		return LTE_FAIL;

	LTE_PIN_STATE_T *pstPinState = (LTE_PIN_STATE_T *)pvData;
	ALTAIR_PIN_STATE_T stPin;

	int argc = 8;
	char *argv[8] = {"-s", ALTAIR_SERVER, "-p", ALTAIR_PORT, "-M", "12291", "-S", "1"};

	if(!altair_control(argc, argv, (void *)&stPin)){
		LTE_LOG(LTE_LOG_ERR, "get pin state fail");
		return LTE_FAIL;
	}

	switch(stPin.enPinState){
		case ALTAIR_SIMREADY:
			pstPinState->enState = PIN_STATE_READY;
			break;
		case ALTAIR_PINLOCKED:
			pstPinState->enState = PIN_STATE_PIN;
			break;
		case ALTAIR_PUKLOCKED:
			pstPinState->enState = PIN_STATE_PUK;
			break;
		default:
			LTE_LOG(LTE_LOG_ERR, "other PIN state");
			return LTE_FAIL;
	}
	return LTE_OK;
}

static LTE_RET_E altair_pin_switch(void *pvData, int iDataLen)
{
	if (pvData == NULL)
		return LTE_FAIL;

	LTE_PIN_SWITCH_T *pstPinSwitch = (LTE_PIN_SWITCH_T *)pvData;

	int argc = 10;
	char *argv[10] = {"-s", ALTAIR_SERVER, "-p", ALTAIR_PORT, "-M", "8193", "-u", NULL, "-i", NULL};

	if(pstPinSwitch->enSwitch)
		argv[7] = "1";
	else
		argv[7] = "0";

	argv[9] = pstPinSwitch->acPin;

	if(altair_control(argc, argv, NULL))
		return LTE_OK;
	else
		return LTE_FAIL;

}

static LTE_RET_E altair_pin_switch_get(void *pvData, int iDataLen)
{
    ALTAIR_PIN_SW_STATE_T stPinSw = {0};
    LTE_PIN_SW_STATE_T *pstPinSwState = (LTE_PIN_SW_STATE_T*)pvData;
    
	if (pvData == NULL)
		return LTE_FAIL;

	int argc = 6;
	char *argv[6] = {"-s", ALTAIR_SERVER, "-p", ALTAIR_PORT, "-M", "8197"};

	if(!altair_control(argc, argv, &stPinSw))
    {
		return LTE_FAIL;
    }   

    switch(stPinSw.enPinSwState){
    	case ALTAIR_PIN_CLOSE:
    		pstPinSwState->enState = PIN_CLOSE;
    		break;
    	case ALTAIR_PIN_OPEN:
    		pstPinSwState->enState = PIN_OPEN;
    		break;
    	default:
    		LTE_LOG(LTE_LOG_ERR, "other PIN sw state");
    		return LTE_FAIL;
    }
	return LTE_OK;
}
static LTE_RET_E altair_pin_unlock(void *pvData, int iDataLen)
{
	if (pvData == NULL)
		return LTE_FAIL;

	LTE_PIN_T *pstPin = (LTE_PIN_T *)pvData;

	int argc = 10;
	char *argv[10] = {"-s", ALTAIR_SERVER, "-p", ALTAIR_PORT, "-M", "8193", "-u", "2", "-i", NULL};

	argv[9] = pstPin->acPin;

	if(altair_control(argc, argv, NULL))
		return LTE_OK;
	else
		return LTE_FAIL;
}

static LTE_RET_E altair_pin_alter(void *pvData, int iDataLen)
{
	if (pvData == NULL)
		return LTE_FAIL;

	LTE_PIN_ALTER_T *pstPinAlter = (LTE_PIN_ALTER_T *)pvData;

	int argc = 12;
	char *argv[12] = {"-s", ALTAIR_SERVER, "-p", ALTAIR_PORT, "-M", "8193", "-u", "3", "-i", NULL, "-E", NULL};

	argv[9] = pstPinAlter->acOldPin;
	argv[11] = pstPinAlter->acNewPin;

	if(altair_control(argc, argv, NULL))
		return LTE_OK;
	else
		return LTE_FAIL;
}

static LTE_RET_E altair_signal_get(void *pvData, int iDataLen)
{
	if (pvData == NULL)
		return LTE_FAIL;

	int rsrp, i, j;
	LTE_SIGNAL_T *pstSignal = (LTE_SIGNAL_T *)pvData;
	ALTAIR_LTE_INFO_T stLte;

	int argc = 8;
	char *argv[8] = {"-s", ALTAIR_SERVER, "-p", ALTAIR_PORT, "-M", "12289", "-G", "4"};

	if(!altair_control(argc, argv, (void *)&stLte)){
		LTE_LOG(LTE_LOG_ERR, "get device info fail");
		return LTE_FAIL;
	}
	if(!strcmp(stLte.rsrp, "UNKNOWN")){
		LTE_LOG(LTE_LOG_ERR, "check SIM card first ...");
		pstSignal->uiSignal = 0;
		return LTE_FAIL;
	}

	i = 0;
	j = 0;
	while(stLte.rsrp[i] != ' ' && stLte.rsrp[i] != '\0')
		i++;
	i++;
	while(stLte.rsrp[i] != ' ' && stLte.rsrp[i] != '\0')
		stLte.rsrp[j++] = stLte.rsrp[i++];
	stLte.rsrp[j] = '\0';
	
	rsrp = (stLte.rsrp[0] == '-' ? -1*atoi(stLte.rsrp+1) : atoi(stLte.rsrp));
    if(rsrp == 99)
    {
        pstSignal->uiSignal = 0;
    }
    else if (rsrp > -65)
		pstSignal->uiSignal = 5;
	else if (rsrp >-75)
		pstSignal->uiSignal = 4;
	else if (rsrp >-85)
		pstSignal->uiSignal = 3;
	else if (rsrp >-95)
		pstSignal->uiSignal = 2;
	else 
		pstSignal->uiSignal = 1;

	return LTE_OK;
}

static LTE_RET_E altair_nettype_set(void *pvData, int iDataLen)
{
	/* only support LTE, need not to configure */
	LTE_NET_TYPE_T *pstNettype = (LTE_NET_TYPE_T *)pvData;

	if (pstNettype->enNetType != LTE_4G_ONLY){
		LTE_LOG(LTE_LOG_ERR, "altair only support LTE mode [4G]");
		return LTE_FAIL;
	}

	return LTE_OK;
}

static LTE_RET_E altair_nettype_get(void *pvData, int iDataLen)
{
	if(pvData == NULL)
		return LTE_FAIL;

	LTE_NET_TYPE_T *pstNettype = (LTE_NET_TYPE_T *)pvData;

	/* only support LTE */
	pstNettype->enNetType = LTE_4G_ONLY;
	pstNettype->enNetTypePri = LTE_NONE;
	
	return LTE_OK;
}

static LTE_RET_E altair_linkstate_get(void *pvData, int iDataLen)
{
	if(pvData == NULL)
		return LTE_FAIL;

	LTE_LINKSTATE_T *pstLinkState = (LTE_LINKSTATE_T *)pvData;
	if (g_connect_fail){
		pstLinkState->enLinkstate = LTE_LINKSTATE_CONNECT_FAIL;
		return LTE_OK;
	}
	
	ALTAIR_LINK_STATE_T stLink;

	int argc = 8;
	char *argv[8] = {"-s", ALTAIR_SERVER, "-p", ALTAIR_PORT, "-M", "12291", "-S", "2"};

	if(!altair_control(argc, argv, (void *)&stLink)){
		LTE_LOG(LTE_LOG_ERR, "get link state fail");
		pstLinkState->enLinkstate = LTE_LINKSTATE_DISCONNECT;
		return LTE_FAIL;
	}

	switch(stLink.enLinkState){
		case ALTAIR_CONNECTED:
			pstLinkState->enLinkstate = LTE_LINKSTATE_CONNECTED;
			g_connect_flag = 2;
			break;
		case ALTAIR_CONNECTING:
			pstLinkState->enLinkstate = LTE_LINKSTATE_CONNECTING;
			g_connect_flag = 1;
			break;
		default:
			pstLinkState->enLinkstate = LTE_LINKSTATE_DISCONNECT;
			g_connect_flag = 0;
			break;
	}

	return LTE_OK;
}

static LTE_RET_E altair_network_prepare(void *pvData, int iDataLen)
{
	LTE_APN_T *pstApn = (LTE_APN_T *)pvData;
	
	int apn_argc = 16;
	/* -p 参数: 0 -> ipv4 ; 1 -> ipv6 ; 2 -> ipv4&ipv6 */
	char *apn_argv[16] = {"-s", ALTAIR_SERVER, "-p", ALTAIR_PORT, "-M", "4101", "-I", "1", "-V", "1", "-j", "1", "-P", "2", "-A", NULL};
	apn_argv[15] = pstApn->acApn;
   
	if(!g_network_prepare_flag && !altair_control(apn_argc, apn_argv, NULL)){
		LTE_LOG(LTE_LOG_ERR, "set apn fail");
		g_connect_fail = 1;
		g_connect_flag = 0;
		return LTE_FAIL;
	}
	g_network_prepare_flag++;
	if (g_network_prepare_flag >= DEFAULT_SEARCH_NET_TIMES -1)
		g_network_prepare_flag = 0;
	
	ALTAIR_LTE_INFO_T stLte;

	int sig_argc = 8;
	char *sig_argv[8] = {"-s", ALTAIR_SERVER, "-p", ALTAIR_PORT, "-M", "12289", "-G", "4"};

	if(altair_control(sig_argc, sig_argv, (void *)&stLte)){
		if(stLte.rsrp[4] != 'N' && stLte.rsrp[6] != 'A')
			return LTE_OK;
	}
	LTE_LOG(LTE_LOG_ERR, "network prepare fail ");
	return LTE_FAIL;
}

static LTE_RET_E altair_connect(void *pvData, int iDataLen)
{
    /*altered by caibing 2014-09-16 begin*/
    char cDeviceMode = '0';
    FILE *pFile = NULL;
    char acFileName[64] = {"/tmp/device_mode.tmp"};
    char acBuf[32] = {0};
    /*altered by caibing 2014-09-16 end*/
#if 1
    if (g_connect_flag){
		/* connecting or connected, don't do it again */
		return LTE_OK;
	}
#endif
	g_connect_flag = 1;

	int argc = 7;
	char *argv[7] = {"-s", ALTAIR_SERVER, "-p", ALTAIR_PORT, "-M", "16385", "-L"};

	if(altair_control(argc, argv, NULL))
    {
        /*altered by caibing 2014-09-16*/
        //system("ifconfig usb0 0.0.0.0");

        system("kill -9 $(pidof "DHCLIENT")");

        system("killall dhclient");

        system("kill -9 $(pidof "DHCPC")");

        system("killall udhcpc");    
		//system_call("udhcpc -i usb0 -s /etc/udhcpc.script &"); 
		    /*altered by caibing 2014-09-16 begin*/
        if (NULL != (pFile = fopen(acFileName, "rb")))
        {
            while (fgets(acBuf, sizeof(acBuf), pFile) != NULL )
            {     
                cDeviceMode = acBuf[0];
                memset(acBuf,0,sizeof(acBuf));
            }
            fclose(pFile);
        }

        if ( '0' == cDeviceMode) //fat
        {
            system_call("udhcpc -i usb0 -s /etc/udhcpc.script &");
        }
        else
        {
            //system("dhclient -p 68 usb0 -cf /var/db/dhclient.conf &");
        }   
        /*altered by caibing 2014-09-16 end*/

		//system_call("udhcpc -i usb0 -s /etc/udhcpc.script &");
		g_connect_flag = 2;
		g_connect_fail = 0;
		g_network_prepare_flag = 0;
		return LTE_OK;
	}
	else{
		g_connect_fail = 1;
		g_connect_flag = 0;
		g_network_prepare_flag = 0;
		return LTE_FAIL;
	}
}

static LTE_RET_E altair_disconnect(void *pvData, int iDataLen)
{
    /*altered by caibing 2014-09-23*/
    g_network_prepare_flag = 0;
	g_connect_flag = 0;

	int argc = 7;
	char *argv[7] = {"-s", ALTAIR_SERVER, "-p", ALTAIR_PORT, "-M", "16385", "-D"};

	//system_call("killall -9 udhcpc");
    /*altered by caibing 2014-09-16*/
    //system("ifconfig usb0 0.0.0.0");

    system("kill -9 $(pidof "DHCLIENT")");

    system("killall dhclient");

    system("kill -9 $(pidof "DHCPC")");

    system("killall udhcpc");    

	system_call("ifconfig usb0 "ALTAIR_DEFAULT_CLIENT_IP" up");
	
	if(altair_control(argc, argv, NULL)){
		g_connect_fail = 0;
		return LTE_OK;
	}
	else
		return LTE_FAIL;
}

static LTE_RET_E altair_imei_get(void *pvData, int iDataLen)
{
	if (pvData == NULL)
		return LTE_FAIL;

	LTE_IMEI_T *pstImei = (LTE_IMEI_T *)pvData;
	ALTAIR_DEV_INFO_T stDev;

	int argc = 8;
	char *argv[8] = {"-s", ALTAIR_SERVER, "-p", ALTAIR_PORT, "-M", "12289", "-G", "1"};

	if(!altair_control(argc, argv, (void *)&stDev)){
		LTE_LOG(LTE_LOG_ERR, "get device info fail");
		return LTE_FAIL;
	}

	memcpy(pstImei->acImei, stDev.imei, sizeof(pstImei->acImei));
	return LTE_OK;
}

static LTE_RET_E altair_cellid_get(void *pvData, int iDataLen)
{
	if (pvData == NULL)
		return LTE_FAIL;

	LTE_CELLID_T *pstCellId = (LTE_CELLID_T *)pvData;
	ALTAIR_LTE_INFO_T stLte;

	int argc = 8;
	char *argv[8] = {"-s", ALTAIR_SERVER, "-p", ALTAIR_PORT, "-M", "12289", "-G", "4"};

	if(!altair_control(argc, argv, (void *)&stLte)){
		LTE_LOG(LTE_LOG_ERR, "get device info fail");
		return LTE_FAIL;
	}

	memcpy(pstCellId->acCellID, stLte.cellId, sizeof(pstCellId->acCellID));
	return LTE_OK;
}
static LTE_RET_E altair_imsi_get(void *pvData, int iDataLen)
{
    LTE_IMSI_T *pstImsiInfo = (LTE_IMSI_T *)pvData;
    ALTAIR_DEV_INFO_T stLteDeviceInfo;
    int i, j;

    int argc = 8;
    char *argv[8] = {"-s", ALTAIR_SERVER, "-p", ALTAIR_PORT, "-M", "12289", "-G", "1"};

    
    if (pvData == NULL)
            return LTE_FAIL;

    if(!altair_control(argc, argv, (void *)&stLteDeviceInfo)){
        LTE_LOG(LTE_LOG_ERR, "get device info fail");
        return LTE_FAIL;
    }
    
    if(!strcmp(stLteDeviceInfo.imsi, "UNKNOWN"))
    {
		LTE_LOG(LTE_LOG_ERR, "get device info [imsi] fail");
	}
   
    strcpy(pstImsiInfo->acIMSI, stLteDeviceInfo.imsi);
    
    return LTE_OK;

}

static LTE_RET_E altair_sim_state_get(void *pvData, int iDataLen)
{
	if (pvData == NULL)
		return LTE_FAIL;

	LTE_SIM_STATE_T *pstSimState = (LTE_SIM_STATE_T *)pvData;
	ALTAIR_PIN_STATE_T stPin;

	int argc = 8;
	char *argv[8] = {"-s", ALTAIR_SERVER, "-p", ALTAIR_PORT, "-M", "12291", "-S", "1"};

	if(!altair_control(argc, argv, (void *)&stPin)){
		LTE_LOG(LTE_LOG_ERR, "get sim state fail");
		return LTE_FAIL;
	}

	if (stPin.enPinState == ALTAIR_NOSIM)
		pstSimState->enSimState = LTE_SIM_NOT_FOUND;
	else
		pstSimState->enSimState = LTE_SIM_READY;
	
	return LTE_OK;
}

static LTE_RET_E altair_lte_info_get(void *pvData, int iDataLen)
{
	if (pvData == NULL)
		return LTE_FAIL;

	LTE_INFO_T *pstLteInfo = (LTE_INFO_T *)pvData;
	ALTAIR_LTE_INFO_T stLte;
	int i, j;

	int argc = 8;
	char *argv[8] = {"-s", ALTAIR_SERVER, "-p", ALTAIR_PORT, "-M", "12289", "-G", "4"};

	if(!altair_control(argc, argv, (void *)&stLte)){
		LTE_LOG(LTE_LOG_ERR, "get device info fail");
		return LTE_FAIL;
	}
	if(!strcmp(stLte.cellId, "UNKNOWN")){
		LTE_LOG(LTE_LOG_ERR, "check SIM card first ...");
		return LTE_FAIL;
	}

	memcpy(pstLteInfo->stCellInfo.acCellId, stLte.cellId, sizeof(pstLteInfo->stCellInfo.acCellId));
	memcpy(pstLteInfo->stCellInfo.acBand , stLte.band, sizeof(pstLteInfo->stCellInfo.acBand));
	pstLteInfo->stCellInfo.iChannel = atoi(stLte.earfcn);

	i = 0;
	j = 0;
	while(stLte.rsrp[i] != ' ' && stLte.rsrp[i] != '\0')
		i++;
	i++;
	while(stLte.rsrp[i] != ' ' && stLte.rsrp[i] != '\0')
		stLte.rsrp[j++] = stLte.rsrp[i++];
	stLte.rsrp[j] = '\0';
	pstLteInfo->stCellInfo.iRsrp = (stLte.rsrp[0] == '-' ? -1*atoi(stLte.rsrp+1) : atoi(stLte.rsrp));
	
	i = 0;
	j = 0;
	while(stLte.rsrq[i] != ' ' && stLte.rsrq[i] != '\0')
		i++;
	i++;
	while(stLte.rsrq[i] != ' ' && stLte.rsrq[i] != '\0')
		stLte.rsrq[j++] = stLte.rsrq[i++];
	stLte.rsrq[j] = '\0';
	pstLteInfo->stCellInfo.iRsrq = (stLte.rsrq[0] == '-' ? -1*atoi(stLte.rsrq+1) : atoi(stLte.rsrq));

	i = 0;
	j = 0;
	while(stLte.sinr[i] != ' ' && stLte.sinr[i] != '\0')
		i++;
	i++;
	while(stLte.sinr[i] != ' ' && stLte.sinr[i] != '\0')
		stLte.sinr[j++] = stLte.sinr[i++];
	stLte.sinr[j] = '\0';
	pstLteInfo->stCellInfo.iSinr = atoi(stLte.sinr);

	/* cannot get this value */
	pstLteInfo->stCellInfo.iRssi = 0;

	return LTE_OK;
}

static LTE_RET_E  altair_access_net_get(void *pvData, int iDataLen)
{
	if (pvData == NULL)
		return LTE_FAIL;

	LTE_ACCESS_NETWORK_T *pstAccessNet = (LTE_ACCESS_NETWORK_T *)pvData;
	ALTAIR_LTE_INFO_T stLte;

	int argc = 8;
	char *argv[8] = {"-s", ALTAIR_SERVER, "-p", ALTAIR_PORT, "-M", "12289", "-G", "4"};

	if(!altair_control(argc, argv, (void *)&stLte)){
		LTE_LOG(LTE_LOG_ERR, "get device info fail");
		return LTE_FAIL;
	}
	if(!strcmp(stLte.cellId, "UNKNOWN")){
		pstAccessNet->enAccessNet = NET_NONE;
	}else{
		pstAccessNet->enAccessNet = NET_4G;
	}

	return LTE_OK;
}

static LTE_RET_E altair_bands_get(void *pvData, int iDataLen)
{
	LTE_BANDS_T *pstBands = (LTE_BANDS_T *)pvData;
	/* PL10-04 */
	char bands[] = "3,39,40,41";
    
	if (pvData == NULL)
		return LTE_FAIL;

/*	ALTAIR_DEV_INFO_T stDev;

	int argc = 8;
	char *argv[8] = {"-s", ALTAIR_SERVER, "-p", ALTAIR_PORT, "-M", "12289", "-G", "1"};

	if(!altair_control(argc, argv, (void *)&stDev)){
		LTE_LOG(LTE_LOG_ERR, "get device info fail");
		return LTE_FAIL;
	}

	memcpy(pstBands->bands, stDev.bands, sizeof(pstBands->bands));
*/

	memcpy(pstBands->bands, bands, sizeof(pstBands->bands));
    pstBands->iClcBandAble = CLC_BAND_ENABLE;
	return LTE_OK;
}

static LTE_RET_E altair_clck_bands_set(void *pvData, int iDataLen)
{
	if (pvData == NULL)
		return LTE_FAIL;

	LTE_BANDS_T *pstClckBands = (LTE_BANDS_T *)pvData;
	/* PL10-04 */
	if (strlen(pstClckBands->bands) == 0){
		strcpy(pstClckBands->bands, "3,39,40,41");
	}

	int argc = 8;
	char *argv[8] = {"-s", ALTAIR_SERVER, "-p", ALTAIR_PORT, "-M", "4109", "-q", NULL};

	argv[7] = pstClckBands->bands;

	if(!altair_control(argc, argv, NULL)){
		LTE_LOG(LTE_LOG_ERR, "set clck bands fail");
		return LTE_FAIL;
	}
	return LTE_OK;
}

static LTE_RET_E altair_sup_nettype_get(void *pvData, int iDataLen)
{
    LTE_NET_TYPE_T *pstNets = (LTE_NET_TYPE_T *)pvData;
    //char NetTypeInfo[] = "3G";
    pstNets->enNetType = LTE_4G_ONLY;
    pstNets->enNetTypePri = LTE_NONE;
    return LTE_OK;

}
/*****************************************************************************
 函 数 名  : altair_sms_sup_func
 功能描述  : 获取模块是否支持短信功能
 输入参数  : void *pvData  
             int iDataLen  
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年12月8日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
static LTE_RET_E altair_sms_sup_func(void *pvData, int iDataLen)
{
    int flag = 0;
    LTE_SUPPORT_SMS_T *pstSup = (LTE_SUPPORT_SMS_T *)pvData;
    pstSup->enSupSms = NO_SUP_SMS_FUNC;
    return LTE_OK;

}

static LTE_RET_E altair_apn_set(void *pvData, int iDataLen)
{
	int argc = 16;
    LTE_APN_T *pstApn = (LTE_APN_T*)pvData;
    
	char *argv[16] = {"-s", ALTAIR_SERVER, "-p", ALTAIR_PORT, "-M", "4101", "-l", "1", "-v", "1", "-j", "1", "-p", "1", "-A", NULL};

    if (pvData == NULL)
            return LTE_FAIL;

    argv[15] = pstApn->acApn;
    
    if(!altair_control(argc, argv, NULL))
    {
		LTE_LOG(LTE_LOG_ERR, "set apn fail");
		return LTE_FAIL;
	}
    
	return LTE_OK;
}
static LTE_RET_E altair_version_get(void *pvData, int iDataLen)
{
    LTE_MODULE_VERSION_T *pstVersion = (LTE_MODULE_VERSION_T *)pvData;
    ALTAIR_DEV_INFO_T stLteDeviceInfo;
    int i, j;

    int argc = 8;
    char *argv[8] = {"-s", ALTAIR_SERVER, "-p", ALTAIR_PORT, "-M", "12289", "-G", "1"};

    
    if (pvData == NULL)
            return LTE_FAIL;

    if(!altair_control(argc, argv, (void *)&stLteDeviceInfo)){
        LTE_LOG(LTE_LOG_ERR, "get device info fail");
        return LTE_FAIL;
    }
    
    if(!strcmp(stLteDeviceInfo.softVersion, "UNKNOWN"))
    {
		pstVersion->iVersionIndex = LTE_VER_IND_MAX;
	}
    else
    {
		pstVersion->iVersionIndex = LTE_ALTAIR_LP10_P4;
	}
   
    strcpy(pstVersion->acVersion, stLteDeviceInfo.softVersion);
    
    return LTE_OK;

}

LTE_IOCTL_FUNC_T astAltairTable[] = 
{    
    {LTE_PIN_STATE,         altair_pin_state,           0,0,    "altair_pin_state"       }, 
    {LTE_PIN_SWITCH,        altair_pin_switch,          0,0,    "altair_pin_switch"       }, 
    {LTE_PIN_SW_GET,        altair_pin_switch_get,      0,0,    "altair_pin_switch_get"       },
    {LTE_PIN_UNLOCK,        altair_pin_unlock,          0,0,    "altair_pin_unlock"       }, 
    {LTE_PIN_ATTEMPTS,      NULL,           0,0,    NULL       }, 
    {LTE_PIN_ALTER,        altair_pin_alter,           0,0,    "altair_pin_alter"       }, 
    {LTE_SIGNAL_GET,        altair_signal_get,           0,0,    "altair_signal_get"       }, 
    {LTE_POWER_ON,          NULL,           0,0,    "altair_power_on"       }, 
    {LTE_POWER_OFF,         NULL,           0,0,    "altair_power_off"       }, 
    {LTE_NETTYPE_SET,      altair_nettype_set,           0,0,    "altair_nettype_set"       }, 
    {LTE_NETTYPE_GET,       altair_nettype_get,           0,0,    "altair_nettype_get"       }, 
    {LTE_PLMN_REGISTER,     NULL,           0,0,    NULL       }, 
    {LTE_PREFER_PLMN_SET,   NULL,           0,0,    NULL       }, 
    {LTE_NETWORK_PREPARE,            altair_network_prepare,           0,0,    "altair_network_prepare"       }, 
    {LTE_NETWORK_STATUS_GET,NULL,           0,0,    NULL       },
    {LTE_CONNECT,            altair_connect,           0,0,    "altair_connect"       }, 
    {LTE_DISCONNECT,        altair_disconnect,           0,0,    "altair_disconnect"       }, 
    {LTE_LINKSTATE_GET,     altair_linkstate_get,           0,0,    "altair_linkstate_get"       }, 
    {LTE_IMEI_GET,          altair_imei_get,           0,0,    "altair_imei_get"       }, 
    {LTE_CELLID_GET,        altair_cellid_get,           0,0,    "altair_cellid_get"       }, 
    {LTE_ICCID_GET,         NULL,           0,0,    NULL       },
    {LTE_IMSI_GET,          altair_imsi_get,           0,0,    "altair_imsi_get"       },
    {LTE_SIM_STATE_GET,        altair_sim_state_get,           0,0,    "altair_sim_state_get"       }, 
    {LTE_INFO_GET,        altair_lte_info_get,           0,0,    "altair_lte_info_get"       }, 
    {LTE_ACCESS_NET_GET,        altair_access_net_get,           0,0,    "altair_access_net_get"       }, 
    {LTE_BANDS_GET,        altair_bands_get,           0,0,    "altair_bands_get"       }, 
    {LTE_CLCK_BANDS_SET,        altair_clck_bands_set,           0,0,    "altair_clck_bands_set"       },
    {LTE_CLCK_BANDS_CLOSE,  NULL,            0,0,       NULL       },
    {LTE_CLCK_CELL_GET,     NULL,                        0,0,    NULL   },
    {LTE_CLCK_CELL_SET,        NULL,           0,0,    NULL       },
    {LTE_OPER_INFO_GET,        NULL,           0,0,    NULL          }, 
    {LTE_NETTYPE_INFO_GET,     altair_sup_nettype_get,           0,0,    "altair_sup_nettype_get"          },
    {LTE_GPS_START,        NULL,           0,0,    NULL       }, 
    {LTE_GPS_STOP,        NULL,           0,0,    NULL       }, 
    {LTE_GPS_DATE_GET,        NULL,           0,0,    NULL       }, 
    {LTE_GPS_POSITION_INFO_GET,        NULL,           0,0,    NULL       }, 
    /* BEGIN: Added by wujian, 2014/11/13   PN:?????????? */
    {LTE_SMS_CENT_CODE_SET, NULL, 0, 0, NULL },
    {LTE_SMS_MODE__SET, NULL, 0, 0, NULL   },
    {LTE_SMS_FUNC_INIT, NULL, 0, 0, NULL   },
    {LTE_SMS_RECV, NULL, 0, 0, NULL   },
    {LTE_SMS_SEND, NULL, 0, 0, NULL   },
    {LTE_SMS_DELE, NULL, 0, 0, NULL   },
    {LTE_SMS_SUPP, altair_sms_sup_func, 0, 0, "altair_sms_sup_func"   },
    /* END:   Added by wujian, 2014/11/13   PN:?????????? */
    {LTE_IPV6_PDP_SET, NULL, 0, 0, NULL   },
    /* BEGIN: Added by wujian, 2014/11/17   PN:????apn?? */
    {LTE_APN_SET,           altair_apn_set, 0,0, "altair_apn_set"   },
    /* END:   Added by wujian, 2014/11/17   PN:????apn?? */
    {LTE_VERSION_GET,       altair_version_get,           0,0,    "altair_version_get"       },
    {LTE_ENODEBID_GET,      NULL,     0,0,     NULL    }, 
    {LTE_TAC_GET,           NULL,     0,0,     NULL    },
    {LTE_LCID_GET,          NULL,     0,0,     NULL    },

    {LTE_CMD_MAX,           NULL,           0,0,    NULL       }, 
};

