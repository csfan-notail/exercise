/*
 * System header files. 
 */
#include <errno.h>          /* errno declaration & error codes.            */
#include <netdb.h>          /* getaddrinfo(3) et al.                       */
#include <netinet/in.h>     /* sockaddr_in & sockaddr_in6 definition.      */
#include <stdio.h>          /* LTE_LOG(LTE_LOG_INFO, 3) et al.                            */
#include <stdlib.h>         /* exit(2).                                    */
#include <string.h>         /* String manipulation & memory functions.     */
#include <sys/poll.h>       /* poll(2) and related definitions.            */
#include <sys/socket.h>     /* Socket functions (socket(2), bind(2), etc). */
#include <unistd.h>         /* getopt(3), read(2), etc.                    */
#include <sys/types.h>  
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <net/if_arp.h>
//#include <getopt.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#include "lte_log.h"
#include "ltecontrolclient.h"

typedef enum _ALTAIR_BOOL{
TRUE = 0,
FALSE
}ALTAIR_BOOL;

#define MAX_MESSAGE_LEN 1024
typedef struct _RxEtsHeaderT{
	char Sync;
	unsigned int Length;
	unsigned short MsgId;
}RxEtsHeaderT;

#define USIM_BUF_LEN 			64
#define SYNC_CHAR 			0x7E  /* '~' */
#define ESC_CHAR 			0x7D  /* '}' */
#define MOD_CHAR 			0x20  /* ' ' */

#define SET_BIT_FLAG 			0x1000
#define USIM_BIT_FLAG 			0x2000
#define GET_BIT_FLAG 			0x3000
#define OPERATION_BIT_FLAG 		0x4000
#define SPECIAL_BIT_FLAG 		0x5000
#define TEST_BIT_FLAG 			0x9000

#define RSP_OK 				0x01
#define RSP_ERROR			0x0


#define ENABLE				1
#define DISABLE				0

#define WORK_MODE_ROUTER 		0x01
#define WORK_MODE_BRIDGE 		0x02

#define CONNECT_MODE_AUTO 		0x01
#define CONNECT_MODE_MANUAL 		0x02

#define AUTHEN_PROTOCOL_PAP 		0x01
#define AUTHEN_PROTOCOL_CHAP 		0x02

#define SET_WORK_MODE_REP 		0x1001
#define SET_WORK_MODE_RSP 		0x1002
#define SET_CONNECT_MODE_REP 		0x1003
#define SET_CONNECT_MODE_RSP 		0x1004


#define SET_APN_REP 			0x1005
#define SET_APN_SUBMSGID_INDEX 		0x1
#define SET_APN_SUBMSGID_PROTOCOL 	0x2
#define SET_APN_SUBMSGID_APN_NAME 	0x3
#define SET_APN_RSP 			0x1006

#define SET_AUTH_APN_REP 		0x1007
#define SET_AUTH_APN_RSP 		0x1008

#define SEARCH_PLMN_REP			0x1009
#define SEARCH_PLMN_RSP			0x100A
#define SET_PLMN_REP			0x100B
#define SET_PLMN_RSP			0x100C
#define SET_BANDS_REP			0x100D
#define SET_BANDS_RSP			0x100E

#define REQUESTING_SEARCH_PLMN		0x1
#define SEARCH_PLMN_RSP_SEARCHING_PLMN	0x0
#define SEARCH_PLMN_RSP_ERROR		0x1

#define SET_AUTO_PLMN			0x0
#define SET_MANUAL_PLMN			0x1

#define PIN_MANAGEMENT_REP 		0x2001
#define PIN_MANAGEMENT_RSP 		0x2002
#define DISENABLE_PIN 			0x0
#define ENABLE_PIN 			0x1
#define UNLOCK_PIN			0x2
#define MODIFY_PIN 			0x3

#define PUK_MANAGEMENT_REP		0x2003
#define PUK_MANAGEMENT_RSP		0x2004

#define QUERY_USIM_REP 			0x2005
#define QUERY_USIM_ID 			0x1
#define QUERY_USIM_RSP 			0x2006

#define QUERY_USIM_RSP_NO_USIM 		0x0
#define QUERY_USIM_RSP_USIM_READY 	0x1
#define QUERY_USIM_RSP_PINLOCKED 	0x2
#define QUERY_USIM_RSP_PUKLOCKED 	0x3
#define QUERY_USIM_RSP_SIMULATOR 	0x4

#define GET_INFORMATION_REP 		0x3001
#define GET_INFORMATION_RSP 		0x3002
#define GET_LTE_DEVICE_INFORMATION 	0x1
#define GET_WAN_STATUS 			0x2
#define GET_RUNTIME			0x3
#define GET_LTE_DIAGNOSTIC_INFORMATION	0x4
#define	GET_LTE_THROUGHPUT		0x5
#define   GET_WORK_MODE			0x6
#define	GET_CONNECTION_MODE		0x7

#define GET_STATUS_REP 			0x3003
#define GET_STATUS_USIM_STATUS 		0x1
#define GET_STATUS_CONNECTION_STATUS	0x2
#define GET_STATUS_RSP			0x3004

#define USIM_STATUS_NO_USIM 		0x0
#define USIM_STATUS_USIM_READY 		0x1
#define USIM_STATUS_PINLOCKED 		0x2
#define USIM_STATUS_PUKLOCKED 		0x3
#define USIM_STATUS_SIMLOCKED 		0x4
#define USIM_STATUS_SIMULATOR 		0x5

#define LTE_STATUS_CONNECTED 		0x1
#define LTE_STATUS_CONNECTING 		0x2
#define LTE_STATUS_DISCONNECTED 	0x3
#define LTE_STATUS_AIRPLANEMODE 	0x4

#define GET_APN_REP			0x3005
#define GET_APN_RSP			0x3006

#define GET_AUTH_APN_REP		0x3007
#define GET_AUTH_APN_RSP		0x3008

#define GET_CM_STATUS_REP		0x3009
#define GET_CM_STATUS_RSP		0x300A

#define MANUAL_CONNECT_REP 		0x4001
#define MANUAL_CONNECT_RSP 		0x4002

#define SET_FACTORY_REP 		0x4003
#define SET_FACTORY_RSP 		0x4004
#define REBOOT_REP			0x4005
#define REBOOT_RSP			0x4006

#define SET_AIRPLANEMODE_REP			0x4007
#define SET_AIRPLANEMODE_RSP			0x4008

#define ENABLE_AIRPLANE_MODE		0x1
#define DISABLE_AIRPLANE_MODE		0x0

#define MANUAL_CONNECT_LTE 		0x1
#define MANUAL_DISCONNECT_LTE		0x0

#define REQUEST_UPGRADE_REP		0x5001
#define REQUEST_UPGRADE_RSP		0x5002

#define REQUEST_UPGRADING_FW		0x1
#define REQUEST_ABORT_UPGRADING		0x2


#define ABORT_UPGRADING_REASON_INTERNAL_ERROR 0x01
#define ABORT_UPGRADING_REASON_CANCEL 	0x02

#define UPGRADE_REP			0x5003
#define UPGRADE_RSP			0x5004

#define REQUEST_QUIT_REP			0x5005
#define REQUEST_QUIT_RSP			0x5006

#define UPGRADED_SUCCESSFUL		0x2
#define UPGRADED_FAIL			0x3
#define REQUEST_STARTING_TEST_RF_REP 	0x900A
#define REQUEST_STARTING_TEST_RF_RSP 	0x900B

#define REQUEST_ENDING_TEST_RF_REP 	0x900C
#define REQUEST_ENDING_TEST_RF_RSP 	0x900D

#define REQUEST_TEST_RF_REP 		0x900E
#define REQUEST_TEST_RF_RSP 		0x900F

#define REQUEST_TEST_RF_READ_RX_RESULTS_REP 0x9010
#define REQUEST_TEST_RF_READ_RX_RESULTS_RSP 0x9011

#define REQUEST_ABORT_TEST_RF_REP 	0x9012
#define REQUEST_ABORT_TEST_RF_RSP 	0x9013

//#define DUMP_SOCKET_BUF

static unsigned char send_data_buf[MAX_MESSAGE_LEN];
static unsigned char temp_data_buf[MAX_MESSAGE_LEN];

#if 0
static struct option const long_options[] = {
	{"server ipv4 address",			required_argument,		NULL,'s'},
	{"listen port",				required_argument,		NULL,'p'},
	{"bind interface",			required_argument,		NULL,'r'},
	{"help",				no_argument,			NULL, 'h'},
	{"Message ID",				required_argument,		NULL, 'M'},
	{"Work Mode",				required_argument,		NULL, 'W'},
	{"Connection Mode",			required_argument,		NULL, 'C'},
	{"APN",					required_argument,		NULL, 'A'},
	{"APN Index",				required_argument,		NULL, 'I'},

	{"APN Active",				required_argument,		NULL, 'V'},
	{"APN Default PDN",			required_argument,		NULL, 'j'},
	{"APN Active Auth",			required_argument,		NULL, 'Q'},

	{"APN Procotol Type ID",		required_argument,		NULL, 'P'},
	{"APN Authentication Protocol ID",	required_argument,		NULL, 'a'},
	{"APN Authentication User",		required_argument,		NULL, 'U'},
	{"APN Authentication Password",		required_argument,		NULL, 'w'},
	{"APN Authentication Host",		required_argument,		NULL, 'o'},
	{"Poll/ Select Timout",			required_argument,		NULL, 'T'},
	{"Connect to LTE",			no_argument,			NULL, 'L'},
	{"Disconnect to LTE",			no_argument,			NULL, 'D'},
	{"Factory setting",			no_argument,			NULL, 'F'},	
	{"Reboot",				no_argument,			NULL, 'R'},	
	{"Get information ID",			required_argument,		NULL, 'G'},	
	{"Get status ID",			required_argument,		NULL, 'S'},	
	{"PIN Management ID",			required_argument,		NULL, 'u'},
	{"PIN",					required_argument,		NULL, 'i'},
	{"NEW PIN",				required_argument,		NULL, 'E'},
	{"PUK",					required_argument,		NULL, 'K'},
	{"Support Multiple",			no_argument,			NULL, 'l'},
	{"Multiple ipv4 address ",		required_argument,		NULL, 'H'},
	{"Multiple Listen Port",		required_argument,		NULL, 'e'},
	{"Multiple Listen Interface",		required_argument,		NULL, 'n'},
	{"upgrading firmware path",		required_argument,		NULL, 'f'},

	{"abort upgrading firmware",		required_argument,		NULL, 'k'},

	{"request starting test RF",		no_argument,			NULL, 't'},
	{"request ending test RF",		no_argument,			NULL, 'N'},
	{"request test RF",			required_argument,		NULL, 'm'},
	{"request test RF -EARFCN",		required_argument,		NULL, 'c'}, 
	/* EARFCN decimal value as per LTE spec*/
	{"request test RF -time",		required_argument,		NULL, 'd'}, 
	/* 1-600000 ms, 0 only for TX continue*/
	{"request test RF -RX_antenna/TX_type",	required_argument,		NULL, 'g'},
	/*0 RX primary antenna 1 RX secondary antenna  0 - SC-FDMA 1 – CW (continuous waveform) */
	{"request test RF -TX Power",		required_argument,		NULL, 'O'}, 
	/*Absolute output power [dBm]*/
	{"request test RF -TX param",		required_argument,		NULL, 'B'},
	/*For SC-FDMA - BW:
		0 – 1.4 MHz
		1 – 3 MHz
		2 – 5 MHz
		3 – 10 MHz
		4 – 15 MHz
		5 – 20 MHz
	  ForCW:
		offset to central frequency in Hz
	*/
	{"request test RF -Read RX Results",	no_argument,			NULL, 'b'},
	{"request test RF -Abort rf test",	no_argument,			NULL, 'J'},
	{"request search PLMN",			no_argument,			NULL, 'z'},
	{"request set PLMN",			required_argument,		NULL, 'Z'},
	{"request set AirPlaneMode",		required_argument,		NULL, 'Y'},
	{NULL,					0, 				NULL, 0},
};
#endif

static char is_request_search_plmn 		;
static char is_request_set_plmn    		;
static char is_request_set_plmn_type_auto 	;
static char request_set_plmn[64] = {0} 	;

static unsigned char bindinterface[128]           = {0} ;
static unsigned char is_request_abort_test_rf     = 0 ;
static unsigned char is_request_starting_test_rf  = 0 ;
static unsigned char is_request_ending_test_rf    = 0 ;
static unsigned char request_test_rf_type         = 0 ;
static unsigned long request_test_rf_EARFCN       = 0 ;
static unsigned long request_test_rf_time         = 0 ;
static unsigned char request_test_rf_rxtx_param1  = 0 ;
static unsigned long request_test_rf_tx_power     = 0 ;
static unsigned long request_test_rf_tx_param1    = 0 ;

static unsigned char is_request_test_rf_read_rx_results = 0 ;

static char new_fw_file[256] = {0} ;
static unsigned long ugrade_fw_size = 0 ;
static unsigned long upgrading_once_size = 0 ;
static int is_upgrade_fw = 0 ;
static unsigned long upgrading_rsp_index = 0 ;
static unsigned int current_msgid = 0 ; 
static unsigned int upgraded_results = 0 ;

