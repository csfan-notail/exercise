/******************************************************************************

                  °æÈ¨ËùÓÐ (C), 2001-2011, ÉÏº£å¾´´Í¨ÐÅ¹É·ÝÓÐÏÞ¹«Ë¾

 ******************************************************************************
  ÎÄ ¼þ Ãû   : ltegetinfo.c
  °æ ±¾ ºÅ   : ³õ¸å
  ×÷    Õß   : xiao wei
  Éú³ÉÈÕÆÚ   : 2016Äê3ÔÂ23ÈÕ ÐÇÆÚÈý
  ×î½üÐÞ¸Ä   :
  ¹¦ÄÜÃèÊö   :
  º¯ÊýÁÐ±í   :
  ÐÞ¸ÄÀúÊ·   :
  1.ÈÕ    ÆÚ   : 2016Äê3ÔÂ23ÈÕ ÐÇÆÚÈý
    ×÷    Õß   : xiao wei
    ÐÞ¸ÄÄÚÈÝ   : ´´½¨ÎÄ¼þ

******************************************************************************/
/*----------------------------------------------*
 * °üº¬Í·ÎÄ¼þ                                   *
 *----------------------------------------------*/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>  
#include <unistd.h>
#include "uci_config.h"
#include "pal_include/pal_uci.h"
#include "ap_types.h"
#include "lte.h"

#define LTE_GET_INFO_TIME	5

INT32 lte_get_info()
{
	INT32 iRet             = 0;
	CHAR acTmpStr[64]      = {0};
	LTE_INFO_T stLteInfo   = {0};
	UINT32 uiCurLteChannel = 0;

	iRet = lte_info_get(&stLteInfo);
    if( LTE_OK == iRet)
   	{
    	printf("[%25s|%30s:%5d] stLteInfo.stCellInfo.iRssi=%d\n",__FILE__,__FUNCTION__,__LINE__,stLteInfo.stCellInfo.iRssi);	
    	
		sprintf(acTmpStr,"echo %d > /tmp/uci/iRssi2",stLteInfo.stCellInfo.iRssi);   //rssi
		system(acTmpStr);
		usleep(500000);
		sprintf(acTmpStr,"echo %d > /tmp/uci/iRsrp2",stLteInfo.stCellInfo.iRsrp);   //rsrp
		system(acTmpStr);
		usleep(500000);
		sprintf(acTmpStr,"echo %d > /tmp/uci/iRsrq2",stLteInfo.stCellInfo.iRsrq);   //iRsrq
		system(acTmpStr);
		usleep(500000);	
		sprintf(acTmpStr,"echo %d > /tmp/uci/iSinr2",stLteInfo.stCellInfo.iSinr);   //iSinr
		system(acTmpStr);
		usleep(500000);	
		sprintf(acTmpStr,"echo %s > /tmp/uci/acCellid2",stLteInfo.stCellInfo.acCellId);   //cellid
		system(acTmpStr);
		usleep(500000);	
		sprintf(acTmpStr,"echo %s > /tmp/uci/acPhyCellid2",stLteInfo.stCellInfo.acPhyCeillid);   //phycellid
		system(acTmpStr);
		
		
		syslog(LOG_NOTICE, "func:%s,line:%d,rssi:%d,rsrp:%d,rsrq:%d,sinr:%d,cellid:%s",__func__,__LINE__,stLteInfo.stCellInfo.iRssi,stLteInfo.stCellInfo.iRsrp,stLteInfo.stCellInfo.iRsrq,stLteInfo.stCellInfo.iSinr,stLteInfo.stCellInfo.acCellId);
   }
    else
    {
    	printf("[%25s|%30s:%5d] get stLteInfo.stCellInfo.iRssi failed! \n",__FILE__,__FUNCTION__,__LINE__);	
		//exit(0);
    }
	return AP_E_NONE;
}

INT32 lte_get_simstatus()
{
    INT32 iRet1 = 0;
 
    CHAR acTmpStr[64] = {0};
    LTE_SIM_STATE_T stSimState = {0};
    iRet1 = lte_sim_state_get(&stSimState);
 
   if(LTE_OK == iRet1)
   {
     printf("[%25s|%30s:%5d] stSimState.enSimState=%d\n",__FILE__,__FUNCTION__,__LINE__,stSimState.enSimState); 
       
	 sprintf(acTmpStr,"echo %d > /tmp/uci/enSimState2",stSimState.enSimState);   //enSimState
	 system(acTmpStr);
   }
   else
   {
      sprintf(acTmpStr,"echo 0 > /tmp/uci/enSimState2");   //enSimState
	  system(acTmpStr);
      syslog(LOG_NOTICE,"func:%s,line:%d,Get Sim Status Fail",__func__,__LINE__);
      printf("[%25s|%30s:%5d] get stSimState.enSimState failed! \n",__FILE__,__FUNCTION__,__LINE__); 
	  //exit(0);
   }
  return AP_E_NONE;
}

