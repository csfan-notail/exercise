/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : trackint.c
  版 本 号   : 初稿
  作    者   : xiao wei
  生成日期   : 2016年8月27日 星期一
  最近修改   :
  功能描述   :
  函数列表   :
  修改历史   :
  1.日    期   : 2016年8月27日 星期一
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
#include <syslog.h>
#include <time.h>
#include <getopt.h>

#include "om_include/om_net.h"
#include "ap_types.h"

//int om_net_lan_link_status( OM_NET_LINK_STA *pstPalLinkSta )
//pstPalLinkSta->uiLinkPort1

#define TRACKINT_LAN_PORT_UP			1
#define TRACKINT_LAN_PORT_DOWN			0

INT32  led_lte_dial_status()
{

	App_log(LOG_NOTICE, "lte_linkstate_get filed ! ");
	//return AP_E_DEFAULT_FAIL;
	
	printf("[%s|%s:%d]  \n",__FILE__,__FUNCTION__,__LINE__);	
    

    return AP_E_NONE;
}

int main(int iArgc, char *apcArgv[])
{
    int iFlag = 0;
    int iRet = 0;
    int iInterface = 0;
    int iTime = 500;
    char acCmd[32] = {0};
    int iCount = 0;
    int iLan = 0;
    
    OM_NET_LINK_STA stPalLinkStaNew = {0};
    OM_NET_LINK_STA stPalLinkStaOld = {0};
    
    char c;
	int option_index = 1;	
    static const char short_options[] = "i:l:t:h";
    static const struct option long_options[] = {
        { "interface",      		1, 0, 'i' },    // down/up interface
        { "lan",                1, 0, 'l' },   //lan
        { "time",      				1, 0, 't' },    // loop time
        { "help",          			0, 0, 'h' },
        { NULL,         			0, 0,  0  }
	};
    
    while (1) 
	{	    
		c = getopt_long(iArgc, apcArgv, short_options, long_options, &option_index);
		if (c == -1) {
			break;
		}
		switch (c) {
		case 'i':
			iInterface  = atoi(optarg);
			break; 
		case 't':
			iTime = atoi(optarg);
			break;
		/*添加-l指令，用来设置LAN口*/
		case 'l':
			iLan  = atoi(optarg);
			break; 	         
		default:
			printf("Command Error ! please input: trackint -h !\n");
			printf("-i : eth1 , eth1.x br-lan ... ; eg: -i 2\n");
			printf("-l : lan1 , lan2 ... ; eg: -l 2\n");	
			printf("-t : 100 ms 500ms ... ; eg: -t 500 \n");	
			return 0;
		    ;
		}
    }

    printf("[%s:%d] interface=%d time=%d\n",__FUNCTION__,__LINE__,iInterface,iTime);
    syslog(LOG_NOTICE, "func:%s,line:%d, start ! ",__func__,__LINE__);
    	
    om_net_lan_link_status(&stPalLinkStaOld);
    	
	do{
		iRet = om_net_lan_link_status(&stPalLinkStaNew);
		if(iRet==0)
		{
			snprintf(acCmd,32,"%s","");
			switch(iLan)
			{
			case 1:
				if( stPalLinkStaNew.uiLinkPort1 != stPalLinkStaOld.uiLinkPort1)
				{
					if(stPalLinkStaNew.uiLinkPort1)
					{
						snprintf(acCmd,32,"ifconfig eth1.%d up",iInterface);
					}else{
						snprintf(acCmd,32,"ifconfig eth1.%d down",iInterface);
					}
					system(acCmd);
					printf("[%s:%d] acCmd=%s\n",__FUNCTION__,__LINE__,acCmd);
					syslog(LOG_NOTICE, "func:%s,line:%d, acCmd=%s ! ",__func__,__LINE__,acCmd);
					
					stPalLinkStaOld.uiLinkPort1 = stPalLinkStaNew.uiLinkPort1;
					stPalLinkStaOld.uiLinkPort2 = stPalLinkStaNew.uiLinkPort2;
					stPalLinkStaOld.uiLinkPort3 = stPalLinkStaNew.uiLinkPort3;
					stPalLinkStaOld.uiLinkPort4 = stPalLinkStaNew.uiLinkPort4;
				}
				break;
			case 2:
				if( stPalLinkStaNew.uiLinkPort2 != stPalLinkStaOld.uiLinkPort2)
				{
					if(stPalLinkStaNew.uiLinkPort2)
					{
						snprintf(acCmd,32,"ifconfig eth1.%d up",iInterface);
					}else{
						snprintf(acCmd,32,"ifconfig eth1.%d down",iInterface);
					}
					system(acCmd);
					printf("[%s:%d] acCmd=%s\n",__FUNCTION__,__LINE__,acCmd);
					syslog(LOG_NOTICE, "func:%s,line:%d, acCmd=%s ! ",__func__,__LINE__,acCmd);
					
					stPalLinkStaOld.uiLinkPort1 = stPalLinkStaNew.uiLinkPort1;
					stPalLinkStaOld.uiLinkPort2 = stPalLinkStaNew.uiLinkPort2;
					stPalLinkStaOld.uiLinkPort3 = stPalLinkStaNew.uiLinkPort3;
					stPalLinkStaOld.uiLinkPort4 = stPalLinkStaNew.uiLinkPort4;
				}
				break;
			case 3:
				if( stPalLinkStaNew.uiLinkPort3 != stPalLinkStaOld.uiLinkPort3)
				{
					if(stPalLinkStaNew.uiLinkPort3)
					{
						snprintf(acCmd,32,"ifconfig eth1.%d up",iInterface);
					}else{
						snprintf(acCmd,32,"ifconfig eth1.%d down",iInterface);
					}
					system(acCmd);
					printf("[%s:%d] acCmd=%s\n",__FUNCTION__,__LINE__,acCmd);
					syslog(LOG_NOTICE, "func:%s,line:%d, acCmd=%s ! ",__func__,__LINE__,acCmd);
					
					stPalLinkStaOld.uiLinkPort1 = stPalLinkStaNew.uiLinkPort1;
					stPalLinkStaOld.uiLinkPort2 = stPalLinkStaNew.uiLinkPort2;
					stPalLinkStaOld.uiLinkPort3 = stPalLinkStaNew.uiLinkPort3;
					stPalLinkStaOld.uiLinkPort4 = stPalLinkStaNew.uiLinkPort4;
				}
				break;
			case 4:
				if( stPalLinkStaNew.uiLinkPort4 != stPalLinkStaOld.uiLinkPort4)
				{
					if(stPalLinkStaNew.uiLinkPort4)
					{
						snprintf(acCmd,32,"ifconfig eth1.%d up",iInterface);
					}else{
						snprintf(acCmd,32,"ifconfig eth1.%d down",iInterface);
					}
					system(acCmd);
					printf("[%s:%d] acCmd=%s\n",__FUNCTION__,__LINE__,acCmd);
					syslog(LOG_NOTICE, "func:%s,line:%d, acCmd=%s ! ",__func__,__LINE__,acCmd);
					
					stPalLinkStaOld.uiLinkPort1 = stPalLinkStaNew.uiLinkPort1;
					stPalLinkStaOld.uiLinkPort2 = stPalLinkStaNew.uiLinkPort2;
					stPalLinkStaOld.uiLinkPort3 = stPalLinkStaNew.uiLinkPort3;
					stPalLinkStaOld.uiLinkPort4 = stPalLinkStaNew.uiLinkPort4;
				}
				break;
			default:
				break;
			}
		}
		usleep(iTime*1000);
		
	}while(1);
    
    syslog(LOG_NOTICE, "func:%s,line:%d, exit ! ",__func__,__LINE__);

    return AP_E_NONE;
}