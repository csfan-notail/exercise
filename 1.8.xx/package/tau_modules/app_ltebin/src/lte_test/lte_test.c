#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lte.h"

void test_get_pinstate(void)
{
	LTE_PIN_STATE_T stPinState;
	printf("Pin state : ");
    	if ( LTE_OK != lte_ioctl(LTE_PIN_STATE, (void*)(&stPinState), sizeof(LTE_PIN_STATE_T)))
    	{
    		printf("fail\n");
		return;
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
}

void test_unlock_pin(void)
{
	LTE_PIN_T stPin;
	printf("unlock pin : ");
	strcpy(stPin.acPin, "1234");

	if (LTE_OK != lte_ioctl(LTE_PIN_UNLOCK, (void*)(&stPin), sizeof(LTE_PIN_T))){
		printf("fail\n");
	}else{
		printf("ok\n");
	}
}

void test_get_nettype(void)
{
	LTE_NET_TYPE_T stNettype = {0,0};
	printf("get net-type : ");
	if (LTE_OK != lte_ioctl(LTE_NETTYPE_GET, &stNettype, sizeof(LTE_NET_TYPE_T))){
		printf("fail\n");
	}else{
		printf("nettype %d,pri %d\n", stNettype.enNetType,stNettype.enNetTypePri);
	}
}
void test_set_nettype(void)
{
	LTE_NET_TYPE_T stNettype = {0,0};
    stNettype.enNetType = LTE_3G_4G;
    stNettype.enNetTypePri = LTE_4G_PRI;
	printf("set net-type : ");
	if (LTE_OK != lte_ioctl(LTE_NETTYPE_SET, &stNettype, sizeof(LTE_NET_TYPE_T))){
		printf("fail\n");
	}else{
		printf("OK!\n");
	}
}


void test_connect(char *pcApn)
{
	LTE_APN_T stApn;
	strcpy(stApn.acApn, pcApn);

	printf("connect : ");
	if (LTE_OK != lte_connect(&stApn, 1)){
		printf("fail\n");
	}else{
		printf(" OK!\n");
	}
}

void test_connect_async(char *pcApn)
{
	LTE_APN_T stApn;
	strcpy(stApn.acApn, pcApn);

	printf("connect : ");
	if (LTE_WAIT != lte_connect_async(&stApn, 1)){
		printf("fail\n");
	}else{
		printf(" OK!\n");
	}
}

void test_disconnect(void)
{
	printf("disconnect : ");
	if (LTE_OK != lte_disconnect()){
		printf("fail\n");
	}else{
		printf(" OK!\n");
	}
}

void test_poweron(void)
{
	printf("power on : ");
	if (LTE_OK != lte_power_on()){
		printf("fail\n");
	}else{
		printf(" OK!\n");
	}
}

void test_poweroff(void)
{
	printf("power off : ");
	if (LTE_OK != lte_power_off()){
		printf("fail\n");
	}else{
		printf(" OK!\n");
	}
}

void test_elecSignal_get(void)
{
	printf("get elec signal : ");
	LTE_SIGNAL_T stSignal;
	if (LTE_OK != lte_signal_get(&stSignal)){
		printf("fail\n");
	}else{
		printf(" %u\n", stSignal.uiSignal);
	}
}

void test_imei_get(void)
{
	printf("get imei : ");
	LTE_IMEI_T stImei;
	if (LTE_OK != lte_imei_get(&stImei)){
		printf("fail\n");
	}else{
		printf(" %s\n", stImei.acImei);
	}
}

void test_cellid_get(void)
{
	printf("get cellid : ");
	LTE_CELLID_T stCellid;
	if (LTE_OK != lte_cellid_get(&stCellid)){
		printf("fail\n");
	}else{
		printf(" %s\n", stCellid.acCellID);
	}
}

void test_sim_state_get(void)
{
	printf("get SIM state : ");
	LTE_SIM_STATE_T stSimState;
	if (LTE_OK != lte_sim_state_get(&stSimState)){
		printf("fail\n");
	}else{
		printf(" %d\n", stSimState.enSimState);
	}
}

void test_lte_stateinfo_get(void)
{
	printf("get LTE info : ");
	LTE_INFO_T stLteInfo = {0};
	if(LTE_OK != lte_info_get(&stLteInfo)){
		printf("fail\n");
	}else{
		printf(" ok\n");
	}

	printf("    CEELLID  : %s\n", stLteInfo.stCellInfo.acCellId);
	printf("    RSSI      : %d dbm\n", stLteInfo.stCellInfo.iRssi);
	printf("    RSRP      : %d dbm\n", stLteInfo.stCellInfo.iRsrp);
	printf("    RSRQ     : %d db\n", stLteInfo.stCellInfo.iRsrq);
	printf("    SINR      : %d db\n", stLteInfo.stCellInfo.iSinr);
	printf("    BAND     : %s\n", stLteInfo.stCellInfo.acBand);
	printf("    CHANNEL: %d\n", stLteInfo.stCellInfo.iChannel);
}

void test_linkstate_get(void)
{
	printf("get LTE linkstate : ");
	LTE_LINKSTATE_T stLink = {0};

	if (LTE_OK != lte_linkstate_get(&stLink)){
		printf("fail\n");
	}else{
		printf("%d\n", stLink.enLinkstate);
	}
}

void test_access_net_get(void)
{
	printf("get LTE access network : ");
	LTE_ACCESS_NETWORK_T stNet;

	if (LTE_OK != lte_access_net_get(&stNet)){
		printf("fail\n");
	}else{
		switch(stNet.enAccessNet){
			case NET_NONE:
				printf("none\n");
				break;
			case NET_3G:
				printf("3G\n");
				break;
			case NET_4G:
				printf("4G\n");
				break;
		}
	}
}

void test_bands_get(void)
{
	printf("get LTE support bands : ");
	LTE_BANDS_T stBands;

	if (LTE_OK != lte_bands_get(&stBands))
	{
		printf("fail\n");
	}else{
		printf("%s\n", stBands.bands);
	}
}

void test_clck_bands_set(char *bands)
{
	printf("set LTE support bands : ");
	LTE_BANDS_T stBands;

	strncpy(stBands.bands, bands, sizeof(stBands.bands));

	if (LTE_OK != lte_clck_bands_set(&stBands))
	{
		printf("fail\n");
	}else{
		printf("success\n");
	}
}

int main(void)
{
	printf(" ... Test Start...\n");

	if (LTE_OK != lte_init(7))
	{
		printf("lte_init failed.\n");
		return 0;
	}

	test_imei_get();
	
	test_connect("cmnet");
	sleep(60);
	test_disconnect();
	test_cellid_get();
	test_poweroff();
	printf(" ... Test End...\n");
	return 0;
}