static unsigned long request_abort_upgrading_reason = 0 ;
static int multiple_port = 0 ;
static char multiple_interface[64] ={0};
static char multiple_domain[64] ={0};
static int is_support_multiple = 0 ;

static unsigned int args_MsgID = 0 ;
static unsigned int timout = 100 ;

static unsigned int workmode = 0 ;
static unsigned int connectmode = 0 ;

static int manualconnectlte = 0 ;
static int isfactorysetting = 0 ;
static int isreboot = 0 ;
static int isairplanemode = 0 ;
static int get_information_id = 0;
static int get_status_id = 0 ;
static int pin_managemen_id = 0 ;

static unsigned char is_process_exit = 0 ;
static unsigned char pin[USIM_BUF_LEN];
static unsigned char newpin[USIM_BUF_LEN];
static unsigned char puk[USIM_BUF_LEN];

static char apn[MAX_MESSAGE_LEN];
static char apnindex = 0 ;
static char apnprotocolid = 0 ;
static unsigned char is_apnactive = 0;
static unsigned char is_apndefaultgw = 0;

static unsigned char is_apnactiveauth = 0 ;
static char apnauth_protocolid = 0 ;
static char apnauth_username[MAX_MESSAGE_LEN];
static char apnauth_password[MAX_MESSAGE_LEN];
static char apnauth_host[MAX_MESSAGE_LEN];

char system_setting_request_args[128] = {0};

static void dump_buf(unsigned char* buf,int len){
#ifdef DUMP_SOCKET_BUF
	if(len <= 0)
		return ;
	int line_count = 15 ;
	char printbuf[64] = {'\0'};
	int i = 0 ;
	int line = 0 ;

	LTE_LOG(LTE_LOG_INFO, "Dump buffer ,buffer lenth is %d",len);
	for(i = 0 ; i < len; i++)
	{	
		if(i == 0 && line == 0)
		{
			LTE_LOG(LTE_LOG_INFO, "Line %d: ",line);
		}
		sprintf(printbuf, "%s %.2X", printbuf, buf[i]);
		if(i != 0 && (i % line_count) == 0){
			LTE_LOG(LTE_LOG_INFO, "%s",printbuf);
			line++;
			memset(printbuf, '\0', sizeof(printbuf));
			LTE_LOG(LTE_LOG_INFO, "Line %d: ",line);		
		}
	}
	LTE_LOG(LTE_LOG_INFO, "%s",printbuf);
#endif
}

int is_digits(char* chars){
	if(chars == NULL)
		return 0 ;
	int length,i ;
	length = strlen(chars);
	if(length == 0)
		return 0 ;
	for(i = 0 ; i < length ; i++){
		if(!isdigit(chars[i])){
			if(i == 0 && chars[i] == '-'){
				continue ;
			}
			return 0 ;
		}
	}
	return 1 ;
}
static unsigned int AtelGetOneMsg(const unsigned char *Data, unsigned int Len)
{
	unsigned int wMsgLen = 0;
	if(Data != NULL && Len >= 0){		
		if(*Data == SYNC_CHAR)
		{
			for(wMsgLen = 1; wMsgLen < Len && Data[wMsgLen] != SYNC_CHAR; wMsgLen++);
		}			
	}
	return wMsgLen;
}

static int AtelSkipEcsByte(unsigned char *ResBuf, unsigned char* DesBuf,unsigned int len)
{
	ALTAIR_BOOL bEsc=FALSE;
	int i=0,j=0;
	for(i=0,j=0;i<len;i++)
	{
		if(bEsc == TRUE)
		{
			DesBuf[j++]=ResBuf[i]^MOD_CHAR;
			bEsc=FALSE;
		}
		else
		{
			if(ResBuf[i]==ESC_CHAR)
				bEsc=TRUE;
			else
				DesBuf[j++]=ResBuf[i];
		}
	}
	return j;
}

static void AtelAppendByte(unsigned char *Buf,unsigned int* offset, unsigned char byte)
{
	if(byte==ESC_CHAR||byte==SYNC_CHAR)
	{
		*(Buf+*offset)=ESC_CHAR;
		(*offset)++;
		*(Buf+*offset)=byte^MOD_CHAR;
		(*offset)++;
	}
	else
	{
		*(Buf+*offset)=byte;
		(*offset)++;
	}
}

static ALTAIR_BOOL AddOneByte(unsigned char *pBuf, unsigned int* pOffset, unsigned char byVal)
{
 if ((pBuf == NULL) || (pOffset == NULL))
  return FALSE;

 if ((byVal == SYNC_CHAR) || (byVal== ESC_CHAR))
 {
  //add ESC CHAR
  *(pBuf + *pOffset) = ESC_CHAR;
  (*pOffset)++;

  *(pBuf + *pOffset) = byVal ^ MOD_CHAR;
  (*pOffset)++;
 }
 else
 {
  //copy the char
  *(pBuf + *pOffset) = byVal;
  (*pOffset)++;
 }
 return TRUE;
}



static int ConstructSendBuf(unsigned char* pDst, unsigned int  nMaxLen, unsigned int  wMsgID,unsigned char* nMsgCont, unsigned int  nMsgLen)
{
 int i, index = 0;
 unsigned int checkSum = 0x00;

 if ((nMaxLen == 0) || (pDst == NULL))
  return -1;
 
 /*Dst buffer is not enough*/
 /*max Dst buffer should >= 1(sync)+2(length)+2(id)+non-fix(Msglen)+1*/
 if (nMaxLen < (2* nMsgLen + 11))
  return -2;

 memset(pDst, 0, nMaxLen);
 pDst[index++] = SYNC_CHAR;

 AddOneByte(pDst, &index, (nMsgLen + 2) & 0xff);
 checkSum += (nMsgLen + 2) & 0xff;

 AddOneByte(pDst, &index, (nMsgLen + 2) >> 8);
 checkSum += (nMsgLen + 2) >> 8;
 
 AddOneByte(pDst, &index, wMsgID & 0xff);
 checkSum += wMsgID & 0xff;
 
 AddOneByte(pDst,&index,wMsgID>>8);
 checkSum += wMsgID >> 8;

 if((nMsgCont != NULL) && (nMsgLen > 0 ))
 {
  for(i = 0; i < nMsgLen; i++)
  { 
   checkSum += nMsgCont[i];
   AddOneByte(pDst, &index, nMsgCont[i]);
  }
 }

 AddOneByte(pDst, &index, -checkSum);

 return index;
}

static int Construct_Upgrading_Packet(unsigned char* pDst, unsigned int  nMaxLen,unsigned char* srcdata,unsigned int srclen)
{
	int sendlength = 0 ;
	sendlength = ConstructSendBuf(pDst,nMaxLen,UPGRADE_REP,srcdata,srclen);
	return sendlength ;
}


static int Construct_set_packect(unsigned char* pDst, unsigned int  nMaxLen, unsigned int  wMsgId)
{
	LTE_LOG(LTE_LOG_INFO, "construct set packect\n");
	
	int sendlength = 0 ;
	int index = 0 ;
	int length = 0 ;
	switch(wMsgId){
		case SET_WORK_MODE_REP:
		{
			LTE_LOG(LTE_LOG_INFO, "construct set work mode packect \n");

			memset(temp_data_buf,0,MAX_MESSAGE_LEN);
			if(workmode == WORK_MODE_ROUTER){
				temp_data_buf[index++] = WORK_MODE_ROUTER ;
			}else if(workmode == WORK_MODE_BRIDGE){
				temp_data_buf[index++] = WORK_MODE_BRIDGE ;
			}
			sendlength = ConstructSendBuf(pDst,nMaxLen,wMsgId,temp_data_buf,index);
			break;
		}
		case SET_CONNECT_MODE_REP:
		{
			LTE_LOG(LTE_LOG_INFO, "construct set connect mode packect\n");

			memset(temp_data_buf,0,MAX_MESSAGE_LEN);
			
			if(connectmode == CONNECT_MODE_AUTO){
				temp_data_buf[index++] = CONNECT_MODE_AUTO ;
			}else if(connectmode == CONNECT_MODE_MANUAL){
				temp_data_buf[index++] = CONNECT_MODE_MANUAL ;	
			}
			sendlength = ConstructSendBuf(pDst,nMaxLen,wMsgId,temp_data_buf,index);
			break;
		}
		case SET_APN_REP:
		{
			LTE_LOG(LTE_LOG_INFO, "construct set apn packect,\n");

			memset(temp_data_buf,0,MAX_MESSAGE_LEN);
			index = 0 ;
			temp_data_buf[index++] = apnindex ;
			temp_data_buf[index++] = is_apnactive ;
			temp_data_buf[index++] = is_apndefaultgw ;

			temp_data_buf[index++] = apnprotocolid ;
			length = strlen(apn);
			memcpy(temp_data_buf+index,apn,length);
			index  += length ;

			dump_buf(temp_data_buf,index);

			sendlength = ConstructSendBuf(pDst,nMaxLen,wMsgId,temp_data_buf,index);
			break;
		}
		case SET_AUTH_APN_REP:
		{
			LTE_LOG(LTE_LOG_INFO, "construct set  adv apn packect\n");

			memset(temp_data_buf,0,MAX_MESSAGE_LEN);
			index = 0 ;
			temp_data_buf[index++] = apnindex ;
			temp_data_buf[index++] = is_apnactiveauth ;
			if(apnauth_protocolid == AUTHEN_PROTOCOL_PAP){
				temp_data_buf[index++] = AUTHEN_PROTOCOL_PAP ;
			}else if (apnauth_protocolid == AUTHEN_PROTOCOL_CHAP){
				temp_data_buf[index++] = AUTHEN_PROTOCOL_PAP ;
			}
			
			length = strlen(apnauth_username);
			memcpy(temp_data_buf+index,apnauth_username,length);
			index += length;
			temp_data_buf[index++] = '\n';

			length = strlen(apnauth_password);
			memcpy(temp_data_buf+index,apnauth_password,length);
			index += length;
			temp_data_buf[index++] = '\n';

			length = strlen(apnauth_host);
			memcpy(temp_data_buf+index,apnauth_host,length);
			index += length;
			temp_data_buf[index++] = '\n';
			sendlength = ConstructSendBuf(pDst,nMaxLen,wMsgId,temp_data_buf,index);
			break;
		}
		case SEARCH_PLMN_REP:
		{
			memset(temp_data_buf,0,MAX_MESSAGE_LEN);
			if(is_request_search_plmn == 1){
				temp_data_buf[index++] = REQUESTING_SEARCH_PLMN ;
				sendlength = ConstructSendBuf(pDst,nMaxLen,wMsgId,temp_data_buf,index);
			}
			break;
		}
		case SET_PLMN_REP:
		{
			if(is_request_set_plmn == 1){
				if(is_request_set_plmn_type_auto == 1){
					temp_data_buf[index++] = SET_AUTO_PLMN;
				}else{
					temp_data_buf[index++] = SET_MANUAL_PLMN ;
				}			
			}
			strcpy(temp_data_buf+index,request_set_plmn);
			index += strlen(request_set_plmn);
			sendlength = ConstructSendBuf(pDst,nMaxLen,wMsgId,temp_data_buf,index);
			break;
		}
		case SET_BANDS_REP:
		{
			unsigned char bands_buf[64] = {0} ;
			int band_index = 0 ;
			if(strlen(system_setting_request_args) > 0){
				char* p  = strtok(system_setting_request_args,",");
				printf("band:[%s]\n",p);
				if(is_digits(p)){
					bands_buf[band_index++] = (unsigned char)(atoi(p));
				}
				
				if(p != NULL){
					while((p = strtok(NULL,",")) != NULL){
						printf("band:[%s]\n",p);
						if(is_digits(p)){
							bands_buf[band_index++] = (unsigned char)(atoi(p));
						}
					}
				}
				
				if(band_index != 0){
					temp_data_buf[index++] = band_index;
					memcpy(temp_data_buf+index,bands_buf,band_index);
					index = band_index + 1;
					sendlength = ConstructSendBuf(pDst,nMaxLen,wMsgId,temp_data_buf,index);				
				}else{
					printf("Incorrect bands,the band must be number.\n");
					sendlength = 0 ;
				}
			}else{
				printf("please input binding bands.\n");
				sendlength = 0 ;
			}
			break;
		}
	}
	return sendlength ;
}

