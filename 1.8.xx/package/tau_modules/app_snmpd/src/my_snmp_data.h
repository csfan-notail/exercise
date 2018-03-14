/******************************************************************************
* COPYRIGHT GBCOM
*******************************************************************************
* 功    能: 
* 修改历史: 
* 2012-9-10     冯静              新建
******************************************************************************/

/******************************* 宏定义 ********************************/
#ifndef __MINI_SNMPD_DATA__
#define __MINI_SNMPD_DATA__

#include "ap_types.h"

#define SNMPAGENT_CONTROL_NAME "snmpagentd"
#define AP_AGENT_TRAP_PORT       162
#define AP_AGENT_TRAP_TIMEOUT    5

// 增加缺省读写共同体定义
#define AGENT_RO_COMMUNITY    "public"
#define AGENT_RW_COMMUNITY    "private"


/****************************snmp读写共同体配置文件***************************/
#define SNMPAGENT_FILE_CONF                "/tmp/snmp_agent.conf"


/******************************* 常量定义 ********************************/
#define SNMP_TIMEOUT  5*60 /* 5分钟 */
#define AGENT_REFRESH_PERIOD_DFTVAL  ((UINT32)10)


/**
RFC1212中建议：如果允许增加或删除MIB表结构的行，列对象中应该包含一个
名为“xType”或“xStatus”对象，它的语法类型应该是枚举型的整数，
这个列对象称为状态列对象。
管理工作站可以通过设置状态列对象的值来控制表中行的创建与删除操作。

状态栏列对象有6个定义值：
-active（1），表明状态行是可用的
-notInService（2），表明行存在但不可用
-notReady（3），表明存在，但因为缺少必要的信息而不能用
-createAndGo（4），有管理者设置，表明希望创建一个概念行并设置该行的状态列对象为active
-createAndWait（5），有管理者设置，表明希望创建一个概念行，但不可用
-destroy（6），删除行

An existing conceptual row can be in any one of the three states, 'notReady', 'notInService', or 'active'. 
If the manager wishes to add a row in a single shot with values for all the columns, the status column should be given as 'createAndGo(4)'. 
After the creation of a row, its value is set to active(1). 
If a row has to be created with values for only some columns, the status column should be 'createAndWait(5)'. 
Also, this row with partially filled columns has the status 'notReady(3)'. 
The entry remains in this state until the values for all the columns are set. 
After all the values are set, the agent changes this value to active(1).

For SMIv2 tables, a new row can be added in any of the following ways:
CreateAndWait
CreateAndGo using Multiple-Variable Set

CreateAndWait
In this method, you should SET the RowStatus with value "CreateAndWait" and SET each column one by one. 
Finally, SET the RowStatus to "Active". 
Each SET method can be performed either using MibBrowser or through command line application. 

CreateAndGo Using Multiple-Variable Set
Instead of using multiple SET methods to create a new row, you can use Multiple-variable SET method to create a new row in a single SET method. 
For this, all the columns with their value should be given and rowStatus should be CreateAndGo.
Multiple-variable SET can be done using the command line application. 
Set all the column OIDs with the instance value (index for the row) using the method setObjectIDList() in SnmpTarget. 
Set the values for all the columns using snmpSetList() method. Rows can be deleted from a table by setting the value for the RowStatus column.

 */
#define ENUM_ROWSTATUS_ACTIVE          1
#define ENUM_ROWSTATUS_NOTINSERVICE    2
#define ENUM_ROWSTATUS_NOTREADY        3
#define ENUM_ROWSTATUS_CREATEANDGO     4
#define ENUM_ROWSTATUS_CREATEANDWAIT   5
#define ENUM_ROWSTATUS_DESTROY         6

