#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>  
#include<sys/types.h>  
#include<sys/stat.h>  
#include<fcntl.h>  
#include<unistd.h>  
#include<termios.h>  
#include<string.h>  
#include<sys/shm.h>
#include<sys/time.h>
#include<sys/select.h>
#include<math.h>
#include "ap_types.h"
#include "gps.h"

int open_dev(char *dev)
{
    int fd = open( dev, O_RDWR| O_NDELAY );         //| O_NOCTTY | O_NDELAY   
    if (-1 == fd)
    {
        perror("Can't Open Serial Port !");
        return -1;
    }
    else
 
    return fd;
}

int init_serial(int fd,int nSpeed, int nBits, char nEvent, int nStop)  
{  
    struct termios newtio,oldtio;  
    if(tcgetattr( fd,&oldtio)  !=  0) {   
        perror("SetupSerial 1");  
        return -1;  
    }  
    bzero( &newtio, sizeof( newtio ) );  
    newtio.c_cflag  |=  CLOCAL | CREAD;  
    newtio.c_cflag &= ~CSIZE;  
  
    switch( nBits )  
    {  
    case 7:  
        newtio.c_cflag |= CS7;  
        break;  
    case 8:  
        newtio.c_cflag |= CS8;  
        break;  
    }  
  
    switch( nEvent )  
    {  
    case 'O':  
        newtio.c_cflag |= PARENB;  
        newtio.c_cflag |= PARODD;  
        newtio.c_iflag |= (INPCK | ISTRIP);  
        break;  
    case 'E':   
        newtio.c_iflag |= (INPCK | ISTRIP);  
        newtio.c_cflag |= PARENB;  
        newtio.c_cflag &= ~PARODD;  
        break;  
    case 'N':    
        newtio.c_cflag &= ~PARENB;  
        break;  
    }  
  
    switch( nSpeed )  
    {  
    case 2400:  
        cfsetispeed(&newtio, B2400);  
        cfsetospeed(&newtio, B2400);  
        break;  
    case 4800:  
        cfsetispeed(&newtio, B4800);  
        cfsetospeed(&newtio, B4800);  
        break;  
    case 9600:  
        cfsetispeed(&newtio, B9600);  
        cfsetospeed(&newtio, B9600);  
        break;  
    case 115200:  
        cfsetispeed(&newtio, B115200);  
        cfsetospeed(&newtio, B115200);  
        break;  
    case 460800:  
        cfsetispeed(&newtio, B460800);  
        cfsetospeed(&newtio, B460800);  
        break;  
    default:  
        cfsetispeed(&newtio, B9600);  
        cfsetospeed(&newtio, B9600);  
        break;  
    }  
    if( nStop == 1 )  
	{
	    newtio.c_cflag &=  ~CSTOPB;
	}
    else if ( nStop == 2 )  
	{
		newtio.c_cflag |=  CSTOPB;
	}
    newtio.c_cc[VTIME]  = 0;
    newtio.c_cc[VMIN] = 100;
    tcflush(fd,TCIFLUSH);  
    if((tcsetattr(fd,TCSANOW,&newtio))!=0)  
    {  
        perror("com set error");  
        return -1;  
    }  
    
    return 0;  
}  


char * get_gprmc (char * pBuf)
{
    char *pBuff=pBuf;
    char *pTarget="$GNRMC";
    char *p=NULL;
                
    if((p=strstr(pBuff,pTarget))==NULL)
    {
        GPS_LOG(LOG_INFO,"No fonud the string GPRMC\n");
        return NULL;
    }


    return p;
}


char * get_gpgga (char * pBuf)
{
    char *pBuff=pBuf;
    char *pTarget="$GNGGA";
    char *p=NULL;
                
    if((p=strstr(pBuff,pTarget))==NULL)
    {
        GPS_LOG(LOG_INFO,"No fonud the string GPGGA\n");
        return NULL;
    }


    return p;
}


static int getcomma(int iNum,char *pStr)
{
    int i,j=0;
    int len=strlen(pStr);
    for(i=0;i<len;i++)
    {
        if(pStr[i]==',')j++;
        if(j==iNum)
		{
			return i+1; 
		}
    }
    return 0;
}


static double get_double_number(char *pBuf)
{
    char acBuf[BUF_SIZE];
    int i;
    double rev;
    i=getcomma(1,pBuf);   
    strncpy(acBuf,pBuf,i);
    acBuf[i]=0;         
    rev=atof(acBuf);   
    return rev;
}


