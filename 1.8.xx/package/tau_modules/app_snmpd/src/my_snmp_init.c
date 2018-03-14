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

BOOL init_sys_dev_info_table(SNMP_TABLE_T *pstTable)
{
    SNMP_SYS_DEV_INFO_T     *type = NULL;
    SNMP_INIT_SCALAR(oid_lte_config_sys_devinfo,
                     TABLE_ID_SYS_DEV_INFO,
                     TABLE_NAME_SYS_DEV_INFO,
                     NULL);
	SNMP_INIT_SCALAR_COLUMN(type->acSysSn,         "sysSn",             TYPE_CHAR,  MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->acSysDevType,    "sysModel",          TYPE_CHAR,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->acSysTime,       "sysTime",           TYPE_CHAR,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->uiSysUptime,     "sysUptime",         TYPE_LONG,  MIB_R);

    return TRUE;
}

BOOL init_sys_sw_info_table(SNMP_TABLE_T *pstTable)
{
    SNMP_SYS_SW_INFO_T     *type = NULL;

    SNMP_INIT_SCALAR(oid_lte_sw_info,
                     TABLE_ID_SYS_SW_INFO,
                     TABLE_NAME_SYS_SW_INFO,
                     NULL);
    SNMP_INIT_SCALAR_COLUMN(type->acTargetVersion,     "targetVersion",    TYPE_CHAR,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->acSoftwareVersion,   "curVersion",       TYPE_CHAR,  MIB_R);
    
    return TRUE;
}

BOOL init_sys_hw_info_table(SNMP_TABLE_T *pstTable)
{
    SNMP_SYS_HW_INFO_T     *type = NULL;

    SNMP_INIT_SCALAR(oid_lte_hw_info,
                     TABLE_ID_SYS_HW_INFO,
                     TABLE_NAME_SYS_HW_INFO,
                     NULL);
    SNMP_INIT_SCALAR_COLUMN(type->aucCpuType,       "CpuType",          TYPE_CHAR,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->aucMemoryType,    "MemoryType",       TYPE_CHAR,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->ulMemorySize,     "MemorySize",       TYPE_LONG,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->ulFlashSize,      "FlashSize",        TYPE_LONG,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->aucCpeHwver,      "CpeHwver",         TYPE_CHAR,  MIB_R);
    
    return TRUE;
}

BOOL init_sys_manage_info_table(SNMP_TABLE_T *pstTable)
{
    SNMP_SYS_MANAGE_INFO_T     *type = NULL;

    SNMP_INIT_SCALAR(oid_lte_config_snmp,
                     TABLE_ID_SYS_MANAGE_INFO,
                     TABLE_NAME_SYS_MANAGE_INFO,
                     snmp_info_set);
    
	SNMP_INIT_SCALAR_COLUMN(type->acManageIp,       "manageIp",         TYPE_IP,    MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->acManageMac,      "manageMac",        TYPE_MAC,   MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->uiManageState,    "manageState",      TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->aucManageIpv6,    "manageIp6",        TYPE_CHAR,  MIB_R);
    
    return TRUE;
}

BOOL init_sys_proto_info_table(SNMP_TABLE_T *pstTable)
{
    SNMP_SYS_PROTO_INFO_T     *type = NULL;

    SNMP_INIT_SCALAR(oid_lte_proto_snmp,
                     TABLE_ID_SYS_PROTO_INFO,
                     TABLE_NAME_SYS_PROTO_INFO,
                     NULL);
    
	SNMP_INIT_SCALAR_COLUMN(type->uiIpv4Support,      "ipv4Support",        TYPE_LONG,    MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->uiIpv6Support,      "ipv6Support",        TYPE_LONG,   MIB_R);
    
    return TRUE;
}

