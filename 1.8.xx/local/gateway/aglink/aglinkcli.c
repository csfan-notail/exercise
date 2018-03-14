/******************************************************************************

  Copyright (C), 2010-2011, GBCOM Co., Ltd.

 ******************************************************************************
  File Name     : aglinkdp.c
  Version       : Initial Draft
  Author        : lvjianlin
  Created       : 2015/3/12
  Last Modified :
  Description   : aglink 调试工具
  Function List :
  History       :
  1.Date        : 2015/3/12
    Author      : lvjianlin
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include "aglinkcli.h"
#include "ag_msg.h"
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
AGLINKCLI_HELP_T	gstAglinkclicmd[]=
{
	{"getmodule", aglinkcli_get_module_info, "get module info already registed in aglink"},
	{"getstate", aglinkcli_get_online_info, "get device state whether is registed"},
	{"getaplist", aglinkcli_get_ap_list_info, "get ap list from GW`s aglink"},
	//{"keeplive", aglinkcli_set_keeplive_tm, "set/get keeplive time"},
	{"debug",aglinckcli_set_debug_level,"set debug level 1--7"},
	#ifndef GW_X86
	//{"registerap",aglinkcli_register_apinfo_to_gw,"send register devinfo to gw"},
	#endif 
};

AGLINKCLI_HELP_T *gpCmd = NULL;
AG_MODULE_IPC_REGISTER gstAglinkcliIPCmgt = {0};
int giargc = 0;
char * (*giargv)[] = {0};

/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macros                                       *
 *----------------------------------------------*/
#if 0
#define LOG_FAIL		2
#define LOG_MSG		1
#define LOG_DEBUG	0
#endif
//#define printf(level, format, arg...) do{\
#define printf(format, arg...) do{\
		printf(format, ##arg);			  \
		/*syslog(level,format, ##arg);*/	\ 
	}while(0);

/*----------------------------------------------*
 * routines' implementations                    *
 *----------------------------------------------*/

int stop_process(void)
{
	return 0;
}

void handle_signal(int signo)
{
	printf("Signo is %d\n", signo);

	/*if support backtrace*/
	#if 0
	get_backtrace();
	#endif

	stop_process();
    	exit(0);
}

int init_signal()
{
	struct sigaction stSig;

	stSig.sa_flags = 0;
	sigemptyset(&stSig.sa_mask);

	sigaddset(&stSig.sa_mask, SIGSEGV);
	sigaddset(&stSig.sa_mask, SIGBUS);
	sigaddset(&stSig.sa_mask, SIGTERM);
	sigaddset(&stSig.sa_mask, SIGHUP);
	sigaddset(&stSig.sa_mask, SIGINT);
	sigaddset(&stSig.sa_mask, SIGPIPE);
	sigaddset(&stSig.sa_mask, SIGALRM);

	stSig.sa_handler = handle_signal;
	sigaction(SIGSEGV, &stSig, NULL);
	sigaction(SIGBUS, &stSig, NULL);
	sigaction(SIGTERM, &stSig, NULL);
	sigaction(SIGHUP, &stSig, NULL);
	sigaction(SIGINT, &stSig, NULL);
	sigaction(SIGPIPE, &stSig, NULL);
	sigaction(SIGALRM, &stSig, NULL);

	/*慢系统调用被信号中断后可能会重启，关闭重启或者将sa_flags设置为取消SA_RESTART标志*/
	siginterrupt(SIGALRM, 1);
	siginterrupt(SIGBUS, 1);
	siginterrupt(SIGTERM, 1);
	siginterrupt(SIGHUP, 1);
	siginterrupt(SIGKILL, 1);
	siginterrupt(SIGSEGV, 1);
	siginterrupt(SIGPIPE, 1);

	return 0;
}

int aglinkcli_help(void)
{
	AGLINKCLI_HELP_T *cmd_help;
	for(cmd_help = gstAglinkclicmd; cmd_help->pcName; cmd_help++)
	{
		printf("\t%-20s -- %s\n", cmd_help->pcName, cmd_help->pcHelp);
	}
	
	return 0;
}


