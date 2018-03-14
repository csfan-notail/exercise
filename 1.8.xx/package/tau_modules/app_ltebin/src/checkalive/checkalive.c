#include <stdio.h>
#include <stdlib.h>  
#include <string.h>  
#include <sys/wait.h>
#include <getopt.h>
#include <fcntl.h>  
#include "lte.h"
#include "checkalive.h"
#define LTE_PING_STATUS "/tmp/ltepingstatus"

/*设定等待时间1s,最多等待15个间隔*/
#define MISS_DEVICE_COUNTER 30
#define MISS_DEVICE_WAIT_INTERVAL_TIME 1

/*对于无有效IP时，最多等待1min，就重启模块*/
#define MISS_IP_COUNTER 10 
#define MISS_IP_WAIT_INTERVAL_TIME 3
#define MISS_IP_WAIT_INTERVAL_TIME_G3 12
/*默认用户的保活时间*/
#define DEFAULT_KEEP_ALIVE_TIME 5*60

#define DISCONNECTING_COUNT 2 
/*add by wangqi 2015-6-9 begin*/
#define START_UP_GPS 1
#define START_UP_IPV6 1
/*add by wangqi 2015-6-9 end*/

#ifndef ALTAIR_DEFAULT_CLIENT_IP
#define ALTAIR_DEFAULT_CLIENT_IP "169.254.0.10"
#endif

int g_iLogLevel = LOG_DEBUG;

struct statistic{
	int sysboot;
	int module_rest;
	int device_miss;
	int ip_miss;
	int disconnect;
	int redail;
};

struct statistic g_check_fail_statisic;
char acApn[32] = {0};     /* APN */
char acFirstIp[32] = {0}; /*the  first ip*/
#if 0
char acSecondIp[32] = {0};/*the  second ip*/
char acDns[32] = {0};/*the  dns*/
#endif
int iInterVal = 0 ;/*the  interval*/
char acResetCmd[32] = {0};/*the  cmd of reset*/
int  iNeedReboot = 0;/*is needed to reboot the system*/
/*add by wangqi 2015-6-9 begin*/
int g_iGpsStartSign = 0;
int g_iIpv6StartSign = 0;
/*add by wangqi 2015-6-9 end*/
int my_system_call(char *pcCmd)
{
	int status, i;

	for (i = 0 ; i < 2; i++){
		status = system(pcCmd);
		if (status < 0){
			LOG(LOG_WARNING, "system call fail [CMD : %s]", pcCmd);
			continue;
		}

		if (WIFEXITED(status)){
			if (0 == WEXITSTATUS(status)){
				LOG(LOG_INFO, "system call \"%s\" success", pcCmd);
				return 0;
			}
			else{
				LOG(LOG_WARNING, "system call fail [exit code: %d] [CMD: %s]", WEXITSTATUS(status), pcCmd);
			}
		}else{
			LOG(LOG_WARNING, "system call fail [exit code: %d] [CMD: %s]", WEXITSTATUS(status), pcCmd);
		}
	}
	return -1;
}
static int modify_accept_ra(int iAccept_ra)
{
    char acCmd[64] = {0};
    int  iRet = 0;
    
    //snprintf(acCmd, sizeof(acCmd),"echo %d > /proc/sys/net/ipv6/conf/usb0/accept_ra", iAccept_ra);
    snprintf(acCmd, sizeof(acCmd),"echo 0 > /proc/sys/net/ipv6/conf/usb0/accept_ra");
    iRet = my_system_call(acCmd);
    snprintf(acCmd, sizeof(acCmd),"echo 0 > /proc/sys/net/ipv6/conf/usb0/autoconf");
    iRet = my_system_call(acCmd);
    snprintf(acCmd, sizeof(acCmd),"echo 0 > /proc/sys/net/ipv6/conf/usb0/accept_ra_pinfo");
    iRet = my_system_call(acCmd);
    
    if(iRet < 0)
    {
        LOG(LOG_WARNING,"modify_accept_ra call failed!");
        return -1;
    }
    return 0;
}
/*****************************************************************************
 函 数 名  : lte_set_ping_status
 功能描述  : 设置用户的最后断开的方式
 输入参数  :  0 ok,1 error
 输出参数  : 
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月17 星期五
    作    者   : 蔡兵
    修改内容   : 新生成函数

*****************************************************************************/

