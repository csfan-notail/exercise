/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : test.c
  版 本 号   : 初稿
  作    者   : 张平
  生成日期   : 2012年8月23日 星期四
  最近修改   :
  功能描述   : 应用层netlink接收模块
  函数列表   :
  修改历史   :
  1.日    期   : 2012年8月23日 星期四
    作    者   : 张平
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <asm/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>

#include "ap_types.h"
#include "util_include/util_netlink.h"

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
static int s_iSockFd = 0;
/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
typedef struct 
{
    struct nlmsghdr hdr;
    UINT8 uiBuff[DATA_BUFF_LEN_MAX];
}NL_EVENT_PACKET_T;

/*****************************************************************************
 函 数 名  : netlink_sock_init
 功能描述  : netlink初始化
 输入参数  : int netlink_proto_type  
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月23日 星期四
    作    者   : 张平
    修改内容   : 新生成函数

*****************************************************************************/
INT32 netlink_sock_init(int iNetlinkProtoType)
{
    int iRet;
    struct sockaddr_nl stNlAddr;
    
    /* 创建 netlink socket*/
    s_iSockFd = socket(AF_NETLINK, SOCK_RAW, iNetlinkProtoType);
    if (s_iSockFd < 0)
    {
        return -1;
    }
    /* 绑定 socket */
    memset(&stNlAddr, 0, sizeof(struct sockaddr_nl));
    stNlAddr.nl_family = AF_NETLINK;
	stNlAddr.nl_pad = 0;
	stNlAddr.nl_pid = getpid();
	stNlAddr.nl_groups = NETLINK_EVENT_GROUPS;
	iRet = bind(s_iSockFd, (struct sockaddr*)&stNlAddr, sizeof(stNlAddr));
	if (iRet < 0)
	{
	    return iRet;
	}
	    
    return s_iSockFd;
}

/*****************************************************************************
 函 数 名  : util_netlink_send
 功能描述  : 用户态下netlink发送接口
 输入参数  : void *pvData   ---发送数据缓冲区   
             INT32 iDataLen ---发送长度 
 输出参数  : 无
 返 回 值  : 实际发送的数据长度，发送失败则返回-1
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月23日 星期四
    作    者   : 张平
    修改内容   : 新生成函数

*****************************************************************************/
INT32 util_netlink_send(void *pvData, INT32 iDataLen)
{
    NL_EVENT_PACKET_T stPkt;
    struct sockaddr_nl stNlPeer;
    int iRet;

    if(iDataLen <= 0 || iDataLen > DATA_BUFF_LEN_MAX)
    {
        return -1;
    }
    if(0 == s_iSockFd)
    {
        s_iSockFd = netlink_sock_init(NETLINK_EVENT_PROTOCOL);  
    }
    if (s_iSockFd < 0)
    {
        return -1;
    }
    /* 准备消息头 */
    memset(&stPkt, 0, sizeof(stPkt));
    stPkt.hdr.nlmsg_len = NLMSG_SPACE(iDataLen);
    stPkt.hdr.nlmsg_flags = 0;
    stPkt.hdr.nlmsg_type = 0x10;      /* 设置自定义消息类型 */          
    stPkt.hdr.nlmsg_pid = getpid();   /* 设置发送者的PID */ 

    /* 准备报文数据 */
    memcpy(stPkt.uiBuff,pvData,iDataLen);
    /* 初始化目标地址为组播地址 nl_pid = 0 nl_groups = 指定组 */
    memset(&stNlPeer, 0, sizeof(struct sockaddr_nl));
    stNlPeer.nl_family = AF_NETLINK;
    // stNlPeer.nl_pid = 0;
    stNlPeer.nl_groups = NETLINK_EVENT_GROUPS;

    iRet = sendto(s_iSockFd, &stPkt, stPkt.hdr.nlmsg_len, 0,
                (struct sockaddr*)&stNlPeer, sizeof(stNlPeer));
    return iDataLen;
}