static int Construct_usim_packect(unsigned char* pDst, unsigned int  nMaxLen, unsigned int  wMsgID)
{
	LTE_LOG(LTE_LOG_INFO, "construct usim packect\n");

	int sendlength = 0 ;
	int index = 0 ;
	int length = 0 ;
	switch(wMsgID){
		case PIN_MANAGEMENT_REP:
		{
			memset(temp_data_buf,0,MAX_MESSAGE_LEN);
			if(pin_managemen_id == DISENABLE_PIN || pin_managemen_id == ENABLE_PIN || pin_managemen_id == UNLOCK_PIN || pin_managemen_id == MODIFY_PIN){
				if(pin_managemen_id == DISENABLE_PIN){
					temp_data_buf[index++] = DISENABLE_PIN ;
				}else if(pin_managemen_id == ENABLE_PIN){
					temp_data_buf[index++] = ENABLE_PIN ;
				}else if(pin_managemen_id == UNLOCK_PIN){
					temp_data_buf[index++] = UNLOCK_PIN ;
				}else if(pin_managemen_id == MODIFY_PIN){
					temp_data_buf[index++] = MODIFY_PIN ;
				}
				int offset = strlen(pin);
				int i= 0 ;
				for(i=0;i<8;i++){
					if(i<offset){
						temp_data_buf[index+i] = pin[i] ;
					}else{
						temp_data_buf[index+i] = '\0';
					}
				}
				index += 8 ;
				if(pin_managemen_id == MODIFY_PIN){
					offset = strlen(newpin);
					for(i=0;i<8;i++){
						if(i<offset){
							temp_data_buf[index+i] = newpin[i];
						}else{
							temp_data_buf[index+i] = '\0';
						}
					}
					index += 8 ;
				}
			}
			sendlength = ConstructSendBuf(pDst,nMaxLen,wMsgID,temp_data_buf,index);
			break;
		}
		case PUK_MANAGEMENT_REP:
		{
			LTE_LOG(LTE_LOG_INFO, "construct puk new pin packect\n");

			memset(temp_data_buf,0,MAX_MESSAGE_LEN);
			int offset = strlen(puk);
			int i= 0 ;
			for(i=0;i<8;i++){
				if(i<offset){
					temp_data_buf[index+i] = puk[i] ;
				}else{
					temp_data_buf[index+i] = '\0';
				}
			}	
			index += 8 ;
			offset = strlen(newpin);
			for(i=0;i<8;i++){
				if(i<offset){
					temp_data_buf[index+i] = newpin[i];
				}else{
					temp_data_buf[index+i] = '\0';
				}	
			}
			index += 8 ;	
			sendlength = ConstructSendBuf(pDst,nMaxLen,wMsgID,temp_data_buf,index);
			break;
		}
		case QUERY_USIM_REP:
		{
			LTE_LOG(LTE_LOG_INFO, "construct query usim packect\n");
			
			memset(temp_data_buf,0,MAX_MESSAGE_LEN);
			temp_data_buf[index++] = QUERY_USIM_ID ;
			sendlength = ConstructSendBuf(pDst,nMaxLen,wMsgID,temp_data_buf,index);
			break;
		}

	}
	return sendlength ;
}

static int Construct_get_packect(unsigned char* pDst, unsigned int  nMaxLen, unsigned int  wMsgID)
{
	LTE_LOG(LTE_LOG_INFO, "construct get packect\n");

	int sendlength = 0 ;
	int index = 0 ;
	int length = 0 ;
	switch(wMsgID){
		case GET_INFORMATION_REP:
		{
			LTE_LOG(LTE_LOG_INFO, "construct get information packect\n");

			memset(temp_data_buf,0,MAX_MESSAGE_LEN);
			if(get_information_id == GET_LTE_DEVICE_INFORMATION){
				temp_data_buf[index++] = GET_LTE_DEVICE_INFORMATION ;
			}else if(get_information_id == GET_WAN_STATUS){
				temp_data_buf[index++] = GET_WAN_STATUS ;
			}else if(get_information_id == GET_RUNTIME){
				temp_data_buf[index++] = GET_RUNTIME ;
			}else if(get_information_id == GET_LTE_DIAGNOSTIC_INFORMATION){
				temp_data_buf[index++] = GET_LTE_DIAGNOSTIC_INFORMATION ;
			}else if(get_information_id == GET_LTE_THROUGHPUT){
				temp_data_buf[index++] = GET_LTE_THROUGHPUT ;
			}else if(get_information_id == GET_WORK_MODE){
				temp_data_buf[index++] = GET_WORK_MODE ;
			}else if(get_information_id == GET_CONNECTION_MODE){
				temp_data_buf[index++] = GET_CONNECTION_MODE ;
			}
			sendlength = ConstructSendBuf(pDst,nMaxLen,wMsgID,temp_data_buf,index);
			break;
		}
		case GET_STATUS_REP:
		{
			LTE_LOG(LTE_LOG_INFO, "construct get status packect\n");

			memset(temp_data_buf,0,MAX_MESSAGE_LEN);
			if(get_status_id == GET_STATUS_USIM_STATUS){
				temp_data_buf[index++] = GET_LTE_DEVICE_INFORMATION ;
			}else if(get_status_id == GET_STATUS_CONNECTION_STATUS){
				temp_data_buf[index++] = GET_WAN_STATUS ;
			}
			sendlength = ConstructSendBuf(pDst,nMaxLen,wMsgID,temp_data_buf,index);
			break;
		}
		case GET_APN_REP:
		{
			LTE_LOG(LTE_LOG_INFO, "construct get apn packect\n");
		
			memset(temp_data_buf,0,MAX_MESSAGE_LEN);
			temp_data_buf[index++] = apnindex ;
			sendlength = ConstructSendBuf(pDst,nMaxLen,wMsgID,temp_data_buf,index);
			break;
		}
		case GET_AUTH_APN_REP:
		{
			LTE_LOG(LTE_LOG_INFO, "construct get apn packect\n");
			
			memset(temp_data_buf,0,MAX_MESSAGE_LEN);
			temp_data_buf[index++] = apnindex ;
			sendlength = ConstructSendBuf(pDst,nMaxLen,wMsgID,temp_data_buf,index);
			break;
		}
		case GET_CM_STATUS_REP:
		{
			LTE_LOG(LTE_LOG_INFO, "construct get cm status \n");
	
			sendlength = ConstructSendBuf(pDst,nMaxLen,wMsgID,NULL,index);
			break;
		}
	}
	return sendlength ;
}

static int Construct_special_packect(unsigned char* pDst, unsigned int  nMaxLen, unsigned int  wMsgID)
{
	LTE_LOG(LTE_LOG_INFO, "construct special packect\n");

	int sendlength = 0 ;
	int index = 0 ;
	int length = 0 ;
	switch(wMsgID){
		case REQUEST_UPGRADE_REP:
		{
			LTE_LOG(LTE_LOG_INFO, "construct upgrade packect\n");

			if(request_abort_upgrading_reason != 0){
				LTE_LOG(LTE_LOG_INFO, "construct abort upgrade packect\n");

				temp_data_buf[index++] = REQUEST_ABORT_UPGRADING ;
				temp_data_buf[index++] = (request_abort_upgrading_reason >> 24) & 0xff;
				temp_data_buf[index++] = (request_abort_upgrading_reason >> 16) & 0xff;
				temp_data_buf[index++] = (request_abort_upgrading_reason >> 8) & 0xff;
				temp_data_buf[index++] = request_abort_upgrading_reason & 0xff;
				sendlength = ConstructSendBuf(pDst,nMaxLen,wMsgID,temp_data_buf,index);
			}else{
				if(is_upgrade_fw){
					struct stat ufw ;
					if(stat(new_fw_file,&ufw) == -1){
						LTE_LOG(LTE_LOG_ERR, "stat");
						return -1 ;			
					}
					
					ugrade_fw_size = ufw.st_size ;

					LTE_LOG(LTE_LOG_INFO, "ugrade_fw_size:[%d]\n",ugrade_fw_size);

					memset(temp_data_buf,0,MAX_MESSAGE_LEN);
					temp_data_buf[index++] = REQUEST_UPGRADING_FW ;
					temp_data_buf[index++] = (ugrade_fw_size >> 24) & 0xff;
					temp_data_buf[index++] = (ugrade_fw_size >> 16) & 0xff;
					temp_data_buf[index++] = (ugrade_fw_size >> 8) & 0xff;
					temp_data_buf[index++] = ugrade_fw_size & 0xff;
					sendlength = ConstructSendBuf(pDst,nMaxLen,wMsgID,temp_data_buf,index);
				}		
			}
			break;
		}
		case UPGRADE_REP:
		{
			if(is_request_starting_test_rf){
				sendlength = ConstructSendBuf(pDst,nMaxLen,wMsgID,NULL,0);
			}
			break;
		}
		case REQUEST_QUIT_REP:
		{
			LTE_LOG(LTE_LOG_INFO, "construct quit socket \n");
	
			sendlength = ConstructSendBuf(pDst,nMaxLen,wMsgID,NULL,index);
			break;
		}
	}
	return sendlength ;	
}

static int Construct_test_packect(unsigned char* pDst, unsigned int  nMaxLen, unsigned int  wMsgID)
{
	int sendlength = 0 ;
	int index = 0 ;
	int length = 0 ;
	switch(wMsgID){
		case REQUEST_STARTING_TEST_RF_REP:
		{
			if(is_request_starting_test_rf){
				sendlength = ConstructSendBuf(pDst,nMaxLen,wMsgID,NULL,0);
			}
			break;
		}
		case REQUEST_ENDING_TEST_RF_REP:
		{
			if(is_request_ending_test_rf){
				sendlength = ConstructSendBuf(pDst,nMaxLen,wMsgID,NULL,0);
			}
			break;
		}
		case REQUEST_TEST_RF_REP:
		{
			index = 0 ;
			temp_data_buf[index++] = request_test_rf_type;

			temp_data_buf[index++] = (request_test_rf_EARFCN >> 24) & 0xff;
			temp_data_buf[index++] = (request_test_rf_EARFCN >> 16) & 0xff;
			temp_data_buf[index++] = (request_test_rf_EARFCN >> 8) & 0xff;
			temp_data_buf[index++] = request_test_rf_EARFCN & 0xff;

			temp_data_buf[index++] = (request_test_rf_time >> 24) & 0xff;
			temp_data_buf[index++] = (request_test_rf_time >> 16) & 0xff;
			temp_data_buf[index++] = (request_test_rf_time >> 8) & 0xff;
			temp_data_buf[index++] = request_test_rf_time & 0xff;

			temp_data_buf[index++] = request_test_rf_rxtx_param1;

			temp_data_buf[index++] = (request_test_rf_tx_power >> 24) & 0xff;
			temp_data_buf[index++] = (request_test_rf_tx_power >> 16) & 0xff;
			temp_data_buf[index++] = (request_test_rf_tx_power >> 8) & 0xff;
			temp_data_buf[index++] = request_test_rf_tx_power & 0xff;

			temp_data_buf[index++] = (request_test_rf_tx_param1 >> 24) & 0xff;
			temp_data_buf[index++] = (request_test_rf_tx_param1 >> 16) & 0xff;
			temp_data_buf[index++] = (request_test_rf_tx_param1 >> 8) & 0xff;
			temp_data_buf[index++] = request_test_rf_tx_param1 & 0xff;

			sendlength = ConstructSendBuf(pDst,nMaxLen,wMsgID,temp_data_buf,index);
			break;
		}
		case REQUEST_TEST_RF_READ_RX_RESULTS_REP:
		{
			if(is_request_test_rf_read_rx_results){
				sendlength = ConstructSendBuf(pDst,nMaxLen,wMsgID,temp_data_buf,index);
			}
			break;
		}
		case REQUEST_ABORT_TEST_RF_REP:
		{
			if(is_request_abort_test_rf){
				sendlength = ConstructSendBuf(pDst,nMaxLen,wMsgID,temp_data_buf,index);
			}
			break;
		}
	}
	return sendlength ;	
}



static int Construct_operation_packect(unsigned char* pDst, unsigned int  nMaxLen, unsigned int  wMsgID)
{
	LTE_LOG(LTE_LOG_INFO, "construct operation packect\n");

	int sendlength = 0 ;
	int index = 0 ;
	int length = 0 ;
	switch(wMsgID){
		case MANUAL_CONNECT_REP:
		{
			LTE_LOG(LTE_LOG_INFO, "construct manaual connect packect\n");

			memset(temp_data_buf,0,MAX_MESSAGE_LEN);
			if(manualconnectlte == MANUAL_CONNECT_LTE){
				temp_data_buf[index++] = MANUAL_CONNECT_LTE ;
			}else if(manualconnectlte == MANUAL_DISCONNECT_LTE){
				temp_data_buf[index++] = MANUAL_DISCONNECT_LTE ;
			}
			sendlength = ConstructSendBuf(pDst,nMaxLen,wMsgID,temp_data_buf,index);
			break;
		}
		case SET_FACTORY_REP:
		{
			LTE_LOG(LTE_LOG_INFO, "construct factory setting packect\n");

			if(isfactorysetting){
				sendlength = ConstructSendBuf(pDst,nMaxLen,wMsgID,NULL,0);
			}
			break;
		}
		case REBOOT_REP:
		{
			LTE_LOG(LTE_LOG_INFO, "construct reboot packect\n");

			if(isreboot){
				sendlength = ConstructSendBuf(pDst,nMaxLen,wMsgID,NULL,0);
			}
			break;
		}
		case SET_AIRPLANEMODE_REP:
		{
			LTE_LOG(LTE_LOG_INFO, "construct set airplanemode packect\n");

			memset(temp_data_buf,0,MAX_MESSAGE_LEN);
			if(isairplanemode == 1){
				temp_data_buf[index++] = ENABLE_AIRPLANE_MODE ;
			}else{
				temp_data_buf[index++] = DISABLE_AIRPLANE_MODE ;
			}
			sendlength = ConstructSendBuf(pDst,nMaxLen,wMsgID,temp_data_buf,index);
			break;
		}
	}
	return sendlength ;
}

