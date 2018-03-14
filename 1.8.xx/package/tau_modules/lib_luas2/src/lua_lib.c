/******************************************************************************

                  °æÈ¨ËùÓÐ (C), 2001-2011, ÉÏº£å¾´´Í¨ÐÅ¹É·ÝÓÐÏÞ¹«Ë¾

 ******************************************************************************
  ÎÄ ¼þ Ãû   : lua_lib.c
  °æ ±¾ ºÅ   : ³õ¸å
  ×÷    Õß   : xiao wei
  Éú³ÉÈÕÆÚ   : 2016Äê3ÔÂ16ÈÕ ÐÇÆÚÈý
  ×î½üÐÞ¸Ä   :
  ¹¦ÄÜÃèÊö   : 
  º¯ÊýÁÐ±í   :
  ÐÞ¸ÄÀúÊ·   :
  1.ÈÕ    ÆÚ   : 2016Äê3ÔÂ16ÈÕ ÐÇÆÚÈý
    ×÷    Õß   : xiao wei
    ÐÞ¸ÄÄÚÈÝ   : ´´½¨ÎÄ¼þ

******************************************************************************/
#include <sys/types.h>
#include <sys/time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#include "ap_types.h"
#include "lte.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "lualib_lte.h"

#define MAX_BUFFER 128
#define DATA_SPACE 32
#define KEY_SPACE 512
typedef struct _vrrp{
    int  iRouterId;
    int  iInterface;
    int  iMaster;
    int  iMode;
    int  iVirtualRouterId;
    int  iPriority;
    int  iAuthType;
    char acAuthPasswd[DATA_SPACE];
    char acVirtualIp[DATA_SPACE];
    char acWanVirtualIp[DATA_SPACE];
    char acTrackInterface[DATA_SPACE];
}stVrrp;

typedef struct _ipsec_psk
{
  char acPfs[DATA_SPACE];
  char acIke_1[DATA_SPACE];
  char acIke_2[DATA_SPACE];
  char acPhase[DATA_SPACE];
  char acPhaseAlg_1[DATA_SPACE];
  char acPhaseAlg_2[DATA_SPACE];
  char acIkeLifeTime[DATA_SPACE];
  char acSaLifeTime[DATA_SPACE];
  char acPsk[KEY_SPACE];
  char acSrcIp[DATA_SPACE];
  char acSrcMask[DATA_SPACE];
  char acSrcSubNet[DATA_SPACE];
  char acSrcNextHop[DATA_SPACE];
  char acDesIp[DATA_SPACE];
  char acDesMask[DATA_SPACE];
  char acDesSubNet[DATA_SPACE];
  char acDesNextHop[DATA_SPACE];
  
}LUA_IPSEC_PSK;

typedef struct _ipsec_rsa
{
  char acLocalId[DATA_SPACE];
  char acRemoteId[DATA_SPACE];
  char acRemoteKey[KEY_SPACE];
  char acPfs[DATA_SPACE];
  char acIke_1[DATA_SPACE];
  char acIke_2[DATA_SPACE];
  char acPhase[DATA_SPACE];
  char acPhaseAlg_1[DATA_SPACE];
  char acPhaseAlg_2[DATA_SPACE];
  char acIkeLifeTime[DATA_SPACE];
  char acSaLifeTime[DATA_SPACE];
  char acPsk[DATA_SPACE];
  char acSrcIp[DATA_SPACE];
  char acSrcMask[DATA_SPACE];
  char acSrcSubNet[DATA_SPACE];
  char acSrcNextHop[DATA_SPACE];
  char acDesIp[DATA_SPACE];
  char acDesMask[DATA_SPACE];
  char acDesSubNet[DATA_SPACE];
  char acDesNextHop[DATA_SPACE];
  /* data */
}LUA_IPSEC_RSA;

typedef struct _ipsec_x509
{ 
  char acPfs[DATA_SPACE];
  char acIke_1[DATA_SPACE];
  char acIke_2[DATA_SPACE];
  char acPhase[DATA_SPACE];
  char acPhaseAlg_1[DATA_SPACE];
  char acPhaseAlg_2[DATA_SPACE];
  char acIkeLifeTime[DATA_SPACE];
  char acSaLifeTime[DATA_SPACE];
  char acSrcIp[DATA_SPACE];
  char acSrcMask[DATA_SPACE];
  char acSrcSubNet[DATA_SPACE];
  char acSrcNextHop[DATA_SPACE];
  char acDesIp[DATA_SPACE];
  char acDesMask[DATA_SPACE];
  char acDesSubNet[DATA_SPACE];
  char acDesNextHop[DATA_SPACE];
  char acSrcCert[DATA_SPACE];
  char acDesCert[DATA_SPACE];
  char acPasswd[DATA_SPACE];    // passwd of private key file
  char acPrivateKey[DATA_SPACE];// private key  file
  char acRemoteId[DATA_SPACE];
  /* data */
}LUA_IPSEC_X509;

typedef struct _ipsec
{
  LUA_IPSEC_PSK  stIpsecPsk;
  LUA_IPSEC_RSA  stIpsecRsa;
  LUA_IPSEC_X509 stIpsecX509;
}LUA_IPSEC;

