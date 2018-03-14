#include "lualib_lte.h"

LTE_RET_E  om_lte_reset_by_platform( void )
{
    int iRet = 0;
    
    iRet = lte_power_off();
    if ( LTE_OK != iRet )
    {

        printf("lte_power_offF failed");
        return LTE_FAIL;
    }
    else
    {

        printf("lte_power_off success");
    }
    
    sleep(1);
    iRet = lte_power_on();
    if ( LTE_OK != iRet )
    {
        printf("lte_power_on");
        return iRet;
    }
    else
    {
        printf("lte_power_on success");
    }

    return LTE_OK;
}

LTE_RET_E  om_lte_pin_state_get( LUA_LTE_PIN_STATE_T *pstPinState)
{
    
    LTE_PIN_SW_STATE_T stSwitch = {0};
    LTE_PIN_STATE_T  stState = {0};
    INT32 iCount = 0;

    if ( NULL == pstPinState )
    {
        printf("pointer is null\n");
        return LTE_FAIL;
    }

    /* try to get pin state  */
    while( LTE_OK != lte_pin_state_get(&stState))
    {
        iCount++;
        printf(" get pin state failed, count[%d]", iCount);
        om_lte_reset_by_platform();
        if(iCount >= 3)
        {
            printf(" get pin state failed after [%d] times", iCount);
            return LTE_FAIL;
        }
    }    
/* try to get pin switch state  */
    if(PIN_STATE_READY == stState.enState)
    {
        if(LTE_OK != lte_pin_sw_get(&stSwitch))
        {
            printf("get pin switch failed\n");
            return LTE_FAIL;
        }
    }
    else
    {
        stSwitch.enState = PIN_SWITCH_ON;
    }

    pstPinState->stPinState.enState = stState.enState;
    pstPinState->stPinSwitch.enState = stSwitch.enState;

    printf("pin state:[%d],pin switch state:[%d]",
                            pstPinState->stPinState.enState,
                            pstPinState->stPinSwitch.enState);

    return LTE_OK;
}
LTE_RET_E  om_lte_pin_off(LUA_LTE_PIN_STATE_T *pstPinState ,CHAR *acPinCode )
{
    LTE_PIN_T        stPin;
    LTE_PIN_SWITCH_T stPinSwitch;
    UINT32 iRet = 0;

    memset(&stPin,0,sizeof(stPin));
    memset(&stPinSwitch,0,sizeof(stPinSwitch));

    if (NULL == pstPinState|| NULL == acPinCode )
    {
        return LTE_FAIL;
    }

    if(PIN_SWITCH_OFF == pstPinState->stPinSwitch.enState)
    {
        return LTE_OK;
    }

    if (PIN_STATE_PIN == pstPinState->stPinState.enState)
    {
        /* 解pin锁 */
        strncpy(stPin.acPin,acPinCode,4);

        iRet = lte_pin_unlock(&stPin);
        if ( LTE_OK != iRet) 
        {
            return iRet;
        }
        sleep(5);
    }
    else if(PIN_STATE_PUK == pstPinState->stPinState.enState)
    {
        return LTE_FAIL;
    }
    

    /* 禁用pin功能 */
    strncpy(stPinSwitch.acPin,acPinCode,4);
    stPinSwitch.enSwitch = PIN_SWITCH_OFF;

    iRet = lte_pin_switch(&stPinSwitch);
    if ( LTE_OK != iRet) 
    {
        return iRet;
    }  

    return LTE_OK;
}

LTE_RET_E  om_lte_pin_on( LUA_LTE_PIN_STATE_T *pstPinState,CHAR *acPinCode )
{
    LTE_PIN_T        stPin;
    LTE_PIN_SWITCH_T stPinSwitch;
    UINT32 iRet;

    memset(&stPin,0,sizeof(stPin));
    memset(&stPinSwitch,0,sizeof(stPinSwitch));

    if (NULL == pstPinState|| NULL == acPinCode )
    {
        printf("om_lte_pin_on pointer is null ");
        return LTE_FAIL;
    }

    if ( PIN_SWITCH_ON == pstPinState->stPinSwitch.enState)
    {
        printf("pin switch alreay on\n");
        return LTE_OK;
    }

    strncpy(stPinSwitch.acPin,acPinCode,4);
    stPinSwitch.enSwitch = PIN_SWITCH_ON;

    printf("pin code:%c%c%c%c\n",
                        stPinSwitch.acPin[0],
                        stPinSwitch.acPin[1],
                        stPinSwitch.acPin[2],
                        stPinSwitch.acPin[3]);
    
    iRet = lte_pin_switch(&stPinSwitch);
    if ( LTE_OK != iRet) 
    {
        printf("lte pin trun on failed\n");
        return LTE_FAIL;
    }

    printf("lte pin on success\n");

    return LTE_OK;
}

