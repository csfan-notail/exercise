
#include <malloc.h>
#include <watchdog.h>
#include "custom_internal.h"

/*
dev:    size   erasesize  name
mtd0: 00040000 00010000 "uboot"
mtd1: 00010000 00010000 "uboot.env"
mtd2: 00170000 00010000 "kernel"
mtd3: 005f0000 00010000 "rootfs"
mtd4: 005f0000 00010000 "rootfs.bak"
mtd5: 001f0000 00010000 "rootfs_data"
mtd6: 00010000 00010000 "nvram"
mtd7: 00010000 00010000 "nvram.bak"
mtd8: 00020000 00010000 "factory.data"
mtd9: 00010000 00010000 "factory.nvram"
mtd10: 00010000 00010000 "factory.nvram.bak"
mtd11: 00010000 00010000 "art"
mtd12: 00760000 00010000 "firmware"
*/

#define NVRAM_DATA_SIZE (0x10000 - 4)
#define NVRAM_SECTOR_SIZE 0x10000

#define FACTORY_NVRAM_OFFSET ((FLASH_SIZE << 20) - ART_SECTOR_SIZE - 2 * NVRAM_SECTOR_SIZE)
#define FACTORY_NVRAM_ADDR (CFG_FLASH_BASE + FACTORY_NVRAM_OFFSET)
#define FACTORY_NVRAM_BAK_ADDR (FACTORY_NVRAM_ADDR + NVRAM_SECTOR_SIZE)

#define NVRAM_OFFSET (FACTORY_NVRAM_OFFSET - FACTORY_DATA_SIZE - 2 * NVRAM_SECTOR_SIZE)
#define NVRAM_ADDR (CFG_FLASH_BASE + NVRAM_OFFSET)
#define NVRAM_BAK_ADDR (NVRAM_ADDR + NVRAM_SECTOR_SIZE)

#define NVRAM_NAME_MAX_LEN 128
#define NVRAM_VALUE_MAX_LEN 256

/**************************************************************************
 *
 * The "nvram" is stored as a list of '\0' terminated
 * "name=value" strings. The end of the list is marked by a double
 * '\0'. New entries are always added at the end. Deleting an entry
 * shifts the remaining entries to the front. Replacing an entry is a
 * combination of deleting the old value and adding the new one.
 *
 * The nvram is preceeded by a 32 bit CRC over the data part.
 *
 ***************************************************************************/
typedef struct nvram_s {
	unsigned long	crc;		        /* CRC32 over data bytes	*/
	unsigned char	data[NVRAM_DATA_SIZE];  /* Environment data		*/
} nvram_t;


static int nvrammatch (uchar *, int);
static uchar *nvram_get_addr(int);
static int savenvram(ulong flash_addr);
static uchar nvram_get_char_memory (int index);
static void nvram_crc_update (void);
//static uchar nvram_get_char_init (int index);
static int _do_setnvram (int flag, int argc, char *argv[]);


uchar default_factory_nvram[] = {
	BOOT_NUM "=0""\0"
	BOOT_SUCCESS "=0""\0"
	"\0"
};

int g_nvram_changed = 0;
nvram_t *nvram_ptr = (nvram_t *)FACTORY_NVRAM_ADDR;


/* Function that returns a character from the NVRAM */
static uchar (*nvram_get_char)(int) = nvram_get_char_memory;


