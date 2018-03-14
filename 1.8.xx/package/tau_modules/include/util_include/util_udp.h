/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : util.h
  版 本 号   : 初稿
  作    者   : zhanghaozhong
  生成日期   : 2013年3月13日
  最近修改   :
  功能描述   : UDP进程间通信头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2013年3月13日
    作    者   : zhanghaozhong
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

#ifndef __UTIL_H__
#define __UTIL_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#define DATA_BUFF_LEN_MAX   2048

/***********define UDP PORT for IAD************************/
#define CGI_PORT            7356
#define TR69C_PORT          7357
#define EVENT_MONITOR_PORT  7358
#define OMA_CLI_PORT        7359
#define VOIP_MSG_LISTEN_PORT   7360
#define VOIP_MSG_SEND_PORT   7361
#define TR69C_ALARM_PORT     7362
#define TR69C_VOIP_PORT      7363

#define PTELNET_SERVER_LISTEN_PORT   20003
#define PTELNET_CLIENT_LISTEN_PORT   20004
#define PTELNET_TR69C_PORT           20005



int util_udp_init(int iMyPort);
int util_udp_send(int sock, int iToPort,void *pvData,int iDataLen);
int util_udp_recv(int sock, void *pvData, int iDataLen, unsigned int iWaitTime);
int util_udp_close(int sock);
int util_udp_socket_recvfrom( int sock, void *pvData, int iDataLen );



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __UTIL_H__ */