int  om_lte_get_device_sn( CHAR *szSn )
{
    FILE *pf;
    CHAR buf[512];
    CHAR *id;

    if ( NULL == szSn)
    {
        printf("pointer is null\n");
        return LTE_FAIL;
    }

    pf = popen("aptool getsn", "r");
    if(NULL == pf)
    {
        printf("get sn failed, default null\n");
        return LTE_FAIL;
    }

    memset(buf, 0, sizeof(buf));
    fread(buf, sizeof(buf), 1, pf);
    pclose(pf);
    if(NULL == (id = strstr(buf, ":")))
    {
        printf("get sn failed, default null\n");
        return LTE_FAIL;
    }
    id++;
    strcpy(szSn, id);

    id = strstr(szSn, "\n");
    if(NULL != id)
        *id = '\0';
    
    return LTE_OK;
}

int  om_lte_pin_code_random( CHAR *acRandomPinCode )
{
    INT32 iRet = 0;
    CHAR szSn[32];
    CHAR szDftSn[32] = "ac:34:cb:12:34:56";
    CHAR szDftIccid[32];
    LTE_ICCID_T stICCID;

    memset(szSn,0,sizeof(szSn));
    memset(&stICCID,0,sizeof(stICCID));
    memset(szDftIccid,0,sizeof(szDftIccid));

    if(NULL == acRandomPinCode)
    {
        printf("the pointer is null\n");
        return LTE_FAIL;
    }

    memset(szSn,0,sizeof(szSn));
    iRet = om_lte_get_device_sn(szSn);
    if ( LTE_OK != iRet )
    {
        printf("get device sn failed:%d,set default sn:<%s>",iRet,szDftSn);
        strncpy(szSn,szDftSn,strlen(szDftSn));
    }

    printf("device sn:<%s>",szSn);

    iRet = lte_iccid_get(&stICCID);
    if ( LTE_OK != iRet )
    {
        printf("get ICCID failed:%d",iRet);
        return LTE_FAIL;
    }

    printf("sim iccid:<%s>",stICCID.acICCID);

    sprintf(acRandomPinCode,"%d%d%d%d",szSn[15]%10,szSn[16]%10,stICCID.acICCID[17]%10,stICCID.acICCID[18]%10);

    printf("new pin code:<%c%c%c%c>",
                                    acRandomPinCode[0],
                                    acRandomPinCode[1],
                                    acRandomPinCode[2],
                                    acRandomPinCode[3]);

    if(UCI_OK != uci_config_set(UCI_LTE_GET_PIN_SIMLOCK_CODE,(char *)acRandomPinCode))
    {
        printf("uci_config_set UCI_LTE_GET_PIN_SIMLOCK_CODE fail");
        return LTE_FAIL;
    }
    else
    {
        uci_config_commit();
    }

    return LTE_OK;
}


