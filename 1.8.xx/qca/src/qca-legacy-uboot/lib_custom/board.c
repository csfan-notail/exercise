
#include "custom_internal.h"
#include "ar9344.h"
#include <watchdog.h>

#define	COMMAND_BYTE_SHIFT	1
#define COMMAND_SHIFT		8
#define COMMAND_RDWR_SHIFT	7

#define COMMAND_RD			1
#define	COMMAND_WR			0

#define	COMMAND_BYTE_GSR1		0x00
#define	COMMAND_BYTE_GSR2		0X01
#define	COMMAND_BYTE_OCR1		0X02
#define	COMMAND_BYTE_OCR2		0X03
#define COMMAND_BYTE_GCR1 		0X06
#define COMMAND_BYTE_GCR2		0X07

#define GPIO_EXPANDER_RESET			12
#define GPIO_LTE_USB_ENABLE			13
#define GPIO_LTE_USB_BYPASS			15
#define GPIO_EX1_SIM_CARD_SW		0
#define GPIO_EX1_SIM_CARD_ST		1
#define GPIO_EX1_ATTENUATOR_SW		13	//ver.c 15 ; ver.d 13

/*override DQS TAP delay */
void custom_overrid_tap(void)
{
#ifdef CONFIG_OVERRIDE_DDR_TAP
    prmsg("override Tap Values to %#x\n", CONFIG_OVERRIDE_DDR_TAP);
    ath_reg_wr(TAP_CONTROL_0_ADDRESS, CONFIG_OVERRIDE_DDR_TAP);
    ath_reg_wr(TAP_CONTROL_1_ADDRESS, CONFIG_OVERRIDE_DDR_TAP);
    ath_reg_wr(TAP_CONTROL_2_ADDRESS, CONFIG_OVERRIDE_DDR_TAP);
    ath_reg_wr(TAP_CONTROL_3_ADDRESS, CONFIG_OVERRIDE_DDR_TAP);
#endif
    return;
}

void custom_reset_phy(void)
{
#if CUSTOM_BOARD_TA62
    /*Enable GPIO6(PHY_RET_N) Out to reset PHY*/
    CUSTOM_GPIO_SET_OUTPUT(CUSTOM_GPIO_PHY_RESET, 1);
#endif
    return;
}

void custom_led_set_run(int on) {
    
    #if 1
    if(on){
        //altered by caibing 2016-06-27
        CUSTOM_GPIO_SET_OUTPUT(CUSTOM_GPIO_LED_RUN_GREEN, !on);
        CUSTOM_GPIO_SET_OUTPUT(CUSTOM_GPIO_LED_RUN_RED, on);
    }
    else
    {
        CUSTOM_GPIO_SET_OUTPUT(CUSTOM_GPIO_LED_RUN_GREEN, !on);
        CUSTOM_GPIO_SET_OUTPUT(CUSTOM_GPIO_LED_RUN_RED, !on);
    }
    #else
    CUSTOM_GPIO_SET_OUTPUT(CUSTOM_GPIO_LED_RUN, on);
    #endif
    return;
}

void custom_led_set_wlan(int on) {
    
    CUSTOM_GPIO_SET_OUTPUT(CUSTOM_GPIO_LED_WLAN, on);
    
    return;
}

void custom_led_set_wan(int on) {

#if CUSTOM_BOARD_TA37
    CUSTOM_GPIO_SET_OUTPUT(CUSTOM_GPIO_LED_WAN, !on);
#endif
    return;
}

void custom_led_set_lan(int on) {

#if CUSTOM_BOARD_TA37
    CUSTOM_GPIO_SET_OUTPUT(CUSTOM_GPIO_LED_LAN1, !on);
    CUSTOM_GPIO_SET_OUTPUT(CUSTOM_GPIO_LED_LAN2, !on);
    CUSTOM_GPIO_SET_OUTPUT(CUSTOM_GPIO_LED_LAN3, !on);
    CUSTOM_GPIO_SET_OUTPUT(CUSTOM_GPIO_LED_LAN4, !on);
#endif
    return;
}

void custom_led_set_wan_lan(int on) {
    custom_led_set_wan(on);
    custom_led_set_lan(on);
    return;
}

