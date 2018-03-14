#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/netlink.h>
#include <linux/if.h>
#include <linux/sockios.h>

#include <sys/un.h>
#include <syslog.h>
#include <linux/filter.h>

#include <config_common.h>
#include <nvram_config.h>

#include <fcntl.h>
#include <unistd.h>

#include <sys/mman.h>

#include "event.h"

#include "list.h"
#include "ag_common.h"
#include "ag_module.h"
#include "ag_msg.h"

s32 ag_module_ipc_get_server_ip_port(u32 *uipcipaddr, u16 *usipcport, u32 *uiSeripaddr, u16 *usSerport, u8 *pacName)
{
	#if 0
	s8 server_ip[NVRAM_LEN], serv_port[NVRAM_LEN];

	while(1)
	{
		if(nvram_get(server_ip, AG_MODULE_IPC_SERVER_IP, NVRAM_LEN) != 0)
		{
			memcpy(server_ip, AG_MODULE_IPC_DEFAULT_IP, strlen(AG_MODULE_IPC_DEFAULT_IP));
			nvram_set(AG_MODULE_IPC_SERVER_IP, server_ip);
			nvram_commit();
		}
		if(inet_pton(AF_INET, server_ip, uipcipaddr) <= 0)
		{
			AG_MODULE_LOG(LOG_ERR, "inet_pton fail");
			sleep(2);
		}
		else
		{
			break;
		}
	}

	if(nvram_get(serv_port, AG_MODULE_IPC_SERVER_PORT, NVRAM_LEN) != 0)
	{
		memcpy(serv_port, AG_MODULE_IPC_DEFAULT_PORT, strlen(AG_MODULE_IPC_DEFAULT_PORT));
		nvram_set(AG_MODULE_IPC_SERVER_PORT, serv_port);
		nvram_commit();
	}
	*usipcport = atoi(serv_port);
	#endif

       if(NULL == uipcipaddr || NULL == usipcport || NULL == uiSeripaddr ||NULL ==usSerport || NULL == pacName)   
       {
            AG_MODULE_LOG(LOG_ERR, "param is NULL return\n");
            return FALSE;
       }
	
	FILE *pfile;
	u8 ucBuffer[256] = "";
	u8 ucName[32] = "";
	u8 ucIpAddr[32] = "";
	u8 ucPort[32] = "";

	 *uipcipaddr=0, 
        *usipcport=0, 
        *uiSeripaddr=0, 
        *usSerport=0;
        
	pfile = fopen(IPC_CONFIG_FILE, "r");
	if(NULL == pfile)
	{
		AG_MODULE_LOG(LOG_ERR, "fopen "IPC_CONFIG_FILE" error, %s(errno: %d)", strerror(errno), errno);
		return FALSE;
	}

      if (NULL == pacName)
      {
          fclose(pfile);
          AG_MODULE_LOG(LOG_ERR, "pacName is NULL\n");
          return FALSE;
      }
      
	AG_MODULE_LOG(LOG_ERR, "pacName:%s", pacName);
       if(NULL == pacName)
        {
            return FALSE;
        }
	
	while(fgets(ucBuffer, 256, pfile))
	{
		/* 查找字符串 */
		if(NULL != strstr(ucBuffer, pacName))
		{
			sscanf(ucBuffer, "%s %s %s",ucName, ucIpAddr, ucPort);
			AG_MODULE_LOG(LOG_ERR, "name:%s IpAddr:%s Port:%s", ucName, ucIpAddr, ucPort);
			inet_pton(AF_INET, ucIpAddr, uipcipaddr);
			*usipcport = atoi(ucPort);
			break;
		}
		
		if(NULL != strstr(ucBuffer, "aglink"))
		{
			sscanf(ucBuffer, "%s %s %s",ucName, ucIpAddr, ucPort);
			AG_MODULE_LOG(LOG_ERR, "name:%s IpAddr:%s Port:%s", ucName, ucIpAddr, ucPort);
			inet_pton(AF_INET, ucIpAddr, uiSeripaddr);
			*usSerport = atoi(ucPort);
		}
		
	}

	fclose(pfile);
	
	AG_MODULE_LOG(LOG_ERR, "IPC Module ip: "IP_FMT" port:%d IPC server ip:"IP_FMT" port:%d ", 
		IP_ARG(uipcipaddr),  *usipcport, IP_ARG(uiSeripaddr), *usSerport);
	
	return TRUE;
}


