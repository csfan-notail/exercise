/******************************************************************************

                  ��Ȩ���� (C), 2010-2012, �Ϻ�徴�ͨ�ſƼ����޹�˾

 ******************************************************************************
  �� �� ��   : test.c
  �� �� ��   : ����
  ��    ��   : ��ƽ
  ��������   : 2012��8��23�� ������
  ����޸�   :
  ��������   : Ӧ�ò�netlink����ģ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��8��23�� ������
    ��    ��   : ��ƽ
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
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
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/
static int s_iSockFd = 0;
/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
typedef struct 
{
    struct nlmsghdr hdr;
    UINT8 uiBuff[DATA_BUFF_LEN_MAX];
}NL_EVENT_PACKET_T;

/*****************************************************************************
 �� �� ��  : netlink_sock_init
 ��������  : netlink��ʼ��
 �������  : int netlink_proto_type  
 �������  : ��
 �� �� ֵ  : static
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��23�� ������
    ��    ��   : ��ƽ
    �޸�����   : �����ɺ���

*****************************************************************************/
INT32 netlink_sock_init(int iNetlinkProtoType)
{
    int iRet;
    struct sockaddr_nl stNlAddr;
    
    /* ���� netlink socket*/
    s_iSockFd = socket(AF_NETLINK, SOCK_RAW, iNetlinkProtoType);
    if (s_iSockFd < 0)
    {
        return -1;
    }
    /* �� socket */
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
 �� �� ��  : util_netlink_send
 ��������  : �û�̬��netlink���ͽӿ�
 �������  : void *pvData   ---�������ݻ�����   
             INT32 iDataLen ---���ͳ��� 
 �������  : ��
 �� �� ֵ  : ʵ�ʷ��͵����ݳ��ȣ�����ʧ���򷵻�-1
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��23�� ������
    ��    ��   : ��ƽ
    �޸�����   : �����ɺ���

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
    /* ׼����Ϣͷ */
    memset(&stPkt, 0, sizeof(stPkt));
    stPkt.hdr.nlmsg_len = NLMSG_SPACE(iDataLen);
    stPkt.hdr.nlmsg_flags = 0;
    stPkt.hdr.nlmsg_type = 0x10;      /* �����Զ�����Ϣ���� */          
    stPkt.hdr.nlmsg_pid = getpid();   /* ���÷����ߵ�PID */ 

    /* ׼���������� */
    memcpy(stPkt.uiBuff,pvData,iDataLen);
    /* ��ʼ��Ŀ���ַΪ�鲥��ַ nl_pid = 0 nl_groups = ָ���� */
    memset(&stNlPeer, 0, sizeof(struct sockaddr_nl));
    stNlPeer.nl_family = AF_NETLINK;
    // stNlPeer.nl_pid = 0;
    stNlPeer.nl_groups = NETLINK_EVENT_GROUPS;

    iRet = sendto(s_iSockFd, &stPkt, stPkt.hdr.nlmsg_len, 0,
                (struct sockaddr*)&stNlPeer, sizeof(stNlPeer));
    return iDataLen;
}

/*****************************************************************************
 �� �� ��  : util_netlink_recv
 ��������  : �û�̬��netlink���սӿ�
 �������  : void *pvData     ---�������ݻ����� 
             INT32 iDataLen   ---���ճ��ȣ���λ���ֽڣ��������0��С�ڵ���2048
             UINT32 iWaitTime ---���յȴ�ʱ�䣬���Ϊ0��һֱ�ȴ������Ϊ��0��
                                 ��ֵ�����ʾ�ȴ���ʱ�䣬��λ����
 �������  : 
 �� �� ֵ  : ʵ�ʽ��յ����ݳ��ȣ�����ʧ���򷵻�-1
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��23�� ������
    ��    ��   : ��ƽ
    �޸�����   : �����ɺ���

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
        /* ��ʼ��slect�ṹ */
        FD_ZERO(&stReadFds); 
        FD_SET(s_iSockFd, &stReadFds);
        iFdMax = s_iSockFd +1;
        
        /* ���ó�ʱʱ�� */
        stTimeOut.tv_sec= iWaitTime;
    	stTimeOut.tv_usec = 0;
        
        /* ��ⱨ�� */
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
 �� �� ��  : pjsua_voip_netlink_recv
 ��������  : �����Ľ�������netlink����Ϣ
 �������  : void *pvData, INT32 iDataLen
 �������  : ��
 �� �� ֵ  : pj_status_t
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��4��26��
    ��    ��   : zhangjiasi
    �޸�����   : �����ɺ���

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
        /* ��ʼ��slect�ṹ */
        FD_ZERO(&stReadFds); 
        FD_SET(s_iSockFd, &stReadFds);
        iFdMax = s_iSockFd +1;
        
        /* ���ó�ʱʱ�� */
        stTimeOut.tv_sec= 0;
    	stTimeOut.tv_usec = iWaitTime;
        
        /* ��ⱨ�� */
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