void custom_led_init(void)
{
#if 0    
#if CUSTOM_BOARD_TA62 | CUSTOM_BOARD_TA37
    /*disable JTAG*/
    ath_reg_rmw_set( GPIO_FUNCTION_ADDRESS,GPIO_FUNCTION_DISABLE_JTAG_SET(1));

    /*turn on led RUN*/
    custom_led_set_run(led_on);
    /*turn off led WLAN*/
    custom_led_set_wlan(led_off);
    /*turn off led WAN & LAN*/
    custom_led_set_wan_lan(led_off);
    
    /*clear gpio function*/
    CUSTOM_GPIO_FUNCTION_SET(CUSTOM_GPIO_LED_RUN, 0);    
    CUSTOM_GPIO_FUNCTION_SET(CUSTOM_GPIO_LED_WLAN, 0);    
#endif
#if CUSTOM_BOARD_TA37 
    CUSTOM_GPIO_FUNCTION_SET(CUSTOM_GPIO_LED_WAN, 0);    
    CUSTOM_GPIO_FUNCTION_SET(CUSTOM_GPIO_LED_LAN1, 0);    
    CUSTOM_GPIO_FUNCTION_SET(CUSTOM_GPIO_LED_LAN2, 0);    
    CUSTOM_GPIO_FUNCTION_SET(CUSTOM_GPIO_LED_LAN3, 0);    
    CUSTOM_GPIO_FUNCTION_SET(CUSTOM_GPIO_LED_LAN4, 0);    
#endif

#if CUSTOM_BOARD_TA30_51
 /*disable JTAG*/
    ath_reg_rmw_set( GPIO_FUNCTION_ADDRESS,GPIO_FUNCTION_DISABLE_JTAG_SET(1));
    /*turn on led RUN*/
    custom_led_set_run(led_on);
     /*clear gpio function*/
    CUSTOM_GPIO_FUNCTION_SET(CUSTOM_GPIO_LED_RUN, 0);     
#endif
    return;
#endif

#if 1
CUSTOM_GPIO_SET_OUTPUT(CUSTOM_GPIO_LED_LAN1, 0);
//CUSTOM_GPIO_SET_OUTPUT(CUSTOM_GPIO_LED_LAN2, 1);
CUSTOM_GPIO_SET_OUTPUT(CUSTOM_GPIO_LED_LAN3, 0);
CUSTOM_GPIO_SET_OUTPUT(CUSTOM_GPIO_LED_LAN4, 0);
#endif



#if 1
    /*disable JTAG*/
    ath_reg_rmw_set( GPIO_FUNCTION_ADDRESS,GPIO_FUNCTION_DISABLE_JTAG_SET(1));
    
     /*clear gpio function*/
    CUSTOM_GPIO_FUNCTION_SET(CUSTOM_GPIO_LED_RUN_GREEN, 0);  
    CUSTOM_GPIO_FUNCTION_SET(CUSTOM_GPIO_LED_RUN_RED, 0);
    //CUSTOM_GPIO_FUNCTION_SET(CUSTOM_GPIO_LED_LAN1, 0);    
    CUSTOM_GPIO_FUNCTION_SET(CUSTOM_GPIO_LED_LAN2, 0);    
    CUSTOM_GPIO_FUNCTION_SET(CUSTOM_GPIO_BUTTON_RST, 0);    
    
    CUSTOM_GPIO_FUNCTION_SET(CUSTOM_GPIO_SPI0, 10);    
    CUSTOM_GPIO_SET_OUTPUT(CUSTOM_GPIO_SPI0, 1);
    //CUSTOM_GPIO_FUNCTION_SET(CUSTOM_GPIO_LED_LAN3, 0);    
    //CUSTOM_GPIO_FUNCTION_SET(CUSTOM_GPIO_LED_LAN4, 0);   

	CUSTOM_GPIO_FUNCTION_SET(CUSTOM_GPIO_SPI1, 11);  	
    CUSTOM_GPIO_SET_OUTPUT(CUSTOM_GPIO_SPI1, 1);

//altered by caibing 
   //CUSTOM_GPIO_SET_OUTPUT(CUSTOM_GPIO_LED_LAN1, 1);
   CUSTOM_GPIO_SET_OUTPUT(CUSTOM_GPIO_LED_LAN2, 1);
   //CUSTOM_GPIO_SET_OUTPUT(CUSTOM_GPIO_LED_LAN3, 1);
   //CUSTOM_GPIO_SET_OUTPUT(CUSTOM_GPIO_LED_LAN4, 1);

    //altered by caibing 2016-06-29
    #if 1
    /*MUX Select Value 45:41 | LED_LINK | 5-port Ethernet switch indicator LEDs*/     
    CUSTOM_GPIO_FUNCTION_SET(CUSTOM_GPIO_LED_LAN1, 41);    
    CUSTOM_GPIO_FUNCTION_SET(CUSTOM_GPIO_LED_LAN2, 42);    
    CUSTOM_GPIO_FUNCTION_SET(CUSTOM_GPIO_LED_LAN3, 43);    
    CUSTOM_GPIO_FUNCTION_SET(CUSTOM_GPIO_LED_LAN4, 44);    
    #endif
    custom_led_set_run(led_on);    
#endif
    return;
}

