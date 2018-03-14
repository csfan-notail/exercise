#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lte.h"

#define IP_FMT	    "%u.%u.%u.%u"
#define IP_ARG(x) ((UINT8*)(x))[0], ((UINT8*)(x))[1], ((UINT8*)(x))[2], ((UINT8*)(x))[3]
#define IP_SSCAN_ARG(x) &((UINT8*)(x))[0], &((UINT8*)(x))[1], &((UINT8*)(x))[2], &((UINT8*)(x))[3]
#define IPV6_FMT	"%u.%u.%u.%u.%u.%u.%u.%u.%u.%u.%u.%u.%u.%u.%u.%u"
#define IPV6_ARG(x) ((UINT8*)(x))[0], ((UINT8*)(x))[1], ((UINT8*)(x))[2], ((UINT8*)(x))[3],\
                    ((UINT8*)(x))[4], ((UINT8*)(x))[5], ((UINT8*)(x))[6], ((UINT8*)(x))[7], \
                    ((UINT8*)(x))[8], ((UINT8*)(x))[9], ((UINT8*)(x))[10], ((UINT8*)(x))[11],\
                    ((UINT8*)(x))[12], ((UINT8*)(x))[13], ((UINT8*)(x))[14], ((UINT8*)(x))[15]
                    
#define IPV6_SSCAN_ARG(x) &((UINT8*)(x))[0], &((UINT8*)(x))[1], &((UINT8*)(x))[2], &((UINT8*)(x))[3],\
                    &((UINT8*)(x))[4], &((UINT8*)(x))[5], &((UINT8*)(x))[6], &((UINT8*)(x))[7], \
                    &((UINT8*)(x))[8], &((UINT8*)(x))[9], &((UINT8*)(x))[10], &((UINT8*)(x))[11],\
                    &((UINT8*)(x))[12], &((UINT8*)(x))[13], &((UINT8*)(x))[14], &((UINT8*)(x))[15]
                    
typedef struct 
{
	const char *pcName;
	int       (*pFunc)(int argc, char ** argv);
	const char *pcHelp;
} LTE_CTL_CMD_T;


int ltectl_get_pinstate(int argc, char **argv)
{
	LTE_PIN_STATE_T stPinState;
	printf("Pin state : ");
	if ( LTE_OK != lte_ioctl(LTE_PIN_STATE, (void*)(&stPinState), sizeof(LTE_PIN_STATE_T)))
	{
		printf("fail\n");
    	return 1;
	}
	switch(stPinState.enState)
	{
        case PIN_STATE_READY:
            printf(" ready\n");
            break;
        case PIN_STATE_PIN:
            printf(" SIM pin\n");
            break;
        case PIN_STATE_PUK:
            printf(" SIM puk\n");
            break;
        default :  
        	printf(" unkown\n");
            break;
	}
    return 0;
}

int ltectl_unlock_pin(int argc, char **argv)
{
	LTE_PIN_T stPin;
    if(argc != 3)
    {
        printf("%s argc num invalid, argc:%d\n",  __func__, argc);
        return 1;
    }
    printf("pin : %s  ", argv[2]);
	printf("unlock pin : ");
	strcpy(stPin.acPin, argv[2]);

	if (LTE_OK != lte_ioctl(LTE_PIN_UNLOCK, (void*)(&stPin), sizeof(LTE_PIN_T))){
		printf("fail\n");
        return 1;
	}else{
		printf("ok\n");
        return 0;
	}
}
int ltectl_pin_switch(int argc, char **argv)
{
	LTE_PIN_SWITCH_T stPinSw;
    if(argc != 4)
    {
        printf("%s argc num invalid, argc:%d\n",  __func__, argc);
        return 1;
    }
    printf("pin sw: %s   ", argv[2]);
    printf("pin : %s\n", argv[3]);
	printf("ltectl_pin_switch: ");
	strcpy(stPinSw.acPin, argv[3]);
    stPinSw.enSwitch = atoi(argv[2]);

	if (LTE_OK != lte_ioctl(LTE_PIN_SWITCH, (void*)(&stPinSw), sizeof(LTE_PIN_SWITCH_T))){
		printf("fail\n");
        return 1;
	}else{
		printf("ok\n");
        return 0;
	}
}

int ltectl_pin_sw_get(int argc, char **argv)
{
	LTE_PIN_SW_STATE_T stPinSwState;

    printf("pin sw get: ");

	if (LTE_OK != lte_ioctl(LTE_PIN_SW_GET, (void*)(&stPinSwState), sizeof(LTE_PIN_SW_STATE_T))){
		printf("fail\n");
        return 1;
	}else{
	    printf("%d\n", stPinSwState.enState);
        
	    if(PIN_CLOSE == stPinSwState.enState)
        {
            printf("pin close\n");
        }
        else if(PIN_OPEN == stPinSwState.enState)
        {
            printf("pin open\n");
        }
        return 0;
	}
}

int ltectl_pin_alter(int argc, char **argv)
{
	LTE_PIN_ALTER_T stPinAlter;
    LTE_PIN_T stPin;
    if(argc != 4)
    {
        printf("%s argc num invalid, argc:%d\n",  __func__, argc);
        return 1;
    }
    printf("pin old passWD: %s   ", argv[2]);
    printf("pin new passWD: %s\n", argv[3]);
	printf("ltectl_pin_alter:\n");
    strcpy(stPin.acPin, argv[2]);
	strcpy(stPinAlter.acOldPin, argv[2]);
    strcpy(stPinAlter.acNewPin, argv[3]);
    if(LTE_OK != lte_pin_unlock(&stPin))
    {
        printf("unlock pin fail\n");
        return 1;
    }
    sleep(2);
    if(LTE_OK != lte_pin_alter(&stPinAlter))
    {
        printf("alter pin fail\n");
        return 1;
    }
    else
    {
        printf("alter pin ok\n");
        return 0;
    }
    #if 0
	if (LTE_OK != lte_ioctl(LTE_PIN_ALTER, (void*)(&stPinAlter), sizeof(LTE_PIN_ALTER_T))){
		printf("fail\n");
        return 1;
	}else{
		printf("ok\n");
        return 0;
	}
    #endif
}


int ltectl_get_nettype(int argc, char **argv)
{
	LTE_NET_TYPE_T stNettype = {0,0};
	printf("get net-type : ");
	if (LTE_OK != lte_ioctl(LTE_NETTYPE_GET, &stNettype, sizeof(LTE_NET_TYPE_T))){
		printf("fail\n");
        return 1;
	}else{
		//printf("nettype %d,pri %d\n", stNettype.enNetType,stNettype.enNetTypePri);
                printf("%d\n", stNettype.enNetType);
        return 0;
	}
}
int ltectl_set_nettype(int argc, char **argv)
{
	LTE_NET_TYPE_T stNettype = {0,0};
    if(argc == 4)
    {
        stNettype.enNetType = atoi(argv[2]);
        stNettype.enNetTypePri = atoi(argv[3]);
    }
    else if(argc == 3)
    {
        stNettype.enNetType = atoi(argv[2]);
    }
    else
    {
        printf("%s argc num invalid, argc:%d\n",  __func__, argc);
        return 1;
    }

	printf("set net-type : ");
	if (LTE_OK != lte_ioctl(LTE_NETTYPE_SET, &stNettype, sizeof(LTE_NET_TYPE_T))){
		printf("fail\n");
        return 1;
	}else{
		printf("OK!\n");
        return 0;
	}
}
int ltectl_nw_status_get(int argc, char **argv)
{
    LTE_NETWORK_PREPARE_T  stNWStatus;
    
    printf("prepare get: ");
    if(LTE_OK != lte_network_status_get(&stNWStatus))
    {
        printf("%s\n", stNWStatus.acNetWorkStatus);    
        printf("net work status is not good!\n");
        return 1;
    }
    else
    {
        printf("%s\n", stNWStatus.acNetWorkStatus); 
		printf(" OK!\n");
        return 0;
    }
}
int ltectl_connect(int argc, char **argv)
{
	LTE_APN_T stApn;
	strcpy(stApn.acApn, argv[2]);

	printf("connect : ");
	if (LTE_OK != lte_connect(&stApn, 1)){
		printf("fail\n");
        return 1;
	}else{
		printf(" OK!\n");
        return 0;
    }
}

