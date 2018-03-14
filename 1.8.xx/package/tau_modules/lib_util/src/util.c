/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : util.c
  版 本 号   : 初稿
  作    者   : zhanghaozhong
  生成日期   : 2013年3月13日
  最近修改   :
  功能描述   : 通用函数库
  函数列表   :
              util_system_cmd
              util_lock_file
              util_unlock_file
              util_itoa
              util_find_pid_by_name
  修改历史   :
  1.日    期   : 2013年3月13日
    作    者   : zhanghaozhong
    修改内容   : 创建文件

******************************************************************************/
/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <netdb.h>
#include <net/if.h>
#include <sys/socket.h>
#if __GLIBC__ >=2 && __GLIBC_MINOR >= 1
#include <netpacket/packet.h>
#include <net/ethernet.h>
#else
#include <asm/types.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#endif


#include "util_include/util.h"


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
 * 模块级变量                                   *
 *----------------------------------------------*/
typedef struct {
    int pid;
    /* basename of executable file in call to exec(2),
        size from kernel headers */
    char short_cmd[16];
} procps_status_t;

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
char * util_uint8_to_macstr(char *pcMacstr, unsigned char *pucMac )
{
    sprintf(pcMacstr, "%02x:%02x:%02x:%02x:%02x:%02x",pucMac[0],pucMac[1],pucMac[2],pucMac[3],pucMac[4],pucMac[5]);

    return pcMacstr;
}


/*****************************************************************************
 函 数 名  : util_system_cmd
 功能描述  : 系统调用
 输入参数  : CHAR *pcCmd
 输出参数  : 无
 返 回 值  : INT32
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月13日星期三
    作    者   : 王刚
    修改内容   : 新生成函数

*****************************************************************************/
int util_system_cmd( char *szCmd )
{
    int iRet;
    int iLoop;

    if  ( NULL == szCmd )
    {
        return 1;
    }

    /* 此处为使util独立，直接采用syslog打印信息 */
    for  ( iLoop = 0 ; iLoop < 5 ; iLoop++ )
    {
         iRet = system(szCmd);
         if  ( -1 != iRet && WIFEXITED(iRet) && 0 == WEXITSTATUS(iRet))
         {
            /* 7 表示日志级别为DEBUG级别 */
             //syslog(7, "[util_system_cmd] Success: %s\n", szCmd);
             return 0;
         }
    }
    /* 3 表示日志级别为ERR级别 */
    syslog(3, "[util_system_cmd] Failed: %s,status = %d\n", szCmd, iRet);

    return iRet;
}

/*****************************************************************************
 函 数 名  : util_popen_cmd
 功能描述  : popen执行命令
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年1月4日
    作    者   : zhangjiasi
    修改内容   : 新生成函数

*****************************************************************************/
int  util_popen_cmd( char *pcCmd,char *pcOut,unsigned int uiOutLenMax )
{
    FILE *pstFp = NULL;
    
    if (NULL == pcCmd || NULL == pcOut)
    {
        syslog(3, "[util_popen_cmd] pointer is NULL");

        return 1;
    }

    pstFp = popen(pcCmd, "r");
    if (NULL == pstFp)
    {
        syslog(3, "[util_popen_cmd] pointer is NULL");
        return 1;
    }

    if (fgets(pcOut, uiOutLenMax, pstFp))
    {
        pcOut[strlen(pcOut) - 1] = '\0';
        pclose(pstFp);
    }
    else
    {
        pclose(pstFp);
    }
    
    return 0;
}
/*****************************************************************************
 函 数 名  : util_lock_file
 功能描述  : 给文件加锁
 输入参数  : struct flock stLock  
             int iComfd           
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月13日
    作    者   : zhanghaozhong
    修改内容   : 新生成函数

*****************************************************************************/
int util_lock_file(struct flock *stLock,int iComfd)
{    
	stLock->l_type = F_WRLCK;
	stLock->l_start = 0;
	stLock->l_whence = SEEK_SET;
	stLock->l_len = 0;
	int iRet = 0;
	iRet = fcntl(iComfd,F_SETLKW,stLock);
	//printf("%s,%d:try to get a lock\n",__func__,__LINE__);
	if(iRet < 0)
	{
        syslog(3,"%s:warming add lock failed\n",__func__);
		return 1;
	}
    //printf("%s,%d:get a lock\n",__func__,__LINE__);
    return 0;
}
/*****************************************************************************
 函 数 名  : util_unlock_file
 功能描述  : 给文件解锁
 输入参数  : struct flock stLock  
             int iComfd           
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月13日
    作    者   : zhanghaozhong
    修改内容   : 新生成函数

*****************************************************************************/
int util_unlock_file(struct flock *stLock,int iComfd)
{
	stLock->l_type = F_UNLCK;
	int iRet = 0;
	iRet = fcntl(iComfd,F_SETLKW,stLock);
	if(iRet < 0)
	{
        syslog(3, "%s:warming  unlock failed\n",__func__);
		return 1;
	}
	return 0;
}