void nvram_relocate_redundant(nvram_t *nvram_flash, nvram_t *nvram_bak_flash, 
                            uchar *default_nvram, int default_nvram_size)
{

    if ((nvram_flash == NULL) || (nvram_bak_flash == NULL)) {
        puts("#### Error: invalid nvram flash addr ####\n");
        return;
    }

    /*
    * We must allocate a buffer for the nvram
    */
    if (nvram_ptr == (nvram_t *)FACTORY_NVRAM_ADDR) {
        nvram_ptr = (nvram_t *)malloc(NVRAM_SECTOR_SIZE);
        CUSTOM_DPRINTF("malloced ENV at %p\n", nvram_ptr);
    }

    memset(nvram_ptr, 0, sizeof(nvram_ptr));
    /*
    * After relocation to RAM, we can always use the "memory" functions
    */
    nvram_get_char = nvram_get_char_memory;

    /* (1) major NVRAM sector data valid */
    if (crc32(0, nvram_flash->data, NVRAM_DATA_SIZE) == nvram_flash->crc) 
	{
	    memcpy (nvram_ptr, (void*)nvram_flash, NVRAM_SECTOR_SIZE);
	    if (nvram_flash->crc != nvram_bak_flash->crc)
	    {
            savenvram((ulong)nvram_bak_flash);  /* backup */
        }
	}
	/* (2) major NVRAM sector unvlid but bakup NVRAM sector valid */
	else if (crc32(0, nvram_bak_flash->data, NVRAM_DATA_SIZE) == nvram_bak_flash->crc) 
	{
	    puts("#### Warning: major nvram CRC bad, using bakup data.####\n");
		memcpy (nvram_ptr, (void*)nvram_bak_flash, NVRAM_SECTOR_SIZE);
        savenvram((ulong)nvram_flash);    /* make NVRAM sector data valid */
	}
	else /* (3) major NVRAM & backup NVRAM two sectors are all unvalid! */
    {
        /* Small probability events, just in case */
        puts("#### Fatal: major & bakup nvram CRC all bad, using default value!####\n");
        if (default_nvram) {
            memcpy (nvram_ptr->data, (void*)default_nvram, default_nvram_size);
        }
        nvram_crc_update();
        savenvram((ulong)nvram_flash);
        savenvram((ulong)nvram_bak_flash);
    }
}

void nvram_relocate(void)
{
    nvram_relocate_redundant((nvram_t *)NVRAM_ADDR, (nvram_t *)NVRAM_BAK_ADDR, NULL, 0);
    nvram_relocate_redundant((nvram_t *)FACTORY_NVRAM_ADDR, (nvram_t *)FACTORY_NVRAM_BAK_ADDR, 
        default_factory_nvram, sizeof(default_factory_nvram));
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
*  无。
* 
* 说    明: varname和varvalue的最大长度分别为128和256
******************************************************************************/
void setnvram (char *varname, char *varvalue)
{
	char *argv[4] = { "setenv", varname, varvalue, NULL };
	if ( (strlen(varname) < NVRAM_NAME_MAX_LEN) &&
	    (strlen(varvalue) < NVRAM_VALUE_MAX_LEN))
	{
	    g_nvram_changed = 1;
	    _do_setnvram (0, 3, argv);
	}
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
				putc(nvram_get_char(k));
			putc  ('\n');

			if (ctrlc()) {
				puts ("\n ** Abort\n");
				return 1;
			}
		}

		printf("NVRAM size: %d/%d bytes\n", i, NVRAM_DATA_SIZE);

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
			putc ('=');
			while (k < nxt)
				putc(nvram_get_char(k++));
			putc ('\n');
			break;
		}
		if (k < 0) {
			printf ("## Error: \"%s\" not defined\n", name);
			rcode ++;
		}
	}
	return rcode;
}

/************************************************************************
 * Look up variable from environment,
 * return address of storage for that variable,
 * or NULL if not found
 */
