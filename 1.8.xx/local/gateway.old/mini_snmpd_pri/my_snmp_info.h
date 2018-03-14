/******************************************************************************
* COPYRIGHT GBCOM
*******************************************************************************
* 功    能: 
* 修改历史: 
* 2012-9-10     冯静              新建
******************************************************************************/

/******************************* 宏定义 ********************************/
#ifndef __MINI_SNMPD_INFO__
#define __MINI_SNMPD_INFO__

/* BEGIN: Added by zoucaihong, 2012/12/4 
add system group and interfaces group of RFC1213*/
//BOOL agent_get_system(VOID);
//BOOL agent_get_interfaces(VOID);
/* END: Added by zoucaihong, 2012/12/4 */
BOOL ac_agent_get_apbasicsetup_config(VOID);
BOOL ac_agent_get_apswinfo(VOID);
BOOL ac_agent_get_aphwinfo(VOID);
BOOL ac_agent_get_apipsetup_config(VOID);
//BOOL ac_agent_get_aplegalap_config(VOID);
//BOOL ac_agent_get_apflowctl_config(VOID);

/* BEGIN: Added by zoucaihong, 2013/3/11 */
//BOOL ac_agent_get_ap5gpriorsetup_config(VOID);
//BOOL ac_agent_get_ap11npriorsetup_config(VOID);
/* END: Added by zoucaihong, 2013/3/11 */
BOOL ac_agent_get_apwifidevicebasic_config(VOID);
BOOL ac_agent_get_apwifidevicechannel_config(VOID);
BOOL ac_agent_get_apwifidevicepower_config(VOID);
/* BEGIN: Added by zoucaihong, 2013/1/22 */
BOOL ac_agent_get_apwifideviceadvance_config(VOID);
/* END: Added by zoucaihong, 2013/1/22 */
/* BEGIN: Added by Zhou Shouya, 2013/8/30 */
BOOL ac_agent_get_apwifidevicerate_config(VOID);
/* END:   Added by Zhou Shouya, 2013/8/30 */
/* BEGIN: Added by zoucaihong, 2013/3/25 */
BOOL ac_agent_get_apwifidevicebasic5_config(VOID);
BOOL ac_agent_get_apwifidevicechannel5_config(VOID);
BOOL ac_agent_get_apwifidevicepower5_config(VOID);
BOOL ac_agent_get_apwifideviceadvance5_config(VOID);
/* END: Added by zoucaihong, 2013/3/25 */
/* BEGIN: Added by Zhou Shouya, 2013/8/30 */
BOOL ac_agent_get_apwifidevicerate5_config(VOID);
/* END:   Added by Zhou Shouya, 2013/8/30 */
BOOL ac_agent_get_apvapbasicsetup_config(VOID);
BOOL ac_agent_get_apvapsecuritysetup_config(VOID);
BOOL ac_agent_get_apalarmsetup_config(VOID);
BOOL ac_agent_get_aptrapdessetup_config(VOID);
BOOL ac_agent_get_apvlansetup_config(VOID);
BOOL ac_agent_get_apssid_setup(VOID);
BOOL ac_agent_get_apsysstat(VOID);
BOOL ac_agent_get_apwifistat(VOID);
BOOL ac_agent_get_apstationstat(VOID);
/* BEGIN: Added by zoucaihong, 2015/3/25 */
BOOL ac_agent_get_onlineuser(VOID);
BOOL ac_agent_get_apinfo(VOID);
BOOL ac_agent_get_apversioninfo(VOID);
BOOL ac_agent_get_apstainfo(VOID);
/* END: Added by zoucaihong, 2015/3/25 */
#if 0
/* BEGIN: Added by zoucaihong, 2013/1/22 */
BOOL ac_agent_get_ap24cochainterfinfo(VOID);
BOOL ac_agent_get_ap5cochainterfinfo(VOID);
BOOL ac_agent_get_ap24adjchainterfinfo(VOID);
BOOL ac_agent_get_ap5adjchainterfinfo(VOID);
BOOL ac_agent_get_ap24wlandevinterfinfo(VOID);
BOOL ac_agent_get_ap5wlandevinterfinfo(VOID);
BOOL ac_agent_get_apnonwlandevinterfinfo(VOID);
BOOL ac_agent_get_apbrmodestainfo(VOID);
/* END: Added by zoucaihong, 2013/1/22 */
/* BEGIN: Added by zoucaihong, 2013/5/10 */
BOOL ac_agent_get_ap24rogueapinfo(VOID);
BOOL ac_agent_get_ap5rogueapinfo(VOID);
/* END: Added by zoucaihong, 2013/5/10 */
#endif

#endif
