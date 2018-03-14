/*
 * Copyright (C) GBCOM ltd.,co 2015
 *
 * This is a part of project tr69cd, which implements TR069 protocal(aka
 * CWMP).
 * 
 * This is a PRIVATE software.
 * All rights reserved.
 */
 

#ifndef __DEVICEINFOPARAMS_H__
#define __DEVICEINFOPARAMS_H__

#include "tr69cdefs.h"

#ifdef CPEVARNAMEINSTANCE
#undef CPEVARNAMEINSTANCE
#endif
/* InternetGatewayDevice.UserInfo. */
SVAR(Password);
SVAR(Status);
SVAR(Limit);
SVAR(Times);
SVAR(Result);
SVAR(ServiceNum);
SVAR(ServiceName);


/* 函数原型定义 */
/* InternetGatewayDevice.DeviceInfo. */

TRXGFUNC(getUserInfoPassword);
TRXSFUNC(setUserInfoPassword);
TRXGFUNC(getUserInfoStatus);
TRXSFUNC(setUserInfoStatus);
TRXGFUNC(getUserInfoLimit);
TRXSFUNC(setUserInfoLimit);
TRXGFUNC(getUserInfoTimes);
TRXSFUNC(setUserInfoTimes);
TRXGFUNC(getUserInfoResult);
TRXSFUNC(setUserInfoResult);
TRXGFUNC(getUserInfoServiceNum);
TRXSFUNC(setUserInfoServiceNum);
TRXGFUNC(getUserInfoServiceName);
TRXSFUNC(setUserInfoServiceName);

#endif /* __DEVICEINFOPARAMS_H__ */

