/******************************************************************************
* COPYRIGHT GBCOM
*******************************************************************************
* 功    能: 
* 修改历史: 
* 2012-9-10     冯静              新建
******************************************************************************/
#include <syslog.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "mini_snmpd.h"

#include "snmp_nms.h"

#include "my_snmp_def.h"
#include "my_snmp_data.h"
#include "my_snmp_oid.h"
#include "my_snmp_handle.h"
#include "my_snmp_util.h"
#include "my_snmp_config.h"
#include "my_snmp_init.h"
#include "my_snmp_trap.h"
#include "my_snmp_alarm.h"
#include "my_snmp_info.h"

#include "ap_types.h"
#include "uci_config.h"
#include "om_include/om_net.h"


#include <netdb.h>
#include <sys/socket.h>
#include <stdio.h>


/*************************************************************************/
extern UINT32      g_ulRootOIDSize;
extern OID         g_rootOID[MAX_SNMP_OID_SIZE];
extern SNMP_AGENT_DATA_T *g_pstSnmpAgentData;
extern OSTOKEN gtSemConfig;	/* 配置互斥信号量 */
extern DEV_STATE_E         g_enCpeState;
extern UINT8 g_ucCmsIP[4];

static char acLodLteClid[32];
static char acLodLteRssi[32];
static char acLodLteRsrp[32];
static char acLodLteRsrq[32];
static char acLodLteSinr[32];

static int acLodLanPortSta1 = 0;
static int acLodLanPortSta2 = 0;
static int acLodLanPortSta3 = 0;
static int acLodLanPortSta4 = 0;

static int times_t=0;

REMOTE AP_ERROR_CODE_E net_port_get_ip (CHAR *pcPortName, UINT32 *pulIp, UINT32 *pulMask);

BOOL get_ip_by_url(const char *name, UINT32 *res)
{
	struct hostent *netent;

	if ((netent = gethostbyname(name))) 
	{
		*res = ntohl( (int)*(int *)*netent->h_addr_list);
		return TRUE;
	} 
	else
	{
		return FALSE;

    }
}

int is_ip_address(const char *s)
{
    int a, b, c, d;
    char t;
    if (4==sscanf(s,"%d.%d.%d.%d%c",&a,&b,&c,&d,&t)) 
	{
        if(0<=a && a<=255
         && 0<=b && b<=255
         && 0<=c && c<=255
         && 0<=d && d<=255) 
		{
            //printf("[%s] is IPv4\n",s);
            return 1;
        } 
        else 
        {
            printf("[%s] is invalid IPv4\n",s);
            return 0;
        }
    } 
    else 
    {
        printf("[%s] is invalid IPv4\n",s);
        return 0;
    }

}


DISCOVER_TRAP_T g_stDisTrapData;//dp

void ipToNum(CHAR* ip, UINT8 *pucSections)
{
	CHAR* p;
	//UINT8 sections[4]={0};
	int i=0;

	p = strtok(ip,".");
	while( p )
	{
		pucSections[i] = atoi(p);
		p = strtok(NULL,".");
		//printf("sections[i]=%d\n", sections[i]);
		i++;
	}
    
	return;
}

OM_ERROR_CODE_E om_get_cms_ip(CHAR *pcNvramValue)
{
    OM_ERROR_CODE_E enRet;
    //CHAR acNvramValue[128] = {0};

    //enRet = nvram_get(acNvramValue, NVRAM_CMS_URL, sizeof(acNvramValue));
//    enRet = nvram_get(pcNvramValue, NVRAM_CMS_URL, 128);
//    if(OM_E_NONE != enRet)
//    {
//        printf("[line:%d] [func:%s]error!!!!!!!!!!!\n", __LINE__, __FUNCTION__);
//        return enRet;
//    }
    //strncpy(pcNvramValue, acNvramValue, sizeof(acNvramValue));

    return OM_E_NONE;

}

/*************************************************************************
* 函数名称: snmp_add_trap_var
* 功    能: 给Trap消息增加绑定变量
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
int snmp_add_trap_var(SNMP_VAR_T **ppstVarList, SNMP_OID *pstOID, UINT32 ulOIDSize, 
                        UINT16 usVarType, VOID* pVar, UINT32 ulVarLen)
{
	SNMP_OID astOID[32] = {0};
    UINT8 ucType = snmp_get_asn1_type(usVarType);

	if (memcmp(pstOID, oid_snmp_trap, sizeof(oid_snmp_trap))== 0)
	{
		memcpy(astOID, &(g_rootOID[0]), g_ulRootOIDSize*sizeof(OID));
		memcpy((UINT8 *)&(astOID[0])+(g_ulRootOIDSize*sizeof(OID)), pVar, 
			ulVarLen);
	    return snmp_varlist_add_var(ppstVarList, pstOID, ulOIDSize,
	                                ucType, 
	                                (unsigned char*)astOID, 
	                                g_ulRootOIDSize*sizeof(OID)+ulVarLen);
	}
	else
	{
		memcpy(astOID, &(g_rootOID[0]), g_ulRootOIDSize*sizeof(OID));
		memcpy((UINT8 *)&(astOID[0])+(g_ulRootOIDSize*sizeof(OID)), pstOID, 
			ulOIDSize*sizeof(OID));
		
	    return snmp_varlist_add_var(ppstVarList, astOID, g_ulRootOIDSize+ulOIDSize,
	                                ucType, 
	                                (unsigned char*)pVar, 
	                                ulVarLen);
	}
}

BOOL snmp_sys_lte_cell_id_info(void)
{
	 
	char  acNowLteClid[32] = {0};
	char  acNowLteRssi[32] = {0};
	char  acNowLteRsrp[32] = {0};
	char  acNowLteRsrq[32] = {0};
	char  acNowLteSinr[32] = {0};
	 
	uci_config_get(UCI_LTEINFO_PHYSICAL_COMMUNITY_ID,acNowLteClid);
	uci_config_get(UCI_LTEINFO_INFO_RSSI,acNowLteRssi);
	uci_config_get(UCI_LTEINFO_INFO_RSRP,acNowLteRsrp);
	uci_config_get(UCI_LTEINFO_INFO_RSRQ,acNowLteRsrq);
	uci_config_get(UCI_LTEINFO_INFO_SINR,acNowLteSinr);
    
	sprintf(g_stDisTrapData.aucLteCellId,"%s","");

    if( strcmp(acNowLteClid,acLodLteClid) )
    {
    	memset(g_stDisTrapData.aucLteCellId,0,sizeof(g_stDisTrapData.aucLteCellId));
    	
    	if( 0 != times_t )
    	{
    		sprintf(g_stDisTrapData.aucLteCellId,"FROM:%s RSSI:%s RSRP:%s RSRQ:%s SINR:%s TO:%s RSSI:%s RSRP:%s RSRQ:%s SINR:%s", \
    			acLodLteClid,acLodLteRssi,acLodLteRsrp,acLodLteRsrq,acLodLteSinr, \
    			acNowLteClid,acNowLteRssi,acNowLteRsrp,acNowLteRsrq,acNowLteSinr);
    	}
    	else 
    	{
    		sprintf(g_stDisTrapData.aucLteCellId,"ID:%s RSSI:%s RSRP:%s RSRQ:%s SINR:%s", \
    			acNowLteClid,acNowLteRssi,acNowLteRsrp,acNowLteRsrq,acNowLteSinr);
    	}
    	
    	sprintf(acLodLteClid,"%s",acNowLteClid);
    	sprintf(acLodLteRssi,"%s",acNowLteRssi);
    	sprintf(acLodLteRsrp,"%s",acNowLteRsrp);
    	sprintf(acLodLteRsrq,"%s",acNowLteRsrq);
    	sprintf(acLodLteSinr,"%s",acNowLteSinr);
    	
    	times_t++;
    	//snmp_log(LOG_NOTICE, "get lte cell id info %s",g_stDisTrapData.aucLteCellId);
    }

    return TRUE;
}
BOOL snmp_sys_lan_port_status_info(void)
{
   	OM_NET_LINK_STA stNetPhyLink = {0};
    	
    sprintf(g_stDisTrapData.aucLanPortSta,"%s","");
    
    if ( OM_E_NONE != om_net_lan_link_status(&stNetPhyLink) )
    {
        snmp_log(LOG_ERR, "get lan port change info failed");
    }
    else
    {
    	memset(g_stDisTrapData.aucLanPortSta,0,sizeof(g_stDisTrapData.aucLanPortSta));
    	if( acLodLanPortSta1 != stNetPhyLink.uiLinkPort1 )
    	{
    		if( 1 == stNetPhyLink.uiLinkPort1 )
    		{
    			sprintf(g_stDisTrapData.aucLanPortSta,"%s LAN1 UP",g_stDisTrapData.aucLanPortSta);
    		}
    		else
    		{
    			sprintf(g_stDisTrapData.aucLanPortSta,"%s LAN1 DOWN",g_stDisTrapData.aucLanPortSta);
    		}
    		acLodLanPortSta1 = stNetPhyLink.uiLinkPort1;
    	}
    	if( acLodLanPortSta2 != stNetPhyLink.uiLinkPort2)
    	{
    		if( 1 == stNetPhyLink.uiLinkPort2 )
    		{
    			sprintf(g_stDisTrapData.aucLanPortSta,"%s LAN2 UP",g_stDisTrapData.aucLanPortSta);
    		}
    		else
    		{
    			sprintf(g_stDisTrapData.aucLanPortSta,"%s LAN2 DOWN",g_stDisTrapData.aucLanPortSta);
    		}
    		acLodLanPortSta2 = stNetPhyLink.uiLinkPort2;
    	}
    	if( acLodLanPortSta3 != stNetPhyLink.uiLinkPort3 )
    	{
    		if( 1 == stNetPhyLink.uiLinkPort3 )
    		{
    			sprintf(g_stDisTrapData.aucLanPortSta,"%s LAN3 UP",g_stDisTrapData.aucLanPortSta);
    		}
    		else
    		{
    			sprintf(g_stDisTrapData.aucLanPortSta,"%s LAN3 DOWN",g_stDisTrapData.aucLanPortSta);
    		}
    		acLodLanPortSta3 = stNetPhyLink.uiLinkPort3;
    	}
    	if( acLodLanPortSta4 != stNetPhyLink.uiLinkPort4 )
    	{
    		if( 1 == stNetPhyLink.uiLinkPort4 )
    		{
    			sprintf(g_stDisTrapData.aucLanPortSta,"%s LAN4 UP",g_stDisTrapData.aucLanPortSta);
    		}
    		else
    		{
    			sprintf(g_stDisTrapData.aucLanPortSta,"%s LAN4 DOWN",g_stDisTrapData.aucLanPortSta);
    		}
    		acLodLanPortSta4 = stNetPhyLink.uiLinkPort4;
    	}
    	//snmp_log(LOG_NOTICE, "get lan port :%d %d %d %d",stNetPhyLink.uiLinkPort1,stNetPhyLink.uiLinkPort2,stNetPhyLink.uiLinkPort3,stNetPhyLink.uiLinkPort4);
    }
    
    return TRUE;
}
BOOL agent_update_discover_snmp(SNMP_AGENT_DATA_T* pstSnmpData)
{
    /* DevInfo */
    snmp_sys_dev_info();
    strncpy(g_stDisTrapData.aucSn, pstSnmpData->stSysDevInfo.acSysSn,CHAR_LEN_32);
    strncpy(g_stDisTrapData.aucDevType, pstSnmpData->stSysDevInfo.acSysDevType,CHAR_LEN_32);
    strncpy(g_stDisTrapData.acSysTime, pstSnmpData->stSysDevInfo.acSysTime,CHAR_LEN_32);
    g_stDisTrapData.uiSysUptime = pstSnmpData->stSysDevInfo.uiSysUptime;

    /* SwInfo */
    strncpy(g_stDisTrapData.aucTargetVersion, pstSnmpData->stSwInfo.acTargetVersion,CHAR_LEN_32);
    strncpy(g_stDisTrapData.aucCurVersion, pstSnmpData->stSwInfo.acSoftwareVersion,CHAR_LEN_32);

    /* HwInfo */
    strncpy(g_stDisTrapData.aucHwVersion, pstSnmpData->stHwInfo.aucCpeHwver,CHAR_LEN_32);

    /* ManagerInfo */
    snmp_sys_manage_info();
    
