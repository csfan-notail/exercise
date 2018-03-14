/******************************************************************************

  Copyright (C), 2010-2011, GBCOM Co., Ltd.

 ******************************************************************************
  File Name     : ag_module.c
  Version       : Initial Draft
  Author        : lvjianlin
  Created       : 2014/11/17
  Last Modified :
  Description   : ag_module 
  Function List :
  History       :
  1.Date        : 2014/11/17
    Author      : lvjianlin
    Modification: Created file

******************************************************************************/
#include "ag_module.h"
#include "ag_msg.h"


//#include "aglink_temp.h"

/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/
s32 ag_module_init_aglink(u8 * pagmodule);
s32 ag_module_contral_init_aglink(u8 * pagmodule);
s32 ag_module_bussiness_init_aglink(void);
s32 ag_module_event_aglink(u8 * pagmodule);


 AG_MODULE_SETUP astAglinkModule[] =
{	
	{ "aglink", ag_module_init_aglink, ag_module_contral_init_aglink, \
		ag_module_bussiness_init_aglink,ag_module_event_aglink, ((void *)0)},
	{0}
};

AG_MODULE_IPC_REGISTER gstAglinkIPCmgt;
u32 gulIPCIdSet = 0;
u32 gulIPCListNum = 0;

struct list_head gstIPClist = LIST_HEAD_INIT(gstIPClist);
static struct hlist_head gstIPCHashList[AG_MODULE_HASH_LIST_SIZE];

pthread_mutex_t ipc_hlist_mutex; 	

#define AG_MSG_LOCK()		pthread_mutex_lock(&ipc_hlist_mutex)
#define AG_MSG_UNLOCK()	pthread_mutex_unlock(&ipc_hlist_mutex)

#define AG_MODULE_ORDER 	9
u8 *gucModuleorder[AG_MODULE_ORDER] = {"aglink", "aglink", "aglink", "stam", "snmpagent", "wifidog","Aglinkcli","Boxll","Stadct"};

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

/*****************************************************************************
 Prototype    : ag_module_is_running
 Description  : 检查是否有实例在运行
 Input        : u8 *pidfile  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/11/17
    Author       : lvjianlin
    Modification : Created function

*****************************************************************************/
s32 ag_module_is_running(u8 *pidfile)
{
	FILE *file;
	s8 *line = NULL;
	s32 pid = 0;
	s32 fd = 0;
	s8 status_file[64] = {0};
	s8 fbuf[512];
	s8 proc_name[32] = {0};

	if(NULL == pidfile)
	{
		return  FALSE;
	}
	/*1.  检查pid file是否已存在且合法*/
	if((file = fopen(pidfile, "r")) == NULL)
	{
		return  FALSE;
	}
	
	if(fscanf(file, "%d", &pid) == EOF){
		AG_MODULE_LOG(LOG_ERR, "Error, reading %s", pidfile);
		fclose(file);
		return  FALSE;
	}
	fclose(file);
	if (pid == getpid()) /*为本进程pidfile*/
	{
		return  FALSE;
	}

	/*2. 如果pid file已存在且合法，检查是否所指进程正在运行*/
	/* Test to see if the daemon is still running by issuing a fake kill */
	if (kill(pid, 0) < 0){
		if(errno == ESRCH) /*进程不存在*/
			return FALSE;
	}

	/*3. 如果进程已在运行，检查是否为本程序实例*/
	sprintf(status_file, "/proc/%d/status", pid);
	
	file = fopen(status_file, "r+");
    while(file && !feof(file)){
        memset(fbuf, 0, sizeof(fbuf));
        if(fgets(fbuf, sizeof(fbuf), file) == 0)
            break;
        if (fbuf[sizeof(fbuf)-1] != 0)
            continue;
        if((line = strstr(fbuf, "Name:")) == NULL)
            continue;

        line += strlen("Name:");
		while(' ' == *line)
			++line;
		
        sscanf(line, "%s", proc_name);
        break;
	}
    if (file)
        fclose(file);

	if(strcmp(proc_name, "aglink")) /*不是本程序实例*/
		return FALSE;

	return TRUE;
}


/*****************************************************************************
 Prototype    : ag_module_write_pidfile
 Description  : write pid to file
 Input        : s8 *pidfile  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/11/17
    Author       : lvjianlin
    Modification : Created function

*****************************************************************************/
s32 ag_module_write_pidfile(s8 *pidfile)
{
	FILE *file;
	s32 pid = 0;
	s32 fd = 0;

	if(NULL == pidfile)
		return FALSE;

	if((fd = open(pidfile, O_RDWR | O_CREAT | O_TRUNC, 0644)) == -1){ 
		AG_MODULE_LOG(LOG_ERR, "Error, open stream %s", pidfile);
		return FALSE;
	}
	if((file = fdopen(fd, "r+")) == NULL){ 
		AG_MODULE_LOG(LOG_ERR, "Error, open file %s", pidfile);
		return FALSE;
	}

	if(flock(fd, LOCK_EX|LOCK_NB) == -1){
		AG_MODULE_LOG(LOG_ERR, "Error, flock %s", pidfile);
		fclose(file);
		return FALSE;
	}

	pid = getpid();
	if(!fprintf(file, "%d\n", pid)) {
		AG_MODULE_LOG(LOG_ERR, "Error, fprintf %s", pidfile);
		fclose(file);
		return FALSE;
	}
	fflush(file);

	if(flock(fd, LOCK_UN) == -1) {
		AG_MODULE_LOG(LOG_ERR, "Error, flock %s", pidfile);
		fclose(file);
		return FALSE;
	}
	fclose(file);

	return TRUE;
}


u32 ag_module_find_module_info(u8 *pname, AG_MODULE_SETUP *pstModule)
{
	s32 iNum = 0;
	s32 iLoop = 0;
	s32 iRet = 0;
	
	iNum = sizeof(astAglinkModule)/sizeof(astAglinkModule[0]);
	AG_MODULE_LOG(LOG_INFO, "we will find %s, resigter module number is:%d", pname, iNum);

	for(iLoop = 0; iLoop< iNum; iLoop++)
	{
		if(!memcmp(astAglinkModule[iLoop].acname, pname, strlen(pname)))
		{
			pstModule = &astAglinkModule[iLoop];
			AG_MODULE_LOG(LOG_INFO, "GET %s info success\n", (AG_MODULE_SETUP *)pstModule->acname);
			return TRUE;
		}
	}

	AG_MODULE_LOG(LOG_ERR, "Can`t find %s info", pname);
	return FALSE;
}