/*****************************************************************************
 函 数 名  : itoa
 功能描述  : 将整数转换为字符串
 输入参数  : int value  
             char *str  
             int radix  
 输出参数  : 无
 返 回 值  : char
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月13日星期三
    作    者   : 王刚
    修改内容   : 新生成函数

*****************************************************************************/
char *util_itoa(int value, char *str, int radix)
{	
	//字符映射表 
	static char szMap[] = {
		'0', '1', '2', '3', '4', '5',
		'6', '7', '8', '9', 'a', 'b',
		'c', 'd', 'e', 'f', 'g', 'h',
		'i', 'j', 'k', 'l', 'm', 'n',
		'o', 'p', 'q', 'r', 's', 't',
		'u', 'v', 'w', 'x', 'y', 'z'
	};
	int nCount = -1,nIndex;
	char *pStr = str,nTemp;
	unsigned int nValue;

    memset(pStr,0,strlen(pStr));
	//限制radix必须在2到36之间  
	if (radix >= 2 && radix <= 36)   
	{
		//如果是负数就在首位添加负号，并将字符串前移 
		if (value < 0 && radix == 10)
		{
			*pStr++ = '-';
			value = -value;//转为正数，
		}
		nValue = *(unsigned*)&value;

		//循环转换每一个数字，直到结束
		do {
			pStr[++nCount] = szMap[nValue % radix];
			nValue /= radix;
		}
		while (nValue > 0);//转换结束后字符串是翻的

		//计算出一半的长度
		nIndex = (nCount + 1)/2;

		//将字符串的字符序翻转
		while (nIndex-- > 0)
		{
			nTemp = pStr[nIndex];
			pStr[nIndex] = pStr[nCount - nIndex];
			pStr[nCount - nIndex] = nTemp;
		}
	}

	//置结束符
	pStr[nCount + 1] = '\0';

	return str;
}
/*****************************************************************************
 函 数 名  : atou8
 功能描述  : atou8
 输入参数  : uchar * string  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月25日星期一
    作    者   : liyoukun
    修改内容   : 新生成函数

*****************************************************************************/
unsigned char atou8(char *string)
{
	unsigned char res = 0;
	while (*string >= '0' && *string <= '9') {
		res *= 10;
		res += *string - '0';
		string++;
	}

	return res;
}
/*****************************************************************************
 函 数 名  : atou8x
 功能描述  : str转换为16进制数
 输入参数  : char *string
 输出参数  : 无
 返 回 值  : unsigned char
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月29日星期五
    作    者   : liyoukun
    修改内容   : 新生成函数

*****************************************************************************/
unsigned char atou8x( char *string )
{
    unsigned char res = 0;
    unsigned char tmp = 0;
	while ((*string >= '0' && *string <= '9')||(*string >= 'a' && *string <= 'f')||(*string >= 'A' && *string <= 'F')) 
    {
        res *= 16;
        if(*string >= '0' && *string <= '9')
        {
            res += *string - '0';
        }
        else if(*string >= 'a' && *string <= 'f')
        {
            res += *string - 'a' + 10;      /* a-f的10进制数为10-15 */
        }
        else if(*string >= 'A' && *string <= 'F')
        {
            res += *string - 'A' + 10;      /* a-f的10进制数为10-15 */
        }
        string++;
	}
    
	return res;
}