/******************************* 表容量定义 ***********************************/
#define TABLE_MAXRECNUM_LTE_USER        ((UINT32)128)
#define TABLE_MAXRECNUM_LTE_ACCOUNT     ((UINT32)4)
#define TABLE_MAXRECNUM_LTE_LAC         ((UINT32)32)
#define TABLE_MAXRECNUM_LTE_KEEPALIVE   ((UINT32)16)
#define TABLE_MAXRECNUM_LTE_LOCK_TABLE   ((UINT32)64)

/******************************* 表名称定义************************************/
/*配置表*/
/* Sys Info */
#define TABLE_NAME_SYS_MANAGE_INFO              "manageInfo"
#define TABLE_NAME_DEVICE_HEART_INFO            "deviceHeartConfInfo"
#define TABLE_NAME_SYS_DEBUG_INFO               "debugInfo"

/* Lte Info */
#define TABLE_NAME_LTE_CFG_INFO                 "lteCfgInfo"

/*deviceStat*/
#define  TABLE_NAME_ACCOUNT_NUM                 "accountNum"
#define  TABLE_NAME_ACCOUNT_TABLE               "accountTable"
#define  TABLE_NAME_NTP_INFO                    "ntpInfo"
#define  TABLE_NAME_LOCAL_LOG                   "localLog"
#define  TABLE_NAME_REMOTE_LOG                  "remoteLog"
#define  TABLE_NAME_LOG_CFG                     "logConfig"
#define  TABLE_NAME_LAC_NUM                     "lacNum"
#define  TABLE_NAME_LAC_INFO                    "lacInfo"
#define  TABLE_NAME_LTEKP_CFG                   "lteKeepaliveCfg"
#define  TABLE_NAME_LTEKP_INFO                  "lteKeepaliveCfg"

/*deviceOper*/
#define TABLE_NAME_LTE_SYS_OPER                 "sysOper"
#define TABLE_NAME_LTE_FILE_TRANS               "fileTransConfig"

/*============================================================================*/
/*统计表*/
/* Sys Info */
#define TABLE_NAME_SYS_DEV_INFO                 "devInfo"
#define TABLE_NAME_SYS_SW_INFO                  "swInfo"
#define TABLE_NAME_SYS_HW_INFO                  "hwInfo"
#define TABLE_NAME_SYS_PROTO_INFO               "protoInfo"

/* Lte Info */
#define TABLE_NAME_LTE_HW_INFO                  "lteHwInfo"
#define TABLE_NAME_LTE_NET_INFO                 "lteNetInfo"

#define  TABLE_NAME_LTE_ENODEB_INFO                    "lteENodeBInfo"
#define  TABLE_NAME_LTE_STATION_LOCK                   "lteStationLock"
#define  TABLE_NAME_LTE_STATION_LOCK_TABLE             "lteStationLockTable"
#define  TABLE_NAME_LTE_STATION_ALL_LOCK_ID            "lteStationAllLockID"


/* Wifi Info */
#define TABLE_NAME_LTE_WIFI_INFO                "wifiInfo"

/*location Info*/
#define TABLE_NAME_SNMP_GPS_INFO                "gpsInfo"

/*deviceStat*/
#define  TABLE_NAME_WAN_STAT                    "wanStat"
#define  TABLE_NAME_USER_NUM                    "userNum"
#define  TABLE_NAME_USER_TABLE                  "userInfoTable"
#define  TABLE_NAME_LAN_STAT                    "lanStat"

#define  TABLE_NAME_PPPOE_USER_NUM                    "pppoeUserNum"
#define  TABLE_NAME_PPPOE_USER_TABLE                  "pppoeUserInfoTable"
#define  TABLE_NAME_PPPOE_ALL_USER_NAMES              "pppoeAllUserNames"



/********************************** 结构定义 **********************************/
typedef BOOL  (*AGENTMETHOD)(VOID);

typedef struct
{
	CHAR acTblName[32];
	AGENTMETHOD md;
	UINT32 ulInitFlag;
}AGENT_TBLMD_MAP_T;

/***************************** 私有mib结构体定义 ******************************/
/******************************* 配置结构体定义 *******************************/
#ifndef CHAR_LEN_32
#define CHAR_LEN_32   	    32    //通用字符串长度
#endif

