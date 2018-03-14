/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : gpioex_ops.c
  �� �� ��   : ����
  ��    ��   : Zhong Huaqing
  ��������   : 2012��8��15�� ������
  ����޸�   :
  ��������   : 
  �����б�   :
  
  �޸���ʷ   :
  1.��    ��   : 2012��8��15�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "gpioex_main.h"

#include "gpioex_ioctl.h"
#include "gpioex_ops.h"
/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/
/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                       *
 *----------------------------------------------*/
 /*****************************************************************************
 �� �� ��  : xra1403_up_clk
 ��������  : ����gpio_expander��ʱ��Ƶ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : static
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��1��18�� ������
    ��    ��   : �¼�
    �޸�����   : �����ɺ���
  1.��    ��   : 2013��1��18�� ������
    ��    ��   : �̱�
    �޸�����   : ȥ������spi�����߼�

*****************************************************************************/
#if 0
int gpioex_spi_down(void)
{
    printk("g-s-d1\n");
//    return ath_flash_spi_down_trylock();
   return ath_flash_spi_down();

    printk("g-s-d2\n");
}
#else
 void gpioex_spi_down(void)
 {
 //    printk("g-s-d1\n");
 //    return ath_flash_spi_down_trylock();
     //ath_flash_spi_down();
 
 //    printk("g-s-d2\n");
 }
#endif
 void gpioex_spi_up(void)
 {
 //    printk("g-s-u1\n");
     //ath_flash_spi_up();
 //    printk("g-s-u2\n");
 }

 static UINT32 xra1403_up_clk(void)
{
    UINT32 uiRd = 0;

    /*altered by caibing 1-18*/
    //ath_reg_wr_nf(ATH_SPI_FS, 1);
//    ath_reg_wr_nf(ATH_SPI_WRITE, ATH_SPI_CS_DIS);          
    uiRd = ar9344_reg_rd(ATH_SPI_CLOCK);
    uiRd = (uiRd &(~(0x3f)))|(3);
    ar9344_reg_wr(ATH_SPI_CLOCK,uiRd);
    /*altered by caibing 1-18*/
    //ath_spi_done();

    return uiRd;
}
 /*****************************************************************************
 �� �� ��  : xra1403_down_clk
 ��������  : ����gpio_expander��ʱ��Ƶ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : static
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��1��18�� ������
    ��    ��   : �¼�
    �޸�����   : �����ɺ���
  1.��    ��   : 2013��1��18�� ������
    ��    ��   : �̱�
    �޸�����   : ȥ������spi�����߼�

*****************************************************************************/
static UINT32 xra1403_down_clk(void)
{
    UINT32 uiRd = 0;
    /*altered by caibing 1-18*/
    //ath_reg_wr_nf(ATH_SPI_FS, 1);
//    ath_reg_wr_nf(ATH_SPI_WRITE, ATH_SPI_CS_DIS);          
    uiRd = ar9344_reg_rd(ATH_SPI_CLOCK);
    uiRd = (uiRd &(~(0x3f)))|(5);
    ar9344_reg_wr(ATH_SPI_CLOCK,uiRd);  
    /*altered by caibing 1-18*/
    //ath_spi_done();

    return uiRd;

}

 static int xra1403_write(UINT32 uiReg, UINT32 uiValue)
{
	u8 cmd = 0x0; 		//command  8bits	
	u8 data = 0x0;		//8bit data


        //GPIOEX_LOG(GPIOEX_LOG_INFO, "Gpioex uiValue %2x.\n",uiValue);

    
	/*	
	COMMAND_WR << COMMAND_RDWR_SHIFT:���λ��0��ʾ������Ϊд������
	(reg & 0x1F) << COMMAND_BYTE_SHIFT):����ռ2~6,��5bits
	*/
	cmd = (~(cmd | (COMMAND_RD << COMMAND_RDWR_SHIFT))) & ((uiReg & 0x1F) << COMMAND_BYTE_SHIFT);
	data = data | (uiValue & 0xFF);			//get data


/* ��ȡflash spi���ź��� */
//    printk("gpioex spi write down\n");
//    ath_flash_spi_down();
#if 0
    if(0 != gpioex_spi_down())
    {
        return 0;
    }
#else
    gpioex_spi_down();
#endif    
    
//    printk("gpioex spi write down\n");
#if 1
/* ��ȡspi���� */
	ath_reg_wr_nf(ATH_SPI_FS, 1);

/*���ʱ�ӿ����߼� caibing 2013-1-18*/
    xra1403_down_clk();
	
/* ��Ƭѡ�ź�disable */
	ath_reg_wr_nf(ATH_SPI_WRITE, ATH_SPI_CS_DIS);
//    printk("gpioex spi write down\n");

/* ����16λ���ݣ������δ��� */
	ath_spi_bit_banger(cmd);
	ath_spi_bit_banger(data);

/*��������*/
	ath_reg_wr_nf(ATH_SPI_WRITE, ATH_SPI_CE_LOW);	

/* ��Ƭѡ�ź�disable */
	ath_reg_wr_nf(ATH_SPI_WRITE, ATH_SPI_CS_DIS);

/*���ʱ�ӿ����߼� caibing 2013-1-18*/
    xra1403_up_clk();

/* �ͷ�spi���� */
    ath_spi_done();
//    printk("gpioex spi write up\n");

#endif
    gpioex_spi_up();
/* �ͷ�flash spi���ź��� */
//    ath_flash_spi_up();
//    printk("gpioex spi write up\n");
    
    return 0;

}