/*****************************************************************************
 函 数 名  : util_find_pid_by_name
 功能描述  : 根据名称获取到进程号
 输入参数  : char *pcPidName  
             int *iPid        
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月13日星期三
    作    者   : 王刚
    修改内容   : 新生成函数

*****************************************************************************/
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
    
    /*存放各级目录名称*/
	char acDirName[32] = {0};
    
    /*打开proc文件系统*/
	if (!pstDir) 
    {
		pstDir = opendir("/proc");
		if (!pstDir)
        {      
            syslog(3, "Can't open /proc\n");
            return -2;
        }
	}
    /*一次读取proc文件系统下的数据文件夹*/
    while ( 1 ) 
    {
        memset(acBuf,0,sizeof(acBuf));
        memset(acDirName,0,sizeof(acDirName));
        memset(stCurstatus.short_cmd,0,sizeof(stCurstatus.short_cmd));
        
        /*读取参数dir目录流的下个目录进入点，读完没有找到则退出*/
    	if ((pstEntry = readdir(pstDir)) == NULL)
        {
    		closedir(pstDir);
    		pstDir = NULL;
    		return -1;
    	}
        
        /*属性:文件名*/
    	pcName = pstEntry->d_name;
        /*在proc文件系统下每个进程都有其进程号对应的文件夹，且文件夹名称是pid号*/
    	if (!(*pcName >= '0' && *pcName <= '9'))
        {
    		continue;
        }   

    	memset(&stCurstatus, 0, sizeof(procps_status_t));
    	iPid = atoi(pcName);
    	stCurstatus.pid = iPid;
    	sprintf(acDirName, "/proc/%d", iPid);
        
        /*获取文件夹的状态，确定文件的存在性*/
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

        /*获取文件内容*/
    	pcName = fgets(acBuf, sizeof(acBuf), pstFp);
        
        /*关闭文件*/
    	fclose(pstFp);
        pstFp = NULL;
        
        acBuf[511] = '\0';
        
    	if (pcName == NULL)
        {
    		continue;
        }   

        /*找到最后字符出现的最后地址，并将其返回*/
        /* split into "PID (cmd" and "<rest>" */
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
            /*将文件的目录流读完*/
            while((pstEntry = readdir(pstDir)) != NULL)
            {
                /*将目录流读空*/;
            }
            /*读空后关闭*/   
            closedir(pstDir);
            pstDir = NULL;
            *piPid = iPid;
            return 1;
        }
    }
    //printf("finally return\n");
    return 1;
}


/*****************************************************************************
 函 数 名  : util_ip_to_str
 功能描述  : 将数组形式的IP转换为字符串形式的IP地址
 输入参数  : unsigned char *ucIp
             char *pcIpAddr
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月14日星期四
    作    者   : 王刚
    修改内容   : 新生成函数

*****************************************************************************/
int util_ip_to_str( unsigned char *pucIp, char *pcIpAddr )
{
    
    /* 入参检测 */
    if (NULL == pucIp)
    {
        return 1;
    }

    if (NULL == pcIpAddr)
    {
        return 1;
    }

    sprintf(pcIpAddr, "%d.%d.%d.%d", pucIp[0], pucIp[1], pucIp[2], pucIp[3]);

    return 0;
}


/*****************************************************************************
 函 数 名  : util_str_to_ip
 功能描述  : 将字符串形式的IP地址转换为数组
 输入参数  : char *pcIpAddr
             unsigned char *pucIp
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月14日星期四
    作    者   : 王刚
    修改内容   : 新生成函数

*****************************************************************************/
int util_str_to_ip( char *pcIpAddr, unsigned char *pucIp )
{
    int iTmp[4];
    
     /* 入参检测 */
    if (NULL == pucIp)
    {
        return 1;
    }

    if (NULL == pcIpAddr)
    {
        return 1;
    }

    sscanf(pcIpAddr, "%d.%d.%d.%d",&iTmp[0], &iTmp[1], &iTmp[2], &iTmp[3]);
    pucIp[0] = iTmp[0];
    pucIp[1] = iTmp[1];
    pucIp[2] = iTmp[2];
    pucIp[3] = iTmp[3];

    return 0;
}

/*****************************************************************************
 函 数 名  : util_str_to_number
 功能描述  : 将特定形式的字符串转换为数组
 输入参数  : char *acScrStr
             int *iCount
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月15日星期五
    作    者   : 王刚
    修改内容   : 新生成函数

*****************************************************************************/
int util_str_to_number( char *pcScrStr, int *piDesInt ,char cFlag)
{
    char *pcBuff = NULL;
    char buff[128 + 1] = {0};
    int iLoop = 0;
    int iCount = 0;
    int len = 0;

    /* 入参检测 */
    if (NULL == pcScrStr)
    {
        /* 打印错误日志 */
        syslog(6, "pcScrStr is NULL");
        return 1;
    }

    if (NULL == piDesInt)
    {
        syslog(6, "piDesInt is NULL");
        return 1;
    }

    /* 将以cFlag分隔的字符串转换为int数组 */
    strcpy(buff, pcScrStr);
    len = strlen(pcScrStr) + 1;
    pcBuff = buff;
    for (iLoop = 0; iLoop < len; iLoop++)
    {
        if (cFlag == buff[iLoop])
        {
            buff[iLoop] = '\0';
            piDesInt[iCount] = atoi(pcBuff);
            pcBuff = &buff[iLoop + 1];
            iCount++;
            continue;
        }
        else if ('\0' == buff[iLoop])
        {
            piDesInt[iCount] = atoi(pcBuff);
            break;
        }
    }
    pcBuff = NULL;
    
    return 0;
}