char *getnvram (char *name)
{
	int i, nxt;

	WATCHDOG_RESET();

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

/******************************************************************************
* 函数名称: getnvram_r
* 功    能: 获取NVRAM变量值
* 参    数:
* 参数名称          输入/输出         描述
* varname              输入           变量名
* buf                  输出           存放变量值
* len                  输入           buf的长度
*
* 函数返回: 
*  成功返回变量值的长度
*  失败返回-1
* 
* 说    明: len的大小一定要为buf的实际长度，如果buf的的实际长度比len的值小的话
*          很可能会导致内存访问越界。
******************************************************************************/
int getnvram_r (char *name, char *buf, unsigned len)
{
	int i, nxt;

	for (i=0; nvram_get_char(i) != '\0'; i=nxt+1) {
		int val, n;

		for (nxt=i; nvram_get_char(nxt) != '\0'; ++nxt) {
			if (nxt >= NVRAM_SECTOR_SIZE) {
				return (-1);
			}
		}
		if ((val=nvrammatch((uchar *)name, i)) < 0)
			continue;
		/* found; copy out */
		n = 0;
		while ((len > n++) && (*buf++ = nvram_get_char(val++)) != '\0')
			;
		if (len == n)
			*buf = '\0';
		return (n);
	}
	return (-1);
}

/************************************************************************
 * Set a new nvram variable,
 * or replace or delete an existing one.
 *
 * This function will ONLY work with a in-RAM copy of the environment
 */
static int _do_setnvram (int flag, int argc, char *argv[])
{
	int   i, len, oldval;
	uchar *env, *nxt = NULL;
	char *name;

	uchar *env_data = nvram_get_addr(0);

	if (!env_data)	/* need copy in RAM */
		return 1;

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
		return 0;
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
	if (len > (&env_data[NVRAM_DATA_SIZE]-env)) {
		printf ("## Error: nvram overflow, \"%s\" deleted\n", name);
		return 1;
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

	return 0;
}

/******************************************************************************
* 函数名称: savenvram
* 功    能: 将设置的nvram变量保存到flash中
* 参    数:
* 参数名称          输入/输出         描述
*
* 函数返回: 
*  无。
* 
* 说    明: 
******************************************************************************/
static int savenvram(ulong flash_addr)
{
	int	len, rc;
	ulong	end_addr;
	ulong	flash_sect_addr;
	uchar *env_buffer = (uchar *)nvram_ptr;
	int rcode = 0;

	if (flash_addr == (ulong)nvram_ptr) {
		printf ("*** Error - nvram@0x%08lx read-only before relocation to RAM\n\n", nvram_ptr);
		return 1;
	}

	flash_sect_addr = (ulong)flash_addr;
	len	 = NVRAM_SECTOR_SIZE;

	end_addr = flash_sect_addr + len - 1;

	CUSTOM_DPRINTF ("nvram_ptr = %08lX  Protect off %08lX ... %08lX\n",
		(ulong)nvram_ptr, (ulong)flash_sect_addr, end_addr);

	if (flash_sect_protect (0, flash_sect_addr, end_addr))
		return 1;

	CUSTOM_DPRINTF ("Erasing Flash...");
	if (flash_sect_erase (flash_sect_addr, end_addr))
		return 1;

	CUSTOM_DPRINTF ("Writing to Flash... ");
	CUSTOM_DPRINTF("(from %08lX to %08lX)\n", (ulong)env_buffer, (ulong)flash_sect_addr);
	rc = flash_write((char *)env_buffer, flash_sect_addr, len);
	if (rc != 0) {
		flash_perror (rc);
		rcode = 1;
	} else {
		puts ("done\n");
	}

	/* try to re-protect */
	(void) flash_sect_protect (1, flash_sect_addr, end_addr);
	return rcode;
}

void save_nvram_auto(void)
{
    if (g_nvram_changed) {
        savenvram(FACTORY_NVRAM_ADDR);
        g_nvram_changed = 0;
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
	return ((uchar *)(nvram_ptr->data + index));
}
/*
static uchar nvram_get_char_init (int index)
{
	uchar c;

	c = *(nvram_flash->data + index);

	return (c);
}
*/
static uchar nvram_get_char_memory (int index)
{
	return ( *((uchar *)(nvram_ptr->data + index)) );
}

static void nvram_crc_update (void)
{
	nvram_ptr->crc = crc32(0, nvram_ptr->data, NVRAM_DATA_SIZE);
}


int do_nvram_save(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    return savenvram(FACTORY_NVRAM_ADDR);
}
int do_nvram_set(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    setnvram(argv[1],argv[2]);
    return 0;
}
int do_nvram_show(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	int rcode = 0;
	
    rcode = do_printnvram(argc,argv);
	return rcode;
}

U_BOOT_CMD(
    nvramshow, 4, 1,	do_nvram_show,
	"nvramshow - print factory.nvram variables\n",
	"\n    - print values of all nvram variables\n"
	"print all nvram ...\n"	"    - print value of nvram variable 'name'"
);

U_BOOT_CMD(
    nvramset, 4, 0,	do_nvram_set,
	"nvramset - set factory.nvram variables\n",
	"nvramset name value ...\n"	"    - set nvram variable 'name' to 'value ...'\n"
	"nvramset name\n"	"    - delete nvram variable 'name'"
);

U_BOOT_CMD(
    nvramsave, 1, 0,	do_nvram_save,
	"nvramsave - save factory.nvram variables to persistent storage\n",
	""
);

