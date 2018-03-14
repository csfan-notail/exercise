/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : ledstatus.c
  版 本 号   : 初稿
  作    者   : xiao wei
  生成日期   : 2016年3月28日 星期一
  最近修改   :
  功能描述   :
  函数列表   :
  修改历史   :
  1.日    期   : 2016年3月8日 星期一
    作    者   : xiao wei
    修改内容   : 创建文件

******************************************************************************/
/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>  
#include <unistd.h>
#include <syslog.h>

#include "ap_types.h"
#include "lte.h"

#define GPIOEX0_BASIC_NUM		48

//altered by caibing
#define LED_STATUS_CHECK_TIMES	11
#define LED_STATUS_FLASH_TIMES	3

#define LED_GPIO_EX				1
#define LED_GPIO				0

#define LED_LTE_SIGNAL_OFF		1
#define LED_LTE_SIGNAL_ON		0
#define LED_LTE_SIGNAL_1		GPIOEX0_BASIC_NUM + 12
#define LED_LTE_SIGNAL_2		GPIOEX0_BASIC_NUM + 11
#define LED_LTE_SIGNAL_3		GPIOEX0_BASIC_NUM + 10
#define LED_LTE_SIGNAL_4		GPIOEX0_BASIC_NUM + 9
#define LED_LTE_SIGNAL_5		GPIOEX0_BASIC_NUM + 8

#define LED_LAN_ON				0     // 1
#define LED_LAN_OFF				1 	  // 0
#define LED_LAN_1				14    // 0
#define LED_LAN_2				1     // 1
#define LED_LAN_3				11    // 4
#define LED_LAN_4				16    // 3
#define LED_FUNC_1				41    // 41 
#define LED_FUNC_2				42    // 42
#define LED_FUNC_3				43    // 43
#define LED_FUNC_4				44    // 44

#define LED_OFF					0
#define LED_ON					1


#define LED_LINK_GREEN			GPIOEX0_BASIC_NUM + 6
#define LED_LINK_RED			GPIOEX0_BASIC_NUM + 7

#define LED_RUN_GREEN			4
#define LED_RUN_RED				0


#define GPIOEX1_BASIC_NUM       32

#define LED_LTE_ON                     0
#define LED_LTE_OFF   				   1

#define LED_LTE_SIM_2_GREEN      GPIOEX1_BASIC_NUM + 14
#define LED_LTE_SIM_1_GREEN      GPIOEX1_BASIC_NUM + 15


void gpioex_init(int gpioex)
{
	char acCmdExp[CHAR_LEN_64] = {0};
	char acCmdDir[CHAR_LEN_64] = {0};
	
	sprintf(acCmdExp,"echo %d >/sys/class/gpio/export", gpioex);
	sprintf(acCmdDir,"echo out >/sys/class/gpio/gpio%d/direction", gpioex);

	printf("[%s|%s:%d] acCmdExp = %s \n",__FILE__,__FUNCTION__,__LINE__,acCmdExp);	
	printf("[%s|%s:%d] acCmdDir = %s \n",__FILE__,__FUNCTION__,__LINE__,acCmdDir);	
	system(acCmdExp);
	usleep(500000);
	system(acCmdDir);
	
	return ;
}

void gpioex_destroy(int gpioex)
{
	char acCmdExp[CHAR_LEN_64] = {0};
	
	sprintf(acCmdExp,"echo %d >/sys/class/gpio/unexport", gpioex);
	printf("[%s|%s:%d] acCmdExp = %s \n",__FILE__,__FUNCTION__,__LINE__,acCmdExp);	
	system(acCmdExp);
	
	return ;
}

void gpioex_set(int gpioex, int status)
{
	char acCmdExp[CHAR_LEN_64] = {0};
	
	sprintf(acCmdExp,"echo %d >/sys/class/gpio/gpio%d/value", status, gpioex);
	printf("[%s|%s:%d] acCmdExp = %s \n",__FILE__,__FUNCTION__,__LINE__,acCmdExp);	
	system(acCmdExp);
	
	return ;
}