INT32 lua_lte_update_pin( LUA_LTE_PIN_CFG_T *pstLtePinCfg,int pin_sw)
{
    UINT32 iRet = 0; 
    char acTemp[4];
    char cCfgValue[CONFIG_VALUE_MAX_LEN];
    LUA_LTE_PIN_STATE_T stPinState;
    LTE_PIN_SWITCH_T    stPinSwitch;
    LTE_PIN_ALTER_T     stPinAlter;
    LTE_PIN_T stPin;
    UINT32 auiTmpPinCode;
    UINT32 uiTmpPinState = 0;
    UINT32 uiTmpPinFunctionSelect = 0;

    memset(&acTemp,0,sizeof(acTemp));
    memset(&stPinState,0,sizeof(stPinState));
    memset(&stPinSwitch,0,sizeof(stPinSwitch));
    memset(&stPinAlter,0,sizeof(stPinAlter));
    memset(&stPin,0,sizeof(stPin));

    if ( NULL == pstLtePinCfg )
    {
        printf("pointer is null,%d",__LINE__);
        return LTE_FAIL;
    }


    /* 配置之前，先判断输入的pin码与配置文件中的pin码是否一致 */
    memset(cCfgValue,0,sizeof(cCfgValue));
    if(UCI_OK != uci_config_get(UCI_LTE_GET_PIN_CODE,cCfgValue))
    {
        printf("uci_config_Get UCI_LTE_GET_PIN_CODE failed");
        return UCI_FAIL;
    }
    else
    {
        if (PIN_FUNCTION_OFF == pstLtePinCfg->eLtePinFuncSelect ||
            PIN_FUNCTION_ON == pstLtePinCfg->eLtePinFuncSelect)
        {
          iRet = strncmp(pstLtePinCfg->acPinCode,cCfgValue,4);

            if ( iRet > 0 )
            {
                pstLtePinCfg->eAlertFlag = LUA_LTE_WEB_ALERT_PIN_CODE_ERR;
                printf("pin code(%s) error",pstLtePinCfg->acPinCode);
                
                return LTE_FAIL;
            }
        }
        else
        {
            if ( strncmp(pstLtePinCfg->acOldPinCode,cCfgValue,4) )
            {
                pstLtePinCfg->eAlertFlag = LUA_LTE_WEB_ALERT_PIN_CODE_ERR;
                printf("pin code(%s) error",pstLtePinCfg->acPinCode);
                
                return LTE_FAIL;
            }
        }       
    }
    /* 获取当前的pin锁开关状态 */
    iRet = om_lte_pin_state_get(&stPinState);
    if ( LTE_OK != iRet )
    {
        goto PIN_CONFIG_ERROR;
    }

    if ( PIN_SWITCH_OFF == stPinState.stPinSwitch.enState)
    {
        uiTmpPinState = LTE_SWITCH_OFF;
    }
    else 
    {
        uiTmpPinState = LTE_SWITCH_ON;
    }

    uiTmpPinFunctionSelect = pstLtePinCfg->eLtePinFuncSelect;
    if(pin_sw == 0)
    {
    /* 关闭pin功能 */
      if ( PIN_FUNCTION_OFF == uiTmpPinFunctionSelect )
      {
        /* 当前pin锁功能开启，且外部配置关闭pin功能，需要先解pin码
            1、获取之前的pin码
            2、解pin码
            3、关闭pin开关
        */
          if ( LTE_SWITCH_OFF== uiTmpPinState )
          {
              printf("no need update pin state,%d \n", __LINE__);
              //pstLtePinCfg->uiPinInputRemain=3;
          }
          else
          {
              iRet = om_lte_pin_off(&stPinState,pstLtePinCfg->acPinCode);
             if ( LTE_OK != iRet) 
             {
                  goto PIN_CONFIG_ERROR;
              }  
          }
      }
                  uci_config_set(UCI_LTE_SET_PIN_LOCK_SW,"0");
                  uci_config_commit();
    }
    else if(pin_sw == 1)
    {

    /* 开启pin功能 */

        /* 当前pin锁功能关闭，且外部配置开启pin功能，开启pin功能
            1、获取pin code
            2、开启pin 锁
        */
          if ( LTE_SWITCH_OFF== uiTmpPinState )
          {
              iRet = om_lte_pin_on(&stPinState,pstLtePinCfg->acPinCode);
              if ( LTE_OK != iRet) 
              {
                  goto PIN_CONFIG_ERROR;
              }
              else
              {
                  uci_config_set(UCI_LTE_SET_PIN_LOCK_SW,"1");
                  uci_config_commit();
              } 
          }
            /* 当前lte设备已经开启pin功能，不再重复开启 */
          else
          {
            //pstLtePinCfg->uiPinInputRemain=3;
          }
      }
    /* 修改pin码 */
    else if( pin_sw == 2)
    {
        /* BEGIN: Modified by zhangjiasi, 2015/4/29   PN:修改pin码时，如果pin禁用，则先开启 */
        uiTmpPinState = stPinState.stPinSwitch.enState;
        if ( LTE_SWITCH_OFF == stPinState.stPinSwitch.enState)
        {
           iRet = om_lte_pin_on(&stPinState,pstLtePinCfg->acOldPinCode);
           if ( LTE_OK != iRet )
           {
               return iRet;
           }
           else
           {
               stPinState.stPinSwitch.enState= PIN_SWITCH_ON;
           }
        }
        /* END:   Modified by zhangjiasi, 2015/4/29   PN:修改pin码时，如果pin禁用，则先开启 */
        iRet = om_lte_pin_code_alter(&stPinState,pstLtePinCfg->acOldPinCode, pstLtePinCfg->acNewPinCode);
        if ( LTE_OK != iRet ) 
        {
            goto PIN_CONFIG_ERROR;
        }
        else 
        {
            /* 修改完pin码后，pin都将处于on状态，如果此前是off，需要将pin状
               态也off */
           if ( LTE_SWITCH_OFF == uiTmpPinState )
           {
               iRet = om_lte_pin_off(&stPinState,pstLtePinCfg->acNewPinCode);
               if ( LTE_OK != iRet )
               {
                   return LTE_FAIL;
               }
           }
        }
                  sprintf(acTemp,"%s", pstLtePinCfg->acNewPinCode);
                  if(UCI_OK != uci_config_set(UCI_LTE_GET_PIN_CODE,(char *)acTemp))
                  {
                      printf("set pin code fail\n");
                  }
                  else
                  {
                      uci_config_set(UCI_LTE_SET_PIN_LOCK_SW,"2");
                      uci_config_commit();
                  }
    }

    //pstLtePinCfg->uiPinInputRemain=3;
    return LTE_OK;

PIN_CONFIG_ERROR:
    
    if ( LTE_WAIT == iRet )
    {
        pstLtePinCfg->eAlertFlag = LUA_LTE_WEB_ALERT_LTE_BUSY;
    }
    else
    {
        //--pstLtePinCfg->uiPinInputRemain;
        pstLtePinCfg->eAlertFlag = LUA_LTE_WEB_ALERT_LTE_MODULE_CONTROL_FAILED;
    }
    
    return LTE_FAIL;
}

int lua_pin_opt(lua_State *L)
{
    INT32 iRet  = 0; 
    int ipin_sw = 0;
    char *pcOldPinCode;
    char *pcNewPinCode;
    char acTemp[CONFIG_VALUE_MAX_LEN];
    LTE_PIN_STATE_T        stState;
    LUA_LTE_PIN_CFG_T      stLtePinCfg;
    LUA_LTE_PIN_CFG_T      stLtePinCfgOld;  
    LTE_PIN_INPUT_REMAIN_T stPinIputRemain;

    memset(acTemp,0,sizeof(acTemp));
    memset(&stState,0,sizeof(stState));
    memset(&stLtePinCfg,0,sizeof(stLtePinCfg));
    memset(&stLtePinCfgOld,0,sizeof(stLtePinCfgOld)); 
    memset(&stPinIputRemain,0,sizeof(stPinIputRemain));

    ipin_sw      = luaL_checknumber(L,1); 
    pcOldPinCode = (char *)luaL_checkstring(L,2);
    pcNewPinCode = (char *)luaL_checkstring(L,3);
	
    sprintf(stLtePinCfg.acPinCode,"%s",pcOldPinCode); 
    sprintf(stLtePinCfg.acOldPinCode,"%s",pcOldPinCode); 
    sprintf(stLtePinCfg.acNewPinCode,"%s",pcNewPinCode);
	
    /* 读  取  剩  余  的  pin码  可  输  入  次  数 */
    
    if(LTE_OK != lte_pin_input_remain_get(&stPinIputRemain))
    {
        if(LTE_OK != lte_pin_state_get(&stState))
        {

        }
        else
        {
            if(PIN_STATE_READY == stState.enState || PIN_STATE_PIN == stState.enState)
            {
                stPinIputRemain.iPinIputRemain = 3;
            }
        }
    }
    stLtePinCfg.uiPinInputRemain = stPinIputRemain.iPinIputRemain;

    /* 判  断  pin码  输  入  次  数  已  经  达  到  上  限 */
    
    if ( stLtePinCfg.uiPinInputRemain <= 0)
    {       
        stLtePinCfg.eAlertFlag = LUA_LTE_WEB_ALERT_PIN_INPUT_REMAIN_ZERO;
        return LTE_FAIL;
    }
  
    iRet = lua_lte_update_pin(&stLtePinCfg,ipin_sw);
    if(iRet != LTE_OK)
    {
        return LTE_FAIL;
    }

    lua_pushnumber(L,1);
 
  return LUA_SUCCESS;
}

