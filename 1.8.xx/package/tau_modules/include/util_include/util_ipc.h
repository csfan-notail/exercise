/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : util_ipc.h
  版 本 号   : 初稿
  作    者   : zhanghaozhong
  生成日期   : 2013年7月19日
  最近修改   :
  功能描述   : util_ipc.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2013年7月19日
    作    者   : zhanghaozhong
    修改内容   : 创建文件

******************************************************************************/
    
#ifndef __UTIL_IPC_H__
#define __UTIL_IPC_H__

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
#define UDP_MSG_HEAD_LEN      4*sizeof(int)

#ifndef DATA_BUFF_LEN_MAX
#define DATA_BUFF_LEN_MAX     32*1024 - UDP_MSG_HEAD_LEN
#endif
/* IPC发送结构体，供调用者使用 */
typedef struct 
{
    int dst_index;
	int src_index;
	int msg_type;
	int data_len;
	char *data;
}IPC_SEND_T;

/* IPC接收结构体，供调用者使用 */
typedef struct 
{
    int dst_index;
	int src_index;
	int msg_type;
	int data_len;
	unsigned char data[DATA_BUFF_LEN_MAX];
}IPC_REV_T;


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

extern int ipc_close(int sock);
extern int ipc_init(int iMyMsgId);
extern int ipc_recv(int sock, void *buff, unsigned int iWaitTime);
extern int ipc_send(int sock, void *buff);
extern int ipc_send_remote(int sock, int iToPort, void *buff, unsigned int iIp);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __UTIL_IPC_H__ */