void gpio_set_led(int led_gpio, int led_status, int gpio)
{
	char acCmdFun[CHAR_LEN_32] = {0};
	char acCmdOut[CHAR_LEN_32] = {0};
	
	if( 0 == gpio )
	{
		sprintf(acCmdFun,"gpio %d func 0", led_gpio);
		sprintf(acCmdOut,"gpio %d out %d", led_gpio, led_status);
	}
	else
	{
		//sprintf(acCmdFun,"gpioex mode %d 0", led_gpio);
		//sprintf(acCmdOut,"gpioex state %d %d", led_gpio, led_status);
	}
	
	printf("[%s|%s:%d] acCmdFun = %s \n",__FILE__,__FUNCTION__,__LINE__,acCmdFun);	
	printf("[%s|%s:%d] acCmdOut = %s \n",__FILE__,__FUNCTION__,__LINE__,acCmdOut);	
	system(acCmdFun);
	usleep(500000);
	system(acCmdOut);
	
	return ;
}


INT32  led_lte_signal_status()
{
    LTE_SIGNAL_T stLteSignal = {0};
	
	if( LTE_OK != lte_signal_get(&stLteSignal) )
	{
		syslog(LOG_NOTICE, "lte_signal_get filed ! ");
		gpioex_set(LED_LTE_SIGNAL_1, LED_LTE_SIGNAL_OFF);
		gpioex_set(LED_LTE_SIGNAL_2, LED_LTE_SIGNAL_OFF);
		gpioex_set(LED_LTE_SIGNAL_3, LED_LTE_SIGNAL_OFF);
		gpioex_set(LED_LTE_SIGNAL_4, LED_LTE_SIGNAL_OFF);
		gpioex_set(LED_LTE_SIGNAL_5, LED_LTE_SIGNAL_OFF);
		return AP_E_DEFAULT_FAIL;
	}
	
	printf("[%s|%s:%d] stLteSignal.uiSignal = %d \n",__FILE__,__FUNCTION__,__LINE__,stLteSignal.uiSignal);	

	if( 1 == stLteSignal.uiSignal )
	{
		gpioex_set(LED_LTE_SIGNAL_1, LED_LTE_SIGNAL_ON);
		gpioex_set(LED_LTE_SIGNAL_2, LED_LTE_SIGNAL_OFF);
		gpioex_set(LED_LTE_SIGNAL_3, LED_LTE_SIGNAL_OFF);
		gpioex_set(LED_LTE_SIGNAL_4, LED_LTE_SIGNAL_OFF);
		gpioex_set(LED_LTE_SIGNAL_5, LED_LTE_SIGNAL_OFF);
	}
	else if ( 2 == stLteSignal.uiSignal  )
	{
		gpioex_set(LED_LTE_SIGNAL_1, LED_LTE_SIGNAL_ON);
		gpioex_set(LED_LTE_SIGNAL_2, LED_LTE_SIGNAL_ON);
		gpioex_set(LED_LTE_SIGNAL_3, LED_LTE_SIGNAL_OFF);
		gpioex_set(LED_LTE_SIGNAL_4, LED_LTE_SIGNAL_OFF);
		gpioex_set(LED_LTE_SIGNAL_5, LED_LTE_SIGNAL_OFF);
	}
	else if ( 3 == stLteSignal.uiSignal  )
	{
		gpioex_set(LED_LTE_SIGNAL_1, LED_LTE_SIGNAL_ON);
		gpioex_set(LED_LTE_SIGNAL_2, LED_LTE_SIGNAL_ON);
		gpioex_set(LED_LTE_SIGNAL_3, LED_LTE_SIGNAL_ON);
		gpioex_set(LED_LTE_SIGNAL_4, LED_LTE_SIGNAL_OFF);
		gpioex_set(LED_LTE_SIGNAL_5, LED_LTE_SIGNAL_OFF);
	}		
	else if ( 4 == stLteSignal.uiSignal  )
	{
		gpioex_set(LED_LTE_SIGNAL_1, LED_LTE_SIGNAL_ON);
		gpioex_set(LED_LTE_SIGNAL_2, LED_LTE_SIGNAL_ON);
		gpioex_set(LED_LTE_SIGNAL_3, LED_LTE_SIGNAL_ON);
		gpioex_set(LED_LTE_SIGNAL_4, LED_LTE_SIGNAL_ON);
		gpioex_set(LED_LTE_SIGNAL_5, LED_LTE_SIGNAL_OFF);
	}
	else if ( 5 == stLteSignal.uiSignal  )
	{
		gpioex_set(LED_LTE_SIGNAL_1, LED_LTE_SIGNAL_ON);
		gpioex_set(LED_LTE_SIGNAL_2, LED_LTE_SIGNAL_ON);
		gpioex_set(LED_LTE_SIGNAL_3, LED_LTE_SIGNAL_ON);
		gpioex_set(LED_LTE_SIGNAL_4, LED_LTE_SIGNAL_ON);
		gpioex_set(LED_LTE_SIGNAL_5, LED_LTE_SIGNAL_ON);
	}	
	else
	{
		gpioex_set(LED_LTE_SIGNAL_1, LED_LTE_SIGNAL_OFF);
		gpioex_set(LED_LTE_SIGNAL_2, LED_LTE_SIGNAL_OFF);
		gpioex_set(LED_LTE_SIGNAL_3, LED_LTE_SIGNAL_OFF);
		gpioex_set(LED_LTE_SIGNAL_4, LED_LTE_SIGNAL_OFF);
		gpioex_set(LED_LTE_SIGNAL_5, LED_LTE_SIGNAL_OFF);
	}		
	
    return AP_E_NONE;
}

