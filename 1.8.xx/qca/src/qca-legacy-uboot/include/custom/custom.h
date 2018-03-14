/*must included after config.h,board956x.h,board953x.h*/

#ifndef _CUSTOM_H_
#define _CUSTOM_H_



/*----------------------------------------------------------------------------*/

#if CUSTOM_BOARD_TA37
#define CONFIG_OVERRIDE_DDR_TAP 0x13
#define CONFIG_CUSTOM_BOARD "ta37"
#elif CUSTOM_BOARD_TA30_51
#define CONFIG_OVERRIDE_DDR_TAP 0x13
#define CONFIG_CUSTOM_BOARD "ta37"
#elif CUSTOM_BOARD_TA62
#define CONFIG_OVERRIDE_DDR_TAP 0x10
#define CONFIG_CUSTOM_BOARD "ta62"
#else
#error "***error,unknow custom board"
#endif


/*override env defined in board956x.h,board953x.h*/
#if FLASH_SIZE == 16
#define CONFIG_CUSTOM_MTDPARTS "256k(uboot),64k(uboot.env),1472k(kernel),6080k(rootfs),6080k(rootfs.bak),1984k(rootfs_data),64k(nvram),64k(nvram.bak),128k(factory.data),64k(factory.nvram),64k(factory.nvram.bak),64k(art),7552k@0x50000(firmware)"
#else
#error "***unsupported FLASH_SIZE"
#endif
#undef CONFIG_BOOTARGS
#define CONFIG_BOOTARGS "console=ttyS0,115200 root=31:03"   \
                        " board=" CONFIG_CUSTOM_BOARD   \
                        " mtdparts=spi0.0:" CONFIG_CUSTOM_MTDPARTS  \
                        " norootfssplit"
#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND "bootm 0x9f050000"
#undef CONFIG_IPADDR
#define CONFIG_IPADDR 192.168.55.1
#undef CONFIG_SERVERIP
#define CONFIG_SERVERIP 192.168.55.136
#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS   \
"upboot=tftp 0x80060000 u-boot.bin&&erase 0x9f000000 +$filesize&&cp.b $fileaddr 0x9f000000 $filesize\0"    \    
"upk=tftp 0x80060000 kernel.bin&&erase 0x9f050000 +$filesize&&cp.b $fileaddr 0x9f050000 $filesize\0"    \
"upfs1=tftp 0x80060000 app.bin&&erase 0x9f1c0000 +$filesize&&cp.b $fileaddr 0x9f1c0000 $filesize&&nvramset targetver 3&&nvramsave&&modifyenv\0"  \
"upfs2=tftp 0x80060000 app.bin&&erase 0x9f7b0000 +$filesize&&cp.b $fileaddr 0x9f7b0000 $filesize&&nvramset targetver 4&&nvramsave&&modifyenv\0"


//#define CONFIG_BOOTDELAY	3
#define CONFIG_AUTOBOOT_KEYED 1
#define CONFIG_AUTOBOOT_PROMPT "Press password to abort autoboot in %d second\n"
#define CONFIG_AUTOBOOT_STOP_STR "wlan"
#define CONFIG_AUTOBOOT_STOP_STR2 "\x03"

#define CONFIG_OVERWRITE_ETHADDR_ONCE
#define CONFIG_WATCHDOG


/*----------------------------------------------------------------------------*/
/* lib_custom/watchdog.c */
void custom_watchdog_enable(void);
void custom_watchdog_disable(void);
#define watchdog_reset custom_watchdog_enable
#define WATCHDOG_CLOSE custom_watchdog_disable

/* lib_custom/bootargs.c */
unsigned long bb_ram_reserve(void);
int modify_bootargs(void);

/* lib_custom/nvram_flash.c */
void nvram_relocate(void);
char *getnvram (char *name);
int getnvram_r (char *name, char *buf, unsigned len);
void setnvram (char *varname, char *varvalue);
void save_nvram_auto(void);

/* lib_custom/ddr_test.c */
void ddr_test_waiting_key(void);

/* lib_custom/bootinfo.c */
int custom_display_banner(void);
void get_boot_info(void);

/* lib_custom/ping_restore.c */
void ping_restore_monitor(void);
void ping_restore_recv_echoreq(int len);

/* lib_custom/update_flashimage.c */
int update_flashimage(void);


/* lib_custom/board.c */
void custom_overrid_tap(void);
void custom_reset_phy(void);
void custom_led_set_run(int on);
void custom_led_set_wlan(int on);
void custom_led_set_wan(int on);
void custom_led_set_lan(int on);
void custom_led_set_wan_lan(int on);
void custom_poweron_init(void);
void custom_reset_default(void);
void custom_main(void);

/*----------------------------------------------------------------------------*/


#endif /*_CUSTOM_H_*/