/***去掉字符的左边的空格或其他字符***/  
void ltrim(char *str, const char ch)  
{  
    char *ptr;  
    for (ptr = str; *ptr; ptr++) {  
        if(*ptr != ch)  break;  
    }  
    strcpy(str, ptr);  
}  

INT32 lte_get_siminfo()
{
 INT32 iRet1 = 0;
 INT32 iRet2 = 0;
 CHAR acTmpStr[32] = {0};
  CHAR acCmd[64] = {0};
  LTE_IMSI_T stImsi = {0};
  LTE_ICCID_T stICCID = {0};
  char ch=' ';
 
  iRet1 = lte_imsi_get(&stImsi);
  iRet2 = lte_iccid_get(&stICCID);  

    if( LTE_OK == iRet1 )
    {
        printf("[%25s|%30s:%5d] stImsi.acIMSI=%s\n",__FILE__,__FUNCTION__,__LINE__,stImsi.acIMSI);
        ltrim(stImsi.acIMSI,ch);
        snprintf(acTmpStr,16,"%s",stImsi.acIMSI);  
        sprintf(acCmd,"echo %s > /tmp/uci/acIMSI2",acTmpStr);
        system(acCmd);
    }
    else
    {
		sprintf(acCmd,"echo 0 > /tmp/uci/acIMSI2");   //acIMSI
		system(acCmd);
		usleep(500000);
		printf("[%25s|%30s:%5d] get stImsi.acIMSI  failed! \n",__FILE__,__FUNCTION__,__LINE__); 
    }

    if(  LTE_OK == iRet2 )
    {
        printf("[%25s|%30s:%5d] stImsi.acICCID=%s\n",__FILE__,__FUNCTION__,__LINE__,stICCID.acICCID);
        snprintf(acTmpStr,16,"%s",stICCID.acICCID);  
        sprintf(acCmd,"echo %s > /tmp/uci/acICCID2",acTmpStr);
		system(acCmd);
    }
    else
    {
		sprintf(acCmd,"echo 0 > /tmp/uci/acICCID2");   //acIMSI
		system(acCmd);
		usleep(500000);
		printf("[%25s|%30s:%5d] get  stICCID.acICCID failed! \n",__FILE__,__FUNCTION__,__LINE__); 
    }
  return AP_E_NONE;
}

INT32 lte_get_pin()
{
 INT32 iRet = 0;
  CHAR acTmpStr[64] = {0};
  LTE_PIN_STATE_T  stState = {0};

  iRet = lte_pin_state_get(&stState);
  if(iRet == LTE_OK)
  {
    printf("[%25s|%30s:%5d] stState.enState=%d\n",__FILE__,__FUNCTION__,__LINE__,stState.enState);
	
	sprintf(acTmpStr,"echo %d > /tmp/uci/enState2",stState.enState);   //enState
	system(acTmpStr);
  }
  else
  {
		sprintf(acTmpStr,"echo -1 > /tmp/uci/enState2");   //enState
		system(acTmpStr);

    printf("[%25s|%30s:%5d] get stState.enState failed! \n",__FILE__,__FUNCTION__,__LINE__); 
  }
  return AP_E_NONE;
}

INT32 lte_get_dial_status()
{
  INT32 iRet = 0;
  CHAR acTmpStr[64] = {0};
  LTE_LINKSTATE_T stLinkState = {0};

  iRet = lte_linkstate_get(&stLinkState);
  if(iRet == LTE_OK)
  {       
    printf("[%25s|%30s:%5d] stLinkState.enLinkstate=%d\n",__FILE__,__FUNCTION__,__LINE__,stLinkState.enLinkstate);
	sprintf(acTmpStr,"echo %d > /tmp/uci/enLinkstate2",stLinkState.enLinkstate);   //enLinkstate
	system(acTmpStr);
  }
  else
  {
    
	sprintf(acTmpStr,"echo 0 > /tmp/uci/enLinkstate2");   //enLinkstate
	system(acTmpStr);
    printf("[%25s|%30s:%5d] get stLinkState.enLinkstate failed! \n",__FILE__,__FUNCTION__,__LINE__); 
	//exit(0);
  }
  return AP_E_NONE;
}