#ifndef CHAR_LEN_64
#define CHAR_LEN_64         64
#endif

#ifndef CHAR_LEN_128
#define CHAR_LEN_128        128
#endif

#ifndef ETH_ALEN
#define ETH_ALEN	6		/* Octets in one ethernet addr	 */
#endif 

#ifndef IP_ALEN
#define IP_ALEN		4	   /* Octets in one ip addr	 */
#endif 

#ifndef IPV6_ALEN
#define IPV6_ALEN		16	   /* Octets in one ipv6 addr	 */
#endif 

typedef struct{
    CHAR        acSysSn[CHAR_LEN_32];               /*CPE的序列号*/
    CHAR        acSysDevType[CHAR_LEN_32];          /*CPE的设备类型*/
    CHAR        acSysTime[CHAR_LEN_32];             /* CPE的系统时间 */
    UINT32      uiSysUptime;           /*设备上电时间*/
    
}SNMP_SYS_DEV_INFO_T;

typedef struct{
    CHAR        acSoftwareVersion[CHAR_LEN_32];     /*目标版本号*/
    CHAR        acTargetVersion[CHAR_LEN_32];       /*当前版本号*/
    
}SNMP_SYS_SW_INFO_T;

typedef struct{
    CHAR        aucCpuType[CHAR_LEN_32];            /*CPU类型*/
    CHAR        aucMemoryType[CHAR_LEN_32];         /*MEM类型*/
    UINT32      ulMemorySize;                       /*MEM大小*/
    UINT32      ulFlashSize;                        /*闪存大小*/
    CHAR        aucCpeHwver[CHAR_LEN_32];           /*CPE硬件版本号*/
    
}SNMP_SYS_HW_INFO_T;

typedef struct{
    UINT8       acManageIp[IP_ALEN];                /* 管理口IP地址 */
    UINT8       acManageMac[ETH_ALEN];              /* 管理口MAC地址 */
    UINT32      uiManageState;                      /* 管理状态：1:discover 2:update image 3:running 4:quit*/
    CHAR        aucManageIpv6[CHAR_LEN_64];                /* 管理口IPV6地址 */
}SNMP_SYS_MANAGE_INFO_T;

typedef struct{
    UINT32      uiIpv4Support;
    UINT32      uiIpv6Support;
}SNMP_SYS_PROTO_INFO_T;

typedef struct{
    UINT32      uiHeartInterval;                    /* device heart interval */
    UINT32      uiTrapInterval;                     /* offline send trap interval */
}SNMP_DEVICE_HEART_INFO_T;

typedef struct{
    UINT8       acDebugServerIp[IP_ALEN];            /* l2tp server ip */
    UINT8       acDebugClientIp[IP_ALEN];            /* l2tp client ip */
    CHAR        auDebugServerUserName[CHAR_LEN_32];  /* l2tp server user name */
    CHAR        auDebugServerPasswd[CHAR_LEN_32];    /* l2tp server passwd */
}SNMP_DEBUG_INFO_T;

typedef struct 
{
    CHAR    aucModuleType[CHAR_LEN_32];
    CHAR    aucModuleVer[CHAR_LEN_128];
    CHAR    aucModuleIMEI[CHAR_LEN_32];
    UINT32  uiSimState;
    CHAR    aucSimICCID[CHAR_LEN_32];
    CHAR   aucSimIMSI[CHAR_LEN_32];
    UINT32  uiSimLock;
}SNMP_LTE_HW_INFO_T;

typedef struct 
{
    UINT8   aucOperCompany[CHAR_LEN_32];
    UINT32  uiLteDailWay;
    UINT32  uiLteDailState;
    UINT32  uiLteSigRssi;
    UINT32  uiLteSigRsrp;
    UINT32  uiLteSigRsrq;
    UINT32  uiLteSigSinr;
    UINT32  uiLteSigLevel;
    UINT32  uiAccessNet;
}SNMP_LTE_NET_INFO_T;

