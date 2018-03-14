/******************************************************************************

                  ��Ȩ���� (C), 2010-2012, �Ϻ�徴�ͨ�ſƼ����޹�˾

 ******************************************************************************
  �� �� ��   : util_ipc.c
  �� �� ��   : ����
  ��    ��   : zhanghaozhong
  ��������   : 2013��7��23��
  ����޸�   :
  ��������   : IPCͨ��
  �����б�   :
              ipc_close
              ipc_init
              ipc_recv
              ipc_send
              ipc_send_remote
  �޸���ʷ   :
  1.��    ��   : 2013��7��23��
    ��    ��   : zhanghaozhong
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
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

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/

/* �������ݿ����Ͷ��壬IPC�ڲ�ʹ�ã������Ÿ������� */
typedef struct
{
    int dst_index;
	int src_index;
	int msg_type;
	int data_len;
	unsigned char data[0];
}IPC_SEND_BLOCK_T;



/*****************************************************************************
 �� �� ��  : ipc_init
 ��������  : �������ϢID��Ҳ���ǽ��̵�IP
 �������  : int iMyMsgId  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��7��19��
    ��    ��   : zhanghaozhong
    �޸�����   : �����ɺ���

*****************************************************************************/
int ipc_init(int iMyMsgId)
{
    //�����̵ĵ�ַ
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
 �� �� ��  : ipc_send
 ��������  : ���̼�UDPͨ�ţ����ͺ���
 �������  : int sock    --�Ѵ����ı�����UDP socket fd
             void *buff  --IPC_SEND_T�ṹ
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��7��19��
    ��    ��   : zhanghaozhong
    �޸�����   : �����ɺ���

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
    memcpy(stSend, stPkt, UDP_MSG_HEAD_LEN);            /* ������Ϣͷ */
    memcpy(stSend->data, stPkt->data, stPkt->data_len); /* �������ݲ��� */
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
 �� �� ��  : ipc_send_remote
 ��������  : ����UDP��Ϣ������Զ������
 �������  : int sock          
             int iToPort       
             void *buff        --ʹ��IPC_SEND_T�ṹ
             unsigned int iIp  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��7��19��
    ��    ��   : zhanghaozhong
    �޸�����   : �����ɺ���

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
    memcpy(stSend, stPkt, UDP_MSG_HEAD_LEN);            /* ������Ϣͷ */
    memcpy(stSend->data, stPkt->data, stPkt->data_len); /* �������ݲ��� */
    
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
 �� �� ��  : ipc_recv
 ��������  : ���̼�UDPͨ�ţ����պ���
 �������  : int sock       --�Ѵ����ı�����UDP socket fd          
             void *buff     --�˴��Ľṹʹ��IPC_REV_T
             unsigned int iWaitTime  --������ȴ�ʱ�䣬�����ȴ���֪����ʱ����Ϊ0����һֱΪ����״̬
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��7��19��
    ��    ��   : zhanghaozhong
    �޸�����   : �����ɺ���

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
        /* ��ʼ��slect�ṹ */
        FD_ZERO(&stReadFds); 
        FD_SET(sock, &stReadFds);
        iFdMax = sock +1;
        
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
    
    iRet = recvfrom(sock, stPkt, sizeof(IPC_REV_T), 0, NULL, NULL);
    if(iRet < 0)
    {   
        return iRet;
    }
   
    return iRet;
}


/*****************************************************************************
 �� �� ��  : ipc_close
 ��������  : �ر�socket
 �������  : int sock  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��7��19��
    ��    ��   : zhanghaozhong
    �޸�����   : �����ɺ���

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
