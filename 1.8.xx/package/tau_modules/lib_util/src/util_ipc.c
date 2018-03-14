/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : util_ipc.c
  版 本 号   : 初稿
  作    者   : zhanghaozhong
  生成日期   : 2013年7月23日
  最近修改   :
  功能描述   : IPC通信
  函数列表   :
              ipc_close
              ipc_init
              ipc_recv
              ipc_send
              ipc_send_remote
  修改历史   :
  1.日    期   : 2013年7月23日
    作    者   : zhanghaozhong
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include "util_include/util_ipc.h"
#include "msg_id.h"

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

/* 发送数据块类型定义，IPC内部使用，不开放给调用者 */
typedef struct
{
    int dst_index;
	int src_index;
	int msg_type;
	int data_len;
	unsigned char data[0];
}IPC_SEND_BLOCK_T;



/*****************************************************************************
 函 数 名  : ipc_init
 功能描述  : 入参是消息ID，也就是进程的IP
 输入参数  : int iMyMsgId  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年7月19日
    作    者   : zhanghaozhong
    修改内容   : 新生成函数

*****************************************************************************/
int ipc_init(int iMyMsgId)
{
    //本进程的地址
    struct sockaddr_in myAddr;
    int sock;
    int iFlag = 0;
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        return -1;
    }
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*) &iFlag, sizeof(iFlag)) < 0)
    {
        close(sock);
        return -1;
    }

    memset(&myAddr, 0, sizeof(myAddr));
    myAddr.sin_family = AF_INET;
    myAddr.sin_addr.s_addr = htonl(iMyMsgId);
    myAddr.sin_port = htons(MSG_PORT);
    if(bind(sock, (struct sockaddr*)&myAddr, sizeof(myAddr)) < 0)
    {
        close(sock);
        return -1;
    }
    return sock;
}

/*****************************************************************************
 函 数 名  : ipc_send
 功能描述  : 进程间UDP通信，发送函数
 输入参数  : int sock    --已创建的本进程UDP socket fd
             void *buff  --IPC_SEND_T结构
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年7月19日
    作    者   : zhanghaozhong
    修改内容   : 新生成函数

*****************************************************************************/
int ipc_send(int sock, void *buff)
{
    IPC_SEND_T *stPkt = (IPC_SEND_T *)buff;
    IPC_SEND_BLOCK_T *stSend = (IPC_SEND_BLOCK_T *)malloc(stPkt->data_len + UDP_MSG_HEAD_LEN);
    struct sockaddr_in toAddr;

    if(stPkt->data_len <= 0 || stPkt->data_len > DATA_BUFF_LEN_MAX || NULL == stPkt->data)
    {
        free(stSend);
        return -1;
    }
    memset(&toAddr, 0, sizeof(toAddr));

    //stSend = malloc(stPkt->data_len + UDP_MSG_HEAD_LEN);
    memcpy(stSend, stPkt, UDP_MSG_HEAD_LEN);            /* 复制消息头 */
    memcpy(stSend->data, stPkt->data, stPkt->data_len); /* 复制数据部分 */
    toAddr.sin_family = AF_INET;
    toAddr.sin_addr.s_addr = htonl(stPkt->dst_index);
    toAddr.sin_port = htons(MSG_PORT);

    if(sendto(sock, stSend, stSend->data_len + UDP_MSG_HEAD_LEN, 0 ,(struct sockaddr*)&toAddr, sizeof(toAddr)) != stSend->data_len + UDP_MSG_HEAD_LEN)
    {
        free(stSend);
        close(sock);
        return -1;
    }
    free(stSend);
    return stPkt->data_len + UDP_MSG_HEAD_LEN;
}


/*****************************************************************************
 函 数 名  : ipc_send_remote
 功能描述  : 发送UDP消息给其他远程主机
 输入参数  : int sock          
             int iToPort       
             void *buff        --使用IPC_SEND_T结构
             unsigned int iIp  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年7月19日
    作    者   : zhanghaozhong
    修改内容   : 新生成函数

*****************************************************************************/
int ipc_send_remote(int sock, int iToPort, void *buff, unsigned int iIp)
{
    IPC_SEND_T *stPkt = (IPC_SEND_T *)buff;
    IPC_SEND_BLOCK_T *stSend = (IPC_SEND_BLOCK_T *)malloc(stPkt->data_len + UDP_MSG_HEAD_LEN);
   
    struct sockaddr_in toAddr;

    if(stPkt->data_len <= 0 || stPkt->data_len > DATA_BUFF_LEN_MAX || NULL == stPkt->data)
    {
        free(stSend);
        return -1;
    }
    memset(&toAddr, 0, sizeof(toAddr));
    memcpy(stSend, stPkt, UDP_MSG_HEAD_LEN);            /* 复制消息头 */
    memcpy(stSend->data, stPkt->data, stPkt->data_len); /* 复制数据部分 */
    
    toAddr.sin_family = AF_INET;
    toAddr.sin_addr.s_addr = htonl(iIp);
    toAddr.sin_port = htons(iToPort);

    if(sendto(sock, stSend, stSend->data_len + UDP_MSG_HEAD_LEN, 0 ,(struct sockaddr*)&toAddr, sizeof(toAddr)) != stPkt->data_len + UDP_MSG_HEAD_LEN)
    {
        free(stSend);
        close(sock);
        return -1;
    }
    free(stSend);
    return stPkt->data_len + UDP_MSG_HEAD_LEN;
}


/*****************************************************************************
 函 数 名  : ipc_recv
 功能描述  : 进程间UDP通信，接收函数
 输入参数  : int sock       --已创建的本进程UDP socket fd          
             void *buff     --此处的结构使用IPC_REV_T
             unsigned int iWaitTime  --接收最长等待时间，阻塞等待，知道超时，若为0，则一直为阻塞状态
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年7月19日
    作    者   : zhanghaozhong
    修改内容   : 新生成函数

*****************************************************************************/
int ipc_recv(int sock, void *buff, unsigned int iWaitTime)
{
    IPC_REV_T *stPkt = (IPC_REV_T *)buff;
    int iRet;
    fd_set stReadFds; /*For select function*/
    struct timeval stTimeOut;
    int iFdMax;

    if(iWaitTime > 0)
    {
        /* 初始化slect结构 */
        FD_ZERO(&stReadFds); 
        FD_SET(sock, &stReadFds);
        iFdMax = sock +1;
        
        /* 设置超时时间 */
        stTimeOut.tv_sec= 0;
        stTimeOut.tv_usec = iWaitTime;
        
        /* 检测报文 */
        iRet = select(iFdMax,&stReadFds, NULL, NULL, &stTimeOut);
        if(-1 == iRet || 0 == iRet)
        {   
            return -1;
        }
    }
    
    iRet = recvfrom(sock, stPkt, sizeof(IPC_REV_T), 0, NULL, NULL);
    if(iRet < 0)
    {   
        return iRet;
    }
   
    return iRet;
}


/*****************************************************************************
 函 数 名  : ipc_close
 功能描述  : 关闭socket
 输入参数  : int sock  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年7月19日
    作    者   : zhanghaozhong
    修改内容   : 新生成函数

*****************************************************************************/
int ipc_close(int sock)
{      
    int iRet;
    if (sock < 0)
    {
        return -1;
    }
    
    return close(sock);
}