/**
 * xra1403_read - Read back register content
 * @spi: The SPI device
 * @reg: Register offset
 */
static int xra1403_read(UINT32 uiReg,UINT32* puiValue)		//reg = command_byte
{
    u8 cmd = 0x0;       //command 8bits

    
/*	
COMMAND_WR << COMMAND_RDWR_SHIFT:���λ��0��ʾ������Ϊд������
(reg & 0x1F) << COMMAND_BYTE_SHIFT):����ռ2~6,��5bits
*/
    cmd = cmd | 
            (COMMAND_RD << COMMAND_RDWR_SHIFT) 
            | ((uiReg & 0x1F) << COMMAND_BYTE_SHIFT);           //get command


/* ��ȡflash spi���ź��� */

//    ath_flash_spi_down();
#if 0
     if(0 != gpioex_spi_down())
     {
         return 0;
     }
#else
    gpioex_spi_down();
#endif

#if 1     
//    printk("gpioex spi read down\n");
/* ��ȡspi���� */
    ath_reg_wr_nf(ATH_SPI_FS, 1);

/*���ʱ�ӿ����߼� caibing 2013-1-18*/
    xra1403_down_clk();

    
/* ��Ƭѡ�ź�disable */
    ar9344_reg_wr_nf(ATH_SPI_WRITE, ATH_SPI_CS_DIS);
    
/* ����16λ���ݣ������δ��� */
    ath_spi_bit_banger(cmd);
    ath_spi_delay_8();
    
/*��������*/
    ath_spi_go();
    *puiValue = 0xff & ath_reg_rd(ATH_SPI_RD_STATUS);
    
/*���ʱ�ӿ����߼� caibing 2013-1-18*/
    xra1403_up_clk();
/* ��Ƭѡ�ź�disable */
    ath_spi_done();
//    printk("gpioex spi read up\n");
/* �ͷ�flash spi���ź��� */
//    ath_flash_spi_up();

#endif
    gpioex_spi_up();
//    *puiValue = 0xff;

    //GPIOEX_LOG(GPIOEX_LOG_INFO, "Gpioex uiValue %2x.\n",*puiValue);
    
    return AP_E_NONE;

}



/*****************************************************************************
 �� �� ��  : gpioex_mode_all_set
 ��������  : ����������չgpio�ڵ�ģʽ������/�����
 �������  : UINT32 uiReg    
             UINT32 usValue  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��11��14��
    ��    ��   : �¼�
    �޸�����   : �����ɺ���

*****************************************************************************/
AP_ERROR_CODE_E gpioex_mode_all_set(UINT32 uiValue)
{

	UINT32  uiMode = uiValue;
    /*altered by caibing 1-18*/
	//xra1403_down_clk();
	
    xra1403_write(COMMAND_BYTE_GCR1,uiMode);
    uiMode = uiMode>>8;
    xra1403_write(COMMAND_BYTE_GCR2,uiMode); 
    /*altered by caibing 1-18*/
    //xra1403_up_clk();

    return AP_E_NONE;
}