int lua_simlock_set(lua_State* L)
{

  INT32 iRet = 0; 
  char acTemp[CONFIG_VALUE_MAX_LEN] = {0};
  UINT32 uiTmpSimlockSwOld = 0;
  LUA_LTE_PIN_CFG_T stLtePinCfg;
  LUA_LTE_PIN_CFG_T stLtePinCfgOld; 
      
  memset(acTemp,0,sizeof(acTemp));
  memset(&stLtePinCfg,0,sizeof(stLtePinCfg));
  memset(&stLtePinCfgOld,0,sizeof(stLtePinCfgOld));

  stLtePinCfg.eSimlockSw = luaL_checknumber(L,1);
  uci_config_get(UCI_LTE_GET_SIM_LOCK_SW,(char *)acTemp);
  stLtePinCfgOld.eSimlockSw = atoi(acTemp);

    if(PIN_SIMLOCK_ON == stLtePinCfg.eSimlockSw)
    { 
        if (PIN_SIMLOCK_ON == stLtePinCfgOld.eSimlockSw)
        {

        }
        else
        {
            iRet = om_lte_pin_simlock_on();
            if ( LTE_OK != iRet )
            {
               return LTE_FAIL;
            }
            else
            {
            }
        }
        
        uci_config_set(UCI_LTE_GET_SIM_LOCK_SW,"1");
        uci_config_commit();
    }
    else if(PIN_SIMLOCK_OFF == stLtePinCfg.eSimlockSw)
    {

        if(PIN_SIMLOCK_ON == stLtePinCfgOld.eSimlockSw)
        {
            iRet = om_lte_pin_simlock_off();
            if ( LTE_OK != iRet )
            {
                return LTE_FAIL;
            }
            else
            {
            }
        }
        else
        {
        }
        
        uci_config_set(UCI_LTE_GET_SIM_LOCK_SW,"0");
        uci_config_commit();
    }
    else if ( PIN_SIMLOCK_SIM_EXCHANGE == stLtePinCfg.eSimlockSw)
    {          
        uci_config_set(UCI_LTE_GET_SIM_LOCK_SW,"0");

        if(UCI_OK != uci_config_get(UCI_LTE_GET_PIN_SIMLOCK_CODE,(char *)acTemp))
        {
            printf("UCI_LTE_GET_PIN_SIMLOCK_CODE get fail");
        }
        else
        {
            if(UCI_OK != uci_config_set(UCI_LTE_SET_PIN_SIMLOCK_CODE_OLD,(char *)acTemp))
            {
              printf("UCI_LTE_SET_PIN_SIMLOCK_CODE_OLD set fail\n");
            }
        }

        if(UCI_OK != uci_config_set(UCI_LTE_GET_PIN_SIMLOCK_CODE,"0"))
        {
            printf("UCI_LTE_GET_PIN_SIMLOCK_CODE set fail\n");
        }

        if(UCI_OK != uci_config_set(UCI_LTE_SET_PIN_FUNCTION_SELECT,"0"))
        {
            printf("UCI_LTE_SET_PIN_FUNCTION_SELECT set fail");
        }

        if(UCI_OK != uci_config_set(UCI_LTE_SET_PIN_UNLOCK_SW,"0"))
        {
            printf("UCI_LTE_SET_PIN_UNLOCK_SW set fail\n");
        }

        if(UCI_OK != uci_config_set(UCI_LTE_GET_PIN_CODE,"1234"))
        {
            printf("UCI_LTE_GET_PIN_CODE set fail\n");
        }
          
        if(UCI_OK != uci_config_set(UCI_LTE_SET_PIN_OLD_CODE,"0"))
        {
            printf("UCI_LTE_SET_PIN_OLD_CODE set fail\n");
        }

        if(UCI_OK != uci_config_set(UCI_LTE_SET_PIN_NEW_CODE,"0"))
        {
            printf("UCI_LTE_SET_PIN_NEW_CODE set fail\n");
        }
       
        uci_config_commit();
    }

  lua_pushnumber(L,1);
  return LUA_SUCCESS;
}
   
int lua_dev_rxtx_get(lua_State* L)
{
	FILE * fp_br0;
	char acCmd[128]={0};
	char p_buff[128]={0};
	char rxInfo[128]={0};
	char txInfo[128]={0};
	int rxpacks=0,rxerror=0,rxdrop=0;
	int txpacks=0,txerror=0,txdrop=0;
	
	snprintf(acCmd,sizeof(acCmd),"%s","cat /proc/net/dev | grep eth1 | cut -d: -f2");
	fp_br0=popen(acCmd,"r");
	if( !fp_br0 )
	{
		strcpy(p_buff,"");
	}
	else 
	{
		fgets(p_buff,sizeof(p_buff),fp_br0);
		pclose(fp_br0);
	}
	
	sscanf(p_buff,"%*d %d %d %d %*d %*d %*d %*d %*d %d %d %d",&rxpacks,&rxerror,&rxdrop,&txpacks,&txerror,&txdrop);
	
	snprintf(rxInfo,CHAR_LEN_32,"%d/%d/%d",rxpacks,rxerror,rxdrop);
	snprintf(txInfo,CHAR_LEN_32,"%d/%d/%d",txpacks,txerror,txdrop);
	
	lua_pushstring(L,rxInfo);
	lua_pushstring(L,txInfo);
	
	return LUA_RET_2;//?¨°??¨º?return 2 ¦Ì¡À??¡¤¦Ì??????¨ºy?Y¨º?¡ê??¨ª¨®|??return(¨ºy?Y¦Ì???¨ºy)
}
int lua_lastroute_set(lua_State* L)
{
	LTE_POST_ROUTE_SW_T ltePostRoute={0};
	
	int last_sw = luaL_checknumber(L,1);

	ltePostRoute.iPostRouteSw = last_sw; 
	if(LTE_OK != lte_post_route_set(&ltePostRoute))
    {
        last_sw = -1;
    }
    else 
    {
    	last_sw = 0;
    }
    
    lua_pushnumber(L,last_sw);
    
	return LUA_SUCCESS;	
}	

int lua_lockfrequency_set(lua_State* L)
{
  INT32 iRet = 0;
  INT32 iBandStart = 0;
  INT32 iBandEnd   = 0;
  char *acCheckCelid;
  char *acCheckFreq;

  LTE_CLK_CELL_T stCellLock = {0}; 




  stCellLock.iLockMode  = luaL_checknumber(L,1);
  acCheckFreq            = (char *)luaL_checkstring(L,2);
  acCheckCelid           = (char *)luaL_checkstring(L,3);
  iBandStart             = luaL_checknumber(L,4);
  iBandEnd               = luaL_checknumber(L,5);
   
  sprintf(stCellLock.acCellid,"%s",acCheckCelid);
  sprintf(stCellLock.acFreq,"%s",acCheckFreq);
  sprintf(stCellLock.acBandEnd,"%d",iBandEnd);
  sprintf(stCellLock.acBandStart,"%d",iBandStart);
	syslog(3,"lte lockfreq :%d %s %s %s %s\n",stCellLock.iLockMode,stCellLock.acCellid,
			stCellLock.acFreq,stCellLock.acBandEnd,stCellLock.acBandStart);	
			
	iRet = lte_clck_cell_set(&stCellLock); 
	syslog(3,"iRet = %d LTE_OK = %d\n",iRet,LTE_OK);
	if(iRet != LTE_OK) 
	{
		lua_pushnumber(L,-1);
	}else{
		lua_pushnumber(L,1);
	}
	return LUA_SUCCESS;
}

