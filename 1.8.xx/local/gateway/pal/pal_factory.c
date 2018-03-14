/******************************************************************************
* COPYRIGHT GBCOM CO.,LTD   
*******************************************************************************
* 功能: 配置和获取NVRAM的配置信息                                                                                                               
* 编制日期: 03/16/2012                                                                                                 
* 修改历史: 徐从江 新建      
* 
******************************************************************************/
#include <stdio.h>   
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include "ap_types.h"
#include "pal_log.h"
#include "pal_factory.h"


static NVRAM_T *gstNVRAM_PTR = NULL;
int gFd = -1;

static int nvrammatch (uchar *, int);
static uchar *nvram_get_addr(int);
int nvram_save_to_staging(void);
static uchar nvram_get_char_memory (int index);
static void nvram_crc_update (void);
int nvram_check_crc(void);
int _do_setnvram (int flag, int argc, char *argv[]);
static int do_nvram_open();
ulong nvram_crc32(const uchar * dataBuf, unsigned int dataLen);

/* Function that returns a character from the NVRAM */
static uchar (*nvram_get_char)(int) = nvram_get_char_memory;

inline int	SYSTEM(char *cmd)
{
	int iRet = 0;
	int iLoop;
    
    if(NULL == cmd)
    {
    	return -1;
    }

    /* execute this cmd */
    for(iLoop = 0; iLoop < 5; iLoop++)
    {
    	iRet = system(cmd);
        if(iRet != 0)
        {
        	NVRAM_DEBUGF("%s fail\n", cmd); 
        	continue;
        }
        break;
    }

	return iRet;
}



/******************************************************************************
* 函数名称: factory_get
* 功    能: 获取NVRAM变量值
* 参    数:
* 参数名称          输入/输出         描述
* name              输入           变量名
* value             输出           存放变量值
* len               输入           value的长度
*
* 函数返回: 
*  成功: 0
*  失败: 1
* 
* 说    明: len的大小一定要为buf的实际长度，如果buf的的实际长度比len的值小的话
*          很可能会导致内存访问越界。
******************************************************************************/
int factory_get(char *value,char *name, int len)

{
    int iRet = 0;
    if (NULL == value || NULL == name)
    {
        return 1;
    }
    memset(value, 0, strlen(value));
    
    if (RET_FAIL == nvram_open())
    {
    	gw_log(LOG_ERR, "nvram oprn  fail!");
        nvram_close();
        return 1;
    }
    if (nvram_check_crc())
    {
    	gw_log(LOG_ERR, "check crc fail!");
        nvram_close();
        printf("nvram's data is destroyed,reboot system after 3s\n");
        system("reboot 3 nvram destroyed");
    }

    iRet = getnvram_r(name, value, len);

    gw_log(LOG_DEBUG,"name:%s, value:%s\n", name, value);

    nvram_close();

    return (iRet == -1) ? 1 : 0;
}

/******************************************************************************
* 函数名称: nvram_set
* 功    能: 设置NVRAM变量值
* 参    数:
* 参数名称          输入/输出         描述
* name              输入           变量名
* value             输入           变量值
*
* 函数返回: 
*  成功: 0
*  失败: 1
* 
* 说    明: 
******************************************************************************/
int factory_set(char *name ,char *value)
{
    int iRet;

    iRet = -1;
    if (NULL == value || NULL == name)
    {
        return 1;
    }
    
    if (RET_FAIL == nvram_open())
    {
        nvram_close();
        return 1;
    }

    if (nvram_check_crc())
    {
        nvram_close(); 
        printf("nvram's data is destroyed,reboot system after 3s\n");
        system("reboot 3 nvram destroyed");
    }

    if (RET_OK == setnvram(name, value))
    {
        iRet = nvram_save_to_staging();
    }

    NVRAM_DEBUGF("value:%s, name:%s\n", name, value);

    nvram_close();
    
    return (RET_OK == iRet) ? 0 : 1;
}
/******************************************************************************
* 函数名称: nvram_unset
* 功    能: 清除NVRAM变量值
* 参    数:
* 参数名称          输入/输出         描述
* name              输入           变量名
* value             输入           变量值
*
* 函数返回: 
*  成功: 0
*  失败: 1
* 
* 说    明: 李文
******************************************************************************/
int factory_unset(char *name)
{
    int iRet;

    iRet = -1;
    if (NULL == name)
    {
        return 1;
    }
    
    if (RET_FAIL == nvram_open())
    {
        nvram_close();
        return 1;
    }

    if (nvram_check_crc())
    {
        nvram_close();    
        printf("nvram's data is destroyed,reboot system after 3s\n");
        system("reboot 3 nvram destroyed");
    }

    if (RET_OK == unsetnvram(name))
    {
        iRet = nvram_save_to_staging();
    }

    NVRAM_DEBUGF("name:%s\n", name);

    nvram_close();
    
    return (RET_OK == iRet) ? 0 : 1;
}