//    printf("[%25s|%30s:%5d] Ip=%u.%u.%u.%u Mac=%x:%x:%x:%x:%x:%x \n",__FILE__,__FUNCTION__,__LINE__, \
//    	pstSnmpData->stManageInfo.acManageIp[0],pstSnmpData->stManageInfo.acManageIp[1], \
//    	pstSnmpData->stManageInfo.acManageIp[2],pstSnmpData->stManageInfo.acManageIp[3], \
//    	pstSnmpData->stManageInfo.acManageMac[0],pstSnmpData->stManageInfo.acManageMac[1], \
//    	pstSnmpData->stManageInfo.acManageMac[2],pstSnmpData->stManageInfo.acManageMac[3], \
//    	pstSnmpData->stManageInfo.acManageMac[4],pstSnmpData->stManageInfo.acManageMac[5]);	
//    	
    memcpy(g_stDisTrapData.aucManagerIp, pstSnmpData->stManageInfo.acManageIp, IP_ALEN);
    memcpy(g_stDisTrapData.aucManagerMac, pstSnmpData->stManageInfo.acManageMac, ETH_ALEN);
    g_stDisTrapData.uiManageState = pstSnmpData->stManageInfo.uiManageState;
    memcpy(g_stDisTrapData.aucManageIpv6, pstSnmpData->stManageInfo.aucManageIpv6, CHAR_LEN_64);

    /*WanInfo*/
    g_stDisTrapData.ulWanMode = pstSnmpData->stWanStateInfo.ulWanMode;

    /* LteInfo */
    snmp_sys_lte_info_init();
    g_stDisTrapData.uiDailWay = pstSnmpData->stLteNetInfo.uiLteDailWay;
    strncpy(g_stDisTrapData.aucApnSelect, pstSnmpData->stLteCfgInfo.aucApnSelect,CHAR_LEN_32);
    g_stDisTrapData.uiAccessMode = pstSnmpData->stLteCfgInfo.uiAccessMode;
    g_stDisTrapData.uiPriAccessMode = pstSnmpData->stLteCfgInfo.uiPriAccessMode;
    g_stDisTrapData.uiSimLock = pstSnmpData->stLteHwInfo.uiSimLock;
    strncpy(g_stDisTrapData.aucModuleType, pstSnmpData->stLteHwInfo.aucModuleType,CHAR_LEN_32);
    strncpy(g_stDisTrapData.aucModuleVer, pstSnmpData->stLteHwInfo.aucModuleVer,CHAR_LEN_128);
    strncpy(g_stDisTrapData.aucSimICCID, pstSnmpData->stLteHwInfo.aucSimICCID,CHAR_LEN_32);
    strncpy(g_stDisTrapData.aucModuleIMEI, pstSnmpData->stLteHwInfo.aucModuleIMEI,CHAR_LEN_32);
    strncpy(g_stDisTrapData.aucSimIMSI, pstSnmpData->stLteHwInfo.aucSimIMSI,CHAR_LEN_32);
    
    return TRUE;    
}

