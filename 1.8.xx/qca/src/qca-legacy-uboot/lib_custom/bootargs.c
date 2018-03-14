
#include "custom_internal.h"




DECLARE_GLOBAL_DATA_PTR;

/* 文件系统所在的分区，'4'表示/dev/mtdblock4, '5'表示/dev/mtdblock5 2012-9-12 mod by zhangping */
#define NVRAM_VERSION_1 '3'
#define NVRAM_VERSION_2 '4'
#define OPENWRT_ROOTFS_NAME "(rootfs)"
#define OPENWRT_ROOTFSBAK_NAME "(rootfs.bak)"


#define print_char(_c) ((_c)?(_c):' ')

unsigned long bb_ram_reserve(void)
{
    unsigned long bb_ram_size = 0;
    /*if ram_size >= 64M, reserve 4M*/
    if (gd->ram_size >= (64 << 20)) {
        bb_ram_size = (4 << 20);
    }
    /*if 32 <= ram_size < 64M, reserve 1M*/
    else if (gd->ram_size >= (32 << 20)) {
        bb_ram_size = (1 << 20);
    }
    
    return bb_ram_size;
}

#if 0
int is_kernel_openwrt(void)
{
    int is_openwrt = 0;
    unsigned long addr;
    char *bootm_addr = NULL;
    char *bootcmd = getenv ("bootcmd");
    struct image_header *hdr = NULL;
        
    if (bootcmd) {
        bootm_addr = strstr(bootcmd, "bootm ");
        if (bootm_addr) {
            bootm_addr += strlen("bootm ");
            addr = simple_strtoul(bootm_addr, NULL, 16);   

            hdr = (struct image_header *)addr;
        	if (ntohl(hdr->ih_magic) == IH_MAGIC) {
                printf("uImage@%p %s\n", hdr, hdr->ih_name);
                /*search "OpenWrt" in kernel uimage name*/
                if (strstr((char *)hdr->ih_name, "OpenWrt")) {
                    is_openwrt = 1;
                }            
            } else {
                printf("Error, uImage@%p Bad Magic Number\n", hdr);
            }
        }
    }

    return is_openwrt;
}
#endif

static int is_new_version(void)
{
    char *boot_suc_ptr = NULL;

    boot_suc_ptr = getnvram(BOOT_SUCCESS);

    CUSTOM_DPRINTF("boot_success=%s\n", boot_suc_ptr);

    return (boot_suc_ptr && ('1' == *boot_suc_ptr)) ? 0 : 1;
}

static int has_chance_to_try(void)
{
    char *boot_num_ptr = NULL;
    char bootnum_val[2]= "0";
    
    boot_num_ptr = getnvram(BOOT_NUM);
    CUSTOM_DPRINTF("bootnum=%s\n", boot_num_ptr);
    if (NULL == boot_num_ptr) /* just in case */
    {
        setnvram(BOOT_NUM, "0");
        save_nvram_auto();
    }
    else {
        bootnum_val[0] = *boot_num_ptr;
    }
    
    CUSTOM_DPRINTF("bootnum_val[0] = %c\n", print_char(bootnum_val[0]));
    
    if (bootnum_val[0] <= '9' && bootnum_val[0] >= '0')
    {
        bootnum_val[0] += 1;
        CUSTOM_DPRINTF("after add 1 bootnum_val=%s\n", bootnum_val);
        setnvram(BOOT_NUM, bootnum_val);
        save_nvram_auto();
        CUSTOM_DPRINTF("After add 1 & saved bootnum=%s\n", getnvram(BOOT_NUM));
        return 1;
    }

    return 0;
}