/*****************************************************************************
 Prototype    : ag_module_init_primary
 Description  : every module init
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/11/17
    Author       : lvjianlin
    Modification : Created function

*****************************************************************************/
s32 ag_module_init_primary()
{
	s32 iNum = 0;
	s32 iLoop = 0;
	s32 iRet = 0;
	
	iNum = sizeof(astAglinkModule)/sizeof(astAglinkModule[0]);
	AG_MODULE_LOG(LOG_INFO, "resigter module number is:%d", iNum);

	for(iLoop = 0; iLoop< iNum; iLoop++)
	{
		if(NULL != astAglinkModule[iLoop].aginit)
		{
			iRet = ((AG_MODULE_INIT)(astAglinkModule[iLoop].aginit))((u8 *)&astAglinkModule[iLoop]);
			if(iRet != TRUE)
			{
				AG_MODULE_LOG(LOG_ERR, "Module:%s init faile", astAglinkModule[iLoop].acname);
				return FALSE;
			}
		}
		else
		{
			AG_MODULE_LOG(LOG_ERR, "Function point is null!");
		}
	}
	return TRUE;
}

s32 ag_module_control_id_init_primary()
{
	s32 iNum = 0;
	s32 iLoop = 0;
	s32 iRet = 0;
	
	iNum = sizeof(astAglinkModule)/sizeof(astAglinkModule[0]);
	AG_MODULE_LOG(LOG_INFO, "resigter module number is:%d", iNum);

	for(iLoop = 0; iLoop< iNum; iLoop++)
	{
		if(NULL != astAglinkModule[iLoop].agcontralinit)
		{
			iRet = ((AG_MODULE_CONTROL_INIT)(astAglinkModule[iLoop].agcontralinit))((u8 *)&astAglinkModule[iLoop]);
			if(iRet != TRUE)
			{
				AG_MODULE_LOG(LOG_ERR, "Module:%s config init faile", astAglinkModule[iLoop].acname);
				return FALSE;
			}
		}
		else
		{
			AG_MODULE_LOG(LOG_ERR, "Function point is null!");
		}
	}
	
	return TRUE;
}

s32 ag_module_business_id_init_primary()
{
	s32 iNum = 0;
	s32 iLoop = 0;
	s32 iRet = 0;
	
	iNum = sizeof(astAglinkModule)/sizeof(astAglinkModule[0]);
	AG_MODULE_LOG(LOG_INFO, "resigter module number is:%d", iNum);

	for(iLoop = 0; iLoop< iNum; iLoop++)
	{
		if(NULL != astAglinkModule[iLoop].agbussinessinit)
		{
			iRet = ((AG_MODULE_BUSINESS_INIT)(astAglinkModule[iLoop].agbussinessinit))((u8 *)&astAglinkModule[iLoop]);
			if(iRet != TRUE)
			{
				AG_MODULE_LOG(LOG_ERR, "Module:%s config init faile", astAglinkModule[iLoop].acname);
				return FALSE;
			}
		}
		else
		{
			AG_MODULE_LOG(LOG_ERR, "Function point is null!");
		}
	}
	
	return TRUE;
}

s32 ag_module_event_init_primary()
{
	s32 iNum = 0;
	s32 iLoop = 0;
	s32 iRet = 0;
	
	iNum = sizeof(astAglinkModule)/sizeof(astAglinkModule[0]);
	AG_MODULE_LOG(LOG_INFO, "resigter module number is:%d", iNum);

	for(iLoop = 0; iLoop< iNum; iLoop++)
	{
		if(NULL != astAglinkModule[iLoop].ageventinit)
		{
			iRet = ((AG_MODULE_EVENT_INIT)(astAglinkModule[iLoop].ageventinit))((u8 *)&astAglinkModule[iLoop]);
			if(iRet != TRUE)
			{
				AG_MODULE_LOG(LOG_ERR, "Module:%s config init faile", astAglinkModule[iLoop].acname);
				return FALSE;
			}
		}
		else
		{
			AG_MODULE_LOG(LOG_ERR, "Function point is null!");
		}
	}
	
	return TRUE;
}


#if 0
s32 ag_module_ipc_get_server_ip_port(u32 *uipcipaddr, u8 *usipcport, u32 *uiSeripaddr, u8 *usSerport, u8 *pacName)
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
	FILE *pfile;
	u8 ucBuffer[256] = "";
	u8 ucName[32] = "";
	u8 ucIpAddr[32] = "";
	u8 ucPort[32] = "";
	
	pfile = fopen(IPC_CONFIG_FILE, "r");
	if(NULL == pfile)
	{
		AG_MODULE_LOG(LOG_ERR, "fopen "IPC_CONFIG_FILE" error, %s(errno: %d)", strerror(errno), errno);
		return FALSE;
	}

	AG_MODULE_LOG(LOG_ERR, "pacName:%s", pacName);
	
	while(fgets(ucBuffer, 256, pfile))
	{
		if(NULL != strstr(ucBuffer, "aglink"))
		{
			sscanf(ucBuffer, "%s %s %s",ucName, ucIpAddr, ucPort);
			AG_MODULE_LOG(LOG_ERR, "name:%s IpAddr:%s Port:%s", ucName, ucIpAddr, ucPort);
			inet_pton(AF_INET, ucIpAddr, uiSeripaddr);
			usSerport = atoi(ucPort);
		}
		
		/* 查找字符串 */
		if(NULL != strstr(ucBuffer, pacName))
		{
			sscanf(ucBuffer, "%s %s %s",ucName, ucIpAddr, ucPort);
			AG_MODULE_LOG(LOG_ERR, "name:%s IpAddr:%s Port:%s", ucName, ucIpAddr, ucPort);
			inet_pton(AF_INET, ucIpAddr, uipcipaddr);
			usipcport = atoi(ucPort);
			break;
		}
		
	}

	fclose(pfile);
	
	AG_MODULE_LOG(LOG_ERR, "IPC Module ip: "IP_FMT" port:%d IPC server ip:"IP_FMT" port:%d ", 
		IP_ARG(uipcipaddr),  usipcport, IP_ARG(uiSeripaddr), usSerport);
	
	return TRUE;
}
s32 ag_module_ipc_connect_to_server(u32 uipcipaddr, u8 usipcport, u32 uiSeripaddr, u8 usSeripcport, AG_MODULE_IPC_REGISTER *pstIpcRegister)
{
	
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
		
	if(bind(sockfd, (struct sockaddr *)&pstIpcRegister->stipcskaddr, sizeof(pstIpcRegister->stipcskaddr)) == -1)
	{
		AG_MODULE_LOG(LOG_ERR, "socket bind error.");
		close(sockfd);
		exit(-1);
	}
	
	/*设置server fd发送超时为timeval*/
	setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(struct timeval));
	/*设置server fd接收超时为timeval*/
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));

	pstIpcRegister->uipcSocket = sockfd;

	AG_MODULE_LOG(LOG_WARNING, "IPC socket success!");
	return TRUE;
}
#endif 

