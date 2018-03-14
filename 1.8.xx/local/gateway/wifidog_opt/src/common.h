/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : common.h
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/11/25
  Last Modified :
  Description   : common.h header file
  Function List :
  History       :
  1.Date        : 2013/11/25
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#ifndef __COMMON_H__
#define __COMMON_H__

/** @brief Read buffer for socket read? */
#define MAX_BUF 4096
#define DEVICE "OP"

/** @etherent name for AP93, TA98, TA40, TA40-21 */
#if defined(AP93_OAM)
#define ETH     "ixp"
#elif defined(AP9341_OAM)
#define WAN     "eth0"
#define LAN     "eth1"
#else
#define ETH     "eth"
#endif

/**
 *@客户端TCP最大并发连接数目
 */
#ifdef GW_X86
#define CLIENT_TCP_8060_NUM 511
#define CLIENT_TCP_8061_NUM 1023 /*select最大支持1024个fd*/
#define CLIENT_TCP_8062_NUM 512
#define CLIENT_TCP_80_NUM 512
#else

#ifdef MEM_128M
#define CLIENT_TCP_8060_NUM 256
#define CLIENT_TCP_8061_NUM 512
#define CLIENT_TCP_8062_NUM 256
#define CLIENT_TCP_80_NUM 256
#else
#define CLIENT_TCP_8060_NUM 96
#define CLIENT_TCP_8061_NUM 192
#define CLIENT_TCP_8062_NUM 96
#define CLIENT_TCP_80_NUM 96
#endif

#endif
#ifndef TRUE
#define TRUE 0
#endif

#ifndef FALSE
#define FALSE 1
#endif

/**
 *@表示无效socket
 */
#define SOCKET_INVALID -1

#endif /* __COMMON_H__ */

