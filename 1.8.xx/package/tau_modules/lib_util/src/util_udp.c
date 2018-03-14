/******************************************************************************

                  ��Ȩ���� (C), 2010-2012, �Ϻ�徴�ͨ�ſƼ����޹�˾

 ******************************************************************************
  �� �� ��   : util.c
  �� �� ��   : ����
  ��    ��   : zhanghaozhong
  ��������   : 2013��3��13��
  ����޸�   :
  ��������   : ���̼�UDPͨ��ģ��
  �����б�   :
              util_udp_init
              util_udp_recv
              util_udp_send
  �޸���ʷ   :
  1.��    ��   : 2013��3��13��
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

#include "util_include/util_udp.h"

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

typedef struct 
{
    unsigned char ucBuff[DATA_BUFF_LEN_MAX];
}UDP_MSG_T;



/*****************************************************************************
 �� �� ��  : util_udp_init
 ��������  : ���̼�UDPͨ�ų�ʼ��socket fd
 �������  : int iMyPort  --�Լ����̵�UDP�˿ں�
 �������  : ��
 �� �� ֵ  : socket fd
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��3��13��
    ��    ��   : zhanghaozhong
    �޸�����   : �����ɺ���

*****************************************************************************/
int util_udp_init(int iMyPort)
{
    
    //�����̵ĵ�ַ
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
 �� �� ��  : util_udp_send
 ��������  : ���̼�UDPͨ�ţ����ͺ���
 �������  : int sock      --�Ѵ����ı�����UDP socket fd
             int iToPort   --�Է����̵�UPD �˿ں�
             void *pvData  --���͵�����
             int iDataLen  --���͵����ݳ���
 �������  : ��
 �� �� ֵ  : ���͵����ݳ���
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��3��13��
    ��    ��   : zhanghaozhong
    �޸�����   : �����ɺ���

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
 �� �� ��  : util_udp_send_remote
 ��������  : ����UDP��Ϣ����������
 �������  : int sock      
             int iToPort   
             void *pvData  
             int iDataLen  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��7��5��
    ��    ��   : zhanghaozhong
    �޸�����   : �����ɺ���

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
 �� �� ��  : util_udp_recv
 ��������  : ���̼�UDPͨ�ţ����պ���
 �������  : int sock       --�Ѵ����ı�����UDP socket fd         
             void *pvData   --Ҫ���յ����ݽṹ         
             int iDataLen   --Ҫ���յ����ݽṹ����            
             unsigned int iWaitTime  --������ȴ�ʱ�䣬�����ȴ���֪����ʱ����Ϊ0����һֱΪ����״̬
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��3��13��
    ��    ��   : zhanghaozhong
    �޸�����   : �����ɺ���

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
        /* ��ʼ��slect�ṹ */
        FD_ZERO(&stReadFds); 
        FD_SET(sock, &stReadFds);
        iFdMax = sock +1;
        
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
    
    iRet = recvfrom(sock, &stPkt, sizeof(stPkt), 0, NULL, NULL);
    if(iRet < 0)
    {
        return iRet;
    }
    memcpy(pvData, stPkt.ucBuff, iDataLen);
   
    return iRet;
}

/*****************************************************************************
 �� �� ��  : util_udp_close
 ��������  : �ر�socket
 �������  : int sock  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��3��18��
    ��    ��   : zhanghaozhong
    �޸�����   : �����ɺ���

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
 �� �� ��  : util_udp_socket_recvfrom
 ��������  : �ú����������ڽ���socket��Ϣ
 �������  : int sock, void *pvData, int iDataLen 
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��4��27��
    ��    ��   : zhangjiasi
    �޸�����   : �����ɺ���

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