static void sepstr(char sepchar,char* data){
	int i = 0 ;
	int j = 0 ;
	int datalength = strlen(data);
	while(i < datalength){
		if(i == 0){
			LTE_LOG(LTE_LOG_INFO, "%d:",j++);
		}
		if(data[i] == sepchar){
			LTE_LOG(LTE_LOG_INFO, "\n%d:",j++);	
		}else{
			LTE_LOG(LTE_LOG_INFO, "%c",data[i]);
		}
		i++;
	}
}


static int sepkeystr(char sepchar,int datalength,char* data,char* key[],int keylen){
	int i = 0 ;
	int j = 0 ;
	int k = 0;
	char buf[64];
	while(i < datalength && j <= keylen){
		if(data[i] == sepchar){
			if(j < keylen){
				buf[k] = '\0';
			    	LTE_LOG(LTE_LOG_INFO, "%s[%d]: %s",key[j], j, buf);
				k = 0;
			}
			j++;
		}else{
			buf[k++] = data[i];
		}
		i++;
	}
	return i ;
}
static char *PLMNLISTS_key[64] = {"PLMN STATUS","PLMN NAME1","PLMN NAME2","PLMN NUMBER"};

static void handle_set_rsp(unsigned int MsgId,unsigned char* data,unsigned int length){
	int index = 0 ;
	switch(MsgId){
		case SET_WORK_MODE_RSP:
		{
			LTE_LOG(LTE_LOG_INFO, "handle response work mode rep\n");

			if(data[index] == RSP_OK){
				LTE_LOG(LTE_LOG_INFO, "set work mode ok\n");	
			}else if(data[index] == RSP_ERROR){
				LTE_LOG(LTE_LOG_INFO, "set work mode FAIL\n");
			}
			break;
		}
		case SET_CONNECT_MODE_RSP:
		{
			LTE_LOG(LTE_LOG_INFO, "handle response connect mode rep\n");

			if(data[index] == RSP_OK){
				LTE_LOG(LTE_LOG_INFO, "set connect mode ok\n");			
			}else if(data[index] == RSP_ERROR){
				LTE_LOG(LTE_LOG_INFO, "set connect mode FAIL\n");			
			}
			break;
		}
		case SET_APN_RSP:
		{
			LTE_LOG(LTE_LOG_INFO, "handle response set apn rep\n");

			if(data[index] == RSP_OK){
				LTE_LOG(LTE_LOG_INFO, "set APN ok\n");			
			}else if(data[index] == RSP_ERROR){
				LTE_LOG(LTE_LOG_INFO, "set APN FAIL\n");			
			}			
			if(data[index+1] == 0x1){
				LTE_LOG(LTE_LOG_INFO, "APN Index:[%d] ok.\n",data[index]);
			}else if(data[index+1] == 0x0){
				LTE_LOG(LTE_LOG_INFO, "APN Index:[%d] fail.\n",data[index]);
			}else{
				LTE_LOG(LTE_LOG_INFO, "unknown.\n");				
			}
			break;
		}
		case SET_AUTH_APN_RSP:
		{
			LTE_LOG(LTE_LOG_INFO, "handle response set adv apn rep\n");

			if(data[index] == RSP_OK){
				LTE_LOG(LTE_LOG_INFO, "set ADV APN ok\n");			
			}else if(data[index] == RSP_ERROR){
				LTE_LOG(LTE_LOG_INFO, "set ADV APN FAIL\n");			
			}
			if(data[index+1] == 0x1){
				LTE_LOG(LTE_LOG_INFO, "set adv APN Index:[%d] ok.\n",data[index]);
			}else if(data[index+1] == 0x0){
				LTE_LOG(LTE_LOG_INFO, "set adv APN Index:[%d] fail.\n",data[index]);
			}else{
				LTE_LOG(LTE_LOG_INFO, "unknown.\n");				
			}
			break;
		}
		case SEARCH_PLMN_RSP:
		{
			if(data[index] == RSP_OK){
				char* databuf = data + index + 1 ;
				char countdata[16] = {0} ;
				char* countend = strchr(databuf,'\n');
				strncpy(countdata,databuf,countend -databuf);
				unsigned char count = atoi(countdata);
				
				LTE_LOG(LTE_LOG_INFO, "search plmn response OK,%d\n",count);
				int i = 0 ;
				int handleindex = 0 ;
				for(i = 0 ; i < count ;i++){
					handleindex = sepkeystr('\n',length -1 - handleindex ,countend+1 + handleindex,PLMNLISTS_key,4);
				}
			}else if(data[index] == RSP_ERROR){
				if(data[index+1] == SEARCH_PLMN_RSP_SEARCHING_PLMN){
					LTE_LOG(LTE_LOG_INFO, "The module is searching PLMN.\n");
				}else{
					LTE_LOG(LTE_LOG_INFO, "The searching PLMN response is fail.\n");
				}		
			}
			break;
		}
		case SET_PLMN_RSP:
		{
			if(data[index] == RSP_OK){
				LTE_LOG(LTE_LOG_INFO, "set plmn response OK.\n");
			}else if(data[index] == RSP_ERROR){
				LTE_LOG(LTE_LOG_INFO, "set plmn response Error.\n");	
			}
			break;
		}
	}
}

static void handle_usim_rsp(unsigned int MsgId,unsigned char* data,unsigned int length, void *pRetInfo){
	int index = 0 ;
	switch(MsgId){
		case PIN_MANAGEMENT_RSP:
		{
			LTE_LOG(LTE_LOG_INFO, "handle response information rep\n");

			if(data[index] == DISENABLE_PIN){
				if(data[index+1] == RSP_OK){
					LTE_LOG(LTE_LOG_INFO, "response PIN Management ok\n");
				}else if(data[index+1] == RSP_ERROR){
					LTE_LOG(LTE_LOG_INFO, "response PIN Management error\n");
				}
			}else if(data[index] == ENABLE_PIN){
				if(data[index+1] == RSP_OK){
					LTE_LOG(LTE_LOG_INFO, "response PIN Management ok\n");
				}else if(data[index+1] == RSP_ERROR){
					LTE_LOG(LTE_LOG_INFO, "response PIN Management error\n");
				}
			}else if(data[index] == UNLOCK_PIN){
				if(data[index+1] == RSP_OK){
					LTE_LOG(LTE_LOG_INFO, "response PIN Management ok\n");
				}else if(data[index+1] == RSP_ERROR){
					LTE_LOG(LTE_LOG_INFO, "response PIN Management error\n");
				}
			}else if(data[index] == MODIFY_PIN){
				if(data[index+1] == RSP_OK){
					LTE_LOG(LTE_LOG_INFO, "response PIN Management ok\n");
				}else if(data[index+1] == RSP_ERROR){
					LTE_LOG(LTE_LOG_INFO, "response PIN Management error\n");
				}
			}
			break;
		}
		case PUK_MANAGEMENT_RSP:
		{
			LTE_LOG(LTE_LOG_INFO, "handle response puk management rep\n");

			if(data[index] == RSP_OK){
				LTE_LOG(LTE_LOG_INFO, "handle response puk management ok\n");
			}else if(data[index] == RSP_ERROR){
				LTE_LOG(LTE_LOG_INFO, "handle response puk management error\n");
			}
			break;
		}
		case QUERY_USIM_RSP:
		{
            ALTAIR_PIN_SW_STATE_T *pstAltairPinSw = (ALTAIR_PIN_SW_STATE_T *)pRetInfo;
			LTE_LOG(LTE_LOG_INFO, "handle response query usim\n");
            if(NULL != pstAltairPinSw)
            {
                if(0 == data[index+1])
                {
                    pstAltairPinSw->enPinSwState = ALTAIR_PIN_CLOSE;
                }
                else if(1 == data[index+1])
                {
                    pstAltairPinSw->enPinSwState = ALTAIR_PIN_OPEN;
                }
            }
			if(data[index] == QUERY_USIM_RSP_NO_USIM){
				LTE_LOG(LTE_LOG_INFO, "handle response query usim :no usim\n");
			}else if(data[index] == QUERY_USIM_RSP_USIM_READY){
				LTE_LOG(LTE_LOG_INFO, "handle response query usim :usim ready\n");
				LTE_LOG(LTE_LOG_INFO, "IS PIN TEST ENABLE:[%d]\n",data[index+1]);
				LTE_LOG(LTE_LOG_INFO, "PIN1 Remaining attempt number:[%d]\n",data[index+2]);
				LTE_LOG(LTE_LOG_INFO, "PUK1 Remaining attempt number:[%d]\n",data[index+3]);
				LTE_LOG(LTE_LOG_INFO, "PIN2 Remaining attempt number:[%d]\n",data[index+4]);
				LTE_LOG(LTE_LOG_INFO, "PUK2 Remaining attempt number:[%d]\n",data[index+5]);
			}else if(data[index] == QUERY_USIM_RSP_PINLOCKED){
				LTE_LOG(LTE_LOG_INFO, "handle response query usim :pinlocked\n");
				LTE_LOG(LTE_LOG_INFO, "IS PIN TEST ENABLE:[%d]\n",data[index+1]);
				LTE_LOG(LTE_LOG_INFO, "PIN1 Remaining attempt number:[%d]\n",data[index+2]);
				LTE_LOG(LTE_LOG_INFO, "PUK1 Remaining attempt number:[%d]\n",data[index+3]);
				LTE_LOG(LTE_LOG_INFO, "PIN2 Remaining attempt number:[%d]\n",data[index+4]);
				LTE_LOG(LTE_LOG_INFO, "PUK2 Remaining attempt number:[%d]\n",data[index+5]);
			}else if(data[index] == QUERY_USIM_RSP_PUKLOCKED){
				LTE_LOG(LTE_LOG_INFO, "handle response query usim :puklocked\n");
				LTE_LOG(LTE_LOG_INFO, "IS PIN TEST ENABLE:[%d]\n",data[index+1]);
				LTE_LOG(LTE_LOG_INFO, "PIN1 Remaining attempt number:[%d]\n",data[index+2]);
				LTE_LOG(LTE_LOG_INFO, "PUK1 Remaining attempt number:[%d]\n",data[index+3]);
				LTE_LOG(LTE_LOG_INFO, "PIN2 Remaining attempt number:[%d]\n",data[index+4]);
				LTE_LOG(LTE_LOG_INFO, "PUK2 Remaining attempt number:[%d]\n",data[index+5]);
			}else if(data[index] == QUERY_USIM_RSP_SIMULATOR){
				LTE_LOG(LTE_LOG_INFO, "handle response query usim :SIMULATOR\n");
				LTE_LOG(LTE_LOG_INFO, "IS PIN TEST ENABLE:[%d]\n",data[index+1]);
				LTE_LOG(LTE_LOG_INFO, "PIN1 Remaining attempt number:[%d]\n",data[index+2]);
				LTE_LOG(LTE_LOG_INFO, "PUK1 Remaining attempt number:[%d]\n",data[index+3]);
				LTE_LOG(LTE_LOG_INFO, "PIN2 Remaining attempt number:[%d]\n",data[index+4]);
				LTE_LOG(LTE_LOG_INFO, "PUK2 Remaining attempt number:[%d]\n",data[index+5]);
			}
			break;
		}
	}
}

static char *LTE_DEV_INFORMATION_key[64] = {"Product Name","ModelID","Hardward Version","Software Version","Mac address","IMEI","IMSI","BANDS","MDN"};
static char *WAN_STATUS_key[64] = {"IP Address","Subnet Mask","Gateway","DNS1","DNS2"};
static char *LTE_DIAGNOSTIC_INFORMATION_key[64] = {"USIM STATUS","EARFCN","BAND","BANDWITH","CELL ID","PLMN","APN","DuplexingMode","RSRP","RSRQ","SINR","TAC","EPS Registration Status"};
static char *LTE_THROUGHPUT_key[64] = {"TX Packets","RX Packets","TX Bytes","RX Bytes"};

//return 0 is LowBit mode 1 is HighBit mode
static int checkHighLowBit(void){
	union w
	{
		int a ;
		char b ;
	}c ;
	c.a = 1 ;
	return (c.b == 1);
}

static void calctime(unsigned long seconds,char* buf){
	char tmpBuf[256]={'\0'};
	int connectedhours = 0 ;
	int connecteddays = 0 ;
	int connectedseconds = 0 ;			
	int connectedminutes =  seconds / 60 ;

	if(connectedminutes == 60){
		connectedminutes = 0 ;
		connectedseconds = seconds - (60 * 60) ;	 	
		connectedhours = 1;				
	}else if(connectedminutes < 60){
		connectedhours = 0;
		connectedseconds = 	seconds - (connectedminutes * 60) ;		
	}else if(connectedminutes > 60){	
		connectedhours = connectedminutes / 60 ;
		connectedminutes =  connectedminutes - (connectedhours * 60);
		connectedseconds =  seconds - (((connectedhours * 60) * 60) + (connectedminutes * 60));
	}
	
	if(connectedhours == 24){
		connecteddays =  1 ;
		connectedhours = 0 ;				
	}else if(connectedhours > 24){
		connecteddays =  connectedhours / 24 ;
		connectedhours = connectedhours - (connecteddays * 24) ;				
	}else{
		connecteddays =  0 ;				
	}

	memset(tmpBuf,0,256);
	if(connecteddays > 0){
		sprintf(tmpBuf,"%d Days %d Hours %d Minutes %d Seconds;",connecteddays,connectedhours,connectedminutes,connectedseconds);

	}else{
		sprintf(tmpBuf,"%d Hours %d Minutes %d Seconds;",connectedhours,connectedminutes,connectedseconds);
	}
	strcpy(buf,tmpBuf);
}

