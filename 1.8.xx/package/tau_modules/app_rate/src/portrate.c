#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>  
#include <unistd.h>

#include "reg_ioctl.h"
#include "ap_types.h"
#include "uci_config.h"

#define UCI_LAN_RATE   "port.lan.rate"

#define CMD_RX "cat /proc/net/dev |grep eth1 |awk '{print$2}' |head -n 1"
#define CMD_TX "cat /proc/net/dev |grep eth1 |awk '{print$10}' |head -n 1"

int rate_statistics()
{
	char acRateRx[128] = {0};
	char acRateTx[128] = {0};;
	char acTmp[128] = {0};;
	FILE *fp_eth1_rx;
	FILE *fp_eth1_tx;
	static int iRateRx = 0;
	static int iRateTx = 0;
	static int iRateRxOld = 0;
	static int iRateTxOld = 0;
	float fRate = 0;

	
	fp_eth1_tx=popen(CMD_TX,"r");
	if( !fp_eth1_tx )
	{
		printf("OpenTx fail \n");
		strcpy(acRateTx,"");
	}
	else 
	{
		fgets(acRateTx,sizeof(acRateTx),fp_eth1_tx);
		pclose(fp_eth1_tx);
	}
	sleep(1);
	fp_eth1_rx=popen(CMD_RX,"r");
	if( !fp_eth1_rx )
	{
		printf("OpenRx fail \n");
		strcpy(acRateRx,"");
	}
	else 
	{
		fgets(acRateRx,sizeof(acRateRx),fp_eth1_rx);
		pclose(fp_eth1_rx);
	}
	
	sscanf(acRateRx,"%d",&iRateRx);
	sscanf(acRateTx,"%d",&iRateTx);

	fRate = ((iRateRx - iRateRxOld) + (iRateTx - iRateTxOld))/5;
	printf("rate ============================ %0.1f\n",fRate);

	sprintf(acTmp,"echo %0.1f > /tmp/uci/lanrate",fRate);   
	system(acTmp);
	iRateTxOld = iRateTx;
	iRateRxOld = iRateRx;

	return 0;
}

int main(int argc, char const *argv[])
{

	while(1){	 
		 rate_statistics();
		
		sleep(5);
	}
	
	return 0;
}