BOOL init_device_heart_info_table(SNMP_TABLE_T *pstTable)
{
    SNMP_DEVICE_HEART_INFO_T     *type = NULL;

    SNMP_INIT_SCALAR(oid_lte_deviceHeart_snmp,
                     TABLE_ID_DEVICE_HEART_INFO,
                     TABLE_NAME_DEVICE_HEART_INFO,
                     device_heart_info_set);
    
    SNMP_INIT_SCALAR_COLUMN(type->uiHeartInterval,  "HeartInterval",   TYPE_LONG,       MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->uiTrapInterval,   "TrapInterval",    TYPE_LONG,       MIB_RW);
   
    return TRUE;
}

BOOL init_sys_debug_info_table(SNMP_TABLE_T *pstTable)
{
    SNMP_DEBUG_INFO_T     *type = NULL;

    SNMP_INIT_SCALAR(oid_lte_debug_info,
                     TABLE_ID_SYS_DEBUG_INFO,
                     TABLE_NAME_SYS_DEBUG_INFO,
                     sys_debug_info_set);
    
    SNMP_INIT_SCALAR_COLUMN(type->acDebugServerIp,      "debugSeverIp",         TYPE_IP,        MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->acDebugClientIp,      "debugClientIp",        TYPE_IP,        MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->auDebugServerUserName,"debugSeverUserName",   TYPE_CHAR,      MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->auDebugServerPasswd,  "debugSeverPasswd",     TYPE_CHAR,      MIB_RW);

    return TRUE;
}


BOOL init_lte_hw_table(SNMP_TABLE_T *pstTable)
{
    SNMP_LTE_HW_INFO_T     *type = NULL;

    SNMP_INIT_SCALAR(oid_lte_modulehw_info,
                     TABLE_ID_LTE_HW_INFO,
                     TABLE_NAME_LTE_HW_INFO,
                     NULL);
    
	SNMP_INIT_SCALAR_COLUMN(type->aucModuleType,    "lteModuleType",    TYPE_CHAR,  MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->aucModuleVer,     "lteModuleVer",     TYPE_CHAR,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->aucModuleIMEI,    "lteModuleIMEI",    TYPE_CHAR,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->uiSimState,       "simState",         TYPE_LONG,  MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->aucSimICCID,      "simICCID",         TYPE_CHAR,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->aucSimIMSI,       "simIMSI",          TYPE_CHAR,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->uiSimLock,        "simLock",          TYPE_LONG,  MIB_RW);
    
    return TRUE;
}

BOOL init_lte_net_table(SNMP_TABLE_T *pstTable)
{
    SNMP_LTE_NET_INFO_T     *type = NULL;

    SNMP_INIT_SCALAR(oid_lte_network_info,
                     TABLE_ID_LTE_NET_INFO,
                     TABLE_NAME_LTE_NET_INFO,
                     NULL);

    SNMP_INIT_SCALAR_COLUMN(type->aucOperCompany,   "operCompany",      TYPE_CHAR,  MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->uiLteDailWay,     "lteDailWay",       TYPE_LONG,  MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->uiLteDailState,   "lteDailState",     TYPE_LONG,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->uiLteSigRssi,     "lteSignalRssi",    TYPE_LONG,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->uiLteSigRsrp,     "lteSignalRsrp",    TYPE_LONG,  MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->uiLteSigRsrq,     "lteSignalRsrq",    TYPE_LONG,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->uiLteSigSinr,     "lteSignalSinr",    TYPE_LONG,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->uiLteSigLevel,    "lteSignalLevel",   TYPE_LONG,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->uiAccessNet,      "accessNet",        TYPE_LONG,  MIB_R);
    
    return TRUE;
}