/*****************************************************************************
 �� �� ��  : gpioex_mode_all_set
 ��������  : ��ȡ������չgpio�ڵ�ģʽ������/�����
 �������  : UINT32 uiReg    
             UINT32 usValue  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��11��14��
    ��    ��   : �¼�
    �޸�����   : �����ɺ���

*****************************************************************************/
AP_ERROR_CODE_E gpioex_mode_all_get(UINT32* puiValue)
{
    UINT32  uiValueHigh = 0;
    UINT32  uiValueLow = 0;
    /*altered by caibing 1-18*/
    //xra1403_down_clk();
//    GPIOEX_LOG(GPIOEX_LOG_EMERG, "gpioex mode get...\n");
    xra1403_read(COMMAND_BYTE_GCR2,&uiValueHigh);
    xra1403_read(COMMAND_BYTE_GCR1,&uiValueLow);
    *puiValue = 0xffff & ((uiValueHigh << 8) | uiValueLow);    
    /*altered by caibing 1-18*/
    //xra1403_up_clk();
    
    return AP_E_NONE;
}

AP_ERROR_CODE_E gpioex_mode_set(UINT32 uiNum,UINT32 uiValue)
{
 //   GPIOEX_LOG(GPIOEX_LOG_EMERG, "gpioex mode set...\n");
    GPIOEX_IOCTL_MODE_ALL_T stAllMode = {0};
    
    gpioex_mode_all_get(&stAllMode.uiValue);
    /*altered by caibing 1-18*/
    //xra1403_down_clk();
    
    if(uiNum < 8)
    {
        if(1 == uiValue)
        {
            stAllMode.uiValue = stAllMode.uiValue | 1<<uiNum;
        }
        else if(0 == uiValue)
        {
            stAllMode.uiValue = stAllMode.uiValue & (~(1<<uiNum));
        }
        uiValue = stAllMode.uiValue;
        //GPIOEX_LOG(GPIOEX_LOG_INFO, "Gpioex  uiNum %d uiValue %2x.\n",uiNum,uiValue);
        xra1403_write(COMMAND_BYTE_GCR1,uiValue);
    }
    else if(uiNum < 16)
    {
        if(1 == uiValue)
        {
            stAllMode.uiValue = stAllMode.uiValue | 1<<uiNum;
        }
        else if(0 == uiValue)
        {
            stAllMode.uiValue = stAllMode.uiValue & (~(1<<uiNum));
        }
        uiValue = stAllMode.uiValue >> 8;
        xra1403_write(COMMAND_BYTE_GCR2,uiValue);        
    }
    else 
    {
        return AP_E_RANGE;
    }
    //xra1403_up_clk();
    
    return AP_E_NONE;
}

AP_ERROR_CODE_E gpioex_mode_get(UINT32 uiNum,UINT32* puiValue)
{
    UINT32 uiValue = 0;
    gpioex_mode_all_get(&uiValue);

    //GPIOEX_LOG(GPIOEX_LOG_INFO, "Gpioex  uiNum %d uiValue %2x.\n",uiNum,uiValue);
    
    *puiValue = (uiValue>>uiNum) & 1;
    return AP_E_NONE;
}

AP_ERROR_CODE_E gpioex_state_all_set(UINT32 uiValue)
{
	UINT32  uiState = uiValue;
    /*altered by caibing 1-18*/
	//xra1403_down_clk();
	
//    GPIOEX_LOG(GPIOEX_LOG_EMERG, "gpioex state get...\n");
    xra1403_write(COMMAND_BYTE_OCR1,uiState);
    uiState = uiState>>8;
    xra1403_write(COMMAND_BYTE_OCR2,uiState);
    /*altered by caibing 1-18*/
    //xra1403_up_clk();
    
    return AP_E_NONE;

}

