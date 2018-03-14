/******************************************************************************
* COPYRIGHT GBCOM CO.,LTD   
*******************************************************************************
* 功能: lte模块升级工具                                                                                                              
* 编制日期: 08/26/2015                                                                                                 
* 修改历史:      
* 
******************************************************************************/
    
#include <stdio.h>
#include <unistd.h>
#include <string.h>
    
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
    

#include "lte.h"
#include "libusb.h"
#include "lte_log.h"
#include "lte_common.h"

#define LTE_VERSION_NAME "lte_version.bin"
#define MAX_SIZE_OF_LTE_VERSION 0x0f00000
#define MIN_SIZE_OF_LTE_VERSION 0x0a00000

//#define VFLAG_DEBUG
#ifdef VFLAG_DEBUG
#define vflag_printf(format, arg...) \
            do{ printf("%s %d : ", __FUNCTION__, __LINE__);\
                printf(format, ##arg);}while(0)
#else
#define vflag_printf(format, arg...) do{}while(0)
#endif

static LTE_RET_E lte_file_is_exists(char *pcFile)
{
    if(NULL == pcFile)
    {
        LTE_LOG(LTE_LOG_ERR, "param is null pointer!");
        return LTE_FAIL;
    }
    
    if(0 != access(pcFile, F_OK))
    {
        LTE_LOG(LTE_LOG_ERR, "file [%S] is not exist!", pcFile);
        return LTE_FAIL;
    }
    else
    {
        return LTE_OK;
    }
}

static LTE_RET_E lte_file_size_get(char *pcFilePath, unsigned int *piFileSize)
{
    struct stat stFileInfo = {0};

    if((NULL == piFileSize) || (NULL == pcFilePath))
    {
        LTE_LOG(LTE_LOG_ERR, "param is null pointer!");
        return LTE_FAIL;
    }

    if(0 != stat(pcFilePath, &stFileInfo))
    {
        LTE_LOG(LTE_LOG_ERR, "get file[%s] stat failed!", pcFilePath);
        return LTE_FAIL;
    }
    else
    {
        *piFileSize = stFileInfo.st_size;
    }
    
    return LTE_OK;
}

static LTE_RET_E check_file_size(const char *pcFile)
{
    unsigned int iFileSize = 0;
    if(LTE_OK != lte_file_size_get(pcFile, &iFileSize))
    {
        return LTE_FAIL;
    }
    if(iFileSize > MAX_SIZE_OF_LTE_VERSION || iFileSize < MIN_SIZE_OF_LTE_VERSION)
    {
        printf("lte version file size error, iFileSize == %d.\n", iFileSize);
        return LTE_FAIL;
    }
    return LTE_OK;
}


static LTE_RET_E check_file_md5(const char *pcFile, char *pcMd5)
{
    FILE *fp = NULL;
    char *pTmp;
    char acStrCmd[128] = {0};
    char acGetData[128] = {0};
    char acMd5sum[128] = {0};

    /*************MD5 校验*************/
    if(pcFile == NULL || NULL == pcMd5)
    {
        return LTE_FAIL;
    }
    snprintf(acStrCmd, sizeof(acStrCmd), "md5sum %s", pcFile);
    fp = popen(acStrCmd, "r");
    if(NULL == fp)
    {
		LTE_LOG(LTE_LOG_ERR, "popen [md5sum] fail!");
		return LTE_FAIL;
    }
    pTmp = fgets(acGetData, sizeof(acGetData), fp);
	pclose(fp);
    if ( (pTmp != NULL) && (strlen(pTmp) != 0))
    {
        sscanf(acGetData,"%s[^ ]" , acMd5sum);
        printf("md5sum == %s\n", acMd5sum);
        if(0 == strncmp(pcMd5, acMd5sum, sizeof(acMd5sum)))
        {
            printf("md5sum OK!\n");
            return LTE_OK;
        }
    }
    return LTE_FAIL;
}



/*
 * 通过tftp下载版本文件，版本文件必须为"LTE_VERSION_NAME"
 * 入参为NULL时，用默认的192.168.1.136作为tftp服务器的IP地址
*/
static LTE_RET_E download_lte_ver_file(char *pcFileName, const char *tftpServerIp)
{
    char acIpAddr[] = "255.255.255.255";
    char acCmd[128] = "";
    LTE_RET_E enRet;

    if(NULL == pcFileName)
    {
        return LTE_FAIL;
    }

    if (NULL == tftpServerIp)
    {
        strcpy(acIpAddr, "192.168.1.136");
    }
    else
    {
        strncpy(acIpAddr, tftpServerIp, strlen(acIpAddr));
    }

    sprintf(acCmd, "tftp -g -l /tmp/"LTE_VERSION_NAME" -r %s %s", pcFileName, acIpAddr);

    system_call("echo ----download "LTE_VERSION_NAME" by tftp start----");
    enRet = system_call(acCmd);
    if (LTE_OK != enRet)
    {
        system_call("echo ----download "LTE_VERSION_NAME" by tftp fail----");
        return LTE_FAIL;
    }
    
    system_call("echo ----download "LTE_VERSION_NAME" by tftp done----"); 

    return LTE_OK;
}

/*
 * /tmp/"LTE_VERSION_NAME"已经存在
*/
static LTE_RET_E update_from_local_path(char *pcFile, char *pcMd5)
{
    LTE_RET_E enRet = LTE_FAIL;
    char acStrCmd[128] = {0};
    
    system_call("echo lteupdate start at `date`");
    
    if(LTE_OK != lte_file_is_exists(pcFile))
    {   
        LTE_LOG(LTE_LOG_ERR, "File[%s] does not exists!\n", pcFile);
        return LTE_FAIL;
    }
    if(LTE_OK != check_file_md5(pcFile, pcMd5))
    {
        goto FAIL;
    }
    if(LTE_OK != check_file_size(pcFile))
    {
        goto FAIL;
    }
    
    enRet = lte_update(pcFile);
FAIL:

    sprintf(acStrCmd, "rm -rf %s", pcFile);
    system_call(acStrCmd);
    if(LTE_OK != enRet)
    {
    	system_call("echo lteupdate fail at `date`");	
    	return enRet;
    }
    system_call("echo lteupdate end at `date`");

    return enRet;
}

void show_help(void)
{
    printf("\nUsage:\n(1) lteupdate update filename md5sum [tftpServer Ip addr]\n\t---version update using tftp(Default IP addr: 192.168.1.136)\n"
                    "(2) lteupdate localupdate filename md5sum\n\t---update from local file.\n");
}

int main(int argc, char *argv[])
{
    LTE_RET_E enRet = LTE_FAIL;
   
    if (argc < 3 || argc > 5)
    {
        show_help();
        return -1;
    }
    
    if (strcmp(argv[1], "update") == 0)
    {

        if (LTE_OK != download_lte_ver_file(argv[2], argv[4]))
        {
            return -1;
        }
        enRet = update_from_local_path("/tmp/"LTE_VERSION_NAME, argv[3]);
        goto OVER;
    }

    if (strcmp(argv[1], "localupdate") == 0)
    {
        enRet = update_from_local_path(argv[2], argv[3]);
    }
OVER:
    if(LTE_OK == enRet)
    {
        return 0;
    }
    
    show_help();
    return -1;
}