s32 ag_module_ipc_connect_to_server(u32 uipcipaddr, u16 usipcport, u32 uiSeripaddr, u16 usSeripcport, AG_MODULE_IPC_REGISTER *pstIpcRegister)
{
	u32 iRet = 0;
	int iFlag = 1;
	struct timeval timeout = {SOCKET_SEND_TIMEOUT_VALUE, 0};
	
	s32 sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	memset(&pstIpcRegister->stipcskaddr, 0, sizeof(pstIpcRegister->stipcskaddr));
	pstIpcRegister->stipcskaddr.sin_family = AF_INET;
	pstIpcRegister->stipcskaddr.sin_port = htons(usipcport);
	pstIpcRegister->stipcskaddr.sin_addr.s_addr = uipcipaddr;

	memset(&pstIpcRegister->stSerskaddr, 0, sizeof(pstIpcRegister->stSerskaddr));
	pstIpcRegister->stSerskaddr.sin_family = AF_INET;
	pstIpcRegister->stSerskaddr.sin_port = htons(usSeripcport);
	pstIpcRegister->stSerskaddr.sin_addr.s_addr = uiSeripaddr;

	if(0 != setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&iFlag, sizeof(int)))
	{
		AG_MODULE_LOG(LOG_ERR, "socket setsockopt SO_REUSEADDR error.%s(errno: %d)", strerror(errno), errno);
		exit(-1);
		
	}

	//if(0 == memcmp(pstIpcRegister->acname, "aglink", strlen("aglink")))
	{
		if(bind(sockfd, (struct sockaddr *)&pstIpcRegister->stipcskaddr, sizeof(pstIpcRegister->stipcskaddr)) == -1)
		{
			AG_MODULE_LOG(LOG_ERR, "socket bind error.%s(errno: %d)", strerror(errno), errno);
			close(sockfd);
			exit(-1);
		}
	}

	#if 0
	int ifileFlag = 0;
	ifileFlag = fcntl(sockfd, F_GETFL, 0);

	if (fcntl(sockfd, F_SETFL, ifileFlag | O_NONBLOCK) < 0) {
		AG_MODULE_LOG(LOG_ERR, "socket bind error.%s(errno: %d)", strerror(errno), errno);
       	 return -1;
    	}
	#endif
	#if 1
	/*设置server fd发送超时为timeval*/
	if(0 !=setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(struct timeval)))
	{
		AG_MODULE_LOG(LOG_ERR, "socket setsockopt SO_SNDTIMEO error.%s(errno: %d)", strerror(errno), errno);
		exit(-1);
	}
	/*设置server fd接收超时为timeval*/
	if(0 !=setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval)))
	{
		AG_MODULE_LOG(LOG_ERR, "socket setsockopt SO_RCVTIMEO error.%s(errno: %d)", strerror(errno), errno);
		exit(-1);
	}
	#endif
	pstIpcRegister->uipcSocket = sockfd;

	AG_MODULE_LOG(LOG_INFO, "Module Socket:%d Dest ip:"IP_FMT" port:%d", pstIpcRegister->uipcSocket, 
			IP_ARG(&(pstIpcRegister->stipcskaddr.sin_addr.s_addr)), ntohs(pstIpcRegister->stipcskaddr.sin_port));

	
	AG_MODULE_LOG(LOG_INFO, "Server  Dest ip:"IP_FMT" port:%d",
				IP_ARG(&(pstIpcRegister->stSerskaddr.sin_addr.s_addr)), ntohs(pstIpcRegister->stSerskaddr.sin_port));

	AG_MODULE_LOG(LOG_WARNING, "IPC socket success!");
	return TRUE;
}