/*************************************************************************
* 函数名称: snmp_udp_send
* 功    能: 发送UDP消息，并接受socket消息（允许超时）
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
int snmp_udp_send(unsigned char *pucServerIP, unsigned short usServerPort, unsigned int ulTimeout,
                  void *pstReqMsg, unsigned int ulReqMsgLen,
                  void *pRcvMsg,   unsigned int ulMaxRcvMsgLen, unsigned int *pulRcvMsgLen,
                  SNMP_TLV_T	*pstReqID)
{
    int     iRet = 0;
    SOCKADDR_IN stSockAddr = {0};
    SOCKADDR *pstAddr = (SOCKADDR *)&stSockAddr;
    int iAddrLen = sizeof(SOCKADDR);
    unsigned int  ulServerIP;
	int i;

    memcpy(&ulServerIP, pucServerIP, 4);
	
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port   = htons(usServerPort);
    stSockAddr.sin_addr.s_addr = ulServerIP;

    for (i = 0; i < 3; i++)
    {    
	    iRet = sendto(g_udp_sockfd, (const char*)pstReqMsg, ulReqMsgLen, 0, pstAddr, iAddrLen);
	    if (iRet > 0)
	    {
	        return 0;
	    }
	    else
	    {
	        snmp_log(LOG_ERR, "snmp_udp_send: sendto(iRet = %d) failed\n", iRet);
  	  	    return -1;
  	  	}
    }
	
	return -1;				
}

/*************************************************************************
* 函数名称: snmp_send_trap_inform
* 功    能: 实现snmp inform操作,inform和trap的区别就是inform需要等待管理者的回复，
		    如果未回复，代理者会重新发送inform信息，而trap则不需要等待回复，
		    inform相对比较可靠
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明: 
*************************************************************************/
int snmp_send_trap_inform(SNMP_SESSION_T *pstSession, SNMP_VAR_T *pstVarList)
{
    int                 iRet;
    SNMP_TRAP2_PDU_T    stPDU;
    unsigned int        ulRecvMsgLen;
    unsigned char       *pBuf = NULL;
    unsigned int        ulBufLen = 64*1024;
	unsigned int	    ulReqPDULen;
	unsigned char       *pucReqPDU;
    unsigned char       *pucRspData = NULL;
    unsigned char       ucTrapType = SNMP_MSG_INFORM;
    
	if (!pstSession)
	{
        agent_printf(LOG_ERR, "snmp_send_trap2: pstSession is null");
		return -1;
	}

    pBuf = (unsigned char *)malloc(ulBufLen);
    if (NULL == pBuf)
    {
        agent_printf(LOG_ERR, "snmp_send_trap2: malloc failed");
        return -1;
    }
    if (pstSession->ulTimeout == 0)
    {
        pucRspData = NULL;
    }
    else
    {
        pucRspData = pBuf;
        ucTrapType = SNMP_MSG_INFORM;
    }
    memset(&stPDU, 0, sizeof(stPDU));
    stPDU.pstVarList = pstVarList;
    
    /*初始化stPDU中的每一个TLV结构 stPDU、stVer	、stCommunity、stSnmpPDU、stReqID	、stErrStatu*/
    iRet = snmp_init_trap2_pdu(pstSession->acCommunity,&stPDU,ucTrapType);
    if (iRet < 0)
    {
        agent_printf(LOG_ERR, "snmp_send_trap2: snmp_init_set_pdu failed");
        goto fail;
    }

    pucReqPDU = pBuf;
    iRet = snmp_create_trap2_pdu(&stPDU, pucReqPDU, ulBufLen, &ulReqPDULen);
    if (iRet < 0)
    {
        agent_printf(LOG_ERR, "snmp_send_trap2: snmp_create_trap2_pdu failed");
		goto fail;
    }

    iRet = snmp_udp_send(pstSession->aucAgentIP, pstSession->usAgentPort, pstSession->ulTimeout, 
						 pucReqPDU, ulReqPDULen,
						 pucRspData, ulBufLen, &ulRecvMsgLen,
						 &(stPDU.stReqID));
    if (iRet < 0)
    {
        agent_printf(LOG_ERR, "snmp_send_trap2: snmp_udp_send failed");
        goto fail;
    }

    free(pBuf);
    snmp_free_trap2_pdu(&stPDU);

    return 0;

fail:
    if (pBuf)
    {
        free(pBuf);
    }

    snmp_free_trap2_pdu(&stPDU);

    return -1;
}
BOOL mac_is_effective(UINT8 *pucMac)
{
    if(pucMac[0] == 0 && pucMac[1] == 0 && pucMac[2] == 0 &&
        pucMac[3] == 0 && pucMac[4] == 0 && pucMac[5] == 0)
    {
        return FALSE;
    }
    return TRUE;
}

BOOL agent_update_status_snmp(SNMP_AGENT_DATA_T* pstSnmpData)
{  
    /* get lte CellId xiaowei 2015-12-14 */
    snmp_sys_lte_cell_id_info();
    
    /* get LAN port status xiaowei 2015-12-14 */
    snmp_sys_lan_port_status_info();
    
    return TRUE;    
}

BOOL snmp_trap_status_changes()
{	
    SNMP_VAR_T              *pstVarList = NULL;
	SNMP_SESSION_T          stSnmpSession;
    
    
    agent_update_status_snmp(g_pstSnmpAgentData);
	
	
	if( 0 == strcmp(g_stDisTrapData.aucLteCellId,"") && 0 == strcmp(g_stDisTrapData.aucLanPortSta,"") )
	{
		return TRUE;
	}
	snmp_log(LOG_NOTICE, "lte cellid status: %s",g_stDisTrapData.aucLteCellId);
	snmp_log(LOG_NOTICE, "lan links status: %s",g_stDisTrapData.aucLanPortSta);
	
    if(FALSE == mac_is_effective(g_stDisTrapData.aucManagerMac))
    {
       //sleep(3);
       //return FALSE;
    }
	/*binding variable*/
    SNMP_INIT_TRAP_VAR(pstVarList, oid_lte_status_trap, sizeof(oid_lte_status_trap));

    /* add lte cellid chenge trap oid by xiaowei 2015-12-14 */
    if( strcmp(g_stDisTrapData.aucLteCellId,"") )
    {
		SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_lc_id, TYPE_CHAR , g_stDisTrapData.aucLteCellId);
	}
	/* add lan port status trap oid by xiaowei 2015-12-14 */
	if( strcmp(g_stDisTrapData.aucLanPortSta,"") )
	{
		SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_lan_port, TYPE_CHAR , g_stDisTrapData.aucLanPortSta);
	}

	stSnmpSession.aucAgentIP[0] = g_ucCmsIP[0];
    stSnmpSession.aucAgentIP[1] = g_ucCmsIP[1];
    stSnmpSession.aucAgentIP[2] = g_ucCmsIP[2];
    stSnmpSession.aucAgentIP[3] = g_ucCmsIP[3];

	stSnmpSession.usAgentPort = 162;
	strcpy(stSnmpSession.acCommunity, g_rocommunity);
	stSnmpSession.ulTimeout = 5;
	
	if(-1 == snmp_send_trap_discover(&stSnmpSession, pstVarList)) 
	{	
	    snmp_log(LOG_ERR, "send status trap fail\n");
	    return FALSE; 
	}
    
    snmp_log(LOG_NOTICE, "send status trap success\n");
    
	return TRUE;
}
BOOL snmp_send_discover_trap(SNMP_AGENT_DATA_T *g_pstSnmpAgentData)
{	
    SNMP_VAR_T              *pstVarList = NULL;
	SNMP_SESSION_T          stSnmpSession;
    int                     ret = -1;
    
    agent_update_discover_snmp(g_pstSnmpAgentData);
    if(FALSE == mac_is_effective(g_stDisTrapData.aucManagerMac))
    {
        //sleep(10);
        //return FALSE;
    }
	/*binding variable*/
    SNMP_INIT_TRAP_VAR(pstVarList, oid_lte_discover_trap, sizeof(oid_lte_discover_trap));

    /* DevInfo */
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_devinfo_sn,           TYPE_CHAR, g_stDisTrapData.aucSn);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_devinfo_devtype,      TYPE_CHAR, g_stDisTrapData.aucDevType);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_devinfo_time,         TYPE_CHAR, g_stDisTrapData.acSysTime);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_devinfo_uptime,       TYPE_LONG, g_stDisTrapData.uiSysUptime);

    /* SwInfo */
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_swinfo_targetVer,     TYPE_CHAR, g_stDisTrapData.aucTargetVersion);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_swinfo_curVer,        TYPE_CHAR, g_stDisTrapData.aucCurVersion);

    /* HwInfo */
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_hwinfo_hardwareVer,   TYPE_CHAR,   g_stDisTrapData.aucHwVersion);
 
    /* ManagerInfo */
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_manager_ip,           TYPE_IP,   g_stDisTrapData.aucManagerIp);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_manager_mac,          TYPE_MAC,  g_stDisTrapData.aucManagerMac);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_manager_state,        TYPE_LONG,  g_stDisTrapData.uiManageState);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_manager_ip6,          TYPE_CHAR,  g_stDisTrapData.aucManageIpv6);

    /*WanInfo*/
	SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_wan_mode,             TYPE_LONG,  g_stDisTrapData.ulWanMode);


    /* LteInfo */
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_network_dailway,      TYPE_LONG, g_stDisTrapData.uiDailWay);    
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_network_apnselect,    TYPE_CHAR, g_stDisTrapData.aucApnSelect);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_network_accessmode,   TYPE_LONG, g_stDisTrapData.uiAccessMode);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_network_priaccessmode,   TYPE_LONG, g_stDisTrapData.uiPriAccessMode);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_sim_lock,            TYPE_LONG, g_stDisTrapData.uiSimLock);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_module_type,            TYPE_CHAR, g_stDisTrapData.aucModuleType);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_module_ver,            TYPE_CHAR, g_stDisTrapData.aucModuleVer);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_sim_iccid,            TYPE_CHAR, g_stDisTrapData.aucSimICCID);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_module_imei,            TYPE_CHAR, g_stDisTrapData.aucModuleIMEI);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_sim_imsi,            TYPE_CHAR, g_stDisTrapData.aucSimIMSI);
    

	stSnmpSession.aucAgentIP[0] = g_ucCmsIP[0];
    stSnmpSession.aucAgentIP[1] = g_ucCmsIP[1];
    stSnmpSession.aucAgentIP[2] = g_ucCmsIP[2];
    stSnmpSession.aucAgentIP[3] = g_ucCmsIP[3];

	stSnmpSession.usAgentPort = 162;
	strcpy(stSnmpSession.acCommunity, g_rocommunity);
	stSnmpSession.ulTimeout = 5;
    //printf("[line:%d] [func:%s]before!!!!!!!!!!!\n", __LINE__, __FUNCTION__);
