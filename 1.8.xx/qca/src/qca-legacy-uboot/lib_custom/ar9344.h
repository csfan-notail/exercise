#ifndef _AR9344_H
#define _AR9344_H

#include <asm/addrspace.h>

typedef unsigned int ar9344_reg_t;

#define ar9344_reg_rd(_phys)	(*(volatile ar9344_reg_t *)KSEG1ADDR(_phys))

#define ar9344_reg_wr_nf(_phys, _val) \
	((*(volatile ar9344_reg_t *)KSEG1ADDR(_phys)) = (_val))

#define ar9344_reg_wr(_phys, _val) do {	\
	ar9344_reg_wr_nf(_phys, _val);	\
	ar9344_reg_rd(_phys);		\
} while(0)

#define ar9344_reg_rmw_set(_reg, _mask)	do {			\
	ar9344_reg_wr((_reg), (ar9344_reg_rd((_reg)) | (_mask)));	\
	ar9344_reg_rd((_reg));					\
} while(0)

#define ar9344_reg_rmw_clear(_reg, _mask) do {			\
	ar9344_reg_wr((_reg), (ar9344_reg_rd((_reg)) & ~(_mask)));	\
	ar9344_reg_rd((_reg));					\
} while(0)



/*
 * Address map
 */
#define AR9344_APB_BASE			0x18000000	/* 384M */

/*
 * APB block
 */
#define AR9344_GPIO_BASE			AR9344_APB_BASE+0x00040000

/*
 * gpio configs
 */
#define AR9344_GPIO_OE			AR9344_GPIO_BASE+0x0
#define AR9344_GPIO_IN			AR9344_GPIO_BASE+0x4
#define AR9344_GPIO_OUT			AR9344_GPIO_BASE+0x8
#define AR9344_GPIO_SET			AR9344_GPIO_BASE+0xc
#define AR9344_GPIO_CLEAR			AR9344_GPIO_BASE+0x10
#define AR9344_GPIO_INT_ENABLE		AR9344_GPIO_BASE+0x14
#define AR9344_GPIO_INT_TYPE		AR9344_GPIO_BASE+0x18
#define AR9344_GPIO_INT_POLARITY		AR9344_GPIO_BASE+0x1c
#define AR9344_GPIO_INT_PENDING		AR9344_GPIO_BASE+0x20
#define AR9344_GPIO_INT_MASK		AR9344_GPIO_BASE+0x24
#define AR9344_GPIO_IN_ETH_SWITCH_LED	AR9344_GPIO_BASE+0x28
#define AR9344_GPIO_OUT_FUNCTION0		AR9344_GPIO_BASE+0x2c
#define AR9344_GPIO_OUT_FUNCTION1		AR9344_GPIO_BASE+0x30
#define AR9344_GPIO_OUT_FUNCTION2		AR9344_GPIO_BASE+0x34
#define AR9344_GPIO_OUT_FUNCTION3		AR9344_GPIO_BASE+0x38
#define AR9344_GPIO_OUT_FUNCTION4		AR9344_GPIO_BASE+0x3c
#define AR9344_GPIO_OUT_FUNCTION5		AR9344_GPIO_BASE+0x40
#define AR9344_GPIO_IN_ENABLE0		AR9344_GPIO_BASE+0x44
#define AR9344_GPIO_IN_ENABLE1		AR9344_GPIO_BASE+0x48
#define AR9344_GPIO_IN_ENABLE2		AR9344_GPIO_BASE+0x4c
#define AR9344_GPIO_IN_ENABLE3		AR9344_GPIO_BASE+0x50
#define AR9344_GPIO_IN_ENABLE4		AR9344_GPIO_BASE+0x54
#define AR9344_GPIO_IN_ENABLE5		AR9344_GPIO_BASE+0x58
#define AR9344_GPIO_IN_ENABLE6		AR9344_GPIO_BASE+0x5c
#define AR9344_GPIO_IN_ENABLE7		AR9344_GPIO_BASE+0x60
#define AR9344_GPIO_IN_ENABLE8		AR9344_GPIO_BASE+0x64
#define AR9344_GPIO_IN_ENABLE9		AR9344_GPIO_BASE+0x68
#define AR9344_GPIO_FUNCTIONS		AR9344_GPIO_BASE+0x6c