/******************************************************************************
* 函数名称: nvram_commit
* 功    能: 保存nvram配置至flash中
* 参    数:
* 参数名称          输入/输出         描述
* name              输入           变量名
* value             输出           存放变量值
* len               输入           value的长度
*
* 函数返回: 
*  成功: 0
*  失败: 1
* 
* 说    明: 
******************************************************************************/
int factory_commit(void)

{
    int iRet;

    iRet = -1;
    if (RET_FAIL == nvram_open())
    {
        nvram_close();
        return 1;
    }

    if (nvram_check_crc())
    {
        nvram_close();
        printf("nvram's data is destroyed,reboot system after 3s\n");
        system("reboot 3 nvram destroyed");
    }

    iRet = do_nvram_commit();

    NVRAM_DEBUGF("save config to flash...\n");

    nvram_close();
    
    return (iRet == RET_OK) ? 0 : 1;
}

/******************************************************************************
* 函数名称: nvram_set_and_commit
* 功    能: 设置NVRAM变量值并保存至flash中
* 参    数:
* 参数名称          输入/输出         描述
* name              输入           变量名
* value             输入           变量值
*
* 函数返回: 
*  成功: 0
*  失败: 1
* 
* 说    明: 
******************************************************************************/
int factory_set_and_commit(char *name ,char *value)
{
    int iRet;

    iRet = -1;
    if (NULL == value || NULL == name)
    {
        return 1;
    }
    
    if (RET_FAIL == nvram_open())
    {
        nvram_close();
        return 1;
    }

    if (nvram_check_crc())
    {
        nvram_close();
        printf("nvram's data is destroyed,reboot system after 3s\n");
        system("reboot 3 nvram destroyed");
    }

    if (RET_OK == setnvram(name, value))
    {
        iRet = nvram_save_to_staging();
    }

    NVRAM_DEBUGF("value:%s, name:%s\n", name, value);

    if (RET_OK == iRet)
    {
        iRet = do_nvram_commit();
        NVRAM_DEBUGF("save config to flash...\n");
    }
    
    nvram_close();
    
    return (RET_OK == iRet) ? 0 : 1;
}




/******************************************************************************
* 函数名称: setnvram
* 功    能: 设置NVRAM变量
* 参    数:
* 参数名称          输入/输出         描述
* varname              输入           变量名
* varvalue             输入           变量值
*
* 函数返回: 
*  0 成功
*  -1 失败
* 
* 说    明: varname和varvalue的最大长度分别为128和256
******************************************************************************/
int setnvram (char *varname, char *varvalue)
{
	char *argv[4] = { "setenv", varname, varvalue, NULL };

	if ( (strlen(varname) > NVRAM_NAME_MAX_LEN) ||
	    (strlen(varvalue) > NVRAM_VALUE_MAX_LEN))
	{
        printf("Warning: The lenth of name or value too long!\n");
        return RET_FAIL;
	}
	return  _do_setnvram (0, 3, argv);
}

/******************************************************************************
* 函数名称: unsetnvram
* 功    能: 删除NVRAM变量
* 参    数:
* 参数名称          输入/输出         描述
* varname              输入           变量名
*
* 函数返回: 
*  无。
* 
* 说    明: 
******************************************************************************/
int unsetnvram (char *varname)
{
	char *argv[3] = { "setenv", varname, NULL };
	return _do_setnvram (0, 2, argv);
}