int ltectl_connect_async(int argc, char **argv)
{
	LTE_APN_T stApn;
	strcpy(stApn.acApn, argv[2]);

	printf("connect : ");
	if (LTE_WAIT != lte_connect_async(&stApn, 1)){
		printf("fail\n");
        return 1;
	}else{
		printf(" OK!\n");
        return 0;
	}
}

int ltectl_disconnect(int argc, char **argv)
{
	printf("disconnect : ");
	if (LTE_OK != lte_disconnect()){
		printf("fail\n");
        return 1;
	}else{
		printf(" OK!\n");
        return 0;
	}
}
int ltectl_disconnect_ipv6(int argc, char **argv)
{
    char acIPFamily[16] = {0};
    if(3 != argc)
    {
        printf("param is not ok!");
        return 1;
    }
    strncpy(acIPFamily, argv[2], sizeof(acIPFamily));
	printf("disconnect : ");
	if (LTE_OK != lte_disconnect_ipv6(acIPFamily)){
		printf("fail\n");
        return 1;
	}else{
		printf(" OK!\n");
        return 0;
	}
}

int ltectl_poweron(int argc, char **argv)
{
	printf("power on : ");
	if (LTE_OK != lte_power_on()){
		printf("fail\n");
        return 1;
	}else{
		printf(" OK!\n");
        return 0;
    }
}

int ltectl_poweroff(int argc, char **argv)
{
	printf("power off : ");
	if (LTE_OK != lte_power_off()){
		printf("fail\n");
        return 1;
	}else{
		printf(" OK!\n");
        return 0;
    }
}

int ltectl_elecSignal_get(int argc, char **argv)
{
	LTE_SIGNAL_T stSignal;
	printf("get elec signal : ");
	if (LTE_OK != lte_signal_get(&stSignal)){
		printf(" failed!\n");
        return 1;
	}else{
		printf(" %u\n", stSignal.uiSignal);
        return 0;
	}
}

int ltectl_imei_get(int argc, char **argv)
{
	LTE_IMEI_T stImei;
	printf("get imei:");
	if (LTE_OK != lte_imei_get(&stImei)){
		printf("fail\n");
        return 1;
	}else{
		printf("%s\n", stImei.acImei);
        return 0;
	}
}

int ltectl_cellid_get(int argc, char **argv)
{
    LTE_CELLID_T stCellid;
	printf("get cellid : ");

	if (LTE_OK != lte_cellid_get(&stCellid)){
		printf("fail\n");
        return 1;
	}else{
		printf(" %s\n", stCellid.acCellID);
        return 0;
	}
}
int ltectl_iccid_get(int argc, char **argv)
{
	LTE_ICCID_T stIccid;
	printf("get iccid :");
	if (LTE_OK != lte_iccid_get(&stIccid)){
		printf("fail\n");
        return 1;
	}else{
		printf("%s\n", stIccid.acICCID);
        return 0;
	}
}
int ltectl_imsi_get(int argc, char **argv)
{
	printf("get imsi :");
	LTE_IMSI_T stImsi;
	if (LTE_OK != lte_imsi_get(&stImsi)){
		printf("fail\n");
        return 1;
	}else{
		printf("%s\n", stImsi.acIMSI);
        return 0;
	}
}

int ltectl_sim_state_get(int argc, char **argv)
{
	LTE_SIM_STATE_T stSimState;
    
	printf("get SIM state : ");
	if (LTE_OK != lte_sim_state_get(&stSimState)){
		printf("fail\n");
        return 1;
	}else{
		printf(" %d\n", stSimState.enSimState);
        return 0;
	}
}

int ltectl_lte_stateinfo_get(int argc, char **argv)
{
	LTE_INFO_T stLteInfo = {0};
	printf("##### Get LTE info :");
	if(LTE_OK != lte_info_get(&stLteInfo)){
		printf("Fail #####\n");
        return 1;
	}else{
		printf("OK #####\n");
	}

	printf("    CELLID    : %s\n", stLteInfo.stCellInfo.acCellId);
    printf("    PHYCELLID : %s\n", stLteInfo.stCellInfo.acPhyCeillid);
	printf("    RSSI      : %d dbm\n", stLteInfo.stCellInfo.iRssi);
	printf("    RSRP      : %d dbm\n", stLteInfo.stCellInfo.iRsrp);
	printf("    RSRQ      : %d db\n", stLteInfo.stCellInfo.iRsrq);
	printf("    SINR      : %d db\n", stLteInfo.stCellInfo.iSinr);
	printf("    BAND      : %s\n", stLteInfo.stCellInfo.acBand);
	printf("    CHANNEL   : %d\n", stLteInfo.stCellInfo.iChannel);
    return 0;
}

int ltectl_linkstate_get(int argc, char **argv)
{
	printf("get LTE linkstate : ");
	LTE_LINKSTATE_T stLink = {0};

	if (LTE_OK != lte_linkstate_get(&stLink)){
		printf("fail\n");
        return 1;
	}else{
		printf("%d\n", stLink.enLinkstate);
        return 0;
	}
}

int ltectl_access_net_get(int argc, char **argv)
{
	printf("get LTE access network : ");
	LTE_ACCESS_NETWORK_T stNet;

	if (LTE_OK != lte_access_net_get(&stNet)){
		printf("fail\n");
        return 1;
	}
    else
    {
		switch(stNet.enAccessNet){
			case NET_NONE:
				printf("unknow\n");
				break;
            case NET_2G:
				printf("2G\n");
				break;
			case NET_3G:
				printf("3G\n");
				break;
			case NET_4G:
				printf("4G\n");
				break;
            default:
                break;
		}
	}
    return 0;
}

int ltectl_bands_get(int argc, char **argv)
{
	printf("get LTE support bands : ");
	LTE_BANDS_T stBands;

	if (LTE_OK != lte_bands_get(&stBands))
	{
		printf("fail\n");
        return 1;
	}else{
		printf("%s\n", stBands.bands);
        return 0;
	}
}

int ltectl_clck_bands_set(int argc, char **argv)
{
	LTE_BANDS_T stBands;
    
    if(argc != 3)
    {
        printf("%s argc num invalid, argc:%d\n",  __func__, argc);
        return 1;
    }
	printf("set LTE support bands : ");
	strncpy(stBands.bands, argv[2], sizeof(stBands.bands));

	if (LTE_OK != lte_clck_bands_set(&stBands))
	{
		printf("fail\n");
        return 1;
	}else{
		printf("success\n");
        return 0;
	}
}

int ltectl_clck_bands_close(int argc, char **argv)
{
    if(argc != 2)
    {
        printf("%s argc num invalid, argc:%d\n",  __func__, argc);
        return 1;
    }
    if(LTE_OK != lte_clck_bands_close())
    {
        printf("close band fail!\n");
        return 1;
    }
    else
    {
        printf("success\n");
        return 0;
    }
    
}

int ltectl_clckcell_flag_get(int argc, char **argv)
{
    LTE_CLKCELL_ENABLE_T stCellClcFlag;
    
    if(LTE_OK != lte_clck_cell_enable_get(&stCellClcFlag))
    {
        printf("this moudle do not support clck cell!\n");
        return 1;
    }
    else
    {
        if(1 != stCellClcFlag.iClkCellEnable)
        {
            printf("this moudle do not support clck cell!\n");            
        }
        else
        {
            printf("this moudle support clck cell!\n");
        }
        
        return 0;
    }
    
}
int ltectl_clck_cell_set(int argc, char **argv)
{
	LTE_CLK_CELL_T stCell;
    
    if(argc != 5)
    {
        printf("%s argc num invalid, argc:%d\n",  __func__, argc);
        return 1;
    }
	printf("set LTE clk cell : ");
	strncpy(stCell.acFreq, argv[2], sizeof(stCell.acFreq));
    strncpy(stCell.acCellid, argv[3], sizeof(stCell.acCellid));
    stCell.iClkEnable = atoi(argv[4]);

	if (LTE_OK != lte_clck_cell_set(&stCell))
	{
		printf("fail\n");
        return 1;
	}else{
		printf("success\n");
        return 0;
	}
}

