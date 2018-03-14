
#include "custom_internal.h"


typedef struct
{
    char acVersion[64];
    char acHardware[16];    /*硬件平台*/
    char acPcbCode[16];     /*PCB代码*/
    char acPcbVer[16];      /*PCB版本*/
    char acBomCode[64];     /*BOM配置代码*/
}BOOTINFO_BIN_T;

#define BOOT_INFO_SEC_SIZE 0x400        /*1k Bytes*/
#define BOOT_INFO_ADDR (CFG_FLASH_BASE + UBOOT_FLASH_SIZE - BOOT_INFO_SEC_SIZE)  /*last 1k Bytes of U-boot*/
BOOTINFO_BIN_T *g_bootinfo = (BOOTINFO_BIN_T *)BOOT_INFO_ADDR;


int custom_display_banner(void)
{
    if (strlen(g_bootinfo->acVersion) < sizeof(g_bootinfo->acVersion)) {
        printf("\n\n%s (" __DATE__ " - " __TIME__ ")\n\n", g_bootinfo->acVersion);
        return 1;
    }

    return 0;
}

void boot_info_to_nvram(BOOTINFO_BIN_T *pstBinInfo)
{
	char *pcBootinfo = NULL;
	pcBootinfo = getnvram("BOOT_VERSION");
	if  ((NULL == pcBootinfo) || strcmp(pcBootinfo, pstBinInfo->acVersion))
	{
	    setnvram("BOOT_VERSION", pstBinInfo->acVersion);
	}    
	pcBootinfo = getnvram("BOOT_HWP");
	if  ((NULL == pcBootinfo) || strcmp(pcBootinfo, pstBinInfo->acHardware))
	{
	    //setnvram("BOOT_HWP", pstBinInfo->acHardware);
	    setnvram("BOOT_HWP", "TA321");
	}
	pcBootinfo = getnvram("BOOT_PCB");
	if  ((NULL == pcBootinfo) || strcmp(pcBootinfo, pstBinInfo->acPcbCode))
	{
	    //setnvram("BOOT_PCB", pstBinInfo->acPcbCode);
	    setnvram("BOOT_PCB", "TA321-70");
	}
	pcBootinfo = getnvram("BOOT_PCBVER");
	if  ((NULL == pcBootinfo) || strcmp(pcBootinfo, pstBinInfo->acPcbVer))
	{
	    setnvram("BOOT_PCBVER", pstBinInfo->acPcbVer);
	}
	pcBootinfo = getnvram("BOOT_BOMCODE");
	if  ((NULL == pcBootinfo) || strcmp(pcBootinfo, pstBinInfo->acBomCode))
	{
	    setnvram("BOOT_BOMCODE", pstBinInfo->acBomCode);
	}
    save_nvram_auto();
}

void get_boot_info(void)
{
    printf("************Hardware:%s PcbCode:%s PcbVer:%s BomCode:%s ************\n",
        g_bootinfo->acHardware, g_bootinfo->acPcbCode, g_bootinfo->acPcbVer, g_bootinfo->acBomCode);
    boot_info_to_nvram(g_bootinfo);
    return;
}



