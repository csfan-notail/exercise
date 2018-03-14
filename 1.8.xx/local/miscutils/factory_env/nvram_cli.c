/******************************************************************************
* COPYRIGHT GBCOM CO.,LTD   
*******************************************************************************
* 功能: 配置和获取NVRAM的配置信息 命令行接口                                                                                                               
* 编制日期: 03/16/2012                                                                                                 
* 修改历史: 徐从江 新建      
* 
******************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "nvram_flash.h"

#define NVRAM_HARD_VERSION  "hard_version"
#define NVRAM_AP_WORK_MODE  "ap_work_mode"
#define NVRAM_PROTECT_NUM 10
char nvram_protect_name[NVRAM_PROTECT_NUM][NVRAM_NAME_MAX_LEN] = {
    NVRAM_HARD_VERSION,
    NVRAM_AP_WORK_MODE,
    "BOOT_HWP",
    "BOOT_PCB",
    "BOOT_PCBVER",
    "BOOT_BOMCODE"
};

#define CMD_FACTORY_NVRAM "factory_env"

#define MTD_NVRAM "6"
#define MTD_FACTORY_NVRAM "9"

char nvram_staging[128] = "/tmp/.nvram_0101011000";
char nvram_flash_mtd[128] = "/dev/mtdblock"; 

/* find mtdpart,set staging file name */
int mtd_select(char *cmdname)
{
	char *p;
    char mtdname[32];
    
	if ((p = strrchr (cmdname, '/')) != NULL) {
        cmdname = p + 1;    
    }

    if (strcmp(cmdname, CMD_FACTORY_NVRAM) == 0) {
        strcat(nvram_staging, MTD_FACTORY_NVRAM);
        strcat(nvram_flash_mtd, MTD_FACTORY_NVRAM);
    }
    else {
        strcat(nvram_staging, MTD_NVRAM);
        strcat(nvram_flash_mtd, MTD_NVRAM);
    }

    return 0;
}

/* nvramcli set name=value */
int set(char *name_value,unsigned char ucFlag)
{
    char name[NVRAM_NAME_MAX_LEN + 1] = {};
    char value[NVRAM_VALUE_MAX_LEN + 1] = {};
    int valLen = 0;
    int nameLen = 0;
    int flag = 0;
    char *pNmae = name;
    char *pBuf = name_value;
    char *pValue = value;

    while ((*pNmae++ = *pBuf++) && nameLen++ < sizeof(name)-1)
    {
        if ('=' == *pBuf)
        {
            flag = 1;
            break;
        }
    }
    NVRAM_DEBUGF("name(len=%d):%s\n", nameLen, name);
    
    if (nameLen == sizeof(name))
    {
        printf("Name lenth too long!\n");
        return  RET_FAIL;
    }
    if (1 != flag)
    {
        printf("%s \nFormat error, must be \"name=value\"\n", name_value);
        return RET_FAIL;
    }
    
    if (!ucFlag)
    {
        int loop = 0;
        for(;loop < NVRAM_PROTECT_NUM;loop++)
        {
            if (nvram_protect_name[loop][0] != '\0' && !strcmp(name,nvram_protect_name[loop]))
            {
                printf("can not set %s !please try superset !\n",nvram_protect_name[loop]);
                return  RET_FAIL;
            }
        }
    }
   
    
    pBuf++;  /* skip '=' */
    while ((*pValue++ = *pBuf++) && valLen++ < sizeof(value)-1)
    {
    }

    NVRAM_DEBUGF("value(len=%d):%s\n", valLen, value);
    if (valLen == sizeof(value))
    {
        printf("Value lenth too long!\n");
        return RET_FAIL;
    }
    
    return setnvram(name, value);
}

/* nvramcli unset name */
int unset(char *name)
{
    int loop = 0;
    if (NULL != name)
    {
        for(;loop < NVRAM_PROTECT_NUM;loop++)
        {
            if (nvram_protect_name[loop][0] != '\0' && !strcmp(name,nvram_protect_name[loop]))
            {
                printf("can not unset %s !please try superset !\n",nvram_protect_name[loop]);
                return  RET_FAIL;
            }
        }
       
        return unsetnvram(name);
    }

    return RET_FAIL;
}

/* nvramcli get name */
int get(char *argv[])
{
    char *pValue = NULL;
    if (NULL != argv[2])
    {
        pValue = (char *)getnvram(argv[2]);
        if (NULL != pValue)
        {
            printf("%s=%s\n",argv[2], pValue);
            return RET_OK;
        }
    }

    return RET_FAIL;
}

void help_info(void)
{
    printf("Usage:\n"
        "  nvramcli show\n\t--show all variables\n"
        "  nvramcli get \"variable\"\n\t--show one variable,like name=value, if variable not found ,show nothing\n"
        "  nvramcli set \"var\"=\"value\" [\"var2\"=\"val2\"] ...\n\t--set one or more variables value\n"
        "  nvramcli unset \"variable\" [\"var2\"] ...\n\t--unset one or more variables\n"
        "  nvramcli commit\n\t--save the config to flash\n"
        "Note:\n"
        "  (1) all variables & value must be in \"\", like \"var1\"=\"val1\";\n"
        "  (2) Only  English alphabet & digit  & '.' & '_' & '-' are valid in variables\n"
        "  (3) Only after excute commit, all nvram config are saved to flash!\n");
}

int main(int argc, char *argv[])
{
    int i;
    int iRet = RET_FAIL;
    
    if (argc < 2)
    {
        help_info();
        return RET_FAIL;
    }

    mtd_select(argv[0]);
    
    if (RET_FAIL == nvram_open())
    {
        nvram_close();
        return RET_FAIL;
    }
#if 0
    for (i = 0; i < 5; i++)
    {
        printf("%d\n", i);
        sleep(1);
    }
#endif

    if (nvram_check_crc())
    {
        nvram_close();
        printf("nvram's data is destroyed,reboot system after 3s\n");
        system("reboot 3 nvram destroyed");
    }
   
    if (!strcmp(argv[1], "show"))
    {
        iRet = printnvram();
    }
    else if (!strcmp(argv[1], "get"))
    {
        iRet = get(argv);
    }
    else if (!strcmp(argv[1], "set"))
    {     
        for (i = 2; i < argc; i++)
        {
            iRet = set(argv[i],0);
            if (RET_FAIL == iRet)
            {
                nvram_close();
                return RET_FAIL;
            }
        }
        iRet = nvram_save_to_staging();
    }
    else if (!strcmp(argv[1], "superset"))
    {
        for (i = 2; i < argc; i++)
        {
            iRet = set(argv[i],1);
            if (RET_FAIL == iRet)
            {
                nvram_close();
                return RET_FAIL;
            }
        }
        iRet = nvram_save_to_staging();
    }
    else if (!strcmp(argv[1], "unset"))
    {
        for (i = 2; i < argc; i++)
        {
            iRet = unset(argv[i]);
            if (RET_FAIL == iRet)
            {
                nvram_close();
                return RET_FAIL;
            }
        }
        iRet = nvram_save_to_staging();
    }
    else if (!strcmp(argv[1], "commit"))
    {
        iRet = nvram_commit();
    }
    else if (!strcmp(argv[1], "init"))
    {
        iRet = nvram_init();
    }
    else 
    {
        help_info();
    }
    
    nvram_close();
    return iRet;
}
