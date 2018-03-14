/******************************************************************************

                  版权所有 (C), 2000-2016, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : regcmd.c
  版 本 号   : 初稿
  作    者   : xiao wei
  生成日期   : 2016/03/6 10:02 
  最近修改   :
  功能描述   : register r/w tool
  函数列表   :
  修改历史   :                
  日    期   : 2016/03/6 10:02 
  作    者   : xiao wei
  修改内容   : 创建文件

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>	
#include <sys/ioctl.h>

#include "procfs_fops.h"
#include "reg_ioctl.h"

//extern unsigned int ath_global_reg_read(unsigned int uiRegaddr);
//extern void ath_global_reg_write(unsigned int uiRegaddr, unsigned int uiRegval);

//extern unsigned int ath_switch_reg_read(unsigned int uiRegaddr);
//extern void ath_switch_reg_write(unsigned int uiRegaddr, unsigned int uiRegval);

#define REG_CMD_MAX_NO           10

#define STR_MATCH(szStr1, szStr2) (0 == strcmp(szStr1, szStr2))
#define STR_NMATCH(szStr1, szStr2, uiSize) (0 == strncmp(szStr1, szStr2, uiSize))
#define MEM_MATCH(uiAddr1, uiAddr2, uiSize) (0 == memcmp(uiAddr1, uiAddr2, uiSize))

typedef struct 
{
	const CHAR *szCmdName;
	AP_ERROR_CODE_E (*fHandler)(INT32 iArgc, CHAR *apcArgv[]);
	const CHAR *szCmdFmt;
	const CHAR *szHelpInfo;
}REG_CMD_T;

AP_ERROR_CODE_E reg_show_help();
AP_ERROR_CODE_E reg_global_operate(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E reg_switch_operate(INT32 iArgc, CHAR *apcArgv[]);
AP_ERROR_CODE_E reg_phy_operate(INT32 iArgc, CHAR *apcArgv[]);

LOCAL REG_CMD_T g_astCmd [REG_CMD_MAX_NO] = 
{
    {"help", reg_show_help, "regcmd help",
            "Show register command help information. \n"
    },
    {"global", reg_global_operate, "regcmd global read addr | regcmd global write addr value",
            "Read/Write global register value. eg: regcmd global read 0x18070000 / regcmd global write 0x18070000 0x023c \n"
    },
    {"switch", reg_switch_operate, "regcmd switch read addr | regcmd switch write addr value",
            "Read/Write switch register value. eg: regcmd switch read 0x0058 / regcmd switch write 0x0078 0x0010 \n"
    },
    {"phy", reg_phy_operate, "regcmd phy read port addr | regcmd switch write port addr value",
            "Read/Write phy TAU: LAN1-PORT0 LAN2-port1 LAN3-port2 LAN4-port3 eg: regcmd switch read 0 0x0058 / regcmd switch write 1 0x0078 0x0010 \n"
    },
    {NULL, NULL, NULL, NULL}
};

REG_CMD_T *reg_get_cmd(CHAR *szCmdName)
{
    REG_CMD_T *pstPos;
    for (pstPos = g_astCmd; pstPos->fHandler; pstPos++)
    {
        if (STR_MATCH(szCmdName, pstPos->szCmdName))
        {
            return pstPos;
        }
    }
    return NULL;
}

AP_ERROR_CODE_E reg_show_help()
{
    REG_CMD_T *pstPos;
    
    printf("\nregister help information: \n\n");
    for (pstPos = g_astCmd; pstPos->fHandler; pstPos++)
    {
        printf("    %-15s -- CMD: %s\n", pstPos->szCmdName, pstPos->szCmdFmt);
        printf("    %-15s    %s\n", "", pstPos->szHelpInfo);
    }

    return AP_E_NONE;
}

AP_ERROR_CODE_E reg_addr_atoi( CHAR *addr_name )
{
    UINT32 uiValue = 0;
    UINT32 uiNumber = 0;
    CHAR * ptrAddr;
    UINT32 uiAddrNum = 0;
    
    if ( NULL == addr_name )
    {
        return 0;
    }

    ptrAddr = addr_name;
    
    if( *(ptrAddr+1) == 'x' || *(ptrAddr+1) == 'X' )
    {   
        uiNumber = 16;
        addr_name += 2;
    
        for(;;addr_name++)
        {
            switch(*addr_name)
            {
                case '0'...'9':
                    uiValue = uiValue*uiNumber + (*addr_name - '0');
                    break;
                case 'a'...'f':
                    uiValue = uiValue*uiNumber + (*addr_name - 'a') + 10;
                    break;
                case 'A'...'F':
                    uiValue = uiValue*uiNumber + (*addr_name - 'A') + 10;
                    break;
                default:
                    //printf("uiValue is %d, 0x%02x\n", uiValue, uiValue);
                    return uiValue;
            }
        }
    }
    else
    {   
        uiNumber = 10;

        for(;;addr_name++)
        {
            switch(*addr_name)
            {
                case '0'...'9':
                    uiValue = uiValue*uiNumber + (*addr_name - '0');
                    break;
                default:
                    //printf("uiValue is %d, 0x%02x\n", uiValue, uiValue);
                    return uiValue;
            }
        }
    }

    return AP_E_NONE;
}

AP_ERROR_CODE_E reg_global_operate(INT32 iArgc, CHAR *apcArgv[])
{
	//printf("iArgc=%d apcArgv[0]=%s apcArgv[1]=%s apcArgv[2]=%s apcArgv[3]=%s\n", iArgc, apcArgv[0], apcArgv[1], apcArgv[2], apcArgv[3]);
	INT32 enRet = 0;
	UINT32 uiRegRetAddr = 0;
	UINT32 uiRegRetValu = 0;
	
	LOCAL REGISTER_IOCTL_T stIoctl = {0};
	
    if( iArgc >= 3 && !strcmp(apcArgv[0], "global") )
    {
        if( iArgc == 3 && !strcmp(apcArgv[1], "read") )
    	{
    		//printf("command ok !\n");
    		uiRegRetAddr = reg_addr_atoi(apcArgv[2]);
    		//printf("regcmd global read addr = 0x%x\n", uiRegRetAddr);
    		
    		stIoctl.stRegData.enRegType = REGISTER_IOCTL_ADDR_GLOBAL;
    		stIoctl.stRegData.uiRegAddr = uiRegRetAddr;
    		enRet = register_ioctl(&stIoctl, REGISTER_IOCTL_READ);
    	}
    	else if ( iArgc == 4 && !strcmp(apcArgv[1], "write") )
    	{
    		//printf("command ok !\n");
    		uiRegRetAddr = reg_addr_atoi(apcArgv[2]);
    		uiRegRetValu = reg_addr_atoi(apcArgv[3]);
    		//printf("regcmd global write addr = 0x%x val = 0x%x\n", uiRegRetAddr, uiRegRetValu);
    		
    		stIoctl.stRegData.enRegType = REGISTER_IOCTL_ADDR_GLOBAL;
    		stIoctl.stRegData.uiRegAddr = uiRegRetAddr;
    		stIoctl.stRegData.uiRegValu = uiRegRetValu;
    		enRet = register_ioctl(&stIoctl, REGISTER_IOCTL_WRITE);
    	}
    	else 
    	{
    		printf("command global read/write error ! \n");
    		return AP_E_DEFAULT_FAIL;
    	}
    }
    else
    {
    	printf("command no global ! \n");
    	return AP_E_DEFAULT_FAIL;
    }
    
    if (AP_E_NONE == enRet)
    {
    	if( REGISTER_IOCTL_SUCCESS == stIoctl.stRegData.uiRetValu )
        {
            printf("\nregcmd global read/write addr = 0x%x value = 0x%x\n", uiRegRetAddr, stIoctl.stRegData.uiRegValu);
        	printf("register read/write success !\n");
        }
        else
        {
            printf("register global read/write failed !\n");
            return AP_E_DEFAULT_FAIL;
        }
    }
    else
    {
        printf("Call register_ioctl failed or No command enRet = %d\n", enRet);
        return AP_E_DEFAULT_FAIL;
    }
    printf("\n");
    
    return AP_E_NONE;
}
AP_ERROR_CODE_E reg_switch_operate(INT32 iArgc, CHAR *apcArgv[])
{
	//printf("iArgc=%d apcArgv[0]=%s apcArgv[1]=%s apcArgv[2]=%s apcArgv[3]=%s\n", iArgc, apcArgv[0], apcArgv[1], apcArgv[2], apcArgv[3]);
	INT32 enRet = 0;
	UINT32 uiRegRetAddr = 0;
	UINT32 uiRegRetValu = 0;
	
	LOCAL REGISTER_IOCTL_T stIoctl = {0};
	
    if( iArgc >= 3 && !strcmp(apcArgv[0], "switch") )
    {
        if( iArgc == 3 && !strcmp(apcArgv[1], "read") )
    	{
    		//printf("command ok !\n");
    		uiRegRetAddr = reg_addr_atoi(apcArgv[2]);
    		//printf("regcmd switch read addr = 0x%x\n", uiRegRetAddr);
    		
    		stIoctl.stRegData.enRegType = REGISTER_IOCTL_ADDR_SWITCH;
    		stIoctl.stRegData.uiRegAddr = uiRegRetAddr;
    		enRet = register_ioctl(&stIoctl, REGISTER_IOCTL_READ);
    	}
    	else if ( iArgc == 4 && !strcmp(apcArgv[1], "write") )
    	{
    		//printf("command ok !\n");
    		uiRegRetAddr = reg_addr_atoi(apcArgv[2]);
    		uiRegRetValu = reg_addr_atoi(apcArgv[3]);
    		//printf("regcmd switch write addr = 0x%x val = 0x%x\n", uiRegRetAddr, uiRegRetValu);
    		
    		stIoctl.stRegData.enRegType = REGISTER_IOCTL_ADDR_SWITCH;
    		stIoctl.stRegData.uiRegAddr = uiRegRetAddr;
    		stIoctl.stRegData.uiRegValu = uiRegRetValu;
    		enRet = register_ioctl(&stIoctl, REGISTER_IOCTL_WRITE);
    	}
    	else 
    	{
    		printf("command switch read/write error ! \n");
    		return AP_E_DEFAULT_FAIL;
    	}
    }
    else
    {
    	printf("command no switch ! \n");
    	return AP_E_DEFAULT_FAIL;
    }
    
    if (AP_E_NONE == enRet)
    {
    	if( REGISTER_IOCTL_SUCCESS == stIoctl.stRegData.uiRetValu )
        {
            printf("\nregcmd switch read/write addr = 0x%x value = 0x%x\n", uiRegRetAddr, stIoctl.stRegData.uiRegValu);
        	printf("register read/write success !\n");
        }
        else
        {
            printf("register switch read/write failed !\n");
            return AP_E_DEFAULT_FAIL;
        }
    }
    else
    {
        printf("Call register_ioctl failed or No command enRet = %d\n", enRet);
        return AP_E_DEFAULT_FAIL;
    }
    printf("\n");
    
    return AP_E_NONE;
}
AP_ERROR_CODE_E reg_phy_operate(INT32 iArgc, CHAR *apcArgv[])
{
	//printf("iArgc=%d apcArgv[0]=%s apcArgv[1]=%s apcArgv[2]=%s apcArgv[3]=%s\n", iArgc, apcArgv[0], apcArgv[1], apcArgv[2], apcArgv[3]);
	INT32 enRet = 0;
	UINT32 uiRegRetAddr = 0;
	UINT32 uiRegRetValu = 0;
	UINT32 uiRegPhyPort = 0;
	
	LOCAL REGISTER_IOCTL_T stIoctl = {0};
	
    if( iArgc >= 4 && !strcmp(apcArgv[0], "phy") )
    {
    	uiRegPhyPort = reg_addr_atoi(apcArgv[2]);
    	stIoctl.stRegData.uiRegPhy = uiRegPhyPort;
    	stIoctl.stRegData.enRegType = REGISTER_IOCTL_ADDR_PHY;
        if( iArgc == 4 && !strcmp(apcArgv[1], "read") )
    	{
    		//printf("command ok !\n");
    		uiRegRetAddr = reg_addr_atoi(apcArgv[3]);
    		//printf("regcmd switch read addr = 0x%x\n", uiRegRetAddr);
    		
    		stIoctl.stRegData.uiRegAddr = uiRegRetAddr;
    		enRet = register_ioctl(&stIoctl, REGISTER_IOCTL_READ);
    	}
    	else if ( iArgc == 5 && !strcmp(apcArgv[1], "write") )
    	{
    		//printf("command ok !\n");
    		uiRegRetAddr = reg_addr_atoi(apcArgv[3]);
    		uiRegRetValu = reg_addr_atoi(apcArgv[4]);
    		//printf("regcmd switch write addr = 0x%x val = 0x%x\n", uiRegRetAddr, uiRegRetValu);
    		
    		stIoctl.stRegData.uiRegAddr = uiRegRetAddr;
    		stIoctl.stRegData.uiRegValu = uiRegRetValu;
    		enRet = register_ioctl(&stIoctl, REGISTER_IOCTL_WRITE);
    	}
    	else 
    	{
    		printf("command phy read/write error ! \n");
    		return AP_E_DEFAULT_FAIL;
    	}
    }
    else
    {
    	printf("command no phy ! \n");
    	return AP_E_DEFAULT_FAIL;
    }
    
    if (AP_E_NONE == enRet)
    {
    	if( REGISTER_IOCTL_SUCCESS == stIoctl.stRegData.uiRetValu )
        {
            printf("\nregcmd switch read/write port = %d addr = 0x%x value = 0x%x\n",uiRegPhyPort, uiRegRetAddr, stIoctl.stRegData.uiRegValu);
        	printf("register read/write success !\n");
        }
        else
        {
            printf("register switch read/write failed !\n");
            return AP_E_DEFAULT_FAIL;
        }
    }
    else
    {
        printf("Call register_ioctl failed or No command enRet = %d\n", enRet);
        return AP_E_DEFAULT_FAIL;
    }
    printf("\n");
    
    return AP_E_NONE;
}

INT32 main(INT32 iArgc, CHAR *apcArgv[])
{
	AP_ERROR_CODE_E enRet;
	REG_CMD_T *pstCmd;
    CHAR *szCmdName = NULL;

    //printf("\nInput iArgc=%d\n", iArgc);

    if( 1 == iArgc ) 
    {
        reg_show_help();
        return 0;
    }

    szCmdName = apcArgv[1];
    pstCmd = reg_get_cmd(szCmdName);
    if (NULL == pstCmd)
    {
        printf("Command no find ! please use: regcmd help\n");
        return -1;
    }
    
    iArgc--;
    apcArgv++;
    enRet = pstCmd->fHandler(iArgc, apcArgv);
    if (AP_E_NONE != enRet)
    {
        printf("Command %s executed failed. ret %d\n", szCmdName, enRet);
    }
    
	return 0;
}