int aglinkcli_get_module_info(int argc, char *argv[])
{	
	int iRet = -1;
	printf("aglinkcli_get_module_info\n");
	iRet = ag_module_ipc_client_send_msg(AGLINK_CLI_GET_MODULE_INFO, AG_IPC_MSG_TYPE_SELF_CPU, &gstAglinkcliIPCmgt, NULL, 0);
	if(FALSE == iRet)
	{
		printf("Send msg AGLINK_CLI_GET_MODULE_INFO error");
	}
	return 0;
}

int aglinkcli_get_online_info(int argc, char *argv[])
{
	int iRet = -1;
	
	printf("aglinkcli_get_online_info\n");
	iRet = ag_module_ipc_client_send_msg(AGLINK_CLI_GET_ONLINE_INFO, AG_IPC_MSG_TYPE_SELF_CPU, &gstAglinkcliIPCmgt, NULL, 0);
	if(FALSE == iRet)
	{
		printf("Send msg AGLINK_CLI_GET_ONLINE_INFO error");
	}
	return 0;
}

int aglinkcli_get_ap_list_info(int argc, char *argv[])
{
	int iRet = -1;
	printf("aglinkcli_get_ap_list_info\n");
	iRet = ag_module_ipc_client_send_msg(AGLINK_CLI_GET_AP_LIST, AG_IPC_MSG_TYPE_SELF_CPU, &gstAglinkcliIPCmgt, NULL, 0);
	if(FALSE == iRet)
	{
		printf("Send msg AGLINK_CLI_GET_AP_LIST error");
	}
	return 0;
}

#ifndef GW_X86
int aglinkcli_register_apinfo_to_gw(int argc, char *argv[])
{
	int iRet = -1;
	printf("aglinkcli_register_apinfo_to_gw\n");
	iRet = ag_module_ipc_client_send_msg(AGLINK_CLI_SEND_APINFO_TO_GW, AG_IPC_MSG_TYPE_SELF_CPU, &gstAglinkcliIPCmgt, NULL, 0);
	if(FALSE == iRet)
	{
		printf("Send msg AGLINK_CLI_GET_AP_LIST error");
	}
	return 0;
}
#endif

int aglinckcli_set_debug_level(int argc, char *argv[])
{
    int iRet = -1;
    char aucLevel[8]={0},cLevel;
    if(2 == argc)
    {
        snprintf(aucLevel,8,argv[1]);
        printf("level = %s\n",aucLevel);
        cLevel = atoi(aucLevel);
        if(cLevel>=0 && cLevel <=7)
        {
            iRet = ag_module_ipc_client_send_msg(AGLINK_CLI_SET_DEBUG_LEVEL, AG_IPC_MSG_TYPE_SELF_CPU, &gstAglinkcliIPCmgt, (u8 *)&(aucLevel[0]), 1);
            if(FALSE == iRet)
            {
                printf("Send msg AGLINK_CLI_SET_KEEPLIVE_TM error");
            }
        }
        else
        {
            printf("debug level 1-7\n");
        }
    }
    
    raise(SIGTERM);
    return iRet;
}



int aglinkcli_set_keeplive_tm(int argc, char *argv[])
{
	int iRet = -1;
	u8 buf[32] = {0};
	u8 *p = &buf[0];
	u8 **pnt = &p;
	u32 ulLength = 0;
	AGLINKCLI_KEEPLIVE_T stKeeptm = {0};
	AGLINKCLI_MSG_T stMsg = {0};

	if(1 == argc)
	{
		iRet = ag_module_ipc_client_send_msg(AGLINK_CLI_SET_KEEPLIVE_TM, AG_IPC_MSG_TYPE_SELF_CPU, &gstAglinkcliIPCmgt, NULL, 0);
		if(FALSE == iRet)
		{
			printf("Send msg AGLINK_CLI_SET_KEEPLIVE_TM error");
		}
	}
	else if( 2 == argc )
	{
		/*设置保活时间*/
		printf("set keeplive time is :%d\n", atoi(argv[1]));
		stMsg.u.stKeeplive.uiKeeptm = atoi(argv[1]);

		stMsg.iFlag = 1;
		ENCODE_PUTL(pnt, stMsg.iFlag);
		ENCODE_PUT(pnt, &stMsg.u.stKeeplive, sizeof(AGLINKCLI_KEEPLIVE_T));
		ulLength = p - &buf[0];
		iRet = ag_module_ipc_client_send_msg(AGLINK_CLI_SET_KEEPLIVE_TM, AG_IPC_MSG_TYPE_SELF_CPU, &gstAglinkcliIPCmgt, (u8 *)&buf, ulLength);
		if(FALSE == iRet)
		{
			printf("Send msg AGLINK_CLI_SET_KEEPLIVE_TM error");
		}

	}
	return 0;
}

