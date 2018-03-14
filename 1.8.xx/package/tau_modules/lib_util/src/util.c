/******************************************************************************

                  ��Ȩ���� (C), 2010-2012, �Ϻ�徴�ͨ�ſƼ����޹�˾

 ******************************************************************************
  �� �� ��   : util.c
  �� �� ��   : ����
  ��    ��   : zhanghaozhong
  ��������   : 2013��3��13��
  ����޸�   :
  ��������   : ͨ�ú�����
  �����б�   :
              util_system_cmd
              util_lock_file
              util_unlock_file
              util_itoa
              util_find_pid_by_name
  �޸���ʷ   :
  1.��    ��   : 2013��3��13��
    ��    ��   : zhanghaozhong
    �޸�����   : �����ļ�

******************************************************************************/
/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
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
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/
typedef struct {
    int pid;
    /* basename of executable file in call to exec(2),
        size from kernel headers */
    char short_cmd[16];
} procps_status_t;

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
char * util_uint8_to_macstr(char *pcMacstr, unsigned char *pucMac )
{
    sprintf(pcMacstr, "%02x:%02x:%02x:%02x:%02x:%02x",pucMac[0],pucMac[1],pucMac[2],pucMac[3],pucMac[4],pucMac[5]);

    return pcMacstr;
}


/*****************************************************************************
 �� �� ��  : util_system_cmd
 ��������  : ϵͳ����
 �������  : CHAR *pcCmd
 �������  : ��
 �� �� ֵ  : INT32
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��3��13��������
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
int util_system_cmd( char *szCmd )
{
    int iRet;
    int iLoop;

    if  ( NULL == szCmd )
    {
        return 1;
    }

    /* �˴�Ϊʹutil������ֱ�Ӳ���syslog��ӡ��Ϣ */
    for  ( iLoop = 0 ; iLoop < 5 ; iLoop++ )
    {
         iRet = system(szCmd);
         if  ( -1 != iRet && WIFEXITED(iRet) && 0 == WEXITSTATUS(iRet))
         {
            /* 7 ��ʾ��־����ΪDEBUG���� */
             //syslog(7, "[util_system_cmd] Success: %s\n", szCmd);
             return 0;
         }
    }
    /* 3 ��ʾ��־����ΪERR���� */
    syslog(3, "[util_system_cmd] Failed: %s,status = %d\n", szCmd, iRet);

    return iRet;
}

/*****************************************************************************
 �� �� ��  : util_popen_cmd
 ��������  : popenִ������
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��1��4��
    ��    ��   : zhangjiasi
    �޸�����   : �����ɺ���

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
 �� �� ��  : util_lock_file
 ��������  : ���ļ�����
 �������  : struct flock stLock  
             int iComfd           
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��3��13��
    ��    ��   : zhanghaozhong
    �޸�����   : �����ɺ���

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
 �� �� ��  : util_unlock_file
 ��������  : ���ļ�����
 �������  : struct flock stLock  
             int iComfd           
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��3��13��
    ��    ��   : zhanghaozhong
    �޸�����   : �����ɺ���

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
 �� �� ��  : itoa
 ��������  : ������ת��Ϊ�ַ���
 �������  : int value  
             char *str  
             int radix  
 �������  : ��
 �� �� ֵ  : char
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��3��13��������
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
char *util_itoa(int value, char *str, int radix)
{	
	//�ַ�ӳ��� 
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
	//����radix������2��36֮��  
	if (radix >= 2 && radix <= 36)   
	{
		//����Ǹ���������λ��Ӹ��ţ������ַ���ǰ�� 
		if (value < 0 && radix == 10)
		{
			*pStr++ = '-';
			value = -value;//תΪ������
		}
		nValue = *(unsigned*)&value;

		//ѭ��ת��ÿһ�����֣�ֱ������
		do {
			pStr[++nCount] = szMap[nValue % radix];
			nValue /= radix;
		}
		while (nValue > 0);//ת���������ַ����Ƿ���

		//�����һ��ĳ���
		nIndex = (nCount + 1)/2;

		//���ַ������ַ���ת
		while (nIndex-- > 0)
		{
			nTemp = pStr[nIndex];
			pStr[nIndex] = pStr[nCount - nIndex];
			pStr[nCount - nIndex] = nTemp;
		}
	}

	//�ý�����
	pStr[nCount + 1] = '\0';

	return str;
}
/*****************************************************************************
 �� �� ��  : atou8
 ��������  : atou8
 �������  : uchar * string  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��3��25������һ
    ��    ��   : liyoukun
    �޸�����   : �����ɺ���

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
 �� �� ��  : atou8x
 ��������  : strת��Ϊ16������
 �������  : char *string
 �������  : ��
 �� �� ֵ  : unsigned char
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��3��29��������
    ��    ��   : liyoukun
    �޸�����   : �����ɺ���

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
            res += *string - 'a' + 10;      /* a-f��10������Ϊ10-15 */
        }
        else if(*string >= 'A' && *string <= 'F')
        {
            res += *string - 'A' + 10;      /* a-f��10������Ϊ10-15 */
        }
        string++;
	}
    
	return res;
}