/*****************************************************************************
 函 数 名  : util_str_to_uint8
 功能描述  : 把特定字符串转换为UINT8型数组
 输入参数  : 无
 输出参数  : 无
 返 回 值  : int 数组大小
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月25日星期一
    作    者   : liyoukun
    修改内容   : 新生成函数

*****************************************************************************/
int util_str_to_uint8( char *pcScrStr, UINT8 *piDesInt ,char cFlag )
{
    char *pcBuff = NULL;
    char buff[128 + 1] = {0};
    int iLoop = 0;
    int iCount = 0;
    int len = 0;

    /* 入参检测 */
    if (NULL == pcScrStr)
    {
        /* 打印错误日志 */
        syslog(6, "pcScrStr is NULL");
        return -1;
    }
    if (NULL == piDesInt)
    {
        syslog(6, "piDesInt is NULL");
        return -1;
    }

    /* 将以cFlag分隔的字符串转换为UINT8数组 */
    strcpy(buff, pcScrStr);
    len = strlen(pcScrStr) + 1;
    pcBuff = buff;
    for (iLoop = 0; iLoop < len; iLoop++)
    {
        if (cFlag == buff[iLoop])
        {
            buff[iLoop] = '\0';
            if (iCount >= 4)
            {
               piDesInt[iCount] = '\0';
               return;
            }
            piDesInt[iCount] = atou8(pcBuff);
            pcBuff = &buff[iLoop + 1];
            iCount++;
            continue;
        }
        else if ('\0' == buff[iLoop])
        {
            piDesInt[iCount] = atou8(pcBuff);
            break;
        }
    }
    pcBuff = NULL;
    return iCount;
}
/*****************************************************************************
 函 数 名  : util_macstr_to_uint8
 功能描述  : 把MAC地址转换为uint8型数组
 输入参数  : char *pcScrStr, 
             UINT8 *piDesInt ,
             char cFlag
 输出参数  : 无
 返 回 值  : unsigned char
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月29日星期五
    作    者   : liyoukun
    修改内容   : 新生成函数

*****************************************************************************/
unsigned char util_macstr_to_uint8( char *pcScrStr, UINT8 *piDesInt ,char cFlag )
{
    char *pcBuff = NULL;
    char buff[128 + 1] = {0};
    int iLoop = 0;
    int iCount = 0;
    int len = 0;

    /* 入参检测 */
    if (NULL == pcScrStr)
    {
        /* 打印错误日志 */
        syslog(6, "pcScrStr is NULL");
        return 1;
    }
    if (NULL == piDesInt)
    {
        syslog(6, "piDesInt is NULL");
        return 1;
    }

    /* 将以cFlag分隔的字符串转换为UINT8数组 */
    strcpy(buff, pcScrStr);
    len = strlen(pcScrStr) + 1;
    pcBuff = buff;
    for (iLoop = 0; iLoop < len; iLoop++)
    {
        if (cFlag == buff[iLoop])
        {
            buff[iLoop] = '\0';
            piDesInt[iCount] = atou8x(pcBuff);
            pcBuff = &buff[iLoop + 1];
            iCount++;
            continue;
        }
        else if ('\0' == buff[iLoop])
        {
            piDesInt[iCount] = atou8x(pcBuff);
            break;
        }
    }
    pcBuff = NULL;
    return 0;
}


/*****************************************************************************
 函 数 名  : util_mac_to_str
 功能描述  : 将uint8的MAC地址转换为字符串
 输入参数  : unsigned char *pucMac
             char *pcMacStr
 输出参数  : 无
 返 回 值  : char *
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年6月9日星期日
    作    者   : 王刚
    修改内容   : 新生成函数

*****************************************************************************/
char * util_mac_to_str( unsigned char *pucMac, char *pcMacStr )
{
    if (NULL == pucMac || NULL == pcMacStr)
    {
        return NULL;
    }
    
    sprintf(pcMacStr, "%02x:%02x:%02x:%02x:%02x:%02x", pucMac[0],pucMac[1],pucMac[2],pucMac[3],pucMac[4],pucMac[5]);

    return pcMacStr;
}

