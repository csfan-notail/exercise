#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>  
#include <unistd.h>

#include "reg_ioctl.h"
#include "ap_types.h"
#include "uci_config.h"

#define REG_PORT_RX_BASE_ADDR  0x2003c
#define REG_PORT_TX_BASE_ADDR  0x20084

#define PORT_COUNTER_OFFSET    8

#define UCI_PORTCOUNTE_RX_LAN_1     "port.rx.lan1" 
#define UCI_PORTCOUNTE_RX_LAN_2	    "port.rx.lan2"
#define UCI_PORTCOUNTE_RX_LAN_3     "port.rx.lan3"
#define UCI_PORTCOUNTE_RX_LAN_4     "port.rx.lan4"

#define UCI_PORTCOUNTE_TX_LAN_1     "port.tx.lan1"
#define UCI_PORTCOUNTE_TX_LAN_2     "port.tx.lan2"
#define UCI_PORTCOUNTE_TX_LAN_3     "port.tx.lan3"
#define UCI_PORTCOUNTE_TX_LAN_4     "port.tx.lan4"

#define DATA_SIZE_2GB 2000000000
int port_statistics()
{
	int iRet;
	int iPort = 0;
	char acTmp[128] = {0};
	char acCount[128] = {0};
	char acTmpStr[128] ={0};
	static int aiTxCount[4] = {0};
	static int aiRxCount[4] = {0};
	REGISTER_IOCTL_T stIoctl = {0};
	static unsigned long alPortRxVal[4] = {0};
	static unsigned long alPortTxVal[4] = {0};
	
	stIoctl.stRegData.enRegType = REGISTER_IOCTL_ADDR_SWITCH;
	stIoctl.stRegData.uiRegValu = 10000000;
	while(1)
	{
		for( iPort = 0;iPort < 4;iPort++ )
		{
			stIoctl.stRegData.uiRegAddr = (REG_PORT_RX_BASE_ADDR | ((iPort + 1) << PORT_COUNTER_OFFSET));	
			register_ioctl(&stIoctl,REGISTER_IOCTL_READ);
			alPortRxVal[iPort] = alPortRxVal[iPort]+ stIoctl.stRegData.uiRegValu;
			
			stIoctl.stRegData.uiRegAddr = (REG_PORT_TX_BASE_ADDR | ((iPort + 1)<< PORT_COUNTER_OFFSET));
			register_ioctl(&stIoctl,REGISTER_IOCTL_READ);
			alPortTxVal[iPort] = alPortTxVal[iPort] + stIoctl.stRegData.uiRegValu;
			
			if (alPortTxVal[iPort]> DATA_SIZE_2GB)
			{
				aiTxCount[iPort]++;
				
				sprintf(acTmpStr,"echo %d > /tmp/uci/txcount_lan%d",aiTxCount[iPort],iPort+1);   //count_lan
				system(acTmpStr);
				alPortTxVal[iPort] = alPortTxVal[iPort] - DATA_SIZE_2GB;
				memset(&alPortTxVal[iPort],0,sizeof(alPortTxVal[iPort]));
			}
			if (alPortRxVal[iPort] > DATA_SIZE_2GB)	
			{
				aiRxCount[iPort]++;
				
				sprintf(acTmpStr,"echo %d > /tmp/uci/rxcount_lan%d",aiRxCount[iPort],iPort+1);   //count_lan
				system(acTmpStr);
				
				alPortRxVal[iPort] = alPortRxVal[iPort] - DATA_SIZE_2GB;
				memset(&alPortRxVal[iPort],0,sizeof(alPortRxVal[iPort]));
			}	
			
			sleep(1);
		}

		/********************** RX ***********************/
		sprintf(acTmp,"echo %ld > /tmp/uci/rxlan1",alPortRxVal[0]);   
		system(acTmp);
		sleep(1);
		
		sprintf(acTmp,"echo %ld > /tmp/uci/rxlan2",alPortRxVal[1]);   
		system(acTmp);
		sleep(1);
		
		sprintf(acTmp,"echo %ld > /tmp/uci/rxlan3",alPortRxVal[2]);   
		system(acTmp);
		sleep(1);
				
		sprintf(acTmp,"echo %ld > /tmp/uci/rxlan4",alPortRxVal[3]);   
		system(acTmp);
		sleep(1);
		
	    /********************** TX ***********************/
		
		sprintf(acTmp,"echo %ld > /tmp/uci/txlan1",alPortTxVal[0]);   
		system(acTmp);
		sleep(1);
		
		sprintf(acTmp,"echo %ld > /tmp/uci/txlan2",alPortTxVal[1]);   
		system(acTmp);
		sleep(1);

		sprintf(acTmp,"echo %ld > /tmp/uci/txlan3",alPortTxVal[2]);   
		system(acTmp);
		sleep(1);

		sprintf(acTmp,"echo %ld > /tmp/uci/txlan4",alPortTxVal[3]);   
		system(acTmp);
		
		printf("port1 rx = %ld   tx = %ld \n",alPortRxVal[0],alPortTxVal[0]);
		printf("port2 rx = %ld   tx = %ld \n",alPortRxVal[1],alPortTxVal[1]);
		printf("port3 rx = %ld   tx = %ld \n",alPortRxVal[2],alPortTxVal[2]);
		printf("port4 rx = %ld   tx = %ld \n",alPortRxVal[3],alPortTxVal[3]);

		sleep(17);
	}

	return 0;
}

int main(int argc, char const *argv[])
{
	system("echo 0 > /tmp/uci/rxcount_lan1");
	system("echo 0 > /tmp/uci/rxcount_lan2");
	system("echo 0 > /tmp/uci/rxcount_lan3");
	system("echo 0 > /tmp/uci/rxcount_lan4");
	system("echo 0 > /tmp/uci/txcount_lan1");
	system("echo 0 > /tmp/uci/txcount_lan2");
	system("echo 0 > /tmp/uci/txcount_lan3");
	system("echo 0 > /tmp/uci/txcount_lan4");
	
	if( 0 != port_statistics())
	{
		printf("port_statistics fail");
	}
	
	return 0;
}