int lua_vrrp(lua_State* L)
{
  stVrrp stVrrpInstatce;
  FILE *fp;
  int iRet;
  char *acPasswd;
  char *acVirtualIp;
  memset(&stVrrpInstatce,0,sizeof(stVrrpInstatce));
  memset(&acPasswd,0,sizeof(acPasswd));
  memset(&acVirtualIp,0,sizeof(acVirtualIp));

  stVrrpInstatce.iRouterId        = luaL_checknumber(L,1);
  stVrrpInstatce.iMode            = luaL_checknumber(L,2);
  stVrrpInstatce.iVirtualRouterId = luaL_checknumber(L,3);
  stVrrpInstatce.iPriority        = luaL_checknumber(L,4);
  stVrrpInstatce.iAuthType        = luaL_checknumber(L,5);
  acPasswd                        = (char *)luaL_checkstring(L,6);
  acVirtualIp                     = (char *)luaL_checkstring(L,7);
  stVrrpInstatce.iMaster          = luaL_checknumber(L,8);
  stVrrpInstatce.iInterface       = luaL_checknumber(L,9);
  char *pWanIP                    = luaL_checkstring(L,10);
  int iTrackInterface             = luaL_checknumber(L,11);

  sprintf(stVrrpInstatce.acAuthPasswd,"%s",acPasswd);
  sprintf(stVrrpInstatce.acVirtualIp,"%s",acVirtualIp);
  sprintf(stVrrpInstatce.acWanVirtualIp,"%s",pWanIP);
  sprintf(stVrrpInstatce.acTrackInterface,"eth1.%d",iTrackInterface);

  fp = fopen("/etc/keepalived/keepalived.conf","w+");
  if(fp == NULL)
  {
    return false;
  }
  char data1[MAX_BUFFER];
  char data2[MAX_BUFFER];
  char data3[MAX_BUFFER];
  char data4[MAX_BUFFER];
  char data5[MAX_BUFFER];
  char data6[MAX_BUFFER];
  char data7[MAX_BUFFER];
  char data8[MAX_BUFFER];
  char data9[MAX_BUFFER];
  char data10[MAX_BUFFER];
  char data11[MAX_BUFFER];
  char data12[MAX_BUFFER];

  memset(&data1[MAX_BUFFER],0,sizeof(data1[MAX_BUFFER]));
  memset(&data2[MAX_BUFFER],0,sizeof(data2[MAX_BUFFER]));
  memset(&data3[MAX_BUFFER],0,sizeof(data3[MAX_BUFFER]));
  memset(&data4[MAX_BUFFER],0,sizeof(data4[MAX_BUFFER]));
  memset(&data5[MAX_BUFFER],0,sizeof(data5[MAX_BUFFER]));
  memset(&data6[MAX_BUFFER],0,sizeof(data6[MAX_BUFFER]));
  memset(&data7[MAX_BUFFER],0,sizeof(data7[MAX_BUFFER]));
  memset(&data8[MAX_BUFFER],0,sizeof(data8[MAX_BUFFER]));
  memset(&data9[MAX_BUFFER],0,sizeof(data9[MAX_BUFFER]));
  memset(&data10[MAX_BUFFER],0,sizeof(data10[MAX_BUFFER]));
  memset(&data11[MAX_BUFFER],0,sizeof(data11[MAX_BUFFER]));
  memset(&data12[MAX_BUFFER],0,sizeof(data12[MAX_BUFFER]));

  sprintf(data1,"global_defs {\n    router_id %d\n}\n",stVrrpInstatce.iRouterId);
  sprintf(data2,"vrrp_sync_group VG_1{\n    group {\n    VI_1\n");
  sprintf(data3,"    }\n    notify_master %c/sbin/to_master.sh%c\n",34,34);
  sprintf(data4,"    notify_backup %c/sbin/to_backup.sh%c\n    notify_fault %c/sbin/to_fault.sh%c\n}\n",34,34,34,34);
  if(stVrrpInstatce.iMaster == 0)
  {
      sprintf(data5,"vrrp_instance VI_1 {\n    state MASTER\n    interface %s\n",stVrrpInstatce.acTrackInterface);
    
  }
  else
  { 
      sprintf(data5,"vrrp_instance VI_1 {\n    state BACKUP\n    interface %s\n",stVrrpInstatce.acTrackInterface);
     
  }
  sprintf(data6,"    track_interface {\n    usb0\n    %s\n",stVrrpInstatce.acTrackInterface);
  sprintf(data7,"    }\n    garp_master_delay 1\n    version 3\n" );
  sprintf(data8,"    use_vmac uvmac\n");
  sprintf(data9,"    virtual_router_id %d\n    priority %d\n    advert_int 0.1\n",stVrrpInstatce.iVirtualRouterId,stVrrpInstatce.iPriority);
  if(stVrrpInstatce.iAuthType == 0)
  {
    sprintf(data10,"authentication {\n        auth_type PASS\n        auth_pass %s\n",stVrrpInstatce.acAuthPasswd);
  }
  else
  {
    sprintf(data10,"authentication {\n        auth_type AH\n        auth_pass %s\n",stVrrpInstatce.acAuthPasswd);
  }
  
  if(stVrrpInstatce.iMode == 0)
  {
    sprintf(data11,"    }\n    nopreempt\n    virtual_ipaddress {\n       %s",stVrrpInstatce.acVirtualIp);
    sprintf(data12,"\n    }\n}");
  }
   else
  {
    sprintf(data11,"    }\n    preempt\n    preempt_delay 1\n");
    sprintf(data12,"virtual_ipaddress {\n       %s\n    }\n}",stVrrpInstatce.acVirtualIp);

    if(stVrrpInstatce.iMaster == 0)
    {
      sprintf(data11,"    }\n    preempt\n    preempt_delay 1\n");
      sprintf(data12,"virtual_ipaddress {\n       %s\n    }\n}",stVrrpInstatce.acVirtualIp);
    }
    else{
      sprintf(data11,"    }\n    preempt\n");
      sprintf(data12,"virtual_ipaddress {\n       %s\n    }\n}",stVrrpInstatce.acVirtualIp);
    }
  }
  
  fwrite(data1,1,strlen(data1),fp);
  fwrite(data2,1,strlen(data2),fp);
  fwrite(data3,1,strlen(data3),fp);
  fwrite(data4,1,strlen(data4),fp);
  fwrite(data5,1,strlen(data5),fp);
  fwrite(data6,1,strlen(data6),fp);
  fwrite(data7,1,strlen(data7),fp);
  fwrite(data8,1,strlen(data8),fp);
  fwrite(data9,1,strlen(data9),fp);
  fwrite(data10,1,strlen(data10),fp);
  fwrite(data11,1,strlen(data11),fp);
  fwrite(data12,1,strlen(data12),fp);
  
  fclose(fp);
  fp = NULL;

  lua_pushnumber(L,1);
  return LUA_SUCCESS;
}