BOOL init_lte_cfg_table(SNMP_TABLE_T *pstTable)
{
    SNMP_LTE_CFG_INFO_T     *type = NULL;

    SNMP_INIT_SCALAR(oid_lte_cfg_info,
                     TABLE_ID_SYS_LTE_CFG_INFO,
                     TABLE_NAME_LTE_CFG_INFO,
                     snmp_lte_cfg_info_set);
    
	SNMP_INIT_SCALAR_COLUMN(type->aucApnSelect,         "lteApnSelect",         TYPE_CHAR,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->uiAccessMode,         "lteAccessMode",        TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->uiPriAccessMode,      "ltePriAccessMode",     TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->aucLteBandList,       "lteBandList",          TYPE_CHAR,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->aucLteBand,           "lteBand",              TYPE_CHAR,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->uiLteChannel,         "lteChannel",           TYPE_LONG,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->aucLteBandLocked,     "lteBandLocked",        TYPE_CHAR,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->aucLteCID,            "lteCID",               TYPE_CHAR,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->aucLtePCID,           "ltePCID",              TYPE_CHAR,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->aucLtePCIDLocked,     "ltePCIDLocked",        TYPE_CHAR,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->aucLteChannelLocked,  "lteChannelLocked",     TYPE_CHAR,  MIB_RW);
    
    return TRUE;
}

BOOL init_wifi_info_table(SNMP_TABLE_T *pstTable)
{
    SNMP_WIFI_INFO_T     *type = NULL;

    SNMP_INIT_SCALAR(oid_lte_wifi_info,
                     TABLE_ID_SYS_LTE_WIFI_INFO,
                     TABLE_NAME_LTE_WIFI_INFO,
                     NULL);
    
    SNMP_INIT_SCALAR_COLUMN(type->ulCpeWifiMode,        "Mode",         TYPE_LONG,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->ulCpeChannel,         "Channel",      TYPE_LONG,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->ulClientChannelWidth, "BandWidth",    TYPE_LONG,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->ulTransPower,         "TxPower",      TYPE_LONG,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->iSignalLevel,         "SignalLevel",  TYPE_INT,   MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->iNoiseLevel,          "NoiseLevel",   TYPE_INT,   MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->ulWifiSupport,        "wifisupport",  TYPE_LONG,  MIB_R);
    
    return TRUE;
}

BOOL init_wan_state_info_table(SNMP_TABLE_T *pstTable)
{
    SNMP_WAN_STATE_INFO_T *type = NULL;
    SNMP_INIT_SCALAR(oid_lte_wan_state,
                     TABLE_ID_WAN_STAT,
                     TABLE_NAME_WAN_STAT,
                     NULL);
    SNMP_INIT_SCALAR_COLUMN(type->ulWanMode,    "wanMode",          TYPE_LONG,      MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ulRxBytes,    "recvBytes",        TYPE_LONG,      MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ulRxGB,       "recvGBytes",       TYPE_LONG,      MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ulTxBytes,    "sendBytes",        TYPE_LONG,      MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->ulTxGB,       "sendGbytes",       TYPE_LONG,      MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->aucWanName,   "wanName",          TYPE_CHAR,      MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->aucIp,        "wanIpAddr",        TYPE_IP,        MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->ulIpMask,     "wanIpMask",        TYPE_LONG,      MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->aucIpv6,      "wanIp6Addr",       TYPE_CHAR,      MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->ulIp6Mask,    "wanIp6Mask",       TYPE_LONG,      MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->aucMac,       "wanMac",           TYPE_MAC,       MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->aucRxAllBytes,"wanAllRx",         TYPE_CHAR,      MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->aucTxAllBytes,"wanAllTx",         TYPE_CHAR,      MIB_R);

    return TRUE;
}

BOOL init_wan_user_num(SNMP_TABLE_T *pstTable)
{
    SNMP_WAN_USER_NUM_T *type = NULL;

    SNMP_INIT_SCALAR(oid_lte_user_num,
                     TABLE_ID_USER_NUM,
                     TABLE_NAME_USER_NUM,
                     NULL);
    SNMP_INIT_SCALAR_COLUMN(type->uiUserNum,      "userNum",            TYPE_LONG,  MIB_R);
    
    return TRUE;
}

