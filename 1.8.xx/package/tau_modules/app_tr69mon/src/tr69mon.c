/******************************************************************************

                  °æÈ¨ËùÓÐ (C), 2001-2011, ÉÏº£å¾´´Í¨ÐÅ¹É·ÝÓÐÏÞ¹«Ë¾

 ******************************************************************************
  ÎÄ ¼þ Ãû   : ltegetinfo.c
  °æ ±¾ ºÅ   : ³õ¸å
  ×÷    Õß   : xiao wei
  Éú³ÉÈÕÆÚ   : 2016-12-13
  ×î½üÐÞ¸Ä   :
  ¹¦ÄÜÃèÊö   :
  º¯ÊýÁÐ±í   :
  ÐÞ¸ÄÀúÊ·   :
  1.ÈÕ    ÆÚ   : 2016Äê3ÔÂ23ÈÕ ÐÇÆÚÈý
    ×÷    Õß   : xiao wei
    ÐÞ¸ÄÄÚÈÝ   : ´´½¨ÎÄ¼þ

******************************************************************************/
/*----------------------------------------------*
 * °üº¬Í·ÎÄ¼þ                                   *
 *----------------------------------------------*/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <asm/types.h>

#include <errno.h>  
#include <unistd.h>
#include "uci_config.h"
#include "pal_include/pal_uci.h"
#include "util_include/util.h"
#include "ap_types.h"
#include "lte.h"

#define PROCESS_TR069         "tr069"

typedef struct {
    int pid;
    /* basename of executable file in call to exec(2),
        size from kernel headers */
    char short_cmd[16];
} procps_status_t;

int util_system_cmd( char *szCmd )
{
    int iRet;
    int iLoop;

    if  ( NULL == szCmd )
    {
        return 1;
    }

    for  ( iLoop = 0 ; iLoop < 5 ; iLoop++ )
    {
         iRet = system(szCmd);
         if  ( -1 != iRet && WIFEXITED(iRet) && 0 == WEXITSTATUS(iRet))
         {
             //syslog(7, "[util_system_cmd] Success: %s\n", szCmd);
             return 0;
         }
    }
    syslog(3, "[util_system_cmd] Failed: %s,status = %d\n", szCmd, iRet);

    return iRet;
}
int util_find_pid_by_name(char *pcPidName, int *piPid)
{
    static DIR *pstDir = NULL;
	struct dirent *pstEntry = NULL;
	char *pcName = NULL;
	procps_status_t stCurstatus = {0};
	char acBuf[512] = {0};
	FILE *pstFp = NULL;
	int iPid = 0;
	struct stat stFileStatus = {0};
    
	char acDirName[32] = {0};
    
	if (!pstDir) 
    {
		pstDir = opendir("/proc");
		if (!pstDir)
        {      
            syslog(3, "Can't open /proc\n");
            return -2;
        }
	}
    while ( 1 ) 
    {
        memset(acBuf,0,sizeof(acBuf));
        memset(acDirName,0,sizeof(acDirName));
        memset(stCurstatus.short_cmd,0,sizeof(stCurstatus.short_cmd));
        
    	if ((pstEntry = readdir(pstDir)) == NULL)
        {
    		closedir(pstDir);
    		pstDir = NULL;
    		return -1;
    	}
        
    	pcName = pstEntry->d_name;
    	if (!(*pcName >= '0' && *pcName <= '9'))
        {
    		continue;
        }   

    	memset(&stCurstatus, 0, sizeof(procps_status_t));
    	iPid = atoi(pcName);
    	stCurstatus.pid = iPid;
    	sprintf(acDirName, "/proc/%d", iPid);
        
    	if (stat(acDirName, &stFileStatus))
        {
    		continue;
        }   

        memset(acDirName,0,sizeof(acDirName));
    	sprintf(acDirName, "/proc/%d/stat", iPid);
        
    	if((pstFp = fopen(acDirName, "r")) == NULL)
        {
            continue;
        }   

    	pcName = fgets(acBuf, sizeof(acBuf), pstFp);
        
    	fclose(pstFp);
        pstFp = NULL;
        
        acBuf[511] = '\0';
        
    	if (pcName == NULL)
        {
    		continue;
        }   

    	pcName = strrchr(acBuf, ')');
    	if(pcName == 0 || pcName[1] != ' ')
        {
    		continue;
        }   
    	*pcName = 0;
    	sscanf(acBuf, "%*s (%15c", stCurstatus.short_cmd);
        //printf("stCurstatus.short_cmd is %s\n",stCurstatus.short_cmd);
        if(0 == strncmp(stCurstatus.short_cmd,pcPidName,strlen(pcPidName)))
        {
            while((pstEntry = readdir(pstDir)) != NULL)
            {
            }
            closedir(pstDir);
            pstDir = NULL;
            *piPid = iPid;
            return 1;
        }
    }
    //printf("finally return\n");
    return 1;
}

LOCAL AP_ERROR_CODE_E monitor_process(const CHAR *szProcess)
{
	CHAR acCmd[CHAR_LEN_128] = {0};
	static INT32 iBoaReStats = 0;
    INT32 iPid;
	INT32 iRet;
	
	iRet = util_find_pid_by_name(szProcess, &iPid);
	if (1 != iRet)
	{
		printf("[func:%s] %s restart\n", __FUNCTION__, szProcess);
		sprintf(acCmd, "%s -D > /dev/null 2>&1 &",szProcess);
		syslog(3,"system Restart %s fail!\n", PROCESS_TR069);
		iRet = util_system_cmd(acCmd);
		if (0 != iRet)
		{
			return AP_E_DEFAULT_FAIL;
		}
	}
		
    return AP_E_NONE;
}

int main(int iArgc, char **apcArgv[])
{
	int iRet =0 ;
	UINT32 ulTick=0;
	char trSwitch[8] = {0};
	
	while(1)
	{
		if (0 == ulTick % 5)
        {
        	
        	uci_config_get("tr069.config.switch",trSwitch);
        	if(atoi(trSwitch) == 1){
            	iRet = monitor_process(PROCESS_TR069);
            	if (AP_E_NONE != iRet)
            	{
                	syslog(3,"Faltal!Restart %s fail!\n", PROCESS_TR069);
           	 	}
         	}
        } 	
	
		ulTick++;
		sleep(1);
	}

    return AP_E_NONE;
}