/*
 * Copyright (C) GBCOM ltd.,co 2015
 *
 * This is a part of project tr69cd, which implements TR069 protocal(aka
 * CWMP).
 * 
 * This is a PRIVATE software.
 * All rights reserved.
 */
 
#ifndef __ROOTPARAMS_H__
#define __ROOTPARAMS_H__


/*
* The CPE parameters are represented by a tree where each node in the
 * tree is an array of TRxObjNode structures. Each array item represents
 * either a paramater item or an object containing a pointer to the next
 * level of TRxObjNode items that make up  parameters that make up the object.
 * 
 * Each item that describes an object contains a pointer component, *objDetail,
 * that points to the array of parameter and object items that form the
 * object. 
 * The function pointer, getTRxParam, may be NULL or point to
 * a hardware dependent function that returns the value of the parameter in
 * a string variable. 
 * The function pointer, setTRxParam, is used to call a hardware dependent
 * function to set the parameter. If the function pointer is NULL the
 * parameter is not writeable. If the item is an object and the pointer is
 * NULL the rpc function AddObject/DeleteObject are not supported by that
 * item.
 *
 * The global variable thisTRxObjNode points to the current TRxObjNode entry
 * when the set/getTRxParam functions are called.
 * 
 * If the node contains a single TRxObjNode item with TRxType of tInstance
 * then this item represents all instances of the object. This function is
 * responsible for keeping track of all instances associated with this
 * object. The parameter handling framework expects the following functionality
 * of the getTRxParam function which will need to maintain state information
 * as the framework accesses the instances. The framework will always call
 * the getTRxParam to access an instance on its way to access its 
 * sub-objects/parameters. As the framework is traversing the parameter
 * tree it will call the getTRxParam function with one of the following
 * forms to validate and set the instance state before proceeding
 * to the next object/parameter level.
 * 
 * To determine the existance of a specific instance:
 *  The getTRxParam function is called as follows:
 *                     
 *  node->getTRXParam(char **ParamInstance)
 *  If *ParamInstance is not NULL then it points to the value to be found.
 *  Returns: TRx_OK if ParameterInstance found. The function should set
 *                  a global state variable for use by the next level
 *                  get/setTRxParam functions to the ParameterInstance.
 *           TRx_ERR if ParameterInstance not found
 * 
 *  To retrieve each of the instances in order:
 *  If *ParamInstance is NULL then it returns the first instance of the
 *  object.
 *  Returns: TRx_OK if any Instances exist. The *ParamInstance pointer points to the
 *                  name (instance number string) of the first instance.
 *                  The global instance state variable is set to the 
 *                  instance returned in the value string.
 *           TRx_ERR no instances of  this object exist.
 * 
 *  If *ParamInstance is (void *)(-1) then find the next instance relative
 * to the last instance returned.
 *  Returns: TRx_OK The *ParamInstance pointer points to the next instance. 
 *                  instance name. Repeated calls with the returned 
 *                  instance name from the previous call as the InstanceValue
 *                  will return all instances. The global instance state
 *                  variable is the instance returned in the value string.
 *          TRx_ERR no more instances.
 * See xxx for an example of how this is coded.
 */
#include "tr69cdefs.h"

#ifdef CPEVARNAMEINSTANCE
#undef CPEVARNAMEINSTANCE
#endif

 /* InternetGatewayDevice. */
SVAR(InternetGatewayDevice);

/* InternetGatewayDevice.LANDeviceNumberOfEntries */
SVAR(LANDeviceNumberOfEntries);
SVAR(WANDeviceNumberOfEntries);

/* InternetGatewayDevice.DeviceSummary */
SVAR(DeviceSummary);
/*add by duanguiyuan 20150915 
	InternetGatewayDevice.
*/
SVAR(X_CMCC_WANDeviceNumberOfEntries);
SVAR(X_CMCC_NetRegStatus);
SVAR(X_CMCC_GprsRegStatus);
SVAR(X_CMCC_EpsRegStatus);
SVAR(X_CMCC_CurrentNetwork);
SVAR(X_CMCC_CurrentNetmode);
SVAR(X_CMCC_NetworkPriority);
SVAR(X_CMCC_SingalLevel);
SVAR(X_CMCC_Txpower);
SVAR(X_CMCC_FrequencyPoint);
SVAR(X_CMCC_Band);
SVAR(X_CMCC_BandWidth);
SVAR(X_CMCC_EnodeBId);
SVAR(X_CMCC_CellId);
SVAR(X_CMCC_GlobeCellId);
SVAR(X_CMCC_PhysicsCellId);
SVAR(X_CMCC_ICCID);
SVAR(X_CMCC_APN);
SVAR(X_CMCC_IsFrequencyLock);
SVAR(X_CMCC_FrequencyLocking);
SVAR(X_CMCC_FrequencyLockInfo);
SVAR(X_CMCC_IsCellLock);
SVAR(X_CMCC_CellLockType);
SVAR(X_CMCC_CellLocking);
SVAR(X_CMCC_CellLockInfo);
SVAR(X_CMCC_LockPin);
SVAR(X_CMCC_LockPinType);
SVAR(X_CMCC_FirstPin);
SVAR(X_CMCC_FixedPin);
SVAR(X_CMCC_EncryptCard);
SVAR(X_CMCC_EncryptCardKey);
SVAR(X_CMCC_RSRP);
SVAR(X_CMCC_RSRQ);
SVAR(X_CMCC_SINR);
SVAR(X_CMCC_UpstreamTotalByte);
SVAR(X_CMCC_DownstreamTotalByte);
SVAR(X_CMCC_StartTime);
SVAR(X_CMCC_LoadAverage);
SVAR(X_CMCC_DeviceMemory);
SVAR(X_CMCC_MTU);
/*add by duanguiyuan   */
/* InternetGatewayDevice.DeviceInfo. */
SVAR(DeviceInfo);