static void handle_get_rsp(unsigned int MsgId,unsigned char* data,unsigned int length, void *pRetInfo){

	int lte_dev_information_key_len = 9;
	int wan_status_key_len = 5;
	int lte_diagnostic_information_key_len = 13;
	int lte_throughput_key_len = 4;
	int index = 0 ;
	switch(MsgId){
		case GET_INFORMATION_RSP:
		{
			LTE_LOG(LTE_LOG_INFO, "handle response information rep\n");

			if(data[index] == GET_LTE_DEVICE_INFORMATION){
				LTE_LOG(LTE_LOG_INFO, "response lte device information\n");

				ALTAIR_DEV_INFO_T *pstDevInfo = (ALTAIR_DEV_INFO_T *)pRetInfo;
				int i, j, k;
				j= 1;
				for(i = 0; i < lte_dev_information_key_len; ){
					k = 0;
					switch(i){
						case 0:
							while(data[j] != '\n')
								pstDevInfo->productName[k++] = data[j++];
							pstDevInfo->productName[k] = '\0';
							break;
						case 1:
							while(data[j] != '\n')
								pstDevInfo->moduleId[k++] = data[j++];
							pstDevInfo->moduleId[k] = '\0';
							break;
						case 2:
							while(data[j] != '\n')
								pstDevInfo->hardVersion[k++] = data[j++];
							pstDevInfo->hardVersion[k] = '\0';
							break;
						case 3:
							while(data[j] != '\n')
								pstDevInfo->softVersion[k++] = data[j++];
							pstDevInfo->softVersion[k] = '\0';
							break;
						case 5:
							while(data[j] != '\n')
								pstDevInfo->imei[k++] = data[j++];
							pstDevInfo->imei[k] = '\0';
							break;
						case 6:
							while(data[j] != '\n')
								pstDevInfo->imsi[k++] = data[j++];
							pstDevInfo->imsi[k] = '\0';
							break;
						case 7:
							while(data[j] != '\n')
								pstDevInfo->bands[k++] = data[j++];
							pstDevInfo->bands[k] = '\0';
							break;
						default:
							break;
					}
					if(data[j] == '\n'){
						i++;
						j++;
					}else{
						while(data[j] != '\n')
							j += 1;
					}
				}

				sepkeystr('\n',length,data+1,LTE_DEV_INFORMATION_key,lte_dev_information_key_len);
			}else if(data[index] == GET_WAN_STATUS){
				LTE_LOG(LTE_LOG_INFO, "response wan status\n");

				sepkeystr('\n',length,data+1,WAN_STATUS_key,wan_status_key_len);
			}else if(data[index] == GET_RUNTIME){
				LTE_LOG(LTE_LOG_INFO, "response runtime\n");
			
				index = 1 ;
				char runtimebuf[128] = {0} ;
				unsigned long runtime = 0 ;
				char* tmp_dst_p = NULL ;
				tmp_dst_p    = (char*)(&runtime) ;
				if(checkHighLowBit()){
					tmp_dst_p[3] = data[index++];
					tmp_dst_p[2] = data[index++];
					tmp_dst_p[1] = data[index++];
					tmp_dst_p[0] = data[index++];				
				}else{
					tmp_dst_p[0] = data[index++];
					tmp_dst_p[1] = data[index++];
					tmp_dst_p[2] = data[index++];
					tmp_dst_p[3] = data[index++];				
				}
				calctime(runtime,runtimebuf);
				
				LTE_LOG(LTE_LOG_INFO, "runtime:[%s]\n",runtimebuf);
			}else if(data[index] == GET_LTE_DIAGNOSTIC_INFORMATION){
				LTE_LOG(LTE_LOG_INFO, "response lte diagnostic information\n");

				ALTAIR_LTE_INFO_T *pstLteInfo = (ALTAIR_LTE_INFO_T *)pRetInfo;
				int i, j, k;
				j= 1;
				for(i = 0; i < lte_diagnostic_information_key_len; ){
					k = 0;
					switch(i){
						case 1:
							while(data[j] != '\n')
								pstLteInfo->earfcn[k++] = data[j++];
							pstLteInfo->earfcn[k] = '\0';
							break;
						case 2:
							while(data[j] != '\n')
								pstLteInfo->band[k++] = data[j++];
							pstLteInfo->band[k] = '\0';
							break;
						case 3:
							while(data[j] != '\n')
								pstLteInfo->bandWith[k++] = data[j++];
							pstLteInfo->bandWith[k] = '\0';
							break;
						case 4:
							while(data[j] != '\n')
								pstLteInfo->cellId[k++] = data[j++];
							pstLteInfo->cellId[k] = '\0';
							break;
						case 8:
							while(data[j] != '\n')
								pstLteInfo->rsrp[k++] = data[j++];
							pstLteInfo->rsrp[k] = '\0';
							break;
						case 9:
							while(data[j] != '\n')
								pstLteInfo->rsrq[k++] = data[j++];
							pstLteInfo->rsrq[k] = '\0';
							break;
						case 10:
							while(data[j] != '\n')
								pstLteInfo->sinr[k++] = data[j++];
							pstLteInfo->sinr[k] = '\0';
							break;

						default:
							break;
					}
					if(data[j] == '\n'){
						i++;
						j++;
					}else{
						while(data[j] != '\n')
							j += 1;
					}
				}

				sepkeystr('\n',length,data+1,LTE_DIAGNOSTIC_INFORMATION_key,lte_diagnostic_information_key_len);
			}else if(data[index] == GET_LTE_THROUGHPUT){
				LTE_LOG(LTE_LOG_INFO, "response lte throughput\n");

				sepkeystr('\n',length,data+1,LTE_THROUGHPUT_key,lte_throughput_key_len);
			}else if(data[index] == GET_WORK_MODE){
				LTE_LOG(LTE_LOG_INFO, "response work mode\n");
	
				if(data[index+1] == WORK_MODE_ROUTER){
					LTE_LOG(LTE_LOG_INFO, "Router mode\n");
				}else if(data[index+1] == WORK_MODE_BRIDGE){
					LTE_LOG(LTE_LOG_INFO, "Bridge mode\n");
				}else{
					LTE_LOG(LTE_LOG_INFO, "dont know work mode\n");
				}
			}else if(data[index] == GET_CONNECTION_MODE){
				LTE_LOG(LTE_LOG_INFO, "response connection mode\n");

				if(data[index+1] == CONNECT_MODE_AUTO){
					LTE_LOG(LTE_LOG_INFO, "Auto mode\n");
				}else if(data[index+1] == CONNECT_MODE_MANUAL){
					LTE_LOG(LTE_LOG_INFO, "Manual mode\n");
				}else{
					LTE_LOG(LTE_LOG_INFO, "dont connection mode\n");
				}
			}
			break;
		}
		case GET_STATUS_RSP:
		{
			LTE_LOG(LTE_LOG_INFO, "handle response factory setting rep\n");

			if(data[index] == GET_STATUS_USIM_STATUS){
				ALTAIR_PIN_STATE_T *pstPinSate = (ALTAIR_PIN_STATE_T *)pRetInfo;
				if(NULL == pstPinSate)
					return;
				if(data[index+1] == USIM_STATUS_NO_USIM){
					LTE_LOG(LTE_LOG_INFO, "handle response usim statis is no usim\n");
					pstPinSate->enPinState = ALTAIR_NOSIM;
				}else if(data[index+1] == USIM_STATUS_USIM_READY){
					LTE_LOG(LTE_LOG_INFO, "handle response usim statis is usim ready\n");
					pstPinSate->enPinState = ALTAIR_SIMREADY;
				}else if(data[index+1] == USIM_STATUS_PINLOCKED){
					LTE_LOG(LTE_LOG_INFO, "handle response usim statis is pinlocked\n");
					pstPinSate->enPinState = ALTAIR_PINLOCKED;
				}else if(data[index+1] == USIM_STATUS_PUKLOCKED){
					LTE_LOG(LTE_LOG_INFO, "handle response usim statis is puklocked\n");
					pstPinSate->enPinState = ALTAIR_PUKLOCKED;
				}else if(data[index+1] == USIM_STATUS_SIMLOCKED){
					LTE_LOG(LTE_LOG_INFO, "handle response usim statis is simulocked\n");
					pstPinSate->enPinState = ALTAIR_SIMULOCKED;
				}else if(data[index+1] == USIM_STATUS_SIMULATOR){
					LTE_LOG(LTE_LOG_INFO, "handle response usim statis is simulator\n");
					pstPinSate->enPinState = ALTAIR_SIMULATOR;
				}
			}else if(data[index] == GET_STATUS_CONNECTION_STATUS){
				ALTAIR_LINK_STATE_T *pstLinkState = (ALTAIR_LINK_STATE_T *)pRetInfo;
				if(NULL == pstLinkState)
					return;
				if(data[index+1] == LTE_STATUS_CONNECTED){
					LTE_LOG(LTE_LOG_INFO, "handle response LTE CONNECTED\n");
					pstLinkState->enLinkState = ALTAIR_CONNECTED;
				}else if(data[index+1] == LTE_STATUS_CONNECTING){
					LTE_LOG(LTE_LOG_INFO, "handle response LTE CONNECTING\n");
					pstLinkState->enLinkState = ALTAIR_CONNECTING;
				}else if(data[index+1] == LTE_STATUS_DISCONNECTED){
					LTE_LOG(LTE_LOG_INFO, "handle response LTE DISCONNECTED\n");
					pstLinkState->enLinkState = ALTAIR_DISCONNECTED;
				}else if(data[index+1] == LTE_STATUS_AIRPLANEMODE){
					LTE_LOG(LTE_LOG_INFO, "handle response AIRPLANEMODE\n");
					pstLinkState->enLinkState = ALTAIR_AIRPLANEMODE;
				}
			}
			break;
		}
		case GET_APN_RSP:
		{
			LTE_LOG(LTE_LOG_INFO, "handle response apn setting rep\n");

			LTE_LOG(LTE_LOG_INFO, "apn index:[%d]\n",data[index]);
			if(data[index++] == 0){
					
			}else{
				LTE_LOG(LTE_LOG_INFO, "%s\n",data+1);
			}
			
			break;
		}
		case GET_AUTH_APN_RSP:
		{
			LTE_LOG(LTE_LOG_INFO, "apn index:[%d]\n",data[index]);
			if(data[index++] == 0){
				
			}else{
				LTE_LOG(LTE_LOG_INFO, "%s\n",data+1);
			}
			break;
		}
		case GET_CM_STATUS_RSP:
		{
			LTE_LOG(LTE_LOG_INFO, "handle response CM STATUS rep\n");

			if(data[index] == 1)
			{
				LTE_LOG(LTE_LOG_INFO, "CM_STATUS:[1]\n");
			}
			else if(data[index] == 2)
			{
				LTE_LOG(LTE_LOG_INFO, "CM_STATUS:[2]\n");
			}
			else if(data[index] == 3)
			{
				LTE_LOG(LTE_LOG_INFO, "CM_STATUS:[3]\n");
			}
			else if(data[index] == 0)
			{
				LTE_LOG(LTE_LOG_INFO, "CM_STATUS:[0]\n");
			}
			break;
		}
	}
}