void openwrt_mtdname(char *bootargs, char root_mtd)
{
    char *p, *p_rootfs, *p_rootfsbak;
    int mtd_rootfs, mtd_rootfsbak;
    int mtd = -1;
    int move_len;
    char *dst, *src;

    CUSTOM_DPRINTF("root_mtd=%c; bootargs=%s \n", print_char(root_mtd), bootargs);
    mtd_rootfs = root_mtd - '0';
    if (root_mtd == NVRAM_VERSION_1) {
        mtd_rootfsbak = NVRAM_VERSION_2 - '0';
    }
    else if (root_mtd == NVRAM_VERSION_2) {
        mtd_rootfsbak = NVRAM_VERSION_1 - '0';
    }
    else {
        /*should not happend*/
        CUSTOM_DPRINTF("invalid root mtd %c\n", print_char(root_mtd));
        return;
    }

    CUSTOM_DPRINTF("mtd_rootfs=%d mtd_rootfsbak=%d\n", mtd_rootfs, mtd_rootfsbak);
    p = strstr(bootargs, "mtdparts");
    p_rootfs = NULL;
    p_rootfsbak = NULL;
    /*find starting point of rootfs & rootfs-bak*/
    while (p && (*p != ' ') && (*p != '\0')) {
        if (*p == '(') {
            mtd++;

            if (mtd == mtd_rootfs) {
                p_rootfs = p;
            }
            if (mtd == mtd_rootfsbak) {
                p_rootfsbak = p;
            }
            
            if (p_rootfs && p_rootfsbak) {
                CUSTOM_DPRINTF("bootargs %p found p_rootfs %p p_rootfsbak %p\n", 
                    bootargs, p_rootfs, p_rootfsbak);
                break;
            }
        }
        p++;
    }

    /* switch mtd names if :
     * current target mtd name is (rootfs-bak), and
     * current backup mtd name is (rootfs)
     */
    if (p_rootfs && (strncmp(p_rootfs, OPENWRT_ROOTFSBAK_NAME, strlen(OPENWRT_ROOTFSBAK_NAME)) == 0)
        && p_rootfsbak && (strncmp(p_rootfsbak, OPENWRT_ROOTFS_NAME, strlen(OPENWRT_ROOTFS_NAME)) == 0))
    {
        if (p_rootfs < p_rootfsbak) {
            /*   
             ***(rootfs-bak)********(rootfs) --> (rootfs)********(rootfs-bak)
                |       |   |       |            |       |   |   |
             p_rootfs  dst src  p_rootfsbak   p_rootfs  dst src  p_rootfsbak-new
             */
            src = p_rootfs + strlen(OPENWRT_ROOTFSBAK_NAME);
            dst = p_rootfs + strlen(OPENWRT_ROOTFS_NAME);
            move_len = p_rootfsbak - src;
            p_rootfsbak += (dst - src);
        }
        else {
            /*   
             *****(rootfs)********(rootfs-bak) --> (rootfs-bak)********(rootfs)
                  |       |   |   |                |       |   |       |
             p_rootfsbak src dst p_rootfs   p_rootfsbak   src dst   p_rootfs-new
             */
            src = p_rootfsbak + strlen(OPENWRT_ROOTFS_NAME);
            dst = p_rootfsbak + strlen(OPENWRT_ROOTFSBAK_NAME);
            move_len = p_rootfs - src;
            p_rootfs += (dst - src);
        }
        CUSTOM_DPRINTF("src %p dst %p move_len %d p_rootfs %p p_rootfsbak %p\n",
            src, dst, move_len, p_rootfs, p_rootfsbak);
        memmove(dst, src, move_len);
        //CUSTOM_DPRINTF("bootargs=%s\n", bootargs);
        memcpy(p_rootfs, OPENWRT_ROOTFS_NAME, strlen(OPENWRT_ROOTFS_NAME));
        memcpy(p_rootfsbak, OPENWRT_ROOTFSBAK_NAME, strlen(OPENWRT_ROOTFSBAK_NAME));
    }

    CUSTOM_DPRINTF("bootargs=%s\n", bootargs);
    return;
}

int modify_bootargs(void)
{
    char bootargs_buf[512];
    char mem_buf[16];
	char *env_bootargs = NULL;
    char *env_root = NULL;
    int root_value_len = 0;
    char *targetver = NULL;
    volatile char cVersion;

    memset(bootargs_buf, 0, sizeof(bootargs_buf));
    memset(mem_buf, 0, sizeof(mem_buf));

    if (NULL == (env_bootargs = getenv("bootargs")))
    {
        printf("%s get bootargs failed!\n", __FUNCTION__);
        return -1;
    }
    
    strcpy(bootargs_buf, env_bootargs);

    if (NULL == (env_root = strstr(bootargs_buf, "root=")))
    {
        printf("%s get env_root or env_rootfstype failed!\n", __FUNCTION__);
        return -1;
    }
    root_value_len = 0;
    do
    {
        if ('\0' == *env_root)
        {
            break;
        }
        root_value_len++;
    }while ( ' ' != *env_root++);
    
    env_root -= 2; /* make env_root point at the position of "4", for example */
    CUSTOM_DPRINTF("root_value_len=%d##env_root pointing at#%c\n", root_value_len, print_char(*env_root));

    if (root_value_len - 1 > strlen("root=/dev/mtdblock4"))
    {
        puts("root=?, only \"root=/dev/mtdblock*\" and \"root=31:0*\" is valid. Please make it clear!\n");
    }
    
    targetver = getnvram(TARGET_VERSION);
    CUSTOM_DPRINTF("targetver=%s\n", targetver);
    cVersion = targetver?(*targetver):0;

    if (NVRAM_VERSION_1 != cVersion && NVRAM_VERSION_2 != cVersion)
    {
        printf("###Warning:Wrong mtd number(targetver=%c), using default(%c)###\n", print_char(cVersion), print_char(*env_root));
        //return -1;
        cVersion = *env_root;
    }

    if (!is_new_version() || has_chance_to_try())
    {
        CUSTOM_DPRINTF("boot mtd %c...\n", print_char(cVersion));
        *env_root = cVersion;
    }
    else
    {
        char pre_version[2] = {};

        puts("#####Warning: New version tried 10 times but all failed, "\
                "so turn back to the previous version#####\n");
        
        pre_version[0] = (NVRAM_VERSION_1 == cVersion)? NVRAM_VERSION_2:NVRAM_VERSION_1;
        *env_root = pre_version[0];
        setnvram(TARGET_VERSION, pre_version);
        setnvram(BOOT_SUCCESS, "1");
        setnvram(BOOT_NUM, "0");
        save_nvram_auto();
    }

    openwrt_mtdname(bootargs_buf, *env_root);

    /*add ram size to bootargs*/
    if (!strstr(bootargs_buf, "mem=")) {
		sprintf(mem_buf, " mem=%luM", (gd->ram_size - bb_ram_reserve()) >> 20);
        strcat(bootargs_buf, mem_buf);
    }

    setenv("bootargs", bootargs_buf);
    return 0;
}


/* 2012-9-6 add start by zhangping */
int do_modify_env(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    return modify_bootargs();
}


U_BOOT_CMD(
    modifyenv, 1, 0,	do_modify_env,
	"modifyenv - modify bootargs of env\n",
	""
);