BOOL init_wan_user_info_table(SNMP_TABLE_T *pstTable, UINT32 ulMaxTableNum)
{
    SNMP_WAN_USER_INFO_T *type = NULL;
    
    SNMP_INIT_TABLE(oid_lte_user_table,
                     TABLE_ID_USER_INFO,
                     TABLE_NAME_USER_TABLE,
                     ulMaxTableNum,
                     NULL);
    
    SNMP_INIT_TABLE_COLUMN(type->uiUserId,          "userID",           TYPE_LONG,      MIB_R, TRUE);
	SNMP_INIT_TABLE_COLUMN(type->aucUserMac,        "userMac",          TYPE_MAC,       MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->uiUserType,        "userType",         TYPE_LONG,      MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulRxBytes,         "userTxBytes",      TYPE_LONG,      MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulRxGB,            "userTxGBytes",     TYPE_LONG,      MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulTxBytes,         "userRxBytes",      TYPE_LONG,      MIB_R, FALSE);
	SNMP_INIT_TABLE_COLUMN(type->ulTxGB,            "userRxGBytes",     TYPE_LONG,      MIB_R, FALSE);
    SNMP_INIT_TABLE_COLUMN(type->aucUserIp,         "userIpAddr",       TYPE_IP,        MIB_R, FALSE);
    SNMP_INIT_TABLE_COLUMN(type->aucUserIpv6,       "userIp6Addr",      TYPE_CHAR,      MIB_R, FALSE);

    return TRUE;
}

BOOL init_log_cfg_table(SNMP_TABLE_T *pstTable)
{
    SNMP_LOG_CFG_T *type = NULL;
    SNMP_INIT_SCALAR(oid_lte_logcfg_info,
                 TABLE_ID_LOG_CFG,
                 TABLE_NAME_LOG_CFG,
                 snmp_log_cfg_set);
    
    SNMP_INIT_SCALAR_COLUMN(type->uiCfgState,          "logCfgState",         TYPE_LONG,  MIB_RW);

    return TRUE;

}

BOOL init_keepalive_cfg_table(SNMP_TABLE_T *pstTable)
{
    SNMP_LTEKP_CFG_T *type = NULL;
    SNMP_INIT_SCALAR(oid_lte_keepalive_info,
                 TABLE_ID_LTEKP_CFG,
                 TABLE_NAME_LTEKP_CFG,
                 snmp_keepalive_cfg_set);
    SNMP_INIT_SCALAR_COLUMN(type->uiNum,          "lteKeepaliveNum",         TYPE_LONG,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->uiSwitch,       "lteKeepaliveSw",          TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->uiInterval,     "lteKeepaliveInterval",    TYPE_LONG,  MIB_RW);

    return TRUE;

}

BOOL init_keepalive_info_table(SNMP_TABLE_T *pstTable, UINT32 ulMaxTableNum)
{
    SNMP_LTEKP_INFO_T *type = NULL;

    SNMP_INIT_TABLE(oid_lte_keepalive_table,
                     TABLE_ID_LTEKP_INFO,
                     TABLE_NAME_LTEKP_INFO,
                     ulMaxTableNum,
                     snmp_keepalive_info_set);
    
    SNMP_INIT_TABLE_COLUMN(type->uiId,          "lteKeepaliveId",          TYPE_LONG,  MIB_RW, TRUE);
    SNMP_INIT_TABLE_COLUMN(type->aucIp,         "lteKeepaliveIp",          TYPE_IP,    MIB_RW, FALSE);
    SNMP_INIT_TABLE_COLUMN(type->aucIpv6,       "lteKeepaliveIpv6",        TYPE_CHAR,  MIB_RW, FALSE);
    SNMP_INIT_TABLE_COLUMN(type->uiCfgState,    "lteKeepaliveCfgState",    TYPE_LONG,  MIB_RW, FALSE);

    return TRUE;
}



