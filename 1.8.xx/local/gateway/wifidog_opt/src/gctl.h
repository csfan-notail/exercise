/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : gctl.h
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/12/11
  Last Modified :
  Description   : gctl.c header file
  Function List :
  History       :
  1.Date        : 2013/12/11
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#ifndef __GCTL_H__
#define __GCTL_H__

#define DEFAULT_SOCK    "/tmp/gwctl.sock"

typedef enum _en_gctl {
    GCTL_START = 0,
    GCTL_STOP,
    GCTL_RESTART,
    GCTL_STATUS,
    GCTL_GLOBAL,
    GCTL_SHOW_AP,
    GCTL_SHOW_CNNT_STA,
    GCTL_SETLOG_LEVEL,
    GCTL_EXTDGB_LEVEL,
    GCTL_AUTH_CTRL,
    GCTL_STA_AUTH,
    GCTL_STAT,
    GCTL_UP_WXL_WHITELIST,
    GCTL_SSID,
    GCTL_WDSTATUS,
    GCTL_MARO_ROAM_STA,
    GCTL_IOS_STA,
    GCTL_STA_ACL,
    GCTL_STA_HTTP_RECORD,
    GCTL_SPAN_SSID_AUTH,
    GCTL_LAN_INFO,
    GCTL_HELP,
    GCTL_MAX
}en_gctl;


typedef struct _gctl_config {
    en_gctl code;
    char *options;
    char *msg;
    int (*function)(int argc, char **argv);
}gctl_config;

#endif /* __GCTL_H__ */