int aglinkcli_handle_module_info_ack(u8 **pnt, int iLength)
{
	AGLINKCLI_MSG_T stMsg = {0};
	int iLoop = 0;
	
	if(NULL == *pnt || NULL ==pnt)
	{
		printf("param is NULL!");
		return FALSE;
	}

	#if 0
	if(iLength != sizeof(AGLINKCLI_MSG_T))
	{
		printf("iLength=%d, sizeof(AGLINKCLI_MSG_T)=%d", iLength, sizeof(AGLINKCLI_MSG_T));
		return FALSE;
	}
	#endif
	
	DECODE_GETL(pnt, stMsg.iFlag);
	

	if(0 == stMsg.iFlag)
	{
		DECODE_GET(&stMsg.u.ucErrMsg, pnt, sizeof(stMsg.u.ucErrMsg));
		printf("%s\n", stMsg.u.ucErrMsg);
		return FALSE;
	}
	
	DECODE_GET(&stMsg.u.stModuleInfo, pnt, sizeof(AGLINKCLI_MODULE_INFO_T));
	printf("Module number is:%d\n", stMsg.u.stModuleInfo.iNum);
	printf("%-15s %-15s\n", "Module Name", "IP             PORT");
	for(iLoop = 0; iLoop < stMsg.u.stModuleInfo.iNum; iLoop++)
	{
		printf("%-15s "IP_FMT":%d\n", stMsg.u.stModuleInfo.stModule[iLoop].acname, IP_ARG(&(stMsg.u.stModuleInfo.stModule[iLoop].stipcskaddr.sin_addr.s_addr)), 
			ntohs(stMsg.u.stModuleInfo.stModule[iLoop].stipcskaddr.sin_port));
	}
	
	return TRUE;
}

int aglinkcli_handle_online_info_ack(u8 **pnt, int iLength)
{
	AGLINKCLI_MSG_T stMsg = {0};
	int iLoop = 0;
	
	if(NULL == *pnt || NULL ==pnt)
	{
		printf("param is NULL!");
		return FALSE;
	}

	#if 0
	if(iLength != sizeof(AGLINKCLI_MSG_T))
	{
		printf("iLength=%d, sizeof(AGLINKCLI_MSG_T)=%d", iLength, sizeof(AGLINKCLI_MSG_T));
		return FALSE;
	}
	#endif

	DECODE_GETL(pnt, stMsg.iFlag);

	if(0 == stMsg.iFlag)
	{
		DECODE_GET(&stMsg.u.ucErrMsg, pnt, sizeof(stMsg.u.ucErrMsg));
		printf("%s\n", stMsg.u.ucErrMsg);
	}
	else if (1 == stMsg.iFlag)
	{
		DECODE_GET(&stMsg.u.stOnline, pnt, sizeof(AGLINKCLI_ONLINE_STATE_T));
		
		printf("Online state:%d\nOnline time:%d\nGW IP:"IP_FMT"\nGW MAC:"MAC_FMT"\n", 
			stMsg.u.stOnline.uiOnlinestate, stMsg.u.stOnline.uiOnlinetm,  IP_ARG(&stMsg.u.stOnline.uiSerIp), MAC_ARG(stMsg.u.stOnline.ucSerMAC));
	}
	else
	{
		printf("error flag(%d)", stMsg.iFlag);
	}
	return TRUE;
}