static int handle_special_rsp(unsigned int MsgId,unsigned char* data,unsigned int length){

	int index = 0 ;
	int ret = -1;
	unsigned char* tmp_dst_p = NULL ; 
	switch(MsgId){
		case REQUEST_UPGRADE_RSP:
		{
			LTE_LOG(LTE_LOG_INFO, "handle response upgrade rep\n");

			if(data[index++] == RSP_OK){
				unsigned long tmpupgrading_once_size = 0 ;
				tmp_dst_p    = (unsigned char*)&tmpupgrading_once_size ;
				if(checkHighLowBit()){
					tmp_dst_p[3] = data[index++];
					tmp_dst_p[2] = data[index++];
					tmp_dst_p[1] = data[index++];
					tmp_dst_p[0] = data[index++];				
				}else{
					tmp_dst_p[0] = data[index++];
					tmp_dst_p[1] = data[index++];
					tmp_dst_p[2] = data[index++];
					tmp_dst_p[3] = data[index++];
				}
				LTE_LOG(LTE_LOG_INFO, "request upgrade ok upgrading_once_size:[%d]\n",tmpupgrading_once_size);	
				
				ret = 0 ;
				upgrading_once_size = tmpupgrading_once_size ;
			}else{
				unsigned char request_upgrading_errorno = data[index++] ;
				LTE_LOG(LTE_LOG_INFO, "request upgrade FAIL [%d]\n",request_upgrading_errorno);
				ret = -1 ;		
			}
			break;
		}
		case UPGRADE_RSP:
		{
			LTE_LOG(LTE_LOG_INFO, "handle response upgrade rep\n");

			unsigned long tmpupgrading_rsp_index = 0 ;
			if(data[index] == RSP_OK){
				index = 1 ;
				tmp_dst_p    = (unsigned char*)&tmpupgrading_rsp_index ;
				if(checkHighLowBit()){
					tmp_dst_p[3] = data[index++];
					tmp_dst_p[2] = data[index++];
					tmp_dst_p[1] = data[index++];
					tmp_dst_p[0] = data[index++];	
				}else{
					tmp_dst_p[0] = data[index++];
					tmp_dst_p[1] = data[index++];
					tmp_dst_p[2] = data[index++];
					tmp_dst_p[3] = data[index++];		
				}
				upgrading_rsp_index = tmpupgrading_rsp_index ;

				LTE_LOG(LTE_LOG_INFO, "upgrade ok upgrading_rsp_index:[%d]\n",upgrading_rsp_index);	

				ret = 0 ;
			}else if(data[index] == RSP_ERROR){
				index = 1 ;
				tmp_dst_p    = (unsigned char*)&tmpupgrading_rsp_index ;
				if(checkHighLowBit()){
					tmp_dst_p[3] = data[index++];
					tmp_dst_p[2] = data[index++];
					tmp_dst_p[1] = data[index++];
					tmp_dst_p[0] = data[index++];	
				}else{
					tmp_dst_p[0] = data[index++];
					tmp_dst_p[1] = data[index++];
					tmp_dst_p[2] = data[index++];
					tmp_dst_p[3] = data[index++];		
				}

				upgrading_rsp_index = tmpupgrading_rsp_index ;

				LTE_LOG(LTE_LOG_INFO, "upgrade FAIL upgrading_rsp_index:[%d]\n",upgrading_rsp_index);

				ret = -1 ;				
			}else if(data[index] == UPGRADED_SUCCESSFUL){				
				LTE_LOG(LTE_LOG_INFO, "upgrade SUCCESS\n");
				index = 1 ;
				upgraded_results = UPGRADED_SUCCESSFUL ;
				ret = 0 ;			
			}else if(data[index] == UPGRADED_FAIL){
				LTE_LOG(LTE_LOG_INFO, "upgrade FAIL\n");
				index = 1 ;
				upgraded_results = UPGRADED_FAIL ;
				ret = 0 ;			
			}
			break;
		}
		case REQUEST_QUIT_RSP:
		{
			LTE_LOG(LTE_LOG_INFO, "handle response quit socket rep\n");

			if(data[index] == RSP_OK){
				LTE_LOG(LTE_LOG_INFO, "quit socket ok\n");
				ret = 0 ;				
			}
			break;
		}
	}
	return ret ;	
}

static int handle_test_rsp(unsigned int MsgId,unsigned char* data,unsigned int length){

	int index = 0 ;
	int ret = -1;
	switch(MsgId){
		case REQUEST_STARTING_TEST_RF_RSP:
		{
			LTE_LOG(LTE_LOG_INFO, "handle response starting RF Test\n");

			if(data[index] == RSP_OK){
				LTE_LOG(LTE_LOG_INFO, "starting RF Test ok\n");	
				ret = 0 ;		
			}else if(data[index] == RSP_ERROR){
				LTE_LOG(LTE_LOG_INFO, "starting RF Test FAIL\n");
				ret = -1 ;				
			}
			break;
		}
		case REQUEST_ENDING_TEST_RF_RSP:
		{
			LTE_LOG(LTE_LOG_INFO, "handle response ending RF Test\n");

			if(data[index] == RSP_OK){
				LTE_LOG(LTE_LOG_INFO, "ending RF Test ok\n");	
				ret = 0 ;		
			}else if(data[index] == RSP_ERROR){
				LTE_LOG(LTE_LOG_INFO, "ending RF Test FAIL\n");
				ret = -1 ;				
			}
			break;
		}
		case REQUEST_TEST_RF_RSP:
		{
			LTE_LOG(LTE_LOG_INFO, "handle response RF Test\n");

			if(data[index] == RSP_OK){
				LTE_LOG(LTE_LOG_INFO, "RF Test ok\n");	
				ret = 0 ;		
			}else if(data[index] == RSP_ERROR){
				LTE_LOG(LTE_LOG_INFO, "RF Test FAIL\n");
				ret = -1 ;				
			}
			break;
		}
		case REQUEST_TEST_RF_READ_RX_RESULTS_RSP:
		{
			LTE_LOG(LTE_LOG_INFO, "handle response RF Test\n");

			if(data[index] == RSP_OK){
				LTE_LOG(LTE_LOG_INFO, "%s\n",data+1);	
				ret = 0 ;		
			}else if(data[index] == RSP_ERROR){
				LTE_LOG(LTE_LOG_INFO, "%s\n",data+1);
				ret = -1 ;				
			}
			break;		
		}
		case REQUEST_ABORT_TEST_RF_RSP:
		{
			LTE_LOG(LTE_LOG_INFO, "handle response Abort RF Test\n");

			if(data[index] == RSP_OK){
				LTE_LOG(LTE_LOG_INFO, "Abort RF Test ok\n");
				ret = 0 ;		
			}else if(data[index] == RSP_ERROR){
				LTE_LOG(LTE_LOG_INFO, "Abort RF Test FAIL\n");
				ret = -1 ;				
			}
			break;
		}
	}
	return ret ;	
}

static int handle_operator_rsp(unsigned int MsgId,unsigned char* data,unsigned int length){

	int index = 0 ;
	int ret = -1;
	switch(MsgId){
		case MANUAL_CONNECT_RSP:
		{
			LTE_LOG(LTE_LOG_INFO, "handle response manual rep\n");

			if(data[index] == RSP_OK){
				LTE_LOG(LTE_LOG_INFO, "manual operation lte ok\n");
				ret = 0 ;		
			}else if(data[index] == RSP_ERROR){
				LTE_LOG(LTE_LOG_INFO, "manual operation lte FAIL\n");	
				ret = -1 ;		
			}
			break;
		}
		case SET_FACTORY_RSP:
		{
			LTE_LOG(LTE_LOG_INFO, "handle response factory setting rep\n");

			if(data[index] == RSP_OK){
				LTE_LOG(LTE_LOG_INFO, "set factory setting ok\n");
				ret = 0 ;				
			}else if(data[index] == RSP_ERROR){
				LTE_LOG(LTE_LOG_INFO, "set factory setting FAIL\n");
				ret = -1 ;		
			}
			break;
		}
		case REBOOT_RSP:
		{
			LTE_LOG(LTE_LOG_INFO, "handle response reboot rep\n");

			if(data[index] == RSP_OK){
				LTE_LOG(LTE_LOG_INFO, "reboot ok\n");	
				ret = 0 ;		
			}else if(data[index] == RSP_ERROR){
				LTE_LOG(LTE_LOG_INFO, "reboot FAIL\n");
				ret = -1 ;				
			}
			break;
		}
		case SET_AIRPLANEMODE_RSP:
		{
			LTE_LOG(LTE_LOG_INFO, "handle response airplane mode rep\n");

			if(data[index] == RSP_OK){
				LTE_LOG(LTE_LOG_INFO, "set airplane mode ok\n");
				ret = 0 ;		
			}else if(data[index] == RSP_ERROR){
				LTE_LOG(LTE_LOG_INFO, "set airplane mode error\n");	
				ret = -1 ;		
			}
			break;
		}
		
	}
	return ret ;
}

static int Construct_Packet(unsigned char* pDst, unsigned int  nMaxLen, unsigned int  wMsgID)
{
	LTE_LOG(LTE_LOG_INFO, "MsgId:[%x]\n",wMsgID);

	int sendlength = 0 ;
	if((wMsgID & 0xf000) == SET_BIT_FLAG){
		sendlength = Construct_set_packect(pDst,nMaxLen,wMsgID);
	}else if((wMsgID & 0xf000) == USIM_BIT_FLAG){
		sendlength = Construct_usim_packect(pDst,nMaxLen,wMsgID);
	}else if((wMsgID & 0xf000) == GET_BIT_FLAG){
		sendlength = Construct_get_packect(pDst,nMaxLen,wMsgID);
	}else if((wMsgID & 0xf000) == OPERATION_BIT_FLAG){
		sendlength = Construct_operation_packect(pDst,nMaxLen,wMsgID);
	}else if((wMsgID & 0xf000) == SPECIAL_BIT_FLAG){
		sendlength = Construct_special_packect(pDst,nMaxLen,wMsgID);
	}else if((wMsgID & 0xf000) == TEST_BIT_FLAG){
		sendlength = Construct_test_packect(pDst,nMaxLen,wMsgID);
	}else{
		sendlength = 0;
	}
	return sendlength ;
}

static int AtelHandleMsg(unsigned int wMsgID,unsigned char* data,unsigned int length, void *pRetInfo){
	LTE_LOG(LTE_LOG_INFO, "MsgId:[%x]length:[%d]\n",wMsgID,length);

	int ret = 0 ;
	if(data == NULL || length <= 0){

	LTE_LOG(LTE_LOG_INFO, "data and length invaild.\n");

		return -1;
	}
	if((wMsgID & 0xf000) == SET_BIT_FLAG){
		handle_set_rsp(wMsgID,data,length);
	}else if((wMsgID & 0xf000) == USIM_BIT_FLAG){
		handle_usim_rsp(wMsgID,data,length,pRetInfo);
	}else if((wMsgID & 0xf000) == GET_BIT_FLAG){
		handle_get_rsp(wMsgID,data,length,pRetInfo);
	}else if((wMsgID & 0xf000) == OPERATION_BIT_FLAG){
		ret = handle_operator_rsp(wMsgID,data,length);
	}else if((wMsgID & 0xf000) == SPECIAL_BIT_FLAG){
		ret = handle_special_rsp(wMsgID,data,length);
	}else if((wMsgID & 0xf000) == TEST_BIT_FLAG){
		ret = handle_test_rsp(wMsgID,data,length);
	}
	return ret ;
}

static ALTAIR_BOOL AtelParseMsg(unsigned char* Data, unsigned int Len, void *pRetInfo)
{
	unsigned int  wMsgLen = 0, wDataLen = Len;
	char* bufTmp = NULL;
	char *pbTemp = (char *)Data;

	RxEtsHeaderT RxEtsHd;

	int data_len = 0;
	ALTAIR_BOOL bResult = TRUE;
	int ret  = -1 ;
	if(Data != NULL && Len >= 0){

		dump_buf(pbTemp,wDataLen);

		while((wMsgLen = AtelGetOneMsg(pbTemp, wDataLen)) > 0)
		{
			memset((void*)&RxEtsHd,0,sizeof(RxEtsHeaderT));
			RxEtsHd.Sync=*pbTemp++;

			LTE_LOG(LTE_LOG_INFO, "RxEtsHd.Sync:[%x] wMsgLen:[%d]\n",RxEtsHd.Sync,wMsgLen);

			memset(temp_data_buf, 0, MAX_MESSAGE_LEN);
			data_len=AtelSkipEcsByte(pbTemp,temp_data_buf,wMsgLen-1);

			bufTmp=temp_data_buf;
			RxEtsHd.Length=*bufTmp++;
			RxEtsHd.Length|=(*bufTmp++)<<8;
			RxEtsHd.MsgId=*bufTmp++;
			RxEtsHd.MsgId|=(*bufTmp++)<<8;

			LTE_LOG(LTE_LOG_INFO, "RxEtsHd.Length :[%d]\n",RxEtsHd.Length);
			LTE_LOG(LTE_LOG_INFO, "RxEtsHd.MsgId :[%x]\n",RxEtsHd.MsgId);

			if(data_len-3!=RxEtsHd.Length||RxEtsHd.Length>MAX_MESSAGE_LEN||RxEtsHd.Length<2){
				bResult = FALSE;
			}else{
				current_msgid = RxEtsHd.MsgId ;	
				ret = AtelHandleMsg(RxEtsHd.MsgId,bufTmp,RxEtsHd.Length-2, pRetInfo);
			}
			pbTemp += wMsgLen - 1;
			wDataLen -= wMsgLen;
		}
		if(ret != 0)
			bResult = FALSE;
	}else{
		bResult = FALSE;	
	}
	return bResult;
}

static void Atelzsys_handler_fn (int signal_value){
    LTE_LOG(LTE_LOG_INFO, "signal_value:[%d]\n",signal_value);

    if(signal_value == SIGINT){
	is_process_exit = 1 ;
    }
}

static void usage(){
	LTE_LOG(LTE_LOG_ERR, "argvs error ");
}