int  lte_set_ping_status(int iPingStatus)
{
	int iFd = 0;
	int iRet = 0;
	int iNwrite = 0;
    char acFlag[32] = {0};
	/*设置写独占锁*/
    struct flock lock;
	/*设置写独占锁*/
	lock.l_type = F_WRLCK;
	lock.l_start = 0;
	lock.l_whence = SEEK_SET;
	lock.l_len = 0;
	iFd = open(LTE_PING_STATUS,O_CREAT | O_WRONLY | O_TRUNC);
	if(iFd < 0)
	{
	    LOG(LOG_ERR,"creat failed or open file failed");
		return -1;
	}
    sprintf(acFlag,"%d",iPingStatus);
    	/*阻塞等待*/
	iRet = fcntl(iFd,F_SETLKW,&lock);
	if(iRet <0)
	{
		LOG(LOG_ERR,"add %s lock failed",LTE_PING_STATUS);
        return -1;
	}

	iNwrite = write(iFd ,acFlag,strlen(acFlag));
    
    /*对于文件进行解锁*/
	lock.l_type = F_UNLCK;
	iRet = fcntl(iFd,F_SETLKW,&lock);
	if(iRet < 0)
	{
		LOG(LOG_ERR,"warning unlock lock failed");
	}
	close(iFd);

	if(iNwrite < 0)
	{
		LOG(LOG_ERR,"set ping status failed");
		return -1;
	}
    else
    {
        return 0;	
    }
}
#if 0
int check_device(void)
{
	LTE_DEVICE_INFO stDev;

	if (LTE_FAIL == lte_device_info_get(&stDev))
    {
		return -1;
	}
	return stDev.enDeviceNum;
}
#endif
static struct option const long_options[] = 
{
	{"-a set apn,Warning:it is a necessary parameter!",			                            required_argument,		NULL,'a'},
	{"-p the first ip address",			                                                    required_argument,		NULL,'p'},
#if 0   
    {"-s the second ip address",			                                                required_argument,		NULL,'s'},
	{"-d the dns",			                                                                required_argument,		NULL,'d'},
#endif
    {"-g ltectl gpsstart",			                                                        no_argument,		    NULL,'g'},
    {"-6 ipv6 process",			                                                            no_argument,		    NULL,'6'},
    {"-i the interval of ping",			                                                    required_argument,		NULL,'i'},
	{"-c the cmd of resetting for the bsp,Warning:it is a necessary parameter!",			required_argument,		NULL,'c'},
	{"-r times to reset module,it needs to reset?",			                                required_argument,		NULL,'r'},
	{"-h help",			                                                                    no_argument,		    NULL,'h'},
    {NULL,					0, 				NULL, 0},
};
void usage(void)
{
	int iCount = 0;
	LOG(LOG_ERR, "Usage:");
    while (long_options[iCount].name != NULL)
    {
		LOG(LOG_ERR, "%s", long_options[iCount].name);
		printf("%s\n", long_options[iCount].name);
        iCount++;
    }
    return ;
}

/* Handle arguments */
int get_parameter(int argc,char** args)
{
    int iIndex = 0 ;
    int iChar ;
    //while((iChar = getopt_long(argc, args, "a:p:s:d:i:c:r:", long_options, &iIndex)) != EOF)
    while((iChar = getopt_long(argc, args, "a:p:i:c:g6hr:", long_options, &iIndex)) != EOF)
    {

    	switch (iChar)
    	{
    		case 'a':
    		{	
    			strcpy(acApn,optarg);
    			break;
            }
    		case 'p':
    		{
    			strcpy(acFirstIp,optarg);
    			break;
    		}
#if 0
    		case 's':
    		{
    			strcpy(acSecondIp,optarg);
    			break;
    		}
    		case 'd':
    		{
    			strcpy(acDns,optarg);
    			break;
    		}
#endif
    		case 'i':
    		{
    			iInterVal = atoi(optarg);
    			break;
    		}
    		case 'c':
    		{
    			strcpy(acResetCmd,optarg);
    			break;
    		}
    		case 'r':
    		{
                /*n次模块reset 后，系统重启*/
                iNeedReboot = atoi(optarg);
    			break;
    		}
    		case 'g':
    		{
    			g_iGpsStartSign = START_UP_GPS;
    			break;
    		}
    		case '6':
    		{
    			g_iIpv6StartSign = START_UP_IPV6;
    			break;
    		}
    		case 'h':
    		{
    			usage();
    			return -1;
    		}
       		default:
    		{
    			usage();
    			return -1;
    		}
    	}
    }
    if ((0 == strlen(acApn)) || (0 == strlen(acResetCmd)))
    {
		LOG(LOG_ERR, "unset apn or unset reboot cmd");
        return -1;
    }
    return 0;
}

