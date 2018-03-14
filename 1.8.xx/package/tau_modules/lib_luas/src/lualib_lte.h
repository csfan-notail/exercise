#ifndef lualib_lte_h
#define lualib_lte_h
#include "lte.h"


#define UCI_LTE_GET_FREQUENCY_PASSWD            "lte.frequency.frequency_passwd"
#define UCI_LTE_GET_FREQUENCY_LOCK_COMMUNITY    "lte.frequency.lock_community"
#define UCI_LTE_GET_FREQUENCY_LOCK_SW           "lte.frequency.lock_sw"
#define UCI_LTE_GET_FREQUENCY_FREQUENCY         "lte.frequency.frequency"

/*********************** PIN **************************/
#define UCI_LTE_GET_SIM_LOCK_SW                 "lte.sim.lock_sw"
#define UCI_LTE_SET_PIN_LOCK_SW                 "lte.sim.pin_sw"
#define UCI_LTE_GET_PIN_SIMLOCK_CODE            "lte.sim.pin_simlock_code"
#define UCI_LTE_SET_PIN_SIMLOCK_CODE_OLD        "lte.sim.pin_simlock_code_old"
#define UCI_LTE_GET_PIN_CODE                    "lte.sim.pin_code"
#define UCI_LTE_SET_PIN_OLD_CODE                "lte.sim.pin_old_code"
#define UCI_LTE_SET_PIN_NEW_CODE                "lte.sim.pin_new_code"
#define UCI_LTE_SET_PIN_FUNCTION_SELECT         "lte.sim.pin_function_select"
#define UCI_LTE_SET_PIN_UNLOCK_SW               "lte.sim.pin_unlock_sw"
#define UCI_LTE_SET_PIN_STATE                   "lte.sim.pin_state"
/*********************** Basic Set **************************/
#define UCI_LTE_BASIC_CONNECT_VALUE             "lte.set.basic_connecttime"


#define LTE_ACCESS_CELL_LOCK_FREQ_DATA "90769296"
#define CONFIG_VALUE_MAX_LEN 256

#define LTE_OK 1
#define LTE_FAIL -1

#ifndef UCI_OK 0
#define UCI_OK 0
#endif

#ifndef UCI_FAIL -1
#define UCI_FAIL -1
#endif

#ifndef LTE_PIN_LTE
#define LTE_PIN_LTE         32
#endif

#ifndef LTE_SWITCH_OFF
#define LTE_SWITCH_OFF      0
#endif

#ifndef LTE_SWITCH_ON
#define LTE_SWITCH_ON       1
#endif

typedef enum 
{
    PIN_SIMLOCK_OFF = 0, //sim锁定禁用
    PIN_SIMLOCK_ON,       //sim锁定启用
    PIN_SIMLOCK_SIM_EXCHANGE,  //更换sim卡，清除之前的simlock配置
}LUA_LTE_PIN_SIMLOCK_E;

typedef enum 
{
    PIN_FUNCTION_OFF=0,
    PIN_FUNCTION_ON=1,
    PIN_FUNCTION_PIN_CHANGE=2,
    PIN_FUNCTION_PUK_UNLOCK=3
}LUA_LTE_PIN_FUNCTION_SELECT_E;

typedef enum 
{
    PIN_UNLOCK_AUTO     = 0,
    PIN_UNLOCK_MANUL    = 1
}LUA_LTE_PIN_UNLOCK_MODE_E;

/* 当前pin状态 */
typedef enum 
{
    LUA_LTE_PIN_OFF_AND_READY= 1,
    LUA_LTE_PIN_ON_AND_READY,
    LUA_LTE_PIN_ON_AND_SIMPIN,
    LUA_LTE_PIN_ON_AND_SIMPUK,
    LUA_LTE_PIN_FAILED 
}LUA_LTE_PIN_STATE_E;

/* 当配置逻辑检查错误时，页面显示对应错误 */
/* 修改该结构体时，请往后增加值 */
typedef enum 
{
    LUA_LTE_WEB_ALERT_NONE = 0,
    LUA_LTE_WEB_ALERT_WAN_ACCESS_SW = 1,  //提示wan接入方式选择lte
    LUA_LTE_WEB_ALERT_LTE_MODULE_CONTROL_FAILED,//LTE模块操作失败
    LUA_LTE_WEB_ALERT_PIN_INPUT_REMAIN_ZERO, //剩余可输入pin码此时为0
    LUA_LTE_WEB_ALERT_LTE_BUSY,       //lte模块执行指令中
    LUA_LTE_WEB_ALERT_PIN_STATE,          //当前处于pin状态
    LUA_LTE_WEB_ALERT_NOT_PIN_STATE,
    LUA_LTE_WEB_ALERT_PIN_CODE_ERR,      //输入的pin码错误
    LUA_LTE_WEB_ALERT_MSG_DEL_FAIL,      //短信删除失败
}LUA_LTE_WEB_ALERT_E;

typedef struct 
{
    CHAR   acPinCode[LTE_PIN_LTE];
    CHAR   acUnPukCode[LTE_PIN_LTE];
    CHAR   acUnPinCode[LTE_PIN_LTE];
    CHAR   acOldPinCode[LTE_PIN_LTE];
    CHAR   acNewPinCode[LTE_PIN_LTE];
    UINT32 uiOperateFlag;        /* 操作标志位：0：pin操作  1：sim卡锁操作   */
    UINT32 uiPinInputRemain;      /* pin码剩余输入此时 */
    LUA_LTE_PIN_STATE_E           ePinState;
    LUA_LTE_WEB_ALERT_E           eAlertFlag;
	LUA_LTE_PIN_SIMLOCK_E         eSimlockSw;
    LUA_LTE_PIN_UNLOCK_MODE_E     eUnlockMode;
    LUA_LTE_PIN_FUNCTION_SELECT_E eLtePinFuncSelect;   
}LUA_LTE_PIN_CFG_T;

typedef struct 
{
    LTE_PIN_SW_STATE_T stPinSwitch;       //pin switch
    LTE_PIN_STATE_T stPinState;       //pin state now
}LUA_LTE_PIN_STATE_T;



int om_lte_get_device_sn( CHAR *szSn );
int om_lte_pin_code_random( CHAR *acPinCode );
int om_lte_update_pin( LUA_LTE_PIN_CFG_T *pstLtePinCfg);

LTE_RET_E  om_lte_pin_simlock_on( void );
LTE_RET_E  om_lte_pin_simlock_off( void );
LTE_RET_E  om_lte_reset_by_platform( void );
LTE_RET_E  om_lte_pin_state_get( LUA_LTE_PIN_STATE_T *pstPinState);
LTE_RET_E  om_lte_pin_on( LUA_LTE_PIN_STATE_T *pstPinState,CHAR *acPinCode );
LTE_RET_E  om_lte_pin_code_alter( LUA_LTE_PIN_STATE_T *pstPinState,CHAR *acPinOld, CHAR *acPinNew );


#endif