/*****************************************************************************
 函 数 名  : util_array_string_to_int
 功能描述  : 将字符串数组转化成int型
             CHAR *src ---源字符串
             CHAR *dst ---数字存放地址
             UINT32 *total  ---转化后数字总数
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年8月6日
    作    者   : zhangjiasi
    修改内容   : 新生成函数

*****************************************************************************/
int  util_array_string_to_int( char *src,char *dst,unsigned int *total )
{
    char *start, *end;
    char *buff;
    int flag;
    
    if(NULL == src || NULL == dst)
    {
        syslog(3, "pcScrStr is NULL");
        return 1;
    }
    buff = NULL;
    buff = (char *)malloc(strlen(src)+1);       /* modify by zhz 2013-12-27 */
    if(NULL == buff)
    {
        syslog(3, "malloc failed");
        return 1;
    }
    memcpy(buff, src, strlen(src)+1);

    flag = 0;
    (*total) = 0;
    start = end = buff;
    while('\0' != *end)
    {
        flag = 1;
        if(*end != ',')
        {
            end++;
            continue;
        }
        else
        {
            *end = '\0';
            end++;
            *dst = atoi(start);
            dst++;
            start = end;
            (*total)++;
        }
    }
    if(1 == flag)
    {
        *dst = atoi(start);
        (*total)++;
    }
    if(NULL != buff)
    {
        free(buff);
    }
    return 0;
}

/*****************************************************************************
 函 数 名  : util_array_add_someone
 功能描述  : 添加某个字符
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年8月6日
    作    者   : zhangjiasi
    修改内容   : 新生成函数

*****************************************************************************/
int  util_array_add_someone( char *src,int id )
{
    char tmp[16];
    char *list;
    int len;
    int total, loop, flag;

    if(NULL == src)
    {
        return 1;
    }
    len = strlen(src);
    list = NULL;
    list = (char *)malloc(len);
    if(NULL == list)
    {
        return 1;
    }
    util_array_string_to_int(src, list, &total);
    flag = 0;
    memset(src, 0, len);
    if(0 == total)
    {
        sprintf(tmp, "%d,", id);
        strcat(src, tmp);
    }
    else
    {
        for(loop = 0; loop < total; loop++)
        {
            if(id < list[loop] && 0 == flag)
            {
                sprintf(tmp, "%d,%d,", id, list[loop]);
                flag = 1;
            }
            else
            {
                sprintf(tmp, "%d,", list[loop]);
            }
            strcat(src, tmp);
        }
        if(0 == flag)
        {
            sprintf(tmp, "%d,", id);
            strcat(src, tmp);
        }
    }
    src[strlen(src)-1] = '\0';
    if(NULL != list)
    {
        free(list);
    }
    return 0;
}

/*****************************************************************************
 函 数 名  : util_array_del_someone
 功能描述  : 删除某个字符
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年8月6日
    作    者   : zhangjiasi
    修改内容   : 新生成函数

*****************************************************************************/
int  util_array_del_someone( char *src,int id )
{
    char tmp[16];
    char *list;
    int len;
    int total, loop, flag;

    if(NULL == src)
    {
        return 1;
    }
    len = strlen(src);
    list = NULL;
    list = (char *)malloc(len);
    if(NULL == list)
    {
        return 1;
    }
    util_array_string_to_int(src, list, &total);
    flag = 0;
    memset(src, 0, len);
    for(loop = 0; loop < total; loop++)
    {
        if(id != list[loop])
        {
            sprintf(tmp, "%d,", list[loop]);
            strcat(src, tmp);
        }
    }
    src[strlen(src)-1] = '\0';
    if(NULL != list)
    {
        free(list);
    }
    return 0;

}

/*****************************************************************************
 函 数 名  : util_get_file_lines
 功能描述  : 获取一个函数的总行数
 输入参数  : file *fp
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月30日星期六
    作    者   : liyoukun
    修改内容   : 新生成函数

*****************************************************************************/
int util_get_file_lines( FILE *fp )
{

    int line=0;
    char c, cb='\0';  //cb存储当前输入字符的前一个字符
    /* 入参检测 */
    if (NULL == fp)
    {
        /* 打印错误日志 */
        syslog(6, "fp is NULL");
        return 1;
    }
    while((c=getc(fp))!=EOF)
    {
        if(c == '\n')
        line++;
        cb = c;
    }
    /* 防止最后一行没有回车换行 */
    if (cb != '\n')
    {
        line++;
    }
        
    return line;

}

