/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : interface.h
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/5/27
  Last Modified :
  Description   : 接口头文件定义
  Function List :
  History       :
  1.Date        : 2013/5/27
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#ifndef __FLOW_CONTROL_H__
#define __FLOW_CONTROL_H__

extern int ssid_flow_config(char *name, int flag, int download, int upload);
extern int vap_flow_base_nvram_config();

#endif
