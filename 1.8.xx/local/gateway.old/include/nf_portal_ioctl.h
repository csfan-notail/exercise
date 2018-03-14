/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : nf_portal_ioctl.h
  �� �� ��   : ����
  ��    ��   : WeiHonggang
  ��������   : 2013��11��26�� ������
  ����޸�   :
  ��������   : nf_portal_ioctl.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��11��26�� ������
    ��    ��   : WeiHonggang
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __NF_PORTAL_IOCTL_H__
#define __NF_PORTAL_IOCTL_H__


/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "ap_types.h"
#include "nf_portal_public.h"

/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �������Ͷ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/
/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
#define NF_PORTAL_PROC_FILE        "nf_portal"

#define NF_PORTAL_IOCTL_CMD_BEGIN 0


#define NF_PORTAL_MOD_VERSION  "v1.0.1"
#define NF_PORTAL_APP_VERSION  "v1.0.1"

#define NF_PORTAL_MOD_DEP_VERSION "v1.0.1"   /*֧���ں�ģ���APP��С�汾��*/
#define NF_PORTAL_APP_DEP_VERSION "v1.0.1"   /*֧��APP���ں�ģ����С�汾��*/


 /*----------------------------------------------*
 * �����                                       *
 *----------------------------------------------*/


 
/*----------------------------------------------*
 * ö�ٶ���                                       *
 *----------------------------------------------*/
typedef enum
{
    NF_PORTAL_IOCTL_CMD_GET_VERSION = NF_PORTAL_IOCTL_CMD_BEGIN,
    
    NF_PORTAL_IOCTL_CMD_SET_DEBUG_LEVEL,
    NF_PORTAL_IOCTL_CMD_GET_DEBUG_LEVEL,

    NF_PORTAL_IOCTL_CMD_SET_AUTH_STATE,
    NF_PORTAL_IOCTL_CMD_GET_AUTH_STATE,
    
    NF_PORTAL_IOCTL_CMD_SET_HOST_IP,
    NF_PORTAL_IOCTL_CMD_GET_HOST_IP,

    NF_PORTAL_IOCTL_CMD_SET_AUTH_SERVER_IP,
    NF_PORTAL_IOCTL_CMD_GET_AUTH_SERVER_IP,
    
    NF_PORTAL_IOCTL_CMD_SET_CCS_SERVER_IP,
    NF_PORTAL_IOCTL_CMD_GET_CCS_SERVER_IP,

	NF_PORTAL_IOCTL_CMD_SET_SNMP_SERVER_IP,
    NF_PORTAL_IOCTL_CMD_GET_SNMP_SERVER_IP,
    
    NF_PORTAL_IOCTL_CMD_SET_FREE_AUTH_SW,
    NF_PORTAL_IOCTL_CMD_GET_FREE_AUTH_SW,

    NF_PORTAL_IOCTL_CMD_GET_STA_LIST,
    NF_PORTAL_IOCTL_CMD_STA_LIST_FLUSH,

    NF_PORTAL_IOCTL_CMD_SET_LAN_IF_NAME,
    NF_PORTAL_IOCTL_CMD_GET_LAN_IF_NAME,

    NF_PORTAL_IOCTL_CMD_ADD_DEST_IP_BAW,
    NF_PORTAL_IOCTL_CMD_DEL_DEST_IP_BAW,
    NF_PORTAL_IOCTL_CMD_GET_DEST_IP_BAW_LIST,
    NF_PORTAL_IOCTL_CMD_DEST_IP_BAW_FLUSH,

    NF_PORTAL_IOCTL_CMD_ADD_FREE_AUTH_PORT,
    NF_PORTAL_IOCTL_CMD_DEL_FREE_AUTH_PORT,
    NF_PORTAL_IOCTL_CMD_GET_FREE_AUTH_PORT_LIST,
    NF_PORTAL_IOCTL_CMD_FREE_AUTH_PORT_FLUSH,

    NF_PORTAL_IOCTL_CMD_ADD_MAC_BAW,
    NF_PORTAL_IOCTL_CMD_DEL_MAC_BAW,
    NF_PORTAL_IOCTL_CMD_GET_MAC_BAW_LIST,
    NF_PORTAL_IOCTL_CMD_MAC_BAW_FLUSH,

    NF_PORTAL_IOCTL_CMD_ADD_USER_IP_BAW,
    NF_PORTAL_IOCTL_CMD_DEL_USER_IP_BAW,
    NF_PORTAL_IOCTL_CMD_GET_USER_IP_BAW_LIST,
    NF_PORTAL_IOCTL_CMD_USER_IP_BAW_FLUSH,

    NF_PORTAL_IOCTL_CMD_ADD_URL_BAW,
    NF_PORTAL_IOCTL_CMD_DEL_URL_BAW,
    NF_PORTAL_IOCTL_CMD_GET_URL_BAW_LIST,
    NF_PORTAL_IOCTL_CMD_URL_BAW_FLUSH,
    NF_PORTAL_IOCTL_CMD_GET_URL_IP_BAW_LIST,
    NF_PORTAL_IOCTL_CMD_ADD_URL_IP_BAW,

    NF_PORTAL_IOCTL_CMD_ADD_PASS_PORT_NUM,
    NF_PORTAL_IOCTL_CMD_DEL_PASS_PORT_NUM,
    NF_PORTAL_IOCTL_CMD_GET_PASS_PORT_NUM_LIST,
    NF_PORTAL_IOCTL_CMD_PASS_PORT_NUM_FLUSH,

	/*begin: add@2015.01.31 for wechat connect wifi*/
	NF_PORTAL_IOCTL_CMD_ADD_WXL_SERVERIP,
    NF_PORTAL_IOCTL_CMD_DEL_WXL_SERVERIP,
    NF_PORTAL_IOCTL_CMD_GET_WXL_SERVERIP_LIST,
    NF_PORTAL_IOCTL_CMD_WXL_SERVERIP_FLUSH,
	/*end: add@2015.01.31 for wechat connect wifi*/
    /*���Զ�ģ���е���������޸Ļ�����*/
    /*.......*/
}NF_PORTAL_IOCTL_CMD_E;