BOOL init_device_oper_table(SNMP_TABLE_T *pstTable)
{
    SNMP_OPER_INFO_T     *type = NULL;

    SNMP_INIT_SCALAR(oid_lte_sys_oper,
                     TABLE_ID_LTE_SYS_OPER,
                     TABLE_NAME_LTE_SYS_OPER,
                     snmp_device_oper_set);

    SNMP_INIT_SCALAR_COLUMN(type->ulCpeUpdateState, "softwareUpdate",       TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulReboot,         "sysReboot",            TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulReset,          "sysReset",             TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulDebug,          "sysDebug",             TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulScript,         "sysScript",            TYPE_LONG,  MIB_RW);

    return TRUE;
}

BOOL init_file_trans_table(SNMP_TABLE_T *pstTable)
{
    SNMP_FILE_TRANS_INFO_T     *type = NULL;

    SNMP_INIT_SCALAR(oid_lte_file_trans,
                     TABLE_ID_LTE_FILE_TRANS,
                     TABLE_NAME_LTE_FILE_TRANS,
                     snmp_file_trans_set);
    
    SNMP_INIT_SCALAR_COLUMN(type->ulLoadFlag,       "LoadFlag",         TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->aucImageDir,      "ImageDir",         TYPE_CHAR,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulFileType,       "FileType",         TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulTransProto,     "TransProto",       TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->aucServerIp,      "ServerIp",         TYPE_IP,    MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulTransPort,      "TransPort",        TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->aucTransUser,     "TransUser",        TYPE_CHAR,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->aucTransPasswd,   "TransPasswd",      TYPE_CHAR,  MIB_RW);
	SNMP_INIT_SCALAR_COLUMN(type->ulTransStatus,    "TransStatus",      TYPE_LONG,  MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->acFailReason,     "FailReason",       TYPE_CHAR,  MIB_R);
	SNMP_INIT_SCALAR_COLUMN(type->acFileVersion,    "FileVersion",      TYPE_CHAR,  MIB_RW);

    return TRUE;
}

BOOL init_pppoe_user_num(SNMP_TABLE_T *pstTable)
{
    SNMP_PPPOE_USER_NUM_T *type = NULL;

    SNMP_INIT_SCALAR(oid_lte_pppoe_user_um,
                     TABLE_ID_PPPOE_USER_NUM,
                     TABLE_NAME_PPPOE_USER_NUM,
                     NULL);
    SNMP_INIT_SCALAR_COLUMN(type->uiUserNum,      "pppoeUserNum",            TYPE_LONG,  MIB_R);
    
    return TRUE;
}

BOOL init_pppoe_user_info_table(SNMP_TABLE_T *pstTable, UINT32 ulMaxTableNum)
{
    SNMP_PPPOE_USER_INFO_T *type = NULL;
    
    SNMP_INIT_TABLE(oid_lte_pppoe_table,
                     TABLE_ID_PPPOE_USER_INFO,
                     TABLE_NAME_PPPOE_USER_TABLE,
                     ulMaxTableNum,
                     NULL);
    
    SNMP_INIT_TABLE_COLUMN(type->uiUserId,          "pppoeUserID",           TYPE_LONG,      MIB_R, TRUE);
    SNMP_INIT_TABLE_COLUMN(type->aucPppoeIp,        "pppoeIpAddr",           TYPE_IP,        MIB_R, FALSE);
    SNMP_INIT_TABLE_COLUMN(type->aucPppoeIpv6,      "pppoeIp6Addr",          TYPE_CHAR,      MIB_R, FALSE);
    SNMP_INIT_TABLE_COLUMN(type->pppoeServerName,   "pppoeServerName",       TYPE_CHAR,      MIB_R, FALSE);
    SNMP_INIT_TABLE_COLUMN(type->pppoeUserName,     "pppoeUserName",         TYPE_CHAR,      MIB_R, FALSE);
    SNMP_INIT_TABLE_COLUMN(type->pppoePassword,     "pppoePassword",         TYPE_CHAR,      MIB_R, FALSE);

    return TRUE;
}

