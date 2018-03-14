/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : lteauto.c
  版 本 号   : 初稿
  作    者   : xiao wei
  生成日期   : 2016年3月15日 星期二
  最近修改   :
  功能描述   :
  函数列表   :
  修改历史   :
  1.日    期   : 2016年3月15日 星期二
    作    者   : xiao wei
    修改内容   : 创建文件

******************************************************************************/
/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>  
#include <unistd.h>

#include "ap_types.h"
#include "lte.h"
#define LEN_64 64

AP_ERROR_CODE_E  lte_get_link_status( LTE_LINKSTATE_E *peLteLinkStatus )
{
    LTE_LINKSTATE_T stLinkState = {0};
    UINT32 iRet;

    if(NULL == peLteLinkStatus)
    {
        syslog(LOG_ERR, "the pointer is null");
        return AP_E_DEFAULT_FAIL;
    }

    iRet = lte_linkstate_get(&stLinkState);
    if ( LTE_OK != iRet)
    {
        syslog(LOG_ERR, "get lte link state failed:%d",iRet);
		*peLteLinkStatus = LTE_LINKSTATE_DISCONNECT;
        return AP_E_NONE;
    }

	*peLteLinkStatus = stLinkState.enLinkstate;
	
	if(LTE_LINKSTATE_CONNECTED == stLinkState.enLinkstate)
	{
		if(LTE_OK != lte_network_prepare_get(NULL))
		{
			//lte_disconnect();
			*peLteLinkStatus = LTE_LINKSTATE_DISCONNECT;
		}
	}	
    
    //syslog(LOG_NOTICE, "lte link state:%d",*peLteLinkStatus);
    
    return AP_E_NONE;
}


INT32 main(INT32 iArgc, CHAR *apcArgv[])
{
	LTE_LINKSTATE_E eLteLinkStatus = {0};
    INT32 iConnectTime = 5;
    INT32 iDisConnectTime = 0;
    INT32 iRet;
	LTE_APN_T stApn = {0};
	INT32 iLteConRet = 0;
	char acGetWebValue[LEN_64];

    if ( iArgc == 2 && NULL != apcArgv[1])
    {
        iConnectTime = atoi(apcArgv[1]);
    }
    else
    {
        iConnectTime = 7;
    }
    syslog(LOG_NOTICE,"func:%s,line:%d,argc:%d,time:%d\n",__func__,__LINE__,iArgc,iConnectTime);
	
	//system("lte_startup &");
	//system("ltecomsev -m 15 -l 5 &");
	
	do
	{   /*将自动连接的时间打印到系统日志中*/
		syslog(LOG_NOTICE, "func:%s,line:%d,lteauto connect time is %ds\n",__func__,__LINE__,iConnectTime);
	    iRet = lte_get_link_status(&eLteLinkStatus);
	    if ( AP_E_NONE != iRet)
	    {	
	         printf("func:%s,line:%d,get link state failed:%d\n",__func__,__LINE__,iRet);
	    }
	    else
	    {	
	        switch ( eLteLinkStatus )
	        {
	            case LTE_LINKSTATE_DISCONNECT :
	            	syslog(LOG_NOTICE, "func:%s,line:%d,link state:disconnect\n",__func__,__LINE__);
	                //printf("func:%s,line:%d,link state:disconnect\n",__func__,__LINE__);
	                //om_lte_connect(LTE_DIAL_LINK_CHECK,LTE_CONNECT_SYN);
	                if(3 <= iDisConnectTime)
				   {
						iLteConRet = lte_disconnect();
						iLteConRet = lte_power_off();
					    iLteConRet = lte_power_on();
						// system("reboot");
				   }
	                iLteConRet = lte_connect(&stApn, 0);
                   	iDisConnectTime++;
	                //printf("iDisConnectTime = %d\n",iDisConnectTime);

			  printf("func:%s,line:%d, lteconnect return = %d.\n",__func__,__LINE__, iLteConRet);
	                break;
	            case LTE_LINKSTATE_CONNECTING :
					syslog(LOG_NOTICE, "func:%s,line:%d,link state:connect\n",__func__,__LINE__);
                    iDisConnectTime = 0;
	                //printf("func:%s,line:%d,link state:connecting\n",__func__,__LINE__);
	                break;
	            case LTE_LINKSTATE_CONNECTED :
					syslog(LOG_NOTICE, "func:%s,line:%d,link state:connect\n",__func__,__LINE__);
                    iDisConnectTime = 0;
	                //printf("func:%s,line:%d,link state:connected\n",__func__,__LINE__);
	                break;
	            case LTE_LINKSTATE_CONNECT_FAIL :
	                //printf("func:%s,line:%d,link state:connect failed\n",__func__,__LINE__);
	                //om_lte_connect(LTE_DIAL_LINK_CHECK,LTE_CONNECT_SYN);
	                
                    iDisConnectTime = 0;
	                lte_connect(&stApn, 0);
	                break;
	            default:
	                //printf("func:%s,line:%d,unknow link state:%d\n",__func__,__LINE__,eLteLinkStatus);
					;
	        }
	        
	    }
		//printf("func:%s,line:%d,argc:%d,time:%d\n",__func__,__LINE__,iArgc,iConnectTime);
	    sleep(iConnectTime);
	} while ( 1 );
    
    syslog(LOG_NOTICE, "func:%s,line:%d,lte module type{1:3G 2:4G},no auto connect",__func__,__LINE__);

    return AP_E_NONE;
}