/*----------------------------------------------*
 * ��չ���Ͷ���                                     *
 *----------------------------------------------*/


/*----------------------------------------------*
 * �ṹ����                                       *
 *----------------------------------------------*/
typedef struct
{
    #define NF_PORTAL_VERSION_SIZE  128
    CHAR acModVersion[NF_PORTAL_VERSION_SIZE];    /*�ں�ģ��汾��*/
    CHAR acModDepVersion[NF_PORTAL_VERSION_SIZE]; /*֧���ں�ģ���APPģ����С�汾��*/
}NF_PORTAL_IOCTL_VERSION_T;


typedef struct
{
    UINT32 uiDebugLevel;
}NF_PORTAL_IOCTL_DEBUG_T;

typedef struct
{
    UINT8 aucMac[ETH_ALEN];
    UINT32 uiIp;
    NF_PORTAL_AUTH_STATE_E enState;
}NF_PORTAL_IOCTL_AUTH_STATE_T;

typedef struct
{
    UINT32 uiIp;
}NF_PORTAL_IOCTL_IP_T;

typedef struct
{
    NF_PORTAL_SW_STATE_E enSW;
}NF_PORTAL_IOCTL_SW_T;

typedef struct
{
    #define NF_PORTAL_STA_LIST_MAX 8
    UINT32 uiStartNum;
    UINT32 uiNum;
    NF_PORTAL_AUTH_STATE_E enState;
    NF_PORTAL_STA_ATTRIB_T astSta[NF_PORTAL_STA_LIST_MAX];
}NF_PORTAL_IOCTL_STA_LIST_T;

typedef struct
{
    UINT8 aucLanIfName[IFNAMSIZ];
}NF_PORTAL_IOCTL_IF_NAME_T;

typedef struct
{
    NF_PORTAL_IP_BAW_ATTRIB_T stIpBaw;
}NF_PORTAL_IOCTL_IP_BAW_T;
typedef struct
{
    #define NF_PORTAL_IP_BAW_LIST_MAX 8
    UINT32 uiStartNum;
    UINT32 uiNum;
    NF_PORTAL_BAW_FLAG_E enFlag;
    NF_PORTAL_IP_BAW_ATTRIB_T astIpBaw[NF_PORTAL_IP_BAW_LIST_MAX];
}NF_PORTAL_IOCTL_IP_BAW_LIST_T;

typedef struct
{
    #define NF_PORTAL_PORT_LIST_MAX 8
    UINT32 uiStartNum;
    UINT32 uiNum;
    NF_PORTAL_IOCTL_IF_NAME_T astPort[NF_PORTAL_PORT_LIST_MAX];
}NF_PORTAL_IOCTL_PORT_LIST_T;

typedef struct
{
    NF_PORTAL_MAC_BAW_ATTRIB_T stMacBaw;
}NF_PORTAL_IOCTL_MAC_BAW_T;
typedef struct
{
    #define NF_PORTAL_MAC_BAW_LIST_MAX 8
    UINT32 uiStartNum;
    UINT32 uiNum;
    NF_PORTAL_BAW_FLAG_E enFlag;
    NF_PORTAL_MAC_BAW_ATTRIB_T astMacBaw[NF_PORTAL_MAC_BAW_LIST_MAX];
}NF_PORTAL_IOCTL_MAC_BAW_LIST_T;

typedef struct
{
    NF_PORTAL_URL_BAW_ATTRIB_T stUrlBaw;
}NF_PORTAL_IOCTL_URL_BAW_T;