typedef struct 
{
    CHAR    aucApnSelect[CHAR_LEN_32];
    UINT32  uiAccessMode;
    UINT32  uiPriAccessMode;
    UINT8   aucLteBandList[CHAR_LEN_32];
    CHAR    aucLteBand[CHAR_LEN_32];
    UINT32  uiLteChannel;
    UINT8   aucLteBandLocked[CHAR_LEN_32];
    CHAR    aucLteCID[CHAR_LEN_32];
    CHAR    aucLtePCID[CHAR_LEN_32];
    CHAR    aucLtePCIDLocked[CHAR_LEN_32];
    CHAR    aucLteChannelLocked[CHAR_LEN_32];
}SNMP_LTE_CFG_INFO_T;



typedef struct{
    UINT32      ulCpeWifiMode;                      /*CPE无线模式 0：无效 1:11a 2:11na 3:11b 4:11g 5:11ng*/
    UINT32      ulCpeChannel;                       /*CPE所在信道*/
    UINT32      ulClientChannelWidth;               /* CPE无线频宽 0-20MHz;1-40MHz*/
    UINT32      ulTransPower;                       /* 射频功率dBm */
    UINT32      ulClientWirelessMode;               /* CPE的无线模式,0:客户端   1:中继 */   
    INT32       iSignalLevel;                       /*CPE接收信号强度*/  
    INT32       iNoiseLevel;                        /*CPE接收信号噪声*/
    UINT32      ulWifiSupport;
}SNMP_WIFI_INFO_T;


typedef struct{
    UINT8       aucLongitude[CHAR_LEN_32];
    UINT8       aucLatitude[CHAR_LEN_32];
    UINT8       aucElevation[CHAR_LEN_32];
    UINT8       aucVelocity[CHAR_LEN_32];
    UINT8       aucOrientation[CHAR_LEN_32];
    UINT8       aucUptime[CHAR_LEN_32];
}SNMP_GPS_INFO_T;


typedef struct{
    UINT32      ulWanMode;                          /* wan口模式 */
    UINT32      ulRxBytes;                          /* 累计收包字节数 */
    UINT32      ulRxGB;  
    UINT32      ulTxBytes;                          /* 累计发包字节数 */
    UINT32      ulTxGB;          
    CHAR        aucWanName[CHAR_LEN_32];
    CHAR        aucIp[IP_ALEN];                     /* wan IP */
    UINT32      ulIpMask;
    CHAR        aucIpv6[CHAR_LEN_64];                     /* wan IP6 */
    UINT32      ulIp6Mask;
    CHAR        aucMac[ETH_ALEN];                   /* wan MAC */
    CHAR        aucRxAllBytes[24];
    CHAR        aucTxAllBytes[24];
}SNMP_WAN_STATE_INFO_T;

typedef struct{
    UINT32      uiUserNum;
}SNMP_WAN_USER_NUM_T;

typedef struct{
    UINT32      uiUserId;                           /* user id */
    CHAR        aucUserMac[ETH_ALEN];               /* user mac */    
    UINT32      uiUserType;                         /* user type */
    UINT32      ulRxBytes;                          /* rx bytes */
    UINT32      ulRxGB;  
    UINT32      ulTxBytes;                          /* tx bytes */
    UINT32      ulTxGB;                     
    CHAR        aucUserIp[IP_ALEN];                 /* user ip */
    CHAR        aucUserIpv6[CHAR_LEN_64];              /* user ip */ 
}SNMP_WAN_USER_INFO_T;

typedef struct{
    UINT8       aucLanName[CHAR_LEN_32];
    CHAR        aucIp[IP_ALEN];                     /* lan IP */
    UINT32      ulIpMask;
    CHAR        aucIp6[IPV6_ALEN];                     /* lan IP6 */
    UINT32      ulIp6Mask;
    UINT32      ulRxBytes;                          /* 累计收包字节数 */
    UINT32      ulRxGB;  
    UINT32      ulTxBytes;                          /* 累计发包字节数 */
    UINT32      ulTxGB;
    UINT32      ulUserIsolate;
}SNMP_LAN_STATE_INFO_T;

