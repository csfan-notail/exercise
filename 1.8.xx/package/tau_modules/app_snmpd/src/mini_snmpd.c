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

#include "snmp_nms.h"
#include "mini_snmpd.h"
#include "my_snmp_def.h"
#include "my_snmp_data.h"
#include "my_snmp_info.h"
#include "my_snmp_handle.h"
#include "my_snmp_util.h"
#include "my_snmp_alarm.h"
#include "my_snmp_trap.h"

#include "pal_include/pal_uci.h"
#include "uci_config.h"
//#include "nvram.h"
//#include "nvram_define.h"


OSTOKEN gtSemConfig;	/* 配置互斥信号量 */
OSTOKEN gtSemStat;      /* 统计互斥信号量 */
OSTOKEN gtSemUdp;       /* UDP报文队列互斥信号量 */
OSTOKEN gtSemAlarm;     /* 告警互斥信号量 */

//解决set操作和get操作冲突
OSTOKEN gtSemSnmp;      /* set和get操作互斥信号量 */

INT32 g_iSnmpLogLevel = LOG_INFO;

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
BOOL     g_ulInitState;

UINT32      g_ulEndian = 0;

QUEUE_T g_stUdpQueue;
extern AP_QUEUE_T      g_alarmQueue;

UINT8 g_ucCmsIP[4]={0};


/******************************************************************************/
extern BOOL snmp_init_tables_info(VOID);
UINT32 app_get_uptime(void);

DEV_STATE_E         g_enCpeState=DEV_STATE_NONE;

UINT32          g_uiSendDiscoverCount = 1;  /* send discover trap count */
UINT32          g_uiSendDiscoverTime  = 3;//48;  /* send discover trap interval default 240s */

UINT32          g_uiSendHeartCount = 1;     /* send heart trap count */
UINT32          g_ulSendHeartTime = 8;      /* device heart trap interval default 30s */

UINT32          g_uiOfflineCount = 1;       /* device offline count */
UINT32          g_uiOfflineTime = 8;        /* device offline interval default 30s */

UINT32          g_uiOfflineFlag = 3;        /* 设备离线标志 2:表示上线标志 1:表示离线标志 */

//UINT32          g_ulRecvTimeout=0;          /* 设备与CMS保活超时时间*/
UINT32          g_ulCmsRecvTime = 0;        /* 记录设备正确接收到CMS发送的SNMP报文的时间 */
#define DEV_DISCOVER_PERIOD      60

#define DEV_CMS_TIMEOUT          30
UINT32          g_uiOfflineTrapTime = DEV_CMS_TIMEOUT; /* device offline interval */

AGENT_TBLMD_MAP_T astMap[] =
{	
    /* Sys Info */
	{TABLE_NAME_SYS_DEV_INFO,           snmp_sys_dev_info,                  0},
	{TABLE_NAME_SYS_SW_INFO,            snmp_sys_sw_info,                   0},
	{TABLE_NAME_SYS_HW_INFO,            snmp_sys_hardware_info,             0},
    {TABLE_NAME_SYS_MANAGE_INFO,        snmp_sys_manage_info,               0},
    {TABLE_NAME_SYS_PROTO_INFO,         snmp_sys_proto_info,                0},

    /* Lte Info */
    {TABLE_NAME_LTE_HW_INFO,            snmp_lte_hw_info,              0},
    {TABLE_NAME_LTE_NET_INFO,           snmp_lte_net_info,                0},
    {TABLE_NAME_LTE_CFG_INFO,           snmp_lte_cfg_info,                  0},

    /* Wifi Info */
    {TABLE_NAME_LTE_WIFI_INFO,          snmp_lte_wifi_info,                 0},

    /* Wan State Info */
    {TABLE_NAME_WAN_STAT,               snmp_lte_wan_state_info,            0},
	{ "", NULL, 0 }
};

/************************************************************************/
/*                                                                      */
/************************************************************************/

/*****************************************************************************
 函 数 名  : snmp_init_community
 功能描述  : 初始化共同体
 输入参数  : VOID  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年10月30日 星期二
    作    者   : 李文
    修改内容   : 新生成函数

*****************************************************************************/
BOOL snmp_init_community(VOID)
{

    UINT8 *pucMac = g_pstSnmpAgentData->stManageInfo.acManageMac;
    g_rwcommunity=(char *)malloc(6*2+1);
    memset(g_rwcommunity,0,6*2+1);
    sprintf(g_rwcommunity,"%02X:%02X:%02X:%02X:%02X:%02X",pucMac[0],pucMac[1]
        ,pucMac[2],pucMac[3],pucMac[4],pucMac[5]);
    //printf("g_rwcommunity==%s\n",g_rwcommunity);
    
	return TRUE;
}