void custom_led_running(void)
{
#if 0
#if CUSTOM_BOARD_TA62 | CUSTOM_BOARD_TA37
    /*turn on led RUN*/
    custom_led_set_run(led_on);
    /*turn off led WLAN*/
    custom_led_set_wlan(led_off);
#endif

#if CUSTOM_BOARD_TA37 
    /*MUX Select Value 45:41 | LED_LINK | 5-port Ethernet switch indicator LEDs*/
    CUSTOM_GPIO_EN_OUTPUT(CUSTOM_GPIO_LED_WAN);
    CUSTOM_GPIO_EN_OUTPUT(CUSTOM_GPIO_LED_LAN1);
    CUSTOM_GPIO_EN_OUTPUT(CUSTOM_GPIO_LED_LAN2);
    CUSTOM_GPIO_EN_OUTPUT(CUSTOM_GPIO_LED_LAN3);
    CUSTOM_GPIO_EN_OUTPUT(CUSTOM_GPIO_LED_LAN4);
    
    //CUSTOM_GPIO_FUNCTION_SET(CUSTOM_GPIO_LED_WAN, 45);    
    CUSTOM_GPIO_FUNCTION_SET(CUSTOM_GPIO_LED_LAN1, 41);    
    CUSTOM_GPIO_FUNCTION_SET(CUSTOM_GPIO_LED_LAN2, 42);    
    CUSTOM_GPIO_FUNCTION_SET(CUSTOM_GPIO_LED_LAN3, 43);    
    CUSTOM_GPIO_FUNCTION_SET(CUSTOM_GPIO_LED_LAN4, 44);    
#endif

#if CUSTOM_BOARD_TA30_51
    /*turn on led RUN*/
    custom_led_set_run(led_on);
#endif
#endif
    return;
}

void custom_reset_default(void)
{
    char *cur_value;
    
    cur_value = getnvram("BOOT_DEFAULT");
    if ((cur_value== NULL) || strcmp(cur_value, "1")) {
        setnvram("BOOT_DEFAULT", "1");
        save_nvram_auto();
    }

    #if CUSTOM_BOARD_TA30_51
    custom_led_set_run(led_on);
    udelay(200000);
    custom_led_set_run(led_off);
    #else
    custom_led_set_wan_lan(led_on);
    udelay(200000);
    custom_led_set_wan_lan(led_off);
    #endif
    return;
}

void custom_check_button(void)
{
#if CUSTOM_BOARD_TA62 | CUSTOM_BOARD_TA37 | CUSTOM_BOARD_TA30_51
    unsigned int gpio = CUSTOM_GPIO_BUTTON_RST;
    unsigned int time_count = 0;

    CUSTOM_GPIO_EN_INPUT(gpio);

    while (CUSTOM_GPIO_IN_VAL(gpio) == (CUSTOM_BTN_RESET_ACTIVE << gpio)) {
        if (time_count >= CUSTOM_BUTTON_SW_RESET_TIMTOUT) {
            custom_reset_default();
            puts ("###  Factory reset by default key ###\n");
            break;
        }
        udelay(1000000);
        time_count++;
    }

#endif

    return;
}

int xra1403_up_clk(void)
{
    int uiRd = 0;

    uiRd = ar9344_reg_rd(ATH_SPI_CLOCK);
    uiRd = (uiRd &(~(0x3f)))|(3);
    ar9344_reg_wr(ATH_SPI_CLOCK,uiRd);

    return uiRd;
}
int xra1403_down_clk(void)
{
    int uiRd = 0;
    
    uiRd = ar9344_reg_rd(ATH_SPI_CLOCK);
    uiRd = (uiRd &(~(0x3f)))|(5);
    ar9344_reg_wr(ATH_SPI_CLOCK,uiRd);  

    return uiRd;

}

