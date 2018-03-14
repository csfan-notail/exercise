/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : gps.h
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
#ifndef __GPS_H__
#define __GPS_H__

#define BUF_SIZE 1024
#define PI                      3.1415926
#define EARTH_RADIUS            6378.137        //地球近似半径

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