BOOL snmp_init_sem(VOID)
{
	/* 创建信号量 */
	if (!OsCreateSemaphore(NULL, 1, 1, SEM_MUTEX, &gtSemConfig))
	{
		snmp_log(LOG_ERR, "snmp_init_data create config semux failed\n");
		return FALSE;
	}
	
	if (!OsCreateSemaphore(NULL, 1, 1, SEM_MUTEX, &gtSemStat))
	{
		snmp_log(LOG_ERR, "snmp_init_data create stat semux failed\n");
		return FALSE;
	}

    /* 创建UDP报文队列互斥信号量 */
	if (!OsCreateSemaphore(NULL, 1, 1, SEM_MUTEX, &gtSemUdp))
	{
		return FALSE;
	}
    
    /* 创建告警队列互斥信号量 */
	if (!OsCreateSemaphore(NULL, 1, 1, SEM_MUTEX, &gtSemAlarm))
	{
		return FALSE;
	}

    //解决set操作和get操作冲突
    if (!OsCreateSemaphore(NULL, 1, 1, SEM_MUTEX, &gtSemSnmp))
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL snmp_init_customer(VOID)
{
	CHAR   *pos = NULL;
	CHAR   acValue[16] = {0};
	UINT32 ulIndex = 0;
	UINT32 ulIndex2 = 0;
	
	sprintf(g_acRootOID, "%s", "1.3.6.1.4.1.28723");
	sprintf(g_acVendor, "%s", "gbcom");

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
    BOOL bRet = FALSE;
	AGENT_TBLMD_MAP_T *hp = NULL;
    
	snmp_get_uptime_10ms();
	snmp_get_uptime();
    

	g_pstSnmpAgentData = SNMP_AGENT_MALLOC(sizeof(SNMP_AGENT_DATA_T));
	if (!g_pstSnmpAgentData)
	{
		snmp_log(LOG_ERR,  "malloc SNMP_AGENT_DATA_T failed\n");
		return FALSE;
	}
	memset(g_pstSnmpAgentData, 0, sizeof(SNMP_AGENT_DATA_T));
    snmp_sys_lte_info_init();

	/* init snmp agent data */
	if ( FALSE == g_ulInitState)
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
				snmp_log(LOG_INFO, "get %s table record succeed\n",
						hp->acTblName);
				hp->ulInitFlag = 1;
			}
			else
			{
				snmp_log(LOG_ERR, "get %s table record failed\n",
						hp->acTblName);
			}
		}

#if 0
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
#endif
        g_ulInitState = TRUE;
    }
	
	return TRUE;
}
/* BEGIN: Added by zoucaihong, 2013/9/24 */
UINT32 ap_init_udp_queue(QUEUE_T *queue, UINT32 nodeNum, UINT32 dataSize)
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
        return AP_FAIL;
    }

    /* alloc memory for node list */
    queue->ulNodeNum = nodeNum;
    queue->ulNodeDataSize = dataSize;

    memAddr = SNMP_AGENT_MALLOC( (nodeSize) * nodeNum );
    if (NULL == memAddr)
    {
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
    
    }

    return ret;
}
/* END: Added by zoucaihong, 2013/9/24 */
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
	
	/* init agent data */
	if (!snmp_init_data())
	{
		agent_printf(LOG_ERR, "snmp_init_data failed\n");
		return FALSE;
	}
	agent_printf(LOG_INFO, "snmp_init_data succeed\n");

#if 1
    if (!snmp_init_community())//初始化g_rwcommunity共同体的值为cpe管理口的mac地址，管理信息报文包括:共同体名(或者还加上其他表示信息)  + 数据
    {
        agent_printf(LOG_ERR, "snmp_init_community failed\n");
        return FALSE;
    }
#endif

    /* init alarm queue */
    if ( AP_OK != snmp_init_alarm_queue() )
    {
		agent_printf(LOG_ERR, "snmp_init_alarm_queue failed\n");
		return FALSE;
    }

    /* init udp queue */
    if ( AP_OK != snmp_init_udp_queue() )
    {
		agent_printf(LOG_ERR, "snmp_init_udp_queue failed\n");
		return FALSE;
    }
	
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
            if ( AP_FAIL == bRet )
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

void dev_get_cms_recvtime()
{
    g_ulCmsRecvTime = app_get_uptime();

    return;
}

