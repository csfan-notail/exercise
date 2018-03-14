/******************************************************************************

  Copyright (C), 2010-2015, GBCOM Co., Ltd.

 ******************************************************************************
  File Name     : client_aglink.h
  Version       : Initial Draft
  Author        : xiabing
  Created       : 2014/11/30
  Last Modified :
  Description   : c header file of client_aglink.c
  Function List :
  History       :
  1.Date        : 2014/11/30
    Author      : xiabing
    Modification: Created file

******************************************************************************/
#ifndef __CLIENT_AGLINK_H__
#define __CLIENT_AGLINK_H__

/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/
extern void thread_aglink_client(void);
extern int wifidog_get_gw_info();


/*----------------------------------------------*
 * module-wide global variables                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macros                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * routines' implementations                    *
 *----------------------------------------------*/


 
#pragma pack(1)
typedef struct _wifidog_audit_sta_info_
{
    char aucStaIp[4];
    char aucStaMac[6];
    char aucApMac[6];
    char aucAuthType[16];
    char aucStaId[32];
    int iStaRssi;
    int iLogOnTime;
    int iLogOffTime;
}WIFIDOG_AUDIT_STA_INFO_T;
#pragma pack()
 
#endif
