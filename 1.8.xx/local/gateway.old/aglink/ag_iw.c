/*
 * ======================================================
 *  
 *    Filename:  ag_iw.c
 *    Description:  iwconfig相关
 *    Version :  
 *    Created :  
 *    Revision:  none
 *    Compiler:  gcc
 *    Author  :  
 *    Company :  www.gbcom.com.cn 
 *
 * ======================================================
 */

#include "iwlib.h"


static int aglink_parse_usr_info(int skfd,
	   char *ifname,
	   char *args[],
	   int count)
{
	struct wireless_info info;
	int i, val, rc = 0;
	char *ptrstr = (char *)args[0];
	void *pstWifiInfo = NULL;
	int (*func)(char *, void *) = NULL;
	unsigned int addr = 0;

	for(i = 0; i < 8; ++i){
		if((ptrstr[i] >= '0') && (ptrstr[i] <= '9'))
			val = ptrstr[i] - '0';
		else if((ptrstr[i] >= 'a') && (ptrstr[i] <= 'f'))
			val = ptrstr[i] - 'a' + 10;
		else if((ptrstr[i] >= 'A') && (ptrstr[i] <= 'F'))
			val = ptrstr[i] - 'A' + 10;
		else
			return -1;

		addr = ((addr << 4) + val);
	}
	if(NULL != addr)
	{
		func = addr;
	}
	else
	{
		perror("addr is not, param error");
		return -1;
	}

	ptrstr = (char *)args[1];	//char 型指针，其值为变量的地址, AP_WIFI_INFO
	addr = 0;
	for(i = 0; i < 8; ++i){
		if((ptrstr[i] >= '0') && (ptrstr[i] <= '9'))
			val = ptrstr[i] - '0';
		else if((ptrstr[i] >= 'a') && (ptrstr[i] <= 'f'))
			val = ptrstr[i] - 'a' + 10;
		else if((ptrstr[i] >= 'A') && (ptrstr[i] <= 'F'))
			val = ptrstr[i] - 'A' + 10;
		else
			return -1;

		addr = ((addr << 4) + val);
	}
	if(NULL != addr)
	{
		pstWifiInfo = addr;
	}
	else
	{
		perror("stWifiInfo address value is error");
		return -1;
	}
		

	if(NULL != func){
		if(iw_get_basic_config(skfd, ifname, &(info.b)) == 0)
			rc = func(ifname, pstWifiInfo);
	}
	
	return rc;
}


int aglink_get_vap_dev(char *args[], int count)
{
	int skfd;		/* generic raw socket desc.	*/
	int goterr = 0;

	/* Create a channel to the NET kernel. */
	if((skfd = iw_sockets_open()) < 0)
	{
		perror("socket");
		exit(-1);
	}

	iw_enum_devices(skfd, &aglink_parse_usr_info, args, count);

	/* Close the socket. */
	iw_sockets_close(skfd);

	return(goterr);
}