/*****************************************************************************
 函 数 名  : util_find_str_from_file
 功能描述  : 从文件查找字符串是否存在
 输入参数  : FILE *fp    ---所要查找的文件描述符
             char *szString  ---所要查找的字符串
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年6月27日
    作    者   : zhangjiasi
    修改内容   : 新生成函数

*****************************************************************************/
int util_find_str_from_file( char *szFileName,char *szString  )
{
    FILE *pf = NULL;
    int iStrLen = 0;
    char *buff = NULL;

    if ( NULL ==  szFileName || NULL == szString)
    {
        syslog(3,"%d[util_find_str_from_file]:null pcStr", __LINE__);
        return 1;
    }

    pf = fopen(szFileName,"rb");
    if ( NULL == pf )
    {
        syslog(3,"%d[util_find_str_from_file]:open file:%s failed", __LINE__,szFileName);
        return 1;
    }

    iStrLen = strlen(szString);
    buff = (char *)malloc(10*iStrLen);
    
    while (fread(buff,sizeof(char),10*iStrLen,pf) > 0)
    {
        if (0 != strstr(buff,szString) )
        {
            syslog(5,"[util_find_str_from_file]%d:found string:%s", __LINE__,szString);
            fclose(pf);
            free(buff);
            buff = NULL;
            return 0;
        }
    }
    syslog(3,"[util_find_str_from_file]%d:find string:%s failed", __LINE__,szString);
    fclose(pf);
    free(buff);
    buff = NULL;
    
    return 1;
}

/*****************************************************************************
 函 数 名  : send_signal
 功能描述  : 给一个进程发送信号
 输入参数  : pid_t pid  --进程PID号
             int sig    --将要发送的信号(如USR1)
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月18日
    作    者   : zhanghaozhong
    修改内容   : 新生成函数

*****************************************************************************/
int send_signal(pid_t pid, int sig) 
{    
    if (kill(pid, sig) < 0) 
    {
        return 1; 
    }
    return 0;
}

/*****************************************************************************
 函 数 名  : look_up_ipaddr
 功能描述  : 将字符串（IP地址或者域名）转换为IP地址字符串
 输入参数  : 
            char *pcStr  --传入的IP地址或者域名
            char *pcDesIP --解析后的地址
 输出参数  : 无
 返 回 值  : 成功0，失败返回1
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月28日星期四
    作    者   : 王刚
    修改内容   : 新生成函数

  2.日    期   : 2013年4月22日星期一
    作    者   : 张浩忠
    修改内容   : 修改功能
*****************************************************************************/
int util_look_up_ipaddr(char *pcStr, char *pcDesIP)
{
    struct hostent *netent;
    struct in_addr addr = {0};

    if (NULL == pcStr)
    {
        syslog(3,"%d[look_up_ipaddr]:null pcStr", __LINE__);
        return 1;
    }
    if((netent = gethostbyname(pcStr)))
    {
        /* 将网络地址转换为字符串 */
        addr.s_addr = ntohl( (int)*(int *)*netent->h_addr_list);
        strcpy(pcDesIP, (char *)inet_ntoa(addr));
    }
    else
        return -1;
    return 0;
}

/*****************************************************************************
 函 数 名  : util_my_split
 功能描述  : 分割字符串
 输入参数  : char * str  
             char sym    
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月12日星期五
    作    者   : 王刚
    修改内容   : 新生成函数

*****************************************************************************/
char** util_my_split(char * str, char sym)
{
    const int ini = 10;
    int size = ini;
    char ** str_table=NULL;
    int line = 0;
    if (!str || !*str) return NULL;

	str_table = (char **)malloc(sizeof(char*) * size);
    while (1)
    {
        if (line >= size - 1)
        {
            size *= 2;
            str_table = (char **)realloc(str_table, sizeof(char*) * size);
        }
        str_table[line++] = str;
        while (*str && *str != sym)
        {
            str++;
        }
        if (!*str)
        {
            break;
        }
        else
        {
            *str = '\0';
            while (*++str == sym);
        }
    }
    str_table[line] = 0;
    return str_table;
}