static void handle_udp_client(void)
{
	struct my_sockaddr_t sockaddr;
	my_socklen_t socklen;
	int rv;
	char straddr[my_inet_addrstrlen];
	int iRet;

	/* Read the whole UDP packet from the socket at once */
	socklen = sizeof (sockaddr);
	rv = recvfrom(g_udp_sockfd, g_udp_client.packet, sizeof (g_udp_client.packet),
		0, (struct sockaddr *)&sockaddr, &socklen);
	if (g_udp_client.size == -1) 
	{
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

	//dump_packet(&g_udp_client);

    dev_get_cms_recvtime();
    
	/* Call the protocol handler which will prepare the response packet */
	inet_ntop(my_af_inet, &sockaddr.my_sin_addr, straddr, sizeof(straddr));

    /*处理收到的snmp request,并构建对应的response存放在client->packet*/
    iRet = snmp(&g_udp_client);
    if (-2 == iRet)     /* heart response */
    {
        return; 
    }
    
	if (iRet == -1) 
	{
		lprintf(LOG_WARNING, "could not handle packet from UDP client %s:%d: %m\n",
			straddr, sockaddr.my_sin_port);
		return;
	}
	else if (g_udp_client.size == 0) 
	{
		lprintf(LOG_WARNING, "could not handle packet from UDP client %s:%d: ignored\n",
			straddr, sockaddr.my_sin_port);
		return;
	}
	g_udp_client.outgoing = 1;

	/* Send the whole UDP packet to the socket at once */
	rv = sendto(g_udp_sockfd, g_udp_client.packet, g_udp_client.size,
		MSG_DONTWAIT, (struct sockaddr *)&sockaddr, socklen);
	inet_ntop(my_af_inet, &sockaddr.my_sin_addr, straddr, sizeof(straddr));
	if (rv == -1) 
	{
		lprintf(LOG_WARNING, "could not send packet to UDP client %s:%d: %m\n",
			straddr, sockaddr.my_sin_port);
	} 
	else if (rv != g_udp_client.size) 
	{
		lprintf(LOG_WARNING, "could not send packet to UDP client %s:%d: "
			"only %d of %d bytes written\n", straddr,
			sockaddr.my_sin_port, rv, (int) g_udp_client.size);
	}
	
    #ifdef DEBUG
	dump_packet(&g_udp_client);
    #endif
}



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

    return;
}
UINT32 app_get_uptime(void)
{
    struct sysinfo info     = {0};
    int iRet                = -1;

    iRet = sysinfo(&info);

    if (0 == iRet)
    {
        //agent_printf(LOG_INFO, "the os uptime is %ld", info.uptime);
        return info.uptime;
    }
    else
    {
        agent_printf(LOG_ERROR, "get os uptime fail");
        return 0;
    }
    
}

#if 0
void dev_get_cms_timeout()
{
    UINT32  ulCurTime = 0;
    
    ulCurTime = app_get_uptime();
    g_ulRecvTimeout = ulCurTime - g_ulCmsRecvTime;

    return;
}
#endif

BOOL agent_check_dev_state(VOID)
{
    BOOL bRet;
    
    //dev_get_cms_timeout();
    
    if( (DEV_STATE_NONE == g_enCpeState)/* beginning to find CMS */
        || ((DEV_STATE_DISCOVER == g_enCpeState)&& 
           (0 == (g_uiSendDiscoverCount % g_uiSendDiscoverTime))) )/* device is offline, send trap to find CMS */
    {
        snmp_log(LOG_INFO, "Send trap again! g_enCpeState=%d\n", g_enCpeState);
        
		
        bRet = snmp_send_discover_trap(g_pstSnmpAgentData);
        if (FALSE == bRet)
        {
            snmp_log(LOG_INFO, "Send trap error! State=%d Count=%d Time=%d\n", g_enCpeState, g_uiSendDiscoverCount, g_uiSendDiscoverTime);
            return bRet;
        }
    }
    
    return TRUE;
}

