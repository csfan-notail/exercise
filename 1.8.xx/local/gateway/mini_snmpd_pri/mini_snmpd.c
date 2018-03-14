/* -----------------------------------------------------------------------------
 * Copyright (C) 2008 Robert Ernst <robert.ernst@linux-solutions.at>
 *
 * This file may be distributed and/or modified under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See COPYING for GPL licensing information.
 */



#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <getopt.h>
#include <signal.h>
#include <syslog.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>

#include "mini_snmpd.h"


/******************************************************************************
* COPYRIGHT GBCOM
*******************************************************************************
* 功    能: 
* 修改历史: 
* 2012-9-10     冯静              新建
******************************************************************************/

#include "snmp_nms.h"

#include "my_snmp_def.h"
#include "my_snmp_data.h"
#include "my_snmp_info.h"
#include "my_snmp_handle.h"
#include "my_snmp_util.h"
#include "my_snmp_alarm.h"
#include "my_snmp_trap.h"

/******************************************************************************/
OSTOKEN gtSemConfig;	/* 配置互斥信号量 */
OSTOKEN gtSemStat;      /* 统计互斥信号量 */
/* BEGIN: Added by zoucaihong, 2013/9/23 */
OSTOKEN gtSemUdp;       /* UDP报文队列互斥信号量 */
OSTOKEN gtSemAlarm;     /* 告警互斥信号量 */
/* END: Added by zoucaihong, 2013/9/23 */

// add by zhoushouya, 解决set操作和get操作冲突
OSTOKEN gtSemSnmp;      /* set和get操作互斥信号量 */

CHAR g_acRootOID[128] = {0};
CHAR g_acVendor[64] = {0};

UINT32      g_ulRootOIDSize = 0;
OID         g_rootOID[MAX_SNMP_OID_SIZE] = {0};

SNMP_AGENT_DATA_T *g_pstSnmpAgentData;
SNMP_TABLE_INFO_T *g_pstTableInfo;
BYTE gabyRecvBuf[MAX_PACKET_SIZE];      
in_port_t g_alarm_port = AGENT_ALARM_PORT;
int g_alarm_sockfd = -1;
//OPEN_MIB_COMMUNITY_CONFIG_T g_astSnmpV2Config[TABLE_MAXRECNUM_SNMPV2] = {{{0}}};
OPEN_MIB_COMMUNITY_CONFIG_T g_stSnmpV2Config;
AGENT_INIT_STATE_T     g_stInitState;
/* 2013-05-16 fengjing add */
UINT32      g_ulEndian = 0;
/* BEGIN: Added by zoucaihong, 2013/9/24 */
QUEUE_T g_stUdpQueue;
extern AP_QUEUE_T      g_alarmQueue;
/* END: Added by zoucaihong, 2013/9/24 */
/* BEGIN: Added by zoucaihong, 2014/5/13 */
DEV_STATE_E     g_enDevState = DEV_STATE_NONE;/*设备与CMS的连接状态*/
UINT8           g_aucStateName[DEV_STATE_MAX][32]={ "S_NONE",
                                                    "S_DISCOVER",
                                                    "S_RUN"};
UINT32          g_ulRecvTimeout=0;      /* 设备与CMS保活超时时间*/
UINT32          g_ulTicks=0;
UINT32          g_ulHeartTicks=0;       /* 心跳计时 */
UINT32          g_ulDisTicks=0;         /* discover超时计时 */
UINT32          g_ulCmsRecvTime = 0;    /* 记录设备正确接收到CMS发送的SNMP报文的时间 */
/* END: Added by zoucaihong, 2014/5/13 */

CHAR    gacConfigVersion[256]={0};  /*ccs 配置模板版本号 */

/******************************************************************************/
extern BOOL snmp_init_tables_info(VOID);

/* BEGIN: Added by zoucaihong, 2013/3/21 */
AGENT_TBLMD_MAP_T astMap[] =
{	
    /* AP基本信息配置表 */
	{TABLE_NAME_AP_CONFIG_SYS_DEVINFO,          ac_agent_get_apbasicsetup_config,               0},
    /* AP软件信息表 */
	{TABLE_NAME_AP_CONFIG_SYS_SWINFO,           ac_agent_get_apswinfo,                          0},
    /* AP硬件信息表 */
	{TABLE_NAME_AP_CONFIG_SYS_HWINFO,           ac_agent_get_aphwinfo,                          0},
    /* AP IP地址配置表 */
	{TABLE_NAME_AP_CONFIG_SYS_IPTABLE,          ac_agent_get_apipsetup_config,                  0},
#ifndef GW_X86
	/* AP 5G优先配置表 */
	//{TABLE_NAME_AP_CONFIG_ADVANCE_5GPRIOR,      ac_agent_get_ap5gpriorsetup_config,             0},
    /* AP 11n优先配置表 */
    //{TABLE_NAME_AP_CONFIG_ADVANCE_11nPRIOR,     ac_agent_get_ap11npriorsetup_config,            0},
    /* AP 2.4G卡基本配置表 */
	{TABLE_NAME_AP_CONFIG_WIFIBASIC,            ac_agent_get_apwifidevicebasic_config,          0},
    /* AP 2.4G卡信道配置表 */
	{TABLE_NAME_AP_CONFIG_WIFICHANNEL,          ac_agent_get_apwifidevicechannel_config,        0},
    /* AP 2.4G卡功率配置表 */
	{TABLE_NAME_AP_CONFIG_WIFIPOWER,            ac_agent_get_apwifidevicepower_config,          0},
    /* AP 2.4G卡高级配置表 */
	{TABLE_NAME_AP_CONFIG_WIFIADVANCE,          ac_agent_get_apwifideviceadvance_config,        0},
    /* AP 2.4G卡速率配置表 */
	{TABLE_NAME_AP_CONFIG_WIFIRATE,             ac_agent_get_apwifidevicerate_config,           0},
    /* AP 5G卡基本配置表 */
	{TABLE_NAME_AP_CONFIG_WIFIBASIC5,           ac_agent_get_apwifidevicebasic5_config,         0},
    /* AP 5G卡信道配置表 */
	{TABLE_NAME_AP_CONFIG_WIFICHANNEL5,         ac_agent_get_apwifidevicechannel5_config,       0},
    /* AP 5G卡功率配置表 */
	{TABLE_NAME_AP_CONFIG_WIFIPOWER5,           ac_agent_get_apwifidevicepower5_config,         0},
    /* AP 5G卡高级配置表 */
	{TABLE_NAME_AP_CONFIG_WIFIADVANCE5,         ac_agent_get_apwifideviceadvance5_config,       0},
    /* AP 5G卡速率配置表 */
	{TABLE_NAME_AP_CONFIG_WIFIRATE5,            ac_agent_get_apwifidevicerate5_config,          0},
    /* AP VAP基本配置表 */
	{TABLE_NAME_AP_CONFIG_VAPBASICTABLE,        ac_agent_get_apvapbasicsetup_config,            0},
    /* AP VAP安全配置表 */
	{TABLE_NAME_AP_CONFIG_VAPSECURITYTABLE,     ac_agent_get_apvapsecuritysetup_config,         0},
    /* AP TRAP服务器配置表 */
	{TABLE_NAME_AP_CONFIG_TRAPDESTABLE,         ac_agent_get_aptrapdessetup_config,             0},
	/* AP VLAN配置表 */
	{TABLE_NAME_AP_CONFIG_VLANTABLE,            ac_agent_get_apvlansetup_config,                0},
#endif	
	{ "", NULL, 0 }
};
/* END: Added by zoucaihong, 2013/3/21 */


/*********add write log to file ********************8*/
#include <time.h>
int snmp_log_set(char *name, const char *format, ...)
{
#define LOG_BUF_SIZE  4096

	FILE *fp = NULL;
	time_t   now;
	struct tm *timenow;
	unsigned long filesize = -1;
	va_list args;
	int len;
	char *log_buf = NULL;

	if((NULL == name) || (NULL == format))
		return -1;
	if((fp = fopen(name, "a+")) == NULL)
		return -1;

	fseek(fp, 0L, SEEK_END);
	filesize = ftell(fp);

#ifdef GW_X86
	if(filesize > 2621440)
#else
#ifdef MEM_128M
	if(filesize > 262144)
#else
	if(filesize > 131072)
#endif
#endif
	{
		fclose(fp);
		remove(name);
		if((fp = fopen(name, "a+")) == NULL)
			return -1;		
	}

	if((log_buf = (char *)malloc(LOG_BUF_SIZE)) == NULL){
		fclose(fp);
		return -1;
	}

	time(&now);
	timenow = localtime(&now);
	strftime(log_buf, LOG_BUF_SIZE, "[%Y-%m-%d %H:%M:%S]  ", timenow);

	va_start(args, format);
	len = strlen(log_buf);
	vsnprintf((log_buf+len), (LOG_BUF_SIZE - 3 - len), format, args);
	va_end(args);
	
	strcat(log_buf, "\n\n");

	fwrite(log_buf, strlen(log_buf), 1, fp);
	
	fclose(fp);
	free(log_buf);
	return 0;
}








