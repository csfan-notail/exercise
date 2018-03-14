/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : update_flashimage.c
  版 本 号   : 初稿
  作    者   : 张平
  生成日期   : 2013年2月27日 星期三
  最近修改   :
  功能描述   : boot下自动下载flash image功能
  函数列表   :
  修改历史   :
  1.日    期   : 2013年2月27日 星期三
    作    者   : 张平
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "custom_internal.h"
#include <net.h>
#include <watchdog.h>

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
#define NVRAM_UPDATE_FLASH      "update_flash"
#define NVRAM_AP_MAC            "ap_mac"
#define NVRAM_UPDATE_RESULT     "upflash_result"

#define TEMP_MEM_LOAD_ADDR   0x80060000
#define FLASH_FILE_NAME     "flash-image.bin"

//#define FLASH_SIZE_8         0x800000  
//#define FLASH_SIZE_16        0x1000000
#define FLASH_BOOT_SIZE      UBOOT_FLASH_SIZE   //0x40000
#define FLASH_ART_SIZE       ART_SECTOR_SIZE    //0x10000   /* 2013-9-23 moded by zhangping 原先为了支持中兴倒数第二个分区为art区，目前中兴art分区已经调整到最后一个分区 */
#define FLASH_BASE_ADDR      CFG_FLASH_BASE     //0x9f000000    

#define SERVER_IP           "1.0.0.1"

#define MAC_STRING_LEN     30
#define IP_STRING_LEN      30
#define STRING_LEN         100
#define IP_LEN         4

#define PING_COUNT_MAX     3
#define TFTP_COUNT_MAX     5
#define CHECK_COUNT_MAX    3

#define DOWNLOAD_ERROR     -1
#define CHECK_ERROR        -2
#define CHECK_SUCESS       0

#define TRUE    1
#define FALSE   0
#ifndef BOOL
#define BOOL    int
#endif

#define LED_WAN_GPIO        13
#define LED_LAN_GPIO        15
#define LED_WLAN_GPIO       14 

#define LED_OFF   led_off
#define LED_ON    led_on

#define BOOTFILE    "boot"
#define KERNELFILE  "kernel"
#define FSFILE      "fs"
#define FLASHFILE   "flash" /* TASK #513 add by zhangping 2013-3-5 */

#define BOARDTYPE   CONFIG_CUSTOM_BOARD
#define BOARDTYPE_MAX_LEN 64
#define FILETYPE_MAX_LEN  16

typedef struct
{
    unsigned long ulCrc;    /*CRC校验值*/
    char acBoardType[BOARDTYPE_MAX_LEN];   /*平台类型*/
    char acFileType[FILETYPE_MAX_LEN];    /*文件类型*/
}BIN_INFO_T;

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/
char acLogStr[STRING_LEN];

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/


