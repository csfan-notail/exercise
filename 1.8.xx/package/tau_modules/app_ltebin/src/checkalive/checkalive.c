#include <stdio.h>
#include <stdlib.h>  
#include <string.h>  
#include <sys/wait.h>
#include <getopt.h>
#include <fcntl.h>  
#include "lte.h"
#include "checkalive.h"
#define LTE_PING_STATUS "/tmp/ltepingstatus"

/*�趨�ȴ�ʱ��1s,���ȴ�15�����*/
#define MISS_DEVICE_COUNTER 30
#define MISS_DEVICE_WAIT_INTERVAL_TIME 1

/*��������ЧIPʱ�����ȴ�1min��������ģ��*/
#define MISS_IP_COUNTER 10 
#define MISS_IP_WAIT_INTERVAL_TIME 3
#define MISS_IP_WAIT_INTERVAL_TIME_G3 12
/*Ĭ���û��ı���ʱ��*/
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
 �� �� ��  : lte_set_ping_status
 ��������  : �����û������Ͽ��ķ�ʽ
 �������  :  0 ok,1 error
 �������  : 
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��17 ������
    ��    ��   : �̱�
    �޸�����   : �����ɺ���

*****************************************************************************/

int  lte_set_ping_status(int iPingStatus)
{
	int iFd = 0;
	int iRet = 0;
	int iNwrite = 0;
    char acFlag[32] = {0};
	/*����д��ռ��*/
    struct flock lock;
	/*����д��ռ��*/
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
    	/*�����ȴ�*/
	iRet = fcntl(iFd,F_SETLKW,&lock);
	if(iRet <0)
	{
		LOG(LOG_ERR,"add %s lock failed",LTE_PING_STATUS);
        return -1;
	}

	iNwrite = write(iFd ,acFlag,strlen(acFlag));
    
    /*�����ļ����н���*/
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
                /*n��ģ��reset ��ϵͳ����*/
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

/*��ȡģ����Ϣ*/
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
    /* add by wangqi 2015-7-20 begin pn:��ɽ����ģ���������*/
    my_system_call("killall xl2tpd");
    my_system_call("killall pppoe-server");
    /*add by wangqi 2015-7-20 end    pn:��ɽ����ģ���������*/
    #if 0
    /*��סƽ̨�ĵ��ýӿڣ���ʱ�κν��̶����벻��*/
	if (LTE_FAIL == lte_mutex_lock())
    {
		LOG(LOG_ERR, "lte_mutex_lock fail before module_reset");
		//return -1;
	}
    #endif
    /*ģ��reset����*/
	//my_system_call(acResetCmd);

    /*�ȴ�ģ���ٴα�ʶ��*/
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
    /*��Ի�Ϊ4Gģ�飬���ת�����̡�add by wangqi*/
    if(M_HUAWEI4G_STORAGE == iModuleType)
    {
        system("usb_modeswitch -J -v 12d1 -p 14fe");
        iModuleType = M_HUAWEI4G_NETCARD;
    }
#if 0
    /*�豸������������֮����Ҫһ����ʱ��,��������ttyUSB*�˿�Ҳ��Ҫһ��ʱ��,�����������˯��*/
    //sleep(10);
	if (LTE_FAIL == lte_mutex_unlock())
    {
		LOG(LOG_ERR, "lte_mutex_unlock fail before module_reset");
	}
#endif
    //snprintf(acCmd, sizeof(acCmd), "ltecomsev -m %d -l 5 &", iModuleType);
    //my_system_call(acCmd);
    //sleep(5);
	/* altairģ�鲦��ǰ��Ҫ��usb0���Ĭ�ϵ�ַ "169.254.0.10" */
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
    /*���²���*/
    strcpy(stApn.acApn,acApn);
    /*add by wangqi begin*/
    /*if (LTE_FAIL == lte_disconnect())
    {
		LOG(LOG_ERR, "disconnect fail,but we will stall reset the module");
	}*/
	/*add by wangqi end*/
    /*һ���Ƿ��첽��*/
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
        /*δ�ҵ���˵��ping ����Ч*/
        if ( strstr(acGetData, acNeedle) == NULL )
        {
            return 0;
        }
    }
    /*�������ping����Ч*/
    return -1;

}
int check_alive(void)
{
	/* default ip :  baidu  &  aliDNS  */
    char default_ip1[32] = "202.108.22.5", default_ip2[32] = "223.5.5.5";
    /*һ����ͨ���3s*/
    /*�û�ָ����һ����ַ*/
    if (strlen(acFirstIp) > 0)
    {
        if ( 0 == ping(acFirstIp) )
        {
            return 0;
        }
        /*����Ĭ�ϵ�һ��ַ*/ 
        if ( 0 == ping(default_ip1) )
        {
            return 0;
        }
		#if 0
        /*����Ĭ�ϵڶ���ַ*/ 	
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
    /*�������������ָ������ģ�������ϵ��ͳ��־����������²���*/
    if ( iNeedReboot > 0)
    {
        /*����ָ��������ϵͳ����*/
        if ( g_check_fail_statisic.module_rest == iNeedReboot )
        {
        	my_system_call(SYS_REBOOT);
        }
    }
}
/*ȷ��ģ���Ƿ���ڵ�*/
int is_device_found(void)
{
    int iModuleType = 0;
    int iRet = 0;
    /*ÿ��3s �鿴һ���豸��15��δ������reset module*/
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

    /*ָ������δ�ҵ�ģ�飬������ģ��*/
    if ( MISS_DEVICE_COUNTER == g_check_fail_statisic.device_miss)
    {
        /*�ָ�device_miss�ĳ�ʼֵ*/
        g_check_fail_statisic.device_miss = 0;
        return -1;
    }    
    return -1;
}
/*ȷ��ģ���Ƿ���ڵ�*/
int is_device_exist(void)
{
    int iModuleType = 0;
    int iRet = 0;
    while ( 1 )
    {
        /*ÿ��1s �鿴һ���豸��30��δ������reset module*/
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
                /*ѭ����Ψһ����*/
                return iModuleType;
            }
        }

        /*ָ������δ�ҵ�ģ�飬������ģ��*/
        if ( MISS_DEVICE_COUNTER == g_check_fail_statisic.device_miss)
        {
            is_need_reboot();
            reset_module();
            g_check_fail_statisic.module_rest++;
            /*���Դ��device_miss������module reset ���*/
            g_check_fail_statisic.device_miss = 0;
        }    
    }
    return -1;
}
/*ȷ����Ч��ip��ַ����,һ���ַ������*/
int is_valid_ip_exist(int iModuleType)
{
    int iRet = 0;
    while (1)
    {
        /*ÿ��3s��ȡһ��ip��ַ��20�β��ɹ���reset module*/
        while ( g_check_fail_statisic.ip_miss < MISS_IP_COUNTER )
        {
            iRet = has_valid_ip(iModuleType);
            if (-1 == iRet)
	        {

                //�ڲ��漰��3g���²���,ģ��reset,��˽�״̬�仯��ǰ����
                //��Ȼ�Ļ����豸������״̬������ô�ļ�ʱ
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
                /* ģ����������ȡ����ַ���޸�ipv6��accept_ra���� */
                if(g_check_fail_statisic.module_rest > 0)
                {
                    modify_accept_ra(2);
                }
                g_check_fail_statisic.ip_miss = 0;
                g_check_fail_statisic.module_rest = 0;
                /*Ψһ�ĳ���*/
                return 0;
            }
        }
        if ( MISS_IP_COUNTER == g_check_fail_statisic.ip_miss)
        {
            //�ڲ��漰��3g���²���,ģ��reset,��˽�״̬�仯��ǰ����
            //��Ȼ�Ļ����豸������״̬������ô�ļ�ʱ
            /*ping failed*/
            /*altered by caibing 2015-03-16*/
            lte_set_ping_status(1);
            
            is_need_reboot();
            reset_module();
            g_check_fail_statisic.module_rest++;
            /*���Դ��ip_miss������module reset ���*/
            g_check_fail_statisic.ip_miss = 0;
        }
    }
}

