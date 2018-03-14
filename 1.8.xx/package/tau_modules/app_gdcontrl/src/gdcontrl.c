#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <getopt.h>
#include <sys/stat.h>  
#include <sys/shm.h> 
#include <sys/time.h>
#include <math.h>
#include "ap_types.h"
#include "list.h"
#include "gps.h"
#include "gdcontrl.h"

#define GPS_WARNING_RADIUS			50				//默认GPS触发半径 单位:m

#define GPSCTL_DEFAULT_CONFIG		"/etc/gdcontrl"
#define GPSCTL_PUT_LOG_FILE			"/mnt/emmc/packet/gdctllog"

#define MGS_RECV_DEFAULT_TIME		3

static int previous_index;
static int log_upload_status;

struct gpsser gpsserinfo;
//static double gps_radian;

struct gdctllist mylist;
GPS_INFO gps_realtime_info;

unsigned short crc16_modbus(unsigned char *data, int length)  
{  
    unsigned char i;  
    unsigned short crc= 0xffff;        // Initial value  
    while(length--)  
    {  
        crc ^= *data++;            // crc ^= *data; data++;  
        for (i = 0; i < 8; ++i)  
        {  
            if (crc & 1)  
                crc = (crc >> 1) ^ 0xA001;        // 0xA001 = reverse 0x8005  
            else  
                crc = (crc >> 1);  
        }  
    }  
    return crc;  
} 

static unsigned short crc16(unsigned char *data, int length)  
{  
	unsigned char i;  
	unsigned short crc = 0xffff;
    while(length--)  
    {  
		crc ^= *data++;
		for (i = 0; i < 8; ++i)  
		{  
			if (crc & 1)  
				crc = (crc >> 1) ^ 0x8408;
			else  
				crc = (crc >> 1);  
		}  
	}
	return ~crc;
}

int gdctl_data_init()
{
	FILE *fp;
	char buff[128] = {0};
	
    struct gdctllist *tmp;
    GD_INFO gd_info = {0};
    
    fp = fopen(GPSCTL_DEFAULT_CONFIG,"r");
    if(fp == NULL){
    	GDCTL_LOG(LOG_ERR,"fopen %s failed\n",GPSCTL_DEFAULT_CONFIG);  
    	return -1;
    }

    fgets(buff,128,fp);
    buff[strlen(buff)-1] = '\0';
    
    sscanf(buff,"%lf;%lf;%[^;];%lf",
    		&gpsserinfo.longitude,&gpsserinfo.latitude,gpsserinfo.ip,&gpsserinfo.radian);
    
    GDCTL_LOG(LOG_INFO,"get log file config: %lf %lf %s %lf\n",
					gpsserinfo.longitude,gpsserinfo.latitude,gpsserinfo.ip,gpsserinfo.radian);
    
    if(access(GPSCTL_PUT_LOG_FILE,R_OK)!=0){
    	GDCTL_LOG(LOG_INFO,"touch "GPSCTL_PUT_LOG_FILE"\n"); 
		system("touch "GPSCTL_PUT_LOG_FILE);
	}
    
    INIT_LIST_HEAD(&mylist.list);
    
    while(fgets(buff,128,fp)){
    	buff[strlen(buff)-1] = '\0';
    	
    	if(0 != strcmp(buff,"")){
    		sscanf(buff,"index%d;%d;%[^;];%[^;];%d;%lf;%lf;%d;%d;",
    				&gd_info.index,&gd_info.cid,gd_info.name,gd_info.ip,&gd_info.position,
    				&gd_info.longitude,&gd_info.latitude,&gd_info.sid,&gd_info.sstat);
    		
    		tmp= (struct gdctllist *)malloc(sizeof(struct gdctllist));
    		memcpy(&tmp->gdinfo,&gd_info,sizeof(GD_INFO));
    		//list_add(&(tmp->list), &(mylist.list));
        	list_add_tail(&tmp->list,&mylist.list);
    	}
    	memset(buff,0,sizeof(buff));
    }
    
	list_for_each_entry(tmp, &mylist.list, list){
		GDCTL_LOG(LOG_INFO,"list data: %d %d %s %s %d %f %f %d %d \n",
						tmp->gdinfo.index,tmp->gdinfo.cid,tmp->gdinfo.name,tmp->gdinfo.ip,
						tmp->gdinfo.position,tmp->gdinfo.latitude,tmp->gdinfo.longitude,
						tmp->gdinfo.sid,tmp->gdinfo.sstat);
    }
	fclose(fp);
	
	return 0;	
}

