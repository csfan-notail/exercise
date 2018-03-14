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

extern SNMP_TABLE_INFO_T *g_pstTableInfo;

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

    /* Sys Info */
    bRet &= init_sys_dev_info_table(pstTable++);
	if(FALSE == bRet)	
	{		
	    return FALSE;	
	}

	bRet &= init_sys_sw_info_table(pstTable++);
	if(FALSE == bRet)	
	{		
	    return FALSE;	
	}

	bRet &= init_sys_hw_info_table(pstTable++);
	if(FALSE == bRet)	
	{		
	    return FALSE;	
	}

	bRet &= init_sys_manage_info_table(pstTable++);
	if(FALSE == bRet)	
	{		
	    return FALSE;	
	}

    bRet &= init_sys_proto_info_table(pstTable++);
	if(FALSE == bRet)	
	{		
	    return FALSE;	
	}

    bRet &= init_device_heart_info_table(pstTable++);
	if(FALSE == bRet)	
	{		
	    return FALSE;	
	}

    bRet &= init_sys_debug_info_table(pstTable++);
	if(FALSE == bRet)	
	{		
	    return FALSE;	
	}

    /* Lte Info */
	bRet &= init_lte_hw_table(pstTable++);
	if(FALSE == bRet)	
	{		
	    return FALSE;	
	}

	bRet &= init_lte_net_table(pstTable++);
	if(FALSE == bRet)	
	{		
	    return FALSE;	
	}

	bRet &= init_lte_cfg_table(pstTable++);
	if(FALSE == bRet)	
	{		
	    return FALSE;	
	}
	
    /* Wifi Info */
	bRet &= init_wifi_info_table(pstTable++);
	if(FALSE == bRet)	
	{		
	    return FALSE;	
	}

	/* Statistics Info */
	bRet &= init_wan_state_info_table(pstTable++);
	if(FALSE == bRet)	
	{		
	    return FALSE;	
	}

	bRet &= init_wan_user_num(pstTable++);
	if(FALSE == bRet)	
	{		
	    return FALSE;	
	}

	bRet &= init_wan_user_info_table(pstTable++, TABLE_MAXRECNUM_LTE_USER);
	if(FALSE == bRet)	
	{		
	    return FALSE;	
	}
	bRet &= init_pppoe_user_num(pstTable++);
	if(FALSE == bRet)	
	{		
	    return FALSE;	
	}

	bRet &= init_pppoe_user_info_table(pstTable++, TABLE_MAXRECNUM_LTE_USER);
	if(FALSE == bRet)	
	{		
	    return FALSE;	
	}

	bRet &= init_pppoe_all_user_names(pstTable++);
	if(FALSE == bRet)	
	{		
	    return FALSE;	
	}

	bRet &= init_lte_enodeb_info(pstTable++);
	if(FALSE == bRet)	
	{		
	    return FALSE;	
	}

    bRet &= init_log_cfg_table(pstTable++);
	if(FALSE == bRet)	
	{		
	    return FALSE;	
	}

	bRet &= init_keepalive_cfg_table(pstTable++);
	if(FALSE == bRet)	
	{		
	    return FALSE;	
	}

    bRet &= init_keepalive_info_table(pstTable++, TABLE_MAXRECNUM_LTE_KEEPALIVE);
	if(FALSE == bRet)	
	{		
	    return FALSE;	
	}
    
    /* Device Oper */
	bRet &= init_device_oper_table(pstTable++);
	if(FALSE == bRet)	
	{		
	    return FALSE;	
	}

	bRet &= init_file_trans_table(pstTable++);
	if(FALSE == bRet)	
	{		
	    return FALSE;	
	}

	bRet &= init_lte_station_lock(pstTable++);
	if(FALSE == bRet)	
	{		
	    return FALSE;	
	}

    bRet &= init_lte_station_lock_table(pstTable++, TABLE_MAXRECNUM_LTE_LOCK_TABLE);
	if(FALSE == bRet)	
	{		
	    return FALSE;	
	}

	bRet &= init_lte_station_all_lock_id(pstTable++);
	if(FALSE == bRet)	
	{		
	    return FALSE;	
	}

	/*check table num*/
	if (g_pstTableInfo->ulTableNum > ARRAY_SIZE(g_pstTableInfo->astTables))
	{
		agent_printf(LOG_ERR, "snmp_init_tables_info() tableNum is too large.\n");
		return FALSE;
	}
	
	agent_printf(LOG_INFO, "snmp_init_tables_info succeed\n");
	return TRUE;
}