#define LOG_SAVE(fmt, args...)                  \
do{											    \
	printf(fmt,##args);                         \
    sprintf(acLogStr,fmt,##args);               \
    setnvram(NVRAM_UPDATE_RESULT, acLogStr);    \
    save_nvram_auto();                          \
}while(0)

/*****************************************************************************
 函 数 名  : gbcom_led_set
 功能描述  : boot下点灯接口
 输入参数  : unsigned int uiLedGpio  
             unsigned int uiOnOff    
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月5日 星期二
    作    者   : 张平
    修改内容   : 新生成函数

*****************************************************************************/
static inline void
gbcom_led_set(unsigned int uiLedGpio,unsigned int uiOnOff)
{

    switch (uiLedGpio) {
        case LED_WAN_GPIO:
            custom_led_set_wan(uiOnOff);
            break;
        case LED_LAN_GPIO:
            custom_led_set_lan(uiOnOff);
            break;
        case LED_WLAN_GPIO:
            custom_led_set_wlan(uiOnOff);
            break;         
        default:
            break;
    }

    return;
}

/*****************************************************************************
 函 数 名  : gbcom_copy_to_flash
 功能描述  : 从指定内存拷贝数据到指定flash地址
 输入参数  : unsigned long ulMemAddr    
             unsigned long ulStartAddr  
             unsigned long ulEraseSize  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月5日 星期二
    作    者   : 张平
    修改内容   : 新生成函数

*****************************************************************************/
int gbcom_copy_to_flash(unsigned long ulMemAddr,unsigned long ulStartAddr,unsigned long ulEraseSize)
{
    unsigned long ulEndAddr;
    int iRet = 0;
    char *pcMemAddr;

	ulEndAddr = ulStartAddr + ulEraseSize - 1;

	if (flash_sect_protect (0, ulStartAddr, ulEndAddr))
    {
        return -1;
    }

#ifdef CONFIG_ATH_NAND_FL
	if (flash_erase(NULL, ulStartAddr, ulEndAddr))
    {
        return -1;
    }
#else
	if (flash_sect_erase (ulStartAddr, ulEndAddr))
    {
        return -1;
    }
#endif

    pcMemAddr = (char *)ulMemAddr;
    iRet = flash_write((char *)pcMemAddr, ulStartAddr, ulEraseSize);
	if (iRet != 0) {
		flash_perror (iRet);
		iRet = -1;
	} else {
		printf("done\n");
	}

    (void) flash_sect_protect (1, ulStartAddr, ulEndAddr);
	return iRet;
}

/*****************************************************************************
 函 数 名  : get_bin_info_in_mem_end
 功能描述  : 获取下载到内存中的bin文件末尾信息
 输入参数  : unsigned long ulMemAddr  
             BIN_INFO_T *pstBinInfo   
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月27日 星期四
    作    者   : 张平
    修改内容   : 新生成函数

*****************************************************************************/
void get_bin_info_in_mem_end(unsigned long ulMemAddr,BIN_INFO_T *pstBinInfo)
{
    unsigned int uiCount=0;
    unsigned long ulMemAddrTem,ulMemData;

    /* crc */
    pstBinInfo->ulCrc = 0;
    for(uiCount=0;uiCount<4;uiCount++)
    {
        ulMemData = *(volatile unsigned char*)ulMemAddr;
        pstBinInfo->ulCrc |= (ulMemData << (8*(3-uiCount)));
        ulMemAddr++;
    }
    ulMemAddrTem = ulMemAddr;
    uiCount = 0;

    /* 平台类型 */
    do
    {
        pstBinInfo->acBoardType[uiCount] = *(volatile unsigned char*)ulMemAddr;
        uiCount++;
        ulMemAddr += sizeof(char);
    }while((uiCount<BOARDTYPE_MAX_LEN) && (0 != pstBinInfo->acBoardType[uiCount-1]));
    
    /* 文件类型 */
    ulMemAddr = ulMemAddrTem + BOARDTYPE_MAX_LEN;
    uiCount =0;
    do
    {
        pstBinInfo->acFileType[uiCount] = *(volatile unsigned char*)ulMemAddr;
        uiCount++;
        ulMemAddr += sizeof(char);
    }while((uiCount<FILETYPE_MAX_LEN) && (0 != pstBinInfo->acFileType[uiCount-1]));
    return ;
}


/*****************************************************************************
 函 数 名  : check_bin_info
 功能描述  : 检测bin文件在内存中的校验信息
 输入参数  : unsigned char *pucStartAddr  
             int iSize                    
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月30日 星期六
    作    者   : 张平
    修改内容   : 新生成函数

*****************************************************************************/
int check_bin_info(unsigned long ulStartAddr,int iSize)
{
    BIN_INFO_T stBinInfo,stBinInfoCalc;
    unsigned char *pcVarAddr = NULL;

    memset(&stBinInfo, 0, sizeof(BIN_INFO_T));
    memset(&stBinInfoCalc, 0, sizeof(BIN_INFO_T));

    pcVarAddr = (unsigned char *)ulStartAddr;
    ulStartAddr = ulStartAddr + iSize - sizeof(BIN_INFO_T);
    get_bin_info_in_mem_end(ulStartAddr,&stBinInfo);

    stBinInfoCalc.ulCrc = crc32(0,pcVarAddr,iSize-sizeof(BIN_INFO_T));
    strcpy(stBinInfoCalc.acBoardType,BOARDTYPE);
    strcpy(stBinInfoCalc.acFileType,FLASHFILE);
    
    printf("check_bin_info: read [ 0x%08x %s %s ], cal [ 0x%08x %s %s ]\n",
        stBinInfo.ulCrc, stBinInfo.acBoardType, stBinInfo.acFileType,
        stBinInfoCalc.ulCrc, stBinInfoCalc.acBoardType, stBinInfoCalc.acFileType);
    if(0 == memcmp(&stBinInfo,&stBinInfoCalc,sizeof(BIN_INFO_T)))
    {
        return CHECK_SUCESS;
    }
    else
    {
        return CHECK_ERROR;
    }
}

/*****************************************************************************
 函 数 名  : download_check
 功能描述  : 通过tftp下载flash镜像并校验flash镜像是否正确
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月5日 星期二
    作    者   : 张平
    修改内容   : 新生成函数

*****************************************************************************/
int download_check(void)
{
    int   iCount;
    int   iSize;

    load_addr = TEMP_MEM_LOAD_ADDR;
    copy_filename (BootFile, FLASH_FILE_NAME, sizeof(BootFile)); 
    setenv ("netretry", "no");/* 设置tftp下载只尝试一次，默认是无限循环 */
    for(iCount=0;iCount<TFTP_COUNT_MAX;iCount++)
    {
        if ((iSize = NetLoop(TFTP)) <= 0)
        {
            continue;
        }
        else
        {
            break;
        }
    }

    if (iCount >= TFTP_COUNT_MAX)
    {
        return DOWNLOAD_ERROR;
    }

    /* 校验flash镜像 */
    if(CHECK_SUCESS == check_bin_info(TEMP_MEM_LOAD_ADDR,iSize))
    {
        return iSize;
    }
    return CHECK_ERROR;
}

/*****************************************************************************
 函 数 名  : set_mac_ip
 功能描述  : 设置boot中环境变量中mac地址和ip地址
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月5日 星期二
    作    者   : 张平
    修改内容   : 新生成函数

*****************************************************************************/
BOOL set_mac_ip(void)
{
    char acApMac[MAC_STRING_LEN]={0},acApMacTem[MAC_STRING_LEN]={0};
    char acTemValue[3] = {0};
    unsigned long ulIpVal[3];
    char acIpAddr[IP_STRING_LEN]={0};
    int   iCount,iCountTem;

    /* 从nvram中读取mac并设置环境变量mac和ip,ip后三个字节采用mac后三个字节 */
    if(getnvram_r(NVRAM_AP_MAC,acApMacTem,MAC_STRING_LEN) > 0)
    {
        /* 判断MAC地址格式是否正确 */
        for(iCount=2;iCount<15;iCount+=3)
        {
            if(':' != acApMacTem[iCount])
            {
                return FALSE;
            }
        }
        
        for(iCount=0,iCountTem=0;iCount<MAC_STRING_LEN-3;iCount++,iCountTem++)
        {
            acApMac[iCount] = '0';
            acApMac[++iCount] = 'x';
            if( ':' == acApMacTem[iCountTem])
            {
                iCountTem++;
            }
            acApMac[++iCount] = acApMacTem[iCountTem];
            if(!((acApMacTem[iCountTem] >= '0' && acApMacTem[iCountTem] <= '9') || \
                (acApMacTem[iCountTem] >= 'a' && acApMacTem[iCountTem] <= 'f') || \
                (acApMacTem[iCountTem] >= 'A' && acApMacTem[iCountTem] <= 'F')))
            {
                return FALSE;
            }
            acApMac[++iCount] = acApMacTem[++iCountTem];
            if(!((acApMacTem[iCountTem] >= '0' && acApMacTem[iCountTem] <= '9') || \
                (acApMacTem[iCountTem] >= 'a' && acApMacTem[iCountTem] <= 'f') || \
                (acApMacTem[iCountTem] >= 'A' && acApMacTem[iCountTem] <= 'F')))
            {
                return FALSE;
            }
            if((MAC_STRING_LEN-1) != iCount)
            {
                acApMac[++iCount] = ':';
            }
        }
        acApMac[MAC_STRING_LEN-1] = '\0';
        setenv ("ethaddr", acApMac);
        printf ("mac is %s\n",acApMac);

        for(iCount=0,iCountTem=9;iCount<3;iCount++,iCountTem++)
        {
            if( ':' == acApMacTem[iCountTem])
            {
                iCountTem++;
            }
            acTemValue[0] = acApMacTem[iCountTem];
            acTemValue[1] = acApMacTem[++iCountTem];
            ulIpVal[iCount] = simple_strtoul (acTemValue, NULL, 16);  
        }
        sprintf(acIpAddr,"1.%u.%u.%u",ulIpVal[0],ulIpVal[1],ulIpVal[2]);
        printf ("ip is %s\n",acIpAddr);
        setenv ("ipaddr", acIpAddr);
        setenv ("serverip", SERVER_IP);
    }
    else
    {
        return FALSE;
    }
    
    return TRUE;
}

/*****************************************************************************
 函 数 名  : update_flashimage
 功能描述  : 更新flash镜像接口
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月5日 星期二
    作    者   : 张平
    修改内容   : 新生成函数

*****************************************************************************/
int update_flashimage(void)
{
    char *pcVarAddr = NULL;
    char cVarValue = '0';
    int   iSize;
    int   iCount;
    int   iRet = 0;
    unsigned long ulRamCrc = 0,ulFlashCrc = 0;/* bug#2322 2013-4-18 add by zhangping */

    pcVarAddr = getnvram(NVRAM_UPDATE_FLASH);
    if(NULL != pcVarAddr)
    {
        cVarValue = *pcVarAddr;
    }
    if(NULL == pcVarAddr || '1' != cVarValue)
    {
        return -1;
    }

    /* 从nvram中读取mac并设置环境变量mac和ip,ip后三个字节采用mac后三个字节 */
    if(!set_mac_ip())
    {
        LOG_SAVE("set mac and ip error.\n");
		return -1;
    }
    gbcom_led_set(LED_WLAN_GPIO,LED_ON);
    gbcom_led_set(LED_LAN_GPIO,LED_OFF);
    gbcom_led_set(LED_WAN_GPIO,LED_OFF);
    
    /* 测试serverip是否可以通信 */
    NetPingIP = getenv_IPaddr ("serverip");
	if (NetPingIP == 0) 
    {
        LOG_SAVE("serverip is 0\n");
		return -1;
	}
    for(iCount=0;iCount<PING_COUNT_MAX;iCount++)
    {
        if (NetLoop(PING) < 0)
        {
            continue;
        }
        else
        {
            break;
        }
    }
    if (iCount >= PING_COUNT_MAX) 
    {
        LOG_SAVE("serverip:0x%x is not alive\n",NetPingIP);
		return -1;
	}

    /* tftp下载和校验内存中flash镜像 */
    for(iCount=0;iCount<CHECK_COUNT_MAX;iCount++)
    {
        iSize = download_check();
        if(DOWNLOAD_ERROR == iSize)
        {
            setenv("netretry",NULL);/* 改为默认设置 */
            LOG_SAVE("TFTP download fail\n");
            return -1;
        }
        else if(CHECK_ERROR == iSize)
        {
            continue;
        }
        else
        {
            break;
        }
    }
    if(iCount >= CHECK_COUNT_MAX)
    {
        LOG_SAVE("File information does not match!\n");
        return -1;
    }
    gbcom_led_set(LED_WLAN_GPIO,LED_ON);
    gbcom_led_set(LED_LAN_GPIO,LED_ON);
    gbcom_led_set(LED_WAN_GPIO,LED_OFF);

    for(iCount=0;iCount<CHECK_COUNT_MAX;iCount++)
    {
        /* 更新非boot区 */
        if((iSize-sizeof(BIN_INFO_T))> CFG_FLASH_SIZE)
        {
            LOG_SAVE("flash-image size is greater than 0x%x\n",CFG_FLASH_SIZE);
            return -1;
        }
        iRet = gbcom_copy_to_flash(TEMP_MEM_LOAD_ADDR+FLASH_BOOT_SIZE,FLASH_BASE_ADDR+FLASH_BOOT_SIZE,CFG_FLASH_SIZE-FLASH_BOOT_SIZE-FLASH_ART_SIZE);
        if(0 != iRet)
        {
            LOG_SAVE("copy to flash error,iRet=%d\n",iRet);
            return -1;
        }

        /* 更新boot区 */
        iRet = gbcom_copy_to_flash(TEMP_MEM_LOAD_ADDR,FLASH_BASE_ADDR,FLASH_BOOT_SIZE);
        if(0 != iRet)
        {
            LOG_SAVE("copy boot to flash error,iRet=%d\n",iRet);
            return -1;
        }    

        /* 校验flash中flash镜像 */
        /* begin:bug#2322 2013-4-18 add by zhangping */
        pcVarAddr = (char *)TEMP_MEM_LOAD_ADDR;
        ulRamCrc = crc32(0,(void *)pcVarAddr,iSize - sizeof(BIN_INFO_T) - FLASH_ART_SIZE);
        pcVarAddr = (char *)FLASH_BASE_ADDR;
        ulFlashCrc = crc32(0,(void *)pcVarAddr,iSize - sizeof(BIN_INFO_T) - FLASH_ART_SIZE);
        if(ulRamCrc != ulFlashCrc)
        {
            continue;
        }
        else
        {
            break;
        }
        /* end:bug#2322 2013-4-18 add by zhangping */
    }
    
    if(CHECK_COUNT_MAX == iCount)
    {
        LOG_SAVE("check flash image error,iRet=%d,trycount:%d\n",iRet,iCount);
        return -1;
    }

    /* 升级成功后一定不能使用LOG_SAVE,否则会破坏flash镜像 */
    printf("update flash success\n");

    while(1)
    {
        WATCHDOG_RESET();
        gbcom_led_set(LED_WLAN_GPIO,LED_ON);
        gbcom_led_set(LED_LAN_GPIO,LED_ON);
        gbcom_led_set(LED_WAN_GPIO,LED_ON);
        udelay(500000);
        gbcom_led_set(LED_WLAN_GPIO,LED_OFF);
        gbcom_led_set(LED_LAN_GPIO,LED_OFF);
        gbcom_led_set(LED_WAN_GPIO,LED_OFF);
        udelay(500000);
        if(tstc())
        {
            if('q' == getc())
            {
                printf("\ninterrupt:key-'q'.\n");
                return 0;
            }
        }
    }
    	
    return 0;
}

/*****************************************************************************
 函 数 名  : do_upflash
 功能描述  : 命令行更新flash镜像接口
 输入参数  : cmd_tbl_t *cmdtp  
             int flag          
             int argc          
             char *argv[]      
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月5日 星期二
    作    者   : 张平
    修改内容   : 新生成函数

*****************************************************************************/
int do_upflash(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    if(0 == update_flashimage())
    {
        printf("update success\n");
    }
    else
    {
        printf("update faild\n");
    }
    return 0;
}


U_BOOT_CMD(
    upflash,2, 1,	do_upflash,
	"upflash - update flashimage by tftp\n",
	"upflash\n"
    ""
);