//    CHAR acNvramValue[NVRAM_VALUE_MAX_LEN] = {0};
//	if (AP_E_NONE != nvram_get(acNvramValue, NVRAM_SNMPC_CMP_TRAP_CHANGE, NVRAM_VALUE_MAX_LEN))
//    {
//        snmp_log(LOG_ERR, "nvram get fail %s.\n", NVRAM_SNMPC_CMP_TRAP_CHANGE);
//    }
//    
//	if( 0 == atoi(acNvramValue) )
//	{
//		return TRUE;
//	}else 
//	{
//    
	   	ret = snmp_send_trap_discover(&stSnmpSession, pstVarList);
		if(-1 == ret) 
		{	
	    	snmp_log(LOG_ERR, "send discover trap fail\n");
	    	return FALSE; 
		}
	//}
    //printf("[line:%d] [func:%s]after!!!!!!!!!!!\n", __LINE__, __FUNCTION__);
    g_enCpeState = DEV_STATE_DISCOVER;
    snmp_log(LOG_ERR, "send discover trap success\n");
    
	return TRUE;
}


BOOL snmp_send_debug_trap(SNMP_AGENT_DATA_T* pstSnmpData)
{
    SNMP_VAR_T              *pstVarList = NULL;
	SNMP_SESSION_T          stSnmpSession;
	UINT8 Sections[4]={0};
	UINT8 sections[4]={0};
	CHAR acIpAddr[20];
	struct in_addr stInAddr;
	CHAR acCmd[CHAR_LEN_128] = {0};
    CHAR acInfo[CHAR_LEN_128] = {0};
    CHAR acNvramValue[CHAR_LEN_128] = {0}; 
    UINT32 uiHostAddr;  
    int                     ret = -1; 
    BOOL bRet;
    OM_ERROR_CODE_E         enRet;

    sprintf(acCmd, "ifconfig | sed -n \'/NOARP MULTICAST  MTU:1500/{g;1!p;};h\' | cut -d : -f 2 | cut -d \"\\ \"\\ -f 1");
//    enRet = popen_get_info(acCmd, acInfo, sizeof(acCmd));
//    if (OM_E_NONE != enRet)
//    {
//        snmp_log(LOG_ERR, "popen failed:%d",enRet);    
//        return enRet;
//    }

    ipToNum(acInfo, Sections);
	    
    pstSnmpData->stDebugInfo.acDebugClientIp[0] = Sections[0];
    pstSnmpData->stDebugInfo.acDebugClientIp[1] = Sections[1];       
    pstSnmpData->stDebugInfo.acDebugClientIp[2] = Sections[2];
    pstSnmpData->stDebugInfo.acDebugClientIp[3] = Sections[3];
    
    SNMP_INIT_TRAP_VAR(pstVarList, oid_lte_debug_trap, sizeof(oid_lte_debug_trap));    
	SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_manager_mac,  TYPE_MAC,  pstSnmpData->stManageInfo.acManageMac);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_debug_ClientIp,           TYPE_IP,  pstSnmpData->stDebugInfo.acDebugClientIp);

    om_get_cms_ip(acNvramValue);
    ret = is_ip_address(acNvramValue);
    if (1 == ret)
    {
        ipToNum(acNvramValue, Sections);
        
        stSnmpSession.aucAgentIP[0] = Sections[0];
        stSnmpSession.aucAgentIP[1] = Sections[1];
        stSnmpSession.aucAgentIP[2] = Sections[2];
        stSnmpSession.aucAgentIP[3] = Sections[3];
    }
    else
    {
        bRet = get_ip_by_url(acNvramValue, &uiHostAddr);
        if (TRUE == bRet)
        {
            stInAddr.s_addr = uiHostAddr;
            strncpy(acIpAddr, inet_ntoa(stInAddr), sizeof(acIpAddr));
            ipToNum(acIpAddr, sections);  
            stSnmpSession.aucAgentIP[0] = sections[0];
            stSnmpSession.aucAgentIP[1] = sections[1];
            stSnmpSession.aucAgentIP[2] = sections[2];
            stSnmpSession.aucAgentIP[3] = sections[3];
        }
        else
        {
            printf("Invalid URL\n");
        }
    }

	stSnmpSession.usAgentPort = 162;
	strcpy(stSnmpSession.acCommunity, g_rocommunity);
	stSnmpSession.ulTimeout = 5;

    ret = snmp_send_trap_discover(&stSnmpSession, pstVarList);
	if(-1 == ret) 
    {
    	snmp_log(LOG_ERR,"snmp_send_trap_discover fail");
        return FALSE; 
    }
    else
    {
    	snmp_log(LOG_INFO,"snmp_send_update_trap success!\n");
    }

	return TRUE;
}

BOOL snmp_trap(SNMP_AGENT_DATA_T* pstSnmpAgentData)
{
    SNMP_AGENT_DATA_T* pstSnmpData = pstSnmpAgentData;

    if(FALSE == snmp_send_debug_trap(pstSnmpData))
    {
		return FALSE;
    }

    return TRUE;
}