s32 ag_module_ipc_connect_server_onetime(AG_MODULE_IPC_REGISTER *pstIpcRegister)
{
        if(0 == strcmp(pstIpcRegister->acname,"aglink"))
        {
            return TRUE;
        }

        u8 buf[AG_MODULE_BUFFER_SIZE] = {0};
	AG_MODULE_LOG(LOG_DEBUG, "Module:%s to connect server onetime,uiFlag=%d\n", pstIpcRegister->uiFlag, pstIpcRegister->acname);

	/* 连接未成功 */
	if(!pstIpcRegister->uiFlag)
	{
		memcpy(buf, pstIpcRegister, sizeof(AG_MODULE_IPC_REGISTER));
		AG_MODULE_LOG(LOG_INFO, "Register module :%s , ModuleID:%d  sizeof(AG_MODULE_IPC_REGISTER):%d sizeof(AG_LINK_MSG_HEAD_T):%d", 
			pstIpcRegister->acname, pstIpcRegister->uiModuleId, sizeof(AG_MODULE_IPC_REGISTER), sizeof(AG_LINK_MSG_HEAD_T));	
		/* send registre info */
		ag_module_ipc_client_send_msg(AG_MSG_IPC_REGISTER, AG_IPC_MSG_TYPE_SELF_CPU, pstIpcRegister, buf, sizeof(
AG_MODULE_IPC_REGISTER));
	}
	
	
	return TRUE;
}






s32 ag_module_ipc_reconnect_server(AG_MODULE_IPC_REGISTER *pstIpcRegister)
{
    AG_MODULE_LOG(LOG_INFO, "uiFlag is %d Module:%s", pstIpcRegister->uiFlag, pstIpcRegister->acname);
    if(0 == pstIpcRegister->uiReconnectTvFlag)
    {/*没有Reconnect 定时任务待执行*/
            pstIpcRegister->uiReconnectTvFlag = 1;
            event_add(&pstIpcRegister->connect_ipc_server_ev, &pstIpcRegister->connect_ipc_server_tv);
    }
    return TRUE;
}

s32 ag_module_ipc_connect_server(s32 fd, short event, void *arg)
{
	AG_MODULE_IPC_REGISTER *pstIpcRegister;
	u8 buf[AG_MODULE_BUFFER_SIZE] = {0};

	pstIpcRegister = (AG_MODULE_IPC_REGISTER *)arg;

	AG_MODULE_LOG(LOG_INFO, "uiFlag is %d Module:%s", pstIpcRegister->uiFlag, pstIpcRegister->acname);

	/* 连接未成功 */
	if(!pstIpcRegister->uiFlag)
	{
		memcpy(buf, pstIpcRegister, sizeof(AG_MODULE_IPC_REGISTER));
		AG_MODULE_LOG(LOG_INFO, "Register module :%s , ModuleID:%d  sizeof(AG_MODULE_IPC_REGISTER):%d sizeof(AG_LINK_MSG_HEAD_T):%d", 
			pstIpcRegister->acname, pstIpcRegister->uiModuleId, sizeof(AG_MODULE_IPC_REGISTER), sizeof(AG_LINK_MSG_HEAD_T));	
		/* send registre info */
		ag_module_ipc_client_send_msg(AG_MSG_IPC_REGISTER, AG_IPC_MSG_TYPE_SELF_CPU, pstIpcRegister, buf, sizeof(AG_MODULE_IPC_REGISTER));
		//event_add(&pstIpcRegister->connect_ipc_server_ev, &pstIpcRegister->connect_ipc_server_tv);
	}

	pstIpcRegister->uiReconnectTvFlag = 0;
	
	return TRUE;
}