int gdctl_data_free()
{
	struct list_head *pos,*n;
    struct gdctllist *p;

    list_for_each_safe(pos,n,&mylist.list)
    {
        list_del(pos);
        p=list_entry(pos,struct gdctllist,list);
        
        GDCTL_LOG(LOG_INFO,"list del: %d %d %s %s %d %f %f %d %d \n",
						p->gdinfo.index,p->gdinfo.cid,p->gdinfo.name,p->gdinfo.ip,
						p->gdinfo.position,p->gdinfo.latitude,p->gdinfo.longitude,
						p->gdinfo.sid,p->gdinfo.sstat);
        free(p);
    }
		
	return 0;	
}
/*
double gdctl_data_radian(double d)
{
    return d * PI / 180.0;   //角度1? = π / 180
}
*/
int gdctl_gps_check(GD_INFO stRtGPS, GPS_INFO stGPS2, double iRadius )
{
	double dRealTimeLat = stRtGPS.latitude;	
	double dRealTimelng = stRtGPS.longitude;
	double lat = stGPS2.latitude;
	double lng = stGPS2.longitude;

    double dRealTimeRadLat = dRealTimeLat * PI / 180.0;
    double dRadLat = lat * PI / 180.0;
    double a = dRealTimeRadLat - dRadLat;
    double b = dRealTimelng * PI / 180.0 - lng * PI / 180.0;
    
    double dst = 2 * asin((sqrt(pow(sin(a / 2), 2) + cos(dRealTimeRadLat) * cos(dRadLat) * pow(sin(b / 2), 2) )));
    
    dst = dst * EARTH_RADIUS;
    dst= round(dst * 10000) / 10000;
    
    GDCTL_LOG(LOG_INFO,"default config       : %lf(m)=%lf(km) \n",iRadius,iRadius/1000);
	GDCTL_LOG(LOG_INFO,"real-time distance   : %lf(m)=%lf(km) \n",dst*1000,dst);
    
	if(dst > (iRadius/1000))
		{return -1;}
	else
		{return 0;}
    //return dst;
}