/*************************************************************************
* 函数名称: snmp_udp_send
* 功    能: 发送UDP消息，并接受socket消息（允许超时）
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
int snmp_udp_send2(unsigned char *pucServerIP, unsigned short usServerPort, unsigned int ulTimeout,
                  void *pstReqMsg, unsigned int ulReqMsgLen,
                  void *pRcvMsg,   unsigned int ulMaxRcvMsgLen, unsigned int *pulRcvMsgLen,
                  SNMP_TLV_T	*pstReqID)
{
	SOCKET  stSock;
    int     iRet = 0;
    SOCKADDR_IN stSockAddr = {0};
    SOCKADDR *pstAddr = (SOCKADDR *)&stSockAddr;
    int iAddrLen = sizeof(SOCKADDR);
    unsigned int  ulServerIP;

	unsigned int i=0;
	unsigned int timeout = 2;

    memcpy(&ulServerIP, pucServerIP, 4);
	
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port   = htons(usServerPort);
    stSockAddr.sin_addr.s_addr = ulServerIP;

    stSock = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (stSock == INVALID_SOCKET)
    {
        agent_printf(LOG_ERR, "snmp_udp_send: socket() failed");
        return -1;
    }
    for (i=0; i<(ulTimeout/timeout)+1; i++)
    {    

		//nms_print_binary(pstReqMsg, ulReqMsgLen);
        iRet = 0;
        ///iRet = sendto(stSock, (const char*)pstReqMsg, ulReqMsgLen, 0, pstAddr, iAddrLen);
	    iRet = sendto(g_udp_sockfd, (const char*)pstReqMsg, ulReqMsgLen, 0, pstAddr, iAddrLen);
	    if (iRet < 0)
	    {
	        agent_printf(LOG_ERR, "snmp_udp_send: sendto() failed");
	        closesocket(stSock);
	        return -1;
	    }


#if 0
	    if (NULL != pRcvMsg && NULL != pulRcvMsgLen)
	    {
	        iRcvSize = snmp_recv_from(stSock, (char*)pRcvMsg, ulMaxRcvMsgLen, timeout);
	        if (iRcvSize < 0)
	        {
	            agent_printf(LOG_ERR, "snmp_udp_send: snmp_recv_from() failed(%s)", snmp_get_socket_err());
	            closesocket(stSock);
	            return -1;
	        }
	        else if(0 == iRcvSize)
	        {
	            continue;
	        }

	        if ((unsigned int)iRcvSize > ulMaxRcvMsgLen)
	        {
	            agent_printf(LOG_ERR, "snmp_udp_send: recv size(%d) is too big", iRcvSize);

	            closesocket(stSock);
	            return -1;
	        }			

	        *pulRcvMsgLen = iRcvSize;
	        
			if (-1 == snmp_check_rsp(pRcvMsg, *pulRcvMsgLen, pstReqID))
			{
				agent_printf(LOG_ERR, "snmp_check_rsp: check failed");
				continue;
			}	
	    }
#endif
		// add temp
		//nms_print_binary(pRcvMsg, *pulRcvMsgLen);
	    closesocket(stSock);	
  	  	return 0;
    }
    closesocket(stSock);	
	agent_printf(LOG_ERR, "snmp_check_rsp: check %u times failed",i);	
	return -1;				
}

/*************************************************************************
* 函数名称: snmp_send_trap2
* 功    能: 实现snmp inform操作,inform和trap的区别就是inform需要等待管理者的回复，
		    如果未回复，代理者会重新发送inform信息，而trap则不需要等待回复，
		    inform相对比较可靠
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明: 
*************************************************************************/
int snmp_send_trap3(SNMP_SESSION_T *pstSession, SNMP_VAR_T *pstVarList)
{
    int                 iRet;
    SNMP_TRAP2_PDU_T    stPDU;
    unsigned int        ulRecvMsgLen;
    unsigned char       *pBuf = NULL;
    unsigned int        ulBufLen = 64*1024;
	unsigned int	    ulReqPDULen;
	unsigned char       *pucReqPDU;
    unsigned char       *pucRspData = NULL;
    unsigned char       ucTrapType = SNMP_MSG_TRAP2;
	if (!pstSession)
	{
        agent_printf(LOG_ERR, "snmp_send_trap2: pstSession is null");
		return -1;
	}

    pBuf = (unsigned char *)malloc(ulBufLen);
    if (NULL == pBuf)
    {
        agent_printf(LOG_ERR, "snmp_send_trap2: malloc failed");
        return -1;
    }
    if (pstSession->ulTimeout == 0)
    {
        pucRspData = NULL;
    }
    else
    {
        pucRspData = pBuf;
        /*需要等待超时回响应的，则使用inform类型 code:liwen 2013-08-15*/
        ucTrapType = SNMP_MSG_INFORM;
    }
    memset(&stPDU, 0, sizeof(stPDU));
    stPDU.pstVarList = pstVarList;
    /*初始化stPDU中的每一个TLV结构 stPDU、stVer	、stCommunity、stSnmpPDU、stReqID	、stErrStatu*/
    iRet = snmp_init_trap2_pdu(pstSession->acCommunity,&stPDU,ucTrapType);
    if (iRet < 0)
    {
        agent_printf(LOG_ERR, "snmp_send_trap2: snmp_init_set_pdu failed");
        goto fail;
    }

    pucReqPDU = pBuf;
    iRet = snmp_create_trap2_pdu(&stPDU, pucReqPDU, ulBufLen, &ulReqPDULen);
    if (iRet < 0)
    {
        agent_printf(LOG_ERR, "snmp_send_trap2: snmp_create_trap2_pdu failed");
		goto fail;
    }

    if (snmp_init_socket() < 0)
    {
        agent_printf(LOG_ERR, "snmp_send_trap2: snmp_init_socket failed");
		goto fail;
    }

    //pstSession->ulTimeout = 0;

    iRet = snmp_udp_send2(pstSession->aucAgentIP, pstSession->usAgentPort, pstSession->ulTimeout, 
						 pucReqPDU, ulReqPDULen,
						 pucRspData, ulBufLen, &ulRecvMsgLen,
						 &(stPDU.stReqID));
    if (iRet < 0)
    {
        agent_printf(LOG_ERR, "snmp_send_trap2: snmp_udp_send failed");
        goto fail;
    }

    free(pBuf);
    snmp_free_trap2_pdu(&stPDU);

    return 0;

fail:
    if (pBuf)
    {
        free(pBuf);
    }

    snmp_free_trap2_pdu(&stPDU);

    return -1;
}
#if 0

//Northbound Interface

BOOL snmp_northbound_inform_send(SNMP_AGENT_DATA_T *pstSnmpData)
{	
    SNMP_VAR_T              *pstVarList = NULL;
	SNMP_SESSION_T          stSnmpSession;
    int                     ret = -1;
    struct in_addr stInAddr;
    CHAR acIpAddr[20];
    UINT8 sections[4]={0};
    UINT8 Sections[4]={0};
    CHAR acNvramValue[128]; 
    UINT32 uiHostAddr;  
    BOOL bRet;

    SNMP_INIT_TRAP_VAR(pstVarList, oid_lte_northbound_trap, sizeof(oid_lte_northbound_trap));  
    /*bRet = snmp_lte_station_all_lock_id();
    if (FALSE == bRet)
    {
        snmp_log(LOG_ERR, "get lte wan user info fail\n");
    }*/
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_station_all_lock_id,     TYPE_CHAR, pstSnmpData->stLteStationAllLockID.acLteLockAllID);
    
    /*bRet = snmp_lte_pppoe_all_user_names();
    if (FALSE == bRet)
    {
        snmp_log(LOG_ERR, "get lte wan user info fail\n");
    }*/   
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_pppoe_all_user_names,  TYPE_CHAR, pstSnmpData->stPppoeAllUserNames.uiAllUserNames);
     
    om_get_cms_ip(acNvramValue);
    ret = is_ip_address(acNvramValue);
    if (1 == ret)
    {
        ipToNum(acNvramValue, Sections);
        
        stSnmpSession.aucAgentIP[0] = Sections[0];
        stSnmpSession.aucAgentIP[1] = Sections[1];
        stSnmpSession.aucAgentIP[2] = Sections[2];
        stSnmpSession.aucAgentIP[3] = Sections[3];
    }
    else
    {
        bRet = get_ip_by_url(acNvramValue, &uiHostAddr);
        if (TRUE == bRet)
        {
            stInAddr.s_addr = uiHostAddr;
            strncpy(acIpAddr, inet_ntoa(stInAddr), sizeof(acIpAddr));
            ipToNum(acIpAddr, sections);  
            stSnmpSession.aucAgentIP[0] = sections[0];
            stSnmpSession.aucAgentIP[1] = sections[1];
            stSnmpSession.aucAgentIP[2] = sections[2];
            stSnmpSession.aucAgentIP[3] = sections[3];
        }
        else
        {
            printf("Invalid URL\n");
        }
    }

    //printf("%d %d %d %d\n", stSnmpSession.aucAgentIP[0], stSnmpSession.aucAgentIP[1], 
    //         stSnmpSession.aucAgentIP[2], stSnmpSession.aucAgentIP[3]);        
             
	stSnmpSession.usAgentPort = 162;
	strcpy(stSnmpSession.acCommunity, g_rocommunity);
    stSnmpSession.ulTimeout = 5;

    ret = snmp_send_trap_inform(&stSnmpSession, pstVarList);

	if(-1 == ret) 
    {
    	snmp_log(LOG_ERR,"snmp_send_trap2 fail");
        return FALSE; 
    }
    else
    {
    	snmp_log(LOG_DEBUG,"snmp_send_inform_trap success!\n");
    }

	return TRUE;

}