u32 ag_module_ipc_name_hash(const u8 *pstr)
{
	u32 hash = 0;
	while(*pstr)
	{
	    hash = (*pstr++) + (hash << 6) + (hash << 16) -hash;
	}
	return (hash &0x7fffffff);
}

struct hlist_head *ag_module_ipc_hash_fn(const u8 *ucurl)
{
	  u32 hash = ag_module_ipc_name_hash(ucurl);
	  return &(gstIPCHashList[hash & (AG_MODULE_HASH_LIST_SIZE - 1)]);
}
	

AG_MODULE_IPC_MODULE_LIST_T *ag_module_ipc_find_by_head(u8 *pucName)
{
	AG_MODULE_IPC_MODULE_LIST_T *pstModule = NULL;
	struct hlist_node *pos = NULL;

	if(pucName == NULL)
	{
		return NULL;
	}
	
	hlist_for_each(pos, ag_module_ipc_hash_fn(pucName))
	{
		pstModule = hlist_entry(pos, AG_MODULE_IPC_MODULE_LIST_T, stHeadHlist);
		if (!memcmp(pstModule->acname, pucName, strlen(pucName)))
		{
			return pstModule; 
		}
	}
    
	return NULL;
}


s32 ag_module_ipc_list_add(u8 * pucData)
{
	AG_MODULE_IPC_REGISTER *psttmp;
	AG_MODULE_IPC_MODULE_LIST_T *pstIPCModule;
	struct hlist_head *head = NULL;
	AG_MODULE_IPC_MODULE_LIST_T *pos = NULL;
	
	psttmp = (AG_MODULE_IPC_REGISTER *)pucData;

	if(psttmp->acname == NULL)
	{
		AG_MODULE_LOG(LOG_ERR, "Module name is NULL!");
		return FALSE;
	}

	AG_MODULE_LOG(LOG_INFO, "Module name is:%s moduleId is:%d", psttmp->acname, psttmp->uiModuleId);


	/* output every module info */
	list_for_each_entry(pos, &gstIPClist, stModuleList)
	{
		AG_MODULE_LOG(LOG_INFO, "Module name:%s moduleId:%d ", pos->acname, pos->uiModuleId);
	}


	//AG_MSG_LOCK();
	pstIPCModule = ag_module_ipc_find_by_head(psttmp->acname);

	if(pstIPCModule)
	{
		AG_MODULE_LOG(LOG_INFO, "Module already register, Module name is:%s moduleId is:%d", pstIPCModule->acname, pstIPCModule->uiModuleId);
		if(psttmp->uiModuleId != pstIPCModule->uiModuleId)
		{
			pstIPCModule->uiModuleId = psttmp->uiModuleId;
		}
		
		return TRUE;
	}
	//AG_MSG_UNLOCK();
	
	
	/* malloc  */
	pstIPCModule = (AG_MODULE_IPC_MODULE_LIST_T *)malloc(sizeof(AG_MODULE_IPC_MODULE_LIST_T));
	if( NULL == pstIPCModule)
	{
		AG_MODULE_LOG(LOG_ERR, "malloc fail!");
		return FALSE;
	}

	memset(pstIPCModule, 0, sizeof(AG_MODULE_IPC_MODULE_LIST_T));
	memcpy(pstIPCModule->acname, psttmp->acname, strlen(psttmp->acname));
	pstIPCModule->uiModuleId = psttmp->uiModuleId;
	memcpy(&pstIPCModule->stipcskaddr, &psttmp->stipcskaddr, sizeof(struct sockaddr_in));
	memcpy(&pstIPCModule->stSerskaddr, &psttmp->stSerskaddr, sizeof(struct sockaddr_in));
	AG_MODULE_LOG(LOG_INFO, "Add list, Module name:%s uiModuleId:%d ", pstIPCModule->acname, pstIPCModule->uiModuleId);

	AG_MSG_LOCK();
	head = ag_module_ipc_hash_fn(pstIPCModule->acname);
    	hlist_add_head(&(pstIPCModule->stHeadHlist), head);
    	list_add(&(pstIPCModule->stModuleList), &gstIPClist);
    	gulIPCListNum++;
	AG_MSG_UNLOCK();
	return TRUE;
}

#if 0
s32 ag_module_ipc_client_send_msg(u16 uiMsgID, u32 uiMsgType, AG_MODULE_IPC_REGISTER *pstIpcRegister, u8 *pbuff, int iMsgLen)
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

	ENCODE_PUT(pnt, &pstMsgHead, sizeof(AG_LINK_MSG_HEAD_T));

	if(iMsgLen > 0)
	{
		ENCODE_PUT(pnt, pbuff, iMsgLen);
	}

	if( sendto(pstIpcRegister->uipcSocket, ucSendbuf, uitotallen, 0, 
			(struct sockaddr *)&pstIpcRegister->stSerskaddr, sizeof(pstIpcRegister->stSerskaddr)) < 0)
	{
		AG_MODULE_LOG(LOG_ERR, "send msg error: %s(errno: %d)", strerror(errno), errno);
		return FALSE;
	}
	
	return TRUE;
}
#endif