int aglinkcli_handle_ap_list_ack(u8 **pnt, int iLength)
{
	u8 buf[6144] = {0};
	u8 *sp = &buf[0];
	u8 **spnt = &sp;
	AGLINKCLI_MSG_T stMsg = {0};
	int iLoop = 0, iRet = -1, ulLength = 0;
	
	if(NULL == *pnt || NULL ==pnt)
	{
		printf("param is NULL!");
		return FALSE;
	}

	DECODE_GETL(pnt, stMsg.iFlag);

	if(0 == stMsg.iFlag)
	{
		DECODE_GET(&stMsg.u.ucErrMsg, pnt, sizeof(stMsg.u.ucErrMsg));
		printf("%s\n", stMsg.u.ucErrMsg);
	}
	else if (1 == stMsg.iFlag)
	{
		DECODE_GET(&stMsg.u.stAplistInfo, pnt, sizeof(AGLINKCLI_AP_LIST_T));
		
		printf("Ap Num is:%d\n", stMsg.u.stAplistInfo.iNum);
		printf("%-15s %-15s %-15s %-15s\n", "IP", "MAC", "Dev name", "Module id");
		for(iLoop = 0; iLoop < stMsg.u.stAplistInfo.iNum; iLoop++)
		{
			printf(IP_FMT"  "IP_FMT"  %s %d\n", 
				IP_ARG(stMsg.u.stAplistInfo.stApInfo[iLoop].ip), MAC_ARG(stMsg.u.stAplistInfo.stApInfo[iLoop].mac), \
				stMsg.u.stAplistInfo.stApInfo[iLoop].dev_name, stMsg.u.stAplistInfo.stApInfo[iLoop].model_id);
		}

		if(AGLINK_GW_AP_NUM_MAX == stMsg.u.stAplistInfo.iNum)
		{
			AGLINKCLI_MSG_T stSendMsg = {0};
			stSendMsg.iFlag = 1;
			stSendMsg.u.stAplistInfo.ibegin = stMsg.u.stAplistInfo.ibegin;

			ENCODE_PUTL(spnt, stMsg.iFlag);
			ENCODE_PUT(spnt, &stMsg.u.stAplistInfo, sizeof(AGLINKCLI_AP_LIST_T));
			ulLength = sp - &buf[0];

			iRet = ag_module_ipc_client_send_msg(AGLINK_CLI_GET_AP_LIST, AG_IPC_MSG_TYPE_SELF_CPU, &gstAglinkcliIPCmgt, (u8 *)&buf, ulLength);
			if(FALSE == iRet)
			{
				printf("Send msg AGLINK_CLI_GET_AP_LIST error");
			}
		}
		
	}
	else
	{
		printf("error flag(%d)", stMsg.iFlag);
	}

	return TRUE;
}

int aglinkcli_handle_keeplive_tm_ack(u8 **pnt, int iLength)
{
	AGLINKCLI_MSG_T stMsg = {0};
	int iLoop = 0;
	
	if(NULL == *pnt || NULL ==pnt)
	{
		printf("param is NULL!");
		return FALSE;
	}

	
	DECODE_GETL(pnt, stMsg.iFlag);
	DECODE_GET(&stMsg.u.stKeeplive.uiKeeptm, pnt, sizeof(AGLINKCLI_KEEPLIVE_T));

	printf("Keep live timer is:%d\n", stMsg.u.stKeeplive.uiKeeptm);

	return TRUE;
}