/*功能:判断用户或者lte_station_all_lock_id是否变化，若变化，发送北向trap*/
BOOL snmp_send_northbound_inform_trap(SNMP_AGENT_DATA_T *pstSnmpData)
{
    static char acLteLockAllIDOld[256] = {0};
    static char acAllUserNamesOld[256] = {0};
    static BOOL bIsFirst = TRUE;
    BOOL bRet;
 
    bRet = snmp_lte_station_all_lock_id();
    if (FALSE == bRet)
    {
        snmp_log(LOG_ERR, "get lte wan user info fail\n");
    } 
    
    
    bRet = snmp_lte_pppoe_all_user_names();
    if (FALSE == bRet)
    {
        snmp_log(LOG_ERR, "get lte wan user info fail\n");
    }

    if(bIsFirst == TRUE)
    {
        bIsFirst = FALSE;
        strncpy(acLteLockAllIDOld, pstSnmpData->stLteStationAllLockID.acLteLockAllID, sizeof(acLteLockAllIDOld));
        strncpy(acAllUserNamesOld, pstSnmpData->stPppoeAllUserNames.uiAllUserNames, sizeof(acAllUserNamesOld));
    }
    if(0 != strncmp(pstSnmpData->stLteStationAllLockID.acLteLockAllID, acLteLockAllIDOld, sizeof(acLteLockAllIDOld)) ||
        0 != strncmp(pstSnmpData->stPppoeAllUserNames.uiAllUserNames, acAllUserNamesOld, sizeof(acAllUserNamesOld)))
    {
        snmp_northbound_inform_send(pstSnmpData);
        strncpy(acLteLockAllIDOld, pstSnmpData->stLteStationAllLockID.acLteLockAllID, sizeof(acLteLockAllIDOld));
        strncpy(acAllUserNamesOld, pstSnmpData->stPppoeAllUserNames.uiAllUserNames, sizeof(acAllUserNamesOld));
    }

    return TRUE;
}
#endif

BOOL snmp_send_inform_trap(SNMP_AGENT_DATA_T *pstSnmpData)
{	
    SNMP_VAR_T              *pstVarList = NULL;
	SNMP_SESSION_T          stSnmpSession;
    int                     ret = -1;

    CHAR   acUnknow[32] = "N/A";
    UINT32 ulBand = 0;
    BOOL bRet;


    SNMP_INIT_TRAP_VAR(pstVarList, oid_lte_heart_trap, sizeof(oid_lte_heart_trap));
	SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_manager_mac,     TYPE_MAC,  pstSnmpData->stManageInfo.acManageMac);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_sim_state,      TYPE_LONG, ulBand);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_signal_rssi,     TYPE_LONG, ulBand);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_signal_rsrp,     TYPE_LONG, ulBand);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_signal_rsrq,     TYPE_LONG, ulBand);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_signal_sinr,     TYPE_LONG, ulBand);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_signal_level,     TYPE_LONG, ulBand);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_network_accessNet,     TYPE_LONG, ulBand);

    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_network_band,         TYPE_LONG, ulBand);
    
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_network_channel,      TYPE_LONG, ulBand);
    

        SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_network_cid,     TYPE_CHAR,  acUnknow);


        SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_network_pcid,      TYPE_CHAR, acUnknow);


        SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_lc_id,      TYPE_CHAR, acUnknow);

        SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_enodeb_id,      TYPE_CHAR, acUnknow);

        SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_network_operCompany,   TYPE_CHAR, acUnknow);

    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_network_dailstate,     TYPE_LONG, ulBand);

    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_wan_AllRxBytes,     TYPE_CHAR, acUnknow);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_wan_AllTxBytes,     TYPE_CHAR, acUnknow);

    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_station_all_lock_id,     TYPE_CHAR, acUnknow);
    
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_pppoe_all_user_names,  TYPE_CHAR, acUnknow);
    

    stSnmpSession.aucAgentIP[0] = g_ucCmsIP[0];
    stSnmpSession.aucAgentIP[1] = g_ucCmsIP[1];
    stSnmpSession.aucAgentIP[2] = g_ucCmsIP[2];
    stSnmpSession.aucAgentIP[3] = g_ucCmsIP[3];
    //printf("%d %d %d %d\n", stSnmpSession.aucAgentIP[0], stSnmpSession.aucAgentIP[1], 
    //         stSnmpSession.aucAgentIP[2], stSnmpSession.aucAgentIP[3]);        
             
	stSnmpSession.usAgentPort = 162;
	strcpy(stSnmpSession.acCommunity, g_rocommunity);
    stSnmpSession.ulTimeout = 5;
    //printf("[line:%d] [func:%s]before!!!!!!!!!!!\n", __LINE__, __FUNCTION__);
    ret = snmp_send_trap_inform(&stSnmpSession, pstVarList);
    //printf("[line:%d] [func:%s]after!!!!!!!!!!!\n", __LINE__, __FUNCTION__);
	if(-1 == ret) 
    {
    	snmp_log(LOG_ERR,"snmp_send_trap2 fail");
        return FALSE; 
    }
    else
    {
    	snmp_log(LOG_DEBUG,"snmp_send_inform_trap success!\n");
    }

	return TRUE;

}

BOOL snmp_send_mib_alarm_trap(OPEN_MIB_ALARM_TRAP_T *pstAlarmTrap)
{
#if 0
	SNMP_VAR_T       *pstVarList = NULL;
	SNMP_SESSION_T stSnmpSession;
    int iRet = 0;
    UINT8   aucInvalidIp[4] = {0};
    if ( 0 == memcmp(g_pstSnmpAgentData->pstApTrapDesConfig->aucTrapDesIPAddress, aucInvalidIp, 4) )
    {
        agent_printf(LOG_ERR, "aucTrapDesIPAddress is 0.0.0.0.\n");
        return FALSE;
    }
	
	if (NULL == pstAlarmTrap)
	{
		agent_printf(LOG_ERR, "%s:snmp_send_alarm_trap2 pstAlarmTrap is NULL\n", 
			SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}
	
	SNMP_INIT_TRAP_VAR(pstVarList, pstAlarmTrap->pstOID, pstAlarmTrap->ulOIDLen);
		
    SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmSn,       TYPE_LONG, pstAlarmTrap->ulAlarmSn);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmNeName,   TYPE_CHAR, pstAlarmTrap->acAlarmNeName);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmLevel,    TYPE_LONG, pstAlarmTrap->ulAlarmLevel);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmType,     TYPE_LONG, pstAlarmTrap->ulAlarmType);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmReasonID, TYPE_LONG, pstAlarmTrap->ulAlarmReasonID);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmReason,   TYPE_CHAR, pstAlarmTrap->acAlarmReason);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmRaiseTime,TYPE_LONG, pstAlarmTrap->ulAlarmRaiseTime);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmStatus,   TYPE_LONG, pstAlarmTrap->ulAlarmStatus);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmTitle,    TYPE_CHAR, pstAlarmTrap->acAlarmTitle);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_ap_alarmInfo,     TYPE_CHAR, pstAlarmTrap->acAlarmInfo);
    
    memset(&stSnmpSession, 0, sizeof(SNMP_SESSION_T));
    memcpy(stSnmpSession.aucAgentIP, g_pstSnmpAgentData->pstApTrapDesConfig->aucTrapDesIPAddress, 4);
    stSnmpSession.usAgentPort = AP_AGENT_TRAP_PORT;
    strcpy(stSnmpSession.acCommunity, 
        ('\0' == g_pstSnmpAgentData->pstApTrapDesConfig->acTrapCommunity[0]?
        g_rocommunity:
        g_pstSnmpAgentData->pstApTrapDesConfig->acTrapCommunity));
    stSnmpSession.ulTimeout = AP_AGENT_TRAP_TIMEOUT;
    iRet = snmp_send_trap_discover(&stSnmpSession, pstVarList);
	if(-1 == iRet) {	return FALSE; }