/*****************************************************************************
 函 数 名  : BubbleSort
 功能描述  : 冒泡排序，入参，a[]是数组，n为数组内需要排序元素的个数(n一定要正确)
 输入参数  : int a[]  
             int n    
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年6月6日
    作    者   : zhanghaozhong
    修改内容   : 新生成函数

*****************************************************************************/
void util_bubble_sort(int a[], int n)
{ //R（l..n)是待排序的文件，采用自下向上扫描，对R做冒泡排序
    int i,j,tmp;
    int exchange = 0; //交换标志
    for(i=1; i<n; i++){ //最多做n-1趟排序
        exchange=0; //本趟排序开始前，交换标志应为假
        for(j=n-1; j>=i; j--) //对当前无序区R[i..n]自下向上扫描
            if(a[j] < a[j-1]){//交换记录
              tmp = a[j];//tmp，仅做暂存单元
              a[j] = a[j-1];
              a[j-1] = tmp;
              exchange = 1; //发生了交换，故将交换标志置为真
             }
        if(!exchange) //本趟排序未发生交换，提前终止算法
             return;
    } //endfor(外循环)
} //BubbleSort
/*****************************************************************************
 函 数 名  : util_del_file_lines
 功能描述  : 删除文件中头几行或者尾部几行。
 输入参数  : char *file     文件名
             int iDelHeadL  删除从文件头开始的函数
             int iDelTailL  删除从文件尾倒数的函数
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年6月8日
    作    者   : zhanghaozhong
    修改内容   : 新生成函数

*****************************************************************************/
int util_del_file_lines(char *file, int iDelHeadL, int iDelTailL)
{ 
    FILE *fp = NULL;
    char *acBuff = (char *)malloc(1024 * 150);
    char *acFileStart = NULL;
    char *acFileNewStart = NULL;
    char *acFileNewEnd = NULL;
    int iLen = 0, iLineCount = 0;
    memset(acBuff, 0, 1024*10);
    if (NULL == file)
    {
        syslog(3,"%d[util_del_file_lines]:NULL pcStr", __LINE__);
        free(acBuff);
        return 1;
    }
    fp = fopen(file, "rw+");
	if(NULL == fp)
	{
		syslog(3, "%dcreat failed or open file failed\n", __LINE__);
        free(acBuff);
		return 1;
	}
	
	fseek(fp, 0L, SEEK_END);
    iLen = ftell(fp);
    rewind(fp);
    fread(acBuff, 1, iLen, fp);
    acFileNewStart = acBuff;
    acFileStart = acBuff;
    if (0 != iDelHeadL)
    {
        while(iDelHeadL != iLineCount)
        {
            if ('\n' == *acBuff++)
                iLineCount++;            
        }
        //printf("iDelHeadL iLineCount = %d\n", iLineCount);
        acFileNewStart = acBuff;
    }
    acFileNewEnd = acFileStart + iLen;
    //printf("acFileNewEnd = acFileStart + iLen iLen = %d\n", iLen);
    if (0 != iDelTailL)
    {
        iLineCount = 0;                 /* 计数清零 */
        acBuff = acFileStart + iLen;    /* 将指针移动到文件末尾 */

        while((iDelTailL + 1) != iLineCount)
        {
            if ('\n' == *--acBuff)
                iLineCount++;            
        }
        *++acBuff = '\0';
        acFileNewEnd = acBuff;        
    }
    fclose(fp);
    fp = NULL;
    unlink(file);   //删除此文件
    fp = fopen(file, "w+"); /* 重新生成该文件 */
	if(NULL == fp)
	{
	    free(acBuff);
		syslog(3, "%dcreat failed or open file failed\n", __LINE__);
		return 1;
	}
    fwrite(acFileNewStart, 1, acFileNewEnd - acFileNewStart, fp);
    free(acBuff);
    fclose(fp);
    fp = NULL;
    return 0;
} 

/* 共享内存操作 add begin by zhanghaozhong 2013-7-10*/
#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
//#include <unistd.h>  //getpagesize(  ) 

