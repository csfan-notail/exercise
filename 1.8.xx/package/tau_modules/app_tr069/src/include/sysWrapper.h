/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : sysWrapper.h
  版 本 号   : 初稿
  作    者   : zhangjiasi
  生成日期   : 2015年6月16日
  最近修改   :
  功能描述   : sysWrapper.c 的头文件,定义了部分系统相关函数的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2015年6月16日
    作    者   : zhangjiasi
    修改内容   : 创建文件

******************************************************************************/

#ifndef __SYSWRAPPER_H__
#define __SYSWRAPPER_H__

#include "tr69cdefs.h"
#include "../SOAPParser/RPCState.h"
/* general system status and configruation retrieval */
/* Returns state of WAN interface to be used by tr69 client */
typedef enum {
    eWAN_INACTIVE,
    eWAN_ACTIVE
} eWanState;

typedef struct gwStateData {
        char   rebootCommandKey[33];
        char   downloadCommandKey[33];
        char   newParameterKey[33];
        int    lastInstaceID;                          
        int    contactedState;
        int    dlFaultStatus;
        time_t startDLTime;
        time_t endDLTime;
        char   dlFaultMsg[129];
        char   padforexpansion[392];             
} GWStateData, *PGWStateData;



extern int getRAMSize(void);
extern void saveACSContactedState();
extern void  saveTR69StatusItems( ACSState *a );
extern eACSContactedState getACSContactedState(void);
extern void setACSContactedState(eACSContactedState state);
extern int getNewInstanceId(void);
extern TRX_STATUS setProvisioningCode(const char *value);
extern void wrapperReboot(eACSContactedState rebootContactValue);
extern void wrapperFactoryReset(void);
extern void wrapperSaveConfigurations(void);
int  downloadComplete(DownloadReq *r, char *buf);
TRX_STATUS getManufacturer(char **value);
TRX_STATUS getManufacturerOUI(char **value);
TRX_STATUS getProductClass (char **value);
TRX_STATUS getSerialNumber(char **value);
TRX_STATUS getCpeMac(char **value);	

void initDeviceInfoInstances(void);
void initDeviceConfigInstances(void);
void initX_ATP_FireWallInstances();
void initX_ATP_SystemMonitorInstances();
void initFreqLockInstances(void);
void initX_ATP_VxlanInstances(void);
void initX_ATP_L2GreInstances(void);
void initX_ATP_VPNInstances(void);
void initX_ATP_AlarmInfoInstances(void);
void initX_ATP_ConfigInstances(void);
void initServicesInstances(void);
void initGatewayUserInterfaceInstances(void);
void initX_ATP_PreserveDataInstances(void);
void initX_ATP_EXTInstances();
void initLayer3ForwardingInstances();
void initX_ATP_ModemLogInstances(void);
#endif /* __SYSWRAPPER_H__ */