/******************************************************************************
* 函数名称: getnvram_r
* 功    能: 获取NVRAM变量值
* 参    数:
* 参数名称          输入/输出         描述
* varname              输入           变量名
* buf                  输出           存放变量值
* len                  输入           buf的长度   最小值为257
*
* 函数返回: 
*  成功返回变量值的长度
*  失败返回-1
* 
* 说    明: len的大小一定要为buf的实际长度，如果buf的的实际长度比len的值小的话
*          很可能会导致内存访问越界。所以这里强制buf的大小至少为257
******************************************************************************/
int getnvram_r (const char *name, char *buf, unsigned len)
{
	int i, nxt;

    /* 
     * 防止buf的的实际长度比len的值小，导致内存越界
    */
	if (len > NVRAM_VALUE_MAX_LEN + 1)
	{
        return -1;
	}
	for (i=0; nvram_get_char(i) != '\0'; i=nxt+1) 
	{
		int val, n;

		for (nxt=i; nvram_get_char(nxt) != '\0'; ++nxt) 
		{
			if (nxt >= NVRAM_SECTOR_SIZE) 
			{
				return (-1);
			}
		}
		if ((val=nvrammatch((uchar *)name, i)) < 0)
		{
			continue;
		}
		/* found; copy out */
		n = 0;
		while ((len > n++) && (*buf++ = nvram_get_char(val++)) != '\0')
		{

		}
		if (len == n)
		{
			*buf = '\0';
		}
		return (n);
	}
	return (-1);
}

static int find_staging_file(void)
{
    int iRet;
    iRet = access(NVRAM_STAGING, 0);
    return (0 == iRet) ? RET_OK : RET_FAIL;
}

/* Copy NVRAM contents to staging file. NVRAM-->/tmp/.nvram */
static int nvram_to_staging(void)
{
    int iRet;
    iRet = SYSTEM("cat "NVRAM_FLASH_MTD" > "NVRAM_STAGING);
    return (0 == iRet) ? RET_OK : RET_FAIL;
}

int nvram_open()
{
    if ( RET_OK == find_staging_file() || RET_OK == nvram_to_staging() )
    {
        NVRAM_DEBUGF("\n");
        return do_nvram_open();
    }

    return RET_FAIL;
}
static int do_nvram_open()
{
    int readBytes = 0;


    gstNVRAM_PTR = (NVRAM_T *)malloc(NVRAM_SECTOR_SIZE);
    if (NULL == gstNVRAM_PTR)
    {
        printf("%s malloc failed!\n", __FUNCTION__);
        return RET_FAIL;
    }

    NVRAM_DEBUGF("malloc at %p\n", (ulong)gstNVRAM_PTR);
    memset((char *)gstNVRAM_PTR, 0, NVRAM_SECTOR_SIZE);
 
    //read from staging file	
    if ((gFd = open(NVRAM_STAGING, O_RDWR)) < 0)
    {
        printf("%s open "NVRAM_STAGING"failed \n", __FUNCTION__);
        return RET_FAIL;
    }
    
    flock(gFd, LOCK_EX);  /* lock */
    readBytes = read(gFd, (char *)gstNVRAM_PTR, NVRAM_SECTOR_SIZE);
    
    if (readBytes != NVRAM_SECTOR_SIZE)
    {
        printf("%s read "NVRAM_STAGING" failed \n", __FUNCTION__);
        printf("nvram's data is destroyed,reboot system after 3s\n");
        system("reboot 3 nvram destroyed");
        return RET_FAIL;
    }
    return RET_OK;
}

void nvram_close(void)
{
    if (gstNVRAM_PTR)
    {
        free(gstNVRAM_PTR);
    }

    if (gFd > -1)
    {
        flock(gFd, LOCK_UN);  /* unlock */
        close(gFd);
    }
}

