#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <sys/wait.h>
#include <unistd.h>


#include "libusb.h"
#include "lte.h"
#include "lte_startup.h"

#define MODULE_LENGTH 128
#define CMD_LENGTH 128
#define MODULE_PATH "/lib/modules/3.3.8/"
#define ICCID_FILE_PATH     "/tmp/zte_iccid.file"


#define ALTAIR_CLIENT_IP "169.254.0.10"

int g_iLogLevel = LOG_INFO;
char acResetCmd[32] = {0};/*the  cmd of reset*/
int  g_iIPStack = 0;
char g_acMacAddr[32] = {0};
int  g_iFlag = 0;
char g_acApn[32] = {0};

DEVICE_INFO g_acDeviceInfo[M_TOTAL_NUM] = {
    {M_LONGCHEER, 0x1c9e, 0x9b05, "usbserial.ko vendor=0x1c9e product=0x9b05;lc_ether.ko;"}, /*龙旗平台*/
    /* BEGIN: Modified by zhangjiasi, 2015/4/20   PN:平台驱动导致的堆栈打印，和设备重启问题，规避方法 */
    //{M_LEADCORE, 0x1ab7, 0x1761, "usbserial.ko;lc_lteacm.ko;lc_ltencm.ko;lc_ltetty.ko;"}, /* 联芯平台 */
    {M_LEADCORE, 0x1ab7, 0x1761, "usbserial.ko;lc_lteacm.ko;lc_ltetty.ko;"}, /* 联芯平台 */
    /* END:   Modified by zhangjiasi, 2015/4/20   PN:平台驱动导致的堆栈打印，和设备重启问题，规避方法 */
    {M_THINKWILL, 0x19f5, 0x9013, 
    "usbserial.ko vendor=0x19f5 product=0x9013;slhc.ko;ppp_generic.ko;ppp_synctty.ko;"\
    "bsd_comp.ko;ppp_deflate.ko;pppox.ko;pppoe.ko;crc-ccitt.ko;ppp_async.ko;pppol2tp.ko;"}, /* 信可平台  */
    {M_ALTAIR, 0x216f, 0x0043, "usbnet.ko;cdc_ether.ko;rndis_host.ko;"}, /* 德明平台 */
    /*altered by caibing 2014/09/12*/
    {M_ZTE, 0x19d2, 0x0117, "usbserial.ko vendor=0x19d2 product=0x0117;"\
    "slhc.ko;ppp_generic.ko;ppp_synctty.ko;bsd_comp.ko;ppp_deflate.ko;"\
    "pppox.ko;pppoe.ko;crc-ccitt.ko;ppp_async.ko;pppol2tp.ko;"}, /* 中兴平台 */
    /* BEGIN: Added by liyoukun, 2014/8/11   PN:添加ZTE的CDMA2000平台 */
    /*altered by caibing 2014/09/12*/
    {M_ZTE_CDMA2000, 0x19d2, 0xffed, "usbserial.ko vendor=0x19d2 product=0xffed;"\
    "slhc.ko;ppp_generic.ko;ppp_synctty.ko;bsd_comp.ko;ppp_deflate.ko;"\
    "pppox.ko;pppoe.ko;crc-ccitt.ko;ppp_async.ko;pppol2tp.ko;"}, /* 中兴CDMA2000平台 */    
    /* END:   Added by liyoukun, 2014/8/11   PN:添加ZTE的CDMA2000平台 */
    /* BEGIN: Added by liyoukun, 2014/8/12   PN:添加乾润平台 */
    {M_ARCCRA, 0x29e0, 0x0303, "lc_ether.ko;usbserial.ko vendor=0x29e0 product=0x0303;"}, /*乾润平台*/
    /* END:   Added by liyoukun, 2014/8/12   PN:添加乾润平台 */
    {M_THINKWILL_CDMA_MI660, 0x19f5, 0x9909, "usbserial.ko vendor=0x19f5 product=0x9909;"\
    "slhc.ko;ppp_generic.ko;ppp_synctty.ko;bsd_comp.ko;ppp_deflate.ko;"\
    "pppox.ko;pppoe.ko;crc-ccitt.ko;ppp_async.ko;pppol2tp.ko;"}, /* 信可cdma mi660平台 */  
    {M_YUGA_CDMA_CEM600, 0x257a, 0x1601, "usbserial.ko vendor=0x257a product=0x1601;"\
    "slhc.ko;ppp_generic.ko;ppp_synctty.ko;bsd_comp.ko;ppp_deflate.ko;"\
    "pppox.ko;pppoe.ko;crc-ccitt.ko;ppp_async.ko;pppol2tp.ko;"}, /* 域格cdma cem600平台 */ 
    {M_ARCCRA_SECOND, 0x29e0, 0x0304, "lc_ether.ko;usbserial.ko vendor=0x29e0 product=0x0304;"}, /* ARCCRA 4 模平台 */ 
    {M_ZTE_MICRO, 0x19d2, 0x0199, "usbnet.ko;cdc_encap_mod.ko;cdc_ether_mod.ko;usbserial.ko;usb_wwan.ko;option.ko;"}, /* zte micro 平台 */ 
    {M_HUAWEI, 0x12d1, 0x1c25, "usbnet.ko;cdc_ether.ko;usbserial.ko;usb_wwan.ko;option.ko;"}, /* 华为平台 */
    {M_YUGA, 0x05c6, 0x9025, "usbserial.ko vendor=0x05c6 product=0x9025;lc_ether.ko;"}, /* 域格4g平台 */
    {M_MARVELL, 0x1286, 0x4e31, "usbnet.ko;cdc_ether.ko;rndis_host.ko;usbserial.ko vendor=0x1286 product=0x4e31;"}, /* 马维尔平台 */
    {M_HUAWEI4G_STORAGE, 0x12d1, 0x14fe, "usbserial.ko;usb_wwan.ko;option.ko;usbnet.ko;cdc_ncm.ko;"}, /* 华为4G平台 */
    {M_HUAWEI4G_NETCARD, 0x12d1, 0x1506, "usbserial.ko;usb_wwan.ko;option.ko;usbnet.ko;cdc_ncm.ko;"}, /* 华为4G平台 */
};
static struct option const long_options[] = 
{
    {"-s ip sigle or double stack config,Warning:it is a necessary parameter!",  required_argument,	NULL,'c'},
    {"-a ip stack apn config,Warning:it is a necessary parameter!",              required_argument,	NULL,'a'},
	{"-c the cmd of resetting for the bsp,Warning:it is a necessary parameter!", required_argument,	NULL,'c'},
	{"-m mac addr",			                                                     required_argument,	NULL,'m'},
	{"-h help",			                                                         no_argument,		NULL,'h'},
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
#if 0
int system_call(char *pcCmd)
{
	int status;

	status = system(pcCmd);
	if (status < 0){
		LOG(LOG_ERR, "[lte_startup] system call fail");
		return -1;
	}

	if (WIFEXITED(status)){
		if (0 == WEXITSTATUS(status)){
			LOG(LOG_INFO, "[lte_startup] system call \"%s\" success", pcCmd);
			return 0;
		}
		else{
			LOG(LOG_ERR, "[lte_startup] system call fail [exit code: %d]", WEXITSTATUS(status));
			return -1;
		}
	}else{
		LOG(LOG_ERR, "[lte_startup] system call fail [exit code: %d]", WEXITSTATUS(status));
		return -1;
	}
}
#endif
int load_modules(char *pcModules)
{
	int loop, start, len;
	char acModule[MODULE_LENGTH];
	char acCmd[CMD_LENGTH];

	LOG(LOG_INFO, "[lte_startup] MODULES: %s", pcModules);
	
	start = 0;
	for(loop = 0; pcModules[loop] != '\0'; loop++)
	{
		if (pcModules[loop] == ';'){
			len = loop - start;
			strncpy(acModule, pcModules+start, len);
			acModule[len] = '\0';

			sprintf(acCmd, "insmod %s%s", MODULE_PATH, acModule);
			LOG(LOG_INFO, "[lte_startup] exe cmd: %s", acCmd);
			if (-1 == system_call(acCmd)){
				LOG(LOG_ERR, "[lte_startup] insmod \"%s\" fail", acModule);
			}
			start = loop+1;
		}
	}
	return 0;
}

LTE_PLATFORM_E find_device_platform(struct libusb_device_descriptor *pstDesc)
{
	int loop = 0;
	
	for (loop = 0; loop < M_TOTAL_NUM; loop++)
	{
		if(pstDesc->idVendor == g_acDeviceInfo[loop].stDevDesc.usVendor && 
		   pstDesc->idProduct == g_acDeviceInfo[loop].stDevDesc.usProduct){

		   	return g_acDeviceInfo[loop].iDeviceNum;
		}
	}
	return M_TOTAL_NUM;
}

int get_device_platform(void)
{
	
	libusb_device **devs, *dev;
	struct libusb_device_descriptor desc;
	int ret, moduleNum, loop;
	ssize_t cnt;

	ret = libusb_init(NULL);
	if (ret < 0)
		return M_TOTAL_NUM;

	cnt = libusb_get_device_list(NULL, &devs);
	if (cnt < 0){
		libusb_exit(NULL);
		return M_TOTAL_NUM;
	}

	loop = 0;
	while ((dev = devs[loop++]) != NULL) {
		
		ret = libusb_get_device_descriptor(dev, &desc);
		if (ret < 0) {
			LOG(LOG_ALERT, "failed to get device descriptor");
			return M_TOTAL_NUM;
		}
		moduleNum = find_device_platform(&desc);
		if (moduleNum < M_TOTAL_NUM)
			break;
	}
	
	
	libusb_free_device_list(devs, 1);
	libusb_exit(NULL);
	return moduleNum;
}
int get_parameter(int argc,char** args)
{
    int iIndex = 0 ;
    int iChar ;
    //while((iChar = getopt_long(argc, args, "a:p:s:d:i:c:r:", long_options, &iIndex)) != EOF)
    while((iChar = getopt_long(argc, args, "s:a:c:m:h:", long_options, &iIndex)) != EOF)
    {

    	switch (iChar)
    	{
    	    case 's':
    		{
                g_iIPStack = atoi(optarg);
    			break;
    		}
    	    case 'a':
    		{
    			strcpy(g_acApn, optarg);
                g_iFlag = 1;
    			break;
    		}
    		case 'c':
    		{
    			strcpy(acResetCmd,optarg);
    			break;
    		}
            case 'm':
    		{
    			strcpy(g_acMacAddr,optarg);
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
    
    if (0 == strlen(acResetCmd))
    {
		LOG(LOG_ERR, "unset reboot cmd");
        return -1;
    }
    return 0;
}
int lte_at_test(void)
{
    char acSysCmd[32] = "ltectl poweron";
    FILE *fp = NULL;
    char  acGetData[128] = {0};
    char *pTmp = NULL;
    
    fp = popen(acSysCmd, "r");
	if ( fp == NULL )
    {
		LOG(LOG_ERR, "popen [%s] fail", acSysCmd);
		return -1;
	}
	pTmp = fgets(acGetData, sizeof(acGetData), fp);
	pclose(fp);
    
    if ( pTmp != NULL )
    {
        /*未找到则说明cmd 执行成功*/
        if ( strstr(acGetData, "fail") == NULL )
        {
            return 0;
        }
    }
    system_call(acResetCmd);
    /*其余均是无效*/
    return -1;
}
int lte_com_test(void)
{
    int iCount = 2;
    int iTimeout = 30;
    int iModule = M_TOTAL_NUM;
    while(iCount--)
    {
        if(0 == lte_at_test())
        {
            LOG(LOG_ERR, "lte_at_test ok!");
            break;
        }
        
        while(iTimeout--)
        {
            iModule = get_device_platform();
            if (iModule != M_TOTAL_NUM)
            {
                LOG(LOG_INFO, "[lte_startup] LTE device: vendor=%x, product=%x",g_acDeviceInfo[iModule].stDevDesc.usVendor,g_acDeviceInfo[iModule].stDevDesc.usProduct);
                break;
            }
            sleep(1);
        }
        if (M_TOTAL_NUM == iModule)
        {
            LOG(LOG_ERR, "failed to get device platform");
            return -1;
        }    
    }
    
    return 0;
}

static int file_is_exists(char *pcFile)
{
    if(NULL == pcFile)
    {
        LOG(LOG_ERR, "param is null pointer!");
        return -1;
    }
   
    if(0 != access(pcFile, F_OK))
    {
        LOG(LOG_ERR, "file [%s] is not exist!", pcFile);
        return -1;
    }
    else
    {
        return 0;
    }
}

int lte_clean_mac_colon(char *before, char *after)
{
    int i;	
    for(i=0; i<strlen(before); i++)
    {       
        if( before[i] != ':')           
            *after++ = before[i];
    }
    return 0;
}
#if 1
int main(int argc,char** args)
{
	int iModule = M_TOTAL_NUM;
	int iTimeout = 30;
	int iTimeoutSec = 20;
    int iRet = 0;
    int iCount = 4;
    int iSetCount = 4;
    char acCmd[128] = {0};
    char acLteComCmd[128] = {0};
    LTE_MOUDLE_MAC_T stMac = {0};
    LTE_MOUDLE_MAC_T stMacSet = {0};
    
    char acSetMacCmd[32] = {0};
    char acIfCmd[64] = {0};
    char acMac6[32] = {0};

    iRet = get_parameter(argc,args);
 
	/*获取出来的信息和全局的结构体比较，确认平台*/
    /*每1s扫描一次，直到发现设备，30s没有扫描到，则不加载模块*/
    while (iTimeout--)
    {
    	iModule = get_device_platform();
       	if (iModule != M_TOTAL_NUM)
       	{
    		LOG(LOG_INFO, "[lte_startup] LTE device: vendor=%x, product=%x",g_acDeviceInfo[iModule].stDevDesc.usVendor,g_acDeviceInfo[iModule].stDevDesc.usProduct);
       		break;
       	}
		sleep(1);
    }

    if (M_TOTAL_NUM == iModule)
    {
		LOG(LOG_ERR, "failed to get device platform");
       	return -1;
    }    
    	
	load_modules(g_acDeviceInfo[iModule].acModules);

	if (iModule == M_ALTAIR){
		sleep(2);
		system_call("ifconfig usb0 "ALTAIR_CLIENT_IP" up");
	}
    else if((M_YUGA_CDMA_CEM600 == iModule) || (M_THINKWILL_CDMA_MI660 == iModule))
    {
        lte_com_test();
    }
    else if(M_HUAWEI4G_STORAGE == iModule)
    {
        system("usb_modeswitch -J -v 12d1 -p 14fe");
        iModule = M_HUAWEI4G_NETCARD;
        //sleep(5);
    }

    /* 判断是否为3G模块，将ppp相关的配置文件复制到/tmp/ppp目录下 */
    if((M_YUGA_CDMA_CEM600 == iModule) || (M_THINKWILL_CDMA_MI660 == iModule)||(M_ZTE_CDMA2000 == iModule)||
        (M_ZTE == iModule) || (M_THINKWILL == iModule) || M_HUAWEI == iModule)
    {
        system_call("cp -fr /etc/ppp-lte/* /tmp/ppp");
    }
    if(M_LEADCORE == iModule)
    {
        sleep(5);
    }
    
     if(M_MARVELL == iModule)
    {
        sleep(10);
    }
     
    sleep(2);
    if((M_ZTE_MICRO == iModule) || (M_ARCCRA == iModule) || (M_ARCCRA_SECOND == iModule) || (M_LONGCHEER == iModule) || 
        (M_LEADCORE == iModule) || (M_YUGA == iModule) || (M_HUAWEI == iModule) || (M_MARVELL == iModule) ||
        (M_HUAWEI4G_NETCARD == iModule))
    {
        //sleep(4);
        snprintf(acLteComCmd, sizeof(acLteComCmd), "ltecomsev -m %d -l 5&", iModule);
        system_call(acLteComCmd);
        //sleep(5);
    }
    #if 0
    /* 中兴模块需启动模块协议栈 */
    if(1 == g_iFlag)
    {
        sprintf(acCmd, "ltectl setpdpstack %d %s", g_iIPStack, g_acApn);
    }
    else
    {
        sprintf(acCmd, "ltectl setpdpstack %d", g_iIPStack);
    }
    while(system_call(acCmd) < 0)
    {
        iCount++;
        if(iCount > 5)
        {
            break;
        }
        sleep(2);
    }
    #endif
    if(M_LEADCORE == iModule)
    {
        sleep(3);
        if (1 != lte_moudle_mac_get(&stMac) )
    	{
            LOG(LOG_ERR, "get mac fail");
            return -1;
    	}
        lte_clean_mac_colon(g_acMacAddr, acMac6);
        LOG(LOG_ERR, "-------usb0Mac:%s,moudleMac:%s", acMac6, stMac.acMac);
        if( strcmp(stMac.acMac, acMac6) != 0 )
        {
            strncpy(stMacSet.acMac, acMac6, sizeof(stMacSet.acMac));
            while(iSetCount--)
            {
                if (1 == lte_moudle_mac_set(&stMacSet))
            	{
                    LOG(LOG_ERR, "set mac success");
                    break;
            	}               
            }
            sleep(2);
            system_call("killall ltecomsev");
            sleep(1);
            system_call("ltecmd reset");
            sleep(10);
            while (iTimeoutSec--)
            {
            	iModule = get_device_platform();
               	if (iModule != M_TOTAL_NUM)
               	{
            		LOG(LOG_INFO, "[lte_startup] LTE device: vendor=%x, product=%x",g_acDeviceInfo[iModule].stDevDesc.usVendor,g_acDeviceInfo[iModule].stDevDesc.usProduct);
               		break;
               	}
        		sleep(1);
            }

            sleep(12);
            snprintf(acLteComCmd, sizeof(acLteComCmd), "ltecomsev -m %d -l 5&", iModule);
            system_call(acLteComCmd); 
            
        }
        /* BEGIN: Added by zhangjiasi, 2015/4/20   PN:平台驱动导致的堆栈打印，和设备重启问题，规避方法 */
        system_call("insmod /lib/modules/lc_ltencm.ko");
        /* END:   Added by zhangjiasi, 2015/4/20   PN:平台驱动导致的堆栈打印，和设备重启问题，规避方法 */
    }
    
    sleep(3);
    if(M_MARVELL == iModule)
    {
        system_call("ltectl poweron");
    }
    if(M_HUAWEI4G_NETCARD == iModule)
    {
        //system_call("ltectl poweroff");
        //sleep(2);
        system_call("ltectl poweron");
    }
    /* 中兴模块需启动模块协议栈 */
    if(M_ZTE_MICRO == iModule)
    {
        /* 用于获取中兴微电子模块的iccid */
        while(iCount--)
        {
            sleep(3);
            system_call("ltectl poweroff");
            sleep(4);
            system_call("ltectl poweron");
            if(0 == file_is_exists(ICCID_FILE_PATH))
            {
                sleep(iCount*4);
                break;
            }
        }
    }
    else
    { 
        //sleep(10);
    }
 
    if(M_ALTAIR == iModule)
    {
        /* \\\"解决传到altairat里面"消失的问题 */
        system_call("altairat at%tstlte=\\\"usim\\\",\\\"erase_ef\\\",\\\"0\\\"");
    }
    if((M_ARCCRA == iModule) || (M_ARCCRA_SECOND == iModule) || (M_ZTE_MICRO == iModule) || (M_LEADCORE == iModule))
    {
        system_call("ltectl smsinit");
    }
    if(M_ZTE_MICRO == iModule)
    {
        system_call("lteclient &");
    }
    /*BEGIN:  Added by wangqi,2015/6/24,  PN: ipv6配置文件 */
    system_call("echo 0 > /proc/sys/net/ipv6/conf/usb0/accept_ra"); 
    system_call("echo 0 > /proc/sys/net/ipv6/conf/usb0/autoconf"); 
    system_call("echo 0 > /proc/sys/net/ipv6/conf/usb0/accept_ra_pinfo");
    /* END:   Added by wangqi,2015/6/24,  PN: ipv6配置文件 */
	return 0;
}
#else
int main(int argc,char** args)
{
	int iModule = M_TOTAL_NUM;
    char acLteComCmd[128] = {0};
    int iTimeout = 30;
  	/*while (iTimeout--)
    {
    	iModule = get_device_platform();
       	if (iModule != M_TOTAL_NUM)
       	{
    		LOG(LOG_INFO, "[lte_startup] LTE device: vendor=%x, product=%x",g_acDeviceInfo[iModule].stDevDesc.usVendor,g_acDeviceInfo[iModule].stDevDesc.usProduct);
       		break;
       	}
		sleep(1);
    }

    if (M_TOTAL_NUM == iModule)
    {
		LOG(LOG_ERR, "failed to get device platform");
       	return -1;
    } */
	//load_modules(g_acDeviceInfo[M_HUAWEI4G_STORAGE].acModules);

    if(LTE_OK == system_call("usb_modeswitch -J -v 12d1 -p 14fe"))
    {  
        sleep(2);
    }
    sleep(1);
    snprintf(acLteComCmd, sizeof(acLteComCmd), "ltecomsev -m %d -l 5&", M_HUAWEI4G_NETCARD);
    system_call(acLteComCmd);

    sleep(1);

    //system_call("ltectl poweroff");
    //sleep(2);
    //system_call("ltectl poweron");

	return 0;
}


#endif

