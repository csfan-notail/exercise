#include <common.h>
#include <command.h>
#include <asm/mipsregs.h>
#include <asm/addrspace.h>
#include <config.h>
#include <version.h>
#include <atheros.h>

//#define DEBUG_CUSTOM
#ifdef DEBUG_CUSTOM
#define CUSTOM_DPRINTF(fmt,args...) printf("%s:%d: "fmt, __func__, __LINE__, ##args)
#else
#define CUSTOM_DPRINTF(fmt,args...)
#endif

/* NVRAM variables name */
#define TARGET_VERSION  "targetver"
#define BOOT_SUCCESS    "boot_success"
#define BOOT_NUM        "bootnum"


#define GPIO_OUT_FUNCTION0 0x1804002c
#define RST_WATCHDOG_TIMER_CONTROL 0x18060008
#define RST_WATCHDOG_TIMER 0x1806000c

#define FACTORY_DATA_SIZE 0x20000
#define ART_SECTOR_SIZE 0x10000

#define CUSTOM_BUTTON_SW_RESET_TIMTOUT  3

#define led_off 0
#define led_on 1

/*------------------------------- GPIO -------------------------------*/
#if CUSTOM_BOARD_TA62

#define CUSTOM_GPIO_PHY_RESET   6       /*PHY_RET_N*/

#define CUSTOM_GPIO_BUTTON_RST  5       /*SW_RST*/    
#define CUSTOM_BTN_RESET_ACTIVE 0       /*0-pressed*/

#define CUSTOM_GPIO_LED_RUN     11      /*RUN_LED   1-on*/
#define CUSTOM_GPIO_LED_WLAN    10      /*WLAN_LED  1-on*/

#elif CUSTOM_BOARD_TA37

//altered by caibing 2016-06-27
//#define CUSTOM_GPIO_BUTTON_RST  15      /*SW_RST*/    
#define CUSTOM_GPIO_BUTTON_RST  3      /*SW_RST*/    
#define CUSTOM_BTN_RESET_ACTIVE 1       /*1-pressed*/

#define CUSTOM_GPIO_LED_RUN     0      /*reserve*/
#define CUSTOM_GPIO_LED_WLAN    0      /*reserve*/
#define CUSTOM_GPIO_LED_WAN     0      /*reserve*/

#define CUSTOM_GPIO_LED_LAN1    14      /*LAN0_LED  1-on*/
#define CUSTOM_GPIO_LED_LAN2    1      /*LAN1_LED  1-on*/
#define CUSTOM_GPIO_LED_LAN3    11      /*LAN2_LED  1-on*/
#define CUSTOM_GPIO_LED_LAN4    16      /*LAN3_LED  1-on*/

//#define CUSTOM_GPIO_LED_RUN_GREEN     16      /*GREEN_RUN_LED   0-on*/
//altered by caibing 2016-06-27
#define CUSTOM_GPIO_LED_RUN_GREEN     0      /*GREEN_RUN_LED   0-on*/
//altered by caibing 2016-06-27
//#define CUSTOM_GPIO_LED_RUN_RED     17      /*RED_RUN_LED   0-on*/
#define CUSTOM_GPIO_LED_RUN_RED     4      /*RED_RUN_LED   0-on*/

//altered by caibing 2016-06-27
#define CUSTOM_GPIO_SPI0     2      /*reserve*/
#define CUSTOM_GPIO_SPI1     17      /*reserve*/

#elif CUSTOM_BOARD_TA30_51
#define CUSTOM_GPIO_BUTTON_RST  0      /*SW_RST*/    
#define CUSTOM_BTN_RESET_ACTIVE 1       /*1-pressed*/

#define CUSTOM_GPIO_LED_RUN     12      /*SYS_LED   1-on*/
#define CUSTOM_GPIO_LED_WLAN    0      /*reserve*/
#define CUSTOM_GPIO_LED_WAN     0      /*reserve*/
#define CUSTOM_GPIO_LED_LAN1    0      /*reserve*/
#define CUSTOM_GPIO_LED_LAN2    0      /*reserve*/
#define CUSTOM_GPIO_LED_LAN3    0      /*reserve*/
#define CUSTOM_GPIO_LED_LAN4    0      /*reserve*/

#else
#error "***error,unknow custom board"
#endif


#if CUSTOM_BOARD_TA62 || CUSTOM_BOARD_TA37 || CUSTOM_BOARD_TA30_51
#define CUSTOM_GPIO_EN_OUTPUT(_gpio) ath_reg_rmw_clear(GPIO_OE_ADDRESS, 1 << (_gpio))
#define CUSTOM_GPIO_EN_INPUT(_gpio) ath_reg_rmw_set(GPIO_OE_ADDRESS, 1 << (_gpio))
#define CUSTOM_GPIO_SET(_gpio) ath_reg_wr_nf(GPIO_SET_ADDRESS, 1 << (_gpio))
#define CUSTOM_GPIO_CLEAR(_gpio) ath_reg_wr_nf(GPIO_CLEAR_ADDRESS, 1 << (_gpio))

#define CUSTOM_GPIO_IN_VAL(_gpio) ((1 << _gpio) & (ath_reg_rd(GPIO_IN_ADDRESS)))

#define CUSTOM_GPIO_SET_OUTPUT(_gpio, _val) do {    \
    CUSTOM_GPIO_EN_OUTPUT(_gpio);   \
    if (_val) {                     \
        CUSTOM_GPIO_SET(_gpio);     \
    }       \
    else {  \
        CUSTOM_GPIO_CLEAR(_gpio);   \
    }       \
} while(0)


#define CUSTOM_GPIO_FUNCTION_ADDRESS(_gpio) (GPIO_OUT_FUNCTION0 + (((_gpio) >> 2) << 2))
#define CUSTOM_GPIO_FUNCTION_OFFSET(_gpio) (((_gpio) % 4) * 8)
#define CUSTOM_GPIO_FUNCTION_SET(_gpio, _func) do {         \
    u_int32_t _reg = CUSTOM_GPIO_FUNCTION_ADDRESS(_gpio);   \
    u_int32_t _offset = CUSTOM_GPIO_FUNCTION_OFFSET(_gpio); \
    ath_reg_wr(_reg, ((ath_reg_rd(_reg)) & ~(0xff << _offset)) | ((_func) << _offset)); \
    ath_reg_rd(_reg);   \
} while(0)

#endif