/* SPI, SLIC and GPIO are all multiplexed on gpio pins */
#define ATH_SPI_BASE			0x1f000000
#define ATH_SPI_FS		(ATH_SPI_BASE+0x00)
#define ATH_SPI_READ		(ATH_SPI_BASE+0x00)
#define ATH_SPI_CLOCK		(ATH_SPI_BASE+0x04)
#define ATH_SPI_WRITE		(ATH_SPI_BASE+0x08)
#define ATH_SPI_RD_STATUS	(ATH_SPI_BASE+0x0c)
#define ATH_SPI_SHIFT_DO	(ATH_SPI_BASE+0x10)
#define ATH_SPI_SHIFT_CNT	(ATH_SPI_BASE+0x14)
#define ATH_SPI_SHIFT_DI	(ATH_SPI_BASE+0x18)
#define ATH_SPI_D0_HIGH		(1<<0)	/* Pin spi_do */
#define ATH_SPI_CLK_HIGH	(1<<8)	/* Pin spi_clk */

#define ATH_SPI_CS_ENABLE_0	(6<<16)	/* Pin gpio/cs0 (active low) */
#define ATH_SPI_CS_ENABLE_1	(5<<16)	/* Pin gpio/cs1 (active low) */
#define ATH_SPI_CS_ENABLE_2	(3<<16)	/* Pin gpio/cs2 (active low) */
//#define ATH_SPI_CS_DIS	(ATH_SPI_CS_ENABLE_0|ATH_SPI_CS_ENABLE_1|ATH_SPI_CS_ENABLE_2)
#define ATH_SPI_CS_DIS		0x70000

#define ath_reg_wr_nf ar9344_reg_wr_nf
#define ath_reg_wr ar9344_reg_wr
#define ath_reg_rd ar9344_reg_rd
/*
 * SOC for flash
 */
#define ATH_SPI_CMD_WRITE_SR		0x01
#define ATH_SPI_CMD_WREN		0x06
#define ATH_SPI_CMD_RD_STATUS		0x05
#define ATH_SPI_CMD_FAST_READ		0x0b
#define ATH_SPI_CMD_PAGE_PROG		0x02
#define ATH_SPI_CMD_SECTOR_ERASE	0xd8

#define ATH_SPI_XRA_CMD_CONFIG		0x02

#define ATH_SPI_CE_LOW		0x30000
#define ATH_SPI_CE_HIGH		0x30100
#define ATH_SPI_SECTOR_SIZE	(1024*64)
#define ATH_SPI_PAGE_SIZE	256

#define ATH_FLASH_MAX_BANKS	1

#define display(_x)	ath_reg_wr_nf(0x18040008, (_x))

//  printk("_byte is %x \n",_byte); 
//		mdelay(10); 

#define ath_be_msb(_val, __i) (((_val) & (1 << (7 - __i))) >> (7 - __i))

#define ath_spi_bit_banger(_byte)	do {				\
	int _i;								\
	for(_i = 0; _i < 8; _i++) {					\
		ath_reg_wr_nf(ATH_SPI_WRITE,				\
			ATH_SPI_CE_LOW | ath_be_msb(_byte, _i));	\
		ath_reg_wr_nf(ATH_SPI_WRITE,				\
			ATH_SPI_CE_HIGH | ath_be_msb(_byte, _i));	\
	}								\
} while(0)

#define ath_spi_go()	do {				\
	ath_reg_wr_nf(ATH_SPI_WRITE, ATH_SPI_CE_LOW);	\
	ath_reg_wr_nf(ATH_SPI_WRITE, ATH_SPI_CS_DIS);	\
} while(0);


#define ath_spi_delay_8()	ath_spi_bit_banger(0)
#define ath_spi_done()		ath_reg_wr(ATH_SPI_FS, 0)

extern void ar9344_gpio_config_input(int gpio);

extern void ar9344_gpio_config_output(int gpio);

extern int ar9344_gpio_in_val(int gpio);

extern void  ar9344_gpio_drive_low(unsigned int mask);

extern void ar9344_gpio_drive_high(unsigned int mask);

#endif