typedef struct{
    UINT32      uiAccountNum;
}SNMP_ACCOUNT_NUM_T;

typedef struct{
    UINT32      uiId;
    CHAR        aucName[CHAR_LEN_32];
    CHAR        aucOldPwd[CHAR_LEN_32];
    CHAR        aucNewPwd[CHAR_LEN_32];
    UINT32      uiType;
    UINT32      uiCfgState;
}SNMP_ACCOUNT_INFO_T;

typedef struct{
    UINT32      uiMode;
    CHAR        aucServer[CHAR_LEN_64];
    UINT32      uiInterval;
    CHAR        aucDate[CHAR_LEN_32];
    CHAR        aucTime[CHAR_LEN_32];
}SNMP_NTP_INFO_T;

typedef struct{
    UINT32      uiSwitch;
    UINT32      uiInterval;
}SNMP_LOCAL_LOG_T;

typedef struct{
    UINT32      uiSwitch;
    CHAR        aucServerIp[IP_ALEN];
    UINT32      uiServerPort;
}SNMP_REMOTE_LOG_T;

typedef struct{
    UINT32      uiCfgState;
}SNMP_LOG_CFG_T;


typedef struct{
    UINT32      uiLacNum;
}SNMP_LAC_NUM_T;

typedef struct{
    UINT32      uiId;
    UINT32      uiMode;
    CHAR        aucIp[IP_ALEN];
    CHAR        aucIp6[IPV6_ALEN]; 
    CHAR        aucName[CHAR_LEN_32];
    UINT32      uiPwdSwitch;
    CHAR        aucPwd[CHAR_LEN_32];
    UINT32      uiCfgState;
}SNMP_LAC_INFO_T;

typedef struct{
    UINT32      uiNum;
    UINT32      uiSwitch;
    UINT32      uiInterval;
}SNMP_LTEKP_CFG_T;

typedef struct{
    UINT32      uiId;
    CHAR        aucIp[IP_ALEN];
    CHAR        aucIpv6[CHAR_LEN_64]; 
    UINT32      uiCfgState;
}SNMP_LTEKP_INFO_T;

typedef struct{
    UINT32      ulCpeUpdateState;                   /*CPE升级状态: 1:Success 2:Receive Error 3:File Wrong 4:Flash Error */
    UINT32      ulReboot;                           /*重启0-不重启，1-重启*/
    UINT32      ulReset;                            /*恢复出厂0-不恢复，1-恢复*/
    UINT32      ulDebug;                            /* 1:on 2:off */  
    UINT32      ulScript;                           /* 1:upload 2:download*/
}SNMP_OPER_INFO_T;

typedef struct{
    UINT32      ulLoadFlag;                         /*1-download,2-upload*/
    CHAR        aucImageDir[CHAR_LEN_32];           /*版本路径*/
    UINT32      ulFileType;                         /*文件类型software(1),config(2),log(3),cert(4),perf(5)*/
    UINT32      ulTransProto;                       /*传输协议类型*/
    UINT8       aucServerIp[IP_ALEN];               /*远程文件所在地址*/
    UINT32      ulTransPort;                        /*传输的端口号*/
    CHAR        aucTransUser[CHAR_LEN_32];          /*用户名*/
    CHAR        aucTransPasswd[CHAR_LEN_32];        /*密码*/
    UINT32      ulTransStatus;                      /*传输状态 0:ok, 1:param invalid, 2:download fail, 3:updatefail */
    CHAR        acFailReason[128]; 
	CHAR        acFileVersion[32];                  /* 软件版本号 */
}SNMP_FILE_TRANS_INFO_T;

typedef struct{
    UINT32      uiUserNum;
}SNMP_PPPOE_USER_NUM_T;

