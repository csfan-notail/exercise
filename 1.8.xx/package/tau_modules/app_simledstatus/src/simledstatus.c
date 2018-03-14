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
#include <sys/types.h>
#include "ap_types.h"
#include "lte.h"

#define GPIOEX1_BASIC_NUM       32

#define LED_STATUS_CHECK_TIMES	3
#define LED_GPIO_EX				1
#define LED_GPIO			  	0

#define LTE_INIT_SUCCESS    1
#define LTE_INIT_FAIL       0

#define LED_LTE_SIM_SELECT_SW        GPIOEX1_BASIC_NUM + 1
#define LED_LTE_SIM_SLOT_STATUS      GPIOEX1_BASIC_NUM

#define LED_LTE_SIM_A                  0
#define LED_LTE_SIM_B                  1

#define LED_LTE_ON                     0
#define LED_LTE_OFF   				         1

#define LED_LTE_SIM_A_RED        GPIOEX1_BASIC_NUM + 2
#define LED_LTE_SIM_A_GREEN      GPIOEX1_BASIC_NUM + 3
#define LED_LTE_SIM_B_RED        GPIOEX1_BASIC_NUM + 4
#define LED_LTE_SIM_B_GREEN      GPIOEX1_BASIC_NUM + 5


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

int gpioex_get(int gpioex)
{
	char acCmdExp[CHAR_LEN_64] = {0};
	char acBuf[CHAR_LEN_64];
	FILE *pStream;

	memset(acBuf,0,sizeof(acBuf));
 
	sprintf(acCmdExp,"cat /sys/class/gpio/gpio%d/value",gpioex);
    printf("[%s|%s:%d] acCmdExp = %s \n",__FILE__,__FUNCTION__,__LINE__,acCmdExp);

	pStream = popen(acCmdExp,"r");
   
    if(pStream == NULL)
    {
        printf("popen fail\n");
        return -1;
    }

	fread(acBuf,sizeof(char),sizeof(acBuf),pStream); 

    pclose(pStream);
	if(strcmp(acBuf,"1\n") == 0)
	{
		return 1;
	}
	else{
		return 0;
	}
}


void gpioex_set(int gpioex, int status)
{
	char acCmdExp[CHAR_LEN_64] = {0};
	
	sprintf(acCmdExp,"echo %d >/sys/class/gpio/gpio%d/value", status, gpioex);
	printf("[%s|%s:%d] acCmdExp = %s \n",__FILE__,__FUNCTION__,__LINE__,acCmdExp);	
	system(acCmdExp);
	
	return ;
}
void popen_get_val(char *acCmd, char *retval)
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
void led_blink(int gpioex)
{
    int iLoop = 1;
    for(iLoop;iLoop < LED_STATUS_CHECK_TIMES; iLoop ++)
    {
	   gpioex_set(gpioex,LED_LTE_ON);
	   usleep(500000);
	   gpioex_set(gpioex,LED_LTE_OFF);
	   usleep(500000);
    }

    return;
}

void double_blink()
{
    int iLoop = 1;
    for(iLoop;iLoop < LED_STATUS_CHECK_TIMES; iLoop ++)
    {
       gpioex_set(LED_LTE_SIM_A_RED,LED_LTE_ON);
       gpioex_set(LED_LTE_SIM_B_RED,LED_LTE_ON);
       gpioex_set(LED_LTE_SIM_A_GREEN,LED_LTE_OFF);
       gpioex_set(LED_LTE_SIM_B_GREEN,LED_LTE_OFF);
       usleep(500000);
       gpioex_set(LED_LTE_SIM_A_RED,LED_LTE_OFF);
       gpioex_set(LED_LTE_SIM_B_RED,LED_LTE_OFF);
       gpioex_set(LED_LTE_SIM_A_GREEN,LED_LTE_OFF);
       gpioex_set(LED_LTE_SIM_B_GREEN,LED_LTE_OFF);
       usleep(500000);
    }

    return;
}

int get_lte_init()
{
  char acCmdGpio[CHAR_LEN_64] = {0};
  char acBuf[CHAR_LEN_64];
  FILE *pStream;

  memset(acBuf,0,sizeof(acBuf));
  
  sprintf(acCmdGpio,"ltectl getpinstate");
  pStream = popen(acCmdGpio,"r");

  if(pStream == NULL)
  {
      printf("popen fail\n");
      return -1;
  }
  
  fread(acBuf,sizeof(char),sizeof(acBuf),pStream); 
  pclose(pStream);

  if(strcmp(acBuf,"lte_init failed.\n") == 0)
  {
    return LTE_INIT_FAIL;
  }
  else{
    return LTE_INIT_SUCCESS;
  }

}

void led_lte_check_sim_status(int sim_flag)
{
	int iRet = 0;
 
	LTE_SIM_STATE_T stSimState = {0};
	iRet = lte_sim_state_get(&stSimState);
 	
 	//sim card ok
	if(LTE_OK == iRet)
	{
		//sim b
		if(sim_flag){
			gpioex_set(LED_LTE_SIM_B_GREEN,LED_LTE_ON);
			gpioex_set(LED_LTE_SIM_B_RED,LED_LTE_OFF);
		}else{ //sim a
			gpioex_set(LED_LTE_SIM_A_GREEN,LED_LTE_ON);
			gpioex_set(LED_LTE_SIM_A_RED,LED_LTE_OFF);
		}
	}else{
		//sim b
		if(sim_flag){
			gpioex_set(LED_LTE_SIM_B_GREEN,LED_LTE_OFF);
			led_blink(LED_LTE_SIM_B_RED);
		}else{ //sim a
			gpioex_set(LED_LTE_SIM_A_GREEN,LED_LTE_OFF);
			led_blink(LED_LTE_SIM_A_RED);
		}
		
	}

	return ;
}


int main(int iArgc, char *apcArgv[])
{
  	gpioex_init(LED_LTE_SIM_A_RED);
  	gpioex_init(LED_LTE_SIM_A_GREEN);
  	gpioex_init(LED_LTE_SIM_B_RED);
  	gpioex_init(LED_LTE_SIM_B_GREEN);
	
	int sim_card = -1;
	char acRet[32] = {0};
	popen_get_val("uci get lte.simswitch.simcard",acRet);
	if(0 == strcmp(acRet,"sima") || 0 == strcmp(acRet,"simb")){
		
		if(0 == strcmp(acRet,"sima")){
			sim_card = 0;	
		}else{
			sim_card = 1;	
		}
		
		do
		{   
			led_lte_check_sim_status(sim_card);
			sleep(11);
		} while ( 1 );
	}else{
		printf("func:%s,line:%d, led status check exit ! ",__func__,__LINE__);
	}
    syslog(LOG_NOTICE, "func:%s,line:%d, led status check exit ! ",__func__,__LINE__);

    return AP_E_NONE;
}