int ltectl_gps_start(int argc, char **argv)
{
	printf("get LTE gps start : ");

	if (LTE_OK != lte_gps_func_start())
	{
		printf("fail\n");
        return 1;
	}else{
		printf("OK\n");
        return 0;
	}
}
int ltectl_gps_stop(int argc, char **argv)
{
	printf("get LTE gps stop : ");

	if (LTE_OK != lte_gps_func_stop())
	{
		printf("fail\n");
        return 1;
	}else{
		printf("OK\n");
        return 0;
	}
}
int ltectl_gpsdate_get(int argc, char **argv)
{
	printf("get LTE GPS date : ");
	LTE_GPS_DATE_T stLteGpsDate;

	if (LTE_OK != lte_gps_date_get(&stLteGpsDate))
	{
		printf("fail\n");
        return 1;
	}else{
		printf("%s\n", stLteGpsDate.acDate);
        return 0;
	}
}
int ltectl_gps_postion_get(int argc, char **argv)
{
	printf("get LTE gps postion\n");
	LTE_GPS_POSTION_T stGpsPostion;

	if (LTE_OK != lte_gps_postion_get(&stGpsPostion))
	{
		printf("fail\n");
        return 1;
	}else{
		printf("Latitude: %s\n", stGpsPostion.acLatitude);
        printf("Longitude: %s\n", stGpsPostion.acLongitude);
        printf("Orientation: %s\n", stGpsPostion.acOrientation);
        printf("Velocity: %s\n", stGpsPostion.acVelocity);
        return 0;
	}
}
int ltectl_gps_set_close_tag(int argc, char **argv)
{
    printf("set LTE gps close tag: ");
    int iGpsCloseFlag = 0;
    iGpsCloseFlag = atoi(argv[2]);
    
    if (LTE_OK != set_gps_close_tag(iGpsCloseFlag))
    {
        printf("fail\n");
        return 1;
    }else{
        printf("OK\n");
        return 0;
    }

}
int ltectl_gps_get_close_tag(int argc, char **argv)
{
    printf("get LTE gps close tag: ");
    int iGpsCloseFlag = 0;
    
    if (LTE_OK != get_gps_close_tag(&iGpsCloseFlag))
    {
        printf("fail\n");
        return 1;
    }else{
        printf("%d\n", iGpsCloseFlag);
        return 0;
    }

}
int ltectl_gps_test(int argc, char **argv)
{
    printf("get LTE gps test : ");
    
    if (LTE_OK != lte_gps_test())
    {
        printf("fail\n");
        return 1;
    }else{
        printf("OK\n");
        return 0;
    }

}
int ltectl_get_gps_data(int argc, char **argv)
{
    int iComFd = 0;
    int iTimeOut = 0;
    int iCount = 0;
    int iLoop = 0;
    int iRet = 0;
    LTE_GPS_INFO_T stLteGpsInfo = {0};
    
    if(2 == argc)
    {
        iCount = 5;/* 获取有效数据的次数 */
        iTimeOut = 20;/* 检测数据超时时间 */
    }
    else if(3 == argc)
    {
        iTimeOut = atoi(argv[2]);
    }
    else if(4 == argc)
    {
        iTimeOut = atoi(argv[2]);
        iCount = atoi(argv[3]);
    }
    printf("############GPS START############\n");
    if(LTE_OK != lte_gps_func_start())
    {
        printf("gps start failed!\n");
        return 1;
    }
    printf("LTE gps valid data :\n");

    if(LTE_OK == gps_serial_com_open(&iComFd))
    {   
        for(iLoop = 0; iLoop < iCount; iLoop++)
        {
            if(LTE_OK != gps_valid_data_get(iComFd, iTimeOut, &stLteGpsInfo))
            {
                printf("Get invalid gps data!\n");
                continue;
            }
            else
            {
                printf("Time:%s %s\n", stLteGpsInfo.acDate, stLteGpsInfo.acTime);
                printf("Latitude:%s\n", stLteGpsInfo.acLatitude);
                printf("Longitude:%s\n", stLteGpsInfo.acLongitude);
                printf("Velocity:%s\n", stLteGpsInfo.acVelocity);
                printf("Elevation:%s\n", stLteGpsInfo.acElevation);
                printf("Orientation:%s\n", stLteGpsInfo.acOrientation);
                memset(&stLteGpsInfo, 0, sizeof(LTE_GPS_INFO_T));
            }
        }
    }
    
    gps_serial_com_close(iComFd);
    printf("############GPS  STOP############\n");
    if(LTE_OK != lte_gps_func_stop())
    {
        printf("gps stop failed!\n");
        return 1;
    }

}
int ltectl_all_status_get(int argc, char **argv)
{
    LTE_PIN_STATE_T stPinState;
    LTE_PIN_SW_STATE_T stPinSwState;
    LTE_NET_TYPE_T stNettype = {0,0};
    LTE_SIGNAL_T stSignal;
    LTE_IMEI_T stImei;
    LTE_CELLID_T stCellid;
    LTE_ICCID_T stIccid;
    LTE_INFO_T stLteInfo = {0};
    LTE_CLKCELL_ENABLE_T stCellClcFlag;
    LTE_SIM_STATE_T stSimState;
    printf("###########GET LTE ALL STATUS:###########\n");
    printf("Pin state : ");
	if ( LTE_OK != lte_ioctl(LTE_PIN_STATE, (void*)(&stPinState), sizeof(LTE_PIN_STATE_T)))
	{
		printf("fail\n");
    	return 1;
	}
	switch(stPinState.enState)
	{
        case PIN_STATE_READY:
            printf(" ready\n");
            break;
        case PIN_STATE_PIN:
            printf(" SIM pin\n");
            break;
        case PIN_STATE_PUK:
            printf(" SIM puk\n");
            break;
        default :  
        	printf(" unkown\n");
            break;
	}
    printf("pin sw get: ");

	if (LTE_OK != lte_ioctl(LTE_PIN_SW_GET, (void*)(&stPinSwState), sizeof(LTE_PIN_SW_STATE_T))){
		printf("fail\n");
        return 1;
	}else{
	    printf("%d\n", stPinSwState.enState);
        
	    if(PIN_CLOSE == stPinSwState.enState)
        {
            printf("pin close\n");
        }
        else if(PIN_OPEN == stPinSwState.enState)
        {
            printf("pin open\n");
        }
	}
    printf("get net-type : ");
	if (LTE_OK != lte_ioctl(LTE_NETTYPE_GET, &stNettype, sizeof(LTE_NET_TYPE_T))){
		printf("fail\n");
        return 1;
	}else{
		printf("nettype %d,pri %d\n", stNettype.enNetType,stNettype.enNetTypePri);
	}
    
	printf("get elec signal : ");
	if (LTE_OK != lte_signal_get(&stSignal)){
		printf(" failed!\n");
        return 1;
	}else{
		printf(" %u\n", stSignal.uiSignal);
	}
    printf("get imei : ");
	if (LTE_OK != lte_imei_get(&stImei)){
		printf("fail\n");
        return 1;
	}else{
		printf(" %s\n", stImei.acImei);
	}
    
	printf("get cellid : ");

	if (LTE_OK != lte_cellid_get(&stCellid)){
		printf("fail\n");
        return 1;
	}else{
		printf(" %s\n", stCellid.acCellID);
	}
    
	printf("get iccid :");
	if (LTE_OK != lte_iccid_get(&stIccid)){
		printf("fail\n");
        return 1;
	}else{
		printf("%s\n", stIccid.acICCID);
	}
    
	printf("##### Get LTE info :");
	if(LTE_OK != lte_info_get(&stLteInfo)){
		printf("Fail #####\n");
        return 1;
	}else{
		printf("OK #####\n");
	}

	printf("    CELLID    : %s\n", stLteInfo.stCellInfo.acCellId);
    printf("    PHYCELLID : %s\n", stLteInfo.stCellInfo.acPhyCeillid);
	printf("    RSSI      : %d dbm\n", stLteInfo.stCellInfo.iRssi);
	printf("    RSRP      : %d dbm\n", stLteInfo.stCellInfo.iRsrp);
	printf("    RSRQ      : %d db\n", stLteInfo.stCellInfo.iRsrq);
	printf("    SINR      : %d db\n", stLteInfo.stCellInfo.iSinr);
	printf("    BAND      : %s\n", stLteInfo.stCellInfo.acBand);
	printf("    CHANNEL   : %d\n", stLteInfo.stCellInfo.iChannel);
    if(LTE_OK != lte_clck_cell_enable_get(&stCellClcFlag))
    {
        printf("this moudle do not support clck cell!\n");
        return 1;
    }
    else
    {
        if(1 != stCellClcFlag.iClkCellEnable)
        {
            printf("this moudle do not support clck cell!\n");            
        }
        else
        {
            printf("this moudle support clck cell!\n");
        }
    }
    printf("get SIM state : ");
    if (LTE_OK != lte_sim_state_get(&stSimState)){
        printf("fail\n");
        return 1;
    }else{
        printf(" %d\n", stSimState.enSimState);
    }
    printf("########################################\n");

    return 0;
}
/*****************************************************************************
 函 数 名  : ltectl_oper_info_get
 功能描述  : 获取运营商信息
 输入参数  : int argc     
             char **argv  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年11月12日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
int ltectl_oper_info_get(int argc, char **argv)
{
	printf("get operator info: ");
	LTE_OPER_INFO_T pstOperInfo;
	if (LTE_OK != lte_oper_info_get(&pstOperInfo))
	{
		printf(" failed!\n");
        	return 1;
	}
	else
	{
		printf(" %s\n", pstOperInfo.acOperName);
        	return 0;
	}

}

int ltectl_nets_info_get(int argc, char **argv)
{
	printf("get nettype info: ");
	LTE_NET_TYPE_T pstNetsInfo;
	if (LTE_OK != lte_sup_nettype_get(&pstNetsInfo))
	{
		printf(" failed!\n");
        	return 1;
	}
	else
	{
		printf("nettype: %d , pri: %d\n", pstNetsInfo.enNetType, pstNetsInfo.enNetTypePri);
        	return 0;
	}

}

int ltectl_sms_cent_code_set(int argc, char **argv)
{
        LTE_SMS_CENT_CODE_T stCentCode = {0};
        if(argc != 3)
        {
            printf("%s argc num invalid, argc:%d\n",  __func__, argc);
            return 1;
        }
	printf("set cent mobile number : ");
	strncpy(stCentCode.acSmsCentCode, argv[2], sizeof(stCentCode.acSmsCentCode));

	if (LTE_OK != lte_sms_cent_code_set(&stCentCode))
	{
	    printf("fail\n");
            return 1;
	}
        else{
	    printf("success\n");
        return 0;
	}

}

int ltectl_sms_mode_set(int argc, char **argv)
{
        LTE_SMS_Mode_T stSmsMode = {0};
        if(argc != 3)
        {
            printf("%s argc num invalid, argc:%d\n",  __func__, argc);
            return 1;
        }
	printf("set sms mode : ");
        stSmsMode.iSmsMode = atoi(argv[2]);
	if (LTE_OK != lte_sms_mode__set(&stSmsMode))
	{
	    printf("fail\n");
            return 1;
	}
        else{
	    printf("success\n");
        return 0;
	}

}


int ltectl_sms_init(int argc, char **argv)
{
	printf("sms func init : ");
         if(argc > 2)
        {
            printf("%s argc num invalid, argc:%d\n",  __func__, argc);
            return 1;
        }
	if (LTE_OK != lte_sms_func_init() )
	{
	    printf(" failed!\n");
            return 1;
	}
        else{
	    printf("success\n");
        return 0;
	}

}
int ltectl_sms_read(int argc, char **argv)
{
        int i = 0;
	LTE_SMS_RECV_T pstReadSms = {0};
         if(argc != 3)
        {
            printf("%s argc num invalid, argc:%d\n",  __func__, argc);
            return 1;
        }       
        //memset(&pstReadSms,0,sizeof(LTE_SMS_RECV_T) );
        pstReadSms.iRecvFlag = atoi(argv[2]);
    	printf("get sms info: ");
	if (LTE_OK != lte_sms_recv(&pstReadSms))
	{
	    printf(" failed!\n");
            return 1;
	}
    
        return 0;

}


int ltectl_sms_send(int argc, char **argv)
{
	LTE_SMS_SEND_T pstSend = {0};
        if(argc < 4)
        {
            printf("%s argc num invalid, argc:%d\n",  __func__, argc);
            return 1;
        }
        if(argc ==5)
        {
            pstSend.stSmsSendHead.eSmsHead = 1;
            strncpy(pstSend.stSmsSendHead.acSmsHead, argv[4], sizeof(pstSend.stSmsSendHead.acSmsHead));
        }
        strncpy(pstSend.acRecvMsgCode, argv[2], sizeof(pstSend.acRecvMsgCode));
        strncpy(pstSend.acMsgCont, argv[3], sizeof(pstSend.acMsgCont));
    	printf("send sms: ");
	if (LTE_OK != lte_sms_send(&pstSend))
        {
            printf(" failed!\n");
            return 1;
        }
         else
         {
            printf("success\n");
            return 0;
        }



}


int ltectl_sms_dele(int argc, char **argv)
{
        int i = 0;
	LTE_SMS_DEL_T pstDeleSms = {0};
        if(argc != 3)
        {
            printf("%s argc num invalid, argc:%d\n",  __func__, argc);
            return 1;
        }       
        pstDeleSms.iDelIndex= atoi(argv[2]);
    	printf("dele sms: ");
	if (LTE_OK != lte_sms_dele(&pstDeleSms))
        {
            printf(" failed!\n");
            return 1;
        }
         else
         {
            printf("success\n");
            return 0;
        }

}
int ltectl_sms_supp_func(int argc, char **argv)
{
        int i = 0;
	LTE_SUPPORT_SMS_T pstSuppSms = {0};
        if(argc != 2)
        {
            printf("%s argc num invalid, argc:%d\n",  __func__, argc);
            return 1;
        }       
    	printf("get sms support code: ");
	if (LTE_OK != lte_sms_sup_func(&pstSuppSms))
        {
            printf(" failed!\n");
            return 1;
        }
        else
         {
            printf("%d\n", pstSuppSms.enSupSms);
            return 0;
        }

}
int ltectl_pdp_set(int argc, char **argv)
{
    LTE_CGDCONT_T stPdpSet = {0};
    if(argc == 4)
    {
        stPdpSet.iApnFlag = 1;
        stPdpSet.enPdpStack = atoi(argv[2]);
    	strncpy(stPdpSet.acApn, argv[3], sizeof(stPdpSet.acApn));
    }
    else if(argc == 3)
    {
        stPdpSet.iApnFlag = 0;
        stPdpSet.enPdpStack = atoi(argv[2]);
    }
    else
    {
        printf("%s argc num invalid, argc:%d\n",  __func__, argc);
        return 1;
    }
	printf("set pdp : ");

	if (LTE_OK != lte_pdp_cgdcont_set(&stPdpSet))
	{
	    printf("fail\n");
            return 1;
	}
        else{
	    printf("success\n");
        return 0;
	}

}
int ltectl_apn_set(int argc, char **argv)
{
    LTE_APN_T stApn = {0};
    if(argc != 3)
    {
        printf("%s argc num invalid, argc:%d\n",  __func__, argc);
        return 1;
    }
	printf("set apn : ");
	strncpy(stApn.acApn, argv[2], sizeof(stApn.acApn));

	if (LTE_OK != lte_apn_set(&stApn))
	{
	    printf("fail\n");
            return 1;
	}
        else{
	    printf("success\n");
        return 0;
	}

}
int ltectl_version_get(int argc, char **argv)
{
    LTE_MODULE_TYPE_T stVersionType = {0};
    
    if(argc != 2)
    {
        printf("%s argc num invalid, argc:%d\n",  __func__, argc);
        return 1;
    }
    printf("Get Module Version: ");
	if (LTE_OK != lte_version_get(&stVersionType))
	{
	    printf("\n");
	    printf("Get Fail\n");
        return 1;
	}
    else
    {
        printf("Flag: %s VER:%s\n", stVersionType.acModuleTypeInd, stVersionType.acVersion);
	    printf("Get OK\n");
        return 0;
	}

}

int ltectl_cifs_mount(int argc, char **argv)
{
    LTE_SERVER_USR_T stSmbServerInfo = {0};
    if(argc != 6)
    {
        printf("%s argc num invalid, argc:%d\n",  __func__, argc);
        return 1;
    }
	printf("mount cifs device : ");
	strncpy(stSmbServerInfo.acUsrName, argv[2], sizeof(stSmbServerInfo.acUsrName));
    strncpy(stSmbServerInfo.acPassWD, argv[3], sizeof(stSmbServerInfo.acPassWD));
    strncpy(stSmbServerInfo.acServerAdd, argv[4], sizeof(stSmbServerInfo.acServerAdd));
    strncpy(stSmbServerInfo.acServerPath, argv[5], sizeof(stSmbServerInfo.acServerPath));

	if (LTE_OK != lte_cifs_mount(&stSmbServerInfo))
	{
	    printf("fail\n");
            return 1;
	}
        else{
	    printf("success\n");
        return 0;
	}

}
int ltectl_transfer_file(int argc, char **argv)
{
    char acSrcFile[64] = {0};
    char acDstFile[64] = {0};
    if(argc != 4)
    {
        printf("%s argc num invalid, argc:%d\n",  __func__, argc);
        return 1;
    }
    strncpy(acSrcFile, argv[2], sizeof(acSrcFile));
    strncpy(acDstFile, argv[3], sizeof(acDstFile));
    printf("File transfer start:\n ");
    if(LTE_OK != lte_up_file_transfer(acSrcFile, acDstFile))
    {
        printf("fail\n");
        return 1;
    }
    else
    {
        printf("success\n");
        return 0;
    }
    
}

int ltectl_lteup(int argc, char **argv)
{
    char acSrcFile[64] = {0};
    if(argc != 3)
    {
        printf("%s argc num invalid, argc:%d\n",  __func__, argc);
        return 1;
    }
    strncpy(acSrcFile, argv[2], sizeof(acSrcFile));
    printf("lte update:\n ");
    if(LTE_OK != lte_update(acSrcFile))
    {
        printf("fail\n");
        return 1;
    }
    else
    {
        printf("success\n");
        return 0;
    }
    
}

int ltectl_enodeb_id_get(int argc, char **argv)
{
	printf("get enodeb ID: ");
	LTE_ENODB_ID_T pstEnodebId = {0};
	if (LTE_OK != lte_enodeb_id_get(&pstEnodebId))
	{
		printf(" failed!\n");
        	return 1;
	}
	else
	{
		printf("%s\n", pstEnodebId.acEnodebId);
        	return 0;
	}

}

int ltectl_lcid_get(int argc, char **argv)
{
	printf("get lcid: ");
	LTE_LCID_T pstLcid = {0};
	if (LTE_OK != lte_lcid_get(&pstLcid))
	{
		printf(" failed!\n");
        return 1;
	}
	else
	{
		printf("%s\n", pstLcid.acLcid);
        return 0;
	}

}


int ltectl_tac_get(int argc, char **argv)
{
	printf("get tac: ");
	LTE_TAC_T pstTac = {0};
	if (LTE_OK != lte_tac_get(&pstTac))
	{
		printf(" failed!\n");
        return 1;
	}
	else
	{
		printf("%s\n", pstTac.acTac);
        return 0;
	}

}

int ltectl_all_cid_get(int argc, char **argv)
{
	printf("get allcid: \n");
	LTE_ALL_CELLID_T pstAllCid = {0};
	if (LTE_OK != lte_all_cellid_get(&pstAllCid))
	{
		printf(" failed!\n");
        return 1;
	}
	else
	{
		printf(" CELLID    : %s\n", pstAllCid.acCellId);
        printf(" ENODEB    : %s\n", pstAllCid.acEnodebId);
        printf(" LCID      : %s\n", pstAllCid.acLcid);
        printf(" TAC       : %s\n", pstAllCid.acTac);
        return 0;
	}

}

int ltectl_mac_set(int argc, char **argv)
{
    LTE_MOUDLE_MAC_T stMac = {0};
    if(argc != 3)
    {
        printf("%s argc num invalid, argc:%d\n",  __func__, argc);
        return 1;
    }
	printf("set mac : ");
	strncpy(stMac.acMac, argv[2], sizeof(stMac.acMac));

	if (LTE_OK != lte_moudle_mac_set(&stMac))
	{
	    printf("fail\n");
        return 1;
	}
    else{
	    printf("success\n");
        return 0;
	}

}

int ltectl_mac_get(int argc, char **argv)
{
	printf("get Mac: ");
    LTE_MOUDLE_MAC_T stMac = {0};
	if (LTE_OK != lte_moudle_mac_get(&stMac))
	{
		printf(" failed!\n");
        return 1;
	}
	else
	{
		printf("%s\n", stMac.acMac);
        return 0;
	}

}

int ltectl_sim_unlock(int argc, char **argv)
{
    LTE_SIM_SECRET_KEY_T stSecretKey= {0};
    if(argc < 4)
    {
        printf("ltectl_sim_unlock argc num invalid,argc=%d\n", argc);
        return 1;
    }
    stSecretKey.man_id = atoi(argv[2]);
    stSecretKey.key_ver = atoi(argv[3]);
    strncpy(stSecretKey.acSecretKey, argv[4], sizeof(stSecretKey.acSecretKey)-1);
    if (LTE_OK != lte_moudle_sim_unlock(&stSecretKey))
    {
        printf("fail\n");
        return 1;
    }else{
        printf("OK\n");
        return 0;
    }

}

int ltectl_sim_disable_unlock(int argc, char **argv)
{
    printf("ltectl_remove_bind : ");
    
    if (LTE_OK != lte_moudle_sim_disable_unlock())
    {
        printf("fail\n");
        return 1;
    }else{
        printf("OK\n");
        return 0;
    }

}

int ltectl_sim_unlock_support(int argc, char **argv)
{
	printf("sim unlock support(0-not support;1-support) : ");
    LTE_UNLOCK_SUPPORT_T stUnlockSupport = {0};
	if (LTE_OK != lte_moudle_sim_unlock_support(&stUnlockSupport))
	{
		printf(" failed!\n");
        return 1;
	}
	else
	{
		printf("%d\n", stUnlockSupport.iUnlockFlag);
        return 0;
	}

}


int ltectl_sim_type_get(int argc, char **argv)
{
    printf("get SIM type(0-general,1-secret): ");
    LTE_SIM_TYPE_E enSIMType;
    if (LTE_OK != lte_moudle_sim_type_get(&enSIMType))
    {
        printf(" failed!\n");
        return 1;
    }
    else
    {
        printf("%d\n", enSIMType);
        return 0;
    }

}

int ltectl_sim_unlock_get(int argc, char **argv)
{
    printf("get SIM unlock state(0-not unlock,1-have unlocked): ");
    LTE_SIM_UNLOCK_STATE_T stSimUnlockState = {0};
    if (LTE_OK != lte_moudle_sim_unlock_get(&stSimUnlockState))
    {
        printf(" failed!\n");
        return 1;
    }
    else
    {
        printf("%d\n", stSimUnlockState.iIsUnlock);
        return 0;
    }

}

int ltectl_signal_value_get(int argc, char **argv)
{
    LTE_SIGNAL_VALUE_T stSignalValue = {0};
    if (LTE_OK != lte_signal_value_get(&stSignalValue))
    {
        printf(" failed!\n");
        return 1;
    }
    else
    {
        printf("%s:%d\n", stSignalValue.acSignalName, stSignalValue.iSignalValue);
        return 0;
    }
}

int ltectl_network_prepare_get(int argc, char **argv)
{
    LTE_NETWORK_PREPARE_T stNWStatus = {0};
    if (LTE_OK != lte_network_prepare_get(&stNWStatus))
    {
        printf(" failed!\n");
        return 1;
    }
    else
    {
        printf("success\n");
        return 0;
    }
}

int ltectl_ip_gateway_dns_get(int argc, char **argv)
{
    LTE_IP_GATEWAY_DNS_T stIpGwDns = {0};
    if (LTE_OK != lte_ip_gateway_dns_get(&stIpGwDns))
    {
        printf(" failed!\n");
        return 1;
    }
    else
    {
        printf("******************************\n");
        printf("PdpStack type:%d\n", stIpGwDns.enPdpStack);
        printf("ipv4:"IP_FMT"\ngw:"IP_FMT"\ndns1:"IP_FMT"\ndns2:"IP_FMT"\n", IP_ARG(stIpGwDns.acIpv4),\
                IP_ARG(stIpGwDns.acGatewayV4),IP_ARG(stIpGwDns.acDns1V4),IP_ARG(stIpGwDns.acDns2V4));
        printf("ipv6:"IPV6_FMT"\ngw:"IPV6_FMT"\ndns1:"IPV6_FMT"\ndns2:"IPV6_FMT"\n", IPV6_ARG(stIpGwDns.acIpv6),\
                IPV6_ARG(stIpGwDns.acGatewayV6),IPV6_ARG(stIpGwDns.acDns1V6),IPV6_ARG(stIpGwDns.acDns2V6));
        printf("******************************\n");
        return 0;
    }
}

int ltectl_all_plmn_mode_get(int argc, char **argv)
{
    LTE_ALL_PLMN_MODE_T stAllPlmnMode = {0};
    if(LTE_OK != lte_all_plmn_mode_get(&stAllPlmnMode))
    {
        printf(" failed!\n");
        return 1;
    }
    else
    {
        printf("all_oper:%s\n", stAllPlmnMode.acAllPlmn);
        return 0;
    }
}

int ltectl_sounet_mode_set(int argc, char **argv)
{
    LTE_SOUNET_MODE_T stSounetMode = {0};
    if(argc < 4)
    {
        printf("%s argc num invalid, argc:%d\n",  __func__, argc);
        return 1;
    }
	strncpy(stSounetMode.acPlmn, argv[2], sizeof(stSounetMode.acPlmn));
	strncpy(stSounetMode.acMode, argv[3], sizeof(stSounetMode.acMode));
    if(LTE_OK != lte_sounet_mode_set(&stSounetMode))
    {
        printf(" failed!\n");
        return 1;
    }
    else
    {
        printf("set success!\n");
        return 0;
    }
}

int ltectl_puk_unlock(int argc, char **argv)
{
    LTE_SIM_UNLOCK_PUK_T stPukUnlock = {0};
    if(argc < 4)
    {
        printf("%s argc num invalid, argc:%d\n",  __func__, argc);
        return 1;
    }
	strncpy(stPukUnlock.acPuk, argv[2], sizeof(stPukUnlock.acPuk));
	strncpy(stPukUnlock.acPinNew, argv[3], sizeof(stPukUnlock.acPinNew));
    if(LTE_OK != lte_sim_puk_unlock(&stPukUnlock))
    {
        printf(" failed!\n");
        return 1;
    }
    else
    {
        printf("set success!\n");
        return 0;
    }
}

int ltectl_moudle_up_support(int argc, char **argv)
{
    LTE_MODULE_UP_SUPPORT_T stModuleUpSupp = {0};
    if(LTE_OK != lte_moudle_up_support(&stModuleUpSupp))
    {
        printf(" failed!\n");
        return 1;
    }
    else
    {
        printf("iModuleUpFlag:%d,(1-support)\n", stModuleUpSupp.iModuleUpFlag);
        return 0;
    }
}

int ltectl_pin_input_remain(int argc, char **argv)
{
    LTE_PIN_INPUT_REMAIN_T stPinIputRemain = {0};
    if(LTE_OK != lte_pin_input_remain_get(&stPinIputRemain))
    {
        printf(" failed!\n");
        return 1;
    }
    else
    {
        printf("pin_input_remain:%d.\n", stPinIputRemain.iPinIputRemain);
        return 0;
    }
}

int ltectl_network_diagnose(int argc, char **argv)
{
    LTE_DIAGNOSE_STATE_E iDiagnoseState = {0};
    char *acHelp[13] = {"OK", "DNS error", "no find module", "module no attach", "no network interface", \
        "no ltecomsev", "no sim card", "sim PUK", "signal low", "resident network fail", "no dial", \
        "no default route", "other error"};
    iDiagnoseState = lte_diagnose();
    printf("iDiagnoseState == %d, %s\n", iDiagnoseState, acHelp[iDiagnoseState]);
    return 0;
}

int ltectl_post_route_set(int argc, char **argv)
{
    LTE_POST_ROUTE_SW_T stPostRouteSw = {0};
    if(argc != 3)
    {
        printf("%s argc num invalid, argc:%d\n",  __func__, argc);
        return 1;
    }
    stPostRouteSw.iPostRouteSw = atoi(argv[2]);
    if(LTE_OK != lte_post_route_set(&stPostRouteSw))
    {
        printf(" failed!\n");
        return 1;
    }
    else
    {
        printf("success!\n");
        return 0;
    }
}

int ltectl_nv_datalock(int argc, char **argv)
{
    LTE_NV_DATALOCK_T stNvDatalock = {0};
    if(argc != 3)
    {
        printf("%s argc num invalid, argc:%d\n",  __func__, argc);
        return 1;
    }
    strncpy(stNvDatalock.acNvDatalock, argv[2], sizeof(stNvDatalock.acNvDatalock)-1);
    if(LTE_OK != lte_nv_datalock(&stNvDatalock))
    {
        printf(" failed!\n");
        return 1;
    }
    else
    {
        printf("success!\n");
        return 0;
    }
}

int ltectl_post_route_get(int argc, char **argv)
{
    LTE_POST_ROUTE_SW_T stPostRouteSw = {0};

    if(LTE_OK == lte_post_route_get(&stPostRouteSw))
    {
        printf("post route sw:%d\n", stPostRouteSw.iPostRouteSw);
        return 0;
    }
    else
    {
        printf("failed!\n");
        return 1;
    }
}

int ltectl_remove_attach_set(int argc, char **argv)
{
    if(LTE_OK == lte_remove_attach_set())
    {
        printf("remove attach seccess!\n");
        return 0;
    }
    else
    {
        printf("failed!\n");
        return 1;
    }
}





int ltectl_qos_set(int argc, char **argv)
{
	LTE_QOS_DATA_S stQosData = {0};
    if(argc == 8)
    {
      	 if(atoi(argv[2])>21 || atoi(argv[2])<1 )
		{
			return 1;
		}
	     stQosData.iQosCid=atoi(argv[2]);
		 if(atoi(argv[3])>9 || atoi(argv[3])<0 )
		{
			 return 1;

		}
		 stQosData.iQosQci=atoi(argv[3]);
		 if(atoi(argv[4])>262144 || atoi(argv[4])<0)
		 {
			return 1;
		 }
		 stQosData.iQosDlGbr=atoi(argv[4]);
		 
		 if(atoi(argv[5])>262144 || atoi(argv[5])<0)
		 {
			return 1;
		 }
		 stQosData.iQosUlGbr=atoi(argv[5]);
		 if(atoi(argv[6])>262144 || atoi(argv[6])<0)
		 {
			return 1;
		 }
		 stQosData.iQosDlMbr=atoi(argv[6]);
		 if(atoi(argv[7])>262144 || atoi(argv[7])<0)
		 {
			return 1;
		 }
		 stQosData.iQosUlMbr=atoi(argv[7]);
    	 stQosData.iArgcNumber=6;
    }
    else if(argc == 7)
    {
        if(atoi(argv[2])>21 || atoi(argv[2])<1 )
		{
			return 1;
		}
	     stQosData.iQosCid=atoi(argv[2]);
		 if(atoi(argv[3])>9 || atoi(argv[3])<0 )
		{
			 return 1;

		}
		 stQosData.iQosQci=atoi(argv[3]);
		 if(atoi(argv[4])>262144 || atoi(argv[4])<0)
		 {
			return 1;
		 }
		 stQosData.iQosDlGbr=atoi(argv[4]);
		 
		 if(atoi(argv[5])>262144 || atoi(argv[5])<0)
		 {
			return 1;
		 }
		 stQosData.iQosUlGbr=atoi(argv[5]);
		 if(atoi(argv[6])>262144 || atoi(argv[6])<0)
		 {
			return 1;
		 }
		 stQosData.iQosDlMbr=atoi(argv[6]);
		 stQosData.iArgcNumber=5;
    }
	else if(argc == 6)
    {
         if(atoi(argv[2])>21 || atoi(argv[2])<1 )
		{
			return 1;
		}
	     stQosData.iQosCid=atoi(argv[2]);
		 if(atoi(argv[3])>9 || atoi(argv[3])<0 )
		{
			 return 1;

		}
		 stQosData.iQosQci=atoi(argv[3]);
		 if(atoi(argv[4])>262144 || atoi(argv[4])<0)
		 {
			return 1;
		 }
		 stQosData.iQosDlGbr=atoi(argv[4]);
		 
		 if(atoi(argv[5])>262144 || atoi(argv[5])<0)
		 {
			return 1;
		 }
		 stQosData.iQosUlGbr=atoi(argv[5]);
		 stQosData.iArgcNumber=4;
    }
	else if(argc == 5)
    {
         if(atoi(argv[2])>21 || atoi(argv[2])<1 )
		{
			return 1;
		}
	     stQosData.iQosCid=atoi(argv[2]);
		 if(atoi(argv[3])>9 || atoi(argv[3])<0 )
		{
			 return 1;

		}
		 stQosData.iQosQci=atoi(argv[3]);
		 if(atoi(argv[4])>262144 || atoi(argv[4])<0)
		 {
			return 1;
		 }
		 stQosData.iQosDlGbr=atoi(argv[4]);
		 stQosData.iArgcNumber=3;
    }
	else if(argc == 4)
    {
		if(atoi(argv[2])>21 || atoi(argv[2])<1 )
		{
			return 1;
		}
	     stQosData.iQosCid=atoi(argv[2]);
		 if(atoi(argv[3])>9 || atoi(argv[3])<0 )
		{
			 return 1;

		}
		 stQosData.iQosQci=atoi(argv[3]);
		 stQosData.iArgcNumber=2;
    }
	else if(argc == 3)
    {
        if(atoi(argv[2])>21 || atoi(argv[2])<1 )
		{
			return 1;
		}
	     stQosData.iQosCid=atoi(argv[2]);
		 stQosData.iArgcNumber=1;
    }
    else
    {
        printf("%s argc num invalid, argc:%d\n",  __func__, argc);
        return 1;
    }
	printf("set qos : ");

	if (LTE_OK != lte_qos_set(&stQosData))
	{
	    printf("fail\n");
            return 1;
	}
    else{
	    printf("success\n");
        return 0;
	}
   
      
  
}




int ltectl_qos_get(int argc, char **argv)
{
    LTE_QOS_DATA_S stQosData={0};
	printf("get QOS data : \n");
    if(LTE_OK == lte_qos_get(&stQosData))
    {
        
		
		printf("    CID    :      %d\n", stQosData.iQosCid);
	    printf("    QCI :        %d\n", stQosData.iQosQci);
		printf("    DL_GBR      : %d kbit/s \n", stQosData.iQosDlGbr);
		printf("    UL_GBR      : %d kbit/s \n", stQosData.iQosUlGbr);
		printf("    DL_MBR      : %d kbit/s \n", stQosData.iQosDlMbr);
		printf("    UL_MBR      : %d kbit/s \n", stQosData.iQosUlMbr);
		
		return 0;
    }
    else
    {
        printf("failed!\n");
        return 1;
    }
}


int ltectl_dload_prot_ver_get(int argc, char **argv)
{
    LTE_DLOAD_PROT_VER_T stDloadProtVer = {0};
    if( LTE_OK == lte_dload_prot_ver_get(&stDloadProtVer) )
    {
        printf("get dload protocol ver info:\n");
        printf("%s",stDloadProtVer.acSwer);
        printf("%s",stDloadProtVer.acIosVer);
        printf("%s",stDloadProtVer.acProductName);
        printf("%s",stDloadProtVer.acDloadType);
        return 0;
    }else{
        printf("failed!\n");
        return 1;
    }
}
int ltectl_dload_ver_info_get(int argc, char **argv)
{
    LTE_DLOAD_VER_INFO_T stDloadVerInfo = {0};
    if( LTE_OK == lte_dload_ver_info_get(&stDloadVerInfo) )
    {
        printf("get dload version info:\n");
        printf("%s\n",stDloadVerInfo.acDloadVerInfo);
        return 0;
    }else{
        printf("failed!\n");
        return 1;
    }
}
int ltectl_nv_backup(int argc, char **argv)
{
    LTE_NV_BACKUP_T stNvBackup = {0};
    if( LTE_OK == lte_nv_backup(&stNvBackup) )
    {
        printf("nv backup state:\n");
        printf("%d\n",stNvBackup.iNvBackup); 
        return 0;
    }else{
        printf("failed!\n");
        return 1;
    }
}
int ltectl_nv_restore(int argc, char **argv)
{
    LTE_NV_RESTORE_T stNvRestore = {0};
    if( LTE_OK == lte_nv_restore(&stNvRestore) )
    {
        printf("nv restore state:\n");
        printf("%d\n",stNvRestore.iNvRestore); 
        return 0;
    }else{
        printf("failed!\n");
        return 1;
    }
}
int ltectl_dload_mode(int argc, char **argv)
{

    if(LTE_OK == lte_dload_mode())
    {
        printf("lte change dload mode seccess!\n");
        return 0;
    }else
    {
        printf("failed!\n");
        return 1;
    }
}
int ltectl_data_mode(int argc, char **argv)
{

    if(LTE_OK == lte_data_mode())
    {
        printf("lte change data mode seccess!\n");
        return 0;
    }else
    {
        printf("failed!\n");
        return 1;
    }
}
int ltectl_reset(int argc, char **argv)
{

    if(LTE_OK == lte_reset())
    {
        printf("lte reset seccess!\n");
        return 0;
    }else
    {
        printf("failed!\n");
        return 1;
    }
}
int ltectl_sim_switch_get(int argc, char **argv)
{
    LTE_SIM_SWITCH_T stSimSwitch = {0};
    if( LTE_OK == lte_sim_switch_get(&stSimSwitch) )
    {
        printf("get sim switch status:");
        printf("%d \n",stSimSwitch.iSimSw);

        return 0;
    }else{
        printf("failed!\n");
        return 1;
    }
}
int ltectl_sim_switch_set(int argc, char **argv)
{
    LTE_SIM_SWITCH_T stSimSwitch = {0};
    if(argc != 3)
    {
        printf("%s argc num invalid, argc:%d\n",  __func__, argc);
        return 1;
    }

    stSimSwitch.iSimSw = atoi(argv[2]);
    
    
    if(LTE_OK != lte_sim_switch_set(&stSimSwitch))
    {
        printf(" failed!\n");
        return 1;
    }
    else
    {
        printf("success!\n");
        return 0;
    }
}
int ltectl_sim_switch_soft_set(int argc, char **argv)
{
    LTE_SIM_SWITCH_T stSimSwitch = {0};
    
    if(argc != 8)
    {
        printf("%s argc num invalid, argc:%d\n",  __func__, argc);
        return 1;
    }

    strncpy(stSimSwitch.acSimImsi, argv[2], sizeof(stSimSwitch.acSimImsi));
    stSimSwitch.iSimMnclen = atoi(argv[3]);
    stSimSwitch.iSimSuauthpara = atoi(argv[4]);
    stSimSwitch.iSimSualg = atoi(argv[5]);
    strncpy(stSimSwitch.acSimKey, argv[6], sizeof(stSimSwitch.acSimKey));
    strncpy(stSimSwitch.acSimOp, argv[7], sizeof(stSimSwitch.acSimOp));
    
    
    if(LTE_OK != lte_sim_switch_soft_set(&stSimSwitch))
    {
        printf(" failed!\n");
        return 1;
    }
    else
    {
        printf("success!\n");
        return 0;
    }
}

LTE_CTL_CMD_T gstLteCmd[] = {
    {"poweron",     ltectl_poweron,             "lte module power on"},
    {"poweroff",    ltectl_poweroff,            "lte module power off"},
    {"getpinstate", ltectl_get_pinstate,        "lte module get pin state"},
    {"getpinsw",    ltectl_pin_sw_get,          "lte module get pin sw value"},
    {"getnettype",  ltectl_get_nettype,         "lte module get net type"},
    {"getlinkstate",ltectl_linkstate_get,       "lte module get link state"},
    {"getsimstate", ltectl_sim_state_get,       "lte module get sim state (1: ready 0:no)"},
    {"getlteinfo",  ltectl_lte_stateinfo_get,   "lte module get lte state info, signal..."},
    {"getaccessnet",ltectl_access_net_get,      "lte module get lte access network, eg,3G or 4G"},
    {"getelcsignal",ltectl_elecSignal_get,      "lte module get lte signal"},
    {"getimei",     ltectl_imei_get,            "lte module get lte imei"},
    {"getcellid",   ltectl_cellid_get,          "lte module get lte cellid"},
    {"geticcid",    ltectl_iccid_get,           "lte module get lte iccid"},
    {"getimsi",     ltectl_imsi_get,            "lte module get lte imsi"},
    {"getbands",    ltectl_bands_get,           "lte module get lte bands"},
    {"getnwstatus", ltectl_nw_status_get,       "lte module get net work prepare status"},
    {"connect",     ltectl_connect,             "lte module connect"},
    {"connectasync",ltectl_connect_async,       "lte module async connect"},
    {"disconnect",  ltectl_disconnect,          "lte module disconnect"},
    {"disconnectipv6",ltectl_disconnect_ipv6,   "lte module disconnect ipv6"},
    {"unlockpin",   ltectl_unlock_pin,          "lte module unlock pin"},
    {"setpinsw",    ltectl_pin_switch,          "lte module open/close pin, 0 close pin, 1 open pin"},
    {"alterpinwd",  ltectl_pin_alter,           "lte module alter pin code, ltectl alterpinwd oldpwd newpwd"},
    {"setnettype",  ltectl_set_nettype,         "ltectl setnettype <1,2,4,8,16,32> [1,2,4,8]"},
    {"setclckbands",ltectl_clck_bands_set,      "lte module set clck bands"},
    {"setclckclose",ltectl_clck_bands_close,    "lte module close clck bands"},
    {"getclcceflag",ltectl_clckcell_flag_get,   "lte module get clck cell flag"},
    {"setclckcell", ltectl_clck_cell_set,       "lte module set clck bands, ltectl setclckcell 38350 135 1"},
    {"gpsstart",    ltectl_gps_start,           "lte module gps start"},
    {"gpsstop",     ltectl_gps_stop,            "lte module gps stop"},
    {"getgpsdate",  ltectl_gpsdate_get,         "lte module get gps date"},
    {"getgpspostion",ltectl_gps_postion_get,    "lte module get gps postion"},
    {"setgpsclosetag",ltectl_gps_set_close_tag, "lte module set gps close tag"},
    {"getgpsclosetag",ltectl_gps_get_close_tag, "lte module get gps close tag"},
    {"gpstest",     ltectl_gps_test,            "lte module gps test"},
    {"getgpsdata",  ltectl_get_gps_data,        "lte module gps test"},
    {"getoperinfo", ltectl_oper_info_get,    "lte module get lte operator info"},
    {"getall",      ltectl_all_status_get,      "lte module get all stats"},
	{"getnetsinfo", ltectl_nets_info_get,    "lte module get lte nettypes  info"},
    /* BEGIN: Added by wujian, 2014/11/13   PN:添加短信功能相关tool */
    {"setcentcode", ltectl_sms_cent_code_set,         "lte module set cent code info"},
    {"setsmsmode",  ltectl_sms_mode_set,         "lte module set sms mode"},
    {"smsinit",     ltectl_sms_init,           "ltectl_sms_init" },
    {"getsms",      ltectl_sms_read,         "read sms:0 express read all sms,1~50 express read 1~50th sms"},
    {"smssend",     ltectl_sms_send,         "send sms : mobilenum data"},
    {"smsdel",      ltectl_sms_dele,         "delete sms by index"},
    {"getsmssupp",  ltectl_sms_supp_func,    "support sms func,1:support,0: no support"},
    /* END:   Added by wujian, 2014/11/13   PN:添加短信功能相关tool */
    {"setpdpstack", ltectl_pdp_set,          "ltectl setpdpstack <0,1,2> [apn]"},
    {"setapn",      ltectl_apn_set,          "ltectl setapn apn"},
    {"getversion",  ltectl_version_get,      "get module version"},
    {"mountcifs",   ltectl_cifs_mount,       "mount cifs device"},
    {"transfile",   ltectl_transfer_file,    "transfer file"},
    {"lteup",       ltectl_lteup,            "lte up version"},
    {"getenodeb",   ltectl_enodeb_id_get,    "ltectl getenodeb"},
    {"gettac",      ltectl_tac_get,          "ltectl gettac"},
    {"getlcid",     ltectl_lcid_get,         "ltectl getlcid"},
    {"getallcid",   ltectl_all_cid_get,      "ltectl getallcid"},
    {"setmac",      ltectl_mac_set,          "ltectl setmac aabbccddeeff"},
    {"getmac",      ltectl_mac_get,          "ltectl getmac"},
    {"simunlock",   ltectl_sim_unlock,       "ltectl simunlock 12 1 XXX..."},
    {"simunlockdisable",ltectl_sim_disable_unlock,      "ltectl simunlockdisable"},
    {"simunlocksupport",ltectl_sim_unlock_support,      "ltectl simunlocksupport"},
    {"getsimtype",  ltectl_sim_type_get,     "ltectl getsimtype"},
    {"getsimunlockstate",  ltectl_sim_unlock_get,     "ltectl getsimunlockstate"},
    {"getsignalvalue",  ltectl_signal_value_get,     "ltectl getsignalvalue"},
    {"getnetworkprepare",  ltectl_network_prepare_get,     "ltectl getnetworkprepare"},
    {"getipgwdns",  ltectl_ip_gateway_dns_get,     "ltectl getipgwdns"},
    {"getallplmnmode",  ltectl_all_plmn_mode_get,     "ltectl getallplmnmode"},
    {"setsounetmode",  ltectl_sounet_mode_set,     "ltectl setsounetmode 46000 4G"},
    {"unlockpuk",  ltectl_puk_unlock,     "ltectl unlockpuk puknumber newpinnumber"},
    {"getmoudleupsupport",  ltectl_moudle_up_support,     "ltectl getmoudleupsupport"},
    {"getpininputremain",  ltectl_pin_input_remain,     "ltectl getpininputremain"},
    {"networkdiagnose",  ltectl_network_diagnose,     "ltectl networkdiagnose"},
    {"setpostroute",  ltectl_post_route_set,     "ltectl setpostroute"},
    {"nvdatalock",  ltectl_nv_datalock,     "ltectl nvdatalock xxx"},
    {"getpostroute",  ltectl_post_route_get,     "ltectl getpostroute"},
    {"removeattach",  ltectl_remove_attach_set,     "ltectl removeattach"},
    {"setepsqos",         ltectl_qos_set,	                           "ltectl set qos <1-21> <0-9> <0-262144> <0-262144> <0-262144> <0-262144>"},
	{"getepsqos",         ltectl_qos_get,	                           "ltectl get qos"},
	{"getdloadprotver", ltectl_dload_prot_ver_get, "ltectl getdloadprotver"},
    {"getdloadverinfo", ltectl_dload_ver_info_get, "ltectl getdloadverinfo"},
    {"nvbackup", ltectl_nv_backup, "ltectl nvbackup"},
    {"nvrestore", ltectl_nv_restore, "ltectl nvrestore"},
    {"dloadmode", ltectl_dload_mode, "ltectl dloadmode"},
    {"datamode", ltectl_data_mode, "ltectl datamode"},
    {"reset", ltectl_reset, "ltectl reset"},
    {"simswitchget", ltectl_sim_switch_get, "ltectl simswitchget"},
    {"simswitchset", ltectl_sim_switch_set, "ltectl simswitchset"},
    {"simswitchsoftset", ltectl_sim_switch_soft_set, "ltectl simswitchsoftset"},
};

