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
进程编号为32位无符号整型数值。

在AP平台中，所有以进程或线程存在的运行实体，相互之间都可以通过通用库的IPC进行异步通信。进程编号是IPC
通信的一个关键字，是每个进程或线程进行通信的窗口。
在AP平台中的IPC通信中，采用UDP通信机制，使用<IP，PORT>二元组确定IPC通信的端点。
目前，采用224作为固定UDP端口，因此，进程编号实际上是指每个进程或线程采用的IP地址。该地址使用回环IP地址（127.A.B.C）。

进程索引号的编制方法如下：
127.A.B.C：
 	A：表示子系统ID号。从1开始进行编号。
 	B：表示子系统内部的进程编号。从1开始进行编号。
 	C：进程实例编号。从1开始进行编号。


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