u32 ag_module_ipc_server_send_msg_to_internal(AG_LINK_MSG_HEAD_T *pstMsgHead, u8 *pstBuffer, int iTotallen, struct sockaddr_in *sk_addr)
{
	AG_MODULE_IPC_MODULE_LIST_T *pstIPCModule;
	AG_MODULE_IPC_MODULE_LIST_T *pos = NULL;	
	AG_LINK_MSG_HEAD_T stMsgHead = {0};
	u8 ucSendbuf[AG_MODULE_BUFFER_SIZE] = {0};
	u32 uiMsgid;

	if(NULL == pstMsgHead || NULL == pstBuffer)
	{
		AG_MODULE_LOG(LOG_INFO, "param is NULL");
		return FALSE;
	}
	//pstIPCModule = ag_module_ipc_find_by_head(pstMsgHead->acname);

	AG_MODULE_LOG(LOG_INFO, "Msghead module name:%s MsgId:%d", pstMsgHead->acname, pstMsgHead->usMsgId);

	//if(NULL == sk_addr)
	{
		/* 消息ID 是否在ipcset中 ,模块类组播发送*/
		//if((pstMsgHead->usMsgId |gulIPCIdSet) == pstMsgHead->usMsgId)
		if(pstMsgHead->usMsgId < BC_MSG_MAX)
		{
			list_for_each_entry(pos, &gstIPClist, stModuleList)
			{
				AG_MODULE_LOG(LOG_INFO, "BC Socket:%d Dest ip:"IP_FMT" port:%d", gstAglinkIPCmgt.uipcSocket,
									IP_ARG(&(pos->stipcskaddr.sin_addr.s_addr)), ntohs(pos->stipcskaddr.sin_port));
						
				if(sendto(gstAglinkIPCmgt.uipcSocket, pstBuffer, iTotallen, 0, &pos->stipcskaddr, sizeof(struct sockaddr_in)) <0 )
				{
					AG_MODULE_LOG(LOG_ERR, "Send msg error:%s (errno:%d)", strerror(errno), errno);
					return FALSE;
				}
			}

			return TRUE;
		}
		//else if((AG_IPC_MSG_BEGIN < pstMsgHead->usMsgId)&&(pstMsgHead->usMsgId < AG_IPC_MSG_MAX))
		else if((AG_MSG_IPC_BEGIN < pstMsgHead->usMsgId)&&(pstMsgHead->usMsgId < AG_MSG_IPC_MAX))
		{
			
			list_for_each_entry(pos, &gstIPClist, stModuleList)
			{
				AG_MODULE_LOG(LOG_INFO, "IDSet Module name:%s(strlen:%d) uiModuleId:%d  Dest ip:"IP_FMT" port:%d", 
					pos->acname, strlen(pos->acname), pos->uiModuleId, IP_ARG(&(pos->stipcskaddr.sin_addr.s_addr)), ntohs(pos->stipcskaddr.sin_port));
			
				if(0 == memcmp(pstMsgHead->acname, pos->acname, strlen(pos->acname)))
				{	
					AG_MODULE_LOG(LOG_INFO, "Find this module IPC idset");
					goto SENDMSG;
				}
			}
			
			AG_MODULE_LOG(LOG_ALERT, "Can`t find IPC info");
			return FALSE;
		}
		else
		{
			/* 依据消息id查找对应模块 */
			list_for_each_entry(pos, &gstIPClist, stModuleList)
			{
				AG_MODULE_LOG(LOG_INFO, "Module name:%s uiModuleId:%d Dest ip:"IP_FMT" port:%d", 
					pos->acname, pos->uiModuleId, IP_ARG(&(pos->stipcskaddr.sin_addr.s_addr)), ntohs(pos->stipcskaddr.sin_port));
				//if(((0X0000FFFF&pstMsgHead->usMsgId) |pos->uiIdSet) == pos->uiIdSet)
				uiMsgid = pstMsgHead->usMsgId;
				if(AG_GET_MODULE_ID(uiMsgid) == (pos->uiModuleId))
				{
					AG_MODULE_LOG(LOG_INFO, "Find this module");
					goto SENDMSG;
				}
			}
			
			AG_MODULE_LOG(LOG_ALERT, "Can`t find IPC info, Module id:%d", AG_GET_MODULE_ID(uiMsgid));
			goto SEND_UNREGISTER;
			return FALSE;
		}

	SENDMSG:
		AG_MODULE_LOG(LOG_INFO, "Socket:%d Dest ip:"IP_FMT" port:%d", gstAglinkIPCmgt.uipcSocket,
					IP_ARG(&(pos->stipcskaddr.sin_addr.s_addr)), ntohs(pos->stipcskaddr.sin_port));
		
		if(sendto(gstAglinkIPCmgt.uipcSocket, pstBuffer, iTotallen, 0, &pos->stipcskaddr, sizeof(struct sockaddr_in)) <0 )
		{
			AG_MODULE_LOG(LOG_ERR, "Send msg error:%s (errno:%d)", strerror(errno), errno);
			return FALSE;
		}

		return TRUE;
	}


	SEND_UNREGISTER:
	{
		/*Can`t find module info through uiMsgid*/
		u32 uipcipaddr;
		u16 usipcport;
		u32 uiServeraddr;
		u16 usServerPort;
		u8 *p = ucSendbuf;
		u8 **pnt = &p;
		struct sockaddr_in  st_addr = {0};
		
		AG_MODULE_LOG(LOG_INFO, "Can`t  find module:%s(%d)", gucModuleorder[AG_GET_MODULE_ID(uiMsgid)], AG_GET_MODULE_ID(uiMsgid));
		ag_module_ipc_get_server_ip_port(&uipcipaddr, &usipcport, &uiServeraddr, &usServerPort, gucModuleorder[AG_GET_MODULE_ID(uiMsgid)]);

		st_addr.sin_family = AF_INET;
		st_addr.sin_port = htons(usipcport);
		st_addr.sin_addr.s_addr = uipcipaddr;

		AG_MODULE_LOG(LOG_INFO, "Module name:%s uiModuleId:%d Dest ip:"IP_FMT" port:%d", 
			gucModuleorder[AG_GET_MODULE_ID(uiMsgid)], uiMsgid, IP_ARG(&(st_addr.sin_addr.s_addr)), ntohs(st_addr.sin_port));

		memset(&stMsgHead, 0, sizeof(AG_LINK_MSG_HEAD_T));
		stMsgHead.usMsgId = AG_MSG_IPC_UNREGISTER;
		stMsgHead.usMsgType = AG_IPC_MSG_TYPE_SELF_CPU;
		memcpy(stMsgHead.acname, "aglink", strlen("aglink"));
		stMsgHead.uiMsgLen = 0;

		ENCODE_PUTL(pnt, stMsgHead.usMsgId);
		ENCODE_PUTW(pnt, stMsgHead.usMsgType);
		ENCODE_PUTW(pnt, stMsgHead.uiUnicast);
		ENCODE_PUT(pnt,stMsgHead.acname, MODULE_NAME_LENGTH);
		ENCODE_PUTL(pnt, stMsgHead.uiMsgLen);
		
		if(sendto(gstAglinkIPCmgt.uipcSocket, ucSendbuf, sizeof(AG_LINK_MSG_HEAD_T), 0, &st_addr, sizeof(struct sockaddr_in)) <0 )
		{
			AG_MODULE_LOG(LOG_ERR, "Send msg error:%s (errno:%d)", strerror(errno), errno);
			return FALSE;
		}

		return TRUE;
	}
	#if 0
	if(pstIPCModule)
	{
		AG_MODULE_LOG(LOG_INFO, "Module name is:%s, IdSet is:%d MsgId is:%d itotallen:%d ", pstIPCModule->acname, pstIPCModule->uiIdSet, pstMsgHead->usMsgId, iTotallen);
		//查找IP是否在ipset中
		if(((pstMsgHead->usMsgId |pstIPCModule->uiIdSet) != pstMsgHead->usMsgId)&&\
			((pstMsgHead->usMsgId |gulIPCIdSet) != pstMsgHead->usMsgId))
		{
			AG_MODULE_LOG(LOG_ALERT, "MsgId is(%d) is not in IdSet", pstMsgHead->usMsgId);			
			return FALSE;
		}

		AG_MODULE_LOG(LOG_INFO, "Socket:%d  Dest ip:"IP_FMT" port:%d", gstAglinkIPCmgt.uipcSocket,
			IP_ARG(&(pstIPCModule->stipcskaddr.sin_addr.s_addr)), ntohs(pstIPCModule->stipcskaddr.sin_port));

		if(sendto(gstAglinkIPCmgt.uipcSocket, pstBuffer, iTotallen, 0, &pstIPCModule->stipcskaddr, sizeof(struct sockaddr_in)) <0 )
		{
			AG_MODULE_LOG(LOG_ERR, "Send msg error:%s (errno:%d)", strerror(errno), errno);
			return FALSE;
		}
		
		return TRUE;
	}
	AG_MODULE_LOG(LOG_INFO, "Can`t find IPC info");
	return FALSE;
	#endif
}