typedef struct{
    UINT32      uiUserId;                           /* user id */
    CHAR        aucPppoeIp[IP_ALEN];                 /* user ip */
    CHAR        aucPppoeIpv6[CHAR_LEN_64];              /* user ip */ 
    CHAR        pppoeServerName[128];
    CHAR        pppoeUserName[128];
    CHAR        pppoePassword[128];
}SNMP_PPPOE_USER_INFO_T;

typedef struct{
    CHAR        uiAllUserNames[256];
}SNMP_PPPOE_ALL_USER_NAMES_T;

typedef struct{
    CHAR        acLteENodeBId[16];
    CHAR        acLteLCID[16];
    CHAR        acLteTac[16];
}SNMP_LTE_ENODEB_INFO_T;

typedef struct{
    UINT32        ulLteLockType;
    UINT32        ulLteAutoLock;
    UINT32        ulLteAutoMax;
    UINT32        ulLteLockNum;
}SNMP_LTE_STATION_LOCK_T;

typedef struct{
    UINT32      ulLteLockNumID;
    CHAR        ulLteLockID[CHAR_LEN_32];
    UINT32      ulLteLockCfgState;
}SNMP_LTE_STATION_LOCK_TABLE_T;

typedef struct{
    CHAR        acLteLockAllID[256];
}SNMP_LTE_STATION_ALL_LOCK_ID_T;
/********************************* 数据存储 **********************************/
typedef struct
{
    /* Sys Info */
    SNMP_SYS_DEV_INFO_T                         stSysDevInfo;
    SNMP_SYS_SW_INFO_T                          stSwInfo;
    SNMP_SYS_HW_INFO_T                          stHwInfo; 
    SNMP_SYS_MANAGE_INFO_T                      stManageInfo;
    SNMP_SYS_PROTO_INFO_T                       stProtoInfo;
    SNMP_DEVICE_HEART_INFO_T                    stDeviceHeartInfo;
    SNMP_DEBUG_INFO_T                           stDebugInfo;
    
    /* Lte Info */
    SNMP_LTE_HW_INFO_T                          stLteHwInfo;
    SNMP_LTE_NET_INFO_T                         stLteNetInfo;
    SNMP_LTE_CFG_INFO_T                         stLteCfgInfo;

    /*WIFI Info*/
    SNMP_WIFI_INFO_T                            stWifiInfo;

    /*Location Info*/
    SNMP_GPS_INFO_T                             stGpsInfo;
    
    /* dev State Info */
    SNMP_WAN_STATE_INFO_T                       stWanStateInfo;
    SNMP_WAN_USER_NUM_T                         stWanUserNum;
    SNMP_WAN_USER_INFO_T                        stWanUserInfo[TABLE_MAXRECNUM_LTE_USER];
    SNMP_LAN_STATE_INFO_T                       stLanStateInfo;
    SNMP_ACCOUNT_NUM_T                          stAccountNum;
    SNMP_ACCOUNT_INFO_T                         stAccountInfo[TABLE_MAXRECNUM_LTE_ACCOUNT];
    SNMP_NTP_INFO_T                             stNtpInfo;
    SNMP_LOCAL_LOG_T                            stLocalLog;
    SNMP_REMOTE_LOG_T                           stRemoteLog;
    SNMP_LOG_CFG_T                              stLogCfg;
    SNMP_LAC_NUM_T                              stLacNum;
    SNMP_LAC_INFO_T                             stLacInfo[TABLE_MAXRECNUM_LTE_LAC];
    SNMP_LTEKP_CFG_T                            stLteKPCfg;
    SNMP_LTEKP_INFO_T                           stLteKPInfo[TABLE_MAXRECNUM_LTE_KEEPALIVE];
    
    /* Oper Info */
    SNMP_OPER_INFO_T                            stOperInfo;
    SNMP_FILE_TRANS_INFO_T                      stFileTrans;

    SNMP_PPPOE_USER_NUM_T                       stPppoeUserNum;
    SNMP_PPPOE_USER_INFO_T                      stPppoeUserInfo[TABLE_MAXRECNUM_LTE_USER];
    SNMP_PPPOE_ALL_USER_NAMES_T                 stPppoeAllUserNames;
    SNMP_LTE_ENODEB_INFO_T                      stLteEnodb;
    SNMP_LTE_STATION_LOCK_T                     stLteStationLock;
    SNMP_LTE_STATION_LOCK_TABLE_T               stLteStationLockTable[TABLE_MAXRECNUM_LTE_LOCK_TABLE];
    SNMP_LTE_STATION_ALL_LOCK_ID_T              stLteStationAllLockID;
}SNMP_AGENT_DATA_T;