LTE_RET_E  om_lte_pin_code_alter( LUA_LTE_PIN_STATE_T *pstPinState,CHAR *acPinOld, CHAR *acPinNew )
{
    INT32 iRet = 0;
    LTE_PIN_T       stPin;
    LTE_PIN_ALTER_T stPinAlter;
    
    memset(&stPin,0,sizeof(stPin));
    memset(&stPinAlter,0,sizeof(stPinAlter));

    if (NULL == pstPinState|| NULL == acPinOld || NULL == acPinNew)
    {
        printf("pointer is null");
       return LTE_FAIL;
    }

    if (PIN_SWITCH_OFF == pstPinState->stPinSwitch.enState)
    {
        printf("pin switch off now,need to open");
        return LTE_FAIL;
    }

    if (PIN_STATE_PIN == pstPinState->stPinState.enState)
    {
        /* 解pin锁 */
        strncpy(stPin.acPin,acPinOld,4);
    
        printf("pin code:%c%c%c%c\n",stPin.acPin[0],stPin.acPin[1],stPin.acPin[2],stPin.acPin[3]);
        iRet = lte_pin_unlock(&stPin);
        if ( LTE_OK != iRet ) 
        {
            printf("lte pin unlock failed");
            return LTE_FAIL;
        }
    }
    else if(PIN_STATE_PUK == pstPinState->stPinState.enState)
    {
        printf("pin state PUK");
        return LTE_FAIL;
    }

    strncpy(stPinAlter.acOldPin,acPinOld,4);
    strncpy(stPinAlter.acNewPin,acPinNew,4);
    uci_config_set(UCI_LTE_SET_PIN_OLD_CODE,(char *)&stPinAlter.acOldPin);
    uci_config_set(UCI_LTE_SET_PIN_NEW_CODE,(char *)&stPinAlter.acNewPin);

    printf("old pin code:%c%c%c%c\n" "new pin code:%c%c%c%c",
                         stPinAlter.acOldPin[0],
                         stPinAlter.acOldPin[1],
                         stPinAlter.acOldPin[2],
                         stPinAlter.acOldPin[3],
                         stPinAlter.acNewPin[0],
                         stPinAlter.acNewPin[1],
                         stPinAlter.acNewPin[2],
                         stPinAlter.acNewPin[3]);


    iRet = lte_pin_alter(&stPinAlter);
    if ( LTE_OK != iRet) 
    {
        printf("lte pin code alter failed");
        return LTE_FAIL;
    }

    printf("lte pin code alter success");

    return LTE_OK;
}

LTE_RET_E  om_lte_pin_simlock_on( void )
{
    /* 1、检  查  当   前   pin开   关   状   态
          #如  果  处   于   SIM on，往  下
          #如  果  处   于   SIM off，先  启   用  pin
       2、检  测  当   前  pin lock状  态
          #如  果  处  于   pin ready，往  下
          #如  果  处  于   sim pin状  态，需  进  行  pin unlock
       3、产  生  随   机  pin码
       4、修  改  pin码
    */
    INT32 iRet ;
    static UINT32 uiTmpPinState = 0;
    static CHAR acPinCode[4];
    static CHAR acRandomPinCode[4];
    LUA_LTE_PIN_STATE_T stPinState;
    char acTemp[CONFIG_VALUE_MAX_LEN];

    memset(acTemp,0,sizeof(acTemp));
    memset(acPinCode,0,sizeof(acPinCode));
    memset(&stPinState,0,sizeof(stPinState));
    memset(acRandomPinCode,0,sizeof(acRandomPinCode));
  
    /* 获  取  当   前  的  pin锁  开  关  状  态 */
    iRet = om_lte_pin_state_get(&stPinState);
    if ( LTE_OK != iRet )
    {
        printf("om_lte_pin_state_get");
        return iRet;
    }

    if ( PIN_SWITCH_OFF == stPinState.stPinSwitch.enState)
    {
        uiTmpPinState = LTE_SWITCH_OFF;
    }
    else
    {
        uiTmpPinState = LTE_SWITCH_ON;
    }

    iRet = uci_config_get(UCI_LTE_GET_PIN_CODE,(char *)acPinCode);
    if(iRet == UCI_FAIL)
    {
      printf("uci_config_get  UCI_LTE_GET_PIN_CODE fail");
      return LTE_FAIL;
    }

    if ( LTE_SWITCH_OFF == uiTmpPinState)
    {
       iRet = om_lte_pin_on(&stPinState,acPinCode);  
       if ( LTE_OK != iRet )
       {
           printf("om_lte_pin_on fail");
           return LTE_FAIL;
       }
       else
       {
           stPinState.stPinSwitch.enState= PIN_SWITCH_ON;
       }
    }
    /* 判断当前是否为首次插卡 */
    if(UCI_FAIL == uci_config_get(UCI_LTE_GET_PIN_SIMLOCK_CODE,(char *)acRandomPinCode))
    {   
        printf("uci_config_get UCI_LTE_PIN_SIMLOCK_CODE fail");
    }
    else
    {
        if(strcmp((char *)acRandomPinCode,"0") == 0)
        {
           if(LTE_OK != om_lte_pin_code_random(acRandomPinCode))
            {
                printf("om_lte_pin_code_random fail ");
                return LTE_FAIL;
            }
            else
            {
            }
        }
    }
   
    /* 修改pin码 */
    iRet = om_lte_pin_code_alter(&stPinState,acPinCode,acRandomPinCode);
    if ( LTE_OK != iRet )
    {
        printf("om_lte_pin_code_alter failed\n");
        return LTE_FAIL;
    }

    om_lte_reset_by_platform();

    printf("lte simlock success\n");

    return LTE_OK;
}