/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL snmp_init_sem(VOID)
{
	/* 创建信号量 */
	if (!OsCreateSemaphore(NULL, 1, 1, SEM_MUTEX, &gtSemConfig))
	{
		agent_printf(LOG_ERR, "snmp_init_data create config semux failed\n");
		return FALSE;
	}
	
	if (!OsCreateSemaphore(NULL, 1, 1, SEM_MUTEX, &gtSemStat))
	{
		agent_printf(LOG_ERR, "snmp_init_data create stat semux failed\n");
		return FALSE;
	}

    /* BEGIN: Added by zoucaihong, 2013/9/23 */
    /* 创建UDP报文队列互斥信号量 */
	if (!OsCreateSemaphore(NULL, 1, 1, SEM_MUTEX, &gtSemUdp))
	{
		agent_printf(LOG_ERR, "create udp semux failed\n");
		return FALSE;
	}
    
    /* 创建告警队列互斥信号量 */
	if (!OsCreateSemaphore(NULL, 1, 1, SEM_MUTEX, &gtSemAlarm))
	{
		agent_printf(LOG_ERR, "create alarm semux failed\n");
		return FALSE;
	}

    // add by zhoushouya, 解决set操作和get操作冲突
    if (!OsCreateSemaphore(NULL, 1, 1, SEM_MUTEX, &gtSemSnmp))
	{
		agent_printf(LOG_ERR, "create snmp semux failed\n");
		return FALSE;
	}
	
    /* END: Added by zoucaihong, 2013/9/23 */

	return TRUE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL snmp_init_customer(VOID)
{
	CHAR   *pos = NULL;
	CHAR   acValue[16] = {0};
	UINT32 ulIndex = 0;
	UINT32 ulIndex2 = 0;
	
	/*test*/
    /* BEGIN: Modified by zoucaihong, 2012/12/4 
    add system group and interfaces group of RFC1213*/
	//sprintf(g_acRootOID, "%s", "1.3.6.1.4.1.171");
	//sprintf(g_acRootOID, "%s", "1.3.6.1");
    /* END: Modified by zoucaihong, 2012/12/4 */
    printf("g_vendor:%s.\n", g_vendor);
    sprintf(g_acRootOID, "%s", g_vendor);
    agent_printf(LOG_ERR, "Func:%s Line:%d g_acRootOID:%s.\n", 
        __func__, __line__, g_acRootOID);
	sprintf(g_acVendor, "%s", "ruijie");

	g_ulRootOIDSize=0;
	memset(g_rootOID, 0, sizeof(g_rootOID));

	pos = &(g_acRootOID[0]);
	memset(acValue, 0, sizeof(acValue));
	while(*pos != '\0')
	{
		while(*pos != '.' && *pos != '\0')
		{
			acValue[ulIndex++] = *pos;
			pos++;
		}

		if (*pos == '.')
		{
			if (ulIndex>0)
			{
				g_rootOID[ulIndex2++] = atoi(acValue);
				ulIndex= 0;
				memset(acValue, 0, sizeof(acValue));
			}
			pos++;
		}
		else if (*pos == '\0')
		{
			if (ulIndex>0)
			{
				g_rootOID[ulIndex2++] = atoi(acValue);
				ulIndex= 0;
				memset(acValue, 0, sizeof(acValue));
			}
		}
	}	

	g_ulRootOIDSize = ulIndex2;

	return TRUE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL snmp_init_data(VOID)
{	
	snmp_get_uptime_10ms();
	snmp_get_uptime();
    BOOL bRet = FALSE;
	AGENT_TBLMD_MAP_T *hp = NULL;

	g_pstSnmpAgentData = SNMP_AGENT_MALLOC(sizeof(SNMP_AGENT_DATA_T));
	if (!g_pstSnmpAgentData)
	{
		agent_printf(LOG_ERR,  "snmp_init_data: malloc g_pstSnmpAgentData failed");
		return FALSE;
	}
	memset(g_pstSnmpAgentData, 0, sizeof(SNMP_AGENT_DATA_T));
    /* BEGIN: Added by zoucaihong, 2012/11/20 */
	memset(&g_stSnmpV2Config, 0, sizeof(g_stSnmpV2Config));
    /* END: Added by zoucaihong, 2012/11/20 */
    memset(&g_stInitState, 0, sizeof(g_stInitState));
    
	// init snmp agent data
	// apConfig
	if ( FALSE == g_stInitState.bInitApConfigData)
	{
		for (hp = astMap; (hp->acTblName != NULL && hp->acTblName[0]!='\0'); hp++)
		{
			if (hp->ulInitFlag)
			{
				continue;
			}
			
	        bRet = (*(hp->md))();
			if (bRet)
			{
				agent_printf(LOG_INFO, "snmp_init_data:get %s table record succeed\n",
						hp->acTblName);
				hp->ulInitFlag = 1;
			}
			else
			{
				agent_printf(LOG_ERR, "snmp_init_data:get %s table record failed\n",
						hp->acTblName);
				//return FALSE;
			}
		}
        
    	bRet = ap_agent_parse_conf_file();
    	if (FALSE == bRet)
    	{
    		agent_printf(LOG_ERR, "%s:ac_agent_parse_agent_cfgfile failed\n",
    			SNMPAGENT_CONTROL_NAME);
    	}
    	else
    	{
    		agent_printf(LOG_INFO, "%s:ac_agent_parse_agent_cfgfile succeed\n",
    			SNMPAGENT_CONTROL_NAME);
    	}

        g_stInitState.bInitApConfigData = TRUE;
    }

	if (g_pstSnmpAgentData->pstApSysOper == NULL)
	{
		g_pstSnmpAgentData->pstApSysOper = SNMP_AGENT_MALLOC(sizeof(AP_OPER_SYSOPER_T));
		if (NULL != g_pstSnmpAgentData->pstApSysOper)
		{
			memset(g_pstSnmpAgentData->pstApSysOper, 0, sizeof(AP_OPER_SYSOPER_T));
		}
	}
    
	if (g_pstSnmpAgentData->pstApFileTransConfig == NULL)
	{
		g_pstSnmpAgentData->pstApFileTransConfig = SNMP_AGENT_MALLOC(sizeof(AP_OPER_FILETRANS_T));
		if (NULL != g_pstSnmpAgentData->pstApFileTransConfig)
		{
			memset(g_pstSnmpAgentData->pstApFileTransConfig, 0, sizeof(AP_OPER_FILETRANS_T));
		}
	}
	
	return TRUE;
}
#if 0
/* BEGIN: Added by zoucaihong, 2013/9/24 */
UINT32 ap_init_udp_queue
(
    QUEUE_T     *queue,
    UINT32      nodeNum,
    UINT32      dataSize
)
{
    /* 局部变量定义 */
    const int       nodeSize = sizeof(QUEUE_NODE_T);
    UINT32          i = 0;

    UINT8           *memAddr = NULL;
    QUEUE_NODE_T    *node;
    QUEUE_NODE_T    *nextNode = NULL;

    /* 检查参数有效性 */
    if (0 == dataSize)
    {
        agent_printf(LOG_ERR, "dataSize is 0!");
        return AP_FAIL;
    }

    /* alloc memory for node list */
    queue->ulNodeNum = nodeNum;
    queue->ulNodeDataSize = dataSize;

    memAddr = SNMP_AGENT_MALLOC( (nodeSize) * nodeNum );
    if (NULL == memAddr)
    {
        agent_printf(LOG_ERR, "alloc memory for queue failed!");
        return AP_FAIL;
    }

    /* init all node */
    queue->pstNodeList = (QUEUE_NODE_T*)memAddr;

    /* init free head node */
    queue->stFreeHead.pstPrev= NULL;

    /* init free data node */
    node = queue->pstNodeList;
    node->pstPrev = &queue->stFreeHead;
    node->pstPrev->pstNext = node;
    node->pucData = NULL;

    for(i=1; i<queue->ulNodeNum; i++)
    {
        nextNode = node + 1;

        node->pstNext = nextNode;
        nextNode->pstPrev = node;

        nextNode->pucData= NULL;

        node++;
    }

    /* init free tail node */
    node->pstNext = &queue->stFreeTail;
    queue->stFreeTail.pstPrev = node; /* here node is actually the last free data node */
    queue->stFreeTail.pstNext = NULL;

    /* init used head and tail node */
    queue->stUsedHead.pstNext = &queue->stUsedTail;
    queue->stUsedHead.pstPrev = NULL;

    queue->stUsedTail.pstNext = NULL;
    queue->stUsedTail.pstPrev = &queue->stUsedHead;

    return AP_OK;
}

UINT32 snmp_init_udp_queue()
{
    /* 局部变量定义 */
    UINT32 ret = AP_FAIL;

    /* 清空告警消息队列 */
    memset(&g_stUdpQueue, 0, sizeof(g_stUdpQueue));

    /* 初始化UDP消息队列 */
    ret = ap_init_udp_queue(&g_stUdpQueue,
                    UDP_QUEUE_NUM,
                    sizeof(client_t));
    if (ret != AP_OK)
    {
        agent_printf(LOG_ERROR, "init g_stUdpQueue failed!");
    }

    return ret;
}
/* END: Added by zoucaihong, 2013/9/24 */
#endif



BOOL snmp_init_sysinfo(VOID)
{
    BOOL bRet;
    
    /*初始化配置模板版本号*/
    app_get_config_version(gacConfigVersion);
    return TRUE;
}





/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL snmp_agent_init(VOID)
{
	if(!snmp_init_sem())
	{
		agent_printf(LOG_ERR, "snmp_init_sem failed\n");
		return FALSE;
	}
	agent_printf(LOG_INFO, "snmp_init_sem succeed\n");
	
	if (!snmp_init_customer())
	{
		agent_printf(LOG_ERR, "snmp_init_customer failed\n");
		return FALSE;
	}
	agent_printf(LOG_INFO, "snmp_init_customer succeed\n");
	
	if (!snmp_init_tables_info())
	{
		agent_printf(LOG_ERR, "snmp_init_table_info failed\n");
		return FALSE;
	}
	agent_printf(LOG_INFO, "snmp_init_tables_info succeed\n");

        /**/
        snmp_init_sysinfo();

	
	/* init agent data */
	if (!snmp_init_data())
	{
		agent_printf(LOG_ERR, "snmp_init_data failed\n");
		return FALSE;
	}
	agent_printf(LOG_INFO, "snmp_init_data succeed\n");

    /* BEGIN: Added by zoucaihong, 2013/9/24 */
    /* init alarm queue */
    if ( AP_OK != snmp_init_alarm_queue() )
    {
		agent_printf(LOG_ERR, "snmp_init_alarm_queue failed\n");
		return FALSE;
    }

#if 0
    /* init udp queue */
    if ( AP_OK != snmp_init_udp_queue() )
    {
		agent_printf(LOG_ERR, "snmp_init_udp_queue failed\n");
		return FALSE;
    }
    /* END: Added by zoucaihong, 2013/9/24 */
#endif

	return TRUE;
}

VOID agent_handle_alarm_msg(UINT16 usEvent, UINT8 *pucMsgBody, UINT16 usMsgLen)
{
    agent_printf(LOG_INFO, "into agent_handle_alarm_msg");
    BOOL bRet = FALSE;
    
    switch ( usEvent )
    {
        case EV_AP_REPORT_ALARM_REQ_MSG :
            bRet = ap_report_alarm_req_msg(pucMsgBody, usMsgLen);
            if ( FALSE == bRet )
            {
                lprintf(LOG_ERR, "ap_report_alarm_req_msg failed.");
            }
            else
            {
                lprintf(LOG_INFO, "ap_report_alarm_req_msg succeed.");
            }
            break;
        default:
            lprintf(LOG_ERR, "invalid event:%u.", usEvent);
    }
}
#if 0
VOID *agent_alarmthread(VOID* arg) 
{
	struct my_sockaddr_t sockaddr;
	my_socklen_t socklen;
	int iByteCount;
	struct timeval tv_sleep;

	socklen = sizeof (sockaddr);
    fd_set alarmfds;
    int nfds;

    TMsgStruc *pstAlarmMsgHead = NULL;
    UINT16 usMsgLen = 0;
    	
	while(1)
	{
	    FD_ZERO (&alarmfds);
	    FD_SET(g_alarm_sockfd, &alarmfds);
        nfds = g_alarm_sockfd;
    	tv_sleep.tv_sec = AGENT_ALARM_TIMEOUT;
    	tv_sleep.tv_usec = 0;
		if (select(nfds+1, &alarmfds, NULL, NULL, &tv_sleep) == -1)
		{
			lprintf(LOG_ERR,"Fatal error in selecting \n");
			continue;
		}
		if (FD_ISSET(g_alarm_sockfd, &alarmfds))
		{
		    memset(gabyRecvBuf, 0, MAX_PACKET_SIZE);
        	iByteCount = recvfrom(g_alarm_sockfd, gabyRecvBuf, MAX_PACKET_SIZE,
        		0, (struct sockaddr *)&sockaddr, &socklen);
        	if (iByteCount == -1) {
        		lprintf(LOG_WARNING, "could not receive packet on UDP port %d: %m\n",
        			g_alarm_port);
        		continue;
        	}
            pstAlarmMsgHead = (TMsgStruc *)gabyRecvBuf;
            usMsgLen = sizeof(TMsgStruc)+pstAlarmMsgHead->wLen;
            if ( iByteCount != usMsgLen )
            {
                lprintf(LOG_ERR, "Byte count mismatch, msg discarded!");
                continue;
            }
            /*如果trap开关为开，则处理告警消息，否则不处理*/
            if ( g_traps_switch )
            {
                agent_handle_alarm_msg(pstAlarmMsgHead->wEvent, gabyRecvBuf+sizeof(TMsgStruc), pstAlarmMsgHead->wLen);
            }
		}
	}
	
	return NULL;
}

VOID snmp_agent_alarm_msg_handler(VOID)
{
    BOOL bFlag = FALSE;
    pthread_t alarm_thread;
	struct my_sockaddr_t sockaddr;
	my_socklen_t socklen;
	/* Open the server's UDP port and prepare it for listening */
	g_alarm_sockfd = socket(my_pf_inet, SOCK_DGRAM, 0);
	if (g_alarm_sockfd == -1) {
		lprintf(LOG_ERR, "could not create UDP socket: %m\n");
		exit(EXIT_SYSCALL);
	}
	sockaddr.my_sin_family = my_af_inet;
	sockaddr.my_sin_port = htons(g_alarm_port);
	sockaddr.my_sin_addr = my_inaddr_any;
	socklen = sizeof (sockaddr);
	if (bind(g_alarm_sockfd, (struct sockaddr *)&sockaddr, socklen) == -1) {
		lprintf(LOG_ERR, "could not bind UDP socket to port %d: %m\n", g_alarm_port);
		exit(EXIT_SYSCALL);
	}

    bFlag = TRUE;
    if (setsockopt(g_alarm_sockfd, SOL_SOCKET, SO_REUSEADDR, &bFlag, sizeof(bFlag)) == -1) 
	{
		lprintf(LOG_ERR, "setsockopt SO_REUSEADDR error.\n");
		exit(EXIT_SYSCALL);
	}

	if ( 0 == pthread_create((pthread_t*)&alarm_thread,
                             (pthread_attr_t*)NULL,
                             (void*)agent_alarmthread,
                             (void*)NULL))
    {
        lprintf(LOG_INFO, "pthread_create agent_alarmthread success.");
    }    
}
#endif
/* BEGIN: Added by zoucaihong, 2013/9/24 */
UINT32 ap_check_alarm_report()
{
    //agent_printf(LOG_INFO, "into ap_check_alarm_report");
	if (!OsSemaphoreP(gtSemAlarm, WAIT_FOREVER, 1))
	{
		agent_printf(LOG_ERR,"gtSemAlarm OsSemaphoreP fail\n");
    	return AP_FAIL;
	}
    ap_resend_queue_msg(&g_alarmQueue);
	OsSemaphoreV(gtSemAlarm, 1);
    return AP_OK;
}
#if 0
UINT32 handle_udp_queue_node_msg_batch(UINT32 ulNum, QUEUE_NODE_T *ppstNode)
{
   
	/* 局部变量定义 */
    client_t        client;
	struct my_sockaddr_t sockaddr;
	my_socklen_t socklen;
	char straddr[my_inet_addrstrlen];
	int rv;
	UINT32              ulLoopNo = 0;

    agent_printf(LOG_INFO, "into handle_udp_queue_node_msg_batch");
    
	/* 检查参数合法性 */
	if (NULL == ppstNode)
	{
		agent_printf(LOG_ERROR, "ppstNode is NULL!");
		return AP_FAIL;
	}

	if (0 == ulNum)
	{
		agent_printf(LOG_ERROR, "ulNum is 0.");
		return AP_FAIL;
	}

	socklen = sizeof (sockaddr);
	for (ulLoopNo=0; ulLoopNo<ulNum; ulLoopNo++)
	{
        memcpy(&client, ppstNode[ulLoopNo].pucData, sizeof(client_t));
	    sockaddr.my_sin_addr = client.addr;
	    sockaddr.my_sin_port = client.port;

	    if(NULL != ppstNode[ulLoopNo].pucData)
	    {
            free(ppstNode[ulLoopNo].pucData);
            ppstNode[ulLoopNo].pucData = NULL;
	    }

    	/* Call the protocol handler which will prepare the response packet */
    	// by zhoushouya ,SET操作收到消息后立即回复
    	//inet_ntop(my_af_inet, &sockaddr.my_sin_addr, straddr, sizeof(straddr));
    	if (!OsSemaphoreP(gtSemSnmp, WAIT_FOREVER, 1))
    	{
    		agent_printf(LOG_ERR,"gtSemUdp OsSemaphoreP fail\n");
        	return AP_FAIL;
    	}
    	if (snmp(&client) == -1) {
    		lprintf(LOG_WARNING, "could not handle packet from UDP client %s:%d: %m\n",
    			straddr, sockaddr.my_sin_port);
    	    OsSemaphoreV(gtSemSnmp, 1);
    		return AP_FAIL;
    	} else if (client.size == 0) {
    		lprintf(LOG_WARNING, "could not handle packet from UDP client %s:%d: ignored\n",
    			straddr, sockaddr.my_sin_port);
    		OsSemaphoreV(gtSemSnmp, 1);
    		return AP_FAIL;
    	}
    	OsSemaphoreV(gtSemSnmp, 1);
    	
    	#if 0
    	client.outgoing = 1;
    	/* Send the whole UDP packet to the socket at once */
    	rv = sendto(g_udp_sockfd, client.packet, client.size,
    		MSG_DONTWAIT, (struct sockaddr *)&sockaddr, socklen);
    	inet_ntop(my_af_inet, &sockaddr.my_sin_addr, straddr, sizeof(straddr));
    	if (rv == -1) {
    		lprintf(LOG_WARNING, "could not send packet to UDP client %s:%d: %m\n",
    			straddr, sockaddr.my_sin_port);
    	} else if (rv != client.size) {
    		lprintf(LOG_WARNING, "could not send packet to UDP client %s:%d: "
    			"only %d of %d bytes written\n", straddr,
    			sockaddr.my_sin_port, rv, (int) client.size);
    	}
#ifdef DEBUG
    	dump_packet(&client);
#endif
        #endif
	}

	return AP_OK;	
}

UINT32 snmp_check_udp_queue()
{
    //agent_printf(LOG_INFO, "into snmp_check_udp_queue");
    /* 局部变量定义 */
    QUEUE_T         *queue = NULL;
    QUEUE_NODE_T    *currNode = NULL;
    QUEUE_NODE_T    *nextNode = NULL;
    QUEUE_NODE_T    *delNode = NULL;
    QUEUE_NODE_T    *firstFreeNode = NULL;
    UINT32                  ret = AP_FAIL;

    /* 一次发送的节点数目 */
	QUEUE_NODE_T   apstSendNode[UDP_QUEUE_BATCHNUM_MAX];
	UINT32          ulIndex = 0;

	if (!OsSemaphoreP(gtSemUdp, WAIT_FOREVER, 1))
	{
		agent_printf(LOG_ERR, "gtSemUdp OsSemaphoreP fail\n");
    	return AP_FAIL;
	}
    //agent_printf(LOG_ERR, "gtSemUdp OsSemaphoreP succeed.\n");
    queue = &g_stUdpQueue;
    
	currNode = queue->stUsedHead.pstNext;
	while(currNode != &queue->stUsedTail&& ulIndex<UDP_QUEUE_BATCHNUM_MAX)
	{
		nextNode = currNode->pstNext;
		//apstSendNode[ulIndex++] = currNode;
		apstSendNode[ulIndex].pucData = SNMP_AGENT_MALLOC(sizeof(client_t));
		memcpy(apstSendNode[ulIndex++].pucData, currNode->pucData, sizeof(client_t));
        agent_printf(LOG_INFO, "ulIndex:%u.\n", ulIndex);

        if(NULL != currNode->pucData)
        {
            free(currNode->pucData);
            currNode->pucData = NULL;
        }

        /* 从UDP报文队列中删除节点 */
        delNode = currNode;
        /* remove the node from used node link list */
        delNode->pstPrev->pstNext = delNode->pstNext;
        delNode->pstNext->pstPrev = delNode->pstPrev;

        /* add the node to the free node link list */
        firstFreeNode = queue->stFreeHead.pstNext;

        queue->stFreeHead.pstNext = delNode;
        delNode->pstPrev = &queue->stFreeHead;

        delNode->pstNext = firstFreeNode;
        firstFreeNode->pstPrev= delNode;

		currNode = nextNode;
		if (NULL == currNode)
		{
			agent_printf(LOG_ERROR, "currNode is null!");
            OsSemaphoreV(gtSemUdp, 1);
            agent_printf(LOG_ERR, "gtSemUdp OsSemaphoreV succeed.\n");
            return AP_FAIL;
		}
	}

	if (ulIndex == 0)
	{
		//agent_printf(LOG_INFO, "queue is empty.");
        OsSemaphoreV(gtSemUdp, 1);
        //agent_printf(LOG_ERR, "gtSemUdp OsSemaphoreV succeed.\n");
        return AP_FAIL;
	}
    OsSemaphoreV(gtSemUdp, 1);
    //agent_printf(LOG_ERR, "gtSemUdp OsSemaphoreV succeed.\n");

	ret = handle_udp_queue_node_msg_batch(ulIndex, apstSendNode);
	if (AP_OK != ret)
	{
		agent_printf(LOG_ERROR, "handle_udp_queue_node_msg_batch failed!");
		return AP_FAIL;
	}

    return AP_OK;
}
#endif

BOOL ap_check_problem_collect()
{
    if ( NULL != g_pstSnmpAgentData->pstApSysOper )
    {
        if ( AP_SYSOPER_SET == g_pstSnmpAgentData->pstApSysOper->ulDiagnose
            && NULL != g_pstSnmpAgentData->pstApFileTransConfig )
        {
            app_problem_collect(g_pstSnmpAgentData->pstApFileTransConfig);
        }
        g_pstSnmpAgentData->pstApSysOper->ulDiagnose = AP_SYSOPER_NORMAL;
    }
    return TRUE;
}

VOID *snmp_agent_timer_loop(VOID* arg) 
{
	static UINT32 ulTicks = 0;
	UINT32 ulNormalCollectCycle = 5;
	
	for(;;)
	{
		ulTicks++;

		if (ulTicks%ulNormalCollectCycle == 0)
		{
			ap_check_alarm_report();
		}

#if 0
        /* 检查UDP报文队列，处理snmp set报文 */
        //if ( ulTicks%ulNormalCollectCycle == 1 )
        {
            snmp_check_udp_queue();
        }
#endif

        /* BEGIN: Added by zoucaihong, 2014/7/15 */
        /* 一键收集 */
        ap_check_problem_collect();
        /* END: Added by zoucaihong, 2014/7/15 */

		sleep(1);
	}
}
BOOL snmp_agent_timer()
{	
	int res= -1;
	pthread_t a_thread;

	res = pthread_create(&a_thread, NULL, snmp_agent_timer_loop, NULL);
	if (0 != res) 
	{
		agent_printf(LOG_ERR, "Thread creation faied!\n");
		return FALSE;
	}
	agent_printf(LOG_INFO, "Thread creation succeed!\n");

	return TRUE;
}
/* END: Added by zoucaihong, 2013/9/24 */
/* -----------------------------------------------------------------------------
 * Helper functions
 */

static void print_help(void)
{
	fprintf(stderr, "usage: mini_snmpd [options]\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "-p, --udp-port nnn         set the UDP port to bind to (161)\n");
	fprintf(stderr, "-P, --tcp-port nnn         set the TCP port to bind to (161)\n");
	fprintf(stderr, "-c, --community nnn        set the community string (public)\n");
	fprintf(stderr, "-D, --description nnn      set the system description (empty)\n");
	fprintf(stderr, "-V, --vendor nnn           set the system vendor (empty)\n");
	fprintf(stderr, "-L, --location nnn         set the system location (empty)\n");
	fprintf(stderr, "-C, --contact nnn          set the system contact (empty)\n");
	fprintf(stderr, "-d, --disks nnn            set the disks to monitor (/)\n");
	fprintf(stderr, "-i, --interfaces nnn       set the network interfaces to monitor (lo)\n");
	fprintf(stderr, "-I, --listen nnn           set the network interface to listen (all)\n");
	fprintf(stderr, "-s, --statistics cycle nnn set statistics cycle\n");
	fprintf(stderr, "-S, --trapswitch nnn       set the traps switch(on:1, off:0)\n");
	fprintf(stderr, "-t, --timeout nnn          set the timeout for MIB updates (1 second)\n");
	fprintf(stderr, "-a, --auth                 require authentication (thus SNMP version 2c)\n");
	fprintf(stderr, "-v, --verbose              verbose syslog messages \n");
	fprintf(stderr, "-l, --licensing            print licensing info and exit\n");
	fprintf(stderr, "-h, --help                 print this help and exit\n");
	fprintf(stderr, "\n");
}

static void print_version(void)
{
	fprintf(stderr, "Mini SNMP Daemon Version " VERSION "\n");
	fprintf(stderr, "A minimal simple network management protocol daemon for embedded Linux\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Copyright (C) 2008 Robert Ernst <robert.ernst@aon.at>\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "This program is free software; you can redistribute it and/or modify\n");
	fprintf(stderr, "it under the terms of the GNU General Public License as published by\n");
	fprintf(stderr, "the Free Software Foundation; either version 2 of the License, or\n");
	fprintf(stderr, "(at your option) any later version.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "This program is distributed in the hope that it will be useful,\n");
	fprintf(stderr, "but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
	fprintf(stderr, "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
	fprintf(stderr, "GNU General Public License for more details.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "You should have received a copy of the GNU General Public License\n");
	fprintf(stderr, "along with this program; if not, write to the Free Software\n");
	fprintf(stderr, "Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA\n");
	fprintf(stderr, "\n");
}

static void handle_signal(int signo)
{
    close(g_alarm_sockfd);
    agent_printf(LOG_INFO, "close g_alarm_sockfd");
    close(g_udp_sockfd);
	g_quit = 1;
}

/* BEGIN: Added by zoucaihong, 2013/9/22 */
static void handle_alarm_msg(void)
{
    agent_printf(LOG_INFO, "into handle_alarm_msg");
	struct my_sockaddr_t sockaddr;
	my_socklen_t socklen;
	int iByteCount;

	socklen = sizeof (sockaddr);

    TMsgStruc *pstAlarmMsgHead = NULL;
    UINT16 usMsgLen = 0;
    	
    memset(gabyRecvBuf, 0, MAX_PACKET_SIZE);
	iByteCount = recvfrom(g_alarm_sockfd, gabyRecvBuf, sizeof(gabyRecvBuf),
		0, (struct sockaddr *)&sockaddr, &socklen);
	if (iByteCount == -1) {
		lprintf(LOG_WARNING, "could not receive packet on UDP port %d: %m\n",
			g_alarm_port);
		return;
	}
    pstAlarmMsgHead = (TMsgStruc *)gabyRecvBuf;
    usMsgLen = sizeof(TMsgStruc)+pstAlarmMsgHead->wLen;
    if ( iByteCount != usMsgLen )
    {
        lprintf(LOG_ERR, "Byte count mismatch, msg discarded!");
        return;
    }
    /*如果trap开关为开，则处理告警消息，否则不处理*/
    if ( g_traps_switch )
    {
        agent_handle_alarm_msg(pstAlarmMsgHead->wEvent, gabyRecvBuf+sizeof(TMsgStruc), pstAlarmMsgHead->wLen);
    }
	
	return;
}
/* END: Added by zoucaihong, 2013/9/22 */
#if 0
/* BEGIN: Added by zoucaihong, 2013/9/24 */
UINT32 udp_add_node_to_queue(QUEUE_T *pstQueue, UINT8* pucData)
{
    agent_printf(LOG_INFO, "into udp_add_node_to_queue");
    /* 局部变量定义 */
    QUEUE_NODE_T* pstNode = NULL;
    QUEUE_NODE_T* pstLastUsedNode = NULL;
	
    /* 检查参数有效性 */
	if (NULL == pstQueue)
	{
		agent_printf(LOG_ERROR, "pstQueue is NULL.\n");
		return AP_FAIL;
	}

	if (NULL == pucData)
	{
		agent_printf(LOG_ERROR, "pucData is NULL.\n");
		return AP_FAIL;
	}

    /* if there's no free data node, then remove the first used node */
    if (pstQueue->stFreeHead.pstNext == &(pstQueue->stFreeTail))
    {
		agent_printf(LOG_ERROR, "no free data node.\n");
    	return AP_FAIL;
    }

    /* add the nodeData */
    pstNode = pstQueue->stFreeHead.pstNext;

    /* free the node from free link list */
    pstQueue->stFreeHead.pstNext = pstNode->pstNext;
    pstNode->pstNext->pstPrev = &pstQueue->stFreeHead;

    /* add the node to used link list */
    pstLastUsedNode = pstQueue->stUsedTail.pstPrev;

    pstLastUsedNode->pstNext = pstNode;
    pstNode->pstPrev = pstLastUsedNode;

    pstNode->pstNext = &pstQueue->stUsedTail;
    pstQueue->stUsedTail.pstPrev = pstNode;
	
	if (NULL == pstNode->pucData)
	{
		pstNode->pucData = SNMP_AGENT_MALLOC( pstQueue->ulNodeDataSize);
		if (NULL == pstNode->pucData)
		{
			agent_printf(LOG_ERROR, "malloc pucData fail.\n");
			return AP_FAIL;
		}
		memset(pstNode->pucData, 0, pstQueue->ulNodeDataSize);
	}

    memcpy(pstNode->pucData, pucData, pstQueue->ulNodeDataSize);

    return AP_OK;
}
UINT32 add_udp_msg_to_queue(client_t *msg)
{
    agent_printf(LOG_INFO, "into add_udp_msg_to_queue");
    /* 局部变量定义 */
    UINT32          ulRet = AP_FAIL;
    QUEUE_T         *queue = NULL;

    /* 检查参数合法性 */
    if (NULL == msg)
    {
        agent_printf(LOG_ERR, "msg is null!");
        return AP_FAIL;
    }
	if (!OsSemaphoreP(gtSemUdp, WAIT_FOREVER, 1))
	{
		agent_printf(LOG_ERR,"gtSemUdp OsSemaphoreP fail\n");
    	return AP_FAIL;
	}
    queue = &g_stUdpQueue;
    ulRet = udp_add_node_to_queue(queue, (UINT8*)msg);
    if (ulRet != AP_OK)
    {
        agent_printf(LOG_ERR,"udp_add_node_to_queue failed!");
    	OsSemaphoreV(gtSemUdp, 1);
        return AP_FAIL;
    }
    agent_printf(LOG_ERR,"udp_add_node_to_queue succeed!");
	OsSemaphoreV(gtSemUdp, 1);
    return AP_OK;
}
/* END: Added by zoucaihong, 2013/9/24 */
#endif

static void handle_udp_client(void)
#if 0
{
	struct my_sockaddr_t sockaddr;
	my_socklen_t socklen;
	int rv;
	char straddr[my_inet_addrstrlen];
    /* BEGIN: Added by zoucaihong, 2013/9/25 */
	request_t request;
    int i;
    /* END: Added by zoucaihong, 2013/9/25 */
    /* BEGIN: Added by zoucaihong, 2014/5/13 */
	UINT8   aucFromIp[4] = {0};
    /* END: Added by zoucaihong, 2014/5/13 */
    
	/* Read the whole UDP packet from the socket at once */
	socklen = sizeof (sockaddr);
	rv = recvfrom(g_udp_sockfd, g_udp_client.packet, sizeof (g_udp_client.packet),
		0, (struct sockaddr *)&sockaddr, &socklen);
	if (g_udp_client.size == -1) {
		lprintf(LOG_WARNING, "could not receive packet on UDP port %d: %m\n",
			g_udp_port);
		return;
	}
	g_udp_client.timestamp = time(NULL);
	g_udp_client.sockfd = g_udp_sockfd;
	g_udp_client.addr = sockaddr.my_sin_addr;
	g_udp_client.port = sockaddr.my_sin_port;
	g_udp_client.size = rv;
	g_udp_client.outgoing = 0;

    /* BEGIN: Added by zoucaihong, 2014/5/13 */
	memcpy(aucFromIp, (UINT8 *)(&sockaddr.my_sin_addr), sizeof(sockaddr.my_sin_addr));
    dev_get_cms_recvtime(aucFromIp);
    /* END: Added by zoucaihong, 2014/5/13 */
    
#ifdef DEBUG
	dump_packet(&g_udp_client);
#endif

    /* BEGIN: Added by zoucaihong, 2013/9/25 */
    /* 判断SNMP报文请求类型，如果为set报文，则将报文保存到队列中，在另一个线程中处理 */
	memset(&request, 0, sizeof (request));

	/* Decode the request (only checks for syntax of the packet) */
	if (get_snmp_request_type(&request, &g_udp_client) == -1) {
        agent_printf(LOG_ERR, "decode_snmp_request fail.\n");
        
    	for(i=0;i<request.oid_list_length;i++)
    	{
    		if(NULL != request.data_list[i].buffer)
    		{
    			free(request.data_list[i].buffer);
    		}
    	}	
    	return;
	}
	for(i=0;i<request.oid_list_length;i++)
	{
		if(NULL != request.data_list[i].buffer)
		{
			free(request.data_list[i].buffer);
		}
	}	
    if ( BER_TYPE_SNMP_SET == request.type )
    {
        agent_printf(LOG_INFO, "request type is set.\n");
        add_udp_msg_to_queue(&g_udp_client);
        // by zhoushouya ,SET操作收到消息后立即回复
        //return;
    }
    /* END: Added by zoucaihong, 2013/9/25 */

	/* Call the protocol handler which will prepare the response packet */
	inet_ntop(my_af_inet, &sockaddr.my_sin_addr, straddr, sizeof(straddr));
	// by zhoushouya ,SET操作收到消息后立即回复
	if(BER_TYPE_SNMP_SET != request.type)
	{
	    if (!OsSemaphoreP(gtSemSnmp, WAIT_FOREVER, 1))
    	{
    		agent_printf(LOG_ERR,"gtSemUdp OsSemaphoreP fail\n");
        	return;
    	}
    	if (snmp(&g_udp_client) == -1) {
    		lprintf(LOG_WARNING, "could not handle packet from UDP client %s:%d: %m\n",
    			straddr, sockaddr.my_sin_port);
    	    OsSemaphoreV(gtSemSnmp, 1);
    		return;
    	} else if (g_udp_client.size == 0) {
    		lprintf(LOG_WARNING, "could not handle packet from UDP client %s:%d: ignored\n",
    			straddr, sockaddr.my_sin_port);
    		OsSemaphoreV(gtSemSnmp, 1);
    		return;
    	}
    	OsSemaphoreV(gtSemSnmp, 1);
    }//end by zhoushouya
	g_udp_client.outgoing = 1;

	// by zhoushouya ,SET操作收到消息后立即回复
	if ( BER_TYPE_SNMP_SET == request.type )
    {
        g_udp_client.packet[17]=0xa2;
    }

	/* Send the whole UDP packet to the socket at once */
	rv = sendto(g_udp_sockfd, g_udp_client.packet, g_udp_client.size,
		MSG_DONTWAIT, (struct sockaddr *)&sockaddr, socklen);
	inet_ntop(my_af_inet, &sockaddr.my_sin_addr, straddr, sizeof(straddr));
	if (rv == -1) {
		lprintf(LOG_WARNING, "could not send packet to UDP client %s:%d: %m\n",
			straddr, sockaddr.my_sin_port);
	} else if (rv != g_udp_client.size) {
		lprintf(LOG_WARNING, "could not send packet to UDP client %s:%d: "
			"only %d of %d bytes written\n", straddr,
			sockaddr.my_sin_port, rv, (int) g_udp_client.size);
	}
#ifdef DEBUG
	dump_packet(&g_udp_client);
#endif
}
#else
{
	struct my_sockaddr_t sockaddr;
	my_socklen_t socklen;
	int rv;
	char straddr[my_inet_addrstrlen];
    /* BEGIN: Added by zoucaihong, 2014/5/13 */
	UINT8   aucFromIp[4] = {0};
    /* END: Added by zoucaihong, 2014/5/13 */

	/* Read the whole UDP packet from the socket at once */
	socklen = sizeof (sockaddr);
	rv = recvfrom(g_udp_sockfd, g_udp_client.packet, sizeof (g_udp_client.packet),
		0, (struct sockaddr *)&sockaddr, &socklen);
	if (g_udp_client.size == -1) {
		lprintf(LOG_WARNING, "could not receive packet on UDP port %d: %m\n",
			g_udp_port);
		return;
	}
	g_udp_client.timestamp = time(NULL);
	g_udp_client.sockfd = g_udp_sockfd;
	g_udp_client.addr = sockaddr.my_sin_addr; //client的结构体存放的是CMS的ip和端口
	g_udp_client.port = sockaddr.my_sin_port;
	g_udp_client.size = rv;
	g_udp_client.outgoing = 0;
#ifdef DEBUG
	/*打印报文基本信息*/
	dump_packet(&g_udp_client);
#endif
    /* BEGIN: Added by zoucaihong, 2014/5/13 */
	memcpy(aucFromIp, (UINT8 *)(&sockaddr.my_sin_addr), sizeof(sockaddr.my_sin_addr));
    dev_get_cms_recvtime(aucFromIp);
    /* END: Added by zoucaihong, 2014/5/13 */

	/* Call the protocol handler which will prepare the response packet */
	inet_ntop(my_af_inet, &sockaddr.my_sin_addr, straddr, sizeof(straddr));
	/*处理收到的snmp request,并构建对应的response存放在client->packet*/
	if (snmp(&g_udp_client) == -1) {
		lprintf(LOG_WARNING, "could not handle packet from UDP client %s:%d: %m\n",
			straddr, sockaddr.my_sin_port);
		return;
	} else if (g_udp_client.size == 0) {
		lprintf(LOG_DEBUG, "could not handle packet from UDP client %s:%d: ignored\n",
			straddr, sockaddr.my_sin_port);
		return;
	}
    //agent_printf(LOG_ERR,"================TEST_SET\n");
	g_udp_client.outgoing = 1;

	/* Send the whole UDP packet to the socket at once */
	rv = sendto(g_udp_sockfd, g_udp_client.packet, g_udp_client.size,
		MSG_DONTWAIT, (struct sockaddr *)&sockaddr, socklen);
	inet_ntop(my_af_inet, &sockaddr.my_sin_addr, straddr, sizeof(straddr));
	if (rv == -1) {
    //agent_printf(LOG_ERR,"====could not send packet to UDP client==========TEST_SET\n");
		lprintf(LOG_WARNING, "could not send packet to UDP client %s:%d: %m\n",
			straddr, sockaddr.my_sin_port);
	} else if (rv != g_udp_client.size) {
    //agent_printf(LOG_ERR,"=======could not send packet to UDP client=========TEST_SET\n");
		lprintf(LOG_WARNING, "could not send packet to UDP client %s:%d: "
			"only %d of %d bytes written\n", straddr,
			sockaddr.my_sin_port, rv, (int) g_udp_client.size);
	}
    //agent_printf(LOG_ERR,"================TEST_SET\n");
#ifdef DEBUG
	dump_packet(&g_udp_client);
#endif
}
#endif

#if 0
static void handle_tcp_connect(void)
{
	struct my_sockaddr_t tmp_sockaddr;
	struct my_sockaddr_t sockaddr;
	my_socklen_t socklen;
	client_t *client;
	int rv;
	char straddr[my_inet_addrstrlen];

	/* Accept the new connection (remember the client's IP address and port) */
	socklen = sizeof (sockaddr);
	rv = accept(g_tcp_sockfd, (struct sockaddr *)&sockaddr, &socklen);
	if (rv == -1) {
		lprintf(LOG_ERR, "could not accept TCP connection: %m\n");
		return;
	} else if (rv >= FD_SETSIZE) {
		lprintf(LOG_ERR, "could not accept TCP connection: FD set overflow\n");
		close(rv);
		return;
	}

	/* Create a new client control structure or overwrite the oldest one */
	if (g_tcp_client_list_length >= MAX_NR_CLIENTS) {
		client = find_oldest_client();
		if (client == NULL) {
			lprintf(LOG_ERR, "could not accept TCP connection: internal error");
			exit(EXIT_SYSCALL);
		}
		tmp_sockaddr.my_sin_addr = client->addr;
		tmp_sockaddr.my_sin_port = client->port;
		inet_ntop(my_af_inet, &tmp_sockaddr.my_sin_addr, straddr, sizeof(straddr));
		lprintf(LOG_WARNING, "maximum number of %d clients reached, kicking out %s:%d\n",
			MAX_NR_CLIENTS, straddr, tmp_sockaddr.my_sin_port);
		close(client->sockfd);
	} else {
		client = malloc(sizeof (client_t));
		if (client == NULL) {
			lprintf(LOG_ERR, "could not accept TCP connection: %m");
			exit(EXIT_SYSCALL);
		}
		g_tcp_client_list[g_tcp_client_list_length++] = client;
	}

	/* Now fill out the client control structure values */
	inet_ntop(my_af_inet, &sockaddr.my_sin_addr, straddr, sizeof(straddr));
	lprintf(LOG_DEBUG, "connected TCP client %s:%d\n",
		straddr, sockaddr.my_sin_port);
	client->timestamp = time(NULL);
	client->sockfd = rv;
	client->addr = sockaddr.my_sin_addr;
	client->port = sockaddr.my_sin_port;
	client->size = 0;
	client->outgoing = 0;
}

static void handle_tcp_client_write(client_t *client)
{
	struct my_sockaddr_t sockaddr;
	int rv;
	char straddr[my_inet_addrstrlen];

	/* Send the packet atomically and close socket if that did not work */
	sockaddr.my_sin_addr = client->addr;
	sockaddr.my_sin_port = client->port;
	rv = send(client->sockfd, client->packet, client->size, 0);
	inet_ntop(my_af_inet, &sockaddr.my_sin_addr, straddr, sizeof(straddr));
	if (rv == -1) {
		lprintf(LOG_WARNING, "could not send packet to TCP client %s:%d: %m\n",
			straddr, sockaddr.my_sin_port);
		close(client->sockfd);
		client->sockfd = -1;
		return;
	} else if (rv != client->size) {
		lprintf(LOG_WARNING, "could not send packet to TCP client %s:%d: "
			"only %d of %d bytes written\n", straddr,
			sockaddr.my_sin_port, rv, (int) client->size);
		close(client->sockfd);
		client->sockfd = -1;
		return;
	}
#ifdef DEBUG
	dump_packet(client);
#endif

	/* Put the client into listening mode again */
	client->size = 0;
	client->outgoing = 0;
}

static void handle_tcp_client_read(client_t *client)
{
	struct my_sockaddr_t sockaddr;
	int rv;
	char straddr[my_inet_addrstrlen];

	/* Read from the socket what arrived and put it into the buffer */
	sockaddr.my_sin_addr = client->addr;
	sockaddr.my_sin_port = client->port;
	rv = read(client->sockfd, client->packet + client->size,
		sizeof (client->packet) - client->size);
	inet_ntop(my_af_inet, &sockaddr.my_sin_addr, straddr, sizeof(straddr));
	if (rv == -1) {
		lprintf(LOG_WARNING, "could not read packet from TCP client %s:%d: %m\n",
			straddr, sockaddr.my_sin_port);
		close(client->sockfd);
		client->sockfd = -1;
		return;
	} else if (rv == 0) {
		lprintf(LOG_DEBUG, "disconnected TCP client %s:%d\n",
			straddr, sockaddr.my_sin_port);
		close(client->sockfd);
		client->sockfd = -1;
		return;
	}
	client->timestamp = time(NULL);
	client->size += rv;

	/* Check whether the packet was fully received and handle packet if yes */
	rv = snmp_packet_complete(client);
	if (rv == -1) {
		lprintf(LOG_WARNING, "could not handle packet from TCP client %s:%d: %m\n",
			straddr, sockaddr.my_sin_port);
		close(client->sockfd);
		client->sockfd = -1;
		return;
	} else if (rv == 0) {
		return;
	}
	client->outgoing = 0;
#ifdef DEBUG
	dump_packet(client);
#endif

	/* Call the protocol handler which will prepare the response packet */
	if (snmp(client) == -1) {
		lprintf(LOG_WARNING, "could not handle packet from TCP client %s:%d: %m\n",
			straddr, sockaddr.my_sin_port);
		close(client->sockfd);
		client->sockfd = -1;
		return;
	} else if (client->size == 0) {
		lprintf(LOG_WARNING, "could not handle packet from TCP client %s:%d: ignored\n",
			straddr, sockaddr.my_sin_port);
		close(client->sockfd);
		client->sockfd = -1;
		return;
	}
	client->outgoing = 1;
}
#endif

/*****************************************************************************
 Prototype    : snmp_agent_destroy
 Description  : 释放内存
 Input        : VOID  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/4/18
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
VOID snmp_agent_destroy(VOID)
{
	/* free memory */	
	//SNMP_AGENT_FREE(g_pstSnmpAgentData->pstSystem);
	//SNMP_AGENT_FREE(g_pstSnmpAgentData->pstInterfaces);
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstSysDevInfo);
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstSysSwInfo);
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstSysHwInfo);
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstSysIpConfig);
	//SNMP_AGENT_FREE(g_pstSnmpAgentData->pstAdvance5GPrior);
	//SNMP_AGENT_FREE(g_pstSnmpAgentData->pstAdvance11nPrior);
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstApWifiBasicConfig);
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstApWifiChannelConfig);
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstApWifiPowerConfig);
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstApWifiAdvanceConfig);
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstApWifiBasic5Config);
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstApWifiChannel5Config);
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstApWifiPower5Config);
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstApWifiAdvance5Config);
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstApVapBasicConfig);
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstApVapSecurityConfig);
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstApAlarmConfig);
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstApTrapDesConfig);
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstApVlanConfig);
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstApSsidConfig);
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstSysStat);
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstApWifiStat);
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstApWifiVapStat);
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstApStationStat);
#if 0
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstApBrModeSta);
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstAp24CoChaInterfInfo);
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstAp5CoChaInterfInfo);
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstAp24AdjChaInterfInfo);
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstAp5AdjChaInterfInfo);
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstAp24WlanDevInterfInfo);
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstAp5WlanDevInterfInfo);
	SNMP_AGENT_FREE(g_pstSnmpAgentData->pstApNonWlanDevInterfInfo);
