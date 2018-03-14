/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : redirect_portal.h
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/12/4
  Last Modified :
  Description   : redirect_portal.c header file
  Function List :
  History       :
  1.Date        : 2013/12/4
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#ifndef __REDIRECT_PORTAL_H__
#define __REDIRECT_PORTAL_H__

#define WXL_ADDRESS "10.1.0.6"
#define WXL_PORT 80
#define WXL_REDIRECT_URL "www.wxlwifi.bl.com" /*任意一个无效的地址*/

extern char acLanMac[32];

#ifdef GW_X86
#include "list.h"
#define MAXSOCKFD 1024
#define HTTP_CONNECT_MAX_NUM 1000

typedef struct {
	request stReq;
    time_t stConnetTime;
    struct hlist_node stNode;
}ClientSockList;

#endif

extern void *thread_redirect(void *args);
/*add@2014.8.15*/
extern void thread_first_page_handle(void);

#endif /* __REDIRECT_PORTAL_H__ */