/******************************* 表ID定义 *************************************/
typedef enum
{
	TABLE_ID_AP_CONFIG_BEGIN, 
     
    /* Sys Info */
    TABLE_ID_SYS_MANAGE_INFO,
    TABLE_ID_DEVICE_HEART_INFO,
	TABLE_ID_SYS_DEBUG_INFO,
    /* Lte Info */
	TABLE_ID_SYS_LTE_CFG_INFO,
	TABLE_ID_LTE_STATION_LOCK,
	TABLE_ID_LTE_STATION_TABLE_LOCK,
    /*deviceStat*/
	TABLE_ID_ACCOUNT_NUM,
	TABLE_ID_ACCOUNT_TABLE,
	TABLE_ID_NTP_INFO,    
	TABLE_ID_LOCAL_LOG,
	TABLE_ID_REMOTE_LOG,
	TABLE_ID_LOG_CFG,
	TABLE_ID_LAC_NUM,
    TABLE_ID_LAC_INFO,
    TABLE_ID_LTEKP_CFG,
    TABLE_ID_LTEKP_INFO,
    /* Device Oper */
    TABLE_ID_LTE_SYS_OPER,
    TABLE_ID_LTE_FILE_TRANS,    
	TABLE_ID_AP_CONFIG_END,

    /*=============================================*/
	TABLE_ID_AP_STAT_BEGIN,
	
    /* Sys Info */
    TABLE_ID_SYS_DEV_INFO,
    TABLE_ID_SYS_SW_INFO,
    TABLE_ID_SYS_HW_INFO,    
    TABLE_ID_SYS_PROTO_INFO,
    /* Lte Info */
    TABLE_ID_LTE_HW_INFO,
    TABLE_ID_LTE_NET_INFO,
    TABLE_ID_LTE_STATION_ALL_LOCK_ID,
    /* Wifi Info */
	TABLE_ID_SYS_LTE_WIFI_INFO,
    /* location Info */
    TABLE_ID_GPS_INFO,
    /* Wan State Info */
    TABLE_ID_WAN_STAT,
    TABLE_ID_USER_NUM,
    TABLE_ID_USER_INFO,
    TABLE_ID_PPPOE_USER_NUM,
    TABLE_ID_PPPOE_USER_INFO,
    TABLE_ID_PPPOE_ALL_USER_NAMES,
    TABLE_ID_LTE_ENODB_INFO,
    TABLE_ID_LAN_STAT,

	TABLE_ID_AP_STAT_END,
	
	MAX_TABLE_NUM 
}SNMP_TABLE_E;

#define UDP_QUEUE_NUM   1200//256
/* 批量处理最大数目 */
#define UDP_QUEUE_BATCHNUM_MAX     32

typedef struct queueNode
{
    struct queueNode 	*pstNext;
    struct queueNode 	*pstPrev;

    UINT8               *pucData;       /* point to the node data */	
} QUEUE_NODE_T;

