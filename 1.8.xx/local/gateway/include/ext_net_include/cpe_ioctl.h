#ifndef CPE_IOCTL_H
#define CPE_IOCTL_H
#include "ap_types.h"

#define CP_TO_USER_ERR    1000
#define CP_FROM_USER_ERR  1001
#define CALL_FUNCTION_ERR 1002
#define CMD_NOT_FOUDN_ERR 1003

#define WIRELESS_CAN_MANAGE     1
#define WIRELESS_CAN_NOT_MANAGE 0

#define DEBUG_ENABLE     1
#define DEBUG_DISABLE    0

#define DHCP_RELAY_ENABLE  1
#define DHCP_RELAY_DISABLE  0


/* userspace used only */
#ifndef __KERNEL__

#define SIOCGCPE		0x8984		/* CPE bridge private ioctl */
#define SIOCSCPE		0x8985		/* CPE bridge private ioctl	*/

typedef struct CPE_IOCTL_CMD_HELP
{
    const char  *pcName;
    int        (*pFunc)(int argc, char **argv);
    const char  *pcHelp;
    
}CPE_IOCTL_CMD_HELP_STRU;
#endif

/*2012-6-11 WeiHonggang add for sta_agent module ioctl start*/
typedef struct
{
    int ulStaAgentDebug;
}STA_AGENT_DEBUG_T;

typedef struct
{
    int ulDhcpRelay;
}DHCP_RELAY_T;
/*2012-6-11 WeiHonggang add for sta_agent module ioctl end*/

/* BEGIN: Modified by xucongjiang, 2012/10/25   PN:1 */
typedef struct
{
    BOOL iSw;                /* 是否开启禁止请求本地的arp转发的功能 */
    unsigned int uiIpAddr; /* localhost ip address */
}CPE_IOCTL_LOCAL_IP_T;
/* END:   Modified by xucongjiang, 2012/10/25   PN:1 */


typedef struct CPE_IOCTL_ARGS
{
    int iCmd;  /* must be one value of the following CPE_IOCTL_CMDS_ENUM  */
    union
    {
        int iManagePortSwitch; /* control the cpe manager port: wireless or wire port */
        STA_AGENT_DEBUG_T stStaAgentDebug;
        DHCP_RELAY_T stDhcpRelay;
        CPE_IOCTL_LOCAL_IP_T stIp;
    }u;
    
}CPE_IOCTL_ARGS_STRU;


enum CPE_IOCTL_CMDS
{
    CPE_SET_MANAGE_PORT = 1,
    CPE_GET_MANAGE_PORT,
    STA_AGENT_DEBUG_SET,
    STA_AGENT_DEBUG_GET,
    DHCP_RELAY_SWITCH_SET,
    DHCP_RELAY_SWITCH_GET,
    CPE_SET_FORBID_LOCAL_ARP,
    CPE_GET_FORBID_LOCAL_ARP,
    
};




#endif /* CPE_IOCTL_H */