int ltectl_help(VOID)
{
    int iLoop;
	
	printf("ltectl help\n");
	printf("commands:\n");

	for(iLoop = 0; iLoop < sizeof(gstLteCmd)/sizeof(gstLteCmd[0]); iLoop++)
	{
		printf("\t%-10s\t%s\n", gstLteCmd[iLoop].pcName, gstLteCmd[iLoop].pcHelp);
	}

	return 0;
}

int main(int argc, char **argv)
{
    int iLoop = 0;
    int iFind = 0;
    int iRet = 1;
	if (LTE_OK != lte_init(7))
	{
		printf("lte_init failed.\n");
		return 0;
	}
    if(1 == argc)
    {
        ltectl_help();
        return 0;
    }
    for(iLoop = 0; iLoop < sizeof(gstLteCmd)/sizeof(gstLteCmd[0]); iLoop++)
	{
		if (!strcmp(argv[1], gstLteCmd[iLoop].pcName))
		{
			iRet = gstLteCmd[iLoop].pFunc(argc, argv);
			iFind = 1;
		}
	}
	
	if(!iFind)
	{
		ltectl_help();
	}
    if(0 != iRet)
    {
        printf("execute command fail\n");
    }
    
    #if 0
	test_imei_get();
	
	test_connect("cmnet");
	sleep(60);
	test_disconnect();
	test_cellid_get();
	test_poweroff();
	printf(" ... Test End...\n");
    #endif
    
    return iRet;
}


