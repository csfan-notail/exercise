/*
 * Custom TA37 board support
 *
 * Copyright (c) 2013 The Linux Foundation. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include <linux/platform_device.h>
#include <linux/ath9k_platform.h>
#include <linux/ar8216_platform.h>

#include <asm/mach-ath79/ar71xx_regs.h>

#include "common.h"
#include "dev-eth.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-m25p80.h"
#include "dev-spi.h"
#include "dev-usb.h"
#include "dev-wmac.h"
#include "machtypes.h"
#include "pci.h"

#define TA37_GPIO_LED_WLAN		3   /*WLAN_2.4G_LED 1-on*/
#define TA37_GPIO_LED_SYSTEM	15  /*SYS_LED   1-on*/

#define TA37_GPIO_LED_WAN		11  /*WAN_LED   0-on*/
#define TA37_GPIO_LED_LAN1		14  /*LAN0_LED  0-on*/
#define TA37_GPIO_LED_LAN2		1  /*LAN1_LED  0-on*/
#define TA37_GPIO_LED_LAN3		11  /*LAN2_LED  0-on*/
#define TA37_GPIO_LED_LAN4		16  /*LAN3_LED  0-on*/

#define TA37_GPIO_BTN_RESET     3  /*SW_RST*/ 

#define TA37_KEYS_POLL_INTERVAL	20	/* msecs */
#define TA37_KEYS_DEBOUNCE_INTERVAL	(3 * TA37_KEYS_POLL_INTERVAL)

#define TA37_MAC0_OFFSET		0
#define TA37_MAC1_OFFSET		6
#define TA37_WMAC_CALDATA_OFFSET	0x1000


#define TA37_GPIO_SPI_CS1     2  /*SPI CS1*/ 
#define TA37_GPIO_SPI_CS2     17  /*SPI CS2*/ 

#define TA37_GPIO_MUX_SPI_CS1     2  /*GPIO OUTPUT MUX :SPICS1*/ 
extern void gpio_set_value(unsigned gpio, int value);


static struct gpio_led ta37_leds_gpio[] __initdata = {
    //altered by caibing 2016-08-30
    #if 0
    {
		.name		= "ta37:green:system",
		.gpio		= TA37_GPIO_LED_SYSTEM,
		.active_low	= 0,
	},
	#endif
    #if 0
	{
		.name		= "ta37:green:wlan",
		.gpio		= TA37_GPIO_LED_WLAN,
		.active_low	= 0,
	},
	#endif
    #if 0
	{
		.name		= "ta37:green:wan",
		.gpio		= TA37_GPIO_LED_WAN,
		.active_low	= 1,
	},
	#endif
	{
		.name		= "ta37:green:lan1",
		.gpio		= TA37_GPIO_LED_LAN1,
		.active_low	= 1,
	},
	{
		.name		= "ta37:green:lan2",
		.gpio		= TA37_GPIO_LED_LAN2,
		.active_low	= 1,
	},
	{
		.name		= "ta37:green:lan3",
		.gpio		= TA37_GPIO_LED_LAN3,
		.active_low	= 1,
	},
	{
		.name		= "ta37:green:lan4",
		.gpio		= TA37_GPIO_LED_LAN4,
		.active_low	= 1,
	}        
};
#if 0
static struct gpio_keys_button ta37_gpio_keys[] __initdata = {
	{
		.desc		= "reset button",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = TA37_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= TA37_GPIO_BTN_RESET,
		.active_low	= 0,
	},
};
#endif
/*
static void __init ta37_gpio_led_setup(void)
{
//	ath79_gpio_direction_select(TA37_GPIO_LED_WAN, true);
	ath79_gpio_direction_select(TA37_GPIO_LED_LAN1, true);
	ath79_gpio_direction_select(TA37_GPIO_LED_LAN2, true);
	ath79_gpio_direction_select(TA37_GPIO_LED_LAN3, true);
	ath79_gpio_direction_select(TA37_GPIO_LED_LAN4, true);

//	ath79_gpio_output_select(TA37_GPIO_LED_WAN,
//			QCA953X_GPIO_OUT_MUX_LED_LINK5);
	ath79_gpio_output_select(TA37_GPIO_LED_LAN1,
			QCA953X_GPIO_OUT_MUX_LED_LINK1);
	ath79_gpio_output_select(TA37_GPIO_LED_LAN2,
			QCA953X_GPIO_OUT_MUX_LED_LINK2);
	ath79_gpio_output_select(TA37_GPIO_LED_LAN3,
			QCA953X_GPIO_OUT_MUX_LED_LINK3);
	ath79_gpio_output_select(TA37_GPIO_LED_LAN4,
			QCA953X_GPIO_OUT_MUX_LED_LINK4);

	ath79_register_leds_gpio(-1, ARRAY_SIZE(ta37_leds_gpio),
			ta37_leds_gpio);
	ath79_register_gpio_keys_polled(-1, TA37_KEYS_POLL_INTERVAL,
			ARRAY_SIZE(ta37_gpio_keys),
			ta37_gpio_keys);
}
*/
static void __init ta37_gpio_led_setup(void)
{
    //altered by caibing 
    //ath79_gpio_direction_select(TA37_GPIO_LED_WAN, true);
	ath79_gpio_direction_select(TA37_GPIO_LED_LAN1, true);
	ath79_gpio_direction_select(TA37_GPIO_LED_LAN2, true);
	ath79_gpio_direction_select(TA37_GPIO_LED_LAN3, true);
	ath79_gpio_direction_select(TA37_GPIO_LED_LAN4, true);

    gpio_set_value(TA37_GPIO_LED_LAN1,0);
    gpio_set_value(TA37_GPIO_LED_LAN2,0);
    gpio_set_value(TA37_GPIO_LED_LAN3,0);
    gpio_set_value(TA37_GPIO_LED_LAN4,0);
    //altered by caibing 
	//ath79_gpio_output_select(TA37_GPIO_LED_WAN,
			//QCA953X_GPIO_OUT_MUX_LED_LINK5);
	ath79_gpio_output_select(TA37_GPIO_LED_LAN1,
			QCA953X_GPIO_OUT_MUX_LED_LINK1);
	ath79_gpio_output_select(TA37_GPIO_LED_LAN2,
			QCA953X_GPIO_OUT_MUX_LED_LINK2);
	ath79_gpio_output_select(TA37_GPIO_LED_LAN3,
			QCA953X_GPIO_OUT_MUX_LED_LINK3);
	ath79_gpio_output_select(TA37_GPIO_LED_LAN4,
			QCA953X_GPIO_OUT_MUX_LED_LINK4);

	ath79_register_leds_gpio(-1, ARRAY_SIZE(ta37_leds_gpio),
			ta37_leds_gpio);
    //alterd by caibing 2016-08-31
    #if 0
    ath79_register_gpio_keys_polled(-1, TA37_KEYS_POLL_INTERVAL,
			ARRAY_SIZE(ta37_gpio_keys),
			ta37_gpio_keys);
    #endif
}

