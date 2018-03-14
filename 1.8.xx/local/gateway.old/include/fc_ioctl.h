/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : fc_ioctl.h
  �� �� ��   : ����
  ��    ��   : Zhong Huaqing
  ��������   : 2012��8��10�� ������
  ����޸�   :
  ��������   : fc_ioctl.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��8��10�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __FC_IOCTL_H__
#define __FC_IOCTL_H__


/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/

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
 #define FC_IOCTL_CMD_BEGIN 600
/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
#define FC_MOD_VERSION  "v1.1.1"            /* For cpe flowctrl only */
#define FC_APP_VERSION  "v1.1.1"

#define FC_MOD_DEP_VERSION "v1.1.1"   /*֧���ں�ģ���APP��С�汾��*/
#define FC_APP_DEP_VERSION "v1.1.1"   /*֧��APP���ں�ģ����С�汾��*/

 /*----------------------------------------------*
 * �����                                       *
 *----------------------------------------------*/


 
/*----------------------------------------------*
 * ö�ٶ���                                       *
 *----------------------------------------------*/
typedef enum
{
    FC_IOCTL_CMD_GET_VERSION = FC_IOCTL_CMD_BEGIN, /*Added on 2012.10.8*/
    
    FC_IOCTL_CMD_SET_ADMIN,
    FC_IOCTL_CMD_GET_ADMIN,
    
    FC_IOCTL_CMD_SET_DEBUG_LEVEL,
    FC_IOCTL_CMD_GET_DEBUG_LEVEL,
    
    
    FC_IOCTL_CMD_SET_RULE,
    FC_IOCTL_CMD_GET_RULE,
    
    FC_IOCTL_CMD_SET_GEAR,
    FC_IOCTL_CMD_GET_GEAR,
    
    FC_IOCTL_CMD_ADD_STA,
    FC_IOCTL_CMD_DEL_STA,
    
    FC_IOCTL_CMD_ADD_VAP,
    FC_IOCTL_CMD_DEL_VAP,
    
    FC_IOCTL_CMD_ADD_SSID,
    FC_IOCTL_CMD_DEL_SSID,
    
    FC_IOCTL_CMD_ADD_AP,
    FC_IOCTL_CMD_DEL_AP,

	FC_IOCTL_CMD_ADD_VLAN,
    FC_IOCTL_CMD_DEL_VLAN,
    
    FC_IOCTL_CMD_GET_LIST,
    
    FC_IOCTL_CMD_FLUSH,
    
    #if 0
    FC_IOCTL_CMD_SET_AUTO_LEARN,
    FC_IOCTL_CMD_GET_AUTO_LEARN,
    #endif
    
    FC_IOCTL_CMD_SET_AUTO_RATE,
    FC_IOCTL_CMD_GET_AUTO_RATE,
    
    FC_IOCTL_CMD_SET_DEV_SSID,
    FC_IOCTL_CMD_GET_DEV_SSID,
    FC_IOCTL_CMD_GET_DEV_SSID_LIST,
    
}FC_IOCTL_CMD_E;

/*----------------------------------------------*
 * ��չ���Ͷ���                                     *
 *----------------------------------------------*/


/*----------------------------------------------*
 * �ṹ����                                       *
 *----------------------------------------------*/
typedef struct
{
    #define FC_VERSION_SIZE  128
    CHAR acModVersion[FC_VERSION_SIZE];    /*�ں�ģ��汾��*/
    CHAR acModDepVersion[FC_VERSION_SIZE]; /*֧���ں�ģ���APPģ����С�汾��*/
}FC_IOCTL_VERSION_T;


typedef struct
{
    BOOL bEnable;
}FC_IOCTL_ADMIN_T;


typedef struct
{
    INT32 iDebugLevel;
}FC_IOCTL_DEBUG_T;


typedef struct
{
    FC_RULE_E enRule;
}FC_IOCTL_RULE_T;

typedef struct
{
    UINT32 uiGearByte;
}FC_IOCTL_GEAR_T;


typedef struct
{
    FC_STA_T stSta;
}FC_IOCTL_STA_T;


typedef struct
{
    FC_VAP_T stVap;
}FC_IOCTL_VAP_T;


typedef struct
{
    FC_SSID_T stSsid;
}FC_IOCTL_SSID_T;


typedef struct
{
    FC_AP_T stAp;
}FC_IOCTL_AP_T;


typedef struct
{
    FC_VLAN_T stVlan;
}FC_IOCTL_VLAN_T;


typedef struct
{
    FC_ENTITY_LIST_T stList;
    UINT32 uiStartNum;
    UINT32 uiNum;

}FC_IOCTL_ENTITY_LIST_T;

#if 0
typedef struct
{
    FC_QUEUE_AUTO_LEARN_T stAutoLearn;
}FC_IOCTL_AUTO_LEARN_T;
#endif

typedef struct
{
    BOOL bIsOn;
}FC_IOCTL_AUTO_RATE_T;

typedef struct
{
    CHAR acDevName[IFNAMSIZ];
    CHAR acSsid[SSID_NAMSIZ];
}FC_IOCTL_DEV_SSID_T;

typedef struct
{
    #define FC_MAX_VAP_NUM 32
    
    FC_IOCTL_DEV_SSID_T astDevSsidList[FC_MAX_VAP_NUM];
    UINT32 uiNum;
}FC_IOCTL_DEV_SSID_LIST_T;



typedef struct
{
	FC_IOCTL_CMD_E enCmd;

	union{
	    FC_IOCTL_VERSION_T stVersion;
	    FC_IOCTL_ADMIN_T stAdmin;
	    FC_IOCTL_DEBUG_T stDebug;
	    FC_IOCTL_RULE_T stRule;
	    
	    FC_IOCTL_GEAR_T stGear;
	    
	    FC_IOCTL_STA_T stSta;
	    FC_IOCTL_VAP_T stVap;
	    FC_IOCTL_SSID_T stSsid;
	    FC_IOCTL_AP_T stAp;
		FC_IOCTL_VLAN_T stVlan;
	    
	    FC_IOCTL_ENTITY_LIST_T stEntityList;
	    
	    //FC_IOCTL_AUTO_LEARN_T stAutoLearn;
	    FC_IOCTL_AUTO_RATE_T stAutoRate;
	    
	    FC_IOCTL_DEV_SSID_T stDevSsid;
	    FC_IOCTL_DEV_SSID_LIST_T stDevSsidList;
	    
	    
	}u;
}FC_IOCTL_T;




/*----------------------------------------------*
 * ��������ԭ��˵��                             *
 *----------------------------------------------*/



#endif /* __FC_IOCTL_H__ */

