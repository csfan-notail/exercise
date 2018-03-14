#ifndef __NF_PORTAL_PUBLIC__
#define __NF_PORTAL_PUBLIC__

#include "ap_types.h"

#ifndef IFNAMSIZ
#define IFNAMSIZ 16
#endif
#ifndef ETH_ALEN
#define ETH_ALEN 6
#endif
#define URL_MAX_SIZE 128

#ifndef IPPROTO_UDP
#define IPPROTO_UDP 17
#endif

#ifndef IPPROTO_TCP
#define IPPROTO_TCP 6
#endif

/*¿ª¹Ø×´Ì¬*/
typedef enum
{
    SW_OFF = 0,
    SW_ON = 1
}NF_PORTAL_SW_STATE_E;

typedef enum
{
    NF_PORTAL_DIR_UP = 0,
    NF_PORTAL_DIR_DOWN,
    NF_PORTAL_DIR_MAX
}NF_PORTAL_DIR_E;

typedef struct
{
    UINT32 ui4GByte;
    UINT32 uiByte;
}NF_PORTAL_TS_T;

typedef enum
{
    STATE_NOT_AUTH = 0,
    STATE_BEING_AUTH,
    STATE_AUTH,
    STATE_DEL,
    STATE_MAX
}NF_PORTAL_AUTH_STATE_E;
typedef struct
{
    UINT8 aucMac[ETH_ALEN];
    UINT32 uiIp;
    NF_PORTAL_AUTH_STATE_E enAuthState;
    UINT32 uiSec;
    NF_PORTAL_TS_T astTS[NF_PORTAL_DIR_MAX];
}NF_PORTAL_STA_ATTRIB_T;

typedef enum
{
    BLACK =0,
    WHITE,
    FREE_AUTH,
    BAW,
    MUST_REDIRECT /*add@2014.11.02*/
}NF_PORTAL_BAW_FLAG_E;

typedef struct
{
    UINT32 uiIp;
    UINT16 usPort;
    NF_PORTAL_BAW_FLAG_E enFlag;
}NF_PORTAL_IP_BAW_ATTRIB_T;

typedef struct
{
    UINT8 aucMac[ETH_ALEN];
    NF_PORTAL_BAW_FLAG_E enFlag;
}NF_PORTAL_MAC_BAW_ATTRIB_T;

typedef struct
{
    UINT8 aucUrl[URL_MAX_SIZE];
    NF_PORTAL_BAW_FLAG_E enFlag;
}NF_PORTAL_URL_BAW_ATTRIB_T;

typedef struct
{
    UINT8 ucIpProto;
    UINT16 usPortNum;
}NF_PORTAL_PORT_NUM_ATTRIB_T;

/*begin: add@2015.01.31 for wechat connect wifi*/
typedef struct
{
    UINT32 uiIp;
    UINT32 uiMask;
}NF_PORTAL_WXL_SERVERIP_ATTRIB_T;
/*end: add@2015.01.31 for wechat connect wifi*/
#endif