static void __init ta37_setup(void)
{
	u8 *art = (u8 *) KSEG1ADDR(0x1fff0000);

	/*ta321-70 enable cs1 :gpio2 */
	/*disable jtag gpio pin2 */
	ath79_gpio_function_enable(AR934X_GPIO_FUNC_JTAG_DISABLE);

	ath79_gpio_direction_select(TA37_GPIO_SPI_CS1, true);
	ath79_gpio_output_select(TA37_GPIO_SPI_CS1,
			QCA953X_GPIO_OUT_MUX_SPI_CS1);

	ath79_gpio_direction_select(TA37_GPIO_SPI_CS2, true);
	ath79_gpio_output_select(TA37_GPIO_SPI_CS2,
			QCA953X_GPIO_OUT_MUX_SPI_CS2);

	ath79_register_m25p80(NULL);

	ta37_gpio_led_setup();

	ath79_register_usb();
	ath79_register_pci();

	ath79_register_wmac(art + TA37_WMAC_CALDATA_OFFSET, NULL);

	ath79_register_mdio(0, 0x0);
	ath79_register_mdio(1, 0x0);

	ath79_init_mac(ath79_eth0_data.mac_addr, art + TA37_MAC0_OFFSET, 0);
	ath79_init_mac(ath79_eth1_data.mac_addr, art + TA37_MAC1_OFFSET, 0);

	/* WAN port */
	ath79_eth0_data.phy_if_mode = PHY_INTERFACE_MODE_MII;
	ath79_eth0_data.speed = SPEED_100;
	ath79_eth0_data.duplex = DUPLEX_FULL;
	ath79_eth0_data.phy_mask = BIT(4);
	ath79_register_eth(0);

	/* LAN ports */
	ath79_eth1_data.phy_if_mode = PHY_INTERFACE_MODE_GMII;
	ath79_eth1_data.speed = SPEED_100;
	ath79_eth1_data.duplex = DUPLEX_FULL;
	ath79_switch_data.phy_poll_mask |= BIT(4);
	ath79_switch_data.phy4_mii_en = 1;
	ath79_register_eth(1);
}

MIPS_MACHINE(ATH79_MACH_CUSTOM_TA37, "ta37", "Custom TA37 board",
	     ta37_setup);