u32 ag_module_ipc_server_send_msg_to_external(AG_LINK_MSG_HEAD_T *pstMsgHead, u8* pstBuffer, u32 iTotallen, u8 *uMac)
{
	u32 iRet = 0;
	
	iRet = aglink_send_ag_msg(NULL, NULL, pstBuffer, iTotallen, uMac);
	if(TRUE == iRet)
	{
		AG_MODULE_LOG(LOG_INFO, "Send msg success!");
	}
	else
	{
		AG_MODULE_LOG(LOG_INFO, "Send msg fail!");
	}
	return TRUE;
}
		
/*****************************************************************************
 Prototype    : ag_module_ipc_server_send_msg
 Description  : 
 Input        : u16 uiMsgID         
                u32 uiMsgType       
                u8 *pstModuleName  	//if uiMsgId & pstModuleName is  NULL,  the msg is complete , send to GW direct,      if not NULL, encoding the msg head ,then send    
                u8 *pbuff           
                int iMsgLen         
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/11/29
    Author       : lvjianlin
    Modification : Created function

*****************************************************************************/
u32 ag_module_ipc_server_send_msg(u32 uiMsgID, u16 uiMsgType, u8 *pstModuleName, u8 *pbuff, int iMsgLen, u8 *uMac)
{
	u8 ucSendbuf[AG_MODULE_BUFFER_SIZE] = {0};
	u8 *p = ucSendbuf;
	u8 **pnt = &p;
	u32 uitotallen = 0;
	AG_LINK_MSG_HEAD_T pstMsgHead;

	AG_MODULE_LOG(LOG_INFO, "begin MsgLength:%d Msgtype:%d", iMsgLen, uiMsgType);

	memset(&pstMsgHead, 0, sizeof(AG_LINK_MSG_HEAD_T));
	
	if((NULL != uiMsgID)&&(NULL != pstModuleName))
	{
		AG_MODULE_LOG(LOG_INFO, "Msgid:%d MsgType:%d, ModuleName:%s.", uiMsgID, uiMsgType, pstModuleName);
		uitotallen += sizeof(AG_LINK_MSG_HEAD_T);
		pstMsgHead.usMsgId = uiMsgID;
		pstMsgHead.usMsgType = uiMsgType;
		memcpy(pstMsgHead.acname, pstModuleName, strlen(pstModuleName));
		pstMsgHead.uiMsgLen = iMsgLen;
		//ENCODE_PUT(pnt, &pstMsgHead, sizeof(AG_LINK_MSG_HEAD_T));
		ENCODE_PUTL(pnt, pstMsgHead.usMsgId);
		ENCODE_PUTW(pnt, pstMsgHead.usMsgType);
		ENCODE_PUTW(pnt, pstMsgHead.uiUnicast);
		ENCODE_PUT(pnt,pstMsgHead.acname, MODULE_NAME_LENGTH);
		ENCODE_PUTL(pnt, pstMsgHead.uiMsgLen);
	}

	uitotallen += iMsgLen;
	if(uitotallen > AG_MODULE_BUFFER_SIZE)

	{
		AG_MODULE_LOG(LOG_INFO, "Send msg length is %d, bigger than  AG_MODULE_BUFFER_SIZE",uitotallen);
		return FALSE;
	}
	
	if(iMsgLen > 0)
	{
		ENCODE_PUT(pnt, pbuff, iMsgLen);		
	}

	if(AG_IPC_MSG_TYPE_SELF_CPU == uiMsgType)
	{
		
		ag_module_ipc_server_send_msg_to_internal(&pstMsgHead, ucSendbuf, uitotallen, NULL);
		
	}
	else if (AG_IPC_MSG_TYPE_OTHER_CPU == uiMsgType)
	{
		ag_module_ipc_server_send_msg_to_external(&pstMsgHead, ucSendbuf, uitotallen, uMac);
		
	}
	else
	{
		AG_MODULE_LOG(LOG_INFO, "Error MsgType:%d", uiMsgType);
		return FALSE;
	}

	return TRUE;
}
/*****************************************************************************
 Prototype    : ag_module_ipc_server_parse_msg
 Description  : 版内消息/版间消息分发
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/11/29
    Author       : lvjianlin
    Modification : Created function

*****************************************************************************/
u32 ag_module_ipc_server_parse_msg(u8 *buf, u32 len, struct sockaddr_in *sk_addr)
{
	u8 *p = buf;
	u8 **pnt = &p;
	u32 iRet;
	AG_LINK_MSG_HEAD_T pstMsgHead;
	AG_MODULE_IPC_REGISTER pstIPCreg;

	if(len <= 0)
	{
		AG_MODULE_LOG(LOG_ERR, "Msg length <= 0");
		return FALSE;
	}
	memset(&pstMsgHead, 0, sizeof(AG_LINK_MSG_HEAD_T));
	//DECODE_GET(&pstMsgHead, pnt, sizeof(AG_LINK_MSG_HEAD_T));
	DECODE_GETL(pnt, pstMsgHead.usMsgId);
	DECODE_GETW(pnt, pstMsgHead.usMsgType);
	DECODE_GETW(pnt, pstMsgHead.uiUnicast);
	DECODE_GET(pstMsgHead.acname, pnt, MODULE_NAME_LENGTH);
	DECODE_GETL(pnt, pstMsgHead.uiMsgLen);
	
	AG_MODULE_LOG(LOG_INFO, "acname:%s usMsgId:%d usMsgType:%d uiMsgLen:%d  uiUnicast:%d msg total len:%d, sizeof(AG_LINK_MSG_HEAD_T):%d", 
		pstMsgHead.acname, pstMsgHead.usMsgId, pstMsgHead.usMsgType, pstMsgHead.uiMsgLen, pstMsgHead.uiUnicast, len, sizeof(AG_LINK_MSG_HEAD_T));
	
	/* 板内消息 */
	if(AG_IPC_MSG_TYPE_SELF_CPU == pstMsgHead.usMsgType)
	{
		/* module register info */
		if(AG_MSG_IPC_REGISTER == pstMsgHead.usMsgId)
		{
			if(len != (sizeof(AG_MODULE_IPC_REGISTER)+sizeof(AG_LINK_MSG_HEAD_T)))
			{
				AG_MODULE_LOG(LOG_ERR, "The message length dismissed, return;");
				return FALSE;
			}
			memset(&pstIPCreg, 0, sizeof(AG_MODULE_IPC_REGISTER));
			DECODE_GET(&pstIPCreg, pnt, sizeof(AG_MODULE_IPC_REGISTER));
			iRet = ag_module_ipc_list_add((u8 *)&pstIPCreg);
			if(TRUE == iRet)
			{
				/* send REGISTER ACK msg */
				ag_module_ipc_server_send_msg(AG_MSG_IPC_REGISTER_ACK, AG_IPC_MSG_TYPE_SELF_CPU, pstIPCreg.acname, NULL, 0, NULL);
			}
			else
			{
				AG_MODULE_LOG(LOG_INFO, "IPC list add failed!");
				return FALSE;
			}
		}
		else if((AGLINK_CLI_BEGIN < pstMsgHead.usMsgId)  && (pstMsgHead.usMsgId <AGLINK_CLI_MAX))
		{
			aglink_parse_agcli_msg(buf, len, NULL);
		}
		else if(AG_GET_MODULE_ID(pstMsgHead.usMsgId) == AG_GET_MODULE_ID(AGLINK_MSG_BEGIN))
		{
			aglink_parse_ag_msg(buf, len, NULL);
		}
		/* 其它版内消息，发送给各个模块 */
		else
		{
			ag_module_ipc_server_send_msg(pstMsgHead.usMsgId, AG_IPC_MSG_TYPE_SELF_CPU, pstMsgHead.acname, \
				*pnt, (len-sizeof(AG_LINK_MSG_HEAD_T)), NULL);
		}
	}
	/* 跨版并指定单播发送 */
	else if((1 == pstMsgHead.uiUnicast)&&(AG_IPC_MSG_TYPE_OTHER_CPU == pstMsgHead.usMsgType))
	{
		AG_LINK_MSG_UNICAST_HEAD_T stUnicastHead;
		memset(&stUnicastHead, 0, sizeof(AG_LINK_MSG_UNICAST_HEAD_T));
		DECODE_GET(stUnicastHead.ucMac, pnt, sizeof(AG_LINK_MSG_UNICAST_HEAD_T));
		AG_MODULE_LOG(LOG_INFO, "Unicast dest:"MAC_FMT, MAC_ARG(stUnicastHead.ucMac));
		ag_module_ipc_server_send_msg(pstMsgHead.usMsgId, AG_IPC_MSG_TYPE_OTHER_CPU, pstMsgHead.acname, \
				*pnt, (len-sizeof(AG_LINK_MSG_HEAD_T)-sizeof(AG_LINK_MSG_UNICAST_HEAD_T)), stUnicastHead.ucMac);
	}
	/* 板间消息 由aglink主线程发送*/
	else if (AG_IPC_MSG_TYPE_OTHER_CPU == pstMsgHead.usMsgType)
	{
		ag_module_ipc_server_send_msg(NULL, AG_IPC_MSG_TYPE_OTHER_CPU, NULL, buf, len, NULL);
	}
	return TRUE;
}