int xra1403_write(int uiReg, int uiValue)
{
	u8 cmd = 0x0; 		//command  8bits	
	u8 data = 0x0;		//8bit data

	cmd = (~(cmd | (COMMAND_RD << COMMAND_RDWR_SHIFT))) & ((uiReg & 0x1F) << COMMAND_BYTE_SHIFT);
	data = data | (uiValue & 0xFF);			//get data

	ath_reg_wr_nf(ATH_SPI_FS, 1);

    xra1403_down_clk();
	
	ath_reg_wr_nf(ATH_SPI_WRITE, ATH_SPI_CS_DIS);
	
	ath_spi_bit_banger(cmd);
	ath_spi_bit_banger(data);

	ath_reg_wr_nf(ATH_SPI_WRITE, ATH_SPI_CE_LOW);	

	ath_reg_wr_nf(ATH_SPI_WRITE, ATH_SPI_CS_DIS);

    xra1403_up_clk();

    ath_spi_done();
    
    return 0;
}

int xra1403_read(int uiReg, int* puiValue)		//reg = command_byte
{
    u8 cmd = 0x0;       //command 8bits
    cmd = cmd | 
            (COMMAND_RD << COMMAND_RDWR_SHIFT) 
            | ((uiReg & 0x1F) << COMMAND_BYTE_SHIFT);           //get command

    ath_reg_wr_nf(ATH_SPI_FS, 1);

    xra1403_down_clk();

    ar9344_reg_wr_nf(ATH_SPI_WRITE, ATH_SPI_CS_DIS);
    
    ath_spi_bit_banger(cmd);
    ath_spi_delay_8();
    
    ath_spi_go();
    *puiValue = 0xff & ath_reg_rd(ATH_SPI_RD_STATUS);
    
    xra1403_up_clk();
    ath_spi_done();

    return 0;
}

void gpioex_mode_all_get(int* puiValue)
{
    int  uiValueHigh = 0;
    int  uiValueLow = 0;

    xra1403_read(COMMAND_BYTE_GCR2,&uiValueHigh);
    xra1403_read(COMMAND_BYTE_GCR1,&uiValueLow);
    *puiValue = 0xffff & ((uiValueHigh << 8) | uiValueLow);    
    
    return ;
}

void gpioex_mode_get(int uiNum,int* puiValue)
{
    int uiValue = 0;
    
    gpioex_mode_all_get(&uiValue);
	
    *puiValue = (uiValue>>uiNum) & 1;
    
    return ;
}

void gpioex_state_all_get(int* puiValue)
{
    int  uiValueHigh = 0;
    int  uiValueLow = 0;

    xra1403_read(COMMAND_BYTE_GSR2,&uiValueHigh);
    xra1403_read(COMMAND_BYTE_GSR1,&uiValueLow);
    *puiValue = 0xffff & ((uiValueHigh << 8)|uiValueLow);

    return ;
}

void gpioex_stat_get(int uiNum,int* puiValue)
{
    int uiValue = 0;
    gpioex_state_all_get(&uiValue);
    *puiValue = (uiValue>>uiNum) & 1;
    
    return ;
}


void gpioex_mode_set(int uiNum,int uiValue)
{
    int iTmpValue = 0;
    
    gpioex_mode_all_get(&iTmpValue);
    
    if(uiNum < 8)
    {
        if(1 == uiValue)
        {
            iTmpValue = iTmpValue | 1<<uiNum;
        }
        else if(0 == uiValue)
        {
            iTmpValue = iTmpValue & (~(1<<uiNum));
        }
        uiValue = iTmpValue;
        xra1403_write(COMMAND_BYTE_GCR1,uiValue);
    }
    else if(uiNum < 16)
    {
        if(1 == uiValue)
        {
            iTmpValue = iTmpValue | 1<<uiNum;
        }
        else if(0 == uiValue)
        {
            iTmpValue = iTmpValue & (~(1<<uiNum));
        }
        uiValue = iTmpValue >> 8;
        xra1403_write(COMMAND_BYTE_GCR2,uiValue);        
    }
    else 
    {
        return ;
    }
    
    return ;
}


int gpioex_stat_set(int uiNum,int uiValue)
{
    int iTmpValue = 0;
    
    gpioex_state_all_get(&iTmpValue);

    if(uiNum < 8)
    {
        if(1 == uiValue)
        {
            iTmpValue = iTmpValue | 1<<uiNum;
        }
        else if(0 == uiValue)
        {
            iTmpValue = iTmpValue & (~(1<<uiNum));
        }
        uiValue = iTmpValue;
        xra1403_write(COMMAND_BYTE_OCR1,uiValue);
    }
    else if(uiNum < 16)
    {
        if(1 == uiValue)
        {
            iTmpValue = iTmpValue | 1<<uiNum;
        }
        else if(0 == uiValue)
        {
            iTmpValue = iTmpValue & (~(1<<uiNum));
        }
        uiValue = iTmpValue >> 8;
        xra1403_write(COMMAND_BYTE_OCR2,uiValue);        
    }
    else
    {
        return 0;
    }

    return 0;
}