/* ========================================================================= */
#define DO1(buf) ulCrc = aulCrcTable[((int)ulCrc ^ (*buf++)) & 0xff] ^ (ulCrc >> 8);
#define DO2(buf)  DO1(buf); DO1(buf);
#define DO4(buf)  DO2(buf); DO2(buf);
#define DO8(buf)  DO4(buf); DO4(buf);
/* ========================================================================= */
ulong nvram_crc32(const uchar * dataBuf, unsigned int dataLen)
{
    unsigned long ulCrc = 0;
    ulCrc = ulCrc ^ 0xffffffffL;
    while (dataLen >= 8)
    {
        DO8(dataBuf);
        dataLen -= 8;
    }
    if (dataLen)
    {
        do 
        {
            DO1(dataBuf);
        } while (--dataLen);
    }
    return ulCrc ^ 0xffffffffL;
}

/************************************************************************
 * Command interface: print one or all environment variables
 */
int do_printnvram (int argc, char *argv[])
{
	int i, j, k, nxt;
	int rcode = 0;

	if (argc == 1) {		/* Print all env variables	*/
		for (i=0; nvram_get_char(i) != '\0'; i=nxt+1) {
			for (nxt=i; nvram_get_char(nxt) != '\0'; ++nxt)
				;
			for (k=i; k<nxt; ++k)
				putchar(nvram_get_char(k));
			putchar  ('\n');
		}

		printf("NVRAM size: %d/%d bytes\n", i+2, NVRAM_DATA_SIZE);

		return 0;
	}

	for (i=1; i<argc; ++i) {	/* print single env variables	*/
		char *name = argv[i];

		k = -1;

		for (j=0; nvram_get_char(j) != '\0'; j=nxt+1) {

			for (nxt=j; nvram_get_char(nxt) != '\0'; ++nxt)
				;
			k = nvrammatch((uchar *)name, j);
			if (k < 0) {
				continue;
			}
			puts (name);
			putchar ('=');
			while (k < nxt)
				putchar(nvram_get_char(k++));
			putchar ('\n');
			break;
		}
		if (k < 0) {
			printf ("## Error: \"%s\" not defined\n", name);
			rcode ++;
		}
	}
	return rcode;
}

void printnvram(void)
{
    do_printnvram(1, NULL);
}


/************************************************************************
 * Look up variable from environment,
 * return address of storage for that variable,
 * or NULL if not found
 */

char *getnvram (char *name)
{
	int i, nxt;

	for (i=0; nvram_get_char(i) != '\0'; i=nxt+1) {
		int val;

		for (nxt=i; nvram_get_char(nxt) != '\0'; ++nxt) {
			if (nxt >= NVRAM_SECTOR_SIZE) {
				return (NULL);
			}
		}
		if ((val=nvrammatch((uchar *)name, i)) < 0)
			continue;
		return ((char *)nvram_get_addr(val));
	}

	return (NULL);
}


/************************************************************************
 * Set a new nvram variable,
 * or replace or delete an existing one.
 */
int _do_setnvram (int flag, int argc, char *argv[])
{
	int   i, len, oldval;
	uchar *env, *nxt = NULL;
	char *name;

	uchar *env_data = nvram_get_addr(0);

	if (!env_data)
		return RET_FAIL;

	name = argv[1];

	/*
	 * search if variable with this name already exists
	 */
	oldval = -1;
	for (env=env_data; *env; env=nxt+1) {
		for (nxt=env; *nxt; ++nxt)
			;
		if ((oldval = nvrammatch((uchar *)name, env-env_data)) >= 0)
			break;
	}

	/*
	 * Delete any existing definition
	 */
	if (oldval >= 0) {

		if (*++nxt == '\0') {
			if (env > env_data) {
				env--;
			} else {
				*env = '\0';
			}
		} else {
			for (;;) {
				*env = *nxt++;
				if ((*env == '\0') && (*nxt == '\0'))
					break;
				++env;
			}
		}
		*++env = '\0';
	}

	/* Delete only ? */
	if ((argc < 3) || argv[2] == NULL) {
		nvram_crc_update ();
		return RET_OK;
	}

	/*
	 * Append new definition at the end
	 */
	for (env=env_data; *env || *(env+1); ++env)
		;
	if (env > env_data)
		++env;
	/*
	 * Overflow when:
	 * "name" + "=" + "val" +"\0\0"  > NVRAM_DATA_SIZE - (env-env_data)
	 */
	len = strlen(name) + 2;
	/* add '=' for first arg, ' ' for all others */
	for (i=2; i<argc; ++i) {
		len += strlen(argv[i]) + 1;
	}
	if (len > (&env_data[NVRAM_DATA_SIZE - 1]-env)) {
		printf ("## Error: nvram overflow, \"%s\" deleted\n", name);
		return RET_FAIL;
	}
	while ((*env = *name++) != '\0')
		env++;
	for (i=2; i<argc; ++i) {
		char *val = argv[i];

		*env = (i==2) ? '=' : ' ';
		while ((*++env = *val++) != '\0')
			;
	}

	/* end is marked with double '\0' */
	*++env = '\0';

	/* Update CRC */
	nvram_crc_update ();

	return RET_OK;
}