/*****************************************************************************
 Prototype    : ag_module_ipc_check_legality
 Description  : 判断消息发送模块是否合法
 Input        : u8 *buf                      
                u32 len                      
                struct sockaddr_in *sk_addr  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/12/11
    Author       : lvjianlin
    Modification : Created function

*****************************************************************************/
u32 ag_module_ipc_check_legality(u8 *buf, u32 len, struct sockaddr_in *sk_addr)
{
	AG_MODULE_IPC_MODULE_LIST_T *pos = NULL;
	u8 *p = buf;
	u8 **pnt = &p;
	u32 iRet;
	AG_LINK_MSG_HEAD_T pstMsgHead;
	AG_MODULE_IPC_REGISTER pstIPCreg;
	u8 ucSendbuf[AG_MODULE_BUFFER_SIZE] = {0};

	if(len <= 0)
	{
		AG_MODULE_LOG(LOG_ERR, "Msg length <= 0");
		return FALSE;
	}
	memset(&pstMsgHead, 0, sizeof(AG_LINK_MSG_HEAD_T));
	//DECODE_GET(&pstMsgHead, pnt, sizeof(AG_LINK_MSG_HEAD_T));
	DECODE_GETL(pnt, pstMsgHead.usMsgId);
	DECODE_GETW(pnt, pstMsgHead.usMsgType);
	DECODE_GETW(pnt, pstMsgHead.uiUnicast);
	DECODE_GET(pstMsgHead.acname, pnt, MODULE_NAME_LENGTH);
	DECODE_GETL(pnt, pstMsgHead.uiMsgLen);
	
	AG_MODULE_LOG(LOG_INFO, "Recv ip:"IP_FMT" port:%d acname:%s usMsgId:%d usMsgType:%d uiMsgLen:%d msg total len:%d", \
		IP_ARG(&(sk_addr->sin_addr.s_addr)), ntohs(sk_addr->sin_port),\
		pstMsgHead.acname, pstMsgHead.usMsgId, pstMsgHead.usMsgType, pstMsgHead.uiMsgLen, len);

	/* 板内消息 */
	if(AG_IPC_MSG_TYPE_SELF_CPU == pstMsgHead.usMsgType)
	{
		if(pstMsgHead.usMsgId > AG_MSG_IPC_MAX)
		{
			/* 依据消息id查找对应模块 */
			list_for_each_entry(pos, &gstIPClist, stModuleList)
			{
				AG_MODULE_LOG(LOG_INFO, "Module name:%s uiModuleId:%d Dest ip:"IP_FMT" port:%d", 
					pos->acname, pos->uiModuleId, IP_ARG(&(pos->stipcskaddr.sin_addr.s_addr)), ntohs(pos->stipcskaddr.sin_port));
				
				if(0 == memcmp(pstMsgHead.acname, pos->acname, strlen(pos->acname)))
				{
					AG_MODULE_LOG(LOG_INFO, "Find this module");
					return TRUE;
				}
			}
			
			AG_MODULE_LOG(LOG_ALERT, "Can`t find IPC info");
			
			memset(&pstMsgHead, 0, sizeof(AG_LINK_MSG_HEAD_T));
			pstMsgHead.usMsgId = AG_MSG_IPC_UNREGISTER;
			pstMsgHead.usMsgType = AG_IPC_MSG_TYPE_SELF_CPU;
			memcpy(pstMsgHead.acname, "aglink", strlen("aglink"));
			pstMsgHead.uiMsgLen = 0;
			memcpy(ucSendbuf, &pstMsgHead, sizeof(AG_LINK_MSG_HEAD_T));
			ag_module_ipc_server_send_msg_to_internal( &pstMsgHead, ucSendbuf, sizeof(AG_LINK_MSG_HEAD_T), sk_addr);
			return FALSE;
		}
	}
	
	return TRUE;
}
u32 ag_module_ipc_server_recv_msg(evutil_socket_t fd, short event, void *arg)
{
	s32 size, iRet;
	s32 sockfd = gstAglinkIPCmgt.uipcSocket;
	u8 ucRecvbuf[AG_MODULE_BUFFER_SIZE] = {0};
	struct sockaddr_in sk_addr;
	socklen_t len = sizeof(sk_addr);

	AG_MODULE_LOG(LOG_INFO, "fd:%d sockfd:%d", fd, sockfd);
	size = recvfrom(sockfd, (void *)ucRecvbuf, AG_MODULE_BUFFER_SIZE, 0, (struct sockaddr *)&sk_addr, &len);
	if(-1 == size)
	{
		AG_MODULE_LOG(LOG_ERR, "receive data error!\n");
		return FALSE;
	}
	else
	{	/*check module wether legal*/
		#if 0
		if(FALSE == ag_module_ipc_check_legality(ucRecvbuf, size, &sk_addr))
		{
			AG_MODULE_LOG(LOG_ERR, "module inlegality");
			return TRUE;
		}
		#endif
		ag_module_ipc_server_parse_msg(ucRecvbuf, size, &sk_addr);
	}
	
	return TRUE;;
}