#endif   

	return TRUE;
}
BOOL ap_agent_convert_alarm_trap(AP_ALARM_INFO_T *pstAlarm, 
									    OPEN_MIB_ALARM_TRAP_T *pstAlarmTrap)
{
	UINT8 *pucMac = NULL;

	if (NULL == pstAlarm)
	{
		agent_printf(LOG_ERR, "ap_agent_convert_alarm_trap() pstAlarm is null.\n");
		return FALSE;
	}

	if (NULL == pstAlarmTrap)
	{
		agent_printf(LOG_ERR, "ap_agent_convert_alarm_trap() pstAlarmTrap is null.\n");
		return FALSE;
	}
	
	pstAlarmTrap->ulAlarmSn = ap_agent_generate_trapsn();
    pucMac = g_pstSnmpAgentData->stManageInfo.acManageMac;
	sprintf(pstAlarmTrap->acAlarmNeName, ""MAC_FMT"", 
				MAC_ARG(pucMac));

	pstAlarmTrap->ulAlarmLevel = pstAlarm->ulAlarmLevel;
    pstAlarmTrap->ulAlarmReasonID = pstAlarm->ulAlarmReasonID;
	pstAlarmTrap->ulAlarmStatus = pstAlarm->stAlarmKey.ulAlarmStatus;
		
	switch(pstAlarm->stAlarmKey.ulAlarmCode)
	{
		case ALARM_ID_APCOLDREBOOT://             1001               /* AP冷启动告警*/
			pstAlarmTrap->ulAlarmType = OPEN_MIB_ALARM_TYPE_DEVICE;
			if (pstAlarm->stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_RAISE)
			{
				sprintf(pstAlarmTrap->acAlarmTitle, "apColdStartTrap");
				pstAlarmTrap->pstOID = (OID *)oid_ap_alarm_sys_apColdStart_trap;
				pstAlarmTrap->ulOIDLen = sizeof(oid_ap_alarm_sys_apColdStart_trap);
			}

			break;
		case ALARM_ID_APWARMREBOOT://             1002               /* AP热启动告警*/
			pstAlarmTrap->ulAlarmType = OPEN_MIB_ALARM_TYPE_DEVICE;
			if (pstAlarm->stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_RAISE)
			{
				sprintf(pstAlarmTrap->acAlarmTitle, "apWarmStartTrap");
				pstAlarmTrap->pstOID = (OID *)oid_ap_alarm_sys_apWarmStart_trap;
				pstAlarmTrap->ulOIDLen = sizeof(oid_ap_alarm_sys_apWarmStart_trap);
			}

			break;
		case ALARM_ID_CPU_OVERLOAD://				1003    /* AP设备CPU负荷过高 */
			pstAlarmTrap->ulAlarmType = OPEN_MIB_ALARM_TYPE_DEVICE;
			if (pstAlarm->stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_RAISE)
			{	
				sprintf(pstAlarmTrap->acAlarmTitle, "apCpuUsageTooHighTrap");
				pstAlarmTrap->pstOID = (OID *)oid_ap_alarm_sys_apCpuUsageTooHigh_trap;
				pstAlarmTrap->ulOIDLen = sizeof(oid_ap_alarm_sys_apCpuUsageTooHigh_trap);
			}
			else if (pstAlarm->stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_CLEAR)
			{
				sprintf(pstAlarmTrap->acAlarmTitle, "apCpuUsageTooHighClearTrap");
				pstAlarmTrap->pstOID = (OID *)oid_ap_alarm_sys_apCpuUsageTooHighClear_trap;
				pstAlarmTrap->ulOIDLen = sizeof(oid_ap_alarm_sys_apCpuUsageTooHighClear_trap);
			}
			break;
		case ALARM_ID_MEM_OVERLOAD://	        	1004    /* AP设备内存负荷过高 */
			pstAlarmTrap->ulAlarmType = OPEN_MIB_ALARM_TYPE_DEVICE;
			if (pstAlarm->stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_RAISE)
			{
				sprintf(pstAlarmTrap->acAlarmTitle, "apMemUsageTooHighTrap");
				pstAlarmTrap->pstOID = (OID *)oid_ap_alarm_sys_apMemUsageTooHigh_trap;
				pstAlarmTrap->ulOIDLen = sizeof(oid_ap_alarm_sys_apMemUsageTooHigh_trap);
			}
			else if (pstAlarm->stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_CLEAR)
			{
				sprintf(pstAlarmTrap->acAlarmTitle, "apMemUsageTooHighClearTrap");
				pstAlarmTrap->pstOID = (OID *)oid_ap_alarm_sys_apMemUsageTooHighClear_trap;
				pstAlarmTrap->ulOIDLen = sizeof(oid_ap_alarm_sys_apMemUsageTooHighClear_trap);
			}
			break;
		case ALARM_ID_COINTERFRE://	        	2001    /* 同频干扰告警 */
			pstAlarmTrap->ulAlarmType = OPEN_MIB_ALARM_TYPE_QOS;
			if (pstAlarm->stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_RAISE)
			{
				sprintf(pstAlarmTrap->acAlarmTitle, "apCoInterfDetectedTrap");
				pstAlarmTrap->pstOID = (OID *)oid_ap_alarm_wids_cointerfre_trap;
				pstAlarmTrap->ulOIDLen = sizeof(oid_ap_alarm_wids_cointerfre_trap);
			}
			else if (pstAlarm->stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_CLEAR)
			{
				sprintf(pstAlarmTrap->acAlarmTitle, "apCoInterfClearTrap");
				pstAlarmTrap->pstOID = (OID *)oid_ap_alarm_wids_cointerfreClear_trap;
				pstAlarmTrap->ulOIDLen = sizeof(oid_ap_alarm_wids_cointerfreClear_trap);
			}
			break;
		case ALARM_ID_NEINTERFRE://	        	2002    /* 邻频干扰告警 */
			pstAlarmTrap->ulAlarmType = OPEN_MIB_ALARM_TYPE_QOS;
			if (pstAlarm->stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_RAISE)
			{
				sprintf(pstAlarmTrap->acAlarmTitle, "apNerborInterfDetectedTrap");
				pstAlarmTrap->pstOID = (OID *)oid_ap_alarm_wids_neinterfre_trap;
				pstAlarmTrap->ulOIDLen = sizeof(oid_ap_alarm_wids_neinterfre_trap);
			}
			else if (pstAlarm->stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_CLEAR)
			{
				sprintf(pstAlarmTrap->acAlarmTitle, "apNeiborInterfClearTrap");
				pstAlarmTrap->pstOID = (OID *)oid_ap_alarm_wids_neinterfreClear_trap;
				pstAlarmTrap->ulOIDLen = sizeof(oid_ap_alarm_wids_neinterfreClear_trap);
			}
			break;
		case ALARM_ID_WLANDEVINTERFRE://	        	2003    /* WLAN设备干扰告警 */
			pstAlarmTrap->ulAlarmType = OPEN_MIB_ALARM_TYPE_QOS;
			if (pstAlarm->stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_RAISE)
			{
				sprintf(pstAlarmTrap->acAlarmTitle, "wlanDevInterfDetectedTrap");
				pstAlarmTrap->pstOID = (OID *)oid_ap_alarm_wids_wlanDevInterfre_trap;
				pstAlarmTrap->ulOIDLen = sizeof(oid_ap_alarm_wids_wlanDevInterfre_trap);
			}
			else if (pstAlarm->stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_CLEAR)
			{
				sprintf(pstAlarmTrap->acAlarmTitle, "wlanDevInterfClearTrap");
				pstAlarmTrap->pstOID = (OID *)oid_ap_alarm_wids_wlanDevInterfreClear_trap;
				pstAlarmTrap->ulOIDLen = sizeof(oid_ap_alarm_wids_wlanDevInterfreClear_trap);
			}
			break;
		case ALARM_ID_OTHERDEVINTERFRE://	        	2004    /* 非WLAN设备干扰告警 */
			pstAlarmTrap->ulAlarmType = OPEN_MIB_ALARM_TYPE_QOS;
			if (pstAlarm->stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_RAISE)
			{
				sprintf(pstAlarmTrap->acAlarmTitle, "nonWlanDevInterfDetectedTrap");
				pstAlarmTrap->pstOID = (OID *)oid_ap_alarm_wids_otherDevInterfre_trap;
				pstAlarmTrap->ulOIDLen = sizeof(oid_ap_alarm_wids_otherDevInterfre_trap);
			}
			else if (pstAlarm->stAlarmKey.ulAlarmStatus == AP_ALARM_STATUS_CLEAR)
			{
				sprintf(pstAlarmTrap->acAlarmTitle, "nonWlanDevInterfClearTrap");
				pstAlarmTrap->pstOID = (OID *)oid_ap_alarm_wids_otherDevInterfreClear_trap;
				pstAlarmTrap->ulOIDLen = sizeof(oid_ap_alarm_wids_otherDevInterfreClear_trap);
			}
			break;
		default:			
			agent_printf(LOG_ERR, "%s:ap_agent_convert_alarm_trap invalid info:", SNMPAGENT_CONTROL_NAME);
			agent_printf(LOG_ERR, "ulAlarmCode       = %lu\n"
				"ulAulAlarmStatus  = %lu\n"
				"ulAlarmChangeTime = %lu\n"
				"ulAlarmSn         = %lu",
				pstAlarm->stAlarmKey.ulAlarmCode,
				pstAlarm->stAlarmKey.ulAlarmStatus,
				pstAlarm->stAlarmKey.ulAlarmChangeTime,
				pstAlarm->stAlarmKey.ulAlarmSn);			
			return FALSE;
	}
		
	if (pstAlarmTrap->ulAlarmStatus == AP_ALARM_STATUS_RAISE)
	{
		if (pstAlarm->ulAlarmRaiseTime != 0)
		{
			pstAlarmTrap->ulAlarmRaiseTime = pstAlarm->ulAlarmRaiseTime;
		}
		else
		{
			pstAlarmTrap->ulAlarmRaiseTime = pstAlarm->stAlarmKey.ulAlarmChangeTime;
		}
        pstAlarmTrap->ulAlarmStatus = ALARM_STATUS_ACTIVE;
	}
	else if(pstAlarmTrap->ulAlarmStatus == AP_ALARM_STATUS_CLEAR)
	{
		pstAlarmTrap->ulAlarmRaiseTime = pstAlarm->ulAlarmClearTime;
        pstAlarmTrap->ulAlarmStatus = ALARM_STATUS_CLEAR;
	}
	else
	{
		pstAlarmTrap->ulAlarmRaiseTime = pstAlarm->stAlarmKey.ulAlarmChangeTime;
	}


	return TRUE;
}
            
