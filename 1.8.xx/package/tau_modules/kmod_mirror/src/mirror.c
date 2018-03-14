/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : mirror.c
  版 本 号   : 初稿
  作    者   : xiao wei
  生成日期   : 2016年3月6日 星期三
  最近修改   :
  功能描述   : 端口镜像模块
  函数列表   :
  修改历史   :
  1.日    期   : 2016年3月6日 星期三
    作    者   : xiao wei
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/proc_fs.h>

#include "procfs_fops.h"
#include "mirror_ioctl.h"
#include "mirror.h"

/*----------------------------------------------*
 * 宏定义                                *
 *----------------------------------------------*/
#define	MIRROR_ENABLE_BIT_13				(1<<13)
#define	MIRROR_PORT_MIRROR_BIT_4_7			(0X000F)
#define	MIRROR_OBSERVE_PORT_RX_BIT_16		(1<<16)
#define	MIRROR_OBSERVE_PORT_TX_BIT_17		(1<<17)

#define ATH_MIRROR_OBSERVE_PORT0_OFFSET		0x0100

#define	ATHR_GMAC_ETH_CFG_BIT_6				(1<<6)
#define	ATHR_GMAC_ETH_CFG_BIT_7				(1<<7)


//extern unsigned int ath_switch_reg_read(unsigned int s27_addr);
//extern void ath_switch_reg_write(unsigned int s27_addr, unsigned int s27_write_data);
extern unsigned int ath_global_reg_read(unsigned int uiRegaddr);
extern unsigned int ath_switch_reg_read(unsigned int uiRegaddr);
extern void ath_switch_reg_write(unsigned int uiRegaddr, unsigned int uiRegval);


AP_ERROR_CODE_E mirror_ioctl_port(MIRROR_IOCTL_DATE_T *pstMirrSw)
{
	UINT32 uiRegValue    = 0;
	UINT32 uiGmacRegBit6 = 0;
	UINT32 uiGmacRegBit7 = 0;

	uiRegValue = ath_global_reg_read(ATHR_GMAC_ETH_CFG);
	MIRROR_LOG(MIRROR_LOG_INFO, "ar7240_reg_rd(0x%0x)=0x%x\n",ATHR_GMAC_ETH_CFG,uiRegValue);
	uiGmacRegBit6 = ath_global_reg_read(ATHR_GMAC_ETH_CFG) & ATHR_GMAC_ETH_CFG_BIT_6;
	uiGmacRegBit7 = ath_global_reg_read(ATHR_GMAC_ETH_CFG) & ATHR_GMAC_ETH_CFG_BIT_7;

	if ( 0x00 == uiGmacRegBit6 && 0x00 == uiGmacRegBit7 )
	{
		pstMirrSw->stGmacStatus.enPortSta = MIRROR_IOCTL_GMAC_SW_NO_MODE;
		// Modify: by xiaowei, 2016/04/19 09:48 PN: ar9531 only port1-4
		pstMirrSw->stGmacStatus.uiStartPort = MIRROR_PORT1;
		//pstMirrSw->stGmacStatus.uiStartPort = MIRROR_PORT2;
		pstMirrSw->stGmacStatus.uiEndPort = MIRROR_PORT4;
		//pstMirrSw->stGmacStatus.uiEndPort = MIRROR_PORT5;
	}
	else if ( 0x00 != uiGmacRegBit6 && 0x00 == uiGmacRegBit7 )
	{
		pstMirrSw->stGmacStatus.enPortSta = MIRROR_IOCTL_GMAC_SW_ONLY_MODE;
		// Modify: by xiaowei, 2016/04/19 09:48 PN: ar9531 only port1-4
		pstMirrSw->stGmacStatus.uiStartPort = MIRROR_PORT1;
		//pstMirrSw->stGmacStatus.uiStartPort = MIRROR_PORT2;
		pstMirrSw->stGmacStatus.uiEndPort = MIRROR_PORT4;
		//pstMirrSw->stGmacStatus.uiEndPort = MIRROR_PORT5;
	}
	else if ( 0x00 == uiGmacRegBit6 && 0x00 != uiGmacRegBit7 )
	{
		pstMirrSw->stGmacStatus.enPortSta = MIRROR_IOCTL_GMAC_SW_PHY_SWAP;
		pstMirrSw->stGmacStatus.uiStartPort = MIRROR_PORT2;
		pstMirrSw->stGmacStatus.uiEndPort = MIRROR_PORT4;
	}
	else
	{
		pstMirrSw->stGmacStatus.enPortSta = MIRROR_IOCTL_GMAC_SW_ONLY_MODE;
		pstMirrSw->stGmacStatus.uiStartPort = MIRROR_PORT2;
		// Modify: by xiaowei, 2016/04/19 09:48 PN: ar9531 only port2-4
		pstMirrSw->stGmacStatus.uiEndPort = MIRROR_PORT4;
		//pstMirrSw->stGmacStatus.uiEndPort = MIRROR_PORT5;
	}
	//MIRROR_LOG(MIRROR_LOG_INFO, "start port =%d\n",pstMirrSw->stGmacStatus.uiStartPort);
	//MIRROR_LOG(MIRROR_LOG_INFO, "End port =%d\n",pstMirrSw->stGmacStatus.uiEndPort);
	return AP_E_NONE;
}