char *match(int select,int num)
{
  char *acIkeMap[10]      = {"des-sha1","des-md5","3des-sha1","3des-md5","aes-sha1","aes-md5","aes128-sha1","aes128-md5","aes256-sha1","aes256-md5"};
  char *acDHMap[3]        = {"modp768","modp1024","modp2048","OFF"};
  char *acBoolMap[2]      = {"yes","no"};
  char *acIPsecAuthMap[2] = {"esp","ah"};
  char *acStateMap[2]     = {"MASTER","BACKUP"};
  char *acInterfaceMap[2] = {"br-lan","usb0"};
  char *acVRRPAuthMap[2]  = {"PASS","AH"};

  if(select == 1)
  {
    return acIkeMap[num];
  }
  else if(select == 2)
  {
    return acDHMap[num];
  }
  else if(select == 3)
  {
    return acBoolMap[num];
  }
  else if(select == 4)
  {
    return acIPsecAuthMap[num];
  }

}

int lua_ipsec_psk(lua_State* L)
{
  LUA_IPSEC stIpsec;
  FILE *fp_buff;
  FILE *fp;

  int iPfs            = luaL_checknumber(L,2);
  int iIke_1          = luaL_checknumber(L,3);
  int iIke_2          = luaL_checknumber(L,4);
  int iPhase          = luaL_checknumber(L,5);
  int iPhasealg1      = luaL_checknumber(L,6);
  int iPhasealg2      = luaL_checknumber(L,7);
  char *pcIkeLifeTime = luaL_checkstring(L,8);
  char *pcSaLifeTime  = luaL_checkstring(L,9);
  char *pPsk          = luaL_checkstring(L,10);
  char *pSrcIp        = luaL_checkstring(L,11);
  char *pSrcMask      = luaL_checkstring(L,12);
  char *pSrcSubNet    = luaL_checkstring(L,13);
  char *pSrcNextHop   = luaL_checkstring(L,14);
  char *pDesIp        = luaL_checkstring(L,15);
  char *pDesMask      = luaL_checkstring(L,16);
  char *pDesSubNet    = luaL_checkstring(L,17);
  char *pDesNextHop   = luaL_checkstring(L,18);
  char acData[14][MAX_BUFFER];
  memset(&acData[14][MAX_BUFFER],0,sizeof(acData[14][MAX_BUFFER]));

  sprintf(stIpsec.stIpsecPsk.acIke_1,"%s",match(1,iIke_1));
  sprintf(stIpsec.stIpsecPsk.acIke_2,"%s",match(2,iIke_2));
  sprintf(stIpsec.stIpsecPsk.acPfs,"%s",match(3,iPfs));
  sprintf(stIpsec.stIpsecPsk.acPhase,"%s",match(4,iPhase));
  sprintf(stIpsec.stIpsecPsk.acPhaseAlg_1,"%s",match(1,iPhasealg1));
  sprintf(stIpsec.stIpsecPsk.acPhaseAlg_2,"%s",match(2,iPhasealg2));
  sprintf(stIpsec.stIpsecPsk.acIkeLifeTime,"%s",pcIkeLifeTime);
  sprintf(stIpsec.stIpsecPsk.acSaLifeTime,"%s",pcSaLifeTime);
  sprintf(stIpsec.stIpsecPsk.acPsk,"%s",pPsk);
  sprintf(stIpsec.stIpsecPsk.acSrcIp,"%s",pSrcIp);
  sprintf(stIpsec.stIpsecPsk.acSrcMask,"%s",pSrcMask);
  sprintf(stIpsec.stIpsecPsk.acSrcSubNet,"%s",pSrcSubNet);
  sprintf(stIpsec.stIpsecPsk.acSrcNextHop,"%s",pSrcNextHop);
  sprintf(stIpsec.stIpsecPsk.acDesIp,"%s",pDesIp);
  sprintf(stIpsec.stIpsecPsk.acDesMask,"%s",pDesMask);
  sprintf(stIpsec.stIpsecPsk.acDesSubNet,"%s",pDesSubNet);
  sprintf(stIpsec.stIpsecPsk.acDesNextHop,"%s",pDesNextHop);

  fp = fopen("/etc/ipsec.conf","w+");
  if(fp == NULL)
  {
    printf("Open /etc/ipsec.conf fail\n");
    return false;
  }

  fp_buff = fopen("/etc/ipsec.secrets","w+");
  if(fp_buff == NULL)
  {
    printf("open /etc/ipsec.secrets fail \n");
    return false;
  }

  sprintf(&acData[0][MAX_BUFFER],"# /etc/ipsec.conf - IPsec configuration file\n \nversion 2.0\n");
  sprintf(&acData[1][MAX_BUFFER],"include /etc/ipsec.uci.conf\n\n# Include non-UCI connections here\n");
  sprintf(&acData[2][MAX_BUFFER],"# They will be preserved across restarts/upgrades\nconfig setup\n");
  sprintf(&acData[3][MAX_BUFFER],"    interfaces=%cdefaultroute\n    nat_traversal=yes\n    protostack=netkey\n",37);
  sprintf(&acData[4][MAX_BUFFER],"conn net-to-net\n    authby=secret\n    ike=%s;%s",stIpsec.stIpsecPsk.acIke_1,stIpsec.stIpsecPsk.acIke_2);
  if(iPhasealg2 == 3)
  {
    sprintf(&acData[5][MAX_BUFFER],"\n    keyexchange=ike\n    phase2=%s\n    phase2alg=%s",stIpsec.stIpsecPsk.acPhase,stIpsec.stIpsecPsk.acPhaseAlg_1);
  }
  else{
    sprintf(&acData[5][MAX_BUFFER],"\n    keyexchange=ike\n    phase2=%s\n    phase2alg=%s;%s",stIpsec.stIpsecPsk.acPhase,stIpsec.stIpsecPsk.acPhaseAlg_1,stIpsec.stIpsecPsk.acPhaseAlg_2);
  }
  sprintf(&acData[6][MAX_BUFFER],"\n    salifetime=%sh\n    ikelifetime=%sh\n    left=%s",stIpsec.stIpsecPsk.acSaLifeTime,stIpsec.stIpsecPsk.acIkeLifeTime,stIpsec.stIpsecPsk.acSrcIp);
  sprintf(&acData[7][MAX_BUFFER],"\n    leftsubnet=%s/%s\n    leftnexthop=%s\n    right=%s",stIpsec.stIpsecPsk.acSrcSubNet,stIpsec.stIpsecPsk.acSrcMask,stIpsec.stIpsecPsk.acSrcNextHop,stIpsec.stIpsecPsk.acDesIp);
  sprintf(&acData[8][MAX_BUFFER],"\n    rightsubnet=%s/%s\n    rightnexthop=%s\n    pfs=%s",stIpsec.stIpsecPsk.acDesSubNet,stIpsec.stIpsecPsk.acDesMask,stIpsec.stIpsecPsk.acDesNextHop,stIpsec.stIpsecPsk.acPfs);
  sprintf(&acData[9][MAX_BUFFER],"\n    auto=add\n");
  sprintf(&acData[10][MAX_BUFFER],"# /etc/ipsec.secrets - IPsec sensitive configuration file\n \n # Include configuration information from UCI\n");
  sprintf(&acData[11][MAX_BUFFER],"include /etc/ipsec.uci.secrets\n \n# Add non-UCI secrets below\n");
  sprintf(&acData[12][MAX_BUFFER],"# This file will be preserved across restarts/upgrades\n");
  sprintf(&acData[13][MAX_BUFFER],"%s %s : PSK %c%s%c \n",stIpsec.stIpsecPsk.acSrcIp,stIpsec.stIpsecPsk.acDesIp,34,stIpsec.stIpsecPsk.acPsk,34);

  int iLoop;
  for(iLoop = 0;iLoop < 10; iLoop ++)
  {
     fwrite(&acData[iLoop][MAX_BUFFER],1,strlen(&acData[iLoop][MAX_BUFFER]),fp);
  }

  for(iLoop = 10; iLoop < 14; iLoop++)
  {
     fwrite(&acData[iLoop][MAX_BUFFER],1,strlen(&acData[iLoop][MAX_BUFFER]),fp_buff);
  }

  fflush(fp);
  fflush(fp_buff);
  fclose(fp);
  fclose(fp_buff);
  fp = NULL;
  fp_buff = NULL;
  lua_pushnumber(L,1);
  return LUA_SUCCESS;
}