#if 0
s32 ag_module_ipc_connect_server(s32 fd, short event, void *arg)
{
	AG_MODULE_IPC_REGISTER *pstIpcRegister;
	u8 buf[AG_MODULE_BUFFER_SIZE] = {0};

	pstIpcRegister = (AG_MODULE_IPC_REGISTER *)arg;

	AG_MODULE_LOG(LOG_INFO, "uiFlag is %d", pstIpcRegister->uiFlag);

	/* 连接未成功 */
	if(!pstIpcRegister->uiFlag)
	{
		memcpy(buf, pstIpcRegister, sizeof(AG_MODULE_IPC_REGISTER));
		AG_MODULE_LOG(LOG_INFO, "Register module :%s , ID set:%d ", pstIpcRegister->acname, pstIpcRegister->uiIdSet);	
		/* send registre info */
		ag_module_ipc_client_send_msg(AG_IPC_MSG_REGISTER, AG_IPC_MSG_TYPE_SELF_CPU, pstIpcRegister, buf, sizeof(AG_MODULE_IPC_REGISTER));
		event_add(&pstIpcRegister->connect_ipc_server_ev, &pstIpcRegister->connect_ipc_server_tv);
	}
	
	
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

	}
	
	/* aglink and !aglink pthread */
	if(pRecv_fun != NULL)
	{
		/*消息接收回调*/
		event_set(&pstIpcRegister->recv_ev, pstIpcRegister->uipcSocket, EV_READ | EV_PERSIST, 
			pRecv_fun, (void *)&pstIpcRegister->recv_ev);
	
		/*开启消息接收*/
		event_add(&pstIpcRegister->recv_ev, NULL);
	}

	return TRUE;
}

