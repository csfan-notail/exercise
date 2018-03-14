/******************************************************************************

                  ��Ȩ���� (C), 2010-2012, �Ϻ�徴�ͨ�ſƼ����޹�˾

 ******************************************************************************
  �� �� ��   : sysWrapper.h
  �� �� ��   : ����
  ��    ��   : zhangjiasi
  ��������   : 2015��6��16��
  ����޸�   :
  ��������   : sysWrapper.c ��ͷ�ļ�,�����˲���ϵͳ��غ�����ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��6��16��
    ��    ��   : zhangjiasi
    �޸�����   : �����ļ�

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