AP_ERROR_CODE_E mirror_ioctl_show(MIRROR_IOCTL_DATE_T *pstMirrSw)
{
	UINT32 uiRegPortAdd = 0;
	UINT32 uiRegValue   = 0;
	UINT32 uiRegRxValue = 0;
	UINT32 uiRegTxValue = 0;
	UINT32 uiLoop       = 0;

	mirror_ioctl_port(pstMirrSw);

	uiRegValue = ath_switch_reg_read(ATH_MIRROR_ENABLE_ADDRESS) & MIRROR_ENABLE_BIT_13;
	//MIRROR_LOG(MIRROR_LOG_INFO, "ath_switch_reg_read(ATH_MIRROR_ENABLE_ADDRESS)=0x%0x\n", ath_switch_reg_read(ATH_MIRROR_ENABLE_ADDRESS));
	pstMirrSw->uiMirrorSw = uiRegValue;

	uiRegValue = (ath_switch_reg_read(ATH_MIRROR_MIRRING_PORT_ADDRESS) >> 4) & MIRROR_PORT_MIRROR_BIT_4_7;
	MIRROR_LOG(MIRROR_LOG_INFO, "ath_switch_reg_read(ATH_MIRROR_MIRRING_PORT_ADDRESS)=0x%0x\n", ath_switch_reg_read(ATH_MIRROR_MIRRING_PORT_ADDRESS));
	pstMirrSw->uiMirrorPort = uiRegValue;

	for( uiLoop = pstMirrSw->stGmacStatus.uiStartPort; uiLoop <= pstMirrSw->stGmacStatus.uiEndPort; uiLoop++ )
	{
		uiRegPortAdd = ATH_MIRROR_OBSERVE_PORT0_ADDRESS + uiLoop*ATH_MIRROR_OBSERVE_PORT0_OFFSET;

		MIRROR_LOG(MIRROR_LOG_INFO, "ath_switch_reg_read(ATH_MIRROR_OBSERVE_PORT[%d]_ADDRESS)=0x%0x\n", uiLoop, ath_switch_reg_read(uiRegPortAdd));
		//MIRROR_LOG(MIRROR_LOG_INFO, "ath_switch_reg_read(uiRegPortAdd)=0x%0x\n", uiRegPortAdd);
		uiRegRxValue = (ath_switch_reg_read(uiRegPortAdd) & MIRROR_OBSERVE_PORT_RX_BIT_16) >> 16;
		uiRegTxValue = (ath_switch_reg_read(uiRegPortAdd) & MIRROR_OBSERVE_PORT_TX_BIT_17) >> 17;

		//MIRROR_LOG(MIRROR_LOG_INFO, "bit16: uiRegRxValue=0x%0x\n", uiRegRxValue);
		//MIRROR_LOG(MIRROR_LOG_INFO, "bit17: uiRegTxValue=0x%0x\n", uiRegTxValue);
		if( 0x00 == uiRegRxValue && 0x00 == uiRegTxValue)
		{
			pstMirrSw->stObservePort[uiLoop].enObservePortCurr = MIRROR_IOCTL_OBSERVE_PORT_NO;
		}
		if( 0x01 == uiRegRxValue && 0x00 == uiRegTxValue)
		{
			pstMirrSw->stObservePort[uiLoop].enObservePortCurr = MIRROR_IOCTL_OBSERVE_PORT_RX;
		}
		if( 0x00 == uiRegRxValue && 0x01 == uiRegTxValue)
		{
			pstMirrSw->stObservePort[uiLoop].enObservePortCurr = MIRROR_IOCTL_OBSERVE_PORT_TX;
		}
		if( 0x01 == uiRegRxValue && 0x01 == uiRegTxValue)
		{
			pstMirrSw->stObservePort[uiLoop].enObservePortCurr = MIRROR_IOCTL_OBSERVE_PORT_ALL;
		}
	}
	
	
	return AP_E_NONE;
}		