s32 ag_module_ipc_module_event_init(AG_MODULE_IPC_REGISTER *pstIpcRegister, IPC_CLIENT_RECV_EVENT *pRecv_fun)
{
	
	/* !aglink */
	if(memcmp(pstIpcRegister->acname, "aglink", strlen("aglink")))
	{
		pstIpcRegister->main_base = event_init();

		pstIpcRegister->connect_ipc_server_tv.tv_sec = CONNECT_IPC_SERVER_INTERVAL;
		pstIpcRegister->connect_ipc_server_tv.tv_usec = 0;

		evtimer_set(&pstIpcRegister->connect_ipc_server_ev,  ag_module_ipc_connect_server, pstIpcRegister);
		event_add(&pstIpcRegister->connect_ipc_server_ev, &pstIpcRegister->connect_ipc_server_tv);
              pstIpcRegister->uiReconnectTvFlag = 1;
		AG_MODULE_LOG(LOG_INFO, "Module:%s Set timer to connect server", pstIpcRegister->acname);
	}
	
	/* aglink and !aglink pthread */
	if(pRecv_fun != NULL)
	{
		/*消息接收回调*/
		event_set(&pstIpcRegister->recv_ev, pstIpcRegister->uipcSocket, EV_READ | EV_PERSIST, 
			pRecv_fun, (void *)&pstIpcRegister->recv_ev);
	
		/*开启消息接收*/
		event_add(&pstIpcRegister->recv_ev, NULL);
		AG_MODULE_LOG(LOG_INFO, "Module:%s  Call back fun address:%x", pstIpcRegister->acname, pRecv_fun);
	}

	return TRUE;
}