/*****************************************************************************
 函 数 名  : util_netlink_recv
 功能描述  : 用户态下netlink接收接口
 输入参数  : void *pvData     ---接收数据缓冲区 
             INT32 iDataLen   ---接收长度，单位是字节，必须大于0且小于等于2048
             UINT32 iWaitTime ---接收等待时间，如果为0则一直等待，如果为非0整
                                 数值，则表示等待的时间，单位是秒
 输出参数  : 
 返 回 值  : 实际接收的数据长度，接收失败则返回-1
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月23日 星期四
    作    者   : 张平
    修改内容   : 新生成函数

*****************************************************************************/
INT32 util_netlink_recv(void *pvData, INT32 iDataLen, UINT32 iWaitTime)
{

    NL_EVENT_PACKET_T stPkt;
    struct sockaddr_nl stNlPeer;
    int iNlSockLen = sizeof(struct sockaddr_nl);
    int iRet;
    fd_set stReadFds; /*For select function*/
    struct timeval stTimeOut;
    INT32 iFdMax;
    
    if(iDataLen <= 0 || iDataLen > DATA_BUFF_LEN_MAX)
    {
        return -1;
    }
    if(0 == s_iSockFd)
    {
        s_iSockFd = netlink_sock_init(NETLINK_EVENT_PROTOCOL);  
    }
    if (s_iSockFd < 0)
    {
        return -1;
    }

    memset(&stNlPeer, 0, sizeof(struct sockaddr_nl));
    stNlPeer.nl_family = AF_NETLINK;
    
    if(iWaitTime > 0)
    {
        /* 初始化slect结构 */
        FD_ZERO(&stReadFds); 
        FD_SET(s_iSockFd, &stReadFds);
        iFdMax = s_iSockFd +1;
        
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
    
    iRet = recvfrom(s_iSockFd, &stPkt, sizeof(stPkt), 0,
                    (struct sockaddr*)&stNlPeer, &iNlSockLen);
    if(iRet < 0)
    {
        return iRet;
    }
    memcpy(pvData,stPkt.uiBuff,iDataLen);
   
    return iRet-sizeof(struct nlmsghdr);
}


/*****************************************************************************
 函 数 名  : pjsua_voip_netlink_recv
 功能描述  : 单纯的接收来自netlink的消息
 输入参数  : void *pvData, INT32 iDataLen
 输出参数  : 无
 返 回 值  : pj_status_t
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月26日
    作    者   : zhangjiasi
    修改内容   : 新生成函数

*****************************************************************************/
INT32  util_netlink_recvfrom( int sock, void *pvData, INT32 iDataLen , INT32 iWaitTime)
{
    NL_EVENT_PACKET_T stPkt;
    struct sockaddr_nl stNlPeer;
    int iNlSockLen = sizeof(struct sockaddr_nl);
    int iRet;
    fd_set stReadFds; /*For select function*/
    struct timeval stTimeOut;
    INT32 iFdMax;
    
    if(iDataLen <= 0 || iDataLen > DATA_BUFF_LEN_MAX)
    {
        return -1;
    }
    s_iSockFd = sock;
    
    if(0 == s_iSockFd)
    {
        s_iSockFd = netlink_sock_init(NETLINK_EVENT_PROTOCOL);  
    }
    
    if (s_iSockFd < 0)
    {
        return -1;
    }

    memset(&stNlPeer, 0, sizeof(struct sockaddr_nl));
    stNlPeer.nl_family = AF_NETLINK;
    
    if(iWaitTime > 0)
    {
        /* 初始化slect结构 */
        FD_ZERO(&stReadFds); 
        FD_SET(s_iSockFd, &stReadFds);
        iFdMax = s_iSockFd +1;
        
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
    
    iRet = recvfrom(s_iSockFd, &stPkt, sizeof(stPkt), 0,
                    (struct sockaddr*)&stNlPeer, &iNlSockLen);
    if(iRet < 0)
    {
        return iRet;
    }
    memcpy(pvData,stPkt.uiBuff,iDataLen);
   
    return iRet-sizeof(struct nlmsghdr);
}