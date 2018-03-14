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

#include "snmp_nms.h"

#include "my_snmp_def.h"
#include "my_snmp_data.h"
#include "my_snmp_oid.h"
#include "my_snmp_handle.h"
#include "my_snmp_util.h"
#include "my_snmp_config.h"
#include "my_snmp_init.h"
#if 0
/* BEGIN: Added by zoucaihong, 2012/12/4 
add system group and interfaces group of RFC1213*/
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL init_snmp_system( SNMP_TABLE_T *pstTable )
{
    SYSTEM_T *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
	SNMP_INIT_SCALAR(oid_system,
		TABLE_ID_SYSTEM,
		TABLE_NAME_SYSTEM,
		snmp_check_valid_systeminfo);
	
	SNMP_INIT_SCALAR_COLUMN(type->acSysDescr,  "SysDescr", TYPE_CHAR, MIB_R);
	//SNMP_INIT_SCALAR_COLUMN(type->aulSysObjectID,  "SysObjectID", TYPE_OID, MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->acSysObjectID,  "SysObjectID", TYPE_OID, MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ulSysUpTime,  "SysUpTime", TYPE_TIME, MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->acSysContact,  "SysContact", TYPE_CHAR, MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->acSysName,  "SysName", TYPE_CHAR, MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->acSysLocation,  "SysLocation", TYPE_CHAR, MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->ulSysServices,  "SysServices", TYPE_LONG, MIB_R);
	
    return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL init_snmp_interfaces( SNMP_TABLE_T *pstTable )
{
    INTERFACES_T *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
	SNMP_INIT_SCALAR(oid_interfaces,
		TABLE_ID_INTERFACES,
		TABLE_NAME_INTERFACES,
		NULL);
	
	SNMP_INIT_SCALAR_COLUMN(type->ulIfNumber,  "IfNumber", TYPE_LONG, MIB_R);
	
    return TRUE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL init_snmp_interfaces_table( SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum)
{
    INTERFACES_ENTRY_T *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_TABLE(oid_interfaces_table,
                    TABLE_ID_INTERFACESTABLE,
                    TABLE_NAME_INTERFACESTABLE,
                    ulMaxTableRecNum,
                    NULL);

	SNMP_INIT_TABLE_COLUMN(type->ulIfIndex, "ulIfIndex", TYPE_LONG, MIB_R, TRUE);
	SNMP_INIT_TABLE_COLUMN(type->acIfDescr, "acIfDescr", TYPE_CHAR, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulIfType, "IfType", TYPE_LONG, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulIfMtu, "IfMtu", TYPE_LONG, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulIfSpeed, "IfSpeed",  TYPE_LONG, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->aucIfPhysAddress, "IfPhysAddress", TYPE_MAC, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulIfAdminStatus, "IfAdminStatus", TYPE_LONG, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulIfOperStatus, "IfOperStatus", TYPE_LONG, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulIfLastChange, "IfLastChange", TYPE_TIME, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulIfInOctets, "IfInOctets", TYPE_COUNTER32, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulIfInUcastPkts, "IfInUcastPkts", TYPE_COUNTER32, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulIfInNUcastPkts,	"ulIfInNUcastPkts", TYPE_COUNTER32, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulIfInDiscards, "IfInDiscards", TYPE_COUNTER32, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulIfInErrors, "IfInErrors", TYPE_COUNTER32, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulIfInUnknownProtos, "IfInUnknownProtos", TYPE_COUNTER32, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulIfOutOctets, "IfOutOctets", TYPE_COUNTER32, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulIfOutUcastPkts, "IfOutUcastPkts", TYPE_COUNTER32, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulIfOutNUcastPkts, "IfOutNUcastPkts", TYPE_COUNTER32, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulIfOutDiscards, "IfOutDiscards", TYPE_COUNTER32, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulIfOutErrors, "IfOutErrors", TYPE_COUNTER32, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulIfOutQLen, "IfOutQLen", TYPE_LONG, MIB_R, FALSE);
	//SNMP_INIT_TABLE_COLUMN(type->aulIfSpecific, "IfSpecific", TYPE_OID, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->acIfSpecific, "IfSpecific", TYPE_CHAR, MIB_R, FALSE);
	
    return TRUE;
}
/* END: Added by zoucaihong, 2012/12/4 */
#endif
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL init_snmp_ap_config_sys_devinfo(SNMP_TABLE_T *pstTable)
{
    AP_CONFIG_SYS_DEVINFO_T     *type = NULL;
    
    if (NULL == pstTable)
    {
        return FALSE;
    }

    SNMP_INIT_SCALAR(oid_ap_config_sys_devinfo,
                     TABLE_ID_AP_CONFIG_SYS_DEVINFO,
                     TABLE_NAME_AP_CONFIG_SYS_DEVINFO,
                     snmp_check_valid_ap_config_sys_devinfo);

    SNMP_INIT_SCALAR_COLUMN(type->aucSysMacAddr,        "",          TYPE_MAC,   MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->ulSysUpTime,          "",           TYPE_TIME,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->ulRegetPeriod,        "",         TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulIfSpeed,            "",             TYPE_LONG,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->ulTrapSwitch,         "",          TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulSysRestart,         "",          TYPE_LONG,  MIB_RW);
    /* BEGIN: Added by zoucaihong, 2014/5/13 */
    SNMP_INIT_SCALAR_COLUMN(type->ulTimeZone,           "",            TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->acTime,               "",                TYPE_CHAR,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->aucIpAddress,         "",           TYPE_IP,    MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->ulSysManageState,     "",      TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulSysModel,           "",            TYPE_LONG,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->acInterModel,         "",          TYPE_CHAR,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->acSysModelName,       "",        TYPE_CHAR,  MIB_R);
    /* END: Added by zoucaihong, 2014/5/13 */
    SNMP_INIT_SCALAR_COLUMN(type->ulTimeOutInterval,    "",  TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulHeartBeatPeriod,    "",  TYPE_LONG,  MIB_RW);
	/* BEGIN: Added by zoucaihong, 2014/9/23 */
    SNMP_INIT_SCALAR_COLUMN(type->ulDevType,    "", TYPE_LONG,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->acSysName,    "", TYPE_CHAR,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->acHotId,      "", TYPE_CHAR,  MIB_R);
	/* END: Added by zoucaihong, 2014/9/23 */
    /* BEGIN: Added by zoucaihong, 2015/3/24 */
    SNMP_INIT_SCALAR_COLUMN(type->ulRunState,   "", TYPE_LONG,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->acAuthCode,   "", TYPE_CHAR,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulAuthState,  "", TYPE_LONG,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->acPartner,    "", TYPE_CHAR,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->acLineName,   "", TYPE_CHAR,  MIB_R);
    /* END: Added by zoucaihong, 2015/3/24 */
    SNMP_INIT_SCALAR_COLUMN(type->acManufacture,"", TYPE_CHAR,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->acSn,         "", TYPE_CHAR,  MIB_R);
    
    return TRUE;
}

/* BEGIN: Added by zoucaihong, 2012/11/5 */
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL init_snmp_ap_config_swinfo(SNMP_TABLE_T *pstTable)
{
    AP_CONFIG_SYS_SWINFO_T     *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }

    SNMP_INIT_SCALAR(oid_ap_config_sys_swinfo,
                     TABLE_ID_AP_CONFIG_SYS_SWINFO,
                     TABLE_NAME_AP_CONFIG_SYS_SWINFO,
                     NULL);

	SNMP_INIT_SCALAR_COLUMN(type->acSoftwareVersion,    "",      TYPE_CHAR,  MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->acTargetVersion,      "",        TYPE_CHAR,  MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->acOemVersion,         "",           TYPE_CHAR,  MIB_R);
		
    return TRUE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL init_snmp_ap_config_hwinfo(SNMP_TABLE_T *pstTable)
{
    AP_CONFIG_SYS_HWINFO_T     *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }

    SNMP_INIT_SCALAR(oid_ap_config_sys_hwinfo,
                     TABLE_ID_AP_CONFIG_SYS_HWINFO,
                     TABLE_NAME_AP_CONFIG_SYS_HWINFO,
                     NULL);

    SNMP_INIT_SCALAR_COLUMN(type->acCpuType,            "",              TYPE_CHAR,  MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->acMemType,            "",              TYPE_CHAR,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->ulMemSize,            "",              TYPE_LONG,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->ulDiskSize,           "",             TYPE_LONG,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->acHardwareVersion,    "",      TYPE_CHAR,  MIB_R);
    /* BEGIN: Added by zoucaihong, 2014/5/15 */
    SNMP_INIT_SCALAR_COLUMN(type->ulHardwareType,       "",         TYPE_LONG,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->ulBoardVersion,       "",         TYPE_LONG,  MIB_R);
    /* END: Added by zoucaihong, 2014/5/15 */
    SNMP_INIT_SCALAR_COLUMN(type->acCpuInfo,    "",      TYPE_CHAR,  MIB_R);

    return TRUE;
}

/*****************************************************************************
 Prototype    : init_snmp_ap_config_ip
 Description  : AP IP配置表IP数目节点注册函数
 Input        : SNMP_TABLE_T *pstTable  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/2/25
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_config_ip(SNMP_TABLE_T *pstTable)
{
	AP_CONFIG_IP_NUM_T  *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }

    SNMP_INIT_SCALAR(oid_ap_config_sys_ip,
                     TABLE_ID_AP_CONFIG_SYS_IP,
                     TABLE_NAME_AP_CONFIG_SYS_IP,
                      NULL);

    SNMP_INIT_SCALAR_COLUMN(type->ulNum,  "",  TYPE_LONG,   MIB_R);

    return TRUE;
}

/*****************************************************************************
 Prototype    : init_snmp_ap_config_ip_table
 Description  : AP IP配置表注册函数
 Input        : SNMP_TABLE_T *pstTable   
                UINT32 ulMaxTableRecNum  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/2/25
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_config_ip_table(SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum)
{
    AP_CONFIG_IP_ENTRY_T    *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_TABLE(oid_ap_config_sys_ip_table,
                    TABLE_ID_AP_CONFIG_SYS_IPTABLE,
                    TABLE_NAME_AP_CONFIG_SYS_IPTABLE,
                    ulMaxTableRecNum,
                    snmp_check_valid_ap_config_ip);

    SNMP_INIT_TABLE_COLUMN(type->ulIndex,       "",        TYPE_LONG,      MIB_RC,   TRUE);
	SNMP_INIT_COLUMN_RANGE(1, 2);
    SNMP_INIT_TABLE_COLUMN(type->ulIpMode,      "",       TYPE_LONG,      MIB_RW,  FALSE);
    SNMP_INIT_TABLE_COLUMN(type->aucIpAddress,  "",    TYPE_IP,        MIB_RW,  FALSE);
    SNMP_INIT_TABLE_COLUMN(type->aucIpNetMask,  "",    TYPE_IP,        MIB_RW,  FALSE);
    SNMP_INIT_TABLE_COLUMN(type->aucGwAddress,  "",    TYPE_IP,        MIB_RW,  FALSE);
    SNMP_INIT_TABLE_COLUMN(type->aucDnsMaster,  "",    TYPE_IP,        MIB_RW,  FALSE);
    SNMP_INIT_TABLE_COLUMN(type->aucDnsSlave,   "",     TYPE_IP,        MIB_RW,  FALSE);

    return TRUE;
}

#if 0
/* BEGIN: Added by zoucaihong, 2013/5/13 */
/*****************************************************************************
 Prototype    : init_snmp_ap_config_legalap
 Description  : 合法AP数目节点注册函数
 Input        : SNMP_TABLE_T *pstTable  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/5/13
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_config_legalap(SNMP_TABLE_T *pstTable)
{
	AP_CONFIG_LEGALAP_NUM_T  *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }

    SNMP_INIT_SCALAR(oid_ap_config_sys_legalap,
                     TABLE_ID_AP_CONFIG_SYS_LEGALAP,
                     TABLE_NAME_AP_CONFIG_SYS_LEGALAP,
                      NULL);

    SNMP_INIT_SCALAR_COLUMN(type->ulNum,  "",  TYPE_LONG,   MIB_R);

    return TRUE;
}

/*****************************************************************************
 Prototype    : init_snmp_ap_config_legal_table
 Description  : 合法AP配置表节点注册函数
 Input        : SNMP_TABLE_T *pstTable   
                UINT32 ulMaxTableRecNum  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/5/13
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_config_legalap_table(SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum)
{
    AP_CONFIG_LEGALAP_ENTRY_T    *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_TABLE(oid_ap_config_sys_legalap_table,
                    TABLE_ID_AP_CONFIG_SYS_LEGALAPTABLE,
                    TABLE_NAME_AP_CONFIG_SYS_LEGALAP_TABLE,
                    ulMaxTableRecNum,
                    snmp_check_valid_ap_config_legalap);

    SNMP_INIT_TABLE_COLUMN(type->ulIndex,           "",        TYPE_LONG,  MIB_RC, TRUE);
    SNMP_INIT_TABLE_COLUMN(type->acLegalApSsid,     "",  TYPE_CHAR,  MIB_R, FALSE);
    SNMP_INIT_TABLE_COLUMN(type->aucLegalApBssid,   "", TYPE_MAC,   MIB_RW, FALSE);

    return TRUE;
}

/*****************************************************************************
 Prototype    : init_snmp_ap_config_flowctl
 Description  : AP级流控配置表节点注册函数
 Input        : SNMP_TABLE_T *pstTable  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/5/13
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_config_flowctl(SNMP_TABLE_T *pstTable)
{
	AP_CONFIG_FLOWCTL_T  *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }

    SNMP_INIT_SCALAR(oid_ap_config_sys_flowctl,
                     TABLE_ID_AP_CONFIG_SYS_FLOWCTL,
                     TABLE_NAME_AP_CONFIG_SYS_FLOWCTL,
                     snmp_check_valid_ap_config_flowctl);

    SNMP_INIT_SCALAR_COLUMN(type->ulFlowSwitch,         "",       TYPE_LONG,   MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulApUplinkMaxRate,    "",  TYPE_LONG,   MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulApDwlinkMaxRate,    "",  TYPE_LONG,   MIB_RW);
    /* BEGIN: Added by Zhou Shouya, 2013/8/13   PN:VLAN */
    SNMP_INIT_SCALAR_COLUMN(type->ulStaUplinkMaxRate,   "",  TYPE_LONG,   MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulStaDwlinkMaxRate,   "",  TYPE_LONG,   MIB_RW);
    /* END:   Added by Zhou Shouya, 2013/8/13   PN:VLAN */

    return TRUE;
}
/* END: Added by zoucaihong, 2013/5/13 */