int lua_ipsec_rsa(lua_State *L)
{
  LUA_IPSEC stIpsec;
  int iLoop = 0;
  FILE *fp;
  char acData[11][MAX_BUFFER];
  char acBuff[KEY_SPACE];
  int iPfs            = luaL_checknumber(L,2);
  int iIke_1          = luaL_checknumber(L,3);
  int iIke_2          = luaL_checknumber(L,4);
  int iPhase          = luaL_checknumber(L,5);
  int iPhasealg1      = luaL_checknumber(L,6);
  int iPhasealg2      = luaL_checknumber(L,7);
  char *pcIkeLifeTime = luaL_checkstring(L,8);
  char *pcSaLifeTime  = luaL_checkstring(L,9);
  char *pSrcIp        = luaL_checkstring(L,10);
  char *pSrcMask      = luaL_checkstring(L,11);
  char *pSrcSubNet    = luaL_checkstring(L,12);
  char *pSrcNextHop   = luaL_checkstring(L,13);
  char *pDesIp        = luaL_checkstring(L,14);
  char *pDesMask      = luaL_checkstring(L,15);
  char *pDesSubNet    = luaL_checkstring(L,16);
  char *pDesNextHop   = luaL_checkstring(L,17);
  char *pLocalId      = luaL_checkstring(L,18);
  char *pRemoteId     = luaL_checkstring(L,19);
  char *pRemoteKey    = luaL_checkstring(L,20);
  memset(acBuff,0,sizeof(acBuff[KEY_SPACE]));
  memset(&acData[11][MAX_BUFFER],0,sizeof(acData[11][MAX_BUFFER]));

  sprintf(stIpsec.stIpsecRsa.acIke_1,"%s",match(1,iIke_1));
  sprintf(stIpsec.stIpsecRsa.acIke_2,"%s",match(2,iIke_2));
  sprintf(stIpsec.stIpsecRsa.acPfs,"%s",match(3,iPfs));
  sprintf(stIpsec.stIpsecRsa.acPhase,"%s",match(4,iPhase));
  sprintf(stIpsec.stIpsecRsa.acPhaseAlg_1,"%s",match(1,iPhasealg1));
  sprintf(stIpsec.stIpsecRsa.acPhaseAlg_2,"%s",match(2,iPhasealg2));
  sprintf(stIpsec.stIpsecRsa.acIkeLifeTime,"%s",pcIkeLifeTime);
  sprintf(stIpsec.stIpsecRsa.acSaLifeTime,"%s",pcSaLifeTime);
  sprintf(stIpsec.stIpsecRsa.acSrcIp,"%s",pSrcIp);
  sprintf(stIpsec.stIpsecRsa.acSrcMask,"%s",pSrcMask);
  sprintf(stIpsec.stIpsecRsa.acSrcSubNet,"%s",pSrcSubNet);
  sprintf(stIpsec.stIpsecRsa.acSrcNextHop,"%s",pSrcNextHop);
  sprintf(stIpsec.stIpsecRsa.acDesIp,"%s",pDesIp);
  sprintf(stIpsec.stIpsecRsa.acDesMask,"%s",pDesMask);
  sprintf(stIpsec.stIpsecRsa.acDesSubNet,"%s",pDesSubNet);
  sprintf(stIpsec.stIpsecRsa.acDesNextHop,"%s",pDesNextHop);
  sprintf(stIpsec.stIpsecRsa.acLocalId,"%s",pLocalId);
  sprintf(stIpsec.stIpsecRsa.acRemoteId,"%s",pRemoteId);
  sprintf(stIpsec.stIpsecRsa.acRemoteKey,"%s",pRemoteKey);
 
  fp = fopen("/etc/ipsec.conf","w+");
  if(fp == NULL)
  {
    printf("Open /etc/ipsec.conf fail\n");
    return false;
  }

  sprintf(&acData[0][MAX_BUFFER],"# /etc/ipsec.conf - IPsec configuration file\n \nversion 2.0\n");
  sprintf(&acData[1][MAX_BUFFER],"include /etc/ipsec.uci.conf\n\n# Include non-UCI connections here\n");
  sprintf(&acData[2][MAX_BUFFER],"# They will be preserved across restarts/upgrades\nconfig setup\n");
  sprintf(&acData[3][MAX_BUFFER],"    interfaces=%cdefaultroute\n    nat_traversal=yes\n    protostack=netkey\n",37);
  sprintf(&acData[4][MAX_BUFFER],"conn net-to-net\n    authby=rsasig\n    ike=%s;%s",stIpsec.stIpsecRsa.acIke_1,stIpsec.stIpsecRsa.acIke_2);
  if(iPhasealg2 == 3)
  {
    sprintf(&acData[5][MAX_BUFFER],"\n    keyexchange=ike\n    phase2=%s\n    phase2alg=%s",stIpsec.stIpsecRsa.acPhase,stIpsec.stIpsecRsa.acPhaseAlg_1);
  }
  else{
    sprintf(&acData[5][MAX_BUFFER],"\n    keyexchange=ike\n    phase2=%s\n    phase2alg=%s;%s",stIpsec.stIpsecRsa.acPhase,stIpsec.stIpsecRsa.acPhaseAlg_1,stIpsec.stIpsecRsa.acPhaseAlg_2);
  }
  sprintf(&acData[6][MAX_BUFFER],"\n    pfs=%s\n    auto=add",stIpsec.stIpsecRsa.acPfs);
  sprintf(&acData[7][MAX_BUFFER],"\n    salifetime=%sh\n    ikelifetime=%sh\n    right=%s",stIpsec.stIpsecRsa.acSaLifeTime,stIpsec.stIpsecRsa.acIkeLifeTime,stIpsec.stIpsecRsa.acDesIp);
  sprintf(&acData[8][MAX_BUFFER],"\n    rightsubnet=%s/%s\n    rightnexthop=%s\n    rightid=%s",stIpsec.stIpsecRsa.acDesSubNet,stIpsec.stIpsecRsa.acDesMask,stIpsec.stIpsecRsa.acDesNextHop,stIpsec.stIpsecRsa.acRemoteId);
  sprintf(acBuff,"\n    rightrsasigkey=%s",stIpsec.stIpsecRsa.acRemoteKey);
  sprintf(&acData[9][MAX_BUFFER],"\n    left=%s\n    leftsubnet=%s/%s\n    leftnexthop=%s",stIpsec.stIpsecRsa.acSrcIp,stIpsec.stIpsecRsa.acSrcSubNet,stIpsec.stIpsecRsa.acSrcMask,stIpsec.stIpsecRsa.acSrcNextHop);
  sprintf(&acData[10][MAX_BUFFER],"\n    leftid=%s\n",stIpsec.stIpsecRsa.acLocalId);

  for(iLoop = 0;iLoop < 9; iLoop ++)
  {
     fwrite(&acData[iLoop][MAX_BUFFER],1,strlen(&acData[iLoop][MAX_BUFFER]),fp);
  }  
  fwrite(acBuff,1,strlen(acBuff),fp);
  fwrite(&acData[9][MAX_BUFFER],1,strlen(&acData[9][MAX_BUFFER]),fp);
  fwrite(&acData[10][MAX_BUFFER],1,strlen(&acData[10][MAX_BUFFER]),fp);

  fflush(fp);
  fclose(fp);
  fp = NULL;
  lua_pushnumber(L,1);
  return LUA_SUCCESS;
}