/*获取模块信息*/
int get_device_info(void)
{
	LTE_DEVICE_INFO stDev;
	if (LTE_FAIL == lte_device_info_get(&stDev))
    {
		LOG(LOG_ERR, "get device fail");
		return -1;
	}
	return stDev.enDeviceNum;
}

int has_valid_ip(int iModuleType)
{
	FILE *fp = NULL;
    char acInterface[8] = {0};
    char acGetIpCmd[128] = {0};
    char acGetData[128] = {0};
    char *pTmp = NULL;
    int iAltairIpVaild = 0;
/* module:
	M_LONGCHEER = 0,
	M_LEADCORE,
	M_THINKWILL,
	M_ALTAIR,
	M_ZTE,
    M_ZTE_CDMA2000,
    M_ARCCRA,
    M_THINKWILL_CDMA_MI660,
*/
	if ((iModuleType == M_THINKWILL) ||(iModuleType == M_ZTE)||(iModuleType == M_ZTE_CDMA2000)||(iModuleType == M_THINKWILL_CDMA_MI660) ||
        (iModuleType ==  M_YUGA_CDMA_CEM600) || (iModuleType ==  M_HUAWEI))
    {
		strcpy(acInterface, PPP_IF);
	}
	else
    {
		strcpy(acInterface, USB_IF);
	}
	sprintf(acGetIpCmd, "/sbin/ifconfig %s | grep \"inet addr:\"", acInterface);
	fp = popen(acGetIpCmd, "r");
	if ( fp == NULL )
    {
		LOG(LOG_ERR, "popen [%s] fail", acGetIpCmd);
		return -1;
	}
	pTmp = fgets(acGetData, sizeof(acGetData), fp);
	pclose(fp);
    if ( (pTmp != NULL) && (strlen(pTmp) != 0))
    {
        if ((iModuleType != M_ALTAIR) || (NULL == strstr(acGetData, DEFAULT_ATAIR_IP)))
        {
            return 0;
        }
    }
    
    return -1;
}
int reset_module(void)
{
	LTE_APN_T stApn;
    int iModuleType = 0;
    char acCmd[32] = {0};
    
	LOG(LOG_ERR, "we reset the module!");
 
    if (LTE_FAIL == lte_disconnect())
    {
		LOG(LOG_ERR, "disconnect fail,but we will stall reset the module");
	}
	#if 0
    my_system_call("killall ltecomsev");
    /*add by wangqi 2015-6-9 begin*/
    if(g_iGpsStartSign == START_UP_GPS)
    {
        my_system_call("killall get_gpsinfo");
    }
    if(g_iIpv6StartSign == START_UP_IPV6)
    {
        my_system_call("killall ipv6");
    }
    /*add by wangqi 2015-6-9 end*/
    /* add by wangqi 2015-7-20 begin pn:马鞍山联新模块断网问题*/
    my_system_call("killall xl2tpd");
    my_system_call("killall pppoe-server");
    /*add by wangqi 2015-7-20 end    pn:马鞍山联新模块断网问题*/
    #if 0
    /*锁住平台的调用接口，此时任何进程都进入不了*/
	if (LTE_FAIL == lte_mutex_lock())
    {
		LOG(LOG_ERR, "lte_mutex_lock fail before module_reset");
		//return -1;
	}
    #endif
    /*模块reset命令*/
	//my_system_call(acResetCmd);

    /*等待模块再次被识别*/
	while ( g_check_fail_statisic.device_miss < MISS_DEVICE_COUNTER)
    {
		if (-1 < check_device())
        {
            g_check_fail_statisic.device_miss = 0;
            break;
        } 
		g_check_fail_statisic.device_miss++;
		LOG(LOG_ERR, "get device fail time :%d",g_check_fail_statisic.device_miss);
		sleep(MISS_DEVICE_WAIT_INTERVAL_TIME);
	}
	if (g_check_fail_statisic.device_miss == MISS_DEVICE_COUNTER)
    {
		LOG(LOG_ERR, "after resetting,can't find the dule");
        return -1;
    }  
#endif
    iModuleType = is_device_found();
    if ( iModuleType < 0 )
    {
        LOG(LOG_ERR, "can't find the module finally");
        #if 0
       	if (LTE_FAIL == lte_mutex_unlock())
        {
    		LOG(LOG_ERR, "lte_mutex_unlock fail before module_reset");
    	}
        #endif
        return -1;
    }
    /*针对华为4G模块，添加转换流程。add by wangqi*/
    if(M_HUAWEI4G_STORAGE == iModuleType)
    {
        system("usb_modeswitch -J -v 12d1 -p 14fe");
        iModuleType = M_HUAWEI4G_NETCARD;
    }
#if 0
    /*设备发现与驱动绑定之间需要一定的时间,驱动生成ttyUSB*端口也需要一定时间,因此在这边添加睡眠*/
    //sleep(10);
	if (LTE_FAIL == lte_mutex_unlock())
    {
		LOG(LOG_ERR, "lte_mutex_unlock fail before module_reset");
	}
#endif
    //snprintf(acCmd, sizeof(acCmd), "ltecomsev -m %d -l 5 &", iModuleType);
    //my_system_call(acCmd);
    //sleep(5);
	/* altair模块拨号前需要给usb0配个默认地址 "169.254.0.10" */
    if(M_ALTAIR == iModuleType)
    {
        my_system_call("ifconfig usb0 "ALTAIR_DEFAULT_CLIENT_IP" up");
    }
    
    if(M_ZTE_MICRO == iModuleType || M_HUAWEI4G_NETCARD == iModuleType )//|| M_LEADCORE == iModuleType 
    {
        if(LTE_FAIL == lte_power_off())
        {
            sleep(1);
            lte_power_off();
        }
        sleep(4);
        if(LTE_FAIL == lte_power_on())
        {
            sleep(4);
            lte_power_on();
        }
        sleep(2);
    }
    /*重新拨号*/
    strcpy(stApn.acApn,acApn);
    /*add by wangqi begin*/
    /*if (LTE_FAIL == lte_disconnect())
    {
		LOG(LOG_ERR, "disconnect fail,but we will stall reset the module");
	}*/
	/*add by wangqi end*/
    /*一定是非异步的*/
	if (LTE_FAIL == lte_connect(&stApn, 0))
    {
    	LOG(LOG_ERR, "connect fail");
	}
	/*add by wangqi 2015-6-9 begin*/
	if(g_iGpsStartSign == START_UP_GPS)
	{    
        if (LTE_FAIL == lte_gps_func_start())
        {
        	LOG(LOG_ERR, "gps func start fail");
    	}
    	system("get_gpsinfo > /dev/null&");
	}
	if(g_iIpv6StartSign == START_UP_IPV6)
    {
        my_system_call("ipv6 > /dev/null&");
    }
	/*add by wangqi 2015-6-9 end*/
    return 0;
    	
}