LTE_RET_E  om_lte_pin_simlock_off(void)
{
    /* 1、获取当前pin状态，如果为ready，则设置为sim pin
       2、获取simlock code和原pin code
       3、修改pin码
       4、关闭pin锁
    */
    INT32 iRet;
    static CHAR acPinCode[4];
    static CHAR acRandomPinCode[4];
    static UINT32 uiTmpPinState = 0;
    LUA_LTE_PIN_STATE_T stPinState;
    char acTemp[CONFIG_VALUE_MAX_LEN];

    memset(acTemp,0,sizeof(acTemp));
    memset(acPinCode,0,sizeof(acPinCode));
    memset(&stPinState,0,sizeof(stPinState));
    memset(acRandomPinCode,0,sizeof(acRandomPinCode));

    /* 获取当前的pin锁开关状态 */
    iRet = om_lte_pin_state_get(&stPinState);
    if ( LTE_OK != iRet )
    {
        printf("get lte pin state failed\n");
        return iRet;
    }

    if ( PIN_SWITCH_OFF == stPinState.stPinSwitch.enState)
    {
        uiTmpPinState = LTE_SWITCH_OFF;
    }
    else
    {
        uiTmpPinState = LTE_SWITCH_ON;
    }

    if(UCI_FAIL == uci_config_get(UCI_LTE_GET_PIN_CODE,(char *)acPinCode))
    {
        printf("uci_config_get UCI_LTE_GET_PIN_CODE fail\n");
        return LTE_FAIL;
    }

    if(UCI_FAIL == uci_config_get(UCI_LTE_GET_PIN_SIMLOCK_CODE,(char *)acRandomPinCode))
    {
        return LTE_FAIL;
    }
    
    printf("pin state:%d,pin code:%s,simlock code:%s",
                                uiTmpPinState,
                                acPinCode,
                                acRandomPinCode);
    if ( LTE_SWITCH_OFF == uiTmpPinState)
    {
       iRet = om_lte_pin_on(&stPinState,acRandomPinCode);
       if ( LTE_OK != iRet )
       {
           printf("set lte pin on failed\n");
           return LTE_FAIL;
       }
       else
       {
            stPinState.stPinSwitch.enState= PIN_SWITCH_ON;
       }
    }

    /* 修改pin码 */
    iRet = om_lte_pin_code_alter(&stPinState,acRandomPinCode,acPinCode);
    if ( LTE_OK != iRet )
    {
        printf("om_lte_pin_code_alter failed\n");
        return LTE_FAIL;
    }

   stPinState.stPinState.enState = PIN_STATE_READY;
   iRet = om_lte_pin_off(&stPinState,acPinCode);
   if ( LTE_OK != iRet )
   {
       printf("set lte pin off failed\n");
       return LTE_FAIL;
   }
   else//simoff 之后，默认将pin关闭
   {
        memset(acTemp,0,sizeof(acTemp));
        sprintf(acTemp, "%d",PIN_FUNCTION_OFF );

        if(UCI_FAIL == uci_config_set(UCI_LTE_SET_PIN_FUNCTION_SELECT,(char *)acTemp))
        {
            printf("uci_config_set UCI_LTE_SET_PIN_FUNCTION_SELECT fail\n");
            return LTE_FAIL;
        }
        else
        {
            uci_config_commit();
        }

   }

    printf("lte simlock off success\n");
    
    return LTE_OK;
}
