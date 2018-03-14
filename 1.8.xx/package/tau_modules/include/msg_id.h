/******************************************************************************

  Copyright (C), 2010-2011, GBCOM Co., Ltd.

 ******************************************************************************
  File Name     : msg_id.h
  Version       : Initial Draft
  Author        : wanggang
  Created       : 2014/6/4
  Last Modified :
  Description   : msg_id.h header file
  Function List :
  History       :
  1.Date        : 2014/6/4
    Author      : wanggang
    Modification: Created file

******************************************************************************/

/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/

/*----------------------------------------------*
 * module-wide global variables                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macros                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * routines' implementations                    *
 *----------------------------------------------*/

#ifndef __MSG_ID_H__
#define __MSG_ID_H__


/*
���̱��Ϊ32λ�޷���������ֵ��

��APƽ̨�У������Խ��̻��̴߳��ڵ�����ʵ�壬�໥֮�䶼����ͨ��ͨ�ÿ��IPC�����첽ͨ�š����̱����IPC
ͨ�ŵ�һ���ؼ��֣���ÿ�����̻��߳̽���ͨ�ŵĴ��ڡ�
��APƽ̨�е�IPCͨ���У�����UDPͨ�Ż��ƣ�ʹ��<IP��PORT>��Ԫ��ȷ��IPCͨ�ŵĶ˵㡣
Ŀǰ������224��Ϊ�̶�UDP�˿ڣ���ˣ����̱��ʵ������ָÿ�����̻��̲߳��õ�IP��ַ���õ�ַʹ�ûػ�IP��ַ��127.A.B.C����

���������ŵı��Ʒ������£�
127.A.B.C��
 	A����ʾ��ϵͳID�š���1��ʼ���б�š�
 	B����ʾ��ϵͳ�ڲ��Ľ��̱�š���1��ʼ���б�š�
 	C������ʵ����š���1��ʼ���б�š�


*/

#ifndef DATA_BUFF_LEN_MAX
#define DATA_BUFF_LEN_MAX   2048
#endif

/* Common struct */

#define MSG_PORT    224

#define MSG_ID(SS_ID, MOD_ID, MOD_MSG_ID) \
	((127<< 24) | (SS_ID <<16) | MOD_ID << 8 | MOD_MSG_ID)

	
#define PROC_NUM(SS_ID, SS_PROC_NUM, INSTANCE_NUM) \
	((127<< 24) | (SS_ID <<16) | (SS_PROC_NUM << 8) | (INSTANCE_NUM))


/* SubSystem ID */
#define SS_ID_APP               1
#define SS_ID_BSP               2
#define SS_ID_LTE               3
#define SS_ID_NET               4
#define SS_ID_WIFI              5
/* Mod ID */

#define MOD_ID_USERM            1

/* Instance ID */
#define INSTANCE_ID_APP         1
#define INSTANCE_ID_PAL         2
#define INSTANCE_ID_KEL         3

/* Mod ID inside */
#define INTERNAL_ID_GET_USR     1
#define INTERNAL_ID_GET_DEV     2
#define INTERNAL_ID_MOD_USR     3
#define INTERNAL_ID_MOD_DEV     4
#define INTERNAL_ID_CLEAR_TS    5


/* All Proc num */
#define PROC_NUM_USERM_APP      PROC_NUM(SS_ID_NET, MOD_ID_USERM, INSTANCE_ID_APP)   
#define PROC_NUM_USERM_PAL      PROC_NUM(SS_ID_NET, MOD_ID_USERM, INSTANCE_ID_PAL)
#define PROC_NUM_USERM_KEL      PROC_NUM(SS_ID_NET, MOD_ID_USERM, INSTANCE_ID_KEL)


/* All msg num */
#define MSG_ID_USERM_GET_USR    MSG_ID(SS_ID_NET, MOD_ID_USERM, INTERNAL_ID_GET_USR)
#define MSG_ID_USERM_GET_DEV    MSG_ID(SS_ID_NET, MOD_ID_USERM, INTERNAL_ID_GET_DEV) 
#define MSG_ID_USERM_MOD_USR    MSG_ID(SS_ID_NET, MOD_ID_USERM, INTERNAL_ID_MOD_USR)
#define MSG_ID_USERM_MOD_DEV    MSG_ID(SS_ID_NET, MOD_ID_USERM, INTERNAL_ID_MOD_DEV)
#define MSG_ID_USERM_CLEAR_TS   MSG_ID(SS_ID_NET, MOD_ID_USERM, INTERNAL_ID_CLEAR_TS)  


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __MSG_ID_H__ */
