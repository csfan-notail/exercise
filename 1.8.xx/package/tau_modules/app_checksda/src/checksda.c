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

void mount_disk()
{
	system("mkdir -p /mnt/emmc/logsave");
	system("mount -t ext4 -rw /dev/sda1  /mnt/emmc/logsave");
	system("mkdir -p /mnt/emmc/packet");
	system("mount -t ext4 -rw /dev/sda2  /mnt/emmc/packet");
	//system("mkdir -p /mnt/emmc/modulelogsave");
	//system("mount -t ext4 -rw /dev/sda3  /mnt/emmc/modulelogsave");
}

int main()
{
	FILE *fp;	
	char *pcCmdDisDir = "ls -l /mnt/emmc";
	char acNeedleSda1[]="logsave";
	char acNeedleSda2[]="packet";
	//char acNeedleSda3[]="modulelogsave";

	char *pcCmdDisSda1 = "df | grep sda1";
	char *pcCmdDisSda2 = "df | grep sda2";
	//char *pcCmdDisSda3 = "df | grep sda3";

	char aucDate[256] = {0};

	while ( 1 )
	{
		if ((fp=popen(pcCmdDisDir,"r"))==NULL)
		{
			syslog(3," sda pcCmdDis failed\n");
			sleep(3);
			continue;
		}
		while (fgets(aucDate,sizeof(aucDate),fp) != NULL)
		{
			//保证系统可配置文件，出现这种情况，文件大概率可能
			//需要格式化
			if (strstr(aucDate,acNeedleSda1)!=NULL)
			{
				 printf("rm -rf /mnt/emmc/logsave  \n");
				 system("rm -rf /mnt/emmc/*");
			}
			else if (strstr(aucDate,acNeedleSda2)!=NULL)
			{
				printf("rm -rf /mnt/emmc/packet  \n");
				system("rm -rf /mnt/emmc/*");
			}
			//else if(strstr(aucDate,acNeedleSda3)!=NULL)
			//{
			//	printf("rm -rf /mnt/emmc/modulelogsave  \n");
			//	system("rm -rf /mnt/emmc/*");
			//}
			else
			{
				printf("rm -rf /mnt/emmc/xxxxx  \n");
			}
			memset(aucDate,0,sizeof(aucDate));

 		}
		pclose(fp);
		fp = NULL;
		//挂在盘
		mount_disk();

		//sda1 check
		memset(aucDate,0,sizeof(aucDate));
		//检查是否挂在上
		if ((fp=popen(pcCmdDisSda1,"r"))==NULL)
		{
			syslog(3," check sda1  default\n");
		}
		else
		{
			if (fgets(aucDate,sizeof(aucDate),fp) != NULL)
			{
				if (strlen(aucDate) != 0)
				{
					syslog(3,"sd1 mount ok!\n");
				}
				else
				{
					system("mkfs.ext4 /dev/sda1 ");
					system("mount -t ext4 -rw /dev/sda1  /mnt/emmc/logsave");
				}
			}
			else
			{
				system("mkfs.ext4 /dev/sda1 ");
				system("mount -t ext4 -rw /dev/sda1  /mnt/emmc/logsave");
			}
			pclose(fp);
			fp = NULL;

		}
		//sda2 check
		memset(aucDate,0,sizeof(aucDate));
		//检查是否挂在上
		if ((fp=popen(pcCmdDisSda2,"r"))==NULL)
		{
			syslog(3," check sda2  default\n");
		}
		else
		{
			if (fgets(aucDate,sizeof(aucDate),fp) != NULL)
			{
				if (strlen(aucDate) != 0)
				{
					syslog(3,"sd2 mount ok!\n");
				}
				else
				{
					system("mkfs.ext4 /dev/sda2 ");
					system("mount -t ext4 -rw /dev/sda2  /mnt/emmc/packet");
				}
			}
			else
			{
				system("mkfs.ext4 /dev/sda2 ");
				system("mount -t ext4 -rw /dev/sda2  /mnt/emmc/packet");
			}

			pclose(fp);
			fp = NULL;

		}
		/*
		//sda3 check
		memset(aucDate,0,sizeof(aucDate));
		//检查是否挂在上
		if ((fp=popen(pcCmdDisSda3,"r"))==NULL)
		{
			syslog(3," check sda3  default\n");
		}
		else
		{
			if (fgets(aucDate,sizeof(aucDate),fp) != NULL)
			{
				if (strlen(aucDate) != 0)
				{
					syslog(3,"sd3 mount ok!\n");
				}
				else
				{
					system("mkfs.ext4 /dev/sda3 ");
					system("mount -t ext4 -rw /dev/sda3  /mnt/emmc/modulelogsave");
				}
			}
			else
			{
				system("mkfs.ext4 /dev/sda3 ");
				system("mount -t ext4 -rw /dev/sda3  /mnt/emmc/modulelogsave");
			}

			pclose(fp);
			fp = NULL;

		}*/

		return 0;


	}

}