s32 ag_module_ipc_module_init(u8 *pacName, u32 uiModuleId, AG_MODULE_IPC_REGISTER *pstIpcRegister,  IPC_CLIENT_RECV_EVENT *pRecv_fun)
{
	u32 uipcipaddr;
	u16 usipcport;
	u32 uiServeraddr;
	u16 usServerPort;
	
	if(pacName == NULL)
	{
		AG_MODULE_LOG(LOG_ERR, "PARAM is NULL");
		return FALSE;
	}
	
	if(NULL == pstIpcRegister)
	{
		AG_MODULE_LOG(LOG_ERR, "ag_module_ipc_module_register fail");
		return FALSE;	
	}

	if(NULL == memcpy(pstIpcRegister->acname, pacName, strlen(pacName)))
	{
		AG_MODULE_LOG(LOG_ERR, "copy module name fail!");
		return FALSE;
	}

	pstIpcRegister->uiModuleId = uiModuleId;
	AG_MODULE_LOG(LOG_WARNING, "module name:%s uiModuleId:%d; pacName:%s", pstIpcRegister->acname, uiModuleId, pacName);

	ag_module_ipc_get_server_ip_port(&uipcipaddr, &usipcport, &uiServeraddr, &usServerPort, pacName);
	
	ag_module_ipc_connect_to_server(uipcipaddr, usipcport, uiServeraddr, usServerPort, pstIpcRegister);

	pstIpcRegister->uiFlag = 0;
	
	/*初始化libevent*/
	ag_module_ipc_module_event_init(pstIpcRegister, pRecv_fun);
	
	
	return TRUE;
}
/*跨版单播发送*/
s32 ag_module_ipc_client_send_msg_to_ap(u32 uiMsgID, u32 uiMsgType, AG_MODULE_IPC_REGISTER *pstIpcRegister, u8 *pbuff, int iMsgLen, u8 *ucMac)
{
	u8 ucSendbuf[AG_MODULE_BUFFER_SIZE] = {0};
	u8 *p = ucSendbuf;
	u8 **pnt = &p;
	u32 uitotallen = 0;
	AG_LINK_MSG_HEAD_T pstMsgHead;
	AG_LINK_MSG_UNICAST_HEAD_T pstUnicastHead;
	
	if(NULL == pstIpcRegister  || NULL == ucMac)
	{
		AG_MODULE_LOG(LOG_EMERG, "pstIpcRegister is NULL");
	}
	uitotallen = sizeof(AG_LINK_MSG_HEAD_T) + iMsgLen + sizeof(AG_LINK_MSG_UNICAST_HEAD_T);
	if(uitotallen > AG_MODULE_BUFFER_SIZE)
	{
		AG_MODULE_LOG(LOG_EMERG, "Send msg length is %d, bigger than  AG_MODULE_BUFFER_SIZE",uitotallen);
		return FALSE;
	}

	memset(&pstMsgHead, 0, sizeof(AG_LINK_MSG_HEAD_T));
	pstMsgHead.usMsgId = uiMsgID;
	pstMsgHead.usMsgType = uiMsgType;
	memcpy(pstMsgHead.acname, pstIpcRegister->acname, sizeof(pstIpcRegister->acname));
	pstMsgHead.uiMsgLen = iMsgLen;
	pstMsgHead.uiUnicast = 1;
	//ENCODE_PUT(pnt, &pstMsgHead, sizeof(AG_LINK_MSG_HEAD_T));
	ENCODE_PUTL(pnt, pstMsgHead.usMsgId);
	ENCODE_PUTW(pnt, pstMsgHead.usMsgType);
	ENCODE_PUTW(pnt, pstMsgHead.uiUnicast);
	ENCODE_PUT(pnt,pstMsgHead.acname, MODULE_NAME_LENGTH);
	ENCODE_PUTL(pnt, pstMsgHead.uiMsgLen);
	

	memset(&pstUnicastHead, 0, sizeof(AG_LINK_MSG_UNICAST_HEAD_T));
	strncpy(&pstUnicastHead.ucMac, ucMac, MAC_LEN);
	ENCODE_PUT(pnt, &pstUnicastHead, sizeof(AG_LINK_MSG_UNICAST_HEAD_T));
	
	AG_MODULE_LOG(LOG_INFO, "Msgid:%d MsgType:%d Name:%s, MsgLen:%d MAC:"MAC_FMT, 
		pstMsgHead.usMsgId, pstMsgHead.usMsgType, pstMsgHead.acname, pstMsgHead.uiMsgLen, MAC_ARG(pstUnicastHead.ucMac));
	
	if(iMsgLen > 0)
	{
		ENCODE_PUT(pnt, pbuff, iMsgLen);
	}

	AG_MODULE_LOG(LOG_INFO, "Socket:%d  Dest ip:"IP_FMT" port:%d", pstIpcRegister->uipcSocket, 
		IP_ARG(&(pstIpcRegister->stSerskaddr.sin_addr.s_addr)), ntohs(pstIpcRegister->stSerskaddr.sin_port));
	
	if( sendto(pstIpcRegister->uipcSocket, ucSendbuf, uitotallen, 0, 
			(struct sockaddr *)&pstIpcRegister->stSerskaddr, sizeof(pstIpcRegister->stSerskaddr)) < 0)
	{
		AG_MODULE_LOG(LOG_ERR, "send msg error: %s(errno: %d)", strerror(errno), errno);
		return FALSE;
	}
	return TRUE;
}
	
