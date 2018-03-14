/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : restore.h
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/6/6
  Last Modified :
  Description   : restore.c header file
  Function List :
  History       :
  1.Date        : 2013/6/6
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#ifndef __RESTORE_FACTORY_H__
#define __RESTORE_FACTORY_H__

#if (defined(GW_X86) || defined(GW_ARM) || defined(GW_AC))
#define RESTORE_CONFIG_PATH "/appfs/etc/.restore_config"
#define DEFAULT_OEM_CONFIG  "/appfs/etc/.default_oem"
#else
#define RESTORE_CONFIG_PATH "/etc/.restore_config"
#define DEFAULT_OEM_CONFIG  "/etc/.default_oem"
#endif

#define RESTORE_CONFIG      1
#define NO_RESTORE_CONFIG   0

#define RESTORE_IP          "ip"
#define RESTORE_NETWORK     "network"
#define RESTORE_WIFI        "wifi"
#define RESTORE_WIFI_24     "wifi_24"
#define RESTORE_WIFI_5      "wifi_5"
#define RESTORE_VAP         "vap"
#define RESTORE_ACCOUNT     "account"
#define RESTORE_OTHER       "other"

#define OPT     "ainuwpocvhrm"

/* BEGIN: Added by wanggang, 2014/10/24   PN:gwifi */
#if (defined(LTE_SUPPORT))
#define OPT_LTE             "ainuwpocvhtrm"
#define RESTORE_LTE         "lte"

#endif
/* END:   Added by wanggang, 2014/10/24   PN:gwifi */


#define DETECT_RESTORE
#define RESTORE_MAIN_LOG	"restore_main_log"

extern int version();
extern int usage();
extern int restore_main(int argc, char **argv);
extern int restore_wifi(void);
extern int restore_vap(int nvram_id);
extern int restore_module(char *module, char *head, int index);
extern int clean_all();
extern int clean_vap();
void restore_route_delete(void);
void restore_portmap_delete(void);

void restore_dhcp_para_delete(void);


#ifdef DETECT_RESTORE
extern int restore_log_set(char *str, const char *format, ...);
#endif

#endif /* __RESTORE_FACTORY_H__ */
