/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : util.c
  版 本 号   : 初稿
  作    者   : zhanghaozhong
  生成日期   : 2013年3月13日
  最近修改   :
  功能描述   : 进程间UDP通信模板
  函数列表   :
              util_udp_init
              util_udp_recv
              util_udp_send
  修改历史   :
  1.日    期   : 2013年3月13日
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

#include "util_include/util_udp.h"

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

typedef struct 
{
    unsigned char ucBuff[DATA_BUFF_LEN_MAX];
}UDP_MSG_T;



/*****************************************************************************
 函 数 名  : util_udp_init
 功能描述  : 进程间UDP通信初始化socket fd
 输入参数  : int iMyPort  --自己进程的UDP端口号
 输出参数  : 无
 返 回 值  : socket fd
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月13日
    作    者   : zhanghaozhong
    修改内容   : 新生成函数

*****************************************************************************/
int util_udp_init(int iMyPort)
{
    
    //本进程的地址
    struct sockaddr_in myAddr;
    int sock;
    int iFlag;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        return -1;
    }
    
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*) &iFlag, sizeof(iFlag)) < 0)
    {
        syslog(6, "util_udp_init() setsockopt ERROR\n");
        close(sock);
        return -1;
    }

    memset(&myAddr, 0, sizeof(myAddr));
    
    myAddr.sin_family = AF_INET;
    myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myAddr.sin_port = htons(iMyPort);
    
    if(bind(sock, (struct sockaddr*)&myAddr, sizeof(myAddr)) < 0)
    {
        close(sock);
        return -1;
    }
    return sock;
}

/*****************************************************************************
 函 数 名  : util_udp_send
 功能描述  : 进程间UDP通信，发送函数
 输入参数  : int sock      --已创建的本进程UDP socket fd
             int iToPort   --对方进程的UPD 端口号
             void *pvData  --发送的数据
             int iDataLen  --发送的数据长度
 输出参数  : 无
 返 回 值  : 发送的数据长度
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月13日
    作    者   : zhanghaozhong
    修改内容   : 新生成函数

*****************************************************************************/
int util_udp_send(int sock, int iToPort,void *pvData,int iDataLen)
{
    UDP_MSG_T stPkt;
   
    struct sockaddr_in toAddr;

    if(iDataLen <= 0 || iDataLen > DATA_BUFF_LEN_MAX)
    {
        return -1;
    }
    memset(&toAddr, 0, sizeof(toAddr));
    memset(&stPkt, 0, sizeof(stPkt));
    
    toAddr.sin_family = AF_INET;
    toAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    toAddr.sin_port = htons(iToPort);
    memcpy(stPkt.ucBuff, pvData, iDataLen);

    if(sendto(sock, &stPkt, iDataLen, 0 ,(struct sockaddr*)&toAddr, sizeof(toAddr)) != iDataLen)
    {
        close(sock);
        return -1;
    }
    return iDataLen;
}

/*****************************************************************************
 函 数 名  : util_udp_send_remote
 功能描述  : 发送UDP消息给其他主机
 输入参数  : int sock      
             int iToPort   
             void *pvData  
             int iDataLen  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年7月5日
    作    者   : zhanghaozhong
    修改内容   : 新生成函数

*****************************************************************************/
int util_udp_send_remote(int sock, int iToPort, void *pvData, int iDataLen, unsigned int iIp)
{
    UDP_MSG_T stPkt;
   
    struct sockaddr_in toAddr;

    if(iDataLen <= 0 || iDataLen > DATA_BUFF_LEN_MAX)
    {
        return -1;
    }
    memset(&toAddr, 0, sizeof(toAddr));
    memset(&stPkt, 0, sizeof(stPkt));
    
    toAddr.sin_family = AF_INET;
    toAddr.sin_addr.s_addr = iIp;
    toAddr.sin_port = htons(iToPort);
    memcpy(stPkt.ucBuff, pvData, iDataLen);

    if(sendto(sock, &stPkt, iDataLen, 0 ,(struct sockaddr*)&toAddr, sizeof(toAddr)) != iDataLen)
    {
        close(sock);
        return -1;
    }
    return iDataLen;
}
/*****************************************************************************
 函 数 名  : util_udp_recv
 功能描述  : 进程间UDP通信，接收函数
 输入参数  : int sock       --已创建的本进程UDP socket fd         
             void *pvData   --要接收的数据结构         
             int iDataLen   --要接收的数据结构长度            
             unsigned int iWaitTime  --接收最长等待时间，阻塞等待，知道超时，若为0，则一直为阻塞状态
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月13日
    作    者   : zhanghaozhong
    修改内容   : 新生成函数

*****************************************************************************/
int util_udp_recv(int sock, void *pvData, int iDataLen, unsigned int iWaitTime)
{      

    UDP_MSG_T stPkt;
    int iRet;
    fd_set stReadFds; /*For select function*/
    struct timeval stTimeOut;
    int iFdMax;
    
    if(iDataLen <= 0 || iDataLen > DATA_BUFF_LEN_MAX)
    {
        return -1;
    }

    memset(&stPkt,0,sizeof(UDP_MSG_T));
    if(iWaitTime > 0)
    {
        /* 初始化slect结构 */
        FD_ZERO(&stReadFds); 
        FD_SET(sock, &stReadFds);
        iFdMax = sock +1;
        
        /* 设置超时时间 */
        stTimeOut.tv_sec= iWaitTime;
    	stTimeOut.tv_usec = 0;
        
        /* 检测报文 */
        iRet = select(iFdMax,&stReadFds, NULL, NULL, &stTimeOut);
        if(-1 == iRet || 0 == iRet)
        {
            return -1;
        }
    }
    
    iRet = recvfrom(sock, &stPkt, sizeof(stPkt), 0, NULL, NULL);
    if(iRet < 0)
    {
        return iRet;
    }
    memcpy(pvData, stPkt.ucBuff, iDataLen);
   
    return iRet;
}

/*****************************************************************************
 函 数 名  : util_udp_close
 功能描述  : 关闭socket
 输入参数  : int sock  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月18日
    作    者   : zhanghaozhong
    修改内容   : 新生成函数

*****************************************************************************/
int util_udp_close(int sock)
{      
    int iRet;
    if (sock < 0)
    {
        return -1;
    }
    
    return close(sock);
}



/*****************************************************************************
 函 数 名  : util_udp_socket_recvfrom
 功能描述  : 该函数单纯用于接收socket消息
 输入参数  : int sock, void *pvData, int iDataLen 
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月27日
    作    者   : zhangjiasi
    修改内容   : 新生成函数

*****************************************************************************/
int  util_udp_socket_recvfrom( int sock, void *pvData, int iDataLen )
{
    UDP_MSG_T stPkt;
    int iRet;
    
    if(iDataLen <= 0 || iDataLen > DATA_BUFF_LEN_MAX)
    {
        return -1;
    }
    memset(&stPkt,0,sizeof(UDP_MSG_T));
    iRet = recvfrom(sock, &stPkt, sizeof(stPkt), 0, NULL, NULL);
    if(iRet < 0)
    {
        return iRet;
    }
    memcpy(pvData, stPkt.ucBuff, iDataLen);

    return iRet;
}