int ping(char *paddress)
{
    char acPingCmd[64] = {0};
    char  acGetData[128] = {0};
    FILE *fp = NULL;
    char *pTmp = NULL;
    char acNeedle[32] = {"100% packet loss"};
    sprintf(acPingCmd,"%s %s %s","/tmp/ping -c 2",paddress,"| grep \"packets transmitted\"");
	fp = popen(acPingCmd, "r");
	if ( fp == NULL )
    {
		LOG(LOG_ERR, "popen [%s] fail", acPingCmd);
		return -1;
	}
	pTmp = fgets(acGetData, sizeof(acGetData), fp);
	pclose(fp);
    
    if ( pTmp != NULL )
    {
        /*未找到则说明ping 包有效*/
        if ( strstr(acGetData, acNeedle) == NULL )
        {
            return 0;
        }
    }
    /*其余均是ping包无效*/
    return -1;

}
int check_alive(void)
{
	/* default ip :  baidu  &  aliDNS  */
    char default_ip1[32] = "202.108.22.5", default_ip2[32] = "223.5.5.5";
    /*一包不通大概3s*/
    /*用户指定第一个地址*/
    if (strlen(acFirstIp) > 0)
    {
        if ( 0 == ping(acFirstIp) )
        {
            return 0;
        }
        /*保活默认第一地址*/ 
        if ( 0 == ping(default_ip1) )
        {
            return 0;
        }
		#if 0
        /*保活默认第二地址*/ 	
        if ( 0 == ping(default_ip2) )
        {
            return 0;
        }
        #endif
        return -1;
    }
    
    return 0;

}