#endif

    return;
}

BOOL agent_dev_state_change(DEV_STATE_E enTarget)
{
    if(enTarget >= DEV_STATE_MAX)
    {
    	agent_printf(LOG_ERR,"func:%s enTarget beyond %u",
            __func__, enTarget);
        return FALSE;
    }
    if ( enTarget != g_enDevState )
    {
        agent_printf(LOG_NOTICE,"AP state ==========from %s to %s"
            ,g_aucStateName[g_enDevState]
            ,g_aucStateName[enTarget]);
        /* BEGIN: Added by zoucaihong, 2014/6/27 */
        /* 离线告警 */
        if ( DEV_STATE_RUNNING == g_enDevState )
        {
            agent_send_dev_offline_trap();
            g_enDevState=enTarget;
            return TRUE;
        }
        /* END: Added by zoucaihong, 2014/6/27 */
    }
    /*state change*/
    g_enDevState=enTarget;
	switch(enTarget)
	{
		case DEV_STATE_DISCOVER:
            g_pstSnmpAgentData->pstSysDevInfo->ulSysManageState = MANAGE_STATE_OFFLINE;
            agent_start_discover();
            break;
		case DEV_STATE_NONE:
		case DEV_STATE_RUNNING:
			break;
		default:
			break;
	}
    return TRUE;
}