typedef struct
{
    QUEUE_NODE_T    stFreeHead;         /* head of free node list */
    QUEUE_NODE_T    stFreeTail;         /* tail of free node list */

    QUEUE_NODE_T    stUsedHead;         /* tail of used node list */
    QUEUE_NODE_T    stUsedTail;         /* tail of used node list */

    UINT32          ulNodeNum;          /* node num */
    UINT32          ulNodeDataSize;     /* node data size */
	
    QUEUE_NODE_T    *pstNodeList;  	    /* node list for queue */	
} QUEUE_T;
/* END: Added by zoucaihong, 2013/9/24 */

/******************************* 注册表宏定义 *********************************/
#define SNMP_INIT_TABLE(table_oid, ulTableID, szTableName, ulMaxTableRecNum, pfTableValidFunc) \
{																	\
    if (!snmp_init_table_info(pstTable,                             \
                         TRUE,                                      \
                         ulTableID,                                 \
                         szTableName,                               \
                         pfTableValidFunc,                          \
                         sizeof(table_oid)/sizeof(OID),             \
                         table_oid,                                 \
                         ulMaxTableRecNum,                          \
                         sizeof(*type)))							\
	{																\
		return FALSE;												\
	}																\
}

#define SNMP_INIT_SCALAR(table_oid, ulTableID, szTableName, pfTableValidFunc) \
{																	\
    if (!snmp_init_table_info(pstTable,                             \
                         FALSE,                                     \
                         ulTableID,                                 \
                         szTableName,                               \
                         pfTableValidFunc,                          \
                         sizeof(table_oid)/sizeof(OID),             \
                         table_oid,                                 \
                         0,											\
                         sizeof(*type)))							\
	{																\
		return FALSE;												\
	}																\
}

#define SNMP_INIT_SCALAR_FIELD(var, ucColumnType, mib_rw)				\
{                                                                       \
    if (!snmp_init_column_info(pstTable, "", ucColumnType, 				\
                           sizeof(var), (UINT16)(UINT32)(&var), mib_rw, FALSE)) \
    {                                                                   \
        return FALSE;                                                   \
    }                                                                   \
}

#define SNMP_INIT_TABLE_FIELD(var, ucColumnType, mib_rw, bIsKey)		\
{                                                                       \
    if (!snmp_init_column_info(pstTable, "", ucColumnType, 				\
                           sizeof(var), (UINT16)(UINT32)(&var), mib_rw, bIsKey))\
    {                                                                   \
        return FALSE;                                                   \
    }                                                                   \
}

#define SNMP_INIT_SCALAR_COLUMN(var, columnName, ucColumnType, mib_rw)	\
{                                                                       \
    if (!snmp_init_column_info(pstTable, columnName, ucColumnType,   	\
                           sizeof(var), (UINT16)(UINT32)(&var), mib_rw, FALSE)) \
    {                                                                   \
        return FALSE;                                                   \
    }                                                                   \
}

#define SNMP_INIT_TABLE_COLUMN(var, columnName, ucColumnType, mib_rw, bIsKey) \
{                                                                       \
    if (!snmp_init_column_info(pstTable, columnName, ucColumnType,  	\
                           sizeof(var), (UINT16)(UINT32)(&var), mib_rw, bIsKey))\
    {                                                                   \
        return FALSE;                                                   \
    }                                                                   \
}

#define SNMP_INIT_COLUMN_ENUM(enumEntry)                \
{                                                        \
    if (!snmp_init_column_enum_info(pstTable, enumEntry))	\
	{                                                                 \
		return FALSE;                                             \
	}                                                                 \
}

#define SNMP_INIT_COLUMN_RANGE(min, max) \
{\
	if (!snmp_init_column_range(pstTable, min, max))\
	{\
		return FALSE;\
	}\
}

#define SNMP_INIT_COLUMN_UNITS(units)\
{\
    if (!snmp_init_column_units(pstTable, units))\
    {\
    	return FALSE;\
    }\
}

#define SNMP_INIT_COLUMN_DEFVAL(pdefval, defval)\
{\
    if (!snmp_init_column_defval(pstTable, pdefval, defval))\
    {\
    	return FALSE;\
    }\
}


#endif