/* BEGIN: Added by zoucaihong, 2013/3/11 */
/*****************************************************************************
 Prototype    : init_snmp_ap_config_advance_5gprior
 Description  : AP 5G优先配置表注册函数
 Input        : SNMP_TABLE_T *pstTable  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/3/11
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_config_advance_5gprior(SNMP_TABLE_T *pstTable)
{
    AP_CONFIG_ADVANCE_5GPRIOR_T     *type = NULL;
    
    if (NULL == pstTable)
    {
        return FALSE;
    }

    SNMP_INIT_SCALAR(oid_ap_config_advance_5gprior,
                     TABLE_ID_AP_CONFIG_ADVANCE_5GPRIOR,
                     TABLE_NAME_AP_CONFIG_ADVANCE_5GPRIOR,
                     snmp_check_valid_ap_config_advance_5gprior);

    SNMP_INIT_SCALAR_COLUMN(type->ul5GPriSwitch,        "",      TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ul2dot4GDelayTime,    "",  TYPE_LONG,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->ulStaTimesOut,        "",      TYPE_LONG,  MIB_R);
		
    return TRUE;
}

/*****************************************************************************
 Prototype    : init_snmp_ap_config_advance_11nprior
 Description  : AP 11n优先配置表注册函数
 Input        : SNMP_TABLE_T *pstTable  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/3/11
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_config_advance_11nprior(SNMP_TABLE_T *pstTable)
{
    AP_CONFIG_ADVANCE_11NPRIOR_T    *type = NULL;
    
    if (NULL == pstTable)
    {
        return FALSE;
    }

    SNMP_INIT_SCALAR(oid_ap_config_advance_11nprior,
                     TABLE_ID_AP_CONFIG_ADVANCE_11NPRIOR,
                     TABLE_NAME_AP_CONFIG_ADVANCE_11nPRIOR,
                     snmp_check_valid_ap_config_advance_11nprior);

    SNMP_INIT_SCALAR_COLUMN(type->ul11nPriSwitch,   "",   TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ul11bgDelayTime,  "",  TYPE_LONG,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->ulStaTimesOut,    "",    TYPE_LONG,  MIB_R);
		
    return TRUE;
}
/* END: Added by zoucaihong, 2013/3/11 */
#endif
#ifndef GW_X86


/*****************************************************************************
 Prototype    : init_snmp_ap_config_wifidevicebasic
 Description  : 2.4G卡基本配置信息节点注册函数
 Input        : SNMP_TABLE_T *pstTable   
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/3/21
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_config_wifidevicebasic(SNMP_TABLE_T *pstTable)
{
    AP_CONFIG_WIFIDEVICEBASIC_T     *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_SCALAR(oid_ap_config_wifibasic,
                    TABLE_ID_AP_CONFIG_WIFIBASIC,
                    TABLE_NAME_AP_CONFIG_WIFIBASIC,
                    NULL);

    SNMP_INIT_SCALAR_COLUMN(type->ulWifiId,         "",     TYPE_LONG,  MIB_R);
	SNMP_INIT_COLUMN_RANGE(0, 3);
    SNMP_INIT_SCALAR_COLUMN(type->aucMacAddress,    "",       TYPE_MAC,   MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ulWifiDeviceType, "",   TYPE_LONG,  MIB_R);

    return TRUE;
}

/*****************************************************************************
 Prototype    : init_snmp_ap_config_wifidevicechannel
 Description  : 2.4G卡信道配置信息节点注册函数
 Input        : SNMP_TABLE_T *pstTable   
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/3/21
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_config_wifidevicechannel(SNMP_TABLE_T *pstTable)
{
    AP_CONFIG_WIFIDEVICECHANNEL_T     *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_SCALAR(oid_ap_config_wifichannel,
                    TABLE_ID_AP_CONFIG_WIFICHANNEL,
                    TABLE_NAME_AP_CONFIG_WIFICHANNEL,
                    snmp_check_valid_ap_config_wifidevicechannel);

    SNMP_INIT_SCALAR_COLUMN(type->ulWifiId,         "", TYPE_LONG, MIB_R);
		
    SNMP_INIT_SCALAR_COLUMN(type->ulAbgMode,        "",      TYPE_LONG, MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulChannelUsing,   "", TYPE_LONG, MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ulChannelState,   "", TYPE_LONG, MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->ulAutoChanTime,   "", TYPE_LONG, MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->ulStaicChannel,   "", TYPE_LONG, MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->acAutoChanList,   "", TYPE_CHAR, MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulRFNum,          "",        TYPE_LONG, MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulBwMode,         "",       TYPE_LONG, MIB_RW);

    return TRUE;
}
/*****************************************************************************
 Prototype    : init_snmp_ap_config_wifidevicepower
 Description  : 2.4G卡功率配置信息节点注册函数
 Input        : SNMP_TABLE_T *pstTable   
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/3/21
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_config_wifidevicepower(SNMP_TABLE_T *pstTable)
{
    AP_CONFIG_WIFIDEVICEPOWER_T     *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_SCALAR(oid_ap_config_wifipower,
                    TABLE_ID_AP_CONFIG_WIFIPOWER,
                    TABLE_NAME_AP_CONFIG_WIFIPOWER,
                    snmp_check_valid_ap_config_wifidevicepower);
	
    SNMP_INIT_SCALAR_COLUMN(type->ulWifiId,         "",     TYPE_LONG, MIB_R);
	
    SNMP_INIT_SCALAR_COLUMN(type->ulLimitTxPower,   "",     TYPE_LONG, MIB_R);	
	SNMP_INIT_SCALAR_COLUMN(type->ulMaxTxPower,     "",       TYPE_LONG, MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulTransmitPower,  "",  TYPE_LONG, MIB_R);

	return TRUE;
}
/*****************************************************************************
 Prototype    : init_snmp_ap_config_wifideviceadvance
 Description  : 2.4G卡告警配置信息节点注册函数
 Input        : SNMP_TABLE_T *pstTable   
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/3/21
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_config_wifideviceadvance(SNMP_TABLE_T *pstTable)
{
    AP_CONFIG_WIFIDEVICEADVANCE_T     *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_SCALAR(oid_ap_config_wifiadvanced,
                    TABLE_ID_AP_CONFIG_WIFIADVANCE,
                    TABLE_NAME_AP_CONFIG_WIFIADVANCE,
                    snmp_check_valid_ap_config_wifideviceadvance);

    SNMP_INIT_SCALAR_COLUMN(type->ulWifiId,     "", TYPE_LONG,  MIB_R);
	
	SNMP_INIT_SCALAR_COLUMN(type->ulRssiMode,   "",     TYPE_INT,   MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->iRssi,        "",      TYPE_INT,   MIB_RW);

	return TRUE;
}
/*****************************************************************************
 Prototype    : init_snmp_ap_config_wifidevicerate
 Description  : 2.4G卡级速率配置
 Input        : SNMP_TABLE_T *pstTable  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/8/29
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_config_wifidevicerate(SNMP_TABLE_T *pstTable)
{
    AP_CONFIG_WIFIDEVICERATE_T     *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_SCALAR(oid_ap_config_wifirate,
                    TABLE_ID_AP_CONFIG_WIFIRATE,
                    TABLE_NAME_AP_CONFIG_WIFIRATE,
                    snmp_check_valid_ap_config_wifidevicerate);

    SNMP_INIT_SCALAR_COLUMN(type->ulWifiId,     "", TYPE_INT,  MIB_R);
	
	SNMP_INIT_SCALAR_COLUMN(type->acTransmitSpeedBasic,   "", TYPE_CHAR,   MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->acTransmitSpeed11n,     "",   TYPE_CHAR,   MIB_RW);
	/* BEGIN: Added by Zhou Shouya, 2013/9/17 */
    SNMP_INIT_SCALAR_COLUMN(type->ulRateMode,   "", TYPE_INT,  MIB_R);
	/* END:   Added by Zhou Shouya, 2013/9/17 */

	return TRUE;
}