AP_ERROR_CODE_E mirror_ioctl_switch(MIRROR_IOCTL_DATE_T *pstMirrSw)
{
	UINT32 uiRegValue = 0;
	if( MIRROR_DISABLE != pstMirrSw->uiMirrorSw && MIRROR_ENABLE != pstMirrSw->uiMirrorSw )
	{
		MIRROR_LOG(MIRROR_LOG_ERR, "mirror_ioctl_enable set data error.\n");
		pstMirrSw->uiSetStatu = MIRROR_SET_FAILED;
		return AP_E_DEFAULT_FAIL;
	}
	uiRegValue = ath_switch_reg_read(ATH_MIRROR_ENABLE_ADDRESS);
	MIRROR_LOG(MIRROR_LOG_INFO, "ATH_MIRROR_ENABLE_ADDRESS=uiRegValue=0x%0x\n", uiRegValue);

	if( MIRROR_ENABLE == pstMirrSw->uiMirrorSw )
	{
		uiRegValue |= MIRROR_ENABLE_BIT_13;
		MIRROR_LOG(MIRROR_LOG_INFO, "uiRegValue=0x%0x\n", uiRegValue);

		ath_switch_reg_write(ATH_MIRROR_ENABLE_ADDRESS, uiRegValue);
		MIRROR_LOG(MIRROR_LOG_INFO, "mirror enable ok .\n");
	}
	else
	{
		uiRegValue &= ~ MIRROR_ENABLE_BIT_13;
		MIRROR_LOG(MIRROR_LOG_INFO, "uiRegValue=0x%0x\n", uiRegValue);

		ath_switch_reg_write(ATH_MIRROR_ENABLE_ADDRESS, uiRegValue);
		MIRROR_LOG(MIRROR_LOG_INFO, "mirror disable ok .\n");
	}
	pstMirrSw->uiSetStatu = MIRROR_SET_SUCCESS;

	return AP_E_NONE;
}