/*3gģ�����粻ͨ���������²���*/
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

/*�����Ƿ���ͨ*/
int is_connect(int iModuleType)
{
    /*�ж���·�Ƿ���ͨ*/
    if ( 0 ==  check_alive())
    {
        /* ģ����������ȡ����ַ���޸�ipv6��accept_ra���� */
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
        //�ڲ��漰��3g���²���,ģ��reset,��˽�״̬�仯��ǰ����
        //��Ȼ�Ļ����豸������״̬������ô�ļ�ʱ
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
        /*���Դ��disconnect������module reset ���*/
        g_check_fail_statisic.disconnect = 0;
    }
    return -1;
}

int main(int argc,char** args)
{
    int iRet = 0;
    /*ģ������*/
    int iModuleType = 0;       
    /*��ȡ����*/
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
        /*�û�������Ĭ�������*/
        iInterVal = DEFAULT_KEEP_ALIVE_TIME;
    }
	/* �豸�����󲦺���checkalive�ļ���ͻ���ȴ����ųɹ���ʼ��⣬ʹ��˯��3����ʵ�֣��״β��ţ����������Ҫ2���ӣ� */
    sleep(80);

    /*�״�һ��Ҫ�ҵ�ģ�飬����һֱ�ȴ�ģ���������ϵͳ*/
    iModuleType =  is_device_exist();

    /*�ҵ�ģ����ܳ�ʼ��*/
	if (LTE_FAIL == lte_init(g_iLogLevel))
    {
		LOG(LOG_ERR, "lte_init error");
		return 0;
	}
    /* ģ�������Ժ��޸�ipv6��ص�accept_ra����, �޸�Ϊ2 */
    modify_accept_ra(2);

    while ( 1 )
    {        
        /*ֱ����ȡ��Чip��ַ*/
        is_valid_ip_exist(iModuleType);
        /*�鿴����״̬*/
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
