/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : my_snmp_ioctl.h
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2012/11/8
  Last Modified :
  Description   : my_snmp_ioctl.c header file
  Function List :
  History       :
  1.Date        : 2012/11/8
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/

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

#ifndef __MY_SNMP_IOCTL_H__
#define __MY_SNMP_IOCTL_H__

extern BOOL check_socket(void);
extern BOOL ioctl_common( char *devName, IOCTL_DATA_T *pstIoctlData );
extern BOOL set80211priv(char *devName, int op, void *data, int len );
extern BOOL get_mac_ioctl(char * devName, char * pcMac);
#endif /* __MY_SNMP_IOCTL_H__ */
