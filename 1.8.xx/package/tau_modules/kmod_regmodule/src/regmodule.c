/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : regmodule.c
  版 本 号   : 初稿
  作    者   : xiao wei
  生成日期   : 2016年3月6日 星期三
  最近修改   :
  功能描述   : 
  函数列表   :
  修改历史   :
  1.日    期   : 2016年3月6日 星期三
    作    者   : xiao wei
    修改内容   : 创建文件

******************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>

#include "ar71xx_regs.h"
#include "ar7240.h"
#include "procfs_fops.h"
#include "reg_ioctl.h"
#include "athrs27_phy.h"

#define ATH_MAC0_REG_ADDR					AR71XX_GE0_BASE
#define ATH_MAC1_REG_ADDR					AR71XX_GE1_BASE

#define ATH_GLOBAL_REG_READ(_reg)			ar7240_reg_rd(_reg)
#define ATH_GLOBAL_REG_WRITE(_reg, _val)	ar7240_reg_wr(_reg, _val)

#define ath_sw_reg_rd(_reg)					(ATH_GLOBAL_REG_READ(ATH_MAC1_REG_ADDR + (_reg)))
#define ath_sw_reg_wr(_reg, _val)			ATH_GLOBAL_REG_WRITE(ATH_MAC1_REG_ADDR + (_reg), (_val))

#define ATH_SW_ADDR_SHIFT					8
#define ATH_SW_MII_MGMT_IND					0x34
#define ATH_SW_MII_MGMT_CMD					0x24
#define ATH_SW_MII_MGMT_ADDRESS				0x28
#define ATH_SW_MGMT_CMD_READ				0x01
#define ATH_SW_MII_MGMT_STATUS				0x30
#define ATH_SW_MII_MGMT_CTRL				0x2c

unsigned int ath_global_reg_read(unsigned int uiRegaddr)
{
	return ATH_GLOBAL_REG_READ(uiRegaddr);
}
void ath_global_reg_write(unsigned int uiRegaddr, unsigned int uiRegval)
{
	ATH_GLOBAL_REG_WRITE(uiRegaddr, uiRegval);
	return ;
}
EXPORT_SYMBOL(ath_global_reg_read);
EXPORT_SYMBOL(ath_global_reg_write);

//unsigned short uint16_t; uint32_t unsigned int; uint8_t unsigned char
unsigned short ath_sw_mii_read(int unit, uint32_t phy_addr, uint8_t reg)
{
    uint16_t      addr  = (phy_addr << ATH_SW_ADDR_SHIFT) | reg, val = 0;
    volatile int           rddata;
    uint16_t      ii = 0x1000;

    do
    {
        udelay(5);
        rddata = ath_sw_reg_rd(ATH_SW_MII_MGMT_IND) & 0x1;
    }while(rddata && --ii);

    if (ii == 0)
		printk("ERROR:%s:%d transaction failed\n",__func__,__LINE__);

    ath_sw_reg_wr(ATH_SW_MII_MGMT_CMD, 0x0);
    ath_sw_reg_wr(ATH_SW_MII_MGMT_ADDRESS, addr);
    ath_sw_reg_wr(ATH_SW_MII_MGMT_CMD, ATH_SW_MGMT_CMD_READ);

    do
    {
        udelay(5);
        rddata = ath_sw_reg_rd(ATH_SW_MII_MGMT_IND) & 0x1;
    }while(rddata && --ii);

    if (ii == 0)
	printk("ERROR:%s:%d transaction failed\n",__func__,__LINE__);

    val = ath_sw_reg_rd(ATH_SW_MII_MGMT_STATUS);
    ath_sw_reg_wr(ATH_SW_MII_MGMT_CMD, 0x0);

    return val;
}

void ath_sw_mii_write(int unit, uint32_t phy_addr, uint8_t reg, uint16_t data)
{
    uint16_t      addr  = (phy_addr << ATH_SW_ADDR_SHIFT) | reg;
    volatile int rddata;
    uint16_t      ii = 0x1000;

    do
    {
        udelay(5);
        rddata = ath_sw_reg_rd(ATH_SW_MII_MGMT_IND) & 0x1;
    }while(rddata && --ii);

    if (ii == 0)
		printk("ERROR:%s:%d transaction failed\n",__func__,__LINE__);

    ath_sw_reg_wr(ATH_SW_MII_MGMT_CMD, 0x0);

    ath_sw_reg_wr(ATH_SW_MII_MGMT_ADDRESS, addr);
    ath_sw_reg_wr(ATH_SW_MII_MGMT_CTRL, data);

    do
    {
        rddata = ath_sw_reg_rd(ATH_SW_MII_MGMT_IND) & 0x1;
    }while(rddata && --ii);

    if (ii == 0)
	printk("ERROR:%s:%d transaction failed\n",__func__,__LINE__);
}