void is_need_reboot( void )
{
    /*如果设置了重启指定次数模块后，重启系的统标志，则进行如下操作*/
    if ( iNeedReboot > 0)
    {
        /*到达指定次数后，系统重启*/
        if ( g_check_fail_statisic.module_rest == iNeedReboot )
        {
        	my_system_call(SYS_REBOOT);
        }
    }
}
/*确认模块是否存在的*/
int is_device_found(void)
{
    int iModuleType = 0;
    int iRet = 0;
    /*每个3s 查看一次设备，15次未发现则reset module*/
    while ( g_check_fail_statisic.device_miss <  MISS_DEVICE_COUNTER)
    {
        iModuleType = get_device_info();
        if (-1 == iModuleType)
        {
            g_check_fail_statisic.device_miss++;
            LOG(LOG_ERR, "can't find the module,the %d time",g_check_fail_statisic.device_miss);
            sleep(MISS_DEVICE_WAIT_INTERVAL_TIME);
        }
        else
        {
        	LOG(LOG_NOTICE, "find the module");
            g_check_fail_statisic.device_miss = 0;
            return iModuleType;
        }
    }

    /*指定次数未找到模块，则重启模块*/
    if ( MISS_DEVICE_COUNTER == g_check_fail_statisic.device_miss)
    {
        /*恢复device_miss的初始值*/
        g_check_fail_statisic.device_miss = 0;
        return -1;
    }    
    return -1;
}
/*确认模块是否存在的*/
int is_device_exist(void)
{
    int iModuleType = 0;
    int iRet = 0;
    while ( 1 )
    {
        /*每个1s 查看一次设备，30次未发现则reset module*/
        while ( g_check_fail_statisic.device_miss <  MISS_DEVICE_COUNTER)
        {
            iModuleType = get_device_info();
            if (-1 == iModuleType)
            {
                g_check_fail_statisic.device_miss++;
                LOG(LOG_ERR, "can't find the module,the %d time",g_check_fail_statisic.device_miss);
                sleep(MISS_DEVICE_WAIT_INTERVAL_TIME);
            }
            else
            {
            	LOG(LOG_NOTICE, "find the module");
                g_check_fail_statisic.device_miss = 0;
                g_check_fail_statisic.module_rest = 0;
                /*循环的唯一出口*/
                return iModuleType;
            }
        }

        /*指定次数未找到模块，则重启模块*/
        if ( MISS_DEVICE_COUNTER == g_check_fail_statisic.device_miss)
        {
            is_need_reboot();
            reset_module();
            g_check_fail_statisic.module_rest++;
            /*清掉源于device_miss带来的module reset 标记*/
            g_check_fail_statisic.device_miss = 0;
        }    
    }
    return -1;
}
/*确认有效的ip地址存在,一般地址均存在*/
int is_valid_ip_exist(int iModuleType)
{
    int iRet = 0;
    while (1)
    {
        /*每个3s获取一次ip地址，20次不成功则reset module*/
        while ( g_check_fail_statisic.ip_miss < MISS_IP_COUNTER )
        {
            iRet = has_valid_ip(iModuleType);
            if (-1 == iRet)
	        {

                //内部涉及到3g重新拨号,模块reset,因此将状态变化提前设置
                //不然的话，设备反馈的状态不会那么的及时
                /*ping failed*/
                /*altered by caibing 2015-03-16*/
                lte_set_ping_status(1);
            
                g_check_fail_statisic.ip_miss++;
	      		LOG(LOG_ERR, "can't find the ip,the %d time",g_check_fail_statisic.ip_miss);
	      		if ((iModuleType == M_THINKWILL) ||(iModuleType == M_ZTE)||(iModuleType == M_ZTE_CDMA2000)||(iModuleType == M_THINKWILL_CDMA_MI660) ||
                    (iModuleType ==  M_YUGA_CDMA_CEM600) || (iModuleType ==  M_HUAWEI))
  				{
					sleep(MISS_IP_WAIT_INTERVAL_TIME_G3);
				}
				else
				{
				    sleep(MISS_IP_WAIT_INTERVAL_TIME);
				}
	        }
            else
            {
        		LOG(LOG_DEBUG, "find the valid ip");
                /* 模块重启，获取到地址后修改ipv6的accept_ra配置 */
                if(g_check_fail_statisic.module_rest > 0)
                {
                    modify_accept_ra(2);
                }
                g_check_fail_statisic.ip_miss = 0;
                g_check_fail_statisic.module_rest = 0;
                /*唯一的出口*/
                return 0;
            }
        }
        if ( MISS_IP_COUNTER == g_check_fail_statisic.ip_miss)
        {
            //内部涉及到3g重新拨号,模块reset,因此将状态变化提前设置
            //不然的话，设备反馈的状态不会那么的及时
            /*ping failed*/
            /*altered by caibing 2015-03-16*/
            lte_set_ping_status(1);
            
            is_need_reboot();
            reset_module();
            g_check_fail_statisic.module_rest++;
            /*清掉源于ip_miss带来的module reset 标记*/
            g_check_fail_statisic.ip_miss = 0;
        }
    }
}

