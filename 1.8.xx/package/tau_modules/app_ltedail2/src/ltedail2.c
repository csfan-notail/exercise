#include <stdio.h>
#include <sys/select.h>
#include <termios.h>
#include <sys/time.h>
#include <fcntl.h>
#include <string.h>  
#include <sys/types.h>  
#include <errno.h>  
#include <sys/stat.h>   
#include <unistd.h>    
#include <stdlib.h>  
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/sysinfo.h>

#define ACMCOM  "/dev/ttyUSB8"
#define COM_DATA_LENGTH 256
#define READ_COM_TIMEOUT 25*1000
#define GET_PRODID_CMD "cat /proc/bus/usb/devices | grep ProdID=14fe | awk '{print$3}'"
#define PRODID "ProdID=14fe\n"
#define TIMEOUT 1
#define DELAY_TIME 60
#define OPEN_DELAY_TIME 15

int inum = 0;
int solf_sim_set(void) 
{
    FILE *file;
    char acline[32];
    file = popen("uci get lte.simswitch.switch2", "r");
    if (NULL != file)
    {
		//printf("uci get success\n");
        while(fgets(acline,32, file) != NULL) 
        {
			//printf("line = %s",acline);
			acline[strlen(acline) -1] = '\0';
			if(0 == strcmp(acline,"0"))
			{
				system("echo -en \"AT^HVSST=11,1\r\n\">/dev/ttyUSB8");
			}
        }
    }
    else
    {
        //printf("no uci\n");
		pclose(file);
        return 1;
    }
    pclose(file);
    return 0;
}

int set_com_attr(int iComFd)
{
    #if 0
    if (fcntl(iComFd, F_SETFL, O_NONBLOCK) < 0)
    {
        printf("%d,%s:Unable set to NONBLOCK mode",__LINE__,__func__);
    }
    #endif

    fcntl(iComFd, F_SETFL, O_NONBLOCK);
    
    struct termios TtyAttr;
    int DeviceSpeed = B115200;
    int ByteBits = CS8;

    memset(&TtyAttr, 0, sizeof(struct termios));
    TtyAttr.c_iflag = IGNPAR;
    TtyAttr.c_cflag = DeviceSpeed | HUPCL | ByteBits | CREAD | CLOCAL;
    TtyAttr.c_cc[VMIN] = 1;

    if (tcsetattr(iComFd, TCSANOW, &TtyAttr) < 0)
    {
        printf("%d,%s:Unable to set comm port",__LINE__,__func__);
        return -1;
    }
    return 0;
}
int writecom_connect(int iComFd)
{
    char acLteCmdConect[] = "AT^NDISDUP=1,1\r";
	
    //char acLteCmdUnlock[] = "AT^DATALOCK=mG1HBjYmeg7a7jh1\r";
    
    int iLteCmdConect = strlen(acLteCmdConect);
    //int iLteCmdUnlock = strlen(acLteCmdUnlock);
    write(iComFd,acLteCmdConect,iLteCmdConect);
    
    usleep(READ_COM_TIMEOUT);
    write(iComFd,acLteCmdConect,iLteCmdConect);

	/*altered by caibing 2016-12-13*/
    usleep(READ_COM_TIMEOUT);
    //write(iComFd,acLteCmdUnlock,iLteCmdUnlock);
	
    //usleep(READ_COM_TIMEOUT);
}
int writecom(int iComFd)
{
	/*添加软sim卡识别  激活软sim卡*/
	//int inum = 0;
	if(0 == inum )
	{
		solf_sim_set();
		inum++;
	}

	
	/******************************/
    char acGereg[] = "AT+CEREG?\r";
    
    int icGereg = strlen(acGereg);
    
    write(iComFd,acGereg,icGereg);
	/*altered by caibing 2016-12-13*/
    usleep(READ_COM_TIMEOUT);
    
}
int readcom_and_analysis(int iComFd)
{
    int iReadLength = 0;
    char acBuffer[COM_DATA_LENGTH] = {0};
    char acNeedleCereg[] = "+CEREG:";
    char *pcTmp = NULL;
    while (1) 
    {
        
        if ((iReadLength = read(iComFd, acBuffer, COM_DATA_LENGTH-1)) <= 0 )
        {
            printf("can't read com\n");
            break;
        }
        acBuffer[iReadLength] = '\0';
    
        printf("get string %s\n",acBuffer);
       
        pcTmp = strstr(acBuffer,acNeedleCereg);
        if(pcTmp)
        {
            printf("get the char is %d\n",pcTmp[10]);
            if (('1' == pcTmp[10])||('5' == pcTmp[10])) // 0: unregistered  1:local registered 2: unregistered,searching
            {											// 3: register refuse  4: unknow  5:roaming registered 
                return 0;
            }
        }
        memset(acBuffer,0,COM_DATA_LENGTH);
        
    }
    return -1;
}

int receive_sys_cmd_return(char *cmd,char *des)
{
	FILE *pStream;
	char acBuf[COM_DATA_LENGTH];

	memset(acBuf,0,sizeof(acBuf));
	pStream = popen(cmd,"r");

	if(pStream == NULL)
	{
		printf("popen fail \n");
		return -1;
	}

	fread(acBuf,sizeof(char),sizeof(acBuf),pStream);
	
    pclose(pStream);
	
    if(strcmp(acBuf,des) == 0)
	{
		return 1;
	}
	else{
		return 0;
	}
}