/*****************************************************************************
 函 数 名  : util_shmem_create
 功能描述  : 创建和获取已创建的共享内存区,iShMemID为某些进程共享一块内存的ID,
                首次创建要填写iSize的大小.
 输入参数  : unsigned int iShMemID  
             unsigned int iSize     
 输出参数  : 无
 返 回 值  : char
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年7月9日
    作    者   : zhanghaozhong
    修改内容   : 新生成函数

*****************************************************************************/
char *util_shmem_create(unsigned int iShMemID, unsigned int iSize)
{
    int segment_id;
    char *acSharedMemory;

    if (iShMemID < 0)
    {
        syslog(3, "shmem ID is NULL");
        return NULL;

    }
        
    /* 绑定到共享内存块 */
    segment_id = shmget(iShMemID, iSize, IPC_CREAT|S_IRUSR|S_IWUSR );
    //printf("getpagesize=%d\n", getpagesize());
    /* 连接到该进程的内存空间 */
    acSharedMemory = (char*)shmat(segment_id, 0, 0);
    if ((char *)-1 == acSharedMemory)   
    {       
        syslog(3, "shmat ERROR");
        return NULL;
    }

    return acSharedMemory;
}
/*****************************************************************************
 函 数 名  : util_shmem_detach
 功能描述  : 脱离已经创建或申请的共享内存区域，入参是util_shmem_create返回的
             指针
 输入参数  : char *acSharedMemory  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年7月9日
    作    者   : zhanghaozhong
    修改内容   : 新生成函数

*****************************************************************************/
int util_shmem_detach(char *acSharedMemory)
{
    int iRet;
    if (NULL == acSharedMemory)
    {
        syslog(3, "acSharedMemory is NULL");
        return -1;

    }
    
    iRet = shmdt(acSharedMemory); /* 脱离该共享内存块 */
    if (-1 == iRet)
    {
        return -1;
    }
    return 0;
}
/*****************************************************************************
 函 数 名  : util_shmem_destroy
 功能描述  : 销毁已经申请的共享内存区域，入参为此共享内存的ID
 输入参数  : unsigned int iShMemID  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年7月9日
    作    者   : zhanghaozhong
    修改内容   : 新生成函数

*****************************************************************************/
int util_shmem_destroy(unsigned int iShMemID)
{
    char* acSharedMemory;
    int segment_id;
    int iRet;

    if (iShMemID < 0)
        return -1;
    /* 获取共享内存ID */
    segment_id = shmget(iShMemID, 0, 0);
    if (-1 == segment_id)
    {
        return -1;
    }
    /* 从系统中删除该共享内存 */
    iRet = shmctl(segment_id, IPC_RMID, 0);
    if (-1 == iRet)
    {
        return -1;
    }
    return 0;
}
/* 共享内存操作 add end by zhanghaozhong 2013-7-10*/

/*****************************************************************************
 函 数 名  : util_read_Ip
 功能描述  : 将ip地址有字符类型转换为int类型
 输入参数  : const char* ip  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年12月6日
    作    者   : zhangjiasi
    修改内容   : 新生成函数

*****************************************************************************/
int util_read_Ip(const char* ip)
{
	int n = 0;
	int res = 0;

	while (n < 4 && *ip) {
		if (isdigit(*ip)) {
			res = (res << 8) | atoi(ip);
			n++;
			while (isdigit(*ip)) {
				ip++;
			}
		} else {
			ip++;
		}
	}
	return res;
}


/*****************************************************************************
 Prototype    : util_get_mac_by_dev
 Description  : Get mac by devname
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/6/17
    Author       : wanggang
    Modification : Created function

*****************************************************************************/
int util_get_mac_by_dev(char *dev, unsigned char *pucMac)
{
    int sock_mac;
    struct ifreq ifr_mac;

    if (NULL == dev)
    {
        return -1;
    }
    /* 创建socket */
    sock_mac = socket(AF_INET, SOCK_STREAM, 0 );
    if( sock_mac == -1)
    {
        return -1;
    }
    /* 获取MAC地址 */
    memset(&ifr_mac, 0 ,sizeof(ifr_mac));
    strncpy(ifr_mac.ifr_name, dev, sizeof(ifr_mac.ifr_name)-1);
    if( (ioctl(sock_mac, SIOCGIFHWADDR, &ifr_mac)) < 0)
    {
		close(sock_mac);
        return -1;
    }
    close(sock_mac);
    memcpy(pucMac, ifr_mac.ifr_hwaddr.sa_data, 6);

    return 0;
}

int util_set_mac(char *pcDev, unsigned char *pucMac)
{
    char acCmd[128] = {0};
    char acTmpMac[32] = {0};
    
    /* 入参 */
    if (NULL == pcDev)
    {
        return -1;
    }
    /* 将网口禁用 */
    sprintf(acCmd, "ifconfig %s down >/dev/null", pcDev);
    util_system_cmd(acCmd);
    /* 修改MAC地址 */
    sprintf(acCmd, "ifconfig %s hw ether %s >/dev/null", pcDev, util_uint8_to_macstr(acTmpMac, pucMac));
    util_system_cmd(acCmd);
    /* 将网口UP */
    sprintf(acCmd, "ifconfig %s up >/dev/null", pcDev);
    util_system_cmd(acCmd);
    
    return 0;
}