/*****************************************************************************
 Prototype    : agent_check_dev_state
 Description  : 检查设备与CMS的状态并做相应的切换
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/5/13
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL agent_check_dev_state(VOID)
{
    UINT32 ulTimeOutInterval, ulHearBeatPeriod;
    if ( g_enDevState > DEV_STATE_RUNNING )
    {
        agent_printf(LOG_ERR, "func:%s g_enDevState:%d is invalid.\n",
            __func__, g_enDevState);
        return FALSE;
    }

    dev_get_cms_timeout();

    if ( NULL != g_pstSnmpAgentData && 
        NULL != g_pstSnmpAgentData->pstSysDevInfo )
    {
        ulTimeOutInterval = g_pstSnmpAgentData->pstSysDevInfo->ulTimeOutInterval;
        if ( 0 == ulTimeOutInterval )
        {
            ulTimeOutInterval = DEV_CMS_TIMEOUT;
        }
        ulHearBeatPeriod = g_pstSnmpAgentData->pstSysDevInfo->ulHeartBeatPeriod;
        if ( 0 == ulHearBeatPeriod )
        {
            ulHearBeatPeriod = DEV_HEART_PERIOD;
        }
    }
    else
    {
        ulTimeOutInterval = DEV_CMS_TIMEOUT;
        ulHearBeatPeriod = DEV_HEART_PERIOD;
    }
    
    if( (DEV_STATE_NONE == g_enDevState)/* 设备开始发现CMS */
        || ((DEV_STATE_DISCOVER == g_enDevState)&& 
           (0 == (g_ulDisTicks%DEV_DISCOVER_PERIOD)))/* 设备发送discover后CMS未响应设置设备状态为running */
        || ((DEV_STATE_RUNNING == g_enDevState)&&
            (g_ulRecvTimeout>=ulTimeOutInterval)) /* CMS报文超时 */
        )
    {
        agent_dev_state_change(DEV_STATE_DISCOVER);
    }
    else if((DEV_STATE_RUNNING == g_enDevState)&&
        (0 == (g_ulHeartTicks%ulHearBeatPeriod)))/* 发送心跳 */
    {
        agent_send_heart_beat_trap();
    }

    return TRUE;
}