typedef struct
{
    #define NF_PORTAL_URL_BAW_LIST_MAX 4
    UINT32 uiStartNum;
    UINT32 uiNum;
    NF_PORTAL_BAW_FLAG_E enFlag;
    NF_PORTAL_URL_BAW_ATTRIB_T astUrlBaw[NF_PORTAL_URL_BAW_LIST_MAX];
}NF_PORTAL_IOCTL_URL_BAW_LIST_T;

typedef struct
{
    UINT32 uiIp;
    UINT8 aucUrl[URL_MAX_SIZE];
}NF_PROTAL_IOCTL_URL_IP_BAW_T;
typedef struct
{
     #define NF_PORTAL_URL_IP_BAW_LIST_MAX 4
    UINT32 uiStartNum;
    UINT32 uiNum;
    NF_PORTAL_BAW_FLAG_E enFlag;
    NF_PROTAL_IOCTL_URL_IP_BAW_T astUrlIpBaw[NF_PORTAL_URL_IP_BAW_LIST_MAX];   
}NF_PORTAL_IOCTL_URL_IP_BAW_LIST_T;

typedef struct
{
    NF_PORTAL_PORT_NUM_ATTRIB_T stPortNum;
}NF_PORTAL_IOCTL_PORT_NUM_T;

typedef struct
{
    #define NF_PORTAL_PORT_NUM_LIST_MAX 8
    UINT32 uiStartNum;
    UINT32 uiNum;
    UINT8 ucIpProto;
    NF_PORTAL_IOCTL_PORT_NUM_T astPortNum[NF_PORTAL_PORT_NUM_LIST_MAX];
}NF_PORTAL_IOCTL_PORT_NUM_LIST_T;
/*begin: add@2015.01.31 for wechat connect wifi*/
typedef struct
{
	NF_PORTAL_WXL_SERVERIP_ATTRIB_T stWxlServerIp;
}NF_PORTAL_IOCTL_WXL_SERVERIP_T;
typedef struct
{
	#define NF_PORTAL_WXL_SERVERIP_NUM_LIST_MAX 8
	UINT32 uiStartNum;
	UINT32 uiNum;
	NF_PORTAL_WXL_SERVERIP_ATTRIB_T astWxlServerip[NF_PORTAL_WXL_SERVERIP_NUM_LIST_MAX];
}NF_PORTAL_IOCTL_WXL_SERVERIP_LIST_T;
/*end: add@2015.01.31 for wechat connect wifi*/

/*���Զ�ģ���е���������޸Ļ�����*/
/*.......*/

typedef struct
{
	NF_PORTAL_IOCTL_CMD_E enCmd;

	union
	{
	    NF_PORTAL_IOCTL_VERSION_T stVersion;
	    NF_PORTAL_IOCTL_DEBUG_T stDebug;
	    NF_PORTAL_IOCTL_AUTH_STATE_T stIoAuthState;
	    NF_PORTAL_IOCTL_IP_T stIoIp;
           NF_PORTAL_IOCTL_SW_T stIoSW;
           NF_PORTAL_IOCTL_STA_LIST_T stIoStaList;
           NF_PORTAL_IOCTL_IF_NAME_T stIoIfName;
           NF_PORTAL_IOCTL_IP_BAW_T stIoIpBaw;
           NF_PORTAL_IOCTL_IP_BAW_LIST_T stIoIpBawList;
           NF_PORTAL_IOCTL_PORT_LIST_T stIoPortList;
           NF_PORTAL_IOCTL_MAC_BAW_T stIoMacBaw;
           NF_PORTAL_IOCTL_MAC_BAW_LIST_T stIoMacBawList;
           NF_PORTAL_IOCTL_URL_BAW_T stIoUrlBaw;
           NF_PORTAL_IOCTL_URL_BAW_LIST_T stIoUrlBawList;
		   NF_PROTAL_IOCTL_URL_IP_BAW_T stIoUrlIpBaw;
           NF_PORTAL_IOCTL_URL_IP_BAW_LIST_T stIoUrlIpBawList;
           NF_PORTAL_IOCTL_PORT_NUM_T stIoPortNum;
           NF_PORTAL_IOCTL_PORT_NUM_LIST_T stIoPortNumList;
		   /*begin: add@2015.01.31 for wechat connect wifi*/
		   NF_PORTAL_IOCTL_WXL_SERVERIP_T stIoWxlServerIp;
		   NF_PORTAL_IOCTL_WXL_SERVERIP_LIST_T stIoWxlServerIpList;
		   /*end: add@2015.01.31 for wechat connect wifi*/
	    /*���Զ�ģ���е���������޸Ļ�����*/
        /*.......*/
	}unData;
}NF_PORTAL_IOCTL_T;




/*----------------------------------------------*
 * ��������ԭ��˵��                             *
 *----------------------------------------------*/



#endif /* __NF_PORTAL_IOCTL_H__ */