INT32  led_lte_dial_status()
{
    LTE_LINKSTATE_T stLinkState = {0};
	
	if( LTE_OK != lte_linkstate_get(&stLinkState) )
	{
		App_log(LOG_NOTICE, "lte_linkstate_get filed ! ");
		gpioex_set(LED_LINK_GREEN, LED_OFF);
    	gpioex_set(LED_LINK_RED,   LED_ON);
		return AP_E_DEFAULT_FAIL;
	}
	
	printf("[%s|%s:%d] stLinkState.enLinkstate = %d \n",__FILE__,__FUNCTION__,__LINE__,stLinkState.enLinkstate);	
    
    if( LTE_LINKSTATE_CONNECTED == stLinkState.enLinkstate )
    {
    	gpioex_set(LED_LINK_GREEN, LED_ON);
    	gpioex_set(LED_LINK_RED,   LED_OFF);
    }
    else
    {
    	gpioex_set(LED_LINK_GREEN, LED_OFF);
    	gpioex_set(LED_LINK_RED,   LED_ON);
    }
    
    return AP_E_NONE;
}

INT32  led_progress_status()
{
    gpio_set_led(LED_RUN_GREEN, LED_ON, LED_GPIO);
    gpio_set_led(LED_RUN_RED,   LED_OFF, LED_GPIO);

    return AP_E_NONE;
}

INT32  led_lan_datas_status()
{
    char acCmdFun[CHAR_LEN_32] = {0};
	char acCmdOut[CHAR_LEN_32] = {0};

	sprintf(acCmdOut,"gpio %d out %d ", LED_LAN_1, LED_LAN_OFF);
	sprintf(acCmdFun,"gpio %d func %d", LED_LAN_1, LED_FUNC_1);
	system(acCmdOut);
	usleep(500000);
	system(acCmdFun);
	printf("[%s|%s:%d] acCmdOut = %s acCmdFun = %s \n",__FILE__,__FUNCTION__,__LINE__, acCmdOut, acCmdFun);	
	sprintf(acCmdOut,"gpio %d out %d ", LED_LAN_2, LED_LAN_OFF);
	sprintf(acCmdFun,"gpio %d func %d", LED_LAN_2, LED_FUNC_2);
	system(acCmdOut);
	usleep(500000);
	system(acCmdFun);
	printf("[%s|%s:%d] acCmdOut = %s acCmdFun = %s \n",__FILE__,__FUNCTION__,__LINE__, acCmdOut, acCmdFun);	
	sprintf(acCmdOut,"gpio %d out %d ", LED_LAN_3, LED_LAN_OFF);
	sprintf(acCmdFun,"gpio %d func %d", LED_LAN_3, LED_FUNC_3);
	system(acCmdOut);
	usleep(500000);
	system(acCmdFun);
	printf("[%s|%s:%d] acCmdOut = %s acCmdFun = %s \n",__FILE__,__FUNCTION__,__LINE__, acCmdOut, acCmdFun);	
	sprintf(acCmdOut,"gpio %d out %d ", LED_LAN_4, LED_LAN_OFF);
	sprintf(acCmdFun,"gpio %d func %d", LED_LAN_4, LED_FUNC_4);
	system(acCmdOut);
	usleep(500000);
	system(acCmdFun);
	printf("[%s|%s:%d] acCmdOut = %s acCmdFun = %s \n",__FILE__,__FUNCTION__,__LINE__, acCmdOut, acCmdFun);	
	
	
    return AP_E_NONE;
}

