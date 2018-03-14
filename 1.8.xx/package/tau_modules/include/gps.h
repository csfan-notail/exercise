/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : gps.h
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
#ifndef __GPS_H__
#define __GPS_H__

#define BUF_SIZE 1024
#define PI                      3.1415926
#define EARTH_RADIUS            6378.137        //������ư뾶

typedef struct{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
}date_time;

typedef struct{
     date_time D;
     char status;       
     double latitude;   
     double longitude;   
     char NS;             
     char EW;           
     double speed;        
     double high;        
}GPS_INFO;

#define GPS_LOG(level, format, arg...) \
    do{ \
        printf("[%16s:%4d] "format,__func__,__LINE__, ##arg);\
        if(level <= LOG_ERR)	\
        	syslog(level, "gps:[%s:%d] "format,__func__,__LINE__, ##arg);\
    }while(0)

#endif /* __GPS_H__ */