s32 ag_module_ipc_client_send_msg(u32 uiMsgID, u32 uiMsgType, AG_MODULE_IPC_REGISTER *pstIpcRegister, u8 *pbuff, int iMsgLen)
{
	u8 ucSendbuf[AG_MODULE_BUFFER_SIZE] = {0};
	u8 *p = ucSendbuf;
	u8 **pnt = &p;
	u32 uitotallen = 0;
	AG_LINK_MSG_HEAD_T pstMsgHead;

	if(NULL == pstIpcRegister)
	{
		AG_MODULE_LOG(LOG_EMERG, "pstIpcRegister is NULL");
	}
	uitotallen = sizeof(AG_LINK_MSG_HEAD_T) + iMsgLen;
	if(uitotallen > AG_MODULE_BUFFER_SIZE)
	{
		AG_MODULE_LOG(LOG_EMERG, "Send msg length is %d, bigger than  AG_MODULE_BUFFER_SIZE",uitotallen);
		return FALSE;
	}

	memset(&pstMsgHead, 0, sizeof(AG_LINK_MSG_HEAD_T));
	pstMsgHead.usMsgId = uiMsgID;
	pstMsgHead.usMsgType = uiMsgType;
	memcpy(pstMsgHead.acname, pstIpcRegister->acname, sizeof(pstIpcRegister->acname));
	pstMsgHead.uiMsgLen = iMsgLen;

	//ENCODE_PUT(pnt, &pstMsgHead, sizeof(AG_LINK_MSG_HEAD_T));
	ENCODE_PUTL(pnt, pstMsgHead.usMsgId);
	ENCODE_PUTW(pnt, pstMsgHead.usMsgType);
	ENCODE_PUTW(pnt, pstMsgHead.uiUnicast);
	ENCODE_PUT(pnt,pstMsgHead.acname, MODULE_NAME_LENGTH);
	ENCODE_PUTL(pnt, pstMsgHead.uiMsgLen);
		
	AG_MODULE_LOG(LOG_INFO, "Msgid:%x MsgType:%d Name:%s, MsgLen:%d", pstMsgHead.usMsgId, pstMsgHead.usMsgType, pstMsgHead.acname, pstMsgHead.uiMsgLen);
	
	if(iMsgLen > 0)
	{
		ENCODE_PUT(pnt, pbuff, iMsgLen);
	}

	AG_MODULE_LOG(LOG_INFO, "Socket:%d  Dest ip:"IP_FMT" port:%d", pstIpcRegister->uipcSocket, 
		IP_ARG(&(pstIpcRegister->stSerskaddr.sin_addr.s_addr)), ntohs(pstIpcRegister->stSerskaddr.sin_port));
	
	if( sendto(pstIpcRegister->uipcSocket, ucSendbuf, uitotallen, 0, 
			(struct sockaddr *)&pstIpcRegister->stSerskaddr, sizeof(pstIpcRegister->stSerskaddr)) < 0)
	{
		AG_MODULE_LOG(LOG_ERR, "send msg error: %s(errno: %d)", strerror(errno), errno);
		return FALSE;
	}
	
	return TRUE;
}

s32 ag_module_ipc_power_on(AG_MODULE_IPC_REGISTER *pstIpcRegister)
{
	AG_MODULE_IPC_MODULE_LIST_T *pstIPCModule;

	if(NULL == pstIpcRegister)
	{
		AG_MODULE_LOG(LOG_ALERT, "param is NULL");
		return FALSE;
	}

	AG_MODULE_LOG(LOG_INFO, "Module name is:%s uiModuleId is:%d poweron", pstIpcRegister->acname, pstIpcRegister->uiModuleId);

	#if 0
	pstIPCModule = ag_module_ipc_find_by_head(pstIpcRegister->acname);

	if(NULL == pstIPCModule)
	{
		AG_MODULE_LOG(LOG_INFO, "Module not register, Module name is:%s IDSet is:%d", pstIPCModule->acname, pstIPCModule->uiIdSet);
				
		return FALSE;
	}
	#endif

	if(NULL == pstIpcRegister->main_base)
	{
		AG_MODULE_LOG(LOG_INFO, "main_base is  NULL, ");
		return FALSE;
	}

	 
     ag_module_ipc_connect_server_onetime(pstIpcRegister);
	/*进入主循环*/
	event_base_loop(pstIpcRegister->main_base, 0);
	
	return TRUE;
}