BOOL get_cms_server_ip(void)
{
    CHAR acNvramValue[128];
    OM_ERROR_CODE_E enRet;
    UINT32 uiHostAddr;
    struct in_addr stInAddr;
    CHAR acIpAddr[20];
    CHAR acCmd[64];

	uci_config_get(UCI_CONFIG_SNMP_TRAP_IP,acNvramValue);
	
	//printf("[%25s|%30s:%5d] gateway:trap_ip=%s \n",__FILE__,__FUNCTION__,__LINE__,acNvramValue);	
    if (1 == is_ip_address(acNvramValue))
    {
        ipToNum(acNvramValue, g_ucCmsIP);
    }
    else
    {
        if (TRUE == get_ip_by_url(acNvramValue, &uiHostAddr))
        {
            stInAddr.s_addr = uiHostAddr;
            strncpy(acIpAddr, inet_ntoa(stInAddr), sizeof(acIpAddr));
            ipToNum(acIpAddr, g_ucCmsIP);  
        }
        else
        {
            printf("Invalid URL\n");
            return FALSE;
        }
    }
    return TRUE;

}

/* -----------------------------------------------------------------------------
 * Main program
 */

int main(int argc, char *argv[])
{
	static const char short_options[] = "p:P:c:w:D:V:L:C:d:i:I:t:T:s:S:avlh";
	static const struct option long_options[] = 
	{
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

	/* Parse commandline options */
	while (1) 
	{
		c = getopt_long(argc, argv, short_options, long_options, &option_index);
		if (c == -1) 
		{
			break;
		}
		switch (c) 
		{
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
		    case 's':
		        g_stati_per=atoi(optarg);
		        break;
		    case 'S':
		        g_traps_switch = atoi(optarg);
		        break;
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
	if (g_bind_to_device[0] != '\0') 
	{
		snmp_log(LOG_INFO, "started, listening on port %d/udp and %d/tcp on interface %s\n",
			g_udp_port, g_tcp_port, g_bind_to_device);
	} else {
		snmp_log(LOG_INFO, "started, listening on port %d/udp and %d/tcp\n",
			g_udp_port, g_tcp_port);
	}

    //sleep(10);  /* Just for get accurate info, optimization later */

//    om_rmtmgr_state_offline();
    
	snmp_agent_init();

	/* Store the starting time since we need it for MIB updates */
	if (gettimeofday(&tv_last, NULL) == -1) 
	{
		memset(&tv_last, 0, sizeof (tv_last));
		memset(&tv_sleep, 0, sizeof (&tv_sleep));
	} 
	else 
	{
		tv_sleep.tv_sec = 5;
		tv_sleep.tv_usec = (g_timeout % 100) * 10000;
	}
    
	/* Open the server's UDP port and prepare it for listening */
	g_alarm_sockfd = socket(my_pf_inet, SOCK_DGRAM, 0);
	if (g_alarm_sockfd == -1) 
	{
		lprintf(LOG_ERR, "could not create UDP socket: %m\n");
		exit(EXIT_SYSCALL);
	}
	sockaddr.my_sin_family = my_af_inet;
	sockaddr.my_sin_port = htons(g_alarm_port);
	sockaddr.my_sin_addr = my_inaddr_any;
	socklen = sizeof (sockaddr);
	if (bind(g_alarm_sockfd, (struct sockaddr *)&sockaddr, socklen) == -1) 
	{
		lprintf(LOG_ERR, "could not bind UDP socket to port %d: %m\n", g_alarm_port);
		exit(EXIT_SYSCALL);
	}

    BOOL bFlag = TRUE;
    if (setsockopt(g_alarm_sockfd, SOL_SOCKET, SO_REUSEADDR, &bFlag, sizeof(bFlag)) == -1) 
	{
		lprintf(LOG_ERR, "setsockopt SO_REUSEADDR error.\n");
		exit(EXIT_SYSCALL);
	}

	/* Open the server's UDP port and prepare it for listening */
	g_udp_sockfd = socket(my_pf_inet, SOCK_DGRAM, 0);
	if (g_udp_sockfd == -1) 
	{
		lprintf(LOG_ERR, "could not create UDP socket: %m\n");
		exit(EXIT_SYSCALL);
	}

	sockaddr.my_sin_family = my_af_inet;
	sockaddr.my_sin_port = htons(g_udp_port);
	sockaddr.my_sin_addr = my_inaddr_any;
	socklen = sizeof (sockaddr);
	if (bind(g_udp_sockfd, (struct sockaddr *)&sockaddr, socklen) == -1) 
	{
		lprintf(LOG_ERR, "could not bind UDP socket to port %d: %m\n", g_udp_port);
		exit(EXIT_SYSCALL);
	}
	if (g_bind_to_device[0] != '\0') 
	{
		snprintf(ifreq.ifr_ifrn.ifrn_name, sizeof (ifreq.ifr_ifrn.ifrn_name), "%s", g_bind_to_device);
		if (setsockopt(g_udp_sockfd, SOL_SOCKET, SO_BINDTODEVICE, (char *)&ifreq, sizeof(ifreq)) == -1) {
			lprintf(LOG_WARNING, "could not bind UDP socket to device %s: %m\n", g_bind_to_device);
		    exit(EXIT_SYSCALL);    
		}
	}

	if(FALSE == get_cms_server_ip())
	{
        snmp_log(LOG_ERR, "get_cms_server_ip err\n");
        exit(EXIT_SYSCALL);  
	}

	/* add nvram change cms or snmpc trap infos by xiaowei 2016/02/17 */
	CHAR acNvramValue[32] = {0};
//	if (AP_E_NONE != nvram_get(acNvramValue, NVRAM_SNMPC_CMP_TRAP_CHANGE, NVRAM_VALUE_MAX_LEN))
//    {
//        snmp_log(LOG_ERR, "nvram get fail %s.\n", NVRAM_SNMPC_CMP_TRAP_CHANGE);
//    }
	uci_config_get(UCI_CONFIG_SNMP_STATUS,acNvramValue);
	snmp_log(LOG_INFO, "uci get current snmp connect status: %s.\n", acNvramValue);
	/* Handle incoming connect requests and incoming data */
	while (!g_quit) {

		if( 0 == atoi(acNvramValue) )
		{
			snmp_trap_status_changes();
		}
		else if( 1 == atoi(acNvramValue) )
		{
			agent_check_dev_state();
		}
        else 
        {
        	agent_check_dev_state();
        	snmp_trap_status_changes();
        }  
        
		/* Sleep until we get a request or the timeout is over */
		FD_ZERO(&rfds);
		FD_ZERO(&wfds);
		FD_SET(g_udp_sockfd, &rfds);

		FD_SET(g_alarm_sockfd, &rfds);
		nfds = (g_udp_sockfd > g_alarm_sockfd) ? g_udp_sockfd : g_alarm_sockfd;

		if (select(nfds + 1, &rfds, &wfds, NULL, &tv_sleep) == -1) 
		{
			if (g_quit) 
			{
				break;
			}
			lprintf(LOG_ERR, "could not select from sockets: %m\n");
			exit(EXIT_SYSCALL);
		}

		/* Determine whether to update the MIB and the next ticks to sleep */
		ticks = ticks_since(&tv_last, &tv_now);
		if (ticks < 0 || ticks >= g_timeout) 
		{
			memcpy(&tv_last, &tv_now, sizeof (tv_now));
			tv_sleep.tv_sec = 4 ;
			tv_sleep.tv_usec = (g_timeout % 100) * 10000;

		} 
		else 
		{
			tv_sleep.tv_sec = (g_timeout - ticks) / 100;
			tv_sleep.tv_usec = ((g_timeout - ticks) % 100) * 10000;
		}

		/* Handle UDP packets, TCP packets and TCP connection connects */
		if (FD_ISSET(g_udp_sockfd, &rfds)) 
		{
			//处理接收到的server的报文
			handle_udp_client();
		}

		if (FD_ISSET(g_alarm_sockfd, &rfds)) 
		{
			handle_alarm_msg();
		}

	    if (1 == g_uiOfflineFlag) /* CMS or device off-line, then start offline counter */
	    {
	        //g_uiOfflineCount = 1;
	        g_uiOfflineCount++;
	    }

		if ((DEV_STATE_RUNNING == g_enCpeState) && (0 == (g_uiSendHeartCount % g_ulSendHeartTime)))
		{
		    //printf("send heart inform trap!\n");
		    
		    //回复收到的 server 发送的报文
		    snmp_send_inform_trap(g_pstSnmpAgentData);
		    g_uiOfflineFlag = 1;
		}

		g_uiSendHeartCount++;
	    g_uiSendDiscoverCount++;
	    
	    //printf("g_uiOfflineFlag=%d\n", g_uiOfflineFlag);

	    if ((0 == g_uiOfflineCount % g_uiOfflineTime) &&(1 == g_uiOfflineFlag))
	    {
	        g_enCpeState = DEV_STATE_DISCOVER;  /* device offline */
	        g_uiOfflineFlag = 3;

	        g_uiSendHeartCount = 1;
	        g_uiOfflineCount = 1;
	        g_uiSendDiscoverCount = 1;

//            //om_rmtmgr_state_offline();
	        printf("device offline!\n");
	    }

	    //printf("g_uiOfflineCount=%d\n", g_uiOfflineCount);
	}

	snmp_agent_destroy();

	return EXIT_OK;
}



/* vim: ts=4 sts=4 sw=4 nowrap
 */