s32 ag_module_ipc_module_init(u8 *pacName, u32 uiIdSet, AG_MODULE_IPC_REGISTER *pstIpcRegister,  IPC_CLIENT_RECV_EVENT *pRecv_fun)
{
	u32 uipcipaddr;
	u8 usipcport;
	u32 uiServeraddr;
	u8 usServerPort;
	
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

	if(NULL == memcpy(pstIpcRegister->acname, pacName, sizeof(pacName)))
	{
		AG_MODULE_LOG(LOG_ERR, "copy module name fail!");
		return FALSE;
	}

	pstIpcRegister->uiIdSet = uiIdSet;
	AG_MODULE_LOG(LOG_WARNING, "module name:%s IdSet:%d", pacName, uiIdSet);

	ag_module_ipc_get_server_ip_port(&uipcipaddr, &usipcport, &uiServeraddr, &usServerPort, pacName);
	
	ag_module_ipc_connect_to_server(uipcipaddr, usipcport, uiServeraddr, usServerPort, pstIpcRegister);

	pstIpcRegister->uiFlag = 0;
	
	/*初始化libevent*/
	ag_module_ipc_module_event_init(pstIpcRegister, pRecv_fun);
	
	
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

	AG_MODULE_LOG(LOG_INFO, "Module name is:%s IDSet is:%d", pstIpcRegister->acname, pstIpcRegister->uiIdSet);

	#if 0
	pstIPCModule =ag_module_ipc_find_by_head (pstIpcRegister->acname);

	if(NULL == pstIPCModule)
	{
		AG_MODULE_LOG(LOG_INFO, "Module not register, Module name is:%s IDSet is:%d", pstIPCModule->acname, pstIPCModule->uiIdSet);
				
		return FALSE;
	}
	#endif
	/*进入主循环*/
	event_base_loop(pstIpcRegister->main_base, 0);
	
	return TRUE;
}
#endif
/*****************************************************************************
 Prototype    : ag_module_ipc_init
 Description  : ipc thread socket init
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/11/20
    Author       : lvjianlin
    Modification : Created function

*****************************************************************************/
s32 ag_module_ipc_init()
{
	#if 0
	struct timeval timeout = {SEND_TIMEOUT_VALUE, 0};

	AG_MODULE_LOG(LOG_INFO, "IPC socket init.");

	ag_module_ipc_get_server_ip_port(&gstAglinkIPCmgt.uipcipaddr, &gstAglinkIPCmgt.usipcport);
	
	s32 sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&gstAglinkIPCmgt.stipcskaddr, sizeof(gstAglinkIPCmgt.stipcskaddr));
	gstAglinkIPCmgt.stipcskaddr.sin_family = AF_INET;
	gstAglinkIPCmgt.stipcskaddr.sin_port = htons(gstAglinkIPCmgt.usipcport);
	gstAglinkIPCmgt.stipcskaddr.sin_addr.s_addr = gstAglinkIPCmgt.uipcipaddr;

	if(bind(sockfd, (struct sockaddr *)&gstAglinkIPCmgt.stipcskaddr, sizeof(gstAglinkIPCmgt.stipcskaddr)) == -1)
	{
		AG_MODULE_LOG(LOG_ERR, "IPC socket bind error.");
		close(sockfd);
		exit(-1);
	}

	/*设置server fd发送超时为timeval*/
	setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(struct timeval));
	/*设置server fd接收超时为timeval*/
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));

	gstAglinkIPCmgt.ipcsocket = sockfd;
	#endif
	/* listen connect from other module */
	/* register aglink_thread for listen IPC msg */
	u32 uiModuleId = 0;
	u32 uiLoop;
	u32 iRet;
	struct hlist_head *head = NULL;

	pthread_mutex_init(&ipc_hlist_mutex, NULL);

	#if 0
	for(uiLoop = AG_IPC_MSG_BEGIN; uiLoop < AG_IPC_MSG_MAX; uiLoop++)
	{
		uiIdSet != uiLoop;
	}
	gulIPCIdSet = uiIdSet;
	#endif
	gulIPCIdSet = (u32 ) AG_GET_MODULE_ID(AG_MSG_IPC_BEGIN);
	
	#if 0
	for(uiLoop = AGLINK_IDSET_BEGIN; uiLoop < AGLINK_IDSET_MAX; uiLoop++)
	{
		uiIdSet |= uiLoop; 
	}
	#endif
	uiModuleId = (u32)AG_GET_MODULE_ID(AGLINK_MSG_BEGIN);
	AG_MODULE_LOG(LOG_INFO, "gulIPCIdSet is %d aglinkIdSet is :%d", gulIPCIdSet, uiModuleId);

	memset(&gstAglinkIPCmgt, 0, sizeof(gstAglinkIPCmgt));
	iRet = ag_module_ipc_module_init("aglink", uiModuleId, &gstAglinkIPCmgt, ag_module_ipc_server_recv_msg);

	#if 0
	AG_MODULE_IPC_MODULE_LIST_T *pstIPCModule;

	pstIPCModule = (AG_MODULE_IPC_MODULE_LIST_T *)malloc(sizeof(AG_MODULE_IPC_MODULE_LIST_T));
	memset(pstIPCModule, 0, sizeof(AG_MODULE_IPC_MODULE_LIST_T));
	memcpy(pstIPCModule->acname, gstAglinkIPCmgt.acname, strlen(gstAglinkIPCmgt.acname));
	pstIPCModule->uiModuleId = gstAglinkIPCmgt.uiModuleId;
	memcpy(&pstIPCModule->stipcskaddr, &gstAglinkIPCmgt.stipcskaddr, sizeof(struct sockaddr_in));
	memcpy(&pstIPCModule->stSerskaddr, &gstAglinkIPCmgt.stipcskaddr, sizeof(struct sockaddr_in));

	AG_MSG_LOCK();
	head = ag_module_ipc_hash_fn(pstIPCModule->acname);
    	hlist_add_head(&(pstIPCModule->stHeadHlist), head);
    	list_add(&(pstIPCModule->stModuleList), &gstIPClist);
    	gulIPCListNum++;
	AG_MSG_UNLOCK();
	#endif
	return iRet;
}
/*****************************************************************************
 Prototype    : ag_module_msg_listen
 Description  : build channel between aglink and other module
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/11/18
    Author       : lvjianlin
    Modification : Created function

*****************************************************************************/
s32 ag_module_ipc_start()
{
	s32 iRet = 0;

	if(ag_module_ipc_init() != TRUE)
	{
		AG_MODULE_LOG(LOG_ALERT, "ipc init fail!");
		return FALSE;
	}

	
	return TRUE;
}

/*****************************************************************************
 Prototype    : ag_module_start_thread
 Description  : create a thread
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/11/18
    Author       : lvjianlin
    Modification : Created function

*****************************************************************************/
s32 ag_module_ipc_thread_entry()
{
	s32 iRet;
	pthread_t stpthread;

	iRet = pthread_create(&stpthread, NULL, ag_module_ipc_start, NULL);
	if(0 != iRet)
	{
			AG_MODULE_LOG(LOG_ALERT, "Create pthread fail!");
			return FALSE;
	}
	AG_MODULE_LOG(LOG_ALERT, "thread start success!");
	return TRUE;
}

s32 main(s32 argc, u8 **argv)
{
	s32 iRet = 0;
	
	if(ag_module_is_running(PID_FILE))
	{
		AG_MODULE_LOG(LOG_INFO, "Net Monitor is already running, exit.");
		exit(0);
	}

	if(ag_module_write_pidfile(PID_FILE) != TRUE)
		AG_MODULE_LOG(LOG_ERR, "Write aglink pidfile %s failed!!", PID_FILE);

	AG_MODULE_LOG(LOG_INFO, "aglink start to init.");

	if(ag_module_init_primary() != TRUE)
	{
		AG_MODULE_LOG(LOG_ERR, "ag_module_init_primary failed!");
		return FALSE;
	}

	if(ag_module_control_id_init_primary() != TRUE)
	{
		AG_MODULE_LOG(LOG_ERR, "ag_module_config_init_primary failed!");
		return FALSE;
	}

	if(ag_module_business_id_init_primary() != TRUE)
	{
		AG_MODULE_LOG(LOG_ERR, "ag_module_business_init_primary failed!");
		return FALSE;
	}

	if(ag_module_event_init_primary() != TRUE)
	{
		AG_MODULE_LOG(LOG_ERR, "ag_module_business_init_primary failed!");
		return FALSE;
	}

	#if 0
	/* 启动一个线程专门用于与注册模块之间的消息传递 */
	if(ag_module_ipc_thread_entry() != TRUE)
	{
		AG_MODULE_LOG(LOG_ERR, "ag_module_ipc_thread_start failed");
		return FALSE;
	}
	#endif
	return TRUE;
}