int gdctl_send_msg(GD_INFO gdinfo,struct msginfo msg_send)
{
	int ret;
	int loop = MGS_RECV_DEFAULT_TIME;
	unsigned short crc_check = 0;
	struct msgdata msg_data = {0};
	struct msginfo msg_recv = {{0}};
	
	int    sockfd;
	struct sockaddr_in    servaddr;
	
    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
    	GDCTL_LOG(LOG_ERR,"create socket error  : %s!\n",strerror(errno));
		return -1;
    }
    
	struct timeval timeout={1,0};//1s
    ret=setsockopt(sockfd,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(timeout));
    if(-1 == ret){
    	GDCTL_LOG(LOG_ERR,"setsockopt set send timeout failed  : %s!\n",strerror(errno));
    	close(sockfd);
		return -1;
    }
    ret=setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
	if(-1 == ret){
    	GDCTL_LOG(LOG_ERR,"setsockopt set recv timeout failed  : %s!\n",strerror(errno));
    	close(sockfd);
		return -1;
    }
	
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(4196);
	
    if( inet_pton(AF_INET, gdinfo.ip, &servaddr.sin_addr) <= 0 ){
    	GDCTL_LOG(LOG_ERR,"inet_pton  error for : %s \n",gdinfo.ip);
    	close(sockfd);
		return -1;
    }
    if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0 ){
    	GDCTL_LOG(LOG_ERR,"connect sockt error  : %s!\n",strerror(errno));
    	close(sockfd);
		return -1;
    }
	sleep(1);
	if( send(sockfd, &msg_send, sizeof(struct msginfo), 0) < 0 )
    {
    	GDCTL_LOG(LOG_ERR,"send msg data error : %s!\n",strerror(errno));
    	close(sockfd);
		return -1;
    }
	
	char acCmd[256] = {0};
	snprintf(acCmd,256,"echo '%04d-%02d-%02d %02d:%02d:%02d cross: %d "
						"send gps: %4.6f %4.6f data: %02x %02x %02x %02x %02x\n' >> "
						""GPSCTL_PUT_LOG_FILE,
				gps_realtime_info.D.year,gps_realtime_info.D.month,gps_realtime_info.D.day,
				gps_realtime_info.D.hour,gps_realtime_info.D.minute,gps_realtime_info.D.second,
				gdinfo.cid,gps_realtime_info.latitude,gps_realtime_info.longitude,
				msg_send.data.head,msg_send.data.id,msg_send.data.type,
				msg_send.data.action,msg_send.crc16);
    system(acCmd);
    GDCTL_LOG(LOG_ERR,"write log  : %s\n",acCmd);

	while(loop--){
		//1*3s
        ret = recv(sockfd, &msg_recv, sizeof(struct msginfo),0);
		if(ret < 0 && errno==EAGAIN ){
        	GDCTL_LOG(LOG_ERR,"recv timeout !\n");
			continue;
		}		
		GDCTL_LOG(LOG_INFO,"recv data  : %02x %02x %02x %02x %02x\n",
							msg_recv.data.head,msg_recv.data.id,msg_recv.data.type,
							msg_recv.data.action,msg_recv.crc16);
		
		msg_data.head = msg_recv.data.head;
		msg_data.id = msg_recv.data.id;
		msg_data.type = msg_recv.data.type;
		msg_data.action = msg_recv.data.action;
		
		crc_check = crc16_modbus((unsigned char*)&(msg_data),sizeof(struct msgdata));
		
		GDCTL_LOG(LOG_INFO,"crc chaeck : %02x\n",crc_check);
		
		if( MSG_PROTOCOL_HEAD == msg_recv.data.head && crc_check == msg_recv.crc16 ){
			
			if( (MSG_VOICE_ALARM_RECV == msg_recv.data.type 
				|| MSG_RED_LED_FLASH_RECV == msg_recv.data.type)
				&& MSG_CMD_RECV_STA == msg_recv.data.action ){
					
				memset(acCmd,0,sizeof(acCmd));
				snprintf(acCmd,256,"echo '%04d-%02d-%02d %02d:%02d:%02d cross: %d "
							"recv gps: %4.6f %4.6f data: %02x %02x %02x %02x %02x\n' >> "
							""GPSCTL_PUT_LOG_FILE,
				gps_realtime_info.D.year,gps_realtime_info.D.month,gps_realtime_info.D.day,
				gps_realtime_info.D.hour,gps_realtime_info.D.minute,gps_realtime_info.D.second,
				gdinfo.cid,gps_realtime_info.latitude,gps_realtime_info.longitude,
				msg_recv.data.head,msg_recv.data.id,msg_recv.data.type,
				msg_recv.data.action,msg_recv.crc16);
    			system(acCmd);
    			GDCTL_LOG(LOG_ERR,"write log  : %s\n",acCmd);					
					
				GDCTL_LOG(LOG_INFO,"recv data  : ok !\n");
				close(sockfd);
				return 0;
			}
		}else{
			GDCTL_LOG(LOG_ERR,"crc chaeck : faild ! \n");	
			close(sockfd);
			return -1;
		}
    }
    GDCTL_LOG(LOG_ERR,"recv data  : faild !\n");
    close(sockfd);
    
	return -1;	
}