static void UTC2BTC(date_time *pstGPS)  
{  
        pstGPS->second++; 
        if(pstGPS->second>59){  
            pstGPS->second=0;  
            pstGPS->minute++;  
            if(pstGPS->minute>59){  
                pstGPS->minute=0;  
                pstGPS->hour++;  
            }  
        }     
  
        pstGPS->hour+=8;        
        if(pstGPS->hour>23)  
        {  
            pstGPS->hour-=24;  
            pstGPS->day+=1;  
            if(pstGPS->month==2 ||pstGPS->month==4 ||pstGPS->month==6 ||pstGPS->month==9 ||pstGPS->month==11 ){  
                if(pstGPS->day>30){          
                    pstGPS->day=1;  
                    pstGPS->month++;  
                }  
            }  
            else{  
                if(pstGPS->day>31){          
                    pstGPS->day=1;  
                    pstGPS->month++;  
                }  
            }  
            if(pstGPS->year % 4 == 0 ){ 
                if(pstGPS->day > 29 && pstGPS->month ==2){       
                    pstGPS->day=1;  
                    pstGPS->month++;  
                }  
            }  
            else{  
                if(pstGPS->day>28 &&pstGPS->month ==2){      
                    pstGPS->day=1;  
                    pstGPS->month++;  
                }  
            }  
            if(pstGPS->month>12){  
                pstGPS->month-=12;  
                pstGPS->year++;  
            }         
        }  
} 


void gps_parse(char *line1,char *line2,GPS_INFO *pstGPS)  
{  
    int i,tmp,start,end;  
    char* acBuf=line1;  
    char* buff=line2;  


    pstGPS->D.hour   =(acBuf[ 7]-'0')*10+(acBuf[ 8]-'0');  
    pstGPS->D.minute =(acBuf[ 9]-'0')*10+(acBuf[10]-'0');  
    pstGPS->D.second =(acBuf[11]-'0')*10+(acBuf[12]-'0');  
    
    tmp = getcomma(9,acBuf);      
    pstGPS->D.day    =(acBuf[tmp+0]-'0')*10+(acBuf[tmp+1]-'0');  
    pstGPS->D.month  =(acBuf[tmp+2]-'0')*10+(acBuf[tmp+3]-'0');  
    pstGPS->D.year   =(acBuf[tmp+4]-'0')*10+(acBuf[tmp+5]-'0')+2000;  
    
    pstGPS->status   =acBuf[getcomma(2,acBuf)];    
    pstGPS->latitude =get_double_number(&acBuf[getcomma(3,acBuf)])/100; 
    pstGPS->NS       =acBuf[getcomma(4,acBuf)];            
    pstGPS->longitude=get_double_number(&acBuf[getcomma(5,acBuf)])/100; 
    pstGPS->EW       =acBuf[getcomma(6,acBuf)];         
    
    UTC2BTC(&pstGPS->D);                    
 
    pstGPS->high     = get_double_number(&buff[getcomma(9,buff)]);   
} 


void show_gps(GPS_INFO *pstGPS)  
{   
    GPS_LOG(LOG_INFO,"\n");
    GPS_LOG(LOG_INFO,"DATE     : %ld-%02d-%02d \n",pstGPS->D.year,pstGPS->D.month,pstGPS->D.day);  
    GPS_LOG(LOG_INFO,"TIME     : %02d:%02d:%02d \n",pstGPS->D.hour,pstGPS->D.minute,pstGPS->D.second);  
    GPS_LOG(LOG_INFO,"Latitude : %4.6f %c\n",pstGPS->latitude,pstGPS->NS);     
    GPS_LOG(LOG_INFO,"Longitude: %4.6f %c\n",pstGPS->longitude,pstGPS->EW);    
    GPS_LOG(LOG_INFO,"high     : %4.4f \n",pstGPS->high);      
    GPS_LOG(LOG_INFO,"STATUS   : %c\n",pstGPS->status);     
}  

/***************************�������***********************************************************/
double radian(double d)
{
    return d * PI / 180.0;   //�Ƕ�1? = �� / 180
}

