/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : gdcontrl.h
  版 本 号   : 初稿
  作    者   : xiao wei
  生成日期   : 2017年5月2日 星期三
  最近修改   :
  功能描述   : 
  函数列表   :
  修改历史   :
  1.日    期   : 2017年5月2日 星期三
    作    者   : xiao wei
    修改内容   : 创建文件

******************************************************************************/
#ifndef __GDCONTRL_H__
#define __GDCONTRL_H__

//协议相关字段protocol
#define MSG_PROTOCOL_HEAD				0xeb90eb90			//协议头

#define MSG_RED_LED_FLASH_SEND			0x0b				//LED红灯闪烁指令
#define MSG_RED_LED_FLASH_RECV			0x8b				//LED红灯闪烁回复指令

#define MSG_RED_LED_FLASH_ON			0x02				//LED红灯闪烁开启
#define MSG_RED_LED_FLASH_OFF			0x00				//LED红灯闪烁关闭

#define MSG_VOICE_ALARM_SEND			0x0c				//音响报警指令
#define MSG_VOICE_ALARM_RECV			0x8c				//音响报警回复指令

#define MSG_VOICE_ALARM_ON				0x01				//音响报警开启
#define MSG_VOICE_ALARM_OFF				0x00				//音响报警关闭

#define MSG_CMD_RECV_STA				0xaa				//协议回复标记字段

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