int gdctl_msg_handle(GD_INFO gdinfo)
{
	int iRet = 0;
	struct msginfo msg_send = {{0}};
	
	//send led
	msg_send.data.head = MSG_PROTOCOL_HEAD;
	msg_send.data.id = gdinfo.sid;
	msg_send.data.type = MSG_RED_LED_FLASH_SEND;
	if(0 ==  gdinfo.sstat){
		msg_send.data.action = MSG_RED_LED_FLASH_OFF;
	}else{
		msg_send.data.action = MSG_RED_LED_FLASH_ON;
	}
	//msg_send.data.action = 0x02;
	msg_send.crc16 = crc16_modbus((unsigned char*)&(msg_send.data),sizeof(struct msgdata));
	//msg_send.crc16 = 0x7883;
	GDCTL_LOG(LOG_INFO,"send led   : [%02x %02x %02x %02x %02x] \n",
						msg_send.data.head,msg_send.data.id,msg_send.data.type,
						msg_send.data.action,msg_send.crc16);
						
	if(0 != gdctl_send_msg(gdinfo,msg_send)){
		GDCTL_LOG(LOG_ERR,"send led   : faild ! \n");
		iRet = 1;	
	}

	//send alarm
	memset(&msg_send,0,sizeof(struct msginfo));
	msg_send.data.head = MSG_PROTOCOL_HEAD;
	msg_send.data.id = gdinfo.sid;
	msg_send.data.type = MSG_VOICE_ALARM_SEND;
	if(0 ==  gdinfo.sstat){
		msg_send.data.action = MSG_VOICE_ALARM_OFF;
	}else{
		msg_send.data.action = MSG_VOICE_ALARM_ON;
	}
	//msg_send.data.action = 0x01;
	msg_send.crc16 = crc16_modbus((unsigned char*)&(msg_send.data),sizeof(struct msgdata));
	//msg_send.crc16 = 0x49C1;
	GDCTL_LOG(LOG_INFO,"send alarm : [%02x %02x %02x %02x %02x] \n",
						msg_send.data.head,msg_send.data.id,msg_send.data.type,
						msg_send.data.action,msg_send.crc16);
	
	if(0 != gdctl_send_msg(gdinfo,msg_send)){
		GDCTL_LOG(LOG_ERR,"send alarm : faild ! \n");
		iRet = 1;	
	}
	
	if(iRet){
		GDCTL_LOG(LOG_ERR,"send staty : led or alarm faild ! \n");
		return -1;
	}
	/*
	//close led
	memset(&msg_send,0,sizeof(struct msginfo));
	msg_send.data.head = MSG_PROTOCOL_HEAD;
	msg_send.data.id = gdinfo.sid;
	msg_send.data.type = MSG_RED_LED_FLASH_SEND;
	if(0 ==  gdinfo.sstat){
		msg_send.data.action = MSG_RED_LED_FLASH_OFF;
	}else{
		msg_send.data.action = MSG_RED_LED_FLASH_ON;
	}
	msg_send.data.action = MSG_RED_LED_FLASH_OFF;
	msg_send.data.action = 0x00;
	msg_send.crc16 = crc16_modbus((unsigned char*)&(msg_send.data),sizeof(struct msgdata));
	GDCTL_LOG(LOG_INFO,"send alarm : [%02x %02x %02x %02x %02x] \n",
						msg_send.data.head,msg_send.data.id,msg_send.data.type,
						msg_send.data.action,msg_send.crc16);
	msg_send.crc16 = 0xB902;
	GDCTL_LOG(LOG_INFO,"send led   : [%02x %02x %02x %02x %02x] \n",
						msg_send.data.head,msg_send.data.id,msg_send.data.type,
						msg_send.data.action,msg_send.crc16);
						
	if(0 != gdctl_send_msg(gdinfo,msg_send)){
		GDCTL_LOG(LOG_ERR,"send led   : faild ! \n");
		return -1;	
	}
	
	//alrem close 
	memset(&msg_send,0,sizeof(struct msginfo));
	msg_send.data.head = MSG_PROTOCOL_HEAD;
	msg_send.data.id = gdinfo.sid;
	msg_send.data.type = MSG_VOICE_ALARM_SEND;
	if(0 ==  gdinfo.sstat){
		msg_send.data.action = MSG_VOICE_ALARM_OFF;
	}else{
		msg_send.data.action = MSG_VOICE_ALARM_ON;
	}
	msg_send.data.action = MSG_VOICE_ALARM_OFF;
	msg_send.data.action = 0x00;
	msg_send.crc16 = crc16_modbus((unsigned char*)&(msg_send.data),sizeof(struct msgdata));
	GDCTL_LOG(LOG_INFO,"send alarm : [%02x %02x %02x %02x %02x] \n",
						msg_send.data.head,msg_send.data.id,msg_send.data.type,
						msg_send.data.action,msg_send.crc16);
	msg_send.crc16 = 0x8900;
	GDCTL_LOG(LOG_INFO,"send alarm : [%02x %02x %02x %02x %02x] \n",
						msg_send.data.head,msg_send.data.id,msg_send.data.type,
						msg_send.data.action,msg_send.crc16);
	
	if(0 != gdctl_send_msg(gdinfo,msg_send)){
		GDCTL_LOG(LOG_ERR,"send alarm : faild ! \n");
		return -1;	
	}
	*/
	
	return 0;
}