/*****************************************************************************
 �� �� ��  : util_find_pid_by_name
 ��������  : �������ƻ�ȡ�����̺�
 �������  : char *pcPidName  
             int *iPid        
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��3��13��������
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
    
    /*��Ÿ���Ŀ¼����*/
	char acDirName[32] = {0};
    
    /*��proc�ļ�ϵͳ*/
	if (!pstDir) 
    {
		pstDir = opendir("/proc");
		if (!pstDir)
        {      
            syslog(3, "Can't open /proc\n");
            return -2;
        }
	}
    /*һ�ζ�ȡproc�ļ�ϵͳ�µ������ļ���*/
    while ( 1 ) 
    {
        memset(acBuf,0,sizeof(acBuf));
        memset(acDirName,0,sizeof(acDirName));
        memset(stCurstatus.short_cmd,0,sizeof(stCurstatus.short_cmd));
        
        /*��ȡ����dirĿ¼�����¸�Ŀ¼����㣬����û���ҵ����˳�*/
    	if ((pstEntry = readdir(pstDir)) == NULL)
        {
    		closedir(pstDir);
    		pstDir = NULL;
    		return -1;
    	}
        
        /*����:�ļ���*/
    	pcName = pstEntry->d_name;
        /*��proc�ļ�ϵͳ��ÿ�����̶�������̺Ŷ�Ӧ���ļ��У����ļ���������pid��*/
    	if (!(*pcName >= '0' && *pcName <= '9'))
        {
    		continue;
        }   

    	memset(&stCurstatus, 0, sizeof(procps_status_t));
    	iPid = atoi(pcName);
    	stCurstatus.pid = iPid;
    	sprintf(acDirName, "/proc/%d", iPid);
        
        /*��ȡ�ļ��е�״̬��ȷ���ļ��Ĵ�����*/
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

        /*��ȡ�ļ�����*/
    	pcName = fgets(acBuf, sizeof(acBuf), pstFp);
        
        /*�ر��ļ�*/
    	fclose(pstFp);
        pstFp = NULL;
        
        acBuf[511] = '\0';
        
    	if (pcName == NULL)
        {
    		continue;
        }   

        /*�ҵ�����ַ����ֵ�����ַ�������䷵��*/
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
            /*���ļ���Ŀ¼������*/
            while((pstEntry = readdir(pstDir)) != NULL)
            {
                /*��Ŀ¼������*/;
            }
            /*���պ�ر�*/   
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
 �� �� ��  : util_ip_to_str
 ��������  : ��������ʽ��IPת��Ϊ�ַ�����ʽ��IP��ַ
 �������  : unsigned char *ucIp
             char *pcIpAddr
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��3��14��������
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
int util_ip_to_str( unsigned char *pucIp, char *pcIpAddr )
{
    
    /* ��μ�� */
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
 �� �� ��  : util_str_to_ip
 ��������  : ���ַ�����ʽ��IP��ַת��Ϊ����
 �������  : char *pcIpAddr
             unsigned char *pucIp
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��3��14��������
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
int util_str_to_ip( char *pcIpAddr, unsigned char *pucIp )
{
    int iTmp[4];
    
     /* ��μ�� */
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
 �� �� ��  : util_str_to_number
 ��������  : ���ض���ʽ���ַ���ת��Ϊ����
 �������  : char *acScrStr
             int *iCount
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��3��15��������
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
int util_str_to_number( char *pcScrStr, int *piDesInt ,char cFlag)
{
    char *pcBuff = NULL;
    char buff[128 + 1] = {0};
    int iLoop = 0;
    int iCount = 0;
    int len = 0;

    /* ��μ�� */
    if (NULL == pcScrStr)
    {
        /* ��ӡ������־ */
        syslog(6, "pcScrStr is NULL");
        return 1;
    }

    if (NULL == piDesInt)
    {
        syslog(6, "piDesInt is NULL");
        return 1;
    }

    /* ����cFlag�ָ����ַ���ת��Ϊint���� */
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
 �� �� ��  : util_str_to_uint8
 ��������  : ���ض��ַ���ת��ΪUINT8������
 �������  : ��
 �������  : ��
 �� �� ֵ  : int �����С
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��3��25������һ
    ��    ��   : liyoukun
    �޸�����   : �����ɺ���

*****************************************************************************/
int util_str_to_uint8( char *pcScrStr, UINT8 *piDesInt ,char cFlag )
{
    char *pcBuff = NULL;
    char buff[128 + 1] = {0};
    int iLoop = 0;
    int iCount = 0;
    int len = 0;

    /* ��μ�� */
    if (NULL == pcScrStr)
    {
        /* ��ӡ������־ */
        syslog(6, "pcScrStr is NULL");
        return -1;
    }
    if (NULL == piDesInt)
    {
        syslog(6, "piDesInt is NULL");
        return -1;
    }

    /* ����cFlag�ָ����ַ���ת��ΪUINT8���� */
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
 �� �� ��  : util_macstr_to_uint8
 ��������  : ��MAC��ַת��Ϊuint8������
 �������  : char *pcScrStr, 
             UINT8 *piDesInt ,
             char cFlag
 �������  : ��
 �� �� ֵ  : unsigned char
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��3��29��������
    ��    ��   : liyoukun
    �޸�����   : �����ɺ���

*****************************************************************************/
unsigned char util_macstr_to_uint8( char *pcScrStr, UINT8 *piDesInt ,char cFlag )
{
    char *pcBuff = NULL;
    char buff[128 + 1] = {0};
    int iLoop = 0;
    int iCount = 0;
    int len = 0;

    /* ��μ�� */
    if (NULL == pcScrStr)
    {
        /* ��ӡ������־ */
        syslog(6, "pcScrStr is NULL");
        return 1;
    }
    if (NULL == piDesInt)
    {
        syslog(6, "piDesInt is NULL");
        return 1;
    }

    /* ����cFlag�ָ����ַ���ת��ΪUINT8���� */
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
 �� �� ��  : util_mac_to_str
 ��������  : ��uint8��MAC��ַת��Ϊ�ַ���
 �������  : unsigned char *pucMac
             char *pcMacStr
 �������  : ��
 �� �� ֵ  : char *
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��6��9��������
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : util_array_string_to_int
 ��������  : ���ַ�������ת����int��
             CHAR *src ---Դ�ַ���
             CHAR *dst ---���ִ�ŵ�ַ
             UINT32 *total  ---ת������������
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2014��8��6��
    ��    ��   : zhangjiasi
    �޸�����   : �����ɺ���

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
 �� �� ��  : util_array_add_someone
 ��������  : ���ĳ���ַ�
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2014��8��6��
    ��    ��   : zhangjiasi
    �޸�����   : �����ɺ���

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
 �� �� ��  : util_array_del_someone
 ��������  : ɾ��ĳ���ַ�
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2014��8��6��
    ��    ��   : zhangjiasi
    �޸�����   : �����ɺ���

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
 �� �� ��  : util_get_file_lines
 ��������  : ��ȡһ��������������
 �������  : file *fp
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��3��30��������
    ��    ��   : liyoukun
    �޸�����   : �����ɺ���

*****************************************************************************/
int util_get_file_lines( FILE *fp )
{

    int line=0;
    char c, cb='\0';  //cb�洢��ǰ�����ַ���ǰһ���ַ�
    /* ��μ�� */
    if (NULL == fp)
    {
        /* ��ӡ������־ */
        syslog(6, "fp is NULL");
        return 1;
    }
    while((c=getc(fp))!=EOF)
    {
        if(c == '\n')
        line++;
        cb = c;
    }
    /* ��ֹ���һ��û�лس����� */
    if (cb != '\n')
    {
        line++;
    }
        
    return line;

}

/*****************************************************************************
 �� �� ��  : util_find_str_from_file
 ��������  : ���ļ������ַ����Ƿ����
 �������  : FILE *fp    ---��Ҫ���ҵ��ļ�������
             char *szString  ---��Ҫ���ҵ��ַ���
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2014��6��27��
    ��    ��   : zhangjiasi
    �޸�����   : �����ɺ���

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
 �� �� ��  : send_signal
 ��������  : ��һ�����̷����ź�
 �������  : pid_t pid  --����PID��
             int sig    --��Ҫ���͵��ź�(��USR1)
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��3��18��
    ��    ��   : zhanghaozhong
    �޸�����   : �����ɺ���

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
 �� �� ��  : look_up_ipaddr
 ��������  : ���ַ�����IP��ַ����������ת��ΪIP��ַ�ַ���
 �������  : 
            char *pcStr  --�����IP��ַ��������
            char *pcDesIP --������ĵ�ַ
 �������  : ��
 �� �� ֵ  : �ɹ�0��ʧ�ܷ���1
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��3��28��������
    ��    ��   : ����
    �޸�����   : �����ɺ���

  2.��    ��   : 2013��4��22������һ
    ��    ��   : �ź���
    �޸�����   : �޸Ĺ���
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
        /* �������ַת��Ϊ�ַ��� */
        addr.s_addr = ntohl( (int)*(int *)*netent->h_addr_list);
        strcpy(pcDesIP, (char *)inet_ntoa(addr));
    }
    else
        return -1;
    return 0;
}

/*****************************************************************************
 �� �� ��  : util_my_split
 ��������  : �ָ��ַ���
 �������  : char * str  
             char sym    
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��4��12��������
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : BubbleSort
 ��������  : ð��������Σ�a[]�����飬nΪ��������Ҫ����Ԫ�صĸ���(nһ��Ҫ��ȷ)
 �������  : int a[]  
             int n    
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��6��6��
    ��    ��   : zhanghaozhong
    �޸�����   : �����ɺ���

*****************************************************************************/
void util_bubble_sort(int a[], int n)
{ //R��l..n)�Ǵ�������ļ���������������ɨ�裬��R��ð������
    int i,j,tmp;
    int exchange = 0; //������־
    for(i=1; i<n; i++){ //�����n-1������
        exchange=0; //��������ʼǰ��������־ӦΪ��
        for(j=n-1; j>=i; j--) //�Ե�ǰ������R[i..n]��������ɨ��
            if(a[j] < a[j-1]){//������¼
              tmp = a[j];//tmp�������ݴ浥Ԫ
              a[j] = a[j-1];
              a[j-1] = tmp;
              exchange = 1; //�����˽������ʽ�������־��Ϊ��
             }
        if(!exchange) //��������δ������������ǰ��ֹ�㷨
             return;
    } //endfor(��ѭ��)
} //BubbleSort
/*****************************************************************************
 �� �� ��  : util_del_file_lines
 ��������  : ɾ���ļ���ͷ���л���β�����С�
 �������  : char *file     �ļ���
             int iDelHeadL  ɾ�����ļ�ͷ��ʼ�ĺ���
             int iDelTailL  ɾ�����ļ�β�����ĺ���
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��6��8��
    ��    ��   : zhanghaozhong
    �޸�����   : �����ɺ���

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
        iLineCount = 0;                 /* �������� */
        acBuff = acFileStart + iLen;    /* ��ָ���ƶ����ļ�ĩβ */

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
    unlink(file);   //ɾ�����ļ�
    fp = fopen(file, "w+"); /* �������ɸ��ļ� */
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

/* �����ڴ���� add begin by zhanghaozhong 2013-7-10*/
#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
//#include <unistd.h>  //getpagesize(  ) 

/*****************************************************************************
 �� �� ��  : util_shmem_create
 ��������  : �����ͻ�ȡ�Ѵ����Ĺ����ڴ���,iShMemIDΪĳЩ���̹���һ���ڴ��ID,
                �״δ���Ҫ��дiSize�Ĵ�С.
 �������  : unsigned int iShMemID  
             unsigned int iSize     
 �������  : ��
 �� �� ֵ  : char
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��7��9��
    ��    ��   : zhanghaozhong
    �޸�����   : �����ɺ���

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
        
    /* �󶨵������ڴ�� */
    segment_id = shmget(iShMemID, iSize, IPC_CREAT|S_IRUSR|S_IWUSR );
    //printf("getpagesize=%d\n", getpagesize());
    /* ���ӵ��ý��̵��ڴ�ռ� */
    acSharedMemory = (char*)shmat(segment_id, 0, 0);
    if ((char *)-1 == acSharedMemory)   
    {       
        syslog(3, "shmat ERROR");
        return NULL;
    }

    return acSharedMemory;
}
/*****************************************************************************
 �� �� ��  : util_shmem_detach
 ��������  : �����Ѿ�����������Ĺ����ڴ����������util_shmem_create���ص�
             ָ��
 �������  : char *acSharedMemory  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��7��9��
    ��    ��   : zhanghaozhong
    �޸�����   : �����ɺ���

*****************************************************************************/
int util_shmem_detach(char *acSharedMemory)
{
    int iRet;
    if (NULL == acSharedMemory)
    {
        syslog(3, "acSharedMemory is NULL");
        return -1;

    }
    
    iRet = shmdt(acSharedMemory); /* ����ù����ڴ�� */
    if (-1 == iRet)
    {
        return -1;
    }
    return 0;
}
/*****************************************************************************
 �� �� ��  : util_shmem_destroy
 ��������  : �����Ѿ�����Ĺ����ڴ��������Ϊ�˹����ڴ��ID
 �������  : unsigned int iShMemID  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��7��9��
    ��    ��   : zhanghaozhong
    �޸�����   : �����ɺ���

*****************************************************************************/
int util_shmem_destroy(unsigned int iShMemID)
{
    char* acSharedMemory;
    int segment_id;
    int iRet;

    if (iShMemID < 0)
        return -1;
    /* ��ȡ�����ڴ�ID */
    segment_id = shmget(iShMemID, 0, 0);
    if (-1 == segment_id)
    {
        return -1;
    }
    /* ��ϵͳ��ɾ���ù����ڴ� */
    iRet = shmctl(segment_id, IPC_RMID, 0);
    if (-1 == iRet)
    {
        return -1;
    }
    return 0;
}
/* �����ڴ���� add end by zhanghaozhong 2013-7-10*/

/*****************************************************************************
 �� �� ��  : util_read_Ip
 ��������  : ��ip��ַ���ַ�����ת��Ϊint����
 �������  : const char* ip  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��12��6��
    ��    ��   : zhangjiasi
    �޸�����   : �����ɺ���

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
    /* ����socket */
    sock_mac = socket(AF_INET, SOCK_STREAM, 0 );
    if( sock_mac == -1)
    {
        return -1;
    }
    /* ��ȡMAC��ַ */
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
    
    /* ��� */
    if (NULL == pcDev)
    {
        return -1;
    }
    /* �����ڽ��� */
    sprintf(acCmd, "ifconfig %s down >/dev/null", pcDev);
    util_system_cmd(acCmd);
    /* �޸�MAC��ַ */
    sprintf(acCmd, "ifconfig %s hw ether %s >/dev/null", pcDev, util_uint8_to_macstr(acTmpMac, pucMac));
    util_system_cmd(acCmd);
    /* ������UP */
    sprintf(acCmd, "ifconfig %s up >/dev/null", pcDev);
    util_system_cmd(acCmd);
    
    return 0;
}