INT32 lte_get_operator()
{
  INT32 iRet = 0;
  CHAR acTmpStr[64] = {0};
  LTE_OPER_INFO_T stOperInfo = {0};

  iRet = lte_oper_info_get(&stOperInfo);
  if(iRet == LTE_OK)
  {
    printf("[%25s|%30s:%5d] stOperInfo.acOperName=%s\n",__FILE__,__FUNCTION__,__LINE__,stOperInfo.acOperName);
   
	sprintf(acTmpStr,"echo %s > /tmp/uci/acOperName2",stOperInfo.acOperName);   //acOperName
	system(acTmpStr);
  }
  else
  {
   
	sprintf(acTmpStr,"echo 0 > /tmp/uci/acOperName2");   //acOperName
	system(acTmpStr);
    printf("[%25s|%30s:%5d] get stOperInfo.acOperName failed! \n",__FILE__,__FUNCTION__,__LINE__); 
	//exit(0);
  }
  return AP_E_NONE;
}

INT32 lte_get_access_network()
{
  INT32 iRet = 0;
  CHAR acTmpStr[32] = {0};
  LTE_ACCESS_NETWORK_T stAccessNetMode = {0};

  iRet = lte_access_net_get(&stAccessNetMode);
  if(iRet == LTE_OK)
  {
     printf("[%25s|%30s:%5d] stAccessNetMode.enAccessNet=%d\n",__FILE__,__FUNCTION__,__LINE__,stAccessNetMode.enAccessNet);
     if(3 == stAccessNetMode.enAccessNet)
     {
        system("echo '4G' > /tmp/uci/enAccessNet2");
     }
     else if(2 == stAccessNetMode.enAccessNet)
     {
        system("echo '3G' > /tmp/uci/enAccessNet2");
     }
     else if(1 == stAccessNetMode.enAccessNet)
     {
        system("echo '2G' > /tmp/uci/enAccessNet2");
     }
     
  }
  else
  {
   	system("echo 'N/A' > /tmp/uci/enAccessNet2");
    printf("[%25s|%30s:%5d] get stAccessNetMode.enAccessNet failed! \n",__FILE__,__FUNCTION__,__LINE__); 
	//exit(0);
  }
  return AP_E_NONE;
}

INT32 lte_get_imei()
{
  INT32 iRet = 0;
  CHAR acTmpStr[32] = {0};
  LTE_IMEI_T stImei = {0};


  iRet = lte_imei_get(&stImei);
  if(iRet == LTE_OK)
  {
    printf("[%25s|%30s:%5d] Module IMEI=%s\n",__FILE__,__FUNCTION__,__LINE__,stImei.acImei);
    
    sprintf(acTmpStr,"echo %s > /tmp/uci/acIMEI2",stImei.acImei);   //acOperName
	system(acTmpStr);
  }
  else
  {
    system("echo 'N/A' > /tmp/uci/acIMEI2");
    printf("[%25s|%30s:%5d] get Module IMEI failed! \n",__FILE__,__FUNCTION__,__LINE__); 
	//exit(0);
  }
  return AP_E_NONE;
}

INT32 lte_get_lte_version()
{
  INT32 iRet = 0;
  CHAR acTmpStr[32] = {0};
  LTE_MODULE_TYPE_T stModuleType = {0};

  iRet = lte_version_get(&stModuleType);
  if(iRet == LTE_OK)
  {
    printf("[%25s|%30s:%5d] stModuleType.acVersion=%s\n",__FILE__,__FUNCTION__,__LINE__,stModuleType.acVersion);
    
    sprintf(acTmpStr,"echo %s > /tmp/uci/acVersion2",stModuleType.acVersion);   //acOperName
	system(acTmpStr);
  }
  else
  {
    system("echo 'N/A' > /tmp/uci/acVersion2");
    printf("[%25s|%30s:%5d] get stModuleType.acVersion failed! \n",__FILE__,__FUNCTION__,__LINE__); 
	//exit(0);
  }
  return AP_E_NONE;
}

INT32 lte_get_tac()
{
  INT32 iRet = 0;
  CHAR acTmpStr[64] = {0};
  LTE_TAC_T stTac = {0};

  iRet = lte_tac_get(&stTac);
  if(iRet == LTE_OK)
  {
     printf("[%25s|%30s:%5d] stTac.acTac=%s\n",__FILE__,__FUNCTION__,__LINE__,stTac.acTac);
   
	 sprintf(acTmpStr,"echo %s > /tmp/uci/acTac2",stTac.acTac);   //acTac
	 system(acTmpStr);
  }
  else
  {
    printf("[%25s|%30s:%5d] get stTac.acTac failed! \n",__FILE__,__FUNCTION__,__LINE__); 
	//exit(0);
  }
  return AP_E_NONE;
}