int lua_ipsec_x509(lua_State *L)
{
  LUA_IPSEC stIpsec;
  FILE *fp;
  FILE *fp_secret;
  int iLoop = 0;
  char acData[17][MAX_BUFFER];
  memset(&acData[17][MAX_BUFFER],0,sizeof(acData[17][MAX_BUFFER]));

  int iPfs           = luaL_checknumber(L,2);
  char *pPasswd      = luaL_checkstring(L,3);
  char *pSrcCert     = luaL_checkstring(L,4);
  char *pSrcIP       = luaL_checkstring(L,5);
  char *pSrcMask     = luaL_checkstring(L,6);
  char *pSrcSubnet   = luaL_checkstring(L,7);
  char *pDesCert     = luaL_checkstring(L,8);
  char *pDesIp       = luaL_checkstring(L,9);
  char *pDesMask     = luaL_checkstring(L,10);
  char *pDesSubnet   = luaL_checkstring(L,11);
  char *pPrivate     = luaL_checkstring(L,12);
  char *pIkelifetime = luaL_checkstring(L,13);
  char *pSalifetime  = luaL_checkstring(L,14);
  int iIke_1         = luaL_checknumber(L,15);
  int iIke_2         = luaL_checknumber(L,16);
  int iPhasealg1     = luaL_checknumber(L,17);
  int iPhasealg2     = luaL_checknumber(L,18);
  int iPhase         = luaL_checknumber(L,19);
  char *pSrcNextHop  = luaL_checkstring(L,20);
  char *pDesNextHop  = luaL_checkstring(L,21);
  char *pRemoteId    = luaL_checkstring(L,22);

  sprintf(stIpsec.stIpsecX509.acPfs,"%s",match(3,iPfs));
  sprintf(stIpsec.stIpsecX509.acPasswd,"%s",pPasswd);
  sprintf(stIpsec.stIpsecX509.acSrcCert,"%s",pSrcCert);
  sprintf(stIpsec.stIpsecX509.acSrcIp,"%s",pSrcIP);
  sprintf(stIpsec.stIpsecX509.acSrcMask,"%s",pSrcMask);
  sprintf(stIpsec.stIpsecX509.acSrcSubNet,"%s",pSrcSubnet);
  sprintf(stIpsec.stIpsecX509.acDesCert,"%s",pDesCert);
  sprintf(stIpsec.stIpsecX509.acDesIp,"%s",pDesIp);
  sprintf(stIpsec.stIpsecX509.acDesMask,"%s",pDesMask);
  sprintf(stIpsec.stIpsecX509.acDesSubNet,"%s",pDesSubnet);
  sprintf(stIpsec.stIpsecX509.acPrivateKey,"%s",pPrivate);
  sprintf(stIpsec.stIpsecX509.acIkeLifeTime,"%s",pIkelifetime);
  sprintf(stIpsec.stIpsecX509.acSaLifeTime,"%s",pSalifetime);
  sprintf(stIpsec.stIpsecX509.acIke_1,"%s",match(1,iIke_1));
  sprintf(stIpsec.stIpsecX509.acIke_2,"%s",match(2,iIke_2));
  sprintf(stIpsec.stIpsecX509.acPhase,"%s",match(4,iPhase));
  sprintf(stIpsec.stIpsecX509.acPhaseAlg_1,"%s",match(1,iPhasealg1));
  sprintf(stIpsec.stIpsecX509.acPhaseAlg_2,"%s",match(2,iPhasealg2));
  sprintf(stIpsec.stIpsecX509.acSrcNextHop,"%s",pSrcNextHop);
  sprintf(stIpsec.stIpsecX509.acDesNextHop,"%s",pDesNextHop);
  sprintf(stIpsec.stIpsecX509.acRemoteId,"%s",pRemoteId);


  fp = fopen("/etc/ipsec.conf","w+");
  if(fp == NULL)
  {
    printf("ipsec Open /etc/ipsec.conf fail\n");
    return false;
  }

  fp_secret = fopen("/etc/ipsec.secrets","w+");
  if(fp_secret == NULL)
  {
    printf("ipsec open /etc/ipsec.secrets fail \n");
    return false;
  }
  sprintf(&acData[0][MAX_BUFFER],"# /etc/ipsec.conf - IPsec configuration file\n \nversion 2.0\n");
  sprintf(&acData[1][MAX_BUFFER],"include /etc/ipsec.uci.conf\n\n# Include non-UCI connections here\n");
  sprintf(&acData[2][MAX_BUFFER],"# They will be preserved across restarts/upgrades\nconfig setup\n");
  sprintf(&acData[3][MAX_BUFFER],"    interfaces=%cdefaultroute\n    nat_traversal=yes\n    protostack=netkey\n",37);
  sprintf(&acData[4][MAX_BUFFER],"conn net-to-net\n    authby=rsasig\n    ike=%s;%s",stIpsec.stIpsecX509.acIke_1,stIpsec.stIpsecX509.acIke_2);
  if(iPhasealg2 == 3)
  {
    sprintf(&acData[5][MAX_BUFFER],"\n    keyexchange=ike\n    phase2=%s\n    phase2alg=%s",stIpsec.stIpsecX509.acPhase,stIpsec.stIpsecX509.acPhaseAlg_1);
  }
  else{
    sprintf(&acData[5][MAX_BUFFER],"\n    keyexchange=ike\n    phase2=%s\n    phase2alg=%s;%s",stIpsec.stIpsecX509.acPhase,stIpsec.stIpsecX509.acPhaseAlg_1,stIpsec.stIpsecX509.acPhaseAlg_2);
  }
  sprintf(&acData[6][MAX_BUFFER],"\n    salifetime=%sh\n    ikelifetime=%sh",stIpsec.stIpsecX509.acSaLifeTime,stIpsec.stIpsecX509.acIkeLifeTime,stIpsec.stIpsecX509.acSrcIp);
  sprintf(&acData[7][MAX_BUFFER],"\n    leftrsasigkey=%ccert\n    rightrsasigkey=%ccert",37,37);
  sprintf(&acData[8][MAX_BUFFER],"\n    left=%s\n    leftsubnet=%s/%s\n    leftcert=/etc/ipsec.d/certs/%s",stIpsec.stIpsecX509.acSrcIp,stIpsec.stIpsecX509.acSrcSubNet,stIpsec.stIpsecX509.acSrcMask,stIpsec.stIpsecX509.acSrcCert);
  if(0 == strcmp("null",stIpsec.stIpsecX509.acRemoteId))
  {
    sprintf(&acData[9][MAX_BUFFER],"\n    leftnexthop=%s",stIpsec.stIpsecX509.acSrcNextHop);
  }
  else{
    sprintf(&acData[9][MAX_BUFFER],"\n    leftnexthop=%s\n    rightid=%s",stIpsec.stIpsecX509.acSrcNextHop,stIpsec.stIpsecX509.acRemoteId);
  } 
  sprintf(&acData[10][MAX_BUFFER],"\n    right=%s\n    rightsubnet=%s/%s\n    rightcert=/etc/ipsec.d/certs/%s",stIpsec.stIpsecX509.acDesIp,stIpsec.stIpsecX509.acDesSubNet,stIpsec.stIpsecX509.acDesMask,stIpsec.stIpsecX509.acDesCert);
  sprintf(&acData[11][MAX_BUFFER],"\n    rightnexthop=%s",stIpsec.stIpsecX509.acDesNextHop);
  sprintf(&acData[12][MAX_BUFFER],"\n    pfs=%s\n    auto=add\n",stIpsec.stIpsecX509.acPfs);
  sprintf(&acData[13][MAX_BUFFER],"# /etc/ipsec.secrets - IPsec sensitive configuration file\n \n # Include configuration information from UCI\n");
  sprintf(&acData[14][MAX_BUFFER],"include /etc/ipsec.uci.secrets\n \n# Add non-UCI secrets below\n");
  sprintf(&acData[15][MAX_BUFFER],"# This file will be preserved across restarts/upgrades\n");
  sprintf(&acData[16][MAX_BUFFER],": RSA /etc/ipsec.d/private/%s %c%s%c\n",stIpsec.stIpsecX509.acPrivateKey,34,stIpsec.stIpsecX509.acPasswd,34);
  for(iLoop = 0;iLoop < 13; iLoop ++)
  {
     fwrite(&acData[iLoop][MAX_BUFFER],1,strlen(&acData[iLoop][MAX_BUFFER]),fp);
  }

  for(iLoop = 13; iLoop < 17; iLoop++)
  {
     fwrite(&acData[iLoop][MAX_BUFFER],1,strlen(&acData[iLoop][MAX_BUFFER]),fp_secret);
  }
  fflush(fp);
  fflush(fp_secret);
  fclose(fp);
  fclose(fp_secret);
  fp = NULL;
  fp_secret = NULL;
  lua_pushnumber(L,1);

  return LUA_SUCCESS;
}