/*3g模块网络不通，尝试重新拨号*/
int checkalive_3g_again_dial(void)
{
	LTE_APN_T stApn;
    if (LTE_FAIL == lte_disconnect())
    {
		LOG(LOG_ERR, "disconnect fail in checkalive_3g_again_dial");
        return -1;
	}
    
    strcpy(stApn.acApn,acApn);
    if (LTE_FAIL == lte_connect(&stApn, 0))
    {
		LOG(LOG_ERR, "connect fail in checkalive_3g_again_dial");
        return -1;
	}
  
    return 0;

}

/*网络是否联通*/
int is_connect(int iModuleType)
{
    /*判断网路是否联通*/
    if ( 0 ==  check_alive())
    {
        /* 模块重启，获取到地址后修改ipv6的accept_ra配置 */
        if(g_check_fail_statisic.module_rest > 0)
        {
            modify_accept_ra(2);
        }
        g_check_fail_statisic.disconnect = 0;
    	g_check_fail_statisic.module_rest = 0;
        return 0;
    }
    else
    {
        g_check_fail_statisic.disconnect++;
    }

    if ( DISCONNECTING_COUNT == g_check_fail_statisic.disconnect )
    {
        //内部涉及到3g重新拨号,模块reset,因此将状态变化提前设置
        //不然的话，设备反馈的状态不会那么的及时
        /*ping failed*/
        /*altered by caibing 2015-03-16*/
        lte_set_ping_status(1);
        
        if ((iModuleType == M_THINKWILL) ||(iModuleType == M_ZTE)||(iModuleType == M_ZTE_CDMA2000)||(iModuleType == M_THINKWILL_CDMA_MI660) ||
            (iModuleType ==  M_YUGA_CDMA_CEM600) || (iModuleType ==  M_HUAWEI))
		{
            if( 0 == checkalive_3g_again_dial() )
            {
                
                LOG(LOG_ERR, "[checkalive_3g_again_dial] success");
                g_check_fail_statisic.disconnect = 0;
                return -1;
            }
		}
        
        is_need_reboot();
        reset_module();
        g_check_fail_statisic.module_rest++;
        /*清掉源于disconnect带来的module reset 标记*/
        g_check_fail_statisic.disconnect = 0;
    }
    return -1;
}

int main(int argc,char** args)
{
    int iRet = 0;
    /*模块类型*/
    int iModuleType = 0;       
    /*获取参数*/
    iRet = get_parameter(argc,args);
    if (-1 == iRet)
    {
        return -1;
    }
    else
    {
		LOG(LOG_NOTICE, "parameter OK!");
    }
    if (iInterVal == 0)
    {
        /*用户保活间隔默认五分钟*/
        iInterVal = DEFAULT_KEEP_ALIVE_TIME;
    }
	/* 设备启动后拨号与checkalive的检测冲突，等待拨号成功后开始检测，使用睡眠3分钟实现（首次拨号，搜网大概需要2分钟） */
    sleep(80);

    /*首次一定要找到模块，否则一直等待模块或者重启系统*/
    iModuleType =  is_device_exist();

    /*找到模块才能初始化*/
	if (LTE_FAIL == lte_init(g_iLogLevel))
    {
		LOG(LOG_ERR, "lte_init error");
		return 0;
	}
    /* 模块起来以后，修改ipv6相关的accept_ra配置, 修改为2 */
    modify_accept_ra(2);

    while ( 1 )
    {        
        /*直到获取有效ip地址*/
        is_valid_ip_exist(iModuleType);
        /*查看网络状态*/
        iRet = is_connect(iModuleType);
        if (0 == iRet)
        {
            /*ping OK*/
            lte_set_ping_status(0);
            sleep(iInterVal);
        }
        else
        {
            /*ping failed*/
            lte_set_ping_status(1);
        }
    }
	return 0;
}