int nvram_save_to_staging(void)
{
    int wt_b;
    int i;
    ulong seek_b;

    if (NVRAM_SECTOR_SIZE == (seek_b = lseek(gFd, 0, SEEK_SET)))
    {
        printf("lseek error: offset=%lu\n", seek_b);
        return RET_FAIL;
    }
    wt_b = write(gFd, (char *)gstNVRAM_PTR, NVRAM_SECTOR_SIZE);
    
    if (wt_b == NVRAM_SECTOR_SIZE)
    {
        return RET_OK;
    }
    else
    {
        perror("write data to staging file failed\n");
	    return RET_FAIL;
    }
}

/* save nvram config to flash */
int do_nvram_commit(void)
{
    int fd;

    if ((fd = open(NVRAM_FLASH_MTD, O_WRONLY)) < 0)
    {
        perror("Open "NVRAM_FLASH_MTD"error\n");
        return RET_FAIL;
    }
    
    if (write(fd, (char *)gstNVRAM_PTR, NVRAM_SECTOR_SIZE) == NVRAM_SECTOR_SIZE)
    {
        close(fd);
        return RET_OK;
    }
    else
    {
        close(fd);
        printf("write data to "NVRAM_FLASH_MTD" failed\n");
	    return RET_FAIL;
    }
}

/************************************************************************
 * Match a name / name=value pair
 *
 * s1 is either a simple 'name', or a 'name=value' pair.
 * i2 is the environment index for a 'name2=value2' pair.
 * If the names match, return the index for the value2, else NULL.
 */

static int nvrammatch (uchar *s1, int i2)
{

	while (*s1 == nvram_get_char(i2++))
		if (*s1++ == '=')
			return(i2);
	if (*s1 == '\0' && nvram_get_char(i2-1) == '=')
		return(i2);
	return(-1);
}

static uchar *nvram_get_addr (int index)
{
	return ((uchar *)(gstNVRAM_PTR->data + index));
}

static uchar nvram_get_char_memory (int index)
{
	return ( *((uchar *)(gstNVRAM_PTR->data + index)) );
}

static void nvram_crc_update (void)
{
	gstNVRAM_PTR->crc = nvram_crc32(gstNVRAM_PTR->data, NVRAM_DATA_SIZE);
}

/* 检查CRC，正确返回0；错误返回1 */
int nvram_check_crc(void)
{
    return !(nvram_crc32(gstNVRAM_PTR->data, NVRAM_DATA_SIZE) == gstNVRAM_PTR->crc);
}



int  nvram_init(void)
{
    int fd;

    if ((fd = open(NVRAM_FLASH_MTD, O_WRONLY)) < 0)
    {
        perror("Open "NVRAM_FLASH_MTD"error\n");
        return RET_FAIL;
    }
    memset(gstNVRAM_PTR,0,NVRAM_SECTOR_SIZE);
    nvram_crc_update();
    if (write(fd, (char *)gstNVRAM_PTR, NVRAM_SECTOR_SIZE) == NVRAM_SECTOR_SIZE)
    {
        close(fd);
        return RET_OK;
    }
    else
    {
        close(fd);
        printf("write data to "NVRAM_FLASH_MTD" failed\n");
	    return RET_FAIL;
    }
}