BOOL init_pppoe_all_user_names(SNMP_TABLE_T *pstTable)
{
    SNMP_PPPOE_ALL_USER_NAMES_T *type = NULL;

    SNMP_INIT_SCALAR(oid_lte_pppoe_all_user_names,
                     TABLE_ID_PPPOE_ALL_USER_NAMES,
                     TABLE_NAME_PPPOE_ALL_USER_NAMES,
                     NULL);
    SNMP_INIT_SCALAR_COLUMN(type->uiAllUserNames,     "pppoeAllUserNames",   TYPE_CHAR,  MIB_R);
    
    return TRUE;
}

BOOL init_lte_enodeb_info(SNMP_TABLE_T *pstTable)
{
    SNMP_LTE_ENODEB_INFO_T *type = NULL;

    SNMP_INIT_SCALAR(oid_lte_enodeb_info,
                     TABLE_ID_LTE_ENODB_INFO,
                     TABLE_NAME_LTE_ENODEB_INFO,
                     NULL);
    SNMP_INIT_SCALAR_COLUMN(type->acLteENodeBId,      "lteENodeBId",        TYPE_CHAR,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->acLteLCID,          "lteLCID",            TYPE_CHAR,  MIB_R);
    SNMP_INIT_SCALAR_COLUMN(type->acLteTac,           "lteTac",             TYPE_CHAR,  MIB_R);
    
    return TRUE;
}

BOOL init_lte_station_lock(SNMP_TABLE_T *pstTable)
{
    SNMP_LTE_STATION_LOCK_T *type = NULL;

    SNMP_INIT_SCALAR(oid_lte_station_lock,
                     TABLE_ID_LTE_STATION_LOCK,
                     TABLE_NAME_LTE_STATION_LOCK,
                     snmp_lte_station_lock_set);
    SNMP_INIT_SCALAR_COLUMN(type->ulLteLockType,      "lteLockType",           TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulLteAutoLock,      "lteAutoLock",           TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulLteAutoMax,       "lteAutoMax",            TYPE_LONG,  MIB_RW);
    SNMP_INIT_SCALAR_COLUMN(type->ulLteLockNum,       "lteLockNum",            TYPE_LONG,  MIB_R);
    
    return TRUE;
}

BOOL init_lte_station_lock_table(SNMP_TABLE_T *pstTable, UINT32 ulMaxTableNum)
{
    SNMP_LTE_STATION_LOCK_TABLE_T *type = NULL;
    
    SNMP_INIT_TABLE(oid_lte_station_lock_table,
                     TABLE_ID_LTE_STATION_TABLE_LOCK,
                     TABLE_NAME_LTE_STATION_LOCK_TABLE,
                     ulMaxTableNum,
                     snmp_lte_station_lock_table_set);

    SNMP_INIT_TABLE_COLUMN(type->ulLteLockNumID,       "lteLockNumID",        TYPE_LONG,   MIB_R,  TRUE);
    SNMP_INIT_TABLE_COLUMN(type->ulLteLockID,          "lteLockID",           TYPE_CHAR,   MIB_RW, FALSE);
    SNMP_INIT_TABLE_COLUMN(type->ulLteLockCfgState,    "lteLockCfgState",     TYPE_LONG,   MIB_RW, FALSE);

    return TRUE;
}

BOOL init_lte_station_all_lock_id(SNMP_TABLE_T *pstTable)
{
    SNMP_LTE_STATION_ALL_LOCK_ID_T *type = NULL;

    SNMP_INIT_SCALAR(oid_lte_station_all_lock_id,
                     TABLE_ID_LTE_STATION_ALL_LOCK_ID,
                     TABLE_NAME_LTE_STATION_ALL_LOCK_ID,
                     NULL);
    SNMP_INIT_SCALAR_COLUMN(type->acLteLockAllID,      "lteStationAllLockID",   TYPE_CHAR,  MIB_R);
    
    return TRUE;
}