AP_ERROR_CODE_E mirror_ioctl_setmirror(MIRROR_IOCTL_DATE_T *pstMirrSw)
{
	UINT32 uiRegValue = 0;
	//UINT32 uiRegWriteVlaue = 0;

	if( pstMirrSw->uiMirrorPort > MIRROR_PORT_CLOSE || pstMirrSw->uiMirrorPort <= MIRROR_PORT0)
	{
		MIRROR_LOG(MIRROR_LOG_ERR, "mirror_ioctl_setmirror set mirror port error.\n");
		pstMirrSw->uiSetStatu = MIRROR_SET_FAILED;
		return AP_E_DEFAULT_FAIL;
	}

	if( MIRROR_PORT_CLOSE == pstMirrSw->uiMirrorPort ) 
	{
		pstMirrSw->uiMirrorPort = 0xffff;
	}

	uiRegValue = pstMirrSw->uiMirrorPort << 4;
	//MIRROR_LOG(MIRROR_LOG_INFO, "user :uiRegValue=0x%0x\n", uiRegValue);

	uiRegValue = (ath_switch_reg_read(ATH_MIRROR_MIRRING_PORT_ADDRESS) & 0Xffffff0f) | (pstMirrSw->uiMirrorPort << 4);

	//MIRROR_LOG(MIRROR_LOG_INFO, "ATH_MIRROR_MIRRING_PORT_ADDRESS=0x%0x\n", ath_switch_reg_read(ATH_MIRROR_MIRRING_PORT_ADDRESS));
	MIRROR_LOG(MIRROR_LOG_INFO, "ATH_MIRROR_MIRRING_PORT_ADDRESS=uiRegValue=0x%0x\n", uiRegValue);

	ath_switch_reg_write(ATH_MIRROR_MIRRING_PORT_ADDRESS, uiRegValue);

	//MIRROR_LOG(MIRROR_LOG_INFO, "mirror setmirror ok .\n");

	pstMirrSw->uiSetStatu = MIRROR_SET_SUCCESS;

	return AP_E_NONE;
}

AP_ERROR_CODE_E mirror_ioctl_setobserve(MIRROR_IOCTL_DATE_T *pstMirrSw)
{
	UINT32 uiRegValue   = 0;
	UINT32 uiRegAddress = 0;
	UINT32 uiRegRead    = 0;

	if( pstMirrSw->uiObservePort > MIRROR_PORT5 || pstMirrSw->uiObservePort <= MIRROR_PORT0 )
	{
		MIRROR_LOG(MIRROR_LOG_ERR, "mirror_ioctl_setobserve set observe port error.\n");
		pstMirrSw->uiSetStatu = MIRROR_SET_FAILED;
		return AP_E_DEFAULT_FAIL;
	}

	//MIRROR_LOG(MIRROR_LOG_INFO, "ATH_MIRROR_OBSERVE_PORT_ADDRESS = 0x%x \n",ATH_MIRROR_OBSERVE_PORT0_ADDRESS + (pstMirrSw->uiObservePort) * ATH_MIRROR_OBSERVE_PORT0_OFFSET);
	uiRegAddress = ATH_MIRROR_OBSERVE_PORT0_ADDRESS + (pstMirrSw->uiObservePort) * ATH_MIRROR_OBSERVE_PORT0_OFFSET;

	if( MIRROR_IOCTL_OBSERVE_PORT_RX == pstMirrSw->stObservePort[pstMirrSw->uiObservePort].enObservePortCurr )
	{
		uiRegValue |= MIRROR_OBSERVE_PORT_RX_BIT_16;
		uiRegValue &= ~ MIRROR_OBSERVE_PORT_TX_BIT_17;
	}
	else if ( MIRROR_IOCTL_OBSERVE_PORT_TX == pstMirrSw->stObservePort[pstMirrSw->uiObservePort].enObservePortCurr )
	{
		uiRegValue |= MIRROR_OBSERVE_PORT_TX_BIT_17;
		uiRegValue &= ~ MIRROR_OBSERVE_PORT_RX_BIT_16;
	}
	else if ( MIRROR_IOCTL_OBSERVE_PORT_ALL == pstMirrSw->stObservePort[pstMirrSw->uiObservePort].enObservePortCurr )
	{
		uiRegValue |= MIRROR_OBSERVE_PORT_RX_BIT_16;
		uiRegValue |= MIRROR_OBSERVE_PORT_TX_BIT_17;
	}
	else
	{
		uiRegValue &= ~ MIRROR_OBSERVE_PORT_RX_BIT_16;
		uiRegValue &= ~ MIRROR_OBSERVE_PORT_TX_BIT_17;
	}
	//MIRROR_LOG(MIRROR_LOG_INFO,"user bit :uiRegValue=0x%0x\n", uiRegValue);

	uiRegRead = ath_switch_reg_read(uiRegAddress);
	//MIRROR_LOG(MIRROR_LOG_INFO,"user read:uiRegRead=0x%0x\n", uiRegRead);

	uiRegRead &= ~ MIRROR_OBSERVE_PORT_RX_BIT_16;
	uiRegRead &= ~ MIRROR_OBSERVE_PORT_TX_BIT_17;

	uiRegValue |= uiRegRead;
	MIRROR_LOG(MIRROR_LOG_INFO,"write :uiRegValue=0x%0x\n", uiRegValue);

	ath_switch_reg_write(uiRegAddress, uiRegValue);

	MIRROR_LOG(MIRROR_LOG_INFO, "mirror setmirror ok .\n");

	pstMirrSw->uiSetStatu = MIRROR_SET_SUCCESS;

	return AP_E_NONE;
}