int lua_ipsec(lua_State* L)
{
  int iSwitchNum = luaL_checknumber(L,1);
  
  if(iSwitchNum == 0)
  {
    lua_ipsec_psk(L);
  }
  else if(iSwitchNum == 1)
  {
    lua_ipsec_rsa(L);
  }
  else if(iSwitchNum == 2)
  {
    lua_ipsec_x509(L);
  }

  return LUA_SUCCESS;
}
int lua_lte_simswitch_set(lua_State* L)
{
	LTE_SIM_SWITCH_T stLteSimSwitch={0};
	
	int simSw = luaL_checknumber(L,1);

	stLteSimSwitch.iSimSw = simSw;
	
	if(0 == simSw)
	{
		char *simImsi = (char*)luaL_checkstring(L,2);
        
		int simMncLen = luaL_checknumber(L,3);
        
        #if 0
		int simSuathPara = luaL_checknumber(L,4);
		int simSualg = luaL_checknumber(L,5);
        #endif
		char *simKey = luaL_checkstring(L,6);
        
		char *simOp = luaL_checkstring(L,7);
        
			
		strncpy(stLteSimSwitch.acSimImsi, simImsi, sizeof(stLteSimSwitch.acSimImsi));
		stLteSimSwitch.iSimMnclen = simMncLen;
		//stLteSimSwitch.iSimSuauthpara = simSuathPara;
		//stLteSimSwitch.iSimSualg  = simSualg;
		strncpy(stLteSimSwitch.acSimKey, simKey, sizeof(stLteSimSwitch.acSimKey));
		strncpy(stLteSimSwitch.acSimOp, simOp, sizeof(stLteSimSwitch.acSimOp));
        
		if(LTE_OK != lte_sim_switch_soft_set(&stLteSimSwitch))
    	{
        	lua_pushnumber(L,-1);
        	return LUA_SUCCESS;	
    	}
	}
    
    //simSw = 0 : soft sim modle
	if(stLteSimSwitch.iSimSw){
		stLteSimSwitch.iSimChoice = 11;
		stLteSimSwitch.iSimEnable = 0;
	}else{
		stLteSimSwitch.iSimChoice = 1;
		stLteSimSwitch.iSimEnable = 0;
	}
	
	if(LTE_OK != lte_sim_switch_set(&stLteSimSwitch))
    {
        lua_pushnumber(L,-1);
        //return LUA_SUCCESS;	
    }
    
    if(stLteSimSwitch.iSimSw){
		stLteSimSwitch.iSimChoice = 1;
		stLteSimSwitch.iSimEnable = 1;
	}else{
		stLteSimSwitch.iSimChoice = 11;
		stLteSimSwitch.iSimEnable = 1;
	}
	
	if(LTE_OK != lte_sim_switch_set(&stLteSimSwitch))
    {
        lua_pushnumber(L,-1);
        //return LUA_SUCCESS;	
    }
    
    
    lua_pushnumber(L,0);
    
	return LUA_SUCCESS;	
}	

static luaL_Reg mylibs[] = { 
    {"lua_lastroute_set", lua_lastroute_set},
    {"lua_dev_rxtx_get", lua_dev_rxtx_get},
    {"lua_lockfrequency_set",lua_lockfrequency_set},
    {"lua_simlock_set",lua_simlock_set},
    {"lua_pin_opt",lua_pin_opt},
    {"lua_vrrp",lua_vrrp},
    {"lua_ipsec",lua_ipsec},
    {"lua_lte_simswitch_set", lua_lte_simswitch_set},
    {NULL, NULL} 
}; 

int luaopen_libluas2(lua_State* L) 
{
    const char* libName = "libluas2";
    luaL_register(L,libName,mylibs);
    
    return LUA_SUCCESS;
}