int gdctl_data_check(GPS_INFO stGPS2,GD_INFO *pstGdInfo)
{
	struct gdctllist *tmp;
	GD_INFO gdctl_data = {0};
	
	list_for_each_entry(tmp, &mylist.list, list){
		
		GDCTL_LOG(LOG_INFO,"list for search      : %d %d %s %s %d %f %f %d %d\n",
							tmp->gdinfo.index,tmp->gdinfo.cid,tmp->gdinfo.name,
							tmp->gdinfo.ip,tmp->gdinfo.position,tmp->gdinfo.latitude,
							tmp->gdinfo.longitude,tmp->gdinfo.sid,tmp->gdinfo.sstat);
        		
       	gdctl_data.latitude = tmp->gdinfo.latitude;
        gdctl_data.longitude = tmp->gdinfo.longitude;
        
        /*对比实时获取的gps和保存的数据,是否有在配置的距离内*/
        /*无论是发送成功或者已经发送或发送失败,都终止查询对比表数据*/
        if(0 == gdctl_gps_check(gdctl_data,stGPS2,gpsserinfo.radian)){
        	
            GDCTL_LOG(LOG_INFO,"find gps match info  : (previous:index%d )\n",previous_index);
            memcpy(pstGdInfo,&tmp->gdinfo,sizeof(GD_INFO));
            GDCTL_LOG(LOG_INFO,"                       %d %d %s %s %d %f %f %d %d\n",
								pstGdInfo->index,pstGdInfo->cid,pstGdInfo->name,
								pstGdInfo->ip,pstGdInfo->position,pstGdInfo->latitude,
								pstGdInfo->longitude,pstGdInfo->sid,pstGdInfo->sstat);
        	
        	/*判断当前点是否是上次已经发送的点,是:取消 否:发送*/
        	if(previous_index != pstGdInfo->index){
        		if(0 == gdctl_msg_handle(tmp->gdinfo)){
        			previous_index = pstGdInfo->index;
        			GDCTL_LOG(LOG_INFO,"send index : index%d \n",previous_index);
        		}else{
        			GDCTL_LOG(LOG_ERR,"send faild : index%d\n",pstGdInfo->index);
        		}
        	}
        	return 0;
        }
    }
    
	return 0;
}

int gdctl_put_server(char acIp[32])
{
	char acCmd[256] = {0};
	
	snprintf(acCmd,256,"cp -a %s %s_%04d%02d%02d_%02d%02d",
				GPSCTL_PUT_LOG_FILE,GPSCTL_PUT_LOG_FILE,
				gps_realtime_info.D.year,gps_realtime_info.D.month,gps_realtime_info.D.day,
				gps_realtime_info.D.hour,gps_realtime_info.D.minute);
	system(acCmd);
	GDCTL_LOG(LOG_ERR,"[log file ftpput to server] %s \n",acCmd);
	
	memset(acCmd,0,sizeof(acCmd));
	//system("killall -9 ftpput");
	snprintf(acCmd,256,"ftpput_sh /mnt/emmc/packet %s gdctllog_%04d%02d%02d_%02d%02d "
						"> /dev/null 2>&1 &",
				acIp,gps_realtime_info.D.year,gps_realtime_info.D.month,gps_realtime_info.D.day,
				gps_realtime_info.D.hour,gps_realtime_info.D.minute);
	system(acCmd);
	GDCTL_LOG(LOG_ERR,"[log file ftpput to server] %s \n",acCmd);
	
	memset(acCmd,0,sizeof(acCmd));
	snprintf(acCmd,256,"echo '' > %s",GPSCTL_PUT_LOG_FILE);
	system(acCmd);
	GDCTL_LOG(LOG_ERR,"[log file ftpput to server] %s \n",acCmd);
	GDCTL_LOG(LOG_INFO,"[log file ftpput to server] ok !!! \n");
	
	return 0;	
}

int gdctl_log_check(GPS_INFO stGpsinfo)
{
	GD_INFO gdctl_data = {0};
	
	GDCTL_LOG(LOG_INFO,"\n");
	gdctl_data.longitude = gpsserinfo.longitude;
	gdctl_data.latitude = gpsserinfo.latitude;
	GDCTL_LOG(LOG_INFO,"log file put status  : %d (1:uploaded 0:not uploaded)\n",log_upload_status);

	if(0 == gdctl_gps_check(gdctl_data,stGpsinfo,gpsserinfo.radian)  ){
		if(0 == log_upload_status){
			gdctl_put_server(gpsserinfo.ip);
			log_upload_status = 1;
		}
	}else{
		log_upload_status = 0;
	}
	
	return 0;
}