BOOL ap_agent_send_alarm_trap(AP_ALARM_INFO_T *pstAlarm)
{
	OPEN_MIB_ALARM_TRAP_T stAlarmTrap = {0};
	BOOL bRet = FALSE;
    int iRet = -1;
        
	if (NULL == pstAlarm)
	{
		agent_printf(LOG_ERR, "%s:ap_agent_send_alarm_trap pstAlarm is null\n",
			SNMPAGENT_CONTROL_NAME);
		return FALSE;
	}

	/* 加上互斥锁 */
	if (!OsSemaphoreP(gtSemConfig, WAIT_FOREVER, 1))
	{
		agent_printf(LOG_ERR,
			"gtSemConfig OsSemaphoreP fail\n");
    	return FALSE;
	}
			
	printf("pstAlarm->stAlarmKey.ulAlarmCode:%lu\n"
		"pstAlarm->stAlarmKey.ulAlarmStatus:%lu\n"
		"pstAlarm->ulAlarmRaiseTime:%lu\n"
		"pstAlarm->stAlarmKey.ulAlarmChangeTime:%lu\n"
		"pstAlarm->ulAlarmClearTime:%lu\n"
		"pstAlarm->stAlarmKey.ulAlarmSn:%lu\n"
		"pstAlarm->ulAlarmReasonID:%lu\n"
		"pstAlarm->ulAlarmLevel:%lu\n"
		,
		pstAlarm->stAlarmKey.ulAlarmCode,
		pstAlarm->stAlarmKey.ulAlarmStatus,
		pstAlarm->ulAlarmRaiseTime,
		pstAlarm->stAlarmKey.ulAlarmChangeTime,
		pstAlarm->ulAlarmClearTime,
		pstAlarm->stAlarmKey.ulAlarmSn,
		pstAlarm->ulAlarmReasonID,
		pstAlarm->ulAlarmLevel
		);
	
	bRet = ap_agent_convert_alarm_trap(pstAlarm, &stAlarmTrap);
	if ( !bRet)
	{
		agent_printf(LOG_ERR, "ap_agent_convert_alarm_trap failed.\n");
	}
	
	if (stAlarmTrap.acAlarmTitle[0] == '\0')
	{
		agent_printf(LOG_INFO, "%s:no need to send trap:%lu\n", SNMPAGENT_CONTROL_NAME, 
			pstAlarm->stAlarmKey.ulAlarmCode);
        goto error;
	}
    
	iRet = snmp_send_mib_alarm_trap(&stAlarmTrap);
	if (-1 == iRet)
	{
		agent_printf(LOG_ERR, "%s:snmp_send_alarm_trap failed\n", 
			SNMPAGENT_CONTROL_NAME);
		goto error;
	}

	OsSemaphoreV(gtSemConfig, 1);
    return TRUE;
	
error:
	OsSemaphoreV(gtSemConfig, 1);
	return FALSE;
}


BOOL snmp_send_update_trap(SNMP_AGENT_DATA_T* pstSnmpData)
{
    SNMP_VAR_T              *pstVarList = NULL;
	SNMP_SESSION_T          stSnmpSession;
    int                     ret = -1; 
    CHAR acNvramValue[128]; 
    UINT32 uiHostAddr;  
    UINT8 Sections[4]={0};
    BOOL bRet;
    struct in_addr stInAddr;
    CHAR acIpAddr[20];
    
    SNMP_INIT_TRAP_VAR(pstVarList, oid_lte_software_update, sizeof(oid_lte_software_update));
    
	SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_manager_mac,      TYPE_MAC,  pstSnmpData->stManageInfo.acManageMac);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_swinfo_targetVer,       TYPE_CHAR, pstSnmpData->stSwInfo.acSoftwareVersion);
	SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_swinfo_curVer,        TYPE_CHAR, pstSnmpData->stSwInfo.acTargetVersion);
    SNMP_ADD_TRAP_VAR(pstVarList, oid_lte_trans_status,     TYPE_LONG, pstSnmpData->stFileTrans.ulTransStatus);

	 om_get_cms_ip(acNvramValue);
    ret = is_ip_address(acNvramValue);
    if (1 == ret)
    {
        ipToNum(acNvramValue, Sections);
        
        stSnmpSession.aucAgentIP[0] = Sections[0];
        stSnmpSession.aucAgentIP[1] = Sections[1];
        stSnmpSession.aucAgentIP[2] = Sections[2];
        stSnmpSession.aucAgentIP[3] = Sections[3];
    }
    else
    {
        bRet = get_ip_by_url(acNvramValue, &uiHostAddr);
        if (TRUE == bRet)
        {
            stInAddr.s_addr = uiHostAddr;
            strncpy(acIpAddr, inet_ntoa(stInAddr), sizeof(acIpAddr));
            ipToNum(acIpAddr, Sections);  
            stSnmpSession.aucAgentIP[0] = Sections[0];
            stSnmpSession.aucAgentIP[1] = Sections[1];
            stSnmpSession.aucAgentIP[2] = Sections[2];
            stSnmpSession.aucAgentIP[3] = Sections[3];
        }
        else
        {
            printf("Invalid URL\n");
        }
    }
	
	stSnmpSession.usAgentPort = 162;
	strcpy(stSnmpSession.acCommunity, g_rocommunity);
	stSnmpSession.ulTimeout = 5;

    ret = snmp_send_trap_discover(&stSnmpSession, pstVarList);
	if(-1 == ret) 
    {
    	snmp_log(LOG_ERR,"snmp_send_trap_discover fail");
        return FALSE; 
    }
    else
    {
    	snmp_log(LOG_INFO,"snmp_send_update_trap success!\n");
    }

	return TRUE;
}