int aglinkcli_module_msg_parse(u8 *buf, u32 len, struct sockaddr_in *sk_addr)
{
	u8 *p = buf;
	u8 **pnt = &p;
	u32 iRet;
	AG_LINK_MSG_HEAD_T stMsgHead = {0};

	if(NULL == buf || (len <= 0))
	{
		printf("param is NULL\n");
		return FALSE;
	}

	DECODE_GETL(pnt, stMsgHead.usMsgId);
	DECODE_GETW(pnt, stMsgHead.usMsgType);
	DECODE_GETW(pnt, stMsgHead.uiUnicast);
	DECODE_GET(stMsgHead.acname, pnt, MODULE_NAME_LENGTH);
	DECODE_GETL(pnt, stMsgHead.uiMsgLen);

	printf("acName:%s, usMsgId:%d, usMsgType:%d, uiMsgLen:%d  msg total Len:%d\n",
		stMsgHead.acname, stMsgHead.usMsgId, stMsgHead.usMsgType, stMsgHead.uiMsgLen, len);

	switch(stMsgHead.usMsgId)
	{
		case AG_MSG_IPC_REGISTER_ACK:
			printf("Recv AG_IPC_MSG_REGISTER_ACK msg\n");
			gstAglinkcliIPCmgt.uiFlag = 1;
			printf("Cmd:%s\n", gpCmd->pcName);
			gpCmd->pFunc(giargc, *giargv);
			break;
		case AG_MSG_IPC_UNREGISTER:
			printf("Recv AG_IPC_MSG_UNREGISTER msg\n");
			gstAglinkcliIPCmgt.uiFlag = 0;
			ag_module_ipc_reconnect_server(&gstAglinkcliIPCmgt);
			break;
		case BC_MSG_AP_REGISTER_TO_GW_SUCCESS:
			printf("Recv BC_MSG_AP_REGISTER_TO_GW_SUCCESS msg\n");
			break;
		case AGLINK_CLI_GET_MODULE_INFO_ACK:
			printf("Recv AGLINK_CLI_GET_MODULE_INFO_ACK msg\n");
			aglinkcli_handle_module_info_ack(pnt, stMsgHead.uiMsgLen-sizeof(AG_LINK_MSG_HEAD_T));
			raise(SIGTERM);
			break;
		case AGLINK_CLI_GET_ONLINE_INFO_ACK:
			printf("Recv AGLINK_CLI_GET_ONLINE_INFO_ACK msg\n");
			aglinkcli_handle_online_info_ack(pnt, stMsgHead.uiMsgLen-sizeof(AG_LINK_MSG_HEAD_T));
			raise(SIGTERM);
			break;
		case AGLINK_CLI_GET_AP_LIST_ACK:
			printf("Recv AGLINK_CLI_GET_AP_LIST_ACK msg\n");
			aglinkcli_handle_ap_list_ack(pnt, stMsgHead.uiMsgLen-sizeof(AG_LINK_MSG_HEAD_T));
			raise(SIGTERM);
			break;
		case AGLINK_CLI_SET_KEEPLIVE_TM_ACK:
			printf("Recv AGLINK_CLI_SET_KEEPLIVE_TM_ACK msg\n");
			aglinkcli_handle_keeplive_tm_ack(pnt, stMsgHead.uiMsgLen-sizeof(AG_LINK_MSG_HEAD_T));
			raise(SIGTERM);
			break;
		case AGLINK_CLI_SEND_APINFO_TO_GW_ACK:
		{
		    printf("Recv AGLINK_CLI_SEND_APINFO_TO_GW_ACK msg\n");
		    raise(SIGTERM);
		    break;
		}
		default:
			printf("Recv msg id(%d)\n", stMsgHead.usMsgId);
			break;
	}

	return TRUE;
}

int aglinkcli_module_recv_msg(evutil_socket_t fd, short event, void *arg)
{
	u8 ucRecvbuf[AG_MODULE_BUFFER_SIZE] = {0};
	s32 size;	
	struct sockaddr_in sk_addr;
	socklen_t len = sizeof(sk_addr);

	size = recvfrom(fd, (void *)ucRecvbuf, AG_MODULE_BUFFER_SIZE, 0, (struct sockaddr *)&sk_addr, &len);
	if(-1 == size)
	{
		printf("Recvfrom error.%s(errno: %d)", strerror(errno), errno);
		return FALSE;
	}
	else
	{
		aglinkcli_module_msg_parse(ucRecvbuf, size, &sk_addr);
	}
		
	return 0;
}

int aglinkcli_module_init()
{
	int iRet=0;
	unsigned int iModuleID = 0;
	memset(&gstAglinkcliIPCmgt, 0, sizeof(AG_MODULE_IPC_REGISTER));

	iModuleID = (unsigned int)AG_GET_MODULE_ID(AGLINKCLI_MSG_BEGIN);
	iRet = ag_module_ipc_module_init("Aglinkcli", iModuleID, &gstAglinkcliIPCmgt, aglinkcli_module_recv_msg);
	if(FALSE == iRet)
	{
		printf("Register fail!");
		return -1;
	}

	ag_module_ipc_power_on(&gstAglinkcliIPCmgt);

	printf("EXIT");
	return 0;
}

int main(int argc, char*argv[])
{
	AGLINKCLI_HELP_T *pCmd;
	int i = 0;
	int iFind = 0;

	init_signal();	

	if(argc < 2)
	{
		aglinkcli_help();
	}

	argc--;
	argv++;

	giargc = argc;
	giargv = argv;
	
	for(pCmd = gstAglinkclicmd; pCmd->pcName; pCmd++)
	{
		if(!strcmp(pCmd->pcName, argv[0]))
		{
			iFind = 1;
			gpCmd = pCmd;
			break;
		}
		
	}
	
	if(0 == iFind)
	{
		aglinkcli_help();
		return 0;
	}

	aglinkcli_module_init();

	return 0;
}