/* InternetGatewayDevice.X_CMCC_UserInfo. */
SVAR(X_CMCC_UserInfo);
/* InternetGatewayDevice.ManagementServer. */
SVAR(ManagementServer);

/* InternetGatewayDevice.Time. */
SVAR(Time);

/* InternetGatewayDevice.LANConfigSecurity. */
SVAR(LANConfigSecurity);

/* InternetGatewayDevice.LANDevice. */
SVAR(LANDevice);

/* InternetGatewayDevice.WANDevice. */
SVAR(WANDevice);

/* InternetGatewayDevice.IPPingDiagnostics. */
SVAR(IPPingDiagnostics);
SVAR(X_PPPoE2L2TP);
SVAR(x_PPPoE2L2TP_i);
SVAR(x_PPPoE2L2TP_i_Desc);



/* InternetGatewayDevice.TraceRouteDiagnostics. */
SVAR(TraceRouteDiagnostics);

/* InternetGatewayDevice.X_CMCC_PPPOE_EMULATOR. */
SVAR(X_CMCC_PPPOE_EMULATOR);

/* InternetGatewayDevice.UserInterface */
//SVAR(UserInterface);

/* InternetGatewayDevice.Layer3Forwarding. */
SVAR(Layer3Forwarding);

/* InternetGatewayDevice.Layer2Bridging */
SVAR(Layer2Bridging);

/* InternetGatewayDevice.QueueManagement */
SVAR(QueueManagement);

/* InternetGatewayDevice.Services. */
SVAR(Services);
SVAR(VoiceService);

/*InternetGatewayDevice.X_DeviceInfo.*/
SVAR(X_DeviceInfo);

/* InternetGatewayDevice.X_LteDevice. */
SVAR(X_LteDevice);

/* LULIN TODO */
TRXGFUNC(getDeviceSummary);
TRXGFUNC(getWANLANDeviceNumEntries);
TRXGFUNC(getDeviceSummary);
TRXGFUNC(getWANLANDeviceNumEntries);
TRXGFUNC(getX_CMCC_WANDeviceNumberOfEntries);
TRXGFUNC(getX_CMCC_NetRegStatus);
TRXGFUNC(getX_CMCC_GprsRegStatus);
TRXGFUNC(getX_CMCC_EpsRegStatus);
TRXGFUNC(getX_CMCC_CurrentNetwork);
TRXGFUNC(getX_CMCC_CurrentNetmode);
TRXGFUNC(getX_CMCC_NetworkPriority);
TRXGFUNC(getX_CMCC_SingalLevel);
TRXGFUNC(getX_CMCC_Txpower);
TRXGFUNC(getX_CMCC_FrequencyPoint);
TRXGFUNC(getX_CMCC_Band);
TRXGFUNC(getX_CMCC_BandWidth);
TRXGFUNC(getX_CMCC_EnodeBId);
TRXGFUNC(getX_CMCC_CellId);
TRXGFUNC(getX_CMCC_GlobeCellId);
TRXGFUNC(getX_CMCC_PhysicsCellId);
TRXGFUNC(getX_CMCC_ICCID);
TRXGFUNC(getX_CMCC_APN);
TRXSFUNC(setX_CMCC_IsFrequencyLock);
TRXGFUNC(getX_CMCC_IsFrequencyLock);
TRXGFUNC(getX_CMCC_FrequencyLocking);
TRXSFUNC(setX_CMCC_FrequencyLocking);
TRXGFUNC(getX_CMCC_FrequencyLockInfo);
TRXGFUNC(getX_CMCC_FrequencyLocking);
TRXGFUNC(getX_CMCC_IsCellLock);
TRXGFUNC(getX_CMCC_CellLockType);
TRXGFUNC(getX_CMCC_CellLocking);
TRXGFUNC(getX_CMCC_CellLockInfo);
TRXGFUNC(getX_CMCC_LockPin);
TRXGFUNC(getX_CMCC_LockPinType);
TRXGFUNC(getX_CMCC_FirstPin);
TRXGFUNC(getX_CMCC_FixedPin);
TRXGFUNC(getX_CMCC_EncryptCard);
TRXGFUNC(getX_CMCC_EncryptCardKey);
TRXGFUNC(getX_CMCC_RSRP);
TRXGFUNC(getX_CMCC_RSRQ);
TRXGFUNC(getX_CMCC_SINR);
TRXGFUNC(getX_CMCC_UpstreamTotalByte);
TRXGFUNC(getX_CMCC_DownstreamTotalByte);
TRXGFUNC(getX_CMCC_StartTime);
TRXGFUNC(getX_CMCC_LoadAverage);
TRXGFUNC(getX_CMCC_DeviceMemory);
TRXGFUNC(getX_CMCC_MTU);

TRXSFUNC(setX_CMCC_IsCellLock);
TRXSFUNC(setX_CMCC_CellLockType);
TRXSFUNC(setX_CMCC_LockPin);
TRXSFUNC(setX_CMCC_LockPinType);
TRXSFUNC(setX_CMCC_EncryptCard);
TRXSFUNC(setX_CMCC_EncryptCardKey);
TRXSFUNC(setX_CMCC_MTU);
#endif /* __ROOTPARAMS_H__ */