void led_blink(int gpioex)
{
    int iLoop = 1;
    for(iLoop;iLoop < LED_STATUS_FLASH_TIMES; iLoop ++)
    {
	   gpioex_set(gpioex,LED_LTE_ON);
	   usleep(500000);
	   gpioex_set(gpioex,LED_LTE_OFF);
	   usleep(500000);
    }

    return;
}
void led_popen_get_val(char *acCmd, char *retval)
{
	FILE *fp_p = NULL;
	char acBuff[32] = {0};
	fp_p = popen(acCmd,"r");
	if(fp_p){
		fgets(acBuff,32,fp_p);
		acBuff[strlen(acBuff)-1] = '\0';
		strcpy(retval,acBuff);
		//printf("[%s:%d] acBuff=%s retval=%s\n",__FUNCTION__,__LINE__,acBuff,retval);	
	}
	pclose(fp_p);
}
void led_lte_check_sim_status()
{
	int iRet = 0;
 
	LTE_SIM_STATE_T stSimState = {0};
	iRet = lte_sim_state_get(&stSimState);
 	
 	//sim card ok
	if(LTE_OK == iRet)
	{
		printf("[%s:%d] lte sim1  ret = %d ok\n",__FUNCTION__,__LINE__,iRet);
		printf("[%s:%d] lte sim1 state  = %d [1:yes sim 0:no sim]\n",__FUNCTION__,__LINE__,stSimState.enSimState);
		if(0 == stSimState.enSimState){
			gpioex_set(LED_LTE_SIM_1_GREEN,LED_LTE_OFF);
			//gpioex_set(LED_LTE_SIM_2_GREEN,LED_LTE_OFF);
		}else{
			gpioex_set(LED_LTE_SIM_1_GREEN,LED_LTE_ON);
			//gpioex_set(LED_LTE_SIM_2_GREEN,LED_LTE_ON);
		}
	}else{
		printf("[%s:%d] lte sim1 state ret = %d fail \n",__FUNCTION__,__LINE__,iRet);
	}
	
	return ;
}

int main(int iArgc, char *apcArgv[])
{
//    INT32 iGetinfoTime = 0;
//    INT32 iRet = 0;
    
//    if ( iArgc == 2 && NULL != apcArgv[1])
//    {
//        iGetinfoTime = atoi(apcArgv[1]);
//    }
//    else
//    {
//        iGetinfoTime = LED_STATUS_CHECK_TIMES;
//    }
    
    //led_lte_dial_status
    gpioex_init(LED_LINK_GREEN);
    gpioex_init(LED_LINK_RED);
    //led_lte_signal_status
    gpioex_init(LED_LTE_SIGNAL_1);
    gpioex_init(LED_LTE_SIGNAL_2);
    gpioex_init(LED_LTE_SIGNAL_3);
    gpioex_init(LED_LTE_SIGNAL_4);
    gpioex_init(LED_LTE_SIGNAL_5);
    
    //sim card
  	gpioex_init(LED_LTE_SIM_1_GREEN);
  	gpioex_init(LED_LTE_SIM_2_GREEN);
    
    //lan datas
	led_lan_datas_status();
	//run progress
	led_progress_status();
	
	int sim_flag = 0;
	
	do
	{   
		//lte signal
		led_lte_signal_status();
		sleep(1);
		//link lte dial
		led_lte_dial_status();
		
		//sim card
		if(30 > sim_flag){
			sleep(1);
			led_lte_check_sim_status();
			sim_flag++;
		}
		//sleep(iGetinfoTime);
		sleep(LED_STATUS_CHECK_TIMES);
	} while ( 1 );
    
    syslog(LOG_NOTICE, "func:%s,line:%d, led status check exit ! ",__func__,__LINE__);

    return AP_E_NONE;
}