//�������
double get_distance(GPS_INFO stRtGPS, GPS_INFO stGPS2,double iRadius)
{
	double dRealTimeLat = stRtGPS.latitude;	
	double dRealTimelng = stRtGPS.longitude;
	double lat = stGPS2.latitude;
	double lng = stGPS2.longitude;

    double dRealTimeRadLat = radian(dRealTimeLat);
    double dRadLat = radian(lat);
    double a = dRealTimeRadLat - dRadLat;
    double b = radian(dRealTimelng) - radian(lng);
    
    double dst = 2 * asin((sqrt(pow(sin(a / 2), 2) + cos(dRealTimeRadLat) * cos(dRadLat) * pow(sin(b / 2), 2) )));
    
    dst = dst * EARTH_RADIUS;
    dst= round(dst * 10000) / 10000;
	if(dst > iRadius)
	{
		return -1;
	}
	else
	{
		return dst;
	}
    //return dst;
}
/***************************�������***********************************************************/


struct shared_use_st  
{  
    int written;
    GPS_INFO gpsinfo;
};  

int main(void)  
{  
    int  fd,iSet,iRead,iRet;  
    char acBuf[BUF_SIZE];  	
    char acBuftmp[BUF_SIZE];  
    char *pBuff_gprmc,*pBuff_gpgga;  
    GPS_INFO stGPS;
	fd_set timefd;
	struct timeval timeout;
	timeout.tv_sec = 2;   
    timeout.tv_usec = 0;
	
    fd = open_dev("/dev/ttyUSB20");
	if(-1 == fd)
	{
		GPS_LOG(LOG_INFO,"cat open ttyUSB20!!\n");		
        exit(1);  
	}
     
    iSet = init_serial(fd,115200, 8, 'N', 1);
    if (iSet == -1)  
	{
	    exit(1);
	}
    FD_ZERO(&timefd);  
    FD_SET(fd,&timefd);
    void *shm = NULL;  
    struct shared_use_st *shared = NULL;  
    char buffer[BUFSIZ + 1];
    int shmid;  
 
    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666|IPC_CREAT);  
    if(shmid == -1)  
    {  
        fprintf(stderr, "shmget failed\n");  
        exit(EXIT_FAILURE);  
    }  

    shm = shmat(shmid, (void*)0, 0);  
    if(shm == (void*)-1)  
    {  
        fprintf(stderr, "shmat failed\n");  
        exit(EXIT_FAILURE);  
    }  
    GPS_LOG(LOG_INFO,"Memory attached at %X\n", (int)shm);  

    shared = (struct shared_use_st*)shm;  
  	shared->written = 0;
    while(1)  
    {   

		memset(acBuf,0,BUF_SIZE);   

		 while(1)
    	{
    	
			timeout.tv_sec = 2;   
			timeout.tv_usec = 0;
	    	iRet = select(fd+1, &timefd, (fd_set *)0, (fd_set *)0, &timeout);
	        if(0 == iRet)
			{
			
				GPS_LOG(LOG_INFO, "select timeout!!\n");
				continue;
			}
			memset(acBuftmp,0,BUF_SIZE);   
	        iRead = read(fd, acBuftmp, BUF_SIZE);
			sprintf(acBuf,"%s%s",acBuf,acBuftmp);
			
            //GPS_LOG(LOG_INFO, "stGPS information as follow:\n\n%s\n", acBuf);
			iRet = strlen(acBuf);
            GPS_LOG(LOG_INFO, "strlen(acBuf):%d\n", iRet);
			if(600 <= iRet)
			{
				break;
			}
    	}
        GPS_LOG(LOG_INFO,"\nGPS DATALen=%d\n",iRet);   
        acBuf[iRet] = '\0';  
        GPS_LOG(LOG_INFO, "stGPS information as follow:\n\n%s\n", acBuf); //杈撳嚭鎵€璇绘暟鎹? 
        
        //GPS_LOG(LOG_INFO, "stGPS information as follow:\n\n\n"); //杈撳嚭鎵€璇绘暟鎹? 
        
       
        pBuff_gprmc=get_gprmc(acBuf);
        pBuff_gpgga=get_gpgga(acBuf);

		if((NULL != pBuff_gprmc)&&(NULL != pBuff_gpgga))
		{
	        gps_parse(pBuff_gprmc,pBuff_gpgga,&stGPS);
	        show_gps(&stGPS) ;
		}
		
        if(shared->written == 0)  
        {  
	    	memcpy(&shared->gpsinfo, &stGPS, sizeof(GPS_INFO));  
        	shared->written = 1;  
		}

    }  
  	
  	if(shmdt(shm) == -1)  
    {  
        fprintf(stderr, "shmdt failed\n");  
        exit(EXIT_FAILURE);  
    } 
  	
    close(fd);  
    return 0;  
}  
