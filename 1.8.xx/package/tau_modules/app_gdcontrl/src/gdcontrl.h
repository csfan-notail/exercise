/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : gdcontrl.h
  �� �� ��   : ����
  ��    ��   : xiao wei
  ��������   : 2017��5��2�� ������
  ����޸�   :
  ��������   : 
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2017��5��2�� ������
    ��    ��   : xiao wei
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __GDCONTRL_H__
#define __GDCONTRL_H__

//Э������ֶ�protocol
#define MSG_PROTOCOL_HEAD				0xeb90eb90			//Э��ͷ

#define MSG_RED_LED_FLASH_SEND			0x0b				//LED�����˸ָ��
#define MSG_RED_LED_FLASH_RECV			0x8b				//LED�����˸�ظ�ָ��

#define MSG_RED_LED_FLASH_ON			0x02				//LED�����˸����
#define MSG_RED_LED_FLASH_OFF			0x00				//LED�����˸�ر�

#define MSG_VOICE_ALARM_SEND			0x0c				//���챨��ָ��
#define MSG_VOICE_ALARM_RECV			0x8c				//���챨���ظ�ָ��

#define MSG_VOICE_ALARM_ON				0x01				//���챨������
#define MSG_VOICE_ALARM_OFF				0x00				//���챨���ر�

#define MSG_CMD_RECV_STA				0xaa				//Э��ظ�����ֶ�

typedef struct{
     int index;
     int cid; 
     char name[32];
     char ip[32];
     int position;
     double latitude;   
     double longitude;   
     int sid; 
     int sstat;
}GD_INFO;

typedef struct shared_mem_st  
{  
    int written;
    GPS_INFO gpsinfo;
}SHARED_MEM;

struct gpsser{
	double latitude;   
	double longitude; 
	char ip[32]; 
	double radian; 	
};

struct gdctllist{
	GD_INFO gdinfo;
	struct list_head list;
};
#pragma pack(1)
struct msgdata{
	unsigned int head;
	unsigned char id;
	unsigned char type;
	unsigned char action;
};

struct msginfo{
	struct msgdata data;
	unsigned short crc16;
};


#define GDCTL_LOG(level, format, arg...) \
    do{ \
        printf("[%16s:%4d] "format,__func__,__LINE__, ##arg);\
        if(level <= LOG_ERR)	\
        	syslog(level, "gdcontrl:[%s:%d] "format,__func__,__LINE__, ##arg);\
    }while(0)

#endif /* __GDCONTRL_H__ */
