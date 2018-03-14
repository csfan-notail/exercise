/******************************************************************************
* COPYRIGHT GBCOM
*******************************************************************************
* 功    能: 
* 修改历史: 
* 2012-9-10     冯静              新建
******************************************************************************/

#include <syslog.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "mini_snmpd.h"

#include "my_snmp_def.h"
#include "my_snmp_data.h"
#include "my_snmp_handle.h"
#include "my_snmp_util.h"
#include "my_snmp_config.h"
#include "my_snmp_init.h"

/*****************************全局变量定义*******************************************/
extern SNMP_TABLE_INFO_T *g_pstTableInfo;
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL snmp_init_tables_info(VOID)
{
	SNMP_TABLE_T    *pstTable;
	BOOL            bRet = TRUE;

	agent_printf(LOG_INFO, "snmp_init_tables_info begin\n");

	g_pstTableInfo = SNMP_AGENT_MALLOC(sizeof(SNMP_TABLE_INFO_T));
	if (NULL == g_pstTableInfo)
	{
		return FALSE;
	}
	memset(g_pstTableInfo, 0, sizeof(SNMP_TABLE_INFO_T));

	g_pstTableInfo->ulTableNum = 0;
	pstTable = &g_pstTableInfo->astTables[0];
#if 0
    /* BEGIN: Added by zoucaihong, 2012/12/4 
    add system group and interfaces group of RFC1213*/
	bRet &= init_snmp_system(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_interfaces(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_interfaces_table(pstTable++, TABLE_MAXRECNUM_INTERFACES);
	if(FALSE == bRet)	{		return FALSE;	}
    /* END: Added by zoucaihong, 2012/12/4 */
#endif
	/*config*/
	bRet &= init_snmp_ap_config_sys_devinfo(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_config_swinfo(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_config_hwinfo(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_config_ip(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_config_ip_table(pstTable++, TABLE_MAXRECNUM_IP);
	if(FALSE == bRet)	{		return FALSE;	}
    #if 0
	bRet &= init_snmp_ap_config_legalap(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_config_legalap_table(pstTable++, TABLE_MAXRECNUM_LEGALAP);
	if(FALSE == bRet)	{		return FALSE;	}
//#if 0
	bRet &= init_snmp_ap_config_flowctl(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
//#endif
	bRet &= init_snmp_ap_config_advance_5gprior(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_config_advance_11nprior(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
    #endif
#ifndef GW_X86
	bRet &= init_snmp_ap_config_wifidevicebasic(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_config_wifidevicechannel(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_config_wifidevicepower(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
    /* BEGIN: Added by zoucaihong, 2013/1/22 */
	bRet &= init_snmp_ap_config_wifideviceadvance(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
    /* END: Added by zoucaihong, 2013/1/22 */
    /* BEGIN: Added by Zhou Shouya, 2013/8/29 */
	bRet &= init_snmp_ap_config_wifidevicerate(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
    /* END:   Added by Zhou Shouya, 2013/8/29 */
    /* BEGIN: Added by zoucaihong, 2013/3/21 */
    /* 2.4G卡配置和5G卡配置分开 */
	bRet &= init_snmp_ap_config_wifidevicebasic5(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_config_wifidevicechannel5(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_config_wifidevicepower5(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_config_wifideviceadvance5(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
    /* END: Added by zoucaihong, 2013/3/21 */
    /* BEGIN: Added by Zhou Shouya, 2013/8/29 */
	bRet &= init_snmp_ap_config_wifidevicerate5(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
    /* END:   Added by Zhou Shouya, 2013/8/29 */
	bRet &= init_snmp_ap_config_vap(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_config_vapbasictable(pstTable++, TABLE_MAXRECNUM_AP_VAP);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_config_vapsecuritytable(pstTable++, TABLE_MAXRECNUM_AP_VAP);
	if(FALSE == bRet)	{		return FALSE;	}
#endif
	bRet &= init_snmp_ap_config_alarm(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_config_trapdes(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	#if 0
	bRet &= init_snmp_ap_config_trapdes_table(pstTable++, TABLE_MAXRECNUM_TRAP_DES);
	if(FALSE == bRet)	{		return FALSE;	}
	#endif
#ifndef GW_X86
    bRet &= init_snmp_ap_config_vlan(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_config_vlan_table(pstTable++, TABLE_MAXRECNUM_VLAN);
	if(FALSE == bRet)	{		return FALSE;	}
    bRet &= init_snmp_ap_config_ssid(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_config_ssid_table(pstTable++, TABLE_MAXRECNUM_AP_VAP);
	if(FALSE == bRet)	{		return FALSE;	}
    
	/*stat*/
	bRet &= init_snmp_ap_stat_sys(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_stat_wifi(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_stat_wifi_table(pstTable++, TABLE_MAXRECNUM_AP_WIFI);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_stat_wifivap(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_stat_wifivap_table(pstTable++, TABLE_MAXRECNUM_AP_VAP);
	if(FALSE == bRet)	{		return FALSE;	}
#endif
	bRet &= init_snmp_ap_stat_station(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_stat_station_table(pstTable++, TABLE_MAXRECNUM_STATION);
	if(FALSE == bRet)	{		return FALSE;	}
    /* BEGIN: Added by zoucaihong, 2013/1/22 */
#if 0    
	bRet &= init_snmp_ap_stat_24cochainterfinfo(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_stat_24cochainterfinfo_table(pstTable++, TABLE_MAXRECNUM_INTERF);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_stat_5cochainterfinfo(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_stat_5cochainterfinfo_table(pstTable++, TABLE_MAXRECNUM_INTERF);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_stat_24adjchainterfinfo(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_stat_24adjchainterfinfo_table(pstTable++, TABLE_MAXRECNUM_INTERF);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_stat_5adjchainterfinfo(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_stat_5adjchainterfinfo_table(pstTable++, TABLE_MAXRECNUM_INTERF);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_stat_24wlandevinterfinfo(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_stat_24wlandevinterfinfo_table(pstTable++, TABLE_MAXRECNUM_WLANINTERF);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_stat_5wlandevinterfinfo(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_stat_5wlandevinterfinfo_table(pstTable++, TABLE_MAXRECNUM_WLANINTERF);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_stat_24rogueapinfo(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_stat_24rogueapinfo_table(pstTable++, TABLE_MAXRECNUM_ROGUEAP);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_stat_5rogueapinfo(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_stat_5rogueapinfo_table(pstTable++, TABLE_MAXRECNUM_ROGUEAP);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_stat_nonwlandevinterfinfo(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_stat_nonwlandevinterfinto_table(pstTable++, TABLE_MAXRECNUM_AP_WIFI);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_stat_brmodestation(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_stat_brmodestation_table(pstTable++, TABLE_MAXRECNUM_STATION);
	if(FALSE == bRet)	{		return FALSE;	}
    /* END: Added by zoucaihong, 2013/1/22 */
#endif
    /* BEGIN: Added by zoucaihong, 2014/5/15 */
	bRet &= init_snmp_ap_oper_sysoper(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
	bRet &= init_snmp_ap_oper_filetransconfig(pstTable++);
	if(FALSE == bRet)	{		return FALSE;	}
    /* END: Added by zoucaihong, 2014/5/15 */

	/*check table num*/
	if (g_pstTableInfo->ulTableNum > ARRAY_SIZE(g_pstTableInfo->astTables))
	{
		agent_printf(LOG_ERR, "snmp_init_tables_info() tableNum is too large.\n");
		return FALSE;
	}
	
	agent_printf(LOG_INFO, "snmp_init_tables_info succeed\n");
	return TRUE;
}