AP_ERROR_CODE_E mirror_ioctl_handler(void *pstArg)
{
	LOCAL MIRROR_IOCTL_T stIoctl = {0};
	BOOL bGetOps = FALSE;  
	AP_ERROR_CODE_E enRet = AP_E_NONE;
	
	if (NULL == pstArg)
	{
		return AP_E_PARAM;
	}
	
	if (copy_from_user(&stIoctl, pstArg, sizeof(MIRROR_IOCTL_T)))
	{
		MIRROR_LOG(MIRROR_LOG_ERR, "mirror_ioctl_handler Copy from user failed !\n");
		return AP_E_DEFAULT_FAIL;
	}
	
	switch (stIoctl.enCmd)
	{
		case MIRROR_IOCTL_SHOW: 
			enRet = mirror_ioctl_show(&stIoctl.stMirrorData);
			bGetOps = TRUE; 
			break;         

		case MIRROR_IOCTL_ENABLE: 
		case MIRROR_IOCTL_DISABLE: 
			enRet = mirror_ioctl_switch(&stIoctl.stMirrorData); 
			bGetOps = TRUE; 
			break;
			
		case MIRROR_IOCTL_MIRROR: 
			enRet = mirror_ioctl_setmirror(&stIoctl.stMirrorData); 
			bGetOps = TRUE; 
			break;

   		case MIRROR_IOCTL_OBSERVE: 
			enRet = mirror_ioctl_setobserve(&stIoctl.stMirrorData); 
			bGetOps = TRUE; 
			break;

		default:
			printk("Unknown cmd %d\n", stIoctl.enCmd);
			return  AP_E_PARAM;
	}
	
	if (AP_E_NONE != enRet)
	{
		return AP_E_DEFAULT_FAIL;
	}
	
	if (bGetOps)
	{
		if (copy_to_user(pstArg, &stIoctl, sizeof(MIRROR_IOCTL_T)))
		{
			MIRROR_LOG(MIRROR_LOG_ERR, "Copy to user failed.\n");
			return AP_E_DEFAULT_FAIL;
		}
	}
	
	return AP_E_NONE;
}

void mirror_init(void)
{
	MIRROR_LOG(MIRROR_LOG_NOTICE, "mirror_init !\n");
	return ;
}

void mirror_destroy(void)
{
	MIRROR_LOG(MIRROR_LOG_NOTICE, "mirror_destroy !\n");
	return ;
}

LOCAL int mirror_init_module(void)
{
	mirror_init();
	
	procfs_fops_hook_set(mirror_ioctl_handler);
	procfs_fops_init(MIRROR_PROC_FILE);
	
	return 0;
}

LOCAL void  mirror_cleanup_module(void)
{

	procfs_fops_destroy(MIRROR_PROC_FILE);
	procfs_fops_hook_set(NULL);
	
	mirror_destroy();
	
	return;
}

module_init(mirror_init_module);
module_exit(mirror_cleanup_module);

MODULE_LICENSE("GPL");