/*
 * Copyright (C) GBCOM ltd.,co 2015
 *
 * This is a part of project tr69cd, which implements TR069 protocal(aka
 * CWMP).
 * 
 * This is a PRIVATE software.
 * All rights reserved.
 */
 
#ifndef INFORMER_H
#define INFORMER_H

#include "../SOAPParser/RPCState.h"
#include "../webproto/wget.h"

typedef enum {
   eJustBooted=0,
   eACSInformed
} eInformState;
/*add by duanguiyuan 20160328 */
typedef struct LTEInfoState {
   /* lte info state */
	int singleLevel;
   	int FrequencyPoint;
	int band;
	int PhysicsCellId;
	char X_CMCC_ICCID[32];
//	char *CellLockInfo;
	int CMCC_RSRP;
	int CMCC_RSRQ;
	int	CMCC_SINR;	
	char   aucModuleType[32];
    char   aucModuleVer[64];
    char   aucModuleIMEI[32];
	char   aucDeviceMAC[32];

	char   aucEnodebId[16];
	char   aucCellID[32];

	char   aucTac[16]; 
	char   aucLcid[32]; 
}LTEInfoState;

typedef struct Diag_CMD_INFO {
   /* lte info state */
	int 	daig_type;
	char   cmd_str[512];
	time_t   daig_time;
}Diag_CMD_INFO;
typedef enum {
   Diag_IPPing=1,
   Diag_Traceroute,
   Diag_PPPoE
}Diag_TYPE;
extern eInformState  informState;

char *readResponse(tWget *wg, int *mlth, int maxBufferSize);
void resetPeriodicInform(unsigned long interval);
void initInformer(void);
void sendDownloadFault(void);
void sendNullHttp(void);
void sendToAcs(int contentLen, char *buf);
void initCFMListener(void);
int  isAcsConnected(void);
tProtoCtx *getAcsConnDesc(void);
void cfmKickListener(void *handle);
void getLetstateinfo();
#endif /*INFORMER_H*/
 