static unsigned int read_poweron_time(){
	int fd = -1;
	int size = 0 ;
	unsigned int pwon = -1;
	unsigned int idle = -1 ;
	double tmppwon,tmpidle;
	unsigned char buf[64];	
	memset(buf,0,64);
	fd = open("/proc/uptime",O_RDONLY);
	if(fd != -1){
		size = read(fd,buf,sizeof(buf));
		close(fd);
		if(size >0){
			sscanf(buf,"%lf %lf",&tmppwon,&tmpidle);
			pwon = (unsigned int)tmppwon;
			idle = (unsigned int)tmpidle;
			return pwon ;	
		}	
	}	 
	return 0 ;
}
int altair_control(int argc, char *argv[], void *pRetInfo){
    int funcRet = RSP_ERROR;
    char *interf = "*";
    char domain[64] = "localhost";
    int port = 5566;
    
    int option_index = 0 ;
    int i ;
    int respon_ret = 0 ;
/* Handle arguments */
    for (i = 0; i < argc; i++)
    {
	if (argv[i][0] == '-'){
	switch (*(argv[i]+1))
	{
		case 'q':
		{
			strcpy(system_setting_request_args,argv[++i]);
			break;
		}
		case 'Y':
		{	
			if(strcmp(argv[++i],"1") == 0){
				isairplanemode = 1 ;
			}else{
				isairplanemode = 0 ;
			}
			break;
		}
		case 'z':
		{	
			is_request_search_plmn = 1 ;
			break;
		}
		case 'Z':
		{	
			is_request_set_plmn = 1 ;
			if(strcmp(argv[++i],"0") == 0){
				is_request_set_plmn_type_auto = 1 ;
			}else{
				is_request_set_plmn_type_auto = 0 ;
				strcpy(request_set_plmn,argv[++i]);
			}
			break;
		}
		case 'J':
		{	
			is_request_abort_test_rf = 1 ;
			break;
		}
		case 'b':
		{
			is_request_test_rf_read_rx_results = 1 ;
			break;
		}
		case 't':
		{
			is_request_starting_test_rf = 1 ;
			break;
		}
		case 'N':
		{
			is_request_ending_test_rf = 1 ;
			break;
		}
		case 'm':
		{
			request_test_rf_type = atoi(argv[++i]);
			break;
		}
		case 'c':
		{
			request_test_rf_EARFCN = atoi(argv[++i]);
			break;
		}
		case 'd':
		{
			request_test_rf_time = atoi(argv[++i]);
			break;
		}
		case 'g':
		{
			request_test_rf_rxtx_param1 = atoi(argv[++i]);
			break;
		}
		case 'O':
		{
			request_test_rf_tx_power = atoi(argv[++i]);
			break;
		}
		case 'B':
		{
			request_test_rf_tx_param1 = atoi(argv[++i]);
			break;
		}
		case 'k':
		{
			request_abort_upgrading_reason = atoi(argv[++i]);
			break;
		}
		case 'f':
		{
			memset(new_fw_file,0,256);
			strcpy(new_fw_file,argv[++i]);
			is_upgrade_fw = 1 ;
			break;
		}
		case 'n':
		{
			memset(multiple_interface,0,64);
			strcpy(multiple_interface,argv[++i]);
			break;
		}
		case 'l':
		{
			is_support_multiple = 1 ;
			break;
		}
		case 'e':
		{
			multiple_port = atoi(argv[++i]);
			break;
		}
		case 'H':
		{
			memset(multiple_domain,0,64);
			strcpy(multiple_domain,argv[++i]);
			break;
		}
		case 'i':
		{
			memset(pin,0,USIM_BUF_LEN);
			strcpy(pin,argv[++i]);
			break;
		}	
		case 'E':
		{
			memset(newpin,0,USIM_BUF_LEN);
			strcpy(newpin,argv[++i]);
			break;
		}
		case 'K':
		{
			memset(puk,0,USIM_BUF_LEN);
			strcpy(puk,argv[++i]);
			break;
		}		
		case 'u':
		{
			pin_managemen_id = atoi(argv[++i]);
			break;
		}	
		case 'S':
		{
			get_status_id = atoi(argv[++i]);
			break;
		}	
		case 'G':
		{
			get_information_id = atoi(argv[++i]);
			break;
		}
		case 'R':
		{
			isreboot = 1;
			break;
		}		
		case 'F':
		{
			isfactorysetting = 1;
			break;
		}
		case 'L':
		{
			manualconnectlte = MANUAL_CONNECT_LTE;
			break;
		}
		case 'D':
		{
			manualconnectlte = MANUAL_DISCONNECT_LTE;
			break;
		}
		case 's':
		{
			memset(domain,0,sizeof(domain));
			strcpy(domain,argv[++i]);
			break;
		}
		case 'r':
		{
			memset(bindinterface,0,sizeof(bindinterface));
			strcpy(bindinterface,argv[++i]);
			break;
		}
		case 'p':
		{
			port = atoi(argv[++i]);
			break;
		}
		case 'W':
		{
			workmode = atoi(argv[++i]) ;
			break ;
		}
		case 'C':
		{
			connectmode = atoi(argv[++i]) ;
			break ;
		}
		case 'Q':
		{
			is_apnactiveauth = atoi(argv[++i]) ;
			break ;
		}	
		case 'a':
		{
			apnauth_protocolid = atoi(argv[++i]) ;
			break ;
		}
		case 'U':
		{
			memset(apnauth_username,0,MAX_MESSAGE_LEN);
			strcpy(apnauth_username,argv[++i]);
			break ;
		}
		case 'w':
		{
			memset(apnauth_password,0,MAX_MESSAGE_LEN);
			strcpy(apnauth_password,argv[++i]);
			break ;
		}
		case 'o':
		{
			memset(apnauth_host,0,MAX_MESSAGE_LEN);
			strcpy(apnauth_host,argv[++i]);
			break ;
		}
		case 'P':
		{
			apnprotocolid = atoi(argv[++i]) ;
			break ;
		}
		case 'V':
		{
			is_apnactive = atoi(argv[++i]) ;
			break ;
		}
		case 'j':
		{
			is_apndefaultgw = atoi(argv[++i]) ;
			break ;
		}
		case 'I':
		{
			apnindex = atoi(argv[++i]) ;
			break ;
		}
		case 'A':
		{
			memset(apn,0,MAX_MESSAGE_LEN);
			strcpy(apn,argv[++i]);
			break ;
		}
		case 'M':
		{
			args_MsgID = atoi(argv[++i]) ;
			break ;
		}
		case 'T':
		{
			timout = atoi(argv[++i]) ;
			break;
		}	
		case 'h':
		default:
		{
			usage();
			return funcRet;
		}
	}
	}else{
		usage();
		return funcRet;
	}
    }

    int loop = 1 ;
    int status   ;
    if(is_support_multiple){
	int sockfd; 
	char buffer[MAX_MESSAGE_LEN];
	struct sockaddr_in saddr;
	struct sockaddr_in raddr;
   	struct ip_mreq imreq;
	int socklen = sizeof(struct sockaddr_in);
	int recvn = 0 ;

	unsigned char ttl = 3;
	
	fd_set  rdfds;
	struct timeval tv;
	
	int poll_count = 1000;

	// open a UDP socket
	sockfd = socket(PF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0){
	     LTE_LOG(LTE_LOG_ERR, "Error creating socket");
	     return funcRet;
	}

	if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR, &loop,sizeof(loop)) < 0){
		LTE_LOG(LTE_LOG_ERR, "Error setsockopt SO_REUSEADDR");

		close(sockfd);
		return funcRet;		 
	}

	// set content of struct saddr and imreq to zero
	memset(&saddr, 0, sizeof(struct sockaddr_in));

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(multiple_port); // Use the first free port
	saddr.sin_addr.s_addr = htonl(INADDR_ANY); // bind socket to any interface
	status = bind(sockfd, (struct sockaddr *)&saddr,socklen);
	if(status < 0){
	     LTE_LOG(LTE_LOG_ERR, "Error binding socket to interface");

	     close(sockfd);
	     return funcRet;
	}
	// Set the outgoing interface to DEFAULT
	struct ifreq if_m_interface;
	strncpy(if_m_interface.ifr_name,multiple_interface,IFNAMSIZ);   
	status = setsockopt(sockfd, SOL_SOCKET,SO_BINDTODEVICE,(char*)&if_m_interface,sizeof(struct ifreq));
	if(status < 0){
		LTE_LOG(LTE_LOG_ERR, "Error setsockopt SO_BINDTODEVICE");

		close(sockfd);
		return funcRet;	 
	}

	// Set multicast packet TTL to 3; default TTL is 1
	status = setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl,sizeof(unsigned char));
	if(status < 0){
		LTE_LOG(LTE_LOG_ERR, "Error setsockopt IPPROTO_IP IP_MULTICAST_TTL");

		close(sockfd);
		return funcRet;	 
	}

	// send multicast traffic to myself too
	status = setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP,&loop,sizeof(loop));
	if(status < 0){
		LTE_LOG(LTE_LOG_ERR, "Error setsockopt IPPROTO_IP IP_MULTICAST_LOOP");

		close(sockfd);
		return funcRet;		 
	}

	memset(&imreq, 0, sizeof(struct ip_mreq));
	imreq.imr_multiaddr.s_addr = inet_addr(multiple_domain);
	imreq.imr_interface.s_addr = INADDR_ANY; // use DEFAULT interface
	if(imreq.imr_multiaddr.s_addr == -1){
		LTE_LOG(LTE_LOG_ERR, "get multiple_domain [%s] fail ", multiple_domain);

		close(sockfd);
		return funcRet;
	}
	// JOIN multicast group on default interface
	status = setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const void *)&imreq, sizeof(struct ip_mreq));
	if(status < 0){
		LTE_LOG(LTE_LOG_ERR, "Error setsockopt IPPROTO_IP IP_ADD_MEMBERSHIP");

		close(sockfd);
		return funcRet;	 
	}	
	memset(&raddr, 0, sizeof(struct sockaddr_in));


	FD_ZERO(&rdfds);
	FD_SET(sockfd, &rdfds);
	tv.tv_sec = 1;
	tv.tv_usec = 500;
	
	LTE_LOG(LTE_LOG_INFO, "start to listen multiple packet.\n");

	while(poll_count > 0){
		/*status = select(sockfd+1,&rdfds,NULL,NULL,&tv);
		if(status < 0){
			LTE_LOG(LTE_LOG_ERR, "select");
			break;
		}else if(status == 0){
			LTE_LOG(LTE_LOG_INFO, "select timeout\n");
			poll_count--;
			continue ;
		}else{
			if(FD_ISSET(sockfd, &rdfds)){*/
				recvn = 0 ;
				memset(buffer,0,MAX_MESSAGE_LEN);
				recvn = recvfrom(sockfd,buffer,MAX_MESSAGE_LEN,0,(struct sockaddr *)&raddr,(socklen_t*)&socklen);
				if(recvn > 0){
					if(AtelParseMsg(buffer,recvn,pRetInfo) != TRUE){
						LTE_LOG(LTE_LOG_INFO, "parse message failed.\n");
					}
				}
		/*	}
		}*/
	}

	status = setsockopt(sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, (const void *)&imreq, sizeof(struct ip_mreq));
	if(status < 0){
		LTE_LOG(LTE_LOG_ERR, "Error setsockopt IPPROTO_IP IP_DROP_MEMBERSHIP");
		close(sockfd);
		return funcRet;
	}

	// shutdown socket
	shutdown(sockfd, 2);
	// close socket
	close(sockfd);
     }else{
	if(is_upgrade_fw){
		LTE_LOG(LTE_LOG_INFO, "new_fw_file:[%s]\n",new_fw_file);

		if(access(new_fw_file,F_OK) != 0){
			LTE_LOG(LTE_LOG_ERR, "can not access upgrading file.");
			return funcRet;		
		}
	}
	LTE_LOG(LTE_LOG_INFO, "connect [%s:%d]\n",domain,port);

	int rc = 0 ;

		signal(SIGINT,Atelzsys_handler_fn);
		int sockfd;
		struct sockaddr_in serv_addr;

		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if(sockfd <0)
		{
			LTE_LOG(LTE_LOG_INFO, "socket creation failed\n");

			return funcRet;
		}
		if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR, &loop,sizeof(loop)) < 0){
			LTE_LOG(LTE_LOG_ERR, "Error setsockopt SO_REUSEADDR");
			
			close(sockfd);
			return funcRet;	 
		}

		if(strlen(bindinterface) > 0){
			int dontroute = 1 ;

			// Set the outgoing interface to DEFAULT
			struct ifreq if_bind_interface;
			strncpy(if_bind_interface.ifr_name,bindinterface,IFNAMSIZ);
			status = setsockopt(sockfd, SOL_SOCKET,SO_BINDTODEVICE,(char*)&if_bind_interface,sizeof(struct ifreq));
			if(status < 0){
				LTE_LOG(LTE_LOG_ERR, "Error setsockopt SO_BINDTODEVICE");

				close(sockfd);
				return funcRet;	 
			}
		}

		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(port);
		serv_addr.sin_addr.s_addr=inet_addr(domain);
	
		if ((rc = connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))) < 0){
		    LTE_LOG(LTE_LOG_ERR, "ERROR connecting");
			close(sockfd);
		    return funcRet;
		}

	      if(rc == 0) {
	  	LTE_LOG(LTE_LOG_INFO, "server connection is OK.\n");
     
		int sendlength =  Construct_Packet(send_data_buf,MAX_MESSAGE_LEN,args_MsgID);

		if(sendlength > 0){
	      	  dump_buf(send_data_buf,sendlength);
	      	  LTE_LOG(LTE_LOG_INFO, "sendlength:[%d]\n",sendlength);
		}

		if(sendlength > 0)
		{
			 fd_set  rdfds;
			 struct timeval tv;
			 int ret;
			 int poll_count = 10;
			 rc = send(sockfd,send_data_buf,sendlength,0);
			 LTE_LOG(LTE_LOG_INFO, "write data length:[%d]\n",rc);

	
			 if(rc > 0)
			 {
				unsigned int datalen ;

				while(poll_count > 0){
					LTE_LOG(LTE_LOG_INFO, "sock fd [%d]\n", sockfd);
					FD_ZERO(&rdfds);
					FD_SET(sockfd, &rdfds);
					tv.tv_sec = 1;
					tv.tv_usec = 0;
					ret = select(sockfd+1,&rdfds,NULL,NULL,&tv);
					if(ret < 0){
						LTE_LOG(LTE_LOG_ERR, "select error\n");
						break;
					}else if(ret == 0){
						LTE_LOG(LTE_LOG_NOTICE, "select timeout\n");
						poll_count--;
						sleep(1);
						continue ;
					}else{
						if(FD_ISSET(sockfd, &rdfds)){
							bzero(send_data_buf,MAX_MESSAGE_LEN);
							unsigned int datalen = recv(sockfd,send_data_buf,MAX_MESSAGE_LEN,0);
							LTE_LOG(LTE_LOG_INFO, "recv msg [length: %u]\n", datalen);
							if(datalen > 0){
								if(AtelParseMsg(send_data_buf,datalen,pRetInfo) != TRUE){
									respon_ret = -1 ;
									LTE_LOG(LTE_LOG_NOTICE, "parse message failed.\n");
								}else{
									funcRet = RSP_OK;
								}
								break;
							}else{
								poll_count-- ;
								sleep(1);
							}
						}
					}
				}
			}

int upgrading_fw_ret = 0 ;
unsigned int recvret   = 0 ;
unsigned long starttime = read_poweron_time() ;
unsigned char spendtimebuf[128] = {0} ;
if(respon_ret == 0 && is_upgrade_fw && upgrading_once_size > 0){

	LTE_LOG(LTE_LOG_INFO, "upgrading fw:[%s]\n",new_fw_file);

	calctime(starttime,spendtimebuf);
	LTE_LOG(LTE_LOG_INFO, "start time:[%s]\n",spendtimebuf);

	int rfd = -1 ;
	int rint = 0 ;

	unsigned char *read_buf = NULL ;
	unsigned char *data_buf = NULL ;
	struct pollfd fds[1] = {0};
	fd_set  rdsockfds;
	struct timeval rdsocktv;
	int poll_ret ;
	unsigned int offset = 0 ;
	unsigned long packet_index  = 0 ;
	unsigned int oncedata_size = 0 ;
	unsigned int packet_size = 0 ;
	unsigned int data_size = 0 ;
	unsigned int sendret   = 0 ;

	unsigned int send_try = 3 ;
	unsigned int recv_poll  = 10 ;

	unsigned int loop_count = (ugrade_fw_size / upgrading_once_size) ;
	if((loop_count * upgrading_once_size) < ugrade_fw_size)
	{
		loop_count++ ;		
	}
	unsigned int static_loop_count = loop_count ;
	unsigned int loop_percent = 100 ;
	unsigned int last_loop_percent = 100 ;
	oncedata_size = upgrading_once_size  + 4;
	data_size   = (2 * oncedata_size) + 32 ;

	read_buf = malloc(sizeof(unsigned char) * oncedata_size);
	if(read_buf == NULL)
	{
		upgrading_fw_ret = ABORT_UPGRADING_REASON_INTERNAL_ERROR ;
		goto UPGRADING_END ;		
	}
	data_buf = malloc(sizeof(unsigned char) * data_size);
	if(data_buf == NULL)
	{
		upgrading_fw_ret = ABORT_UPGRADING_REASON_INTERNAL_ERROR ;
		goto UPGRADING_END ;		
	}

	rfd = open(new_fw_file,O_RDONLY);
	if(rfd != -1)
	{

		while(loop_count > 0){
			LTE_LOG(LTE_LOG_INFO, "loop_count:[%d]\n",loop_count);

			unsigned int loop_percent = (unsigned int) ( ( ((float)loop_count) / ((float)static_loop_count) ) * 100);
			if(last_loop_percent != loop_percent){
				LTE_LOG(LTE_LOG_INFO, "process:%d\n",loop_percent);	
			}
			last_loop_percent = loop_percent ;
			if(is_process_exit){
				upgrading_fw_ret = ABORT_UPGRADING_REASON_INTERNAL_ERROR ;
				break ;				
			}	
			memset(read_buf,0,oncedata_size);
			rint = read(rfd,read_buf+4,oncedata_size-4);
			if(rint > 0){
				LTE_LOG(LTE_LOG_INFO, "rint:[%d]\n",rint);
				
				send_try   = 3 ;
				read_buf[0] = (packet_index >> 24) & 0xff;
				read_buf[1] = (packet_index >> 16) & 0xff;
				read_buf[2] = (packet_index >> 8) & 0xff;
				read_buf[3] = packet_index & 0xff;
				memset(data_buf,0,data_size);
				packet_size = Construct_Upgrading_Packet(data_buf,data_size,read_buf,rint+4);
				LTE_LOG(LTE_LOG_INFO, "packet_size:[%d] data_size:[%d]\n",packet_size,data_size);

SEND_AGAIN:
				recv_poll  = 10 ;
				
				dump_buf(data_buf,15);
				LTE_LOG(LTE_LOG_INFO, "%s(%d) ready to send data.\n",__FUNCTION__,__LINE__);

				sendret = send(sockfd,data_buf,packet_size,0);
				if(sendret  > 0){
					LTE_LOG(LTE_LOG_INFO, "%s(%d) sendret:[%d]\n",__FUNCTION__,__LINE__,sendret);

					while(recv_poll > 0){

			#if 1 //POLL
						fds[0].fd = sockfd ;
						fds[0].events = POLLIN;
						poll_ret = poll(fds,sizeof(fds)/sizeof(fds[0]),1000);
						if(poll_ret < 0){
							LTE_LOG(LTE_LOG_ERR, "poll");
							upgrading_fw_ret = ABORT_UPGRADING_REASON_INTERNAL_ERROR ;
							goto UPGRADING_END ;
						}else if(poll_ret == 0){
							LTE_LOG(LTE_LOG_INFO, "poll timeout:[%d]\n",recv_poll);

							recv_poll--;
							sleep(1);
							continue ;
						}else{
							if(fds[0].revents & POLLIN){
			#elif 0 //SELECT 
						FD_ZERO(&rdsockfds);
						FD_SET(sockfd, &rdsockfds);
						rdsocktv.tv_sec = 1;
						rdsocktv.tv_usec = 0;
						poll_ret = select(sockfd+1,&rdsockfds,NULL,NULL,&rdsocktv);
						if(poll_ret < 0){ 
						  	LTE_LOG(LTE_LOG_ERR, "select");
							upgrading_fw_ret = ABORT_UPGRADING_REASON_INTERNAL_ERROR ;
					  		goto UPGRADING_END ;
						}else if(poll_ret == 0){
							LTE_LOG(LTE_LOG_INFO, "select timeout:[%d]\n",recv_poll);

							recv_poll--;
							sleep(1);
							continue ;
						}else{
							if(FD_ISSET(sockfd, &rdfds)){
			#else
							{
			#endif
								bzero(send_data_buf,MAX_MESSAGE_LEN);
								recvret = recv(sockfd,send_data_buf,MAX_MESSAGE_LEN,0);
								if(recvret > 0){
									if(AtelParseMsg(send_data_buf,recvret,pRetInfo) != TRUE)
									{
										LTE_LOG(LTE_LOG_INFO, "%s(%d)\n",__FUNCTION__,__LINE__);

						if(send_try > 0){
							send_try-- ;				
							goto SEND_AGAIN ;
						}else{
							upgrading_fw_ret = ABORT_UPGRADING_REASON_INTERNAL_ERROR ;
							goto UPGRADING_END ;
						}
									}else{
							LTE_LOG(LTE_LOG_INFO, "%s(%d) current_msgid:[%x] upgrading_rsp_index:[%d] packet_index:[%d]\n",__FUNCTION__,__LINE__,current_msgid,upgrading_rsp_index,packet_index);

										if(current_msgid == UPGRADE_RSP){ 
// make sure recv upgrading rsp packet
											if(upgrading_rsp_index == packet_index){
//make sure send packet index equal rsp packet index
												packet_index++ ;
												offset += rint ;
												break ;	
											}else{
	                                                                                        continue ;
											}
										}else{
											recv_poll  = 10 ;
											continue ;				
										}

									}
								}else{
									LTE_LOG(LTE_LOG_INFO, "%s(%d) recvice error.\n",__FUNCTION__,__LINE__);

									if(send_try > 0){
										send_try-- ;				
										goto SEND_AGAIN ;
									}else{
										upgrading_fw_ret = ABORT_UPGRADING_REASON_INTERNAL_ERROR ;
										goto UPGRADING_END ;
									}
								}
							}
						}
					}
					if(recv_poll <= 0){		
						if(send_try > 0){
							send_try-- ;					
							goto SEND_AGAIN ;
						}else{
							LTE_LOG(LTE_LOG_INFO, "%s(%d)\n",__FUNCTION__,__LINE__);

							upgrading_fw_ret = ABORT_UPGRADING_REASON_INTERNAL_ERROR ;
							break ;	
						}		
					}
				}else{
					LTE_LOG(LTE_LOG_INFO, "%s(%d) send_try:[%d]\n",__FUNCTION__,__LINE__,send_try);

					if(send_try > 0){
						send_try-- ;					
						goto SEND_AGAIN ;
					}else{
						LTE_LOG(LTE_LOG_INFO, "%s(%d)\n",__FUNCTION__,__LINE__);

						upgrading_fw_ret = ABORT_UPGRADING_REASON_INTERNAL_ERROR ;
						break ;	
					}				
				}
			}else{
				// read fail,exit upgrading.
				LTE_LOG(LTE_LOG_INFO, "%s(%d)\n",__FUNCTION__,__LINE__);

				break ;
			}
			loop_count-- ;
		}	
	}else{
		upgrading_fw_ret = ABORT_UPGRADING_REASON_INTERNAL_ERROR ;
	}
UPGRADING_END:
	if(read_buf != NULL)
		free(read_buf);
	if(data_buf != NULL)
		free(data_buf);
	if(rfd != -1){
		close(rfd);		
	}	
}else{
	upgrading_fw_ret = ABORT_UPGRADING_REASON_INTERNAL_ERROR ;
}

if(is_upgrade_fw){
	if(upgrading_fw_ret != 0){
		LTE_LOG(LTE_LOG_INFO, "upgrading fw failed.\n");

		memset(send_data_buf,0,MAX_MESSAGE_LEN);
		request_abort_upgrading_reason = upgrading_fw_ret ;
		int sendlength =  Construct_Packet(send_data_buf,MAX_MESSAGE_LEN,REQUEST_UPGRADE_REP);
		send(sockfd,send_data_buf,sendlength,0);
	}else{
		LTE_LOG(LTE_LOG_INFO, "Waiting some minutes for upgrading process,module would reboot automaticly.\n");
		unsigned int wait_for_upgraded_poll  = 60 * 3 ;
		while(wait_for_upgraded_poll > 0){
			FD_ZERO(&rdfds);
			FD_SET(sockfd, &rdfds);
			tv.tv_sec = 1;
			tv.tv_usec = 0;
			ret = select(sockfd+1,&rdfds,NULL,NULL,&tv);
			if(ret < 0){
				LTE_LOG(LTE_LOG_ERR, "select");
				break;
			}else if(ret == 0){
				wait_for_upgraded_poll--;
				sleep(1);
				continue ;
			}else{
				if(FD_ISSET(sockfd, &rdfds)){
					bzero(send_data_buf,MAX_MESSAGE_LEN);
					recvret = recv(sockfd,send_data_buf,MAX_MESSAGE_LEN,0);
					if(recvret > 0){
						if(AtelParseMsg(send_data_buf,recvret,pRetInfo) != TRUE)
						{
							continue ;
						}else
						{
							if(current_msgid == UPGRADE_RSP){ // make sure recv upgrading rsp packet
								break ;
							}
						}
					}
				}
			}
		}
		if(upgraded_results == UPGRADED_SUCCESSFUL){
			LTE_LOG(LTE_LOG_INFO, "upgrading fw ok.\n");

			memset(spendtimebuf,0,sizeof(spendtimebuf));
			unsigned long spendtime = read_poweron_time() - starttime ;
			calctime(spendtime,spendtimebuf);
			LTE_LOG(LTE_LOG_INFO, "spend time:[%s]\n",spendtimebuf);

		}else if(upgraded_results == UPGRADED_FAIL){
			LTE_LOG(LTE_LOG_INFO, "upgrading fw fail.\n");	
		}else{
			LTE_LOG(LTE_LOG_INFO, "upgrading fw No Response.\n");	
		}
	}						
}
	}// Construct_Packet packect length bigger than 0 

	} // connection server return ok.
	else
	{
	    	LTE_LOG(LTE_LOG_INFO, "server invaild.\n");
	}
	close(sockfd);

    }

    return funcRet;
}