/* -----------------------------------------------------------------------------
 * Main program
 */

int main(int argc, char *argv[])
{
	static const char short_options[] = "p:P:c:w:D:V:L:C:d:i:I:t:T:s:S:avlh";
	static const struct option long_options[] = {
		{ "udp-port", 1, 0, 'p' },
		{ "tcp-port", 1, 0, 'P' },
		{ "ro-community", 1, 0, 'c' },
		{ "rw-community", 1, 0, 'w' },
		{ "description", 1, 0, 'D' },
		{ "vendor", 1, 0, 'V' },
		{ "location", 1, 0, 'L' },
		{ "contact", 1, 0, 'C' },
		{ "disks", 1, 0, 'd' },
		{ "interfaces", 1, 0, 'i' },
		{ "listen", 1, 0, 'I' },
		{ "timeout", 1, 0, 't' },
		{ "traps", 1, 0, 'T' },
		{ "statistics-cycle", 1, 0, 's' },
		{ "trapswitch", 1, 0, 'S'},
		{ "auth", 0, 0, 'a' },
		{ "verbose", 0, 0, 'v' },
		{ "licensing", 0, 0, 'l' },
		{ "help", 0, 0, 'h' },
		{ NULL, 0, 0, 0 }
	};
	int option_index = 1;
	int c;

	struct my_sockaddr_t sockaddr;
	my_socklen_t socklen;
	struct timeval tv_last;
	struct timeval tv_now;
	struct timeval tv_sleep;
	struct ifreq ifreq;
	int ticks;
	fd_set rfds;
	fd_set wfds;
	int nfds;
	//int i;
    BOOL bCheckFlag = FALSE;

	/* Prevent TERM and HUP signals from interrupting system calls */
	signal(SIGTERM, handle_signal);
	signal(SIGHUP, handle_signal);
	siginterrupt(SIGTERM, 0);
	siginterrupt(SIGHUP, 0);

	/* Open the syslog connection if needed */
#ifdef SYSLOG
	openlog("mini_snmpd", LOG_CONS | LOG_PID, LOG_DAEMON);
#endif

	/* Parse commandline options */
	while (1) {
		c = getopt_long(argc, argv, short_options, long_options, &option_index);
		if (c == -1) {
			break;
		}
		switch (c) {
			case 'p':
				g_udp_port = atoi(optarg);
				break;
			case 'P':
				g_tcp_port = atoi(optarg);
				break;
			case 'c':
				g_rocommunity = strdup(optarg);
				break;
			case 'w':
				g_rwcommunity = strdup(optarg);
				break;
			case 'D':
				g_description = strdup(optarg);
				break;
			case 'V':
				g_vendor = strdup(optarg);
				break;
			case 'L':
				g_location = strdup(optarg);
				break;
			case 'C':
				g_contact = strdup(optarg);
				break;
			case 'I':
				g_bind_to_device = strdup(optarg);
				break;
			case 'd':
				g_disk_list_length = split(optarg, ",:;", g_disk_list, MAX_NR_DISKS);
				break;
			case 'i':
				g_interface_list_length = split(optarg, ",:;", g_interface_list, MAX_NR_INTERFACES);
				break;
		    /* BEGIN: Added by Zhou Shouya, 2013/4/2 */
		    case 's':
		        g_stati_per=atoi(optarg);
		        break;
		    case 'S':
		        g_traps_switch = atoi(optarg);
		        break;
		    /* END:   Added by Zhou Shouya, 2013/4/2 */
			case 't':
				g_timeout = atoi(optarg) * 100;
				break;
			case 'a':
				g_auth = 1;
				break;
			case 'v':
				g_verbose = 1;
				break;
			case 'l':
				print_version();
				exit(EXIT_ARGS);
				break;
			default:
				print_help();
				exit(EXIT_ARGS);
		}
	}

	/* Print a starting message (so the user knows the args were ok) */
	if (g_bind_to_device[0] != '\0') {
		lprintf(LOG_INFO, "started, listening on port %d/udp and %d/tcp on interface %s\n",
			g_udp_port, g_tcp_port, g_bind_to_device);
	} else {
		lprintf(LOG_INFO, "started, listening on port %d/udp and %d/tcp\n",
			g_udp_port, g_tcp_port);
	}


    snmp_log_set(SNMP_AGENT_LOG, ">>>>>>>>>>start snmp_agentd");
        

	snmp_agent_init();
	//test_snmp_trap();
	/* BEGIN: Modified by zoucaihong, 2013/9/22 */
	#if 0
	/* BEGIN: Added by zoucaihong, 2012/11/12 */
	/*告警消息处理函数*/
    snmp_agent_alarm_msg_handler();
	/* END: Added by zoucaihong, 2012/11/12 */
    #endif
    /* 创建线程，处理消息队列 */
	/* 用线程实现定时器 */
	snmp_agent_timer();
	/* END: Modified by zoucaihong, 2013/9/22 */
	/* 2013-05-16 fengjing add */
	get_cpu_endian();

	/* Store the starting time since we need it for MIB updates */
	if (gettimeofday(&tv_last, NULL) == -1) {
		memset(&tv_last, 0, sizeof (tv_last));
		memset(&tv_sleep, 0, sizeof (&tv_sleep));
	} else {
		tv_sleep.tv_sec = g_timeout / 100;
		tv_sleep.tv_usec = (g_timeout % 100) * 10000;
	}

    #if 0
	/* Build the MIB and execute the first MIB update to get actual values */
	if (mib_build() == -1) {
		exit(EXIT_SYSCALL);
	} else if (mib_update(1) == -1) {
		exit(EXIT_SYSCALL);
	}
    
#ifdef DEBUG
	dump_mib(g_mib, g_mib_length);
#endif
    #endif

    /* BEGIN: Added by zoucaihong, 2013/9/22 */
	/* Open the server's UDP port and prepare it for listening */
	g_alarm_sockfd = socket(my_pf_inet, SOCK_DGRAM, 0);
	if (g_alarm_sockfd == -1) {
		lprintf(LOG_ERR, "could not create UDP socket: %m\n");
		exit(EXIT_SYSCALL);
	}
	sockaddr.my_sin_family = my_af_inet;
	sockaddr.my_sin_port = htons(g_alarm_port);
	sockaddr.my_sin_addr = my_inaddr_any;
	socklen = sizeof (sockaddr);

    BOOL bFlag = TRUE;
    if (setsockopt(g_alarm_sockfd, SOL_SOCKET, SO_REUSEADDR, &bFlag, sizeof(bFlag)) == -1) 
	{
		lprintf(LOG_ERR, "setsockopt SO_REUSEADDR error.\n");
		exit(EXIT_SYSCALL);
	}
	if (bind(g_alarm_sockfd, (struct sockaddr *)&sockaddr, socklen) == -1) {
		lprintf(LOG_ERR, "could not bind UDP socket to port %d: %m\n", g_alarm_port);
		exit(EXIT_SYSCALL);
	}
    /* END: Added by zoucaihong, 2013/9/22 */

	/* Open the server's UDP port and prepare it for listening */
	g_udp_sockfd = socket(my_pf_inet, SOCK_DGRAM, 0);
	if (g_udp_sockfd == -1) {
		lprintf(LOG_ERR, "could not create UDP socket: %m\n");
		exit(EXIT_SYSCALL);
	}
	sockaddr.my_sin_family = my_af_inet;
	sockaddr.my_sin_port = htons(g_udp_port);
	sockaddr.my_sin_addr = my_inaddr_any;
	socklen = sizeof (sockaddr);
    if (setsockopt(g_udp_sockfd, SOL_SOCKET, SO_REUSEADDR, &bFlag, sizeof(bFlag)) == -1) 
	{
		lprintf(LOG_ERR, "setsockopt SO_REUSEADDR error.\n");
		exit(EXIT_SYSCALL);
	}
	if (bind(g_udp_sockfd, (struct sockaddr *)&sockaddr, socklen) == -1) {
		lprintf(LOG_ERR, "could not bind UDP socket to port %d: %m\n", g_udp_port);
		exit(EXIT_SYSCALL);
	}
	if (g_bind_to_device[0] != '\0') {
		snprintf(ifreq.ifr_ifrn.ifrn_name, sizeof (ifreq.ifr_ifrn.ifrn_name), "%s", g_bind_to_device);
		if (setsockopt(g_udp_sockfd, SOL_SOCKET, SO_BINDTODEVICE, (char *)&ifreq, sizeof(ifreq)) == -1) {
			lprintf(LOG_WARNING, "could not bind UDP socket to device %s: %m\n", g_bind_to_device);
		    exit(EXIT_SYSCALL);    
		}
	}

    /* BEGIN: Modified by zoucaihong, 2013/9/22 */
    #if 0
	/* Open the server's TCP port and prepare it for listening */
	g_tcp_sockfd = socket(my_pf_inet, SOCK_STREAM, 0);
	if (g_tcp_sockfd == -1) {
		lprintf(LOG_ERR, "could not create TCP socket: %m\n");
		exit(EXIT_SYSCALL);
	}
	if (g_bind_to_device[0] != '\0') {
		snprintf(ifreq.ifr_ifrn.ifrn_name, sizeof (ifreq.ifr_ifrn.ifrn_name), "%s", g_bind_to_device);
		if (setsockopt(g_tcp_sockfd, SOL_SOCKET, SO_BINDTODEVICE, (char *)&ifreq, sizeof(ifreq)) == -1) {
			lprintf(LOG_WARNING, "could not bind TCP socket to device %s: %m\n", g_bind_to_device);
		    exit(EXIT_SYSCALL);    
		}
	}
	i = 1;
	if (setsockopt(g_tcp_sockfd, SOL_SOCKET, SO_REUSEADDR, &c, sizeof (i)) == -1) {
		lprintf(LOG_WARNING, "could not set SO_REUSEADDR on TCP socket: %m\n");
		exit(EXIT_SYSCALL);
	}
	sockaddr.my_sin_family = my_af_inet;
	sockaddr.my_sin_port = htons(g_tcp_port);
	sockaddr.my_sin_addr = my_inaddr_any;
	socklen = sizeof (sockaddr);
	if (bind(g_tcp_sockfd, (struct sockaddr *)&sockaddr, socklen) == -1) {
		lprintf(LOG_ERR, "could not bind TCP socket to port %d: %m\n", g_tcp_port);
		exit(EXIT_SYSCALL);
	}
	if (listen(g_tcp_sockfd, 128) == -1) {
		lprintf(LOG_ERR, "could not prepare TCP socket for listening: %m\n");
		exit(EXIT_SYSCALL);
	}
    #endif
    /* END: Modified by zoucaihong, 2013/9/22 */

	/* Handle incoming connect requests and incoming data */
	while (!g_quit) {
        /* BEGIN: Added by zoucaihong, 2014/5/13 */
        /*检查AP状态，并做相应切换，1秒检查1次*/
        if ( bCheckFlag )
        {
            agent_check_dev_state();
        }
        /* END: Added by zoucaihong, 2014/5/13 */
        
		/* Sleep until we get a request or the timeout is over */
		FD_ZERO(&rfds);
		FD_ZERO(&wfds);
		FD_SET(g_udp_sockfd, &rfds);
        /* BEGIN: Modified by zoucaihong, 2013/9/22 */
        #if 0
		FD_SET(g_tcp_sockfd, &rfds);
        #endif
        /* END: Modified by zoucaihong, 2013/9/22 */
        /* BEGIN: Added by zoucaihong, 2013/9/22 */
		FD_SET(g_alarm_sockfd, &rfds);
		//nfds = (g_udp_sockfd > g_tcp_sockfd) ? g_udp_sockfd : g_tcp_sockfd;
		nfds = (g_udp_sockfd > g_alarm_sockfd) ? g_udp_sockfd : g_alarm_sockfd;
        #if 0
		for (i = 0; i < g_tcp_client_list_length; i++) {
			if (g_tcp_client_list[i]->outgoing) {
				FD_SET(g_tcp_client_list[i]->sockfd, &wfds);
			} else {
				FD_SET(g_tcp_client_list[i]->sockfd, &rfds);
			}
			if (nfds < g_tcp_client_list[i]->sockfd) {
				nfds = g_tcp_client_list[i]->sockfd;
			}
		}
        #endif
        /* END: Added by zoucaihong, 2013/9/22 */
		if (select(nfds + 1, &rfds, &wfds, NULL, &tv_sleep) == -1) {
			if (g_quit) {
				break;
			}
			lprintf(LOG_ERR, "could not select from sockets: %m\n");
			exit(EXIT_SYSCALL);
		}
		/* Determine whether to update the MIB and the next ticks to sleep */
		ticks = ticks_since(&tv_last, &tv_now);
		if (ticks < 0 || ticks >= g_timeout) {
			/*lprintf(LOG_DEBUG, "updating the MIB (full)\n");
			if (mib_update(1) == -1) {
				exit(EXIT_SYSCALL);
			}*/
			memcpy(&tv_last, &tv_now, sizeof (tv_now));
			tv_sleep.tv_sec = g_timeout / 100;
			tv_sleep.tv_usec = (g_timeout % 100) * 10000;
            TICKS_UP();
            bCheckFlag = TRUE;
		} else {
			/*lprintf(LOG_DEBUG, "updating the MIB (partial)\n");
			if (mib_update(0) == -1) {
				exit(EXIT_SYSCALL);
			}*/
			tv_sleep.tv_sec = (g_timeout - ticks) / 100;
			tv_sleep.tv_usec = ((g_timeout - ticks) % 100) * 10000;
            bCheckFlag = FALSE;
		}
        
#ifdef DEBUG
		//dump_mib(g_mib, g_mib_length);
#endif

		/* Handle UDP packets, TCP packets and TCP connection connects */
		if (FD_ISSET(g_udp_sockfd, &rfds)) {
			handle_udp_client();
		}
        /* BEGIN: Added by zoucaihong, 2013/9/22 */
		if (FD_ISSET(g_alarm_sockfd, &rfds)) {
			handle_alarm_msg();
		}
        /* END: Added by zoucaihong, 2013/9/22 */
        /* BEGIN: Modified by zoucaihong, 2013/9/22 */
        #if 0
		if (FD_ISSET(g_tcp_sockfd, &rfds)) {
			handle_tcp_connect();
		}
		for (i = 0; i < g_tcp_client_list_length; i++) {
			if (g_tcp_client_list[i]->outgoing) {
				if (FD_ISSET(g_tcp_client_list[i]->sockfd, &wfds)) {
					handle_tcp_client_write(g_tcp_client_list[i]);
				}
			} else {
				if (FD_ISSET(g_tcp_client_list[i]->sockfd, &rfds)) {
					handle_tcp_client_read(g_tcp_client_list[i]);
				}
			}
		}
		/* If there was a TCP disconnect, remove the client from the list */
		for (i = 0; i < g_tcp_client_list_length; i++) {
			if (g_tcp_client_list[i]->sockfd == -1) {
				g_tcp_client_list_length--;
				if (i < g_tcp_client_list_length) {
					free(g_tcp_client_list[i]);
					memmove(&g_tcp_client_list[i], &g_tcp_client_list[i + 1],
						(g_tcp_client_list_length - i) * sizeof (g_tcp_client_list[i]));
				}
			}
		}
        #endif
        /* END: Modified by zoucaihong, 2013/9/22 */
	}

	/* We were killed, print a message and exit */
	lprintf(LOG_INFO, "stopped\n");
    /* BEGIN: Added by zoucaihong, 2013/4/18 */
	snmp_agent_destroy();
    /* END: Added by zoucaihong, 2013/4/18 */
	return EXIT_OK;
}



/* vim: ts=4 sts=4 sw=4 nowrap
 */