/* BEGIN: Added by zoucaihong, 2013/3/21 */
/*****************************************************************************
 Prototype    : init_snmp_ap_config_wifidevicebasic5
 Description  : 5G卡基本配置信息节点注册函数
 Input        : SNMP_TABLE_T *pstTable   
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/3/21
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_config_wifidevicebasic5(SNMP_TABLE_T *pstTable)
{
    AP_CONFIG_WIFIDEVICEBASIC_T     *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_SCALAR(oid_ap_config_wifibasic5,
                    TABLE_ID_AP_CONFIG_WIFIBASIC5,
                    TABLE_NAME_AP_CONFIG_WIFIBASIC5,
                    NULL);

    SNMP_INIT_SCALAR_COLUMN(type->ulWifiId,         "",     TYPE_LONG,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->aucMacAddress,    "",       TYPE_MAC,   MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ulWifiDeviceType, "",   TYPE_LONG,  MIB_R);

    return TRUE;
}

/*****************************************************************************
 Prototype    : init_snmp_ap_config_wifidevicechannel5
 Description  : 5G卡信道配置信息节点注册函数
 Input        : SNMP_TABLE_T *pstTable   
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/3/21
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_config_wifidevicechannel5(SNMP_TABLE_T *pstTable)
{
    AP_CONFIG_WIFIDEVICECHANNEL_T     *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_SCALAR(oid_ap_config_wifichannel5,
                    TABLE_ID_AP_CONFIG_WIFICHANNEL5,
                    TABLE_NAME_AP_CONFIG_WIFICHANNEL5,
                    snmp_check_valid_ap_config_wifidevicechannel);

    SNMP_INIT_SCALAR_COLUMN(type->ulWifiId,         "", TYPE_LONG, MIB_R);
		
    SNMP_INIT_SCALAR_COLUMN(type->ulAbgMode,        "",      TYPE_LONG, MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulChannelUsing,   "", TYPE_LONG, MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ulChannelState,   "", TYPE_LONG, MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->ulAutoChanTime,   "", TYPE_LONG, MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->ulStaicChannel,   "", TYPE_LONG, MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->acAutoChanList,   "", TYPE_CHAR, MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulRFNum,          "",        TYPE_LONG, MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulBwMode,         "",       TYPE_LONG, MIB_RW);

    return TRUE;
}
/*****************************************************************************
 Prototype    : init_snmp_ap_config_wifidevicepower5
 Description  : 5G卡功率配置信息节点注册函数
 Input        : SNMP_TABLE_T *pstTable   
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/3/21
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_config_wifidevicepower5(SNMP_TABLE_T *pstTable)
{
    AP_CONFIG_WIFIDEVICEPOWER_T     *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_SCALAR(oid_ap_config_wifipower5,
                    TABLE_ID_AP_CONFIG_WIFIPOWER5,
                    TABLE_NAME_AP_CONFIG_WIFIPOWER5,
                    snmp_check_valid_ap_config_wifidevicepower);
	
    SNMP_INIT_SCALAR_COLUMN(type->ulWifiId,         "",     TYPE_LONG, MIB_R);
	
    SNMP_INIT_SCALAR_COLUMN(type->ulLimitTxPower,   "",     TYPE_LONG, MIB_R);	
	SNMP_INIT_SCALAR_COLUMN(type->ulMaxTxPower,     "",       TYPE_LONG, MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulTransmitPower,  "",  TYPE_LONG, MIB_R);

	return TRUE;
}
/*****************************************************************************
 Prototype    : init_snmp_ap_config_wifideviceadvance5
 Description  : 5G卡告警配置信息节点注册函数
 Input        : SNMP_TABLE_T *pstTable   
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/3/21
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_config_wifideviceadvance5(SNMP_TABLE_T *pstTable)
{
    AP_CONFIG_WIFIDEVICEADVANCE_T     *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_SCALAR(oid_ap_config_wifiadvanced5,
                    TABLE_ID_AP_CONFIG_WIFIADVANCE5,
                    TABLE_NAME_AP_CONFIG_WIFIADVANCE5,
                    snmp_check_valid_ap_config_wifideviceadvance);

    SNMP_INIT_SCALAR_COLUMN(type->ulWifiId,     "", TYPE_LONG,  MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ulRssiMode,   "",     TYPE_INT,   MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->iRssi,        "",      TYPE_INT,   MIB_RW);
	
	return TRUE;
}
/* END: Added by zoucaihong, 2013/3/21 */
/*****************************************************************************
 Prototype    : init_snmp_ap_config_wifidevicerate5
 Description  : 5G卡级速率配置
 Input        : SNMP_TABLE_T *pstTable  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/8/29
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_config_wifidevicerate5(SNMP_TABLE_T *pstTable)
{
    AP_CONFIG_WIFIDEVICERATE_T     *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_SCALAR(oid_ap_config_wifirate5,
                    TABLE_ID_AP_CONFIG_WIFIRATE5,
                    TABLE_NAME_AP_CONFIG_WIFIRATE5,
                    snmp_check_valid_ap_config_wifidevicerate);

    SNMP_INIT_SCALAR_COLUMN(type->ulWifiId,     "", TYPE_INT,  MIB_R);
	
	SNMP_INIT_SCALAR_COLUMN(type->acTransmitSpeedBasic,   "", TYPE_CHAR,   MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->acTransmitSpeed11n,     "",   TYPE_CHAR,   MIB_RW);
	/* BEGIN: Added by Zhou Shouya, 2013/9/17 */
    SNMP_INIT_SCALAR_COLUMN(type->ulRateMode,   "", TYPE_INT,  MIB_R);
	/* END:   Added by Zhou Shouya, 2013/9/17 */

	return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL init_snmp_ap_config_vap( SNMP_TABLE_T *pstTable )
{
    AP_CONFIG_VAP_T *type = NULL;

    if ( NULL == pstTable )
    {
        return FALSE;
    }

    SNMP_INIT_SCALAR(oid_ap_config_vap,
                    TABLE_ID_AP_CONFIG_VAP,
                    TABLE_NAME_AP_CONFIG_VAP,
                    NULL);

    SNMP_INIT_SCALAR_COLUMN(type->ulVapNum, "", TYPE_LONG, MIB_R);

    return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL init_snmp_ap_config_vapbasictable( SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum )
{
    AP_CONFIG_VAPBASIC_ENTRY_T  *type = NULL;

    if ( NULL == pstTable )
    {
        return FALSE;
    }

    SNMP_INIT_TABLE(oid_ap_config_vapbasic_table,
                    TABLE_ID_AP_CONFIG_VAPBASICTABLE,
                    TABLE_NAME_AP_CONFIG_VAPBASICTABLE,
                    ulMaxTableRecNum,
                    snmp_check_valid_ap_config_vapbasic_table);

    SNMP_INIT_TABLE_COLUMN(type->ulIndex,	           "",             TYPE_LONG, MIB_R,  TRUE);
	SNMP_INIT_COLUMN_RANGE(1, 16);
	SNMP_INIT_TABLE_COLUMN(type->ulVapId,              "",             TYPE_LONG, MIB_R,  FALSE);
	SNMP_INIT_COLUMN_RANGE(0, 32);
	SNMP_INIT_TABLE_COLUMN(type->ulWifiId,             "",            TYPE_LONG, MIB_R,  FALSE);
    SNMP_INIT_TABLE_COLUMN(type->acSsid,	           "",           TYPE_CHAR, MIB_RW, FALSE);
	SNMP_INIT_COLUMN_RANGE(0, 32);
    SNMP_INIT_TABLE_COLUMN(type->ulSsidEnabled,	       "",        TYPE_LONG, MIB_RW, FALSE);
    SNMP_INIT_TABLE_COLUMN(type->ulSsidBroadcast,      "",      TYPE_LONG, MIB_RW, FALSE);
    SNMP_INIT_TABLE_COLUMN(type->ulVlanId,	           "",            TYPE_LONG, MIB_RW, FALSE);
	SNMP_INIT_COLUMN_RANGE(1, 4096);
    SNMP_INIT_TABLE_COLUMN(type->ulVlanEnabled,	       "",       TYPE_LONG, MIB_RW, FALSE);
    SNMP_INIT_TABLE_COLUMN(type->aucBssid,             "",             TYPE_MAC,  MIB_R,  FALSE);
    SNMP_INIT_TABLE_COLUMN(type->ulMaxSimultUsers,	   "",    TYPE_LONG, MIB_RW, FALSE);
	SNMP_INIT_COLUMN_RANGE(0, 128);
    SNMP_INIT_TABLE_COLUMN(type->ulFlowSwitch,   "",  TYPE_LONG, MIB_RW,  FALSE);
    SNMP_INIT_TABLE_COLUMN(type->ulUplinkMaxRate,   "",  TYPE_LONG, MIB_RW,  FALSE);
	SNMP_INIT_COLUMN_RANGE(0, 30000);
    SNMP_INIT_TABLE_COLUMN(type->ulDownlinkMaxRate, "",TYPE_LONG, MIB_RW,  FALSE);
	SNMP_INIT_COLUMN_RANGE(0, 30000);
    SNMP_INIT_TABLE_COLUMN(type->ulUserUpload,      "",  TYPE_LONG, MIB_RW,  FALSE);
    SNMP_INIT_TABLE_COLUMN(type->ulUserDownload,    "",TYPE_LONG, MIB_RW,  FALSE);
    SNMP_INIT_TABLE_COLUMN(type->ulEndUserTime,           "",           TYPE_LONG, MIB_RW, FALSE);
    SNMP_INIT_TABLE_COLUMN(type->ulRowStatus,          "",         TYPE_ROWSTATUS, MIB_RW,  FALSE);

    return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL init_snmp_ap_config_vapsecuritytable( SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum )
{
    AP_CONFIG_VAPSECURITY_ENTRY_T  *type = NULL;

    if ( NULL == pstTable )
    {
        return FALSE;
    }

    SNMP_INIT_TABLE(oid_ap_config_vapsecurity_table,
                    TABLE_ID_AP_CONFIG_VAPSECURITYTABLE,
                    TABLE_NAME_AP_CONFIG_VAPSECURITYTABLE,
                    ulMaxTableRecNum,
                    snmp_check_valid_ap_config_vapsecurity_table);

    SNMP_INIT_TABLE_COLUMN(type->ulIndex,	            "",              TYPE_LONG, MIB_R,  TRUE);
	SNMP_INIT_COLUMN_RANGE(1, 16);
	SNMP_INIT_TABLE_COLUMN(type->ulVapId,               "",              TYPE_LONG, MIB_R,  FALSE);
	SNMP_INIT_COLUMN_RANGE(0, 32);
	SNMP_INIT_TABLE_COLUMN(type->ulWifiId,              "",             TYPE_LONG, MIB_R,  FALSE);
    SNMP_INIT_TABLE_COLUMN(type->acSsid,	            "",            TYPE_CHAR, MIB_R,  FALSE);
    SNMP_INIT_TABLE_COLUMN(type->ulAuthenticationType,  "", TYPE_LONG, MIB_RW, FALSE);
    SNMP_INIT_TABLE_COLUMN(type->ulSecurityMode,	    "",       TYPE_LONG, MIB_RW, FALSE);
    SNMP_INIT_TABLE_COLUMN(type->ulEncryptType,	        "",        TYPE_LONG, MIB_RW, FALSE);
    SNMP_INIT_TABLE_COLUMN(type->acWpaKey,	            "",             TYPE_CHAR, MIB_RW, FALSE);
    SNMP_INIT_TABLE_COLUMN(type->ulWpaKeyRenewal,       "",      TYPE_LONG, MIB_RW, FALSE);
    SNMP_INIT_TABLE_COLUMN(type->ulWepKeyFormat,        "",       TYPE_LONG, MIB_RW, FALSE);
    SNMP_INIT_TABLE_COLUMN(type->acWepKey,              "",             TYPE_CHAR, MIB_RW, FALSE);
    SNMP_INIT_TABLE_COLUMN(type->ulRadiusPort,          "",         TYPE_LONG, MIB_RW, FALSE);
    SNMP_INIT_TABLE_COLUMN(type->aucRadiusAddr,         "",         TYPE_IP,   MIB_RW, FALSE);
    SNMP_INIT_TABLE_COLUMN(type->acRadiusSecu,          "",         TYPE_CHAR, MIB_RW, FALSE);
    SNMP_INIT_TABLE_COLUMN(type->ulAccountPort,         "",        TYPE_LONG, MIB_RW, FALSE);
    SNMP_INIT_TABLE_COLUMN(type->aucAccountAddr,        "",        TYPE_IP,   MIB_RW, FALSE);
    SNMP_INIT_TABLE_COLUMN(type->acAccountSecu,         "",        TYPE_CHAR, MIB_RW, FALSE);
    SNMP_INIT_TABLE_COLUMN(type->ulAccountInterval,     "",    TYPE_LONG, MIB_RW, FALSE);
    SNMP_INIT_TABLE_COLUMN(type->ulRadiusAgingTime,     "",    TYPE_LONG, MIB_RW, FALSE);
    SNMP_INIT_TABLE_COLUMN(type->ulRowStatus,           "",          TYPE_ROWSTATUS, MIB_RW,  FALSE);

    return TRUE;
}

#endif
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL init_snmp_ap_config_alarm(SNMP_TABLE_T *pstTable)
{
    AP_CONFIG_ALARM_T *type = NULL;
    
    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_SCALAR(oid_ap_config_alarm,
                     TABLE_ID_AP_CONFIG_ALARM,
                     TABLE_NAME_AP_CONFIG_ALARM,
                     snmp_check_valid_ap_config_alarm);

    #if 0
    SNMP_INIT_SCALAR_COLUMN(type->ulApInterfNumThreshold,  "ApInterfNumThreshold",  TYPE_LONG,    MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->ulStaInterfNumThreshold, "StaInterfNumThreshold", TYPE_LONG,    MIB_R);
    //SNMP_INIT_SCALAR_COLUMN(type->ulCpuUsageThreshold,     "CpuUsageThreshold",     TYPE_LONG,    MIB_RW);
	//SNMP_INIT_COLUMN_RANGE(1, 100);
    //SNMP_INIT_SCALAR_COLUMN(type->ulMemUsageThreshold,     "MemUsageThreshold",     TYPE_LONG,    MIB_RW);
	//SNMP_INIT_COLUMN_RANGE(1, 100);
    #endif
	//SNMP_INIT_SCALAR_COLUMN(type->ulCpuLoadPeriod,  "CpuLoadPeriod",   TYPE_LONG,    MIB_RW);
	//SNMP_INIT_SCALAR_COLUMN(type->ulCpuLoadCount,   "CpuLoadCount",    TYPE_LONG,    MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->ulCpuLoadHigh,    "",     TYPE_LONG,    MIB_RW);
	SNMP_INIT_COLUMN_RANGE(1, 100);
	SNMP_INIT_SCALAR_COLUMN(type->ulCpuLoadLow,     "",      TYPE_LONG,    MIB_RW);
	SNMP_INIT_COLUMN_RANGE(1, 100);
	
	//SNMP_INIT_SCALAR_COLUMN(type->ulMemLoadPeriod,  "MemLoadPeriod",   TYPE_LONG,    MIB_RW);
	//SNMP_INIT_SCALAR_COLUMN(type->ulMemLoadCount,   "MemLoadCount",    TYPE_LONG,    MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->ulMemLoadHigh,    "",     TYPE_LONG,    MIB_RW);
	SNMP_INIT_COLUMN_RANGE(1, 100);
	SNMP_INIT_SCALAR_COLUMN(type->ulMemLoadLow,     "",      TYPE_LONG,    MIB_RW);
	SNMP_INIT_COLUMN_RANGE(1, 100);
	#if 0
	SNMP_INIT_SCALAR_COLUMN(type->ulTempPeriod,     "TempPeriod",      TYPE_LONG,    MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->ulTempCount,      "TempCount",       TYPE_LONG,    MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->ulTempHigh,       "TempHighRaise",   TYPE_LONG,    MIB_RW);
	SNMP_INIT_COLUMN_RANGE(0, 100);
	SNMP_INIT_COLUMN_UNITS("C");
	SNMP_INIT_SCALAR_COLUMN(type->ulTempLow,        "TempHighClear",   TYPE_LONG,    MIB_RW);
	SNMP_INIT_COLUMN_RANGE(0, 100);
	SNMP_INIT_COLUMN_UNITS("C");
	
	SNMP_INIT_SCALAR_COLUMN(type->iTempHigh,        "TempLowRaise",    TYPE_INT,     MIB_RW);
	SNMP_INIT_COLUMN_RANGE(-50, 0);
	SNMP_INIT_COLUMN_UNITS("C");
	SNMP_INIT_SCALAR_COLUMN(type->iTempLow,         "TempLowClear",    TYPE_INT,     MIB_RW);
	SNMP_INIT_COLUMN_RANGE(-50, 0);
	SNMP_INIT_COLUMN_UNITS("C");
    #endif
	
    return TRUE;
}
/* END: Added by zoucaihong, 2012/11/5 */
/* BEGIN: Added by zoucaihong, 2012/11/15 */
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL init_snmp_ap_config_trapdes(SNMP_TABLE_T *pstTable)
{
	AP_CONFIG_TRAPDES_ENTRY_T *type = NULL;

	if (NULL == pstTable)
	{
		return FALSE;
	}

	SNMP_INIT_SCALAR(oid_ap_config_trapdes,
		TABLE_ID_AP_CONFIG_TRAPDES,
		TABLE_NAME_AP_CONFIG_TRAPDES,
		snmp_check_valid_ap_config_trapdes);

	SNMP_INIT_SCALAR_COLUMN(type->ulSwtich,             "",           TYPE_INT,   MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->aucTrapDesIPAddress,  "",           TYPE_IP,    MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulTrapDesPort,        "",          TYPE_INT,   MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->acTrapCommunity,      "",        TYPE_CHAR,  MIB_RW);
	return TRUE;
}

#if 0
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL init_snmp_ap_config_trapdes_table(SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum)
{
	AP_CONFIG_TRAPDES_ENTRY_T *type = NULL;

	if (NULL == pstTable)
	{
		return FALSE;
	}

	SNMP_INIT_TABLE(oid_ap_config_trapdes_table,
		TABLE_ID_AP_CONFIG_TRAPDES_TABLE,
		TABLE_NAME_AP_CONFIG_TRAPDESTABLE,
		ulMaxTableRecNum,
		snmp_check_valid_ap_config_trapdes);

	SNMP_INIT_TABLE_COLUMN(type->ulIndex,               "Index",            TYPE_LONG,  MIB_RC, TRUE);
	SNMP_INIT_TABLE_COLUMN(type->aucTrapDesIPAddress,   "TrapDestIP",       TYPE_IP,    MIB_RW, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulTrapDesPort,         "TrapDesPort",      TYPE_LONG,  MIB_RW, FALSE);
	SNMP_INIT_COLUMN_DEFVAL(NULL, 162);
    SNMP_INIT_TABLE_COLUMN(type->acTrapCommunity,       "TrapCommunity",    TYPE_CHAR,  MIB_RW, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulRowStatus,           "RowStatus",        TYPE_ROWSTATUS, MIB_RW, FALSE);

	return TRUE;
}
#endif
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL init_snmp_ap_stat_sys(SNMP_TABLE_T *pstTable)
{
    AP_STAT_SYS_T *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }

    SNMP_INIT_SCALAR(oid_ap_stat_sys,
                     TABLE_ID_AP_STAT_SYS,
                     TABLE_NAME_AP_STAT_SYS,
                     NULL);

    SNMP_INIT_SCALAR_COLUMN(type->ulCPURTUsage,         "", TYPE_LONG,    MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ulCPUPeakUsage,       "", TYPE_LONG,    MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ulCPUAvgUsage,        "", TYPE_LONG,    MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ulMemRTUsage,         "", TYPE_LONG,    MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ulMemPeakUsage,       "", TYPE_LONG,    MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ulMemAvgUsage,        "", TYPE_LONG,    MIB_R);
    /* BEGIN: Added by zoucaihong, 2015/3/24 */
    SNMP_INIT_SCALAR_COLUMN(type->ulMemFree,            "", TYPE_LONG,    MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ulApLoad,             "", TYPE_LONG,    MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ulApNum,              "", TYPE_LONG,    MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ulApOnlineNum,        "", TYPE_LONG,    MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ulApOfflineNum,       "", TYPE_LONG,    MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ulOnlineUserNum,      "", TYPE_LONG,    MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->ulFeelUserNum,        "", TYPE_LONG,    MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ulTotalUp,            "", TYPE_LONGLONG,    MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ulTotalDw,            "", TYPE_LONGLONG,    MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ulUpSpeed,            "", TYPE_LONG,    MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ulDwSpeed,            "", TYPE_LONG,    MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ulTotalWideBand,      "", TYPE_LONG,    MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ulWidebandPerused,    "", TYPE_LONG,    MIB_R);
    /* END: Added by zoucaihong, 2015/3/24 */
    return TRUE;
}

#ifndef GW_X86
/* END: Added by zoucaihong, 2012/11/15 */
/* BEGIN: Added by zoucaihong, 2012/11/21 
增加VLAN配置表注册函数*/
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL init_snmp_ap_config_vlan(SNMP_TABLE_T *pstTable)
{
	AP_CONFIG_VLAN_T *type = NULL;

	if (NULL == pstTable)
	{
		return FALSE;
	}

	SNMP_INIT_SCALAR(oid_ap_config_vlan,
		TABLE_ID_AP_CONFIG_VLAN,
		TABLE_NAME_AP_CONFIG_VLAN,
		NULL);

	SNMP_INIT_SCALAR_COLUMN(type->ulVlanNum, "", TYPE_LONG,  MIB_R);

	return TRUE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL init_snmp_ap_config_vlan_table(SNMP_TABLE_T * pstTable, UINT32 ulMaxTableRecNum)
{
	AP_CONFIG_VLAN_ENTRY_T *type = NULL;

	if (NULL == pstTable)
	{
		return FALSE;
	}

	SNMP_INIT_TABLE(oid_ap_config_vlan_table,
		TABLE_ID_AP_CONFIG_VLAN_TABLE,
		TABLE_NAME_AP_CONFIG_VLANTABLE,
		ulMaxTableRecNum,
		snmp_check_valid_ap_config_vlan);

	SNMP_INIT_TABLE_COLUMN(type->ulIndex,       "",        TYPE_LONG,      MIB_RC, TRUE);
	SNMP_INIT_TABLE_COLUMN(type->ulVlanId,      "",       TYPE_LONG,      MIB_RW, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->acVlanName,    "",     TYPE_CHAR,      MIB_RW, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulNvramId,     "",      TYPE_LONG,      MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulVlanPrio,    "",     TYPE_LONG,      MIB_RW, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulRowStatus,   "",    TYPE_ROWSTATUS, MIB_RW, FALSE);

	return TRUE;
}
/* END: Added by zoucaihong, 2012/11/21 */
BOOL init_snmp_ap_config_ssid(SNMP_TABLE_T *pstTable)
{
	AP_CONFIG_SSID_T *type = NULL;

	if (NULL == pstTable)
	{
		return FALSE;
	}

	SNMP_INIT_SCALAR(oid_ap_config_ssid,
		TABLE_ID_AP_CONFIG_SSID,
		TABLE_NAME_AP_CONFIG_SSID,
		NULL);

	SNMP_INIT_SCALAR_COLUMN(type->ulNum,     "",       TYPE_LONG,  MIB_R);

	return TRUE;
}
BOOL init_snmp_ap_config_ssid_table(SNMP_TABLE_T * pstTable, UINT32 ulMaxTableRecNum)
{
	AP_CONFIG_SSID_ENTRY_T *type = NULL;

	if (NULL == pstTable)
	{
		return FALSE;
	}

	SNMP_INIT_TABLE(oid_ap_config_ssid_table,
		TABLE_ID_AP_CONFIG_SSID_TABLE,
		TABLE_NAME_AP_CONFIG_SSIDTABLE,
		ulMaxTableRecNum,
		snmp_check_valid_ap_config_ssid);

	SNMP_INIT_TABLE_COLUMN(type->ulIndex,       "",        TYPE_LONG,      MIB_RC, TRUE);
	SNMP_INIT_TABLE_COLUMN(type->ulWifiId,      "",       TYPE_LONG,      MIB_RC, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulVapId,       "",        TYPE_LONG,      MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->acSsidName,    "",     TYPE_CHAR,      MIB_RC, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulRowStatus,   "",    TYPE_ROWSTATUS, MIB_RC, FALSE);

	return TRUE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL init_snmp_ap_stat_wifi( SNMP_TABLE_T *pstTable )
{
    AP_STAT_WIFI_T  *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }

	SNMP_INIT_SCALAR(oid_ap_stat_wifi,
		TABLE_ID_AP_STAT_WIFI,
		TABLE_NAME_AP_STAT_WIFI,
		NULL);
	
	SNMP_INIT_SCALAR_COLUMN(type->ulWifiNum,            "",          TYPE_LONG,     MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ullApRecvBytes,       "",      TYPE_LONGLONG, MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ullApSendBytes,       "",      TYPE_LONGLONG, MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ullWifi24RecvBytes,   "",  TYPE_LONGLONG, MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ullWifi24SendBytes,   "",  TYPE_LONGLONG, MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ullWifi5RecvBytes,    "",   TYPE_LONGLONG, MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ullWifi5SendBytes,    "",   TYPE_LONGLONG, MIB_R);

	return TRUE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL init_snmp_ap_stat_wifi_table( SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum)
{
    AP_STAT_WIFI_ENTRY_T    *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_TABLE(oid_ap_stat_wifi_table,
                    TABLE_ID_AP_STAT_WIFITABLE,
                    TABLE_NAME_AP_STAT_WIFITABLE,
                    ulMaxTableRecNum,
                    NULL);

	SNMP_INIT_TABLE_COLUMN(type->ulIndex,	            "",              TYPE_LONG, MIB_R, TRUE);
	SNMP_INIT_TABLE_COLUMN(type->ulWifiId,	            "",             TYPE_LONG, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ullRecvBytes,	        "",          TYPE_LONGLONG, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ullSendBytes,	        "",          TYPE_LONGLONG, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulAssoStaNum,          "",         TYPE_LONG, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulRecvFrameNum,        "",       TYPE_LONG, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulSendFrameNum,        "",       TYPE_LONG, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulRecvErrFrame,        "",       TYPE_LONG, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulSendErrFrame,        "",       TYPE_LONG, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulErrFrameRate,        "",       TYPE_LONG, MIB_R, FALSE);
	
    return TRUE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL init_snmp_ap_stat_wifivap( SNMP_TABLE_T *pstTable )
{
    AP_STAT_WIFIVAP_T  *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
	SNMP_INIT_SCALAR(oid_ap_stat_wifivap,
		TABLE_ID_AP_STAT_WIFIVAP,
		TABLE_NAME_AP_STAT_WIFIVAP,
		NULL);
	
	SNMP_INIT_SCALAR_COLUMN(type->ulVapNum,  "", TYPE_LONG, MIB_R);
	
    return TRUE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL init_snmp_ap_stat_wifivap_table( SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum)
{
    AP_STAT_WIFIVAP_ENTRY_T    *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_TABLE(oid_ap_stat_wifivap_table,
                    TABLE_ID_AP_STAT_WIFIVAPTABLE,
                    TABLE_NAME_AP_STAT_WIFIVAPTABLE,
                    ulMaxTableRecNum,
                    NULL);

	SNMP_INIT_TABLE_COLUMN(type->ulIndex,	            "",              TYPE_LONG, MIB_R, TRUE);
	SNMP_INIT_TABLE_COLUMN(type->ulVapId,	            "",              TYPE_LONG, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulWifiId,	            "",             TYPE_LONG, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->acVapSsid,	            "",            TYPE_CHAR, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ullRecvBytes,	        "",          TYPE_LONGLONG, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ullSendBytes,	        "",          TYPE_LONGLONG, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulAssoStaNum,          "",         TYPE_LONG, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulRecvFrameNum,        "",       TYPE_LONG, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulSendFrameNum,        "",       TYPE_LONG, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulRecvErrFrame,        "",       TYPE_LONG, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulSendErrFrame,        "",       TYPE_LONG, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulErrFrameRate,        "",       TYPE_LONG, MIB_R, FALSE);
	
    return TRUE;
}
#endif
/* BEGIN: Added by zoucaihong, 2015/3/24 */
/* 用户添加/删除 */
BOOL init_snmp_ap_config_user( SNMP_TABLE_T *pstTable )
{
    AP_CONFIG_USER_T    *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
	SNMP_INIT_SCALAR(oid_ap_config_user,
		TABLE_ID_AP_CONFIG_USER,
		TABLE_NAME_AP_CONFIG_USER,
		snmp_check_valid_ap_config_user);
	
	SNMP_INIT_SCALAR_COLUMN(type->ulAction,     "", TYPE_LONG,  MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->aucMac,       "", TYPE_MAC,   MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->ulAuthType,   "", TYPE_LONG,  MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->acUserId,     "", TYPE_CHAR,  MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->ulTimeLimit,  "", TYPE_LONG,  MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->ulCommit,     "", TYPE_LONG,  MIB_RW);
	
    return TRUE;
}
/* 路由器SSID配置 */
BOOL init_snmp_ap_config_gwssid( SNMP_TABLE_T *pstTable )
{
    AP_CONFIG_GWSSID_T  *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
	SNMP_INIT_SCALAR(oid_ap_config_gwssid,
		TABLE_ID_AP_CONFIG_GWSSID,
		TABLE_NAME_AP_CONFIG_GWSSID,
		snmp_check_valid_ap_config_gwssid);
	
	SNMP_INIT_SCALAR_COLUMN(type->acGwSsid,  "", TYPE_CHAR, MIB_RW);
	
    return TRUE;
}
/* 路由器认证策略配置 */
BOOL init_snmp_ap_config_auth( SNMP_TABLE_T *pstTable )
{
    AP_CONFIG_AUTH_T    *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
	SNMP_INIT_SCALAR(oid_ap_config_auth,
		TABLE_ID_AP_CONFIG_AUTH,
		TABLE_NAME_AP_CONFIG_AUTH,
		snmp_check_valid_ap_config_auth);
	
	SNMP_INIT_SCALAR_COLUMN(type->ulSwitch,     "", TYPE_LONG, MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->ulWebAuth,    "", TYPE_LONG, MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->ulOffline,    "", TYPE_LONG, MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->ulReverify,   "", TYPE_LONG, MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->acFreeMac,    "", TYPE_CHAR, MIB_RW);
	
    return TRUE;
}

/* 批量设置认证客户端黑/白名单 */
BOOL init_snmp_ap_config_batch_bwlist( SNMP_TABLE_T *pstTable )\
{
    AP_CONFIG_BATCH_BWLIST_T    *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
	SNMP_INIT_SCALAR(oid_ap_config_batch_bwlist,
		TABLE_ID_AP_CONFIG_BATCH_BWLIST,
		TABLE_NAME_AP_CONFIG_BATCH_BWLIST,
		snmp_check_valid_ap_config_batch_bwlist);
	
	SNMP_INIT_SCALAR_COLUMN(type->ulListType,   "", TYPE_LONG, MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->acMacList,    "", TYPE_CHAR, MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->ulStatus,     "", TYPE_LONG, MIB_RW);
	
    return TRUE;
}
/* END: Added by zoucaihong, 2015/3/24 */
/* BEGIN: Added by zoucaihong, 2015/4/16 */
BOOL init_snmp_ap_config_domain_filter(SNMP_TABLE_T *pstTable)
{
    AP_CONFIG_DOMAIN_FILTER_T   *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_SCALAR(oid_ap_config_domainfilter,
                    TABLE_ID_AP_CONFIG_DOAMINFILTER,
                    TABLE_NAME_AP_CONFIG_DOMAINFILTER,
                    snmp_check_valid_ap_config_domain_filter);

    SNMP_INIT_SCALAR_COLUMN(type->ulIndex,  "", TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->acDomain, "", TYPE_CHAR,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulType,   "", TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulStatus, "", TYPE_LONG,  MIB_RW);

    return TRUE;
}
BOOL init_snmp_ap_config_user_speed(SNMP_TABLE_T *pstTable)
{
    AP_CONFIG_USER_SPEED_T  *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_SCALAR(oid_ap_config_user_speed,
                    TABLE_ID_AP_CONFIG_USER_SPEED,
                    TABLE_NAME_AP_CONFIG_USER_SPEED,
                    snmp_check_valid_ap_config_user_speed);

    SNMP_INIT_SCALAR_COLUMN(type->ulUploadMax,  "", TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulDwloadMax,  "", TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulCommit,     "", TYPE_LONG,  MIB_RW);

    return TRUE;
}
BOOL init_snmp_ap_config_white_user_speed(SNMP_TABLE_T *pstTable)
{
    AP_CONFIG_USER_SPEED_T  *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_SCALAR(oid_ap_config_white_user_speed,
                    TABLE_ID_AP_CONFIG_WHITE_USER_SPEED,
                    TABLE_NAME_AP_CONFIG_WHITE_USER_SPEED,
                    snmp_check_valid_ap_config_white_user_speed);

    SNMP_INIT_SCALAR_COLUMN(type->ulUploadMax,  "", TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulDwloadMax,  "", TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulCommit,     "", TYPE_LONG,  MIB_RW);

    return TRUE;
}

BOOL init_snmp_ap_config_user_logout(SNMP_TABLE_T *pstTable)
{
    AP_CONFIG_USER_LOGOUT_T *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_SCALAR(oid_ap_config_user_logout,
                    TABLE_ID_AP_CONFIG_USER_LOGOUT,
                    TABLE_NAME_AP_CONFIG_USER_LOGOUT,
                    snmp_check_valid_ap_config_user_logout);

    SNMP_INIT_SCALAR_COLUMN(type->ulLogoutStatus,   "", TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulTimeCondition,  "", TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulFlowCondition,  "", TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulCommit,         "", TYPE_LONG,  MIB_RW);

    return TRUE;
}

BOOL init_snmp_ap_config_get_secret_key(SNMP_TABLE_T *pstTable)
{
    AP_CONFIG_GET_SECRET_KEY_T *type = NULL;

    if ( NULL == pstTable )
    {
        return FALSE;
    }

    SNMP_INIT_SCALAR(oid_ap_config_get_secret_key,
                    TABLE_ID_AP_CONFIG_GET_SECRET_KEY,
                    TABLE_NAME_AP_CONFIG_GET_SECRET_KEY,
                    snmp_check_valid_ap_config_get_secret_key);

    SNMP_INIT_SCALAR_COLUMN(type->ulInterval,   "", TYPE_LONG,  MIB_RW);
    return TRUE;
}

/* END: Added by zoucaihong, 2015/4/16 */
/* BEGIN: Added by zoucaihong, 2012/11/22 */
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL init_snmp_ap_stat_station( SNMP_TABLE_T *pstTable )
{
    AP_STAT_STATION_T  *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
	SNMP_INIT_SCALAR(oid_ap_stat_station,
		TABLE_ID_AP_STAT_STATION,
		TABLE_NAME_AP_STAT_STATION,
		NULL);
	
	SNMP_INIT_SCALAR_COLUMN(type->ulStationNum,  "", TYPE_LONG, MIB_R);
	
    return TRUE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL init_snmp_ap_stat_station_table( SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum )
{
    AP_STAT_STATION_ENTRY_T    *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_TABLE(oid_ap_stat_station_table,
                    TABLE_ID_AP_STAT_STATIONTABLE,
                    TABLE_NAME_AP_STAT_STATIONTABLE,
                    ulMaxTableRecNum,
                    NULL);

	SNMP_INIT_TABLE_COLUMN(type->ulStaID,               "",                TYPE_LONG, MIB_R, TRUE);
	SNMP_INIT_TABLE_COLUMN(type->aucBssid,              "",                TYPE_MAC,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->aucSsid,               "",                 TYPE_CHAR, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulVlanId,              "",               TYPE_LONG, MIB_R, FALSE);
    /* BEGIN: Added by zoucaihong, 2013/4/18 */
    SNMP_INIT_TABLE_COLUMN(type->ulVlanEnable,          "",           TYPE_LONG, MIB_R, FALSE);
    /* END: Added by zoucaihong, 2013/4/18 */
	SNMP_INIT_TABLE_COLUMN(type->aucStaMac,             "",               TYPE_MAC,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->aucStaDevType,         "",           TYPE_CHAR, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulConnectTime,         "",          TYPE_TIME, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulStaUplinkOctets,     "",      TYPE_LONGLONG, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulStaDownlinkOctets,   "",    TYPE_LONGLONG, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->iRssi,                 "",                 TYPE_INT,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->aucFrequency,          "",            TYPE_CHAR, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulSnr,	                "",                  TYPE_LONG, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->acTxRate,              "",               TYPE_CHAR, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulErrRate,             "",              TYPE_LONG, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->uAssoRate,             "",             TYPE_LONG, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->aucStaIp,              "",                TYPE_IP,   MIB_R, FALSE);
	
    return TRUE;
}
/* END: Added by zoucaihong, 2012/11/22 */
/* BEGIN: Added by zoucaihong, 2015/3/24 */
/* 路由器在线用户列表 */
BOOL init_snmp_ap_stat_onlineuser_table( SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum )
{
    AP_STAT_ONLINEUSER_ENTRY_T    *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_TABLE(oid_ap_stat_onlineuser,
                    TABLE_ID_AP_STAT_ONLINEUSERTABLE,
                    TABLE_NAME_AP_STAT_ONLINEUSERTABLE,
                    ulMaxTableRecNum,
                    NULL);

	SNMP_INIT_TABLE_COLUMN(type->aucUserMac,        "", TYPE_MAC,       MIB_R, TRUE);
	SNMP_INIT_TABLE_COLUMN(type->ulAuthType,        "", TYPE_LONG,      MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulUpLoad,          "", TYPE_LONGLONG,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulDwLoad,          "", TYPE_LONGLONG,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulConnect,         "", TYPE_LONG,      MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulTotalUp,         "", TYPE_LONG,      MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulTotalDw,         "", TYPE_LONG,      MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulConnectTime,     "", TYPE_LONG,      MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->acUserId,          "", TYPE_CHAR,      MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->aucUserIp,         "", TYPE_IP,        MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->acUserSsid,        "", TYPE_CHAR,      MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->acUserLoginTime,   "", TYPE_CHAR,      MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->acUserLogoutTime,  "", TYPE_CHAR,      MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulOnlineTime,      "", TYPE_LONG,      MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->acLogoutType,      "", TYPE_CHAR,      MIB_R, FALSE);
	
    return TRUE;
}

/* AP列表 */
BOOL init_snmp_ap_stat_ap_table( SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum )
{
    AP_STAT_APINFO_ENTRY_T    *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_TABLE(oid_ap_stat_apinfo,
                    TABLE_ID_AP_STAT_APINFO,
                    TABLE_NAME_AP_STAT_APINFO,
                    ulMaxTableRecNum,
                    NULL);

	SNMP_INIT_TABLE_COLUMN(type->aucApMac,          "", TYPE_MAC,   MIB_R, TRUE);
	SNMP_INIT_TABLE_COLUMN(type->ulApNo,            "", TYPE_LONG,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->aucApIp,           "", TYPE_IP,    MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulCurLink,         "", TYPE_LONG,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulTotalLink,       "", TYPE_LONG,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulRoad,            "", TYPE_LONG,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulTotalUp,         "", TYPE_LONG,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulTotalDw,         "", TYPE_LONG,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->acApMemo,          "", TYPE_CHAR,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulApStatus,        "", TYPE_LONG,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->acApCpuStatus,     "", TYPE_CHAR,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulApRuntimeLong,   "", TYPE_LONG,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->acApFirstSsid,     "", TYPE_CHAR,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->acApSecondSsid,    "", TYPE_CHAR,  MIB_R, FALSE);
	
    return TRUE;
}

/* AP系统版本信息 */
BOOL init_snmp_ap_stat_version_table( SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum )
{
    AP_STAT_VERSION_ENTRY_T *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_TABLE(oid_ap_stat_version,
                    TABLE_ID_AP_STAT_VERSION,
                    TABLE_NAME_AP_STAT_VERSION,
                    ulMaxTableRecNum,
                    NULL);

	SNMP_INIT_TABLE_COLUMN(type->aucApMac,      "", TYPE_MAC,   MIB_R, TRUE);
	SNMP_INIT_TABLE_COLUMN(type->ulApNo,        "", TYPE_LONG,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->acCurVer,      "", TYPE_CHAR,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->acNewVer,      "", TYPE_CHAR,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulApStatus,    "", TYPE_LONG,  MIB_R, FALSE);
	
    return TRUE;
}

/* AP连接终端列表 */
BOOL init_snmp_ap_stat_ap_sta_table( SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum )
{
    AP_STAT_AP_STA_ENTRY_T  *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_TABLE(oid_ap_stat_ap_sta,
                    TABLE_ID_AP_STAT_AP_STA,
                    TABLE_NAME_AP_STAT_AP_STA,
                    ulMaxTableRecNum,
                    NULL);

	SNMP_INIT_TABLE_COLUMN(type->aucApMac,      "", TYPE_MAC,   MIB_R, TRUE);
	SNMP_INIT_TABLE_COLUMN(type->aucStaMac,     "", TYPE_MAC,   MIB_R, TRUE);
	SNMP_INIT_TABLE_COLUMN(type->ulStaNo,       "", TYPE_LONG,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->acStaName,     "", TYPE_CHAR,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->aucStaIp,      "", TYPE_IP,    MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulRssi,        "", TYPE_LONG,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulConnectTime, "", TYPE_LONG,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulStaTotalUp,  "", TYPE_LONG,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulStaTotalDw,  "", TYPE_LONG,  MIB_R, FALSE);
	
    return TRUE;
}
/* END: Added by zoucaihong, 2015/3/24 */
#if 0
/*****************************************************************************
 Prototype    : init_snmp_ap_stat_24cochainterfinfo
 Description  : AP 2.4G 同频干扰信息数目节点注册函数
 Input        : SNMP_TABLE_T *pstTable  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/23
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_stat_24cochainterfinfo( SNMP_TABLE_T *pstTable )
{
    AP_STAT_24COCHAINTERFINFO_T  *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
	SNMP_INIT_SCALAR(oid_ap_stat_24cochainterfinfo,
		TABLE_ID_AP_STAT_24COCHAINTERFINFO,
		TABLE_NAME_AP_STAT_24COCHAINTERFINFO,
		NULL);
	
	SNMP_INIT_SCALAR_COLUMN(type->ulNum,  "Num", TYPE_LONG, MIB_R);
	
    return TRUE;
}
/*****************************************************************************
 Prototype    : init_snmp_ap_stat_24cochainterfinfo_table
 Description  : AP 2.4G 同频干扰信息表注册函数
 Input        : SNMP_TABLE_T *pstTable   
                UINT32 ulMaxTableRecNum  
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/23
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_stat_24cochainterfinfo_table( SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum )
{
    AP_STAT_24COCHAINTERFINFO_ENTRY_T    *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_TABLE(oid_ap_stat_24cochainterfinfo_table,
                    TABLE_ID_AP_STAT_24COCHAINTERFINFOTABLE,
                    TABLE_NAME_AP_STAT_24COCHAINTERFINFOTABLE,
                    ulMaxTableRecNum,
                    NULL);

	SNMP_INIT_TABLE_COLUMN(type->ulIndex,   "Index",    TYPE_LONG,  MIB_R, TRUE);
	SNMP_INIT_TABLE_COLUMN(type->aucBssid,  "Bssid",    TYPE_MAC,   MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->acSsid,    "Ssid",     TYPE_CHAR,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->iRssi,     "Rssi",     TYPE_INT,   MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulChannel, "Channel",  TYPE_LONG,  MIB_R, FALSE);
	
    return TRUE;
}

/*****************************************************************************
 Prototype    : init_snmp_ap_stat_5cochainterfinfo
 Description  : AP 5G 同频干扰信息数目节点注册函数
 Input        : SNMP_TABLE_T *pstTable  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/23
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_stat_5cochainterfinfo( SNMP_TABLE_T *pstTable )
{
    AP_STAT_5COCHAINTERFINFO_T  *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
	SNMP_INIT_SCALAR(oid_ap_stat_5cochainterfinfo,
		TABLE_ID_AP_STAT_5COCHAINTERFINFO,
		TABLE_NAME_AP_STAT_5COCHAINTERFINFO,
		NULL);
	
	SNMP_INIT_SCALAR_COLUMN(type->ulNum,  "Num", TYPE_LONG, MIB_R);
	
    return TRUE;
}
/*****************************************************************************
 Prototype    : init_snmp_ap_stat_5cochainterfinfo_table
 Description  : AP 5G 同频干扰信息表注册函数
 Input        : SNMP_TABLE_T *pstTable   
                UINT32 ulMaxTableRecNum  
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/23
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_stat_5cochainterfinfo_table( SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum )
{
    AP_STAT_5COCHAINTERFINFO_ENTRY_T    *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_TABLE(oid_ap_stat_5cochainterfinfo_table,
                    TABLE_ID_AP_STAT_5COCHAINTERFINFOTABLE,
                    TABLE_NAME_AP_STAT_5COCHAINTERFINFOTABLE,
                    ulMaxTableRecNum,
                    NULL);

	SNMP_INIT_TABLE_COLUMN(type->ulIndex,   "Index",    TYPE_LONG,  MIB_R, TRUE);
	SNMP_INIT_TABLE_COLUMN(type->aucBssid,  "Bssid",    TYPE_MAC,   MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->acSsid,    "Ssid",     TYPE_CHAR,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->iRssi,     "Rssi",     TYPE_INT,   MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulChannel, "Channel",  TYPE_LONG,  MIB_R, FALSE);
	
    return TRUE;
}
/*****************************************************************************
 Prototype    : init_snmp_ap_stat_24adjchainterfinfo
 Description  : AP 2.4G 邻频干扰信息数目节点注册函数
 Input        : SNMP_TABLE_T *pstTable  
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/23
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_stat_24adjchainterfinfo( SNMP_TABLE_T *pstTable )
{
    AP_STAT_24ADJCHAINTERFINFO_T  *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
	SNMP_INIT_SCALAR(oid_ap_stat_24adjchainterfinfo,
		TABLE_ID_AP_STAT_24ADJCHAINTERFINFO,
		TABLE_NAME_AP_STAT_24ADJCHAINTERFINFO,
		NULL);
	
	SNMP_INIT_SCALAR_COLUMN(type->ulNum,  "Num", TYPE_LONG, MIB_R);
	
    return TRUE;
}
/*****************************************************************************
 Prototype    : init_snmp_ap_stat_24adjchainterfinfo_table
 Description  : AP 2.4G 邻频干扰信息表注册函数
 Input        : SNMP_TABLE_T *pstTable   
                UINT32 ulMaxTableRecNum  
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/23
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_stat_24adjchainterfinfo_table( SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum )
{
    AP_STAT_24ADJCHAINTERFINFO_ENTRY_T    *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_TABLE(oid_ap_stat_24adjchainterfinfo_table,
                    TABLE_ID_AP_STAT_24ADJCHAINTERFINFOTABLE,
                    TABLE_NAME_AP_STAT_24ADJCHAINTERFINFOTABLE,
                    ulMaxTableRecNum,
                    NULL);

	SNMP_INIT_TABLE_COLUMN(type->ulIndex,   "Index",    TYPE_LONG,  MIB_R, TRUE);
	SNMP_INIT_TABLE_COLUMN(type->aucBssid,  "Bssid",    TYPE_MAC,   MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->acSsid,    "Ssid",     TYPE_CHAR,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->iRssi,     "Rssi",     TYPE_INT,   MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulChannel, "Channel",  TYPE_LONG,  MIB_R, FALSE);
	
    return TRUE;
}

/*****************************************************************************
 Prototype    : init_snmp_ap_stat_5adjchainterfinfo
 Description  : AP 5G 邻频干扰信息数目节点注册函数
 Input        : SNMP_TABLE_T *pstTable  
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/23
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_stat_5adjchainterfinfo( SNMP_TABLE_T *pstTable )
{
    AP_STAT_5ADJCHAINTERFINFO_T  *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
	SNMP_INIT_SCALAR(oid_ap_stat_5adjchainterfinfo,
		TABLE_ID_AP_STAT_5ADJCHAINTERFINFO,
		TABLE_NAME_AP_STAT_5ADJCHAINTERFINFO,
		NULL);
	
	SNMP_INIT_SCALAR_COLUMN(type->ulNum,  "Num", TYPE_LONG, MIB_R);
	
    return TRUE;
}
/*****************************************************************************
 Prototype    : init_snmp_ap_stat_5adjchainterfinfo_table
 Description  : AP 5G 邻频干扰信息表注册函数
 Input        : SNMP_TABLE_T *pstTable   
                UINT32 ulMaxTableRecNum  
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/23
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_stat_5adjchainterfinfo_table( SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum )
{
    AP_STAT_5ADJCHAINTERFINFO_ENTRY_T    *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_TABLE(oid_ap_stat_5adjchainterfinfo_table,
                    TABLE_ID_AP_STAT_5ADJCHAINTERFINFOTABLE,
                    TABLE_NAME_AP_STAT_5ADJCHAINTERFINFOTABLE,
                    ulMaxTableRecNum,
                    NULL);

	SNMP_INIT_TABLE_COLUMN(type->ulIndex,   "Index",    TYPE_LONG,  MIB_R, TRUE);
	SNMP_INIT_TABLE_COLUMN(type->aucBssid,  "Bssid",    TYPE_MAC,   MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->acSsid,    "Ssid",     TYPE_CHAR,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->iRssi,     "Rssi",     TYPE_INT,   MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulChannel, "Channel",  TYPE_LONG,  MIB_R, FALSE);
	
    return TRUE;
}
/*****************************************************************************
 Prototype    : init_snmp_ap_stat_24wlandevinterfinfo
 Description  : AP 2.4G WLAN设备干扰信息数目节点注册函数
 Input        : SNMP_TABLE_T *pstTable  
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/23
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_stat_24wlandevinterfinfo( SNMP_TABLE_T *pstTable )
{
    AP_STAT_24WLANDEVINTERFINFO_T  *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
	SNMP_INIT_SCALAR(oid_ap_stat_24wlandevinterfinfo,
		TABLE_ID_AP_STAT_24WLANDEVINTERFINFO,
		TABLE_NAME_AP_STAT_24WLANDEVINTERFINFO,
		NULL);
	
	SNMP_INIT_SCALAR_COLUMN(type->ulNum,  "Num", TYPE_LONG, MIB_R);
	
    return TRUE;
}
/*****************************************************************************
 Prototype    : init_snmp_ap_stat_24wlandevinterfinfo_table
 Description  : AP 2.4G WLAN设备干扰信息表注册函数
 Input        : SNMP_TABLE_T *pstTable   
                UINT32 ulMaxTableRecNum  
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/23
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_stat_24wlandevinterfinfo_table( SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum )
{
    AP_STAT_24WLANDEVINTERFINFO_ENTRY_T    *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_TABLE(oid_ap_stat_24wlandevinterfinfo_table,
                    TABLE_ID_AP_STAT_24WLANDEVINTERFINFOTABLE,
                    TABLE_NAME_AP_STAT_24WLANDEVINTERFINFOTABLE,
                    ulMaxTableRecNum,
                    NULL);

	SNMP_INIT_TABLE_COLUMN(type->ulIndex,   "Index",    TYPE_LONG,  MIB_R, TRUE);
	SNMP_INIT_TABLE_COLUMN(type->aucBssid,  "Bssid",    TYPE_MAC,   MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->acSsid,    "Ssid",     TYPE_CHAR,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->iRssi,     "Rssi",     TYPE_INT,   MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulChannel, "Channel",  TYPE_LONG,  MIB_R, FALSE);
	
    return TRUE;
}
/*****************************************************************************
 Prototype    : init_snmp_ap_stat_5wlandevinterfinfo
 Description  : AP 5G WLAN设备干扰信息数目节点注册函数
 Input        : SNMP_TABLE_T *pstTable  
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/23
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_stat_5wlandevinterfinfo( SNMP_TABLE_T *pstTable )
{
    AP_STAT_5WLANDEVINTERFINFO_T  *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
	SNMP_INIT_SCALAR(oid_ap_stat_5wlandevinterfinfo,
		TABLE_ID_AP_STAT_5WLANDEVINTERFINFO,
		TABLE_NAME_AP_STAT_5WLANDEVINTERFINFO,
		NULL);
	
	SNMP_INIT_SCALAR_COLUMN(type->ulNum,  "Num", TYPE_LONG, MIB_R);
	
    return TRUE;
}
/*****************************************************************************
 Prototype    : init_snmp_ap_stat_5wlandevinterfinfo_table
 Description  : AP 5G WLAN设备干扰信息表注册函数
 Input        : SNMP_TABLE_T *pstTable   
                UINT32 ulMaxTableRecNum  
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/23
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_stat_5wlandevinterfinfo_table( SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum )
{
    AP_STAT_5WLANDEVINTERFINFO_ENTRY_T    *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_TABLE(oid_ap_stat_5wlandevinterfinfo_table,
                    TABLE_ID_AP_STAT_5WLANDEVINTERFINFOTABLE,
                    TABLE_NAME_AP_STAT_5WLANDEVINTERFINFOTABLE,
                    ulMaxTableRecNum,
                    NULL);

	SNMP_INIT_TABLE_COLUMN(type->ulIndex,   "Index",    TYPE_LONG,  MIB_R, TRUE);
	SNMP_INIT_TABLE_COLUMN(type->aucBssid,  "Bssid",    TYPE_MAC,   MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->acSsid,    "Ssid",     TYPE_CHAR,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->iRssi,     "Rssi",     TYPE_INT,   MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulChannel, "Channel",  TYPE_LONG,  MIB_R, FALSE);
	
    return TRUE;
}
/* BEGIN: Added by zoucaihong, 2013/5/10 */
/*****************************************************************************
 Prototype    : init_snmp_ap_stat_24rogueapinfo
 Description  : AP 2.4G非法AP数目节点注册函数
 Input        : SNMP_TABLE_T *pstTable  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/5/10
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_stat_24rogueapinfo( SNMP_TABLE_T *pstTable )
{
    AP_STAT_24ROGUEAPINFO_T  *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
	SNMP_INIT_SCALAR(oid_ap_stat_24rogueapinfo,
		TABLE_ID_AP_STAT_24ROGUEAPINFO,
		TABLE_NAME_AP_STAT_24ROGUEAPINFO,
		NULL);
	
	SNMP_INIT_SCALAR_COLUMN(type->ulNum,  "Num", TYPE_LONG, MIB_R);
	
    return TRUE;
}
/*****************************************************************************
 Prototype    : init_snmp_ap_stat_24rogueapinfo_table
 Description  : AP 2.4G非法AP信息表注册函数
 Input        : SNMP_TABLE_T *pstTable   
                UINT32 ulMaxTableRecNum  
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/5/10
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_stat_24rogueapinfo_table( SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum )
{
    AP_STAT_24ROGUEAPINFO_ENTRY_T    *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_TABLE(oid_ap_stat_24rogueapinfo_table,
                    TABLE_ID_AP_STAT_24ROGUEAPINFOTABLE,
                    TABLE_NAME_AP_STAT_24ROGUEAPINFOTABLE,
                    ulMaxTableRecNum,
                    NULL);

	SNMP_INIT_TABLE_COLUMN(type->ulIndex,   "Index",    TYPE_LONG,  MIB_R, TRUE);
	SNMP_INIT_TABLE_COLUMN(type->aucBssid,  "Bssid",    TYPE_MAC,   MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->acSsid,    "Ssid",     TYPE_CHAR,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->iRssi,     "Rssi",     TYPE_INT,   MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulChannel, "Channel",  TYPE_LONG,  MIB_R, FALSE);
	
    return TRUE;
}
/*****************************************************************************
 Prototype    : init_snmp_ap_stat_5rogueapfinfo
 Description  : AP 5G非法AP数目节点注册函数
 Input        : SNMP_TABLE_T *pstTable  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/5/10
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_stat_5rogueapinfo( SNMP_TABLE_T *pstTable )
{
    AP_STAT_5ROGUEAPINFO_T  *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
	SNMP_INIT_SCALAR(oid_ap_stat_5rogueapinfo,
		TABLE_ID_AP_STAT_5ROGUEAPINFO,
		TABLE_NAME_AP_STAT_5ROGUEAPINFO,
		NULL);
	
	SNMP_INIT_SCALAR_COLUMN(type->ulNum,  "Num", TYPE_LONG, MIB_R);
	
    return TRUE;
}
/*****************************************************************************
 Prototype    : init_snmp_ap_stat_5rogueapinfo_table
 Description  : AP 5G非法AP信息表注册函数
 Input        : SNMP_TABLE_T *pstTable   
                UINT32 ulMaxTableRecNum  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/5/10
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_stat_5rogueapinfo_table( SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum )
{
    AP_STAT_5ROGUEAPINFO_ENTRY_T    *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_TABLE(oid_ap_stat_5rogueapinfo_table,
                    TABLE_ID_AP_STAT_5ROGUEAPINFOTABLE,
                    TABLE_NAME_AP_STAT_5ROGUEAPINFOTABLE,
                    ulMaxTableRecNum,
                    NULL);

	SNMP_INIT_TABLE_COLUMN(type->ulIndex,   "Index",    TYPE_LONG,  MIB_R, TRUE);
	SNMP_INIT_TABLE_COLUMN(type->aucBssid,  "Bssid",    TYPE_MAC,   MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->acSsid,    "Ssid",     TYPE_CHAR,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->iRssi,     "Rssi",     TYPE_INT,   MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulChannel, "Channel",  TYPE_LONG,  MIB_R, FALSE);
	
    return TRUE;
}
/* END: Added by zoucaihong, 2013/5/10 */
/*****************************************************************************
 Prototype    : init_snmp_ap_stat_nonwlandevinterfinfo
 Description  : 非WLAN设备干扰数目节点注册
 Input        : SNMP_TABLE_T *pstTable  
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/24
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_stat_nonwlandevinterfinfo( SNMP_TABLE_T *pstTable )
{
    AP_STAT_NONWLANDEVINTERFINFO_T  *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
	SNMP_INIT_SCALAR(oid_ap_stat_nonwlandevinterfinfo,
		TABLE_ID_AP_STAT_NONWLANDEVINTERFINFO,
		TABLE_NAME_AP_STAT_NONWLANDEVINTERFINFO,
		NULL);
	
	SNMP_INIT_SCALAR_COLUMN(type->ulNum,  "Num", TYPE_LONG, MIB_R);
	
    return TRUE;
}
/*****************************************************************************
 Prototype    : init_snmp_ap_stat_nonwlandevinterfinto_table
 Description  : 非WLAN设备干扰信息表注册
 Input        : SNMP_TABLE_T *pstTable   
                UINT32 ulMaxTableRecNum  
 Output       : None
 Return Value : BOOL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/1/24
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
BOOL init_snmp_ap_stat_nonwlandevinterfinto_table( SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum )
{
    AP_STAT_NONWLANDEVINTERFINFO_ENTRY_T    *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_TABLE(oid_ap_stat_nonwlandevinterfinfo_table,
                    TABLE_ID_AP_STAT_NONWLANDEVINTERFINFOTABLE,
                    TABLE_NAME_AP_STAT_NONWLANDEVINTERFINFOTABLE,
                    ulMaxTableRecNum,
                    NULL);

	SNMP_INIT_TABLE_COLUMN(type->ulIndex,   "Index",    TYPE_LONG,  MIB_R, TRUE);
	SNMP_INIT_TABLE_COLUMN(type->ulWifiId,  "WifiId",   TYPE_LONG,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->iNoise,    "Noise",    TYPE_INT,   MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulChannel, "Channel",  TYPE_LONG,  MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->acTime,    "Time",     TYPE_CHAR,  MIB_R, FALSE);
	
    return TRUE;
}

BOOL init_snmp_ap_stat_brmodestation( SNMP_TABLE_T *pstTable )
{
    AP_STAT_BRMODESTA_T  *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
	SNMP_INIT_SCALAR(oid_ap_stat_brmodesta,
		TABLE_ID_AP_STAT_BRMODESTA,
		TABLE_NAME_AP_STAT_BRMODESTA,
		NULL);
	
	SNMP_INIT_SCALAR_COLUMN(type->ulStationNum,  "StationNum", TYPE_LONG, MIB_R);
	
    return TRUE;
}
BOOL init_snmp_ap_stat_brmodestation_table( SNMP_TABLE_T *pstTable, UINT32 ulMaxTableRecNum )    
{
    AP_STAT_BRMODESTA_ENTRY_T    *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
    SNMP_INIT_TABLE(oid_ap_stat_brmodesta_table,
                    TABLE_ID_AP_STAT_BRMODESTATABLE,
                    TABLE_NAME_AP_STAT_BRMODESTATABLE,
                    ulMaxTableRecNum,
                    NULL);

	SNMP_INIT_TABLE_COLUMN(type->ulStaID, "StaID", TYPE_LONG, MIB_R, TRUE);
	SNMP_INIT_TABLE_COLUMN(type->ulMode, "Mode", TYPE_LONG, MIB_R, FALSE);
    SNMP_INIT_COLUMN_ENUM("ponit-point(0), point-multiplepoint(1)")
	SNMP_INIT_TABLE_COLUMN(type->aucStaMac, "StaMac",  TYPE_MAC, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->iRssi, "Rssi", TYPE_INT, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulSnr,	"Snr", TYPE_LONG, MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->acTxRate, "TxRate", TYPE_CHAR, MIB_R, FALSE);
	
    return TRUE;
}
#endif

BOOL init_snmp_ap_oper_sysoper( SNMP_TABLE_T *pstTable )
{
    AP_OPER_SYSOPER_T   *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
	SNMP_INIT_SCALAR(oid_ap_oper_sysoper,
		TABLE_ID_AP_OPER_SYSOPER,
		TABLE_NAME_AP_OPER_SYSOPER,
		snmp_check_valid_ap_oper_sysoper);
	
	SNMP_INIT_SCALAR_COLUMN(type->ulSoftwareUpdate, "", TYPE_LONG, MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->ulSysRestart,     "", TYPE_LONG, MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->ulSysReset,       "", TYPE_LONG, MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->ulCfgFileImpExp,  "", TYPE_LONG, MIB_RW);
    /* BEGIN: Added by zoucaihong, 2014/7/15 */
	SNMP_INIT_SCALAR_COLUMN(type->ulDiagnose,       "", TYPE_LONG, MIB_RW);
    /* END: Added by zoucaihong, 2014/7/15 */
	SNMP_INIT_SCALAR_COLUMN(type->ulSysStopRecover, "", TYPE_LONG, MIB_RW);
    return TRUE;
}
BOOL init_snmp_ap_oper_filetransconfig( SNMP_TABLE_T *pstTable )
{
    AP_OPER_FILETRANS_T *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
	SNMP_INIT_SCALAR(oid_ap_oper_filetrans,
		TABLE_ID_AP_OPER_FILETRANS,
		TABLE_NAME_AP_OPER_FILETRANS,
		NULL);
	
	SNMP_INIT_SCALAR_COLUMN(type->ulLoadFlag,       "",     TYPE_LONG, MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->acFileName,       "",     TYPE_CHAR, MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->ulFileType,       "",     TYPE_LONG, MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->ulTransProto,     "",   TYPE_LONG, MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->aucServerIp,      "",     TYPE_IP,   MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->ulTransPort,      "",    TYPE_LONG, MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->aucTransUser,     "",    TYPE_CHAR, MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->aucTransPasswd,   "",  TYPE_CHAR, MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->ulTransStatus,    "",  TYPE_LONG, MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->acFailReason,     "",   TYPE_CHAR, MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->acFileVersion,    "",  TYPE_CHAR, MIB_RW);
    return TRUE;
}

/* BEGIN: Added by zoucaihong, 2015/3/24 */
/* 网关对AP的操作 */
BOOL init_snmp_ap_oper_gwap( SNMP_TABLE_T *pstTable )
{
    AP_OPER_GWAP_T  *type = NULL;

    if (NULL == pstTable)
    {
        return FALSE;
    }
    
	SNMP_INIT_SCALAR(oid_ap_oper_gwap,
		TABLE_ID_AP_OPER_GWAP,
		TABLE_NAME_AP_OPER_GWAP,
		snmp_check_valid_ap_oper_gwap);
	
	SNMP_INIT_SCALAR_COLUMN(type->ulAction,     "", TYPE_LONG, MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->acApMac,      "", TYPE_CHAR, MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->acFirstSsid,  "", TYPE_CHAR, MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->acSecondSsid, "", TYPE_CHAR, MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->ulTotalLink,  "", TYPE_LONG, MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->ulRoad,       "", TYPE_LONG, MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->acApMemo,     "", TYPE_CHAR, MIB_RW);
    return TRUE;
}
/* END: Added by zoucaihong, 2015/3/24 */