int time_counter(struct timeval *tv,int time_limmit)
{
	struct timezone tz1;
	struct timeval tv1;
	gettimeofday(&tv1,&tz1);

	if(tv1.tv_sec  > (tv->tv_sec + time_limmit))
	{
		return TIMEOUT;
	}
	else{
		return 0;
	}

}
int main(int argc,char *argv[])
{
	    int iCount = 0;
        int iComFd = 0;
        int iReadCount = 0;
        int iAttachFlag = 0;
		int iCfunCount = 0;
        struct timezone tz,tz_count;
        struct timeval tv,tv_count,stTimeout;        
        stTimeout.tv_sec = 0;
        stTimeout.tv_usec=25*1000L;//1s=1000ms=1000*1000us
        
        fd_set ReadSetFD,WriteSetFD;
        FD_ZERO(&ReadSetFD);
        FD_ZERO(&WriteSetFD);
		
		system("ifconfig usb0 up");
/*
	    while(receive_sys_cmd_return(GET_PRODID_CMD,PRODID) != 1)
        {
            if( iCount < 60 )
            {
	   	        usleep(250000);
                printf("--------------------------------iCount = %d \n",iCount );
	   	        iCount++ ;
            }
            else{
            printf("Time out  iCount > 60");
            return -1;
            }
        }
    
        system("usb_modeswitch -J -v 0x12d1 -p 0x14fe");
*/      
        //Add by:zhanghao  添加打开com口计时，时间为15s
        gettimeofday(&tv_count,&tz_count);        
        while ( 1 ) 
        {
            if(time_counter(&tv_count,OPEN_DELAY_TIME) == TIMEOUT)
            {
                return -1;
            }
            gettimeofday(&tv,&tz);
            printf("time:%d--%d\n",tv.tv_sec,tv.tv_usec);
            if ((iComFd = open(ACMCOM,O_RDWR | O_NOCTTY)) < 0) 
            {
                usleep(READ_COM_TIMEOUT);
                perror("open");
                continue;
            }
            gettimeofday(&tv,&tz);
            printf("open com,time:%d--%d\n",tv.tv_sec,tv.tv_usec);
            break;
        }
        if (set_com_attr(iComFd)< 0)
        {
            close(iComFd);
            return -1;
        }
		gettimeofday(&tv_count,&tz_count);
        while (1)
        {            
        	if(time_counter(&tv_count,DELAY_TIME) == TIMEOUT)
			{
		            //altered caibing
		            close(iComFd);			
			    return -1;
        	}
            if ( 0 == iAttachFlag )
            {
				if(0 == iCfunCount)
				{
					FD_SET(iComFd, &WriteSetFD);
					if (select(iComFd + 1, NULL, &WriteSetFD, NULL, &stTimeout) == 0)
					{
					    printf("write select time out\n");
					    //close(iComFd);
					    //return 0;
					    FD_CLR(iComFd,&WriteSetFD);
					    continue;
					}
					if (FD_ISSET(iComFd, &WriteSetFD)) 
					{
					    gettimeofday(&tv,&tz);
					    printf("write time:%d--%d\n",tv.tv_sec,tv.tv_usec);
					    //writecfuncom(iComFd);
					    FD_CLR(iComFd,&WriteSetFD);
						iCfunCount++;
					    //break;
					}
				}
                while (1)
                {
  	                  //altered by caibing
		  	if(time_counter(&tv_count,DELAY_TIME) == TIMEOUT)
			{
				//altered caibing
				close(iComFd);			
				return -1;
			}
 
	            //altered by caibing
	            stTimeout.tv_sec = 0;
		    stTimeout.tv_usec=25*1000L;//1s=1000ms=1000*1000us
                    FD_SET(iComFd, &WriteSetFD);
                    if (select(iComFd + 1, NULL, &WriteSetFD, NULL, &stTimeout) == 0)
                    {
                        printf("write select time out\n");
                        //close(iComFd);
                        //return 0;
                        FD_CLR(iComFd,&WriteSetFD);
                        continue;
                    }
                    if (FD_ISSET(iComFd, &WriteSetFD)) 
                    {
                        gettimeofday(&tv,&tz);
                        printf("write time:%d--%d\n",tv.tv_sec,tv.tv_usec);
                        writecom(iComFd);
                        FD_CLR(iComFd,&WriteSetFD);
                        break;
                    }
                }             
             }
             else
             {
                writecom_connect(iComFd);
                close(iComFd);
                //printf("connect time:%d--%d\n",tv.tv_sec,tv.tv_usec);
                //writecom(iComFd);
                system("udhcpc -i usb1 -s /etc/udhcpc.script &");
                return 0;
             }
            while (1)
            {
                //altered by caibing
	            stTimeout.tv_sec = 0;
		    stTimeout.tv_usec=25*1000L;//1s=1000ms=1000*1000us
                if (iReadCount > 3)
                {
                    iReadCount = 0;
                    break;
                }
                FD_SET(iComFd, &ReadSetFD);
                if (select(iComFd + 1, &ReadSetFD, NULL, NULL,&stTimeout) == 0)
                {
                    printf("read select time out\n");
                    FD_CLR(iComFd,&ReadSetFD);
                    iReadCount++;
                    continue;
                }

                if (FD_ISSET(iComFd, &ReadSetFD)) 
                {
                    if (0 == readcom_and_analysis(iComFd))
                    {
                        gettimeofday(&tv,&tz);
						
                        iAttachFlag = 1;                         
                    }                    
                }
                FD_CLR(iComFd,&ReadSetFD);
                break;
          }
    }             
             
	  //system("/sbin/busybox udhcpc -i usb0 -s /etc/udhcpc.script &");
      return 0;
}
