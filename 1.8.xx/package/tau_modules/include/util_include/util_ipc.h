/******************************************************************************

                  ��Ȩ���� (C), 2010-2012, �Ϻ�徴�ͨ�ſƼ����޹�˾

 ******************************************************************************
  �� �� ��   : util_ipc.h
  �� �� ��   : ����
  ��    ��   : zhanghaozhong
  ��������   : 2013��7��19��
  ����޸�   :
  ��������   : util_ipc.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��7��19��
    ��    ��   : zhanghaozhong
    �޸�����   : �����ļ�

******************************************************************************/
    
#ifndef __UTIL_IPC_H__
#define __UTIL_IPC_H__

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/

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
#define UDP_MSG_HEAD_LEN      4*sizeof(int)

#ifndef DATA_BUFF_LEN_MAX
#define DATA_BUFF_LEN_MAX     32*1024 - UDP_MSG_HEAD_LEN
#endif
/* IPC���ͽṹ�壬��������ʹ�� */
typedef struct 
{
    int dst_index;
	int src_index;
	int msg_type;
	int data_len;
	char *data;
}IPC_SEND_T;

/* IPC���սṹ�壬��������ʹ�� */
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