int main(int argc, char **argv)
{
	
	int shmid;
	void *shm = NULL;
	int shmup = 0;
	
    SHARED_MEM *shared;
    
    struct timezone tz;
    struct timeval tv;
    //struct stTimeout;
    
	GPS_INFO gpsinfo = {{0}};
	GD_INFO gdctl_data = {0};
	
    shmid = shmget((key_t)1234,sizeof(SHARED_MEM), 0666|IPC_CREAT);  
    if(shmid == -1)  
    {  
    	GDCTL_LOG(LOG_ERR,"shmget failed !\n");
        exit(EXIT_FAILURE);
    }  
    shm = shmat(shmid, 0, 0);
    if(shm == (void*)-1)  
    {  
    	GDCTL_LOG(LOG_ERR,"shmat failed !\n");
        exit(EXIT_FAILURE);
    }  
    shared = (SHARED_MEM *)shm;  
    shared->written = 0; 
    
    GDCTL_LOG(LOG_CRIT,"Memory shmat attached at 0x%X start ...\n",(int)shm); 
 	      
	//stTimeout.tv_sec = 0;
	//stTimeout.tv_usec=25*1000L;//1s=1000ms=1000*1000us
	
 	if(0 != gdctl_data_init()){
 		GDCTL_LOG(LOG_ERR,"gdctl data init failed !\n");
 		return -1;
 	}
 	
 	GDCTL_LOG(LOG_INFO,"===============start================\n\n\n");
	while(1)
    {  
        if(shared->written != 0)
        {
        	GDCTL_LOG(LOG_INFO,"=============gps handle=============\n");
        	gettimeofday(&tv,&tz);
            GDCTL_LOG(LOG_INFO,"time start : %ld(s)-%ld(us)\n",tv.tv_sec,tv.tv_usec);
            
            memcpy(&gpsinfo, &shared->gpsinfo, sizeof(GPS_INFO)); 
            memcpy(&gps_realtime_info, &shared->gpsinfo, sizeof(GPS_INFO)); 
            
            //test longitude latitude
            //gpsinfo.longitude = 121.2700643;
            //gpsinfo.latitude = 31.0150051;
            //gpsinfo.longitude = 1;
            //gpsinfo.latitude = 1;
            GDCTL_LOG(LOG_INFO,"get gpsinfo:\n");
            GDCTL_LOG(LOG_INFO,"date       : %04d-%02d-%02d\n",
            					gpsinfo.D.year,gpsinfo.D.month,gpsinfo.D.day);
            GDCTL_LOG(LOG_INFO,"time       : %02d:%02d:%02d\n",
            					gpsinfo.D.hour,gpsinfo.D.minute,gpsinfo.D.second);
            GDCTL_LOG(LOG_INFO,"longitude  : %4.6f %c\n",gpsinfo.longitude,gpsinfo.EW);
            GDCTL_LOG(LOG_INFO,"latitude   : %4.6f %c\n",gpsinfo.latitude,gpsinfo.NS);
            GDCTL_LOG(LOG_INFO,"high       : %4.4f\n",gpsinfo.high);
            GDCTL_LOG(LOG_INFO,"status     : %c\n",gpsinfo.status);
            
            //当前GPS经度纬度在默认50m范围内 
            if(0 == gdctl_data_check(gpsinfo,&gdctl_data)){
            	memset(&gdctl_data,0,sizeof(GD_INFO));
            }
            //检查是否上传gps log服务器
            gdctl_log_check(gpsinfo);
            
            //共享内存读写状态,可写
            shared->written = 0; 
            shmup = 0;	//仅作标记log 输出
            gettimeofday(&tv,&tz);
            GDCTL_LOG(LOG_INFO,"time stop  : %ld(s)-%ld(us)\n\n\n",tv.tv_sec,tv.tv_usec);
        }else{
        	if(0 == shmup){
        		GDCTL_LOG(LOG_INFO,"shared memory no data updated !\n\n");
        		shmup = 1;
        	}
            usleep(500*1000);
        }
    }  
    
	if(shmdt(shm) == -1)  
    {  
    	GDCTL_LOG(LOG_ERR,"shmdt free failed !\n");  
        exit(EXIT_FAILURE);  
    }  
    if(shmctl(shmid, IPC_RMID, 0) == -1)  
    {  
    	GDCTL_LOG(LOG_ERR,"shmctl(IPC_RMID) uninstall failed !\n");  
        exit(EXIT_FAILURE);  
    }  
    
	gdctl_data_free();
	
	GDCTL_LOG(LOG_INFO,"=============end=============\n");
	
	return 0;	
}