INT32 lte_get_qos_data()
{	
	INT32 iRet = 0;
    CHAR acTmpStr[32] = {0};
	LTE_QOS_DATA_S stQosData={0};
	iRet = lte_qos_get(&stQosData);
	if(iRet == LTE_OK)
	{
	     printf("[%25s|%30s:%5d] stQosData.iQosCid=%d\n",__FILE__,__FUNCTION__,__LINE__,stQosData.iQosCid);
		 printf("[%25s|%30s:%5d] stQosData.iQosQci=%d\n",__FILE__,__FUNCTION__,__LINE__,stQosData.iQosQci);
		 printf("[%25s|%30s:%5d] stQosData.iQosDlGbr=%d\n",__FILE__,__FUNCTION__,__LINE__,stQosData.iQosDlGbr);
		 printf("[%25s|%30s:%5d] stQosData.iQosUlGbr=%d\n",__FILE__,__FUNCTION__,__LINE__,stQosData.iQosUlGbr);
		 printf("[%25s|%30s:%5d] stQosData.iQosDlMbr=%d\n",__FILE__,__FUNCTION__,__LINE__,stQosData.iQosDlMbr);
		 printf("[%25s|%30s:%5d] stQosData.iQosDlMbr=%d\n",__FILE__,__FUNCTION__,__LINE__,stQosData.iQosUlMbr);
		 
	    sprintf(acTmpStr,"echo %d > /tmp/uci/iQosCid2",stQosData.iQosCid);
	 	system(acTmpStr);
		usleep(500000);
		 sprintf(acTmpStr,"echo %d > /tmp/uci/iQosQci2",stQosData.iQosQci);
	 	system(acTmpStr);
		usleep(500000);
		 sprintf(acTmpStr,"echo %d > /tmp/uci/iQosDlGbr2",stQosData.iQosDlGbr);
	 	system(acTmpStr);
		 usleep(500000);
		sprintf(acTmpStr,"echo %d > /tmp/uci/iQosUlGbr2",stQosData.iQosUlGbr);
	 	system(acTmpStr);
		usleep(500000);
		sprintf(acTmpStr,"echo %d > /tmp/uci/iQosDlMbr2",stQosData.iQosDlMbr);
	 	system(acTmpStr);
		usleep(500000);
		 sprintf(acTmpStr,"echo %d > /tmp/uci/iQosUlMbr2",stQosData.iQosUlMbr);
	 	system(acTmpStr);
	}
	else
    {
    	system("echo 0 > /tmp/uci/iQosCid2");
    	usleep(500000);
    	system("echo 0 > /tmp/uci/iQosQci2");
    	usleep(500000);
    	system("echo 0 > /tmp/uci/iQosDlGbr2");
    	usleep(500000);
    	system("echo 0 > /tmp/uci/iQosUlGbr2");
    	usleep(500000);
    	system("echo 0 > /tmp/uci/iQosDlMbr2");
    	usleep(500000);
    	system("echo 0 > /tmp/uci/iQosUlMbr2");

    	 printf("[%25s|%30s:%5d] get stQosData failed! \n",__FILE__,__FUNCTION__,__LINE__); 
    }
	return AP_E_NONE;
}

INT32 main(INT32 iArgc, CHAR **apcArgv[])
{
    INT32 iGetinfoTime = 10;
    
    //if ( iArgc == 2 && NULL != apcArgv[1])
    //{
    //    iGetinfoTime = atoi(apcArgv[1]);
    //}
    //else
    //{
        iGetinfoTime = LTE_GET_INFO_TIME;
    //}
    int iFlag = 0;
	do
	{ 
   
     	lte_get_info();                   // 获取rsrp rssi rsrq sinr 物理小区 逻辑小区 频段 频点   过
     	sleep(1);
     	lte_get_simstatus();              // sim卡状态              过
     	sleep(1);
     	lte_get_pin();                    // 获取pin状态            过
     	sleep(1);
		lte_get_dial_status();            //获取拨号状态            过
    	sleep(1);
		lte_get_operator();               //获取运营商信息          过
		sleep(1);
		lte_get_siminfo();                //获取imsi iccid          过
		sleep(1);
		
		lte_get_imei(); 	   //add imei  2017-12-19  by chenshaofan
		sleep(1);
		if(iFlag < 3)
		{
			lte_get_lte_version();            //获取LTE版本             过
			iFlag++;
		}
		sleep(1);
		lte_get_tac();                    //获取tac                 过
		sleep(1);
		lte_get_access_network();         //获取入网状态   
		sleep(1);
		lte_get_qos_data();
		
		sleep(iGetinfoTime);
	} while ( 1 );
 
    syslog(LOG_NOTICE, "func:%s,line:%d,lte get info exit ! ",__func__,__LINE__);

    return AP_E_NONE;
}