/*
 * ar80xx.h: ar80xx PHY driver
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __AR80XX_H__
#define __AR80XX_H__
#define AR80XX_PHY_ID_AR8035 0x004dd072
#define ETH_XMII_ADDRESS_OFFSET 0x28
#define ATH_ETH_CFG_OFFSET 0x00

#define AR80XX_PHY_ID_MASK		0xffffffff
#define AR80XX_PHY_ID_AR8033		0x004dd074
#define ETH_SGMII_ADDRESS_OFFSET	0x48

#define AR80XX_REG_CHIP_CONFIG		0x1f
#define AR80XX_BT_BX_REG_SEL		0x8000	/* bit 15, select copper page register */
#define AR80XX_AUTO_NEGO		0x1000	/* bit 12, enable auto negotiation */

#define ETH_SGMII_GIGE_MSB		24
#define ETH_SGMII_GIGE_LSB		24
#define ETH_SGMII_GIGE_MASK		0x01000000
#define ETH_SGMII_GIGE_GET(x)		(((x) & ETH_SGMII_GIGE_MASK) >> ETH_SGMII_GIGE_LSB)
#define ETH_SGMII_GIGE_SET(x)		(((x) << ETH_SGMII_GIGE_LSB) & ETH_SGMII_GIGE_MASK)

#define ETH_SGMII_CLK_SEL_MSB		25
#define ETH_SGMII_CLK_SEL_LSB		25
#define ETH_SGMII_CLK_SEL_MASK		0x02000000
#define ETH_SGMII_CLK_SEL_GET(x)	(((x) & ETH_SGMII_CLK_SEL_MASK) >> ETH_SGMII_CLK_SEL_LSB)
#define ETH_SGMII_CLK_SEL_SET(x)	(((x) << ETH_SGMII_CLK_SEL_LSB) & ETH_SGMII_CLK_SEL_MASK)

#define ETH_SGMII_PHASE0_COUNT_MSB	7
#define ETH_SGMII_PHASE0_COUNT_LSB	0
#define ETH_SGMII_PHASE0_COUNT_MASK	0x000000ff
#define ETH_SGMII_PHASE0_COUNT_GET(x)	(((x) & ETH_SGMII_PHASE0_COUNT_MASK) >> ETH_SGMII_PHASE0_COUNT_LSB)
#define ETH_SGMII_PHASE0_COUNT_SET(x)	(((x) << ETH_SGMII_PHASE0_COUNT_LSB) & ETH_SGMII_PHASE0_COUNT_MASK)

#define ETH_SGMII_PHASE1_COUNT_MSB	15
#define ETH_SGMII_PHASE1_COUNT_LSB	8
#define ETH_SGMII_PHASE1_COUNT_MASK	0x0000ff00
#define ETH_SGMII_PHASE1_COUNT_GET(x)	(((x) & ETH_SGMII_PHASE1_COUNT_MASK) >> ETH_SGMII_PHASE1_COUNT_LSB)
#define ETH_SGMII_PHASE1_COUNT_SET(x)	(((x) << ETH_SGMII_PHASE1_COUNT_LSB) & ETH_SGMII_PHASE1_COUNT_MASK)

#define ETH_XMII_GIGE_MSB         25
#define ETH_XMII_GIGE_LSB          25
#define ETH_XMII_GIGE_MASK       0x02000000
#define ETH_XMII_GIGE_GET(x)      (((x) & ETH_XMII_GIGE_MASK) >> ETH_XMII_GIGE_LSB)
#define ETH_XMII_GIGE_SET(x)      (((x) << ETH_XMII_GIGE_LSB) & ETH_XMII_GIGE_MASK)

#define ETH_XMII_TX_INVERT_MSB 31
#define ETH_XMII_TX_INVERT_LSB 31
#define ETH_XMII_TX_INVERT_MASK 0x80000000
#define ETH_XMII_TX_INVERT_GET(x) (((x) & ETH_XMII_TX_INVERT_MASK) >> ETH_XMII_TX_INVERT_LSB)
#define ETH_XMII_TX_INVERT_SET(x) (((x) << ETH_XMII_TX_INVERT_LSB) & ETH_XMII_TX_INVERT_MASK)

#define ETH_XMII_RX_DELAY_MSB                                        29
#define ETH_XMII_RX_DELAY_LSB                                        28
#define ETH_XMII_RX_DELAY_MASK                                       0x30000000
#define ETH_XMII_RX_DELAY_GET(x)                                     (((x) & ETH_XMII_RX_DELAY_MASK) >> ETH_XMII_RX_DELAY_LSB)
#define ETH_XMII_RX_DELAY_SET(x)                                     (((x) << ETH_XMII_RX_DELAY_LSB) & ETH_XMII_RX_DELAY_MASK)

#define ETH_XMII_TX_DELAY_MSB                                        27
#define ETH_XMII_TX_DELAY_LSB                                        26
#define ETH_XMII_TX_DELAY_MASK                                       0x0c000000
#define ETH_XMII_TX_DELAY_GET(x)                                     (((x) & ETH_XMII_TX_DELAY_MASK) >> ETH_XMII_TX_DELAY_LSB)
#define ETH_XMII_TX_DELAY_SET(x)                                     (((x) << ETH_XMII_TX_DELAY_LSB) & ETH_XMII_TX_DELAY_MASK)

#define ETH_CFG_ETH_RXDV_DELAY_MSB                                   17
#define ETH_CFG_ETH_RXDV_DELAY_LSB                                   16
#define ETH_CFG_ETH_RXDV_DELAY_MASK                                  0x00030000
#define ETH_CFG_ETH_RXDV_DELAY_GET(x)                                (((x) & ETH_CFG_ETH_RXDV_DELAY_MASK) >> ETH_CFG_ETH_RXDV_DELAY_LSB)
#define ETH_CFG_ETH_RXDV_DELAY_SET(x)                                (((x) << ETH_CFG_ETH_RXDV_DELAY_LSB) & ETH_CFG_ETH_RXDV_DELAY_MASK)

#define ETH_CFG_ETH_RXD_DELAY_MSB                                    15
#define ETH_CFG_ETH_RXD_DELAY_LSB                                    14
#define ETH_CFG_ETH_RXD_DELAY_MASK                                   0x0000c000
#define ETH_CFG_ETH_RXD_DELAY_GET(x)                                 (((x) & ETH_CFG_ETH_RXD_DELAY_MASK) >> ETH_CFG_ETH_RXD_DELAY_LSB)
#define ETH_CFG_ETH_RXD_DELAY_SET(x)                                 (((x) << ETH_CFG_ETH_RXD_DELAY_LSB) & ETH_CFG_ETH_RXD_DELAY_MASK)

#define ETH_CFG_RGMII_GE0_MSB                                        0
#define ETH_CFG_RGMII_GE0_LSB                                        0
#define ETH_CFG_RGMII_GE0_MASK                                       0x00000001
#define ETH_CFG_RGMII_GE0_GET(x)                                     (((x) & ETH_CFG_RGMII_GE0_MASK) >> ETH_CFG_RGMII_GE0_LSB)
#define ETH_CFG_RGMII_GE0_SET(x)                                     (((x) << ETH_CFG_RGMII_GE0_LSB) & ETH_CFG_RGMII_GE0_MASK)

#endif