AP_ERROR_CODE_E gpioex_state_all_get(UINT32* puiValue)
{
    UINT32  uiValueHigh = 0;
    UINT32  uiValueLow = 0;
    /*altered by caibing 1-18*/
    //xra1403_down_clk();
//    GPIOEX_LOG(GPIOEX_LOG_EMERG, "gpioex_state_all_get...\n");
    xra1403_read(COMMAND_BYTE_GSR2,&uiValueHigh);
    xra1403_read(COMMAND_BYTE_GSR1,&uiValueLow);
    *puiValue = 0xffff & ((uiValueHigh << 8)|uiValueLow);
    /*altered by caibing 1-18*/
    //xra1403_up_clk();
    return AP_E_NONE;
}

AP_ERROR_CODE_E gpioex_state_set(UINT32 uiNum,UINT32 uiValue)
{
//    GPIOEX_LOG(GPIOEX_LOG_EMERG, "gpioex_state_set...\n");
    GPIOEX_IOCTL_STATE_ALL_T stAllState = {0};
    
    gpioex_state_all_get(&stAllState.uiValue);
    /*altered by caibing 1-18*/
    //xra1403_down_clk();
    if(uiNum < 8)
    {
        if(1 == uiValue)
        {
            stAllState.uiValue = stAllState.uiValue | 1<<uiNum;
        }
        else if(0 == uiValue)
        {
            stAllState.uiValue = stAllState.uiValue & (~(1<<uiNum));
        }
        uiValue = stAllState.uiValue;
        xra1403_write(COMMAND_BYTE_OCR1,uiValue);
    }
    else if(uiNum < 16)
    {
        if(1 == uiValue)
        {
            stAllState.uiValue = stAllState.uiValue | 1<<uiNum;
        }
        else if(0 == uiValue)
        {
            stAllState.uiValue = stAllState.uiValue & (~(1<<uiNum));
        }
        uiValue = stAllState.uiValue >> 8;
        xra1403_write(COMMAND_BYTE_OCR2,uiValue);        
    }
    else
    {
        return AP_E_RANGE;
    }
    /*altered by caibing 1-18*/
    //xra1403_up_clk();
    return AP_E_NONE;

}

AP_ERROR_CODE_E gpioex_state_get(UINT32 uiNum,UINT32* puiValue)
{
    UINT32 uiValue = 0;
    gpioex_state_all_get(&uiValue);
    *puiValue = (uiValue>>uiNum) & 1;
    return AP_E_NONE;
}

/*****************************************************************************
 Prototype    : gpioex_led_state_get
 Description  : ֱ�ӻ�ȡ��ǰ�Ƶ�״ֵ̬
 Input        : 
                
 Output       : None
 Return Value : void
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/01/29
    Author       : caibing
    Modification : Created function

*****************************************************************************/
AP_ERROR_CODE_E gpioex_iad_led_state_get(UINT32* puiLedStatus)
{ 
    //printk("kernel gpioex_lte_led_state_get\n");
    xra1403_read(COMMAND_BYTE_GSR2,puiLedStatus);
//    printk("gpioex_iad_led_state_get is %02x\n",*puiLedStatus);
    *puiLedStatus = 0xff & (*puiLedStatus);
    return AP_E_NONE;
}

/*****************************************************************************
 Prototype    : gpioex_led_state_set
 Description  : ֱ�����õ�ǰ�Ƶ�״ֵ̬
 Input        : 
                
 Output       : None
 Return Value : void
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/01/29
    Author       : caibing
    Modification : Created function

*****************************************************************************/
AP_ERROR_CODE_E gpioex_iad_led_state_set(UINT32 uiLedStatus)
{ 
    uiLedStatus = 0xff & uiLedStatus;
//    printk("gpioex_iad_led_state_set is %02x\n",uiLedStatus);
    xra1403_write(COMMAND_BYTE_OCR2,uiLedStatus);
    return AP_E_NONE;
}

EXPORT_SYMBOL(gpioex_mode_set);
EXPORT_SYMBOL(gpioex_mode_get);
EXPORT_SYMBOL(gpioex_mode_all_set);
EXPORT_SYMBOL(gpioex_mode_all_get);
EXPORT_SYMBOL(gpioex_state_set);
EXPORT_SYMBOL(gpioex_state_get);
EXPORT_SYMBOL(gpioex_state_all_set);
EXPORT_SYMBOL(gpioex_state_all_get);
EXPORT_SYMBOL(gpioex_iad_led_state_set);
EXPORT_SYMBOL(gpioex_iad_led_state_get);