unsigned int ar7240_sw_reg_read(unsigned int s27_addr)
{
    unsigned int addr_temp;
    unsigned int s27_rd_csr_low, s27_rd_csr_high, s27_rd_csr;
    unsigned int data,unit = 0;
    unsigned int phy_address, reg_address;

    addr_temp = s27_addr >>2;
    data = addr_temp >> 7;

    phy_address = 0x1f;
    reg_address = 0x10;

    if (is_ar7240()) {
        unit = 0;
    } 
    else if(is_ar7241() || is_ar7242() || is_ar934x()) {
        unit = 1;
    }

    ath_sw_mii_write(unit,phy_address, reg_address, data);

    phy_address = (0x17 & ((addr_temp >> 4) | 0x10));
    reg_address = ((addr_temp << 1) & 0x1e);
    s27_rd_csr_low = (uint32_t) ath_sw_mii_read(unit,phy_address, reg_address);

    reg_address = reg_address | 0x1;
    s27_rd_csr_high = (uint32_t) ath_sw_mii_read(unit,phy_address, reg_address);
    s27_rd_csr = (s27_rd_csr_high << 16) | s27_rd_csr_low ;

    return(s27_rd_csr);
}

void ar7240_sw_reg_write(unsigned int s27_addr, unsigned int s27_write_data)
{
    unsigned int addr_temp;
    unsigned int data;
    unsigned int phy_address, reg_address,unit = 0;

    addr_temp = (s27_addr ) >>2;
    data = addr_temp >> 7;

    phy_address = 0x1f;
    reg_address = 0x10;

    if (is_ar7240()) {
        unit = 0;
    }
    else if(is_ar7241() || is_ar7242() || is_ar934x()) {
        unit = 1;
    }

    ath_sw_mii_write(unit,phy_address, reg_address, data);

    phy_address = (0x17 & ((addr_temp >> 4) | 0x10));

    reg_address = (((addr_temp << 1) & 0x1e) | 0x1);
    data = (s27_write_data >> 16) & 0xffff;
    ath_sw_mii_write(unit,phy_address, reg_address, data);

    reg_address = ((addr_temp << 1) & 0x1e);
    data = s27_write_data  & 0xffff;
    ath_sw_mii_write(unit,phy_address, reg_address, data);
}

unsigned int ath_switch_reg_read(unsigned int uiRegaddr)
{
	return ar7240_sw_reg_read(uiRegaddr);
}
void ath_switch_reg_write(unsigned int uiRegaddr, unsigned int uiRegval)
{
	ar7240_sw_reg_write(uiRegaddr, uiRegval);
	return ;
}
EXPORT_SYMBOL(ath_switch_reg_read);
EXPORT_SYMBOL(ath_switch_reg_write);


unsigned int ath_phy_reg_read(unsigned int phy_addr, unsigned int reg_addr)
{
     unsigned int rddata, i = 100;
     unsigned long flags; 
     
     if(phy_addr >= S27_PHY_MAX) {
         printk("%s:Error invalid Phy Address:0x%x >= 5\n",__func__,phy_addr);
         return -1 ;
     }

     local_irq_save(flags); 

    /* MDIO_CMD is set for read */
    rddata = ath_switch_reg_read(S27_MDIO_CTRL_REGISTER);
    rddata = (rddata & 0x0) | (reg_addr << S27_MDIO_REG_ADDR) 
              | (phy_addr << S27_MDIO_PHY_ADDR) | S27_MDIO_CMD_RD
              | S27_MDIO_MASTER | S27_MDIO_BUSY ;

    ath_switch_reg_write(S27_MDIO_CTRL_REGISTER, rddata);

    rddata = ath_switch_reg_read(S27_MDIO_CTRL_REGISTER);
    rddata = rddata & S27_MDIO_BUSY;

    /* Check MDIO_BUSY status */
    while(rddata && --i){
        rddata = ath_switch_reg_read(S27_MDIO_CTRL_REGISTER);
        rddata = rddata & S27_MDIO_BUSY;
    }

    if(i <= 0)
      printk("ERROR:%s failed:phy:%d reg:%X rd_data:%X\n",
                __func__,phy_addr,reg_addr,rddata);
    /* Read the data from phy */

    rddata = ath_switch_reg_read(S27_MDIO_CTRL_REGISTER);
    rddata = rddata & 0xffff;
    
    local_irq_restore(flags); 
    
    return(rddata);
}

