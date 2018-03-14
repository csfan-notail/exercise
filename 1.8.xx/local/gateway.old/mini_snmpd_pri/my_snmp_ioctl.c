/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : my_snmp_ioctl.c
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2012/11/7
  Last Modified :
  Description   : ioctl调用函数
  Function List :
  History       :
  1.Date        : 2012/11/7
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/wireless.h>

#include <gbcom_wifi_ioctl.h>
#include "my_snmp_def.h"
#include "my_snmp_data.h"
#include "mini_snmpd.h"
#include "my_snmp_handle.h"
#include "my_snmp_util.h"
#include "my_snmp_ioctl.h"
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
static int skfd = -1;

/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macros                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * routines' implementations                    *
 *----------------------------------------------*/

/*****************************************************************************
 Prototype    : check_socket
 Description  : 检查ioctl的socket
 Input        : None
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2012/11/7
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
BOOL check_socket(void)
{
    static int siFlag = 0;

    if (0 == siFlag)
    {
        skfd = socket(AF_INET, SOCK_DGRAM, 0);
        agent_printf(LOG_ERROR, "ioclt socket creat ok");
        siFlag = 1;
        return 0;
    }
    
    if (-1 == skfd && 0 != siFlag)
    {
        skfd = socket(AF_INET, SOCK_DGRAM, 0);
        agent_printf(LOG_ERROR, "ioclt socket creat fail");
        return -1;
    }

    return 0;
}


/*****************************************************************************
 Prototype    : set80211priv
 Description  : 调用ioctl接口
 Input        : int socket
                char *devName
                int op
                void *data
                int len
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2012/11/7
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
BOOL set80211priv(char *devName, int op, void *data, int len )
{
    struct iwreq stIwreq = {{{0}}};

    check_socket();
    strncpy(stIwreq.ifr_name, devName, IFNAMSIZ);
    if (len < IFNAMSIZ)
    {
        memcpy(stIwreq.u.name, data, len);
    }
    else
    {
        stIwreq.u.data.pointer = data;
        stIwreq.u.data.length = len;
    }
    if (0 != ioctl(skfd, op, &stIwreq))
    {
        agent_printf(LOG_ERROR, "ioctl(op: %x, devName: %s, socket:%d) fail", op, devName, skfd);
        return FALSE;
    }
    return TRUE;

}

/*****************************************************************************
 Prototype    : get_mac_ioctl
 Description  : 获取vap的MAC地址
 Input        : char *devName
                char *pcMac
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2012/11/10
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
BOOL get_mac_ioctl( char *devName, char *pcMac )
{
    struct iwreq iwr;

    memset(&iwr, 0x00, sizeof(struct iwreq));
	strncpy(iwr.ifr_ifrn.ifrn_name, devName, IFNAMSIZ); 
	iwr.u.data.length = 6;
	iwr.u.data.pointer = (void __user*)pcMac;

	if (0 != ioctl(skfd, SIOCDEVPRIVATE+13, &iwr))
    {
        agent_printf(LOG_ERROR, "ioctl(op: %x, devName: %s, socket:%d) fail", SIOCDEVPRIVATE+13, devName, skfd);
        return FALSE;
    }
    return TRUE;
}

/*****************************************************************************
 Prototype    : ioctl_common
 Description  : 获取无线信息ioctl接口
 Input        : char *devName
                IOCTL_DATA_T *pstIoctlData
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2012/11/7
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
BOOL ioctl_common( char *devName, IOCTL_DATA_T *pstIoctlData )
{
    char ifname[IFNAMSIZ] = {0};
    int iRet              = 0;
    
    strncpy(ifname, devName, IFNAMSIZ);

    iRet = set80211priv(devName, IEEE80211_IOCTL_COMMON, pstIoctlData, sizeof(IOCTL_DATA_T));
    if (FALSE == iRet)
    {
        agent_printf(LOG_ERROR, "ioctl_comm fail(devName:%s)", devName);
        return FALSE;
    }
    return TRUE;
}

/*****************************************************************************
 Prototype    : iw_get_ext
 Description  : 获取功率， essid等
 Input        : None
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2012/11/8
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
BOOL iw_get_exts(char *ifname,		/* Device name */
	            int request,	    /* WE ID */
	            struct iwreq *pwrq)
{
    /* Set device name */
    strncpy(pwrq->ifr_name, ifname, IFNAMSIZ);
    /* Do the request */
    return(ioctl(skfd, request, pwrq));    
}/* end iw_get_ext */

