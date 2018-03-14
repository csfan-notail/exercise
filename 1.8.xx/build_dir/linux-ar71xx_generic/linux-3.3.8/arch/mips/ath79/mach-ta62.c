
/*
 * Custom TA62 board support
 *
 * Copyright (c) 2014 The Linux Foundation. All rights reserved.
 * Copyright (c) 2012 Gabor Juhos <juhosg@openwrt.org>
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
#include "dev-m25p80.h"
#include "machtypes.h"
#include "pci.h"
#include "dev-eth.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-spi.h"
#include "dev-usb.h"
#include "dev-wmac.h"

#define TA62_GPIO_LED_WLAN             10
#define TA62_GPIO_LED_RUN              11

#define TA62_GPIO_BTN_RESET            5

#define TA62_KEYS_POLL_INTERVAL        20     /* msecs */
#define TA62_KEYS_DEBOUNCE_INTERVAL    (3 * TA62_KEYS_POLL_INTERVAL)

#define TA62_MAC0_OFFSET               0
#define TA62_WMAC_CALDATA_OFFSET       0x1000

#define TA62_GPIO_MDC			3
#define TA62_GPIO_MDIO			4

static struct gpio_led ta62_leds_gpio[] __initdata = {
	{
		.name		= "ta62:green:run",
		.gpio		= TA62_GPIO_LED_RUN,
		.active_low	= 0,
	},
	{
		.name		= "ta62:green:wlan",
		.gpio		= TA62_GPIO_LED_WLAN,
		.active_low	= 0,
	},
};

static struct gpio_keys_button ta62_gpio_keys[] __initdata = {
        {
                .desc           = "Reset button",
                .type           = EV_KEY,
                .code           = KEY_RESTART,
                .debounce_interval = TA62_KEYS_DEBOUNCE_INTERVAL,
                .gpio           = TA62_GPIO_BTN_RESET,
                .active_low     = 1,
        },
};

static struct ar8327_pad_cfg ta62_ar8033_pad0_cfg = {
	.mode = AR8327_PAD_MAC_SGMII,
	.sgmii_txclk_phase_sel = AR8327_SGMII_CLK_PHASE_RISE,
	.sgmii_rxclk_phase_sel = AR8327_SGMII_CLK_PHASE_FALL,
};

static struct ar8327_platform_data ta62_ar8033_data = {
	.pad0_cfg = &ta62_ar8033_pad0_cfg,
	.cpuport_cfg = {
		.force_link = 1,
		.speed = AR8327_PORT_SPEED_1000,
		.duplex = 1,
		.txpause = 1,
		.rxpause = 1,
	},
};

static struct mdio_board_info ta62_mdio0_info[] = {
	{
		.bus_id = "ag71xx-mdio.0",
		.phy_addr = 0,
		.platform_data = &ta62_ar8033_data,
	},
};

static void __init ta62_mdio_setup(void)
{
	ath79_gpio_output_select(TA62_GPIO_MDC, QCA956X_GPIO_OUT_MUX_GE0_MDC);
	ath79_gpio_output_select(TA62_GPIO_MDIO, QCA956X_GPIO_OUT_MUX_GE0_MDO);

	ath79_register_mdio(0, 0x0);
}

static void __init ta62_setup(void)
{
	u8 *art = (u8 *) KSEG1ADDR(0x1fff0000);

	ath79_register_m25p80(NULL);

	ath79_register_leds_gpio(-1, ARRAY_SIZE(ta62_leds_gpio),
			ta62_leds_gpio);
    ath79_register_gpio_keys_polled(-1, TA62_KEYS_POLL_INTERVAL,
                                    ARRAY_SIZE(ta62_gpio_keys),
                                    ta62_gpio_keys);

	ath79_register_usb();

	ta62_mdio_setup();

	mdiobus_register_board_info(ta62_mdio0_info,
				    ARRAY_SIZE(ta62_mdio0_info));

	ath79_register_wmac(art + TA62_WMAC_CALDATA_OFFSET, NULL);
	ath79_register_pci();

	/* GMAC0 is connected to an AR8033 */
	ath79_init_mac(ath79_eth0_data.mac_addr, art + TA62_MAC0_OFFSET, 0);
	ath79_eth0_data.phy_if_mode = PHY_INTERFACE_MODE_SGMII;
	ath79_eth0_data.speed = SPEED_1000;
	ath79_eth0_data.duplex = DUPLEX_FULL;
	ath79_eth0_data.phy_mask = BIT(0);
	/* modified for TA62: GMAC0 is connected to AR8033 instead, set force_link to 0*/
	ath79_eth0_data.force_link = 0;
	ath79_eth0_data.mii_bus_dev = &ath79_mdio0_device.dev;
	ath79_eth0_pll_data.pll_1000 = 0x06000000;
	ath79_register_eth(0);
}

MIPS_MACHINE(ATH79_MACH_CUSTOM_TA62, "ta62", "Custom TA62 board",
	     ta62_setup);