void ath_phy_reg_write(unsigned int phy_addr, unsigned int reg_addr, unsigned int write_data)
{
    unsigned int rddata,i = 100;
    unsigned long flags; 
    
    if(phy_addr >= S27_PHY_MAX) {
        printk("%s:Error invalid Phy Address:0x%x >= 5\n",__func__,phy_addr);
        return ;
    }

    local_irq_save(flags);
    
    /* MDIO_CMD is set for read */
    rddata = ath_switch_reg_read(S27_MDIO_CTRL_REGISTER);
    rddata = (rddata & 0x0) | (write_data & 0xffff)
               | (reg_addr << S27_MDIO_REG_ADDR) 
               | (phy_addr << S27_MDIO_PHY_ADDR) | S27_MDIO_CMD_WR
               | S27_MDIO_MASTER | S27_MDIO_BUSY;


    ath_switch_reg_write(S27_MDIO_CTRL_REGISTER, rddata);


    rddata = ath_switch_reg_read(S27_MDIO_CTRL_REGISTER);
    rddata = rddata & S27_MDIO_BUSY;

    /* Check MDIO_BUSY status */
    while(rddata && --i){
        rddata = ath_switch_reg_read(S27_MDIO_CTRL_REGISTER);
        rddata = rddata & S27_MDIO_BUSY;
    }
    
    local_irq_restore(flags); 
    
    if(i <= 0)
      printk("ERROR:%s failed:phy:%d reg%X\n",__func__,phy_addr,reg_addr);
}
EXPORT_SYMBOL(ath_phy_reg_read);
EXPORT_SYMBOL(ath_phy_reg_write);
int register_ioctl_read(REGISTER_IOCTL_DATE_T *pstRegRead)
{
	UINT32 uiRegValue = 0;
#if 0
	if( 0 == pstRegRead->uiRegAddr )
	{
		printk("register_ioctl_read set data error.\n");
		pstRegRead->uiRetValu = REGISTER_IOCTL_FAILED;
		return -1;
	}
#endif	
	if( REGISTER_IOCTL_ADDR_GLOBAL == pstRegRead->enRegType)
	{
		uiRegValue = ath_global_reg_read(pstRegRead->uiRegAddr);
		//printk("ath_global_reg_read uiRegValue=0x%x\n",uiRegValue);
	}
	else if(REGISTER_IOCTL_ADDR_SWITCH == pstRegRead->enRegType)
	{
		uiRegValue = ath_switch_reg_read(pstRegRead->uiRegAddr);
		//printk("ath_switch_reg_read uiRegValue=0x%x\n",uiRegValue);
	}
	else if(REGISTER_IOCTL_ADDR_PHY == pstRegRead->enRegType)
	{
		uiRegValue = ath_phy_reg_read(pstRegRead->uiRegPhy,pstRegRead->uiRegAddr);
		//printk("ath_switch_reg_read uiRegValue=0x%x\n",uiRegValue);
	}
	pstRegRead->uiRegValu = uiRegValue;
	pstRegRead->uiRetValu = REGISTER_IOCTL_SUCCESS;

	return 0;
}