void custom_gpioex_init()
{
	int iNum = 0;
	int iDir = 0;
	int iVal = 0;
	
	CUSTOM_GPIO_FUNCTION_SET(GPIO_EXPANDER_RESET, 0);
    CUSTOM_GPIO_SET_OUTPUT(GPIO_EXPANDER_RESET, 1);
    	
	CUSTOM_GPIO_FUNCTION_SET(GPIO_LTE_USB_BYPASS, 0);
    CUSTOM_GPIO_SET_OUTPUT(GPIO_LTE_USB_BYPASS, 1);
    
	//CUSTOM_GPIO_FUNCTION_SET(17, 0xb);
    //CUSTOM_GPIO_SET_OUTPUT(17, 1);
    
	//gpioex_mode_get(GPIO_EX1_NUM_15,&iDir);
	//gpioex_stat_get(GPIO_EX1_NUM_15,&iVal);
	//printf("[%s:%d] num: %d dir: %d val: %d\n",__FUNCTION__,__LINE__,GPIO_EX1_NUM_15,iDir,iVal);	
	
	gpioex_mode_set(GPIO_EX1_ATTENUATOR_SW,0);
	gpioex_stat_set(GPIO_EX1_ATTENUATOR_SW,1);
	
	gpioex_mode_get(GPIO_EX1_ATTENUATOR_SW,&iDir);
	gpioex_stat_get(GPIO_EX1_ATTENUATOR_SW,&iVal);
	printf("[%s:%d] GPIO_EX1_ATTENUATOR_SW=%d dir: %d val: %d\n",__FUNCTION__,__LINE__,GPIO_EX1_ATTENUATOR_SW,iDir,iVal);	
	
	/* only H3000
	printf("[%s:%d] delay 3s start !\n",__FUNCTION__,__LINE__);
	
	int i = 0;
	// delay 3s 
	// delay 100 * 10ms 
	for (i=0; i<100; ++i) {
		udelay (10000);
	}
	for (i=0; i<100; ++i) {
		udelay (10000);
	}
	for (i=0; i<100; ++i) {
		udelay (10000);
	}
	printf("[%s:%d] delay 3s end !\n",__FUNCTION__,__LINE__);	
	*/
	CUSTOM_GPIO_FUNCTION_SET(GPIO_LTE_USB_ENABLE, 0);
    CUSTOM_GPIO_SET_OUTPUT(GPIO_LTE_USB_ENABLE, 1);
	
	//get gpioex1 1 state delay 
	udelay(200*1000);
	
	gpioex_mode_get(GPIO_EX1_SIM_CARD_ST,&iDir);
	gpioex_stat_get(GPIO_EX1_SIM_CARD_ST,&iVal);
		
	printf("[%s:%d] GPIO_EX1_SIM_CARD_ST dir: %d val: %d\n",__FUNCTION__,__LINE__,iDir,iVal);
	
	gpioex_mode_set(GPIO_EX1_SIM_CARD_SW,0);
	if(0 == iVal)
	{
		gpioex_stat_set(GPIO_EX1_SIM_CARD_SW,0);//SIM A
	}else{
		gpioex_stat_set(GPIO_EX1_SIM_CARD_SW,1);//SIM B
	}
	
	gpioex_mode_get(GPIO_EX1_SIM_CARD_SW,&iDir);
	gpioex_stat_get(GPIO_EX1_SIM_CARD_SW,&iVal);
	
	printf("[%s:%d] GPIO_EX1_SIM_CARD_SW dir: %d val: %d\n",__FUNCTION__,__LINE__,iDir,iVal);
	
	return ;
}

void custom_poweron_init(void)
{
    custom_led_init();

    /*2012-12-26 zhangping add start*/
    //del by xiaowei 2016-3-15
    //ddr_test_waiting_key();
    /*2012-12-26 zhangping add end */
	
    return;
}

void custom_main(void)
{
    nvram_relocate();
    get_boot_info();
    update_flashimage();
    //altered by caibing 2016-7-05
    //custom_check_button();
    ping_restore_monitor();
    modify_bootargs();
    //altered by caibing 2016-7-05
    //custom_led_running();
    
    //add by xiaowei 2016-9-2 gpio expander
	custom_gpioex_init();
    
    return;
}