int register_ioctl_write(REGISTER_IOCTL_DATE_T *pstRegRead)
{
	UINT32 uiRegValue = 0;
#if 0
	if( 0 == pstRegRead->uiRegAddr )
	{
		printk("register_ioctl_write set data error.\n");
		pstRegRead->uiRetValu = REGISTER_IOCTL_FAILED;
		return -1;
	}
#endif		
	if( REGISTER_IOCTL_ADDR_GLOBAL == pstRegRead->enRegType)
	{
		uiRegValue = ath_global_reg_read(pstRegRead->uiRegAddr);
		//printk("ath_global_reg_read uiRegValue=0x%x\n",uiRegValue);
		ath_global_reg_write(pstRegRead->uiRegAddr,pstRegRead->uiRegValu);
		//printk("ath_global_reg_write uiRegValue=0x%x pstRegRead->uiRegValu=0x%x\n",uiRegValue,pstRegRead->uiRegValu);
	}
	else if(REGISTER_IOCTL_ADDR_SWITCH == pstRegRead->enRegType)
	{
		uiRegValue = ath_switch_reg_read(pstRegRead->uiRegAddr);
		//printk("ath_switch_reg_write uiRegValue=0x%x\n",uiRegValue);
		ath_switch_reg_write(pstRegRead->uiRegAddr,pstRegRead->uiRegValu);
		//printk("ath_switch_reg_write uiRegValue=0x%x pstRegRead->uiRegValu=0x%x\n",uiRegValue,pstRegRead->uiRegValu);
	}
	else if(REGISTER_IOCTL_ADDR_PHY == pstRegRead->enRegType)
	{
		uiRegValue = ath_phy_reg_read(pstRegRead->uiRegPhy,pstRegRead->uiRegAddr);
		//printk("ath_phy_reg_read uiRegValue=0x%x\n",uiRegValue);
		ath_phy_reg_write(pstRegRead->uiRegPhy,pstRegRead->uiRegAddr,pstRegRead->uiRegValu);
		//printk("ath_phy_reg_write uiRegValue=0x%x pstRegRead->uiRegValu=0x%x\n",uiRegValue,pstRegRead->uiRegValu);	
	}
	
	uiRegValue = pstRegRead->uiRegValu;
	pstRegRead->uiRegValu = uiRegValue;
	pstRegRead->uiRetValu = REGISTER_IOCTL_SUCCESS;

	return 0;
}

int register_ioctl_handler(void *pstArg)
{
	LOCAL REGISTER_IOCTL_T stIoctl = {0};
	BOOL bGetOps = FALSE;  
	int enRet = 0;
	
	if (NULL == pstArg)
	{
		printk("register_ioctl_handler ioctl cmd/date Error !\n");
		return -1;
	}
	
	if (copy_from_user(&stIoctl, pstArg, sizeof(REGISTER_IOCTL_T)))
	{
		printk("register_ioctl_handler Copy from user failed !\n");
		return -1;
	}
	
	switch (stIoctl.enCmd)
	{
		case REGISTER_IOCTL_SHOW: 
			//enRet = register_ioctl_show(&stIoctl.stRegData);
			bGetOps = TRUE; 
			break;         

		case REGISTER_IOCTL_READ: 
			enRet = register_ioctl_read(&stIoctl.stRegData); 
			bGetOps = TRUE; 
			break;
			
		case REGISTER_IOCTL_WRITE: 
			enRet = register_ioctl_write(&stIoctl.stRegData); 
			bGetOps = TRUE; 
			break;

		default:
			printk("Unknown cmd %d\n", stIoctl.enCmd);
			return  -1;
	}
	
	if (AP_E_NONE != enRet)
	{
		printk("register_ioctl_handler ioctl command Error !\n");
		return -1;
	}
	
	if (bGetOps)
	{
		if (copy_to_user(pstArg, &stIoctl, sizeof(REGISTER_IOCTL_T)))
		{
			printk("register_ioctl_handler Copy to user failed !\n");
			return -1;
		}
		else
		{
			//printk("register_ioctl_handler Copy to user success !\n");
		}
	}
	
	return 0;
}

static int __init register_init(void)
{
	printk("register_init \n");
//	unsigned int uiRegGetValue;
//	uiRegGetValue = 0;
//	uiRegGetValue = ath_global_reg_read(0x18040008);
//	printk("uiRegGetValue = 0x%x \n",uiRegGetValue);
//	
//	uiRegGetValue = 0;
//	uiRegGetValue = ath_switch_reg_read(0x48);
//	printk("uiRegGetValue = 0x%x \n",uiRegGetValue);
	
	procfs_fops_hook_set(register_ioctl_handler);
	procfs_fops_init(REG_PROC_FILE);
	
	return 0;
}

static void __exit register_exit(void)
{
	procfs_fops_destroy(REG_PROC_FILE);
	procfs_fops_hook_set(NULL);
	printk("register_exit \n");
}

module_init(register_init);
module_exit(register_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiaowei");
